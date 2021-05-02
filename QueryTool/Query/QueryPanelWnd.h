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

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __QueryPanelWnd_h__
#define __QueryPanelWnd_h__

#include "Common/FakeRClick.h"
#include "Query/GridView.h"
#include "Query/QueryBar.h"
#include "Frame/ChildFrm.h"

// Defines which pane we mean
#define QPW_QUERY_VIEW         0
#define QPW_STATISTICS_VIEW    1
#define QPW_OUTPUT_VIEW        2
#define QPW_HISTORY_VIEW       3
#define QPW_NUM_OF_PANEL       4

#define ID_QUERY_VIEW           2001
#define ID_STATISTICS_VIEW      2002
#define ID_OUTPUT_VIEW          2003
#define ID_HISTORY_VIEW         2004

class CQueryPanelWnd : public CView
{
// Constructors
public:
  CQueryPanelWnd();
  virtual ~CQueryPanelWnd();
	DECLARE_DYNCREATE(CQueryPanelWnd);

public:
  void       ChangePanel(int);
  void       OnOpenFiles_RClick (NMHDR* pNMHDR, LRESULT* pResult);
  CGridView* GetQueryView();
  CGridView* GetStatsView();
  CGridView* GetOutptView();
  CGridView* GetHistoView();
  void       OnDraw(CDC* pDC);
  CToolBar*  GetDialogBar();
  void       RegisterFrame(CMDIChildFrame* frame); 
  CGridView& GetActivePane();
protected:
    bool         m_isQueryInitialized;
    bool         m_isStatsInitialized;
    bool         m_isOutptInitialized;
    bool         m_isHistoInitialized;

    int             m_activePane;
    QueryBar        m_queryBar;
    CGridView       m_queryView;
    CGridView       m_statsView;
    CGridView       m_outptView;
    CGridView       m_histoView;
    CMDIChildFrame* m_parentFrame;

protected:
	DECLARE_MESSAGE_MAP()
	void PostNcDestroy();

public:
  afx_msg int  OnCreate (LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnSize (UINT nType, int cx, int cy);
  afx_msg void OnTimer(UINT nIDEvent);
  afx_msg void OnShowQueryGrid();
  afx_msg void OnShowQueryStatistics();
  afx_msg void OnShowQueryOutput();
  afx_msg void OnShowQueryHistory();
  afx_msg void OnQuerySizeHeader();
  afx_msg void OnQuerySizeData();
  afx_msg void OnQuerySizeBoth();
  afx_msg void OnQueryBegin();
  afx_msg void OnQueryPrevPage();
  afx_msg void OnQueryPrev();
  afx_msg void OnQueryNext();
  afx_msg void OnQueryNextPage();
  afx_msg void OnQueryEnd();
  afx_msg void OnQueryExcel();
  afx_msg void OnQueryHtml();
  afx_msg void OnQueryTxt();
};

inline CGridView*
CQueryPanelWnd::GetQueryView()
{
  return &m_queryView;
}

inline CGridView* 
CQueryPanelWnd::GetStatsView()
{
  return &m_statsView;
}

inline CGridView* 
CQueryPanelWnd::GetOutptView()
{
  return &m_outptView;
}

inline CGridView* 
CQueryPanelWnd::GetHistoView()
{
  return &m_histoView;
}

inline CToolBar*    
CQueryPanelWnd::GetDialogBar()
{
  return &m_queryBar;
}

inline void
CQueryPanelWnd::RegisterFrame(CMDIChildFrame* frame)
{
  m_parentFrame = frame;
}

#endif//__QueryPanelWnd_h__