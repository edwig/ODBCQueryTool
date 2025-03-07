////////////////////////////////////////////////////////////////////////
//
// File: SQLTime.cpp
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
#include "StdAfx.h"
#include "SQLComponents.h"
#include "SQLTime.h" 
#include "SQLDate.h"
#include "SQLTimestamp.h"
#include "SQLInterval.h"
#include "SQLDatabase.h"
#include "SQLLanguage.h"
#include <sstream>
#include <ctime>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{

// CTOR as a NULL time
SQLTime::SQLTime()
{
  SetNull();
}

// Constructor from a string
SQLTime::SQLTime(XString p_time)
{
  ParseTime(p_time);
}

// Constructor from the parts
//
SQLTime::SQLTime(const int p_hour, 
                 const int p_minute, 
                 const int p_second)
{
  m_theTime.m_hour   = p_hour;
  m_theTime.m_minute = p_minute;
  m_theTime.m_second = p_second;
  SetTime();
}

// Construct from another time
//
SQLTime::SQLTime(const SQLTime& p_time) 
        :m_seconds(p_time.m_seconds)
        ,m_theTime(p_time.m_theTime)
{
}

// Constructor from a timestamp
//
SQLTime::SQLTime(const SQLTimestamp& p_timestamp)
{
  if(p_timestamp.IsNull())
  {
    SetNull();
  }
  else
  {
    m_theTime.m_hour   = p_timestamp.Hour();
    m_theTime.m_minute = p_timestamp.Minute();
    m_theTime.m_second = p_timestamp.Second();
    SetTime();
  }
}

// Constructor from a total number of seconds
//
SQLTime::SQLTime(__int64 p_seconden)
{
  m_seconds = (long)p_seconden;
  Normalise();
}

// CTOR from a SQL_TIME_STRUCT
//
SQLTime::SQLTime(const SQL_TIME_STRUCT* p_timestruct)
{
  if (p_timestruct == nullptr)
  {
    SetNull();
  }
  else
  {
    m_theTime.m_hour   = p_timestruct->hour;
    m_theTime.m_minute = p_timestruct->minute;
    m_theTime.m_second = p_timestruct->second;
    SetTime();
  }
}

// DTOR for SQLTime
//
SQLTime::~SQLTime()
{
  // Nothing here yet
}

// Setting the time
// Storage value is the total number of seconds on a day
void
SQLTime::SetTime(int p_hour,int p_minute,int p_second)
{
  m_theTime.m_hour   = p_hour;
  m_theTime.m_minute = p_minute;
  m_theTime.m_second = p_second;
  SetTime();
}

// Private SetTime: Calculate the number of seconds from the storage
void
SQLTime::SetTime()
{
  m_seconds = 0;
  if(m_theTime.m_hour   >= 0 && m_theTime.m_hour   < 24 &&
     m_theTime.m_minute >= 0 && m_theTime.m_minute < 60 &&
     m_theTime.m_second >= 0 && m_theTime.m_second < 60)
  {
    m_seconds = (m_theTime.m_hour   * ONE_HOUR  ) + 
                (m_theTime.m_minute * ONE_MINUTE) +
                 m_theTime.m_second;
  }
  else
  {
    // Tell it to the outside world
    XString error;
    error.Format(_T("Incorrect time %02d:%02d:%02d"),m_theTime.m_hour,m_theTime.m_minute,m_theTime.m_second);
    // Reset the time to NULL
    SetNull();
    // Then throw the error
    throw StdException(error);
  }
}

// Reset the time to NULL
void
SQLTime::SetNull()
{
  m_seconds = -1;
  m_theTime.m_hour   = 
  m_theTime.m_minute = 
  m_theTime.m_second = 0;
}

// The time as a string
XString
SQLTime::AsString() const
{
  XString buffer;
  if(IsNull() == false)
  {
    buffer.Format(_T("%02d:%02d:%02d"), Hour(), Minute(), Second());
  }
  return buffer;
}

// Time as a string for storage in a database
XString
SQLTime::AsSQLString(SQLDatabase* p_database) const
{
  if(IsNull())
  {
    return _T("");
  }
  return p_database->GetSQLTimeString(Hour(),Minute(),Second());
}

// Time as a stripped SQL string for usage in SQL statements
XString
SQLTime::AsStrippedSQLString(SQLDatabase* p_database) const
{
  if(IsNull())
  {
    return _T("");
  }
  return p_database->GetStrippedSQLTimeString(Hour(),Minute(),Second());
}

