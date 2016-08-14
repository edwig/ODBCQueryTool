/* 
Copyright (C) 2004 Edwig Huisman

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

#include "stdafx.h"
#include "RebindDlg.h"
#include "OpenEditorApp.h"
#include "Database\SQLVariant.h"

// RebindDlg dialog

IMPLEMENT_DYNAMIC(RebindDlg, CDialog)

RebindDlg::RebindDlg(CWnd* pParent /*=NULL*/)
          :CDialog(RebindDlg::IDD, pParent)
{
}

RebindDlg::~RebindDlg()
{
}

void RebindDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_DATATYPE, m_typeCombo);
  DDX_Control(pDX,IDC_REBIND,   m_rebindCombo);
}


BEGIN_MESSAGE_MAP(RebindDlg, CDialog)
  ON_CBN_SELCHANGE(IDC_DATATYPE,&RebindDlg::OnCbnSelchangeDatatype)
  ON_CBN_SELCHANGE(IDC_REBIND,  &RebindDlg::OnCbnSelchangeRebind)
  ON_BN_CLICKED   (IDC_RESET,   &RebindDlg::OnBnClickedReset)
  ON_BN_CLICKED   (IDC_ADD,     &RebindDlg::OnBnClickedAdd)
  ON_BN_CLICKED   (IDOK,        &RebindDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL
RebindDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  DataTypes* types = allTypes;
  while(types->name)
  {
    m_typeCombo.AddString(types->name);
    m_rebindCombo.AddString(types->name);
    ++types;
  }
  return TRUE;
}

// RebindDlg message handlers

void 
RebindDlg::OnCbnSelchangeDatatype()
{
  int num = m_typeCombo.GetCurSel();
  if(num >= 0)
  {
    CString type;
    m_typeCombo.GetLBText(num,type);
    SQLVariant var;
    m_type = var.FindDatatype((char*)type.GetString());
  }
}

void 
RebindDlg::OnCbnSelchangeRebind()
{
  int num = m_rebindCombo.GetCurSel();
  if(num >= 0)
  {
    CString type;
    m_rebindCombo.GetLBText(num,type);
    SQLVariant var;
    m_rebind = var.FindDatatype((char*)type.GetString());
  }
}

void 
RebindDlg::OnBnClickedReset()
{
  if(AfxMessageBox("Clear the rebind mappings?",MB_YESNO|MB_DEFBUTTON2) == IDYES)
  {
    RebindMap* rebinds = theApp.GetRebinds();
    rebinds->clear();
    AfxMessageBox("Cleared all rebindings of datatypes!",MB_OK|MB_ICONINFORMATION);
  }
}

void 
RebindDlg::OnBnClickedAdd()
{
  if(m_type > 0 && m_rebind > 0)
  {
    RebindMap* rebinds = theApp.GetRebinds();
    rebinds->insert(std::make_pair(m_type,m_rebind));

    SQLVariant var;
    CString typeName   = var.FindDatatype(m_type);
    CString rebindName = var.FindDatatype(m_rebind);
    CString msg;
    msg.Format("Rebind datatype [%s] to [%s]",typeName,rebindName);
    AfxMessageBox(msg,MB_OK);
  }
}

void 
RebindDlg::OnBnClickedOk()
{
  OnOK();
}
