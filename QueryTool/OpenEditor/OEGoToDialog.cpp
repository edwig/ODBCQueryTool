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

#include "pch.h"
#include "OEGoToDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COEGoToDialog dialog


COEGoToDialog::COEGoToDialog(CWnd* pParent,int whereFrom,int whereTo,CString p_info)
	            :StyleDialog(COEGoToDialog::IDD, pParent)
              ,m_WhereTo(whereTo)
              ,m_WhereFrom(whereFrom)
              ,m_InfoString(p_info)
{
}

void 
COEGoToDialog::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);

  DDX_Control(pDX, IDC_OEGT_WHERE_TO,   m_editTo,  m_WhereTo);
  DDX_Control(pDX, IDC_OEGT_INFO,       m_editInfo,m_InfoString);
  DDX_Control(pDX, IDC_OEGT_WHERE_FROM1,m_checkTop);
  DDX_Control(pDX, IDC_OEGT_WHERE_FROM2,m_checkBottom);
  DDX_Control(pDX, IDC_OEGT_WHERE_FROM3,m_checkCurrent);
  DDX_Radio  (pDX, IDC_OEGT_WHERE_FROM1,m_WhereFrom);
  DDX_Control(pDX, IDOK,                m_buttonOK);
  DDX_Control(pDX, IDCANCEL,            m_buttonCancel);
}

/*
BEGIN_MESSAGE_MAP(COEGoToDialog, CDialog)
	//{{AFX_MSG_MAP(COEGoToDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
*/

BOOL
COEGoToDialog::OnInitDialog()
{
  StyleDialog::OnInitDialog();
  SetWindowText(_T("Goto line"));

  return InitFirstFocus();
}

/////////////////////////////////////////////////////////////////////////////
// COEGoToDialog message handlers
