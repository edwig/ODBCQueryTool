
////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantOperatorSmallEqual.h
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
static CompareFunctionArray OperatorSmallerEqual[CT_LAST][CT_LAST];

// Comparison functions for the EQUAL operator
bool   OperVarSmallEQChar     (SQLVariant& p_right);  // TYPE == CHAR
bool   OperGuidSmallEQChar    (SQLVariant& p_right);
bool   OperSShortSmallEQSShort(SQLVariant& p_right);  // TYPE == SSHORT
bool   OperUShortSmallEQSShort(SQLVariant& p_right);
bool   OperSLongSmallEQSShort (SQLVariant& p_right);
bool   OperULongSmallEQSShort (SQLVariant& p_right);
bool   OperFloatSmallEQSShort (SQLVariant& p_right);
bool   OperDoubleSmallEQSShort(SQLVariant& p_right);
bool   OperBitSmallEQSShort   (SQLVariant& p_right);
bool   OperSTinySmallEQSShort (SQLVariant& p_right);
bool   OperUTinySmallEQSShort (SQLVariant& p_right);
bool   OperSBigSmallEQSShort  (SQLVariant& p_right);
bool   OperUBigSmallEQSShort  (SQLVariant& p_right);
bool   OperNumSmallEQSShort   (SQLVariant& p_right);
bool   OperSShortSmallEQUShort(SQLVariant& p_right);  // TYPE == USHORT
bool   OperUShortSmallEQUShort(SQLVariant& p_right);
bool   OperSLongSmallEQUShort (SQLVariant& p_right);
bool   OperUlongSmallEQUShort (SQLVariant& p_right);
bool   OperFloatSmallEQUShort (SQLVariant& p_right);
bool   OperDoubleSmallEQUShort(SQLVariant& p_right);
bool   OperBitSmallEQUShort   (SQLVariant& p_right);
bool   OperSTinySmallEQUShort (SQLVariant& p_right);
bool   OperUTinySmallEQUShort (SQLVariant& p_right);
bool   OperSBigSmallEQUShort  (SQLVariant& p_right);
bool   OperUBigSmallEQUShort  (SQLVariant& p_right);
bool   OperNumSmallEQUShort   (SQLVariant& p_right);
bool   OperSShortSmallEQSLong (SQLVariant& p_right);  // TYPE == SLONG
bool   OperUShortSmallEQSLong (SQLVariant& p_right);
bool   OperSLongSmallEQSLong  (SQLVariant& p_right);
bool   OperULongSmallEQSLong  (SQLVariant& p_right);
bool   OperFloatSmallEQSLong  (SQLVariant& p_right);
bool   OperDoubleESmallEQSLong(SQLVariant& p_right);
bool   OperBitSmallEQSLong    (SQLVariant& p_right);
bool   OperSTinySmallEQSLong  (SQLVariant& p_right);
bool   OperUTinySmallEQSLong  (SQLVariant& p_right);
bool   OperSBigSmallEQSLong   (SQLVariant& p_right);
bool   OperUBigSmallEQSLong   (SQLVariant& p_right);
bool   OperNumSmallEQSLong    (SQLVariant& p_right);
bool   OperSShortSmallEQULong (SQLVariant& p_right);  // TYPE == ULONG
bool   OperUShortSmallEQULong (SQLVariant& p_right);
bool   OperSLongSmallEQULong  (SQLVariant& p_right);
bool   OperULongSmallEQULong  (SQLVariant& p_right);
bool   OperFloatSmallEQULong  (SQLVariant& p_right);
bool   OperDoubleSmallEQULong (SQLVariant& p_right);
bool   OperBitSmallEQULong    (SQLVariant& p_right);
bool   OperSTinySmallEQULong  (SQLVariant& p_right);
bool   OperUTinySmallEQULong  (SQLVariant& p_right);
bool   OperSBigSmallEQULong   (SQLVariant& p_right);
bool   OperUBigSmallEQULong   (SQLVariant& p_right);
bool   OperNumSmallEQULong    (SQLVariant& p_right);
bool   OperSShortSmallEQFloat (SQLVariant& p_right);  // TYPE == FLOAT
bool   OperUShortSmallEQFloat (SQLVariant& p_right);
bool   OperSLongSmallEQFloat  (SQLVariant& p_right);
bool   OperULongSmallEQFloat  (SQLVariant& p_right);
bool   OperFloatSmallEQFloat  (SQLVariant& p_right);
bool   OperDoubleSmallEQFloat (SQLVariant& p_right);
bool   OperBitSmallEQFloat    (SQLVariant& p_right);
bool   OperSTinySmallEQFloat  (SQLVariant& p_right);
bool   OperUTinySmallEQFloat  (SQLVariant& p_right);
bool   OperSBigSmallEQFloat   (SQLVariant& p_right);
bool   OperUBigSmallEQFloat   (SQLVariant& p_right);
bool   OperNumSmallEQFloat    (SQLVariant& p_right);
bool   OperSShortSmallEQDouble(SQLVariant& p_right);  // TYPE == DOUBLE
bool   OperUShortSmallEQDouble(SQLVariant& p_right);
bool   OperSLongSmallEQDouble (SQLVariant& p_right);
bool   OperULongSmallEQDouble (SQLVariant& p_right);
bool   OperFloatSmallEQDouble (SQLVariant& p_right);
bool   OperDoubleSmallEQDouble(SQLVariant& p_right);
bool   OperBitSmallEQDouble   (SQLVariant& p_right);
bool   OperSTinySmallEQDouble (SQLVariant& p_right);
bool   OperUTinySmallEQDouble (SQLVariant& p_right);
bool   OperSBigSmallEQDouble  (SQLVariant& p_right);
bool   OperUBigSmallEQDouble  (SQLVariant& p_right);
bool   OperNumSmallEQDouble   (SQLVariant& p_right);
bool   OperSShortSmallEQBit   (SQLVariant& p_right);  // TYPE == BIT
bool   OperUShortSmallEQBit   (SQLVariant& p_right);
bool   OperSLongSmallEQBit    (SQLVariant& p_right);
bool   OperULongSmallEQBit    (SQLVariant& p_right);
bool   OperFloatSmallEQBit    (SQLVariant& p_right);
bool   OperDoubleSmallEQBit   (SQLVariant& p_right);
bool   OperBitSmallEQBit      (SQLVariant& p_right);
bool   OperSTinySmallEQBit    (SQLVariant& p_right);
bool   OperUTinySmallEQBit    (SQLVariant& p_right);
bool   OperSBigSmallEQBit     (SQLVariant& p_right);
bool   OperUBigSmallEQBit     (SQLVariant& p_right);
bool   OperNumSmallEQBit      (SQLVariant& p_right);
bool   OperSShortSmallEQSTiny (SQLVariant& p_right);  // TYPE == STINYINT
bool   OperUShortSmallEQSTiny (SQLVariant& p_right);
bool   OperSLongSmallEQSTiny  (SQLVariant& p_right);
bool   OperULongSmallEQSTiny  (SQLVariant& p_right);
bool   OperFloatSmallEQSTiny  (SQLVariant& p_right);
bool   OperDoubleSmallEQSTiny (SQLVariant& p_right);
bool   OperBitSmallEQSTiny    (SQLVariant& p_right);
bool   OperSTinySmallEQSTiny  (SQLVariant& p_right);
bool   OperUTinySmallEQSTiny  (SQLVariant& p_right);
bool   OperSBigSmallEQSTiny   (SQLVariant& p_right);
bool   OperUBigSmallEQSTiny   (SQLVariant& p_right);
bool   OperNumSmallEQSTiny    (SQLVariant& p_right);
bool   OperSShortSmallEQUTiny (SQLVariant& p_right);  // TYPE == UTINYINT
bool   OperUShortSmallEQUTiny (SQLVariant& p_right);
bool   OperSLongSmallEQUTiny  (SQLVariant& p_right);
bool   OperULongSmallEQUTiny  (SQLVariant& p_right);
bool   OperFloatSmallEQUTiny  (SQLVariant& p_right);
bool   OperDoubleSmallEQUTiny (SQLVariant& p_right);
bool   OperBitSmallEQUTiny    (SQLVariant& p_right);
bool   OperSTinySmallEQUTiny  (SQLVariant& p_right);
bool   OperUTinySmallEQUTiny  (SQLVariant& p_right);
bool   OperSBigSmallEQUTiny   (SQLVariant& p_right);
bool   OperUBigSmallEQUTiny   (SQLVariant& p_right);
bool   OperNumSmallEQUTiny    (SQLVariant& p_right);
bool   OperSShortSmallEQSBig  (SQLVariant& p_right);  // TYPE == SBIGINT
bool   OperUShortSmallEQSBig  (SQLVariant& p_right);
bool   OperSLongSmallEQSBig   (SQLVariant& p_right);
bool   OperULongSmallEQSBig   (SQLVariant& p_right);
bool   OperFloatSmallEQSBig   (SQLVariant& p_right);
bool   OperDoubleSmallEQSBig  (SQLVariant& p_right);
bool   OperBitSmallEQSBig     (SQLVariant& p_right);
bool   OperSTinySmallEQSBig   (SQLVariant& p_right);
bool   OperUTinySmallEQSBig   (SQLVariant& p_right);
bool   OperSBigSmallEQSBig    (SQLVariant& p_right);
bool   OperUBigSmallEQSBig    (SQLVariant& p_right);
bool   OperNumSmallEQSBig     (SQLVariant& p_right);
bool   OperSShortSmallEQUBig  (SQLVariant& p_right);  // TYPE == UBIGINT
bool   OperUShortSmallEQUBig  (SQLVariant& p_right);
bool   OperSLongSmallEQUBig   (SQLVariant& p_right);
bool   OperULongSmallEQUBig   (SQLVariant& p_right);
bool   OperFloatSmallEQUBig   (SQLVariant& p_right);
bool   OperDoubleSmallEQUBig  (SQLVariant& p_right);
bool   OperBitSmallEQUBig     (SQLVariant& p_right);
bool   OperSTinySmallEQUBig   (SQLVariant& p_right);
bool   OperUTinySmallEQUBig   (SQLVariant& p_right);
bool   OperSBigSmallEQUBig    (SQLVariant& p_right);
bool   OperUBigSmallEQUBig    (SQLVariant& p_right);
bool   OperNumSmallEQUBig     (SQLVariant& p_right);
bool   OperSShortSmallEQNum   (SQLVariant& p_right);  // TYPE == NUMERIC
bool   OperUShortSmallEQNum   (SQLVariant& p_right);
bool   OperSLongSmallEQNum    (SQLVariant& p_right);
bool   OperULongSmallEQNum    (SQLVariant& p_right);
bool   OperFloatSmallEQNum    (SQLVariant& p_right);
bool   OperDoubleSmallEQNum   (SQLVariant& p_right);
bool   OperBitSmallEQNum      (SQLVariant& p_right);
bool   OperSTinySmallEQNum    (SQLVariant& p_right);
bool   OperUTinySmallEQNum    (SQLVariant& p_right);
bool   OperSBigSmallEQNum     (SQLVariant& p_right);
bool   OperUBigSmallEQNum     (SQLVariant& p_right);
bool   OperNumSmallEQNum      (SQLVariant& p_right);
bool   OperGuidSmallEQGuid    (SQLVariant& p_right);  // TYPE == GUID
bool   OperBinarySmallEQBinary(SQLVariant& p_right);  // TYPE == BINARY
bool   OperDateSmallEQDate    (SQLVariant& p_right);  // TYPE == DATE
bool   OperStampSmallEQDate   (SQLVariant& p_right);
bool   OperTimeSmallEQTime    (SQLVariant& p_right);  // TYPE == TIME
bool   OperStampSmallEQTime   (SQLVariant& p_right);
bool   OperStampSmallEQStamp  (SQLVariant& p_right);  // TYPE == TIMESTAMP
bool   OperIntYMSmallEQIntYM  (SQLVariant& p_right);  // TYPE == INTERVAL_YEAR_MONTH
bool   OperIntDSSmallEQIntDS  (SQLVariant& p_right);  // TYPE == INTERVAL_DAY_SECOND


