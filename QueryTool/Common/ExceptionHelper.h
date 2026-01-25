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
#include <exception>
#include <StdException.h>

namespace Common
{
    void default_handler (LPCTSTR file,int line); // for ...
    void default_handler (LPCTSTR er,            LPCTSTR file,int line);
    void default_handler (const std::exception&, LPCTSTR file,int line);
    void default_handler (      StdException&,   LPCTSTR file,int line);
    void default_handler (const CString&,        LPCTSTR file,int line);
#ifdef _AFX
    void default_handler (CException*, LPCTSTR file, int line);
#endif//_AFX
}

#define _CHECK_AND_THROW_(x, msg)   if (!(x)) { throw StdException((msg)); }

#define DEFAULT_HANDLER(x)          Common::default_handler(x,_T(__FILE__), __LINE__);
#define DEFAULT_HANDLER_ALL         Common::default_handler(  _T(__FILE__), __LINE__);

#ifdef _AFX
#define _COMMON_DEFAULT_HANDLER_ \
    catch (LPCTSTR x)               { DEFAULT_HANDLER(x); } \
    catch (CException* x)           { DEFAULT_HANDLER(x); } \
    catch (StdException& x)         { DEFAULT_HANDLER(x); } \
    catch (const CString& x)        { DEFAULT_HANDLER(x); } \
    catch (...)                     { DEFAULT_HANDLER_ALL; }
#else
#define _COMMON_DEFAULT_HANDLER_ \
    catch (LPTSTR x)                { DEFAULT_HANDLER(x); } \
    catch (...)                     { DEFAULT_HANDLER_ALL; }
#endif//_AFX


