////////////////////////////////////////////////////////////////////////
//
// File: SQLVariant.cpp
//
// Copyright (c) 1998- 2014 ir. W.E. Huisman
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
// Last Revision:   01-01-2015
// Version number:  1.1.0
//
#include "StdAfx.h"
#include "SQLVariant.h"
#include "SQLDate.h"
#include "SQLTime.h"
#include "SQLTimestamp.h"
#include <math.h>
#include <float.h>
#include <limits.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning (disable : 4996)

// Internal NUMERIC conversion routines
static void sqlnum_scale(int *ary, int s);
static void sqlnum_unscale_le(int *ary);
static void sqlnum_unscale_be(int *ary, int start);
static void sqlnum_carry(int *ary);

DataTypes allTypes[] = 
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
DataTypes allOther[] = 
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
DataTypes allParams[] = 
{
  { "<UNKNOWN>", SQL_PARAM_TYPE_UNKNOWN }    // 0
 ,{ "INPUT",     SQL_PARAM_INPUT        }    // 1
 ,{ "INOUT",     SQL_PARAM_INPUT_OUTPUT }    // 2
 ,{ "COLUMN",    SQL_RESULT_COL         }    // 3
 ,{ "OUTPUT",    SQL_PARAM_OUTPUT       }    // 4
 ,{ "RETURN",    SQL_RETURN_VALUE       }    // 5
 ,{ NULL,        0                      }
};

// General constructors
SQLVariant::SQLVariant()
{
  Init();
}

SQLVariant::SQLVariant(int p_type,int p_space)
{
  Init();
  ReserveSpace(p_type,p_space);
}

SQLVariant::SQLVariant(SQLVariant* that)
{
  // Use assignment operator
  *this = *that;
}

// XTOR SQL_C_CHAR
SQLVariant::SQLVariant(const char* p_data)
{
  Init();
  SetData(SQL_C_CHAR,p_data);
}

SQLVariant::SQLVariant(CString& p_data)
{
  Init();
  SetData(SQL_C_CHAR,(const char*)p_data);
}

// XTOR SQL_C_SHORT / SQL_C_SSHORT
SQLVariant::SQLVariant(short p_short)
{
  Init();
  m_datatype    = SQL_C_SSHORT;
  m_sqlDatatype = SQL_C_SSHORT;
  m_indicator   = 0;
  m_data.m_dataSSHORT  = p_short;
}

// XTOR SQL_C_USHORT
SQLVariant::SQLVariant(unsigned short p_short)
{
  Init();
  m_datatype    = SQL_C_USHORT;
  m_sqlDatatype = SQL_C_USHORT;
  m_indicator   = 0;
  m_data.m_dataUSHORT  = p_short;
}

// XTOR SQL_C_LONG / SQL_C_SLONG
SQLVariant::SQLVariant(long p_long)
{
  Init();
  m_datatype    = SQL_C_SLONG;
  m_sqlDatatype = SQL_C_SLONG;
  m_indicator   = 0;
  m_data.m_dataSLONG = p_long;
}

// XTOR SQL_C_ULONG
SQLVariant::SQLVariant(unsigned long p_long)
{
  Init();
  m_datatype    = SQL_C_ULONG;
  m_sqlDatatype = SQL_C_ULONG;
  m_indicator   = 0;
  m_data.m_dataULONG = p_long;
}

// XTOR SQL_C_FLOAT
SQLVariant::SQLVariant(float p_float)
{
  Init();
  m_datatype    = SQL_C_FLOAT;
  m_sqlDatatype = SQL_C_FLOAT;
  m_indicator   = 0;
  m_data.m_dataFLOAT = p_float;
}

// XTOR SQL_C_DOUBLE
SQLVariant::SQLVariant(double p_double)
{
  Init();
  m_datatype    = SQL_C_DOUBLE;
  m_sqlDatatype = SQL_C_DOUBLE;
  m_indicator   = 0;
  m_data.m_dataDOUBLE = p_double;
}

// XTOR SQL_C_BIT
SQLVariant::SQLVariant(bool p_bit)
{
  Init();
  m_datatype    = SQL_C_BIT;
  m_sqlDatatype = SQL_C_BIT;
  m_indicator   = 0;
  m_data.m_dataBIT = p_bit;
}

// XTOR SQL_C_TINYINT / SQL_C_STINYINT
SQLVariant::SQLVariant(char p_tinyint)
{
  Init();
  m_datatype    = SQL_C_STINYINT;
  m_sqlDatatype = SQL_C_STINYINT;
  m_indicator   = 0;
  m_data.m_dataSTINYINT = p_tinyint;
}

// XTOR SQL_C_UTINYINT
SQLVariant::SQLVariant(unsigned char p_tinyint)
{
  Init();
  m_datatype    = SQL_C_UTINYINT;
  m_sqlDatatype = SQL_C_UTINYINT;
  m_indicator   = 0;
  m_data.m_dataUTINYINT = p_tinyint;
}

// XTOR SQL_C_BIGINT
SQLVariant::SQLVariant(__int64 p_bigint)
{
  Init();
  m_datatype    = SQL_C_SBIGINT;
  m_sqlDatatype = SQL_C_SBIGINT;
  m_indicator   = 0;
  m_data.m_dataSBIGINT = p_bigint;
}

// XTOR SQL_C_UBIGINT
SQLVariant::SQLVariant(unsigned __int64 p_bigint)
{
  Init();
  m_datatype    = SQL_C_UBIGINT;
  m_sqlDatatype = SQL_C_UBIGINT;
  m_indicator   = 0;
  m_data.m_dataUBIGINT = p_bigint;
}

// XTOR SQL_C_NUMERIC
SQLVariant::SQLVariant(SQL_NUMERIC_STRUCT* p_numeric)
{
  Init();
  m_datatype    = SQL_C_NUMERIC;
  m_sqlDatatype = SQL_C_NUMERIC;
  m_indicator   = 0;
  memcpy(&m_data.m_dataNUMERIC,p_numeric,sizeof(SQL_NUMERIC_STRUCT));
}

// XTOR SQL_C_GUID
SQLVariant::SQLVariant(SQLGUID* p_guid)
{
  Init();
  m_datatype    = SQL_C_GUID;
  m_sqlDatatype = SQL_C_GUID;
  m_indicator   = 0;
  memcpy(&m_data.m_dataGUID,p_guid,sizeof(SQLGUID));
}

// XTOR SQL_C_BINARY
SQLVariant::SQLVariant(void* p_binary,size_t p_size)
{
  Init();
  m_datatype    = SQL_C_BINARY;
  m_sqlDatatype = SQL_C_BINARY;
  m_indicator   = 0;
  m_data.m_dataBINARY = malloc(p_size + 1);
  memcpy(m_data.m_dataBINARY,p_binary,p_size);
}

// XTOR SQL_C_DATE
SQLVariant::SQLVariant(DATE_STRUCT* p_date)
{
  Init();
  m_datatype    = SQL_C_DATE;
  m_sqlDatatype = SQL_C_DATE;
  m_indicator   = 0;
  memcpy(&m_data.m_dataDATE,p_date,sizeof(DATE_STRUCT));
}

// XTOR SQL_C_TIME
SQLVariant::SQLVariant(TIME_STRUCT* p_time)
{
  Init();
  m_datatype    = SQL_C_TIME;
  m_sqlDatatype = SQL_C_TIME;
  m_indicator   = 0;
  memcpy(&m_data.m_dataTIME,p_time,sizeof(TIME_STRUCT));
}

SQLVariant::SQLVariant(TIMESTAMP_STRUCT* p_stamp)
{
  Init();
  m_datatype    = SQL_C_TIMESTAMP;
  m_sqlDatatype = SQL_C_TIMESTAMP;
  m_indicator   = 0;
  memcpy(&m_data.m_dataTIMESTAMP,p_stamp,sizeof(TIMESTAMP_STRUCT));
}

// XTOR SQL_C_INTERVAL_YEAR -> SQL_C_INTERVAL_DAY_TO_SECOND
SQLVariant::SQLVariant(int p_type,SQL_INTERVAL_STRUCT* p_inter)
{
  Init();
  m_datatype    = p_type;
  m_sqlDatatype = p_type;
  m_indicator   = 0;
  if(IsIntervalType() == false)
  {
    m_datatype    = SQL_C_INTERVAL_DAY_TO_SECOND;
    m_sqlDatatype = SQL_C_INTERVAL_DAY_TO_SECOND;
  }
  memcpy(&m_data.m_dataINTERVAL,p_inter,sizeof(SQL_INTERVAL_STRUCT));
}

SQLVariant::SQLVariant(SQLDate* p_date)
{
  Init();
  m_datatype    = SQL_C_DATE;
  m_sqlDatatype = SQL_C_DATE;

  if(p_date->IsNull())
  {
    m_indicator = SQL_NULL_DATA;
  }
  else
  {
    m_indicator   = 0;
    m_data.m_dataDATE.year  = (SQLSMALLINT) p_date->Year();
    m_data.m_dataDATE.month = (SQLSMALLINT) p_date->Month();
    m_data.m_dataDATE.day   = (SQLSMALLINT) p_date->Day();
  }
}

SQLVariant::SQLVariant(SQLTime* p_time)
{
  Init();
  m_datatype    = SQL_C_TIME;
  m_sqlDatatype = SQL_C_TIME;

  if(p_time->IsNull())
  {
    m_indicator = SQL_NULL_DATA;
  }
  else
  {
    m_indicator   = 0;
    m_data.m_dataTIME.hour   = (SQLSMALLINT) p_time->Hour();
    m_data.m_dataTIME.minute = (SQLSMALLINT) p_time->Minute();
    m_data.m_dataTIME.second = (SQLSMALLINT) p_time->Second();
  }
}

SQLVariant::SQLVariant(SQLTimestamp* p_stamp)
{
  Init();
  m_datatype    = SQL_C_TIMESTAMP;
  m_sqlDatatype = SQL_C_TIMESTAMP;

  if(p_stamp->IsNull())
  {
    m_indicator = SQL_NULL_DATA;
  }
  else
  {
    m_indicator   = 0;
    m_data.m_dataTIMESTAMP.year     = (SQLSMALLINT) p_stamp->Year();
    m_data.m_dataTIMESTAMP.month    = (SQLSMALLINT) p_stamp->Month();
    m_data.m_dataTIMESTAMP.day      = (SQLSMALLINT) p_stamp->Day();
    m_data.m_dataTIMESTAMP.hour     = (SQLSMALLINT) p_stamp->Hour();
    m_data.m_dataTIMESTAMP.minute   = (SQLSMALLINT) p_stamp->Minute();
    m_data.m_dataTIMESTAMP.second   = (SQLSMALLINT) p_stamp->Second();
    m_data.m_dataTIMESTAMP.fraction = (SQLUINTEGER) p_stamp->Fraction();
  }
}

// GENERAL DTOR
SQLVariant::~SQLVariant()
{
  // De-allocate char/binary buffers
  ResetDataType(0);
}

void
SQLVariant::Init()
{
  memset(&m_data,0,sizeof(m_data));
  m_datatype        = 0;
  m_sqlDatatype     = 0;
  m_binaryLength    = 0;
  m_binaryPieceSize = 0;
  m_columnNumber    = 0;
  m_paramType       = 0;
  m_useAtExec       = false;
  m_indicator       = SQL_NULL_DATA;
}

void
SQLVariant::ReserveSpace(int p_type,int p_space)
{
  ResetDataType(p_type);
  if(p_type == SQL_C_CHAR)
  {
    m_binaryLength = p_space + 1;
    m_data.m_dataCHAR = (char*) calloc(m_binaryLength,1);
  }
  if(p_type == SQL_C_BINARY)
  {
    m_binaryLength = p_space + 1;
    m_data.m_dataBINARY = (unsigned char*) calloc(m_binaryLength,1);
  }
  // Reserving space means, that the contents is reset to logical NULL
  // Only, leave the indicator in case of AT_EXEC_DATA so drivers will not get upset!
  // AT_EXEC_DATA means that the contents cannot be known beforehand, but must be supplied
  if(m_indicator >= 0 && m_useAtExec == false)
  {
    m_indicator = SQL_NULL_DATA;
  }
}

// Check if the data is 'empty'
// CHAR -> Empty string, scalar types are 0 or 0.0
bool
SQLVariant::IsEmpty()
{
  int len = 0;
  unsigned char* data = (unsigned char*)m_data.m_dataSHORT;
  switch(m_datatype)
  {
    case SQL_C_CHAR:    return ((m_data.m_dataCHAR == NULL) || (m_data.m_dataCHAR != NULL && m_data.m_dataCHAR[0] == 0));
    case SQL_C_BINARY:  return m_binaryLength == 0;
    default:            len = GetDataSize();
                        for(int i=0;i<len;++i)
                        {
                          if(*data++ != 0)
                          {
                            return false;
                          }
                        }
                        return true;
  }
}

bool
SQLVariant::IsNumericType()
{
  switch(m_datatype)
  {
    case SQL_C_SHORT:     // Fall through
    case SQL_C_SSHORT:    // Fall through
    case SQL_C_USHORT:    // Fall through
    case SQL_C_LONG:      // Fall through
    case SQL_C_SLONG:     // Fall through
    case SQL_C_ULONG:     // Fall through
    case SQL_C_FLOAT:     // Fall through
    case SQL_C_DOUBLE:    // Fall through
    case SQL_C_TINYINT:   // Fall through
    case SQL_C_STINYINT:  // Fall through
    case SQL_C_UTINYINT:  // Fall through
    case SQL_C_SBIGINT:   // Fall through
    case SQL_C_UBIGINT:   // Fall through
    case SQL_C_NUMERIC:   return true;
    default:              return false;
  }
}

bool
SQLVariant::IsIntervalType()
{
  switch(m_datatype)
  {
    case SQL_C_INTERVAL_YEAR:             // Fall through
    case SQL_C_INTERVAL_MONTH:            // Fall through
    case SQL_C_INTERVAL_DAY:              // Fall through
    case SQL_C_INTERVAL_HOUR:             // Fall through
    case SQL_C_INTERVAL_MINUTE:           // Fall through
    case SQL_C_INTERVAL_SECOND:           // Fall through
    case SQL_C_INTERVAL_YEAR_TO_MONTH:    // Fall through
    case SQL_C_INTERVAL_DAY_TO_HOUR:      // Fall through
    case SQL_C_INTERVAL_DAY_TO_MINUTE:    // Fall through
    case SQL_C_INTERVAL_DAY_TO_SECOND:    // Fall through
    case SQL_C_INTERVAL_HOUR_TO_MINUTE:   // Fall through
    case SQL_C_INTERVAL_HOUR_TO_SECOND:   // Fall through
    case SQL_C_INTERVAL_MINUTE_TO_SECOND: return true;
    default:                              return false;
  }
}

bool
SQLVariant::IsDateTimeType()
{
  switch(m_datatype)
  {
    case SQL_C_TIME:
    case SQL_C_DATE:
    case SQL_C_TIMESTAMP:
    case SQL_C_TYPE_TIME:
    case SQL_C_TYPE_DATE:
    case SQL_C_TYPE_TIMESTAMP:  return true;
    default:                    return false;
  }
}

void
SQLVariant::ResetDataType(int p_type)
{
  switch(m_datatype)
  {
    case SQL_C_CHAR:  free(m_data.m_dataCHAR);  break;
    case SQL_C_BINARY:free(m_data.m_dataBINARY);break;
    default:          break;
  }
  memset(&m_data,0,sizeof(m_data));
  m_datatype    = p_type;
  m_sqlDatatype = p_type;
  m_indicator   = SQL_NULL_DATA;
}

void
SQLVariant::SetNULL()
{
  if(m_datatype == SQL_C_CHAR || m_datatype == SQL_C_BINARY)
  {
    m_data.m_dataCHAR[0] = 0;
  }
  else
  {
    memset(&m_data,0,sizeof(m_data));
  }
  m_indicator = SQL_NULL_DATA;
}

