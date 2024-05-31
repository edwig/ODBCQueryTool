////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantFormat.cpp
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
#include "stdafx.h"
#include "SQLComponents.h"
#include "SQLVariantFormat.h"
#include "SQLDataType.h"
#include "SQLFormatErrors.h"
#include "SQLTime.h"
#include "SQLDate.h"
#include "SQLTimestamp.h"
#include "SQLInfoDB.h"
#include <bcd.h>
#include <StringUtilities.h>
#include <locale.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{

SQLVariantFormat::SQLVariantFormat(SQLVariant* p_variant)
                 :m_variant(p_variant)
                 ,m_userStatus(0)
                 ,m_owner(false)
{
  ::InitValutaString();
}

SQLVariantFormat::SQLVariantFormat(SQLVariant& p_variant)
                 :m_variant(&p_variant)
                 ,m_userStatus(0)
                 ,m_owner(false)
{
  ::InitValutaString();
}

SQLVariantFormat::~SQLVariantFormat()
{
  if(m_owner && m_variant)
  {
    delete m_variant;
  }
  m_variant = nullptr;
}

void
SQLVariantFormat::Reset()
{
  if(m_owner && m_variant)
  {
    delete m_variant;
  }
  m_variant    = nullptr;
  m_userStatus = 0;
  m_format     = _T("");
}

void
SQLVariantFormat::ResetValue()
{
  if(m_owner && m_variant)
  {
    delete m_variant;
  }
  m_variant = nullptr;
  m_format  = _T("");
}

void
SQLVariantFormat::ReFormat()
{
  if(m_variant)
  {
    m_variant->GetAsString(m_format);
  }
}

// All words get a initial capital
// The rest of the words become lower case
// Diacritic letters within words will be left alone
void
SQLVariantFormat::StringInitCapital()
{
  bool doCapital = true;
  int     length = m_format.GetLength();
  LPCTSTR buffer = m_format.GetBuffer();

  for(int ind = 0; ind < length; ++ind)
  {
    TCHAR ch = buffer[ind];

    if(isblank(ch))
    {
      doCapital = true;
    }
    else
    {
      if(isalpha(ch))
      {
        if(doCapital)
        {
          m_format.SetAt(ind, (TCHAR)toupper(ch));
        }
        else
        {
          m_format.SetAt(ind, (TCHAR)tolower(ch));
        }
      }
      doCapital = false;
    }
  }
}

// First character of the string becomes a capital
// The rest becomes all lower case
// But only if the first character is NOT a diacritic character
void
SQLVariantFormat::StringStartCapital()
{
  bool doCapital = true;
  int     length = m_format.GetLength();
  LPCTSTR buffer = m_format.GetBuffer();

  for(int ind = 0;ind < length; ++ind)
  {
    TCHAR ch = buffer[ind];

    if(isblank(ch) == false)
    {
      if(isalpha(ch))
      {
        m_format.SetAt(ind,doCapital ? (TCHAR)toupper(ch) : (TCHAR) tolower(ch));
      }
      doCapital = false;
    }
  }
}

// The format is ONE constant or a simple number (-,+, . included)
// Characters '-', '+' and '.' may only be scanned once!
bool
SQLVariantFormat::IsConstantOrNumber(char p_seperator /*='.'*/)
{
  bool isDecimal = false;
  bool isSpace   = false;
  bool isSign    = false;
  bool isNumber  = false;
  XString string = m_format;

  string.Trim();
  int pos = 0;

  while(pos < string.GetLength())
  {
    int ch = string.GetAt(pos);
    if(isdigit(ch))
    {
      if(isSpace)
      {
        return false;
      }
      isNumber = true;
    }
    else if(ch == '-' || ch == '+')
    {
      if(isNumber || isSpace || isDecimal || isSign)
      {
        return false;
      }
      isSign = true;
    }
    else if(ch == p_seperator)
    {
      if(isSpace || isDecimal)
      {
        return false;
      }
      isDecimal = true;
    }
    else if(isspace(ch))
    {
      isSpace = true;
    }
    else
    {
      return false;
    }
    // Next character
    ++pos;
  }
  return true;
}

int 
SQLVariantFormat::RemoveValutaEuro(XString& p_string,bool p_enkelValuta)
{
  p_string.Replace(_T(" ")   ,_T(""));
  p_string.Replace(_T("EUR."),_T(""));
  p_string.Replace(_T("eur."),_T(""));
  p_string.Replace(_T("E.")  ,_T(""));
  p_string.Replace(_T("E")   ,_T(""));

  if(! p_enkelValuta)
  {
    p_string.Replace(_T("."),_T(""));
    p_string.Replace(_T(","),_T("."));
  }
  return p_string.GetLength();
}

int 
SQLVariantFormat::RemoveValutaDollar(XString& p_string,bool p_enkelValuta)
{
  p_string.Replace(_T(" ") ,_T(""));
  p_string.Replace(_T("$."),_T(""));
  p_string.Replace(_T("$") ,_T(""));

  if(! p_enkelValuta)
  {
    p_string.Replace(_T(","),_T(""));
    p_string.Replace(_T("."),_T(","));
  }
  return p_string.GetLength();
}

