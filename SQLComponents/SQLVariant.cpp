////////////////////////////////////////////////////////////////////////
//
// File: SQLVariant.cpp
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
#include "StdAfx.h"
#include "SQLVariant.h"
#include "SQLDataType.h"
#include "SQLVariantTrim.h"
#include "SQLDate.h"
#include "SQLTime.h"
#include "SQLTimestamp.h"
#include "SQLGuid.h"
#include <bcd.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{

// General constructors
SQLVariant::SQLVariant()
{
  Init();
}

// XTOR reserving space for a string
SQLVariant::SQLVariant(int p_type,int p_space)
{
  Init();
  ReserveSpace(p_type,p_space);
}

// XTOR from SQLVariant pointer
SQLVariant::SQLVariant(const SQLVariant* p_that)
{
  // Use assignment operator
  *this = *p_that;
}

// XTOR From another SQLVariant reference
SQLVariant::SQLVariant(const SQLVariant& p_var)
{
  *this = p_var;
}

// XTOR SQL_C_CHAR / SQL_C_WCHAR
SQLVariant::SQLVariant(LPCTSTR p_data,bool p_wide /*=false*/)
{
  Init();
  SetData(p_wide ? SQL_C_WCHAR : SQL_C_CHAR,p_data);
}

// XTOR SQL_C_CHAR / SQL_C_WCHAR FROM MFC XString
SQLVariant::SQLVariant(const XString& p_data,bool p_wide /*=false*/)
{
  Init();
  SetData(p_wide ? SQL_C_WCHAR : SQL_C_CHAR,reinterpret_cast<LPCTSTR>(p_data.GetString()));
}

// XTOR SQL_C_SHORT / SQL_C_SSHORT
SQLVariant::SQLVariant(short p_short)
{
  Init();
  m_datatype    = SQL_C_SSHORT;
  m_sqlDatatype = SQL_INTEGER;
  m_indicator   = 0;
  m_data.m_dataSSHORT  = p_short;
}

// XTOR SQL_C_USHORT
SQLVariant::SQLVariant(unsigned short p_short)
{
  Init();
  m_datatype    = SQL_C_USHORT;
  m_sqlDatatype = SQL_INTEGER;
  m_indicator   = 0;
  m_data.m_dataUSHORT  = p_short;
}

// XTOR SQL_C_LONG / SQL_C_SLONG
SQLVariant::SQLVariant(int p_long)
{
  Init();
  m_datatype    = SQL_C_SLONG;
  m_sqlDatatype = SQL_INTEGER;
  m_indicator   = 0;
  m_data.m_dataSLONG = p_long;
}

// XTOR SQL_C_ULONG
SQLVariant::SQLVariant(unsigned int p_long)
{
  Init();
  m_datatype    = SQL_C_ULONG;
  m_sqlDatatype = SQL_BIGINT;
  m_indicator   = 0;
  m_data.m_dataULONG = p_long;
}

// XTOR SQL_C_FLOAT
SQLVariant::SQLVariant(float p_float)
{
  Init();
  m_datatype    = SQL_C_FLOAT;
  m_sqlDatatype = SQL_FLOAT;
  m_indicator   = 0;
  m_data.m_dataFLOAT = p_float;
}

// XTOR SQL_C_DOUBLE
SQLVariant::SQLVariant(double p_double)
{
  Init();
  m_datatype    = SQL_C_DOUBLE;
  m_sqlDatatype = SQL_DOUBLE;
  m_indicator   = 0;
  m_data.m_dataDOUBLE = p_double;
}

// XTOR SQL_C_BIT
SQLVariant::SQLVariant(bool p_bit)
{
  Init();
  m_datatype    = SQL_C_BIT;
  m_sqlDatatype = SQL_BIT;
  m_indicator   = 0;
  m_data.m_dataBIT = p_bit;
}

// XTOR SQL_C_TINYINT / SQL_C_STINYINT
SQLVariant::SQLVariant(char p_tinyint)
{
  Init();
  m_datatype    = SQL_C_STINYINT;
  m_sqlDatatype = SQL_TINYINT;
  m_indicator   = 0;
  m_data.m_dataSTINYINT = p_tinyint;
}

// XTOR SQL_C_UTINYINT
SQLVariant::SQLVariant(unsigned char p_tinyint)
{
  Init();
  m_datatype    = SQL_C_UTINYINT;
  m_sqlDatatype = SQL_TINYINT;
  m_indicator   = 0;
  m_data.m_dataUTINYINT = p_tinyint;
}

// XTOR SQL_C_BIGINT
SQLVariant::SQLVariant(__int64 p_bigint)
{
  Init();
  m_datatype    = SQL_C_SBIGINT;
  m_sqlDatatype = SQL_BIGINT;
  m_indicator   = 0;
  m_data.m_dataSBIGINT = p_bigint;
}

// XTOR SQL_C_UBIGINT
SQLVariant::SQLVariant(unsigned __int64 p_bigint)
{
  Init();
  m_datatype    = SQL_C_UBIGINT;
  m_sqlDatatype = SQL_BIGINT;
  m_indicator   = 0;
  m_data.m_dataUBIGINT = p_bigint;
}

// XTOR SQL_C_NUMERIC
SQLVariant::SQLVariant(const SQL_NUMERIC_STRUCT* p_numeric)
{
  Init();
  m_datatype    = SQL_C_NUMERIC;
  m_sqlDatatype = SQL_NUMERIC;
  m_indicator   = 0;
  memcpy(&m_data.m_dataNUMERIC,p_numeric,sizeof(SQL_NUMERIC_STRUCT));
}

// XTOR SQL_C_GUID
SQLVariant::SQLVariant(const SQLGUID* p_guid)
{
  Init();
  m_datatype    = SQL_C_GUID;
  m_sqlDatatype = SQL_GUID;
  m_indicator   = 0;
  memcpy(&m_data.m_dataGUID,p_guid,sizeof(SQLGUID));
}

// XTOR SQL_C_BINARY
SQLVariant::SQLVariant(const void* p_binary,size_t p_size)
{
  Init();
  m_datatype    = SQL_C_BINARY;
  m_sqlDatatype = SQL_BINARY;
  m_indicator   = 0;
  m_data.m_dataBINARY = new BYTE[(size_t)p_size + 1];
  memcpy(m_data.m_dataBINARY,p_binary,p_size);
}

// XTOR SQL_C_DATE
SQLVariant::SQLVariant(const DATE_STRUCT* p_date)
{
  Init();
  m_datatype    = SQL_C_DATE;
  m_sqlDatatype = SQL_DATE;
  m_indicator   = 0;
  memcpy(&m_data.m_dataDATE,p_date,sizeof(DATE_STRUCT));
}

// XTOR SQL_C_TIME
SQLVariant::SQLVariant(const TIME_STRUCT* p_time)
{
  Init();
  m_datatype    = SQL_C_TIME;
  m_sqlDatatype = SQL_TIME;
  m_indicator   = 0;
  memcpy(&m_data.m_dataTIME,p_time,sizeof(TIME_STRUCT));
}

SQLVariant::SQLVariant(const TIMESTAMP_STRUCT* p_stamp)
{
  Init();
  m_datatype    = SQL_C_TIMESTAMP;
  m_sqlDatatype = SQL_TIMESTAMP;
  m_indicator   = 0;
  memcpy(&m_data.m_dataTIMESTAMP,p_stamp,sizeof(TIMESTAMP_STRUCT));
}

// XTOR SQL_C_INTERVAL_YEAR -> SQL_C_INTERVAL_DAY_TO_SECOND
SQLVariant::SQLVariant(const SQL_INTERVAL_STRUCT* p_inter)
{
  Init();
  m_datatype    = p_inter->interval_type;
  m_sqlDatatype = p_inter->interval_type;
  m_indicator   = 0;
  if(IsIntervalType() == false)
  {
    m_datatype    = SQL_C_INTERVAL_DAY_TO_SECOND;
    m_sqlDatatype = SQL_INTERVAL_DAY_TO_SECOND;
  }
  memcpy(&m_data.m_dataINTERVAL,p_inter,sizeof(SQL_INTERVAL_STRUCT));
}

SQLVariant::SQLVariant(const SQLDate* p_date)
{
  Init();
  m_datatype    = SQL_C_DATE;
  m_sqlDatatype = SQL_DATE;

  if(p_date->IsNull())
  {
    m_indicator = SQL_NULL_DATA;
  }
  else
  {
    m_indicator = 0;
    p_date->AsDateStruct(&m_data.m_dataDATE);
  }
}

SQLVariant::SQLVariant(const SQLTime* p_time)
{
  Init();
  m_datatype    = SQL_C_TIME;
  m_sqlDatatype = SQL_TIME;

  if(p_time->IsNull())
  {
    m_indicator = SQL_NULL_DATA;
  }
  else
  {
    m_indicator = 0;
    p_time->AsTimeStruct(&m_data.m_dataTIME);
  }
}

SQLVariant::SQLVariant(const SQLTimestamp* p_stamp)
{
  Init();
  m_datatype    = SQL_C_TIMESTAMP;
  m_sqlDatatype = SQL_TIMESTAMP;

  if(p_stamp->IsNull())
  {
    m_indicator = SQL_NULL_DATA;
  }
  else
  {
    m_indicator   = 0;
    p_stamp->AsTimeStampStruct(&m_data.m_dataTIMESTAMP);
  }
}

SQLVariant::SQLVariant(const SQLInterval* p_interval)
{
  Init();
  m_datatype    = p_interval->GetSQLDatatype();
  m_sqlDatatype = m_datatype;

  if(p_interval->IsNull())
  {
    m_indicator = SQL_NULL_DATA;
  }
  else
  {
    m_indicator = 0;
    p_interval->AsIntervalStruct(&m_data.m_dataINTERVAL);
  }
}

SQLVariant::SQLVariant(const bcd* p_bcd)
{
  Init();
  m_datatype    = SQL_C_NUMERIC;
  m_sqlDatatype = SQL_NUMERIC;
  m_indicator   = 0;
  p_bcd->AsNumeric(&m_data.m_dataNUMERIC);
}

SQLVariant::SQLVariant(const SQLGuid* p_guid)
{
  Init();
  m_datatype    = SQL_C_GUID;
  m_sqlDatatype = SQL_GUID;
  m_indicator   = 0;
  memcpy_s(&m_data.m_dataGUID,sizeof(SQLGUID),p_guid->AsGUID(),sizeof(SQLGUID));
}

// GENERAL DTOR
SQLVariant::~SQLVariant()
{
  // De-allocate char/binary buffers
  ResetDataType(0);
}

// Reset the complete variant
void
SQLVariant::Init()
{
  // Set contents of m_data to zero's
  ZeroMemory(&m_data,sizeof(m_data));

  ResetDataType(0);
  m_binaryLength    = 0;
  m_binaryPieceSize = 0;
  m_columnNumber    = 0;
  m_paramType       = P_SQL_PARAM_TYPE_UNKNOWN;
  m_useAtExec       = false;
}

void
SQLVariant::ReserveSpace(int p_type,int p_space)
{
  ResetDataType(p_type);
  if(p_type == SQL_C_CHAR)
  {
    m_binaryLength = p_space + 1;
    m_data.m_dataCHAR = new char[(size_t)m_binaryLength];
    m_data.m_dataCHAR[0] = 0;
  }
  if(p_type == SQL_C_WCHAR)
  {
    m_binaryLength = (p_space + 1) * 2;
    m_data.m_dataWCHAR = new wchar_t[(size_t) (p_space + 1)];
    m_data.m_dataWCHAR[0] = 0;
  }
  if(p_type == SQL_C_BINARY)
  {
    m_binaryLength = p_space + 1;
    m_data.m_dataBINARY = new BYTE[(size_t)m_binaryLength];
  }
  // Reserving space means, that the contents is reset to logical NULL
  // Only, leave the indicator in case of AT_EXEC_DATA so drivers will not get upset!
  // AT_EXEC_DATA means that the contents cannot be known beforehand, but must be supplied
  if(m_indicator >= 0 && m_useAtExec == false)
  {
    m_indicator = SQL_NULL_DATA;
  }
}

// In case of CHAR parameter output, shrink the spaces
void
SQLVariant::ShrinkSpace()
{
  // Only to do for character strings
  if(m_datatype != SQL_C_CHAR && m_datatype != SQL_C_WCHAR)
  {
    return;
  }
  if((m_paramType != P_SQL_PARAM_OUTPUT) && (m_paramType != P_SQL_PARAM_INPUT_OUTPUT))
  {
    return;
  }

  // Trim of the trailing spaces. Some RDBMS will return trailing spaces instead of SQL_NTS. 
  // This makes it impossible to return strings with spaces in the end.
  // DO NOT realloc the buffer, or we would lose the data binding
  if(m_datatype == SQL_C_CHAR)
  {
    int length = m_binaryLength - 2;
    while(length >= 0)
    {
      char c = m_data.m_dataCHAR[length];
      if(c == ' ' || c == 0)
      {
        m_data.m_dataCHAR[length--] = 0;
      }
      else
      {
        break;
      }
    }
  }
  else  // Must be SQL_C_WCHAR
  {
    int length = (m_binaryLength - 4) / 2;
    while(length)
    {
      wchar_t c = m_data.m_dataWCHAR[length];
      if(c == L' ' || c == 0)
      {
        m_data.m_dataWCHAR[length--] = 0;
      }
      else
      {
        break;
      }
    }
  }
}

