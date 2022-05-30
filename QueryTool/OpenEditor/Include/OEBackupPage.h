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

#pragma once
#include "resource.h"
#include "OESettings.h"

using OpenEditor::SettingsManager;

class COEBackupPage : public StyleDialog
{
public:
	COEBackupPage (SettingsManager& manager,CWnd* p_parent);
	virtual ~COEBackupPage();

// Dialog Data
	enum { IDD = IDD_OE_BACKUP };

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;

	DECLARE_MESSAGE_MAP()
public:
  virtual BOOL OnApply();

  SettingsManager& m_manager;

  BOOL    m_Backup;
  CString m_BackupName;
  CString m_BackupDirectory;

  UINT    m_MemMapThreshold;
  BOOL    m_DetectFileChanges;
  int     m_BackupMethod;

  StyleCheckbox m_checkBackup;
  StyleEdit     m_editBackupName;
  StyleEdit     m_editBackupDirectory;
  StyleCheckbox m_radioToFile;
  StyleCheckbox m_radioToDir;
  StyleButton   m_buttonDir;

  afx_msg void OnUpdateData();
  afx_msg void OnBnClicked_FileBackupDirChange();
};
