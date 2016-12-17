////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantOperatorGreatEqual.cpp
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
SQL_OperVarGreatEQChar(SQLVariant& p_left,SQLVariant& p_right)
{
  CString leftString,rightString;
  p_left .GetAsString(leftString);
  p_right.GetAsString(rightString);

  return leftString.Compare(rightString) >= 0;
}

bool
SQL_OperGuidGreatEQChar(SQLVariant& p_left,SQLVariant& p_right)
{
  CString leftString,rightString;
  p_left. GetAsString(leftString);
  p_right.GetAsString(rightString);

  leftString. TrimLeft ('{');
  leftString. TrimRight('}');
  rightString.TrimLeft ('}');
  rightString.TrimRight('{');

  return leftString.Compare(rightString) >= 0;
}

// SIGNED SHORT

bool
SQL_OperSShortGreatEQSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSShort() >= p_right.GetAsSShort();
}

bool
SQL_OperUShortGreatEQSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUShort() >= SQL_ShortToUShort(p_right.GetAsSShort());
}

bool
SQL_OperSLongGreatEQSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SLongToShort(p_left.GetAsSLong()) >= p_right.GetAsSShort();
}

bool
SQL_OperULongGreatEQSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_ULongToUShort(p_left.GetAsULong()) >= SQL_ShortToUShort(p_right.GetAsSShort());
}

bool
SQL_OperFloatGreatEQSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_FloatToShort(p_left.GetAsFloat()) >= p_right.GetAsSShort();
}

bool
SQL_OperDoubleGreatEQSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_DoubleToShort(p_left.GetAsDouble()) >= p_right.GetAsSShort();
}

bool
SQL_OperBitGreatEQSShort(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  return p_left.GetAsBit() != 0;
}

bool   
SQL_OperSTinyGreatEQSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() >= SQL_ShortToTinyInt(p_right.GetAsSShort());
}

bool
SQL_OperUTinyGreatEQSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() >= SQL_ShortToUTinyInt(p_right.GetAsSShort());
}

bool
SQL_OperSBigGreatEQSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_BIGINTToShort(p_left.GetAsSBigInt()) >= p_right.GetAsSShort();
}

bool
SQL_OperUBigGreatEQSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_USQL_BIGINTToShort(p_left.GetAsUBigInt()) >= p_right.GetAsSShort();
}

bool   
SQL_OperNumGreatEQSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SLongToShort(p_left.GetAsBCD().AsLong()) >= p_right.GetAsSShort();
}

// UNSIGNED SHORT

bool   
SQL_OperSShortGreatEQUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_ShortToUShort(p_left.GetAsSShort()) >= p_right.GetAsUShort();
}

bool   
SQL_OperUShortGreatEQUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUShort() >= p_right.GetAsUShort();
}

bool
SQL_OperSLongGreatEQUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SLongToUShort(p_left.GetAsSLong()) >= p_right.GetAsUShort();
}

bool
SQL_OperUlongGreatEQUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_ULongToUShort(p_left.GetAsULong()) >= p_right.GetAsUShort();
}

bool
SQL_OperFloatGreatEQUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_FloatToUShort(p_left.GetAsFloat()) >= p_right.GetAsUShort();
}

bool
SQL_OperDoubleGreatEQUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_DoubleToUShort(p_left.GetAsDouble()) >= p_right.GetAsUShort();
}

bool
SQL_OperBitGreatEQUShort(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  return p_left.GetAsBit() != 0;
}

bool
SQL_OperSTinyGreatEQUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() >= SQL_UShortToTinyInt(p_right.GetAsUShort());
}

bool
SQL_OperUTinyGreatEQUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() >= SQL_UShortToUTinyInt(p_right.GetAsUShort());
}

bool
SQL_OperSBigGreatEQUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_BIGINTToUShort(p_left.GetAsSBigInt()) >= p_right.GetAsUShort();
}

bool
SQL_OperUBigGreatEQUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_UBIGINTToUShort(p_left.GetAsUBigInt()) >= p_right.GetAsUShort();
}

bool
SQL_OperNumGreatEQUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SLongToUShort(p_left.GetAsBCD().AsLong()) >= p_right.GetAsUShort();
}

// SIGNED LONG

bool   
SQL_OperSShortGreatEQSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSShort() >= SQL_SLongToShort(p_right.GetAsSLong());
}

bool
SQL_OperUShortGreatEQSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUShort() >= SQL_SLongToUShort(p_right.GetAsSLong());
}

bool
SQL_OperSLongGreatEQSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSLong() >= p_right.GetAsSLong();
}

bool
SQL_OperULongGreatEQSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsULong() >= SQL_LongToULong(p_right.GetAsSLong());
}

