////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantOperatorAdd.cpp
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

SQLVariant
static SQL_OperCharAddChar(SQLVariant& p_left,SQLVariant& p_right)
{
  XString left;
  XString right;

  p_left .GetAsString(left);
  p_right.GetAsString(right);

  XString result = left + right;
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortAddChar(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result += p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortAddChar(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result += p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongAddChar(SQLVariant& p_left,SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result += p_right.GetAsSLong();
  return SQLVariant(result);
}
SQLVariant 
static SQL_OperULongAddChar(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result += p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatAddChar(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result += p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleAddChar(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result += p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitAddChar(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() || p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyAddChar(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result += p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyAddChar(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result += p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigAddChar(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result += p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigAddChar(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result += p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumAddChar(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() + bcd((const char*)p_right.GetAsChar());
  SQLVariant var(&num);
  return var;
}

// TYPE == SSHORT

SQLVariant 
static SQL_OperCharAddSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result += p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortAddSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result += p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortAddSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result += p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant static SQL_OperSLongAddSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result += p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongAddSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result += p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatAddSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result += p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleAddSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result += p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitAddSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() || p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyAddSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result += p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyAddSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result += p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigAddSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result += p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigAddSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result += p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumAddSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() + bcd(p_right.GetAsSShort());
  SQLVariant var(&num);
  return var;
}

// TYPE == USHORT

SQLVariant 
static SQL_OperCharAddUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result += p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortAddUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result += p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortAddUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result += p_right.GetAsUShort();
  return SQLVariant(result);
}


SQLVariant 
static SQL_OperSLongAddUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result += p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUlongAddUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result += p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatAddUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result += p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleAddUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result += p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitAddUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() || p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyAddUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result += p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyAddUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result += p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigAddUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result += p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigAddUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result += p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumAddUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() + bcd((long)p_right.GetAsUShort());
  SQLVariant var(&num);
  return var;
}

// TYPE == SLONG

SQLVariant 
static SQL_OperCharAddSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result += p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortAddSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result += p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortAddSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result += p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongAddSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result += p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongAddSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result += p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatAddSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result += p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleAddSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result += p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitAddSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() || p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyAddSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result += p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyAddSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result += p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigAddSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result += p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigAddSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result += p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumAddSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() + bcd(p_right.GetAsSLong());
  SQLVariant var(&num);
  return var;
}

// TYPE == ULONG

SQLVariant 
static SQL_OperCharAddULong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result += p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortAddULong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result += p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortAddULong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result += p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongAddULong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result += p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongAddULong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result += p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatAddULong(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result += p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleAddULong(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result += p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitAddULong(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() || p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyAddULong(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result += p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyAddULong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result += p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigAddULong(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result += p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigAddULong(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result += p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumAddULong(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() + bcd((int64)p_right.GetAsULong());
  SQLVariant var(&num);
  return var;
}

// TYPE == FLOAT

SQLVariant 
static SQL_OperCharAddFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result += p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortAddFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result += p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortAddFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result += p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongAddFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result += p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongAddFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result += p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatAddFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result += p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleAddFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result += p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitAddFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() || p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyAddFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result += p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyAddFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result += p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigAddFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result += p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigAddFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result += p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumAddFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() + bcd((double)p_right.GetAsFloat());
  SQLVariant var(&num);
  return var;
}

// TYPE == DOUBLE

SQLVariant 
static SQL_OperCharAddDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result += p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortAddDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsUShort();
  result += p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortAddDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result += p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongAddDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result += p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongAddDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result += p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatAddDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result += p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleAddDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result += p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitAddDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() || p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyAddDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result += p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyAddDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result += p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigAddDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result += p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigAddDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result += p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumAddDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() + bcd(p_right.GetAsDouble());
  SQLVariant var(&num);
  return var;
}

// TYPE == BIT

SQLVariant 
static SQL_OperCharAddBit(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() || p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortAddBit(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  if(p_right.GetAsBit()) 
  {
    ++result;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortAddBit(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  if(p_right.GetAsBit())
  {
    ++result;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongAddBit(SQLVariant& p_left,SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  if(p_right.GetAsBit())
  {
    ++result;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongAddBit(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  if(p_right.GetAsBit())
  {
    ++result;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatAddBit(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  if(p_right.GetAsBit())
  {
    result += 1.0;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleAddBit(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  if(p_right.GetAsBit())
  {
    result += 1.0;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitAddBit(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() || p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyAddBit(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  if(p_right.GetAsBit())
  {
    ++result;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyAddBit(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  if(p_right.GetAsBit())
  {
    ++result;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigAddBit(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  if(p_right.GetAsBit())
  {
    ++result;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigAddBit(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  if(p_right.GetAsBit())
  {
    ++result;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumAddBit(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() + bcd(p_right.GetAsBit());
  SQLVariant var(&num);
  return var;
}

// TYPE == STINYINT

SQLVariant 
static SQL_OperCharAddSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result += p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortAddSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result += p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortAddSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result += p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongAddSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result += p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongAddSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result += p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatAddSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result += p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleAddSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result += p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitAddSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() || p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyAddSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result += p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyAddSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result += p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigAddSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result += p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigAddSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result += p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumAddSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() + bcd(p_right.GetAsSTinyInt());
  SQLVariant var(&num);
  return var;
}

// TYPE = UTINYINT

SQLVariant 
static SQL_OperCharAddUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result += p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortAddUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result += p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortAddUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result += p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongAddUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result += p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongAddUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result += p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatAddUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result += p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleAddUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result += p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitAddUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() || p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyAddUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result += p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyAddUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result += p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigAddUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result += p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigAddUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result += p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumAddUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() + bcd((short)p_right.GetAsUTinyInt());
  SQLVariant var(&num);
  return var;
}

// TYPE == SBIGINT

SQLVariant 
static SQL_OperCharAddSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result += p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortAddSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result += p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortAddSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result += p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongAddSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result += p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongAddSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result += p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatAddSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result += p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleAddSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result += p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitAddSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() || p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyAddSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result += p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyAddSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result += p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigAddSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result += p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigAddSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result += p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumAddSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() + bcd(p_right.GetAsSBigInt());
  SQLVariant var(&num);
  return var;
}

// TYPE == UBIGINT

SQLVariant 
static SQL_OperCharAddUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result += p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortAddUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result += p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortAddUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result += p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongAddUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result += p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongAddUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result += p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatAddUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result += p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleAddUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result += p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitAddUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() || p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyAddUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result += p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyAddUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result += p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigAddUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result += p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigAddUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result += p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumAddUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() + bcd(p_right.GetAsUBigInt());
  SQLVariant var(&num);
  return var;
}

// TYPE == NUMERIC

SQLVariant 
static SQL_OperCharAddNum(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = bcd(p_left.GetAsChar()) + p_right.GetAsBCD();
  SQLVariant var(num.AsString(bcd::Format::Bookkeeping,false,0));
  return var;
}

SQLVariant 
static SQL_OperSShortAddNum(SQLVariant& p_left,SQLVariant& p_right)
{
  short num = p_left.GetAsUShort() + (short)p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperUShortAddNum(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short num = p_left.GetAsUShort() + (unsigned short)p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperSLongAddNum(SQLVariant& p_left,SQLVariant& p_right)
{
  int num = p_left.GetAsSLong() + p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperULongAddNum(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned int num = p_left.GetAsULong() + (unsigned long) p_right.GetAsBCD().AsInt64();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperFloatAddNum(SQLVariant& p_left,SQLVariant& p_right)
{
  float num = p_left.GetAsFloat() + (float)p_right.GetAsBCD().AsDouble();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperDoubleAddNum(SQLVariant& p_left,SQLVariant& p_right)
{
  double num = p_left.GetAsDouble() + p_right.GetAsBCD().AsDouble();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperBitAddNum(SQLVariant& p_left,SQLVariant& p_right)
{
  bool num = false;
  if(p_left.GetAsBit() || p_right.GetAsBCD().IsZero() == false)
  {
    num = true;
  }
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperSTinyAddNum(SQLVariant& p_left,SQLVariant& p_right)
{
  char num = p_left.GetAsSTinyInt() + (char) p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperUTinyAddNum(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char num = p_left.GetAsUTinyInt() + (unsigned char) p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperSBigAddNum(SQLVariant& p_left,SQLVariant& p_right)
{
  int64 num = p_left.GetAsSBigInt() + p_right.GetAsBCD().AsInt64();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperUBigAddNum(SQLVariant& p_left,SQLVariant& p_right)
{
  uint64 num = p_left.GetAsUBigInt() + p_right.GetAsBCD().AsUInt64();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperNumAddNum(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() + p_right.GetAsBCD();
  SQLVariant var(&num);
  return var;
}

// TYPE == DATE

SQLVariant 
static SQL_OperTimeAddDate(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLTimestamp result(p_right.GetAsSQLDate(),p_left.GetAsSQLTime());
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperIntDSAddDate(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLDate result = p_right.GetAsSQLDate();
  result = result + p_left.GetAsSQLInterval();
  return SQLVariant(&result);
}

// TYPE == TIME

SQLVariant 
static SQL_OperDateAddTime(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLTimestamp result(p_left.GetAsSQLDate(),p_right.GetAsSQLTime());
  return SQLVariant(&result);
}
SQLVariant 
static SQL_OperIntDSAddTime(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLTime result = p_right.GetAsSQLTime();
  result = result + p_left.GetAsSQLInterval();
  return SQLVariant(&result);
}

// TYPE == TIMESTAMP

SQLVariant 
static SQL_OperIntDSAddStamp(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLTimestamp result = p_right.GetAsSQLTimestamp();
  result = result + p_left.GetAsSQLInterval();
  return SQLVariant(&result);
}

// TYPE == INTERVAL_YEAR_MONTH

SQLVariant 
static SQL_OperIntYMAddIntYM(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval();
  result = result + p_right.GetAsSQLInterval();
  return SQLVariant(&result);
}

// TYPE == INTERVAL_DAY_SECOND

SQLVariant 
static SQL_OperDateAddIntDS(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLDate result = p_left.GetAsSQLDate();
  result = result + p_right.GetAsSQLInterval();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperTimeAddIntDS(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLTime result = p_left.GetAsSQLTime();
  result = result + p_right.GetAsSQLInterval();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperStampAddIntDS(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLTimestamp result = p_left.GetAsSQLTimestamp();
  result = result + p_right.GetAsSQLInterval();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperIntDSAddIntDS(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() + p_right.GetAsSQLInterval();
  return SQLVariant(&result);
}

// OPERATOR ARRAY

static CalculateFunctionArray OperatorAdd[CT_LAST][CT_LAST] =
{
                         // CT_CHAR                CT_SSHORT                CT_USHORT                CT_SLONG                CT_ULONG                CT_FLOAT                CT_DOUBLE                CT_BIT                CT_STINYINT             CT_UTINYINT             CT_SBIGINT             CT_UBIGINT             CT_NUMERIC            CT_GUID  CT_BINARY CT_DATE               CT_TIME               CT_TIMESTAMP           CT_INTERVAL_YM         CT_INTERVAL_DS
                         // ---------------------- ------------------------ ------------------------ ----------------------- ----------------------- ----------------------- ------------------------ --------------------- ----------------------- ----------------------- ---------------------- ---------------------- --------------------- -------- --------- --------------------- --------------------- ---------------------- ---------------------- ------------------------
  /* CT_CHAR        */   {  &SQL_OperCharAddChar  ,&SQL_OperCharAddSShort  ,&SQL_OperCharAddUShort  ,&SQL_OperCharAddSLong  ,&SQL_OperCharAddULong  ,&SQL_OperCharAddFloat  ,&SQL_OperCharAddDouble  ,&SQL_OperCharAddBit  ,&SQL_OperCharAddSTiny  ,&SQL_OperCharAddUTiny  ,&SQL_OperCharAddSBig  ,&SQL_OperCharAddUBig  ,&SQL_OperCharAddNum  ,nullptr ,nullptr  ,nullptr              ,nullptr              ,nullptr               ,nullptr               ,nullptr                }
  /* CT_SSHORT      */  ,{  &SQL_OperSShortAddChar,&SQL_OperSShortAddSShort,&SQL_OperSShortAddUShort,&SQL_OperSShortAddSLong,&SQL_OperSShortAddULong,&SQL_OperSShortAddFloat,&SQL_OperSShortAddDouble,&SQL_OperSShortAddBit,&SQL_OperSShortAddSTiny,&SQL_OperSShortAddUTiny,&SQL_OperSShortAddSBig,&SQL_OperSShortAddUBig,&SQL_OperSShortAddNum,nullptr ,nullptr  ,nullptr              ,nullptr              ,nullptr               ,nullptr               ,nullptr                }
  /* CT_USHORT      */  ,{  &SQL_OperUShortAddChar,&SQL_OperUShortAddSShort,&SQL_OperUShortAddUShort,&SQL_OperUShortAddSLong,&SQL_OperUShortAddULong,&SQL_OperUShortAddFloat,&SQL_OperUShortAddDouble,&SQL_OperUShortAddBit,&SQL_OperUShortAddSTiny,&SQL_OperUShortAddUTiny,&SQL_OperUShortAddSBig,&SQL_OperUShortAddUBig,&SQL_OperUShortAddNum,nullptr ,nullptr  ,nullptr              ,nullptr              ,nullptr               ,nullptr               ,nullptr                }
  /* CT_SLONG       */  ,{  &SQL_OperSLongAddChar ,&SQL_OperSLongAddSShort ,&SQL_OperSLongAddUShort ,&SQL_OperSLongAddSLong ,&SQL_OperSLongAddULong ,&SQL_OperSLongAddFloat ,&SQL_OperSLongAddDouble ,&SQL_OperSLongAddBit ,&SQL_OperSLongAddSTiny ,&SQL_OperSLongAddUTiny ,&SQL_OperSLongAddSBig ,&SQL_OperSLongAddUBig ,&SQL_OperSLongAddNum ,nullptr ,nullptr  ,nullptr              ,nullptr              ,nullptr               ,nullptr               ,nullptr                }
  /* CT_ULONG       */  ,{  &SQL_OperULongAddChar ,&SQL_OperULongAddSShort ,&SQL_OperUlongAddUShort ,&SQL_OperULongAddSLong ,&SQL_OperULongAddULong ,&SQL_OperULongAddFloat ,&SQL_OperULongAddDouble ,&SQL_OperULongAddBit ,&SQL_OperULongAddSTiny ,&SQL_OperULongAddUTiny ,&SQL_OperULongAddSBig ,&SQL_OperULongAddUBig ,&SQL_OperULongAddNum ,nullptr ,nullptr  ,nullptr              ,nullptr              ,nullptr               ,nullptr               ,nullptr                }
  /* CT_FLOAT       */  ,{  &SQL_OperFloatAddChar ,&SQL_OperFloatAddSShort ,&SQL_OperFloatAddUShort ,&SQL_OperFloatAddSLong ,&SQL_OperFloatAddULong ,&SQL_OperFloatAddFloat ,&SQL_OperFloatAddDouble ,&SQL_OperFloatAddBit ,&SQL_OperFloatAddSTiny ,&SQL_OperFloatAddUTiny ,&SQL_OperFloatAddSBig ,&SQL_OperFloatAddUBig ,&SQL_OperFloatAddNum ,nullptr ,nullptr  ,nullptr              ,nullptr              ,nullptr               ,nullptr               ,nullptr                }
  /* CT_DOUBLE      */  ,{  &SQL_OperDoubleAddChar,&SQL_OperDoubleAddSShort,&SQL_OperDoubleAddUShort,&SQL_OperDoubleAddSLong,&SQL_OperDoubleAddULong,&SQL_OperDoubleAddFloat,&SQL_OperDoubleAddDouble,&SQL_OperDoubleAddBit,&SQL_OperDoubleAddSTiny,&SQL_OperDoubleAddUTiny,&SQL_OperDoubleAddSBig,&SQL_OperDoubleAddUBig,&SQL_OperDoubleAddNum,nullptr ,nullptr  ,nullptr              ,nullptr              ,nullptr               ,nullptr               ,nullptr                }
  /* CT_BIT         */  ,{  &SQL_OperBitAddChar   ,&SQL_OperBitAddSShort   ,&SQL_OperBitAddUShort   ,&SQL_OperBitAddSLong   ,&SQL_OperBitAddULong   ,&SQL_OperBitAddFloat   ,&SQL_OperBitAddDouble   ,&SQL_OperBitAddBit   ,&SQL_OperBitAddSTiny   ,&SQL_OperBitAddUTiny   ,&SQL_OperBitAddSBig   ,&SQL_OperBitAddUBig   ,&SQL_OperBitAddNum   ,nullptr ,nullptr  ,nullptr              ,nullptr              ,nullptr               ,nullptr               ,nullptr                }
  /* CT_STINYINT    */  ,{  &SQL_OperSTinyAddChar ,&SQL_OperSTinyAddSShort ,&SQL_OperSTinyAddUShort ,&SQL_OperSTinyAddSLong ,&SQL_OperSTinyAddULong ,&SQL_OperSTinyAddFloat ,&SQL_OperSTinyAddDouble ,&SQL_OperSTinyAddBit ,&SQL_OperSTinyAddSTiny ,&SQL_OperSTinyAddUTiny ,&SQL_OperSTinyAddSBig ,&SQL_OperSTinyAddUBig ,&SQL_OperSTinyAddNum ,nullptr ,nullptr  ,nullptr              ,nullptr              ,nullptr               ,nullptr               ,nullptr                }
  /* CT_UTINYINT    */  ,{  &SQL_OperUTinyAddChar ,&SQL_OperUTinyAddSShort ,&SQL_OperUTinyAddUShort ,&SQL_OperUTinyAddSLong ,&SQL_OperUTinyAddULong ,&SQL_OperUTinyAddFloat ,&SQL_OperUTinyAddDouble ,&SQL_OperUTinyAddBit ,&SQL_OperUTinyAddSTiny ,&SQL_OperUTinyAddUTiny ,&SQL_OperUTinyAddSBig ,&SQL_OperUTinyAddUBig ,&SQL_OperUTinyAddNum ,nullptr ,nullptr  ,nullptr              ,nullptr              ,nullptr               ,nullptr               ,nullptr                }
  /* CT_SBIGINT     */  ,{  &SQL_OperSBigAddChar  ,&SQL_OperSBigAddSShort  ,&SQL_OperSBigAddUShort  ,&SQL_OperSBigAddSLong  ,&SQL_OperSBigAddULong  ,&SQL_OperSBigAddFloat  ,&SQL_OperSBigAddDouble  ,&SQL_OperSBigAddBit  ,&SQL_OperSBigAddSTiny  ,&SQL_OperSBigAddUTiny  ,&SQL_OperSBigAddSBig  ,&SQL_OperSBigAddUBig  ,&SQL_OperSBigAddNum  ,nullptr ,nullptr  ,nullptr              ,nullptr              ,nullptr               ,nullptr               ,nullptr                }
  /* CT_UBIGINT     */  ,{  &SQL_OperUBigAddChar  ,&SQL_OperUBigAddSShort  ,&SQL_OperUBigAddUShort  ,&SQL_OperUBigAddSLong  ,&SQL_OperUBigAddULong  ,&SQL_OperUBigAddFloat  ,&SQL_OperUBigAddDouble  ,&SQL_OperUBigAddBit  ,&SQL_OperUBigAddSTiny  ,&SQL_OperUBigAddUTiny  ,&SQL_OperUBigAddSBig  ,&SQL_OperUBigAddUBig  ,&SQL_OperUBigAddNum  ,nullptr ,nullptr  ,nullptr              ,nullptr              ,nullptr               ,nullptr               ,nullptr                }
  /* CT_NUMERIC     */  ,{  &SQL_OperNumAddChar   ,&SQL_OperNumAddSShort   ,&SQL_OperNumAddUShort   ,&SQL_OperNumAddSLong   ,&SQL_OperNumAddULong   ,&SQL_OperNumAddFloat   ,&SQL_OperNumAddDouble   ,&SQL_OperNumAddBit   ,&SQL_OperNumAddSTiny   ,&SQL_OperNumAddUTiny   ,&SQL_OperNumAddSBig   ,&SQL_OperNumAddUBig   ,&SQL_OperNumAddNum   ,nullptr ,nullptr  ,nullptr              ,nullptr              ,nullptr               ,nullptr               ,nullptr                }
  /* CT_GUID        */  ,{  nullptr               ,nullptr                 ,nullptr                 ,nullptr                ,nullptr                ,nullptr                ,nullptr                 ,nullptr              ,nullptr                ,nullptr                ,nullptr               ,nullptr               ,nullptr              ,nullptr ,nullptr  ,nullptr              ,nullptr              ,nullptr               ,nullptr               ,nullptr                }
  /* CT_BINARY      */  ,{  nullptr               ,nullptr                 ,nullptr                 ,nullptr                ,nullptr                ,nullptr                ,nullptr                 ,nullptr              ,nullptr                ,nullptr                ,nullptr               ,nullptr               ,nullptr              ,nullptr ,nullptr  ,nullptr              ,nullptr              ,nullptr               ,nullptr               ,nullptr                }
  /* CT_DATE        */  ,{  nullptr               ,nullptr                 ,nullptr                 ,nullptr                ,nullptr                ,nullptr                ,nullptr                 ,nullptr              ,nullptr                ,nullptr                ,nullptr               ,nullptr               ,nullptr              ,nullptr ,nullptr  ,nullptr              ,&SQL_OperDateAddTime ,nullptr               ,nullptr               ,&SQL_OperDateAddIntDS  }
  /* CT_TIME        */  ,{  nullptr               ,nullptr                 ,nullptr                 ,nullptr                ,nullptr                ,nullptr                ,nullptr                 ,nullptr              ,nullptr                ,nullptr                ,nullptr               ,nullptr               ,nullptr              ,nullptr ,nullptr  ,&SQL_OperTimeAddDate ,nullptr              ,nullptr               ,nullptr               ,&SQL_OperTimeAddIntDS  }
  /* CT_TIMESTAMP   */  ,{  nullptr               ,nullptr                 ,nullptr                 ,nullptr                ,nullptr                ,nullptr                ,nullptr                 ,nullptr              ,nullptr                ,nullptr                ,nullptr               ,nullptr               ,nullptr              ,nullptr ,nullptr  ,nullptr              ,nullptr              ,nullptr               ,nullptr               ,&SQL_OperStampAddIntDS }
  /* CT_INTERVAL_YM */  ,{  nullptr               ,nullptr                 ,nullptr                 ,nullptr                ,nullptr                ,nullptr                ,nullptr                 ,nullptr              ,nullptr                ,nullptr                ,nullptr               ,nullptr               ,nullptr              ,nullptr ,nullptr  ,nullptr              ,nullptr              ,nullptr               ,&SQL_OperIntYMAddIntYM,nullptr                }
  /* CT_INTERVAL_DS */  ,{  nullptr               ,nullptr                 ,nullptr                 ,nullptr                ,nullptr                ,nullptr                ,nullptr                 ,nullptr              ,nullptr                ,nullptr                ,nullptr               ,nullptr               ,nullptr              ,nullptr ,nullptr  ,&SQL_OperIntDSAddDate,&SQL_OperIntDSAddTime,&SQL_OperIntDSAddStamp,nullptr               ,&SQL_OperIntDSAddIntDS }
};


// Add operator for SQLVariant
SQLVariant
SQLVariant::operator+(SQLVariant& p_right)
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
  OperatorCalculate function = OperatorAdd[left][right].function;
  if(function)
  {
    return (*function)(*this,p_right);
  }
  // No compare function found
  // Data types are not comparable
  XString leftType  = FindDatatype(m_datatype);
  XString rightType = FindDatatype(p_right.m_datatype);
  XString error;
  error.Format("Cannot do the add operator on (%s + %s)",leftType.GetString(),rightType.GetString());
  throw StdException(error);
}

SQLVariant&
SQLVariant::operator+=(SQLVariant& p_right)
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
    OperatorCalculate function = OperatorAdd[left][right].function;
    if(function)
    {
      *this = (*function)(*this,p_right);
      return *this;
    }
  }
  else
  {
    ThrowErrorOperator(SVO_AssignAdd);
  }
  // No compare function found
  // Data types are not comparable
  XString leftType  = FindDatatype(m_datatype);
  XString rightType = FindDatatype(p_right.m_datatype);
  XString error;
  error.Format("Cannot do the += operator on (%s + %s)",leftType.GetString(),rightType.GetString());
  throw StdException(error);
}


// End of namespace
}
