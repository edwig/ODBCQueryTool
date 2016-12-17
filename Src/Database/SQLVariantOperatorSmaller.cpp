////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantOperatorSmaller.cpp
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
#include "SQLVariantTrim.h"
#include "SQLVariantOperator.h"
#include "SQLDate.h"
#include "bcd.h"

//////////////////////////////////////////////////////////////////////////
//
// ALL EQUALITY OPERATOR FUNCTIONS
//
//////////////////////////////////////////////////////////////////////////

bool
SQL_OperVarSmallerChar(SQLVariant& p_left,SQLVariant& p_right)
{
  CString leftString,rightString;
  p_left .GetAsString(leftString);
  p_right.GetAsString(rightString);

  return leftString.Compare(rightString) < 0;
}

bool
SQL_OperGuidSmallerChar(SQLVariant& p_left,SQLVariant& p_right)
{
  CString leftString,rightString;
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
SQL_OperSShortSmallerSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSShort() < p_right.GetAsSShort();
}

bool
SQL_OperUShortSmallerSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUShort() < SQL_ShortToUShort(p_right.GetAsSShort());
}

bool
SQL_OperSLongSmallerSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SLongToShort(p_left.GetAsSLong()) < p_right.GetAsSShort();
}

bool
SQL_OperULongSmallerSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_ULongToUShort(p_left.GetAsULong()) < SQL_ShortToUShort(p_right.GetAsSShort());
}

bool
SQL_OperFloatSmallerSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_FloatToShort(p_left.GetAsFloat()) < p_right.GetAsSShort();
}

bool
SQL_OperDoubleSmallerSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_DoubleToShort(p_left.GetAsDouble()) < p_right.GetAsSShort();
}

bool
SQL_OperBitSmallerSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBit() == 0 && p_right.GetAsSShort() != 0;
}

bool   
SQL_OperSTinySmallerSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() < SQL_ShortToTinyInt(p_right.GetAsSShort());
}

bool
SQL_OperUTinySmallerSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() < SQL_ShortToUTinyInt(p_right.GetAsSShort());
}

bool
SQL_OperSBigSmallerSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_BIGINTToShort(p_left.GetAsSBigInt()) < p_right.GetAsSShort();
}

bool
SQL_OperUBigSmallerSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_USQL_BIGINTToShort(p_left.GetAsUBigInt()) < p_right.GetAsSShort();
}

bool
SQL_OperNumSmallerSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SLongToShort(p_left.GetAsBCD().AsLong()) < p_right.GetAsSShort();
}

// UNSIGNED SHORT

bool   
SQL_OperSShortSmallerUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_ShortToUShort(p_left.GetAsSShort()) < p_right.GetAsUShort();
}

bool   
SQL_OperUShortSmallerUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUShort() < p_right.GetAsUShort();
}

bool
SQL_OperSLongSmallerUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SLongToUShort(p_left.GetAsSLong()) < p_right.GetAsUShort();
}

bool
SQL_OperUlongSmallerUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_ULongToUShort(p_left.GetAsULong()) < p_right.GetAsUShort();
}

bool
SQL_OperFloatSmallerUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_FloatToUShort(p_left.GetAsFloat()) < p_right.GetAsUShort();
}

bool
SQL_OperDoubleSmallerUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_DoubleToUShort(p_left.GetAsDouble()) < p_right.GetAsUShort();
}

bool
SQL_OperBitSmallerUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBit() == 0 && p_right.GetAsUShort() != 0;
}

bool
SQL_OperSTinySmallerUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() < SQL_UShortToTinyInt(p_right.GetAsUShort());
}

bool
SQL_OperUTinySmallerUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() < SQL_UShortToUTinyInt(p_right.GetAsUShort());
}

bool
SQL_OperSBigSmallerUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_BIGINTToUShort(p_left.GetAsSBigInt()) < p_right.GetAsUShort();
}

