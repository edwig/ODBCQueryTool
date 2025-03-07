////////////////////////////////////////////////////////////////////////
//
// File: SQLFilter.cpp
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

OperatorName operatornames[] =
{
   { _T("Equal"),         OP_Equal        }
  ,{ _T("LikeBegin"),     OP_LikeBegin    }
  ,{ _T("LikeMiddle"),    OP_LikeMiddle   }
  ,{ _T("LikeEnd"),       OP_LikeEnd      }
  ,{ _T("IN"),            OP_IN           }
  ,{ _T("IsNULL"),        OP_IsNULL       }
  ,{ _T("IsNotNULL"),     OP_IsNotNULL    }
  ,{ _T("Greater"),       OP_Greater      }
  ,{ _T("GreaterEqual"),  OP_GreaterEqual }
  ,{ _T("Smaller"),       OP_Smaller      }
  ,{ _T("GreaterEqual"),  OP_GreaterEqual }
  ,{ _T("NotEqual"),      OP_NotEqual     }
  ,{ _T("Between"),       OP_Between      }
  ,{ _T("Exists"),        OP_Exists       }
  ,{ _T("NOP"),           OP_NOP          }
};

// XTOR: Create empty
SQLFilter::SQLFilter()
          :m_operator(OP_NOP)
{
}

// XTOR: Creates a new filter
//       Used for filters without an operand, or more than one
SQLFilter::SQLFilter(XString p_field,SQLOperator p_operator)
          :m_field(p_field)
          ,m_operator(p_operator)
{
}

// XTOR: Creates a new filter
//       Used for standard filters with one or more operands
SQLFilter::SQLFilter(XString p_field,SQLOperator p_operator,SQLVariant* p_value)
          :m_field(p_field)
          ,m_operator(p_operator)
{
  if(p_value)
  {
    m_values.push_back(new SQLVariant(p_value));
  }
}

// XTOR from a single integer
SQLFilter::SQLFilter(XString p_field,SQLOperator p_operator,int p_value)
          :m_field(p_field)
          ,m_operator(p_operator)
{
  SQLVariant* val = new SQLVariant(p_value);
  m_values.push_back(val);
}

// XTOR from a single XString
SQLFilter::SQLFilter(XString p_field,SQLOperator p_operator,XString p_value)
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
SQLFilter::SQLFilter(const SQLFilter* p_other)
{
  *this = *p_other;
}

// XTOR from another filter
SQLFilter::SQLFilter(const SQLFilter& p_other)
{
  *this = p_other;
  m_subfilters = p_other.m_subfilters;
  const_cast<SQLFilter&>(p_other).m_subfilters = nullptr;
}

// DTOR: Remove all variant values
SQLFilter::~SQLFilter()
{
  for(auto& val : m_values)
  {
    delete val;
  }
  m_values.clear();
  delete m_subfilters;
}

// Adding a comparison field (if not yet set)
bool
SQLFilter::SetField(XString p_field)
{
  if(m_field.IsEmpty() && m_subfilters == nullptr)
  {
    m_field = p_field;
    return true;
  }
  return false;
}

void
SQLFilter::SetField2(XString p_field2)
{
  int pos = p_field2.Find('@');
  if(pos > 0)
  {
    m_field2 = p_field2.Mid(pos + 1);
  }
  else
  {
    m_field2 = p_field2;
  }
}

// Adding an operator (if not yet set)
bool
SQLFilter::SetOperator(SQLOperator p_oper)
{
  if(m_operator == OP_NOP)
  {
    m_operator = p_oper;
    return true;
  }
  return false;
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
  m_function         = p_other.m_function;
  m_negate           = p_other.m_negate;
  m_openParenthesis  = p_other.m_openParenthesis;
  m_closeParenthesis = p_other.m_closeParenthesis;
  m_extract          = p_other.m_extract;
  m_field2           = p_other.m_field2;
  m_castType         = p_other.m_castType;
  m_castScale        = p_other.m_castScale;
  m_castPrecision    = p_other.m_castPrecision;

  for(auto& variant : p_other.m_values)
  {
    var* value = new SQLVariant(variant);
    m_values.push_back(value);
  }
  return *this;
}

