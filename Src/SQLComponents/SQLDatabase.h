////////////////////////////////////////////////////////////////////////
//
// File: Database.h
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
#include "SQLComponents.h"
#include "SQLTransaction.h"
#include <sql.h>
#include <sqlext.h>
#include <stack>
#include <vector>
#include <map>

namespace SQLComponents
{

// Redefine all handle types to pointer-to-void
#define SQL_HANDLE void*

// Max length of the resulting connect string
#define CONNECTSTRING_MAXLEN 1024

// Derived name of max 10 chars length
// By convention in use for older INFORMIX systems
#define DS_IDENT_LEN 10

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

// SQLSetConnectAttr driver specific defines.
// Microsoft has 1200 through 1249 reserved for Microsoft SQL Server Native Client driver usage.
// Multiple Active Result Set (MARS) per connection
// COPT_SS_MARS_ENABLED comes effectively from <sqlncli.h> from the MS-SQL Native client driver
#define SQL_COPT_SS_MARS_ENABLED    1224  
#define SQL_MARS_ENABLED_NO         0L
#define SQL_MARS_ENABLED_YES        1L

typedef enum _schemaAction
{
  SCHEMA_NO_ACTION    = 0
 ,SCHEMA_REMOVE       = 1
 ,SCHEMA_REPLACE      = 2
}
SchemaAction;

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

typedef std::vector<DataSourceInternal> DSMap;
typedef std::stack<SQLTransaction*>     TransactionStack;
typedef std::map<int,int>               RebindMap;
typedef std::map<CString,CString>       ODBCOptions;
typedef std::map<CString,CString>       Macros;

typedef void (CALLBACK* LOGPRINT)(void*,const char*);
typedef int  (CALLBACK* LOGLEVEL)(void*);

// Forward declaration
class SQLInfoDB;
class SQLTimestamp;

//////////////////////////////////////////////////////////////////////////
//
// THE DATABASE CLASS
//
//////////////////////////////////////////////////////////////////////////

class SQLDatabase
{
public:
  SQLDatabase();
  SQLDatabase(HDBC p_hdbc);
 ~SQLDatabase(); 
  void Reset();

  // OPEN/CLOSE CYCLE
  
  // Open the database on basis of datasource, user and password
  bool           Open(CString const& p_datasource
                     ,CString const& p_username
                     ,CString const& p_password
                     ,bool           p_readOnly = false);
  // Open the database on basis of a connect string only
  bool           Open(CString const& p_connectString,bool p_readOnly = false);

  bool           IsOpen();       // Is the database open?
  void           Close();        // Close it for further use
  bool           CollectInfo();  // Collect database info

  // OPTIONS FOR THE OPEN/CLOSE CYCLE

  // Override of the login timeout of the database engine (in seconds)
  void           SetLoginTimeout(int p_timeout);
  // Set the Multiple Active Record Set option (MARS) for MS-SQL-Server
  void           SetMARS(bool p_set);
  // Setting the database connection to read-only (if supported at all)
  bool           SetReadOnly(bool p_readOnly);
  // Setting the autocommit mode
  bool           SetAutoCommitMode(bool p_autoCommit);
  // Add a general ODBC option for use in the connection string
  void           AddConnectOption(CString p_keyword,CString p_value);

  // GETTING/CONSTRUCTING the SQLInfo object
  SQLInfoDB*     GetSQLInfoDB();

  // MACRO's FOR SQL TEXT

  // Do the Query text macro replacement
  void           ReplaceMacros(CString& p_statement);
  // Add a macro replacement for SQL text
  void           AddMacro(CString p_macro,CString p_replacement);
  // Remove macro
  void           DeleteMacro(CString p_macro);

  // DATABASE POOL METHODS
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
  CString        GetConnect();              // Resulting connect string
  CString        GetOriginalConnect();      // Original  connect string
  int            GetODBCVersion();          // Main version 1/2/3
  CString        GetODBCVersionComplete();  // Complete ODBC version e.g."3.51"
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
  bool           GetAutoCommitMode();

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
  // Get the available datasources in a list
  void           GetDataSources(DSMap& p_list,int p_type = SQL_FETCH_FIRST);
  // Setting the database type (once)
  bool           SetDatabaseType(DatabaseType p_type);

  // Asking for database-dependent constructions
  // Shortcuts to a SQLInfoDB function
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
  void           LogPrint(int p_level,const char* p_text);
  int            LogLevel();
  bool           WilLog(int p_loglevel);
  
