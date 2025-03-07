////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantOperator.cpp
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
#include "SQLComponents.h"
#include "SQLVariant.h"
#include "SQLVariantOperator.h"
#include "SQLDataType.h"
#include "SQLDate.h"
#include "SQLGuid.h"
#include "bcd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{

// The concise datatype is used to scale the ODBC datatypes to a short integer
// that can be used in the arrays of functions for the comparison and mathematical operators
//
SQLConciseType SQLTypeToConciseType(int p_datatype)
{
  switch(p_datatype)
  {
    case SQL_C_CHAR:                      // fall through
    case SQL_C_WCHAR:                     return CT_CHAR;
    case SQL_C_SHORT:                     // fall through
    case SQL_C_SSHORT:                    return CT_SSHORT;
    case SQL_C_USHORT:                    return CT_USHORT;
    case SQL_C_LONG:                      // fall through
    case SQL_C_SLONG:                     return CT_SLONG;
    case SQL_C_ULONG:                     return CT_ULONG;
    case SQL_C_FLOAT:                     return CT_FLOAT;
    case SQL_C_DOUBLE:                    return CT_DOUBLE;
    case SQL_C_BIT:                       return CT_BIT;
    case SQL_C_TINYINT:                   // fall through
    case SQL_C_STINYINT:                  return CT_STINYINT;
    case SQL_C_UTINYINT:                  return CT_UTINYINT;
    case SQL_C_SBIGINT:                   return CT_SBIGINT;
    case SQL_C_UBIGINT:                   return CT_UBIGINT;
    case SQL_C_NUMERIC:                   return CT_NUMERIC;
    case SQL_C_GUID:                      return CT_GUID;
    case SQL_C_BINARY:                    return CT_BINARY;
    case SQL_C_DATE:                      // fall through
    case SQL_C_TYPE_DATE:                 return CT_DATE;
    case SQL_C_TIME:                      // fall through
    case SQL_C_TYPE_TIME:                 return CT_TIME;
    case SQL_C_TIMESTAMP:                 // fall through
    case SQL_C_TYPE_TIMESTAMP:            return CT_TIMESTAMP;
    case SQL_C_INTERVAL_YEAR:             // fall through
    case SQL_C_INTERVAL_MONTH:            // fall through
    case SQL_C_INTERVAL_YEAR_TO_MONTH:    return CT_INTERVAL_YM;
    case SQL_C_INTERVAL_DAY:              // fall through
    case SQL_C_INTERVAL_HOUR:             // fall through
    case SQL_C_INTERVAL_MINUTE:           // fall through
    case SQL_C_INTERVAL_SECOND:           // fall through
    case SQL_C_INTERVAL_DAY_TO_HOUR:      // fall through
    case SQL_C_INTERVAL_DAY_TO_MINUTE:    // fall through
    case SQL_C_INTERVAL_DAY_TO_SECOND:    // fall through
    case SQL_C_INTERVAL_HOUR_TO_MINUTE:   // fall through
    case SQL_C_INTERVAL_HOUR_TO_SECOND:   // fall through
    case SQL_C_INTERVAL_MINUTE_TO_SECOND: return CT_INTERVAL_DS;
  }
  return CT_LAST;
}

// Assignment operator for a new SQLVariant
SQLVariant&
SQLVariant::operator=(const SQLVariant& p_original)
{
  // Check for assignment to self
  if(this == &p_original)
  {
    return *this;
  }

  // Copy the members
  m_datatype        = p_original.m_datatype;
  m_sqlDatatype     = p_original.m_sqlDatatype;
  m_binaryLength    = p_original.m_binaryLength;
  m_binaryPieceSize = p_original.m_binaryPieceSize;
  m_columnNumber    = p_original.m_columnNumber;
  m_paramType       = p_original.m_paramType;
  m_useAtExec       = p_original.m_useAtExec;
  m_indicator       = p_original.m_indicator;

  // Copy the data
  if(m_datatype == SQL_C_CHAR  || m_datatype == SQL_C_BINARY )
  {
    // Make a new buffer and copy it
    m_data.m_dataBINARY = new unsigned char[(size_t)m_binaryLength + 1];
    memcpy(m_data.m_dataBINARY,p_original.m_data.m_dataBINARY,(size_t)m_binaryLength + 1);
  }
  else if(m_datatype == SQL_C_WCHAR)
  {
    m_data.m_dataWCHAR = new wchar_t[(size_t)m_binaryLength / 2 + 1];
    memcpy(m_data.m_dataWCHAR,p_original.m_data.m_dataWCHAR,m_binaryLength + 2);
  }
  else
  {
    // Copy the data member for ALL OTHER DATATYPES!
    memcpy(&m_data.m_dataBIT,&p_original.m_data.m_dataBIT,sizeof(m_data));
  }
  return *this;
}

