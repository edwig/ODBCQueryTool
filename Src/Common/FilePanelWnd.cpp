/* 
    Copyright (C) 2002 Aleksey Kochetov
    Copyright (c) 2006 Edwig Huisman

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
*/

#include "stdafx.h"
#include <Shlwapi.h>
#include <string>
#include <COMMON/ExceptionHelper.h>
#include <Frame/WinException.h>
#include <COMMON/FilePanelWnd.h>
#include <COMMON/WorkbookMDIFrame.h>
#include "COMMON/GUICommandDictionary.h"
#include "COMMON/DocManagerExt.h"
#include "OpenEditorApp.h"
#include "SQLDatabase.h"

#define FPW_OPEN_FILES_TAB  0
#define FPW_EXPLORER_TAB    1
#define FPW_ODBCTREE_TAB    2
#define FPW_TABLETREE_TAB   3
#define FPW_NUM_OF_TAB      4

#define ID_FPW_OPEN_FILES   1000
#define ID_FPW_DRIVES       1001
#define ID_FPW_EXPLORER     1002
#define ID_FPW_FILTER       1003
#define ID_FPW_ODBC         1004
#define ID_FPW_TABLE        1005
#define ID_FPW_TABFILTER    1006

#define ACTIVATE_FILE_TIMER 777
#define ACTIVATE_FILE_DELAY 500

using namespace SQLComponents;

static BOOL GetVolumeName (LPCSTR rootDirectory, CString& volumeName)
{
    const int STRSIZE = 80;
    DWORD volumeSerialNumber;           // volume serial number
    DWORD maximumComponentLength;       // maximum file name length
    DWORD fileSystemFlags;              // file system options
    char  fileSystemNameBuffer[STRSIZE];// file system name buffer

    BOOL volInfo = GetVolumeInformation
    (
        rootDirectory,                  // root directory
        volumeName.GetBuffer(STRSIZE),  // volume name buffer
        STRSIZE,                        // length of name buffer
        &volumeSerialNumber,            // volume serial number
        &maximumComponentLength,        // maximum file name length
        &fileSystemFlags,               // file system options
        fileSystemNameBuffer,           // file system name buffer
        sizeof(fileSystemNameBuffer)    // length of file system name buffer
    );
    volumeName.ReleaseBuffer();
    return volInfo;
}

static LPCSTR MakeVolumeLabel (LPCSTR rootDirectory, CString& volumeLabel)
{
    volumeLabel = rootDirectory;
    volumeLabel.TrimRight('\\');
    
    CString volumeName;
    if (GetVolumeName(rootDirectory, volumeName))
    {
      volumeLabel += "   ";
      volumeLabel += volumeName;
    }
    return volumeLabel;
}

