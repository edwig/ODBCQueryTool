////////////////////////////////////////////////////////////////////////
//
// File: DirTreeView.cpp
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of 
// this software and associated documentation files (the "Software"), 
// to deal in the Software without restriction, including without limitation the rights 
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies 
// or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION 
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
#include "pch.h"
#include "WinException.h"
#include "DirTreeView.h"
#include "SortStringArray.h"
#include "QueryTool.h"
#include "Common\ExceptionHelper.h"
#include "Common\DocManagerExt.h"

#define SUBDIR_ALWAYS 1

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
//
// DirTree Dockable View
//
//////////////////////////////////////////////////////////////////////////

static BOOL GetVolumeName(LPCTSTR rootDirectory, CString& volumeName)
{
  const int STRSIZE = 80;
  DWORD volumeSerialNumber;           // volume serial number
  DWORD maximumComponentLength;       // maximum file name length
  DWORD fileSystemFlags;              // file system options
  TCHAR fileSystemNameBuffer[STRSIZE];// file system name buffer

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

static LPCTSTR MakeVolumeLabel(LPCTSTR rootDirectory, CString& volumeLabel)
{
  volumeLabel = rootDirectory;
  volumeLabel.TrimRight('\\');

  CString volumeName;
  if (GetVolumeName(rootDirectory, volumeName))
  {
    volumeLabel += _T("   ");
    volumeLabel += volumeName;
  }
  return volumeLabel;
}

static BOOL IsFolder(const CString& path)
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

class CDirViewMenuButton : public CMFCToolBarMenuButton
{
  friend class CClassView;

  DECLARE_SERIAL(CDirViewMenuButton)

public:
  CDirViewMenuButton(HMENU hMenu = nullptr) noexcept : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
  {
  }

  virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE,
    BOOL bCustomizeMode = FALSE, BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE)
  {
    pImages = CMFCToolBar::GetImages();

    CAfxDrawState ds;
    pImages->PrepareDrawImage(ds);

    CMFCToolBarMenuButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);

    pImages->EndDrawImage(ds);
  }
};

IMPLEMENT_SERIAL(CDirViewMenuButton, CMFCToolBarMenuButton, 1)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DirTreeView::DirTreeView() noexcept
            :m_isExplorerInitialized(false)
            ,m_isDrivesInitialized(false)
            ,m_nCurrSort(0)
{
}

DirTreeView::~DirTreeView()
{
}

BEGIN_MESSAGE_MAP(DirTreeView, CDockablePane)
  ON_WM_CREATE()
  ON_WM_SIZE()
  ON_WM_CONTEXTMENU()
  ON_WM_PAINT()
  ON_WM_SETFOCUS()

  ON_CBN_SETFOCUS (ID_FPW_DRIVES,       OnDrive_SetFocus)
  ON_CBN_SELCHANGE(ID_FPW_DRIVES,       OnDrive_SelChange)
  ON_NOTIFY(NM_DBLCLK, ID_FPW_EXPLORER, OnExplorerTree_DblClick)
  ON_NOTIFY(NM_RCLICK, ID_FPW_EXPLORER, OnExplorerTree_RClick)
  ON_NOTIFY(NM_RCLICK, ID_FPW_DRIVES,   OnDrivers_RClick)
  ON_CBN_SETFOCUS (ID_FPW_FILTER,       OnFilter_SetFocus)
  ON_CBN_SELCHANGE(ID_FPW_FILTER,       OnFilter_SelChange)
  ON_CBN_KILLFOCUS(ID_FPW_FILTER,       OnFilter_SelChange)
  ON_COMMAND(ID_FPW_OPEN,               OnFpwOpen)
  ON_COMMAND(ID_FPW_REFRESH,            OnFpwRefresh)
  ON_COMMAND(ID_FPW_SET_DEF_DIR,        OnFpwSetWorDir)
  ON_COMMAND(ID_FPW_REFRESH_DRIVERS,    OnFpwRefreshDrivers)
  ON_NOTIFY_EX(TTN_NEEDTEXT, 0,         OnTooltipNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassView message handlers

int DirTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CDockablePane::OnCreate(lpCreateStruct) == -1)
    return -1;

  CRect rectDummy;
  rectDummy.SetRectEmpty();

  if(!m_drivesCBox.Create(WS_CHILD | WS_TABSTOP | CBS_DROPDOWNLIST | WS_VISIBLE,
                          CRect(0, 0, 0, GetSystemMetrics(SM_CYFULLSCREEN) / 2), this, ID_FPW_DRIVES))
  {
    TRACE0("Failed to create drives combo box\n");
    return -1;
  }

  // Create views:
  const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | TVS_SHOWSELALWAYS;

  m_wndDirTree.SetDirectInit(false);
  if (!m_wndDirTree.Create(dwViewStyle, rectDummy, this, ID_FPW_EXPLORER))
  {
    TRACE0("Failed to create ODBC Info View\n");
    return -1;      // fail to create
  }
  m_wndDirTree.InitSkin();

  if(!m_filterCBox.Create(WS_CHILD | WS_TABSTOP | CBS_DROPDOWN | WS_VISIBLE,CRect(0,0,0,GetSystemMetrics(SM_CYFULLSCREEN) / 2),this,ID_FPW_FILTER))
  {
    TRACE0("Failed to create filter combo box\n");
    return -1;
  }
  m_filterCBox.EnableToolTips(TRUE);

  // Load images:
