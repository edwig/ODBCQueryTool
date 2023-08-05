////////////////////////////////////////////////////////////////////////
//
// File: SQLTimestamp.h
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
#include "SQLLanguage.h"
#include <sqltypes.h>
#include <iostream>
#include <set>

namespace SQLComponents
{

class SQLTime;
class SQLDate;
class SQLInterval;
class SQLDatabase;

// Constants for one day
#define SECONDS_PER_DAY    (60 * 60 * 24)
#define SECONDS_PER_HOUR   (60*60)     // One hour in seconds
#define SECONDS_PER_MINUTE (60)        // One minute in seconds
// Number of nanoseconds per second
#ifndef NANOSECONDS_PER_SEC
#define NANOSECONDS_PER_SEC 1000000000
#endif
// Modified Julian Day Shift from Epoch (Astronomically)
// Actually it's 2,400,000.5 days (12:00 (noon) January 1st -4713 BC)
#define MJD_EPOCH 2400001

// Difference between local time and UTC
extern SQLInterval g_sql_timezone;

// Storage of a timestamp is done by means of the exact number 
// of seconds since MJD (Modified Julian Date). By doing this
// the SQLTimestamps and SQLDates can be internally recalculated
// as DateValue * SECONDS_PER_DAY = StampValue
// MJD = (2,400,000.5) + 0.5 (17 November 1858 instead of 16 November 12:00 hours)

struct StampStorage
{
  short m_year;      // 1 - 9999  year
  char  m_month;     // 1 - 12    months
  char  m_day;       // 1 - 31    days
  char  m_hour;      // 0 - 23    hour
  char  m_minute;    // 0 - 59    minute
  char  m_second;    // 0 - 59    seconds
};

// BEWARE: The epoch of the SQLDate class is 16 November 1858 12:00 hours (noon)
//         with a DateValue of zero (0) for that date.
// Timestamp is stored internally as a total number of seconds
// since MJD + 0.5 = 17 November 1858 instead of 16 November 12:00 hours
typedef __int64 StampValue;

class SQLTimestamp
{  
public:
  SQLTimestamp();
  explicit SQLTimestamp(const XString& p_string);
           SQLTimestamp(const SQLTimestamp& p_timestamp);
           SQLTimestamp(const SQLDate& p_date, const SQLTime& p_time);
           SQLTimestamp(int p_year,int p_month,int p_day,int p_hour,int p_min,int p_sec,int p_fraction = 0);
  explicit SQLTimestamp(const TIMESTAMP_STRUCT* p_stamp);
  explicit SQLTimestamp(StampValue p_value,int p_fraction = 0);
 ~SQLTimestamp();
 
  void Init(const XString& string);

  void SetTimestamp(const XString& p_string);
  void SetTimestamp(int p_year,int p_month,int p_day,
                    int p_hour,int p_min,  int p_sec,
                    int p_fraction = 0);
  void SetFraction(int p_fraction);
  static       SQLTimestamp  CurrentTimestamp(bool p_fraction = false);
  static const SQLTimestamp& FarInTheFuture();
  static const SQLTimestamp& FarInThePast();

  // Assignment operators
  SQLTimestamp& operator= (const SQLTimestamp& p_timestamp);
  SQLTimestamp& operator= (const SQLDate& p_date);
  SQLTimestamp& operator= (const SQLTime& p_time);
  // Temporal operators
  SQLInterval   operator- (const SQLTimestamp& p_timestamp) const;
  SQLTimestamp  operator+ (const SQLInterval&  p_interval)  const;
  SQLTimestamp  operator- (const SQLInterval&  p_interval)  const;
  SQLTimestamp  operator+ (const SQLTime&      p_time)      const;
  SQLTimestamp  operator- (const SQLTime&      p_time)      const;
  // Comparison operators
  bool          operator==(const SQLTimestamp& p_timestamp) const;
  bool          operator!=(const SQLTimestamp& p_timestamp) const;
  bool          operator< (const SQLTimestamp& p_timestamp) const;
  bool          operator> (const SQLTimestamp& p_timestamp) const;
  bool          operator<=(const SQLTimestamp& p_timestamp) const;
  bool          operator>=(const SQLTimestamp& p_timestamp) const;

  // Comparison without correct NULL behavior
  // So two NULL timestamps equals to each other
  bool    ExactEqual   (const SQLTimestamp& p_timestamp) const;
  bool    NotExactEqual(const SQLTimestamp& p_timestamp) const;

  int     Year()    const;      // Year                 (0-9999)
  int     Month()   const;      // Month of the year    (0-12)
  int     Day()     const;      // Day of the month     (0-31)
  int     Hour()    const;      // Hour of the day      (0-23)
  int     Minute()  const;      // Minute in the hour   (0-59)
  int     Second()  const;      // Second in the minute (0-59)
  int     Fraction()const;      // Fraction of a second in nanoseconds (0 - 999.999.999)
  int     WeekDay() const;      // 1=Sunday, 2=Thursday, ... 7=Saturday
  XString WeekDayName(Language p_lang = LN_DEFAULT) const;  // Name of the day of the week
  XString MonthName  (Language p_lang = LN_DEFAULT) const;  // Name of the month of the year
  int     DaysInMonth() const;

