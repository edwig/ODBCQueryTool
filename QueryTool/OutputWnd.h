
#pragma once
#include "MGridCtrl.h"

#define OUTPUT_PANES  3


/////////////////////////////////////////////////////////////////////////////
// COutputList window

class COutputList : public StyleGridCtrl
{
// Construction
public:
	COutputList() noexcept;

// Implementation
public:
	virtual ~COutputList();

protected:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint point);
  afx_msg void COutputList::OnViewOutput();
  afx_msg void OnFindFilesClicked(NMHDR* pNotifyStruct, LRESULT* result);

	DECLARE_MESSAGE_MAP()
};

class COutputWnd : public CDockablePane
{
// Construction
public:
	COutputWnd() noexcept;

	StyleGridCtrl* GetEmptyFindGrid();
	// Attributes
protected:
	CMFCTabCtrl	m_wndTabs;
	int m_current { 0 };

	COutputList m_wndFind1;
	COutputList m_wndFind2;
	COutputList m_wndFind3;

protected:
	void AdjustHorzScroll(CListBox& wndListBox);
	void InitGridEmpty(COutputList& p_list);

// Implementation
public:
	virtual ~COutputWnd();

protected:
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};

