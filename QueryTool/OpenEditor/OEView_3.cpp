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

/*
     9/04/2002 bug fix, paint fail if delete block which start above the top of page
    25/09/2002 bug fix, smart scrolling on insert/delete for views which are linked to the same storage
    09.03.2003 bug fix, horz scroll bar does not appear when it's supposed to do
    09.03.2003 bug fix, sroll limit should be maximum of line count and the last line on the screen
    17.03.2003 bug fix, cursor position become wrong after document type was changed

    Deferred tasks:
        optimize OnHScroll & OnVScroll, add SrollWindow

*/
#include "pch.h"
#include "OEDocument.h"
#include "OEView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace OpenEditor;


void COEditorView::EnableSyntaxHighlight (BOOL enable)
{
    if (m_bSyntaxHighlight != enable)
    {
        m_bSyntaxHighlight = enable;
        Invalidate(FALSE);
    }
}


// helper function for scrolling service
bool COEditorView::Ruler::OnScroll (UINT code, UINT pos, int last)
{
    int old_topmost = m_Topmost;

    switch (code)
    {
    case SB_LINELEFT:
        if (m_Topmost > 0) m_Topmost--;
        break;

    case SB_LINERIGHT:
        /*if (last - m_Topmost > m_Count)*/ m_Topmost++;
        break;

    case SB_PAGEUP:
        if (m_Topmost > 0)
            m_Topmost = max(0, m_Topmost - m_Count + 1);
        break;

    case SB_PAGEDOWN:
//        if (last - m_Topmost > m_Count)
        if (last > m_Topmost)
            m_Topmost = min(last, m_Topmost + m_Count - 1);
        break;

    case SB_THUMBPOSITION:
    case SB_THUMBTRACK:
        m_Topmost = min(max(0, (int)pos), last);
        break;
    }

    _ASSERTE(m_Topmost >= 0);

    return (old_topmost != m_Topmost) ? true : false;
}

void COEditorView::SetCaretPosition ()
{
    HWND hWnd = ::GetFocus();

    if (hWnd == m_hWnd || m_autocompleteList.m_hWnd)
    {
        Position pos = GetPosition();

        if (pos.line >= m_Rulers[1].m_Topmost
        && pos.column >= m_Rulers[0].m_Topmost)
            ::SetCaretPos(m_Rulers[0].PosToPix(pos.column)/* + 1*/, m_Rulers[1].PosToPix(pos.line));
        else
            ::SetCaretPos(-10000, -10000);
    }
}

bool COEditorView::GetCaretPosition (CPoint& pt) const
{
    Position pos = GetPosition();

    if (pos.line >= m_Rulers[1].m_Topmost
    && pos.column >= m_Rulers[0].m_Topmost)
    {
        pt.x = m_Rulers[0].PosToPix(pos.column);
        pt.y = m_Rulers[1].PosToPix(pos.line);

        return (pt.x <= m_Rulers[0].m_ClientSize && pt.y <= m_Rulers[1].m_ClientSize) ? true : false;
    }

    return false;
}

void COEditorView::AdjustCaretPosition ()
{
    bool redraw = false;
    Position pos = GetPosition();

    for (int i(0); i < 2; i++)
    {
        if (m_Rulers[i].m_Topmost > pos.coordinate[i])
        {
            m_Rulers[i].m_Topmost = pos.coordinate[i];
            _ASSERTE(m_Rulers[i].m_Topmost >= 0);
            redraw = true;
        }
        else if (pos.coordinate[i] >= m_Rulers[i].m_Topmost + m_Rulers[i].m_Count)
        {
            m_Rulers[i].m_Topmost = pos.coordinate[i] - m_Rulers[i].m_Count + 1;
            _ASSERTE(m_Rulers[i].m_Topmost >= 0);
            redraw = true;
        }
    }

    SetCaretPosition();

    if (redraw)
    {
        Invalidate(FALSE);
        AdjustVertScroller();
        AdjustHorzScroller();
        AdjustVertSibling();
        AdjustHorzSibling();
    }
}

void COEditorView::AdjustVertSibling ()
{
    if (m_Rulers[1].m_sibling && m_Rulers[1].m_sibling->m_hWnd
    && m_Rulers[1].m_sibling->m_Rulers[1].m_Topmost != m_Rulers[1].m_Topmost)
    {
        m_Rulers[1].m_sibling->m_Rulers[1].m_Topmost = m_Rulers[1].m_Topmost;
        m_Rulers[1].m_sibling->Invalidate(FALSE);
    }
}

