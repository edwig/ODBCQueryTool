////////////////////////////////////////////////////////////////////////
//
// File: SQLDatabase.cpp
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
#include <sqlucode.h>
#include "SQLComponents.h"
#include "SQLDatabase.h"
#include "SQLQuery.h"
#include "SQLWrappers.h"
#include "SQLInfoDB.h"
#include "SQLInfoMySQL.h"
#include "SQLInfoAccess.h"
#include "SQLInfoOracle.h"
#include "SQLInfoMariaDB.h"
#include "SQLInfoFirebird.h"
#include "SQLInfoInformix.h"
#include "SQLInfoSQLServer.h"
#include "SQLInfoPostgreSQL.h"
#include "SQLInfoGenericODBC.h"
#include "SQLTimestamp.h"
#include "sqlncli.h"
#include <time.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{

SQLDatabase::SQLDatabase()
            :m_info(NULL)
{
  // Initialize locking
  InitializeCriticalSection(&m_databaseLock);
}

SQLDatabase::SQLDatabase(HDBC p_hdbc)
            :m_hdbc(p_hdbc)
{
  // Initialize locking
  InitializeCriticalSection(&m_databaseLock);

  // Discover our database
  if(p_hdbc)
  {
    CollectInfo();
    SetAttributesAfterConnect(false);
  }
}

SQLDatabase::~SQLDatabase()
{
  // Only close the database if we did open it
  if(m_henv)
  {
    Close();
  }
  DeleteCriticalSection(&m_databaseLock);
}

void
SQLDatabase::Close()
{
  // Set lock on the stack
  Locker<SQLDatabase> lock(this,INFINITE);
  
  try
  {
    // Close database handle
    if(m_hdbc != SQL_NULL_HANDLE)
    {
      // See if there are pending transactions,
      CloseAllTransactions();

      // Try to disconnect 
      // (time consuming for some database engines)
      FreeDbcHandle();
    }
    // Close environment handle
    if(m_henv != SQL_NULL_HANDLE)
    {
      // Disconnect environment
      FreeEnvHandle();
    }
  }
  catch(StdException& ex)
  {
    // Can go wrong in many places in the ODBC stack or the RDMBS drivers stack
    LogPrint(ex.GetErrorMessage());
  }
  catch(...)
  {
    // Can go wrong in many places in the ODBC stack or the RDMBS drivers stack
    LogPrint(_T("Closing the database\n"));
  }
  // Empty parameter and column rebinding
  m_rebindParameters.clear();
  m_rebindColumns.clear();

  // Forget the SQLInfo + subclasses
  if(m_info)
  {
    delete m_info;
    m_info = NULL;
  }
  
  // Reset RDBMS related members
  // In fact the current connect info

  m_datasource.Empty();
  m_username.Empty();
  m_password.Empty();
  m_DBName.Empty();
  m_DBVersion.Empty();
  m_DriverName.Empty();
  m_DriverVersion.Empty();
  m_databaseName.Empty();
  m_originalConnect.Empty();
  m_completeConnect.Empty();
  m_odbcVersionComplete.Empty();

  m_rdbmsType         = RDBMS_UNKNOWN;
  m_async_possible    = 0;
  m_canDoTransactions = 0;
  m_odbcVersion       = 0;
  m_driverMainVersion = 0;
  m_needLongDataLen   = false;
  m_schemaAction      = SCHEMA_NO_ACTION;

  m_dbIdent.Empty();
  m_dataIdent.Empty();
  
  // DO NOT reset the logging pointers
  // This should be done externally by the connected application
  // But in most cases we just forget about it
  // m_logLevel   = NULL;
  // m_logPrinter = NULL;
  // m_logContext = NULL;
}

void 
SQLDatabase::Reset()
{
  CollectInfo();
}

// Can be set once (1 time) by the DatabasePool
void  
SQLDatabase::SetDatasource(XString p_dsn)
{
  if(m_datasource.IsEmpty())
  {
    m_datasource = p_dsn;
  }
}

// Can be set once (1 time) by the DatabasePool
void
SQLDatabase::SetConnectionName(XString p_connectionName)
{
  if(m_connectionName.IsEmpty())
  {
    m_connectionName = p_connectionName;
  }
}

// Can be set once (1 time) by the DatabasePool
void
SQLDatabase::SetUserName(XString p_user)
{
  if(m_username.IsEmpty())
  {
    m_username = p_user;
  }
}

void
SQLDatabase::SetPoolIdleMinutes(int p_minutes)
{
  if(IDLE_MINUTES_MIN <= p_minutes && p_minutes <= IDLE_MINUTES_MAX)
  {
    m_dbpoolIdleMinutes = p_minutes;
  }
}

// Last time the database was used by the database pool
void
SQLDatabase::SetLastActionTime()
{
  m_lastAction = GetTickCount64();
}

// Number of minutes not-in-action
// Needed for the DatabasePool system
bool
SQLDatabase::PastWaitingTime()
{
  return (GetTickCount64() - m_lastAction) > (m_dbpoolIdleMinutes * 60 * CLOCKS_PER_SEC);
}

// Add a general ODBC option for use in the connection string
void
SQLDatabase::AddConnectOption(XString p_keyword,XString p_value)
{
  // Connection strings uses all uppercase
  p_keyword.MakeUpper();
  
  // Find option and add or replace
  ODBCOptions::iterator it = m_options.find(p_keyword);
  if(it == m_options.end())
  {
    m_options[p_keyword] = p_value;
  }
  else
  {
    it->second = p_value;
  }
}

// Open the database (separate parameters)
bool 
SQLDatabase::Open(XString const& p_datasource
                 ,XString const& p_username
                 ,XString const& p_password
                 ,XString        p_options  /* = ""    */
                 ,bool           p_readOnly /* = false */)
{
  // get the connect string
  XString connect;
  connect.Format(_T("DSN=%s;UID=%s;PWD=%s;"), p_datasource.GetString(), p_username.GetString(), p_password.GetString());
  if(!p_options.IsEmpty())
  {
    connect += ";" + p_options;
  }
  // Add any options passed to 'AddConnectOption'  
  ODBCOptions::iterator it;
  for(it = m_options.begin();it != m_options.end();++it)
  {
    XString text;
    text.Format(_T("%s=%s;"),it->first.GetString(),it->second.GetString());
    connect += text;
  }

  // Open the database
  Open(connect,p_readOnly);

  // Remember datasource and user
  m_datasource = p_datasource;
  m_username   = p_username;

  return true;
}

// Open the database (connect string)
bool 
SQLDatabase::Open(XString const& p_connectString,bool p_readOnly)
{
  // Set lock on the stack
  Locker<SQLDatabase> lock(this,INFINITE);

  // Test if we are properly initialized
  SQLComponentsInitialized();

  // Close the open connection
  Close();

  // Make a new environment handle
  MakeEnvHandle();

  // Make a new connection handle
  MakeDbcHandle();

  // Time to set our attributes
  SetAttributesBeforeConnect();

  // The Connect function wants a non-const ptr
  SQLTCHAR* pszConnect = reinterpret_cast<SQLTCHAR*>(const_cast<TCHAR*>(p_connectString.GetString()));
  SQLTCHAR  szConnectOut[CONNECTSTRING_MAXLEN + 1];
  SQLSMALLINT total = 0;

  // MAKE THE CONNECTION
  SQLRETURN res = SqlDriverConnect(m_hdbc,              // Database handle
                                   0,                   // Window handle
                                   pszConnect,          // Connect string
                                   SQL_NTS,             // Connect string is null-terminated string
                                   szConnectOut,        // Connect result string
                                   CONNECTSTRING_MAXLEN,// Max length of the result string
                                   &total,              // Resulting length of szConnectOut
                                   SQL_DRIVER_NOPROMPT  // Option flags: do not show prompt window
                                   );
  // If connection fails
  if(!Check(res))
  {
    XString error(GetErrorString());
    Close();
    throw StdException(_T("Error at opening database: ") + error);
  }
  // Remember the returned completed connect string of the database
  // This contains all the database option settings from the ODBC Driver
  // Must be done before the Collect info for determining the database's name
  m_originalConnect = p_connectString;
  m_completeConnect  = _T("ODBC;") + XString(szConnectOut);

  // Get all info options
  CollectInfo();

  // Now that we now all options, we can perform special actions
  // that can only be performed once-in-a connection, at the beginning
  SetAttributesAfterConnect(p_readOnly);

  SetConnectionInitialisations();

  // Success
  return true;
}

