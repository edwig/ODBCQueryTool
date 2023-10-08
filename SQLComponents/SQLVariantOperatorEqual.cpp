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
static SQL_OperVarEqualsChar(const SQLVariant& p_left,const SQLVariant& p_right)
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
static SQL_OperBitEqualsChar(const SQLVariant& p_left,const SQLVariant& p_right)
{
  if((p_left.GetAsBit() == 0 && p_right.GetAsDouble() == 0.0) ||
     (p_left.GetAsBit() != 0 && p_right.GetAsDouble() != 0.0) )
  {
    return true;
  }
  return false;
}

bool
static SQL_OperGuidEqualsChar(const SQLVariant& p_left,const SQLVariant& p_right)
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
static SQL_OperSShortEqualsSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSShort() == p_right.GetAsSShort();
}

bool
static SQL_OperUShortEqualsSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUShort() == SQL_ShortToUShort(p_right.GetAsSShort());
}

bool
static SQL_OperSLongEqualsSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_SLongToShort(p_left.GetAsSLong()) == p_right.GetAsSShort();
}

bool
static SQL_OperULongEqualsSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_ULongToUShort(p_left.GetAsULong()) == SQL_ShortToUShort(p_right.GetAsSShort());
}

bool
static SQL_OperFloatEqualsSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_FloatToShort(p_left.GetAsFloat()) == p_right.GetAsSShort();
}

bool
static SQL_OperDoubleEqualsSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_DoubleToShort(p_left.GetAsDouble()) == p_right.GetAsSShort();
}

bool
static SQL_OperBitEqualsSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  if((p_left.GetAsBit() != 0 && p_right.GetAsSShort() != 0) ||
     (p_left.GetAsBit() == 0 && p_right.GetAsSShort() == 0) )
  {
    return true;
  }
  return false;
}

bool   
static SQL_OperSTinyEqualsSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() == SQL_ShortToTinyInt(p_right.GetAsSShort());
}

bool
static SQL_OperUTinyEqualsSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() == SQL_ShortToUTinyInt(p_right.GetAsSShort());
}

bool
static SQL_OperSBigEqualsSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_BIGINTToShort(p_left.GetAsSBigInt()) == p_right.GetAsSShort();
}

bool
static SQL_OperUBigEqualsSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_UBIGINTToShort(p_left.GetAsUBigInt()) == p_right.GetAsSShort();
}

bool
static SQL_OperNumEqualsSShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_SLongToShort(p_left.GetAsBCD().AsLong()) == p_right.GetAsSShort();
}

// UNSIGNED SHORT

bool   
static SQL_OperSShortEqualsUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_ShortToUShort(p_left.GetAsSShort()) == p_right.GetAsUShort();
}

bool   
static SQL_OperUShortEqualsUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUShort() == p_right.GetAsUShort();
}

bool
static SQL_OperSLongEqualsUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_SLongToUShort(p_left.GetAsSLong()) == p_right.GetAsUShort();
}

bool
static SQL_OperUlongEqualsUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_ULongToUShort(p_left.GetAsULong()) == p_right.GetAsUShort();
}

bool
static SQL_OperFloatEqualsUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_FloatToUShort(p_left.GetAsFloat()) == p_right.GetAsUShort();
}

bool
static SQL_OperDoubleEqualsUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_DoubleToUShort(p_left.GetAsDouble()) == p_right.GetAsUShort();
}

bool
static SQL_OperBitEqualsUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  if((p_left.GetAsBit() == 0 && p_right.GetAsUShort() == 0) ||
     (p_left.GetAsBit() != 0 && p_right.GetAsUShort() != 0) )
  {
    return true;
  }
  return false;
}

bool
static SQL_OperSTinyEqualsUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() == SQL_UShortToTinyInt(p_right.GetAsUShort());
}

bool
static SQL_OperUTinyEqualsUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() == SQL_UShortToUTinyInt(p_right.GetAsUShort());
}

bool
static SQL_OperSBigEqualsUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_BIGINTToUShort(p_left.GetAsSBigInt()) == p_right.GetAsUShort();
}

