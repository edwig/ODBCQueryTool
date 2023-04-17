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
#include "VariablesDlg.h"
#include "RebindDlg.h"

#pragma warning (disable: 4800)
// VariablesDlg dialog

IMPLEMENT_DYNAMIC(VariablesDlg, StyleDialog)

VariablesDlg::VariablesDlg(CWnd*    p_parent
                          ,SQLVariant* p_variable1
                          ,SQLVariant* p_variable2
                          ,SQLVariant* p_variable3
                          ,SQLVariant* p_variable4)
	           :StyleDialog(VariablesDlg::IDD, p_parent)
             ,m_variable1(p_variable1)
             ,m_variable2(p_variable2)
             ,m_variable3(p_variable3)
             ,m_variable4(p_variable4)
{
}

VariablesDlg::~VariablesDlg()
{
}

void VariablesDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_VAR1,  m_editVariable1,m_variableString1);
  DDX_Control(pDX,IDC_TYPE1, m_typeCombo1);
  DDX_Control(pDX,IDC_SIZE1, m_editLength1,m_length1);
  DDX_Control(pDX,IDC_EXEC1, m_AtExec1);
  DDX_Control(pDX,IDC_INOUT1,m_inoutCombo1);

  DDX_Control(pDX,IDC_VAR2,  m_editVariable2,m_variableString2);
  DDX_Control(pDX,IDC_TYPE2, m_typeCombo2);
  DDX_Control(pDX,IDC_SIZE2, m_editLength2,m_length2);
  DDX_Control(pDX,IDC_EXEC2, m_AtExec2);
  DDX_Control(pDX,IDC_INOUT2,m_inoutCombo2);

  DDX_Control(pDX,IDC_VAR3,  m_editVariable3,m_variableString3);
  DDX_Control(pDX,IDC_TYPE3, m_typeCombo3);
  DDX_Control(pDX,IDC_SIZE3, m_editLength3,m_length3);
  DDX_Control(pDX,IDC_EXEC3, m_AtExec3);
  DDX_Control(pDX,IDC_INOUT3,m_inoutCombo3);

  DDX_Control(pDX,IDC_VAR4,  m_editVariable4,m_variableString4);
  DDX_Control(pDX,IDC_TYPE4, m_typeCombo4);
  DDX_Control(pDX,IDC_SIZE4, m_editLength4,m_length4);
  DDX_Control(pDX,IDC_EXEC4, m_AtExec4);
  DDX_Control(pDX,IDC_INOUT4,m_inoutCombo4);

  DDX_Control(pDX,IDC_BIND,  m_buttonRebind);
  DDX_Control(pDX,IDOK,      m_buttonOK);
  DDX_Control(pDX,IDCANCEL,  m_buttonCancel);
}

BEGIN_MESSAGE_MAP(VariablesDlg, StyleDialog)
  ON_EN_KILLFOCUS (IDC_VAR1,  &VariablesDlg::OnEnChangeVar1)
  ON_CBN_SELCHANGE(IDC_TYPE1, &VariablesDlg::OnCbnSelchangeType1)
  ON_EN_KILLFOCUS (IDC_SIZE1, &VariablesDlg::OnEnChangeSize1)
  ON_BN_CLICKED   (IDC_EXEC1, &VariablesDlg::OnBnClickedExec1)

  ON_EN_KILLFOCUS (IDC_VAR2,  &VariablesDlg::OnEnChangeVar2)
  ON_CBN_SELCHANGE(IDC_TYPE2, &VariablesDlg::OnCbnSelchangeType2)
  ON_BN_CLICKED   (IDC_EXEC2, &VariablesDlg::OnBnClickedExec2)
  ON_EN_KILLFOCUS (IDC_SIZE2, &VariablesDlg::OnEnChangeSize2)
  
  ON_EN_KILLFOCUS (IDC_VAR3,  &VariablesDlg::OnEnChangeVar3)
  ON_CBN_SELCHANGE(IDC_TYPE3, &VariablesDlg::OnCbnSelchangeType3)
  ON_BN_CLICKED   (IDC_EXEC3, &VariablesDlg::OnBnClickedExec3)
  ON_EN_KILLFOCUS (IDC_SIZE3, &VariablesDlg::OnEnChangeSize3)

  ON_EN_KILLFOCUS (IDC_VAR4,  &VariablesDlg::OnEnChangeVar4)
  ON_CBN_SELCHANGE(IDC_TYPE4, &VariablesDlg::OnCbnSelchangeType4)
  ON_BN_CLICKED   (IDC_EXEC4, &VariablesDlg::OnBnClickedExec4)
  ON_EN_KILLFOCUS (IDC_SIZE4, &VariablesDlg::OnEnChangeSize4)

  ON_CBN_SELCHANGE(IDC_INOUT1, &VariablesDlg::OnCbnSelchangeInout1)
  ON_CBN_SELCHANGE(IDC_INOUT2, &VariablesDlg::OnCbnSelchangeInout2)
  ON_CBN_SELCHANGE(IDC_INOUT3, &VariablesDlg::OnCbnSelchangeInout3)
  ON_CBN_SELCHANGE(IDC_INOUT4, &VariablesDlg::OnCbnSelchangeInout4)

  ON_BN_CLICKED   (IDOK,       &VariablesDlg::OnBnClickedOk)
  ON_BN_CLICKED   (IDC_BIND,   &VariablesDlg::OnBnClickedRebind)
