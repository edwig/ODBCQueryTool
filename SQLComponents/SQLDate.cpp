////////////////////////////////////////////////////////////////////////
//
// File: SQLDate.h
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
#include "Stdafx.h"
#include "SQLComponents.h"
#include "SQLDate.h"  
#include "SQLLanguage.h"
#include <windows.h>
#include <oleauto.h>
#include <cmath>
#include <ctime>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{

//////////////////////////////////////////////////////////////////////////////////////////
//
// SQLDate::SQLDate
// Default date that can be stored in all databases
// Biggest constraint here is INFORMIX, that does not winds further back
// than the date of 30 december 1899
//
SQLDate::SQLDate()
{
  SetNull();
}

// SQLDate::SQLDate
SQLDate::SQLDate(const SQLDate& p_date)
{
  SetDate(p_date.Year(),p_date.Month(),p_date.Day());
}

// SQLDate from day/month/year
SQLDate::SQLDate(const long p_day,const long p_month,const long p_year)
{
  SetDate(p_year,p_month,p_day);
}

// SQLDate from dateValue
SQLDate::SQLDate(DateValue p_value)
{
  m_mjd = p_value;
  MJDtoDate();  
}

// SQLDate made from a string
SQLDate::SQLDate(const XString& p_string)
{
  SetDate(p_string);
}

// Date made from a SQLTimestamp
SQLDate::SQLDate(const SQLTimestamp& p_timestamp)
{
  if(p_timestamp.IsNull())
  {
    SetNull();
  }
  else
  {
    SetDate(p_timestamp.Year(),p_timestamp.Month(),p_timestamp.Day());
  }
}

// SQLDate made from a DATE_STRUCT
SQLDate::SQLDate(const DATE_STRUCT* p_date)
{
  SetDate((int)p_date->year,(int)p_date->month,(int)p_date->day);
}

// SQLDate made from a COleDateTime value
SQLDate::SQLDate(double p_oledatetime)
{
  m_mjd = ((long)p_oledatetime) + OLEDATE_MJD_SHIFT;
  MJDtoDate();
}

// DTOR SQLDate
SQLDate::~SQLDate()
{
  // Nothing to do here
}

// Set to logical NULL (not filled in the database)
void
SQLDate::SetNull()
{
  m_mjd = -1;
  m_date.m_year  = 0;
  m_date.m_month = 0;
  m_date.m_day   = 0;
}

// SQLDate::SetDate
// Set the date year-month-day in a date instance
bool
SQLDate::SetDate(long p_year, long p_month, long p_day)
{
  m_date.m_year  = (short) p_year;
  m_date.m_month = (char)  p_month;
  m_date.m_day   = (char)  p_day;
  return SetMJD();
}

// SQLDate::SetDate
// Set the date value from a string in the date instance
// String must have the format 'yyyy-mm-dd'
bool 
SQLDate::SetDate(const XString& string)
{
  return CalculateDate(string);
}

// Calucalate the DateValue m_mjd for a date
bool
SQLDate::SetMJD()
{
  int year  = m_date.m_year;
  int month = m_date.m_month;
  int day   = m_date.m_day;

  // Validate year, month and day by the ODBC definition
  if (year  > 0 && year  < 10000 &&
      month > 0 && month < 13    &&
      day   > 0 && day   < 32)
  {
    // Check on the Gregorian definition of months
    // Checks on leapyears and gets the days-in-the-month
    bool leapyear = ((year & 3) == 0) &&
                    ((year % 100) != 0 || (year % 400) == 0);

    long daysInMonth = g_daysInTheMonth[month] - g_daysInTheMonth[month-1] +
                       ((leapyear && day == 29 && month == 2) ? 1 : 0);

    // Finish validating the date
    if (day <= daysInMonth)
    {
      // Calculate Astronomical Modified Juliaanse Day Nummer (MJD)
      // Method P.D-Smith: Practical Astronomy
      // Pagi 9: Paragraaf 4: Julian day numbers.
      // See alsoo:
      // Robin.M. Green: Spherical Astronomy, page 250 and next.
      if(month < 3)
      {
        month += 12;
        --year;
      }
      long gregorianB = 0;
      long factorC, factorD;
      if(year > 1582)
      {
        long gregorianA = 0;
        gregorianA = year / 100;
        gregorianB = 2 - gregorianA + (gregorianA / 4);
      }
      factorC = (long)(365.25  * (double) year);
      factorD = (long)(30.6001 * (double)(month + 1));
      // The correction factor (Modified JD) 
      // Falls on 16 november 1858 12:00 hours (noon), 
      // so subtract 679006 (17 november 1858 00:00:00 hour)
      m_mjd = gregorianB + factorC + factorD + day - 679006;
      return true;
    }
  }
  // Preset NULL value
  m_mjd = -1;
  return false;
}

