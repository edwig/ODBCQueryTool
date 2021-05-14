// GridViewDemoView.h : interface of the CGridViewDemoView class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "GridCtrl.h"

#define ID_TOGGLE_READONLY 20000

class CGridViewDemoDoc : public CDocument
{
  DECLARE_DYNAMIC(CGridViewDemoDoc);

public:
  CGridViewDemoDoc() { };
};

class CGridViewDemoView : public CView
{
protected: // create from serialization only
	CGridViewDemoView();
	DECLARE_DYNCREATE(CGridViewDemoView)

// Attributes
public:
	CGridViewDemoDoc* GetDocument();
	CGridCtrl* m_pGridCtrl;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridViewDemoView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual void OnInitialUpdate();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGridViewDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGridViewDemoView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnToggleReadonly();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in GridViewDemoView.cpp
inline CGridViewDemoDoc* CGridViewDemoView::GetDocument()
   { return (CGridViewDemoDoc*)m_pDocument; }
#endif