bool
SQL_OperFloatGreatEQSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_FloatToLong(p_left.GetAsFloat()) >= p_right.GetAsSLong();
}

bool
SQL_OperDoubleEGreatEQSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_DoubleToLong(p_left.GetAsDouble()) >= p_right.GetAsSLong();
}

bool
SQL_OperBitGreatEQSLong(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  return p_left.GetAsBit() != 0;
}

bool
SQL_OperSTinyGreatEQSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() >= SQL_SLongToTinyInt(p_right.GetAsSLong());
}

bool
SQL_OperUTinyGreatEQSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() >= SQL_SLongToUTinyInt(p_right.GetAsSLong());
}

bool
SQL_OperSBigGreatEQSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_BIGINTToLong(p_left.GetAsSBigInt()) >= p_right.GetAsSLong();
}

bool
SQL_OperUBigGreatEQSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_UBIGINTToLong(p_left.GetAsUBigInt()) >= p_right.GetAsSLong();
}

bool   
SQL_OperNumGreatEQSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBCD().AsLong() >= p_right.GetAsSLong();
}

// UNSIGNED LONG

bool   
SQL_OperSShortGreatEQULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_ShortToUShort(p_left.GetAsSShort()) >= SQL_ULongToUShort(p_right.GetAsULong());
}

bool
SQL_OperUShortGreatEQULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUShort() >= SQL_ULongToUShort(p_right.GetAsULong());
}

bool
SQL_OperSLongGreatEQULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_LongToULong(p_left.GetAsSLong()) >= p_right.GetAsULong();
}

bool
SQL_OperULongGreatEQULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsULong() >= p_right.GetAsULong();
}

bool
SQL_OperFloatGreatEQULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_FloatToULong(p_left.GetAsFloat()) >= p_right.GetAsULong();
}

bool
SQL_OperDoubleGreatEQULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_DoubleToULong(p_left.GetAsDouble()) >= p_right.GetAsULong();
}

bool
SQL_OperBitGreatEQULong(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  return p_left.GetAsBit() != 0;
}

bool
SQL_OperSTinyGreatEQULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_TinyIntToUTinyInt(p_left.GetAsSTinyInt()) >= SQL_ULongToUTinyInt(p_right.GetAsULong());
}

bool
SQL_OperUTinyGreatEQULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() >= SQL_ULongToUTinyInt(p_right.GetAsULong());
}

bool
SQL_OperSBigGreatEQULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_BIGINTToULong(p_left.GetAsSBigInt()) >= p_right.GetAsULong();
}

bool
SQL_OperUBigGreatEQULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_UBIGINTToULong(p_left.GetAsUBigInt()) >= p_right.GetAsULong();
}

bool
SQL_OperNumGreatEQULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_BIGINTToULong(p_left.GetAsBCD().AsInt64()) >= p_right.GetAsULong();
}

// FLOAT

bool   
SQL_OperSShortGreatEQFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSShort() >= SQL_FloatToShort(p_right.GetAsFloat());
}

bool   
SQL_OperUShortGreatEQFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUShort() >= SQL_FloatToUShort(p_right.GetAsFloat());
}

bool
SQL_OperSLongGreatEQFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSLong() >= SQL_FloatToLong(p_right.GetAsFloat());
}

bool
SQL_OperULongGreatEQFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsULong() >= SQL_FloatToULong(p_right.GetAsFloat());
}

bool
SQL_OperFloatGreatEQFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsFloat() >= p_right.GetAsFloat();
}

bool
SQL_OperDoubleGreatEQFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_DoubleToFloat(p_left.GetAsDouble()) >= p_right.GetAsFloat();
}

bool
SQL_OperBitGreatEQFloat(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  return p_left.GetAsBit() != 0;
}

bool
SQL_OperSTinyGreatEQFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() >= SQL_FloatToTinyInt(p_right.GetAsFloat());
}

bool
SQL_OperUTinyGreatEQFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() >= SQL_FloatToUTinyInt(p_right.GetAsFloat());
}

bool
SQL_OperSBigGreatEQFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSBigInt() >= SQL_FloatToBIGINT(p_right.GetAsFloat());
}

bool
SQL_OperUBigGreatEQFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUBigInt() >= SQL_FloatToUBIGINT(p_right.GetAsFloat());
}

bool
SQL_OperNumGreatEQFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBCD().AsDouble() >= (double)p_right.GetAsFloat();
}

// DOUBLE

bool
SQL_OperSShortGreatEQDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSShort() >= SQL_DoubleToShort(p_right.GetAsDouble());
}

