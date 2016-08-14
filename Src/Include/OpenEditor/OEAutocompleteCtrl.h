/* 
    Copyright (C) 2003 Aleksey Kochetov

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

#ifndef __OEAutocompleteCtrl_h__
#define __OEAutocompleteCtrl_h__

#include "OpenEditor/OETemplates.h"

    class COEditorView;

// COEAutocompleteCtrl

class COEAutocompleteCtrl : public CListCtrl
{
	//DECLARE_DYNAMIC(COEAutocompleteCtrl)
    
    bool m_hidden;
    HWND m_hwndEditor;
    OpenEditor::Square m_selection;

public:
	COEAutocompleteCtrl();
	virtual ~COEAutocompleteCtrl();

    BOOL Create ();
    void Populate (const OpenEditor::TemplatePtr);
    void CheckMatch (bool init = false);
    void ExpandTemplate (int index);

    void ShowControl (COEditorView* pEditor);
    void HideControl (BOOL focusToEditor = TRUE);

    BOOL IsActive () const { return m_hWnd && IsWindow(m_hWnd) && IsWindowVisible(); }

protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnSetFocus (CWnd* pOldWnd);
    afx_msg void OnKillFocus (CWnd* pNewWnd);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
    afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnDestroy();
protected:
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

#endif//__OEAutocompleteCtrl_h__
