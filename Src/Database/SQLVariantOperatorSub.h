////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantOperatorSub.h
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
// This is the part of the class that takes care of the - operator

   // Arrays with comparison functions for the operators
   static CalculateFunctionArray OperatorSub[CT_LAST][CT_LAST];

   // Calculate functions for the Sub operator
   SQLVariant OperSShortSubChar  (SQLVariant& p_right); // TYPE == CHAR
   SQLVariant OperUShortSubChar  (SQLVariant& p_right);
   SQLVariant OperSLongSubChar   (SQLVariant& p_right);
   SQLVariant OperULongSubChar   (SQLVariant& p_right);
   SQLVariant OperFloatSubChar   (SQLVariant& p_right);
   SQLVariant OperDoubleSubChar  (SQLVariant& p_right);
   SQLVariant OperBitSubChar     (SQLVariant& p_right);
   SQLVariant OperSTinySubChar   (SQLVariant& p_right);
   SQLVariant OperUTinySubChar   (SQLVariant& p_right);
   SQLVariant OperSBigSubChar    (SQLVariant& p_right);
   SQLVariant OperUBigSubChar    (SQLVariant& p_right);
   SQLVariant OperNumSubChar     (SQLVariant& p_right);
   SQLVariant OperCharSubSShort  (SQLVariant& p_right);  // TYPE == SSHORT
   SQLVariant OperSShortSubSShort(SQLVariant& p_right);
   SQLVariant OperUShortSubSShort(SQLVariant& p_right);
   SQLVariant OperSLongSubSShort (SQLVariant& p_right);
   SQLVariant OperULongSubSShort (SQLVariant& p_right);
   SQLVariant OperFloatSubSShort (SQLVariant& p_right);
   SQLVariant OperDoubleSubSShort(SQLVariant& p_right);
   SQLVariant OperBitSubSShort   (SQLVariant& p_right);
   SQLVariant OperSTinySubSShort (SQLVariant& p_right);
   SQLVariant OperUTinySubSShort (SQLVariant& p_right);
   SQLVariant OperSBigSubSShort  (SQLVariant& p_right);
   SQLVariant OperUBigSubSShort  (SQLVariant& p_right);
   SQLVariant OperNumSubSShort   (SQLVariant& p_right);
   SQLVariant OperCharSubUShort  (SQLVariant& p_right);  // TYPE == USHORT
   SQLVariant OperSShortSubUShort(SQLVariant& p_right);
   SQLVariant OperUShortSubUShort(SQLVariant& p_right);
   SQLVariant OperSLongSubUShort (SQLVariant& p_right);
   SQLVariant OperUlongSubUShort (SQLVariant& p_right);
   SQLVariant OperFloatSubUShort (SQLVariant& p_right);
   SQLVariant OperDoubleSubUShort(SQLVariant& p_right);
   SQLVariant OperBitSubUShort   (SQLVariant& p_right);
   SQLVariant OperSTinySubUShort (SQLVariant& p_right);
   SQLVariant OperUTinySubUShort (SQLVariant& p_right);
   SQLVariant OperSBigSubUShort  (SQLVariant& p_right);
   SQLVariant OperUBigSubUShort  (SQLVariant& p_right);
   SQLVariant OperNumSubUShort   (SQLVariant& p_right);
   SQLVariant OperCharSubSLong   (SQLVariant& p_right);  // TYPE == SLONG
   SQLVariant OperSShortSubSLong (SQLVariant& p_right);
   SQLVariant OperUShortSubSLong (SQLVariant& p_right);
   SQLVariant OperSLongSubSLong  (SQLVariant& p_right);
   SQLVariant OperULongSubSLong  (SQLVariant& p_right);
   SQLVariant OperFloatSubSLong  (SQLVariant& p_right);
   SQLVariant OperDoubleSubSLong (SQLVariant& p_right);
   SQLVariant OperBitSubSLong    (SQLVariant& p_right);
   SQLVariant OperSTinySubSLong  (SQLVariant& p_right);
   SQLVariant OperUTinySubSLong  (SQLVariant& p_right);
   SQLVariant OperSBigSubSLong   (SQLVariant& p_right);
   SQLVariant OperUBigSubSLong   (SQLVariant& p_right);
   SQLVariant OperNumSubSLong    (SQLVariant& p_right);
   SQLVariant OperCharSubULong   (SQLVariant& p_right);  // TYPE == ULONG
   SQLVariant OperSShortSubULong (SQLVariant& p_right);
   SQLVariant OperUShortSubULong (SQLVariant& p_right);
   SQLVariant OperSLongSubULong  (SQLVariant& p_right);
   SQLVariant OperULongSubULong  (SQLVariant& p_right);
   SQLVariant OperFloatSubULong  (SQLVariant& p_right);
   SQLVariant OperDoubleSubULong (SQLVariant& p_right);
   SQLVariant OperBitSubULong    (SQLVariant& p_right);
   SQLVariant OperSTinySubULong  (SQLVariant& p_right);
   SQLVariant OperUTinySubULong  (SQLVariant& p_right);
   SQLVariant OperSBigSubULong   (SQLVariant& p_right);
   SQLVariant OperUBigSubULong   (SQLVariant& p_right);
   SQLVariant OperNumSubULong    (SQLVariant& p_right);
   SQLVariant OperCharSubFloat   (SQLVariant& p_right);  // TYPE == FLOAT
   SQLVariant OperSShortSubFloat (SQLVariant& p_right);
   SQLVariant OperUShortSubFloat (SQLVariant& p_right);
   SQLVariant OperSLongSubFloat  (SQLVariant& p_right);
   SQLVariant OperULongSubFloat  (SQLVariant& p_right);
   SQLVariant OperFloatSubFloat  (SQLVariant& p_right);
   SQLVariant OperDoubleSubFloat (SQLVariant& p_right);
   SQLVariant OperBitSubFloat    (SQLVariant& p_right);
   SQLVariant OperSTinySubFloat  (SQLVariant& p_right);
   SQLVariant OperUTinySubFloat  (SQLVariant& p_right);
   SQLVariant OperSBigSubFloat   (SQLVariant& p_right);
   SQLVariant OperUBigSubFloat   (SQLVariant& p_right);
   SQLVariant OperNumSubFloat    (SQLVariant& p_right);
   SQLVariant OperCharSubDouble  (SQLVariant& p_right);  // TYPE == DOUBLE
   SQLVariant OperSShortSubDouble(SQLVariant& p_right);
   SQLVariant OperUShortSubDouble(SQLVariant& p_right);
   SQLVariant OperSLongSubDouble (SQLVariant& p_right);
   SQLVariant OperULongSubDouble (SQLVariant& p_right);
   SQLVariant OperFloatSubDouble (SQLVariant& p_right);
   SQLVariant OperDoubleSubDouble(SQLVariant& p_right);
   SQLVariant OperBitSubDouble   (SQLVariant& p_right);
   SQLVariant OperSTinySubDouble (SQLVariant& p_right);
   SQLVariant OperUTinySubDouble (SQLVariant& p_right);
   SQLVariant OperSBigSubDouble  (SQLVariant& p_right);
   SQLVariant OperUBigSubDouble  (SQLVariant& p_right);
   SQLVariant OperNumSubDouble   (SQLVariant& p_right);
   SQLVariant OperCharSubBit     (SQLVariant& p_right);  // TYPE == BIT
   SQLVariant OperSShortSubBit   (SQLVariant& p_right);
   SQLVariant OperUShortSubBit   (SQLVariant& p_right);
   SQLVariant OperSLongSubBit    (SQLVariant& p_right);
   SQLVariant OperULongSubBit    (SQLVariant& p_right);
   SQLVariant OperFloatSubBit    (SQLVariant& p_right);
   SQLVariant OperDoubleSubBit   (SQLVariant& p_right);
   SQLVariant OperBitSubBit      (SQLVariant& p_right);
   SQLVariant OperSTinySubBit    (SQLVariant& p_right);
   SQLVariant OperUTinySubBit    (SQLVariant& p_right);
   SQLVariant OperSBigSubBit     (SQLVariant& p_right);
   SQLVariant OperUBigSubBit     (SQLVariant& p_right);
   SQLVariant OperNumSubBit      (SQLVariant& p_right);
   SQLVariant OperCharSubSTiny   (SQLVariant& p_right);  // TYPE == STINYINT
   SQLVariant OperSShortSubSTiny (SQLVariant& p_right);
   SQLVariant OperUShortSubSTiny (SQLVariant& p_right);
   SQLVariant OperSLongSubSTiny  (SQLVariant& p_right);
   SQLVariant OperULongSubSTiny  (SQLVariant& p_right);
   SQLVariant OperFloatSubSTiny  (SQLVariant& p_right);
   SQLVariant OperDoubleSubSTiny (SQLVariant& p_right);
   SQLVariant OperBitSubSTiny    (SQLVariant& p_right);
   SQLVariant OperSTinySubSTiny  (SQLVariant& p_right);
   SQLVariant OperUTinySubSTiny  (SQLVariant& p_right);
   SQLVariant OperSBigSubSTiny   (SQLVariant& p_right);
   SQLVariant OperUBigSubSTiny   (SQLVariant& p_right);
   SQLVariant OperNumSubSTiny    (SQLVariant& p_right);
   SQLVariant OperCharSubUTiny   (SQLVariant& p_right);  // TYPE == UTINYINT
   SQLVariant OperSShortSubUTiny (SQLVariant& p_right);
   SQLVariant OperUShortSubUTiny (SQLVariant& p_right);
   SQLVariant OperSLongSubUTiny  (SQLVariant& p_right);
   SQLVariant OperULongSubUTiny  (SQLVariant& p_right);
   SQLVariant OperFloatSubUTiny  (SQLVariant& p_right);
   SQLVariant OperDoubleSubUTiny (SQLVariant& p_right);
   SQLVariant OperBitSubUTiny    (SQLVariant& p_right);
   SQLVariant OperSTinySubUTiny  (SQLVariant& p_right);
   SQLVariant OperUTinySubUTiny  (SQLVariant& p_right);
   SQLVariant OperSBigSubUTiny   (SQLVariant& p_right);
   SQLVariant OperUBigSubUTiny   (SQLVariant& p_right);
   SQLVariant OperNumSubUTiny    (SQLVariant& p_right);
   SQLVariant OperCharSubSBig    (SQLVariant& p_right);  // TYPE == SBIGINT
   SQLVariant OperSShortSubSBig  (SQLVariant& p_right);
   SQLVariant OperUShortSubSBig  (SQLVariant& p_right);
   SQLVariant OperSLongSubSBig   (SQLVariant& p_right);
   SQLVariant OperULongSubSBig   (SQLVariant& p_right);
   SQLVariant OperFloatSubSBig   (SQLVariant& p_right);
   SQLVariant OperDoubleSubSBig  (SQLVariant& p_right);
   SQLVariant OperBitSubSBig     (SQLVariant& p_right);
   SQLVariant OperSTinySubSBig   (SQLVariant& p_right);
   SQLVariant OperUTinySubSBig   (SQLVariant& p_right);
   SQLVariant OperSBigSubSBig    (SQLVariant& p_right);
   SQLVariant OperUBigSubSBig    (SQLVariant& p_right);
   SQLVariant OperNumSubSBig     (SQLVariant& p_right);
   SQLVariant OperCharSubUBig    (SQLVariant& p_right);  // TYPE == UBIGINT
   SQLVariant OperSShortSubUBig  (SQLVariant& p_right);
   SQLVariant OperUShortSubUBig  (SQLVariant& p_right);
   SQLVariant OperSLongSubUBig   (SQLVariant& p_right);
   SQLVariant OperULongSubUBig   (SQLVariant& p_right);
   SQLVariant OperFloatSubUBig   (SQLVariant& p_right);
   SQLVariant OperDoubleSubUBig  (SQLVariant& p_right);
   SQLVariant OperBitSubUBig     (SQLVariant& p_right);
   SQLVariant OperSTinySubUBig   (SQLVariant& p_right);
   SQLVariant OperUTinySubUBig   (SQLVariant& p_right);
   SQLVariant OperSBigSubUBig    (SQLVariant& p_right);
   SQLVariant OperUBigSubUBig    (SQLVariant& p_right);
   SQLVariant OperNumSubUBig     (SQLVariant& p_right);
   SQLVariant OperCharSubNum     (SQLVariant& p_right);  // TYPE == NUMERIC
   SQLVariant OperSShortSubNum   (SQLVariant& p_right);
   SQLVariant OperUShortSubNum   (SQLVariant& p_right);
   SQLVariant OperSLongSubNum    (SQLVariant& p_right);
   SQLVariant OperULongSubNum    (SQLVariant& p_right);
   SQLVariant OperFloatSubNum    (SQLVariant& p_right);
   SQLVariant OperDoubleSubNum   (SQLVariant& p_right);
   SQLVariant OperBitSubNum      (SQLVariant& p_right);
   SQLVariant OperSTinySubNum    (SQLVariant& p_right);
   SQLVariant OperUTinySubNum    (SQLVariant& p_right);
   SQLVariant OperSBigSubNum     (SQLVariant& p_right);
   SQLVariant OperUBigSubNum     (SQLVariant& p_right);
   SQLVariant OperNumSubNum      (SQLVariant& p_right);
   SQLVariant OperDateSubDate    (SQLVariant& p_right);  // TYPE == DATE
   SQLVariant OperTimeSubTime    (SQLVariant& p_right);  // TYPE == TIME
   SQLVariant OperStampSubStamp  (SQLVariant& p_right);  // TYPE == TIMESTAMP
   SQLVariant OperIntYMSubIntYM  (SQLVariant& p_right);  // TYPE == INTERVAL_YEAR_MONTH
   SQLVariant OperDateSubIntDS   (SQLVariant& p_right);  // TYPE == INTERVAL_DAY_SECOND
   SQLVariant OperTimeSubIntDS   (SQLVariant& p_right);
   SQLVariant OperStampSubIntDS  (SQLVariant& p_right);
   SQLVariant OperIntDSSubIntDS  (SQLVariant& p_right);