END_MESSAGE_MAP()

BOOL
VariablesDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();
  SetWindowText("Variables (binding)");
  
  DataTypes* types = allTypes;
  while(types->name)
  {
    m_typeCombo1.AddString(types->name);
    m_typeCombo2.AddString(types->name);
    m_typeCombo3.AddString(types->name);
    m_typeCombo4.AddString(types->name);
    ++types;
  }
  m_variableString1 = m_variable1->GetAsChar();
  m_type1           = m_variable1->GetDataType();
  m_length1         = m_variable1->GetBinaryPieceSize();
  CString type      = m_variable1->FindDatatype(m_type1);
  int ind = m_typeCombo1.FindStringExact(0,type.GetString());
  m_typeCombo1.SetCurSel(ind);
  m_AtExec1.SetCheck(m_variable1->GetAtExec());
  SetParam(m_inoutCombo1,m_variable1->GetParameterType());

  m_variableString2 = m_variable2->GetAsChar();
  m_type2           = m_variable2->GetDataType();
  m_length2         = m_variable2->GetBinaryPieceSize();
  type              = m_variable2->FindDatatype(m_type2);
  ind = m_typeCombo2.FindStringExact(0,type.GetString());
  m_typeCombo2.SetCurSel(ind);
  m_AtExec2.SetCheck(m_variable2->GetAtExec());
  SetParam(m_inoutCombo2,m_variable2->GetParameterType());

  m_variableString3 = m_variable3->GetAsChar();
  m_type3           = m_variable3->GetDataType();
  m_length3         = m_variable3->GetBinaryPieceSize();
  type              = m_variable3->FindDatatype(m_type3);
  ind = m_typeCombo3.FindStringExact(0,type.GetString());
  m_typeCombo3.SetCurSel(ind);
  m_AtExec3.SetCheck(m_variable3->GetAtExec());
  SetParam(m_inoutCombo3,m_variable3->GetParameterType());

  m_variableString4 = m_variable4->GetAsChar();
  m_type4           = m_variable4->GetDataType();
  m_length4         = m_variable4->GetBinaryPieceSize();
  type              = m_variable4->FindDatatype(m_type4);
  ind = m_typeCombo4.FindStringExact(0,type.GetString());
  m_typeCombo4.SetCurSel(ind);
  m_AtExec4.SetCheck(m_variable4->GetAtExec());
  SetParam(m_inoutCombo4,m_variable4->GetParameterType());

  UpdateData(FALSE);
  OnBnClickedExec1();
  OnBnClickedExec2();
  OnBnClickedExec3();
  OnBnClickedExec4();
  return TRUE;
}

void
VariablesDlg::SetParam(CComboBox& box,int curval)
{
  DataTypes* param = allParams;
  while(param->name)
  {
    box.AddString(param->name);
    ++param;
  }
  box.SetCurSel(curval);
}

// VariablesDlg message handlers

void VariablesDlg::OnEnChangeVar1()
{
  UpdateData();
  m_variable1->SetData(m_type1,(char*)m_variableString1.GetString());
}

void 
VariablesDlg::OnCbnSelchangeType1()
{
  UpdateData();
  int ind = m_typeCombo1.GetCurSel();
  if(ind >= 0)
  {
    CString type;
    m_typeCombo1.GetLBText(ind,type);
    m_type1 = m_variable1->FindDatatype((char*)type.GetString());

    // Do the conversion
    m_variableString1 = m_variable1->GetAsChar();
    m_variable1->SetData(m_type1,(char*)m_variableString1.GetString());
    // Could be reformatted
    m_variableString1 = m_variable1->GetAsChar();

    UpdateData(FALSE);
  }
}

void 
VariablesDlg::OnEnChangeSize1()
{
  UpdateData();
  m_variable1->SetBinaryPieceSize(m_length1);
}

void 
VariablesDlg::OnBnClickedExec1()
{
  m_variable1->SetAtExec(m_AtExec1.GetCheck());
  CWnd* w = GetDlgItem(IDC_SIZE1);
  w->EnableWindow(m_AtExec1.GetCheck());
}

// VARIABLE 2

void 
VariablesDlg::OnEnChangeVar2()
{
  UpdateData();
  m_variable2->SetData(m_type2,(char*)m_variableString2.GetString());
}

void 
VariablesDlg::OnCbnSelchangeType2()
{
  UpdateData();
  int ind = m_typeCombo2.GetCurSel();
  if(ind >= 0)
  {
    CString type;
    m_typeCombo2.GetLBText(ind,type);
    m_type2 = m_variable2->FindDatatype((char*)type.GetString());

    // Do the conversion
    m_variableString2 = m_variable2->GetAsChar();
    m_variable2->SetData(m_type2,(char*)m_variableString2.GetString());
    // Could be reformatted
    m_variableString2 = m_variable2->GetAsChar();

    UpdateData(FALSE);
  }
}