void
SQLVariant::SetSQLDataType(int p_type)
{
  // type should have relation to m_datatype!!
  if(m_datatype == SQL_C_CHAR)
  {
    if(p_type == SQL_WCHAR       || 
       p_type == SQL_VARCHAR     ||
       p_type == SQL_WVARCHAR    ||
       p_type == SQL_LONGVARCHAR  )
    {
      m_sqlDatatype = p_type;
    }
  }
  if(m_datatype == SQL_C_BINARY)
  {
    if(p_type == SQL_VARBINARY     ||
       p_type == SQL_LONGVARBINARY )
    {
      m_sqlDatatype = p_type;
    }
  }
}

// Reduction of SQL_XXX to SQL_C_XXX type
int
SQLVariant::FindDataTypeFromSQLType()
{
  switch(m_sqlDatatype)
  {
    case SQL_WCHAR:
    case SQL_VARCHAR:
    case SQL_WVARCHAR:  
    case SQL_LONGVARCHAR:   return SQL_C_CHAR;
    case SQL_VARBINARY:
    case SQL_LONGVARBINARY: return SQL_C_BINARY;
  }
  // Gets you the ordinary datatype
  return m_datatype;
}

int
SQLVariant::FindDatatype(char* p_type)
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

char* 
SQLVariant::FindDatatype(int p_type)
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

int
SQLVariant::FindParamtype(char* p_type)
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

char*
SQLVariant::FindParamtype(int p_type)
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
int
SQLVariant::FindSQLDatatype(char* p_type)
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

char* 
SQLVariant::FindSQLDatatype(int p_type)
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

void
SQLVariant::SetParameterType(int p_type)
{
  if(p_type >= SQL_PARAM_TYPE_UNKNOWN &&
     p_type <= SQL_RETURN_VALUE)
  {
    m_paramType = p_type;
  }
}

//////////////////////////////////////////////////////////////////////////
//
//  GENERAL ACCESS
//
//////////////////////////////////////////////////////////////////////////

void
SQLVariant::GetAsString(CString& result)
{
  if(m_indicator == SQL_NULL_DATA)
  {
    result = "";
    return;
  }
  unsigned char* pointer;
  switch(m_datatype)
  {
    case SQL_C_CHAR:                      result = m_data.m_dataCHAR;
                                          break;
  //case SQL_C_VARBOOKMARK:               // Fall through
    case SQL_C_BINARY:                    pointer = (unsigned char*)result.GetBufferSetLength(m_binaryLength * 2 + 2);
                                          BinaryToString(pointer,m_binaryLength*2 + 2);
                                          result.ReleaseBuffer();
                                          break;
    case SQL_C_SHORT:                     // Fall through
    case SQL_C_SSHORT:                    result.Format("%i",(int)m_data.m_dataSHORT);
                                          break;
    case SQL_C_USHORT:                    result.Format("%ui",(unsigned)m_data.m_dataUSHORT);
                                          break;
    case SQL_C_LONG:                      // Fall through
    case SQL_C_SLONG:                     result.Format("%li",m_data.m_dataLONG);
                                          break;
  //case SQL_C_BOOKMARK:                  // Fall through
    case SQL_C_ULONG:                     result.Format("%lu",m_data.m_dataULONG);
                                          break;
    case SQL_C_FLOAT:                     result.Format("%.7g",m_data.m_dataFLOAT);
                                          break;
    case SQL_C_DOUBLE:                    // Standaard Oracle ODBC kan maximaal 15 decimalen ophalen
                                          result.Format("%.15lg",m_data.m_dataDOUBLE);
                                          break;
    case SQL_C_BIT:                       result.Format("%d",(int)m_data.m_dataBIT);
                                          break;
    case SQL_C_TINYINT:                   // Fall through
    case SQL_C_STINYINT:                  result.Format("%d",(int)m_data.m_dataTINYINT);
                                          break;
    case SQL_C_UTINYINT:                  result.Format("%d",(unsigned)m_data.m_dataUTINYINT);
                                          break;
    case SQL_C_SBIGINT:                   result.Format("%I64d",m_data.m_dataSBIGINT);
                                          break;
    case SQL_C_UBIGINT:                   result.Format("%I64u",m_data.m_dataUBIGINT);
                                          break;
    case SQL_C_NUMERIC:                   NumericToString(&m_data.m_dataNUMERIC,result);
                                          break;
    case SQL_C_GUID:                      result.Format("{%LX-%X-%X-%X%X-%X%X%X%X%X%X}"
                                                       ,m_data.m_dataGUID.Data1
                                                       ,m_data.m_dataGUID.Data2
                                                       ,m_data.m_dataGUID.Data3
                                                       ,m_data.m_dataGUID.Data4[0]
                                                       ,m_data.m_dataGUID.Data4[1]
                                                       ,m_data.m_dataGUID.Data4[2]
                                                       ,m_data.m_dataGUID.Data4[3]
                                                       ,m_data.m_dataGUID.Data4[4]
                                                       ,m_data.m_dataGUID.Data4[5]
                                                       ,m_data.m_dataGUID.Data4[6]
                                                       ,m_data.m_dataGUID.Data4[7]);
                                          break;
    case SQL_C_DATE:                      // Fall through
    case SQL_C_TYPE_DATE:                 result.Format("%02.2d-%02.2d-%04.4d"
                                                       ,m_data.m_dataDATE.day
                                                       ,m_data.m_dataDATE.month
                                                       ,m_data.m_dataDATE.year);
                                          break;
    case SQL_C_TIME:                      // Fall through
    case SQL_C_TYPE_TIME:                 result.Format("%02.2d:%02.2d:%02.2d"
                                                       ,m_data.m_dataTIME.hour
                                                       ,m_data.m_dataTIME.minute
                                                       ,m_data.m_dataTIME.second);
                                          break;
    case SQL_C_TIMESTAMP:                 // Fall through
    case SQL_C_TYPE_TIMESTAMP:            result.Format("%04.4d-%02.2d-%02.2d %02.2d:%02.2d:%02.2d"
                                                       ,m_data.m_dataTIMESTAMP.year
                                                       ,m_data.m_dataTIMESTAMP.month
                                                       ,m_data.m_dataTIMESTAMP.day
                                                       ,m_data.m_dataTIMESTAMP.hour
                                                       ,m_data.m_dataTIMESTAMP.minute
                                                       ,m_data.m_dataTIMESTAMP.second);
                                          if(m_data.m_dataTIMESTAMP.fraction)
                                          {
                                            // Fractions are in NANO-Second resolution
                                            CString frac;
                                            frac.Format("%0.6f",(double)m_data.m_dataTIMESTAMP.fraction / 1000000000.0);
                                            result += frac.Mid(1);
                                          }
                                          break;
    case SQL_C_INTERVAL_YEAR:             result.Format("%d",m_data.m_dataINTERVAL.intval.year_month.year);
                                          break;
    case SQL_C_INTERVAL_MONTH:            result.Format("%d",m_data.m_dataINTERVAL.intval.year_month.month);
                                          break;
    case SQL_C_INTERVAL_DAY:              result.Format("%d",m_data.m_dataINTERVAL.intval.day_second.day);
                                          break;
    case SQL_C_INTERVAL_HOUR:             result.Format("%d",m_data.m_dataINTERVAL.intval.day_second.hour);
                                          break;
    case SQL_C_INTERVAL_MINUTE:           result.Format("%d",m_data.m_dataINTERVAL.intval.day_second.minute);
                                          break;
    case SQL_C_INTERVAL_SECOND:           result.Format("%d",m_data.m_dataINTERVAL.intval.day_second.second);
                                          break;
    case SQL_C_INTERVAL_YEAR_TO_MONTH:    result.Format("%d-%d"
                                                       ,m_data.m_dataINTERVAL.intval.year_month.year
                                                       ,m_data.m_dataINTERVAL.intval.year_month.month);
                                          break;
    case SQL_C_INTERVAL_DAY_TO_HOUR:      result.Format("%d %d"
                                                       ,m_data.m_dataINTERVAL.intval.day_second.day
                                                       ,m_data.m_dataINTERVAL.intval.day_second.hour);
                                          break;
    case SQL_C_INTERVAL_DAY_TO_MINUTE:    result.Format("%d %d:%d"
                                                        ,m_data.m_dataINTERVAL.intval.day_second.day
                                                        ,m_data.m_dataINTERVAL.intval.day_second.hour
                                                        ,m_data.m_dataINTERVAL.intval.day_second.minute);
                                          break;
    case SQL_C_INTERVAL_DAY_TO_SECOND:    result.Format("%d %d:%d:%d"
                                                        ,m_data.m_dataINTERVAL.intval.day_second.day
                                                        ,m_data.m_dataINTERVAL.intval.day_second.hour
                                                        ,m_data.m_dataINTERVAL.intval.day_second.minute
                                                        ,m_data.m_dataINTERVAL.intval.day_second.second);
                                          break;
    case SQL_C_INTERVAL_HOUR_TO_MINUTE:   result.Format("%d:%d"
                                                       ,m_data.m_dataINTERVAL.intval.day_second.hour
                                                       ,m_data.m_dataINTERVAL.intval.day_second.minute);
                                          break;
    case SQL_C_INTERVAL_HOUR_TO_SECOND:   result.Format("%d:%d:%d"
                                                        ,m_data.m_dataINTERVAL.intval.day_second.hour
                                                        ,m_data.m_dataINTERVAL.intval.day_second.minute
                                                        ,m_data.m_dataINTERVAL.intval.day_second.second);
                                          break;
    case SQL_C_INTERVAL_MINUTE_TO_SECOND: result.Format("%d:%d"
                                                        ,m_data.m_dataINTERVAL.intval.day_second.minute
                                                        ,m_data.m_dataINTERVAL.intval.day_second.second);
                                          break;
    default:                              result = "<NO VALUE>";
                                          break;
  }
  // SET THE INTERVAL SIGN
  if(m_datatype <= SQL_C_INTERVAL_MINUTE_TO_SECOND &&
     m_datatype >= SQL_C_INTERVAL_YEAR)
  {
    if(m_data.m_dataINTERVAL.interval_sign)
    {
      result = "-" + result;
    }
  }
}

void*   
SQLVariant::GetDataPointer()
{
  void* data = NULL;
  switch(m_datatype)
  {
    case SQL_C_CHAR:                      data = (void*)m_data.m_dataCHAR;
                                          break;
  //case SQL_C_VARBOOKMARK:               // Fall through
    case SQL_C_BINARY:                    data = (void*)m_data.m_dataBINARY;
                                          break;
    case SQL_C_SHORT:                     // Fall through
    case SQL_C_SSHORT:                    data = (void*)&m_data.m_dataSHORT;
                                          break;
    case SQL_C_USHORT:                    data = (void*)&m_data.m_dataUSHORT;
                                          break;
    case SQL_C_LONG:                      // Fall through
    case SQL_C_SLONG:                     data = (void*)&m_data.m_dataLONG;
                                          break;
  //case SQL_C_VARBOOKMARK:               // Fall through
    case SQL_C_ULONG:                     data = (void*)&m_data.m_dataULONG;
                                          break;
    case SQL_C_FLOAT:                     data = (void*)&m_data.m_dataFLOAT;
                                          break;
    case SQL_C_DOUBLE:                    data = (void*)&m_data.m_dataDOUBLE;
                                          break;
    case SQL_C_BIT:                       data = (void*)&m_data.m_dataBIT;
                                          break;
    case SQL_C_TINYINT:                   // Fall through
    case SQL_C_STINYINT:                  data = (void*)&m_data.m_dataTINYINT;
                                          break;
    case SQL_C_UTINYINT:                  data = (void*)&m_data.m_dataUTINYINT;
                                          break;
    case SQL_C_SBIGINT:                   data = (void*)&m_data.m_dataSBIGINT;
                                          break;
    case SQL_C_UBIGINT:                   data = (void*)&m_data.m_dataUBIGINT;
                                          break;
    case SQL_C_NUMERIC:                   data = (void*)&m_data.m_dataNUMERIC;
                                          break;
    case SQL_C_GUID:                      data = (void*)&m_data.m_dataGUID;
                                          break;
    case SQL_C_DATE:                      // Fall through
    case SQL_C_TYPE_DATE:                 data = (void*)&m_data.m_dataDATE;
                                          break;
    case SQL_C_TIME:                      // Fall through
    case SQL_C_TYPE_TIME:                 data = (void*)&m_data.m_dataTIME;
                                          break;
    case SQL_C_TIMESTAMP:                 // Fall through
    case SQL_C_TYPE_TIMESTAMP:            data = (void*)&m_data.m_dataTIMESTAMP;
                                          break;
    case SQL_C_INTERVAL_YEAR:             // Fall through
    case SQL_C_INTERVAL_MONTH:            // Fall through
    case SQL_C_INTERVAL_DAY:              // Fall through
    case SQL_C_INTERVAL_HOUR:             // Fall through
    case SQL_C_INTERVAL_MINUTE:           // Fall through
    case SQL_C_INTERVAL_SECOND:           // Fall through
    case SQL_C_INTERVAL_YEAR_TO_MONTH:    // Fall through
    case SQL_C_INTERVAL_DAY_TO_HOUR:      // Fall through
    case SQL_C_INTERVAL_DAY_TO_MINUTE:    // Fall through
    case SQL_C_INTERVAL_DAY_TO_SECOND:    // Fall through
    case SQL_C_INTERVAL_HOUR_TO_MINUTE:   // Fall through
    case SQL_C_INTERVAL_HOUR_TO_SECOND:   // Fall through
    case SQL_C_INTERVAL_MINUTE_TO_SECOND: data = (void*)&m_data.m_dataINTERVAL;
                                          break;
    default:                              break;
  }
  return data;
}

int     
SQLVariant::GetDataSize()
{
  switch(m_datatype)
  {
    case SQL_C_CHAR:                      // Fall through
  //case SQL_C_VARBOOKMARK:               // Fall through
    case SQL_C_BINARY:                    return m_binaryLength;
    case SQL_C_SHORT:                     // Fall through
    case SQL_C_SSHORT:                    return sizeof(short);
    case SQL_C_USHORT:                    return sizeof(unsigned short);
    case SQL_C_LONG:                      // Fall through
    case SQL_C_SLONG:                     return sizeof(long);
  //case SQL_C_BOOKMARK:                  // Fall through
    case SQL_C_ULONG:                     return sizeof(unsigned long);
    case SQL_C_FLOAT:                     return sizeof(float);
    case SQL_C_DOUBLE:                    return sizeof(double);
    case SQL_C_BIT:                       return sizeof(unsigned char);
    case SQL_C_TINYINT:                   // Fall through
    case SQL_C_STINYINT:                  return sizeof(char);
    case SQL_C_UTINYINT:                  return sizeof(unsigned char);
    case SQL_C_SBIGINT:                   // Fall through
    case SQL_C_UBIGINT:                   return sizeof(SQLBIGINT);
    case SQL_C_NUMERIC:                   return sizeof(SQL_NUMERIC_STRUCT);
    case SQL_C_GUID:                      return sizeof(SQLGUID);
    case SQL_C_DATE:                      // Fall through
    case SQL_C_TYPE_DATE:                 return sizeof(DATE_STRUCT);
    case SQL_C_TIME:                      // Fall through
    case SQL_C_TYPE_TIME:                 return sizeof(TIME_STRUCT);
    case SQL_C_TIMESTAMP:                 // Fall through
    case SQL_C_TYPE_TIMESTAMP:            return sizeof(TIMESTAMP_STRUCT);
    case SQL_C_INTERVAL_YEAR:             // Fall through
    case SQL_C_INTERVAL_MONTH:            // Fall through
    case SQL_C_INTERVAL_DAY:              // Fall through
    case SQL_C_INTERVAL_HOUR:             // Fall through
    case SQL_C_INTERVAL_MINUTE:           // Fall through
    case SQL_C_INTERVAL_SECOND:           // Fall through
    case SQL_C_INTERVAL_YEAR_TO_MONTH:    // Fall through
    case SQL_C_INTERVAL_DAY_TO_HOUR:      // Fall through
    case SQL_C_INTERVAL_DAY_TO_MINUTE:    // Fall through
    case SQL_C_INTERVAL_DAY_TO_SECOND:    // Fall through
    case SQL_C_INTERVAL_HOUR_TO_MINUTE:   // Fall through
    case SQL_C_INTERVAL_HOUR_TO_SECOND:   // Fall through
    case SQL_C_INTERVAL_MINUTE_TO_SECOND: return sizeof(SQL_INTERVAL_STRUCT);
    default:                              return 0;
  }
}

