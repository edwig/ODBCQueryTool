////////////////////////////////////////////////////////////////////////
//
// File: SQLQuery.h
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
#include "SQLVariant.h"
#include "SQLDatabase.h"
#include "Locker.h"
#include "bcd.h"
#include <sql.h>
#include <map>

namespace SQLComponents
{

// Arbitrary max limit for a normal string to be retrieved
// Above this limit the stream interface is used
// Can be modified by the "SetBufferSize" method
#define OPTIM_BUFFERSIZE (4*1024)

// Some drivers give no buffer sizes for NVARCHAR back
// So use a maximum default value
#define MAX_CHAR_BUFFER  (32*1024 - 1)

// After this amount of seconds it's been toooooo long
#define QUERY_TOO_LONG 2.0

// Separates SQL statements in a string of batched SQL's
#define SQL_STATEMENT_SEPARATOR "<@>"
#define SQL_SEPARATOR_LENGTH    3

class SQLDate;
class SQLDatabase;

typedef std::map<int,    SQLVariant*> ColNumMap;
typedef std::map<XString,SQLVariant*> ColNameMap;
typedef std::map<int,    SQLVariant*> VarMap;
typedef std::map<int,    unsigned>    MaxSizeMap;

class SQLQuery
{
public:
  // Construct SQL query to be later connected
  SQLQuery();
  // Construct SQL query connected to a database
  SQLQuery(SQLDatabase* p_database);
  SQLQuery(SQLDatabase& p_database);
  SQLQuery(HDBC p_hdbc);
 ~SQLQuery();

  void Init(SQLDatabase* p_database);
  void Init(HDBC p_connection);
  void Close(bool p_throw = true);
  void Open();
  void ResetParameters();

  // BEFORE DOING A QUERY

  // Set the rebind map for datatypes (prior to executing SQL)
  void SetRebindMap(RebindMap* p_map);
  // Set other buffer optimization size
  void SetBufferSize(int p_bufferSize);
  // Set speed threshold for reporting
  void SetSpeedThreshold(double p_seconds);
  // Set maximum rows to get
  void SetMaxRows(int p_maxrows);
  // Setting the locking concurrency level
  void SetConcurrency(int p_concurrency);
  // Setting the maximum size of a SQLCHAR parameter
  void SetParameterMaxSize(int p_num,unsigned p_maxSize);

  // Set parameters for statement
  void SetParameter  (int p_num,SQLVariant*   p_param,SQLParamType p_type = P_SQL_PARAM_INPUT);
  void SetParameter  (int p_num,int           p_param,SQLParamType p_type = P_SQL_PARAM_INPUT);
  void SetParameterUL(int p_num,unsigned int  p_param,SQLParamType p_type = P_SQL_PARAM_INPUT);
  void SetParameter  (int p_num,const char*   p_param,SQLParamType p_type = P_SQL_PARAM_INPUT);
  void SetParameter  (int p_num,XString&      p_param,SQLParamType p_type = P_SQL_PARAM_INPUT);
  void SetParameter  (int p_num,SQLDate&      p_param,SQLParamType p_type = P_SQL_PARAM_INPUT);
  void SetParameter  (int p_num,SQLTime&      p_param,SQLParamType p_type = P_SQL_PARAM_INPUT);
  void SetParameter  (int p_num,SQLTimestamp& p_param,SQLParamType p_type = P_SQL_PARAM_INPUT);
  void SetParameter  (int p_num,const bcd&    p_param,SQLParamType p_type = P_SQL_PARAM_INPUT);

  void SetParameter  (SQLVariant*   p_param,SQLParamType p_type = P_SQL_PARAM_INPUT);
  void SetParameter  (int           p_param,SQLParamType p_type = P_SQL_PARAM_INPUT);
  void SetParameterUL(unsigned int  p_param,SQLParamType p_type = P_SQL_PARAM_INPUT);
  void SetParameter  (const char*   p_param,SQLParamType p_type = P_SQL_PARAM_INPUT);
  void SetParameter  (XString&      p_param,SQLParamType p_type = P_SQL_PARAM_INPUT);
  void SetParameter  (SQLDate&      p_param,SQLParamType p_type = P_SQL_PARAM_INPUT);
  void SetParameter  (SQLTime&      p_param,SQLParamType p_type = P_SQL_PARAM_INPUT);
  void SetParameter  (SQLTimestamp& p_param,SQLParamType p_type = P_SQL_PARAM_INPUT);
  void SetParameter  (const bcd&    p_param,SQLParamType p_type = P_SQL_PARAM_INPUT);

