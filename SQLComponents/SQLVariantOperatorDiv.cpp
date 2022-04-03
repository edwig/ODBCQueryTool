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
static SQL_OperSShortDivChar(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result /= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortDivChar(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongDivChar(SQLVariant& p_left,SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result /= p_right.GetAsSLong();
  return SQLVariant(result);
}
SQLVariant 
static SQL_OperULongDivChar(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned int  result = p_left.GetAsULong();
  result /= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatDivChar(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result /= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleDivChar(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result /= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitDivChar(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyDivChar(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result /= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyDivChar(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result /= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigDivChar(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result /= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigDivChar(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result /= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumDivChar(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() / bcd(p_right.GetAsChar());
  SQLVariant var(&num);
  return var;
}

// TYPE == SSHORT

SQLVariant 
static SQL_OperCharDivSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result /= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortDivSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result /= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortDivSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant static SQL_OperSLongDivSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result /= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongDivSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result /= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatDivSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result /= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleDivSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result /= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitDivSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyDivSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result /= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyDivSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result /= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigDivSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result /= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigDivSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result /= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumDivSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() / bcd(p_right.GetAsSShort());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperIntYMDivSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperIntDSDivSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}


// TYPE == USHORT

SQLVariant 
static SQL_OperCharDivUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortDivUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortDivUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}


SQLVariant 
static SQL_OperSLongDivUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result /= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUlongDivUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result /= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatDivUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result /= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleDivUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result /= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitDivUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyDivUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result /= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyDivUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result /= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigDivUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result /= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigDivUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result /= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumDivUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() / bcd((long)p_right.GetAsUShort());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperIntYMDivUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperIntDSDivUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == SLONG

SQLVariant 
static SQL_OperCharDivSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result /= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortDivSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result /= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortDivSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongDivSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result /= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongDivSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result /= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatDivSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result /= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleDivSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result /= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitDivSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyDivSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result /= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyDivSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result /= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigDivSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result /= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigDivSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result /= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumDivSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() / bcd(p_right.GetAsSLong());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperIntYMDivSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperIntDSDivSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == ULONG

SQLVariant 
static SQL_OperCharDivULong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result /= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortDivULong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortDivULong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongDivULong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongDivULong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result /= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatDivULong(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result /= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleDivULong(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result /= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitDivULong(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyDivULong(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result /= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyDivULong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result /= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigDivULong(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result /= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigDivULong(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result /= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumDivULong(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() / bcd((int64)p_right.GetAsULong());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperIntYMDivULong(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperIntDSDivULong(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == FLOAT

SQLVariant 
static SQL_OperCharDivFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result /= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortDivFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result /= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortDivFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongDivFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result /= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongDivFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result /= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatDivFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result /= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleDivFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result /= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitDivFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyDivFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result /= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyDivFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result /= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigDivFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result /= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigDivFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result /= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumDivFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() / bcd((double)p_right.GetAsFloat());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperIntYMDivFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() / p_right.GetAsDouble();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperIntDSDivFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() / p_right.GetAsDouble();
  return SQLVariant(&result);
}

// TYPE == DOUBLE

SQLVariant 
static SQL_OperCharDivDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result /= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortDivDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsUShort();
  result /= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortDivDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongDivDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result /= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongDivDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result /= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatDivDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result /= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleDivDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result /= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitDivDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyDivDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result /= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyDivDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result /= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigDivDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result /= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigDivDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result /= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumDivDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() / bcd(p_right.GetAsDouble());
  SQLVariant var(&num);
  return var;
}

SQLVariant
static SQL_OperIntYMDivDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() / p_right.GetAsDouble();
  return SQLVariant(&result);
}

SQLVariant
static SQL_OperIntDSDivDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() / p_right.GetAsDouble();
  return SQLVariant(&result);
}


// TYPE == BIT

SQLVariant 
static SQL_OperCharDivBit(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  bool result = p_left.GetAsBit() != 0;
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortDivBit(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  short result = p_left.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortDivBit(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  unsigned short result = p_left.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongDivBit(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  int result = p_left.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongDivBit(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  unsigned int result = p_left.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatDivBit(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  float result = p_left.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleDivBit(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  double result = p_left.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitDivBit(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  bool result = p_left.GetAsBit() != 0;
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyDivBit(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  char result = p_left.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyDivBit(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  unsigned char result = p_left.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigDivBit(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigDivBit(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumDivBit(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  SQLVariant result(p_left);
  return result;
}

SQLVariant 
static SQL_OperIntYMDivBit(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  SQLInterval result = p_left.GetAsSQLInterval();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperIntDSDivBit(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  SQLInterval result = p_left.GetAsSQLInterval();
  return SQLVariant(&result);
}

// TYPE == STINYINT

SQLVariant 
static SQL_OperCharDivSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result /= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortDivSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result /= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortDivSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongDivSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result /= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongDivSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result /= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatDivSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result /= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleDivSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result /= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitDivSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyDivSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result /= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyDivSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result /= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigDivSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result /= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigDivSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result /= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumDivSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() / bcd(p_right.GetAsSTinyInt());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperIntYMDivSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperIntDSDivSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE = UTINYINT

SQLVariant 
static SQL_OperCharDivUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result /= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortDivUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result /= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortDivUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongDivUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result /= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongDivUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result /= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatDivUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result /= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleDivUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result /= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitDivUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyDivUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result /= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyDivUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result /= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigDivUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result /= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigDivUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result /= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumDivUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() / bcd((short)p_right.GetAsUTinyInt());
  SQLVariant var(&num);
  return var;
}

SQLVariant
static SQL_OperIntYMDivUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant
static SQL_OperIntDSDivUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}


// TYPE == SBIGINT

SQLVariant 
static SQL_OperCharDivSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result /= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortDivSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result /= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortDivSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongDivSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result /= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongDivSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result /= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatDivSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result /= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleDivSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result /= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitDivSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyDivSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result /= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyDivSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result /= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigDivSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result /= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigDivSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result /= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumDivSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() / bcd(p_right.GetAsSBigInt());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperIntYMDivSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperIntDSDivSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == UBIGINT

SQLVariant 
static SQL_OperCharDivUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result /= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortDivUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result /= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortDivUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result /= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongDivUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result /= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongDivUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result /= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatDivUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result /= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleDivUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result /= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitDivUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinyDivUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result /= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinyDivUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result /= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigDivUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result /= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigDivUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result /= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumDivUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() / bcd(p_right.GetAsUBigInt());
  SQLVariant var(&num);
  return var;
}

SQLVariant
static SQL_OperIntYMDivUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant
static SQL_OperIntDSDivUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() / p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == NUMERIC

SQLVariant 
static SQL_OperCharDivNum(SQLVariant& p_left,SQLVariant& p_right)
{
  double num = p_left.GetAsDouble() / p_right.GetAsBCD().AsDouble();
  XString str;
  str.Format("%lf",num);
  SQLVariant var(str);
  return var;
}

SQLVariant 
static SQL_OperSShortDivNum(SQLVariant& p_left,SQLVariant& p_right)
{
  short num = p_left.GetAsSShort() / (short) p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperUShortDivNum(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short num = p_left.GetAsUShort() / (unsigned short)p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperSLongDivNum(SQLVariant& p_left,SQLVariant& p_right)
{
  int num = p_left.GetAsSLong() / p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperULongDivNum(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned int num = p_left.GetAsULong() / (unsigned long)p_right.GetAsBCD().AsInt64();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperFloatDivNum(SQLVariant& p_left,SQLVariant& p_right)
{
  float num = p_left.GetAsFloat() / (float)p_right.GetAsBCD().AsDouble();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperDoubleDivNum(SQLVariant& p_left,SQLVariant& p_right)
{
  double num = p_left.GetAsDouble() / p_right.GetAsBCD().AsDouble();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperBitDivNum(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  SQLVariant var(p_left.GetAsBit());
  return var;
}

SQLVariant 
static SQL_OperSTinyDivNum(SQLVariant& p_left,SQLVariant& p_right)
{
  char num = p_left.GetAsSTinyInt() / (char) p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperUTinyDivNum(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char num = p_left.GetAsUTinyInt() / (unsigned char) p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperSBigDivNum(SQLVariant& p_left,SQLVariant& p_right)
{
  int64 num = p_left.GetAsSBigInt() / p_right.GetAsBCD().AsInt64();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperUBigDivNum(SQLVariant& p_left,SQLVariant& p_right)
{
  uint64 num = p_left.GetAsUBigInt() / p_right.GetAsBCD().AsUInt64();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperNumDivNum(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() / p_right.GetAsBCD();
  SQLVariant var(&num);
  return var;
}

SQLVariant 
static SQL_OperIntYMDivNum(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() / p_right.GetAsDouble();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperIntDSDivNum(SQLVariant& p_left,SQLVariant& p_right)
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
SQLVariant::operator/(SQLVariant& p_right)
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
  XString leftType  = FindDatatype(m_datatype);
  XString rightType = FindDatatype(p_right.m_datatype);
  XString error;
  error.Format("Cannot do the division operator on (%s / %s)",leftType.GetString(),rightType.GetString());
  throw StdException(error);
}

SQLVariant& 
SQLVariant::operator/=(SQLVariant& p_right)
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

  // Check whether both datatypes are valid
  if(left == CT_LAST || right == CT_LAST)
  {
    ThrowErrorOperator(SVO_AssignDivide);
  }

  // Find our comparison function
  OperatorCalculate function = OperatorDiv[left][right].function;
  if(function)
  {
    *this = (*function)(*this,p_right);
    return *this;
  }
  // No compare function found
  // Data types are not comparable
  XString leftType  = FindDatatype(m_datatype);
  XString rightType = FindDatatype(p_right.m_datatype);
  XString error;
  error.Format("Cannot do the /= operator on (%s + %s)",leftType.GetString(),rightType.GetString());
  throw StdException(error);
}

// End of namespace
}
