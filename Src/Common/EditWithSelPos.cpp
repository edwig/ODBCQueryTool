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

#include "stdafx.h"
#include <COMMON/EditWithSelPos.h>
#include <winuser.h>

// typedef struct tagCOMBOBOXINFO
// {
//     DWORD cbSize;
//     RECT  rcItem;
//     RECT  rcButton;
//     DWORD stateButton;
//     HWND  hwndCombo;
//     HWND  hwndItem;
//     HWND  hwndList;
// } COMBOBOXINFO, *PCOMBOBOXINFO, *LPCOMBOBOXINFO;
// CEditWithSelPos

CEditWithSelPos::CEditWithSelPos()
{
    m_nStartChar = m_nEndChar = 0;
}

BOOL CEditWithSelPos::SubclassComboBoxEdit (HWND hWnd)
{
    COMBOBOXINFO info;
    info.cbSize = sizeof(info);

    // Win95 compatibility
    static HINSTANCE g_user32_dll = NULL;

    if (!g_user32_dll) 
        g_user32_dll = LoadLibrary("USER32.DLL");

    typedef BOOL (WINAPI *GetComboBoxInfo_t)(HWND, PCOMBOBOXINFO);
    FARPROC g_getComboBoxInfo = NULL;

    if (!g_getComboBoxInfo && g_user32_dll) 
        g_getComboBoxInfo = GetProcAddress(g_user32_dll, "GetComboBoxInfo");

    if (g_getComboBoxInfo)
    {
        if (((GetComboBoxInfo_t)g_getComboBoxInfo)(hWnd, &info))
        {
            SubclassWindow(info.hwndItem);
            return TRUE;
        }
    }

    //if (::GetComboBoxInfo(hWnd, &info))
    //{
    //    SubclassWindow(info.hwndItem);
    //    return TRUE;
    //}

    return FALSE;
}

void CEditWithSelPos::InsertAtCurPos (const char* str, int offset)
{
    CString text;
    GetWindowText(text);
    
    if (m_nStartChar != m_nEndChar)
        text.Delete(m_nStartChar, m_nEndChar - m_nStartChar);
    
    text.Insert(m_nStartChar, str);
    SetWindowText(text);

    int pos = m_nStartChar + ((offset != -1) ? offset : (int)strlen(str));
    
    SetWindowText(text);
    SetFocus();
    SetSel(pos, pos);
}

BEGIN_MESSAGE_MAP(CEditWithSelPos, CEdit)
    ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

// CEditWithSelPos message handlers

void CEditWithSelPos::OnKillFocus (CWnd* pNewWnd)
{
    GetSel(m_nStartChar, m_nEndChar);
    CEdit::OnKillFocus(pNewWnd);
}
