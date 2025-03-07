////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantOperatorMul.cpp
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
#include "bcd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{

SQLVariant 
static SQL_OperSShortMulChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result *= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortMulChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongMulChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result *= p_right.GetAsSLong();
  return SQLVariant(result);
}
SQLVariant 
static SQL_OperULongMulChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int  result = p_left.GetAsULong();
  result *= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatMulChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result *= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleMulChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result *= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitMulChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyMulChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result *= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyMulChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result *= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigMulChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result *= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigMulChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result *= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumMulChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() * bcd(p_right.GetAsString());
  SQLVariant var(&num);
  return var;
}

// TYPE == SSHORT

SQLVariant 
static SQL_OperCharMulSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result *= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortMulSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result *= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortMulSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant static SQL_OperSLongMulSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result *= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongMulSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result *= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatMulSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result *= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleMulSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result *= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitMulSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyMulSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result *= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyMulSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result *= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigMulSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result *= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigMulSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result *= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumMulSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() * bcd(p_right.GetAsSShort());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperIntYMMulSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperIntDSMulSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}


// TYPE == USHORT

SQLVariant 
static SQL_OperCharMulUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortMulUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortMulUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}


SQLVariant 
static SQL_OperSLongMulUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result *= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUlongMulUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result *= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatMulUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result *= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleMulUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result *= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitMulUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyMulUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result *= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyMulUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result *= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigMulUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result *= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigMulUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result *= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumMulUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() * bcd((long)p_right.GetAsUShort());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperIntYMMulUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperIntDSMulUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == SLONG

SQLVariant 
static SQL_OperCharMulSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result *= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortMulSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result *= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortMulSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongMulSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result *= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongMulSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result *= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatMulSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result *= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleMulSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result *= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitMulSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyMulSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result *= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyMulSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result *= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigMulSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result *= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigMulSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result *= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumMulSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() * bcd(p_right.GetAsSLong());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperIntYMMulSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperIntDSMulSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == ULONG

SQLVariant 
static SQL_OperCharMulULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result *= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortMulULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortMulULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongMulULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongMulULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result *= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatMulULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result *= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleMulULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result *= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitMulULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyMulULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result *= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyMulULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result *= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigMulULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result *= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigMulULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result *= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumMulULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() * bcd((int64)p_right.GetAsULong());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperIntYMMulULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperIntDSMulULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == FLOAT

SQLVariant 
static SQL_OperCharMulFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result *= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortMulFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result *= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortMulFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongMulFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result *= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongMulFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result *= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatMulFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result *= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleMulFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result *= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitMulFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyMulFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result *= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyMulFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result *= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigMulFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result *= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigMulFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result *= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumMulFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() * bcd((double)p_right.GetAsFloat());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperIntYMMulFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsDouble();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperIntDSMulFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsDouble();
  return SQLVariant(&result);
}

// TYPE == DOUBLE

SQLVariant 
static SQL_OperCharMulDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result *= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortMulDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short result = p_left.GetAsUShort();
  result *= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortMulDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongMulDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result *= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongMulDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result *= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatMulDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result *= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleMulDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result *= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitMulDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyMulDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result *= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyMulDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result *= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigMulDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result *= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigMulDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result *= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumMulDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() * bcd(p_right.GetAsDouble());
  SQLVariant var(&num);
  return var;
}

SQLVariant
static SQL_OperIntYMMulDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsDouble();
  return SQLVariant(&result);
}

SQLVariant
static SQL_OperIntDSMulDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsDouble();
  return SQLVariant(&result);
}


// TYPE == BIT

