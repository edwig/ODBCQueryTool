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

#ifndef __QuickArray_h__
#define __QuickArray_h__

#include <stdexcept>
#include <string.h>
#include <malloc.h>

namespace Common 
{
    class FakeArray
    {
    public:
        void   realloc  (size_t elemSize, size_t extra);
        void   init     (size_t elemSize, size_t items);
        void   reduce   (size_t elemSize, size_t items);
        void   expand   (size_t elemSize, size_t items);
        void   copy     (size_t elemSize, FakeArray *src);
        BYTE*  insert   (size_t elemSize, size_t index = 0, size_t count = 1);
        BYTE*  append   (size_t elemSize);
        void   erase    (size_t elemSize, size_t index, size_t count = 1);
        void   free     ();
        void   append   (size_t elemSize, FakeArray *src);
        void   swap     (size_t elemSize, size_t idest, size_t isrc);
        void   construct(size_t elemSize, size_t capacity, size_t growBy);
        void   extract  (FakeArray* array);
        
        static FakeArray *castPtr (void *ptr) { return (FakeArray *)ptr; }


    private:
        BYTE*  m_data;
        size_t m_size, m_capacity, m_growBy;

    private:
        // don't support
        FakeArray ();
        FakeArray (const FakeArray&);
        int operator = (const FakeArray&);
    };

    ///////////////////////////////////////////////////////////////////////////
    template <class T> 
    class QuickArray
    {
    public :
        QuickArray (size_t capacity = 0, size_t growBy = 0);
        QuickArray (const QuickArray<T>& array);
        ~QuickArray ();

        size_t &increment ();

        void init (size_t items);
        size_t size() const;

        T& operator [] (size_t i);
        const T& operator [] (size_t i) const;
        T& at (size_t i);
        const T& at (size_t i) const;
        T& first ();
        const T& first () const;
        T& last ();
        const T& last () const;

        T& append (const T&);
        T& append ();
        T& push_back (const T&);
        T& push_back ();
        T& insert (const T&, size_t index = 0);
        void insert (const QuickArray<T>& array, size_t index = 0);

        void swap (size_t idest, size_t isrc);
        
        void expand (size_t items);

        void cut ();
        void erase (size_t index, size_t cout = 1);
        void reduce (size_t items = 0);
        void clear ();
        void free ();
        void reserve (size_t items);

        QuickArray& operator = (const QuickArray<T> &array);
        void extract (QuickArray<T>& array);

    protected :
        void copy (const QuickArray<T> &array);

        T* m_data;
        size_t m_size, m_capacity, m_growBy;
    };

    
    template <class T> inline
    Common::QuickArray<T>::QuickArray (size_t capacity,  size_t growBy) 
    {
        FakeArray::castPtr(this)->construct(sizeof(T), capacity, growBy);
    }

    template <class T> inline
    Common::QuickArray<T>::QuickArray(const QuickArray<T>& array) 
    {
        memset(this, 0, sizeof(*this)); 
        copy(array);
    }

    template <class T> inline
    Common::QuickArray<T>::~QuickArray () 
    {
        reduce();
        FakeArray::castPtr(this)->free();
    }

    template <class T> inline
    size_t& QuickArray<T>::increment ()              
    { 
        return m_growBy; 
    }

    template <class T>
    void QuickArray<T>::init (size_t items) 
    {
        reduce();
        FakeArray::castPtr(this)->init(sizeof(T), items);

        for (size_t i = 0; i < items; i ++)
            new (&at(i)) T();
    }

    template <class T> inline 
    size_t QuickArray<T>::size() const 
    { 
        return m_size; 
    }

    template <class T> inline 
    T& QuickArray<T>::operator [] (size_t i) 
    { 
        _ASSERTE(i < m_size && m_data);
        return m_data[i]; 
    }

    template <class T> inline 
    const T& QuickArray<T>::operator [] (size_t i) const 
    { 
        _ASSERTE(i < m_size && m_data);
        return m_data[i]; 
    }

    template <class T> inline 
    T& QuickArray<T>::at (size_t i) 
    { 
        //_ASSERTE(i < m_size && m_data);
        if (!(i < m_size && m_data))
            throw std::out_of_range("invalid QuickArray<T> subscript");

        return m_data[i]; 
    }

    template <class T> inline 
    const T& QuickArray<T>::at (size_t i) const 
    { 
        //_ASSERTE(i < m_size && m_data);
        if (!(i < m_size && m_data))
            throw std::out_of_range("invalid QuickArray<T> subscript");

        return m_data[i]; 
    }

