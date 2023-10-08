////////////////////////////////////////////////////////////////////////
//
// File: SQLInterval.cpp
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
#include "StdAfx.h"
#include "SQLComponents.h"
#include "SQLInterval.h"
#include "SQLDate.h"
#include "SQLTime.h"
#include "SQLDatabase.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{

// XTOR Clean NULL
SQLInterval::SQLInterval()
{
  SetNull();
}

// XTOR from a string
SQLInterval::SQLInterval(SQLINTERVAL p_type,const XString p_string)
{
  ParseInterval(p_type,p_string);
}

// XTOR from another interval
SQLInterval::SQLInterval(const SQLInterval& p_interval)
{
  memcpy(&m_interval,&p_interval.m_interval,sizeof(SQL_INTERVAL_STRUCT));
  m_value = p_interval.m_value;
}

// XTOR from an interval struct (possibly from a database)
SQLInterval::SQLInterval(const SQL_INTERVAL_STRUCT* p_interval)
{
  memcpy(&m_interval,p_interval,sizeof(SQL_INTERVAL_STRUCT));
  RecalculateValue();
}

// XTOR from a decimal day interval
// Possibly from a RDBMS that doesn't implement the INTERVAL datatype
SQLInterval::SQLInterval(SQLINTERVAL p_type,double p_databaseDouble)
{
  SetFromDatabaseDouble(p_type,p_databaseDouble);
}

// XTOR from a nanoseconds interval
SQLInterval::SQLInterval(SQLINTERVAL p_type,InterValue p_nanoseconds)
{
  SetInterval(p_type,p_nanoseconds);
}

// XTOR for year/year-month/month intervals
SQLInterval::SQLInterval(SQLINTERVAL p_type,int p_years,int p_months)
{
  SetInterval(p_type,p_years,p_months);
}

// XTOR for days -> seconds intervals
SQLInterval::SQLInterval(SQLINTERVAL p_type,int p_days,int p_hours,int p_minutes,int p_seconds,int p_fraction /*=0*/)
{
  SetInterval(p_type,p_days,p_hours,p_minutes,p_seconds,p_fraction);
}

// XTOR for a XML duration string
SQLInterval::SQLInterval(XString p_duration)
{
  SetInterval(p_duration);
}

// DTOR
SQLInterval::~SQLInterval()
{
}

//////////////////////////////////////////////////////////////////////////
//
// SETTERS
//
//////////////////////////////////////////////////////////////////////////

bool
SQLInterval::SetInterval(SQLINTERVAL p_type,const XString p_string)
{
  ParseInterval(p_type,p_string);
  return Valid();
}

bool    
SQLInterval::SetInterval(SQLINTERVAL p_type,InterValue p_nanoseconds)
{
  // Retain type
  m_interval.interval_type = p_type;
  // Recalculate the structure
  m_value = p_nanoseconds;
  RecalculateInterval();
  // Check negative
  if(p_nanoseconds < 0L)
  {
    m_value *= -1L;
    m_interval.interval_sign = SQL_TRUE;
  }
  return Valid();
}

bool    
SQLInterval::SetInterval(const SQL_INTERVAL_STRUCT& p_interval)
{
  memcpy(&m_interval,&p_interval,sizeof(SQL_INTERVAL_STRUCT));
  RecalculateValue();
  return Valid();
}

// Set from years-months
// Years/Months can be negative, to set negative interval
// Both parameters must be set to negative!
void
SQLInterval::SetInterval(SQLINTERVAL p_type,int p_years,int p_months)
{
  if(p_type == SQL_IS_YEAR  ||
     p_type == SQL_IS_MONTH ||
     p_type == SQL_IS_YEAR_TO_MONTH)
  {
    // Reinit
    memset(&m_interval,0,sizeof(SQL_INTERVAL_STRUCT));
    // Retain the type
    m_interval.interval_type = p_type;
    // Check for negative parameters
    if(p_years < 0 || p_months < 0)
    {
      m_interval.interval_sign = SQL_TRUE;
      if(p_years  < 0) p_years  *= -1;
      if(p_months < 0) p_months *= -1;
    }
    // Store the interval
    m_interval.intval.year_month.year  = p_years;
    m_interval.intval.year_month.month = p_months;
    Normalise();
    RecalculateValue();
  }
  else
  {
    throw StdException(_T("Cannot set day-second interval type from year-month value"));
  }
}

// Set from day-seconds
// Values can be negative, to set a negative interval
// All parameters must be positive or negative!
void
SQLInterval::SetInterval(SQLINTERVAL p_type,int p_days,int p_hours,int p_minutes,int p_seconds,int p_fraction /*=0*/)
{
  // Check for correct type
  if(p_type == SQL_IS_YEAR  ||
     p_type == SQL_IS_MONTH ||
     p_type == SQL_IS_YEAR_TO_MONTH)
  {
    throw StdException(_T("Cannot set year-month interval type from day-second value"));
  }
  // Re-init the interval
  memset(&m_interval,0,sizeof(SQL_INTERVAL_STRUCT));

  // Check for negative
  if(p_days < 0 || p_hours < 0 || p_minutes < 0 || p_seconds < 0 || p_fraction < 0)
  {
    m_interval.interval_sign = SQL_TRUE;
    if(p_days     < 0) p_days     *= -1;
    if(p_hours    < 0) p_hours    *= -1;
    if(p_minutes  < 0) p_minutes  *= -1;
    if(p_seconds  < 0) p_seconds  *= -1;
    if(p_fraction < 0) p_fraction *= -1;
  }
  // Retain the type
  m_interval.interval_type = p_type;
  // Set the scalars
  m_interval.intval.day_second.day      = p_days;
  m_interval.intval.day_second.hour     = p_hours;
  m_interval.intval.day_second.minute   = p_minutes;
  m_interval.intval.day_second.second   = p_seconds;
  m_interval.intval.day_second.fraction = p_fraction;
  // Normalise and set the structure
  Normalise();
  RecalculateValue();
}

void
SQLInterval::SetInterval(XString p_duration)
{
  ParseInterval(p_duration);
}

void    
SQLInterval::SetFromDatabaseDouble(SQLINTERVAL p_type,double p_number)
{
  // Re-init the interval
  memset(&m_interval,0,sizeof(SQL_INTERVAL_STRUCT));

  // Retain the type
  m_interval.interval_type = p_type;

  // Check for negativity
  if(p_number < 0.0)
  {
    p_number *= -1.0;
    m_interval.interval_sign = SQL_TRUE;
  }

  // Check for type
  if(p_type == SQL_IS_YEAR  ||
     p_type == SQL_IS_MONTH ||
     p_type == SQL_IS_YEAR_TO_MONTH)
  {
    m_value = static_cast<InterValue>(p_number);
  }
  else
  {
    m_value = static_cast<InterValue>((size_t)p_number * SECONDS_PER_DAY * NANOSECONDS_PER_SEC);
  }
  Normalise();
  RecalculateInterval();
}

// ONLY sets the fractional part
void    
SQLInterval::SetFractionPart(int p_fraction)
{
  m_interval.intval.day_second.fraction = p_fraction;
  RecalculateValue();
}

