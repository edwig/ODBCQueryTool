////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantOperatorMod.cpp
//
// Copyright (c) 1998-2024 ir. W.E. Huisman
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
#include "bcd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{

SQLVariant 
static SQL_OperSShortModChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result %= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortModChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongModChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result %= p_right.GetAsSLong();
  return SQLVariant(result);
}
SQLVariant 
static SQL_OperULongModChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result %= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatModChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = ::fmod(p_left.GetAsFloat(),p_right.GetAsFloat());
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleModChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = ::fmod(p_left.GetAsDouble(),p_right.GetAsDouble());
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitModChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyModChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result %= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyModChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result %= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigModChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result %= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigModChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt() % p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumModChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() % bcd(p_right.GetAsChar());
  SQLVariant var(&num);
  return var;
}

// TYPE == SSHORT

SQLVariant 
static SQL_OperCharModSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result %= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortModSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result %= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortModSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant static SQL_OperSLongModSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result %= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongModSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result %= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatModSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = ::fmod(p_left.GetAsFloat(),p_right.GetAsFloat());
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleModSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = ::fmod(p_left.GetAsDouble(),p_right.GetAsDouble());
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitModSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyModSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result %= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyModSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result %= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigModSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result %= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigModSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result %= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumModSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() % bcd(p_right.GetAsSShort());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperIntYMModSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperIntDSModSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}


// TYPE == USHORT

SQLVariant 
static SQL_OperCharModUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortModUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortModUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}


SQLVariant 
static SQL_OperSLongModUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result %= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUlongModUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result %= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatModUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = ::fmod(p_left.GetAsFloat(),p_right.GetAsFloat());
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleModUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = ::fmod(p_left.GetAsDouble(),p_right.GetAsDouble());
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitModUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyModUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result %= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyModUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result %= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigModUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result %= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigModUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result %= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumModUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() % bcd((int)p_right.GetAsUShort());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperIntYMModUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperIntDSModUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == SLONG

SQLVariant 
static SQL_OperCharModSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result %= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortModSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result %= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortModSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongModSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result %= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongModSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result %= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatModSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = ::fmod(p_left.GetAsFloat(),p_right.GetAsFloat());
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleModSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = ::fmod(p_left.GetAsDouble(),p_right.GetAsDouble());
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitModSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyModSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result %= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyModSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result %= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigModSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result %= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigModSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result %= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumModSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() % bcd(p_right.GetAsSLong());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperIntYMModSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperIntDSModSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == ULONG

SQLVariant 
static SQL_OperCharModULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result %= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortModULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortModULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongModULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongModULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result %= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatModULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = ::fmod(p_left.GetAsFloat(),p_right.GetAsFloat());
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleModULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = ::fmod(p_left.GetAsDouble(),p_right.GetAsDouble());
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitModULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyModULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result %= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyModULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result %= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigModULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result %= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigModULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result %= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumModULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() % bcd((int64)p_right.GetAsUShort());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperIntYMModULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperIntDSModULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == FLOAT

SQLVariant 
static SQL_OperCharModFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = ::fmod(p_left.GetAsFloat(),p_right.GetAsFloat());
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortModFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result %= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortModFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongModFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result %= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongModFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result %= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatModFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = ::fmod(p_left.GetAsFloat(),p_right.GetAsFloat());
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleModFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = ::fmod(p_left.GetAsDouble(),p_right.GetAsDouble());
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitModFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyModFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result %= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyModFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result %= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigModFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result %= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigModFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result %= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumModFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() % bcd((double)p_right.GetAsFloat());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperIntYMModFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() % p_right.GetAsDouble();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperIntDSModFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() % p_right.GetAsDouble();
  return SQLVariant(&result);
}

// TYPE == DOUBLE