//   m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT);
//   m_wndToolBar.LoadToolBar(IDR_SORT, 0, 0, TRUE /* Is locked */);

  OnChangeVisualStyle();

//   m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
//   m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
// 
//   m_wndToolBar.SetOwner(this);

  // All commands will be routed via this control , not via the parent frame:
//  m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

//   CMenu menuSort;
//   menuSort.LoadMenu(IDR_POPUP_SORT);
// 
//   m_wndToolBar.ReplaceButton(ID_SORT_MENU, CDirViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));
// 
//   CDirViewMenuButton* pButton = DYNAMIC_DOWNCAST(CDirViewMenuButton, m_wndToolBar.GetButton(0));
// 
//   if (pButton != nullptr)
//   {
//     pButton->m_bText = FALSE;
//     pButton->m_bImage = TRUE;
//     pButton->SetImage(GetCmdMgr()->GetCmdImage(m_nCurrSort));
//     pButton->SetMessageWnd(this);
//   }

  m_drivesCBox.SetImageList(&m_wndDirTree.GetSysImageList());
  m_dirTreeStateImageList.Create(IDB_OE_EXPLORER_STATE_LIST, 16, 64, RGB(0, 0, 255));
  m_wndDirTree.SetImageList(&m_dirTreeStateImageList, TVSIL_STATE);

  // Adjust combo boxes and tree control inside the pane
  AdjustLayout();

  return 0;
}

void
DirTreeView::OnSize(UINT nType, int cx, int cy)
{
  CDockablePane::OnSize(nType, cx, cy);
  AdjustLayout();
}

void
DirTreeView::OnActivate()
{
  if(m_isExplorerInitialized)
  {
    return;
  }
  CWaitCursor wait;
  UpdateWindow();

  DisplayDrivers(FALSE, TRUE);
  SelectDrive(m_curDrivePath, TRUE);
  m_isExplorerInitialized = TRUE;
}