bool
SQL_OperUBigSmallerUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_UBIGINTToUShort(p_left.GetAsUBigInt()) < p_right.GetAsUShort();
}

bool
SQL_OperNumSmallerUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SLongToUShort(p_left.GetAsBCD().AsLong()) < p_right.GetAsUShort();
}

// SIGNED LONG

bool   
SQL_OperSShortSmallerSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSShort() < SQL_SLongToShort(p_right.GetAsSLong());
}

bool
SQL_OperUShortSmallerSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUShort() < SQL_SLongToUShort(p_right.GetAsSLong());
}

bool
SQL_OperSLongSmallerSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSLong() < p_right.GetAsSLong();
}

bool
SQL_OperULongSmallerSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsULong() < SQL_LongToULong(p_right.GetAsSLong());
}

bool
SQL_OperFloatSmallerSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_FloatToLong(p_left.GetAsFloat()) < p_right.GetAsSLong();
}

bool
SQL_OperDoubleESmallerSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_DoubleToLong(p_left.GetAsDouble()) < p_right.GetAsSLong();
}

bool
SQL_OperBitSmallerSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBit() == 0 && p_right.GetAsSLong() != 0;
}

bool
SQL_OperSTinySmallerSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() < SQL_SLongToTinyInt(p_right.GetAsSLong());
}

bool
SQL_OperUTinySmallerSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() < SQL_SLongToUTinyInt(p_right.GetAsSLong());
}

bool
SQL_OperSBigSmallerSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_BIGINTToLong(p_left.GetAsSBigInt()) < p_right.GetAsSLong();
}

bool
SQL_OperUBigSmallerSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_UBIGINTToLong(p_left.GetAsUBigInt()) < p_right.GetAsSLong();
}

bool
SQL_OperNumSmallerSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBCD().AsLong() < p_right.GetAsSLong();
}

// UNSIGNED LONG

bool   
SQL_OperSShortSmallerULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_ShortToUShort(p_left.GetAsSShort()) < SQL_ULongToUShort(p_right.GetAsULong());
}

bool
SQL_OperUShortSmallerULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUShort() < SQL_ULongToUShort(p_right.GetAsULong());
}

bool
SQL_OperSLongSmallerULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_LongToULong(p_left.GetAsSLong()) < p_right.GetAsULong();
}

bool
SQL_OperULongSmallerULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsULong() < p_right.GetAsULong();
}

bool
SQL_OperFloatSmallerULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_FloatToULong(p_left.GetAsFloat()) < p_right.GetAsULong();
}

bool
SQL_OperDoubleSmallerULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_DoubleToULong(p_left.GetAsDouble()) < p_right.GetAsULong();
}

bool
SQL_OperBitSmallerULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBit() == 0 && p_right.GetAsULong() != 0;
}

bool
SQL_OperSTinySmallerULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_TinyIntToUTinyInt(p_left.GetAsSTinyInt()) < SQL_ULongToUTinyInt(p_right.GetAsULong());
}

bool
SQL_OperUTinySmallerULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() < SQL_ULongToUTinyInt(p_right.GetAsULong());
}

bool
SQL_OperSBigSmallerULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_BIGINTToULong(p_left.GetAsSBigInt()) < p_right.GetAsULong();
}

bool
SQL_OperUBigSmallerULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_UBIGINTToULong(p_left.GetAsUBigInt()) < p_right.GetAsULong();
}

bool
SQL_OperNumSmallerULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_BIGINTToULong(p_left.GetAsBCD().AsInt64()) < p_right.GetAsULong();
}

// FLOAT

bool   
SQL_OperSShortSmallerFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSShort() < SQL_FloatToShort(p_right.GetAsFloat());
}

bool   
SQL_OperUShortSmallerFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUShort() < SQL_FloatToUShort(p_right.GetAsFloat());
}

