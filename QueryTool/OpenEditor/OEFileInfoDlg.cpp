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
#include "resource.h"
#include "OpenEditor/OEFileInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COEFileInfoDlg dialog


COEFileInfoDlg::COEFileInfoDlg ()
	: CPropertyPage(COEFileInfoDlg::IDD)
{
    /*
	//{{AFX_DATA_INIT(COEFileInfoDlg)
	m_Created = _T("Unknown");
	m_LastModified = _T("Unknown");
	m_DriveUsage = _T("Unknown");
	m_MemoryUsage = _T("Unknown");
	m_MemoryAllocated = _T("Unknown");
	m_Lines = _T("Unknown");
	m_UndoMemoryUsage = _T("Unknown");
	m_Path = _T("");
	//}}AFX_DATA_INIT
    */
}


void COEFileInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COEFileInfoDlg)
	DDX_Text(pDX, IDC_OEP_CREATED, m_Created);
	DDX_Text(pDX, IDC_OEP_LAST_MODIFIED, m_LastModified);
	DDX_Text(pDX, IDC_OEP_DRIVE_USAGE, m_DriveUsage);
	DDX_Text(pDX, IDC_OEP_MEMORY_USAGE, m_MemoryUsage);
	DDX_Text(pDX, IDC_OEP_MEMORY_ALLOCATED, m_MemoryAllocated);
	DDX_Text(pDX, IDC_OEP_LINES, m_Lines);
	DDX_Text(pDX, IDC_OEP_UNDO_MEMORY, m_UndoMemoryUsage);
	DDX_Text(pDX, IDC_OEP_PATH, m_Path);
	//}}AFX_DATA_MAP
}

/*
BEGIN_MESSAGE_MAP(COEFileInfoDlg, CPropertyPage)
	//{{AFX_MSG_MAP(COEFileInfoDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
*/
/////////////////////////////////////////////////////////////////////////////
// COEFileInfoDlg message handlers
