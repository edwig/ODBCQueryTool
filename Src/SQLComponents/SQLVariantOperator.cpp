////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantOperator.cpp
//
// Copyright (c) 1998-2017 ir. W.E. Huisman
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
// Last Revision:   08-01-2017
// Version number:  1.4.0
//
#include "stdafx.h"
#include "SQLComponents.h"
#include "SQLVariant.h"
#include "SQLVariantOperator.h"
#include "SQLDate.h"
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
    case SQL_C_CHAR:                      return CT_CHAR;
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
    m_data.m_dataBINARY = (unsigned char*) malloc(m_binaryLength + 1);
    memcpy(m_data.m_dataBINARY,p_original.m_data.m_dataBINARY,m_binaryLength + 1);
  }
  else
  {
    // Copy the data member for ALL OTHER DATATYPES!
    memcpy(&m_data.m_dataBIT,&p_original.m_data.m_dataBIT,sizeof(m_data));
  }
  return *this;
}

SQLVariant& 
SQLVariant::operator=(const char* p_data)
{
  SetData(SQL_C_CHAR,p_data);
  return *this;
}

SQLVariant& 
SQLVariant::operator=(CString& p_data)
{
  SetData(SQL_C_CHAR,p_data.GetString());
  return *this;
}

SQLVariant& 
SQLVariant::operator=(short p_data)
{
  Init();
  m_datatype    = SQL_C_SSHORT;
  m_sqlDatatype = SQL_INTEGER;
  m_indicator   = 0;
  m_data.m_dataSSHORT = p_data;

  return *this;
}

SQLVariant& 
SQLVariant::operator=(unsigned short p_data)
{
  Init();
  m_datatype    = SQL_C_USHORT;
  m_sqlDatatype = SQL_INTEGER;
  m_indicator   = 0;
  m_data.m_dataUSHORT = p_data;

  return *this;
}

SQLVariant& 
SQLVariant::operator=(long p_data)
{
  Init();
  m_datatype    = SQL_C_SLONG;
  m_sqlDatatype = SQL_INTEGER;
  m_indicator   = 0;
  m_data.m_dataSLONG = p_data;

  return *this;
}

SQLVariant& 
SQLVariant::operator=(unsigned long p_data)
{
  Init();
  m_datatype    = SQL_C_ULONG;
  m_sqlDatatype = SQL_BIGINT;
  m_indicator   = 0;
  m_data.m_dataULONG = p_data;

  return *this;
}

SQLVariant& 
SQLVariant::operator=(float p_data)
{
  Init();
  m_datatype    = SQL_C_FLOAT;
  m_sqlDatatype = SQL_FLOAT;
  m_indicator   = 0;
  m_data.m_dataFLOAT = p_data;

  return *this;
}

SQLVariant& 
SQLVariant::operator=(double p_data)
{
  Init();
  m_datatype    = SQL_C_DOUBLE;
  m_sqlDatatype = SQL_DOUBLE;
  m_indicator   = 0;
  m_data.m_dataDOUBLE = p_data;

  return *this;
}

SQLVariant& 
SQLVariant::operator=(bool p_data)
{
  Init();
  m_datatype    = SQL_C_BIT;
  m_sqlDatatype = SQL_BIT;
  m_indicator   = 0;
  m_data.m_dataBIT = p_data;

  return *this;
}

SQLVariant& 
SQLVariant::operator=(char p_data)
{
  Init();
  m_datatype    = SQL_C_STINYINT;
  m_sqlDatatype = SQL_TINYINT;
  m_indicator   = 0;
  m_data.m_dataSTINYINT = p_data;

  return *this;
}

SQLVariant& 
SQLVariant::operator=(unsigned char p_data)
{
  Init();
  m_datatype    = SQL_C_UTINYINT;
  m_sqlDatatype = SQL_TINYINT;
  m_indicator   = 0;
  m_data.m_dataUTINYINT = p_data;

  return *this;
}