// Truncate the CHAR data, but leave the data pointer
// otherwise we would lose the data binding
void    
SQLVariant::TruncateSpace(unsigned p_length)
{
  if(m_datatype == SQL_C_CHAR && ((unsigned)m_binaryLength > p_length))
  {
    // Save length on the heap
    m_binaryPieceSize = m_binaryLength;
    // Make exact buffer for streaming interface
    m_data.m_dataCHAR[m_binaryLength = p_length] = 0;
    m_indicator = p_length > 0 ? SQL_NTS : SQL_NULL_DATA;
  }
  if(m_datatype == SQL_C_WCHAR && ((unsigned) m_binaryLength > (p_length * 2)))
  {
    // Save length on the heap
    m_binaryPieceSize = m_binaryLength;
    // Make exact buffer for streaming interface
    m_data.m_dataWCHAR[p_length] = 0;
    m_binaryLength = p_length * 2;
    m_indicator = p_length > 0 ? SQL_NTS : SQL_NULL_DATA;
  }
}

// Check if the data is 'empty'
// CHAR -> Empty string, scalar types are 0 or 0.0
bool
SQLVariant::IsEmpty() const
{
  int len = 0;
  const unsigned char* data = reinterpret_cast<const unsigned char*>(&m_data.m_dataSHORT);
  switch(m_datatype)
  {
    case SQL_C_CHAR:    return ((m_data.m_dataCHAR  == NULL) || (m_data.m_dataCHAR [0] == 0));
    case SQL_C_WCHAR:   return ((m_data.m_dataWCHAR == NULL) || (m_data.m_dataWCHAR[0] == 0));
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
SQLVariant::IsNumericType() const
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
    case SQL_C_NUMERIC:   // Fall through
    case SQL_DECIMAL:     return true;
    default:              return false;
  }
}

bool
SQLVariant::IsDecimalType() const
{
  if(m_datatype == SQL_C_NUMERIC)
  {
    return true;
  }
  if(m_sqlDatatype == SQL_NUMERIC || m_sqlDatatype == SQL_DECIMAL)
  {
    return true;
  }
  return false;
}


bool
SQLVariant::IsIntervalType() const
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
SQLVariant::IsDateTimeType() const
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

bool
SQLVariant::IsBinaryType() const
{
  if(m_datatype == SQL_C_BINARY)
  {
    return true;
  }
  switch(m_sqlDatatype)
  {
    case SQL_BINARY:
    case SQL_VARBINARY:
    case SQL_LONGVARBINARY: return true;
    default:                return false;
  }
}

void
SQLVariant::Reset()
{
  ResetDataType(m_datatype);
}