// Terse try to set the interval type.
// If other values present, bail out with a 'false' result
bool
SQLInterval::SetIntervalType(SQLINTERVAL p_type)
{
  switch(p_type)
  {
    case SQL_IS_YEAR:           if(m_interval.intval.year_month.month)
                                {
                                  return false;
                                }
                                break;
    case SQL_IS_MONTH:          if(m_interval.intval.year_month.year)
                                {
                                  return false;
                                }
                                break;
    case SQL_IS_YEAR_TO_MONTH:  break;
    case SQL_IS_DAY:            if(m_interval.intval.day_second.hour   ||
                                   m_interval.intval.day_second.minute ||
                                   m_interval.intval.day_second.second ||
                                   m_interval.intval.day_second.fraction)
                                {
                                  return false;
                                }
                                break;
    case SQL_IS_HOUR:           if(m_interval.intval.day_second.day    ||
                                   m_interval.intval.day_second.minute ||
                                   m_interval.intval.day_second.second ||
                                   m_interval.intval.day_second.fraction)
                                {
                                  return false;
                                }
                                break;
    case SQL_IS_MINUTE:         if(m_interval.intval.day_second.day    ||
                                   m_interval.intval.day_second.hour   ||
                                   m_interval.intval.day_second.second ||
                                   m_interval.intval.day_second.fraction)
                                {
                                  return false;
                                }
                                break;
    case SQL_IS_SECOND:         if(m_interval.intval.day_second.day    ||
                                   m_interval.intval.day_second.hour   ||
                                   m_interval.intval.day_second.minute )
                                {
                                  return false;
                                }
                                break;
    case SQL_IS_DAY_TO_HOUR:    if(m_interval.intval.day_second.minute ||
                                   m_interval.intval.day_second.second ||
                                   m_interval.intval.day_second.fraction)
                                {
                                  return false;
                                }
                                break;
    case SQL_IS_DAY_TO_MINUTE:  if(m_interval.intval.day_second.second ||
                                   m_interval.intval.day_second.fraction)
                                {
                                  return false;
                                }
                                break;
    case SQL_IS_DAY_TO_SECOND:  break;
    case SQL_IS_HOUR_TO_MINUTE: if(m_interval.intval.day_second.day    ||
                                   m_interval.intval.day_second.second ||
                                   m_interval.intval.day_second.fraction)
                                {
                                  return false;
                                }
                                break;
    case SQL_IS_HOUR_TO_SECOND: if(m_interval.intval.day_second.day    )
                                {
                                  return false;
                                }
                                break;
    case SQL_IS_MINUTE_TO_SECOND:if(m_interval.intval.day_second.day    ||
                                    m_interval.intval.day_second.hour   )
                                {
                                  return false;
                                }
                                break;
    default:                    // Wrong interval type
                                return false; 

  }
  m_interval.interval_type = p_type;
  return true;
}

int
SQLInterval::GetYears() const
{
  if(m_interval.interval_type == SQL_IS_YEAR ||
     m_interval.interval_type == SQL_IS_YEAR_TO_MONTH)
  {
    return m_interval.intval.year_month.year;
  }
  return 0;
}

int
SQLInterval::GetMonths() const
{
  if(GetIsYearMonthType())
  {
    int value = m_interval.intval.year_month.year * 12 +
                m_interval.intval.year_month.month;
    return m_interval.interval_sign ? -value : value;
  }
  return 0;
}

int
SQLInterval::GetDays() const
{
  if(m_interval.interval_type == SQL_IS_DAY           ||
     m_interval.interval_type == SQL_IS_DAY_TO_HOUR   ||
     m_interval.interval_type == SQL_IS_DAY_TO_MINUTE ||
     m_interval.interval_type == SQL_IS_DAY_TO_SECOND  )
  {
    int value = m_interval.intval.day_second.day;
    return m_interval.interval_sign ? -value : value;
  }
  return 0;
}

int
SQLInterval::GetHours() const
{
  if(GetIsDaySecondType())
  {
    int value = m_interval.intval.day_second.day * 24 +
                m_interval.intval.day_second.hour;
    return m_interval.interval_sign ? -value : value;
  }
  return 0;
}

int
SQLInterval::GetMinutes() const
{
  if(GetIsDaySecondType())
  {
    int value = m_interval.intval.day_second.day  * 60 * 24 +
                m_interval.intval.day_second.hour * 60 +
                m_interval.intval.day_second.minute;;
    return m_interval.interval_sign ? -value : value;
  }
  return 0;
}

int
SQLInterval::GetSeconds() const
{
  if(GetIsDaySecondType())
  {
    int value = m_interval.intval.day_second.day    * 60 * 60 * 24 +
                m_interval.intval.day_second.hour   * 60 * 60 +
                m_interval.intval.day_second.minute * 60 +
                m_interval.intval.day_second.second;
    return m_interval.interval_sign ? -value : value;
  }
  return 0;

}

int
SQLInterval::GetFractionPart() const
{
  if(m_interval.interval_type == SQL_IS_DAY_TO_SECOND    ||
     m_interval.interval_type == SQL_IS_HOUR_TO_SECOND   ||
     m_interval.interval_type == SQL_IS_MINUTE_TO_SECOND ||
     m_interval.interval_type == SQL_IS_SECOND            )
  {
    int value = m_interval.intval.day_second.fraction;
    return m_interval.interval_sign ? -value : value;
  }
  return 0;
}

XString 
SQLInterval::GetTypeAsString() const
{
  switch(m_interval.interval_type)
  {
    case SQL_IS_YEAR:             return _T("year");
    case SQL_IS_MONTH:            return _T("month");
    case SQL_IS_DAY:              return _T("day");
    case SQL_IS_HOUR:             return _T("hour");
    case SQL_IS_MINUTE:           return _T("minute");
    case SQL_IS_SECOND:           return _T("second");
    case SQL_IS_YEAR_TO_MONTH:    return _T("year to month");
    case SQL_IS_DAY_TO_HOUR:      return _T("day to hour");
    case SQL_IS_DAY_TO_MINUTE:    return _T("day to minute");
    case SQL_IS_DAY_TO_SECOND:    return _T("day to second");
    case SQL_IS_HOUR_TO_MINUTE:   return _T("hour to minute");
    case SQL_IS_HOUR_TO_SECOND:   return _T("hour to second");
    case SQL_IS_MINUTE_TO_SECOND: return _T("minute to second");
  }
  return _T("");
}

// Now very simple to do
// Returns months or nanoseconds!!
InterValue 
SQLInterval::AsValue() const
{
  return m_value;
}

// Databases like Oracle store a date interval
// in the form of decimal days (24 hours = 1.0000)
// Or just the number of months
double  
SQLInterval::AsDatabaseDouble() const
{
  // Year-month intervals
  if(GetIsYearMonthType())
  {
    return static_cast<double>(m_value);
  }

  // Return a decimal day value
  double value = static_cast<double>(m_value);
  value /= (double) NANOSECONDS_PER_SEC;
  value /= (double) SECONDS_PER_DAY;

  return value;
}

