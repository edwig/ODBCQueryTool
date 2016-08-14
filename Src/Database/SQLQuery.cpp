////////////////////////////////////////////////////////////////////////
//
// File: SQLQuery.cpp
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
#include "stdafx.h"
#include <sqlext.h>
#include "SQLDatabase.h"
#include "SQLQuery.h"
#include "SQLWrappers.h"
#include "SQLDate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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

SQLQuery::SQLQuery(HDBC p_hdbc)
         :m_lock(NULL,INFINITE)
{
  Init(p_hdbc);
}

SQLQuery::~SQLQuery()
{
  Close();
  ResetParameters();
}

void 
SQLQuery::Init(SQLDatabase* p_database)
{
  m_database       = p_database;
  m_hstmt          = 0;
  m_retCode        = SQL_SUCCESS;
  m_numColumns     = -1;
  m_rows           = 0;
  m_fetchIndex     = 0;
  m_rebindParameters = NULL;
  m_rebindColumns    = NULL;
  m_hasLongColumns = false;
  m_bufferSize     = 0;
  m_prepareDone    = false;
  m_boundDone      = false;
  m_lastError      = "";
  m_maxRows        = 0;
  m_isSelectQuery  = false;
  m_speedThreshold = QUERY_TOO_LONG;
  m_connection       = NULL;
}

void
SQLQuery::Init(HDBC p_connection)
{
  Init((SQLDatabase*)NULL);
  m_connection = p_connection;
}

void
SQLQuery::Close()
{
  // Statement resetten
  if (m_hstmt)
  {
    // If a cursor was openend (binding columns present)
    // And not read until the end of the cursor stream
    // Try closing the cursor first
    if(m_numMap.size() && m_retCode != SQL_NO_DATA)
    {
      m_retCode = ::SQLCloseCursor(m_hstmt);
      if(!SQL_SUCCEEDED(m_retCode))
      {
        GetLastError("Closing the cursor: ");

        // Some databases give a SQLSTATE = 24000 if the cursor was at the end
        // This is documented behaviour of SQLCloseCursor
        if(m_database)
        {
        if(m_database->GetSQLState() != "24000")
        {
          m_database->LogPrint(0,m_lastError);
          m_database->LogPrint(0,"Trying to continue without closing the cursor!");
        }
      }
    }
    }
    // Free the statement and drop alle associated info
    // And all cursors on the database engine
    m_retCode = SQLDatabase::FreeSQLHandle(&m_hstmt,SQL_DROP);
    if(!SQL_SUCCEEDED(m_retCode))
    {
      GetLastError("Freeing the cursor: ");
      // throw m_lastError;
    }
  }
  // Clear nummer map
  ColNumMap::iterator it = m_numMap.begin();
  while(it != m_numMap.end())
  {
    delete it->second;
    it->second = NULL;
    ++it;
  }
  m_numMap.clear();
  m_nameMap.clear();

  // Reset other variables
  m_lastError.Empty();
  m_hasLongColumns  = false;
  m_maxColumnLength = 0;
  m_bufferSize      = 0;
  m_maxRows         = 0;
  m_numColumns      = 0;
  m_rows            = 0;
  m_fetchIndex      = 0;
  m_prepareDone     = false;
  m_boundDone       = false;
  // LEAVE ALONE THESE PARAMETERS FOR REUSE OF THE QUERY
  // m_database
  // m_connection
  // m_parameters
}

void
SQLQuery::ResetParameters()
{
  // Clear parameter map
  VarMap::iterator it3 = m_parameters.begin();
  while(it3 != m_parameters.end())
  {
    delete it3->second;
    it3->second = NULL;
    ++it3;
  }
  m_parameters.clear();
}