SQLVariant& 
SQLVariant::operator=(__int64 p_data)
{
  Init();
  m_datatype    = SQL_C_SBIGINT;
  m_sqlDatatype = SQL_BIGINT;
  m_indicator   = 0;
  m_data.m_dataSBIGINT = p_data;

  return *this;
}

SQLVariant& 
SQLVariant::operator=(unsigned __int64 p_data)
{
  Init();
  m_datatype    = SQL_C_UBIGINT;
  m_sqlDatatype = SQL_BIGINT;
  m_indicator   = 0;
  m_data.m_dataUBIGINT = p_data;

  return *this;
}

SQLVariant& 
SQLVariant::operator=(SQL_NUMERIC_STRUCT* p_data)
{
  Init();
  m_datatype    = SQL_C_NUMERIC;
  m_sqlDatatype = SQL_NUMERIC;
  m_indicator   = 0;
  memcpy(&m_data.m_dataNUMERIC,p_data,sizeof(SQL_NUMERIC_STRUCT));

  return* this;
}

SQLVariant& 
SQLVariant::operator=(SQLGUID* p_data)
{
  Init();
  m_datatype    = SQL_C_GUID;
  m_sqlDatatype = SQL_GUID;
  m_indicator   = 0;
  memcpy(&m_data.m_dataGUID,p_data,sizeof(SQLGUID));

  return *this;
}

SQLVariant& 
SQLVariant::operator=(DATE_STRUCT* p_data)
{
  Init();
  m_datatype    = SQL_C_DATE;
  m_sqlDatatype = SQL_DATE;
  m_indicator   = 0;
  memcpy(&m_data.m_dataDATE,p_data,sizeof(DATE_STRUCT));

  return *this;
}

SQLVariant& 
SQLVariant::operator=(TIME_STRUCT* p_data)
{
  Init();
  m_datatype    = SQL_C_TIME;
  m_sqlDatatype = SQL_TIME;
  m_indicator   = 0;
  memcpy(&m_data.m_dataTIME,p_data,sizeof(TIME_STRUCT));

  return *this;
}

SQLVariant& 
SQLVariant::operator=(TIMESTAMP_STRUCT* p_data)
{
  Init();
  m_datatype    = SQL_C_TIMESTAMP;
  m_sqlDatatype = SQL_TIMESTAMP;
  m_indicator   = 0;
  memcpy(&m_data.m_dataTIMESTAMP,p_data,sizeof(TIMESTAMP_STRUCT));

  return *this;
}
SQLVariant& 
SQLVariant::operator=(SQL_INTERVAL_STRUCT* p_data)
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
SQLVariant::operator=(SQLDate& p_data)
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
SQLVariant::operator=(SQLTime& p_data)
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
SQLVariant::operator=(SQLTimestamp& p_data)
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
SQLVariant::operator=(SQLInterval& p_data)
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

// Binary Coded Decimal
SQLVariant& 
SQLVariant::operator=(bcd& p_bcd)
{
  Init();
  m_datatype    = SQL_C_NUMERIC;
  m_sqlDatatype = SQL_NUMERIC;
  m_indicator   = 0;

  int prec  = p_bcd.GetExponent() + 1;  // Exponent 0 = 1 digit before the decimal point
  int scale = p_bcd.GetPrecision();     // Digits after the decimal point

  // At least 1 digit before the decimal point
  if(prec < 1)
  {
    prec = 1;
  }
  // precision scaling of a numeric = prec + scale
  prec += scale;

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

SQLVariant::operator unsigned char()
{
  return GetAsUTinyInt();
}

SQLVariant::operator const char*()
{
  return (const char*)GetAsChar();
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

SQLVariant::operator CString()
{
  return CString(GetAsChar());
}

SQLVariant::operator bcd()
{
  return GetAsBCD();
}

// End of namespace
}