bool
SQL_OperUShortGreatEQDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUShort() >= SQL_DoubleToUShort(p_right.GetAsDouble());
}

bool
SQL_OperSLongGreatEQDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSLong() >= SQL_DoubleToLong(p_right.GetAsDouble());
}

bool
SQL_OperULongGreatEQDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsULong() >= SQL_DoubleToULong(p_right.GetAsDouble());
}

bool
SQL_OperFloatGreatEQDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsFloat() >= SQL_DoubleToFloat(p_right.GetAsDouble());
}

bool
SQL_OperDoubleGreatEQDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsDouble() >= p_right.GetAsDouble();
}

bool
SQL_OperBitGreatEQDouble(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  return p_left.GetAsBit() != 0;
}

bool
SQL_OperSTinyGreatEQDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() >= SQL_DoubleToTinyInt(p_right.GetAsDouble());
}

bool
SQL_OperUTinyGreatEQDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() >= SQL_DoubleToUTinyInt(p_right.GetAsDouble());
}

bool
SQL_OperSBigGreatEQDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSBigInt() >= SQL_DoubleToBIGINT(p_right.GetAsDouble());
}

bool
SQL_OperUBigGreatEQDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUBigInt() >= SQL_DoubleToUBIGINT(p_right.GetAsDouble());
}

bool
SQL_OperNumGreatEQDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBCD().AsDouble() >= p_right.GetAsDouble();
}

// BIT

bool   
SQL_OperSShortGreatEQBit(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  return p_left.GetAsSShort() != 0;
}

bool
SQL_OperUShortGreatEQBit(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  return p_left.GetAsUShort() > 0;
}

bool   
SQL_OperSLongGreatEQBit(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  return p_left.GetAsSLong() > 0;
}

bool
SQL_OperULongGreatEQBit(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  return p_left.GetAsULong() > 0;
}

bool
SQL_OperFloatGreatEQBit(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  return p_left.GetAsFloat() > 0.0;
}

bool
SQL_OperDoubleGreatEQBit(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  return p_left.GetAsDouble() > 0.0;
}

bool
SQL_OperBitGreatEQBit(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  return p_left.GetAsBit() > 0;
}

bool
SQL_OperSTinyGreatEQBit(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  return p_left.GetAsSTinyInt() > 0;
}

bool
SQL_OperUTinyGreatEQBit(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  return p_left.GetAsUTinyInt() > 0;
}

bool
SQL_OperSBigGreatEQBit(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  return p_left.GetAsSBigInt() > 0;
}

bool
SQL_OperUBigGreatEQBit(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  return p_left.GetAsUBigInt() > 0;
}

bool
SQL_OperNumGreatEQBit(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  return p_left.GetAsBCD().AsLong() != 0;
}

// SIGNED TINYINT

bool
SQL_OperSShortGreatEQSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_ShortToTinyInt(p_left.GetAsSShort()) >= p_right.GetAsSTinyInt();
}

bool
SQL_OperUShortGreatEQSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_UShortToTinyInt(p_left.GetAsUShort()) >= p_right.GetAsSTinyInt();
}

bool
SQL_OperSLongGreatEQSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SLongToTinyInt(p_left.GetAsSLong()) >= p_right.GetAsSTinyInt();
}

bool
SQL_OperULongGreatEQSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_ULongToTinyInt(p_left.GetAsULong()) >= p_right.GetAsSTinyInt();
}

bool
SQL_OperFloatGreatEQSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_FloatToTinyInt(p_left.GetAsFloat()) >= p_right.GetAsSTinyInt();
}

bool
SQL_OperDoubleGreatEQSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_DoubleToTinyInt(p_left.GetAsDouble()) >= p_right.GetAsSTinyInt();
}

bool
SQL_OperBitGreatEQSTiny(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  return p_left.GetAsBit() != 0;
}

bool
SQL_OperSTinyGreatEQSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() >= p_right.GetAsSTinyInt();
}

bool
SQL_OperUTinyGreatEQSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() >= SQL_TinyIntToUTinyInt(p_right.GetAsSTinyInt());
}

bool
SQL_OperSBigGreatEQSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SBIGINTToTinyInt(p_left.GetAsSBigInt()) >= p_right.GetAsSTinyInt();
}

bool
SQL_OperUBigGreatEQSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_UBIGINTToTinyInt(p_left.GetAsUBigInt()) >= p_right.GetAsSTinyInt();
}

bool
SQL_OperNumGreatEQSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SLongToTinyInt(p_left.GetAsBCD().AsLong()) >= p_right.GetAsSTinyInt();
}

// UNSIGNED TINYINT

bool
SQL_OperSShortGreatEQUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_ShortToUTinyInt(p_left.GetAsSShort()) >= p_right.GetAsUTinyInt();
}

