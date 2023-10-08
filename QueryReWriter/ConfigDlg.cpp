////////////////////////////////////////////////////////////////////////
//
// File: ConfigDlg.cpp
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
#include "QueryReWriterApp.h"
#include "QueryReWriterDlg.h"
#include "afxdialogex.h"
#include "ConfigDlg.h"
#include <GridCellCombo.h>
#include <FileDialog.h>
#include <StringUtilities.h>
#include <RegistryManager.h>
#include <WinFile.h>
#include <stdlib.h>
#include <vector>

// ConfigDlg dialog

IMPLEMENT_DYNAMIC(ConfigDlg, StyleDialog)

ConfigDlg::ConfigDlg(CWnd* pParent /*=nullptr*/)
	        :StyleDialog(IDD_CONFIG, pParent)
          ,m_strings(0)
{
}

ConfigDlg::~ConfigDlg()
{
}

void 
ConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);

  DDX_Control(pDX,IDC_CONFIG,     m_editConfig,   m_config);
  DDX_Control(pDX,IDC_BUTT_CONFIG,m_buttonConfig);
  DDX_Control(pDX,IDC_PURPOSE,    m_editPurpose,  m_purpose);
  DDX_Control(pDX,IDC_SCHEMA,     m_editSchema,   m_schema);
  DDX_CBIndex(pDX,IDC_STRINGS,    m_comboStrings, m_strings);
  DDX_Control(pDX,IDC_WORDS,      m_grid);
  DDX_Control(pDX,IDC_BUTT_ADD,   m_buttonAdd);
  DDX_Control(pDX,IDC_BUTT_DEL,   m_buttonDelete);

  if(pDX->m_bSaveAndValidate == FALSE)
  {
    m_buttonDelete.EnableWindow(m_grid.GetRowCount() > 1);
  }
}


BEGIN_MESSAGE_MAP(ConfigDlg, StyleDialog)
  ON_EN_KILLFOCUS (IDC_CONFIG,      &ConfigDlg::OnEnChangeConfig)
  ON_BN_CLICKED   (IDC_BUTT_CONFIG, &ConfigDlg::OnBnClickedButtConfig)
  ON_EN_KILLFOCUS (IDC_PURPOSE,     &ConfigDlg::OnEnChangePurpose)
  ON_EN_KILLFOCUS (IDC_SCHEMA,      &ConfigDlg::OnEnChangeSchema)
  ON_CBN_SELCHANGE(IDC_STRINGS,     &ConfigDlg::OnCbnSelchangeStrings)
  ON_BN_CLICKED   (IDC_BUTT_ADD,    &ConfigDlg::OnBnClickedAdd)
  ON_BN_CLICKED   (IDC_BUTT_DEL,    &ConfigDlg::OnBnClickedDelete)
END_MESSAGE_MAP()

BOOL
ConfigDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();

  InitCombos();
  InitGrid();

  m_buttonConfig.SetStyle(_T("dir"));
  m_buttonAdd.SetIconImage(IDI_ADD);
  m_buttonDelete.SetStyle(_T("rem"));

  // Connect to this dialog
  theApp.SetConfig(this);

  SetCanResize();
  return FALSE;
}

void
ConfigDlg::SetupDynamicLayout()
{
  StyleDialog::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif
  manager.AddItem(IDC_CONFIG,     CMFCDynamicLayout::MoveNone(),         CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_BUTT_CONFIG,CMFCDynamicLayout::MoveHorizontal(100),CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_PURPOSE,    CMFCDynamicLayout::MoveNone(),         CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_SCHEMA,     CMFCDynamicLayout::MoveNone(),         CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_STRINGS,    CMFCDynamicLayout::MoveNone(),         CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_WORDS,      CMFCDynamicLayout::MoveNone(),         CMFCDynamicLayout::SizeHorizontalAndVertical(100,100));
  manager.AddItem(IDC_BUTT_ADD,   CMFCDynamicLayout::MoveVertical(100),  CMFCDynamicLayout::SizeNone());
  manager.AddItem(IDC_BUTT_DEL,   CMFCDynamicLayout::MoveVertical(100),  CMFCDynamicLayout::SizeNone());
}

