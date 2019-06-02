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

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __ExceptionHelper_h__
#define __ExceptionHelper_h__

#include <exception>
#include <StdException.h>

namespace Common
{
    void default_handler (const char* file, int line); // for ...
    void default_handler (const char* er,        const char* file,int line);
    void default_handler (const std::exception&, const char* file,int line);
    void default_handler (      StdException&,   const char* file,int line);
#ifdef _AFX
    void default_handler (CException*, const char* file, int line);
#endif//_AFX
}

#define _CHECK_AND_THROW_(x, msg)   if (!(x)) { _ASSERTE(0); throw std::exception((msg)); }

#define DEFAULT_HANDLER(x)          Common::default_handler(x, __FILE__, __LINE__);
#define DEFAULT_HANDLER_ALL         Common::default_handler(__FILE__, __LINE__);

#ifdef _AFX
#define _COMMON_DEFAULT_HANDLER_ \
    catch (const char *x)           { DEFAULT_HANDLER(x); } \
    catch (CException* x)           { DEFAULT_HANDLER(x); } \
    catch (const std::exception& x) { DEFAULT_HANDLER(x); } \
    catch (StdException& x)         { DEFAULT_HANDLER(x); } \
    catch (...)                     { DEFAULT_HANDLER_ALL; }
#else
#define _COMMON_DEFAULT_HANDLER_ \
    catch (char *x)                 { DEFAULT_HANDLER(x); } \
    catch (const std::exception& x) { DEFAULT_HANDLER(x); } \
    catch (...)                     { DEFAULT_HANDLER_ALL; }
#endif//_AFX

#endif//__ExceptionHelper_h__
