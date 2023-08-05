////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantTrim.cpp
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
#include "SQLComponents.h"
#include "SQLVariant.h"
#include "SQLVariantTrim.h"
#include "SQLDataType.h"
#include <float.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{

//////////////////////////////////////////////////////////////////////////
//
// GENERAL ERRORS - THROWING A XString
//
//////////////////////////////////////////////////////////////////////////

void SQL_ThrowErrorTruncate(int p_from,int p_to)
{
  XString error;
  const char* from = SQLDataType::FindDatatype(p_from);
  const char* to   = SQLDataType::FindDatatype(p_to);
  error.Format("Cannot truncate %s to %s",from,to);
  throw StdException(error);
}

//////////////////////////////////////////////////////////////////////////
//
// TRIMMING CONVERSION ROUTINES BEGIN HERE
//
//////////////////////////////////////////////////////////////////////////

long
SQL_ULongToLong(unsigned long p_value)
{
  if(p_value > LONG_MAX)
  {
    SQL_ThrowErrorTruncate(SQL_C_ULONG,SQL_C_SLONG);
  }
  return (long)p_value;
}

long
SQL_FloatToLong(float p_value)
{
  if(p_value > LONG_MAX || p_value < (float)LONG_MIN)
  {
    SQL_ThrowErrorTruncate(SQL_C_FLOAT,SQL_C_LONG);
  }
  return (long)p_value;
}

long
SQL_DoubleToLong(double p_value)
{
  if(p_value > LONG_MAX || p_value < (double)LONG_MIN)
  {
    SQL_ThrowErrorTruncate(SQL_C_DOUBLE,SQL_C_LONG);
  }
  return (long)p_value;
}

long
SQL_BIGINTToLong(SQLBIGINT p_value)
{
  if(p_value > LONG_MAX || p_value < (SQLBIGINT)LONG_MIN)
  {
    SQL_ThrowErrorTruncate(SQL_C_SBIGINT,SQL_C_LONG);
  }
  return (long)p_value;
}

long
SQL_UBIGINTToLong(SQLUBIGINT p_value)
{
  if(p_value > LONG_MAX)
  {
    SQL_ThrowErrorTruncate(SQL_C_UBIGINT,SQL_C_LONG);
  }
  return (long)p_value;
}

short
SQL_UShortToShort(unsigned short p_value)
{
  if(p_value > SHRT_MAX)
  {
    SQL_ThrowErrorTruncate(SQL_C_USHORT,SQL_C_SHORT);
  }
  return (short)p_value;
}

short
SQL_SLongToShort(long p_value)
{
  if(p_value > SHRT_MAX || p_value < SHRT_MIN)
  {
    SQL_ThrowErrorTruncate(SQL_C_SLONG,SQL_C_SHORT);
  }
  return (short)p_value;
}

short
SQL_ULongToShort(unsigned long p_value)
{
  if(p_value > SHRT_MAX)
  {
    SQL_ThrowErrorTruncate(SQL_C_ULONG,SQL_C_SHORT);
  }
  return (short)p_value;
}

short
SQL_FloatToShort(float p_value)
{
  if(p_value > SHRT_MAX || p_value < SHRT_MIN)
  {
    SQL_ThrowErrorTruncate(SQL_C_FLOAT,SQL_C_SHORT);
  }
  return (short)p_value;
}

short
SQL_DoubleToShort(double p_value)
{
  if(p_value > SHRT_MAX || p_value < SHRT_MIN)
  {
    SQL_ThrowErrorTruncate(SQL_C_DOUBLE,SQL_C_SHORT);
  }
  return (short)p_value;
}

short
SQL_BIGINTToShort(SQLBIGINT p_value)
{
  if(p_value > SHRT_MAX || p_value < SHRT_MIN)
  {
    SQL_ThrowErrorTruncate(SQL_C_SBIGINT,SQL_C_SHORT);
  }
  return (short)p_value;
}

short
SQL_UBIGINTToShort(SQLUBIGINT p_value)
{
  if(p_value > SHRT_MAX)
  {
    SQL_ThrowErrorTruncate(SQL_C_UBIGINT,SQL_C_SHORT);
  }
  return (short)p_value;
}

float
SQL_DoubleToFloat(double p_value)
{
  if(p_value > FLT_MAX || p_value < -FLT_MAX)
  {
    SQL_ThrowErrorTruncate(SQL_C_DOUBLE,SQL_C_FLOAT);
  }
  return (float)p_value;
}

