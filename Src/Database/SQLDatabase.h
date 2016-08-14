////////////////////////////////////////////////////////////////////////
//
// File: Database.h
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

#include <afx.h>
#include <afxdb.h>
#include <afxcoll.h>
#include <sql.h>
#include <sqlext.h>
#include "SQLTransaction.h"
#include <stack>
#include <vector>
#include <map>

// Redefine all handle types to pointer-to-void
#define SQL_HANDLE void*

// Max length of the resulting connect string
#define CONNECTSTRING_MAXLEN 1024

// Derived name of max 10 chars length
// By convention in use for older INFORMIX systems
#define DS_IDENT_LEN 10

// Loglevel for various systems (BRIEF4all)
// Log queries at this loglevel
#define LOGLEVEL_MAX    6
// Log actions at this level
#define LOGLEVEL_ACTION 2

// After this much minutes, the database can no longer be
// used from a database pool and will 'go-away'
#define IDLE_MINUTES   5

// Standard login timeout after which login will fail
// Overrides the standard login timeout of the database
#define LOGIN_TIMEOUT   15

// Maximum string length of the error buffer to be gotten
#define ERROR_BUFFERSIZE 4096

// Length of the SQLSTATE: See ISO standard
#define SQLSTATE_LEN 6

// Huidige herkende database typen
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
 ,RDBMS_PERVASIVE     = 8
 ,RDBMS_EXCEL         = 9
}
DatabaseType;

typedef enum _schemaAction
{
  SCHEMA_NO_ACTION    = 0
 ,SCHEMA_REMOVE       = 1
 ,SCHEMA_REPLACE      = 2
}
SchemaAction;

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

typedef std::vector<DataSourceInternal> DSMap;
typedef std::stack<SQLTransaction*>     TransactieStack;
typedef std::map<int,int>               RebindMap;
typedef std::map<CString,CString>       ODBCOptions;

typedef void (CALLBACK* LOGPRINT)(void*,const char*);
typedef int  (CALLBACK* LOGLEVEL)(void*);

// Foreward declaration
class SQLInfo;
class SQLInfoTree;
class SQLTimestamp;

class SQLDatabase : public CObject
{
public:
  SQLDatabase();
  SQLDatabase(HDBC p_hdbc);
 ~SQLDatabase(); 
  void Reset();

  // OPEN/CLOSE CYCLE
  
  // Get the available datasources in a list
  void           GetDataSources(DSMap& p_list,int p_type = SQL_FETCH_FIRST);
  // Override of the login timeout of the database engine (in seconds)
  void           SetLoginTimeout(int p_timeout);
  // Set the Multiple Active Record Set option (MARS) for MS-SQL-Server
  void           SetMARS(bool p_set);
  // Setting the database connection to readonly (if supported at all)
  bool           SetReadOnly(bool p_readOnly);
  // Add a general ODBC option for use in the connection string
  void           AddConnectOption(CString p_keyword,CString p_value);
  
  // Open the database on basis of datasource, user and password
  bool           Open(CString const& p_datasource
                     ,CString const& p_username
                     ,CString const& p_password
                     ,bool           p_readOnly = false);
  // Open the database on basis of a connect string only
  bool           Open(CString const& p_connectString,bool p_readOnly = false);

  bool           IsOpen();       // Is de database open?
  void           Close();        // Close it for further use
  bool           CollectInfo();  // Collect database info
  void           SetDirtyRead(); // Readmode for Informix

  // GETTING/CONSTRUCTING the SQLInfo object
  SQLInfo*       GetSQLInfo();
  SQLInfoTree*   GetSQLInfoTree();

  // DATABASE POOL METHODEN
  void           SetDatasource(CString p_dsn);
  void           SetLastActionTime();
  bool           PastWaitingTime();

  // GETTERS
  CString        GetUserName();
  CString        GetPassword();
  CString        GetDatasource();
  CString        GetDatabaseTypeName();
  CString        GetDatabaseName();         // Real database name
  DatabaseType   GetDatabaseType();
  CString        GetNamingMethod();
  CString        GetConnect();              // Resulting connet string
  CString        GetOriginalConnect();      // Original  connect string
  int            GetODBCVersion();          // Main version 1/2/3
  CString        GetODBCVersionComplete();  // Compolete ODBC version e.g."3.51"
  bool           GetNeedLongDataLen();
  CString        GetDataIdent();
  CString        GetDBIdent();
  CString        GetDBVendorName();
  CString        GetDBVendorVersion();
  CString        GetDBVendorDriverName();
  CString        GetDBVendorDriverVersion();
  RebindMap*     GetRebindMapParameters();
  RebindMap*     GetRebindMapColumns();
  CString        GetSQLState();
  bool           GetReadOnly();
  int            GetLoginTimeout();