void
SQLQuery::Open()
{
  // get statement handle in m_hstmt:
  if(m_database)
  {
  RETCODE m_retCode = m_database->GetSQLHandle(&m_hstmt,false);
    if(!SQL_SUCCEEDED(m_retCode))
  {
    GetLastError("Getting statement handle: ");
    throw m_lastError;
  }
  }
  else
  {
    if(m_connection == SQL_NULL_HANDLE)
    {
      throw CString("No database handle. Are you logged in to a database?");
    }
    // Create the statement handle
    SQLRETURN res = odbc_std_app ? SqlAllocHandle(SQL_HANDLE_STMT,m_connection,&m_hstmt)
                                 : SqlAllocStmt(m_connection,&m_hstmt);
    if(!SQL_SUCCEEDED(res))
    {
      GetLastError("Error creating a statement handle: ");
      throw m_lastError;
    }
  }

  if(m_hstmt == NULL)
  {
    throw CString("No database connection at SQLQUery::Open function");
  }
  // DISABLED!!
  // Without escae scanning other things will go wrong.
  // For instance if a query ends on a "? " : So "<Placeholder><space>"

  // SPEED: Do not look for ODBC escapes sequences
  //   m_retCode = SQLSetStmtAttr(m_hstmt,SQL_ATTR_NOSCAN,(SQLPOINTER)SQL_NOSCAN_ON,SQL_IS_UINTEGER);
  //   if (!SQL_SUCCEEDED(m_retCode))
  //   {
  //     GetLastError("Cannot set NOSCAN attribute: ");
  //     throw m_lastError;
  //   }

  // Change cursor type to the cheapest qua performance
  m_retCode = SQLSetStmtAttr(m_hstmt,SQL_CURSOR_TYPE,(SQLPOINTER)SQL_CURSOR_FORWARD_ONLY,SQL_IS_UINTEGER);
  if(!SQL_SUCCEEDED(m_retCode))
  {
    GetLastError("Cannot set CURSOR-FORWARD-ONLY attribute: ");
    throw m_lastError;
  }

  // Set max-rows if our database DOES support it (Oracle!)
  if(m_maxRows)
  {
    m_retCode = SQLSetStmtAttr(m_hstmt,SQL_MAX_ROWS,(SQLPOINTER)(ULONG_PTR)m_maxRows,SQL_IS_UINTEGER);
    if(!SQL_SUCCEEDED(m_retCode))
    {
      GetLastError("Cannot set SQL_MAX_ROWS attribute: ");
      throw m_lastError;
    }
  }
  // Solving formatting for various databases (Oracle / MS-Access)
  // Numeric and Decimal formats can be mangled by disbehaving ODCBC drivers
  // So they must be set or gotten in a predefined format (e.g. Oracle needs DOUBLE for a NUMERIC column)
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
  ColNumMap::iterator it = m_numMap.begin();
  while(it != m_numMap.end())
  {
    it->second->SetNULL();
    ++it;
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
  seconds = (long) secondsDBL; // afronden

  CString message;
  if(seconds > m_speedThreshold)
  {
    message.Format("[999] Query too long: %.6f seconds\n",secondsDBL);
  }
  else
  {
    message.Format("Query time: %.6f seconds\n",secondsDBL);
  }
  m_database->LogPrint(LOGLEVEL_MAX,message);
}

//////////////////////////////////////////////////////////////////////////
//
// Set parameters for statement
//
//////////////////////////////////////////////////////////////////////////

// Private and hidden setting of the parameter
// The p_param is **NOT** copied, but stored and owned by the SQLQuery
void
SQLQuery::InternalSetParameter(int p_num,SQLVariant* p_param,int p_type /*=SQL_PARAM_INPUT*/)
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

// Setting a parameter. Copies the SQLVariant!!
void
SQLQuery::SetParameter(int p_num,SQLVariant* p_param,int p_type /*=SQL_PARAM_INPUT*/)
{
  SQLVariant* var = new SQLVariant(p_param);
  InternalSetParameter(p_num,var,p_type);
}

void 
SQLQuery::SetParameter(int p_num,long p_param,int p_type /*=SQL_PARAM_INPUT*/)
{
  SQLVariant* var = new SQLVariant(p_param);
  InternalSetParameter(p_num,var,p_type);
}

void 
SQLQuery::SetParameterUL(int p_num,unsigned long p_param,int p_type /*=SQL_PARAM_INPUT*/)
{
  SQLVariant *var = new SQLVariant(p_param);
  InternalSetParameter(p_num,var,p_type);
}

void 
SQLQuery::SetParameter(int p_num,const char* p_param,int p_type /*=SQL_PARAM_INPUT*/)
{
  SQLVariant* var = new SQLVariant(p_param);
  InternalSetParameter(p_num,var,p_type);
}

void 
SQLQuery::SetParameter(int p_num,CString& p_param,int p_type /*=SQL_PARAM_INPUT*/)
{
  SQLVariant* var = new SQLVariant(p_param);
  InternalSetParameter(p_num,var,p_type);
}

void 
SQLQuery::SetParameter(int p_num,SQLDate& p_param,int p_type /*=SQL_PARAM_INPUT*/)
{
  SQLVariant* var = new SQLVariant(&p_param);
  InternalSetParameter(p_num,var,p_type);
}

void 
SQLQuery::SetParameter (int p_num,SQLTime& p_param,int p_type /*=SQL_PARAM_INPUT*/)
{
  SQLVariant* var = new SQLVariant(&p_param);
  InternalSetParameter(p_num,var,p_type);
}

void 
SQLQuery::SetParameter(int p_num,SQLTimestamp& p_param,int p_type /*=SQL_PARAM_INPUT*/)
{
  SQLVariant* var = new SQLVariant(&p_param);
  InternalSetParameter(p_num,var,p_type);
}

//////////////////////////////////////////////////////////////////////////
//
// STATEMENTS
//
//////////////////////////////////////////////////////////////////////////

void 
SQLQuery::DoSQLStatement(const CString& p_statement,int p_param1)
{
  SetParameter(1,p_param1);
  DoSQLStatement(p_statement);
}

void 
SQLQuery::DoSQLStatement(const CString& p_statement,const char* p_param1)
{
  SetParameter(1,p_param1);
  DoSQLStatement(p_statement);
}

void
SQLQuery::DoSQLStatement(const CString& p_statement)
{
  // Check for filled statement
  if(p_statement.IsEmpty())
  {
    m_lastError = "Error in SQL statement: Empty statement.";
    throw m_lastError;
  }
  // Begin of query clock
  LARGE_INTEGER start;
  QueryPerformanceCounter(&start);

  // Close last cursor/statement and open a new one
  Close();
  Open();

  m_isSelectQuery = false;
  if(p_statement.Left(6).CompareNoCase("select") == 0)
  {
    m_isSelectQuery = true;
  }

  if(m_database && m_database->LogLevel() >= LOGLEVEL_MAX)
  {
    m_database->LogPrint(LOGLEVEL_MAX,"[Datbase query]\n");
    m_database->LogPrint(LOGLEVEL_MAX,p_statement.GetString());
    m_database->LogPrint(LOGLEVEL_MAX,"\n");
  }
  // Bind parameters
  BindParameters();

  // In special cases queries can go wrong through ORACLE ODBC if they contain newlines
  // Hence all newlines are replaces by spaces, if the query does NOT contain any comments
  CString statement(p_statement);
  if(statement.Find("--") < 0)
  {
    statement.Replace("\n"," ");
    statement.Replace("\r"," ");
  }
  // Optimization: remove trailing spaces
  statement.Trim();

  // The Oracle 10.2.0.3.0 ODBC Driver - and later versions - contain a bug
  // in the processing of the query-strings which crashes it in CharNexW
  // by a missing NUL-Terminator. By changing the length of the statement
  // _including_ the terminating NUL, it won't crash at all
  SQLINTEGER lengthStatement = statement.GetLength() + 1;

  // GO DO IT RIGHT AWAY
  m_retCode = SqlExecDirect(m_hstmt,(SQLCHAR*)(LPCSTR)p_statement,lengthStatement);

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
      CString fout;
      fout.Format("Error [%d] in getting the number of columns from a query: ",m_retCode);
      GetLastError(fout);
      throw m_lastError;
    }
  }
  else if(m_retCode < 0)
  {
    // rcExec == SQL_ERROR
    // rcExec == SQL_INVALID_HANDLE
    GetLastError("Error in SQL statement: ");
    throw m_lastError;
  }
  else
  {
    // rcExec == SQL_NO_DATA
    // rcExec == SQL_STILL_EXECUTING
    // rcExec == SQL_NEED_DATA
  }
  if(m_database && m_database->LogLevel() >= LOGLEVEL_MAX)
  {
    ReportQuerySpeed(start);
  }
}

