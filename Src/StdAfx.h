/*
    Copyright (C) 2002 Aleksey Kochetov

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#pragma once

// Was 0x0600 (vista) Back to 0x0510 (XP)
// Support problems with fonts and ShGetFolderPath
#ifndef WINVER
#define WINVER 0x0510 
#endif

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS  // some CString constructors will be explicit
#define _AFX_ALL_WARNINGS   // turns off MFC's hiding of some common and often safely ignored warning messages
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#include <afxcmn.h>			// MFC support for Windows Common Controls

#define _OE_DEFAULT_HANDLER_ _COMMON_DEFAULT_HANDLER_

#pragma warning ( disable : 4786 )
#pragma warning ( disable : 4788 )
#pragma warning ( disable : 4238 )
#pragma warning ( disable : 4239 )
#pragma warning ( disable : 4996 )

#undef isalpha
#undef isupper
#undef islower
#undef isdigit
#undef isxdigit
#undef isspace
#undef ispunct
#undef isalnum
#undef isprint
#undef isgraph
#undef iscntrl
#undef toupper
#undef tolower

inline int isalpha (char ch) { return isalpha((int)(unsigned char)ch); }
inline int isupper (char ch) { return isupper((int)(unsigned char)ch); }
inline int islower (char ch) { return islower((int)(unsigned char)ch); }
inline int isdigit (char ch) { return isdigit((int)(unsigned char)ch); }
inline int isxdigit (char ch) { return isxdigit((int)(unsigned char)ch); }
inline int isspace (char ch) { return isspace((int)(unsigned char)ch); }
inline int ispunct (char ch) { return ispunct((int)(unsigned char)ch); }
inline int isalnum (char ch) { return isalnum((int)(unsigned char)ch); }
inline int isprint (char ch) { return isprint((int)(unsigned char)ch); }
inline int isgraph (char ch) { return isgraph((int)(unsigned char)ch); }
inline int iscntrl (char ch) { return iscntrl((int)(unsigned char)ch); }
inline char toupper (char ch) { return (char)toupper((int)(unsigned char)ch); }
inline char tolower (char ch) { return (char)tolower((int)(unsigned char)ch); }

#ifdef GetRValue
#undef GetRValue
#endif
#ifdef GetGValue
#undef GetGValue
#endif
#ifdef GetBValue
#undef GetBValue
#endif
inline BYTE GetRValue (COLORREF rgb) { return static_cast<BYTE>(0xFF & rgb); }
inline BYTE GetGValue (COLORREF rgb) { return static_cast<BYTE>((0xFF00 & rgb) >> 8); }
inline BYTE GetBValue (COLORREF rgb) { return static_cast<BYTE>((0xFF0000 & rgb) >> 16); }


#include <exception>
#include <memory>
#include <string>
#include <map>
#include <list>
#include <vector>

// @EH TEST
//#define _SCB_REPLACE_MINIFRAME
//#define _SCB_MINIFRAME_CAPTION

#define baseCMyBar CSizingControlBarG
#include "Frame/sizecbar.h"
#include "Frame/scbarg.h"
#include "Frame/scbarcf.h"
#include <afxdlgs.h>

#if _MFC_VER <= 0x0600

#ifndef INVALID_FILE_ATTRIBUTES
#define INVALID_FILE_ATTRIBUTES 0xFFFFFFFF
#define VK_OEM_PLUS       0xBB   // '+' any country
#define VK_OEM_COMMA      0xBC   // ',' any country
#define VK_OEM_MINUS      0xBD   // '-' any country
#define VK_OEM_PERIOD     0xBE   // '.' any country
#define CB_GETCOMBOBOXINFO 0x0164
#endif

#else//_MFC_VER > 0x0600

#include <afxdhtml.h>
#include "Common\WideMessageBox.h"

#endif

// Using the SQLComponents library
#include "SQLComponents.h"

#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
