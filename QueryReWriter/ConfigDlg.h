////////////////////////////////////////////////////////////////////////
//
// File: ConfigDlg.h
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
#include "resource.h"
#include <QueryReWriter.h>

// ConfigDlg dialog

class ConfigDlg : public StyleTab
{
	DECLARE_DYNAMIC(ConfigDlg)

public:
	ConfigDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ConfigDlg();

  void            SetConfigFile(CString p_file);
  bool            ReadConfig();
  bool            WriteConfig();
  QueryReWriter*  CreateReWriter();
  CString         GetSchema() { return m_schema; }
  CString         ODBCEscapeToString(OdbcEsc p_escape);
  OdbcEsc         StringToODBCEscape(CString p_escape);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONFIG };
#endif

protected:
  virtual void  DoDataExchange(CDataExchange* pDX) override;
  virtual void  SetupDynamicLayout() override;
  virtual BOOL  OnInitDialog() override;
          void  InitCombos();
          void  InitGrid();
          int   InsertRow();
          int   InsertRow(CString p_string);


  CString       m_config;
  CString       m_purpose;
  CString       m_schema;
  int           m_strings;

  StyleEdit     m_editConfig;
  StyleButton   m_buttonConfig;
  StyleEdit     m_editPurpose;
  StyleEdit     m_editSchema;
  StyleComboBox m_comboStrings;
  StyleGridCtrl m_grid;
  StyleButton   m_buttonAdd;
  StyleButton   m_buttonDelete;


	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnEnChangeConfig();
  afx_msg void OnBnClickedButtConfig();
  afx_msg void OnEnChangePurpose();
  afx_msg void OnEnChangeSchema();
  afx_msg void OnCbnSelchangeStrings();
  afx_msg void OnBnClickedAdd();
  afx_msg void OnBnClickedDelete();
};
