/////////////////////////////////////////////////////////////////////////////////
//
// SourceFile: SchemaReWriter.cpp
//
// BaseLibrary: Indispensable general objects and functions
// 
// Copyright (c) 2014-2022 ir. W.E. Huisman
// All rights reserved
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
#include "pch.h"
#include "SchemaReWriter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const char* tokens[] =
{
   ""
  ,""
  ,"'"
  ,"\""
  ,"."
  ,","
  ,"--"
  ,"/*"
  ,"//"
  ,"("
  ,")"
  ,"+"
  ,"||"
  ," "
  ,"\t"
  ,"\r"
  ,"\n"
  ,"SELECT"
  ,"INSERT"
  ,"UPDATE"
  ,"DELETE"
  ,"FROM"
  ,"JOIN"
  ,"WHERE"
  ,"GROUP"
  ,"ORDER"
  ,"HAVING"
  ,"INTO"
  ,"UNION"
};

///////////////////////////////////////////////////////////////////////////

SchemaReWriter::SchemaReWriter(XString p_schema)
               :m_schema(p_schema)
{

}

void
SchemaReWriter::SetOption(SROption p_option)
{
  m_options |= (int) p_option;
}

void
SchemaReWriter::SetRewriteCode(FCodes* p_codes)
{
  m_codes = p_codes;
}

XString 
SchemaReWriter::Parse(XString p_input)
{
  m_input = p_input;
  ParseStatement();

  if(m_level != 0)
  {
    MessageBox(NULL,"Odd number of '(' and ')' tokens in the statement","Warning",MB_OK|MB_ICONERROR);
  }
  return m_output;
}

void
SchemaReWriter::ParseStatement()
{
  bool begin = true;

  ++m_level;
  while(true)
  {
    m_token = GetToken();
    if(m_token == Token::TK_EOS)
    {
      break;
    }

    SkipSpaceAndComment();

    // First token in the statement
    if(begin && (m_token == Token::TK_SELECT || 
                 m_token == Token::TK_INSERT || 
                 m_token == Token::TK_UPDATE || 
                 m_token == Token::TK_DELETE))
    {
      m_inStatement = m_token;
      begin = false;
    }

    // Next level statement
    if(m_token == Token::TK_PAR_OPEN)
    {
      PrintToken();
      Token oldStatement = m_inStatement;
      bool  oldFrom      = m_inFrom;
      m_inStatement = Token::TK_EOS;
      ParseStatement();
      m_inStatement = oldStatement;
      m_inFrom      = oldFrom;
      continue;
    }

    // Append schema
    if(m_nextTable)
    {
      AppendSchema();
    }

    // Find next table for appending a schema
    if(m_inStatement == Token::TK_SELECT && (m_token == Token::TK_FROM || m_token == Token::TK_JOIN))
    {
      m_nextTable = true;
      if(m_token == Token::TK_FROM)
      {
        m_inFrom = true;
      }
    }
    if(m_inStatement == Token::TK_SELECT && m_inFrom && m_token == Token::TK_COMMA)
    {
      m_nextTable = true;
    }
    if(m_inStatement == Token::TK_INSERT && m_token == Token::TK_INTO)
    {
      m_nextTable = true;
    }
    if(m_inStatement == Token::TK_UPDATE)
    {
      m_nextTable = true;
    }
    if(m_inStatement == Token::TK_DELETE && m_token == Token::TK_FROM)
    {
      m_nextTable = true;
    }

    // End of next table
    if(m_token == Token::TK_WHERE || m_token == Token::TK_GROUP || m_token == Token::TK_ORDER || m_token == Token::TK_HAVING)
    {
      m_inFrom    = false;
      m_nextTable = false;
    }

    PrintToken();

    // Check reset on next UNION
    if(m_token == Token::TK_UNION)
    {
      begin         = true;
      m_inFrom      = false;
      m_nextTable   = false;
      m_inStatement = Token::TK_EOS;
    }

    // End of level
    if(m_token == Token::TK_PAR_CLOSE)
    {
      break;
    }
  }
  --m_level;
}