bool
SQL_OperUShortGreatEQUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_UShortToUTinyInt(p_left.GetAsUShort()) >= p_right.GetAsUTinyInt();
}

bool
SQL_OperSLongGreatEQUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SLongToUTinyInt(p_left.GetAsSLong()) >= p_right.GetAsUTinyInt();
}

bool
SQL_OperULongGreatEQUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_ULongToUTinyInt(p_left.GetAsULong()) >= p_right.GetAsUTinyInt();
}

bool
SQL_OperFloatGreatEQUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_FloatToUTinyInt(p_left.GetAsFloat()) >= p_right.GetAsUTinyInt();
}

bool
SQL_OperDoubleGreatEQUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_DoubleToUTinyInt(p_left.GetAsDouble()) >= p_right.GetAsUTinyInt();
}

bool
SQL_OperBitGreatEQUTiny(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  return p_left.GetAsBit() != 0;
}

bool
SQL_OperSTinyGreatEQUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_TinyIntToUTinyInt(p_left.GetAsSTinyInt()) >= p_right.GetAsUTinyInt();
}

bool
SQL_OperUTinyGreatEQUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() >= p_right.GetAsUTinyInt();
}

bool
SQL_OperSBigGreatEQUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SBIGINTToUTinyInt(p_left.GetAsSBigInt()) >= p_right.GetAsUTinyInt();
}

bool
SQL_OperUBigGreatEQUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_UBIGINTToUTinyInt(p_left.GetAsUBigInt()) >= p_right.GetAsUTinyInt();
}

bool
SQL_OperNumGreatEQUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SLongToUTinyInt(p_left.GetAsBCD().AsLong()) >= p_right.GetAsUTinyInt();
}

// SIGNED BIGINT

bool
SQL_OperSShortGreatEQSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSShort() >= SQL_BIGINTToShort(p_right.GetAsSBigInt());
}

bool
SQL_OperUShortGreatEQSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUShort() >= SQL_BIGINTToUShort(p_right.GetAsSBigInt());
}

bool
SQL_OperSLongGreatEQSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSLong() >= SQL_BIGINTToLong(p_right.GetAsSBigInt());
}

bool
SQL_OperULongGreatEQSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsULong() >= SQL_BIGINTToULong(p_right.GetAsSBigInt());
}

bool
SQL_OperFloatGreatEQSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsFloat() >= SQL_BIGINTToFloat(p_right.GetAsSBigInt());
}

bool
SQL_OperDoubleGreatEQSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsDouble() >= SQL_BIGINTToDouble(p_right.GetAsSBigInt());
}

bool
SQL_OperBitGreatEQSBig(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  return p_left.GetAsBit() != 0;
}

bool
SQL_OperSTinyGreatEQSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() >= SQL_SBIGINTToTinyInt(p_right.GetAsSBigInt());
}

bool
SQL_OperUTinyGreatEQSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() >= SQL_SBIGINTToUTinyInt(p_right.GetAsSBigInt());
}

bool
SQL_OperSBigGreatEQSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSBigInt() >= p_right.GetAsSBigInt();
}

bool
SQL_OperUBigGreatEQSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUBigInt() >= SQL_SBIGINTToUBIGINT(p_right.GetAsSBigInt());
}

bool   
SQL_OperNumGreatEQSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBCD().AsInt64() >= p_right.GetAsSBigInt();
}

// UNSIGNED BIGINT

bool
SQL_OperSShortGreatEQUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSShort() >= SQL_USQL_BIGINTToShort(p_right.GetAsUBigInt());
}

bool
SQL_OperUShortGreatEQUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUShort() >= SQL_UBIGINTToUShort(p_right.GetAsUBigInt());
}

bool
SQL_OperSLongGreatEQUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSLong() >= SQL_UBIGINTToLong(p_right.GetAsUBigInt());
}

bool
SQL_OperULongGreatEQUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsULong() >= SQL_UBIGINTToULong(p_right.GetAsUBigInt());
}

bool
SQL_OperFloatGreatEQUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsFloat() >= SQL_UBIGINTToFloat(p_right.GetAsUBigInt());
}

bool
SQL_OperDoubleGreatEQUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsDouble() >= SQL_UBIGINTToDouble(p_right.GetAsUBigInt());
}

bool
SQL_OperBitGreatEQUBig(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  return p_left.GetAsBit() != 0;
}

bool
SQL_OperSTinyGreatEQUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() >= SQL_UBIGINTToUTinyInt(p_right.GetAsUBigInt());
}

bool
SQL_OperUTinyGreatEQUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() >= SQL_UBIGINTToUTinyInt(p_right.GetAsUBigInt());
}