static BOOL IsFolder (const CString& path)
{
    // 16.12.2002 bug fix, file manager context menu fails
    DWORD dwFileAttributes = GetFileAttributes(path);
    if (dwFileAttributes != INVALID_FILE_ATTRIBUTES)
    {
      return (dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? TRUE : FALSE;
    }
    else
    {
      throw new CWinException();
    }
}

///////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CFilePanelWnd, CTabCtrl)

BEGIN_MESSAGE_MAP(CFilePanelWnd, CTabCtrl)
    ON_WM_CREATE()
    ON_WM_DESTROY()
    ON_WM_SIZE()
    ON_WM_KEYDOWN()
    ON_NOTIFY_REFLECT    (TCN_SELCHANGE,    OnTab_SelChange)
    ON_CBN_SETFOCUS      (ID_FPW_DRIVES,    OnDrive_SetFocus)
    ON_CBN_SELCHANGE     (ID_FPW_DRIVES,    OnDrive_SelChange)
    ON_CBN_SETFOCUS      (ID_FPW_FILTER,    OnFilter_SetFocus)
    ON_CBN_SELCHANGE     (ID_FPW_FILTER,    OnFilter_SelChange)
    ON_CBN_KILLFOCUS     (ID_FPW_FILTER,    OnFilter_SelChange) 
    ON_CBN_SETFOCUS      (ID_FPW_TABFILTER, OnTable_SetFocus)
    ON_CBN_KILLFOCUS     (ID_FPW_TABFILTER, OnTable_SelChange)
    ON_NOTIFY(NM_CLICK,   ID_FPW_OPEN_FILES,OnOpenFiles_Click)
    ON_NOTIFY(NM_RCLICK,  ID_FPW_OPEN_FILES,OnOpenFiles_RClick)
    ON_NOTIFY(LVN_KEYDOWN,ID_FPW_OPEN_FILES,OnOpenFiles_KeyDown)
    ON_NOTIFY(NM_DBLCLK,  ID_FPW_EXPLORER,  OnExplorerTree_DblClick)
    ON_NOTIFY(NM_RCLICK,  ID_FPW_EXPLORER,  OnExplorerTree_RClick)
    ON_NOTIFY(NM_RCLICK,  ID_FPW_DRIVES,    OnDrivers_RClick)
    ON_NOTIFY_EX(TTN_NEEDTEXT,0,            OnTooltipNotify)
    ON_WM_TIMER()
    ON_COMMAND(ID_FPW_OPEN,           OnFpwOpen)
    ON_COMMAND(ID_FPW_REFRESH,        OnFpwRefresh)
    ON_COMMAND(ID_FPW_SET_DEF_DIR,    OnFpwSetWorDir)
    ON_COMMAND(ID_FPW_REFRESH_DRIVERS,OnFpwRefreshDrivers)
END_MESSAGE_MAP()


CFilePanelWnd::CFilePanelWnd()
{
    m_isExplorerInitialized = FALSE;
    m_isDrivesInitialized   = FALSE;
    m_isOdbcInitialized     = false;
}

CFilePanelWnd::~CFilePanelWnd()
{
}

void 
CFilePanelWnd::SelectDrive (const CString& path, BOOL force)
{
  if(!m_hWnd)
  {
    m_curDrivePath = path;
  }
  else
  {
    CWaitCursor wait;
    for (int i(0), count((int)m_driverPaths.GetSize()); i < count; i++)
    {
      if (!m_driverPaths.ElementAt(i).CompareNoCase(path)
      && (force || m_driverPaths.ElementAt(i).CompareNoCase(m_curDrivePath)))
      {
        CString volumeName;
        // check driver availability
        if (!force && !GetVolumeName(path, volumeName))
        {
          CString message;
          message.Format("Cannot select drive \"%s\". Drive/device is not available.", (LPCSTR)path);
          AfxMessageBox(message, MB_OK|MB_ICONSTOP);
          SelectDrive(m_curDrivePath, TRUE);
        }
        else
        {
          try
          {
            m_drivesCBox.SetCurSel(i);
            m_curDrivePath = path;
            m_explorerTree.DisplayTree(m_curDrivePath, TRUE);
            m_explorerTree.SetSelPath(m_curDrivePath);
          } 
          catch (CWinException* x) 
          {
            CString message;
            message.Format("Cannot select drive \"%s\". %s", (LPCSTR)path, (LPCSTR)*x);
            AfxMessageBox(message, MB_OK|MB_ICONSTOP);
          }
        }
      }
    }
  }
}

void 
CFilePanelWnd::DisplayFilters(BOOL force,BOOL curOnly)
{
  if(force)
  {
    if(!curOnly)
    {
      m_filterCBox.LockWindowUpdate();
    }
    m_filterCBox.SetRedraw(FALSE);
    m_filterCBox.ResetContent();

    for(int ind = 0; ind < m_filters.GetCount(); ++ind)
    {
      CString text = m_filters.GetAt(ind);
      text.GetBuffer();
      m_filterCBox.AddString(text);
      text.ReleaseBuffer();
    }
    m_filterCBox.SetRedraw(TRUE);
    if(!curOnly)
    {
      m_filterCBox.UnlockWindowUpdate();
    }
  }
}

void CFilePanelWnd::AddToFilters(CString& filter)
{
  for(int ind = 0; ind < m_filters.GetCount(); ++ind)
  {
    CString text = m_filters.GetAt(ind);
    if(!text.Find(filter)  && (text.GetLength() == filter.GetLength()))
    {
      // OK, Already in the list
      return;
    }
  }
  m_filters.Add(filter);
  filter.GetBuffer();
  m_filterCBox.AddString(filter);
  filter.ReleaseBuffer();
}

void CFilePanelWnd::DisplayDrivers (BOOL force, BOOL curOnly)
{
  if (force || !m_isDrivesInitialized)
  {
    CWaitCursor wait;

    if (!curOnly) 
    {
      // 09.03.2003 bug fix, desktop flickers when the program is starting
      m_drivesCBox.LockWindowUpdate(); 
    }
    m_drivesCBox.SetRedraw(FALSE);
    m_drivesCBox.ResetContent();
    char  szDrives[2*1024];
    char* pDrive;

    if (curOnly)
    {
      _CHECK_AND_THROW_(m_curDrivePath.GetLength() < sizeof(szDrives) - 2, 
                        "Current drive path is too long.");
      memset(szDrives, 0, sizeof(szDrives));
      memcpy(szDrives, (LPCSTR)m_curDrivePath, m_curDrivePath.GetLength());
    }
    else
    {
      _CHECK_AND_THROW_(GetLogicalDriveStrings(sizeof(szDrives), szDrives), 
                        "Cannot get logical drive strings.");
    }

    m_driverPaths.RemoveAll();

    COMBOBOXEXITEM item;
    memset(&item, 0, sizeof(item));
    item.mask = CBEIF_TEXT;

    for (pDrive = szDrives; *pDrive; pDrive += strlen(pDrive) + 1, item.iItem++)
    {
      m_driverPaths.Add(pDrive);

      CString volumeLabel;
      item.pszText = (LPSTR)MakeVolumeLabel(pDrive, volumeLabel);

      SHFILEINFO shFinfo;
      if (SHGetFileInfo(pDrive, 0, &shFinfo, sizeof(shFinfo), SHGFI_ICON|SHGFI_SMALLICON))
      {
        item.mask |= CBEIF_IMAGE|CBEIF_SELECTEDIMAGE;
        item.iSelectedImage = item.iImage = shFinfo.iIcon;
        DestroyIcon(shFinfo.hIcon); // we only need the index from the system image list
      }
      m_drivesCBox.InsertItem(&item);
      
      if (m_curDrivePath == pDrive)
      {
        m_drivesCBox.SetCurSel((int)item.iItem);
      }
    }
    m_drivesCBox.SetRedraw(TRUE);
    if (!curOnly) 
    {
      m_drivesCBox.UnlockWindowUpdate();
    }
    m_isDrivesInitialized = !curOnly;
  }
}

void CFilePanelWnd::ChangeTab (int nTab)
{
  _CHECK_AND_THROW_(nTab < FPW_NUM_OF_TAB, "Invalid index of a file manager tab.");

  if (nTab != GetCurSel())
  {
    SetCurSel(nTab);
  }
  m_openFilesList.ShowWindow(nTab == FPW_OPEN_FILES_TAB ? SW_SHOW : SW_HIDE);
  m_drivesCBox.   ShowWindow(nTab == FPW_EXPLORER_TAB   ? SW_SHOW : SW_HIDE);
  m_explorerTree. ShowWindow(nTab == FPW_EXPLORER_TAB   ? SW_SHOW : SW_HIDE);
  m_filterCBox.   ShowWindow(nTab == FPW_EXPLORER_TAB   ? SW_SHOW : SW_HIDE);
  m_odbcTree.     ShowWindow(nTab == FPW_ODBCTREE_TAB   ? SW_SHOW : SW_HIDE);
  m_tableTree.    ShowWindow(nTab == FPW_TABLETREE_TAB  ? SW_SHOW : SW_HIDE);
  m_tableCBox.    ShowWindow(nTab == FPW_TABLETREE_TAB  ? SW_SHOW : SW_HIDE);

  if (nTab == FPW_EXPLORER_TAB && !m_isExplorerInitialized)
  {
    CWaitCursor wait;
    UpdateWindow();

    DisplayDrivers(FALSE, TRUE);
    SelectDrive(m_curDrivePath, TRUE);
    m_isExplorerInitialized = TRUE;
  }
  if (nTab == FPW_ODBCTREE_TAB && !m_isOdbcInitialized)
  {
    OnFpwRefreshOdbc();
    m_isOdbcInitialized = true;
  }
  if (nTab == FPW_TABLETREE_TAB)
  {
    OnTable_SelChange();
  }
  m_tab = nTab;
}

void CFilePanelWnd::ActivateOpenFile ()
{
  POSITION pos = m_openFilesList.GetFirstSelectedItemPosition();

  if (pos)
  {
    LVITEM item;
    memset(&item, 0, sizeof(item));
    item.mask = LVIF_PARAM;
    item.iItem = m_openFilesList.GetNextSelectedItem(pos);

    VERIFY(m_openFilesList.GetItem(&item));
    _ASSERT(AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CWorkbookMDIFrame)));
    _ASSERT(((CMDIChildWnd*)item.lParam)->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)));

    ((CWorkbookMDIFrame*)AfxGetMainWnd())->ActivateChild((CMDIChildWnd*)item.lParam);
  }
}

