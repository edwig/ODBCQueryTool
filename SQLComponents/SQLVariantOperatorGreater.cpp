////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantOperatorGreater.cpp
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
#include "SQLVariantTrim.h"
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

//////////////////////////////////////////////////////////////////////////
//
// ALL EQUALITY OPERATOR FUNCTIONS
//
//////////////////////////////////////////////////////////////////////////

bool
static SQL_OperVarGreaterChar(SQLVariant& p_left,SQLVariant& p_right)
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

  return leftString.Compare(rightString) > 0;
}

bool
static SQL_OperGuidGreaterChar(SQLVariant& p_left,SQLVariant& p_right)
{
  XString leftString,rightString;
  p_left .GetAsString(leftString);
  p_right.GetAsString(rightString);

  leftString. TrimLeft ('{');
  leftString. TrimRight('}');
  rightString.TrimLeft ('}');
  rightString.TrimRight('{');

  return leftString.Compare(rightString) > 0;
}

// SIGNED SHORT

bool
static SQL_OperSShortGreaterSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSShort() > p_right.GetAsSShort();
}

bool
static SQL_OperUShortGreaterSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUShort() > SQL_ShortToUShort(p_right.GetAsSShort());
}

bool
static SQL_OperSLongGreaterSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SLongToShort(p_left.GetAsSLong()) > p_right.GetAsSShort();
}

bool
static SQL_OperULongGreaterSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_ULongToUShort(p_left.GetAsULong()) > SQL_ShortToUShort(p_right.GetAsSShort());
}

bool
static SQL_OperFloatGreaterSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_FloatToShort(p_left.GetAsFloat()) > p_right.GetAsSShort();
}

bool
static SQL_OperDoubleGreaterSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_DoubleToShort(p_left.GetAsDouble()) > p_right.GetAsSShort();
}

bool
static SQL_OperBitGreaterSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBit() != 0 && p_right.GetAsSShort() == 0;
}

bool   
static SQL_OperSTinyGreaterSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() > SQL_ShortToTinyInt(p_right.GetAsSShort());
}

bool
static SQL_OperUTinyGreaterSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() > SQL_ShortToUTinyInt(p_right.GetAsSShort());
}

bool
static SQL_OperSBigGreaterSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_BIGINTToShort(p_left.GetAsSBigInt()) > p_right.GetAsSShort();
}

bool
static SQL_OperUBigGreaterSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_UBIGINTToShort(p_left.GetAsUBigInt()) > p_right.GetAsSShort();
}

bool
static SQL_OperNumGreaterSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SLongToShort(p_left.GetAsBCD().AsLong()) > p_right.GetAsUShort();
}

// UNSIGNED SHORT

bool   
static SQL_OperSShortGreaterUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_ShortToUShort(p_left.GetAsSShort()) > p_right.GetAsUShort();
}

bool   
static SQL_OperUShortGreaterUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUShort() > p_right.GetAsUShort();
}

bool
static SQL_OperSLongGreaterUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SLongToUShort(p_left.GetAsSLong()) > p_right.GetAsUShort();
}

bool
static SQL_OperUlongGreaterUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_ULongToUShort(p_left.GetAsULong()) > p_right.GetAsUShort();
}

bool
static SQL_OperFloatGreaterUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_FloatToUShort(p_left.GetAsFloat()) > p_right.GetAsUShort();
}

bool
static SQL_OperDoubleGreaterUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_DoubleToUShort(p_left.GetAsDouble()) > p_right.GetAsUShort();
}

bool
static SQL_OperBitGreaterUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBit() != 0 && p_right.GetAsUShort() == 0;
}

bool
static SQL_OperSTinyGreaterUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() > SQL_UShortToTinyInt(p_right.GetAsUShort());
}

bool
static SQL_OperUTinyGreaterUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() > SQL_UShortToUTinyInt(p_right.GetAsUShort());
}

bool
static SQL_OperSBigGreaterUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_BIGINTToUShort(p_left.GetAsSBigInt()) > p_right.GetAsUShort();
}

bool
static SQL_OperUBigGreaterUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_UBIGINTToUShort(p_left.GetAsUBigInt()) > p_right.GetAsUShort();
}

bool
static SQL_OperNumGreaterUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SLongToUShort(p_left.GetAsBCD().AsLong()) > p_right.GetAsUShort();
}


// SIGNED LONG

bool   
static SQL_OperSShortGreaterSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSShort() > SQL_SLongToShort(p_right.GetAsSLong());
}

bool
static SQL_OperUShortGreaterSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUShort() > SQL_SLongToUShort(p_right.GetAsSLong());
}

