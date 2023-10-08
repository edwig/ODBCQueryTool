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

#ifndef __STRHELPERS_H__
#define __STRHELPERS_H__

#include <string>

namespace Common
{
    using std::vector;

    void trim_symmetric (CString& str, LPCTSTR skip = _T(" \t\n\r"));
    void to_printable_str   (LPCTSTR from, CString& to);
    void to_unprintable_str (LPCTSTR from, CString& to, bool skipEscDgt = false);
    void date_c_to_oracle   (LPCTSTR from, CString& to);
    void date_oracle_to_c   (LPCTSTR from, CString& to);
    void to_upper_str       (LPCTSTR from, CString& to);
    void to_lower_str       (LPCTSTR from, CString& to);

    //  This object-procedure is quicker (two times) than vprintf.
    class Substitutor
    {
    public:
        Substitutor (bool casesensitive = true) { m_casesensitive = casesensitive; }
        
        void ResetContent ();
        void ResetResult ();

        void AddPair (LPCTSTR, LPCTSTR);
        void AddPair (const CString&, const CString&);
        Substitutor& operator << (LPCTSTR);
        LPCTSTR GetResult () const;

    private:
        CString         m_result;
        vector<CString> m_whatFind;
        vector<CString> m_withReplace;
        bool            m_casesensitive;
    };

    inline
    LPCTSTR Substitutor::GetResult () const
    { 
      return m_result.GetString(); 
    }

    inline
    void Substitutor::ResetResult ()
    { 
      m_result.Empty();
    }

} // END namespace Common

#endif//__STRHELPERS_H__