  // SCHEMA HANDLING
  void           SetSchema(CString p_schema);
  void           SetSchemaAction(SchemaAction p_action);
  void           ParseSchema(CString& p_query);

  // MULTI-THREADING LOCKING
  void           Acquire(unsigned p_timeout);   // Acquire a multi threading lock
  void           Release();                     // Releasing the lock
  bool           IsLocked();                    // See if database is locked

protected:
  // HANDLE MANAGEMENT
  void           MakeEnvHandle();   // Environment
  void           MakeDbcHandle();   // Database
  HSTMT          MakeStmtHandle();  // Statement
  void           FreeEnvHandle();   // Environment
  void           FreeDbcHandle();   // Database
  // Before closing the database, close transactions
  void           CloseAllTransactions();
  // Setting of a connection attribute
  void           SetConnectAttr(int attr, int value,int type);
  // Setting known rebind mappings of databases
  void           SetKnownRebinds();
  // Various database-dependent ways of determining the 'name-of-the-database
  bool           RealDatabaseName();
  // Last resource: take name from datasource-name
  bool           DatabaseNameFromDSN();
  // Setting connection attributes BEFORE connect
  void           SetAttributesBeforeConnect();
  // Setting connection attributes AFTER connect
  void           SetAttributesAfterConnect(bool p_readOnly);
  // Find number of quotes up to the lastpos position
  int            FindQuotes(CString& p_statement,int p_lastpos);
  // Replace **ONE** macro in the statement text
  void           ReplaceMacro(CString& p_statement,int p_pos,int p_length,CString p_replace);

  // HOW WE ARE CONNECTED TO A DATABASE
  CString           m_datasource;  // Datasource at login
  CString           m_username;    // System user
  CString           m_password;    // System's password

  // Info about the database
  DatabaseType      m_rdbmsType    = { RDBMS_UNKNOWN };  // Which RDBMS engine
  SQLInfoDB*        m_info         = { nullptr       };  // The SQLInfo object
  int               m_loginTimeout = { LOGIN_TIMEOUT };  // Timeout before login fails
  bool              m_mars         = { true          };  // Multiple Active Record Sets
  bool              m_readOnly     = { false         };  // ReadOnly connection
  CString	          m_DBName;
  CString	          m_DBVersion;
  CString	          m_DriverName;
  CString	          m_DriverVersion;
  CString           m_databaseName;
  CString           m_namingMethod;
  CString           m_originalConnect;
  CString           m_completeConnect;
  SQLUINTEGER       m_async_possible      = { 0     };
  SQLUSMALLINT      m_canDoTransactions   = { 0     };
  CString           m_odbcVersionComplete;
  int               m_odbcVersion         = { 0     };
  int               m_driverMainVersion   = { 0     };   
  bool              m_needLongDataLen     = { false };
  bool              m_autoCommitMode      = { true  };
  DWORD             m_lastAction          = { 0     };  // Last moment of usage (for database pool)
  RebindMap         m_rebindParameters;                 // Rebinding of parameters for SQLBindParam
  RebindMap         m_rebindColumns;                    // Rebinding of result columns for SQLBindCol
  CString           m_sqlState;                         // Last SQLSTATE
  CString           m_schemaName;
  SchemaAction      m_schemaAction = { SCHEMA_NO_ACTION };
  Macros            m_macros;                      // Macro replacements for SQL

  // Derived identifier names for various systems
  CString           m_dbIdent;                     // Database   identifier (6 chars name, 2 chars main-version)
  CString           m_dataIdent;                   // Datasource identifier (10 chars at most = DS_IDENT_LEN)
  // Handles
  HENV              m_henv = { SQL_NULL_HANDLE };
  HDBC              m_hdbc = { SQL_NULL_HANDLE };

  // Generic logging
  LOGPRINT          m_logPrinter   = { nullptr };  // Printing a line to the logger
  LOGLEVEL          m_logLevel     = { nullptr };  // Getting the log level
  void*             m_logContext   = { nullptr };  // Logging context (e.g. and object)
  int               m_loggingLevel = { 0       };  // Current level
  
  // Login options for connect string
  ODBCOptions       m_options;
  // Locking  
  CRITICAL_SECTION  m_databaseLock;
  // Transaction administration
  TransactionStack  m_transactions;
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
  return m_DBVersion;
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

inline bool
SQLDatabase::GetAutoCommitMode()
{
  return m_autoCommitMode;
}

// End of namespace
}