bool
SQL_OperSBigGreatEQUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SBIGINTToUBIGINT(p_left.GetAsSBigInt()) >= p_right.GetAsUBigInt();
}

bool
SQL_OperUBigGreatEQUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUBigInt() >= p_right.GetAsUBigInt();
}

bool
SQL_OperNumGreatEQUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBCD().AsUInt64() >= p_right.GetAsUBigInt();
}

// NUMERIC

bool
SQL_OperSShortGreatEQNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSShort() >= SQL_SLongToShort(p_right.GetAsBCD().AsLong());
}

bool
SQL_OperUShortGreatEQNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUShort() >= SQL_SLongToUShort(p_right.GetAsBCD().AsLong());
}

bool
SQL_OperSLongGreatEQNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSLong() >= p_right.GetAsBCD().AsLong();
}

bool
SQL_OperULongGreatEQNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsULong() >= SQL_BIGINTToULong(p_right.GetAsBCD().AsInt64());
}

bool
SQL_OperFloatGreatEQNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsFloat() >= (float) p_right.GetAsBCD().AsDouble();
}

bool
SQL_OperDoubleGreatEQNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsDouble() >= p_right.GetAsBCD().AsDouble();
}

bool
SQL_OperBitGreatEQNum(SQLVariant& p_left,SQLVariant& /*p_right*/)
{
  return p_left.GetAsBit() != 0;
}

bool
SQL_OperSTinyGreatEQNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() >= SQL_SLongToTinyInt(p_right.GetAsBCD().AsLong());
}

bool
SQL_OperUTinyGreatEQNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() >= SQL_SLongToUTinyInt(p_right.GetAsBCD().AsLong());
}

bool
SQL_OperSBigGreatEQNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSBigInt() >= p_right.GetAsBCD().AsInt64();
}

bool
SQL_OperUBigGreatEQNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUBigInt() >= p_right.GetAsBCD().AsUInt64();
}

bool
SQL_OperNumGreatEQNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBCD() >= p_right.GetAsBCD();
}

// TYPE == GUID

bool
SQL_OperGuidGreatEQGuid(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLGUID* left  = p_left.GetAsGUID();
  SQLGUID* right = p_right.GetAsGUID();

  return memcmp(left,right,sizeof(SQLGUID)) >= 0;
}

// TYPE == BINARY

bool
SQL_OperBinaryGreatEQBinary(SQLVariant& p_left,SQLVariant& p_right)
{
  void* left   = p_left.GetAsBinary();
  void* right  = p_right.GetAsBinary();
  // Take longest of two binaries in comparison
  long  length = p_left.GetBinaryLength() > p_right.GetBinaryLength() ? p_left.GetBinaryLength(): p_right.GetBinaryLength();

  return memcmp(left,right,length) >= 0;
}

// TYPE == DATE

bool
SQL_OperDateGreatEQDate(SQLVariant& p_left,SQLVariant& p_right)
{
  // Relies on the > operator of SQLDate
  return p_left.GetAsSQLDate() >= p_right.GetAsSQLDate();
}

bool
SQL_OperStampGreatEQDate(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLDate date = p_left.GetAsSQLTimestamp().AsSQLDate();
  return date >= p_right.GetAsSQLDate();
}

// TYPE == TIME

bool
SQL_OperTimeGreatEQTime(SQLVariant& p_left,SQLVariant& p_right)
{
  // Relies on the > operator of SQLTime
  return p_left.GetAsSQLTime() >= p_right.GetAsSQLTime();
}

bool
SQL_OperStampGreatEQTime(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLTime time = p_left.GetAsSQLTimestamp().AsSQLTime();
  return time >= p_right.GetAsSQLTime();
}

// TYPE == TIMESTAMP

bool
SQL_OperStampGreatEQStamp(SQLVariant& p_left,SQLVariant& p_right)
{
  // Relies on the > operator of SQLTimestamp
  return p_left.GetAsSQLTimestamp() >= p_right.GetAsSQLTimestamp();
}

// TYPE == INTERVAL_YEAR_MONTH

bool
SQL_OperIntYMGreatEQIntYM(SQLVariant& p_left,SQLVariant& p_right)
{
  // Relies on the calculation of months in SQLInterval
  return p_left.GetAsSQLInterval().AsDatabaseDouble() >= p_right.GetAsSQLInterval().AsDatabaseDouble();
}

// TYPE == INTERVAL_DAY_SECOND

bool
SQL_OperIntDSGreatEQIntDS(SQLVariant& p_left,SQLVariant& p_right)
{
  // Relies on the calculation of seconds in SQLInterval
  return p_left.GetAsSQLInterval().AsDatabaseDouble() >= p_right.GetAsSQLInterval().AsDatabaseDouble();
}

