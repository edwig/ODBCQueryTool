////////////////////////////////////////////////////////////////////////
//
// File: SQLFunction.h
//
// Copyright (c) 1998-2025 ir. W.E. Huisman
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
#include "SQLComponents.h"

namespace SQLComponents
{

// All ODBC {fn } escaped functions
typedef enum _sqlFunction
{
   FN_NOP = 0
  ,FN_ASCII
  ,FN_BIT_LENGTH
  ,FN_CHAR
  ,FN_CHAR_LENGTH
  ,FN_CHARACTER_LENGTH
  ,FN_CONCAT
  ,FN_DIFFERENCE
  ,FN_INSERT
  ,FN_LCASE
  ,FN_LEFT
  ,FN_LENGTH
  ,FN_LOCATE
  ,FN_LTRIM
  ,FN_OCTET_LENGTH
  ,FN_POSITION
  ,FN_INSTRING
  ,FN_REPEAT
  ,FN_REPLACE
  ,FN_RIGHT
  ,FN_RTRIM
  ,FN_SOUNDEX
  ,FN_SPACE
  ,FN_SUBSTRING
  ,FN_UCASE
  // NUMERIC
  ,FN_ABS
  ,FN_ACOS
  ,FN_ASIN
  ,FN_ATAN
  ,FN_ATAN2
  ,FN_CEILING
  ,FN_COS
  ,FN_COT
  ,FN_DEGREES
  ,FN_EXP
  ,FN_FLOOR
  ,FN_LOG
  ,FN_LOG10
  ,FN_MOD
  ,FN_PI
  ,FN_POWER
  ,FN_RADIANS
  ,FN_RAND
  ,FN_ROUND
  ,FN_SIGN
  ,FN_SIN
  ,FN_SQRT
  ,FN_TAN
  ,FN_TRUNCATE
  // TIME, DATE
  ,FN_CURRENT_DATE
  ,FN_CURRENT_TIME
  ,FN_CURRENT_TIMESTAMP
  ,FN_CURDATE
  ,FN_CURTIME
  ,FN_DAYNAME
  ,FN_DAYOFMONTH
  ,FN_DAYOFWEEK
  ,FN_DAYOFYEAR
  ,FN_EXTRACT
  ,FN_HOUR
  ,FN_MINUTE
  ,FN_SECOND
  ,FN_MONTH
  ,FN_MONTHNAME
  ,FN_NOW
  ,FN_QUARTER
  ,FN_TIMESTAMPADD
  ,FN_TIMESTAMPDIFF
  ,FN_WEEK
  ,FN_YEAR
  // SYSTEM
  ,FN_DATABASE
  ,FN_IFNULL
  ,FN_USER
  // HAVING FUNCTIONS
  ,FN_SUM
  ,FN_COUNT
  ,FN_COUNTDIST
  ,FN_MIN
  ,FN_MAX
  ,FN_AVG
  // EXTRA FUNCTIONS
  ,FN_REGEXP_MATCH
}
SQLFunction;

}
