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
#include <string.h>
#include <algorithm>
#include <common/arg_shared.h>
#include <COMMON/ExceptionHelper.h>
#include "OEHelpers.h"
#include "OEStorage.h"
#include <COmmon/regexp.h>

//#define BOOST_REGEX_NO_LIB
//#define BOOST_REGEX_STATIC_LINK
//#include <boost/regex.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace OpenEditor
{
    using namespace std;
    using arg::counted_ptr;

    const int cnRegMatchSize = 100;
/*

    // for isolation from "boost"
    struct Searcher::RegexpCxt
    {
        regex_t     pattern;
        regmatch_t  match[cnRegMatchSize];

        RegexpCxt ()  { ::memset(this, 0, sizeof(*this)); }
        ~RegexpCxt () { regfree(&pattern); }
    };

*/
Searcher::Searcher (bool persistent)
//: m_cxt(new RegexpCxt)
{
    m_bPersistent = persistent;

    m_bBackward   = false;
    m_bWholeWords = false;
    m_bMatchCase  = false;
    m_bRegExpr    = false;
    m_bSearchAll  = false;

    m_bPatternCompiled = false;
}

Searcher::~Searcher ()
{
    //delete m_cxt;
}

void Searcher::AddRef (Storage* context)
{
  if (context)
  {
    vector<const Storage*>::iterator it(m_refList.begin()), end(m_refList.end());

    for (; it != end && *it != context; it++);

    if (it == end)
    {
      m_refList.push_back(context);
    }
  }
}

void Searcher::RemoveRef (Storage* context)
{
  if (context)
  {
    vector<const Storage*>::iterator it(m_refList.begin()), end(m_refList.end());

    for (; it != end && *it != context; it++);

    if (*it == context)
    {
      m_refList.erase(it, it + 1);
    }
  }

  if (!m_bPersistent && !m_refList.size())
  {
    delete this;
  }
}

void Searcher::SetText (LPCTSTR str)
{
  m_strText = str;
  m_bPatternCompiled = false;
}

void Searcher::SetOption (bool backward
                         ,bool wholeWords
                         ,bool matchCase
                         ,bool regExpr
                         ,bool searchAll)
{
  m_bBackward   = backward;
  m_bWholeWords = wholeWords;
  m_bMatchCase  = matchCase;
  m_bRegExpr    = regExpr;
  m_bSearchAll  = searchAll;
  m_bPatternCompiled = false;
}

void Searcher::GetOption (bool& backward
                         ,bool& wholeWords
                         ,bool& matchCase
                         ,bool& regExpr
                         ,bool& searchAll) const
{
    backward   = m_bBackward;
    wholeWords = m_bWholeWords;
    matchCase  = m_bMatchCase;
    regExpr    = m_bRegExpr;
    searchAll  = m_bSearchAll;
}

bool Searcher::isMatched (LPCTSTR str
                         ,int offset
                         ,int len
                         ,const DelimitersMap& delim
                         ,int& start
                         ,int& end) const
{
    if (!str) // workaround for rx bug
    {
        str = _T("");
        offset = len = 0;
    }
    if (!m_bPatternCompiled)
    {
      compileExpression();
    }
    //m_cxt->match[0].rm_so = offset;
    //m_cxt->match[0].rm_eo = len;

    while(m_regex.Match(&str[offset]) >= 0)
    {
      // [EH regex]
    //if(m_regex.GetMatchLen() < len)
      if(m_regex.GetMatchPos() < len)
      {
        if(m_bWholeWords)
        {
          if((m_regex.GetMatchPos() > 0   && !delim[str[m_regex.GetMatchPos()-1]]) ||
             (m_regex.GetMatchEnd() < len && !delim[str[m_regex.GetMatchEnd()]]))
          {
            ++offset;
            continue;
          }
        }
        start = offset + m_regex.GetMatchPos();
        end   = offset + m_regex.GetMatchEnd();
        return true;
      }
      else
      {
        return false;
      }
    }

    /*
    while (m_cxt->match[0].rm_so < len
    && !regexec(&m_cxt->pattern, str, cnRegMatchSize, m_cxt->match, REG_STARTEND))
    {
        if (m_bWholeWords)
        {
            if ((m_cxt->match[0].rm_so > 0 && !delim[str[m_cxt->match[0].rm_so - 1]])
            || (m_cxt->match[0].rm_eo < len && !delim[str[m_cxt->match[0].rm_eo]]))
            {
                m_cxt->match[0].rm_so++;
                m_cxt->match[0].rm_eo = len;
                continue;
            }
        }

        start = m_cxt->match[0].rm_so;
        end   = m_cxt->match[0].rm_eo;

        return true;
    }*/

    return false;
}

bool Searcher::isMatchedBackward (LPCTSTR str, int rlimit, int len, const DelimitersMap& delim, int& start, int& end) const
{
    if (!str) // workaround for rx bug
    {
        str = _T("");
        len = 0;
    }

    bool retVal = false;

    //m_cxt->match[0].rm_so = 0;
    //m_cxt->match[0].rm_eo = len;

    int offset = 0;
    while(m_regex.Match(&str[offset]) >= 0)
    {
      if(m_regex.GetMatchLen() < len)
      {
        if(m_bWholeWords)
        {
          if((m_regex.GetMatchPos() > 0   && !delim[str[m_regex.GetMatchPos()-1]]) ||
             (m_regex.GetMatchEnd() < len && !delim[str[m_regex.GetMatchEnd()]]))
          {
            ++offset;
            continue;
          }
        }
        if(m_regex.GetMatchPos() < rlimit)
        {
          retVal = true;
          start  = offset + m_regex.GetMatchPos();
          end    = offset + m_regex.GetMatchEnd();
          ++offset;
        }
        else
        {
          break;
        }
      }
      else
      {
        // [EH] No match stop here
        break;
      }
    }
    
/*
    while (m_cxt->match[0].rm_so < len
    && !regexec(&m_cxt->pattern, str, cnRegMatchSize, m_cxt->match, REG_STARTEND))
    {
        if (m_bWholeWords)
        {
            if ((m_cxt->match[0].rm_so > 0 && !delim[str[m_cxt->match[0].rm_so - 1]])
            || (m_cxt->match[0].rm_eo < len && !delim[str[m_cxt->match[0].rm_eo]]))
            {
                m_cxt->match[0].rm_so++;
                m_cxt->match[0].rm_eo = len;
                continue;
            }
        }

        if (m_cxt->match[0].rm_so < rlimit)
        {
            retVal = true;
            start = m_cxt->match[0].rm_so;
            end   = m_cxt->match[0].rm_eo;

            m_cxt->match[0].rm_so++;
            m_cxt->match[0].rm_eo = len;
        }
        else
            break;
    }*/

    return retVal;
}

void Searcher::compileExpression () const
{
  if (!m_bPatternCompiled)
  {
    CString pattern;

    if (m_bRegExpr)
    {
      pattern = m_strText.GetString();
    }
    else
    {
      for(int ind = 0;ind < m_strText.GetLength(); ++ind)
      {
        TCHAR ch = m_strText.GetAt(ind);

        switch (ch) // 17/05/2002 bug fix, cannot search for $,[,],^
        {
          case '.': 
          case '*': 
          case '\\':
          case '[': 
          case ']':
          case '^': 
          case '$': pattern += (TCHAR)'\\';
        }
        pattern += (ch);
      }
    }
    int comp_ok = m_regex.SetExp(pattern);
    m_bPatternCompiled = true;

    if (!comp_ok)
    {
      CString text = _T("Regular expression error: ") + pattern;
      StyleMessageBox(nullptr,text,_T("ERROR"),MB_OK|MB_ICONERROR);
      _CHECK_AND_THROW_(1, text);
    }
  }
}

bool Searcher::isSelectionMatched (const Storage* pstorage, int line, int start, int end) const
{
  if(!m_bPatternCompiled)
  {
    compileExpression();
  }
    if (start != end)
    {
        int len;
        LPCTSTR str = nullptr;
        pstorage->GetLine(line, str, len);

        if (!str) str = _T(""); // workaround for rx bug

//        m_cxt->match[0].rm_so = start;
//        m_cxt->match[0].rm_eo = end;
//        if(len >= end && !regexec(&m_cxt->pattern, str, cnRegMatchSize, m_cxt->match, REG_STARTEND))
//            return true;
        if((len > end) && m_regex.Match(&str[start]) >= 0)
        {
          if(m_regex.GetMatchEnd() >= end)
          {
            return true;
          }
        }
    }
    return false;
}

// all parameters are in-out
bool Searcher::Find (const Storage*& pStorage, int& line, int& _start, int& _end, bool thruEof) const
{
  if(!m_bPatternCompiled)
  {
    compileExpression();
  }
    vector<const Storage*> storages;
    vector<const Storage*>::const_iterator it;

    if (m_bSearchAll)
    {
        it = find(m_refList.begin(), m_refList.end(), pStorage);

        if (!m_bBackward)
        {
            storages.insert(storages.end(), it, m_refList.end());
            storages.insert(storages.end(), m_refList.begin(), it);
        }
        else
        {
            storages.insert(storages.end(), ++it, m_refList.end());
            storages.insert(storages.end(), m_refList.begin(), it);
            reverse(storages.begin(), storages.end());
        }
    }
    //else
    // if we find nothing in other windows then we try again for the current from top
    // 16.12.2002 bug fix, Search all windows does not find occurrence above the current
        storages.push_back(pStorage);

    int llimit = _end;
    int rlimit = _end;

    if (m_bBackward && isSelectionMatched(pStorage, line, _start, _end))
        rlimit = _start; // if something is selected and it is equal the expression, skip it!

    // the loop works only for all windows searching
    //      if we find nothing in other windows then we try again for the current from top
    for (int attempt(0); attempt < (((!thruEof || storages.size()) == 1) ? 1 : 2); attempt++)
    {
        // all window loop
        for (it = storages.begin(); it != storages.end(); it++)
        {
            int nlines = (*it)->GetLineCount();
            const DelimitersMap& delim = (*it)->GetDelimiters();

            // lines saerching, double loop,
            //      in the first saerching below current line
            //      in the second saerching from top to current line
            int first_line = 0;
            for (int loop(0); loop < ((!thruEof || m_bSearchAll) ? 1 : 2); loop++)
            {
                if (!m_bBackward)
                {
                    for (int i(line); i < nlines; i++)
                    {
                        int len;
                        LPCTSTR str;
                        (*it)->GetLine(i, str, len);

                        if (isMatched(str, llimit, len, delim, _start, _end))
                        {
                            pStorage = *it;
                            line     = i;
                            return true;
                        }
                        // reset for next line (after first)
                        llimit = 0;
                    }
                    // reset for upper text part searching
                    nlines = min(nlines, line + 1); // 22/05/2002 bug fix, search fails it it started after EOF
                    line   = 0;
                    llimit = 0;
                }
                else
                {
                    if (line == -1 || line > nlines - 1)
                    {
                        line = nlines - 1;
                        rlimit = -1;
                    }

                    for (int i(line); i >= first_line; i--)
                    {
                        int len;
                        LPCTSTR str;
                        (*it)->GetLine(i, str, len);

                        if (rlimit == -1)
                            rlimit = len;

                        if (isMatchedBackward(str, rlimit, len, delim, _start, _end))
                        {
                            pStorage = *it;
                            line     = i;
                            return true;
                        }
                        // reset for next line (after first)
                        rlimit = -1;
                    }
                    // reset for upper text part searching
                    first_line = line;
                    line       = -1;
                    rlimit     = -1;
                }
            }
        }
    }

    return false;
}


// 1st & 2d parameters are in, 3d,4th,5th are out
bool Searcher::Replace (Storage* pStorage, LPCTSTR  text, int line, int start, int& end)
{
    _CHECK_AND_THROW_(_tcspbrk(text, _T("\n\r")) == NULL,_T("Replace error: replace string with <Carriage return> not supporded!"));

    if (line < pStorage->GetLineCount())
    {
        int len;
        LPCTSTR str;
        pStorage->GetLine(line, str, len);

        int _start, _end;
        // check the current selection
        // !!! it's possible to suppress double checking !!!
        if (isMatched(str, start, len, pStorage->GetDelimiters(), _start, _end)
        && _start == start && _end == end)
        {
            CString buff;

            if (m_bRegExpr)
            {
                int len;
                LPCTSTR str;
                pStorage->GetLine(line, str, len);

                for (LPCTSTR ptr = text; *ptr; ptr++)
                {
                    if (*ptr == '\\' && ::isdigit(*(ptr+1)))
                    {
                        TCHAR _group[2] = { *(++ptr), 0 }; // 28/07/2002 bug fix, RegExp replace fails on \1...
                        int group = _ttoi(_group);
/*
                        if (group < cnRegMatchSize
                        && m_cxt->match[group].rm_so != -1 && m_cxt->match[group].rm_eo != -1)
                        {
                            _CHECK_AND_THROW_(m_cxt->match[group].rm_so < len && m_cxt->match[group].rm_eo <= len,
                                              "Replace error: regular expression match group failure!");
                            buff.append(str + m_cxt->match[group].rm_so, m_cxt->match[group].rm_eo - m_cxt->match[group].rm_so);
                        }*/
                    }
                    else
                    {
                        buff += *ptr;
                    }
                }
            }
            else
            {
                buff = text;
            }

            Storage::UndoGroup undoGroup(*pStorage);
//          pStorage->ReplaceLinePart(line, m_cxt->match[0].rm_so, m_cxt->match[0].rm_eo, buff.c_str(), buff.length());
            if(m_bRegExpr)
            {
              pStorage->ReplaceLinePart(line, m_regex.GetMatchPos(),m_regex.GetMatchEnd(),buff.GetString(),(int)buff.GetLength());
            }
            else
            {
              pStorage->ReplaceLinePart(line,start,end,buff.GetString(),(int)buff.GetLength());
            }
            end = start + buff.GetLength();
            return true;
        }
    }
    return false;
}

// 09.09.2002 improvement, find/replace batch performance
int  Searcher::DoBatch (Storage* _storage, LPCTSTR text, ESearchBatch mode, Square& last)
{
   int counter = 0;

    bool _bBackward = m_bBackward;
    m_bBackward = false;

    counted_ptr<Storage::UndoGroup> pUndoGroup;
    counted_ptr<Storage::NotificationDisabler> pDisabler;

    try
    {
        int line = 0, start = 0, end = 0;
        int firstOccurenceLine = -1, firstOccurencePos = -1;

        const Storage* storage1 = _storage, *storage2 = 0;

        while (storage1 && Find(storage1, line, start, end, false))
        {
            // 16.03.2003 bug fix, infinite cycle is possible on "Count" or "Mark all" in search all windows mode
            if (storage2                    // it's not first time
            && storage1 == _storage         // it's a start storage
            && firstOccurenceLine == line   // it's the first occurrence
            && firstOccurencePos == start)  // it's the first occurrence
            {
                break;
            }

            // a next storage
            if (storage1 != storage2)
            {
                // it's again the first storage!
                if (storage2 && storage1 == _storage)
                    break;

                if (mode == esbReplace)
                {
                  Storage* p = (Storage*)storage1;
                  pUndoGroup = counted_ptr<Storage::UndoGroup>(new Storage::UndoGroup(*p));
                }
                firstOccurenceLine = line;
                firstOccurencePos  = start;

                if (mode != esbCount)
                {
                    Storage* p = (Storage*) storage1;
                    pDisabler  = counted_ptr<Storage::NotificationDisabler>(
                        new Storage::NotificationDisabler(p, mode == esbReplace ? false : true)
                        );
                }
                storage2 = storage1;
            }

            switch (mode)
            {
            case esbReplace:
                {
                  Storage* p = (Storage*) storage1;
                  Replace(p, text, line, start, end);
                  if (_storage == storage1)
                  {
                      last.start.line = last.end.line = line;
                      last.start.column = start;
                      last.end.column = end;
                  }
                }
                break;
            case esbMark:
              { Storage* p = (Storage*)storage1;
                p->SetBookmark(line, eBmkGroup1, true);
              }
                break;
            }

            counter++;

            _CHECK_AND_THROW_(counter < 100000, 
                              _T("Infinite cycle or the number of occurences is too much (> 100000)! ")
                              _T("You can try to repeat it again."));
        }
    }
    catch(StdException& ex)
    {
      StyleMessageBox(nullptr,ex.GetErrorMessage(),_T("ERROR"),MB_OK | MB_ICONERROR);
    }
    catch (...)
    {
        m_bBackward = _bBackward;
        throw;
    }
    m_bBackward = _bBackward;

    return counter;
}

};
