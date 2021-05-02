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

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __OEFileInfoDlg_h__
#define __OEFileInfoDlg_h__

#include "resource.h"


class COEFileInfoDlg : public CPropertyPage
{
// Construction
public:
	COEFileInfoDlg ();

	//{{AFX_DATA(COEFileInfoDlg)
	enum { IDD = IDD_OE_FILE_INFO };
	CString	m_Created;
	CString	m_LastModified;
	CString	m_DriveUsage;
	CString	m_MemoryUsage;
	CString	m_MemoryAllocated;
	CString	m_Lines;
	CString	m_UndoMemoryUsage;
	CString	m_Path;
	//}}AFX_DATA


	//{{AFX_VIRTUAL(COEFileInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(COEFileInfoDlg)
	//}}AFX_MSG
	//DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif //__OEFileInfoDlg_h__
