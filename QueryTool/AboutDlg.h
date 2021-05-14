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

#ifndef __AboutDlg_H__
#define __AboutDlg_H__

#pragma once
#include "resource.h"

// CAboutDlg dialog

class CAboutDlg : public StyleDialog
{
	DECLARE_DYNAMIC(CAboutDlg)

public:
	CAboutDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAboutDlg();

// Dialog Data
	enum { IDD = IDD_OE_ABOUTBOX };

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;

	DECLARE_MESSAGE_MAP()
public:
  StyleEdit   m_editLicense;
  CString     m_license;
  StyleButton m_buttonOK;
	afx_msg void OnStnClickedOeaLogo();
};

#endif//__AboutDlg_H__
