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

#ifndef __OEMatchSearcher_h__
#define __OEMatchSearcher_h__

#include <string>
#include <map>
#include <stack>
#include "OEHelpers.h"
#include "Common/Fastmap.h"


namespace OpenEditor
{
    using std::map;
    using std::stack;
    using Common::Fastmap;

    class EditContext;

    enum EPLSToken
    {
        etUNKNOWN,
        etPROCEDURE,
        etFUNCTION,
        etPACKAGE,
        etDECLARE,
        etBEGIN,
        etEXCEPTION,
        etEND,
        etIF,
        etTHEN,
        etELSE,
        etELSIF,
//        etFOR,
//        etWHILE,
        etLOOP,
        etEXIT,
        etDOT,
        etSEMICOLON,
        etQUOTE,
        etDOUBLE_QUOTE,
        etLEFT_BRACKET,
        etRIGHT_BRACKET,
        etLEFT_ROUND_BRACKET,
        etRIGHT_ROUND_BRACKET,
        etMINUS,
        etSLASH,
        etSTAR,
        // second level tokens
        etEOL,
        etEOF,
        etNUMBER,
        etDELIMITER,
        etIDENTIFIER,
        eQUOTED_STRING,
        etDOUBLE_QUOTED_STRING,
        etCOMMENT,
    };

    struct TokenInfo
    {
        EPLSToken m_token;
        int m_line, m_offset, m_length;

        TokenInfo ()
        {
            m_token = etUNKNOWN;
            m_line = m_offset = m_length = 0;
        }
    };


    enum EPLSLexeme
    {
        elUNKNOWN,
        elBLOCK,
        elSELECTION,
        elITERATION,
        elEXPRESSION,
    };

    const char* GetLexemeName (EPLSLexeme lexeme);

    struct LexemeInfo
    {
        enum EStatus { eOk, eDeferredClosing };

        EStatus m_status;
        EPLSLexeme m_lexeme;
        vector<TokenInfo> m_tokenInfos;

        LexemeInfo ()
        {
            m_status = eOk;
            m_lexeme = elUNKNOWN;
        }

        void AddToken (const TokenInfo& tokenInfo) { m_tokenInfos.push_back(tokenInfo); }
        const TokenInfo& LastTokenInfo () const    { return m_tokenInfos.back(); }
        EPLSToken LastToken () const               { return m_tokenInfos.back().m_token; }
        bool IsDeferred () const                   { return m_status == eDeferredClosing; }
        void Defer ()                              { m_status = eDeferredClosing; }
    };

    class SyntaxAnalyser
    {
    public:
        virtual ~SyntaxAnalyser () {};
        virtual bool PutToken (const TokenInfo&, const CString&) = 0;
    };


    class LexicalAnalyser
    {
    public:
        LexicalAnalyser ();

        void Join (SyntaxAnalyser*);
        bool PutLine (const char*, int line, int offset, int length, bool backward);

    private:
        Fastmap<bool> m_fastmap;
        map<CString, int> m_tokens;

        SyntaxAnalyser* m_analyzer;
        EPLSToken m_lastToken;

        enum ESequenceOf { eNone, eQuotedString, eDblQuotedString, eEndLineComment, eComment };
        ESequenceOf m_sequenceOf;
        CString m_stringOf;

        static DelimitersMap m_Delimiters;
    };


    class StartTokenFinder : public SyntaxAnalyser
    {
    public:
        StartTokenFinder ();
        virtual bool PutToken (const TokenInfo&, const CString&);
        bool FoundOut (LexemeInfo&, bool& backward) const;

    private:
        int m_step;
        LexemeInfo m_lexemeInfo;
        bool       m_backward;
    };



    class MatchAnalyser : public SyntaxAnalyser
    {
    public:
        MatchAnalyser (const LexemeInfo& lexemeInfo, bool backward);
        virtual bool PutToken (const TokenInfo&, const CString&);

        bool FoundOut (LexemeInfo&) const;

    private:
        bool putToken (const TokenInfo&, const CString&);
        bool putTokenBackward (const TokenInfo&, const CString&);

        bool m_backward, m_firstApproach;
        stack<LexemeInfo> m_stack;
        LexemeInfo m_lastLexemeInfo;
        TokenInfo m_lastTokenInfo;

        void open (EPLSLexeme lexeme, const TokenInfo& tokenInfo);
        void close (const TokenInfo&);
        void defer (const TokenInfo&);
        void closeDeferred ();
        void closeDeferred (const TokenInfo&);
        void closeAllDeferred (LexemeInfo&);
    };


