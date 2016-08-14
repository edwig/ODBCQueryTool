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

#include "stdafx.h"
#include "OpenEditor/OEGoToDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COEGoToDialog dialog


COEGoToDialog::COEGoToDialog(CWnd* pParent /*=NULL*/)
	: CDialog(COEGoToDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(COEGoToDialog)
	m_WhereTo = 0;
	m_WhereFrom = -1;
	m_InfoString = _T("");
	//}}AFX_DATA_INIT
}


void COEGoToDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COEGoToDialog)
	DDX_Text(pDX, IDC_OEGT_WHERE_TO, m_WhereTo);
	DDX_Radio(pDX, IDC_OEGT_WHERE_FROM1, m_WhereFrom);
	DDX_Text(pDX, IDC_OEGT_INFO, m_InfoString);
	//}}AFX_DATA_MAP
}

/*
BEGIN_MESSAGE_MAP(COEGoToDialog, CDialog)
	//{{AFX_MSG_MAP(COEGoToDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
*/
/////////////////////////////////////////////////////////////////////////////
// COEGoToDialog message handlers
