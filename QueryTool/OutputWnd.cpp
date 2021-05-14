
#include "pch.h"
#include "framework.h"

#include "OutputWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "OEView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputBar

COutputWnd::COutputWnd() noexcept
{
}

COutputWnd::~COutputWnd()
{
}

BEGIN_MESSAGE_MAP(COutputWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int COutputWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create tabs window:
	if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1))
	{
		TRACE0("Failed to create output tab window\n");
		return -1;      // fail to create
	}

	// Create output panes:
	const DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;

	if(!m_wndFind1.Create(rectDummy,&m_wndTabs,2,dwStyle) ||
		 !m_wndFind2.Create(rectDummy,&m_wndTabs,3,dwStyle) ||
		 !m_wndFind3.Create(rectDummy,&m_wndTabs,4,dwStyle))
	{
		TRACE0("Failed to create output windows\n");
		return -1;      // fail to create
	}


	InitGridEmpty(m_wndFind1);
	InitGridEmpty(m_wndFind2);
	InitGridEmpty(m_wndFind3);

	CString strTabName;
	BOOL bNameValid;

	// Attach list windows to tab:
	bNameValid = strTabName.LoadString(IDS_FIND_TAB1);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(m_wndFind1.GetSkin(), strTabName, (UINT)0,FALSE);
	bNameValid = strTabName.LoadString(IDS_FIND_TAB2);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(m_wndFind2.GetSkin(), strTabName, (UINT)1,FALSE);
	bNameValid = strTabName.LoadString(IDS_FIND_TAB3);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(m_wndFind3.GetSkin(), strTabName, (UINT)2,FALSE);

	return 0;
}

void
COutputWnd::InitGridEmpty(COutputList& p_list)
{
	p_list.InitSkin();
  p_list.SetFont(&STYLEFONTS.ListTextFont);

  // Set standard
  p_list.SetEditable(FALSE);
  p_list.EnableDragAndDrop(TRUE);
  p_list.SetHeaderSort(TRUE);

  // Delete previous results
	p_list.DeleteNonFixedRows();

  // Set empty grid
  try
  {
    p_list.SetRowCount(1);
    p_list.SetColumnCount(3);
    p_list.SetFixedRowCount(1);
  }
  catch (CMemoryException* e)
  {
    e->ReportError();
    e->Delete();
    return;
  }
  GV_ITEM item;
  item.mask = GVIF_TEXT | GVIF_FORMAT;
  item.row = 0;
  item.col = 0;
  item.nFormat = DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX;

  item.strText = "File";
  p_list.SetItem(&item);
  p_list.SetColumnWidth(0, 200);

  item.col = 2;
  item.strText = "Text";
  p_list.SetItem(&item);
  p_list.SetColumnWidth(0, 200);

  item.col = 1;
  item.strText = "Line";
  item.nFormat |= DT_SORT_NUMERIC;
  p_list.SetItem(&item);
  p_list.SetColumnWidth(0, 200);

  // Sync with the outside world
  p_list.AutoSize();
  p_list.AutoSizeColumns(GVS_BOTH);
  p_list.ExpandLastColumn();
}

void COutputWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// Tab control should cover the whole client area:
	m_wndTabs.SetWindowPos (nullptr, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void COutputWnd::AdjustHorzScroll(CListBox& wndListBox)
{
	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(&afxGlobalData.fontRegular);

	int cxExtentMax = 0;

	for (int i = 0; i < wndListBox.GetCount(); i ++)
	{
		CString strItem;
		wndListBox.GetText(i, strItem);

		cxExtentMax = max(cxExtentMax, (int)dc.GetTextExtent(strItem).cx);
	}

	wndListBox.SetHorizontalExtent(cxExtentMax);
	dc.SelectObject(pOldFont);
}

StyleGridCtrl*
COutputWnd::GetEmptyFindGrid()
{
	if (++m_current > OUTPUT_PANES)
	{
		m_current = 1;
	}
	switch (m_current)
	{
		case 1:		m_wndFind1.DeleteNonFixedRows();
							m_wndTabs.SetActiveTab(0);
							return &m_wndFind1;
		case 2:		m_wndFind2.DeleteNonFixedRows();
							m_wndTabs.SetActiveTab(1);
							return &m_wndFind2;
		case 3:		m_wndFind3.DeleteNonFixedRows();
							m_wndTabs.SetActiveTab(2);
							return &m_wndFind3;
		default:	break;
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////
// COutputList1

COutputList::COutputList() noexcept
{
}

COutputList::~COutputList()
{
}

BEGIN_MESSAGE_MAP(COutputList, StyleGridCtrl)
	ON_WM_CONTEXTMENU()
  ON_NOTIFY_REFLECT(NM_CLICK,OnFindFilesClicked)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutputList message handlers

void COutputList::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_OUTPUT_POPUP);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}

	SetFocus();
}

void COutputList::OnViewOutput()
{
	CDockablePane* pParentBar = DYNAMIC_DOWNCAST(CDockablePane, GetOwner());
	CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());

	if (pMainFrame != nullptr && pParentBar != nullptr)
	{
		pMainFrame->SetFocus();
		pMainFrame->ShowPane(pParentBar, FALSE, FALSE, FALSE);
		pMainFrame->RecalcLayout();

	}
}

void
COutputList::OnFindFilesClicked(NMHDR* pNotifyStruct, LRESULT* result)
{
  int   maxrow = GetRowCount();
  CCellID cell = GetFocusCell();
  int row = cell.row;

  if(row >= maxrow - 2)
  {
    // Last two rows contain counting
    return;
  }
  CString fileName = GetItemText(row, 0);
  int   lineNumber = atoi(GetItemText(row, 1));


  if (fileName.IsEmpty())
  {
    AfxMessageBox("Cannot find file: " + fileName, MB_OK | MB_ICONEXCLAMATION);
    return;
  }
  CDocument* doc = AfxGetApp()->OpenDocumentFile(fileName);
  if (doc)
  {
    // OK, doc was opened
    POSITION pos = doc->GetFirstViewPosition();
    while (pos != NULL)
    {
      CView* pView = doc->GetNextView(pos);
      COEditorView* view = dynamic_cast<COEditorView*> (pView);
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

