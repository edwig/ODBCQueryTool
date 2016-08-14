/* 
    Copyright (C) 2002 Aleksey Kochetov

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

/*
    07.03.2003 improvement, shortcut/acceleration descriptions nave been included in toolbar tooltips 
    07.03.2003 bug fix, wrong icon for a new document
    16.03.2003 bug fix, a woorkbook tab toltip shows a wrong path if any left tab has been closed
    23.03.2003 improvement, added a new command - mdi last window - default shortcut is Ctrl+Tab
*/

#include "stdafx.h"
#include <Shlwapi.h>
#include "OpenEditor/OEView.h"
#undef String

#include "COMMON/AppUtilities.h"
#include "COMMON/ExceptionHelper.h"
#include "COMMON/WorkbookMDIFrame.h"
#include "COMMON/GUICommandDictionary.h"
#include "OpenEditor/OEFindFiles.h"
#include <windows.h>
#include <iostream>

using namespace Common;

// default key names
const char* cszProfileName = "Workspace\\";
const char* cszMainFrame = "MainFrame";
const char* cszWP = "WP";


void CWorkbookControlBar::OnContextMenu (CWnd*, CPoint point)
{
    CMenu menu;
    VERIFY(menu.LoadMenu(IDR_OE_CBAR_POPUP));
    CMenu* pPopup = menu.GetSubMenu(0);

    if (m_pDockBar && m_pDockContext)
        pPopup->CheckMenuItem(ID_CBAR_DOCKING, 
                m_pDockContext->m_pBar->IsFloating() 
                    ? MF_BYCOMMAND|MF_UNCHECKED : MF_BYCOMMAND|MF_CHECKED);

    ASSERT(pPopup != NULL);
    ASSERT_KINDOF(CFrameWnd, AfxGetMainWnd());
    Common::GUICommandDictionary::AddAccelDescriptionToMenu(pPopup->m_hMenu);
    pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}

void CWorkbookControlBar::OnCbar_Docking ()
{
	if (m_pDockBar != NULL)
	{
		ASSERT(m_pDockContext != NULL);
		m_pDockContext->ToggleDocking();
	}
}

void CWorkbookControlBar::OnCbar_Hide ()
{
    ((CFrameWnd*)AfxGetMainWnd())->ShowControlBar(this, FALSE, TRUE);
}


BEGIN_MESSAGE_MAP(CWorkbookControlBar, baseCMyBar)
    ON_WM_CONTEXTMENU()
    ON_COMMAND(ID_CBAR_DOCKING, OnCbar_Docking)
    ON_COMMAND(ID_CBAR_HIDE,    OnCbar_Hide)
END_MESSAGE_MAP()

// CWorkbookMDIFrame

IMPLEMENT_DYNCREATE(CWorkbookMDIFrame, CMDIFrameWnd)

CWorkbookMDIFrame::CWorkbookMDIFrame()
: m_defaultFileExtension("SQL")
{
    m_bSaveMainWinPosition = TRUE;
    m_bShowed = FALSE;
    IDC_MF_WORKBOOK_BAR  = (AFX_IDW_CONTROLBAR_LAST - 1);
    IDC_MF_FILEPANEL_BAR = (AFX_IDW_CONTROLBAR_LAST - 2);
    IDC_MF_FILEPANEL     = (AFX_IDW_CONTROLBAR_LAST - 3);
    IDC_MF_FINDPANEL_BAR = (AFX_IDW_CONTROLBAR_LAST - 4);
    IDC_MF_FINDPANEL     = (AFX_IDW_CONTROLBAR_LAST - 5);
    m_cszProfileName = cszProfileName;
    m_cszMainFrame   = cszMainFrame;
    m_cszWP          = cszWP;

    m_hActiveChild = m_hLastChild = m_hSkipChild = NULL;
    m_bMDINextSeq  = FALSE;
}

CWorkbookMDIFrame::~CWorkbookMDIFrame()
{
}


