////////////////////////////////////////////////////////////////////////
//
// File: SQLDataType.cpp
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
  const char* name;
  int         type;
}
DataTypes;

static DataTypes allTypes[] = 
{
   { "<NO TYPE>",                  0                               }
  ,{ "CHAR",                       SQL_C_CHAR                      } 
  ,{ "SHORT",                      SQL_C_SHORT                     }
  ,{ "SIGNED SHORT",               SQL_C_SSHORT                    }
  ,{ "UNSIGNED SHORT",             SQL_C_USHORT                    }
  ,{ "LONG",                       SQL_C_LONG                      }
  ,{ "SIGNED LONG",                SQL_C_SLONG                     }
  ,{ "UNSIGNED LONG",              SQL_C_ULONG                     }
  ,{ "REAL",                       SQL_C_FLOAT                     }
  ,{ "FLOAT",                      SQL_C_FLOAT                     }
  ,{ "DOUBLE",                     SQL_C_DOUBLE                    }
  ,{ "BIT",                        SQL_C_BIT                       }
  ,{ "TINYINT",                    SQL_C_TINYINT                   }
  ,{ "SIGNED TINYINT",             SQL_C_STINYINT                  }
  ,{ "UNSIGNED TINYINT",           SQL_C_UTINYINT                  }
  ,{ "SIGNED BIGINT",              SQL_C_SBIGINT                   }
  ,{ "UNSIGNED BIGINT",            SQL_C_UBIGINT                   }
  ,{ "DECIMAL",                    SQL_C_NUMERIC                   }
  ,{ "NUMERIC",                    SQL_C_NUMERIC                   }
  ,{ "GUID",                       SQL_C_GUID                      }
  ,{ "BINARY",                     SQL_C_BINARY                    }
  ,{ "DATE",                       SQL_C_DATE                      }
  ,{ "TIME",                       SQL_C_TIME                      }
  ,{ "TIMESTAMP",                  SQL_C_TIMESTAMP                 }
  ,{ "TYPE DATE",                  SQL_C_TYPE_DATE                 }
  ,{ "TYPE TIME",                  SQL_C_TYPE_TIME                 }
  ,{ "TYPE TIMESTAMP",             SQL_C_TYPE_TIMESTAMP            }
  ,{ "INTERVAL YEAR",              SQL_C_INTERVAL_YEAR             }
  ,{ "INTERVAL MONTH",             SQL_C_INTERVAL_MONTH            }
  ,{ "INTERVAL DAY",               SQL_C_INTERVAL_DAY              }
  ,{ "INTERVAL HOUR",              SQL_C_INTERVAL_HOUR             }
  ,{ "INTERVAL MINUTE",            SQL_C_INTERVAL_MINUTE           }
  ,{ "INTERVAL SECOND",            SQL_C_INTERVAL_SECOND           }
  ,{ "INTERVAL YEAR TO MONTH",     SQL_C_INTERVAL_YEAR_TO_MONTH    }
  ,{ "INTERVAL DAY TO HOUR",       SQL_C_INTERVAL_DAY_TO_HOUR      }
  ,{ "INTERVAL DAY TO MINUTE",     SQL_C_INTERVAL_DAY_TO_MINUTE    }
  ,{ "INTERVAL DAY TO SECOND",     SQL_C_INTERVAL_DAY_TO_SECOND    }
  ,{ "INTERVAL HOUR TO MINUTE",    SQL_C_INTERVAL_HOUR_TO_MINUTE   }
  ,{ "INTERVAL HOUR TO SECOND",    SQL_C_INTERVAL_HOUR_TO_SECOND   }
  ,{ "INTERVAL MINUTE TO SECOND",  SQL_C_INTERVAL_MINUTE_TO_SECOND }
  ,{ NULL,                         0                               }
};

// All datatypes not having a counterpart in SQL_C_XXX set
// SQL Datatypes should be used for binding in SQLBindParameter
static DataTypes allOther[] = 
{
  { "VARBINARY",       SQL_VARBINARY      }  // LONG RAW
 ,{ "LONGVARBINARY",   SQL_LONGVARBINARY  }  // BLOB
 ,{ "LONGVARCHAR",     SQL_LONGVARCHAR    }  // CLOB
 ,{ "VARCHAR",         SQL_VARCHAR        }  // VARCHAR2
 ,{ "WCHAR",           SQL_WCHAR          }  // NCHAR
 ,{ "WVARCHAR",        SQL_WVARCHAR       }  // NVARCHAR
 ,{ "WLONGVARCHAR",    SQL_WLONGVARCHAR   }  // NCLOB
 ,{ NULL,              0                  } 
};

