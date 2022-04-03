////////////////////////////////////////////////////////////////////////
//
// File: SQLWrappers.h
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
#include "SQLComponents.h"
#include <sqltypes.h>

//////////////////////////////////////////////////////////////////////////
//
// This files contains wrappers for all SQLXxxxx functions to circumvent
// access violations and other exceptions from ODBC drivers by catching them all
//

#define sql_catch(ex)    catch(StdException& ex) { ReThrowSafeException(ex); return SQL_ERROR; }

namespace SQLComponents
{

inline SQLRETURN SqlDriverConnect(SQLHDBC hdbc, SQLHWND hwnd, SQLCHAR *szConnStrIn, SQLSMALLINT cbConnStrIn, SQLCHAR *szConnStrOut, SQLSMALLINT cbConnStrOutMax, SQLSMALLINT *pcbConnStrOut, SQLUSMALLINT fDriverCompletion)
{
  try
  {
    return ::SQLDriverConnect(hdbc, hwnd, szConnStrIn, cbConnStrIn, szConnStrOut, cbConnStrOutMax, pcbConnStrOut, fDriverCompletion);
  }
  sql_catch(ex)
}

inline SQLRETURN SqlDisconnect(SQLHDBC ConnectionHandle)
{
  try
  {
    return ::SQLDisconnect(ConnectionHandle);
  }
  sql_catch(ex)
}

inline SQLRETURN SqlFreeHandle(SQLSMALLINT HandleType, SQLHANDLE Handle)
{
  try
  {
    return SQLFreeHandle(HandleType, Handle);
  }
  sql_catch(ex)
}

inline SQLRETURN SqlFreeStmt(HSTMT stmt,SQLUSMALLINT option)
{
  try
  {
    return SQLFreeStmt(stmt,option);
  }
  sql_catch(ex)
}

inline SQLRETURN SqlGetInfo(SQLHDBC hdbc, SQLUSMALLINT fInfoType, SQLPOINTER rgbInfoValue, SQLSMALLINT cbInfoValueMax, SQLSMALLINT* pcbInfoValue)
{
  try
  {
    return SQLGetInfo(hdbc, fInfoType, rgbInfoValue, cbInfoValueMax, pcbInfoValue);
  }
  sql_catch(ex)
}

inline SQLRETURN SqlAllocHandle(SQLSMALLINT HandleType, SQLHANDLE InputHandle, SQLHANDLE *OutputHandle)
{
  try
  {
    return SQLAllocHandle(HandleType, InputHandle, OutputHandle);
  }
  sql_catch(ex)
}

inline SQLRETURN SqlAllocEnv(SQLHENV *EnvironmentHandle)
{
  try
  {
    return SQLAllocEnv(EnvironmentHandle);
  }
  sql_catch(ex)
}

inline SQLRETURN SqlAllocConnect(SQLHENV EnvironmentHandle, SQLHDBC *ConnectionHandle)
{
  try
  {
    return SQLAllocConnect(EnvironmentHandle, ConnectionHandle);
  }
  sql_catch(ex)
}

inline SQLRETURN SqlAllocStmt(SQLHDBC ConnectionHandle, SQLHSTMT *StatementHandle)
{
  try
  {
    return SQLAllocStmt(ConnectionHandle, StatementHandle);
  }
  sql_catch(ex)
}

inline SQLRETURN SqlGetDiagRec(SQLSMALLINT fHandleType, SQLHANDLE handle, SQLSMALLINT iRecord, SQLCHAR *szSqlState, SQLINTEGER *pfNativeError, SQLCHAR *szErrorMsg, SQLSMALLINT cbErrorMsgMax, SQLSMALLINT *pcbErrorMsg)
{
  try
  {
    return SQLGetDiagRec(fHandleType, handle, iRecord, szSqlState, pfNativeError, szErrorMsg, cbErrorMsgMax, pcbErrorMsg);
  }
  sql_catch(ex)
}

inline SQLRETURN SqlEndTran(SQLSMALLINT HandleType, SQLHANDLE Handle, SQLSMALLINT CompletionType)
{
  try
  {
    return SQLEndTran(HandleType, Handle, CompletionType);
  }
  sql_catch(ex)
}

inline SQLRETURN SqlSetConnectAttr(SQLHDBC hdbc, SQLINTEGER fAttribute, SQLPOINTER rgbValue, SQLINTEGER cbValue)
{
  try
  {
    return SQLSetConnectAttr(hdbc, fAttribute, rgbValue, cbValue);
  }
  sql_catch(ex)
}

inline SQLRETURN SqlCloseCursor(SQLHSTMT StatementHandle)
{
  try
  {
    return SQLCloseCursor(StatementHandle);
  }
  sql_catch(ex)
}

inline SQLRETURN SqlPrepare(SQLHSTMT hstmt, SQLCHAR *szSqlStr, SQLINTEGER cbSqlStr)
{
  try
  {
    return SQLPrepare(hstmt, szSqlStr, cbSqlStr);
  }
  sql_catch(ex)
}

inline SQLRETURN SqlNumParams(SQLHSTMT hstmt, SQLSMALLINT *pcpar)
{
  try
  {
    return SQLNumParams(hstmt, pcpar);
  }
  sql_catch(ex)
}

inline SQLRETURN SqlExecute(SQLHSTMT StatementHandle)
{
  try
  {
    return SQLExecute(StatementHandle);
  }
  sql_catch(ex)
}

inline SQLRETURN SqlExecDirect(SQLHSTMT hstmt, SQLCHAR *szSqlStr, SQLINTEGER cbSqlStr)
{
  try
  {
    return SQLExecDirect(hstmt, szSqlStr, cbSqlStr);
  }
  sql_catch(ex)
}

inline SQLRETURN SqlRowCount(SQLHSTMT StatementHandle, SQLLEN* RowCount)
{
  try
  {
    return SQLRowCount(StatementHandle, RowCount);
  }
  sql_catch(ex)
}

inline SQLRETURN SqlFetch(SQLHSTMT StatementHandle)
{
  try
  {
    return SQLFetch(StatementHandle);
  }
  sql_catch(ex)
}

inline SQLRETURN SqlNumResultCols(SQLHSTMT StatementHandle, SQLSMALLINT *ColumnCount)
{
  try
  {
    return SQLNumResultCols(StatementHandle, ColumnCount);
  }
  sql_catch(ex)
}

inline SQLRETURN SqlDescribeCol(SQLHSTMT hstmt, SQLUSMALLINT icol, SQLCHAR *szColName, SQLSMALLINT cbColNameMax, SQLSMALLINT *pcbColName, SQLSMALLINT *pfSqlType, SQLULEN* pcbColDef, SQLSMALLINT *pibScale, SQLSMALLINT *pfNullable)
{
  try
  {
    return SQLDescribeCol(hstmt, icol, szColName, cbColNameMax, pcbColName, pfSqlType, pcbColDef, pibScale, pfNullable);
  }
  sql_catch(ex)
}

inline SQLRETURN SqlBindCol(SQLHSTMT StatementHandle, SQLUSMALLINT ColumnNumber, SQLSMALLINT TargetType, SQLPOINTER TargetValue, SQLLEN BufferLength, SQLLEN *StrLen_or_Ind)
{
  try
  {
    return ::SQLBindCol(StatementHandle, ColumnNumber, TargetType, TargetValue, BufferLength, StrLen_or_Ind);
  }
  sql_catch(ex)
}

inline SQLRETURN SqlBindParameter(SQLHSTMT hstmt, SQLUSMALLINT ipar, SQLSMALLINT fParamType, SQLSMALLINT fCType, SQLSMALLINT fSqlType, SQLULEN cbColDef, SQLSMALLINT ibScale, SQLPOINTER rgbValue, SQLLEN cbValueMax, SQLLEN *pcbValue)
{
  try
  {
    return ::SQLBindParameter(hstmt, ipar, fParamType, fCType, fSqlType, cbColDef, ibScale, rgbValue, cbValueMax, pcbValue);
  }
  sql_catch(ex)
}

inline SQLRETURN SqlCancel(SQLHSTMT hstmt)
{
  try
  {
    return ::SQLCancel(hstmt);
  }
  sql_catch(ex)
}

inline SQLRETURN SqlNativeSql(HDBC hdbc,SQLCHAR* stmt,SQLINTEGER len,SQLCHAR* sql,SQLINTEGER max,SQLINTEGER* reslen)
{
  try
  {
    return ::SQLNativeSql(hdbc,stmt,len,sql,max,reslen);
  }
  sql_catch(ex)
}

inline SQLRETURN SqlDataSources(SQLHENV       EnvironmentHandle
                               ,SQLUSMALLINT  Direction
                               ,SQLCHAR*      ServerName
                               ,SQLSMALLINT   BufferLength1
                               ,SQLSMALLINT*  NameLength1Ptr
                               ,SQLCHAR*      Description
                               ,SQLSMALLINT   BufferLength2
                               ,SQLSMALLINT*  NameLength2Ptr)
{
  try
  {
    return SQLDataSources(EnvironmentHandle,Direction,ServerName,BufferLength1,NameLength1Ptr,Description,BufferLength2,NameLength2Ptr);
  }
  sql_catch(ex)
}

inline SQLRETURN SqlColAttribute(SQLHSTMT     p_hstmt
                                ,SQLUSMALLINT p_column
                                ,SQLUSMALLINT p_field
                                ,SQLPOINTER   p_attribute
                                ,SQLSMALLINT  p_buflen
                                ,SQLSMALLINT* p_strlen
                                ,SQLLEN*      p_numattrib)
{
  try
  {
    return SQLColAttribute(p_hstmt,p_column,p_field,p_attribute,p_buflen,p_strlen,p_numattrib);
  }
  sql_catch(ex)
}

inline SQLRETURN SqlGetData(SQLHSTMT     p_hstmt
                           ,SQLUSMALLINT p_column
                           ,SQLSMALLINT  p_type
                           ,SQLPOINTER   p_value
                           ,SQLLEN       p_bufferlen
                           ,SQLLEN*      p_strlen_or_ind)
{
  try
  {
    return SQLGetData(p_hstmt,p_column,p_type,p_value,p_bufferlen,p_strlen_or_ind);
  }
  sql_catch(ex)
}

inline SQLRETURN SqlDrivers(SQLHENV      EnvironmentHandle
                           ,SQLUSMALLINT Direction
                           ,SQLCHAR*     DriverDescription
                           ,SQLSMALLINT  BufferLength1
                           ,SQLSMALLINT* DescriptionLengthPtr
                           ,SQLCHAR*     DriverAttributes
                           ,SQLSMALLINT  BufferLength2
                           ,SQLSMALLINT* AttributesLengthPtr)
{
  try
  {
    return ::SQLDrivers(EnvironmentHandle
                       ,Direction
                       ,DriverDescription
                       ,BufferLength1
                       ,DescriptionLengthPtr
                       ,DriverAttributes
                       ,BufferLength2
                       ,AttributesLengthPtr);
  }
  sql_catch(ex)
}

inline SQLRETURN SqlPutData(SQLHSTMT   p_hstmt
                           ,SQLPOINTER p_data
                           ,SQLINTEGER p_strlen_or_ind)
{
  try
  {
    return SQLPutData(p_hstmt,p_data,p_strlen_or_ind);
  }
  sql_catch(ex)
}

inline SQLRETURN SqlGetStmtAttr(SQLHSTMT    p_hstmt
                               ,SQLINTEGER  p_attribute
                               ,SQLPOINTER  p_value
                               ,SQLINTEGER  p_buflen
                               ,SQLINTEGER* p_strlen)
{
  try
  {
    return SQLGetStmtAttr(p_hstmt,p_attribute,p_value,p_buflen,p_strlen);
  }
  sql_catch(ex)
}

inline SQLRETURN SqlSetDescField(SQLHDESC    p_handle
                                ,SQLSMALLINT p_record
                                ,SQLSMALLINT p_field
                                ,SQLPOINTER  p_value
                                ,SQLINTEGER  p_bufLength)
{
  try
  {
    return ::SQLSetDescField(p_handle,p_record,p_field,p_value,p_bufLength);
  }
  sql_catch(ex)
}

inline SQLRETURN SqlError(SQLHENV      p_henv
                         ,SQLHDBC      p_hdbc
                         ,SQLHSTMT     p_hstmt
                         ,SQLCHAR*     p_state
                         ,SQLINTEGER*  p_native
                         ,SQLCHAR*     p_text
                         ,SQLSMALLINT  p_buflen
                         ,SQLSMALLINT* p_txtlen)
{
  try
  {
    return SQLError(p_henv,p_hdbc,p_hstmt,p_state,p_native,p_text,p_buflen,p_txtlen);
  }
  sql_catch(ex)
}

inline SQLRETURN SqlGetFunctions(SQLHDBC       p_hdbc
                                ,SQLUSMALLINT  p_id
                                ,SQLUSMALLINT* p_supported)
{
  try
  {
    return SQLGetFunctions(p_hdbc,p_id,p_supported);
  }
  sql_catch(ex)
}

inline SQLRETURN SqlGetTypeInfo(SQLHSTMT p_hstmt,SQLSMALLINT p_type)
{
  try
  {
    return SQLGetTypeInfo(p_hstmt,p_type);
  }
  sql_catch(ex)
}

inline SQLRETURN SqlSetStmtAttr(SQLHSTMT   p_hstmt
                               ,SQLINTEGER p_attribute
                               ,SQLPOINTER p_value
                               ,SQLINTEGER p_strlen)
{
  try
  {
    return SQLSetStmtAttr(p_hstmt,p_attribute,p_value,p_strlen);
  }
  sql_catch(ex)
}

inline SQLRETURN SqlParamData(SQLHSTMT p_hstmt,SQLPOINTER* p_value)
{
  try
  {
    return ::SQLParamData(p_hstmt,p_value);
  }
  sql_catch(ex)
}

inline SQLRETURN SqlMoreResults(SQLHSTMT p_hstmt)
{
  try
  {
    return ::SQLMoreResults(p_hstmt);
  }
  sql_catch(ex)
}

inline SQLRETURN SqlGetCursorName(SQLHSTMT p_hstmt,SQLCHAR* p_buffer,SQLSMALLINT p_buflen,SQLSMALLINT* p_resultLength)
{
  try
  {
    return ::SQLGetCursorName(p_hstmt,p_buffer,p_buflen,p_resultLength);
  }
  sql_catch(ex)
}

// OLD STYLE ODBC 1.x / 2.x call. Do only use if absolutely necessary!!
inline SQLRETURN SqlColAttributes(SQLHSTMT     p_hstmt
                                 ,SQLUSMALLINT p_icol
                                 ,SQLUSMALLINT p_descType
                                 ,SQLPOINTER   p_rgbDesc
                                 ,SQLSMALLINT  p_cbDescMax
                                 ,SQLSMALLINT* p_pcbDesc
                                 ,SQLLEN*      p_pfDesc)
{
  try
  {
    return SQLColAttributes(p_hstmt,p_icol,p_descType,p_rgbDesc,p_cbDescMax,p_pcbDesc,p_pfDesc);
  }
  sql_catch(ex)
}

// End of namespace
}