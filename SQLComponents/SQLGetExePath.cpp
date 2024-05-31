////////////////////////////////////////////////////////////////////////
//
// File: SQLGetExePath.h
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
#include "stdafx.h"
#include "SQLGetExePath.h"
#include "SQLMessage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{

static TCHAR g_staticAddress;

HMODULE SQLGetModuleHandle()
{
  // Getting the module handle, if any
  // If it fails, the process names will be retrieved
  // Thus we get the *.DLL handle in IIS instead of a
  // %systemdrive\system32\inetsrv\w3wp.exe path
  HMODULE mod = NULL;
  GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                    GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT
                   ,static_cast<LPCTSTR>(&g_staticAddress)
                   ,&mod);
  return mod;
}

XString SQLGetExeFile()
{
  TCHAR buffer[_MAX_PATH + 1];

  HMODULE mod = SQLGetModuleHandle();

  // Retrieve the path
  GetModuleFileName(mod,buffer,_MAX_PATH);
  return XString(buffer);
}

XString SQLGetExePath()
{
  // Retrieve the path
  XString applicationAndPath = SQLGetExeFile();

  int slashPositie = applicationAndPath.ReverseFind('\\');
  if (slashPositie == 0)
  {
    return "";
  }
  return applicationAndPath.Left(slashPositie + 1);
}

static void
SQLTerminateWithoutCleanup(int p_exitcode)
{
  //  Tha... tha... that's all folks.
  HANDLE hProcess = OpenProcess(PROCESS_TERMINATE,FALSE,GetCurrentProcessId());
  if(!hProcess || !TerminateProcess(hProcess,p_exitcode))
  {
    if(hProcess) CloseHandle(hProcess);
    _exit(p_exitcode);
  }
  // No rights to terminate, do a clean abort
  abort();
}

// Check the renaming of the runtimer (EXE, DLL, or whatever)
void
SQLCheckExePath(XString p_runtimer)
{
  TCHAR drive    [_MAX_DRIVE];
  TCHAR directory[_MAX_DIR  ];
  TCHAR filename [_MAX_FNAME];
  TCHAR extension[_MAX_EXT  ];

  XString program = SQLGetExeFile();
  _tsplitpath_s(program.GetString(),drive,_MAX_DRIVE,directory,_MAX_DIR,filename,_MAX_FNAME,extension,_MAX_EXT);

  XString runtimer = XString(filename) + XString(extension);
  if(runtimer.CompareNoCase(p_runtimer))
  {
    XString error;
    error.Format(_T("You have started the program: %s\n"
                    "But in reality it is now: %s\n"
                    "Your product cannot function properly under this difference!\n"
                    "\n"
                    "Contact your friendly system administrator about this problem.\n"
                    "Fix the installation of the program and retry this operation.")
                ,runtimer.GetString()
                ,p_runtimer.GetString());
    SQLMessage(NULL,error,_T("Installation"),MB_OK|MB_ICONERROR);
    SQLTerminateWithoutCleanup(-4);
  }
}

}
