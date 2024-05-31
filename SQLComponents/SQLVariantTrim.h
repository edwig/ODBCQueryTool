////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantTrim.h
//
// Copyright (c) 1998-2024 ir. W.E. Huisman
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
#pragma once

namespace SQLComponents
{
// Internal conversion and trimming
// Used for requesting a value as another type
// and for the operators where both sides have different types

// Throw error as a result of internal trimming
void           SQL_ThrowErrorTruncate(int p_from,int p_to);

// Internal conversion and trimming
long           SQL_ULongToLong(unsigned long p_value);
long           SQL_FloatToLong(float p_value);
long           SQL_DoubleToLong(double p_value);
long           SQL_BIGINTToLong(SQLBIGINT p_value);
long           SQL_UBIGINTToLong(SQLUBIGINT p_value);
short          SQL_UShortToShort(unsigned short p_value);
short          SQL_SLongToShort(long p_value);
short          SQL_ULongToShort(unsigned long p_value);
short          SQL_FloatToShort(float p_value);
short          SQL_DoubleToShort(double p_value);
short          SQL_BIGINTToShort(SQLBIGINT p_value);
short          SQL_UBIGINTToShort(SQLUBIGINT p_value);
float          SQL_DoubleToFloat(double p_value);
float          SQL_BIGINTToFloat(SQLBIGINT p_value);
float          SQL_UBIGINTToFloat(SQLUBIGINT p_value);
double         SQL_BIGINTToDouble(SQLBIGINT p_value);
double         SQL_UBIGINTToDouble(SQLUBIGINT p_value);
SQLBIGINT      SQL_FloatToBIGINT(float p_value);
SQLBIGINT      SQL_DoubleToBIGINT(double p_value);
SQLBIGINT      SQL_UBIGINTToBIGINT(SQLUBIGINT p_value);
char           SQL_ShortToTinyInt(short p_value);
char           SQL_UShortToTinyInt(unsigned short p_value);
char           SQL_SLongToTinyInt(long p_value);
char           SQL_ULongToTinyInt(long p_value);
char           SQL_FloatToTinyInt(float p_value);
char           SQL_DoubleToTinyInt(double p_value);
char           SQL_UTinyIntToTinyInt(unsigned char p_value);
char           SQL_SBIGINTToTinyInt(SQLBIGINT p_value);
char           SQL_UBIGINTToTinyInt(SQLUBIGINT p_value);
unsigned long  SQL_ShortToULong(short p_value);
unsigned long  SQL_LongToULong(long p_value);
unsigned long  SQL_FloatToULong(float p_value);
unsigned long  SQL_DoubleToULong(double p_value);
unsigned long  SQL_TinyIntToULong(char p_value);
unsigned long  SQL_BIGINTToULong(SQLBIGINT p_value);
unsigned long  SQL_UBIGINTToULong(SQLUBIGINT p_value);
SQLUBIGINT     SQL_ShortToUBIGINT(short p_value);
SQLUBIGINT     SQL_LongToUBIGINT(long p_value);
SQLUBIGINT     SQL_FloatToUBIGINT(float p_value);
SQLUBIGINT     SQL_DoubleToUBIGINT(double p_value);
SQLUBIGINT     SQL_TinyIntToUBIGINT(char p_value);
SQLUBIGINT     SQL_SBIGINTToUBIGINT(SQLBIGINT p_value);
unsigned short SQL_ShortToUShort(short p_value);
unsigned short SQL_SLongToUShort(long p_value);
unsigned short SQL_ULongToUShort(unsigned long p_value);
unsigned short SQL_FloatToUShort(float p_value);
unsigned short SQL_DoubleToUShort(double p_value);
unsigned short SQL_TinyIntToUShort(char p_value);
unsigned short SQL_BIGINTToUShort(SQLBIGINT p_value);
unsigned short SQL_UBIGINTToUShort(SQLUBIGINT p_value);
unsigned char  SQL_ShortToUTinyInt(short p_value);
unsigned char  SQL_UShortToUTinyInt(short p_value);
unsigned char  SQL_SLongToUTinyInt(long p_value);
unsigned char  SQL_ULongToUTinyInt(unsigned long p_value);
unsigned char  SQL_FloatToUTinyInt(float p_value);
unsigned char  SQL_DoubleToUTinyInt(double p_value);
unsigned char  SQL_TinyIntToUTinyInt(char p_value);
unsigned char  SQL_SBIGINTToUTinyInt(SQLBIGINT p_value);
unsigned char  SQL_UBIGINTToUTinyInt(SQLUBIGINT p_value);

// End of namespace
}
