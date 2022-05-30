/* 
    Copyright (C) 2005 Edwig Huisman

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

class COESQLSettingsPage : public StyleDialog
{
public:
	COESQLSettingsPage(SettingsManager& manager,CWnd* p_parent);
	virtual ~COESQLSettingsPage();

// Dialog Data
	enum { IDD = IDD_OE_SQLSETTINGS };

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;

	DECLARE_MESSAGE_MAP()
public:
  virtual BOOL OnApply();

  SettingsManager& m_manager;

  int           m_prefetch;
  CString       m_terminator;
  CString       m_font;
  bool          m_odbcMetaSQL;

  StyleEdit     m_editPrefetch;
  StyleEdit     m_editTerminator;
  StyleEdit     m_editFont;
  StyleButton   m_buttonFont;
  StyleCheckbox m_buttonODBC;

  afx_msg void OnUpdateData();
  afx_msg void OnEnChangePrefetch();
  afx_msg void OnEnChangeTerminator();
  afx_msg void OnEnChangeFont();
  afx_msg void OnBnClickedButFont();
  afx_msg void OnBnClickedPreferODBC();
};
