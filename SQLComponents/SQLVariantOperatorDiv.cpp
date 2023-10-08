////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantOperatorDiv.cpp
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
static SQL_OperSShortDivChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result /= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortDivChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongDivChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result /= p_right.GetAsSLong();
  return SQLVariant(result);
}
SQLVariant 
static SQL_OperULongDivChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int  result = p_left.GetAsULong();
  result /= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatDivChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result /= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleDivChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result /= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitDivChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyDivChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result /= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyDivChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result /= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigDivChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result /= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigDivChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result /= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumDivChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() / bcd(p_right.GetAsChar());
  SQLVariant var(&num);
  return var;
}

// TYPE == SSHORT

SQLVariant 
static SQL_OperCharDivSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result /= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortDivSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result /= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortDivSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant static SQL_OperSLongDivSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result /= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongDivSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result /= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatDivSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result /= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleDivSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result /= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitDivSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyDivSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result /= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyDivSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result /= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigDivSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result /= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigDivSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result /= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumDivSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() / bcd(p_right.GetAsSShort());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperIntYMDivSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperIntDSDivSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}


// TYPE == USHORT

SQLVariant 
static SQL_OperCharDivUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortDivUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortDivUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}


SQLVariant 
static SQL_OperSLongDivUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result /= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUlongDivUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result /= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatDivUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result /= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleDivUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result /= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitDivUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyDivUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result /= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyDivUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result /= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigDivUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result /= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigDivUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result /= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumDivUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() / bcd((long)p_right.GetAsUShort());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperIntYMDivUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperIntDSDivUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == SLONG

SQLVariant 
static SQL_OperCharDivSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result /= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortDivSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result /= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortDivSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongDivSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result /= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongDivSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result /= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatDivSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result /= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleDivSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result /= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitDivSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyDivSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result /= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyDivSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result /= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigDivSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result /= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigDivSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result /= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumDivSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() / bcd(p_right.GetAsSLong());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperIntYMDivSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperIntDSDivSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == ULONG

SQLVariant 
static SQL_OperCharDivULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result /= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortDivULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortDivULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongDivULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongDivULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result /= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatDivULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result /= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleDivULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result /= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitDivULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyDivULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result /= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyDivULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result /= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigDivULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result /= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigDivULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result /= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumDivULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() / bcd((int64)p_right.GetAsULong());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperIntYMDivULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperIntDSDivULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == FLOAT

SQLVariant 
static SQL_OperCharDivFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result /= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortDivFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result /= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortDivFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongDivFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result /= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongDivFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result /= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatDivFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result /= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleDivFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result /= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitDivFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyDivFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result /= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyDivFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result /= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigDivFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result /= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigDivFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result /= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumDivFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() / bcd((double)p_right.GetAsFloat());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperIntYMDivFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() / p_right.GetAsDouble();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperIntDSDivFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() / p_right.GetAsDouble();
  return SQLVariant(&result);
}

// TYPE == DOUBLE

SQLVariant 
static SQL_OperCharDivDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result /= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortDivDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short result = p_left.GetAsUShort();
  result /= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortDivDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongDivDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result /= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongDivDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result /= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatDivDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result /= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleDivDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result /= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitDivDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyDivDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result /= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyDivDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result /= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigDivDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result /= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigDivDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result /= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumDivDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() / bcd(p_right.GetAsDouble());
  SQLVariant var(&num);
  return var;
}

SQLVariant
static SQL_OperIntYMDivDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() / p_right.GetAsDouble();
  return SQLVariant(&result);
}

SQLVariant
static SQL_OperIntDSDivDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() / p_right.GetAsDouble();
  return SQLVariant(&result);
}


// TYPE == BIT

