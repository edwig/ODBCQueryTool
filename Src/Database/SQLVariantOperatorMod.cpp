////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantOperatorMod.cpp
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
SQL_OperSShortModChar(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result %= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUShortModChar(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSLongModChar(SQLVariant& p_left,SQLVariant& p_right)
{
  long result = p_left.GetAsSLong();
  result %= p_right.GetAsSLong();
  return SQLVariant(result);
}
SQLVariant 
SQL_OperULongModChar(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned long result = p_left.GetAsULong();
  result %= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperFloatModChar(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = fmod(p_left.GetAsFloat(),p_right.GetAsFloat());
  return SQLVariant(result);
}

SQLVariant 
SQL_OperDoubleModChar(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = fmod(p_left.GetAsDouble(),p_right.GetAsDouble());
  return SQLVariant(result);
}

SQLVariant 
SQL_OperBitModChar(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSTinyModChar(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result %= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUTinyModChar(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result %= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSBigModChar(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result %= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUBigModChar(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt() % p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperNumModChar(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() % bcd(p_right.GetAsChar());
  SQLVariant var(&num);
  return var;
}

// TYPE == SSHORT

SQLVariant 
SQL_OperCharModSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result %= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSShortModSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result %= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUShortModSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant SQL_OperSLongModSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  long result = p_left.GetAsSLong();
  result %= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperULongModSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned long result = p_left.GetAsULong();
  result %= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperFloatModSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = fmod(p_left.GetAsFloat(),p_right.GetAsFloat());
  return SQLVariant(result);
}

SQLVariant 
SQL_OperDoubleModSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = fmod(p_left.GetAsDouble(),p_right.GetAsDouble());
  return SQLVariant(result);
}

SQLVariant 
SQL_OperBitModSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSTinyModSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result %= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUTinyModSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result %= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSBigModSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result %= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUBigModSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result %= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperNumModSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() % bcd(p_right.GetAsSShort());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
SQL_OperIntYMModSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
SQL_OperIntDSModSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}


// TYPE == USHORT

SQLVariant 
SQL_OperCharModUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSShortModUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUShortModUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}


SQLVariant 
SQL_OperSLongModUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  long result = p_left.GetAsSLong();
  result %= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUlongModUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned long result = p_left.GetAsULong();
  result %= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperFloatModUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = fmod(p_left.GetAsFloat(),p_right.GetAsFloat());
  return SQLVariant(result);
}

SQLVariant 
SQL_OperDoubleModUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = fmod(p_left.GetAsDouble(),p_right.GetAsDouble());
  return SQLVariant(result);
}

SQLVariant 
SQL_OperBitModUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSTinyModUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result %= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUTinyModUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result %= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSBigModUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result %= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUBigModUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result %= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperNumModUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() % bcd((int)p_right.GetAsUShort());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
SQL_OperIntYMModUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
SQL_OperIntDSModUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == SLONG

SQLVariant 
SQL_OperCharModSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  long result = p_left.GetAsSLong();
  result %= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSShortModSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result %= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUShortModSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSLongModSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  long result = p_left.GetAsSLong();
  result %= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperULongModSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned long result = p_left.GetAsULong();
  result %= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperFloatModSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = fmod(p_left.GetAsFloat(),p_right.GetAsFloat());
  return SQLVariant(result);
}

SQLVariant 
SQL_OperDoubleModSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = fmod(p_left.GetAsDouble(),p_right.GetAsDouble());
  return SQLVariant(result);
}

SQLVariant 
SQL_OperBitModSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSTinyModSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result %= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUTinyModSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result %= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSBigModSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result %= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUBigModSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result %= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperNumModSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() % bcd(p_right.GetAsSLong());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
SQL_OperIntYMModSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
SQL_OperIntDSModSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == ULONG

SQLVariant 
SQL_OperCharModULong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned long result = p_left.GetAsULong();
  result %= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSShortModULong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUShortModULong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSLongModULong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperULongModULong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned long result = p_left.GetAsULong();
  result %= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperFloatModULong(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = fmod(p_left.GetAsFloat(),p_right.GetAsFloat());
  return SQLVariant(result);
}

