////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantOperatorMul.cpp
//
// Copyright (c) 1998-2016 ir. W.E. Huisman
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
// Last Revision:   14-12-2016
// Version number:  1.3.0
//
#include "stdafx.h"
#include "SQLVariant.h"
#include "SQLVariantOperator.h"
#include "SQLDate.h"
#include "bcd.h"

#pragma warning (disable: 4239)

SQLVariant 
SQL_OperSShortMulChar(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result *= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUShortMulChar(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSLongMulChar(SQLVariant& p_left,SQLVariant& p_right)
{
  long result = p_left.GetAsSLong();
  result *= p_right.GetAsSLong();
  return SQLVariant(result);
}
SQLVariant 
SQL_OperULongMulChar(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned long result = p_left.GetAsULong();
  result *= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperFloatMulChar(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result *= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperDoubleMulChar(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result *= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperBitMulChar(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSTinyMulChar(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result *= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUTinyMulChar(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result *= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSBigMulChar(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result *= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUBigMulChar(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result *= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperNumMulChar(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() * bcd(p_right.GetAsChar());
  SQLVariant var(&num);
  return var;
}

// TYPE == SSHORT

SQLVariant 
SQL_OperCharMulSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result *= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSShortMulSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result *= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUShortMulSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant SQL_OperSLongMulSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  long result = p_left.GetAsSLong();
  result *= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperULongMulSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned long result = p_left.GetAsULong();
  result *= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperFloatMulSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result *= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperDoubleMulSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result *= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperBitMulSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSTinyMulSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result *= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUTinyMulSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result *= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSBigMulSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result *= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUBigMulSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result *= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperNumMulSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() * bcd(p_right.GetAsSShort());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
SQL_OperIntYMMulSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
SQL_OperIntDSMulSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}


// TYPE == USHORT

SQLVariant 
SQL_OperCharMulUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSShortMulUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUShortMulUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}


SQLVariant 
SQL_OperSLongMulUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  long result = p_left.GetAsSLong();
  result *= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUlongMulUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned long result = p_left.GetAsULong();
  result *= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperFloatMulUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result *= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperDoubleMulUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result *= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperBitMulUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSTinyMulUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result *= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUTinyMulUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result *= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSBigMulUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result *= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUBigMulUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result *= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperNumMulUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() * bcd((long)p_right.GetAsUShort());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
SQL_OperIntYMMulUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
SQL_OperIntDSMulUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == SLONG

SQLVariant 
SQL_OperCharMulSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  long result = p_left.GetAsSLong();
  result *= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSShortMulSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result *= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUShortMulSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSLongMulSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  long result = p_left.GetAsSLong();
  result *= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperULongMulSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned long result = p_left.GetAsULong();
  result *= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperFloatMulSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result *= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperDoubleMulSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result *= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperBitMulSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSTinyMulSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result *= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUTinyMulSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result *= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSBigMulSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result *= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUBigMulSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result *= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperNumMulSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() * bcd(p_right.GetAsSLong());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
SQL_OperIntYMMulSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
SQL_OperIntDSMulSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == ULONG

SQLVariant 
SQL_OperCharMulULong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned long result = p_left.GetAsULong();
  result *= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSShortMulULong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUShortMulULong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSLongMulULong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperULongMulULong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned long result = p_left.GetAsULong();
  result *= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperFloatMulULong(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result *= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperDoubleMulULong(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result *= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperBitMulULong(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSTinyMulULong(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result *= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUTinyMulULong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result *= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSBigMulULong(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result *= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUBigMulULong(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result *= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperNumMulULong(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() * bcd((int64)p_right.GetAsULong());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
SQL_OperIntYMMulULong(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
SQL_OperIntDSMulULong(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == FLOAT

SQLVariant 
SQL_OperCharMulFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result *= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSShortMulFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result *= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUShortMulFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSLongMulFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  long result = p_left.GetAsSLong();
  result *= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperULongMulFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned long result = p_left.GetAsULong();
  result *= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperFloatMulFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result *= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperDoubleMulFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result *= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperBitMulFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSTinyMulFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result *= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUTinyMulFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result *= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSBigMulFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result *= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUBigMulFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result *= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperNumMulFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() * bcd((double)p_right.GetAsFloat());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
SQL_OperIntYMMulFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsDouble();
  return SQLVariant(&result);
}

SQLVariant 
SQL_OperIntDSMulFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsDouble();
  return SQLVariant(&result);
}

// TYPE == DOUBLE

SQLVariant 
SQL_OperCharMulDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result *= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSShortMulDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsUShort();
  result *= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUShortMulDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSLongMulDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  long result = p_left.GetAsSLong();
  result *= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperULongMulDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned long result = p_left.GetAsULong();
  result *= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperFloatMulDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result *= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperDoubleMulDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result *= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperBitMulDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSTinyMulDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result *= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUTinyMulDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result *= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSBigMulDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result *= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUBigMulDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result *= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperNumMulDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() * bcd(p_right.GetAsDouble());
  SQLVariant var(&num);
  return var;
}

SQLVariant
SQL_OperIntYMMulDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsDouble();
  return SQLVariant(&result);
}

SQLVariant
SQL_OperIntDSMulDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsDouble();
  return SQLVariant(&result);
}


// TYPE == BIT

SQLVariant 
SQL_OperCharMulBit(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSShortMulBit(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  if(p_right.GetAsBit() == 0) 
  {
    result = 0;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUShortMulBit(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  if(p_right.GetAsBit() == 0)
  {
    result = 0;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSLongMulBit(SQLVariant& p_left,SQLVariant& p_right)
{
  long result = p_left.GetAsSLong();
  if(p_right.GetAsBit() == 0)
  {
    result = 0;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperULongMulBit(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned long result = p_left.GetAsULong();
  if(p_right.GetAsBit() == 0)
  {
    result = 0;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperFloatMulBit(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  if(p_right.GetAsBit() == 0)
  {
    result = 0.0;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperDoubleMulBit(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  if(p_right.GetAsBit() == 0)
  {
    result = 0.0;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperBitMulBit(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSTinyMulBit(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  if(p_right.GetAsBit() == 0)
  {
    result = 0;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUTinyMulBit(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  if(p_right.GetAsBit() == 0)
  {
    result = 0;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSBigMulBit(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  if(p_right.GetAsBit() == 0)
  {
    result = 0;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUBigMulBit(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  if(p_right.GetAsBit() == 0)
  {
    result = 0;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperNumMulBit(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() * bcd(p_right.GetAsBit());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
SQL_OperIntYMMulBit(SQLVariant& p_left,SQLVariant& p_right)
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
SQL_OperIntDSMulBit(SQLVariant& p_left,SQLVariant& p_right)
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
SQL_OperCharMulSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result *= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSShortMulSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result *= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUShortMulSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSLongMulSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  long result = p_left.GetAsSLong();
  result *= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperULongMulSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned long result = p_left.GetAsULong();
  result *= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperFloatMulSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result *= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperDoubleMulSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result *= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperBitMulSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSTinyMulSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result *= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUTinyMulSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result *= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSBigMulSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result *= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUBigMulSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result *= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperNumMulSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() * bcd(p_right.GetAsSTinyInt());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
SQL_OperIntYMMulSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
SQL_OperIntDSMulSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE = UTINYINT

SQLVariant 
SQL_OperCharMulUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result *= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSShortMulUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result *= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUShortMulUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSLongMulUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  long result = p_left.GetAsSLong();
  result *= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperULongMulUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned long result = p_left.GetAsULong();
  result *= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperFloatMulUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result *= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperDoubleMulUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result *= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperBitMulUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSTinyMulUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result *= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUTinyMulUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result *= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSBigMulUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result *= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUBigMulUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result *= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperNumMulUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() * bcd((short)p_right.GetAsUTinyInt());
  SQLVariant var(&num);
  return var;
}

SQLVariant
SQL_OperIntYMMulUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant
SQL_OperIntDSMulUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}


// TYPE == SBIGINT

SQLVariant 
SQL_OperCharMulSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result *= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSShortMulSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result *= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUShortMulSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSLongMulSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  long result = p_left.GetAsSLong();
  result *= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperULongMulSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned long result = p_left.GetAsULong();
  result *= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperFloatMulSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result *= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperDoubleMulSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result *= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperBitMulSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSTinyMulSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result *= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUTinyMulSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result *= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSBigMulSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result *= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUBigMulSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result *= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperNumMulSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() * bcd(p_right.GetAsSBigInt());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
SQL_OperIntYMMulSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
SQL_OperIntDSMulSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == UBIGINT

SQLVariant 
SQL_OperCharMulUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result *= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSShortMulUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result *= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUShortMulUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result *= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSLongMulUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  long result = p_left.GetAsSLong();
  result *= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperULongMulUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned long result = p_left.GetAsULong();
  result *= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperFloatMulUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result *= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperDoubleMulUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result *= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperBitMulUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSTinyMulUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result *= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUTinyMulUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result *= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSBigMulUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result *= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUBigMulUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result *= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperNumMulUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() * bcd(p_right.GetAsUBigInt());
  SQLVariant var(&num);
  return var;
}

SQLVariant
SQL_OperIntYMMulUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant
SQL_OperIntDSMulUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == NUMERIC

SQLVariant 
SQL_OperCharMulNum(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = bcd(p_left.GetAsChar()) * p_right.GetAsBCD();
  SQLVariant var(&num);
  return var;
}

SQLVariant 
SQL_OperSShortMulNum(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = bcd(p_left.GetAsSShort()) * p_right.GetAsBCD();
  SQLVariant var(&num);
  return var;
}

SQLVariant 
SQL_OperUShortMulNum(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = bcd((long)p_left.GetAsUShort()) * p_right.GetAsBCD();
  SQLVariant var(&num);
  return var;
}

SQLVariant 
SQL_OperSLongMulNum(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = bcd(p_left.GetAsSLong()) * p_right.GetAsBCD();
  SQLVariant var(&num);
  return var;
}

SQLVariant 
SQL_OperULongMulNum(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = bcd((int64)p_left.GetAsULong()) * p_right.GetAsBCD();
  SQLVariant var(&num);
  return var;
}

SQLVariant 
SQL_OperFloatMulNum(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = bcd((double)p_left.GetAsFloat()) * p_right.GetAsBCD();
  SQLVariant var(&num);
  return var;
}

SQLVariant 
SQL_OperDoubleMulNum(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = bcd(p_left.GetAsDouble()) * p_right.GetAsBCD();
  SQLVariant var(&num);
  return var;
}

SQLVariant 
SQL_OperBitMulNum(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = bcd(p_left.GetAsBit()) * p_right.GetAsBCD();
  SQLVariant var(&num);
  return var;
}

SQLVariant 
SQL_OperSTinyMulNum(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = bcd(p_left.GetAsSTinyInt()) * p_right.GetAsBCD();
  SQLVariant var(&num);
  return var;
}

SQLVariant 
SQL_OperUTinyMulNum(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = bcd((short)p_left.GetAsUTinyInt()) * p_right.GetAsBCD();
  SQLVariant var(&num);
  return var;
}

SQLVariant 
SQL_OperSBigMulNum(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = bcd(p_left.GetAsSBigInt()) * p_right.GetAsBCD();
  SQLVariant var(&num);
  return var;
}

SQLVariant 
SQL_OperUBigMulNum(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = bcd(p_left.GetAsUBigInt()) * p_right.GetAsBCD();
  SQLVariant var(&num);
  return var;
}

SQLVariant 
SQL_OperNumMulNum(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() * p_right.GetAsBCD();
  SQLVariant var(&num);
  return var;
}

SQLVariant 
SQL_OperIntYMMulNum(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsDouble();
  return SQLVariant(&result);
}

SQLVariant 
SQL_OperIntDSMulNum(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() * p_right.GetAsDouble();
  return SQLVariant(&result);
}

// OPERATOR ARRAY

static CalculateFunctionArray OperatorMul[CT_LAST][CT_LAST] =
{
                         // CT_CHAR                CT_SSHORT                CT_USHORT                CT_SLONG                CT_ULONG                CT_FLOAT                CT_DOUBLE                CT_BIT                CT_STINYINT             CT_UTINYINT             CT_SBIGINT             CT_UBIGINT             CT_NUMERIC            CT_GUID  CT_BINARY CT_DATE  CT_TIME  CT_TIMESTAMP CT_INTERVAL_YM CT_INTERVAL_DS
                         // ---------------------- ------------------------ ------------------------ ----------------------- ----------------------- ----------------------- ------------------------ --------------------- ----------------------- ----------------------- ---------------------- ---------------------- --------------------- -------- --------- -------- -------- ------------ -------------- --------------
  /* CT_CHAR        */   {  nullptr               ,&SQL_OperCharMulSShort  ,&SQL_OperCharMulUShort  ,&SQL_OperCharMulSLong  ,&SQL_OperCharMulULong  ,&SQL_OperCharMulFloat  ,&SQL_OperCharMulDouble  ,&SQL_OperCharMulBit  ,&SQL_OperCharMulSTiny  ,&SQL_OperCharMulUTiny  ,&SQL_OperCharMulSBig  ,&SQL_OperCharMulUBig  ,&SQL_OperCharMulNum  ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_SSHORT      */  ,{  &SQL_OperSShortMulChar,&SQL_OperSShortMulSShort,&SQL_OperSShortMulUShort,&SQL_OperSShortMulSLong,&SQL_OperSShortMulULong,&SQL_OperSShortMulFloat,&SQL_OperSShortMulDouble,&SQL_OperSShortMulBit,&SQL_OperSShortMulSTiny,&SQL_OperSShortMulUTiny,&SQL_OperSShortMulSBig,&SQL_OperSShortMulUBig,&SQL_OperSShortMulNum,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_USHORT      */  ,{  &SQL_OperUShortMulChar,&SQL_OperUShortMulSShort,&SQL_OperUShortMulUShort,&SQL_OperUShortMulSLong,&SQL_OperUShortMulULong,&SQL_OperUShortMulFloat,&SQL_OperUShortMulDouble,&SQL_OperUShortMulBit,&SQL_OperUShortMulSTiny,&SQL_OperUShortMulUTiny,&SQL_OperUShortMulSBig,&SQL_OperUShortMulUBig,&SQL_OperUShortMulNum,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_SLONG       */  ,{  &SQL_OperSLongMulChar ,&SQL_OperSLongMulSShort ,&SQL_OperSLongMulUShort ,&SQL_OperSLongMulSLong ,&SQL_OperSLongMulULong ,&SQL_OperSLongMulFloat ,&SQL_OperSLongMulDouble ,&SQL_OperSLongMulBit ,&SQL_OperSLongMulSTiny ,&SQL_OperSLongMulUTiny ,&SQL_OperSLongMulSBig ,&SQL_OperSLongMulUBig ,&SQL_OperSLongMulNum ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_ULONG       */  ,{  &SQL_OperULongMulChar ,&SQL_OperULongMulSShort ,&SQL_OperUlongMulUShort ,&SQL_OperULongMulSLong ,&SQL_OperULongMulULong ,&SQL_OperULongMulFloat ,&SQL_OperULongMulDouble ,&SQL_OperULongMulBit ,&SQL_OperULongMulSTiny ,&SQL_OperULongMulUTiny ,&SQL_OperULongMulSBig ,&SQL_OperULongMulUBig ,&SQL_OperULongMulNum ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_FLOAT       */  ,{  &SQL_OperFloatMulChar ,&SQL_OperFloatMulSShort ,&SQL_OperFloatMulUShort ,&SQL_OperFloatMulSLong ,&SQL_OperFloatMulULong ,&SQL_OperFloatMulFloat ,&SQL_OperFloatMulDouble ,&SQL_OperFloatMulBit ,&SQL_OperFloatMulSTiny ,&SQL_OperFloatMulUTiny ,&SQL_OperFloatMulSBig ,&SQL_OperFloatMulUBig ,&SQL_OperFloatMulNum ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_DOUBLE      */  ,{  &SQL_OperDoubleMulChar,&SQL_OperDoubleMulSShort,&SQL_OperDoubleMulUShort,&SQL_OperDoubleMulSLong,&SQL_OperDoubleMulULong,&SQL_OperDoubleMulFloat,&SQL_OperDoubleMulDouble,&SQL_OperDoubleMulBit,&SQL_OperDoubleMulSTiny,&SQL_OperDoubleMulUTiny,&SQL_OperDoubleMulSBig,&SQL_OperDoubleMulUBig,&SQL_OperDoubleMulNum,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_BIT         */  ,{  &SQL_OperBitMulChar   ,&SQL_OperBitMulSShort   ,&SQL_OperBitMulUShort   ,&SQL_OperBitMulSLong   ,&SQL_OperBitMulULong   ,&SQL_OperBitMulFloat   ,&SQL_OperBitMulDouble   ,&SQL_OperBitMulBit   ,&SQL_OperBitMulSTiny   ,&SQL_OperBitMulUTiny   ,&SQL_OperBitMulSBig   ,&SQL_OperBitMulUBig   ,&SQL_OperBitMulNum   ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_STINYINT    */  ,{  &SQL_OperSTinyMulChar ,&SQL_OperSTinyMulSShort ,&SQL_OperSTinyMulUShort ,&SQL_OperSTinyMulSLong ,&SQL_OperSTinyMulULong ,&SQL_OperSTinyMulFloat ,&SQL_OperSTinyMulDouble ,&SQL_OperSTinyMulBit ,&SQL_OperSTinyMulSTiny ,&SQL_OperSTinyMulUTiny ,&SQL_OperSTinyMulSBig ,&SQL_OperSTinyMulUBig ,&SQL_OperSTinyMulNum ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_UTINYINT    */  ,{  &SQL_OperUTinyMulChar ,&SQL_OperUTinyMulSShort ,&SQL_OperUTinyMulUShort ,&SQL_OperUTinyMulSLong ,&SQL_OperUTinyMulULong ,&SQL_OperUTinyMulFloat ,&SQL_OperUTinyMulDouble ,&SQL_OperUTinyMulBit ,&SQL_OperUTinyMulSTiny ,&SQL_OperUTinyMulUTiny ,&SQL_OperUTinyMulSBig ,&SQL_OperUTinyMulUBig ,&SQL_OperUTinyMulNum ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_SBIGINT     */  ,{  &SQL_OperSBigMulChar  ,&SQL_OperSBigMulSShort  ,&SQL_OperSBigMulUShort  ,&SQL_OperSBigMulSLong  ,&SQL_OperSBigMulULong  ,&SQL_OperSBigMulFloat  ,&SQL_OperSBigMulDouble  ,&SQL_OperSBigMulBit  ,&SQL_OperSBigMulSTiny  ,&SQL_OperSBigMulUTiny  ,&SQL_OperSBigMulSBig  ,&SQL_OperSBigMulUBig  ,&SQL_OperSBigMulNum  ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_UBIGINT     */  ,{  &SQL_OperUBigMulChar  ,&SQL_OperUBigMulSShort  ,&SQL_OperUBigMulUShort  ,&SQL_OperUBigMulSLong  ,&SQL_OperUBigMulULong  ,&SQL_OperUBigMulFloat  ,&SQL_OperUBigMulDouble  ,&SQL_OperUBigMulBit  ,&SQL_OperUBigMulSTiny  ,&SQL_OperUBigMulUTiny  ,&SQL_OperUBigMulSBig  ,&SQL_OperUBigMulUBig  ,&SQL_OperUBigMulNum  ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_NUMERIC     */  ,{  &SQL_OperNumMulChar   ,&SQL_OperNumMulSShort   ,&SQL_OperNumMulUShort   ,&SQL_OperNumMulSLong   ,&SQL_OperNumMulULong   ,&SQL_OperNumMulFloat   ,&SQL_OperNumMulDouble   ,&SQL_OperNumMulBit   ,&SQL_OperNumMulSTiny   ,&SQL_OperNumMulUTiny   ,&SQL_OperNumMulSBig   ,&SQL_OperNumMulUBig   ,&SQL_OperNumMulNum   ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_GUID        */  ,{  nullptr               ,nullptr                 ,nullptr                 ,nullptr                ,nullptr                ,nullptr                ,nullptr                 ,nullptr              ,nullptr                ,nullptr                ,nullptr               ,nullptr               ,nullptr              ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_BINARY      */  ,{  nullptr               ,nullptr                 ,nullptr                 ,nullptr                ,nullptr                ,nullptr                ,nullptr                 ,nullptr              ,nullptr                ,nullptr                ,nullptr               ,nullptr               ,nullptr              ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_DATE        */  ,{  nullptr               ,nullptr                 ,nullptr                 ,nullptr                ,nullptr                ,nullptr                ,nullptr                 ,nullptr              ,nullptr                ,nullptr                ,nullptr               ,nullptr               ,nullptr              ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_TIME        */  ,{  nullptr               ,nullptr                 ,nullptr                 ,nullptr                ,nullptr                ,nullptr                ,nullptr                 ,nullptr              ,nullptr                ,nullptr                ,nullptr               ,nullptr               ,nullptr              ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_TIMESTAMP   */  ,{  nullptr               ,nullptr                 ,nullptr                 ,nullptr                ,nullptr                ,nullptr                ,nullptr                 ,nullptr              ,nullptr                ,nullptr                ,nullptr               ,nullptr               ,nullptr              ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_INTERVAL_YM */  ,{  nullptr               ,&SQL_OperIntYMMulSShort ,&SQL_OperIntYMMulUShort ,&SQL_OperIntYMMulSLong ,&SQL_OperIntYMMulULong ,&SQL_OperIntYMMulFloat ,&SQL_OperIntYMMulDouble ,&SQL_OperIntYMMulBit ,&SQL_OperIntYMMulSTiny ,&SQL_OperIntYMMulUTiny ,&SQL_OperIntYMMulSBig ,&SQL_OperIntYMMulUBig ,&SQL_OperIntYMMulNum ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_INTERVAL_DS */  ,{  nullptr               ,&SQL_OperIntDSMulSShort ,&SQL_OperIntDSMulUShort ,&SQL_OperIntDSMulSLong ,&SQL_OperIntDSMulULong ,&SQL_OperIntDSMulFloat ,&SQL_OperIntDSMulDouble ,&SQL_OperIntDSMulBit ,&SQL_OperIntDSMulSTiny ,&SQL_OperIntDSMulUTiny ,&SQL_OperIntDSMulSBig ,&SQL_OperIntDSMulUBig ,&SQL_OperIntDSMulNum ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
};

// Multiplication operator for SQLVariant
SQLVariant
SQLVariant::operator*(SQLVariant& p_right)
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
  CString leftType  = FindDatatype(m_datatype);
  CString rightType = FindDatatype(p_right.m_datatype);
  CString error;
  error.Format("Cannot do the multiplication operator on (%s * %s)",leftType,rightType);
  throw error;
}