// Undesired side effect if SQL_C_CHAR to SQL_C_WCHAR copy
SQLVariant& 
SQLVariant::operator=(LPCTSTR p_data)
{
  if(m_datatype == SQL_C_WCHAR)
  {
    SetData(SQL_C_WCHAR,p_data);
  }
  else
  {
    SetData(SQL_C_CHAR,p_data);
  }
  return *this;
}

// Undesired side effect if SQL_C_CHAR to SQL_C_WCHAR copy
SQLVariant& 
SQLVariant::operator=(const XString& p_data)
{
  if(m_datatype == SQL_C_WCHAR)
  {
    SetData(SQL_C_WCHAR,p_data.GetString());
  }
  else
  {
    SetData(SQL_C_CHAR,p_data.GetString());
  }
  return *this;
}

// Undesired side effect if SQL_C_CHAR to SQL_C_WCHAR copy
SQLVariant& 
SQLVariant::operator=(const XString p_data)
{
  if(m_datatype == SQL_C_WCHAR)
  {
    SetData(SQL_C_WCHAR,p_data.GetString());
  }
  else
  {
    SetData(SQL_C_CHAR,p_data.GetString());
  }
  return *this;
}

SQLVariant& 
SQLVariant::operator=(const short p_data)
{
  Init();
  m_datatype    = SQL_C_SSHORT;
  m_sqlDatatype = SQL_INTEGER;
  m_indicator   = 0;
  m_data.m_dataSSHORT = p_data;

  return *this;
}

SQLVariant& 
SQLVariant::operator=(const unsigned short p_data)
{
  Init();
  m_datatype    = SQL_C_USHORT;
  m_sqlDatatype = SQL_INTEGER;
  m_indicator   = 0;
  m_data.m_dataUSHORT = p_data;

  return *this;
}

SQLVariant& 
SQLVariant::operator=(const int p_data)
{
  Init();
  m_datatype    = SQL_C_SLONG;
  m_sqlDatatype = SQL_INTEGER;
  m_indicator   = 0;
  m_data.m_dataSLONG = p_data;

  return *this;
}

SQLVariant& 
SQLVariant::operator=(const unsigned int p_data)
{
  Init();
  m_datatype    = SQL_C_ULONG;
  m_sqlDatatype = SQL_BIGINT;
  m_indicator   = 0;
  m_data.m_dataULONG = p_data;

  return *this;
}

SQLVariant& 
SQLVariant::operator=(const float p_data)
{
  Init();
  m_datatype    = SQL_C_FLOAT;
  m_sqlDatatype = SQL_FLOAT;
  m_indicator   = 0;
  m_data.m_dataFLOAT = p_data;

  return *this;
}

SQLVariant& 
SQLVariant::operator=(const double p_data)
{
  Init();
  m_datatype    = SQL_C_DOUBLE;
  m_sqlDatatype = SQL_DOUBLE;
  m_indicator   = 0;
  m_data.m_dataDOUBLE = p_data;

  return *this;
}

SQLVariant& 
SQLVariant::operator=(const bool p_data)
{
  Init();
  m_datatype    = SQL_C_BIT;
  m_sqlDatatype = SQL_BIT;
  m_indicator   = 0;
  m_data.m_dataBIT = p_data;

  return *this;
}

SQLVariant& 
SQLVariant::operator=(const char p_data)
{
  Init();
  m_datatype    = SQL_C_STINYINT;
  m_sqlDatatype = SQL_TINYINT;
  m_indicator   = 0;
  m_data.m_dataSTINYINT = p_data;

  return *this;
}

