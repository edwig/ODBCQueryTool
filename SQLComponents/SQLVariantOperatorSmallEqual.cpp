////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantOperatorSmallEQ.cpp
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
#include "SQLVariantTrim.h"
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

//////////////////////////////////////////////////////////////////////////
//
// ALL EQUALITY OPERATOR FUNCTIONS
//
//////////////////////////////////////////////////////////////////////////

bool
static SQL_OperVarSmallEQChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  XString leftString,rightString;
  p_left .GetAsString(leftString);
  p_right.GetAsString(rightString);

  p_left .GetAsString(leftString);
  p_right.GetAsString(rightString);

  if(p_left.IsDecimalType())
  {
    leftString = leftString.TrimRight('0');
    leftString = leftString.TrimRight('.');
  }
  if(p_right.IsDecimalType())
  {
    rightString = rightString.TrimRight('0');
    rightString = rightString.TrimRight('.');
  }

  return leftString.Compare(rightString) <= 0;
}

bool
static SQL_OperGuidSmallEQChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  XString leftString,rightString;
  p_left .GetAsString(leftString);
  p_right.GetAsString(rightString);

  leftString. TrimLeft ('{');
  leftString. TrimRight('}');
  rightString.TrimLeft ('}');
  rightString.TrimRight('{');

  return leftString.Compare(rightString) <= 0;
}

// SIGNED SHORT

bool
static SQL_OperSShortSmallEQSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSShort() <= p_right.GetAsSShort();
}

bool
static SQL_OperUShortSmallEQSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUShort() <= SQL_ShortToUShort(p_right.GetAsSShort());
}

bool
static SQL_OperSLongSmallEQSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_SLongToShort(p_left.GetAsSLong()) <= p_right.GetAsSShort();
}

bool
static SQL_OperULongSmallEQSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_ULongToUShort(p_left.GetAsULong()) <= SQL_ShortToUShort(p_right.GetAsSShort());
}

bool
static SQL_OperFloatSmallEQSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_FloatToShort(p_left.GetAsFloat()) <= p_right.GetAsSShort();
}

bool
static SQL_OperDoubleSmallEQSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_DoubleToShort(p_left.GetAsDouble()) <= p_right.GetAsSShort();
}

bool
static SQL_OperBitSmallEQSShort(const SQLVariant& /*p_left*/,const SQLVariant& p_right)
{
  return p_right.GetAsSShort() != 0;
}

bool   
static SQL_OperSTinySmallEQSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() <= SQL_ShortToTinyInt(p_right.GetAsSShort());
}

bool
static SQL_OperUTinySmallEQSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() <= SQL_ShortToUTinyInt(p_right.GetAsSShort());
}

bool
static SQL_OperSBigSmallEQSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_BIGINTToShort(p_left.GetAsSBigInt()) <= p_right.GetAsSShort();
}

bool
static SQL_OperUBigSmallEQSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_UBIGINTToShort(p_left.GetAsUBigInt()) <= p_right.GetAsSShort();
}

bool
static SQL_OperNumSmallEQSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_SLongToShort(p_left.GetAsBCD().AsLong()) <= p_right.GetAsSShort();
}

// UNSIGNED SHORT

bool   
static SQL_OperSShortSmallEQUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_ShortToUShort(p_left.GetAsSShort()) <= p_right.GetAsUShort();
}

bool   
static SQL_OperUShortSmallEQUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUShort() <= p_right.GetAsUShort();
}

bool
static SQL_OperSLongSmallEQUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_SLongToUShort(p_left.GetAsSLong()) <= p_right.GetAsUShort();
}

bool
static SQL_OperUlongSmallEQUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_ULongToUShort(p_left.GetAsULong()) <= p_right.GetAsUShort();
}

bool
static SQL_OperFloatSmallEQUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_FloatToUShort(p_left.GetAsFloat()) <= p_right.GetAsUShort();
}

bool
static SQL_OperDoubleSmallEQUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_DoubleToUShort(p_left.GetAsDouble()) <= p_right.GetAsUShort();
}

bool
static SQL_OperBitSmallEQUShort(const SQLVariant& /*p_left*/,const SQLVariant& p_right)
{
  return p_right.GetAsUShort() != 0;
}

bool
static SQL_OperSTinySmallEQUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() <= SQL_UShortToTinyInt(p_right.GetAsUShort());
}

bool
static SQL_OperUTinySmallEQUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() <= SQL_UShortToUTinyInt(p_right.GetAsUShort());
}

bool
static SQL_OperSBigSmallEQUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_BIGINTToUShort(p_left.GetAsSBigInt()) <= p_right.GetAsUShort();
}

bool
static SQL_OperUBigSmallEQUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_UBIGINTToUShort(p_left.GetAsUBigInt()) <= p_right.GetAsUShort();
}

bool   
static SQL_OperNumSmallEQUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_SLongToUShort(p_left.GetAsBCD().AsLong()) <= p_right.GetAsUShort();
}

// SIGNED LONG