SQLVariant 
SQL_OperDoubleModULong(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = fmod(p_left.GetAsDouble(),p_right.GetAsDouble());
  return SQLVariant(result);
}

SQLVariant 
SQL_OperBitModULong(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSTinyModULong(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result %= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUTinyModULong(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result %= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSBigModULong(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result %= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUBigModULong(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result %= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperNumModULong(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() % bcd((int64)p_right.GetAsUShort());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
SQL_OperIntYMModULong(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
SQL_OperIntDSModULong(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == FLOAT

SQLVariant 
SQL_OperCharModFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = fmod(p_left.GetAsFloat(),p_right.GetAsFloat());
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSShortModFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result %= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUShortModFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSLongModFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  long result = p_left.GetAsSLong();
  result %= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperULongModFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned long result = p_left.GetAsULong();
  result %= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperFloatModFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = fmod(p_left.GetAsFloat(),p_right.GetAsFloat());
  return SQLVariant(result);
}

SQLVariant 
SQL_OperDoubleModFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = fmod(p_left.GetAsDouble(),p_right.GetAsDouble());
  return SQLVariant(result);
}

SQLVariant 
SQL_OperBitModFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSTinyModFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result %= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUTinyModFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result %= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSBigModFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result %= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUBigModFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result %= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperNumModFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() % bcd((double)p_right.GetAsFloat());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
SQL_OperIntYMModFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() % p_right.GetAsDouble();
  return SQLVariant(&result);
}

SQLVariant 
SQL_OperIntDSModFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() % p_right.GetAsDouble();
  return SQLVariant(&result);
}

// TYPE == DOUBLE

SQLVariant 
SQL_OperCharModDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = fmod(p_left.GetAsDouble(),p_right.GetAsDouble());
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSShortModDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsUShort();
  result %= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUShortModDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSLongModDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  long result = p_left.GetAsSLong();
  result %= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperULongModDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned long result = p_left.GetAsULong();
  result %= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperFloatModDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = fmod(p_left.GetAsFloat(),p_right.GetAsFloat());
  return SQLVariant(result);
}

SQLVariant 
SQL_OperDoubleModDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = fmod(p_left.GetAsDouble(),p_right.GetAsDouble());
  return SQLVariant(result);
}

SQLVariant 
SQL_OperBitModDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSTinyModDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result %= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUTinyModDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result %= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSBigModDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result %= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUBigModDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result %= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperNumModDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() % bcd(p_right.GetAsDouble());
  SQLVariant var(&num);
  return var;
}

SQLVariant
SQL_OperIntYMModDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() % p_right.GetAsDouble();
  return SQLVariant(&result);
}

SQLVariant
SQL_OperIntDSModDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() % p_right.GetAsDouble();
  return SQLVariant(&result);
}


// TYPE == BIT

