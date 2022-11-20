////////////////////////////////////////////////////////////////////////
//
// File: SQLQuery.cpp
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
#include "stdafx.h"
#include "SQLComponents.h"
#include "SQLDatabase.h"
#include "SQLInfoDB.h"
#include "SQLQuery.h"
#include "SQLWrappers.h"
#include "SQLDate.h"
#include "bcd.h"
#include <sqlext.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{

// CTOR: To be later connected to a database
// by calling Init() seperatly
SQLQuery::SQLQuery()
         :m_lock(NULL,INFINITE)
         ,m_database(NULL)
{
  Init((SQLDatabase*)NULL);
}

// CTOR: After this we're good to go
SQLQuery::SQLQuery(SQLDatabase* p_database)
         :m_lock(p_database,INFINITE)
{
  Init(p_database);
}

// CTOR: Also for a reference to a database
SQLQuery::SQLQuery(SQLDatabase& p_database)
         :m_lock(&p_database,INFINITE)
{
  Init(&p_database);
}

// CTOR on a stand-alone handle
SQLQuery::SQLQuery(HDBC p_hdbc)
         :m_lock(NULL,INFINITE)
{
  Init(p_hdbc);
}

SQLQuery::~SQLQuery()
{
  Close(false);
  ResetParameters();
}

void 
SQLQuery::Init(SQLDatabase* p_database)
{
  m_database         = p_database;
  m_hstmt            = 0;
  m_retCode          = SQL_SUCCESS;
  m_numColumns       = -1;
  m_rows             = 0;
  m_fetchIndex       = 0;
  m_rebindParameters = NULL;
  m_rebindColumns    = NULL;
  m_hasLongColumns   = false;
  m_bufferSize       = 0;
  m_prepareDone      = false;
  m_boundDone        = false;
  m_lastError        = "";
  m_maxRows          = 0;
  m_maxColumnLength  = 0;
  m_isSelectQuery    = false;
  m_speedThreshold   = QUERY_TOO_LONG;
  m_connection       = NULL;
  m_concurrency      = SQL_CONCUR_READ_ONLY;
}

void
SQLQuery::Init(HDBC p_connection)
{
  Init((SQLDatabase*)NULL);
  m_connection = p_connection;
}

void
SQLQuery::Close(bool p_throw /*= true*/)
{
  XString error;

  // Statement reset
  if(m_hstmt)
  {
    // If a cursor was opened (binding columns present)
    // And not read until the end of the cursor stream
    // Try closing the cursor first
    if(m_numMap.size() && m_retCode != SQL_NO_DATA)
    {
      m_retCode = ::SQLCloseCursor(m_hstmt);
      if(!SQL_SUCCEEDED(m_retCode))
      {
        GetLastError("Closing the cursor: ");
        error += m_lastError;

        // Some databases give a SQLSTATE = 24000 if the cursor was at the end
        // This is documented behavior of SQLCloseCursor
        if(m_database)
        {
          if(m_database->GetSQLState() != "24000")
          {
            m_database->LogPrint(m_lastError);
            m_database->LogPrint("Trying to continue without closing the cursor!");
          }
        }
      }
    }

    // Free the statement and drop all associated info
    // And all cursors on the database engine
    m_retCode = SQLDatabase::FreeSQLHandle(&m_hstmt,SQL_DROP);
    if(!SQL_SUCCEEDED(m_retCode))
    {
      GetLastError("Freeing the cursor: ");
      error += m_lastError;
    }
  }
  // Clear number map
  for(auto& column : m_numMap)
  {
    delete column.second;
  }
  m_numMap.clear();
  m_nameMap.clear();

  // Reset other variables
  m_lastError.Empty();
  m_cursorName.Empty();
  m_hasLongColumns  = false;
  m_maxColumnLength = 0;
  m_bufferSize      = 0;
  m_maxRows         = 0;
  m_numColumns      = 0;
  m_rows            = 0;
  m_fetchIndex      = 0;
  m_prepareDone     = false;
  m_boundDone       = false;
  m_concurrency = SQL_CONCUR_READ_ONLY;
  // LEAVE ALONE THESE PARAMETERS FOR REUSE OF THE QUERY
  // m_database
  // m_connection
  // m_parameters
  if(p_throw && !error.IsEmpty())
  {
    throw StdException(error);
  }
}

void
SQLQuery::ResetParameters()
{
  // Clear parameter map
  for(auto& parm : m_parameters)
  {
    delete parm.second;
  }
  m_parameters.clear();
}

void
SQLQuery::Open()
{
  // get statement handle in m_hstmt:
  if(m_database)
  {
    m_retCode = m_database->GetSQLHandle(&m_hstmt,false);
    if(!SQL_SUCCEEDED(m_retCode))
    {
      GetLastError("Getting statement handle: ");
      throw StdException(m_lastError);
    }
    m_connection = m_database->GetDBHandle();
  }
  else
  {
    if(m_connection == SQL_NULL_HANDLE)
    {
      throw StdException("No database handle. Are you logged in to a database?");
    }
    // Create the statement handle
    SQLRETURN res = SqlAllocHandle(SQL_HANDLE_STMT,m_connection,&m_hstmt);
    if(!SQL_SUCCEEDED(res))
    {
      GetLastError("Error creating a statement handle: ");
      throw StdException(m_lastError);
    }
  }

  if(m_hstmt == NULL)
  {
    throw StdException("No database connection at SQLQUery::Open function");
  }
  // DISABLED!!
  // Without escape scanning other things will go wrong.
  // For instance if a query ends on a "? " : So "<Placeholder><space>"

  // SPEED: Do not look for ODBC escapes sequences
  //   m_retCode = SQLSetStmtAttr(m_hstmt,SQL_ATTR_NOSCAN,(SQLPOINTER)SQL_NOSCAN_ON,SQL_IS_UINTEGER);
  //   if (!SQL_SUCCEEDED(m_retCode))
  //   {
  //     GetLastError("Cannot set NOSCAN attribute: ");
  //     throw StdException(m_lastError);
  //   }

  // Change cursor type to the cheapest qua performance
  m_retCode = SqlSetStmtAttr(m_hstmt,SQL_CURSOR_TYPE,(SQLPOINTER)SQL_CURSOR_FORWARD_ONLY,SQL_IS_UINTEGER);
  if(!SQL_SUCCEEDED(m_retCode))
  {
    GetLastError("Cannot set CURSOR-FORWARD-ONLY attribute: ");
    throw StdException(m_lastError);
  }

  // Set max-rows if our database DOES support it (Oracle!)
  if(m_maxRows)
  {
    m_retCode = SqlSetStmtAttr(m_hstmt,SQL_MAX_ROWS,(SQLPOINTER)(DWORD_PTR)m_maxRows,SQL_IS_UINTEGER);
    if(!SQL_SUCCEEDED(m_retCode))
    {
      GetLastError("Cannot set MAX_ROWS attribute: ");
      throw StdException(m_lastError);
    }
  }

  // Setting the concurrency level of the cursor
  if(m_concurrency > SQL_CONCUR_READ_ONLY)
  {
    m_retCode = SqlSetStmtAttr(m_hstmt,SQL_ATTR_CONCURRENCY,(SQLPOINTER)(DWORD_PTR)m_concurrency,SQL_IS_UINTEGER);
    if(!SQL_SUCCEEDED(m_retCode))
    {
      GetLastError("Cannot set CONCURRENCY attribute: ");
      throw StdException(m_lastError);
    }
  }

  // Solving formatting for various databases (Oracle / MS-Access)
  // So they must be set or gotten in a predefined format (e.g. Oracle needs LONG for SLONG and ULONG)
  // Also see method "SQLType2CType" for the use of the rebind maps
  if(m_database)
  {
    m_rebindParameters = m_database->GetRebindMapParameters();
    m_rebindColumns    = m_database->GetRebindMapColumns();
  }
}

// Set all columns to NULL
void
SQLQuery::ResetColumns()
{
  for(auto& col : m_numMap)
  {
    col.second->SetNULL();
  }
}

// Set other buffer optimization size
void 
SQLQuery::SetBufferSize(int p_bufferSize)
{
  if(p_bufferSize > 0 && p_bufferSize < OPTIM_BUFFERSIZE)
  {
    m_bufferSize = p_bufferSize;
  }
}