// FilePanelWnd message handlers

int CFilePanelWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CTabCtrl::OnCreate(lpCreateStruct) == -1)
  {
    return -1;
  }
  InsertItem(FPW_OPEN_FILES_TAB, "Files");
  InsertItem(FPW_EXPLORER_TAB,   "Explorer");
  InsertItem(FPW_ODBCTREE_TAB,   "ODBC");
  InsertItem(FPW_TABLETREE_TAB,  "Object");

  if (!m_openFilesList.Create(
      WS_CHILD|WS_TABSTOP
      |LVS_REPORT|LVS_SORTASCENDING|LVS_NOCOLUMNHEADER
      |LVS_SHOWSELALWAYS|LVS_SINGLESEL, 
      CRect(0, 0, 0, 0), this, ID_FPW_OPEN_FILES))
  {
    TRACE0("Failed to create open files list\n");
    return -1;
  }
  if (!m_drivesCBox.Create(
      WS_CHILD|WS_TABSTOP|CBS_DROPDOWNLIST,
      CRect(0, 0, 0, GetSystemMetrics(SM_CYFULLSCREEN)/2), this, ID_FPW_DRIVES))
  {
    TRACE0("Failed to create drives combo box\n");
    return -1;
  }
  if (!m_tableCBox.Create(
      WS_CHILD|WS_TABSTOP|CBS_DROPDOWN,
      CRect(0, 0, 0, GetSystemMetrics(SM_CYFULLSCREEN)/2), this, ID_FPW_TABFILTER))
  {
    TRACE0("Failed to create table combo box\n");
    return -1;
  }
  if (!m_explorerTree.Create(
      WS_CHILD|WS_TABSTOP
      |TVS_LINESATROOT|TVS_HASBUTTONS|TVS_HASLINES|TVS_SHOWSELALWAYS, 
      CRect(0, 0, 0, 0), this, ID_FPW_EXPLORER))
  {
    TRACE0("Failed to create explorer tree\n");
    return -1;
  }
  if (!m_odbcTree.Create(
      WS_CHILD|WS_TABSTOP
      |TVS_LINESATROOT|TVS_HASBUTTONS|TVS_HASLINES|TVS_SHOWSELALWAYS, 
      CRect(0, 0, 0, 0), this, ID_FPW_ODBC))
  {
    TRACE0("Failed to create odbc tree\n");
    return -1;
  }
  if(!m_tableTree.Create(WS_CHILD|WS_TABSTOP
      |TVS_LINESATROOT|TVS_HASBUTTONS|TVS_HASLINES|TVS_SHOWSELALWAYS, 
     CRect(0,0,0,0), this, ID_FPW_TABLE))
  {
    TRACE0("Failed to create table tree\n");
    return -1;
  }
  if (!m_filterCBox.Create(
        WS_CHILD|WS_TABSTOP|CBS_DROPDOWN,
        CRect(0, 0, 0, GetSystemMetrics(SM_CYFULLSCREEN)/2), this, ID_FPW_FILTER))
  {
    TRACE0("Failed to create filter combo box\n");
    return -1;
  }
  EnableToolTips(TRUE);
  m_filterCBox.EnableToolTips(TRUE);

  // insert a column
  m_openFilesList.InsertColumn(0, (LPCSTR)NULL);
  m_openFilesList.SetExtendedStyle(m_openFilesList.GetExtendedStyle()|LVS_EX_FULLROWSELECT);

  ModifyStyleEx(0, WS_EX_CONTROLPARENT, 0);
  m_openFilesList.ModifyStyleEx(0, WS_EX_CLIENTEDGE, 0);
  m_explorerTree. ModifyStyleEx(0, WS_EX_CLIENTEDGE, 0);
  m_odbcTree.     ModifyStyleEx(0, WS_EX_CLIENTEDGE, 0);
  m_tableTree.    ModifyStyleEx(0, WS_EX_CLIENTEDGE, 0);

  m_drivesCBox.   SetImageList(&m_explorerTree.GetSysImageList());
  m_openFilesList.SetImageList(&m_explorerTree.GetSysImageList(), LVSIL_SMALL);

  m_explorerStateImageList.Create(IDB_OE_EXPLORER_STATE_LIST, 16, 64, RGB(0,0,255));
  m_explorerTree.SetImageList(&m_explorerStateImageList, TVSIL_STATE);

  // Reset table tree
  m_tableTree.ClearTree();

  // load saved tab
  ChangeTab(FPW_EXPLORER_TAB);
  
  m_drivesRClick.SubclassWindow(::GetWindow(m_drivesCBox.m_hWnd, GW_CHILD));
  m_filterRClick.SubclassWindow(::GetWindow(m_filterCBox.m_hWnd, GW_CHILD));

  return 0;
}

