////////////////////////////////////////////////////////////////////////
//
// File: SQLComponents.h
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
#pragma once
#include "SQLLanguage.h"
#include "SQLComponentsVersion.h"

// Everything in this library is part of this namespace
namespace SQLComponents
{

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
 ,RDBMS_MYSQL         = 8
 ,RDBMS_MARIADB       = 9
}
DatabaseType;

// Array with session initialization statements
#define SQLCOMP_MAX_SESS_DATABASES 10
#define SQLCOMP_MAX_SESS_SETTINGS  10
extern char* g_SQLSessionInitialization[SQLCOMP_MAX_SESS_DATABASES][SQLCOMP_MAX_SESS_SETTINGS];

// We are running in a background server (No message boxes allowed!)
extern bool g_SQLComponentsInServer;

// Initialization of the SQLComponents library
// Call at least **ONCE** at the beginning of your program
void InitSQLComponents(Language p_language = LN_ENGLISH,bool p_inServer = false);

// Test if we are properly initialized
void SQLComponentsInitialized();

// SQLComponents always works in localtime (UTC +/- timezone)
void SQLSetLocalTimezone();

// Accept up to SQLCOMP_MAX_SESS_SETTINGS for a new SQLDatabase connection
bool SQLSetSessionInitialisation(DatabaseType p_type,int p_number,const char* p_statement);

// End of namespace
}

// Selecting the right library to link with automatically
// So we do not need to worry about which library to use in the linker settings
#if defined _M_IX86
#define SQL_PLATFORM "x86"
#else
#define SQL_PLATFORM "x64"
#endif

#if defined _DEBUG
#define SQL_CONFIGURATION "D"
#else
#define SQL_CONFIGURATION "R"
#endif 

#ifndef SQL_COMPONENTS_NOAUTOLINK
#pragma comment(lib,"SQLComponents_"                        SQL_PLATFORM SQL_CONFIGURATION ".lib")
#pragma message("Automatically linking with SQLComponents_" SQL_PLATFORM SQL_CONFIGURATION ".lib")
#endif 