void
SQLVariant::ResetDataType(int p_type)
{
  switch(m_datatype)
  {
    case SQL_C_CHAR:  delete [] m_data.m_dataCHAR;   break;
    case SQL_C_WCHAR: delete [] m_data.m_dataWCHAR;  break;
    case SQL_C_BINARY:delete [] m_data.m_dataBINARY; break;
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
  else if(m_datatype == SQL_C_WCHAR)
  {
    m_data.m_dataWCHAR[0] = 0;
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
    if(p_type == SQL_VARCHAR     ||
       p_type == SQL_LONGVARCHAR  )
    {
      m_sqlDatatype = p_type;
    }
  }
  else if(m_datatype == SQL_C_WCHAR)
  {
    if(p_type == SQL_WCHAR   ||
       p_type == SQL_WVARCHAR )
    {
      m_sqlDatatype = p_type;
    }
  }
  else if(m_datatype == SQL_C_BINARY)
  {
    if(p_type == SQL_VARBINARY     ||
       p_type == SQL_LONGVARBINARY )
    {
      m_sqlDatatype = p_type;
    }
  }
  else 
  {
    // Various numerics, dates, timestamps etc.
    m_sqlDatatype = p_type;
  }
}

void
SQLVariant::SetDataTypeDate()
{
  if (m_datatype == SQL_C_TIMESTAMP || m_datatype == SQL_C_TYPE_TIMESTAMP)
  {
    m_datatype    = SQL_C_TYPE_DATE;
    m_sqlDatatype = SQL_C_TYPE_DATE;
  }
}


// Setting the NUMERIC precision and scale
// Can e.g. be used for output parameter settings
void    
SQLVariant::SetNumericPrecisionScale(int p_precision,int p_scale)
{
  if(m_datatype != SQL_C_NUMERIC && m_sqlDatatype != SQL_DECIMAL)
  {
    return;
  }
  m_data.m_dataNUMERIC.precision = (SQLCHAR)  p_precision;
  m_data.m_dataNUMERIC.scale     = (SQLSCHAR) p_scale;
}

// Reduction of SQL_XXX to SQL_C_XXX type
int
SQLVariant::FindDataTypeFromSQLType() const
{
  switch(m_sqlDatatype)
  {
    case SQL_WCHAR:
    case SQL_WVARCHAR:      return SQL_C_WCHAR;
    case SQL_VARCHAR:
    case SQL_LONGVARCHAR:   return SQL_C_CHAR;
    case SQL_VARBINARY:
    case SQL_LONGVARBINARY: return SQL_C_BINARY;
  }
  // Gets you the ordinary datatype
  return m_datatype;
}

void
SQLVariant::SetParameterType(SQLParamType p_type)
{
  if(p_type >= P_SQL_PARAM_TYPE_UNKNOWN &&
     p_type <= P_SQL_RETURN_VALUE)
  {
    m_paramType = p_type;
  }
}

// Setting the variant from a binary buffer
// For instance 'stream data' like data dump
void
SQLVariant::SetFromBinaryStreamData(int   p_type
                                   ,int   p_length
                                   ,void* p_data
                                   ,bool  p_isnull)
{
  // Reset data member
  ResetDataType(0);

  // Set C and SQL datatypes alike
  m_datatype    = p_type;
  m_sqlDatatype = p_type;

  // Depends on datatype
  if(p_type == SQL_C_CHAR || p_type == SQL_C_WCHAR || p_type == SQL_C_BINARY)
  {
    int detected = p_length;
    if(p_type == SQL_C_CHAR)
    {
      detected = (int) strlen((char*)p_data);
    }
    if(p_type == SQL_C_WCHAR)
    {
       detected = (int)wcslen((wchar_t*)p_data) * 2;
    }
    if(detected < p_length)
    {
      p_length = detected;
    }
    // Allocated buffer types
    m_binaryLength = p_length;
    m_indicator    = p_isnull ? SQL_NULL_DATA : p_length;
    m_data.m_dataBINARY = new BYTE[(size_t)m_binaryLength + 2];
    memcpy(m_data.m_dataBINARY,p_data,(size_t)p_length);
    (reinterpret_cast<BYTE*>(m_data.m_dataBINARY))[p_length] = 0;
  }
  else
  {
    // Store in the m_data union block as an internal
    if(p_length < sizeof(m_data))
    {
      memcpy(&m_data.m_dataSHORT,p_data,p_length);
      m_indicator = p_isnull ? SQL_NULL_DATA : 0;
    }
    else
    {
      throw StdException("Cannot set variant from binary stream data.");
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
//  GENERAL ACCESS
//
//////////////////////////////////////////////////////////////////////////

void
SQLVariant::GetAsString(XString& result) const
{
  if(m_indicator == SQL_NULL_DATA)
  {
    result = "";
    return;
  }
  unsigned char* pointer;
  switch(m_datatype)
  {
    case SQL_C_CHAR:
#ifdef UNICODE
                                          { int length = MultiByteToWideChar(GetACP(),0,m_data.m_dataCHAR,-1,NULL,NULL);
                                            LPWSTR resbuf = result.GetBufferSetLength(length + 1);
                                            // Doing the 'real' conversion
                                            MultiByteToWideChar(GetACP(),0,m_data.m_dataCHAR,-1,resbuf,length);
                                            result.ReleaseBuffer();
                                          }
#else
                                          result = m_data.m_dataCHAR;
#endif
                                          break;
    case SQL_C_WCHAR:
#ifdef UNICODE
                                          result = m_data.m_dataWCHAR;
#else
                                          {
                                            // Getting the length of the translation buffer first
                                            int clength = ::WideCharToMultiByte(GetACP(),0,m_data.m_dataWCHAR,-1,NULL,0,NULL,NULL);
                                            char* resbuf = result.GetBufferSetLength(clength + 1);
                                            int blength = ::WideCharToMultiByte(GetACP(),0,m_data.m_dataWCHAR,clength,resbuf,clength,NULL,NULL);
                                            resbuf[clength] = 0;
                                            if(blength > 0 && blength < clength)
                                            {
                                              resbuf[blength] = 0;
                                            }
                                            result.ReleaseBuffer();
                                          }
#endif
                                          break;
    case SQL_C_BINARY:                    pointer = reinterpret_cast<unsigned char*>(result.GetBufferSetLength(m_binaryLength * 2 + 2));
                                          BinaryToString(pointer,m_binaryLength*2 + 2);
                                          result.ReleaseBuffer();
                                          break;
    case SQL_C_SHORT:                     // Fall through
    case SQL_C_SSHORT:                    result.Format(_T("%i"),(int)m_data.m_dataSHORT);
                                          break;
    case SQL_C_USHORT:                    result.Format(_T("%i"),(int)m_data.m_dataUSHORT);
                                          break;
    case SQL_C_LONG:                      // Fall through
    case SQL_C_SLONG:                     result.Format(_T("%li"),m_data.m_dataLONG);
                                          break;
    case SQL_C_ULONG:                     result.Format(_T("%lu"),m_data.m_dataULONG);
                                          break;
    case SQL_C_FLOAT:                     result.Format(_T("%.7g"),m_data.m_dataFLOAT);
                                          break;
    case SQL_C_DOUBLE:                    // Standard Oracle ODBC can get a maximum of 15 decimals
                                          result.Format(_T("%.15lg"),m_data.m_dataDOUBLE);
                                          break;
    case SQL_C_BIT:                       result.Format(_T("%d"),(int)m_data.m_dataBIT);
                                          break;
    case SQL_C_TINYINT:                   // Fall through
    case SQL_C_STINYINT:                  result.Format(_T("%d"),(int)m_data.m_dataTINYINT);
                                          break;
    case SQL_C_UTINYINT:                  result.Format(_T("%u"),(unsigned)m_data.m_dataUTINYINT);
                                          break;
    case SQL_C_SBIGINT:                   result.Format(_T("%I64d"),m_data.m_dataSBIGINT);
                                          break;
    case SQL_C_UBIGINT:                   result.Format(_T("%I64u"),m_data.m_dataUBIGINT);
                                          break;
    case SQL_DECIMAL:                     // Fall through
    case SQL_C_NUMERIC:                   { bcd num(&m_data.m_dataNUMERIC);
                                            result = num.AsString(bcd::Format::Bookkeeping,false,0);
                                          }
                                          break;
    case SQL_C_GUID:                      result.Format(_T("{%04X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}")
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
    case SQL_C_TYPE_DATE:                 result.Format(_T("%02.2d-%02.2d-%04.4d")
                                                       ,m_data.m_dataDATE.day
                                                       ,m_data.m_dataDATE.month
                                                       ,m_data.m_dataDATE.year);
                                          break;
    case SQL_C_TIME:                      // Fall through
    case SQL_C_TYPE_TIME:                 result.Format(_T("%02.2d:%02.2d:%02.2d")
                                                       ,m_data.m_dataTIME.hour
                                                       ,m_data.m_dataTIME.minute
                                                       ,m_data.m_dataTIME.second);
                                          break;
    case SQL_C_TIMESTAMP:                 // Fall through
    case SQL_C_TYPE_TIMESTAMP:            result.Format(_T("%04.4d-%02.2d-%02.2d %02.2d:%02.2d:%02.2d")
                                                       ,m_data.m_dataTIMESTAMP.year
                                                       ,m_data.m_dataTIMESTAMP.month
                                                       ,m_data.m_dataTIMESTAMP.day
                                                       ,m_data.m_dataTIMESTAMP.hour
                                                       ,m_data.m_dataTIMESTAMP.minute
                                                       ,m_data.m_dataTIMESTAMP.second);
                                          if(m_data.m_dataTIMESTAMP.fraction)
                                          {
                                            // Fractions are in NANO-Second resolution
                                            XString frac;
                                            frac.Format(_T("%0.6f"),(double)m_data.m_dataTIMESTAMP.fraction / 1000000000.0);
                                            result += frac.Mid(1);
                                          }
                                          break;
    case SQL_C_INTERVAL_YEAR:             result.Format(_T("%d"),m_data.m_dataINTERVAL.intval.year_month.year);
                                          break;
    case SQL_C_INTERVAL_MONTH:            result.Format(_T("%d"),m_data.m_dataINTERVAL.intval.year_month.month);
                                          break;
    case SQL_C_INTERVAL_DAY:              result.Format(_T("%d"),m_data.m_dataINTERVAL.intval.day_second.day);
                                          break;
    case SQL_C_INTERVAL_HOUR:             result.Format(_T("%d"),m_data.m_dataINTERVAL.intval.day_second.hour);
                                          break;
    case SQL_C_INTERVAL_MINUTE:           result.Format(_T("%d"),m_data.m_dataINTERVAL.intval.day_second.minute);
                                          break;
    case SQL_C_INTERVAL_SECOND:           result.Format(_T("%d"),m_data.m_dataINTERVAL.intval.day_second.second);
                                          break;
    case SQL_C_INTERVAL_YEAR_TO_MONTH:    result.Format(_T("%d-%d")
                                                       ,m_data.m_dataINTERVAL.intval.year_month.year
                                                       ,m_data.m_dataINTERVAL.intval.year_month.month);
                                          break;
    case SQL_C_INTERVAL_DAY_TO_HOUR:      result.Format(_T("%d %d")
                                                       ,m_data.m_dataINTERVAL.intval.day_second.day
                                                       ,m_data.m_dataINTERVAL.intval.day_second.hour);
                                          break;
    case SQL_C_INTERVAL_DAY_TO_MINUTE:    result.Format(_T("%d %d:%d")
                                                        ,m_data.m_dataINTERVAL.intval.day_second.day
                                                        ,m_data.m_dataINTERVAL.intval.day_second.hour
                                                        ,m_data.m_dataINTERVAL.intval.day_second.minute);
                                          break;
    case SQL_C_INTERVAL_DAY_TO_SECOND:    result.Format(_T("%d %d:%d:%d")
                                                        ,m_data.m_dataINTERVAL.intval.day_second.day
                                                        ,m_data.m_dataINTERVAL.intval.day_second.hour
                                                        ,m_data.m_dataINTERVAL.intval.day_second.minute
                                                        ,m_data.m_dataINTERVAL.intval.day_second.second);
                                          break;
    case SQL_C_INTERVAL_HOUR_TO_MINUTE:   result.Format(_T("%d:%d")
                                                       ,m_data.m_dataINTERVAL.intval.day_second.hour
                                                       ,m_data.m_dataINTERVAL.intval.day_second.minute);
                                          break;
    case SQL_C_INTERVAL_HOUR_TO_SECOND:   result.Format(_T("%d:%d:%d")
                                                        ,m_data.m_dataINTERVAL.intval.day_second.hour
                                                        ,m_data.m_dataINTERVAL.intval.day_second.minute
                                                        ,m_data.m_dataINTERVAL.intval.day_second.second);
                                          break;
    case SQL_C_INTERVAL_MINUTE_TO_SECOND: result.Format(_T("%d:%d")
                                                        ,m_data.m_dataINTERVAL.intval.day_second.minute
                                                        ,m_data.m_dataINTERVAL.intval.day_second.second);
                                          break;
    default:                              result = _T("<NO VALUE>");
                                          break;
  }
  // SET THE INTERVAL SIGN
  if(m_datatype <= SQL_C_INTERVAL_MINUTE_TO_SECOND &&
     m_datatype >= SQL_C_INTERVAL_YEAR)
  {
    if(m_data.m_dataINTERVAL.interval_sign)
    {
      result = _T("-") + result;
    }
  }
}

const void*   
SQLVariant::GetDataPointer() const
{
  const void* data = NULL;
  switch(m_datatype)
  {
    case SQL_C_CHAR:                      data = reinterpret_cast<const void*>(m_data.m_dataCHAR);
                                          break;
    case SQL_C_WCHAR:                     data = reinterpret_cast<const void*>(m_data.m_dataWCHAR);
                                          break;
    case SQL_C_BINARY:                    data = reinterpret_cast<const void*>(m_data.m_dataBINARY);
                                          break;
    case SQL_C_SHORT:                     // Fall through
    case SQL_C_SSHORT:                    data = reinterpret_cast<const void*>(&m_data.m_dataSHORT);
                                          break;
    case SQL_C_USHORT:                    data = reinterpret_cast<const void*>(&m_data.m_dataUSHORT);
                                          break;
    case SQL_C_LONG:                      // Fall through
    case SQL_C_SLONG:                     data = reinterpret_cast<const void*>(&m_data.m_dataLONG);
                                          break;
    case SQL_C_ULONG:                     data = reinterpret_cast<const void*>(&m_data.m_dataULONG);
                                          break;
    case SQL_C_FLOAT:                     data = reinterpret_cast<const void*>(&m_data.m_dataFLOAT);
                                          break;
    case SQL_C_DOUBLE:                    data = reinterpret_cast<const void*>(&m_data.m_dataDOUBLE);
                                          break;
    case SQL_C_BIT:                       data = reinterpret_cast<const void*>(&m_data.m_dataBIT);
                                          break;
    case SQL_C_TINYINT:                   // Fall through
    case SQL_C_STINYINT:                  data = reinterpret_cast<const void*>(&m_data.m_dataTINYINT);
                                          break;
    case SQL_C_UTINYINT:                  data = reinterpret_cast<const void*>(&m_data.m_dataUTINYINT);
                                          break;
    case SQL_C_SBIGINT:                   data = reinterpret_cast<const void*>(&m_data.m_dataSBIGINT);
                                          break;
    case SQL_C_UBIGINT:                   data = reinterpret_cast<const void*>(&m_data.m_dataUBIGINT);
                                          break;
    case SQL_DECIMAL:                     // Fall through
    case SQL_C_NUMERIC:                   data = reinterpret_cast<const void*>(&m_data.m_dataNUMERIC);
                                          break;
    case SQL_C_GUID:                      data = reinterpret_cast<const void*>(&m_data.m_dataGUID);
                                          break;
    case SQL_C_DATE:                      // Fall through
    case SQL_C_TYPE_DATE:                 data = reinterpret_cast<const void*>(&m_data.m_dataDATE);
                                          break;
    case SQL_C_TIME:                      // Fall through
    case SQL_C_TYPE_TIME:                 data = reinterpret_cast<const void*>(&m_data.m_dataTIME);
                                          break;
    case SQL_C_TIMESTAMP:                 // Fall through
    case SQL_C_TYPE_TIMESTAMP:            data = reinterpret_cast<const void*>(&m_data.m_dataTIMESTAMP);
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
    case SQL_C_INTERVAL_MINUTE_TO_SECOND: data = reinterpret_cast<const void*>(&m_data.m_dataINTERVAL);
                                          break;
    default:                              break;
  }
  return data;
}

int     
SQLVariant::GetDataSize() const
{
  switch(m_datatype)
  {
    case SQL_C_CHAR:                      // Fall through
    case SQL_C_WCHAR:                     // Fall through
    case SQL_C_BINARY:                    return m_binaryLength;
    case SQL_C_SHORT:                     // Fall through
    case SQL_C_SSHORT:                    return sizeof(short);
    case SQL_C_USHORT:                    return sizeof(unsigned short);
    case SQL_C_LONG:                      // Fall through
    case SQL_C_SLONG:                     return sizeof(long);
    case SQL_C_ULONG:                     return sizeof(unsigned long);
    case SQL_C_FLOAT:                     return sizeof(float);
    case SQL_C_DOUBLE:                    return sizeof(double);
    case SQL_C_BIT:                       return sizeof(unsigned char);
    case SQL_C_TINYINT:                   // Fall through
    case SQL_C_STINYINT:                  return sizeof(char);
    case SQL_C_UTINYINT:                  return sizeof(unsigned char);
    case SQL_C_SBIGINT:                   // Fall through
    case SQL_C_UBIGINT:                   return sizeof(SQLBIGINT);
    case SQL_DECIMAL:                     // Fall through
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

// Here for backward compatibility reasons!!
// ONLY TO BE USED IN ANSI BUILDS !!!
char*
SQLVariant::GetAsChar() const
{
  if(m_indicator == SQL_NULL_DATA)
  {
    static char empty[1] = { 0 };
    return empty;
  }
  return m_data.m_dataCHAR;
}

XString
SQLVariant::GetAsString() const
{
  XString result;

  if(m_indicator == SQL_NULL_DATA)
  {
    return result;
  }
  GetAsString(result);
  return result;
}

void*
SQLVariant::GetAsBinary() const
{
  if(m_datatype == SQL_C_BINARY)
  {
    return m_data.m_dataBINARY;
  }
  if(m_datatype == SQL_C_CHAR)
  {
    return (void*)m_data.m_dataCHAR;
  }
  if(m_datatype == SQL_C_WCHAR)
  {
    return (void*)m_data.m_dataWCHAR;
  }
  // Use GetDataPointer instead
  return ThrowErrorDatatype(SQL_C_BINARY);
}

bool
SQLVariant::GetAsBoolean() const
{
  return GetAsSLong() != 0;
}

short
SQLVariant::GetAsSShort() const
{
  if(m_indicator == SQL_NULL_DATA)
  {
    return 0;
  }

  switch(m_datatype)
  {
    case SQL_C_CHAR:     return SQL_SLongToShort(atoi(m_data.m_dataCHAR));
    case SQL_C_WCHAR:    return SQL_SLongToShort(_wtoi(m_data.m_dataWCHAR));
    case SQL_C_SSHORT:   // fall through
    case SQL_C_SHORT:    return m_data.m_dataSHORT;
    case SQL_C_USHORT:   return SQL_UShortToShort(m_data.m_dataUSHORT);
    case SQL_C_LONG:     // fall through
    case SQL_C_SLONG:    return SQL_SLongToShort(m_data.m_dataLONG);
    case SQL_C_ULONG:    return SQL_ULongToShort(m_data.m_dataULONG);
    case SQL_C_FLOAT:    return SQL_FloatToShort(m_data.m_dataFLOAT);
    case SQL_C_DOUBLE:   return SQL_DoubleToShort(m_data.m_dataDOUBLE);
    case SQL_C_BIT:      return (short)m_data.m_dataBIT;
    case SQL_C_TINYINT:  // fall through
    case SQL_C_STINYINT: return (short)m_data.m_dataTINYINT;
    case SQL_C_UTINYINT: return (short)m_data.m_dataUTINYINT;
    case SQL_C_SBIGINT:  return SQL_BIGINTToShort(m_data.m_dataSBIGINT);
    case SQL_C_UBIGINT:  return SQL_UBIGINTToShort(m_data.m_dataUBIGINT);
    case SQL_DECIMAL:    // Fall through
    case SQL_C_NUMERIC:  { bcd num(&m_data.m_dataNUMERIC);
                           return SQL_SLongToShort(num.AsLong());
                         }
                         break;
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
  return 0;
}

unsigned short
SQLVariant::GetAsUShort() const
{
  if(m_indicator == SQL_NULL_DATA)
  {
    return 0;
  }

  switch(m_datatype)
  {
    case SQL_C_CHAR:     return SQL_SLongToUShort(atoi(m_data.m_dataCHAR));
    case SQL_C_WCHAR:    return SQL_SLongToUShort(_wtoi(m_data.m_dataWCHAR));
    case SQL_C_SSHORT:   // fall through
    case SQL_C_SHORT:    return SQL_ShortToUShort(m_data.m_dataSHORT);
    case SQL_C_USHORT:   return m_data.m_dataUSHORT;
    case SQL_C_LONG:     // fall through
    case SQL_C_SLONG:    return SQL_SLongToUShort(m_data.m_dataLONG);
    case SQL_C_ULONG:    return SQL_ULongToUShort(m_data.m_dataULONG);
    case SQL_C_FLOAT:    return SQL_FloatToUShort(m_data.m_dataFLOAT);
    case SQL_C_DOUBLE:   return SQL_DoubleToUShort(m_data.m_dataDOUBLE);
    case SQL_C_BIT:      return (unsigned short)m_data.m_dataBIT;
    case SQL_C_TINYINT:  // fall through
    case SQL_C_STINYINT: return SQL_TinyIntToUShort(m_data.m_dataTINYINT);
    case SQL_C_UTINYINT: return (unsigned short)m_data.m_dataUTINYINT;
    case SQL_C_SBIGINT:  return SQL_BIGINTToUShort(m_data.m_dataSBIGINT);
    case SQL_C_UBIGINT:  return SQL_UBIGINTToUShort(m_data.m_dataUBIGINT);
    case SQL_DECIMAL:    // Fall through
    case SQL_C_NUMERIC:  { bcd num(&m_data.m_dataNUMERIC);
                           return SQL_SLongToUShort(num.AsLong());
                         }
                         break;

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
  ThrowErrorDatatype(SQL_C_USHORT);
  return 0;
}

int
SQLVariant::GetAsSLong() const
{
  if(m_indicator == SQL_NULL_DATA)
  {
    return 0;
  }

  switch(m_datatype)
  {
    case SQL_C_CHAR:     return (long)atoi(m_data.m_dataCHAR);
    case SQL_C_WCHAR:    return (long)_wtoi(m_data.m_dataWCHAR);
    case SQL_C_SSHORT:   // fall through
    case SQL_C_SHORT:    return (long)m_data.m_dataSHORT;
    case SQL_C_USHORT:   return (long)m_data.m_dataUSHORT;
    case SQL_C_LONG:     // fall through
    case SQL_C_SLONG:    return m_data.m_dataLONG;
    case SQL_C_ULONG:    return SQL_ULongToLong(m_data.m_dataULONG);
    case SQL_C_FLOAT:    return SQL_FloatToLong(m_data.m_dataFLOAT);
    case SQL_C_DOUBLE:   return SQL_DoubleToLong(m_data.m_dataDOUBLE);
    case SQL_C_BIT:      return (long)m_data.m_dataBIT;
    case SQL_C_TINYINT:  // fall through
    case SQL_C_STINYINT: return (long)m_data.m_dataTINYINT;
    case SQL_C_UTINYINT: return (long)m_data.m_dataUTINYINT;
    case SQL_C_SBIGINT:  return SQL_BIGINTToLong(m_data.m_dataSBIGINT);
    case SQL_C_UBIGINT:  return SQL_UBIGINTToLong(m_data.m_dataUBIGINT);
    case SQL_DECIMAL:    // Fall through
    case SQL_C_NUMERIC:  { bcd num(&m_data.m_dataNUMERIC);
                           return num.AsLong();
                         }
                         break;
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
  ThrowErrorDatatype(SQL_C_SLONG);
  return 0L;
}


unsigned int
SQLVariant::GetAsULong() const
{
  if(m_indicator == SQL_NULL_DATA)
  {
    return 0;
  }

  switch(m_datatype)
  {
    case SQL_C_CHAR:     return SQL_LongToULong(atoi(m_data.m_dataCHAR));
    case SQL_C_WCHAR:    return SQL_LongToULong(_wtoi(m_data.m_dataWCHAR));
    case SQL_C_SSHORT:   // fall through
    case SQL_C_SHORT:    return SQL_ShortToULong(m_data.m_dataSHORT);
    case SQL_C_USHORT:   return (unsigned long)m_data.m_dataUSHORT;
    case SQL_C_LONG:     // fall through
    case SQL_C_SLONG:    return SQL_LongToULong(m_data.m_dataLONG);
    case SQL_C_ULONG:    return m_data.m_dataULONG;
    case SQL_C_FLOAT:    return SQL_FloatToULong(m_data.m_dataFLOAT);
    case SQL_C_DOUBLE:   return SQL_DoubleToULong(m_data.m_dataDOUBLE);
    case SQL_C_BIT:      return (unsigned long)m_data.m_dataBIT;
    case SQL_C_TINYINT:  // fall through
    case SQL_C_STINYINT: return SQL_TinyIntToULong(m_data.m_dataTINYINT);
    case SQL_C_UTINYINT: return (unsigned long)m_data.m_dataUTINYINT;
    case SQL_C_SBIGINT:  return SQL_BIGINTToULong(m_data.m_dataSBIGINT);
    case SQL_C_UBIGINT:  return SQL_UBIGINTToULong(m_data.m_dataUBIGINT);
    case SQL_DECIMAL:    // Fall through
    case SQL_C_NUMERIC:  { bcd num(&m_data.m_dataNUMERIC);
                           return SQL_BIGINTToULong(num.AsInt64());
                         }
                         break;

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
  ThrowErrorDatatype(SQL_C_ULONG);
  return 0UL;
}

float
SQLVariant::GetAsFloat() const
{
  if(m_indicator == SQL_NULL_DATA)
  {
    return 0.0;
  }

  switch(m_datatype)
  {
    case SQL_C_CHAR:     return SQL_DoubleToFloat(atof(m_data.m_dataCHAR));
    case SQL_C_WCHAR:    return SQL_DoubleToFloat(_wtof(m_data.m_dataWCHAR));
    case SQL_C_SSHORT:   // fall through
    case SQL_C_SHORT:    return (float)m_data.m_dataSHORT;
    case SQL_C_USHORT:   return (float)m_data.m_dataUSHORT;
    case SQL_C_LONG:     // fall through
    case SQL_C_SLONG:    return (float)m_data.m_dataLONG;
    case SQL_C_ULONG:    return (float)m_data.m_dataULONG;
    case SQL_C_FLOAT:    return m_data.m_dataFLOAT;
    case SQL_C_DOUBLE:   return SQL_DoubleToFloat(m_data.m_dataDOUBLE);
    case SQL_C_BIT:      return (float)m_data.m_dataBIT;
    case SQL_C_TINYINT:  // fall through
    case SQL_C_STINYINT: return (float)m_data.m_dataTINYINT;
    case SQL_C_UTINYINT: return (float)m_data.m_dataUTINYINT;
    case SQL_C_SBIGINT:  return (float)m_data.m_dataSBIGINT;
    case SQL_C_UBIGINT:  return (float)m_data.m_dataUBIGINT;
    case SQL_DECIMAL:    // Fall through
    case SQL_C_NUMERIC:  { bcd num(&m_data.m_dataNUMERIC); 
                           return (float)num.AsDouble();
                         }
                         break;
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
  return 0.0;
}

double
SQLVariant::GetAsDouble() const
{
  if(m_indicator == SQL_NULL_DATA)
  {
    return 0.0;
  }

  switch(m_datatype)
  {
    case SQL_C_CHAR:     return atof(m_data.m_dataCHAR);
    case SQL_C_WCHAR:    return _wtof(m_data.m_dataWCHAR);
    case SQL_C_SSHORT:   // fall through
    case SQL_C_SHORT:    return (double)m_data.m_dataSHORT;
    case SQL_C_USHORT:   return (double)m_data.m_dataUSHORT;
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
    case SQL_DECIMAL:    // Fall through
    case SQL_C_NUMERIC:  { bcd num(&m_data.m_dataNUMERIC);
                           return num.AsDouble();
                         }
                         break;
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
  ThrowErrorDatatype(SQL_C_DOUBLE);
  // We never come here, but this is to prevent 
  // Warning C4715 not all control paths return a value
  // In various versions of the MSC++ compiler
  return NULL;
}

char
SQLVariant::GetAsBit() const
{
  if(m_indicator == SQL_NULL_DATA)
  {
    return 0;
  }

  switch(m_datatype)
  {
    case SQL_C_CHAR:     return m_data.m_dataCHAR[0]  != 0;
    case SQL_C_WCHAR:    return m_data.m_dataWCHAR[0] != 0;
    case SQL_C_SSHORT:   // fall through
    case SQL_C_SHORT:    return m_data.m_dataSHORT    != 0;
    case SQL_C_USHORT:   return m_data.m_dataUSHORT   != 0;
    case SQL_C_LONG:     // fall through
    case SQL_C_SLONG:    return m_data.m_dataLONG     != 0;
    case SQL_C_ULONG:    return m_data.m_dataULONG    != 0;
    case SQL_C_FLOAT:    return m_data.m_dataFLOAT  < -FLT_EPSILON || FLT_EPSILON < m_data.m_dataFLOAT;
    case SQL_C_DOUBLE:   return m_data.m_dataDOUBLE < -DBL_EPSILON || DBL_EPSILON < m_data.m_dataDOUBLE;
    case SQL_C_BIT:      return m_data.m_dataBIT;
    case SQL_C_TINYINT:  // fall through
    case SQL_C_STINYINT: return m_data.m_dataTINYINT  != 0;
    case SQL_C_UTINYINT: return m_data.m_dataUTINYINT != 0;
    case SQL_C_SBIGINT:  return m_data.m_dataSBIGINT  != 0L;
    case SQL_C_UBIGINT:  return m_data.m_dataUBIGINT  != 0L;
    case SQL_DECIMAL:    // Fall through
    case SQL_C_NUMERIC:  { bcd num(&m_data.m_dataNUMERIC);
                           return num.AsLong() != 0;
                         }
                         break;

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
  ThrowErrorDatatype(SQL_C_BIT);
  // We never come here, but this is to prevent 
  // Warning C4715 not all control paths return a value
  // In various versions of the MSC++ compiler
  return NULL;
}

char
SQLVariant::GetAsSTinyInt() const
{
  if(m_indicator == SQL_NULL_DATA)
  {
    return 0;
  }

  switch(m_datatype)
  {
    case SQL_C_CHAR:     return SQL_SLongToTinyInt(atoi(m_data.m_dataCHAR));
    case SQL_C_WCHAR:    return SQL_SLongToTinyInt(_wtoi(m_data.m_dataWCHAR));
    case SQL_C_SSHORT:   // fall through
    case SQL_C_SHORT:    return SQL_ShortToTinyInt(m_data.m_dataSHORT);
    case SQL_C_USHORT:   return SQL_UShortToTinyInt(m_data.m_dataUSHORT);
    case SQL_C_LONG:     // fall through
    case SQL_C_SLONG:    return SQL_SLongToTinyInt(m_data.m_dataLONG);
    case SQL_C_ULONG:    return SQL_ULongToTinyInt(m_data.m_dataULONG);
    case SQL_C_FLOAT:    return SQL_FloatToTinyInt(m_data.m_dataFLOAT);
    case SQL_C_DOUBLE:   return SQL_DoubleToTinyInt(m_data.m_dataDOUBLE);
    case SQL_C_BIT:      return m_data.m_dataBIT;
    case SQL_C_TINYINT:  // fall through
    case SQL_C_STINYINT: return m_data.m_dataTINYINT;
    case SQL_C_UTINYINT: return SQL_UTinyIntToTinyInt(m_data.m_dataUTINYINT);
    case SQL_C_SBIGINT:  return SQL_SBIGINTToTinyInt(m_data.m_dataSBIGINT);
    case SQL_C_UBIGINT:  return SQL_UBIGINTToTinyInt(m_data.m_dataUBIGINT);
    case SQL_DECIMAL:    // Fall through
    case SQL_C_NUMERIC:  { bcd num(&m_data.m_dataNUMERIC);
                           return SQL_SLongToTinyInt(num.AsLong());
                         }
                         break;

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
  ThrowErrorDatatype(SQL_C_STINYINT);
  // We never come here, but this is to prevent 
  // Warning C4715 not all control paths return a value
  // In various versions of the MSC++ compiler
  return NULL;
}

unsigned char
SQLVariant::GetAsUTinyInt() const
{
  if(m_indicator == SQL_NULL_DATA)
  {
    return 0;
  }

  switch(m_datatype)
  {
    case SQL_C_CHAR:     return SQL_SLongToUTinyInt(atoi(m_data.m_dataCHAR));
    case SQL_C_WCHAR:    return SQL_SLongToUTinyInt(_wtoi(m_data.m_dataWCHAR));
    case SQL_C_SSHORT:   // fall through
    case SQL_C_SHORT:    return SQL_ShortToUTinyInt(m_data.m_dataSHORT);
    case SQL_C_USHORT:   return SQL_UShortToUTinyInt(m_data.m_dataUSHORT);
    case SQL_C_LONG:     // fall through
    case SQL_C_SLONG:    return SQL_SLongToUTinyInt(m_data.m_dataLONG);
    case SQL_C_ULONG:    return SQL_ULongToUTinyInt(m_data.m_dataULONG);
    case SQL_C_FLOAT:    return SQL_FloatToUTinyInt(m_data.m_dataFLOAT);
    case SQL_C_DOUBLE:   return SQL_DoubleToUTinyInt(m_data.m_dataDOUBLE);
    case SQL_C_BIT:      return (unsigned char) m_data.m_dataBIT;
    case SQL_C_TINYINT:  // fall through
    case SQL_C_STINYINT: return SQL_TinyIntToUTinyInt(m_data.m_dataTINYINT);
    case SQL_C_UTINYINT: return m_data.m_dataUTINYINT;
    case SQL_C_SBIGINT:  return SQL_SBIGINTToUTinyInt(m_data.m_dataSBIGINT);
    case SQL_C_UBIGINT:  return SQL_UBIGINTToUTinyInt(m_data.m_dataUBIGINT);
    case SQL_DECIMAL:    // Fall through
    case SQL_C_NUMERIC:  { bcd num(&m_data.m_dataNUMERIC);
                           return SQL_SLongToUTinyInt(num.AsLong());
                         }
                         break;
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
  ThrowErrorDatatype(SQL_C_UTINYINT);
  // We never come here, but this is to prevent 
  // Warning C4715 not all control paths return a value
  // In various versions of the MSC++ compiler
  return NULL;
}

SQLBIGINT
SQLVariant::GetAsSBigInt() const
{
  if(m_indicator == SQL_NULL_DATA)
  {
    return 0;
  }

  switch(m_datatype)
  {
    case SQL_C_CHAR:     return _atoi64(m_data.m_dataCHAR);
    case SQL_C_WCHAR:    return _wtoi64(m_data.m_dataWCHAR);
    case SQL_C_SSHORT:   // fall through
    case SQL_C_SHORT:    return (SQLBIGINT)m_data.m_dataSHORT;
    case SQL_C_USHORT:   return (SQLBIGINT)m_data.m_dataUSHORT;
    case SQL_C_LONG:     // fall through
    case SQL_C_SLONG:    return (SQLBIGINT)m_data.m_dataLONG;
    case SQL_C_ULONG:    return (SQLBIGINT)(m_data.m_dataULONG);
    case SQL_C_FLOAT:    return SQL_FloatToBIGINT(m_data.m_dataFLOAT);
    case SQL_C_DOUBLE:   return SQL_DoubleToBIGINT(m_data.m_dataDOUBLE);
    case SQL_C_BIT:      return (SQLBIGINT)m_data.m_dataBIT;
    case SQL_C_TINYINT:  // fall through
    case SQL_C_STINYINT: return (SQLBIGINT)m_data.m_dataTINYINT;
    case SQL_C_UTINYINT: return (SQLBIGINT)m_data.m_dataUTINYINT;
    case SQL_C_SBIGINT:  return m_data.m_dataSBIGINT;
    case SQL_C_UBIGINT:  return SQL_UBIGINTToBIGINT(m_data.m_dataUBIGINT);
    case SQL_DECIMAL:    // Fall through
    case SQL_C_NUMERIC:  { bcd num(&m_data.m_dataNUMERIC);
                           return num.AsInt64();
                         }
                         break;
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
  ThrowErrorDatatype(SQL_C_SBIGINT);
  // We never come here, but this is to prevent 
  // Warning C4715 not all control paths return a value
  // In various versions of the MSC++ compiler
  return NULL;
}

SQLUBIGINT
SQLVariant::GetAsUBigInt() const
{
  if(m_indicator == SQL_NULL_DATA)
  {
    return 0;
  }

  switch(m_datatype)
  {
    case SQL_C_CHAR:     return SQL_SBIGINTToUBIGINT(_atoi64(m_data.m_dataCHAR));
    case SQL_C_WCHAR:    return SQL_SBIGINTToUBIGINT(_wtoi64(m_data.m_dataWCHAR));
    case SQL_C_SSHORT:   // fall through
    case SQL_C_SHORT:    return SQL_ShortToUBIGINT(m_data.m_dataSHORT);
    case SQL_C_USHORT:   return (SQLUBIGINT)m_data.m_dataUSHORT;
    case SQL_C_LONG:     // fall through
    case SQL_C_SLONG:    return SQL_LongToUBIGINT(m_data.m_dataLONG);
    case SQL_C_ULONG:    return (SQLUBIGINT)(m_data.m_dataULONG);
    case SQL_C_FLOAT:    return SQL_FloatToUBIGINT(m_data.m_dataFLOAT);
    case SQL_C_DOUBLE:   return SQL_DoubleToUBIGINT(m_data.m_dataDOUBLE);
    case SQL_C_BIT:      return (SQLUBIGINT)m_data.m_dataBIT;
    case SQL_C_TINYINT:  // fall through
    case SQL_C_STINYINT: return SQL_TinyIntToUBIGINT(m_data.m_dataTINYINT);
    case SQL_C_UTINYINT: return (SQLUBIGINT)m_data.m_dataUTINYINT;
    case SQL_C_SBIGINT:  return SQL_SBIGINTToUBIGINT(m_data.m_dataSBIGINT);
    case SQL_C_UBIGINT:  return m_data.m_dataUBIGINT;
    case SQL_DECIMAL:    // Fall through
    case SQL_C_NUMERIC:  { bcd num(&m_data.m_dataNUMERIC);
                           return num.AsUInt64();
                         }
                         break;
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
  ThrowErrorDatatype(SQL_C_UBIGINT);
  // We never come here, but this is to prevent 
  // Warning C4715 not all control paths return a value
  // In various versions of the MSC++ compiler
  return NULL; 
}

__declspec(thread) static SQL_NUMERIC_STRUCT g_number;

const SQL_NUMERIC_STRUCT*
SQLVariant::GetAsNumeric() const
{
  if(m_datatype == SQL_C_NUMERIC)
  {
    return &m_data.m_dataNUMERIC;
  }
  // Sometimes we come her unexpectedly in various programs
  switch(m_datatype)
  {
    case SQL_C_CHAR:      { // VARCHAR in the RDBMS
#ifdef UNICODE
                          bcd num(CA2W(m_data.m_dataCHAR));
#else
                          bcd num(m_data.m_dataCHAR);
#endif
                          num.AsNumeric(&g_number);
                          return &g_number;
                          }
                          break;
    case SQL_C_WCHAR:     {
                          }
                          break;
    case SQL_C_SSHORT:    // Fall through
    case SQL_C_SHORT:     { bcd num(m_data.m_dataSHORT);
                            num.AsNumeric(&g_number);
                            return &g_number;
                          }
                          break;
    case SQL_C_USHORT:    { bcd num((int)m_data.m_dataUSHORT);
                            num.AsNumeric(&g_number);
                            return &g_number;
                          }
                          break;
    case SQL_C_LONG:      // Fall through
    case SQL_C_SLONG:     { bcd num(m_data.m_dataLONG);
                            num.AsNumeric(&g_number);
                            return &g_number;
                          }
                          break;
    case SQL_C_ULONG:     { bcd num((int64)m_data.m_dataULONG);
                            num.AsNumeric(&g_number);
                            return &g_number;
                          }
                          break;
    case SQL_C_FLOAT:     { bcd num((double)m_data.m_dataFLOAT);
                            num.AsNumeric(&g_number);
                            return &g_number;
                          }
                          break;
    case SQL_C_DOUBLE:    { bcd num(m_data.m_dataDOUBLE);
                            num.AsNumeric(&g_number);
                            return &g_number;
                          }
                          break;
    case SQL_C_BIT:       { bcd num(m_data.m_dataBIT);
                            num.AsNumeric(&g_number);
                            return &g_number;
                          }
                          break;
    case SQL_C_TINYINT:   // Fall through
    case SQL_C_STINYINT:  { bcd num(m_data.m_dataSTINYINT);
                            num.AsNumeric(&g_number);
                            return &g_number;
                          }
                          break;
    case SQL_C_UTINYINT:  { bcd num(m_data.m_dataUTINYINT);
                            num.AsNumeric(&g_number);
                            return &g_number;
                          }
                          break;
    case SQL_C_SBIGINT:   { bcd num(m_data.m_dataSBIGINT);
                            num.AsNumeric(&g_number);
                            return &g_number;
                          }
                          break;
    case SQL_C_UBIGINT:   { bcd num(m_data.m_dataUBIGINT);
                            num.AsNumeric(&g_number);
                            return &g_number;
                          }
                          break;
  }
  // Other datatypes cannot convert
  ThrowErrorDatatype(SQL_C_NUMERIC);
  return NULL;
}

const SQLGUID*
SQLVariant::GetAsGUID() const
{
  if(m_datatype == SQL_C_GUID)
  {
    return &m_data.m_dataGUID;
  }
  ThrowErrorDatatype(SQL_C_GUID);
  return NULL;
}

__declspec(thread) static DATE_STRUCT g_date;

const DATE_STRUCT*
SQLVariant::GetAsDate() const
{
  if(m_datatype == SQL_C_DATE || m_datatype == SQL_C_TYPE_DATE)
  {
    return &m_data.m_dataDATE;
  }
  if(m_datatype == SQL_C_TIMESTAMP || m_datatype == SQL_C_TYPE_TIMESTAMP)
  {
    return (DATE_STRUCT *) &m_data.m_dataTIMESTAMP;
  }
  if(m_datatype == SQL_C_CHAR ||
     m_datatype == SQL_C_WCHAR)
  {
    try
    {
      XString strdate = GetAsString();
      SQLDate date(strdate);
      memset(&g_date,0,sizeof(DATE_STRUCT));
      date.AsDateStruct(&g_date);
    }
    catch(StdException& er)
    {
      ReThrowSafeException(er);
    }
    return &g_date;
  }
  ThrowErrorDatatype(SQL_C_DATE);
  return NULL;
}

__declspec(thread) static TIME_STRUCT g_time;

const TIME_STRUCT*
SQLVariant::GetAsTime() const
{
  if(m_datatype == SQL_C_TIME || m_datatype == SQL_C_TYPE_TIME)
  {
    return &m_data.m_dataTIME;
  }
  if(m_datatype == SQL_C_TIMESTAMP || m_datatype == SQL_C_TYPE_TIMESTAMP)
  {
    return (TIME_STRUCT*)&m_data.m_dataTIMESTAMP.hour;
  }
  if(m_datatype == SQL_C_CHAR ||
     m_datatype == SQL_C_WCHAR)
  {
    try
    {
      XString strtime = GetAsString();
      SQLTime time(strtime);
      memset(&g_time,0,sizeof(TIME_STRUCT));
      time.AsTimeStruct(&g_time);
    }
    catch(StdException& er)
    {
      ReThrowSafeException(er);
    }
    return &g_time;
  }
  ThrowErrorDatatype(SQL_C_TIME);
  return NULL;
}

__declspec(thread) static TIMESTAMP_STRUCT g_timestamp;

const TIMESTAMP_STRUCT*
SQLVariant::GetAsTimestamp() const
{
  if(m_datatype == SQL_C_TIMESTAMP || m_datatype == SQL_C_TYPE_TIMESTAMP)
  {
    return &m_data.m_dataTIMESTAMP;
  }
  if(m_datatype == SQL_C_DATE || m_datatype == SQL_C_TYPE_DATE)
  {
    ASSERT(m_data.m_dataTIMESTAMP.hour     == 0);
    ASSERT(m_data.m_dataTIMESTAMP.minute   == 0);
    ASSERT(m_data.m_dataTIMESTAMP.second   == 0);
    ASSERT(m_data.m_dataTIMESTAMP.fraction == 0);
    return &m_data.m_dataTIMESTAMP;
  }
  if(m_datatype == SQL_C_CHAR ||
     m_datatype == SQL_C_WCHAR)
  {
    try
    {
      XString strstamp = GetAsString();
      SQLTimestamp stamp(strstamp);
      memset(&g_timestamp,0,sizeof(TIMESTAMP_STRUCT));
      stamp.AsTimeStampStruct(&g_timestamp);
    }
    catch(StdException& er)
    {
      ReThrowSafeException(er);
    }
    return &g_timestamp;
  }
  ThrowErrorDatatype(SQL_C_TIMESTAMP);
  return NULL;
}

// European timestamp has the day-month-year order
// instead of the standard 'year-month-day' order of a timestamp
const XString
SQLVariant::GetAsEuropeanTimestamp() const
{
  XString result;
  if(m_datatype == SQL_C_TIMESTAMP ||
     m_datatype == SQL_C_TYPE_TIMESTAMP)
  {
    result.Format(_T("%02.2d-%02.2d-%04.4d %02.2d:%02.2d:%02.2d")
                 ,m_data.m_dataTIMESTAMP.day
                 ,m_data.m_dataTIMESTAMP.month
                 ,m_data.m_dataTIMESTAMP.year
                 ,m_data.m_dataTIMESTAMP.hour
                 ,m_data.m_dataTIMESTAMP.minute
                 ,m_data.m_dataTIMESTAMP.second);
    if(m_data.m_dataTIMESTAMP.fraction)
    {
      // Nanosecond resolution
      XString frac;
      frac.Format(_T("%0.6f"),(double)m_data.m_dataTIMESTAMP.fraction / 1000000000.0);
      result += frac.Mid(1);
    }
  }
  else if(m_datatype == SQL_C_DATE ||
          m_datatype == SQL_C_TYPE_DATE)
  {
    result.Format(_T("%02.2d-%02.2d-%04.4d")
                 ,m_data.m_dataDATE.day
                 ,m_data.m_dataDATE.month
                 ,m_data.m_dataDATE.year);
  }
  else if(m_datatype == SQL_C_TIME ||
          m_datatype == SQL_C_TYPE_TIME)
  {
    result.Format(_T("%02.2d:%02.2d:%02.2d")
                 ,m_data.m_dataTIME.hour
                 ,m_data.m_dataTIME.minute
                 ,m_data.m_dataTIME.second);
  }
  return result;
}

const SQL_INTERVAL_STRUCT*
SQLVariant::GetAsInterval() const
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
SQLVariant::GetAsSQLDate() const
{
  if(IsNULL())
  {
    return SQLDate();
  }
  const DATE_STRUCT* date = GetAsDate();
  return SQLDate(date->day,date->month,date->year);
}

SQLTime
SQLVariant::GetAsSQLTime() const
{
  if(IsNULL())
  {
    return SQLTime();
  }
  const TIME_STRUCT* time = GetAsTime();
  return SQLTime(time->hour,time->minute,time->second);
}

SQLTimestamp
SQLVariant::GetAsSQLTimestamp() const
{
  if(IsNULL())
  {
    return SQLTimestamp();
  }
  const TIMESTAMP_STRUCT* stamp = GetAsTimestamp();
  return SQLTimestamp(stamp->year,stamp->month,stamp->day
                     ,stamp->hour,stamp->minute,stamp->second
                     ,stamp->fraction);
}

SQLInterval
SQLVariant::GetAsSQLInterval() const
{
  if(IsNULL())
  {
    SQLInterval intval;
    return intval;
  }
  SQLInterval interval(&m_data.m_dataINTERVAL);
  return interval;
}

SQLGuid
SQLVariant::GetAsSQLGuid() const
{
  if(IsNULL())
  {
    SQLGuid guid;
    return guid;
  }
  SQLGuid guid(&m_data.m_dataGUID);
  return guid;
}

int
SQLVariant::GetFraction() const
{
  switch(m_datatype)
  {
    case SQL_C_TIMESTAMP:       // Fall through
    case SQL_C_TYPE_TIMESTAMP:  return 6;
    case SQL_C_NUMERIC:         return m_data.m_dataNUMERIC.scale;
  }
  return 0;
}

// Only for SQL_NUMERIC: The precision
int     
SQLVariant::GetNumericPrecision() const
{
  if(m_datatype == SQL_C_NUMERIC)
  {
    return m_data.m_dataNUMERIC.precision;
  }
  throw StdException("Cannot get the numeric precision of this datatype");
}

// Only for SQL_NUMERIC: The scale
int
SQLVariant::GetNumericScale() const
{
  if(m_datatype == SQL_C_NUMERIC)
  {
    return m_data.m_dataNUMERIC.scale;
  }
  // Cannot get the numeric scale of this datatype
  return 0;
}

bcd
SQLVariant::GetAsBCD() const
{
  if(m_indicator == SQL_NULL_DATA)
  {
    return bcd();
  }

  switch(m_datatype)
  {
    case SQL_C_CHAR:     // Fall through
    case SQL_C_WCHAR:    { XString num = GetAsString();
                           return bcd(num);
                         }
                         break;
    case SQL_C_SSHORT:   // fall through
    case SQL_C_SHORT:    return bcd(m_data.m_dataSHORT);
    case SQL_C_USHORT:   return bcd(m_data.m_dataUSHORT);
    case SQL_C_LONG:     // fall through
    case SQL_C_SLONG:    return bcd(m_data. m_dataLONG);
    case SQL_C_ULONG:    return bcd((int64) m_data.m_dataULONG);
    case SQL_C_FLOAT:    return bcd((double)m_data.m_dataFLOAT);
    case SQL_C_DOUBLE:   return bcd(m_data.m_dataDOUBLE);
    case SQL_C_BIT:      return bcd(m_data.m_dataBIT);
    case SQL_C_TINYINT:  // fall through
    case SQL_C_STINYINT: return bcd(m_data.m_dataTINYINT);
    case SQL_C_UTINYINT: return bcd((short)m_data.m_dataUTINYINT);
    case SQL_C_SBIGINT:  return bcd(m_data.m_dataSBIGINT);
    case SQL_C_UBIGINT:  return bcd(m_data.m_dataUBIGINT);
    case SQL_C_NUMERIC:  return bcd(&m_data.m_dataNUMERIC);

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
  ThrowErrorDatatype(SQL_C_NUMERIC);
  // We never come here, but this is to prevent 
  // Warning C4715 not all control paths return a value
  // In various versions of the MSC++ compiler
  return bcd(); 
}

// Get the data as an string for a SQL expression or condition
// So quotes and ODBC escapes matter
XString
SQLVariant::GetAsSQLString() const
{
  XString value;
  GetAsString(value);

  switch(m_datatype)
  {
    case SQL_C_CHAR:            // Fallthrough (conversion already done)
    case SQL_C_WCHAR:           value = _T("'") + value + _T("'");
                                break;
    case SQL_C_GUID:            value.Replace(_T("{"),_T("{guid '"));
                                value.Replace(_T("}"),_T("'}"));
                                break;
    case SQL_C_DATE:            // Fall through: Reformat
    case SQL_C_TYPE_DATE:       value.Format(_T("{d '%04d-%02d-%02d'}")
                                            ,m_data.m_dataDATE.year
                                            ,m_data.m_dataDATE.month
                                            ,m_data.m_dataDATE.day);
                                break;
    case SQL_C_TIME:            // Fall through
    case SQL_C_TYPE_TIME:       value = _T("{t '") + value + _T("'}");
                                break;
    case SQL_C_TIMESTAMP:
    case SQL_C_TYPE_TIMESTAMP:  value = _T("{ts '") + value + _T("'}");
                                break;
    case SQL_C_INTERVAL_YEAR:             value = _T("{INTERVAL '") + value + _T("' YEAR}");
                                          break;
    case SQL_C_INTERVAL_MONTH:            value = _T("{INTERVAL '") + value + _T("' MONTH}");
                                          break;
    case SQL_C_INTERVAL_YEAR_TO_MONTH:    value = _T("{INTERVAL '") + value + _T("' YEAR TO MONTH}");
                                          break;
    case SQL_C_INTERVAL_DAY:              value = _T("{INTERVAL '") + value + _T("' DAY}");
                                          break;
    case SQL_C_INTERVAL_HOUR:             value = _T("{INTERVAL '") + value + _T("' HOUR}");
                                          break;
    case SQL_C_INTERVAL_MINUTE:           value = _T("{INTERVAL '") + value + _T("' MINUTE}");
                                          break;
    case SQL_C_INTERVAL_SECOND:           value = _T("{INTERVAL '") + value + _T("' SECOND}");
                                          break;
    case SQL_C_INTERVAL_DAY_TO_HOUR:      value = _T("{INTERVAL '") + value + _T("' DAY TO HOUR}");
                                          break;
    case SQL_C_INTERVAL_DAY_TO_MINUTE:    value = _T("{INTERVAL '") + value + _T("' DAY TO MINUTE}");
                                          break;
    case SQL_C_INTERVAL_DAY_TO_SECOND:    value = _T("{INTERVAL '") + value + _T("' DAY TO SECOND}");
                                          break;
    case SQL_C_INTERVAL_HOUR_TO_MINUTE:   value = _T("{INTERVAL '") + value + _T("' HOUR TO MINUTE}");
                                          break;
    case SQL_C_INTERVAL_HOUR_TO_SECOND:   value = _T("{INTERVAL '") + value + _T("' HOUR TO SECOND}");
                                          break;
    case SQL_C_INTERVAL_MINUTE_TO_SECOND: value = _T("{INTERVAL '") + value + _T("' MINUTE TO SECOND}");
                                          break;
  }
  return value;
}

// Some databases (Oracle) need to know the size of the data member
// for an AT_EXEC operation beforehand in the indicator
void
SQLVariant::SetSizeIndicator(bool p_realSize,bool p_binary /* = false */)
{
  __int64 size = (__int64) GetDataSize();
  if(size > 0 && p_realSize)
  {
    if(p_binary)
    {
      // Special ODBC macro to set the data size
      // SQL_BINARY
      // SQL_LONGVARCHAR
      // SQL_LONGVARBINARY
      // SQL_WLONGVARCHAR
      m_indicator = (SQLLEN) SQL_LEN_DATA_AT_EXEC(size);
    }
    else
    {
      // Simply the AT_EXEC   indicator
      m_indicator = SQL_DATA_AT_EXEC;
    }
  }
  m_useAtExec = true;
}

// Truncation of a char field
void
SQLVariant::TruncateCharacter()
{
  // Only for character data types
  if(m_datatype != SQL_C_CHAR && m_datatype != SQL_C_WCHAR)
  {
    return;
  }
  // Only non-null char data and not streaming mode
  if(m_indicator == SQL_NULL_DATA || m_useAtExec || m_indicator < 0)
  {
    return;
  }
  // Loop over the char data, truncating from right-to-left
  if(m_datatype == SQL_C_CHAR)
  {
    size_t dataLength = strlen(m_data.m_dataCHAR);
    TruncateSpace((unsigned) (dataLength));
  }
  else
  {
    size_t dataLength = wcslen(m_data.m_dataWCHAR);
    TruncateSpace((unsigned) (dataLength));
  }
}

// Truncation of a char field
void
SQLVariant::TruncateCharacterReset()
{
  // Only non-null char data and not streaming mode
  if(m_datatype != SQL_C_CHAR && m_datatype != SQL_C_WCHAR)
  {
    return;
  }
  // Restore from binary piece
  if(m_binaryPieceSize > m_binaryLength)
  {
    // Restore the heap size
    m_binaryLength    = m_binaryPieceSize;
    m_binaryPieceSize = 0;
  }
}

// Truncating the TIMESTAMP fraction
// Done for conversions between databases
// Truncate the timestamps to a number of decimals (0 - 6)
void
SQLVariant::TruncateTimestamp(int p_decimals /*=0*/)
{
  if(m_datatype != SQL_C_TIMESTAMP && m_datatype != SQL_C_TYPE_TIMESTAMP)
  {
    return;
  }
  if(p_decimals > 0 && p_decimals <= 6)
  {
    // Rounding to this number of decimals
    int val = static_cast<int>(pow(10,(6 - p_decimals)));
    m_data.m_dataTIMESTAMP.fraction -= m_data.m_dataTIMESTAMP.fraction % val;
  }
  else
  {
    m_data.m_dataTIMESTAMP.fraction = 0;
  }
}

bool    
SQLVariant::SetData(int p_type,LPCTSTR p_data)
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
  if(p_type != SQL_C_CHAR || p_type == SQL_C_WCHAR)
  {
    while(isspace(p_data[0]))
    {
      // Trim the leading spaces
      // So we can translate whatever is in the string
      ++p_data;
    }
  }
  size_t dataLen = _tcslen(p_data);

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
    case SQL_C_CHAR:
#ifdef UNICODE
                                          { 
                                            // Getting the length of the translation buffer first
                                            int clength = ::WideCharToMultiByte(GetACP(),0,p_data,-1,NULL,0,NULL,NULL);
                                            m_data.m_dataCHAR = new char[clength + 1];
                                            int blength = ::WideCharToMultiByte(GetACP(),0,p_data,clength,(LPSTR)m_data.m_dataCHAR,clength,NULL,NULL);
                                            m_data.m_dataCHAR[clength] = 0;
                                            if(blength > 0 && blength < clength)
                                            {
                                              m_data.m_dataCHAR[blength] = 0;
                                            }
                                            m_binaryLength = blength;
                                            m_indicator = blength > 0 ? SQL_NTS : SQL_NULL_DATA;
                                          }
#else
                                          m_binaryLength = (int)(dataLen);
                                          m_data.m_dataCHAR = new char[(size_t)m_binaryLength + 1];
                                          strcpy_s(m_data.m_dataCHAR, (size_t)m_binaryLength + 1,p_data);
                                          m_indicator = dataLen > 0 ? SQL_NTS : SQL_NULL_DATA;
#endif
                                          break;
    case SQL_C_WCHAR:
#ifdef UNICODE
                                          m_binaryLength = (int)(dataLen * 2);
                                          m_data.m_dataWCHAR = new TCHAR[(size_t)m_binaryLength + 1];
                                          _tcscpy_s(m_data.m_dataWCHAR,(size_t)m_binaryLength + 1,p_data);
                                          m_indicator = dataLen > 0 ? SQL_NTS : SQL_NULL_DATA;
#else
                                          {
                                            // Getting the needed buffer space (in codepoints! Not bytes!!)
                                            int length = MultiByteToWideChar(GetACP(),0,p_data,-1,NULL,NULL);
                                            m_data.m_dataWCHAR = new wchar_t[(size_t)length + 1];
                                            MultiByteToWideChar(GetACP(),0,p_data,-1,reinterpret_cast<LPWSTR>(m_data.m_dataWCHAR),length);
                                            m_data.m_dataWCHAR[length] = 0;
                                            m_binaryLength = 2 * length;
                                            m_indicator = length > 0 ? SQL_NTS : SQL_NULL_DATA;
                                          }
#endif
                                          break;
    case SQL_C_BINARY:                    m_binaryLength = (int)(dataLen / 2);
                                          m_indicator    = m_binaryLength > 0 ? m_binaryLength : SQL_NULL_DATA;
                                          m_data.m_dataBINARY = new BYTE[(size_t)m_binaryLength + 2];
                                          StringToBinary((const char*)p_data);
                                          break;
    case SQL_C_SHORT:                     // Fall through
    case SQL_C_SSHORT:                    m_data.m_dataSHORT = (short)_ttoi(p_data);
                                          break;
    case SQL_C_USHORT:                    m_data.m_dataUSHORT = (short)_ttoi(p_data);
                                          break;
    case SQL_C_LONG:                      // Fall through
    case SQL_C_SLONG:                     m_data.m_dataLONG = (long)_ttoi(p_data);
                                          break;
    case SQL_C_ULONG:                     scannum = _stscanf_s(p_data,_T("%lu"),&m_data.m_dataULONG);
                                          if(scannum != 1)
                                          {
                                            m_data.m_dataULONG = NULL;
                                          }
                                          break;
    case SQL_C_FLOAT:                     scannum = _stscanf_s(p_data,_T("%f"),&m_data.m_dataFLOAT);
                                          if(scannum != 1)
                                          {
                                            m_data.m_dataFLOAT = NULL;
                                          }
                                          break;
    case SQL_C_DOUBLE:                    scannum = _stscanf_s(p_data,_T("%lf"),&m_data.m_dataDOUBLE);
                                          if(scannum != 1)
                                          {
                                            m_data.m_dataDOUBLE = NULL;
                                          }
                                          break;
    case SQL_C_BIT:                       m_data.m_dataBIT = (_ttoi(p_data) == 1);
                                          break;
    case SQL_C_TINYINT:                   // Fall through
    case SQL_C_STINYINT:                  m_data.m_dataTINYINT = (char)_ttoi(p_data);
                                          break;
    case SQL_C_UTINYINT:                  m_data.m_dataTINYINT = (unsigned char)_ttoi(p_data);
                                          break;
    case SQL_C_SBIGINT:                   scannum = _stscanf_s(p_data,_T("%I64d"),&m_data.m_dataSBIGINT);
                                          if(scannum != 1)
                                          {
                                            m_data.m_dataSBIGINT = NULL;
                                          }
                                          break;
    case SQL_C_UBIGINT:                   scannum = _stscanf_s(p_data,_T("%I64u"),&m_data.m_dataUBIGINT);
                                          if(scannum != 1)
                                          {
                                            m_data.m_dataUBIGINT = NULL;
                                          }
                                          break;
    case SQL_C_NUMERIC:                   {  bcd num(p_data);
                                             num.AsNumeric(&m_data.m_dataNUMERIC);
                                          }
                                          break;
    case SQL_C_GUID:                      //aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee 
                                          scannum = _stscanf_s(p_data,_T("{%lX-%hX-%hX-%2hhX%2hhX-%2hhX%2hhX%2hhX%2hhX%2hhX%2hhX}")
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
    case SQL_C_TYPE_DATE:                 scannum = _stscanf_s(p_data,_T("%d-%d-%d"),&day,&month,&year);
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
    case SQL_C_TYPE_TIME:                 scannum = _stscanf_s(p_data,_T("%d:%d:%d"),&hour,&min,&sec);
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
    case SQL_C_TYPE_TIMESTAMP:            scannum = _stscanf_s(p_data,_T("%d-%d-%d %d:%d:%lf")
                                                              ,&year,&month,&day
                                                              ,&hour,&min,&seconds);
                                          if(scannum == 6)
                                          {
                                            m_data.m_dataTIMESTAMP.year   = (SQLSMALLINT)  year;
                                            m_data.m_dataTIMESTAMP.month  = (SQLUSMALLINT) month;
                                            m_data.m_dataTIMESTAMP.day    = (SQLUSMALLINT) day;
                                            m_data.m_dataTIMESTAMP.hour   = (SQLUSMALLINT) hour;
                                            m_data.m_dataTIMESTAMP.minute = (SQLUSMALLINT) min;
                                            // Nanosecond resolution
                                            sec = (int)::floor(seconds);
                                            m_data.m_dataTIMESTAMP.second   = (SQLUSMALLINT) sec;
                                            m_data.m_dataTIMESTAMP.fraction = (SQLUINTEGER)(((seconds - (double)sec) * 1000000000.0) + 0.0000005);
                                          }
                                          else
                                          {
                                            retval = false;
                                          }
                                          break;
    case SQL_C_INTERVAL_YEAR:             m_data.m_dataINTERVAL.intval.year_month.year   = _ttoi(p_data);
                                          m_data.m_dataINTERVAL.interval_type            = SQL_IS_YEAR;
                                          break;
    case SQL_C_INTERVAL_MONTH:            m_data.m_dataINTERVAL.intval.year_month.month  = _ttoi(p_data);
                                          m_data.m_dataINTERVAL.interval_type            = SQL_IS_MONTH;
                                          break;
    case SQL_C_INTERVAL_DAY:              m_data.m_dataINTERVAL.intval.day_second.day    = _ttoi(p_data);
                                          m_data.m_dataINTERVAL.interval_type            = SQL_IS_DAY;
                                          break;
    case SQL_C_INTERVAL_HOUR:             m_data.m_dataINTERVAL.intval.day_second.hour   = _ttoi(p_data);
                                          m_data.m_dataINTERVAL.interval_type            = SQL_IS_HOUR;
                                          break;
    case SQL_C_INTERVAL_MINUTE:           m_data.m_dataINTERVAL.intval.day_second.minute = _ttoi(p_data);
                                          m_data.m_dataINTERVAL.interval_type            = SQL_IS_MINUTE;
                                          break;
    case SQL_C_INTERVAL_SECOND:           seconds = _ttof(p_data);
                                          m_data.m_dataINTERVAL.interval_type = SQL_IS_SECOND;
                                          // Nanosecond resolution
                                          sec = (int)::floor(seconds);
                                          m_data.m_dataINTERVAL.intval.day_second.second   = (SQLUSMALLINT)sec;
                                          m_data.m_dataINTERVAL.intval.day_second.fraction = (SQLUINTEGER)(((seconds - (double)sec) * 1000000000.0) + 0.0000005);

                                          break;
    case SQL_C_INTERVAL_YEAR_TO_MONTH:    m_data.m_dataINTERVAL.interval_type = SQL_IS_YEAR_TO_MONTH;
                                          scannum = _stscanf_s(p_data,_T("%d %d"),&year,&month);
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
                                          scannum = _stscanf_s(p_data,_T("%d %d"),&day,&hour);
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
                                          scannum = _stscanf_s(p_data,_T("%d %d:%d"),&day,&hour,&min);
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
                                          scannum = _stscanf_s(p_data,_T("%d %d:%d:%lf"),&day,&hour,&min,&seconds);
                                          if(scannum == 4)
                                          {
                                            m_data.m_dataINTERVAL.intval.day_second.day    = day;
                                            m_data.m_dataINTERVAL.intval.day_second.hour   = hour;
                                            m_data.m_dataINTERVAL.intval.day_second.minute = min;
                                            // Nanosecond resolution
                                            sec = (int)::floor(seconds);
                                            m_data.m_dataINTERVAL.intval.day_second.second   = (SQLUSMALLINT)sec;
                                            m_data.m_dataINTERVAL.intval.day_second.fraction = (SQLUINTEGER)(((seconds - (double)sec) * 1000000000.0) + 0.0000005);
                                          }
                                          else
                                          {
                                            retval = false;
                                          }
                                          break;
    case SQL_C_INTERVAL_HOUR_TO_MINUTE:   m_data.m_dataINTERVAL.interval_type = SQL_IS_HOUR_TO_MINUTE;
                                          scannum = _stscanf_s(p_data,_T("%d:%d"),&hour,&min);
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
                                          scannum = _stscanf_s(p_data,_T("%d:%d:%lf"),&hour,&min,&seconds);
                                          if(scannum == 3)
                                          {
                                            m_data.m_dataINTERVAL.intval.day_second.hour   = hour;
                                            m_data.m_dataINTERVAL.intval.day_second.minute = min;
                                            // Nanosecond resolution
                                            sec = (int)::floor(seconds);
                                            m_data.m_dataINTERVAL.intval.day_second.second   = (SQLUSMALLINT)sec;
                                            m_data.m_dataINTERVAL.intval.day_second.fraction = (SQLUINTEGER)(((seconds - (double)sec) * 1000000000.0) + 0.0000005);
                                          }
                                          else
                                          {
                                            retval = false;
                                          }
                                          break;
    case SQL_C_INTERVAL_MINUTE_TO_SECOND: m_data.m_dataINTERVAL.interval_type = SQL_IS_MINUTE_TO_SECOND;
                                          scannum = _stscanf_s(p_data,_T("%d:%lf"),&min,&seconds);
                                          if(scannum == 2)
                                          {
                                            m_data.m_dataINTERVAL.intval.day_second.minute = min;
                                            // Nanosecond resolution
                                            sec = (int)::floor(seconds);
                                            m_data.m_dataINTERVAL.intval.day_second.second   = (SQLUSMALLINT)sec;
                                            m_data.m_dataINTERVAL.intval.day_second.fraction = (SQLUINTEGER)(((seconds - (double)sec) * 1000000000.0) + 0.0000005);
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

// Set from a raw data pointer
// Data **MUST** be of the same data type as the SQLVariant !!
// Beware: parameter p_size is needed in case of a SQL_C_BINARY type
void
SQLVariant::SetFromRawDataPointer(void* p_pointer,int p_size /*= 0*/)
{
  // Free previously allocated data
  if(m_datatype == SQL_C_CHAR || m_datatype == SQL_C_BINARY)
  {
    free(m_data.m_dataCHAR);
    m_data.m_dataCHAR = nullptr;

    if(m_datatype == SQL_C_CHAR && p_size == 0)
    {
      p_size = static_cast<int>(strlen(reinterpret_cast<char*>(p_pointer)));
    }
  }
  if(m_datatype == SQL_C_WCHAR)
  {
    free(m_data.m_dataWCHAR);
    m_data.m_dataWCHAR = nullptr;
    if(p_size == 0)
    {
      p_size = static_cast<int>(wcslen(reinterpret_cast<wchar_t*>(p_pointer)));
    }
  }

  // Default for ordinal values is NOT-NULL
  m_indicator = 0;

  // Record data of the type
  switch (m_datatype)
  {
    case SQL_C_CHAR:
#ifdef UNICODE
                                          { 
                                            // Getting the length of the translation buffer first
                                            int clength = ::WideCharToMultiByte(GetACP(),0,(wchar_t*)p_pointer,-1,NULL,0,NULL,NULL);
                                            m_data.m_dataCHAR = new char[clength + 1];
                                            int blength = ::WideCharToMultiByte(GetACP(),0,(wchar_t*)p_pointer,clength,(LPSTR)m_data.m_dataCHAR,clength,NULL,NULL);
                                            m_data.m_dataCHAR[clength] = 0;
                                            if(blength > 0 && blength < clength)
                                            {
                                              m_data.m_dataCHAR[blength] = 0;
                                            }
                                            m_binaryLength = blength;
                                            m_indicator = blength > 0 ? SQL_NTS : SQL_NULL_DATA;
                                          }

#else
                                          m_binaryLength    = p_size;
                                          m_data.m_dataCHAR = new char[(size_t)m_binaryLength + 1];
                                          strcpy_s(m_data.m_dataCHAR,  (size_t)m_binaryLength + 1,reinterpret_cast<char*>(p_pointer));
                                          m_indicator = p_size > 0 ? SQL_NTS : SQL_NULL_DATA;
#endif
                                          break;
    case SQL_C_WCHAR:
#ifdef UNICODE
                                          m_binaryLength = (int)(p_size * 2);
                                          m_data.m_dataWCHAR = new TCHAR[(size_t)p_size + 1];
                                          wcscpy_s(m_data.m_dataWCHAR,(size_t)p_size+ 1,(wchar_t*)p_pointer);
                                          m_indicator = p_size > 0 ? SQL_NTS : SQL_NULL_DATA;
#else
                                          {
                                            // Getting the needed buffer space (in codepoints! Not bytes!!)
                                            int length = MultiByteToWideChar(GetACP(),0,(char*)p_pointer,-1,NULL,NULL);
                                            m_data.m_dataWCHAR = new wchar_t[(size_t) length + 1];
                                            MultiByteToWideChar(GetACP(),0,(char*)p_pointer,-1,reinterpret_cast<LPWSTR>(m_data.m_dataWCHAR),length);
                                            m_data.m_dataWCHAR[length] = 0;
                                            m_binaryLength = 2 * length;
                                            m_indicator = length > 0 ? SQL_NTS : SQL_NULL_DATA;
                                          }
#endif
                                          break;
    case SQL_C_BINARY:                    m_binaryLength = p_size;
                                          m_indicator = m_binaryLength > 0 ? m_binaryLength : SQL_NULL_DATA;
                                          m_data.m_dataBINARY = new BYTE[(size_t)m_binaryLength + 1];
                                          memcpy_s(m_data.m_dataBINARY,p_size,p_pointer,p_size);
                                          break;
    case SQL_C_SHORT:                     // Fall through
    case SQL_C_SSHORT:                    m_data.m_dataSHORT = *((short*) p_pointer);
                                          break;
    case SQL_C_USHORT:                    m_data.m_dataUSHORT = *((unsigned short*) p_pointer);
                                          break;
    case SQL_C_LONG:                      // Fall through
    case SQL_C_SLONG:                     m_data.m_dataLONG = *((long *) p_pointer);
                                          break;
    case SQL_C_ULONG:                     m_data.m_dataULONG = *((unsigned long *) p_pointer);
                                          break;
    case SQL_C_FLOAT:                     m_data.m_dataFLOAT = *((float *)p_pointer);
                                          break;
    case SQL_C_DOUBLE:                    m_data.m_dataDOUBLE = *((double *)p_pointer);
                                          break;
    case SQL_C_BIT:                       m_data.m_dataBIT = *((unsigned char*)p_pointer);
                                          break;
    case SQL_C_TINYINT:                   // Fall through
    case SQL_C_STINYINT:                  m_data.m_dataTINYINT = *((char *)p_pointer);
                                          break;
    case SQL_C_UTINYINT:                  m_data.m_dataTINYINT = *((unsigned char*)p_pointer);
                                          break;
    case SQL_C_SBIGINT:                   m_data.m_dataSBIGINT = *((__int64*)p_pointer);
                                          break;
    case SQL_C_UBIGINT:                   m_data.m_dataUBIGINT = *((unsigned __int64*)p_pointer);
                                          break;
    case SQL_C_NUMERIC:                   memcpy_s(&m_data.m_dataNUMERIC,sizeof(SQL_NUMERIC_STRUCT),p_pointer,sizeof(SQL_NUMERIC_STRUCT));
                                          break;
    case SQL_C_GUID:                      memcpy_s(&m_data.m_dataGUID,sizeof(SQLGUID),p_pointer,sizeof(SQLGUID));
                                          break;
    case SQL_C_DATE:                      // Fall Through
    case SQL_C_TYPE_DATE:                 memcpy_s(&m_data.m_dataDATE,sizeof(DATE_STRUCT),p_pointer,sizeof(DATE_STRUCT));
                                          break;
    case SQL_C_TIME:                      // Fall through
    case SQL_C_TYPE_TIME:                 memcpy_s(&m_data.m_dataTIME,sizeof(TIME_STRUCT),p_pointer,sizeof(TIME_STRUCT));
                                          break;
    case SQL_C_TIMESTAMP:                 // Fall through
    case SQL_C_TYPE_TIMESTAMP:            memcpy_s(&m_data.m_dataTIMESTAMP,sizeof(TIMESTAMP_STRUCT),p_pointer,sizeof(TIMESTAMP_STRUCT));
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
    case SQL_C_INTERVAL_MINUTE_TO_SECOND: memcpy_s(&m_data.m_dataINTERVAL,sizeof(SQL_INTERVAL_STRUCT),(SQL_INTERVAL_STRUCT*)p_pointer,sizeof(SQL_INTERVAL_STRUCT));
                                          break;
    default:                              m_indicator = SQL_NULL_DATA;
  }
}

void
SQLVariant::Set(LPCTSTR p_string,bool p_wide /*=false*/)
{
  if(p_wide)
  {
    if(m_datatype != SQL_C_WCHAR)
    {
      ResetDataType(SQL_C_WCHAR);
    }
  }
  else if(m_datatype != SQL_C_CHAR)
  {
    ResetDataType(SQL_C_CHAR);
  }
  SetFromRawDataPointer((void*)p_string,0);
}

void
SQLVariant::Set(const XString p_string,bool p_wide /*=false*/)
{
  if(p_wide)
  {
    if(m_datatype != SQL_C_WCHAR)
    {
      ResetDataType(SQL_C_WCHAR);
    }
  }
  else if(m_datatype != SQL_C_CHAR)
  {
    ResetDataType(SQL_C_CHAR);
  }
  SetFromRawDataPointer((void *)p_string.GetString(),p_string.GetLength());
}

void
SQLVariant::Set(void* p_pointer,int p_length)
{
  if(m_datatype != SQL_C_BINARY)
  {
    ResetDataType(SQL_C_BINARY);
  }
  SetFromRawDataPointer(p_pointer,p_length);
}

void
SQLVariant::Set(bool p_boolean)
{
  if(m_datatype != SQL_C_BIT)
  {
    ResetDataType(SQL_C_BIT);
  }
  SetFromRawDataPointer((void*)&p_boolean,0);
}

void
SQLVariant::Set(short p_short)
{
  if(m_datatype != SQL_C_SSHORT)
  {
    ResetDataType(SQL_C_SSHORT);
  }
  SetFromRawDataPointer((void*)&p_short,0);
}

void
SQLVariant::Set(unsigned short p_short)
{
  if(m_datatype != SQL_C_USHORT)
  {
    ResetDataType(SQL_C_USHORT);
  }
  SetFromRawDataPointer((void*)&p_short,0);
}

void
SQLVariant::Set(int p_slong)
{
  if(m_datatype != SQL_C_SLONG)
  {
    ResetDataType(SQL_C_SLONG);
  }
  SetFromRawDataPointer((void*)&p_slong,0);
}

void
SQLVariant::Set(unsigned int p_ulong)
{
  if(m_datatype != SQL_C_ULONG)
  {
    ResetDataType(SQL_C_ULONG);
  }
  SetFromRawDataPointer((void*)&p_ulong,0);
}

void
SQLVariant::Set(float p_float)
{
  if(m_datatype != SQL_C_FLOAT)
  {
    ResetDataType(SQL_C_FLOAT);
  }
  SetFromRawDataPointer((void*)&p_float,0);
}

void
SQLVariant::Set(double p_double)
{
  if(m_datatype != SQL_C_DOUBLE)
  {
    ResetDataType(SQL_C_DOUBLE);
  }
  SetFromRawDataPointer((void*)&p_double,0);
}

void
SQLVariant::Set(char p_stinyint)
{
  if(m_datatype != SQL_C_STINYINT)
  {
    ResetDataType(SQL_C_STINYINT);
  }
  SetFromRawDataPointer((void*)&p_stinyint,0);
}

void
SQLVariant::Set(unsigned char p_utinyint)
{
  if(m_datatype != SQL_C_UTINYINT)
  {
    ResetDataType(SQL_C_UTINYINT);
  }
  SetFromRawDataPointer((void*)&p_utinyint,0);
}

void
SQLVariant::Set(bcd p_bcd)
{
  if(m_datatype != SQL_C_NUMERIC)
  {
    ResetDataType(SQL_C_NUMERIC);
  }
  SQL_NUMERIC_STRUCT numeric;
  p_bcd.AsNumeric(&numeric);
  SetFromRawDataPointer((void*)&numeric,0);
}

void
SQLVariant::Set(SQLBIGINT p_sbigint)
{
  if(m_datatype != SQL_C_SBIGINT)
  {
    ResetDataType(SQL_C_SBIGINT);
  }
  SetFromRawDataPointer((void*)&p_sbigint,0);
}

void
SQLVariant::Set(SQLUBIGINT p_ubigint)
{
  if(m_datatype != SQL_C_UBIGINT)
  {
    ResetDataType(SQL_C_UBIGINT);
  }
  SetFromRawDataPointer((void*)&p_ubigint,0);
}

void
SQLVariant::Set(const SQL_NUMERIC_STRUCT* p_numeric)
{
  if(m_datatype != SQL_C_NUMERIC)
  {
    ResetDataType(SQL_C_NUMERIC);
  }
  SetFromRawDataPointer((void*)p_numeric,0);
}

void
SQLVariant::Set(const SQLGUID* p_guid)
{
  if(m_datatype != SQL_C_GUID)
  {
    ResetDataType(SQL_C_GUID);
  }
  SetFromRawDataPointer((void*)p_guid,0);
}

void
SQLVariant::Set(const DATE_STRUCT* p_date)
{
  if(m_datatype != SQL_C_DATE && 
     m_datatype != SQL_C_TYPE_DATE)
  {
    ResetDataType(SQL_C_DATE);
  }
  SetFromRawDataPointer((void*)p_date,0);
}

void
SQLVariant::Set(const TIME_STRUCT* p_time)
{
  if(m_datatype != SQL_C_TIME && 
     m_datatype != SQL_C_TYPE_TIME)
  {
    ResetDataType(SQL_C_TIME);
  }
  SetFromRawDataPointer((void*)p_time,0);
}

void
SQLVariant::Set(const TIMESTAMP_STRUCT* p_timestamp)
{
  if(m_datatype != SQL_C_TIMESTAMP && 
     m_datatype != SQL_C_TYPE_TIMESTAMP)
  {
    ResetDataType(SQL_C_TIMESTAMP);
  }
  SetFromRawDataPointer(reinterpret_cast<void*>(const_cast<TIMESTAMP_STRUCT*>(p_timestamp)),0);
}

void
SQLVariant::Set(const SQL_INTERVAL_STRUCT* p_interval)
{
  if(m_datatype != p_interval->interval_type)
  {
    ResetDataType(p_interval->interval_type);
  }
  SetFromRawDataPointer(reinterpret_cast<void*>(const_cast<SQL_INTERVAL_STRUCT*>(p_interval)),0);
}

void
SQLVariant::Set(const SQLDate* p_date)
{
  if(m_datatype != SQL_C_DATE && 
     m_datatype != SQL_C_TYPE_DATE)
  {
    ResetDataType(SQL_C_DATE);
  }
  DATE_STRUCT date;
  p_date->AsDateStruct(&date);
  SetFromRawDataPointer(reinterpret_cast<void*>(&date),0);
}

void
SQLVariant::Set(const SQLTime* p_time)
{
  if(m_datatype != SQL_C_TIME && 
     m_datatype != SQL_C_TYPE_TIME)
  {
    ResetDataType(SQL_C_TIME);
  }
  TIME_STRUCT time;
  p_time->AsTimeStruct(&time);
  SetFromRawDataPointer(reinterpret_cast<void*>(&time),0);
}

void
SQLVariant::Set(const SQLTimestamp* p_timestamp)
{
  if(m_datatype != SQL_C_TIMESTAMP && 
     m_datatype != SQL_C_TYPE_TIMESTAMP)
  {
    ResetDataType(SQL_C_TIMESTAMP);
  }
  TIMESTAMP_STRUCT stamp;
  p_timestamp->AsTimeStampStruct(&stamp);
  SetFromRawDataPointer(reinterpret_cast<void*>(&stamp),0);
}

void
SQLVariant::Set(const SQLInterval* p_interval)
{
  if(m_datatype != p_interval->GetSQLDatatype())
  {
    ResetDataType(p_interval->GetSQLDatatype());
  }
  SQL_INTERVAL_STRUCT inter;
  p_interval->AsIntervalStruct(&inter);
  SetFromRawDataPointer(reinterpret_cast<void*>(&inter),0);
}

void
SQLVariant::Set(const SQLGuid* p_guid)
{
  if(m_datatype != SQL_C_GUID)
  {
    ResetDataType(SQL_C_GUID);
  }
  SetFromRawDataPointer(reinterpret_cast<void*>(const_cast<SQLGUID*>(p_guid->AsGUID())),0);
}

void
SQLVariant::SetFromEuropeanTimestamp(const XString p_stamp)
{
  SQLTimestamp european(p_stamp);
  if(m_datatype != SQL_C_TIMESTAMP && 
     m_datatype != SQL_C_TYPE_TIMESTAMP)
  {
    ResetDataType(SQL_C_TIMESTAMP);
  }
  TIMESTAMP_STRUCT stamp;
  european.AsTimeStampStruct(&stamp);
  SetFromRawDataPointer(reinterpret_cast<void*>(&stamp),0);
}

//////////////////////////////////////////////////////////////////////////
//
// BLOB Functions
//
//////////////////////////////////////////////////////////////////////////

void
SQLVariant::AttachBinary(void* p_pointer,unsigned long p_size /*= 0*/)
{
  // Forget previous allocations
  ResetDataType(0);

  // Set relevant members
  m_datatype          = SQL_C_BINARY;
  m_sqlDatatype       = SQL_LONGVARBINARY;
  m_useAtExec         = true;
  m_binaryPieceSize   = 4 * 1000; // Default is 1 TCP/IP block
  m_indicator         = p_size > 0 ? p_size : SQL_NULL_DATA;
  // Connect the binary block
  m_binaryLength      = p_size;
  m_data.m_dataBINARY = p_pointer;
}

void
SQLVariant::DetachBinary()
{
  // We forget about the binary block
  // We must **NEVER** do a free on it!!
  Init();
}

//////////////////////////////////////////////////////////////////////////
//
// CONVERSION ROUTINES BEGIN HERE
//
//////////////////////////////////////////////////////////////////////////

// Get a binary buffer in a string
// as hex printed values as Oracle does
bool
SQLVariant::BinaryToString(unsigned char* buffer,int buflen) const
{
  static const char number[17] = "0123456789ABCDEF";

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
  BYTE* colPointer = reinterpret_cast<BYTE*>(m_data.m_dataBINARY);
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
SQLVariant::StringToBinary(const char* p_data) const
{
  const char* colPointer = p_data;
  int len = (int)strlen(p_data);
  BYTE* buffer = reinterpret_cast<BYTE*>(m_data.m_dataBINARY);
  int binlen = m_binaryLength;

  if(m_datatype != SQL_C_BINARY)
  {
    return false;
  }
  while(len && binlen && *colPointer)
  {
    int a = toupper(*colPointer++) - '0';
    int b = toupper(*colPointer++) - '0';
    if(a > 9) a -= 7;
    if(b > 9) b -= 7;
    *buffer++ = (BYTE)((a * 16) + b);
    len -= 2;
    --binlen;
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////
//
// GENERAL ERRORS - THROWING A XString
//
//////////////////////////////////////////////////////////////////////////

void*
SQLVariant::ThrowErrorDatatype(int p_getas) const
{
  XString error;
  LPCTSTR type  = SQLDataType::FindDatatype(m_datatype);
  LPCTSTR getas = SQLDataType::FindDatatype(p_getas);
  error.Format(_T("Cannot get a %s as a %s datatype."),type,getas);
  throw StdException(error);
}

// End of namespace
}
