////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantOperatorSmaller.h
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
   static CompareFunctionArray OperatorSmaller[CT_LAST][CT_LAST];

   // Comparison functions for the EQUAL operator
   bool   OperVarSmallerChar     (SQLVariant& p_right);  // TYPE == CHAR
   bool   OperGuidSmallerChar    (SQLVariant& p_right);
   bool   OperSShortSmallerSShort(SQLVariant& p_right);  // TYPE == SSHORT
   bool   OperUShortSmallerSShort(SQLVariant& p_right);
   bool   OperSLongSmallerSShort (SQLVariant& p_right);
   bool   OperULongSmallerSShort (SQLVariant& p_right);
   bool   OperFloatSmallerSShort (SQLVariant& p_right);
   bool   OperDoubleSmallerSShort(SQLVariant& p_right);
   bool   OperBitSmallerSShort   (SQLVariant& p_right);
   bool   OperSTinySmallerSShort (SQLVariant& p_right);
   bool   OperUTinySmallerSShort (SQLVariant& p_right);
   bool   OperSBigSmallerSShort  (SQLVariant& p_right);
   bool   OperUBigSmallerSShort  (SQLVariant& p_right);
   bool   OperNumSmallerSShort   (SQLVariant& p_right);
   bool   OperSShortSmallerUShort(SQLVariant& p_right);  // TYPE == USHORT
   bool   OperUShortSmallerUShort(SQLVariant& p_right);
   bool   OperSLongSmallerUShort (SQLVariant& p_right);
   bool   OperUlongSmallerUShort (SQLVariant& p_right);
   bool   OperFloatSmallerUShort (SQLVariant& p_right);
   bool   OperDoubleSmallerUShort(SQLVariant& p_right);
   bool   OperBitSmallerUShort   (SQLVariant& p_right);
   bool   OperSTinySmallerUShort (SQLVariant& p_right);
   bool   OperUTinySmallerUShort (SQLVariant& p_right);
   bool   OperSBigSmallerUShort  (SQLVariant& p_right);
   bool   OperUBigSmallerUShort  (SQLVariant& p_right);
   bool   OperNumSmallerUShort   (SQLVariant& p_right);
   bool   OperSShortSmallerSLong (SQLVariant& p_right);  // TYPE == SLONG
   bool   OperUShortSmallerSLong (SQLVariant& p_right);
   bool   OperSLongSmallerSLong  (SQLVariant& p_right);
   bool   OperULongSmallerSLong  (SQLVariant& p_right);
   bool   OperFloatSmallerSLong  (SQLVariant& p_right);
   bool   OperDoubleESmallerSLong(SQLVariant& p_right);
   bool   OperBitSmallerSLong    (SQLVariant& p_right);
   bool   OperSTinySmallerSLong  (SQLVariant& p_right);
   bool   OperUTinySmallerSLong  (SQLVariant& p_right);
   bool   OperSBigSmallerSLong   (SQLVariant& p_right);
   bool   OperUBigSmallerSLong   (SQLVariant& p_right);
   bool   OperNumSmallerSLong    (SQLVariant& p_right);
   bool   OperSShortSmallerULong (SQLVariant& p_right);  // TYPE == ULONG
   bool   OperUShortSmallerULong (SQLVariant& p_right);
   bool   OperSLongSmallerULong  (SQLVariant& p_right);
   bool   OperULongSmallerULong  (SQLVariant& p_right);
   bool   OperFloatSmallerULong  (SQLVariant& p_right);
   bool   OperDoubleSmallerULong (SQLVariant& p_right);
   bool   OperBitSmallerULong    (SQLVariant& p_right);
   bool   OperSTinySmallerULong  (SQLVariant& p_right);
   bool   OperUTinySmallerULong  (SQLVariant& p_right);
   bool   OperSBigSmallerULong   (SQLVariant& p_right);
   bool   OperUBigSmallerULong   (SQLVariant& p_right);
   bool   OperNumSmallerULong    (SQLVariant& p_right);
   bool   OperSShortSmallerFloat (SQLVariant& p_right);  // TYPE == FLOAT
   bool   OperUShortSmallerFloat (SQLVariant& p_right);
   bool   OperSLongSmallerFloat  (SQLVariant& p_right);
   bool   OperULongSmallerFloat  (SQLVariant& p_right);
   bool   OperFloatSmallerFloat  (SQLVariant& p_right);
   bool   OperDoubleSmallerFloat (SQLVariant& p_right);
   bool   OperBitSmallerFloat    (SQLVariant& p_right);
   bool   OperSTinySmallerFloat  (SQLVariant& p_right);
   bool   OperUTinySmallerFloat  (SQLVariant& p_right);
   bool   OperSBigSmallerFloat   (SQLVariant& p_right);
   bool   OperUBigSmallerFloat   (SQLVariant& p_right);
   bool   OperNumSmallerFloat    (SQLVariant& p_right);
   bool   OperSShortSmallerDouble(SQLVariant& p_right);  // TYPE == DOUBLE
   bool   OperUShortSmallerDouble(SQLVariant& p_right);
   bool   OperSLongSmallerDouble (SQLVariant& p_right);
   bool   OperULongSmallerDouble (SQLVariant& p_right);
   bool   OperFloatSmallerDouble (SQLVariant& p_right);
   bool   OperDoubleSmallerDouble(SQLVariant& p_right);
   bool   OperBitSmallerDouble   (SQLVariant& p_right);
   bool   OperSTinySmallerDouble (SQLVariant& p_right);
   bool   OperUTinySmallerDouble (SQLVariant& p_right);
   bool   OperSBigSmallerDouble  (SQLVariant& p_right);
   bool   OperUBigSmallerDouble  (SQLVariant& p_right);
   bool   OperNumSmallerDouble   (SQLVariant& p_right);
   bool   OperSShortSmallerBit   (SQLVariant& p_right);  // TYPE == BIT
   bool   OperUShortSmallerBit   (SQLVariant& p_right);
   bool   OperSLongSmallerBit    (SQLVariant& p_right);
   bool   OperULongSmallerBit    (SQLVariant& p_right);
   bool   OperFloatSmallerBit    (SQLVariant& p_right);
   bool   OperDoubleSmallerBit   (SQLVariant& p_right);
   bool   OperBitSmallerBit      (SQLVariant& p_right);
   bool   OperSTinySmallerBit    (SQLVariant& p_right);
   bool   OperUTinySmallerBit    (SQLVariant& p_right);
   bool   OperSBigSmallerBit     (SQLVariant& p_right);
   bool   OperUBigSmallerBit     (SQLVariant& p_right);
   bool   OperNumSmallerBit      (SQLVariant& p_right);
   bool   OperSShortSmallerSTiny (SQLVariant& p_right);  // TYPE == STINYINT
   bool   OperUShortSmallerSTiny (SQLVariant& p_right);
   bool   OperSLongSmallerSTiny  (SQLVariant& p_right);
   bool   OperULongSmallerSTiny  (SQLVariant& p_right);
   bool   OperFloatSmallerSTiny  (SQLVariant& p_right);
   bool   OperDoubleSmallerSTiny (SQLVariant& p_right);
   bool   OperBitSmallerSTiny    (SQLVariant& p_right);
   bool   OperSTinySmallerSTiny  (SQLVariant& p_right);
   bool   OperUTinySmallerSTiny  (SQLVariant& p_right);
   bool   OperSBigSmallerSTiny   (SQLVariant& p_right);
   bool   OperUBigSmallerSTiny   (SQLVariant& p_right);
   bool   OperNumSmallerSTiny    (SQLVariant& p_right);
   bool   OperSShortSmallerUTiny (SQLVariant& p_right);  // TYPE == UTINYINT
   bool   OperUShortSmallerUTiny (SQLVariant& p_right);
   bool   OperSLongSmallerUTiny  (SQLVariant& p_right);
   bool   OperULongSmallerUTiny  (SQLVariant& p_right);
   bool   OperFloatSmallerUTiny  (SQLVariant& p_right);
   bool   OperDoubleSmallerUTiny (SQLVariant& p_right);
   bool   OperBitSmallerUTiny    (SQLVariant& p_right);
   bool   OperSTinySmallerUTiny  (SQLVariant& p_right);
   bool   OperUTinySmallerUTiny  (SQLVariant& p_right);
   bool   OperSBigSmallerUTiny   (SQLVariant& p_right);
   bool   OperUBigSmallerUTiny   (SQLVariant& p_right);
   bool   OperNumSmallerUTiny    (SQLVariant& p_right);
   bool   OperSShortSmallerSBig  (SQLVariant& p_right);  // TYPE == SBIGINT
   bool   OperUShortSmallerSBig  (SQLVariant& p_right);
   bool   OperSLongSmallerSBig   (SQLVariant& p_right);
   bool   OperULongSmallerSBig   (SQLVariant& p_right);
   bool   OperFloatSmallerSBig   (SQLVariant& p_right);
   bool   OperDoubleSmallerSBig  (SQLVariant& p_right);
   bool   OperBitSmallerSBig     (SQLVariant& p_right);
   bool   OperSTinySmallerSBig   (SQLVariant& p_right);
   bool   OperUTinySmallerSBig   (SQLVariant& p_right);
   bool   OperSBigSmallerSBig    (SQLVariant& p_right);
   bool   OperUBigSmallerSBig    (SQLVariant& p_right);
   bool   OperNumSmallerSBig     (SQLVariant& p_right);
   bool   OperSShortSmallerUBig  (SQLVariant& p_right);  // TYPE == UBIGINT
   bool   OperUShortSmallerUBig  (SQLVariant& p_right);
   bool   OperSLongSmallerUBig   (SQLVariant& p_right);
   bool   OperULongSmallerUBig   (SQLVariant& p_right);
   bool   OperFloatSmallerUBig   (SQLVariant& p_right);
   bool   OperDoubleSmallerUBig  (SQLVariant& p_right);
   bool   OperBitSmallerUBig     (SQLVariant& p_right);
   bool   OperSTinySmallerUBig   (SQLVariant& p_right);
   bool   OperUTinySmallerUBig   (SQLVariant& p_right);
   bool   OperSBigSmallerUBig    (SQLVariant& p_right);
   bool   OperUBigSmallerUBig    (SQLVariant& p_right);
   bool   OperNumSmallerUBig     (SQLVariant& p_right);
   bool   OperSShortSmallerNum   (SQLVariant& p_right);  // TYPE == NUMERIC
   bool   OperUShortSmallerNum   (SQLVariant& p_right);
   bool   OperSLongSmallerNum    (SQLVariant& p_right);
   bool   OperULongSmallerNum    (SQLVariant& p_right);
   bool   OperFloatSmallerNum    (SQLVariant& p_right);
   bool   OperDoubleSmallerNum   (SQLVariant& p_right);
   bool   OperBitSmallerNum      (SQLVariant& p_right);
   bool   OperSTinySmallerNum    (SQLVariant& p_right);
   bool   OperUTinySmallerNum    (SQLVariant& p_right);
   bool   OperSBigSmallerNum     (SQLVariant& p_right);
   bool   OperUBigSmallerNum     (SQLVariant& p_right);
   bool   OperNumSmallerNum      (SQLVariant& p_right);
   bool   OperGuidSmallerGuid    (SQLVariant& p_right);  // TYPE == GUID
   bool   OperBinarySmallerBinary(SQLVariant& p_right);  // TYPE == BINARY
   bool   OperDateSmallerDate    (SQLVariant& p_right);  // TYPE == DATE
   bool   OperStampSmallerDate   (SQLVariant& p_right);
   bool   OperTimeSmallerTime    (SQLVariant& p_right);  // TYPE == TIME
   bool   OperStampSmallerTime   (SQLVariant& p_right);
   bool   OperStampSmallerStamp  (SQLVariant& p_right);  // TYPE == TIMESTAMP
   bool   OperIntYMSmallerIntYM  (SQLVariant& p_right);  // TYPE == INTERVAL_YEAR_MONTH
   bool   OperIntDSSmallerIntDS  (SQLVariant& p_right);  // TYPE == INTERVAL_DAY_SECOND


