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

#ifndef __FixedString_h__
#define __FixedString_h__

#include <COMMON/ExceptionHelper.h>

#define FIXEDSTRING_INLINE_EXPAND 0

#if FIXEDSTRING_INLINE_EXPAND == 1
    #define FIXEDSTRING_INLINE inline
#else//FIXEDSTRING_INLINE_EXPAND == 0
    #define FIXEDSTRING_INLINE
#endif//FIXEDSTRING_INLINE_EXPAND

#pragma warning (disable: 4267)

namespace Common
{

//#if _MSC_VER <= 1200
    const size_t maxlen = USHRT_MAX;
//#else
//    const size_type maxlen = USHRT_MAX;
//#endif


class FixedString
{
public:
    typedef size_t size_type;


    FixedString ();
    FixedString (const FixedString& other);
    FixedString (const char* str, size_type len = maxlen);
    ~FixedString ();

    char at (size_type pos) const;
    const char* data () const;          // it is NOT a NULL terminated string
    char first () const;
    char last () const;

    size_type size () const;
    size_type length () const;
    size_type capacity () const;         // it returns 0 for externally allocated string
    size_type max_size () const;
    bool is_static_str () const;
    
    void cleanup ();
    void operator = (const FixedString& other);
    void assign (const FixedString& other);
    void assign (const char* str, size_type len = maxlen, bool exact = false);
    void assign_static (const char* str, size_type len);
    void append (char ch);
    void append (const char* str, size_type len);
    void append (const FixedString& other);
    void insert (size_type pos, const char* str, size_type len = maxlen);
    void replace (size_type pos, char ch);
    void erase (size_type from, size_type count = maxlen);
    void truncate ();

	static void _Xlen();	// report a length_error
	static void _Xran();	// report an out_of_range error

private:
    void reallocate (unsigned short length, bool exact = false);
    void reallocate (unsigned short length1, unsigned short length2, bool exact = false);
    unsigned char& ref_counter (const char* ptr);

private:
    typedef unsigned short ushort;

    char* _buffer;
    ushort _length, _capacity;

    void set_length (size_type length)          { _length = static_cast<ushort>(length); }
    void set_capacity (size_type capacity)      { _capacity = static_cast<ushort>(capacity); }

protected:
    static const char* _string_is_too_long;
}; // FixedString


    inline
    FixedString::FixedString ()
    {
        memset(this, 0, sizeof(FixedString));
    }

    inline
    FixedString::FixedString (const FixedString& other)
    {
        memset(this, 0, sizeof(FixedString));
        *this = other;
    }

    inline
    FixedString::FixedString (const char* str, size_type len)
    {
        memset(this, 0, sizeof(FixedString));
        assign(str, len);
    }

    inline
    FixedString::~FixedString ()
    {
        cleanup();
    }

    inline
    char FixedString::at (size_type pos) const
    {
        if (pos >= length()) _Xlen();

        return _buffer[pos];
    }

    // it is NOT a NULL terminated string
    inline
    const char* FixedString::data () const
    {
        return _buffer;
    }

    inline
    char FixedString::first () const
    {
        return at(0);
    }

    inline
    char FixedString::last () const
    {
        return at(length()-1);
    }

    inline
    FixedString::size_type FixedString::size () const
    {
        return _length;
    }

    inline
    FixedString::size_type FixedString::length () const
    {
        return _length;
    }

    // it returns 0 for externally allocated string
    inline
    FixedString::size_type FixedString::capacity () const
    {
        return _capacity;
    }

    inline
    FixedString::size_type FixedString::max_size () const
    {
        return maxlen;
    }

    inline
    bool FixedString::is_static_str () const
    {
        return !capacity() && _buffer;
    }

    inline
    void FixedString::cleanup ()
    {
        if (_buffer)
        {
            if (capacity() && !(--ref_counter(_buffer)))
                delete[] (_buffer-1);
            _buffer = 0;
        }
        set_length(0);
        set_capacity(0);
    }

    inline
    void FixedString::operator = (const FixedString& other)
    {
        assign(other);
    }

#if (FIXEDSTRING_INLINE_EXPAND == 1) || defined(FIXEDSTRING_MODULE)
    FIXEDSTRING_INLINE
    void FixedString::assign (const FixedString& other)
    {
        if (this != &other)
        {
            cleanup();

            if (other._buffer)
            {
                if (!other.capacity()) // other has an external buffer!
                {
                    _buffer = other._buffer;
                }
                else if (ref_counter(other._buffer) < 255) // the usual thing
                {
                    _buffer = other._buffer;
                    ++ref_counter(_buffer);
                }
                else // ref counter overflow!
                {
                    _buffer   = new char[other.capacity() + 1] + 1;
                    ref_counter(_buffer) = 1;
                    memcpy(_buffer, other._buffer, other.length());
                }
            }
            set_length(other.length());
            set_capacity(other.capacity());
        }
    }