bool
SQL_OperSLongSmallerFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSLong() < SQL_FloatToLong(p_right.GetAsFloat());
}

bool
SQL_OperULongSmallerFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsULong() < SQL_FloatToULong(p_right.GetAsFloat());
}

bool
SQL_OperFloatSmallerFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsFloat() < p_right.GetAsFloat();
}

bool
SQL_OperDoubleSmallerFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_DoubleToFloat(p_left.GetAsDouble()) < p_right.GetAsFloat();
}

bool
SQL_OperBitSmallerFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBit() == 0 && p_right.GetAsFloat() != 0.0;
}

bool
SQL_OperSTinySmallerFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() < SQL_FloatToTinyInt(p_right.GetAsFloat());
}

bool
SQL_OperUTinySmallerFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() < SQL_FloatToUTinyInt(p_right.GetAsFloat());
}

bool
SQL_OperSBigSmallerFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSBigInt() < SQL_FloatToBIGINT(p_right.GetAsFloat());
}

bool
SQL_OperUBigSmallerFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUBigInt() < SQL_FloatToUBIGINT(p_right.GetAsFloat());
}

bool
SQL_OperNumSmallerFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBCD().AsDouble() < (double) p_right.GetAsFloat();
}

// DOUBLE

bool
SQL_OperSShortSmallerDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSShort() < SQL_DoubleToShort(p_right.GetAsDouble());
}

bool
SQL_OperUShortSmallerDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUShort() < SQL_DoubleToUShort(p_right.GetAsDouble());
}

bool
SQL_OperSLongSmallerDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSLong() < SQL_DoubleToLong(p_right.GetAsDouble());
}

bool
SQL_OperULongSmallerDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsULong() < SQL_DoubleToULong(p_right.GetAsDouble());
}

bool
SQL_OperFloatSmallerDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsFloat() < SQL_DoubleToFloat(p_right.GetAsDouble());
}

bool
SQL_OperDoubleSmallerDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsDouble() < p_right.GetAsDouble();
}

bool
SQL_OperBitSmallerDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBit() == 0 && p_right.GetAsDouble() != 0.0;
}

bool
SQL_OperSTinySmallerDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() < SQL_DoubleToTinyInt(p_right.GetAsDouble());
}

bool
SQL_OperUTinySmallerDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() < SQL_DoubleToUTinyInt(p_right.GetAsDouble());
}

bool
SQL_OperSBigSmallerDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSBigInt() < SQL_DoubleToBIGINT(p_right.GetAsDouble());
}

bool
SQL_OperUBigSmallerDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUBigInt() < SQL_DoubleToUBIGINT(p_right.GetAsDouble());
}

bool
SQL_OperNumSmallerDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBCD().AsDouble() < p_right.GetAsDouble();
}

// BIT

bool   
SQL_OperSShortSmallerBit(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSShort() == 0 && p_right.GetAsBit() != 0;
}

bool
SQL_OperUShortSmallerBit(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUShort() == 0 && p_right.GetAsBit() != 0;
}

bool   
SQL_OperSLongSmallerBit(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSLong() == 0 && p_right.GetAsBit() != 0;
}

bool
SQL_OperULongSmallerBit(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsULong() == 0 && p_right.GetAsBit() != 0;
}

bool
SQL_OperFloatSmallerBit(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsFloat() == 0.0 && p_right.GetAsBit() != 0;
}

bool
SQL_OperDoubleSmallerBit(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsDouble() == 0.0 && p_right.GetAsBit() != 0;
}

bool
SQL_OperBitSmallerBit(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBit() < p_right.GetAsBit();
}

bool
SQL_OperSTinySmallerBit(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() == 0 && p_right.GetAsBit() != 0;
}

bool
SQL_OperUTinySmallerBit(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() == 0 && p_right.GetAsBit() != 0;
}

bool
SQL_OperSBigSmallerBit(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSBigInt() == 0 && p_right.GetAsBit() != 0;
}