bool
static SQL_OperUBigEqualsUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_UBIGINTToUShort(p_left.GetAsUBigInt()) == p_right.GetAsUShort();
}

bool   
static SQL_OperNumEqualsUShort(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_SLongToUShort(p_left.GetAsBCD().AsLong()) == p_right.GetAsUShort();
}

// SIGNED LONG

bool   
static SQL_OperSShortEqualsSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSShort() == SQL_SLongToShort(p_right.GetAsSLong());
}

bool
static SQL_OperUShortEqualsSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUShort() == SQL_SLongToUShort(p_right.GetAsSLong());
}

bool
static SQL_OperSLongEqualsSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSLong() == p_right.GetAsSLong();
}

bool
static SQL_OperULongEqualsSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsULong() == SQL_LongToULong(p_right.GetAsSLong());
}

bool
static SQL_OperFloatEqualsSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_FloatToLong(p_left.GetAsFloat()) == p_right.GetAsSLong();
}

bool
static SQL_OperDoubleEequalsSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_DoubleToLong(p_left.GetAsDouble()) == p_right.GetAsSLong();
}

bool
static SQL_OperBitEqualsSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  if((p_left.GetAsBit() == 0 && p_right.GetAsSLong() == 0) ||
     (p_left.GetAsBit() != 0 && p_right.GetAsSLong() != 0) )
  {
    return true;
  }
  return false;
}

bool
static SQL_OperSTinyEqualsSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() == SQL_SLongToTinyInt(p_right.GetAsSLong());
}

bool
static SQL_OperUTinyEqualsSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() == SQL_SLongToUTinyInt(p_right.GetAsSLong());
}

bool
static SQL_OperSBigEqualsSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_BIGINTToLong(p_left.GetAsSBigInt()) == p_right.GetAsSLong();
}

bool
static SQL_OperUBigEqualsSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_UBIGINTToLong(p_left.GetAsUBigInt()) == p_right.GetAsSLong();
}

bool
static SQL_OperNumEqualsSLong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsBCD().AsLong() == p_right.GetAsSLong();
}

// UNSIGNED LONG

bool   
static SQL_OperSShortEqualsULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_ShortToUShort(p_left.GetAsSShort()) == SQL_ULongToUShort(p_right.GetAsULong());
}

bool
static SQL_OperUShortEqualsULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUShort() == SQL_ULongToUShort(p_right.GetAsULong());
}

bool
static SQL_OperSLongEqualsULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_LongToULong(p_left.GetAsSLong()) == p_right.GetAsULong();
}

bool
static SQL_OperULongEqualsULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsULong() == p_right.GetAsULong();
}

bool
static SQL_OperFloatEqualsULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_FloatToULong(p_left.GetAsFloat()) == p_right.GetAsULong();
}

bool
static SQL_OperDoubleEqualsULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_DoubleToULong(p_left.GetAsDouble()) == p_right.GetAsULong();
}

bool
static SQL_OperBitEqualsULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  if((p_left.GetAsBit() == 0 && p_right.GetAsULong() == 0) ||
     (p_left.GetAsBit() != 0 && p_right.GetAsULong() != 0) )
  {
    return true;
  }
  return false;
}

bool
static SQL_OperSTinyEqualsULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_TinyIntToUTinyInt(p_left.GetAsSTinyInt()) == SQL_ULongToUTinyInt(p_right.GetAsULong());
}

bool
static SQL_OperUTinyEqualsULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() == SQL_ULongToUTinyInt(p_right.GetAsULong());
}

bool
static SQL_OperSBigEqualsULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_BIGINTToULong(p_left.GetAsSBigInt()) == p_right.GetAsULong();
}

bool
static SQL_OperUBigEqualsULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_UBIGINTToULong(p_left.GetAsUBigInt()) == p_right.GetAsULong();
}

bool
static SQL_OperNumEqualsULong(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_UBIGINTToULong(p_left.GetAsBCD().AsUInt64()) == p_right.GetAsULong();
}