// Is string a (formatted) windows number?
bool
SQLVariantFormat::IsWinNumber(const XString p_string
                             ,PTCHAR        p_decSeperator
                             ,PTCHAR        p_thouSeperator
                             ,PTCHAR        p_valuta
                             ,XString*      p_newNumber)
{
  XString newNumber(_T("+"));
  bool isDecimal   = false;   // decimal separator found
  bool isThousend  = false;   // thousand separator found
  bool isSpace     = false;   // Spaces behind the number
  bool isSign      = false;   // Found a sign
  bool isSignAfter = false;   // Sign after the number
  bool isNumber    = false;   // First digit found
  bool isNegative  = false;   // Negative number
  int  stringPos   = 0;

  int DSLen = 0;
  int TSLen = 0;
  int VALen = 0;

  ::InitValutaString();

  if(p_decSeperator != nullptr)
  {
    DSLen = (int)_tcslen(p_decSeperator);
  }
  if(p_thouSeperator != nullptr)
  {
    TSLen = (int)_tcslen(p_thouSeperator);
  }
  if(p_valuta != nullptr)
  {
    VALen = (int)_tcslen(p_valuta);
  }
  while(isspace(p_string.GetAt(stringPos)))
  {
    ++stringPos;
  }
  while(stringPos < p_string.GetLength())
  {
    if(isdigit(p_string.GetAt(stringPos)))
    {
      if(isSpace || isSignAfter)
      {
        return false;
      }
      isNumber = true;
      newNumber += p_string.GetAt(stringPos++);
    }
    else if(p_string.GetAt(stringPos) == _T('-') ||
            p_string.GetAt(stringPos) == _T('+') )
    {
      if(isSign)
      {
        return false;
      }
      isSign      = true;
      isSignAfter = isNumber;
      isNegative  = (p_string.GetAt(stringPos) == _T('-'));
      newNumber.SetAt(0,p_string.GetAt(stringPos++));
    }
    else if(DSLen > 0 && _tcsncmp(p_string.Mid(stringPos),p_decSeperator,DSLen) == 0)
    {
      if(isSpace || isDecimal)
      {
        return false;
      }
      if(!isNumber)
      {
        newNumber += '0';
      }
      newNumber += '.';
      isNumber   = true;
      isDecimal  = true;
      stringPos += DSLen;
    }
    else if(TSLen > 0 && _tcsncmp(p_string.Mid(stringPos),p_thouSeperator,TSLen) == 0)
    {
      if(isSpace || isDecimal)
      {
        return false;
      }
      stringPos += TSLen;
      isThousend = true;
    }
    else if(VALen > 0 && _tcsncmp(p_string.Mid(stringPos),p_valuta,VALen) == 0)
    {
      stringPos += VALen;
    }
    else if(isspace(p_string.GetAt(stringPos)))
    {
      if(isNumber || isDecimal)
      {
        isSpace = true;
      }
      ++stringPos;
    }
    else
    {
      return false;
    }
  }

  // Eventually place extra 0 after a number ending in a decimal point
  if(newNumber.Right(1) == _T('.'))
  {
    newNumber += _T('0');
  }
  if(isNumber && p_newNumber != nullptr)
  {
    *p_newNumber = newNumber;
  }
  return true;
}

bcd
SQLVariantFormat::StringDecimalValue(CString& p_error)
{
  bcd result;
  ::InitValutaString();

  if(IsConstantOrNumber())
  {
    result = bcd(m_format);
  }
  else
  {
    XString waarde = m_format;
    if(IsConstantOrNumber(','))
    {
      RemoveValutaEuro(waarde,false);
      result = bcd(waarde);
    }
    else
    {
      XString newGetal;
      if(IsWinNumber(waarde,::g_locale_decimalSep,::g_locale_thousandSep,::g_locale_strCurrency,&newGetal))
      {
        result = bcd(newGetal);
      }
      else
      {
        XString newWaarde = waarde;
        RemoveValutaEuro(newWaarde,true);
        if(IsWinNumber(newWaarde,::g_locale_decimalSep,::g_locale_thousandSep,::g_locale_strCurrency,&newGetal))
        {
          result = bcd(newGetal);
        }
        else
        {
          if(IsWinNumber(newWaarde,const_cast<TCHAR*>(_T(",")),const_cast<TCHAR*>(_T(".")),::g_locale_strCurrency,&newGetal))
          {
            result = bcd(newGetal);
          }
          else
          {
            if(IsWinNumber(newWaarde,const_cast<TCHAR*>(_T(".")),const_cast<TCHAR*>(_T(",")),::g_locale_strCurrency,&newGetal))
            {
              result = bcd(newGetal);
            }
            else
            {
              // Cross our fingers and hope to die!!
              try
              {
                result = bcd(waarde);
              }
              catch(StdException& ex)
              {
                p_error  = ex.GetErrorMessage();
                p_error += " : ";
                p_error += waarde;
                result.Zero();
              }
            }
          }
        }
      }
    }
  }
  return result;
}

// Setting the current date and time
void
SQLVariantFormat::SetCurrentDateAndTime()
{
  // Making place for our own variant
  if(m_variant && m_owner)
  {
    delete m_variant;
    m_variant = nullptr;
  }

  // Setting the current timestamp in the format string
  // Does so in standard ISO order "Y-M-D H:M:S" 
  SQLTimestamp stamp = SQLTimestamp::CurrentTimestamp();
  m_format = stamp.AsXMLString();
  m_format.Replace(_T("T"),_T(" "));

  // Getting a new variant
  m_variant = new SQLVariant(&stamp);
  m_owner   = true;
}