bool
SQL_OperUBigSmallerBit(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUBigInt() == 0 && p_right.GetAsBit() != 0;
}

bool
SQL_OperNumSmallerBit(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBCD().IsNull() && p_right.GetAsBit() != 0;
}

// SIGNED TINYINT

bool
SQL_OperSShortSmallerSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_ShortToTinyInt(p_left.GetAsSShort()) < p_right.GetAsSTinyInt();
}

bool
SQL_OperUShortSmallerSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_UShortToTinyInt(p_left.GetAsUShort()) < p_right.GetAsSTinyInt();
}

bool
SQL_OperSLongSmallerSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SLongToTinyInt(p_left.GetAsSLong()) < p_right.GetAsSTinyInt();
}

bool
SQL_OperULongSmallerSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_ULongToTinyInt(p_left.GetAsULong()) < p_right.GetAsSTinyInt();
}

bool
SQL_OperFloatSmallerSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_FloatToTinyInt(p_left.GetAsFloat()) < p_right.GetAsSTinyInt();
}

bool
SQL_OperDoubleSmallerSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_DoubleToTinyInt(p_left.GetAsDouble()) < p_right.GetAsSTinyInt();
}

bool
SQL_OperBitSmallerSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBit() == 0 && p_right.GetAsSTinyInt() != 0;
}

bool
SQL_OperSTinySmallerSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() < p_right.GetAsSTinyInt();
}

bool
SQL_OperUTinySmallerSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() < SQL_TinyIntToUTinyInt(p_right.GetAsSTinyInt());
}

bool
SQL_OperSBigSmallerSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SBIGINTToTinyInt(p_left.GetAsSBigInt()) < p_right.GetAsSTinyInt();
}

bool
SQL_OperUBigSmallerSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_UBIGINTToTinyInt(p_left.GetAsUBigInt()) < p_right.GetAsSTinyInt();
}

bool
SQL_OperNumSmallerSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SLongToTinyInt(p_left.GetAsBCD().AsLong()) < p_right.GetAsSTinyInt();
}

// UNSIGNED TINYINT

bool
SQL_OperSShortSmallerUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_ShortToUTinyInt(p_left.GetAsSShort()) < p_right.GetAsUTinyInt();
}

bool
SQL_OperUShortSmallerUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_UShortToUTinyInt(p_left.GetAsUShort()) < p_right.GetAsUTinyInt();
}

bool
SQL_OperSLongSmallerUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SLongToUTinyInt(p_left.GetAsSLong()) < p_right.GetAsUTinyInt();
}

bool
SQL_OperULongSmallerUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_ULongToUTinyInt(p_left.GetAsULong()) < p_right.GetAsUTinyInt();
}

bool
SQL_OperFloatSmallerUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_FloatToUTinyInt(p_left.GetAsFloat()) < p_right.GetAsUTinyInt();
}

bool
SQL_OperDoubleSmallerUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_DoubleToUTinyInt(p_left.GetAsDouble()) < p_right.GetAsUTinyInt();
}

bool
SQL_OperBitSmallerUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBit() == 0 && p_right.GetAsUTinyInt() != 0;
}

bool
SQL_OperSTinySmallerUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_TinyIntToUTinyInt(p_left.GetAsSTinyInt()) < p_right.GetAsUTinyInt();
}

bool
SQL_OperUTinySmallerUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() < p_right.GetAsUTinyInt();
}

bool
SQL_OperSBigSmallerUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SBIGINTToUTinyInt(p_left.GetAsSBigInt()) < p_right.GetAsUTinyInt();
}

bool
SQL_OperUBigSmallerUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_UBIGINTToUTinyInt(p_left.GetAsUBigInt()) < p_right.GetAsUTinyInt();
}

bool
SQL_OperNumSmallerUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SLongToUTinyInt(p_left.GetAsBCD().AsLong()) < p_right.GetAsUTinyInt();
}

