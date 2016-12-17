////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantOperatorMul.h
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
// This is the part of the class that takes care of the * operator

   // Arrays with calculation functions for the operators
   static CalculateFunctionArray OperatorMul[CT_LAST][CT_LAST];

   // Calculate functions for the multiplication operator
   SQLVariant OperSShortMulChar  (SQLVariant& p_right);  // TYPE == CHAR
   SQLVariant OperUShortMulChar  (SQLVariant& p_right);
   SQLVariant OperSLongMulChar   (SQLVariant& p_right);
   SQLVariant OperULongMulChar   (SQLVariant& p_right);
   SQLVariant OperFloatMulChar   (SQLVariant& p_right);
   SQLVariant OperDoubleMulChar  (SQLVariant& p_right);
   SQLVariant OperBitMulChar     (SQLVariant& p_right);
   SQLVariant OperSTinyMulChar   (SQLVariant& p_right);
   SQLVariant OperUTinyMulChar   (SQLVariant& p_right);
   SQLVariant OperSBigMulChar    (SQLVariant& p_right);
   SQLVariant OperUBigMulChar    (SQLVariant& p_right);
   SQLVariant OperNumMulChar     (SQLVariant& p_right);
   SQLVariant OperCharMulSShort  (SQLVariant& p_right);  // TYPE == SSHORT
   SQLVariant OperSShortMulSShort(SQLVariant& p_right);
   SQLVariant OperUShortMulSShort(SQLVariant& p_right);
   SQLVariant OperSLongMulSShort (SQLVariant& p_right);
   SQLVariant OperULongMulSShort (SQLVariant& p_right);
   SQLVariant OperFloatMulSShort (SQLVariant& p_right);
   SQLVariant OperDoubleMulSShort(SQLVariant& p_right);
   SQLVariant OperBitMulSShort   (SQLVariant& p_right);
   SQLVariant OperSTinyMulSShort (SQLVariant& p_right);
   SQLVariant OperUTinyMulSShort (SQLVariant& p_right);
   SQLVariant OperSBigMulSShort  (SQLVariant& p_right);
   SQLVariant OperUBigMulSShort  (SQLVariant& p_right);
   SQLVariant OperNumMulSShort   (SQLVariant& p_right);
   SQLVariant OperIntYMMulSShort (SQLVariant& p_right);
   SQLVariant OperIntDSMulSShort (SQLVariant& p_right);
   SQLVariant OperCharMulUShort  (SQLVariant& p_right);  // TYPE == USHORT
   SQLVariant OperSShortMulUShort(SQLVariant& p_right);
   SQLVariant OperUShortMulUShort(SQLVariant& p_right);
   SQLVariant OperSLongMulUShort (SQLVariant& p_right);
   SQLVariant OperUlongMulUShort (SQLVariant& p_right);
   SQLVariant OperFloatMulUShort (SQLVariant& p_right);
   SQLVariant OperDoubleMulUShort(SQLVariant& p_right);
   SQLVariant OperBitMulUShort   (SQLVariant& p_right);
   SQLVariant OperSTinyMulUShort (SQLVariant& p_right);
   SQLVariant OperUTinyMulUShort (SQLVariant& p_right);
   SQLVariant OperSBigMulUShort  (SQLVariant& p_right);
   SQLVariant OperUBigMulUShort  (SQLVariant& p_right);
   SQLVariant OperNumMulUShort   (SQLVariant& p_right);
   SQLVariant OperIntYMMulUShort (SQLVariant& p_right);
   SQLVariant OperIntDSMulUShort (SQLVariant& p_right);
   SQLVariant OperCharMulSLong   (SQLVariant& p_right);  // TYPE == SLONG
   SQLVariant OperSShortMulSLong (SQLVariant& p_right);
   SQLVariant OperUShortMulSLong (SQLVariant& p_right);
   SQLVariant OperSLongMulSLong  (SQLVariant& p_right);
   SQLVariant OperULongMulSLong  (SQLVariant& p_right);
   SQLVariant OperFloatMulSLong  (SQLVariant& p_right);
   SQLVariant OperDoubleMulSLong (SQLVariant& p_right);
   SQLVariant OperBitMulSLong    (SQLVariant& p_right);
   SQLVariant OperSTinyMulSLong  (SQLVariant& p_right);
   SQLVariant OperUTinyMulSLong  (SQLVariant& p_right);
   SQLVariant OperSBigMulSLong   (SQLVariant& p_right);
   SQLVariant OperUBigMulSLong   (SQLVariant& p_right);
   SQLVariant OperNumMulSLong    (SQLVariant& p_right);
   SQLVariant OperIntYMMulSLong  (SQLVariant& p_right);
   SQLVariant OperIntDSMulSLong  (SQLVariant& p_right);
   SQLVariant OperCharMulULong   (SQLVariant& p_right);  // TYPE == ULONG
   SQLVariant OperSShortMulULong (SQLVariant& p_right);
   SQLVariant OperUShortMulULong (SQLVariant& p_right);
   SQLVariant OperSLongMulULong  (SQLVariant& p_right);
   SQLVariant OperULongMulULong  (SQLVariant& p_right);
   SQLVariant OperFloatMulULong  (SQLVariant& p_right);
   SQLVariant OperDoubleMulULong (SQLVariant& p_right);
   SQLVariant OperBitMulULong    (SQLVariant& p_right);
   SQLVariant OperSTinyMulULong  (SQLVariant& p_right);
   SQLVariant OperUTinyMulULong  (SQLVariant& p_right);
   SQLVariant OperSBigMulULong   (SQLVariant& p_right);
   SQLVariant OperUBigMulULong   (SQLVariant& p_right);
   SQLVariant OperNumMulULong    (SQLVariant& p_right);
   SQLVariant OperIntYMMulULong  (SQLVariant& p_right);
   SQLVariant OperIntDSMulULong  (SQLVariant& p_right);
   SQLVariant OperCharMulFloat   (SQLVariant& p_right);  // TYPE == FLOAT
   SQLVariant OperSShortMulFloat (SQLVariant& p_right);
   SQLVariant OperUShortMulFloat (SQLVariant& p_right);
   SQLVariant OperSLongMulFloat  (SQLVariant& p_right);
   SQLVariant OperULongMulFloat  (SQLVariant& p_right);
   SQLVariant OperFloatMulFloat  (SQLVariant& p_right);
   SQLVariant OperDoubleMulFloat (SQLVariant& p_right);
   SQLVariant OperBitMulFloat    (SQLVariant& p_right);
   SQLVariant OperSTinyMulFloat  (SQLVariant& p_right);
   SQLVariant OperUTinyMulFloat  (SQLVariant& p_right);
   SQLVariant OperSBigMulFloat   (SQLVariant& p_right);
   SQLVariant OperUBigMulFloat   (SQLVariant& p_right);
   SQLVariant OperNumMulFloat    (SQLVariant& p_right);
   SQLVariant OperIntYMMulFloat  (SQLVariant& p_right);
   SQLVariant OperIntDSMulFloat  (SQLVariant& p_right);
   SQLVariant OperCharMulDouble  (SQLVariant& p_right);  // TYPE == DOUBLE
   SQLVariant OperSShortMulDouble(SQLVariant& p_right);
   SQLVariant OperUShortMulDouble(SQLVariant& p_right);
   SQLVariant OperSLongMulDouble (SQLVariant& p_right);
   SQLVariant OperULongMulDouble (SQLVariant& p_right);
   SQLVariant OperFloatMulDouble (SQLVariant& p_right);
   SQLVariant OperDoubleMulDouble(SQLVariant& p_right);
   SQLVariant OperBitMulDouble   (SQLVariant& p_right);
   SQLVariant OperSTinyMulDouble (SQLVariant& p_right);
   SQLVariant OperUTinyMulDouble (SQLVariant& p_right);
   SQLVariant OperSBigMulDouble  (SQLVariant& p_right);
   SQLVariant OperUBigMulDouble  (SQLVariant& p_right);
   SQLVariant OperNumMulDouble   (SQLVariant& p_right);
   SQLVariant OperIntYMMulDouble (SQLVariant& p_right);
   SQLVariant OperIntDSMulDouble (SQLVariant& p_right);
   SQLVariant OperCharMulBit     (SQLVariant& p_right);  // TYPE == BIT
   SQLVariant OperSShortMulBit   (SQLVariant& p_right);
   SQLVariant OperUShortMulBit   (SQLVariant& p_right);
   SQLVariant OperSLongMulBit    (SQLVariant& p_right);
   SQLVariant OperULongMulBit    (SQLVariant& p_right);
   SQLVariant OperFloatMulBit    (SQLVariant& p_right);
   SQLVariant OperDoubleMulBit   (SQLVariant& p_right);
   SQLVariant OperBitMulBit      (SQLVariant& p_right);
   SQLVariant OperSTinyMulBit    (SQLVariant& p_right);
   SQLVariant OperUTinyMulBit    (SQLVariant& p_right);
   SQLVariant OperSBigMulBit     (SQLVariant& p_right);
   SQLVariant OperUBigMulBit     (SQLVariant& p_right);
   SQLVariant OperNumMulBit      (SQLVariant& p_right);
   SQLVariant OperIntYMMulBit    (SQLVariant& p_right);
   SQLVariant OperIntDSMulBit    (SQLVariant& p_right);
   SQLVariant OperCharMulSTiny   (SQLVariant& p_right);  // TYPE == STINYINT
   SQLVariant OperSShortMulSTiny (SQLVariant& p_right);
   SQLVariant OperUShortMulSTiny (SQLVariant& p_right);
   SQLVariant OperSLongMulSTiny  (SQLVariant& p_right);
   SQLVariant OperULongMulSTiny  (SQLVariant& p_right);
   SQLVariant OperFloatMulSTiny  (SQLVariant& p_right);
   SQLVariant OperDoubleMulSTiny (SQLVariant& p_right);
   SQLVariant OperBitMulSTiny    (SQLVariant& p_right);
   SQLVariant OperSTinyMulSTiny  (SQLVariant& p_right);
   SQLVariant OperUTinyMulSTiny  (SQLVariant& p_right);
   SQLVariant OperSBigMulSTiny   (SQLVariant& p_right);
   SQLVariant OperUBigMulSTiny   (SQLVariant& p_right);
   SQLVariant OperNumMulSTiny    (SQLVariant& p_right);
   SQLVariant OperIntYMMulSTiny  (SQLVariant& p_right);
   SQLVariant OperIntDSMulSTiny  (SQLVariant& p_right);
   SQLVariant OperCharMulUTiny   (SQLVariant& p_right);  // TYPE == UTINYINT
   SQLVariant OperSShortMulUTiny (SQLVariant& p_right);
   SQLVariant OperUShortMulUTiny (SQLVariant& p_right);
   SQLVariant OperSLongMulUTiny  (SQLVariant& p_right);
   SQLVariant OperULongMulUTiny  (SQLVariant& p_right);
   SQLVariant OperFloatMulUTiny  (SQLVariant& p_right);
   SQLVariant OperDoubleMulUTiny (SQLVariant& p_right);
   SQLVariant OperBitMulUTiny    (SQLVariant& p_right);
   SQLVariant OperSTinyMulUTiny  (SQLVariant& p_right);
   SQLVariant OperUTinyMulUTiny  (SQLVariant& p_right);
   SQLVariant OperSBigMulUTiny   (SQLVariant& p_right);
   SQLVariant OperUBigMulUTiny   (SQLVariant& p_right);
   SQLVariant OperNumMulUTiny    (SQLVariant& p_right);
   SQLVariant OperIntYMMulUTiny  (SQLVariant& p_right);
   SQLVariant OperIntDSMulUTiny  (SQLVariant& p_right);
   SQLVariant OperCharMulSBig    (SQLVariant& p_right);  // TYPE == SBIGINT
   SQLVariant OperSShortMulSBig  (SQLVariant& p_right);
   SQLVariant OperUShortMulSBig  (SQLVariant& p_right);
   SQLVariant OperSLongMulSBig   (SQLVariant& p_right);
   SQLVariant OperULongMulSBig   (SQLVariant& p_right);
   SQLVariant OperFloatMulSBig   (SQLVariant& p_right);
   SQLVariant OperDoubleMulSBig  (SQLVariant& p_right);
   SQLVariant OperBitMulSBig     (SQLVariant& p_right);
   SQLVariant OperSTinyMulSBig   (SQLVariant& p_right);
   SQLVariant OperUTinyMulSBig   (SQLVariant& p_right);
   SQLVariant OperSBigMulSBig    (SQLVariant& p_right);
   SQLVariant OperUBigMulSBig    (SQLVariant& p_right);
   SQLVariant OperNumMulSBig     (SQLVariant& p_right);
   SQLVariant OperIntYMMulSBig   (SQLVariant& p_right);
   SQLVariant OperIntDSMulSBig   (SQLVariant& p_right);
   SQLVariant OperCharMulUBig    (SQLVariant& p_right);  // TYPE == UBIGINT
   SQLVariant OperSShortMulUBig  (SQLVariant& p_right);
   SQLVariant OperUShortMulUBig  (SQLVariant& p_right);
   SQLVariant OperSLongMulUBig   (SQLVariant& p_right);
   SQLVariant OperULongMulUBig   (SQLVariant& p_right);
   SQLVariant OperFloatMulUBig   (SQLVariant& p_right);
   SQLVariant OperDoubleMulUBig  (SQLVariant& p_right);
   SQLVariant OperBitMulUBig     (SQLVariant& p_right);
   SQLVariant OperSTinyMulUBig   (SQLVariant& p_right);
   SQLVariant OperUTinyMulUBig   (SQLVariant& p_right);
   SQLVariant OperSBigMulUBig    (SQLVariant& p_right);
   SQLVariant OperUBigMulUBig    (SQLVariant& p_right);
   SQLVariant OperNumMulUBig     (SQLVariant& p_right);
   SQLVariant OperIntYMMulUBig   (SQLVariant& p_right);
   SQLVariant OperIntDSMulUBig   (SQLVariant& p_right);
   SQLVariant OperCharMulNum     (SQLVariant& p_right);  // TYPE == NUMERIC
   SQLVariant OperSShortMulNum   (SQLVariant& p_right);
   SQLVariant OperUShortMulNum   (SQLVariant& p_right);
   SQLVariant OperSLongMulNum    (SQLVariant& p_right);
   SQLVariant OperULongMulNum    (SQLVariant& p_right);
   SQLVariant OperFloatMulNum    (SQLVariant& p_right);
   SQLVariant OperDoubleMulNum   (SQLVariant& p_right);
   SQLVariant OperBitMulNum      (SQLVariant& p_right);
   SQLVariant OperSTinyMulNum    (SQLVariant& p_right);
   SQLVariant OperUTinyMulNum    (SQLVariant& p_right);
   SQLVariant OperSBigMulNum     (SQLVariant& p_right);
   SQLVariant OperUBigMulNum     (SQLVariant& p_right);
   SQLVariant OperNumMulNum      (SQLVariant& p_right);
   SQLVariant OperIntYMMulNum    (SQLVariant& p_right);  // TYPE == Interval YEAR-MONTH
   SQLVariant OperIntDSMulNum    (SQLVariant& p_right);  // TYPE == Interval DAY-SECOND
