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

#ifndef __OEBackupPage_h__
#define __OEBackupPage_h__

#include "resource.h"
#include "OpenEditor/OESettings.h"

    using OpenEditor::SettingsManager;

class COEBackupPage : public CPropertyPage
{
public:
	COEBackupPage (SettingsManager& manager);
	virtual ~COEBackupPage();

// Dialog Data
	enum { IDD = IDD_OE_BACKUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnApply();

    SettingsManager& m_manager;

    BOOL m_Backup;
    CString m_BackupName;
    UINT m_MemMapThreshold;
    BOOL m_DetectFileChanges;
    CString m_BackupDirectory;
    int m_BackupMethod;

    afx_msg void OnUpdateData();
    afx_msg void OnBnClicked_FileBackupDirChange();
};

#endif//__OEBackupPage_h__
