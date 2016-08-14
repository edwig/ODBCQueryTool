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


namespace Common
{
    typedef unsigned short ushort;

#if _MSC_VER <= 1200
    const ushort maxlen = USHRT_MAX;
    const ushort BYTES_PER_PARA = 16;
#endif

class FixedString
{
public:
    template<class _String, class _Pointer, class _Reference, 
             class _String2, class _Pointer2, class _Reference2> class _Iter;

    typedef _Iter<FixedString, char*, char&, 
                  FixedString, char*, char&> iterator;

    typedef _Iter<const FixedString, const char*, const char&,
                  FixedString, char*, char&> const_iterator;


#if _MSC_VER > 1200
    static const ushort maxlen = USHRT_MAX;
    static const ushort BYTES_PER_PARA = 16;
#endif

    FixedString ();
    FixedString (const FixedString& other);
    FixedString (const char* str, ushort len = maxlen);
    ~FixedString ();

    char at (ushort pos) const;
    const char* f_str() const;          // it is NOT a NULL terminated string
    char first () const;
    char last () const;

    ushort length () const;
    ushort capacity () const;         // it returns 0 for externally allocated string
    bool is_static_str () const;
    
    void cleanup ();
    void operator = (const FixedString& other);
    void assign (const FixedString& other);
    void assign (const char* str, ushort len = maxlen, bool exact = false);
    void assign_static (const char* str, ushort len);
    void append (char ch);
    void append (const char* str, ushort len);
    void append (const FixedString& other);
    void insert (ushort pos, const char* str, ushort len = maxlen);
    void replace (ushort pos, char ch);
    void erase (ushort from, ushort count = maxlen);
    void truncate ();

    iterator begin ();
    const_iterator begin () const;
    iterator end ();
    const_iterator end () const;

private:
    void reallocate (ushort length, bool = false);
    void reallocate (ushort length1, ushort length2, bool = false);
    unsigned char& ref_counter (const char* ptr);

private:
    char* _buffer;
    ushort _length, _capacity;

protected:
	void _Xlen() const;	// report a length_error
	void _Xran() const;	// report an out_of_range error
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
    FixedString::FixedString (const char* str, ushort len)
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
    char FixedString::at (ushort pos) const
    {
        if (pos >= _length) _Xlen();

        return _buffer[pos];
    }

    // it is NOT a NULL terminated string
    inline
    const char* FixedString::f_str() const
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
        return at(_length-1);
    }

    inline
    ushort FixedString::length () const
    {
        return _length;
    }

    // it returns 0 for externally allocated string
    inline
    ushort FixedString::capacity () const
    {
        return _capacity;
    }

    inline
    bool FixedString::is_static_str () const
    {
        return !_capacity && _buffer;
    }

    inline
    void FixedString::cleanup ()
    {
        if (_buffer)
        {
            if (_capacity && !(--ref_counter(_buffer)))
                delete[] (_buffer-1);
            _buffer = 0;
        }
        _length = _capacity = 0;
    }

    inline
    void FixedString::operator = (const FixedString& other)
    {
        assign(other);
    }