// Resetting the filter
void
SQLFilter::Reset()
{
  m_field.Empty();
  m_field2.Empty();
  m_expression.Empty();
  m_negate            = false;
  m_openParenthesis   = false;
  m_closeParenthesis  = false;
  m_extract.m_extract = TS_EXT_NONE;
  m_operator          = OP_NOP;
  m_function          = FN_NOP;

  for(auto& variant : m_values)
  {
    delete variant;
  }
  m_values.clear();
}

// Getting one of the values of the filter
SQLVariant* 
SQLFilter::GetValue(int p_number) const
{
  if(p_number >= 0 && p_number < (int)m_values.size())
  {
    return m_values[p_number];
  }
  return nullptr;
}

// Getting the SQL Condition
XString
SQLFilter::GetSQLFilter(SQLQuery& p_query)
{
  XString sql;

  // Add an extra parenthesis level
  if(m_openParenthesis)
  {
    sql = _T("(");
  }

  // See if our own home-brewn expression on a filter without operators
  if(m_operator == OP_NOP && m_function == FN_NOP && !m_expression.IsEmpty())
  {
    sql += m_expression;
  }

  // See if extra function is pending
  if(m_function != FN_NOP)
  {
    sql += ConstructFunctionSQL(p_query);
  }
  // Add the field without a function
  else if(!m_field.IsEmpty() && m_operator != OP_Exists)
  {
    if(!m_castType.IsEmpty())
    {
      sql += ConstructCastAs();
    }
    else
    {
      // Simply add the field
      sql += m_field;
    }
  }

  // Test for special ISNULL case
  if(m_operator == OP_Equal && m_values.size() == 1)
  {
    SQLVariant* var = m_values.front();
    XString value;
    var->GetAsString(value);
    if(var->IsNULL() || value.CompareNoCase(_T("null")) == 0)
    {
      m_operator = OP_IsNULL;
    }
  }

  // Add the operator
  switch(m_operator)
  {
    case OP_Equal:        sql += _T(" = ");          break;
    case OP_NotEqual:     sql += _T(" <> ");         break;
    case OP_Greater:      sql += _T(" > ");          break;
    case OP_GreaterEqual: sql += _T(" >= ");         break;
    case OP_Smaller:      sql += _T(" < ");          break;
    case OP_SmallerEqual: sql += _T(" <= ");         break;
    case OP_LikeBegin:    sql += _T(" LIKE '");      break;
    case OP_LikeMiddle:   sql += _T(" LIKE '%");     break;
    case OP_LikeEnd:      sql += _T(" LIKE '%");     break;
    case OP_IsNULL:       sql += _T(" IS NULL");     break;
    case OP_IsNotNULL:    sql += _T(" IS NOT NULL"); break;
    case OP_IN:           sql += _T(" IN (");        break;
    case OP_Between:      sql += _T(" BETWEEN ");    break;
    case OP_Exists:       sql += _T(" EXISTS ");     break;
    //case OP_NOP:          // Fall through
    //default:              throw StdException("SQLFilter without an operator!");
  }

  // In case of a LIKE search of a character field
  if(m_operator == OP_LikeBegin  || 
     m_operator == OP_LikeMiddle ||
     m_operator == OP_LikeEnd    )
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
  else if(m_operator == OP_Exists)
  {
    ConstructExists(sql,p_query);
  }
  else if(m_operator != OP_IsNULL && m_operator != OP_IsNotNULL && m_operator != OP_NOP)
  {
    // For all other operators, getting the argument
    // Getting the value as an SQL expression string (with ODBC escapes)
    ConstructOperand(sql,p_query);
  }

  // See if we must NEGATE the condition
  // Beware that it is OUTSIDE the parenthesis
  if(m_negate)
  {
    sql = _T("NOT ") + sql;
  }

  // End an extra parenthesis level
  if(m_closeParenthesis)
  {
    sql += _T(")");
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
    throw StdException(_T("Cannot match a filter internally on an expression!"));
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
    default:              throw StdException(_T("SQLFilter with unknown operator!"));
  }
  // Return the correct result
  return m_negate ? !result : result;
}

