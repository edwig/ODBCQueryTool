////////////////////////////////////////////////////////////////////////
//
// File: SQLInfo.h
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

// include files needed here
#include "SQLDatabase.h"
#include <sql.h>
#include <sqlext.h>
#include <map>
#include <list>

// SQL/CLI Standard states that 128 is the maximum length
// for any identifier in any namespace
#define SQL_MAX_IDENTIFIER  128
#define SQL_MAX_BUFFER      129

// Finding META types
#define META_CATALOGS      1
#define META_SCHEMAS       2
#define META_TABLES        3

typedef struct _TypeInfo
{
  CString     m_type_name;
  SQLSMALLINT m_data_type;
  SQLINTEGER  m_precision;
  CString     m_literal_prefix;
  CString     m_literal_suffix;
  CString     m_create_params;
  SQLSMALLINT m_nullable;
  SQLSMALLINT m_case_sensitive;
  SQLSMALLINT m_searchable;
  SQLSMALLINT m_unsigned;
  SQLSMALLINT m_money;
  SQLSMALLINT m_autoincrement;
  CString     m_local_type_name;
  SQLSMALLINT m_minimum_scale;
  SQLSMALLINT m_maximum_scale;
}
TypeInfo;

typedef struct _PrimKeyInfo
{
  CString  m_colName;
  int      m_colPos;
  int      m_queryPos;
}
PrimKeyInfo;

typedef std::map<int,PrimKeyInfo>   PrimaryMap;
typedef std::map<CString,TypeInfo*> DataTypeMap;
typedef std::list<CString>          WordList;

class SQLInfo : public CObject
{
public:
   SQLInfo(SQLDatabase* p_database);
  ~SQLInfo();

  // Initialize all internal data structures to defaults
  void Init();
  // (Re)set the connected database (e.g. after re-open)
  void SetConnection(SQLDatabase* p_database = NULL);

  // Get all static info from the ODBC driver via SQLGetInfo 
  void GetInfo();

  // Add an ODBC SQL Keyword
  bool AddSQLWord(CString sql);
  // Extra to be done on getting info 
  virtual void OnGetInfo(HDBC ,int ) {return;};
  // Is it a correct identifier (type 0=table,1=column)
  bool IsCorrectName(LPCSTR naam,int type = 0);
  // Can we start a transaction on the database
  bool CanStartTransaction();
  // Returns the fact whether an API function is supported
  // by the ODBC driver, regardless of ODBC version
  bool SupportedFunction(unsigned int api_function);
  // Return the native SQL command from an ODBC-escaped command
  CString NativeSQL(HDBC hdbc,CString& sqlCommand);
  // AT_EXEC data provider needs length beforehand
  bool NeedLongDataLen();
  // Character name of an SQL_XXX datatype
  CString ODBCDataType(int DataType);
  // Show metadata warning (for interactive mode)
  void    ShowMetaDataWarning(bool p_show);

  // All the version numbers from the driver
  CString GetVersionODBCManager();
  CString GetVersionODBCDriver();
  CString GetVersionODBCStandard();
  CString GetVersionRDBMS();

  // Get information about the primary key of a table
  bool GetPrimaryKeyInfo(CString&    p_tablename
                        ,CString&    p_primary
                        ,PrimaryMap& p_keymap);

  // GETTING ALL THE TABLES OF A NAME PATTERN
  // GETTING ALL THE INFO FOR ONE TABLE
  bool MakeInfoTableTablepart (WordList* p_list,WordList* p_tables,CString& p_findTable);
  bool MakeInfoTableColumns   (WordList* p_list);
  bool MakeInfoTablePrimary   (WordList* p_list,CString& primary,PrimaryMap& keymap);
  bool MakeInfoTableForeign   (WordList* p_list,bool ref = false);
  bool MakeInfoTableStatistics(WordList* p_list,CString& keyName,PrimaryMap& keyMap);
  bool MakeInfoTableSpecials  (WordList* p_list);
  bool MakeInfoTablePrivileges(WordList* p_list);
  // GETTING ALL THE INFO FOR ONE PROCEDURE
  bool MakeInfoProcedureProcedurepart(WordList* p_list,CString& procedure);
  bool MakeInfoProcedureParameters   (WordList* p_list);
  // GETTING ALL META TYPES
  bool MakeInfoMetaTypes(WordList* p_list,int type);