// FLOAT

bool   
static SQL_OperSShortEqualsFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSShort() == SQL_FloatToShort(p_right.GetAsFloat());
}

bool   
static SQL_OperUShortEqualsFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUShort() == SQL_FloatToUShort(p_right.GetAsFloat());
}

bool
static SQL_OperSLongEqualsFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSLong() == SQL_FloatToLong(p_right.GetAsFloat());
}

bool
static SQL_OperULongEqualsFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsULong() == SQL_FloatToULong(p_right.GetAsFloat());
}

bool
static SQL_OperFloatEqualsFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsFloat() == p_right.GetAsFloat();
}

bool
static SQL_OperDoubleEqualsFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_DoubleToFloat(p_left.GetAsDouble()) == p_right.GetAsFloat();
}

bool
static SQL_OperBitEqualsFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  if((p_left.GetAsBit() == 0 && p_right.GetAsFloat() == 0.0) ||
     (p_left.GetAsBit() != 0 && p_right.GetAsFloat() != 0.0) )
  {
    return true;
  }
  return false;
}

bool
static SQL_OperSTinyEqualsFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() == SQL_FloatToTinyInt(p_right.GetAsFloat());
}

bool
static SQL_OperUTinyEqualsFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() == SQL_FloatToUTinyInt(p_right.GetAsFloat());
}

bool
static SQL_OperSBigEqualsFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSBigInt() == SQL_FloatToBIGINT(p_right.GetAsFloat());
}

bool
static SQL_OperUBigEqualsFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUBigInt() == SQL_FloatToUBIGINT(p_right.GetAsFloat());
}

bool
static SQL_OperNumEqualsFloat(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsBCD().AsDouble() == (double) p_right.GetAsFloat();
}

// DOUBLE

bool
static SQL_OperSShortEqualsDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSShort() == SQL_DoubleToShort(p_right.GetAsDouble());
}

bool
static SQL_OperUShortEqualsDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUShort() == SQL_DoubleToUShort(p_right.GetAsDouble());
}

bool
static SQL_OperSLongEqualsDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSLong() == SQL_DoubleToLong(p_right.GetAsDouble());
}

bool
static SQL_OperULongEqualsDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsULong() == SQL_DoubleToULong(p_right.GetAsDouble());
}

bool
static SQL_OperFloatEqualsDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsFloat() == SQL_DoubleToFloat(p_right.GetAsDouble());
}

bool
static SQL_OperDoubleEqualsDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsDouble() == p_right.GetAsDouble();
}

bool
static SQL_OperBitEqualsDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  if((p_left.GetAsBit() == 0 && p_right.GetAsDouble() == 0.0) ||
     (p_left.GetAsBit() != 0 && p_right.GetAsDouble() != 0.0))
  {
    return true;
  }
  return false;
}

bool
static SQL_OperSTinyEqualsDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() == SQL_DoubleToTinyInt(p_right.GetAsDouble());
}

bool
static SQL_OperUTinyEqualsDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() == SQL_DoubleToUTinyInt(p_right.GetAsDouble());
}

bool
static SQL_OperSBigEqualsDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSBigInt() == SQL_DoubleToBIGINT(p_right.GetAsDouble());
}

bool
static SQL_OperUBigEqualsDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUBigInt() == SQL_DoubleToUBIGINT(p_right.GetAsDouble());
}

bool
static SQL_OperNumEqualsDouble(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsBCD().AsDouble() == p_right.GetAsDouble();
}

// BIT

bool
static SQL_OperVarEqualsBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  if((p_left.GetAsDouble() == 0.0 && p_right.GetAsBit() == 0) ||
     (p_left.GetAsDouble() != 0.0 && p_right.GetAsBit() != 0) )
  {
    return true;
  }
  return false;
}

bool   
static SQL_OperSShortEqualsBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  if((p_left.GetAsSShort() == 0 && p_right.GetAsBit() == 0) ||
     (p_left.GetAsSShort() != 0 && p_right.GetAsBit() != 0))
  {
    return true;
  }
  return false;
}