void COEditorView::AdjustHorzSibling ()
{
    if (m_Rulers[0].m_sibling && m_Rulers[0].m_sibling->m_hWnd
    && m_Rulers[0].m_sibling->m_Rulers[0].m_Topmost != m_Rulers[0].m_Topmost)
    {
        m_Rulers[0].m_sibling->m_Rulers[0].m_Topmost = m_Rulers[0].m_Topmost;
        m_Rulers[0].m_sibling->Invalidate(FALSE);
    }
}

void COEditorView::AdjustVertScroller ()
{
    SCROLLINFO scrollInfo;
    scrollInfo.cbSize    = sizeof SCROLLINFO;
    scrollInfo.fMask     = SIF_POS|SIF_RANGE|SIF_PAGE;
    scrollInfo.nMin      = 0;
    scrollInfo.nMax      = max(m_Rulers[1].m_Topmost + m_Rulers[1].m_Count, GetLineCount()) - 1;
    scrollInfo.nPage     = m_Rulers[1].m_Count;
    scrollInfo.nPos      = m_Rulers[1].m_Topmost;
    scrollInfo.nTrackPos = 0;

    SetScrollInfo(SB_VERT, &scrollInfo, TRUE);
}

void COEditorView::AdjustHorzScroller ()
{
    SCROLLINFO scrollInfo;
    scrollInfo.cbSize    = sizeof SCROLLINFO;
    scrollInfo.fMask     = SIF_POS|SIF_RANGE|SIF_PAGE;
    scrollInfo.nMin      = 0;
    scrollInfo.nMax      = max(m_Rulers[0].m_Topmost + m_Rulers[0].m_Count, m_nMaxLineLength) - 1;
    scrollInfo.nPage     = m_Rulers[0].m_Count;
    scrollInfo.nPos      = m_Rulers[0].m_Topmost;
    scrollInfo.nTrackPos = 0;

    SetScrollInfo(SB_HORZ, &scrollInfo, TRUE);
}

void COEditorView::DoSize (UINT nType, int cx, int cy)
{
    if (m_bAttached && cx > 0 && cy > 0
    && (nType == SIZE_MAXIMIZED || nType == SIZE_RESTORED))
    {
        m_Rulers[0].m_ClientSize = (cx - m_Rulers[0].m_Indent);
        m_Rulers[1].m_ClientSize = cy;

		m_Rulers[0].m_Count = 0;
		m_Rulers[1].m_Count = 0;
		if(m_Rulers[0].m_CharSize)
		{
	        m_Rulers[0].m_Count = m_Rulers[0].m_ClientSize / m_Rulers[0].m_CharSize;
		}
		if(m_Rulers[1].m_CharSize)
		{
			m_Rulers[1].m_Count = m_Rulers[1].m_ClientSize / m_Rulers[1].m_CharSize;
		}
        AdjustVertScroller();
        AdjustHorzScroller();
    }
}

void COEditorView::SelectByMouse (CPoint point)
{
    if (m_nMouseSelStartLine >= 0)
    {
        Square selection;
        GetSelection(selection);
        selection.end = GetPosition();

        Position pos = MouseToPos(point);
        pos.column = 0;

        MoveTo(pos);

        selection.start.column = 0;
        if (selection.end.line < m_nMouseSelStartLine)
            selection.start.line = m_nMouseSelStartLine + 1;
        else
        {
            selection.start.line = m_nMouseSelStartLine;
            selection.end.line++;
        }

        SetSelection(selection);

        m_bMouseSelectionStart = FALSE;
    }
    else
    {
        Square selection;
        GetSelection(selection);
        Position pos = MouseToPos(point, GetBlockMode() == ebtColumn);

        if (m_bMouseSelectionStart)
        {
            selection.start = selection.end = pos;
            m_bMouseSelectionStart = FALSE;
        }

        MoveTo(pos, (GetBlockMode() == ebtColumn));
        selection.end = GetPosition();

        SetSelection(selection);
    }
}

OpenEditor::Position
COEditorView::MouseToPos (CPoint point, bool force) const
{
    Position pos;
    pos.column = (point.x - m_Rulers[0].m_Indent + m_Rulers[0].m_CharSize / 2)
                 / m_Rulers[0].m_CharSize
                 + m_Rulers[0].m_Topmost;

    if (pos.column < 0) pos.column = 0;

    pos.line   = point.y / m_Rulers[1].m_CharSize
                 + m_Rulers[1].m_Topmost;

    if (pos.line < 0) pos.line = 0;

    AdjustPosition(pos, force);

    return pos;
}

void COEditorView::InvalidateLines (int start, int end)
{
    if (start > end)
        std::swap(start, end);
    RECT rc;
    rc.left   = 0;
    rc.right  = m_Rulers[0].m_Indent + m_Rulers[0].m_ClientSize;
    rc.top    = m_Rulers[1].PosToPix(start);
    rc.bottom = m_Rulers[1].PosToPix(end+1);
    InvalidateRect(&rc);
}

