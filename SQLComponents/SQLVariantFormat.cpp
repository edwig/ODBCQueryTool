////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantFormat.cpp
//
// Copyright (c) 1998-2022 ir. W.E. Huisman
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
#include "SQLFormatErrors.h"
#include "SQLTime.h"
#include "SQLDate.h"
#include "SQLTimestamp.h"
#include "bcd.h"
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
  m_variant = NULL;
}

void
SQLVariantFormat::Reset()
{
  if(m_owner && m_variant)
  {
    delete m_variant;
  }
  m_variant    = NULL;
  m_userStatus = 0;
  m_format     = "";
}

void
SQLVariantFormat::ResetValue()
{
  if(m_owner && m_variant)
  {
    delete m_variant;
  }
  m_variant = NULL;
  m_format  = "";
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
  int  length = m_format.GetLength();
  unsigned char* buffer = (unsigned char*)m_format.GetBuffer();

  for(int ind = 0; ind < length; ++ind)
  {
    int ch = buffer[ind];

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
          m_format.SetAt(ind, (unsigned char)toupper(ch));
          doCapital = false;
        }
        else
        {
          m_format.SetAt(ind, (unsigned char)tolower(ch));
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
  int  length = m_format.GetLength();
  unsigned char* buffer = (unsigned char*)m_format.GetBuffer();

  for(int ind = 0;ind < length; ++ind)
  {
    int ch = buffer[ind];

    if(isblank(ch) == false)
    {
      if(isalpha(ch))
      {
        m_format.SetAt(ind,doCapital ? (unsigned char)toupper(ch) : (unsigned char) tolower(ch));
      }
      doCapital = false;
    }
  }
}

// The format is a constant or a simple number
bool
SQLVariantFormat::IsConstantOrNumber(char p_seperator /*='.'*/)
{
  bool bDecimal  = false;
  bool bSpace    = false;
  bool bTeken    = false;
  bool bNumber   = false;
  XString string = m_format;

  string.Trim();
  int pos = 0;

  while(pos < string.GetLength())
  {
    int ch = string.GetAt(pos);
    if(isdigit(ch))
    {
      if(bSpace)
      {
        return false;
      }
      bNumber = true;
    }
    else if (ch == '-' || ch == '+')
    {
      if(bNumber || bSpace || bDecimal || bTeken)
      {
        return false;
      }
      bTeken = true;
    }
    else if (ch == p_seperator)
    {
      if(bSpace || bDecimal)
      {
        return false;
      }
      bDecimal = true;
    }
    else if(isspace(ch))
    {
      bSpace = true;
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
SQLVariantFormat::StrValutaNLOmzetten(XString& p_string,bool p_enkelValuta)
{
  p_string.Replace(" "   ,"");
  p_string.Replace("EUR.","");
  p_string.Replace("eur.","");
  p_string.Replace("E."  ,"");
  p_string.Replace("E"   ,"");

  if(! p_enkelValuta)
  {
    p_string.Replace(".","");
    p_string.Replace(",",".");
  }
  return p_string.GetLength();
}

int 
SQLVariantFormat::StrValutaAMOmzetten(XString& p_string,bool p_enkelValuta)
{
  p_string.Replace(" " ,"");
  p_string.Replace("$.","");
  p_string.Replace("$" ,"");

  if(! p_enkelValuta)
  {
    p_string.Replace(",","");
    p_string.Replace(".",",");
  }
  return p_string.GetLength();
}

// Is string a (formatted) windows number?
bool
SQLVariantFormat::IsWinNumber(const XString p_string
                             ,char*         p_decSeperator
                             ,char*         p_thouSeperator
                             ,char*         p_valuta
                             ,XString*      p_newNumber)
{
  XString newGetal("+");
  bool bDecimal  = false;   // decimal separator found
  bool bThousend = false;   // thousand separator found
  bool bSpace    = false;   // Spaces behind the number
  bool bTeken    = false;   // Found a sign
  bool bNaTeken  = false;   // Sign after the number
  bool bNumber   = false;   // First digit found
  bool bNegatief = false;   // Negative number
  int  pString   = 0;

  int DSLen = 0;
  int TSLen = 0;
  int VALen = 0;

  ::InitValutaString();

  if(p_decSeperator != NULL)
  {
    DSLen = (int)strlen(p_decSeperator);
  }
  if(p_thouSeperator != NULL)
  {
    TSLen = (int)strlen(p_thouSeperator);
  }
  if(p_valuta != NULL)
  {
    VALen = (int)strlen(p_valuta);
  }
  while(isspace(p_string.GetAt(pString)))
  {
    ++pString;
  }
  while(pString < p_string.GetLength())
  {
    if(isdigit(p_string.GetAt(pString)))
    {
      if(bSpace || bNaTeken)
      {
        return false;
      }
      bNumber = true;
      newGetal += p_string.GetAt(pString++);
    }
    else if(p_string.GetAt(pString) == '-' ||
            p_string.GetAt(pString) == '+' )
    {
      if(bTeken)
      {
        return false;
      }
      bTeken    = true;
      bNaTeken  = bNumber;
      bNegatief = (p_string.GetAt(pString) == '-');
      newGetal.SetAt(0,p_string.GetAt(pString++));
    }
    else if(DSLen > 0 && strncmp(p_string.Mid(pString),p_decSeperator,DSLen) == 0)
    {
      if(bSpace || bDecimal)
      {
        return false;
      }
      if(!bNumber)
      {
        newGetal += '0';
      }
      newGetal += '.';
      bNumber   = true;
      bDecimal  = true;
      pString  += DSLen;
    }
    else if(TSLen > 0 && strncmp(p_string.Mid(pString),p_thouSeperator,TSLen) == 0)
    {
      if(bSpace || bDecimal)
      {
        return false;
      }
      pString += TSLen;
      bThousend = true;
    }
    else if(VALen > 0 && strncmp(p_string.Mid(pString),p_valuta,VALen) == 0)
    {
      pString += VALen;
    }
    else if(isspace(p_string.GetAt(pString)))
    {
      if(bNumber || bDecimal)
      {
        bSpace = true;
      }
      ++pString;
    }
    else
    {
      return false;
    }
  }

  // Eventually place extra 0 after a number ending in a decimal point
  if(newGetal.Right(1) == '.')
  {
    newGetal += '0';
  }
  if(bNumber && p_newNumber != NULL)
  {
    *p_newNumber = newGetal;
  }
  return true;
}

double
SQLVariantFormat::StringDoubleValue()
{
  double result = 0.0;
  ::InitValutaString();

  if(IsConstantOrNumber())
  {
    result = atof(m_format);
  }
  else
  {
    XString waarde = m_format;
    if(IsConstantOrNumber(','))
    {
      StrValutaNLOmzetten(waarde,false);
      result = atof(waarde);
    }
    else
    {
      XString newGetal;
      if(IsWinNumber(waarde,::g_locale_decimalSep,::g_locale_thousandSep,::g_locale_strCurrency,&newGetal))
      {
        result = atof(newGetal);
      }
      else
      {
        XString newWaarde = waarde;
        StrValutaNLOmzetten(newWaarde,true);
        if(IsWinNumber(newWaarde,::g_locale_decimalSep,::g_locale_thousandSep,::g_locale_strCurrency,&newGetal))
        {
          result = atof(newGetal);
        }
        else
        {
          if(IsWinNumber(newWaarde,(char*)",",(char*)".",::g_locale_strCurrency,&newGetal))
          {
            result = atof(newGetal);
          }
          else
          {
            if(IsWinNumber(newWaarde,(char*)".",(char*)",",::g_locale_strCurrency,&newGetal))
            {
              result = atof(newGetal);
            }
            else
            {
              // Cross our fingers and hope to die!!
              result = atof(waarde);
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
SQLVariantFormat::SetCurrentDate()
{
  // Making place for our own variant
  if(m_variant && m_owner)
  {
    delete m_variant;
    m_variant = nullptr;
  }

  // Setting the current timestamp in the format string
  // Does so in standard order "Y-M-D H:M:S" 
  SQLTimestamp stamp = SQLTimestamp::CurrentTimestamp();
  m_format = stamp.AsXMLString();
  m_format.Replace("T"," ");

  // Getting a new variant
  m_variant = new SQLVariant(&stamp);
  m_owner   = true;
}

bool
SQLVariantFormat::GetTimeFromStringVariant(SQLVariant* p_variant,XString p_format,TIME_STRUCT* p_time)
{
  ZeroMemory(p_time,sizeof(TIME_STRUCT));
  //XString tijd("");
  
  XString tijd(p_variant ? XString(p_variant->GetAsChar()) : p_format);

  int pos = tijd.Find(':');
  if(pos < 0)
  {
    return false;
  }

  // Is there a time present int he string?
  if(pos > 0 && isdigit(tijd.GetAt(pos - 1)))
  {
    --pos;
    if(pos > 0 && isdigit(tijd.GetAt(pos - 1)))
    {
      --pos;
    }
  }
  tijd = tijd.Mid(pos);
  int uur = 0;
  int min = 0;
  int sec = 0;
  if(sscanf_s(tijd,"%d:%d:%d",&uur,&min,&sec) == 3)
  {
    p_time->hour   = (SQLUSMALLINT) uur;
    p_time->minute = (SQLUSMALLINT) min;
    p_time->second = (SQLUSMALLINT) sec;

    return true;
  }
  return false;
}

bool
SQLVariantFormat::GetDateFromStringVariant(SQLVariant* p_variant,XString p_format,DATE_STRUCT* p_date)
{
  ZeroMemory(p_date,sizeof(DATE_STRUCT));

  XString datum = p_variant ? p_variant->GetAsChar() : "";
  if(datum.IsEmpty() && !p_format.IsEmpty())
  {
    datum = p_format;
  }

  // To fix the American date formats
  datum.Replace("/","-");

  // Let SQLDate catch formats like "dd" "ddmm" and "ddmmyy[yy]"
  // And all special cases where we do "NOW +1 DAY" etc
  int posDate = datum.Find('-');
  if(posDate < 0)
  {
    try
    {
      SQLDate lang(datum);
      lang.AsDateStruct(p_date);
      return true;
    }
    catch(StdException& er)
    {
      ReThrowSafeException(er);
    }
    return false;
  }
  int posDat2 = datum.Find('-',posDate + 1);
  if(posDat2 < 0)
  {
    return false;
  }
  if(posDate == 2 && posDat2 == 5)
  {
    // Formatted as "dd-mm-yy[yy]" (from a date)
    p_date->day   = (SQLUSMALLINT) atoi(datum.Left(2));
    p_date->month = (SQLUSMALLINT) atoi(datum.Mid(3,2));
    p_date->year  = (SQLUSMALLINT) atoi(datum.Mid(6,4));
    return true;
  }
  if(posDate == 4 && posDat2 == 7)
  {
    // Formatted as "yyyy-mm-dd" (from a timestamp)
    p_date->year  = (SQLUSMALLINT) atoi(datum.Left(4));
    p_date->month = (SQLUSMALLINT) atoi(datum.Mid(5,2));
    p_date->day   = (SQLUSMALLINT) atoi(datum.Mid(8,2));
    return true;
  }
  return false;
}

// Format the date
// Empty string -> Short windows notation
// "@"          -> long  windows notation
// "d MMMM yyyy om |H:mm:ss|"  -> Most expanded form
int
SQLVariantFormat::FormatDate(XString p_pattern)
{
  char    buffer1[NUMBER_BUFFER_SIZE + 1] = "";
  char    buffer2[NUMBER_BUFFER_SIZE + 1] = "";
  XString dateFormat;
  XString timeFormat;
  DWORD   opties = 0;
  bool    doTime = false;

  // STEP 1: Check datatypes and contents of the variant for strings
  if(m_variant && m_variant->GetDataType() != SQL_C_CHAR)
  {
    if(! m_variant->IsDateTimeType())
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
      datestr.year = date.year;
      datestr.month = date.month;
      datestr.day = date.day;
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
        timestr.hour = time.hour;
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
  else if(p_pattern == "@")
  {
    opties = DATE_LONGDATE;
    p_pattern = "";
  }
  else
  {
    // Prepare for an API call
    p_pattern.Replace("jj","yy");
  }
  // STEP 4: Splitting of the patterns
  if(!p_pattern.IsEmpty())
  {
    int pos1 = p_pattern.Find('|');
    int pos2 = -1;
    if(pos1 >= 0)
    {
      pos2 = p_pattern.Find('|',pos1 + 1);
    }
    if(pos1 >= 0)
    {
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
  int type = m_variant->GetDataType();
  if(type == SQL_C_DATE      || type == SQL_C_TIMESTAMP ||
     type == SQL_C_TYPE_DATE || type == SQL_C_TYPE_TIMESTAMP)
  {
    SYSTEMTIME str;
    memset((void*)&str,0,sizeof(SYSTEMTIME));
    DATE_STRUCT* date = m_variant->GetAsDate();
    str.wYear  = date->year;
    str.wMonth = date->month;
    str.wDay   = date->day;
    int buflen;
    if((buflen = GetDateFormat(0,opties,&str, ((opties != 0) ? NULL : (LPCTSTR)dateFormat) , buffer1, NUMBER_BUFFER_SIZE)) < 0)
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
    memset((void*)&str,0,sizeof(SYSTEMTIME));
    TIME_STRUCT* time = m_variant->GetAsTime();
    str.wHour   = time->hour;
    str.wMinute = time->minute;
    str.wSecond = time->second;
    int buflen;
    if((buflen = GetTimeFormat(0,0,&str,(timeFormat.GetLength() > 0 ? (LPCTSTR)timeFormat : NULL),buffer2,NUMBER_BUFFER_SIZE)) < 0)
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
//                nY  -> n years (english)
//                nJ  -> n years (dutch / german)
//                nA  -> n years (french)
int
SQLVariantFormat::DateCalculate(char p_operator,XString p_argument)
{
  int  number = 0;
  int  amount = 0;
  char numberType = 'D';

  // STEP 1: Checking datatypes and the contents of the variant for strings
  //         Making sure the underlying SQLVariant is of type SQL_C_DATE
  if(m_variant && m_variant->GetDataType() != SQL_C_CHAR)
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
    if(m_variant == NULL)
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
    amount = sscanf_s(p_argument,"%d %c",&number,&numberType,(unsigned int)sizeof(char));
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
      switch (toupper(numberType))
      {
        case 'D': date = date.AddDays(number);
                  date.AsDateStruct(m_variant->GetAsDate());
                  m_format = date.AsString();
                  break;
        case 'M': date = date.AddMonths(number);
                  date.AsDateStruct(m_variant->GetAsDate());
                  m_format = date.AsString();
                  break;
        case 'Y': // Fall through
        case 'J': date = date.AddYears(number);
                  date.AsDateStruct(m_variant->GetAsDate());
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
      m_format.Format("%d",days);
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
  double value = StringDoubleValue();
  number.Format("%.6f",value);

  if(p_format.IsEmpty())
  {
    char buffer[NUMBER_BUFFER_SIZE + 1];
    int	 bufLen = 0;
    if (p_currency)
    {
      bufLen = GetCurrencyFormat(LOCALE_USER_DEFAULT,0,number,NULL,buffer,NUMBER_BUFFER_SIZE);
    }
    else
    {
      bufLen = GetNumberFormat(LOCALE_USER_DEFAULT,0,number,NULL,buffer,NUMBER_BUFFER_SIZE);
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
      char buffer[NUMBER_BUFFER_SIZE + 1];
      strncpy_s(buffer,number,NUMBER_BUFFER_SIZE);
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

  if(toCdatatype == SQL_C_CHAR)
  {
    m_variant->GetAsString(text);
    text.Replace("\'","\'\'");
    text = "\'" + text + "\'";
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
//   else if(m_variant->IsIntervalType())
//   {
//     text = FormatVariantAsInterval(p_rdbmsType);
//   }
  else if(toCdatatype == SQL_C_GUID)
  {
    m_variant->GetAsString(text);
    text.TrimLeft("{");
    text.TrimRight("}");
    text = "{guid \'" + text + "\'}";
  }
  else
  {
    XString datatypeString = m_variant->FindDatatype(datatype);
    throw StdException("Cannot prepare datatype for query: " + datatypeString);
  }
  return text;
}

// Internally formatting a number according to a template
int
SQLVariantFormat::FormatNumberTemplate(char *Getal,const char *strNumFormat,int p_buflen)
{
  char	strFormat[2 * NUMBER_BUFFER_SIZE + 1];
  char	Buffer   [2 * NUMBER_BUFFER_SIZE + 1];
  int	  BufLen = 0;
  char	*pFormat;
  char	*pGetal;
  char	*pFormatStart;

  BOOL	bInFormat  = FALSE;
  BOOL	bFormat    = FALSE;
  BOOL	bInDecimal = FALSE;
  BOOL	bZero      = FALSE;
  BOOL	bNummer    = FALSE;
  BOOL	bNegatief  = FALSE;
  BOOL	bNul       = TRUE;
  BOOL	bAfronden  = FALSE;
  BOOL	bSign      = FALSE;
  char	InString   = '\0';
  char	LastChar   = '\0';
  char	LastOpmaak = '\0';
  int	  Tel;
  int	  Pos;
  char	FmtString [NUMBER_BUFFER_SIZE + 1];
  char	RestString[NUMBER_BUFFER_SIZE + 1];
  int	  FmtPos  = 0;
  int	  RestPos = 0;

  BOOL	bGrouping     = FALSE;	// Do we have a grouping
  int	  nEersteGroup  = 0;		  // Size of the first group
  int	  nTweedeGroup  = 0;		  // Size of the second group
  int	  nGroup        = 0;			// Current group counter
  int	  nGroupSize    = 0;			// Size of the repeating group

  int	  iGetalTrailing  = 0;
  int	  iGetalLeading   = 0;
  int	  iLeadingDigits  = 0;
  int	  iLeadingZero    = 0;
  int	  iTrailingDigits = 0;
  int	  iTrailingZero   = 0;
  int	  iAfronden       = 0;

  // Finding the format of the number
  // Remove junk and first checks
  bInDecimal = FALSE;
  bNummer    = FALSE;
  bSign      = FALSE;

  pFormatStart = Getal;
  for(pFormat = Getal;*pFormat != '\0';++pFormat)
  {
    if (!bNummer && (*pFormat == ' ' || *pFormat == '0' ))
    {
      continue;
    }
    if (strchr("1234567890", *pFormat) != NULL )
    {
      bNummer = TRUE;
      *pFormatStart++ = *pFormat;
    }
    else if (*pFormat == '.')
    {
      if (bInDecimal)
      {
        return ER_FormatNumberTemplateDecimal;
      }
      bInDecimal = TRUE;
      bNummer    = TRUE;
      *pFormatStart++ = '.';
    }
    else if (*pFormat == '-' || *pFormat == '+')
    {
      if ( !bSign )
      {
        bSign = TRUE;
        bNegatief = (*pFormat == '-');
        if (bNummer)
        {
          *pFormat = '\0';
          break;
        }
      }
      else
      {
        *pFormat = '\0';
        break;
      }
    }
    else
    {
      *pFormat = '\0';
      break;
    }
  }
  *pFormatStart = '\0';

  // Finding out the format string
  // RestString[0] = '\0';
  // FmtString[0] = '\0';
  strncpy_s(strFormat, strNumFormat, NUMBER_BUFFER_SIZE);

  bInDecimal = FALSE;
  bNummer    = FALSE;
  for (pFormat = strFormat ; *pFormat != '\0' ; LastChar = *(pFormat++) )
  {
    if (LastChar == '\\')
    {
      RestPos--;
      RestString[RestPos++] = *pFormat;
      continue;
    }
    if (InString == '\0')
    {
      if (strchr("\"'", *pFormat) != NULL )
      {
        InString = *pFormat;
      }
      else if (strchr("@", *pFormat) != NULL )
      {
        // @# = Original number format
        // @$ = Original money  format
        if (*(pFormat + 1) == '#')
        {
          if(GetNumberFormat(0,LOCALE_NOUSEROVERRIDE, Getal, NULL, Buffer, p_buflen) > 0)
          {
            strcpy_s(&RestString[RestPos], NUMBER_BUFFER_SIZE,Buffer);
            RestPos += (int)strlen(Buffer);
          }
          else
          {
            return ER_FormatNumberTemplateGetNumberFormat;
          }
          pFormat++;
        }
        if (*(pFormat + 1) == '$')
        {
          if(GetCurrencyFormat(0, LOCALE_NOUSEROVERRIDE, Getal, NULL, Buffer, p_buflen) > 0)
          {
            strcpy_s(&RestString[RestPos], NUMBER_BUFFER_SIZE, Buffer);
            RestPos += (int)strlen(Buffer);
          }
          else
          {
            return ER_FormatNumberTemplateGetCurrencyFormat;
          }
          pFormat++;
        }
      }
      else if (strchr("-+~^", *pFormat) != NULL )
      {
        switch (*pFormat)
        {
          case '^': strcpy_s(&RestString[RestPos],10, ::g_locale_strCurrency);
                    RestPos += (int)strlen(::g_locale_strCurrency);
                    break;
          case '+': RestString[RestPos++] = 1;
                    break;
          case '-': RestString[RestPos++] = 2;
                    break;
          case '~': RestString[RestPos++] = 3;
                    break;
        }
        bInFormat = FALSE;
      }
      else if (strchr("&#0xX.:,", *pFormat) != NULL )
      {
        if (! bInFormat)
        {
          if (bFormat)
          {
            return ER_FormatNumberTemplateFormat;
          }
          strcpy_s(&RestString[RestPos],NUMBER_BUFFER_SIZE, "@*@");
          RestPos += 3;
          bFormat = TRUE;
          bInFormat = TRUE;
        }
        switch (*pFormat)
        {
          case '#':
          case '&': bNummer = TRUE;
                    if (bInDecimal)
                    {
                      iTrailingDigits++;
                    }
                    else
                    {
                      if (bZero)
                        return ER_FormatNumberTemplateZero;
                      iLeadingDigits++;
                      nGroup++;
                    }
                    FmtString[FmtPos++] = *pFormat;
                    break;
          case '0': bZero = TRUE;
                    if (bInDecimal)
                    {
                      if (bNummer)
                        return ER_FormatNumberTemplateNummer ;
                      iTrailingZero++;
                      iTrailingDigits++;
                    }
                    else
                    {
                      iLeadingZero++;
                      iLeadingDigits++;
                      nGroup++;
                    }
                    FmtString[FmtPos++] = *pFormat;
                    break;
          case 'x':
          case 'X': if (bInDecimal)
                    {
                      if (bAfronden)
                      {
                        return ER_FormatNumberTemplateRounding;
                      }
                      bAfronden = TRUE;
                      if (*pFormat == 'x')
                      {
                        iTrailingDigits++;
                        iAfronden = iTrailingDigits;
                        FmtString[FmtPos++] = *pFormat;
                      }
                      else
                      {
                        iAfronden = iTrailingDigits;
                      }
                    }
                    else
                    {
                      return ER_FormatNumberTemplateNoDecimal;
                    }
                    break;
          case '.':
          case ':': bZero = FALSE;
                    bNummer = FALSE;
                    if (bInDecimal)
                    {
                      return ER_FormatNumberTemplateDecimal2;
                    }
                    bInDecimal = TRUE;
                    // Finding the grouping
                    if (bGrouping)
                    {
                      if (nEersteGroup == 0)
                      {
                        nEersteGroup = nGroup;
                      }
                      else if (nTweedeGroup == 0)
                      {
                        nTweedeGroup = nGroup;
                      }
                    }
                    nGroup = 0;
                    //
                    FmtString[FmtPos++] = *pFormat;
                    break;
          case ',': if (bInDecimal)
                    {
                      return ER_FormatNumberTemplateDecimal3;
                    }
                    // Finding the grouping
                    bGrouping = TRUE;
                    if (nEersteGroup == 0)
                    {
                      nEersteGroup = nGroup;
                    }
                    else if (nTweedeGroup == 0)
                    {
                      nTweedeGroup = nGroup;
                    }
                    nGroup = 0;
                    //
                    FmtString[FmtPos++] = *pFormat;
                    break;
          case '"':
          case '\'':InString = *pFormat;
                    break;
          default : RestString[RestPos++] = *pFormat;
                    break;
        }
      }
      else
      {
        RestString[RestPos++] = *pFormat;
      }
    }
    else
    {
      if (*pFormat == InString)
      {
        InString = '\0';
      }
      else
      {
        RestString[RestPos++] = *pFormat;
      }
    }
  }
  // Eventually the last group
  if (!bInDecimal && bGrouping)
  {
    if (nEersteGroup == 0)
    {
      nEersteGroup = nGroup;
    }
    else if (nTweedeGroup == 0)
    {
      nTweedeGroup = nGroup;
    }
  }
  if (bGrouping)
  {
    nGroupSize = max(nEersteGroup, nTweedeGroup);
  }
  // Closing the strings
  RestString[RestPos] = '\0';
  FmtString[FmtPos] = '\0';

  // Do the rounding if necessary
  if(bAfronden)
  {
    NUMBERFMT NumFormat;
    NumFormat.NumDigits     = iAfronden;
    NumFormat.LeadingZero   = 0;
    NumFormat.Grouping      = 0;
    NumFormat.lpDecimalSep  = (LPSTR)".";
    NumFormat.lpThousandSep = (LPSTR)",";
    NumFormat.NegativeOrder = 0;
    BufLen = GetNumberFormat(0, 0, Getal, &NumFormat, Buffer, p_buflen);
    if (BufLen <= 0)
    {
      return ER_FormatNumberTemplateBuflen;
    }
    strcpy_s(Getal,p_buflen,Buffer);
  }
  //
  // Getting the attributes of the number
  //
  bInDecimal = FALSE;
  for(pFormat = Getal ; *pFormat != '\0' ;++pFormat)
  {
    if (strchr("0123456789", *pFormat) != NULL )
    {
      if (bInDecimal)
      {
        iGetalTrailing++;
      }
      else
      {
        iGetalLeading++;
      }
    }
    else if (*pFormat == '.')
    {
      bInDecimal = TRUE;
    }
    else
    {
      *pFormat = '\0';
      break;
    }
  }
  *pFormatStart = '\0';
  if (bInDecimal && iGetalTrailing > 0 )
  {
    while (iGetalTrailing > 0)
    {
      if (*(pFormatStart - 1) == '0' )
      {
        pFormatStart--;
        *pFormatStart = '\0';
        iGetalTrailing--;
      }
      else
      {
        break;
      }
    }
  }
  // Adjusting the number size and format string size
  if (iGetalLeading > iLeadingDigits)
  {
    // Adding '#' to the format string
    XString NewFormat;
    Pos = 0;
    nGroup = nEersteGroup;
    for (Tel = 0 ; Tel < (iGetalLeading - iLeadingDigits) ; Tel++ )
    {
      if (bGrouping)
      {
        if (nGroup == nGroupSize)
        {
          NewFormat = ',' + NewFormat;
          nGroup = 0;
        }
        NewFormat = '#' + NewFormat;
        nGroup++;
      }
      else
      {
        NewFormat += '#';
      }
    }
    NewFormat += FmtString;
    strcpy_s(FmtString, NUMBER_BUFFER_SIZE, (LPCSTR)NewFormat);
  }
  else if (iGetalLeading < iLeadingDigits)
  {
    XString NewGetal;
    for (Tel = 0 ; Tel < (iLeadingDigits - iGetalLeading) ; Tel++ )
    {
      NewGetal += '0';
    }
    NewGetal += Getal;
    strcpy_s(Getal, p_buflen,(LPCSTR)NewGetal);
  }

  // Formatting the number
  Pos = 0;
  RestPos = 0;
  bNummer = FALSE;
  LastOpmaak = '#';
  for (pFormat = FmtString, pGetal = Getal ; *pFormat != '\0' ; pFormat++)
  {
    switch (*pFormat)
    {
      case 'X' : break;
      case 'x' : *pFormat = LastOpmaak;
      case '#' :
      case '&' :
      case '0' :  LastOpmaak = *pFormat;
                  if ( (LastChar = *pGetal) == '\0' )
                    LastChar = '0';
                  else
                    pGetal++;
                  if (isdigit(LastChar))
                  {
                    if (*pFormat != '0')
                    {
                      bNul = FALSE;
                    }
                    if (!bNummer && LastChar == '0')
                    {
                      if (*pFormat == '#' )
                      {
                        Buffer[Pos++] = ' ';
                      }
                      else if (*pFormat != '&' )
                      {
                        Buffer[Pos++] = '0';
                        bNummer = TRUE;
                      }
                    }
                    else
                    {
                      Buffer[Pos++] = LastChar;
                      bNummer = TRUE;
                    }
                  }
                  else
                  {
                    return ER_FormatNumberTemplateNoDigit;
                  }
                  break;
      case ',' :  if (bNummer)
                  {
                    strcpy_s(&Buffer[Pos], NUMBER_BUFFER_SIZE, ::g_locale_thousandSep);
                    Pos += (int)strlen(::g_locale_thousandSep);
                  }
                  else
                  {
                    if (LastOpmaak != '&')
                      Buffer[Pos++] = ' ';
                  }
                  break;
      case '.' :
      case ':' :  if (*pGetal != '.' && *pGetal != '\0')
                  {
                    return ER_FormatNumberTemplateSomeKindOfError ;
                  }
                  if(*pGetal)
                  {
                    pGetal++;
                  }
                  if (iTrailingDigits > 0)
                  {
                    strcpy_s(&Buffer[Pos], NUMBER_BUFFER_SIZE,::g_locale_decimalSep);
                    Pos += (int)strlen(::g_locale_decimalSep);
                  }
                  if (*pFormat == ':')
                  {
                    Pos = 0;
                  }
                  break;
    }
  }
  Buffer[Pos] = '\0';
  XString string = RestString;
  string.Replace("@*@",Buffer);
  // The placing of the '+' or '-' sign
  char tPlus[2]  = {1 , 0};
  char tMin[2]   = {2 , 0};
  char tTilde[2] = {3 , 0};
  if (bNul)
  {
    string.Replace(tPlus," ");
    string.Replace(tMin, " ");
    string.Replace(tTilde,"");
  }
  else if (bNegatief)
  {
    string.Replace(tPlus, "-");
    string.Replace(tMin,  "-");
    string.Replace(tTilde,"-");
  }
  else
  {
    string.Replace(tPlus," ");
    string.Replace(tMin, "+");
    string.Replace(tTilde,"");
  }
  m_format = string;
  return OK;
}

// End of namespace
}