BEGIN_MESSAGE_MAP(CWorkbookMDIFrame, CMDIFrameWnd)
	ON_NOTIFY(TCN_SELCHANGE, IDC_OE_WORKBOOK_TAB, OnChangeWorkbookTab)
	ON_WM_SYSCOMMAND()
  ON_WM_CLOSE()
  ON_WM_SHOWWINDOW()
	ON_COMMAND(ID_VIEW_WORKBOOK, OnViewWorkbook)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WORKBOOK, OnUpdateViewWorkbook)
  ON_COMMAND(ID_VIEW_FILE_PANEL, OnViewFilePanel)
  ON_UPDATE_COMMAND_UI(ID_VIEW_FILE_PANEL, OnUpdateViewFilePanel)
  ON_COMMAND(ID_VIEW_FIND_PANEL, OnViewFindPanel)
  ON_UPDATE_COMMAND_UI(ID_VIEW_FIND_PANEL,OnUpdateViewFindPanel)
  ON_COMMAND(ID_FILE_FIND_IN_FILE,OnFindFiles)
  ON_COMMAND(ID_FILE_SYNC_LOCATION, OnViewFilePanelSync)
  ON_UPDATE_COMMAND_UI(ID_FILE_SYNC_LOCATION, OnUpdateViewFilePanelSync)
	// toolbar "tooltip" notification
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
  ON_COMMAND(ID_WINDOW_LAST, OnLastWindow)
  // test
  ON_NOTIFY(NM_CLICK,(0xE8FF-5) /*IDC_MF_FINDPANEL*/,OnFindFilesClicked)
END_MESSAGE_MAP()

int CWorkbookMDIFrame::DoCreate (BOOL loadBarState)
{
#ifdef _SCB_REPLACE_MINIFRAME
    m_pFloatingFrameClass = RUNTIME_CLASS(CSCBMiniDockFrameWnd);
#endif //_SCB_REPLACE_MINIFRAME

    CString drivePath = AfxGetApp()->GetProfileString("FileManager", "CurrentDrive");
    if (drivePath.IsEmpty())
    {
        std::string path;
        AppGetPath(path);
        PathBuildRoot(drivePath.GetBuffer(MAX_PATH), PathGetDriveNumber(path.c_str()));
        drivePath.ReleaseBuffer();
    }
    m_wndFilePanel.SetCurDrivePath(drivePath);

  if (!m_wndFilePanelBar.Create("File panel", this, IDC_MF_FILEPANEL_BAR))
	{
		TRACE0("Failed to create instant bar\n");
		return -1;
	}
  m_wndFilePanelBar.ModifyStyle(0, WS_CLIPCHILDREN, 0);
	m_wndFilePanelBar.SetSCBStyle(m_wndFilePanelBar.GetSCBStyle()|SCBS_SIZECHILD);
  m_wndFilePanelBar.SetBarStyle(m_wndFilePanelBar.GetBarStyle()|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC);
	m_wndFilePanelBar.EnableDocking(CBRS_ALIGN_LEFT|CBRS_ALIGN_RIGHT);
	DockControlBar(&m_wndFilePanelBar, AFX_IDW_DOCKBAR_LEFT);
    
	if (!m_wndFilePanel.Create(WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN
        |TCS_BUTTONS|TCS_FLATBUTTONS|TCS_FOCUSNEVER, 
        CRect(0, 0, 0, 0), &m_wndFilePanelBar, IDC_MF_FILEPANEL)
    )
	{
		TRACE0("Failed to create instant bar child\n");
		return -1;
	}

  if (!m_wndFindPanelBar.Create("Find panel", this, IDC_MF_FINDPANEL_BAR))
	{
		TRACE0("Failed to create find instant bar\n");
		return -1;
	}
  m_wndFindPanelBar.ModifyStyle(0,WS_CLIPCHILDREN,0);
  m_wndFindPanelBar.SetSCBStyle(m_wndFindPanelBar.GetSCBStyle() | SCBS_SIZECHILD);
  m_wndFindPanelBar.SetBarStyle(m_wndFindPanelBar.GetBarStyle() | CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC);
  m_wndFindPanelBar.EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
  DockControlBar(&m_wndFindPanelBar, AFX_IDW_DOCKBAR_BOTTOM);

  if(!m_wndFindPanel.Create(CRect(0,0,0,0)
                           ,&m_wndFindPanelBar
                           ,IDC_MF_FINDPANEL
                           ,WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|TCS_BUTTONS|TCS_FLATBUTTONS|TCS_FOCUSNEVER))
  {
    TRACE0("Failed to create find-file bar child\n");
    return -1;
  }
  InitGridEmpty();

  if (!m_wndWorkbookBar.Create(this, IDC_MF_WORKBOOK_BAR))
	{
		TRACE0("Failed to create Workbook bar\n");
		return -1;
	}

  if(!m_font.CreatePointFont(90,"Tahoma"))
  {
	  // older versions of Windows* (NT 3.51 for instance)
	  // fail with DEFAULT_GUI_FONT
	  if (!m_font.CreateStockObject(DEFAULT_GUI_FONT))
	  {
      if (!m_font.CreatePointFont(80, "MS Sans Serif"))
      {
        return -1;
      }
    }
  }

  m_wndFilePanel.SetFont(&m_font);
  m_wndWorkbookBar.SetFont(&m_font);
  //m_wndWorkbookBar.SetImageList(&m_wndFilePanel.GetSysImageList());

  if (loadBarState)
      LoadBarState();

  return 0;
}

