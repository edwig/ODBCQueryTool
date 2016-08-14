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

#ifndef __VisualAttributesPage_h__
#define __VisualAttributesPage_h__

#include <map>
#include <vector>
#include "resource.h"
#include "OpenEditor/OESettings.h"

    using std::map;
    using std::vector;
    using Common::VisualAttribute;
    using Common::VisualAttributesSet;
    using OpenEditor::ClassSettings;
    using OpenEditor::SettingsManager;


class CVisualAttributesPage : public CPropertyPage
{
    CFont* m_pFont;
    vector<VisualAttributesSet*> m_vasets;
    map<const VisualAttribute*, VisualAttributesSet*> m_vasetsMap;

public:
	CVisualAttributesPage (vector<VisualAttributesSet*> data);
	~CVisualAttributesPage ();

	//{{AFX_DATA(CVisualAttributesPage)
	enum { IDD = IDD_OE_PROP_FONT_COLORS };
	CTreeCtrl	m_Categories;
	CComboBox	m_FontName;
	CComboBox	m_FontSize;
	CButton	    m_FontBold;
	CButton	    m_FontItalic;
	CButton	    m_FontUnderline;
	CComboBox	m_Foreground;
	CComboBox	m_Background;
	CStatic	    m_Sample;
	//}}AFX_DATA

    VisualAttributesSet* GetCurrentAttributesSet ();
    VisualAttribute* GetCurrentAttribute ();
    bool GetCurrentAttributeMixedWithBase (VisualAttribute&);
    const VisualAttribute* GetBaseAttribute (const VisualAttribute*);

    void ShowAttribute ();
    void ShowFont (const VisualAttribute&);
    void SetFont (const VisualAttribute&);

	//{{AFX_VIRTUAL(CVisualAttributesPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

protected:
	//{{AFX_MSG(CVisualAttributesPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnSelChangedOnCategory(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFontChanged();
	afx_msg void OnColorChanged();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}

#endif//__VisualAttributesPage_h__