    class MatchSearcherBase
    {
    public:
        virtual void Find (EditContext&, bool select) = 0;
    };

    class GeneralMatchSearcher : public MatchSearcherBase
    {
    public:
        virtual void Find (EditContext&, bool select);
    };

    class PlSqlMatchSearcher : public MatchSearcherBase
    {
    public:
        virtual void Find (EditContext&, bool select);
    };

    using arg::counted_ptr;
    class HighlighterBase;
    typedef counted_ptr<MatchSearcherBase> MatchSearcherBasePtr;

    struct MatchSearcherFactory
    {
        static MatchSearcherBasePtr CreateMatchSearcher (const char* lang);
        static MatchSearcherBasePtr CreateMatchSearcher (const string& lang)
            { return CreateMatchSearcher (lang.c_str()); }
    };


    class UnexpectedToken : public std::exception
    {
    public:
        UnexpectedToken::UnexpectedToken () :
            std::exception("Match brace: unexpected token!") {}
    };


    inline
    bool StartTokenFinder::FoundOut (LexemeInfo& lexemeInfo, bool& backward) const
    {
        lexemeInfo = m_lexemeInfo;
        backward = m_backward;
        return (elUNKNOWN != m_lexemeInfo.m_lexeme) ? true : false;
    }

    inline
    MatchAnalyser::MatchAnalyser (const LexemeInfo& lexemeInfo, bool backward)
    {
        TRACE("MatchAnalyser::MatchAnalyser\n");
        m_backward = backward;
        m_stack.push(lexemeInfo);
    }

    inline
    bool MatchAnalyser::FoundOut (LexemeInfo& lexemeInfo) const
    {
        lexemeInfo = m_lastLexemeInfo;
        return !m_backward && m_stack.size() < 2
            || m_backward && (m_stack.empty() || (m_stack.size() < 2 && m_stack.top().IsDeferred()));
    }

    inline
    void MatchAnalyser::open (EPLSLexeme lexeme, const TokenInfo& tokenInfo)
    {
        TRACE("MatchAnalyser::open %s\n", GetLexemeName(lexeme));

        LexemeInfo lexemeInfo;
        lexemeInfo.m_lexeme = lexeme;
        m_stack.push(lexemeInfo);
        m_stack.top().m_tokenInfos.push_back(tokenInfo);
    }


    inline
    void MatchAnalyser::defer (const TokenInfo& tokenInfo)
    {
        TRACE("MatchAnalyser::defer %s\n", GetLexemeName(m_stack.top().m_lexeme));

        m_stack.top().m_status = LexemeInfo::eDeferredClosing;
        m_stack.top().AddToken(tokenInfo);
    }

    inline
    void MatchAnalyser::closeDeferred (const TokenInfo& tokenInfo)
    {
        TRACE("MatchAnalyser::closeDeferred %s\n", GetLexemeName(m_stack.top().m_lexeme));

        if (!m_stack.top().IsDeferred())
            throw UnexpectedToken();

        m_stack.top().AddToken(tokenInfo);
        m_lastLexemeInfo = m_stack.top();
        m_stack.pop();
    }

    inline
    void MatchAnalyser::closeAllDeferred (LexemeInfo& top)
    {
        TRACE("MatchAnalyser::closeAllDeferred\n");

        while (top.IsDeferred() && m_stack.size() > 1)
        {
            closeDeferred();
            top = m_stack.top();
        }
    }

    inline
    void MatchAnalyser::closeDeferred ()
    {
        TRACE("MatchAnalyser::closeDeferred %s\n", GetLexemeName(m_stack.top().m_lexeme));

        if (!m_stack.top().IsDeferred())
            throw UnexpectedToken();

        m_lastLexemeInfo = m_stack.top();
        m_stack.pop();
    }

    inline
    void MatchAnalyser::close (const TokenInfo& tokenInfo)
    {
        TRACE("MatchAnalyser::close %s\n", GetLexemeName(m_stack.top().m_lexeme));

        if (m_stack.top().m_status == LexemeInfo::eDeferredClosing)
        {
            m_lastLexemeInfo = m_stack.top();
            m_stack.pop();
        }

        if (!m_stack.size())
            throw UnexpectedToken();

        m_stack.top().AddToken(tokenInfo);
        m_lastLexemeInfo = m_stack.top();
        m_stack.pop();
    }

    /*
    [declare|procedure|function]
    [exception]
    end;

    if
    [elsif]
    [else]
    end if;

    [for]|[while]
    loop
    [exit]
    end loop;

    (
    )

    [
    ]
    */
};

#endif//__OEMatchSearcher_h__