// Names must appear in this order to work properly!!
static DataTypes allParams[] = 
{
  { "<UNKNOWN>", P_SQL_PARAM_TYPE_UNKNOWN }    // 0
 ,{ "INPUT",     P_SQL_PARAM_INPUT        }    // 1
 ,{ "INOUT",     P_SQL_PARAM_INPUT_OUTPUT }    // 2
 ,{ "COLUMN",    P_SQL_RESULT_COL         }    // 3
 ,{ "OUTPUT",    P_SQL_PARAM_OUTPUT       }    // 4
 ,{ "RETURN",    P_SQL_RETURN_VALUE       }    // 5
 ,{ NULL,        0                        }
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
  m_sqlType  = FindDatatype(reinterpret_cast<char*>(const_cast<char*>(p_typeName.GetString())));
}

SQLDataType::SQLDataType(MetaColumn& p_column)
            :m_typeName     (p_column.m_typename)
            ,m_columnSize   (p_column.m_columnSize)
            ,m_bufferSize   (p_column.m_bufferLength)
            ,m_decimalDigits(p_column.m_decimalDigits)
            ,m_radix        (p_column.m_numRadix)
            ,m_octetLength  (p_column.m_octet_length)
            ,m_subType      (p_column.m_sub_datatype)
{
  m_sqlType = FindDatatype(reinterpret_cast<char*>(const_cast<char*>(m_typeName.GetString())));
}

SQLDataType::~SQLDataType()
{
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
SQLDataType::FindDatatype(char* p_type)
{
  DataTypes* types = allTypes;
  while(types->name)
  {
    if(_stricmp(p_type,types->name) == 0)
    {
      return types->type;
    }
    ++types;
  }
  return 0;
}

/*static*/ const char*
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
  return "";
}

/*static*/ int
SQLDataType::FindParamtype(char* p_type)
{
  DataTypes* param = allParams;
  while(param->name)
  {
    if(_stricmp(p_type,param->name) == 0)
    {
      return param->type;
    }
    ++param;
  }
  return 0;
}

/*static*/ const char*
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
  return "";
}

// Find the extra SQL_XXX datatype for
// variants of CHAR and BINARY
/*static*/ int
SQLDataType::FindSQLDatatype(char* p_type)
{
  DataTypes* types = allOther;
  while(types->name)
  {
    if(_stricmp(types->name,p_type) == 0)
    {
      return types->type;
    }
    ++types;
  }
  return 0;
}

/*static*/ const char*
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
  return "";
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
    case SQL_WLONGVARCHAR:      return "Edm.String";
    case SQL_C_SHORT:
    case SQL_C_SSHORT:
    case SQL_C_USHORT:          return "Edm.Int16";
    case SQL_C_UTINYINT:        return "Edm.Byte";
    case SQL_C_TINYINT:
    case SQL_C_STINYINT:        return "Edm.SByte";
    case SQL_C_LONG:
    case SQL_C_ULONG:
    case SQL_C_SLONG:           return "Edm.Int32";
    case SQL_C_UBIGINT:
    case SQL_C_SBIGINT:         return "Edm.Int64";
    case SQL_C_FLOAT:
    case SQL_C_DOUBLE:          return "Edm.Double";
    case SQL_C_BIT:             return "Edm.Boolean";
    case SQL_C_NUMERIC:         return "Edm.Decimal";
    case SQL_VARBINARY:
    case SQL_LONGVARBINARY:
    case SQL_C_BINARY:          return "Edm.Binary";
    case SQL_C_DATE:
    case SQL_C_TYPE_DATE:       return "Edm.Date";
    case SQL_C_TIME:
    case SQL_C_TYPE_TIME:       return "Edm.TimeOfDay";
    case SQL_C_TIMESTAMP:
    case SQL_C_TYPE_TIMESTAMP:  return "Edm.DateTimeOffset";
    case SQL_C_GUID:            return "Edm.Guid";
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
    case SQL_C_INTERVAL_MINUTE_TO_SECOND:  return "Edm.Duration";
  }
  return "Edm.Unknown";
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
    length.Format("%d",m_columnSize);
  }
  if(m_columnSize > 0 || m_decimalDigits > 0)
  {
    precision.Format("%d",m_columnSize);
    scale.Format("%d",m_decimalDigits);
  }

  // Replace as strings
  if(m_columnSize > 0)
  {
    params.Replace("max length",length);    // ORACLE DOES THIS!!
    params.Replace("length",length);
  }
  else if(p_type->m_type_name.CompareNoCase("varchar") == 0)
  {
    // SQL-Server does this as a replacement for CLOB
    params.Replace("max length","max");
    max = true;
  }

  if(m_columnSize > 0)
  {
    params.Replace("precision",precision);
    params.Replace("scale",scale);
  }
  else if(!max)
  {
    params.Empty();
  }
  // Make sure we have parenthesis
  if(!params.IsEmpty() && params.Left(1) != "(" && params != ",")
  {
    params = "(" + params + ")";
  }
  if(params != ",")
  {
    return m_typeName + params;
  }
  return m_typeName;
}

}