float
SQL_BIGINTToFloat(SQLBIGINT p_value)
{
  if(p_value > FLT_MAX || p_value < -FLT_MAX)
  {
    SQL_ThrowErrorTruncate(SQL_C_SBIGINT,SQL_C_FLOAT);
  }
  return (float)p_value;
}

float
SQL_UBIGINTToFloat(SQLUBIGINT p_value)
{
  if(p_value > FLT_MAX)
  {
    SQL_ThrowErrorTruncate(SQL_C_UBIGINT,SQL_C_FLOAT);
  }
  return (float)p_value;
}

double
SQL_BIGINTToDouble(SQLBIGINT p_value)
{
  if(p_value > DBL_MAX || p_value < -DBL_MAX)
  {
    SQL_ThrowErrorTruncate(SQL_C_SBIGINT,SQL_C_DOUBLE);
  }
  return (double)p_value;
}

double
SQL_UBIGINTToDouble(SQLUBIGINT p_value)
{
  if(p_value > DBL_MAX)
  {
    SQL_ThrowErrorTruncate(SQL_C_UBIGINT,SQL_C_DOUBLE);
  }
  return (double)p_value;
}

SQLBIGINT
SQL_FloatToBIGINT(float p_value)
{
  if(p_value > LLONG_MAX || p_value < LLONG_MIN)
  {
    SQL_ThrowErrorTruncate(SQL_C_FLOAT,SQL_C_SBIGINT);
  }
  return (SQLBIGINT)p_value;
}

SQLBIGINT
SQL_DoubleToBIGINT(double p_value)
{
  if(p_value > LLONG_MAX || p_value < LLONG_MIN)
  {
    SQL_ThrowErrorTruncate(SQL_C_DOUBLE,SQL_C_SBIGINT);
  }
  return (SQLBIGINT)p_value;
}

SQLBIGINT
SQL_UBIGINTToBIGINT(SQLUBIGINT p_value)
{
  if(p_value > LLONG_MAX)
  {
    SQL_ThrowErrorTruncate(SQL_C_UBIGINT,SQL_C_SBIGINT);
  }
  return (SQLBIGINT)p_value;
}

char
SQL_ShortToTinyInt(short p_value)
{
  if(p_value > _I8_MAX || p_value < _I8_MIN)
  {
    SQL_ThrowErrorTruncate(SQL_C_SHORT,SQL_C_STINYINT);
  }
  return (char)p_value;
}

char
SQL_UShortToTinyInt(unsigned short p_value)
{
  if(p_value > _I8_MAX)
  {
    SQL_ThrowErrorTruncate(SQL_C_USHORT,SQL_C_STINYINT);
  }
  return (char)p_value;
}

char
SQL_SLongToTinyInt(long p_value)
{
  if(p_value > _I8_MAX || p_value < _I8_MIN)
  {
    SQL_ThrowErrorTruncate(SQL_C_SLONG,SQL_C_STINYINT);
  }
  return (char)p_value;
}

char
SQL_ULongToTinyInt(long p_value)
{
  if(p_value > _I8_MAX)
  {
    SQL_ThrowErrorTruncate(SQL_C_ULONG,SQL_C_STINYINT);
  }
  return (char)p_value;
}

char
SQL_FloatToTinyInt(float p_value)
{
  if(p_value > _I8_MAX || p_value < _I8_MIN)
  {
    SQL_ThrowErrorTruncate(SQL_C_FLOAT,SQL_C_STINYINT);
  }
  return (char)p_value;
}

char
SQL_DoubleToTinyInt(double p_value)
{
  if(p_value > _I8_MAX || p_value < (double)_I8_MIN)
  {
    SQL_ThrowErrorTruncate(SQL_C_DOUBLE,SQL_C_STINYINT);
  }
  return (char)p_value;
}

char
SQL_UTinyIntToTinyInt(unsigned char p_value)
{
  if(p_value > _I8_MAX)
  {
    SQL_ThrowErrorTruncate(SQL_C_UTINYINT,SQL_C_STINYINT);
  }
  return (char)p_value;
}

char
SQL_SBIGINTToTinyInt(SQLBIGINT p_value)
{
  if(p_value > _I8_MAX || p_value < _I8_MIN)
  {
    SQL_ThrowErrorTruncate(SQL_C_SBIGINT,SQL_C_STINYINT);
  }
  return (char)p_value;
}

