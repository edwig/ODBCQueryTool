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
#include <COMMON/ExceptionHelper.h>

#pragma warning (disable: 4267)

namespace Common
{

const size_t maxlen = USHRT_MAX;

class FixedString
{
public:
    typedef size_t size_type;

    FixedString ();
    FixedString (const FixedString& other);
    FixedString (const TCHAR* str, size_type len = maxlen);
    ~FixedString ();

    TCHAR at (size_type pos) const;
    const TCHAR* data () const;          // it is NOT a NULL terminated string
    TCHAR first () const;
    TCHAR last () const;

    size_type size () const;
    size_type length () const;
    size_type capacity () const;         // it returns 0 for externally allocated string
    size_type max_size () const;
    bool is_static_str () const;
    
    void cleanup ();
    void operator = (const FixedString& other);
    void assign (const FixedString& other);
    void assign (const TCHAR* str, size_type len = maxlen, bool exact = false);
    void assign_static (const TCHAR* str, size_type len);
    void append (TCHAR ch);
    void append (const TCHAR* str, size_type len);
    void append (const FixedString& other);
    void insert (size_type pos, const TCHAR* str, size_type len = maxlen);
    void replace (size_type pos, TCHAR ch);
    void erase (size_type from, size_type count = maxlen);
    void truncate ();

	static void _Xlen();	// report a length_error
	static void _Xran();	// report an out_of_range error

protected:
  static const TCHAR* _string_is_too_long;

private:
  typedef unsigned short ushort;

  void set_length(size_type length)     { _length   = static_cast<ushort>(length);   }
  void set_capacity(size_type capacity) { _capacity = static_cast<ushort>(capacity); }

  void reallocate(unsigned short length,bool exact = false);
  void reallocate(unsigned short length,unsigned short length2,bool exact = false);
  
  _TUCHAR& ref_counter(const TCHAR* ptr);

  TCHAR* _buffer;     // ref-counter (pos = 0) and string (pos = 1 and greater)
  ushort _length;     // Used length in the string
  ushort _capacity;   // Capacity in the buffer ( > _length !!)
};

}; // Common

