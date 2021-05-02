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

#include "stdafx.h"
#include "OpenEditor/OEHelpers.h"
#include "OpenEditor/OEStorage.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


namespace OpenEditor
{
    using namespace std;

MultilineQuotesScanner::MultilineQuotesScanner (Storage& storage)
: m_Storage(storage)
{
    m_nQuotesValidLimit = 0;
//    m_pDelimiters = 0;
    m_bCaseSensitive = false;
    m_OpeningFastMapDirty = true;
    m_bParsingAlways = true;
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
    m_escapeChar.erase();
    m_OpeningFastMapDirty = true;
    m_bParsingAlways = true;
    m_ParsingQuotes.first.erase();
    m_ParsingQuotes.second.erase();
//    m_pDelimiters = 0;
}

void MultilineQuotesScanner::SetParsingQuotes (const pair<string, string>& _pair)
{
    m_bParsingAlways = false;
    m_ParsingQuotes = _pair;
    m_OpeningFastMapDirty = true;
}

int MultilineQuotesScanner::AddStartLineQuotes (const string& str)
{
    _ASSERTE(str.size());
    m_StartLineQuotes.push_back(str);
    m_OpeningFastMapDirty = true;
    return (int)m_StartLineQuotes.size() - 1;
}

int MultilineQuotesScanner::AddSingleLineQuotes (const string& str)
{
    _ASSERTE(str.size());
    m_SingleLineQuotes.push_back(str);
    m_OpeningFastMapDirty = true;
    return (int)m_SingleLineQuotes.size() - 1;
}

int MultilineQuotesScanner::AddMultilineQuotes (const pair<string, string>& _pair)
{
    _ASSERTE(_pair.first.size() && _pair.second.size());
    m_MultilineQuotes.push_back(_pair);
    m_OpeningFastMapDirty = true;
    return (int)m_MultilineQuotes.size() - 1;
}

void MultilineQuotesScanner::AddEscapeChar (const string& esc)
{
    m_escapeChar = esc;
}

void MultilineQuotesScanner::SetDelimitersMap (const DelimitersMap& delimiters)
{
    m_delimiters = delimiters;
}

void MultilineQuotesScanner::Scan (int to_line, int& state, int& quoteId, bool& parsing)
{
    if (to_line > m_nQuotesValidLimit)
        buildMap(to_line + 100);

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

bool MultilineQuotesScanner::ScanLine (const char* str, int len, int& state, int& quoteId, bool& parsing, int* parsingLength) const
{
    int  quotesBalance = 0;
    int  parsingBalance = 0;
    bool isStartSpace = true;
    bool ignoreRemainder = false;

    char closingFastChr = (state) ? m_MultilineQuotes.at(quoteId).second.at(0) : '\0';

    if (m_OpeningFastMapDirty) buildOpeningFastMap();

    int i;
    for (i = 0; i < len; i++)
    {
        char ch = str[i];
        char probe = m_OpeningFastMap[ch];

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
                vector<string>::const_iterator
                    it(m_StartLineQuotes.begin()), end(m_StartLineQuotes.end());

                for (; it != end; it++)
                    if (is_equal(str, len, i, *it))
                    {
                        ignoreRemainder = true;
                        break;
                    }

                if (ignoreRemainder) break;
            }

            if (probe & eqtSingleLine)
            {
                vector<string>::const_iterator
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
                vector<std::pair<string, string> >::const_iterator
                    it(m_MultilineQuotes.begin()), end(m_MultilineQuotes.end());

                for (int j(0); it != end; it++, j++)
                    if (is_equal(str, len, i, it->first))
                    {
                        state   = 1;
                        quoteId = j;
                        closingFastChr = m_MultilineQuotes.at(quoteId).second.at(0);
                        quotesBalance++;
                        break;
                    }
            }
        }
        else if (ch == closingFastChr)
        {
            if (is_equal(str, len, i, m_MultilineQuotes[quoteId].second)
                && (!i
                    // escape is not defined or the previos fragment is not escape or it is not double escape
                    || m_escapeChar.empty()
                    // the current position < escape size
                    || i < static_cast<int>(m_escapeChar.size())
                    // the previous fragment is not escape
                    || strncmp(m_escapeChar.c_str(), str + i - m_escapeChar.size(), m_escapeChar.size())
                    // it is double escape
                    || (
                        // the current position >= 2 * escape size
                        i >= 2 * static_cast<int>(m_escapeChar.size())
                        // the previous previous fragment is escape
                        && !strncmp(m_escapeChar.c_str(), str + i - 2 * m_escapeChar.size(), m_escapeChar.size())
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
        const char* str;
        m_Storage.GetLine(line, str, length);

        if (ScanLine(str, length, state, quoteId, parsing))
            m_mapQuotes.insert(map<int,Entry>::value_type(line, Entry(state, quoteId, parsing)));
    }

    m_nQuotesValidLimit = to_line;
}

void MultilineQuotesScanner::buildOpeningFastMap () const
{
    m_OpeningFastMap.erase();

    if (m_ParsingQuotes.first.size())
        m_OpeningFastMap[m_ParsingQuotes.first.at(0)] |= eqtOpeningParse;
    if (m_ParsingQuotes.second.size())
        m_OpeningFastMap[m_ParsingQuotes.second.at(0)] |= eqtClosingParse;

    {
        vector<string>::const_iterator
            it(m_StartLineQuotes.begin()), end(m_StartLineQuotes.end());
        for (; it != end; it++)
        {
            if (m_bCaseSensitive)
                m_OpeningFastMap[it->at(0)] |= eqtStartLine;
            else
            {
                char str[2];
                str[0] = it->at(0);
                str[1] = 0;
                m_OpeningFastMap[*strupr(str)] |= eqtStartLine;
                m_OpeningFastMap[*strlwr(str)] |= eqtStartLine;
            }
        }
    }

    {
        vector<string>::const_iterator
            it(m_SingleLineQuotes.begin()), end(m_SingleLineQuotes.end());
        for (; it != end; it++)
            if (m_bCaseSensitive)
                m_OpeningFastMap[it->at(0)] |= eqtSingleLine;
            else
            {
                char str[2];
                str[0] = it->at(0);
                str[1] = 0;
                m_OpeningFastMap[*strupr(str)] |= eqtSingleLine;
                m_OpeningFastMap[*strlwr(str)] |= eqtSingleLine;
            }
    }

    {
        vector<std::pair<string, string> >::const_iterator
            it(m_MultilineQuotes.begin()), end(m_MultilineQuotes.end());
        for (; it != end; it++)
            if (m_bCaseSensitive)
                m_OpeningFastMap[it->first.at(0)] |= eqtOpeningMultiline;
            else
            {
                char str[2];
                str[0] = it->first.at(0);
                str[1] = 0;
                m_OpeningFastMap[*strupr(str)] |= eqtOpeningMultiline;
                m_OpeningFastMap[*strlwr(str)] |= eqtOpeningMultiline;
            }
    }

    m_OpeningFastMapDirty = false;
}

bool MultilineQuotesScanner::is_equal (const char* str, int len, int offset, const string& shape) const
{
    bool retVal = false;

    if ((len - offset) >= static_cast<int>(shape.length()))
    {
        if (m_bCaseSensitive)
            retVal = !strncmp(str + offset, shape.c_str(), shape.size()) ? true : false;
        else
            retVal = !strnicmp(str + offset, shape.c_str(), shape.size()) ? true : false;

        // check begin of fragment
        if (retVal && !m_delimiters[*shape.begin()]
        && offset > 0 && !m_delimiters[str[offset - 1]])
            retVal = false;

        // check end of fragment
        // 10.03.2003 bug fix, start-line comment (promt & remark) should be separeted by delimiter from text
        if (retVal && !m_delimiters[*shape.rbegin()]                    // last char is not delim
        && offset + static_cast<int>(shape.size()) 
            < len && str[offset + shape.size()]                         // there is a char after keyword 
        && !m_delimiters[str[offset + shape.size()]])                   // this char is not delim
            retVal = false;
    }

    return retVal;
}

};
