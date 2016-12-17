////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantOperatorGreater.h
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

// BEWARE
// THE CONTENTS OF THIS FILE IS PART (INCLUDED) OF THE DEFINITION OF SQLVariant
// This is the part of the class that takes care of the > operator


   // Arrays with comparison functions for the operators
   static CompareFunctionArray OperatorGreater[CT_LAST][CT_LAST];

   // Comparison functions for the EQUAL operator
   bool   OperVarGreaterChar     (SQLVariant& p_right);  // TYPE == CHAR
   bool   OperGuidGreaterChar    (SQLVariant& p_right);
   bool   OperSShortGreaterSShort(SQLVariant& p_right);  // TYPE == SSHORT
   bool   OperUShortGreaterSShort(SQLVariant& p_right);
   bool   OperSLongGreaterSShort (SQLVariant& p_right);
   bool   OperULongGreaterSShort (SQLVariant& p_right);
   bool   OperFloatGreaterSShort (SQLVariant& p_right);
   bool   OperDoubleGreaterSShort(SQLVariant& p_right);
   bool   OperBitGreaterSShort   (SQLVariant& p_right);
   bool   OperSTinyGreaterSShort (SQLVariant& p_right);
   bool   OperUTinyGreaterSShort (SQLVariant& p_right);
   bool   OperSBigGreaterSShort  (SQLVariant& p_right);
   bool   OperUBigGreaterSShort  (SQLVariant& p_right);
   bool   OperNumGreaterSShort   (SQLVariant& p_right);
   bool   OperSShortGreaterUShort(SQLVariant& p_right);  // TYPE == USHORT
   bool   OperUShortGreaterUShort(SQLVariant& p_right);
   bool   OperSLongGreaterUShort (SQLVariant& p_right);
   bool   OperUlongGreaterUShort (SQLVariant& p_right);
   bool   OperFloatGreaterUShort (SQLVariant& p_right);
   bool   OperDoubleGreaterUShort(SQLVariant& p_right);
   bool   OperBitGreaterUShort   (SQLVariant& p_right);
   bool   OperSTinyGreaterUShort (SQLVariant& p_right);
   bool   OperUTinyGreaterUShort (SQLVariant& p_right);
   bool   OperSBigGreaterUShort  (SQLVariant& p_right);
   bool   OperUBigGreaterUShort  (SQLVariant& p_right);
   bool   OperNumGreaterUShort   (SQLVariant& p_right);
   bool   OperSShortGreaterSLong (SQLVariant& p_right);  // TYPE == SLONG
   bool   OperUShortGreaterSLong (SQLVariant& p_right);
   bool   OperSLongGreaterSLong  (SQLVariant& p_right);
   bool   OperULongGreaterSLong  (SQLVariant& p_right);
   bool   OperFloatGreaterSLong  (SQLVariant& p_right);
   bool   OperDoubleEGreaterSLong(SQLVariant& p_right);
   bool   OperBitGreaterSLong    (SQLVariant& p_right);
   bool   OperSTinyGreaterSLong  (SQLVariant& p_right);
   bool   OperUTinyGreaterSLong  (SQLVariant& p_right);
   bool   OperSBigGreaterSLong   (SQLVariant& p_right);
   bool   OperUBigGreaterSLong   (SQLVariant& p_right);
   bool   OperNumGreaterSLong    (SQLVariant& p_right);
   bool   OperSShortGreaterULong (SQLVariant& p_right);  // TYPE == ULONG
   bool   OperUShortGreaterULong (SQLVariant& p_right);
   bool   OperSLongGreaterULong  (SQLVariant& p_right);
   bool   OperULongGreaterULong  (SQLVariant& p_right);
   bool   OperFloatGreaterULong  (SQLVariant& p_right);
   bool   OperDoubleGreaterULong (SQLVariant& p_right);
   bool   OperBitGreaterULong    (SQLVariant& p_right);
   bool   OperSTinyGreaterULong  (SQLVariant& p_right);
   bool   OperUTinyGreaterULong  (SQLVariant& p_right);
   bool   OperSBigGreaterULong   (SQLVariant& p_right);
   bool   OperUBigGreaterULong   (SQLVariant& p_right);
   bool   OperNumGreaterULong    (SQLVariant& p_right);
   bool   OperSShortGreaterFloat (SQLVariant& p_right);  // TYPE == FLOAT
   bool   OperUShortGreaterFloat (SQLVariant& p_right);
   bool   OperSLongGreaterFloat  (SQLVariant& p_right);
   bool   OperULongGreaterFloat  (SQLVariant& p_right);
   bool   OperFloatGreaterFloat  (SQLVariant& p_right);
   bool   OperDoubleGreaterFloat (SQLVariant& p_right);
   bool   OperBitGreaterFloat    (SQLVariant& p_right);
   bool   OperSTinyGreaterFloat  (SQLVariant& p_right);
   bool   OperUTinyGreaterFloat  (SQLVariant& p_right);
   bool   OperSBigGreaterFloat   (SQLVariant& p_right);
   bool   OperUBigGreaterFloat   (SQLVariant& p_right);
   bool   OperNumGreaterFloat    (SQLVariant& p_right);
   bool   OperSShortGreaterDouble(SQLVariant& p_right);  // TYPE == DOUBLE
   bool   OperUShortGreaterDouble(SQLVariant& p_right);
   bool   OperSLongGreaterDouble (SQLVariant& p_right);
   bool   OperULongGreaterDouble (SQLVariant& p_right);
   bool   OperFloatGreaterDouble (SQLVariant& p_right);
   bool   OperDoubleGreaterDouble(SQLVariant& p_right);
   bool   OperBitGreaterDouble   (SQLVariant& p_right);
   bool   OperSTinyGreaterDouble (SQLVariant& p_right);
   bool   OperUTinyGreaterDouble (SQLVariant& p_right);
   bool   OperSBigGreaterDouble  (SQLVariant& p_right);
   bool   OperUBigGreaterDouble  (SQLVariant& p_right);
   bool   OperNumGreaterDouble   (SQLVariant& p_right);
   bool   OperSShortGreaterBit   (SQLVariant& p_right);  // TYPE == BIT
   bool   OperUShortGreaterBit   (SQLVariant& p_right);
   bool   OperSLongGreaterBit    (SQLVariant& p_right);
   bool   OperULongGreaterBit    (SQLVariant& p_right);
   bool   OperFloatGreaterBit    (SQLVariant& p_right);
   bool   OperDoubleGreaterBit   (SQLVariant& p_right);
   bool   OperBitGreaterBit      (SQLVariant& p_right);
   bool   OperSTinyGreaterBit    (SQLVariant& p_right);
   bool   OperUTinyGreaterBit    (SQLVariant& p_right);
   bool   OperSBigGreaterBit     (SQLVariant& p_right);
   bool   OperUBigGreaterBit     (SQLVariant& p_right);
   bool   OperNumGreaterBit      (SQLVariant& p_right);
   bool   OperSShortGreaterSTiny (SQLVariant& p_right);  // TYPE == STINYINT
   bool   OperUShortGreaterSTiny (SQLVariant& p_right);
   bool   OperSLongGreaterSTiny  (SQLVariant& p_right);
   bool   OperULongGreaterSTiny  (SQLVariant& p_right);
   bool   OperFloatGreaterSTiny  (SQLVariant& p_right);
   bool   OperDoubleGreaterSTiny (SQLVariant& p_right);
   bool   OperBitGreaterSTiny    (SQLVariant& p_right);
   bool   OperSTinyGreaterSTiny  (SQLVariant& p_right);
   bool   OperUTinyGreaterSTiny  (SQLVariant& p_right);
   bool   OperSBigGreaterSTiny   (SQLVariant& p_right);
   bool   OperUBigGreaterSTiny   (SQLVariant& p_right);
   bool   OperNumGreaterSTiny    (SQLVariant& p_right);
   bool   OperSShortGreaterUTiny (SQLVariant& p_right);  // TYPE == UTINYINT
   bool   OperUShortGreaterUTiny (SQLVariant& p_right);
   bool   OperSLongGreaterUTiny  (SQLVariant& p_right);
   bool   OperULongGreaterUTiny  (SQLVariant& p_right);
   bool   OperFloatGreaterUTiny  (SQLVariant& p_right);
   bool   OperDoubleGreaterUTiny (SQLVariant& p_right);
   bool   OperBitGreaterUTiny    (SQLVariant& p_right);
   bool   OperSTinyGreaterUTiny  (SQLVariant& p_right);
   bool   OperUTinyGreaterUTiny  (SQLVariant& p_right);
   bool   OperSBigGreaterUTiny   (SQLVariant& p_right);
   bool   OperUBigGreaterUTiny   (SQLVariant& p_right);
   bool   OperNumGreaterUTiny    (SQLVariant& p_right);
   bool   OperSShortGreaterSBig  (SQLVariant& p_right);  // TYPE == SBIGINT
   bool   OperUShortGreaterSBig  (SQLVariant& p_right);
   bool   OperSLongGreaterSBig   (SQLVariant& p_right);
   bool   OperULongGreaterSBig   (SQLVariant& p_right);
   bool   OperFloatGreaterSBig   (SQLVariant& p_right);
   bool   OperDoubleGreaterSBig  (SQLVariant& p_right);
   bool   OperBitGreaterSBig     (SQLVariant& p_right);
   bool   OperSTinyGreaterSBig   (SQLVariant& p_right);
   bool   OperUTinyGreaterSBig   (SQLVariant& p_right);
   bool   OperSBigGreaterSBig    (SQLVariant& p_right);
   bool   OperUBigGreaterSBig    (SQLVariant& p_right);
   bool   OperNumGreaterSBig     (SQLVariant& p_right);
   bool   OperSShortGreaterUBig  (SQLVariant& p_right);  // TYPE == UBIGINT
   bool   OperUShortGreaterUBig  (SQLVariant& p_right);
   bool   OperSLongGreaterUBig   (SQLVariant& p_right);
   bool   OperULongGreaterUBig   (SQLVariant& p_right);
   bool   OperFloatGreaterUBig   (SQLVariant& p_right);
   bool   OperDoubleGreaterUBig  (SQLVariant& p_right);
   bool   OperBitGreaterUBig     (SQLVariant& p_right);
   bool   OperSTinyGreaterUBig   (SQLVariant& p_right);
   bool   OperUTinyGreaterUBig   (SQLVariant& p_right);
   bool   OperSBigGreaterUBig    (SQLVariant& p_right);
   bool   OperUBigGreaterUBig    (SQLVariant& p_right);
   bool   OperNumGreaterUBig     (SQLVariant& p_right);
   bool   OperSShortGreaterNum   (SQLVariant& p_right);  // TYPE == NUMERIC
   bool   OperUShortGreaterNum   (SQLVariant& p_right);
   bool   OperSLongGreaterNum    (SQLVariant& p_right);
   bool   OperULongGreaterNum    (SQLVariant& p_right);
   bool   OperFloatGreaterNum    (SQLVariant& p_right);
   bool   OperDoubleGreaterNum   (SQLVariant& p_right);
   bool   OperBitGreaterNum      (SQLVariant& p_right);
   bool   OperSTinyGreaterNum    (SQLVariant& p_right);
   bool   OperUTinyGreaterNum    (SQLVariant& p_right);
   bool   OperSBigGreaterNum     (SQLVariant& p_right);
   bool   OperUBigGreaterNum     (SQLVariant& p_right);
   bool   OperNumGreaterNum      (SQLVariant& p_right);
   bool   OperGuidGreaterGuid    (SQLVariant& p_right);  // TYPE == GUID
   bool   OperBinaryGreaterBinary(SQLVariant& p_right);  // TYPE == BINARY
   bool   OperDateGreaterDate    (SQLVariant& p_right);  // TYPE == DATE
   bool   OperStampGreaterDate   (SQLVariant& p_right);
   bool   OperTimeGreaterTime    (SQLVariant& p_right);  // TYPE == TIME
   bool   OperStampGreaterTime   (SQLVariant& p_right);
   bool   OperStampGreaterStamp  (SQLVariant& p_right);  // TYPE == TIMESTAMP
   bool   OperIntYMGreaterIntYM  (SQLVariant& p_right);  // TYPE == INTERVAL_YEAR_MONTH
   bool   OperIntDSGreaterIntDS  (SQLVariant& p_right);  // TYPE == INTERVAL_DAY_SECOND