XString 
SQLInterval::AsString(bool p_withFraction /*= false*/) const
{
  XString string;

  // Check for NULL value
  if(IsNull())
  {
    return string;
  }

  // String depends on the interval type
  switch(m_interval.interval_type)
  {
    case SQL_IS_YEAR:             string.Format(_T("%d"),m_interval.intval.year_month.year); 
                                  break;
    case SQL_IS_MONTH:            string.Format(_T("%d"),m_interval.intval.year_month.month);
                                  break;
    case SQL_IS_DAY:              string.Format(_T("%d"),m_interval.intval.day_second.day);
                                  break;
    case SQL_IS_HOUR:             string.Format(_T("%d"),m_interval.intval.day_second.hour);
                                  break;
    case SQL_IS_MINUTE:           string.Format(_T("%d"),m_interval.intval.day_second.minute);
                                  break;
    case SQL_IS_SECOND:           string.Format(_T("%d"),m_interval.intval.day_second.second);
                                  break;
    case SQL_IS_YEAR_TO_MONTH:    string.Format(_T("%d %d"),m_interval.intval.year_month.year
                                                           ,m_interval.intval.year_month.month);
                                  break;
    case SQL_IS_DAY_TO_HOUR:      string.Format(_T("%d %d"),m_interval.intval.day_second.day
                                                           ,m_interval.intval.day_second.hour);
                                  break;
    case SQL_IS_DAY_TO_MINUTE:    string.Format(_T("%d %d:%d"),m_interval.intval.day_second.day
                                                              ,m_interval.intval.day_second.hour
                                                              ,m_interval.intval.day_second.minute);
                                  break;
    case SQL_IS_DAY_TO_SECOND:    string.Format(_T("%d %d:%d:%d"),m_interval.intval.day_second.day
                                                                 ,m_interval.intval.day_second.hour
                                                                 ,m_interval.intval.day_second.minute
                                                                 ,m_interval.intval.day_second.second);
                                  break;
    case SQL_IS_HOUR_TO_MINUTE:   string.Format(_T("%d:%d"),m_interval.intval.day_second.hour
                                                           ,m_interval.intval.day_second.minute);
                                  break;
    case SQL_IS_HOUR_TO_SECOND:   string.Format(_T("%d:%d:%d"),m_interval.intval.day_second.hour
                                                              ,m_interval.intval.day_second.minute
                                                              ,m_interval.intval.day_second.second);
                                  break;
    case SQL_IS_MINUTE_TO_SECOND: string.Format(_T("%d:%d"),m_interval.intval.day_second.minute
                                                           ,m_interval.intval.day_second.second);
                                  break;
    default:                      // NULL or unknown
                                  break;

  }
  // Optionally add a fraction
  if(p_withFraction)
  {
    if(m_interval.interval_type == SQL_IS_SECOND           ||
       m_interval.interval_type == SQL_IS_MINUTE_TO_SECOND ||
       m_interval.interval_type == SQL_IS_HOUR_TO_SECOND   ||
       m_interval.interval_type == SQL_IS_DAY_TO_SECOND)
    {
      string.AppendFormat(_T(".%09d"),m_interval.intval.day_second.fraction);
      string.TrimRight('0');
    }
  }

  // If necessary, add a minus sign
  if((m_interval.interval_type >= SQL_IS_YEAR &&
      m_interval.interval_type <= SQL_IS_MINUTE_TO_SECOND) &&
      m_interval.interval_sign == SQL_TRUE)
  {
    string = _T("-") + string;
  }
  return string;
}

// Currently implemented as a string
XString 
SQLInterval::AsXMLString(bool p_withFraction /*=false*/) const
{
  return AsString(p_withFraction);
}

// Currently implemented as a string with quotes
XString 
SQLInterval::AsSQLString(SQLDatabase* /*p_database*/,bool p_withFraction /*= false*/) const
{
  XString string = AsString(p_withFraction);
  return XString(_T("\'")) + string + _T("\'");
}

void    
SQLInterval::AsIntervalStruct(SQL_INTERVAL_STRUCT* p_struct) const
{
  memcpy(p_struct,&m_interval,sizeof(SQL_INTERVAL_STRUCT));
}

XString 
SQLInterval::AsXMLDuration() const
{
  XString duration;
  // Check if NULL
  if(IsNull())
  {
    return duration;
  }

  // Add sign and leading 'P'eriod marker
  duration = m_interval.interval_sign == SQL_TRUE ? _T("-P") : _T("P");

  if (m_interval.interval_type == SQL_IS_YEAR  ||
      m_interval.interval_type == SQL_IS_MONTH ||
      m_interval.interval_type == SQL_IS_YEAR_TO_MONTH)
  {
    // Add year-month-day part
    if (m_interval.intval.year_month.year)
    {
      duration.AppendFormat(_T("%dY"), m_interval.intval.year_month.year);
    }
    if (m_interval.intval.year_month.month)
    {
      duration.AppendFormat(_T("%dM"), m_interval.intval.year_month.month);
    }
  }
  else
  {
    if(m_interval.intval.day_second.day)
    {
      duration.AppendFormat(_T("%dD"),m_interval.intval.day_second.day);
    }

    // See if we must do the time part
    if(m_interval.intval.day_second.hour   ||
       m_interval.intval.day_second.minute ||
       m_interval.intval.day_second.second ||
       m_interval.intval.day_second.fraction)
    {
      duration += _T("T");

      if(m_interval.intval.day_second.hour)
      {
        duration.AppendFormat(_T("%dH"),m_interval.intval.day_second.hour);
      }
      if(m_interval.intval.day_second.minute)
      {
        duration.AppendFormat(_T("%dM"),m_interval.intval.day_second.minute);
      }
      if(m_interval.intval.day_second.second)
      {
        duration.AppendFormat(_T("%d"),m_interval.intval.day_second.second);

        if(m_interval.intval.day_second.fraction)
        {
          duration.AppendFormat(_T(".%09d"),m_interval.intval.day_second.fraction);
        }
        duration += _T("S");
      }
    }
  }
  return duration;
}

SQLINTERVAL 
SQLInterval::GetIntervalType() const
{
  return m_interval.interval_type;
}

int 
SQLInterval::GetSQLDatatype() const
{
  switch(m_interval.interval_type)
  {
    case SQL_IS_YEAR:             return SQL_C_INTERVAL_YEAR;
    case SQL_IS_MONTH:            return SQL_C_INTERVAL_MONTH;
    case SQL_IS_DAY:              return SQL_C_INTERVAL_DAY;
    case SQL_IS_HOUR:             return SQL_C_INTERVAL_HOUR;
    case SQL_IS_MINUTE:           return SQL_C_INTERVAL_MINUTE;
    case SQL_IS_SECOND:           return SQL_C_INTERVAL_SECOND;
    case SQL_IS_YEAR_TO_MONTH:    return SQL_C_INTERVAL_YEAR_TO_MONTH;
    case SQL_IS_DAY_TO_HOUR:      return SQL_C_INTERVAL_DAY_TO_HOUR;
    case SQL_IS_DAY_TO_MINUTE:    return SQL_C_INTERVAL_DAY_TO_MINUTE;
    case SQL_IS_DAY_TO_SECOND:    return SQL_C_INTERVAL_DAY_TO_SECOND;
    case SQL_IS_HOUR_TO_MINUTE:   return SQL_C_INTERVAL_HOUR_TO_MINUTE;
    case SQL_IS_HOUR_TO_SECOND:   return SQL_C_INTERVAL_HOUR_TO_SECOND;
    case SQL_IS_MINUTE_TO_SECOND: return SQL_C_INTERVAL_MINUTE_TO_SECOND;
  }
  return 0;
}