// Calculate the MJD (Modified Julian Day) back to the 'real' Gregorian Calendar
void
SQLDate::MJDtoDate()
{
  long gregorianB = 0;
  long factorC = 0;
  long factorD = 0;
  long factorE = 0;
  long factorG = 0;
  // Calculate Civil Day from the Modified Juliaanse Day Nummer (MJD)
  // Method P.D-Smith: Practical Astronomy
  // Page 11: Paragraph 5: Converting Julian day number to the calendar date
  // See alsoo Robin M. Green: Spherical Astronomy, page 250 and next

  // Correction factor is MJD (2,400,000.5) + 0.5 (17 nov 1858 instead of 16 nov 12:00 hours)
  double JD = m_mjd + JULIAN_DAY_MODIFIED;
  if(JD > 2299160)
  {
    long gregorianA = 0;
    gregorianA = (long)((JD - 1867216.25) / 36524.25);
    gregorianB = (long)(JD + 1 + gregorianA - (gregorianA / 4));
  }
  else
  {
    gregorianB = (long) JD;
  }
  factorC = gregorianB + 1524;
  factorD = (long) (((double)factorC - 122.1) / 365.25);
  factorE = (long) ((double)factorD * 365.25);
  factorG = (long) (((double)(factorC - factorE)) / 30.6001);
  m_date.m_day   = (char)   (factorC - factorE - (int)((double)factorG * 30.6001));
  m_date.m_month = (char)  ((factorG > 13) ? factorG - 13 : factorG - 1);
  m_date.m_year  = (short) ((m_date.m_month > 2) ? factorD - 4716 : factorD - 4715);
}

// Gets the number of days in the month
inline long
SQLDate::DaysInMonth() const
{
  if(Valid())
  {
    long year  = m_date.m_year;
    long month = m_date.m_month;
    bool leapyear = ((year & 3) == 0) &&                       // if (year & 3) == 0 then it is divisible by 4
                    ((year % 100) != 0 || (year % 400) == 0);  // through binary comparison

    long daysInMonth = g_daysInTheMonth[month  ] - 
                       g_daysInTheMonth[month-1] +
                       ((leapyear && month == 2) ? 1 : 0);
    return daysInMonth;
  }
  return 0;
}

// Return as general European string
XString 
SQLDate::AsString() const
{
  XString buffer;
  if(IsNull() == false)
  {
    buffer.Format("%02ld-%02ld-%04ld", Day(), Month(), Year());
  }
  return buffer;
}

XString
SQLDate::AsXMLString() const
{
  XString buffer;
  if (IsNull() == false)
  {
    buffer.Format("%04d-%02d-%02d",Year(),Month(),Day());
  }
  return buffer;
}


XString     
SQLDate::AsSQLString(SQLDatabase* p_database)
{
  if(IsNull())
  {
    return "NULL";
  }
  return p_database->GetSQLDateString(Day(),Month(),Year()); 
}

XString
SQLDate::AsStrippedSQLString(SQLDatabase* /*p_database*/)
{
  XString string;
  string.Format("%04d-%02d-%02d",Year(),Month(),Day());
  return string;
}

void
SQLDate::AsDateStruct(DATE_STRUCT* p_date)
{
  p_date->day   = (SQLUSMALLINT) Day();
  p_date->month = (SQLUSMALLINT) Month();
  p_date->year  = (SQLSMALLINT)  Year();
}

// Today's date
inline SQLDate
SQLDate::Today()
{
  _tzset();
  time_t ltime;
  time(&ltime);
  struct tm now;
  localtime_s(&now,&ltime);

  // Return as SQLDate object
  return SQLDate(now.tm_mday,now.tm_mon + 1,now.tm_year + 1900);
}

// Returns the name of the day of the week
XString
SQLDate::WeekDayName(Language p_lang /*=LN_DEFAULT*/) const 
{ 
  if(Valid())
  {
    // Based on 0 (= sunday)
    int weekday = (m_mjd + 3) % 7;

    if(p_lang == LN_DEFAULT)
    {
      p_lang = g_defaultLanguage;
    }
    if(p_lang >= LN_DUTCH && p_lang <= LN_FRENCH)
    {
      return g_weekdays[p_lang][weekday];
    }
  }
  return "";
}

