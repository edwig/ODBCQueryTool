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
    15.12.2002 bug fix, undo/redo of lower/upper don't save position/selection
    05.03.2003 bug fix, lower/upper/... operaration causes an exception on blank space
*/

#include "stdafx.h"
#include <algorithm>
#include <strstream>
#include <COMMON/ExceptionHelper.h>
#include "OpenEditor/OEContext.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define _CHECK_ALL_PTR_ _CHECK_AND_THROW_(m_pStorage, "Editor has not been initialized properly!")

namespace OpenEditor
{
    using namespace std;

void EditContext::StartScan (EBlockMode blkMode, const Square* pSqr)
{
    if (pSqr)
    {
        m_ScanBlockMode = blkMode;
        m_ScanSquare = *pSqr;
        m_ScanSquare.normalize();

        if (m_ScanBlockMode == ebtColumn
        && m_ScanSquare.start.column > m_ScanSquare.end.column)
            swap(m_ScanSquare.start.column, m_ScanSquare.end.column);

        int lines = GetLineCount();
        if (!(m_ScanSquare.end.line < lines))
        {
            m_ScanSquare.end.line = lines - 1;
            
            if (m_ScanBlockMode != ebtColumn)
                m_ScanSquare.end.column = INT_MAX;
        }

        m_nScanCurrentLine = pSqr->start.line;
    }
    else
    {
        memset(&m_ScanSquare, 0, sizeof m_ScanSquare);
        m_ScanBlockMode = blkMode;
        m_nScanCurrentLine = 0;
    }

    calculateScanLine();
}

bool EditContext::Next ()
{
    m_nScanCurrentLine++;

    bool eof = Eof();

    if (!eof) 
        calculateScanLine();

    return !eof;
}

bool EditContext::Eof () const
{
    if (m_ScanSquare.is_empty())
        return m_nScanCurrentLine < GetLineCount() ? false : true;
    else
        return m_nScanCurrentLine < m_ScanSquare.end.line + 1 ? false : true;
}

void EditContext::calculateScanLine ()
{
    m_nScanStartPos = 0;
    m_nScanEndPos   = GetLineLength(m_nScanCurrentLine);

    switch (m_ScanBlockMode)
    {
    case ebtStream:
        if (m_nScanCurrentLine == m_ScanSquare.start.line)
        {
            m_nScanStartPos = pos2inx(m_nScanCurrentLine, m_ScanSquare.start.column);
        }
        if (m_nScanCurrentLine == m_ScanSquare.end.line)
        {
            m_nScanEndPos = pos2inx(m_nScanCurrentLine, m_ScanSquare.end.column);
        }
        break;
    case ebtColumn:
        m_nScanStartPos = pos2inx(m_nScanCurrentLine, m_ScanSquare.start.column);
        m_nScanEndPos   = pos2inx(m_nScanCurrentLine, m_ScanSquare.end.column);
        break;
    }
}

void EditContext::GetScanLine (int& line, int& start, int& end) const
{
    line  = m_nScanCurrentLine;
    start = m_nScanStartPos;
    end   = m_nScanEndPos;
}

void EditContext::GetScanLine (const char*& ptr, int& len) const
{
    int line, start, end;
    GetScanLine(line, start, end);

    GetLine(line, ptr, len);

    if (start < len)
    {
        ptr += start;
        len = min(len, end - start);
    }
    else
    {
        ptr = 0;
        len = 0;
    }
}

void EditContext::PutLine (const char* ptr, int len)
{
    _CHECK_ALL_PTR_

    int line, start, end;
    GetScanLine(line, start, end);

    m_pStorage->ReplaceLinePart(line, start, end, ptr, len);
}

void EditContext::StopScan ()
{
    memset(&m_ScanSquare, 0, sizeof m_ScanSquare);
    m_nScanCurrentLine = 0;
}


void EditContext::ScanAndReplaceText (bool (*pmfnDo)(const EditContext&, string&), bool curentWord)
{
    Square blkPos;

    // 05.03.2003 bug fix, lower/upper/... operaration causes an exception on blank space
    if (!curentWord || !WordFromPoint(GetPosition(), blkPos))
        GetSelection(blkPos);

    blkPos.normalize();

    if (!blkPos.is_empty())
    {
        // start undo transaction
        UndoGroup undoGroup(*this);
        PushInUndoStack(m_curPos);
        Square sel;
        GetSelection(sel);
        PushInUndoStack(GetBlockMode(), sel);

        Storage::NotificationDisabler disabler(m_pStorage);

        StartScan(GetBlockMode(), &blkPos);

        while (!Eof())
        {
            string str;
            int len = 0;
            const char* ptr = 0;

            GetScanLine(ptr, len);

            if (len > 0)
            {
                str.assign(ptr, len);

                if (!(*pmfnDo)(*this, str))
                    break;

                if (static_cast<int>(str.length()) != len || strncmp(str.c_str(), ptr, len))
                    PutLine(str.c_str(), (int)str.length());
            }

            Next();
        }

        disabler.Enable();

        StopScan();

        GetSelection(sel);
        PushInUndoStack(GetBlockMode(), sel);
        PushInUndoStack(m_curPos);
    }
    // !!! to catch exceptions and undo the curent operation !!!
}

bool EditContext::LowerText (const EditContext&, string& str)
{
    for (string::iterator it = str.begin(); it != str.end(); it++)
        *it = tolower(*it);

    return true;
}

bool EditContext::UpperText (const EditContext&, string& str)
{
    for (string::iterator it = str.begin(); it != str.end(); it++)
        *it = toupper(*it);

    return true;
}

bool EditContext::CapitalizeText  (const EditContext&, string& str)
{
    for (string::iterator it = str.begin(); it != str.end(); it++)
        if (it == str.begin() || !isalpha(*(it-1)))
            *it = toupper(*it);
        else
            *it = tolower(*it);

    return true;
}

bool EditContext::InvertCaseText  (const EditContext&, string& str)
{
    for (string::iterator it = str.begin(); it != str.end(); it++)
        if (islower(*it))
            *it = toupper(*it);
        else if (isupper(*it))
            *it = tolower(*it);

    return true;
}

bool EditContext::tabify (const EditContext& edt, string& str, bool leading)
{
    // simplify all!
    if (leading) UntabifyLeadingSpaces(edt, str); 
    else         UntabifyText(edt, str);

    string buff;

    int tabSpacing = edt.GetTabSpacing();

    unsigned i,k;
    for (i = 0; str[i] && i < str.size(); )
    {
        if (str[i] == ' ')
        {
            // If all characters are space untill next tab position
            // then we replace them with a tab character
            unsigned pos = (i / tabSpacing + 1) * tabSpacing;

            for (k = 0; i + k < pos && str[i + k] == ' '; k++)
                ;

            if (i + k == pos)
            {
                buff +=  '\t';
                i = pos;
                continue;
            }
        }
        else if (leading) // exit when it's not space
        {
            buff += str.c_str() + i;
            break;
        }

        buff += str[i++];
    }

    str = buff;

    return true;
}

bool EditContext::untabify (const EditContext& edt, string& str, bool leading)
{
    string buff;

    int tabSpacing = edt.GetTabSpacing();

    for (unsigned i(0), j(0); str[i] && i < str.size(); i++)
    {
        if (str[i] == '\t')
        {
            unsigned pos = (j / tabSpacing + 1) * tabSpacing;

            while (j < pos)
            {
                buff += ' ';
                j++;
            }
        }
        else
        {
            if (leading)  // exit when it's not space
            {
                buff += str.c_str() + i;
                break;
            }
            else
            {
                buff += str[i];
                j++;
            }
        }
    }

    str = buff;

    return true;
}

bool EditContext::TabifyText (const EditContext& edt, string& str)
{
    return tabify(edt, str, false);
}

bool EditContext::TabifyLeadingSpaces (const EditContext& edt, string& str)
{
    return tabify(edt, str, true);
}

bool EditContext::UntabifyText (const EditContext& edt, string& str)
{
    return untabify(edt, str, false);
}

bool EditContext::UntabifyLeadingSpaces (const EditContext& edt, string& str)
{
    return untabify(edt, str, true);
}

};