void 
DirTreeView::DisplayDrivers(BOOL force, BOOL curOnly)
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
    TCHAR  szDrives[2 * 1024];
    LPTSTR pDrive;

    if (curOnly)
    {
      _CHECK_AND_THROW_(m_curDrivePath.GetLength() < sizeof(szDrives) - 2,
                        _T("Current drive path is too long."));
      memset(szDrives, 0, sizeof(szDrives));
      memcpy(szDrives, (LPCTSTR)m_curDrivePath, m_curDrivePath.GetLength());
    }
    else
    {
      _CHECK_AND_THROW_(GetLogicalDriveStrings(sizeof(szDrives), szDrives),
                       _T("Cannot get logical drive strings."));
    }

    m_driverPaths.RemoveAll();

    COMBOBOXEXITEM item;
    memset(&item, 0, sizeof(item));
    item.mask = CBEIF_TEXT;

    for (pDrive = szDrives; *pDrive; pDrive += _tcslen(pDrive) + 1, item.iItem++)
    {
      m_driverPaths.Add(pDrive);

      CString volumeLabel;
      item.pszText = (LPTSTR)MakeVolumeLabel(pDrive, volumeLabel);

      SHFILEINFO shFinfo;
      if (SHGetFileInfo(pDrive, 0, &shFinfo, sizeof(shFinfo), SHGFI_ICON | SHGFI_SMALLICON))
      {
        item.mask |= CBEIF_IMAGE | CBEIF_SELECTEDIMAGE;
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

void
DirTreeView::SelectDrive(const CString& path, BOOL force)
{
  if (!m_hWnd || m_driverPaths.GetCount() == 0)
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
          message.Format(_T("Cannot select drive \"%s\". Drive/device is not available."), (LPCTSTR)path);
          AfxMessageBox(message, MB_OK | MB_ICONSTOP);
          SelectDrive(m_curDrivePath, TRUE);
        }
        else
        {
          try
          {
            m_drivesCBox.SetCurSel(i);
            m_curDrivePath = path;
            m_wndDirTree.DisplayTree(m_curDrivePath, TRUE);
            m_wndDirTree.SetSelPath(m_curDrivePath);
          }
          catch (CWinException* x)
          {
            CString message;
            message.Format(_T("Cannot select drive \"%s\". %s"), (LPCTSTR)path, (LPCTSTR)*x);
            AfxMessageBox(message, MB_OK | MB_ICONSTOP);
          }
        }
      }
    }
  }
}