  // Meta pointer to SQLGet<META> functions
  unsigned char* GetMetaPointer(unsigned char* p_buffer,bool p_meta);

  // CONNECTION ATTRIBUTES

  // Retrieve the fact that we are living or dead
  bool    GetConnectionDead();
  // Connection in quiet mode
  int     GetAttributeQuiet();
  // Automatic population of the IPD, so driver supports prepared statements
  int     GetAttributeAutoIPD();
  // ODBC Tracing is on or off
  bool    GetAttributeTracing();
  // The file to which we are tracing
  CString GetAttributeTraceFile();
  // Getting the current catalog name
  CString GetAttributeCatalog();
  // Getting the IP packet size
  int     GetAttributePacketSize();
  // METADATA ID is supported?
  bool    GetAttributeMetadataID();
  // The current transaction level
  int     GetAttributeTransLevel();
  // The translation library path
  CString GetAttributeTranslib();
  // Translation option of the translation library
  int     GetAttributeTransoption();
  // Connection by File DSN
  CString GetAttributeFileDSN();
  CString GetAttributeFileDSNSave();
  // Timeout for the whole connection
  int     GetAttributeConnTimeout();

  // Connection by File DSN
  void    SetAttributeFileDSN    (CString p_fileDSN);
  void    SetAttributeFileDSNSave(CString p_fileDSN);
  // Set the using of ODBC cursors
  bool    SetAttributeOdbcCursors(int p_cursors);
  // Setting the optimal IP Packet size
  bool    SetAttributePacketSize(int p_packet);
   // Setting the ODBC Tracing file
  bool    SetAttributeTraceFile(CString p_traceFile);
  // Set tracing on or off
  bool    SetAttributeTracing(bool p_tracing);
  // Setting the automatic connection timeout (if supported)
  bool    SetAttributeConnTimeout(int p_timeout);
  // Setting the METADATA-ID of the connection
  bool    SetAttributeMetadataID(bool p_metadata);
  // Setting the transaction isolation level
  bool    SetAttributeTransLevel(int p_txnlevel);
  // Setting the transaction library (with or without connection)
  bool    SetAttributeTranslib(CString p_transLib);
  // Setting the translation optioen (with or without connection)
  bool    SetAttributeTransoption(int p_transOption);

private:
  // SQLDatabase has access to attribute methods
  friend SQLDatabase;

  // Get the supported ODBC functions (2.x/3.x)
  void SupportedODBCFunctions();
  // Make a statement handle for METADATA query
  bool GetStatement(bool p_metadataID = true);
  // Close the internal statement handle
  void CloseStatement();
  // Get a string from GetInfo with extra security checks for overflow
  CString GetInfoString(SQLUSMALLINT info);
  // Get a 32 bit integer value from GetInfo
  unsigned int GetInfoInteger(SQLUSMALLINT info);
  // Get a 16 bit integer value from GetInfo
  unsigned short GetInfoShortInteger(SQLUSMALLINT info);

  // Getting a general INTEGER connection attribute
  int     GetAttributeInteger(CString description,SQLINTEGER attrib);
  // Getting a general STRING connection attribute
  CString GetAttributeString(CString description,SQLINTEGER attrib);
  // Setting an INTEGER attribute
  bool    SetAttributeInteger(CString description,SQLINTEGER  attrib,SQLUINTEGER value);
  // Setting a STRING attribute
  bool    SetAttributeString(CString description,SQLINTEGER attrib,SQLCHAR* value);

protected:
  // Get the catalog.schema.table from a user string
  void    GetObjectName (CString& name
                        ,unsigned char* search_catalog
                        ,unsigned char* search_schema
                        ,unsigned char* search_table
                        ,unsigned char* search_type);
  // Reprint the catalog.schema.table combination
  CString MakeObjectName(SQLCHAR* search_catalog
                        ,SQLCHAR* search_schema
                        ,SQLCHAR* search_table
                        ,SQLCHAR* search_type);

protected:
  bool           m_initDone;           // Already read in?
  SQLDatabase*   m_database;           // Database for which we provide the info
  HDBC           m_hdbc;               // Database handle (if open)
  HSTMT          m_hstmt;              // Statement handle for info of tables/procedures
  RETCODE        m_retCode;            // Generic return code from ::SQL function