void CWorkbookMDIFrame::OnClose()
{
    AfxGetApp()->WriteProfileString("FileManager", "CurrentDrive", m_wndFilePanel.GetCurDrivePath());

    WINDOWPLACEMENT wp;
	wp.length = sizeof wp;
    if (GetWindowPlacement(&wp))
    {
        if (wp.showCmd == SW_MINIMIZE)
        {
            wp.showCmd = SW_SHOWNORMAL;
            wp.flags   = 0;
        }
        AfxGetApp()->WriteProfileBinary(m_cszMainFrame, m_cszWP, (LPBYTE)&wp, sizeof(wp));
    }

    SaveBarState(m_cszProfileName);
    CSizingControlBar::GlobalSaveState(this, m_cszProfileName);

    CMDIFrameWnd::OnClose();
}

void CWorkbookMDIFrame::OnShowWindow(BOOL bShow, UINT nStatus)
{
    if (m_bSaveMainWinPosition && !m_bShowed)
    {
        m_bShowed = TRUE;

        BYTE *pWP;
        UINT size;

        if (AfxGetApp()->GetProfileBinary(m_cszMainFrame, m_cszWP, &pWP, &size)
        && size == sizeof(WINDOWPLACEMENT))
        {
	        SetWindowPlacement((WINDOWPLACEMENT*)pWP);
	        delete []pWP;
            return;
        }
    }

    CMDIFrameWnd::OnShowWindow(bShow, nStatus);
}

BOOL CWorkbookMDIFrame::VerifyBarState ()
{
    CDockState state;
    state.LoadState(m_cszProfileName);

    for (int i = 0; i < state.m_arrBarInfo.GetSize(); i++)
    {
        CControlBarInfo* pInfo = (CControlBarInfo*)state.m_arrBarInfo[i];
        ASSERT(pInfo != NULL);
        int nDockedCount = (int)pInfo->m_arrBarID.GetSize();
        if (nDockedCount > 0)
        {
            // dockbar
            for (int j = 0; j < nDockedCount; j++)
            {
                UINT nID = (UINT) pInfo->m_arrBarID[j];
                if (nID == 0) continue; // row separator
                if (nID > 0xFFFF)
                    nID &= 0xFFFF; // placeholder - get the ID
                if (GetControlBar(nID) == NULL)
                    return FALSE;
            }
        }
        
        if (!pInfo->m_bFloating) // floating dockbars can be created later
            if (GetControlBar(pInfo->m_nBarID) == NULL)
                return FALSE; // invalid bar ID
    }

    return TRUE;
}

BOOL CWorkbookMDIFrame::LoadBarState ()
{
    if (VerifyBarState())
    {
        CMDIFrameWnd::LoadBarState(m_cszProfileName);
        CSizingControlBar::GlobalLoadState(this, m_cszProfileName);
        ShowControlBar(&m_wndFilePanelBar, m_wndFilePanelBar.IsVisible(), FALSE); 
        return TRUE;
    }
    return FALSE;
}

void CWorkbookMDIFrame::OnCreateChild (CMDIChildWnd* pWnd)
{
    static CString strTitle;
    pWnd->GetWindowText(strTitle);

    {
        TCITEM item;
        memset(&item, 0, sizeof(item));
        item.mask = TCIF_TEXT|TCIF_PARAM;
        item.pszText = (LPSTR)(LPCSTR)strTitle;
        item.lParam  = (LPARAM)pWnd;
        m_wndWorkbookBar.AppendItem(item);
    }

    {
        LVITEM item;
        memset(&item, 0, sizeof(item));
        item.mask    = LVIF_TEXT|LVIF_PARAM;
        item.pszText = (LPSTR)(LPCSTR)strTitle;
        item.lParam  = (LPARAM)pWnd;
        m_wndFilePanel.OpenFiles_Append(item);
    }
}

