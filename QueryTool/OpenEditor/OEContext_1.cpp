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

#include "pch.h"
#include <algorithm>
#include <COMMON/ExceptionHelper.h>
#include <OEContext.h>
#include <OELanguage.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define _CHECK_ALL_PTR_ _CHECK_AND_THROW_(m_pStorage, "Editor has not been initialized properly!")

namespace OpenEditor
{
    using namespace std;

EditContext::EditContext ()
{
    m_pStorage = 0;
    m_BlockMode = ebtStream;

    Init ();
}

EditContext::~EditContext ()
{
    if (m_pStorage)
        m_pStorage->UnlinkContext(this);
}

void EditContext::Init ()
{
    m_orgHrzPos     =
    m_curPos.line   =
    m_curPos.column = 0;

    m_blkPos.start.line   =
    m_blkPos.start.column =
    m_blkPos.end.line     =
    m_blkPos.end.column   = 0;

    m_bDirtyPosition = false;
    m_bCurrentLineHighlighted = true;
}

void EditContext::SetStorage (Storage* pStorage)
{
    if (m_pStorage)
        m_pStorage->UnlinkContext(this);

    m_pStorage = pStorage;

    if (m_pStorage)
        m_pStorage->LinkContext(this);

    _CHECK_ALL_PTR_;

    Init ();
}

void EditContext::OnSettingsChanged ()
{
    InvalidateWindow ();
    adjustCursorPosition();
}

int EditContext::GetLineLength (int line) const
{
    _CHECK_ALL_PTR_;

    //_ASSERTE(line < GetLineCount());

    if (line < GetLineCount())
    {
        int len;
        const char* ptr;
        GetLine(line, ptr, len);
        return EditContext::inx2pos (ptr, len, len);
    }

    return 0;
}

bool EditContext::Find (const EditContext*& pEditContext, int& line, int& start, int& end, bool thruEof) const
{
    _ASSERTE(m_pStorage);

    const Storage* pStorage = m_pStorage;

    int _start = pos2inx(line, start, GetCursorBeyondEOL());
    int _end   = pos2inx(line, end, GetCursorBeyondEOL());

    bool retVal = m_pStorage->Find(pStorage, line, _start, _end, thruEof);

    if (retVal)
    {
        if (pStorage != m_pStorage)
            pEditContext = (EditContext*)pStorage->GetFistEditContext();
        else
            pEditContext = this;

        start = pEditContext->inx2pos(line, _start);
        end   = pEditContext->inx2pos(line, _end);
    }

    return retVal;
}

bool EditContext::Replace (const char* text, int line, int start, int& end)
{
    _ASSERTE(m_pStorage);

    int _start = pos2inx(line, start, GetCursorBeyondEOL());
    int _end   = pos2inx(line, end, GetCursorBeyondEOL());

    bool retVal = m_pStorage->Replace(text, line, _start, _end);

    if (retVal)
    {
        start = inx2pos(line, _start);
        end   = inx2pos(line, _end);

        Square blk;
        blk.start.column = start;
        blk.end.column = end;
        blk.end.line = blk.start.line = line;

        SetSelection(blk);
        MoveTo(blk.end);
    }

    return retVal;
}

int EditContext::SearchBatch (const char* text, ESearchBatch mode)
{
    _ASSERTE(m_pStorage);
    Square blk;
    int retVal = m_pStorage->SearchBatch(text, mode, blk);
    if (mode == esbReplace && retVal > 0)
    {
        blk.start.column = inx2pos(blk.start.line, blk.start.column);
        blk.end.column = inx2pos(blk.end.line, blk.end.column);
        SetSelection(blk);
        MoveTo(blk.end);
    }
    return retVal;
}

};
