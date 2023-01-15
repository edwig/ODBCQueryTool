////////////////////////////////////////////////////////////////////////
//
// File: QueryReWriterDlg.h
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

class QueryDlg;
class ConfigDlg;

// QueryReWriterDlg dialog
class QueryReWriterDlg : public StyleDialog
{
// Construction
public:
	QueryReWriterDlg(CWnd* pParent = nullptr);	// standard constructor
 ~QueryReWriterDlg();

  void SetConfig(CString p_config);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_QUERYREWRITER_DIALOG };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual void SetupDynamicLayout() override;
  virtual BOOL OnInitDialog() override;
          void InitTabs();
          void InitConfig();

// Implementation
protected:
	HICON m_hIcon;
  CString       m_config;
  StyleEdit     m_editConfig;
  StyleTabCtrl  m_tabs;

  QueryDlg*     m_page1;
  ConfigDlg*    m_page2;

  DECLARE_MESSAGE_MAP()

  // Generated message map functions
  afx_msg HCURSOR OnQueryDragIcon();
  afx_msg void OnSysCommand(UINT nID,LPARAM lParam);
	afx_msg void OnPaint();
  afx_msg void OnEnChangeConfig();
  afx_msg void OnAbout();
  afx_msg void OnExit();
};