// Record the CAST AS information
void
SQLFilter::SetCastAs(XString p_datatype,int p_scale /*= 0*/,int p_precision /*= 0*/)
{
  m_castType      = p_datatype;
  m_castScale     = p_scale;
  m_castPrecision = p_precision;
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
    throw StdException(_T("No operand value for SQLFilter provided!"));
  }
}

// CHeck that we have EXACTLY two values for a BETWEEN
void
SQLFilter::CheckTwoValues()
{
  if(m_values.size() != 2)
  {
    throw StdException(_T("SQLFilter with BETWEEN operator and number of arguments is not two (2) values"));
  }
}

// Constructing the default operand
void
SQLFilter::ConstructOperand(XString& p_sql,SQLQuery& p_query)
{
  if(m_expression.IsEmpty())
  {
    if(m_field2.IsEmpty())
    {
      // Check that we have a value, and use it
      if(m_values.empty())
      {
        p_sql += _T("IS NULL");
      }
      else
      {
        // Use last value (earlier can be used by functions!!)
        p_sql += _T("?");
        size_t num = m_values.size() - 1;
        p_query.SetParameter(m_values[num]);
      }
    }
    else
    {
      // "[function]m_field <oper> m_field2"
      p_sql += m_field2;
    }
  }
  else
  {
    // Free expression used
    p_sql += m_expression;
  }
}

// Constructing the LIKE clause
void
SQLFilter::ConstructLike(XString& p_sql)
{
  // Check that we have a value
  CheckValue();

  // Check that we do the operation on a character string
  if(m_values[0]->GetDataType() != SQL_C_CHAR &&
     m_values[0]->GetDataType() != SQL_C_WCHAR)
  {
    throw StdException(_T("Cannot use LIKE on a non-string data type!"));
  }
  // Get string without leading/trailing quote
  XString value;
  m_values[0]->GetAsString(value);
  // Add as a LIKE string
  p_sql += value;

  // Eventually a closing '%'
  p_sql += m_operator == OP_LikeEnd ? _T("'") : _T("%'");
}

// Constructing the IN clause
void
SQLFilter::ConstructIN(XString& p_sql,SQLQuery& p_query)
{
  // Add any subfilter values to the IN construct
  if(m_subfilters)
  {
    for(int index = 0;index < m_subfilters->Size(); ++index)
    {
      SQLFilter* filter = m_subfilters->GetFilter(index);
      for(auto& var : filter->m_values)
      {
        p_sql += _T("?,");
        p_query.SetParameter(var);
      }
    }
  }
  // Getting a series of values, comma separated
  for(auto& var : m_values)
  {
    p_sql += _T("?,");
    p_query.SetParameter(var);
  }
  p_sql.TrimRight(',');
  p_sql += _T(")");
}

// Constructing the BETWEEN clause
void
SQLFilter::ConstructBetween(XString& p_sql,SQLQuery& p_query)
{
  // CHeck that we have EXACTLY two values
  CheckTwoValues();

  p_sql += _T("? AND ? ");
  p_query.SetParameter(m_values[0]);
  p_query.SetParameter(m_values[1]);
}