bool
static SQL_OperSLongGreaterSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSLong() > p_right.GetAsSLong();
}

bool
static SQL_OperULongGreaterSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsULong() > SQL_LongToULong(p_right.GetAsSLong());
}

bool
static SQL_OperFloatGreaterSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_FloatToLong(p_left.GetAsFloat()) > p_right.GetAsSLong();
}

bool
static SQL_OperDoubleEGreaterSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_DoubleToLong(p_left.GetAsDouble()) > p_right.GetAsSLong();
}

bool
static SQL_OperBitGreaterSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBit() != 0 && p_right.GetAsSLong() == 0;
}

bool
static SQL_OperSTinyGreaterSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() > SQL_SLongToTinyInt(p_right.GetAsSLong());
}

bool
static SQL_OperUTinyGreaterSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() > SQL_SLongToUTinyInt(p_right.GetAsSLong());
}

bool
static SQL_OperSBigGreaterSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_BIGINTToLong(p_left.GetAsSBigInt()) > p_right.GetAsSLong();
}

bool
static SQL_OperUBigGreaterSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_UBIGINTToLong(p_left.GetAsUBigInt()) > p_right.GetAsSLong();
}

bool
static SQL_OperNumGreaterSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBCD().AsLong() > p_right.GetAsSLong();
}

// UNSIGNED LONG

bool   
static SQL_OperSShortGreaterULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_ShortToUShort(p_left.GetAsSShort()) > SQL_ULongToUShort(p_right.GetAsULong());
}

bool
static SQL_OperUShortGreaterULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUShort() > SQL_ULongToUShort(p_right.GetAsULong());
}

bool
static SQL_OperSLongGreaterULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_LongToULong(p_left.GetAsSLong()) > p_right.GetAsULong();
}

bool
static SQL_OperULongGreaterULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsULong() > p_right.GetAsULong();
}

bool
static SQL_OperFloatGreaterULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_FloatToULong(p_left.GetAsFloat()) > p_right.GetAsULong();
}

bool
static SQL_OperDoubleGreaterULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_DoubleToULong(p_left.GetAsDouble()) > p_right.GetAsULong();
}

bool
static SQL_OperBitGreaterULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBit() != 0 && p_right.GetAsULong() == 0;
}

bool
static SQL_OperSTinyGreaterULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_TinyIntToUTinyInt(p_left.GetAsSTinyInt()) > SQL_ULongToUTinyInt(p_right.GetAsULong());
}

bool
static SQL_OperUTinyGreaterULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() > SQL_ULongToUTinyInt(p_right.GetAsULong());
}

bool
static SQL_OperSBigGreaterULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_BIGINTToULong(p_left.GetAsSBigInt()) > p_right.GetAsULong();
}

bool
static SQL_OperUBigGreaterULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_UBIGINTToULong(p_left.GetAsUBigInt()) > p_right.GetAsULong();
}

bool
static SQL_OperNumGreaterULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_BIGINTToULong(p_left.GetAsBCD().AsInt64()) > p_right.GetAsULong();
}

// FLOAT

bool   
static SQL_OperSShortGreaterFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSShort() > SQL_FloatToShort(p_right.GetAsFloat());
}

bool   
static SQL_OperUShortGreaterFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUShort() > SQL_FloatToUShort(p_right.GetAsFloat());
}

bool
static SQL_OperSLongGreaterFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSLong() > SQL_FloatToLong(p_right.GetAsFloat());
}

bool
static SQL_OperULongGreaterFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsULong() > SQL_FloatToULong(p_right.GetAsFloat());
}

bool
static SQL_OperFloatGreaterFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsFloat() > p_right.GetAsFloat();
}

bool
static SQL_OperDoubleGreaterFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_DoubleToFloat(p_left.GetAsDouble()) > p_right.GetAsFloat();
}

bool
static SQL_OperBitGreaterFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  float right = p_right.GetAsFloat();
  return p_left.GetAsBit() != 0 && (-FLT_EPSILON < right && right < FLT_EPSILON);
}

bool
static SQL_OperSTinyGreaterFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() > SQL_FloatToTinyInt(p_right.GetAsFloat());
}

bool
static SQL_OperUTinyGreaterFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() > SQL_FloatToUTinyInt(p_right.GetAsFloat());
}

bool
static SQL_OperSBigGreaterFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSBigInt() > SQL_FloatToBIGINT(p_right.GetAsFloat());
}

bool
static SQL_OperUBigGreaterFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUBigInt() > SQL_FloatToUBIGINT(p_right.GetAsFloat());
}