//////////////////////////////////////////////////////////////////////////
//
//  ACCESS PER TYPE
//
//////////////////////////////////////////////////////////////////////////

char*
SQLVariant::GetAsChar()
{
  if(m_datatype == SQL_C_CHAR)
  {
    return m_data.m_dataCHAR;
  }
  if(m_datatype == SQL_C_BINARY)
  {
    return (char*)m_data.m_dataBINARY;
  }
  // Should be: GetErrorDatatype(SQL_C_CHAR);
  // Sometimes we come her unexpectedley in various programs
  // This is a non-multi-treaded solution as an after-thought
  // IT IS NOT SAFE. REWRITE YOUR PROGRAM!!
  TRACE("ALARM: Rewrite your program. Use 'GetAsString' instead\n");
  static CString waarde;
  GetAsString(waarde);
  return (char*)waarde.GetString();
}

void*
SQLVariant::GetAsBinary()
{
  if(m_datatype == SQL_C_BINARY)
  {
    return m_data.m_dataBINARY;
  }
  if(m_datatype == SQL_C_CHAR)
  {
    return (void*)m_data.m_dataCHAR;
  }
  // Use GetDataPointer instead
  ThrowErrorDatatype(SQL_C_BINARY);
  // We never come here, but this is to prevent 
  // Warning C4715 not all control paths return a value
  // In various versions of the MSC++ compiler
  return NULL; 
}

bool
SQLVariant::GetAsBoolean()
{
  return GetAsSLong() != 0;
}

short
SQLVariant::GetAsSShort()
{
  switch(m_datatype)
  {
    case SQL_C_CHAR:     return (short)atoi(m_data.m_dataCHAR);
    case SQL_C_SHORT:    return m_data.m_dataSHORT;
    case SQL_C_USHORT:   return UShortToShort(m_data.m_dataUSHORT);
  //case SQL_C_BOOKMARK: // fall through
    case SQL_C_LONG:     // fall through
    case SQL_C_SLONG:    return SLongToShort(m_data.m_dataLONG);
    case SQL_C_ULONG:    return ULongToShort(m_data.m_dataULONG);
    case SQL_C_FLOAT:    return FloatToShort(m_data.m_dataFLOAT);
    case SQL_C_DOUBLE:   return DoubleToShort(m_data.m_dataDOUBLE);
    case SQL_C_BIT:      return (short)m_data.m_dataBIT;
    case SQL_C_TINYINT:  // fall through
    case SQL_C_STINYINT: return (short)m_data.m_dataTINYINT;
    case SQL_C_UTINYINT: return (short)m_data.m_dataUTINYINT;
    case SQL_C_SBIGINT:  return BIGINTToShort(m_data.m_dataSBIGINT);
    case SQL_C_UBIGINT:  return UBIGINTToShort(m_data.m_dataUBIGINT);
    case SQL_C_NUMERIC:{ CString num; 
                         GetAsString(num); 
                         return SLongToShort(atoi(num));
                       }
                       break;
  //case SQL_C_VARBOOKMARK:               // Fall through
    case SQL_C_BINARY:                    // Fall through
    case SQL_C_GUID:                      // Fall through
    case SQL_C_DATE:                      // Fall through
    case SQL_C_TYPE_DATE:                 // Fall through
    case SQL_C_TIME:                      // Fall through
    case SQL_C_TYPE_TIME:                 // Fall through
    case SQL_C_TIMESTAMP:                 // Fall through
    case SQL_C_TYPE_TIMESTAMP:            // Fall through
    case SQL_C_INTERVAL_YEAR:             // Fall through
    case SQL_C_INTERVAL_MONTH:            // Fall through
    case SQL_C_INTERVAL_DAY:              // Fall through
    case SQL_C_INTERVAL_HOUR:             // Fall through
    case SQL_C_INTERVAL_MINUTE:           // Fall through
    case SQL_C_INTERVAL_SECOND:           // Fall through
    case SQL_C_INTERVAL_YEAR_TO_MONTH:    // Fall through
    case SQL_C_INTERVAL_DAY_TO_HOUR:      // Fall through
    case SQL_C_INTERVAL_DAY_TO_MINUTE:    // Fall through
    case SQL_C_INTERVAL_DAY_TO_SECOND:    // Fall through
    case SQL_C_INTERVAL_HOUR_TO_MINUTE:   // Fall through
    case SQL_C_INTERVAL_HOUR_TO_SECOND:   // Fall through
    case SQL_C_INTERVAL_MINUTE_TO_SECOND: // Fall through
    default:                              break;
  }
  ThrowErrorDatatype(SQL_C_SHORT);
  // We never come here, but this is to prevent 
  // Warning C4715 not all control paths return a value
  // In various versions of the MSC++ compiler
  return NULL;
}

unsigned short
SQLVariant::GetAsUShort()
{
  switch(m_datatype)
  {
    case SQL_C_CHAR:     return SLongToUShort(atoi(m_data.m_dataCHAR));
    case SQL_C_SHORT:    return ShortToUShort(m_data.m_dataSHORT);
    case SQL_C_USHORT:   return m_data.m_dataUSHORT;
  //case SQL_C_BOOKMARK: // fall through
    case SQL_C_LONG:     // fall through
    case SQL_C_SLONG:    return SLongToUShort(m_data.m_dataLONG);
    case SQL_C_ULONG:    return ULongToUShort(m_data.m_dataULONG);
    case SQL_C_FLOAT:    return FloatToUShort(m_data.m_dataFLOAT);
    case SQL_C_DOUBLE:   return DoubleToUShort(m_data.m_dataDOUBLE);
    case SQL_C_BIT:      return (unsigned short)m_data.m_dataBIT;
    case SQL_C_TINYINT:  // fall through
    case SQL_C_STINYINT: return TinyIntToUShort(m_data.m_dataTINYINT);
    case SQL_C_UTINYINT: return (unsigned short)m_data.m_dataUTINYINT;
    case SQL_C_SBIGINT:  return BIGINTToUShort(m_data.m_dataSBIGINT);
    case SQL_C_UBIGINT:  return UBIGINTToUShort(m_data.m_dataUBIGINT);
    case SQL_C_NUMERIC:  { CString num;
                           GetAsString(num); 
                           return DoubleToUShort(atof(num));
                         }
                         break;

    case SQL_C_BINARY:                    // Fall through
  //case SQL_C_VARBOOKMARK:               // Fall through
    case SQL_C_GUID:                      // Fall through
    case SQL_C_DATE:                      // Fall through
    case SQL_C_TYPE_DATE:                 // Fall through
    case SQL_C_TIME:                      // Fall through
    case SQL_C_TYPE_TIME:                 // Fall through
    case SQL_C_TIMESTAMP:                 // Fall through
    case SQL_C_TYPE_TIMESTAMP:            // Fall through
    case SQL_C_INTERVAL_YEAR:             // Fall through
    case SQL_C_INTERVAL_MONTH:            // Fall through
    case SQL_C_INTERVAL_DAY:              // Fall through
    case SQL_C_INTERVAL_HOUR:             // Fall through
    case SQL_C_INTERVAL_MINUTE:           // Fall through
    case SQL_C_INTERVAL_SECOND:           // Fall through
    case SQL_C_INTERVAL_YEAR_TO_MONTH:    // Fall through
    case SQL_C_INTERVAL_DAY_TO_HOUR:      // Fall through
    case SQL_C_INTERVAL_DAY_TO_MINUTE:    // Fall through
    case SQL_C_INTERVAL_DAY_TO_SECOND:    // Fall through
    case SQL_C_INTERVAL_HOUR_TO_MINUTE:   // Fall through
    case SQL_C_INTERVAL_HOUR_TO_SECOND:   // Fall through
    case SQL_C_INTERVAL_MINUTE_TO_SECOND: // Fall through
    default:                              break;
  }
  ThrowErrorDatatype(SQL_C_USHORT);
  // We never come here, but this is to prevent 
  // Warning C4715 not all control paths return a value
  // In various versions of the MSC++ compiler
  return NULL;
}

long
SQLVariant::GetAsSLong()
{
  switch(m_datatype)
  {
    case SQL_C_CHAR:     return (long)atoi(m_data.m_dataCHAR);
    case SQL_C_SHORT:    return (long)m_data.m_dataSHORT;
    case SQL_C_USHORT:   return (long)m_data.m_dataUSHORT;
  //case SQL_C_BOOKMARK: // fall through
    case SQL_C_LONG:     // fall through
    case SQL_C_SLONG:    return m_data.m_dataLONG;
    case SQL_C_ULONG:    return ULongToLong(m_data.m_dataULONG);
    case SQL_C_FLOAT:    return FloatToLong(m_data.m_dataFLOAT);
    case SQL_C_DOUBLE:   return DoubleToLong(m_data.m_dataDOUBLE);
    case SQL_C_BIT:      return (long)m_data.m_dataBIT;
    case SQL_C_TINYINT:  // fall through
    case SQL_C_STINYINT: return (long)m_data.m_dataTINYINT;
    case SQL_C_UTINYINT: return (long)m_data.m_dataUTINYINT;
    case SQL_C_SBIGINT:  return BIGINTToLong(m_data.m_dataSBIGINT);
    case SQL_C_UBIGINT:  return UBIGINTToLong(m_data.m_dataUBIGINT);
    case SQL_C_NUMERIC:  {
                           CString num; 
                           GetAsString(num); 
                           return (long)atoi(num);
                         }
                         break;
    case SQL_C_BINARY:                    // Fall through
  //case SQL_C_VARBOOKMARK:               // Fall through
    case SQL_C_GUID:                      // Fall through
    case SQL_C_DATE:                      // Fall through
    case SQL_C_TYPE_DATE:                 // Fall through
    case SQL_C_TIME:                      // Fall through
    case SQL_C_TYPE_TIME:                 // Fall through
    case SQL_C_TIMESTAMP:                 // Fall through
    case SQL_C_TYPE_TIMESTAMP:            // Fall through
    case SQL_C_INTERVAL_YEAR:             // Fall through
    case SQL_C_INTERVAL_MONTH:            // Fall through
    case SQL_C_INTERVAL_DAY:              // Fall through
    case SQL_C_INTERVAL_HOUR:             // Fall through
    case SQL_C_INTERVAL_MINUTE:           // Fall through
    case SQL_C_INTERVAL_SECOND:           // Fall through
    case SQL_C_INTERVAL_YEAR_TO_MONTH:    // Fall through
    case SQL_C_INTERVAL_DAY_TO_HOUR:      // Fall through
    case SQL_C_INTERVAL_DAY_TO_MINUTE:    // Fall through
    case SQL_C_INTERVAL_DAY_TO_SECOND:    // Fall through
    case SQL_C_INTERVAL_HOUR_TO_MINUTE:   // Fall through
    case SQL_C_INTERVAL_HOUR_TO_SECOND:   // Fall through
    case SQL_C_INTERVAL_MINUTE_TO_SECOND: // Fall through
    default:                              break;
  }
  ThrowErrorDatatype(SQL_C_SLONG);
  // We never come here, but this is to prevent 
  // Warning C4715 not all control paths return a value
  // In various versions of the MSC++ compiler
  return NULL;
}

unsigned long
SQLVariant::GetAsULong()
{
  switch(m_datatype)
  {
    case SQL_C_CHAR:     return LongToULong(atoi(m_data.m_dataCHAR));
    case SQL_C_SHORT:    return ShortToULong(m_data.m_dataSHORT);
    case SQL_C_USHORT:   return (unsigned long)m_data.m_dataUSHORT;
  //case SQL_C_BOOKMARK: // fall through
    case SQL_C_LONG:     // fall through
    case SQL_C_SLONG:    return LongToULong(m_data.m_dataLONG);
    case SQL_C_ULONG:    return m_data.m_dataULONG;
    case SQL_C_FLOAT:    return FloatToULong(m_data.m_dataFLOAT);
    case SQL_C_DOUBLE:   return DoubleToULong(m_data.m_dataDOUBLE);
    case SQL_C_BIT:      return (unsigned long)m_data.m_dataBIT;
    case SQL_C_TINYINT:  // fall through
    case SQL_C_STINYINT: return TinyIntToULong(m_data.m_dataTINYINT);
    case SQL_C_UTINYINT: return (unsigned long)m_data.m_dataUTINYINT;
    case SQL_C_SBIGINT:  return BIGINTToULong(m_data.m_dataSBIGINT);
    case SQL_C_UBIGINT:  return UBIGINTToULong(m_data.m_dataUBIGINT);
    case SQL_C_NUMERIC:  { CString num;
                           GetAsString(num); 
                           return DoubleToULong(atof(num));
                         }
                         break;

    case SQL_C_BINARY:                    // Fall through
  //case SQL_C_VARBOOKMARK:               // Fall through
    case SQL_C_GUID:                      // Fall through
    case SQL_C_DATE:                      // Fall through
    case SQL_C_TYPE_DATE:                 // Fall through
    case SQL_C_TIME:                      // Fall through
    case SQL_C_TYPE_TIME:                 // Fall through
    case SQL_C_TIMESTAMP:                 // Fall through
    case SQL_C_TYPE_TIMESTAMP:            // Fall through
    case SQL_C_INTERVAL_YEAR:             // Fall through
    case SQL_C_INTERVAL_MONTH:            // Fall through
    case SQL_C_INTERVAL_DAY:              // Fall through
    case SQL_C_INTERVAL_HOUR:             // Fall through
    case SQL_C_INTERVAL_MINUTE:           // Fall through
    case SQL_C_INTERVAL_SECOND:           // Fall through
    case SQL_C_INTERVAL_YEAR_TO_MONTH:    // Fall through
    case SQL_C_INTERVAL_DAY_TO_HOUR:      // Fall through
    case SQL_C_INTERVAL_DAY_TO_MINUTE:    // Fall through
    case SQL_C_INTERVAL_DAY_TO_SECOND:    // Fall through
    case SQL_C_INTERVAL_HOUR_TO_MINUTE:   // Fall through
    case SQL_C_INTERVAL_HOUR_TO_SECOND:   // Fall through
    case SQL_C_INTERVAL_MINUTE_TO_SECOND: // Fall through
    default:                              break;
  }
  ThrowErrorDatatype(SQL_C_ULONG);
  // We never come here, but this is to prevent 
  // Warning C4715 not all control paths return a value
  // In various versions of the MSC++ compiler
  return NULL;
}