// Getting a time value from a string
// There can be various info before the time part, but not AFTER it
// E.g. in most cases it has a date before it, so we can read from a timestamp string
bool
SQLVariantFormat::GetTimeFromStringVariant(const SQLVariant* p_variant,XString p_format,TIME_STRUCT* p_time)
{
  ZeroMemory(p_time,sizeof(TIME_STRUCT));
  
  XString theTime;
  if(p_variant)
  {
    p_variant->GetAsString(theTime);
  }
  else
  {
    theTime = p_format;
  }
  int pos = theTime.Find(_T(':'));
  if(pos < 0)
  {
    return false;
  }

  // Is there a time present in the string?
  if(pos > 0 && isdigit(theTime.GetAt(pos - 1)))
  {
    --pos;
    if(pos > 0 && isdigit(theTime.GetAt(pos - 1)))
    {
      --pos;
    }
  }
  theTime = theTime.Mid(pos);
  int our = 0;
  int min = 0;
  int sec = 0;
  if(_stscanf_s(theTime,_T("%d:%d:%d"),&our,&min,&sec) == 3)
  {
    if(our >= 0 && our < 23 &&
       min >= 0 && min < 60 &&
       sec >= 0 && min < 61)  // Allow for leap second!
    {
      p_time->hour   = (SQLUSMALLINT)our;
      p_time->minute = (SQLUSMALLINT)min;
      p_time->second = (SQLUSMALLINT)sec;
      return true;
    }
  }
  return false;
}

// Find a date at the beginning of a string. 
// Dates can be given in various formats. The following are recognized:
// DDMMYY       -> Compact 2 digit year/month/day
// DDMMYYYY     -> Compact 4 digit year/month/day
// DD/MM/YY     -> American formatting 2 digit year/month/day
// DD/MM/YYYY   -> American formatting 4 digit year/month/day
// DD-MM-YY     -> ISO formatting 2 digit year/month/day
// DD-MM-YYYY   -> ISO formatting 4 digit year/month/day
// YYYY-MM-DD   -> ISO formatting 4 digit year/month day
// YYYY/MM/DD   -> American formatting 4 digit year/month/day
// NOW          -> Current day (and Dutch/French/German forms)
// Dates can have coded additions or subtractions
// <date> [- | +] n [DAY | MONTH | YEAR]
bool
SQLVariantFormat::GetDateFromStringVariant(const SQLVariant* p_variant,XString p_format,DATE_STRUCT* p_date)
{
  ZeroMemory(p_date,sizeof(DATE_STRUCT));

  XString theDate;
  if(p_variant)
  {
    p_variant->GetAsString(theDate);
  }
  if(theDate.IsEmpty() && !p_format.IsEmpty())
  {
    theDate = p_format;
  }

  // To fix the American date formats
  theDate.Replace(_T("/"),_T("-"));

  // Let SQLDate catch formats like "dd" "ddmm" and "ddmmyy[yy]"
  // And all special cases where we do "NOW +1 DAY" etc
  int countMinus = CountOfChars(theDate,_T('-'));
  int countPlus  = CountOfChars(theDate,_T('+'));

  if(countMinus != 2 || countPlus)
  {
    try
    {
      SQLDate lang(theDate);
      lang.AsDateStruct(p_date);
      return true;
    }
    catch(StdException& er)
    {
      ReThrowSafeException(er);
    }
    return false;
  }
  int posDate = theDate.Find(_T('-'));
  int posDat2 = theDate.Find(_T('-'),posDate + 1);
  if(posDat2 < 0)
  {
    return false;
  }
  if(posDate == 2 && posDat2 == 5)
  {
    // Formatted as "dd-mm-yy[yy]" (from a date)
    p_date->day   = (SQLUSMALLINT) _ttoi(theDate.Left(2));
    p_date->month = (SQLUSMALLINT) _ttoi(theDate.Mid(3,2));
    p_date->year  = (SQLUSMALLINT) _ttoi(theDate.Mid(6,4));
    return true;
  }
  if(posDate == 4 && posDat2 == 7)
  {
    // Formatted as "yyyy-mm-dd" (from a timestamp)
    p_date->year  = (SQLUSMALLINT) _ttoi(theDate.Left(4));
    p_date->month = (SQLUSMALLINT) _ttoi(theDate.Mid(5,2));
    p_date->day   = (SQLUSMALLINT) _ttoi(theDate.Mid(8,2));
    return true;
  }
  return false;
}

