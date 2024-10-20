////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantOperatorSub.cpp
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
static SQL_OperSShortSubChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short result = p_left.GetAsSShort() - p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortSubChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort() - p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongSubChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong() - p_right.GetAsSLong();
  return SQLVariant(result);
}
SQLVariant 
static SQL_OperULongSubChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result -= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatSubChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result -= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleSubChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result -= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitSubChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit() == false)
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinySubChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result -= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinySubChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result -= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigSubChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result -= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigSubChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result -= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumSubChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() - bcd(p_right.GetAsString());
  SQLVariant var(&num);
  return var;
}

// TYPE == SSHORT

SQLVariant 
static SQL_OperCharSubSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result -= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortSubSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result -= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortSubSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result -= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant static SQL_OperSLongSubSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result -= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongSubSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result -= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatSubSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result -= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleSubSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result -= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitSubSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit() == false)
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinySubSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result -= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinySubSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result -= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigSubSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result -= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigSubSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result -= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumSubSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() - bcd(p_right.GetAsSShort());
  SQLVariant var(&num);
  return var;
}

// TYPE == USHORT

SQLVariant 
static SQL_OperCharSubUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result -= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortSubUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result -= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortSubUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result -= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongSubUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result -= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUlongSubUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result -= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatSubUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result -= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleSubUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result -= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitSubUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit() == false)
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinySubUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result -= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinySubUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result -= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigSubUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result -= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigSubUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result -= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumSubUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() - bcd((long)p_right.GetAsUShort());
  SQLVariant var(&num);
  return var;
}

// TYPE == SLONG

SQLVariant 
static SQL_OperCharSubSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result -= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortSubSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result -= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortSubSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result -= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongSubSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result -= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongSubSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result -= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatSubSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result -= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleSubSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result -= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitSubSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit() == false)
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinySubSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result -= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinySubSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result -= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigSubSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result -= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigSubSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result -= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumSubSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() - bcd(p_right.GetAsSLong());
  SQLVariant var(&num);
  return var;
}

// TYPE == ULONG

SQLVariant 
static SQL_OperCharSubULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result -= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortSubULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result -= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortSubULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result -= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongSubULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result -= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongSubULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result -= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatSubULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result -= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleSubULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result -= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitSubULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit() == false)
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinySubULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result -= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinySubULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result -= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigSubULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result -= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigSubULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result -= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumSubULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() - bcd((int64)p_right.GetAsUShort());
  SQLVariant var(&num);
  return var;
}

// TYPE == FLOAT

SQLVariant 
static SQL_OperCharSubFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result -= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortSubFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result -= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortSubFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result -= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongSubFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result -= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongSubFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result -= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatSubFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result -= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleSubFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result -= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitSubFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit() == false)
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinySubFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result -= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinySubFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result -= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigSubFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result -= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigSubFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result -= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumSubFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() - bcd((double)p_right.GetAsFloat());
  SQLVariant var(&num);
  return var;
}

// TYPE == DOUBLE

SQLVariant 
static SQL_OperCharSubDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result -= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortSubDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short result = p_left.GetAsUShort();
  result -= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortSubDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result -= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongSubDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result -= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongSubDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result -= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatSubDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result -= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleSubDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result -= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitSubDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit() == false)
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinySubDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result -= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinySubDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result -= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigSubDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result -= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigSubDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result -= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumSubDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() - bcd(p_right.GetAsDouble());
  SQLVariant var(&num);
  return var;
}

// TYPE == BIT

SQLVariant 
static SQL_OperCharSubBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit() == false)
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortSubBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  if(p_right.GetAsBit()) 
  {
    --result;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortSubBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  if(p_right.GetAsBit())
  {
    --result;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongSubBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  if(p_right.GetAsBit())
  {
    --result;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongSubBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  if(p_right.GetAsBit())
  {
    --result;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatSubBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  if(p_right.GetAsBit())
  {
    result -= 1.0;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleSubBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  if(p_right.GetAsBit())
  {
    result -= 1.0;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitSubBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit() == false)
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinySubBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  if(p_right.GetAsBit())
  {
    --result;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinySubBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  if(p_right.GetAsBit())
  {
    --result;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigSubBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  if(p_right.GetAsBit())
  {
    --result;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigSubBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  if(p_right.GetAsBit())
  {
    --result;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumSubBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() - bcd(p_right.GetAsBit());
  SQLVariant var(&num);
  return var;
}

// TYPE == STINYINT

SQLVariant 
static SQL_OperCharSubSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result -= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortSubSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result -= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortSubSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result -= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongSubSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result -= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongSubSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result -= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatSubSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result -= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleSubSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result -= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitSubSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit() == false)
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinySubSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result -= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinySubSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result -= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigSubSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result -= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigSubSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result -= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumSubSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() - bcd(p_right.GetAsSTinyInt());
  SQLVariant var(&num);
  return var;
}

// TYPE = UTINYINT

SQLVariant 
static SQL_OperCharSubUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result -= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortSubUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result -= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortSubUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result -= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongSubUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result -= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongSubUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result -= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatSubUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result -= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleSubUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result -= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitSubUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit() == false)
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinySubUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result -= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinySubUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result -= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigSubUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result -= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigSubUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result -= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumSubUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() - bcd((short)p_right.GetAsUTinyInt());
  SQLVariant var(&num);
  return var;
}

// TYPE == SBIGINT

SQLVariant 
static SQL_OperCharSubSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result -= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortSubSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result -= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortSubSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result -= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongSubSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result -= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongSubSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result -= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatSubSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result -= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleSubSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result -= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitSubSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit() == false)
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinySubSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result -= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinySubSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result -= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigSubSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result -= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigSubSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result -= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumSubSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() - bcd(p_right.GetAsSBigInt());
  SQLVariant var(&num);
  return var;
}

// TYPE == UBIGINT

SQLVariant 
static SQL_OperCharSubUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result -= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSShortSubUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result -= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUShortSubUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result -= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSLongSubUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int result = p_left.GetAsSLong();
  result -= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperULongSubUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int result = p_left.GetAsULong();
  result -= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperFloatSubUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float result = p_left.GetAsFloat();
  result -= p_right.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperDoubleSubUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double result = p_left.GetAsDouble();
  result -= p_right.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperBitSubUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit() == false)
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSTinySubUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result -= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUTinySubUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result -= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperSBigSubUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result -= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperUBigSubUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result -= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
static SQL_OperNumSubUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() - bcd(p_right.GetAsUBigInt());
  SQLVariant var(&num);
  return var;
}