char
SQL_UBIGINTToTinyInt(SQLUBIGINT p_value)
{
  if(p_value > _I8_MAX)
  {
    SQL_ThrowErrorTruncate(SQL_C_UBIGINT,SQL_C_STINYINT);
  }
  return (char)p_value;
}

unsigned long
SQL_ShortToULong(short p_value)
{
  if(p_value < 0)
  {
    SQL_ThrowErrorTruncate(SQL_C_SHORT,SQL_C_ULONG);
  }
  return (unsigned long)p_value;
}

unsigned long
SQL_LongToULong(long p_value)
{
  if(p_value < 0)
  {
    SQL_ThrowErrorTruncate(SQL_C_LONG,SQL_C_ULONG);
  }
  return (unsigned long)p_value;
}

unsigned long
SQL_FloatToULong(float p_value)
{
  if(p_value < 0.0 || p_value > ULONG_MAX)
  {
    SQL_ThrowErrorTruncate(SQL_C_FLOAT,SQL_C_ULONG);
  }
  return (unsigned long)p_value;
}

unsigned long
SQL_DoubleToULong(double p_value)
{
  if(p_value < 0.0 || p_value > ULONG_MAX)
  {
    SQL_ThrowErrorTruncate(SQL_C_DOUBLE,SQL_C_ULONG);
  }
  return (unsigned long)p_value;
}

unsigned long
SQL_TinyIntToULong(char p_value)
{
  if(p_value < 0)
  {
    SQL_ThrowErrorTruncate(SQL_C_STINYINT,SQL_C_ULONG);
  }
  return (unsigned long)p_value;
}

unsigned long
SQL_BIGINTToULong(SQLBIGINT p_value)
{
  if(p_value < 0 || p_value > ULONG_MAX)
  {
    SQL_ThrowErrorTruncate(SQL_C_SBIGINT,SQL_C_ULONG);
  }
  return (unsigned long)p_value;
}

unsigned long
SQL_UBIGINTToULong(SQLUBIGINT p_value)
{
  if(p_value > ULONG_MAX)
  {
    SQL_ThrowErrorTruncate(SQL_C_UBIGINT,SQL_C_ULONG);
  }
  return (unsigned long)p_value;
}

SQLUBIGINT
SQL_ShortToUBIGINT(short p_value)
{
  if(p_value < 0)
  {
    SQL_ThrowErrorTruncate(SQL_C_SHORT,SQL_C_UBIGINT);
  }
  return (SQLUBIGINT)p_value;
}

SQLUBIGINT
SQL_LongToUBIGINT(long p_value)
{
  if(p_value < 0)
  {
    SQL_ThrowErrorTruncate(SQL_C_LONG,SQL_C_UBIGINT);
  }
  return (SQLUBIGINT)p_value;
}

SQLUBIGINT
SQL_FloatToUBIGINT(float p_value)
{
  if(p_value < 0.0 || p_value > ULLONG_MAX)
  {
    SQL_ThrowErrorTruncate(SQL_C_FLOAT,SQL_C_UBIGINT);
  }
  return (SQLUBIGINT)p_value;
}

SQLUBIGINT
SQL_DoubleToUBIGINT(double p_value)
{
  if(p_value < 0.0 || p_value > ULLONG_MAX)
  {
    SQL_ThrowErrorTruncate(SQL_C_DOUBLE,SQL_C_UBIGINT);
  }
  return (SQLUBIGINT)p_value;
}

SQLUBIGINT
SQL_TinyIntToUBIGINT(char p_value)
{
  if(p_value < 0)
  {
    SQL_ThrowErrorTruncate(SQL_C_TINYINT,SQL_C_UBIGINT);
  }
  return (SQLUBIGINT)p_value;
}

SQLUBIGINT
SQL_SBIGINTToUBIGINT(SQLBIGINT p_value)
{
  if(p_value < 0)
  {
    SQL_ThrowErrorTruncate(SQL_C_SBIGINT,SQL_C_UBIGINT);
  }
  return (SQLUBIGINT)p_value;
}

unsigned short
SQL_ShortToUShort(short p_value)
{
  if(p_value < 0)
  {
    SQL_ThrowErrorTruncate(SQL_C_SHORT,SQL_C_USHORT);
  }
  return (unsigned short)p_value;
}

unsigned short
SQL_SLongToUShort(long p_value)
{
  if(p_value < 0 || p_value > SHRT_MAX)
  {
    SQL_ThrowErrorTruncate(SQL_C_LONG,SQL_C_USHORT);
  }
  return (unsigned short)p_value;
}

