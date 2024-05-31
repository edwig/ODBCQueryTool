////////////////////////////////////////////////////////////////////////
//
// File: SQLTime.h
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
#include <sqltypes.h>

namespace SQLComponents
{

class SQLDate;
class SQLTimestamp;
class SQLInterval;
class SQLDatabase;

#define ONE_MINUTE (60)        // One minute in seconds
#define ONE_HOUR   (60*60)     // One hour in seconds
#define ONE_DAY    (24*60*60)  // One day in seconds

// Storage of the time in hours,minutes & seconds
struct TimeStorage
{
  int m_hour;
  int m_minute;
  int m_second;
};

// Storage of the time in seconds in the day
// NULL value is represented by -1 !!!
typedef long  TimeValue;

class SQLTime
{
public:   
  SQLTime();
  SQLTime(const SQLTime& p_time);
  explicit SQLTime(const SQLTimestamp& p_timestamp);
  explicit SQLTime(XString p_time);
  explicit SQLTime(const int p_hours,const int p_minutes,const int p_seconds);
  explicit SQLTime(__int64 p_seconds);
  explicit SQLTime(const SQL_TIME_STRUCT* p_timestruct);
 ~SQLTime();

  XString AsString() const;
  XString AsSQLString(SQLDatabase* p_database) const;
  XString AsStrippedSQLString(SQLDatabase* p_database) const;
  void    AsTimeStruct(SQL_TIME_STRUCT* p_timestruct) const;
  long    AsNumber() const;

  // Parts of the time
  int     Hour()   const;
  int     Minute() const;
  int     Second() const;

  // Calculations on the time
  SQLTime AddHours      (const int p_number) const;
  SQLTime AddMinutes    (const int p_number) const;
  SQLTime AddSeconds    (const int p_number) const;
  int     HoursBetween  (const SQLTime& p_time) const; 
  int     MinutesBetween(const SQLTime& p_time) const; 
  int     SecondsBetween(const SQLTime& p_time) const;

  // Assignment of a time
  SQLTime&      operator= (const SQLTime& p_time);
  SQLTime&      operator= (const SQLTimestamp& p_timestamp);
  // Temporal operators
  SQLInterval   operator- (const SQLTime& p_time) const;
  SQLTimestamp  operator+ (const SQLDate& p_date) const;
  SQLTime       operator+ (const SQLInterval& p_interval) const;
  SQLTime       operator- (const SQLInterval& p_interval) const;
  // Comparison operators
  bool          operator==(const SQLTime& p_time) const;
  bool          operator!=(const SQLTime& p_time) const;
  bool          operator< (const SQLTime& p_time) const;
  bool          operator> (const SQLTime& p_time) const;
  bool          operator<=(const SQLTime& p_time) const;
  bool          operator>=(const SQLTime& p_time) const;
  
  // Contains a valid time
  bool    Valid() const;
  bool    IsNull() const;

  // Set a new time
  void    SetTime(int p_hour,int p_minute,int p_second);
  // To set the time from an internal string
  void    SetTime(const XString& p_string);
  // Reset to NULL
  void    SetNull();

  // Time entered from an user interface
  void    ParseTime(const XString& str);

  // Current system time
  static SQLTime SystemTime();

private:
  // Recalculate the sconds from hour/minute/second
  void SetTime();
  // Normalise seconds back to hour/minute/second
  void Normalise();
  // Scan for short time notation
  bool ShortTime(const XString& p_string,int& p_hour,int& p_min,int& p_sec);
  // XML Time support
  bool ParseXMLTime(const XString& p_string);

  // STORAGE OF THE TIME

  TimeValue   m_seconds;    // Total number of seconds in the day, -1 = NULL VALUE
  TimeStorage m_theTime;    // hour, minute, second
};          

inline int
SQLTime::Hour() const
{
  return m_theTime.m_hour;
}

inline int
SQLTime::Minute() const
{
  return m_theTime.m_minute;
}

inline int
SQLTime::Second() const
{
  return m_theTime.m_second;
}

inline bool 
SQLTime::Valid() const
{
  return m_seconds >= 0;
}

inline bool
SQLTime::IsNull() const
{
  return (m_seconds < 0);
}

// Number of hours between the times
inline int
SQLTime::HoursBetween(const SQLTime& p_time) const 
{
  return (p_time.m_seconds - m_seconds) / ONE_HOUR;
}
            
// Returns the minutes between the times
inline int
SQLTime::MinutesBetween(const SQLTime& p_time) const 
{
  return (p_time.m_seconds - m_seconds) / ONE_MINUTE;
}
            
// Returns the seconds between the times
inline int
SQLTime::SecondsBetween(const SQLTime& p_time) const 
{
  return p_time.m_seconds - m_seconds;
}
            
inline bool
SQLTime::operator<(const SQLTime& p_time) const
{
  return m_seconds < p_time.m_seconds;
}

inline bool
SQLTime::operator>(const SQLTime& p_time) const
{
  return m_seconds > p_time.m_seconds;
}

inline bool
SQLTime::operator==(const SQLTime& p_time) const
{
  return m_seconds == p_time.m_seconds;
}

inline bool 
SQLTime::operator!=(const SQLTime& p_time) const
{
  return p_time.m_seconds != m_seconds;
}

inline bool 
SQLTime::operator<=(const SQLTime& p_time) const
{
  return m_seconds <= p_time.m_seconds;
}

inline bool 
SQLTime::operator>=(const SQLTime& p_time) const
{
  return m_seconds >= p_time.m_seconds;
}

// End of namespace
}