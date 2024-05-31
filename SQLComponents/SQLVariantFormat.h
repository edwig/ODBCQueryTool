////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantFormat.h
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
#include "SQLVariant.h"
#include "SQLDatabase.h"
#include <bcd.h>

namespace SQLComponents
{

// User status
#define USER_DISPLAY         1
#define NUMBER_BUFFER_SIZE 100

class SQLVariantFormat
{
public:
  explicit SQLVariantFormat(SQLVariant* p_variant);
  explicit SQLVariantFormat(SQLVariant& p_variant);
  ~SQLVariantFormat();
  void        Reset();
  void        ResetValue();
  void        ReFormat();
  // All words begin with a capital
  void        StringInitCapital();
  // First letter of first word starts with a capital
  void        StringStartCapital();
  // Set current date and time
  void        SetCurrentDateAndTime();
  // Format the date
  int         FormatDate(XString p_pattern);
  // Is a constant or a numeric / IsConstanteOfNummer
  bool        IsConstantOrNumber(char p_seperator = '.');
  // Converting European values to system values
  int         RemoveValutaEuro(XString& p_string,bool p_enkelValuta);
  // Converting American $ values to dutch values
  int         RemoveValutaDollar(XString& p_string,bool p_enkelValuta);
  // Value of a string as a double
  bcd         StringDecimalValue(XString& p_error);
  // Format according to user interface
  int         FormatNumber(XString p_format,bool p_currency);
  // Do math on dates
  int         DateCalculate(char p_operator,XString p_argument);

  XString     FormatVariantForSQL(SQLDatabase* p_database);
  void        SetVariant(const SQLVariant* p_variant);
  void        SetFormat(XString p_format);
  void        SetUserStatus(int p_status);
  SQLVariant* GetVariant();
  int         GetUserStatus();
  XString&    GetFormat();
private:
  // Internal formatting of a number by a template
  int         FormatNumberTemplate(LPTSTR Getal,LPCTSTR strNumFormat,int p_buflen);
  // Internal formatting of an interval
  XString     FormatVariantAsInterval(SQLDatabase* p_database);
  // Scan for '23-09-2012' or for '2012-09-23' // Beware "09/23/2012" is not supported!
  bool        GetDateFromStringVariant(const SQLVariant* p_variant,XString p_format,DATE_STRUCT* p_date);
  bool        GetTimeFromStringVariant(const SQLVariant* p_variant,XString p_format,TIME_STRUCT* p_date);

  // Is string a (formatted) windows number?
  bool        IsWinNumber(const XString p_string,PTCHAR p_decSeperator,PTCHAR p_thouSeperator,PTCHAR p_valuta,XString* p_newNumber = NULL);


  SQLVariant* m_variant;
  bool        m_owner;
  int         m_userStatus;
  XString     m_format;
};

inline void
SQLVariantFormat::SetVariant(const SQLVariant* p_variant)
{
  if(m_owner && m_variant)
  {
    delete m_variant;
    m_variant = NULL;
  }
  if(p_variant)
  {
    m_variant = new SQLVariant(p_variant);
    m_owner   = true;
  }
}

inline SQLVariant*
SQLVariantFormat::GetVariant()
{
  return m_variant;
}

inline void
SQLVariantFormat::SetUserStatus(int p_status)
{
  m_userStatus = p_status;
}

inline int
SQLVariantFormat::GetUserStatus()
{
  return m_userStatus;
}

inline void
SQLVariantFormat::SetFormat(XString p_format)
{
  m_format = p_format;
}

inline XString&
SQLVariantFormat::GetFormat()
{
  return m_format;
}

// End of namespace
}
