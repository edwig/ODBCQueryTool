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

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __OEHighlighter_h__
#define __OEHighlighter_h__

#include <map>
#include <string>
#include <common/arg_shared.h>
#include <OEHelpers.h>
#include <OELanguage.h>
#include <Common/Fastmap.h>


namespace Common
{
    struct VisualAttribute;
    class  VisualAttributesSet;
};

namespace OpenEditor
{
    using std::map;
    using std::set;
    using std::pair;
    using std::string;
    using std::vector;

    using arg::counted_ptr;
    using Common::Fastmap;
    using Common::VisualAttribute;
    using Common::VisualAttributesSet;

    class HighlighterBase;
    typedef counted_ptr<HighlighterBase> HighlighterBasePtr;

    struct HighlighterFactory
    {
        static HighlighterBasePtr CreateHighlighter (const char* name);
        static HighlighterBasePtr CreateHighlighter (const string& name)
            { return CreateHighlighter (name.c_str()); }
    };


    class HighlighterBase : public CObject
    {
    public:
	    HighlighterBase ();
	    virtual ~HighlighterBase () {};

	    virtual void Init (const VisualAttributesSet&)  = 0;
	    virtual void NextLine (const char*, int)        = 0;
	    virtual void NextWord (const char*, int)        = 0;

        virtual void SetMultilineQuotesState (int, int, bool) = 0;
        virtual void InitStorageScanner (MultilineQuotesScanner&) const = 0;

	    COLORREF GetTextColor () const;
	    unsigned GetFontIndex () const;
	    bool IsSpacePrintable () const; // for underlined spaces in string and comments
        const DelimitersMap& GetDelimiters () const;

        enum EFontMask { efmNormal = 0x00, efmBold = 0x01, efmItalic = 0x02, efmUnderline = 0x04 };

        // export some additional functionality
        virtual bool IsKeyword (const char* str, int len, string&) = 0;
        virtual bool IsPlainText () = 0;
    protected:
        struct Attrib
        {
	        COLORREF color;
	        unsigned font;

            void operator = (const VisualAttribute&);
        }
        m_current;

        DelimitersMap m_delimiters;
        bool m_printableSpaces;
    };

    ///////////////////////////////////////////////////////////////////////

    class CommonHighlighter : public HighlighterBase
    {
    public:
	    CommonHighlighter (const char* language);

	    virtual void Init (const VisualAttributesSet&);
	    virtual void NextLine (const char*, int);
	    virtual void NextWord (const char*, int);
        
        virtual void SetMultilineQuotesState (int, int, bool);
        virtual void InitStorageScanner (MultilineQuotesScanner&) const;

        // export some additional functionality
        virtual bool IsKeyword (const char* str, int len, string&);
        virtual bool IsPlainText () { return m_seqOf & ePlainText? true : false; };
    protected:
        bool openingOfSeq (const char* str, int len);
        bool closingOfSeq (const char* str, int len);
        bool isKeyword (const char* str, int len);
        bool isDigit (const char* str, int len);

        // language attributes
        bool                    m_caseSensiteve;
        vector<string>          m_keywordGroups;
        pair<string, string>    m_commentPair;
        string                  m_endLineComment;
        set<string>             m_startLineComment;
        string                  m_escapeChar;
        pair<string, string>    m_stringPair;
        pair<string, string>    m_charPair;
        LanguageKeywordMapPtr   m_LanguageKeywordMap;
        
        Fastmap<bool> m_symbolFastMap;
        Fastmap<bool> m_lineCommentFastMap;

        // whole line required for for some checks !!!
        const char* m_currentLine;
        int m_currentLineLength;

        // current state
        enum ESeqOf { 
            ePlainGroup = 0x10, eCommentGroup = 0x100, eStringGroup = 0x1000, eMultilineGroup = 0x10000, 
            ePlainText      = 0x0 | ePlainGroup, 
            eString         = 0x1 | eStringGroup  | eMultilineGroup, 
            eCharacter      = 0x2 | eStringGroup  | eMultilineGroup, 
            eComment        = 0x3 | eCommentGroup | eMultilineGroup, 
            eLineComment    = 0x4 | eCommentGroup,
            eKeyword        = 0x5 | ePlainGroup,
        };
        ESeqOf m_seqOf;
        bool m_isStartLine;

  		Attrib m_textAttr, 
               m_commentAttr, m_lineCommentAttr,
               m_characterAttr, m_stringAttr,
               m_numberAttr;

        vector<Attrib> m_keyAttrs;

