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

#include "stdafx.h"
#include "OpenEditor/OEHighlighter.h"
#include "COMMON/VisualAttributes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


namespace OpenEditor
{

///////////////////////////////////////////////////////////////////////////////

void XmlHighlighter::Init (const VisualAttributesSet& set_)
{
    m_xmlSeqOf = eXmlPlain;

    // owerride required hardcoded categories
    m_textLabel        = "Text";
    m_numberLabel      = "Text";
    m_characterLabel   = "Values";
    m_stringLabel      = "Values";

    CommonHighlighter::Init(set_);

    // required hardcoded categories
    m_elementsAttr = set_.FindByName("Elements");
    m_attrsAttr    = set_.FindByName("Attributes");
}

void XmlHighlighter::NextLine (const char* currentLine, int currentLineLength)
{
    m_xmlAmpSeq = eXmlAmpNone;
    CommonHighlighter::NextLine(currentLine, currentLineLength);
}

void XmlHighlighter::NextWord (const char* str, int len)
{
    ESeqOf _seqOf = m_seqOf;

    // m_xmlAmpSeq: restore the current attributes
    if (m_xmlAmpSeq == eXmlAmpClose)
    {
        m_xmlAmpSeq = eXmlAmpNone;
        m_current = m_savedAttr;
    }

    //  m_xmlAmpSeq: the current highliting has to be restored after ';' 
    if (m_xmlAmpSeq == eXmlAmpOpen)
    {
        if (*str == ';')
            m_xmlAmpSeq = eXmlAmpClose;

        return;
    }

    //  m_xmlAmpSeq: "&abc;" owerrides the current highliting except comments
    if (m_xmlSeqOf != eXmlComment && *str == '&')
    {
        m_savedAttr = m_current; // save the current attributes

        int i;
        int limit = (int)(m_currentLine - str) + m_currentLineLength;
        for (i = 0; i < limit && str[i] != ';'; i++)
            ;

        if (str[i] == ';')
        {
            if (isKeyword(str, i + 1))
            {
                m_xmlAmpSeq = eXmlAmpOpen;
                m_seqOf = _seqOf;
                return;
            }
            else // restore to a state before findKeywords
                m_seqOf = _seqOf;
        }
    }

    // xml comment processing
    if (m_seqOf & ePlainGroup)
    {
        switch (*str)
        {
        case '<': 
            if (str + 4 <= m_currentLine + m_currentLineLength
            && !strncmp(str, "<!--", 4))
                m_xmlSeqOf = eXmlComment;
            else
                m_xmlSeqOf = eXmlOpen;  
            break;
        case '>': 
            m_xmlSeqOf = eXmlClose; 
            break;
        }
    }

    // it's not a plain text 
    if (m_xmlSeqOf != eXmlPlain)
        CommonHighlighter::NextWord(str, len);

    switch (m_xmlSeqOf)
    {
    case eXmlPlain:
        m_current = m_textAttr;
        break;
    case eXmlOpen:
        if (str + 1 < m_currentLine + m_currentLineLength && str[1] != '/')
            m_xmlSeqOf = eXmlElement;
        break;
    case eXmlElement:
        m_current = m_elementsAttr;
        m_xmlSeqOf = eXmlOther;
        break;
    case eXmlClose:
        m_xmlSeqOf = eXmlPlain;
        break;
    case eXmlComment:
        if (m_seqOf & ePlainGroup)
        {
            m_xmlSeqOf = eXmlPlain;
            m_current = m_commentAttr;
        }
        break;
    default:
        if (m_seqOf == ePlainText && _seqOf != eString)
            m_current = m_attrsAttr;
    }
}

void XmlHighlighter::InitStorageScanner (MultilineQuotesScanner& scanner) const
{
    CommonHighlighter::InitStorageScanner(scanner);
    scanner.SetParsingQuotes(pair<string,string>("<",">"));
}

void XmlHighlighter::SetMultilineQuotesState (int state, int quoteId, bool parsing)
{
    if (!parsing) state = quoteId = 0;

    CommonHighlighter::SetMultilineQuotesState(state, quoteId, parsing);

    switch (m_seqOf) 
    {
    case ePlainText:    m_xmlSeqOf = parsing ? eXmlOther : eXmlPlain; break;
    case eComment:      m_xmlSeqOf = eXmlComment;   break;
    default:            m_xmlSeqOf = eXmlOther;
    }
}

};