// Setting connection attributes BEFORE connect
void    
SQLDatabase::SetAttributesBeforeConnect()
{
  SetConnectAttr(SQL_ATTR_ASYNC_ENABLE,SQL_ASYNC_ENABLE_OFF,0);

  // No dialog boxes at the connect moment please
  SetConnectAttr(SQL_ATTR_QUIET_MODE,0,0);

  // Maximum timeout for the connect to fail
  if(m_loginTimeout > 0)
  {
    SetConnectAttr(SQL_LOGIN_TIMEOUT,m_loginTimeout,0);
  }

  // In case of a SQL-Server client, enable the MARS option
  // MARS = Multiple Active Result Sets
  // Other drivers will ignore this option.
  // MUST be set before the connect is done!
  if(m_mars)
  {
    SetConnectAttr(SQL_COPT_SS_MARS_ENABLED,SQL_MARS_ENABLED_YES,SQL_IS_UINTEGER);
  }
}

// Setting connection attributes AFTER connect
void
SQLDatabase::SetAttributesAfterConnect(bool p_readOnly)
{
  // Connection is readOnly or ReadWrite
  if((m_readOnly = p_readOnly) == true)
  {
    SetReadOnly(m_readOnly);
  }

  if(m_async_possible == SQL_AM_CONNECTION ||
     m_async_possible == SQL_AM_STATEMENT  )
  {
    SetConnectAttr(SQL_ATTR_ASYNC_ENABLE,SQL_ASYNC_ENABLE_OFF,SQL_IS_UINTEGER);
  }
  if(m_canDoTransactions != SQL_TC_NONE)
  {
    SetConnectAttr(SQL_ATTR_TXN_ISOLATION,SQL_TXN_READ_COMMITTED,SQL_IS_UINTEGER);
    SetConnectAttr(SQL_ATTR_TXN_ISOLATION,SQL_TXN_READ_COMMITTED,SQL_IS_UINTEGER);
    if(m_rdbmsType == RDBMS_ACCESS || m_rdbmsType == RDBMS_SQLSERVER)
    {
      // Microsoft products (MS-Access and SQL-Server) can only shift the autocommit mode
      // once at the start of a connection.
      // Afterwards, after statements have occurred, it cannot be turned on or off.
      // See Microsoft KB169469 article for confirmation. It's and ODBC 3.x issue for MS-Access
      // All insert/updates/deletes **must** be transactions
      // So we work in autocommit mode = off
      // Programs **MUST** put SQLTransaction on the stack to modify the database
      SetConnectAttr(SQL_ATTR_AUTOCOMMIT,SQL_AUTOCOMMIT_OFF,SQL_IS_UINTEGER);
    }
  }

  // Set our initial autocommit mode
  SetAutoCommitMode(m_autoCommitMode);
}

// Running the initializations for the session
void
SQLDatabase::SetConnectionInitialisations()
{
  try
  {
    SQLQuery query(this);
    for(int index = 0;index < SQLCOMP_MAX_SESS_SETTINGS;++index)
    {
      PTCHAR sql = g_SQLSessionInitialization[m_rdbmsType][index];
      if(sql)
      {
        query.DoSQLStatementNonQuery(sql);
      }
      else break;
    }
  }
  catch(StdException& ex)
  {
    ReThrowSafeException(ex);
  }
}

bool 
SQLDatabase::CollectInfo()
{
  TCHAR szInfo1[_MAX_PATH];
  TCHAR szInfo2[_MAX_PATH];
  BOOL  LoadVersie = TRUE;
  SQLSMALLINT nResult = 0;

  // Make empty
  szInfo1[0] = 0;
  szInfo2[0] = 0;

  if(!IsOpen())
  {
    LogPrint(_T("Database not open at the begin of CollectInfo."));
    return false;
  }
  // Set lock on the stack
  Locker<SQLDatabase> lock(this,INFINITE);

  SqlGetInfo(m_hdbc, SQL_DATA_SOURCE_NAME, szInfo1, sizeof(szInfo1), &nResult);
  m_datasource = szInfo1;

  // DB name & version
  SqlGetInfo(m_hdbc, SQL_DBMS_NAME,szInfo1, sizeof(szInfo1),&nResult);
  SqlGetInfo(m_hdbc, SQL_DBMS_VER, szInfo2, sizeof(szInfo2),&nResult);


  if ( m_DBName.CompareNoCase(szInfo1)   == 0 && 
       m_DBVersion.CompareNoCase(szInfo2) == 0)
  {
    LoadVersie = FALSE;
  }
  m_DBName   = szInfo1;
  m_DBVersion = szInfo2;

  if (LoadVersie)
  {
    // driver name
    SqlGetInfo(m_hdbc, SQL_DRIVER_NAME,szInfo1, sizeof(szInfo1), &nResult);
    m_DriverName = szInfo1;

    // driver version
    SqlGetInfo(m_hdbc, SQL_DRIVER_VER,szInfo1, sizeof(szInfo1), &nResult);
    m_DriverVersion = szInfo1;
    int pos = m_DriverVersion.Find('.');
    if(pos >= 0)
    {
      // "02.12.0000"   CLI 2.50 INFORMIX
      // " 3.50.TC2DE"  CLI 3.50 INFORMIX
      XString main = m_DriverVersion.Left(pos);
      m_driverMainVersion = _ttoi(main);
    }

    SqlGetInfo(m_hdbc, SQL_DRIVER_ODBC_VER, szInfo1, sizeof(szInfo1), &nResult);
    m_odbcVersionComplete = szInfo1;
    m_odbcVersion = _ttoi(szInfo1);

    SqlGetInfo(m_hdbc, SQL_DBMS_NAME, szInfo1, sizeof(szInfo1), &nResult);
    m_DBName = szInfo1;

    SqlGetInfo(m_hdbc, SQL_DBMS_VER, szInfo1, sizeof(szInfo1), &nResult);
    m_DBVersion = szInfo1;

    SqlGetInfo(m_hdbc, SQL_ASYNC_MODE, &m_async_possible, sizeof(m_async_possible), &nResult);

    SqlGetInfo(m_hdbc, SQL_TXN_CAPABLE, &m_canDoTransactions, sizeof(m_async_possible), &nResult);

    SqlGetInfo(m_hdbc, SQL_NEED_LONG_DATA_LEN, szInfo1, sizeof(szInfo1), &nResult);
    m_needLongDataLen = (szInfo1[0] == 'Y');
  }

  // Get the default identifier for the kind of database
  // Consists of 6 chars name and 2 chars of main-version of the database
  // For instance "INFORM09" or "ORACLE09"
  m_DBVersion.Trim();
  m_dbIdent.Format(_T("%-6s%02d"),m_DBName.GetString(),_ttoi(m_DBVersion));


  // Get the type of the database
  XString baseName = m_dbIdent.Left(6);
  baseName.Trim();
       if(baseName.CompareNoCase(_T("INFORM")) == 0)  m_rdbmsType = RDBMS_INFORMIX;
  else if(baseName.CompareNoCase(_T("ORACLE")) == 0)  m_rdbmsType = RDBMS_ORACLE;
  else if(baseName.CompareNoCase(_T("ACCESS")) == 0)  m_rdbmsType = RDBMS_ACCESS;
  else if(baseName.CompareNoCase(_T("MICROS")) == 0)  m_rdbmsType = RDBMS_SQLSERVER;
  else if(baseName.CompareNoCase(_T("FIREBI")) == 0)  m_rdbmsType = RDBMS_FIREBIRD;
  else if(baseName.CompareNoCase(_T("POSTGR")) == 0)  m_rdbmsType = RDBMS_POSTGRESQL;
  else if(baseName.CompareNoCase(_T("MYSQL"))  == 0)  m_rdbmsType = RDBMS_MYSQL;
  else if(baseName.CompareNoCase(_T("MARIAD")) == 0)  m_rdbmsType = RDBMS_MARIADB;
  else
  {
    // Generic default type, now supported by SQLInfoGenericODBC class
    m_rdbmsType = RDBMS_ODBC_STANDARD;
  }

  // After finding the database type, set the rebinds
  SetKnownRebinds();

  // Now find the 'real' database name
  if(RealDatabaseName() == false)
  {
    return DatabaseNameFromDSN();
  }
  return true;
}