// Format the date
// Empty string -> Short windows notation
// "@"          -> long  windows notation
// "d MMMM yyyy <word> |H:mm:ss|"  -> Most expanded form
int
SQLVariantFormat::FormatDate(XString p_pattern)
{
  TCHAR   buffer1[NUMBER_BUFFER_SIZE + 1] = _T("");
  TCHAR   buffer2[NUMBER_BUFFER_SIZE + 1] = _T("");
  XString dateFormat;
  XString timeFormat;
  DWORD   opties = 0;
  bool    doTime = false;

  // STEP 1: Check datatypes and contents of the variant for strings
  if(m_variant && (m_variant->GetDataType() != SQL_C_CHAR)
               && (m_variant->GetDataType() != SQL_C_WCHAR))
  {
    if(!m_variant->IsDateTimeType())
    {
      // ACTUALLY AN ERROR
      // But we let it pass unnoticed to support constructions like '0'/@yyyy
      return 0;
    }
  }
  else if(!m_format.IsEmpty())
  {
    bool hasTime = p_pattern.Find('|') >= 0;
    bool hasDate = p_pattern.Left(1) != '|' || p_pattern.Right(1) != '|';
   
    DATE_STRUCT date;
    TIME_STRUCT time;
    ZeroMemory(&date,sizeof(DATE_STRUCT));
    ZeroMemory(&time,sizeof(TIME_STRUCT));

    // Is there a date present in the string?
    if (hasDate)
    {
      if(!GetDateFromStringVariant(m_variant,m_format,&date))
      {
        if(m_variant)
        {
          return ER_FormatDateTypeValue;
        }
        return OK;
      }
    }
    // Is there a time present in the string?
    if(hasTime)
    {
      // Can result in an error and '00:00:00'
      GetTimeFromStringVariant(m_variant,m_format,&time);
    }
  
    if(hasDate && hasTime)
    {
      TIMESTAMP_STRUCT timestamp;
      timestamp.year    = date.year;
      timestamp.month   = date.month;
      timestamp.day     = date.day;
      timestamp.hour    = time.hour;
      timestamp.minute  = time.minute;
      timestamp.second  = time.second;

      if(m_owner)
      {
        delete m_variant;
      }
      m_variant = new SQLVariant(&timestamp);
      m_owner=true;
    }
    else if(hasDate)
    {
      DATE_STRUCT datestr;
      datestr.year  = date.year;
      datestr.month = date.month;
      datestr.day   = date.day;
      if(m_owner)
      {
        delete m_variant;
      }
      m_variant = new SQLVariant(&datestr);
      m_owner=true;
    }
    else if(hasTime)
    {
        TIME_STRUCT timestr;
        timestr.hour   = time.hour;
        timestr.minute = time.minute;
        timestr.second = time.second;
        if(m_owner)
        {
          delete m_variant;
        }
        m_variant = new SQLVariant(&timestr);
        m_owner=true;
    }
  }
  else
  {
    // Not a Variant AND not a string
    // Can be that there is no record, or that the contents are NULL
    // Formatting remains "" (Empty string)
    return OK;
  }
  // STEP 3: Getting our options
  doTime = p_pattern.Find('|') >= 0;

  if(p_pattern.IsEmpty())
  {
    opties = DATE_SHORTDATE;
  }
  else if(p_pattern == _T("@"))
  {
    opties = DATE_LONGDATE;
    p_pattern = "";
  }
  else
  {
    // Prepare for an API call from German/Dutch/French
    p_pattern.Replace(_T("jj"),_T("yy"));
    p_pattern.Replace(_T("aa"),_T("yy"));
  }
  // STEP 4: Splitting of the patterns
  if(!p_pattern.IsEmpty())
  {
    int pos1 = p_pattern.Find('|');
    if(pos1 >= 0)
    {
      int pos2 = p_pattern.Find('|',pos1 + 1);
      dateFormat = p_pattern.Left(pos1);
      timeFormat = p_pattern.Mid(pos1+1, pos2 - pos1 - 1);
    }
    else
    {
      // date only
      dateFormat = p_pattern;
    }
  }
  // STEP 5: Processing the DATE part
  int type(SQL_C_TIMESTAMP); // Defaults to full timestamp
  if(m_variant)
  {
    type = m_variant->GetDataType();
  }
  if(type == SQL_C_DATE      || type == SQL_C_TIMESTAMP ||
     type == SQL_C_TYPE_DATE || type == SQL_C_TYPE_TIMESTAMP)
  {
    SYSTEMTIME str;
    memset(reinterpret_cast<void*>(&str),0,sizeof(SYSTEMTIME));
    const DATE_STRUCT* date = m_variant->GetAsDate();
    str.wYear  = date->year;
    str.wMonth = date->month;
    str.wDay   = date->day;
    int buflen;
    if((buflen = GetDateFormat(0,opties,&str,((opties != 0) ? nullptr : reinterpret_cast<LPCTSTR>(dateFormat.GetBuffer())),buffer1,NUMBER_BUFFER_SIZE)) < 0)
    {
      buflen = 0;
    }
    buffer1[buflen] = 0;
  }
  // STEP 6: Processing the time part
  if((type == SQL_C_TIME      || type == SQL_C_TIMESTAMP ||
      type == SQL_C_TYPE_TIME || type == SQL_C_TYPE_TIMESTAMP) 
      && (opties != DATE_SHORTDATE)
      && (opties != DATE_LONGDATE ))
  {
    SYSTEMTIME str;
    memset(reinterpret_cast<void*>(&str),0,sizeof(SYSTEMTIME));
    const TIME_STRUCT* time = m_variant->GetAsTime();
    str.wHour   = time->hour;
    str.wMinute = time->minute;
    str.wSecond = time->second;
    int buflen;
    if((buflen = GetTimeFormat(0,0,&str,(timeFormat.GetLength() > 0 ? reinterpret_cast<LPCTSTR>(timeFormat.GetBuffer()) : nullptr),buffer2,NUMBER_BUFFER_SIZE)) < 0)
    {
      buflen = 0;
    }
    buffer2[buflen] = 0;
  }
  // STEP 7: Putting date and time back together again
  m_format = XString(buffer1);
  if(doTime)
  {
    m_format += XString(buffer2);
  }
  return OK;
}

