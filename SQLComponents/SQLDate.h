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
#pragma once
#include "SQLDatabase.h"
#include "SQLTime.h"
#include "SQLTimestamp.h"
#include "SQLInterval.h"
#include "SQLLanguage.h"

namespace SQLComponents
{

#define SECONDS_PER_DAY   (60 * 60 * 24)
#define OLEDATE_MJD_SHIFT 15018             // Difference between MJD and OLE_DATE

// BEWARE: The epoch of the SQLDate class is 16 november 1858 12:00 hours (noon)
//         with a DateValue of zero (0) for that date.
// Dates are stored as a MJD (Modified Julian Date = 17-11-1858)
// and works correctly for dates from 1-1-1 up until 31-12-9999 (ODBC Definition)
typedef long DateValue;

// Date in memory is a struct
typedef struct _Date
{
  short m_year;   // 0 - 9999 jaren
  char  m_month;  // 1 - 12   maanden
  char  m_day;    // 1 - 31   dagen
}
DateStorage;

class SQLDate
{
public:  
  // Date constructed as NULL date
  SQLDate();

  // Date constructed from a day, month and year
  SQLDate(const long p_day,const long p_month,const long p_year);

  // Date made from another date
  SQLDate(const SQLDate& p_date);

  // Date made from a string
  explicit SQLDate(const XString& p_string);

  // Date made from a timestamp
  explicit SQLDate(const SQLTimestamp& p_timestamp);

  // Date made from a DATE_STRUCT
  explicit SQLDate(const DATE_STRUCT* date);

  // Date made from a MJD date value
  explicit SQLDate(DateValue waarde);

  // Date made from a COleDateTime value
  explicit SQLDate(double p_oledatetime);

  ~SQLDate();

  bool        SetDate(const XString& p_string); 
  bool        SetDate(long p_year,long p_month,long p_day);
  void        SetNull();

  // Get in different formats
  XString     AsString() const;
  XString     AsXMLString() const;
  DateValue   AsNumber() const;
  DateValue   AsMJD()    const;
  DateValue   AsJulianDate()     const;
  DateValue   AsTimeSinchEpoch() const;
  XString     AsSQLString(SQLDatabase* p_database);
  XString     AsStrippedSQLString(SQLDatabase* p_database);
  void        AsDateStruct(DATE_STRUCT* p_date) const;

  // Internal status of a date
  bool        Valid()         const;
  bool        IsNull()        const;

  // Formatted parts of a date
  long        Year()          const;
  long        Month()         const;
  long        Day()           const;
  long        WeekDay()       const;
  int         WeekNumber()    const;
  long        DaysInMonth()   const;
  XString     WeekDayName(Language p_lang = LN_DEFAULT) const;
  XString     MonthName  (Language p_lang = LN_DEFAULT) const;
  XString     FullDate   (Language p_lang = LN_DEFAULT) const;
 
  // Perform operations on a date
  SQLDate     AddDays       (const long p_numberOfDays)   const;
  SQLDate     AddMonths     (const long p_numberOfMonths) const;
  SQLDate     AddYears      (const long p_numberOfYears)  const;

  long        YearsBetween  (const SQLDate& p_date) const; 
  long        MonthsBetween (const SQLDate& p_date) const; 
  long        DaysBetween   (const SQLDate& p_date) const;

  // Assignment operator and methods
  SQLDate&      operator= (const SQLDate&      p_date);
  SQLDate&      operator= (const SQLTimestamp& p_date);
  // Temporal operators
  SQLTimestamp  operator+ (const SQLTime& p_time) const;
  SQLInterval   operator- (const SQLDate& p_date) const;
  SQLDate       operator+ (const SQLInterval& p_interval) const;
  SQLDate       operator- (const SQLInterval& p_interval) const;
  // Logical comparison operators on a a date
  bool          operator <(const SQLDate& p_date) const;
  bool          operator >(const SQLDate& p_date) const;
  bool          operator==(const SQLDate& p_date) const;
  bool          operator!=(const SQLDate& p_date) const;
  bool          operator<=(const SQLDate& p_date) const;
  bool          operator>=(const SQLDate& p_date) const;