  int     YearsBetween    (const SQLTimestamp& p_timestamp) const; 
  int     MonthsBetween   (const SQLTimestamp& p_timestamp) const; 
  int     DaysBetween     (const SQLTimestamp& p_timestamp) const;
  int     HoursBetween    (const SQLTimestamp& p_timestamp) const; 
  int     MinutesBetween  (const SQLTimestamp& p_timestamp) const; 
  __int64 SecondsBetween  (const SQLTimestamp& p_timestamp) const;

  SQLTimestamp AddDays    (int p_number) const;
  SQLTimestamp AddMonths  (int p_number) const;
  SQLTimestamp AddYears   (int p_number) const;
  SQLTimestamp AddHours   (int p_number) const;
  SQLTimestamp AddMinutes (int p_number) const;
  SQLTimestamp AddSeconds (__int64 p_number) const;
  SQLTimestamp AddFraction(int p_fraction) const;

  XString  AsString      (int p_precision = 0) const;   // DD-MM-YYYY HH:MM:SS[.ffff]
  XString  AsXMLString   (int p_precision = 0) const;   // YYYY-MM-DDTHH:MM:SS[.ffff]             LocalTime
  XString  AsXMLStringTZ (int p_precision = 0) const;   // YYYY-MM-DDTHH:MM_SS[.ffff]{+/-}HH.MM   LocalTime + UTC offset
  XString  AsXMLStringUTC(int p_precision = 0) const;   // YYYY-MM-DDTHH:MM:SS[.ffff]Z            UTC-Time  + Zulu
  XString  AsSQLString(SQLDatabase* p_database) const;  // In current database default format
  SQLDate  AsSQLDate() const;
  SQLTime  AsSQLTime() const;
  void     AsTimeStampStruct(TIMESTAMP_STRUCT* p_struct) const;

  // NULL Status
  bool     IsNull()  const;
  bool     Valid()   const;
  void     SetNull();

  // For usage in integer, date, time and interval 
  StampValue Value() const;
  StampValue AsNumber() const;
  StampValue AsTimeSinceEpoch() const;

  //////////////////////////////////////////////////////
  // Static use by SQLDate and SQLTimestamp
  static void SplitStrDate(const XString& strDate,
                           XString&       CurrentDate,
                           XString&       Sign,
                           XString&       ExtraTime,
                           int&           interval);

private:
  //
  // Private methods
  //
  // Parse string to timestamp
  void    ParseMoment(const XString& p_string);
  // Named timestamp with short or long month names
  bool    ParseNamedDate(const XString& p_string);
  // Calculate value from a StampStorage
  void    RecalculateValue();
  // Calculate StampStorage from a value
  void    Normalise();
  // Set StampValue to an explicit value
  void    SetValue(StampValue dubbel);
  // Calculates the number of days in a month
  int     DaysInMonth(int p_year,int p_month) const;
  // Calculates a timestamp from an interface string with extra operands
  bool    GetVirtualMoment(XString        p_sign
                          ,XString        p_extraTime
                          ,int            p_interval
                          ,StampStorage&  p_temp
                          ,bool           p_doTimes = true);
  // Validate the timestamp for valid values for a database
  void    Validate();
  // Print the fraction to a string
  XString PrintFraction(int p_precision) const;

  ///////////////////////////////////////////////////////////////////////////////////////////
  //
  // data members
  //
  StampStorage m_timestamp; // year/month/second/hour/minute/day
  StampValue   m_value;		  // Seconds since MJD Epoch
  int          m_fraction;  // 0 - 999.999.999 nanoseconds
};

inline StampValue
SQLTimestamp::Value() const
{
  return m_value;
}

inline StampValue
SQLTimestamp::AsNumber() const
{
  return m_value;
}

inline StampValue
SQLTimestamp::AsTimeSinceEpoch() const
{
  return m_value;
}

inline void
SQLTimestamp::SetValue(StampValue waarde)
{
  m_value = waarde;
}

inline bool
SQLTimestamp::ExactEqual(const SQLTimestamp& moment) const
{
  return m_value == moment.m_value;
}

inline bool
SQLTimestamp::NotExactEqual(const SQLTimestamp& moment) const
{
  return m_value != moment.m_value;
}

inline int
SQLTimestamp::DaysInMonth() const
{
  return DaysInMonth(Year(),Month());
}

inline void
SQLTimestamp::SetFraction(int p_fraction)
{
  m_fraction = p_fraction;
}

// End of namespace
}