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

#ifndef __OEFindReplaceDlg_h__
#define __OEFindReplaceDlg_h__

#include "resource.h"
#include "OpenEditor/OEHelpers.h"
#include <COMMON/EditWithSelPos.h>


    class COEditorView;

class CFindReplaceDlg : public CDialog
{
    static CPoint m_ptTopLeft;
public:
    CString  m_SearchText, m_ReplaceText;

    BOOL m_MatchCase;
    BOOL m_MatchWholeWord;
    BOOL m_AllWindows;
    BOOL m_RegExp;
    BOOL m_BackslashExpressions;
    int  m_Direction;    // 0 - up, 1 - down
    int  m_WhereReplace; // 0 - in selection, 1 - in whole file

private:
    void toPrintableStr (const char* from, std::string& _to);
    void toUnprintableStr (const char* from, std::string& _to, bool skipEscDgt);

    COEditorView* m_pView;

    CComboBox m_wndSearchText;
    CComboBox m_wndReplaceText;
    CEditWithSelPos m_edtSearchText;
    CEditWithSelPos m_edtReplaceText;

    BOOL m_ReplaceMode;
    BOOL m_Modified;

    void AdjustPosition ();
    void SaveOption ();
    void SetupButtons ();
    void ShowPopupMenu (UINT btnId, UINT menuId);

// Construction
public:
    CFindReplaceDlg (BOOL replace, COEditorView*);
    void SearchBatch (OpenEditor::ESearchBatch mode);

    // Dialog Data
    //{{AFX_DATA(CFindReplaceDlg)
    enum { IDD = IDD_OE_EDIT_REPLACE };
    //}}AFX_DATA
    //{{AFX_VIRTUAL(CFindReplaceDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    //{{AFX_MSG(CFindReplaceDlg)
    afx_msg void OnFindNext();
    virtual BOOL OnInitDialog();
    afx_msg void OnChangeSettings();
    afx_msg void OnReplace();
    afx_msg void OnReplaceAll();
    afx_msg void OnAllWindows();
    afx_msg void OnReplaceWhere();
    afx_msg void OnSearchText();
    afx_msg void OnCount();
    afx_msg void OnMarkAll();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClicked_RegexpFind();
    afx_msg void OnBnClicked_RegexpReplace();
    afx_msg void OnInsertRegexpFind (UINT nID);
    afx_msg void OnInsertRegexpReplace (UINT nID);
protected:
    virtual void OnCancel();
    virtual void OnOK();
};

//{{AFX_INSERT_LOCATION}}

#endif//__OEFindReplaceDlg_h__