// SIGNED BIGINT

bool
SQL_OperSShortSmallerSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSShort() < SQL_BIGINTToShort(p_right.GetAsSBigInt());
}

bool
SQL_OperUShortSmallerSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUShort() < SQL_BIGINTToUShort(p_right.GetAsSBigInt());
}

bool
SQL_OperSLongSmallerSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSLong() < SQL_BIGINTToLong(p_right.GetAsSBigInt());
}

bool
SQL_OperULongSmallerSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsULong() < SQL_BIGINTToULong(p_right.GetAsSBigInt());
}

bool
SQL_OperFloatSmallerSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsFloat() < SQL_BIGINTToFloat(p_right.GetAsSBigInt());
}

bool
SQL_OperDoubleSmallerSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsDouble() < SQL_BIGINTToDouble(p_right.GetAsSBigInt());
}

bool
SQL_OperBitSmallerSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBit() == 0 && p_right.GetAsSBigInt() != 0;
}

bool
SQL_OperSTinySmallerSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() < SQL_SBIGINTToTinyInt(p_right.GetAsSBigInt());
}

bool
SQL_OperUTinySmallerSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() < SQL_SBIGINTToUTinyInt(p_right.GetAsSBigInt());
}

bool
SQL_OperSBigSmallerSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSBigInt() < p_right.GetAsSBigInt();
}

bool
SQL_OperUBigSmallerSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUBigInt() < SQL_SBIGINTToUBIGINT(p_right.GetAsSBigInt());
}

bool   
SQL_OperNumSmallerSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBCD().AsInt64() < p_right.GetAsSBigInt();
}

// UNSIGNED BIGINT

bool
SQL_OperSShortSmallerUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSShort() < SQL_USQL_BIGINTToShort(p_right.GetAsUBigInt());
}

bool
SQL_OperUShortSmallerUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUShort() < SQL_UBIGINTToUShort(p_right.GetAsUBigInt());
}

bool
SQL_OperSLongSmallerUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSLong() < SQL_UBIGINTToLong(p_right.GetAsUBigInt());
}

bool
SQL_OperULongSmallerUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsULong() < SQL_UBIGINTToULong(p_right.GetAsUBigInt());
}

bool
SQL_OperFloatSmallerUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsFloat() < SQL_UBIGINTToFloat(p_right.GetAsUBigInt());
}

bool
SQL_OperDoubleSmallerUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsDouble() < SQL_UBIGINTToDouble(p_right.GetAsUBigInt());
}

bool
SQL_OperBitSmallerUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBit() == 0 && p_right.GetAsUBigInt() != 0;
}

bool
SQL_OperSTinySmallerUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() < SQL_UBIGINTToUTinyInt(p_right.GetAsUBigInt());
}

bool
SQL_OperUTinySmallerUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() < SQL_UBIGINTToUTinyInt(p_right.GetAsUBigInt());
}

bool
SQL_OperSBigSmallerUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SBIGINTToUBIGINT(p_left.GetAsSBigInt()) < p_right.GetAsUBigInt();
}

bool
SQL_OperUBigSmallerUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUBigInt() < p_right.GetAsUBigInt();
}

bool
SQL_OperNumSmallerUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBCD().AsUInt64() < p_right.GetAsUBigInt();
}

// NUMERIC

bool
SQL_OperSShortSmallerNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSShort() < SQL_SLongToShort(p_right.GetAsBCD().AsLong());
}

bool
SQL_OperUShortSmallerNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUShort() < SQL_SLongToUShort(p_right.GetAsBCD().AsLong());
}

bool
SQL_OperSLongSmallerNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSLong() < p_right.GetAsBCD().AsLong();
}

bool
SQL_OperULongSmallerNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsULong() < SQL_BIGINTToULong(p_right.GetAsBCD().AsInt64());
}