bool
static SQL_OperUShortEqualsBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  if((p_left.GetAsUShort() == 0 && p_right.GetAsBit() == 0) ||
     (p_left.GetAsUShort() != 0 && p_right.GetAsBit() != 0))
  {
    return true;
  }
  return false;
}

bool   
static SQL_OperSLongEqualsBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  if((p_left.GetAsSLong() == 0 && p_right.GetAsBit() == 0) ||
     (p_left.GetAsSLong() != 0 && p_right.GetAsBit() != 0))
  {
    return true;
  }
  return false;
}

bool
static SQL_OperULongEqualsBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  if((p_left.GetAsULong() == 0 && p_right.GetAsBit() == 0) ||
     (p_left.GetAsULong() != 0 && p_right.GetAsBit() != 0))
  {
    return true;
  }
  return false;
}

bool
static SQL_OperFloatEqualsBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  if((p_left.GetAsFloat() == 0.0 && p_right.GetAsBit() == 0) ||
     (p_left.GetAsFloat() != 0.0 && p_right.GetAsBit() != 0))
  {
    return true;
  }
  return false;
}

bool
static SQL_OperDoubleEqualsBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  if((p_left.GetAsDouble() == 0.0 && p_right.GetAsBit() == 0) ||
     (p_left.GetAsDouble() != 0.0 && p_right.GetAsBit() != 0))
  {
    return true;
  }
  return false;
}

bool
static SQL_OperBitEqualsBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsBit() == p_right.GetAsBit();
}

bool
static SQL_OperSTinyEqualsBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  if((p_left.GetAsSTinyInt() == 0 && p_right.GetAsBit() == 0) ||
     (p_left.GetAsSTinyInt() != 0 && p_right.GetAsBit() != 0))
  {
    return true;
  }
  return false;
}

bool
static SQL_OperUTinyEqualsBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  if((p_left.GetAsUTinyInt() == 0 && p_right.GetAsBit() == 0) ||
     (p_left.GetAsUTinyInt() != 0 && p_right.GetAsBit() != 0))
  {
    return true;
  }
  return false;
}

bool
static SQL_OperSBigEqualsBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  if((p_left.GetAsSBigInt() == 0 && p_right.GetAsBit() == 0) ||
     (p_left.GetAsSBigInt() != 0 && p_right.GetAsBit() != 0))
  {
    return true;
  }
  return false;
}

bool
static SQL_OperUBigEqualsBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  if((p_left.GetAsUBigInt() == 0 && p_right.GetAsBit() == 0) ||
     (p_left.GetAsUBigInt() != 0 && p_right.GetAsBit() != 0))
  {
    return true;
  }
  return false;
}

bool
static SQL_OperNumEqualsBit(const SQLVariant& p_left,const SQLVariant& p_right)
{
  XString left;
  p_left.GetAsString(left);
  int lf = _ttoi(left.GetString());
 
  if((lf == 0 && p_right.GetAsBit() == 0) ||
     (lf != 0 && p_right.GetAsBit() != 0))
  {
    return true;
  }
  return false;
}

// SIGNED TINYINT

bool
static SQL_OperSShortEqualsSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_ShortToTinyInt(p_left.GetAsSShort()) == p_right.GetAsSTinyInt();
}

bool
static SQL_OperUShortEqualsSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_UShortToTinyInt(p_left.GetAsUShort()) == p_right.GetAsSTinyInt();
}

bool
static SQL_OperSLongEqualsSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_SLongToTinyInt(p_left.GetAsSLong()) == p_right.GetAsSTinyInt();
}

bool
static SQL_OperULongEqualsSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_ULongToTinyInt(p_left.GetAsULong()) == p_right.GetAsSTinyInt();
}

bool
static SQL_OperFloatEqualsSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_FloatToTinyInt(p_left.GetAsFloat()) == p_right.GetAsSTinyInt();
}