// Returns the name of the month
XString
SQLDate::MonthName(Language p_lang /*=LN_DEFAULT*/) const 
{ 
  if(Valid())
  {
    if (m_date.m_month >= 1 && m_date.m_month <= 12)
    {
      if(p_lang == LN_DEFAULT)
      {
        p_lang = g_defaultLanguage;
      }
      if(p_lang >= LN_DUTCH && p_lang <= LN_FRENCH)
      {
        return g_monthnames[p_lang][m_date.m_month - 1];
      }
    }
  }
  return "";
}

// Full date name in writing
// <weekdayname> <daynumber> <monthname> <year>
XString
SQLDate::FullDate(Language p_lang /*=LN_DEFAULT*/) const
{
  XString fullName;

  if(Valid())
  {
    if(p_lang == LN_DEFAULT)
    {
      p_lang = g_defaultLanguage;
    }
    if(p_lang >= LN_DUTCH && p_lang <= LN_FRENCH)
    {
      fullName.Format("%s %ld %s %ld",WeekDayName(p_lang).GetString(),Day(),MonthName(p_lang).GetString(),Year());
    }
  }
  return fullName;
}

// Gets the number of the week
int     
SQLDate::WeekNumber()  const
{
  if(Valid())
  {
    // Algorithme by: Tommy Skaue (CodeProject) / Peter-Paul Koch
    // ISO8601 Weeknumber (monday is the beginning and 1)
    // Two equivalent definitions
    // 1) Week in which 4th of january falls is week 1, OR
    // 2) Week where the first thursday of the year falls is week 1
    long a  = (14 - m_date.m_month) / 12;
    long y  = m_date.m_year + 4800 - a;
    long m  = m_date.m_month + (12 * a) - 3;
    // Beware JD is misleading. In fact: Gregoriaanse day
    long jd = m_date.m_day + (long)(((153*m)+2)/5) +
              (365 * y)   + (long)(y / 4) - (long)(y / 100) +
              (long)(y / 400) - 32045;

    long d4   = (jd + 31741 - (jd % 7)) % 146097 % 36524 % 1461;
    long l    = d4 / 1460;
    long d1   = ((d4-l) % 365) + l;
    long week = (d1 / 7) + 1;
    return week;
  }
  return 0;
}

// Static function
// Check that a numeric string has only digits
/*static*/ bool
SQLDate::IsNumericString(const XString& p_string)
{
  for(int ind = 0; ind < p_string.GetLength(); ++ind)
  {
    if(!isdigit(p_string.GetAt(ind)))
    {
      return false;
    }
  }
  return true;
}

