////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantOperatorSub.cpp
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

SQLVariant 
SQL_OperSShortSubChar(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort() - p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUShortSubChar(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort() - p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSLongSubChar(SQLVariant& p_left,SQLVariant& p_right)
{
  long result = p_left.GetAsSLong() - p_right.GetAsSLong();
  return SQLVariant(result);
}
SQLVariant 
SQL_OperULongSubChar(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned long result = p_left.GetAsULong();
  result -= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperFloatSubChar(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result -= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperDoubleSubChar(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result -= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperBitSubChar(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit() == false)
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSTinySubChar(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result -= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUTinySubChar(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result -= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSBigSubChar(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result -= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUBigSubChar(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result -= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperNumSubChar(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() - bcd((const char*)p_right.GetAsChar());
  SQLVariant var(&num);
  return var;
}

// TYPE == SSHORT

SQLVariant 
SQL_OperCharSubSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result -= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSShortSubSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result -= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUShortSubSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result -= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant SQL_OperSLongSubSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  long result = p_left.GetAsSLong();
  result -= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperULongSubSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned long result = p_left.GetAsULong();
  result -= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperFloatSubSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result -= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperDoubleSubSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result -= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperBitSubSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit() == false)
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSTinySubSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result -= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUTinySubSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result -= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSBigSubSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result -= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUBigSubSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result -= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperNumSubSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() - bcd(p_right.GetAsSShort());
  SQLVariant var(&num);
  return var;
}

// TYPE == USHORT

SQLVariant 
SQL_OperCharSubUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result -= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSShortSubUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result -= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUShortSubUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result -= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSLongSubUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  long result = p_left.GetAsSLong();
  result -= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUlongSubUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned long result = p_left.GetAsULong();
  result -= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperFloatSubUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result -= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperDoubleSubUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result -= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperBitSubUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit() == false)
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSTinySubUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result -= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUTinySubUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result -= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSBigSubUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result -= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUBigSubUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result -= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperNumSubUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() - bcd((long)p_right.GetAsUShort());
  SQLVariant var(&num);
  return var;
}

// TYPE == SLONG

SQLVariant 
SQL_OperCharSubSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  long result = p_left.GetAsSLong();
  result -= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSShortSubSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result -= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUShortSubSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result -= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSLongSubSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  long result = p_left.GetAsSLong();
  result -= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperULongSubSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned long result = p_left.GetAsULong();
  result -= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperFloatSubSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result -= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperDoubleSubSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result -= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperBitSubSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit() == false)
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSTinySubSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result -= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUTinySubSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result -= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSBigSubSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result -= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUBigSubSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result -= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperNumSubSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() - bcd(p_right.GetAsSLong());
  SQLVariant var(&num);
  return var;
}

// TYPE == ULONG

SQLVariant 
SQL_OperCharSubULong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned long result = p_left.GetAsULong();
  result -= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSShortSubULong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result -= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUShortSubULong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result -= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSLongSubULong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result -= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperULongSubULong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned long result = p_left.GetAsULong();
  result -= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperFloatSubULong(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result -= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperDoubleSubULong(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result -= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperBitSubULong(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit() == false)
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSTinySubULong(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result -= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUTinySubULong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result -= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSBigSubULong(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result -= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUBigSubULong(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result -= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperNumSubULong(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() - bcd((int64)p_right.GetAsUShort());
  SQLVariant var(&num);
  return var;
}

// TYPE == FLOAT

SQLVariant 
SQL_OperCharSubFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result -= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSShortSubFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result -= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUShortSubFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result -= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSLongSubFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  long result = p_left.GetAsSLong();
  result -= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperULongSubFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned long result = p_left.GetAsULong();
  result -= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperFloatSubFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result -= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperDoubleSubFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result -= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperBitSubFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit() == false)
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSTinySubFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result -= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUTinySubFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result -= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSBigSubFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result -= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUBigSubFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result -= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperNumSubFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() - bcd((double)p_right.GetAsFloat());
  SQLVariant var(&num);
  return var;
}

// TYPE == DOUBLE

SQLVariant 
SQL_OperCharSubDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result -= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSShortSubDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsUShort();
  result -= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUShortSubDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result -= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSLongSubDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  long result = p_left.GetAsSLong();
  result -= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperULongSubDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned long result = p_left.GetAsULong();
  result -= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperFloatSubDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result -= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperDoubleSubDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result -= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperBitSubDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit() == false)
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSTinySubDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result -= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUTinySubDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result -= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSBigSubDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result -= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUBigSubDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result -= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperNumSubDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() - bcd(p_right.GetAsDouble());
  SQLVariant var(&num);
  return var;
}

// TYPE == BIT

SQLVariant 
SQL_OperCharSubBit(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit() == false)
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSShortSubBit(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  if(p_right.GetAsBit()) 
  {
    --result;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUShortSubBit(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  if(p_right.GetAsBit())
  {
    --result;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSLongSubBit(SQLVariant& p_left,SQLVariant& p_right)
{
  long result = p_left.GetAsSLong();
  if(p_right.GetAsBit())
  {
    --result;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperULongSubBit(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned long result = p_left.GetAsULong();
  if(p_right.GetAsBit())
  {
    --result;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperFloatSubBit(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  if(p_right.GetAsBit())
  {
    result -= 1.0;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperDoubleSubBit(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  if(p_right.GetAsBit())
  {
    result -= 1.0;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperBitSubBit(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit() == false)
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSTinySubBit(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  if(p_right.GetAsBit())
  {
    --result;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUTinySubBit(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  if(p_right.GetAsBit())
  {
    --result;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSBigSubBit(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  if(p_right.GetAsBit())
  {
    --result;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUBigSubBit(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  if(p_right.GetAsBit())
  {
    --result;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperNumSubBit(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() - bcd(p_right.GetAsBit());
  SQLVariant var(&num);
  return var;
}

// TYPE == STINYINT

SQLVariant 
SQL_OperCharSubSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result -= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSShortSubSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result -= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUShortSubSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result -= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSLongSubSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  long result = p_left.GetAsSLong();
  result -= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperULongSubSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned long result = p_left.GetAsULong();
  result -= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperFloatSubSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result -= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperDoubleSubSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result -= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperBitSubSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit() == false)
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSTinySubSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result -= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUTinySubSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result -= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSBigSubSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result -= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUBigSubSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result -= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperNumSubSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() - bcd(p_right.GetAsSTinyInt());
  SQLVariant var(&num);
  return var;
}

// TYPE = UTINYINT

SQLVariant 
SQL_OperCharSubUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result -= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSShortSubUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result -= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUShortSubUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result -= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSLongSubUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  long result = p_left.GetAsSLong();
  result -= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperULongSubUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned long result = p_left.GetAsULong();
  result -= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperFloatSubUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result -= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperDoubleSubUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result -= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperBitSubUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit() == false)
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSTinySubUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result -= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUTinySubUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result -= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSBigSubUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result -= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUBigSubUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result -= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperNumSubUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() - bcd((short)p_right.GetAsUTinyInt());
  SQLVariant var(&num);
  return var;
}

// TYPE == SBIGINT

SQLVariant 
SQL_OperCharSubSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result -= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSShortSubSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result -= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUShortSubSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result -= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSLongSubSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  long result = p_left.GetAsSLong();
  result -= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperULongSubSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned long result = p_left.GetAsULong();
  result -= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperFloatSubSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result -= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperDoubleSubSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result -= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperBitSubSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit() == false)
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSTinySubSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result -= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUTinySubSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result -= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSBigSubSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result -= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUBigSubSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result -= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperNumSubSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() - bcd(p_right.GetAsSBigInt());
  SQLVariant var(&num);
  return var;
}

// TYPE == UBIGINT

SQLVariant 
SQL_OperCharSubUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result -= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSShortSubUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result -= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUShortSubUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result -= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSLongSubUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  long result = p_left.GetAsSLong();
  result -= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperULongSubUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned long result = p_left.GetAsULong();
  result -= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperFloatSubUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result -= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperDoubleSubUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result -= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperBitSubUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit() == false)
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSTinySubUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result -= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUTinySubUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result -= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSBigSubUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result -= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUBigSubUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result -= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperNumSubUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() - bcd(p_right.GetAsUBigInt());
  SQLVariant var(&num);
  return var;
}

// TYPE == NUMERIC

SQLVariant 
SQL_OperCharSubNum(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = bcd(p_left.GetAsChar()) - p_right.GetAsBCD();
  CString str;
  str = num.AsString();
  SQLVariant var(str);
  return var;
}

SQLVariant 
SQL_OperSShortSubNum(SQLVariant& p_left,SQLVariant& p_right)
{
  short num = p_left.GetAsSShort() - (short) p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
SQL_OperUShortSubNum(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short num = p_left.GetAsUShort() - (unsigned short) p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
SQL_OperSLongSubNum(SQLVariant& p_left,SQLVariant& p_right)
{
  long num = p_left.GetAsSLong() - p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
SQL_OperULongSubNum(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned long num = p_left.GetAsULong() - (unsigned long) p_right.GetAsBCD().AsInt64();
  SQLVariant var(num);
  return var;
}

SQLVariant 
SQL_OperFloatSubNum(SQLVariant& p_left,SQLVariant& p_right)
{
  float num = p_left.GetAsFloat() - (float) p_right.GetAsBCD().AsDouble();
  SQLVariant var(num);
  return var;
}

SQLVariant 
SQL_OperDoubleSubNum(SQLVariant& p_left,SQLVariant& p_right)
{
  double num = p_left.GetAsDouble() - p_right.GetAsBCD().AsDouble();
  SQLVariant var(num);
  return var;
}

SQLVariant 
SQL_OperBitSubNum(SQLVariant& p_left,SQLVariant& p_right)
{
  bool num = false;
  if(p_left.GetAsBit() && p_right.GetAsBCD().IsNull())
  {
    num = true;
  }
  SQLVariant var(num);
  return var;
}

SQLVariant 
SQL_OperSTinySubNum(SQLVariant& p_left,SQLVariant& p_right)
{
  char num = p_left.GetAsSTinyInt() - (char)p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
SQL_OperUTinySubNum(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char num = p_left.GetAsUTinyInt() - (unsigned char) p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
SQL_OperSBigSubNum(SQLVariant& p_left,SQLVariant& p_right)
{
  int64 num = p_left.GetAsSBigInt() - p_right.GetAsBCD().AsInt64();
  SQLVariant var(num);
  return var;
}

SQLVariant 
SQL_OperUBigSubNum(SQLVariant& p_left,SQLVariant& p_right)
{
  uint64 num = p_left.GetAsUBigInt() - p_right.GetAsBCD().AsUInt64();
  SQLVariant var(num);
  return var;
}

SQLVariant 
SQL_OperNumSubNum(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() - p_right.GetAsBCD();
  SQLVariant var(&num);
  return var;
}

// TYPE == DATE
#pragma warning (disable: 4239)

SQLVariant 
SQL_OperDateSubDate(SQLVariant& p_left,SQLVariant& p_right)
{
  const SQLDate date1 = p_left.GetAsSQLDate();
  const SQLDate date2 = p_right.GetAsSQLDate();
  SQLInterval result = date1 - date2;
  return SQLVariant(&result);
}

// TYPE == TIME

SQLVariant 
SQL_OperTimeSubTime(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLTime() - p_right.GetAsSQLTime();
  return SQLVariant(&result);
}

// TYPE == TIMESTAMP

SQLVariant 
SQL_OperStampSubStamp(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLTimestamp() - p_right.GetAsSQLTimestamp();
  return SQLVariant(&result);
}


// TYPE == INTERVAL_YEAR_MONTH

SQLVariant 
SQL_OperIntYMSubIntYM(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() - p_right.GetAsSQLInterval();
  return SQLVariant(&result);
}

// TYPE == INTERVAL_DAY_SECOND

SQLVariant 
SQL_OperDateSubIntDS(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLDate result = p_left.GetAsSQLDate();
  result = result - p_right.GetAsSQLInterval();
  return SQLVariant(&result);
}

SQLVariant 
SQL_OperTimeSubIntDS(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLTime result = p_left.GetAsSQLTime();
  result = result - p_right.GetAsSQLInterval();
  return SQLVariant(&result);
}

SQLVariant 
SQL_OperStampSubIntDS(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLTimestamp result = p_left.GetAsSQLTimestamp();
  result = result - p_right.GetAsSQLInterval();
  return SQLVariant(&result);
}

SQLVariant 
SQL_OperIntDSSubIntDS(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() - p_right.GetAsSQLInterval();
  return SQLVariant(&result);
}

// OPERATOR ARRAY

static CalculateFunctionArray OperatorSub[CT_LAST][CT_LAST] =
{
                         // CT_CHAR                CT_SSHORT                CT_USHORT                CT_SLONG                CT_ULONG                CT_FLOAT                CT_DOUBLE                CT_BIT                CT_STINYINT             CT_UTINYINT             CT_SBIGINT             CT_UBIGINT             CT_NUMERIC            CT_GUID  CT_BINARY CT_DATE               CT_TIME              CT_TIMESTAMP           CT_INTERVAL_YM         CT_INTERVAL_DS
                         // ---------------------- ------------------------ ------------------------ ----------------------- ----------------------- ----------------------- ------------------------ --------------------- ----------------------- ----------------------- ---------------------- ---------------------- --------------------- -------- --------- --------------------- -------------------- ---------------------- ---------------------- ------------------------
  /* CT_CHAR        */   {  nullptr               ,&SQL_OperCharSubSShort  ,&SQL_OperCharSubUShort  ,&SQL_OperCharSubSLong  ,&SQL_OperCharSubULong  ,&SQL_OperCharSubFloat  ,&SQL_OperCharSubDouble  ,&SQL_OperCharSubBit  ,&SQL_OperCharSubSTiny  ,&SQL_OperCharSubUTiny  ,&SQL_OperCharSubSBig  ,&SQL_OperCharSubUBig  ,&SQL_OperCharSubNum  ,nullptr ,nullptr  ,nullptr              ,nullptr             ,nullptr               ,nullptr               ,nullptr                }
  /* CT_SSHORT      */  ,{  &SQL_OperSShortSubChar,&SQL_OperSShortSubSShort,&SQL_OperSShortSubUShort,&SQL_OperSShortSubSLong,&SQL_OperSShortSubULong,&SQL_OperSShortSubFloat,&SQL_OperSShortSubDouble,&SQL_OperSShortSubBit,&SQL_OperSShortSubSTiny,&SQL_OperSShortSubUTiny,&SQL_OperSShortSubSBig,&SQL_OperSShortSubUBig,&SQL_OperSShortSubNum,nullptr ,nullptr  ,nullptr              ,nullptr             ,nullptr               ,nullptr               ,nullptr                }
  /* CT_USHORT      */  ,{  &SQL_OperUShortSubChar,&SQL_OperUShortSubSShort,&SQL_OperUShortSubUShort,&SQL_OperUShortSubSLong,&SQL_OperUShortSubULong,&SQL_OperUShortSubFloat,&SQL_OperUShortSubDouble,&SQL_OperUShortSubBit,&SQL_OperUShortSubSTiny,&SQL_OperUShortSubUTiny,&SQL_OperUShortSubSBig,&SQL_OperUShortSubUBig,&SQL_OperUShortSubNum,nullptr ,nullptr  ,nullptr              ,nullptr             ,nullptr               ,nullptr               ,nullptr                }
  /* CT_SLONG       */  ,{  &SQL_OperSLongSubChar ,&SQL_OperSLongSubSShort ,&SQL_OperSLongSubUShort ,&SQL_OperSLongSubSLong ,&SQL_OperSLongSubULong ,&SQL_OperSLongSubFloat ,&SQL_OperSLongSubDouble ,&SQL_OperSLongSubBit ,&SQL_OperSLongSubSTiny ,&SQL_OperSLongSubUTiny ,&SQL_OperSLongSubSBig ,&SQL_OperSLongSubUBig ,&SQL_OperSLongSubNum ,nullptr ,nullptr  ,nullptr              ,nullptr             ,nullptr               ,nullptr               ,nullptr                }
  /* CT_ULONG       */  ,{  &SQL_OperULongSubChar ,&SQL_OperULongSubSShort ,&SQL_OperUlongSubUShort ,&SQL_OperULongSubSLong ,&SQL_OperULongSubULong ,&SQL_OperULongSubFloat ,&SQL_OperULongSubDouble ,&SQL_OperULongSubBit ,&SQL_OperULongSubSTiny ,&SQL_OperULongSubUTiny ,&SQL_OperULongSubSBig ,&SQL_OperULongSubUBig ,&SQL_OperULongSubNum ,nullptr ,nullptr  ,nullptr              ,nullptr             ,nullptr               ,nullptr               ,nullptr                }
  /* CT_FLOAT       */  ,{  &SQL_OperFloatSubChar ,&SQL_OperFloatSubSShort ,&SQL_OperFloatSubUShort ,&SQL_OperFloatSubSLong ,&SQL_OperFloatSubULong ,&SQL_OperFloatSubFloat ,&SQL_OperFloatSubDouble ,&SQL_OperFloatSubBit ,&SQL_OperFloatSubSTiny ,&SQL_OperFloatSubUTiny ,&SQL_OperFloatSubSBig ,&SQL_OperFloatSubUBig ,&SQL_OperFloatSubNum ,nullptr ,nullptr  ,nullptr              ,nullptr             ,nullptr               ,nullptr               ,nullptr                }
  /* CT_DOUBLE      */  ,{  &SQL_OperDoubleSubChar,&SQL_OperDoubleSubSShort,&SQL_OperDoubleSubUShort,&SQL_OperDoubleSubSLong,&SQL_OperDoubleSubULong,&SQL_OperDoubleSubFloat,&SQL_OperDoubleSubDouble,&SQL_OperDoubleSubBit,&SQL_OperDoubleSubSTiny,&SQL_OperDoubleSubUTiny,&SQL_OperDoubleSubSBig,&SQL_OperDoubleSubUBig,&SQL_OperDoubleSubNum,nullptr ,nullptr  ,nullptr              ,nullptr             ,nullptr               ,nullptr               ,nullptr                }
  /* CT_BIT         */  ,{  &SQL_OperBitSubChar   ,&SQL_OperBitSubSShort   ,&SQL_OperBitSubUShort   ,&SQL_OperBitSubSLong   ,&SQL_OperBitSubULong   ,&SQL_OperBitSubFloat   ,&SQL_OperBitSubDouble   ,&SQL_OperBitSubBit   ,&SQL_OperBitSubSTiny   ,&SQL_OperBitSubUTiny   ,&SQL_OperBitSubSBig   ,&SQL_OperBitSubUBig   ,&SQL_OperBitSubNum   ,nullptr ,nullptr  ,nullptr              ,nullptr             ,nullptr               ,nullptr               ,nullptr                }
  /* CT_STINYINT    */  ,{  &SQL_OperSTinySubChar ,&SQL_OperSTinySubSShort ,&SQL_OperSTinySubUShort ,&SQL_OperSTinySubSLong ,&SQL_OperSTinySubULong ,&SQL_OperSTinySubFloat ,&SQL_OperSTinySubDouble ,&SQL_OperSTinySubBit ,&SQL_OperSTinySubSTiny ,&SQL_OperSTinySubUTiny ,&SQL_OperSTinySubSBig ,&SQL_OperSTinySubUBig ,&SQL_OperSTinySubNum ,nullptr ,nullptr  ,nullptr              ,nullptr             ,nullptr               ,nullptr               ,nullptr                }
  /* CT_UTINYINT    */  ,{  &SQL_OperUTinySubChar ,&SQL_OperUTinySubSShort ,&SQL_OperUTinySubUShort ,&SQL_OperUTinySubSLong ,&SQL_OperUTinySubULong ,&SQL_OperUTinySubFloat ,&SQL_OperUTinySubDouble ,&SQL_OperUTinySubBit ,&SQL_OperUTinySubSTiny ,&SQL_OperUTinySubUTiny ,&SQL_OperUTinySubSBig ,&SQL_OperUTinySubUBig ,&SQL_OperUTinySubNum ,nullptr ,nullptr  ,nullptr              ,nullptr             ,nullptr               ,nullptr               ,nullptr                }
  /* CT_SBIGINT     */  ,{  &SQL_OperSBigSubChar  ,&SQL_OperSBigSubSShort  ,&SQL_OperSBigSubUShort  ,&SQL_OperSBigSubSLong  ,&SQL_OperSBigSubULong  ,&SQL_OperSBigSubFloat  ,&SQL_OperSBigSubDouble  ,&SQL_OperSBigSubBit  ,&SQL_OperSBigSubSTiny  ,&SQL_OperSBigSubUTiny  ,&SQL_OperSBigSubSBig  ,&SQL_OperSBigSubUBig  ,&SQL_OperSBigSubNum  ,nullptr ,nullptr  ,nullptr              ,nullptr             ,nullptr               ,nullptr               ,nullptr                }
  /* CT_UBIGINT     */  ,{  &SQL_OperUBigSubChar  ,&SQL_OperUBigSubSShort  ,&SQL_OperUBigSubUShort  ,&SQL_OperUBigSubSLong  ,&SQL_OperUBigSubULong  ,&SQL_OperUBigSubFloat  ,&SQL_OperUBigSubDouble  ,&SQL_OperUBigSubBit  ,&SQL_OperUBigSubSTiny  ,&SQL_OperUBigSubUTiny  ,&SQL_OperUBigSubSBig  ,&SQL_OperUBigSubUBig  ,&SQL_OperUBigSubNum  ,nullptr ,nullptr  ,nullptr              ,nullptr             ,nullptr               ,nullptr               ,nullptr                }
  /* CT_NUMERIC     */  ,{  &SQL_OperNumSubChar   ,&SQL_OperNumSubSShort   ,&SQL_OperNumSubUShort   ,&SQL_OperNumSubSLong   ,&SQL_OperNumSubULong   ,&SQL_OperNumSubFloat   ,&SQL_OperNumSubDouble   ,&SQL_OperNumSubBit   ,&SQL_OperNumSubSTiny   ,&SQL_OperNumSubUTiny   ,&SQL_OperNumSubSBig   ,&SQL_OperNumSubUBig   ,&SQL_OperNumSubNum   ,nullptr ,nullptr  ,nullptr              ,nullptr             ,nullptr               ,nullptr               ,nullptr                }
  /* CT_GUID        */  ,{  nullptr               ,nullptr                 ,nullptr                 ,nullptr                ,nullptr                ,nullptr                ,nullptr                 ,nullptr              ,nullptr                ,nullptr                ,nullptr               ,nullptr               ,nullptr              ,nullptr ,nullptr  ,nullptr              ,nullptr             ,nullptr               ,nullptr               ,nullptr                }
  /* CT_BINARY      */  ,{  nullptr               ,nullptr                 ,nullptr                 ,nullptr                ,nullptr                ,nullptr                ,nullptr                 ,nullptr              ,nullptr                ,nullptr                ,nullptr               ,nullptr               ,nullptr              ,nullptr ,nullptr  ,nullptr              ,nullptr             ,nullptr               ,nullptr               ,nullptr                }
  /* CT_DATE        */  ,{  nullptr               ,nullptr                 ,nullptr                 ,nullptr                ,nullptr                ,nullptr                ,nullptr                 ,nullptr              ,nullptr                ,nullptr                ,nullptr               ,nullptr               ,nullptr              ,nullptr ,nullptr  ,&SQL_OperDateSubDate ,nullptr             ,nullptr               ,nullptr               ,&SQL_OperDateSubIntDS  }
  /* CT_TIME        */  ,{  nullptr               ,nullptr                 ,nullptr                 ,nullptr                ,nullptr                ,nullptr                ,nullptr                 ,nullptr              ,nullptr                ,nullptr                ,nullptr               ,nullptr               ,nullptr              ,nullptr ,nullptr  ,nullptr              ,&SQL_OperTimeSubTime,nullptr               ,nullptr               ,&SQL_OperTimeSubIntDS  }
  /* CT_TIMESTAMP   */  ,{  nullptr               ,nullptr                 ,nullptr                 ,nullptr                ,nullptr                ,nullptr                ,nullptr                 ,nullptr              ,nullptr                ,nullptr                ,nullptr               ,nullptr               ,nullptr              ,nullptr ,nullptr  ,nullptr              ,nullptr             ,&SQL_OperStampSubStamp,nullptr               ,&SQL_OperStampSubIntDS }
  /* CT_INTERVAL_YM */  ,{  nullptr               ,nullptr                 ,nullptr                 ,nullptr                ,nullptr                ,nullptr                ,nullptr                 ,nullptr              ,nullptr                ,nullptr                ,nullptr               ,nullptr               ,nullptr              ,nullptr ,nullptr  ,nullptr              ,nullptr             ,nullptr               ,&SQL_OperIntYMSubIntYM,nullptr                }
  /* CT_INTERVAL_DS */  ,{  nullptr               ,nullptr                 ,nullptr                 ,nullptr                ,nullptr                ,nullptr                ,nullptr                 ,nullptr              ,nullptr                ,nullptr                ,nullptr               ,nullptr               ,nullptr              ,nullptr ,nullptr  ,nullptr              ,nullptr             ,nullptr               ,nullptr               ,&SQL_OperIntDSSubIntDS }
};

// Sub operator for SQLVariant
SQLVariant
SQLVariant::operator-(SQLVariant& p_right)
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
  OperatorCalculate function = OperatorSub[left][right].function;
  if(function)
  {
    return (*function)(*this,p_right);
  }
  // No compare function found
  // Data types are not comparable
  CString leftType  = FindDatatype(m_datatype);
  CString rightType = FindDatatype(p_right.m_datatype);
  CString error;
  error.Format("Cannot do the subtract operator on (%s - %s)",leftType,rightType);
  throw error;
}