void CWorkbookMDIFrame::OnDestroyChild (CMDIChildWnd* pWnd)
{
    m_wndWorkbookBar.RemoveItemByParam((LPARAM)pWnd);
    m_wndFilePanel.OpenFiles_RemoveByParam((LPARAM)pWnd);

    // 16.03.2003 bug fix, a woorkbook tab toltip shows a wrong path if any left tab has been closed
    CWnd* pMDIClient = GetWindow(GW_CHILD);
    CWnd* pMDIChild = (pMDIClient) ? pMDIClient->GetWindow(GW_CHILD) : 0;

    while (pMDIChild)
    {
        if (pMDIChild->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)))
        {
            CString title;
            pMDIChild->GetWindowText(title);

            if (pMDIChild != pWnd)
                OnRenameChild((CMDIChildWnd*)pMDIChild, title);
        }

        pMDIChild = pMDIChild->GetNextWindow();
    }
}

void CWorkbookMDIFrame::OnActivateChild (CMDIChildWnd* pWnd)
{
    m_hLastChild = m_hActiveChild;
    m_hActiveChild = pWnd ? pWnd->m_hWnd : NULL;
    m_wndWorkbookBar.ActivateItemByParam((LPARAM)pWnd);
    m_wndFilePanel.OpenFiles_ActivateByParam((LPARAM)pWnd);
}

void CWorkbookMDIFrame::OnRenameChild (CMDIChildWnd* pWnd, LPCTSTR szTitle)
{
    int iImage = GetImageByDocument(pWnd->GetActiveDocument());

    {
        TCITEM item;
        item.mask = TCIF_TEXT;
        item.pszText = (LPTSTR)szTitle;
        //if (iImage != -1) 
        //{
            item.iImage = iImage;
            item.mask |= TCIF_IMAGE;
        //}
        CString path;
        CDocument* doc = pWnd->GetActiveDocument();
        if(doc)
        {
          path = doc->GetPathName();
        }
        if (path.IsEmpty()) path = "<Path not specified>";
        m_wndWorkbookBar.UpdateItemByParam((LPARAM)pWnd, item, path);
    }

    {
        LVITEM item;
        memset(&item, 0, sizeof(item));
        item.mask    = LVIF_TEXT|LVIF_PARAM;
        item.pszText = (LPTSTR)szTitle;
        item.lParam  = (LPARAM)pWnd;
        //if (iImage != -1) 
        //{
            item.iImage = iImage;
            item.mask |= LVIF_IMAGE;
        //}
        m_wndFilePanel.OpenFiles_UpdateByParam((LPARAM)pWnd, item);
    }
}

int CWorkbookMDIFrame::GetImageByDocument (const CDocument* pDoc)
{
    if (pDoc)
    {
        CString path = pDoc->GetPathName();

        if (path.IsEmpty()) path   = "*." + m_defaultFileExtension;
        
        BOOL newDoc = !PathFileExists(path);

	    SHFILEINFO shFinfo;
        if (SHGetFileInfo(path, FILE_ATTRIBUTE_NORMAL, &shFinfo, sizeof(shFinfo), 
                (newDoc ? SHGFI_ICON|SHGFI_SMALLICON|SHGFI_USEFILEATTRIBUTES : SHGFI_ICON|SHGFI_SMALLICON)))
        {
	        DestroyIcon(shFinfo.hIcon);
            return shFinfo.iIcon;
        }
    }
    return -1;
}


void CWorkbookMDIFrame::ActivateChild (CMDIChildWnd* child)
{
    BOOL bMaximized;
    MDIGetActive(&bMaximized);

    if (!bMaximized  && child->IsIconic())
        child->MDIRestore();
    else
        child->MDIActivate();
}


void CWorkbookMDIFrame::OnChangeWorkbookTab (NMHDR*, LRESULT* pResult)
{
    LPARAM param = m_wndWorkbookBar.GetCurSelParam();
    
    if (param)
    {
        CMDIChildWnd* child = (CMDIChildWnd*)param;
        _ASSERT(child->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)));
        ActivateChild(child);
    }

    *pResult = 0;
}