BOOL
CFilePanelWnd::OnTooltipNotify(UINT wnd,NMHDR* tooltip,LRESULT* res)
{
  ((TOOLTIPTEXT*)tooltip)->szText[0] = 0;
  if(wnd == (UINT)(DWORD_PTR) m_filterCBox.m_hWnd)
  {
    strcpy(((TOOLTIPTEXT*)tooltip)->szText,"Filter for extensions of files in the explorer");
    return (BOOL)(*res = true);
  }
  return (BOOL)(*res = false);
}

VOID CFilePanelWnd::OnDestroy ()
{
  // for Win95 compatibility
  m_openFilesList.DestroyWindow();
  m_explorerTree. DestroyWindow();
  m_drivesCBox.   DestroyWindow(); 
  m_odbcTree.     DestroyWindow();
  m_tableTree.    DestroyWindow();
  m_filterCBox.   DestroyWindow();
  m_tableCBox.    DestroyWindow();
  m_drivesRClick. DestroyWindow();
  m_filterRClick. DestroyWindow();

  CTabCtrl::OnDestroy();
}

void CFilePanelWnd::OnSize(UINT nType, int cx, int cy)
{
  CTabCtrl::OnSize(nType, cx, cy);

  if (cx > 0 && cy > 0 && nType != SIZE_MAXHIDE && nType != SIZE_MINIMIZED) 
  {
    CRect rect;
    GetClientRect(&rect);
    AdjustRect(FALSE, &rect);
    rect.InflateRect(3, 0);
    
    CRect comboRect;
    m_drivesCBox.GetWindowRect(&comboRect);
    int comboH = comboRect.Height() + 2;

    HDWP hdwp = ::BeginDeferWindowPos(10);
    ::DeferWindowPos(hdwp, m_openFilesList, 0, rect.left, rect.top, 
        rect.Width(), rect.Height(), SWP_NOZORDER);
    ::DeferWindowPos(hdwp, m_drivesCBox, 0, rect.left, rect.top, 
        rect.Width(), rect.Height()/2, SWP_NOZORDER);
    ::DeferWindowPos(hdwp, m_odbcTree,   0, rect.left, rect.top,
        rect.Width(), rect.Height(), SWP_NOZORDER);
    ::DeferWindowPos(hdwp, m_tableTree,  0, rect.left, rect.top,
        rect.Width(), rect.Height(), SWP_NOZORDER);
    ::DeferWindowPos(hdwp, m_explorerTree, 0, rect.left, 
      rect.top + comboH, rect.Width(), rect.Height() - (2 * comboH), SWP_NOZORDER);
    ::DeferWindowPos(hdwp, m_filterCBox, 0, rect.left, 
       rect.top + rect.Height() - comboH + 2, rect.Width(), rect.Height()/2, SWP_NOZORDER);
    ::DeferWindowPos(hdwp, m_tableCBox, 0, rect.left, rect.top, 
        rect.Width(), rect.Height()/2, SWP_NOZORDER);
    ::DeferWindowPos(hdwp, m_tableTree, 0, rect.left,
      rect.top + comboH, rect.Width(), rect.Height() - comboH, SWP_NOZORDER);
    ::EndDeferWindowPos(hdwp);

    m_openFilesList.GetClientRect(rect);
    m_openFilesList.SetColumnWidth(0, rect.Width());
  }
}

