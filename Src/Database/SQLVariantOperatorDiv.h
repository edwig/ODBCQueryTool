////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantOperatorDiv.h
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
// This is the part of the class that takes care of the / operator

   // Arrays with calculation functions for the operators
   static CalculateFunctionArray OperatorDiv[CT_LAST][CT_LAST];

   // Calculate functions for the Division operator
   SQLVariant OperSShortDivChar  (SQLVariant& p_right);  // TYPE == CHAR
   SQLVariant OperUShortDivChar  (SQLVariant& p_right);
   SQLVariant OperSLongDivChar   (SQLVariant& p_right);
   SQLVariant OperULongDivChar   (SQLVariant& p_right);
   SQLVariant OperFloatDivChar   (SQLVariant& p_right);
   SQLVariant OperDoubleDivChar  (SQLVariant& p_right);
   SQLVariant OperBitDivChar     (SQLVariant& p_right);
   SQLVariant OperSTinyDivChar   (SQLVariant& p_right);
   SQLVariant OperUTinyDivChar   (SQLVariant& p_right);
   SQLVariant OperSBigDivChar    (SQLVariant& p_right);
   SQLVariant OperUBigDivChar    (SQLVariant& p_right);
   SQLVariant OperNumDivChar     (SQLVariant& p_right);
   SQLVariant OperCharDivSShort  (SQLVariant& p_right);  // TYPE == SSHORT
   SQLVariant OperSShortDivSShort(SQLVariant& p_right);
   SQLVariant OperUShortDivSShort(SQLVariant& p_right);
   SQLVariant OperSLongDivSShort (SQLVariant& p_right);
   SQLVariant OperULongDivSShort (SQLVariant& p_right);
   SQLVariant OperFloatDivSShort (SQLVariant& p_right);
   SQLVariant OperDoubleDivSShort(SQLVariant& p_right);
   SQLVariant OperBitDivSShort   (SQLVariant& p_right);
   SQLVariant OperSTinyDivSShort (SQLVariant& p_right);
   SQLVariant OperUTinyDivSShort (SQLVariant& p_right);
   SQLVariant OperSBigDivSShort  (SQLVariant& p_right);
   SQLVariant OperUBigDivSShort  (SQLVariant& p_right);
   SQLVariant OperNumDivSShort   (SQLVariant& p_right);
   SQLVariant OperIntYMDivSShort (SQLVariant& p_right);
   SQLVariant OperIntDSDivSShort (SQLVariant& p_right);
   SQLVariant OperCharDivUShort  (SQLVariant& p_right);  // TYPE == USHORT
   SQLVariant OperSShortDivUShort(SQLVariant& p_right);
   SQLVariant OperUShortDivUShort(SQLVariant& p_right);
   SQLVariant OperSLongDivUShort (SQLVariant& p_right);
   SQLVariant OperUlongDivUShort (SQLVariant& p_right);
   SQLVariant OperFloatDivUShort (SQLVariant& p_right);
   SQLVariant OperDoubleDivUShort(SQLVariant& p_right);
   SQLVariant OperBitDivUShort   (SQLVariant& p_right);
   SQLVariant OperSTinyDivUShort (SQLVariant& p_right);
   SQLVariant OperUTinyDivUShort (SQLVariant& p_right);
   SQLVariant OperSBigDivUShort  (SQLVariant& p_right);
   SQLVariant OperUBigDivUShort  (SQLVariant& p_right);
   SQLVariant OperNumDivUShort   (SQLVariant& p_right);
   SQLVariant OperIntYMDivUShort (SQLVariant& p_right);
   SQLVariant OperIntDSDivUShort (SQLVariant& p_right);
   SQLVariant OperCharDivSLong   (SQLVariant& p_right);  // TYPE == SLONG
   SQLVariant OperSShortDivSLong (SQLVariant& p_right);
   SQLVariant OperUShortDivSLong (SQLVariant& p_right);
   SQLVariant OperSLongDivSLong  (SQLVariant& p_right);
   SQLVariant OperULongDivSLong  (SQLVariant& p_right);
   SQLVariant OperFloatDivSLong  (SQLVariant& p_right);
   SQLVariant OperDoubleDivSLong (SQLVariant& p_right);
   SQLVariant OperBitDivSLong    (SQLVariant& p_right);
   SQLVariant OperSTinyDivSLong  (SQLVariant& p_right);
   SQLVariant OperUTinyDivSLong  (SQLVariant& p_right);
   SQLVariant OperSBigDivSLong   (SQLVariant& p_right);
   SQLVariant OperUBigDivSLong   (SQLVariant& p_right);
   SQLVariant OperNumDivSLong    (SQLVariant& p_right);
   SQLVariant OperIntYMDivSLong  (SQLVariant& p_right);
   SQLVariant OperIntDSDivSLong  (SQLVariant& p_right);
   SQLVariant OperCharDivULong   (SQLVariant& p_right);  // TYPE == ULONG
   SQLVariant OperSShortDivULong (SQLVariant& p_right);
   SQLVariant OperUShortDivULong (SQLVariant& p_right);
   SQLVariant OperSLongDivULong  (SQLVariant& p_right);
   SQLVariant OperULongDivULong  (SQLVariant& p_right);
   SQLVariant OperFloatDivULong  (SQLVariant& p_right);
   SQLVariant OperDoubleDivULong (SQLVariant& p_right);
   SQLVariant OperBitDivULong    (SQLVariant& p_right);
   SQLVariant OperSTinyDivULong  (SQLVariant& p_right);
   SQLVariant OperUTinyDivULong  (SQLVariant& p_right);
   SQLVariant OperSBigDivULong   (SQLVariant& p_right);
   SQLVariant OperUBigDivULong   (SQLVariant& p_right);
   SQLVariant OperNumDivULong    (SQLVariant& p_right);
   SQLVariant OperIntYMDivULong  (SQLVariant& p_right);
   SQLVariant OperIntDSDivULong  (SQLVariant& p_right);
   SQLVariant OperCharDivFloat   (SQLVariant& p_right);  // TYPE == FLOAT
   SQLVariant OperSShortDivFloat (SQLVariant& p_right);
   SQLVariant OperUShortDivFloat (SQLVariant& p_right);
   SQLVariant OperSLongDivFloat  (SQLVariant& p_right);
   SQLVariant OperULongDivFloat  (SQLVariant& p_right);
   SQLVariant OperFloatDivFloat  (SQLVariant& p_right);
   SQLVariant OperDoubleDivFloat (SQLVariant& p_right);
   SQLVariant OperBitDivFloat    (SQLVariant& p_right);
   SQLVariant OperSTinyDivFloat  (SQLVariant& p_right);
   SQLVariant OperUTinyDivFloat  (SQLVariant& p_right);
   SQLVariant OperSBigDivFloat   (SQLVariant& p_right);
   SQLVariant OperUBigDivFloat   (SQLVariant& p_right);
   SQLVariant OperNumDivFloat    (SQLVariant& p_right);
   SQLVariant OperIntYMDivFloat  (SQLVariant& p_right);
   SQLVariant OperIntDSDivFloat  (SQLVariant& p_right);
   SQLVariant OperCharDivDouble  (SQLVariant& p_right);  // TYPE == DOUBLE
   SQLVariant OperSShortDivDouble(SQLVariant& p_right);
   SQLVariant OperUShortDivDouble(SQLVariant& p_right);
   SQLVariant OperSLongDivDouble (SQLVariant& p_right);
   SQLVariant OperULongDivDouble (SQLVariant& p_right);
   SQLVariant OperFloatDivDouble (SQLVariant& p_right);
   SQLVariant OperDoubleDivDouble(SQLVariant& p_right);
   SQLVariant OperBitDivDouble   (SQLVariant& p_right);
   SQLVariant OperSTinyDivDouble (SQLVariant& p_right);
   SQLVariant OperUTinyDivDouble (SQLVariant& p_right);
   SQLVariant OperSBigDivDouble  (SQLVariant& p_right);
   SQLVariant OperUBigDivDouble  (SQLVariant& p_right);
   SQLVariant OperNumDivDouble   (SQLVariant& p_right);
   SQLVariant OperIntYMDivDouble (SQLVariant& p_right);
   SQLVariant OperIntDSDivDouble (SQLVariant& p_right);
   SQLVariant OperCharDivBit     (SQLVariant& p_right);  // TYPE == BIT
   SQLVariant OperSShortDivBit   (SQLVariant& p_right);
   SQLVariant OperUShortDivBit   (SQLVariant& p_right);
   SQLVariant OperSLongDivBit    (SQLVariant& p_right);
   SQLVariant OperULongDivBit    (SQLVariant& p_right);
   SQLVariant OperFloatDivBit    (SQLVariant& p_right);
   SQLVariant OperDoubleDivBit   (SQLVariant& p_right);
   SQLVariant OperBitDivBit      (SQLVariant& p_right);
   SQLVariant OperSTinyDivBit    (SQLVariant& p_right);
   SQLVariant OperUTinyDivBit    (SQLVariant& p_right);
   SQLVariant OperSBigDivBit     (SQLVariant& p_right);
   SQLVariant OperUBigDivBit     (SQLVariant& p_right);
   SQLVariant OperNumDivBit      (SQLVariant& p_right);
   SQLVariant OperIntYMDivBit    (SQLVariant& p_right);
   SQLVariant OperIntDSDivBit    (SQLVariant& p_right);
   SQLVariant OperCharDivSTiny   (SQLVariant& p_right);  // TYPE == STINYINT
   SQLVariant OperSShortDivSTiny (SQLVariant& p_right);
   SQLVariant OperUShortDivSTiny (SQLVariant& p_right);
   SQLVariant OperSLongDivSTiny  (SQLVariant& p_right);
   SQLVariant OperULongDivSTiny  (SQLVariant& p_right);
   SQLVariant OperFloatDivSTiny  (SQLVariant& p_right);
   SQLVariant OperDoubleDivSTiny (SQLVariant& p_right);
   SQLVariant OperBitDivSTiny    (SQLVariant& p_right);
   SQLVariant OperSTinyDivSTiny  (SQLVariant& p_right);
   SQLVariant OperUTinyDivSTiny  (SQLVariant& p_right);
   SQLVariant OperSBigDivSTiny   (SQLVariant& p_right);
   SQLVariant OperUBigDivSTiny   (SQLVariant& p_right);
   SQLVariant OperNumDivSTiny    (SQLVariant& p_right);
   SQLVariant OperIntYMDivSTiny  (SQLVariant& p_right);
   SQLVariant OperIntDSDivSTiny  (SQLVariant& p_right);
   SQLVariant OperCharDivUTiny   (SQLVariant& p_right);  // TYPE == UTINYINT
   SQLVariant OperSShortDivUTiny (SQLVariant& p_right);
   SQLVariant OperUShortDivUTiny (SQLVariant& p_right);
   SQLVariant OperSLongDivUTiny  (SQLVariant& p_right);
   SQLVariant OperULongDivUTiny  (SQLVariant& p_right);
   SQLVariant OperFloatDivUTiny  (SQLVariant& p_right);
   SQLVariant OperDoubleDivUTiny (SQLVariant& p_right);
   SQLVariant OperBitDivUTiny    (SQLVariant& p_right);
   SQLVariant OperSTinyDivUTiny  (SQLVariant& p_right);
   SQLVariant OperUTinyDivUTiny  (SQLVariant& p_right);
   SQLVariant OperSBigDivUTiny   (SQLVariant& p_right);
   SQLVariant OperUBigDivUTiny   (SQLVariant& p_right);
   SQLVariant OperNumDivUTiny    (SQLVariant& p_right);
   SQLVariant OperIntYMDivUTiny  (SQLVariant& p_right);
   SQLVariant OperIntDSDivUTiny  (SQLVariant& p_right);
   SQLVariant OperCharDivSBig    (SQLVariant& p_right);  // TYPE == SBIGINT
   SQLVariant OperSShortDivSBig  (SQLVariant& p_right);
   SQLVariant OperUShortDivSBig  (SQLVariant& p_right);
   SQLVariant OperSLongDivSBig   (SQLVariant& p_right);
   SQLVariant OperULongDivSBig   (SQLVariant& p_right);
   SQLVariant OperFloatDivSBig   (SQLVariant& p_right);
   SQLVariant OperDoubleDivSBig  (SQLVariant& p_right);
   SQLVariant OperBitDivSBig     (SQLVariant& p_right);
   SQLVariant OperSTinyDivSBig   (SQLVariant& p_right);
   SQLVariant OperUTinyDivSBig   (SQLVariant& p_right);
   SQLVariant OperSBigDivSBig    (SQLVariant& p_right);
   SQLVariant OperUBigDivSBig    (SQLVariant& p_right);
   SQLVariant OperNumDivSBig     (SQLVariant& p_right);
   SQLVariant OperIntYMDivSBig   (SQLVariant& p_right);
   SQLVariant OperIntDSDivSBig   (SQLVariant& p_right);
   SQLVariant OperCharDivUBig    (SQLVariant& p_right);  // TYPE == UBIGINT
   SQLVariant OperSShortDivUBig  (SQLVariant& p_right);
   SQLVariant OperUShortDivUBig  (SQLVariant& p_right);
   SQLVariant OperSLongDivUBig   (SQLVariant& p_right);
   SQLVariant OperULongDivUBig   (SQLVariant& p_right);
   SQLVariant OperFloatDivUBig   (SQLVariant& p_right);
   SQLVariant OperDoubleDivUBig  (SQLVariant& p_right);
   SQLVariant OperBitDivUBig     (SQLVariant& p_right);
   SQLVariant OperSTinyDivUBig   (SQLVariant& p_right);
   SQLVariant OperUTinyDivUBig   (SQLVariant& p_right);
   SQLVariant OperSBigDivUBig    (SQLVariant& p_right);
   SQLVariant OperUBigDivUBig    (SQLVariant& p_right);
   SQLVariant OperNumDivUBig     (SQLVariant& p_right);
   SQLVariant OperIntYMDivUBig   (SQLVariant& p_right);
   SQLVariant OperIntDSDivUBig   (SQLVariant& p_right);
   SQLVariant OperCharDivNum     (SQLVariant& p_right);  // TYPE == NUMERIC
   SQLVariant OperSShortDivNum   (SQLVariant& p_right);
   SQLVariant OperUShortDivNum   (SQLVariant& p_right);
   SQLVariant OperSLongDivNum    (SQLVariant& p_right);
   SQLVariant OperULongDivNum    (SQLVariant& p_right);
   SQLVariant OperFloatDivNum    (SQLVariant& p_right);
   SQLVariant OperDoubleDivNum   (SQLVariant& p_right);
   SQLVariant OperBitDivNum      (SQLVariant& p_right);
   SQLVariant OperSTinyDivNum    (SQLVariant& p_right);
   SQLVariant OperUTinyDivNum    (SQLVariant& p_right);
   SQLVariant OperSBigDivNum     (SQLVariant& p_right);
   SQLVariant OperUBigDivNum     (SQLVariant& p_right);
   SQLVariant OperNumDivNum      (SQLVariant& p_right);
   SQLVariant OperIntYMDivNum    (SQLVariant& p_right);  
   SQLVariant OperIntDSDivNum    (SQLVariant& p_right);
