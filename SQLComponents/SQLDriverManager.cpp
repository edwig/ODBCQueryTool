////////////////////////////////////////////////////////////////////////
//
// File: SQLDriverManager.cpp
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
#include "SQLDriverManager.h"
#include "StdException.h"
#include <odbcinst.h>
#include <sqlext.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma comment(lib,"odbccp32.lib")

namespace SQLComponents
{

static const int SQLSTATE_LEN     = 6;
static const int ERROR_BUFFERSIZE = 4096;

SQLDriverManager::SQLDriverManager()
                 :m_environment(SQL_NULL_HANDLE)
                 ,m_error(NULL)
{
  MakeEnvironmentHandle();
}

SQLDriverManager::~SQLDriverManager()
{
  CloseEnvironmentHandle();
}

XString   
SQLDriverManager::GetDriverManagerPath()
{
  if(m_driverManagerPath.IsEmpty())
  {
    WORD size = 0;
    TCHAR path[MAX_PATH + 1];

    memset(path,0,MAX_PATH+1);
    if(SQLInstallDriverManager(path,MAX_PATH,&size) && size > 0)
    {
      m_driverManagerPath = path;
    }
    else
    {
      GetInstallerError();
    }
  }
  return m_driverManagerPath;
}

bool
SQLDriverManager::GetDataSources(DataSources& p_list,int p_type /*= SQL_FETCH_FIRST*/)
{
  // Make sure we have an environment handle
  if(m_environment == NULL)
  {
    MakeEnvironmentHandle();
  }
  // Reset the error state
  ResetErrorState();

  // Data variables
  SQLTCHAR     server           [SQL_MAX_DSN_LENGTH+1];
  SQLTCHAR     description      [SQL_MAX_CATALOG_NAME_LEN+1];
  SQLSMALLINT  serverLengthInp = SQL_MAX_DSN_LENGTH;
  SQLSMALLINT  serverLengthOut = 0;
  SQLSMALLINT  descLengthInp   = SQL_MAX_CATALOG_NAME_LEN;
  SQLSMALLINT  descLengthOut   = 0;
  SQLUSMALLINT direction       = 0;

  switch(p_type)
  {
    case SQL_FETCH_FIRST_USER:    // Fall through
    case SQL_FETCH_FIRST_SYSTEM:  direction = (SQLUSMALLINT)p_type;
                                  break;
    case SQL_FETCH_FIRST:         // Fall through
    default:                      direction = SQL_FETCH_FIRST;
                                  break;
  }
  if(SQLDataSources(m_environment
                   ,direction
                   ,server
                   ,serverLengthInp
                   ,&serverLengthOut
                   ,description
                   ,descLengthInp
                   ,&descLengthOut) == SQL_SUCCESS)
  {
    do 
    {
      DataSource source;
      source.m_datasource = server;
      source.m_system     = (direction == SQL_FETCH_FIRST_SYSTEM);
      source.m_changed    = false;
      source.m_default    = false;
      p_list.push_back(source);
    } 
    while(SQL_SUCCEEDED(SQLDataSources(m_environment
                                      ,SQL_FETCH_NEXT
                                      ,server
                                      ,serverLengthInp
                                      ,&serverLengthOut
                                      ,description
                                      ,descLengthInp
                                      ,&descLengthOut)));
  }
  else
  {
    GetEnvironmentError();
  }
  return m_error == 0;
}

// Get the name of a special ODBC driver
// e.g. the Excel-ODBC driver and searches for a sub-capability
// e.g. the first ".xls" or ".xlsx" capable driver
XString
SQLDriverManager::GetSpecialDriver(XString p_base,XString p_extension)
{
  SQLTCHAR    driverDescription[MAX_DRIVER_DESCRIPTION + 1];
  SQLSMALLINT dLength = 0;
  SQLTCHAR    attribDescription[MAX_DRIVER_DESCRIPTION + 1];
  SQLSMALLINT aLength = 0;
  RETCODE     result  = SQL_ERROR;

  // Make handle
  if(m_environment == NULL)
  {
    MakeEnvironmentHandle();
  }
  ResetErrorState();
  p_base.MakeLower();
  p_extension.MakeLower();

  // Get the names of the installed drivers 1 by 1.
  result = SQLDrivers(m_environment,SQL_FETCH_FIRST
                     ,driverDescription,MAX_DRIVER_DESCRIPTION,&dLength
                     ,attribDescription,MAX_DRIVER_DESCRIPTION,&aLength);
  while(SQL_SUCCEEDED(result))
  {
    // See if it's e.g. an Excel driver and if it supports
    // the right extension (e.g. "xls" or "xlsx")
    // Most of the time there are multiple Excel drivers on the system
    XString description(driverDescription);

    if(description.Find(p_base)      >= 0 || 
       description.Find(p_extension) >= 0  )
    {
      // Found a match
      return description;
    }
    // GET Next driver
    result = SQLDrivers(m_environment,SQL_FETCH_NEXT
                       ,driverDescription,MAX_DRIVER_DESCRIPTION,&dLength
                       ,attribDescription,MAX_DRIVER_DESCRIPTION,&aLength);
  }
  // Nothing found
  return XString("");
}

// Show the MS-Windows ODBC management dialog window
bool
SQLDriverManager::ODBCManagerDialog(HWND p_parent)
{
  if(::SQLManageDataSources(p_parent) == false)
  {
    m_error = ERROR_ACCESS_DENIED;
    m_errorString = _T("ODBCManager has been blocked by your system administrator");
    return false;
  }
  return true;
}


// Return error information from the driver manager
int
SQLDriverManager::GetDriverManagerError(XString& p_error,XString* p_sqlstate /*= nullptr*/)
{
  if(m_error == 0)
  {
    // Try to get the error status information
    GetInstallerError();
  }
  // Return the error information
  if(p_sqlstate)
  {
    *p_sqlstate = m_sqlState;
  }
  p_error = m_errorString;
  return m_error;
}


//////////////////////////////////////////////////////////////////////////
//
// PRIVATE
//
//////////////////////////////////////////////////////////////////////////

void
SQLDriverManager::MakeEnvironmentHandle()
{
  // Create the handle
  m_environment = SQL_NULL_HANDLE;
  SQLRETURN res = SQLAllocHandle(SQL_HANDLE_ENV,SQL_NULL_HANDLE,&m_environment);

  // Check results
  if(res != SQL_SUCCESS && res != SQL_SUCCESS_WITH_INFO)
  {
    throw StdException(_T("Error at opening: cannot create an ODBC environment."));
  }

  // Tell the driver how we use the handles
  // Henceforth we are a ODBC 3.x program
  SQLPOINTER startVersion = (SQLPOINTER)SQL_OV_ODBC3;
  res = SQLSetEnvAttr(m_environment,SQL_ATTR_ODBC_VERSION,startVersion,0);
  if(res != SQL_SUCCESS && res != SQL_SUCCESS_WITH_INFO)
  {
    throw StdException(_T("Cannot set the ODBC version for the environment."));
  }
}

void
SQLDriverManager::CloseEnvironmentHandle()
{
  // Disconnect environment
  SQLRETURN res = SQLFreeHandle(SQL_HANDLE_ENV,m_environment);
  if(res != SQL_SUCCESS && res != SQL_SUCCESS_WITH_INFO)
  {
    GetEnvironmentError();
  }
  m_environment = SQL_NULL_HANDLE;
}

// Get error info specific to the DriverManager install functions
void
SQLDriverManager::GetInstallerError()
{
  // Installer error records are 1 based to a maximum of 8 records
  for(int err_index = 1;err_index <= MAX_DRIVER_ERROR; ++err_index)
  {
    WORD  size  = 0;
    DWORD error = 0;
    TCHAR errorMsg[MAX_PATH + 1];
    SQLRETURN ret = SQLInstallerError(static_cast<WORD>(err_index),&error,reinterpret_cast<LPTSTR>(&errorMsg),MAX_PATH,&size);
    if(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
      m_error = error;
      if(size)
      {
        if(!m_errorString.IsEmpty())
        {
          m_errorString += "\n";
        }
        m_errorString += errorMsg;
      }
    }
  }
}

// General error from the environment handle only
void
SQLDriverManager::GetEnvironmentError()
{
  // Fields for SQLGetDiagRec
  SQLTCHAR    szSqlState[SQLSTATE_LEN + 1];
  SQLINTEGER  fNativeError;
  SQLTCHAR    szErrorMsg[ERROR_BUFFERSIZE + 1];
  SQLSMALLINT cbErrorMsg;
  SQLSMALLINT recNummer = 0;

  // Get all error records
  XString errors;
  while (m_environment)
  {
    // Getting the diagnostic info record
    szErrorMsg[0] = 0;
    szSqlState[0] = 0;
    SQLRETURN res = SQLGetDiagRec(SQL_HANDLE_ENV,     // Type of the handle
                                  m_environment,      // Handle for which info is gotten
                                  ++recNummer,        // Records start at 1
                                  szSqlState,         // SQL State
                                  &fNativeError,      // Native error
                                  szErrorMsg,         // Error string
                                  ERROR_BUFFERSIZE,   // Max chars of error buffer
                                  &cbErrorMsg);       // Number of chars gotten
    // End of all error info
    if(res == SQL_NO_DATA)
    {
      break;
    }
    // Setting the native error
    if (recNummer == 1)
    {
      m_error = fNativeError;
    }
    // Remember SQLSTATE value
    if (szSqlState[0])
    {
      m_sqlState = szSqlState;
    }
    // Optional add a linefeed
    if (errors.GetLength())
    {
      errors += _T("\n");
    }
    // Error at getting errors
    if(!(res == SQL_SUCCESS || res == SQL_SUCCESS_WITH_INFO))
    {
      XString err;
      err.Format(_T("Error %d found while reading the SQL error status."), res);
      errors += err;
      break;
    }
    // Strip error message. Should be done at least for Oracle!!
    if (cbErrorMsg > 0 && cbErrorMsg <= ERROR_BUFFERSIZE)
    {
      szErrorMsg[cbErrorMsg] = 0;
    }
    // Take SQLState and native error into account
    XString error;
    error.Format(_T("[%s][%d]"), szSqlState, fNativeError);
    // Add state and error message
    errors += error + XString(szErrorMsg);
  }
  // ready
  m_errorString = errors;
}

// Resetting all error members
void
SQLDriverManager::ResetErrorState()
{
  m_error = 0;
  m_errorString.Empty();
  m_sqlState.Empty();
}

}
