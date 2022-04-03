O////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantOperatorEqual.cpp
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
static SQL_OperVarEqualsChar(SQLVariant& p_left,SQLVariant& p_right)
{
  XString leftString,rightString;
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

  return leftString.Compare(rightString) == 0;
}

bool   
static SQL_OperBitEqualsChar(SQLVariant& p_left,SQLVariant& p_right)
{
  if((p_left.GetAsBit() == 0 && p_right.GetAsDouble() == 0.0) ||
     (p_left.GetAsBit() != 0 && p_right.GetAsDouble() != 0.0) )
  {
    return true;
  }
  return false;
}

bool
static SQL_OperGuidEqualsChar(SQLVariant& p_left,SQLVariant& p_right)
{
  XString leftString,rightString;
  p_left .GetAsString(leftString);
  p_right.GetAsString(rightString);

  leftString. TrimLeft ('{');
  leftString. TrimRight('}');
  rightString.TrimLeft ('}');
  rightString.TrimRight('{');

  return leftString.Compare(rightString) == 0;
}

// SIGNED SHORT

bool
static SQL_OperSShortEqualsSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSShort() == p_right.GetAsSShort();
}

bool
static SQL_OperUShortEqualsSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUShort() == SQL_ShortToUShort(p_right.GetAsSShort());
}

bool
static SQL_OperSLongEqualsSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SLongToShort(p_left.GetAsSLong()) == p_right.GetAsSShort();
}

bool
static SQL_OperULongEqualsSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_ULongToUShort(p_left.GetAsULong()) == SQL_ShortToUShort(p_right.GetAsSShort());
}

bool
static SQL_OperFloatEqualsSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_FloatToShort(p_left.GetAsFloat()) == p_right.GetAsSShort();
}

bool
static SQL_OperDoubleEqualsSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_DoubleToShort(p_left.GetAsDouble()) == p_right.GetAsSShort();
}

bool
static SQL_OperBitEqualsSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  if((p_left.GetAsBit() != 0 && p_right.GetAsSShort() != 0) ||
     (p_left.GetAsBit() == 0 && p_right.GetAsSShort() == 0) )
  {
    return true;
  }
  return false;
}

bool   
static SQL_OperSTinyEqualsSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() == SQL_ShortToTinyInt(p_right.GetAsSShort());
}

bool
static SQL_OperUTinyEqualsSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() == SQL_ShortToUTinyInt(p_right.GetAsSShort());
}

bool
static SQL_OperSBigEqualsSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_BIGINTToShort(p_left.GetAsSBigInt()) == p_right.GetAsSShort();
}

bool
static SQL_OperUBigEqualsSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_UBIGINTToShort(p_left.GetAsUBigInt()) == p_right.GetAsSShort();
}

bool
static SQL_OperNumEqualsSShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SLongToShort(p_left.GetAsBCD().AsLong()) == p_right.GetAsSShort();
}

// UNSIGNED SHORT

bool   
static SQL_OperSShortEqualsUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_ShortToUShort(p_left.GetAsSShort()) == p_right.GetAsUShort();
}

bool   
static SQL_OperUShortEqualsUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUShort() == p_right.GetAsUShort();
}

bool
static SQL_OperSLongEqualsUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SLongToUShort(p_left.GetAsSLong()) == p_right.GetAsUShort();
}

bool
static SQL_OperUlongEqualsUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_ULongToUShort(p_left.GetAsULong()) == p_right.GetAsUShort();
}

bool
static SQL_OperFloatEqualsUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_FloatToUShort(p_left.GetAsFloat()) == p_right.GetAsUShort();
}

bool
static SQL_OperDoubleEqualsUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_DoubleToUShort(p_left.GetAsDouble()) == p_right.GetAsUShort();
}

bool
static SQL_OperBitEqualsUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  if((p_left.GetAsBit() == 0 && p_right.GetAsUShort() == 0) ||
     (p_left.GetAsBit() != 0 && p_right.GetAsUShort() != 0) )
  {
    return true;
  }
  return false;
}

bool
static SQL_OperSTinyEqualsUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() == SQL_UShortToTinyInt(p_right.GetAsUShort());
}

bool
static SQL_OperUTinyEqualsUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() == SQL_UShortToUTinyInt(p_right.GetAsUShort());
}

bool
static SQL_OperSBigEqualsUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_BIGINTToUShort(p_left.GetAsSBigInt()) == p_right.GetAsUShort();
}

bool
static SQL_OperUBigEqualsUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_UBIGINTToUShort(p_left.GetAsUBigInt()) == p_right.GetAsUShort();
}

bool   
static SQL_OperNumEqualsUShort(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SLongToUShort(p_left.GetAsBCD().AsLong()) == p_right.GetAsUShort();
}

// SIGNED LONG

bool   
static SQL_OperSShortEqualsSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSShort() == SQL_SLongToShort(p_right.GetAsSLong());
}

bool
static SQL_OperUShortEqualsSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUShort() == SQL_SLongToUShort(p_right.GetAsSLong());
}

bool
static SQL_OperSLongEqualsSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSLong() == p_right.GetAsSLong();
}

bool
static SQL_OperULongEqualsSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsULong() == SQL_LongToULong(p_right.GetAsSLong());
}

bool
static SQL_OperFloatEqualsSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_FloatToLong(p_left.GetAsFloat()) == p_right.GetAsSLong();
}

bool
static SQL_OperDoubleEequalsSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_DoubleToLong(p_left.GetAsDouble()) == p_right.GetAsSLong();
}

