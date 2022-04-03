////////////////////////////////////////////////////////////////////////
//
// File: SQLStatement.h
//
// Copyright (c) 1998-2022 ir. W.E. Huisman
// All rights reserved
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of 
// this software and associated documentation files (the "Software"), 
// to deal in the Software without restriction, including without limitation the rights 
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies 
// or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION 
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// Version number: See SQLComponents.h
//
#pragma once

namespace SQLComponents
{

class SQLStatement
{
public :
  SQLStatement();
  SQLStatement(int p_number,char* p_statement,int p_minimum,int p_maximum);
 ~SQLStatement();
  bool    CheckRecords(int p_records);
  void    AddToStatement(XString& p_statement);

  // SETTERS

  void    SetNumber(int p_number)           { m_number      = p_number;     };
  void    SetExplanation(XString p_explain) { m_explanation = p_explain;    };
  void    SetStatement(XString p_statement) { m_statement   = p_statement;  };
  void    SetMinNumber(int p_number)        { m_minNumber   = p_number;     };
  void    SetMaxNumber(int p_number)        { m_maxNumber   = p_number;     };
  void    SetCustomer(bool p_customer)      { m_customer    = p_customer;   };

  // GETTERS

  int     GetNumber()                       { return m_number;      };
  XString GetExplanation()                  { return m_explanation; };
  XString GetStatement()                    { return m_statement;   };
  int     GetMinNumber()                    { return m_minNumber;   };
  int     GetMaxNumber()                    { return m_maxNumber;   };
  bool    GetCustomer()                     { return m_customer;    };

private:
  int	    m_number;
  XString	m_explanation;
  XString	m_statement;
  int	    m_minNumber;
  int	    m_maxNumber;
  bool 	  m_customer;  // Specific for a customer
};

// End of namespace
}