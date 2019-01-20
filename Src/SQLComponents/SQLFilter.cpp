////////////////////////////////////////////////////////////////////////
//
// File: SQLFilter.cpp
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
#include "stdafx.h"
#include "SQLFilter.h"
#include "SQLRecord.h"
#include "SQLQuery.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{

// Word translation of filters, needed for XMLMessages
typedef struct _filterName
{
  const char* m_name;
  int         m_filter;
}
FilterName;

FilterName sqfilter[] =
{
   { "Equal",         OP_Equal        }
  ,{ "LikeBegin",     OP_LikeBegin    }
  ,{ "LikeMiddle",    OP_LikeMiddle   }
  ,{ "IN",            OP_IN           }
  ,{ "IsNULL",        OP_IsNULL       }
  ,{ "IsNotNULL",     OP_IsNotNULL    }
  ,{ "Greater",       OP_Greater      }
  ,{ "GreaterEqual",  OP_GreaterEqual }
  ,{ "Smaller",       OP_Smaller      }
  ,{ "GreaterEqual",  OP_GreaterEqual }
  ,{ "NotEqual",      OP_NotEqual     }
  ,{ "Between",       OP_Between      }
  ,{ "Exists",        OP_Exists       }
  ,{ "NOP",           OP_NOP          }
};


// XTOR: Creates a new filter
//       Used for filters without an operand, or more than one
SQLFilter::SQLFilter(CString p_field,SQLOperator p_operator)
          :m_field(p_field)
          ,m_operator(p_operator)
{
}

// XTOR: Creates a new filter
//       Used for standard filters with one or more operands
SQLFilter::SQLFilter(CString p_field,SQLOperator p_operator,SQLVariant* p_value)
          :m_field(p_field)
          ,m_operator(p_operator)
{
  if(p_value)
  {
    m_values.push_back(new SQLVariant(p_value));
  }
}

// XTOR from a single integer
SQLFilter::SQLFilter(CString p_field,SQLOperator p_operator,int p_value)
          :m_field(p_field)
          ,m_operator(p_operator)
{
  SQLVariant* val = new SQLVariant(p_value);
  m_values.push_back(val);
}

// XTOR from a single CString
SQLFilter::SQLFilter(CString p_field,SQLOperator p_operator,CString p_value)
          :m_field(p_field)
          ,m_operator(p_operator)
{
  SQLVariant* val = new SQLVariant(p_value);
  m_values.push_back(val);
}

// XTOR from a single operator
SQLFilter::SQLFilter(SQLOperator p_operator)
          :m_operator(p_operator)
{
}

// XTOR from another filter
SQLFilter::SQLFilter(SQLFilter* p_other)
{
  *this = *p_other;
}

// XTOR from another filter
SQLFilter::SQLFilter(SQLFilter& p_other)
{
  *this = p_other;
}

// DTOR: Remove all variant values
SQLFilter::~SQLFilter()
{
  for(auto& val : m_values)
  {
    delete val;
  }
  m_values.clear();
}

// Copy a SQLFilter
SQLFilter&
SQLFilter::operator=(const SQLFilter& p_other)
{
  // Check if we are not copying ourselves
  if(this == &p_other)
  {
    return *this;
  }
  // Copy all values
  m_field            = p_other.m_field;
  m_operator         = p_other.m_operator;
  m_expression       = p_other.m_expression;
  m_negate           = p_other.m_negate;
  m_openParenthesis  = p_other.m_openParenthesis;
  m_closeParenthesis = p_other.m_closeParenthesis;

  for(auto& variant : p_other.m_values)
  {
    var* value = new SQLVariant(variant);
    m_values.push_back(value);
  }
  return *this;
}


// Getting one of the values of the filter
SQLVariant* 
SQLFilter::GetValue(int p_number)
{
  if(p_number >= 0 && p_number < (int)m_values.size())
  {
    return m_values[p_number];
  }
  return nullptr;
}