bool
static SQL_OperBitEqualsSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  if((p_left.GetAsBit() == 0 && p_right.GetAsSLong() == 0) ||
     (p_left.GetAsBit() != 0 && p_right.GetAsSLong() != 0) )
  {
    return true;
  }
  return false;
}

bool
static SQL_OperSTinyEqualsSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() == SQL_SLongToTinyInt(p_right.GetAsSLong());
}

bool
static SQL_OperUTinyEqualsSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() == SQL_SLongToUTinyInt(p_right.GetAsSLong());
}

bool
static SQL_OperSBigEqualsSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_BIGINTToLong(p_left.GetAsSBigInt()) == p_right.GetAsSLong();
}

bool
static SQL_OperUBigEqualsSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_UBIGINTToLong(p_left.GetAsUBigInt()) == p_right.GetAsSLong();
}

bool
static SQL_OperNumEqualsSLong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBCD().AsLong() == p_right.GetAsSLong();
}

// UNSIGNED LONG

bool   
static SQL_OperSShortEqualsULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_ShortToUShort(p_left.GetAsSShort()) == SQL_ULongToUShort(p_right.GetAsULong());
}

bool
static SQL_OperUShortEqualsULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUShort() == SQL_ULongToUShort(p_right.GetAsULong());
}

bool
static SQL_OperSLongEqualsULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_LongToULong(p_left.GetAsSLong()) == p_right.GetAsULong();
}

bool
static SQL_OperULongEqualsULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsULong() == p_right.GetAsULong();
}

bool
static SQL_OperFloatEqualsULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_FloatToULong(p_left.GetAsFloat()) == p_right.GetAsULong();
}

bool
static SQL_OperDoubleEqualsULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_DoubleToULong(p_left.GetAsDouble()) == p_right.GetAsULong();
}

bool
static SQL_OperBitEqualsULong(SQLVariant& p_left,SQLVariant& p_right)
{
  if((p_left.GetAsBit() == 0 && p_right.GetAsULong() == 0) ||
     (p_left.GetAsBit() != 0 && p_right.GetAsULong() != 0) )
  {
    return true;
  }
  return false;
}

bool
static SQL_OperSTinyEqualsULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_TinyIntToUTinyInt(p_left.GetAsSTinyInt()) == SQL_ULongToUTinyInt(p_right.GetAsULong());
}

bool
static SQL_OperUTinyEqualsULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() == SQL_ULongToUTinyInt(p_right.GetAsULong());
}

bool
static SQL_OperSBigEqualsULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_BIGINTToULong(p_left.GetAsSBigInt()) == p_right.GetAsULong();
}

bool
static SQL_OperUBigEqualsULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_UBIGINTToULong(p_left.GetAsUBigInt()) == p_right.GetAsULong();
}

bool
static SQL_OperNumEqualsULong(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_UBIGINTToULong(p_left.GetAsBCD().AsUInt64()) == p_right.GetAsULong();
}

// FLOAT

bool   
static SQL_OperSShortEqualsFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSShort() == SQL_FloatToShort(p_right.GetAsFloat());
}

bool   
static SQL_OperUShortEqualsFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUShort() == SQL_FloatToUShort(p_right.GetAsFloat());
}

bool
static SQL_OperSLongEqualsFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSLong() == SQL_FloatToLong(p_right.GetAsFloat());
}

bool
static SQL_OperULongEqualsFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsULong() == SQL_FloatToULong(p_right.GetAsFloat());
}

bool
static SQL_OperFloatEqualsFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsFloat() == p_right.GetAsFloat();
}

bool
static SQL_OperDoubleEqualsFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_DoubleToFloat(p_left.GetAsDouble()) == p_right.GetAsFloat();
}

bool
static SQL_OperBitEqualsFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  if((p_left.GetAsBit() == 0 && p_right.GetAsFloat() == 0.0) ||
     (p_left.GetAsBit() != 0 && p_right.GetAsFloat() != 0.0) )
  {
    return true;
  }
  return false;
}

bool
static SQL_OperSTinyEqualsFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() == SQL_FloatToTinyInt(p_right.GetAsFloat());
}

bool
static SQL_OperUTinyEqualsFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() == SQL_FloatToUTinyInt(p_right.GetAsFloat());
}

bool
static SQL_OperSBigEqualsFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSBigInt() == SQL_FloatToBIGINT(p_right.GetAsFloat());
}

bool
static SQL_OperUBigEqualsFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUBigInt() == SQL_FloatToUBIGINT(p_right.GetAsFloat());
}

bool
static SQL_OperNumEqualsFloat(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBCD().AsDouble() == (double) p_right.GetAsFloat();
}

// DOUBLE

bool
static SQL_OperSShortEqualsDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSShort() == SQL_DoubleToShort(p_right.GetAsDouble());
}

bool
static SQL_OperUShortEqualsDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUShort() == SQL_DoubleToUShort(p_right.GetAsDouble());
}

bool
static SQL_OperSLongEqualsDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSLong() == SQL_DoubleToLong(p_right.GetAsDouble());
}

bool
static SQL_OperULongEqualsDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsULong() == SQL_DoubleToULong(p_right.GetAsDouble());
}

bool
static SQL_OperFloatEqualsDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsFloat() == SQL_DoubleToFloat(p_right.GetAsDouble());
}

bool
static SQL_OperDoubleEqualsDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsDouble() == p_right.GetAsDouble();
}

bool
static SQL_OperBitEqualsDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  if((p_left.GetAsBit() == 0 && p_right.GetAsDouble() == 0.0) ||
     (p_left.GetAsBit() != 0 && p_right.GetAsDouble() != 0.0))
  {
    return true;
  }
  return false;
}