SQLVariant& 
SQLVariant::operator=(const unsigned char p_data)
{
  Init();
  m_datatype    = SQL_C_UTINYINT;
  m_sqlDatatype = SQL_TINYINT;
  m_indicator   = 0;
  m_data.m_dataUTINYINT = p_data;

  return *this;
}

SQLVariant& 
SQLVariant::operator=(const __int64 p_data)
{
  Init();
  m_datatype    = SQL_C_SBIGINT;
  m_sqlDatatype = SQL_BIGINT;
  m_indicator   = 0;
  m_data.m_dataSBIGINT = p_data;

  return *this;
}

SQLVariant& 
SQLVariant::operator=(const unsigned __int64 p_data)
{
  Init();
  m_datatype    = SQL_C_UBIGINT;
  m_sqlDatatype = SQL_BIGINT;
  m_indicator   = 0;
  m_data.m_dataUBIGINT = p_data;

  return *this;
}

SQLVariant& 
SQLVariant::operator=(const SQL_NUMERIC_STRUCT* p_data)
{
  Init();
  m_datatype    = SQL_C_NUMERIC;
  m_sqlDatatype = SQL_NUMERIC;
  m_indicator   = 0;
  memcpy(&m_data.m_dataNUMERIC,p_data,sizeof(SQL_NUMERIC_STRUCT));

  return* this;
}

SQLVariant& 
SQLVariant::operator=(const SQLGUID* p_data)
{
  Init();
  m_datatype    = SQL_C_GUID;
  m_sqlDatatype = SQL_GUID;
  m_indicator   = 0;
  memcpy(&m_data.m_dataGUID,p_data,sizeof(SQLGUID));

  return *this;
}

SQLVariant& 
SQLVariant::operator=(const DATE_STRUCT* p_data)
{
  Init();
  m_datatype    = SQL_C_DATE;
  m_sqlDatatype = SQL_DATE;
  m_indicator   = 0;
  memcpy(&m_data.m_dataDATE,p_data,sizeof(DATE_STRUCT));

  return *this;
}

SQLVariant& 
SQLVariant::operator=(const TIME_STRUCT* p_data)
{
  Init();
  m_datatype    = SQL_C_TIME;
  m_sqlDatatype = SQL_TIME;
  m_indicator   = 0;
  memcpy(&m_data.m_dataTIME,p_data,sizeof(TIME_STRUCT));

  return *this;
}

SQLVariant& 
SQLVariant::operator=(const TIMESTAMP_STRUCT* p_data)
{
  Init();
  m_datatype    = SQL_C_TIMESTAMP;
  m_sqlDatatype = SQL_TIMESTAMP;
  m_indicator   = 0;
  memcpy(&m_data.m_dataTIMESTAMP,p_data,sizeof(TIMESTAMP_STRUCT));

  return *this;
}
SQLVariant& 
SQLVariant::operator=(const SQL_INTERVAL_STRUCT* p_data)
{
  Init();
  m_datatype    = p_data->interval_type;
  m_sqlDatatype = p_data->interval_type;
  m_indicator   = 0;
  if(IsIntervalType() == false)
  {
    m_datatype    = SQL_C_INTERVAL_DAY_TO_SECOND;
    m_sqlDatatype = SQL_INTERVAL_DAY_TO_SECOND;
  }
  memcpy(&m_data.m_dataINTERVAL,p_data,sizeof(SQL_INTERVAL_STRUCT));

  return *this;
}

// Assignments from complex constructors

SQLVariant& 
SQLVariant::operator=(const SQLDate& p_data)
{
  Init();
  m_datatype    = SQL_C_DATE;
  m_sqlDatatype = SQL_DATE;

  if(p_data.IsNull())
  {
    m_indicator = SQL_NULL_DATA;
  }
  else
  {
    m_indicator = 0;
    p_data.AsDateStruct(&m_data.m_dataDATE);
  }
  return *this;
}

SQLVariant& 
SQLVariant::operator=(const SQLTime& p_data)
{
  Init();
  m_datatype    = SQL_C_TIME;
  m_sqlDatatype = SQL_TIME;

  if(p_data.IsNull())
  {
    m_indicator = SQL_NULL_DATA;
  }
  else
  {
    m_indicator = 0;
    p_data.AsTimeStruct(&m_data.m_dataTIME);
  }
  return *this;
}


