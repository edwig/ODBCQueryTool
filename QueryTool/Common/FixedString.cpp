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
#define FIXEDSTRING_MODULE
#include <COMMON/FixedString.h>
#include <stdexcept>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace Common
{
  static const FixedString::size_type BYTES_PER_PARA = 16;
  LPCTSTR      FixedString::_string_is_too_long = _T("FixedString is too long (>=64K).");

  // XTORS

  FixedString::FixedString()
  {
    memset(this,0,sizeof(FixedString));
  }

  FixedString::FixedString(const FixedString& other)
  {
    memset(this,0,sizeof(FixedString));
    *this = other;
  }

  FixedString::FixedString(const TCHAR* str,size_type len)
  {
    memset(this,0,sizeof(FixedString));
    assign(str,len);
  }

  // DTOR

  FixedString::~FixedString()
  {
    cleanup();
  }

  // FUNCTIONS

  TCHAR 
  FixedString::at(size_type pos) const
  {
    if(pos >= length())
    {
      _Xlen();
    }
    return _buffer[pos];
  }

  // it is NULL terminated string
  const
  TCHAR* FixedString::data() const
  {
    return _buffer;
  }

  TCHAR
  FixedString::first() const
  {
    return at(0);
  }

  TCHAR
  FixedString::last() const
  {
    return at(length() - 1);
  }

  FixedString::size_type
  FixedString::size() const
  {
    return _length;
  }

  FixedString::size_type
  FixedString::length() const
  {
    return _length;
  }

  // it returns 0 for externally allocated string
  FixedString::size_type
  FixedString::capacity() const
  {
    return _capacity;
  }

  FixedString::size_type
  FixedString::max_size() const
  {
    return maxlen;
  }

  bool 
  FixedString::is_static_str() const
  {
    return !capacity() && _buffer;
  }

  void 
  FixedString::cleanup()
  {
    if(_buffer)
    {
      if(capacity() && !(--ref_counter(_buffer)))
      {
        delete[](_buffer - 1);
      }
      _buffer = 0;
    }
    set_length(0);
    set_capacity(0);
  }

  void
  FixedString::operator=(const FixedString& other)
  {
    assign(other);
  }

  void
  FixedString::assign(const FixedString& other)
  {
    if(this != &other)
    {
      cleanup();

      if(other._buffer)
      {
        if(!other.capacity()) // other has an external buffer!
        {
          _buffer = other._buffer;
        }
        else if(ref_counter(other._buffer) < 255) // the usual thing
        {
          _buffer = other._buffer;
          ++ref_counter(_buffer);
        }
        else // ref counter overflow!
        {
          _buffer = new TCHAR[other.capacity() + 2] + 1;
          ref_counter(_buffer) = 1;
          memcpy(_buffer,other._buffer,(other.length() + 1) * sizeof(TCHAR));
        }
      }
      set_length(other.length());
      set_capacity(other.capacity());
    }
  }

  void
  FixedString::assign(const TCHAR* str,size_type len,bool exact)
  {
    if(len > 0)
    {
      if(len == max_size())
      {
        size_type size = _tcslen(str);
        _CHECK_AND_THROW_(size < max_size(),_string_is_too_long);
        len = size;
      }

      if(!capacity() || len > capacity() || ref_counter(_buffer) > 1)
      {
        reallocate((int) len,(int) capacity(),exact);
      }
      memcpy(_buffer,str,(len + 1) * sizeof(TCHAR));
      set_length(len);
    }
    else
    {
      cleanup();
    }
  }

  void
  FixedString::assign_static(const TCHAR* str,size_type len)
  {
    _CHECK_AND_THROW_(len < max_size(),_string_is_too_long);

    if(capacity())
    {
      cleanup();
    }
    _buffer = const_cast<TCHAR*>(str);
    set_length(len);
  }

  void
  FixedString::append(TCHAR ch)
  {
    if(!capacity() || length() + 1 > capacity() || ref_counter(_buffer) > 1)
    {
      reallocate((unsigned short) length() + 1,(unsigned short) capacity());
    }
    _buffer[_length++] = ch;
  }

  void
  FixedString::append(const TCHAR* str,size_type len)
  {
    if(len > 0)
    {
      if(!capacity() || length() + len > capacity() || ref_counter(_buffer) > 1)
      {
        reallocate((unsigned short) (length() + len),(unsigned short) capacity());
      }
      memcpy(_buffer + length(),str,(len + 1) * sizeof(TCHAR));
      set_length(length() + len);
    }
  }

  void
  FixedString::append(const FixedString& other)
  {
    if(other.length() > 0)
    {
      if(!capacity() || length() + other.length() > capacity() || ref_counter(_buffer) > 1)
      {
        reallocate((unsigned short) (length() + other.length()),(unsigned short) capacity());
      }
      memcpy(_buffer + length(),other._buffer,(other.length() + 1) * sizeof(TCHAR));
      set_length(length() + other.length());
    }
  }

  void
  FixedString::insert(size_type pos,const TCHAR* str,size_type len)
  {
    if(len > 0)
    {
      if(pos > length())
      {
        _Xlen();
      }
      if(len == max_size())
      {
        len = _tcslen(str);
      }
      if(!capacity() || length() + len > capacity() || ref_counter(_buffer) > 1)
      {
        reallocate((unsigned short) (length() + len),(unsigned short) capacity());
      }
      memmove(_buffer + pos + len,_buffer + pos,((length() - pos) + 1) * sizeof(TCHAR));
      memcpy(_buffer + pos,str,len * sizeof(TCHAR));
      set_length(length() + len);
    }
  }
  
  void
  FixedString::replace(size_type pos,TCHAR ch)
  {
    if(pos >= length()) _Xlen();

    if(!capacity() || ref_counter(_buffer) > 1)
    {
      reallocate((unsigned short) length(),(unsigned short) capacity());
    }
    _buffer[pos] = ch;
  }

  void
  FixedString::erase(size_type from,size_type count)
  {
    if(from > length())
    {
      _Xlen();
    }
    if(from < length())
    {
      if(count >= length() - from)
      {
        if(from)
        {
          set_length(from);

          if(capacity() && ref_counter(_buffer))
          {
            reallocate((int) length()); // split
          }
        }
        else  // erase from 0 pos
        {
          cleanup();
        }
      }
      else
      {
        if(!capacity() && !from)
        {
          _buffer += count;
        }
        else
        {
          if(!capacity() || ref_counter(_buffer) > 1)
          {
            reallocate((unsigned short) length(),(unsigned short) capacity());
          }
          if(from + count > length())
          {
            count = length() - from;
          }
          memmove(_buffer + from,_buffer + from + count,(length() - (from + count)) * sizeof(TCHAR));
        }
        set_length(length() - count);
      }
    }
  }

  void
  FixedString::truncate()
  {
    if(_buffer)
    {
      for(; _length > 0 && _istspace(_buffer[_length - 1]); _length--)
      {
        ;
      }
      _buffer[_length] = _T('\0');
    }
  }

  // ERROR MESSAGES

  void 
  FixedString::_Xlen() // report a length_error
  {
    throw StdException(_T("Length error: string_is_too_long"));
  }

  void
  FixedString::_Xran() // report an out_of_range error
  {
    throw StdException(_T("Out of range: FixedString invalid subscript."));
  }

  //////////////////////////////////////////////////////////////////////////
  //
  // PRIVATE
  //
  //////////////////////////////////////////////////////////////////////////
  
  void
  FixedString::reallocate (unsigned short length1, unsigned short length2, bool exact)
  {
      reallocate(max(length1, length2), exact);
  }

  void
  FixedString::reallocate (unsigned short len, bool exact)
  {
      _CHECK_AND_THROW_(len >= length(), _T("FixedString cannot truncate data."));

      if (!len) cleanup();

      size_type cap = exact ? len : (len + BYTES_PER_PARA - 1) & ~(BYTES_PER_PARA - 1);

      _CHECK_AND_THROW_(cap + 1 < max_size(), _string_is_too_long);

      LPTSTR buffer = new TCHAR[cap + 2];
      memset(buffer,0,(cap + 2) * sizeof(TCHAR));
      ref_counter(buffer + 1) = 1;
      if(_buffer)
      {
        memcpy(buffer + 1,_buffer,length() * sizeof(TCHAR));
      }
      if(capacity() && !(--ref_counter(_buffer)))
      {
          delete[] (_buffer-1);
      }

      _buffer = buffer + 1;
      set_capacity(cap);
  }

  _TUCHAR&
  FixedString::ref_counter(const TCHAR* ptr)
  {
    return (((_TUCHAR*) ptr)[-1]);
  }
}