bool
static SQL_OperSTinyEqualsDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() == SQL_DoubleToTinyInt(p_right.GetAsDouble());
}

bool
static SQL_OperUTinyEqualsDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() == SQL_DoubleToUTinyInt(p_right.GetAsDouble());
}

bool
static SQL_OperSBigEqualsDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSBigInt() == SQL_DoubleToBIGINT(p_right.GetAsDouble());
}

bool
static SQL_OperUBigEqualsDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUBigInt() == SQL_DoubleToUBIGINT(p_right.GetAsDouble());
}

bool
static SQL_OperNumEqualsDouble(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBCD().AsDouble() == p_right.GetAsDouble();
}

// BIT

bool
static SQL_OperVarEqualsBit(SQLVariant& p_left,SQLVariant& p_right)
{
  if((p_left.GetAsDouble() == 0.0 && p_right.GetAsBit() == 0) ||
     (p_left.GetAsDouble() != 0.0 && p_right.GetAsBit() != 0) )
  {
    return true;
  }
  return false;
}

bool   
static SQL_OperSShortEqualsBit(SQLVariant& p_left,SQLVariant& p_right)
{
  if((p_left.GetAsSShort() == 0 && p_right.GetAsBit() == 0) ||
     (p_left.GetAsSShort() != 0 && p_right.GetAsBit() != 0))
  {
    return true;
  }
  return false;
}

bool
static SQL_OperUShortEqualsBit(SQLVariant& p_left,SQLVariant& p_right)
{
  if((p_left.GetAsUShort() == 0 && p_right.GetAsBit() == 0) ||
     (p_left.GetAsUShort() != 0 && p_right.GetAsBit() != 0))
  {
    return true;
  }
  return false;
}

bool   
static SQL_OperSLongEqualsBit(SQLVariant& p_left,SQLVariant& p_right)
{
  if((p_left.GetAsSLong() == 0 && p_right.GetAsBit() == 0) ||
     (p_left.GetAsSLong() != 0 && p_right.GetAsBit() != 0))
  {
    return true;
  }
  return false;
}

bool
static SQL_OperULongEqualsBit(SQLVariant& p_left,SQLVariant& p_right)
{
  if((p_left.GetAsULong() == 0 && p_right.GetAsBit() == 0) ||
     (p_left.GetAsULong() != 0 && p_right.GetAsBit() != 0))
  {
    return true;
  }
  return false;
}

bool
static SQL_OperFloatEqualsBit(SQLVariant& p_left,SQLVariant& p_right)
{
  if((p_left.GetAsFloat() == 0.0 && p_right.GetAsBit() == 0) ||
     (p_left.GetAsFloat() != 0.0 && p_right.GetAsBit() != 0))
  {
    return true;
  }
  return false;
}

bool
static SQL_OperDoubleEqualsBit(SQLVariant& p_left,SQLVariant& p_right)
{
  if((p_left.GetAsDouble() == 0.0 && p_right.GetAsBit() == 0) ||
     (p_left.GetAsDouble() != 0.0 && p_right.GetAsBit() != 0))
  {
    return true;
  }
  return false;
}

bool
static SQL_OperBitEqualsBit(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBit() == p_right.GetAsBit();
}

bool
static SQL_OperSTinyEqualsBit(SQLVariant& p_left,SQLVariant& p_right)
{
  if((p_left.GetAsSTinyInt() == 0 && p_right.GetAsBit() == 0) ||
     (p_left.GetAsSTinyInt() != 0 && p_right.GetAsBit() != 0))
  {
    return true;
  }
  return false;
}

bool
static SQL_OperUTinyEqualsBit(SQLVariant& p_left,SQLVariant& p_right)
{
  if((p_left.GetAsUTinyInt() == 0 && p_right.GetAsBit() == 0) ||
     (p_left.GetAsUTinyInt() != 0 && p_right.GetAsBit() != 0))
  {
    return true;
  }
  return false;
}

bool
static SQL_OperSBigEqualsBit(SQLVariant& p_left,SQLVariant& p_right)
{
  if((p_left.GetAsSBigInt() == 0 && p_right.GetAsBit() == 0) ||
     (p_left.GetAsSBigInt() != 0 && p_right.GetAsBit() != 0))
  {
    return true;
  }
  return false;
}

bool
static SQL_OperUBigEqualsBit(SQLVariant& p_left,SQLVariant& p_right)
{
  if((p_left.GetAsUBigInt() == 0 && p_right.GetAsBit() == 0) ||
     (p_left.GetAsUBigInt() != 0 && p_right.GetAsBit() != 0))
  {
    return true;
  }
  return false;
}

bool
static SQL_OperNumEqualsBit(SQLVariant& p_left,SQLVariant& p_right)
{
  XString left;
  p_left.GetAsString(left);
  int lf = atoi(left);
 
  if((lf == 0 && p_right.GetAsBit() == 0) ||
     (lf != 0 && p_right.GetAsBit() != 0))
  {
    return true;
  }
  return false;
}

// SIGNED TINYINT

bool
static SQL_OperSShortEqualsSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_ShortToTinyInt(p_left.GetAsSShort()) == p_right.GetAsSTinyInt();
}

bool
static SQL_OperUShortEqualsSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_UShortToTinyInt(p_left.GetAsUShort()) == p_right.GetAsSTinyInt();
}

bool
static SQL_OperSLongEqualsSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SLongToTinyInt(p_left.GetAsSLong()) == p_right.GetAsSTinyInt();
}