bool
SQLInterval::GetIsYearMonthType() const
{
  // distinguish between year-month and day-second intervals
  return (m_interval.interval_type == SQL_IS_MONTH       ||
          m_interval.interval_type == SQL_IS_YEAR        ||
          m_interval.interval_type == SQL_IS_YEAR_TO_MONTH);
}

bool
SQLInterval::GetIsDaySecondType() const
{
  // distinquish between year-month and day-second intervals
  return (m_interval.interval_type >= SQL_IS_DAY              &&
          m_interval.interval_type <= SQL_IS_MINUTE_TO_SECOND &&
          m_interval.interval_type != SQL_IS_YEAR_TO_MONTH);
}

bool
SQLInterval::GetIsTimeType() const
{
  return (m_interval.interval_type == SQL_IS_HOUR   ||
          m_interval.interval_type == SQL_IS_MINUTE ||
          m_interval.interval_type == SQL_IS_SECOND ||
          m_interval.interval_type == SQL_IS_HOUR_TO_MINUTE ||
          m_interval.interval_type == SQL_IS_HOUR_TO_SECOND ||
          m_interval.interval_type == SQL_IS_MINUTE_TO_SECOND);
}

bool
SQLInterval::GetIsNegative() const
{
  return m_interval.interval_sign == SQL_TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
// CALCULATIONS
// 
//////////////////////////////////////////////////////////////////////////

void
SQLInterval::AddYears(int p_years)
{
  if(m_interval.interval_type == SQL_IS_YEAR ||
     m_interval.interval_type == SQL_IS_YEAR_TO_MONTH)
  {
    m_interval.intval.year_month.year += p_years;
  }
  else if(m_interval.interval_type == SQL_IS_MONTH)
  {
    m_interval.intval.year_month.month += (p_years * 12);
  }
  else
  {
    throw StdException(_T("Cannot add years to a day-second interval"));
  }
  RecalculateValue();
}

void
SQLInterval::AddMonths(int p_months)
{
  if(m_interval.interval_type == SQL_IS_MONTH ||
     m_interval.interval_type == SQL_IS_YEAR_TO_MONTH)
  {
    m_interval.intval.year_month.month += p_months;
  }
  else if(m_interval.interval_type == SQL_IS_YEAR)
  {
    m_interval.intval.year_month.year += (p_months / 24);
  }
  else
  {
    throw StdException(_T("Cannot add months to a day-second interval"));
  }
  Normalise();
  RecalculateValue();
}

void    
SQLInterval::AddDays(int p_days)
{
  if(GetIsDaySecondType())
  {
    m_value += (InterValue)p_days * SECONDS_PER_DAY * NANOSECONDS_PER_SEC;
  }
  else
  {
    throw StdException(_T("Cannot add days to a year-month interval type"));
  }
  RecalculateInterval();
}


void
SQLInterval::AddHours(int p_hours)
{
  if(GetIsDaySecondType())
  {
    m_value += (InterValue)p_hours * SECONDS_PER_HOUR * NANOSECONDS_PER_SEC;
  }
  else
  {
    throw StdException(_T("Cannot add hours to a year-month interval type"));
  }
  RecalculateInterval();
}

void
SQLInterval::AddMinutes(int p_minutes)
{
  if(GetIsDaySecondType())
  {
    m_value += (InterValue)p_minutes * SECONDS_PER_MINUTE * NANOSECONDS_PER_SEC;
  }
  else
  {
    throw StdException(_T("Cannot add minutes to a year-month interval type"));
  }
  RecalculateInterval();
}

void
SQLInterval::AddSeconds(int p_seconds)
{
  if(GetIsDaySecondType())
  {
    m_value += (InterValue)p_seconds * NANOSECONDS_PER_SEC;
  }
  else
  {
    throw StdException(_T("Cannot add seconds to a year-month interval type"));
  }
  RecalculateInterval();
}

void
SQLInterval::AddFraction(int p_fraction)
{
  if(GetIsDaySecondType())
  {
    m_value += (InterValue)p_fraction;
  }
  else
  {
    throw StdException(_T("Cannot add a fraction to a year-month interval type"));
  }
  RecalculateInterval();
}

//////////////////////////////////////////////////////////////////////////
//
// NULL
//
//////////////////////////////////////////////////////////////////////////

// NULL Status
bool
SQLInterval::IsNull() const
{
  // Not an SQLINTERVAL value, but a NULL
  return (m_interval.interval_type == 0);
}

void
SQLInterval::SetNull()
{
  memset(&m_interval,0,sizeof(SQL_INTERVAL_STRUCT));
  m_value = 0L;
}

bool
SQLInterval::Valid() 
{
  // Check the SQLINTERVAL type (0 = NULL!)
  if(m_interval.interval_type < 0 ||
     m_interval.interval_type > SQL_IS_MINUTE_TO_SECOND)
  {
    return false;
  }
  // Check the sign
  if(m_interval.interval_sign < SQL_FALSE ||
     m_interval.interval_sign > SQL_TRUE  )
  {
    return false;
  }
  // Trick: By using my own interval type, this function
  // will return false, if non-valid fields are set in the
  // internal interval struct
  return SetIntervalType(m_interval.interval_type);
}

//////////////////////////////////////////////////////////////////////////
//
// PRIVATE INTERNALS
//
//////////////////////////////////////////////////////////////////////////

// Parsing from a string
bool
SQLInterval::ParseInterval(SQLINTERVAL p_type,const XString& p_string)
{
  XString string(p_string);
  bool retval   = true;
  bool negative = false;
  int  scannum  = 0;
  int  year     = 0;
  int  month    = 0;
  int  day      = 0;
  int  hour     = 0;
  int  minute   = 0;
  int  sec      = 0;
  // Allow for fractions also
  double seconds = 0.0;

  // Set the current interval to NULL
  SetNull();

  // Parse the negative sign
  string.Trim();
  if(string.Left(1) == _T("-"))
  {
    negative = true;
    string = string.Mid(1);
  }

  switch(p_type)
  {
    case SQL_IS_YEAR:             m_interval.intval.year_month.year = _ttoi(string);
                                  break;
    case SQL_IS_MONTH:            m_interval.intval.year_month.month = _ttoi(string);
                                  break;
    case SQL_IS_DAY:              m_interval.intval.day_second.day = _ttoi(string);
                                  break;
    case SQL_IS_HOUR:             m_interval.intval.day_second.hour = _ttoi(string);
                                  break;
    case SQL_IS_MINUTE:           m_interval.intval.day_second.minute = _ttoi(string);
                                  break;
    case SQL_IS_SECOND:           seconds = _ttof(string);
                                  // Nanosecond resolution
                                  sec = (int)floor(seconds);
                                  m_interval.intval.day_second.second   = (SQLUINTEGER)seconds;
                                  m_interval.intval.day_second.fraction = (SQLUINTEGER)(((seconds - (double)sec) * 1000000000.0) + 0.0000005);
                                  break;
    case SQL_IS_YEAR_TO_MONTH:    scannum = _stscanf_s(string,_T("%d %d"),&year,&month);
                                  if(scannum == 2)
                                  {
                                    m_interval.intval.year_month.year  = year;
                                    m_interval.intval.year_month.month = month;
                                            
                                  }
                                  else
                                  {
                                    retval = false;
                                  }
                                  break;
    case SQL_IS_DAY_TO_HOUR:      scannum = _stscanf_s(string,_T("%d %d"),&day,&hour);
                                  if(scannum == 2)
                                  {
                                    m_interval.intval.day_second.day  = day;
                                    m_interval.intval.day_second.hour = hour;
                                  }
                                  else
                                  {
                                    retval = false;
                                  }
                                  break;
    case SQL_IS_DAY_TO_MINUTE:    scannum = _stscanf_s(string,_T("%d %d:%d"),&day,&hour,&minute);
                                  if(scannum == 3)
                                  {
                                    m_interval.intval.day_second.day    = day;
                                    m_interval.intval.day_second.hour   = hour;
                                    m_interval.intval.day_second.minute = minute;
                                  }
                                  else
                                  {
                                    retval = false;
                                  }
                                  break;
    case SQL_IS_DAY_TO_SECOND:    scannum = _stscanf_s(string,_T("%d %d:%d:%lf"),&day,&hour,&minute,&seconds);
                                  if(scannum == 4)
                                  {
                                    m_interval.intval.day_second.day    = day;
                                    m_interval.intval.day_second.hour   = hour;
                                    m_interval.intval.day_second.minute = minute;
                                    // Nanosecond resolution
                                    sec = (int)floor(seconds);
                                    m_interval.intval.day_second.second   = (SQLUINTEGER)seconds;
                                    m_interval.intval.day_second.fraction = (SQLUINTEGER)(((seconds - (double)sec) * 1000000000.0) + 0.0000005);
                                  }
                                  else
                                  {
                                    retval = false;
                                  }
                                  break;
    case SQL_IS_HOUR_TO_MINUTE:   scannum = _stscanf_s(string,_T("%d:%d"),&hour,&minute);
                                  if(scannum == 2)
                                  {
                                    m_interval.intval.day_second.hour   = hour;
                                    m_interval.intval.day_second.minute = minute;
                                  }
                                  else
                                  {
                                    retval = false;
                                  }
                                  break;
    case SQL_IS_HOUR_TO_SECOND:   scannum = _stscanf_s(string,_T("%d:%d:%lf"),&hour,&minute,&seconds);
                                  if(scannum == 3)
                                  {
                                    m_interval.intval.day_second.hour   = hour;
                                    m_interval.intval.day_second.minute = minute;
                                    // Nanosecond resolution
                                    sec = (int)floor(seconds);
                                    m_interval.intval.day_second.second   = (SQLUINTEGER)seconds;
                                    m_interval.intval.day_second.fraction = (SQLUINTEGER)(((seconds - (double)sec) * 1000000000.0) + 0.0000005);
                                  }
                                  else
                                  {
                                    retval = false;
                                  }
                                  break;
    case SQL_IS_MINUTE_TO_SECOND: scannum = _stscanf_s(string,_T("%d:%lf"),&minute,&seconds);
                                  if(scannum == 2)
                                  {
                                    m_interval.intval.day_second.minute = minute;
                                    // Nanosecond resolution
                                    sec = (int)floor(seconds);
                                    m_interval.intval.day_second.second   = (SQLUINTEGER)seconds;
                                    m_interval.intval.day_second.fraction = (SQLUINTEGER)(((seconds - (double)sec) * 1000000000.0) + 0.0000005);
                                  }
                                  else
                                  {
                                    retval = false;
                                  }
                                  break;
  }

  // Retain the type and sign
  // Otherwise (invalid string) leave the value to NULL
  if(retval)
  {
    m_interval.interval_type = p_type;
    if(negative)
    {
      m_interval.interval_sign = SQL_TRUE;
    }
    Normalise();
    RecalculateValue();
  }
  return retval;
}

// Parse an interval from a XML duration string
// a la: http://www.w3.org/TR/2012/REC-xmlschema11-2-20120405/datatypes.html#duration
bool
SQLInterval::ParseInterval(XString p_duration)
{
  bool  negative    = false;
  bool  didTime     = false;
  int   value       = 0;
  int   fraction    = 0;
  TCHAR marker      = 0;
  TCHAR firstMarker = 0;
  TCHAR lastMarker  = 0;
  SQLINTERVAL type;

  // Set the current interval to NULL
  SetNull();

  // Parse the negative sign
  p_duration.Trim();
  if(p_duration.Left(1) == _T("-"))
  {
    negative = true;
    p_duration = p_duration.Mid(1);
  }

  // Must see a 'P' for period
  if(p_duration.Left(1) != 'P')
  {
    return false; // Leave interval at NULL
  }
  p_duration = p_duration.Mid(1);

  // Scan year/month/day/hour/min/second/fraction values
  while(ScanDurationValue(p_duration,value,fraction,marker,didTime))
  {
    switch(marker)
    {
      case 'Y': m_interval.intval.year_month.year = value;
                break;
      case 'D': m_interval.intval.day_second.day  = value;
                break;
      case 'H': if(!didTime)
                {
                  throw StdException(_T("Illegal duriation period (hours without a 'T')"));
                }
                m_interval.intval.day_second.hour = value;
                break;
      case 'M': if(didTime)
                {
                  m_interval.intval.day_second.minute = value;
                  marker = 'm'; // minutes!
                }
                else
                {
                  m_interval.intval.year_month.month = value;
                }
                break;
      case 'S': if(!didTime)
                {
                  throw StdException(_T("Illegal duration period (seconds without a 'T')"));
                }
                m_interval.intval.day_second.second   = value;
                m_interval.intval.day_second.fraction = fraction;
                break;
      default:  // Illegal string, leave interval at NULL
                return false;
    }
    // Getting first/last marker
    lastMarker = marker;
    if(firstMarker == 0)
    {
      firstMarker = marker;
    }
  }

  // Finding the interval type
       if(firstMarker == 'Y' && lastMarker == 'Y') type = SQL_IS_YEAR;
  else if(firstMarker == 'M' && lastMarker == 'M') type = SQL_IS_MONTH;
  else if(firstMarker == 'D' && lastMarker == 'D') type = SQL_IS_DAY;
  else if(firstMarker == 'H' && lastMarker == 'H') type = SQL_IS_HOUR;
  else if(firstMarker == 'm' && lastMarker == 'm') type = SQL_IS_MINUTE;
  else if(firstMarker == 'S' && lastMarker == 'S') type = SQL_IS_SECOND;
  else if(firstMarker == 'Y' && lastMarker == 'M') type = SQL_IS_YEAR_TO_MONTH;
  else if(firstMarker == 'D' && lastMarker == 'H') type = SQL_IS_DAY_TO_HOUR;
  else if(firstMarker == 'D' && lastMarker == 'm') type = SQL_IS_DAY_TO_MINUTE;
  else if(firstMarker == 'D' && lastMarker == 'S') type = SQL_IS_DAY_TO_SECOND;
  else if(firstMarker == 'H' && lastMarker == 'm') type = SQL_IS_HOUR_TO_MINUTE;
  else if(firstMarker == 'H' && lastMarker == 'S') type = SQL_IS_HOUR_TO_SECOND;
  else if(firstMarker == 'm' && lastMarker == 'S') type = SQL_IS_MINUTE_TO_SECOND;
  else
  {
    // Beware: XML duration has combinations that are NOT compatible
    // with the SQL definition of an interval, like Month-to-Day
    XString error;
    error.Format(_T("XML duration period not compatible with SQL (%c to %c)"),firstMarker,lastMarker);
    throw StdException(error);
  }

  // Found everything: wrap up
  m_interval.interval_type = type;
  if(negative)
  {
    m_interval.interval_sign = SQL_TRUE;
  }
  Normalise();
  RecalculateValue();

  return true;
}

bool
SQLInterval::ScanDurationValue(XString& p_duration
                              ,int&     p_value
                              ,int&     p_fraction
                              ,TCHAR&   p_marker
                              ,bool&    p_didTime)
{
  // Reset values
  p_value  = 0;
  p_marker = 0;
  bool found = false;

  // Check for empty string
  if(p_duration.IsEmpty())
  {
    return false;
  }

  // Scan for beginning of time part
  if(p_duration.GetAt(0) == 'T')
  {
    p_didTime  = true;
    p_duration = p_duration.Mid(1);
  }

  // Scan a number
  while(isdigit(p_duration.GetAt(0)))
  {
    found = true;
    p_value *= 10;
    p_value += p_duration.GetAt(0) - '0';
    p_duration = p_duration.Mid(1);
  }

  if(p_duration.GetAt(0) == '.')
  {
    p_duration = p_duration.Mid(1);

    int frac = 9;
    while(isdigit(p_duration.GetAt(0)))
    {
      --frac;
      p_fraction *= 10;
      p_fraction += p_duration.GetAt(0) - '0';
      p_duration  = p_duration.Mid(1);
    }
    p_fraction *= (int) pow(10,frac);
  }

  // Scan a marker
  if(isalpha(p_duration.GetAt(0)))
  {
    p_marker = p_duration.GetAt(0);
    p_duration = p_duration.Mid(1);
  }

  // True if both found, and fraction only found for seconds
  return (p_fraction && p_marker == 'S') ||
         (p_fraction == 0 && found && p_marker > 0);
}

// Recalculate the total interval value
void 
SQLInterval::RecalculateValue()
{
  if(m_interval.interval_type == SQL_IS_YEAR  ||
     m_interval.interval_type == SQL_IS_MONTH ||
     m_interval.interval_type == SQL_IS_YEAR_TO_MONTH)
  {
    m_value = (InterValue)m_interval.intval.year_month.year * 24 +
              (InterValue)m_interval.intval.year_month.month;

  }
  else if(m_interval.interval_type > 0 &&
          m_interval.interval_type <= SQL_IS_MINUTE_TO_SECOND)
  {
    m_value = (InterValue)m_interval.intval.day_second.day    * NANOSECONDS_PER_SEC * SECONDS_PER_DAY  +
              (InterValue)m_interval.intval.day_second.hour   * NANOSECONDS_PER_SEC * SECONDS_PER_HOUR +
              (InterValue)m_interval.intval.day_second.minute * NANOSECONDS_PER_SEC * SECONDS_PER_MINUTE +
              (InterValue)m_interval.intval.day_second.second * NANOSECONDS_PER_SEC +
              (InterValue)m_interval.intval.day_second.fraction;
  }
  else
  {
    m_value = 0L;
  }
  if(m_interval.interval_sign)
  {
    m_value *= -1L;
  }
}

// Normalise the field values
void 
SQLInterval::Normalise()
{
  switch(m_interval.interval_type)
  {
    case SQL_IS_YEAR:             break; // Nothing to do
    case SQL_IS_MONTH:            break; // Nothing to do
    case SQL_IS_DAY:              break; // Nothing to do
    case SQL_IS_HOUR:             break; // Nothing to do
    case SQL_IS_MINUTE:           break; // Nothing to do
    case SQL_IS_SECOND:           if(m_interval.intval.day_second.fraction > NANOSECONDS_PER_SEC)
                                  {
                                    m_interval.intval.day_second.second   += m_interval.intval.day_second.fraction / NANOSECONDS_PER_SEC;
                                    m_interval.intval.day_second.fraction %= NANOSECONDS_PER_SEC;
                                  }
                                  break;
    case SQL_IS_YEAR_TO_MONTH:    if(m_interval.intval.year_month.month > 12)
                                  {
                                    m_interval.intval.year_month.year  += m_interval.intval.year_month.month / 12;
                                    m_interval.intval.year_month.month %= 12;
                                  }
                                  break;
    case SQL_IS_DAY_TO_HOUR:      if(m_interval.intval.day_second.hour > 24)
                                  {
                                    m_interval.intval.day_second.day  += m_interval.intval.day_second.hour / 24;
                                    m_interval.intval.day_second.hour %= 24;
                                  }
                                  break;
    case SQL_IS_DAY_TO_MINUTE:    if(m_interval.intval.day_second.minute > 60)
                                  {
                                    m_interval.intval.day_second.hour   += m_interval.intval.day_second.minute / 60;
                                    m_interval.intval.day_second.minute %= 60;
                                  }
                                  if(m_interval.intval.day_second.hour > 24)
                                  {
                                    m_interval.intval.day_second.day  += m_interval.intval.day_second.hour / 24;
                                    m_interval.intval.day_second.hour %= 24;
                                  }
                                  break;
    case SQL_IS_DAY_TO_SECOND:    if(m_interval.intval.day_second.fraction > NANOSECONDS_PER_SEC)
                                  {
                                    m_interval.intval.day_second.second   += m_interval.intval.day_second.fraction / NANOSECONDS_PER_SEC;
                                    m_interval.intval.day_second.fraction %= NANOSECONDS_PER_SEC;
                                  }
                                  if(m_interval.intval.day_second.second > 60)
                                  {
                                    m_interval.intval.day_second.minute += m_interval.intval.day_second.second / 60;
                                    m_interval.intval.day_second.second %= 60;
                                  }
                                  if(m_interval.intval.day_second.minute > 60)
                                  {
                                    m_interval.intval.day_second.hour   += m_interval.intval.day_second.minute / 60;
                                    m_interval.intval.day_second.minute %= 60;
                                  }
                                  if(m_interval.intval.day_second.hour > 24)
                                  {
                                    m_interval.intval.day_second.day  += m_interval.intval.day_second.hour / 24;
                                    m_interval.intval.day_second.hour %= 24;
                                  }
                                  break;
    case SQL_IS_HOUR_TO_MINUTE:   if(m_interval.intval.day_second.minute > 60)
                                  {
                                    m_interval.intval.day_second.hour   += m_interval.intval.day_second.minute / 60;
                                    m_interval.intval.day_second.minute %= 60;
                                  }
                                  break;
    case SQL_IS_HOUR_TO_SECOND:   if(m_interval.intval.day_second.fraction > NANOSECONDS_PER_SEC)
                                  {
                                    m_interval.intval.day_second.second   += m_interval.intval.day_second.fraction / NANOSECONDS_PER_SEC;
                                    m_interval.intval.day_second.fraction %= NANOSECONDS_PER_SEC;
                                  }
                                  if(m_interval.intval.day_second.second > 60)
                                  {
                                    m_interval.intval.day_second.minute += m_interval.intval.day_second.second / 60;
                                    m_interval.intval.day_second.second %= 60;
                                  }
                                  if(m_interval.intval.day_second.minute > 60)
                                  {
                                    m_interval.intval.day_second.hour   += m_interval.intval.day_second.minute / 60;
                                    m_interval.intval.day_second.minute %= 60;
                                  }
                                  break;
    case SQL_IS_MINUTE_TO_SECOND: if(m_interval.intval.day_second.fraction > NANOSECONDS_PER_SEC)
                                  {
                                    m_interval.intval.day_second.second   += m_interval.intval.day_second.fraction / NANOSECONDS_PER_SEC;
                                    m_interval.intval.day_second.fraction %= NANOSECONDS_PER_SEC;
                                  }
                                  if(m_interval.intval.day_second.second > 60)
                                  {
                                    m_interval.intval.day_second.minute += m_interval.intval.day_second.second / 60;
                                    m_interval.intval.day_second.second %= 60;
                                  }
                                  break;
    default:                      break;
  }
}

// Parse value back to m_interval struct
void 
SQLInterval::RecalculateInterval()
{
  InterValue  value(m_value);
  SQLINTERVAL type(m_interval.interval_type);

  // Reset the interval
  memset(&m_interval,0,sizeof(SQL_INTERVAL_STRUCT));
  m_interval.interval_type = type;

  // See if negative
  if(value < 0L)
  {
    m_interval.interval_sign = SQL_TRUE;
    value *= -1L;
  }

  // Calculate per type
  switch(type)
  {
    case SQL_IS_YEAR:             m_interval.intval.year_month.year     = (SQLUINTEGER) value;
                                  break;
    case SQL_IS_MONTH:            m_interval.intval.year_month.month    = (SQLUINTEGER) value;
                                  break;
    case SQL_IS_DAY:              m_interval.intval.day_second.day      = (SQLUINTEGER) (value / NANOSECONDS_PER_SEC / SECONDS_PER_DAY);
                                  break;
    case SQL_IS_HOUR:             m_interval.intval.day_second.hour     = (SQLUINTEGER) (value / NANOSECONDS_PER_SEC / SECONDS_PER_HOUR);
                                  break;
    case SQL_IS_MINUTE:           m_interval.intval.day_second.minute   = (SQLUINTEGER) (value / NANOSECONDS_PER_SEC / SECONDS_PER_MINUTE);
                                  break;
    case SQL_IS_SECOND:           m_interval.intval.day_second.second   = (SQLUINTEGER) (value / NANOSECONDS_PER_SEC);
                                  m_interval.intval.day_second.fraction = (SQLUINTEGER) (value % NANOSECONDS_PER_SEC);
                                  break;
    case SQL_IS_YEAR_TO_MONTH:    m_interval.intval.year_month.month    = (SQLUINTEGER) value;
                                  Normalise();
                                  break;
    case SQL_IS_DAY_TO_HOUR:      m_interval.intval.day_second.hour     = (SQLUINTEGER) (value / NANOSECONDS_PER_SEC / SECONDS_PER_HOUR);
                                  Normalise();
                                  break;
    case SQL_IS_DAY_TO_MINUTE:    m_interval.intval.day_second.minute   = (SQLUINTEGER) (value / NANOSECONDS_PER_SEC / SECONDS_PER_MINUTE);
                                  Normalise();
                                  break;
    case SQL_IS_DAY_TO_SECOND:    m_interval.intval.day_second.second   = (SQLUINTEGER) (value / NANOSECONDS_PER_SEC);
                                  m_interval.intval.day_second.fraction = (SQLUINTEGER) (value % NANOSECONDS_PER_SEC);
                                  Normalise();
                                  break;
    case SQL_IS_HOUR_TO_MINUTE:   m_interval.intval.day_second.minute   = (SQLUINTEGER) (value / NANOSECONDS_PER_SEC / SECONDS_PER_MINUTE);
                                  Normalise();
                                  break;
    case SQL_IS_HOUR_TO_SECOND:   m_interval.intval.day_second.second   = (SQLUINTEGER) (value / NANOSECONDS_PER_SEC);
                                  m_interval.intval.day_second.fraction = (SQLUINTEGER) (value % NANOSECONDS_PER_SEC);
                                  Normalise();
                                  break;
    case SQL_IS_MINUTE_TO_SECOND: m_interval.intval.day_second.second   = (SQLUINTEGER) (value / NANOSECONDS_PER_SEC);
                                  m_interval.intval.day_second.fraction = (SQLUINTEGER) (value % NANOSECONDS_PER_SEC);
                                  Normalise();
                                  break;
  }
}

//////////////////////////////////////////////////////////////////////////
//
// OPERATORS
//
//////////////////////////////////////////////////////////////////////////
 
// Assignment of the interval

SQLInterval&  
SQLInterval::operator=(const SQLInterval& p_interval)
{
  if(this != &p_interval)
  {
    // Copy everything including NULL status
    memcpy(&m_interval,&p_interval.m_interval,sizeof(SQL_INTERVAL_STRUCT));
    m_value = p_interval.m_value;
  }
  return *this;
}

// Temporal operators

SQLDate
SQLInterval::operator+(const SQLDate& p_date) const
{
  // Check NULL status
  if(IsNull() || p_date.IsNull())
  {
    SQLDate date;
    return date;
  }

  // Getting the sign
  int sign = GetIsNegative() ? -1 : 1;

  // Check type  
  if(GetIsYearMonthType())
  {
    // Year-month interval
    SQLDate date(p_date);
    date.AddYears (sign * m_interval.intval.year_month.year);
    date.AddMonths(sign * m_interval.intval.year_month.month);
    return date;
  }
  else
  {
    // Day-second interval
    DateValue value = sign * (p_date.AsNumber() + static_cast<long>(m_value / NANOSECONDS_PER_SEC / SECONDS_PER_DAY));
    SQLDate date(value);
    return date;
  }
}

SQLTime
SQLInterval::operator+(const SQLTime& p_time) const
{
  // Check for NULL
  if(IsNull() || p_time.IsNull())
  {
    SQLTime time;
    return time;
  }
  // Check type  
  if(GetIsTimeType())
  {
    TimeValue value = p_time.AsNumber();
    value += static_cast<long>(m_value / NANOSECONDS_PER_SEC);
    return SQLTime(value);
  }
  else
  {
    throw StdException("Cannot add a time and and non-time interval");
  }
}

SQLTimestamp
SQLInterval::operator+(const SQLTimestamp& p_stamp) const
{
  // Check for NULL state
  if(IsNull() || p_stamp.IsNull())
  {
    SQLTimestamp stamp;
    return stamp;
  }

  // Getting the sign
  int sign = GetIsNegative() ? -1 : 1;

  // Check type  
  if(GetIsYearMonthType())
  {
    // Year-month intervals
    SQLTimestamp stamp(p_stamp);
    stamp.AddYears (sign * m_interval.intval.year_month.year);
    stamp.AddMonths(sign * m_interval.intval.year_month.month);
    return stamp;
  }
  else
  {
    // Day-second intervals  
    StampValue value = (m_value / NANOSECONDS_PER_SEC) + p_stamp.Value();
    value *= sign;
    int fraction = m_value % NANOSECONDS_PER_SEC;
    SQLTimestamp stamp(value,abs(fraction));
    return stamp;
  }
}

SQLInterval
SQLInterval::operator+(const SQLInterval& p_interval) const
{
  // Check for NULL state
  if(IsNull() || p_interval.IsNull())
  {
    SQLInterval intval;
    return intval;
  }

  // Check both sides
  if(GetIsYearMonthType() && p_interval.GetIsYearMonthType())
  {
    InterValue value = m_value + p_interval.m_value;
    SQLInterval val(m_interval.interval_type,0,(int)value);
    return val;
  }
  else if(GetIsDaySecondType() && p_interval.GetIsDaySecondType())
  {
    InterValue value = m_value - p_interval.m_value;
    SQLInterval intval(m_interval.interval_type,value);
    return intval;
  }
  else
  {
    throw StdException("Cannot add two ordinal different intervals");
  }
}

SQLInterval   
SQLInterval::operator-(const SQLInterval& p_interval) const
{
  // Check for NULL state
  if(IsNull() || p_interval.IsNull())
  {
    SQLInterval intval;
    return intval;
  }

  // Check both sides
  if(GetIsYearMonthType() && p_interval.GetIsYearMonthType())
  {
    InterValue value = m_value - p_interval.m_value;
    SQLInterval val(m_interval.interval_type,0,(int) value);
    return val;
  }
  else if(GetIsDaySecondType() && p_interval.GetIsDaySecondType())
  {
    InterValue value = m_value - p_interval.m_value;
    SQLInterval intval(m_interval.interval_type,value);
    return intval;
  }
  else
  {
    throw StdException(_T("Cannot subtract two ordinal different intervals"));
  }
}


// Scalar sizing of the interval

SQLInterval   
SQLInterval::operator*(int p_number) const
{
  // Check for NULL status
  if(IsNull())
  {
    SQLInterval intval;
    return intval;
  }
  // Calculate new value
  InterValue value = AsValue() * static_cast<InterValue>(p_number);
  SQLInterval interval;
  interval.SetIntervalType(GetIntervalType());
  interval.m_value = value;
  interval.RecalculateInterval();

  return interval;
}

SQLInterval
SQLInterval::operator*(double p_number) const
{
  // Check for NULL status
  if(IsNull())
  {
    SQLInterval intval;
    return intval;
  }
  // Calculate new value
  InterValue value = AsValue() * static_cast<InterValue>(p_number);
  SQLInterval interval;
  interval.SetIntervalType(GetIntervalType());
  interval.m_value = value;
  interval.RecalculateInterval();

  return interval;
}

SQLInterval   
SQLInterval::operator/(int p_number) const
{
  // Check for NULL status
  if(IsNull())
  {
    SQLInterval intval;
    return intval;
  }
  // Calculate new value
  InterValue value = AsValue() / static_cast<InterValue>(p_number);
  SQLInterval interval;
  interval.SetIntervalType(GetIntervalType());
  interval.m_value = value;
  interval.RecalculateInterval();

  return interval;
}

SQLInterval
SQLInterval::operator/(double p_number) const
{
  // Check for NULL status
  if(IsNull())
  {
    SQLInterval intval;
    return intval;
  }
  // Calculate new value
  InterValue value = AsValue() / static_cast<InterValue>(p_number);
  SQLInterval interval;
  interval.SetIntervalType(GetIntervalType());
  interval.m_value = value;
  interval.RecalculateInterval();

  return interval;
}

SQLInterval
SQLInterval::operator%(int p_number) const
{
  // Check for NULL status
  if(IsNull())
  {
    SQLInterval intval;
    return intval;
  }
  // Calculate new value
  InterValue value = AsValue() % static_cast<InterValue>(p_number);
  SQLInterval interval;
  interval.SetIntervalType(GetIntervalType());
  interval.m_value = value;
  interval.RecalculateInterval();

  return interval;
}

SQLInterval
SQLInterval::operator%(double p_number) const
{
  // Check for NULL status
  if(IsNull())
  {
    SQLInterval intval;
    return intval;
  }
  // Calculate new value
  InterValue value = AsValue() % static_cast<InterValue>(p_number);
  SQLInterval interval;
  interval.SetIntervalType(GetIntervalType());
  interval.m_value = value;
  interval.RecalculateInterval();

  return interval;
}

SQLInterval&
SQLInterval::operator*=(int p_number)
{
  if(!IsNull())
  {
    m_value *= static_cast<InterValue>(p_number);
    RecalculateInterval();
  }
  return *this;
}

SQLInterval&
SQLInterval::operator*=(double p_number)
{
  if(!IsNull())
  {
    m_value = static_cast<InterValue>(static_cast<double>(m_value) * p_number);
    RecalculateInterval();
  }
  return *this;
}

SQLInterval&
SQLInterval::operator/=(int p_number)
{
  if(!IsNull())
  {
    m_value /= static_cast<InterValue>(p_number);
    RecalculateInterval();
  }
  return *this;
}

SQLInterval&
SQLInterval::operator/=(double p_number)
{
  if(!IsNull())
  {
    m_value = static_cast<InterValue>(static_cast<double>(m_value) / p_number);
    RecalculateInterval();
  }
  return *this;
}

// Comparison to other intervals
bool
SQLInterval::operator==(const SQLInterval& p_interval) const
{
  // Check for NULL status
  if(IsNull() || p_interval.IsNull())
  {
    return false;
  }
  if((GetIsYearMonthType() && p_interval.GetIsYearMonthType()) ||
     (GetIsDaySecondType() && p_interval.GetIsDaySecondType()))
  {
    return m_value == p_interval.m_value;
  }
  // Incompatible ordinal types
  return false;
}

bool
SQLInterval::operator!=(const SQLInterval& p_interval) const
{
  // Check for NULL status
  if(IsNull() || p_interval.IsNull())
  {
    return true;
  }
  if((GetIsYearMonthType() && p_interval.GetIsYearMonthType()) ||
     (GetIsDaySecondType() && p_interval.GetIsDaySecondType()))
  {
    return m_value != p_interval.m_value;
  }
  // Incompatible ordinal types
  return true;
}

bool
SQLInterval::operator>(const SQLInterval& p_interval) const
{
  // Check for NULL status
  if(IsNull() || p_interval.IsNull())
  {
    return false;
  }
  if((GetIsYearMonthType() && p_interval.GetIsYearMonthType()) ||
     (GetIsDaySecondType() && p_interval.GetIsDaySecondType()))
  {
    return m_value > p_interval.m_value;
  }
  // Incompatible ordinal types
  return false;
}

bool
SQLInterval::operator>=(const SQLInterval& p_interval) const
{
  // Check for NULL status
  if(IsNull() || p_interval.IsNull())
  {
    return false;
  }
  if((GetIsYearMonthType() && p_interval.GetIsYearMonthType()) ||
     (GetIsDaySecondType() && p_interval.GetIsDaySecondType()))
  {
    return m_value >= p_interval.m_value;
  }
  // Incompatible ordinal types
  return false;
}

bool
SQLInterval::operator<(const SQLInterval& p_interval) const
{
  // Check for NULL status
  if(IsNull() || p_interval.IsNull())
  {
    return false;
  }
  if((GetIsYearMonthType() && p_interval.GetIsYearMonthType()) ||
     (GetIsDaySecondType() && p_interval.GetIsDaySecondType()))
  {
    return m_value < p_interval.m_value;
  }
  // Incompatible ordinal types
  return false;
}

bool
SQLInterval::operator<=(const SQLInterval& p_interval) const
{
  // Check for NULL status
  if(IsNull() || p_interval.IsNull())
  {
    return false;
  }
  if((GetIsYearMonthType() && p_interval.GetIsYearMonthType()) ||
     (GetIsDaySecondType() && p_interval.GetIsDaySecondType()))
  {
    return m_value <= p_interval.m_value;
  }
  // Incompatible ordinal types
  return false;
}

// End of namespace
}