bool
static SQL_OperDoubleEqualsSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_DoubleToTinyInt(p_left.GetAsDouble()) == p_right.GetAsSTinyInt();
}

bool
static SQL_OperBitEqualsSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  if((p_left.GetAsBit() == 0 && p_right.GetAsSTinyInt() == 0) ||
     (p_left.GetAsBit() != 0 && p_right.GetAsSTinyInt() != 0))
  {
    return true;
  }
  return false;
}

bool
static SQL_OperSTinyEqualsSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() == p_right.GetAsSTinyInt();
}

bool
static SQL_OperUTinyEqualsSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() == SQL_TinyIntToUTinyInt(p_right.GetAsSTinyInt());
}

bool
static SQL_OperSBigEqualsSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_SBIGINTToTinyInt(p_left.GetAsSBigInt()) == p_right.GetAsSTinyInt();
}

bool
static SQL_OperUBigEqualsSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_UBIGINTToTinyInt(p_left.GetAsUBigInt()) == p_right.GetAsSTinyInt();
}

bool
static SQL_OperNumEqualsSTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_SLongToTinyInt(p_left.GetAsBCD().AsLong()) == p_right.GetAsSTinyInt();
}

// UNSIGNED TINYINT

bool
static SQL_OperSShortEqualsUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_ShortToUTinyInt(p_left.GetAsSShort()) == p_right.GetAsUTinyInt();
}

bool
static SQL_OperUShortEqualsUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_UShortToUTinyInt(p_left.GetAsUShort()) == p_right.GetAsUTinyInt();
}

bool
static SQL_OperSLongEqualsUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_SLongToUTinyInt(p_left.GetAsSLong()) == p_right.GetAsUTinyInt();
}

bool
static SQL_OperULongEqualsUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_ULongToUTinyInt(p_left.GetAsULong()) == p_right.GetAsUTinyInt();
}

bool
static SQL_OperFloatEqualsUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_FloatToUTinyInt(p_left.GetAsFloat()) == p_right.GetAsUTinyInt();
}

bool
static SQL_OperDoubleEqualsUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_DoubleToUTinyInt(p_left.GetAsDouble()) == p_right.GetAsUTinyInt();
}

bool
static SQL_OperBitEqualsUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  if((p_left.GetAsBit() == 0 && p_right.GetAsUTinyInt() == 0) ||
     (p_left.GetAsBit() != 0 && p_right.GetAsUTinyInt() != 0))
  {
    return true;
  }
  return false;
}

bool
static SQL_OperSTinyEqualsUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_TinyIntToUTinyInt(p_left.GetAsSTinyInt()) == p_right.GetAsUTinyInt();
}

bool
static SQL_OperUTinyEqualsUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() == p_right.GetAsUTinyInt();
}

bool
static SQL_OperSBigEqualsUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_SBIGINTToUTinyInt(p_left.GetAsSBigInt()) == p_right.GetAsUTinyInt();
}

bool
static SQL_OperUBigEqualsUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_UBIGINTToUTinyInt(p_left.GetAsUBigInt()) == p_right.GetAsUTinyInt();
}

bool
static SQL_OperNumEqualsUTiny(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_SLongToUTinyInt(p_left.GetAsBCD().AsLong()) == p_right.GetAsUTinyInt();
}

// SIGNED BIGINT

bool
static SQL_OperSShortEqualsSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSShort() == SQL_BIGINTToShort(p_right.GetAsSBigInt());
}

bool
static SQL_OperUShortEqualsSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUShort() == SQL_BIGINTToUShort(p_right.GetAsSBigInt());
}

bool
static SQL_OperSLongEqualsSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSLong() == SQL_BIGINTToLong(p_right.GetAsSBigInt());
}

bool
static SQL_OperULongEqualsSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsULong() == SQL_BIGINTToULong(p_right.GetAsSBigInt());
}

bool
static SQL_OperFloatEqualsSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsFloat() == SQL_BIGINTToFloat(p_right.GetAsSBigInt());
}

bool
static SQL_OperDoubleEqualsSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsDouble() == SQL_BIGINTToDouble(p_right.GetAsSBigInt());
}