void CFilePanelWnd::OnTab_SelChange (NMHDR*, LRESULT *pResult)
{
  ChangeTab(GetCurSel());
  *pResult = 0;
}


// move the following code to refresh command handler
void CFilePanelWnd::OnDrive_SetFocus ()
{
  DisplayDrivers();
}

void CFilePanelWnd::OnDrive_SelChange ()
{
  int inx = m_drivesCBox.GetCurSel();
  _ASSERTE(inx < m_driverPaths.GetSize());

  if (inx < m_driverPaths.GetSize())
  {
    SelectDrive(m_driverPaths.ElementAt(inx), FALSE);
  }
}

void CFilePanelWnd::OnFilter_SetFocus()
{
  DisplayFilters();
}

void CFilePanelWnd::OnFilter_SelChange()
{
  int inx = m_filterCBox.GetCurSel();
  if(inx != CB_ERR)
  {
    m_filterCBox.SetCurSel(inx);
  }
  CString filter;
  m_filterCBox.GetWindowText(filter);
  AddToFilters(filter);

  m_explorerTree.SetFilter(filter);
  OnFpwRefresh();
}

void CFilePanelWnd::OpenFiles_Append (LVITEM& item)
{
  item.iItem = m_openFilesList.GetItemCount();
  VERIFY(m_openFilesList.InsertItem(&item) != -1);
}


void CFilePanelWnd::OpenFiles_UpdateByParam (LPARAM param, LVITEM& item)
{
  int nItem = OpenFiles_FindByParam(param);

  ASSERT(nItem != -1);

  if (nItem != -1)
  {
    // sorting does not work for update so ...
    VERIFY(m_openFilesList.DeleteItem(nItem));
    VERIFY(m_openFilesList.InsertItem(&item) != -1);
  }
}