SQLVariant& 
SQLVariant::operator=(const SQLTimestamp& p_data)
{
  Init();
  m_datatype    = SQL_C_TIMESTAMP;
  m_sqlDatatype = SQL_TIMESTAMP;

  if(p_data.IsNull())
  {
    m_indicator = SQL_NULL_DATA;
  }
  else
  {
    m_indicator = 0;
    p_data.AsTimeStampStruct(&m_data.m_dataTIMESTAMP);
  }
  return *this;
}

SQLVariant& 
SQLVariant::operator=(const SQLInterval& p_data)
{
  Init();
  m_datatype    = p_data.GetIntervalType();
  m_sqlDatatype = m_datatype;

  if(p_data.IsNull())
  {
    m_indicator = SQL_NULL_DATA;
  }
  else
  {
    m_indicator = 0;
    p_data.AsIntervalStruct(&m_data.m_dataINTERVAL);
  }
  return *this;
}

// SQLGuid
SQLVariant& 
SQLVariant::operator =(const SQLGuid& p_guid)
{
  Init();
  m_datatype    = SQL_C_GUID;
  m_sqlDatatype = SQL_GUID;

  if(p_guid.IsValid())
  {
    m_indicator = 0;
    memcpy_s(&m_data.m_dataGUID,sizeof(SQLGUID),p_guid.AsGUID(),sizeof(SQLGUID));
  }
  else
  {
    m_indicator = SQL_NULL_DATA;
  }
  return *this;
}

// Binary Coded Decimal
SQLVariant& 
SQLVariant::operator=(const bcd& p_bcd)
{
  Init();
  m_datatype    = SQL_C_NUMERIC;
  m_sqlDatatype = SQL_NUMERIC;
  m_indicator   = 0;

  p_bcd.AsNumeric(&m_data.m_dataNUMERIC);
  return *this;
}

//////////////////////////////////////////////////////////////////////////
//
// CAST OPERATORS
//
//////////////////////////////////////////////////////////////////////////

SQLVariant::operator bool()
{
  return GetAsBit() > 0;
}

SQLVariant::operator char()
{
  return GetAsSTinyInt();
}

SQLVariant::operator uchar()
{
  return GetAsUTinyInt();
}

SQLVariant::operator LPCSTR()
{
  return (LPCSTR)GetAsChar();
}

SQLVariant::operator short()
{
  return GetAsSShort();
}

SQLVariant::operator unsigned short()
{
  return GetAsUShort();
}

SQLVariant::operator int()
{
  return GetAsSLong();
}

SQLVariant::operator unsigned int()
{
  return GetAsULong();
}

SQLVariant::operator long()
{
  return GetAsSLong();
}

SQLVariant::operator unsigned long()
{
  return GetAsULong();
}

SQLVariant::operator float()
{
  return GetAsFloat();
}

SQLVariant::operator double()
{
  return GetAsDouble();
}

SQLVariant::operator __int64()
{
  return GetAsSBigInt();
}

SQLVariant::operator unsigned __int64()
{
  return GetAsUBigInt();
}

SQLVariant::operator SQLDate()
{
  return GetAsSQLDate();
}

SQLVariant::operator SQLTime()
{
  return GetAsSQLTime();
}

SQLVariant::operator SQLTimestamp()
{
  return GetAsSQLTimestamp();
}

SQLVariant::operator SQLInterval()
{
  return GetAsSQLInterval();
}

SQLVariant::operator SQLGuid()
{
  return GetAsSQLGuid();
}

SQLVariant::operator XString()
{
  return GetAsString();
}

SQLVariant::operator bcd()
{
  return GetAsBCD();
}

//////////////////////////////////////////////////////////////////////////
//
// PRE- AND POST-INCREMENT AND DECREMENT OPERATORS
//
//////////////////////////////////////////////////////////////////////////