  // SUPPORT FOR SQLQuery
  HDBC           GetDBHandle();
  RETCODE        GetSQLHandle (HSTMT* p_statementHandle, BOOL p_exception);
  static RETCODE FreeSQLHandle(HSTMT* p_statementHandle, UWORD p_option = SQL_DROP);
  CString        GetErrorString(SQL_HANDLE statement = 0);
  int            GetErrorNumber(SQL_HANDLE statement = 0);
  bool           GetErrorInfo(SQLSMALLINT type, SQLHANDLE handle, int& nummer, CString& tekst);
  BOOL           Check(INT nRetCode);
  // ODBC Native Support
  bool           ODBCNativeSQL(CString& p_sql);

  // TRANSACTION SUPPORT
  CString         StartTransaction   (SQLTransaction* p_transaction, bool startSubtransactie);
  void            CommitTransaction  (SQLTransaction* p_transaction);
  void            RollbackTransaction(SQLTransaction* p_transaction);
  SQLTransaction* GetTransaction();

  // Get the name of a special ODBC driver
  CString        GetSpecialDriver(CString p_base,CString p_extensie);
  // Oracle results caching on/off switching
  void           SetOracleResultCacheMode(const CString& mode);

  // Asking for database-dependent constructions
  CString        GetSQLTimeString        (int p_hour,int p_minute,int p_second);
  CString        GetStrippedSQLTimeString(int p_hour,int p_minute,int p_second);
  CString        GetSQLDateString        (int p_day, int p_month, int p_year);
  CString        GetCurrentTimestampQualifier();
  CString        GetSQL_GenerateSerial(CString p_table);
  int            GetSQL_EffectiveSerial(CString p_oid_string);
  CString        GetTimestampAsString(const SQLTimestamp& p_timestamp);
  CString        GetTimestampAsBoundString();
  CString        GetSQLDateTimeStrippedString(const SQLTimestamp& p_timestamp);
  CString        GetInterval1MinuteAgo();
  CString        GetNVLStatement(CString p_test,CString p_isnull);
  CString        GetUpper(CString p_veld);

  // Support of logging functions 
  void           RegisterLogContext(int p_level,LOGLEVEL p_loglevel,LOGPRINT p_logprinter,void* p_logContext);
  // Printing to generic logfile
  void           LogPrint(int p_level,const char* p_text);
  int            LogLevel();
  bool           WilLog(int p_loglevel);

  // SCHEMA HANDLING
  
  void           SetSchema(CString p_schema);
  void           SetSchemaAction(SchemaAction p_action);
  void           ParseSchema(CString& p_query);

  // MULTI-THREADING LOCKING

  // Acquire a multithread lock for SQLQuery
  void    Acquire(unsigned p_timeout);
  // Release the multithread lock for SQLQuery
  void    Release();
  // See if the database is locked
  bool    IsLocked();

protected:
  // HANDLE MANAGEMENT
  void    MakeEnvHandle();   // Environment
  void    MakeDbcHandle();   // Database
  HSTMT   MakeStmtHandle();  // Statement
  // Setting of a connection attribute
  void    SetConnectAttr(int attr,int value,int type);
  // Setting known rebind mappings of databases
  void           SetKnownRebinds();
  // Various database-dependent ways of determining the 'name-of-the-database
  bool    RealDatabaseName();
  // Last resource: take name from datasource-name
  bool           DatabaseNameFromDSN();
  // Setting connection attributes BEFORE connect
  void           SetAttributesBeforeConnect();
  // Setting connection attributes AFTER connect
  void           SetAttributesAfterConnect(bool p_readOnly);

  // From connect
  CString           m_datasource;           // Datasource at login
  CString           m_username;             // System user
  CString           m_password;             // System's password