        // labels for runtime linking
        const char* m_textLabel        ;//= "Text";
        const char* m_numberLabel      ;//= "Number";
        const char* m_characterLabel   ;//= "Character";
        const char* m_stringLabel      ;//= "String";
        const char* m_commentLabel     ;//= "Comment";
        const char* m_lineCommentLabel ;//= "Line Comment";
    };

    ///////////////////////////////////////////////////////////////////////

    class CPlusPlusHighlighter : public CommonHighlighter
    {
    public:
        CPlusPlusHighlighter ();

	    virtual void Init (const VisualAttributesSet&);
	    virtual void NextLine (const char*, int);
	    virtual void NextWord (const char*, int);

    private:
        bool m_includeDirective;
        char m_openBrace;
        Attrib m_preprocessorAttrs;
    };

    ///////////////////////////////////////////////////////////////////////

    class PlSqlHighlighter : public CommonHighlighter
    {
    public:
        PlSqlHighlighter ();

	    virtual void Init (const VisualAttributesSet&);
	    virtual void NextLine (const char*, int);
	    virtual void NextWord (const char*, int);

    private:
        char m_openBrace;
        Attrib m_bindVarAttrs;
        Attrib m_substitutionAttrs;
        Attrib m_fileNameAttrs;
    };

    ///////////////////////////////////////////////////////////////////////

    class SqrHighlighter : public CommonHighlighter
    {
    public:
        SqrHighlighter ();

	    virtual void Init (const VisualAttributesSet&);
	    virtual void NextWord (const char*, int);

    private:
        Attrib m_variablesAttrs;
        Attrib m_preprocessorAttrs;
    };

    ///////////////////////////////////////////////////////////////////////

    class ShellHighlighter : public CommonHighlighter
    {
    public:
        ShellHighlighter ();

	    virtual void Init (const VisualAttributesSet&);
	    virtual void NextLine (const char*, int);
	    virtual void NextWord (const char*, int);

    private:
        bool m_isSubstitution;
        bool m_isBraceToken;
        Attrib m_substAttrs;
    };

    ///////////////////////////////////////////////////////////////////////

    class XmlHighlighter : public CommonHighlighter
    {
    public:
        XmlHighlighter ();

	    virtual void Init (const VisualAttributesSet&);
	    virtual void NextLine (const char*, int);
	    virtual void NextWord (const char*, int);

        void SetMultilineQuotesState (int state, int quoteId, bool parsing);
        void InitStorageScanner (MultilineQuotesScanner&) const;

    private:
        Attrib m_elementsAttr;
        Attrib m_attrsAttr;
        
        enum EXmlSeqOf { eXmlOpen, eXmlPlain, eXmlElement, eXmlOther, eXmlClose, eXmlComment };
        EXmlSeqOf m_xmlSeqOf;
        
        enum EXmlAmpSeqOf { eXmlAmpNone, eXmlAmpOpen, eXmlAmpClose };
        EXmlAmpSeqOf m_xmlAmpSeq;
        Attrib m_savedAttr;
    };

    ///////////////////////////////////////////////////////////////////////

    class PerlHighlighter : public CommonHighlighter
    {
    public:
        PerlHighlighter ();

	    virtual void Init (const VisualAttributesSet&);
	    virtual void NextWord (const char*, int);

    private:
        Attrib m_vartAttrs;
    };

    ///////////////////////////////////////////////////////////////////////

    inline
    COLORREF HighlighterBase::GetTextColor () const
        { return m_current.color; }

    inline
	unsigned HighlighterBase::GetFontIndex () const
        { return m_current.font; }

    inline
	bool HighlighterBase::IsSpacePrintable () const
        { return m_printableSpaces; }

    inline
    const DelimitersMap& HighlighterBase::GetDelimiters () const
        { return m_delimiters; }

    inline
    CPlusPlusHighlighter::CPlusPlusHighlighter () 
        : CommonHighlighter("C++") {} 

    inline
    PlSqlHighlighter::PlSqlHighlighter () 
        : CommonHighlighter("SQL") {} 

    inline
    SqrHighlighter::SqrHighlighter () 
        : CommonHighlighter("SQR") {} 

    inline
    ShellHighlighter::ShellHighlighter () 
        : CommonHighlighter("Shell") {} 

    inline
    XmlHighlighter::XmlHighlighter () 
        : CommonHighlighter("XML") {} 

    inline
    PerlHighlighter::PerlHighlighter ()
        : CommonHighlighter("PERL") {} 
};

#endif//__OEHighlighter_h__