bool   
static SQL_OperSShortSmallEQSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSShort() <= SQL_SLongToShort(p_right.GetAsSLong());
}

bool
static SQL_OperUShortSmallEQSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUShort() <= SQL_SLongToUShort(p_right.GetAsSLong());
}

bool
static SQL_OperSLongSmallEQSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSLong() <= p_right.GetAsSLong();
}

bool
static SQL_OperULongSmallEQSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsULong() <= SQL_LongToULong(p_right.GetAsSLong());
}

bool
static SQL_OperFloatSmallEQSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_FloatToLong(p_left.GetAsFloat()) <= p_right.GetAsSLong();
}

bool
static SQL_OperDoubleESmallEQSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_DoubleToLong(p_left.GetAsDouble()) <= p_right.GetAsSLong();
}

bool
static SQL_OperBitSmallEQSLong(const SQLVariant& /*p_left*/,const SQLVariant& p_right)
{
  return p_right.GetAsSLong() != 0;
}

bool
static SQL_OperSTinySmallEQSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() <= SQL_SLongToTinyInt(p_right.GetAsSLong());
}

bool
static SQL_OperUTinySmallEQSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() <= SQL_SLongToUTinyInt(p_right.GetAsSLong());
}

bool
static SQL_OperSBigSmallEQSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_BIGINTToLong(p_left.GetAsSBigInt()) <= p_right.GetAsSLong();
}

bool
static SQL_OperUBigSmallEQSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_UBIGINTToLong(p_left.GetAsUBigInt()) <= p_right.GetAsSLong();
}

bool
static SQL_OperNumSmallEQSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsBCD().AsLong() <= p_right.GetAsSLong();
}

// UNSIGNED LONG

bool   
static SQL_OperSShortSmallEQULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_ShortToUShort(p_left.GetAsSShort()) <= SQL_ULongToUShort(p_right.GetAsULong());
}

bool
static SQL_OperUShortSmallEQULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUShort() <= SQL_ULongToUShort(p_right.GetAsULong());
}

bool
static SQL_OperSLongSmallEQULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_LongToULong(p_left.GetAsSLong()) <= p_right.GetAsULong();
}

bool
static SQL_OperULongSmallEQULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsULong() <= p_right.GetAsULong();
}

bool
static SQL_OperFloatSmallEQULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_FloatToULong(p_left.GetAsFloat()) <= p_right.GetAsULong();
}

bool
static SQL_OperDoubleSmallEQULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_DoubleToULong(p_left.GetAsDouble()) <= p_right.GetAsULong();
}

bool
static SQL_OperBitSmallEQULong(const SQLVariant& /*p_left*/,const SQLVariant& p_right)
{
  return p_right.GetAsULong() != 0;
}

bool
static SQL_OperSTinySmallEQULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_TinyIntToUTinyInt(p_left.GetAsSTinyInt()) <= SQL_ULongToUTinyInt(p_right.GetAsULong());
}

bool
static SQL_OperUTinySmallEQULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() <= SQL_ULongToUTinyInt(p_right.GetAsULong());
}

bool
static SQL_OperSBigSmallEQULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_BIGINTToULong(p_left.GetAsSBigInt()) <= p_right.GetAsULong();
}

bool
static SQL_OperUBigSmallEQULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_UBIGINTToULong(p_left.GetAsUBigInt()) <= p_right.GetAsULong();
}

bool
static SQL_OperNumSmallEQULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_BIGINTToULong(p_left.GetAsBCD().AsInt64()) <= p_right.GetAsULong();
}

// FLOAT

bool   
static SQL_OperSShortSmallEQFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSShort() <= SQL_FloatToShort(p_right.GetAsFloat());
}

bool   
static SQL_OperUShortSmallEQFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUShort() <= SQL_FloatToUShort(p_right.GetAsFloat());
}

bool
static SQL_OperSLongSmallEQFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSLong() <= SQL_FloatToLong(p_right.GetAsFloat());
}

bool
static SQL_OperULongSmallEQFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsULong() <= SQL_FloatToULong(p_right.GetAsFloat());
}

bool
static SQL_OperFloatSmallEQFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsFloat() <= p_right.GetAsFloat();
}

bool
static SQL_OperDoubleSmallEQFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_DoubleToFloat(p_left.GetAsDouble()) <= p_right.GetAsFloat();
}

bool
static SQL_OperBitSmallEQFloat(const SQLVariant& /*p_left*/,const SQLVariant& p_right)
{
  return p_right.GetAsFloat() < -FLT_EPSILON || FLT_EPSILON < p_right.GetAsFloat();
}

bool
static SQL_OperSTinySmallEQFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() <= SQL_FloatToTinyInt(p_right.GetAsFloat());
}

bool
static SQL_OperUTinySmallEQFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() <= SQL_FloatToUTinyInt(p_right.GetAsFloat());
}

bool
static SQL_OperSBigSmallEQFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSBigInt() <= SQL_FloatToBIGINT(p_right.GetAsFloat());
}

bool
static SQL_OperUBigSmallEQFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUBigInt() <= SQL_FloatToUBIGINT(p_right.GetAsFloat());
}