// Constructing a FUNCTION(field)
XString
SQLFilter::ConstructFunctionSQL(SQLQuery& p_query)
{
  XString sql;
  XString comma;
  int parameters = 0;
  bool trim  = false;
  bool extra = false;

  switch(m_function)
  {
    // STRING FUNCTIONS
    case FN_ASCII:            sql = _T("ASCII");            parameters = 1; break;
    case FN_BIT_LENGTH:       sql = _T("BIT_LENGTH");       parameters = 1; break;
    case FN_CHAR:             sql = _T("CHAR");             parameters = 1; break;
    case FN_CHAR_LENGTH:      sql = _T("CHAR_LENGTH");      parameters = 1; break;
    case FN_CHARACTER_LENGTH: sql = _T("CHARACTER_LENGTH"); parameters = 1; break;
    case FN_CONCAT:           sql = _T("CONCAT");           parameters = 2; break;
    case FN_DIFFERENCE:       sql = _T("DIFFERENCE");       parameters = 2; break;
    case FN_INSERT:           sql = _T("INSERT");           parameters = 4; break;
    case FN_LCASE:            sql = _T("LCASE");            parameters = 1; break;
    case FN_LEFT:             sql = _T("LEFT");             parameters = 2; break;
    case FN_LENGTH:           sql = _T("LENGTH");           parameters = 1; break;
    case FN_LOCATE:           sql = _T("LOCATE");           parameters = 4; break;
    case FN_LTRIM:            sql = _T("LTRIM");            parameters = 1; break;
    case FN_OCTET_LENGTH:     sql = _T("OCTET_LENGTH");     parameters = 1; break;
    case FN_POSITION:         sql = _T("POSITION");         parameters = 2; comma = _T(" IN "); break;
    case FN_INSTRING:         sql = _T("INSTR");            parameters = 2; trim = true;    break;
    case FN_REPEAT:           sql = _T("REPEAT");           parameters = 2; break;
    case FN_REPLACE:          sql = _T("REPLACE");          parameters = 3; break;
    case FN_RIGHT:            sql = _T("RIGHT");            parameters = 2; break;
    case FN_RTRIM:            sql = _T("RTRIM");            parameters = 1; break;
    case FN_SOUNDEX:          sql = _T("SOUNDEX");          parameters = 1; break;
    case FN_SPACE:            sql = _T("SPACE");            parameters = 1; break;
    case FN_SUBSTRING:        sql = _T("SUBSTRING");        parameters = 3; break;
    case FN_UCASE:            sql = _T("UCASE");            parameters = 1; break;
    // NUMERIC FUNCTIONS
    case FN_ABS:              sql = _T("ABS");              parameters = 1; break;
    case FN_ACOS:             sql = _T("ACOS");             parameters = 1; break;
    case FN_ASIN:             sql = _T("ASIN");             parameters = 1; break;
    case FN_ATAN:             sql = _T("ATAN");             parameters = 1; break;
    case FN_ATAN2:            sql = _T("ATAN2");            parameters = 1; break;
    case FN_CEILING:          sql = _T("CEILING");          parameters = 1; break;
    case FN_COS:              sql = _T("COS");              parameters = 1; break;
    case FN_COT:              sql = _T("COT");              parameters = 1; break;
    case FN_DEGREES:          sql = _T("DEGREES");          parameters = 1; break;
    case FN_EXP:              sql = _T("EXP");              parameters = 1; break;
    case FN_FLOOR:            sql = _T("FLOOR");            parameters = 1; break;
    case FN_LOG:              sql = _T("LOG");              parameters = 1; break;
    case FN_LOG10:            sql = _T("LOG10");            parameters = 1; break;
    case FN_MOD:              sql = _T("MOD");              parameters = 2; break;
    case FN_PI:               sql = _T("PI");               parameters = 0; break;
    case FN_POWER:            sql = _T("POWER");            parameters = 2; break;
    case FN_RADIANS:          sql = _T("RADIANS");          parameters = 1; break;
    case FN_RAND:             sql = _T("RAND");             parameters = 1; break;
    case FN_ROUND:            sql = _T("ROUND");            parameters = 2; break;
    case FN_SIGN:             sql = _T("SIGN");             parameters = 1; break;
    case FN_SIN:              sql = _T("SIN");              parameters = 1; break;
    case FN_SQRT:             sql = _T("SQRT");             parameters = 1; break;
    case FN_TAN:              sql = _T("TAN");              parameters = 1; break;
    case FN_TRUNCATE:         sql = _T("TRUNCATE");         parameters = 2; break;
    // DATE/TIME FUNCTIONS
    case FN_CURRENT_DATE:     sql = _T("CURRENT_DATE");     parameters = 0; break;
    case FN_CURRENT_TIME:     sql = _T("CURRENT_TIME");     parameters = 0; break;
    case FN_CURRENT_TIMESTAMP:sql = _T("CURRENT_TIMESTAMP");parameters = 0; break;
    case FN_CURDATE:          sql = _T("CURDATE");          parameters = 0; break;
    case FN_CURTIME:          sql = _T("CURTIME");          parameters = 0; break;
    case FN_DAYNAME:          sql = _T("DAYNAME");          parameters = 1; break;
    case FN_DAYOFMONTH:       sql = _T("DAYOFMONTH");       parameters = 1; break;
    case FN_DAYOFWEEK:        sql = _T("DAYOFWEEK");        parameters = 1; break;
    case FN_DAYOFYEAR:        sql = _T("DAYOFYEAR");        parameters = 1; break;
    case FN_EXTRACT:          sql = _T("EXTRACT");          parameters =-1; break; // BEWARE: Negative parameters
    case FN_HOUR:             sql = _T("HOUR");             parameters = 1; break;
    case FN_MINUTE:           sql = _T("MINUTE");           parameters = 1; break;
    case FN_MONTH:            sql = _T("MONTH");            parameters = 1; break;
    case FN_MONTHNAME:        sql = _T("MONTHNAME");        parameters = 1; break;
    case FN_NOW:              sql = _T("NOW");              parameters = 0; break;
    case FN_QUARTER:          sql = _T("QUARTER");          parameters = 1; break;
    case FN_SECOND:           sql = _T("SECOND");           parameters = 1; break;
    case FN_TIMESTAMPADD:     sql = _T("TIMESTAMPADD");     parameters =-2; break; // BEWARE: Negative parameters
    case FN_TIMESTAMPDIFF:    sql = _T("TIMESTAMPDIFF");    parameters =-2; break; // BEWARE: Negative parameters
    case FN_WEEK:             sql = _T("WEEK");             parameters = 1; break;
    case FN_YEAR:             sql = _T("YEAR");             parameters = 1; break;
    // SYSTEM FUNCTIONS
    case FN_DATABASE:         sql = _T("DATABASE");         parameters = 0; break;
    case FN_USER:             sql = _T("USER");             parameters = 0; break;
    case FN_IFNULL:           sql = _T("IFNULL");           parameters = 2; break;
    // HAVING FUNCTIONS
    case FN_SUM:              sql = _T("SUM");              parameters = 1; trim = true; break;
    case FN_COUNT:            sql = _T("COUNT");            parameters = 1; trim = true; break;
    case FN_COUNTDIST:        sql = _T("COUNT(DISTINCT(");  parameters = 1; trim = true; extra = true; break;
    case FN_MIN:              sql = _T("MIN");              parameters = 1; trim = true; break;
    case FN_MAX:              sql = _T("MAX");              parameters = 1; trim = true; break;
    case FN_AVG:              sql = _T("AVG");              parameters = 1; trim = true; break;
    // EXTRA FUNCTIONS
    case FN_REGEXP_MATCH:     sql = _T("REGEXP_LIKE");      parameters = 3; trim = true; break;
  }

  // Construct ODBC Function
  if (m_subfilters != nullptr)
  {
    m_field = m_subfilters->ParseFiltersToCondition(p_query);
  }
  // Check for optional parameters
  if(parameters > ((int)m_values.size()) + 1)
  {
    parameters = ((int)m_values.size()) + 1;
  }
  switch(parameters)
  {
    case 0: m_expression = sql = _T("{fn ") + sql + _T("()}");
            sql = m_field;
            break;
    case 1: sql = _T("{fn ") + sql + _T("(") + m_field + _T(")}");
            break;
    case 2: sql = _T("{fn ") + sql + _T("(") + m_field + _T(",?)}");
            p_query.SetParameter(m_values[0]);
            break;
    case 3: sql = _T("{fn ") + sql + _T("(") + m_field + _T(",?,?)}");
            p_query.SetParameter(m_values[0]);
            p_query.SetParameter(m_values[1]);
            break;
    case 4: sql = _T("{fn ") + sql + _T("(") + m_field + _T(",?,?,?)}");
            p_query.SetParameter(m_values[0]);
            p_query.SetParameter(m_values[1]);
            p_query.SetParameter(m_values[2]);
            break;
            // SPECIAL CASES
    case -1:sql = _T("{fn ") + sql + _T("(") + ConstructExtractPart() + _T(" FROM ") + m_field + _T(")}");
            break;
    case -2:sql = _T("{fn ") + sql + _T("(") + ConstructTimestampCalcPart() + _T(",?,") + m_field + _T(")}");
            p_query.SetParameter(m_values[0]);
            break;
  }

  // Eventually remove the ODBC function escape!
  if(trim)
  {
    sql = sql.Mid(4);
    sql = sql.TrimRight('}');
  }

  // Eventually an extra parenthesis closing
  if(extra)
  {
    sql += _T(")");
  }

  // Eventually replace the ',' with the operator
  if(!comma.IsEmpty())
  {
    sql.Replace(_T(","),comma);
  }

  return sql;
}