// TYPE == NUMERIC

SQLVariant 
static SQL_OperCharSubNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = bcd(p_left.GetAsString()) - p_right.GetAsBCD();
  XString str;
  str = num.AsString(bcd::Format::Bookkeeping,false,0);
  SQLVariant var(str);
  return var;
}

SQLVariant 
static SQL_OperSShortSubNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  short num = p_left.GetAsSShort() - (short) p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperUShortSubNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned short num = p_left.GetAsUShort() - (unsigned short) p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperSLongSubNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int num = p_left.GetAsSLong() - p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperULongSubNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned int num = p_left.GetAsULong() - (unsigned long) p_right.GetAsBCD().AsInt64();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperFloatSubNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float num = p_left.GetAsFloat() - (float) p_right.GetAsBCD().AsDouble();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperDoubleSubNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double num = p_left.GetAsDouble() - p_right.GetAsBCD().AsDouble();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperBitSubNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bool num = false;
  if(p_left.GetAsBit() && p_right.GetAsBCD().IsZero())
  {
    num = true;
  }
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperSTinySubNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  char num = p_left.GetAsSTinyInt() - (char)p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperUTinySubNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  unsigned char num = p_left.GetAsUTinyInt() - (unsigned char) p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperSBigSubNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  int64 num = p_left.GetAsSBigInt() - p_right.GetAsBCD().AsInt64();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperUBigSubNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  uint64 num = p_left.GetAsUBigInt() - p_right.GetAsBCD().AsUInt64();
  SQLVariant var(num);
  return var;
}

SQLVariant 
static SQL_OperNumSubNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() - p_right.GetAsBCD();
  SQLVariant var(&num);
  return var;
}

// TYPE == DATE

SQLVariant 
static SQL_OperDateSubDate(const SQLVariant& p_left,const SQLVariant& p_right)
{
  const SQLDate date1 = p_left.GetAsSQLDate();
  const SQLDate date2 = p_right.GetAsSQLDate();
  SQLInterval result = date1 - date2;
  return SQLVariant(&result);
}

// TYPE == TIME

SQLVariant 
static SQL_OperTimeSubTime(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLTime() - p_right.GetAsSQLTime();
  return SQLVariant(&result);
}

// TYPE == TIMESTAMP

SQLVariant 
static SQL_OperStampSubStamp(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLTimestamp() - p_right.GetAsSQLTimestamp();
  return SQLVariant(&result);
}


// TYPE == INTERVAL_YEAR_MONTH

SQLVariant 
static SQL_OperIntYMSubIntYM(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() - p_right.GetAsSQLInterval();
  return SQLVariant(&result);
}

// TYPE == INTERVAL_DAY_SECOND

SQLVariant 
static SQL_OperDateSubIntDS(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLDate result = p_left.GetAsSQLDate();
  result = result - p_right.GetAsSQLInterval();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperTimeSubIntDS(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLTime result = p_left.GetAsSQLTime();
  result = result - p_right.GetAsSQLInterval();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperStampSubIntDS(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLTimestamp result = p_left.GetAsSQLTimestamp();
  result = result - p_right.GetAsSQLInterval();
  return SQLVariant(&result);
}

SQLVariant 
static SQL_OperIntDSSubIntDS(const SQLVariant& p_left,const SQLVariant& p_right)
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
SQLVariant::operator-(const SQLVariant& p_right)
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
  XString leftType  = SQLDataType::FindDatatype(m_datatype);
  XString rightType = SQLDataType::FindDatatype(p_right.m_datatype);
  XString error;
  error.Format(_T("Cannot do the subtract operator on (%s - %s)"),leftType.GetString(),rightType.GetString());
  throw StdException(error);
}

SQLVariant& 
SQLVariant::operator-=(const SQLVariant& p_right)
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
    OperatorCalculate function = OperatorSub[left][right].function;
    if(function)
    {
      *this = (*function)(*this,p_right);
      return *this;
    }
  }
  else
  {
    ThrowErrorOperator(SVO_AssignSubtract);
  }
  // No compare function found
  // Data types are not comparable
  XString leftType  = SQLDataType::FindDatatype(m_datatype);
  XString rightType = SQLDataType::FindDatatype(p_right.m_datatype);
  XString error;
  error.Format(_T("Cannot do the -= operator on (%s + %s)"),leftType.GetString(),rightType.GetString());
  throw StdException(error);
}

// End of namespace
}
