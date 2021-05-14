/* 
    Copyright (C) 2004 - 2017 Edwig Huisman

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

#include "pch.h"
//#include <Shlwapi.h>
#include <string>
#include <COMMON/ExceptionHelper.h>
#include <WinException.h>
#include <Query/QueryPanelWnd.h>
#include "COMMON/GUICommandDictionary.h"

///////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CQueryPanelWnd, CView)

BEGIN_MESSAGE_MAP(CQueryPanelWnd, CView)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_COMMAND(ID_QUERY_GRID,      OnShowQueryGrid)
    ON_COMMAND(ID_QUERY_STATISTICS,OnShowQueryStatistics)
    ON_COMMAND(ID_QUERY_OUTPUT,    OnShowQueryOutput)
    ON_COMMAND(ID_QUERY_HISTORY,   OnShowQueryHistory)
    ON_COMMAND(ID_QUERY_SIZEHEADER,OnQuerySizeHeader)
    ON_COMMAND(ID_QUERY_SIZEDATA,  OnQuerySizeData)
    ON_COMMAND(ID_QUERY_SIZEBOTH,  OnQuerySizeBoth)
    ON_COMMAND(ID_QUERY_BEGIN,     OnQueryBegin)
    ON_COMMAND(ID_QUERY_PREVPAGE,  OnQueryPrevPage)
    ON_COMMAND(ID_QUERY_PREV,      OnQueryPrev)
    ON_COMMAND(ID_QUERY_NEXT,      OnQueryNext)
    ON_COMMAND(ID_QUERY_NEXTPAGE,  OnQueryNextPage)
    ON_COMMAND(ID_QUERY_END,       OnQueryEnd)
    ON_COMMAND(ID_QUERY_EXCEL,     OnQueryExcel)
    ON_COMMAND(ID_QUERY_HTML,      OnQueryHtml)
    ON_COMMAND(ID_QUERY_TXT,       OnQueryTxt)
END_MESSAGE_MAP()

CQueryPanelWnd::CQueryPanelWnd()
               :CView()
{
}

CQueryPanelWnd::~CQueryPanelWnd()
{
  CView::~CView();
}

void CQueryPanelWnd::ChangePanel (int nTab)
{
  _CHECK_AND_THROW_(nTab < QPW_NUM_OF_PANEL, "Invalid index of a query views.");

  m_queryView.ShowWindow(nTab == QPW_QUERY_VIEW      ? SW_SHOW : SW_HIDE);
  m_statsView.ShowWindow(nTab == QPW_STATISTICS_VIEW ? SW_SHOW : SW_HIDE);
  m_outptView.ShowWindow(nTab == QPW_OUTPUT_VIEW     ? SW_SHOW : SW_HIDE);
  m_histoView.ShowWindow(nTab == QPW_HISTORY_VIEW    ? SW_SHOW : SW_HIDE);

  if (nTab == QPW_QUERY_VIEW && !m_isQueryInitialized)
  {
    CWaitCursor wait;
    UpdateWindow();

    // Do init
    m_isQueryInitialized = TRUE;
  }
  m_activePane = nTab;
}

CGridView&
CQueryPanelWnd::GetActivePane()
{
  switch(m_activePane)
  {
    case QPW_QUERY_VIEW:      return m_queryView;
    case QPW_STATISTICS_VIEW: return m_statsView;
    case QPW_OUTPUT_VIEW:     return m_outptView;
    case QPW_HISTORY_VIEW:    return m_histoView;
  }
  return m_queryView;
}

void 
CQueryPanelWnd::PostNcDestroy()
{
  delete this;
}

static UINT BASED_CODE QueryIndicators[] =
{
  ID_QUERY_GRID
 ,ID_SEPARATOR        // 1 : "Query"
 ,ID_QUERY_OUTPUT
 ,ID_SEPARATOR        // 3 : "Output"
 ,ID_QUERY_STATISTICS
 ,ID_SEPARATOR        // 5 : "Statistics"
 ,ID_QUERY_HISTORY
 ,ID_SEPARATOR        // 7 " "History"

 ,ID_SEPARATOR
 ,ID_QUERY_SIZEHEADER
 ,ID_QUERY_SIZEDATA
 ,ID_QUERY_SIZEBOTH

 ,ID_SEPARATOR
 ,ID_QUERY_BEGIN
 ,ID_QUERY_PREVPAGE
 ,ID_QUERY_PREV
 ,ID_QUERY_NEXT
 ,ID_QUERY_NEXTPAGE
 ,ID_QUERY_END

 ,ID_SEPARATOR
 ,ID_QUERY_HTML
 ,ID_QUERY_EXCEL
 ,ID_QUERY_TXT
 ,ID_QUERY_SETTINGS
};


// FilePanelWnd message handlers
BOOL CQueryPanelWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  // Don't: Leads to multiple destruct of views
  //if (CView::OnCreate(lpCreateStruct) == -1)
  //     return -1;
  CRect rect;
  GetClientRect(&rect);

  if (!m_queryBar.CreateEx(this
                           ,TBSTYLE_FLAT
                           ,WS_CHILD|CBRS_TOP|CBRS_TOOLTIPS|CBRS_SIZE_DYNAMIC|CBRS_FLYBY
                           ,rect
                           ,IDR_QUERYBAR))
  {
    TRACE0("Failed to create QueryToolBar\n");
    return -1;      // Fail to create.
  }
  else
  {
    m_queryBar.LoadToolBar(IDR_QUERYBAR);
    m_queryBar.SetButtons(QueryIndicators,sizeof(QueryIndicators) / sizeof(UINT));

    //  Font maken
    LOGFONT logFont;
    memset(&logFont, 0, sizeof(logFont));
    logFont.lfHeight = -12;
    logFont.lfPitchAndFamily = VARIABLE_PITCH | FF_SWISS;
    lstrcpy(logFont.lfFaceName, "MS Sans Serif");
    if (!m_queryBar.m_font.CreateFontIndirect(&logFont))
    {
      TRACE0("Cannot make the font for the querybar");
    }
    // Buton info text
    m_queryBar.SetButtonInfo(1, 0, TBBS_SEPARATOR, 50);
    m_queryBar.GetItemRect(1, &rect);
    rect.DeflateRect(2,4,2,2);
    if(!m_queryBar.m_textQuery.Create("Query",SS_CENTER,rect,&m_queryBar,0))
    {
      TRACE0("Cannot create static Query text");
    }

    m_queryBar.SetButtonInfo(3, 0, TBBS_SEPARATOR, 50);
    m_queryBar.GetItemRect(3,&rect);
    rect.DeflateRect(2,4,2,2);
    if(!m_queryBar.m_textOuptput.Create("Output",SS_CENTER,rect,&m_queryBar,0))
    {
      TRACE0("Cannot create static output text");
    }

    m_queryBar.SetButtonInfo(5, 0, TBBS_SEPARATOR, 50);
    m_queryBar.GetItemRect(5, &rect);
    rect.DeflateRect(2,4,2,2);
    if(!m_queryBar.m_textStatistics.Create("Statistics",SS_CENTER,rect,&m_queryBar,0))
    {
      TRACE0("Cannot create static statistics text");
    }
 
    m_queryBar.SetButtonInfo(7, 0 , TBBS_SEPARATOR, 50);
    m_queryBar.GetItemRect(7,&rect);
    rect.DeflateRect(2,4,2,2);
    if(!m_queryBar.m_textHistory.Create("History",SS_CENTER,rect,&m_queryBar,0))
    {
      TRACE0("Cannot create static history text");
    }

    // Show labels    
    m_queryBar.m_textQuery.SetFont(&m_queryBar.m_font);
    m_queryBar.m_textStatistics.SetFont(&m_queryBar.m_font);
    m_queryBar.m_textOuptput.SetFont(&m_queryBar.m_font);
    m_queryBar.m_textHistory.SetFont(&m_queryBar.m_font);

    m_queryBar.m_textQuery.ShowWindow(SW_SHOW);
    m_queryBar.m_textStatistics.ShowWindow(SW_SHOW);
    m_queryBar.m_textOuptput.ShowWindow(SW_SHOW);
    m_queryBar.m_textHistory.ShowWindow(SW_SHOW);

  }
 
  if (!m_queryView.Create(NULL
                         ,"QueryView"
                         ,WS_CHILD|WS_TABSTOP|WS_VISIBLE
                         ,rect
                         ,this
                         ,ID_QUERY_VIEW
                         ,NULL))
  {
    TRACE0("Failed to create query grid\n");
    return -1;
  }
  if (!m_outptView.Create(NULL
                         ,"OutputView"
                         ,WS_CHILD|WS_TABSTOP|WS_VISIBLE
                         ,rect
                         ,this
                         ,ID_OUTPUT_VIEW
                         ,NULL))
  {
    TRACE0("Failed to create output view\n");
    return -1;
  }
  if (!m_statsView.Create(NULL
                         ,"StatisticsView"
                         ,WS_CHILD|WS_TABSTOP|WS_VISIBLE
                         ,rect
                         ,this
                         ,ID_STATISTICS_VIEW
                         ,NULL))
  {
    TRACE0("Failed to create statistics view\n");
    return -1;
  }

  if (!m_histoView.Create(NULL
                         ,"HistoryView"
                         ,WS_CHILD|WS_TABSTOP|WS_VISIBLE
                         ,rect
                         ,this
                         ,ID_HISTORY_VIEW
                         ,NULL))
  {
    TRACE0("Failed to create history view\n");
    return -1;
  }
  // Activate first panel
  ChangePanel(QPW_QUERY_VIEW);
  return 0;
}

void CQueryPanelWnd::OnSize(UINT nType, int cx, int cy)
{
  CView::OnSize(nType, cx, cy);

  if (cx > 0 && 
      cy > 0 && 
      nType != SIZE_MAXHIDE && 
      nType != SIZE_MINIMIZED) 
  {
    CRect rect;
    GetClientRect(&rect);
    // rect.InflateRect(3, 0);

    HDWP hdwp = ::BeginDeferWindowPos(10);
    ::DeferWindowPos(hdwp, m_queryBar,  0, rect.left, rect.top,      rect.Width(), 32,                 SWP_NOZORDER);
    ::DeferWindowPos(hdwp, m_queryView, 0, rect.left, rect.top + 32, rect.Width(), rect.Height() - 32, SWP_NOZORDER);
    ::DeferWindowPos(hdwp, m_statsView, 0, rect.left, rect.top + 32, rect.Width(), rect.Height() - 32, SWP_NOZORDER);
    ::DeferWindowPos(hdwp, m_outptView, 0, rect.left, rect.top + 32, rect.Width(), rect.Height() - 32, SWP_NOZORDER);
    ::DeferWindowPos(hdwp, m_histoView, 0, rect.left, rect.top + 32, rect.Width(), rect.Height() - 32, SWP_NOZORDER);
    ::EndDeferWindowPos(hdwp);

    m_parentFrame->ShowQueryBar();
  }
}

void CQueryPanelWnd::OnOpenFiles_RClick (NMHDR* pNMHDR, LRESULT* pResult)
{
  NMITEMACTIVATE* pItem = (NMITEMACTIVATE*)pNMHDR;
  if (pItem && pItem->iItem != -1)
  {
    CPoint point(pItem->ptAction);
    //m_openFilesList.ClientToScreen(&point);

    CMenu menu;
    VERIFY(menu.LoadMenu(IDR_OE_WORKBOOK_POPUP));
    CMenu* pPopup = menu.GetSubMenu(0);

    ASSERT(pPopup != NULL);
    ASSERT_KINDOF(CView, AfxGetMainWnd());
    Common::GUICommandDictionary::AddAccelDescriptionToMenu(pPopup->m_hMenu);
    pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, AfxGetMainWnd());

    *pResult = 1;
  }
  else
  {
    *pResult = 0;
  }
}

void CQueryPanelWnd::OnDraw(CDC* pDC)
{
	// TODO: add draw code for native data here
}

void 
CQueryPanelWnd::OnShowQueryGrid()
{
  ChangePanel(QPW_QUERY_VIEW);
}

void 
CQueryPanelWnd::OnShowQueryStatistics()
{
  ChangePanel(QPW_STATISTICS_VIEW);
}

void
CQueryPanelWnd::OnShowQueryOutput()
{
  ChangePanel(QPW_OUTPUT_VIEW);
}

void 
CQueryPanelWnd::OnShowQueryHistory()
{
  ChangePanel(QPW_HISTORY_VIEW);
}

void
CQueryPanelWnd::OnQuerySizeHeader()
{
  GetActivePane().SizeColumns(GVS_HEADER);
}

void
CQueryPanelWnd::OnQuerySizeData()
{
  GetActivePane().SizeColumns(GVS_DATA);
}

void
CQueryPanelWnd::OnQuerySizeBoth()
{
  GetActivePane().SizeColumns(GVS_BOTH);
}

void 
CQueryPanelWnd::OnQueryBegin()
{
  GetActivePane().GotoQueryBegin();
}

void 
CQueryPanelWnd::OnQueryPrevPage()
{
  GetActivePane().GotoQueryPrevPage();
}

void 
CQueryPanelWnd::OnQueryPrev()
{
  GetActivePane().GotoQueryPrev();
}

void 
CQueryPanelWnd::OnQueryNext()
{
  GetActivePane().GotoQueryNext();
}

void 
CQueryPanelWnd::OnQueryNextPage()
{
  GetActivePane().GotoQueryNextPage();
}

void 
CQueryPanelWnd::OnQueryEnd()
{
  GetActivePane().GotoQueryEnd();
}

void 
CQueryPanelWnd::OnQueryExcel()
{
  GetActivePane().ExportToExcel();
}

void
CQueryPanelWnd::OnQueryHtml()
{
  GetActivePane().ExportToHTML();
}

void
CQueryPanelWnd::OnQueryTxt()
{
  GetActivePane().ExportToTXT();
}