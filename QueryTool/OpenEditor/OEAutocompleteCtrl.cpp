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

/*
    16.03.2003 bug fix, autocomplete template list cannot be activated on word delemiters ( \t"',! ...)
    16.03.2003 bug fix, autocomplete template list does not show properly on the second monitor
*/

#include "pch.h"
#include "resource.h"
#include "OEView.h"
#include "OEAutocompleteCtrl.h"

#pragma warning(disable: 4996)

#ifdef _AFXDLL
#define COMPILE_MULTIMON_STUBS
#include <MultiMon.h>
#else
// extern "C" {
//     extern BOOL InitMultipleMonitorStubs(void);
//     extern HMONITOR (WINAPI* g_pfnMonitorFromWindow)(HWND, DWORD);
//     extern BOOL     (WINAPI* g_pfnGetMonitorInfo)(HMONITOR, LPMONITORINFO);
// };
#endif

const int MIN_WIDTH  = 200;
const int MIN_HEIGHT = 100;
const int KEY_WIDTH  = 60;
const int NAME_WIDTH = 110;

const char* cszSection   = "AutocompleteCtrl";
const char* cszWidth     = "Width";
const char* cszHeight    = "Height";
const char* cszKeyWidth  = "KeyWidth";
const char* cszNameWidth = "NameWidth";

using namespace OpenEditor;

// COEAutocompleteCtrl

//IMPLEMENT_DYNAMIC(COEAutocompleteCtrl, CListCtrl)

COEAutocompleteCtrl::COEAutocompleteCtrl()
{
    m_hwndEditor = 0;
    m_hidden = true;
    
    //InitMultipleMonitorStubs();
}

COEAutocompleteCtrl::~COEAutocompleteCtrl()
{
}

BEGIN_MESSAGE_MAP(COEAutocompleteCtrl, CListCtrl)
    ON_WM_KILLFOCUS()
    ON_WM_SETFOCUS()
    ON_WM_KEYDOWN()
    ON_WM_CHAR()
    ON_WM_GETMINMAXINFO()
    ON_NOTIFY_REFLECT(NM_DBLCLK, OnNMDblclk)
    ON_WM_DESTROY()
END_MESSAGE_MAP()

// COEAutocompleteCtrl message handlers

void COEAutocompleteCtrl::ShowControl (COEditorView* pEditor)
{
    _ASSERTE(pEditor);

    const TemplatePtr tmpl = pEditor->GetSettings().GetTemplate();
    if (!tmpl->GetCount())
    {
        AfxMessageBox(
            (std::string("Template \"") + pEditor->GetSettings().GetName() + "\" is empty."
                "\n\nYou should populate it before using."
                "\n\nOn \"Text\" menu, click \"Permanent Settings\" item\t\nand choose \"Templates\" tab.").c_str(), 
            MB_ICONEXCLAMATION);
        return;
    }
    Populate(tmpl);

    m_hidden = false;
    m_hwndEditor = pEditor->m_hWnd;
    CheckMatch(true);

    if (m_hidden) return;
    
    int rowHeight = pEditor->m_Rulers[1].m_CharSize;
    POINT pt;
    pt.x = pEditor->m_Rulers[0].PosToPix(m_selection.start.column); 
    pt.y = pEditor->m_Rulers[1].PosToPix(m_selection.start.line+1);
    
    pEditor->ClientToScreen(&pt);

    CRect rc, desktopRc, ctrlRc;
    GetWindowRect(rc);

//     // 16.03.2003 bug fix, autocomplete template list does not show properly on the second monitor
//     if (g_pfnMonitorFromWindow)
//     {
//         HMONITOR hmonitor = g_pfnMonitorFromWindow(*pEditor, MONITOR_DEFAULTTONEAREST);
//         MONITORINFO monitorInfo;
//         memset(&monitorInfo, 0, sizeof(monitorInfo));
//         monitorInfo.cbSize = sizeof(monitorInfo);
// 
//         if (g_pfnGetMonitorInfo(hmonitor, &monitorInfo))
//         {
//             desktopRc = monitorInfo.rcMonitor;
//         }
//         else
//             GetDesktopWindow()->GetWindowRect(desktopRc);
//     }
//     else
        GetDesktopWindow()->GetWindowRect(desktopRc);

    // vert position adjustment
    if (pt.y + rc.Height() > desktopRc.bottom           // it does not fit downward
    && pt.y - rowHeight > desktopRc.Height() - pt.y)    // && pt is below the center
        pt.y -= rowHeight + rc.Height();
    
    // horz position adjustment
    if (pt.x + rc.Width() > desktopRc.right)
        pt.x = desktopRc.right - rc.Width();
   
    ctrlRc = CRect(pt, rc.Size());

    // clipping
    if (ctrlRc.top < 0) ctrlRc.top = 0;
    if (ctrlRc.bottom > desktopRc.bottom) ctrlRc.bottom = desktopRc.bottom;
    if (ctrlRc.left < 0) ctrlRc.left = 0;
    if (ctrlRc.right > desktopRc.right) ctrlRc.right = desktopRc.right;

    SetWindowPos(0, ctrlRc.left, ctrlRc.top, ctrlRc.Width(), ctrlRc.Height(), SWP_SHOWWINDOW);
    SetFocus();
}