bool
static SQL_OperNumSmallEQFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsBCD().AsDouble() <= (double) p_right.GetAsFloat();
}

// DOUBLE

bool
static SQL_OperSShortSmallEQDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSShort() <= SQL_DoubleToShort(p_right.GetAsDouble());
}

bool
static SQL_OperUShortSmallEQDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUShort() <= SQL_DoubleToUShort(p_right.GetAsDouble());
}

bool
static SQL_OperSLongSmallEQDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSLong() <= SQL_DoubleToLong(p_right.GetAsDouble());
}

bool
static SQL_OperULongSmallEQDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsULong() <= SQL_DoubleToULong(p_right.GetAsDouble());
}

bool
static SQL_OperFloatSmallEQDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsFloat() <= SQL_DoubleToFloat(p_right.GetAsDouble());
}

bool
static SQL_OperDoubleSmallEQDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsDouble() <= p_right.GetAsDouble();
}

bool
static SQL_OperBitSmallEQDouble(const SQLVariant& /*p_left*/,const SQLVariant& p_right)
{
  return p_right.GetAsDouble() < -DBL_EPSILON || DBL_EPSILON < p_right.GetAsDouble();
}

bool
static SQL_OperSTinySmallEQDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() <= SQL_DoubleToTinyInt(p_right.GetAsDouble());
}

bool
static SQL_OperUTinySmallEQDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() <= SQL_DoubleToUTinyInt(p_right.GetAsDouble());
}

bool
static SQL_OperSBigSmallEQDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSBigInt() <= SQL_DoubleToBIGINT(p_right.GetAsDouble());
}

bool
static SQL_OperUBigSmallEQDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUBigInt() <= SQL_DoubleToUBIGINT(p_right.GetAsDouble());
}

bool
static SQL_OperNumSmallEQDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsBCD().AsDouble() <= p_right.GetAsDouble();
}

// BIT

bool   
static SQL_OperSShortSmallEQBit(const SQLVariant& /*p_left*/,const SQLVariant& p_right)
{
  return p_right.GetAsBit() != 0;
}

bool
static SQL_OperUShortSmallEQBit(const SQLVariant& /*p_left*/,const SQLVariant& p_right)
{
  return p_right.GetAsBit() != 0;
}

bool   
static SQL_OperSLongSmallEQBit(const SQLVariant& /*p_left*/,const SQLVariant& p_right)
{
  return p_right.GetAsBit() != 0;
}

bool
static SQL_OperULongSmallEQBit(const SQLVariant& /*p_left*/,const SQLVariant& p_right)
{
  return p_right.GetAsBit() != 0;
}

bool
static SQL_OperFloatSmallEQBit(const SQLVariant& /*p_left*/,const SQLVariant& p_right)
{
  return p_right.GetAsBit() != 0;
}

bool
static SQL_OperDoubleSmallEQBit(const SQLVariant& /*p_left*/,const SQLVariant& p_right)
{
  return p_right.GetAsBit() != 0;
}

bool
static SQL_OperBitSmallEQBit(const SQLVariant& /*p_left*/,const SQLVariant& p_right)
{
  return p_right.GetAsBit() != 0;
}

bool
static SQL_OperSTinySmallEQBit(const SQLVariant& /*p_left*/,const SQLVariant& p_right)
{
  return p_right.GetAsBit() != 0;
}

bool
static SQL_OperUTinySmallEQBit(const SQLVariant& /*p_left*/,const SQLVariant& p_right)
{
  return p_right.GetAsBit() != 0;
}

bool
static SQL_OperSBigSmallEQBit(const SQLVariant& /*p_left*/,const SQLVariant& p_right)
{
  return p_right.GetAsBit() != 0;
}

bool
static SQL_OperUBigSmallEQBit(const SQLVariant& /*p_left*/,const SQLVariant& p_right)
{
  return p_right.GetAsBit() != 0;
}

bool
static SQL_OperNumSmallEQBit(const SQLVariant& /*p_left*/,const SQLVariant& p_right)
{
  return p_right.GetAsBit() != 0;
}

// SIGNED TINYINT

bool
static SQL_OperSShortSmallEQSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_ShortToTinyInt(p_left.GetAsSShort()) <= p_right.GetAsSTinyInt();
}

bool
static SQL_OperUShortSmallEQSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_UShortToTinyInt(p_left.GetAsUShort()) <= p_right.GetAsSTinyInt();
}

bool
static SQL_OperSLongSmallEQSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_SLongToTinyInt(p_left.GetAsSLong()) <= p_right.GetAsSTinyInt();
}

bool
static SQL_OperULongSmallEQSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_ULongToTinyInt(p_left.GetAsULong()) <= p_right.GetAsSTinyInt();
}

bool
static SQL_OperFloatSmallEQSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_FloatToTinyInt(p_left.GetAsFloat()) <= p_right.GetAsSTinyInt();
}

bool
static SQL_OperDoubleSmallEQSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_DoubleToTinyInt(p_left.GetAsDouble()) <= p_right.GetAsSTinyInt();
}