float
SQLVariant::GetAsFloat()
{
  switch(m_datatype)
  {
    case SQL_C_CHAR:     return DoubleToFloat(atof(m_data.m_dataCHAR));
    case SQL_C_SHORT:    return (float)m_data.m_dataSHORT;
    case SQL_C_USHORT:   return (float)m_data.m_dataUSHORT;
  //case SQL_C_BOOKMARK: // fall through
    case SQL_C_LONG:     // fall through
    case SQL_C_SLONG:    return (float)m_data.m_dataLONG;
    case SQL_C_ULONG:    return (float)m_data.m_dataULONG;
    case SQL_C_FLOAT:    return m_data.m_dataFLOAT;
    case SQL_C_DOUBLE:   return DoubleToFloat(m_data.m_dataDOUBLE);
    case SQL_C_BIT:      return (float)m_data.m_dataBIT;
    case SQL_C_TINYINT:  // fall through
    case SQL_C_STINYINT: return (float)m_data.m_dataTINYINT;
    case SQL_C_UTINYINT: return (float)m_data.m_dataUTINYINT;
    case SQL_C_SBIGINT:  return (float)m_data.m_dataSBIGINT;
    case SQL_C_UBIGINT:  return (float)m_data.m_dataUBIGINT;
    case SQL_C_NUMERIC:{ CString num; 
                         GetAsString(num); 
                         return DoubleToFloat(atof(num));
                       }
                       break;
  //case SQL_C_VARBOOKMARK:               // Fall through
    case SQL_C_BINARY:                    // Fall through
    case SQL_C_GUID:                      // Fall through
    case SQL_C_DATE:                      // Fall through
    case SQL_C_TYPE_DATE:                 // Fall through
    case SQL_C_TIME:                      // Fall through
    case SQL_C_TYPE_TIME:                 // Fall through
    case SQL_C_TIMESTAMP:                 // Fall through
    case SQL_C_TYPE_TIMESTAMP:            // Fall through
    case SQL_C_INTERVAL_YEAR:             // Fall through
    case SQL_C_INTERVAL_MONTH:            // Fall through
    case SQL_C_INTERVAL_DAY:              // Fall through
    case SQL_C_INTERVAL_HOUR:             // Fall through
    case SQL_C_INTERVAL_MINUTE:           // Fall through
    case SQL_C_INTERVAL_SECOND:           // Fall through
    case SQL_C_INTERVAL_YEAR_TO_MONTH:    // Fall through
    case SQL_C_INTERVAL_DAY_TO_HOUR:      // Fall through
    case SQL_C_INTERVAL_DAY_TO_MINUTE:    // Fall through
    case SQL_C_INTERVAL_DAY_TO_SECOND:    // Fall through
    case SQL_C_INTERVAL_HOUR_TO_MINUTE:   // Fall through
    case SQL_C_INTERVAL_HOUR_TO_SECOND:   // Fall through
    case SQL_C_INTERVAL_MINUTE_TO_SECOND: // Fall through
    default:                              break;
  }
  ThrowErrorDatatype(SQL_C_FLOAT);
  // We never come here, but this is to prevent 
  // Warning C4715 not all control paths return a value
  // In various versions of the MSC++ compiler
  return NULL;
}

double
SQLVariant::GetAsDouble()
{
  switch(m_datatype)
  {
    case SQL_C_CHAR:     return atof(m_data.m_dataCHAR);
    case SQL_C_SHORT:    return (double)m_data.m_dataSHORT;
    case SQL_C_USHORT:   return (double)m_data.m_dataUSHORT;
  //case SQL_C_BOOKMARK: // fall through
    case SQL_C_LONG:     // fall through
    case SQL_C_SLONG:    return (double)m_data.m_dataLONG;
    case SQL_C_ULONG:    return (double)m_data.m_dataULONG;
    case SQL_C_FLOAT:    return (double)m_data.m_dataFLOAT;
    case SQL_C_DOUBLE:   return m_data.m_dataDOUBLE;
    case SQL_C_BIT:      return (double)m_data.m_dataBIT;
    case SQL_C_TINYINT:  // fall through
    case SQL_C_STINYINT: return (double)m_data.m_dataTINYINT;
    case SQL_C_UTINYINT: return (double)m_data.m_dataUTINYINT;
    case SQL_C_SBIGINT:  return (double)m_data.m_dataSBIGINT;
    case SQL_C_UBIGINT:  return (double)m_data.m_dataUBIGINT;
    case SQL_C_NUMERIC:  { CString num;
                           GetAsString(num); 
                           return atof(num);
                         }
                         break;
    case SQL_C_BINARY:                    // Fall through
  //case SQL_C_VARBOOKMARK:               // Fall through
    case SQL_C_GUID:                      // Fall through
    case SQL_C_DATE:                      // Fall through
    case SQL_C_TYPE_DATE:                 // Fall through
    case SQL_C_TIME:                      // Fall through
    case SQL_C_TYPE_TIME:                 // Fall through
    case SQL_C_TIMESTAMP:                 // Fall through
    case SQL_C_TYPE_TIMESTAMP:            // Fall through
    case SQL_C_INTERVAL_YEAR:             // Fall through
    case SQL_C_INTERVAL_MONTH:            // Fall through
    case SQL_C_INTERVAL_DAY:              // Fall through
    case SQL_C_INTERVAL_HOUR:             // Fall through
    case SQL_C_INTERVAL_MINUTE:           // Fall through
    case SQL_C_INTERVAL_SECOND:           // Fall through
    case SQL_C_INTERVAL_YEAR_TO_MONTH:    // Fall through
    case SQL_C_INTERVAL_DAY_TO_HOUR:      // Fall through
    case SQL_C_INTERVAL_DAY_TO_MINUTE:    // Fall through
    case SQL_C_INTERVAL_DAY_TO_SECOND:    // Fall through
    case SQL_C_INTERVAL_HOUR_TO_MINUTE:   // Fall through
    case SQL_C_INTERVAL_HOUR_TO_SECOND:   // Fall through
    case SQL_C_INTERVAL_MINUTE_TO_SECOND: // Fall through
    default:                              break;
  }
  ThrowErrorDatatype(SQL_C_DOUBLE);
  // We never come here, but this is to prevent 
  // Warning C4715 not all control paths return a value
  // In various versions of the MSC++ compiler
  return NULL;
}

char
SQLVariant::GetAsBit()
{
  switch(m_datatype)
  {
    case SQL_C_CHAR:     return m_data.m_dataCHAR[0]  != 0;
    case SQL_C_SHORT:    return m_data.m_dataSHORT    != 0;
    case SQL_C_USHORT:   return m_data.m_dataUSHORT   != 0;
  //case SQL_C_BOOKMARK: // fall through
    case SQL_C_LONG:     // fall through
    case SQL_C_SLONG:    return m_data.m_dataLONG     != 0;
    case SQL_C_ULONG:    return m_data.m_dataULONG    != 0;
    case SQL_C_FLOAT:    return m_data.m_dataFLOAT    != 0.0;
    case SQL_C_DOUBLE:   return m_data.m_dataDOUBLE   != 0.0;
    case SQL_C_BIT:      return m_data.m_dataBIT;
    case SQL_C_TINYINT:  // fall through
    case SQL_C_STINYINT: return m_data.m_dataTINYINT  != 0;
    case SQL_C_UTINYINT: return m_data.m_dataUTINYINT != 0;
    case SQL_C_SBIGINT:  return m_data.m_dataSBIGINT  != 0L;
    case SQL_C_UBIGINT:  return m_data.m_dataUBIGINT  != 0L;
    case SQL_C_NUMERIC:  { CString num;
                           GetAsString(num); 
                           return atof(num) != 0.0;
                         }
                         break;

    case SQL_C_BINARY:                    // Fall through
  //case SQL_C_VARBOOKMARK:               // Fall through
    case SQL_C_GUID:                      // Fall through
    case SQL_C_DATE:                      // Fall through
    case SQL_C_TYPE_DATE:                 // Fall through
    case SQL_C_TIME:                      // Fall through
    case SQL_C_TYPE_TIME:                 // Fall through
    case SQL_C_TIMESTAMP:                 // Fall through
    case SQL_C_TYPE_TIMESTAMP:            // Fall through
    case SQL_C_INTERVAL_YEAR:             // Fall through
    case SQL_C_INTERVAL_MONTH:            // Fall through
    case SQL_C_INTERVAL_DAY:              // Fall through
    case SQL_C_INTERVAL_HOUR:             // Fall through
    case SQL_C_INTERVAL_MINUTE:           // Fall through
    case SQL_C_INTERVAL_SECOND:           // Fall through
    case SQL_C_INTERVAL_YEAR_TO_MONTH:    // Fall through
    case SQL_C_INTERVAL_DAY_TO_HOUR:      // Fall through
    case SQL_C_INTERVAL_DAY_TO_MINUTE:    // Fall through
    case SQL_C_INTERVAL_DAY_TO_SECOND:    // Fall through
    case SQL_C_INTERVAL_HOUR_TO_MINUTE:   // Fall through
    case SQL_C_INTERVAL_HOUR_TO_SECOND:   // Fall through
    case SQL_C_INTERVAL_MINUTE_TO_SECOND: // Fall through
    default:                              break;
  }
  ThrowErrorDatatype(SQL_C_BIT);
  // We never come here, but this is to prevent 
  // Warning C4715 not all control paths return a value
  // In various versions of the MSC++ compiler
  return NULL;
}

char
SQLVariant::GetAsSTinyInt()
{
  switch(m_datatype)
  {
    case SQL_C_CHAR:     return SLongToTinyInt(atoi(m_data.m_dataCHAR));
    case SQL_C_SHORT:    return ShortToTinyInt(m_data.m_dataSHORT);
    case SQL_C_USHORT:   return UShortToTinyInt(m_data.m_dataUSHORT);
  //case SQL_C_BOOKMARK: // fall through
    case SQL_C_LONG:     // fall through
    case SQL_C_SLONG:    return SLongToTinyInt(m_data.m_dataLONG);
    case SQL_C_ULONG:    return ULongToTinyInt(m_data.m_dataULONG);
    case SQL_C_FLOAT:    return FloatToTinyInt(m_data.m_dataFLOAT);
    case SQL_C_DOUBLE:   return DoubleToTinyInt(m_data.m_dataDOUBLE);
    case SQL_C_BIT:      return m_data.m_dataBIT;
    case SQL_C_TINYINT:  // fall through
    case SQL_C_STINYINT: return m_data.m_dataTINYINT;
    case SQL_C_UTINYINT: return UTinyIntToTinyInt(m_data.m_dataUTINYINT);
    case SQL_C_SBIGINT:  return SBIGINTToTinyInt(m_data.m_dataSBIGINT);
    case SQL_C_UBIGINT:  return UBIGINTToTinyInt(m_data.m_dataUBIGINT);
    case SQL_C_NUMERIC:  { CString num;
                           GetAsString(num); 
                           return DoubleToTinyInt(atof(num));
                         }
                         break;

    case SQL_C_BINARY:                    // Fall through
  //case SQL_C_VARBOOKMARK:               // Fall through
    case SQL_C_GUID:                      // Fall through
    case SQL_C_DATE:                      // Fall through
    case SQL_C_TYPE_DATE:                 // Fall through
    case SQL_C_TIME:                      // Fall through
    case SQL_C_TYPE_TIME:                 // Fall through
    case SQL_C_TIMESTAMP:                 // Fall through
    case SQL_C_TYPE_TIMESTAMP:            // Fall through
    case SQL_C_INTERVAL_YEAR:             // Fall through
    case SQL_C_INTERVAL_MONTH:            // Fall through
    case SQL_C_INTERVAL_DAY:              // Fall through
    case SQL_C_INTERVAL_HOUR:             // Fall through
    case SQL_C_INTERVAL_MINUTE:           // Fall through
    case SQL_C_INTERVAL_SECOND:           // Fall through
    case SQL_C_INTERVAL_YEAR_TO_MONTH:    // Fall through
    case SQL_C_INTERVAL_DAY_TO_HOUR:      // Fall through
    case SQL_C_INTERVAL_DAY_TO_MINUTE:    // Fall through
    case SQL_C_INTERVAL_DAY_TO_SECOND:    // Fall through
    case SQL_C_INTERVAL_HOUR_TO_MINUTE:   // Fall through
    case SQL_C_INTERVAL_HOUR_TO_SECOND:   // Fall through
    case SQL_C_INTERVAL_MINUTE_TO_SECOND: // Fall through
    default:                              break;
  }
  ThrowErrorDatatype(SQL_C_STINYINT);
  // We never come here, but this is to prevent 
  // Warning C4715 not all control paths return a value
  // In various versions of the MSC++ compiler
  return NULL;
}

unsigned char
SQLVariant::GetAsUTinyInt()
{
  switch(m_datatype)
  {
    case SQL_C_CHAR:     return SLongToUTinyInt(atoi(m_data.m_dataCHAR));
    case SQL_C_SHORT:    return ShortToUTinyInt(m_data.m_dataSHORT);
    case SQL_C_USHORT:   return UShortToUTinyInt(m_data.m_dataUSHORT);
  //case SQL_C_BOOKMARK: // fall through
    case SQL_C_LONG:     // fall through
    case SQL_C_SLONG:    return SLongToUTinyInt(m_data.m_dataLONG);
    case SQL_C_ULONG:    return ULongToUTinyInt(m_data.m_dataULONG);
    case SQL_C_FLOAT:    return FloatToUTinyInt(m_data.m_dataFLOAT);
    case SQL_C_DOUBLE:   return DoubleToUTinyInt(m_data.m_dataDOUBLE);
    case SQL_C_BIT:      return (unsigned char) m_data.m_dataBIT;
    case SQL_C_TINYINT:  // fall through
    case SQL_C_STINYINT: return TinyIntToUTinyInt(m_data.m_dataTINYINT);
    case SQL_C_UTINYINT: return m_data.m_dataUTINYINT;
    case SQL_C_SBIGINT:  return SBIGINTToUTinyInt(m_data.m_dataSBIGINT);
    case SQL_C_UBIGINT:  return UBIGINTToUTinyInt(m_data.m_dataUBIGINT);
    case SQL_C_NUMERIC:  { CString num;
                           GetAsString(num); 
                           return DoubleToUTinyInt(atof(num));
                         }
                         break;
    case SQL_C_BINARY:                    // Fall through
  //case SQL_C_VARBOOKMARK:               // Fall through
    case SQL_C_GUID:                      // Fall through
    case SQL_C_DATE:                      // Fall through
    case SQL_C_TYPE_DATE:                 // Fall through
    case SQL_C_TIME:                      // Fall through
    case SQL_C_TYPE_TIME:                 // Fall through
    case SQL_C_TIMESTAMP:                 // Fall through
    case SQL_C_TYPE_TIMESTAMP:            // Fall through
    case SQL_C_INTERVAL_YEAR:             // Fall through
    case SQL_C_INTERVAL_MONTH:            // Fall through
    case SQL_C_INTERVAL_DAY:              // Fall through
    case SQL_C_INTERVAL_HOUR:             // Fall through
    case SQL_C_INTERVAL_MINUTE:           // Fall through
    case SQL_C_INTERVAL_SECOND:           // Fall through
    case SQL_C_INTERVAL_YEAR_TO_MONTH:    // Fall through
    case SQL_C_INTERVAL_DAY_TO_HOUR:      // Fall through
    case SQL_C_INTERVAL_DAY_TO_MINUTE:    // Fall through
    case SQL_C_INTERVAL_DAY_TO_SECOND:    // Fall through
    case SQL_C_INTERVAL_HOUR_TO_MINUTE:   // Fall through
    case SQL_C_INTERVAL_HOUR_TO_SECOND:   // Fall through
    case SQL_C_INTERVAL_MINUTE_TO_SECOND: // Fall through
    default:                              break;
  }
  ThrowErrorDatatype(SQL_C_UTINYINT);
  // We never come here, but this is to prevent 
  // Warning C4715 not all control paths return a value
  // In various versions of the MSC++ compiler
  return NULL;
}

