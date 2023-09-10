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
    16.03.2003 bug fix, plsql match sometimes fails after some edit operations
*/

#include "pch.h"
#include <map>
#include <COMMON/ExceptionHelper.h>
#include "OEContext.h"
#include "OEPlsSqlParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


namespace OpenEditor
{
    using std::map;

	static struct 
	{
		EToken token;
		const char* keyword;
	}
	g_tokenDescs[] = 
	{
		etDECLARE,				"DECLARE",	   
		etFUNCTION,				"FUNCTION",	
		etPROCEDURE,			"PROCEDURE",
		etPACKAGE,				"PACKAGE",	
		etBODY,					"BODY",		
		etBEGIN,				"BEGIN",	
		etEXCEPTION,			"EXCEPTION",
		etEND,					"END",		
		etIF,					"IF",		
		etTHEN,					"THEN",		
		etELSE,					"ELSE",		
		etELSIF,				"ELSIF",	
		etFOR,					"FOR",		
		etWHILE,				"WHILE",	
		etLOOP,					"LOOP",		
		etEXIT,					"EXIT",		
		etIS,					"IS",		
		etAS,					"AS",       
		etSEMICOLON,			";",        
		etQUOTE,				"'",		
		etDOUBLE_QUOTE,			"\"",		
		etLEFT_ROUND_BRACKET,	"(",		
		etRIGHT_ROUND_BRACKET,	")",		
		etMINUS,				"-",		
		etSLASH,				"/",		
		etSTAR,					"*",		
		etSELECT,				"SELECT",	
		etINSERT,				"INSERT",	
		etUPDATE,				"UPDATE",	
		etDELETE,				"DELETE",	
		etALTER,				"ALTER",	
		etANALYZE,				"ANALYZE",	
		etCREATE,				"CREATE",	
		etDROP,					"DROP",		
		etFROM,					"FROM",
		etWHERE,				"WHERE",
		etSET,					"SET",
		etOPEN,                 "OPEN",
        etLANGUAGE,             "LANGUAGE"
	};

    const char* Token::GetString (EToken token)
	{
		for (int i(0); i < sizeof(g_tokenDescs)/sizeof(g_tokenDescs[0]); i++)
			if (g_tokenDescs[i].token == token)
				return g_tokenDescs[i].keyword;
		return "Unknown Token";
	}
        
