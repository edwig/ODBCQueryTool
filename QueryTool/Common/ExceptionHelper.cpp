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

#define VERBOSE_EXCEPTION_HANDLER

namespace Common
{
    using namespace std;

#ifdef _AFX
    inline
    void print_exception (const string& str) 
    { 
      StyleMessageBox(nullptr,str.c_str(), "ERROR",MB_OK|MB_ICONSTOP); 
    }
#else
    inline
    void print_exception (const string& str) 
        { cerr << str << endl; }
#endif//_AFX

#ifdef VERBOSE_EXCEPTION_HANDLER

    void default_handler (const char* file, int line) // for ...
    {
        ostringstream o;
        o << "Unknown exception is being caught at " << file << '(' << line << ").\n\n"
  		  << "Please notify me about this problem. You can use email to edwig.huisman@hetnet.nl\n"
             "Thank you\n";

        print_exception(o.str());
    }

    void default_handler (const char *er, const char* file,int line)
    {
       ostringstream o;
        o << "Unexpected exception \"" << er << "\" is being caught at " << file << '(' << line << ").\n\n"
          << "Please notify me about this problem. You can use email to edwig.huisman@hetnet.nl\n"
             "Thank you\n";
    
        print_exception(o.str());
    }

    void default_handler(const CString& er, const char* file, int line)
    {
      ostringstream o;
      o << "Unexpected exception \"" << er << "\" is being caught at " << file << '(' << line << ").\n\n"
        << "Please notify me about this problem. You can use email to edwig.huisman@hetnet.nl\n"
           "Thank you\n";

      print_exception(o.str());
    }

    void default_handler (const std::exception& x, const char* file, int line)
    {
        ostringstream o;
        o << "Unexpected exception \"" << x.what() << "\" is being caught at " << file << '(' << line << ").\n\n"
          << "Please notify me about this problem. You can use email to edwig.huisman@hetnet.nl\n"
             "Thank you\n";

        print_exception(o.str());
    }

    void default_handler(StdException& x, const char* file, int line)
    {
      ostringstream o;
      o << "UNEXPECTED EXCEPTION:\n\n" << x.GetErrorMessage();
      o << "\nError is being caught at " << file << '(' << line << ").\n";
      o << "Please notify me about this problem. You can use email to edwig.huisman@hetnet.nl\n"
           "Thank you\n";

      print_exception(o.str());
    }


#ifdef _AFX
    void default_handler (CException* /*x*/, const char* /*file*/, int /*line*/)
    { 
        throw; 
    }
#endif//_AFX

#else// VERBOSE_EXCEPTION_HANDLER is not defined
    void default_handler (const char* file, int line) // for ...
    {
#ifdef _AFX
        AfxMessageBox("Unknown exception!", MB_OK|MB_ICONSTOP);
#else
        cerr << "Unknown exception!" << endl;
#endif//_AFX
    }


    void default_handler (const std::exception& x, const char* file, int line)
    {
#ifdef _AFX
        AfxMessageBox(x.what(), MB_OK|MB_ICONSTOP);
#else
        cerr << CString("Unexpected error (") << x.what() << ")!" << endl;
#endif//_AFX
    }

#ifdef _AFX
    void default_handler (CException*, const char* file, int line)
    {
        throw;
    }
#endif//_AFX

#endif//VERBOSE_EXCEPTION_HANDLER

}

