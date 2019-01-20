////////////////////////////////////////////////////////////////////////
//
// File: SQLFilter.h
//
// Copyright (c) 1998-2018 ir. W.E. Huisman
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
// Last Revision:   20-01-2019
// Version number:  1.5.4
//
#pragma once
#include "SQLComponents.h"
#include "SQLVariant.h"
#include <vector>

namespace SQLComponents
{

// Operator for a condition filter
typedef enum _sqlOperator
{
   OP_NOP = 0
  ,OP_Equal = 1
  ,OP_NotEqual
  ,OP_Greater
  ,OP_GreaterEqual
  ,OP_Smaller
  ,OP_SmallerEqual
  ,OP_IsNULL
  ,OP_IsNotNULL
  ,OP_LikeBegin
  ,OP_LikeMiddle
  ,OP_IN
  ,OP_Between
  ,OP_Exists
  ,OP_OR               // OR chaining to next filter(s)
}
SQLOperator;

// Forward declarations
class SQLRecord;
class SQLQuery;
// Mapping typedefs
typedef std::vector<SQLVariant*> VariantSet;

// Translate a string from the message to an operator
SQLOperator StringToSQLOperator(CString p_oper);
CString     SQLOperatorToString(SQLOperator p_oper);

// THE FILTER CLASS

class SQLFilter
{
public:
  // Construct a filter
  SQLFilter(CString p_field,SQLOperator p_operator);
  SQLFilter(CString p_field,SQLOperator p_operator,SQLVariant* p_value);
  SQLFilter(CString p_field,SQLOperator p_operator,int         p_value);
  SQLFilter(CString p_field,SQLOperator p_operator,CString     p_value);
  SQLFilter(SQLOperator p_operator);
  SQLFilter(SQLFilter* p_other);
  SQLFilter(SQLFilter& p_other);
 ~SQLFilter();

  // Adding extra values for the IN or BETWEEN operators
  void        AddValue(SQLVariant* p_value);
  // Adding an expression as replacement for concrete values
  void        AddExpression(CString p_expression);
  // Negate the filter
  void        Negate();
  // Setting a parenthesis level
  void        SetOpenParenthesis();
  void        SetCloseParenthesis();

  // OPERATIONS

  // Getting the SQL Condition filter
  CString     GetSQLFilter(SQLQuery& p_query);
  // Match a record to the filter internally
  bool        MatchRecord(SQLRecord* p_record);

  // GETTERS

  // Getting the elements of the filter
  CString     GetField();
  SQLOperator GetOperator();
  SQLVariant* GetValue();
  SQLVariant* GetValue(int p_number);
  CString     GetExpression();
  bool        GetNegate();

  SQLFilter&  operator=(const SQLFilter& p_other);
private:
  // Check that we have at least one operand value
  void        CheckValue();
  void        CheckTwoValues();
  // Constructing the default operand
  void        ConstructOperand(CString& p_sql,SQLQuery& p_query);
  // Constructing the LIKE clause
  void        ConstructLike(CString& p_sql);
  // Constructing the IN clause
  void        ConstructIN(CString& p_sql,SQLQuery& p_query);
  // Constructing the BETWEEN clause
  void        ConstructBetween(CString& p_sql,SQLQuery& p_query);

  // Internal matching
  bool        MatchEqual       (SQLVariant* p_field);
  bool        MatchNotEqual    (SQLVariant* p_field);
  bool        MatchGreater     (SQLVariant* p_field);
  bool        MatchGreaterEqual(SQLVariant* p_field);
  bool        MatchSmaller     (SQLVariant* p_field);
  bool        MatchSmallerEqual(SQLVariant* p_field);
  bool        MatchLikeBegin   (SQLVariant* p_field);
  bool        MatchLikeMiddle  (SQLVariant* p_field);
  bool        MatchIsNULL      (SQLVariant* p_field);
  bool        MatchIsNotNull   (SQLVariant* p_field);
  bool        MatchIN          (SQLVariant* p_field);
  bool        MatchBetween     (SQLVariant* p_field);

  // Data for the filter
  CString     m_field;              // Name of the field to act upon
  SQLOperator m_operator;           // Operator of the condition
  VariantSet  m_values;             // Multiple values for IN and BETWEEN
  CString     m_expression;         // Free expression (no values!)
  bool        m_negate { false };   // Negate the whole condition
  bool        m_openParenthesis  { false };
  bool        m_closeParenthesis { false };
};

inline CString
SQLFilter::GetField()
{
  return m_field;
}

inline SQLOperator
SQLFilter::GetOperator()
{
  return m_operator;
}

inline SQLVariant*
SQLFilter::GetValue()
{
  return m_values.front();
}

inline void
SQLFilter::AddValue(SQLVariant* p_value)
{
  SQLVariant* value = new SQLVariant(p_value);
  m_values.push_back(value);
}

inline void
SQLFilter::AddExpression(CString p_expression)
{
  m_expression = p_expression;
}

inline void
SQLFilter::Negate()
{
  m_negate = !m_negate;
}

inline CString
SQLFilter::GetExpression()
{
  return m_expression;
}

inline bool
SQLFilter::GetNegate()
{
  return m_negate;
}

inline void
SQLFilter::SetOpenParenthesis()
{
  m_openParenthesis = true;
}

inline void
SQLFilter::SetCloseParenthesis()
{
  m_closeParenthesis = true;
}

//////////////////////////////////////////////////////////////////////////
// And finally: a filter set is a vector of SQLFilters

// using SQLFilterSet = std::vector<SQLFilter>;
class SQLFilterSet
{
public:
  ~SQLFilterSet()
  {
    for(auto& filter : m_filters)
    {
      delete filter;
    }
  }

  void AddFilter(SQLFilter* p_filter)
  {
    SQLFilter* filter = new SQLFilter(p_filter);
    m_filters.push_back(filter);
  }

  void AddFilter(SQLFilter& p_filter)
  {
    SQLFilter* filter = new SQLFilter(p_filter);
    m_filters.push_back(filter);
  }

  bool Empty()
  {
    return m_filters.empty();
  }

  std::vector<SQLFilter*>& GetFilters()
  {
    return m_filters;
  }

  int Size()
  {
    return (int)m_filters.size();
  }

  CString ParseFiltersToCondition(SQLQuery& p_query);

private:
  std::vector<SQLFilter*> m_filters;
};

//////////////////////////////////////////////////////////////////////////
// Alias for other systems

typedef SQLFilter Filter;


}