// Adding and subtracting dates
// p_operator  : '+'  -> Add days, months, years
//               '-'  -> Subtract days, months, years
//               '~'  -> Subtract two dates
// p_argument  :  n   -> Always days
//                nD  -> n days
//                nM  -> n months
//                nY  -> n years (English)
//                nJ  -> n years (Dutch / German)
//                nA  -> n years (French)
int
SQLVariantFormat::DateCalculate(char p_operator,XString p_argument)
{
  int   number = 0;
  TCHAR numberType = 'D';

  // STEP 1: Checking datatypes and the contents of the variant for strings
  //         Making sure the underlying SQLVariant is of type SQL_C_DATE
  if(m_variant && (m_variant->GetDataType() != SQL_C_CHAR && 
                   m_variant->GetDataType() != SQL_C_WCHAR))
  {
    if(! m_variant->IsDateTimeType())
    {
      return ER_FormatDateTypeValue;
    }
  }
  else if(!m_format.IsEmpty())
  {
    DATE_STRUCT date;
    if(!GetDateFromStringVariant(m_variant,m_format,&date))
    {
      if(m_variant)
      {
        return ER_FormatDateTypeValue;
      }
      return OK;
    }
    if(m_owner)
    {
      delete m_variant;
    }
    m_variant = new SQLVariant(&date);
    m_owner   = true;
  }
  else
  {
    // Not a variant and not a string
    // Default is a NULL date
    if(m_variant == nullptr)
    {
      DATE_STRUCT date = {0,0,0};
      m_variant = new SQLVariant(&date);
      m_owner   = true;
    }
    else
    {
      // It's a variant, but a string
      // Try to convert to a date
      XString date = m_variant->GetAsChar();
      m_variant->SetData(SQL_C_DATE,date);
    }
  }
  
  // Setting the default operator. Making sure we have an operator
  if(p_operator != '+' && p_operator != '-' && p_operator != '~')
  {
    p_operator = '+';
  }

  // Getting the date to work on
  SQLDate date = m_variant->GetAsSQLDate();

  if(date.Valid() && (p_operator == '+' || p_operator == '-'))
  {
    _stscanf_s(p_argument,_T("%d %c"),&number,&numberType,(unsigned int)sizeof(TCHAR));
    if(number != 0)
    {
      if(number < 0)
      {
        p_operator = (p_operator == '+' ? '-' : '+');
        number     = abs(number);
      }
      if(p_operator == '-')
      {
        number = -number;
      }
      switch(toupper(numberType))
      {
        case 'D': date = date.AddDays(number);
                  m_variant->Set(&date);
                  m_format = date.AsString();
                  break;
        case 'M': date = date.AddMonths(number);
                  m_variant->Set(&date);
                  m_format = date.AsString();
                  break;
        case 'Y': [[fallthrough]];
        case 'A': [[fallthrough]];
        case 'J': date = date.AddYears(number);
                  m_variant->Set(&date);
                  m_format = date.AsString();
                  break;
      }
    }
  }
  if(date.Valid() && p_operator == '~')
  {
    try
    {
      int days = 0;
      SQLDate other(p_argument);
      if(other.Valid())
      {
        SQLInterval intval = date - other;
        days = intval.GetDays();
      }
      else
      {
        SQLTimestamp other2(p_argument);
        if(other2.Valid())
        {
          SQLInterval intval2 = date - other2.AsSQLDate();
          days = intval2.GetDays();
        }
      }
      m_format.Format(_T("%d"),days);
    }
    catch(StdException& ex)
    {
      ReThrowSafeException(ex);
      // Does nothing. Underlying date is unchanged
    }
  }
  return OK;
}

// Formatting of a number
int
SQLVariantFormat::FormatNumber(XString p_format,bool p_currency)
{
  // Standard MS-Windows format
  XString number;

  // Freshly gotten for the underlying SQLVariant
  if(m_variant)
  {
    if(m_variant->IsNumericType())
    {
      m_variant->GetAsString(m_format);
    }
  }
  // Converting number variants. Also "123,4500000" to "123.45"
  XString error;
  bcd value = StringDecimalValue(error);
  number = value.AsString(bcd::Format::Bookkeeping,false,SQLNUM_MAX_PREC / 2);

  if(p_format.IsEmpty())
  {
    TCHAR buffer[NUMBER_BUFFER_SIZE + 1];
    int	 bufLen = 0;
    if (p_currency)
    {
      bufLen = GetCurrencyFormat(LOCALE_USER_DEFAULT,0,number,nullptr,buffer,NUMBER_BUFFER_SIZE);
    }
    else
    {
      bufLen = GetNumberFormat(LOCALE_USER_DEFAULT,0,number,nullptr,buffer,NUMBER_BUFFER_SIZE);
    }
    if (bufLen <= 0)
    {
      bufLen = 0;
    }
    buffer[bufLen] = 0;
    m_format = buffer;
  }
  else
  {
    if(p_format.Find(';') < 0)
    {
      TCHAR buffer[NUMBER_BUFFER_SIZE + 1];
      _tcsncpy_s(buffer,number,NUMBER_BUFFER_SIZE);
      FormatNumberTemplate(buffer,p_format,NUMBER_BUFFER_SIZE);
      number = buffer;
    }
    else
    {
      // NOTHING? Wat to do here?
    }
  }
  return OK;
}