SQLBIGINT
SQLVariant::GetAsSBigInt()
{
  switch(m_datatype)
  {
    case SQL_C_CHAR:     return (SQLBIGINT)atoi(m_data.m_dataCHAR);
    case SQL_C_SHORT:    return (SQLBIGINT)m_data.m_dataSHORT;
    case SQL_C_USHORT:   return (SQLBIGINT)m_data.m_dataUSHORT;
  //case SQL_C_BOOKMARK: // fall through
    case SQL_C_LONG:     // fall through
    case SQL_C_SLONG:    return (SQLBIGINT)m_data.m_dataLONG;
    case SQL_C_ULONG:    return (SQLBIGINT)(m_data.m_dataULONG);
    case SQL_C_FLOAT:    return FloatToBIGINT(m_data.m_dataFLOAT);
    case SQL_C_DOUBLE:   return DoubleToBIGINT(m_data.m_dataDOUBLE);
    case SQL_C_BIT:      return (SQLBIGINT)m_data.m_dataBIT;
    case SQL_C_TINYINT:  // fall through
    case SQL_C_STINYINT: return (SQLBIGINT)m_data.m_dataTINYINT;
    case SQL_C_UTINYINT: return (SQLBIGINT)m_data.m_dataUTINYINT;
    case SQL_C_SBIGINT:  return m_data.m_dataSBIGINT;
    case SQL_C_UBIGINT:  return UBIGINTToBIGINT(m_data.m_dataUBIGINT);
    case SQL_C_NUMERIC:  { CString num;
                           GetAsString(num); 
                           return DoubleToBIGINT(atof(num));
                         }
                         break;
    case SQL_C_BINARY:                    // Fall through
  //case SQL_C_VARBOOKMARK:               // Fall through
    case SQL_C_GUID:                      // Fall through
    case SQL_C_DATE:                      // Fall through
    case SQL_C_TYPE_DATE:                 // Fall through
    case SQL_C_TIME:                      // Fall through
    case SQL_C_TYPE_TIME:                 // Fall through
    case SQL_C_TIMESTAMP:                 // Fall through
    case SQL_C_TYPE_TIMESTAMP:            // Fall through
    case SQL_C_INTERVAL_YEAR:             // Fall through
    case SQL_C_INTERVAL_MONTH:            // Fall through
    case SQL_C_INTERVAL_DAY:              // Fall through
    case SQL_C_INTERVAL_HOUR:             // Fall through
    case SQL_C_INTERVAL_MINUTE:           // Fall through
    case SQL_C_INTERVAL_SECOND:           // Fall through
    case SQL_C_INTERVAL_YEAR_TO_MONTH:    // Fall through
    case SQL_C_INTERVAL_DAY_TO_HOUR:      // Fall through
    case SQL_C_INTERVAL_DAY_TO_MINUTE:    // Fall through
    case SQL_C_INTERVAL_DAY_TO_SECOND:    // Fall through
    case SQL_C_INTERVAL_HOUR_TO_MINUTE:   // Fall through
    case SQL_C_INTERVAL_HOUR_TO_SECOND:   // Fall through
    case SQL_C_INTERVAL_MINUTE_TO_SECOND: // Fall through
    default:                              break;
  }
  ThrowErrorDatatype(SQL_C_SBIGINT);
  // We never come here, but this is to prevent 
  // Warning C4715 not all control paths return a value
  // In various versions of the MSC++ compiler
  return NULL;
}

SQLUBIGINT
SQLVariant::GetAsUBigInt()
{
  switch(m_datatype)
  {
    case SQL_C_CHAR:     return LongToUBIGINT(atoi(m_data.m_dataCHAR));
    case SQL_C_SHORT:    return ShortToUBIGINT(m_data.m_dataSHORT);
    case SQL_C_USHORT:   return (SQLUBIGINT)m_data.m_dataUSHORT;
  //case SQL_C_BOOKMARK: // fall through
    case SQL_C_LONG:     // fall through
    case SQL_C_SLONG:    return LongToUBIGINT(m_data.m_dataLONG);
    case SQL_C_ULONG:    return (SQLUBIGINT)(m_data.m_dataULONG);
    case SQL_C_FLOAT:    return FloatToUBIGINT(m_data.m_dataFLOAT);
    case SQL_C_DOUBLE:   return DoubleToUBIGINT(m_data.m_dataDOUBLE);
    case SQL_C_BIT:      return (SQLUBIGINT)m_data.m_dataBIT;
    case SQL_C_TINYINT:  // fall through
    case SQL_C_STINYINT: return TinyIntToUBIGINT(m_data.m_dataTINYINT);
    case SQL_C_UTINYINT: return (SQLUBIGINT)m_data.m_dataUTINYINT;
    case SQL_C_SBIGINT:  return SBIGINTToUBIGINT(m_data.m_dataSBIGINT);
    case SQL_C_UBIGINT:  return m_data.m_dataUBIGINT;
    case SQL_C_NUMERIC:  { CString num;
                           GetAsString(num); 
                           return DoubleToUBIGINT(atof(num));
                         }
                         break;
    case SQL_C_BINARY:                    // Fall through
  //case SQL_C_VARBOOKMARK:               // Fall through
    case SQL_C_GUID:                      // Fall through
    case SQL_C_DATE:                      // Fall through
    case SQL_C_TYPE_DATE:                 // Fall through
    case SQL_C_TIME:                      // Fall through
    case SQL_C_TYPE_TIME:                 // Fall through
    case SQL_C_TIMESTAMP:                 // Fall through
    case SQL_C_TYPE_TIMESTAMP:            // Fall through
    case SQL_C_INTERVAL_YEAR:             // Fall through
    case SQL_C_INTERVAL_MONTH:            // Fall through
    case SQL_C_INTERVAL_DAY:              // Fall through
    case SQL_C_INTERVAL_HOUR:             // Fall through
    case SQL_C_INTERVAL_MINUTE:           // Fall through
    case SQL_C_INTERVAL_SECOND:           // Fall through
    case SQL_C_INTERVAL_YEAR_TO_MONTH:    // Fall through
    case SQL_C_INTERVAL_DAY_TO_HOUR:      // Fall through
    case SQL_C_INTERVAL_DAY_TO_MINUTE:    // Fall through
    case SQL_C_INTERVAL_DAY_TO_SECOND:    // Fall through
    case SQL_C_INTERVAL_HOUR_TO_MINUTE:   // Fall through
    case SQL_C_INTERVAL_HOUR_TO_SECOND:   // Fall through
    case SQL_C_INTERVAL_MINUTE_TO_SECOND: // Fall through
    default:                              break;
  }
  ThrowErrorDatatype(SQL_C_UBIGINT);
  // We never come here, but this is to prevent 
  // Warning C4715 not all control paths return a value
  // In various versions of the MSC++ compiler
  return NULL; 
}

SQL_NUMERIC_STRUCT*
SQLVariant::GetAsNumeric()
{
  if(m_datatype == SQL_C_NUMERIC)
  {
    return &m_data.m_dataNUMERIC;
  }
  // Sometimes we come her unexpectedley in various programs
  // This is a non-multi-treaded solution as an after-thought
  // IT IS NOT SAFE. REWRITE YOUR PROGRAM!!
  TRACE("ALARM: Rewrite your program. Use 'GetAs<cardinal>' instead\n");
  CString number;
  static SQL_NUMERIC_STRUCT val;
  switch(m_datatype)
  {
    case SQL_C_SHORT:     // Fall through
    case SQL_C_USHORT:    // Fall through
    case SQL_C_LONG:      // Fall through
    case SQL_C_SLONG:     // Fall through
    case SQL_C_ULONG:     // Fall through
    case SQL_C_FLOAT:     // Fall through
    case SQL_C_DOUBLE:    // Fall through
    case SQL_C_BIT:       // Fall through
    case SQL_C_TINYINT:   // Fall through
    case SQL_C_STINYINT:  // Fall through
    case SQL_C_UTINYINT:  // Fall through
    case SQL_C_SBIGINT:   // Fall through
    case SQL_C_UBIGINT:   GetAsString(number);
                          StringToNumeric(number,&val);
                          return &val;
  }
  // Other datatypes cannot convert
  ThrowErrorDatatype(SQL_C_NUMERIC);
  return NULL;
}

SQLGUID*
SQLVariant::GetAsGUID()
{
  if(m_datatype == SQL_C_GUID)
  {
    return &m_data.m_dataGUID;
  }
  ThrowErrorDatatype(SQL_C_GUID);
  return NULL;
}

DATE_STRUCT*
SQLVariant::GetAsDate()
{
  if(m_datatype == SQL_C_DATE || m_datatype == SQL_C_TYPE_DATE)
  {
    return &m_data.m_dataDATE;
  }
  if(m_datatype == SQL_C_TIMESTAMP || m_datatype == SQL_C_TYPE_TIMESTAMP)
  {
    return (DATE_STRUCT *) &m_data.m_dataTIMESTAMP;
  }
  if(m_datatype == SQL_C_CHAR)
  {
    // Try  conversion from char
    char buffer[20];
    strcpy_s(buffer,20,m_data.m_dataCHAR);
    if(SetData(SQL_C_DATE,buffer))
    {
      return &m_data.m_dataDATE;
    }
  }
  ThrowErrorDatatype(SQL_C_DATE);
  return NULL;
}

TIME_STRUCT*
SQLVariant::GetAsTime()
{
  if(m_datatype == SQL_C_TIME || m_datatype == SQL_C_TYPE_TIME)
  {
    return &m_data.m_dataTIME;
  }
  if(m_datatype == SQL_C_TIMESTAMP || m_datatype == SQL_C_TYPE_TIMESTAMP)
  {
    return (TIME_STRUCT*)&m_data.m_dataTIMESTAMP.hour;
  }
  if(m_datatype == SQL_C_CHAR)
  {
    char buffer[20];
    strcpy_s(buffer,20,m_data.m_dataCHAR);
    if(SetData(SQL_C_TIME,buffer))
    {
      return &m_data.m_dataTIME;
    }
  }
  ThrowErrorDatatype(SQL_C_TIME);
  return NULL;
}

TIMESTAMP_STRUCT*
SQLVariant::GetAsTimestamp()
{
  if(m_datatype == SQL_C_TIMESTAMP || m_datatype == SQL_C_TYPE_TIMESTAMP)
  {
    return &m_data.m_dataTIMESTAMP;
  }
  if(m_datatype == SQL_C_DATE || m_datatype == SQL_C_TYPE_DATE)
  {
    m_data.m_dataTIMESTAMP.hour     = 0;
    m_data.m_dataTIMESTAMP.minute   = 0;
    m_data.m_dataTIMESTAMP.second   = 0;
    m_data.m_dataTIMESTAMP.fraction = 0;
    return &m_data.m_dataTIMESTAMP;
  }
  if(m_datatype == SQL_C_CHAR)
  {
    char buffer[30];
    strcpy_s(buffer,30,m_data.m_dataCHAR);
    if(SetData(SQL_C_TIMESTAMP,buffer))
    {
      return &m_data.m_dataTIMESTAMP;
    }
  }
  ThrowErrorDatatype(SQL_C_TIMESTAMP);
  return NULL;
}

CString
SQLVariant::GetAsEuropeanTimestamp()
{
  CString result;
  if(m_datatype == SQL_C_TIMESTAMP ||
     m_datatype == SQL_C_TYPE_TIMESTAMP)
  {
    result.Format("%02.2d-%02.2d-%04.4d %02.2d:%02.2d:%02.2d"
                 ,m_data.m_dataTIMESTAMP.day
                 ,m_data.m_dataTIMESTAMP.month
                 ,m_data.m_dataTIMESTAMP.year
                 ,m_data.m_dataTIMESTAMP.hour
                 ,m_data.m_dataTIMESTAMP.minute
                 ,m_data.m_dataTIMESTAMP.second);
    if(m_data.m_dataTIMESTAMP.fraction)
    {
      // Nano-second resulution
      CString frac;
      frac.Format("%0.6f",(double)m_data.m_dataTIMESTAMP.fraction / 1000000000.0);
      result += frac.Mid(1);
    }
  }
  else if(m_datatype == SQL_C_DATE ||
          m_datatype == SQL_C_TYPE_DATE)
  {
    result.Format("%02.2d-%02.2d-%04.4d"
                 ,m_data.m_dataDATE.day
                 ,m_data.m_dataDATE.month
                 ,m_data.m_dataDATE.year);
  }
  else if(m_datatype == SQL_C_TIME ||
          m_datatype == SQL_C_TYPE_TIME)
  {
    result.Format("%02.2d:%02.2d:%02.2d"
                 ,m_data.m_dataTIME.hour
                 ,m_data.m_dataTIME.minute
                 ,m_data.m_dataTIME.second);
  }
  return result;
}

SQL_INTERVAL_STRUCT*
SQLVariant::GetAsInterval()
{
  if(m_datatype <= SQL_C_INTERVAL_YEAR &&
     m_datatype >= SQL_C_INTERVAL_MINUTE_TO_SECOND)
  {
    return &m_data.m_dataINTERVAL;
  }
  ThrowErrorDatatype(SQL_C_INTERVAL_DAY_TO_SECOND);
  return NULL;
}

SQLDate
SQLVariant::GetAsSQLDate()
{
  if(IsNULL())
  {
    return SQLDate();
  }
  DATE_STRUCT* date = GetAsDate();
  return SQLDate(date->day,date->month,date->year);
}

SQLTime
SQLVariant::GetAsSQLTime()
{
  if(IsNULL())
  {
    return SQLTime();
  }
  TIME_STRUCT* time = GetAsTime();
  return SQLTime(time->hour,time->minute,time->second);
}

SQLTimestamp
SQLVariant::GetAsSQLTimestamp()
{
  if(IsNULL())
  {
    return SQLTimestamp();
  }
  TIMESTAMP_STRUCT* stamp = GetAsTimestamp();
  return SQLTimestamp(stamp->year,stamp->month,stamp->day
                     ,stamp->hour,stamp->minute,stamp->second
                     ,stamp->fraction);
}

int
SQLVariant::GetFraction()
{
  switch(m_datatype)
  {
    case SQL_C_TIMESTAMP:       // Fall through
    case SQL_C_TYPE_TIMESTAMP:  return 6;
    case SQL_C_NUMERIC:         return m_data.m_dataNUMERIC.precision;
  }
  return 0;
}

// Some databases (Oracle) need to know the size of the data member
// for an AT_EXEC operation beforehand in the indicator
void
SQLVariant::SetSizeIndicator(bool p_realSize)
{
  int size = GetDataSize();
  if(size > 0 && p_realSize)
  {
    // Special ODBC macro to set the data size
    m_indicator = SQL_LEN_DATA_AT_EXEC(size);
    m_useAtExec = true;
  }
  else
  {
    // Simply the AT_EXEC indicator
    m_indicator = SQL_DATA_AT_EXEC;
    m_useAtExec = true;
  }
}