SQLVariant 
static SQL_OperCharDivBit(const SQLVariant& p_left,const SQLVariant& /*p_right*/)
{
  bool result = p_left.GetAsBit() != 0;
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortDivBit(const SQLVariant& p_left,const SQLVariant& /*p_right*/)
{
  short result = p_left.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortDivBit(const SQLVariant& p_left,const SQLVariant& /*p_right*/)
{
  unsigned short result = p_left.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongDivBit(const SQLVariant& p_left,const SQLVariant& /*p_right*/)
{
  int result = p_left.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongDivBit(const SQLVariant& p_left,const SQLVariant& /*p_right*/)
{
  unsigned int result = p_left.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatDivBit(const SQLVariant& p_left,const SQLVariant& /*p_right*/)
{
  float result = p_left.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleDivBit(const SQLVariant& p_left,const SQLVariant& /*p_right*/)
{
  double result = p_left.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitDivBit(const SQLVariant& p_left,const SQLVariant& /*p_right*/)
{
  bool result = p_left.GetAsBit() != 0;
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyDivBit(const SQLVariant& p_left,const SQLVariant& /*p_right*/)
{
  char result = p_left.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyDivBit(const SQLVariant& p_left,const SQLVariant& /*p_right*/)
{
  unsigned char result = p_left.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigDivBit(const SQLVariant& p_left,const SQLVariant& /*p_right*/)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigDivBit(const SQLVariant& p_left,const SQLVariant& /*p_right*/)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumDivBit(const SQLVariant& p_left,const SQLVariant& /*p_right*/)
{
  SQLVariant result(p_left);
  return result;
}

SQLVariant 
static SQL_OperIntYMDivBit(const SQLVariant& p_left,const SQLVariant& /*p_right*/)
{
  SQLInterval result = p_left.GetAsSQLInterval();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperIntDSDivBit(const SQLVariant& p_left,const SQLVariant& /*p_right*/)
{
  SQLInterval result = p_left.GetAsSQLInterval();
  return SQLVariant(&result);
}

// TYPE == STINYINT

SQLVariant 
static SQL_OperCharDivSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result /= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortDivSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result /= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortDivSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongDivSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result /= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongDivSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result /= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatDivSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result /= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleDivSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result /= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitDivSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyDivSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result /= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyDivSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result /= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigDivSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result /= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigDivSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result /= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumDivSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() / bcd(p_right.GetAsSTinyInt());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperIntYMDivSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperIntDSDivSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE = UTINYINT

SQLVariant 
static SQL_OperCharDivUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result /= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortDivUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result /= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortDivUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongDivUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result /= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongDivUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result /= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatDivUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result /= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleDivUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result /= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitDivUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyDivUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result /= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyDivUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result /= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigDivUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result /= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigDivUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result /= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumDivUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() / bcd((short)p_right.GetAsUTinyInt());
  SQLVariant var(&num);
  return var;
}

SQLVariant
static SQL_OperIntYMDivUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant
static SQL_OperIntDSDivUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}


// TYPE == SBIGINT

SQLVariant 
static SQL_OperCharDivSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result /= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortDivSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result /= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortDivSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongDivSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result /= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongDivSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result /= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatDivSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result /= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleDivSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result /= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitDivSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyDivSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result /= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyDivSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result /= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigDivSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result /= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigDivSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result /= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumDivSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() / bcd(p_right.GetAsSBigInt());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperIntYMDivSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperIntDSDivSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == UBIGINT

SQLVariant 
static SQL_OperCharDivUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result /= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortDivUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result /= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortDivUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongDivUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result /= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongDivUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result /= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatDivUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result /= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleDivUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result /= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitDivUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyDivUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result /= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyDivUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result /= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigDivUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result /= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigDivUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result /= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumDivUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() / bcd(p_right.GetAsUBigInt());
  SQLVariant var(&num);
  return var;
}

SQLVariant
static SQL_OperIntYMDivUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant
static SQL_OperIntDSDivUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == NUMERIC

SQLVariant 
static SQL_OperCharDivNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double num = p_left.GetAsDouble() / p_right.GetAsBCD().AsDouble();
  XString str;
  str.Format(_T("%lf"),num);
  SQLVariant var(str);
  return var;
}

SQLVariant 
static SQL_OperSShortDivNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short num = p_left.GetAsSShort() / (short) p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperUShortDivNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short num = p_left.GetAsUShort() / (unsigned short)p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperSLongDivNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int num = p_left.GetAsSLong() / p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperULongDivNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int num = p_left.GetAsULong() / (unsigned long)p_right.GetAsBCD().AsInt64();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperFloatDivNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float num = p_left.GetAsFloat() / (float)p_right.GetAsBCD().AsDouble();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperDoubleDivNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double num = p_left.GetAsDouble() / p_right.GetAsBCD().AsDouble();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperBitDivNum(const SQLVariant& p_left,const SQLVariant& /*p_right*/)
{
  SQLVariant var(p_left.GetAsBit());
  return var;
}

SQLVariant 
static SQL_OperSTinyDivNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char num = p_left.GetAsSTinyInt() / (char) p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperUTinyDivNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char num = p_left.GetAsUTinyInt() / (unsigned char) p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperSBigDivNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int64 num = p_left.GetAsSBigInt() / p_right.GetAsBCD().AsInt64();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperUBigDivNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  uint64 num = p_left.GetAsUBigInt() / p_right.GetAsBCD().AsUInt64();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperNumDivNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() / p_right.GetAsBCD();
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperIntYMDivNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() / p_right.GetAsDouble();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperIntDSDivNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() / p_right.GetAsDouble();
  return SQLVariant(&result);
}

// OPERATOR ARRAY

static CalculateFunctionArray OperatorDiv[CT_LAST][CT_LAST] =
{
                         // CT_CHAR                CT_SSHORT                CT_USHORT                CT_SLONG                CT_ULONG                CT_FLOAT                CT_DOUBLE                CT_BIT                CT_STINYINT             CT_UTINYINT             CT_SBIGINT             CT_UBIGINT             CT_NUMERIC            CT_GUID  CT_BINARY CT_DATE  CT_TIME  CT_TIMESTAMP CT_INTERVAL_YM CT_INTERVAL_DS
                         // ---------------------- ------------------------ ------------------------ ----------------------- ----------------------- ----------------------- ------------------------ --------------------- ----------------------- ----------------------- ---------------------- ---------------------- --------------------- -------- --------- -------- -------- ------------ -------------- --------------
  /* CT_CHAR        */   {  nullptr               ,&SQL_OperCharDivSShort  ,&SQL_OperCharDivUShort  ,&SQL_OperCharDivSLong  ,&SQL_OperCharDivULong  ,&SQL_OperCharDivFloat  ,&SQL_OperCharDivDouble  ,&SQL_OperCharDivBit  ,&SQL_OperCharDivSTiny  ,&SQL_OperCharDivUTiny  ,&SQL_OperCharDivSBig  ,&SQL_OperCharDivUBig  ,&SQL_OperCharDivNum  ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_SSHORT      */  ,{  &SQL_OperSShortDivChar,&SQL_OperSShortDivSShort,&SQL_OperSShortDivUShort,&SQL_OperSShortDivSLong,&SQL_OperSShortDivULong,&SQL_OperSShortDivFloat,&SQL_OperSShortDivDouble,&SQL_OperSShortDivBit,&SQL_OperSShortDivSTiny,&SQL_OperSShortDivUTiny,&SQL_OperSShortDivSBig,&SQL_OperSShortDivUBig,&SQL_OperSShortDivNum,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_USHORT      */  ,{  &SQL_OperUShortDivChar,&SQL_OperUShortDivSShort,&SQL_OperUShortDivUShort,&SQL_OperUShortDivSLong,&SQL_OperUShortDivULong,&SQL_OperUShortDivFloat,&SQL_OperUShortDivDouble,&SQL_OperUShortDivBit,&SQL_OperUShortDivSTiny,&SQL_OperUShortDivUTiny,&SQL_OperUShortDivSBig,&SQL_OperUShortDivUBig,&SQL_OperUShortDivNum,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_SLONG       */  ,{  &SQL_OperSLongDivChar ,&SQL_OperSLongDivSShort ,&SQL_OperSLongDivUShort ,&SQL_OperSLongDivSLong ,&SQL_OperSLongDivULong ,&SQL_OperSLongDivFloat ,&SQL_OperSLongDivDouble ,&SQL_OperSLongDivBit ,&SQL_OperSLongDivSTiny ,&SQL_OperSLongDivUTiny ,&SQL_OperSLongDivSBig ,&SQL_OperSLongDivUBig ,&SQL_OperSLongDivNum ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_ULONG       */  ,{  &SQL_OperULongDivChar ,&SQL_OperULongDivSShort ,&SQL_OperUlongDivUShort ,&SQL_OperULongDivSLong ,&SQL_OperULongDivULong ,&SQL_OperULongDivFloat ,&SQL_OperULongDivDouble ,&SQL_OperULongDivBit ,&SQL_OperULongDivSTiny ,&SQL_OperULongDivUTiny ,&SQL_OperULongDivSBig ,&SQL_OperULongDivUBig ,&SQL_OperULongDivNum ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_FLOAT       */  ,{  &SQL_OperFloatDivChar ,&SQL_OperFloatDivSShort ,&SQL_OperFloatDivUShort ,&SQL_OperFloatDivSLong ,&SQL_OperFloatDivULong ,&SQL_OperFloatDivFloat ,&SQL_OperFloatDivDouble ,&SQL_OperFloatDivBit ,&SQL_OperFloatDivSTiny ,&SQL_OperFloatDivUTiny ,&SQL_OperFloatDivSBig ,&SQL_OperFloatDivUBig ,&SQL_OperFloatDivNum ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_DOUBLE      */  ,{  &SQL_OperDoubleDivChar,&SQL_OperDoubleDivSShort,&SQL_OperDoubleDivUShort,&SQL_OperDoubleDivSLong,&SQL_OperDoubleDivULong,&SQL_OperDoubleDivFloat,&SQL_OperDoubleDivDouble,&SQL_OperDoubleDivBit,&SQL_OperDoubleDivSTiny,&SQL_OperDoubleDivUTiny,&SQL_OperDoubleDivSBig,&SQL_OperDoubleDivUBig,&SQL_OperDoubleDivNum,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_BIT         */  ,{  &SQL_OperBitDivChar   ,&SQL_OperBitDivSShort   ,&SQL_OperBitDivUShort   ,&SQL_OperBitDivSLong   ,&SQL_OperBitDivULong   ,&SQL_OperBitDivFloat   ,&SQL_OperBitDivDouble   ,&SQL_OperBitDivBit   ,&SQL_OperBitDivSTiny   ,&SQL_OperBitDivUTiny   ,&SQL_OperBitDivSBig   ,&SQL_OperBitDivUBig   ,&SQL_OperBitDivNum   ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_STINYINT    */  ,{  &SQL_OperSTinyDivChar ,&SQL_OperSTinyDivSShort ,&SQL_OperSTinyDivUShort ,&SQL_OperSTinyDivSLong ,&SQL_OperSTinyDivULong ,&SQL_OperSTinyDivFloat ,&SQL_OperSTinyDivDouble ,&SQL_OperSTinyDivBit ,&SQL_OperSTinyDivSTiny ,&SQL_OperSTinyDivUTiny ,&SQL_OperSTinyDivSBig ,&SQL_OperSTinyDivUBig ,&SQL_OperSTinyDivNum ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_UTINYINT    */  ,{  &SQL_OperUTinyDivChar ,&SQL_OperUTinyDivSShort ,&SQL_OperUTinyDivUShort ,&SQL_OperUTinyDivSLong ,&SQL_OperUTinyDivULong ,&SQL_OperUTinyDivFloat ,&SQL_OperUTinyDivDouble ,&SQL_OperUTinyDivBit ,&SQL_OperUTinyDivSTiny ,&SQL_OperUTinyDivUTiny ,&SQL_OperUTinyDivSBig ,&SQL_OperUTinyDivUBig ,&SQL_OperUTinyDivNum ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_SBIGINT     */  ,{  &SQL_OperSBigDivChar  ,&SQL_OperSBigDivSShort  ,&SQL_OperSBigDivUShort  ,&SQL_OperSBigDivSLong  ,&SQL_OperSBigDivULong  ,&SQL_OperSBigDivFloat  ,&SQL_OperSBigDivDouble  ,&SQL_OperSBigDivBit  ,&SQL_OperSBigDivSTiny  ,&SQL_OperSBigDivUTiny  ,&SQL_OperSBigDivSBig  ,&SQL_OperSBigDivUBig  ,&SQL_OperSBigDivNum  ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_UBIGINT     */  ,{  &SQL_OperUBigDivChar  ,&SQL_OperUBigDivSShort  ,&SQL_OperUBigDivUShort  ,&SQL_OperUBigDivSLong  ,&SQL_OperUBigDivULong  ,&SQL_OperUBigDivFloat  ,&SQL_OperUBigDivDouble  ,&SQL_OperUBigDivBit  ,&SQL_OperUBigDivSTiny  ,&SQL_OperUBigDivUTiny  ,&SQL_OperUBigDivSBig  ,&SQL_OperUBigDivUBig  ,&SQL_OperUBigDivNum  ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_NUMERIC     */  ,{  &SQL_OperNumDivChar   ,&SQL_OperNumDivSShort   ,&SQL_OperNumDivUShort   ,&SQL_OperNumDivSLong   ,&SQL_OperNumDivULong   ,&SQL_OperNumDivFloat   ,&SQL_OperNumDivDouble   ,&SQL_OperNumDivBit   ,&SQL_OperNumDivSTiny   ,&SQL_OperNumDivUTiny   ,&SQL_OperNumDivSBig   ,&SQL_OperNumDivUBig   ,&SQL_OperNumDivNum   ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_GUID        */  ,{  nullptr               ,nullptr                 ,nullptr                 ,nullptr                ,nullptr                ,nullptr                ,nullptr                 ,nullptr              ,nullptr                ,nullptr                ,nullptr               ,nullptr               ,nullptr              ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_BINARY      */  ,{  nullptr               ,nullptr                 ,nullptr                 ,nullptr                ,nullptr                ,nullptr                ,nullptr                 ,nullptr              ,nullptr                ,nullptr                ,nullptr               ,nullptr               ,nullptr              ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_DATE        */  ,{  nullptr               ,nullptr                 ,nullptr                 ,nullptr                ,nullptr                ,nullptr                ,nullptr                 ,nullptr              ,nullptr                ,nullptr                ,nullptr               ,nullptr               ,nullptr              ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_TIME        */  ,{  nullptr               ,nullptr                 ,nullptr                 ,nullptr                ,nullptr                ,nullptr                ,nullptr                 ,nullptr              ,nullptr                ,nullptr                ,nullptr               ,nullptr               ,nullptr              ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_TIMESTAMP   */  ,{  nullptr               ,nullptr                 ,nullptr                 ,nullptr                ,nullptr                ,nullptr                ,nullptr                 ,nullptr              ,nullptr                ,nullptr                ,nullptr               ,nullptr               ,nullptr              ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_INTERVAL_YM */  ,{  nullptr               ,&SQL_OperIntYMDivSShort ,&SQL_OperIntYMDivUShort ,&SQL_OperIntYMDivSLong ,&SQL_OperIntYMDivULong ,&SQL_OperIntYMDivFloat ,&SQL_OperIntYMDivDouble ,&SQL_OperIntYMDivBit ,&SQL_OperIntYMDivSTiny ,&SQL_OperIntYMDivUTiny ,&SQL_OperIntYMDivSBig ,&SQL_OperIntYMDivUBig ,&SQL_OperIntYMDivNum ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
  /* CT_INTERVAL_DS */  ,{  nullptr               ,&SQL_OperIntDSDivSShort ,&SQL_OperIntDSDivUShort ,&SQL_OperIntDSDivSLong ,&SQL_OperIntDSDivULong ,&SQL_OperIntDSDivFloat ,&SQL_OperIntDSDivDouble ,&SQL_OperIntDSDivBit ,&SQL_OperIntDSDivSTiny ,&SQL_OperIntDSDivUTiny ,&SQL_OperIntDSDivSBig ,&SQL_OperIntDSDivUBig ,&SQL_OperIntDSDivNum ,nullptr ,nullptr  ,nullptr ,nullptr ,nullptr     ,nullptr       ,nullptr      }
};

// Division operator for SQLVariant
SQLVariant
SQLVariant::operator/(const SQLVariant& p_right)
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

  // Check whether there is something to divide by!
  if(p_right.IsEmpty())
  {
    throw StdException("Cannot do a division by zero/empty");
  }

  // Find our comparison function
  OperatorCalculate function = OperatorDiv[left][right].function;
  if(function)
  {
    return (*function)(*this,p_right);
  }
  // No compare function found
  // Data types are not comparable
  XString leftType  = SQLDataType::FindDatatype(m_datatype);
  XString rightType = SQLDataType::FindDatatype(p_right.m_datatype);
  XString error;
  error.Format(_T("Cannot do the division operator on (%s / %s)"),leftType.GetString(),rightType.GetString());
  throw StdException(error);
}

SQLVariant& 
SQLVariant::operator/=(const SQLVariant& p_right)
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
    OperatorCalculate function = OperatorDiv[left][right].function;
    if(function)
    {
      *this = (*function)(*this,p_right);
      return *this;
    }
  }
  else
  {
    ThrowErrorOperator(SVO_AssignDivide);
  }
  // No compare function found
  // Data types are not comparable
  XString leftType  = SQLDataType::FindDatatype(m_datatype);
  XString rightType = SQLDataType::FindDatatype(p_right.m_datatype);
  XString error;
  error.Format(_T("Cannot do the /= operator on (%s + %s)"),leftType.GetString(),rightType.GetString());
  throw StdException(error);
}

// End of namespace
}
