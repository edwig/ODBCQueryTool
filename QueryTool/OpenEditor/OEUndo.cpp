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
    23.09.2002 improvement/performance tuning, undo has been reimplemented
    06.12.2002 bug fix, in find dialog count and mark change "modified" status
    13.01.2003 bug fix, losing modification status if undo stack overflows
*/

#include "stdafx.h"
#include "COMMON/StrHelpers.h"
#include "OpenEditor/OEUndo.h"
#include "OpenEditor/OEStorage.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


namespace OpenEditor
{
#ifdef _DEBUG
#define CHECK_UNDOSTACK   //debug_checking()
#else
#define CHECK_UNDOSTACK
#endif


///////////////////////////////////////////////////////////////////////////////

UndoContext::UndoContext (Storage& storage)
: m_storage(storage)
{
    m_selMode = ebtUndefined;
    m_position.column = m_position.line = 0;
    m_selection.clear();
};

///////////////////////////////////////////////////////////////////////////////

UndoStack::UndoStack ()
{
    m_top = m_bottom = m_current = 0;
    m_saved = 0;

    m_countLimit = 1000;
    m_memLimit   = 1024 * 1024;
    m_memUsage   = 0;

    m_cachedDataModified = false;
    m_savedIsGone = false;
    m_completeUndoImpossible = false;
}

UndoStack::~UndoStack ()
{
    Clear ();
}

void UndoStack::Clear ()
{
    CHECK_UNDOSTACK;
    removeAllAbove(m_bottom);
    
    m_top = m_bottom = m_current = 0;
    m_saved = 0;

    // dont reset m_countLimit, m_memLimit;
    m_memUsage = 0;

    m_cachedDataModified = false;
    m_savedIsGone = false;
    m_completeUndoImpossible = false;

    m_actionSeq.Reset();
    m_cachedActionId = m_actionSeq;
}

#ifdef _DEBUG
void UndoStack::debug_checking () const
{
    _ASSERTE((m_top && m_bottom && m_memUsage) 
        || (!m_top && !m_bottom && !m_current && !m_memUsage));

    if (m_top && m_bottom && m_current)
    {
        UndoBase* item = m_top;
        while (item && item->m_below)
            item = item->m_below;
        _ASSERTE(item == m_bottom);

        if (m_current)
        {
            item = m_current;
            while (item && item->m_above)
                item = item->m_above;
            _ASSERTE(item == m_top);
        }

        item = m_bottom;
        while (item && item->m_above)
            item = item->m_above;
        _ASSERTE(item == m_top);
    }
}
#endif

void UndoStack::removeAllAbove (UndoBase* item)
{
    m_actionSeq++;

    while (item)
    {
        UndoBase* next = item->m_above;
#ifdef _DEBUG
        // just for testing
        item->m_below = 0;
        item->m_above = 0;
#endif
        m_memUsage -= item->GetMemUsage();
        delete item;
        item = next;
    }
    m_top = 0;
}

void UndoStack::Push (UndoBase* pUndo)
{
    CHECK_UNDOSTACK;
    m_actionSeq++;

    if (m_current != m_top)
    {
        removeAllAbove(m_current ? m_current->m_above : m_bottom);
        m_top = m_current;
    }

    pUndo->SetGroupId(m_groupSeq);

    if (!m_top) 
    {
        pUndo->m_below = pUndo->m_above = 0;
        m_current = m_top = m_bottom = pUndo;
    }
    else 
    {
        pUndo->m_above = 0;
        pUndo->m_below = m_top;
        m_top->m_above = pUndo;
        m_current = m_top = pUndo;
    }

    m_memUsage += pUndo->GetMemUsage();
    

    if (!IsEmpty()
    && m_bottom->GetGroupId() != m_current->GetGroupId()
    && (m_memUsage > m_memLimit 
        || ((m_top->GetGroupId() - m_bottom->GetGroupId()) > m_countLimit)))
        RemoveExceed();

    CHECK_UNDOSTACK;
}

void UndoStack::RemoveExceed ()
{
    CHECK_UNDOSTACK;

    SequenceId curGroup;

    while (m_bottom
    && (
        // allocated more then limit
        m_memUsage > m_memLimit 
        // the number of stored undo groups more then limit
        || ((m_top->GetGroupId() - m_bottom->GetGroupId()) > m_countLimit)
        // group has to be removed whole!
        || m_bottom->GetGroupId() == curGroup
    ))
    {
        if (!m_saved || m_bottom == m_saved) 
            m_savedIsGone = true;
        if (m_savedIsGone && !m_completeUndoImpossible)
            m_completeUndoImpossible |= m_bottom->Modifying();

        UndoBase* next = m_bottom->m_above;
#ifdef _DEBUG
        // just for testing
        m_bottom->m_above = 0;
        m_bottom->m_below = 0;
#endif
        m_memUsage -= m_bottom->GetMemUsage();
        curGroup = m_bottom->GetGroupId();
        delete m_bottom;
        m_bottom = next;
        if (m_bottom)
            m_bottom->m_below = 0;
    }
    if (!m_bottom) 
        m_current = m_top = 0;
    
    CHECK_UNDOSTACK;
}

bool UndoStack::Undo (UndoContext& cxt)
{
    CHECK_UNDOSTACK;
    m_actionSeq++;

    if (CanUndo())
    {
        try
        {
            cxt.m_storage.m_bUndoRedoProcess = true;
            SequenceId groupId = m_current->GetGroupId();

            while (m_current && m_current->GetGroupId() == groupId)
            {
                m_current->Undo(cxt);
                m_current = m_current->m_below;
            }

            cxt.m_storage.m_bUndoRedoProcess = false;
        }
        catch (...)
        {
            cxt.m_storage.m_bUndoRedoProcess = false;
            throw;
        }

        CHECK_UNDOSTACK;
        return true;
    }
    return false;
}

bool UndoStack::Redo (UndoContext& cxt)
{
    CHECK_UNDOSTACK;
    m_actionSeq++;

    if (CanRedo())
    {
        try
        {
            cxt.m_storage.m_bUndoRedoProcess = true;

            UndoBase* item = m_current ? m_current->m_above : m_bottom;
            SequenceId groupId = item->GetGroupId();

            while (item && item->GetGroupId() == groupId)
            {
                item->Redo(cxt);
                m_current = item;
                item = item->m_above;
            }

            cxt.m_storage.m_bUndoRedoProcess = false;
        }
        catch (...)
        {
            cxt.m_storage.m_bUndoRedoProcess = false;
            throw;
        }

        CHECK_UNDOSTACK;
        return true;
    }
    return false;
}

bool UndoStack::AppendOnInsert (int line, int col, const char* str, int len, const DelimitersMap& delim)
{
    if (len == 1
    && !IsEmpty()
    && m_top == m_current
    && m_top->Type() == eeoInsert)
    {
        unsigned memUsage = m_top->GetMemUsage();

        if (static_cast<UndoInsert*>(m_top)->Append(line, col, str, len, delim))
        {
            m_actionSeq++;
            m_memUsage += m_top->GetMemUsage() - memUsage;
            return true;
        }
    }
    return false;
}

bool UndoStack::IsDataModified () const
{
    if (m_completeUndoImpossible) 
        return true;
    if (IsSavedPos()) 
        return false;
    if (m_cachedActionId == m_actionSeq)
        return m_cachedDataModified;

    bool modified = false, currFound = false, savedFound = false;

    for (UndoBase* item = m_top; !modified && item; item = item->m_below)
    {
        if (item == m_current)
            currFound = true;
        if (item == m_saved)
            savedFound = true;
        if (currFound && savedFound)
            break;
        if (currFound || savedFound)
            modified |= item->Modifying();
    }

    m_cachedActionId = m_actionSeq;
    m_cachedDataModified = modified;

    return modified;
}

///////////////////////////////////////////////////////////////////////////////

bool UndoInsert::Append (int line, int col, const char* str, int len, const DelimitersMap& delim)
{
    if (str && len
    && m_position.line == line 
    && m_position.column + static_cast<int>(m_str.length()) == col
    && delim[m_str.last()] == delim[*str])
    {
        m_str.append(str, len);
        return true;
    }
    return false;
}

void UndoInsert::Undo (UndoContext& cxt) const
{
    cxt.m_position = m_position;
    cxt.m_storage.DeleteLinePart(cxt.m_position.line, cxt.m_position.column, cxt.m_position.column + (int)m_str.length());
}

void UndoInsert::Redo (UndoContext& cxt) const
{
    cxt.m_position = m_position;
    cxt.m_storage.InsertLinePart(cxt.m_position.line, cxt.m_position.column, m_str.data(), (int)m_str.length());
    cxt.m_position.column += (int)m_str.length();
}

void UndoOverwrite::Undo (UndoContext& cxt) const
{
    cxt.m_position = m_position;
    cxt.m_storage.ReplaceLinePart(cxt.m_position.line, cxt.m_position.column, cxt.m_position.column + (int)m_newStr.length(), 
                            m_orgStr.data(), (int)m_orgStr.length());
}

void UndoOverwrite::Redo (UndoContext& cxt) const
{
    cxt.m_position = m_position;
    cxt.m_storage.ReplaceLinePart(cxt.m_position.line, cxt.m_position.column, cxt.m_position.column + (int)m_orgStr.length(), 
                            m_newStr.data(), (int)m_newStr.length());
    cxt.m_position.column += (int)m_newStr.length();
}

void UndoDelete::Undo (UndoContext& cxt) const
{
    cxt.m_position = m_position;
    cxt.m_storage.InsertLinePart(cxt.m_position.line, cxt.m_position.column, m_str.data(), (int)m_str.length());
    cxt.m_position.column += (int)m_str.length();
}

void UndoDelete::Redo (UndoContext& cxt) const
{
    cxt.m_position = m_position;
    cxt.m_storage.DeleteLinePart(cxt.m_position.line, cxt.m_position.column, cxt.m_position.column + (int)m_str.length());
}

void UndoInsertLine::Undo (UndoContext& cxt) const
{
    cxt.m_position.column = 0;
    cxt.m_position.line = m_line;
    cxt.m_storage.DeleteLine(cxt.m_position.line);
}

void UndoInsertLine::Redo (UndoContext& cxt) const
{
    cxt.m_position.column = 0;
    cxt.m_position.line = m_line;
    cxt.m_storage.InsertLine(cxt.m_position.line, m_str.data(), (int)m_str.length());
}

void UndoDeleteLine::Undo (UndoContext& cxt) const
{
    cxt.m_position.column = 0;
    cxt.m_position.line = m_line;
    cxt.m_storage.InsertLine(cxt.m_position.line, m_str.data(), (int)m_str.length());
}

void UndoDeleteLine::Redo (UndoContext& cxt) const
{
    cxt.m_position.column = 0;
    cxt.m_position.line = m_line;
    cxt.m_storage.DeleteLine(cxt.m_position.line);
}

void UndoSplitLine::Undo (UndoContext& cxt) const
{
    cxt.m_position = m_position;
    cxt.m_storage.MergeLines(cxt.m_position.line);
}

void UndoSplitLine::Redo (UndoContext& cxt) const
{
    cxt.m_position = m_position;
    cxt.m_storage.SplitLine(cxt.m_position.line, cxt.m_position.column);
}

void UndoMergeLine::Undo (UndoContext& cxt) const
{
    cxt.m_position = m_position;
    cxt.m_storage.SplitLine(cxt.m_position.line, cxt.m_position.column);
}

void UndoMergeLine::Redo (UndoContext& cxt) const
{
    cxt.m_position = m_position;
    cxt.m_storage.MergeLines(cxt.m_position.line);
}

void UndoInsertLines::Undo (UndoContext& cxt) const
{
    cxt.m_position.column = 0;
    cxt.m_position.line = m_line;
    cxt.m_storage.DeleteLines(m_line, (int)m_lines.size());
}

void UndoInsertLines::Redo (UndoContext& cxt) const
{
    cxt.m_position.column = 0;
    cxt.m_position.line = m_line;
    cxt.m_storage.InsertLines(cxt.m_position.line, m_lines);
}

unsigned UndoInsertLines::GetMemUsage() const 
{ 
    unsigned usage = sizeof(*this);

    for (unsigned i = 0; i < m_lines.size(); i++)
    {
      usage += (int) m_lines[i].length();
    }
    return usage; 
};

void UndoDeleteLines::Undo (UndoContext& cxt) const
{
    cxt.m_position.column = 0;
    cxt.m_position.line = m_line;
    cxt.m_storage.InsertLines(cxt.m_position.line, m_lines);
}

void UndoDeleteLines::Redo (UndoContext& cxt) const
{
    cxt.m_position.column = 0;
    cxt.m_position.line = (int)m_line;
    cxt.m_storage.DeleteLines(cxt.m_position.line, (int)m_lines.size());
}

unsigned UndoDeleteLines::GetMemUsage() const 
{ 
    unsigned usage = sizeof(*this);

    for (unsigned i = 0; i < m_lines.size(); i++)
    {
      usage += (int)m_lines[i].length();
    }
    return usage; 
};

void UndoCursorPosition::Undo (UndoContext& cxt) const
{
    cxt.m_position = m_position;
}

void UndoCursorPosition::Redo (UndoContext& cxt) const
{
    cxt.m_position = m_position;
}

void UndoSelection::Undo (UndoContext& cxt) const
{
    cxt.m_selMode  = m_selMode;
    cxt.m_selection = m_selection;
}

void UndoSelection::Redo (UndoContext& cxt) const
{
    cxt.m_selMode  = m_selMode;
    cxt.m_selection = m_selection;
}

void UndoNotification::Undo (UndoContext& cxt) const
{
    cxt.m_storage.m_bDisableNotifications = !m_disabled; 
    
    if (!cxt.m_storage.m_bDisableNotifications)
    {
        cxt.m_storage.Notify_ChangedLines(0, cxt.m_storage.GetLineCount()+1);
        cxt.m_storage.Notify_ChangedLinesQuantity(0, 0);
    }
}

void UndoNotification::Redo (UndoContext& cxt) const
{
    cxt.m_storage.m_bDisableNotifications = m_disabled; 
    
    if (!cxt.m_storage.m_bDisableNotifications)
    {
        cxt.m_storage.Notify_ChangedLines(0, cxt.m_storage.GetLineCount()+1);
        cxt.m_storage.Notify_ChangedLinesQuantity(0, 0);
    }
}

}
