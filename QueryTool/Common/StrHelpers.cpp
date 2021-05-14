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
    using std::string;
    
    void to_printable_str (const char* from, string& _to)
    {
        string to;

        for (; from && *from; from++)
        {
            switch (*from)
            {
            case '\a': to += "\\a";  break; // \a Bell (alert) 
            case '\b': to += "\\b";  break; // \b Backspace 
            case '\f': to += "\\f";  break; // \f Formfeed 
            case '\n': to += "\\n";  break; // \n New line 
            case '\r': to += "\\r";  break; // \r Carriage return 
            case '\t': to += "\\t";  break; // \t Horizontal tab 
            case '\v': to += "\\v";  break; // \v Vertical tab 
//            case '\'': to += "\'";   break; // \' Single quotation mark 
//            case '\"': to += "\"";   break; // \" Double quotation mark 
            case '\\': to += "\\\\"; break; // \\ Backslash 
//            case '\?': to += "\?";   break; // \? Literal question mark 
            default:
                if (isprint(*from))
                    to += (char)*from;
                else // \ddd ASCII character in decimal notation
                {
                    char buff[20];
                    to += "\\";
                    _itoa_s(*from,buff,20, 10);
                    for (int j(0), len(3 - (int)strlen(buff)); j < len; j++)
                        to += '0';
                    to += buff;
                }
            }
        }
        _to = to;
    }

    void to_unprintable_str (const char* from, string& _to, bool skipEscDgt)
    {
        string to;

        for (; *from; from++)
        {
            unsigned char ch = *from;

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
                        char digits[4];
                        int j;
                        for (j = 0; j < 3 && isdigit(from[j+1]); j++)
                            digits[j] = from[j+1];
                
                        if (j > 0)
                        {
                            digits[j] = 0;
                            ch = static_cast<char>(static_cast<unsigned>(atoi(digits)));
                            from += j; 
                        }
                        else
                            _ASSERTE(0);
                    }
                }
            }
            to += ch;
        }
        _to = to;
    }

    void trim_symmetric (string& str, const char* skip)
    {
        if (!str.empty()) {
            std::string::size_type beg = str.find_first_not_of(skip);
            std::string::size_type end = str.find_last_not_of(skip);

            if (beg != std::string::npos || end != std::string::npos) {
                if (string::npos == beg) beg = 0;
                if (string::npos == end) end = str.size();
                str = str.substr(beg, end - beg + 1);
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////

    void Substitutor::ResetContent ()
    {
        m_result.erase();
        m_whatFind.clear(); 
        m_withReplace.clear();
    }

    void Substitutor::AddPair (const char* find, const char* replace)
    {
        m_whatFind.push_back(find); 
        m_withReplace.push_back(replace);
    }

    void Substitutor::AddPair (const string& find, const string& replace)
    {
        m_whatFind.push_back(find); 
        m_withReplace.push_back(replace);
    }

    Substitutor& Substitutor::operator << (const char* input)
    {
        Fastmap<bool> fast_map;

        std::vector<string>::const_iterator 
            it(m_whatFind.begin()), end(m_whatFind.end());

        if (m_casesensitive)
        {
            for (; it != end; it++)
                fast_map[(*it)[0]] = true;
        }
        else
        {
            for (; it != end; it++)
                fast_map[static_cast<unsigned char>(toupper((*it)[0]))] = true;
        }

        const char* chunk_begin = input;
        const char* chunk_end   = input;

        while (*chunk_end)
        {
            if (m_casesensitive && fast_map[*chunk_end]
            || (!m_casesensitive && fast_map[static_cast<unsigned char>(toupper(*chunk_end))])) 
            {
                bool hit = false;
                it = m_whatFind.begin();

                int i;
                for (i = 0; it != end; it++, i++) 
                {
                    const string& str = (*it);

                    if ((m_casesensitive && !strncmp(chunk_end, str.c_str(), str.size()))
                    || (!m_casesensitive && !_strnicmp(chunk_end, str.c_str(), str.size())))
                    {
                        hit = true;
                        break;
                    }
                }

                if (hit)
                {
                    m_result.append(chunk_begin, chunk_end - chunk_begin);
                    m_result.append(m_withReplace[i].c_str(), m_withReplace[i].size());
                    chunk_end  += m_whatFind[i].size();
                    chunk_begin = chunk_end;
                    continue;
                }
            }
            chunk_end++;
        }

        m_result.append(chunk_begin);

        return *this;
    }

    void date_c_to_oracle (const char* from, string& to)
    {
        Common::Substitutor subst;
        subst.AddPair("%d", "dd"  );
        subst.AddPair("%m", "mm"  );
        subst.AddPair("%B", "month" );
        subst.AddPair("%b", "mon" );
        subst.AddPair("%Y", "yyyy");
        subst.AddPair("%y", "yy"  );
        subst.AddPair("%H", "hh24");
        subst.AddPair("%I", "hh12");
        subst.AddPair("%p", "am"  );
        subst.AddPair("%M", "mi"  );
        subst.AddPair("%S", "ss"  );
        subst << from;
        to = subst.GetResult();
    }

    void date_oracle_to_c (const char* from, string& to)
    {
        Common::Substitutor subst(false);
        subst.AddPair("dd"  , "%d");
        subst.AddPair("mm"  , "%m");
         // 31.03.2003 bug fix, "Month" is not recognized as a valid token for date conversion
        subst.AddPair("month","%B");
        subst.AddPair("mon" , "%b");
        subst.AddPair("yyyy", "%Y");
        subst.AddPair("yy"  , "%y");
        subst.AddPair("rrrr", "%Y");
        subst.AddPair("rr"  , "%y");
        subst.AddPair("hh24", "%H");
        subst.AddPair("hh12", "%I");
        subst.AddPair("am"  , "%p");
        subst.AddPair("mi"  , "%M");
        subst.AddPair("ss"  , "%S");
        subst << from;
        to = subst.GetResult();
    }

    void to_upper_str (const char* from, string& _to)
    {
        string to;
        for (const char* ptr = from; ptr && *ptr; ptr++)
            to += toupper(*ptr);
        _to = to;
    }

    void to_lower_str (const char* from, string& _to)
    {
        string to;
        for (const char* ptr = from; ptr && *ptr; ptr++)
            to += tolower(*ptr);
        _to = to;
    }
}// END namespace Common
