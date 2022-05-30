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

class COETemplatesPage : public StyleDialog
{
  OpenEditor::SettingsManager& m_manager;
  OpenEditor::TemplatePtr m_currTemplate;
  Common::VisualAttribute m_textAttr;

  int getCurrentSelection () const;
  void insertTemplListline (LV_ITEM&);

public:
	COETemplatesPage(OpenEditor::SettingsManager&,CWnd* p_parent);
	virtual ~COETemplatesPage ();

	enum { IDD = IDD_OE_TEMPLATES };

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;

  StyleListCtrl m_templateList;
  CString       m_templateName;
  StyleComboBox m_comboTemplate;
  StyleCheckbox m_alternative;

protected:
	afx_msg void OnSelchangeTemplList();

  DECLARE_MESSAGE_MAP()

public:
  afx_msg void OnLvnItemchanged_TemplList(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnLvnGetdispinfo_TemplList(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnBnClicked_AddTempl();
  afx_msg void OnBnClicked_EditTempl();
  afx_msg void OnBnClicked_DelTempl();
  afx_msg void OnBnClicked_MoveDownTempl();
  afx_msg void OnBnClicked_MoveUpTempl();
  afx_msg void OnNMDblclk_TemplList(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnCbnSelchange_Category();
  afx_msg void OnBnClicked_ListIfAlternative();
protected:
  virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam) override;
};