SQLVariant 
static SQL_OperCharMulBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortMulBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  if(p_right.GetAsBit() == 0) 
  {
    result = 0;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortMulBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  if(p_right.GetAsBit() == 0)
  {
    result = 0;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongMulBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  if(p_right.GetAsBit() == 0)
  {
    result = 0;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongMulBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  if(p_right.GetAsBit() == 0)
  {
    result = 0;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatMulBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  if(p_right.GetAsBit() == 0)
  {
    result = 0.0;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleMulBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  if(p_right.GetAsBit() == 0)
  {
    result = 0.0;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitMulBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyMulBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  if(p_right.GetAsBit() == 0)
  {
    result = 0;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyMulBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  if(p_right.GetAsBit() == 0)
  {
    result = 0;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigMulBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  if(p_right.GetAsBit() == 0)
  {
    result = 0;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigMulBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  if(p_right.GetAsBit() == 0)
  {
    result = 0;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumMulBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() * bcd(p_right.GetAsBit());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperIntYMMulBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval();
  if(p_right.GetAsBit() == 0)
  {
    // Set 0 years, 0 months
    result.SetInterval(result.GetIntervalType(),0,0);
  }
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperIntDSMulBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval();
  if(p_right.GetAsBit() == 0)
  {
    // Set 0 days, 0 hours, 0 minutes, 0 seconds, 0 fraction
    result.SetInterval(result.GetIntervalType(),0,0,0,0,0);
  }
  return SQLVariant(&result);
}

// TYPE == STINYINT

SQLVariant 
static SQL_OperCharMulSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result *= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortMulSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result *= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortMulSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongMulSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result *= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongMulSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result *= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatMulSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result *= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleMulSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result *= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitMulSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyMulSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result *= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyMulSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result *= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigMulSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result *= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigMulSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result *= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumMulSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() * bcd(p_right.GetAsSTinyInt());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperIntYMMulSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperIntDSMulSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE = UTINYINT

SQLVariant 
static SQL_OperCharMulUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result *= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortMulUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result *= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortMulUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongMulUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result *= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongMulUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result *= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatMulUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result *= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleMulUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result *= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitMulUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyMulUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result *= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyMulUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result *= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigMulUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result *= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigMulUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result *= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumMulUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() * bcd((short)p_right.GetAsUTinyInt());
  SQLVariant var(&num);
  return var;
}

SQLVariant
static SQL_OperIntYMMulUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant
static SQL_OperIntDSMulUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}


// TYPE == SBIGINT

SQLVariant 
static SQL_OperCharMulSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result *= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortMulSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result *= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortMulSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongMulSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result *= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongMulSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result *= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatMulSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result *= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleMulSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result *= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitMulSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyMulSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result *= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyMulSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result *= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigMulSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result *= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigMulSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result *= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumMulSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() * bcd(p_right.GetAsSBigInt());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperIntYMMulSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperIntDSMulSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == UBIGINT

SQLVariant 
static SQL_OperCharMulUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result *= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortMulUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result *= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortMulUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongMulUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result *= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongMulUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result *= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatMulUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result *= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleMulUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result *= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitMulUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyMulUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result *= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyMulUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result *= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigMulUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result *= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigMulUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result *= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumMulUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() * bcd(p_right.GetAsUBigInt());
  SQLVariant var(&num);
  return var;
}

SQLVariant
static SQL_OperIntYMMulUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant
static SQL_OperIntDSMulUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == NUMERIC

SQLVariant 
static SQL_OperCharMulNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = bcd(p_left.GetAsString()) * p_right.GetAsBCD();
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperSShortMulNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = bcd(p_left.GetAsSShort()) * p_right.GetAsBCD();
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperUShortMulNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = bcd((long)p_left.GetAsUShort()) * p_right.GetAsBCD();
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperSLongMulNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = bcd(p_left.GetAsSLong()) * p_right.GetAsBCD();
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperULongMulNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = bcd((int64)p_left.GetAsULong()) * p_right.GetAsBCD();
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperFloatMulNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = bcd((double)p_left.GetAsFloat()) * p_right.GetAsBCD();
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperDoubleMulNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = bcd(p_left.GetAsDouble()) * p_right.GetAsBCD();
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperBitMulNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = bcd(p_left.GetAsBit()) * p_right.GetAsBCD();
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperSTinyMulNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = bcd(p_left.GetAsSTinyInt()) * p_right.GetAsBCD();
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperUTinyMulNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = bcd((short)p_left.GetAsUTinyInt()) * p_right.GetAsBCD();
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperSBigMulNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = bcd(p_left.GetAsSBigInt()) * p_right.GetAsBCD();
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperUBigMulNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = bcd(p_left.GetAsUBigInt()) * p_right.GetAsBCD();
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperNumMulNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() * p_right.GetAsBCD();
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperIntYMMulNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsDouble();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperIntDSMulNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsDouble();
  return SQLVariant(&result);
}

// TYPE = INTERVAL YM

SQLVariant 
static SQL_OperSShortMulIntYM(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_right.GetAsSQLInterval() * p_left.GetAsSShort();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperUShortMulIntYM(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_right.GetAsSQLInterval() * p_left.GetAsUShort();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperSLongMulIntYM(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_right.GetAsSQLInterval() * p_left.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperULongMulIntYM(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_right.GetAsSQLInterval() * p_left.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperFloatMulIntYM(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_right.GetAsSQLInterval() * p_left.GetAsFloat();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperDoubleMulIntYM(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_right.GetAsSQLInterval() * p_left.GetAsDouble();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperBitMulIntYM(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_right.GetAsSQLInterval() * p_left.GetAsBit();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperSTinyMulIntYM(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_right.GetAsSQLInterval() * p_left.GetAsSTinyInt();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperUTinyMulIntYM(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_right.GetAsSQLInterval() * (int)p_left.GetAsUTinyInt();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperSBigMulIntYM(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_right.GetAsSQLInterval() * p_left.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperUBigMulIntYM(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_right.GetAsSQLInterval() * p_left.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperNumMulIntYM(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_right.GetAsSQLInterval() * p_left.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE = INTERVAL DS

SQLVariant
static SQL_OperSShortMulIntDS(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_right.GetAsSQLInterval() * p_left.GetAsSShort();
  return SQLVariant(&result);
}

SQLVariant
static SQL_OperUShortMulIntDS(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_right.GetAsSQLInterval() * p_left.GetAsUShort();
  return SQLVariant(&result);
}

SQLVariant
static SQL_OperSLongMulIntDS(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_right.GetAsSQLInterval() * p_left.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant
static SQL_OperULongMulIntDS(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_right.GetAsSQLInterval() * p_left.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant
static SQL_OperFloatMulIntDS(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_right.GetAsSQLInterval() * p_left.GetAsFloat();
  return SQLVariant(&result);
}

SQLVariant
static SQL_OperDoubleMulIntDS(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_right.GetAsSQLInterval() * p_left.GetAsDouble();
  return SQLVariant(&result);
}

SQLVariant
static SQL_OperBitMulIntDS(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_right.GetAsSQLInterval() * p_left.GetAsBit();
  return SQLVariant(&result);
}

SQLVariant
static SQL_OperSTinyMulIntDS(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_right.GetAsSQLInterval() * (int)p_left.GetAsSTinyInt();
  return SQLVariant(&result);
}

SQLVariant
static SQL_OperUTinyMulIntDS(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_right.GetAsSQLInterval() * (int)p_left.GetAsUTinyInt();
  return SQLVariant(&result);
}

SQLVariant
static SQL_OperSBigMulIntDS(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_right.GetAsSQLInterval() * p_left.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant
static SQL_OperUBigMulIntDS(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_right.GetAsSQLInterval() * p_left.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant
static SQL_OperNumMulIntDS(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_right.GetAsSQLInterval() * p_left.GetAsSLong();
  return SQLVariant(&result);
}

// OPERATOR ARRAY

static CalculateFunctionArray OperatorMul[CT_LAST][CT_LAST] =
{
                         // CT_CHAR                CT_SSHORT                CT_USHORT                CT_SLONG                CT_ULONG                CT_FLOAT                CT_DOUBLE                CT_BIT                CT_STINYINT             CT_UTINYINT             CT_SBIGINT             CT_UBIGINT             CT_NUMERIC            CT_GUID  CT_BINARY CT_DATE  CT_TIME  CT_TIMESTAMP CT_INTERVAL_YM          CT_INTERVAL_DS
                         // ---------------------- ------------------------ ------------------------ ----------------------- ----------------------- ----------------------- ------------------------ --------------------- ----------------------- ----------------------- ---------------------- ---------------------- --------------------- -------- --------- -------- -------- ------------ ----------------------- -------------------------
  /* CT_CHAR        */   {  nullptr               ,&SQL_OperCharMulSShort  ,&SQL_OperCharMulUShort  ,&SQL_OperCharMulSLong  ,&SQL_OperCharMulULong  ,&SQL_OperCharMulFloat  ,&SQL_OperCharMulDouble  ,&SQL_OperCharMulBit  ,&SQL_OperCharMulSTiny  ,&SQL_OperCharMulUTiny  ,&SQL_OperCharMulSBig  ,&SQL_OperCharMulUBig  ,&SQL_OperCharMulNum  ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr                ,nullptr                 }
  /* CT_SSHORT      */  ,{  &SQL_OperSShortMulChar,&SQL_OperSShortMulSShort,&SQL_OperSShortMulUShort,&SQL_OperSShortMulSLong,&SQL_OperSShortMulULong,&SQL_OperSShortMulFloat,&SQL_OperSShortMulDouble,&SQL_OperSShortMulBit,&SQL_OperSShortMulSTiny,&SQL_OperSShortMulUTiny,&SQL_OperSShortMulSBig,&SQL_OperSShortMulUBig,&SQL_OperSShortMulNum,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,&SQL_OperSShortMulIntYM,&SQL_OperSShortMulIntDS }
  /* CT_USHORT      */  ,{  &SQL_OperUShortMulChar,&SQL_OperUShortMulSShort,&SQL_OperUShortMulUShort,&SQL_OperUShortMulSLong,&SQL_OperUShortMulULong,&SQL_OperUShortMulFloat,&SQL_OperUShortMulDouble,&SQL_OperUShortMulBit,&SQL_OperUShortMulSTiny,&SQL_OperUShortMulUTiny,&SQL_OperUShortMulSBig,&SQL_OperUShortMulUBig,&SQL_OperUShortMulNum,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,&SQL_OperUShortMulIntYM,&SQL_OperUShortMulIntDS }
  /* CT_SLONG       */  ,{  &SQL_OperSLongMulChar ,&SQL_OperSLongMulSShort ,&SQL_OperSLongMulUShort ,&SQL_OperSLongMulSLong ,&SQL_OperSLongMulULong ,&SQL_OperSLongMulFloat ,&SQL_OperSLongMulDouble ,&SQL_OperSLongMulBit ,&SQL_OperSLongMulSTiny ,&SQL_OperSLongMulUTiny ,&SQL_OperSLongMulSBig ,&SQL_OperSLongMulUBig ,&SQL_OperSLongMulNum ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,&SQL_OperSLongMulIntYM ,&SQL_OperSLongMulIntDS  }
  /* CT_ULONG       */  ,{  &SQL_OperULongMulChar ,&SQL_OperULongMulSShort ,&SQL_OperUlongMulUShort ,&SQL_OperULongMulSLong ,&SQL_OperULongMulULong ,&SQL_OperULongMulFloat ,&SQL_OperULongMulDouble ,&SQL_OperULongMulBit ,&SQL_OperULongMulSTiny ,&SQL_OperULongMulUTiny ,&SQL_OperULongMulSBig ,&SQL_OperULongMulUBig ,&SQL_OperULongMulNum ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,&SQL_OperULongMulIntYM ,&SQL_OperULongMulIntDS  }
  /* CT_FLOAT       */  ,{  &SQL_OperFloatMulChar ,&SQL_OperFloatMulSShort ,&SQL_OperFloatMulUShort ,&SQL_OperFloatMulSLong ,&SQL_OperFloatMulULong ,&SQL_OperFloatMulFloat ,&SQL_OperFloatMulDouble ,&SQL_OperFloatMulBit ,&SQL_OperFloatMulSTiny ,&SQL_OperFloatMulUTiny ,&SQL_OperFloatMulSBig ,&SQL_OperFloatMulUBig ,&SQL_OperFloatMulNum ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,&SQL_OperFloatMulIntYM ,&SQL_OperFloatMulIntDS  }
  /* CT_DOUBLE      */  ,{  &SQL_OperDoubleMulChar,&SQL_OperDoubleMulSShort,&SQL_OperDoubleMulUShort,&SQL_OperDoubleMulSLong,&SQL_OperDoubleMulULong,&SQL_OperDoubleMulFloat,&SQL_OperDoubleMulDouble,&SQL_OperDoubleMulBit,&SQL_OperDoubleMulSTiny,&SQL_OperDoubleMulUTiny,&SQL_OperDoubleMulSBig,&SQL_OperDoubleMulUBig,&SQL_OperDoubleMulNum,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,&SQL_OperDoubleMulIntYM,&SQL_OperDoubleMulIntDS }
  /* CT_BIT         */  ,{  &SQL_OperBitMulChar   ,&SQL_OperBitMulSShort   ,&SQL_OperBitMulUShort   ,&SQL_OperBitMulSLong   ,&SQL_OperBitMulULong   ,&SQL_OperBitMulFloat   ,&SQL_OperBitMulDouble   ,&SQL_OperBitMulBit   ,&SQL_OperBitMulSTiny   ,&SQL_OperBitMulUTiny   ,&SQL_OperBitMulSBig   ,&SQL_OperBitMulUBig   ,&SQL_OperBitMulNum   ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,&SQL_OperBitMulIntYM   ,&SQL_OperBitMulIntDS    }
  /* CT_STINYINT    */  ,{  &SQL_OperSTinyMulChar ,&SQL_OperSTinyMulSShort ,&SQL_OperSTinyMulUShort ,&SQL_OperSTinyMulSLong ,&SQL_OperSTinyMulULong ,&SQL_OperSTinyMulFloat ,&SQL_OperSTinyMulDouble ,&SQL_OperSTinyMulBit ,&SQL_OperSTinyMulSTiny ,&SQL_OperSTinyMulUTiny ,&SQL_OperSTinyMulSBig ,&SQL_OperSTinyMulUBig ,&SQL_OperSTinyMulNum ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,&SQL_OperSTinyMulIntYM ,&SQL_OperSTinyMulIntDS  }
  /* CT_UTINYINT    */  ,{  &SQL_OperUTinyMulChar ,&SQL_OperUTinyMulSShort ,&SQL_OperUTinyMulUShort ,&SQL_OperUTinyMulSLong ,&SQL_OperUTinyMulULong ,&SQL_OperUTinyMulFloat ,&SQL_OperUTinyMulDouble ,&SQL_OperUTinyMulBit ,&SQL_OperUTinyMulSTiny ,&SQL_OperUTinyMulUTiny ,&SQL_OperUTinyMulSBig ,&SQL_OperUTinyMulUBig ,&SQL_OperUTinyMulNum ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,&SQL_OperUTinyMulIntYM ,&SQL_OperUTinyMulIntDS  }
  /* CT_SBIGINT     */  ,{  &SQL_OperSBigMulChar  ,&SQL_OperSBigMulSShort  ,&SQL_OperSBigMulUShort  ,&SQL_OperSBigMulSLong  ,&SQL_OperSBigMulULong  ,&SQL_OperSBigMulFloat  ,&SQL_OperSBigMulDouble  ,&SQL_OperSBigMulBit  ,&SQL_OperSBigMulSTiny  ,&SQL_OperSBigMulUTiny  ,&SQL_OperSBigMulSBig  ,&SQL_OperSBigMulUBig  ,&SQL_OperSBigMulNum  ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,&SQL_OperSBigMulIntYM  ,&SQL_OperSBigMulIntDS   }
  /* CT_UBIGINT     */  ,{  &SQL_OperUBigMulChar  ,&SQL_OperUBigMulSShort  ,&SQL_OperUBigMulUShort  ,&SQL_OperUBigMulSLong  ,&SQL_OperUBigMulULong  ,&SQL_OperUBigMulFloat  ,&SQL_OperUBigMulDouble  ,&SQL_OperUBigMulBit  ,&SQL_OperUBigMulSTiny  ,&SQL_OperUBigMulUTiny  ,&SQL_OperUBigMulSBig  ,&SQL_OperUBigMulUBig  ,&SQL_OperUBigMulNum  ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,&SQL_OperUBigMulIntYM  ,&SQL_OperUBigMulIntDS   }
  /* CT_NUMERIC     */  ,{  &SQL_OperNumMulChar   ,&SQL_OperNumMulSShort   ,&SQL_OperNumMulUShort   ,&SQL_OperNumMulSLong   ,&SQL_OperNumMulULong   ,&SQL_OperNumMulFloat   ,&SQL_OperNumMulDouble   ,&SQL_OperNumMulBit   ,&SQL_OperNumMulSTiny   ,&SQL_OperNumMulUTiny   ,&SQL_OperNumMulSBig   ,&SQL_OperNumMulUBig   ,&SQL_OperNumMulNum   ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,&SQL_OperNumMulIntYM   ,&SQL_OperNumMulIntDS    }
  /* CT_GUID        */  ,{  nullptr               ,nullptr                 ,nullptr                 ,nullptr                ,nullptr                ,nullptr                ,nullptr                 ,nullptr              ,nullptr                ,nullptr                ,nullptr               ,nullptr               ,nullptr              ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr                ,nullptr                 }
  /* CT_BINARY      */  ,{  nullptr               ,nullptr                 ,nullptr                 ,nullptr                ,nullptr                ,nullptr                ,nullptr                 ,nullptr              ,nullptr                ,nullptr                ,nullptr               ,nullptr               ,nullptr              ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr                ,nullptr                 }
  /* CT_DATE        */  ,{  nullptr               ,nullptr                 ,nullptr                 ,nullptr                ,nullptr                ,nullptr                ,nullptr                 ,nullptr              ,nullptr                ,nullptr                ,nullptr               ,nullptr               ,nullptr              ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr                ,nullptr                 }
  /* CT_TIME        */  ,{  nullptr               ,nullptr                 ,nullptr                 ,nullptr                ,nullptr                ,nullptr                ,nullptr                 ,nullptr              ,nullptr                ,nullptr                ,nullptr               ,nullptr               ,nullptr              ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr                ,nullptr                 }
  /* CT_TIMESTAMP   */  ,{  nullptr               ,nullptr                 ,nullptr                 ,nullptr                ,nullptr                ,nullptr                ,nullptr                 ,nullptr              ,nullptr                ,nullptr                ,nullptr               ,nullptr               ,nullptr              ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr                ,nullptr                 }
  /* CT_INTERVAL_YM */  ,{  nullptr               ,&SQL_OperIntYMMulSShort ,&SQL_OperIntYMMulUShort ,&SQL_OperIntYMMulSLong ,&SQL_OperIntYMMulULong ,&SQL_OperIntYMMulFloat ,&SQL_OperIntYMMulDouble ,&SQL_OperIntYMMulBit ,&SQL_OperIntYMMulSTiny ,&SQL_OperIntYMMulUTiny ,&SQL_OperIntYMMulSBig ,&SQL_OperIntYMMulUBig ,&SQL_OperIntYMMulNum ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr                ,nullptr                 }
  /* CT_INTERVAL_DS */  ,{  nullptr               ,&SQL_OperIntDSMulSShort ,&SQL_OperIntDSMulUShort ,&SQL_OperIntDSMulSLong ,&SQL_OperIntDSMulULong ,&SQL_OperIntDSMulFloat ,&SQL_OperIntDSMulDouble ,&SQL_OperIntDSMulBit ,&SQL_OperIntDSMulSTiny ,&SQL_OperIntDSMulUTiny ,&SQL_OperIntDSMulSBig ,&SQL_OperIntDSMulUBig ,&SQL_OperIntDSMulNum ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr                ,nullptr                 }
};

// Multiplication operator for SQLVariant
SQLVariant
SQLVariant::operator*(const SQLVariant& p_right)
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

  // Find our comparison function
  OperatorCalculate function = OperatorMul[left][right].function;
  if(function)
  {
    return (*function)(*this,p_right);
  }
  // No compare function found
  // Data types are not comparable
  XString leftType  = SQLDataType::FindDatatype(m_datatype);
  XString rightType = SQLDataType::FindDatatype(p_right.m_datatype);
  XString error;
  error.Format(_T("Cannot do the multiplication operator on (%s * %s)"),leftType.GetString(),rightType.GetString());
  throw StdException(error);
}

SQLVariant& 
SQLVariant::operator*=(const SQLVariant& p_right)
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
    OperatorCalculate function = OperatorMul[left][right].function;
    if(function)
    {
      *this = (*function)(*this,p_right);
      return *this;
    }
  }
  else
  {
    ThrowErrorOperator(SVO_AssignMultiply);
  }
  // No compare function found
  // Data types are not comparable
  XString leftType  = SQLDataType::FindDatatype(m_datatype);
  XString rightType = SQLDataType::FindDatatype(p_right.m_datatype);
  XString error;
  error.Format(_T("Cannot do the *= operator on (%s + %s)"),leftType.GetString(),rightType.GetString());
  throw StdException(error);
}

// End of namespace
}