	static class TokenDescsMap : public map<CString, int> 
	{ 
	public:
		TokenDescsMap ()
		{
			for (int i(0); i < sizeof(g_tokenDescs)/sizeof(g_tokenDescs[0]); i++)
				insert(std::map<CString, int>::value_type(g_tokenDescs[i].keyword, g_tokenDescs[i].token));
		}
	} 
	g_tokenDescsMap;

	
	DelimitersMap PlsSqlParser::m_Delimiters(" \t\'\\()[]{}+-*/.,!?;:=><%|@&^");


PlsSqlParser::PlsSqlParser (SyntaxAnalyser& analyzer)
	: m_analyzer(analyzer)
{
    m_sequenceOf = eNone;

    m_fastmap.erase();

    for (std::map<CString, int>::const_iterator it = g_tokenDescsMap.begin();
        it != g_tokenDescsMap.end();
        it++)
    {
		m_fastmap[it->first[0]] = true;
    }
}

// 16.03.2003 bug fix, plsql match sometimes fails after some edit operations
void PlsSqlParser::Clear ()
{
    m_sequenceOf = eNone;
}

void PlsSqlParser::PutEOF (int line)
{
	Token token;
	token = etEOF;
    token.line = line;
    token.offset = 0;
	token.length = 0;
	m_analyzer.PutToken(token);
}

bool PlsSqlParser::PutLine (int line, const char* str, int length)
{
    //TRACE("PutLine: %4d:%s\n", line+1, string(str, length).c_str());

    Token token;

	for (int offset = 0; offset < length; )
    {
        token = etUNKNOWN;
        token.line = line;
        token.offset = offset;
		token.length = 0;

        CString buffer;

        if (m_sequenceOf == eNone)
        {
            // skip white space
            for (; offset < length && isspace(str[offset]); offset++)
                {}

            // check EOL
            if (!(offset < length))
                break;

			token.offset = offset;
            // read string token
            if (m_Delimiters[str[offset]])
                buffer += str[offset++];
            else
                for (; offset < length && !m_Delimiters[str[offset]]; offset++)
                    buffer += (char)toupper(str[offset]);

			token.length = buffer.GetLength();
        }
        else
        {
            switch (m_sequenceOf)
            {
            case eEndLineComment:
                m_stringOf.Append(str + offset, length - offset);
                offset = length;
                m_sequenceOf = eNone;
				token = m_sequenceToken;
                break;
            case eQuotedString:
                for (; offset < length; offset++)
                {
                    if (str[offset] != '\'')
                        m_stringOf += str[offset];
                    else
                    {
                        offset++;
                        m_sequenceOf = eNone;
						token = m_sequenceToken;
                        break;
                    }
                }
                break;
            case eDblQuotedString:
                for (; offset < length; offset++)
                {
                    if (str[offset] != '\"')
                        m_stringOf += str[offset];
                    else
                    {
                        offset++;
                        m_sequenceOf = eNone;
						token = m_sequenceToken;
                        break;
                    }
                }
                break;
            case eComment:
                for (; offset < length; offset++)
                {
                    if (str[offset] != '*' || str[offset+1] != '/')
                        m_stringOf += str[offset];
                    else
                    {
                        offset += 2;
                        m_sequenceOf = eNone;
						token = m_sequenceToken;
                        break;
                    }
                }
                break;
            }

            // sequence end
            if (m_sequenceOf == eNone)
            {
                buffer = m_stringOf;
                m_stringOf.Empty();
            }
        }


        if (token == etUNKNOWN && !buffer.IsEmpty() && m_fastmap[buffer.GetAt(0)])
        {
            std::map<CString, int>::const_iterator it = g_tokenDescsMap.find(buffer);

            if (it != g_tokenDescsMap.end())
            {
				token = (EToken)it->second;

                switch (token)
                {
                case etQUOTE:
                    m_sequenceOf = eQuotedString; // start string accumulation
					m_sequenceToken = etQUOTED_STRING;
					m_sequenceToken.line = token.line;
					m_sequenceToken.offset = token.offset;
					m_sequenceToken.length = token.length;
                    break;
                case etDOUBLE_QUOTE:
                    m_sequenceOf = eDblQuotedString; // start string accumulation
					m_sequenceToken = etDOUBLE_QUOTED_STRING;
					m_sequenceToken.line = token.line;
					m_sequenceToken.offset = token.offset;
					m_sequenceToken.length = token.length;
                    break;
                case etMINUS:
                    if (str[offset] == '-')
                    {
                        m_sequenceOf = eEndLineComment; // skip line remainder
						m_sequenceToken = etCOMMENT;
						m_sequenceToken.line = token.line;
						m_sequenceToken.offset = token.offset;
						m_sequenceToken.length = token.length;
                        offset++;
                    }
                    break;
                case etSLASH:
                    if (str[offset] == '*')
                    {
                        m_sequenceOf = eComment; // skip comment
						m_sequenceToken = etCOMMENT;
						m_sequenceToken.line = token.line;
						m_sequenceToken.offset = token.offset;
						m_sequenceToken.length = token.length;
                        offset++;
                    }
                    break;
                }
            }
        }

        if (m_sequenceOf == eNone)
        {
//			TRACE("PutToken %s(%s)\n", get_token_keyword(token), buffer.c_str());
			m_analyzer.PutToken(token);
        }
    }

    if (m_sequenceOf == eEndLineComment)
        m_sequenceOf = eNone;
	//token = etEOL;
	//token.line = line;
	//token.offset = length;
	//m_sequenceToken.length = 0;
	//m_analyzer.PutToken(token);

    return true;
}

};