void
ConfigDlg::InitCombos()
{
  m_comboStrings.AddString(_T("No action"));
  m_comboStrings.AddString(_T("Transform standard || to SQL-Server '+'"));
  m_comboStrings.AddString(_T("Transform SQL-Server '+' to standard ||"));
}

void
ConfigDlg::InitGrid()
{
  m_grid.SetColumnCount(5);
  m_grid.SetRowCount(1);
  m_grid.SetFixedRowCount(1);
  m_grid.SetFixedColumnCount(1);
  m_grid.GetCell(0,0)->SetText(_T("Number"));
  m_grid.GetCell(0,1)->SetText(_T("Keyword"));
  m_grid.GetCell(0,2)->SetText(_T("Schema"));
  m_grid.GetCell(0,3)->SetText(_T("Replace by"));
  m_grid.GetCell(0,4)->SetText(_T("Function"));
  m_grid.SetColumnWidth(0,60);
  m_grid.SetColumnWidth(1,200);
  m_grid.SetColumnWidth(2,200);
  m_grid.SetColumnWidth(3,200);
  m_grid.SetColumnWidth(4,500);
  m_grid.SetEditable();
  m_grid.SetSingleRowSelection();
  m_grid.Refresh();
}

void
ConfigDlg::SetConfigFile(CString p_file)
{
  m_config = p_file;
}

bool
ConfigDlg::ReadConfig()
{
  WinFile file(m_config);
  if(file.Open(winfile_read))
  {
    CString string;
    while(file.Read(string))
    {
      // Empty line
      if(string.GetLength() == 0)
      {
        continue;
      }
      // Comment
      if(string.GetAt(0) == '#')
      {
        continue;
      }
      string.Replace(_T("\n"),_T(""));
      string.Replace(_T("\r"),_T(""));

      if(string.Left(3) == _T(":P:"))
      {
        m_purpose = string.Mid(3);
        continue;
      }
      if(string.Left(3) == _T(":S:"))
      {
        m_schema = string.Mid(3);
        continue;
      }
      if(string.Left(3) == _T(":O:"))
      {
        m_strings = _ttoi(string.Mid(3));
        continue;
      }
      if(string.GetAt(0) == '-')
      {
        InsertRow(string);
      }
    }
    file.Close();
    UpdateData(FALSE);
    OnEnChangePurpose();
  }
  else
  {
    StyleMessageBox(this,_T("Cannot open configuration file: ") + m_config,_T("ERROR"),MB_OK|MB_ICONERROR);
    return false;
  }
  return true;
}

bool
ConfigDlg::WriteConfig()
{
  // Nothing to write?
  if(m_config.IsEmpty())
  {
    return true;
  }

  WinFile file(m_config);
  if(file.Open(winfile_write))
  {
    CString string;
    string = _T("# Configuration file for Query ReWriter application\n");
    file.Write(string);
    string = _T(":P:") + m_purpose + _T("\n");
    file.Write(string);
    string = _T(":S:") + m_schema + _T("\n");
    file.Write(string);
    string.Format(_T(":O:%d\n"),m_strings);
    file.Write(string);

    for(int row = 1;row < m_grid.GetRowCount();++row)
    {
      CString keyword  = m_grid.GetCell(row,1)->GetText();
      CString schema   = m_grid.GetCell(row,2)->GetText();
      CString replace  = m_grid.GetCell(row,3)->GetText();
      CString function = m_grid.GetCell(row,4)->GetText();

      string.Format(_T("-%s-%s-%s-%s\n")
                    ,keyword.GetString()
                    ,schema.GetString()
                    ,replace.GetString()
                    ,function.GetString());
      file.Write(string);
    }
    file.Close();

    RegistryManager manager;
    manager.SetRegistryString(_T("HKCU\\Software\\EDO\\QueryRewriter"),_T("configfile"),m_config);
  }
  else
  {
    StyleMessageBox(this,_T("Cannot open configuration file for writing : ") + m_config,_T("ERROR"),MB_OK | MB_ICONERROR);
    return false;
  }
  return true;
}