/*
    This handler remove Workbook Tab resizing bug on maximize/restore
*/
void CWorkbookMDIFrame::OnSysCommand (UINT nID, LPARAM lParam)
{
    CFrameWnd::OnSysCommand(nID, lParam);

	UINT nItemID = (nID & 0xFFF0);

    // 16.03.2003 bug fix, violetion in debug mode when the application is closing
	if ((nItemID == SC_MAXIMIZE || nItemID == SC_RESTORE) && !m_bHelpMode)
        RecalcLayout(TRUE/*bNotify*/);
}

void CWorkbookMDIFrame::OnViewWorkbook ()
{
    ShowControlBar(&m_wndWorkbookBar, !m_wndWorkbookBar.IsVisible(), TRUE);
}


void CWorkbookMDIFrame::OnUpdateViewWorkbook (CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndWorkbookBar.IsVisible());
}

void CWorkbookMDIFrame::OnViewFilePanel()
{
    ShowControlBar(&m_wndFilePanelBar, !m_wndFilePanelBar.IsVisible(), TRUE);
}

void CWorkbookMDIFrame::OnViewFindPanel()
{
  ShowControlBar(&m_wndFindPanelBar, !m_wndFindPanelBar.IsVisible(), TRUE);
}

void CWorkbookMDIFrame::OnUpdateViewFilePanel(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndFilePanelBar.IsVisible());
}

void CWorkbookMDIFrame::OnUpdateViewFindPanel(CCmdUI *pCmdUI)
{
  pCmdUI->SetCheck(m_wndFindPanelBar.IsVisible());
}

void CWorkbookMDIFrame::OnViewFilePanelSync ()
{
    LPARAM param = m_wndWorkbookBar.GetCurSelParam();
    
    if (param)
    {
        CMDIChildWnd* child = (CMDIChildWnd*)param;
        _ASSERT(child->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)));

        CDocument* pDocum = child->GetActiveDocument();
        if (pDocum)
        {
            CString path = pDocum->GetPathName();
            if (!path.IsEmpty() && m_wndFilePanel.SetCurPath(path))
            {
                if (!m_wndFilePanelBar.IsVisible())
                    ShowControlBar(&m_wndFilePanelBar, TRUE, TRUE);
                return; // Ok!
            }
        }
    }
    MessageBeep((UINT)-1); // something fails
}

void CWorkbookMDIFrame::OnUpdateViewFilePanelSync (CCmdUI *pCmdUI)
{
    pCmdUI->Enable(!m_wndWorkbookBar.IsEpmty() ? TRUE : FALSE);
}

//void CWorkbookMDIFrame::DockControlBarLeftOf (CWorkbookControlBar* Bar, CWorkbookControlBar* LeftOf)
void CWorkbookMDIFrame::DockControlBarLeftOf(CControlBar* Bar,CControlBar* LeftOf)
{
	UINT n;
	DWORD dw;
	CRect rect;

	// get MFC to adjust the dimensions of all docked ToolBars
	// so that GetWindowRect will be accurate
	RecalcLayout();
	LeftOf->GetWindowRect(&rect);
	rect.OffsetRect(1,1);
	dw=LeftOf->GetBarStyle();
	n = 0;
	n = (dw&CBRS_ALIGN_TOP)            ? AFX_IDW_DOCKBAR_TOP    : n;
	n = (dw&CBRS_ALIGN_BOTTOM && n==0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
	n = (dw&CBRS_ALIGN_LEFT && n==0)   ? AFX_IDW_DOCKBAR_LEFT   : n;
	n = (dw&CBRS_ALIGN_RIGHT && n==0)  ? AFX_IDW_DOCKBAR_RIGHT  : n;

	// When we take the default parameters on rect, DockControlBar will dock
	// each Toolbar on a seperate line.  By calculating a rectangle, we in effect
	// are simulating a Toolbar being dragged to that location and docked.
	DockControlBar(Bar,n,&rect);
}

LRESULT CWorkbookMDIFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    try
    {
        return CMDIFrameWnd::WindowProc(message, wParam, lParam);
    }
    _COMMON_DEFAULT_HANDLER_

    return 0;
}

// PORTING2005
//#define _countof(array)         (sizeof(array)/sizeof(array[0]))

