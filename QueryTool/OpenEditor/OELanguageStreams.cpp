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
#include <algorithm>
#include <locale>
#include <COMMON/StrHelpers.h>
#include <COMMON/ExceptionHelper.h>
#include <OELanguageStreams.h>
//#include "COMMON/AppUtilities.h" //temporary here

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define OESMS_WRITE_MEMBER(o,p) m_out.write(_T(#p), o.m_##p);
#define OESMS_READ_MEMBER(o,p)   m_in.read (_T(#p), o.m_##p);

namespace OpenEditor
{
    using namespace Common;

/////////////////////////////////////////////////////////////////////////////
// LanguagesCollection Reader/Writer
const int TheLanguagesCollectionVersion = 1001;

void LanguagesCollectionWriter::operator << (const LanguagesCollection&)
{
    m_out.write(_T("version"), TheLanguagesCollectionVersion);
    m_out.write(_T("languages"), (int)LanguagesCollection::m_Languages.size());

    std::vector<LanguagePtr>::const_iterator
        colIt = LanguagesCollection::m_Languages.begin(),
        colEnd = LanguagesCollection::m_Languages.end();

    Stream::Section sect(m_out, _T("lng"));
    for (int i = 0; colIt != colEnd; colIt++, i++)
    {
        Stream::Section sect2(m_out, i);
        write(**colIt);
    }
}

void LanguagesCollectionWriter::write (const Language& lang)
{
    OESMS_WRITE_MEMBER(lang, name);
    OESMS_WRITE_MEMBER(lang, delimiters);
    OESMS_WRITE_MEMBER(lang, caseSensiteve);
    OESMS_WRITE_MEMBER(lang, commentPair);
    OESMS_WRITE_MEMBER(lang, endLineComment);
    OESMS_WRITE_MEMBER(lang, startLineComment);
    OESMS_WRITE_MEMBER(lang, escapeChar);
    OESMS_WRITE_MEMBER(lang, stringPair);
    OESMS_WRITE_MEMBER(lang, charPair);
    OESMS_WRITE_MEMBER(lang, matchBraces);

    write(lang.m_LanguageKeywordMap, lang.m_keywordGroups);
}

void LanguagesCollectionWriter::write (const LanguageKeywordMapPtr& keywordMap, const vector<CString>& groups)
{
    
    m_out.write(_T("keywordGroups"), (int)groups.size());

    Stream::Section sect1(m_out, _T("grp"));

    std::vector<CString>::const_iterator groupIt = groups.begin();
    for (int groupIndex(0); groupIt != groups.end(); groupIt++, groupIndex++)
    {
        int groupCounter = 0;
        LanguageKeywordMapConstIterator it = keywordMap->begin(), endIt =keywordMap->end();
        for (; it != endIt; it++)
            if (groupIndex == it->second.groupIndex)
                groupCounter++;
        
        Stream::Section sect2(m_out, groupIndex);
        m_out.write(_T("name"), *groupIt);
        m_out.write(_T("keywords"), groupCounter);
        
        Stream::Section sect3(m_out, _T("kwd"));
        it = keywordMap->begin();
        for (int i(0); it != endIt; it++)
            if (groupIndex == it->second.groupIndex)
            {
                TCHAR buff[80];
                _itot_s(i++, buff, 80, 10);
                m_out.write(buff,it->second.keyword);
            }
    }
}

void LanguagesCollectionReader::operator >> (LanguagesCollection&)
{
    m_version = 0;
    m_in.read(_T("version"), m_version);

    _CHECK_AND_THROW_(m_version >= 1000 && m_version <= TheLanguagesCollectionVersion, 
                      _T("Invalid language stream version!"));

    int languges = 0;
    m_in.read(_T("languages"), languges);

    LanguagesCollection::m_Languages.clear();

    Stream::Section sect(m_in, _T("lng"));
    for (int i = 0; i < languges; i++)
    {
        Stream::Section sect2(m_in, i);
        LanguagePtr lang = new Language;
        read(*lang);
        LanguagesCollection::m_Languages.push_back(lang);
    }
}

void LanguagesCollectionReader::read (Language& lang)
{
    OESMS_READ_MEMBER(lang, name);
    TRACE1("Loading Language=%s\n", lang.m_name.GetString());

    OESMS_READ_MEMBER(lang, delimiters);
    OESMS_READ_MEMBER(lang, caseSensiteve);
    OESMS_READ_MEMBER(lang, commentPair);
    OESMS_READ_MEMBER(lang, endLineComment);
    OESMS_READ_MEMBER(lang, startLineComment);
    OESMS_READ_MEMBER(lang, escapeChar);
    OESMS_READ_MEMBER(lang, stringPair);
    OESMS_READ_MEMBER(lang, charPair);
    
    if (m_version > 1000)
    {
        OESMS_READ_MEMBER(lang, matchBraces);
    }
    else
    {
        struct 
        {
          LPCTSTR open;
          LPCTSTR close;
        } 
        g_braces [] = 
        {
            {   _T("{"), _T("}")    },
            {   _T("("), _T(")")    },
            {   _T("["), _T("]")    },
        };

        for (int i = 0; i < sizeof g_braces/sizeof g_braces[0]; i++)
        {
            vector<CString> pair;
            pair.push_back(g_braces[i].open); 
            pair.push_back(g_braces[i].close);
            lang.m_matchBraces.push_back(pair);
        }
    }

    read(lang.m_LanguageKeywordMap, lang.m_keywordGroups, lang.m_caseSensiteve);
}

void LanguagesCollectionReader::read (LanguageKeywordMapPtr& keywordMap, vector<CString>& groups, bool caseSensiteve)
{
    keywordMap->clear();
    groups.clear();

    int count;
    m_in.read(_T("keywordGroups"), count);

    Stream::Section sect1(m_in, _T("grp"));

    for (int i(0); i < count; i++)
    {
        Stream::Section sect2(m_in, i);
        
        CString name;
        m_in.read(_T("name"), name);
        groups.push_back(name);
        int groupIndex = (int)groups.size() - 1;

        int keywords;
        m_in.read(_T("keywords"), keywords);

        Stream::Section sect3(m_in, _T("kwd"));

        for (int j(0); j < keywords; j++)
        {
            TCHAR buff[40];
            LanguageKeyword keyword;
            _itot_s(j, buff, 40, 10);
            m_in.read(buff, keyword.keyword);
            keyword.groupIndex = groupIndex;
            CString key = keyword.keyword;

            trim_symmetric(key);

            if(!caseSensiteve)
            {
              key.MakeUpper();
            }

            if(keywordMap->find(key) == keywordMap->end())
            {
              keywordMap->insert(pair<CString,LanguageKeyword>(key,keyword));
            }
            else
            {
              TRACE1("\tWARNING: skip duplicated keyword \"%s\"\n",key.GetString());
            }
        }
    }
}

#if 0
struct _LanguagesCollection : LanguagesCollection
{
    _LanguagesCollection ()
    {
        LanguagesCollectionReader(
                OpenEditor::FileInStream("\\languages-3.dat")
            ) >> *this;

        m_Languages.push_back(LanguagePtr(new Language));

        (**m_Languages.rbegin()).m_name = "Shell";
        (**m_Languages.rbegin()).m_caseSensiteve = true;
//        (**m_Languages.rbegin()).m_commentPair = pair<string,string>("/*","*/");
        (**m_Languages.rbegin()).m_endLineComment = "#";
        (**m_Languages.rbegin()).m_stringPair = pair<string,string>("\"","\"");
        (**m_Languages.rbegin()).m_charPair = pair<string,string>("\'","\'");
        (**m_Languages.rbegin()).m_keywordGroups.push_back("Operators");
        (**m_Languages.rbegin()).m_keywordGroups.push_back("Keywords");
        (**m_Languages.rbegin()).m_keywordGroups.push_back("Built-in Commands");
        
		LPCTSTR filenames[] =
	    {
      _T("\\Operators.kwd"), _T("\\Keywords.kwd"), _T("\\preprocessor.kwd"), 
	    };

	    for (int i(0); i < sizeof filenames/sizeof filenames[0]; i++)
	    {
		    std::ifstream is(filenames[i]);

            string buff;
            while (std::getline(is, buff))
		    {
			    if (buff[0])
			    {
	                TCHAR key[60];
                    int len = min(sizeof key, buff.size());
	                _tcsncpy(key, buff.c_str(), len);
                    key[len] = 0;
                    (*(**m_Languages.rbegin()).m_LanguageKeywordMap)[key] = LanguageKeyword(key, i);
			    }
		    }
	    }

        LanguagesCollectionWriter(
                OpenEditor::FileOutStream("\\languages-4.dat")
            ) << *this;
    }
}
g_test;
#endif
#if 0
struct _LanguagesCollection : LanguagesCollection
{
    _LanguagesCollection ()
    {
        LanguagesCollectionReader(
                OpenEditor::FileInStream("\\languages.dat")
            ) >> *this;
    }
}
g_test;
#endif

};//namespace OpenEditor