int
ConfigDlg::InsertRow()
{
  int rows = m_grid.GetRowCount();
  CString num;
  num.Format(_T("%d"),rows);
  int newrow = m_grid.InsertRow(num);

  // Add function list
  m_grid.SetCellType(newrow,4,RUNTIME_CLASS(CGridCellCombo));
  CGridCellCombo* odbc = (CGridCellCombo*) m_grid.GetCell(newrow,4);

  CStringArray list;
  for(int index = 0; index < 10;++index)
  {
    list.Add(ODBCEscapeToString((OdbcEsc) index));
  }
  odbc->SetOptions(list);
  odbc->SetStyle(CBS_DROPDOWNLIST);

  m_grid.Refresh();
  UpdateData(FALSE);

  return newrow;
}

// Insert a string from the config file
// Format("-%s-%s-%s-%s\n",keyword,schema,replace,function);

int
ConfigDlg::InsertRow(CString p_string)
{
  CString keyword,schema,replace,function;

  std::vector<XString> values;
  SplitString(p_string,values,'-');
  
  if(values.size() > 1) keyword  = values[1];
  if(values.size() > 2) schema   = values[2];
  if(values.size() > 3) replace  = values[3];
  if(values.size() > 4) function = values[4];

  int row = InsertRow();
  m_grid.GetCell(row,1)->SetText(keyword);
  m_grid.GetCell(row,2)->SetText(schema);
  m_grid.GetCell(row,3)->SetText(replace);
  m_grid.GetCell(row,4)->SetText(function);

  return row;
}

QueryReWriter*
ConfigDlg::CreateReWriter()
{
  if(m_schema.IsEmpty())
  {
    return nullptr;
  }
  QueryReWriter* rewriter = new QueryReWriter(m_schema);
  rewriter->SetOption((SROption)m_strings);

  for(int row = 1;row < m_grid.GetRowCount();++row)
  {
    CString keyword  = m_grid.GetCell(row,1)->GetText();
    CString schema   = m_grid.GetCell(row,2)->GetText();
    CString replace  = m_grid.GetCell(row,3)->GetText();
    CString function = m_grid.GetCell(row,4)->GetText();

    rewriter->AddSQLWord(keyword,replace,schema,Token::TK_EOS,StringToODBCEscape(function));
  }
  return rewriter;
}

CString
ConfigDlg::ODBCEscapeToString(OdbcEsc p_escape)
{
  CString string;
  switch(p_escape)
  {
    case OdbcEsc::None:      break;
    case OdbcEsc::Function:  string = _T("ODBC Function { fn xxx }");         break;
    case OdbcEsc::Procedure: string = _T("ODBC Procedure { [?=]call xxxx }"); break;
    case OdbcEsc::Date:      string = _T("ODBC Date {d 'yyyy-mm-dd'}");       break;
    case OdbcEsc::Time:      string = _T("ODBC Time {t 'hh:mm:ss'}");         break;
    case OdbcEsc::Timestamp: string = _T("ODBC Timestamp {ts 'yyyy-mm-dd hh:mm:ss'}");               break;
    case OdbcEsc::Guid:      string = _T("ODBC GUID {guid 'nnnnnnnn-nnnn-nnnn-nnnn-nnnnnnnnnnnn'}"); break;
    case OdbcEsc::LikeEsc:   string = _T("ODBC LIKE Escape {'x'}");                                  break;
    case OdbcEsc::Interval:  string = _T("ODBC Interval {INTERVAL [+|-]'n YEAR TO m SECOND(s)'}");   break;
    case OdbcEsc::OuterJoin: string = _T("ODBC Outer-Join {oj table1 [as1] [LEFT|RIGHT|FULL] OUTER JOIN table2 [as2] ON as1.one = as2.two }"); break;
  }
  return string;
}