// Setting the locking concurrency level
// Valid values are (See: sqlext.h)
// SQL_CONCUR_READ_ONLY   = 1
// SQL_CONCUR_LOCK        = 2
// SQL_CONCUR_ROWVER      = 3
// SQL_CONCUR_VALUES      = 4
void 
SQLQuery::SetConcurrency(int p_concurrency)
{
  if(p_concurrency >= SQL_CONCUR_READ_ONLY &&
     p_concurrency <= SQL_CONCUR_VALUES)
  {
    m_concurrency = p_concurrency;
  }
}

int
SQLQuery::GetODBCVersion()
{
  if(m_database)
  {
    return m_database->GetODBCVersion();
  }
  // Default is at least 3.x 
  return 3;
}

// Report timing to logfile
void
SQLQuery::ReportQuerySpeed(LARGE_INTEGER p_start)
{
  // Cannot do reporting for a 'lose' SQLQuery
  if(m_database == NULL)
  {
    return;
  }

  // Getting the counters
  long seconds = 0;
  LARGE_INTEGER einde;
  LARGE_INTEGER freq;
  QueryPerformanceFrequency(&freq);
  QueryPerformanceCounter(&einde);

  double secondsDBL = ((double)(einde.QuadPart - p_start.QuadPart)) / (double)freq.QuadPart;
  seconds = (long) secondsDBL; // rounding

  XString message;
  if(seconds > m_speedThreshold)
  {
    message.Format("[999] Query too long: %.6f seconds\n",secondsDBL);
  }
  else
  {
    message.Format("Query time: %.6f seconds\n",secondsDBL);
  }
  m_database->LogPrint(message);
}

//////////////////////////////////////////////////////////////////////////
//
// Set parameters for statement
//
//////////////////////////////////////////////////////////////////////////

// Private and hidden setting of the parameter
// The p_param is **NOT** copied, but stored and owned by the SQLQuery
void
SQLQuery::InternalSetParameter(int p_num,SQLVariant* p_param,SQLParamType p_type /*=P_SQL_PARAM_INPUT*/)
{
  p_param->SetParameterType(p_type);
  VarMap::iterator it = m_parameters.find(p_num);
  if(it == m_parameters.end())
  {
    m_parameters.insert(std::make_pair(p_num,p_param));
  }
  else
  {
    SQLVariant* var = it->second;
    it->second = p_param;
    delete var;
  }
}

// Setting the maximum size of a SQLCHAR parameter
void 
SQLQuery::SetParameterMaxSize(int p_num,unsigned p_maxSize)
{
  MaxSizeMap::iterator it = m_paramMaxSizes.find(p_num);
  if(it == m_paramMaxSizes.end())
  {
    m_paramMaxSizes.insert(std::make_pair(p_num,p_maxSize));
  }
  else
  {
    it->second = p_maxSize;
  }
}

// Setting a parameter. Copies the SQLVariant!!
void
SQLQuery::SetParameter(int p_num,SQLVariant* p_param,SQLParamType p_type /*=SQL_PARAM_INPUT*/)
{
  SQLVariant* var = new SQLVariant(p_param);
  InternalSetParameter(p_num,var,p_type);
}

void 
SQLQuery::SetParameter(int p_num,int p_param,SQLParamType p_type /*=SQL_PARAM_INPUT*/)
{
  SQLVariant* var = new SQLVariant(p_param);
  InternalSetParameter(p_num,var,p_type);
}

void 
SQLQuery::SetParameterUL(int p_num,unsigned int p_param,SQLParamType p_type /*=SQL_PARAM_INPUT*/)
{
  SQLVariant *var = new SQLVariant(p_param);
  InternalSetParameter(p_num,var,p_type);
}

void 
SQLQuery::SetParameter(int p_num,const char* p_param,SQLParamType p_type /*=SQL_PARAM_INPUT*/)
{
  SQLVariant* var = new SQLVariant(p_param);
  InternalSetParameter(p_num,var,p_type);
}

void 
SQLQuery::SetParameter(int p_num,XString& p_param,SQLParamType p_type /*=SQL_PARAM_INPUT*/)
{
  SQLVariant* var = new SQLVariant(p_param);
  InternalSetParameter(p_num,var,p_type);
}

void 
SQLQuery::SetParameter(int p_num,SQLDate& p_param,SQLParamType p_type /*=SQL_PARAM_INPUT*/)
{
  SQLVariant* var = new SQLVariant(&p_param);
  InternalSetParameter(p_num,var,p_type);
}

void 
SQLQuery::SetParameter (int p_num,SQLTime& p_param,SQLParamType p_type /*=SQL_PARAM_INPUT*/)
{
  SQLVariant* var = new SQLVariant(&p_param);
  InternalSetParameter(p_num,var,p_type);
}

void 
SQLQuery::SetParameter(int p_num,SQLTimestamp& p_param,SQLParamType p_type /*=SQL_PARAM_INPUT*/)
{
  SQLVariant* var = new SQLVariant(&p_param);
  InternalSetParameter(p_num,var,p_type);
}

void
SQLQuery::SetParameter(int p_num,const bcd& p_param,SQLParamType p_type /*=SQL_PARAM_INPUT*/)
{
  SQLVariant* var = new SQLVariant(&p_param);
  InternalSetParameter(p_num,var,p_type);
}

//////////////////////////////////////////////////////////////////////////
// Variants of SetParameter without an explicit parameter number
//////////////////////////////////////////////////////////////////////////

// Setting a parameter. Copies the SQLVariant!!
void
SQLQuery::SetParameter(SQLVariant* p_param,SQLParamType p_type /*=SQL_PARAM_INPUT*/)
{
  int size = (int) m_parameters.size() + 1;
  SQLVariant* var = new SQLVariant(p_param);
  InternalSetParameter(size,var,p_type);
}

void
SQLQuery::SetParameter(int p_param,SQLParamType p_type /*=SQL_PARAM_INPUT*/)
{
  int size = (int)m_parameters.size() + 1;
  SQLVariant* var = new SQLVariant(p_param);
  InternalSetParameter(size,var,p_type);
}

void
SQLQuery::SetParameterUL(unsigned int p_param,SQLParamType p_type /*=SQL_PARAM_INPUT*/)
{
  int size = (int)m_parameters.size() + 1;
  SQLVariant *var = new SQLVariant(p_param);
  InternalSetParameter(size,var,p_type);
}

void
SQLQuery::SetParameter(const char* p_param,SQLParamType p_type /*=SQL_PARAM_INPUT*/)
{
  int size = (int)m_parameters.size() + 1;
  SQLVariant* var = new SQLVariant(p_param);
  InternalSetParameter(size,var,p_type);
}

void
SQLQuery::SetParameter(XString& p_param,SQLParamType p_type /*=SQL_PARAM_INPUT*/)
{
  int size = (int)m_parameters.size() + 1;
  SQLVariant* var = new SQLVariant(p_param);
  InternalSetParameter(size,var,p_type);
}

void
SQLQuery::SetParameter(SQLDate& p_param,SQLParamType p_type /*=SQL_PARAM_INPUT*/)
{
  int size = (int)m_parameters.size() + 1;
  SQLVariant* var = new SQLVariant(&p_param);
  InternalSetParameter(size,var,p_type);
}

void
SQLQuery::SetParameter(SQLTime& p_param,SQLParamType p_type /*=SQL_PARAM_INPUT*/)
{
  int size = (int)m_parameters.size() + 1;
  SQLVariant* var = new SQLVariant(&p_param);
  InternalSetParameter(size,var,p_type);
}

void
SQLQuery::SetParameter(SQLTimestamp& p_param,SQLParamType p_type /*=SQL_PARAM_INPUT*/)
{
  int size = (int)m_parameters.size() + 1;
  SQLVariant* var = new SQLVariant(&p_param);
  InternalSetParameter(size,var,p_type);
}

void
SQLQuery::SetParameter(const bcd& p_param,SQLParamType p_type /*=SQL_PARAM_INPUT*/)
{
  int size = (int)m_parameters.size() + 1;
  SQLVariant* var = new SQLVariant(&p_param);
  InternalSetParameter(size,var,p_type);
}

// Named parameters for DoSQLCall()
void 
SQLQuery::SetParameterName(int p_param,XString p_name)
{
  SQLVariant* var = GetParameter(p_param);
  var->SetColumnNumber(p_param);
  // Keep as name
  m_nameMap.insert(std::make_pair(p_name,var));
}

