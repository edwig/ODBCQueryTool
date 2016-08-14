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

#include "stdafx.h"
#include <COMMON/QuickArray.h>

namespace Common 
{

    void FakeArray::realloc (size_t elemSize, size_t extra) 
    {
        size_t size = m_size + extra;

	    if (m_capacity < size)
	    {
            size_t capacity = 0;
			size_t growBy = m_growBy;

			if (!growBy)
			{
				// heuristically determine growth when nGrowBy == 0
				//  (this avoids heap fragmentation in many situations)
				growBy = m_size / 8;
				growBy = (growBy < 4) ? 4 : ((growBy > 1024) ? 1024 : growBy);
			}

			if (size < (m_capacity + growBy))
				capacity = m_capacity + growBy;  // granularity
			else
				capacity = size;  // no slush

            char* data = new char[capacity * elemSize];
#ifdef _MFC_VER
            if (!data) AfxThrowMemoryException();
#else
            if (!data) throw std::bad_alloc();
#endif
            memset(data, 0, capacity * elemSize);

            if (m_size) 
                memcpy(data, m_data, m_size * elemSize);
            
            delete[] m_data;

            m_data = data;
            m_capacity = capacity;
	    }
    }

    void FakeArray::init (size_t elemSize, size_t items) 
    {
        size_t growBy = m_growBy;
        m_growBy = items;
        
        if (items)
            realloc(elemSize, items);
        
        m_growBy = growBy;
    }

    void FakeArray::reduce (size_t /*elemSize*/, size_t items) 
    {
        if (items < m_size) 
            m_size = items;
    }
    
    void FakeArray::expand (size_t elemSize, size_t items)
    {
        if (items > m_size)
        {
            realloc(elemSize, items - m_size);
            m_size = items;
        }
    }

    void FakeArray::copy (size_t elemSize, FakeArray *src) 
    {
        _ASSERTE(0/*it has not tested yet*/);
        
        reduce(elemSize, 0);

        if (src->m_size)
            realloc(elemSize, src->m_size);

        if (src->m_size)
            memcpy(m_data, src->m_data, elemSize * src->m_size);

        m_growBy = src->m_growBy;
    }

    char* FakeArray::insert (size_t elemSize, size_t index, size_t count) 
    {
        _ASSERTE(!index || index-1 < m_size);

        if (!(!index || index-1 < m_size))
            throw std::out_of_range("invalid FakeArray<T> subscript");

        realloc(elemSize, count);
        char* ptr = m_data + index * elemSize;
        memmove(ptr + count * elemSize, ptr, (m_size - index) * elemSize);
        m_size += count;

        return m_data + index * elemSize;
    }

    char* FakeArray::append (size_t elemSize) 
    {
        realloc(elemSize, 1);
        m_size++;
        return m_data + (m_size - 1) * elemSize;
    }

    void FakeArray::erase (size_t elemSize, size_t index, size_t count) 
    {
        _ASSERTE(index < m_size && index + count <= m_size);

        if (!(index < m_size && index + count <= m_size))
            throw std::out_of_range("invalid FakeArray<T> subscript");

        memmove(m_data + elemSize * index,
                m_data + elemSize * (index + count),
                (m_size - index - count) * elemSize);

        m_size -= count;
    }

    void FakeArray::free () 
    {
        delete[] m_data;
        size_t growBy = m_growBy;
        memset(this,  0, sizeof (*this));
        m_growBy = growBy;
    }

    void FakeArray::append (size_t elemSize, FakeArray *src) 
    {
        if (src->m_size)
        {
            realloc(elemSize, src->m_size);
            memcpy(m_data + m_size * elemSize, src->m_data, src->m_size * elemSize);
            m_size += src->m_size;
        }
    }

    void FakeArray::swap (size_t elemSize, size_t idest, size_t isrc) 
    {
        _ASSERTE(idest < m_size || isrc < m_size);

        if (!(idest < m_size || isrc < m_size))
            throw std::out_of_range("invalid FakeArray<T> subscript");

        if (isrc != idest)
        {
            char* buf = new char[elemSize],
                * src = m_data + elemSize * isrc,
                * dest = m_data + elemSize * idest;

            memcpy(buf,  src,  elemSize);
            memcpy(src,  dest, elemSize);
            memcpy(dest, buf,  elemSize);

            delete [] buf;
        }
    }


    void FakeArray::construct (size_t elemSize, size_t alloc, size_t growBy) 
    {
        memset(this, 0, sizeof (*this));
        //m_growBy = growBy;
        realloc(elemSize, alloc);
        m_growBy = growBy;
    }


    void FakeArray::extract (FakeArray *array) 
    {
        array->free();
        size_t growBy = array->m_growBy;
        memcpy(array, this, sizeof (*this));
        memset(this, 0, sizeof (*this));
        m_growBy = array->m_growBy;
        array->m_growBy = growBy;
    }

}; //  namespace Common 
