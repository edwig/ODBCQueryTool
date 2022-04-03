////////////////////////////////////////////////////////////////////////
//
// File: SQLGuid.cpp
//
// Copyright (c) 1998-2022 ir. W.E. Huisman
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
#include "stdafx.h"
#include "SQLComponents.h"
#include "SQLGuid.h"
#include <guiddef.h>
#include <combaseapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{

// XTOR
SQLGuid::SQLGuid()
{
  // Set the guid to zeros
  memset((void*)&m_guid, 0, sizeof(SQLGUID));
}

// XTOR from other SQLGuid
SQLGuid::SQLGuid(const SQLGuid& p_guid)
{
  *this = p_guid;
}

// XTOR from database GUID
SQLGuid::SQLGuid(const SQLGUID* p_guid)
{
  Set(p_guid);
}

// XTOR from string
SQLGuid::SQLGuid(const XString p_string)
{
  Set(p_string);
}

// Re-Generate a new GUID value
bool
SQLGuid::New()
{
  m_initialized = false;
  return GenerateGUID();
}

// Set from an internal string
bool
SQLGuid::Set(const XString p_string)
{
  m_initialized = false;
  if(UuidFromString((unsigned char*)p_string.GetString(),&m_guid) == RPC_S_OK)
  {
    m_initialized = true;
  }
  return m_initialized;
}

// Set from a database GUID
bool
SQLGuid::Set(const SQLGUID* p_guid)
{
  memcpy(&m_guid,p_guid,sizeof(SQLGUID));
  return (m_initialized = true);
}

// Is a valid initialized GUID
bool
SQLGuid::IsValid()
{
  return m_initialized;
}

// Get as a XString
XString 
SQLGuid::AsString()
{
  XString guid;
  if(m_initialized)
  {
    RPC_CSTR guidString = nullptr;
    // Convert the GUID to a string
    if(UuidToString(&m_guid,&guidString) == RPC_S_OK)
    {
      // And convert to a XString
      guid = XString(guidString);
      RpcStringFree(&guidString);
    }
  }
  return guid;
}

SQLGUID* 
SQLGuid::AsGUID()
{
  return &m_guid;
}

bool
SQLGuid::GenerateGUID()
{
  // CoInitialize done in InitSQLComponents
  HRESULT hr = CoCreateGuid(&m_guid);
  if(SUCCEEDED(hr))
  {
    m_initialized = true;
    return true;
  }
  return false;
}

// See if GUID's are equal
bool 
SQLGuid::operator ==(const SQLGuid& p_other)
{
  if(this == &p_other)
  {
    return true;
  }
  if(m_initialized != p_other.m_initialized)
  {
    return false;
  }
  return memcmp(&m_guid,&p_other.m_guid,sizeof(SQLGUID)) == 0;
}

// Assignment
SQLGuid& 
SQLGuid::operator =(const SQLGuid& p_other)
{
  // Check if we are not copying ourselves
  if(&p_other == this)
  {
    return *this;
  }
  // Copy the GUID
  m_initialized = p_other.m_initialized;
  memcpy_s(&m_guid,sizeof(SQLGUID),&p_other.m_guid,sizeof(SQLGUID));
  return *this;
}

}