void CFilePanelWnd::OpenFiles_RemoveByParam (LPARAM param)
{
  int nItem = OpenFiles_FindByParam(param);
  ASSERT(nItem != -1);
  if (nItem != -1)
  {
    VERIFY(m_openFilesList.DeleteItem(nItem));
  }
}

void CFilePanelWnd::OpenFiles_ActivateByParam (LPARAM param)
{
  int nItem = OpenFiles_FindByParam(param);
  ASSERT(nItem != -1);
  if (nItem != -1)
  {
    VERIFY(m_openFilesList.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED));
  }
}


LPARAM CFilePanelWnd::OpenFiles_GetCurSelParam ()
{
  int nItem = m_openFilesList.GetNextItem(-1, LVNI_SELECTED);
  ASSERT(nItem != -1);

  if (nItem != -1)
  {
    LVITEM item;
    item.mask = LVIF_PARAM;
    item.iItem = nItem;
    VERIFY(m_openFilesList.GetItem(&item));
    return item.lParam;
  }
  return 0;
}


int CFilePanelWnd::OpenFiles_FindByParam (LPARAM param)
{
  LVITEM item;
  memset(&item, 0, sizeof(item));
  item.mask = LVIF_PARAM|LVIF_TEXT;
  int nItems = m_openFilesList.GetItemCount();

  for (item.iItem = 0; item.iItem < nItems; item.iItem++) 
  {
    VERIFY(m_openFilesList.GetItem(&item));

    if (item.lParam == param)
    {
      return item.iItem;
    }
  }
  return -1;
}

void CFilePanelWnd::OnOpenFiles_Click (NMHDR*, LRESULT* pResult)
{
  ActivateOpenFile();
  *pResult = 0;
}

void CFilePanelWnd::OnOpenFiles_KeyDown (NMHDR*, LRESULT* pResult)
{
  SetTimer(ACTIVATE_FILE_TIMER, ACTIVATE_FILE_DELAY, 0);
  *pResult = 0;
}

void CFilePanelWnd::OnOpenFiles_RClick (NMHDR* pNMHDR, LRESULT* pResult)
{
  ActivateOpenFile();

  NMITEMACTIVATE* pItem = (NMITEMACTIVATE*)pNMHDR;
  if (pItem && pItem->iItem != -1)
  {
    CPoint point(pItem->ptAction);
    m_openFilesList.ClientToScreen(&point);

    CMenu menu;
    VERIFY(menu.LoadMenu(IDR_OE_WORKBOOK_POPUP));
    CMenu* pPopup = menu.GetSubMenu(0);

    ASSERT(pPopup != NULL);
    ASSERT_KINDOF(CFrameWnd, AfxGetMainWnd());
    Common::GUICommandDictionary::AddAccelDescriptionToMenu(pPopup->m_hMenu);
    pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, AfxGetMainWnd());

    *pResult = 1;
  }
  else
  {
    *pResult = 0;
  }
}

void CFilePanelWnd::OnTimer (UINT_PTR nIDEvent)
{
  if (nIDEvent == ACTIVATE_FILE_TIMER)
  {
    BOOL active = GetFocus() == &m_openFilesList ? TRUE : FALSE;
    ActivateOpenFile();
    if (active) 
    {
      m_openFilesList.SetFocus();
    }
  }
  CTabCtrl::OnTimer(nIDEvent);
}

void CFilePanelWnd::OnExplorerTree_DblClick (NMHDR*, LRESULT* pResult)
{
  HTREEITEM hCurSel = m_explorerTree.GetNextItem(TVI_ROOT, TVGN_CARET);

  if (hCurSel)
  {
    CPoint point;
    ::GetCursorPos(&point);
    m_explorerTree.ScreenToClient(&point);
    if (m_explorerTree.HitTest(point, 0) == hCurSel)
    {
      CString path = m_explorerTree.GetFullPath(hCurSel);
      if (!IsFolder(path) || (0xFF00 & GetKeyState(VK_CONTROL)))
      {
        PostMessage(WM_COMMAND, ID_FPW_OPEN);
        *pResult = 1;
        return;
      }
    }
  }
  *pResult = 0;
}

