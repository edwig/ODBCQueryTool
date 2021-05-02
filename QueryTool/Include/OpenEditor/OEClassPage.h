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

#ifndef __OEClassPage_h__
#define __OEClassPage_h__

#include "resource.h"
#include "OpenEditor/OESettings.h"


    using OpenEditor::ClassSettings;
    using OpenEditor::SettingsManager;


class COEClassPage : public CPropertyPage
{
    SettingsManager& m_manager;
    bool m_dataInitialized;
public:
	COEClassPage(SettingsManager&);
	~COEClassPage();

	//{{AFX_DATA(COEClassPage)
	enum { IDD = IDD_OE_CLASSES };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(COEClassPage)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(COEClassPage)
	afx_msg void OnSelChangeLanguage();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeData();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif//__OEClassPage_h__