// Bepaal datum is veranderd in het zetten
// van de datum van het huidige object i.p.v. een datum teruggeven
bool
SQLDate::CalculateDate(const XString& p_datum)
{
  XString datum(p_datum);
  bool    success = false;

  // Remove spaces at both ends
  datum.Trim();

  if(datum.IsEmpty())
  {
    SetNull();
    return true;
  }

  // Test if we are properly initialized
  SQLComponentsInitialized();

  // Speed optimalization: only if alpha-chars found
  // with numbers: go straight to the date-parser
  if(isalpha(datum.GetAt(0)))
  {
    XString     sign;
    XString     extraTime;
    XString     currentDate;
    DateStorage temp;
    int         interval = 0;

    // Static split-string-date
    SplitStrDate(datum, currentDate, sign, extraTime, interval);

    // Search for special days
    if (currentDate.CompareNoCase(g_dateNames[g_defaultLanguage][DN_TODAY])   == 0 ||
        currentDate.CompareNoCase(g_dateNames[g_defaultLanguage][DN_CURRENT]) == 0 ||
        currentDate.CompareNoCase(g_dateNames[g_defaultLanguage][DN_NOW])     == 0 )
    {
      if (GetVirtualDate(sign, extraTime, interval, temp))
      {
        SetDate(temp.m_year,temp.m_month,temp.m_day);
        success = true;
      }
    }
    else if (currentDate.CompareNoCase(g_dateNames[g_defaultLanguage][DN_FOM]) == 0)
    {
      // EersteVanMaand FirstOfMonth
      SQLDate nu(Today());
      SetDate(nu.Year(),nu.Month(),1);
      success = true;
    }
    else if (currentDate.CompareNoCase(g_dateNames[g_defaultLanguage][DN_LOM]) == 0)
    {
      // EersteVanMaand / FirstOfMonth
      SQLDate nu(Today());
      SetDate(nu.Year(),nu.Month(),nu.DaysInMonth());
      success = true;
    }
    else if(currentDate.CompareNoCase(g_dateNames[g_defaultLanguage][DN_FOY]) == 0 ||
            currentDate.CompareNoCase(g_dateNames[g_defaultLanguage][DN_CY])  == 0 )
    {
      // EersteVanJaar / FirstOfYear / HuidigJaar / CurrentYear
      SetDate(Today().Year(),1,1);
      success = true;
    }
    else if (currentDate.CompareNoCase(g_dateNames[g_defaultLanguage][DN_LOY]) == 0)
    {
      // LaatsteVanJaar / LastOfYear
      SetDate(Today().Year(),12,31);
      success = true;
    }
  }
  if(!success)
  {
    int jaar  = 0;
    int maand = 0;
    int dag   = 0;

    if(ShortDate(datum,jaar,maand,dag))
    {
      SetDate(jaar,maand,dag);
      success = true;
    }
  }
  if(!success)
  {
    SQLTimestamp mom;
    if(ParseXMLDate(datum,mom))
    {
      *this = mom.AsSQLDate();
      success = true;
    }
  }
  if(!success)
  {
    int jaar  = 0;
    int maand = 0;
    int dag   = 0;

    if(NamedDate(datum,jaar,maand,dag))
    {
      SetDate(jaar,maand,dag);
      success = true;
    }
  }
  if(!success)
  {
    int jaar  = 0;
    int maand = 0;
    int dag   = 0;
    if(!ParseDate(datum,&jaar,&maand,&dag))
    {
      // Wrong formatted date
      throw StdException("Date has a wrong format");
    }
    // Special case: Some applications do this
    // 00-00-00 -> Becomes today
    if (dag == 0 && maand == 0)
    {
      *this = Today();
      success = true;
    }
    else
    {
      // Do a SetDate on the year/month/day values
      // and does a check on the correctness of the date
      success = SetDate(jaar, maand, dag);
    }
  }
  return success;
}

// Short date for some interfaces
bool
SQLDate::ShortDate(const XString& p_date,int& p_year,int& p_month,int& p_day)
{
  bool success = false;
  // What we expect to see
  // d        -> daynumber in the current month of the current year
  // dd       -> daynumber in the current month of the current year
  // ddmm     -> daynumber/monthnumber in the current year
  // ddmmjj   -> daynumber/monthnumber/yearnumber in the current century or the last century (1900-1999)
  // ddmmjjjj -> daynumber/monthnumber/yearnumber in the gregorian calendar.
  unsigned len = p_date.GetLength();
  if((len == 1 || len == 2 || len == 4 || len == 6 || len == 8) && IsNumericString(p_date))
  {
    SQLDate vandaag(Today());
    int   eeuw = 20;
    switch(len)
    {
      case 1: // Fall through
      case 2: p_year  = vandaag.Year();
              p_month = vandaag.Month();
              p_day   = atoi(p_date);
              success = true;
              break;
      case 4: p_year  = vandaag.Year();
              p_month = atoi(p_date.Mid(2,2));
              p_day   = atoi(p_date.Left(2));
              success = true;
              break;
      case 6: p_year = atoi(p_date.Mid(4,2));
              if(p_year > 50)
              {
                eeuw = 19;
              }
              p_year += eeuw * 100;
              p_month = atoi(p_date.Mid(2,2));
              p_day   = atoi(p_date.Left(2));
              success = true;
              break;
      case 8: p_year  = atoi(p_date.Mid(4,4));
              p_month = atoi(p_date.Mid(2,2));
              p_day   = atoi(p_date.Left(2));
              success = true;
              break;
    }
  }
  return success;
}