SQLVariant 
SQL_OperCharModBit(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  bool result = p_left.GetAsBit() != 0;
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSShortModBit(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  short result = p_left.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUShortModBit(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  unsigned short result = p_left.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSLongModBit(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  long result = p_left.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperULongModBit(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  unsigned long result = p_left.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperFloatModBit(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  float result = p_left.GetAsFloat();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperDoubleModBit(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  double result = p_left.GetAsDouble();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperBitModBit(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  bool result = p_left.GetAsBit() != 0;
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSTinyModBit(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  char result = p_left.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUTinyModBit(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  unsigned char result = p_left.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSBigModBit(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUBigModBit(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperNumModBit(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  SQLVariant result(p_left);
  return result;
}

SQLVariant 
SQL_OperIntYMModBit(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  SQLInterval result = p_left.GetAsSQLInterval();
  return SQLVariant(&result);
}

SQLVariant 
SQL_OperIntDSModBit(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  SQLInterval result = p_left.GetAsSQLInterval();
  return SQLVariant(&result);
}

// TYPE == STINYINT

SQLVariant 
SQL_OperCharModSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result %= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSShortModSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result %= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUShortModSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSLongModSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  long result = p_left.GetAsSLong();
  result %= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperULongModSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned long result = p_left.GetAsULong();
  result %= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperFloatModSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = fmod(p_left.GetAsFloat(),p_right.GetAsFloat());
  return SQLVariant(result);
}

SQLVariant 
SQL_OperDoubleModSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = fmod(p_left.GetAsDouble(),p_right.GetAsDouble());
  return SQLVariant(result);
}

SQLVariant 
SQL_OperBitModSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSTinyModSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result %= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUTinyModSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result %= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSBigModSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result %= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUBigModSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result %= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperNumModSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() % bcd(p_right.GetAsSTinyInt());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
SQL_OperIntYMModSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
SQL_OperIntDSModSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE = UTINYINT

SQLVariant 
SQL_OperCharModUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result %= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSShortModUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result %= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUShortModUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSLongModUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  long result = p_left.GetAsSLong();
  result %= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperULongModUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned long result = p_left.GetAsULong();
  result %= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperFloatModUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = fmod(p_left.GetAsFloat(),p_right.GetAsFloat());
  return SQLVariant(result);
}

SQLVariant 
SQL_OperDoubleModUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = fmod(p_left.GetAsDouble(),p_right.GetAsDouble());
  return SQLVariant(result);
}

SQLVariant 
SQL_OperBitModUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSTinyModUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result %= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUTinyModUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result %= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSBigModUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result %= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUBigModUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result %= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperNumModUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() % bcd((int)p_right.GetAsUTinyInt());
  SQLVariant var(&num);
  return var;
}

SQLVariant
SQL_OperIntYMModUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant
SQL_OperIntDSModUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}


// TYPE == SBIGINT

SQLVariant 
SQL_OperCharModSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result %= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSShortModSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result %= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUShortModSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSLongModSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  long result = p_left.GetAsSLong();
  result %= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperULongModSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned long result = p_left.GetAsULong();
  result %= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperFloatModSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = fmod(p_left.GetAsFloat(),p_right.GetAsFloat());
  return SQLVariant(result);
}

SQLVariant 
SQL_OperDoubleModSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = fmod(p_left.GetAsDouble(),p_right.GetAsDouble());
  return SQLVariant(result);
}

SQLVariant 
SQL_OperBitModSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSTinyModSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result %= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUTinyModSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result %= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSBigModSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result %= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUBigModSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result %= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperNumModSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() % bcd(p_right.GetAsSBigInt());
  SQLVariant var(&num);
  return var;
}

SQLVariant 
SQL_OperIntYMModSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant 
SQL_OperIntDSModSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == UBIGINT

SQLVariant 
SQL_OperCharModUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result %= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSShortModUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  short result = p_left.GetAsSShort();
  result %= p_right.GetAsSShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUShortModUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short result = p_left.GetAsUShort();
  result %= p_right.GetAsUShort();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSLongModUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  long result = p_left.GetAsSLong();
  result %= p_right.GetAsSLong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperULongModUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned long result = p_left.GetAsULong();
  result %= p_right.GetAsULong();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperFloatModUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  float result = fmod(p_left.GetAsFloat(),p_right.GetAsFloat());
  return SQLVariant(result);
}

SQLVariant 
SQL_OperDoubleModUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  double result = fmod(p_left.GetAsDouble(),p_right.GetAsDouble());
  return SQLVariant(result);
}

SQLVariant 
SQL_OperBitModUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  bool result = false;
  if(p_left.GetAsBit() && p_right.GetAsBit())
  {
    result = true;
  }
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSTinyModUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  char result = p_left.GetAsSTinyInt();
  result %= p_right.GetAsSTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUTinyModUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char result = p_left.GetAsUTinyInt();
  result %= p_right.GetAsUTinyInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperSBigModUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLBIGINT result = p_left.GetAsSBigInt();
  result %= p_right.GetAsSBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperUBigModUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLUBIGINT result = p_left.GetAsUBigInt();
  result %= p_right.GetAsUBigInt();
  return SQLVariant(result);
}

