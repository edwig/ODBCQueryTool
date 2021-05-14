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
#include <map>
#include <vector>
#include "resource.h"
#include "OESettings.h"

using std::map;
using std::vector;
using Common::VisualAttribute;
using Common::VisualAttributesSet;
using OpenEditor::ClassSettings;
using OpenEditor::SettingsManager;

class CVisualAttributesPage : public StyleDialog
{
  CFont* m_pFont;
  vector<VisualAttributesSet*> m_vasets;
  map<const VisualAttribute*, VisualAttributesSet*> m_vasetsMap;

public:
  CVisualAttributesPage(SettingsManager& manager,CWnd* p_parent);
  virtual ~CVisualAttributesPage ();

	enum { IDD = IDD_OE_PROP_FONT_COLORS };

  StyleTreeCtrl	m_treeCategories;
  CStatic	      m_sample;
  StyleComboBox	m_comboFontName;
  StyleComboBox	m_comboFontSize;

  // Rework
	StyleCheckbox m_checkFontBold;
	StyleCheckbox m_checkFontItalic;
	StyleCheckbox m_checkFontUnderline;
	StyleComboBox	m_comboForeground;
	StyleComboBox	m_comboBackground;

  VisualAttributesSet*  GetCurrentAttributesSet();
  VisualAttribute*      GetCurrentAttribute();
  bool                  GetCurrentAttributeMixedWithBase(VisualAttribute&);
  const VisualAttribute*GetBaseAttribute(const VisualAttribute*);

  void    ShowAttribute ();
  void    ShowFont(const VisualAttribute&);
  void    SetFont (const VisualAttribute&);
  CString CreateColorString(DWORD p_color);

protected:
  virtual void    DoDataExchange(CDataExchange* pDX) override;
  virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam) override;
  virtual BOOL    OnInitDialog() override;

protected:
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnSelChangedOnCategory(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFontChanged();
	afx_msg void OnColorChanged();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

  DECLARE_MESSAGE_MAP()
};

