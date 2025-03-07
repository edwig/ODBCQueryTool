////////////////////////////////////////////////////////////////////////
//
// File: Locker.h
//
// Copyright (c) 1998-2025 ir. W.E. Huisman
// All rights reserved
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of 
// this software and associated documentation files (the "Software"), 
// to deal in the Software without restriction, including without limitation the rights 
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies 
// or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION 
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// Version number: See SQLComponents.h
//
#pragma once
//
// Simple template to balance Acquire/Release calls for mutexes etc
//
template <class TC>
class Locker
{
public:
  Locker(TC* p_toBeLocked, unsigned p_timeout) 
        :m_toBeLocked(p_toBeLocked)
        ,m_locked(0)
  {
    Acquire(p_timeout);
  }
  ~Locker()
  {
    if(m_toBeLocked)
    {
      // Release always when m_locked >= 1
      if(m_locked >= 1)
      {
        m_toBeLocked->Release();
      }
    }
  }
  void Register(TC* p_toBeLocked)
  {
    if(m_toBeLocked == NULL)
    {
      m_toBeLocked = p_toBeLocked;
      Acquire(p_toBeLocked);
    }
  }
  void Acquire(unsigned p_timeout)
  {
    if(m_toBeLocked)
    {
      if(InterlockedIncrement(&m_locked) == 1)
      {
        m_toBeLocked->Acquire(p_timeout);
      }
    }
  }
  void Release(void)
  {
    if(m_toBeLocked)
    {
      if(InterlockedDecrement(&m_locked) <= 0)
      {
        m_toBeLocked->Release();
      }
    }
  }
private:
  TC*  m_toBeLocked; // Pointer to template class
  long m_locked;     // int, not unsigned (when there are to much Releases)
};
