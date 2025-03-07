////////////////////////////////////////////////////////////////////////
//
// File: SQLConnections.cpp
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
#include "stdafx.h"
#include "SQLConnections.h"
#include "XMLMessage.h"
#include "SQLGetExePath.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{

SQLConnections::SQLConnections()
{
}

void
SQLConnections::Reset()
{
  m_connections.clear();
}

SQLConnection*
SQLConnections::GetConnection(XString p_name)
{
  p_name.MakeLower();
  ConnMap::iterator it = m_connections.find(p_name);
  if(it != m_connections.end())
  {
    return &(it->second);
  }
  return nullptr;
}

SQLConnection* 
SQLConnections::GetConnection(unsigned p_number)
{
  unsigned index = 0;
  ConnMap::iterator it = m_connections.begin();
  while(it != m_connections.end())
  {
    if(index == p_number)
    {
      return &(it->second);
    }
    ++index;
    ++it;
  }
  return nullptr;
}

XString
SQLConnections::GetConnectionString(XString p_name)
{
  SQLConnection* conn = GetConnection(p_name);
  if(conn == nullptr)
  {
    return "";
  }
  XString connect;
  connect.Format(_T("DSN=%s;UID=%s;PWD=%s;")
                 ,conn->m_datasource.GetString()
                 ,conn->m_username.GetString()
                 ,conn->m_password.GetString());
  if(!conn->m_options.IsEmpty())
  {
    connect += conn->m_options;
  }
  return connect;
}

bool
SQLConnections::AddConnection(XString p_name
                             ,XString p_datasource
                             ,XString p_username
                             ,XString p_password
                             ,XString p_options)
{
  // See if it is a double registration
  const SQLConnection* fnd = GetConnection(p_name);
  if(fnd)
  {
    return false;
  }

  // New connection
  SQLConnection connect;
  connect.m_name       = p_name;
  connect.m_datasource = p_datasource;
  connect.m_username   = p_username;
  connect.m_password   = p_password;
  connect.m_options    = p_options;

  // Keep this connection
  p_name.MakeLower();
  m_connections.insert(std::make_pair(p_name,connect));
  return true;
}

bool
SQLConnections::DelConnection(XString p_name)
{
  p_name.MakeLower();
  ConnMap::iterator it = m_connections.find(p_name);
  if(it != m_connections.end())
  {
    m_connections.erase(it);
    return true;
  }
  return false;
}

bool
SQLConnections::LoadConnectionsFile(XString p_filename /*=""*/,bool p_reset /*=false*/)
{
  if(p_filename.IsEmpty())
  {
    p_filename = _T("Database.xml");
  }
  p_filename = SQLGetExePath() + p_filename;

  XMLMessage msg;
  if(msg.LoadFile(p_filename) == false)
  {
    return false;
  }
  if(msg.GetRoot()->GetName().Compare(_T("Connections")))
  {
    return false;
  }

  // Optionally completely reset the databases
  if(p_reset)
  {
    m_connections.clear();
  }

  // Read all connections
  XMLElement* conn = msg.FindElement(_T("Connect"));
  while(conn)
  {
    SQLConnection connect;
    connect.m_name       = msg.GetElement(conn,_T("Name"));
    connect.m_datasource = msg.GetElement(conn,_T("DSN"));
    connect.m_username   = msg.GetElement(conn,_T("User"));
    connect.m_options    = msg.GetElement(conn,_T("Options"));
    connect.m_password   = PasswordDecoding(msg.GetElement(conn,_T("Password")));

    XString name(connect.m_name);
    name.MakeLower();
    ConnMap::iterator it = m_connections.find(name);
    if(it != m_connections.end())
    {
      m_connections.erase(it);
    }
    m_connections.insert(std::make_pair(name,connect));

    // Get next connection
    conn = msg.GetElementSibling(conn);
  }
  return true;
}

bool
SQLConnections::SaveConnectionsFile(XString p_filename /*=""*/)
{
  if(p_filename.IsEmpty())
  {
    p_filename = _T("Database.xml");
  }
  p_filename = SQLGetExePath() + p_filename;

  XMLMessage msg;
  msg.SetRootNodeName(_T("Connections"));
  
  for(auto& connect : m_connections)
  {
    XMLElement* conn = msg.AddElement(nullptr,_T("Connect"),XDT_String,_T(""));

    msg.AddElement(conn,_T("Name"),     XDT_String,connect.second.m_name);
    msg.AddElement(conn,_T("DSN"),      XDT_String,connect.second.m_datasource);
    msg.AddElement(conn,_T("User"),     XDT_String,connect.second.m_username);
    msg.AddElement(conn,_T("Options"),  XDT_String,connect.second.m_options);
    msg.AddElement(conn,_T("Password"), XDT_String,PasswordScramble(connect.second.m_password));
  }

  // Save the file
  return msg.SaveFile(p_filename);
}

//////////////////////////////////////////////////////////////////////////
//
// PRIVATE
//
//////////////////////////////////////////////////////////////////////////

XString
SQLConnections::PasswordScramble(XString p_password)
{
  XString scramble;

  // XOR scrambling if ASCII
  for(int index = 0;index < p_password.GetLength(); ++index)
  {
    int ch = p_password.GetAt(index);
    if(ch < 128)
    {
      --ch;
      ch ^= 0x7F;
    }
    scramble += (unsigned char)ch;
  }

  // Code to hexadecimal
  XString coded;
  for(int index = 0;index < scramble.GetLength(); ++index)
  {
    coded.AppendFormat(_T("%2.2X"),(int)scramble.GetAt(index));
  }

  // Coded result of our password
  return coded;
}

XString
SQLConnections::PasswordDecoding(XString p_scramble)
{
  XString coded;
  int number = 0;

  // Hexadecimal to coded
  for(int index = 0; index < p_scramble.GetLength();++index)
  {
    int ch = p_scramble.GetAt(index);
    if(ch <= '9') number += (ch - '0');
    if(ch >= 'A') number += (ch - 'A' + 10);

    if(index % 2)
    {
      coded += (unsigned char)number;
      number = 0;
    }
    else
    {
      number *= 16;
    }
  }

  // Coded back to ASCII
  XString password;
  for(int index = 0;index < coded.GetLength(); ++index)
  {
    int ch = coded.GetAt(index);
    if(ch < 128)
    {
      ch ^= 0x7F;
      ++ch;
    }
    password += (unsigned char)ch;
  }

  return password;
}

}
