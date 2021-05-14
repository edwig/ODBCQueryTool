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

// 07/09/2002 bug fix, copy/paste operation loses the first blank line

#include "pch.h"
#include <algorithm>
#include <strstream>
#include <COMMON/ExceptionHelper.h>
#include "OEContext.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define _CHECK_ALL_PTR_ _CHECK_AND_THROW_(m_pStorage, "Editor has not been initialized properly!")

namespace OpenEditor
{
    using namespace std;

int EditContext::adjustPosByTab (int line, int column, bool next) const
{
    _CHECK_ALL_PTR_;

    if (line < GetLineCount())
    {
        int len;
        const char* ptr;
        GetLine(line, ptr, len);
        int col, i;

        int adjCol = 0;
        int tabSpacing = GetTabSpacing();

        for (i = 0, col = 0; i < len && col < column; i++)
        {
            if (ptr[i] == '\t')
            {
                //_t--**** 1 % 4 = 1; 4 - 1 % 4 = 3
                //01234567 1 / 4 = 0; (0 + 1) * 4 = 4
                col = (col / tabSpacing + 1) * tabSpacing;

                if (next)
                    adjCol = col;

                if (col > column)
                    break;
            }
            else
                col++;

            adjCol = col;
        }

        for (; col < column ; adjCol++, col++);

        return adjCol;
    }
    return column;
}

int EditContext::inx2pos (int line, int inx) const
{
    _CHECK_ALL_PTR_;

    int len;
    const char* str;
    GetLine(line, str, len);

    return inx2pos(str, len, inx);
}

int EditContext::inx2pos (const char* ptr, int len, int inx) const
{
    _CHECK_ALL_PTR_;

    int tabSpacing = GetTabSpacing();

    int i,pos;
    for (i = 0,pos = 0; i < len && i < inx ; i++)
    {
        if (ptr[i] == '\t')
        {
            //_t--**** 1 % 4 = 1; 4 - 1 % 4 = 3
            //01234567 1 / 4 = 0; (0 + 1) * 4 = 4
            pos = (pos / tabSpacing + 1) * tabSpacing;
        }
        else
            pos++;
    }

    if (i < inx)
    {
        pos += inx - i;
//        _ASSERTE(0);
    }

    return pos;
}

int EditContext::pos2inx (int line, int pos, bool virt_spaces) const
{
    _CHECK_ALL_PTR_;

    if (line < m_pStorage->GetLineCount())
    {
        int len;
        const char* str;
        GetLine(line, str, len);

        return pos2inx(str, len, pos, virt_spaces);
    }
    else
    {
//        _ASSERTE(virt_spaces);
        return pos;
    }
}

int EditContext::pos2inx (const char* ptr, int len, int _pos, bool virt_spaces) const
{
    _CHECK_ALL_PTR_;

    int tabSpacing = GetTabSpacing();

    int i,pos;
    for (i = 0,pos = 0; i < len && pos < _pos ; i++)
    {
        if (ptr[i] == '\t')
        {
            //_t--**** 1 % 4 = 1; 4 - 1 % 4 = 3
            //01234567 1 / 4 = 0; (0 + 1) * 4 = 4
            pos = (pos / tabSpacing + 1) * tabSpacing;

            if (pos > _pos)
            {
// I need to check when it's happened
// _ASSERTE(0);
                i++;
                break;
            }
        }
        else
            pos++;
    }

    if (virt_spaces && pos < _pos)
    {
        for (; pos < _pos ; i++, pos++);
    }

    return i;
}

//
// maybe the best way is throwing exceptions insted of ...
//
bool EditContext::adjustCursorPosition ()
{
    _CHECK_ALL_PTR_;

    Position pos = m_curPos;
    
    AdjustPosition(pos, false);

    if (m_curPos != pos)
    {
        m_curPos = pos;
        return true;
    }

    return false;
}

void EditContext::line2buff (int line, int start, int end, string& buff, bool fill) const
{
    int appended = 0;

    if (line < GetLineCount())
    {
        int len;
        const char* str;
        GetLine(line, str, len);

        if (start < inx2pos(str, len, len))
        {
            int _start = pos2inx(str, len, start, false);
            int _end   = pos2inx(str, len, end, false);
            appended = _end - _start;
            buff.append(str + _start, _end - _start);
        }
    }

    if (fill)
        for (int i(appended); i < (end - start); i++)
            buff += ' ';
}

bool EditContext::getLine (std::istrstream& io, string& buff, bool& with_CR)
{
    char chr;
    bool ret_val = false;
    
    buff.erase();
    with_CR = false;

    while (io.get(chr))
    {
        ret_val = true;

        switch (chr)
        {
        case '\r':
            io.get(chr);
            if (chr != '\n') 
                io.putback(chr);
        case '\n':
            with_CR = true;
            break;
        }

        if (with_CR) break;

        buff += chr;
    }

    return ret_val;
}

};
