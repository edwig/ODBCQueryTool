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
    13/05/2002 bug fix, undo operation completes with garbage on empty lines or following by them
*/

#include "pch.h"
#include "COMMON/StrHelpers.h"
#include "OEHelpers.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


namespace OpenEditor
{
    using namespace std;

    LPCTSTR DelimitersMap::m_cszDefDelimiters = _T(" \t\'\\()[]{}+-*/.,!?;:=><%|@&^");

    const _TUCHAR LineTokenizer::cbSpaceChar     =  32;
    const _TUCHAR LineTokenizer::cbVirtSpaceChar = 183;
    const _TUCHAR LineTokenizer::cbTabChar       = 187;


/////////////////////////////////////////////////////////////////////////////

DelimitersMap::DelimitersMap ()
{
}

DelimitersMap::DelimitersMap (LPCTSTR delimiters)
{
    Set(delimiters);
}

void DelimitersMap::Set (LPCTSTR str)
{
    CString delim;
    erase();

    for(; str && *str; str++)
    {
      operator[](*str) = (_TUCHAR) true;
    }
    // they're required
    operator[](' ')  = true;
    operator[]('\t') = true;
    operator[]('\n') = true;
    operator[]('\r') = true;
}

void DelimitersMap::Get (CString& buff)
{
    buff.Empty();
    for(_TUCHAR i = 0; i < size(); i++)
    {
      if((*this)[i])
      {
        buff += static_cast<TCHAR>(i);
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

LineTokenizer::LineTokenizer (bool showWhiteSpace, int tabSpacing, const DelimitersMap& delimiters)
:   m_delimiters(delimiters)
{
    m_processSpaces  =  false;
    m_showWhiteSpace = showWhiteSpace;

    if (m_showWhiteSpace)
    {
        m_spaceChar     = cbSpaceChar;
        m_virtSpaceChar = cbVirtSpaceChar;
        m_tabChar       = cbTabChar;
    }
    else
    {
        m_spaceChar     =
        m_virtSpaceChar =
        m_tabChar       = cbSpaceChar;
    }
   
    m_TabSpacing = tabSpacing;
    m_Buffer = 0;
    m_Length = m_Offset = m_Position = m_Position = 0;
}

void LineTokenizer::StartScan (LPCTSTR str, int len)
{
    m_Buffer = str;
    m_Length = len;
    m_Offset = m_Position = 0;

    if (!(m_showWhiteSpace || m_processSpaces))
    {
        // skip white space & expand tabs
        for (; m_Offset < m_Length && isspace(m_Buffer[m_Offset]); m_Offset++)
        {
            if (m_Buffer[m_Offset] == '\t')
                m_Position = (m_Position / m_TabSpacing + 1) * m_TabSpacing;
            else
                m_Position++;
        }
    }
}

bool LineTokenizer::Next ()
{
    bool isSpace = isspace(m_Buffer[m_Offset]) ? true : false;

    if (!isSpace)                       // it's world
    {
        if (m_Offset < m_Length && m_delimiters[m_Buffer[m_Offset]])
        {
            m_Offset++;
            m_Position++;
        }
        else
            for (; m_Offset < m_Length && !m_delimiters[m_Buffer[m_Offset]]; m_Offset++)
                m_Position++;
    }
    if (isSpace || !(m_showWhiteSpace || m_processSpaces))  // it's space
    {
        for (; m_Offset < m_Length && isspace(m_Buffer[m_Offset]); m_Offset++)
        {
            if (m_Buffer[m_Offset] == '\t')
                m_Position = (m_Position / m_TabSpacing + 1) * m_TabSpacing;
            else
                m_Position++;
        }
    }
    return !Eol();
}

void LineTokenizer::GetCurentWord (LPCTSTR& str, int& pos, int& len) const
{
  if (!_istspace(m_Buffer[m_Offset]))   // it's world
  {
    if (m_delimiters[m_Buffer[m_Offset]])
        len = 1;
    else
        // skip word
        for (len = 0; m_Offset + len < m_Length
                      && !m_delimiters[m_Buffer[m_Offset + len]]; len++)
            ;

    str = m_Buffer + m_Offset;
    pos = m_Position;
  }
  else                                // it's space
  {
    m_Whitespaces.Empty();

    for (int offset(m_Offset), position(m_Position); offset < m_Length && isspace(m_Buffer[offset]); offset++)
    {
      if (m_Buffer[offset] == '\t')
      {
        position = (position / m_TabSpacing + 1) * m_TabSpacing;
        m_Whitespaces += m_tabChar;

        //m_Whitespaces.resize(position - m_Position, m_spaceChar);
        m_Whitespaces = m_Whitespaces.Left(position - m_Position);
        while((position - m_Position) > m_Whitespaces.GetLength())
        {
          TCHAR space[2] = { (TCHAR)m_spaceChar, 0};
          m_Whitespaces.Append(space);
        }
      }
      else
      {
        position++;
        m_Whitespaces += m_virtSpaceChar;
      }
    }
    str = m_Whitespaces.GetString();
    len = m_Whitespaces.GetLength();
    pos = m_Position;
  }
}

// Special LineTokenizer
// eats words and strings alike
void LineTokenizer::GetCurrentSQLToken(LPCTSTR& str, int& pos, int& len,TCHAR& instring) 
{
  if(instring)
  {
    len = 0;
    pos = m_Position;
    str = m_Buffer + m_Offset;
    bool mayReturn = false;
    if(m_Offset < m_Length)
    {
      do
      {
        if(m_Buffer[m_Offset + len] == instring)
        {
          mayReturn = !mayReturn;
          if(!(m_Buffer[m_Offset + len - 1] == '\\' && len>0) &&
              (m_Buffer[m_Offset + len + 1] != instring))
          {
            if(mayReturn)
            {
              instring = '\0';
              // .Next must eat the last delimiter first, so offset/position 
              // ends ON the last delimiter and not after it!
              m_Offset   += len;
              m_Position += len;
              ++len;
              return;
            }
          }
        }
        ++len;
      }
      while(m_Offset + len < m_Length);
    }
    return;
  }
  GetCurentWord(str, pos, len);
  if(len == 1)
  {
    if(*str == '\'' || *str == '\"')
    {
      // Begin of a String
      // \' is a metachar for various ODBC sources   (Microsoft)
      // '' is a metachar for various SQL  databases (Oracle, Informix)
      // "" is a metachar for various SQL  databases (Informix)
      LPCTSTR delim = str;
      instring = *delim;
      bool mayReturn = false;
      do
      {
        ++len;
        if(m_Buffer[m_Offset + len] == *delim)
        {
          mayReturn = !mayReturn;
          if((m_Buffer[m_Offset + len - 1] != '\\') &&
             (m_Buffer[m_Offset + len + 1] != *delim))
          {
            if(mayReturn)
            {
              instring = '\0';
              // .Next must eat the last delimiter first, so offset/position 
              // ends ON the last delimiter and not after it!
              m_Offset   += len;
              m_Position += len;
              ++len;
              return;
            }
          }
        }
      }
      while(m_Offset + len < m_Length);
    }
  }
}

}
