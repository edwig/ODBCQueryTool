////////////////////////////////////////////////////////////////////////
//
// File: ChildFrm.h
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
#pragma once

class CMDIChildFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CMDIChildFrame)
public:
	CMDIChildFrame() noexcept;
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

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

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
	DECLARE_MESSAGE_MAP()
};

