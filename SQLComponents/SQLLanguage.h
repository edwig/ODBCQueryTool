////////////////////////////////////////////////////////////////////////
//
// File: SQLLanguage.h
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

namespace SQLComponents
{

// Make some macro's (as if that's going to change!)
#define DAYS_IN_WEEK    7
#define MONTHS_IN_YEAR 12

typedef enum _language
{
   LN_DEFAULT  = -1
  ,LN_DUTCH    = 0
  ,LN_ENGLISH
  ,LN_GERMAN
  ,LN_FRENCH
  ,LN_NUMLANG // Array size
}
Language;

typedef enum _weekdays
{
   WD_SUNDAY = 0
  ,WD_MONDAY
  ,WD_TUESDAY
  ,WD_WEDNESDAY
  ,WD_THURSDAY
  ,WD_FRIDAY
  ,WD_SATURDAY
}
Weekday;

typedef enum _dateNames
{
   DN_TODAY   = 0    // TODAY   VANDAAG
  ,DN_CURRENT        // CURRENT HUIDIGMOMENT
  ,DN_CURRENTTIME    // CURRENT HUIDIGETIJD
  ,DN_NOW            // NOW     NU
  ,DN_FOM            // FOM     EVM      First Of Month    Monatsersten        Premier Jour du Mois
  ,DN_LOM            // LOM     LVM      Last  Of Month    Monatsletzten       Dernier Jour du Mois
  ,DN_FOY            // FOY     EVJ      First of Year     Erste des Jahres    Premier de l'annee
  ,DN_CY             // CY      HJ       Current  Year     Laufendes Jahr      Anee en cours
  ,DN_LOY            // LOY     LVJ      Last  Of Year     Letzte des Jahres   Dernier Jour de l'annee
  ,DN_SEC
  ,DN_SECOND
  ,DN_SECONDS
  ,DN_MIN
  ,DN_MINUTE
  ,DN_MINUTES
  ,DN_HOUR
  ,DN_HOURS
  ,DN_DAY
  ,DN_DAYS
  ,DN_WEEK
  ,DN_WEEKS
  ,DN_MONTH
  ,DN_MONTHS
  ,DN_YEAR
  ,DN_YEARS
  ,DN_LASTNAME       // Arraysize!
}
DateNames;

// STATIC DEFINITIONS OF NAME ARRAYS

// Default language
extern Language  g_defaultLanguage;
// Names of the week
extern LPCTSTR   g_weekdays[LN_NUMLANG][DAYS_IN_WEEK]; 
// Number of days at the beginning of the month + 2 months in the next year
extern const int g_daysInTheMonth[MONTHS_IN_YEAR + 2];
// Names of the months
extern LPCTSTR   g_monthnames[LN_NUMLANG][MONTHS_IN_YEAR];
// Short (3 char) variants of the month names
extern LPCTSTR   g_shortMonths[LN_NUMLANG][MONTHS_IN_YEAR]; 
// Several key names 
extern LPCTSTR   g_dateNames[LN_NUMLANG][DN_LASTNAME];

// Setting the default language
Language SetDefaultSQLLanguage(Language p_lang);

// End of namespace
}