SQLVariant 
SQL_OperNumModUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() % bcd(p_right.GetAsUBigInt());
  SQLVariant var(&num);
  return var;
}

SQLVariant
SQL_OperIntYMModUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}

SQLVariant
SQL_OperIntDSModUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() % p_right.GetAsSLong();
  return SQLVariant(&result);
}

// TYPE == NUMERIC

SQLVariant 
SQL_OperCharModNum(SQLVariant& p_left,SQLVariant& p_right)
{
  double num = fmod(p_left.GetAsDouble(),p_right.GetAsBCD().AsDouble());
  CString str;
  str.Format("%lf",num);
  SQLVariant var(str);
  return var;
}

SQLVariant 
SQL_OperSShortModNum(SQLVariant& p_left,SQLVariant& p_right)
{
  short num = p_left.GetAsSShort() % p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
SQL_OperUShortModNum(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned short num = p_left.GetAsUShort() % p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
SQL_OperSLongModNum(SQLVariant& p_left,SQLVariant& p_right)
{
  long num = p_left.GetAsSLong() % p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
SQL_OperULongModNum(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned long num = p_left.GetAsULong() % p_right.GetAsBCD().AsInt64();
  SQLVariant var(num);
  return var;
}

SQLVariant 
SQL_OperFloatModNum(SQLVariant& p_left,SQLVariant& p_right)
{
  float num = (float) fmod(p_left.GetAsFloat(),p_right.GetAsBCD().AsDouble());
  SQLVariant var(num);
  return var;
}

SQLVariant 
SQL_OperDoubleModNum(SQLVariant& p_left,SQLVariant& p_right)
{
  double num = fmod(p_left.GetAsDouble(),p_right.GetAsBCD().AsDouble());
  SQLVariant var(num);
  return var;
}

SQLVariant 
SQL_OperBitModNum(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  SQLVariant var(p_left.GetAsBit());
  return var;
}

SQLVariant 
SQL_OperSTinyModNum(SQLVariant& p_left,SQLVariant& p_right)
{
  char num = p_left.GetAsSTinyInt() % (char) p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
SQL_OperUTinyModNum(SQLVariant& p_left,SQLVariant& p_right)
{
  unsigned char num = p_left.GetAsUTinyInt() % (unsigned char)p_right.GetAsBCD().AsLong();
  SQLVariant var(num);
  return var;
}

SQLVariant 
SQL_OperSBigModNum(SQLVariant& p_left,SQLVariant& p_right)
{
  int64 num = p_left.GetAsSBigInt() % p_right.GetAsBCD().AsInt64();
  SQLVariant var(num);
  return var;
}

SQLVariant 
SQL_OperUBigModNum(SQLVariant& p_left,SQLVariant& p_right)
{
  uint64 num = p_left.GetAsUBigInt() % p_right.GetAsBCD().AsUInt64();
  SQLVariant var(num);
  return var;
}

SQLVariant 
SQL_OperNumModNum(SQLVariant& p_left,SQLVariant& p_right)
{
  bcd num = p_left.GetAsBCD() % p_right.GetAsBCD();
  SQLVariant var(&num);
  return var;
}

SQLVariant 
SQL_OperIntYMModNum(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLInterval result = p_left.GetAsSQLInterval() % p_right.GetAsDouble();
  return SQLVariant(&result);
}

SQLVariant 
SQL_OperIntDSModNum(SQLVariant& p_left,SQLVariant& p_right)
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
SQLVariant::operator%(SQLVariant& p_right)
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
    throw CString("Cannot do a modulo by zero/empty");
  }

  // Find our comparison function
  OperatorCalculate function = OperatorMod[left][right].function;
  if(function)
  {
    return (*function)(*this,p_right);
  }
  // No compare function found
  // Data types are not comparable
  CString leftType  = FindDatatype(m_datatype);
  CString rightType = FindDatatype(p_right.m_datatype);
  CString error;
  error.Format("Cannot do the modulo operator on (%s % %s)",leftType,rightType);
  throw error;
}

