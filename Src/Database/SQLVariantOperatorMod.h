////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantOperatorMod.h
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
// This is the part of the class that takes care of the % operator

   // Arrays with calculate functions for the operators
   static CalculateFunctionArray OperatorMod[CT_LAST][CT_LAST];

   // Calculate functions for the modulo operator
   SQLVariant OperSShortModChar  (SQLVariant& p_right);  // TYPE == CHAR
   SQLVariant OperUShortModChar  (SQLVariant& p_right);
   SQLVariant OperSLongModChar   (SQLVariant& p_right);
   SQLVariant OperULongModChar   (SQLVariant& p_right);
   SQLVariant OperFloatModChar   (SQLVariant& p_right);
   SQLVariant OperDoubleModChar  (SQLVariant& p_right);
   SQLVariant OperBitModChar     (SQLVariant& p_right);
   SQLVariant OperSTinyModChar   (SQLVariant& p_right);
   SQLVariant OperUTinyModChar   (SQLVariant& p_right);
   SQLVariant OperSBigModChar    (SQLVariant& p_right);
   SQLVariant OperUBigModChar    (SQLVariant& p_right);
   SQLVariant OperNumModChar     (SQLVariant& p_right);
   SQLVariant OperCharModSShort  (SQLVariant& p_right);  // TYPE == SSHORT
   SQLVariant OperSShortModSShort(SQLVariant& p_right);
   SQLVariant OperUShortModSShort(SQLVariant& p_right);
   SQLVariant OperSLongModSShort (SQLVariant& p_right);
   SQLVariant OperULongModSShort (SQLVariant& p_right);
   SQLVariant OperFloatModSShort (SQLVariant& p_right);
   SQLVariant OperDoubleModSShort(SQLVariant& p_right);
   SQLVariant OperBitModSShort   (SQLVariant& p_right);
   SQLVariant OperSTinyModSShort (SQLVariant& p_right);
   SQLVariant OperUTinyModSShort (SQLVariant& p_right);
   SQLVariant OperSBigModSShort  (SQLVariant& p_right);
   SQLVariant OperUBigModSShort  (SQLVariant& p_right);
   SQLVariant OperNumModSShort   (SQLVariant& p_right);
   SQLVariant OperIntYMModSShort (SQLVariant& p_right);
   SQLVariant OperIntDSModSShort (SQLVariant& p_right);
   SQLVariant OperCharModUShort  (SQLVariant& p_right);  // TYPE == USHORT
   SQLVariant OperSShortModUShort(SQLVariant& p_right);
   SQLVariant OperUShortModUShort(SQLVariant& p_right);
   SQLVariant OperSLongModUShort (SQLVariant& p_right);
   SQLVariant OperUlongModUShort (SQLVariant& p_right);
   SQLVariant OperFloatModUShort (SQLVariant& p_right);
   SQLVariant OperDoubleModUShort(SQLVariant& p_right);
   SQLVariant OperBitModUShort   (SQLVariant& p_right);
   SQLVariant OperSTinyModUShort (SQLVariant& p_right);
   SQLVariant OperUTinyModUShort (SQLVariant& p_right);
   SQLVariant OperSBigModUShort  (SQLVariant& p_right);
   SQLVariant OperUBigModUShort  (SQLVariant& p_right);
   SQLVariant OperNumModUShort   (SQLVariant& p_right);
   SQLVariant OperIntYMModUShort (SQLVariant& p_right);
   SQLVariant OperIntDSModUShort (SQLVariant& p_right);
   SQLVariant OperCharModSLong   (SQLVariant& p_right);  // TYPE == SLONG
   SQLVariant OperSShortModSLong (SQLVariant& p_right);
   SQLVariant OperUShortModSLong (SQLVariant& p_right);
   SQLVariant OperSLongModSLong  (SQLVariant& p_right);
   SQLVariant OperULongModSLong  (SQLVariant& p_right);
   SQLVariant OperFloatModSLong  (SQLVariant& p_right);
   SQLVariant OperDoubleModSLong (SQLVariant& p_right);
   SQLVariant OperBitModSLong    (SQLVariant& p_right);
   SQLVariant OperSTinyModSLong  (SQLVariant& p_right);
   SQLVariant OperUTinyModSLong  (SQLVariant& p_right);
   SQLVariant OperSBigModSLong   (SQLVariant& p_right);
   SQLVariant OperUBigModSLong   (SQLVariant& p_right);
   SQLVariant OperNumModSLong    (SQLVariant& p_right);
   SQLVariant OperIntYMModSLong  (SQLVariant& p_right);
   SQLVariant OperIntDSModSLong  (SQLVariant& p_right);
   SQLVariant OperCharModULong   (SQLVariant& p_right);  // TYPE == ULONG
   SQLVariant OperSShortModULong (SQLVariant& p_right);
   SQLVariant OperUShortModULong (SQLVariant& p_right);
   SQLVariant OperSLongModULong  (SQLVariant& p_right);
   SQLVariant OperULongModULong  (SQLVariant& p_right);
   SQLVariant OperFloatModULong  (SQLVariant& p_right);
   SQLVariant OperDoubleModULong (SQLVariant& p_right);
   SQLVariant OperBitModULong    (SQLVariant& p_right);
   SQLVariant OperSTinyModULong  (SQLVariant& p_right);
   SQLVariant OperUTinyModULong  (SQLVariant& p_right);
   SQLVariant OperSBigModULong   (SQLVariant& p_right);
   SQLVariant OperUBigModULong   (SQLVariant& p_right);
   SQLVariant OperNumModULong    (SQLVariant& p_right);
   SQLVariant OperIntYMModULong  (SQLVariant& p_right);
   SQLVariant OperIntDSModULong  (SQLVariant& p_right);
   SQLVariant OperCharModFloat   (SQLVariant& p_right);  // TYPE == FLOAT
   SQLVariant OperSShortModFloat (SQLVariant& p_right);
   SQLVariant OperUShortModFloat (SQLVariant& p_right);
   SQLVariant OperSLongModFloat  (SQLVariant& p_right);
   SQLVariant OperULongModFloat  (SQLVariant& p_right);
   SQLVariant OperFloatModFloat  (SQLVariant& p_right);
   SQLVariant OperDoubleModFloat (SQLVariant& p_right);
   SQLVariant OperBitModFloat    (SQLVariant& p_right);
   SQLVariant OperSTinyModFloat  (SQLVariant& p_right);
   SQLVariant OperUTinyModFloat  (SQLVariant& p_right);
   SQLVariant OperSBigModFloat   (SQLVariant& p_right);
   SQLVariant OperUBigModFloat   (SQLVariant& p_right);
   SQLVariant OperNumModFloat    (SQLVariant& p_right);
   SQLVariant OperIntYMModFloat  (SQLVariant& p_right);
   SQLVariant OperIntDSModFloat  (SQLVariant& p_right);
   SQLVariant OperCharModDouble  (SQLVariant& p_right);  // TYPE == DOUBLE
   SQLVariant OperSShortModDouble(SQLVariant& p_right);
   SQLVariant OperUShortModDouble(SQLVariant& p_right);
   SQLVariant OperSLongModDouble (SQLVariant& p_right);
   SQLVariant OperULongModDouble (SQLVariant& p_right);
   SQLVariant OperFloatModDouble (SQLVariant& p_right);
   SQLVariant OperDoubleModDouble(SQLVariant& p_right);
   SQLVariant OperBitModDouble   (SQLVariant& p_right);
   SQLVariant OperSTinyModDouble (SQLVariant& p_right);
   SQLVariant OperUTinyModDouble (SQLVariant& p_right);
   SQLVariant OperSBigModDouble  (SQLVariant& p_right);
   SQLVariant OperUBigModDouble  (SQLVariant& p_right);
   SQLVariant OperNumModDouble   (SQLVariant& p_right);
   SQLVariant OperIntYMModDouble (SQLVariant& p_right);
   SQLVariant OperIntDSModDouble (SQLVariant& p_right);
   SQLVariant OperCharModBit     (SQLVariant& p_right);  // TYPE == BIT
   SQLVariant OperSShortModBit   (SQLVariant& p_right);
   SQLVariant OperUShortModBit   (SQLVariant& p_right);
   SQLVariant OperSLongModBit    (SQLVariant& p_right);
   SQLVariant OperULongModBit    (SQLVariant& p_right);
   SQLVariant OperFloatModBit    (SQLVariant& p_right);
   SQLVariant OperDoubleModBit   (SQLVariant& p_right);
   SQLVariant OperBitModBit      (SQLVariant& p_right);
   SQLVariant OperSTinyModBit    (SQLVariant& p_right);
   SQLVariant OperUTinyModBit    (SQLVariant& p_right);
   SQLVariant OperSBigModBit     (SQLVariant& p_right);
   SQLVariant OperUBigModBit     (SQLVariant& p_right);
   SQLVariant OperNumModBit      (SQLVariant& p_right);
   SQLVariant OperIntYMModBit    (SQLVariant& p_right);
   SQLVariant OperIntDSModBit    (SQLVariant& p_right);
   SQLVariant OperCharModSTiny   (SQLVariant& p_right);  // TYPE == STINYINT
   SQLVariant OperSShortModSTiny (SQLVariant& p_right);
   SQLVariant OperUShortModSTiny (SQLVariant& p_right);
   SQLVariant OperSLongModSTiny  (SQLVariant& p_right);
   SQLVariant OperULongModSTiny  (SQLVariant& p_right);
   SQLVariant OperFloatModSTiny  (SQLVariant& p_right);
   SQLVariant OperDoubleModSTiny (SQLVariant& p_right);
   SQLVariant OperBitModSTiny    (SQLVariant& p_right);
   SQLVariant OperSTinyModSTiny  (SQLVariant& p_right);
   SQLVariant OperUTinyModSTiny  (SQLVariant& p_right);
   SQLVariant OperSBigModSTiny   (SQLVariant& p_right);
   SQLVariant OperUBigModSTiny   (SQLVariant& p_right);
   SQLVariant OperNumModSTiny    (SQLVariant& p_right);
   SQLVariant OperIntYMModSTiny  (SQLVariant& p_right);
   SQLVariant OperIntDSModSTiny  (SQLVariant& p_right);
   SQLVariant OperCharModUTiny   (SQLVariant& p_right);  // TYPE == UTINYINT
   SQLVariant OperSShortModUTiny (SQLVariant& p_right);
   SQLVariant OperUShortModUTiny (SQLVariant& p_right);
   SQLVariant OperSLongModUTiny  (SQLVariant& p_right);
   SQLVariant OperULongModUTiny  (SQLVariant& p_right);
   SQLVariant OperFloatModUTiny  (SQLVariant& p_right);
   SQLVariant OperDoubleModUTiny (SQLVariant& p_right);
   SQLVariant OperBitModUTiny    (SQLVariant& p_right);
   SQLVariant OperSTinyModUTiny  (SQLVariant& p_right);
   SQLVariant OperUTinyModUTiny  (SQLVariant& p_right);
   SQLVariant OperSBigModUTiny   (SQLVariant& p_right);
   SQLVariant OperUBigModUTiny   (SQLVariant& p_right);
   SQLVariant OperNumModUTiny    (SQLVariant& p_right);
   SQLVariant OperIntYMModUTiny  (SQLVariant& p_right);
   SQLVariant OperIntDSModUTiny  (SQLVariant& p_right);
   SQLVariant OperCharModSBig    (SQLVariant& p_right);  // TYPE == SBIGINT
   SQLVariant OperSShortModSBig  (SQLVariant& p_right);
   SQLVariant OperUShortModSBig  (SQLVariant& p_right);
   SQLVariant OperSLongModSBig   (SQLVariant& p_right);
   SQLVariant OperULongModSBig   (SQLVariant& p_right);
   SQLVariant OperFloatModSBig   (SQLVariant& p_right);
   SQLVariant OperDoubleModSBig  (SQLVariant& p_right);
   SQLVariant OperBitModSBig     (SQLVariant& p_right);
   SQLVariant OperSTinyModSBig   (SQLVariant& p_right);
   SQLVariant OperUTinyModSBig   (SQLVariant& p_right);
   SQLVariant OperSBigModSBig    (SQLVariant& p_right);
   SQLVariant OperUBigModSBig    (SQLVariant& p_right);
   SQLVariant OperNumModSBig     (SQLVariant& p_right);
   SQLVariant OperIntYMModSBig   (SQLVariant& p_right);
   SQLVariant OperIntDSModSBig   (SQLVariant& p_right);
   SQLVariant OperCharModUBig    (SQLVariant& p_right);  // TYPE == UBIGINT
   SQLVariant OperSShortModUBig  (SQLVariant& p_right);
   SQLVariant OperUShortModUBig  (SQLVariant& p_right);
   SQLVariant OperSLongModUBig   (SQLVariant& p_right);
   SQLVariant OperULongModUBig   (SQLVariant& p_right);
   SQLVariant OperFloatModUBig   (SQLVariant& p_right);
   SQLVariant OperDoubleModUBig  (SQLVariant& p_right);
   SQLVariant OperBitModUBig     (SQLVariant& p_right);
   SQLVariant OperSTinyModUBig   (SQLVariant& p_right);
   SQLVariant OperUTinyModUBig   (SQLVariant& p_right);
   SQLVariant OperSBigModUBig    (SQLVariant& p_right);
   SQLVariant OperUBigModUBig    (SQLVariant& p_right);
   SQLVariant OperNumModUBig     (SQLVariant& p_right);
   SQLVariant OperIntYMModUBig   (SQLVariant& p_right);
   SQLVariant OperIntDSModUBig   (SQLVariant& p_right);
   SQLVariant OperCharModNum     (SQLVariant& p_right);  // TYPE == NUMERIC
   SQLVariant OperSShortModNum   (SQLVariant& p_right);
   SQLVariant OperUShortModNum   (SQLVariant& p_right);
   SQLVariant OperSLongModNum    (SQLVariant& p_right);
   SQLVariant OperULongModNum    (SQLVariant& p_right);
   SQLVariant OperFloatModNum    (SQLVariant& p_right);
   SQLVariant OperDoubleModNum   (SQLVariant& p_right);
   SQLVariant OperBitModNum      (SQLVariant& p_right);
   SQLVariant OperSTinyModNum    (SQLVariant& p_right);
   SQLVariant OperUTinyModNum    (SQLVariant& p_right);
   SQLVariant OperSBigModNum     (SQLVariant& p_right);
   SQLVariant OperUBigModNum     (SQLVariant& p_right);
   SQLVariant OperNumModNum      (SQLVariant& p_right);
   SQLVariant OperIntYMModNum    (SQLVariant& p_right);
   SQLVariant OperIntDSModNum    (SQLVariant& p_right);