// Last resource: take name from datasource-name
bool
SQLDatabase::DatabaseNameFromDSN()
{
  // Fall back on the datasource name
  // Only if we have nothing else, because it can be different per machine
  int  pos = 0;

  m_dataIdent.Empty();
  pos = m_completeConnect.Find(_T("DSN="));
  if(pos >= 0)
  {
    pos += 4;
    TCHAR sp = m_completeConnect.GetAt(pos);
    int number = 0;
    while(sp && sp != ';' && number++ < DS_IDENT_LEN)
    {
      m_dataIdent += sp;
      sp = m_completeConnect.GetAt(++pos);
    }
  }
  else
  {
    // Cannot determine a database name
    return false;
  }
  // Record database name as the identifier
  m_databaseName = m_dataIdent;
  return true;
}

// Setting known rebind mappings of databases
void
SQLDatabase::SetKnownRebinds()
{
  // Solving formatting for various databases (Oracle / MS-Access)
  // Also see method "SQLQuery::SQLType2CType" for the use of the rebind maps
  if(m_rdbmsType == RDBMS_ORACLE)
  {
    m_rebindColumns.clear();
    m_rebindColumns[SQL_REAL   ] = SQL_C_NUMERIC;
    m_rebindColumns[SQL_FLOAT  ] = SQL_C_NUMERIC;
    m_rebindColumns[SQL_DOUBLE ] = SQL_C_NUMERIC;
  }
  else if(m_rdbmsType == RDBMS_SQLSERVER)
  {
    m_rebindParameters.clear();
    m_rebindParameters[SQL_C_SLONG] = SQL_C_LONG;
    m_rebindParameters[SQL_C_ULONG] = SQL_C_LONG;
  }
  else if(m_rdbmsType == RDBMS_ACCESS)
  {
    m_rebindParameters.clear();
    m_rebindParameters[SQL_C_SLONG] = SQL_C_LONG;
    m_rebindParameters[SQL_C_ULONG] = SQL_C_LONG;
  }
  else if(m_rdbmsType == RDBMS_POSTGRESQL)
  {
    m_rebindParameters.clear();
    m_rebindParameters[SQL_C_SLONG] = SQL_C_LONG;
    m_rebindParameters[SQL_C_ULONG] = SQL_C_LONG;
  }
  else if (m_rdbmsType == RDBMS_MARIADB)
  {
    m_rebindParameters.clear();
    m_rebindParameters[SQL_VARCHAR] = SQL_VARCHAR;
  }
}

// Add a column rebind for this database session: No bounds checking!
void
SQLDatabase::AddColumnRebind(int p_sqlType,int p_cppType)
{
  m_rebindColumns[p_sqlType] = p_cppType;
}

// Add a parameter rebind for this database session: No bounds checking!
void
SQLDatabase::AddParameterRebind(int p_sqlType,int p_cppType)
{
  m_rebindParameters[p_sqlType] = p_cppType;
}

// Get the SQL Info object by database
// SQLInfoDB Factory for subclass per database
SQLInfoDB*
SQLDatabase::GetSQLInfoDB()
{
  if(m_info == NULL && IsOpen())
  {
    switch(m_rdbmsType)
    {
      case RDBMS_ORACLE:    m_info = new SQLInfoOracle     (this); break;
      case RDBMS_INFORMIX:  m_info = new SQLInfoInformix   (this); break;
      case RDBMS_ACCESS:    m_info = new SQLInfoAccess     (this); break;
      case RDBMS_SQLSERVER: m_info = new SQLInfoSQLServer  (this); break;
      case RDBMS_POSTGRESQL:m_info = new SQLInfoPostgreSQL (this); break;
      case RDBMS_FIREBIRD:  m_info = new SQLInfoFirebird   (this); break;
      case RDBMS_MYSQL:     m_info = new SQLInfoMySQL      (this); break;
      case RDBMS_MARIADB:   m_info = new SQLInfoMariaDB    (this); break;
      default:              m_info = new SQLInfoGenericODBC(this); break;
    }
  }
  return m_info;
}

// Setting the default database schema after login
bool
SQLDatabase::SetDefaultSchema(XString p_user,XString p_schema)
{
  XString sql = GetSQLInfoDB()->GetSQLDefaultSchema(p_user,p_schema);
  if(!sql.IsEmpty())
  {
    try
    {
      SQLQuery query(this);
      query.DoSQLStatement(sql);
    }
    catch(StdException& /*ex*/)
    {
      return false;
    }
  }
  return true;
}

