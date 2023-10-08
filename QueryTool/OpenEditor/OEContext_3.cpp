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

// 22.07.2002 bug fix, stream block delete fails below EOF
// 03.08.2002 bug fix, block copy is too slow on 10M file
// 14.08.2002 bug fix, indent/undent block does not work if selection is not normalized
// 17.08.2002 bug fix, block copy fails below EOF
// 07.09.2002 bug fix, copy/paste operation loses the first blank line
// 17.09.2002 performance tuning, insert/delete block operation
// 13.01.2003 bug fix, useless selection on delete word undo
// 07.03.2003 bug fix, wrong columnar selection after undo
// 16.03.2003 bug fix, autoindent always uses a previos line as base even the line is empty
// 16.03.2003 bug fix, scroll position fails on delete boolmark lines operation
// 26.05.2003 bug fix, selection may still be out of visible text after undo
// 03.06.2003 bug fix, sql find match fails on select/insert/... if there is no ending ';'

#include "pch.h"
#include <algorithm>
#include <strstream>
#include <COMMON/AppGlobal.h>
#include <COMMON/ExceptionHelper.h>
#include "OEContext.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define _CHECK_ALL_PTR_ _CHECK_AND_THROW_(m_pStorage, _T("Editor has not been initialized properly!"))

namespace OpenEditor
{
    using namespace std;


