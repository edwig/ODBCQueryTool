////////////////////////////////////////////////////////////////////////
//
// File: SQLTimestampExtract.h
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
#include "SQLLanguage.h"

namespace SQLComponents
{

typedef enum _extractPart
{
  TS_EXT_NONE = 0
 ,TS_EXT_YEAR
 ,TS_EXT_MONTH
 ,TS_EXT_DAY
 ,TS_EXT_HOUR
 ,TS_EXT_MINUTE
 ,TS_EXT_SECOND
}
SQLExtractPart;

typedef enum _calcPart
{
  SQL_TSI_NONE = 0
 ,SQL_TSI_FRAC_SECOND
 ,SQL_TSI_SECOND
 ,SQL_TSI_MINUTE
 ,SQL_TSI_HOUR
 ,SQL_TSI_DAY
 ,SQL_TSI_WEEK
 ,SQL_TSI_MONTH
 ,SQL_TSI_QUARTER
 ,SQL_TSI_YEAR
}
SQLTimestampCalcPart;

}