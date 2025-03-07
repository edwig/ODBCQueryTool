+////////////////////////////////////////////////////////////////////////
//
// File: SQLDataType.cpp
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
#include "SQLDataType.h"
#include "SQLParameterType.h"
#include "sql.h"
#include "sqlext.h"

namespace SQLComponents
{

// Translation list of SQL datatype constants and names
typedef struct _types
{
  LPCTSTR name;
  int     type;
}
DataTypes;

static DataTypes allTypes[] = 
{
   { _T("<NO TYPE>"),                  0                               }
  ,{ _T("CHAR"),                       SQL_C_CHAR                      } 
  ,{ _T("NCHAR"),                      SQL_C_WCHAR                     } 
  ,{ _T("SHORT"),                      SQL_C_SHORT                     }
  ,{ _T("SIGNED SHORT"),               SQL_C_SSHORT                    }
  ,{ _T("UNSIGNED SHORT"),             SQL_C_USHORT                    }
  ,{ _T("LONG"),                       SQL_C_LONG                      }
  ,{ _T("SIGNED LONG"),                SQL_C_SLONG                     }
  ,{ _T("UNSIGNED LONG"),              SQL_C_ULONG                     }
  ,{ _T("REAL"),                       SQL_C_FLOAT                     }
  ,{ _T("FLOAT"),                      SQL_C_FLOAT                     }
  ,{ _T("DOUBLE"),                     SQL_C_DOUBLE                    }
  ,{ _T("BIT"),                        SQL_C_BIT                       }
  ,{ _T("TINYINT"),                    SQL_C_TINYINT                   }
  ,{ _T("SIGNED TINYINT"),             SQL_C_STINYINT                  }
  ,{ _T("UNSIGNED TINYINT"),           SQL_C_UTINYINT                  }
  ,{ _T("SIGNED BIGINT"),              SQL_C_SBIGINT                   }
  ,{ _T("UNSIGNED BIGINT"),            SQL_C_UBIGINT                   }
  ,{ _T("DECIMAL"),                    SQL_C_NUMERIC                   }
  ,{ _T("NUMERIC"),                    SQL_C_NUMERIC                   }
  ,{ _T("GUID"),                       SQL_C_GUID                      }
  ,{ _T("BINARY"),                     SQL_C_BINARY                    }
  ,{ _T("DATE"),                       SQL_C_DATE                      }
  ,{ _T("TIME"),                       SQL_C_TIME                      }
  ,{ _T("TIMESTAMP"),                  SQL_C_TIMESTAMP                 }
  ,{ _T("TYPE DATE"),                  SQL_C_TYPE_DATE                 }
  ,{ _T("TYPE TIME"),                  SQL_C_TYPE_TIME                 }
  ,{ _T("TYPE TIMESTAMP"),             SQL_C_TYPE_TIMESTAMP            }
  ,{ _T("INTERVAL YEAR"),              SQL_C_INTERVAL_YEAR             }
  ,{ _T("INTERVAL MONTH"),             SQL_C_INTERVAL_MONTH            }
  ,{ _T("INTERVAL DAY"),               SQL_C_INTERVAL_DAY              }
  ,{ _T("INTERVAL HOUR"),              SQL_C_INTERVAL_HOUR             }
  ,{ _T("INTERVAL MINUTE"),            SQL_C_INTERVAL_MINUTE           }
  ,{ _T("INTERVAL SECOND"),            SQL_C_INTERVAL_SECOND           }
  ,{ _T("INTERVAL YEAR TO MONTH"),     SQL_C_INTERVAL_YEAR_TO_MONTH    }
  ,{ _T("INTERVAL DAY TO HOUR"),       SQL_C_INTERVAL_DAY_TO_HOUR      }
  ,{ _T("INTERVAL DAY TO MINUTE"),     SQL_C_INTERVAL_DAY_TO_MINUTE    }
  ,{ _T("INTERVAL DAY TO SECOND"),     SQL_C_INTERVAL_DAY_TO_SECOND    }
  ,{ _T("INTERVAL HOUR TO MINUTE"),    SQL_C_INTERVAL_HOUR_TO_MINUTE   }
  ,{ _T("INTERVAL HOUR TO SECOND"),    SQL_C_INTERVAL_HOUR_TO_SECOND   }
  ,{ _T("INTERVAL MINUTE TO SECOND"),  SQL_C_INTERVAL_MINUTE_TO_SECOND }
  ,{ NULL,                             0                               }
};

// All datatypes not having a counterpart in SQL_C_XXX set
// SQL Datatypes should be used for binding in SQLBindParameter
static DataTypes allOther[] = 
{
  { _T("VARBINARY"),       SQL_VARBINARY      }  // LONG RAW
 ,{ _T("LONGVARBINARY"),   SQL_LONGVARBINARY  }  // BLOB
 ,{ _T("LONGVARCHAR"),     SQL_LONGVARCHAR    }  // CLOB
 ,{ _T("VARCHAR"),         SQL_VARCHAR        }  // VARCHAR2
 ,{ _T("WVARCHAR"),        SQL_WVARCHAR       }  // NVARCHAR
 ,{ _T("WLONGVARCHAR"),    SQL_WLONGVARCHAR   }  // NCLOB
 ,{ NULL,                  0                  } 
};

// Names must appear in this order to work properly!!
static DataTypes allParams[] = 
{
  { _T("<UNKNOWN>"), P_SQL_PARAM_TYPE_UNKNOWN }    // 0
 ,{ _T("INPUT"),     P_SQL_PARAM_INPUT        }    // 1
 ,{ _T("INOUT"),     P_SQL_PARAM_INPUT_OUTPUT }    // 2
 ,{ _T("COLUMN"),    P_SQL_RESULT_COL         }    // 3
 ,{ _T("OUTPUT"),    P_SQL_PARAM_OUTPUT       }    // 4
 ,{ _T("RETURN"),    P_SQL_RETURN_VALUE       }    // 5
 ,{ NULL,            0                        }
};

SQLDataType::SQLDataType()
{
}

SQLDataType::SQLDataType(int p_sqlType)
            :m_sqlType(p_sqlType)
            ,m_typeName(FindDatatype(p_sqlType))
{
}

SQLDataType::SQLDataType(XString p_typeName)
            :m_typeName(p_typeName)
{
  m_sqlType  = FindDatatype(reinterpret_cast<LPTSTR>(const_cast<TCHAR*>(p_typeName.GetString())));
}

SQLDataType::SQLDataType(MetaColumn& p_column)
            :m_sqlType      (p_column.m_datatype)
            ,m_typeName     (p_column.m_typename)
            ,m_columnSize   (p_column.m_columnSize)
            ,m_bufferSize   (p_column.m_bufferLength)
            ,m_decimalDigits(p_column.m_decimalDigits)
            ,m_radix        (p_column.m_numRadix)
            ,m_octetLength  (p_column.m_octet_length)
            ,m_subType      (p_column.m_sub_datatype)
{
  if(!m_sqlType)
  {
    m_sqlType = FindDatatype(reinterpret_cast<LPTSTR>(const_cast<TCHAR*>(m_typeName.GetString())));
  }
}

SQLDataType::~SQLDataType()
{
}

// CHAR, WCHAR, VARCHAR, WVARCHAR
bool
SQLDataType::GetIsString()
{
  if(m_sqlType == SQL_C_CHAR  || m_sqlType == SQL_VARCHAR  || m_sqlType == SQL_LONGVARCHAR ||
     m_sqlType == SQL_C_WCHAR || m_sqlType == SQL_WVARCHAR || m_sqlType == SQL_WLONGVARCHAR)
  {
    return true;
  }
  return false;
}

// 1, 8, 16, 32 or 64 bits Integer
bool
SQLDataType::GetIsInteger()
{
  if(m_sqlType == SQL_C_LONG    || m_sqlType == SQL_C_ULONG    || m_sqlType == SQL_C_SLONG    ||
     m_sqlType == SQL_C_SHORT   || m_sqlType == SQL_C_USHORT   || m_sqlType == SQL_C_SSHORT   ||
     m_sqlType == SQL_C_TINYINT || m_sqlType == SQL_C_UTINYINT || m_sqlType == SQL_C_STINYINT ||
     m_sqlType == SQL_C_SBIGINT || m_sqlType == SQL_C_UBIGINT  || m_sqlType == SQL_C_BIT)
  { 
    return true;
  }
  return false;
}

// DECIMAL or NUMERIC
bool
SQLDataType::GetIsDecimal()
{
  if(m_sqlType == SQL_C_NUMERIC)
  {
    return true;
  }
  return false;
}

// Number like (integer, approx, decimals)
bool
SQLDataType::GetIsNumeric()
{
  if(GetIsDecimal() || GetIsInteger())
  {
    return true;
  }
  return false;
}

// FLOAT or DOUBLE
bool
SQLDataType::GetIsApproximate()
{
  if(m_sqlType == SQL_C_FLOAT || m_sqlType == SQL_C_DOUBLE)
  {
    return true;
  }
  return false;
}

// DATE / TYPE_DATE
bool
SQLDataType::GetIsDate()
{
  if(m_sqlType == SQL_C_DATE || m_sqlType == SQL_C_TYPE_DATE)
  {
    return true;
  }
  return false;
}

// TIME / TYPE_TIME
bool
SQLDataType::GetIsTime()
{
  if(m_sqlType == SQL_C_TIME || m_sqlType == SQL_C_TYPE_TIME)
  {
    return true;
  }
  return false;
}

// TIMESTAMP / TYPE_TIMESTAMP
bool
SQLDataType::GetIsTimestamp()
{
  if(m_sqlType == SQL_C_TIMESTAMP || m_sqlType == SQL_C_TYPE_TIMESTAMP)
  {
    return true;
  }
  return false;
}

// YEAR upto DAY_TO_SECOND
bool
SQLDataType::GetisInterval()
{
  if(m_sqlType <= SQL_C_INTERVAL_YEAR && m_sqlType >= SQL_C_INTERVAL_MINUTE_TO_SECOND)
  {
    return true;
  }
  return false;
}

/*static*/ int
SQLDataType::FindDatatype(LPTSTR p_type)
{
  DataTypes* types = allTypes;
  while(types->name)
  {
    if(_tcsicmp(p_type,types->name) == 0)
    {
      return types->type;
    }
    ++types;
  }
  return 0;
}

/*static*/ LPCTSTR
SQLDataType::FindDatatype(int p_type)
{
  DataTypes* types = allTypes;
  while(types->name)
  {
    if(types->type == p_type)
    {
      return types->name;
    }
    ++types;
  }
  return _T("");
}

/*static*/ int
SQLDataType::FindParamtype(LPTSTR p_type)
{
  DataTypes* param = allParams;
  while(param->name)
  {
    if(_tcsicmp(p_type,param->name) == 0)
    {
      return param->type;
    }
    ++param;
  }
  return 0;
}

/*static*/ LPCTSTR
SQLDataType::FindParamtype(int p_type)
{
  DataTypes* param = allParams;
  while(param->name)
  {
    if(param->type == p_type)
    {
      return param->name;
    }
    ++param;
  }
  return _T("");
}

// Find the extra SQL_XXX datatype for
// variants of CHAR and BINARY
/*static*/ int
SQLDataType::FindSQLDatatype(LPTSTR p_type)
{
  DataTypes* types = allOther;
  while(types->name)
  {
    if(_tcsicmp(types->name,p_type) == 0)
    {
      return types->type;
    }
    ++types;
  }
  return 0;
}

/*static*/ LPCTSTR
SQLDataType::FindSQLDatatype(int p_type)
{
  DataTypes* types = allOther;
  while(types->name)
  {
    if(types->type == p_type)
    {
      return types->name;
    }
    ++types;
  }
  return _T("");
}

// Microsoft Entity Framework datatypes (CSDL like)
XString
SQLDataType::GetAsEdmType()
{
  switch(m_sqlType)
  {
    case SQL_C_CHAR:
    case SQL_LONGVARCHAR:
    case SQL_VARCHAR:
    case SQL_WCHAR:
    case SQL_WVARCHAR:
    case SQL_WLONGVARCHAR:      return _T("Edm.String");
    case SQL_C_SHORT:
    case SQL_C_SSHORT:
    case SQL_C_USHORT:          return _T("Edm.Int16");
    case SQL_C_UTINYINT:        return _T("Edm.Byte");
    case SQL_C_TINYINT:
    case SQL_C_STINYINT:        return _T("Edm.SByte");
    case SQL_C_LONG:
    case SQL_C_ULONG:
    case SQL_C_SLONG:           return _T("Edm.Int32");
    case SQL_C_UBIGINT:
    case SQL_C_SBIGINT:         return _T("Edm.Int64");
    case SQL_C_FLOAT:           return _T("Edm.Single");
    case SQL_C_DOUBLE:          return _T("Edm.Double");
    case SQL_C_BIT:             return _T("Edm.Boolean");
    case SQL_DECIMAL:
    case SQL_C_NUMERIC:         return _T("Edm.Decimal");
    case SQL_VARBINARY:
    case SQL_LONGVARBINARY:
    case SQL_C_BINARY:          return _T("Edm.Binary");
    case SQL_C_DATE:
    case SQL_C_TYPE_DATE:       return _T("Edm.Date");
    case SQL_C_TIME:
    case SQL_C_TYPE_TIME:       return _T("Edm.TimeOfDay");
    case SQL_C_TIMESTAMP:
    case SQL_C_TYPE_TIMESTAMP:  return _T("Edm.DateTimeOffset");
    case SQL_C_GUID:            return _T("Edm.Guid");
    case SQL_C_INTERVAL_YEAR:
    case SQL_C_INTERVAL_MONTH:
    case SQL_C_INTERVAL_YEAR_TO_MONTH:
    case SQL_C_INTERVAL_DAY:
    case SQL_C_INTERVAL_HOUR:
    case SQL_C_INTERVAL_MINUTE:
    case SQL_C_INTERVAL_SECOND:
    case SQL_C_INTERVAL_DAY_TO_HOUR:
    case SQL_C_INTERVAL_DAY_TO_MINUTE:
    case SQL_C_INTERVAL_DAY_TO_SECOND:
    case SQL_C_INTERVAL_HOUR_TO_MINUTE:
    case SQL_C_INTERVAL_HOUR_TO_SECOND:
    case SQL_C_INTERVAL_MINUTE_TO_SECOND:  return _T("Edm.Duration");
  }
  return _T("Edm.Unknown");
}

// Datatype formatted according to a specific RDBMS dialect
XString
SQLDataType::TypeNameWithPrecisionScale(TypeInfo* p_type)
{
  bool max = false;
  XString params = p_type->m_create_params;

  // Set in lowercase for replacing
  params.MakeLower();

  // Format as strings
  XString length,precision,scale;
  if(m_columnSize > 0)
  {
    length.Format(_T("%d"),m_columnSize);
  }
  if(m_columnSize > 0 || m_decimalDigits > 0)
  {
    precision.Format(_T("%d"),m_columnSize);
    scale.Format(_T("%d"),m_decimalDigits);
  }

  // Replace as strings
  if(m_columnSize > 0)
  {
    params.Replace(_T("max length"),length);    // ORACLE DOES THIS!!
    params.Replace(_T("length"),length);
  }
  else if(p_type->m_type_name.CompareNoCase(_T("varchar")) == 0)
  {
    // SQL-Server does this as a replacement for CLOB
    params.Replace(_T("max length"),_T("max"));
    max = true;
  }

  if(m_columnSize > 0)
  {
    params.Replace(_T("precision"),precision);
    params.Replace(_T("scale"),scale);
  }
  else if(!max)
  {
    params.Empty();
  }
  // Make sure we have parenthesis
  if(!params.IsEmpty() && params.Left(1) != _T("(") && params != _T(","))
  {
    params = _T("(") + params + _T(")");
  }
  if(params != _T(","))
  {
    return m_typeName + params;
  }
  return m_typeName;
}

}