////////////////////////////////////////////////////////////////////////
//
// File: SQLComponents.h
//
// Copyright (c) 1998-2017 ir. W.E. Huisman
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
// Last Revision:   08-01-2017
// Version number:  1.4.0
//
#pragma once
#include "SQLLanguage.h"

// Everything in this library is part of this namespace
namespace SQLComponents
{

// General version, date and copyright
// of the SQLComponents library

#define SQL_COMPONENTS_VERSION   "1.4.0"
#define SQL_COMPONENTS_DATE      "08-01-2017"
#define SQL_COMPONENTS_COPYRIGHT "Copyright (c) 2017 ir. W.E. Huisman"

// SQL/CLI Standard states that 128 is the maximum length
// for any identifier in any namespace
#define SQL_MAX_IDENTIFIER  128
#define SQL_MAX_BUFFER      129

// Logging level for various systems and purposes
// Log only errors
#define LOGLEVEL_ERROR  1
// Log actions at this level
#define LOGLEVEL_ACTION 2
// Log queries at this logging level
#define LOGLEVEL_MAX    6

// Currently known database types
typedef enum _databaseType
{
  RDBMS_UNKNOWN       = 0
 ,RDBMS_ODBC_STANDARD = 1
 ,RDBMS_ORACLE        = 2
 ,RDBMS_INFORMIX      = 3
 ,RDBMS_ACCESS        = 4
 ,RDBMS_SQLSERVER     = 5
 ,RDBMS_POSTGRESQL    = 6
 ,RDBMS_FIREBIRD      = 7
}
DatabaseType;

// Structure to hold information about a DSN (DataSourceName)
typedef struct _datasource
{
  CString m_datasource;
  CString m_username;
  CString m_password;
  bool    m_system;
  bool    m_outputOMF;
  bool    m_default;
  bool    m_dataConnection;
}
DataSourceInternal;

// Initialization of the SQLComponents library
void InitSQLComponents(Language p_language = LN_ENGLISH);

// Marker boolean to see if we where properly initialized
extern bool g_SQLComponentsInitialized;

// Test if we are properly initialized
void SQLComponentsInitialized();

// End of namespace
}
