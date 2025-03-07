////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantOperatorSmaller.cpp
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
static SQL_OperVarSmallerChar(const SQLVariant& p_left,const SQLVariant& p_right)
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

  return leftString.Compare(rightString) < 0;
}

bool
static SQL_OperGuidSmallerChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  XString leftString,rightString;
  p_left .GetAsString(leftString);
  p_right.GetAsString(rightString);

  leftString. TrimLeft ('{');
  leftString. TrimRight('}');
  rightString.TrimLeft ('}');
  rightString.TrimRight('{');

  return leftString.Compare(rightString) < 0;
}

// SIGNED SHORT

bool
static SQL_OperSShortSmallerSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSShort() < p_right.GetAsSShort();
}

bool
static SQL_OperUShortSmallerSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUShort() < SQL_ShortToUShort(p_right.GetAsSShort());
}

bool
static SQL_OperSLongSmallerSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_SLongToShort(p_left.GetAsSLong()) < p_right.GetAsSShort();
}

bool
static SQL_OperULongSmallerSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_ULongToUShort(p_left.GetAsULong()) < SQL_ShortToUShort(p_right.GetAsSShort());
}

bool
static SQL_OperFloatSmallerSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_FloatToShort(p_left.GetAsFloat()) < p_right.GetAsSShort();
}

bool
static SQL_OperDoubleSmallerSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_DoubleToShort(p_left.GetAsDouble()) < p_right.GetAsSShort();
}

bool
static SQL_OperBitSmallerSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsBit() == 0 && p_right.GetAsSShort() != 0;
}

bool   
static SQL_OperSTinySmallerSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() < SQL_ShortToTinyInt(p_right.GetAsSShort());
}

bool
static SQL_OperUTinySmallerSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() < SQL_ShortToUTinyInt(p_right.GetAsSShort());
}

bool
static SQL_OperSBigSmallerSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_BIGINTToShort(p_left.GetAsSBigInt()) < p_right.GetAsSShort();
}

bool
static SQL_OperUBigSmallerSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_UBIGINTToShort(p_left.GetAsUBigInt()) < p_right.GetAsSShort();
}

bool
static SQL_OperNumSmallerSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_SLongToShort(p_left.GetAsBCD().AsLong()) < p_right.GetAsSShort();
}

// UNSIGNED SHORT

bool   
static SQL_OperSShortSmallerUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_ShortToUShort(p_left.GetAsSShort()) < p_right.GetAsUShort();
}

bool   
static SQL_OperUShortSmallerUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUShort() < p_right.GetAsUShort();
}

bool
static SQL_OperSLongSmallerUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_SLongToUShort(p_left.GetAsSLong()) < p_right.GetAsUShort();
}

bool
static SQL_OperUlongSmallerUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_ULongToUShort(p_left.GetAsULong()) < p_right.GetAsUShort();
}

bool
static SQL_OperFloatSmallerUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_FloatToUShort(p_left.GetAsFloat()) < p_right.GetAsUShort();
}

bool
static SQL_OperDoubleSmallerUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_DoubleToUShort(p_left.GetAsDouble()) < p_right.GetAsUShort();
}

bool
static SQL_OperBitSmallerUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsBit() == 0 && p_right.GetAsUShort() != 0;
}

bool
static SQL_OperSTinySmallerUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() < SQL_UShortToTinyInt(p_right.GetAsUShort());
}

bool
static SQL_OperUTinySmallerUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() < SQL_UShortToUTinyInt(p_right.GetAsUShort());
}

bool
static SQL_OperSBigSmallerUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_BIGINTToUShort(p_left.GetAsSBigInt()) < p_right.GetAsUShort();
}

bool
static SQL_OperUBigSmallerUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_UBIGINTToUShort(p_left.GetAsUBigInt()) < p_right.GetAsUShort();
}

bool
static SQL_OperNumSmallerUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_SLongToUShort(p_left.GetAsBCD().AsLong()) < p_right.GetAsUShort();
}

// SIGNED LONG

bool   
static SQL_OperSShortSmallerSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSShort() < SQL_SLongToShort(p_right.GetAsSLong());
}

bool
static SQL_OperUShortSmallerSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUShort() < SQL_SLongToUShort(p_right.GetAsSLong());
}

bool
static SQL_OperSLongSmallerSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSLong() < p_right.GetAsSLong();
}

bool
static SQL_OperULongSmallerSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsULong() < SQL_LongToULong(p_right.GetAsSLong());
}

bool
static SQL_OperFloatSmallerSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_FloatToLong(p_left.GetAsFloat()) < p_right.GetAsSLong();
}

