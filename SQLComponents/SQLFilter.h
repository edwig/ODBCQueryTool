////////////////////////////////////////////////////////////////////////
//
// File: SQLFilter.h
//
// Copyright (c) 1998-2025 ir. W.E. Huisman
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
#include "SQLComponents.h"
#include "SQLVariant.h"
#include "SQLOperator.h"
#include "SQLFunction.h"
#include "SQLTimestampExtract.h"
#include <vector>

namespace SQLComponents
{

// Forward declarations
class SQLRecord;
class SQLQuery;
class SQLFilterSet;

// Mapping typedefs
typedef std::vector<SQLVariant*> VariantSet;

// Translate a string from the message to an operator
SQLOperator StringToSQLOperator(XString p_oper);
XString     SQLOperatorToString(SQLOperator p_oper);

// THE FILTER CLASS

class SQLFilter
{
public:
  // Construct a filter
  SQLFilter();
  SQLFilter(const SQLFilter& p_other);
  explicit SQLFilter(const SQLFilter* p_other);
  explicit SQLFilter(XString p_field,SQLOperator p_operator);
  explicit SQLFilter(XString p_field,SQLOperator p_operator,SQLVariant* p_value);
  explicit SQLFilter(XString p_field,SQLOperator p_operator,int         p_value);
  explicit SQLFilter(XString p_field,SQLOperator p_operator,XString     p_value);
  explicit SQLFilter(SQLOperator p_operator);
 ~SQLFilter();

  // BUILDING THE FILTER

  // Adding a comparison field (if not yet set)
  bool        SetField(XString p_field);
  // Adding an operator (if not yet set)
  bool        SetOperator(SQLOperator p_oper);
  // Adding extra values for the IN or BETWEEN operators
  void        AddValue(const SQLVariant* p_value);
  // Adding an expression as replacement for concrete values
  void        AddExpression(XString p_expression);
  // Negate the filter
  void        Negate();
  // Setting a parenthesis level
  void        SetOpenParenthesis();
  void        SetCloseParenthesis();
  // Setting an extra function
  void        SetFunction(SQLFunction p_function);
  // Setting an extract part or timestamp add/difference part
  void        SetExtractPart  (SQLExtractPart       p_part);
  void        SetTimestampPart(SQLTimestampCalcPart p_part);
  // Set extra optional field
  void        SetField2(XString p_field);
  // Set casting of the first field
  void        SetCastAs(XString p_datatype,int p_scale = 0,int p_precision = 0);

  // OPERATIONS

  // Getting the SQL Condition filter
  XString     GetSQLFilter(SQLQuery& p_query);
  // Match a record to the filter internally
  bool        MatchRecord(SQLRecord* p_record);

  // GETTERS

  // Getting the elements of the filter
  XString     GetField() const;
  SQLOperator GetOperator() const;
  SQLVariant* GetValue() const;
  SQLVariant* GetValue(int p_number) const;
  XString     GetExpression() const;
  SQLFunction GetFunction() const;
  bool        GetNegate() const;
  XString     GetField2() const;
  SQLExtractPart        GetExtractPart() const;
  SQLTimestampCalcPart  GetTimestampPart() const;

  // Filter assignment to another filter
  SQLFilter&  operator=(const SQLFilter& p_other);

  // Resetting the filter
  void        Reset();
  void        ResetField()                            { m_field.Empty();           }
  void        SetSubFilters(SQLFilterSet* subfilters) { m_subfilters = subfilters; }

private:
  // Check that we have at least one operand value
  void        CheckValue();
  void        CheckTwoValues();
  // Constructing the default operand
  void        ConstructOperand(XString& p_sql,SQLQuery& p_query);
  // Constructing the LIKE clause
  void        ConstructLike(XString& p_sql,SQLQuery& p_query);
  // Constructing the IN clause
  void        ConstructIN(XString& p_sql,SQLQuery& p_query);
  // Constructing the BETWEEN clause
  void        ConstructBetween(XString& p_sql,SQLQuery& p_query);
  // Constructing the EXISTS clause
  void        ConstructExists(XString& p_sql,SQLQuery& p_query);
  // Constructing a FUNCTION(field)
  XString     ConstructFunctionSQL(SQLQuery& p_query);
  // Constructing the extraction part in the EXTRACT function
  XString     ConstructExtractPart();
  // Constructing the calculation part in the TIMESTAMPADD/TIMESTAMPDIFF functions
  XString     ConstructTimestampCalcPart();
  // Constructing the CAST(field AS <datatype>)
  XString     ConstructCastAs();