void CFilePanelWnd::OnExplorerTree_RClick (NMHDR*, LRESULT* pResult)
{
  UINT uFlags;
  CPoint point;
  ::GetCursorPos(&point);
  m_explorerTree.ScreenToClient(&point);
  HTREEITEM hItem = m_explorerTree.HitTest(point, &uFlags);

  if (hItem && TVHT_ONITEM & uFlags)
  {
    if (m_explorerTree.GetNextItem(TVI_ROOT, TVGN_CARET) != hItem)
    {
      m_explorerTree.SelectItem(hItem);
    }
    CMenu menu;
    VERIFY(menu.LoadMenu(IDR_OE_EXPLORER_POPUP));
    CMenu* pPopup = menu.GetSubMenu(0);

    BOOL isFolder = IsFolder(m_explorerTree.GetFullPath(hItem));
    if (!isFolder) 
    {
      pPopup->SetDefaultItem(ID_FPW_OPEN);
    }
    pPopup->ModifyMenu(ID_FPW_OPEN, MF_BYCOMMAND, ID_FPW_OPEN, !isFolder ? "Open File\tDblClick" : "Open File Dalog...\tCtrl+DblClick");
    pPopup->EnableMenuItem(ID_FPW_REFRESH, isFolder ? MF_BYCOMMAND|MF_ENABLED : MF_BYCOMMAND|MF_GRAYED);

    pPopup->RemoveMenu(2, MF_BYPOSITION);
    pPopup->RemoveMenu(2, MF_BYPOSITION);
    pPopup->RemoveMenu(2, MF_BYPOSITION);
    //pPopup->EnableMenuItem(ID_FPW_GOTO_DEF_DIR, isFolder ? MF_BYCOMMAND|MF_ENABLED : MF_BYCOMMAND|MF_GRAYED);
    //pPopup->EnableMenuItem(ID_FPW_SET_DEF_DIR, isFolder ? MF_BYCOMMAND|MF_ENABLED : MF_BYCOMMAND|MF_GRAYED);

    ASSERT(pPopup != NULL);
    m_explorerTree.ClientToScreen(&point);
    pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);

    *pResult = 1;
  }
  else
  {
    *pResult = 0;
  }
}

void CFilePanelWnd::OnDrivers_RClick (NMHDR*, LRESULT* pResult)
{
  CPoint point;
  ::GetCursorPos(&point);

  CMenu menu;
  VERIFY(menu.LoadMenu(IDR_OE_DRIVERS_POPUP));
  CMenu* pPopup = menu.GetSubMenu(0);
  ASSERT(pPopup != NULL);
  pPopup->SetDefaultItem(ID_FPW_REFRESH_DRIVERS);
  pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);

  *pResult = 1;
}

void CFilePanelWnd::OnFpwOpen ()
{
  HTREEITEM hCurSel = m_explorerTree.GetNextItem(TVI_ROOT, TVGN_CARET);
  if (hCurSel)
  {
    CString path = m_explorerTree.GetFullPath(hCurSel);
    if (!IsFolder(path))
    {
      AfxGetApp()->OpenDocumentFile(path);
    }
    else
    {
      //VERIFY(::SetCurrentDirectory(path));
      CDocManagerExt::SetFileOpenPath(path);
      // 31.03.2003 bug fix, Attempt to open File dialog for a specified folder in File explorer fails 
      //                     if any file already open from another location
      AfxGetApp()->OnCmdMsg(ID_FILE_OPEN, 0, 0, 0);
      CDocManagerExt::SetFileOpenPath("");
    }
  }
}

void CFilePanelWnd::OnFpwRefresh ()
{
  HTREEITEM hCurSel = m_explorerTree.GetNextItem(TVI_ROOT, TVGN_CARET);
  if (hCurSel)
  {
    CString path = m_explorerTree.GetFullPath(hCurSel);
    if (IsFolder(path))
    {
      m_explorerTree.RefreshFolder(hCurSel);
    }
    else
    {
      MessageBeep((UINT)-1);
    }
  }
}

void CFilePanelWnd::OnFpwSetWorDir ()
{
  HTREEITEM hCurSel = m_explorerTree.GetNextItem(TVI_ROOT, TVGN_CARET);
  if (hCurSel)
  {
    CString path = m_explorerTree.GetFullPath(hCurSel);
    if (IsFolder(path))
    {
      m_explorerTree.SetItemState(hCurSel, INDEXTOSTATEIMAGEMASK(1), TVIS_STATEIMAGEMASK);
    }
    else
    {
      MessageBeep((UINT)-1);
    }
  }
}

BOOL CFilePanelWnd::SetCurPath (const CString& path)
{
  CString buffer = path;

  if (strnicmp(buffer, m_curDrivePath, m_curDrivePath.GetLength()))
  {
    DisplayDrivers();
  }
  for (int i(0), count((int)m_driverPaths.GetSize()); i < count; i++)
  {
    if (!strnicmp(buffer, m_driverPaths[i], m_driverPaths[i].GetLength()))
    {
      SelectDrive(m_driverPaths[i]);
      if (m_explorerTree.SetSelPath(buffer))
      {
        ChangeTab(FPW_EXPLORER_TAB);
        return TRUE;
      }
    }
  }
  return FALSE;
}