// Getting the SQL Condition
CString
SQLFilter::GetSQLFilter(SQLQuery& p_query)
{
  CString sql;

  // Add an extra parenthesis level
  if(m_openParenthesis)
  {
    sql = "(";
  }

  // Add the field
  if(!m_field.IsEmpty())
  {
    sql += m_field;
  }

  // Add the operator
  switch(m_operator)
  {
    case OP_Equal:        sql += " = ";         break;
    case OP_NotEqual:     sql += " <> ";        break;
    case OP_Greater:      sql += " > ";         break;
    case OP_GreaterEqual: sql += " >= ";        break;
    case OP_Smaller:      sql += " < ";         break;
    case OP_SmallerEqual: sql += " <= ";        break;
    case OP_LikeBegin:    sql += " LIKE '";     break;
    case OP_LikeMiddle:   sql += " LIKE '%";    break;
    case OP_IsNULL:       sql += " IS NULL";    break;
    case OP_IsNotNULL:    sql += " IS NOT NULL";break;
    case OP_IN:           sql += " IN (";       break;
    case OP_Between:      sql += " BETWEEN ";   break;
    case OP_Exists:       sql += " EXISTS ";    break;
    case OP_NOP:          // Fall through
    default:              throw StdException("SQLFilter without an operator!");
  }

  // In case of a LIKE search of a character field
  if(m_operator == OP_LikeBegin || m_operator == OP_LikeMiddle)
  {
    ConstructLike(sql);
  }
  else if(m_operator == OP_IN)
  {
    ConstructIN(sql,p_query);
  }
  else if(m_operator == OP_Between)
  {
    ConstructBetween(sql,p_query);
  }
  else if(m_operator != OP_IsNULL && m_operator != OP_IsNotNULL)
  {
    // For all other operators, getting the argument
    // Getting the value as an SQL expression string (with ODBC escapes)
    ConstructOperand(sql,p_query);
  }

  // See if we must NEGATE the condition
  // Beware that it is OUTSIDE the parenthesis
  if(m_negate)
  {
    sql = "NOT " + sql;
  }

  // End an extra parenthesis level
  if(m_closeParenthesis)
  {
    sql = ")";
  }

  return sql;
}

// Match a record to the filter internally
bool
SQLFilter::MatchRecord(SQLRecord* p_record)
{
  bool result = false;

  // Check that we do NOT have a free expression
  if(!m_expression.IsEmpty())
  {
    throw StdException("Cannot match a filter internally on an expression!");
  }

  // Getting the left hand operand
  SQLVariant* field = p_record->GetField(m_field);
  if(!field)
  {
    return false;
  }

  // Using the operator
  switch(m_operator)
  {
    case OP_Equal:        result = MatchEqual       (field); break;
    case OP_NotEqual:     result = MatchNotEqual    (field); break;
    case OP_Greater:      result = MatchGreater     (field); break;
    case OP_GreaterEqual: result = MatchGreaterEqual(field); break;
    case OP_Smaller:      result = MatchSmaller     (field); break;
    case OP_SmallerEqual: result = MatchSmallerEqual(field); break;
    case OP_LikeBegin:    result = MatchLikeBegin   (field); break;
    case OP_LikeMiddle:   result = MatchLikeMiddle  (field); break;
    case OP_IsNULL:       result = MatchIsNULL      (field); break;
    case OP_IsNotNULL:    result = MatchIsNotNull   (field); break;
    case OP_IN:           result = MatchIN          (field); break;
    case OP_Between:      result = MatchBetween     (field); break;
    default:              throw StdException("SQLFilter with unknown operator!");
  }
  // Return the correct result
  return m_negate ? !result : result;
}

//////////////////////////////////////////////////////////////////////////
// 
// PRIVATE
//
//////////////////////////////////////////////////////////////////////////

// Check that we have at least one operand value
void
SQLFilter::CheckValue()
{
  if(m_values.empty())
  {
    throw StdException("No operand value for SQLFilter provided!");
  }
}

// CHeck that we have EXACTLY two values for a BETWEEN
void
SQLFilter::CheckTwoValues()
{
  if(m_values.size() != 2)
  {
    throw StdException("SQLFilter with BETWEEN operator and number of arguments is not two (2) values");
  }
}

// Constructing the default operand
void
SQLFilter::ConstructOperand(CString& p_sql,SQLQuery& p_query)
{
  if(m_expression.IsEmpty())
  {
    // Check that we have a value, and use it
    CheckValue();
    p_sql += "?";
    p_query.SetParameter(m_values[0]);
  }
  else
  {
    // Free expression used
    p_sql += m_expression;
  }
}

// Constructing the LIKE clause
void
SQLFilter::ConstructLike(CString& p_sql)
{
  // Check that we have a value
  CheckValue();

  // Check that we do the operation on a character string
  if(m_values[0]->GetDataType() != SQL_C_CHAR)
  {
    throw StdException("Cannot use LIKE on a non-string data type!");
  }
  // Get string without leading/trailing quote
  CString value;
  m_values[0]->GetAsString(value);
  // Add as a LIKE string
  p_sql += value;
  p_sql += "%'";
}

