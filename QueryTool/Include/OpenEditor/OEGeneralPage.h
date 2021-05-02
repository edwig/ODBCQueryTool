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

#ifndef __OEGeneralPage_h__
#define __OEGeneralPage_h__

#include "resource.h"
#include "OpenEditor/OESettings.h"


    using OpenEditor::SettingsManager;

class COEGeneralPage : public CPropertyPage
{
public:
	COEGeneralPage (SettingsManager& manager);
	virtual ~COEGeneralPage();

// Dialog Data
	enum { IDD = IDD_OE_GENERAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	//DECLARE_MESSAGE_MAP()
public:
    SettingsManager& m_manager;
    BOOL m_AllowMultipleInstances;
    BOOL m_NewDocOnStartup;
    BOOL m_MaximizeFirstDocument;
    BOOL m_WorkDirFollowsDoc;
    BOOL m_SaveCursPosAndBookmarks;
    BOOL m_SaveMainWindowPosition;
    virtual BOOL OnApply();
    CString m_keymapLayout;
    CString m_locale;

    static const char* m_keymapLayoutList;
};

#endif//__OEGeneralPage_h__