bool    
SQLVariant::SetData(int p_type,const char* p_data)
{
  int    year,month,day;
  int    hour,min,sec;
  double seconds;
  int    scannum;
  bool   retval = true;

  // Reset data member
  ResetDataType(0);
  // Record new type
  m_datatype        = p_type;
  m_sqlDatatype     = p_type;
  m_binaryLength    = 0;
  m_binaryPieceSize = 0;
  m_useAtExec       = false;
  m_indicator       = 0;

  // Trim leading whitespace and calculate the size
  if(p_type != SQL_C_CHAR)
  {
    while(isspace(p_data[0]))
  {
    // Trim the leading spaces
      // So we can translate whatever is in the string
    ++p_data;
  }
  }
  size_t dataLen = strlen(p_data);

  // FIND OUT ABOUT THE INTERVAL SIGN
  if(p_type <= SQL_C_INTERVAL_MINUTE_TO_SECOND &&
     p_type >= SQL_C_INTERVAL_YEAR)
  {
    ZeroMemory(&m_data.m_dataINTERVAL,sizeof(SQL_INTERVAL_STRUCT));
    if(dataLen > 0 && p_data[0] == '-')
    {
      m_data.m_dataINTERVAL.interval_sign = TRUE;
      ++p_data;
    }
  }

  // TEST VOOR LOGICAL NULL
  if(dataLen == 0)
  {
    m_indicator = SQL_NULL_DATA;
  }

  // Record data of the type
  switch(p_type)
  {
    case SQL_C_CHAR:                      m_binaryLength = (int)(dataLen + 1);
                                          m_data.m_dataCHAR = (char*) malloc(m_binaryLength);
                                          strcpy_s(m_data.m_dataCHAR,m_binaryLength,p_data);
                                          m_indicator = SQL_NTS;
                                          break;
  //case SQL_C_VARBOOKMARK:               // Fall through
    case SQL_C_BINARY:                    m_indicator    =
                                          m_binaryLength = (int)(dataLen / 2);
                                          m_data.m_dataBINARY = (unsigned char*) malloc(2 + m_binaryLength);
                                          StringToBinary(p_data);
                                          break;
    case SQL_C_SHORT:                     // Fall through
    case SQL_C_SSHORT:                    m_data.m_dataSHORT = (short)atoi(p_data);
                                          break;
    case SQL_C_USHORT:                    m_data.m_dataUSHORT = (short)atoi(p_data);
                                          break;
    case SQL_C_LONG:                      // Fall through
    case SQL_C_SLONG:                     m_data.m_dataLONG = (long)atoi(p_data);
                                          break;
  //case SQL_C_BOOKMARK:                  // Fall through
    case SQL_C_ULONG:                     scannum = sscanf(p_data,"%lu",&m_data.m_dataULONG);
                                          if(scannum != 1)
                                          {
                                            m_data.m_dataULONG = NULL;
                                          }
                                          break;
    case SQL_C_FLOAT:                     scannum = sscanf(p_data,"%f",&m_data.m_dataFLOAT);
                                          if(scannum != 1)
                                          {
                                            m_data.m_dataFLOAT = NULL;
                                          }
                                          break;
    case SQL_C_DOUBLE:                    scannum = sscanf(p_data,"%lf",&m_data.m_dataDOUBLE);
                                          if(scannum != 1)
                                          {
                                            m_data.m_dataDOUBLE = NULL;
                                          }
                                          break;
    case SQL_C_BIT:                       m_data.m_dataBIT = (atoi(p_data) == 1);
                                          break;
    case SQL_C_TINYINT:                   // Fall through
    case SQL_C_STINYINT:                  m_data.m_dataTINYINT = (char)atoi(p_data);
                                          break;
    case SQL_C_UTINYINT:                  m_data.m_dataTINYINT = (unsigned char)atoi(p_data);
                                          break;
    case SQL_C_SBIGINT:                   scannum = sscanf(p_data,"%I64d",&m_data.m_dataSBIGINT);
                                          if(scannum != 1)
                                          {
                                            m_data.m_dataSBIGINT = NULL;
                                          }
                                          break;
    case SQL_C_UBIGINT:                   scannum = sscanf(p_data,"%I64u",&m_data.m_dataUBIGINT);
                                          if(scannum != 1)
                                          {
                                            m_data.m_dataUBIGINT = NULL;
                                          }
                                          break;
    case SQL_C_NUMERIC:                   StringToNumeric(p_data,&m_data.m_dataNUMERIC);
                                          break;
    case SQL_C_GUID:                      //aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee 
                                          scannum = scanf("{%lX-%hX-%hX-%2hhX%2hhX-%2hhX%2hhX%2hhX%2hhX%2hhX%2hhX}"
                                                          ,&m_data.m_dataGUID.Data1 
                                                          ,&m_data.m_dataGUID.Data2 
                                                          ,&m_data.m_dataGUID.Data3 
                                                          ,&m_data.m_dataGUID.Data4[0]
                                                          ,&m_data.m_dataGUID.Data4[1]
                                                          ,&m_data.m_dataGUID.Data4[2]
                                                          ,&m_data.m_dataGUID.Data4[3]
                                                          ,&m_data.m_dataGUID.Data4[4]
                                                          ,&m_data.m_dataGUID.Data4[5]
                                                          ,&m_data.m_dataGUID.Data4[6]
                                                          ,&m_data.m_dataGUID.Data4[7]);
                                          if(scannum != 11)
                                          {
                                            memset(&m_data.m_dataGUID,0,sizeof(SQLGUID));
                                            retval = false;
                                          }
                                          break;
    case SQL_C_DATE:                      // Fall Through
    case SQL_C_TYPE_DATE:                 scannum = sscanf(p_data,"%d-%d-%d",&day,&month,&year);
                                          if(scannum == 3)
                                          {
                                            m_data.m_dataDATE.day   = (SQLUSMALLINT) day;
                                            m_data.m_dataDATE.month = (SQLUSMALLINT) month;
                                            m_data.m_dataDATE.year  = (SQLSMALLINT)  year;
                                          }
                                          else
                                          {
                                            retval = false;
                                          }
                                          break;
    case SQL_C_TIME:                      // Fall through
    case SQL_C_TYPE_TIME:                 scannum = sscanf(p_data,"%d:%d:%d" ,&hour,&min,&sec);
                                          if(scannum == 3)
                                          {
                                            m_data.m_dataTIME.hour   = (SQLUSMALLINT) hour;
                                            m_data.m_dataTIME.minute = (SQLUSMALLINT) min;
                                            m_data.m_dataTIME.second = (SQLUSMALLINT) sec;
                                          }
                                          else
                                          {
                                            retval = false;
                                          }
                                          break;
    case SQL_C_TIMESTAMP:                 // Fall through
    case SQL_C_TYPE_TIMESTAMP:            scannum = sscanf(p_data,"%d-%d-%d %d:%d:%lf"
                                                          ,&year,&month,&day
                                                          ,&hour,&min,&seconds);
                                          if(scannum == 6) 
                                          {
                                            m_data.m_dataTIMESTAMP.year   = (SQLSMALLINT)  year;
                                            m_data.m_dataTIMESTAMP.month  = (SQLUSMALLINT) month;
                                            m_data.m_dataTIMESTAMP.day    = (SQLUSMALLINT) day;
                                            m_data.m_dataTIMESTAMP.hour   = (SQLUSMALLINT) hour;
                                            m_data.m_dataTIMESTAMP.minute = (SQLUSMALLINT) min;
                                            // Nanosecond resultion
                                            sec = (int)floor(seconds);
                                            m_data.m_dataTIMESTAMP.second   = (SQLUSMALLINT) sec;
                                            m_data.m_dataTIMESTAMP.fraction = (SQLUINTEGER)(((seconds - (double)sec) * 1000000000.0) + 0.0000005);
                                          }
                                          else
                                          {
                                            retval = false;
                                          }
                                          break;
    case SQL_C_INTERVAL_YEAR:             m_data.m_dataINTERVAL.intval.year_month.year   = atoi(p_data);
                                          m_data.m_dataINTERVAL.interval_type            = SQL_IS_YEAR;
                                          break;
    case SQL_C_INTERVAL_MONTH:            m_data.m_dataINTERVAL.intval.year_month.month  = atoi(p_data);
                                          m_data.m_dataINTERVAL.interval_type            = SQL_IS_MONTH;
                                          break;
    case SQL_C_INTERVAL_DAY:              m_data.m_dataINTERVAL.intval.day_second.day    = atoi(p_data);
                                          m_data.m_dataINTERVAL.interval_type            = SQL_IS_DAY;
                                          break;
    case SQL_C_INTERVAL_HOUR:             m_data.m_dataINTERVAL.intval.day_second.hour   = atoi(p_data);
                                          m_data.m_dataINTERVAL.interval_type            = SQL_IS_HOUR;
                                          break;
    case SQL_C_INTERVAL_MINUTE:           m_data.m_dataINTERVAL.intval.day_second.minute = atoi(p_data);
                                          m_data.m_dataINTERVAL.interval_type            = SQL_IS_MINUTE;
                                          break;
    case SQL_C_INTERVAL_SECOND:           m_data.m_dataINTERVAL.intval.day_second.second = atoi(p_data);
                                          m_data.m_dataINTERVAL.interval_type            = SQL_IS_SECOND;
                                          break;
    case SQL_C_INTERVAL_YEAR_TO_MONTH:    m_data.m_dataINTERVAL.interval_type = SQL_IS_YEAR_TO_MONTH;
                                          scannum = sscanf(p_data,"%d %d",&year,&month);
                                          if(scannum == 2)
                                          {
                                            m_data.m_dataINTERVAL.intval.year_month.year  = year;
                                            m_data.m_dataINTERVAL.intval.year_month.month = month;
                                            
                                          }
                                          else
                                          {
                                            retval = false;
                                          }
                                          break;
    case SQL_C_INTERVAL_DAY_TO_HOUR:      m_data.m_dataINTERVAL.interval_type = SQL_IS_DAY_TO_HOUR;
                                          scannum = sscanf(p_data,"%d %d",&day,&hour);
                                          if(scannum == 2)
                                          {
                                            m_data.m_dataINTERVAL.intval.day_second.day  = day;
                                            m_data.m_dataINTERVAL.intval.day_second.hour = hour;
                                          }
                                          else
                                          {
                                            retval = false;
                                          }
                                          break;
    case SQL_C_INTERVAL_DAY_TO_MINUTE:    m_data.m_dataINTERVAL.interval_type = SQL_IS_DAY_TO_MINUTE;
                                          scannum = sscanf(p_data,"%d %d:%d",&day,&hour,&min);
                                          if(scannum == 3)
                                          {
                                            m_data.m_dataINTERVAL.intval.day_second.day    = day;
                                            m_data.m_dataINTERVAL.intval.day_second.hour   = hour;
                                            m_data.m_dataINTERVAL.intval.day_second.minute = min;
                                          }
                                          else
                                          {
                                            retval = false;
                                          }
                                          break;
    case SQL_C_INTERVAL_DAY_TO_SECOND:    m_data.m_dataINTERVAL.interval_type = SQL_IS_DAY_TO_SECOND;
                                          scannum = sscanf(p_data,"%d %d:%d:%d",&day,&hour,&min,&sec);
                                          if(scannum == 4)
                                          {
                                            m_data.m_dataINTERVAL.intval.day_second.day    = day;
                                            m_data.m_dataINTERVAL.intval.day_second.hour   = hour;
                                            m_data.m_dataINTERVAL.intval.day_second.minute = min;
                                            m_data.m_dataINTERVAL.intval.day_second.second = sec;
                                          }
                                          else
                                          {
                                            retval = false;
                                          }
                                          break;
    case SQL_C_INTERVAL_HOUR_TO_MINUTE:   m_data.m_dataINTERVAL.interval_type = SQL_IS_HOUR_TO_MINUTE;
                                          scannum = sscanf(p_data,"%d:%d",&hour,&min);
                                          if(scannum == 2)
                                          {
                                            m_data.m_dataINTERVAL.intval.day_second.hour   = hour;
                                            m_data.m_dataINTERVAL.intval.day_second.minute = min;
                                          }
                                          else
                                          {
                                            retval = false;
                                          }
                                          break;
    case SQL_C_INTERVAL_HOUR_TO_SECOND:   m_data.m_dataINTERVAL.interval_type = SQL_IS_HOUR_TO_SECOND;
                                          scannum = sscanf(p_data,"%d:%d:%d",&hour,&min,&sec);
                                          if(scannum == 3)
                                          {
                                            m_data.m_dataINTERVAL.intval.day_second.hour   = hour;
                                            m_data.m_dataINTERVAL.intval.day_second.minute = min;
                                            m_data.m_dataINTERVAL.intval.day_second.second = sec;
                                          }
                                          else
                                          {
                                            retval = false;
                                          }
                                          break;
    case SQL_C_INTERVAL_MINUTE_TO_SECOND: m_data.m_dataINTERVAL.interval_type = SQL_IS_MINUTE_TO_SECOND;
                                          scannum = sscanf(p_data,"%d:%d",&min,&sec);
                                          if(scannum == 2)
                                          {
                                            m_data.m_dataINTERVAL.intval.day_second.minute = min;
                                            m_data.m_dataINTERVAL.intval.day_second.second = sec;
                                          }
                                          else
                                          {
                                            retval = false;
                                          }
                                          break;
    default:                              m_indicator = SQL_NULL_DATA;
                                          return false;
  }
  return retval;
}

// Assignment operator for a new SQLVariant
SQLVariant&
SQLVariant::operator=(const SQLVariant& p_origineel)
{
  // Check for assignment to self
  if(this == &p_origineel)
  {
    return *this;
  }

  // Copy the members
  m_datatype        = p_origineel.m_datatype;
  m_sqlDatatype     = p_origineel.m_sqlDatatype;
  m_binaryLength    = p_origineel.m_binaryLength;
  m_binaryPieceSize = p_origineel.m_binaryPieceSize;
  m_columnNumber    = p_origineel.m_columnNumber;
  m_paramType       = p_origineel.m_paramType;
  m_useAtExec       = p_origineel.m_useAtExec;
  m_indicator       = p_origineel.m_indicator;

  // Copy the data
  if(m_datatype == SQL_C_CHAR  || m_datatype == SQL_C_BINARY )
  {
    // Make a new buffer and copy it
    m_data.m_dataBINARY = (unsigned char*) malloc(m_binaryLength + 1);
    memcpy(m_data.m_dataBINARY,p_origineel.m_data.m_dataBINARY,m_binaryLength + 1);
  }
  else
  {
    // Copy the data member
    memcpy(&m_data.m_dataBIT,&p_origineel.m_data.m_dataBIT,sizeof(m_data));
  }
  return *this;
}

// Equality operator for SQLVariant
bool
SQLVariant::operator ==(const SQLVariant& p_right)
{
  if(m_datatype != p_right.m_datatype)
  {
    return false;
  }
  switch(m_datatype)
  {
    case SQL_C_CHAR:      return strncmp(m_data.m_dataCHAR,  p_right.m_data.m_dataCHAR,  m_binaryLength) == 0;
    case SQL_C_BINARY:    return memcmp (m_data.m_dataBINARY,p_right.m_data.m_dataBINARY,m_binaryLength) == 0;
    case SQL_C_SHORT:     // fall through
    case SQL_C_SSHORT:    return (m_data.m_dataSHORT  == p_right.m_data.m_dataSHORT);
    case SQL_C_USHORT:    return (m_data.m_dataUSHORT == p_right.m_data.m_dataUSHORT);
    case SQL_C_LONG:      // fall through
    case SQL_C_SLONG:     return (m_data.m_dataLONG   == p_right.m_data.m_dataLONG);
    case SQL_C_ULONG:     return (m_data.m_dataULONG  == p_right.m_data.m_dataULONG);
    case SQL_C_FLOAT:     return (m_data.m_dataFLOAT  == p_right.m_data.m_dataFLOAT);
    case SQL_C_DOUBLE:    return (m_data.m_dataDOUBLE == p_right.m_data.m_dataDOUBLE);
    case SQL_C_BIT:       return (m_data.m_dataBIT    == p_right.m_data.m_dataBIT);
    case SQL_C_TINYINT:   // fall through
    case SQL_C_STINYINT:  return (m_data.m_dataTINYINT  == p_right.m_data.m_dataTINYINT);
    case SQL_C_UTINYINT:  return (m_data.m_dataUTINYINT == p_right.m_data.m_dataUTINYINT);
    case SQL_C_SBIGINT:   return (m_data.m_dataSBIGINT  == p_right.m_data.m_dataSBIGINT);
    case SQL_C_UBIGINT:   return (m_data.m_dataUBIGINT  == p_right.m_data.m_dataUBIGINT);
    case SQL_C_NUMERIC:   return memcmp(&m_data.m_dataNUMERIC,&p_right.m_data.m_dataNUMERIC,sizeof(SQL_NUMERIC_STRUCT)) == 0;
    case SQL_C_GUID:      return memcmp(&m_data.m_dataGUID,   &p_right.m_data.m_dataGUID,   sizeof(SQLGUID)) == 0;
    case SQL_C_DATE:      // fall through
    case SQL_C_TYPE_DATE: return memcmp(&m_data.m_dataDATE,   &p_right.m_data.m_dataDATE,   sizeof(DATE_STRUCT)) == 0;
    case SQL_C_TIME:      // fall through
    case SQL_C_TYPE_TIME: return memcmp(&m_data.m_dataTIME,   &p_right.m_data.m_dataTIME,   sizeof(TIME_STRUCT)) == 0;
    case SQL_C_TIMESTAMP:                 // fall through
    case SQL_C_TYPE_TIMESTAMP:            return memcmp(&m_data.m_dataTIMESTAMP,&p_right.m_data.m_dataTIMESTAMP, sizeof(TIMESTAMP_STRUCT)) == 0;
    case SQL_C_INTERVAL_YEAR:             // fall through
    case SQL_C_INTERVAL_MONTH:            // fall through
    case SQL_C_INTERVAL_YEAR_TO_MONTH:    // fall through
    case SQL_C_INTERVAL_DAY:              // fall through
    case SQL_C_INTERVAL_DAY_TO_HOUR:      // fall through
    case SQL_C_INTERVAL_DAY_TO_MINUTE:    // fall through
    case SQL_C_INTERVAL_DAY_TO_SECOND:    // fall through
    case SQL_C_INTERVAL_HOUR:             // fall through
    case SQL_C_INTERVAL_HOUR_TO_MINUTE:   // fall through
    case SQL_C_INTERVAL_HOUR_TO_SECOND:   // fall through
    case SQL_C_INTERVAL_MINUTE:           // fall through
    case SQL_C_INTERVAL_MINUTE_TO_SECOND: // fall through
    case SQL_C_INTERVAL_SECOND:           return memcmp(&m_data.m_dataINTERVAL,&p_right.m_data.m_dataINTERVAL,sizeof(SQL_INTERVAL_STRUCT)) == 0;
  }
  return false;
}