bool
static SQL_OperULongEqualsSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_ULongToTinyInt(p_left.GetAsULong()) == p_right.GetAsSTinyInt();
}

bool
static SQL_OperFloatEqualsSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_FloatToTinyInt(p_left.GetAsFloat()) == p_right.GetAsSTinyInt();
}

bool
static SQL_OperDoubleEqualsSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_DoubleToTinyInt(p_left.GetAsDouble()) == p_right.GetAsSTinyInt();
}

bool
static SQL_OperBitEqualsSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  if((p_left.GetAsBit() == 0 && p_right.GetAsSTinyInt() == 0) ||
     (p_left.GetAsBit() != 0 && p_right.GetAsSTinyInt() != 0))
  {
    return true;
  }
  return false;
}

bool
static SQL_OperSTinyEqualsSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() == p_right.GetAsSTinyInt();
}

bool
static SQL_OperUTinyEqualsSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() == SQL_TinyIntToUTinyInt(p_right.GetAsSTinyInt());
}

bool
static SQL_OperSBigEqualsSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SBIGINTToTinyInt(p_left.GetAsSBigInt()) == p_right.GetAsSTinyInt();
}

bool
static SQL_OperUBigEqualsSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_UBIGINTToTinyInt(p_left.GetAsUBigInt()) == p_right.GetAsSTinyInt();
}

bool
static SQL_OperNumEqualsSTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SLongToTinyInt(p_left.GetAsBCD().AsLong()) == p_right.GetAsSTinyInt();
}

// UNSIGNED TINYINT

bool
static SQL_OperSShortEqualsUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_ShortToUTinyInt(p_left.GetAsSShort()) == p_right.GetAsUTinyInt();
}

bool
static SQL_OperUShortEqualsUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_UShortToUTinyInt(p_left.GetAsUShort()) == p_right.GetAsUTinyInt();
}

bool
static SQL_OperSLongEqualsUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SLongToUTinyInt(p_left.GetAsSLong()) == p_right.GetAsUTinyInt();
}

bool
static SQL_OperULongEqualsUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_ULongToUTinyInt(p_left.GetAsULong()) == p_right.GetAsUTinyInt();
}

bool
static SQL_OperFloatEqualsUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_FloatToUTinyInt(p_left.GetAsFloat()) == p_right.GetAsUTinyInt();
}

bool
static SQL_OperDoubleEqualsUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_DoubleToUTinyInt(p_left.GetAsDouble()) == p_right.GetAsUTinyInt();
}

bool
static SQL_OperBitEqualsUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  if((p_left.GetAsBit() == 0 && p_right.GetAsUTinyInt() == 0) ||
     (p_left.GetAsBit() != 0 && p_right.GetAsUTinyInt() != 0))
  {
    return true;
  }
  return false;
}

bool
static SQL_OperSTinyEqualsUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_TinyIntToUTinyInt(p_left.GetAsSTinyInt()) == p_right.GetAsUTinyInt();
}

bool
static SQL_OperUTinyEqualsUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() == p_right.GetAsUTinyInt();
}

bool
static SQL_OperSBigEqualsUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SBIGINTToUTinyInt(p_left.GetAsSBigInt()) == p_right.GetAsUTinyInt();
}

bool
static SQL_OperUBigEqualsUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_UBIGINTToUTinyInt(p_left.GetAsUBigInt()) == p_right.GetAsUTinyInt();
}

bool
static SQL_OperNumEqualsUTiny(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SLongToUTinyInt(p_left.GetAsBCD().AsLong()) == p_right.GetAsUTinyInt();
}

// SIGNED BIGINT

bool
static SQL_OperSShortEqualsSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSShort() == SQL_BIGINTToShort(p_right.GetAsSBigInt());
}

bool
static SQL_OperUShortEqualsSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUShort() == SQL_BIGINTToUShort(p_right.GetAsSBigInt());
}

bool
static SQL_OperSLongEqualsSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSLong() == SQL_BIGINTToLong(p_right.GetAsSBigInt());
}

bool
static SQL_OperULongEqualsSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsULong() == SQL_BIGINTToULong(p_right.GetAsSBigInt());
}

bool
static SQL_OperFloatEqualsSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsFloat() == SQL_BIGINTToFloat(p_right.GetAsSBigInt());
}

bool
static SQL_OperDoubleEqualsSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsDouble() == SQL_BIGINTToDouble(p_right.GetAsSBigInt());
}

bool
static SQL_OperBitEqualsSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  if((p_left.GetAsBit() == 0 && p_right.GetAsSBigInt() == 0) ||
     (p_left.GetAsBit() != 0 && p_right.GetAsSBigInt() != 0))
  {
    return true;
  }
  return false;
}

bool
static SQL_OperSTinyEqualsSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() == SQL_SBIGINTToTinyInt(p_right.GetAsSBigInt());
}

bool
static SQL_OperUTinyEqualsSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() == SQL_SBIGINTToUTinyInt(p_right.GetAsSBigInt());
}

bool
static SQL_OperSBigEqualsSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSBigInt() == p_right.GetAsSBigInt();
}

bool
static SQL_OperUBigEqualsSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUBigInt() == SQL_SBIGINTToUBIGINT(p_right.GetAsSBigInt());
}

bool   
static SQL_OperNumEqualsSBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBCD().AsInt64() == p_right.GetAsSBigInt();
}

// UNSIGNED BIGINT

bool
static SQL_OperSShortEqualsUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSShort() == SQL_UBIGINTToShort(p_right.GetAsUBigInt());
}

