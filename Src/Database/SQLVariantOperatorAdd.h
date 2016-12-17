////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantOperatorAdd.h
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
// This is the part of the class that takes care of the + operator


   // Arrays with comparison functions for the operators
   static CalculateFunctionArray OperatorAdd[CT_LAST][CT_LAST];

   // Calculate functions for the ADD operator
   SQLVariant OperCharAddChar    (SQLVariant& p_right);  // TYPE == CHAR
   SQLVariant OperSShortAddChar  (SQLVariant& p_right);
   SQLVariant OperUShortAddChar  (SQLVariant& p_right);
   SQLVariant OperSLongAddChar   (SQLVariant& p_right);
   SQLVariant OperULongAddChar   (SQLVariant& p_right);
   SQLVariant OperFloatAddChar   (SQLVariant& p_right);
   SQLVariant OperDoubleAddChar  (SQLVariant& p_right);
   SQLVariant OperBitAddChar     (SQLVariant& p_right);
   SQLVariant OperSTinyAddChar   (SQLVariant& p_right);
   SQLVariant OperUTinyAddChar   (SQLVariant& p_right);
   SQLVariant OperSBigAddChar    (SQLVariant& p_right);
   SQLVariant OperUBigAddChar    (SQLVariant& p_right);
   SQLVariant OperNumAddChar     (SQLVariant& p_right);
   SQLVariant OperCharAddSShort  (SQLVariant& p_right);  // TYPE == SSHORT
   SQLVariant OperSShortAddSShort(SQLVariant& p_right);
   SQLVariant OperUShortAddSShort(SQLVariant& p_right);
   SQLVariant OperSLongAddSShort (SQLVariant& p_right);
   SQLVariant OperULongAddSShort (SQLVariant& p_right);
   SQLVariant OperFloatAddSShort (SQLVariant& p_right);
   SQLVariant OperDoubleAddSShort(SQLVariant& p_right);
   SQLVariant OperBitAddSShort   (SQLVariant& p_right);
   SQLVariant OperSTinyAddSShort (SQLVariant& p_right);
   SQLVariant OperUTinyAddSShort (SQLVariant& p_right);
   SQLVariant OperSBigAddSShort  (SQLVariant& p_right);
   SQLVariant OperUBigAddSShort  (SQLVariant& p_right);
   SQLVariant OperNumAddSShort   (SQLVariant& p_right);
   SQLVariant OperCharAddUShort  (SQLVariant& p_right);  // TYPE == USHORT
   SQLVariant OperSShortAddUShort(SQLVariant& p_right);
   SQLVariant OperUShortAddUShort(SQLVariant& p_right);
   SQLVariant OperSLongAddUShort (SQLVariant& p_right);
   SQLVariant OperUlongAddUShort (SQLVariant& p_right);
   SQLVariant OperFloatAddUShort (SQLVariant& p_right);
   SQLVariant OperDoubleAddUShort(SQLVariant& p_right);
   SQLVariant OperBitAddUShort   (SQLVariant& p_right);
   SQLVariant OperSTinyAddUShort (SQLVariant& p_right);
   SQLVariant OperUTinyAddUShort (SQLVariant& p_right);
   SQLVariant OperSBigAddUShort  (SQLVariant& p_right);
   SQLVariant OperUBigAddUShort  (SQLVariant& p_right);
   SQLVariant OperNumAddUShort   (SQLVariant& p_right);
   SQLVariant OperCharAddSLong   (SQLVariant& p_right);  // TYPE == SLONG
   SQLVariant OperSShortAddSLong (SQLVariant& p_right);
   SQLVariant OperUShortAddSLong (SQLVariant& p_right);
   SQLVariant OperSLongAddSLong  (SQLVariant& p_right);
   SQLVariant OperULongAddSLong  (SQLVariant& p_right);
   SQLVariant OperFloatAddSLong  (SQLVariant& p_right);
   SQLVariant OperDoubleAddSLong (SQLVariant& p_right);
   SQLVariant OperBitAddSLong    (SQLVariant& p_right);
   SQLVariant OperSTinyAddSLong  (SQLVariant& p_right);
   SQLVariant OperUTinyAddSLong  (SQLVariant& p_right);
   SQLVariant OperSBigAddSLong   (SQLVariant& p_right);
   SQLVariant OperUBigAddSLong   (SQLVariant& p_right);
   SQLVariant OperNumAddSLong    (SQLVariant& p_right);
   SQLVariant OperCharAddULong   (SQLVariant& p_right);  // TYPE == ULONG
   SQLVariant OperSShortAddULong (SQLVariant& p_right);
   SQLVariant OperUShortAddULong (SQLVariant& p_right);
   SQLVariant OperSLongAddULong  (SQLVariant& p_right);
   SQLVariant OperULongAddULong  (SQLVariant& p_right);
   SQLVariant OperFloatAddULong  (SQLVariant& p_right);
   SQLVariant OperDoubleAddULong (SQLVariant& p_right);
   SQLVariant OperBitAddULong    (SQLVariant& p_right);
   SQLVariant OperSTinyAddULong  (SQLVariant& p_right);
   SQLVariant OperUTinyAddULong  (SQLVariant& p_right);
   SQLVariant OperSBigAddULong   (SQLVariant& p_right);
   SQLVariant OperUBigAddULong   (SQLVariant& p_right);
   SQLVariant OperNumAddULong    (SQLVariant& p_right);
   SQLVariant OperCharAddFloat   (SQLVariant& p_right);  // TYPE == FLOAT
   SQLVariant OperSShortAddFloat (SQLVariant& p_right);
   SQLVariant OperUShortAddFloat (SQLVariant& p_right);
   SQLVariant OperSLongAddFloat  (SQLVariant& p_right);
   SQLVariant OperULongAddFloat  (SQLVariant& p_right);
   SQLVariant OperFloatAddFloat  (SQLVariant& p_right);
   SQLVariant OperDoubleAddFloat (SQLVariant& p_right);
   SQLVariant OperBitAddFloat    (SQLVariant& p_right);
   SQLVariant OperSTinyAddFloat  (SQLVariant& p_right);
   SQLVariant OperUTinyAddFloat  (SQLVariant& p_right);
   SQLVariant OperSBigAddFloat   (SQLVariant& p_right);
   SQLVariant OperUBigAddFloat   (SQLVariant& p_right);
   SQLVariant OperNumAddFloat    (SQLVariant& p_right);
   SQLVariant OperCharAddDouble  (SQLVariant& p_right);  // TYPE == DOUBLE
   SQLVariant OperSShortAddDouble(SQLVariant& p_right);
   SQLVariant OperUShortAddDouble(SQLVariant& p_right);
   SQLVariant OperSLongAddDouble (SQLVariant& p_right);
   SQLVariant OperULongAddDouble (SQLVariant& p_right);
   SQLVariant OperFloatAddDouble (SQLVariant& p_right);
   SQLVariant OperDoubleAddDouble(SQLVariant& p_right);
   SQLVariant OperBitAddDouble   (SQLVariant& p_right);
   SQLVariant OperSTinyAddDouble (SQLVariant& p_right);
   SQLVariant OperUTinyAddDouble (SQLVariant& p_right);
   SQLVariant OperSBigAddDouble  (SQLVariant& p_right);
   SQLVariant OperUBigAddDouble  (SQLVariant& p_right);
   SQLVariant OperNumAddDouble   (SQLVariant& p_right);
   SQLVariant OperCharAddBit     (SQLVariant& p_right);  // TYPE == BIT
   SQLVariant OperSShortAddBit   (SQLVariant& p_right);
   SQLVariant OperUShortAddBit   (SQLVariant& p_right);
   SQLVariant OperSLongAddBit    (SQLVariant& p_right);
   SQLVariant OperULongAddBit    (SQLVariant& p_right);
   SQLVariant OperFloatAddBit    (SQLVariant& p_right);
   SQLVariant OperDoubleAddBit   (SQLVariant& p_right);
   SQLVariant OperBitAddBit      (SQLVariant& p_right);
   SQLVariant OperSTinyAddBit    (SQLVariant& p_right);
   SQLVariant OperUTinyAddBit    (SQLVariant& p_right);
   SQLVariant OperSBigAddBit     (SQLVariant& p_right);
   SQLVariant OperUBigAddBit     (SQLVariant& p_right);
   SQLVariant OperNumAddBit      (SQLVariant& p_right);
   SQLVariant OperCharAddSTiny   (SQLVariant& p_right);  // TYPE == STINYINT
   SQLVariant OperSShortAddSTiny (SQLVariant& p_right);
   SQLVariant OperUShortAddSTiny (SQLVariant& p_right);
   SQLVariant OperSLongAddSTiny  (SQLVariant& p_right);
   SQLVariant OperULongAddSTiny  (SQLVariant& p_right);
   SQLVariant OperFloatAddSTiny  (SQLVariant& p_right);
   SQLVariant OperDoubleAddSTiny (SQLVariant& p_right);
   SQLVariant OperBitAddSTiny    (SQLVariant& p_right);
   SQLVariant OperSTinyAddSTiny  (SQLVariant& p_right);
   SQLVariant OperUTinyAddSTiny  (SQLVariant& p_right);
   SQLVariant OperSBigAddSTiny   (SQLVariant& p_right);
   SQLVariant OperUBigAddSTiny   (SQLVariant& p_right);
   SQLVariant OperNumAddSTiny    (SQLVariant& p_right);
   SQLVariant OperCharAddUTiny   (SQLVariant& p_right);  // TYPE == UTINYINT
   SQLVariant OperSShortAddUTiny (SQLVariant& p_right);
   SQLVariant OperUShortAddUTiny (SQLVariant& p_right);
   SQLVariant OperSLongAddUTiny  (SQLVariant& p_right);
   SQLVariant OperULongAddUTiny  (SQLVariant& p_right);
   SQLVariant OperFloatAddUTiny  (SQLVariant& p_right);
   SQLVariant OperDoubleAddUTiny (SQLVariant& p_right);
   SQLVariant OperBitAddUTiny    (SQLVariant& p_right);
   SQLVariant OperSTinyAddUTiny  (SQLVariant& p_right);
   SQLVariant OperUTinyAddUTiny  (SQLVariant& p_right);
   SQLVariant OperSBigAddUTiny   (SQLVariant& p_right);
   SQLVariant OperUBigAddUTiny   (SQLVariant& p_right);
   SQLVariant OperNumAddUTiny    (SQLVariant& p_right);
   SQLVariant OperCharAddSBig    (SQLVariant& p_right);  // TYPE == SBIGINT
   SQLVariant OperSShortAddSBig  (SQLVariant& p_right);
   SQLVariant OperUShortAddSBig  (SQLVariant& p_right);
   SQLVariant OperSLongAddSBig   (SQLVariant& p_right);
   SQLVariant OperULongAddSBig   (SQLVariant& p_right);
   SQLVariant OperFloatAddSBig   (SQLVariant& p_right);
   SQLVariant OperDoubleAddSBig  (SQLVariant& p_right);
   SQLVariant OperBitAddSBig     (SQLVariant& p_right);
   SQLVariant OperSTinyAddSBig   (SQLVariant& p_right);
   SQLVariant OperUTinyAddSBig   (SQLVariant& p_right);
   SQLVariant OperSBigAddSBig    (SQLVariant& p_right);
   SQLVariant OperUBigAddSBig    (SQLVariant& p_right);
   SQLVariant OperNumAddSBig     (SQLVariant& p_right);
   SQLVariant OperCharAddUBig    (SQLVariant& p_right);  // TYPE == UBIGINT
   SQLVariant OperSShortAddUBig  (SQLVariant& p_right);
   SQLVariant OperUShortAddUBig  (SQLVariant& p_right);
   SQLVariant OperSLongAddUBig   (SQLVariant& p_right);
   SQLVariant OperULongAddUBig   (SQLVariant& p_right);
   SQLVariant OperFloatAddUBig   (SQLVariant& p_right);
   SQLVariant OperDoubleAddUBig  (SQLVariant& p_right);
   SQLVariant OperBitAddUBig     (SQLVariant& p_right);
   SQLVariant OperSTinyAddUBig   (SQLVariant& p_right);
   SQLVariant OperUTinyAddUBig   (SQLVariant& p_right);
   SQLVariant OperSBigAddUBig    (SQLVariant& p_right);
   SQLVariant OperUBigAddUBig    (SQLVariant& p_right);
   SQLVariant OperNumAddUBig     (SQLVariant& p_right);
   SQLVariant OperCharAddNum     (SQLVariant& p_right);  // TYPE == NUMERIC
   SQLVariant OperSShortAddNum   (SQLVariant& p_right);
   SQLVariant OperUShortAddNum   (SQLVariant& p_right);
   SQLVariant OperSLongAddNum    (SQLVariant& p_right);
   SQLVariant OperULongAddNum    (SQLVariant& p_right);
   SQLVariant OperFloatAddNum    (SQLVariant& p_right);
   SQLVariant OperDoubleAddNum   (SQLVariant& p_right);
   SQLVariant OperBitAddNum      (SQLVariant& p_right);
   SQLVariant OperSTinyAddNum    (SQLVariant& p_right);
   SQLVariant OperUTinyAddNum    (SQLVariant& p_right);
   SQLVariant OperSBigAddNum     (SQLVariant& p_right);
   SQLVariant OperUBigAddNum     (SQLVariant& p_right);
   SQLVariant OperNumAddNum      (SQLVariant& p_right);
   SQLVariant OperTimeAddDate    (SQLVariant& p_right);  // TYPE == DATE
   SQLVariant OperIntDSAddDate   (SQLVariant& p_right); 
   SQLVariant OperDateAddTime    (SQLVariant& p_right);  // TYPE == TIME
   SQLVariant OperIntDSAddTime   (SQLVariant& p_right);
   SQLVariant OperIntDSAddStamp  (SQLVariant& p_right);  // TYPE == TIMESTAMP
   SQLVariant OperIntYMAddIntYM  (SQLVariant& p_right);  // TYPE == INTERVAL_YEAR_MONTH
   SQLVariant OperDateAddIntDS   (SQLVariant& p_right);  // TYPE == INTERVAL_DAY_SECOND
   SQLVariant OperTimeAddIntDS   (SQLVariant& p_right);
   SQLVariant OperStampAddIntDS  (SQLVariant& p_right);
   SQLVariant OperIntDSAddIntDS  (SQLVariant& p_right);




