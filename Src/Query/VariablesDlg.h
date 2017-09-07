/*
Copyright (C) 2004 - 2017 Edwig Huisman

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
#pragma once
#include "SQLComponents.h"
#include "SQLVariant.h"
#include "resource.h"

using namespace SQLComponents;

// VariablesDlg dialog

class VariablesDlg : public CDialog
{
	DECLARE_DYNAMIC(VariablesDlg)

public:
	VariablesDlg(CWnd*     p_parent
              ,SQLVariant* p_variable1
              ,SQLVariant* p_variable2
              ,SQLVariant* p_variable3
              ,SQLVariant* p_variable4);
	~VariablesDlg();
  BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_VARIABLES };

protected:
	void    DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  void    SetParam(CComboBox& box,int curval);


	DECLARE_MESSAGE_MAP()

  SQLVariant*  m_variable1;
  CString      m_variableString1;
  CComboBox    m_typeCombo1;
  int          m_type1;
  int          m_length1;
  CButton      m_AtExec1;
  CComboBox    m_inoutCombo1;

  SQLVariant*  m_variable2;
  CString      m_variableString2;
  CComboBox    m_typeCombo2;
  int          m_type2;
  int          m_length2;
  CButton      m_AtExec2;
  CComboBox    m_inoutCombo2;

  SQLVariant*  m_variable3;
  CString      m_variableString3;
  CComboBox    m_typeCombo3;
  int          m_type3;
  int          m_length3;
  CButton      m_AtExec3;
  CComboBox    m_inoutCombo3;

  SQLVariant*  m_variable4;
  CString      m_variableString4;
  CComboBox    m_typeCombo4;
  int          m_type4;
  int          m_length4;
  CButton      m_AtExec4;
  CComboBox    m_inoutCombo4;

public:
  afx_msg void OnEnChangeVar1();
  afx_msg void OnCbnSelchangeType1();
  afx_msg void OnEnChangeSize1();
  afx_msg void OnBnClickedExec1();
  afx_msg void OnEnChangeVar2();
  afx_msg void OnCbnSelchangeType2();
  afx_msg void OnBnClickedExec2();
  afx_msg void OnEnChangeSize2();
  afx_msg void OnEnChangeVar3();
  afx_msg void OnCbnSelchangeType3();
  afx_msg void OnBnClickedExec3();
  afx_msg void OnEnChangeSize3();
  afx_msg void OnEnChangeVar4();
  afx_msg void OnCbnSelchangeType4();
  afx_msg void OnBnClickedExec4();
  afx_msg void OnEnChangeSize4();
  afx_msg void OnCbnSelchangeInout1();
  afx_msg void OnCbnSelchangeInout2();
  afx_msg void OnCbnSelchangeInout3();
  afx_msg void OnCbnSelchangeInout4();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedRebind();
};