// OPERATOR ARRAY

static CompareFunctionArray OperatorGreaterEqual[CT_LAST][CT_LAST] =
{
                         // CT_CHAR                  CT_SSHORT                    CT_USHORT                    CT_SLONG                     CT_ULONG                    CT_FLOAT                    CT_DOUBLE                    CT_BIT                    CT_STINYINT                 CT_UTINYINT                 CT_SBIGINT                 CT_UBIGINT                 CT_NUMERIC                CT_GUID                  CT_BINARY                    CT_DATE                   CT_TIME                   CT_TIMESTAMP               CT_INTERVAL_YM             CT_INTERVAL_DS
                         // ------------------------ ---------------------------- ---------------------------- ---------------------------- --------------------------- --------------------------- ---------------------------- ------------------------- --------------------------- --------------------------- -------------------------- -------------------------- ------------------------- ------------------------ ---------------------------- ------------------------- ------------------------- -------------------------- -------------------------- ----------------------------
  /* CT_CHAR        */   {  &SQL_OperVarGreatEQChar ,&SQL_OperVarGreatEQChar     ,&SQL_OperVarGreatEQChar     ,&SQL_OperVarGreatEQChar     ,&SQL_OperVarGreatEQChar    ,&SQL_OperVarGreatEQChar    ,&SQL_OperVarGreatEQChar     ,&SQL_OperVarGreatEQChar  ,&SQL_OperVarGreatEQChar    ,&SQL_OperVarGreatEQChar    ,&SQL_OperVarGreatEQChar   ,&SQL_OperVarGreatEQChar   ,&SQL_OperVarGreatEQChar  ,&SQL_OperVarGreatEQChar ,nullptr                     ,&SQL_OperVarGreatEQChar  ,&SQL_OperVarGreatEQChar  ,&SQL_OperVarGreatEQChar   ,&SQL_OperVarGreatEQChar   ,&SQL_OperVarGreatEQChar    }
  /* CT_SSHORT      */  ,{  &SQL_OperVarGreatEQChar ,&SQL_OperSShortGreatEQSShort,&SQL_OperSShortGreatEQUShort,&SQL_OperSShortGreatEQSLong ,&SQL_OperSShortGreatEQULong,&SQL_OperSShortGreatEQFloat,&SQL_OperSShortGreatEQDouble,&SQL_OperSShortGreatEQBit,&SQL_OperSShortGreatEQSTiny,&SQL_OperSShortGreatEQUTiny,&SQL_OperSShortGreatEQSBig,&SQL_OperSShortGreatEQUBig,&SQL_OperSShortGreatEQNum,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_USHORT      */  ,{  &SQL_OperVarGreatEQChar ,&SQL_OperUShortGreatEQSShort,&SQL_OperUShortGreatEQUShort,&SQL_OperUShortGreatEQSLong ,&SQL_OperUShortGreatEQULong,&SQL_OperUShortGreatEQFloat,&SQL_OperUShortGreatEQDouble,&SQL_OperUShortGreatEQBit,&SQL_OperUShortGreatEQSTiny,&SQL_OperUShortGreatEQUTiny,&SQL_OperUShortGreatEQSBig,&SQL_OperUShortGreatEQUBig,&SQL_OperUShortGreatEQNum,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_SLONG       */  ,{  &SQL_OperVarGreatEQChar ,&SQL_OperSLongGreatEQSShort ,&SQL_OperSLongGreatEQUShort ,&SQL_OperSLongGreatEQSLong  ,&SQL_OperSLongGreatEQULong ,&SQL_OperSLongGreatEQFloat ,&SQL_OperSLongGreatEQDouble ,&SQL_OperSLongGreatEQBit ,&SQL_OperSLongGreatEQSTiny ,&SQL_OperSLongGreatEQUTiny ,&SQL_OperSLongGreatEQSBig ,&SQL_OperSLongGreatEQUBig ,&SQL_OperSLongGreatEQNum ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_ULONG       */  ,{  &SQL_OperVarGreatEQChar ,&SQL_OperULongGreatEQSShort ,&SQL_OperUlongGreatEQUShort ,&SQL_OperULongGreatEQSLong  ,&SQL_OperULongGreatEQULong ,&SQL_OperULongGreatEQFloat ,&SQL_OperULongGreatEQDouble ,&SQL_OperULongGreatEQBit ,&SQL_OperULongGreatEQSTiny ,&SQL_OperULongGreatEQUTiny ,&SQL_OperULongGreatEQSBig ,&SQL_OperULongGreatEQUBig ,&SQL_OperULongGreatEQNum ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_FLOAT       */  ,{  &SQL_OperVarGreatEQChar ,&SQL_OperFloatGreatEQSShort ,&SQL_OperFloatGreatEQUShort ,&SQL_OperFloatGreatEQSLong  ,&SQL_OperFloatGreatEQULong ,&SQL_OperFloatGreatEQFloat ,&SQL_OperFloatGreatEQDouble ,&SQL_OperFloatGreatEQBit ,&SQL_OperFloatGreatEQSTiny ,&SQL_OperFloatGreatEQUTiny ,&SQL_OperFloatGreatEQSBig ,&SQL_OperFloatGreatEQUBig ,&SQL_OperFloatGreatEQNum ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_DOUBLE      */  ,{  &SQL_OperVarGreatEQChar ,&SQL_OperDoubleGreatEQSShort,&SQL_OperDoubleGreatEQUShort,&SQL_OperDoubleEGreatEQSLong,&SQL_OperDoubleGreatEQULong,&SQL_OperDoubleGreatEQFloat,&SQL_OperDoubleGreatEQDouble,&SQL_OperDoubleGreatEQBit,&SQL_OperDoubleGreatEQSTiny,&SQL_OperDoubleGreatEQUTiny,&SQL_OperDoubleGreatEQSBig,&SQL_OperDoubleGreatEQUBig,&SQL_OperDoubleGreatEQNum,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_BIT         */  ,{  &SQL_OperVarGreatEQChar ,&SQL_OperBitGreatEQSShort   ,&SQL_OperBitGreatEQUShort   ,&SQL_OperBitGreatEQSLong    ,&SQL_OperBitGreatEQULong   ,&SQL_OperBitGreatEQFloat   ,&SQL_OperBitGreatEQDouble   ,&SQL_OperBitGreatEQBit   ,&SQL_OperBitGreatEQSTiny   ,&SQL_OperBitGreatEQUTiny   ,&SQL_OperBitGreatEQSBig   ,&SQL_OperBitGreatEQUBig   ,&SQL_OperBitGreatEQNum   ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_STINYINT    */  ,{  &SQL_OperVarGreatEQChar ,&SQL_OperSTinyGreatEQSShort ,&SQL_OperSTinyGreatEQUShort ,&SQL_OperSTinyGreatEQSLong  ,&SQL_OperSTinyGreatEQULong ,&SQL_OperSTinyGreatEQFloat ,&SQL_OperSTinyGreatEQDouble ,&SQL_OperSTinyGreatEQBit ,&SQL_OperSTinyGreatEQSTiny ,&SQL_OperSTinyGreatEQUTiny ,&SQL_OperSTinyGreatEQSBig ,&SQL_OperSTinyGreatEQUBig ,&SQL_OperSTinyGreatEQNum ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_UTINYINT    */  ,{  &SQL_OperVarGreatEQChar ,&SQL_OperUTinyGreatEQSShort ,&SQL_OperUTinyGreatEQUShort ,&SQL_OperUTinyGreatEQSLong  ,&SQL_OperUTinyGreatEQULong ,&SQL_OperUTinyGreatEQFloat ,&SQL_OperUTinyGreatEQDouble ,&SQL_OperUTinyGreatEQBit ,&SQL_OperUTinyGreatEQSTiny ,&SQL_OperUTinyGreatEQUTiny ,&SQL_OperUTinyGreatEQSBig ,&SQL_OperUTinyGreatEQUBig ,&SQL_OperUTinyGreatEQNum ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_SBIGINT     */  ,{  &SQL_OperVarGreatEQChar ,&SQL_OperSBigGreatEQSShort  ,&SQL_OperSBigGreatEQUShort  ,&SQL_OperSBigGreatEQSLong   ,&SQL_OperSBigGreatEQULong  ,&SQL_OperSBigGreatEQFloat  ,&SQL_OperSBigGreatEQDouble  ,&SQL_OperSBigGreatEQBit  ,&SQL_OperSBigGreatEQSTiny  ,&SQL_OperSBigGreatEQUTiny  ,&SQL_OperSBigGreatEQSBig  ,&SQL_OperSBigGreatEQUBig  ,&SQL_OperSBigGreatEQNum  ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_UBIGINT     */  ,{  &SQL_OperVarGreatEQChar ,&SQL_OperUBigGreatEQSShort  ,&SQL_OperUBigGreatEQUShort  ,&SQL_OperUBigGreatEQSLong   ,&SQL_OperUBigGreatEQULong  ,&SQL_OperUBigGreatEQFloat  ,&SQL_OperUBigGreatEQDouble  ,&SQL_OperUBigGreatEQBit  ,&SQL_OperUBigGreatEQSTiny  ,&SQL_OperUBigGreatEQUTiny  ,&SQL_OperUBigGreatEQSBig  ,&SQL_OperUBigGreatEQUBig  ,&SQL_OperUBigGreatEQNum  ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_NUMERIC     */  ,{  &SQL_OperVarGreatEQChar ,&SQL_OperNumGreatEQSShort   ,&SQL_OperNumGreatEQUShort   ,&SQL_OperNumGreatEQSLong    ,&SQL_OperNumGreatEQULong   ,&SQL_OperNumGreatEQFloat   ,&SQL_OperNumGreatEQDouble   ,&SQL_OperNumGreatEQBit   ,&SQL_OperNumGreatEQSTiny   ,&SQL_OperNumGreatEQUTiny   ,&SQL_OperNumGreatEQSBig   ,&SQL_OperNumGreatEQUBig   ,&SQL_OperNumGreatEQNum   ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_GUID        */  ,{  &SQL_OperGuidGreatEQChar,nullptr                     ,nullptr                     ,nullptr                     ,nullptr                    ,nullptr                    ,nullptr                     ,nullptr                  ,nullptr                    ,nullptr                    ,nullptr                   ,nullptr                   ,nullptr                  ,&SQL_OperGuidGreatEQGuid,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_BINARY      */  ,{  nullptr                 ,nullptr                     ,nullptr                     ,nullptr                     ,nullptr                    ,nullptr                    ,nullptr                     ,nullptr                  ,nullptr                    ,nullptr                    ,nullptr                   ,nullptr                   ,nullptr                  ,nullptr                 ,&SQL_OperBinaryGreatEQBinary,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_DATE        */  ,{  &SQL_OperVarGreatEQChar ,nullptr                     ,nullptr                     ,nullptr                     ,nullptr                    ,nullptr                    ,nullptr                     ,nullptr                  ,nullptr                    ,nullptr                    ,nullptr                   ,nullptr                   ,nullptr                  ,nullptr                 ,nullptr                     ,&SQL_OperDateGreatEQDate ,nullptr                  ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_TIME        */  ,{  &SQL_OperVarGreatEQChar ,nullptr                     ,nullptr                     ,nullptr                     ,nullptr                    ,nullptr                    ,nullptr                     ,nullptr                  ,nullptr                    ,nullptr                    ,nullptr                   ,nullptr                   ,nullptr                  ,nullptr                 ,nullptr                     ,nullptr                  ,&SQL_OperTimeGreatEQTime ,nullptr                   ,nullptr                   ,nullptr                    }
  /* CT_TIMESTAMP   */  ,{  &SQL_OperVarGreatEQChar ,nullptr                     ,nullptr                     ,nullptr                     ,nullptr                    ,nullptr                    ,nullptr                     ,nullptr                  ,nullptr                    ,nullptr                    ,nullptr                   ,nullptr                   ,nullptr                  ,nullptr                 ,nullptr                     ,&SQL_OperStampGreatEQDate,&SQL_OperStampGreatEQTime,&SQL_OperStampGreatEQStamp,nullptr                   ,nullptr                    }
  /* CT_INTERVAL_YM */  ,{  &SQL_OperVarGreatEQChar ,nullptr                     ,nullptr                     ,nullptr                     ,nullptr                    ,nullptr                    ,nullptr                     ,nullptr                  ,nullptr                    ,nullptr                    ,nullptr                   ,nullptr                   ,nullptr                  ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,&SQL_OperIntYMGreatEQIntYM,nullptr                    }
  /* CT_INTERVAL_DS */  ,{  &SQL_OperVarGreatEQChar ,nullptr                     ,nullptr                     ,nullptr                     ,nullptr                    ,nullptr                    ,nullptr                     ,nullptr                  ,nullptr                    ,nullptr                    ,nullptr                   ,nullptr                   ,nullptr                  ,nullptr                 ,nullptr                     ,nullptr                  ,nullptr                  ,nullptr                   ,nullptr                   ,&SQL_OperIntDSGreatEQIntDS }
};

// Greater-equal operator for SQLVariant
bool
SQLVariant::operator>=(SQLVariant& p_right)
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
  OperatorCompare function = OperatorGreaterEqual[left][right].function;
  if(function)
  {
    return (*function)(*this,p_right);
  }
  // No compare function found
  // Data types are not comparable
  CString leftType  = FindDatatype(m_datatype);
  CString rightType = FindDatatype(p_right.m_datatype);
  CString error;
  error.Format("Cannot do the greater-equal operator on (%s >= %s)",leftType,rightType);
  throw error;
}

