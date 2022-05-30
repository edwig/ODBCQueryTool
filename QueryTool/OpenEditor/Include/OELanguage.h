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

#ifndef __OELanguage_h__
#define __OELanguage_h__

#include <map>
#include <set>
#include <vector>
#include <string>
#include <common/arg_shared.h>


namespace OpenEditor
{
    using std::pair;
    using std::map;
    using std::set;
    using std::vector;
    using std::string;
    using arg::counted_ptr;

    class Storage;
    class EditContext;

    struct LanguageKeyword 
    { 
        LanguageKeyword () {}
        LanguageKeyword (string _keyword, int _groupIndex) : keyword(_keyword), groupIndex(_groupIndex) {}
        string keyword; 
        int groupIndex; 
    };

	typedef map<string, LanguageKeyword> LanguageKeywordMap;
	typedef LanguageKeywordMap::iterator LanguageKeywordMapIterator;
	typedef LanguageKeywordMap::const_iterator LanguageKeywordMapConstIterator;
    typedef counted_ptr<LanguageKeywordMap> LanguageKeywordMapPtr;

    class Language
    {
    public:
        Language () : m_LanguageKeywordMap(new LanguageKeywordMap) {}

        const string& GetName () const;
        bool  IsCaseSensitive () const;
        const vector<string>& GetKeywordGroups () const;
        const string& GetPreprocessor () const;
        const pair<string, string>& GetCommentPair () const;
        const string& GetEndLineComment () const;
        const set<string>& GetStartLineComment () const;
        const string& GetEscapeChar () const;
        const pair<string, string>& GetStringPair () const;
        const pair<string, string>& GetCharPair () const;
        const LanguageKeywordMapPtr GetLanguageKeywordMap () const;
        const string& GetDelimiters () const;
        const vector<vector<string> >& GetMatchBraces () const;

//    private:
        string                  m_name;
        bool                    m_caseSensiteve;
        vector<string>          m_keywordGroups;
        pair<string, string>    m_commentPair;
        string                  m_endLineComment;
        set<string>             m_startLineComment;
        string                  m_escapeChar;
        pair<string, string>    m_stringPair;
        pair<string, string>    m_charPair;
        string                  m_delimiters;
        vector<vector<string> > m_matchBraces;

        /*
            charEsc, stringEsc, hyphenChar     
        */

	    LanguageKeywordMapPtr m_LanguageKeywordMap;

        friend class LanguagesCollectionReader;
        friend class LanguagesCollectionWriter;
    };

        typedef counted_ptr<Language> LanguagePtr;
    
    class LanguagesCollection
    {
    public:
        static const LanguagePtr Find (const string&);
   private:
        static vector<LanguagePtr> m_Languages;
        friend class LanguagesCollectionReader;
        friend class LanguagesCollectionWriter;
    };

    inline
    const string& Language::GetName () const
        { return m_name; }
    inline
    bool  Language::IsCaseSensitive () const
        { return m_caseSensiteve; }
    inline
    const vector<string>& Language::GetKeywordGroups () const
        { return m_keywordGroups; }
    inline
    const pair<string, string>& Language::GetCommentPair () const
        { return m_commentPair; }
    inline
    const string& Language::GetEndLineComment () const
        { return m_endLineComment; }
    inline
    const set<string>& Language::GetStartLineComment () const
        { return m_startLineComment; }
    inline
    const string& Language::GetEscapeChar () const
        { return m_escapeChar; }
    inline
    const pair<string, string>& Language::GetStringPair () const
        { return m_stringPair; }
    inline
    const pair<string, string>& Language::GetCharPair () const
        { return m_charPair; }
    inline
    const LanguageKeywordMapPtr Language::GetLanguageKeywordMap () const
        { return m_LanguageKeywordMap; }
    inline
    const string& Language::GetDelimiters () const
        { return m_delimiters; }
    inline
    const vector<vector<string> >& Language::GetMatchBraces () const
        { return m_matchBraces; }

};//namespace OpenEditor


#endif//__OELanguage_h__