  // Temporary caching of the current object being queried
  CString     m_searchCatalogName;
  CString     m_searchSchemaName;
  CString     m_searchTableName;
  CString     m_searchTableType;

	WordList    m_ODBCKeywords;          // Stationary ODBC keywords
  WordList    m_RDBMSkeywords;         // Keywords  reported by the RDBMS
  DataTypeMap m_dataTypes;             // Datatypes reported by the RDBMS

  // CONFORMANCE TO THE SQL-LANGUAGE
  CString      m_cli_year;             // Year of X/Open CLI standard
  SQLUINTEGER  m_sql_conformance;      // Overall SQL  Conformance
  SQLUINTEGER  m_odbc_conformance;     // Overall ODBC Conformance
  SQLUINTEGER  m_cli_conformance;      // Overall CLI  Conformance
  SQLUINTEGER  m_aggr_functions;       // Supports  MIN/MAX/SUM/COUNT/AVG/DISTINCT
  SQLUINTEGER  m_alterDomain;          // Supported ALTER  DOMAIN variants
  SQLUINTEGER  m_alterTable;           // Supported ALTER  TABLE  variants
  SQLUINTEGER  m_createDomain;         // Supported CREATE DOMAIN variants
  SQLUINTEGER  m_createSchema;         // Supported CREATE SCHEMA variants
  SQLUINTEGER  m_createTable;          // Supported CREATE TABLE  variants
  SQLUINTEGER  m_createView;           // Supported CREATE VIEW   variants
  SQLUINTEGER  m_dropDomain;           // Supported DROP   DOMAIN variants
  SQLUINTEGER  m_dropSchema;           // Supported DROP   SCHEMA variants
  SQLUINTEGER  m_dropTable;            // Supported DROP   TABLE  variants
  SQLUINTEGER  m_dropView;             // Supported DROP   VIEW   variants
  SQLUINTEGER  m_grantOptions;         // Supported GRANT         variants
  SQLUINTEGER  m_insertOptions;        // Supported INSERT        variants
  SQLUINTEGER  m_revokeOptions;        // Supported REVOKE        variants
  SQLUINTEGER  m_indexOptions1;        // Supported CREATE/DROP INDEX
  SQLUINTEGER  m_indexOptions2;        // Supported ASC/DESC on INDEX
  SQLUINTEGER  m_predicates;           // Supported WHERE predicates
  SQLUINTEGER  m_groupBy;              // Supported GROUP BY behaviour
  SQLUINTEGER  m_unions;               // Supported UNION variants
  SQLUINTEGER  m_rowConstructors;      // Supported row constructors in select
  SQLSMALLINT  m_maxColSelect;         // Supported max columns in select
  SQLSMALLINT  m_maxTabSelect;         // Supported max tables  in FROM
  SQLSMALLINT  m_maxColGroupBy;        // Supported max columns in group by
  SQLSMALLINT  m_maxColOrderBy;        // Supported max columns in order by
  SQLSMALLINT  m_maxColTable;          // Supported max columns in a table
  SQLSMALLINT  m_maxColIndex;          // Supported max columns in a index
  SQLUINTEGER  m_funcNumeric;          // Supported numeric   functions
  SQLUINTEGER  m_funcString;           // Supported string    functions ODBC 1.x/2.x
  SQLUINTEGER  m_funcString3;          // Supported string    functions ODBC 3.x
  SQLUINTEGER  m_funcSystem;           // Supported system    functions
  SQLUINTEGER  m_funcDateTime;         // Supported date/time functions
  SQLUINTEGER  m_funcTimestampadd;     // Supported TIMESTAMPADD  fields
  SQLUINTEGER  m_funcTimestampdiff;    // Supported TIMESTAMPDIFF fields
  bool         m_orderByInSelect;      // Order by must be in the select
  bool         m_columnAliases;        // Supports "columnname AS alias"
  bool         m_exprInOrderBy;        // Supports expressions in the ORDER BY
  SQLSMALLINT  m_correlationNames;     // Correlation of table aliases
  SQLUINTEGER  m_oj_cap;               // Outer join capabilities in WHERE
  SQLUINTEGER  m_oj_cap92;             // Outer join capabilities in FROM
  SQLUINTEGER  m_valExpressions;       // Value Expressions
  SQLUINTEGER  m_predSubqueries;       // Predicates that support subqueries
  SQLUINTEGER  m_deleteRules;          // Supported DELETE rules
  SQLUINTEGER  m_updateRules;          // Supported UPDATE rules