void COEAutocompleteCtrl::CheckMatch (bool init)
{
    COEditorView* pEditor = (COEditorView*)FromHandlePermanent(m_hwndEditor);
    if (pEditor && pEditor->IsKindOf(RUNTIME_CLASS(COEditorView)))
    {
        bool resetSelecetion = false;
        std::string substring;
        Square selection;

        if (pEditor->GetBlockOrWordUnderCursor(substring, selection, false))
        {
            // don't support multiline block
            if (selection.start.line != selection.end.line) 
                resetSelecetion = true;

            // selection must not contain delemiters
            for (std::string::const_iterator it = substring.begin(); it != substring.end(); ++it)
                if (pEditor->GetDelimiters()[*it]) 
                {
                    resetSelecetion = true;
                    break;
                }
        }
        else
            resetSelecetion = true;

        // 16.03.2003 bug fix, autocomplete template list cannot be activated on word delemiters ( \t"',! ...)
        if (resetSelecetion)
        {
            selection.start = 
            selection.end = pEditor->GetPosition();
        }

        LVFINDINFO info;
        info.flags = LVFI_PARTIAL|LVFI_STRING;
        info.psz   = substring.c_str(); 
        int index  = FindItem(&info);
        if (index != -1)
        {
            int top = GetTopIndex();
            int count = GetCountPerPage();
            // the item is not visible
            if (!(index >= top && index < top + count))
            {
                top = index > count/2 ? index - count/2 : 0;
                EnsureVisible(top+count, FALSE);
                EnsureVisible(top, FALSE);
                //EnsureVisible(0, FALSE);
                //EnsureVisible(index + count/2, FALSE);
            }
            EnsureVisible(index, FALSE);
 
            UINT state = (substring.length() && !_strnicmp(substring.c_str(), GetItemText(index, 0), substring.length())) 
                 ? LVIS_FOCUSED|LVIS_SELECTED : LVIS_FOCUSED;
            
            SetItemState(index, state, state);
            
            if (state == LVIS_FOCUSED)
            {
                POSITION pos = GetFirstSelectedItemPosition();
                index = GetNextSelectedItem(pos);
                SetItemState(index, 0, LVIS_SELECTED);
            }
        }

        if (!init 
        && (selection.start.line != selection.start.line
            || selection.start.column != m_selection.start.column))
        {
            HideControl();
        }

        m_selection = selection;
    }
}

void COEAutocompleteCtrl::ExpandTemplate (int index)
{
    if (index != -1)
    {
        COEditorView* pEditor = (COEditorView*)FromHandlePermanent(m_hwndEditor);
        if (pEditor && pEditor->IsKindOf(RUNTIME_CLASS(COEditorView)))
        {
            EditContext::UndoGroup undoGroup(*pEditor);
            pEditor->SetSelection(m_selection);
            pEditor->DeleteBlock();
            pEditor->ExpandTemplate((int)GetItemData(index));
        }
    }
}

void COEAutocompleteCtrl::HideControl (BOOL focusToEditor)
{
    if (!m_hidden && m_hWnd)
    {
        if (focusToEditor)
        {
            _ASSERTE(m_hwndEditor && ::IsWindow(m_hwndEditor));
            if (m_hwndEditor && ::IsWindow(m_hwndEditor))
                ::SetFocus(m_hwndEditor);
        }
        ShowWindow(SW_HIDE);
        m_hidden = true;
    }
}

BOOL COEAutocompleteCtrl::Create ()
{
    BOOL retVal = CWnd::CreateEx(
        WS_EX_PALETTEWINDOW|WS_EX_WINDOWEDGE,
        WC_LISTVIEW, 
        "OEAutocompleteCtrl",
        WS_POPUP|WS_THICKFRAME
        |LVS_REPORT|LVS_SINGLESEL|LVS_SORTASCENDING|LVS_NOSORTHEADER, 
		0, 0, 
        AfxGetApp()->GetProfileInt(cszSection, cszWidth, MIN_WIDTH), 
        AfxGetApp()->GetProfileInt(cszSection, cszHeight, MIN_HEIGHT), 
        AfxGetMainWnd()->m_hWnd, 0, 0);
	
    LV_COLUMN lvcolumn;
    for (int i = 0; i < 2; i++) 
    {
        lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
        lvcolumn.fmt = LVCFMT_LEFT;
        lvcolumn.pszText = !i ? "Key" : "Name";
        lvcolumn.iSubItem = i;
        if (!i) lvcolumn.cx = AfxGetApp()->GetProfileInt(cszSection, cszKeyWidth,  KEY_WIDTH);
        else    lvcolumn.cx = AfxGetApp()->GetProfileInt(cszSection, cszNameWidth, NAME_WIDTH);
        InsertColumn(i, &lvcolumn);
    }

    SetExtendedStyle(LVS_EX_FULLROWSELECT);

    return retVal;
}

