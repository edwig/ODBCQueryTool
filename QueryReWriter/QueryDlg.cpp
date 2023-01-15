////////////////////////////////////////////////////////////////////////
//
// File: QueryDlg.cpp
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
#include "pch.h"
#include "QueryReWriter.h"
#include "QueryReWriterApp.h"
#include "afxdialogex.h"
#include "QueryDlg.h"

/* TESTQUERY

SELECT length(name) as name_length
     , sysdate      as date_of_today
     , ucase(name)  as name_case_insens
     , lower(name)  as name_lowercase
     , name || ' ' || nvl(subname,'') as total_name
  FROM name_table

*/


// QueryDlg dialog

IMPLEMENT_DYNAMIC(QueryDlg, StyleDialog)

QueryDlg::QueryDlg(CWnd* pParent /*=nullptr*/)
	       :StyleDialog(IDD_QUERIES, pParent)
{
}

QueryDlg::~QueryDlg()
{
}

void 
QueryDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);

  DDX_Control(pDX,IDC_QUERY,    m_editQuery,m_query);
  DDX_Control(pDX,IDC_GO,       m_buttonGO);
  DDX_Control(pDX,IDC_REWRITTEN,m_editResult,m_result);
}

BEGIN_MESSAGE_MAP(QueryDlg, StyleDialog)
  ON_EN_CHANGE   (IDC_QUERY,     &QueryDlg::OnEnChangeQuery)
  ON_EN_KILLFOCUS(IDC_QUERY,     &QueryDlg::OnEnChangeQuery)
  ON_BN_CLICKED  (IDC_GO,        &QueryDlg::OnBnClickedGo)
  ON_EN_CHANGE   (IDC_REWRITTEN, &QueryDlg::OnEnChangeRewritten)
END_MESSAGE_MAP()

BOOL
QueryDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();

  m_buttonGO.SetIconImage(IDI_GO);
  SetCanResize();
  InitFields();

  m_editQuery.SetFocus();
  return TRUE;
}

void
QueryDlg::SetupDynamicLayout()
{
  StyleDialog::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif
  manager.AddItem(IDC_QUERY,    CMFCDynamicLayout::MoveNone(),        CMFCDynamicLayout::SizeHorizontalAndVertical(50,100));
  manager.AddItem(IDC_REWRITTEN,CMFCDynamicLayout::MoveHorizontal(50),CMFCDynamicLayout::SizeHorizontalAndVertical(50,100));
  manager.AddItem(IDC_GO,       CMFCDynamicLayout::MoveHorizontal(50),CMFCDynamicLayout::SizeNone());
}

void
QueryDlg::InitFields()
{
  m_editQuery .SetFontName("Courier New",120);
  m_editResult.SetFontName("Courier New",120);
}

// QueryDlg message handlers

void 
QueryDlg::OnEnChangeQuery()
{
  UpdateData();
}

void 
QueryDlg::OnBnClickedGo()
{
  QueryReWriter* rewriter = theApp.GetQueryReWriter();
  if(rewriter)
  {
    m_result = rewriter->Parse(m_query);
    UpdateData(FALSE);
  }
  else
  {
    StyleMessageBox(this,"Configure a rewriting schema and actions first!"
                         "Use the tab 'Configuration' to do so..."
                        ,"QueryRewriter",MB_OK|MB_ICONERROR);
  }
}

void 
QueryDlg::OnEnChangeRewritten()
{
  UpdateData();
}