#define _AfxGetDlgCtrlID(hWnd)  ((UINT)(WORD)::GetDlgCtrlID(hWnd))

BOOL CWorkbookMDIFrame::OnToolTipText (UINT, NMHDR* pNMHDR, LRESULT* pResult)
{
	ASSERT(pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW);

	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	TCHAR szFullText[256];
	CString strTipText;
	UINT_PTR nID = pNMHDR->idFrom;
	if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
		pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
	{
		// idFrom is actually the HWND of the tool
		nID = _AfxGetDlgCtrlID((HWND)nID);
	}

	if (nID != 0) // will be zero on a separator
	{
		// don't handle the message if no string resource found
		if (AfxLoadString((UINT)nID, szFullText) == 0)
			return FALSE;

		// this is the command id, not the button index
		AfxExtractSubString(strTipText, szFullText, 1, '\n');

        std::string accLabel;
        if (Common::GUICommandDictionary::GetCommandAccelLabel(static_cast<Common::Command>(nID), accLabel))
        {
            strTipText += " (";
            strTipText += accLabel.c_str();
            strTipText +=  ")";
        }
	}

#ifndef _UNICODE
	if (pNMHDR->code == TTN_NEEDTEXTA)
		lstrcpyn(pTTTA->szText, strTipText, _countof(pTTTA->szText));
	else
		_mbstowcsz(pTTTW->szText, strTipText, _countof(pTTTW->szText));
#else
	if (pNMHDR->code == TTN_NEEDTEXTA)
		_wcstombsz(pTTTA->szText, strTipText, _countof(pTTTA->szText));
	else
		lstrcpyn(pTTTW->szText, strTipText, _countof(pTTTW->szText));
#endif
	*pResult = 0;

	// bring the tooltip window above other popup windows
	::SetWindowPos(pNMHDR->hwndFrom, HWND_TOP, 0, 0, 0, 0,
		SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOMOVE|SWP_NOOWNERZORDER);

	return TRUE;    // message was handled
}

void CWorkbookMDIFrame::OnLastWindow ()
{
    if (!m_bMDINextSeq && m_hLastChild && ::IsWindow(m_hLastChild) )
    {
        CMDIChildWnd* active = MDIGetActive();
        m_hSkipChild = active ? active->m_hWnd : NULL;

        MDIActivate(FromHandle(m_hLastChild));
        m_bMDINextSeq = TRUE;
    }
    else
    {
        MDINext();

        CMDIChildWnd* active = MDIGetActive();
        
        if (active && active->m_hWnd == m_hSkipChild)
            MDINext();

        m_hSkipChild = NULL;
    }
}

BOOL CWorkbookMDIFrame::PreTranslateMessage (MSG* pMsg)
{

    if (pMsg->message == WM_KEYUP && pMsg->wParam == VK_CONTROL)
    {
        m_bMDINextSeq = FALSE;
        m_hSkipChild = NULL;
    }

    return CMDIFrameWnd::PreTranslateMessage(pMsg);
}

void 
CWorkbookMDIFrame::OnFpwRefreshOdbc()
{
  m_wndFilePanel.OnFpwRefreshOdbc();
  m_wndFilePanel.OnTable_SelChange();
}

void
CWorkbookMDIFrame::FindTable(CString& table)
{
  m_wndFilePanel.FindTable(table);
}

CString
CWorkbookMDIFrame::FindNativeSQL(CString& command)
{
  return m_wndFilePanel.FindNativeSQL(command);
}

void
CWorkbookMDIFrame::ReportCapabilities(CString& filename)
{
  m_wndFilePanel.ReportCapabilities(filename);
}

