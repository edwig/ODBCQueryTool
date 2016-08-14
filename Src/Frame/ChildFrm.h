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

#ifndef __CHILDFRM_H__
#define __CHILDFRM_H__
#pragma once

class CMDIChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CMDIChildFrame)
public:
	CMDIChildFrame();
	virtual ~CMDIChildFrame();

  void  ShowQueryBar();
  static BOOL m_MaximizeFirstDocument;

protected:
	CSplitterWnd    m_wndSplitter;
  CWnd*           m_editorView; // The editor
  CWnd*           m_queryPanel; // CQueryPanelWnd
  CWnd*           m_gridView;   // The data grid   (CGridView)
  CWnd*           m_outptView;  // The output view (CGridView)
  CWnd*           m_statsView;  // The statistics view
  CWnd*           m_histoView;  // The history view

public:

	//{{AFX_VIRTUAL(CMDIChildFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

protected:
  // Workbook support
  afx_msg void OnDestroy ();
  afx_msg void OnMDIActivate (BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
  afx_msg LRESULT OnSetText (WPARAM, LPARAM lParam);
  afx_msg void OnUpdateShowQueryGrid(CCmdUI* pCmdUI);
  afx_msg void OnUpdateShowQueryStatistics(CCmdUI* pCmdUI);
  afx_msg void OnUpdateShowQueryOutput(CCmdUI* pCmdUI);
  afx_msg void OnUpdateShowQueryHistory(CCmdUI* pCmdUI);
  afx_msg void OnUpdateQuerySizeHeader(CCmdUI* pCmdUI);
  afx_msg void OnUpdateQuerySizeData(CCmdUI* pCmdUI);
  afx_msg void OnUpdateQuerySizeBoth(CCmdUI* pCmdUI);
  afx_msg void OnUpdateQueryBegin(CCmdUI* pCmdUI);
  afx_msg void OnUpdateQueryPrevPage(CCmdUI* pCmdUI);
  afx_msg void OnUpdateQueryPrev(CCmdUI* pCmdUI);
  afx_msg void OnUpdateQueryNext(CCmdUI* pCmdUI);
  afx_msg void OnUpdateQueryNextPage(CCmdUI* pCmdUI);
  afx_msg void OnUpdateQueryEnd(CCmdUI* pCmdUI);
  afx_msg void OnUpdateQueryExcel(CCmdUI* pCmdUI);
  afx_msg void OnUpdateQueryHtml(CCmdUI* pCmdUI);
  afx_msg void OnUpdateQueryTxt(CCmdUI* pCmdUI);
  
protected:
	//{{AFX_MSG(CMDIChildFrame)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif//__CHILDFRM_H__