XString 
SQLVariantFormat::FormatVariantForSQL(SQLDatabase* p_database)
{
  XString text;
  int datatype    = m_variant->GetDataType();
  int toCdatatype = m_variant->FindDataTypeFromSQLType();

  if(toCdatatype == SQL_C_CHAR || toCdatatype == SQL_C_WCHAR)
  {
    m_variant->GetAsString(text);
    text.Replace(_T("\'"),_T("\'\'"));
    text = _T("\'") + text + _T("\'");
    if(toCdatatype == SQL_C_WCHAR)
    {
      // NATIONAL CHAR / NATIONAL VARCHAR
      text = "N" + text;
    }
  }
  else if(m_variant->IsNumericType())
  {
    m_variant->GetAsString(text);
  }
  else if(datatype == SQL_C_TIME || datatype == SQL_C_TYPE_TIME)
  {
    SQLTime time = m_variant->GetAsSQLTime();
    text = p_database->GetSQLTimeString(time.Hour(),time.Minute(),time.Second());
  }
  else if(datatype == SQL_C_DATE || datatype == SQL_C_TYPE_DATE)
  {
    SQLDate date = m_variant->GetAsSQLDate();
    text = p_database->GetSQLDateString(date.Day(),date.Month(),date.Year());
  }
  else if(datatype == SQL_C_TIMESTAMP || datatype == SQL_C_TYPE_TIMESTAMP)
  {
    SQLTimestamp stamp = m_variant->GetAsSQLTimestamp();
    text = p_database->GetTimestampAsString(stamp);
  }
  else if(m_variant->IsIntervalType())
  {
    text = FormatVariantAsInterval(p_database);
  }
  else if(toCdatatype == SQL_C_GUID)
  {
    m_variant->GetAsString(text);
    text.TrimLeft(_T("{"));
    text.TrimRight(_T("}"));
    text = _T("{guid \'") + text + _T("\'}");
  }
  else
  {
    XString datatypeString = SQLDataType::FindDatatype(datatype);
    throw StdException(_T("Cannot prepare datatype for query: ") + datatypeString);
  }
  return text;
}

// Return the value as a SQL string for the database interval fields
XString
SQLVariantFormat::FormatVariantAsInterval(SQLDatabase* p_database)
{
  SQLInfoDB* info = p_database->GetSQLInfoDB();
  if(info->GetRDBMSSupportsDatatypeInterval())
  {
    return m_variant->GetAsSQLInterval().AsSQLString(p_database,true);
  }
  else
  {
    bcd intval = m_variant->GetAsSQLInterval().AsDatabaseNumber();
    return intval.AsString(bcd::Format::Bookkeeping,false,0);
  }
}