bool
static SQL_OperDoubleESmallerSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_DoubleToLong(p_left.GetAsDouble()) < p_right.GetAsSLong();
}

bool
static SQL_OperBitSmallerSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsBit() == 0 && p_right.GetAsSLong() != 0;
}

bool
static SQL_OperSTinySmallerSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() < SQL_SLongToTinyInt(p_right.GetAsSLong());
}

bool
static SQL_OperUTinySmallerSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() < SQL_SLongToUTinyInt(p_right.GetAsSLong());
}

bool
static SQL_OperSBigSmallerSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_BIGINTToLong(p_left.GetAsSBigInt()) < p_right.GetAsSLong();
}

bool
static SQL_OperUBigSmallerSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_UBIGINTToLong(p_left.GetAsUBigInt()) < p_right.GetAsSLong();
}

bool
static SQL_OperNumSmallerSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsBCD().AsLong() < p_right.GetAsSLong();
}

// UNSIGNED LONG

bool   
static SQL_OperSShortSmallerULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_ShortToUShort(p_left.GetAsSShort()) < SQL_ULongToUShort(p_right.GetAsULong());
}

bool
static SQL_OperUShortSmallerULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUShort() < SQL_ULongToUShort(p_right.GetAsULong());
}

bool
static SQL_OperSLongSmallerULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_LongToULong(p_left.GetAsSLong()) < p_right.GetAsULong();
}

bool
static SQL_OperULongSmallerULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsULong() < p_right.GetAsULong();
}

bool
static SQL_OperFloatSmallerULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_FloatToULong(p_left.GetAsFloat()) < p_right.GetAsULong();
}

bool
static SQL_OperDoubleSmallerULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_DoubleToULong(p_left.GetAsDouble()) < p_right.GetAsULong();
}

bool
static SQL_OperBitSmallerULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsBit() == 0 && p_right.GetAsULong() != 0;
}

bool
static SQL_OperSTinySmallerULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_TinyIntToUTinyInt(p_left.GetAsSTinyInt()) < SQL_ULongToUTinyInt(p_right.GetAsULong());
}

bool
static SQL_OperUTinySmallerULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() < SQL_ULongToUTinyInt(p_right.GetAsULong());
}

bool
static SQL_OperSBigSmallerULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_BIGINTToULong(p_left.GetAsSBigInt()) < p_right.GetAsULong();
}

bool
static SQL_OperUBigSmallerULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_UBIGINTToULong(p_left.GetAsUBigInt()) < p_right.GetAsULong();
}

bool
static SQL_OperNumSmallerULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_BIGINTToULong(p_left.GetAsBCD().AsInt64()) < p_right.GetAsULong();
}

// FLOAT

bool   
static SQL_OperSShortSmallerFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSShort() < SQL_FloatToShort(p_right.GetAsFloat());
}

bool   
static SQL_OperUShortSmallerFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUShort() < SQL_FloatToUShort(p_right.GetAsFloat());
}

bool
static SQL_OperSLongSmallerFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSLong() < SQL_FloatToLong(p_right.GetAsFloat());
}

bool
static SQL_OperULongSmallerFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsULong() < SQL_FloatToULong(p_right.GetAsFloat());
}

bool
static SQL_OperFloatSmallerFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsFloat() < p_right.GetAsFloat();
}

bool
static SQL_OperDoubleSmallerFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_DoubleToFloat(p_left.GetAsDouble()) < p_right.GetAsFloat();
}

bool
static SQL_OperBitSmallerFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float right = p_right.GetAsFloat();
  return p_left.GetAsBit() == 0 && (right < -FLT_EPSILON || FLT_EPSILON < right);
}

bool
static SQL_OperSTinySmallerFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() < SQL_FloatToTinyInt(p_right.GetAsFloat());
}

bool
static SQL_OperUTinySmallerFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() < SQL_FloatToUTinyInt(p_right.GetAsFloat());
}

bool
static SQL_OperSBigSmallerFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSBigInt() < SQL_FloatToBIGINT(p_right.GetAsFloat());
}

bool
static SQL_OperUBigSmallerFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUBigInt() < SQL_FloatToUBIGINT(p_right.GetAsFloat());
}

bool
static SQL_OperNumSmallerFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsBCD().AsDouble() < (double) p_right.GetAsFloat();
}

// DOUBLE

bool
static SQL_OperSShortSmallerDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSShort() < SQL_DoubleToShort(p_right.GetAsDouble());
}

bool
static SQL_OperUShortSmallerDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUShort() < SQL_DoubleToUShort(p_right.GetAsDouble());
}