  // Asking for the current date
  static SQLDate Today();
  // Ook voor gebruik vanuit de interface (static aanroepen)
  bool CalculateDate(const XString& p_date);
  // Haal een extra virtuele datum op (+/- <getal> <JAAR/MAAND/DAG/WEEK>)
  bool GetVirtualDate(XString       p_sign,
                      XString       p_extraTime,
                      long          p_interval,
                      DateStorage&  p_temp);
  static void SplitStrDate(const  XString& p_strDate,
                                  XString& p_currentDate,
                                  XString& p_sign,
                                  XString& p_extraTime,
                                  int&     p_interval);
  // Check that a numeric string has only digits
  static bool IsNumericString(const XString& p_string);

private:
  // Correction factor is MJD (2,400,000.5) + 0.5 (17 nov 1858 instead of 16 nov 12:00 hours)
  const long JULIAN_DAY_MODIFIED  = 2400001;

  // Calculate m_mjd from a date
  bool SetMJD();
  // Calculate MJD back to a date
  void MJDtoDate();
  // Short date for some interfaces
  bool ShortDate(const XString& p_date,int& p_year,int& p_month,int& p_day);
  // Named date with short or long monthnames
  static
  bool NamedDate(const XString& p_date,int& p_year,int& p_month,int& p_day);
  // Pure date string "dd-mm-[yy]yy"
  bool ParseDate(const XString& p_date,int* p_year,int* p_month,int* p_day);
  // XML Datum support
  static bool ParseXMLDate(const XString& p_string,SQLTimestamp& p_moment);
  // For the parsing of XML
  friend SQLTimestamp;

  // STORAGE OF THE DATE
  DateValue   m_mjd;    // Modified Julian day
  DateStorage m_date;   // Day/Month/Year
};      
 
// Is Empty for databases and not-filled alike
inline bool
SQLDate::IsNull() const
{
  return (m_mjd == -1);
}

// Return the calculated value (MJD) !!
inline DateValue
SQLDate::AsNumber() const
{
  return m_mjd;
}

// Return the Modified Julian Date
inline DateValue
SQLDate::AsMJD() const
{
  return m_mjd;
}

inline DateValue
SQLDate::AsJulianDate() const
{
  return m_mjd + JULIAN_DAY_MODIFIED;
}

// Return the time-since-epoch
inline DateValue
SQLDate::AsTimeSinchEpoch() const
{
  return m_mjd;
}
      
// Test on validity of a date instance
inline bool
SQLDate::Valid() const
{
  return (m_mjd >= 0);
}

// Weekday 1= sunday, 2=monday, ... 7=saturday
inline long
SQLDate::WeekDay() const 
{ 
  return ((m_mjd + 3) % 7);
}

// Day of the date
inline long
SQLDate::Day() const 
{
  return m_date.m_day;
}

// Month of the date, 1=jan, 2=feb, .... , 12=december
inline long
SQLDate::Month() const 
{
  return m_date.m_month;
}

// Year of the instance in 4 digits (1997,2001 etc)
inline long 
SQLDate::Year() const 
{
  return m_date.m_year;
}

// Operator 'smaller than'
inline bool
SQLDate::operator<(const SQLDate& datum) const
{
  return m_mjd < datum.m_mjd;
}

// Operator 'greater than'
inline bool
SQLDate::operator>(const SQLDate& datum) const
{
  return m_mjd > datum.m_mjd;
}

// Operator 'equals'
inline bool
SQLDate::operator==(const SQLDate& datum) const
{
  return m_mjd == datum.m_mjd;
}

// Operator 'not equal'
inline bool
SQLDate::operator!=(const SQLDate& datum) const 
{
  return !(operator==(datum));
}

// Operator 'smaller than or equal to'
inline bool
SQLDate::operator<=(const SQLDate& datum) const
{
  return !(operator>(datum));
}

// Operator 'greater than or equal to'
inline bool
SQLDate::operator>=(const SQLDate& datum) const
{
  return !(operator<(datum));
}

// End of namespace
}