    template <class T> inline 
    T& QuickArray<T>::first () 
    { 
        _ASSERTE(m_size && m_data);
        return m_data[0]; 
    }

    template <class T> inline 
    const T& QuickArray<T>::first () const 
    { 
        _ASSERTE(m_size && m_data);
        return m_data[0]; 
    }

    template <class T> inline 
    T& QuickArray<T>::last () 
    { 
        _ASSERTE(m_size && m_data);
        return m_data[m_size-1]; 
    }

    template <class T> inline 
    const T& QuickArray<T>::last () const 
    { 
        _ASSERTE(m_size && m_data);
        return m_data[m_size-1]; 
    }

    template <class T>
    T& QuickArray<T>::append (const T& elem) 
    {
      BYTE* ptr = FakeArray::castPtr(this)->append(sizeof(T));
      new ((T*)ptr)T(elem);
      return *(T*)ptr;
    }

    template <class T>
    T& QuickArray<T>::append () 
    {
      BYTE* ptr = FakeArray::castPtr(this)->append(sizeof(T));
      new ((T*)ptr)T();
      return *(T*)ptr;
    }

    template <class T> inline
    T& QuickArray<T>::push_back (const T& elem)       
    { 
        return append(elem);
    }
    
    template <class T> inline
    T& QuickArray<T>::push_back ()                    
    { 
        return append(); 
    }

    template <class T>
    T& QuickArray<T>::insert (const T& elem, size_t index) 
    {
      BYTE* ptr = FakeArray::castPtr(this)->insert(sizeof(T), index);
      new ((T*)ptr)T(elem);
      return *(T*)ptr;
    }
   
    template <class T>
    void QuickArray<T>::insert (const QuickArray<T>& array, size_t index)
    {
      FakeArray::castPtr(this)->insert(sizeof(T), index, array.size());
      for(size_t i = 0; i < array.size(); i++)
      {
        new ((T*) &operator[](index + i))T(array[i]);
      }
    }
   
    template <class T> inline 
    void QuickArray<T>::swap (size_t idest, size_t isrc) 
    {
      FakeArray::castPtr(this)->swap(sizeof(T), idest, isrc);
    }

    template <class T>
    void QuickArray<T>::expand (size_t items)
    {
        size_t size = m_size;
        FakeArray::castPtr(this)->expand(sizeof(T), items);
        
        for (size_t index = size; index < m_size; index++)
            new ((T*)&at(index))T();
    }

    template <class T> inline
    void QuickArray<T>::cut() 
    {
        if (size()) erase(size() - 1);
    }

    template <class T>
    void QuickArray<T>::erase (size_t index, size_t count)
    {
        if (!(index < m_size && m_data))
            throw std::out_of_range("invalid QuickArray<T> subscript");
        
        size_t i;
        size_t end;
        for (i = index, end = index + count; i < end && i < m_size; i++)
            operator[](i).~T();

        FakeArray::castPtr(this)->erase(sizeof(T), index, i - index);
    }

    template <class T>
    void QuickArray<T>::reduce (size_t items) 
    {
        for (size_t index = items; index < size(); index++)
            at(index).~T();

        FakeArray::castPtr(this)->reduce(sizeof(T), items);
    }

    template <class T> inline
    void QuickArray<T>::clear ()                       
    { 
        reduce(0); 
    }
    
    template <class T> inline
    void QuickArray<T>::free () 
    {
        reduce(); 
        FakeArray::castPtr(this)->free();
    }

    template <class T> inline
    void QuickArray<T>::reserve (size_t items)
    {
        size_t extra = (items > m_size) ? items - m_size : 0;
        if (extra)
            FakeArray::castPtr(this)->realloc(sizeof(T), extra);
    }

    template <class T> inline
    QuickArray<T>& QuickArray<T>::operator = (const QuickArray<T> &array) 
    {
        copy(array);
        return *this;
    }

    template <class T> inline
    void QuickArray<T>::extract (QuickArray<T>& array) 
    {
        array.reduce(); 
        FakeArray::castPtr(this)->extract(FakeArray::castPtr(&array));
    }

    template <class T> 
    void QuickArray<T>::copy (const QuickArray<T> &array) 
    {
        reduce();
        
        if (size_t items = array.size()) 
        {
            m_growBy = 0;

            FakeArray::castPtr(this)->realloc(sizeof(T), items);
            
            for (size_t i = 0; i < items; i ++)
                append(array.at(i));
        }
            
        m_growBy = array.m_growBy;
    }

};
// namespace Common


#endif  // __QuickArray_h__