// Set parameters from another SQLQuery
void
SQLQuery::SetParameters(VarMap* p_map)
{
  if(p_map)
  {
    m_parameters = *p_map;
  }
  else
  {
    m_parameters.clear();
  }
}

//////////////////////////////////////////////////////////////////////////
//
// STATEMENTS
//
//////////////////////////////////////////////////////////////////////////

void 
SQLQuery::DoSQLStatement(const XString& p_statement,int p_param1)
{
  SetParameter(1,p_param1);
  DoSQLStatement(p_statement);
}

void 
SQLQuery::DoSQLStatement(const XString& p_statement,const char* p_param1)
{
  SetParameter(1,p_param1);
  DoSQLStatement(p_statement);
}

void
SQLQuery::DoSQLStatement(const XString& p_statement,const bcd&  p_param1)
{
  SetParameter(1,p_param1);
  DoSQLStatement(p_statement);
}

void
SQLQuery::DoSQLStatement(const XString& p_statement)
{
  bool logging = false;

  // Check for filled statement
  if(p_statement.IsEmpty())
  {
    m_lastError = "Error in SQL statement: Empty statement.";
    throw StdException(m_lastError);
  }
  // Begin of query clock
  LARGE_INTEGER start;
  QueryPerformanceCounter(&start);

  // Close last cursor/statement and open a new one
  Close();
  Open();

  // See if it is a 'SELECT' query
  m_isSelectQuery = false;
  if(p_statement.Left(6).CompareNoCase("select") == 0)
  {
    m_isSelectQuery = true;
  }

  // Bind parameters
  BindParameters();

  // In special cases queries can go wrong through ORACLE ODBC if they contain newlines
  // So we need to remove trailing white and red space
  XString statement(p_statement);
  statement.TrimRight("\n\r\t\f ");

  // Do the Query text macro replacement
  if(m_database)
  {
    m_database->ReplaceMacros(statement);
  }

  // Log the query, just before we run it, replaced macro's and all
  if(m_database && m_database->WilLog())
  {
    logging = true;
    m_database->LogPrint("[Database query]\n");
    m_database->LogPrint(statement.GetString());
    m_database->LogPrint("\n");
  }

  // The Oracle 10.2.0.3.0 ODBC Driver - and later versions - contain a bug
  // in the processing of the query-strings which crashes it in CharNextW
  // by a missing NULL-Terminator. By changing the length of the statement
  // _including_ the terminating NUL, it won't crash at all
  // NOTE: This also means we cannot use the SQL_NTS terminator
  SQLINTEGER lengthStatement = statement.GetLength() + 1;

  // GO DO IT RIGHT AWAY
  m_retCode = SqlExecDirect(m_hstmt,(SQLCHAR*)statement.GetString(),lengthStatement);

  if(SQL_SUCCEEDED(m_retCode))
  {
    // rcExec == SQL_SUCCESS
    // rcExec == SQL_SUCCESS_WITH_INFO
    // rcExec == SQL_NO_DATA
    // get number of result columns -> m_numColumns
    m_retCode = SqlNumResultCols(m_hstmt,&m_numColumns);
    if(SQL_SUCCEEDED(m_retCode))
    {
      if(m_numColumns > 0)
      {
        BindColumns();
      }
    }
    else
    {
      XString fout;
      fout.Format("Error [%d] in getting the number of columns from a query: ",m_retCode);
      GetLastError(fout);
      throw StdException(m_lastError);
    }
    FetchCursorName();
  }
  else if(m_retCode < 0)
  {
    // rcExec == SQL_ERROR
    // rcExec == SQL_INVALID_HANDLE
    GetLastError("Error in SQL statement: ");
    throw StdException(m_lastError);
  }
  else
  {
    // rcExec == SQL_NO_DATA
    // rcExec == SQL_STILL_EXECUTING
    // rcExec == SQL_NEED_DATA
  }
  if(logging)
  {
    ReportQuerySpeed(start);
  }
}

// Variant with a catch to it
void 
SQLQuery::TryDoSQLStatement(const XString& p_statement)
{
  try
  {
    DoSQLStatement(p_statement);
  }
  catch(StdException& error)
  {
    ReThrowSafeException(error);
    // Do Nothing, ignoring the error
  }
}

SQLVariant* 
SQLQuery::DoSQLStatementScalar(const XString& p_statement)
{
  DoSQLStatement(p_statement);
  if(GetRecord())
  {
    return GetColumn(1);
  }
  return NULL;
}

SQLVariant* 
SQLQuery::DoSQLStatementScalar(const XString& p_statement,const int p_param1)
{
  SetParameter(1,p_param1);
  return DoSQLStatementScalar(p_statement);
}

SQLVariant* 
SQLQuery::DoSQLStatementScalar(const XString& p_statement,const char* p_param1)
{
  SetParameter(1,p_param1);
  return DoSQLStatementScalar(p_statement);
}

SQLVariant* 
SQLQuery::DoSQLStatementScalar(const XString& p_statement,const bcd&  p_param1)
{
  SetParameter(1,p_param1);
  return DoSQLStatementScalar(p_statement);
}

int
SQLQuery::DoSQLStatementNonQuery(const XString& p_statement,const int p_param1)
{
  SetParameter(1,p_param1);
  return DoSQLStatementNonQuery(p_statement);
}

int
SQLQuery::DoSQLStatementNonQuery(const XString& p_statement,const char* p_param1)
{
  SetParameter(1,p_param1);
  return DoSQLStatementNonQuery(p_statement);
}

int
SQLQuery::DoSQLStatementNonQuery(const XString& p_statement,const bcd&  p_param1)
{
  SetParameter(1,p_param1);
  return DoSQLStatementNonQuery(p_statement);
}

int
SQLQuery::DoSQLStatementNonQuery(const XString& p_statement)
{
  if(p_statement.Left(6).CompareNoCase("select") == 0)
  {
    throw StdException("SQL Non-query cannot contain a 'SELECT' statement");
  }
  DoSQLStatement(p_statement);
  return GetNumberOfRows();
}

// Batching multiple SQL statements by "<@>" separation
// Only Non-Query statements can be run in the top of the batch
// Only the LAST statement can be a SELECT statement!
// AND all parameters across all statements must be EXACTLY the same!
void        
SQLQuery::DoSQLStatementBatch(XString p_statements)
{
  XString statement;
  int pos = p_statements.Find(SQL_STATEMENT_SEPARATOR);

  while((pos >= 0) || !p_statements.IsEmpty())
  {
    // Find one statement
    statement = (pos >= 0) ? p_statements.Left(pos) : p_statements;

    // Fire the statement
    if(!statement.IsEmpty())
    {
      if(pos >= 0)
      {
        DoSQLStatementNonQuery(statement);
      }
      else
      {
        DoSQLStatement(statement);
      }
    }
    // Find next statement
    if(pos >= 0)
    {
      p_statements = p_statements.Mid(pos + SQL_SEPARATOR_LENGTH);
      pos = p_statements.Find(SQL_STATEMENT_SEPARATOR);
    }
    else
    {
      p_statements.Empty();
    }
  }
}

void 
SQLQuery::DoSQLPrepare(const XString& p_statement)
{
  if(p_statement.IsEmpty())
  {
    m_lastError = "Error in SQL statement: Empty statement.";
    throw StdException(m_lastError);
  }
  // close last m_hstmt if still open
  Close();
  Open();

  // In special cases queries can go wrong through and ORACLE ODBC if they contain newlines
  // Hence all newlines are replaces by spaces, if the query does NOT contain any comments
  XString statement(p_statement);
  if(statement.Find("--") < 0)
  {
    statement.Replace("\n"," ");
    statement.Replace("\r"," ");
  }
  // Optimization: remove trailing spaces
  statement.Trim();

  // Do the Query text macro replacement
  if(m_database)
  {
    m_database->ReplaceMacros(statement);
  }

  // See if it is a 'SELECT' query
  m_isSelectQuery = false;
  if(p_statement.Left(6).CompareNoCase("select") == 0)
  {
    m_isSelectQuery = true;
  }

  // Log the query, just before we run it, replaced macro's and all
  if(m_database && m_database->WilLog())
  {
    m_database->LogPrint("[Database query]\n");
    m_database->LogPrint(statement.GetString());
    m_database->LogPrint("\n");
  }

  // The Oracle 10.2.0.3.0 ODBC Driver - and later versions - contain a bug
  // in the processing of the query-strings which crashes it in CharNexW
  // by a missing NUL-Terminator. By changing the length of the statement
  // _including_ the terminating NUL, it won't crash at all
  SQLINTEGER lengthStatement = statement.GetLength() + 1;

  // GO DO THE PREPARE
  m_retCode = SqlPrepare(m_hstmt,(SQLCHAR*)(LPCSTR)statement,lengthStatement);
  if(SQL_SUCCEEDED(m_retCode))
  {
    m_prepareDone = true;
  }
  if(m_retCode < 0)
  {
    // rcExec == SQL_ERROR
    // rcExec == SQL_INVALID_HANDLE
    GetLastError("Error in SQL statement: ");
    throw StdException(m_lastError);
  }
}