// Variant with a catch to it
void 
SQLQuery::TryDoSQLStatement(const CString& p_statement)
{
  try
  {
    DoSQLStatement(p_statement);
  }
  catch(...)
  {
    // Do Nothing, ignoring the error
  }
}

SQLVariant* 
SQLQuery::DoSQLStatementScalar(const CString& p_statement)
{
  DoSQLStatement(p_statement);
  if(GetRecord())
  {
    return GetColumn(1);
  }
  return NULL;
}

SQLVariant* 
SQLQuery::DoSQLStatementScalar(const CString& p_statement,const int p_param1)
{
  SetParameter(1,p_param1);
  return DoSQLStatementScalar(p_statement);
}

SQLVariant* 
SQLQuery::DoSQLStatementScalar(const CString& p_statement,const char* p_param1)
{
  SetParameter(1,p_param1);
  return DoSQLStatementScalar(p_statement);
}

int
SQLQuery::DoSQLStatementNonQuery(const CString& p_statement,const int p_param1)
{
  SetParameter(1,p_param1);
  return DoSQLStatementNonQuery(p_statement);
}

int
SQLQuery::DoSQLStatementNonQuery(const CString& p_statement,const char* p_param1)
{
  SetParameter(1,p_param1);
  return DoSQLStatementNonQuery(p_statement);
}