SQLVariant 
static SQL_OperCharModDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = ::fmod(p_left.GetAsDouble(),p_right.GetAsDouble());
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortModDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short result = p_left.GetAsUShort();
  result %= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortModDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongModDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result %= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongModDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result %= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatModDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = ::fmod(p_left.GetAsFloat(),p_right.GetAsFloat());
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleModDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = ::fmod(p_left.GetAsDouble(),p_right.GetAsDouble());
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitModDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyModDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result %= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyModDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result %= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigModDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result %= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigModDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result %= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumModDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() % bcd(p_right.GetAsDouble());
  SQLVariant var(&num);
  return var;
}

SQLVariant
static SQL_OperIntYMModDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() % p_right.GetAsDouble();
  return SQLVariant(&result);
}

SQLVariant
static SQL_OperIntDSModDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() % p_right.GetAsDouble();
  return SQLVariant(&result);
}

// TYPE == BIT

SQLVariant 
static SQL_OperCharModBit(const SQLVariant& p_left,const SQLVariant& /*p_right*/)
{
  bool result = p_left.GetAsBit() != 0;
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortModBit(const SQLVariant& p_left,const SQLVariant& /*p_right*/)
{
  short result = p_left.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortModBit(const SQLVariant& p_left,const SQLVariant& /*p_right*/)
{
  unsigned short result = p_left.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongModBit(const SQLVariant& p_left,const SQLVariant& /*p_right*/)
{
  int result = p_left.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongModBit(const SQLVariant& p_left,const SQLVariant& /*p_right*/)
{
  unsigned int result = p_left.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatModBit(const SQLVariant& p_left,const SQLVariant& /*p_right*/)
{
  float result = p_left.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleModBit(const SQLVariant& p_left,const SQLVariant& /*p_right*/)
{
  double result = p_left.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitModBit(const SQLVariant& p_left,const SQLVariant& /*p_right*/)
{
  bool result = p_left.GetAsBit() != 0;
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyModBit(const SQLVariant& p_left,const SQLVariant& /*p_right*/)
{
  char result = p_left.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyModBit(const SQLVariant& p_left,const SQLVariant& /*p_right*/)
{
  unsigned char result = p_left.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigModBit(const SQLVariant& p_left,const SQLVariant& /*p_right*/)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigModBit(const SQLVariant& p_left,const SQLVariant& /*p_right*/)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumModBit(const SQLVariant& p_left,const SQLVariant& /*p_right*/)
{
  SQLVariant result(p_left);
  return result;
}

SQLVariant 
static SQL_OperIntYMModBit(const SQLVariant& p_left,const SQLVariant& /*p_right*/)
{
  SQLInterval result = p_left.GetAsSQLInterval();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperIntDSModBit(const SQLVariant& p_left,const SQLVariant& /*p_right*/)
{
  SQLInterval result = p_left.GetAsSQLInterval();
  return SQLVariant(&result);
}

// TYPE == STINYINT

SQLVariant 
static SQL_OperCharModSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result %= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortModSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result %= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortModSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongModSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result %= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongModSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result %= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatModSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = ::fmod(p_left.GetAsFloat(),p_right.GetAsFloat());
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleModSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = ::fmod(p_left.GetAsDouble(),p_right.GetAsDouble());
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitModSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyModSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result %= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyModSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result %= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigModSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result %= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigModSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result %= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumModSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() % bcd(p_right.GetAsSTinyInt());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperIntYMModSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperIntDSModSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE = UTINYINT

SQLVariant 
static SQL_OperCharModUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result %= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortModUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result %= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortModUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongModUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result %= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongModUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result %= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatModUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = ::fmod(p_left.GetAsFloat(),p_right.GetAsFloat());
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleModUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = ::fmod(p_left.GetAsDouble(),p_right.GetAsDouble());
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitModUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyModUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result %= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyModUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result %= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigModUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result %= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigModUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result %= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumModUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() % bcd((int)p_right.GetAsUTinyInt());
  SQLVariant var(&num);
  return var;
}

SQLVariant
static SQL_OperIntYMModUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant
static SQL_OperIntDSModUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}


// TYPE == SBIGINT

SQLVariant 
static SQL_OperCharModSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result %= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortModSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result %= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortModSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongModSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result %= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongModSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result %= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatModSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = ::fmod(p_left.GetAsFloat(),p_right.GetAsFloat());
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleModSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = ::fmod(p_left.GetAsDouble(),p_right.GetAsDouble());
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitModSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyModSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result %= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyModSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result %= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigModSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result %= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigModSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result %= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumModSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() % bcd(p_right.GetAsSBigInt());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperIntYMModSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperIntDSModSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == UBIGINT

SQLVariant 
static SQL_OperCharModUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result %= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortModUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result %= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortModUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongModUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result %= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongModUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result %= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatModUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = ::fmod(p_left.GetAsFloat(),p_right.GetAsFloat());
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleModUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = ::fmod(p_left.GetAsDouble(),p_right.GetAsDouble());
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitModUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyModUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result %= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyModUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result %= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigModUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result %= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigModUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result %= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumModUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() % bcd(p_right.GetAsUBigInt());
  SQLVariant var(&num);
  return var;
}

SQLVariant
static SQL_OperIntYMModUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant
static SQL_OperIntDSModUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == NUMERIC

SQLVariant 
static SQL_OperCharModNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double num = ::fmod(p_left.GetAsDouble(),p_right.GetAsBCD().AsDouble());
  XString str;
  str.Format(_T("%lf"),num);
  SQLVariant var(str);
  return var;
}

SQLVariant 
static SQL_OperSShortModNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short num = p_left.GetAsSShort() % p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperUShortModNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short num = p_left.GetAsUShort() % p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperSLongModNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int num = p_left.GetAsSLong() % p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperULongModNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int num = p_left.GetAsULong() % p_right.GetAsBCD().AsInt64();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperFloatModNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float num = (float) ::fmod(p_left.GetAsFloat(),p_right.GetAsBCD().AsDouble());
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperDoubleModNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double num = ::fmod(p_left.GetAsDouble(),p_right.GetAsBCD().AsDouble());
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperBitModNum(const SQLVariant& p_left,const SQLVariant& /*p_right*/)
{
  SQLVariant var(p_left.GetAsBit());
  return var;
}

SQLVariant 
static SQL_OperSTinyModNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char num = p_left.GetAsSTinyInt() % (char) p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperUTinyModNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char num = p_left.GetAsUTinyInt() % (unsigned char)p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperSBigModNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int64 num = p_left.GetAsSBigInt() % p_right.GetAsBCD().AsInt64();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperUBigModNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  uint64 num = p_left.GetAsUBigInt() % p_right.GetAsBCD().AsUInt64();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperNumModNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() % p_right.GetAsBCD();
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperIntYMModNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() % p_right.GetAsDouble();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperIntDSModNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() % p_right.GetAsDouble();
  return SQLVariant(&result);
}

// OPERATOR ARRAY

static CalculateFunctionArray OperatorMod[CT_LAST][CT_LAST] =
{
                         // CT_CHAR                CT_SSHORT                CT_USHORT                CT_SLONG                CT_ULONG                CT_FLOAT                CT_DOUBLE                CT_BIT                CT_STINYINT             CT_UTINYINT             CT_SBIGINT             CT_UBIGINT             CT_NUMERIC            CT_GUID  CT_BINARY CT_DATE  CT_TIME  CT_TIMESTAMP CT_INTERVAL_YM CT_INTERVAL_DS
                         // ---------------------- ------------------------ ------------------------ ----------------------- ----------------------- ----------------------- ------------------------ --------------------- ----------------------- ----------------------- ---------------------- ---------------------- --------------------- -------- --------- -------- -------- ------------ -------------- --------------
  /* CT_CHAR        */   {  nullptr               ,&SQL_OperCharModSShort  ,&SQL_OperCharModUShort  ,&SQL_OperCharModSLong  ,&SQL_OperCharModULong  ,&SQL_OperCharModFloat  ,&SQL_OperCharModDouble  ,&SQL_OperCharModBit  ,&SQL_OperCharModSTiny  ,&SQL_OperCharModUTiny  ,&SQL_OperCharModSBig  ,&SQL_OperCharModUBig  ,&SQL_OperCharModNum  ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_SSHORT      */  ,{  &SQL_OperSShortModChar,&SQL_OperSShortModSShort,&SQL_OperSShortModUShort,&SQL_OperSShortModSLong,&SQL_OperSShortModULong,&SQL_OperSShortModFloat,&SQL_OperSShortModDouble,&SQL_OperSShortModBit,&SQL_OperSShortModSTiny,&SQL_OperSShortModUTiny,&SQL_OperSShortModSBig,&SQL_OperSShortModUBig,&SQL_OperSShortModNum,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_USHORT      */  ,{  &SQL_OperUShortModChar,&SQL_OperUShortModSShort,&SQL_OperUShortModUShort,&SQL_OperUShortModSLong,&SQL_OperUShortModULong,&SQL_OperUShortModFloat,&SQL_OperUShortModDouble,&SQL_OperUShortModBit,&SQL_OperUShortModSTiny,&SQL_OperUShortModUTiny,&SQL_OperUShortModSBig,&SQL_OperUShortModUBig,&SQL_OperUShortModNum,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_SLONG       */  ,{  &SQL_OperSLongModChar ,&SQL_OperSLongModSShort ,&SQL_OperSLongModUShort ,&SQL_OperSLongModSLong ,&SQL_OperSLongModULong ,&SQL_OperSLongModFloat ,&SQL_OperSLongModDouble ,&SQL_OperSLongModBit ,&SQL_OperSLongModSTiny ,&SQL_OperSLongModUTiny ,&SQL_OperSLongModSBig ,&SQL_OperSLongModUBig ,&SQL_OperSLongModNum ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_ULONG       */  ,{  &SQL_OperULongModChar ,&SQL_OperULongModSShort ,&SQL_OperUlongModUShort ,&SQL_OperULongModSLong ,&SQL_OperULongModULong ,&SQL_OperULongModFloat ,&SQL_OperULongModDouble ,&SQL_OperULongModBit ,&SQL_OperULongModSTiny ,&SQL_OperULongModUTiny ,&SQL_OperULongModSBig ,&SQL_OperULongModUBig ,&SQL_OperULongModNum ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_FLOAT       */  ,{  &SQL_OperFloatModChar ,&SQL_OperFloatModSShort ,&SQL_OperFloatModUShort ,&SQL_OperFloatModSLong ,&SQL_OperFloatModULong ,&SQL_OperFloatModFloat ,&SQL_OperFloatModDouble ,&SQL_OperFloatModBit ,&SQL_OperFloatModSTiny ,&SQL_OperFloatModUTiny ,&SQL_OperFloatModSBig ,&SQL_OperFloatModUBig ,&SQL_OperFloatModNum ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_DOUBLE      */  ,{  &SQL_OperDoubleModChar,&SQL_OperDoubleModSShort,&SQL_OperDoubleModUShort,&SQL_OperDoubleModSLong,&SQL_OperDoubleModULong,&SQL_OperDoubleModFloat,&SQL_OperDoubleModDouble,&SQL_OperDoubleModBit,&SQL_OperDoubleModSTiny,&SQL_OperDoubleModUTiny,&SQL_OperDoubleModSBig,&SQL_OperDoubleModUBig,&SQL_OperDoubleModNum,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_BIT         */  ,{  &SQL_OperBitModChar   ,&SQL_OperBitModSShort   ,&SQL_OperBitModUShort   ,&SQL_OperBitModSLong   ,&SQL_OperBitModULong   ,&SQL_OperBitModFloat   ,&SQL_OperBitModDouble   ,&SQL_OperBitModBit   ,&SQL_OperBitModSTiny   ,&SQL_OperBitModUTiny   ,&SQL_OperBitModSBig   ,&SQL_OperBitModUBig   ,&SQL_OperBitModNum   ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_STINYINT    */  ,{  &SQL_OperSTinyModChar ,&SQL_OperSTinyModSShort ,&SQL_OperSTinyModUShort ,&SQL_OperSTinyModSLong ,&SQL_OperSTinyModULong ,&SQL_OperSTinyModFloat ,&SQL_OperSTinyModDouble ,&SQL_OperSTinyModBit ,&SQL_OperSTinyModSTiny ,&SQL_OperSTinyModUTiny ,&SQL_OperSTinyModSBig ,&SQL_OperSTinyModUBig ,&SQL_OperSTinyModNum ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_UTINYINT    */  ,{  &SQL_OperUTinyModChar ,&SQL_OperUTinyModSShort ,&SQL_OperUTinyModUShort ,&SQL_OperUTinyModSLong ,&SQL_OperUTinyModULong ,&SQL_OperUTinyModFloat ,&SQL_OperUTinyModDouble ,&SQL_OperUTinyModBit ,&SQL_OperUTinyModSTiny ,&SQL_OperUTinyModUTiny ,&SQL_OperUTinyModSBig ,&SQL_OperUTinyModUBig ,&SQL_OperUTinyModNum ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_SBIGINT     */  ,{  &SQL_OperSBigModChar  ,&SQL_OperSBigModSShort  ,&SQL_OperSBigModUShort  ,&SQL_OperSBigModSLong  ,&SQL_OperSBigModULong  ,&SQL_OperSBigModFloat  ,&SQL_OperSBigModDouble  ,&SQL_OperSBigModBit  ,&SQL_OperSBigModSTiny  ,&SQL_OperSBigModUTiny  ,&SQL_OperSBigModSBig  ,&SQL_OperSBigModUBig  ,&SQL_OperSBigModNum  ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_UBIGINT     */  ,{  &SQL_OperUBigModChar  ,&SQL_OperUBigModSShort  ,&SQL_OperUBigModUShort  ,&SQL_OperUBigModSLong  ,&SQL_OperUBigModULong  ,&SQL_OperUBigModFloat  ,&SQL_OperUBigModDouble  ,&SQL_OperUBigModBit  ,&SQL_OperUBigModSTiny  ,&SQL_OperUBigModUTiny  ,&SQL_OperUBigModSBig  ,&SQL_OperUBigModUBig  ,&SQL_OperUBigModNum  ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_NUMERIC     */  ,{  &SQL_OperNumModChar   ,&SQL_OperNumModSShort   ,&SQL_OperNumModUShort   ,&SQL_OperNumModSLong   ,&SQL_OperNumModULong   ,&SQL_OperNumModFloat   ,&SQL_OperNumModDouble   ,&SQL_OperNumModBit   ,&SQL_OperNumModSTiny   ,&SQL_OperNumModUTiny   ,&SQL_OperNumModSBig   ,&SQL_OperNumModUBig   ,&SQL_OperNumModNum   ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_GUID        */  ,{  nullptr               ,nullptr                 ,nullptr                 ,nullptr                ,nullptr                ,nullptr                ,nullptr                 ,nullptr              ,nullptr                ,nullptr                ,nullptr               ,nullptr               ,nullptr              ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_BINARY      */  ,{  nullptr               ,nullptr                 ,nullptr                 ,nullptr                ,nullptr                ,nullptr                ,nullptr                 ,nullptr              ,nullptr                ,nullptr                ,nullptr               ,nullptr               ,nullptr              ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_DATE        */  ,{  nullptr               ,nullptr                 ,nullptr                 ,nullptr                ,nullptr                ,nullptr                ,nullptr                 ,nullptr              ,nullptr                ,nullptr                ,nullptr               ,nullptr               ,nullptr              ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_TIME        */  ,{  nullptr               ,nullptr                 ,nullptr                 ,nullptr                ,nullptr                ,nullptr                ,nullptr                 ,nullptr              ,nullptr                ,nullptr                ,nullptr               ,nullptr               ,nullptr              ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_TIMESTAMP   */  ,{  nullptr               ,nullptr                 ,nullptr                 ,nullptr                ,nullptr                ,nullptr                ,nullptr                 ,nullptr              ,nullptr                ,nullptr                ,nullptr               ,nullptr               ,nullptr              ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_INTERVAL_YM */  ,{  nullptr               ,&SQL_OperIntYMModSShort ,&SQL_OperIntYMModUShort ,&SQL_OperIntYMModSLong ,&SQL_OperIntYMModULong ,&SQL_OperIntYMModFloat ,&SQL_OperIntYMModDouble ,&SQL_OperIntYMModBit ,&SQL_OperIntYMModSTiny ,&SQL_OperIntYMModUTiny ,&SQL_OperIntYMModSBig ,&SQL_OperIntYMModUBig ,&SQL_OperIntYMModNum ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_INTERVAL_DS */  ,{  nullptr               ,&SQL_OperIntDSModSShort ,&SQL_OperIntDSModUShort ,&SQL_OperIntDSModSLong ,&SQL_OperIntDSModULong ,&SQL_OperIntDSModFloat ,&SQL_OperIntDSModDouble ,&SQL_OperIntDSModBit ,&SQL_OperIntDSModSTiny ,&SQL_OperIntDSModUTiny ,&SQL_OperIntDSModSBig ,&SQL_OperIntDSModUBig ,&SQL_OperIntDSModNum ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
};

// Modulo operator for SQLVariant
SQLVariant
SQLVariant::operator%(const SQLVariant& p_right)
{
  // If one of both is NULL, the result is false
  if(IsNULL() || p_right.IsNULL())
  {
    return SQLVariant();
  }

  // Getting the concise type
  SQLConciseType left  = SQLTypeToConciseType(m_datatype);
  SQLConciseType right = SQLTypeToConciseType(p_right.m_datatype);

  // Check whether both datatypes are valid
  if(left == CT_LAST || right == CT_LAST)
  {
    return SQLVariant();
  }

  // Check whether there is something to modulo by!
  if(p_right.IsEmpty())
  {
    throw StdException("Cannot do a modulo by zero/empty");
  }

  // Find our comparison function
  OperatorCalculate function = OperatorMod[left][right].function;
  if(function)
  {
    return (*function)(*this,p_right);
  }
  // No compare function found
  // Data types are not comparable
  XString leftType  = SQLDataType::FindDatatype(m_datatype);
  XString rightType = SQLDataType::FindDatatype(p_right.m_datatype);
  XString error;
  error.Format(_T("Cannot do the modulo operator on (%s %% %s)"),leftType.GetString(),rightType.GetString());
  throw StdException(error);
}

SQLVariant& 
SQLVariant::operator%=(const SQLVariant& p_right)
{
  // If one of both is NULL, the result is false
  if(IsNULL() || p_right.IsNULL())
  {
    SetNULL();
    return *this;
  }

  // Getting the concise type
  SQLConciseType left  = SQLTypeToConciseType(m_datatype);
  SQLConciseType right = SQLTypeToConciseType(p_right.m_datatype);

  // Find our comparison function
  // Check whether both datatypes are valid
  if(left >= 0 && left < CT_LAST && right >= 0 && right < CT_LAST)
  {
    OperatorCalculate function = OperatorMod[left][right].function;
    if(function)
    {
      *this = (*function)(*this,p_right);
      return *this;
    }
  }
  else
  {
    ThrowErrorOperator(SVO_AssignModulo);
  }
  // No compare function found
  // Data types are not comparable
  XString leftType  = SQLDataType::FindDatatype(m_datatype);
  XString rightType = SQLDataType::FindDatatype(p_right.m_datatype);
  XString error;
  error.Format(_T("Cannot do the %= operator on (%s + %s)"),leftType.GetString(),rightType.GetString());
  throw StdException(error);
}

// End of namespace
}