// Internally formatting a number according to a template
int
SQLVariantFormat::FormatNumberTemplate(LPTSTR p_number,LPCTSTR p_numFormat,int p_buflen)
{
  TCHAR  strFormat[2 * NUMBER_BUFFER_SIZE + 1];
  TCHAR  buffer   [2 * NUMBER_BUFFER_SIZE + 1];
  int    BufLen = 0;
  TCHAR* format;
  TCHAR* number;

  bool   inFormat   = false;
  bool   formatting = false;
  bool   isZero     = false;
  bool   isNegative = false;
  bool   isNull     = true;
  bool   rounding   = false;
  TCHAR  inString   = '\0';
  TCHAR  lastChar   = '\0';
  TCHAR  lastFormat = '\0';
  int    position   = 0;
  TCHAR  formString[NUMBER_BUFFER_SIZE + 1];
  TCHAR  restString[NUMBER_BUFFER_SIZE + 1];
  int    formPos = 0;
  int    RestPos = 0;

  bool   grouping        = false;  // Do we have a grouping
  int    sizeFirstGroup  = 0;      // Size of the first group
  int    sizeSecondGroup = 0;      // Size of the second group
  int    groupCounter    = 0;      // Current group counter
  int    groupSize       = 0;      // Size of the repeating group

  int    iNumberTrailing  = 0;
  int    iNumberLeading   = 0;
  int    iLeadingDigits   = 0;
  int    iLeadingZero     = 0;
  int    iTrailingDigits  = 0;
  int    iTrailingZero    = 0;
  int    iRouding         = 0;

  // Finding the format of the number
  // Remove junk and first checks
  bool   inDecimal = false;
  bool   isNumber  = false;
  bool   hasSign   = false;

  LPTSTR formatStart = p_number;
  for(format  = p_number;*format != '\0';++format)
  {
    if(!isNumber && (*format == ' ' || *format == '0' ))
    {
      continue;
    }
    if(_tcsrchr(_T("1234567890"),*format) != nullptr )
    {
      isNumber = true;
      *formatStart++ = *format;
    }
    else if(*format == '.')
    {
      if(inDecimal)
      {
        return ER_FormatNumberTemplateDecimal;
      }
      inDecimal = true;
      isNumber  = true;
      *formatStart++ = '.';
    }
    else if(*format == '-' || *format == '+')
    {
      if(!hasSign )
      {
        hasSign = true;
        isNegative = (*format == '-');
        if(isNumber)
        {
          *format = '\0';
          break;
        }
      }
      else
      {
        *format = '\0';
        break;
      }
    }
    else
    {
      *format = '\0';
      break;
    }
  }
  *formatStart = '\0';

  // Finding out the format string
  // restString[0] = '\0';
  // formString[0] = '\0';
  _tcsncpy_s(strFormat, p_numFormat, NUMBER_BUFFER_SIZE);

  inDecimal = false;
  isNumber  = false;
  for (format = strFormat ; *format != '\0' ; lastChar = *(format++) )
  {
    if (lastChar == '\\')
    {
      RestPos--;
      restString[RestPos++] = *format;
      continue;
    }
    if(inString == '\0')
    {
      if(_tcsrchr(_T("\"'"), *format) != nullptr )
      {
        inString = *format;
      }
      else if(_tcsrchr(_T("@"), *format) != nullptr )
      {
        // @# = Original number format
        // @$ = Original money  format
        if (*(format + 1) == '#')
        {
          if(GetNumberFormat(0,LOCALE_NOUSEROVERRIDE,p_number,nullptr,buffer,p_buflen) > 0)
          {
            _tcscpy_s(&restString[RestPos],NUMBER_BUFFER_SIZE,buffer);
            RestPos += (int)_tcslen(buffer);
          }
          else
          {
            return ER_FormatNumberTemplateGetNumberFormat;
          }
          format++;
        }
        if (*(format + 1) == '$')
        {
          if(GetCurrencyFormat(0,LOCALE_NOUSEROVERRIDE,p_number,nullptr,buffer,p_buflen) > 0)
          {
            _tcscpy_s(&restString[RestPos],NUMBER_BUFFER_SIZE,buffer);
            RestPos += (int)_tcslen(buffer);
          }
          else
          {
            return ER_FormatNumberTemplateGetCurrencyFormat;
          }
          format++;
        }
      }
      else if(_tcschr(_T("-+~^"), *format) != nullptr)
      {
        switch(*format)
        {
          case '^': _tcscpy_s(&restString[RestPos],10, ::g_locale_strCurrency);
                    RestPos += (int)_tcslen(::g_locale_strCurrency);
                    break;
          case '+': restString[RestPos++] = 1;
                    break;
          case '-': restString[RestPos++] = 2;
                    break;
          case '~': restString[RestPos++] = 3;
                    break;
        }
        inFormat = false;
      }
      else if(_tcschr(_T("&#0xX.:,"), *format) != nullptr)
      {
        if(!inFormat)
        {
          if(formatting)
          {
            return ER_FormatNumberTemplateFormat;
          }
          _tcscpy_s(&restString[RestPos],NUMBER_BUFFER_SIZE,_T("@*@"));
          RestPos   += 3;
          formatting = true;
          inFormat   = true;
        }
        switch (*format)
        {
          case '#':
          case '&': isNumber = true;
                    if (inDecimal)
                    {
                      iTrailingDigits++;
                    }
                    else
                    {
                      if(isZero)
                      {
                        return ER_FormatNumberTemplateZero;
                      }
                      iLeadingDigits++;
                      groupCounter++;
                    }
                    formString[formPos++] = *format;
                    break;
          case '0': isZero = true;
                    if (inDecimal)
                    {
                      if(isNumber)
                      {
                        return ER_FormatNumberTemplateNummer;
                      }
                      iTrailingZero++;
                      iTrailingDigits++;
                    }
                    else
                    {
                      iLeadingZero++;
                      iLeadingDigits++;
                      groupCounter++;
                    }
                    formString[formPos++] = *format;
                    break;
          case 'x': [[fallthrough]];
          case 'X': if (inDecimal)
                    {
                      if(rounding)
                      {
                        return ER_FormatNumberTemplateRounding;
                      }
                      rounding = true;
                      if (*format == 'x')
                      {
                        iTrailingDigits++;
                        iRouding = iTrailingDigits;
                        formString[formPos++] = *format;
                      }
                      else
                      {
                        iRouding = iTrailingDigits;
                      }
                    }
                    else
                    {
                      return ER_FormatNumberTemplateNoDecimal;
                    }
                    break;
          case '.': [[fallthrough]];
          case ':': isZero   = false;
                    isNumber = false;
                    if (inDecimal)
                    {
                      return ER_FormatNumberTemplateDecimal2;
                    }
                    inDecimal = true;
                    // Finding the grouping
                    if (grouping)
                    {
                      if (sizeFirstGroup == 0)
                      {
                        sizeFirstGroup = groupCounter;
                      }
                      else if (sizeSecondGroup == 0)
                      {
                        sizeSecondGroup = groupCounter;
                      }
                    }
                    groupCounter = 0;
                    //
                    formString[formPos++] = *format;
                    break;
          case ',': if (inDecimal)
                    {
                      return ER_FormatNumberTemplateDecimal3;
                    }
                    // Finding the grouping
                    grouping = true;
                    if (sizeFirstGroup == 0)
                    {
                      sizeFirstGroup = groupCounter;
                    }
                    else if (sizeSecondGroup == 0)
                    {
                      sizeSecondGroup = groupCounter;
                    }
                    groupCounter = 0;
                    //
                    formString[formPos++] = *format;
                    break;
          case '"':
          case '\'':inString = *format;
                    break;
          default : restString[RestPos++] = *format;
                    break;
        }
      }
      else
      {
        restString[RestPos++] = *format;
      }
    }
    else
    {
      if(*format == inString)
      {
        inString = '\0';
      }
      else
      {
        restString[RestPos++] = *format;
      }
    }
  }
  // Eventually the last group
  if(!inDecimal && grouping)
  {
    if(sizeFirstGroup == 0)
    {
      sizeFirstGroup = groupCounter;
    }
    else if(sizeSecondGroup == 0)
    {
      sizeSecondGroup = groupCounter;
    }
  }
  if(grouping)
  {
    groupSize = max(sizeFirstGroup, sizeSecondGroup);
  }
  // Closing the strings
  restString[RestPos] = '\0';
  formString[formPos] = '\0';

  // Do the rounding if necessary
  if(rounding)
  {
    NUMBERFMT NumFormat;
    NumFormat.NumDigits     = iRouding;
    NumFormat.LeadingZero   = 0;
    NumFormat.Grouping      = 0;
    NumFormat.lpDecimalSep  = (LPTSTR)_T(".");
    NumFormat.lpThousandSep = (LPTSTR)_T(",");
    NumFormat.NegativeOrder = 0;
    BufLen = GetNumberFormat(0, 0, p_number, &NumFormat, buffer, p_buflen);
    if (BufLen <= 0)
    {
      return ER_FormatNumberTemplateBuflen;
    }
    _tcscpy_s(p_number,p_buflen,buffer);
  }
  //
  // Getting the attributes of the number
  //
  inDecimal = false;
  for(format = p_number ; *format != '\0' ;++format)
  {
    if(_tcsrchr(_T("0123456789"),*format) != nullptr)
    {
      if(inDecimal)
      {
        iNumberTrailing++;
      }
      else
      {
        iNumberLeading++;
      }
    }
    else if(*format == '.')
    {
      inDecimal = true;
    }
    else
    {
      *format = '\0';
      break;
    }
  }
  *formatStart = '\0';
  if(inDecimal && iNumberTrailing > 0 )
  {
    while(iNumberTrailing > 0)
    {
      if(*(formatStart - 1) == '0' )
      {
        formatStart--;
        *formatStart = '\0';
        iNumberTrailing--;
      }
      else
      {
        break;
      }
    }
  }
  // Adjusting the number size and format string size
  if(iNumberLeading > iLeadingDigits)
  {
    // Adding '#' to the format string
    XString NewFormat;
    position = 0;
    groupCounter = sizeFirstGroup;
    for(int count = 0 ; count < (iNumberLeading - iLeadingDigits) ; count++ )
    {
      if(grouping)
      {
        if(groupCounter == groupSize)
        {
          NewFormat = ',' + NewFormat;
          groupCounter = 0;
        }
        NewFormat = '#' + NewFormat;
        groupCounter++;
      }
      else
      {
        NewFormat += '#';
      }
    }
    NewFormat += formString;
    _tcscpy_s(formString, NUMBER_BUFFER_SIZE, (LPCTSTR)NewFormat);
  }
  else if(iNumberLeading < iLeadingDigits)
  {
    XString NewGetal;
    for(int count = 0 ; count < (iLeadingDigits - iNumberLeading) ; count++)
    {
      NewGetal += '0';
    }
    NewGetal += p_number;
    _tcscpy_s(p_number, p_buflen,(LPCTSTR)NewGetal);
  }

  // Formatting the number
  position   = 0;
  RestPos    = 0;
  isNumber   = false;
  lastFormat = '#';
  for(format = formString, number = p_number ; *format != '\0' ; format++)
  {
    switch(*format)
    {
      case 'X' :  break;
      case 'x' :  *format = lastFormat;
      case '#':   [[fallthrough]];
      case '&':   [[fallthrough]];
      case '0' :  lastFormat = *format;
                  if((lastChar = *number) == '\0')
                  {
                    lastChar = '0';
                  }
                  else
                  {
                    ++number;
                  }
                  if(isdigit(lastChar))
                  {
                    if(*format != '0')
                    {
                      isNull = false;
                    }
                    if(!isNumber && lastChar == '0')
                    {
                      if(*format == '#' )
                      {
                        buffer[position++] = ' ';
                      }
                      else if(*format != '&' )
                      {
                        buffer[position++] = '0';
                        isNumber = true;
                      }
                    }
                    else
                    {
                      buffer[position++] = lastChar;
                      isNumber = true;
                    }
                  }
                  else
                  {
                    return ER_FormatNumberTemplateNoDigit;
                  }
                  break;
      case ',' :  if(isNumber)
                  {
                    _tcscpy_s(&buffer[position],NUMBER_BUFFER_SIZE,::g_locale_thousandSep);
                    position += (int)_tcslen(::g_locale_thousandSep);
                  }
                  else
                  {
                    if(lastFormat != '&')
                    {
                      buffer[position++] = ' ';
                    }
                  }
                  break;
      case '.' :  [[fallthrough]];
      case ':' :  if(*number != '.' && *number != '\0')
                  {
                    return ER_FormatNumberTemplateSomeKindOfError ;
                  }
                  if(*number)
                  {
                    number++;
                  }
                  if(iTrailingDigits > 0)
                  {
                    _tcscpy_s(&buffer[position], NUMBER_BUFFER_SIZE,::g_locale_decimalSep);
                    position += (int)_tcslen(::g_locale_decimalSep);
                  }
                  if(*format == ':')
                  {
                    position = 0;
                  }
                  break;
    }
  }
  buffer[position] = '\0';
  XString string = restString;
  string.Replace(_T("@*@"),buffer);
  // The placing of the '+' or '-' sign
  TCHAR tPlus[2]  = {1 , 0};
  TCHAR tMin[2]   = {2 , 0};
  TCHAR tTilde[2] = {3 , 0};
  if(isNull)
  {
    string.Replace(tPlus, _T(" "));
    string.Replace(tMin,  _T(" "));
    string.Replace(tTilde,_T(""));
  }
  else if(isNegative)
  {
    string.Replace(tPlus, _T("-"));
    string.Replace(tMin,  _T("-"));
    string.Replace(tTilde,_T("-"));
  }
  else
  {
    string.Replace(tPlus, _T(" "));
    string.Replace(tMin,  _T("+"));
    string.Replace(tTilde,_T(""));
  }
  m_format = string;
  return OK;
}

// End of namespace
}