    FIXEDSTRING_INLINE
    void FixedString::assign (const char* str, size_type len, bool exact)
    {
        if (len > 0)
        {
            if (len == max_size()) 
            {
                size_type size = strlen(str);
                _CHECK_AND_THROW_(size < max_size(), _string_is_too_long);
                len = size;
            }

            if (!capacity() || len > capacity() || ref_counter(_buffer) > 1)  
            {
              reallocate((int)len,(int)capacity(),exact);
            }
            memcpy(_buffer, str, len);
            set_length(len);
        }
        else
            cleanup();
    }

    FIXEDSTRING_INLINE
    void FixedString::assign_static (const char* str, size_type len)
    {
        _CHECK_AND_THROW_(len < max_size(), _string_is_too_long);

        if (capacity()) cleanup();
        _buffer = const_cast<char*>(str);
        set_length(len);
    }

    FIXEDSTRING_INLINE
    void FixedString::append (char ch)
    {
        if (!capacity() || length() + 1 > capacity() || ref_counter(_buffer) > 1) 
            reallocate((unsigned short)length() + 1,(unsigned short) capacity());

        _buffer[_length++] = ch;
    }
   
    FIXEDSTRING_INLINE
    void FixedString::append (const char* str, size_type len)
    {
        if (len > 0)
        {
            if (!capacity() || length() + len > capacity() || ref_counter(_buffer) > 1) 
                reallocate((unsigned short)(length() + len),(unsigned short) capacity());

            memcpy(_buffer + length(), str, len);
            set_length(length() + len); 
        }
    }

    FIXEDSTRING_INLINE
    void FixedString::append (const FixedString& other)
    {
        if (other.length() > 0)
        {
            if (!capacity() || length() + other.length() > capacity() || ref_counter(_buffer) > 1) 
                reallocate((unsigned short)(length() + other.length()),(unsigned short)capacity());

            memcpy(_buffer + length(), other._buffer, other.length());
            set_length(length() + other.length()); 
        }
    }

    FIXEDSTRING_INLINE
    void FixedString::insert (size_type pos, const char* str, size_type len)
    {
        if (len > 0)
        {
            if (pos > length()) _Xlen();

            if (len == max_size()) 
                len = strlen(str);

            if (!capacity() || length() + len > capacity() || ref_counter(_buffer) > 1)
                reallocate((unsigned short)(length() + len), (unsigned short)capacity());

            memmove(_buffer + pos + len, _buffer + pos, length() - pos);
            memcpy(_buffer + pos, str, len);
            set_length(length() + len); 
        }
    }

    FIXEDSTRING_INLINE
    void FixedString::replace (size_type pos, char ch)
    {
        if (pos >= length()) _Xlen();

        if (!capacity() || ref_counter(_buffer) > 1) 
            reallocate((unsigned short)length(), (unsigned short)capacity());

        _buffer[pos] = ch;
    }

    FIXEDSTRING_INLINE
    void FixedString::erase (size_type from, size_type count)
    {
        if (from > length()) _Xlen();

        if (from < length())
        {
            if (count >= length() - from)
            {
                if (from)
                {
                    set_length(from);

                    if (capacity() && ref_counter(_buffer)) 
                        reallocate((int)length()); // split
                }
                else  // erase from 0 pos
                    cleanup();

            }
            else
            {
                if (!capacity() && !from)
                {
                    _buffer += count;
                }
                else
                {
                    if (!capacity() || ref_counter(_buffer) > 1) 
                        reallocate((unsigned short)length(),(unsigned short)capacity());

                    if (from + count > length()) 
                        count = length() - from;

                    memmove(_buffer + from, _buffer + from + count, length() - (from + count));
                }
                set_length(length() - count);
            }
        }
    }

    FIXEDSTRING_INLINE
    void FixedString::truncate ()
    {
        if (_buffer)
            for (; _length > 0 && isspace(_buffer[_length-1]); _length--)
                ;
    }
#endif//(FIXEDSTRING_INLINE_EXPAND == 1) || defined(FIXEDSTRING_MODULE)

    inline
    unsigned char& FixedString::ref_counter (const char* ptr) 
    { 
        return (((unsigned char *)ptr)[-1]); 
    }

}; // Common

#endif//__FixedString_h__