// Get the virtual date as in (+/- <number> <YEAR(S)/MONTH(S)/DAY(S)/WEEK(S)>)
bool 
SQLDate::GetVirtualDate(XString       p_sign,
                        XString       p_extraTime,
                        long          p_interval,
                        DateStorage&  p_temp)
{       
  SQLDate dt;
  SQLDate mom = SQLDate::Today();

  if(!p_sign.IsEmpty())
  {
    p_extraTime.MakeUpper();
    if(p_sign != "-" && p_sign != "+")
    {
      return false;
    }
    int factor = (p_sign == "-") ? -1 : 1;
    
    // Apply extra time
    if(p_extraTime == g_dateNames[g_defaultLanguage][DN_DAY] || 
       p_extraTime == g_dateNames[g_defaultLanguage][DN_DAYS] )
    {
      mom = mom.AddDays(factor * p_interval);   
    }
    else if (p_extraTime == g_dateNames[g_defaultLanguage][DN_WEEK] || 
             p_extraTime == g_dateNames[g_defaultLanguage][DN_WEEKS] )
    {
      mom = mom.AddDays(factor * p_interval * 7);
    }
    else if (p_extraTime == g_dateNames[g_defaultLanguage][DN_MONTH] || 
             p_extraTime == g_dateNames[g_defaultLanguage][DN_MONTHS] )
    {
      mom = mom.AddMonths(factor * p_interval);
    }
    else if (p_extraTime == g_dateNames[g_defaultLanguage][DN_YEAR]  || 
             p_extraTime == g_dateNames[g_defaultLanguage][DN_YEARS] )
    {
      mom = mom.AddYears(factor * p_interval);
    }
    else return false;
  }
  p_temp.m_year  = (short) mom.Year();
  p_temp.m_month = (char)  mom.Month();
  p_temp.m_day   = (char)  mom.Day();

  return true;
}

bool
SQLDate::ParseDate(const XString& p_datum,int* p_jaar,int* p_maand,int* p_dag)
{
  XString datum(p_datum);

  // Minimum European of American format
  datum.Replace('-',' ');
  datum.Replace('/',' ');
  datum.Replace('.',' ');

  int num = sscanf_s(datum,"%d %d %d",p_dag,p_maand,p_jaar);

  // Need at x-y-z pattern or x-y pattern
  bool result = (num == 2 || num == 3);

  if(*p_jaar == 0)
  {
    SQLDate current = SQLDate::Today();
    *p_jaar = current.Year();
  }

  // Optional century correction
  if(*p_jaar < 100 && *p_jaar > 0)
  {
    *p_jaar += *p_jaar <= 50 ? 2000 : 1900;
  }
  return result;
}

// Adds days to the date and returns as a new instance
SQLDate
SQLDate::AddDays(const long p_numberOfDays) const
{
  SQLDate dat(*this);
  if(Valid())
  {
    dat.m_mjd += p_numberOfDays;
    dat.MJDtoDate(); // Recalc the date
  }
  return dat;
}

// adds months to the date and returns as a new instance
SQLDate
SQLDate::AddMonths(const long p_numberMonths) const
{
  SQLDate dat(*this);
  if(Valid())
  {
    if (p_numberMonths)
    {
      int month = dat.Month() + p_numberMonths - 1;
      if (month >= 0)
      {
        dat.m_date.m_month  = (char) ((month % 12) + 1);
        dat.m_date.m_year  += (short) (month / 12);
      }
      else
      {
        // Back in time
        // Making sure it rounds to a month
        month = -month - 1;
        dat.m_date.m_year -= (short) ((month / 12) + 1);
        dat.m_date.m_month = (char)  (12 - month % 12); 
      }
      // Round the number of days
      int maxDay = dat.DaysInMonth();
      if(dat.m_date.m_day > maxDay)
      {
        dat.m_date.m_day = (char) maxDay;
      }
      dat.SetMJD();
    }
  }
  return dat;
}

// Adds years to the date, and returns as a new instance
SQLDate
SQLDate::AddYears(const long p_numberOfYears) const
{
  SQLDate dat(*this);
  if(Valid())
  {
    dat.SetDate(Year() + p_numberOfYears,Month(),Day());
    if(!dat.Valid() && Month() == 2 && Day() == 29)
    {
      dat.SetDate(Year() + p_numberOfYears,Month(),Day() -1);
    }
  }
  return dat;
}

// Calculates the number of days betwee dates
long
SQLDate::DaysBetween(const SQLDate& p_date) const
{
  if(Valid() && p_date.Valid())
  {
    return (p_date.AsNumber() - AsNumber());
  }
  return 0;
}

// Calculates the number of months between dates
long
SQLDate::MonthsBetween(const SQLDate& p_date) const
{
  if(Valid() && p_date.Valid())
  {
    return ((p_date.Year() * 12) + p_date.Month()) - ((Year() * 12) + Month());
  }
  return 0;
}

// Calculates the number of years between dates
long 
SQLDate::YearsBetween(const SQLDate& p_date) const
{
  if(Valid() && p_date.Valid())
  {
    return p_date.Year() - Year();
  }
  return 0;
}

