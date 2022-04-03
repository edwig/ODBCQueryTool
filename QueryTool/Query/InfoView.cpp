////////////////////////////////////////////////////////////////////////
//
// File: InfoView.cpp
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
#include "InfoView.h"
#include "QueryTool.h"
#include "SQLComponents.h"
#include "SQLDatabase.h"
#include "sqlext.h"
#include "Resource.h"
#include "OEDocument.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
//
// Info Dockable View
//
//////////////////////////////////////////////////////////////////////////

class CInfoViewMenuButton : public CMFCToolBarMenuButton
{
  friend class CClassView;

  DECLARE_SERIAL(CInfoViewMenuButton)

public:
  CInfoViewMenuButton(HMENU hMenu = nullptr) noexcept : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
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

IMPLEMENT_SERIAL(CInfoViewMenuButton, CMFCToolBarMenuButton, 1)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

InfoView::InfoView() noexcept
{
  m_nCurrSort = ID_SORTING_GROUPBYTYPE;
  m_isOdbcInitialized = false;
}

InfoView::~InfoView()
{
}

BEGIN_MESSAGE_MAP(InfoView, CDockablePane)
  ON_WM_CREATE()
  ON_WM_SIZE()
  ON_WM_CONTEXTMENU()
  //   ON_COMMAND(ID_CLASS_ADD_MEMBER_FUNCTION, OnClassAddMemberFunction)
  //   ON_COMMAND(ID_CLASS_ADD_MEMBER_VARIABLE, OnClassAddMemberVariable)
  //   ON_COMMAND(ID_CLASS_DEFINITION, OnClassDefinition)
  //   ON_COMMAND(ID_CLASS_PROPERTIES, OnClassProperties)
  //   ON_COMMAND(ID_NEW_FOLDER, OnNewFolder)
  ON_WM_PAINT()
  ON_WM_SETFOCUS()
  ON_COMMAND_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnSort)
  ON_UPDATE_COMMAND_UI_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnUpdateSort)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassView message handlers

int InfoView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CDockablePane::OnCreate(lpCreateStruct) == -1)
    return -1;

  CRect rectDummy;
  rectDummy.SetRectEmpty();

  // Create views:
  const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

  m_wndInfoTree.SetDirectInit(false);
  if (!m_wndInfoTree.Create(dwViewStyle, rectDummy, this, 2))
  {
    TRACE0("Failed to create ODBC Info View\n");
    return -1;      // fail to create
  }
  m_wndInfoTree.InitSkin();

  // Load images:
//   m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT);
//   m_wndToolBar.LoadToolBar(IDR_SORT, 0, 0, TRUE /* Is locked */);

  OnChangeVisualStyle();

  //   m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
  //   m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

  //  m_wndToolBar.SetOwner(this);

    // All commands will be routed via this control , not via the parent frame:
  //  m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

  //   CMenu menuSort;
  //   menuSort.LoadMenu(IDR_POPUP_SORT);

  //  m_wndToolBar.ReplaceButton(ID_SORT_MENU, CInfoViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));

  //   CInfoViewMenuButton* pButton = DYNAMIC_DOWNCAST(CInfoViewMenuButton, m_wndToolBar.GetButton(0));

  //   if (pButton != nullptr)
  //   {
  //     pButton->m_bText = FALSE;
  //     pButton->m_bImage = TRUE;
  //     pButton->SetImage(GetCmdMgr()->GetCmdImage(m_nCurrSort));
  //     pButton->SetMessageWnd(this);
  //   }

    // Fill in some static tree view data (dummy code, nothing magic here)
  FillInfoView();
  AdjustLayout();

  return 0;
}

void
InfoView::OnSize(UINT nType, int cx, int cy)
{
  CDockablePane::OnSize(nType, cx, cy);
  AdjustLayout();
}

void InfoView::FillInfoView()
{
  CWaitCursor sighAndWait;
  QueryToolApp* app = (QueryToolApp*)AfxGetApp();

  // Get option for the preferred META-SQL
  const OpenEditor::SettingsManager& manager = COEDocument::GetSettingsManager();
  const OpenEditor::GlobalSettings& settings = manager.GetGlobalSettings();
  bool  prefer = settings.GetPreferODBCMetaSQL();
  // Set the option
  SQLInfoDB* info = app->GetDatabase().GetSQLInfoDB();
  if (info)
  {
    info->SetPreferODBC(prefer);
  }
  // Connect the database to the tree
  m_wndInfoTree.SetDatabase(&(app->GetDatabase()));
  m_wndInfoTree.MakeTreeInfo();

  m_isOdbcInitialized = true;
}

void InfoView::OnContextMenu(CWnd* pWnd, CPoint point)
{
  CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndInfoTree;
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

void
InfoView::AdjustLayout()
{
  if (GetSafeHwnd() == nullptr)
  {
    return;
  }

  CRect rectClient;
  GetClientRect(rectClient);

  int cyTlb = 0;
  //int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;
  //m_wndToolBar .SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
  m_wndInfoTree.GetSkin()->SetWindowPos(nullptr, rectClient.left, rectClient.top + cyTlb, rectClient.Width(), rectClient.Height() - cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
}

void
InfoView::ReportCapabilities(CString& filename)
{
  QueryToolApp* app = (QueryToolApp*)AfxGetApp();
  if (app->DatabaseIsOpen())
  {
    CWaitCursor take_a_deep_sigh;
    m_wndInfoTree.MakeTreeInfo();
    m_wndInfoTree.ReportAllCapabilities(filename);
  }
}



BOOL
InfoView::PreTranslateMessage(MSG* pMsg)
{
  return CDockablePane::PreTranslateMessage(pMsg);
}

void InfoView::OnSort(UINT id)
{
  if (m_nCurrSort == id)
  {
    return;
  }

  //   m_nCurrSort = id;
  // 
  //   CInfoViewMenuButton* pButton = DYNAMIC_DOWNCAST(CInfoViewMenuButton, m_wndToolBar.GetButton(0));
  //   if (pButton != nullptr)
  //   {
  //     pButton->SetImage(GetCmdMgr()->GetCmdImage(id));
  //     m_wndToolBar.Invalidate();
  //     m_wndToolBar.UpdateWindow();
  //   }
}

void
InfoView::OnUpdateSort(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(pCmdUI->m_nID == m_nCurrSort);
}

void
InfoView::OnPaint()
{
  CPaintDC dc(this); // device context for painting

  CRect rectTree;
  m_wndInfoTree.GetWindowRect(rectTree);
  ScreenToClient(rectTree);

  rectTree.InflateRect(1, 1);
  dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void
InfoView::OnSetFocus(CWnd* pOldWnd)
{
  CDockablePane::OnSetFocus(pOldWnd);

  m_wndInfoTree.SetFocus();
}

void InfoView::OnChangeVisualStyle()
{
  m_ClassViewImages.DeleteImageList();

  UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_CLASS_VIEW_24 : IDB_CLASS_VIEW;

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

  m_wndInfoTree.SetImageList(&m_ClassViewImages, TVSIL_NORMAL);

  //   m_wndToolBar.CleanUpLockedImages();
  //   m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SORT_24 : IDR_SORT, 0, 0, TRUE /* Locked */);
}