// Constructing the extraction part in the EXTRACT function
XString
SQLFilter::ConstructExtractPart()
{
  XString part;
  switch(m_extract.m_extract)
  {
    case TS_EXT_YEAR:   part = _T("YEAR");    break;
    case TS_EXT_MONTH:  part = _T("MONTH");   break;
    case TS_EXT_DAY:    part = _T("DAY");     break;
    case TS_EXT_HOUR:   part = _T("HOUR");    break;
    case TS_EXT_MINUTE: part = _T("MINUTE");  break;
    case TS_EXT_SECOND: part = _T("SECOND");  break;
    case TS_EXT_NONE:   // Fall through
    default:            throw XString(_T("Unknown or unset timestamp part for EXTRACT function"));
  }
  return part;
}

// Constructing the calculation part in the TIMESTAMPADD/TIMESTAMPDIFF functions
XString
SQLFilter::ConstructTimestampCalcPart()
{
  XString part;
  switch(m_extract.m_calcpart)
  {
    case SQL_TSI_FRAC_SECOND: part = _T("SQL_TSI_FRAC_SECOND"); break;
    case SQL_TSI_SECOND:      part = _T("SQL_TSI_SECOND");      break;
    case SQL_TSI_MINUTE:      part = _T("SQL_TSI_MINUTE");      break;
    case SQL_TSI_HOUR:        part = _T("SQL_TSI_HOUR");        break;
    case SQL_TSI_DAY:         part = _T("SQL_TSI_DAY");         break;
    case SQL_TSI_WEEK:        part = _T("SQL_TSI_WEEK");        break;
    case SQL_TSI_MONTH:       part = _T("SQL_TSI_MONTH");       break;
    case SQL_TSI_QUARTER:     part = _T("SQL_TSI_QUARTER");     break;
    case SQL_TSI_YEAR:        part = _T("SQL_TSI_YEAR");        break;
    case SQL_TSI_NONE:        // Fall through
    default:                  throw XString(_T("Unknown or unset calculation part for TIMESTAMPADD/TIMESTAMPDIFF function"));
  }
  return part;
}

