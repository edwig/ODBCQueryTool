////////////////////////////////////////////////////////////////////////
//
// File: SQLLanguage.h
//
// Copyright (c) 1998-2025 ir. W.E. Huisman
// All rights reserved
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of 
// this software and associated documentation files (the _T("Software"), 
// to deal in the Software without restriction, including without limitation the rights 
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies 
// or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED _T("AS IS"), WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
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
LPCTSTR g_weekdays[LN_NUMLANG][DAYS_IN_WEEK] = 
{
   { _T("zondag"),  _T("maandag"), _T("dinsdag"), _T("woensdag"), _T("donderdag"), _T("vrijdag"), _T("zaterdag") }
  ,{ _T("sunday"),  _T("monday"),  _T("tuesday"), _T("wednesday"),_T("thursday"),  _T("friday"),  _T("saturday") }
  ,{ _T("Sonntag"), _T("Montag"),  _T("Dienstag"),_T("Mittwoch"), _T("Donnerstag"),_T("Freitag"), _T("Samstag")  }
  ,{ _T("dimanche"),_T("lundi"),   _T("mardi"),   _T("mercredi"), _T("jeudi"),     _T("venvredi"),_T("samedi")   }
};

// All the months in the year
LPCTSTR g_monthnames[LN_NUMLANG][MONTHS_IN_YEAR] =
{
   { _T("januari"), _T("februari"), _T("maart"), _T("april"), _T("mei"), _T("juni"), _T("juli"),   _T("augustus"),_T("september"),_T("oktober"), _T("november"), _T("december") }
  ,{ _T("january"), _T("february"), _T("march"), _T("april"), _T("may"), _T("june"), _T("july"),   _T("august"),  _T("september"),_T("october"), _T("november"), _T("december") }
  ,{ _T("Januar"),  _T("Februar"),  _T("März"),  _T("April"), _T("Mai"), _T("Juni"), _T("Juli"),   _T("August"),  _T("September"),_T("Oktober"), _T("November"), _T("Dezember") }
  ,{ _T("janvier"), _T("février"),  _T("mars"),  _T("avril"), _T("mai"), _T("juin"), _T("juillet"),_T("août"),    _T("septembre"),_T("octobre"), _T("novembre"), _T("décembre") }
};

LPCTSTR g_shortMonths[LN_NUMLANG][MONTHS_IN_YEAR] = 
{
  { _T("jan"), _T("feb"), _T("maa"), _T("apr"), _T("mei"), _T("jun"), _T("jul"), _T("aug"), _T("sep"), _T("okt"), _T("nov"), _T("dec") }
 ,{ _T("jan"), _T("feb"), _T("mar"), _T("apr"), _T("may"), _T("jun"), _T("jul"), _T("aug"), _T("sep"), _T("oct"), _T("nov"), _T("dec") }
 ,{ _T("Jan"), _T("Feb"), _T("Mär"), _T("Apr"), _T("Mai"), _T("Jun"), _T("Jul"), _T("Aug"), _T("Sep"), _T("Okt"), _T("Nov"), _T("Dez") }
 ,{ _T("jan"), _T("fév"), _T("mar"), _T("avr"), _T("mai"), _T("jui"), _T("jul"), _T("aoû"), _T("sep"), _T("oct"), _T("nov"), _T("déc") }
};