void
SQLQuery::DoSQLExecute(bool p_rebind /*=false*/)
{
  m_retCode = SQL_ERROR;

  if(!m_prepareDone)
  {
    m_lastError = "Internal error: SQLExecute without SQLPrepare.";
    throw StdException(m_lastError);
  }
  
  if(!m_boundDone || p_rebind)
  {
    BindParameters();
  }

  // Go execute it (again)
  m_retCode = SqlExecute(m_hstmt);
  if(m_retCode == SQL_NEED_DATA)
  {
    m_retCode = (short)ProvideAtExecData();
  }

  if(SQL_SUCCEEDED(m_retCode))
  {
    // rcExec == SQL_SUCCESS
    // rcExec == SQL_SUCCESS_WITH_INFO
    // get number of result columns -> m_numColumns
    m_retCode = SqlNumResultCols(m_hstmt,&m_numColumns);

    if(SQL_SUCCEEDED(m_retCode))
    {
      if(m_numColumns > 0)
      {
        BindColumns();
      }
    }
    else
    {
      XString fout;
      fout.Format("Error [%d] in determining the number of columns in the query: ",m_retCode);
      GetLastError(fout);
      throw StdException(m_lastError);
    }
    FetchCursorName();
  }
  else if(m_retCode < 0)
  {
    // rcExec == SQL_ERROR
    // rcExec == SQL_INVALID_HANDLE
    GetLastError("Error in SQL statement: ");
    throw StdException(m_lastError);
  }
  else
  {
    // rcExec == SQL_NO_DATA
    // rcExec == SQL_STILL_EXECUTING
  }
  // Do bindings only once in a prepare -> multiple-execute cycle
  m_boundDone = true;
}

// Bind application parameters
// Override for other methods as SQLVariant
void
SQLQuery::BindParameters()
{
  bool logging = false;

  // Optimize for no-parameters
  if(m_parameters.empty())
  {
    return;
  }

  // See if we must truncate input CHAR parameters
  if(!m_paramMaxSizes.empty())
  {
    TruncateInputParameters();
  }

  // Optimize logging
  if(m_database && m_database->WilLog())
  {
    logging = true;
  }

  // See if we have an extra function-return parameter
  int extra = (m_parameters.find(0) == m_parameters.end()) ? 0 : 1;

  // Walk the parameter list
  for(auto& parameter : m_parameters)
  {
    // Getting the variant for the parameter for a column
    SQLINTEGER  icol = parameter.first + extra;
    SQLVariant* var  = parameter.second;

    // Getting the info from the variable
    SQLSMALLINT scale       = (SQLSMALLINT)var->GetNumericScale();
    SQLPOINTER  dataPointer = (SQLPOINTER )var->GetDataPointer();
    SQLSMALLINT dataType    = (SQLSMALLINT)var->GetDataType();
    SQLSMALLINT sqlDatatype = (SQLSMALLINT)var->GetSQLDataType();
    SQLSMALLINT paramType   = (SQLSMALLINT)var->GetParameterType();
    SQLUINTEGER columnSize  = var->GetDataSize();
    SQLINTEGER  bufferSize  = var->GetDataSize();

    // Check for an at-execution-streaming of values
    if(var->GetAtExec())
    {
      // AT EXEC data piece by piece
      dataPointer = (SQLPOINTER)(DWORD_PTR) icol;
      // Some database types need to know the length beforehand (Oracle!)
      // If no database type known, set to true, just to be sure!
      var->SetSizeIndicator(m_database ? m_database->GetNeedLongDataLen() : true);
      bufferSize = 0;
    }

    // Check rebinds to do for scripting 
    sqlDatatype = RebindParameter(sqlDatatype);

    // Check minimum for an input type
    if(paramType == SQL_PARAM_TYPE_UNKNOWN)
    {
      paramType = SQL_PARAM_INPUT;
      var->SetParameterType(SQLParamType::P_SQL_PARAM_INPUT);
    }

    // Ugly hack!!
    if(m_database && m_database->GetDatabaseType() == RDBMS_SQLSERVER)
    {
      if((sqlDatatype == SQL_CHAR ||sqlDatatype == SQL_VARCHAR ) && columnSize > 8000)
      {
        columnSize = 8000;
      }
    }

    // Log what we bind here
    if(logging)
    {
      LogParameter(icol,var);
    }

    // Do the bindings
    m_retCode = SqlBindParameter(m_hstmt                     // Statement handle
                                ,(ushort)icol                // Number of parameter
                                ,paramType                   // SQL_PARAM_INPUT etc
                                ,dataType                    // SQL_C_XXX Types
                                ,sqlDatatype                 // SQL_XXX Type
                                ,columnSize                  // Column size
                                ,scale                       // Numeric scale
                                ,dataPointer                 // Buffer pointer
                                ,bufferSize                  // Buffer size (truncate on output)
                                ,var->GetIndicatorPointer());// NULL indicator
    if(!SQL_SUCCEEDED(m_retCode))
    {
      GetLastError("Cannot bind parameter. Error: ");
      m_lastError.AppendFormat(" Parameter: %d",icol);
      throw StdException(m_lastError);
    }

    // Bind NUMERIC/DECIMAL precision and scale
    // But only if the sql datatype and the C++ datatype are the same (a SQLVariant from the application)
    // In case of a SQLVariant that came from the ODBC driver earlier we may NOT rebind the precision and scale
    // Oracle ODBC drivers beyond 12.x.x.x.x **WILL** crash on that
    if(dataType == SQL_C_NUMERIC && (dataType == sqlDatatype))
    {
      BindColumnNumeric((ushort)icol,var,SQL_PARAM_INPUT);
    }
  }
}

// Truncate CHAR parameters on input to reflect the size of the database column
// Parameter size can be set with SetParameterMaxSize()!
void
SQLQuery::TruncateInputParameters()
{
  for(auto& param : m_parameters)
  {
    SQLVariant* parm = param.second;

    // If CHAR/VARCHAR column does not support more than this amount of characters
    if(parm->GetDataType() == SQL_C_CHAR && parm->GetParameterType() == P_SQL_PARAM_INPUT)
    {
      MaxSizeMap::iterator ms = m_paramMaxSizes.find(param.first);
      if (ms != m_paramMaxSizes.end())
      {
        parm->TruncateSpace(ms->second);
      }
    }
  }
}

// Do the rebind parameter datatype replacement
short
SQLQuery::RebindParameter(short p_datatype)
{
  if(m_rebindParameters)
  {
    RebindMap::iterator rt = m_rebindParameters->find(p_datatype);
    if(rt != m_rebindParameters->end())
    {
      return (SQLSMALLINT)rt->second;
    }
  }
  return p_datatype;
}

// Log parameter during the binding process
void
SQLQuery::LogParameter(int p_column,SQLVariant* p_parameter)
{
  if(p_column == 1)
  {
    m_database->LogPrint("Parameters as passed on to the database:\n");
  }
  XString text,value;
  p_parameter->GetAsString(value);
  text.Format("Parameter %d: %s\n",p_column,value.GetString());
  m_database->LogPrint(text);
}

