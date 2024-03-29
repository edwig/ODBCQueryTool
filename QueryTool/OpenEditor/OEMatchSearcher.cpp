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
#include <COMMON/AppGlobal.h>
#include <COMMON/ExceptionHelper.h>
#include "OEHelpers.h"
#include "OEContext.h"
#include "OEMatchSearcher.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


namespace OpenEditor
{
    LPCTSTR GetLexemeName (EPLSLexeme lexeme)
    {
      switch (lexeme)
      {
        case elUNKNOWN:    return _T("UNKNOWN");
        case elBLOCK:      return _T("BLOCK");
        case elSELECTION:  return _T("SELECTION");
        case elITERATION:  return _T("ITERATION");
        case elEXPRESSION: return _T("EXPRESSION");
      }

      return _T("Not found");
    }

DelimitersMap LexicalAnalyser::m_Delimiters(_T(" \t\'\\()[]{}+-*/.,!?;:=><%|@&^"));

LexicalAnalyser::LexicalAnalyser ()
{
  m_analyzer   = 0;
  m_lastToken  = etUNKNOWN;
  m_sequenceOf = eNone;

  m_tokens.insert(std::map<CString,int>::value_type(_T("DECLARE"),    etDECLARE));
  m_tokens.insert(std::map<CString,int>::value_type(_T("FUNCTION"),   etFUNCTION));
  m_tokens.insert(std::map<CString,int>::value_type(_T("PROCEDURE"),  etPROCEDURE));
  m_tokens.insert(std::map<CString,int>::value_type(_T("BEGIN"),      etBEGIN));
  m_tokens.insert(std::map<CString,int>::value_type(_T("EXCEPTION"),  etEXCEPTION));
  m_tokens.insert(std::map<CString,int>::value_type(_T("END"),        etEND));
  m_tokens.insert(std::map<CString,int>::value_type(_T("IF"),         etIF));
  m_tokens.insert(std::map<CString,int>::value_type(_T("THEN"),       etTHEN));
  m_tokens.insert(std::map<CString,int>::value_type(_T("ELSE"),       etELSE));
  m_tokens.insert(std::map<CString,int>::value_type(_T("ELSIF"),      etELSIF));
  //m_tokens.insert(std::map<CString,int>::value_type(_T("FOR"),        etFOR));
  //m_tokens.insert(std::map<CString,int>::value_type(_T("WHILE"),      etWHILE));
  m_tokens.insert(std::map<CString,int>::value_type(_T("LOOP"),       etLOOP));
  m_tokens.insert(std::map<CString,int>::value_type(_T("EXIT"),       etEXIT));
  m_tokens.insert(std::map<CString,int>::value_type(_T(";"),          etSEMICOLON));
  m_tokens.insert(std::map<CString,int>::value_type(_T("'"),          etQUOTE));
  m_tokens.insert(std::map<CString,int>::value_type(_T("\""),         etDOUBLE_QUOTE));
  m_tokens.insert(std::map<CString,int>::value_type(_T("("),          etLEFT_BRACKET));
  m_tokens.insert(std::map<CString,int>::value_type(_T(")"),          etRIGHT_BRACKET));
  m_tokens.insert(std::map<CString,int>::value_type(_T("["),          etLEFT_ROUND_BRACKET));
  m_tokens.insert(std::map<CString,int>::value_type(_T("]"),          etRIGHT_ROUND_BRACKET));
  m_tokens.insert(std::map<CString,int>::value_type(_T("-"),          etMINUS));
  m_tokens.insert(std::map<CString,int>::value_type(_T("/"),          etSLASH));
  m_tokens.insert(std::map<CString,int>::value_type(_T("*"),          etSTAR));

  m_fastmap.erase();

  for(auto& tok : m_tokens)
  {
      m_fastmap[tok.first[0]] = true;
  }
}

void LexicalAnalyser::Join (SyntaxAnalyser* analyser)
{
    m_analyzer  = analyser;
    m_lastToken = etUNKNOWN;
    m_sequenceOf = eNone;
}

bool LexicalAnalyser::PutLine (LPCTSTR str, int line, int offset, int length, bool backward)
{
    _ASSERTE(m_analyzer);

    CString reversed;

    if (!str) // EOF
    {
        _CHECK_AND_THROW_(!(m_sequenceOf && m_sequenceOf != eEndLineComment),_T("Unexpected end of file!"));

        TokenInfo tokenInfo;
        tokenInfo.m_token  = etEOF;
        tokenInfo.m_line   = line;
        tokenInfo.m_offset = offset;
        tokenInfo.m_length = 0;
        m_analyzer->PutToken(tokenInfo, CString());

        return false;
    }
    else if (backward)
    {
        offset = 0; // if backward then ignore offset

        for (; offset < length; offset++)
            if (str[offset] == '-' && str[offset+1] == '-')
            {
                if (length > offset) // it's a doubtful situation
                {
                    length = offset;
                }
                break;
            }

        offset = 0; // if backward then ignore offset

        while (1) // reorder a token sequence
        {
            // skip white space
            for (; offset < length && isspace(str[offset]); offset++)
            {
                TCHAR buf[2];
                buf[0] = str[offset];
                buf[1] = 0;
                CString c(buf);
                reversed = c + reversed;
            }
            // check EOL
            if (!(offset < length))
                break;

            // read CString token
            if (m_Delimiters[str[offset]])
            {
              TCHAR buf[2];
              buf[0] = str[offset++];
              buf[1] = 0;
              CString c(buf);
              reversed = c + reversed;
            }
            else
            {
                CString buffer;
                for(; offset < length && !m_Delimiters[str[offset]]; offset++)
                {
                  buffer += (TCHAR) toupper(str[offset]);
                }
                reversed = buffer + reversed;
            }
        }
        str = reversed.GetString();
        offset = 0;
    }

    while (1)
    {
        CString buffer;
        EPLSToken token = etUNKNOWN;

        if (m_sequenceOf == eNone)
        {
            // skip white space
            for (; offset < length && isspace(str[offset]); offset++)
                {}

            // check EOL
            if (!(offset < length))
                break;

            // read CString token
            if (m_Delimiters[str[offset]])
                buffer += str[offset++];
            else
                for (; offset < length && !m_Delimiters[str[offset]]; offset++)
                    buffer += (TCHAR)toupper(str[offset]);
        }
        else
        {
            switch (m_sequenceOf)
            {
            case eEndLineComment:
                m_stringOf.Append(str + offset, length - offset);
                offset = length;
                m_sequenceOf = eNone;
                token = etCOMMENT;
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
                        token = eQUOTED_STRING;
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
                        token = etDOUBLE_QUOTED_STRING;
                        break;
                    }
                }
                break;
            case eComment:
                for (; offset < length; offset++)
                {
                    if (str[offset] != '*' && str[offset] != '/')
                        m_stringOf += str[offset];
                    else
                    {
                        offset += 2;
                        m_sequenceOf = eNone;
                        token = etCOMMENT;
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

            // check EOL
//            if (!(offset < length))
//                break;
        }


        if (token == etUNKNOWN && !buffer.IsEmpty() && m_fastmap[buffer.GetAt(0)])
        {
            std::map<CString, int>::const_iterator it = m_tokens.find(buffer);

            if (it != m_tokens.end())
            {
                token = (EPLSToken)(it->second);

                switch (token)
                {
                case etQUOTE:
                    m_sequenceOf = eQuotedString; // start CString accumulation
                    break;
                case etDOUBLE_QUOTE:
                    m_sequenceOf = eDblQuotedString; // start CString accumulation
                    break;
                case etMINUS:
                    //if (m_lastToken == etMINUS)
                    if (str[offset] == '-')
                    {
                        m_sequenceOf = eEndLineComment; // skip line remainder
                        offset++;
                    }
                    break;
                case etSTAR:
                    if (m_lastToken == etSLASH)
                        m_sequenceOf = eComment; // skip comment
                    break;
//                case etSLASH:
//                    if (m_lastToken == etSTAR)
//                        _CHECK_AND_THROW_(0, "Unexpected comment end token!");
                }
            }
        }

        m_lastToken = token;

        if (m_sequenceOf == eNone)
        {

            TokenInfo tokenInfo;
            tokenInfo.m_token  = token;
            tokenInfo.m_line   = line;
            tokenInfo.m_length = (int)buffer.GetLength();

            tokenInfo.m_offset = !backward
                                ? (offset - (int)buffer.GetLength())
                                : (length - offset);

            if (!m_analyzer->PutToken(tokenInfo, buffer))
                return false;
        }

        // check EOL
        if (!(offset < length))
            break;

    }

    return true;
}


StartTokenFinder::StartTokenFinder ()
{
    m_step = 0;
    m_lexemeInfo.m_lexeme = elUNKNOWN;
    m_backward = false;
}

bool StartTokenFinder::PutToken (const TokenInfo& tokenInfo, const CString& /*str*/)
{
    if (m_step++ == 0)
    {
        m_lexemeInfo.AddToken(tokenInfo);

        switch (tokenInfo.m_token)
        {
        case etDECLARE:
        case etFUNCTION:
        case etPROCEDURE:
        case etBEGIN:
        case etEXCEPTION:
            m_lexemeInfo.m_lexeme = elBLOCK;
            break;
        case etELSE:
        case etELSIF:
            m_lexemeInfo.m_lexeme = elSELECTION;
            break;
        //case etFOR:
        //case etWHILE:
// exit can be in embedded bloc
//case etEXIT:
//    m_lexemeInfo.m_lexeme = elITERATION;
//    break;
        case etLEFT_BRACKET:
        case etLEFT_ROUND_BRACKET:
            m_lexemeInfo.m_lexeme = elEXPRESSION;
            break;
        case etRIGHT_BRACKET:
        case etRIGHT_ROUND_BRACKET:
            m_lexemeInfo.m_lexeme = elEXPRESSION;
            m_backward = true;
            break;
        case etIF:
        case etEND:
        case etLOOP:
            return true; // need a next token
        default:
            ; // nothing
        }
    }
    else // the second step
    {
        switch (m_lexemeInfo.LastToken())
        {
        case etIF:
            if (tokenInfo.m_token != etSEMICOLON)
            {
                m_lexemeInfo.m_lexeme = elSELECTION;
            }
            else
            {
                m_lexemeInfo.m_lexeme = elSELECTION;
                m_backward = true;
            }
            break;
        case etLOOP:
            if (tokenInfo.m_token != etSEMICOLON)
            {
                m_lexemeInfo.m_lexeme = elITERATION;
            }
            else
            {
                m_lexemeInfo.m_lexeme = elITERATION;
                m_backward = true;
            }
            break;
        case etEND:
            switch (tokenInfo.m_token)
            {
            default:
                m_lexemeInfo.m_lexeme = elBLOCK;
                m_backward = true;
                break;
            case etIF:
                m_lexemeInfo.m_lexeme = elSELECTION;
                m_backward = true;
                break;
            case etLOOP:
                m_lexemeInfo.m_lexeme = elITERATION;
                m_backward = true;
                break;
            }
            break;
        }
    }

    return false; // stop here
}

bool MatchAnalyser::PutToken (const TokenInfo& tokenInfo, const CString& str)
{
    if (!m_backward)
        return putToken(tokenInfo, str);
    else
        return putTokenBackward(tokenInfo, str);
}

bool MatchAnalyser::putToken (const TokenInfo& tokenInfo, const CString& str)
{
    TRACE1("MatchAnalyser::putToken: %s\n", str.GetString());

    LexemeInfo &top = m_stack.top();
    //const EPLSToken prevToken = top.LastToken();
    int level = (int)m_stack.size();
    bool nothing = false;
    bool check_for_stop = false;

    switch (tokenInfo.m_token)
    {
    case etDECLARE:
    case etFUNCTION:
    case etPROCEDURE:
        // always new lexeme
        open(elBLOCK, tokenInfo);
        break;
    case etBEGIN:
        if (top.m_lexeme == elBLOCK
          && (top.LastToken() == etDECLARE
            || top.LastToken() == etFUNCTION
            || top.LastToken() == etPROCEDURE)) // next step
        {
            top.AddToken(tokenInfo);
            check_for_stop = true;
        }
        else // new lexeme
        {
            open(elBLOCK, tokenInfo);
        }
        break;
    case etEXCEPTION:
        if (top.m_lexeme == elBLOCK)
        {
            if (top.LastToken() == etBEGIN)
            {
                top.AddToken(tokenInfo);
                check_for_stop = true;
            }
            else // declare, procedure or function
                 nothing = true; // else ignore because it is declaration token too
        }
        break;
    case etELSE:
    case etELSIF:
        _CHECK_AND_THROW_(top.m_lexeme == elSELECTION, _T("Unexpected token!"));
        //ignore an order and second else
        top.AddToken(tokenInfo);
        check_for_stop = true;
        break;
    //case etFOR:
    //case etWHILE:
    //    // always new lexeme
    //    open(elITERATION, tokenInfo);
    //    break;

// exit can be in embedded bloc
//case etEXIT:
//    _CHECK_AND_THROW_(top.m_lexeme == elITERATION, "Unexpected token!");
//    top.AddToken(tokenInfo);
//    check_for_stop = true;
//    //ignore an order
//    break;
    case etLEFT_BRACKET:
    case etLEFT_ROUND_BRACKET:
        // always new lexeme
        open(elEXPRESSION, tokenInfo);
        break;
    case etRIGHT_BRACKET:
    case etRIGHT_ROUND_BRACKET:
        _CHECK_AND_THROW_(top.m_lexeme == elEXPRESSION
                          && ((top.LastToken() == etLEFT_BRACKET && tokenInfo.m_token == etRIGHT_BRACKET)
                             ||(top.LastToken() == etLEFT_ROUND_BRACKET && tokenInfo.m_token == etRIGHT_ROUND_BRACKET)),
                          _T("Unexpected token!"));
        close(tokenInfo); // close lexeme
        check_for_stop = true;
        break;
    case etEND:
        if (top.m_lexeme == elBLOCK)
        {
            close(tokenInfo); // close lexeme but maybe to get semicolon it's better?
            check_for_stop = true;
        }
        else
            top.AddToken(tokenInfo);
        break;
    case etIF:
        if (top.m_lexeme != elSELECTION)
        {
            open(elSELECTION, tokenInfo);
        }
        else
        {
            if (top.LastToken() != etEND)
                open(elSELECTION, tokenInfo);
            else
            {
                close(tokenInfo); // close lexeme
                check_for_stop = true;
            }
        }
        break;
    case etLOOP:
        if (top.m_lexeme != elITERATION)
        {
            open(elITERATION, tokenInfo);
        }
        else
        {
            switch (top.LastToken())
            {
            //case etWHILE:
            //case etFOR:
            //    top.AddToken(tokenInfo); // next step
            //    check_for_stop = true;
            //    break;
            case etEND:
                close(tokenInfo); // close lexeme
                check_for_stop = true;
                break;
            default:
                open(elITERATION, tokenInfo);
            }
        }
        break;
    default:
        nothing = true; // nothing
    }

    if (nothing) // it's not recognized
    {
        return !m_stack.empty();
    }
    else if (m_stack.empty()) // something has happened
    {
        return false;
    }
//    else if (level == 1 && m_stack.size() == 1 && prevToken != top.LastToken())
    else if (check_for_stop && level == 1 && m_stack.size() == 1)
    {
        m_lastLexemeInfo = m_stack.top();
        return false;
    }
    else
        return true;
}

//
// BLOCK
// end       - start
// exception - skip
// begin     - stop and defer before a next recognized token
// declare   - if defer stop here else skip
//
// SELECTION:
// if end    - start
// else      - skip
// else if   - skip
// if        - stop
//
// ITERATION:
// loop end  - start
// exit      - skip
// loop      - stop and defer
// for|while - stop if deferred
//
// EXPRESSION:
// -//-
//

bool MatchAnalyser::putTokenBackward (const TokenInfo& tokenInfo, const CString& str)
{
    TRACE1("MatchAnalyser::putTokenBackward: %s\n", str.GetString());

    LexemeInfo &top = m_stack.top();

    try
    {

        switch (tokenInfo.m_token)
        {
        case etEND:
            if (top.m_lexeme == elSELECTION && top.LastToken() == etIF)
                top.AddToken(tokenInfo);
            else if (top.m_lexeme == elITERATION && top.LastToken() == etLOOP)
                top.AddToken(tokenInfo);
            else
                open(elBLOCK, tokenInfo);
            break; // new lexeme
        case etEXCEPTION:
            if (top.m_lexeme == elBLOCK && top.LastToken() == etEND)
                top.AddToken(tokenInfo);
            //else ignore because it is declaration token too
            break;
        case etBEGIN:
            closeAllDeferred(top);

            if (!(top.m_lexeme == elBLOCK && top.LastToken() != etBEGIN))
                throw UnexpectedToken();

            defer(tokenInfo); // close lexeme (!DEFERRED CLOSING)
            break;
        case etDECLARE:
        case etPROCEDURE:
        case etFUNCTION:
            // if deferred closing then final closing
            // else ignore
            if (top.m_lexeme != elBLOCK)
                throw UnexpectedToken();
            closeDeferred(tokenInfo); // close lexeme
            break;
        case etELSE:
        case etELSIF:
            closeAllDeferred(top);
            if (top.m_lexeme != elSELECTION)
                throw UnexpectedToken();
            //ignore an order and second else
            top.AddToken(tokenInfo);
            break;
        //case etFOR:
        //case etWHILE:
        //    // if deferred closing then final closing
        //    // else ignore
        //    if (top.m_lexeme != elITERATION)
        //        throw UnexpectedToken();
        //    closeDeferred(tokenInfo); // close lexeme
        //    break;
// exit can be in embedded bloc
//case etEXIT:
//    closeAllDeferred(top);
//    if (top.m_lexeme != elITERATION)
//        throw UnexpectedToken();
//    top.AddToken(tokenInfo);
//    //ignore an order
//    break;
        case etRIGHT_BRACKET:
        case etRIGHT_ROUND_BRACKET:
            // always new lexeme
            open(elEXPRESSION, tokenInfo);
            break;
        case etLEFT_BRACKET:
        case etLEFT_ROUND_BRACKET:
            if (!(top.m_lexeme == elEXPRESSION
                  && ((top.LastToken() == etRIGHT_BRACKET && tokenInfo.m_token == etLEFT_BRACKET)
                       ||(top.LastToken() == etRIGHT_ROUND_BRACKET && tokenInfo.m_token == etLEFT_ROUND_BRACKET))))
                throw UnexpectedToken();
            close(tokenInfo); // close lexeme
            break;
        case etIF:
            if (m_lastTokenInfo.m_token == etSEMICOLON)
            {
                open(elSELECTION, tokenInfo);
            }
            else
            {
                closeAllDeferred(top);
                if (top.m_lexeme != elSELECTION)
                    throw UnexpectedToken();
                close(tokenInfo); // close lexeme
            }
            break;
        case etLOOP:
            if (m_lastTokenInfo.m_token == etSEMICOLON)
            {
                open(elITERATION, tokenInfo);
            }
            else
            {
                closeAllDeferred(top);
                if (!(top.m_lexeme == elITERATION && top.LastToken() != etLOOP))
                    throw UnexpectedToken();
                defer(tokenInfo); // close lexeme (!DEFERRED CLOSING)
            }
            break;
        default:
            ; // nothing
        }
        m_lastTokenInfo = tokenInfo;

    }
    catch (const UnexpectedToken&)
    {
        if (m_stack.size())
        {
            // ignore all before the first lexeme
            while (m_stack.size() > 1)
                m_stack.pop();

            // check the first lexeme
            if (m_stack.size() == 1 && m_stack.top().IsDeferred())
            {
                m_lastLexemeInfo = m_stack.top();
                m_stack.pop();
            }
            else
                throw;
        }
    }

    return !m_stack.empty();
}

/*
    it's a very primitive match algorithm
    at least it should handle comments
    search for comments too
*/
void GeneralMatchSearcher::Find (EditContext& editor, bool select)
{
    TCHAR openBraces [] = _T("([{");
    TCHAR closeBraces[] = _T(")]}");

    Fastmap<TCHAR> fastmap;
    for (int i(0); i < sizeof(openBraces)-1; i++)
    {
        fastmap[openBraces [i]] = closeBraces[i];
        fastmap[closeBraces[i]] = openBraces [i];
    }

    Square sqr;
    CString buff;
    if (editor.WordFromPoint(editor.GetPosition(), sqr))
        editor.GetBlock(buff, &sqr);
    else
        editor.GetSelection(sqr);

    bool backward, for_any = false;
    int  balance ;
    
    if (buff.GetLength() == 1 && fastmap[buff[0]]) // search for match
    {
        TCHAR startWith = buff[0];
        TCHAR searchFor = fastmap[startWith];
        backward = _tcsrchr(closeBraces, startWith) ? true : false;
        balance = 0;
        
        fastmap.erase();
        fastmap[startWith] = 1;
        fastmap[searchFor] = -1;
    }
    else // search for any
    {
        for_any = true;
        backward = false;
        balance = 1;
        for (int i(0); i < sizeof(openBraces)-1; i++)
        {
            fastmap[openBraces[i]]  = 1;
            fastmap[closeBraces[i]] = -1;
        }
    }

    LPCTSTR str = 0;
    int col = -1, line = -1, length = -1;
    int nlines = editor.GetLineCount();
    int offset = editor.PosToInx(sqr.end.line, sqr.start.column);

    if (!backward)
        for (line = sqr.end.line; line < nlines; line++)
        {
            editor.GetLine(line, str, length);
            for (col = offset; col < length; col++)
            {
                balance += fastmap[str[col]];
                if (!balance) break;
            }
            if (!balance) break;
            offset = 0;
        }
    else
        for (line = sqr.end.line; line >= 0; line--)
        {
            editor.GetLine(line, str, length);
            for (col = (offset != -1) ? offset : (length - 1); col >= 0; col--)
            {
                balance += fastmap[str[col]];
                if (!balance) break;
            }
            if (!balance) break;
            offset = -1;
        }

    if (!balance)
    {
        editor.ClearSelection();
        sqr.end.line   = line;
        sqr.end.column = editor.InxToPos(line, col);

        if (!select)
        {
            editor.MoveTo(sqr.end);
        }
        else
        {
            if (for_any)
            {
                editor.MoveTo(sqr.end);
                Find(editor, true); // try again!
            }
            else
            {
                if (backward)
                    sqr.start.column++;
                else
                    sqr.end.column++;

                editor.MoveTo(sqr.end);
                editor.SetSelection(sqr);
            }
        }
    }
}

void PlSqlMatchSearcher::Find (EditContext& editor, bool select)
{
    Square sqr;
    if (editor.WordFromPoint(editor.GetPosition(), sqr))
    {
        CString buff;
        editor.GetBlock(buff, &sqr);

        TRACE("Start with: \"%s\"\n", buff.GetString());

        int length;
        LPCTSTR str;
        int nlines = editor.GetLineCount();
        int offset = editor.PosToInx(sqr.end.line, sqr.start.column);

        LexicalAnalyser lexer;
        StartTokenFinder starter;
        lexer.Join(&starter);

        int line;
        for (line = sqr.end.line; line < nlines; line++)
        {
            editor.GetLine(line, str, length);

	        if (!lexer.PutLine(str, line, offset, length, false))
                break;

            offset = 0;
        }

        bool backward;
        LexemeInfo lexemeInfo;

        if (starter.FoundOut(lexemeInfo, backward))
        {
            MatchAnalyser matcher(lexemeInfo, backward);
            lexer.Join(&matcher);

            if (!backward)
            {
                if (select)
                {
                    if (!editor.IsSelectionEmpty())
                    {
                        editor.GetSelection(sqr);
                    }
                    else
                    {
                        sqr.start.line   = lexemeInfo.LastTokenInfo().m_line;
                        sqr.start.column = editor.InxToPos(sqr.start.line, lexemeInfo.LastTokenInfo().m_offset);
                    }
                }

                offset = lexemeInfo.LastTokenInfo().m_offset
                         + lexemeInfo.LastTokenInfo().m_length;

                for (line = sqr.end.line; line < nlines; line++)
                {
                    editor.GetLine(line, str, length);

	                if (!lexer.PutLine(str, line, offset, length, false))
                        break;

                    offset = 0;
                }
            }
            else
            {
                if (select)
                {
                    sqr.end.line   = lexemeInfo.LastTokenInfo().m_line;
                    sqr.end.column = editor.InxToPos(sqr.end.line, lexemeInfo.LastTokenInfo().m_offset
                                                                  + lexemeInfo.LastTokenInfo().m_length);
                }

                offset = lexemeInfo.LastTokenInfo().m_offset;

                for (line = sqr.end.line; line >= 0; line--)
                {
                    editor.GetLine(line, str, length);

                    if (!lexer.PutLine(str, line, 0, (line == sqr.end.line) ? offset : length, true))
                        break;

                    offset = 0;
                }
            }

            if (matcher.FoundOut(lexemeInfo))
            {
                editor.ClearSelection();

                if (!select)
                {
                    sqr.end.line   = lexemeInfo.LastTokenInfo().m_line;
                    sqr.end.column = editor.InxToPos(sqr.end.line, lexemeInfo.LastTokenInfo().m_offset);
                    editor.MoveTo(sqr.end);
                }
                else
                {
                    if (backward)
                    {
                        sqr.start.line   = lexemeInfo.LastTokenInfo().m_line;
                        sqr.start.column = editor.InxToPos(sqr.start.line, lexemeInfo.LastTokenInfo().m_offset);
                    }
                    else
                    {
                        sqr.end.line   = lexemeInfo.LastTokenInfo().m_line;
                        sqr.end.column = editor.InxToPos(sqr.end.line, lexemeInfo.LastTokenInfo().m_offset
                                                              + lexemeInfo.LastTokenInfo().m_length);
                    }
                    editor.SetSelection(sqr);
                    editor.MoveTo(sqr.end);
                }
            }
            else
                Common::SetStatusText(_T("Can't find a match"));
        }
        else
            Common::SetStatusText(_T("Nothing for match searching"));

    }
}

MatchSearcherBasePtr MatchSearcherFactory::CreateMatchSearcher (LPCTSTR lang)
{
    if (!_tcscmp(lang, _T("SQL")))
        return MatchSearcherBasePtr(new PlSqlMatchSearcher);
    else
        return MatchSearcherBasePtr(new GeneralMatchSearcher);
}

};