bool
static SQL_OperNumGreaterFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_DoubleToFloat(p_left.GetAsBCD().AsDouble()) > p_right.GetAsFloat();
}

// DOUBLE

bool
static SQL_OperSShortGreaterDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSShort() > SQL_DoubleToShort(p_right.GetAsDouble());
}

bool
static SQL_OperUShortGreaterDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUShort() > SQL_DoubleToUShort(p_right.GetAsDouble());
}

bool
static SQL_OperSLongGreaterDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSLong() > SQL_DoubleToLong(p_right.GetAsDouble());
}

bool
static SQL_OperULongGreaterDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsULong() > SQL_DoubleToULong(p_right.GetAsDouble());
}

bool
static SQL_OperFloatGreaterDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsFloat() > SQL_DoubleToFloat(p_right.GetAsDouble());
}

bool
static SQL_OperDoubleGreaterDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsDouble() > p_right.GetAsDouble();
}

bool
static SQL_OperBitGreaterDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  double right = p_right.GetAsDouble();
  return p_left.GetAsBit() != 0 && (-DBL_EPSILON < right && right < DBL_EPSILON);
}

bool
static SQL_OperSTinyGreaterDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() > SQL_DoubleToTinyInt(p_right.GetAsDouble());
}

bool
static SQL_OperUTinyGreaterDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() > SQL_DoubleToUTinyInt(p_right.GetAsDouble());
}

bool
static SQL_OperSBigGreaterDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSBigInt() > SQL_DoubleToBIGINT(p_right.GetAsDouble());
}

bool
static SQL_OperUBigGreaterDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUBigInt() > SQL_DoubleToUBIGINT(p_right.GetAsDouble());
}

bool
static SQL_OperNumGreaterDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBCD().AsDouble() > p_right.GetAsDouble();
}

// BIT

bool   
static SQL_OperSShortGreaterBit(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSShort() != 0 && p_right.GetAsBit() == 0;
}

bool
static SQL_OperUShortGreaterBit(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUShort() > 0 && p_right.GetAsBit() == 0;
}

bool   
static SQL_OperSLongGreaterBit(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSLong() > 0 && p_right.GetAsBit() == 0;
}

bool
static SQL_OperULongGreaterBit(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsULong() > 0 && p_right.GetAsBit() == 0;
}

bool
static SQL_OperFloatGreaterBit(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsFloat() > 0.0 && p_right.GetAsBit() == 0;
}

bool
static SQL_OperDoubleGreaterBit(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsDouble() > 0.0 && p_right.GetAsBit() == 0;
}

bool
static SQL_OperBitGreaterBit(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBit() > p_right.GetAsBit();
}

bool
static SQL_OperSTinyGreaterBit(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() > 0 && p_right.GetAsBit() == 0;
}

bool
static SQL_OperUTinyGreaterBit(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() > 0 && p_right.GetAsBit() == 0;
}

bool
static SQL_OperSBigGreaterBit(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSBigInt() > 0 && p_right.GetAsBit() == 0;
}

bool
static SQL_OperUBigGreaterBit(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUBigInt() > 0 && p_right.GetAsBit() == 0;
}

bool
static SQL_OperNumGreaterBit(SQLVariant& p_left,SQLVariant& p_right)
{
  return !p_left.GetAsBCD().IsNull() && p_right.GetAsBit() == 0;
}

// SIGNED TINYINT

bool
static SQL_OperSShortGreaterSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_ShortToTinyInt(p_left.GetAsSShort()) > p_right.GetAsSTinyInt();
}

bool
static SQL_OperUShortGreaterSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_UShortToTinyInt(p_left.GetAsUShort()) > p_right.GetAsSTinyInt();
}

bool
static SQL_OperSLongGreaterSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SLongToTinyInt(p_left.GetAsSLong()) > p_right.GetAsSTinyInt();
}

bool
static SQL_OperULongGreaterSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_ULongToTinyInt(p_left.GetAsULong()) > p_right.GetAsSTinyInt();
}

bool
static SQL_OperFloatGreaterSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_FloatToTinyInt(p_left.GetAsFloat()) > p_right.GetAsSTinyInt();
}

bool
static SQL_OperDoubleGreaterSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_DoubleToTinyInt(p_left.GetAsDouble()) > p_right.GetAsSTinyInt();
}

bool
static SQL_OperBitGreaterSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBit() != 0 && p_right.GetAsSTinyInt() == 0;
}

bool
static SQL_OperSTinyGreaterSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() > p_right.GetAsSTinyInt();
}

bool
static SQL_OperUTinyGreaterSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() > SQL_TinyIntToUTinyInt(p_right.GetAsSTinyInt());
}

