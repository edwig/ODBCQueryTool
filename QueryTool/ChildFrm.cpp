/*
*/

#include "pch.h"
#include "framework.h"
#include "ChildFrm.h"
#include "MainFrm.h"
#include "OEView.h"
#include "QueryTool.h"
#include "Query/GridView.h"
#include "Query/QueryPanelWnd.h"
#include "Query/QueryBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMDIChildFrame

BOOL CMDIChildFrame::m_MaximizeFirstDocument = TRUE;

IMPLEMENT_DYNCREATE(CMDIChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CMDIChildFrame, CMDIChildWndEx)
  ON_WM_DESTROY()
  ON_WM_MDIACTIVATE()
  // ON_MESSAGE(WM_SETTEXT, OnSetText)

  ON_UPDATE_COMMAND_UI(ID_QUERY_GRID,       OnUpdateShowQueryGrid)
  ON_UPDATE_COMMAND_UI(ID_QUERY_STATISTICS, OnUpdateShowQueryStatistics)
  ON_UPDATE_COMMAND_UI(ID_QUERY_OUTPUT,     OnUpdateShowQueryOutput)
  ON_UPDATE_COMMAND_UI(ID_QUERY_HISTORY,    OnUpdateShowQueryHistory)
  ON_UPDATE_COMMAND_UI(ID_QUERY_SIZEHEADER, OnUpdateQuerySizeHeader)
  ON_UPDATE_COMMAND_UI(ID_QUERY_SIZEDATA,   OnUpdateQuerySizeData)
  ON_UPDATE_COMMAND_UI(ID_QUERY_SIZEBOTH,   OnUpdateQuerySizeBoth)
  ON_UPDATE_COMMAND_UI(ID_QUERY_BEGIN,      OnUpdateQueryBegin)
  ON_UPDATE_COMMAND_UI(ID_QUERY_PREVPAGE,   OnUpdateQueryPrevPage)
  ON_UPDATE_COMMAND_UI(ID_QUERY_PREV,       OnUpdateQueryPrev)
  ON_UPDATE_COMMAND_UI(ID_QUERY_NEXT,       OnUpdateQueryNext)
  ON_UPDATE_COMMAND_UI(ID_QUERY_NEXTPAGE,   OnUpdateQueryNextPage)
  ON_UPDATE_COMMAND_UI(ID_QUERY_END,        OnUpdateQueryEnd)
  ON_UPDATE_COMMAND_UI(ID_QUERY_EXCEL,      OnUpdateQueryExcel)
  ON_UPDATE_COMMAND_UI(ID_QUERY_HTML,       OnUpdateQueryHtml)
  ON_UPDATE_COMMAND_UI(ID_QUERY_TXT,        OnUpdateQueryTxt)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMDIChildFrame construction/destruction

CMDIChildFrame::CMDIChildFrame() noexcept
{
}

CMDIChildFrame::~CMDIChildFrame()
{
}

BOOL CMDIChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
  if (!CMDIChildWndEx::PreCreateWindow(cs))
  {
    return FALSE;
  }
  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMDIChildFrame message handlers

BOOL CMDIChildFrame::OnCreateClient(LPCREATESTRUCT /* lpcs */, CCreateContext* pContext)
{
  BOOL  retVal = m_wndSplitter.CreateStatic(this, 2, 1);
  CRect rect;
  int   size;

  m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(COEditorView), CSize(0, 0), pContext);
  m_wndSplitter.CreateView(1, 0, RUNTIME_CLASS(CQueryPanelWnd), CSize(0, 0), pContext);
  SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
  size = (rect.bottom - (5 * 32)) / 2;
  m_wndSplitter.SetRowInfo(0, size, 0);
  m_wndSplitter.SetRowInfo(1, size, 0);

  // QUERY TOOL: Register each other as a couple
  m_editorView  = m_wndSplitter.GetPane(0, 0);
  m_queryPanel  = m_wndSplitter.GetPane(1, 0);
  m_gridView    = ((CQueryPanelWnd*)m_queryPanel)->GetQueryView();
  m_statsView   = ((CQueryPanelWnd*)m_queryPanel)->GetStatsView();
  m_outptView   = ((CQueryPanelWnd*)m_queryPanel)->GetOutptView();
  m_histoView   = ((CQueryPanelWnd*)m_queryPanel)->GetHistoView();

  ((CGridView*)   m_gridView)   ->RegisterEditor(m_editorView, TYPE_QUERY);
  ((CGridView*)   m_statsView)  ->RegisterEditor(m_editorView, TYPE_STATS);
  ((CGridView*)   m_outptView)  ->RegisterEditor(m_editorView, TYPE_OUTPT);
  ((CGridView*)   m_histoView)  ->RegisterEditor(m_editorView, TYPE_HISTO);
  ((COEditorView*)m_editorView) ->RegisterGrid  (m_gridView);
  ((COEditorView*)m_editorView) ->RegisterOutput(m_outptView);
  ((COEditorView*)m_editorView) ->RegisterStatistics(m_statsView);
  ((COEditorView*)m_editorView) ->RegisterHistory(m_histoView);
  ((COEditorView*)m_editorView) ->RegisterQueryPanel((CQueryPanelWnd*)m_queryPanel);

  ((CQueryPanelWnd*)m_queryPanel)->RegisterFrame(this);
  CToolBar* dialogBar = ((CQueryPanelWnd*)m_queryPanel)->GetDialogBar();
  ShowControlBar(dialogBar, TRUE, TRUE);

  if (m_MaximizeFirstDocument)
  {
    int i = 0;
    CWnd* other = GetParent()->GetWindow(GW_CHILD);
    for (i = 0; other; )
    {
      if (other->IsKindOf(RUNTIME_CLASS(CMDIChildFrame))
        && other != this)
      {
        i++;
        break;
      }
      other = other->GetNextWindow();
    }
    if (!i)
    {
      GetParent()->PostMessage(WM_MDIMAXIMIZE, (WPARAM)m_hWnd);
    }
  }
  // Workbook implementation