bool
static SQL_OperBitSmallEQSTiny(const SQLVariant& /*p_left*/,const SQLVariant& p_right)
{
  return p_right.GetAsSTinyInt() != 0;
}

bool
static SQL_OperSTinySmallEQSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() <= p_right.GetAsSTinyInt();
}

bool
static SQL_OperUTinySmallEQSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() <= SQL_TinyIntToUTinyInt(p_right.GetAsSTinyInt());
}

bool
static SQL_OperSBigSmallEQSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_SBIGINTToTinyInt(p_left.GetAsSBigInt()) <= p_right.GetAsSTinyInt();
}

bool
static SQL_OperUBigSmallEQSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_UBIGINTToTinyInt(p_left.GetAsUBigInt()) <= p_right.GetAsSTinyInt();
}

bool
static SQL_OperNumSmallEQSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_SLongToTinyInt(p_left.GetAsBCD().AsLong()) <= p_right.GetAsSTinyInt();
}

// UNSIGNED TINYINT

bool
static SQL_OperSShortSmallEQUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_ShortToUTinyInt(p_left.GetAsSShort()) <= p_right.GetAsUTinyInt();
}

bool
static SQL_OperUShortSmallEQUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_UShortToUTinyInt(p_left.GetAsUShort()) <= p_right.GetAsUTinyInt();
}

bool
static SQL_OperSLongSmallEQUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_SLongToUTinyInt(p_left.GetAsSLong()) <= p_right.GetAsUTinyInt();
}

bool
static SQL_OperULongSmallEQUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_ULongToUTinyInt(p_left.GetAsULong()) <= p_right.GetAsUTinyInt();
}

bool
static SQL_OperFloatSmallEQUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_FloatToUTinyInt(p_left.GetAsFloat()) <= p_right.GetAsUTinyInt();
}

bool
static SQL_OperDoubleSmallEQUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_DoubleToUTinyInt(p_left.GetAsDouble()) <= p_right.GetAsUTinyInt();
}

bool
static SQL_OperBitSmallEQUTiny(const SQLVariant& /*p_left*/,const SQLVariant& p_right)
{
  return p_right.GetAsUTinyInt() != 0;
}

bool
static SQL_OperSTinySmallEQUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_TinyIntToUTinyInt(p_left.GetAsSTinyInt()) <= p_right.GetAsUTinyInt();
}

bool
static SQL_OperUTinySmallEQUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() <= p_right.GetAsUTinyInt();
}

bool
static SQL_OperSBigSmallEQUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_SBIGINTToUTinyInt(p_left.GetAsSBigInt()) <= p_right.GetAsUTinyInt();
}

bool
static SQL_OperUBigSmallEQUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_UBIGINTToUTinyInt(p_left.GetAsUBigInt()) <= p_right.GetAsUTinyInt();
}

bool
static SQL_OperNumSmallEQUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_SLongToUTinyInt(p_left.GetAsBCD().AsLong()) <= p_right.GetAsUTinyInt();
}

// SIGNED BIGINT

bool
static SQL_OperSShortSmallEQSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSShort() <= SQL_BIGINTToShort(p_right.GetAsSBigInt());
}

bool
static SQL_OperUShortSmallEQSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUShort() <= SQL_BIGINTToUShort(p_right.GetAsSBigInt());
}

bool
static SQL_OperSLongSmallEQSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSLong() <= SQL_BIGINTToLong(p_right.GetAsSBigInt());
}

bool
static SQL_OperULongSmallEQSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsULong() <= SQL_BIGINTToULong(p_right.GetAsSBigInt());
}

bool
static SQL_OperFloatSmallEQSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsFloat() <= SQL_BIGINTToFloat(p_right.GetAsSBigInt());
}

bool
static SQL_OperDoubleSmallEQSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsDouble() <= SQL_BIGINTToDouble(p_right.GetAsSBigInt());
}

bool
static SQL_OperBitSmallEQSBig(const SQLVariant& /*p_left*/,const SQLVariant& p_right)
{
  return p_right.GetAsSBigInt() != 0;
}

bool
static SQL_OperSTinySmallEQSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() <= SQL_SBIGINTToTinyInt(p_right.GetAsSBigInt());
}

bool
static SQL_OperUTinySmallEQSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() <= SQL_SBIGINTToUTinyInt(p_right.GetAsSBigInt());
}

bool
static SQL_OperSBigSmallEQSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSBigInt() <= p_right.GetAsSBigInt();
}

bool
static SQL_OperUBigSmallEQSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUBigInt() <= SQL_SBIGINTToUBIGINT(p_right.GetAsSBigInt());
}

bool   
static SQL_OperNumSmallEQSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsBCD().AsInt64() <= p_right.GetAsSBigInt();
}

// UNSIGNED BIGINT

bool
static SQL_OperSShortSmallEQUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSShort() <= SQL_UBIGINTToShort(p_right.GetAsUBigInt());
}