bool
static SQL_OperSBigGreaterSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SBIGINTToTinyInt(p_left.GetAsSBigInt()) > p_right.GetAsSTinyInt();
}

bool
static SQL_OperUBigGreaterSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_UBIGINTToTinyInt(p_left.GetAsUBigInt()) > p_right.GetAsSTinyInt();
}

bool
static SQL_OperNumGreaterSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SLongToTinyInt(p_left.GetAsBCD().AsLong()) > p_right.GetAsSTinyInt();
}

// UNSIGNED TINYINT

bool
static SQL_OperSShortGreaterUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_ShortToUTinyInt(p_left.GetAsSShort()) > p_right.GetAsUTinyInt();
}

bool
static SQL_OperUShortGreaterUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_UShortToUTinyInt(p_left.GetAsUShort()) > p_right.GetAsUTinyInt();
}

bool
static SQL_OperSLongGreaterUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SLongToUTinyInt(p_left.GetAsSLong()) > p_right.GetAsUTinyInt();
}

bool
static SQL_OperULongGreaterUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_ULongToUTinyInt(p_left.GetAsULong()) > p_right.GetAsUTinyInt();
}

bool
static SQL_OperFloatGreaterUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_FloatToUTinyInt(p_left.GetAsFloat()) > p_right.GetAsUTinyInt();
}

bool
static SQL_OperDoubleGreaterUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_DoubleToUTinyInt(p_left.GetAsDouble()) > p_right.GetAsUTinyInt();
}

bool
static SQL_OperBitGreaterUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBit() != 0 && p_right.GetAsUTinyInt() == 0;
}

bool
static SQL_OperSTinyGreaterUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_TinyIntToUTinyInt(p_left.GetAsSTinyInt()) > p_right.GetAsUTinyInt();
}

bool
static SQL_OperUTinyGreaterUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() > p_right.GetAsUTinyInt();
}

bool
static SQL_OperSBigGreaterUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SBIGINTToUTinyInt(p_left.GetAsSBigInt()) > p_right.GetAsUTinyInt();
}

bool
static SQL_OperUBigGreaterUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_UBIGINTToUTinyInt(p_left.GetAsUBigInt()) > p_right.GetAsUTinyInt();
}

bool
static SQL_OperNumGreaterUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SLongToUTinyInt(p_left.GetAsBCD().AsLong()) > p_right.GetAsUTinyInt();
}

// SIGNED BIGINT

bool
static SQL_OperSShortGreaterSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSShort() > SQL_BIGINTToShort(p_right.GetAsSBigInt());
}

bool
static SQL_OperUShortGreaterSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUShort() > SQL_BIGINTToUShort(p_right.GetAsSBigInt());
}

bool
static SQL_OperSLongGreaterSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSLong() > SQL_BIGINTToLong(p_right.GetAsSBigInt());
}

bool
static SQL_OperULongGreaterSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsULong() > SQL_BIGINTToULong(p_right.GetAsSBigInt());
}

bool
static SQL_OperFloatGreaterSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsFloat() > SQL_BIGINTToFloat(p_right.GetAsSBigInt());
}

bool
static SQL_OperDoubleGreaterSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsDouble() > SQL_BIGINTToDouble(p_right.GetAsSBigInt());
}

bool
static SQL_OperBitGreaterSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBit() != 0 && p_right.GetAsSBigInt() == 0;
}

bool
static SQL_OperSTinyGreaterSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() > SQL_SBIGINTToTinyInt(p_right.GetAsSBigInt());
}

bool
static SQL_OperUTinyGreaterSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() > SQL_SBIGINTToUTinyInt(p_right.GetAsSBigInt());
}

bool
static SQL_OperSBigGreaterSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSBigInt() > p_right.GetAsSBigInt();
}

bool
static SQL_OperUBigGreaterSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUBigInt() > SQL_SBIGINTToUBIGINT(p_right.GetAsSBigInt());
}

bool   
static SQL_OperNumGreaterSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBCD().AsInt64() > p_right.GetAsSBigInt();
}

// UNSIGNED BIGINT

bool
static SQL_OperSShortGreaterUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSShort() > SQL_UBIGINTToShort(p_right.GetAsUBigInt());
}

bool
static SQL_OperUShortGreaterUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUShort() > SQL_UBIGINTToUShort(p_right.GetAsUBigInt());
}

bool
static SQL_OperSLongGreaterUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSLong() > SQL_UBIGINTToLong(p_right.GetAsUBigInt());
}

bool
static SQL_OperULongGreaterUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsULong() > SQL_UBIGINTToULong(p_right.GetAsUBigInt());
}

