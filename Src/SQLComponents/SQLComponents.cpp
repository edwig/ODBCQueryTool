////////////////////////////////////////////////////////////////////////
//
// File: SQLComponents.h
//
// Copyright (c) 1998-2017 ir. W.E. Huisman
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
// Last Revision:   08-01-2017
// Version number:  1.4.0
//
#include "stdafx.h"
#include "SQLComponents.h"

// To execute at the end of the program
void __cdecl AtExitOfSQLComponents(void)
{
#ifndef SQL_COMPONENTS_MFC
  _CrtMemDumpAllObjectsSince(NULL);
#endif
}

namespace SQLComponents
{
  bool g_SQLComponentsInitialized = false;

  // Initialization of the SQLComponents library
  void InitSQLComponents(Language p_language)
  {
    // Do reporting of memory leaks if we have no MFC loaded
    // By registering our AtExit function
    atexit(::AtExitOfSQLComponents);

    // Setting our default language for SQLDate, SQLTime and SQLTimestamp processing
    SetDefaultSQLLanguage(p_language);

    // We are now officially 'in business' :-)
    g_SQLComponentsInitialized = true;
  }

  // Test if we are properly initialized
  void SQLComponentsInitialized()
  {
    if(g_SQLComponentsInitialized == false)
    {
      throw CString("Call InitSQLComponents() before you use the 'SQLComponents' library.");
    }
  }
}