bool
static SQL_OperSLongSmallerDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSLong() < SQL_DoubleToLong(p_right.GetAsDouble());
}

bool
static SQL_OperULongSmallerDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsULong() < SQL_DoubleToULong(p_right.GetAsDouble());
}

bool
static SQL_OperFloatSmallerDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsFloat() < SQL_DoubleToFloat(p_right.GetAsDouble());
}

bool
static SQL_OperDoubleSmallerDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsDouble() < p_right.GetAsDouble();
}

bool
static SQL_OperBitSmallerDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double right = p_right.GetAsDouble();
  return p_left.GetAsBit() == 0 && (right < -DBL_EPSILON || DBL_EPSILON < right);
}

bool
static SQL_OperSTinySmallerDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() < SQL_DoubleToTinyInt(p_right.GetAsDouble());
}

bool
static SQL_OperUTinySmallerDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() < SQL_DoubleToUTinyInt(p_right.GetAsDouble());
}

bool
static SQL_OperSBigSmallerDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSBigInt() < SQL_DoubleToBIGINT(p_right.GetAsDouble());
}

bool
static SQL_OperUBigSmallerDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUBigInt() < SQL_DoubleToUBIGINT(p_right.GetAsDouble());
}

bool
static SQL_OperNumSmallerDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsBCD().AsDouble() < p_right.GetAsDouble();
}

// BIT

bool   
static SQL_OperSShortSmallerBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSShort() == 0 && p_right.GetAsBit() != 0;
}

bool
static SQL_OperUShortSmallerBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUShort() == 0 && p_right.GetAsBit() != 0;
}

bool   
static SQL_OperSLongSmallerBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSLong() == 0 && p_right.GetAsBit() != 0;
}

bool
static SQL_OperULongSmallerBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsULong() == 0 && p_right.GetAsBit() != 0;
}

bool
static SQL_OperFloatSmallerBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  float left = p_left.GetAsFloat();
  return (-FLT_EPSILON < left && left < FLT_EPSILON) && p_right.GetAsBit() != 0;
}

bool
static SQL_OperDoubleSmallerBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  double left = p_left.GetAsDouble();
  return (-DBL_EPSILON < left && left < DBL_EPSILON) && p_right.GetAsBit() != 0;
}

bool
static SQL_OperBitSmallerBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsBit() < p_right.GetAsBit();
}

bool
static SQL_OperSTinySmallerBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() == 0 && p_right.GetAsBit() != 0;
}

bool
static SQL_OperUTinySmallerBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() == 0 && p_right.GetAsBit() != 0;
}

bool
static SQL_OperSBigSmallerBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSBigInt() == 0 && p_right.GetAsBit() != 0;
}

bool
static SQL_OperUBigSmallerBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUBigInt() == 0 && p_right.GetAsBit() != 0;
}

bool
static SQL_OperNumSmallerBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsBCD().IsZero() && p_right.GetAsBit() != 0;
}

// SIGNED TINYINT

bool
static SQL_OperSShortSmallerSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_ShortToTinyInt(p_left.GetAsSShort()) < p_right.GetAsSTinyInt();
}

bool
static SQL_OperUShortSmallerSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_UShortToTinyInt(p_left.GetAsUShort()) < p_right.GetAsSTinyInt();
}

bool
static SQL_OperSLongSmallerSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_SLongToTinyInt(p_left.GetAsSLong()) < p_right.GetAsSTinyInt();
}

bool
static SQL_OperULongSmallerSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_ULongToTinyInt(p_left.GetAsULong()) < p_right.GetAsSTinyInt();
}

bool
static SQL_OperFloatSmallerSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_FloatToTinyInt(p_left.GetAsFloat()) < p_right.GetAsSTinyInt();
}

bool
static SQL_OperDoubleSmallerSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_DoubleToTinyInt(p_left.GetAsDouble()) < p_right.GetAsSTinyInt();
}

bool
static SQL_OperBitSmallerSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsBit() == 0 && p_right.GetAsSTinyInt() != 0;
}

bool
static SQL_OperSTinySmallerSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() < p_right.GetAsSTinyInt();
}

bool
static SQL_OperUTinySmallerSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() < SQL_TinyIntToUTinyInt(p_right.GetAsSTinyInt());
}

bool
static SQL_OperSBigSmallerSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_SBIGINTToTinyInt(p_left.GetAsSBigInt()) < p_right.GetAsSTinyInt();
}

bool
static SQL_OperUBigSmallerSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_UBIGINTToTinyInt(p_left.GetAsUBigInt()) < p_right.GetAsSTinyInt();
}