bool
SQL_OperFloatSmallerNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsFloat() < (float) p_right.GetAsBCD().AsDouble();
}

bool
SQL_OperDoubleSmallerNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsDouble() < p_right.GetAsBCD().AsDouble();
}

bool
SQL_OperBitSmallerNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBit() == 0 && !p_right.GetAsBCD().IsNull();
}

bool
SQL_OperSTinySmallerNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() < SQL_SLongToTinyInt(p_right.GetAsBCD().AsLong());
}

bool
SQL_OperUTinySmallerNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() < SQL_SLongToUTinyInt(p_right.GetAsBCD().AsLong());
}

bool
SQL_OperSBigSmallerNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSBigInt() < p_right.GetAsBCD().AsInt64();
}

bool
SQL_OperUBigSmallerNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUBigInt() < p_right.GetAsBCD().AsUInt64();
}

bool
SQL_OperNumSmallerNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBCD() < p_right.GetAsBCD();
}

// TYPE == GUID

bool
SQL_OperGuidSmallerGuid(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLGUID* left  = p_left.GetAsGUID();
  SQLGUID* right = p_right.GetAsGUID();

  return memcmp(left,right,sizeof(SQLGUID)) < 0;
}

// TYPE == BINARY

bool
SQL_OperBinarySmallerBinary(SQLVariant& p_left,SQLVariant& p_right)
{
  void* left   = p_left.GetAsBinary();
  void* right  = p_right.GetAsBinary();
  // Take longest of two binaries in comparison
  long  length = p_left.GetBinaryLength() < p_right.GetBinaryLength() ? p_left.GetBinaryLength() : p_right.GetBinaryLength();

  return memcmp(left,right,length) < 0;
}

// TYPE == DATE

bool
SQL_OperDateSmallerDate(SQLVariant& p_left,SQLVariant& p_right)
{
  // Relies on the < operator of SQLDate
  return p_left.GetAsSQLDate() < p_right.GetAsSQLDate();
}

bool
SQL_OperStampSmallerDate(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLDate date = p_left.GetAsSQLTimestamp().AsSQLDate();
  return date < p_right.GetAsSQLDate();
}

// TYPE == TIME

bool
SQL_OperTimeSmallerTime(SQLVariant& p_left,SQLVariant& p_right)
{
  // Relies on the < operator of SQLTime
  return p_left.GetAsSQLTime() < p_right.GetAsSQLTime();
}

bool
SQL_OperStampSmallerTime(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLTime time = p_left.GetAsSQLTimestamp().AsSQLTime();
  return time < p_right.GetAsSQLTime();
}

// TYPE == TIMESTAMP

bool
SQL_OperStampSmallerStamp(SQLVariant& p_left,SQLVariant& p_right)
{
  // Relies on the < operator of SQLTimestamp
  return p_left.GetAsSQLTimestamp() < p_right.GetAsSQLTimestamp();
}

// TYPE == INTERVAL_YEAR_MONTH

bool
SQL_OperIntYMSmallerIntYM(SQLVariant& p_left,SQLVariant& p_right)
{
  // Relies on the calculation of months in SQLInterval
  return p_left.GetAsSQLInterval().AsDatabaseDouble() < p_right.GetAsSQLInterval().AsDatabaseDouble();
}

// TYPE == INTERVAL_DAY_SECOND

bool
SQL_OperIntDSSmallerIntDS(SQLVariant& p_left,SQLVariant& p_right)
{
  // Relies on the calculation of seconds in SQLInterval
  return p_left.GetAsSQLInterval().AsDatabaseDouble() < p_right.GetAsSQLInterval().AsDatabaseDouble();
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
SQLVariant::operator<(SQLVariant& p_right)
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
  CString leftType  = FindDatatype(m_datatype);
  CString rightType = FindDatatype(p_right.m_datatype);
  CString error;
  error.Format("Cannot do the Smaller operator on (%s < %s)",leftType,rightType);
  throw error;
}