    inline
    void FixedString::assign (const FixedString& other)
    {
        if (this != &other)
        {
            cleanup();

            if (other._buffer)
            {
                if (!other._capacity) // other has an external buffer!
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
                    _buffer   = new char[other._capacity + 1] + 1;
                    ref_counter(_buffer) = 1;
                    memcpy(_buffer, other._buffer, other._length);
                }
            }
            _length   = other._length;
            _capacity = other._capacity;
        }
    }

    inline
    void FixedString::assign (const char* str, ushort len, bool exact)
    {
        if (len > 0)
        {
            if (len == maxlen) len = strlen(str);

            _CHECK_AND_THROW_(len < USHRT_MAX, _string_is_too_long);

            if (!_capacity || len > _capacity || ref_counter(_buffer) > 1)  
                reallocate(len, _capacity, exact);

            memcpy(_buffer, str, len);
            _length = len;
        }
        else
            cleanup();
    }

    inline
    void FixedString::assign_static (const char* str, ushort len)
    {
        _CHECK_AND_THROW_(len < USHRT_MAX, _string_is_too_long);

        if (_capacity) cleanup();
        _buffer = const_cast<char*>(str);
        _length = len;
    }

    inline
    void FixedString::append (char ch)
    {
        if (!_capacity || _length + 1 > _capacity || ref_counter(_buffer) > 1) 
            reallocate(_length + 1, _capacity);

        _buffer[_length++] = ch;
    }
   
    inline
    void FixedString::append (const char* str, ushort len)
    {
        if (len > 0)
        {
            if (!_capacity || _length + len > _capacity || ref_counter(_buffer) > 1) 
                reallocate(_length + len, _capacity);

            memcpy(_buffer + _length, str, len);
            _length += len;
        }
    }

    inline
    void FixedString::append (const FixedString& other)
    {
        if (other._length > 0)
        {
            if (!_capacity || _length + other._length > _capacity || ref_counter(_buffer) > 1) 
                reallocate(_length + other._length, _capacity);

            memcpy(_buffer + _length, other._buffer, other._length);
            _length += other._length;
        }
    }

    inline
    void FixedString::insert (ushort pos, const char* str, ushort len)
    {
        if (len > 0)
        {
            if (pos > _length) _Xlen();
            if (len == maxlen) len = strlen(str);

            if (!_capacity || _length + len > _capacity || ref_counter(_buffer) > 1)
                reallocate(_length + len, _capacity);

            memmove(_buffer + pos + len, _buffer + pos, _length - pos);
            memcpy(_buffer + pos, str, len);
            _length += len;
        }
    }

    inline
    void FixedString::replace (ushort pos, char ch)
    {
        if (pos >= _length) _Xlen();

        if (!_capacity || ref_counter(_buffer) > 1) 
            reallocate(_capacity, _length);

        _buffer[pos] = ch;
    }

    inline
    void FixedString::erase (ushort from, ushort count)
    {
        if (from > _length) _Xlen();

        if (from < _length)
        {
            if (count >= _length - from)
            {
                if (from)
                {
                    _length = from;

                    if (_capacity && ref_counter(_buffer)) 
                        reallocate(_length);
                }
                else  // erase from 0 pos
                    cleanup();

            }
            else
            {
                if (!_capacity && !from)
                {
                    _buffer += count;
                }
                else
                {
                    if (!_capacity || ref_counter(_buffer) > 1) 
                        reallocate(_capacity, _length);

                    if (from + count > _length) 
                        count = _length - from;

                    memmove(_buffer + from, _buffer + from + count, _length - (from + count));
                }
                _length -= count;
            }
        }
    }

    inline
    void FixedString::truncate ()
    {
        if (_buffer)
            for (; _length > 0 && isspace(_buffer[_length-1]); _length--)
                ;
    }

    inline
    unsigned char& FixedString::ref_counter (const char* ptr) 
    { 
        return (((unsigned char *)ptr)[-1]); 
    }

    ///////////////////////////////////////////////////////////////////////////

    template<class _String, class _Pointer, class _Reference, 
      class _String2, class _Pointer2, class _Reference2> 
        class FixedString::_Iter
    {
    public:
        typedef _Iter<_String, _Pointer, _Reference, _String2, _Pointer2, _Reference2> _MyIter;

        _String* _pstr;
        size_t _current;
        static _String _null;

    public:
        // stub for regexp only
	    typedef int iterator_category;
	    typedef char value_type;
	    typedef short difference_type;
	    typedef difference_type distance_type;
	    typedef _Pointer pointer;
	    typedef _Reference reference;

    public:
        _MyIter () : _pstr(&_null), _current(0) {}
        _MyIter (_String str, ushort off) : _pstr(&str), _current(off) {}
        // const converter or copy constructor
        _MyIter (const _Iter<_String2, _Pointer2, _Reference2, _String2, _Pointer2, _Reference2>& it)
            : _pstr(it._pstr), _current(it._current) {}

	    _Reference operator* () const
		{
            _ASSERTE(!(_ptr == 0 || _current >= _length));
		    return _ptr[_current];
		}

	    _Pointer operator->() const
		{
		    return (&**this);
		}

	    _MyIter& operator++ ()
		{
		    ++_current;
		    return (*this);
		}

	    _MyIter operator++ (int)
		{
		    _MyIter it = *this;
		    ++_current;
		    return (it);
		}

	    _MyIter& operator-- ()
		{
		    --_current;
		    return (*this);
		}

	    _MyIter operator-- (int)
		{
		    _MyIter it = *this;
		    --_current;
		    return (_it);
		}

	    bool operator == (const _MyIter& other) const
		{
            _ASSERTE(_ptr == other._ptr);
		    return (_ptr + _current == other._ptr + other._current);
		}

	    bool operator != (const _MyIter& other) const
		{
		    return (!(*this == other));
		}

	    _MyIter& operator+= (short off)
		{
		    _current += off;
		    return (*this);
		}

	    _MyIter operator+ (short off) const
		{
            _MyIter it(*this);
            it += off;
		    return it;
		}

	    _MyIter& operator-= (short off)
		{
		    _current -= off;
		    return (*this);
		}

	    _MyIter operator- (short off) const
		{
            _MyIter it(*this);
            it -= off;
		    return it;
		}

	    _Reference operator[] (short off) const
		{
		    return (*(*this + off));
		}

	    bool operator < (const _MyIter& other) const
		{
            _ASSERTE(_ptr == other._ptr);
		    return (_ptr + _current < other._ptr + other._current);
		}

	    bool operator > (const _MyIter& other) const
		{
            _ASSERTE(_ptr == other._ptr);
		    return (_ptr + _current > other._ptr + other._current);
		}

	    bool operator <= (const _MyIter& other) const
		{
		    return (!(other < *this));
		}

	    bool operator >= (const _MyIter& other) const
		{
		    return (!(*this < other));
		}

	    short operator - (const _MyIter& other) const
		{
		    return (current - other.current);
		}
	};

    ///////////////////////////////////////////////////////////////////////////
    
    inline
    FixedString::iterator FixedString::begin ()
    {
        return iterator(*this, 0);
    }

    inline
    FixedString::const_iterator FixedString::begin () const
    {
        return const_iterator(*this, 0);
    }

    inline
    FixedString::iterator FixedString::end ()
    {
        return iterator(*this, length());
    }

    inline
    FixedString::const_iterator FixedString::end () const
    {
        return const_iterator(*this, length());
    }

}; // Common

    struct std::iterator_traits<Common::FixedString>
	{
	typedef int iterator_category;
    typedef char value_type;
	typedef int difference_type;
	typedef difference_type distance_type;	// retained
	typedef char* pointer;
	typedef char& reference;
	};


#endif//__FixedString_h__