// Find the **REAL** database name
bool
SQLDatabase::RealDatabaseName()
{
  SQLSMALLINT len = 0;
  PTSTR buffer = NULL;
  bool  file   = false;
  bool  result = false;

  XString databaseName;
  // ODBC 1.x method. Database name.
  buffer = databaseName.GetBuffer(SQL_MAX_OPTION_STRING_LENGTH);
  SQLGetInfo(m_hdbc, SQL_DATABASE_NAME, buffer, SQL_MAX_OPTION_STRING_LENGTH, &len);
  databaseName.ReleaseBuffer();
  m_namingMethod = _T("ODBC database name");

  if(databaseName.IsEmpty())
  {
    // After ODBC 2.0, SQL_DATABASE_NAME is replaced by current_qualifier
    long length = 0;
    buffer = databaseName.GetBuffer(SQL_MAX_OPTION_STRING_LENGTH);
    SQLGetConnectAttr(m_hdbc,SQL_CURRENT_QUALIFIER,buffer,SQL_MAX_OPTION_STRING_LENGTH,&length);
    databaseName.ReleaseBuffer();
    m_namingMethod = _T("ODBC current qualifier");
  }
  if(databaseName.IsEmpty())
  {
    if(GetSQLInfoDB())
    {
      // Get the SQLInfo<Database> implementation's name
      databaseName   = m_info->GetRDBMSPhysicalDatabaseName();
      m_namingMethod = _T("Physical database name");
    }
  }
  if(databaseName.IsEmpty())
  {
    // Or else: try the server name
    buffer = databaseName.GetBuffer(SQL_MAX_OPTION_STRING_LENGTH);
    SQLGetInfo(m_hdbc,SQL_SERVER_NAME,buffer,SQL_MAX_OPTION_STRING_LENGTH,&len);
    databaseName.ReleaseBuffer();
    m_namingMethod = _T("ODBC server name");
  }
  // Strip physical name for text sources 
  // Such as: DB3, FoxBase, Firebird, MS-Access, MS-Excel, PostgreSQL
  if(databaseName.Find(_T(".")) >= 0)
  {
    file = true;
    databaseName = databaseName.Left(databaseName.Find('.'));
  }
  while(databaseName.Find(_T("\\")) != -1)
  {
    databaseName = databaseName.Mid(databaseName.Find(_T("\\")) + 1);
    file = true;
  }
  while(databaseName.Find(_T("/")) != -1)
  {
    databaseName = databaseName.Mid(databaseName.Find(_T("/")) + 1);
    file = true;
  }
  while(databaseName.Find(_T(":")) != -1)
  {
    databaseName = databaseName.Mid(databaseName.Find(_T(":")) + 1);
    file = true;
  }
  if(file == true && !databaseName.IsEmpty())
  {
    m_namingMethod += _T(" : Name stripped or physical file name");
  }
  // Found?
  if(!databaseName.IsEmpty())
  {
    result = true;
    if(m_dataIdent.IsEmpty())
    {
      // Record the database name identifier
      m_dataIdent = databaseName.Left(DS_IDENT_LEN);
    }
  }
  // Still nothing?
  if(databaseName.IsEmpty())
  {
    m_namingMethod = _T("SQL Database name unsupported");
  }
  // Register the result
  m_databaseName = databaseName;
  // Log the connection
  XString log;
  log.Format(_T("Database connection at login => DATABASE: %s\n"),databaseName.GetString());
  LogPrint(log);
  return result;
}

// Setting the database connection to read-only (if supported at all)
bool 
SQLDatabase::SetReadOnly(bool p_readOnly)
{
  SQLUINTEGER access = p_readOnly ? SQL_MODE_READ_ONLY : SQL_MODE_READ_WRITE;
  SQLInfoDB* info = GetSQLInfoDB();
  if(info == NULL)
  {
    return false;
  }
  if(!info->SetAttributeInteger(_T("read-only"),SQL_ATTR_ACCESS_MODE,access))
  {
    return false;
  }
  // Record in this class
  m_readOnly = p_readOnly;
  return true;
}

// Setting the autocommit mode
bool
SQLDatabase::SetAutoCommitMode(bool p_autoCommit)
{
  // If we try to turn it off, but the database does not understand transactions
  // We cannot set the autocommit mode to OFF and go do transactions
  if(p_autoCommit == false && m_canDoTransactions == SQL_TC_NONE)
  {
    return false;
  }
  // If we have a database type that can change the autocommit mode
  if(m_rdbmsType != RDBMS_ACCESS && m_rdbmsType != RDBMS_SQLSERVER)
  {
    // Set autocommit mode to be sure. 
    // If 'ON', Programs **CAN** use a transaction by setting SQLTransaction on the stack.
    DWORD_PTR commit = p_autoCommit ? SQL_AUTOCOMMIT_ON : SQL_AUTOCOMMIT_OFF;
    SQLRETURN ret = SqlSetConnectAttr(m_hdbc,SQL_ATTR_AUTOCOMMIT,(SQLPOINTER)commit,SQL_IS_INTEGER);
    if(Check(ret))
    {
      // If succeeded, save the autocommit mode
      m_autoCommitMode = p_autoCommit;
      return true;
    }
  }
  return false;
}

XString        
SQLDatabase::GetDatabaseTypeName()
{
  switch(m_rdbmsType)
  {
    case RDBMS_UNKNOWN:       return _T("Unknown");
    case RDBMS_ORACLE:        return _T("Oracle");
    case RDBMS_INFORMIX:      return _T("Informix");
    case RDBMS_ACCESS:        return _T("MS-Access");
    case RDBMS_SQLSERVER:     return _T("SQL-Server");
    case RDBMS_POSTGRESQL:    return _T("PostgreSQL");
    case RDBMS_FIREBIRD:      return _T("Firebird");
    case RDBMS_MYSQL:         return _T("MySQL");
    case RDBMS_MARIADB:       return _T("MariaDB");
    case RDBMS_ODBC_STANDARD: return _T("Generic ODBC");
  }
  return _T("");
}

bool
SQLDatabase::Ping()
{
  if(IsOpen())
  {
    // Send a ping query
    XString ping = GetSQLInfoDB()->GetPing();
    if(!ping.IsEmpty())
    {
      // No transaction here. We could disturb a running one!
      SQLQuery qry(this);
      return qry.DoSQLStatementScalar(ping) != nullptr;
    }
  }
  return false;
}

//////////////////////////////////////////////////////////////////////////
//
// Creating the ODBC handles.
// This code is here implemented to be able to cope with the 
// ODBC_NO_STANDARD flag in the headers
//
void
SQLDatabase::MakeEnvHandle()
{
  // Create the handle
  m_henv = SQL_NULL_HANDLE;
  SQLRETURN res = SqlAllocHandle(SQL_HANDLE_ENV,SQL_NULL_HANDLE,&m_henv);

    // Check results
  if(!Check(res))
  {
    throw StdException(_T("Error at opening: cannot create an ODBC environment."));
  }

  // Tell the driver how we use the handles
  // Henceforth we are a ODBC 3.x program
  SQLPOINTER startVersion = (SQLPOINTER)SQL_OV_ODBC3;
  res = SQLSetEnvAttr(m_henv,SQL_ATTR_ODBC_VERSION,startVersion,0);
  if(!Check(res))
  {
    throw StdException(_T("Cannot set the ODBC version for the environment."));
  }
}

void
SQLDatabase::MakeDbcHandle()
{
  // Create the handle
  m_hdbc = SQL_NULL_HANDLE;
  SQLRETURN res = SqlAllocHandle(SQL_HANDLE_DBC,m_henv,&m_hdbc);

  // Check the results
  if(!Check(res))
  {
    throw StdException(XString(_T("Error at opening: ")) + GetErrorString());
  }
}

HSTMT 
SQLDatabase::MakeStmtHandle()
{
  HSTMT stmt = SQL_NULL_HANDLE;

  // Check the HDBC
  if(m_hdbc == SQL_NULL_HANDLE)
  {
    throw StdException(_T("No database handle. Are you logged in to a database?"));
  }
  // Create the statement handle
  SQLRETURN res = SqlAllocHandle(SQL_HANDLE_STMT,m_hdbc,&stmt);

  // Check the results
  if(!Check(res))
  {
    XString fout;
    fout.Format(_T("Error creating a statement handle: ") + GetErrorString());
    throw StdException(fout);
  }
  // return the statement
  return stmt;
}

RETCODE 
SQLDatabase::GetSQLHandle(HSTMT *p_statementHandle, BOOL p_exception)
{
  try
  {
    *p_statementHandle = MakeStmtHandle();
    return SQL_SUCCESS;
  }
  catch(StdException& ex)
  {
    ReThrowSafeException(ex);
    *p_statementHandle = NULL;
    if(p_exception)
    {
      throw;
    }
  }
  return SQL_ERROR;
}