  // Info about the database
  DatabaseType      m_rdbmsType;            // Which RDBMS engine
  SQLInfo*          m_info;                 // The SQLInfo object
  SQLInfoTree*      m_infoTree;             // The TREE object
  int               m_loginTimeout;         // Timeout before login fails
  bool              m_mars;                 // Multiple Active Record Sets
  bool              m_readOnly;             // ReadOnly connection
  CString	          m_DBName;
  CString	          m_DBVersie;
  CString	          m_DriverName;
  CString	          m_DriverVersion;
  CString           m_databaseName;
  CString           m_namingMethod;
  CString           m_originalConnect;
  CString           m_completeConnect;
  SQLUINTEGER       m_async_possible;
  SQLUSMALLINT      m_canDoTransactions;
  CString           m_odbcVersionComplete;
  int               m_odbcVersion;
  int               m_driverMainVersion;       // Main version (2/3) van de Informix driver
  bool              m_needLongDataLen;
  RebindMap         m_rebindParameters;        // Rebinding of parameters for SQLBindParam
  RebindMap         m_rebindColumns;           // Rebinding of result columns for SQLBindCol
  CString           m_sqlState;                // Last SQLSTATE
  CString           m_schemaName;
  SchemaAction      m_schemaAction;

  // Derived identifier names for various systems (BRIEF4all, Key2Brief)
  CString           m_dbIdent;                 // Database   identifier (6 chars name, 2 chars main-version)
  CString           m_dataIdent;               // Datasource identifier (10 chars at most = DS_IDENT_LEN)
  // Handles
  HENV              m_henv;
  HDBC              m_hdbc;

  // Generic logging
  LOGPRINT          m_logPrinter;       // Printing a line to the logger
  LOGLEVEL          m_logLevel;         // Getting the loglevel
  void*             m_logContext;       // Logging context (e.g. and object)
  int               m_loggingLevel;     // Current level
  
  // Login options for connect string
  ODBCOptions       m_options;
  // Last moment of usage (for database pool)
  DWORD             m_lastAction;
  // Locking  
  CRITICAL_SECTION  m_databaseLock;
  // Transaction adminstration
  TransactieStack   m_transactions;
};

inline bool 
SQLDatabase::IsOpen()
{
  return (m_hdbc != NULL);
}

inline CString
SQLDatabase::GetOriginalConnect()
{
  return m_originalConnect;
}

inline CString
SQLDatabase::GetConnect()
{
  // Result from DriverConnect
  return m_completeConnect;
}

inline CString
SQLDatabase::GetUserName()
{
  return m_username;
}

inline CString
SQLDatabase::GetPassword()
{
  return m_password;
}

inline CString        
SQLDatabase::GetDatasource()
{
  return m_datasource;
}

inline int
SQLDatabase::GetODBCVersion()
{
  return m_odbcVersion;
}

inline CString
SQLDatabase::GetODBCVersionComplete()
{
  return m_odbcVersionComplete;
}

inline DatabaseType   
SQLDatabase::GetDatabaseType()
{
  return m_rdbmsType;
}

inline CString
SQLDatabase::GetDatabaseName()
{
  return m_databaseName;
}

inline bool
SQLDatabase::GetNeedLongDataLen()
{
  return m_needLongDataLen;
}

inline void
SQLDatabase::RegisterLogContext(int p_level,LOGLEVEL p_loglevel,LOGPRINT p_logprinter,void* p_logContext)
{
  m_loggingLevel = p_level;
  m_logLevel     = p_loglevel;
  m_logPrinter   = p_logprinter;
  m_logContext   = p_logContext;
}

inline HDBC
SQLDatabase::GetDBHandle()
{
  return m_hdbc;
}

inline CString
SQLDatabase::GetNamingMethod()
{
  return m_namingMethod;
}

inline void
SQLDatabase::SetLoginTimeout(int p_timeout)
{
  m_loginTimeout = p_timeout;
}

inline CString        
SQLDatabase::GetDBVendorName()
{
  return m_DBName;
}

inline CString
SQLDatabase::GetDBVendorVersion()
{
  return m_DBVersie;
}

inline CString 
SQLDatabase::GetDBVendorDriverName()
{
  return m_DriverName;
}

inline CString        
SQLDatabase::GetDBVendorDriverVersion()
{
  return m_DriverVersion;
}

inline CString
SQLDatabase::GetDataIdent()
{
  // return m_dBIdent;
  return m_dataIdent;
}

inline RebindMap*
SQLDatabase::GetRebindMapParameters()
{
  return &m_rebindParameters;
}

inline RebindMap*
SQLDatabase::GetRebindMapColumns()
{
  return &m_rebindColumns;
}

inline CString
SQLDatabase::GetDBIdent()
{
  return m_dbIdent;
}

inline void
SQLDatabase::SetMARS(bool p_set)
{
  m_mars = p_set;
}

inline CString
SQLDatabase::GetSQLState()
{
  return m_sqlState;
}

inline bool
SQLDatabase::GetReadOnly()
{
  return m_readOnly;
}

inline int
SQLDatabase::GetLoginTimeout()
{
  return m_loginTimeout;
}