//   ASSERT_KINDOF(CMainFrame, GetMDIFrame());
//   ((CMainFrame*)GetMDIFrame())->OnCreateChild(this);

  return retVal;
}

// Workbook implementation

void CMDIChildFrame::OnDestroy()
{
  CMDIChildWndEx::OnDestroy();

  // ((CMainFrame*)GetMDIFrame())->OnDestroyChild(this);
}

void CMDIChildFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
  CMDIChildWndEx::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);

//   if (bActivate && this == pActivateWnd)
//   {
//     ((CMainFrame*)GetMDIFrame())->OnActivateChild(this);
//   }
}

// LRESULT CMDIChildFrame::OnSetText(WPARAM, LPARAM lParam)
// {
//   Default();
// 
//   ((CMainFrame*)GetMDIFrame())->OnRenameChild(this, (LPCTSTR)lParam);
// 
//   return TRUE;
// }

void
CMDIChildFrame::ShowQueryBar()
{
  CToolBar* dialogBar = ((CQueryPanelWnd*)m_queryPanel)->GetDialogBar();
  ShowControlBar(dialogBar, TRUE, FALSE);
  dialogBar->ShowWindow(SW_SHOW);
}

void
CMDIChildFrame::OnUpdateShowQueryGrid(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(true);
}

void
CMDIChildFrame::OnUpdateShowQueryStatistics(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(true);
}

void
CMDIChildFrame::OnUpdateShowQueryOutput(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(true);
}

void
CMDIChildFrame::OnUpdateShowQueryHistory(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(true);
}

void
CMDIChildFrame::OnUpdateQuerySizeHeader(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(true);
}

void
CMDIChildFrame::OnUpdateQuerySizeData(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(true);
}

void
CMDIChildFrame::OnUpdateQuerySizeBoth(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(true);
}

void
CMDIChildFrame::OnUpdateQueryBegin(CCmdUI* pCmdUI)
{
  bool atbegin = ((CQueryPanelWnd*)m_queryPanel)->GetActivePane().AtBegin();
  pCmdUI->Enable(!atbegin);
}

void
CMDIChildFrame::OnUpdateQueryPrevPage(CCmdUI* pCmdUI)
{
  bool atbegin = ((CQueryPanelWnd*)m_queryPanel)->GetActivePane().AtBegin();
  pCmdUI->Enable(!atbegin);
}

void
CMDIChildFrame::OnUpdateQueryPrev(CCmdUI* pCmdUI)
{
  bool atbegin = ((CQueryPanelWnd*)m_queryPanel)->GetActivePane().AtBegin();
  pCmdUI->Enable(!atbegin);
}

void
CMDIChildFrame::OnUpdateQueryNext(CCmdUI* pCmdUI)
{
  bool atend = ((CQueryPanelWnd*)m_queryPanel)->GetActivePane().AtEnd();
  pCmdUI->Enable(!atend);
}

void
CMDIChildFrame::OnUpdateQueryNextPage(CCmdUI* pCmdUI)
{
  bool atend = ((CQueryPanelWnd*)m_queryPanel)->GetActivePane().AtEnd();
  pCmdUI->Enable(!atend);
}

void
CMDIChildFrame::OnUpdateQueryEnd(CCmdUI* pCmdUI)
{
  bool atend = ((CQueryPanelWnd*)m_queryPanel)->GetActivePane().AtEnd();
  pCmdUI->Enable(!atend);
}

void
CMDIChildFrame::OnUpdateQueryExcel(CCmdUI* pCmdUI)
{
  bool filled = ((CQueryPanelWnd*)m_queryPanel)->GetActivePane().IsFilled();
  pCmdUI->Enable(filled);
}

void
CMDIChildFrame::OnUpdateQueryHtml(CCmdUI* pCmdUI)
{
  bool filled = ((CQueryPanelWnd*)m_queryPanel)->GetActivePane().IsFilled();
  pCmdUI->Enable(filled);
}

void
CMDIChildFrame::OnUpdateQueryTxt(CCmdUI* pCmdUI)
{
  bool filled = ((CQueryPanelWnd*)m_queryPanel)->GetActivePane().IsFilled();
  pCmdUI->Enable(filled);
}
