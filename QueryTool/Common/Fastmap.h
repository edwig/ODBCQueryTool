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

#ifndef __FASTMAP_H__
#define __FASTMAP_H__

namespace Common
{
#pragma warning (push)
#pragma warning (disable : 4309)
    template<class T> // T should be char, bool, int...
    class Fastmap 
    {
    public:
      Fastmap ();
      Fastmap (const Fastmap&);
      Fastmap& operator = (const Fastmap&);
      void erase ();
      // actual class was created for operator [] only!
          // Safe conversion from char to unsigned char!
          // it's important for the second part of ascii table
      T  operator [] (_TUCHAR i) const    { return m_data[i]; }
      T& operator [] (_TUCHAR i)          { return m_data[i]; }
      _TUCHAR size () const               { return static_cast<_TUCHAR>(sizeof(m_data)/sizeof(m_data[0])); }

    private:
      T m_data[256];
    };
#pragma warning (pop)

    template<class T>
    Common::Fastmap<T>::Fastmap ()
    {
        erase();
    }

    template<class T>
    Common::Fastmap<T>::Fastmap (const Fastmap<T>& other)
    {
        memcpy(this, &other, sizeof(*this));
    }

    template<class T>
    Fastmap<T>& Fastmap<T>::operator = (const Fastmap<T>& other)
    {
        memcpy(this, &other, sizeof(*this));
        return *this;
    }

    template<class T>
    void Fastmap<T>::erase ()
    {
        memset(this, 0, sizeof(*this));
    }

} // END namespace Common

#endif//__FASTMAP_H__