bool
static SQL_OperFloatGreaterUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsFloat() > SQL_UBIGINTToFloat(p_right.GetAsUBigInt());
}

bool
static SQL_OperDoubleGreaterUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsDouble() > SQL_UBIGINTToDouble(p_right.GetAsUBigInt());
}

bool
static SQL_OperBitGreaterUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBit() != 0 && p_right.GetAsUBigInt() == 0;
}

bool
static SQL_OperSTinyGreaterUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() > SQL_UBIGINTToUTinyInt(p_right.GetAsUBigInt());
}

bool
static SQL_OperUTinyGreaterUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() > SQL_UBIGINTToUTinyInt(p_right.GetAsUBigInt());
}

bool
static SQL_OperSBigGreaterUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SBIGINTToUBIGINT(p_left.GetAsSBigInt()) > p_right.GetAsUBigInt();
}

bool
static SQL_OperUBigGreaterUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUBigInt() > p_right.GetAsUBigInt();
}

bool
static SQL_OperNumGreaterUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBCD().AsUInt64() > p_right.GetAsUBigInt();
}

// NUMERIC

bool
static SQL_OperSShortGreaterNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSShort() > (short) p_right.GetAsBCD().AsLong();
}

bool
static SQL_OperUShortGreaterNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUShort() > (unsigned short) p_right.GetAsBCD().AsLong();
}

bool
static SQL_OperSLongGreaterNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSLong() > p_right.GetAsBCD().AsLong();
}

bool
static SQL_OperULongGreaterNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsULong() > (unsigned long) p_right.GetAsBCD().AsInt64();
}

bool
static SQL_OperFloatGreaterNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsFloat() > (float) p_right.GetAsBCD().AsDouble();
}

bool
static SQL_OperDoubleGreaterNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsDouble() > p_right.GetAsBCD().AsDouble();
}

bool
static SQL_OperBitGreaterNum(SQLVariant& p_left,SQLVariant& p_right)
{
  int ri = p_right.GetAsBCD().AsLong();
  return p_left.GetAsBit() != 0 && ri == 0;
}

bool
static SQL_OperSTinyGreaterNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() > SQL_SLongToTinyInt(p_right.GetAsBCD().AsLong());
}

bool
static SQL_OperUTinyGreaterNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() > SQL_SLongToUTinyInt(p_right.GetAsBCD().AsLong());
}

bool
static SQL_OperSBigGreaterNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSBigInt() > p_right.GetAsBCD().AsInt64();
}

bool
static SQL_OperUBigGreaterNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUBigInt() > p_right.GetAsBCD().AsUInt64();
}

bool
static SQL_OperNumGreaterNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBCD() > p_right.GetAsBCD();
}

// TYPE == GUID

bool
static SQL_OperGuidGreaterGuid(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLGUID* left  = p_left .GetAsGUID();
  SQLGUID* right = p_right.GetAsGUID();

  return memcmp(left,right,sizeof(SQLGUID)) > 0;
}

// TYPE == BINARY

bool
static SQL_OperBinaryGreaterBinary(SQLVariant& p_left,SQLVariant& p_right)
{
  void* left   = p_left .GetAsBinary();
  void* right  = p_right.GetAsBinary();
  // Take longest of two binaries in comparison
  long  length = p_left.GetBinaryLength() > p_right.GetBinaryLength() ? p_left.GetBinaryLength() : p_right.GetBinaryLength();

  return memcmp(left,right,length) > 0;
}

// TYPE == DATE

bool
static SQL_OperDateGreaterDate(SQLVariant& p_left,SQLVariant& p_right)
{
  // Relies on the > operator of SQLDate
  return p_left.GetAsSQLDate() > p_right.GetAsSQLDate();
}

bool
static SQL_OperStampGreaterDate(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLDate date = p_left.GetAsSQLTimestamp().AsSQLDate();
  return date > p_right.GetAsSQLDate();
}

// TYPE == TIME

bool
static SQL_OperTimeGreaterTime(SQLVariant& p_left,SQLVariant& p_right)
{
  // Relies on the > operator of SQLTime
  return p_left.GetAsSQLTime() > p_right.GetAsSQLTime();
}

bool
static SQL_OperStampGreaterTime(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLTime time = p_left.GetAsSQLTimestamp().AsSQLTime();
  return time > p_right.GetAsSQLTime();
}

// TYPE == TIMESTAMP

bool
static SQL_OperStampGreaterStamp(SQLVariant& p_left,SQLVariant& p_right)
{
  // Relies on the > operator of SQLTimestamp
  return p_left.GetAsSQLTimestamp() > p_right.GetAsSQLTimestamp();
}