bool
static SQL_OperUShortSmallEQUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUShort() <= SQL_UBIGINTToUShort(p_right.GetAsUBigInt());
}

bool
static SQL_OperSLongSmallEQUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSLong() <= SQL_UBIGINTToLong(p_right.GetAsUBigInt());
}

bool
static SQL_OperULongSmallEQUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsULong() <= SQL_UBIGINTToULong(p_right.GetAsUBigInt());
}

bool
static SQL_OperFloatSmallEQUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsFloat() <= SQL_UBIGINTToFloat(p_right.GetAsUBigInt());
}

bool
static SQL_OperDoubleSmallEQUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsDouble() <= SQL_UBIGINTToDouble(p_right.GetAsUBigInt());
}

bool
static SQL_OperBitSmallEQUBig(const SQLVariant& /*p_left*/,const SQLVariant& p_right)
{
  return p_right.GetAsUBigInt() != 0;
}

bool
static SQL_OperSTinySmallEQUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() <= SQL_UBIGINTToUTinyInt(p_right.GetAsUBigInt());
}

bool
static SQL_OperUTinySmallEQUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() <= SQL_UBIGINTToUTinyInt(p_right.GetAsUBigInt());
}

bool
static SQL_OperSBigSmallEQUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_SBIGINTToUBIGINT(p_left.GetAsSBigInt()) <= p_right.GetAsUBigInt();
}

bool
static SQL_OperUBigSmallEQUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUBigInt() <= p_right.GetAsUBigInt();
}

bool
static SQL_OperNumSmallEQUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsBCD().AsUInt64() <= p_right.GetAsUBigInt();
}

// NUMERIC

bool
static SQL_OperSShortSmallEQNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSShort() <= SQL_SLongToShort(p_right.GetAsBCD().AsLong());
}

bool
static SQL_OperUShortSmallEQNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUShort() <= SQL_SLongToUShort(p_right.GetAsBCD().AsLong());
}

bool
static SQL_OperSLongSmallEQNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSLong() <= p_right.GetAsBCD().AsLong();
}

bool
static SQL_OperULongSmallEQNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsULong() <= SQL_BIGINTToULong(p_right.GetAsBCD().AsInt64());
}

bool
static SQL_OperFloatSmallEQNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsFloat() <= (float) p_right.GetAsBCD().AsDouble();
}

bool
static SQL_OperDoubleSmallEQNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsDouble() <= p_right.GetAsBCD().AsDouble();
}

bool
static SQL_OperBitSmallEQNum(const SQLVariant& /*p_left*/,const SQLVariant& p_right)
{
  return !p_right.GetAsBCD().IsZero();
}

bool
static SQL_OperSTinySmallEQNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() <= SQL_SLongToTinyInt(p_right.GetAsBCD().AsLong());
}

bool
static SQL_OperUTinySmallEQNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() <= SQL_SLongToUTinyInt(p_right.GetAsBCD().AsLong());
}

bool
static SQL_OperSBigSmallEQNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSBigInt() <= p_right.GetAsBCD().AsInt64();
}

bool
static SQL_OperUBigSmallEQNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUBigInt() <= p_right.GetAsBCD().AsUInt64();
}

bool
static SQL_OperNumSmallEQNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsBCD() <= p_right.GetAsBCD();
}

// TYPE == GUID

bool
static SQL_OperGuidSmallEQGuid(const SQLVariant& p_left,const SQLVariant& p_right)
{
  const SQLGUID* left  = p_left.GetAsGUID();
  const SQLGUID* right = p_right.GetAsGUID();

  return memcmp(left,right,sizeof(SQLGUID)) <= 0;
}

// TYPE == BINARY

bool
static SQL_OperBinarySmallEQBinary(const SQLVariant& p_left,const SQLVariant& p_right)
{
  void* left   = p_left.GetAsBinary();
  void* right  = p_right.GetAsBinary();
  // Take longest of two binaries in comparison
  long  length = p_left.GetBinaryLength() < p_right.GetBinaryLength() ? p_left.GetBinaryLength() : p_right.GetBinaryLength();

  return memcmp(left,right,length) <= 0;
}

// TYPE == DATE

bool
static SQL_OperDateSmallEQDate(const SQLVariant& p_left,const SQLVariant& p_right)
{
  // Relies on the < operator of SQLDate
  return p_left.GetAsSQLDate() <= p_right.GetAsSQLDate();
}

bool
static SQL_OperStampSmallEQDate(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLDate date = p_left.GetAsSQLTimestamp().AsSQLDate();
  return date <= p_right.GetAsSQLDate();
}

// TYPE == TIME

bool
static SQL_OperTimeSmallEQTime(const SQLVariant& p_left,const SQLVariant& p_right)
{
  // Relies on the < operator of SQLTime
  return p_left.GetAsSQLTime() <= p_right.GetAsSQLTime();
}

bool
static SQL_OperStampSmallEQTime(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLTime time = p_left.GetAsSQLTimestamp().AsSQLTime();
  return time <= p_right.GetAsSQLTime();
}

// TYPE == TIMESTAMP