//////////////////////////////////////////////////////////////////////////
//
// CONVERSION ROUTINES BEGIN HERE
//
//////////////////////////////////////////////////////////////////////////

// Get a binary buffer in a string
// as hex printed values as Oracle does
bool
SQLVariant::BinaryToString(unsigned char* buffer,int buflen)
{
  static char number[17] = "0123456789ABCDEF";

  *buffer = 0;
  if(m_datatype != SQL_C_BINARY)
  {
    return false;
  }
  if(m_indicator == SQL_NULL_DATA)
  {
    // Returning a NULL is very easy
    return true;
  }
  unsigned char* colPointer = (unsigned char*)m_data.m_dataBINARY;
  SQLLEN dataLen = m_binaryLength;
  if(m_indicator >= 0 && m_indicator < m_binaryLength)
  {
    dataLen = m_indicator;
  }
  while((buflen > 2) && (dataLen > 0))
  {
    int c = *colPointer++;
    *buffer++ = number[c / 16];
    *buffer++ = number[c % 16];
    buflen -= 2;
    --dataLen;
  }
  *buffer = 0;
  return true;
}

// Store a hex string encoded binary 
// back into a binary buffer as Oracle does
bool
SQLVariant::StringToBinary(const char* p_data)
{
  const char* colPointer = p_data;
  int len = (int)strlen(p_data);
  unsigned char* buffer = (unsigned char*)m_data.m_dataBINARY;
  int binlen = m_binaryLength;

  if(m_datatype != SQL_C_BINARY)
  {
    return false;
  }
  int a,b;
  while(len && binlen && *colPointer)
  {
    a = toupper(*colPointer++) - '0';
    b = toupper(*colPointer++) - '0';
    if(a > 9) a -= 7;
    if(b > 9) b -= 7;
    *buffer++ = (unsigned char)((a * 16) + b);
    len -= 2;
    --binlen;
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////
//
//  SQL_NUMERIC_STRUCT and conversions
//
//////////////////////////////////////////////////////////////////////////

void
SQLVariant::StringToNumeric(const char* p_data,SQL_NUMERIC_STRUCT* p_numeric)
{
  int   len = (int) strlen(p_data);
  const char* pos = strchr(p_data,'.');
  int precision = len;
  int scale     = 0;
  if(pos)
  {
    --precision;
    scale = len - (int)(pos - p_data) - 1;
  }
  if(scale > 32)
  {
    // SQL_NUMERIC cannot handle more than 32 digits precision
    // in the fraction portion of the number
    scale = 32;
    *((char *)(pos + scale + 1)) = 0;
  }
  // Must set precision/scale beforehand
  p_numeric->precision = (char)precision;
  p_numeric->scale     = (char)scale;

  int overflow = 0;
  StringToNumericMantissa(p_data,p_numeric,&overflow);
  if(overflow)
  {
    memset(&m_data.m_dataNUMERIC,0,sizeof(SQL_NUMERIC_STRUCT));
  }
}

// Retrieve a SQL_NUMERIC_STRUCT from a string. The requested scale
// and precision are first read from sqlnum, and then updated values
// are written back at the end.
// 
// @param[in] numstr       String representation of number to convert
// @param[in] sqlnum       Destination struct
// @param[in] overflow_ptr Whether or not whole-number overflow occurred.
//                         This indicates failure, and the result of sqlnum
//                         is undefined.
void 
SQLVariant::StringToNumericMantissa(const char*         numstr
                                   ,SQL_NUMERIC_STRUCT* sqlnum
                                   ,int*                overflow_ptr)
{
  /*
  We use 16 bits of each integer to convert the
  current segment of the number leaving extra bits
  to multiply/carry
  */
  int build_up[8], tmp_prec_calc[8];
  /* current segment as integer */
  unsigned int curnum;
  /* current segment digits copied for strtoul() */
  char curdigs[5];
  /* number of digits in current segment */
  int usedig;
  int i;
  int len;
  char *decpt = (char*)strchr(numstr, '.');
  int overflow= 0;
  SQLSCHAR reqscale = sqlnum->scale;
  SQLCHAR  reqprec  = sqlnum->precision;

  memset(&sqlnum->val, 0, sizeof(sqlnum->val));
  memset(build_up, 0, sizeof(build_up));

  /* handle sign */
  sqlnum->sign = !(*numstr == '-');
  if (sqlnum->sign == 0)
  {
    numstr++;
  }
  len= (int) strlen(numstr);
  sqlnum->precision = (SQLCHAR) len;
  sqlnum->scale     = 0;

  /* process digits in groups of <=4 */
  for (i= 0; i < len; i += usedig)
  {
    if (i + 4 < len)
    {
      usedig = 4;
    }
    else
    {
      usedig = len - i;
    }
    // if we have the decimal point, ignore it by setting it to the
    // last char (will be ignored by strtoul)
    if (decpt && decpt >= numstr + i && decpt < numstr + i + usedig)
    {
      usedig = (int) (decpt - (numstr + i) + 1);
      sqlnum->scale = (SQLSCHAR)(len - (i + usedig));
      sqlnum->precision--;
      decpt= NULL;
    }
    /* terminate prematurely if we can't do anything else */
    /*if (overflow && !decpt)
    break;
    else */if (overflow)
      /*continue;*/goto end;
    /* grab just this piece, and convert to int */
    memcpy(curdigs, numstr + i, usedig);
    curdigs[usedig]= 0;
    curnum= strtoul(curdigs, NULL, 10);
    if (curdigs[usedig - 1] == '.')
    {
      sqlnum_scale(build_up, usedig - 1);
    }
    else
    {
      sqlnum_scale(build_up, usedig);
    }
    /* add the current number */
    build_up[0] += curnum;
    sqlnum_carry(build_up);
    if (build_up[7] & ~0xffff)
    {
      overflow = 1;
    }
  }
  /* scale up to SQL_DESC_SCALE */
  if (reqscale > 0 && reqscale > sqlnum->scale)
  {
    while (reqscale > sqlnum->scale)
    {
      sqlnum_scale(build_up, 1);
      sqlnum_carry(build_up);
      sqlnum->scale++;
    }
  }
  /* scale back, truncating decimals */
  else if (reqscale < sqlnum->scale)
  {
    while (reqscale < sqlnum->scale && sqlnum->scale > 0)
    {
      sqlnum_unscale_le(build_up);
      build_up[0] /= 10;
      sqlnum->precision--;
      sqlnum->scale--;
    }
  }
  /* scale back whole numbers while there's no significant digits */
  if (reqscale < 0)
  {
    memcpy(tmp_prec_calc, build_up, sizeof(build_up));
    while (reqscale < sqlnum->scale)
    {
      sqlnum_unscale_le(tmp_prec_calc);
      if (tmp_prec_calc[0] % 10)
      {
        overflow= 1;
        goto end;
      }
      sqlnum_unscale_le(build_up);
      tmp_prec_calc[0] /= 10;
      build_up[0] /= 10;
      sqlnum->precision--;
      sqlnum->scale--;
    }
  }
  /* calculate minimum precision */
  memcpy(tmp_prec_calc, build_up, sizeof(build_up));
  do
  {
    sqlnum_unscale_le(tmp_prec_calc);
    i= tmp_prec_calc[0] % 10;
    tmp_prec_calc[0] /= 10;
    if (i == 0)
    {
      sqlnum->precision--;
    }
  } 
  while (i == 0 && sqlnum->precision > 0);

  /* detect precision overflow */
  if (sqlnum->precision > reqprec)
  {
    overflow= 1;
  }
  else
  {
    sqlnum->precision = reqprec;
  }
  /* compress results into SQL_NUMERIC_STRUCT.val */
  for (i= 0; i < 8; ++i)
  {
    int elem= 2 * i;
    sqlnum->val[elem]   =  build_up[i] & 0xff;
    sqlnum->val[elem+1] = (build_up[i] >> 8) & 0xff;
  }
end:
  if (overflow_ptr)
  {
    *overflow_ptr = overflow;
  }
}

// Scale an int[] representing SQL_C_NUMERIC
// 
// @param[in] ary   Array in little endian form
// @param[in] s     Scale
static void 
sqlnum_scale(int *ary, int s)
{
  /* multiply out all pieces */
  while (s--)
  {
    ary[0] *= 10;
    ary[1] *= 10;
    ary[2] *= 10;
    ary[3] *= 10;
    ary[4] *= 10;
    ary[5] *= 10;
    ary[6] *= 10;
    ary[7] *= 10;
  }
}

// Unscale an int[] representing SQL_C_NUMERIC. This
// leaves the last element (0) with the value of the
// last digit.
// 
// @param[in] ary   Array in little endian form
static void 
sqlnum_unscale_le(int *ary)
{
  int i;
  for (i= 7; i > 0; --i)
  {
    ary[i - 1] += (ary[i] % 10) << 16;
    ary[i] /= 10;
  }
}

// Unscale an int[] representing SQL_C_NUMERIC. This
// leaves the last element (7) with the value of the
// last digit.
// 
// @param[in] ary   Array in big endian form
static void 
sqlnum_unscale_be(int *ary, int start)
{
  int i;
  for (i= start; i < 7; ++i)
  {
    ary[i + 1] += (ary[i] % 10) << 16;
    ary[i] /= 10;
  }
}

// Perform the carry to get all elements below 2^16.
// Should be called right after sqlnum_scale().
// 
// @param[in] ary   Array in little endian form
static void 
sqlnum_carry(int *ary)
{
  int i;
  /* carry over rest of structure */
  for (i= 0; i < 7; ++i)
  {
    ary[i+1] += ary[i] >> 16;
    ary[i] &= 0xffff;
  }
}

// AND BACK TO A STRING

void
SQLVariant::NumericToString(SQL_NUMERIC_STRUCT* p_numeric,CString& p_string)
{
  SQLCHAR  buffer[SQLNUM_MAXLEN+1];
  SQLCHAR* numbegin = NULL;
  int truncate = 0;
  int scale = p_numeric->scale;
  scale = (scale == -127) ? 16 : scale;
  memset(buffer,0,SQLNUM_MAXLEN+1);
  NumericToStringMantissa(p_numeric
                         ,&buffer[SQLNUM_MAXLEN]
                         ,&numbegin
                         ,p_numeric->precision
                         ,(char)scale
                         ,&truncate);
  if(truncate == SQLNUM_TRUNC_WHOLE)
  {
    p_string = "ERROR truncation";
  }
  else if(numbegin)
  {
    p_string = (char*)numbegin;
    if(truncate == SQLNUM_TRUNC_FRAC)
    {
      p_string += " FRACTION!!";
    }
  }
}

// Convert a SQL_NUMERIC_STRUCT to a string. Only val and sign are
// read from the struct. precision and scale will be updated on the
// struct with the final values used in the conversion.
// 
// @param[in] sqlnum       Source struct
// @param[in] numstr       Buffer to convert into string. Note that you
//                         MUST use numbegin to read the result string.
//                         This should point to the LAST byte available.
//                         (We fill in digits backwards.)
// @param[in] numbegin     String pointer that will be set to the start of
//                         the result string.
// @param[in] reqprec      Requested precision
// @param[in] reqscale     Requested scale
// @param[in] truncptr     Pointer to set the truncation type encountered.
//                         If SQLNUM_TRUNC_WHOLE, this indicates a failure
//                         and the contents of numstr are undefined and
//                         numbegin will not be written to.
void 
SQLVariant::NumericToStringMantissa(SQL_NUMERIC_STRUCT* sqlnum
                                   ,SQLCHAR*            numstr
                                   ,SQLCHAR**           numbegin
                                   ,SQLCHAR             reqprec
                                   ,SQLSCHAR            reqscale
                                   ,int*                truncptr)
{
  int expanded[8];
  int i, j;
  int max_space= 0;
  int calcprec= 0;
  int trunc= 0; /* truncation indicator */

  *numstr--= 0;

  /*
  it's expected to have enough space
  (~at least min(39, max(prec, scale+2)) + 3)
  */

  /*
  expand the packed sqlnum->val so we have space to divide through
  expansion happens into an array in big-endian form
  */
  for (i= 0; i < 8; ++i)
  {
    expanded[7 - i] = (sqlnum->val[(2 * i) + 1] << 8) | sqlnum->val[2 * i];
  }
  /* max digits = 39 = log_10(2^128)+1 */
  for (j= 0; j < 39; ++j)
  {
    /* skip empty prefix */
    while (!expanded[max_space])
    {
      max_space++;
    }
    /* if only the last piece has a value, it's the end */
    if (max_space >= 7)
    {
      i= 7;
      if (!expanded[7])
      {
        /* special case for zero, we'll end immediately */
        if (!*(numstr + 1))
        {
          *numstr--= '0';
          calcprec= 1;
        }
        break;
      }
    }
    else
    {
      /* extract the next digit */
      sqlnum_unscale_be(expanded, max_space);
    }
    *numstr--= '0' + (expanded[7] % 10);
    expanded[7] /= 10;
    calcprec++;
    if (j == reqscale - 1)
    {
      *numstr--= '.';
    }
  }
  sqlnum->scale= reqscale;
  /* add <- dec pt */
  if (calcprec < reqscale)
  {
    while (calcprec < reqscale)
    {
      *numstr--= '0';
      reqscale--;
    }
    *numstr--= '.';
    *numstr--= '0';
  }
  /* handle fractional truncation */
  if (calcprec > reqprec && reqscale > 0)
  {
    SQLCHAR *end= numstr + strlen((char *)numstr) - 1;
    while (calcprec > reqprec && reqscale)
    {
      *end--= 0;
      calcprec--;
      reqscale--;
    }
    if (calcprec > reqprec && reqscale == 0)
    {
      trunc = SQLNUM_TRUNC_WHOLE;
      goto end;
    }
    if (*end == '.')
    {
      *end--= 0;
    }
    else
    {
      /* move the dec pt-- ??? */
      /*
      char c2, c= numstr[calcprec - reqscale];
      numstr[calcprec - reqscale]= '.';
      while (reqscale)
      {
      c2= numstr[calcprec + 1 - reqscale];
      numstr[calcprec + 1 - reqscale]= c;
      c= c2;
      reqscale--;
      }
      */
    }
    trunc = SQLNUM_TRUNC_FRAC;
  }
  /* add zeros for negative scale */
  if (reqscale < 0)
  {
    int i;
    reqscale *= -1;
    for (i= 1; i <= calcprec; ++i)
    {
      *(numstr + i - reqscale) = *(numstr + i);
    }
    numstr -= reqscale;
    memset(numstr + calcprec + 1, '0', reqscale);
  }
  sqlnum->precision = (SQLCHAR)calcprec;

  /* finish up, handle auxiliary fix-ups */
  if (!sqlnum->sign)
  {
    *numstr--= '-';
  }
  numstr++;
  *numbegin= numstr;

end:
  if (truncptr)
  {
    *truncptr= trunc;
  }
}

//////////////////////////////////////////////////////////////////////////
//
// TRIMMING CONVERSION ROUTINES BEGIN HERE
//
//////////////////////////////////////////////////////////////////////////

long
SQLVariant::ULongToLong(unsigned long p_value)
{
  if(p_value > LONG_MAX)
  {
    ThrowErrorTruncate(SQL_C_ULONG,SQL_C_SLONG);
  }
  return (long)p_value;
}

long
SQLVariant::FloatToLong(float p_value)
{
  if(p_value > LONG_MAX || p_value < LONG_MIN)
  {
    ThrowErrorTruncate(SQL_C_FLOAT,SQL_C_LONG);
  }
  return (long)p_value;
}

long
SQLVariant::DoubleToLong(double p_value)
{
  if(p_value > LONG_MAX || p_value < LONG_MIN)
  {
    ThrowErrorTruncate(SQL_C_DOUBLE,SQL_C_LONG);
  }
  return (long)p_value;
}

long
SQLVariant::BIGINTToLong(SQLBIGINT p_value)
{
  if(p_value > LONG_MAX || p_value < LONG_MIN)
  {
    ThrowErrorTruncate(SQL_C_SBIGINT,SQL_C_LONG);
  }
  return (long)p_value;
}

long
SQLVariant::UBIGINTToLong(SQLUBIGINT p_value)
{
  if(p_value > LONG_MAX)
  {
    ThrowErrorTruncate(SQL_C_UBIGINT,SQL_C_LONG);
  }
  return (long)p_value;
}

short
SQLVariant::UShortToShort(unsigned short p_value)
{
  if(p_value > SHRT_MAX)
  {
    ThrowErrorTruncate(SQL_C_USHORT,SQL_C_SHORT);
  }
  return (short)p_value;
}

short
SQLVariant::SLongToShort(long p_value)
{
  if(p_value > SHRT_MAX || p_value < SHRT_MIN)
  {
    ThrowErrorTruncate(SQL_C_SLONG,SQL_C_SHORT);
  }
  return (short)p_value;
}

short
SQLVariant::ULongToShort(unsigned long p_value)
{
  if(p_value > SHRT_MAX)
  {
    ThrowErrorTruncate(SQL_C_ULONG,SQL_C_SHORT);
  }
  return (short)p_value;
}

short
SQLVariant::FloatToShort(float p_value)
{
  if(p_value > SHRT_MAX || p_value < SHRT_MIN)
  {
    ThrowErrorTruncate(SQL_C_FLOAT,SQL_C_SHORT);
  }
  return (short)p_value;
}

short
SQLVariant::DoubleToShort(double p_value)
{
  if(p_value > SHRT_MAX || p_value < SHRT_MIN)
  {
    ThrowErrorTruncate(SQL_C_DOUBLE,SQL_C_SHORT);
  }
  return (short)p_value;
}

short
SQLVariant::BIGINTToShort(SQLBIGINT p_value)
{
  if(p_value > SHRT_MAX || p_value < SHRT_MIN)
  {
    ThrowErrorTruncate(SQL_C_SBIGINT,SQL_C_SHORT);
  }
  return (short)p_value;
}

short
SQLVariant::UBIGINTToShort(SQLUBIGINT p_value)
{
  if(p_value > SHRT_MAX)
  {
    ThrowErrorTruncate(SQL_C_UBIGINT,SQL_C_SHORT);
  }
  return (short)p_value;
}

float
SQLVariant::DoubleToFloat(double p_value)
{
  if(p_value > FLT_MAX || p_value < -FLT_MAX)
  {
    ThrowErrorTruncate(SQL_C_DOUBLE,SQL_C_FLOAT);
  }
  return (float) p_value;
}

SQLBIGINT
SQLVariant::FloatToBIGINT(float p_value)
{
  if(p_value > LLONG_MAX || p_value < LLONG_MIN)
  {
    ThrowErrorTruncate(SQL_C_FLOAT,SQL_C_SBIGINT);
  }
  return (SQLBIGINT) p_value;
}

SQLBIGINT
SQLVariant::DoubleToBIGINT(double p_value)
{
  if(p_value > LLONG_MAX || p_value < LLONG_MIN)
  {
    ThrowErrorTruncate(SQL_C_DOUBLE,SQL_C_SBIGINT);
  }
  return (SQLBIGINT)p_value;
}

SQLBIGINT 
SQLVariant::UBIGINTToBIGINT(SQLUBIGINT p_value)
{
  if(p_value > LLONG_MAX || p_value < LLONG_MIN)
  {
    ThrowErrorTruncate(SQL_C_UBIGINT,SQL_C_SBIGINT);
  }
  return (SQLBIGINT)p_value;
}

char
SQLVariant::ShortToTinyInt(short p_value)
{
  if(p_value > _I8_MAX || p_value < _I8_MIN)
  {
    ThrowErrorTruncate(SQL_C_SHORT,SQL_C_STINYINT);
  }
  return (char)p_value;
}

char
SQLVariant::UShortToTinyInt(unsigned short p_value)
{
  if(p_value > _I8_MAX)
  {
    ThrowErrorTruncate(SQL_C_USHORT,SQL_C_STINYINT);
  }
  return (char)p_value;
}

char
SQLVariant::SLongToTinyInt(long p_value)
{
  if(p_value > _I8_MAX || p_value < _I8_MIN)
  {
    ThrowErrorTruncate(SQL_C_SLONG,SQL_C_STINYINT);
  }
  return (char)p_value;
}

char
SQLVariant::ULongToTinyInt(long p_value)
{
  if(p_value > _I8_MAX)
  {
    ThrowErrorTruncate(SQL_C_ULONG,SQL_C_STINYINT);
  }
  return (char)p_value;
}

char
SQLVariant::FloatToTinyInt(float p_value)
{
  if(p_value > _I8_MAX || p_value < _I8_MIN)
  {
    ThrowErrorTruncate(SQL_C_FLOAT,SQL_C_STINYINT);
  }
  return (char)p_value;
}

char
SQLVariant::DoubleToTinyInt(double p_value)
{
  if(p_value > _I8_MAX || p_value < _I8_MIN)
  {
    ThrowErrorTruncate(SQL_C_DOUBLE,SQL_C_STINYINT);
  }
  return (char)p_value;
}

char
SQLVariant::UTinyIntToTinyInt(unsigned char p_value)
{
  if(p_value > _I8_MAX)
  {
    ThrowErrorTruncate(SQL_C_UTINYINT,SQL_C_STINYINT);
  }
  return (char)p_value;
}

char
SQLVariant::SBIGINTToTinyInt(SQLBIGINT p_value)
{
  if(p_value > _I8_MAX || p_value < _I8_MIN)
  {
    ThrowErrorTruncate(SQL_C_SBIGINT,SQL_C_STINYINT);
  }
  return (char)p_value;
}

char
SQLVariant::UBIGINTToTinyInt(SQLUBIGINT p_value)
{
  if(p_value > _I8_MAX)
  {
    ThrowErrorTruncate(SQL_C_UBIGINT,SQL_C_STINYINT);
  }
  return (char)p_value;
}

unsigned long
SQLVariant::ShortToULong(short p_value)
{
  if(p_value < 0)
  {
    ThrowErrorTruncate(SQL_C_SHORT,SQL_C_ULONG);
  }
  return (unsigned long) p_value;
}

unsigned long
SQLVariant::LongToULong(long p_value)
{
  if(p_value < 0)
  {
    ThrowErrorTruncate(SQL_C_LONG,SQL_C_ULONG);
  }
  return (unsigned long) p_value;
}

unsigned long
SQLVariant::FloatToULong(float p_value)
{
  if(p_value < 0.0 || p_value > ULONG_MAX)
  {
    ThrowErrorTruncate(SQL_C_FLOAT,SQL_C_ULONG);
  }
  return (unsigned long) p_value;
}

unsigned long
SQLVariant::DoubleToULong(double p_value)
{
  if(p_value < 0.0 || p_value > ULONG_MAX)
  {
    ThrowErrorTruncate(SQL_C_DOUBLE,SQL_C_ULONG);
  }
  return (unsigned long) p_value;
}

unsigned long
SQLVariant::TinyIntToULong(char p_value)
{
  if(p_value < 0)
  {
    ThrowErrorTruncate(SQL_C_STINYINT,SQL_C_ULONG);
  }
  return (unsigned long) p_value;
}

unsigned long
SQLVariant::BIGINTToULong(SQLBIGINT p_value)
{
  if(p_value < 0 || p_value > ULONG_MAX)
  {
    ThrowErrorTruncate(SQL_C_SBIGINT,SQL_C_ULONG);
  }
  return (unsigned long) p_value;
}

unsigned long
SQLVariant::UBIGINTToULong(SQLUBIGINT p_value)
{
  if(p_value > ULONG_MAX)
  {
    ThrowErrorTruncate(SQL_C_UBIGINT,SQL_C_ULONG);
  }
  return (unsigned long) p_value;
}

SQLUBIGINT
SQLVariant::ShortToUBIGINT(short p_value)
{
  if(p_value < 0)
  {
    ThrowErrorTruncate(SQL_C_SHORT,SQL_C_UBIGINT);
  }
  return (SQLUBIGINT) p_value;
}

SQLUBIGINT
SQLVariant::LongToUBIGINT(long p_value)
{
  if(p_value < 0)
  {
    ThrowErrorTruncate(SQL_C_LONG,SQL_C_UBIGINT);
  }
  return (SQLUBIGINT) p_value;
}

SQLUBIGINT
SQLVariant::FloatToUBIGINT(float p_value)
{
  if(p_value < 0.0 || p_value > ULLONG_MAX)
  {
    ThrowErrorTruncate(SQL_C_FLOAT,SQL_C_UBIGINT);
  }
  return (SQLUBIGINT) p_value;
}

SQLUBIGINT
SQLVariant::DoubleToUBIGINT(double p_value)
{
  if(p_value < 0.0 || p_value > ULLONG_MAX)
  {
    ThrowErrorTruncate(SQL_C_DOUBLE,SQL_C_UBIGINT);
  }
  return (SQLUBIGINT) p_value;
}

SQLUBIGINT
SQLVariant::TinyIntToUBIGINT(char p_value)
{
  if(p_value < 0)
  {
    ThrowErrorTruncate(SQL_C_TINYINT,SQL_C_UBIGINT);
  }
  return (SQLUBIGINT) p_value;
}

SQLUBIGINT
SQLVariant::SBIGINTToUBIGINT(SQLBIGINT p_value)
{
  if(p_value < 0)
  {
    ThrowErrorTruncate(SQL_C_SBIGINT,SQL_C_UBIGINT);
  }
  return (SQLUBIGINT) p_value;
}

unsigned short
SQLVariant::ShortToUShort(short p_value)
{
  if(p_value < 0)
  {
    ThrowErrorTruncate(SQL_C_SHORT,SQL_C_USHORT);
  }
  return (unsigned short) p_value;
}

unsigned short
SQLVariant::SLongToUShort(long p_value)
{
  if(p_value < 0 || p_value > SHRT_MAX)
  {
    ThrowErrorTruncate(SQL_C_LONG,SQL_C_USHORT);
  }
  return (unsigned short) p_value;
}

unsigned short
SQLVariant::ULongToUShort(unsigned long p_value)
{
  if(p_value > SHRT_MAX)
  {
    ThrowErrorTruncate(SQL_C_ULONG,SQL_C_USHORT);
  }
  return (unsigned short) p_value;
}

unsigned short
SQLVariant::FloatToUShort(float p_value)
{
  if(p_value < 0.0 || p_value > SHRT_MAX)
  {
    ThrowErrorTruncate(SQL_C_FLOAT,SQL_C_USHORT);
  }
  return (unsigned short) p_value;
}

unsigned short
SQLVariant::DoubleToUShort(double p_value)
{
  if(p_value < 0.0 || p_value > SHRT_MAX)
  {
    ThrowErrorTruncate(SQL_C_DOUBLE,SQL_C_USHORT);
  }
  return (unsigned short) p_value;
}

unsigned short
SQLVariant::TinyIntToUShort(char p_value)
{
  if(p_value < 0)
  {
    ThrowErrorTruncate(SQL_C_STINYINT,SQL_C_USHORT);
  }
  return (unsigned short) p_value;
}

unsigned short
SQLVariant::BIGINTToUShort(SQLBIGINT p_value)
{
  if(p_value < 0.0 || p_value > SHRT_MAX)
  {
    ThrowErrorTruncate(SQL_C_SBIGINT,SQL_C_USHORT);
  }
  return (unsigned short) p_value;
}

unsigned short
SQLVariant::UBIGINTToUShort(SQLUBIGINT p_value)
{
  if(p_value > SHRT_MAX)
  {
    ThrowErrorTruncate(SQL_C_UBIGINT,SQL_C_USHORT);
  }
  return (unsigned short) p_value;
}

unsigned char
SQLVariant::ShortToUTinyInt(short p_value)
{
  if(p_value < 0 || p_value > _UI8_MAX)
  {
    ThrowErrorTruncate(SQL_C_SHORT,SQL_C_UTINYINT);
  }
  return (unsigned char) p_value;
}

unsigned char
SQLVariant::UShortToUTinyInt(short p_value)
{
  if(p_value < 0 || p_value > _UI8_MAX)
  {
    ThrowErrorTruncate(SQL_C_SHORT,SQL_C_UTINYINT);
  }
  return (unsigned char) p_value;
}

unsigned char
SQLVariant::SLongToUTinyInt(long p_value)
{
  if(p_value < 0 || p_value > _UI8_MAX)
  {
    ThrowErrorTruncate(SQL_C_LONG,SQL_C_UTINYINT);
  }
  return (unsigned char) p_value;
}

unsigned char
SQLVariant::ULongToUTinyInt(unsigned long p_value)
{
  if(p_value > _UI8_MAX)
  {
    ThrowErrorTruncate(SQL_C_ULONG,SQL_C_UTINYINT);
  }
  return (unsigned char) p_value;
}

unsigned char
SQLVariant::FloatToUTinyInt(float p_value)
{
  if(p_value < 0.0 || p_value > _UI8_MAX)
  {
    ThrowErrorTruncate(SQL_C_FLOAT,SQL_C_UTINYINT);
  }
  return (unsigned char) p_value;
}

unsigned char
SQLVariant::DoubleToUTinyInt(double p_value)
{
  if(p_value < 0.0 || p_value > _UI8_MAX)
  {
    ThrowErrorTruncate(SQL_C_DOUBLE,SQL_C_UTINYINT);
  }
  return (unsigned char) p_value;
}

unsigned char
SQLVariant::TinyIntToUTinyInt(char p_value)
{
  if(p_value < 0 || p_value > _UI8_MAX)
  {
    ThrowErrorTruncate(SQL_C_STINYINT,SQL_C_UTINYINT);
  }
  return (unsigned char) p_value;
}

unsigned char
SQLVariant::SBIGINTToUTinyInt(SQLBIGINT p_value)
{
  if(p_value < 0 || p_value > _UI8_MAX)
  {
    ThrowErrorTruncate(SQL_C_SBIGINT,SQL_C_UTINYINT);
  }
  return (unsigned char) p_value;
}

unsigned char
SQLVariant::UBIGINTToUTinyInt(SQLUBIGINT p_value)
{
  if(p_value > _UI8_MAX)
  {
    ThrowErrorTruncate(SQL_C_UBIGINT,SQL_C_UTINYINT);
  }
  return (unsigned char) p_value;
}

//////////////////////////////////////////////////////////////////////////
//
// GENERAL ERRORS - THROWING A CString
//
//////////////////////////////////////////////////////////////////////////

void
SQLVariant::ThrowErrorDatatype(int p_getas)
{
  CString error;
  char* type  = FindDatatype(m_datatype);
  char* getas = FindDatatype(p_getas);
  error.Format("Cannot get a %s as a %s datatype.",type,getas);
  throw error;
}

void
SQLVariant::ThrowErrorTruncate(int p_from,int p_to)
{
  CString error;
  char* from = FindDatatype(p_from);
  char* to   = FindDatatype(p_to);
  error.Format("Cannot truncate %s to %s",from,to);
  throw error;
}