/* 
    Copyright (C) 2002 Aleksey Kochetov

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

#ifndef __NativeSQLDlg_H__
#define __NativeSQLDlg_H__

#pragma once
#include "resource.h"
#include "afxdialogex.h"

// CNativeSQLDlg dialog

class CNativeSQLDlg : public StyleDialog
{
	DECLARE_DYNAMIC(CNativeSQLDlg)

public:
	CNativeSQLDlg(CWnd*   pParent = NULL
               ,CString native  = ""
               ,CString title   = ""); 
	virtual ~CNativeSQLDlg();
  void SetMonoFont();
  BOOL OnInitDialog();

  // Dialog Data
	enum { IDD = IDD_NATIVE_SQL };
protected:
  virtual void CNativeSQLDlg::SetupDynamicLayout() override;

  StyleEdit   m_editNative;
  StyleButton m_buttonOK;
  CString     m_title;
  CString     m_native;
  CFont       m_font;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//DECLARE_MESSAGE_MAP()
};

#endif//__NativeSQLDlg_H__