void
SQLVariant::ThrowErrorOperator(SQLVarOperator p_operator)
{
  XString error;
  LPCTSTR type = SQLDataType::FindDatatype(m_datatype);
  LPCTSTR oper = nullptr;
  switch (p_operator)
  {
    case SVO_PreIncrement:    oper = _T("pre-increment");  break;
    case SVO_PreDecrement:    oper = _T("pre-decrement");  break;
    case SVO_PostIncrement:   oper = _T("post-increment"); break;
    case SVO_PostDecrement:   oper = _T("post-decrement"); break;
    case SVO_AssignAdd:       oper = _T("+=");             break;
    case SVO_AssignSubtract:  oper = _T("-=");             break;
    case SVO_AssignMultiply:  oper = _T("*=");             break;
    case SVO_AssignDivide:    oper = _T("/=");             break;
    case SVO_AssignModulo:    oper = _T("%=");             break;
    default:                  oper = _T("unknown");        break;
  }
  error.Format(_T("Cannot execute %s operator on datatype: %s."),oper,type);
  throw StdException(error);
}

// Pre-increment
SQLVariant& 
SQLVariant::operator++()
{
  // NULL will always be NULL
  if(IsNULL())
  {
    return *this;
  }
  switch(m_datatype)
  {
    case SQL_C_CHAR:                      // fall through
    case SQL_C_WCHAR:                     // fall through
    case SQL_C_GUID:                      // fall through
    case SQL_C_BINARY:                    // fall through
    default:                              ThrowErrorOperator(SVO_PreIncrement);

    case SQL_C_SHORT:                     // fall through
    case SQL_C_SSHORT:                    ++m_data.m_dataSHORT;
                                          break;
    case SQL_C_USHORT:                    ++m_data.m_dataUSHORT;
                                          break;
    case SQL_C_LONG:                      // fall through
    case SQL_C_SLONG:                     ++m_data.m_dataSLONG;
                                          break;
    case SQL_C_ULONG:                     ++m_data.m_dataULONG;
                                          break;
    case SQL_C_FLOAT:                     ++m_data.m_dataFLOAT;
                                          break;
    case SQL_C_DOUBLE:                    ++m_data.m_dataDOUBLE;
                                          break;
    case SQL_C_BIT:                       ++m_data.m_dataBIT;
                                          break;
    case SQL_C_TINYINT:                   // fall through
    case SQL_C_STINYINT:                  ++m_data.m_dataSTINYINT;
                                          break;
    case SQL_C_UTINYINT:                  ++m_data.m_dataUTINYINT;
                                          break;
    case SQL_C_SBIGINT:                   ++m_data.m_dataSBIGINT;
                                          break;
    case SQL_C_UBIGINT:                   ++m_data.m_dataUBIGINT;
                                          break;
    case SQL_C_NUMERIC:                   {  bcd data(&m_data.m_dataNUMERIC);
                                             ++data;
                                             data.AsNumeric(&m_data.m_dataNUMERIC);
                                          }
                                          break;
    case SQL_C_DATE:                      // fall through
    case SQL_C_TYPE_DATE:                 {  SQLDate date(&m_data.m_dataDATE);
                                             date.AddDays(1);
                                             date.AsDateStruct(&m_data.m_dataDATE);
                                          }
                                          break;
    case SQL_C_TIME:                      // fall through
    case SQL_C_TYPE_TIME:                 {  SQLTime time(&m_data.m_dataTIME);
                                             time.AddSeconds(1);
                                             time.AsTimeStruct(&m_data.m_dataTIME);
                                          }
                                          break;
    case SQL_C_TIMESTAMP:                 // fall through
    case SQL_C_TYPE_TIMESTAMP:            {  SQLTimestamp stamp(&m_data.m_dataTIMESTAMP);
                                             stamp.AddSeconds(1);
                                             stamp.AsTimeStampStruct(&m_data.m_dataTIMESTAMP);
                                          }
                                          break;
    case SQL_C_INTERVAL_YEAR:             {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddYears(1);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_MONTH:            {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddMonths(1);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_YEAR_TO_MONTH:    {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddMonths(1);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_DAY:              {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddDays(1);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_HOUR:             {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddHours(1);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_MINUTE:           {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddMinutes(1);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_SECOND:           {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddSeconds(1);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_DAY_TO_HOUR:      {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddHours(1);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_DAY_TO_MINUTE:    {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddMinutes(1);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_DAY_TO_SECOND:    {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddSeconds(1);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_HOUR_TO_MINUTE:   {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddMinutes(1);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_HOUR_TO_SECOND:   {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddSeconds(1);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_MINUTE_TO_SECOND: {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddSeconds(1);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
  }
  return* this;
}

// Post increment
SQLVariant  
SQLVariant::operator++(int p_val)
{
  // NULL will always be NULL
  if (IsNULL())
  {
    return *this;
  }
  // Value before increment
  SQLVariant pre(this);

  // Could possibly called with a discrete value
  if(p_val == 0)
  {
    p_val = 1;
  }

  switch(m_datatype)
  {
    case SQL_C_CHAR:                      // fall through
    case SQL_C_WCHAR:                     // fall through
    case SQL_C_GUID:                      // fall through
    case SQL_C_BINARY:                    // fall through
    default:                              ThrowErrorOperator(SVO_PostIncrement);

    case SQL_C_SHORT:                     // fall through
    case SQL_C_SSHORT:                    m_data.m_dataSHORT += (short)p_val;
                                          break;
    case SQL_C_USHORT:                    m_data.m_dataUSHORT += (ushort)p_val;
                                          break;
    case SQL_C_LONG:                      // fall through
    case SQL_C_SLONG:                     m_data.m_dataSLONG += p_val;
                                          break;
    case SQL_C_ULONG:                     m_data.m_dataULONG += p_val;
                                          break;
    case SQL_C_FLOAT:                     m_data.m_dataFLOAT += p_val;
                                          break;
    case SQL_C_DOUBLE:                    m_data.m_dataDOUBLE += p_val;
                                          break;
    case SQL_C_BIT:                       ++m_data.m_dataBIT;
                                          break;
    case SQL_C_TINYINT:                   // fall through
    case SQL_C_STINYINT:                  m_data.m_dataSTINYINT += (char)p_val;
                                          break;
    case SQL_C_UTINYINT:                  m_data.m_dataUTINYINT += (uchar)p_val;
                                          break;
    case SQL_C_SBIGINT:                   m_data.m_dataSBIGINT += p_val;
                                          break;
    case SQL_C_UBIGINT:                   m_data.m_dataUBIGINT += p_val;
                                          break;
    case SQL_C_NUMERIC:                   {  bcd data(&m_data.m_dataNUMERIC);
                                             data += bcd(p_val);
                                             data.AsNumeric(&m_data.m_dataNUMERIC);
                                          }
                                          break;
    case SQL_C_DATE:                      // fall through
    case SQL_C_TYPE_DATE:                 {  SQLDate date(&m_data.m_dataDATE);
                                             date.AddDays(p_val);
                                             date.AsDateStruct(&m_data.m_dataDATE);
                                          }
                                          break;
    case SQL_C_TIME:                      // fall through
    case SQL_C_TYPE_TIME:                 {  SQLTime time(&m_data.m_dataTIME);
                                             time.AddSeconds(p_val);
                                             time.AsTimeStruct(&m_data.m_dataTIME);
                                          }
                                          break;
    case SQL_C_TIMESTAMP:                 // fall through
    case SQL_C_TYPE_TIMESTAMP:            {  SQLTimestamp stamp(&m_data.m_dataTIMESTAMP);
                                             stamp.AddSeconds(p_val);
                                             stamp.AsTimeStampStruct(&m_data.m_dataTIMESTAMP);
                                          }
                                          break;
    case SQL_C_INTERVAL_YEAR:             {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddYears(p_val);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_MONTH:            {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddMonths(p_val);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_YEAR_TO_MONTH:    {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddMonths(p_val);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_DAY:              {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddDays(p_val);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_HOUR:             {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddHours(p_val);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_MINUTE:           {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddMinutes(p_val);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_SECOND:           {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddSeconds(p_val);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_DAY_TO_HOUR:      {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddHours(p_val);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_DAY_TO_MINUTE:    {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddMinutes(p_val);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_DAY_TO_SECOND:    {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddSeconds(p_val);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_HOUR_TO_MINUTE:   {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddMinutes(p_val);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_HOUR_TO_SECOND:   {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddSeconds(p_val);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_MINUTE_TO_SECOND: {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddSeconds(p_val);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
  }
  // Post increment returns the value before the increment
  return pre;
}

// Prefix decrement
SQLVariant&
SQLVariant::operator--()
{
  // NULL will always be NULL
  if (IsNULL())
  {
    return *this;
  }
  switch(m_datatype)
  {
    case SQL_C_CHAR:                      // fall through
    case SQL_C_WCHAR:                     // fall through
    case SQL_C_GUID:                      // fall through
    case SQL_C_BINARY:                    // fall through
    default:                              ThrowErrorOperator(SVO_PreDecrement);

    case SQL_C_SHORT:                     // fall through
    case SQL_C_SSHORT:                    --m_data.m_dataSHORT;
                                          break;
    case SQL_C_USHORT:                    --m_data.m_dataUSHORT;
                                          break;
    case SQL_C_LONG:                      // fall through
    case SQL_C_SLONG:                     --m_data.m_dataSLONG;
                                          break;
    case SQL_C_ULONG:                     --m_data.m_dataULONG;
                                          break;
    case SQL_C_FLOAT:                     --m_data.m_dataFLOAT;
                                          break;
    case SQL_C_DOUBLE:                    --m_data.m_dataDOUBLE;
                                          break;
    case SQL_C_BIT:                       --m_data.m_dataBIT;
                                          break;
    case SQL_C_TINYINT:                   // fall through
    case SQL_C_STINYINT:                  --m_data.m_dataSTINYINT;
                                          break;
    case SQL_C_UTINYINT:                  --m_data.m_dataUTINYINT;
                                          break;
    case SQL_C_SBIGINT:                   --m_data.m_dataSBIGINT;
                                          break;
    case SQL_C_UBIGINT:                   --m_data.m_dataUBIGINT;
                                          break;
    case SQL_C_NUMERIC:                   {  bcd data(&m_data.m_dataNUMERIC);
                                             --data;
                                             data.AsNumeric(&m_data.m_dataNUMERIC);
                                          }
                                          break;
    case SQL_C_DATE:                      // fall through
    case SQL_C_TYPE_DATE:                 {  SQLDate date(&m_data.m_dataDATE);
                                             date.AddDays(-1);
                                             date.AsDateStruct(&m_data.m_dataDATE);
                                          }
                                          break;
    case SQL_C_TIME:                      // fall through
    case SQL_C_TYPE_TIME:                 {  SQLTime time(&m_data.m_dataTIME);
                                             time.AddSeconds(-1);
                                             time.AsTimeStruct(&m_data.m_dataTIME);
                                          }
                                          break;
    case SQL_C_TIMESTAMP:                 // fall through
    case SQL_C_TYPE_TIMESTAMP:            {  SQLTimestamp stamp(&m_data.m_dataTIMESTAMP);
                                             stamp.AddSeconds(-1);
                                             stamp.AsTimeStampStruct(&m_data.m_dataTIMESTAMP);
                                          }
                                          break;
    case SQL_C_INTERVAL_YEAR:             {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddYears(-1);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_MONTH:            {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddMonths(-1);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_YEAR_TO_MONTH:    {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddMonths(-1);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_DAY:              {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddDays(-1);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_HOUR:             {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddHours(-1);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_MINUTE:           {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddMinutes(-1);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_SECOND:           {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddSeconds(-1);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_DAY_TO_HOUR:      {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddHours(-1);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_DAY_TO_MINUTE:    {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddMinutes(-1);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_DAY_TO_SECOND:    {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddSeconds(-1);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_HOUR_TO_MINUTE:   {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddMinutes(-1);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_HOUR_TO_SECOND:   {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddSeconds(-1);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_MINUTE_TO_SECOND: {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddSeconds(-1);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
  }
  return *this;
}

// Post decrement
SQLVariant
SQLVariant::operator--(int p_val)
{
  // NULL will always be NULL
  if (IsNULL())
  {
    return *this;
  }
  // Value before increment
  SQLVariant pre(this);

  // Could possibly called with a discrete value
  if(p_val == 0)
  {
    p_val = 1;
  }

  switch(m_datatype)
  {
    case SQL_C_CHAR:                      // fall through
    case SQL_C_WCHAR:                     // fall through
    case SQL_C_GUID:                      // fall through
    case SQL_C_BINARY:                    // fall through
    default:                              ThrowErrorOperator(SVO_PostDecrement);

    case SQL_C_SHORT:                     // fall through
    case SQL_C_SSHORT:                    m_data.m_dataSHORT -= (short)p_val;
                                          break;
    case SQL_C_USHORT:                    m_data.m_dataUSHORT -= (ushort)p_val;
                                          break;
    case SQL_C_LONG:                      // fall through
    case SQL_C_SLONG:                     m_data.m_dataSLONG -= p_val;
                                          break;
    case SQL_C_ULONG:                     m_data.m_dataULONG -= p_val;
                                          break;
    case SQL_C_FLOAT:                     m_data.m_dataFLOAT -= p_val;
                                          break;
    case SQL_C_DOUBLE:                    m_data.m_dataDOUBLE -= p_val;
                                          break;
    case SQL_C_BIT:                       --m_data.m_dataBIT;
                                          break;
    case SQL_C_TINYINT:                   // fall through
    case SQL_C_STINYINT:                  m_data.m_dataSTINYINT -= (char)p_val;
                                          break;
    case SQL_C_UTINYINT:                  m_data.m_dataUTINYINT -= (uchar)p_val;
                                          break;
    case SQL_C_SBIGINT:                   m_data.m_dataSBIGINT -= p_val;
                                          break;
    case SQL_C_UBIGINT:                   m_data.m_dataUBIGINT -= p_val;
                                          break;
    case SQL_C_NUMERIC:                   {  bcd data(&m_data.m_dataNUMERIC);
                                             data += bcd(-p_val);
                                             data.AsNumeric(&m_data.m_dataNUMERIC);
                                          }
                                          break;
    case SQL_C_DATE:                      // fall through
    case SQL_C_TYPE_DATE:                 {  SQLDate date(&m_data.m_dataDATE);
                                             date.AddDays(-p_val);
                                             date.AsDateStruct(&m_data.m_dataDATE);
                                          }
                                          break;
    case SQL_C_TIME:                      // fall through
    case SQL_C_TYPE_TIME:                 {  SQLTime time(&m_data.m_dataTIME);
                                             time.AddSeconds(-p_val);
                                             time.AsTimeStruct(&m_data.m_dataTIME);
                                          }
                                          break;
    case SQL_C_TIMESTAMP:                 // fall through
    case SQL_C_TYPE_TIMESTAMP:            {  SQLTimestamp stamp(&m_data.m_dataTIMESTAMP);
                                             stamp.AddSeconds(-p_val);
                                             stamp.AsTimeStampStruct(&m_data.m_dataTIMESTAMP);
                                          }
                                          break;
    case SQL_C_INTERVAL_YEAR:             {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddYears(-p_val);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_MONTH:            {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddMonths(-p_val);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_YEAR_TO_MONTH:    {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddMonths(-p_val);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_DAY:              {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddDays(-p_val);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_HOUR:             {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddHours(-p_val);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_MINUTE:           {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddMinutes(-p_val);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_SECOND:           {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddSeconds(-p_val);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_DAY_TO_HOUR:      {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddHours(-p_val);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_DAY_TO_MINUTE:    {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddMinutes(-p_val);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_DAY_TO_SECOND:    {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddSeconds(-p_val);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_HOUR_TO_MINUTE:   {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddMinutes(-p_val);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_HOUR_TO_SECOND:   {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddSeconds(-p_val);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
    case SQL_C_INTERVAL_MINUTE_TO_SECOND: {  SQLInterval intval(&m_data.m_dataINTERVAL);
                                             intval.AddSeconds(-p_val);
                                             intval.AsIntervalStruct(&m_data.m_dataINTERVAL);
                                          }
                                          break;
  }
  // Post increment returns the value before the increment
  return pre;
}



// End of namespace
}