void
SQLTime::AsTimeStruct(SQL_TIME_STRUCT* p_timestruct) const
{
  p_timestruct->hour   = (SQLUSMALLINT) Hour();
  p_timestruct->minute = (SQLUSMALLINT) Minute();
  p_timestruct->second = (SQLUSMALLINT) Second();
}

// Time as a number (seconds in the day)
// Can return "-1" for a NULL value
long
SQLTime::AsNumber() const
{
  return m_seconds;
}

// Get the value from a string of format: uu:mm(:ss)
void
SQLTime::SetTime(const XString& string)
{
  ParseTime(string);
}

// The current system time
SQLTime
SQLTime::SystemTime()
{
  _tzset();
  __time64_t ltime;
  _time64(&ltime);
  struct tm thetime ;
  _localtime64_s(&thetime,&ltime);

  SQLTime nu(thetime.tm_hour,thetime.tm_min,thetime.tm_sec);
  return nu;
}

// Adds the number as hours and returns a new SQLTime
SQLTime
SQLTime::AddHours(const int p_number) const 
{
  SQLTime now(*this);
  if(Valid() && p_number)
  {
    now.m_seconds += p_number * ONE_HOUR;
    now.Normalise();
  }
  return now;
}
            
// Adds the minutes and returns a new SQLTime
SQLTime
SQLTime::AddMinutes(const int p_number) const 
{
  SQLTime now(*this);
  if(Valid() && p_number)
  {
    now.m_seconds += p_number * ONE_MINUTE;
    now.Normalise();
  }
  return now;
}
            
// Adds the seconds and returns a new SQLTime
SQLTime
SQLTime::AddSeconds(const int p_number) const 
{
  SQLTime now(*this);
  if(Valid() && p_number)
  {
    now.m_seconds += p_number;
    now.Normalise();
  }
  return now;
}     
    
// Normalise the number of hours, minutes and seconds       
//
void
SQLTime::Normalise()
{
  if(m_seconds >= ONE_DAY)
  {
    m_seconds %= ONE_DAY;
  }
  if(m_seconds < 0)
  {
    m_seconds = ONE_DAY - (-m_seconds % ONE_DAY);
  }
  m_theTime.m_hour   = m_seconds / ONE_HOUR;
  long rest          = m_seconds % ONE_HOUR;
  m_theTime.m_minute = rest      / ONE_MINUTE;
  m_theTime.m_second = rest      % ONE_MINUTE;
}

// Parse time from a string
// hh:mm(:ss) / CURRENT / NOW / etc
void
SQLTime::ParseTime(const XString& p_string)
{
  // Copy and trim the string
  XString string(p_string);
  string.Trim();

  if(string.IsEmpty())
  {
    // No time string, do not parse
    SetNull();
    return;
  }

  // Test if we are properly initialized
  SQLComponentsInitialized();

  // Speed optimization. Parse only if alpha char
  if(isalpha(string.GetAt(0)))
  {
    if (string.CompareNoCase(g_dateNames[g_defaultLanguage][DN_CURRENT]) == 0 ||
        string.CompareNoCase(g_dateNames[g_defaultLanguage][DN_NOW])     == 0 )
    {
      *this = SystemTime();
      return;
    }
  }

  if(ParseXMLTime(p_string))
  {
    return;
  }

  // Support different separators 
  string.Replace(_T(" "),_T(":"));
  string.Replace(_T("-"),_T(":"));
  string.Replace(_T("."),_T(":"));
  string.Replace(_T("/"),_T(":"));

  // Clear the result
  m_theTime.m_hour   = -1;
  m_theTime.m_minute = 0;
  m_theTime.m_second = 0;

  if(ShortTime(string,m_theTime.m_hour,m_theTime.m_minute,m_theTime.m_second) == false)
  {
    //  Parse the string
    int n = _stscanf_s(string,_T("%d:%d:%d")
                      ,&m_theTime.m_hour
                      ,&m_theTime.m_minute 
                      ,&m_theTime.m_second);
    if(n < 2)
    {
      throw StdException(_T("Wrong format for conversion to time. Must be in the format: hh:mm[:ss]"));
    }
  }

  // Check for correct values
  if (m_theTime.m_hour < 0 || m_theTime.m_hour > 23)
  {
    throw StdException(_T("Incorrect time format: the value of the hour must be between 0 and 23 (inclusive)"));
  }

  if (m_theTime.m_minute < 0 || m_theTime.m_minute > 59)
  {
    throw StdException(_T("Incorrect time format: the value of minutes must be between 0 and 59 (inclusive)"));
  }

  if (m_theTime.m_second < 0 || m_theTime.m_second > 59)
  {
    throw StdException(_T("Incorrect time format: the value of the seconds must be between 0 and 59 (inclusive)"));
  }
  SetTime();
}