void 
CWorkbookMDIFrame::InitGridEmpty()
{
  if(!m_wndFindPanel)
  {
    return;
  }
	// Set standard
	m_wndFindPanel.SetEditable(FALSE);
	m_wndFindPanel.EnableDragAndDrop(TRUE);
	m_wndFindPanel.SetHeaderSort(TRUE);

  // Delete previous results
  while(m_wndFindPanel.GetRowCount() > 1)
  {
    m_wndFindPanel.DeleteRow(m_wndFindPanel.GetRowCount() - 1);
  }
  while(m_wndFindPanel.GetColumnCount() > 1)
  {
    m_wndFindPanel.DeleteColumn(m_wndFindPanel.GetColumnCount() - 1);
  }

  // Set empty grid
  try
  {
    m_wndFindPanel.SetRowCount(1);
    m_wndFindPanel.SetColumnCount(3);
		m_wndFindPanel.SetFixedRowCount(1);
  }
  catch(CMemoryException* e)
  {
    e->ReportError();
    e->Delete();
    return;
  }
  GV_ITEM item;
  item.mask = GVIF_TEXT | GVIF_FORMAT;
  item.row = 0;
  item.col = 0;
	item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX;

  item.strText = "File";
  m_wndFindPanel.SetItem(&item);
  m_wndFindPanel.SetColumnWidth(0,200);

  item.col     = 2;
  item.strText = "Text";
  m_wndFindPanel.SetItem(&item);
  m_wndFindPanel.SetColumnWidth(0,200);

  item.col      = 1;
  item.strText  = "Line";
  item.nFormat |= DT_SORT_NUMERIC;
  m_wndFindPanel.SetItem(&item);
  m_wndFindPanel.SetColumnWidth(0,200);

  // Sync with the outside world
	m_wndFindPanel.AutoSize();
  m_wndFindPanel.AutoSizeColumns(GVS_BOTH);
  m_wndFindPanel.ExpandLastColumn();
}

SettingsManager CWorkbookMDIFrame::m_settingsManager;

void CWorkbookMDIFrame::OnFindFiles()
{
  LoadSettingsManager();

  static UINT gStartPage = 0;
  Common::CPropertySheetMem sheet("Settings", gStartPage);
  sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
  sheet.m_psh.dwFlags &= ~PSH_HASHELP;

  COEFindFiles findFiles(m_settingsManager);

  if(findFiles.DoModal() == IDOK)
  {
    SaveSettingsManager();

    // Hide the find-in-files control for pre-emptive redrawing
    // and re-init the control
    ShowControlBar(&m_wndFindPanelBar,FALSE,TRUE);
    InitGridEmpty();

    if(findFiles.GoFind(&m_wndFindPanel))
    {
      // Something found
      ShowControlBar(&m_wndFindPanelBar,TRUE,TRUE);
      DockControlBar(&m_wndFindPanelBar,AFX_IDW_DOCKBAR_BOTTOM);
      m_wndFindPanel.ExpandLastColumn();
    }
  }
}

void 
CWorkbookMDIFrame::SaveSettingsManager()
{
  std::string path;
  AppGetPath(path);

  // old settings backup
  _CHECK_AND_THROW_(
      CopyFile((path + "\\data\\settings.dat").c_str(), 
               (path + "\\data\\settings.dat.old").c_str(), FALSE) != 0,
      "File search: settings error!"
      );

  OpenEditor::SettingsManagerWriter(OpenEditor::FileOutStream((path + "\\data\\settings.dat").c_str())) << m_settingsManager;
}

void
CWorkbookMDIFrame::LoadSettingsManager()
{
    std::string path;
    AppGetPath(path);

    // Load editor settings
    OpenEditor::SettingsManagerReader(OpenEditor::FileInStream((path + "\\data\\settings.dat").c_str())) >> m_settingsManager;
}

void 
CWorkbookMDIFrame::OnFindFilesClicked(NMHDR * pNotifyStruct, LRESULT * result)
{
  int maxrow = m_wndFindPanel.GetRowCount();
  CCellID cell = m_wndFindPanel.GetFocusCell();
  int row = cell.row;

  if(row >= maxrow-2)
  {
    // Last two rows contain countings
    return;
  }
  CString fileName = m_wndFindPanel.GetItemText(row,0);
  int   lineNumber = atoi(m_wndFindPanel.GetItemText(row,1));


  if(fileName.IsEmpty())
  {
    AfxMessageBox("Cannot find file: " + fileName,MB_OK|MB_ICONEXCLAMATION);
    return;
  }
  CDocument* doc = AfxGetApp()->OpenDocumentFile(fileName);
  if(doc)
  {
    // OK, doc was openend
    POSITION pos = doc->GetFirstViewPosition();
	  while (pos != NULL)
	  {
		  CView* pView = doc->GetNextView(pos);
      COEditorView* view = dynamic_cast<COEditorView *> (pView);
      if(view)
      {
        OpenEditor::Position pos;
        pos.line = lineNumber - 1;
        view->MoveToAndCenter(pos);
        return;
      }
	  }
  }
}