void COEAutocompleteCtrl::Populate (const TemplatePtr tmpl)
{
    DeleteAllItems();
    
    Template::ConstIterator 
        it = tmpl->Begin(), end = tmpl->End();

    int i = 0;
    for (i = 0; it != end; i++, ++it)
    {
        LVITEM item;
        item.iItem  = i;
        item.lParam = (LPARAM)i;
        item.iSubItem = 0;
        item.mask = LVIF_TEXT|LVIF_PARAM;
        item.pszText = (LPSTR)it->keyword.c_str(); 
        item.iItem = InsertItem(&item);
        SetItemText(item.iItem, 1, (LPSTR)it->name.c_str());
    }

    if (i > 0)
        SetItemState(0, LVIS_FOCUSED|LVIS_SELECTED, LVIS_FOCUSED|LVIS_SELECTED);
}

void COEAutocompleteCtrl::OnSetFocus (CWnd* pOldWnd)
{
    CListCtrl::OnSetFocus(pOldWnd);

    CWnd* pEditor = FromHandlePermanent(m_hwndEditor);
    if (pEditor && pEditor->IsKindOf(RUNTIME_CLASS(COEditorView)))
        ((COEditorView*)pEditor)->ShowCaret(TRUE);
}

void COEAutocompleteCtrl::OnKillFocus (CWnd* pNewWnd)
{
    CListCtrl::OnKillFocus(pNewWnd);

    HideControl(FALSE);

    CWnd* pEditor = FromHandlePermanent(m_hwndEditor);
    if (pEditor && pNewWnd != pEditor && pEditor->IsKindOf(RUNTIME_CLASS(COEditorView)))
        ((COEditorView*)pEditor)->ShowCaret(FALSE);
}

void COEAutocompleteCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    switch (nChar)
    {
    case VK_LEFT:
    case VK_RIGHT:
        if (m_hwndEditor && ::IsWindow(m_hwndEditor))
        {
            ::SendMessage(m_hwndEditor, WM_KEYDOWN, nChar, MAKELPARAM(nRepCnt, nFlags));
            CheckMatch();
        }
        break;
    case VK_BACK:
    case VK_DELETE:
        if (m_hwndEditor && ::IsWindow(m_hwndEditor))
        {
            ::SendMessage(m_hwndEditor, WM_KEYDOWN, nChar, MAKELPARAM(nRepCnt, nFlags));
            CheckMatch();
        }
        break;
    case VK_RETURN:
        {
            COEditorView* pEditor = (COEditorView*)FromHandlePermanent(m_hwndEditor);
            if (pEditor && pEditor->IsKindOf(RUNTIME_CLASS(COEditorView)))
            {
                POSITION pos = GetFirstSelectedItemPosition();
                ExpandTemplate(GetNextSelectedItem(pos));
            }
        }
    case VK_ESCAPE:
        HideControl();
        break;
    default:
        CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
    }
}

void COEAutocompleteCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (isalnum((char)nChar) || nChar == VK_BACK)
    {
        if (m_hwndEditor && ::IsWindow(m_hwndEditor))
        {
            ::SendMessage(m_hwndEditor, WM_CHAR, nChar, MAKELPARAM(nRepCnt, nFlags));
            CheckMatch();
        }
    }
    else
        HideControl();
}

void COEAutocompleteCtrl::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
    CListCtrl::OnGetMinMaxInfo(lpMMI);

    CRect desktopRc;
    GetDesktopWindow()->GetWindowRect(desktopRc);
    lpMMI->ptMaxTrackSize = CPoint(desktopRc.Width(), desktopRc.Height());
    lpMMI->ptMinTrackSize = CPoint(MIN_WIDTH, MIN_HEIGHT);
}

void COEAutocompleteCtrl::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pItemActivate = (LPNMITEMACTIVATE)pNMHDR;

    if (pItemActivate 
    && pItemActivate->iItem != -1)
        ExpandTemplate(pItemActivate->iItem);

    HideControl();

    *pResult = 0;
}

void COEAutocompleteCtrl::OnDestroy()
{
    CRect rc; 
    GetWindowRect(rc);
    AfxGetApp()->WriteProfileInt(cszSection, cszWidth,     rc.Width());
    AfxGetApp()->WriteProfileInt(cszSection, cszHeight,    rc.Height());
    AfxGetApp()->WriteProfileInt(cszSection, cszKeyWidth,  GetColumnWidth(0));
    AfxGetApp()->WriteProfileInt(cszSection, cszNameWidth, GetColumnWidth(1));
    CListCtrl::OnClose();
}

LRESULT COEAutocompleteCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    try
    {
        return CListCtrl::WindowProc(message, wParam, lParam);
    }
    _OE_DEFAULT_HANDLER_;

    return 0;
}