bool
static SQL_OperBitEqualsSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  if((p_left.GetAsBit() == 0 && p_right.GetAsSBigInt() == 0) ||
     (p_left.GetAsBit() != 0 && p_right.GetAsSBigInt() != 0))
  {
    return true;
  }
  return false;
}

bool
static SQL_OperSTinyEqualsSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() == SQL_SBIGINTToTinyInt(p_right.GetAsSBigInt());
}

bool
static SQL_OperUTinyEqualsSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() == SQL_SBIGINTToUTinyInt(p_right.GetAsSBigInt());
}

bool
static SQL_OperSBigEqualsSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSBigInt() == p_right.GetAsSBigInt();
}

bool
static SQL_OperUBigEqualsSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUBigInt() == SQL_SBIGINTToUBIGINT(p_right.GetAsSBigInt());
}

bool   
static SQL_OperNumEqualsSBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsBCD().AsInt64() == p_right.GetAsSBigInt();
}

// UNSIGNED BIGINT

bool
static SQL_OperSShortEqualsUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSShort() == SQL_UBIGINTToShort(p_right.GetAsUBigInt());
}

bool
static SQL_OperUShortEqualsUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUShort() == SQL_UBIGINTToUShort(p_right.GetAsUBigInt());
}

bool
static SQL_OperSLongEqualsUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSLong() == SQL_UBIGINTToLong(p_right.GetAsUBigInt());
}

bool
static SQL_OperULongEqualsUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsULong() == SQL_UBIGINTToULong(p_right.GetAsUBigInt());
}

bool
static SQL_OperFloatEqualsUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsFloat() == SQL_UBIGINTToFloat(p_right.GetAsUBigInt());
}

bool
static SQL_OperDoubleEqualsUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsDouble() == SQL_UBIGINTToDouble(p_right.GetAsUBigInt());
}

bool
static SQL_OperBitEqualsUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  if((p_left.GetAsBit() == 0 && p_right.GetAsUBigInt() == 0) ||
     (p_left.GetAsBit() != 0 && p_right.GetAsUBigInt() != 0))
  {
    return true;
  }
  return false;
}

bool
static SQL_OperSTinyEqualsUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() == SQL_UBIGINTToUTinyInt(p_right.GetAsUBigInt());
}

bool
static SQL_OperUTinyEqualsUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() == SQL_UBIGINTToUTinyInt(p_right.GetAsUBigInt());
}

bool
static SQL_OperSBigEqualsUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return SQL_SBIGINTToUBIGINT(p_left.GetAsSBigInt()) == p_right.GetAsUBigInt();
}

bool
static SQL_OperUBigEqualsUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUBigInt() == p_right.GetAsUBigInt();
}

bool
static SQL_OperNumEqualsUBig(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsBCD().AsUInt64() == p_right.GetAsUBigInt();
}

// NUMERIC

bool
static SQL_OperSShortEqualsNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSShort() == (short) p_right.GetAsBCD().AsLong();
}

bool
static SQL_OperUShortEqualsNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUShort() == (unsigned short) p_right.GetAsBCD().AsLong();
}

bool
static SQL_OperSLongEqualsNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSLong() == p_right.GetAsBCD().AsLong();
}

bool
static SQL_OperULongEqualsNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsULong() == (unsigned long) p_right.GetAsBCD().AsInt64();
}

bool
static SQL_OperFloatEqualsNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsFloat() == (float)p_right.GetAsBCD().AsDouble();
}

bool
static SQL_OperDoubleEqualsNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsDouble() == p_right.GetAsBCD().AsDouble();
}

bool
static SQL_OperBitEqualsNum(const SQLVariant& p_left,const SQLVariant& p_right)
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
static SQL_OperSTinyEqualsNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSTinyInt() == SQL_SLongToTinyInt(p_right.GetAsBCD().AsLong());
}

bool
static SQL_OperUTinyEqualsNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUTinyInt() == SQL_SLongToUTinyInt(p_right.GetAsBCD().AsLong());
}