void
SQLQuery::BindColumns()
{
  SQLULEN     precision = 0;
  SQLSMALLINT scale     = 0;
  SQLSMALLINT dummy     = 0;
  SQLSMALLINT nullable  = 0;
  SQLSMALLINT dataType  = 0;
  SQLCHAR     colName[SQL_MAX_IDENTIFIER + 1];
  unsigned short icol;
  int atexec = 0;

  GetMaxColumnLength();

  // COLLECT INFO FOR ALL COLUMNS AND CREATE VARIANTS

  for(icol = 1; icol <= m_numColumns; ++icol)
  {
    atexec = 0;
    scale  = SQLNUM_DEF_SCALE;
 
    // Getting the info for this column
    m_retCode = SqlDescribeCol(m_hstmt            // statement handle
                              ,icol               // Column number
                              ,colName            // Column name
                              ,SQL_MAX_IDENTIFIER // name buffer length
                              ,&dummy             // actual name length gotten
                              ,&dataType          // SQL data type (SQL_XX)
                              ,&precision         // precision of numbers
                              ,&scale             // decimal scale
                              ,&nullable);        // NULL values OK?
    if(!SQL_SUCCEEDED(m_retCode))
    {
      XString fout;
      fout.Format("Error [%d] at the determining of the attributes of column [%d] : ",m_retCode,icol);
      GetLastError(fout);
      throw StdException(m_lastError);
    }

    int type = SQLType2CType(dataType);
    XString name(colName);
    if(type == SQL_C_CHAR || type == SQL_C_BINARY)
    {
      if(precision == 0)
      {
        // Some ODBC drivers do not give you the length of a NVARCHAR type column
        // All we can do here is to proceed with fingers crossed (and max buffers)
        // Found in MS-SQLServer
        precision = MAX_CHAR_BUFFER;
      }
      else
      {
        int BUFFERSIZE = m_bufferSize ? m_bufferSize : OPTIM_BUFFERSIZE;
        if(precision > (unsigned)BUFFERSIZE)
        {
          // Must use AT_EXEC SQLGetData Interface
          atexec = min((int)m_maxColumnLength,BUFFERSIZE);
          if(atexec == 0)
          {
            // Provide arbitrary border value
            atexec = BUFFERSIZE;
          }
          precision = atexec;
        }
        // Some ODBC drivers crash as a result of the fact
        // that CHAR types could be WCHAR types and they 
        // reserve the privilege to allocate double the memory
        // IF YOU DON'T DO THIS, YOU WILL CRASH!!
        precision *= 2;
      }
    }
    // Create new variant and reserve space for CHAR and BINARY types
    SQLVariant* var = new SQLVariant(type,(int)precision);
    var->SetColumnNumber(icol);
    var->SetSQLDataType(dataType);
    if(atexec)
    {
      m_hasLongColumns = true;
      var->SetAtExec(true);
      var->SetBinaryPieceSize(atexec);
    }

    // Record precision/scale for NUMERIC/DECIMAL types
    if(type == SQL_C_NUMERIC)
    {
      SQL_NUMERIC_STRUCT* numeric = var->GetAsNumeric();
      if (m_database)
      {
        m_database->GetSQLInfoDB()->GetRDBMSNumericPrecisionScale(precision,scale);
      }
      numeric->precision = (SQLCHAR)  precision;
      numeric->scale     = (SQLSCHAR) scale;
    }

    XString columnName(colName);
    columnName.MakeLower();
    m_numMap .insert(std::make_pair(icol,var));
    m_nameMap.insert(std::make_pair(columnName,var));
//     TRACE("COLUMN\n");
//     TRACE("- Number   : %d\n",icol);
//     TRACE("- Name     : %s\n",colName);
//     TRACE("- Datatype : %s\n",colName,var->FindDatatype(var->GetDataType()));
//     TRACE("- Precision: %d\n",precision);
//     TRACE("- ATEXEC   : %d\n",atexec);
  }

  // NOW WE HAVE ALL INFORMATION
  // TO BEGIN THE BINDING PROCES

  for(auto& column : m_numMap)
  {
    SQLVariant* var = column.second;
    if(var->GetAtExec() == false)
    {
      ushort bcol = (ushort) var->GetColumnNumber();
      short  type = (short)  var->GetDataType();

      // Rebind the column datatype
      type = RebindColumn(type);

      m_retCode = SqlBindCol(m_hstmt                    // statement handle
                            ,bcol                       // Column number
                            ,type                       // Data type
                            ,var->GetDataPointer()      // Data pointer
                            ,var->GetDataSize()         // Data length
                            ,var->GetIndicatorPointer() // Indicator address
                            );
      if(!SQL_SUCCEEDED(m_retCode))
      {
        GetLastError("Cannot bind to column. Error: ");
        m_lastError.AppendFormat(" Column number: %d",icol);
        throw StdException(m_lastError);
      }

      // Now do the SQL_NUMERIC precision/scale binding
      if(type == SQL_C_NUMERIC)
      {
        BindColumnNumeric((SQLSMALLINT)bcol,var,SQL_RESULT_COL);
      }
    }
  }
}

// Do the rebind replacement for a column
short
SQLQuery::RebindColumn(short p_datatype)
{
  if(m_rebindColumns)
  {
    RebindMap::iterator re = m_rebindColumns->find(p_datatype);
    if(re != m_rebindColumns->end())
    {
      return (short) re->second;
    }
  }
  return p_datatype;
}

// Specifying the precision/scale of a NUMERIC/DECIMAL number
// Must be set in the ARD/APD of the record descriptor to work
//
void
SQLQuery::BindColumnNumeric(SQLSMALLINT p_column,SQLVariant* p_var,int p_type)
{
  // Row descriptor for RESULT rows or PARAMeter rows
  SQLHDESC rowdesc = NULL;

  // Is it for a result set, or for a binded parameter?
  SQLINTEGER attribute = (p_type == SQL_RESULT_COL) ? SQL_ATTR_APP_ROW_DESC : SQL_ATTR_APP_PARAM_DESC;

  // Getting the ROW descriptor and set the precision/scale fields
  m_retCode = SqlGetStmtAttr(m_hstmt,attribute,&rowdesc,SQL_IS_POINTER,nullptr);
  if(SQL_SUCCEEDED(m_retCode))
  {
    int     precision = p_var->GetNumericPrecision();
    int     scale     = p_var->GetNumericScale();
    RETCODE retCode1  = SqlSetDescField(rowdesc,p_column,SQL_DESC_TYPE,     (SQLPOINTER)(DWORD_PTR)SQL_C_NUMERIC,SQL_IS_INTEGER);
    RETCODE retCode2  = SqlSetDescField(rowdesc,p_column,SQL_DESC_PRECISION,(SQLPOINTER)(DWORD_PTR)precision,    SQL_IS_INTEGER);
    RETCODE retCode3  = SqlSetDescField(rowdesc,p_column,SQL_DESC_SCALE,    (SQLPOINTER)(DWORD_PTR)scale,        SQL_IS_INTEGER);

    if(SQL_SUCCEEDED(retCode1) && SQL_SUCCEEDED(retCode2) && SQL_SUCCEEDED(retCode3))
    {
      // Now trigger the reset and check of the descriptor record, by re-supplying the data pointer again.
      // Very covertly described in the ODBC documentation. But if you do not do this one last step
      // results will be very different - and faulty - depending on your RDBMS
      SQLPOINTER pointer = p_var->GetDataPointer();
      m_retCode = SqlSetDescField(rowdesc,p_column,SQL_DESC_DATA_PTR,pointer,SQL_IS_POINTER);
      if(SQL_SUCCEEDED(m_retCode))
      {
        // All went well, we are done
        return;
      }
    }
  }
  XString error;
  error.Format("Cannot bind NUMERIC attributes PRECISION/SCALE for column: %d",p_column);
  throw StdException(error);
}

int 
SQLQuery::ProvideAtExecData()
{
  if(m_parameters.size() == 0)
  {
    return SQL_SUCCESS;
  }
  do 
  {
    // Find the parameter that needs the data
    SQLLEN parameter = NULL;
    m_retCode = SqlParamData(m_hstmt,(SQLPOINTER*)&parameter);

    if(m_retCode == SQL_NEED_DATA)
    {
      VarMap::iterator it = m_parameters.find((int)parameter);
      if(it != m_parameters.end())
      {
        SQLVariant* var  = it->second;
        SQLPOINTER data  = var->GetDataPointer();
        SQLINTEGER size  = var->GetDataSize();
        SQLINTEGER piece = var->GetBinaryPieceSize();
        SQLINTEGER total = 0;

        // Initially the piece can be bigger than the requested size
        if(piece > size)
        {
          piece = size;
        }

        // Put all the data pieces into place
        while(total < size)
        {
          m_retCode = SqlPutData(m_hstmt,data,piece);
          if(SQL_SUCCEEDED(m_retCode) == false && m_retCode != SQL_NEED_DATA)
          {
            // Next SQLParamData/SQLExecute cycle
            break;
          }
          data   = (SQLPOINTER)((DWORD_PTR)data + (DWORD_PTR)piece);
          total += piece;
          if(size - total < piece)
          {
            // Last piece can be smaller
            piece = (size - total);
          }
        }
      }
      // Force SQLParamData one more time
      // See if there are other parameters that are in need of data
      m_retCode = SQL_NEED_DATA;
    }
  } 
  while(m_retCode == SQL_NEED_DATA);

  // Last result from SQLParamData is result from SQLExecute
  return m_retCode;
}

