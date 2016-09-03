////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantFormat.cpp
//
// Copyright (c) 1998- 2014 ir. W.E. Huisman
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
// Last Revision:   01-01-2015
// Version number:  1.1.0
//
#include "stdafx.h"
#include "SQLVariantFormat.h"
#include "SQLFormatErrors.h"
#include "SQLTime.h"
#include "SQLDate.h"
#include "SQLTimestamp.h"
#include <locale.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning (disable: 4482) // Non standard enum used

// string format number en valuta format functies
static bool ValutaInit = false;
static char DecimalSep[11];
static char ThousandSep[11];
static char strCurrency[11];
static int  DecimalSepLen;
static int  ThousandSepLen;
static int  strCurrencyLen;

static void 
InitValutaString()
{
  if(ValutaInit == false)
  {
    GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL,  DecimalSep,  10);
    GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_STHOUSAND, ThousandSep, 10);
    GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SCURRENCY, strCurrency, 10);
    DecimalSepLen  = (int)strlen(DecimalSep);
    ThousandSepLen = (int)strlen(ThousandSep);
    strCurrencyLen = (int)strlen(strCurrency);

    setlocale(LC_NUMERIC, "C");
    ValutaInit = true;
  }
}

SQLVariantFormat::SQLVariantFormat(SQLVariant* p_variant)
                 :m_variant(p_variant)
                 ,m_userStatus(0)
                 ,m_owner(false)
{
  InitValutaString();
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

// Alle woorden een eerste hoofdletter
void
SQLVariantFormat::StringInitCapital()
{
  bool doCapital = true;
  for(int ind = 0;ind < m_format.GetLength(); ++ind)
  {
    int ch = m_format.GetAt(ind);
    if(ch > 0 && ch <= 127)
    {
      if(isalpha(ch))
      {
        if(doCapital)
        {
          m_format.SetAt(ind,(unsigned char)toupper(ch));
          doCapital = false;
        }
        else
        {
          m_format.SetAt(ind,(unsigned char)tolower(ch));
        }
      }
      else
      {
        doCapital = true;
      }
    }
  }
}

// Eerste letter een hoofdletter van de hele string
void
SQLVariantFormat::StringStartCapital()
{
  bool doCapital = true;
  for(int ind = 0;ind < m_format.GetLength(); ++ind)
  {
    int ch = m_format.GetAt(ind);
    if(ch > 0 && ch <= 127)
    {
      if(isalpha(ch))
      {
        if(doCapital)
        {
          m_format.SetAt(ind,(char)toupper(ch));
          doCapital = false;
        }
        else
        {
          m_format.SetAt(ind,(char)tolower(ch));
        }
      }
    }
  }
}

bool
SQLVariantFormat::IsConstanteOfNummer(char p_separator)
{
  bool bDecimal  = false;
  bool bSpace    = false;
  bool bTeken    = false;
  bool bNumber   = false;
  CString string = m_format;

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
    else if (ch == p_separator)
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
    // Volgend teken
    ++pos;
  }
  return true;
}

int 
SQLVariantFormat::StrValutaNLOmzetten(CString& p_string,bool p_enkelValuta)
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
SQLVariantFormat::StrValutaAMOmzetten(CString& p_string,bool p_enkelValuta)
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
SQLVariantFormat::IsWinNumber(const CString p_string,CString* p_newNumber)
{
  return IsWinNumber(p_string,DecimalSep,ThousandSep,strCurrency,p_newNumber);
}

