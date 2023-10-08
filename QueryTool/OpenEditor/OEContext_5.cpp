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

#define _CHECK_ALL_PTR_ _CHECK_AND_THROW_(m_pStorage, _T("Editor has not been initialized properly!"))

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

void EditContext::GetScanLine (LPCTSTR& ptr, int& len) const
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

void EditContext::PutLine (LPCTSTR ptr, int len)
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


void EditContext::ScanAndReplaceText (bool (*pmfnDo)(const EditContext&, CString&), bool curentWord)
{
    Square blkPos;

    // 05.03.2003 bug fix, lower/upper/... operation causes an exception on blank space
    if(!curentWord || !WordFromPoint(GetPosition(),blkPos))
    {
      GetSelection(blkPos);
    }
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
            CString str;
            int len = 0;
            LPCTSTR ptr = 0;

            GetScanLine(ptr, len);

            if (len > 0)
            {
              LPTSTR bufpnt = str.GetBufferSetLength(len + 1);
              _tcsncpy_s(bufpnt,(len+1),ptr,len + 1);
              str.ReleaseBuffer();

              if(!(*pmfnDo)(*this,str))
              {
                break;
              }
              if(str.GetLength() != len || _tcsncmp(str.GetString(),ptr,len))
              {
                PutLine(str.GetString(),str.GetLength());
              }
            }

            Next();
        }

        disabler.Enable();

        StopScan();

        GetSelection(sel);
        PushInUndoStack(GetBlockMode(), sel);
        PushInUndoStack(m_curPos);
    }
    // !!! to catch exceptions and undo the current operation !!!
}

bool EditContext::LowerText (const EditContext&, CString& str)
{
  str.MakeLower();
  return true;
}

bool EditContext::UpperText (const EditContext&, CString& str)
{
  str.MakeUpper();
  return true;
}

bool EditContext::CapitalizeText  (const EditContext&, CString& str)
{
  for(int ind = 0;ind < str.GetLength(); ++ind)
  {
    if(ind == 0 || !isalpha(str.GetAt(ind - 1)))
    {
      str.SetAt(ind,toupper(str.GetAt(ind)));
    }
  }
  return true;
}

bool EditContext::InvertCaseText  (const EditContext&, CString& str)
{
  for(int ind = 0;ind < str.GetLength(); ++ind)
  {
    if(islower(str.GetAt(ind)))
    {
      str.SetAt(ind,toupper(str.GetAt(ind)));
    }
    else
    {
      str.SetAt(ind,tolower(str.GetAt(ind)));
    }
  }
  return true;
}

bool EditContext::tabify (const EditContext& edt, CString& str, bool leading)
{
  // simplify all!
  if (leading) UntabifyLeadingSpaces(edt, str); 
  else         UntabifyText(edt, str);

  CString buff;
  int tabSpacing = edt.GetTabSpacing();

  for(unsigned i = 0;i < (unsigned)str.GetLength();)
  {
    if (str[i] == ' ')
    {
      // If all characters are space until next tab position
      // then we replace them with a tab character
      unsigned pos = (i / tabSpacing + 1) * tabSpacing;

      unsigned k = 0;
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
      buff += str.GetString() + i;
      break;
    }
    buff += str[i++];
  }
  str = buff;

  return true;
}

bool EditContext::untabify (const EditContext& edt, CString& str, bool leading)
{
    CString buff;

    int tabSpacing = edt.GetTabSpacing();

    for (unsigned i(0), j(0);i < (unsigned)str.GetLength(); i++)
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
                buff += str.GetString() + i;
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

bool EditContext::TabifyText (const EditContext& edt, CString& str)
{
    return tabify(edt, str, false);
}

bool EditContext::TabifyLeadingSpaces (const EditContext& edt, CString& str)
{
    return tabify(edt, str, true);
}

bool EditContext::UntabifyText (const EditContext& edt, CString& str)
{
    return untabify(edt, str, false);
}

bool EditContext::UntabifyLeadingSpaces (const EditContext& edt, CString& str)
{
    return untabify(edt, str, true);
}

};