  // How an object is named. Standard is m_catalogName.m_schemaName.m_tableName
  bool         m_supportsCatalogs;      // Database supports catalogs
  CString      m_catalogName;           // empty if catalogs ar not supported
  CString      m_schemaName;            // Standard is 'owner'
  CString      m_tableName;             // Standard is 'table'
  SQLUSMALLINT m_catalogLocation;       // Before schema or after tablename
  SQLUINTEGER  m_catalogUsage;          // Where catalog names can be used
  CString      m_catalogNameSeparator;  // Separator for the catalog name
  SQLSMALLINT  m_identifierCase;        // Case-specific of names in the catalog
  CString      m_procedureName;         // Standard is 'procedure'

  // RDBMS Implementation specifics
  SQLSMALLINT  m_fileUsage;             // Type of database storage!
  SQLSMALLINT  m_maxCatalogName;
  SQLSMALLINT  m_maxSchemaName;
  SQLSMALLINT  m_maxTableNaam;
  SQLSMALLINT  m_maxColumNaam;
  bool         m_accesibleProcedures;   // User can execute all returned names
  bool         m_accesibleTables;       // User can select from all returned names
  SQLSMALLINT  m_txn_cap;               // Capable of transactions
  SQLINTEGER   m_txnLevel;              // Current transacation level
  SQLSMALLINT  m_cursor_commit;
  SQLSMALLINT  m_cursor_rollback;
  SQLSMALLINT  m_activeEnvironments;    // Maximum of environments (catalogs?)
  CString      m_specialChars;
  CString      m_likeEscape;
  SQLUINTEGER  m_datetimeLiterals;      // Supported date/time/interval literals
  CString      m_identifierQuote;       // RDBMS identifier quote character
  CString      m_collationSequence;     // RDBMS default collation sequence
  SQLINTEGER   m_defaultTransaction;    // RDBMS default transaction level
  SQLINTEGER   m_schemaViews;           // INFORMATION_SCHEMA support
  SQLSMALLINT  m_concatBehaviour;       // NULL concat behaviour
  CString      m_userName;              // User name in database
  SQLSMALLINT  m_nullCollation;         // How NULL's are sorted
  SQLSMALLINT  m_nullableColumns;       // Columns must be nullable
  bool         m_integrity;             // Integrity support
  bool         m_needLongDataLen;       // Need length for SQL_AT_EXEC 
  SQLINTEGER   m_packetSize;            // Optimization of the IP packet size
  SQLINTEGER   m_connTimeout;           // Connection timeout
  SQLINTEGER   m_odbcCursors;           // Using ODBC Cursors
  CString      m_transLib;              // Translation library
  SQLINTEGER   m_transOption;           // Translation option