unsigned short
SQL_ULongToUShort(unsigned long p_value)
{
  if(p_value > SHRT_MAX)
  {
    SQL_ThrowErrorTruncate(SQL_C_ULONG,SQL_C_USHORT);
  }
  return (unsigned short)p_value;
}

unsigned short
SQL_FloatToUShort(float p_value)
{
  if(p_value < 0.0 || p_value > SHRT_MAX)
  {
    SQL_ThrowErrorTruncate(SQL_C_FLOAT,SQL_C_USHORT);
  }
  return (unsigned short)p_value;
}

unsigned short
SQL_DoubleToUShort(double p_value)
{
  if(p_value < 0.0 || p_value > SHRT_MAX)
  {
    SQL_ThrowErrorTruncate(SQL_C_DOUBLE,SQL_C_USHORT);
  }
  return (unsigned short)p_value;
}

unsigned short
SQL_TinyIntToUShort(char p_value)
{
  if(p_value < 0)
  {
    SQL_ThrowErrorTruncate(SQL_C_STINYINT,SQL_C_USHORT);
  }
  return (unsigned short)p_value;
}

unsigned short
SQL_BIGINTToUShort(SQLBIGINT p_value)
{
  if(p_value < 0.0 || p_value > SHRT_MAX)
  {
    SQL_ThrowErrorTruncate(SQL_C_SBIGINT,SQL_C_USHORT);
  }
  return (unsigned short)p_value;
}

unsigned short
SQL_UBIGINTToUShort(SQLUBIGINT p_value)
{
  if(p_value > SHRT_MAX)
  {
    SQL_ThrowErrorTruncate(SQL_C_UBIGINT,SQL_C_USHORT);
  }
  return (unsigned short)p_value;
}

unsigned char
SQL_ShortToUTinyInt(short p_value)
{
  if(p_value < 0 || p_value > _UI8_MAX)
  {
    SQL_ThrowErrorTruncate(SQL_C_SHORT,SQL_C_UTINYINT);
  }
  return (unsigned char)p_value;
}

unsigned char
SQL_UShortToUTinyInt(short p_value)
{
  if(p_value < 0 || p_value > _UI8_MAX)
  {
    SQL_ThrowErrorTruncate(SQL_C_SHORT,SQL_C_UTINYINT);
  }
  return (unsigned char)p_value;
}

unsigned char
SQL_SLongToUTinyInt(long p_value)
{
  if(p_value < 0 || p_value > _UI8_MAX)
  {
    SQL_ThrowErrorTruncate(SQL_C_LONG,SQL_C_UTINYINT);
  }
  return (unsigned char)p_value;
}

unsigned char
SQL_ULongToUTinyInt(unsigned long p_value)
{
  if(p_value > _UI8_MAX)
  {
    SQL_ThrowErrorTruncate(SQL_C_ULONG,SQL_C_UTINYINT);
  }
  return (unsigned char)p_value;
}

unsigned char
SQL_FloatToUTinyInt(float p_value)
{
  if(p_value < 0.0 || p_value > _UI8_MAX)
  {
    SQL_ThrowErrorTruncate(SQL_C_FLOAT,SQL_C_UTINYINT);
  }
  return (unsigned char)p_value;
}

unsigned char
SQL_DoubleToUTinyInt(double p_value)
{
  if(p_value < 0.0 || p_value > _UI8_MAX)
  {
    SQL_ThrowErrorTruncate(SQL_C_DOUBLE,SQL_C_UTINYINT);
  }
  return (unsigned char)p_value;
}

unsigned char
SQL_TinyIntToUTinyInt(char p_value)
{
  if(p_value < 0 || p_value > _UI8_MAX)
  {
    SQL_ThrowErrorTruncate(SQL_C_STINYINT,SQL_C_UTINYINT);
  }
  return (unsigned char)p_value;
}

unsigned char
SQL_SBIGINTToUTinyInt(SQLBIGINT p_value)
{
  if(p_value < 0 || p_value > _UI8_MAX)
  {
    SQL_ThrowErrorTruncate(SQL_C_SBIGINT,SQL_C_UTINYINT);
  }
  return (unsigned char)p_value;
}

unsigned char
SQL_UBIGINTToUTinyInt(SQLUBIGINT p_value)
{
  if(p_value > _UI8_MAX)
  {
    SQL_ThrowErrorTruncate(SQL_C_UBIGINT,SQL_C_UTINYINT);
  }
  return (unsigned char)p_value;
}

// End of namespace
}