void
SchemaReWriter::PrintToken()
{
  switch(m_token)
  {
    case Token::TK_PLAIN:     PrintSpecials();
                              break;
    case Token::TK_SQUOTE:    m_output += '\'';
                              m_output += m_tokenString;
                              m_output += '\'';
                              break;
    case Token::TK_DQUOTE:    m_output += '\"';
                              m_output += m_tokenString;
                              m_output += '\"';
                              break;
    case Token::TK_COMM_SQL:  m_output += "--";
                              m_output += m_tokenString;
                              m_output += '\n';
                              break;
    case Token::TK_COMM_C:    m_output += "/*";
                              m_output += m_tokenString;
                              m_output += "/";
                              break;
    case Token::TK_COMM_CPP:  m_output += "//";
                              m_output += m_tokenString;
                              m_output += '\n';
                              break;
    case Token::TK_PAR_ADD:   m_output += (m_options & (int)SROption::SRO_ADD_TO_CONCAT) ? "||" : "+";
                              break;
    case Token::TK_PAR_CONCAT:m_output += (m_options & (int)SROption::SRO_CONCAT_TO_ADD) ? "+" : "||";
                              break;
    case Token::TK_POINT:     [[fallthrough]];
    case Token::TK_COMMA:     [[fallthrough]];
    case Token::TK_PAR_OPEN:  [[fallthrough]];
    case Token::TK_PAR_CLOSE: [[fallthrough]];
    case Token::TK_SPACE:     [[fallthrough]];
    case Token::TK_TAB:       [[fallthrough]];
    case Token::TK_CR:        [[fallthrough]];
    case Token::TK_NEWLINE:   [[fallthrough]];
    case Token::TK_SELECT:    [[fallthrough]];
    case Token::TK_INSERT:    [[fallthrough]];
    case Token::TK_UPDATE:    [[fallthrough]];
    case Token::TK_DELETE:    [[fallthrough]];
    case Token::TK_FROM:      [[fallthrough]];
    case Token::TK_JOIN:      [[fallthrough]];
    case Token::TK_WHERE:     [[fallthrough]];
    case Token::TK_GROUP:     [[fallthrough]];
    case Token::TK_ORDER:     [[fallthrough]];
    case Token::TK_HAVING:    [[fallthrough]];
    case Token::TK_INTO:      [[fallthrough]];
    case Token::TK_UNION:     m_output += tokens[(int)m_token];
                              break;
  }
}

void
SchemaReWriter::PrintSpecials()
{
  if(m_tokenString.CompareNoCase("brief_ew") == 0)
  {
    m_output += m_schema;
    m_output += '.';
  }
  m_output += m_tokenString; 
}

// THIS IS WHY WE ARE HERE IN THIS CLASS!
void
SchemaReWriter::AppendSchema()
{
  SkipSpaceAndComment();

  int ch = GetChar();

  if(ch == '.')
  {
    // There already was a schema name
    m_output += m_tokenString;
    m_token   = GetToken();
  }
  else
  {
    // Reset token
    UnGetChar(ch);

    if(!m_schema.IsEmpty())
    {
      ++m_replaced;
      m_output += m_schema;
      m_output += '.';
    }
  }

  m_nextTable = false;

  // Reset for update
  if(m_inStatement == Token::TK_UPDATE)
  {
    m_inStatement = Token::TK_EOS;
  }
}

void
SchemaReWriter::SkipSpaceAndComment()
{
  while(true)
  {
    if(m_token == Token::TK_SPACE || m_token == Token::TK_TAB || m_token == Token::TK_CR || m_token == Token::TK_NEWLINE)
    {
      PrintToken();
      m_token = GetToken();
      continue;
    }
    if(m_token == Token::TK_COMM_SQL || m_token == Token::TK_COMM_C || m_token == Token::TK_COMM_CPP)
    {
      PrintToken();
      m_token = GetToken();
      continue;
    }
    return;
  }
}


