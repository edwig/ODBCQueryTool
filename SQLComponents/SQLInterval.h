////////////////////////////////////////////////////////////////////////
//
// File: SQLInterval.h
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

class SQLDate;
class SQLTime;
class SQLTimestamp;
class SQLDatabase;

// Interval is stored internally as a total number of seconds
typedef __int64 InterValue;

class SQLInterval
{
public:
  // Interval constructed as NULL interval
  SQLInterval();

  // Interval constructed from another interval
  SQLInterval(const SQLInterval& p_interval);

  // Interval constructed from an ODBC structure
  SQLInterval(SQL_INTERVAL_STRUCT* p_interval);

  // Interval constructed from a string
  SQLInterval(SQLINTERVAL p_type,const XString p_string);

  // Interval constructed from a database DECIMAL (Oracle etc)
  SQLInterval(SQLINTERVAL p_type,double p_databaseDouble);

  // Interval constructed from an internal value (other temporal type)
  SQLInterval(SQLINTERVAL p_type,InterValue p_nanoseconds);

  // Interval constructed from scalars (year-month)
  SQLInterval(SQLINTERVAL p_type,int p_years,int p_months);

  // Interval constructed from scalars (day-second)
  SQLInterval(SQLINTERVAL p_type,int p_days, int p_hours,int p_minutes,int p_seconds,int p_fraction = 0);

  // Interval constructed from a XML duration string
  SQLInterval(XString p_duration);

  // Destructor
 ~SQLInterval();

  // SETTERS

  // Setting an interval from a string
  bool    SetInterval(SQLINTERVAL p_type,const XString p_string);
  // Setting an interval from an internal value
  bool    SetInterval(SQLINTERVAL p_type,InterValue p_nanoseconds);
  // Setting an interval from an ODBC structure
  bool    SetInterval(SQL_INTERVAL_STRUCT& p_interval);
  // Setting from a (year,month) pair. If either negative -> interval is negative.
  void    SetInterval(SQLINTERVAL p_type,int p_years,int p_months);
  // Setting from a (day,hour,min,sec,frac) set. If either is negative -> interval is negative
  void    SetInterval(SQLINTERVAL p_type,int p_days,int p_hours,int p_minutes,int p_seconds,int p_fraction = 0);
  // Setting from a database double (possibly Oracle)
  void    SetFromDatabaseDouble(SQLINTERVAL p_type,double p_number);
  // ONLY set the fraction part as a correction
  void    SetFractionPart(int p_fraction);
  // Try to set the interval type, depends on the internal values
  bool    SetIntervalType(SQLINTERVAL p_type);
  // Set from an XML duration string
  void    SetInterval(XString p_duration);

  // GETTERS

  int     GetYears() const;
  int     GetMonths() const;
  int     GetDays() const;
  int     GetHours() const;
  int     GetMinutes() const;
  int     GetSeconds() const;
  int     GetFractionPart() const;
  XString GetTypeAsString() const;

  double  AsDatabaseDouble() const;
  XString AsString(bool p_withFraction = false) const;
  XString AsXMLString(bool p_withFraction = false) const;
  XString AsXMLDuration() const;
  XString AsSQLString(SQLDatabase* p_database,bool p_withFraction = false) const;
  void    AsIntervalStruct(SQL_INTERVAL_STRUCT* p_struct) const;

  // CALCULATIONS

  void    AddYears   (int p_years);
  void    AddMonths  (int p_months);
  void    AddDays    (int p_days);
  void    AddHours   (int p_hours);
  void    AddMinutes (int p_minutes);
  void    AddSeconds (int p_seconds);
  void    AddFraction(int p_fraction);

  // OPERATORS

  // Assignment of the interval
  SQLInterval&  operator= (const SQLInterval& p_interval);
  // Adding to temporal types
  SQLDate       operator+ (const SQLDate& p_date) const;
  SQLTime       operator+ (const SQLTime& p_time) const;
  SQLTimestamp  operator+ (const SQLTimestamp& p_stamp) const;
  SQLInterval   operator+ (const SQLInterval& p_interval) const;
  SQLInterval   operator- (const SQLInterval& p_interval) const;
  // Scalar sizing of the interval
  SQLInterval   operator* (int    p_number) const;
  SQLInterval   operator* (double p_number) const;
  SQLInterval   operator/ (int    p_number) const;
  SQLInterval   operator/ (double p_number) const;
  SQLInterval   operator% (int    p_number) const;
  SQLInterval   operator% (double p_number) const;
  SQLInterval&  operator*=(int    p_number);
  SQLInterval&  operator*=(double p_number);
  SQLInterval&  operator/=(int    p_number);
  SQLInterval&  operator/=(double p_number);
  // Comparison to other intervals
  bool          operator==(const SQLInterval& p_interval) const;
  bool          operator!=(const SQLInterval& p_interval) const;
  bool          operator >(const SQLInterval& p_interval) const;
  bool          operator>=(const SQLInterval& p_interval) const;
  bool          operator <(const SQLInterval& p_interval) const;
  bool          operator<=(const SQLInterval& p_interval) const;

  // NULL Status
  bool IsNull()  const;
  bool Valid();
  void SetNull();

  // Internals
  InterValue  AsValue() const;
  SQLINTERVAL GetIntervalType() const;
  int         GetSQLDatatype()  const;
  bool        GetIsYearMonthType() const;
  bool        GetIsDaySecondType() const;
  bool        GetIsTimeType() const;
  bool        GetIsNegative() const;

private:
  // Parsing from a (database) string
  bool ParseInterval(SQLINTERVAL p_type,const XString& p_string);
  // Parsing from a XML duration string
  bool ParseInterval(XString p_duration);
  // Parsing/scanning one value of a XML duration string
  bool ScanDurationValue(XString& p_duraction,int& p_value,int& p_fraction,char& p_marker,bool& p_didTime);

  // Recalculate the total interval value
  void RecalculateValue();
  // Normalise the field values
  void Normalise();
  // Parse value back to m_interval struct
  void RecalculateInterval();

  // The one and only interval
  SQL_INTERVAL_STRUCT m_interval;
  // The total interval
  // YEAR-MONTH -> Number of months!!
  // DAY-SECOND -> Number of nanoseconds
  InterValue m_value;
};

// End of namespace
}