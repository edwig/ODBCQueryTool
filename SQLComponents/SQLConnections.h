////////////////////////////////////////////////////////////////////////
//
// File: SQLConnections.h
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
#include <map>

namespace SQLComponents
{

typedef struct _connection
{
  XString m_name;
  XString m_datasource;
  XString m_targetSchema;
  XString m_username;
  XString m_password;
  XString m_options;
}
SQLConnection;

using ConnMap = std::map<XString,SQLConnection>;

#define DEFAULT_ENCRYPTION_KEY _T("S~Q!L@C#$n%ne^&c*t(i)o<n>s/")

class SQLConnections
{
public:
  SQLConnections();

  // File interface
  bool        LoadConnectionsFile(XString p_filename = _T(""),bool p_reset = false);
  bool        SaveConnectionsFile(XString p_filename = _T(""));

  // GETTERS
  SQLConnection*  GetConnection(XString p_name);
  SQLConnection*  GetConnection(unsigned p_number);
  XString         GetConnectionString(XString p_name);
  int             GetConnectionsCount();

  // SETTERS
  void        Reset();
  void        SetEncryptionKey(XString p_key);
  bool        AddConnection(XString p_name
                           ,XString p_datasource
                           ,XString p_username
                           ,XString p_password
                           ,XString p_options
                           ,XString p_targetSchema = _T(""));
  bool        DelConnection(XString p_name);

private:
  XString     PasswordScramble(XString p_password);
  XString     PasswordDecoding(XString p_scramble);

  // All saved connections from "database.xml"
  ConnMap     m_connections;
  XString     m_cryptKey;
};

}