Token
SchemaReWriter::GetToken()
{
  m_tokenString.Empty();
  int ch = 0;

  if((ch = GetChar()) != (int)Token::TK_EOS)
  {
    switch(ch)
    {
      case '\'':  QuoteString(ch);
                  return Token::TK_SQUOTE;
      case '\"':  QuoteString(ch);
                  return Token::TK_DQUOTE;
      case '-':   return CommentSQL();
      case '/':   return CommentCPP();
      case '|':   return Concat();
      case '.':   return Token::TK_POINT;
      case ',':   return Token::TK_COMMA;
      case '(':   return Token::TK_PAR_OPEN;
      case ')':   return Token::TK_PAR_CLOSE;
      case '+':   return Token::TK_PAR_ADD;
      case ' ':   return Token::TK_SPACE;
      case '\t':  return Token::TK_TAB;
      case '\r':  return Token::TK_CR;
      case '\n':  return Token::TK_NEWLINE;
    }
    if(isdigit(ch))
    {
      m_tokenString = XString((char)ch,1);
      return Token::TK_PLAIN;
    }
    if(isalpha(ch))
    {
      m_tokenString = XString((char) ch,1);
      ch = GetChar();
      while(isalnum(ch) || ch == '_')
      {
        m_tokenString += (char) ch;
        ch = GetChar();
      }
      if(ch)
      {
        UnGetChar(ch);
      }
      return FindToken();
    }
    m_tokenString = XString((char) ch,1);
    return Token::TK_PLAIN;
  }
  return Token::TK_EOS;
}

Token
SchemaReWriter::FindToken()
{
  for(int ind = 0;ind < sizeof(tokens) / sizeof(const char*); ++ind)
  {
    if(m_tokenString.CompareNoCase(tokens[ind]) == 0)
    {
      return (Token) ind;
    }
  }
  // Replacement code
  if(m_codes)
  {
    FCodes::iterator it = m_codes->find(m_tokenString);
    if(it != m_codes->end())
    {
      m_tokenString = it->second;
    }
  }
  return Token::TK_PLAIN;
}

Token
SchemaReWriter::CommentSQL()
{
  int ch = GetChar();
  if(ch == '-')
  {
    while(true)
    {
      ch = GetChar();
      if(ch == 0 || ch == '\n')
      {
        break;
      }
      m_tokenString += (char) ch;
    }
    return Token::TK_COMM_SQL;
  }
  UnGetChar(ch);
  return Token::TK_PLAIN;
}

Token
SchemaReWriter::CommentCPP()
{
  int ch = GetChar();
  if(ch == '/')
  {
    while(true)
    {
      ch = GetChar();
      if(ch == 0 || ch == '\n')
      {
        break;
      }
      m_tokenString += (char) ch;
    }
    return Token::TK_COMM_CPP;
  }
  else if(ch == '*')
  {
    int lastchar = 0;
    while(true)
    {
      ch = GetChar();
      if(ch == 0 || (ch == '/' && lastchar == '*'))
      {
        break;
      }
      m_tokenString += (char) ch;
      lastchar = ch;
    }
    return Token::TK_COMM_C;
  }
  else
  {
    UnGetChar(ch);
    return Token::TK_PLAIN;
  }
}

Token
SchemaReWriter::Concat()
{
  int ch = GetChar();
  if(ch == '|')
  {
    return Token::TK_PAR_CONCAT;
  }
  UnGetChar(ch);
  return Token::TK_PLAIN;
}

void
SchemaReWriter::QuoteString(int p_ending)
{
  int ch = 0;

  while(true)
  {
    ch = GetChar();
    if(ch == p_ending || ch == 0)
    {
      return;
    }
    m_tokenString += (char) ch;
  } 
}

void
SchemaReWriter::UnGetChar(int p_char)
{
  if(m_ungetch == 0)
  {
    m_ungetch = p_char;
  }
}

int
SchemaReWriter::GetChar()
{
  if(m_ungetch)
  {
    int ch = m_ungetch;
    m_ungetch = 0;
    return ch;
  }
  if(m_position < m_input.GetLength())
  {
    return m_input.GetAt(m_position++);
  }
  return 0;
}
