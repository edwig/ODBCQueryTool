/*
Copyright (C) 2004-2023 Edwig Huisman

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#include "pch.h"
#include "QueryTool.h"
#include "ConnStringDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

ConnStringDlg::ConnStringDlg(CWnd* p_parent,CString p_connString)
              :StyleDialog(IDD_CON_STRING,p_parent)
              ,m_connString(p_connString)
{
}

ConnStringDlg::~ConnStringDlg()
{
}

void
ConnStringDlg::DoDataExchange(CDataExchange* pDX)
{
  StyleDialog::DoDataExchange(pDX);

  DDX_Control(pDX,IDC_CONN_STRING,m_editConnString,m_connString);
  DDX_Control(pDX,IDOK,           m_buttonOK);
  DDX_Control(pDX,IDCANCEL,       m_buttonCancel);
}

BEGIN_MESSAGE_MAP(ConnStringDlg,StyleDialog)
  ON_EN_KILLFOCUS(IDC_CON_STRING,OnKillfocusConnString)
  ON_BN_CLICKED  (IDOK,          OnOK)
  ON_BN_CLICKED  (IDCANCEL,      OnCancel)
END_MESSAGE_MAP()

BOOL
ConnStringDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();
  SetWindowText(_T("Connection string"));

  SetCanResize();
  m_editConnString.SetFocus();
  return FALSE;
}

void
ConnStringDlg::SetupDynamicLayout()
{
  StyleDialog::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif

  manager.AddItem(IDC_CONN_STRING,manager.MoveNone(),         manager.SizeHorizontal(100));
  manager.AddItem(IDOK,           manager.MoveHorizontal(100),manager.SizeNone());
  manager.AddItem(IDCANCEL,       manager.MoveHorizontal(100),manager.SizeNone());
  manager.Adjust();
}

void 
ConnStringDlg::OnKillfocusConnString()
{
  UpdateData();
}

void 
ConnStringDlg::OnOK()
{
  StyleDialog::OnOK();
}

void 
ConnStringDlg::OnCancel()
{
  StyleDialog::OnCancel();
}
