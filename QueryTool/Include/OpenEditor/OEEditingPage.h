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

#ifndef __OEEditingPage_h__
#define __OEEditingPage_h__

#include "resource.h"
#include "OpenEditor/OESettings.h"

    using OpenEditor::SettingsManager;

class COEEditingPage : public CPropertyPage
{
public:
	COEEditingPage (SettingsManager& manager);
	~COEEditingPage ();

// Dialog Data
	//{{AFX_DATA(COEEditingPage)
	enum { IDD = IDD_OE_EDITING };
    CString m_DefFileExtension;
	UINT	m_MaxUndoCount;
    UINT	m_UndoMemLimit;
    BOOL    m_UndoAfterSaving;
	BOOL	m_CursorBeyondEOL;
	BOOL	m_CursorBeyondEOF;
	BOOL	m_TruncateSpaces;
	//}}AFX_DATA
    SettingsManager& m_manager;


	//{{AFX_VIRTUAL(COEEditingPage)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(COEEditingPage)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClicked_CursorBeyondEol();
};

//{{AFX_INSERT_LOCATION}}

#endif//__OEEditingPage_h__
