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

#ifndef __OETemplatesPage_h__
#define __OETemplatesPage_h__

#include "resource.h"
#include "OpenEditor/OESettings.h"


class COETemplatesPage : public CPropertyPage
{
    OpenEditor::SettingsManager& m_manager;
    OpenEditor::TemplatePtr m_currTemplate;
    Common::VisualAttribute m_textAttr;

    int getCurrentSelection () const;
    void insertTemplListline (LV_ITEM&);

public:
	COETemplatesPage (OpenEditor::SettingsManager&);
	~COETemplatesPage ();

	//{{AFX_DATA(COETemplatesPage)
	enum { IDD = IDD_OE_TEMPLATES };
	CListCtrl m_templateList;
    CString   m_templateName;
	//}}AFX_DATA


	//{{AFX_VIRTUAL(COETemplatesPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(COETemplatesPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTemplList();
	//}}AFX_MSG
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
protected:
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
    afx_msg void OnBnClicked_ListIfAlternative();
};

//{{AFX_INSERT_LOCATION}}

#endif//__OETemplatesPage_h__
