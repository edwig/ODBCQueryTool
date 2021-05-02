/* 
    Copyright (C) 2003 Aleksey Kochetov

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

#ifndef __OEPlsSqlParser_h__
#define __OEPlsSqlParser_h__

#include <string>
#include "Common/Fastmap.h"


namespace OpenEditor
{
    using std::string;
    using Common::Fastmap;

    enum EToken
    {
        etUNKNOWN = 0,
        etPROCEDURE,
        etFUNCTION,
        etPACKAGE,
        etBODY,
        etDECLARE,
        etBEGIN,
        etEXCEPTION,
        etEND,
        etIF,
        etTHEN,
        etELSE,
        etELSIF,
        etFOR,
        etWHILE,
        etLOOP,
        etEXIT,
        etDOT,
        etSEMICOLON,
        etQUOTE,
        etDOUBLE_QUOTE,
        etLEFT_ROUND_BRACKET,
        etRIGHT_ROUND_BRACKET,
        etMINUS,
        etSLASH,
        etSTAR,
        etIS,
        etAS,
        etSELECT,
        etINSERT,
        etUPDATE,
        etDELETE,
        etALTER,
        etANALYZE,
        etCREATE,
        etDROP,
        etFROM,
        etWHERE,
        etSET,
        etOPEN,
        etLANGUAGE,
        // second level tokens
        etEOL,
        etEOF,
        etNUMBER,
        etDELIMITER,
        etIDENTIFIER,
        etQUOTED_STRING,
        etDOUBLE_QUOTED_STRING,
        etCOMMENT
    };

    struct Token
    {
        EToken token;
        int line, offset, length;

        Token () { memset(this, 0, sizeof(*this)); }
        operator EToken () const { return token; }
        Token& operator = (EToken tk) { token = tk; return *this; }

        static const char* GetString (EToken token);
    };

    class SyntaxAnalyser
    {
    public:
        virtual ~SyntaxAnalyser () {};
        virtual void PutToken (const Token&) = 0;
    };

    class PlsSqlParser
    {
    public:
        PlsSqlParser (SyntaxAnalyser&);
        void Clear ();

        bool PutLine (int line, const char*, int length);
        void PutEOF  (int line);

    private:
        SyntaxAnalyser& m_analyzer;
        Fastmap<bool> m_fastmap;

        enum ESequenceOf { eNone, eQuotedString, eDblQuotedString, eEndLineComment, eComment };
        ESequenceOf m_sequenceOf;
        string m_stringOf;
        Token m_sequenceToken;

        static DelimitersMap m_Delimiters;
    };

};

#endif//__OEPlsSqlParser_h__