// STATIC is used by SQLTimestamp and SQLDate
//
// Split timestamp expressions of the form
// TODAY + 2 DAYS
// p_currentDate -> TODAY
// p_sign        -> +
// p_extraTime   -> DAYS
// p_interval    -> 2
void 
SQLDate::SplitStrDate(const XString& p_strDate,
                            XString& p_currentDate,
                            XString& p_sign,
                            XString& p_extraTime,
                            int&     p_interval)
{
  bool blnFound     = false;
  p_currentDate     = "";
  p_sign			      = "";
  p_extraTime		    = "";
  p_interval		    = 0;
  XString strIntrval;

  int intLength = p_strDate.GetLength();

  for (int index = 0; index < intLength; index++)
  {
    XString temp(p_strDate.GetAt(index),1);
    if (temp.Compare(" ") != 0)
    {
      if ((p_strDate.GetAt(index) >= '0') && (p_strDate.GetAt(index) <= '9'))
      {
        strIntrval += p_strDate.GetAt(index);
        p_interval = atoi(strIntrval); 
      }
      else if (p_strDate.GetAt(index) == '+' || p_strDate.GetAt(index) == '-')
      {
        p_sign += p_strDate.GetAt(index);
        blnFound = true;
      }
      else if (blnFound)
      {
        p_extraTime += p_strDate.GetAt(index);
      }
      else
      {
        p_currentDate += p_strDate.GetAt(index);
      }
    }
  }
  if (p_currentDate.IsEmpty())
  {
    p_currentDate = p_strDate;
  }
}

