/* 
    Copyright (C) 2004 Edwig Huisman

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

#ifndef __OEFindFiles_h__
#define __OEFindFiles_h__

#include "resource.h"
#include "OpenEditor/OESettings.h"
#include "GridControl/GridCtrl.h"

// For file operations
#include <io.h>           // Voor _findfirst/next/close
#include <direct.h>       // Voor _mkdir, _rmdir
#include <sys/stat.h>     // Voor stat operatie
#include <sys/types.h>    // Voor stat en localtime

using OpenEditor::SettingsManager;

//const bool Controls2Data = true;   // UpdateData
//const bool Data2Controls = false;
const int  MAX_BUFSIZE   = 1024;

class COEFindFiles : public CDialog
{
public:
	COEFindFiles (SettingsManager& manager);
	virtual ~COEFindFiles();

// Dialog Data
	enum { IDD = IDD_FIND_FILES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  virtual BOOL OnApply();
  virtual void OnOK();
  // Go find the requested files
  virtual bool GoFind(CGridCtrl* grid);
  virtual int  GoFindInFolder(CString Folder,CGridCtrl* grid);
  virtual void GoFindInFile(CString fileName,CGridCtrl* grid);
  virtual void AppendRow(CGridCtrl* grid,CString& fileName,int lineNumber,char* buffer);

  SettingsManager& m_manager;
  int m_whole_word;
  int m_match_case;
  int m_use_regex;
  int m_subfolders;
  int m_savebefore;
  int m_collapse;

  CString m_whattofind;
  CString m_filetypes;
  CString m_directories;

  afx_msg void OnUpdateData();
  afx_msg void OnBrowseDirectories();
};

#endif//__OEFindFiles_h__
