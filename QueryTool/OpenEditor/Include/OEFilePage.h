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

class COEFilePage : public StyleDialog
{
public:
	COEFilePage (SettingsManager& manager,CWnd* p_parent);
	virtual ~COEFilePage();

// Dialog Data
	enum { IDD = IDD_OE_FILES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  virtual BOOL OnApply();

  SettingsManager& m_manager;

  BOOL m_Locking;
  BOOL m_MemMapForBig;
  BOOL m_SaveOnSwitch;
  BOOL m_ReloadChangedFiles;
  BOOL m_AutoscrollAfterReload;
  BOOL m_OverwriteReadonly;
  BOOL m_DetectFileChanges;

  StyleCheckbox m_checkLocking;
  StyleCheckbox m_checkMemMap;
  StyleCheckbox m_checkSaveSwitch;
  StyleCheckbox m_checkReload;
  StyleCheckbox m_checkOverwrite;
  StyleCheckbox m_checkDetect;
  StyleCheckbox m_checkAutoscroll;
  StyleEdit     m_editThreshold;
  int           m_MemMapThreshold;


  afx_msg void OnUpdateData();
};