// XML Datum support
bool
SQLDate::ParseXMLDate(const XString& p_string,SQLTimestamp& p_moment)
{
  int ja[4] = {0,0,0,0};
  int ma[2] = {0,0};
  int da[2] = {0,0};
  int uu[2] = {0,0};
  int mi[2] = {0,0};
  int se[2] = {0,0};
  int fraction = 0;
  int UTCuu[2] = {0,0};
  int UTCmi[2] = {0,0};

  // changed char to unsigned int for 64 bit implementation
  char sep3,sep4,sep5,sep6,sep7,sep8;

  int n = sscanf_s(p_string
                  ,"%1d%1d%1d%1d-%1d%1d-%1d%1d%c%1d%1d%c%1d%1d%c%1d%1d%c%d%c%1d%1d%c%1d%1d"
                  ,&ja[0],&ja[1],&ja[2],&ja[3]
                  ,&ma[0],&ma[1]
                  ,&da[0],&da[1]
                  ,&sep3,(unsigned int) sizeof(char)
                  ,&uu[0],&uu[1]
                  ,&sep4,(unsigned int) sizeof(char)
                  ,&mi[0],&mi[1]
                  ,&sep5,(unsigned int) sizeof(char)
                  ,&se[0],&se[1]
                  ,&sep6,(unsigned int) sizeof(char)
                  ,&fraction
                  ,&sep7,(unsigned int) sizeof(char)
                  ,&UTCuu[0],&UTCuu[1]
                  ,&sep8,(unsigned int) sizeof(char)
                  ,&UTCmi[0],&UTCmi[1]);

  int jaar    = ja[0] * 1000 + ja[1] * 100 + ja[2] * 10 + ja[3];
  int maand   = ma[0] * 10 + ma[1];
  int dag     = da[0] * 10 + da[1];
  int uur     = uu[0] * 10 + uu[1];
  int minuut  = mi[0] * 10 + mi[1];
  int seconde = se[0] * 10 + se[1];
  int UTCuurBuffer = UTCuu[0] * 10 + UTCuu[1];
  int UTCminBuffer = UTCmi[0] * 10 + UTCmi[1];

  if(n >= 8 && jaar >= 0 && jaar <= 9999 && maand >= 0 && maand <= 12 && dag >= 0 && dag <= 31)
  {
    bool valid  = false;
    bool UTC    = false;
    bool offset = false;
    int  offsetminuten = 0;

    switch(n)
    {
    case 8:   // Has "YYYY-MM-DD" pattern
              valid = true;
              break;
    case 9:   // Has "YYYY-MM-DDZ" pattern
              UTC = (sep3 == 'Z');
              break;
    case 14:  // UTCuurBuffer in uur
              // UTCminBuffer in minuut
              if((sep3 == '-' || sep3 == '+') && sep4 == ':' && minuut >= 0 && minuut <= 59)
              {
                // Has "YYYY-MM-DD[+-]HH:MM" pattern
                offsetminuten = uur * 60 + minuut;
                if(offsetminuten >= 0 && offsetminuten <= 840)
                {
                  if(sep3 == '-')
                  {
                    offsetminuten = -offsetminuten;
                  }
                  uur    = 0;
                  minuut = 0;
                  offset = true;
                }
              }
              break;
    case 17:  // Has "YYYY-MM-DDTHH:MM:SS" pattern
              valid = sep3 == 'T' && sep4 == ':' && sep5 == ':';
              break;
    case 18:  // Has "YYYY-MM-DDTHH:MM:SSZ" pattern
              UTC   = sep3 == 'T' && sep4 == ':' && sep5 == ':' && sep6 == 'Z';
              break;
    case 19:  // Has "YYYY-MM-DDTHH:MM:SS.fraction" pattern
              valid = sep3 == 'T' && sep4 == ':' && sep5 == ':' && sep6 == '.';
              break;
    case 20:  // Has "YYYY-MM-DDTHH:MM:SS.fractionZ" pattern
              UTC   = sep3 == 'T' && sep4 == ':' && sep5 == ':' && sep6 == '.' && sep7 == 'Z';
              break;
    case 22:  // UTCuurBuffer in fractie
              // UTCminBuffer in UTCuurBuffer
              if(sep3 == 'T' && sep4 == ':'  && sep5 == ':' && 
                (sep6 == '+' || sep6 == '-') && sep7 == ':' && 
                UTCuurBuffer >= 0 && 
                UTCuurBuffer <= 59)
              {
                // Has "YYYY-MM-DDTHH:MM:SS[+-]HH:MM" pattern
                offsetminuten = fraction * 60 + UTCuurBuffer;
                if(offsetminuten >= 0 && offsetminuten <= 840)
                {
                  if(sep6 == '-')
                  {
                    offsetminuten = -offsetminuten;
                  }
                  offset = true;
                }
                fraction = 0;
              }
              break;
    case 25: if(sep3 == 'T' && sep4 == ':'  && sep5 == ':' && sep6 == '.' && 
               (sep7 == '+' || sep7 == '-') && sep8 == ':' && 
               UTCminBuffer >= 0 && 
               UTCminBuffer <= 59)
             {
               // Has "YYYY-MM-DDTHH:MM:SS.fraction[+-]HH:MM" pattern
               offsetminuten = UTCuurBuffer * 60 + UTCminBuffer;
               if(offsetminuten >= 0 && offsetminuten <= 840)
               {
                 if(sep7 == '-')
                 {
                   offsetminuten = -offsetminuten;
                 }
                 offset = true;
               }
             }
             break;
    }
    if((valid || offset || UTC) && 
      ((uur     >= 0 && uur     <= 23 && 
        minuut  >= 0 && minuut  <= 59 && 
        seconde >= 0 && seconde <= 59) || 
        (uur == 24 && minuut == 0 && seconde <= 1 && fraction == 0)))
    {
      // Leap seconds are fixed by adding an extra day and 0 hours
      bool plusday = (uur == 24);
      if(plusday)
      {
        uur = 0;
      }

      // Construct the timestamp
      p_moment.SetTimestamp(jaar,maand,dag,uur,minuut,seconde);
      if(plusday)
      {
        p_moment = p_moment.AddDays(1);
      }
      if(offset)
      {
        p_moment = p_moment.AddMinutes(offsetminuten);
      }
      // Store the fraction (if any)
      p_moment.SetFraction(fraction);

      // Zero time. Add the offset from UTC (including daylight-savings-time)
      if(sep6 == 'Z' || sep7 == 'Z')
      {
        extern SQLComponents::SQLInterval g_sql_timezone;
        p_moment = p_moment + g_sql_timezone;
      }

      return true;
    }
  }
  return false;
}