void 
VariablesDlg::OnBnClickedExec2()
{
  m_variable2->SetAtExec(m_AtExec2.GetCheck());
  CWnd* w = GetDlgItem(IDC_SIZE2);
  w->EnableWindow(m_AtExec2.GetCheck());
}

void 
VariablesDlg::OnEnChangeSize2()
{
  UpdateData();
  m_variable2->SetBinaryPieceSize(m_length2);
}

void 
VariablesDlg::OnEnChangeVar3()
{
  UpdateData();
  m_variable3->SetData(m_type3,(char*)m_variableString3.GetString());
}

void 
VariablesDlg::OnCbnSelchangeType3()
{
  UpdateData();
  int ind = m_typeCombo3.GetCurSel();
  if(ind >= 0)
  {
    CString type;
    m_typeCombo3.GetLBText(ind,type);
    m_type3 = m_variable3->FindDatatype((char*)type.GetString());

    // Do the conversion
    m_variableString3 = m_variable3->GetAsChar();
    m_variable3->SetData(m_type3,(char*)m_variableString3.GetString());
    // Could be reformatted
    m_variableString3 = m_variable3->GetAsChar();

    UpdateData(FALSE);
  }
}

void 
VariablesDlg::OnBnClickedExec3()
{
  m_variable3->SetAtExec(m_AtExec3.GetCheck());
  CWnd* w = GetDlgItem(IDC_SIZE3);
  w->EnableWindow(m_AtExec3.GetCheck());
}

void 
VariablesDlg::OnEnChangeSize3()
{
  UpdateData();
  m_variable3->SetBinaryPieceSize(m_length3);
}

// VAR 4

void 
VariablesDlg::OnEnChangeVar4()
{
  UpdateData();
  m_variable4->SetData(m_type4,(char*)m_variableString4.GetString());
}

void 
VariablesDlg::OnCbnSelchangeType4()
{
  UpdateData();
  int ind = m_typeCombo4.GetCurSel();
  if(ind >= 0)
  {
    CString type;
    m_typeCombo4.GetLBText(ind,type);
    m_type4 = m_variable4->FindDatatype((char*)type.GetString());

    // Do the conversion
    m_variableString4 = m_variable4->GetAsChar();
    m_variable4->SetData(m_type4,(char*)m_variableString4.GetString());
    // Could be reformatted
    m_variableString4 = m_variable4->GetAsChar();

    UpdateData(FALSE);
  }
}

void 
VariablesDlg::OnBnClickedExec4()
{
  m_variable4->SetAtExec(m_AtExec4.GetCheck());
  CWnd* w = GetDlgItem(IDC_SIZE4);
  w->EnableWindow(m_AtExec4.GetCheck());
}

void 
VariablesDlg::OnEnChangeSize4()
{
  UpdateData();
  m_variable4->SetBinaryPieceSize(m_length4);
}

void 
VariablesDlg::OnCbnSelchangeInout1()
{
  UpdateData();
  int num = m_inoutCombo1.GetCurSel();
  if(num >= 0)
  {
    CString typeName;
    m_inoutCombo1.GetLBText(num,typeName);
    int type = m_variable1->FindParamtype((char*)typeName.GetString());
    m_variable1->SetParameterType((SQLParamType)type);
  }
}

void 
VariablesDlg::OnCbnSelchangeInout2()
{
  UpdateData();
  int num = m_inoutCombo2.GetCurSel();
  if(num >= 0)
  {
    CString typeName;
    m_inoutCombo2.GetLBText(num,typeName);
    int type = m_variable2->FindParamtype((char*)typeName.GetString());
    m_variable2->SetParameterType((SQLParamType)type);
  }
}

void 
VariablesDlg::OnCbnSelchangeInout3()
{
  UpdateData();
  int num = m_inoutCombo3.GetCurSel();
  if(num >= 0)
  {
    CString typeName;
    m_inoutCombo3.GetLBText(num,typeName);
    int type = m_variable3->FindParamtype((char*)typeName.GetString());
    m_variable3->SetParameterType((SQLParamType)type);
  }
}

void 
VariablesDlg::OnCbnSelchangeInout4()
{
  UpdateData();
  int num = m_inoutCombo4.GetCurSel();
  if(num >= 0)
  {
    CString typeName;
    m_inoutCombo4.GetLBText(num,typeName);
    int type = m_variable4->FindParamtype((char*)typeName.GetString());
    m_variable4->SetParameterType((SQLParamType)type);
  }
}

void 
VariablesDlg::OnBnClickedOk()
{
  UpdateData();
  OnOK();
}

void 
VariablesDlg::OnBnClickedRebind()
{
  RebindDlg dlg(this);
  dlg.DoModal();
}