bool
static SQL_OperNumSmallerSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_SLongToTinyInt(p_left.GetAsBCD().AsLong()) < p_right.GetAsSTinyInt();
}

// UNSIGNED TINYINT

bool
static SQL_OperSShortSmallerUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_ShortToUTinyInt(p_left.GetAsSShort()) < p_right.GetAsUTinyInt();
}

bool
static SQL_OperUShortSmallerUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_UShortToUTinyInt(p_left.GetAsUShort()) < p_right.GetAsUTinyInt();
}

bool
static SQL_OperSLongSmallerUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_SLongToUTinyInt(p_left.GetAsSLong()) < p_right.GetAsUTinyInt();
}

bool
static SQL_OperULongSmallerUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_ULongToUTinyInt(p_left.GetAsULong()) < p_right.GetAsUTinyInt();
}

bool
static SQL_OperFloatSmallerUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_FloatToUTinyInt(p_left.GetAsFloat()) < p_right.GetAsUTinyInt();
}

bool
static SQL_OperDoubleSmallerUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_DoubleToUTinyInt(p_left.GetAsDouble()) < p_right.GetAsUTinyInt();
}

bool
static SQL_OperBitSmallerUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsBit() == 0 && p_right.GetAsUTinyInt() != 0;
}

bool
static SQL_OperSTinySmallerUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_TinyIntToUTinyInt(p_left.GetAsSTinyInt()) < p_right.GetAsUTinyInt();
}

bool
static SQL_OperUTinySmallerUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() < p_right.GetAsUTinyInt();
}

bool
static SQL_OperSBigSmallerUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_SBIGINTToUTinyInt(p_left.GetAsSBigInt()) < p_right.GetAsUTinyInt();
}

bool
static SQL_OperUBigSmallerUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_UBIGINTToUTinyInt(p_left.GetAsUBigInt()) < p_right.GetAsUTinyInt();
}

bool
static SQL_OperNumSmallerUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_SLongToUTinyInt(p_left.GetAsBCD().AsLong()) < p_right.GetAsUTinyInt();
}

// SIGNED BIGINT

bool
static SQL_OperSShortSmallerSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSShort() < SQL_BIGINTToShort(p_right.GetAsSBigInt());
}

bool
static SQL_OperUShortSmallerSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUShort() < SQL_BIGINTToUShort(p_right.GetAsSBigInt());
}

bool
static SQL_OperSLongSmallerSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSLong() < SQL_BIGINTToLong(p_right.GetAsSBigInt());
}

bool
static SQL_OperULongSmallerSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsULong() < SQL_BIGINTToULong(p_right.GetAsSBigInt());
}

bool
static SQL_OperFloatSmallerSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsFloat() < SQL_BIGINTToFloat(p_right.GetAsSBigInt());
}

bool
static SQL_OperDoubleSmallerSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsDouble() < SQL_BIGINTToDouble(p_right.GetAsSBigInt());
}

bool
static SQL_OperBitSmallerSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsBit() == 0 && p_right.GetAsSBigInt() != 0;
}

bool
static SQL_OperSTinySmallerSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() < SQL_SBIGINTToTinyInt(p_right.GetAsSBigInt());
}

bool
static SQL_OperUTinySmallerSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() < SQL_SBIGINTToUTinyInt(p_right.GetAsSBigInt());
}

bool
static SQL_OperSBigSmallerSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSBigInt() < p_right.GetAsSBigInt();
}

bool
static SQL_OperUBigSmallerSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUBigInt() < SQL_SBIGINTToUBIGINT(p_right.GetAsSBigInt());
}

bool   
static SQL_OperNumSmallerSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsBCD().AsInt64() < p_right.GetAsSBigInt();
}

// UNSIGNED BIGINT

bool
static SQL_OperSShortSmallerUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSShort() < SQL_UBIGINTToShort(p_right.GetAsUBigInt());
}

bool
static SQL_OperUShortSmallerUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUShort() < SQL_UBIGINTToUShort(p_right.GetAsUBigInt());
}

bool
static SQL_OperSLongSmallerUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSLong() < SQL_UBIGINTToLong(p_right.GetAsUBigInt());
}

bool
static SQL_OperULongSmallerUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsULong() < SQL_UBIGINTToULong(p_right.GetAsUBigInt());
}

bool
static SQL_OperFloatSmallerUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsFloat() < SQL_UBIGINTToFloat(p_right.GetAsUBigInt());
}

bool
static SQL_OperDoubleSmallerUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsDouble() < SQL_UBIGINTToDouble(p_right.GetAsUBigInt());
}