bool
static SQL_OperUShortEqualsUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUShort() == SQL_UBIGINTToUShort(p_right.GetAsUBigInt());
}

bool
static SQL_OperSLongEqualsUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSLong() == SQL_UBIGINTToLong(p_right.GetAsUBigInt());
}

bool
static SQL_OperULongEqualsUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsULong() == SQL_UBIGINTToULong(p_right.GetAsUBigInt());
}

bool
static SQL_OperFloatEqualsUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsFloat() == SQL_UBIGINTToFloat(p_right.GetAsUBigInt());
}

bool
static SQL_OperDoubleEqualsUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsDouble() == SQL_UBIGINTToDouble(p_right.GetAsUBigInt());
}

bool
static SQL_OperBitEqualsUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  if((p_left.GetAsBit() == 0 && p_right.GetAsUBigInt() == 0) ||
     (p_left.GetAsBit() != 0 && p_right.GetAsUBigInt() != 0))
  {
    return true;
  }
  return false;
}

bool
static SQL_OperSTinyEqualsUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() == SQL_UBIGINTToUTinyInt(p_right.GetAsUBigInt());
}

bool
static SQL_OperUTinyEqualsUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() == SQL_UBIGINTToUTinyInt(p_right.GetAsUBigInt());
}

bool
static SQL_OperSBigEqualsUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return SQL_SBIGINTToUBIGINT(p_left.GetAsSBigInt()) == p_right.GetAsUBigInt();
}

bool
static SQL_OperUBigEqualsUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUBigInt() == p_right.GetAsUBigInt();
}

bool
static SQL_OperNumEqualsUBig(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBCD().AsUInt64() == p_right.GetAsUBigInt();
}

// NUMERIC

bool
static SQL_OperSShortEqualsNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSShort() == (short) p_right.GetAsBCD().AsLong();
}

bool
static SQL_OperUShortEqualsNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUShort() == (unsigned short) p_right.GetAsBCD().AsLong();
}

bool
static SQL_OperSLongEqualsNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSLong() == p_right.GetAsBCD().AsLong();
}

bool
static SQL_OperULongEqualsNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsULong() == (unsigned long) p_right.GetAsBCD().AsInt64();
}

bool
static SQL_OperFloatEqualsNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsFloat() == (float)p_right.GetAsBCD().AsDouble();
}

bool
static SQL_OperDoubleEqualsNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsDouble() == p_right.GetAsBCD().AsDouble();
}

bool
static SQL_OperBitEqualsNum(SQLVariant& p_left,SQLVariant& p_right)
{
  int ri = p_right.GetAsBCD().AsLong();

  if((p_left.GetAsBit() == 0 && ri == 0) ||
     (p_left.GetAsBit() != 0 && ri != 0) )
  {
    return true;
  }
  return false;
}

bool
static SQL_OperSTinyEqualsNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() == SQL_SLongToTinyInt(p_right.GetAsBCD().AsLong());
}

bool
static SQL_OperUTinyEqualsNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() == SQL_SLongToUTinyInt(p_right.GetAsBCD().AsLong());
}

bool
static SQL_OperSBigEqualsNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsSBigInt() == p_right.GetAsBCD().AsInt64();
}

bool
static SQL_OperUBigEqualsNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsUBigInt() == p_right.GetAsBCD().AsUInt64();
}

bool
static SQL_OperNumEqualsNum(SQLVariant& p_left,SQLVariant& p_right)
{
  return p_left.GetAsBCD() == p_right.GetAsBCD();
}

// TYPE == GUID

bool
static SQL_OperGuidEqualsGuid(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLGUID* left  = p_left .GetAsGUID();
  SQLGUID* right = p_right.GetAsGUID();

  return memcmp(left,right,sizeof(SQLGUID)) == 0;
}

// TYPE == BINARY

bool
static SQL_OperBinaryEqualsBinary(SQLVariant& p_left,SQLVariant& p_right)
{
  void* left   = p_left.GetAsBinary();
  void* right  = p_right.GetAsBinary();
  // Take longest of two binaries in comparison
  long  length = p_left.GetBinaryLength() > p_right.GetBinaryLength() ? p_left.GetBinaryLength() : p_right.GetBinaryLength();

  return memcmp(left,right,length) == 0;
}

// TYPE == DATE

bool
static SQL_OperDateEqualsDate(SQLVariant& p_left,SQLVariant& p_right)
{
  // Relies on the == operator of SQLDate
  return p_left.GetAsSQLDate() == p_right.GetAsSQLDate();
}

bool
static SQL_OperStampEqualsDate(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLDate date = p_left.GetAsSQLTimestamp().AsSQLDate();
  return date == p_right.GetAsSQLDate();
}

// TYPE == TIME

bool
static SQL_OperTimeEqualsTime(SQLVariant& p_left,SQLVariant& p_right)
{
  // Relies on the == operator of SQLTime
  return p_left.GetAsSQLTime() == p_right.GetAsSQLTime();
}

bool
static SQL_OperStampEqualsTime(SQLVariant& p_left,SQLVariant& p_right)
{
  SQLTime time = p_left.GetAsSQLTimestamp().AsSQLTime();
  return time == p_right.GetAsSQLTime();
}

// TYPE == TIMESTAMP

bool
static SQL_OperStampEqualsStamp(SQLVariant& p_left,SQLVariant& p_right)
{
  // Relies on the == operator of SQLTimestamp
  return p_left.GetAsSQLTimestamp() == p_right.GetAsSQLTimestamp();
}

// TYPE == INTERVAL_YEAR_MONTH