// Fetch the resulting cursor name
void
SQLQuery::FetchCursorName()
{
  SQLSMALLINT length = 0;
  SQLCHAR cursorName[SQL_MAX_IDENTIFIER + 1];
  cursorName[0] = 0;

  // Not all RDBMS'es return cursor names for all types of queries
  // So this could go wrong, we ignore the errors!!
  m_retCode = SqlGetCursorName(m_hstmt,cursorName,SQL_MAX_IDENTIFIER,&length);
  if(SQL_SUCCEEDED(m_retCode))
  {
    m_cursorName = cursorName;
  }
}

// Try to get the row count for an INSERT/UPDATE/DELETE command
int
SQLQuery::GetNumberOfRows()
{
  m_retCode = SqlRowCount(m_hstmt,&m_rows);
  if(!SQL_SUCCEEDED(m_retCode))
  {
    return 0;
  }
  return m_rows < 0 ? 0 : (int)m_rows;
}

int 
SQLQuery::GetMaxColumnLength()
{
  m_retCode = SqlGetStmtAttr(m_hstmt,SQL_ATTR_MAX_LENGTH,&m_maxColumnLength,sizeof(int),NULL);
  if(!SQL_SUCCEEDED(m_retCode))
  {
    m_maxColumnLength = 0;
  }
  return m_maxColumnLength;
}

// POST PROCESSING of the query result

// Truncate the char fields in the gotten buffer
void
SQLQuery::TruncateCharFields()
{
  for(auto& column : m_numMap)
  {
    // Get variable
    SQLVariant* var = column.second;
    // We try to truncate the CHAR/VARCHAR fields
    if(var->GetDataType() == SQL_C_CHAR)
    {
      var->TruncateCharacter();
    }
  }
}