// Constructing the IN clause
void
SQLFilter::ConstructIN(CString& p_sql,SQLQuery& p_query)
{
  // Getting a series of values, comma separated
  for(auto& var : m_values)
  {
    p_sql += "?,";
    p_query.SetParameter(var);
  }
  p_sql.TrimRight(',');
  p_sql += ")";
}

// Constructing the BETWEEN clause
void
SQLFilter::ConstructBetween(CString& p_sql,SQLQuery& p_query)
{
  // CHeck that we have EXACTLY two values
  CheckTwoValues();

  p_sql += "? AND ? ";
  p_query.SetParameter(m_values[0]);
  p_query.SetParameter(m_values[1]);
}

//////////////////////////////////////////////////////////////////////////
//
// Internal matching of the SQLRecord by matching SQLVariants
//
//////////////////////////////////////////////////////////////////////////

bool
SQLFilter::MatchEqual(SQLVariant* p_field)
{
  CheckValue();
  return *p_field == *m_values.front();
}

bool
SQLFilter::MatchNotEqual(SQLVariant* p_field)
{
  CheckValue();
  return *p_field != *m_values.front();
}

bool
SQLFilter::MatchGreater(SQLVariant* p_field)
{
  CheckValue();
  return *p_field > *m_values.front();
}

bool
SQLFilter::MatchGreaterEqual(SQLVariant* p_field)
{
  CheckValue();
  return *p_field >= *m_values.front();
}

bool
SQLFilter::MatchSmaller(SQLVariant* p_field)
{
  CheckValue();
  return *p_field < *m_values.front();
}

bool
SQLFilter::MatchSmallerEqual(SQLVariant* p_field)
{
  CheckValue();
  return *p_field <= *m_values.front();
}

bool
SQLFilter::MatchLikeBegin(SQLVariant* p_field)
{
  CheckValue();
  CString match;
  m_values.front()->GetAsString(match);
  int length = match.GetLength();

  CString field;
  p_field->GetAsString(field);
  return field.Left(length).Compare(match) == 0;
}

bool
SQLFilter::MatchLikeMiddle(SQLVariant* p_field)
{
  CheckValue();
  CString match;
  m_values.front()->GetAsString(match);

  CString field;
  p_field->GetAsString(field);
  return field.Find(match) >= 0;
}

bool
SQLFilter::MatchIsNULL(SQLVariant* p_field)
{
  // Relies on the ODBC NULL indicator field!
  return p_field->IsNULL();
}

bool
SQLFilter::MatchIsNotNull(SQLVariant* p_field)
{
  return !p_field->IsNULL();
}

bool
SQLFilter::MatchIN(SQLVariant* p_field)
{
  // Simply walk the vector of values for a match
  for(auto& var : m_values)
  {
    // Relies on the operator== of SQLVariant
    if(*var == *p_field)
    {
      return true;
    }
  }
  return false;
}

bool
SQLFilter::MatchBetween(SQLVariant* p_field)
{
  // Check that we have EXACTLY two values
  CheckTwoValues();

  // Relies on the operator<= of SQLVariant
  if(*(m_values[0]) <= *p_field && *p_field <= *(m_values[1]))
  {
    return true;
  }
  return false;
}

//////////////////////////////////////////////////////////////////////////
//
// Static translation functions (thread safe)
//
//////////////////////////////////////////////////////////////////////////

// Translate a string from the message to an operator
SQLOperator 
StringToSQLOperator(CString p_oper)
{
  FilterName* filter = sqfilter;

  while(filter->m_filter != OP_NOP)
  {
    if(p_oper.Compare(filter->m_name) == 0)
    {
      return static_cast<SQLOperator>(filter->m_filter);
    }
    ++filter;
  }
  return OP_NOP;
}

// Translate an operator to a string
CString
SQLOperatorToString(SQLOperator p_oper)
{
  FilterName* filter = sqfilter;

  while(filter->m_filter != OP_NOP)
  {
    if(filter->m_filter == p_oper)
    {
      return filter->m_name;
    }
    ++filter;
  }
  return "";
}

//////////////////////////////////////////////////////////////////////////
//
// HERE IT COMES ALL TOGETHER
// We parse a FilterSet to a condition filter for a WHERE part
//
//////////////////////////////////////////////////////////////////////////

CString 
SQLFilterSet::ParseFiltersToCondition(SQLQuery& p_query)
{
  CString query;
  bool first = true;

  // Add all filters
  for(auto& filt : m_filters)
  {
    if(first == true)
    {
      first = false;
    }
    else if(filt->GetOperator() == SQLOperator::OP_OR)
    {
      query += "\n    OR ";
      continue;
    }
    query += "\n   AND ";
    query += filt->GetSQLFilter(p_query);
  }
  return query;
}



}

