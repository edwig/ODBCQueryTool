// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but are changed infrequently
// 
// USED TO BUILD THE *.PCH (pre-compiled header) on
//
#pragma once

// Exclude rarely-used stuff from Windows headers
#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            
#endif

// Using current OS SDK (Windows 8/10)
#include "targetver.h"
#include "framework.h"

// Use default IO library 
#include <stdio.h>

// #include <afxext.h>             // MFC extensions
// #include <afxcmn.h>             // MFC support for Windows Common Controls

// Autolink records only generated OUTSIDE this library
#define SQL_COMPONENTS_NOAUTOLINK
// Autolink to the base library
#include <BaseLibrary.h>

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