void DirTreeView::OnContextMenu(CWnd* pWnd, CPoint point)
{
  StyleTreeCtrl* pWndTree = (StyleTreeCtrl*)&m_wndDirTree;
  ASSERT_VALID(pWndTree);

  if (pWnd != pWndTree)
  {
    CDockablePane::OnContextMenu(pWnd, point);
    return;
  }

  if (point != CPoint(-1, -1))
  {
    // Select clicked item:
    CPoint ptTree = point;
    pWndTree->ScreenToClient(&ptTree);

    UINT flags = 0;
    HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
    if (hTreeItem != nullptr)
    {
      pWndTree->SelectItem(hTreeItem);
    }
  }

  pWndTree->SetFocus();
  CMenu menu;
  menu.LoadMenu(IDR_POPUP_SORT);

  CMenu* pSumMenu = menu.GetSubMenu(0);

  if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
  {
    CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

    if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
      return;

    ((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
    UpdateDialogControls(this, FALSE);
  }
}

#define COMBOBOX_SIZE_HEIGHT 24

void
DirTreeView::AdjustLayout()
{
  if (GetSafeHwnd() == nullptr)
  {
    return;
  }
  UINT flags = SWP_NOACTIVATE | SWP_NOZORDER;
  CRect rectClient;
  GetClientRect(rectClient);

  int cyTlb = 0;
//int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;
//m_wndToolBar.SetWindowPos(nullptr,rectClient.left,rectClient.top,rectClient.Width(),cyTlb,flags);
  m_drivesCBox.SetWindowPos(nullptr,rectClient.left,rectClient.top + cyTlb,rectClient.Width(),COMBOBOX_SIZE_HEIGHT,flags);

  CRect comboRect;
  m_drivesCBox.GetWindowRect(&comboRect);
  int size = comboRect.Height();

  m_wndDirTree.GetSkin()->SetWindowPos(nullptr,rectClient.left,rectClient.top + cyTlb + size, rectClient.Width(),rectClient.Height() - cyTlb - 2 * size,flags);
  m_filterCBox.SetWindowPos(nullptr,rectClient.left,rectClient.Height() - size,rectClient.Width(),size,flags);
}

BOOL
DirTreeView::PreTranslateMessage(MSG* pMsg)
{
  return CDockablePane::PreTranslateMessage(pMsg);
}

void
DirTreeView::OnPaint()
{
  CPaintDC dc(this); // device context for painting

  CRect rectTree;
  m_wndDirTree.GetWindowRect(rectTree);
  ScreenToClient(rectTree);

  rectTree.InflateRect(1, 1);
  dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void
DirTreeView::OnSetFocus(CWnd* pOldWnd)
{
  CDockablePane::OnSetFocus(pOldWnd);

  OnActivate();
  m_wndDirTree.SetFocus();
}

void DirTreeView::OnChangeVisualStyle()
{
  m_ClassViewImages.DeleteImageList();

  UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

  CBitmap bmp;
  if (!bmp.LoadBitmap(uiBmpId))
  {
    TRACE(_T("Can't load bitmap: %x\n"), uiBmpId);
    ASSERT(FALSE);
    return;
  }

  BITMAP bmpObj;
  bmp.GetBitmap(&bmpObj);

  UINT nFlags = ILC_MASK;

  nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

  m_ClassViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
  m_ClassViewImages.Add(&bmp, RGB(255, 0, 0));

  m_wndDirTree.SetImageList(&m_ClassViewImages, TVSIL_NORMAL);

//   m_wndToolBar.CleanUpLockedImages();
//   m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SORT_24 : IDR_SORT, 0, 0, TRUE /* Locked */);
}

BOOL 
DirTreeView::SetCurPath(const CString& path)
{
  CString buffer = path;

  if(path.CompareNoCase(m_curDrivePath))
  {
    DisplayDrivers();
  }
  for (int i(0), count((int)m_driverPaths.GetSize()); i < count; i++)
  {
    if(buffer.Left(m_driverPaths[i].GetLength()).CompareNoCase(m_driverPaths[i]) == 0)
    {
      SelectDrive(m_driverPaths[i]);
      if(m_wndDirTree.SetSelPath(buffer))
      {
        return TRUE;
      }
    }
  }
  return FALSE;
}

const CString& 
DirTreeView::GetCurDrivePath() const
{
  return m_curDrivePath;
}

void 
DirTreeView::SetCurDrivePath(const CString& curDrivePath)
{
  SelectDrive(curDrivePath);
}

void 
DirTreeView::OnDrive_SetFocus()
{
  DisplayDrivers();
}

void 
DirTreeView::OnFpwRefreshDrivers()
{
  DisplayDrivers(TRUE);
}

void 
DirTreeView::OnDrive_SelChange()
{
  int inx = m_drivesCBox.GetCurSel();
  _ASSERTE(inx < m_driverPaths.GetSize());

  if (inx >= 0 && inx < m_driverPaths.GetSize())
  {
    SelectDrive(m_driverPaths.ElementAt(inx), FALSE);
  }
}

void 
DirTreeView::OnExplorerTree_DblClick(NMHDR*, LRESULT* pResult)
{
  HTREEITEM hCurSel = m_wndDirTree.GetNextItem(TVI_ROOT, TVGN_CARET);

  if (hCurSel)
  {
    CPoint point;
    ::GetCursorPos(&point);
    m_wndDirTree.ScreenToClient(&point);
    if (m_wndDirTree.HitTest(point, 0) == hCurSel)
    {
      CString path = m_wndDirTree.GetFullPath(hCurSel);
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

void 
DirTreeView::OnExplorerTree_RClick(NMHDR*, LRESULT* pResult)
{
  UINT uFlags;
  CPoint point;
  ::GetCursorPos(&point);
  m_wndDirTree.ScreenToClient(&point);
  HTREEITEM hItem = m_wndDirTree.HitTest(point, &uFlags);

  if (hItem && TVHT_ONITEM & uFlags)
  {
    if (m_wndDirTree.GetNextItem(TVI_ROOT, TVGN_CARET) != hItem)
    {
     m_wndDirTree.SelectItem(hItem);
    }
    CMenu menu;
    VERIFY(menu.LoadMenu(IDR_OE_EXPLORER_POPUP));
    CMenu* pPopup = menu.GetSubMenu(0);

    BOOL isFolder = IsFolder(m_wndDirTree.GetFullPath(hItem));
    if (!isFolder)
    {
      pPopup->SetDefaultItem(ID_FPW_OPEN);
    }
    pPopup->ModifyMenu(ID_FPW_OPEN, MF_BYCOMMAND, ID_FPW_OPEN, !isFolder ? _T("Open File\tDblClick") : _T("Open File Dalog...\tCtrl+DblClick"));
    pPopup->EnableMenuItem(ID_FPW_REFRESH, isFolder ? MF_BYCOMMAND | MF_ENABLED : MF_BYCOMMAND | MF_GRAYED);

    pPopup->RemoveMenu(2, MF_BYPOSITION);
    pPopup->RemoveMenu(2, MF_BYPOSITION);
    pPopup->RemoveMenu(2, MF_BYPOSITION);
    //pPopup->EnableMenuItem(ID_FPW_GOTO_DEF_DIR, isFolder ? MF_BYCOMMAND|MF_ENABLED : MF_BYCOMMAND|MF_GRAYED);
    //pPopup->EnableMenuItem(ID_FPW_SET_DEF_DIR, isFolder ? MF_BYCOMMAND|MF_ENABLED : MF_BYCOMMAND|MF_GRAYED);

    ASSERT(pPopup != NULL);
   m_wndDirTree.ClientToScreen(&point);
    pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);

    *pResult = 1;
  }
  else
  {
    *pResult = 0;
  }
}

void 
DirTreeView::OnDrivers_RClick(NMHDR*, LRESULT* pResult)
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

void 
DirTreeView::OnFpwOpen()
{
  HTREEITEM hCurSel = m_wndDirTree.GetNextItem(TVI_ROOT, TVGN_CARET);
  if (hCurSel)
  {
    CString path = m_wndDirTree.GetFullPath(hCurSel);
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
      CDocManagerExt::SetFileOpenPath(_T(""));
    }
  }
}

void 
DirTreeView::OnFpwRefresh()
{
  HTREEITEM hCurSel = m_wndDirTree.GetNextItem(TVI_ROOT, TVGN_CARET);
  if (hCurSel)
  {
    CString path = m_wndDirTree.GetFullPath(hCurSel);
    if (IsFolder(path))
    {
      m_wndDirTree.RefreshFolder(hCurSel);
    }
    else
    {
      MessageBeep((UINT)-1);
    }
  }
}

void 
DirTreeView::OnFpwSetWorDir()
{
  HTREEITEM hCurSel = m_wndDirTree.GetNextItem(TVI_ROOT, TVGN_CARET);
  if (hCurSel)
  {
    CString path = m_wndDirTree.GetFullPath(hCurSel);
    if (IsFolder(path))
    {
      m_wndDirTree.SetItemState(hCurSel, INDEXTOSTATEIMAGEMASK(1), TVIS_STATEIMAGEMASK);
    }
    else
    {
      MessageBeep((UINT)-1);
    }
  }
}

void
DirTreeView::DisplayFilters(BOOL force, BOOL curOnly)
{
  if (force)
  {
    if (!curOnly)
    {
      m_filterCBox.LockWindowUpdate();
    }
    m_filterCBox.SetRedraw(FALSE);
    m_filterCBox.ResetContent();

    for (int ind = 0; ind < m_filters.GetCount(); ++ind)
    {
      CString text = m_filters.GetAt(ind);
      text.GetBuffer();
      m_filterCBox.AddString(text);
      text.ReleaseBuffer();
    }
    m_filterCBox.SetRedraw(TRUE);
    if (!curOnly)
    {
      m_filterCBox.UnlockWindowUpdate();
    }
  }
}

void 
DirTreeView::AddToFilters(CString filter)
{
  for (int ind = 0; ind < m_filters.GetCount(); ++ind)
  {
    CString text = m_filters.GetAt(ind);
    if (!text.Find(filter) && (text.GetLength() == filter.GetLength()))
    {
      // OK, Already in the list
      return;
    }
  }
  m_filters.Add(filter);
  m_filterCBox.AddString(filter);
}

BOOL
DirTreeView::OnTooltipNotify(UINT wnd, NMHDR* tooltip, LRESULT* res)
{
  ((TOOLTIPTEXT*)tooltip)->szText[0] = 0;
  if (wnd == (UINT)(DWORD_PTR)m_filterCBox.m_hWnd)
  {
    _tcscpy_s(((TOOLTIPTEXT*)tooltip)->szText,80,_T("Filter for extensions of files in the explorer"));
    return (BOOL)(*res = true);
  }
  return (BOOL)(*res = false);
}

void 
DirTreeView::OnFilter_SetFocus()
{
  DisplayFilters();
}

void 
DirTreeView::OnFilter_SelChange()
{
  int inx = m_filterCBox.GetCurSel();
  if (inx != CB_ERR)
  {
    m_filterCBox.SetCurSel(inx);
  }
  CString filter;
  m_filterCBox.GetWindowText(filter);
  AddToFilters(filter);

  m_wndDirTree.SetFilter(filter);
  OnFpwRefresh();
}

