////////////////////////////////////////////////////////////////////////
//
// File: SQLLanguage.h
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
#include "SQLLanguage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{

// Default language now English and not Dutch
Language g_defaultLanguage = LN_ENGLISH;

// All the days of the week 
const char* g_weekdays[LN_NUMLANG][DAYS_IN_WEEK] = 
{
   { "zondag",  "maandag", "dinsdag", "woensdag", "donderdag", "vrijdag", "zaterdag" }
  ,{ "sunday",  "monday",  "tuesday", "wednesday","thursday",  "friday",  "saturday" }
  ,{ "Sonntag", "Montag",  "Dienstag","Mittwoch", "Donnerstag","Freitag", "Samstag"  }
  ,{ "dimanche","lundi",   "mardi",   "mercredi", "jeudi",     "venvredi","samedi"   }
};

// All the months in the year
const char* g_monthnames[LN_NUMLANG][MONTHS_IN_YEAR] =
{
   { "januari", "februari", "maart", "april", "mei", "juni", "juli",   "augustus","september","oktober", "november", "december" }
  ,{ "january", "february", "march", "april", "may", "june", "july",   "august",  "september","october", "november", "december" }
  ,{ "Januar",  "Februar",  "März",  "April", "Mai", "Juni", "Juli",   "August",  "September","Oktober", "November", "Dezember" }
  ,{ "janvier", "février",  "mars",  "avril", "mai", "juin", "juillet","août",    "septembre","octobre", "novembre", "décembre" }
};

const char* g_shortMonths[LN_NUMLANG][MONTHS_IN_YEAR] = 
{
  { "jan", "feb", "maa", "apr", "mei", "jun", "jul", "aug", "sep", "okt", "nov", "dec" }
 ,{ "jan", "feb", "mar", "apr", "may", "jun", "jul", "aug", "sep", "oct", "nov", "dec" }
 ,{ "Jan", "Feb", "Mär", "Apr", "Mai", "Jun", "Jul", "Aug", "Sep", "Okt", "Nov", "Dez" }
 ,{ "jan", "fév", "mar", "avr", "mai", "jui", "jul", "aoû", "sep", "oct", "nov", "déc" }
};