// Scan for short time notation
// Support of short time formats (HH, HHMM of HHMMSS)
bool
SQLTime::ShortTime(const XString& p_string,int& p_hour,int& p_min,int& p_sec)
{
  bool success = false;
  // Expectation pattern
  // HH        Hour
  // HHMM      Hour:Minute
  // HHMMSS    Hour:Minute:Seconds

  // Support for short formats (HH, HHMM of HHMMSS)
  int len = p_string.GetLength();
  if(len == 2 || len == 4 || len == 6 && SQLDate::IsNumericString(p_string))
  {
    switch(len)
    {
      case 2: p_hour  = _ttoi(p_string);
              p_min   = 0;
              p_sec   = 0;
              success = true;
              break;
      case 4: p_hour  = _ttoi(p_string.Left(2));
              p_min   = _ttoi(p_string.Mid(2));
              p_sec   = 0;
              success = true;
              break;
      case 6: p_hour  = _ttoi(p_string.Left(2));
              p_min   = _ttoi(p_string.Mid(2,2));
              p_sec   = _ttoi(p_string.Mid(4));
              success = true;
              break;
    }  
  }
  return success;
}

// XML Time support
bool
SQLTime::ParseXMLTime(const XString& p_string)
{
  unsigned int uu[2] = {0,0};
  unsigned int mi[2] = {0,0};
  unsigned int se[2] = {0,0};
  unsigned int fraction = 0;
  unsigned int UTCuu[2] = {0,0};
  unsigned int UTCmi[2] = {0,0};

  //  Parse the string
  // changed char to unsigned int for 64 bit implementation
  TCHAR sep1,sep2,sep3,sep4,sep5;
  int n = _stscanf_s(p_string,_T("%1u%1u%c%1u%1u%c%1u%1u%c%u%c%1u%1u%c%1u%1u"),
                  &uu[0],&uu[1],
                  &sep1,(unsigned int) sizeof(TCHAR),
                  &mi[0],&mi[1],
                  &sep2,(unsigned int) sizeof(TCHAR),
                  &se[0],&se[1],
                  &sep3,(unsigned int) sizeof(TCHAR),
                  &fraction,
                  &sep4,(unsigned int) sizeof(TCHAR),
                  &UTCuu[0],&UTCuu[1],
                  &sep5,(unsigned int) sizeof(TCHAR),
                  &UTCmi[0],&UTCmi[1]);

  int uurBuffer = uu[0] * 10 + uu[1];
  int minBuffer = mi[0] * 10 + mi[1];
  int secBuffer = se[0] * 10 + se[1];
  int UTCuurBuffer = UTCuu[0] * 10 + UTCuu[1];
  int UTCminBuffer = UTCmi[0] * 10 + UTCmi[1];

  if(n >= 8 && sep1 == ':' && sep2 == ':' && 
     ((uurBuffer >=  0 && uurBuffer <= 23 && 
       minBuffer >=  0 && minBuffer <= 59 && 
       secBuffer >=  0 && secBuffer <= 59) ||
      (uurBuffer == 24 && minBuffer == 0 && secBuffer == 0)))
  {
    SetTime(uurBuffer,minBuffer,secBuffer);
    bool UTC = false;

    switch(n)
    {
      case 8:     // Has "HH:MM:SS" pattern
                  return true;
                  break;
      case 9:     // Has "HH:MM:SSZ" pattern for UTC time
                  UTC = sep3 == 'Z';
                  break;
      case 10:    if(sep3 == '.')
                  {
                    // Has "HH:MM:SS.fraction" pattern
                    return true;
                  }
                  break;
      case 11:    // Has "HH:MM:SS.fractionZ" pattern for UTC time
                  UTC = sep3 == '.' && sep4 == 'Z';
                  break;
      case 13:    // UTCuurBuffer in fraction
                  // UTCminBuffer in UTCuurBuffer
                  //
                  if((sep3 == '-' || sep3 == '+') && sep4 == ':' && 
                    UTCuurBuffer >= 0 && 
                    UTCuurBuffer <= 59)
                  {
                    // Has "HH:MM:SS[+-]UU:MM" pattern
                    int minutes = fraction * 60 + UTCuurBuffer;
                    if(minutes >= 0 && minutes <= 840)
                    {
                      if(sep3 == '+')
                      {
                        minutes = -minutes;
                      }
                      m_seconds += minutes * ONE_MINUTE;
                      Normalise();
                      UTC = true;
                    }
                  }
                  break;
      case 16:    if(sep3 == '.' && (sep4 == '-' || sep4 == '+') && sep5 == ':' && 
                    UTCminBuffer >= 0 && UTCminBuffer <= 59)
                  {
                    // Has "HH:MM:SS.fraction[+-]UU:MM" pattern
                    int minutes = UTCuurBuffer * 60 + UTCminBuffer;
                    if(minutes >= 0 && minutes <= 840)
                    {
                      if(sep4 == '+')
                      {
                        minutes = -minutes;
                      }
                      m_seconds += minutes * ONE_MINUTE;
                      Normalise();
                      UTC = true;
                    }
                  }
                  break;
    }
    if(UTC)
    {
      TIME_ZONE_INFORMATION tziCurrent;
      ::ZeroMemory(&tziCurrent,sizeof(tziCurrent));
      if(::GetTimeZoneInformation(&tziCurrent) != TIME_ZONE_ID_INVALID)
      {
        m_seconds += -tziCurrent.Bias * ONE_MINUTE;
        Normalise();
      }
      return true;
    }
  }
  return false;
}

