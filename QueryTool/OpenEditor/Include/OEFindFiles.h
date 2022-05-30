/* 
    Copyright (C) 2004 - 2017 Edwig Huisman

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

#ifndef __OEFindFiles_h__
#define __OEFindFiles_h__

#include "resource.h"
#include "OESettings.h"
#include "MGridCtrl.h"

// For file operations
#include <io.h>           // _findfirst/next/close
#include <direct.h>       // _mkdir, _rmdir
#include <sys/stat.h>     // stat operation
#include <sys/types.h>    // stat and localtime

using OpenEditor::SettingsManager;

const int  MAX_BUFSIZE   = 1024;

class COEFindFiles : public StyleDialog
{
public:
	COEFindFiles (SettingsManager& manager,CWnd* p_parent);
	virtual ~COEFindFiles();

// Dialog Data
	enum { IDD = IDD_FIND_FILES };

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;
  virtual void SetupDynamicLayout() override;

	DECLARE_MESSAGE_MAP()
public:
  virtual BOOL OnApply();
  virtual void OnOK();
  // Go find the requested files
  virtual bool GoFind(StyleGridCtrl* grid);
  virtual int  GoFindInFolder(CString Folder,StyleGridCtrl* grid);
  virtual void GoFindInFile(CString fileName,StyleGridCtrl* grid);
  virtual void AppendRow(StyleGridCtrl* grid,CString& fileName,int lineNumber,char* buffer);

  SettingsManager& m_manager;

  int m_whole_word;
  int m_match_case;
  int m_use_regex;
  int m_subfolders;
  int m_savebefore;
  int m_collapse;

  StyleCheckbox m_checkWholeWord;
  StyleCheckbox m_checkMatchCase;
  StyleCheckbox m_checkUseRegex;
  StyleCheckbox m_checkSubfolders;
  StyleCheckbox m_checkSaveBefore;
  StyleCheckbox m_checkCollapse;

  StyleEdit m_editWhatToFind;
  StyleEdit m_editFileTypes;
  StyleEdit m_editDirectories;

  StyleButton m_buttonBrowse;
  StyleButton m_buttonFind;
  StyleButton m_buttonCancel;

  CString   m_whattofind;
  CString   m_filetypes;
  CString   m_directories;

  afx_msg void OnUpdateData();
  afx_msg void OnBrowseDirectories();
};

#endif//__OEFindFiles_h__