bool
static SQL_OperStampSmallEQStamp(const SQLVariant& p_left,const SQLVariant& p_right)
{
  // Relies on the < operator of SQLTimestamp
  return p_left.GetAsSQLTimestamp() <= p_right.GetAsSQLTimestamp();
}

// TYPE == INTERVAL_YEAR_MONTH

bool
static SQL_OperIntYMSmallEQIntYM(const SQLVariant& p_left,const SQLVariant& p_right)
{
  // Relies on the calculation of months in SQLInterval
  return p_left.GetAsSQLInterval().AsDatabaseNumber() <= p_right.GetAsSQLInterval().AsDatabaseNumber();
}

// TYPE == INTERVAL_DAY_SECOND

bool
static SQL_OperIntDSSmallEQIntDS(const SQLVariant& p_left,const SQLVariant& p_right)
{
  // Relies on the calculation of seconds in SQLInterval
  return p_left.GetAsSQLInterval().AsDatabaseNumber() <= p_right.GetAsSQLInterval().AsDatabaseNumber();
}

// OPERATOR ARRAY

static CompareFunctionArray OperatorSmallerEqual[CT_LAST][CT_LAST] =
{
                         // CT_CHAR                  CT_SSHORT                    CT_USHORT                    CT_SLONG                     CT_ULONG                    CT_FLOAT                    CT_DOUBLE                    CT_BIT                    CT_STINYINT                 CT_UTINYINT                 CT_SBIGINT                 CT_UBIGINT                 CT_NUMERIC                CT_GUID                  CT_BINARY                    CT_DATE                   CT_TIME                   CT_TIMESTAMP               CT_INTERVAL_YM             CT_INTERVAL_DS
                         // ------------------------ ---------------------------- ---------------------------- ---------------------------- --------------------------- --------------------------- ---------------------------- ------------------------- --------------------------- --------------------------- -------------------------- -------------------------- ------------------------- ------------------------ ---------------------------- ------------------------- ------------------------- -------------------------- -------------------------- ----------------------------
  /* CT_CHAR        */   {  &SQL_OperVarSmallEQChar ,&SQL_OperVarSmallEQChar     ,&SQL_OperVarSmallEQChar     ,&SQL_OperVarSmallEQChar     ,&SQL_OperVarSmallEQChar    ,&SQL_OperVarSmallEQChar    ,&SQL_OperVarSmallEQChar     ,&SQL_OperVarSmallEQChar  ,&SQL_OperVarSmallEQChar    ,&SQL_OperVarSmallEQChar    ,&SQL_OperVarSmallEQChar   ,&SQL_OperVarSmallEQChar   ,&SQL_OperVarSmallEQChar  ,&SQL_OperVarSmallEQChar ,nullptr                     ,&SQL_OperVarSmallEQChar  ,&SQL_OperVarSmallEQChar  ,&SQL_OperVarSmallEQChar   ,&SQL_OperVarSmallEQChar   ,&SQL_OperVarSmallEQChar    }
  /* CT_SSHORT      */  ,{  &SQL_OperVarSmallEQChar ,&SQL_OperSShortSmallEQSShort,&SQL_OperSShortSmallEQUShort,&SQL_OperSShortSmallEQSLong ,&SQL_OperSShortSmallEQULong,&SQL_OperSShortSmallEQFloat,&SQL_OperSShortSmallEQDouble,&SQL_OperSShortSmallEQBit,&SQL_OperSShortSmallEQSTiny,&SQL_OperSShortSmallEQUTiny,&SQL_OperSShortSmallEQSBig,&SQL_OperSShortSmallEQUBig,&SQL_OperSShortSmallEQNum,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_USHORT      */  ,{  &SQL_OperVarSmallEQChar ,&SQL_OperUShortSmallEQSShort,&SQL_OperUShortSmallEQUShort,&SQL_OperUShortSmallEQSLong ,&SQL_OperUShortSmallEQULong,&SQL_OperUShortSmallEQFloat,&SQL_OperUShortSmallEQDouble,&SQL_OperUShortSmallEQBit,&SQL_OperUShortSmallEQSTiny,&SQL_OperUShortSmallEQUTiny,&SQL_OperUShortSmallEQSBig,&SQL_OperUShortSmallEQUBig,&SQL_OperUShortSmallEQNum,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_SLONG       */  ,{  &SQL_OperVarSmallEQChar ,&SQL_OperSLongSmallEQSShort ,&SQL_OperSLongSmallEQUShort ,&SQL_OperSLongSmallEQSLong  ,&SQL_OperSLongSmallEQULong ,&SQL_OperSLongSmallEQFloat ,&SQL_OperSLongSmallEQDouble ,&SQL_OperSLongSmallEQBit ,&SQL_OperSLongSmallEQSTiny ,&SQL_OperSLongSmallEQUTiny ,&SQL_OperSLongSmallEQSBig ,&SQL_OperSLongSmallEQUBig ,&SQL_OperSLongSmallEQNum ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_ULONG       */  ,{  &SQL_OperVarSmallEQChar ,&SQL_OperULongSmallEQSShort ,&SQL_OperUlongSmallEQUShort ,&SQL_OperULongSmallEQSLong  ,&SQL_OperULongSmallEQULong ,&SQL_OperULongSmallEQFloat ,&SQL_OperULongSmallEQDouble ,&SQL_OperULongSmallEQBit ,&SQL_OperULongSmallEQSTiny ,&SQL_OperULongSmallEQUTiny ,&SQL_OperULongSmallEQSBig ,&SQL_OperULongSmallEQUBig ,&SQL_OperULongSmallEQNum ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_FLOAT       */  ,{  &SQL_OperVarSmallEQChar ,&SQL_OperFloatSmallEQSShort ,&SQL_OperFloatSmallEQUShort ,&SQL_OperFloatSmallEQSLong  ,&SQL_OperFloatSmallEQULong ,&SQL_OperFloatSmallEQFloat ,&SQL_OperFloatSmallEQDouble ,&SQL_OperFloatSmallEQBit ,&SQL_OperFloatSmallEQSTiny ,&SQL_OperFloatSmallEQUTiny ,&SQL_OperFloatSmallEQSBig ,&SQL_OperFloatSmallEQUBig ,&SQL_OperFloatSmallEQNum ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_DOUBLE      */  ,{  &SQL_OperVarSmallEQChar ,&SQL_OperDoubleSmallEQSShort,&SQL_OperDoubleSmallEQUShort,&SQL_OperDoubleESmallEQSLong,&SQL_OperDoubleSmallEQULong,&SQL_OperDoubleSmallEQFloat,&SQL_OperDoubleSmallEQDouble,&SQL_OperDoubleSmallEQBit,&SQL_OperDoubleSmallEQSTiny,&SQL_OperDoubleSmallEQUTiny,&SQL_OperDoubleSmallEQSBig,&SQL_OperDoubleSmallEQUBig,&SQL_OperDoubleSmallEQNum,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_BIT         */  ,{  &SQL_OperVarSmallEQChar ,&SQL_OperBitSmallEQSShort   ,&SQL_OperBitSmallEQUShort   ,&SQL_OperBitSmallEQSLong    ,&SQL_OperBitSmallEQULong   ,&SQL_OperBitSmallEQFloat   ,&SQL_OperBitSmallEQDouble   ,&SQL_OperBitSmallEQBit   ,&SQL_OperBitSmallEQSTiny   ,&SQL_OperBitSmallEQUTiny   ,&SQL_OperBitSmallEQSBig   ,&SQL_OperBitSmallEQUBig   ,&SQL_OperBitSmallEQNum   ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_STINYINT    */  ,{  &SQL_OperVarSmallEQChar ,&SQL_OperSTinySmallEQSShort ,&SQL_OperSTinySmallEQUShort ,&SQL_OperSTinySmallEQSLong  ,&SQL_OperSTinySmallEQULong ,&SQL_OperSTinySmallEQFloat ,&SQL_OperSTinySmallEQDouble ,&SQL_OperSTinySmallEQBit ,&SQL_OperSTinySmallEQSTiny ,&SQL_OperSTinySmallEQUTiny ,&SQL_OperSTinySmallEQSBig ,&SQL_OperSTinySmallEQUBig ,&SQL_OperSTinySmallEQNum ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_UTINYINT    */  ,{  &SQL_OperVarSmallEQChar ,&SQL_OperUTinySmallEQSShort ,&SQL_OperUTinySmallEQUShort ,&SQL_OperUTinySmallEQSLong  ,&SQL_OperUTinySmallEQULong ,&SQL_OperUTinySmallEQFloat ,&SQL_OperUTinySmallEQDouble ,&SQL_OperUTinySmallEQBit ,&SQL_OperUTinySmallEQSTiny ,&SQL_OperUTinySmallEQUTiny ,&SQL_OperUTinySmallEQSBig ,&SQL_OperUTinySmallEQUBig ,&SQL_OperUTinySmallEQNum ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_SBIGINT     */  ,{  &SQL_OperVarSmallEQChar ,&SQL_OperSBigSmallEQSShort  ,&SQL_OperSBigSmallEQUShort  ,&SQL_OperSBigSmallEQSLong   ,&SQL_OperSBigSmallEQULong  ,&SQL_OperSBigSmallEQFloat  ,&SQL_OperSBigSmallEQDouble  ,&SQL_OperSBigSmallEQBit  ,&SQL_OperSBigSmallEQSTiny  ,&SQL_OperSBigSmallEQUTiny  ,&SQL_OperSBigSmallEQSBig  ,&SQL_OperSBigSmallEQUBig  ,&SQL_OperSBigSmallEQNum  ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_UBIGINT     */  ,{  &SQL_OperVarSmallEQChar ,&SQL_OperUBigSmallEQSShort  ,&SQL_OperUBigSmallEQUShort  ,&SQL_OperUBigSmallEQSLong   ,&SQL_OperUBigSmallEQULong  ,&SQL_OperUBigSmallEQFloat  ,&SQL_OperUBigSmallEQDouble  ,&SQL_OperUBigSmallEQBit  ,&SQL_OperUBigSmallEQSTiny  ,&SQL_OperUBigSmallEQUTiny  ,&SQL_OperUBigSmallEQSBig  ,&SQL_OperUBigSmallEQUBig  ,&SQL_OperUBigSmallEQNum  ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_NUMERIC     */  ,{  &SQL_OperVarSmallEQChar ,&SQL_OperNumSmallEQSShort   ,&SQL_OperNumSmallEQUShort   ,&SQL_OperNumSmallEQSLong    ,&SQL_OperNumSmallEQULong   ,&SQL_OperNumSmallEQFloat   ,&SQL_OperNumSmallEQDouble   ,&SQL_OperNumSmallEQBit   ,&SQL_OperNumSmallEQSTiny   ,&SQL_OperNumSmallEQUTiny   ,&SQL_OperNumSmallEQSBig   ,&SQL_OperNumSmallEQUBig   ,&SQL_OperNumSmallEQNum   ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_GUID        */  ,{  &SQL_OperGuidSmallEQChar,nullptr                     ,nullptr                     ,nullptr                     ,nullptr                    ,nullptr                    ,nullptr                     ,nullptr                  ,nullptr                    ,nullptr                    ,nullptr                   ,nullptr                   ,nullptr                  ,&SQL_OperGuidSmallEQGuid,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_BINARY      */  ,{  nullptr                 ,nullptr                     ,nullptr                     ,nullptr                     ,nullptr                    ,nullptr                    ,nullptr                     ,nullptr                  ,nullptr                    ,nullptr                    ,nullptr                   ,nullptr                   ,nullptr                  ,nullptr                 ,&SQL_OperBinarySmallEQBinary,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_DATE        */  ,{  &SQL_OperVarSmallEQChar ,nullptr                     ,nullptr                     ,nullptr                     ,nullptr                    ,nullptr                    ,nullptr                     ,nullptr                  ,nullptr                    ,nullptr                    ,nullptr                   ,nullptr                   ,nullptr                  ,nullptr                 ,nullptr                     ,&SQL_OperDateSmallEQDate ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_TIME        */  ,{  &SQL_OperVarSmallEQChar ,nullptr                     ,nullptr                     ,nullptr                     ,nullptr                    ,nullptr                    ,nullptr                     ,nullptr                  ,nullptr                    ,nullptr                    ,nullptr                   ,nullptr                   ,nullptr                  ,nullptr                 ,nullptr                     ,nullptr                  ,&SQL_OperTimeSmallEQTime ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_TIMESTAMP   */  ,{  &SQL_OperVarSmallEQChar ,nullptr                     ,nullptr                     ,nullptr                     ,nullptr                    ,nullptr                    ,nullptr                     ,nullptr                  ,nullptr                    ,nullptr                    ,nullptr                   ,nullptr                   ,nullptr                  ,nullptr                 ,nullptr                     ,&SQL_OperStampSmallEQDate,&SQL_OperStampSmallEQTime,&SQL_OperStampSmallEQStamp,nullptr                   ,nullptr                    }
  /* CT_INTERVAL_YM */  ,{  &SQL_OperVarSmallEQChar ,nullptr                     ,nullptr                     ,nullptr                     ,nullptr                    ,nullptr                    ,nullptr                     ,nullptr                  ,nullptr                    ,nullptr                    ,nullptr                   ,nullptr                   ,nullptr                  ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,&SQL_OperIntYMSmallEQIntYM,nullptr                    }
  /* CT_INTERVAL_DS */  ,{  &SQL_OperVarSmallEQChar ,nullptr                     ,nullptr                     ,nullptr                     ,nullptr                    ,nullptr                    ,nullptr                     ,nullptr                  ,nullptr                    ,nullptr                    ,nullptr                   ,nullptr                   ,nullptr                  ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,&SQL_OperIntDSSmallEQIntDS }
};

// Smaller-Equal operator for SQLVariant
bool
SQLVariant::operator<=(const SQLVariant& p_right) const
{
  // If one of both is NULL, the result is false
  if(IsNULL() || p_right.IsNULL())
  {
    return false;
  }

  // Getting the concise type
  SQLConciseType left  = SQLTypeToConciseType(m_datatype);
  SQLConciseType right = SQLTypeToConciseType(p_right.m_datatype);

  // Check whether both datatypes are valid
  if(left == CT_LAST || right == CT_LAST)
  {
    return false;
  }

  // Find our comparison function
  OperatorCompare function = OperatorSmallerEqual[left][right].function;
  if(function)
  {
    return (*function)(*this,p_right);
  }
  // No compare function found
  // Data types are not comparable
  XString leftType  = SQLDataType::FindDatatype(m_datatype);
  XString rightType = SQLDataType::FindDatatype(p_right.m_datatype);
  XString error;
  error.Format(_T("Cannot do the smaller-equal operator on (%s <= %s)"),leftType.GetString(),rightType.GetString());
  throw StdException(error);
}

// End of namespace
}