// Named date with short or long month names
bool
SQLDate::NamedDate(const XString& p_date,int& p_year,int& p_month,int& p_day)
{
  bool result = false;
  bool alpha  = false;
  for(int ind = 0;ind < p_date.GetLength(); ++ind)
  {
    if(isalpha(p_date.GetAt(ind)))
    {
      alpha = true;
      break;
    }
  }
  if(alpha)
  {
    int month = 0;
    int alphapos = -1;
    int lang = g_defaultLanguage;
    
    // Adjust language for array origin
    if(lang == LN_DEFAULT)
    {
      lang = LN_ENGLISH;
    }
        
    for(month = 0;month < MONTHS_IN_YEAR; ++month)
    {
      alphapos = p_date.Find(g_shortMonths[lang][month]);
      if(alphapos >= 0) break;
      alphapos = p_date.Find(g_monthnames [lang][month]);
      if(alphapos >= 0) break;
    }
    if(alphapos >= 0)
    {
      // Found a month name. Add one for array origin
      p_month = month + 1;
      
      // Default year is the current year
      SQLDate current = SQLDate::Today();
      p_year = current.Year();

      // Default day in the month = 1
      p_day = 1;
      
      // Check if first item is a number (day in the month)
      if(alphapos > 0)
      {
        p_day = atoi(p_date);
      }
      int spacepos = p_date.Find(' ',alphapos);
      if(spacepos > 0)
      {
        // More to come after the month
        XString after = p_date.Mid(spacepos);
        after.TrimLeft();
        
        // 'after' part contains 1 or 2 numbers
        int spacepos2 = after.Find(' ');
        if(spacepos2 > 0)
        {
          // Two parts after the month name
          p_day  = atoi(after);
          p_year = atoi(after.Mid(spacepos2 + 1));
        }
        else
        {
          // Just one part (year)
          p_year = atoi(after);
        }
      }
      // Adjust for century
      if(p_year < 100 && p_year > 0)
      {
        p_year += p_year <= 50 ? 2000 : 1900;
      }
      // Found a legal date.
      // Validation of numbers still to come
      result = true;
    }
  }
  return result;
}

//////////////////////////////////////////////////////////////////////////
//
// OPERATORS
//
//////////////////////////////////////////////////////////////////////////

// Assignment operators

SQLDate&
SQLDate::operator=(const SQLDate& datum)
{
  if(&datum != this)
  {
    if(datum.IsNull())
    {
      SetNull();
    }
    else
    {
      SetDate(datum.Year(),datum.Month(),datum.Day());
    }
  }
  return *this;
}

SQLDate&
SQLDate::operator=(const SQLTimestamp& p_date)
{
  if(p_date.IsNull())
  {
    SetNull();
  }
  else
  {
    SetDate(p_date.Year(),p_date.Month(),p_date.Day());
  }
  return *this;
}

// Temporal operators

SQLTimestamp  
SQLDate::operator+(const SQLTime& p_time) const
{
  SQLTimestamp stamp;

  // NULL if either of both sides is NULL
  if(IsNull() || p_time.IsNull())
  {
    return stamp;
  }
  stamp.SetTimestamp(Year()
                    ,Month()
                    ,Day()
                    ,p_time.Hour()
                    ,p_time.Minute()
                    ,p_time.Second());
  return stamp;
}

SQLInterval   
SQLDate::operator-(const SQLDate& p_date) const
{
  SQLInterval intval;
  intval.SetIntervalType(SQL_IS_DAY);

  // NULL if either of both sides is NULL
  if(IsNull() || p_date.IsNull())
  {
    return intval;
  }
  DateValue value = AsNumber() - p_date.AsNumber();
  intval.SetInterval(SQL_IS_DAY,value,0,0,0,0);
  return intval;
}

SQLDate
SQLDate::operator+(const SQLInterval& p_interval) const
{
  // NULL if either of both sides is NULL
  if(IsNull() || p_interval.IsNull())
  {
    SQLDate date;
    return date;
  }
  // Check if interval is the correct type
  if(p_interval.GetIsYearMonthType())
  {
    throw StdException("Incompatible interval to add to a date");
  }
  // Do the calculation
  SQLDate date(*this);
  date = date.AddDays(p_interval.GetDays());

  // Corner case where we land on the previous day!!
  if(p_interval.GetIsNegative())
  {
    if(p_interval.GetHours()   % 24 || 
       p_interval.GetMinutes() % 60 || 
       p_interval.GetSeconds()% 60  || 
       p_interval.GetFractionPart())
    {
      date = date.AddDays(-1);
    }
  }

  return date;
}

SQLDate
SQLDate::operator-(const SQLInterval& p_interval) const
{
  // NULL if either of both sides is NULL
  if(IsNull() || p_interval.IsNull())
  {
    SQLDate date;
    return date;
  }
  // Check if interval is the correct type
  if(p_interval.GetIsYearMonthType())
  {
    throw StdException("Incompatible interval to add to a date");
  }
  // Do the calculation
  SQLDate date(*this);
  date = date.AddDays(-(p_interval.GetDays()));

  // Corner case where we land on the previous day!!
  if(!p_interval.GetIsNegative())
  {
    if(p_interval.GetHours()   % 24 || 
       p_interval.GetMinutes() % 60 || 
       p_interval.GetSeconds() % 60 || 
       p_interval.GetFractionPart())
    {
      date = date.AddDays(-1);
    }
  }
  return date;
}

// End of namespace
}
