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
using OpenEditor::InstanceSettings;
using OpenEditor::SettingsManager;


class COEPropPage : public StyleDialog
{
	
public:
	COEPropPage (const SettingsManager&, InstanceSettings&,CWnd* p_parent);
 ~COEPropPage ();

	enum { IDD = IDD_OE_FILE_SETTINGS };

protected:
	virtual void    DoDataExchange(CDataExchange* pDX);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL    OnInitDialog();

  afx_msg void OnCbnSelChangeLanguage();
  afx_msg void OnCbnSelChangeFileType();
  afx_msg void OnEnChangeTabSize();
  afx_msg void OnEnChangeIndentSize();
  afx_msg void OnBnClickedInsertSpaces();
  afx_msg void OnBnClickedKeepTabs();
  afx_msg void OnBnClickedIndNone();
  afx_msg void OnBnClickedIndDefault();
  afx_msg void OnBnClickedIndSmart();

  DECLARE_MESSAGE_MAP()

private:
  void ChangeLanguage();

  bool                    m_initialized;
  const SettingsManager&  m_manager;
  InstanceSettings&       m_settings;
  StyleComboBox           m_comboLanguage;
  StyleComboBox           m_comboFileType;
  StyleEdit               m_editTabSize;
  StyleEdit               m_editIndentSize;
  StyleSpinButtonCtrl     m_spinTabSize;
  StyleSpinButtonCtrl     m_spinIndentSize;
  StyleCheckbox           m_radioInsSpaces;
  StyleCheckbox           m_radioKeepTabs;
  StyleCheckbox           m_radioIndNone;
  StyleCheckbox           m_radioIndDefault;
  StyleCheckbox           m_radioIndSmart;
  int                     m_tabSize   { 0 };
  int                     m_indentSize{ 0 };
};