/*static*/ RETCODE 
SQLDatabase::FreeSQLHandle(HSTMT* p_statementHandle,UWORD p_option)
{
  SQLRETURN ret = SQL_SUCCESS;
  if(p_statementHandle != SQL_NULL_HSTMT)
  {
    // Call the correct deallocator
    // Will otherwise not free the cursor on the database
    ret = SQLFreeStmt(*p_statementHandle,p_option);
    // On success, remove the handle, if the option was to drop it
    if(ret == SQL_SUCCESS && p_option == SQL_DROP)
    {
      *p_statementHandle = SQL_NULL_HSTMT;
    }
  }
  return ret;
};

// Freeing the environment handle, disconnecting the ODBC driver
void
SQLDatabase::FreeEnvHandle()
{
  // Disconnect environment
  SQLRETURN ret = SqlFreeHandle(SQL_HANDLE_ENV,m_henv);
  if(Check(ret) == FALSE)
  {
    XString error = GetErrorString(0);
    LogPrint(_T("Error at closing the database environment\n"));
    LogPrint(error);
  }
  m_henv = SQL_NULL_HANDLE;
}

// Freeing the database handle, disconnecting from the database
void
SQLDatabase::FreeDbcHandle()
{
  // Time consuming for some databases
  SQLRETURN ret = SqlDisconnect(m_hdbc);
  if(Check(ret) == FALSE)
  {
    XString error = GetErrorString(0);
    LogPrint(_T("Error at closing the database\n"));
    LogPrint(error);
  }
  // And free the handle
  SqlFreeHandle(SQL_HANDLE_DBC,m_hdbc);
  m_hdbc = SQL_NULL_HANDLE;
}

void 
SQLDatabase::SetConnectAttr(int attr, int value,int type)
{
  SQLRETURN ret = SqlSetConnectAttr(m_hdbc,attr,(SQLPOINTER)(DWORD_PTR)value,type);
  if(!Check(ret))
  {
    throw StdException(XString(_T("Error at setting connection attributes at open: ")) + GetErrorString());
  }
}

// ODBC Native Support
bool
SQLDatabase::ODBCNativeSQL(XString& p_sql)
{
  // Check that we have an open connection
  if(IsOpen() == false)
  {
    return false;
  }

  // See if we have something to do
  p_sql.Trim();
  if(p_sql.IsEmpty())
  {
    return false;
  }

  // Create a buffer that's 2 times the length
  // just to be sure for native constructions
  int len = p_sql.GetLength();
  SQLTCHAR* buffer = new SQLTCHAR[(size_t)len * 2];
  SQLINTEGER lengte = 0;
  buffer[0] = 0;

  // But first replace eventual macro texts
  // Otherwise the parser will go haywire!
  ReplaceMacros(p_sql);

  // Let the driver do the translation
  SQLRETURN ret = SqlNativeSql(m_hdbc
                              ,(SQLTCHAR*)p_sql.GetString()
                              ,p_sql.GetLength() + 1
                              ,(SQLTCHAR*)buffer
                              ,2*len
                              ,&lengte);
  // Check if succeeded
  if(Check(ret))
  {
    p_sql = buffer;
    delete [] buffer;
    return true;
  }
  delete [] buffer;
  return false;
}

//////////////////////////////////////////////////////////////////////////
//
// ERROR Handling
//
//////////////////////////////////////////////////////////////////////////

XString 
SQLDatabase::GetErrorString(SQL_HANDLE statement)
{
  XString errors;
  int     number;

  // Errors on the statement-handle
  if(statement)
  {
    XString error;
    GetErrorInfo(SQL_HANDLE_STMT, statement, number, error);
    errors = error;
  }

  // Errors on the connection handle
  if(m_hdbc)
  {
    XString error;
    GetErrorInfo(SQL_HANDLE_DBC, m_hdbc, number, error);
    if(error.GetLength())
    {
      if(errors.GetLength()) errors += _T("\n");
      errors += error;
    }
  }
  // Errors on the environment handle
  if(m_henv)
  {
    XString error;
    GetErrorInfo(SQL_HANDLE_ENV, m_henv, number, error);
    if(error.GetLength())
    {
      if(errors.GetLength()) errors += _T("\n");
      errors += error;
    }
  }
  // No error information found
  if(errors.GetLength() == 0)
  {
    errors = _T("No error information is available.");
  }
  // Ready with all errors
  return errors;
}

int    
SQLDatabase::GetErrorNumber(SQL_HANDLE statement)
{
  XString text;
  int     number;

  // Errors on the statement handle
  if(statement && GetErrorInfo(SQL_HANDLE_STMT, statement, number, text))
  {
    return number;
  }
  // Errors on the connection handle
  if(m_hdbc && GetErrorInfo(SQL_HANDLE_DBC, m_hdbc, number, text))
  {
    return number;
  }
  // Errors on the environment handle
  if(m_henv && GetErrorInfo(SQL_HANDLE_ENV, m_henv, number, text))
  {
    return number;
  }
  // Nothing found
  return 0;
}

