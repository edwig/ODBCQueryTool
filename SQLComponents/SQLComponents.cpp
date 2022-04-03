////////////////////////////////////////////////////////////////////////
//
// File: SQLComponents.h
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
#include "ConvertWideString.h"

namespace SQLComponents
{
  bool  g_SQLComponentsInitialized = false;
  bool  g_SQLComponentsInServer    = false;
  char* g_SQLSessionInitialization [SQLCOMP_MAX_SESS_DATABASES][SQLCOMP_MAX_SESS_SETTINGS] = { 0 };

  // Initialization of the SQLComponents library
  void InitSQLComponents(Language p_language,bool p_inServer /*= false*/)
  {
    // Save the server setting
    g_SQLComponentsInServer = p_inServer;

    // Use StdExceptions in this library
    // BEWARE: All threads must call this function!!
    _set_se_translator(SeTranslator);

    // Setting our default language for SQLDate, SQLTime and SQLTimestamp processing
    SetDefaultSQLLanguage(p_language);

    // SQLComponents always works in localtime (UTC +/- timezone)
    SQLSetLocalTimezone();

    // Check if something to do for memory
    if(g_SQLComponentsInitialized)
    {
      return;
    }

    // Initialize the code page name system
    InitCodePageNames();

    // Initialize the COM subsystem
    CoInitialize(nullptr);

    // We are now officially 'in business' :-)
    g_SQLComponentsInitialized = true;
  }

  // Test if we are properly initialized
  void SQLComponentsInitialized()
  {
    if(g_SQLComponentsInitialized == false)
    {
      throw StdException("Call InitSQLComponents() before you use the 'SQLComponents' library.");
    }
  }

  // Accept up to SQLCOMP_MAX_SESS_SETTINGS for a new SQLDatabase connection
  bool SQLSetSessionInitialisation(DatabaseType p_type,int p_number,const char* p_statement)
  {
    int type = (int)p_type;
    if(type >= 0 && type < SQLCOMP_MAX_SESS_DATABASES)
    {
      if(p_number >= 0 && p_number < SQLCOMP_MAX_SESS_SETTINGS)
      {
        g_SQLSessionInitialization[type][p_number] = const_cast<char*>(p_statement);
        return true;
      }
    }
    return false;
  }
}