    void space4insertion (CString& buff, int from, int to, bool use_tab, int tab_spacing)
    {
        _ASSERTE(to - from >= 0);

        buff.Empty();

        int cur_tabs = from / tab_spacing;
        int new_tabs = to / tab_spacing;

        if (use_tab)
        {
            int spaces   = to - new_tabs * tab_spacing;
            int tabs     = new_tabs - cur_tabs;
            buff.Append(_T("\t"),tabs);
            buff.Append(_T(" "),spaces);

        }
        else
        {
          buff.Append(_T(" "),(to - from));
        }
    }


void EditContext::PushInUndoStack (Position pos)
{
    pos.column = pos2inx(pos.line, pos.column, true); // 07.03.2003 bug fix, wrong columnar selection after undo
    m_pStorage->PushInUndoStack(pos);
}

void EditContext::PushInUndoStack (EBlockMode mode, Square sel)
{
    sel.start.column = pos2inx(sel.start.line, sel.start.column, true); // 07.03.2003 bug fix, wrong columnar selection after undo
    sel.end.column = pos2inx(sel.end.line, sel.end.column, true);
    m_pStorage->PushInUndoStack(mode, sel);
}

void EditContext::expandVirtualSpace (int line, int column)
{
    m_pStorage->ExpandLinesTo(line);

    int length = GetLineLength(line);
    if (column > length)
    {
        CString buff;
        space4insertion(buff, length, column, !GetTabExpand(), GetTabSpacing());
        InsertLinePart(line, length, buff.GetString(), (int)buff.GetLength());
    }
}

// string mustn't have '\r' or '\n'
void EditContext::InsertLine (int line, LPCTSTR str, int len)
{
    if (line)
        m_pStorage->ExpandLinesTo(line - 1);

    m_pStorage->InsertLine(line, str, len);
}

// string mustn't have '\r' or '\n'
void EditContext::InsertLinePart (int line, int column, LPCTSTR str, int len)
{
    expandVirtualSpace(line, column);
    m_pStorage->InsertLinePart(line, !column ? 0 : pos2inx(line, column), str, len);
}

void EditContext::DeleteLinePart (int line, int from, int to)
{
    if (line < GetLineCount())
    {
        m_pStorage->DeleteLinePart(line, !from ? 0 : pos2inx(line, from), !to   ? 0 : pos2inx(line, to));
    }
}

void EditContext::SplitLine (int line, int pos)
{
    if (line < GetLineCount())
    {
        m_pStorage->SplitLine(line, !pos ? 0 : pos2inx(line, pos, true));
    }
}

void EditContext::DoIndent (bool moveOnly)
{
    UndoGroup undoGroup(*this);

    Position pos   = GetPosition();
    int indentSize = GetIndentSpacing();
    int newColumn  = (pos.column / indentSize + 1) * indentSize;

    if (!moveOnly && (!GetCursorBeyondEOL()
    || (pos.line < GetLineCount() && pos.column < GetLineLength(pos.line))))
    {
        CString buff;
        space4insertion(buff, pos.column, newColumn, !GetTabExpand(), GetTabSpacing());
        InsertLinePart(pos.line, pos.column, buff.GetString(),(int) buff.GetLength());
    }
    pos.column = newColumn;

    MoveTo(pos);
}

void EditContext::DoUndent ()
{
    int indentSize = GetIndentSpacing();
    Position pos   = GetPosition();
    pos.column = max(0, (pos.column / indentSize
                         - ((pos.column % indentSize) ? 0 : 1)) * indentSize);
    MoveTo(pos);
}

void EditContext::DoCarriageReturn ()
{
    UndoGroup undoGroup(*this);

    int column;

    if (m_curPos.line < GetLineCount())
        //column = m_curPos.column;
        column = min(m_curPos.column, GetLineLength(m_curPos.line));
    else
        column = 0;

    expandVirtualSpace(m_curPos.line, column);
    m_pStorage->Insert('\r', m_curPos.line, !column ? 0 : pos2inx(m_curPos.line, column));

    if (GoToDown(true) && GoToStart(true))
    {
        switch (GetIndentType())
        {
        case eiAuto:
            if (m_curPos.line > 0 && m_curPos.line < GetLineCount())
            {
                int len = 0;
                LPCTSTR str = 0;
                Position pos = GetPosition();

                // 16.03.2003 bug fix, auto indent always uses a previous line as base even the line is empty
                for (int baseLine = pos.line - 1; baseLine >= 0 && len == 0; baseLine--)
                    GetLine(baseLine, str, len);

                int i;
                for (i = 0; i < len && isspace(str[i]); i++);

                pos.column = inx2pos(str, len, i);

                if (!(GetCursorBeyondEOL() && GetLineLength(pos.line) == 0))
                {
                    CString buff;
                    space4insertion(buff, 0, pos.column, !GetTabExpand(), GetTabSpacing());
                    InsertLinePart(pos.line, 0, buff.GetString(),(int) buff.GetLength());
                }

                MoveTo(pos, true);
            }
            break;
        default:; // eiNone & others
        }
    }
    PushInUndoStack(m_curPos);
}

void EditContext::Insert (TCHAR chr)
{
    _CHECK_ALL_PTR_

    switch (chr)
    {
    case '\r':
        DoCarriageReturn();
        break;
    case '\t':
        DoIndent();
        break;
    default:
        {
            UndoGroup undoGroup(*this);

            expandVirtualSpace(m_curPos.line, m_curPos.column);
            m_pStorage->Insert(chr, m_curPos.line, pos2inx(m_curPos.line, m_curPos.column));
            GoToRight(true);
        }
    }
}

void EditContext::Backspace ()
{
    _CHECK_ALL_PTR_

    int nlines = GetLineCount();
    int length = (m_curPos.line < nlines) ? GetLineLength(m_curPos.line) : 0;

    if (m_curPos.line < nlines && m_curPos.column <= length)
    {
        UndoGroup undoGroup(*this);

        if (!m_curPos.column)
        {
            if (GoToLeft())
            {
                int inx = pos2inx(m_curPos.line, m_curPos.column, true);
                m_pStorage->Delete(m_curPos.line, inx);
            }
            return;
        }

        int inx = pos2inx(m_curPos.line, m_curPos.column);

        TCHAR chr(0);
        try
        {
            chr = m_pStorage->GetChar(m_curPos.line, inx-1);
            m_pStorage->Delete(m_curPos.line, inx-1);
        }
		catch (std::out_of_range&) {}

        if (chr == '\t')
        {
            m_curPos.column =
            m_orgHrzPos     = inx2pos(m_curPos.line, inx-1);
        }
        else
            GoToLeft();
    }
    else
    {
        if (m_curPos.column > 0)
        {
            int indentSize = GetIndentSpacing();
            int lastIndent = (length / indentSize + 1) * indentSize;

            if (m_curPos.column <= lastIndent)
                GoToEnd();
            else
                DoUndent();
        }
        else
            GoToLeft();
    }
}

void EditContext::Overwrite (TCHAR chr)
{
    _CHECK_ALL_PTR_

    UndoGroup undoGroup(*this);

    switch (chr)
    {
    case '\r':
        if (GoToDown()) GoToStart();
        break;
    case '\t':
        DoIndent();
        break;
    default:
        expandVirtualSpace(m_curPos.line, m_curPos.column + 1);
        m_pStorage->Overwrite(chr, m_curPos.line, pos2inx(m_curPos.line, m_curPos.column));
        GoToRight(true);
        break;
    }
}

void EditContext::Delete ()
{
    _CHECK_ALL_PTR_

    if (m_curPos.line < GetLineCount())
    {
        UndoGroup undoGroup(*this);

        expandVirtualSpace(m_curPos.line, m_curPos.column);
        m_pStorage->Delete(m_curPos.line, pos2inx(m_curPos.line, m_curPos.column));
    }
}

void EditContext::DeleteLine ()
{
    if (m_curPos.line < GetLineCount())
    {
        UndoGroup undoGroup(*this);

        // save pos because it can be changed by notification handler
        Position pos = GetPosition();
        m_pStorage->DeleteLine(pos.line);
        MoveTo(pos);

        AdjustCaretPosition();
        ClearSelection();
    }
}

void EditContext::DeleteWordToLeft ()
{
    Square blkPos;
    blkPos.start = m_curPos;
    blkPos.end   = wordLeft(false);

    if (!blkPos.is_empty())
    {
        EBlockMode blkMode = GetBlockMode();
        SetBlockMode(ebtStream);
        SetSelection(blkPos);
        DeleteBlock(false);
        SetBlockMode(blkMode);
        AdjustCaretPosition();
    }
}

void EditContext::DeleteWordToRight ()
{
    Square blkPos;
    blkPos.start = wordRight(false);
    blkPos.end   = m_curPos;

    if (!blkPos.is_empty())
    {
        EBlockMode blkMode = GetBlockMode();
        SetBlockMode(ebtStream);
        SetSelection(blkPos);
        DeleteBlock(false);
        SetBlockMode(blkMode);
        AdjustCaretPosition();
    }
}

void EditContext::Undo ()
{
    _CHECK_ALL_PTR_

    UndoContext cxt(*m_pStorage);

    if (m_pStorage->Undo(cxt))
    {
        if (cxt.m_position.line < GetLineCount())
            cxt.m_position.column = inx2pos(cxt.m_position.line, cxt.m_position.column);
        // 26.06.2003 bug fix, selection may still be out of visible text after undo
        MoveToAndCenter(cxt.m_position);

        if (cxt.m_selMode != ebtUndefined)
        {
            if (cxt.m_selection.start.line < GetLineCount())
                cxt.m_selection.start.column = inx2pos(cxt.m_selection.start.line, cxt.m_selection.start.column);
            if (cxt.m_selection.end.line < GetLineCount())
                cxt.m_selection.end.column = inx2pos(cxt.m_selection.end.line, cxt.m_selection.end.column);
            SetBlockMode(cxt.m_selMode);
            SetSelection(cxt.m_selection);
        }
        else
            ClearSelection();

        //if (cxt.m_position.line < GetLineCount())
        //    cxt.m_position.column = inx2pos(cxt.m_position.line, cxt.m_position.column);
        //MoveTo(cxt.m_position);
    }
}

void EditContext::Redo ()
{
    _CHECK_ALL_PTR_

    UndoContext cxt(*m_pStorage);

    if (m_pStorage->Redo(cxt))
    {
        if (cxt.m_selMode != ebtUndefined)
        {
            if (cxt.m_selection.start.line < GetLineCount())
                cxt.m_selection.start.column = inx2pos(cxt.m_selection.start.line, cxt.m_selection.start.column);
            if (cxt.m_selection.end.line < GetLineCount())
                cxt.m_selection.end.column = inx2pos(cxt.m_selection.end.line, cxt.m_selection.end.column);
            SetBlockMode(cxt.m_selMode);
            SetSelection(cxt.m_selection);
        }
        else
            ClearSelection();

        if (cxt.m_position.line < GetLineCount())
            cxt.m_position.column = inx2pos(cxt.m_position.line, cxt.m_position.column);
        MoveTo(cxt.m_position);
    }
}

void EditContext::ClearSelection ()
{
    if (!m_blkPos.is_empty())
    {
        if (m_BlockMode == ebtColumn)
            InvalidateSquare(m_blkPos);
        else
            InvalidateLines(m_blkPos.start.line, m_blkPos.end.line);

        m_blkPos.start = m_blkPos.end;

        if (m_bDirtyPosition)
            adjustCursorPosition();
    }
}

void EditContext::ConvertSelectionToLines ()
{
    if (GetBlockMode() != ebtStream)
        SetBlockMode(ebtStream);

    Square pos;
    GetSelection(pos);

    if (!pos.is_empty())
    {
        pos.normalize();
        pos.normalize();

        bool reselect = false;

        if (pos.start.column > 0)
        {
            reselect = true;
            pos.start.column = 0;
        }

        if (pos.end.column > 0 && GetLineLength(pos.end.line))
        {
            reselect = true;
            pos.end.line++;
            pos.end.column = 0;
        }

        if (reselect)
        {
            SetSelection(pos);
            MoveTo(pos.end, true);
        }
    }
}

void EditContext::SetSelection (const Square& blkPos)
{
    if (m_blkPos != blkPos)
    {
        ClearSelection();

        m_blkPos = blkPos;

        if (GetBlockMode() == ebtColumn)
            InvalidateSquare(m_blkPos);
        else
            InvalidateLines(m_blkPos.start.line, m_blkPos.end.line);
    }
}

void EditContext::SelectAll()
{
    if (GetLineCount())
    {
        Square selection;
        selection.start.line   = 0;
        selection.end.line     = GetLineCount() - 1;
        selection.start.column = 0;
        selection.end.column   = GetLineLength(selection.end.line);

        SetBlockMode(ebtStream);
        MoveTo(selection.end);
        SetSelection(selection);

        InvalidateLines(selection.start.line, selection.end.line);
    }
}

void EditContext::SelectLine (int line)
{
    ClearSelection();
    if (GetCursorBeyondEOF() || line < GetLineCount())
    {
        Square selection;
        selection.start.line   = line;
        selection.end.line     = line + 1;
        selection.start.column = 0;
        selection.end.column   = 0;

        SetBlockMode(ebtStream);
        MoveTo(selection.end);
        SetSelection(selection);

        InvalidateLines(selection.start.line, selection.end.line);
    }
}

void EditContext::SelectByCursor (Position pos)
{
    if (m_blkPos.is_empty())
        m_blkPos.start = m_blkPos.end = pos;
    else if (m_BlockMode == ebtColumn)
        InvalidateSquare(m_blkPos);

    int old_end_line = m_blkPos.end.line;

    // it's necessary to add checks here
    m_blkPos.end = GetPosition();

    if (m_BlockMode == ebtColumn)
        InvalidateSquare(m_blkPos);
    else
        InvalidateLines(old_end_line, m_blkPos.end.line);
}

void EditContext::GetBlock (CString& str, const Square* sqr) const
{
    _CHECK_ALL_PTR_

    Square pos;

    if (sqr)
        pos = *sqr;
    else
        GetSelection(pos);

    if (!pos.is_empty())
    {
        pos.normalize();

        int niLength;
        LPCTSTR pszLine;
        int nLines = GetLineCount();

        switch (GetBlockMode())
        {
        case ebtStream:
            {
                int reserve = 0;
                for (int i(pos.start.line); i <= pos.end.line; i++)
                    // 03/08/2002 bug fix, block copy is too slow on 10M file
                    // 17/08/2002 bug fix, block copy fails below EOF
                    if (i < nLines) 
                        reserve += m_pStorage->GetLineLength(i) + (sizeof("\r\n")-1);
                    else
                        break;

                str.Preallocate(reserve);

                if (pos.start.line == pos.end.line)
                {
                    line2buff(pos.start.line, pos.start.column, pos.end.column, str);
                }
                else
                {
                    line2buff(pos.start.line, pos.start.column, INT_MAX, str);
                    for (int i(pos.start.line + 1); i <= pos.end.line - 1; i++)
                    {
                        str += _T("\r\n");
                        if (i < nLines)
                        {
                            GetLine(i, pszLine, niLength);
                            str.Append(pszLine, niLength);
                        }
                    }
                    str += _T("\r\n");
                    line2buff(pos.end.line, 0, pos.end.column, str);
                }
                TRACE("Copy Block reserved = %d, actual length = %d\n",  reserve, str.GetLength());

                if ((pos.end.line < nLines && pos.end.column > GetLineLength(pos.end.line))
                 || (pos.end.line >= nLines && pos.end.column > 0))
                {
                  str += "\r\n";
                }
            }
            break;
        case ebtColumn:
            {
                if ((pos.end.column - pos.start.column) < 0)
                    swap(pos.end.column, pos.start.column);

                str.Preallocate((pos.end.line - pos.start.line)
                               *(pos.end.column - pos.start.column));

                for (int i(pos.start.line); i <= pos.end.line; i++)
                {
                  if(i > pos.start.line)
                  {
                    str += _T("\r\n");
                  }
                  line2buff(i, pos.start.column, pos.end.column, str, true);
                }
            }
            break;
        }
    }
}

void EditContext::InsertBlock (LPCTSTR str)
{
    InsertBlock(str, !GetBlockKeepMarking());
}

void EditContext::InsertBlock (LPCTSTR str, bool hideSelection, bool putSelInUndo)
{
    _CHECK_ALL_PTR_

    CString buff;
    CString io(str);
    bool with_CR;
    Position orgPos = m_curPos;

    UndoGroup undoGroup(*this);
    PushInUndoStack(m_curPos);
    Square sel;
    GetSelection(sel);
    if (putSelInUndo)
        PushInUndoStack(GetBlockMode(), sel);

    switch (GetBlockMode())
    {
    case ebtStream:
        {
            if (getLine(io, buff, with_CR))
            {
                if (with_CR) 
                    SplitLine(m_curPos.line, m_curPos.column); // multiline selection

                InsertLinePart(m_curPos.line, m_curPos.column, buff.GetString(), (int)buff.GetLength());

                if (with_CR) // multiline selection
                {
                  StringArray lines(1024,16 * 1024);

                    m_curPos.column = 0;
                    while (getLine(io, buff, with_CR))
                    {
                      if(with_CR)
                      {
                        lines.append().assign(buff.GetString(), buff.GetLength(), true);
                      }
                      else
                      {
                          InsertLinePart(m_curPos.line+1, 0, buff.GetString(), (int)buff.GetLength());
                          m_curPos.column = inx2pos(buff.GetString(), (int)buff.GetLength(),(int) buff.GetLength());
                      }
                    }

                    if (lines.size())
                    {
                      m_pStorage->InsertLines(m_curPos.line+1, lines);
                    }
                    m_curPos.line += (int)lines.size() + 1;
                }
                else
                {
                  m_curPos.column = inx2pos(m_curPos.line,pos2inx(m_curPos.line,m_curPos.column) + (int) buff.GetLength());
                }
                MoveTo(m_curPos);
            }
        }
        break;
    case ebtColumn:
        {
            int lastLineLen = 0;
            int line = m_curPos.line;

            Storage::NotificationDisabler disabler(m_pStorage);

            while (getLine(io, buff, with_CR))
            {
                InsertLinePart(line, m_curPos.column, buff.GetString(),(int) buff.GetLength());
                line++;
                lastLineLen = (int)buff.GetLength();
            }
            orgPos.line  += line - m_curPos.line - 1;
            orgPos.column = inx2pos(orgPos.line, pos2inx(orgPos.line, m_curPos.column + lastLineLen));

            disabler.Enable();
        }
        break;
    }

    switch (GetBlockMode())
    {
    case ebtStream:
        sel.start = orgPos;
        sel.end   = m_curPos;
        SetSelection(sel);
        break;
    case ebtColumn:
        sel.start = m_curPos;
        sel.end   = orgPos;
        SetSelection(sel);
        if (GetColBlockCursorToStartAfterPaste())
            MoveTo(orgPos);
        break;
    }

    if (putSelInUndo)
    {
        GetSelection(sel);
        PushInUndoStack(GetBlockMode(), sel);
    }

    if (hideSelection)
        ClearSelection();

    AdjustCaretPosition();
    PushInUndoStack(m_curPos);
}

void EditContext::DeleteBlock (bool putSelInUndo)
{
    _CHECK_ALL_PTR_

    Square sel;
    GetSelection(sel);
    sel.normalize();
    int nLines = GetLineCount();

    if (sel.start.line < nLines 
    && (sel.start.line != nLines-1 || sel.start.column < GetLineLength(nLines-1))
    && !sel.is_empty())
    {
        UndoGroup undoGroup(*this);
        PushInUndoStack(m_curPos);
        if (putSelInUndo) 
            PushInUndoStack(GetBlockMode(), sel);

        switch (GetBlockMode())
        {
        case ebtStream:
            {
                // for single-line selection
                if (sel.start.line == sel.end.line)
                {
                    DeleteLinePart(sel.start.line, sel.start.column, sel.end.column);
                }
                else if (sel.start.line < nLines) // 22/07/2002 bug fix, stream block delete fails below EOF
                {
                    // delete selection, skip first and last lines
                    if (sel.end.line-sel.start.line > 1)
                        m_pStorage->DeleteLines(sel.start.line + 1, 
                            sel.end.line - sel.start.line - 1);

                    DeleteLinePart(sel.start.line, sel.start.column, USHRT_MAX);
                    expandVirtualSpace(sel.start.line, sel.start.column);
                    DeleteLinePart(sel.start.line + 1, 0, sel.end.column);
                    m_pStorage->MergeLines(sel.start.line);
                }

                MoveTo(sel.start);
            }
            break;
        case ebtColumn:
            {
                sel.normalize_columns();

                Storage::NotificationDisabler disabler(m_pStorage);

                bool deleteSpace = GetColBlockDeleteSpaceAfterMove();
                CString space;

                if(!deleteSpace)
                {
                  if(sel.end.column > sel.start.column)
                  {
                    space.Truncate(sel.end.column - sel.start.column);
                  }
                  else
                  {
                    space.Append(_T(" "),sel.start.column - sel.end.column);
                  }
//                   space.resize(sel.end.column > sel.start.column
//                                ? sel.end.column - sel.start.column
//                                : sel.start.column - sel.end.column,' ');
                }
                for (int i(sel.start.line); i <= sel.end.line && i < nLines; i++)
                {
                    DeleteLinePart(i, sel.start.column, sel.end.column);

                    if(!deleteSpace)
                    {
                      InsertLinePart(i,sel.start.column,space.GetString(),(int) space.GetLength());
                    }
                }

                disabler.Enable();

                Position pos;
                pos.line = m_curPos.line;
                pos.column = sel.start.column;
                MoveTo(pos);
            }
            break;
        }

        ClearSelection();
        AdjustCaretPosition();
        
        if (putSelInUndo)
        {
            GetSelection(sel); // save empty selection in undo stack
            PushInUndoStack(GetBlockMode(), sel);
        }
        PushInUndoStack(m_curPos);
    }
    else if (!sel.is_empty())
    {
        ClearSelection();
        MoveTo(sel.start);
    }
}

void EditContext::IndentBlock ()
{
    _CHECK_ALL_PTR_

    Square sel;
    GetSelection(sel);
    sel.normalize(); // 14/08/2002 bug fix, indent/unintended block does not work if selection is not normalized

    if (!sel.is_empty())
    {
        UndoGroup undoGroup(*this);
        PushInUndoStack(m_curPos);
        PushInUndoStack(GetBlockMode(), sel);

        int nLines = GetLineCount();
        int indentSize = GetIndentSpacing();

        ConvertSelectionToLines();

        CString buff;
        space4insertion(buff, 0, indentSize, !GetTabExpand(), GetTabSpacing());

        Storage::NotificationDisabler disabler(m_pStorage);

        for (int i = sel.start.line; i < sel.end.line && i < nLines; i++)
        {
          if(m_pStorage->GetLineLength(i))
          {
            InsertLinePart(i,0,buff.GetString(),(int) buff.GetLength());
          }
        }
        disabler.Enable();
        PushInUndoStack(m_curPos);
    }
}

void EditContext::UndentBlock ()
{
    _CHECK_ALL_PTR_

    Square sel;
    GetSelection(sel);
    sel.normalize(); // 14/08/2002 bug fix, indent/unindented block does not work if selection is not normalized

    if (!sel.is_empty())
    {
        UndoGroup undoGroup(*this);
        PushInUndoStack(m_curPos);
        PushInUndoStack(GetBlockMode(), sel);

        int nLines = GetLineCount();
        int indentSize = GetIndentSpacing();

        ConvertSelectionToLines();

        CString buff;
        bool    tabExpand  = GetTabExpand(); 
        int     tabSpacing = GetTabSpacing();
        Storage::NotificationDisabler disabler(m_pStorage);

        for (int i = sel.start.line; i < sel.end.line && i < nLines; i++)
        {
            int len;
            LPCTSTR str;
            GetLine(i, str, len);

            if (len)
            {
                int j;
                for (j = 0; isspace(str[j]) && j < len; j++);

                int pos = inx2pos(str, len, j);

                if (pos > 0)
                    DeleteLinePart(i, 0, pos);

                if (pos > indentSize)
                {
                    space4insertion(buff, 0, pos - indentSize, !tabExpand, tabSpacing);
                    InsertLinePart(i, 0, buff.GetString(), buff.GetLength());
                }
            }
        }
        disabler.Enable();
        PushInUndoStack(m_curPos);
    }
}

void EditContext::CopyBookmarkedLines (CString& buff) const
{
    _CHECK_ALL_PTR_

    vector<int> lines;
    int reserve = 0;
    
    m_pStorage->GetBookmarkedLines(lines, eBmkGroup1);

    vector<int>::const_iterator it = lines.begin();
    for (; it != lines.end(); it++)
        reserve += m_pStorage->GetLineLength(*it) + (sizeof("\r\n")-1);
    
    if (reserve)
    {
        buff.Preallocate(reserve);
        it = lines.begin();
        for (; it != lines.end(); it++)
        {
            int len;
            LPCTSTR str;
            GetLine(*it, str, len);
            buff.Append(str, len);
            buff += "\r\n";
        }
    }
}

void EditContext::DeleteBookmarkedLines ()
{
    _CHECK_ALL_PTR_

    vector<int> lines;
    m_pStorage->GetBookmarkedLines(lines, eBmkGroup1);
    
    UndoGroup undoGroup(*this);
    Storage::NotificationDisabler disabler(m_pStorage);

    vector<int>::reverse_iterator it = lines.rbegin();
    for (; it != lines.rend(); it++)
        m_pStorage->DeleteLine(*it);
    
    if (lines.size())
    {
        disabler.Enable();
        // Undo/Redo operation does not support this notification :(
        m_pStorage->Notify_ChangedLinesQuantity(lines[0], -(int)lines.size()); // 16.03.2003 bug fix, scroll position fails on delete boolmark lines operation
        AdjustCaretPosition();
        ClearSelection();
    }
}

bool EditContext::ExpandTemplate (int index)
{
    _CHECK_ALL_PTR_

    Square sqr;
    CString buff;
    if (index != -1 || GetBlockOrWordUnderCursor(buff, sqr, false))
    {
        const TemplatePtr tmpl = GetSettings().GetTemplate(); 
        
        Position pos;
        CString text;

        bool expand = false;
        
        if (index != -1)
        {
            expand = tmpl->ExpandKeyword(index, text, pos);
            GetSelection(sqr);
        }
        else
        {
            expand = tmpl->ExpandKeyword(buff, text, pos);
        }

        if (expand)
        {
            EBlockMode orgBlockMode = GetBlockMode();
            if (orgBlockMode != ebtStream)
                SetBlockMode(ebtStream);

            UndoGroup undoGroup(*this);
            SetSelection(sqr);
            DeleteBlock(false);

            bool with_CR;
            CString io(text.GetString());
            CString line;
            CString result;
            CString indent('\n');
            indent.Append(_T(" "),sqr.start.column);

            if (getLine(io, line, with_CR))
                result += line;

            while (getLine(io, line, with_CR))
            {
              if(!line.IsEmpty())
              {
                result += indent + line;
              }
              else if(with_CR)
              {
                result += '\n';
              }
            }

            InsertBlock(result.GetString(), false, false);

            if (!GetTabExpand()) 
                ScanAndReplaceText(TabifyLeadingSpaces, false);

            SetBlockMode(orgBlockMode);
            ClearSelection();

            pos.line += sqr.start.line;
            pos.column += sqr.start.column;

            MoveTo(pos);
            PushInUndoStack(pos);

            //GetSelection(sqr);
            //PushInUndoStack(orgBlockMode, sqr);

            return true;
        }
    }
    return false;
}

void EditContext::FindMatch (bool select)
{
    _CHECK_ALL_PTR_
    
    LanguageSupportPtr ls = m_pStorage->GetLanguageSupport();

    if (ls.get())
    {
        int line = m_curPos.line, 
            offset = pos2inx(m_curPos.line, m_curPos.column);

        LanguageSupport::Match match;
        ls->FindMatch(line, offset, match);

        if (offset > 0 && (!match.found || match.partial))
        {
            LanguageSupport::Match match2;
            ls->FindMatch(line, offset-1, match2); // step back and try again
            
            if (match2.found && !match2.partial)
                match = match2;
        }

        if (match.found)
        {
            Position pos;
            pos.line   = match.line[1];
            pos.column = (match.line[1] < m_pStorage->GetLineCount()) ? inx2pos(match.line[1], match.offset[1]) : match.offset[1];
            MoveTo(pos);

            if (select && !match.broken)
            {
                Square sqr;
                GetSelection(sqr);

                if (match.partial)
                {
                    LanguageSupport::Match match2;

                    if (ls->FindMatch(match.line[1], match.offset[1], match2))
                    {
                        sqr.start.line   = match2.line[1];
                        sqr.start.column = inx2pos(match2.line[1], match2.offset[1]);
                    }
                }
                else if (sqr.is_empty()) // adjust a original cursor position to a start token position
                {
                    if (!match.backward) 
                    {
                        sqr.start.line   = match.line[0];
                        sqr.start.column = inx2pos(match.line[0], match.offset[0]);
                    }
                    else
                    {
                        sqr.end.line   = match.line[0];
                        sqr.end.column = inx2pos(match.line[0], match.offset[0] + match.length[0]);
                    }
                }

                if (match.backward) 
                {
                    sqr.start.line   = match.line[1];
                    sqr.start.column = inx2pos(match.line[1], match.offset[1]);
                }
                else
                {
                    sqr.end.line   = match.line[1];
                    sqr.end.column = inx2pos(match.line[1], match.offset[1] + match.length[1]);
                }
                SetSelection(sqr);
                MoveTo(sqr.end);
            }
            
            if(match.broken)
            {
              MessageBeep((UINT) -1);
            }
            Common::SetStatusText(!match.broken ? _T("Match found.") : _T("Match broken."));
        }
        else
        {
          Common::SetStatusText(_T("Match not found."));
        }
    }
}

};