bool
static SQL_OperSBigEqualsNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsSBigInt() == p_right.GetAsBCD().AsInt64();
}

bool
static SQL_OperUBigEqualsNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsUBigInt() == p_right.GetAsBCD().AsUInt64();
}

bool
static SQL_OperNumEqualsNum(const SQLVariant& p_left,const SQLVariant& p_right)
{
  return p_left.GetAsBCD() == p_right.GetAsBCD();
}

// TYPE == GUID

bool
static SQL_OperGuidEqualsGuid(const SQLVariant& p_left,const SQLVariant& p_right)
{
  const SQLGUID* left  = p_left .GetAsGUID();
  const SQLGUID* right = p_right.GetAsGUID();

  return memcmp(left,right,sizeof(SQLGUID)) == 0;
}

// TYPE == BINARY

bool
static SQL_OperBinaryEqualsBinary(const SQLVariant& p_left,const SQLVariant& p_right)
{
  const void* left   = p_left.GetAsBinary();
  const void* right  = p_right.GetAsBinary();
  // Take longest of two binaries in comparison
  long  length = p_left.GetBinaryLength() > p_right.GetBinaryLength() ? p_left.GetBinaryLength() : p_right.GetBinaryLength();

  return memcmp(left,right,length) == 0;
}

// TYPE == DATE

bool
static SQL_OperDateEqualsDate(const SQLVariant& p_left,const SQLVariant& p_right)
{
  // Relies on the == operator of SQLDate
  return p_left.GetAsSQLDate() == p_right.GetAsSQLDate();
}

bool
static SQL_OperStampEqualsDate(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLDate date = p_left.GetAsSQLTimestamp().AsSQLDate();
  return date == p_right.GetAsSQLDate();
}

// TYPE == TIME

bool
static SQL_OperTimeEqualsTime(const SQLVariant& p_left,const SQLVariant& p_right)
{
  // Relies on the == operator of SQLTime
  return p_left.GetAsSQLTime() == p_right.GetAsSQLTime();
}

bool
static SQL_OperStampEqualsTime(const SQLVariant& p_left,const SQLVariant& p_right)
{
  SQLTime time = p_left.GetAsSQLTimestamp().AsSQLTime();
  return time == p_right.GetAsSQLTime();
}

// TYPE == TIMESTAMP

bool
static SQL_OperStampEqualsStamp(const SQLVariant& p_left,const SQLVariant& p_right)
{
  // Relies on the == operator of SQLTimestamp
  return p_left.GetAsSQLTimestamp() == p_right.GetAsSQLTimestamp();
}

// TYPE == INTERVAL_YEAR_MONTH

bool
static SQL_OperIntYMEqualsIntYM(const SQLVariant& p_left,const SQLVariant& p_right)
{
  // Relies on the calculation of months in SQLInterval
  return p_left.GetAsSQLInterval().AsDatabaseDouble() == p_right.GetAsSQLInterval().AsDatabaseDouble();
}

// TYPE == INTERVAL_DAY_SECOND

bool
static SQL_OperIntDSEqualsIntDS(const SQLVariant& p_left,const SQLVariant& p_right)
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
SQLVariant::operator==(const SQLVariant& p_right) const
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
  XString leftType  = SQLDataType::FindDatatype(m_datatype);
  XString rightType = SQLDataType::FindDatatype(p_right.m_datatype);
  XString error;
  error.Format(_T("Cannot do the equality operator on (%s == %s)"),leftType.GetString(),rightType.GetString());
  throw StdException(error);
}

// Inequality operator for SQLVariant
bool
SQLVariant::operator!=(const SQLVariant& p_right) const
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
  XString leftType  = SQLDataType::FindDatatype(m_datatype);
  XString rightType = SQLDataType::FindDatatype(p_right.m_datatype);
  XString error;
  error.Format(_T("Cannot do the inequality operator on (%s != %s)"),leftType.GetString(),rightType.GetString());
  throw StdException(error);
}

// End of namespace
}