  // Internal matching
  bool        MatchEqual       (const SQLVariant* p_field);
  bool        MatchNotEqual    (const SQLVariant* p_field);
  bool        MatchGreater     (const SQLVariant* p_field);
  bool        MatchGreaterEqual(const SQLVariant* p_field);
  bool        MatchSmaller     (const SQLVariant* p_field);
  bool        MatchSmallerEqual(const SQLVariant* p_field);
  bool        MatchLikeBegin   (const SQLVariant* p_field);
  bool        MatchLikeMiddle  (const SQLVariant* p_field);
  bool        MatchIsNULL      (const SQLVariant* p_field);
  bool        MatchIsNotNull   (const SQLVariant* p_field);
  bool        MatchIN          (const SQLVariant* p_field);
  bool        MatchBetween     (const SQLVariant* p_field);

  // Data for the filter
  XString     m_field;                         // Name of the field to act upon
  SQLOperator m_operator;                      // Operator of the condition
  VariantSet  m_values;                        // Multiple values for IN and BETWEEN
  XString     m_expression;                    // Free expression (no values!)
  SQLFunction m_function         { FN_NOP };   // Extra function for the field operator
  bool        m_negate           { false  };   // Negate the whole condition
  int         m_openParenthesis  { 0      };   // Start with an opening parenthesis
  int         m_closeParenthesis { 0      };   // End with a closing parenthesis
  union       {                                // Parts for functions EXTRACT/TIMESTAMP ADD/DIFF
                SQLExtractPart        m_extract  { TS_EXT_NONE  };
                SQLTimestampCalcPart  m_calcpart;
              }
              m_extract;
  XString     m_field2;                        // Optional extra field as in "m_field <oper> m_fileld2"
  // SubFilters for functions or a (NOT) EXSITS
  SQLFilterSet* m_subfilters  { nullptr };     // Extra filters to be appended
  // Cast for the first field
  XString     m_castType;                      // Cast to SQL_<TYPE> See: SQLInfo.GetTypeInfo
  int         m_castScale     { 0 };           // Cast first  level scaling
  int         m_castPrecision { 0 };           // Cast second level scaling
};

inline XString
SQLFilter::GetField() const
{
  return m_field;
}

inline SQLOperator
SQLFilter::GetOperator() const
{
  return m_operator;
}

inline SQLVariant*
SQLFilter::GetValue() const
{
  return m_values.front();
}

inline void
SQLFilter::AddValue(const SQLVariant* p_value)
{
  SQLVariant* value = new SQLVariant(p_value);
  m_values.push_back(value);
}

inline void
SQLFilter::AddExpression(XString p_expression)
{
  m_expression = p_expression;
}

inline void
SQLFilter::Negate()
{
  m_negate = !m_negate;
}

inline XString
SQLFilter::GetExpression() const
{
  return m_expression;
}

inline bool
SQLFilter::GetNegate() const
{
  return m_negate;
}

inline void
SQLFilter::SetOpenParenthesis()
{
  ++m_openParenthesis;
}

inline void
SQLFilter::SetCloseParenthesis()
{
  ++m_closeParenthesis;
}

inline void
SQLFilter::SetFunction(SQLFunction p_function)
{
  m_function = p_function;
}

inline SQLFunction
SQLFilter::GetFunction() const
{
  return m_function;
}

inline void
SQLFilter::SetExtractPart(SQLExtractPart p_part)
{
  m_extract.m_extract = p_part;
}

inline void
SQLFilter::SetTimestampPart(SQLTimestampCalcPart p_part)
{
  m_extract.m_calcpart = p_part;
}

inline SQLExtractPart
SQLFilter::GetExtractPart() const
{
  return m_extract.m_extract;
}

inline SQLTimestampCalcPart
SQLFilter::GetTimestampPart() const
{
  return m_extract.m_calcpart;
}

inline XString
SQLFilter::GetField2() const
{
  return m_field2;
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

  void AddFilter(const SQLFilter* p_filter)
  {
    SQLFilter* filter = new SQLFilter(p_filter);
    m_filters.push_back(filter);
  }

  void AddFilter(const SQLFilter& p_filter)
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

  SQLFilter* GetFilter(int p_index)
  {
    if(p_index >= 0 && p_index < (int)m_filters.size())
    {
      return m_filters[p_index];
    }
    return nullptr;
  }

  XString ParseFiltersToCondition(SQLQuery& p_query);

private:
  std::vector<SQLFilter*> m_filters;
};

//////////////////////////////////////////////////////////////////////////
// Alias for other systems

typedef SQLFilter Filter;

}