// Operational names for dates/moments
LPCTSTR g_dateNames[LN_NUMLANG][DN_LASTNAME] =
{
   {  _T("VANDAAG")         // DN_TODAY
     ,_T("HUIDIGMOMENT")    // DN_CURRENT
     ,_T("HUIDIGETIJD")     // DN_CURRENTTIME
     ,_T("NU")              // DN_NOW
     ,_T("EVM")             // DN_FOM
     ,_T("LVM")             // DN_LOM
     ,_T("EVJ")             // DN_FOY
     ,_T("HJ")              // DN_CY
     ,_T("LVJ")             // DN_LOY
     ,_T("SEC")             // DN_SEC
     ,_T("SECONDE")         // DN_SECOND
     ,_T("SECONDEN")        // DN_SECONDS
     ,_T("MIN")             // DN_MIN
     ,_T("MINUTE")          // DN_MINUTE
     ,_T("MINUTEN")         // DN_MINUTES
     ,_T("UUR")             // DN_HOUR
     ,_T("UREN")            // DN_HOURS
     ,_T("DAG")             // DN_DAY
     ,_T("DAGEN")           // DN_DAYS
     ,_T("WEEK")            // DN_WEEK
     ,_T("WEKEN")           // DN_WEEKS
     ,_T("MAAND")           // DN_MONTH
     ,_T("MAANDEN")         // DN_MONTHS
     ,_T("JAAR")            // DN_YEAR
     ,_T("JAREN")           // DN_YEARS
   }
  ,{
      _T("TODAY")
     ,_T("CURRENT")
     ,_T("CURRENTTIME")
     ,_T("NOW")
     ,_T("FOM")
     ,_T("LOM")
     ,_T("FOY")
     ,_T("CY")
     ,_T("LOY")
     ,_T("SEC")             // DN_SEC
     ,_T("SECOND")          // DN_SECOND
     ,_T("SECONDS")         // DN_SECONDS
     ,_T("MIN")             // DN_MIN
     ,_T("MINUTE")          // DN_MINUTE
     ,_T("MINUTES")         // DN_MINUTES
     ,_T("HOUR")            // DN_HOUR
     ,_T("HOURS")           // DN_HOURS
     ,_T("DAY")
     ,_T("DAYS")
     ,_T("WEEK")
     ,_T("WEEKS")
     ,_T("MONTH")
     ,_T("MONTHS")
     ,_T("YEAR")
     ,_T("YEARS")  
   }
  ,{ 
      _T("HEUTE")           // DN_TODAY
     ,_T("UHRZEIT")         // DN_CURRENT
     ,_T("ZEIT")            // DN_CURRENTTIME
     ,_T("JETZT")           // DN_NOW
     ,_T("ME")              // DN_FOM     Monats ersten
     ,_T("ML")              // DN_LOM     Monats letzten
     ,_T("EDJ")             // DN_FOY     Erste des Jahren
     ,_T("LJ")              // DN_CY      Laufenden Jahr
     ,_T("LDJ")             // DN_LOY     Letzten des Jahren
     ,_T("SEK")             // DN_SEC
     ,_T("SEKUNDE")         // DN_SECOND
     ,_T("SEKUNDEN")        // DN_SECONDS
     ,_T("MIN")             // DN_MIN
     ,_T("MINUTE")          // DN_MINUTE
     ,_T("MINUTEN")         // DN_MINUTES
     ,_T("STUNDE")          // DN_HOUR
     ,_T("STUNDEN")         // DN_HOURS
     ,_T("TAG")             // DN_DAY
     ,_T("TAGE")            // DN_DAYS
     ,_T("WOCHE")           // DN_WEEK
     ,_T("WOCHEN")          // DN_WEEKS
     ,_T("MONAT")           // DN_MONTH
     ,_T("MONATE")          // DN_MONTHS
     ,_T("JAHR")            // DN_YEAR
     ,_T("JAHRE")           // DN_YEARS
   }
  ,{
      _T("AUJOURD'HUI")     // DN_TODAY
     ,_T("MAINTENANT")      // DN_CURRENT
     ,_T("L'HEURE")         // DN_CURRENTTIME
     ,_T("ACTUEL")          // DN_NOW
     ,_T("PJM")             // DN_FOM       Premier Jour du Mois
     ,_T("DJM")             // DN_LOM       Dernier Jour du Mois
     ,_T("PJA")             // DN_FOY       Premier jour de l'année
     ,_T("AC")              // DN_CY        Année en Cours
     ,_T("DJA")             // DN_LOY       Dernier jour de l'année
     ,_T("SEC")             // DN_SEC
     ,_T("SECONDE")         // DN_SECOND
     ,_T("SECONDES")        // DN_SECONDS
     ,_T("MIN")             // DN_MIN
     ,_T("MINUTE")          // DN_MINUTE
     ,_T("MINUTES")         // DN_MINUTES
     ,_T("HEURE")           // DN_HOUR
     ,_T("HEURES")          // DN_HOURS
     ,_T("JOUR")            // DN_DAY
     ,_T("JOURNEES")        // DN_DAYS
     ,_T("SEMAINE")         // DN_WEEK
     ,_T("SEMAINES")        // DN_WEEKS
     ,_T("MOIS")            // DN_MONTH
     ,_T("MOIS")            // DN_MONTHS
     ,_T("ANNEE")           // DN_YEAR
     ,_T("ANS")             // DN_YEARS
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