// TYPE == INTERVAL_YEAR_MONTH

bool
static SQL_OperIntYMGreaterIntYM(SQLVariant& p_left,SQLVariant& p_right)
{
  // Relies on the calculation of months in SQLInterval
  return p_left.GetAsSQLInterval().AsDatabaseDouble() > p_right.GetAsSQLInterval().AsDatabaseDouble();
}

// TYPE == INTERVAL_DAY_SECOND

bool
static SQL_OperIntDSGreaterIntDS(SQLVariant& p_left,SQLVariant& p_right)
{
  // Relies on the calculation of seconds in SQLInterval
  return p_left.GetAsSQLInterval().AsDatabaseDouble() > p_right.GetAsSQLInterval().AsDatabaseDouble();
}

// OPERATOR ARRAY

static CompareFunctionArray OperatorGreater[CT_LAST][CT_LAST] =
{
                         // CT_CHAR                  CT_SSHORT                    CT_USHORT                    CT_SLONG                     CT_ULONG                    CT_FLOAT                    CT_DOUBLE                    CT_BIT                    CT_STINYINT                 CT_UTINYINT                 CT_SBIGINT                 CT_UBIGINT                 CT_NUMERIC                CT_GUID                  CT_BINARY                    CT_DATE                   CT_TIME                   CT_TIMESTAMP               CT_INTERVAL_YM             CT_INTERVAL_DS
                         // ------------------------ ---------------------------- ---------------------------- ---------------------------- --------------------------- --------------------------- ---------------------------- ------------------------- --------------------------- --------------------------- -------------------------- -------------------------- ------------------------- ------------------------ ---------------------------- ------------------------- ------------------------- -------------------------- -------------------------- ----------------------------
  /* CT_CHAR        */   {  &SQL_OperVarGreaterChar ,&SQL_OperVarGreaterChar     ,&SQL_OperVarGreaterChar     ,&SQL_OperVarGreaterChar     ,&SQL_OperVarGreaterChar    ,&SQL_OperVarGreaterChar    ,&SQL_OperVarGreaterChar     ,&SQL_OperVarGreaterChar  ,&SQL_OperVarGreaterChar    ,&SQL_OperVarGreaterChar    ,&SQL_OperVarGreaterChar   ,&SQL_OperVarGreaterChar   ,&SQL_OperVarGreaterChar  ,&SQL_OperVarGreaterChar ,nullptr                     ,&SQL_OperVarGreaterChar  ,&SQL_OperVarGreaterChar  ,&SQL_OperVarGreaterChar   ,&SQL_OperVarGreaterChar   ,&SQL_OperVarGreaterChar    }
  /* CT_SSHORT      */  ,{  &SQL_OperVarGreaterChar ,&SQL_OperSShortGreaterSShort,&SQL_OperSShortGreaterUShort,&SQL_OperSShortGreaterSLong ,&SQL_OperSShortGreaterULong,&SQL_OperSShortGreaterFloat,&SQL_OperSShortGreaterDouble,&SQL_OperSShortGreaterBit,&SQL_OperSShortGreaterSTiny,&SQL_OperSShortGreaterUTiny,&SQL_OperSShortGreaterSBig,&SQL_OperSShortGreaterUBig,&SQL_OperSShortGreaterNum,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_USHORT      */  ,{  &SQL_OperVarGreaterChar ,&SQL_OperUShortGreaterSShort,&SQL_OperUShortGreaterUShort,&SQL_OperUShortGreaterSLong ,&SQL_OperUShortGreaterULong,&SQL_OperUShortGreaterFloat,&SQL_OperUShortGreaterDouble,&SQL_OperUShortGreaterBit,&SQL_OperUShortGreaterSTiny,&SQL_OperUShortGreaterUTiny,&SQL_OperUShortGreaterSBig,&SQL_OperUShortGreaterUBig,&SQL_OperUShortGreaterNum,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_SLONG       */  ,{  &SQL_OperVarGreaterChar ,&SQL_OperSLongGreaterSShort ,&SQL_OperSLongGreaterUShort ,&SQL_OperSLongGreaterSLong  ,&SQL_OperSLongGreaterULong ,&SQL_OperSLongGreaterFloat ,&SQL_OperSLongGreaterDouble ,&SQL_OperSLongGreaterBit ,&SQL_OperSLongGreaterSTiny ,&SQL_OperSLongGreaterUTiny ,&SQL_OperSLongGreaterSBig ,&SQL_OperSLongGreaterUBig ,&SQL_OperSLongGreaterNum ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_ULONG       */  ,{  &SQL_OperVarGreaterChar ,&SQL_OperULongGreaterSShort ,&SQL_OperUlongGreaterUShort ,&SQL_OperULongGreaterSLong  ,&SQL_OperULongGreaterULong ,&SQL_OperULongGreaterFloat ,&SQL_OperULongGreaterDouble ,&SQL_OperULongGreaterBit ,&SQL_OperULongGreaterSTiny ,&SQL_OperULongGreaterUTiny ,&SQL_OperULongGreaterSBig ,&SQL_OperULongGreaterUBig ,&SQL_OperULongGreaterNum ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_FLOAT       */  ,{  &SQL_OperVarGreaterChar ,&SQL_OperFloatGreaterSShort ,&SQL_OperFloatGreaterUShort ,&SQL_OperFloatGreaterSLong  ,&SQL_OperFloatGreaterULong ,&SQL_OperFloatGreaterFloat ,&SQL_OperFloatGreaterDouble ,&SQL_OperFloatGreaterBit ,&SQL_OperFloatGreaterSTiny ,&SQL_OperFloatGreaterUTiny ,&SQL_OperFloatGreaterSBig ,&SQL_OperFloatGreaterUBig ,&SQL_OperFloatGreaterNum ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_DOUBLE      */  ,{  &SQL_OperVarGreaterChar ,&SQL_OperDoubleGreaterSShort,&SQL_OperDoubleGreaterUShort,&SQL_OperDoubleEGreaterSLong,&SQL_OperDoubleGreaterULong,&SQL_OperDoubleGreaterFloat,&SQL_OperDoubleGreaterDouble,&SQL_OperDoubleGreaterBit,&SQL_OperDoubleGreaterSTiny,&SQL_OperDoubleGreaterUTiny,&SQL_OperDoubleGreaterSBig,&SQL_OperDoubleGreaterUBig,&SQL_OperDoubleGreaterNum,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_BIT         */  ,{  &SQL_OperVarGreaterChar ,&SQL_OperBitGreaterSShort   ,&SQL_OperBitGreaterUShort   ,&SQL_OperBitGreaterSLong    ,&SQL_OperBitGreaterULong   ,&SQL_OperBitGreaterFloat   ,&SQL_OperBitGreaterDouble   ,&SQL_OperBitGreaterBit   ,&SQL_OperBitGreaterSTiny   ,&SQL_OperBitGreaterUTiny   ,&SQL_OperBitGreaterSBig   ,&SQL_OperBitGreaterUBig   ,&SQL_OperBitGreaterNum   ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_STINYINT    */  ,{  &SQL_OperVarGreaterChar ,&SQL_OperSTinyGreaterSShort ,&SQL_OperSTinyGreaterUShort ,&SQL_OperSTinyGreaterSLong  ,&SQL_OperSTinyGreaterULong ,&SQL_OperSTinyGreaterFloat ,&SQL_OperSTinyGreaterDouble ,&SQL_OperSTinyGreaterBit ,&SQL_OperSTinyGreaterSTiny ,&SQL_OperSTinyGreaterUTiny ,&SQL_OperSTinyGreaterSBig ,&SQL_OperSTinyGreaterUBig ,&SQL_OperSTinyGreaterNum ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_UTINYINT    */  ,{  &SQL_OperVarGreaterChar ,&SQL_OperUTinyGreaterSShort ,&SQL_OperUTinyGreaterUShort ,&SQL_OperUTinyGreaterSLong  ,&SQL_OperUTinyGreaterULong ,&SQL_OperUTinyGreaterFloat ,&SQL_OperUTinyGreaterDouble ,&SQL_OperUTinyGreaterBit ,&SQL_OperUTinyGreaterSTiny ,&SQL_OperUTinyGreaterUTiny ,&SQL_OperUTinyGreaterSBig ,&SQL_OperUTinyGreaterUBig ,&SQL_OperUTinyGreaterNum ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_SBIGINT     */  ,{  &SQL_OperVarGreaterChar ,&SQL_OperSBigGreaterSShort  ,&SQL_OperSBigGreaterUShort  ,&SQL_OperSBigGreaterSLong   ,&SQL_OperSBigGreaterULong  ,&SQL_OperSBigGreaterFloat  ,&SQL_OperSBigGreaterDouble  ,&SQL_OperSBigGreaterBit  ,&SQL_OperSBigGreaterSTiny  ,&SQL_OperSBigGreaterUTiny  ,&SQL_OperSBigGreaterSBig  ,&SQL_OperSBigGreaterUBig  ,&SQL_OperSBigGreaterNum  ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_UBIGINT     */  ,{  &SQL_OperVarGreaterChar ,&SQL_OperUBigGreaterSShort  ,&SQL_OperUBigGreaterUShort  ,&SQL_OperUBigGreaterSLong   ,&SQL_OperUBigGreaterULong  ,&SQL_OperUBigGreaterFloat  ,&SQL_OperUBigGreaterDouble  ,&SQL_OperUBigGreaterBit  ,&SQL_OperUBigGreaterSTiny  ,&SQL_OperUBigGreaterUTiny  ,&SQL_OperUBigGreaterSBig  ,&SQL_OperUBigGreaterUBig  ,&SQL_OperUBigGreaterNum  ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_NUMERIC     */  ,{  &SQL_OperVarGreaterChar ,&SQL_OperNumGreaterSShort   ,&SQL_OperNumGreaterUShort   ,&SQL_OperNumGreaterSLong    ,&SQL_OperNumGreaterULong   ,&SQL_OperNumGreaterFloat   ,&SQL_OperNumGreaterDouble   ,&SQL_OperNumGreaterBit   ,&SQL_OperNumGreaterSTiny   ,&SQL_OperNumGreaterUTiny   ,&SQL_OperNumGreaterSBig   ,&SQL_OperNumGreaterUBig   ,&SQL_OperNumGreaterNum   ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_GUID        */  ,{  &SQL_OperGuidGreaterChar,nullptr                     ,nullptr                     ,nullptr                     ,nullptr                    ,nullptr                    ,nullptr                     ,nullptr                  ,nullptr                    ,nullptr                    ,nullptr                   ,nullptr                   ,nullptr                  ,&SQL_OperGuidGreaterGuid,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_BINARY      */  ,{  nullptr                 ,nullptr                     ,nullptr                     ,nullptr                     ,nullptr                    ,nullptr                    ,nullptr                     ,nullptr                  ,nullptr                    ,nullptr                    ,nullptr                   ,nullptr                   ,nullptr                  ,nullptr                 ,&SQL_OperBinaryGreaterBinary,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_DATE        */  ,{  &SQL_OperVarGreaterChar ,nullptr                     ,nullptr                     ,nullptr                     ,nullptr                    ,nullptr                    ,nullptr                     ,nullptr                  ,nullptr                    ,nullptr                    ,nullptr                   ,nullptr                   ,nullptr                  ,nullptr                 ,nullptr                     ,&SQL_OperDateGreaterDate ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_TIME        */  ,{  &SQL_OperVarGreaterChar ,nullptr                     ,nullptr                     ,nullptr                     ,nullptr                    ,nullptr                    ,nullptr                     ,nullptr                  ,nullptr                    ,nullptr                    ,nullptr                   ,nullptr                   ,nullptr                  ,nullptr                 ,nullptr                     ,nullptr                  ,&SQL_OperTimeGreaterTime ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_TIMESTAMP   */  ,{  &SQL_OperVarGreaterChar ,nullptr                     ,nullptr                     ,nullptr                     ,nullptr                    ,nullptr                    ,nullptr                     ,nullptr                  ,nullptr                    ,nullptr                    ,nullptr                   ,nullptr                   ,nullptr                  ,nullptr                 ,nullptr                     ,&SQL_OperStampGreaterDate,&SQL_OperStampGreaterTime,&SQL_OperStampGreaterStamp,nullptr                   ,nullptr                    }
  /* CT_INTERVAL_YM */  ,{  &SQL_OperVarGreaterChar ,nullptr                     ,nullptr                     ,nullptr                     ,nullptr                    ,nullptr                    ,nullptr                     ,nullptr                  ,nullptr                    ,nullptr                    ,nullptr                   ,nullptr                   ,nullptr                  ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,&SQL_OperIntYMGreaterIntYM,nullptr                    }
  /* CT_INTERVAL_DS */  ,{  &SQL_OperVarGreaterChar ,nullptr                     ,nullptr                     ,nullptr                     ,nullptr                    ,nullptr                    ,nullptr                     ,nullptr                  ,nullptr                    ,nullptr                    ,nullptr                   ,nullptr                   ,nullptr                  ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,&SQL_OperIntDSGreaterIntDS }
};

// Greater operator for SQLVariant
bool
SQLVariant::operator>(SQLVariant& p_right)
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
  OperatorCompare function = OperatorGreater[left][right].function;
  if(function)
  {
    return (*function)(*this,p_right);
  }
  // No compare function found
  // Data types are not comparable
  XString leftType  = FindDatatype(m_datatype);
  XString rightType = FindDatatype(p_right.m_datatype);
  XString error;
  error.Format("Cannot do the greater operator on (%s > %s)",leftType.GetString(),rightType.GetString());
  throw StdException(error);
}

// End of namespace
}