bool
static SQL_OperIntYMEqualsIntYM(SQLVariant& p_left,SQLVariant& p_right)
{
  // Relies on the calculation of months in SQLInterval
  return p_left.GetAsSQLInterval().AsDatabaseDouble() == p_right.GetAsSQLInterval().AsDatabaseDouble();
}

// TYPE == INTERVAL_DAY_SECOND

bool
static SQL_OperIntDSEqualsIntDS(SQLVariant& p_left,SQLVariant& p_right)
{
  // Relies on the calculation of seconds in SQLInterval
  return p_left.GetAsSQLInterval().AsDatabaseDouble() == p_right.GetAsSQLInterval().AsDatabaseDouble();
}

// OPERATOR ARRAY

static CompareFunctionArray OperatorEqual[CT_LAST][CT_LAST] =
{
                         // CT_CHAR                 CT_SSHORT                   CT_USHORT                   CT_SLONG                    CT_ULONG                   CT_FLOAT                   CT_DOUBLE                   CT_BIT                   CT_STINYINT                CT_UTINYINT                CT_SBIGINT                CT_UBIGINT                CT_NUMERIC               CT_GUID                 CT_BINARY                   CT_DATE                  CT_TIME                  CT_TIMESTAMP              CT_INTERVAL_YM            CT_INTERVAL_DS
                         // ----------------------- --------------------------- --------------------------- --------------------------- -------------------------- -------------------------- --------------------------- ------------------------ -------------------------- -------------------------- ------------------------- ------------------------- ------------------------ ----------------------- --------------------------- ------------------------ ------------------------ ------------------------- ------------------------- ---------------------------
  /* CT_CHAR        */   {  &SQL_OperVarEqualsChar ,&SQL_OperVarEqualsChar     ,&SQL_OperVarEqualsChar     ,&SQL_OperVarEqualsChar     ,&SQL_OperVarEqualsChar    ,&SQL_OperVarEqualsChar    ,&SQL_OperVarEqualsChar     ,&SQL_OperVarEqualsBit   ,&SQL_OperVarEqualsChar    ,&SQL_OperVarEqualsChar    ,&SQL_OperVarEqualsChar   ,&SQL_OperVarEqualsChar   ,&SQL_OperVarEqualsChar  ,&SQL_OperVarEqualsChar ,nullptr                    ,&SQL_OperVarEqualsChar  ,&SQL_OperVarEqualsChar  ,&SQL_OperVarEqualsChar   ,&SQL_OperVarEqualsChar   ,&SQL_OperVarEqualsChar    }
  /* CT_SSHORT      */  ,{  &SQL_OperVarEqualsChar ,&SQL_OperSShortEqualsSShort,&SQL_OperSShortEqualsUShort,&SQL_OperSShortEqualsSLong ,&SQL_OperSShortEqualsULong,&SQL_OperSShortEqualsFloat,&SQL_OperSShortEqualsDouble,&SQL_OperSShortEqualsBit,&SQL_OperSShortEqualsSTiny,&SQL_OperSShortEqualsUTiny,&SQL_OperSShortEqualsSBig,&SQL_OperSShortEqualsUBig,&SQL_OperSShortEqualsNum,nullptr                ,nullptr                    ,nullptr                 ,nullptr                 ,nullptr                  ,nullptr                  ,nullptr                   }
  /* CT_USHORT      */  ,{  &SQL_OperVarEqualsChar ,&SQL_OperUShortEqualsSShort,&SQL_OperUShortEqualsUShort,&SQL_OperUShortEqualsSLong ,&SQL_OperUShortEqualsULong,&SQL_OperUShortEqualsFloat,&SQL_OperUShortEqualsDouble,&SQL_OperUShortEqualsBit,&SQL_OperUShortEqualsSTiny,&SQL_OperUShortEqualsUTiny,&SQL_OperUShortEqualsSBig,&SQL_OperUShortEqualsUBig,&SQL_OperUShortEqualsNum,nullptr                ,nullptr                    ,nullptr                 ,nullptr                 ,nullptr                  ,nullptr                  ,nullptr                   }
  /* CT_SLONG       */  ,{  &SQL_OperVarEqualsChar ,&SQL_OperSLongEqualsSShort ,&SQL_OperSLongEqualsUShort ,&SQL_OperSLongEqualsSLong  ,&SQL_OperSLongEqualsULong ,&SQL_OperSLongEqualsFloat ,&SQL_OperSLongEqualsDouble ,&SQL_OperSLongEqualsBit ,&SQL_OperSLongEqualsSTiny ,&SQL_OperSLongEqualsUTiny ,&SQL_OperSLongEqualsSBig ,&SQL_OperSLongEqualsUBig ,&SQL_OperSLongEqualsNum ,nullptr                ,nullptr                    ,nullptr                 ,nullptr                 ,nullptr                  ,nullptr                  ,nullptr                   }
  /* CT_ULONG       */  ,{  &SQL_OperVarEqualsChar ,&SQL_OperULongEqualsSShort ,&SQL_OperUlongEqualsUShort ,&SQL_OperULongEqualsSLong  ,&SQL_OperULongEqualsULong ,&SQL_OperULongEqualsFloat ,&SQL_OperULongEqualsDouble ,&SQL_OperULongEqualsBit ,&SQL_OperULongEqualsSTiny ,&SQL_OperULongEqualsUTiny ,&SQL_OperULongEqualsSBig ,&SQL_OperULongEqualsUBig ,&SQL_OperULongEqualsNum ,nullptr                ,nullptr                    ,nullptr                 ,nullptr                 ,nullptr                  ,nullptr                  ,nullptr                   }
  /* CT_FLOAT       */  ,{  &SQL_OperVarEqualsChar ,&SQL_OperFloatEqualsSShort ,&SQL_OperFloatEqualsUShort ,&SQL_OperFloatEqualsSLong  ,&SQL_OperFloatEqualsULong ,&SQL_OperFloatEqualsFloat ,&SQL_OperFloatEqualsDouble ,&SQL_OperFloatEqualsBit ,&SQL_OperFloatEqualsSTiny ,&SQL_OperFloatEqualsUTiny ,&SQL_OperFloatEqualsSBig ,&SQL_OperFloatEqualsUBig ,&SQL_OperFloatEqualsNum ,nullptr                ,nullptr                    ,nullptr                 ,nullptr                 ,nullptr                  ,nullptr                  ,nullptr                   }
  /* CT_DOUBLE      */  ,{  &SQL_OperVarEqualsChar ,&SQL_OperDoubleEqualsSShort,&SQL_OperDoubleEqualsUShort,&SQL_OperDoubleEequalsSLong,&SQL_OperDoubleEqualsULong,&SQL_OperDoubleEqualsFloat,&SQL_OperDoubleEqualsDouble,&SQL_OperDoubleEqualsBit,&SQL_OperDoubleEqualsSTiny,&SQL_OperDoubleEqualsUTiny,&SQL_OperDoubleEqualsSBig,&SQL_OperDoubleEqualsUBig,&SQL_OperDoubleEqualsNum,nullptr                ,nullptr                    ,nullptr                 ,nullptr                 ,nullptr                  ,nullptr                  ,nullptr                   }
  /* CT_BIT         */  ,{  &SQL_OperBitEqualsChar ,&SQL_OperBitEqualsSShort   ,&SQL_OperBitEqualsUShort   ,&SQL_OperBitEqualsSLong    ,&SQL_OperBitEqualsULong   ,&SQL_OperBitEqualsFloat   ,&SQL_OperBitEqualsDouble   ,&SQL_OperBitEqualsBit   ,&SQL_OperBitEqualsSTiny   ,&SQL_OperBitEqualsUTiny   ,&SQL_OperBitEqualsSBig   ,&SQL_OperBitEqualsUBig   ,&SQL_OperBitEqualsNum   ,nullptr                ,nullptr                    ,nullptr                 ,nullptr                 ,nullptr                  ,nullptr                  ,nullptr                   }
  /* CT_STINYINT    */  ,{  &SQL_OperVarEqualsChar ,&SQL_OperSTinyEqualsSShort ,&SQL_OperSTinyEqualsUShort ,&SQL_OperSTinyEqualsSLong  ,&SQL_OperSTinyEqualsULong ,&SQL_OperSTinyEqualsFloat ,&SQL_OperSTinyEqualsDouble ,&SQL_OperSTinyEqualsBit ,&SQL_OperSTinyEqualsSTiny ,&SQL_OperSTinyEqualsUTiny ,&SQL_OperSTinyEqualsSBig ,&SQL_OperSTinyEqualsUBig ,&SQL_OperSTinyEqualsNum ,nullptr                ,nullptr                    ,nullptr                 ,nullptr                 ,nullptr                  ,nullptr                  ,nullptr                   }
  /* CT_UTINYINT    */  ,{  &SQL_OperVarEqualsChar ,&SQL_OperUTinyEqualsSShort ,&SQL_OperUTinyEqualsUShort ,&SQL_OperUTinyEqualsSLong  ,&SQL_OperUTinyEqualsULong ,&SQL_OperUTinyEqualsFloat ,&SQL_OperUTinyEqualsDouble ,&SQL_OperUTinyEqualsBit ,&SQL_OperUTinyEqualsSTiny ,&SQL_OperUTinyEqualsUTiny ,&SQL_OperUTinyEqualsSBig ,&SQL_OperUTinyEqualsUBig ,&SQL_OperUTinyEqualsNum ,nullptr                ,nullptr                    ,nullptr                 ,nullptr                 ,nullptr                  ,nullptr                  ,nullptr                   }
  /* CT_SBIGINT     */  ,{  &SQL_OperVarEqualsChar ,&SQL_OperSBigEqualsSShort  ,&SQL_OperSBigEqualsUShort  ,&SQL_OperSBigEqualsSLong   ,&SQL_OperSBigEqualsULong  ,&SQL_OperSBigEqualsFloat  ,&SQL_OperSBigEqualsDouble  ,&SQL_OperSBigEqualsBit  ,&SQL_OperSBigEqualsSTiny  ,&SQL_OperSBigEqualsUTiny  ,&SQL_OperSBigEqualsSBig  ,&SQL_OperSBigEqualsUBig  ,&SQL_OperSBigEqualsNum  ,nullptr                ,nullptr                    ,nullptr                 ,nullptr                 ,nullptr                  ,nullptr                  ,nullptr                   }
  /* CT_UBIGINT     */  ,{  &SQL_OperVarEqualsChar ,&SQL_OperUBigEqualsSShort  ,&SQL_OperUBigEqualsUShort  ,&SQL_OperUBigEqualsSLong   ,&SQL_OperUBigEqualsULong  ,&SQL_OperUBigEqualsFloat  ,&SQL_OperUBigEqualsDouble  ,&SQL_OperUBigEqualsBit  ,&SQL_OperUBigEqualsSTiny  ,&SQL_OperUBigEqualsUTiny  ,&SQL_OperUBigEqualsSBig  ,&SQL_OperUBigEqualsUBig  ,&SQL_OperUBigEqualsNum  ,nullptr                ,nullptr                    ,nullptr                 ,nullptr                 ,nullptr                  ,nullptr                  ,nullptr                   }
  /* CT_NUMERIC     */  ,{  &SQL_OperVarEqualsChar ,&SQL_OperNumEqualsSShort   ,&SQL_OperNumEqualsUShort   ,&SQL_OperNumEqualsSLong    ,&SQL_OperNumEqualsULong   ,&SQL_OperNumEqualsFloat   ,&SQL_OperNumEqualsDouble   ,&SQL_OperNumEqualsBit   ,&SQL_OperNumEqualsSTiny   ,&SQL_OperNumEqualsUTiny   ,&SQL_OperNumEqualsSBig   ,&SQL_OperNumEqualsUBig   ,&SQL_OperNumEqualsNum   ,nullptr                ,nullptr                    ,nullptr                 ,nullptr                 ,nullptr                  ,nullptr                  ,nullptr                   }
  /* CT_GUID        */  ,{  &SQL_OperGuidEqualsChar,nullptr                    ,nullptr                    ,nullptr                    ,nullptr                   ,nullptr                   ,nullptr                    ,nullptr                 ,nullptr                   ,nullptr                   ,nullptr                  ,nullptr                  ,nullptr                 ,&SQL_OperGuidEqualsGuid,nullptr                    ,nullptr                 ,nullptr                 ,nullptr                  ,nullptr                  ,nullptr                   }
  /* CT_BINARY      */  ,{  nullptr                ,nullptr                    ,nullptr                    ,nullptr                    ,nullptr                   ,nullptr                   ,nullptr                    ,nullptr                 ,nullptr                   ,nullptr                   ,nullptr                  ,nullptr                  ,nullptr                 ,nullptr                ,&SQL_OperBinaryEqualsBinary,nullptr                 ,nullptr                 ,nullptr                  ,nullptr                  ,nullptr                   }
  /* CT_DATE        */  ,{  &SQL_OperVarEqualsChar ,nullptr                    ,nullptr                    ,nullptr                    ,nullptr                   ,nullptr                   ,nullptr                    ,nullptr                 ,nullptr                   ,nullptr                   ,nullptr                  ,nullptr                  ,nullptr                 ,nullptr                ,nullptr                    ,&SQL_OperDateEqualsDate ,nullptr                 ,nullptr                  ,nullptr                  ,nullptr                   }
  /* CT_TIME        */  ,{  &SQL_OperVarEqualsChar ,nullptr                    ,nullptr                    ,nullptr                    ,nullptr                   ,nullptr                   ,nullptr                    ,nullptr                 ,nullptr                   ,nullptr                   ,nullptr                  ,nullptr                  ,nullptr                 ,nullptr                ,nullptr                    ,nullptr                 ,&SQL_OperTimeEqualsTime ,nullptr                  ,nullptr                  ,nullptr                   }
  /* CT_TIMESTAMP   */  ,{  &SQL_OperVarEqualsChar ,nullptr                    ,nullptr                    ,nullptr                    ,nullptr                   ,nullptr                   ,nullptr                    ,nullptr                 ,nullptr                   ,nullptr                   ,nullptr                  ,nullptr                  ,nullptr                 ,nullptr                ,nullptr                    ,&SQL_OperStampEqualsDate,&SQL_OperStampEqualsTime,&SQL_OperStampEqualsStamp,nullptr                  ,nullptr                   }
  /* CT_INTERVAL_YM */  ,{  &SQL_OperVarEqualsChar ,nullptr                    ,nullptr                    ,nullptr                    ,nullptr                   ,nullptr                   ,nullptr                    ,nullptr                 ,nullptr                   ,nullptr                   ,nullptr                  ,nullptr                  ,nullptr                 ,nullptr                ,nullptr                    ,nullptr                 ,nullptr                 ,nullptr                  ,&SQL_OperIntYMEqualsIntYM,nullptr                   }
  /* CT_INTERVAL_DS */  ,{  &SQL_OperVarEqualsChar ,nullptr                    ,nullptr                    ,nullptr                    ,nullptr                   ,nullptr                   ,nullptr                    ,nullptr                 ,nullptr                   ,nullptr                   ,nullptr                  ,nullptr                  ,nullptr                 ,nullptr                ,nullptr                    ,nullptr                 ,nullptr                 ,nullptr                  ,nullptr                  ,&SQL_OperIntDSEqualsIntDS }
};

// Equality operator for SQLVariant
bool
SQLVariant::operator==(SQLVariant& p_right)
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
  OperatorCompare function = OperatorEqual[left][right].function;
  if(function)
  {
    return (*function)(*this,p_right);
  }
  // No compare function found
  // Data types are not comparable
  XString leftType  = FindDatatype(m_datatype);
  XString rightType = FindDatatype(p_right.m_datatype);
  XString error;
  error.Format("Cannot do the equality operator on (%s == %s)",leftType.GetString(),rightType.GetString());
  throw StdException(error);
}

// Inequality operator for SQLVariant
bool
SQLVariant::operator!=(SQLVariant& p_right)
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
  OperatorCompare function = OperatorEqual[left][right].function;
  if(function)
  {
    // Because we use the equality function, 
    // we must do a negate of the result
    return !(*function)(*this,p_right);
  }
  // No compare function found
  // Data types are not comparable
  XString leftType  = FindDatatype(m_datatype);
  XString rightType = FindDatatype(p_right.m_datatype);
  XString error;
  error.Format("Cannot do the inequality operator on (%s != %s)",leftType.GetString(),rightType.GetString());
  throw StdException(error);
}

// End of namespace
}