// Truncate the timestamps to a number of decimals (0 - 6)
void
SQLQuery::TruncateTimestamps(int p_decimals /*= 0*/)
{
  for(auto& column : m_numMap)
  {
    // Get variable
    SQLVariant* var = column.second;

    if(var->GetDataType() == SQL_C_TIMESTAMP || 
       var->GetDataType() == SQL_C_TYPE_TIMESTAMP)
    {
      // We truncate the TIMESTAMP fields
      var->TruncateTimestamp(p_decimals);
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
// Get the next record from the executed statement
// TRUE if succeeded to get one more record
// FALSE if no more data records found
// Exception on error
//
//////////////////////////////////////////////////////////////////////////

bool
SQLQuery::GetRecord()
{
  if(!m_hstmt)
  {
    m_retCode = SQL_ERROR;
    return false;
  }
  if(m_isSelectQuery == false)
  {
    return false;
  }
  // Enforce maximum number of rows?
  if(m_maxRows && (m_fetchIndex == m_maxRows))
  {
    return false;
  }
  // Set all columns to NULL
  ResetColumns();

  // Do the fetch
  m_retCode = SqlFetch(m_hstmt);
  if(SQL_SUCCEEDED(m_retCode))
  {
    // Gotten record
    ++m_fetchIndex;

    if(m_hasLongColumns)
    {
      if(SQL_SUCCEEDED(RetrieveAtExecData()))
      {
        return true;
      }
    }
    else
    {
      return true;
    }
  }
  else if(m_retCode == SQL_NO_DATA)
  {
    return false;
  }
  GetLastError("Error in fetch-next-record: ");
  throw StdException(m_lastError);
}

// Retrieve the piece-by-piece data at exec time of the SQLFetch
int
SQLQuery::RetrieveAtExecData()
{
  for(auto& column : m_numMap)
  { 
    SQLVariant* var = column.second;
    if(var->GetAtExec())
    {
      // Retrieve actual length of this instance of the column
      int col = column.first;
      SQLLEN actualLength = 0;
      m_retCode = SqlGetData(m_hstmt
                            ,(SQLUSMALLINT) col
                            ,(SQLSMALLINT)  var->GetDataType()
                            ,(SQLPOINTER)   var->GetDataPointer()
                            ,(SQLINTEGER)   0  // Request the actual length of this field
                            ,&actualLength);
      if(SQL_SUCCEEDED(m_retCode))
      {
        if(actualLength == SQL_NO_TOTAL)
        {
          // Cannot determine the data in this column
          return m_retCode;
        }
        else if(actualLength > 0)
        {
          // Retrieve the data piece-by-piece
          int size = var->GetBinaryPieceSize();
          int length = 0;
          int extra  = var->GetDataType() == SQL_C_CHAR ? 1 : 0;
          // Reserve space in the SQLVariant for this data
          var->ReserveSpace(var->GetDataType(),(int)actualLength);

          // Now go get it
          while(length < actualLength)
          {
            unsigned char* pointer = (unsigned char*)var->GetDataPointer();
            pointer += length;
            m_retCode = SqlGetData(m_hstmt
                                 ,(SQLUSMALLINT) col
                                 ,(SQLUSMALLINT) var->GetDataType()
                                 ,(SQLPOINTER)   pointer
                                 ,(SQLINTEGER)  ((size_t)size + extra)
                                 ,(SQLLEN*)      var->GetIndicatorPointer());
            if(SQL_SUCCEEDED(m_retCode))
            {
              SQLLEN* sizepointer = (SQLLEN*) var->GetIndicatorPointer();
              // sizepointer sometimes points to the actual size of this piece
              // or can point to the 'pending length' of the column to be retrieved
              // ODBC Standard does not give any documentation on this.
              length += size;
              if(m_retCode == SQL_SUCCESS)
              {
                // All pieces are retrieved.
                // Extra failsafe at inner loop, apart from while(length < actualLength)
                // Indicator is sometimes the size of the last block
                *sizepointer = SQL_LEN_DATA_AT_EXEC(actualLength);
                break;
              }
            }
            else
            {
              // SQL_ERROR / SQL_NO_DATA / SQL_STILL_EXECUTING / SQL_INVALID_HANDLE
              return m_retCode;
            }
          }
          // End of while-loop
        }
      }
      else
      {
        // SQL_ERROR / SQL_NO_DATA / SQL_STILL_EXECUTING / SQL_INVALID_HANDLE
        return m_retCode;
      }
    }
  }
  return SQL_SUCCESS;
}

void
SQLQuery::DoCancelQuery()
{
  if(m_hstmt)
  {
    m_retCode = SqlCancel(m_hstmt);
    if(SQL_SUCCEEDED(m_retCode))
    {
      GetLastError("Parallel cancel SQL statement: ");
      throw StdException(m_lastError);
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
// Various status getter members
//
//////////////////////////////////////////////////////////////////////////

SQLVariant* 
SQLQuery::GetColumn(int icol)
{
  ColNumMap::iterator it = m_numMap.find(icol);
  if(it != m_numMap.end())
  {
    return it->second;
  }
  m_retCode = SQL_ERROR;
  return NULL;
}

int
SQLQuery::GetColumnType(int icol)
{
  ColNumMap::iterator it = m_numMap.find(icol);
  if(it != m_numMap.end())
  {
    return it->second->GetDataType();
  }
  m_retCode = SQL_ERROR;
  return NULL;
}

// See if column is NULL value
bool    
SQLQuery::IsNull(int col)
{
  ColNumMap::iterator it = m_numMap.find(col);
  if(it != m_numMap.end())
  {
    return it->second->IsNULL();
  }
  // Not a column yields always a NULL result
  return true;
}

// See if column is EMPTY value
bool    
SQLQuery::IsEmpty(int col)
{
  ColNumMap::iterator it = m_numMap.find(col);
  if(it != m_numMap.end())
  {
    return it->second->IsEmpty();
  }
  // Not a column yields an empty result
  return true;
}

// Get an error string from the handle
void
SQLQuery::GetLastError(XString p_prefix /*=""*/)
{
  m_lastError.Empty();
  if (!m_hstmt)
  {
    m_lastError = "Statement handle is null";
    return;
  }
  if(SQL_SUCCEEDED(m_retCode))
  {
    return;
  }
  // Record prefix
  if(!m_lastError.IsEmpty())
  {
    m_lastError += "\n";
  }
  m_lastError += p_prefix;

  // Append last return value
  XString prefix;
  prefix.Format("ODBC-call returned [%d] : ",m_retCode);
  m_lastError += prefix;

  // Ask database class for error info
  if(m_database)
  {
    m_lastError += m_database->GetErrorString(m_hstmt);
    return;
  }
  // Fallback in case we don't have a database pointer
  while(1)
  {
    SQLINTEGER  nativeError = 0;
    SQLCHAR     SqlState[SQL_SQLSTATE_SIZE + 1];
    SQLCHAR     ErrorMsg[SQL_MAX_MESSAGE_LENGTH + 1];
    SQLSMALLINT messageLen = 0;

    m_retCode = SqlError(SQL_NULL_HENV
                        ,SQL_NULL_HDBC
                        ,m_hstmt
                        ,SqlState
                        ,&nativeError
                        ,ErrorMsg
                        ,SQL_MAX_MESSAGE_LENGTH
                        ,&messageLen);
    if(!SQL_SUCCEEDED(m_retCode) || m_retCode == SQL_NO_DATA)
    {
      break;
    }
    XString error;
    error.Format("[%d][%s] %s",nativeError,SqlState,ErrorMsg);
    if(!m_lastError.IsEmpty())
    {
      m_lastError += "\n";
    }
    m_lastError += error;
  }
}

// ColumnName -> column number
int  
SQLQuery::GetColumnNumber(const char* p_columnName) 
{
  XString colName(p_columnName);
  ColNameMap::iterator it = m_nameMap.find(colName);
  if(it != m_nameMap.end())
  {
    return it->second->GetColumnNumber();
  }
  return 0;
}

// ColumnNumber -> column name
bool 
SQLQuery::GetColumnName(int p_column,XString& p_name) 
{
  for(auto& column : m_nameMap)
  { 
    if(column.second->GetColumnNumber() == p_column)
    {
      p_name = column.first;
      return true;
    }
  }
  return false;
}

// Convert database dependent SQL_XXXX types to C-types SQL_C_XXXX
short
SQLQuery::SQLType2CType(short p_sqlType)
{
  short type = SQL_C_CHAR; // Default;
  switch(p_sqlType)
  {
    case SQL_CHAR:                      // Fall through
    case SQL_WCHAR:                     // Fall through
    case SQL_VARCHAR:                   // Fall through
    case SQL_WVARCHAR:                  // Fall through
    case SQL_WLONGVARCHAR:              // Fall through
    case SQL_LONGVARCHAR:               type = SQL_C_CHAR;            break;
    case SQL_BINARY:                    // Fall through
    case SQL_VARBINARY:                 // Fall through
    case SQL_LONGVARBINARY:             type = SQL_C_BINARY;          break;
    case SQL_DOUBLE:                    type = SQL_C_DOUBLE;          break;
    case SQL_REAL:                      // Fall through
    case SQL_FLOAT:                     type = SQL_C_FLOAT;           break;
    case SQL_INTEGER:                   type = SQL_C_LONG;            break;
    case SQL_TINYINT:                   type = SQL_C_TINYINT;         break;
    case SQL_BIT:                       type = SQL_C_BIT;             break;
    case SQL_SMALLINT:                  type = SQL_C_SHORT;           break;
    case SQL_BIGINT:                    type = SQL_C_SBIGINT;         break;
    case SQL_DECIMAL:                   // Fall through
    case SQL_NUMERIC:                   type = SQL_C_NUMERIC;         break;
    case SQL_DATE:                      type = SQL_C_DATE;            break;
    case SQL_TIMESTAMP:                 type = SQL_C_TIMESTAMP;       break;
    case SQL_TIME:                      type = SQL_C_TIME;            break;
    case SQL_TYPE_DATE:                 type = SQL_C_TYPE_DATE;       break;
    case SQL_TYPE_TIMESTAMP:            type = SQL_C_TYPE_TIMESTAMP;  break;
    case SQL_INTERVAL_YEAR:             type = SQL_C_INTERVAL_YEAR;   break;
    case SQL_INTERVAL_MONTH:            type = SQL_C_INTERVAL_MONTH;  break;
    case SQL_INTERVAL_DAY:              type = SQL_C_INTERVAL_DAY;    break;
    case SQL_INTERVAL_HOUR:             type = SQL_C_INTERVAL_HOUR;   break;
    case SQL_INTERVAL_MINUTE:           type = SQL_C_INTERVAL_MINUTE; break;
    case SQL_INTERVAL_SECOND:           type = SQL_C_INTERVAL_SECOND; break;
    case SQL_INTERVAL_YEAR_TO_MONTH:    type = SQL_C_INTERVAL_YEAR_TO_MONTH;    break;
    case SQL_INTERVAL_DAY_TO_HOUR:      type = SQL_C_INTERVAL_DAY_TO_HOUR;      break;
    case SQL_INTERVAL_DAY_TO_MINUTE:    type = SQL_C_INTERVAL_DAY_TO_MINUTE;    break;
    case SQL_INTERVAL_DAY_TO_SECOND:    type = SQL_C_INTERVAL_DAY_TO_SECOND;    break;
    case SQL_INTERVAL_HOUR_TO_MINUTE:   type = SQL_C_INTERVAL_HOUR_TO_MINUTE;   break;
    case SQL_INTERVAL_HOUR_TO_SECOND:   type = SQL_C_INTERVAL_HOUR_TO_SECOND;   break;
    case SQL_INTERVAL_MINUTE_TO_SECOND: type = SQL_C_INTERVAL_MINUTE_TO_SECOND; break;
  }
  if(m_rebindColumns)
  {
    // A rebind simply transposes a SQL_C_XXX type for another
    // So we can as to return say a SQL_C_DATE as an SQL_C_CHAR from the ODBC driver
    RebindMap::iterator it = m_rebindColumns->find(p_sqlType);
    if(it != m_rebindColumns->end())
    {
      // Return rebind type to this one
      return (short)(it->second);
    }
  }
  // return the SQL_C_XXXX type
  return type;
}

int
SQLQuery::GetColumnLength(int p_column)
{
  UCHAR	      characters[1] = "";
  SQLSMALLINT inputSize = 1;
  SQLSMALLINT outputSize;
  SQLLEN     integerValue = 0;

  if(p_column <= m_numColumns)
  {
    m_retCode = SqlColAttribute(m_hstmt
                               ,(SQLUSMALLINT) p_column
                               ,(SQLUSMALLINT) SQL_DESC_LENGTH
                               ,(SQLPOINTER)   characters
                               ,(SQLSMALLINT)  inputSize
                               ,(SQLSMALLINT*) &outputSize
                               ,(SQLLEN*)      &integerValue);
    if (!SQL_SUCCEEDED(m_retCode))
    {
      GetLastError();
      throw StdException(m_lastError);
    }
  }
  return (int)integerValue;
}

int
SQLQuery::GetColumnDisplaySize(int p_column)
{
  UCHAR	      characters[1] = "";
  SQLSMALLINT inputSize = 1;
  SQLSMALLINT outputSize;
  SQLINTEGER  integerValue = 0;

  if(p_column <= m_numColumns)
  {
    m_retCode = SqlColAttribute(m_hstmt
                               ,(SQLUSMALLINT) p_column
                               ,(SQLUSMALLINT) SQL_DESC_DISPLAY_SIZE
                               ,(SQLPOINTER)   characters
                               ,(SQLSMALLINT)  inputSize
                               ,(SQLSMALLINT*) &outputSize
                               ,(SQLLEN*)      &integerValue);
    if (!SQL_SUCCEEDED(m_retCode))
    {
      GetLastError();
      throw StdException(m_lastError);
    }
  }
  return (int)integerValue;
}

// LEGACY SUPPORT ODBC 1.x AND 2.x Style applications

void
SQLQuery::DescribeColumn(int           p_col
                        ,XString&      p_columnName
                        ,XString&      p_colLabel
                        ,SQLSMALLINT&  p_sqlType
                        ,SQLUINTEGER&  p_colSize
                        ,SQLSMALLINT&  p_colScale
                        ,SQLSMALLINT&  p_colNullable
                        ,SQLINTEGER&   p_colDispSize)
{
  SQLSMALLINT	cbDescMax     = SQL_MAX_IDENTIFIER;
  SQLSMALLINT cbDescResult  = 0;
  SQLSMALLINT	sqlType       = 0;
  SQLUINTEGER cbColDef      = 0;
  SQLSMALLINT ibScale       = 0;
  SQLSMALLINT fNullable     = 0;
  SQLLEN      fDesc         = 0;
  SQLCHAR     szColName[SQL_MAX_BUFFER] = "";
  SQLCHAR     rgbDesc  [SQL_MAX_BUFFER] = "";

  m_retCode = SqlDescribeCol(m_hstmt
                            ,(SQLUSMALLINT) p_col
                            ,(SQLCHAR*)     szColName
                            ,(SQLSMALLINT)  cbDescMax
                            ,(SQLSMALLINT*) &cbDescResult
                            ,(SQLSMALLINT*) &sqlType
                            ,(SQLULEN*)     &cbColDef
                            ,(SQLSMALLINT*) &ibScale
                            ,(SQLSMALLINT*) &fNullable);
  if(!SQL_SUCCEEDED(m_retCode))
  {
    GetLastError();
    throw StdException(m_lastError);
  }
  // Explicit 2.x call (SQLColAttributes instead of SQLColAttribute)
  m_retCode = SqlColAttributes(m_hstmt
                              ,(SQLUSMALLINT) p_col
                              ,(SQLUSMALLINT) SQL_COLUMN_LABEL
                              ,(SQLPOINTER)   rgbDesc
                              ,(SQLSMALLINT)  cbDescMax
                              ,(SQLSMALLINT*) &cbDescResult
                              ,(SQLLEN*)      &fDesc);
  if(!SQL_SUCCEEDED(m_retCode))
  {
    GetLastError();
    throw StdException(m_lastError);
  }
  // Explicit 2.x call (SQLColAttributes instead of SQLColAttribute)
  m_retCode = SqlColAttributes(m_hstmt
                              ,(SQLUSMALLINT) p_col
                              ,(SQLUSMALLINT) SQL_COLUMN_LENGTH
                              ,(SQLPOINTER)   rgbDesc
                              ,(SQLSMALLINT)  cbDescMax
                              ,(SQLSMALLINT*) &cbDescResult
                              ,(SQLLEN*)      &fDesc);
  if(!SQL_SUCCEEDED(m_retCode))
  {
    GetLastError();
    throw StdException(m_lastError);
  }
  // Rebinding
  sqlType = SQLType2CType(sqlType);

  // Results
  p_columnName  = szColName;
  p_sqlType     = sqlType;
  p_colSize     = cbColDef;
  p_colScale    = ibScale;
  p_colNullable = fNullable;
  p_colLabel    = rgbDesc;
  p_colDispSize = (SQLINTEGER)fDesc;
}

//////////////////////////////////////////////////////////////////////////
//
// FUNCTION / PROCEDURE calls
//
// Parameter 0  -> The return parameter (if present) type = PARAM_OUTPUT
// Parameter 1  -> First procedure parameter (input, output, inout)
// Parameter n  -> nth   procedure parameter (input, output, inout)
//
//////////////////////////////////////////////////////////////////////////

// Short forms for 1 (one) input parameter and 1 output parameter
SQLVariant*
SQLQuery::DoSQLCall(XString p_schema,XString p_procedure,const int p_param1)
{
  SQLVariant* var = new SQLVariant(p_param1);
  InternalSetParameter(1,var,P_SQL_PARAM_INPUT);
  return DoSQLCall(p_schema,p_procedure,true);
}

SQLVariant*
SQLQuery::DoSQLCall(XString p_schema,XString p_procedure,const char* p_param1)
{
  SQLVariant* var = new SQLVariant(p_param1);
  InternalSetParameter(1,var,P_SQL_PARAM_INPUT);
  return DoSQLCall(p_schema,p_procedure,true);
}

SQLVariant*
SQLQuery::DoSQLCall(XString p_schema,XString p_procedure,const bcd& p_param1)
{
  SQLVariant* var = new SQLVariant(&p_param1);
  InternalSetParameter(1,var,P_SQL_PARAM_INPUT);
  return DoSQLCall(p_schema,p_procedure,true);
}

// Call procedure, do your own parameter plumbing  
SQLVariant*
SQLQuery::DoSQLCall(XString p_schema,XString p_procedure,bool p_hasReturn /*=false*/)
{
  // Check we have a database object and not a isolated HDBC
  if(m_database == nullptr)
  {
    throw StdException("Cannot do a function/procedure call without a database object");
  }

  // Make sure we have a minimal output parameter (SQL_SLONG !!)
  // OTHERWISE, YOU HAVE TO PROVIDE IT YOURSELF!
  if(p_hasReturn && m_parameters.find(0) == m_parameters.end() && m_nameMap.empty())
  {
    SQLVariant* var = new SQLVariant((int)0);
    InternalSetParameter(0,var,P_SQL_PARAM_OUTPUT);
  }

  // See if we ask for a call with named parameters
  if(!m_nameMap.empty())
  {
    return m_database->GetSQLInfoDB()->DoSQLCallNamedParameters(this,p_schema,p_procedure);
  }

  // Is we support standard ODBC, do that call
  if(m_database->GetSQLInfoDB()->GetRDBMSSupportsODBCCallEscapes())
  {
    return DoSQLCallODBCEscape(p_schema,p_procedure,p_hasReturn);
  }

  // Let the database implementation take care of it
  return m_database->GetSQLInfoDB()->DoSQLCall(this,p_schema,p_procedure);
}

// Direct call through ODBC escape language
SQLVariant*
SQLQuery::DoSQLCallODBCEscape(XString& p_schema,XString& p_procedure,bool p_hasReturn)
{
  // Start with generating the SQL
  XString sql = ConstructSQLForCall(p_schema,p_procedure,p_hasReturn);

  // Do the call and fetch return values
  DoSQLStatement(sql);

  // Clear any result sets generated
  // On some RDBMS'es this fetches the OUTPUT parameters of the procedure/function
  do
  {
    m_retCode = SqlMoreResults(m_hstmt);
  }
  while(m_retCode != SQL_NO_DATA && m_retCode != SQL_ERROR);

  // Correct RDBMS that give too much spaces in strings
  LimitOutputParameters();

  // Return the return-parameter (if any)
  return GetParameter(0);
}

// Construct the SQL for a function/procedure call with binding markers
// form 1: Without parameters     { CALL function }
// form 2: Without parameters     { CALL function() }
// form 3: With input parameters  { CALL function(?,?) }
// form 4: With return parameter  { ? = CALL function(?,?) }
// form 5: Only return parameter  { ? = CALL function }
XString
SQLQuery::ConstructSQLForCall(XString& p_schema,XString& p_procedure,bool p_hasReturn)
{
  // Start with ODBC-escape character
  XString sql("{");

  // Add placeholder for return parameter
  if(p_hasReturn)
  {
    sql += "?=";
  }

  // Add function / procedure name
  sql += "CALL ";
  if(!p_schema.IsEmpty())
  {
    sql += p_schema;
    sql += ".";
  }
  sql += p_procedure;

  // Only add parenthesis if we need them
  if(!m_parameters.empty())
  {
    // Opening parenthesis
    sql += "(";

    // Construct parameter markers 
    for(auto& param : m_parameters)
    {
      if(param.first == 0) continue;
      if(param.first > 1) sql += ",";
      sql += "?";
    }
    // Closing parenthesis
    sql += ")";
  }
  // Add closing ODBC-escape character
  sql += "}";

  return sql;
}

// Character output parameters are sometimes not limited
void
SQLQuery::LimitOutputParameters()
{
  for(auto& param : m_parameters)
  {
    int type = param.second->GetParameterType();
    if(type == P_SQL_PARAM_OUTPUT || type == P_SQL_PARAM_INPUT_OUTPUT)
    {
      if(param.second->GetDataType() == SQL_C_CHAR)
      {
        param.second->ShrinkSpace();
      }
    }
  }
}

// Getting the result parameter value
SQLVariant* 
SQLQuery::GetParameter(int p_num)
{
  VarMap::iterator it = m_parameters.find(p_num);
  if(it != m_parameters.end())
  {
    return it->second;
  }
  return nullptr;
}

// Getting the database handle (if any)
HDBC
SQLQuery::GetDatabaseHandle()
{
  return m_connection;
}

// Getting the statement handle (if any)
HSTMT
SQLQuery::GetStatementHandle()
{
  return m_hstmt;
}

// End of namespace
}