int
SQLQuery::DoSQLStatementNonQuery(const CString& p_statement)
{
  if(p_statement.Left(6).CompareNoCase("select") == 0)
  {
    throw CString("SQL Non-query cannot contain a 'SELECT' statement");
  }
  DoSQLStatement(p_statement);
  return GetNumberOfRows();
}

void 
SQLQuery::DoSQLPrepare(const CString& p_statement)
{
  if(p_statement.IsEmpty())
  {
    m_lastError = "Error in SQL statement: Empty statement.";
    throw m_lastError;
  }
  // close last m_hstmt if still open
  Close();
  Open();

  // In special cases queries can go wrong through and ORACLE ODBC if they contain newlines
  // Hence all newlines are replaces by spaces, if the query does NOT contain any comments
  CString statement(p_statement);
  if(statement.Find("--") < 0)
  {
    statement.Replace("\n"," ");
    statement.Replace("\r"," ");
  }
  // Optimization: remove trailing spaces
  statement.Trim();

  // The Oracle 10.2.0.3.0 ODBC Driver - and later versions - contain a bug
  // in the processing of the query-strings which crashes it in CharNexW
  // by a missing NUL-Terminator. By changing the length of the statement
  // _including_ the terminating NUL, it won't crash at all
  SQLINTEGER lengthStatement = statement.GetLength() + 1;

  // GO DO THE PREPARE
  m_retCode = SqlPrepare(m_hstmt,(SQLCHAR*)(LPCSTR)p_statement,lengthStatement);
  if(SQL_SUCCEEDED(m_retCode))
  {
    m_prepareDone = true;
  }
  if(m_retCode < 0)
  {
    // rcExec == SQL_ERROR
    // rcExec == SQL_INVALID_HANDLE
    GetLastError("Error in SQL statement: ");
    throw m_lastError;
  }
}

void
SQLQuery::DoSQLExecute()
{
  m_retCode = SQL_ERROR;

  if(!m_prepareDone)
  {
    m_lastError = "Internal error: SQLExecute without SQLPrepare.";
    throw m_lastError;
  }
  
  if(!m_boundDone)
  {
    BindParameters();
  }

  m_retCode = SqlExecute(m_hstmt);
  //m_prepareDone = false;
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
      CString fout;
      fout.Format("Error [%d] in determining the number of columns in the query: ",m_retCode);
      GetLastError(fout);
      throw m_lastError;
    }
  }
  if(m_retCode < 0)
  {
    // rcExec == SQL_ERROR
    // rcExec == SQL_INVALID_HANDLE
    GetLastError("Error in SQL statement: ");
    throw m_lastError;
  }
  // Do bindings only once in a prepare -> multiple-execute cycle
  m_boundDone = true;
}

