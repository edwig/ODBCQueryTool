////////////////////////////////////////////////////////////////////////
//
// File: SQLWrappers.h
//
// Copyright (c) 1998- 2014 ir. W.E. Huisman
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
// Last Revision:   01-01-2015
// Version number:  1.1.0
//
#pragma once

// Maak een constante waarmee we de vlag kunnen zien in c++
#ifdef ODBC_NOT_STANDARD
enum { odbc_std_app = false } ;
#else
enum { odbc_std_app = true  } ;
#endif

#pragma warning(disable: 4702)

//////////////////////////////////////////////////////////////////////////
//
// Deze file bevat wrappers voor alle SQLXxx functies om
// access violations en dergelijke op te kunnen vangen.
//

inline SQLRETURN SqlDriverConnect(SQLHDBC hdbc, SQLHWND hwnd, SQLCHAR *szConnStrIn, SQLSMALLINT cbConnStrIn, SQLCHAR *szConnStrOut, SQLSMALLINT cbConnStrOutMax, SQLSMALLINT *pcbConnStrOut, SQLUSMALLINT fDriverCompletion)
{
  try
  {
    return ::SQLDriverConnect(hdbc, hwnd, szConnStrIn, cbConnStrIn, szConnStrOut, cbConnStrOutMax, pcbConnStrOut, fDriverCompletion);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlDisconnect(SQLHDBC ConnectionHandle)
{
  try
  {
    return ::SQLDisconnect(ConnectionHandle);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlFreeHandle(SQLSMALLINT HandleType, SQLHANDLE Handle)
{
  try
  {
    return SQLFreeHandle(HandleType, Handle);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlFreeStmt(HSTMT stmt,SQLUSMALLINT option)
{
  try
  {
    return SQLFreeStmt(stmt,option);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlGetInfo(SQLHDBC hdbc, SQLUSMALLINT fInfoType, SQLPOINTER rgbInfoValue, SQLSMALLINT cbInfoValueMax, SQLSMALLINT* pcbInfoValue)
{
  try
  {
    return SQLGetInfo(hdbc, fInfoType, rgbInfoValue, cbInfoValueMax, pcbInfoValue);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlAllocHandle(SQLSMALLINT HandleType, SQLHANDLE InputHandle, SQLHANDLE *OutputHandle)
{
  try
  {
    return SQLAllocHandle(HandleType, InputHandle, OutputHandle);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlAllocEnv(SQLHENV *EnvironmentHandle)
{
  try
  {
    return SQLAllocEnv(EnvironmentHandle);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlAllocConnect(SQLHENV EnvironmentHandle, SQLHDBC *ConnectionHandle)
{
  try
  {
    return SQLAllocConnect(EnvironmentHandle, ConnectionHandle);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlAllocStmt(SQLHDBC ConnectionHandle, SQLHSTMT *StatementHandle)
{
  try
  {
    return SQLAllocStmt(ConnectionHandle, StatementHandle);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlGetDiagRec(SQLSMALLINT fHandleType, SQLHANDLE handle, SQLSMALLINT iRecord, SQLCHAR *szSqlState, SQLINTEGER *pfNativeError, SQLCHAR *szErrorMsg, SQLSMALLINT cbErrorMsgMax, SQLSMALLINT *pcbErrorMsg)
{
  try
  {
    return SQLGetDiagRec(fHandleType, handle, iRecord, szSqlState, pfNativeError, szErrorMsg, cbErrorMsgMax, pcbErrorMsg);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlEndTran(SQLSMALLINT HandleType, SQLHANDLE Handle, SQLSMALLINT CompletionType)
{
  try
  {
    return SQLEndTran(HandleType, Handle, CompletionType);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlSetConnectAttr(SQLHDBC hdbc, SQLINTEGER fAttribute, SQLPOINTER rgbValue, SQLINTEGER cbValue)
{
  try
  {
    return SQLSetConnectAttr(hdbc, fAttribute, rgbValue, cbValue);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlCloseCursor(SQLHSTMT StatementHandle)
{
  try
  {
    return SQLCloseCursor(StatementHandle);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

#pragma warning (disable: 4996)
inline SQLRETURN SqlSetStmtOption(SQLHSTMT StatementHandle, SQLUSMALLINT Option, SQLULEN Value)
{
  try
  {
    return SQLSetStmtOption(StatementHandle, Option, Value);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlPrepare(SQLHSTMT hstmt, SQLCHAR *szSqlStr, SQLINTEGER cbSqlStr)
{
  try
  {
    return SQLPrepare(hstmt, szSqlStr, cbSqlStr);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlNumParams(SQLHSTMT hstmt, SQLSMALLINT *pcpar)
{
  try
  {
    return SQLNumParams(hstmt, pcpar);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlExecute(SQLHSTMT StatementHandle)
{
  try
  {
    return SQLExecute(StatementHandle);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlExecDirect(SQLHSTMT hstmt, SQLCHAR *szSqlStr, SQLINTEGER cbSqlStr)
{
  try
  {
    return SQLExecDirect(hstmt, szSqlStr, cbSqlStr);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlRowCount(SQLHSTMT StatementHandle, SQLLEN* RowCount)
{
  try
  {
    return SQLRowCount(StatementHandle, RowCount);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlFetch(SQLHSTMT StatementHandle)
{
  try
  {
    return SQLFetch(StatementHandle);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlNumResultCols(SQLHSTMT StatementHandle, SQLSMALLINT *ColumnCount)
{
  try
  {
    return SQLNumResultCols(StatementHandle, ColumnCount);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlDescribeCol(SQLHSTMT hstmt, SQLUSMALLINT icol, SQLCHAR *szColName, SQLSMALLINT cbColNameMax, SQLSMALLINT *pcbColName, SQLSMALLINT *pfSqlType, SQLULEN* pcbColDef, SQLSMALLINT *pibScale, SQLSMALLINT *pfNullable)
{
  try
  {
    return SQLDescribeCol(hstmt, icol, szColName, cbColNameMax, pcbColName, pfSqlType, pcbColDef, pibScale, pfNullable);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlBindCol(SQLHSTMT StatementHandle, SQLUSMALLINT ColumnNumber, SQLSMALLINT TargetType, SQLPOINTER TargetValue, SQLLEN BufferLength, SQLLEN *StrLen_or_Ind)
{
  try
  {
    return ::SQLBindCol(StatementHandle, ColumnNumber, TargetType, TargetValue, BufferLength, StrLen_or_Ind);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlBindParameter(SQLHSTMT hstmt, SQLUSMALLINT ipar, SQLSMALLINT fParamType, SQLSMALLINT fCType, SQLSMALLINT fSqlType, SQLULEN cbColDef, SQLSMALLINT ibScale, SQLPOINTER rgbValue, SQLLEN cbValueMax, SQLLEN *pcbValue)
{
  try
  {
    return ::SQLBindParameter(hstmt, ipar, fParamType, fCType, fSqlType, cbColDef, ibScale, rgbValue, cbValueMax, pcbValue);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlCancel(SQLHSTMT hstmt)
{
  try
  {
    return ::SQLCancel(hstmt);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlNativeSql(HDBC hdbc,SQLCHAR* stmt,SQLINTEGER len,SQLCHAR* sql,SQLINTEGER max,SQLINTEGER* reslen)
{
  try
  {
    return ::SQLNativeSql(hdbc,stmt,len,sql,max,reslen);
  }
  catch(...)
  {
    return SQL_ERROR;
  }
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
  catch (...)
  {
    return SQL_ERROR;
  }
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
  catch (...)
  {
    return SQL_ERROR;
  }
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
  catch (...)
  {
    return SQL_ERROR;
  }
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
  catch(...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlPutData(SQLHSTMT   p_hstmt
                           ,SQLPOINTER p_data
                           ,SQLINTEGER p_strlen_or_ind)
{
  try
  {
    return SQLPutData(p_hstmt,p_data,p_strlen_or_ind);
  }
  catch (...)
  {
  	return SQL_ERROR;
  }
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
  catch (...)
  {
    return SQL_ERROR;
  }
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
  catch (...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlGetFunctions(SQLHDBC       p_hdbc
                                ,SQLUSMALLINT  p_id
                                ,SQLUSMALLINT* p_supported)
{
  try
  {
    return SQLGetFunctions(p_hdbc,p_id,p_supported);
  }
  catch (...)
  {
    return SQL_ERROR;
  }
}

inline SQLRETURN SqlGetTypeInfo(SQLHSTMT p_hstmt,SQLSMALLINT p_type)
{
  try
  {
    return SQLGetTypeInfo(p_hstmt,p_type);
  }
  catch (...)
  {
    return SQL_ERROR;
  }
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
  catch (...)
  {
    return SQL_ERROR;
  }
}

// OLD STYLE ODBC 1.x / 2.x call. Do only use if absolutly nessecary!!
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
  catch (...)
  {
    return SQL_ERROR;
  }
}