void COEditorView::InvalidateSquare (const OpenEditor::Square& sqr)
{
    CRect rc;
    rc.left   = m_Rulers[0].PosToPix(min(sqr.start.column, sqr.end.column));
    rc.right  = m_Rulers[0].PosToPix(max(sqr.end.column, sqr.start.column) + 1);
    rc.top    = m_Rulers[1].PosToPix(min(sqr.start.line, sqr.end.line));
    rc.bottom = m_Rulers[1].PosToPix(max(sqr.end.line, sqr.start.line) + 1);
    InvalidateRect(&rc);
}

void COEditorView::OnChangedLinesQuantity (int line, int quantity)
{
    if (quantity < 0) // line deleting
    {
        // invalidate lines between old & new EOF mark
        int lines = GetLineCount();
        InvalidateLines(lines, lines - quantity);

        if (line <= m_Rulers[1].m_Topmost // the first deleted line is above viewport
        && line - quantity >= m_Rulers[1].m_Topmost + m_Rulers[1].m_Count 
            - (m_Rulers[1].m_ClientSize % m_Rulers[1].m_CharSize ? 1 : 0)) // the last deleted line is below viewport
        {
            // move deleted lines position in the center of viewport
            m_Rulers[1].m_Topmost = line - m_Rulers[1].m_Count/2;
            if (m_Rulers[1].m_Topmost < 0) m_Rulers[1].m_Topmost = 0;

            InvalidateLines(m_Rulers[1].m_Topmost, m_Rulers[1].m_Topmost + m_Rulers[1].m_Count);
        }
        else if (m_Rulers[1].m_Topmost >= line) // the line is above the topmost visible line
        {
                if (m_Rulers[1].m_Topmost >= line - quantity) // all deleted lines are above topmost
                {
                    m_Rulers[1].m_Topmost += quantity;
                    if (m_Rulers[1].m_Topmost < 0) 
                        m_Rulers[1].m_Topmost = 0;
                    
                    // repainting is not required
                }
                else if (m_Rulers[1].m_Topmost + m_Rulers[1].m_Count > line - quantity) // the last deleted line is visible
                {
                    // the next line on the screen should save its position
                    m_Rulers[1].m_Topmost += quantity - 1;
                    if (m_Rulers[1].m_Topmost < 0) 
                        m_Rulers[1].m_Topmost = 0;

                    InvalidateLines(m_Rulers[1].m_Topmost, m_Rulers[1].m_Topmost + m_Rulers[1].m_Count);
                }
        }

        // adjust the current line
        int curLine = GetCursorLine();
        if (curLine >= line - quantity) // the current line is below the last deleted line
        {
            curLine += quantity;
            if (curLine < 0) curLine = 0;
        }
        else if (curLine > line && curLine < line - quantity) // the current line is in the deleted lines
            curLine = line;
        
        moveCurrentLine(curLine);
    }
    else // inserting
    {
        if (line < m_Rulers[1].m_Topmost) // the first deleted line is above viewport
        {
            m_Rulers[1].m_Topmost += quantity;
            // repainting is not required
        }
        else if (line < m_Rulers[1].m_Topmost + m_Rulers[1].m_Count) // the first deleted line is above of viewport bottom
        {
            // just repainting
            InvalidateLines(line, m_Rulers[1].m_Topmost + m_Rulers[1].m_Count);
        }

        // adjust the cursor line
        int curLine = GetCursorLine();
        if (line < curLine)
        {
            curLine += quantity;
            moveCurrentLine(curLine);
        }
    }
    
    ClearSelection(); // either clear or recalculate selection
    AdjustVertScroller();

    if (GetSettings().GetLineNumbers())
    {
        int digits = CalculateDigitsInLatsLineNumber();
        if (m_nDigitsInLatsLineNumber != digits)
        {
            m_nDigitsInLatsLineNumber = digits;

            int leftMargin = CalculateLeftMargin();
            if (m_Rulers[0].m_Indent != leftMargin)
            {
                m_Rulers[0].m_Indent = leftMargin;
                AdjustCaretPosition(); // 17.03.2003 bug fix, cursor position become wrong after document type was changed
                Invalidate(FALSE);
            }
        }
    }

    m_curCharCache.Reset();
}

void COEditorView::OnChangedLines (int startLine, int endLine)
{
    if ( ((startLine <= m_Rulers[1].m_Topmost) && (m_Rulers[1].m_Topmost <= endLine))
       || (m_Rulers[1].m_Topmost <= startLine) && (startLine <= (m_Rulers[1].m_Topmost + m_Rulers[1].m_Count))
       )
    {
        InvalidateLines(startLine, endLine);
    }
    AdjustHorzScroller();

    m_curCharCache.Reset();
}

void COEditorView::OnChangedLinesStatus (int, int)
{
}