  // DATATYPE conversions
  SQLUINTEGER  m_conversionFuncs;       // Supported conversion functions
  SQLINTEGER   m_convertBigint;         // Convert BIGINT to ...
  SQLINTEGER   m_convertBinary;         // Convert BINARY to ...
  SQLINTEGER   m_convertBit;            // Convert BIT to ...
  SQLINTEGER   m_convertChar;           // Convert CHAR to ...
  SQLINTEGER   m_convertDate;           // Convert DATE to ...
  SQLINTEGER   m_convertDecimal;        // Convert DECIMAL to ...
  SQLINTEGER   m_convertDouble;         // Convert DOUBLE to ...
  SQLINTEGER   m_convertFloat;          // Convert FLOAT to ...
  SQLINTEGER   m_convertInteger;        // Convert INTEGER to ...
  SQLINTEGER   m_convertIYM;            // Convert INTERVAL year to month to ...
  SQLINTEGER   m_convertIDT;            // Convert INTERVAL day to second to ...
  SQLINTEGER   m_convertLVarBinary;     // Convert LONGVARBINARY to ...
  SQLINTEGER   m_convertLVarchar;       // Convert LONGVARCHAR to ...
  SQLINTEGER   m_convertNumeric;        // Convert NUMERIC to ...
  SQLINTEGER   m_convertReal;           // Convert REAL to ...
  SQLINTEGER   m_convertSmallint;       // Convert SMALLINT to ...
  SQLINTEGER   m_convertTime;           // Convert TIME to ...
  SQLINTEGER   m_convertTimestamp;      // Convert TIMESTAMP to ...
  SQLINTEGER   m_convertTinyint;        // Convert TINYINT to ...
  SQLINTEGER   m_convertVarBinary;      // Convert VARBINARY to ...
  SQLINTEGER   m_convertVarchar;        // Convert VARCHAR to ...

  // VERSION NUMBERS of the various layers
  CString      m_manager_version;      // ODBC Driver manager version (from MS-Windows)
  CString      m_odbc_driver_version;  // ODBC Driver version
  CString      m_odbc_version;         // ODBC Standard version
  CString      m_rdbms_version;        // RDBMS Version
  // The ODBC Driver
  CString      m_driverName;           // Name of the driver DLL
  SQLUINTEGER  m_getdata_extensions;   // Driver SQLGetData extensions
  // File DSN
  CString      m_fileDSN;
  CString      m_fileDSNSave;

  // Work arounds
  bool         m_metadataID;
  bool         m_METADATA_ID_unsupported;
  bool         m_METADATA_ID_errorseen;
  // ODBC supported functions
  bool         m_functions_use_3;
  SQLUSMALLINT m_ODBCFunctions_2[100];   // ODBC 1.x and 2.x standard is 100 int's
  SQLUSMALLINT m_ODBCFunctions_3[SQL_API_ODBC3_ALL_FUNCTIONS_SIZE]; // ODBC 3.x standard = 4000 bits

  // How to use META types
  bool         m_canFindCatalogs;
  bool         m_canFindSchemas;
  bool         m_canFindTypes;
};
 
inline CString 
SQLInfo::GetVersionODBCManager()
{
  return m_manager_version;
}

inline CString 
SQLInfo::GetVersionODBCDriver()
{
  return m_odbc_driver_version;
}

inline CString 
SQLInfo::GetVersionODBCStandard()
{
  return m_odbc_version;
}

inline CString 
SQLInfo::GetVersionRDBMS()
{
  return m_rdbms_version;
}

inline bool 
SQLInfo::NeedLongDataLen()
{
  return m_needLongDataLen;
}

inline void
SQLInfo::ShowMetaDataWarning(bool p_show)
{
  m_METADATA_ID_errorseen = !p_show;
}

// Connection by File DSN
inline CString 
SQLInfo::GetAttributeFileDSN()
{
  return m_fileDSN;
}

inline CString 
SQLInfo::GetAttributeFileDSNSave()
{
  return m_fileDSNSave;
}

inline void
SQLInfo::SetAttributeFileDSN(CString p_fileDSN)
{
  m_fileDSN = p_fileDSN;
}

inline void
SQLInfo::SetAttributeFileDSNSave(CString p_fileDSN)
{
  m_fileDSNSave = p_fileDSN;
}