bool
SQLDatabase::GetErrorInfo(SQLSMALLINT p_type, SQLHANDLE p_handle, int& p_number, XString& p_text)
{
  // Fields for SQLGetDiagRec
  SQLTCHAR    szSqlState[SQLSTATE_LEN + 1];
  SQLINTEGER  fNativeError;
  SQLTCHAR    szErrorMsg[ERROR_BUFFERSIZE + 1];
  SQLSMALLINT cbErrorMsg;
  SQLSMALLINT recNummer = 0;
  bool        result = true;

  // Get all error records
  XString errors;
  while(true)
  {
    // Getting the diagnostic info record
    szErrorMsg[0] = 0;
    szSqlState[0] = 0;
    SQLRETURN res = SqlGetDiagRec(p_type,             // Type of the handle
                                  p_handle,           // Handle for which info is gotten
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
    if(recNummer == 1)
    {
      p_number = fNativeError;
    }
    // Remember SQLSTATE value
    if(szSqlState[0])
    {
      m_sqlState = szSqlState;
    }
    // Optional add a linefeed
    if(errors.GetLength())
    {
      errors += "\n";
    }
    // Error at getting errors
    if(!Check(res))
    {
      XString err;
      err.Format(_T("Error %d found while reading the SQL error status."), res);
      errors += err;
      result = false;
      break;
    }
    // Strip error message. Should be done at least for Oracle!!
    if(cbErrorMsg > 0 && cbErrorMsg <= ERROR_BUFFERSIZE)
    {
      szErrorMsg[cbErrorMsg] = 0;
    }
    // Take SQLState and native error into account
    XString error;
    error.Format(_T("[%s][%d]"),szSqlState,fNativeError);
    // Add state and error message
    errors += error + XString(szErrorMsg);
  }
  // ready
  p_text = errors;
  return result;
}

BOOL
SQLDatabase::Check(INT nRetCode)
{
  switch (nRetCode)
  {
    case SQL_SUCCESS_WITH_INFO: if(WilLog())
                                {
                                  XString error;
                                  error.Format(_T("=> ODBC Success with info: %s\n"),GetErrorString().GetString());
                                  LogPrint(error);
                                }
    case SQL_SUCCESS:           // Fall through
    case SQL_NO_DATA_FOUND:     return TRUE;
  }
  return FALSE;
}

//////////////////////////////////////////////////////////////////////////
//
// TRANSACTIONS
//
//////////////////////////////////////////////////////////////////////////

XString
SQLDatabase::StartTransaction(SQLTransaction* p_transaction, bool p_startSubtransaction)
{
  XString transName;

  if(m_canDoTransactions != SQL_TC_NONE)
  {
    // If no transaction active yet, we must turn of autocommit
    if(m_transactions.size() == 0)
    {
      try
      {
        if(m_rdbmsType != RDBMS_ACCESS && m_rdbmsType != RDBMS_SQLSERVER)
        {
          SetConnectAttr(SQL_ATTR_AUTOCOMMIT,SQL_AUTOCOMMIT_OFF,SQL_IS_UINTEGER);
        }
      }
      catch(StdException& error)
      {
        ReThrowSafeException(error);
        XString message;
        message.Format(_T("Error at starting transaction [%s] : %s"),p_transaction->GetName().GetString(),error.GetErrorMessage().GetString());
        throw StdException(message);
      }
    }

    // If asked so, start a sub-transaction if there was a transaction
    // otherwise this still is NOT a sub-transaction!
    if(m_transactions.size() > 0 || p_startSubtransaction)
    {
      // Get transaction name and add the 'Auto Save Point'
      transName = p_transaction->GetName();
      transName.AppendFormat(_T("ASP%d"),static_cast<int>(m_transactions.size()));

      // Set savepoint
      XString startSubtrans = m_info->GetSQLStartSubTransaction(transName);
      if(!startSubtrans.IsEmpty())
      {
        try
        {
          SQLQuery rs(this);
          rs.DoSQLStatement(startSubtrans);
          // TRACE("Start transaction: %s\n",startSubtrans.GetString());
        }
        catch(StdException& err)
        {
          ReThrowSafeException(err);
          XString message;
          message.Format(_T("Error starting sub-transaction [%s:%s] : %s")
                        ,p_transaction->GetName().GetString()
                        ,transName.GetString()
                        ,err.GetErrorMessage().GetString());
          throw StdException(message);
        }
      }
    }
  }
  // Add the transaction on the transaction stack
  m_transactions.push(p_transaction);

  // return the transaction's name
  return transName;
}

void 
SQLDatabase::CommitTransaction(SQLTransaction* p_transaction)
{
  if(m_canDoTransactions != SQL_TC_NONE)
  {
    // Check that this transaction is the top-of-the-stack transaction
    if(GetTransaction() != p_transaction)
    {
      // Note: If this exception is indeed reached it means that
      // a transaction lower on the stack is now being committed
      // This is clearly not what we want, and points to an error
      // in our application's logic in the calling code.
      XString message;
      message.Format(_T("Error at commit: transaction [%s] is not the current transaction"),p_transaction->GetName().GetString());
      throw StdException(message);
    }

    // Only the last transaction will really be committed
    if(m_transactions.size() == 1)
    {
      // Try to commit this transaction
      // As this may lead to exceptions and violations 
      // we do this in a try - catch block
      try
      {
        // Do the commit
        if(!Check(SqlEndTran(SQL_HANDLE_DBC, m_hdbc, SQL_COMMIT)))
        {
          // Throw something, so we reach the catch block
          throw StdException(0);
        }
        // Re-engage the autocommit mode. If it goes wrong we
        // will automatically reach the catch block
        if(m_rdbmsType != RDBMS_ACCESS && m_rdbmsType != RDBMS_SQLSERVER)
        {
          SetConnectAttr(SQL_ATTR_AUTOCOMMIT,SQL_AUTOCOMMIT_ON,SQL_IS_UINTEGER);
        }
      }
      catch(StdException& ex) 
      {
        ReThrowSafeException(ex);
        // Get the error information
        XString error = GetErrorString();

        // do the rollback of the transaction
        RollbackTransaction(p_transaction);

        // Throw an exception with the error info of the failed commit
        XString message;
        message.Format(_T("Error in commit of transaction [%s] : %s. OS Error: %s")
                      ,p_transaction->GetName().GetString()
                      ,ex.GetErrorMessage().GetString()
                      ,error.GetString());
        throw StdException(message);
      }
    }
    else
    {
      // It's a sub transaction
      // If the database is capable: Do the commit of the sub transaction
      // Otherwise: do nothing and wait for the outer transaction to commit the whole in-one-go
      XString startSubtrans = GetSQLInfoDB()->GetSQLCommitSubTransaction(p_transaction->GetSavePoint());
      if(!startSubtrans.IsEmpty())
      {
        try
        {
          SQLQuery rs(this);
          rs.DoSQLStatement(startSubtrans);
          TRACE("Commit transaction: %s\n",startSubtrans.GetString());
        }
        catch(StdException& error)
        {
          ReThrowSafeException(error);
          XString message;
          message.Format(_T("Error in commit of sub-transaction [%s:%s] : %s")
                        ,p_transaction->GetName().GetString()
                        ,p_transaction->GetSavePoint().GetString()
                        ,error.GetErrorMessage().GetString());
          throw StdException(message);
        }
      }
    }
  }
  // Remove transaction from the stack
  m_transactions.pop();
}

void 
SQLDatabase::RollbackTransaction(SQLTransaction* p_transaction)
{
  // Check that it is the top-of-the-stack transaction
  if(GetTransaction() != p_transaction)
  {
    XString message;
    message.Format(_T("Error in rollback: transaction [%s] is not the current transaction"),p_transaction->GetName().GetString());
    throw StdException(message);
  }

  // Look for the first savepoint on the stack
  // Beware: the transaction is always removed from the stack
  // even if the rollback may fail.
  // So we cannot try to rollback or commit it again
  TransactionStack transactions;
  while(m_transactions.size())
  {
    // Get the top of the stack
    p_transaction = m_transactions.top();
    m_transactions.pop();

    // Save if for later use
    transactions.push(p_transaction);

    // See if it is a savepoint
    if(!p_transaction->GetSavePoint().IsEmpty())
    {
      // We will do a rollback to this savepoint
      break;
    }
  }

  if(m_canDoTransactions != SQL_TC_NONE)
  {
    // Rollback the transaction
    if(p_transaction->GetSavePoint().IsEmpty())
    {
      // It's the main transaction
      try
      {
        // Do the rollback
        if(!Check(SqlEndTran(SQL_HANDLE_DBC, m_hdbc, SQL_ROLLBACK)))
        {
          // Throw something, so we reach the catch block
          throw StdException(0);
        }
        // Re-engage the autocommit mode, will throw in case of an error
        if(m_rdbmsType != RDBMS_ACCESS && m_rdbmsType != RDBMS_SQLSERVER)
        {
          SetConnectAttr(SQL_ATTR_AUTOCOMMIT,SQL_AUTOCOMMIT_ON,SQL_IS_UINTEGER);
        }
      }
      catch(StdException& ex)
      {
        ReThrowSafeException(ex);
        // Throw an exception with error info at a failed rollback1
        XString message;
        XString error = GetErrorString();
        message.Format(_T("Error at rollback of transaction [%s] : %s. OS Error: %s")
                       ,p_transaction->GetName().GetString()
                       ,ex.GetErrorMessage().GetString()
                       ,error.GetString());
        throw StdException(message);
      }
    }
    else
    {
      // It is a sub-transaction
      XString startSubtrans = m_info->GetSQLRollbackSubTransaction(p_transaction->GetSavePoint());
      if(!startSubtrans.IsEmpty())
      {
        try
        {
          SQLQuery rs(this);
          rs.DoSQLStatement(startSubtrans);
          TRACE("Rollback transaction: %s\n",startSubtrans.GetString());
        }
        catch(StdException& error)
        {
          ReThrowSafeException(error);
          XString message;
          message.Format(_T("Error in rolling back sub-transaction [%s:%s] : %s")
                        ,p_transaction->GetName().GetString()
                        ,p_transaction->GetSavePoint().GetString()
                        ,error.GetErrorMessage().GetString());
          throw StdException(message);
        }
      }
    }
  }
  // Notify all rolled back transactions
  while(transactions.size())
  {
    transactions.top()->AfterRollback();
    transactions.pop();
  }
}

SQLTransaction* 
SQLDatabase::GetTransaction()
{
  // return the current top-of-the-stack transaction
  return m_transactions.size() ? m_transactions.top() : 0;
}

// Before closing the database, close transactions
void
SQLDatabase::CloseAllTransactions()
{
  SQLRETURN ret = 0;

  // See if there are pending transactions,
  if(m_transactions.empty())
  {
    // Commit last SELECT in multi-version databases
    // Otherwise we gat an error at the disconnect of de HDBC
    ret = SqlEndTran(SQL_HANDLE_DBC,m_hdbc,SQL_COMMIT);
  }
  else
  {
    // IF SO: rollback these transactions
    ret = SqlEndTran(SQL_HANDLE_DBC,m_hdbc,SQL_ROLLBACK);

    // Notifying all transactions that we are done!
    // and clearing the transactions stack
    while(!m_transactions.empty())
    {
      m_transactions.top()->AfterRollback();
      m_transactions.pop();
    }
  }
  if(Check(ret) == FALSE)
  {
    XString error = GetErrorString(0);
    LogPrint(_T("Error in rollback at closing the database\n"));
    LogPrint(error);
  }
}

//////////////////////////////////////////////////////////////////////////
//
// DATABASE DEPENDENT STRINGS
//
//////////////////////////////////////////////////////////////////////////

XString
SQLDatabase::GetSQLTimeString(int p_hour,int p_minute,int p_second)
{
  if(GetSQLInfoDB())
  {
    return m_info->GetSQLTimeString(p_hour,p_minute,p_second);
  }
  return _T("");
}

XString
SQLDatabase::GetStrippedSQLTimeString(int p_hour,int p_minute,int p_second)
{
  if(GetSQLInfoDB())
  {
    return m_info->GetSQLTimeString(p_hour,p_minute,p_second);
  }
  return _T("");
}

XString
SQLDatabase::GetSQLDateString(int p_day, int p_month, int p_year)
{
  if(GetSQLInfoDB())
  {
    return m_info->GetSQLDateString(p_year,p_month,p_day);
  }
  return _T("");
}

XString
SQLDatabase::GetCurrentTimestampQualifier()
{
  if(GetSQLInfoDB())
  {
    return m_info->GetKEYWORDCurrentTimestamp();
  }
  return _T("");
}

XString
SQLDatabase::GetSQL_NewSerial(XString p_table,XString p_sequence)
{
  XString sql;
  if(GetSQLInfoDB())
  {
    sql = m_info->GetSQLNewSerial(p_table,p_sequence);
  }
  return sql;
}

XString 
SQLDatabase::GetSQL_GenerateSerial(XString p_table,XString p_sequence /*=""*/)
{
  XString query;
  if(GetSQLInfoDB())
  {
    if(p_sequence.IsEmpty())
    {
      query = m_info->GetSQLGenerateSerial(p_table);
    }
    else
    {
      query = m_info->GetSQLGenerateSequence(p_sequence);
    }
  }
  if(query.Left(6).CompareNoCase(_T("SELECT")) == 0)
  {
    SQLQuery rs(this);
    rs.DoSQLStatement(query);
    if(rs.GetRecord())
    {
      int serial = rs[1];
      XString result;
      result.Format(_T("%d"),serial);
      return result;
    }
    return "0";
  }
  return query;
}

int 
SQLDatabase::GetSQL_EffectiveSerial(XString p_oid_string)
{
  XString query;
  if(GetSQLInfoDB())
  {
    query = m_info->GetSQLEffectiveSerial(p_oid_string);
  }
  if(query.Left(6).CompareNoCase(_T("SELECT")) == 0)
  {
    // Now get that number
    SQLQuery rs(this);
    rs.DoSQLStatement(query);
    if(rs.GetRecord())
    {
      int num = rs.GetColumn(1)->GetAsSLong();
      return num;
    }
    return 0;
  }
  return _ttoi(query);
}

XString 
SQLDatabase::GetTimestampAsString(const SQLTimestamp& p_timestamp)
{
  if(GetSQLInfoDB())
  {
    return m_info->GetSQLDateTimeString(p_timestamp.Year()
                                       ,p_timestamp.Month()
                                       ,p_timestamp.Day()
                                       ,p_timestamp.Hour()
                                       ,p_timestamp.Minute()
                                       ,p_timestamp.Second());
  }
  return "";
}

XString 
SQLDatabase::GetTimestampAsBoundString()
{
  if(GetSQLInfoDB())
  {
    return m_info->GetSQLDateTimeBoundString();
  }
  return "";
}

XString
SQLDatabase::GetSQLDateTimeStrippedString(const SQLTimestamp& p_timestamp)
{
  if(GetSQLInfoDB())
  {
    return m_info->GetSQLDateTimeStrippedString(p_timestamp.Year()
                                               ,p_timestamp.Month()
                                               ,p_timestamp.Day()
                                               ,p_timestamp.Hour()
                                               ,p_timestamp.Minute()
                                               ,p_timestamp.Second());
  }
  return "";
}

XString
SQLDatabase::GetInterval1MinuteAgo()
{
  if(GetSQLInfoDB())
  {
    return m_info->GetKEYWORDInterval1MinuteAgo();
  }
  return "";
}

XString
SQLDatabase::GetUpper(XString p_veld)
{
  if(GetSQLInfoDB())
  {
    return m_info->GetKEYWORDUpper(p_veld);
  }
  return "";
}

XString
SQLDatabase::GetNVLStatement(XString p_test,XString p_isnull)
{
  if(GetSQLInfoDB())
  {
    return m_info->GetKEYWORDStatementNVL(p_test,p_isnull);
  }
  return "";
}

// Oracle results caching on/off switching
void 
SQLDatabase::SetOracleResultCacheMode(const XString& p_mode)
{
  // Check to see if we are logged in, and 
  if(!GetSQLInfoDB() || (m_rdbmsType != RDBMS_ORACLE))
  {
    return;
  }

  // See if we've got a setting
  // Check mode parameter for correct values
  XString query;
  if(p_mode.CompareNoCase(_T("manual")) == 0 ||
     p_mode.CompareNoCase(_T("force")) == 0 ||
     p_mode.CompareNoCase(_T("auto")) == 0)
  {
    query = _T("ALTER SESSION SET RESULT_CACHE_MODE = ") + p_mode;
  }
  if(query.IsEmpty())
  {
    return;
  }
  // Execute on the database
  try
  {
    SQLQuery rs(this);
    rs.DoSQLStatement(query);
  }
  catch(StdException& ex)
  {
    ReThrowSafeException(ex);
    XString error = _T("Database error while setting RESULT_CACHE_MODE: ") + ex.GetErrorMessage();
    throw StdException(error);
  }
}

// Setting the database type (once)
// Can only be done in unopened state 
// Opening a database will overwrite this
bool
SQLDatabase::SetDatabaseType(DatabaseType p_type)
{
  if(m_rdbmsType == RDBMS_UNKNOWN && !IsOpen())
  {
    MakeEnvHandle();
    MakeDbcHandle();
    m_rdbmsType = p_type;
    return true;
  }
  return false;
}

//////////////////////////////////////////////////////////////////////////
//
// Various service routines
//
//////////////////////////////////////////////////////////////////////////

void
SQLDatabase::GetDataSources(DSMap& p_list,int p_type /*= SQL_FETCH_FIRST*/)
{
  if(m_henv == NULL)
  {
    MakeEnvHandle();
  }
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
  if(SqlDataSources(m_henv
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
      DataSourceInternal source;
      source.m_datasource = server;
      source.m_system     = (direction == SQL_FETCH_FIRST_SYSTEM);
      source.m_outputOMF  = false;
      source.m_default    = false;
      p_list.push_back(source);
    } 
    while(SQL_SUCCEEDED(SqlDataSources(m_henv,SQL_FETCH_NEXT
                                      ,server
                                      ,serverLengthInp
                                      ,&serverLengthOut
                                      ,description
                                      ,descLengthInp
                                      ,&descLengthOut)));
  }
}

// Get the name of a special ODBC driver
// e.g. the Excel-ODBC driver and searches for a sub-capability
// e.g. the first ".xls" or ".xlsx" capable driver
XString
SQLDatabase::GetSpecialDriver(XString p_base,XString p_extensie)
{
  SQLTCHAR    driverDescription[250];
  SQLSMALLINT dLength = 0;
  SQLTCHAR    attribDescription[250];
  SQLSMALLINT aLength = 0;
  RETCODE     result  = SQL_ERROR;

  // Make handle
  if(m_henv == NULL)
  {
    MakeEnvHandle();
  }
  // Get the names of the installed drivers 1 by 1.
  result = SqlDrivers(m_henv,SQL_FETCH_FIRST
                     ,driverDescription,250,&dLength
                     ,attribDescription,250,&aLength);
  while(SQL_SUCCEEDED(result))
  {
    // See if it's e.g. an Excel driver and if it supports
    // the right extension (xls or xlsx)
    // Most of the time there are multiple Excel drivers on the system
    if(_tcsstr(reinterpret_cast<TCHAR*>(driverDescription),p_base)     != 0 &&
       _tcsstr(reinterpret_cast<TCHAR*>(driverDescription),p_extensie) != 0 )
    {
      // Found !
      return XString(driverDescription);
    }
    // GET Next driver
    result = SqlDrivers(m_henv,SQL_FETCH_NEXT
                       ,driverDescription,250,&dLength
                       ,attribDescription,250,&aLength);
  }
  // Nothing found
  return XString();
}

//////////////////////////////////////////////////////////////////////////
//
// LOGGING SUPPORT
//
//////////////////////////////////////////////////////////////////////////

// Support printing to generic logfile
void
SQLDatabase::LogPrint(LPCTSTR p_text)
{
  // If the loglevel is above the activation level
  if(m_loggingLevel >= m_logActive)
  {
    // If we have both the printing routine AND the context
    if(m_logPrinter && m_logContext)
    {
      // Print to the logfile
      (*m_logPrinter)(m_logContext,p_text);
    }
  }
}

// Getting the current loglevel
int
SQLDatabase::LogLevel()
{
  if(m_logContext)
  {
    m_loggingLevel = (*m_logLevel)(m_logContext);
    return m_loggingLevel;
  }
  return -1;
}

bool
SQLDatabase::WilLog()
{
  // If we have both the loglevel routine AND the context
  if(m_logLevel && m_logContext)
  {
    // Refresh the loglevel
    m_loggingLevel = (*m_logLevel)(m_logContext);
    // True if at log active threshold or above
    if(m_loggingLevel >= m_logActive)
    {
      return true;
    }
  }
  return false;
}

//////////////////////////////////////////////////////////////////////////
//
// SCHEMA QUERY SUPPORT
//
//////////////////////////////////////////////////////////////////////////

void 
SQLDatabase::SetSchema(XString p_schema)
{
  m_schemaName   = p_schema;
  m_schemaAction = p_schema.IsEmpty() ? SCHEMA_NO_ACTION : SCHEMA_REPLACE;
  
  // Database types that cannot do a schema query
  if(m_rdbmsType == RDBMS_ACCESS && m_schemaAction == SCHEMA_REPLACE)
  {
    m_schemaAction = SCHEMA_REMOVE;
  }
}

void
SQLDatabase::ParseSchema(XString& p_query)
{
  if(m_schemaAction == SCHEMA_REMOVE)
  {
    p_query.Replace(_T("$SCHEMA."), _T(" "));
  }
  else if(m_schemaAction == SCHEMA_REPLACE)
  {
    p_query.Replace(_T("$SCHEMA"),m_schemaName);
  }
}

void
SQLDatabase::SetSchemaAction(SchemaAction p_action)
{
  if(p_action >= SCHEMA_NO_ACTION && p_action <= SCHEMA_REPLACE)
  {
    m_schemaAction = p_action;
    if(p_action == SCHEMA_NO_ACTION || p_action == SCHEMA_REMOVE)
    {
      m_schemaName.Empty();
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
// MACROS
//
//////////////////////////////////////////////////////////////////////////

// Do the Query text macro replacement
void           
SQLDatabase::ReplaceMacros(XString& p_statement)
{
  for(const auto& macro : m_macros)
  {
    XString text = macro.first;
    XString repl = macro.second;

    while(true)
    {
      // Search the macro text
      int pos = p_statement.Find(text);
      if(pos < 0)
      {
        break;
      }
      // Replace the macro
      int quotes = FindQuotes(p_statement,pos);
      if((quotes % 2) == 0)
      {
        // Do the replacement
        ReplaceMacro(p_statement,pos,text.GetLength(),repl);
      }
    }
  }
}

int
SQLDatabase::FindQuotes(XString& p_statement,int p_lastpos)
{
  int quotes = 0;
  for(int ind = 0; ind < p_lastpos; ++ind)
  {
    if(p_statement.GetAt(ind) == '\'')
    {
      ++quotes;
    }
  }
  return quotes;
}

void
SQLDatabase::ReplaceMacro(XString& p_statement,int p_pos,int p_length,XString p_replace)
{
  XString newStatement;
  // First part, before macro
  newStatement = p_statement.Left(p_pos);
  // Add macro replacement
  newStatement += p_replace;
  // Add part after the macro
  newStatement += p_statement.Mid(p_pos + p_length);

  // Result
  p_statement = newStatement;
}

// Add a macro replacement for SQL text
void
SQLDatabase::AddMacro(XString p_macro,XString p_replacement)
{
  p_macro.MakeUpper();
  m_macros[p_macro] = p_replacement;
}

// Remove macro
void
SQLDatabase::DeleteMacro(XString p_macro)
{
  p_macro.MakeUpper();
  Macros::iterator it = m_macros.find(p_macro);
  if(it != m_macros.end())
  {
    m_macros.erase(it);
  }
}

//////////////////////////////////////////////////////////////////////////
//
// LOCKING THE DATABASE
//
//////////////////////////////////////////////////////////////////////////

// Acquire a multi threaded lock for SQLQuery
void 
SQLDatabase::Acquire(unsigned /*p_timeout*/)   
{
  EnterCriticalSection(&m_databaseLock);
}

// Release the multi threaded lock for SQLQuery
void 
SQLDatabase::Release()   
{
  LeaveCriticalSection(&m_databaseLock);
}

bool
SQLDatabase::IsLocked()
{
  if(TryEnterCriticalSection(&m_databaseLock))
  {
    LeaveCriticalSection(&m_databaseLock);
    return false;
  }
  return true;
}

// End of namespace
}
