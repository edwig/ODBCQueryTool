// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but are changed infrequently
// 
// USED TO BUILD THE *.PCH (pre-compiled header) on
//
#pragma once

// All programs are MBCS programs
#define NO_WARN_MBCS_MFC_DEPRECATION

// Using current OS SDK (Windows 8/10)
#include "targetver.h"

// Use default IO library and MBCS Character set
#include <stdio.h>
#include <tchar.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit
//#define _AFX_NO_MFC_CONTROLS_IN_DIALOGS         // remove support for MFC controls in dialogs

// Exclude rarely-used stuff from Windows headers
#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            
#endif

// IF We want SQLInfoTree to work, we need the whole shebang from MFC
// IF we want SQLDatasetXLS to work, we need CArhive for oldstyle *.XLS
//    so we also need to include the full set
// So we include the full MFC set, otherwise: we just need CString
//
#include <afx.h>
#include <afxwin.h>             // MFC core and standard components
#include <afxext.h>             // MFC extensions
#include <afxcmn.h>             // MFC support for Windows Common Controls

// Autolink records only generated OUTSIDE this library
#define SQL_COMPONENTS_NOAUTOLINK

// Automatically include the correct manifest
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
