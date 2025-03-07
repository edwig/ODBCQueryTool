////////////////////////////////////////////////////////////////////////
//
// File: SQLMessage.h
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
#include "stdafx.h"
#include "SQLMessage.h"
#include "SQLComponents.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{

#define SVC_ERROR   ((DWORD)0xC0020001L)
#define SVC_INFO    ((DWORD)0x40020002L)

// Message in the foreground or to the WMI Event log
INT_PTR
SQLMessage(HWND p_wnd,XString p_message,XString p_title,int p_buttons)
{
  if(g_SQLComponentsInServer)
  {
    // print to the event log
    HANDLE  hEventSource = OpenEventLog(NULL,_T("SQLComponents ODBC"));
    WORD    type  = (p_buttons & MB_ICONERROR) ? EVENTLOG_ERROR_TYPE : EVENTLOG_INFORMATION_TYPE;
    DWORD   ident = (p_buttons & MB_ICONERROR) ? SVC_ERROR           : SVC_INFO;

    if (hEventSource != NULL)
    {
      LPCTSTR lpszStrings[2];

      lpszStrings[0] = _T("SQLComponents (ODBC 3.51) Version: ") SQL_COMPONENTS_VERSION;
      lpszStrings[1] = p_message;

      ReportEvent(hEventSource,  // event log handle
                  type,          // event type
                  0,             // event category
                  ident,         // event identifier
                  NULL,          // no security identifier
                  2,             // size of lpszStrings array
                  0,             // no binary data
                  lpszStrings,   // array of strings
                  NULL);         // no binary data
      CloseEventLog(hEventSource);
    }
    // These are the three cases in the WordEditor code
    if(p_buttons & MB_YESNOCANCEL)
    {
      return IDCANCEL;
    }
    if (p_buttons & MB_YESNO)
    {
      return IDNO;
    }
    return IDOK;
  }
  else
  {
    // Just show the message box in the foreground
    return MessageBox(p_wnd,p_message,p_title,p_buttons);
  }
}

}
