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

#include "pch.h"
#include <string>
#include <sstream>
#include <COMMON/ExceptionHelper.h>
#include <StdException.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace Common
{
    using namespace std;

    inline
    void print_exception (const CString& str) 
    { 
      StyleMessageBox(nullptr,str.GetString(), _T("ERROR"),MB_OK|MB_ICONSTOP); 
    }

    void default_handler (LPCTSTR file, int line) // for ...
    {
      CString str;
      str.Format(_T("Unknown exception is being caught at [%s] on line: %d.\n\n")
                 _T("Please notify me about this problem. You can use email address 'edwig.huisman@hetnet.nl'\n")
                 _T("Thank you!!\n")
                 ,file,line);
      print_exception(str);
    }

    void default_handler (LPCTSTR er, LPCTSTR file,int line)
    {
      CString str;
      str.Format(_T("Unexpected exception \"%s\" is being caught at [%s] ;on line: %d.\n\n")
                 _T("Please notify me about this problem. You can use email address: 'edwig.huisman@hetnet.nl'\n")
                 _T("Thank you!!\n")
                 ,er,file,line);
        print_exception(str);
    }

    void default_handler(const CString& er, LPCTSTR file, int line)
    {
      CString str;
      str.Format(_T("Unexpected exception \"%s\" is being caught at [%s] ;on line: %d.\n\n")
                 _T("Please notify me about this problem. You can use email address: 'edwig.huisman@hetnet.nl'\n")
                 _T("Thank you!!\n")
                 ,er.GetString(),file,line);
      print_exception(str);
    }

    void default_handler (const std::exception& x, LPCTSTR file, int line)
    {
      CString str;
      str.Format(_T("Unexpected exception \"%s\" is being caught at [%s] ;on line: %d.\n\n")
                 _T("Please notify me about this problem. You can use email address: 'edwig.huisman@hetnet.nl'\n")
                 _T("Thank you!!\n")
                 ,x.what(),file,line);
      print_exception(str);
    }

    void default_handler(StdException& x, LPCTSTR file, int line)
    {
      CString str;
      str.Format(_T("Unexpected exception \"%s\" is being caught at [%s] ;on line: %d.\n\n")
                 _T("Please notify me about this problem. You can use email address: 'edwig.huisman@hetnet.nl'\n")
                 _T("Thank you!!\n")
                 ,x.GetErrorMessage().GetString(),file,line);
      print_exception(str);
    }

    void default_handler (CException* /*x*/, LPCTSTR /*file*/, int /*line*/)
    { 
        throw; 
    }
}

