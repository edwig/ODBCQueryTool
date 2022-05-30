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

using OpenEditor::ClassSettings;
using OpenEditor::SettingsManager;

class COEClassPage : public StyleDialog
{
  SettingsManager& m_manager;
  bool m_dataInitialized;
public:
  COEClassPage(SettingsManager&,CWnd* p_parent);
  virtual ~COEClassPage();

  enum { IDD = IDD_OE_CLASSES };

public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;
  virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam) override;

protected:
	afx_msg void OnSelChangeLanguage();
	afx_msg void OnChangeData();

  DECLARE_MESSAGE_MAP()

  CString       m_class;
  CString       m_fileExtensions;
  CString       m_fileType;
  CString       m_delimiters;
  int           m_tabSize;
  int           m_indentSize;
  int           m_insertSpaces;
  int           m_indent;

  StyleComboBox m_comboClass;
  StyleEdit     m_editFileExtensions;
  StyleComboBox m_comboFileType;
  StyleEdit     m_editDelimiters;
  StyleEdit     m_editTabSize;
  StyleEdit     m_editIndentSize;
  StyleSpinButtonCtrl m_spinTabSize;
  StyleSpinButtonCtrl m_spinIndentSize;
  StyleCheckbox m_checkKeepTabs;
  StyleCheckbox m_checkInsertSpaces;
  StyleCheckbox m_checkIndentNone;
  StyleCheckbox m_checkIndentDefault;
  StyleCheckbox m_checkIndentSmart;
  StyleCheckbox m_checkLineNumbers;
  StyleCheckbox m_checkVisibleSpaces;
  StyleCheckbox m_checkNormalizeKW;
};