OdbcEsc
ConfigDlg::StringToODBCEscape(CString p_escape)
{
  if(p_escape.CompareNoCase(_T("ODBC Function { fn xxx }"))         == 0) return OdbcEsc::Function;
  if(p_escape.CompareNoCase(_T("ODBC Procedure { [?=]call xxxx }")) == 0) return OdbcEsc::Procedure;
  if(p_escape.CompareNoCase(_T("ODBC Date {d 'yyyy-mm-dd'}"))       == 0) return OdbcEsc::Date;
  if(p_escape.CompareNoCase(_T("ODBC Time {t 'hh:mm:ss'}"))         == 0) return OdbcEsc::Time;
  if(p_escape.CompareNoCase(_T("ODBC Timestamp {ts 'yyyy-mm-dd hh:mm:ss'}"))                == 0) return OdbcEsc::Timestamp;
  if(p_escape.CompareNoCase(_T("ODBC GUID {guid 'nnnnnnnn-nnnn-nnnn-nnnn-nnnnnnnnnnnn'}"))  == 0) return OdbcEsc::Guid;
  if(p_escape.CompareNoCase(_T("ODBC LIKE Escape {'x'}"))                                   == 0) return OdbcEsc::LikeEsc;
  if(p_escape.CompareNoCase(_T("ODBC Interval {INTERVAL [+|-]'n YEAR TO m SECOND(s)'}"))    == 0) return OdbcEsc::Interval;
  if(p_escape.CompareNoCase(_T("ODBC Outer-Join {oj table1 [as1] [LEFT|RIGHT|FULL] OUTER JOIN table2 [as2] ON as1.one = as2.two }")) == 0) return OdbcEsc::OuterJoin;

  return OdbcEsc::None;
}

// ConfigDlg message handlers

void 
ConfigDlg::OnEnChangeConfig()
{
  UpdateData();
}

void 
ConfigDlg::OnBnClickedButtConfig()
{
  UpdateData();

  DocFileDialog dlg(GetSafeHwnd(),true,_T("Select query-rewriter config file"),_T("qrc"),_T(""),0
                   ,_T("Query-Rewriter config *.qrc|*.qrc|All files *.*|*.*|"));
  if(dlg.DoModal() == IDOK)
  {
    m_config = dlg.GetChosenFile();
    ReadConfig();
    UpdateData(FALSE);
  }
}

void 
ConfigDlg::OnEnChangePurpose()
{
  UpdateData();

  QueryReWriterDlg* dlg = reinterpret_cast<QueryReWriterDlg*>(GetParent()->GetParent());
  dlg->SetConfig(m_purpose);
}

void 
ConfigDlg::OnEnChangeSchema()
{
  UpdateData();
  theApp.ResetReWriter();
}

void 
ConfigDlg::OnCbnSelchangeStrings()
{
  UpdateData();
  theApp.ResetReWriter();
}

void
ConfigDlg::OnBnClickedAdd()
{
  int newrow = InsertRow();

  m_grid.SetFocus();
  m_grid.SetFocusCell(newrow,1);

  theApp.ResetReWriter();
}

void
ConfigDlg::OnBnClickedDelete()
{
  CCellID cell = m_grid.GetFocusCell();
  if(cell.row < 1)
  {
    StyleMessageBox(this,_T("Select a row in the keyword grid first!"),_T("ERROR"),MB_OK|MB_ICONERROR);
    return;
  }
  CString keyword = m_grid.GetCell(cell.row,cell.col)->GetText();
  if(StyleMessageBox(this,_T("Do you want to remove the row for keyword: ") + keyword
                    ,_T("Query ReWrite"),MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION) == IDYES)
  {
    m_grid.DeleteRow(cell.row);
    m_grid.Refresh();
  }
  UpdateData(FALSE);
  theApp.ResetReWriter();
}
