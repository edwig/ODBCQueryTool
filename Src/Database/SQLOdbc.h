////////////////////////////////////////////////////////////////////////
//
// File: SQLOdbc.h
//
// Copyright (c) 1998-2016 ir. W.E. Huisman
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
// Last Revision:   14-12-2016
// Version number:  1.3.0
//
#pragma once

#ifndef ALL_WARNINGS
#pragma warning(disable: 4073)  // disable warning about using init_seg
#endif

#ifndef _countof
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif

enum DbOpenOptions
{
	openExclusive =         0x0001, // Not implemented
	openReadOnly =          0x0002, // Open database read only
	useCursorLib =          0x0004, // Use ODBC cursor lib
	noOdbcDialog =          0x0008, // Don't display ODBC Connect dialog
	forceOdbcDialog =       0x0010, // Always display ODBC connect dialog
};

// This MACRO is used for asynchronious ODBC calls, but makes it synchroniously
#define ODBC_CALL_ASYNC(SQLFunc) \
	try \
	{ \
    do \
    { \
      nRetCode = SQL_ERROR; \
      nRetCode = (SQLFunc); \
	  } while (nRetCode == SQL_STILL_EXECUTING) \
  } catch(...) {}

// This macro is used for synchronious ODBC calls
#define ODBC_CALL_ONCE(SQLFunc) \
	try \
	{ \
    nRetCode = SQL_ERROR; \
    nRetCode = (SQLFunc); \
  } catch(...) {}

// Max display length in chars of timestamp (date & time) value
#define TIMESTAMP_PRECISION 23

//  Miscellaneous sizing info
#define MAX_CURRENCY     30     // Max size of Currency($) string
#define MAX_TNAME_LEN    64     // Max size of table names
#define MAX_FNAME_LEN    256    // Max size of field names
#define MAX_DBNAME_LEN   32     // Max size of a database name
#define MAX_DNAME_LEN    256    // Max size of Recordset names
// Comes from afxdb.h (1024)
//#define MAX_CONNECT_LEN  512    // Max size of Connect string 
#define MAX_CURSOR_NAME  18     // Max size of a cursor name
#define DEFAULT_FIELD_TYPE SQL_TYPE_NULL // pick "C" data type to match SQL data type

// Timeout and net wait defaults
#define DEFAULT_LOGIN_TIMEOUT 15    // seconds to before fail on connect
#define DEFAULT_QUERY_TIMEOUT 15    // seconds to before fail waiting for results