// Bind application parameters
// Override for other methods as SQLVariant
void
SQLQuery::BindParameters()
{
  if(m_parameters.size() == 0)
  {
    return;
  }
  VarMap::iterator it = m_parameters.begin();
  while(it != m_parameters.end())
  {
    int icol = it->first;
    SQLVariant* var = it->second;
    if(var->GetDataType() == 0) 
    {
      continue;
    }
    // Special case for SCALE
    int scale = 0;
    if(var->GetDataType() == SQL_C_NUMERIC)
    {
      scale = var->GetAsNumeric()->scale;
    }
    SQLPOINTER  dataPointer = var->GetDataPointer();
    SQLSMALLINT sqlDatatype = (SQLSMALLINT)var->GetSQLDataType();
    SQLUINTEGER columnSize  = var->GetDataSize();
    SQLINTEGER  bufferSize  = var->GetDataSize();

    if(var->GetAtExec())
    {
      // AT EXEC data piece by piece
      dataPointer = (SQLPOINTER)(ULONG_PTR) icol;
      // Some database types need to know the length beforehand (Oracle!)
      // If no database type known, set to true, just to be sure!
      var->SetSizeIndicator(m_database ? m_database->GetNeedLongDataLen() : true);
      bufferSize = 0;
    }
    // Check rebinds to do for scripting 
    if(m_rebindParameters)
    {
      RebindMap::iterator rt = m_rebindParameters->find(sqlDatatype);
      if(rt != m_rebindParameters->end())
      {
        sqlDatatype = (SQLSMALLINT)rt->second;
      }
    }
    // Check minimaal input type
    if(var->GetParameterType() == SQL_PARAM_TYPE_UNKNOWN)
    {
      var->SetParameterType(SQL_PARAM_INPUT);
    }
    if(m_database && m_database->LogLevel() >= LOGLEVEL_MAX)
    {
      if(icol == 1)
      {
        m_database->LogPrint(LOGLEVEL_MAX,"Parameters as passed on to the database:\n");
      }
      CString text;
      CString value;
      var->GetAsString(value);
      text.Format("Parameter %d: %s\n",icol,value);
      m_database->LogPrint(LOGLEVEL_MAX,text);
    }

    // Do the bindings
    m_retCode = SqlBindParameter(m_hstmt
                               ,(SQLUSMALLINT)icol                        // Number of parameter
                               ,(SQLSMALLINT) var->GetParameterType()     // SQL_PARAM_INPUT etc
                               ,(SQLSMALLINT) var->GetDataType()          // SQL_C_XXX Types
                               ,(SQLSMALLINT) sqlDatatype                 // SQL_XXX Type
                               ,(SQLUINTEGER) columnSize                  // Column size
                               ,(SQLSMALLINT) scale                       // Numeric scale
                               ,(SQLPOINTER)  dataPointer                 // Buffer pointer
                               ,(SQLINTEGER)  bufferSize                  // Buffer size (truncate on output)
                               ,(SQLLEN*)     var->GetIndicatorPointer());// NULL indicator
    if(!SQL_SUCCEEDED(m_retCode))
    {
      CString prefix;
      prefix.Format("Cannot bind parameter %d. Error: ",icol);
      GetLastError(prefix);
      throw m_lastError;
    }
    // Next column
    ++it;
  }
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
  for(icol = 1; icol <= m_numColumns; ++icol)
  {
    atexec = 0;
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
      CString fout;
      fout.Format("Error [%d] at the determining of the attributes of column [%d] : ",m_retCode,icol);
      GetLastError(fout);
      throw m_lastError;
    }

    int type = SQLType2CType(dataType);
    CString name(colName);
    if(type == SQL_C_CHAR || type == SQL_C_BINARY)
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
    SQLVariant* var = new SQLVariant(type,(int)precision);
    var->SetColumnNumber(icol);
    if(atexec)
    {
      m_hasLongColumns = true;
      var->SetAtExec(true);
      var->SetBinaryPieceSize(atexec);
    }
    CString columnName(colName);
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
  ColNumMap::iterator it = m_numMap.begin();
  while(it != m_numMap.end())
  {
    SQLVariant* var = it->second;
    if(var->GetAtExec() == false)
    {
      int icol = var->GetColumnNumber();
      int type = var->GetDataType();
      if(m_rebindColumns)
      {
        RebindMap::iterator re = m_rebindColumns->find(type);
        if(re != m_rebindColumns->end())
        {
          type = re->second;
        }
      }
      m_retCode = SqlBindCol(m_hstmt                                   // statement handle
                            ,(SQLUSMALLINT) icol                       // Column number
                            ,(SQLSMALLINT)  type                       // Data type
                            ,(SQLPOINTER)   var->GetDataPointer()      // Data pointer
                            ,(SQLINTEGER)   var->GetDataSize()         // Data length
                            ,(SQLLEN*)      var->GetIndicatorPointer() // Indicator address
                            );
      if(!SQL_SUCCEEDED(m_retCode))
      {
        CString prefix;
        prefix.Format("Cannot bind to column %d: ",icol);
        GetLastError(prefix);
        throw m_lastError;
      }
    }
    ++it;
  }
}

