////////////////////////////////////////////////////////////////////////
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
// CREDITS:
// Loosely based on the article by: Yap Chun Wei
// http://www.codeproject.com/Articles/1636/CSpreadSheet-A-Class-to-Read-and-Write-to-Excel-an
// 
// Version number: See SQLComponents.h
//
#pragma once
#include "SQLMetaInfo.h"
#include "SQLInfo.h"

namespace SQLComponents
{

class SQLDataType
{
public:
  SQLDataType();
  explicit SQLDataType(int p_sqlType);
  explicit SQLDataType(XString p_typeName);
  explicit SQLDataType(MetaColumn& p_column);

  ~SQLDataType();

  // GETTERS
  XString   GetDatatypeName()   { return m_typeName;      }
  int       GetSQLType()        { return m_sqlType;       }
  int       GetColumnSize()     { return m_columnSize;    }
  INT64     GetBufferSize()     { return m_bufferSize;    }
  int       GetDecimalDigits()  { return m_decimalDigits; }  
  int       GetRadix()          { return m_radix;         }
  INT64     GetOctetLength()    { return m_octetLength;   }
  int       GetSubType()        { return m_subType;       }

  bool      GetIsString();      // CHAR, WCHAR, VARCHAR, WVARCHAR
  bool      GetIsInteger();     // 1, 8, 16, 32 or 64 bits Integer
  bool      GetIsNumeric();     // Number like (integer, approx, decimals)
  bool      GetIsDecimal();     // DECIMAL or NUMERIC
  bool      GetIsApproximate(); // FLOAT or DOUBLE
  bool      GetIsDate();        // DATE / TYPE_DATE
  bool      GetIsTime();        // TIME / TYPE_TIME
  bool      GetIsTimestamp();   // TIMESTAMP / TYPE_TIMESTAMP
  bool      GetisInterval();    // INTERVAL_YEAR up to INTERVAL_DAY_TO_SECOND

  // Microsoft Entity Framework datatypes (CSDL like)
  XString   GetAsEdmType();

  // SETTERS
  void      SetTypeName(XString p_typeName)   { m_typeName      = p_typeName; }
  void      SetSQLType(int p_type)            { m_sqlType       = p_type;     }
  void      SetColumnSize(int p_size)         { m_columnSize    = p_size;     }
  void      SetBufferSize(int p_size)         { m_bufferSize    = p_size;     }
  void      SetDecimalDigits(int p_digits)    { m_decimalDigits = p_digits;   }
  void      SetRadix(int p_radix)             { m_radix         = p_radix;    }
  void      SetOctetLength(int p_size)        { m_octetLength   = p_size;     }
  void      SetSubType(int p_type)            { m_subType       = p_type;     }

  // Datatype formatted according to a specific RDBMS dialect
  XString   TypeNameWithPrecisionScale(TypeInfo* p_type);

  // INFO about type names/values
  static  int     FindDatatype   (LPTSTR p_type);
  static  LPCTSTR FindDatatype   (int    p_type);
  static  int     FindParamtype  (LPTSTR p_type);
  static  LPCTSTR FindParamtype  (int    p_type);
  static  int     FindSQLDatatype(LPTSTR p_type);
  static  LPCTSTR FindSQLDatatype(int    p_type);

private:
  XString m_typeName;
  int     m_sqlType       { 0 };  // SQL_C_* Macro SQL TYPE
  int     m_columnSize    { 0 };  // Display size
  INT64   m_bufferSize    { 0 };  // Physical buffer size
  int     m_decimalDigits { 0 };  // Decimal digits for DECIMAL type
  int     m_radix         { 0 };  // Radix for numeric types
  INT64   m_octetLength   { 0 };  // In case of a (N)VARCHAR type
  int     m_subType       { 0 };  // IN case of TIME/TIMESTAMP with a fraction
};

}