bool
static SQL_OperBitSmallerUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsBit() == 0 && p_right.GetAsUBigInt() != 0;
}

bool
static SQL_OperSTinySmallerUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() < SQL_UBIGINTToUTinyInt(p_right.GetAsUBigInt());
}

bool
static SQL_OperUTinySmallerUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() < SQL_UBIGINTToUTinyInt(p_right.GetAsUBigInt());
}

bool
static SQL_OperSBigSmallerUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_SBIGINTToUBIGINT(p_left.GetAsSBigInt()) < p_right.GetAsUBigInt();
}

bool
static SQL_OperUBigSmallerUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUBigInt() < p_right.GetAsUBigInt();
}

bool
static SQL_OperNumSmallerUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsBCD().AsUInt64() < p_right.GetAsUBigInt();
}

// NUMERIC

bool
static SQL_OperSShortSmallerNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSShort() < SQL_SLongToShort(p_right.GetAsBCD().AsLong());
}

bool
static SQL_OperUShortSmallerNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUShort() < SQL_SLongToUShort(p_right.GetAsBCD().AsLong());
}

bool
static SQL_OperSLongSmallerNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSLong() < p_right.GetAsBCD().AsLong();
}

bool
static SQL_OperULongSmallerNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsULong() < SQL_BIGINTToULong(p_right.GetAsBCD().AsInt64());
}

bool
static SQL_OperFloatSmallerNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsFloat() < (float) p_right.GetAsBCD().AsDouble();
}

bool
static SQL_OperDoubleSmallerNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsDouble() < p_right.GetAsBCD().AsDouble();
}

bool
static SQL_OperBitSmallerNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsBit() == 0 && !p_right.GetAsBCD().IsZero();
}

bool
static SQL_OperSTinySmallerNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() < SQL_SLongToTinyInt(p_right.GetAsBCD().AsLong());
}

bool
static SQL_OperUTinySmallerNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() < SQL_SLongToUTinyInt(p_right.GetAsBCD().AsLong());
}

bool
static SQL_OperSBigSmallerNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSBigInt() < p_right.GetAsBCD().AsInt64();
}

bool
static SQL_OperUBigSmallerNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUBigInt() < p_right.GetAsBCD().AsUInt64();
}

bool
static SQL_OperNumSmallerNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsBCD() < p_right.GetAsBCD();
}

// TYPE == GUID

bool
static SQL_OperGuidSmallerGuid(const SQLVariant& p_left,const SQLVariant& p_right)
{
  const SQLGUID* left  = p_left.GetAsGUID();
  const SQLGUID* right = p_right.GetAsGUID();

  return memcmp(left,right,sizeof(SQLGUID)) < 0;
}

// TYPE == BINARY

bool
static SQL_OperBinarySmallerBinary(const SQLVariant& p_left,const SQLVariant& p_right)
{
  void* left   = p_left.GetAsBinary();
  void* right  = p_right.GetAsBinary();
  // Take longest of two binaries in comparison
  long  length = p_left.GetBinaryLength() < p_right.GetBinaryLength() ? p_left.GetBinaryLength() : p_right.GetBinaryLength();

  return memcmp(left,right,length) < 0;
}

// TYPE == DATE

bool
static SQL_OperDateSmallerDate(const SQLVariant& p_left,const SQLVariant& p_right)
{
  // Relies on the < operator of SQLDate
  return p_left.GetAsSQLDate() < p_right.GetAsSQLDate();
}

bool
static SQL_OperStampSmallerDate(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLDate date = p_left.GetAsSQLTimestamp().AsSQLDate();
  return date < p_right.GetAsSQLDate();
}

// TYPE == TIME

bool
static SQL_OperTimeSmallerTime(const SQLVariant& p_left,const SQLVariant& p_right)
{
  // Relies on the < operator of SQLTime
  return p_left.GetAsSQLTime() < p_right.GetAsSQLTime();
}

bool
static SQL_OperStampSmallerTime(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLTime time = p_left.GetAsSQLTimestamp().AsSQLTime();
  return time < p_right.GetAsSQLTime();
}

// TYPE == TIMESTAMP

bool
static SQL_OperStampSmallerStamp(const SQLVariant& p_left,const SQLVariant& p_right)
{
  // Relies on the < operator of SQLTimestamp
  return p_left.GetAsSQLTimestamp() < p_right.GetAsSQLTimestamp();
}

// TYPE == INTERVAL_YEAR_MONTH

