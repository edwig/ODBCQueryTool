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
#include "OEHighlighter.h"
#include "COMMON/VisualAttributes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace OpenEditor
{

HighlighterBasePtr HighlighterFactory::CreateHighlighter (LPCTSTR name)
{
    if (!_tcscmp(name, _T("NONE")))  return HighlighterBasePtr();
    if (!_tcscmp(name, _T("SQL")))   return HighlighterBasePtr(new PlSqlHighlighter);
    if (!_tcscmp(name, _T("C++")))   return HighlighterBasePtr(new CPlusPlusHighlighter);
    if (!_tcscmp(name, _T("Shell"))) return HighlighterBasePtr(new ShellHighlighter);
    if (!_tcscmp(name, _T("XML")))   return HighlighterBasePtr(new XmlHighlighter);
    if (!_tcscmp(name, _T("SQR")))   return HighlighterBasePtr(new SqrHighlighter);
    if (!_tcscmp(name, _T("PERL")))  return HighlighterBasePtr(new PerlHighlighter);

    return HighlighterBasePtr(new CommonHighlighter(name));
}

HighlighterBase::HighlighterBase ()
{
    m_current.color = RGB(0,0,0);
    m_current.font  = efmNormal;
    m_printableSpaces = false;
}

///////////////////////////////////////////////////////////////////////////////
// CommonHighlighter
//  TODO:
//      SQL     - '.' as separator, sys package hook, a new list of sys packages
//      Shell   - one string
//      XML/XSL -
//

CommonHighlighter::CommonHighlighter (LPCTSTR langName)
{
    m_currentLine = 0;
    m_currentLineLength = 0;

    m_seqOf = ePlainText;
    m_isStartLine = false;

    m_textLabel        = _T("Text");
    m_numberLabel      = _T("Number");
    m_characterLabel   = _T("Character");
    m_stringLabel      = _T("String");
    m_commentLabel     = _T("Comment");
    m_lineCommentLabel = _T("Line Comment");

    const LanguagePtr langPtr = LanguagesCollection::Find(langName);

    m_LanguageKeywordMap = langPtr->GetLanguageKeywordMap();
    m_keywordGroups      = langPtr->GetKeywordGroups();
    m_caseSensiteve      = langPtr->IsCaseSensitive();
    m_commentPair        = langPtr->GetCommentPair();
    m_endLineComment     = langPtr->GetEndLineComment();
    m_startLineComment   = langPtr->GetStartLineComment();
    m_escapeChar         = langPtr->GetEscapeChar();
    m_stringPair         = langPtr->GetStringPair();
    m_charPair           = langPtr->GetCharPair();

    m_delimiters.Set(langPtr->GetDelimiters().GetString());

    m_symbolFastMap.erase();
    m_lineCommentFastMap.erase();

    if (!m_commentPair.first.IsEmpty())  m_symbolFastMap[m_commentPair.first .GetAt(0)] = true;
    if (!m_commentPair.second.IsEmpty()) m_symbolFastMap[m_commentPair.second.GetAt(0)] = true;
    if (!m_stringPair.first.IsEmpty())   m_symbolFastMap[m_stringPair .first .GetAt(0)] = true;
    if (!m_stringPair.second.IsEmpty())  m_symbolFastMap[m_stringPair .second.GetAt(0)] = true;
    if (!m_charPair.first.IsEmpty())     m_symbolFastMap[m_charPair   .first .GetAt(0)] = true;
    if (!m_charPair.second.IsEmpty())    m_symbolFastMap[m_charPair   .second.GetAt(0)] = true;

    if (!m_endLineComment.IsEmpty())
    {
        m_symbolFastMap[m_endLineComment.GetAt(0)] = true;
        m_lineCommentFastMap[m_endLineComment.GetAt(0)]= true;
    }

    if (!m_endLineComment.IsEmpty())
    {
        TCHAR ch = m_endLineComment.GetAt(0);
        if (m_caseSensiteve)
        {
            m_symbolFastMap[ch] = true;
            m_lineCommentFastMap[ch]= true;
        }
        else
        {
            m_symbolFastMap[toupper(ch)]= true;
            m_symbolFastMap[tolower(ch)]= true;
            m_lineCommentFastMap[toupper(ch)]= true;
            m_lineCommentFastMap[tolower(ch)]= true;
        }
    }

    std::set<CString>::const_iterator it = m_startLineComment.begin();
    for (; it != m_startLineComment.end(); it++)
        if (it->GetLength())
        {
            TCHAR ch = it->GetAt(0);
            if (m_caseSensiteve)
            {
                m_symbolFastMap[ch] = true;
                m_lineCommentFastMap[ch] = true;
            }
            else
            {
                m_symbolFastMap[toupper(ch)]= true;
                m_symbolFastMap[tolower(ch)]= true;
                m_lineCommentFastMap[toupper(ch)]= true;
                m_lineCommentFastMap[tolower(ch)]= true;
            }
        }
}

void HighlighterBase::Attrib::operator = (const VisualAttribute& attr)
{
    color = attr.m_Foreground;
    font  = (attr.m_FontBold ? efmBold : efmNormal)
        | (attr.m_FontItalic ? efmItalic : efmNormal)
        | (attr.m_FontUnderline ? efmUnderline : efmNormal);
}

void CommonHighlighter::Init (const VisualAttributesSet& set_)
{
    // required hard coded categories
    m_textAttr   = set_.FindByName(m_textLabel);
    m_numberAttr = set_.FindByName(m_numberLabel);

    if (!m_charPair.first.IsEmpty())    m_characterAttr   = set_.FindByName(m_characterLabel);
    if (!m_stringPair.first.IsEmpty())  m_stringAttr      = set_.FindByName(m_stringLabel);
    if (!m_commentPair.first.IsEmpty()) m_commentAttr     = set_.FindByName(m_commentLabel);
    if (!m_endLineComment.IsEmpty())    m_lineCommentAttr = set_.FindByName(m_lineCommentLabel);

    // language depended categories
    m_keyAttrs.resize(m_keywordGroups.size());
    std::vector<CString>::const_iterator it = m_keywordGroups.begin();
    for (int i(0); it != m_keywordGroups.end(); it++, i++)
    {
        m_keyAttrs.at(i) = set_.FindByName(it->GetString());
    }
}

void CommonHighlighter::NextLine (LPCTSTR currentLine, int currentLineLength)
{
    m_currentLine = currentLine;
    m_currentLineLength = currentLineLength;

    m_isStartLine = true;

    if (!(m_seqOf & eMultilineGroup))
    {
        m_current = m_textAttr;
        m_seqOf = ePlainText;
        m_printableSpaces = false;
    }
}

bool CommonHighlighter::closingOfSeq (LPCTSTR str, int /*len*/)
{
    switch (m_seqOf)
    {
    case eComment:
        if ((str - m_commentPair.second.GetLength() + 1) >= m_currentLine
            && !_tcsncmp(
                    m_commentPair.second.GetString(),
                    str - m_commentPair.second.GetLength() + 1,
                    m_commentPair.second.GetLength())
        )
        {
            m_seqOf = ePlainText;
            m_printableSpaces = false;
            return true;
        }
        break;
    case eCharacter:
        if ((str - m_charPair.second.GetLength() + 1) >= m_currentLine
            && !_tcsncmp(
                    m_charPair.second.GetString(),
                    str - m_charPair.second.GetLength() + 1,
                    m_charPair.second.GetLength())
            && (
                // escape is not defined or the previous fragment is not escape or it is not double escape
                m_escapeChar.IsEmpty()
                // the current position < escape size
                || str - m_currentLine < static_cast<int>(m_escapeChar.GetLength())
                // the previous fragment is not escape
                || _tcsncmp(m_escapeChar.GetString(), str - m_escapeChar.GetLength(), m_escapeChar.GetLength())
                // it is double escape
                || (
                    // the current position >= 2 * escape size
                    str - m_currentLine >= 2 * static_cast<int>(m_escapeChar.GetLength())
                    // the previous previous fragment is escape
                    && !_tcsncmp(m_escapeChar.GetString(), str - 2 * m_escapeChar.GetLength(), m_escapeChar.GetLength())
                )
            )
        )
        {
            m_seqOf = ePlainText;
            m_printableSpaces = false;
            return true;
        }
        break;
    case eString:
        if ((str - m_stringPair.second.GetLength() + 1) >= m_currentLine
            && !_tcsncmp(
                    m_stringPair.second.GetString(),
                    str - m_stringPair.second.GetLength() + 1,
                    m_stringPair.second.GetLength())
            && (
                // escape is not defined or the previous fragment is not escape or it is not double escape
                m_escapeChar.IsEmpty()
                // the current position < escape size
                || str - m_currentLine < static_cast<int>(m_escapeChar.GetLength())
                // the previous fragment is not escape
                || _tcsncmp(m_escapeChar.GetString(), str - m_escapeChar.GetLength(), m_escapeChar.GetLength())
                // it is double escape
                || (
                    // the current position >= 2 * escape size
                    str - m_currentLine >= 2 * static_cast<int>(m_escapeChar.GetLength())
                    // the previous previous fragment is escape
                    && !_tcsncmp(m_escapeChar.GetString(), str - 2 * m_escapeChar.GetLength(), m_escapeChar.GetLength())
                )
            )
        )
        {
            m_seqOf = ePlainText;
            m_printableSpaces = false;
            return true;
        }
        break;
    }
    return false;
}

bool CommonHighlighter::openingOfSeq (LPCTSTR str, int len)
{
    bool comment = false;

    if (m_lineCommentFastMap[*str])
    {
        if (!m_endLineComment.IsEmpty()
            && !_tcsncmp(str, m_endLineComment.GetString(), m_endLineComment.GetLength())
        )
            comment = true;

        if (m_isStartLine)
        {
            std::set<CString>::const_iterator it = m_startLineComment.begin();
            for (; it != m_startLineComment.end(); it++)
                if (len == static_cast<int>(it->GetLength()) // 10.03.2003 bug fix, start-line comment (prompt & remark) should be separated by delimiter from text
                && (m_caseSensiteve && !_tcsncmp(str, it->GetString(), it->GetLength())
                    || !m_caseSensiteve && !_tcsnicmp(str, it->GetString(), it->GetLength()))
                )
                {
                    comment = true;
                    break;
                }
        }
    }

    if (comment)
    {
        m_seqOf = eLineComment;
        m_current = m_lineCommentAttr;
        m_printableSpaces = true;
        return true;
    }
    else if (!m_charPair.first.IsEmpty()
        && (m_currentLine + m_currentLineLength) >= (str + m_charPair.first.GetLength())
        && !_tcsncmp(str, m_charPair.first.GetString(), m_charPair.first.GetLength()))
    {
        m_seqOf = eCharacter;
        m_current = m_characterAttr;
        m_printableSpaces = true;
        return true;
    }
    else if (!m_stringPair.first.IsEmpty()
        && (m_currentLine + m_currentLineLength) >= (str + m_stringPair.first.GetLength())
        && !_tcsncmp(str, m_stringPair.first.GetString(), m_stringPair.first.GetLength()))
    {
        m_seqOf = eString;
        m_current = m_stringAttr;
        m_printableSpaces = true;
        return true;
    }
    else if (!m_commentPair.first.IsEmpty()
        && (m_currentLine + m_currentLineLength) >= (str + m_commentPair.first.GetLength())
        && !_tcsncmp(str, m_commentPair.first.GetString(), m_commentPair.first.GetLength()))
    {
        m_seqOf = eComment;
        m_current = m_commentAttr;
        m_printableSpaces = true;
        return true;
    }

    return false;
}

// export some additional functionality
bool CommonHighlighter::IsKeyword (LPCTSTR str, int len, CString& keyword)
{
  CString key(str);
  key = key.Left(len);

  if(!m_caseSensiteve)
  {
    key.MakeUpper();
  }
  LanguageKeywordMapConstIterator it = m_LanguageKeywordMap->find(key);
	if(it != m_LanguageKeywordMap->end())
  {
      keyword = it->second.keyword;
      return true;
  }
  return false;
}

bool CommonHighlighter::isKeyword (LPCTSTR str, int len)
{
  CString key(str);
  key = key.Left(len);

  if(!m_caseSensiteve)
  {
    key.MakeUpper();
  }
  LanguageKeywordMapConstIterator it = m_LanguageKeywordMap->find(key);

	if (it != m_LanguageKeywordMap->end())
  {
      m_seqOf = eKeyword;
      m_current = m_keyAttrs.at(it->second.groupIndex);
      return true;
  }
  else
  {
      m_seqOf = ePlainText;
      m_current = m_textAttr;
      return false;
  }
}

// it's the simplest implementation of isDigit
// we check only the first character
bool CommonHighlighter::isDigit (LPCTSTR str, int /*len*/)
{
  if (isdigit(*str))
  {
    m_current = m_numberAttr;
    return true;
  }
  return false;
}

void CommonHighlighter::NextWord (LPCTSTR str, int len)
{
	_ASSERTE(str && len > 0);

  if(!(m_symbolFastMap[*str] && closingOfSeq(str,len))
     && (m_seqOf & ePlainGroup)
     && !(m_symbolFastMap[*str] && openingOfSeq(str,len))
     && !isDigit(str,len)
     )
  {
    isKeyword(str,len);
  }
  m_isStartLine = false;
}

void CommonHighlighter::InitStorageScanner (MultilineQuotesScanner& scanner) const
{
    scanner.ClearSettings ();
    scanner.SetCaseSensitive(m_caseSensiteve);

    for(auto& str : m_startLineComment)
    {
      scanner.AddStartLineQuotes(str);
    }

    if (!m_endLineComment.IsEmpty())    scanner.AddSingleLineQuotes(m_endLineComment);
    if (!m_commentPair.first.IsEmpty()) scanner.AddMultilineQuotes(m_commentPair);
    if (!m_stringPair.first.IsEmpty())  scanner.AddMultilineQuotes(m_stringPair);
    if (!m_charPair.first.IsEmpty())    scanner.AddMultilineQuotes(m_charPair);
    if (!m_escapeChar.IsEmpty())        scanner.AddEscapeChar(m_escapeChar);

    scanner.SetDelimitersMap(m_delimiters);
}

void CommonHighlighter::SetMultilineQuotesState (int state, int quoteId, bool)
{
    m_current = m_textAttr;
    m_seqOf = ePlainText;

    if (state > 0)
    {
        if (m_commentPair.first.IsEmpty()) quoteId++;
        if (m_stringPair.first.IsEmpty()) quoteId++;
        //if (m_charPair.first.empty())   quoteId++;

        switch (quoteId)
        {
        case 0:
            m_seqOf = eComment;
            m_current = m_commentAttr;
            break;
        case 1:
            m_seqOf = eString;
            m_current = m_stringAttr;
            break;
        case 2:
            m_seqOf = eCharacter;
            m_current = m_characterAttr;
        }

        m_printableSpaces = true;
    }
}

};