  // Named parameters for DoSQLCall()
  void SetParameterName(int p_num,XString p_name);
  // Set bounded parameters for execute (all in one go)
  void SetParameters(VarMap* p_map);

  // SINGLE STATEMENT

  // Do a SQL statement
  void        DoSQLStatement        (const XString& p_statement);
  SQLVariant* DoSQLStatementScalar  (const XString& p_statement);
  int         DoSQLStatementNonQuery(const XString& p_statement);
  // Overrides with one parameter
  void        DoSQLStatement(const XString& p_statement,const int   p_param1);
  void        DoSQLStatement(const XString& p_statement,const char* p_param1);
  void        DoSQLStatement(const XString& p_statement,const bcd&  p_param1);
  // Variants of the DoSQLStatement
  SQLVariant* DoSQLStatementScalar  (const XString& p_statement,const int   p_param1);
  SQLVariant* DoSQLStatementScalar  (const XString& p_statement,const char* p_param1);
  SQLVariant* DoSQLStatementScalar  (const XString& p_statement,const bcd&  p_param1);
  int         DoSQLStatementNonQuery(const XString& p_statement,const int   p_param1);
  int         DoSQLStatementNonQuery(const XString& p_statement,const char* p_param1);
  int         DoSQLStatementNonQuery(const XString& p_statement,const bcd&  p_param1);
  // Variant with a catch to it
  void        TryDoSQLStatement(const XString& p_statement);
  // Batching multiple SQL statements
  void        DoSQLStatementBatch(XString p_statements);

  // POST PROCESSING of the query result
  // Truncate the char fields in the gotten buffer
  void        TruncateCharFields();
  // Truncate the timestamps to a number of decimals (0 - 6)
  void        TruncateTimestamps(int p_decimals = 0);

  // Call FUNCTION / PROCEDURE
  SQLVariant* DoSQLCall(XString p_schema,XString p_procedure,bool p_hasReturn = false);
  // Overrides with one input parameter and an int return parameter
  SQLVariant* DoSQLCall(XString p_schema,XString p_procedure,const int   p_param1);
  SQLVariant* DoSQLCall(XString p_schema,XString p_procedure,const char* p_param1);
  SQLVariant* DoSQLCall(XString p_schema,XString p_procedure,const bcd&  p_param1);
  // Getting the result parameters values
  SQLVariant* GetParameter(int p_num);

  // BOUND STATEMENT
  // Divide a SQL statement in Prepare/Execute/Fetch
  void        DoSQLPrepare(const XString& p_statement);
  void        DoSQLExecute(bool p_rebind = false);
  // Get bounded columns from query
  ColNumMap*  GetBoundedColumns();

  // Bind application parameters
  void        BindParameters();
  void        BindColumns();

  // Parallel cancellation of the statement
  void        DoCancelQuery();

  // GET RESULTS FROM AN EXECUTED QUERY

  // Get next record of previously executed select statement
  bool        GetRecord();
  // Get a pointer to a variable of the column
  SQLVariant* GetColumn(int icol);
  // Get the name of the SQL cursor for the statement
  XString     GetCursorName();
  // Get the variant data type of the columns
  int         GetColumnType(int icol);
  // Get number of columns in the result set
  int         GetNumberOfColumns() const;
  // Get number of records read so far
  int         GetNumberOfRows();
  // ColumnName -> column number
  int         GetColumnNumber(const char* p_columnName);
  // ColumnNumber -> column name
  bool        GetColumnName(int p_column,XString& p_name);
  // Get length of the column
  int         GetColumnLength(int p_column);
  // Get Display size of the column
  int         GetColumnDisplaySize(int p_column);
  // Getting the database (if any)
  SQLDatabase* GetDatabase();
  // Getting the database handle (if any)
  HDBC        GetDatabaseHandle();
  // Getting the statement handle (if any)
  HSTMT       GetStatementHandle();

  // Getting the results of the query as a SQLVariant reference
  SQLVariant& operator[](int p_index);

  // VARIOUS STATUS METHODS
  // Get true if no error found so far
  bool        IsOk() const;
  // Get the error string
  XString     GetError();
  // See if column is NULL value
  bool        IsNull(int col);
  // See if column is EMPTY value
  bool        IsEmpty(int col);
  // Get version for work-around
  int         GetODBCVersion();

  // LEGACY SUPPORT ODBC 1.x AND 2.x
  void DescribeColumn(int           p_col
                     ,XString&      p_columnName
                     ,XString&      p_colLabel
                     ,SQLSMALLINT&  p_sqlType
                     ,SQLUINTEGER&  p_colSize
                     ,SQLSMALLINT&  p_colScale
                     ,SQLSMALLINT&  p_colNullable
                     ,SQLINTEGER&   p_colDispSize);

private:
  // Set parameter for statement
  void  InternalSetParameter(int p_num,SQLVariant* p_param,SQLParamType p_type = P_SQL_PARAM_INPUT);
  // Bind application parameters
  void  TruncateInputParameters();
  void  BindColumnNumeric(SQLSMALLINT p_column,SQLVariant* p_var,int p_type);

