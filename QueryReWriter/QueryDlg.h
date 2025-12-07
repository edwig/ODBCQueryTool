////////////////////////////////////////////////////////////////////////
//
// File: QueryDlg.h
//
// Copyright (c) 1998-2022 ir. W.E. Huisman
// All rights reserved
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
#include "afxdialogex.h"

// QueryDlg dialog

class QueryDlg : public StyleTab
{
	DECLARE_DYNAMIC(QueryDlg)

public:
	QueryDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~QueryDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_QUERIES };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual void SetupDynamicLayout() override;
  virtual BOOL OnInitDialog() override;
          void InitFields();

  CString     m_query;
  CString     m_result;

  StyleEdit   m_editQuery;
  StyleEdit   m_editResult;
  StyleButton m_buttonGO;

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnEnChangeQuery();
  afx_msg void OnBnClickedGo();
  afx_msg void OnEnChangeRewritten();
};