// Is string a (formatted) windows number?
bool
SQLVariantFormat::IsWinNumber(const CString p_string
                             ,char*         p_decSeperator
                             ,char*         p_thouSeperator
                             ,char*         p_valuta
                             ,CString*      p_newNumber)
{
  CString newGetal("+");
  bool bDecimal  = false;   // decimal seperator found
  bool bThousend = false;   // thousand seperator found
  bool bSpace    = false;   // Spaces behind the number
  bool bTeken    = false;   // Found a sign
  bool bNaTeken  = false;   // Sign after the number
  bool bNumber   = false;   // First digit found
  bool bNegatief = false;   // Negative number
  int  pString   = 0;

  int DSLen = 0;
  int TSLen = 0;
  int VALen = 0;

  InitValutaString();

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
SQLVariantFormat::StringDoubleWaarde()
{
  double result = 0.0;
  InitValutaString();

  if(IsConstanteOfNummer())
  {
    result = atof(m_format);
  }
  else
  {
    CString waarde = m_format;
    if(IsConstanteOfNummer(','))
    {
      StrValutaNLOmzetten(waarde,false);
      result = atof(waarde);
    }
    else
    {
      CString newGetal;
      if(IsWinNumber(waarde,DecimalSep,ThousandSep,strCurrency,&newGetal))
      {
        result = atof(newGetal);
      }
      else
      {
        CString newWaarde = waarde;
        StrValutaNLOmzetten(newWaarde,true);
        if(IsWinNumber(newWaarde,DecimalSep,ThousandSep,strCurrency,&newGetal))
        {
          result = atof(newGetal);
        }
        else
        {
          if(IsWinNumber(newWaarde,",",".",strCurrency,&newGetal))
          {
            result = atof(newGetal);
          }
          else
          {
            if(IsWinNumber(newWaarde,".",",",strCurrency,&newGetal))
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

// Zet huidige datum en tijd
void
SQLVariantFormat::SetCurrentDate()
{
  CTime tm  = CTime::GetCurrentTime();
  m_format  = tm.Format("%Y-%m-%d %H:%M:%S");
  m_owner   = true;
  m_variant = new SQLVariant();
  m_variant->SetData(SQL_C_TIMESTAMP,m_format);
}

#pragma warning (disable: 4996)
bool
SQLVariantFormat::GetTimeFromStringVariant(SQLVariant* p_variant,CString p_format,TIME_STRUCT* p_time)
{
  ZeroMemory(p_time,sizeof(TIME_STRUCT));
  //CString tijd("");
  
  CString tijd(p_variant ? p_variant->GetAsChar() : p_format);

  int pos = tijd.Find(':');
  if(pos < 0)
  {
    return false;
  }

  //Is er een tijd in de string aanwezig
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
  if(sscanf(tijd,"%d:%d:%d",&uur,&min,&sec) == 3)
  {
    p_time->hour   = (SQLUSMALLINT) uur;
    p_time->minute = (SQLUSMALLINT) min;
    p_time->second = (SQLUSMALLINT) sec;

    return true;
  }
  return false;
}
#pragma warning (error:4996)

bool
SQLVariantFormat::GetDateFromStringVariant(SQLVariant* p_variant,CString p_format,DATE_STRUCT* p_date)
{
  ZeroMemory(p_date,sizeof(DATE_STRUCT));
  CString datum(p_variant ? p_variant->GetAsChar() : p_format);

  // Om amerikaanse datum formaten voor te zijn.
  datum.Replace("/","-");

  int posDate = datum.Find('-');
  if(posDate < 0)
  {
    return false;
  }
  int posDat2 = datum.Find('-',posDate + 1);
  if(posDat2 < 0)
  {
    try
    {
      SQLDate lang(datum);
      p_date->day   = (SQLUSMALLINT)lang.Day();
      p_date->month = (SQLUSMALLINT)lang.Month();
      p_date->year  = (SQLUSMALLINT)lang.Year();
      return true;
    }
    catch(...)
    {
    }
    return false;
  }
  if(posDate == 2 && posDat2 == 5)
  {
    p_date->day   = (SQLUSMALLINT) atoi(datum.Left(2));
    p_date->month = (SQLUSMALLINT) atoi(datum.Mid(3,2));
    p_date->year  = (SQLUSMALLINT) atoi(datum.Mid(6,4));
    return true;
  }
  if(posDate == 4 && posDat2 == 7)
  {
    p_date->year  = (SQLUSMALLINT) atoi(datum.Left(4));
    p_date->month = (SQLUSMALLINT) atoi(datum.Mid(5,2));
    p_date->day   = (SQLUSMALLINT) atoi(datum.Mid(8,2));
    return true;
  }
  return false;
}


// Format the date
// Lege string -> Korte windows notatie
// "@"         -> Lange windows notatie
// "d MMMM yyyy om |H:mm:ss|"  -> meest uitgebreide variant
int
SQLVariantFormat::FormatDate(CString p_pattern)
{
  char    buffer1[100] = "";
  char    buffer2[100] = "";
  CString dateFormat;
  CString timeFormat;
  DWORD   opties = 0;
  bool    doTime = false;

  // STAP 1: Controle datatypen en inhoud variant of string
  if(m_variant && m_variant->GetDataType() != SQL_C_CHAR)
  {
    if(! m_variant->IsDateTimeType())
    {
      // IS EIGENLIJK FOUT
      // return ER_FormatDateTypeWaarde;
      // We laten m_format ongemoeid om constructies als '0'/@jjjj te ondersteunen
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

    //Is er een datum in de string aanwezig ?
    if (hasDate)
    {
      if(m_variant == NULL || m_variant->IsEmpty())
      {
        return OK;
      }
      if(!GetDateFromStringVariant(m_variant,m_format,&date))
      {
        return ER_FormatDateTypeValue;
      }
    }
    //Is er een tijd in de string aanwezig
    if(hasTime)
    {
      // Mag fout en dus '00:00:00' opleveren
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
    // Geen variant EN geen string
    // Kan zijn dat er geen record is of inhoud NULL is
    // Formattering blijft "" (lege string
    return OK;
  }
  // STAP 3: Opties bepalen
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
    // Prepareren voor API call
    p_pattern.Replace("jj","yy");
  }
  // STAP 4: Splitsen van patronen
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
      // Alleen een datum
      dateFormat = p_pattern;
    }
  }
  // STAP 5: Verwerken datum deel
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
    if((buflen = GetDateFormat(0,opties,&str, ((opties != 0) ? NULL : (LPCTSTR)dateFormat) , buffer1, 100)) < 0)
    {
      buflen = 0;
    }
    buffer1[buflen] = 0;
  }
  // STAP 6: Verwerken tijd deel
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
    if((buflen = GetTimeFormat(0,0,&str,(timeFormat.GetLength() > 0 ? (LPCTSTR)timeFormat : NULL),buffer2,100)) < 0)
    {
      buflen = 0;
    }
    buffer2[buflen] = 0;
  }
  // STAP 7: Datum en tijd weer achter elkaar
  m_format = CString(buffer1);
  if(doTime)
  {
    m_format += CString(buffer2);
  }
  return OK;
}

// Data optellen/aftrekken
int
SQLVariantFormat::DateCalculate(char p_bewerking,CString p_argument)
{
  int Getal;
  int Tel;
  int Incr;
  int Aantal;
  char GetalType;

  // STAP 1: Controle datatypen en inhoud variant of string
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
      return ER_FormatDateTypeValue;
    }
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
    // STAP 2: String naar DATE/TIME/TIMESTAMP
    //m_variant = new SQLVariant();
    //m_owner   = true;

   // m_variant->SetData(SQL_C_DATE,m_format);
  }
  else
  {
    // Geen variant EN geen string
    // RFC 406801: Default is NULL datum
    if(m_variant == NULL)
    {
      DATE_STRUCT date = {0,0,0};
      m_variant = new SQLVariant(&date);
      m_owner   = true;
    }
    else
    {
      // Wel variant, maar ander type, omvormen
      m_variant->SetData(SQL_C_DATE,"");
    }
  }
  
  DATE_STRUCT* date = m_variant->GetAsDate();
  COleDateTime     dDatum(date->year,date->month,date->day,0,0,0);
  COleDateTimeSpan dTimeSpan;

  Getal = 0;
  GetalType = 'D';
  if(p_bewerking != '+' && p_bewerking != '-' && p_bewerking != '~')
  {
    p_bewerking = '+';
  }
  if((dDatum.GetStatus() == COleDateTime::DateTimeStatus::valid) &&
     (p_bewerking == '+' || p_bewerking == '-'))
  {

    Aantal = sscanf_s(p_argument, "%d %c", &Getal, &GetalType,(unsigned int)sizeof(char));

    //SetDateTimeSpan( long lDays, int nHours, int nMins, int nSecs );
    if (Getal != 0 )
    {
      if (Getal < 0 )
      {
        p_bewerking = ( p_bewerking == '+' ? '-' : '+' );
        Getal = abs(Getal);
      }
      GetalType = (unsigned char)toupper(GetalType);

      switch (GetalType)
      {
      case 'D': dTimeSpan.SetDateTimeSpan(Getal, 0, 0, 0);
                if (p_bewerking == '+' )
                {
                  dDatum += dTimeSpan;
                }
                else
                {
                  dDatum -= dTimeSpan;
                }
                date->year  = (SQLSMALLINT)  dDatum.GetYear();
                date->month = (SQLUSMALLINT) dDatum.GetMonth();
                date->day   = (SQLUSMALLINT) dDatum.GetDay();
                m_variant->GetAsString(m_format);
                break;
      case 'M': Incr = ( p_bewerking == '+' ? 1 : -1 );
                for ( Tel = Getal ; Tel > 0 ; Tel--)
                {
                  date->month += (SQLUSMALLINT) Incr;
                  if(date->month > 12)
                  {
                    date->month = 1;
                    date->year += 1;
                  }
                  else if(date->month <= 0)
                  {
                    date->month = 12;
                    date->year -= 1;
                  }
                }
                m_variant->GetAsString(m_format);
                break;
      case 'Y':
      case 'J': if (p_bewerking == '+' )
                {
                  date->year += (SQLSMALLINT)Getal;
                }
                else
                {
                  if(date->year < Getal)
                  {
                    date->year = 0;
                  }
                  else
                  {
                    date->year -= (SQLUSMALLINT)Getal;
                  }
                }
                m_variant->GetAsString(m_format);
                break;
      }
    }
  }
  if(p_bewerking == '~')
  {
    double dagen;
    COleDateTime DT;
    // RFC 406801: Default is 0 dagen
    m_format = "0";
    if((dDatum.GetStatus() == COleDateTime::DateTimeStatus::valid) && 
       DT.ParseDateTime(p_argument))
    {
      dagen = DT - dDatum;
      m_format.Format("%i", (int)dagen);
    }
  }
  return OK;

}

// Formatteer volgens BRIEF
int
SQLVariantFormat::FormatNumber(CString p_format,bool p_currency)
{
  // standaard windows opmaak
  CString getal;

  // Eventueel vers ophalen uit de SQLVariant
  if(m_variant)
  {
    if(m_variant->IsNumericType())
    {
      m_variant->GetAsString(m_format);
    }
  }
  // Getal varianten omzetten. Zet ook "123,4500000" om naar "123.45"
  double waarde = StringDoubleWaarde();
  getal.Format("%f",waarde);

  if(p_format.IsEmpty())
  {
    char buffer[250];
    int	 bufLen = 0;
    if (p_currency)
    {
      bufLen = GetCurrencyFormat(LOCALE_USER_DEFAULT,0,getal,NULL,buffer,250);
    }
    else
    {
      bufLen = GetNumberFormat(LOCALE_USER_DEFAULT,0,getal,NULL,buffer,250);
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
      char buffer[101];
      strncpy_s(buffer,getal,100);
      FormatNumberTemplate(buffer,p_format,100);
      getal = buffer;
    }
    else
    {
      // NIETS? Wat doen we hier?
    }
  }
  return OK;
}

CString 
SQLVariantFormat::FormatVariantForSQL(SQLDatabase* p_database)
{
  CString text;
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
    CString datatypeString = m_variant->FindDatatype(datatype);
    throw CString("Cannot prepare datatype for query: " + datatypeString);
  }
  return text;
}

// Intern formatteren van een getal volgens BRIEF
int
SQLVariantFormat::FormatNumberTemplate(char *Getal,const char *strNumFormat,int p_buflen)
{
  char	strFormat[200];
  char	Buffer[250];
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
  char	FmtString[100];
  int	  FmtPos = 0;
  char	RestString[100];
  int	  RestPos = 0;

  BOOL	bGrouping     = FALSE;	// Wordt er grouping toegepast
  int	  nEersteGroup  = 0;		  // Grote van de eerste groep
  int	  nTweedeGroup  = 0;		  // grote van de tweede groep
  int	  nGroup        = 0;			// Huidige groep teller
  int	  nGroupSize    = 0;			// Grote van de repeating group

  int	  iGetalTrailing  = 0;
  int	  iGetalLeading   = 0;
  int	  iLeadingDigits  = 0;
  int	  iLeadingZero    = 0;
  int	  iTrailingDigits = 0;
  int	  iTrailingZero   = 0;
  int	  iAfronden       = 0;

  // opmaken en uitpluizen van het getal
  // Rommel verwijderen en de eerste controle
  bInDecimal = FALSE;
  bNummer    = FALSE;
  bSign      = FALSE;
  for (pFormatStart = Getal , pFormat = Getal ; *pFormat != '\0' ; *pFormat++)
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

  // uitpluizen van de format string
  // RestString[0] = '\0';
  // FmtString[0] = '\0';
  strncpy_s(strFormat, strNumFormat, 100);

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
        // @# = Oorspronkelijke Nummer opmaak
        // @$ = Oorspronkelijke Valuta opmaak
        if (*(pFormat + 1) == '#')
        {
          if(GetNumberFormat(0,LOCALE_NOUSEROVERRIDE, Getal, NULL, Buffer, p_buflen) > 0)
          {
            strcpy_s(&RestString[RestPos], 100,Buffer);
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
            strcpy_s(&RestString[RestPos], 100, Buffer);
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
          case '^': strcpy_s(&RestString[RestPos],10, strCurrency);
                    RestPos += (int)strlen(strCurrency);
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
          strcpy_s(&RestString[RestPos],100, "@*@");
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
                    // groep vaststellen
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
                    // groep vaststellen
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
    { // instring
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
  // eventueel de laatste groep
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
  // strings afsluiten
  RestString[RestPos] = '\0';
  FmtString[FmtPos] = '\0';

  /* @plaats opschonen getal */
  // Eventueel afronden indien noodzakelijk
  if (	bAfronden )
  {
    //strFormatLog += "[AFRONDEN DIE HAP]\r\n";
    NUMBERFMT NumFormat;
    NumFormat.NumDigits = iAfronden;
    NumFormat.LeadingZero = 0;
    NumFormat.Grouping = 0;
    NumFormat.lpDecimalSep = ".";
    NumFormat.lpThousandSep = ",";
    NumFormat.NegativeOrder = 0;
    BufLen = GetNumberFormat(0, 0, Getal, &NumFormat, Buffer, p_buflen);
    if (BufLen <= 0)
    {
      return ER_FormatNumberTemplateBuflen;
    }
    strcpy_s(Getal,p_buflen,Buffer);
  }
  //
  // eigenschappen van het getal bepalen
  //
  bInDecimal = FALSE;
  for (pFormat = Getal ; *pFormat != '\0' ; *pFormat++)
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
  // Getal en formaat string op elkaar aanpassen
  // Getal grote als de format string
  if (iGetalLeading > iLeadingDigits)
  {
    // Aantal '#' toevoegen aan format string
    CString NewFormat;
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
    strcpy_s(FmtString, 100, (LPCSTR)NewFormat);
  }
  else if (iGetalLeading < iLeadingDigits)
  {
    CString NewGetal;
    for (Tel = 0 ; Tel < (iLeadingDigits - iGetalLeading) ; Tel++ )
    {
      NewGetal += '0';
    }
    NewGetal += Getal;
    strcpy_s(Getal, p_buflen,(LPCSTR)NewGetal);
  }

  // Het Getal Formateren
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
//                       else if (*pFormat == '&' )
//                       {
//                         Pos = Pos;
//                       }
//                       else
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
                    strcpy_s(&Buffer[Pos], 250, ThousandSep);
                    Pos += (int)strlen(ThousandSep);
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
                    strcpy_s(&Buffer[Pos], 250, DecimalSep);
                    Pos += (int)strlen(DecimalSep);
                  }
                  if (*pFormat == ':')
                  {
                    Pos = 0;
                  }
                  break;
    }
  }
  Buffer[Pos] = '\0';
  CString string = RestString;
  string.Replace("@*@",Buffer);
  // Teken Plaatsen
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
    string.Replace(tPlus,"-");
    string.Replace(tMin, "-");
    string.Replace(tTilde,"-");
  }
  else
  {
    string.Replace(tPlus," ");
    string.Replace(tMin, "-");
    string.Replace(tTilde,"");
  }
  m_format = string;
  return OK;
}
