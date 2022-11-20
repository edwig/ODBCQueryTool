////////////////////////////////////////////////////////////////////////
//
// File: SQLDriverManager.h
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
#include <sqltypes.h>
#include <sql.h>
#include <vector>

// BEWARE
// To use this module, your programm **MUST** be linked with the following library definitions
// in the linker settings:
// - legacy_stdio_definitions.lib
// - odbccp32.lib

namespace SQLComponents
{

// SQLGetInstallerError can return a maximum of 8 error records
#define MAX_DRIVER_ERROR         8
// Max length of a driver description gotten
#define MAX_DRIVER_DESCRIPTION 250

// Structure to hold information about a DSN (DataSourceName)
typedef struct _datasource
{
  XString m_datasource;                 // DSN Name
  XString m_description;                // Description seen in MS-Windows
  XString m_username;                   // Default user name to use
  XString m_password;                   // Password to use
  bool    m_system          { false };  // DSN is a system-wide datasource name
  bool    m_default         { false };  // DSN is to be used by default in our system
  bool    m_changed         { false };  // DSN is changed. Write back to ...
  bool    m_dataConnection  { false };  // DSN is for a end-user data connection
}
DataSource;

typedef std::vector<DataSource> DataSources;


class SQLDriverManager
{
public:
  SQLDriverManager();
 ~SQLDriverManager();

  // Place where the SQL Driver manager resides in MS-Windows
  XString   GetDriverManagerPath();
  // Getting all the data sources in one list
  bool      GetDataSources(DataSources& p_list,int p_type = SQL_FETCH_FIRST);
  // Get a driver with special capabilities
  XString   GetSpecialDriver(XString p_base,XString p_extension);
  // Show the MS-Windows ODBC management dialog window
  bool      ODBCManagerDialog(HWND p_parent);
  // Return error information from the driver manager
  int       GetDriverManagerError(XString& p_error,XString* p_sqlstate = nullptr);

private:
  // Private methods
  void      MakeEnvironmentHandle();
  void      CloseEnvironmentHandle();
  void      GetInstallerError();
  void      GetEnvironmentError();
  void      ResetErrorState();

  // Data
  HENV      m_environment;
  XString   m_driverManagerPath;

  // Error information
  DWORD     m_error;
  XString   m_errorString;
  XString   m_sqlState;
};

}