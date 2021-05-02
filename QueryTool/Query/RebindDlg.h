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
#include "resource.h"

// Translation list of SQL datatype constants and names
typedef struct _types
{
  char*  name;
  int    type;
}
DataTypes;

extern DataTypes allTypes[];
extern DataTypes allParams[];

// RebindDlg dialog

class RebindDlg : public CDialog
{
	DECLARE_DYNAMIC(RebindDlg)

public:
	RebindDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~RebindDlg();
  BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_REBIND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

  int       m_type;
  int       m_rebind;
  CComboBox m_typeCombo;
  CComboBox m_rebindCombo;
public:
  afx_msg void OnCbnSelchangeDatatype();
  afx_msg void OnCbnSelchangeRebind();
  afx_msg void OnBnClickedReset();
  afx_msg void OnBnClickedAdd();
  afx_msg void OnBnClickedOk();
};
