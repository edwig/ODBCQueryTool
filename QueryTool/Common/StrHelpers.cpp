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
    31.03.2003 bug fix, "Month" is not recognized as a valid token for date conversion
*/

#include "pch.h"
#include <string>
#include <vector>
#include "COMMON\StrHelpers.h"
#include "COMMON\Fastmap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace Common 
{
    void to_printable_str (LPCTSTR from, CString& _to)
    {
        CString to;

        for (; from && *from; from++)
        {
            switch (*from)
            {
            case '\a': to += _T("\\a");  break; // \a Bell (alert) 
            case '\b': to += _T("\\b");  break; // \b Backspace 
            case '\f': to += _T("\\f");  break; // \f Form Feed 
            case '\n': to += _T("\\n");  break; // \n New line 
            case '\r': to += _T("\\r");  break; // \r Carriage return 
            case '\t': to += _T("\\t");  break; // \t Horizontal tab 
            case '\v': to += _T("\\v");  break; // \v Vertical tab 
//          case '\'': to += _T("\'");   break; // \' Single quotation mark 
//          case '\"': to += _T("\"");   break; // \" Double quotation mark 
            case '\\': to += _T("\\\\"); break; // \\ Backslash 
//          case '\?': to += _T("\?");   break; // \? Literal question mark 
            default:
                if (isprint(*from))
                    to += (TCHAR)*from;
                else // \ddd ASCII character in decimal notation
                {
                    TCHAR buff[20];
                    to += "\\";
                    _itot_s(*from,buff,20, 10);
                    for(int j(0),len(3 - (int) _tcslen(buff)); j < len; j++)
                    {
                      to += '0';
                    }
                    to += buff;
                }
            }
        }
        _to = to;
    }

    void to_unprintable_str (LPCTSTR from, CString& _to, bool skipEscDgt)
    {
        CString to;

        for (; *from; from++)
        {
            _TUCHAR ch = *from;

            if (*from == '\\')
            {
                switch (from[1])
                {
                case 'a':  ch = '\a'; from++; break; // \a Bell (alert) 
                case 'b':  ch = '\b'; from++; break; // \b Backspace 
                case 'f':  ch = '\f'; from++; break; // \f Formfeed 
                case 'n':  ch = '\n'; from++; break; // \n New line 
                case 'r':  ch = '\r'; from++; break; // \r Carriage return 
                case 't':  ch = '\t'; from++; break; // \t Horizontal tab 
                case 'v':  ch = '\v'; from++; break; // \v Vertical tab 
                case '\'': ch = '\''; from++; break; // \' Single quotation mark 
                case '\"': ch = '\"'; from++; break; // \" Double quotation mark 
                case '\\': ch = '\\'; from++; break; // \\ Backslash 
                case '?':  ch = '\?'; from++; break; // \? Literal question mark 
                default:
                    if (!skipEscDgt)
                    {
                        TCHAR digits[4];
                        int j;
                        for(j = 0; j < 3 && isdigit(from[j + 1]); j++)
                        {
                          digits[j] = from[j + 1];
                        }
                
                        if (j > 0)
                        {
                            digits[j] = 0;
                            ch = static_cast<TCHAR>(static_cast<unsigned>(_ttoi(digits)));
                            from += j; 
                        }
                        else
                        {
                          _ASSERTE(0);
                        }
                    }
                }
            }
            to += ch;
        }
        _to = to;
    }

    void trim_symmetric (CString& str, LPCTSTR skip)
    {
      if(!str.IsEmpty())
      {
        str = str.Trim(skip);
      }
    }

    /////////////////////////////////////////////////////////////////////////

    void Substitutor::ResetContent ()
    {
        m_result.Empty();
        m_whatFind.clear(); 
        m_withReplace.clear();
    }

    void Substitutor::AddPair (LPCTSTR find, LPCTSTR replace)
    {
        m_whatFind.push_back(find); 
        m_withReplace.push_back(replace);
    }

    void Substitutor::AddPair (const CString& find, const CString& replace)
    {
        m_whatFind.push_back(find); 
        m_withReplace.push_back(replace);
    }

    Substitutor& Substitutor::operator << (LPCTSTR input)
    {
        Fastmap<bool> fast_map;

        std::vector<CString>::const_iterator 
            it(m_whatFind.begin()), end(m_whatFind.end());

        if (m_casesensitive)
        {
          for(; it != end; it++)
          {
            fast_map[(int) (*it).GetAt(0)] = true;
          }
        }
        else
        {
          for(; it != end; it++)
          {
            fast_map[(int) toupper((*it).GetAt(0))] = true;
          }
        }

        LPCTSTR chunk_begin = input;
        LPCTSTR chunk_end   = input;

        while (*chunk_end)
        {
            if (m_casesensitive  && fast_map[(int)*chunk_end]
            || (!m_casesensitive && fast_map[(int)toupper(*chunk_end)]))
            {
                bool hit = false;
                it = m_whatFind.begin();

                int i;
                for (i = 0; it != end; it++, i++) 
                {
                    const CString& str = (*it);

                    if ((m_casesensitive && !_tcsncmp (chunk_end, str.GetString(), str.GetLength()))
                    || (!m_casesensitive && !_tcsnicmp(chunk_end, str.GetString(), str.GetLength())))
                    {
                        hit = true;
                        break;
                    }
                }

                if (hit)
                {
                    m_result.Append(chunk_begin, (int)(chunk_end - chunk_begin));
                    m_result.Append(m_withReplace[i].GetString(), m_withReplace[i].GetLength());
                    chunk_end  += m_whatFind[i].GetLength();
                    chunk_begin = chunk_end;
                    continue;
                }
            }
            chunk_end++;
        }

        m_result.Append(chunk_begin);

        return *this;
    }

    void date_c_to_oracle (LPCTSTR from,CString& to)
    {
        Common::Substitutor subst;
        subst.AddPair(_T("%d"), _T("dd"  ));
        subst.AddPair(_T("%m"), _T("mm"  ));
        subst.AddPair(_T("%B"), _T("month"));
        subst.AddPair(_T("%b"), _T("mon" ));
        subst.AddPair(_T("%Y"), _T("yyyy"));
        subst.AddPair(_T("%y"), _T("yy"  ));
        subst.AddPair(_T("%H"), _T("hh24"));
        subst.AddPair(_T("%I"), _T("hh12"));
        subst.AddPair(_T("%p"), _T("am"  ));
        subst.AddPair(_T("%M"), _T("mi"  ));
        subst.AddPair(_T("%S"), _T("ss"  ));
        subst << from;
        to = subst.GetResult();
    }

    void date_oracle_to_c (LPCTSTR from, CString& to)
    {
        Common::Substitutor subst(false);
        subst.AddPair(_T("dd")  , _T("%d"));
        subst.AddPair(_T("mm")  , _T("%m"));
        subst.AddPair(_T("month"),_T("%B"));
        subst.AddPair(_T("mon") , _T("%b"));
        subst.AddPair(_T("yyyy"), _T("%Y"));
        subst.AddPair(_T("yy")  , _T("%y"));
        subst.AddPair(_T("rrrr"), _T("%Y"));
        subst.AddPair(_T("rr")  , _T("%y"));
        subst.AddPair(_T("hh24"), _T("%H"));
        subst.AddPair(_T("hh12"), _T("%I"));
        subst.AddPair(_T("am")  , _T("%p"));
        subst.AddPair(_T("mi")  , _T("%M"));
        subst.AddPair(_T("ss")  , _T("%S"));
        subst << from;
        to = subst.GetResult();
    }

    void to_upper_str (LPCTSTR from, CString& _to)
    {
        CString to;
        for(LPCTSTR ptr = from; ptr && *ptr; ptr++)
        {
          to += (TCHAR) toupper(*ptr);
        }
        _to = to;
    }

    void to_lower_str (LPCTSTR from, CString& _to)
    {
        CString to;
        for(LPCTSTR ptr = from; ptr && *ptr; ptr++)
        {
          to += (TCHAR) tolower(*ptr);
        }
        _to = to;
    }
}// END namespace Common
