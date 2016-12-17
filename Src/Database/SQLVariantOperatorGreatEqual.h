////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantOperatorGreatEqual.h
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
static CompareFunctionArray OperatorGreaterEqual[CT_LAST][CT_LAST];

// Comparison functions for the EQUAL operator
bool   OperVarGreatEQChar     (SQLVariant& p_right);  // TYPE == CHAR
bool   OperGuidGreatEQChar    (SQLVariant& p_right);
bool   OperSShortGreatEQSShort(SQLVariant& p_right);  // TYPE == SSHORT
bool   OperUShortGreatEQSShort(SQLVariant& p_right);
bool   OperSLongGreatEQSShort (SQLVariant& p_right);
bool   OperULongGreatEQSShort (SQLVariant& p_right);
bool   OperFloatGreatEQSShort (SQLVariant& p_right);
bool   OperDoubleGreatEQSShort(SQLVariant& p_right);
bool   OperBitGreatEQSShort   (SQLVariant& p_right);
bool   OperSTinyGreatEQSShort (SQLVariant& p_right);
bool   OperUTinyGreatEQSShort (SQLVariant& p_right);
bool   OperSBigGreatEQSShort  (SQLVariant& p_right);
bool   OperUBigGreatEQSShort  (SQLVariant& p_right);
bool   OperNumGreatEQSShort   (SQLVariant& p_right);
bool   OperSShortGreatEQUShort(SQLVariant& p_right);  // TYPE == USHORT
bool   OperUShortGreatEQUShort(SQLVariant& p_right);
bool   OperSLongGreatEQUShort (SQLVariant& p_right);
bool   OperUlongGreatEQUShort (SQLVariant& p_right);
bool   OperFloatGreatEQUShort (SQLVariant& p_right);
bool   OperDoubleGreatEQUShort(SQLVariant& p_right);
bool   OperBitGreatEQUShort   (SQLVariant& p_right);
bool   OperSTinyGreatEQUShort (SQLVariant& p_right);
bool   OperUTinyGreatEQUShort (SQLVariant& p_right);
bool   OperSBigGreatEQUShort  (SQLVariant& p_right);
bool   OperUBigGreatEQUShort  (SQLVariant& p_right);
bool   OperNumGreatEQUShort   (SQLVariant& p_right);
bool   OperSShortGreatEQSLong (SQLVariant& p_right);  // TYPE == SLONG
bool   OperUShortGreatEQSLong (SQLVariant& p_right);
bool   OperSLongGreatEQSLong  (SQLVariant& p_right);
bool   OperULongGreatEQSLong  (SQLVariant& p_right);
bool   OperFloatGreatEQSLong  (SQLVariant& p_right);
bool   OperDoubleEGreatEQSLong(SQLVariant& p_right);
bool   OperBitGreatEQSLong    (SQLVariant& p_right);
bool   OperSTinyGreatEQSLong  (SQLVariant& p_right);
bool   OperUTinyGreatEQSLong  (SQLVariant& p_right);
bool   OperSBigGreatEQSLong   (SQLVariant& p_right);
bool   OperUBigGreatEQSLong   (SQLVariant& p_right);
bool   OperNumGreatEQSLong    (SQLVariant& p_right);
bool   OperSShortGreatEQULong (SQLVariant& p_right);  // TYPE == ULONG
bool   OperUShortGreatEQULong (SQLVariant& p_right);
bool   OperSLongGreatEQULong  (SQLVariant& p_right);
bool   OperULongGreatEQULong  (SQLVariant& p_right);
bool   OperFloatGreatEQULong  (SQLVariant& p_right);
bool   OperDoubleGreatEQULong (SQLVariant& p_right);
bool   OperBitGreatEQULong    (SQLVariant& p_right);
bool   OperSTinyGreatEQULong  (SQLVariant& p_right);
bool   OperUTinyGreatEQULong  (SQLVariant& p_right);
bool   OperSBigGreatEQULong   (SQLVariant& p_right);
bool   OperUBigGreatEQULong   (SQLVariant& p_right);
bool   OperNumGreatEQULong    (SQLVariant& p_right);
bool   OperSShortGreatEQFloat (SQLVariant& p_right);  // TYPE == FLOAT
bool   OperUShortGreatEQFloat (SQLVariant& p_right);
bool   OperSLongGreatEQFloat  (SQLVariant& p_right);
bool   OperULongGreatEQFloat  (SQLVariant& p_right);
bool   OperFloatGreatEQFloat  (SQLVariant& p_right);
bool   OperDoubleGreatEQFloat (SQLVariant& p_right);
bool   OperBitGreatEQFloat    (SQLVariant& p_right);
bool   OperSTinyGreatEQFloat  (SQLVariant& p_right);
bool   OperUTinyGreatEQFloat  (SQLVariant& p_right);
bool   OperSBigGreatEQFloat   (SQLVariant& p_right);
bool   OperUBigGreatEQFloat   (SQLVariant& p_right);
bool   OperNumGreatEQFloat    (SQLVariant& p_right);
bool   OperSShortGreatEQDouble(SQLVariant& p_right);  // TYPE == DOUBLE
bool   OperUShortGreatEQDouble(SQLVariant& p_right);
bool   OperSLongGreatEQDouble (SQLVariant& p_right);
bool   OperULongGreatEQDouble (SQLVariant& p_right);
bool   OperFloatGreatEQDouble (SQLVariant& p_right);
bool   OperDoubleGreatEQDouble(SQLVariant& p_right);
bool   OperBitGreatEQDouble   (SQLVariant& p_right);
bool   OperSTinyGreatEQDouble (SQLVariant& p_right);
bool   OperUTinyGreatEQDouble (SQLVariant& p_right);
bool   OperSBigGreatEQDouble  (SQLVariant& p_right);
bool   OperUBigGreatEQDouble  (SQLVariant& p_right);
bool   OperNumGreatEQDouble   (SQLVariant& p_right);
bool   OperSShortGreatEQBit   (SQLVariant& p_right);  // TYPE == BIT
bool   OperUShortGreatEQBit   (SQLVariant& p_right);
bool   OperSLongGreatEQBit    (SQLVariant& p_right);
bool   OperULongGreatEQBit    (SQLVariant& p_right);
bool   OperFloatGreatEQBit    (SQLVariant& p_right);
bool   OperDoubleGreatEQBit   (SQLVariant& p_right);
bool   OperBitGreatEQBit      (SQLVariant& p_right);
bool   OperSTinyGreatEQBit    (SQLVariant& p_right);
bool   OperUTinyGreatEQBit    (SQLVariant& p_right);
bool   OperSBigGreatEQBit     (SQLVariant& p_right);
bool   OperUBigGreatEQBit     (SQLVariant& p_right);
bool   OperNumGreatEQBit      (SQLVariant& p_right);
bool   OperSShortGreatEQSTiny (SQLVariant& p_right);  // TYPE == STINYINT
bool   OperUShortGreatEQSTiny (SQLVariant& p_right);
bool   OperSLongGreatEQSTiny  (SQLVariant& p_right);
bool   OperULongGreatEQSTiny  (SQLVariant& p_right);
bool   OperFloatGreatEQSTiny  (SQLVariant& p_right);
bool   OperDoubleGreatEQSTiny (SQLVariant& p_right);
bool   OperBitGreatEQSTiny    (SQLVariant& p_right);
bool   OperSTinyGreatEQSTiny  (SQLVariant& p_right);
bool   OperUTinyGreatEQSTiny  (SQLVariant& p_right);
bool   OperSBigGreatEQSTiny   (SQLVariant& p_right);
bool   OperUBigGreatEQSTiny   (SQLVariant& p_right);
bool   OperNumGreatEQSTiny    (SQLVariant& p_right);
bool   OperSShortGreatEQUTiny (SQLVariant& p_right);  // TYPE == UTINYINT
bool   OperUShortGreatEQUTiny (SQLVariant& p_right);
bool   OperSLongGreatEQUTiny  (SQLVariant& p_right);
bool   OperULongGreatEQUTiny  (SQLVariant& p_right);
bool   OperFloatGreatEQUTiny  (SQLVariant& p_right);
bool   OperDoubleGreatEQUTiny (SQLVariant& p_right);
bool   OperBitGreatEQUTiny    (SQLVariant& p_right);
bool   OperSTinyGreatEQUTiny  (SQLVariant& p_right);
bool   OperUTinyGreatEQUTiny  (SQLVariant& p_right);
bool   OperSBigGreatEQUTiny   (SQLVariant& p_right);
bool   OperUBigGreatEQUTiny   (SQLVariant& p_right);
bool   OperNumGreatEQUTiny    (SQLVariant& p_right);
bool   OperSShortGreatEQSBig  (SQLVariant& p_right);  // TYPE == SBIGINT
bool   OperUShortGreatEQSBig  (SQLVariant& p_right);
bool   OperSLongGreatEQSBig   (SQLVariant& p_right);
bool   OperULongGreatEQSBig   (SQLVariant& p_right);
bool   OperFloatGreatEQSBig   (SQLVariant& p_right);
bool   OperDoubleGreatEQSBig  (SQLVariant& p_right);
bool   OperBitGreatEQSBig     (SQLVariant& p_right);
bool   OperSTinyGreatEQSBig   (SQLVariant& p_right);
bool   OperUTinyGreatEQSBig   (SQLVariant& p_right);
bool   OperSBigGreatEQSBig    (SQLVariant& p_right);
bool   OperUBigGreatEQSBig    (SQLVariant& p_right);
bool   OperNumGreatEQSBig     (SQLVariant& p_right);
bool   OperSShortGreatEQUBig  (SQLVariant& p_right);  // TYPE == UBIGINT
bool   OperUShortGreatEQUBig  (SQLVariant& p_right);
bool   OperSLongGreatEQUBig   (SQLVariant& p_right);
bool   OperULongGreatEQUBig   (SQLVariant& p_right);
bool   OperFloatGreatEQUBig   (SQLVariant& p_right);
bool   OperDoubleGreatEQUBig  (SQLVariant& p_right);
bool   OperBitGreatEQUBig     (SQLVariant& p_right);
bool   OperSTinyGreatEQUBig   (SQLVariant& p_right);
bool   OperUTinyGreatEQUBig   (SQLVariant& p_right);
bool   OperSBigGreatEQUBig    (SQLVariant& p_right);
bool   OperUBigGreatEQUBig    (SQLVariant& p_right);
bool   OperNumGreatEQUBig     (SQLVariant& p_right);
bool   OperSShortGreatEQNum   (SQLVariant& p_right);  // TYPE == NUMERIC
bool   OperUShortGreatEQNum   (SQLVariant& p_right);
bool   OperSLongGreatEQNum    (SQLVariant& p_right);
bool   OperULongGreatEQNum    (SQLVariant& p_right);
bool   OperFloatGreatEQNum    (SQLVariant& p_right);
bool   OperDoubleGreatEQNum   (SQLVariant& p_right);
bool   OperBitGreatEQNum      (SQLVariant& p_right);
bool   OperSTinyGreatEQNum    (SQLVariant& p_right);
bool   OperUTinyGreatEQNum    (SQLVariant& p_right);
bool   OperSBigGreatEQNum     (SQLVariant& p_right);
bool   OperUBigGreatEQNum     (SQLVariant& p_right);
bool   OperNumGreatEQNum      (SQLVariant& p_right);
bool   OperGuidGreatEQGuid    (SQLVariant& p_right);  // TYPE == GUID
bool   OperBinaryGreatEQBinary(SQLVariant& p_right);  // TYPE == BINARY
bool   OperDateGreatEQDate    (SQLVariant& p_right);  // TYPE == DATE
bool   OperStampGreatEQDate   (SQLVariant& p_right);
bool   OperTimeGreatEQTime    (SQLVariant& p_right);  // TYPE == TIME
bool   OperStampGreatEQTime   (SQLVariant& p_right);
bool   OperStampGreatEQStamp  (SQLVariant& p_right);  // TYPE == TIMESTAMP
bool   OperIntYMGreatEQIntYM  (SQLVariant& p_right);  // TYPE == INTERVAL_YEAR_MONTH
bool   OperIntDSGreatEQIntDS  (SQLVariant& p_right);  // TYPE == INTERVAL_DAY_SECOND


