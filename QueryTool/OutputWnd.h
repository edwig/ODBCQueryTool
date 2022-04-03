////////////////////////////////////////////////////////////////////////
//
// File: OutputWnd.h
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