// Constructing the EXISTS clause
// m_field        -> contains the table we subquery from
// m_field2       -> contains a possibly "as alias"
// m_subfileters  -> contains the extra where clause
void
SQLFilter::ConstructExists(XString& p_sql,SQLQuery& p_query)
{
  if(!m_expression.IsEmpty())
  {
    p_sql += _T("(") + m_expression;
    if(m_subfilters)
    {
      p_sql += m_subfilters->ParseFiltersToCondition(p_query);
    }
    p_sql += _T(")");
    return;
  }

  p_sql += _T("\n  (SELECT 1\n")
           _T("     FROM ");
  p_sql += m_field;
  if(!m_field2.IsEmpty())
  {
    p_sql += _T(" ");
    p_sql += m_field2;
  }

  if(m_subfilters)
  {
    p_sql += _T("\n   WHERE ");
    p_sql += m_subfilters->ParseFiltersToCondition(p_query);
  }
  p_sql += _T(")");
}

// Construct a "CAST(field AS <datatype>)"
XString
SQLFilter::ConstructCastAs()
{
  XString sql(_T("CAST("));

  // Add field
  sql += m_field;
  sql += _T(" AS ");

  // Adding the datatype
  sql += m_castType;
  if(m_castScale > 0)
  {
    sql.AppendFormat(_T("(%d"),m_castScale);
    if(m_castPrecision > 0)
    {
      sql.AppendFormat(_T(",%d"),m_castPrecision);
    }
    sql += _T(")");
  }
  sql += _T(")");

  return sql;
}

