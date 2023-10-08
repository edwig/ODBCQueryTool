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
  10.03.2003 bug fix, start-line comment (promt & remark) should be separeted by delimiter from text
*/

#include "pch.h"
#include "OEHelpers.h"
#include "OEStorage.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


namespace OpenEditor
{
    using namespace std;

MultilineQuotesScanner::MultilineQuotesScanner (Storage& storage)
                       :m_Storage(storage)
{
    m_nQuotesValidLimit   = 0;
    m_bCaseSensitive      = false;
    m_OpeningFastMapDirty = true;
    m_bParsingAlways      = true;
}

bool MultilineQuotesScanner::IsCaseSensitive () const
{
  return m_bCaseSensitive;
}

void MultilineQuotesScanner::SetCaseSensitive (bool flag)
{
    m_bCaseSensitive = flag;
    m_OpeningFastMapDirty = true;
}

void MultilineQuotesScanner::ClearSettings ()
{
    // reset scanned quotes too, because it depends on settings
    m_mapQuotes.clear();
    m_nQuotesValidLimit = 0;

    m_StartLineQuotes.clear();
    m_SingleLineQuotes.clear();
    m_MultilineQuotes.clear();
    m_escapeChar.Empty();
    m_OpeningFastMapDirty = true;
    m_bParsingAlways = true;
    m_ParsingQuotes.first.Empty();
    m_ParsingQuotes.second.Empty();
//    m_pDelimiters = 0;
}

void MultilineQuotesScanner::SetParsingQuotes (const pair<CString, CString>& _pair)
{
    m_bParsingAlways = false;
    m_ParsingQuotes = _pair;
    m_OpeningFastMapDirty = true;
}

int MultilineQuotesScanner::AddStartLineQuotes (const CString& str)
{
    _ASSERTE(str.GetLength());
    m_StartLineQuotes.push_back(str);
    m_OpeningFastMapDirty = true;
    return (int)m_StartLineQuotes.size() - 1;
}

int MultilineQuotesScanner::AddSingleLineQuotes (const CString& str)
{
    _ASSERTE(str.GetLength());
    m_SingleLineQuotes.push_back(str);
    m_OpeningFastMapDirty = true;
    return (int)m_SingleLineQuotes.size() - 1;
}

int MultilineQuotesScanner::AddMultilineQuotes (const pair<CString, CString>& _pair)
{
    _ASSERTE(_pair.first.GetLength() && _pair.second.GetLength());
    m_MultilineQuotes.push_back(_pair);
    m_OpeningFastMapDirty = true;
    return (int)m_MultilineQuotes.size() - 1;
}

void MultilineQuotesScanner::AddEscapeChar (const CString& esc)
{
    m_escapeChar = esc;
}

void MultilineQuotesScanner::SetDelimitersMap (const DelimitersMap& delimiters)
{
    m_delimiters = delimiters;
}

void MultilineQuotesScanner::Scan (int to_line, int& state, int& quoteId, bool& parsing)
{
  if(to_line > m_nQuotesValidLimit)
  {
    buildMap(to_line + 100);
  }
    map<int,Entry>::const_iterator
        it  = m_mapQuotes.begin(),
        end = m_mapQuotes.end();

    for (; it != end && it->first < to_line; it++)
    {
        state   = it->second.state;
        quoteId = it->second.quoteId;
        parsing = it->second.parsing;
    }
}

bool MultilineQuotesScanner::ScanLine (LPCTSTR str, int len, int& state, int& quoteId, bool& parsing, int* parsingLength) const
{
    int  quotesBalance = 0;
    int  parsingBalance = 0;
    bool isStartSpace = true;
    bool ignoreRemainder = false;

    TCHAR closingFastChr = (state) ? m_MultilineQuotes.at(quoteId).second.GetAt(0) : '\0';

    if (m_OpeningFastMapDirty) buildOpeningFastMap();

    int i;
    for (i = 0; i < len; i++)
    {
        TCHAR ch = str[i];
        TCHAR probe = m_OpeningFastMap[ch];

        if (!m_bParsingAlways && !parsing)
        {
            if ((probe & eqtOpeningParse) && is_equal(str, len, i, m_ParsingQuotes.first))
            {
                parsing = true;
                parsingBalance++;
            }
            else
                continue;
        }

        if (!state)
        {
            if (isStartSpace && probe & eqtStartLine)
            {
                vector<CString>::const_iterator
                    it(m_StartLineQuotes.begin()), end(m_StartLineQuotes.end());

                for (; it != end; it++)
                    if (is_equal(str, len, i, it->GetString()))
                    {
                        ignoreRemainder = true;
                        break;
                    }

                if (ignoreRemainder) break;
            }

            if (probe & eqtSingleLine)
            {
                vector<CString>::const_iterator
                    it(m_SingleLineQuotes.begin()), end(m_SingleLineQuotes.end());

                for (; it != end; it++)
                    if (is_equal(str, len, i, *it))
                    {
                        ignoreRemainder = true;
                        break;
                    }

                if (ignoreRemainder) break;
            }

            if (probe & eqtOpeningMultiline)
            {
                vector<std::pair<CString, CString> >::const_iterator
                    it(m_MultilineQuotes.begin()), end(m_MultilineQuotes.end());

                for (int j(0); it != end; it++, j++)
                    if (is_equal(str, len, i, it->first))
                    {
                        state   = 1;
                        quoteId = j;
                        closingFastChr = m_MultilineQuotes.at(quoteId).second.GetAt(0);
                        quotesBalance++;
                        break;
                    }
            }
        }
        else if (ch == closingFastChr)
        {
            if (is_equal(str, len, i, m_MultilineQuotes[quoteId].second)
                && (!i
                    // escape is not defined or the previous fragment is not escape or it is not double escape
                    || m_escapeChar.IsEmpty()
                    // the current position < escape size
                    || i < static_cast<int>(m_escapeChar.GetLength())
                    // the previous fragment is not escape
                    || _tcsncmp(m_escapeChar.GetString(), str + i - m_escapeChar.GetLength(), m_escapeChar.GetLength())
                    // it is double escape
                    || (
                        // the current position >= 2 * escape size
                        i >= 2 * static_cast<int>(m_escapeChar.GetLength())
                        // the previous previous fragment is escape
                        && !_tcsncmp(m_escapeChar.GetString(), str + i - 2 * m_escapeChar.GetLength(), m_escapeChar.GetLength())
                    )
                )
            )
            {
                state = 0;
                quotesBalance--;
            }
        }

        if (isStartSpace && !isspace(ch))
            isStartSpace = false;

        if (!m_bParsingAlways && !state 
        && (probe & eqtClosingParse) && is_equal(str, len, i, m_ParsingQuotes.second))
        {
            parsing = false;
            parsingBalance--;
            state = quoteId = 0;
        }
    }

    if (parsingLength)
        *parsingLength = ignoreRemainder ? i : len;

    return quotesBalance || parsingBalance ? true : false;
}

void MultilineQuotesScanner::buildMap (int to_line)
{
    m_mapQuotes.erase(m_mapQuotes.lower_bound(m_nQuotesValidLimit), m_mapQuotes.end());

    if (m_OpeningFastMapDirty) buildOpeningFastMap();

    int state(0), quoteId(0);
    bool parsing(m_bParsingAlways);
    Scan(m_nQuotesValidLimit, state, quoteId, parsing);

    to_line = min(to_line, m_Storage.GetLineCount());
    for (int line = m_nQuotesValidLimit; line < to_line; line++)
    {
        int length;
        LPCTSTR str;
        m_Storage.GetLine(line, str, length);

        if (ScanLine(str, length, state, quoteId, parsing))
            m_mapQuotes.insert(map<int,Entry>::value_type(line, Entry(state, quoteId, parsing)));
    }

    m_nQuotesValidLimit = to_line;
}

#pragma warning (disable: 4996)

void MultilineQuotesScanner::buildOpeningFastMap () const
{
    m_OpeningFastMap.erase();

    if (m_ParsingQuotes.first.GetLength())
        m_OpeningFastMap[m_ParsingQuotes.first.GetAt(0)] |= eqtOpeningParse;
    if (m_ParsingQuotes.second.GetLength())
        m_OpeningFastMap[m_ParsingQuotes.second.GetAt(0)] |= eqtClosingParse;

    {
        vector<CString>::const_iterator
            it(m_StartLineQuotes.begin()), end(m_StartLineQuotes.end());
        for (; it != end; it++)
        {
            if (m_bCaseSensitive)
                m_OpeningFastMap[it->GetAt(0)] |= eqtStartLine;
            else
            {
                TCHAR str[2];
                str[0] = it->GetAt(0);
                str[1] = 0;
                m_OpeningFastMap[*_tcsupr(str)] |= eqtStartLine;
                m_OpeningFastMap[*_tcslwr(str)] |= eqtStartLine;
            }
        }
    }

    {
        vector<CString>::const_iterator
            it(m_SingleLineQuotes.begin()), end(m_SingleLineQuotes.end());
        for (; it != end; it++)
            if (m_bCaseSensitive)
                m_OpeningFastMap[it->GetAt(0)] |= eqtSingleLine;
            else
            {
                TCHAR str[2];
                str[0] = it->GetAt(0);
                str[1] = 0;
                m_OpeningFastMap[*_tcsupr(str)] |= eqtSingleLine;
                m_OpeningFastMap[*_tcslwr(str)] |= eqtSingleLine;
            }
    }

    {
        vector<std::pair<CString, CString> >::const_iterator
            it(m_MultilineQuotes.begin()), end(m_MultilineQuotes.end());
        for (; it != end; it++)
            if (m_bCaseSensitive)
                m_OpeningFastMap[it->first.GetAt(0)] |= eqtOpeningMultiline;
            else
            {
                TCHAR str[2];
                str[0] = it->first.GetAt(0);
                str[1] = 0;
                m_OpeningFastMap[*_tcsupr(str)] |= eqtOpeningMultiline;
                m_OpeningFastMap[*_tcslwr(str)] |= eqtOpeningMultiline;
            }
    }

    m_OpeningFastMapDirty = false;
}

bool MultilineQuotesScanner::is_equal (LPCTSTR str, int len, int offset, const CString& shape) const
{
    bool retVal = false;

    if ((len - offset) >= shape.GetLength())
    {
      if(m_bCaseSensitive)
      {
        retVal = !_tcsncmp(str + offset,shape.GetString(),shape.GetLength()) ? true : false;
      }
      else
      {
        retVal = !_tcsnicmp(str + offset,shape.GetString(),shape.GetLength()) ? true : false;
      }

        // check begin of fragment
        if (retVal && !m_delimiters[shape.GetAt(0)]
        && offset > 0 && !m_delimiters[str[offset - 1]])
            retVal = false;

        // check end of fragment
        // 10.03.2003 bug fix, start-line comment (prompt & remark) should be separated by delimiter from text
        if (retVal && !m_delimiters[shape.GetAt(shape.GetLength() - 1)]      // last TCHAR is not delimiter
        && offset + shape.GetLength() 
            < len && str[offset + shape.GetLength()]                         // there is a TCHAR after keyword 
        && !m_delimiters[str[offset + shape.GetLength()]])                   // this TCHAR is not delimiter
            retVal = false;
    }

    return retVal;
}

};
