/* 
* 
*/

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