//////////////////////////////////////////////////////////////////////////
//
// OPERATORS
//
//////////////////////////////////////////////////////////////////////////

// Assignment operators

SQLTime&
SQLTime::operator=(const SQLTime& p_time)
{
  if(this != &p_time)
  {
    // NULL is also copied
    m_seconds = p_time.m_seconds;
    m_theTime = p_time.m_theTime;
  }
  return *this;
}

SQLTime&
SQLTime::operator=(const SQLTimestamp& p_timestamp)
{
  if(IsNull() || p_timestamp.IsNull())
  {
    SetNull();
  }
  else
  {
    SetTime(p_timestamp.Hour()
           ,p_timestamp.Minute()
           ,p_timestamp.Second());
  }
  return *this;
}

// Temporal operators

SQLInterval   
SQLTime::operator-(const SQLTime& p_time) const
{
  SQLInterval intval;

  // Test for NULL status
  if(IsNull() || p_time.IsNull())
  {
    return intval;
  }
  // Setting interval by the number of seconds
  intval.SetInterval(SQL_IS_HOUR_TO_SECOND,0,0,0,m_seconds - p_time.m_seconds,0);

  return intval;
}

SQLTimestamp  
SQLTime::operator+(const SQLDate& p_date) const
{
  SQLTimestamp stamp;

  // Test for NULL status
  if(IsNull() || p_date.IsNull())
  {
    return stamp;
  }
  // Setting the the timestamp
  stamp.SetTimestamp(p_date.Day()
                    ,p_date.Month()
                    ,p_date.Year()
                    ,Hour()
                    ,Minute()
                    ,Second());
  return stamp;
}

SQLTime
SQLTime::operator+(const SQLInterval& p_interval) const
{
  // Test for NULL status
  if(IsNull() || p_interval.IsNull())
  {
    SQLTime time;
    return time;
  }
  if(!p_interval.GetIsTimeType())
  {
    throw StdException(_T("Cannot add incompatible interval to time type"));
  }
  TimeValue value = m_seconds + p_interval.GetSeconds();
  SQLTime time((__int64) value);

  return time;
}

SQLTime
SQLTime::operator-(const SQLInterval& p_interval) const
{
  // Test for NULL status
  if(IsNull() || p_interval.IsNull())
  {
    SQLTime time;
    return time;
  }
  if(!p_interval.GetIsTimeType())
  {
    throw StdException(_T("Cannot add incompatible interval to time type"));
  }
  TimeValue value = m_seconds - p_interval.GetSeconds();
  SQLTime time((__int64)value);

  return time;
}

// End of namespace
}