  // Reset all column to NULL
  void  ResetColumns();
  // Fetch the resulting cursor name
  void  FetchCursorName();
  // Convert database dependent SQL_XXXX types to C-types SQL_C_XXXX
  short SQLType2CType(short p_sqlType);
  // Provide piece-by-piece data at exec time of the SQLExecute
  int   ProvideAtExecData();
  // Retrieve the piece-by-piece data at exec time of the SQLFetch
  int   RetrieveAtExecData();
  // Get max column length
  int   GetMaxColumnLength();
  // Get the internal error string
  void  GetLastError(XString p_prefix = "");
  // Report timing to logfile
  void  ReportQuerySpeed(LARGE_INTEGER p_start);
  // Construct the SQL for a function/procedure call
  XString     ConstructSQLForCall(XString& p_schema,XString& p_procedure,bool p_hasReturn);
  // Direct call through ODBC escape language
  SQLVariant* DoSQLCallODBCEscape(XString& p_schema,XString& p_procedure,bool p_hasReturn);
  // Log parameter during the binding process
  void  LogParameter(int p_column,SQLVariant* p_parameter);
  // Do the rebind replacement for a parameter
  short RebindParameter(short p_datatype);
  // Do the rebind replacement for a column
  short RebindColumn(short p_datatype);
  // Character output parameters are sometimes not limited
  void  LimitOutputParameters();

  SQLDatabase*  m_database;          // Database
  HDBC          m_connection;        // In CTOR connection handle.
  HSTMT         m_hstmt;             // Statement handle
  RETCODE       m_retCode;           // last SQL (error)code
  XString       m_lastError;         // last error string
  unsigned      m_maxColumnLength;   // Max length
  bool          m_hasLongColumns;    // Use SQLGetData for long columns
  int           m_bufferSize;        // Alternate Buffer size
  int           m_maxRows;           // Maximum rows to fetch
  double        m_speedThreshold;    // After this amount of seconds, it's taken too long
  int           m_concurrency;       // Concurrency level of the cursor

  XString       m_cursorName;        // Name of the SQL Cursor
  short         m_numColumns;        // Number of result columns in result set
  SQLLEN        m_rows;              // Number of rows processed in INSERT/UPDATE/DELETE
  long          m_fetchIndex;        // Number of rows fetched
  bool          m_prepareDone;       // Internal prepare flag
  bool          m_boundDone;         // Internal binding flag
  bool          m_isSelectQuery;     // Internal SELECT  flag

  VarMap        m_parameters;        // Parameter map at execute
  MaxSizeMap    m_paramMaxSizes;     // Parameter maximum sizes for SQLCHAR parameters
  RebindMap*    m_rebindParameters;  // Rebind map for datatypes of parameter bindings
  RebindMap*    m_rebindColumns;     // Rebind map for datatypes of result columns
  ColNumMap     m_numMap;            // column maps of the derived result set
  ColNameMap    m_nameMap;           // column by names

  // Lock database for multi-access from other threads
  // For as long as the current statement takes
  Locker<SQLDatabase> m_lock;
};

// Set the rebind map for datatypes (prior to executing SQL)
// Simply registers a std::map<int,int> to rebind types
inline void 
SQLQuery::SetRebindMap(RebindMap* p_map)
{
  m_rebindColumns = p_map;
}

inline XString
SQLQuery::GetCursorName()
{
  return m_cursorName;
}

inline void
SQLQuery::SetMaxRows(int p_maxrows)
{
  m_maxRows = p_maxrows;
}

inline bool
SQLQuery::IsOk() const
{
  return SQL_SUCCEEDED(m_retCode);
}

inline int  
SQLQuery::GetNumberOfColumns() const
{
  return (int)m_numMap.size();
}

inline XString
SQLQuery::GetError()
{
  return m_lastError;
}

inline ColNumMap* 
SQLQuery::GetBoundedColumns()
{
  return &m_numMap;
}

inline void 
SQLQuery::SetSpeedThreshold(double p_seconds)
{
  m_speedThreshold = p_seconds;
}

inline SQLVariant&
SQLQuery::operator[](int p_index)
{
  return *GetColumn(p_index);
}

inline SQLDatabase*
SQLQuery::GetDatabase()
{
  return m_database;
}

// End of namespace
}