bool
static SQL_OperIntYMSmallerIntYM(const SQLVariant& p_left,const SQLVariant& p_right)
{
  // Relies on the calculation of months in SQLInterval
  return p_left.GetAsSQLInterval().AsDatabaseNumber() < p_right.GetAsSQLInterval().AsDatabaseNumber();
}

// TYPE == INTERVAL_DAY_SECOND

bool
static SQL_OperIntDSSmallerIntDS(const SQLVariant& p_left,const SQLVariant& p_right)
{
  // Relies on the calculation of seconds in SQLInterval
  return p_left.GetAsSQLInterval().AsDatabaseNumber() < p_right.GetAsSQLInterval().AsDatabaseNumber();
}

// OPERATOR ARRAY

static CompareFunctionArray OperatorSmaller[CT_LAST][CT_LAST] =
{
                         // CT_CHAR                  CT_SSHORT                    CT_USHORT                    CT_SLONG                     CT_ULONG                    CT_FLOAT                    CT_DOUBLE                    CT_BIT                    CT_STINYINT                 CT_UTINYINT                 CT_SBIGINT                 CT_UBIGINT                 CT_NUMERIC                CT_GUID                  CT_BINARY                    CT_DATE                   CT_TIME                   CT_TIMESTAMP               CT_INTERVAL_YM             CT_INTERVAL_DS
                         // ------------------------ ---------------------------- ---------------------------- ---------------------------- --------------------------- --------------------------- ---------------------------- ------------------------- --------------------------- --------------------------- -------------------------- -------------------------- ------------------------- ------------------------ ---------------------------- ------------------------- ------------------------- -------------------------- -------------------------- ----------------------------
  /* CT_CHAR        */   {  &SQL_OperVarSmallerChar ,&SQL_OperVarSmallerChar     ,&SQL_OperVarSmallerChar     ,&SQL_OperVarSmallerChar     ,&SQL_OperVarSmallerChar    ,&SQL_OperVarSmallerChar    ,&SQL_OperVarSmallerChar     ,&SQL_OperVarSmallerChar  ,&SQL_OperVarSmallerChar    ,&SQL_OperVarSmallerChar    ,&SQL_OperVarSmallerChar   ,&SQL_OperVarSmallerChar   ,&SQL_OperVarSmallerChar  ,&SQL_OperVarSmallerChar ,nullptr                     ,&SQL_OperVarSmallerChar  ,&SQL_OperVarSmallerChar  ,&SQL_OperVarSmallerChar   ,&SQL_OperVarSmallerChar   ,&SQL_OperVarSmallerChar    }
  /* CT_SSHORT      */  ,{  &SQL_OperVarSmallerChar ,&SQL_OperSShortSmallerSShort,&SQL_OperSShortSmallerUShort,&SQL_OperSShortSmallerSLong ,&SQL_OperSShortSmallerULong,&SQL_OperSShortSmallerFloat,&SQL_OperSShortSmallerDouble,&SQL_OperSShortSmallerBit,&SQL_OperSShortSmallerSTiny,&SQL_OperSShortSmallerUTiny,&SQL_OperSShortSmallerSBig,&SQL_OperSShortSmallerUBig,&SQL_OperSShortSmallerNum,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_USHORT      */  ,{  &SQL_OperVarSmallerChar ,&SQL_OperUShortSmallerSShort,&SQL_OperUShortSmallerUShort,&SQL_OperUShortSmallerSLong ,&SQL_OperUShortSmallerULong,&SQL_OperUShortSmallerFloat,&SQL_OperUShortSmallerDouble,&SQL_OperUShortSmallerBit,&SQL_OperUShortSmallerSTiny,&SQL_OperUShortSmallerUTiny,&SQL_OperUShortSmallerSBig,&SQL_OperUShortSmallerUBig,&SQL_OperUShortSmallerNum,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_SLONG       */  ,{  &SQL_OperVarSmallerChar ,&SQL_OperSLongSmallerSShort ,&SQL_OperSLongSmallerUShort ,&SQL_OperSLongSmallerSLong  ,&SQL_OperSLongSmallerULong ,&SQL_OperSLongSmallerFloat ,&SQL_OperSLongSmallerDouble ,&SQL_OperSLongSmallerBit ,&SQL_OperSLongSmallerSTiny ,&SQL_OperSLongSmallerUTiny ,&SQL_OperSLongSmallerSBig ,&SQL_OperSLongSmallerUBig ,&SQL_OperSLongSmallerNum ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_ULONG       */  ,{  &SQL_OperVarSmallerChar ,&SQL_OperULongSmallerSShort ,&SQL_OperUlongSmallerUShort ,&SQL_OperULongSmallerSLong  ,&SQL_OperULongSmallerULong ,&SQL_OperULongSmallerFloat ,&SQL_OperULongSmallerDouble ,&SQL_OperULongSmallerBit ,&SQL_OperULongSmallerSTiny ,&SQL_OperULongSmallerUTiny ,&SQL_OperULongSmallerSBig ,&SQL_OperULongSmallerUBig ,&SQL_OperULongSmallerNum ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_FLOAT       */  ,{  &SQL_OperVarSmallerChar ,&SQL_OperFloatSmallerSShort ,&SQL_OperFloatSmallerUShort ,&SQL_OperFloatSmallerSLong  ,&SQL_OperFloatSmallerULong ,&SQL_OperFloatSmallerFloat ,&SQL_OperFloatSmallerDouble ,&SQL_OperFloatSmallerBit ,&SQL_OperFloatSmallerSTiny ,&SQL_OperFloatSmallerUTiny ,&SQL_OperFloatSmallerSBig ,&SQL_OperFloatSmallerUBig ,&SQL_OperFloatSmallerNum ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_DOUBLE      */  ,{  &SQL_OperVarSmallerChar ,&SQL_OperDoubleSmallerSShort,&SQL_OperDoubleSmallerUShort,&SQL_OperDoubleESmallerSLong,&SQL_OperDoubleSmallerULong,&SQL_OperDoubleSmallerFloat,&SQL_OperDoubleSmallerDouble,&SQL_OperDoubleSmallerBit,&SQL_OperDoubleSmallerSTiny,&SQL_OperDoubleSmallerUTiny,&SQL_OperDoubleSmallerSBig,&SQL_OperDoubleSmallerUBig,&SQL_OperDoubleSmallerNum,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_BIT         */  ,{  &SQL_OperVarSmallerChar ,&SQL_OperBitSmallerSShort   ,&SQL_OperBitSmallerUShort   ,&SQL_OperBitSmallerSLong    ,&SQL_OperBitSmallerULong   ,&SQL_OperBitSmallerFloat   ,&SQL_OperBitSmallerDouble   ,&SQL_OperBitSmallerBit   ,&SQL_OperBitSmallerSTiny   ,&SQL_OperBitSmallerUTiny   ,&SQL_OperBitSmallerSBig   ,&SQL_OperBitSmallerUBig   ,&SQL_OperBitSmallerNum   ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_STINYINT    */  ,{  &SQL_OperVarSmallerChar ,&SQL_OperSTinySmallerSShort ,&SQL_OperSTinySmallerUShort ,&SQL_OperSTinySmallerSLong  ,&SQL_OperSTinySmallerULong ,&SQL_OperSTinySmallerFloat ,&SQL_OperSTinySmallerDouble ,&SQL_OperSTinySmallerBit ,&SQL_OperSTinySmallerSTiny ,&SQL_OperSTinySmallerUTiny ,&SQL_OperSTinySmallerSBig ,&SQL_OperSTinySmallerUBig ,&SQL_OperSTinySmallerNum ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_UTINYINT    */  ,{  &SQL_OperVarSmallerChar ,&SQL_OperUTinySmallerSShort ,&SQL_OperUTinySmallerUShort ,&SQL_OperUTinySmallerSLong  ,&SQL_OperUTinySmallerULong ,&SQL_OperUTinySmallerFloat ,&SQL_OperUTinySmallerDouble ,&SQL_OperUTinySmallerBit ,&SQL_OperUTinySmallerSTiny ,&SQL_OperUTinySmallerUTiny ,&SQL_OperUTinySmallerSBig ,&SQL_OperUTinySmallerUBig ,&SQL_OperUTinySmallerNum ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_SBIGINT     */  ,{  &SQL_OperVarSmallerChar ,&SQL_OperSBigSmallerSShort  ,&SQL_OperSBigSmallerUShort  ,&SQL_OperSBigSmallerSLong   ,&SQL_OperSBigSmallerULong  ,&SQL_OperSBigSmallerFloat  ,&SQL_OperSBigSmallerDouble  ,&SQL_OperSBigSmallerBit  ,&SQL_OperSBigSmallerSTiny  ,&SQL_OperSBigSmallerUTiny  ,&SQL_OperSBigSmallerSBig  ,&SQL_OperSBigSmallerUBig  ,&SQL_OperSBigSmallerNum  ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_UBIGINT     */  ,{  &SQL_OperVarSmallerChar ,&SQL_OperUBigSmallerSShort  ,&SQL_OperUBigSmallerUShort  ,&SQL_OperUBigSmallerSLong   ,&SQL_OperUBigSmallerULong  ,&SQL_OperUBigSmallerFloat  ,&SQL_OperUBigSmallerDouble  ,&SQL_OperUBigSmallerBit  ,&SQL_OperUBigSmallerSTiny  ,&SQL_OperUBigSmallerUTiny  ,&SQL_OperUBigSmallerSBig  ,&SQL_OperUBigSmallerUBig  ,&SQL_OperUBigSmallerNum  ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_NUMERIC     */  ,{  &SQL_OperVarSmallerChar ,&SQL_OperNumSmallerSShort   ,&SQL_OperNumSmallerUShort   ,&SQL_OperNumSmallerSLong    ,&SQL_OperNumSmallerULong   ,&SQL_OperNumSmallerFloat   ,&SQL_OperNumSmallerDouble   ,&SQL_OperNumSmallerBit   ,&SQL_OperNumSmallerSTiny   ,&SQL_OperNumSmallerUTiny   ,&SQL_OperNumSmallerSBig   ,&SQL_OperNumSmallerUBig   ,&SQL_OperNumSmallerNum   ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_GUID        */  ,{  &SQL_OperGuidSmallerChar,nullptr                     ,nullptr                     ,nullptr                     ,nullptr                    ,nullptr                    ,nullptr                     ,nullptr                  ,nullptr                    ,nullptr                    ,nullptr                   ,nullptr                   ,nullptr                  ,&SQL_OperGuidSmallerGuid,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_BINARY      */  ,{  nullptr                 ,nullptr                     ,nullptr                     ,nullptr                     ,nullptr                    ,nullptr                    ,nullptr                     ,nullptr                  ,nullptr                    ,nullptr                    ,nullptr                   ,nullptr                   ,nullptr                  ,nullptr                 ,&SQL_OperBinarySmallerBinary,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_DATE        */  ,{  &SQL_OperVarSmallerChar ,nullptr                     ,nullptr                     ,nullptr                     ,nullptr                    ,nullptr                    ,nullptr                     ,nullptr                  ,nullptr                    ,nullptr                    ,nullptr                   ,nullptr                   ,nullptr                  ,nullptr                 ,nullptr                     ,&SQL_OperDateSmallerDate ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_TIME        */  ,{  &SQL_OperVarSmallerChar ,nullptr                     ,nullptr                     ,nullptr                     ,nullptr                    ,nullptr                    ,nullptr                     ,nullptr                  ,nullptr                    ,nullptr                    ,nullptr                   ,nullptr                   ,nullptr                  ,nullptr                 ,nullptr                     ,nullptr                  ,&SQL_OperTimeSmallerTime ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_TIMESTAMP   */  ,{  &SQL_OperVarSmallerChar ,nullptr                     ,nullptr                     ,nullptr                     ,nullptr                    ,nullptr                    ,nullptr                     ,nullptr                  ,nullptr                    ,nullptr                    ,nullptr                   ,nullptr                   ,nullptr                  ,nullptr                 ,nullptr                     ,&SQL_OperStampSmallerDate,&SQL_OperStampSmallerTime,&SQL_OperStampSmallerStamp,nullptr                   ,nullptr                    }
  /* CT_INTERVAL_YM */  ,{  &SQL_OperVarSmallerChar ,nullptr                     ,nullptr                     ,nullptr                     ,nullptr                    ,nullptr                    ,nullptr                     ,nullptr                  ,nullptr                    ,nullptr                    ,nullptr                   ,nullptr                   ,nullptr                  ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,&SQL_OperIntYMSmallerIntYM,nullptr                    }
  /* CT_INTERVAL_DS */  ,{  &SQL_OperVarSmallerChar ,nullptr                     ,nullptr                     ,nullptr                     ,nullptr                    ,nullptr                    ,nullptr                     ,nullptr                  ,nullptr                    ,nullptr                    ,nullptr                   ,nullptr                   ,nullptr                  ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,&SQL_OperIntDSSmallerIntDS }
};

// Smaller operator for SQLVariant
bool
SQLVariant::operator<(const SQLVariant& p_right) const
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
  OperatorCompare function = OperatorSmaller[left][right].function;
  if(function)
  {
    return (*function)(*this,p_right);
  }
  // No compare function found
  // Data types are not comparable
  XString leftType  = SQLDataType::FindDatatype(m_datatype);
  XString rightType = SQLDataType::FindDatatype(p_right.m_datatype);
  XString error;
  error.Format(_T("Cannot do the Smaller operator on (%s < %s)"),leftType.GetString(),rightType.GetString());
  throw StdException(error);
}

// End of namespace
}