// Operational names for dates/moments
const char* g_dateNames[LN_NUMLANG][DN_LASTNAME] =
{
   {  "VANDAAG"         // DN_TODAY
     ,"HUIDIGMOMENT"    // DN_CURRENT
     ,"HUIDIGETIJD"     // DN_CURRENTTIME
     ,"NU"              // DN_NOW
     ,"EVM"             // DN_FOM
     ,"LVM"             // DN_LOM
     ,"EVJ"             // DN_FOY
     ,"HJ"              // DN_CY
     ,"LVJ"             // DN_LOY
     ,"SEC"             // DN_SEC
     ,"SECONDE"         // DN_SECOND
     ,"SECONDEN"        // DN_SECONDS
     ,"MIN"             // DN_MIN
     ,"MINUTE"          // DN_MINUTE
     ,"MINUTEN"         // DN_MINUTES
     ,"UUR"             // DN_HOUR
     ,"UREN"            // DN_HOURS
     ,"DAG"             // DN_DAY
     ,"DAGEN"           // DN_DAYS
     ,"WEEK"            // DN_WEEK
     ,"WEKEN"           // DN_WEEKS
     ,"MAAND"           // DN_MONTH
     ,"MAANDEN"         // DN_MONTHS
     ,"JAAR"            // DN_YEAR
     ,"JAREN"           // DN_YEARS
   }
  ,{
      "TODAY"
     ,"CURRENT"
     ,"CURRENTTIME"
     ,"NOW"
     ,"FOM"
     ,"LOM"
     ,"FOY"
     ,"CY"
     ,"LOY"
     ,"SEC"             // DN_SEC
     ,"SECOND"          // DN_SECOND
     ,"SECONDS"         // DN_SECONDS
     ,"MIN"             // DN_MIN
     ,"MINUTE"          // DN_MINUTE
     ,"MINUTES"         // DN_MINUTES
     ,"HOUR"            // DN_HOUR
     ,"HOURS"           // DN_HOURS
     ,"DAY"
     ,"DAYS"
     ,"WEEK"
     ,"WEEKS"
     ,"MONTH"
     ,"MONTHS"
     ,"YEAR"
     ,"YEARS"  
   }
  ,{ 
      "HEUTE"           // DN_TODAY
     ,"UHRZEIT"         // DN_CURRENT
     ,"ZEIT"            // DN_CURRENTTIME
     ,"JETZT"           // DN_NOW
     ,"ME"              // DN_FOM     Monats ersten
     ,"ML"              // DN_LOM     Monats letzten
     ,"EDJ"             // DN_FOY     Erste des Jahren
     ,"LJ"              // DN_CY      Laufenden Jahr
     ,"LDJ"             // DN_LOY     Letzten des Jahren
     ,"SEK"             // DN_SEC
     ,"SEKUNDE"         // DN_SECOND
     ,"SEKUNDEN"        // DN_SECONDS
     ,"MIN"             // DN_MIN
     ,"MINUTE"          // DN_MINUTE
     ,"MINUTEN"         // DN_MINUTES
     ,"STUNDE"          // DN_HOUR
     ,"STUNDEN"         // DN_HOURS
     ,"TAG"             // DN_DAY
     ,"TAGE"            // DN_DAYS
     ,"WOCHE"           // DN_WEEK
     ,"WOCHEN"          // DN_WEEKS
     ,"MONAT"           // DN_MONTH
     ,"MONATE"          // DN_MONTHS
     ,"JAHR"            // DN_YEAR
     ,"JAHRE"           // DN_YEARS
   }
  ,{
      "AUJOURD'HUI"     // DN_TODAY
     ,"MAINTENANT"      // DN_CURRENT
     ,"L'HEURE"         // DN_CURRENTTIME
     ,"ACTUEL"          // DN_NOW
     ,"PJM"             // DN_FOM       Premier Jour du Mois
     ,"DJM"             // DN_LOM       Dernier Jour du Mois
     ,"PJA"             // DN_FOY       Premier jour de l'année
     ,"AC"              // DN_CY        Année en Cours
     ,"DJA"             // DN_LOY       Dernier jour de l'année
     ,"SEC"             // DN_SEC
     ,"SECONDE"         // DN_SECOND
     ,"SECONDES"        // DN_SECONDS
     ,"MIN"             // DN_MIN
     ,"MINUTE"          // DN_MINUTE
     ,"MINUTES"         // DN_MINUTES
     ,"HEURE"           // DN_HOUR
     ,"HEURES"          // DN_HOURS
     ,"JOUR"            // DN_DAY
     ,"JOURNEES"        // DN_DAYS
     ,"SEMAINE"         // DN_WEEK
     ,"SEMAINES"        // DN_WEEKS
     ,"MOIS"            // DN_MONTH
     ,"MOIS"            // DN_MONTHS
     ,"ANNEE"           // DN_YEAR
     ,"ANS"             // DN_YEARS
   }
};

// Number of days at the beginning of the month
// 365 days at the end of the year
//
const int g_daysInTheMonth[14] =
{
   0    // January
  ,31   // February
  ,59   // March
  ,90   // April
  ,120  // May
  ,151  // June
  ,181  // July
  ,212  // August
  ,243  // September
  ,273  // October
  ,304  // November
  ,334  // December
  ,365  // 1 Year
  ,396  // February next year
};

// Setting the default language
Language SetDefaultSQLLanguage(Language p_lang)
{
  Language oldLanguage = g_defaultLanguage;
  if(LN_DEFAULT <= p_lang && p_lang < LN_NUMLANG)
  {
    g_defaultLanguage = p_lang;
  }
  return oldLanguage;
}

// End of namespace
}