void CFilePanelWnd::OnFpwRefreshDrivers()
{
  DisplayDrivers(TRUE);
}

void
CFilePanelWnd::OnFpwRefreshOdbc()
{
  CWaitCursor sighAndWait;
  COpenEditorApp* app =(COpenEditorApp *) AfxGetApp();

  m_odbcTree.SetDatabase(&(app->GetDatabase()));
  m_odbcTree.MakeTreeInfo();

  m_tableTree.SetFilter("");
  m_tableTree.ClearTree();
}

void 
CFilePanelWnd::DisplayTables(BOOL force, BOOL curOnly)
{
  if(force)
  {
    if(!curOnly)
    {
      m_tableCBox.LockWindowUpdate();
    }
    m_tableCBox.SetRedraw(FALSE);
    m_tableCBox.ResetContent();

    for(int ind = 0; ind < m_tables.GetCount(); ++ind)
    {
      CString text = m_tables.GetAt(ind);
      text.GetBuffer();
      m_tableCBox.AddString(text);
      text.ReleaseBuffer();
    }
    m_tableCBox.SetRedraw(TRUE);
    if(!curOnly)
    {
      m_tableCBox.UnlockWindowUpdate();
    }
  }
}

void CFilePanelWnd::AddToTables(CString& table)
{
  for(int ind = 0; ind < m_tables.GetCount(); ++ind)
  {
    CString text = m_tables.GetAt(ind);
    if(!text.Find(table)  && (text.GetLength() == table.GetLength()))
    {
      // OK, Already in the list
      return;
    }
  }
  m_tables.Add(table);
  table.GetBuffer();
  m_tableCBox.AddString(table);
  table.ReleaseBuffer();
}

void 
CFilePanelWnd::OnTable_SetFocus()
{
  DisplayTables();
}

void 
CFilePanelWnd::OnTable_SelChange()
{
  // Get current selection
  CString table;
  int inx = m_tableCBox.GetCurSel();
  if(inx > CB_ERR)
  {
    m_tableCBox.GetLBText(inx,table);
  }
  else
  {
    m_tableCBox.GetWindowText(table);
    table.Trim();
    if(!table.IsEmpty())
    {
      AddToTables(table);
    }
  }

  // Compare with filter, if changed
  if(m_tableTree.GetFilter().CompareNoCase(table))
  {
    m_tableTree.ClearTree();
    m_tableTree.SetFilter(table);
  }
}

void
CFilePanelWnd::OnTable_Enter()
{
  CString filter;
  m_tableCBox.GetWindowText(filter);
  filter.Trim();

  // Potentially add to list
  if(!filter.IsEmpty())
  {
    AddToTables(filter);
  }

  // Compare with filter, if changed
  if(m_tableTree.GetFilter().CompareNoCase(filter))
  {
    m_tableTree.ClearTree();
    m_tableTree.SetFilter(filter);
  }
}

void 
CFilePanelWnd::FindTable(CString& table)
{
  // Add to listbox
  AddToTables(table);
  m_tableCBox.SelectString(-1,table);

  // Display this tab
  ChangeTab(FPW_TABLETREE_TAB);

  m_tableTree.ClearTree();
  m_tableTree.SetFilter(table);

  // Expand total table
  m_tableTree.ExpandFirstTable();
}

CString
CFilePanelWnd::FindNativeSQL(CString& command)
{
  COpenEditorApp* app = (COpenEditorApp *)AfxGetApp();
  if(app->DatabaseIsOpen())
  {
    CString sql(command);
    if(app->GetDatabase().ODBCNativeSQL(sql))
    {
      return sql;
    }
    return command;
  }
  return "";
}

void
CFilePanelWnd::ReportCapabilities(CString& filename)
{
  COpenEditorApp* app = (COpenEditorApp *)AfxGetApp();
  if(app->DatabaseIsOpen())
  {
    CWaitCursor take_a_deep_sigh;
    m_odbcTree.MakeTreeInfo();
    m_odbcTree.ReportAllCapabilities(filename);
  }
}

BOOL 
CFilePanelWnd::PreTranslateMessage(MSG* pMsg)
{
  if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
  {
    if(m_tab == FPW_TABLETREE_TAB)
    {
      OnTable_Enter();
      return TRUE;
    }
  }
  return CTabCtrl::PreTranslateMessage(pMsg);
}
