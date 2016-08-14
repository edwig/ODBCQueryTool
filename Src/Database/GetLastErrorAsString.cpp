/////////////////////////////////////////////////////////////////////////////////
//
// SourceFile: GetLastErrorAsString.cpp
//
// WinHTTP Component: Internet server/client
// 
// Copyright (c) 2015 ir. W.E. Huisman
// All rights reserved
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
#include "StdAfx.h"
#include "GetLastErrorAsString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString
GetLastErrorAsString(DWORD p_error /*=0*/)
{
  // This will be the resulting message
  CString message;

  // This is the "errno" error number
  DWORD dwError = p_error;
  if(dwError == 0)
  {
    dwError = GetLastError();
  }
  // Buffer that gets the error message string
  HLOCAL  hlocal = NULL;   
  HMODULE hDll   = NULL;

  // Use the default system locale since we look for Windows messages.
  // Note: this MAKELANGID combination has 0 as value
  DWORD systemLocale = MAKELANGID(LANG_NEUTRAL,SUBLANG_NEUTRAL);

  // Get the error code's textual description
  BOOL fOk = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM     | 
                           FORMAT_MESSAGE_IGNORE_INSERTS  |
                           FORMAT_MESSAGE_ALLOCATE_BUFFER 
                          ,NULL               // DLL
                          ,dwError            // The error code
                          ,systemLocale       // Language
                          ,(PTSTR) &hlocal    // Buffer handle
                          ,0                  // Size if not buffer handle
                          ,NULL);             // Variable arguments
  if(!fOk) 
  {
    // Is it a network-related error?
    hDll = LoadLibraryEx(TEXT("netmsg.dll"),NULL,DONT_RESOLVE_DLL_REFERENCES);

    // Only if NETMSG was found and loaded
    if(hDll != NULL) 
    {
      fOk = FormatMessage(FORMAT_MESSAGE_FROM_HMODULE    | 
                          FORMAT_MESSAGE_IGNORE_INSERTS  |
                          FORMAT_MESSAGE_ALLOCATE_BUFFER 
                         ,hDll
                         ,dwError
                         ,systemLocale
                         ,(PTSTR) &hlocal
                         ,0
                         ,NULL);
    }
  }
  if(fOk && (hlocal != NULL)) 
  {
    // Getting the message from the buffer;
    message = (PCTSTR) LocalLock(hlocal);
    LocalFree(hlocal);
    // Formatting the message in one line
    message.Replace('\n',' ');
    message.Remove ('\r');
  }
  if(hDll)
  {
    FreeLibrary(hDll);
  }
  // Resulting 
  return message;
}