//////////////////////////////////////////////////////////////////////////
//
// Internal matching of the SQLRecord by matching SQLVariants
//
//////////////////////////////////////////////////////////////////////////

bool
SQLFilter::MatchEqual(const SQLVariant* p_field)
{
  CheckValue();
  return *p_field == *m_values.front();
}

bool
SQLFilter::MatchNotEqual(const SQLVariant* p_field)
{
  CheckValue();
  return *p_field != *m_values.front();
}

bool
SQLFilter::MatchGreater(const SQLVariant* p_field)
{
  CheckValue();
  return *p_field > *m_values.front();
}

bool
SQLFilter::MatchGreaterEqual(const SQLVariant* p_field)
{
  CheckValue();
  return *p_field >= *m_values.front();
}

bool
SQLFilter::MatchSmaller(const SQLVariant* p_field)
{
  CheckValue();
  return *p_field < *m_values.front();
}

bool
SQLFilter::MatchSmallerEqual(const SQLVariant* p_field)
{
  CheckValue();
  return *p_field <= *m_values.front();
}

bool
SQLFilter::MatchLikeBegin(const SQLVariant* p_field)
{
  CheckValue();
  XString match;
  m_values.front()->GetAsString(match);
  int length = match.GetLength();

  XString field;
  p_field->GetAsString(field);
  return field.Left(length).Compare(match) == 0;
}

bool
SQLFilter::MatchLikeMiddle(const SQLVariant* p_field)
{
  CheckValue();
  XString match;
  m_values.front()->GetAsString(match);

  XString field;
  p_field->GetAsString(field);
  return field.Find(match) >= 0;
}

bool
SQLFilter::MatchIsNULL(const SQLVariant* p_field)
{
  // Relies on the ODBC NULL indicator field!
  return p_field->IsNULL();
}

bool
SQLFilter::MatchIsNotNull(const SQLVariant* p_field)
{
  return !p_field->IsNULL();
}

bool
SQLFilter::MatchIN(const SQLVariant* p_field)
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
SQLFilter::MatchBetween(const SQLVariant* p_field)
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
StringToSQLOperator(XString p_oper)
{
  OperatorName* filter = operatornames;

  while(filter->m_operator != OP_NOP)
  {
    if(p_oper.Compare(filter->m_name) == 0)
    {
      return static_cast<SQLOperator>(filter->m_operator);
    }
    ++filter;
  }
  return OP_NOP;
}

// Translate an operator to a string
XString
SQLOperatorToString(SQLOperator p_oper)
{
  OperatorName* filter = operatornames;

  while(filter->m_operator != OP_NOP)
  {
    if(filter->m_operator == p_oper)
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

XString 
SQLFilterSet::ParseFiltersToCondition(SQLQuery& p_query)
{
  XString query;
  bool first = true;
  bool orDone = false;

  // Add all filters
  for(auto& filt : m_filters)
  {
    if(first == true)
    {
      first = false;
    }
    else
    {
      if(filt->GetOperator() == SQLOperator::OP_OR)
      {
        query += _T("\n    OR ");
        orDone = true;
        continue;
      }
      if(!orDone)
      {
        query += _T("\n   AND ");
      }
      orDone = false;
    }
    query += filt->GetSQLFilter(p_query);
  }
  return query;
}

}