int 
SQLQuery::ProvideAtExecData()
{
  if(m_parameters.size() == 0)
  {
    return SQL_SUCCESS;
  }
  int value;
  do 
  {
    m_retCode = ::SQLParamData(m_hstmt,(SQLPOINTER*)&value);
    if(m_retCode == SQL_NEED_DATA)
    {
      VarMap::iterator it = m_parameters.find(value);
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

        // Put all the datapieces into place
        while(total < size)
        {
          m_retCode = SqlPutData(m_hstmt,data,piece);
          if(SQL_SUCCEEDED(m_retCode) == false && m_retCode != SQL_NEED_DATA)
          {
            // Next SQLParamData/SQLExecute cycle
            break;
          }
          data   = (SQLPOINTER)((ULONG_PTR)data + (ULONG_PTR)piece);
          total += piece;
          if(size - total < piece)
          {
            // Last piece can be smaller
            piece = (size - total);
          }
        }
      }
      // Force SQLParamData one more time
      m_retCode = SQL_NEED_DATA;
    }
  } 
  while(m_retCode == SQL_NEED_DATA);

  // Last result from SQLParamData is result from SQLExecute
  return m_retCode;
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
  throw m_lastError;
}

// Retrieve the piece-by-piece data at exec time of the SQLFetch
int
SQLQuery::RetrieveAtExecData()
{
  ColNumMap::iterator it = m_numMap.begin();
  while(it != m_numMap.end())
  {
    SQLVariant* var = it->second;
    if(var->GetAtExec())
    {
      // Retrieve actual length of this instance of the column
      int col = it->first;
      SQLLEN actualLength = 0;
      m_retCode = SqlGetData(m_hstmt
                            ,(SQLUSMALLINT) col
                            ,(SQLSMALLINT)  var->GetSQLDataType()
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
                                 ,(SQLUSMALLINT) var->GetSQLDataType()
                                 ,(SQLPOINTER)   pointer
                                 ,(SQLINTEGER)   size + extra
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
    // Next column
    ++it;
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
      throw m_lastError;
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
SQLQuery::GetLastError(CString p_prefix /*=""*/)
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
  CString prefix;
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
    CString error;
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
  CString colName(p_columnName);
  ColNameMap::iterator it = m_nameMap.find(colName);
  if(it != m_nameMap.end())
  {
    return it->second->GetColumnNumber();
  }
  return 0;
}

// ColumnNumber -> column name
bool 
SQLQuery::GetColumnName(int p_column,CString& p_name) 
{
  ColNameMap::iterator it = m_nameMap.begin();
  while(it != m_nameMap.end())
  {
    if(it->second->GetColumnNumber() == p_column)
    {
      p_name = it->first;
      return true;
    }
    ++it;
  }
  return false;
}

// Convert database dependent SQL_XXXX types to C-types SQL_C_XXXX
short
SQLQuery::SQLType2CType(short nSQLType)
{
  short type = SQL_C_CHAR; // Default;
  switch(nSQLType)
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
    RebindMap::iterator it = m_rebindColumns->find(nSQLType);
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
  SQLINTEGER  integerValue = 0;

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
      throw m_lastError;
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
      throw m_lastError;
    }
  }
  return (int)integerValue;
}

// LEGACY SUPPORT ODBC 1.x AND 2.x

void
SQLQuery::DescribeColumn(int           p_col
                        ,CString&      p_columnName
                        ,CString&      p_colLabel
                        ,SQLSMALLINT&  p_sqlType
                        ,SQLUINTEGER&  p_colSize
                        ,SQLSMALLINT&  p_colScale
                        ,SQLSMALLINT&  p_colNullable
                        ,SQLINTEGER&   p_colDispSize)
{
  SQLSMALLINT	cbDescMax     = MAX_FNAME_LEN;
  SQLSMALLINT cbDescResult  = 0;
  SQLSMALLINT	sqlType       = 0;
  SQLUINTEGER cbColDef      = 0;
  SQLSMALLINT ibScale       = 0;
  SQLSMALLINT fNullable     = 0;
  SQLLEN      fDesc         = 0;
  SQLCHAR     szColName[MAX_FNAME_LEN + 1] = "";
  SQLCHAR     rgbDesc  [MAX_FNAME_LEN + 1] = "";

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
    throw m_lastError;
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
    throw m_lastError;
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
    throw m_lastError;
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
