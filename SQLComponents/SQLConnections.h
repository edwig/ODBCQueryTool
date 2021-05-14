////////////////////////////////////////////////////////////////////////
//
// File: SQLConnections.h
//
// Copyright (c) 1998-2021 ir. W.E. Huisman
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
  CString m_name;
  CString m_datasource;
  CString m_username;
  CString m_password;
  CString m_options;
}
SQLConnection;

using ConnMap = std::map<CString,SQLConnection>;

class SQLConnections
{
public:
  SQLConnections();

  // File interface
  bool        LoadConnectionsFile(CString p_filename = "");
  bool        SaveConnectionsFile(CString p_filename = "");

  // GETTERS
  SQLConnection*  GetConnection(CString p_name);
  SQLConnection*  GetConnection(unsigned p_number);
  CString         GetConnectionString(CString p_name);

  // SETTERS
  void        Reset();
  bool        AddConnection(CString p_name,CString p_datasource,CString p_username,CString p_password,CString p_options);
  bool        DelConnection(CString p_name);

private:
  CString     PasswordScramble(CString p_password);
  CString     PasswordDecoding(CString p_scramble);

  // All saved connections from "database.xml"
  ConnMap     m_connections;
};

}
