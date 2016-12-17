////////////////////////////////////////////////////////////////////////
//
// File: SQLInfo.cpp
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
#include "stdafx.h"
#include "SQLDatabase.h"
#include "SQLQuery.h"
#include "SQLInfo.h"
#include "SQLWrappers.h"
#include <sqlext.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning (disable: 4996)
#pragma warning (disable: 4312)

// This macro is used for synchronious ODBC calls
#define ODBC_CALL_ONCE(SQLFunc) \
  try \
  { \
  m_retCode = SQL_ERROR; \
  m_retCode = (SQLFunc); \
  } catch(...) {}

SQLInfo::SQLInfo(SQLDatabase* p_database)
        :m_database(p_database)
        ,m_hdbc(NULL)
        ,m_initDone(false)
{
  m_METADATA_ID_errorseen = false;
  SetConnection();
  Init();
}

SQLInfo::~SQLInfo()
{
  Init();
}

void
SQLInfo::SetConnection(SQLDatabase* p_database /*=NULL*/)
{
  if(p_database)
  {
    m_database = p_database;
  }
  if(m_database && m_database->IsOpen())
  {
    m_hdbc = m_database->GetDBHandle();
  }
}

// Initialize all internal data structures to defaults
void
SQLInfo::Init()
{
  m_fileUsage           = 0;
  m_sql_conformance     = 0;
  m_odbc_conformance    = 0;
  m_cli_conformance     = 0;
  m_maxTableNaam        = 0;
  m_maxColumNaam        = 0;
  m_oj_cap              = 0;
  m_oj_cap92            = 0;
  m_txn_cap             = 0;
  m_txnLevel            = 0;
  m_cursor_commit       = 0;
  m_cursor_rollback     = 0;
  m_aggr_functions      = 0;
  m_alterDomain         = 0;
  m_alterTable          = 0;
  m_createDomain        = 0;
  m_createTable         = 0;
  m_createSchema        = 0;
  m_createView          = 0;
  m_dropDomain          = 0;
  m_dropTable           = 0;
  m_dropSchema          = 0;
  m_dropView            = 0;
  m_grantOptions        = 0;
  m_indexOptions1       = 0;
  m_indexOptions2       = 0;
  m_insertOptions       = 0;
  m_revokeOptions       = 0;
  m_predicates          = 0;
  m_groupBy             = 0;
  m_unions              = 0;
  m_identifierCase      = 0;
  m_supportsCatalogs    = false;
  m_catalogLocation     = 0;
  m_catalogUsage        = 0;
  m_datetimeLiterals    = 0;
  m_funcNumeric         = 0;
  m_funcString          = 0;
  m_funcString3         = 0;
  m_funcSystem          = 0;
  m_funcDateTime        = 0;
  m_correlationNames    = 0;
  m_activeEnvironments  = 0;
  m_defaultTransaction  = 0;
  m_schemaViews         = 0;
  m_maxColSelect        = 0;
  m_maxColGroupBy       = 0;
  m_maxColOrderBy       = 0;
  m_maxColTable         = 0;
  m_maxColIndex         = 0;
  m_maxTabSelect        = 0;
  m_valExpressions      = 0;
  m_predSubqueries      = 0;
  m_concatBehaviour     = 0;
  m_rowConstructors     = 0;
  m_nullCollation       = 0;
  m_nullableColumns     = 0;
  m_deleteRules         = 0;
  m_updateRules         = 0;
  m_funcTimestampadd    = 0;
  m_funcTimestampdiff   = 0;
  m_packetSize          = 0;
  m_odbcCursors         = 0;
  m_accesibleTables     = false;
  m_accesibleProcedures = false;
  m_exprInOrderBy       = false;
  m_orderByInSelect     = false;
  m_integrity           = false;
  m_cli_year            = "";
  m_collationSequence   = "";
  m_searchCatalogName   = "";
  m_searchSchemaName    = "";
  m_searchTableName     = "";
  m_catalogName         = "";
  m_schemaName          = "";
  m_tableName           = "";
  m_procedureName       = "";
  m_userName            = "";
  m_catalogNameSeparator= "";
  m_specialChars        = "";
  m_likeEscape          = "";
  m_identifierQuote     = "";
  m_collationSequence   = "";
  m_manager_version     = "";
  m_odbc_driver_version = "";
  m_rdbms_version       = "";

  // Fill as ANSI-ISO 9075E standard
  m_specialChars     = "";
  m_likeEscape       = "\\";
  m_identifierQuote  = "\"";
  m_catalogName      = "catalog";
  m_schemaName       = "schema";
  m_tableName        = "table";
  m_procedureName    = "procedure";
  m_orderByInSelect  = true;
  m_columnAliases    = true;
  m_maxCatalogName   = 128;
  m_maxSchemaName    = 128;
  m_maxColumNaam     = 128;
  m_maxTableNaam     = 128;

  // Conversions
  m_conversionFuncs   = 0;
  m_convertBigint     = 0;
  m_convertBinary     = 0;
  m_convertBit        = 0;
  m_convertChar       = 0;
  m_convertDate       = 0;
  m_convertDecimal    = 0;
  m_convertDouble     = 0;
  m_convertFloat      = 0;
  m_convertInteger    = 0;
  m_convertIYM        = 0;
  m_convertIDT        = 0;
  m_convertLVarBinary = 0;
  m_convertLVarchar   = 0;
  m_convertNumeric    = 0;
  m_convertReal       = 0;
  m_convertSmallint   = 0;
  m_convertTime       = 0;
  m_convertTimestamp  = 0;
  m_convertTinyint    = 0;
  m_convertVarBinary  = 0;
  m_convertVarchar    = 0;

  // The driver
  m_driverName         = "";
  m_getdata_extensions = 0;

  // Statement
  m_hstmt              = NULL;
  m_retCode            = SQL_SUCCESS;

  // Workarounds
  m_metadataID              = true; // DEFAULT!!
  m_METADATA_ID_unsupported = false;
  m_METADATA_ID_errorseen   = false;
  m_functions_use_3         = true;
  m_canFindCatalogs         = true;
  m_canFindSchemas          = true;
  m_canFindTypes            = true;

  // Empty all mappings
  m_RDBMSkeywords.clear();
  m_ODBCKeywords.clear();

  // Alle datatypen verwijderen
  for(auto& type : m_dataTypes)
  {
    delete type.second;
  }
  m_dataTypes.clear();
}

// Add an ODBC SQL Keyword
bool
SQLInfo::AddSQLWord(CString sql)
{
  for(WordList::iterator it = m_ODBCKeywords.begin(); it != m_ODBCKeywords.end(); ++it)
  {
    if(sql.CompareNoCase(*it) == 0)
    {
      // Does already exist
      return false;
    }
  }
  m_ODBCKeywords.push_back(sql);
  return true;
}

// Get the supported ODBC functions (2.x/3.x)
void
SQLInfo::SupportedODBCFunctions()
{
  m_functions_use_3 = true;
  memset(m_ODBCFunctions_2,0,sizeof(m_ODBCFunctions_2));
  memset(m_ODBCFunctions_3,0,sizeof(m_ODBCFunctions_3));
  m_retCode = SqlGetFunctions(m_hdbc,SQL_API_ODBC3_ALL_FUNCTIONS,m_ODBCFunctions_3);
  if(!SQL_SUCCEEDED(m_retCode))
  {
    m_functions_use_3 = false;
    m_retCode = SqlGetFunctions(m_hdbc,SQL_API_ALL_FUNCTIONS,m_ODBCFunctions_2);
    if(!SQL_SUCCEEDED(m_retCode))
    {
      TRACE("Cannot determine which ODBC functions are supported. Proceed with caution!");
      TRACE("In effect: Get a better driver, because this is a basic ODBC function!\n");
    }
  }
}

// Get a string from GetInfo with extra security checks for overflow
CString
SQLInfo::GetInfoString(SQLUSMALLINT info)
{
  SQLSMALLINT len  = 0;
  bool    overflow = false;
  CString answer;
  char buffer[5120];

  if(::SQLGetInfo(m_hdbc,info,buffer,5120,&len) == SQL_SUCCESS)
  {
    if(len >= 0 && len < 5120)
    {
      // This occurs on some ODBC drivers as strings are not really '0' terminated
      // so there is a trailing 'ллллллллллллллллллллллллллллллллл' buffer
      // Occurs only on sloppy drivers
      buffer[len] = 0;
    }
    else
    {
      // Truncate data from the driver. Buffer overflow already occurred
      buffer[5119] = 0;
      overflow     = true;
    }
  }
  else
  {
    // Init buffer for the case that we are not SQL_SUCCESS
    buffer[0] = 0;
  }
  if(overflow)
  {
    AfxMessageBox("Buffer overflow on ::SQLGetInfo()\n\r"
                  "This is a serious error in the ODBCDriver\n\r"
                  "Please close this program and get a better ODBC driver!!"
                  ,MB_OK | MB_ICONERROR);
  }
  answer = buffer;
  return answer;
}

// Get a 32 bit integer value from GetInfo
unsigned int
SQLInfo::GetInfoInteger(SQLUSMALLINT info)
{
  SQLUINTEGER  val32 = 0;
  SQLSMALLINT  len   = 0;

  m_retCode = SqlGetInfo(m_hdbc,info,&val32,0,&len);
  if(SQL_SUCCEEDED(m_retCode))
  {
    if(len == 4)
    {
      return (unsigned int) val32; 
    }
  }
  return 0;
}

// Get a 16 bit integer value from GetInfo
unsigned short
SQLInfo::GetInfoShortInteger(SQLUSMALLINT info)
{
  SQLUSMALLINT val16 = 0;
  SQLSMALLINT  len   = 0;

  m_retCode = SqlGetInfo(m_hdbc,info,&val16,0,&len);
  if(SQL_SUCCEEDED(m_retCode))
  {
    if(len == 2)
    {
      return (unsigned int) val16;
    }
  }
  return 0;
}

// Get all static info from the ODBC driver via SQLGetInfo 
void
SQLInfo::GetInfo()
{
  // Only do this once!
  if(m_initDone)
  {
    return;
  }

  char  buffer[5120];
  char  woord [1024];
  char* pw;
  char* pb;
  SQLSMALLINT len;

  Init();
  if(!m_hdbc)
  {
    return;
  }
  SupportedODBCFunctions();
  
  if(!SupportedFunction(SQL_API_SQLGETINFO))
  {
    TRACE("Cannot get ODBC info with SQLGetInfo. Cannot determine capabilities\n");
    TRACE("In effect: Get a better driver, because this is a basic ODBC function!\n");
    return;
  }
  // STATIC KEYWORDS FROM ODBC 3.5 as of compilation
  if (m_ODBCKeywords.size() == 0)
  {
    woord[0] = '\0';
    pw = woord;
    for(pb = SQL_ODBC_KEYWORDS;*pb != '\0';pb++)
    {
      if (*pb == ',')
      {
        *pw = '\0';
        m_ODBCKeywords.push_back(woord);
        pw = woord;
      }
      else
      {
        *pw++ = *pb;
      }
    }
    *pw = '\0';
    m_ODBCKeywords.push_back(woord);
    pw = woord;
  }
  // KEYWORDS reported by the RDBMS
  if(SQLGetInfo(m_hdbc, SQL_KEYWORDS, buffer, 5120, &len) == SQL_SUCCESS)
  {
    woord[0] = '\0';
    pw = woord;
    for(pb = buffer;*pb != '\0';pb++)
    {
      if (*pb == ',')
      {
        *pw = '\0';
        m_RDBMSkeywords.push_back(woord);
        pw = woord;
      }
      else
      {
        *pw++ = *pb;
      }
    }
    *pw = '\0';
    m_RDBMSkeywords.push_back(woord);
    pw = woord;
  }   
  else
  {
    m_RDBMSkeywords.push_back("(No information)");
  }
  // STRINGS
  m_manager_version     = GetInfoString(SQL_DM_VER);
  m_odbc_version        = GetInfoString(SQL_DRIVER_ODBC_VER);
  m_odbc_driver_version = GetInfoString(SQL_DRIVER_VER);
  m_rdbms_version       = GetInfoString(SQL_DBMS_VER);
  m_cli_year            = GetInfoString(SQL_XOPEN_CLI_YEAR);
  m_specialChars        = GetInfoString(SQL_SPECIAL_CHARACTERS);
  m_identifierQuote     = GetInfoString(SQL_IDENTIFIER_QUOTE_CHAR);
  m_procedureName       = GetInfoString(SQL_PROCEDURE_TERM);
  m_tableName           = GetInfoString(SQL_TABLE_TERM);
  m_schemaName          = GetInfoString(SQL_SCHEMA_TERM);
  m_catalogName         = GetInfoString(SQL_CATALOG_TERM);
  m_catalogNameSeparator= GetInfoString(SQL_CATALOG_NAME_SEPARATOR);
  m_collationSequence   = GetInfoString(SQL_COLLATION_SEQ);
  m_driverName          = GetInfoString(SQL_DRIVER_NAME);
  m_userName            = GetInfoString(SQL_USER_NAME);

  // 'Y' or 'N' strings
  m_supportsCatalogs    = (GetInfoString(SQL_CATALOG_NAME)              .GetAt(0) == 'Y');
  m_accesibleTables     = (GetInfoString(SQL_ACCESSIBLE_TABLES)         .GetAt(0) == 'Y');
  m_accesibleProcedures = (GetInfoString(SQL_ACCESSIBLE_PROCEDURES)     .GetAt(0) == 'Y');
  m_columnAliases       = (GetInfoString(SQL_COLUMN_ALIAS)              .GetAt(0) == 'Y');
  m_exprInOrderBy       = (GetInfoString(SQL_EXPRESSIONS_IN_ORDERBY)    .GetAt(0) == 'Y');
  m_orderByInSelect     = (GetInfoString(SQL_ORDER_BY_COLUMNS_IN_SELECT).GetAt(0) == 'Y');
  m_likeEscape          = (GetInfoString(SQL_LIKE_ESCAPE_CLAUSE)        .GetAt(0) == 'Y') ? "% and '_'" : "";
  m_integrity           = (GetInfoString(SQL_INTEGRITY)                 .GetAt(0) == 'Y');
  m_needLongDataLen     = (GetInfoString(SQL_NEED_LONG_DATA_LEN)        .GetAt(0) == 'Y');

  // INTEGERS
  m_sql_conformance     = GetInfoInteger(SQL_SQL_CONFORMANCE);
  m_odbc_conformance    = GetInfoInteger(SQL_ODBC_INTERFACE_CONFORMANCE);
  m_cli_conformance     = GetInfoInteger(SQL_STANDARD_CLI_CONFORMANCE);
  m_maxColumNaam        = GetInfoShortInteger(SQL_MAX_COLUMN_NAME_LEN);
  m_maxTableNaam        = GetInfoShortInteger(SQL_MAX_TABLE_NAME_LEN);
  m_maxSchemaName       = GetInfoShortInteger(SQL_MAX_SCHEMA_NAME_LEN);
  m_maxCatalogName      = GetInfoShortInteger(SQL_MAX_CATALOG_NAME_LEN);
  m_identifierCase      = GetInfoShortInteger(SQL_IDENTIFIER_CASE);
  m_fileUsage           = GetInfoShortInteger(SQL_FILE_USAGE);
  m_activeEnvironments  = GetInfoShortInteger(SQL_ACTIVE_ENVIRONMENTS);
  m_oj_cap              = GetInfoInteger(SQL_OJ_CAPABILITIES);
  m_oj_cap92            = GetInfoInteger(SQL_SQL92_RELATIONAL_JOIN_OPERATORS);
  m_txn_cap             = GetInfoShortInteger(SQL_TXN_CAPABLE);
  m_defaultTransaction  = GetInfoInteger(SQL_DEFAULT_TXN_ISOLATION);
  m_cursor_commit       = GetInfoShortInteger(SQL_CURSOR_COMMIT_BEHAVIOR);
  m_cursor_rollback     = GetInfoShortInteger(SQL_CURSOR_ROLLBACK_BEHAVIOR);
  m_correlationNames    = GetInfoShortInteger(SQL_CORRELATION_NAME);
  m_alterDomain         = GetInfoInteger(SQL_ALTER_DOMAIN);
  m_alterTable          = GetInfoInteger(SQL_ALTER_TABLE);
  m_createDomain        = GetInfoInteger(SQL_CREATE_DOMAIN);
  m_createTable         = GetInfoInteger(SQL_CREATE_TABLE);
  m_createView          = GetInfoInteger(SQL_CREATE_VIEW);
  m_createSchema        = GetInfoInteger(SQL_CREATE_SCHEMA);
  m_dropDomain          = GetInfoInteger(SQL_DROP_DOMAIN);
  m_dropView            = GetInfoInteger(SQL_DROP_VIEW);
  m_dropTable           = GetInfoInteger(SQL_DROP_TABLE);
  m_dropSchema          = GetInfoInteger(SQL_DROP_SCHEMA);
  m_grantOptions        = GetInfoInteger(SQL_SQL92_GRANT);
  m_insertOptions       = GetInfoInteger(SQL_INSERT_STATEMENT);
  m_revokeOptions       = GetInfoInteger(SQL_SQL92_REVOKE);
  m_groupBy             = GetInfoInteger(SQL_GROUP_BY);
  m_unions              = GetInfoInteger(SQL_UNION);
  m_predicates          = GetInfoInteger(SQL_SQL92_PREDICATES);
  m_catalogLocation     = GetInfoShortInteger(SQL_CATALOG_LOCATION);
  m_catalogUsage        = GetInfoInteger(SQL_CATALOG_USAGE);
  m_datetimeLiterals    = GetInfoInteger(SQL_DATETIME_LITERALS);
  m_funcNumeric         = GetInfoInteger(SQL_NUMERIC_FUNCTIONS);
  m_funcString          = GetInfoInteger(SQL_STRING_FUNCTIONS);
  m_funcString3         = GetInfoInteger(SQL_SQL92_STRING_FUNCTIONS);
  m_funcDateTime        = GetInfoInteger(SQL_TIMEDATE_FUNCTIONS);
  m_funcSystem          = GetInfoInteger(SQL_SYSTEM_FUNCTIONS);
  m_schemaViews         = GetInfoInteger(SQL_INFO_SCHEMA_VIEWS);
  m_maxColSelect        = GetInfoShortInteger(SQL_MAX_COLUMNS_IN_SELECT);
  m_maxColGroupBy       = GetInfoShortInteger(SQL_MAX_COLUMNS_IN_GROUP_BY);
  m_maxColOrderBy       = GetInfoShortInteger(SQL_MAX_COLUMNS_IN_ORDER_BY);
  m_maxColTable         = GetInfoShortInteger(SQL_MAX_COLUMNS_IN_TABLE);
  m_maxColIndex         = GetInfoShortInteger(SQL_MAX_COLUMNS_IN_INDEX);
  m_maxTabSelect        = GetInfoShortInteger(SQL_MAX_TABLES_IN_SELECT);
  m_valExpressions      = GetInfoInteger(SQL_SQL92_VALUE_EXPRESSIONS);
  m_predSubqueries      = GetInfoInteger(SQL_SUBQUERIES);
  m_concatBehaviour     = GetInfoShortInteger(SQL_CONCAT_NULL_BEHAVIOR);
  m_rowConstructors     = GetInfoInteger(SQL_SQL92_ROW_VALUE_CONSTRUCTOR);
  m_nullCollation       = GetInfoShortInteger(SQL_NULL_COLLATION);
  m_nullableColumns     = GetInfoShortInteger(SQL_NON_NULLABLE_COLUMNS);
  m_deleteRules         = GetInfoInteger(SQL_SQL92_FOREIGN_KEY_DELETE_RULE);
  m_updateRules         = GetInfoInteger(SQL_SQL92_FOREIGN_KEY_UPDATE_RULE);
  m_funcTimestampadd    = GetInfoInteger(SQL_TIMEDATE_ADD_INTERVALS);
  m_funcTimestampdiff   = GetInfoInteger(SQL_TIMEDATE_DIFF_INTERVALS);
  m_getdata_extensions  = GetInfoInteger(SQL_GETDATA_EXTENSIONS);

  // Aggregate functions
  m_aggr_functions      = GetInfoInteger(SQL_AGGREGATE_FUNCTIONS);
  if(len && m_aggr_functions == SQL_AF_ALL)
  {
    // Just in case the ODBC driver reports ALL but none of the others
    m_aggr_functions |= SQL_AF_AVG|SQL_AF_SUM|SQL_AF_DISTINCT|
                        SQL_AF_MIN|SQL_AF_MAX|SQL_AF_COUNT;
  }
  // Indexing
  m_indexOptions1 = GetInfoInteger(SQL_DDL_INDEX);
  m_indexOptions2 = GetInfoInteger(SQL_INDEX_KEYWORDS);
  if(len && m_indexOptions2 == SQL_IK_ALL)
  {
    // Just in case the ODBC driver reports ALL but none of the others
    m_indexOptions2 |= SQL_IK_ASC | SQL_IK_DESC;
  }
  // Datatype conversions
  m_conversionFuncs   = GetInfoInteger(SQL_CONVERT_FUNCTIONS);
  m_convertBigint     = GetInfoInteger(SQL_CONVERT_BIGINT);
  m_convertBinary     = GetInfoInteger(SQL_CONVERT_BINARY);
  m_convertBit        = GetInfoInteger(SQL_CONVERT_BIT);
  m_convertChar       = GetInfoInteger(SQL_CONVERT_CHAR);
  m_convertDate       = GetInfoInteger(SQL_CONVERT_DATE);
  m_convertDecimal    = GetInfoInteger(SQL_CONVERT_DECIMAL);
  m_convertDouble     = GetInfoInteger(SQL_CONVERT_DOUBLE);
  m_convertFloat      = GetInfoInteger(SQL_CONVERT_FLOAT);
  m_convertInteger    = GetInfoInteger(SQL_CONVERT_INTEGER);
  m_convertIYM        = GetInfoInteger(SQL_CONVERT_INTERVAL_YEAR_MONTH);
  m_convertIDT        = GetInfoInteger(SQL_CONVERT_INTERVAL_DAY_TIME);
  m_convertLVarBinary = GetInfoInteger(SQL_CONVERT_LONGVARBINARY);
  m_convertLVarchar   = GetInfoInteger(SQL_CONVERT_LONGVARCHAR);
  m_convertNumeric    = GetInfoInteger(SQL_CONVERT_NUMERIC);
  m_convertReal       = GetInfoInteger(SQL_CONVERT_REAL);
  m_convertSmallint   = GetInfoInteger(SQL_CONVERT_SMALLINT);
  m_convertTime       = GetInfoInteger(SQL_CONVERT_TIME);
  m_convertTimestamp  = GetInfoInteger(SQL_CONVERT_TIMESTAMP);
  m_convertTinyint    = GetInfoInteger(SQL_CONVERT_TINYINT);
  m_convertVarBinary  = GetInfoInteger(SQL_CONVERT_VARBINARY);
  m_convertVarchar    = GetInfoInteger(SQL_CONVERT_VARCHAR);

  ReadingDataTypes();
  m_initDone = true;
}

void
SQLInfo::ReadingDataTypes()
{
  // Get all datatypes from this ODBC datasource in m_dataTypes
  if(!SupportedFunction(SQL_API_SQLGETTYPEINFO))
  {
    // Cannot get datatype info from this ODBC driver
    return;
  }
  SQLHSTMT handle;
  SQLLEN   dataLen;
  char     buffer[5120];

  m_retCode = SqlAllocHandle(SQL_HANDLE_STMT,m_hdbc,&handle);
  if (m_retCode == SQL_SUCCESS )
  {
    m_retCode = SqlGetTypeInfo(handle,SQL_ALL_TYPES );
    if (m_retCode == SQL_SUCCESS )
    {
      while(true)
      {
        m_retCode = SqlFetch(handle);
        if(m_retCode != SQL_SUCCESS)
        {
          break;
        }
        int num = 1;
        CString key;
        TypeInfo* ti = new TypeInfo();
        dataLen =0;

        // DATA SOURCE DEPENDENT TYPE NAME. USE FOR CREATE TABLE
        if(::SQLGetData(handle,1,SQL_C_CHAR,buffer,5120,&dataLen) == SQL_SUCCESS)
        {
          if(dataLen > 0) 
          {
            ti->m_type_name = buffer;
            key = buffer;
          }
        }
        if(!dataLen)
        {
          // Cannot get the type name for this type. Reject it
          continue;
        }
        while(m_dataTypes.find(key) != m_dataTypes.end())
        {
          // Prefix duplicated datatypes with a number
          key.Format("%d: %s",++num,buffer);
        }
        // Put in m_datatypes
        m_dataTypes.insert(std::make_pair(key,ti));

        // SQL_<*> TYPE NUMBER
        if(::SQLGetData(handle,2,SQL_C_SSHORT,&ti->m_data_type,0,&dataLen) == SQL_SUCCESS)
        {
          if(dataLen != 2) ti->m_data_type = 0;
        }
        // ODBC COLUMN SIZE
        if(::SQLGetData(handle,3,SQL_C_LONG,&ti->m_precision,0,&dataLen) == SQL_SUCCESS)
        {
          if(dataLen != 4) ti->m_precision = 0;
        }
        // LITERAL PREFIX FOR ODBC DRIVER, like {ts' for timestamp
        if(::SQLGetData(handle,4,SQL_C_CHAR,buffer,5120,&dataLen) == SQL_SUCCESS)
        {
          if(dataLen > 0) ti->m_literal_prefix = buffer;
        }
        // LITERAL SUFFIX FOR ODBC DRIVER, like '} for timestamp
        if(::SQLGetData(handle,5,SQL_C_CHAR,buffer,5120,&dataLen) == SQL_SUCCESS)
        {
          if(dataLen > 0) ti->m_literal_suffix = buffer;
        }
        // HOW TO CREATE PARAMETERS, like "(precision,scale)"
        if(::SQLGetData(handle,6,SQL_C_CHAR,buffer,5120,&dataLen) == SQL_SUCCESS)
        {
          if(dataLen > 0) ti->m_create_params = buffer;
        }
        // Nullable: SQL_NO_NULLS, SQL_NULLABLE, SQL_NULLABLE_UNKNOWN
        ti->m_nullable = 0;
        if(::SQLGetData(handle,7,SQL_C_SSHORT,&ti->m_nullable,0,&dataLen) == SQL_SUCCESS)
        {
          if(dataLen != 2) ti->m_nullable = 0;
        }
        // Case sensitive search in datatype (TRUE/FALSE)
        ti->m_case_sensitive = 0;
        if(::SQLGetData(handle,8,SQL_C_SSHORT,&ti->m_case_sensitive,0,&dataLen) == SQL_SUCCESS)
        {
          if(dataLen != 2) ti->m_case_sensitive = 0;
        }
        // Searchable in where clause
        // SQL_PRED_NONE (SQL_UNSEARCHABLE), SQL_PRED_CHAR, SQL_PRED_BASIC, SQL_SEARCHABLE
        ti->m_searchable = 0;
        if(::SQLGetData(handle,9,SQL_C_SSHORT,&ti->m_searchable,0,&dataLen) == SQL_SUCCESS)
        {
          if(dataLen != 2) ti->m_searchable = 0;
        }
        // UNSIGNED (TRUE, FALSE)
        ti->m_unsigned = 0;
        if(::SQLGetData(handle,10,SQL_C_SSHORT,&ti->m_unsigned,0,&dataLen) == SQL_SUCCESS)
        {
          if(dataLen != 2) ti->m_unsigned = 0;
        }
        // FIXED PRECISION SCALE (LIKE MONEY)
        ti->m_money = 0;
        if(::SQLGetData(handle,11,SQL_C_SSHORT,&ti->m_money,0,&dataLen) == SQL_SUCCESS)
        {
          if(dataLen != 2) ti->m_money = 0;
        }
        // Auto increment type like SERIAL (TRUE, FALSE, NULL for non-numeric)
        ti->m_autoincrement = 0;
        if(::SQLGetData(handle,12,SQL_C_SSHORT,&ti->m_autoincrement,0,&dataLen) == SQL_SUCCESS)
        {
          if(dataLen != 2) ti->m_autoincrement = 0;
        }
        // Local type name for display on UI's (not in DDL!)
        ti->m_local_type_name = buffer;
        if(::SQLGetData(handle,13,SQL_C_CHAR,buffer,5120,&dataLen) == SQL_SUCCESS)
        {
          if(dataLen > 0) ti->m_local_type_name = buffer;
        }
        // Minimum scale of the datatype (e.g. in timestamps)
        ti->m_minimum_scale = 0;
        if(::SQLGetData(handle,14,SQL_C_SSHORT,&ti->m_minimum_scale,0,&dataLen) == SQL_SUCCESS)
        {
          if(dataLen != 2) ti->m_minimum_scale = 0;
        }
        // Maximum scale of the datatype. If 0 or NULL use column_size
        ti->m_maximum_scale = 0;
        if(::SQLGetData(handle,15,SQL_C_SSHORT,&ti->m_maximum_scale,0,&dataLen) == SQL_SUCCESS)
        {
          if(dataLen != 2) ti->m_maximum_scale = 0;
        }
        // Driver independent SQL datatype
        ti->m_sqlDatatype = 0;
        if(::SQLGetData(handle,16,SQL_C_SSHORT,&ti->m_sqlDatatype,0,&dataLen) == SQL_SUCCESS)
        {
          if(dataLen != 2) ti->m_sqlDatatype = 0;
        }
        // SQL subtype for TYPE_TIMESTAMP and INTERVAL types
        ti->m_sqlSubType = 0;
        if(::SQLGetData(handle,17,SQL_C_SSHORT,&ti->m_sqlSubType,0,&dataLen) == SQL_SUCCESS)
        {
          if(dataLen != 2) ti->m_sqlSubType = 0;
        }
        // Decimal radix (2,10 or NULL)
        ti->m_radix = 0;
        if(::SQLGetData(handle,18,SQL_C_SLONG,&ti->m_radix,0,&dataLen) == SQL_SUCCESS)
        {
          if(dataLen != 4) ti->m_radix = 0;
        }
        // Number of decimals in interval precision of leading type
        ti->m_interval_precision = 0;
        if(::SQLGetData(handle,19,SQL_C_SSHORT,&ti->m_interval_precision,0,&dataLen) == SQL_SUCCESS)
        {
          if(dataLen != 2) ti->m_interval_precision = 0;
        }
      }
    }
    SqlFreeHandle(SQL_HANDLE_STMT,handle);
  }
}

// Getting datatype info
TypeInfo* 
SQLInfo::GetTypeInfo(int p_sqlDatatype)
{
  DataTypeMap::iterator it;
  
  for(it = m_dataTypes.begin();it != m_dataTypes.end();++it)
  {
    if(it->second->m_data_type == p_sqlDatatype)
    {
      return it->second;
    }
  }
  return nullptr;
}

// Returns the fact whether an API function is supported
// by the ODBC driver, regardless of ODBC version
bool
SQLInfo::SupportedFunction(unsigned int api_function)
{
  if(m_functions_use_3)
  {
    // IF ODBC 3.x Standard
    if(api_function < 0 || api_function > 4000)
    {
      return false;
    }
    if(SQL_FUNC_EXISTS(m_ODBCFunctions_3,api_function))
    {
      return true;
    }
    return false;
  }
  else
  {
    // If ODBC 1.x / 2.x Standard
    if(api_function < 0 || api_function > SQL_API_SQLBINDPARAMETER)
    {
      return false;
    }
    if(m_ODBCFunctions_2[api_function])
    {
      return true;
    }
  }
  return false;
}

//////////////////////////////////////////////////////////////////////////
//
// SETTING AND RETRIEVING ATTRIBUTES OF THE CONNECTION
//
//////////////////////////////////////////////////////////////////////////

// Getting a general INTEGER connection attribute
int
SQLInfo::GetAttributeInteger(CString description,SQLINTEGER attrib)
{
  SQLUINTEGER value;
  RETCODE nRetCode = SQL_ERROR;
  SQLINTEGER cbMax = 0;
  nRetCode = SQLGetConnectAttr(m_hdbc
                              ,attrib
                              ,(SQLPOINTER)&value
                              ,0
                              ,&cbMax);
  if(!m_database->Check(nRetCode))
  {
    CString error;
    error.Format("Cannot get connection attribute \"%s\": ",description);
    error += m_database->GetErrorString(NULL);
    CString state = m_database->GetSQLState();
    if(state.CompareNoCase("S1C00") == 0 || // Driver not capable
       state.CompareNoCase("S1092") == 0 || // Option-id not supported on your database
       state.CompareNoCase("HYC00") == 0 )  // Optional feature not implemented
    {
      // Driver not capable to get/set this attribute
      TRACE("%s\n",error);
      return -1;
    }
    AfxMessageBox(error);
    return 0;
  }
  if(cbMax != 4)
  {
    TRACE("Attribute \"%s\" not supported on your database\n",description);
  }
  TRACE("Database connection attribute \"%s\" was: %d\n",description,value);
  return value;
}

// Getting a general STRING connection attribute
CString
SQLInfo::GetAttributeString(CString description,SQLINTEGER attrib)
{
  SQLCHAR    value[512 + 1];
  SQLINTEGER cbMax = 0;
  RETCODE    nRetCode = SQL_ERROR;
  nRetCode = ::SQLGetConnectAttr(m_hdbc
                                ,attrib
                                ,(SQLPOINTER)&value
                                ,512
                                ,&cbMax);
  if(!m_database->Check(nRetCode))
  {
    CString error;
    error.Format("Cannot get connection attribute \"%s\": ",description);
    error += m_database->GetErrorString(NULL);
    AfxMessageBox(error);
    return "";
  }
  value[cbMax] = 0;
  TRACE("Database connection attribute \"%s\" was: %s\n",description,value);
  return CString(value);
}

// Setting an INTEGER attribute
bool 
SQLInfo::SetAttributeInteger(CString     description
                            ,SQLINTEGER  attrib
                            ,SQLUINTEGER value)
{
  RETCODE nRetCode = SQL_ERROR;
  nRetCode = ::SQLSetConnectAttr(m_hdbc
                                ,attrib
                                ,(SQLPOINTER)value
                                ,SQL_IS_UINTEGER);
  if(!m_database->Check(nRetCode))
  {
    CString error;
    error.Format("Cannot set connection attribute \"%s\": ",description);
    error += m_database->GetErrorString(NULL);
    AfxMessageBox(error);
    return false;
  }
  TRACE("Database connection attribute \"%s\" set to: %d\n",description,value);
  return true;
}

// Setting a STRING attribute
bool 
SQLInfo::SetAttributeString(CString    description
                           ,SQLINTEGER attrib
                           ,SQLCHAR*   value)
{
  RETCODE nRetCode = SQL_ERROR;
  nRetCode = ::SQLSetConnectAttr(m_hdbc
                                ,attrib
                                ,(SQLPOINTER)value
                                ,SQL_NTS);
  if(!m_database->Check(nRetCode))
  {
    CString error;
    error.Format("Cannot set connection attribute \"%s\": ",description);
    error += m_database->GetErrorString(NULL);
    AfxMessageBox(error);
    return false;
  }
  TRACE("Database connection attribute \"%s\" set to: %s\n",description,value);
  return true;
}

// Retrieve the fact that we are living or dead
bool
SQLInfo::GetConnectionDead()
{
  int dead = GetAttributeInteger("dead-connection",SQL_ATTR_CONNECTION_DEAD);
  return (bool)(dead == SQL_CD_TRUE);
}

// Connection in quiet mode
int
SQLInfo::GetAttributeQuiet()
{
  int hand = GetAttributeInteger("quiet-mode",SQL_ATTR_QUIET_MODE);
  return (hand == 0);
}

// Automatic population of the IPD, so driver supports prepared statements
int
SQLInfo::GetAttributeAutoIPD()
{
  return GetAttributeInteger("auto-ipd",SQL_ATTR_AUTO_IPD);
}

// ODBC Tracing is on or off
bool
SQLInfo::GetAttributeTracing()
{
  int trace = GetAttributeInteger("tracing",SQL_ATTR_TRACE);
  return (bool)(trace == SQL_OPT_TRACE_ON);
}

// The file to which we are tracing
CString       
SQLInfo::GetAttributeTraceFile()
{
  return GetAttributeString("tracefile",SQL_ATTR_TRACEFILE);
}

// Getting the current catalog name
CString       
SQLInfo::GetAttributeCatalog()
{
  CString catalog = GetAttributeString("current-catalog",SQL_ATTR_CURRENT_CATALOG);
  if(catalog.IsEmpty())
  {
    // Does not supports catalogs. Use database name
    return m_database->GetDatabaseName();
  }
  return catalog;
}

// Getting the IP packet size
int
SQLInfo::GetAttributePacketSize()
{
  m_packetSize = GetAttributeInteger("packet-size",SQL_ATTR_PACKET_SIZE);
  return m_packetSize;
}

// METADATA ID is supported?
bool    
SQLInfo::GetAttributeMetadataID()
{
  m_metadataID = GetAttributeInteger("metadata ID",SQL_ATTR_METADATA_ID) != 0;
  return m_metadataID;
}

// The current transaction level
int
SQLInfo::GetAttributeTransLevel()
{
  m_txnLevel = GetAttributeInteger("txn-isolation-level",SQL_ATTR_TXN_ISOLATION);
  return m_txnLevel;
}

// Timeout for the whole connection
int  
SQLInfo::GetAttributeConnTimeout()
{
  m_connTimeout = GetAttributeInteger("connection-timeout",SQL_ATTR_CONNECTION_TIMEOUT);
  return m_connTimeout;
}

// The translation library path
CString
SQLInfo::GetAttributeTranslib()
{
  m_transLib = GetAttributeString("translation-library",SQL_ATTR_TRANSLATE_LIB);
  return m_transLib;
}

// Translation option of the translation library
int
SQLInfo::GetAttributeTransoption()
{
  m_transOption = GetAttributeInteger("translation-option",SQL_ATTR_TRANSLATE_OPTION);
  return m_transOption;
}

// Set the using of ODBC cursors
bool 
SQLInfo::SetAttributeOdbcCursors(int p_cursors)
{
  m_odbcCursors = p_cursors;
  return SetAttributeInteger("use-odbc-cursors",SQL_ATTR_ODBC_CURSORS,p_cursors);
}

// Setting the optimal IP Packet size
bool
SQLInfo::SetAttributePacketSize(int p_packet)
{
  m_packetSize = p_packet;
  return SetAttributeInteger("packet-size",SQL_ATTR_PACKET_SIZE,p_packet);
}

// Setting the ODBC Tracing file
bool 
SQLInfo::SetAttributeTraceFile(CString p_traceFile)
{
  SQLCHAR traceFile[512 + 1];
  SQLINTEGER cbMax = p_traceFile.GetLength();
  strncpy_s((char *)traceFile,512,p_traceFile.GetString(),cbMax);

  return SetAttributeString("tracefile",SQL_ATTR_TRACEFILE,traceFile);
}

// Set tracing on or off
bool
SQLInfo::SetAttributeTracing(bool p_tracing)
{
  SQLUINTEGER tracing = p_tracing ? SQL_OPT_TRACE_ON : SQL_OPT_TRACE_OFF;
  return SetAttributeInteger("tracing",SQL_ATTR_TRACE,tracing);
}

// Setting the automatic connection timeout (if supported)
bool
SQLInfo::SetAttributeConnTimeout(int p_timeout)
{
  m_connTimeout = p_timeout;
  return SetAttributeInteger("connection-timeout",SQL_ATTR_CONNECTION_TIMEOUT,p_timeout);
}

// Setting the METADATA-ID of the connection
bool 
SQLInfo::SetAttributeMetadataID(bool p_metadata)
{
  m_metadataID = p_metadata;
  return SetAttributeInteger("metadata ID",SQL_ATTR_METADATA_ID,p_metadata);
}

// Setting the transaction isolation level
bool 
SQLInfo::SetAttributeTransLevel(int p_txnlevel)
{
  m_txnLevel = p_txnlevel;
  return SetAttributeInteger("txn-isolation-level",SQL_ATTR_TXN_ISOLATION,p_txnlevel);
}

// Setting the transaction library (with or without connection)
bool
SQLInfo::SetAttributeTranslib(CString p_transLib)
{
  m_transLib = p_transLib;
  if(m_hdbc)
  {
    return SetAttributeString("translation-library",SQL_ATTR_TRANSLATE_LIB,(SQLCHAR *)p_transLib.GetString());
  }
  return true;
}

// Setting the translation optioen (with or without connection)
bool
SQLInfo::SetAttributeTransoption(int p_transOption)
{
  m_transOption = p_transOption;
  if(m_hdbc)
  {
    return SetAttributeInteger("translation-option",SQL_ATTR_TRANSLATE_OPTION,p_transOption);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////
//
// GETTING SCHEMA META DATA FROM THE DATABASE
//
//////////////////////////////////////////////////////////////////////////

// Is it a correct identifier (type 0=table,1=column)
bool 
SQLInfo::IsCorrectName(LPCSTR naam,int type)
{
  if (!naam)
  {
    return true;
  }
  int nlen = (int)strlen(naam);
  if (nlen == 0)
  {
    return true;
  }
  if (type == 0 && nlen > (m_maxColumNaam - 4))
  {
    return false;
  }
  if (type == 0 && nlen > (m_maxTableNaam - 4))
  {
    return false;
  }
  LPCSTR p;
  for(p = naam; *p != '\0'; p++)
  {
    if ( *p == ' ' || ( !isalnum(*p) && *p != '_' && strchr(m_specialChars,*p) == NULL ))
    {
      return false;
    }
  }
  for(WordList::iterator it = m_ODBCKeywords.begin(); it != m_ODBCKeywords.end(); ++it)
  {
    if((*it).CompareNoCase(naam) == 0)
    {
      return false;
    }
  }
  for(WordList::iterator it = m_RDBMSkeywords.begin(); it != m_RDBMSkeywords.end(); ++it)
  {
    if((*it).CompareNoCase(naam) == 0)
    {
      return false;
    }
  }
  return true;
}

// Can we start a transaction on the database
bool
SQLInfo::CanStartTransaction()
{
  if (m_cursor_rollback != SQL_CB_PRESERVE ||
        m_cursor_commit != SQL_CB_PRESERVE )
  {
    return false;
  }
  return true;
}

// Get information about the primary key of a table
bool 
SQLInfo::GetPrimaryKeyInfo(CString&    p_tablename
                          ,CString&    p_primary
                          ,PrimaryMap& p_keymap)
{
  // Make sure we have the info
  GetInfo();

  // Reset
  p_primary = "";
  p_keymap.clear();

  if(!MakeInfoTableTablepart(NULL,NULL,p_tablename))
  {
    // Table could not be found
    return false;
  }
  MakeInfoTablePrimary(NULL,p_primary,p_keymap);
  if(!p_keymap.size())
  {
    // If no primary key found, search for the first unique key
    MakeInfoTableStatistics(NULL,p_primary,p_keymap);
  }
  return (p_keymap.size() > 0);
}

// Make a statement handle for METADATA query
bool
SQLInfo::GetStatement(bool p_metadataID /*= true*/)
{
  // Opvragen statement handle in m_hstmt:
  m_retCode = m_database->GetSQLHandle(&m_hstmt,FALSE);
  if (!SQL_SUCCEEDED(m_retCode))
  {
    CString errorText = "Error in ODBC statement: ";
    errorText += m_database->GetErrorString(m_hstmt);
    throw errorText;
  }
  TRACE("DBInfo::GetStatement\n");

  SQLUINTEGER meta = p_metadataID ? SQL_TRUE : SQL_FALSE;
  // On Various ODBC databases metadata is or is not case-sensitive. To work around
  // these differences, the statement should be aware that it is about metadata!
  m_retCode = SqlSetStmtAttr(m_hstmt,SQL_ATTR_METADATA_ID,(SQLPOINTER)meta,SQL_IS_UINTEGER);
  if(!SQL_SUCCEEDED(m_retCode))
  {
    m_retCode = SqlSetConnectAttr(m_hdbc,SQL_ATTR_METADATA_ID,(SQLPOINTER)meta,SQL_IS_UINTEGER);
    if(!SQL_SUCCEEDED(m_retCode))
    {
      // OOPS Cannot set the METADATA attribute.
      m_METADATA_ID_unsupported = true;
      return false;
    }
  }
  return p_metadataID;
}

// Close the internal statement handle
void
SQLInfo::CloseStatement()
{
  if (m_hstmt)
  {
    SqlCancel(m_hstmt);
    SqlFreeStmt(m_hstmt,SQL_DROP);
    m_hstmt   = NULL;
    m_retCode = SQL_SUCCESS;

    TRACE("DBInfo::CloseStatement\n");
  }
}

// Get the catalog.schema.table from a user string
void
SQLInfo::GetObjectName(CString& pattern
                      ,unsigned char* search_catalog
                      ,unsigned char* search_schema
                      ,unsigned char* search_table
                      ,unsigned char* search_type)
{
  search_catalog[0] = 0;
  search_schema [0] = 0;
  search_table  [0] = 0;
  search_type   [0] = 0;

  int pos = 0;
  CString name = pattern;

  // Search for type
  pos = name.Find(':');
  if(pos > 0)
  {
    bool filled = false;
    CString type = name.Left(pos);
    name = name.Right(name.GetLength()-pos-1);
    while(!type.IsEmpty())
    {
      // Search for type names
      if(type.Left(1).CompareNoCase("T") == 0)
      {
        type = type.Mid(1);
        if(filled) strcat((char*)search_type,",");
        strcat((char*)search_type,"TABLE");
        filled = true;
      }
      else if(type.Left(1).CompareNoCase("V") == 0)
      {
        type = type.Mid(1);
        if(filled) strcat((char*)search_type,",");
        strcat((char*)search_type,"VIEW");
        filled = true;
      }
      else if(type.Left(1).CompareNoCase("C") == 0)
      {
        type = type.Mid(1);
        if(filled) strcat((char*)search_type,",");
        strcat((char*)search_type,"SYSTEM TABLE");
        filled = true;
      }
      else if(type.Left(1).CompareNoCase("G") == 0)
      {
        type = type.Mid(1);
        if(filled) strcat((char*)search_type,",");
        strcat((char*)search_type,"GLOBAL TEMPORARY");
        filled = true;
      }
      else if(type.Left(1).CompareNoCase("L") == 0)
      {
        type = type.Mid(1);
        if(filled) strcat((char*)search_type,",");
        strcat((char*)search_type,"LOCAL TEMPORARY");
        filled = true;
      }
      else if(type.Left(1).CompareNoCase("A") == 0)
      {
        type = type.Mid(1);
        if(filled) strcat((char*)search_type,",");
        strcat((char*)search_type,"ALIAS");
        filled = true;
      }
      else if(type.Left(1).CompareNoCase("S") == 0)
      {
        type = type.Mid(1);
        if(filled) strcat((char*)search_type,",");
        strcat((char*)search_type,"SYNONYM");
        filled = true;
      }
      else
      {
        AfxMessageBox(CString("Unknown catalog type: ") + type.GetAt(1),MB_OK|MB_ICONEXCLAMATION);
        type = type.Mid(1);
      }
    }
  }
  // Search for tablename
  pos = name.ReverseFind('.');
  if(pos < 0)
  {
    // Only a table name
    strcpy((char *)search_table,name.GetString());
  }
  else
  {
    strcpy((char *)search_table,name.Right(name.GetLength()-pos-1).GetString());
    CString qualifier = name.Left(pos);
    pos = qualifier.ReverseFind('.');
    if(pos < 0)
    {
      // Only a schema name
      strcpy((char *)search_schema,qualifier.GetString());
    }
    else
    {
      // Split in catalog / schema
      strcpy((char*)search_schema,qualifier.Right(qualifier.GetLength()-pos-1).GetString());
      strcpy((char*)search_catalog,qualifier.Left(pos));
    }
  }
  if(m_maxCatalogName && strlen((char*)search_catalog) > (size_t)m_maxCatalogName)
  {
    AfxMessageBox("Requested catalog name is longer than this ODBC database supports!",MB_OK);
  }
  if(m_maxSchemaName  && strlen((char*)search_schema)  > (size_t)m_maxSchemaName)
  {
    AfxMessageBox("Requested schema name is longer than this ODBC database supports!",MB_OK);
  }
  if(m_maxTableNaam   && strlen((char*)search_table)   > (size_t)m_maxTableNaam)
  {
    AfxMessageBox("Requested table name is longer than this ODBC database supports!",MB_OK);
  }
}

// Reprint the catalog.schema.table combination
CString
SQLInfo::MakeObjectName(SQLCHAR* search_catalog
                       ,SQLCHAR* search_schema
                       ,SQLCHAR* search_table
                       ,SQLCHAR* search_type)
{
  CString objectName;

  if(strlen((char*)search_schema))
  {
    objectName += CString(search_schema);
    objectName += ".";
  }
  if(strlen((char*)search_table))
  {
    objectName += CString(search_table);
  }
  if(strlen((char*)search_catalog))
  {
    CString separator = m_catalogNameSeparator;
    if(separator.IsEmpty())
    {
      separator = ":"; // ANSI separator
    }
    if(m_catalogLocation == SQL_CL_END)
    {
      objectName += separator + CString(search_catalog);
    }
    else // m_catalogLocation == SQL_CL_START
    {
      objectName = CString(search_catalog) + separator + objectName;
    }
  }
  if(strlen((char*)search_type))
  {
    objectName = CString(search_type) + ": " + objectName;
  }
  return objectName;
}

CString
SQLInfo::ODBCDataType(int DataType)
{
  CString type;

  switch(DataType)
  {
    case SQL_CHAR:                      type = "CHAR";          break;
    case SQL_VARCHAR:                   type = "VARCHAR";       break;
    case SQL_LONGVARCHAR:               type = "LONGVARCHAR";   break;
    case SQL_WCHAR:                     type = "WCHAR";         break;
    case SQL_WVARCHAR:                  type = "WVARCHAR";      break;
    case SQL_WLONGVARCHAR:              type = "WLONGVARCHAR";  break;
    case SQL_NUMERIC:                   type = "NUMERIC";       break;
    case SQL_DECIMAL:                   type = "DECIMAL";       break;
    case SQL_INTEGER:                   type = "INTEGER";       break;
    case SQL_SMALLINT:                  type = "SMALLINT";      break;
    case SQL_FLOAT:                     type = "FLOAT";         break;
    case SQL_REAL:                      type = "REAL";          break;
    case SQL_DOUBLE:                    type = "DOUBLE";        break;
    //case SQL_DATE:
    case SQL_DATETIME:                  type = "DATETIME";      break;
    case SQL_TYPE_DATE:                 type = "TYPE DATE";     break;
    case SQL_TIME:                      type = "TIME";          break;
    case SQL_TYPE_TIME:                 type = "TYPE TIME";     break;
    case SQL_TIMESTAMP:                 type = "TIMESTAMP";     break;
    case SQL_TYPE_TIMESTAMP:            type = "TYPE TIMESTAMP";break;
    case SQL_BINARY:                    type = "BINARY";        break;
    case SQL_VARBINARY:                 type = "VARBINARY";     break;
    case SQL_LONGVARBINARY:             type = "LONGVARBINARY"; break;
    case SQL_BIGINT:                    type = "BIGINT";        break;
    case SQL_TINYINT:                   type = "TINYINT";       break;
    case SQL_BIT:                       type = "BIT";           break;
    case SQL_GUID:                      type = "GUID";          break;
    case SQL_INTERVAL_YEAR:             type = "INTERVAL YEAR"; break;
    case SQL_INTERVAL_MONTH:            type = "INTERVAL MONTH";break;
    case SQL_INTERVAL_DAY:              type = "INTERVAL DAY";  break;
    case SQL_INTERVAL_HOUR:             type = "INTERVAL HOUR"; break;
    case SQL_INTERVAL_MINUTE:           type = "INTERVAL MINUTE";          break;
    case SQL_INTERVAL_SECOND:           type = "INTERVAL SECOND";          break;
    case SQL_INTERVAL_YEAR_TO_MONTH:    type = "INTERVAL YEAR TO MONTH";   break;
    case SQL_INTERVAL_DAY_TO_HOUR:      type = "INTERVAL DAY TO HOUR";     break;
    case SQL_INTERVAL_DAY_TO_MINUTE:    type = "INTERVAL DAY TO MINUTE";   break;
    case SQL_INTERVAL_DAY_TO_SECOND:    type = "INTERVAL DAY TO SECOND";   break;
    case SQL_INTERVAL_HOUR_TO_MINUTE:   type = "INTERVAL HOUR TO MINUTE";  break;
    case SQL_INTERVAL_HOUR_TO_SECOND:   type = "INTERVAL HOUR TO SECOND";  break;
    case SQL_INTERVAL_MINUTE_TO_SECOND: type = "INTERVAL MINUTE TO SECOND";break;
    case SQL_UNKNOWN_TYPE:
    default:                            type = "UNKNOWN ODBC DATA TYPE!";  break;
  }
  return type;
}

// GETTING ALL THE TABLES OF A NAME PATTERN
// GETTING ALL THE INFO FOR ONE TABLE
bool
SQLInfo::MakeInfoTableTablepart(WordList* p_list,WordList* p_tables,CString& p_findTable)
{
  bool         retValue      = true;
  SQLCHAR      szCatalogName [SQL_MAX_BUFFER];
  SQLLEN       cbCatalogName = 0;
  SQLCHAR      szSchemaName  [SQL_MAX_BUFFER];
  SQLLEN       cbSchemaName  = 0;
  SQLCHAR      szTableName   [SQL_MAX_BUFFER];
  SQLLEN       cbTableName   = 0;
  SQLCHAR      szTableType   [SQL_MAX_BUFFER];
  SQLLEN       cbTableType   = 0;
  SQLCHAR      szRemarks     [2 * SQL_MAX_BUFFER];
  SQLLEN       cbRemarks     = 0;
  // Where to search
  unsigned char search_catalog[SQL_MAX_BUFFER];
  unsigned char search_schema [SQL_MAX_BUFFER];
  unsigned char search_table  [SQL_MAX_BUFFER];
  unsigned char search_type   [SQL_MAX_BUFFER];

  // Check whether we can do this
  if(!SupportedFunction(SQL_API_SQLTABLES))
  {
    if(p_list)
    {
      p_list->push_back("SQLTables unsupported. Get a better ODBC driver!");
    }
    return false;
  }
  // Get a statement handle for metadata use
  CloseStatement();
  bool meta = GetStatement(false);

  if(m_METADATA_ID_unsupported || !meta)
  {
    // Oops: Cannot search directly on table name!
    switch(m_identifierCase)
    {
      case SQL_IC_UPPER:     p_findTable.MakeUpper(); break;  // e.g. Oracle / DB2
      case SQL_IC_LOWER:     p_findTable.MakeLower(); break;  // e.g. Informix
      case SQL_IC_SENSITIVE: // Nothing to be done, Catalog is treated case-insensitive
      case SQL_IC_MIXED:     // but is fysically stored case-sensitive only
                             // e.g. MS-SQLServer / MS-Access / PostgreSQL / mySQL
      default:               if(m_METADATA_ID_unsupported && (m_METADATA_ID_errorseen == false))
                             {
                               AfxMessageBox("Cannot garantee to find object '" + p_findTable + "' for one of the following reasons:\r\n"
                                             "- The usage of SQL_ATTR_METADATA_ID is not supported on the statement level\r\n"
                                             "- The usage of SQL_ATTR_METADATA_ID is not supported on the connection level\r\n"
                                             "- SQLInfo of catalog identifiers is not simply SQL_IC_UPPER or SQL_IC_LOWER\r\n"
                                             "  and the catalog is not treated in a case-insensitive way."
                                            ,MB_OK);
                               m_METADATA_ID_errorseen = true;
                             }
                             break;
    }
  }
  // Split name in a maximum of three parts
  GetObjectName(p_findTable,search_catalog,search_schema,search_table,search_type);

  // Reset tablesearch
  bool foundOneTable = false;
  m_searchCatalogName = "";
  m_searchSchemaName  = "";
  m_searchTableName   = "";
  m_searchTableType   = "";

  // Have care: Empty strings denotes a special case
  // - Empty strings for a search catalog means tables with no catalog in an 
  //   environment with catalogs. So not searching on catalogs means we must provide a NULL pointer
  // - Empty strings for a schema name means tables with no schema/owner in an
  //   environment with schema's (most multi-tier database). So not searching on
  //   owner means we must provide a NULL pointer
  // - If the driver cannot search on this type of META-object the pointer MUST be NULL

  unsigned char* catalog = GetMetaPointer(search_catalog,meta);
  unsigned char* schema  = GetMetaPointer(search_schema, meta);
  unsigned char* table   = GetMetaPointer(search_table,  meta);
  unsigned char* stype   = GetMetaPointer(search_type,   meta);

  ODBC_CALL_ONCE(SQLTables(m_hstmt
                          ,catalog                   // Catalog name to search for
                          ,catalog ? SQL_NTS : 0     // Catalog name length
                          ,schema                    // Schema name to search for
                          ,schema  ? SQL_NTS : 0     // Schema name length
                          ,table                     // Table name to search for
                          ,table   ? SQL_NTS : 0     // Table Name length
                          ,stype                     // Table types to search for
                          ,stype   ? SQL_NTS : 0 )); // Table types length
  if(m_retCode == SQL_SUCCESS)
  {
     SQLBindCol(m_hstmt, 1, SQL_C_CHAR,szCatalogName,SQL_MAX_BUFFER, &cbCatalogName);
     SQLBindCol(m_hstmt, 2, SQL_C_CHAR,szSchemaName, SQL_MAX_BUFFER, &cbSchemaName);
     SQLBindCol(m_hstmt, 3, SQL_C_CHAR,szTableName,  SQL_MAX_BUFFER, &cbTableName);
     SQLBindCol(m_hstmt, 4, SQL_C_CHAR,szTableType,  SQL_MAX_BUFFER, &cbTableType);
     SQLBindCol(m_hstmt, 5, SQL_C_CHAR,szRemarks,  2*SQL_MAX_BUFFER, &cbRemarks);
     while(true)
     {
       m_retCode = SqlFetch(m_hstmt);
       if(m_retCode == SQL_ERROR || m_retCode == SQL_SUCCESS_WITH_INFO)
       {
         CString err = m_database->GetErrorString(m_hstmt);
         AfxMessageBox(err,MB_OK);
         if(m_retCode == SQL_ERROR)
         {
           return false;
         }
       }
       if(m_retCode == SQL_SUCCESS || m_retCode == SQL_SUCCESS_WITH_INFO)
       {
         CString line;
         // Testing for SQL_NO_TOTAL en SQL_NO_DATA
         // Build line "TYPE: catalog.schema.table (remarks)"
         line += MakeObjectName(cbCatalogName > 0 ? szCatalogName : (unsigned char *)""
                               ,cbSchemaName  > 0 ? szSchemaName  : (unsigned char *)""
                               ,cbTableName   > 0 ? szTableName   : (unsigned char *)""
                               ,cbTableType   > 0 ? szTableType   : (unsigned char *)"");
         if(cbRemarks > 0)  
         {
           line += CString(" (") + CString(szRemarks) + ")";
         }
         if(p_list)
         {
           p_list->push_back(line);
         }
         if(p_tables && cbTableName > 0)
         {
           p_tables->push_back(CString(szTableName));
         }
         // Now record the qualifiers for a table
         if(cbTableType > 0)
         {
           // Valid ODBC TableTypes are:
           // "TABLE"             : Real fysical table of data
           // "VIEW"              : View over one or more fysical tables
           // "SYSTEM TABLE"      : System catalog table
           // "GLOBAL TEMPORARY"  : Global temporary table (visible to other sessions)
           // "LOCAL TEMPORARY"   : Local temporary table only visible to current session
           // "ALIAS"             : MSSQL server like alias to another database/table
           // "SYNONYM"           : Oracle/Informix like alias to another database/table
           // "SYSTEM VIEW"       : Added for ORACLE: View over one or more system tables
           //                     : Alsoo in MS-SQLServer for the INFORMATION_SCHEMA
           // or RDBMS specific type, not listed above
           if((strcmp((char *)szTableType,"TABLE")             ==0) ||
              (strcmp((char *)szTableType,"VIEW")              ==0) ||
              (strcmp((char *)szTableType,"SYSTEM TABLE")      ==0) ||
              (strcmp((char *)szTableType,"SYSTEM VIEW")       ==0) ||
              (strcmp((char *)szTableType,"GLOBAL TEMPORARY")  ==0) ||
              (strcmp((char *)szTableType,"LOCAL TEMPORARY")   ==0))
           {
             if(foundOneTable)
             {
               // Found more than one table. Let user decide first
               retValue = false;
             }
             if(cbCatalogName > 0) m_searchCatalogName = szCatalogName;
             if(cbSchemaName  > 0) m_searchSchemaName  = szSchemaName;
             m_searchTableName = szTableName;
             foundOneTable = true;
           }
         }
       }
       else
       {
         break;
       }
     }
  }
  else
  {
    if(p_list)
    {
      CString sitem = "Driver not capable to find table/view: ";
      sitem += MakeObjectName(search_catalog,search_schema,search_table,search_type);
      p_list->push_back(sitem);

      CString errorText = "Error in ODBC statement: ";
      errorText += m_database->GetErrorString(m_hstmt);
      p_list->push_back(errorText);
    }
    retValue = false;
  }
  return retValue;
}

bool
SQLInfo::MakeInfoTableColumns(WordList* p_list)
{
  if(m_searchTableName.IsEmpty())
  {
    return false;
  }
  SQLCHAR      szCatalogName [SQL_MAX_BUFFER+1];
  SQLLEN       cbCatalogName = 0;
  SQLCHAR      szSchemaName  [SQL_MAX_BUFFER+1];
  SQLLEN       cbSchemaName  = 0;
  SQLCHAR      szTableName   [SQL_MAX_BUFFER+1];
  SQLLEN       cbTableName   = 0;
  SQLCHAR      szColumnName  [SQL_MAX_BUFFER+1];
  SQLLEN       cbColumnName  = 0;
  SQLCHAR      szTypeName    [SQL_MAX_BUFFER+1];
  SQLLEN       cbTypeName    = 0;
  SQLCHAR      szRemarks     [2 *SQL_MAX_BUFFER+1];
  SQLLEN       cbRemarks     = 0;
  SQLSMALLINT  DataType    = 0;
  SQLLEN       cbDataType    = 0;
  SQLINTEGER   Precision     = 0;
  SQLINTEGER   Length        = 0;
  SQLSMALLINT  Scale         = 0;
  SQLSMALLINT  Nullable      = 0;
  SQLLEN       cbPrecision   = 0;
  SQLLEN       cbLength      = 0;
  SQLLEN       cbScale       = 0;
  SQLLEN       cbNullable    = 0;

  // Check whether we can do this
  if(!SupportedFunction(SQL_API_SQLCOLUMNS))
  {
    p_list->push_back("SQLColumns unsupported. Get a better ODBC driver!");
    return true;
  }
  strcpy((char*)szCatalogName,m_searchCatalogName.GetString());
  strcpy((char*)szSchemaName, m_searchSchemaName.GetString());
  strcpy((char*)szTableName,  m_searchTableName.GetString());
  szColumnName[0] = 0;

  CloseStatement();
  bool meta = GetStatement(false);

  // - If the driver cannot search on this type of META-object the pointer MUST be NULL
  unsigned char* catalog = GetMetaPointer(szCatalogName,meta);
  unsigned char* schema  = GetMetaPointer(szSchemaName, meta);
  unsigned char* table   = GetMetaPointer(szTableName,  meta);
  unsigned char* column  = GetMetaPointer(szColumnName, meta);

  ODBC_CALL_ONCE(SQLColumns(m_hstmt
                           ,catalog                  // Catalog name to search for
                           ,catalog ? SQL_NTS : 0    // Catalog name length
                           ,schema                   // Schema name to search for
                           ,schema  ? SQL_NTS : 0    // Schema name length
                           ,table                    // Table Name to search for
                           ,table   ? SQL_NTS : 0    // Table name length
                           ,column                   // Column name to search for
                           ,column  ? SQL_NTS : 0)); // Column name length
  if(m_retCode == SQL_SUCCESS)
  {
     SQLBindCol(m_hstmt, 1, SQL_C_CHAR,   szCatalogName,SQL_MAX_BUFFER, &cbCatalogName);
     SQLBindCol(m_hstmt, 2, SQL_C_CHAR,   szSchemaName, SQL_MAX_BUFFER, &cbSchemaName);
     SQLBindCol(m_hstmt, 3, SQL_C_CHAR,   szTableName,  SQL_MAX_BUFFER, &cbTableName);
     SQLBindCol(m_hstmt, 4, SQL_C_CHAR,   szColumnName, SQL_MAX_BUFFER, &cbColumnName);
     SQLBindCol(m_hstmt, 5, SQL_C_SHORT, &DataType,     2,              &cbDataType);
     SQLBindCol(m_hstmt, 6, SQL_C_CHAR,   szTypeName,   SQL_MAX_BUFFER, &cbTypeName);
     SQLBindCol(m_hstmt, 7, SQL_C_SLONG, &Precision,    4,              &cbPrecision);
     SQLBindCol(m_hstmt, 8, SQL_C_SLONG, &Length,       4,              &cbLength);
     SQLBindCol(m_hstmt, 9, SQL_C_SSHORT,&Scale,        2,              &cbScale);
     SQLBindCol(m_hstmt,11, SQL_C_SSHORT,&Nullable,     2,              &cbNullable);
     SQLBindCol(m_hstmt,12, SQL_C_CHAR,   szRemarks,  2*SQL_MAX_BUFFER, &cbRemarks);
     while(true)
     {
       m_retCode = SqlFetch(m_hstmt);
       if(m_retCode == SQL_ERROR || m_retCode == SQL_SUCCESS_WITH_INFO)
       {
         CString err = m_database->GetErrorString(m_hstmt);
         AfxMessageBox(err,MB_OK);
         if(m_retCode == SQL_ERROR)
         {
           return false;
         }
       }
       if(m_retCode == SQL_SUCCESS || m_retCode == SQL_SUCCESS_WITH_INFO)
       {
         CString line;
         CString type;
         // NAME
         if(cbColumnName > 0)
         {
           line += CString(szColumnName);
         }
         // DATATYPE
         if(cbDataType > 0)
         {
           type = ODBCDataType(DataType);
         }
         if(cbTypeName > 0)
         {
           if(type.CompareNoCase((char*)szTypeName))
           {
             type = CString(szTypeName);
           }
         }
         if(cbDataType<0 && cbTypeName<0)
         {
           type = "UNKNOWN-TYPE";
         }
         line += CString(" ") + type;
         // PRECISION and SCALE
         if(cbPrecision > 0 && Precision > 0)
         {
           CString precscale;
           precscale.Format("%d",Precision);
           if(cbScale > 0)
           {
             CString scale;
             scale.Format("%d",Scale);
             precscale += "," + scale;
           }
           line += "(" + precscale + ")";
         }
         // NULLABLE
         if(cbNullable > 0)
         {
           if(Nullable == SQL_NO_NULLS)         line += " NOT NULL";
           if(Nullable == SQL_NULLABLE_UNKNOWN) line += " NULLABLE UNKNOWN";
         }
         // REMARKS
         if(cbRemarks > 0)
         {
           line += CString(" (") + CString(szRemarks) + ")";
         }
         p_list->push_back(line);
       }
       else
       {
         break;
       }
     }
  }
  else
  {
    CString sitem = "Driver not capable to find columns for: ";
    sitem += MakeObjectName((SQLCHAR *)m_searchCatalogName.GetString()
                           ,(SQLCHAR *)m_searchSchemaName.GetString()
                           ,(SQLCHAR *)m_searchTableName.GetString()
                           ,(SQLCHAR *)m_searchTableType.GetString());
    p_list->push_back(sitem);

    CString errorText = "Error in ODBC statement: ";
    errorText += m_database->GetErrorString(m_hstmt);
    p_list->push_back(errorText);
  }
  return true;
}

bool
SQLInfo::MakeInfoTablePrimary(WordList*   p_list
                             ,CString&    p_primaryName
                             ,PrimaryMap& p_keymap)
{
  if(m_searchTableName.IsEmpty())
  {
    return false;
  }
  SQLCHAR      szCatalogName [SQL_MAX_BUFFER];
  SQLLEN       cbCatalogName = 0;
  SQLCHAR      szSchemaName  [SQL_MAX_BUFFER];
  SQLLEN       cbSchemaName  = 0;
  SQLCHAR      szTableName   [SQL_MAX_BUFFER];
  SQLLEN       cbTableName   = 0;
  SQLCHAR      szColumnName  [SQL_MAX_BUFFER];
  SQLLEN       cbColumnName  = 0;
  SWORD          KeySeq      = 0;
  SQLLEN       cbKeySeq      = 0;
  SQLCHAR      szPkName      [SQL_MAX_BUFFER];
  SQLLEN       cbPkName      = 0;
  bool         first         = true;

  // Check whether we can do this
  if(!SupportedFunction(SQL_API_SQLPRIMARYKEYS))
  {
    if(p_list)
    {
      p_list->push_back("SQLPrimaryKeys unsupported. Get a better ODBC driver!");
    }
    return true;
  }
  strcpy((char*)szCatalogName,m_searchCatalogName.GetString());
  strcpy((char*)szSchemaName, m_searchSchemaName.GetString());
  strcpy((char*)szTableName,  m_searchTableName.GetString());

  CloseStatement();
  bool meta = GetStatement(false);

  unsigned char* catalog = GetMetaPointer(szCatalogName,meta);
  unsigned char* schema  = GetMetaPointer(szSchemaName, meta);
  unsigned char* table   = GetMetaPointer(szTableName,  meta);

  ODBC_CALL_ONCE(SQLPrimaryKeys(m_hstmt
                               ,catalog                  // Catalog name to search for
                               ,catalog ? SQL_NTS : 0    // Catalog name length
                               ,schema                   // Schema name to search for
                               ,schema  ? SQL_NTS : 0    // Schema name length
                               ,table                    // Table Name to search for
                               ,table   ? SQL_NTS : 0    // Table name length
                               ));
  if(m_retCode == SQL_SUCCESS)
  {
     SQLBindCol(m_hstmt, 1, SQL_C_CHAR,  szCatalogName,SQL_MAX_BUFFER, &cbCatalogName);
     SQLBindCol(m_hstmt, 2, SQL_C_CHAR,  szSchemaName, SQL_MAX_BUFFER, &cbSchemaName);
     SQLBindCol(m_hstmt, 3, SQL_C_CHAR,  szTableName,  SQL_MAX_BUFFER, &cbTableName);
     SQLBindCol(m_hstmt, 4, SQL_C_CHAR,  szColumnName, SQL_MAX_BUFFER, &cbColumnName);
     SQLBindCol(m_hstmt, 5, SQL_C_SSHORT,&KeySeq,      0,              &cbKeySeq);
     SQLBindCol(m_hstmt, 6, SQL_C_CHAR,  szPkName,     SQL_MAX_BUFFER, &cbPkName);
     while(true)
     {
       m_retCode = SqlFetch(m_hstmt);
       if(m_retCode == SQL_ERROR || m_retCode == SQL_SUCCESS_WITH_INFO)
       {
         CString err = m_database->GetErrorString(m_hstmt);
         AfxMessageBox(err,MB_OK);
         if(m_retCode == SQL_ERROR)
         {
           return false;
         }
       }
       if(m_retCode == SQL_SUCCESS || m_retCode == SQL_SUCCESS_WITH_INFO)
       {
         if(first)
         {
           first = false;
           CString name("Name: ");
           if(cbPkName > 0)
           {
             name += CString(szPkName);
           }
           if(p_list)
           {
             p_list->push_back(name);
             p_list->push_back("+");
           }
         }
         CString line;
         line.Format("%d: ",KeySeq);
         if(cbColumnName)
         {
           line += CString(szColumnName);
         }
         if(p_list)
         {
           // Return info in a InfoTree
           p_list->push_back(line);
         }
         else
         {
           // Return Primary key info in a map
           p_primaryName = CString(szPkName);

           // Save the primary key column
           PrimKeyInfo info;
           info.m_colName   = CString(szColumnName);
           info.m_colPos    = KeySeq;
           info.m_queryPos  = 0;
           p_keymap.insert(std::make_pair(KeySeq - 1,info));
         }
       }
       else
       {
         break;
       }
     }
  }
  else
  {
    if(p_list)
    {
      CString sitem = "Driver not capable to find primary key for: ";
      sitem += MakeObjectName((SQLCHAR *)m_searchCatalogName.GetString()
                             ,(SQLCHAR *)m_searchSchemaName.GetString()
                             ,(SQLCHAR *)m_searchTableName.GetString()
                             ,(SQLCHAR *)m_searchTableType.GetString());
      p_list->push_back(sitem);

      CString errorText = "Error in ODBC statement: ";
      errorText += m_database->GetErrorString(m_hstmt);
      p_list->push_back(errorText);
    }
  }
  return true;
}

bool 
SQLInfo::MakeInfoTableForeign(WordList* p_list,bool ref)
{
  if(m_searchTableName.IsEmpty())
  {
    return false;
  }
  SQLCHAR      szPKCatalogName [SQL_MAX_BUFFER];
  SQLLEN       cbPKCatalogName = 0;
  SQLCHAR      szPKSchemaName  [SQL_MAX_BUFFER];
  SQLLEN       cbPKSchemaName  = 0;
  SQLCHAR      szPKTableName   [SQL_MAX_BUFFER];
  SQLLEN       cbPKTableName   = 0;
  SQLCHAR      szFKCatalogName [SQL_MAX_BUFFER];
  SQLLEN       cbFKCatalogName = 0;
  SQLCHAR      szFKSchemaName  [SQL_MAX_BUFFER];
  SQLLEN       cbFKSchemaName  = 0;
  SQLCHAR      szFKTableName   [SQL_MAX_BUFFER];
  SQLLEN       cbFKTableName   = 0;
  SQLCHAR      szPKColumnName  [SQL_MAX_BUFFER];
  SQLLEN       cbPKColumnName  = 0;
  SQLCHAR      szFKColumnName  [SQL_MAX_BUFFER];
  SQLLEN       cbFKColumnName  = 0;
  SWORD          KeySeq        = 0;
  SQLLEN       cbKeySeq        = 0;
  SWORD          UpdateRule    = 0;
  SQLLEN       cbUpdateRule    = 0;
  SWORD          DeleteRule    = 0;
  SQLLEN       cbDeleteRule    = 0;
  SWORD          Deferrab      = 0;
  SQLLEN       cbDeferrab      = 0;
  SQLCHAR      szFKKeyName    [SQL_MAX_BUFFER];
  SQLLEN       cbFKKeyName     = 0;
  SQLCHAR      szPKKeyName    [SQL_MAX_BUFFER];
  SQLLEN       cbPKKeyName     = 0;

  // Check whether we can do this
  if(!SupportedFunction(SQL_API_SQLFOREIGNKEYS))
  {
    p_list->push_back("SQLForeignKeys unsupported. Get a better ODBC driver!");
    return true;
  }
  CloseStatement();
  bool meta = GetStatement(false);

  if(ref)
  {
    strcpy((char*)szPKCatalogName,m_searchCatalogName.GetString());
    strcpy((char*)szPKSchemaName, m_searchSchemaName.GetString());
    strcpy((char*)szPKTableName,  m_searchTableName.GetString());
    szFKCatalogName[0] = 0;
    szFKSchemaName [0] = 0;
    szFKTableName  [0] = 0;
  }
  else
  {
    szPKCatalogName[0] = 0;
    szPKSchemaName [0] = 0;
    szPKTableName  [0] = 0;
    strcpy((char*)szFKCatalogName,m_searchCatalogName.GetString());
    strcpy((char*)szFKSchemaName, m_searchSchemaName.GetString());
    strcpy((char*)szFKTableName,  m_searchTableName.GetString());
  }
  unsigned char* PKcatalog = GetMetaPointer(szPKCatalogName,meta);
  unsigned char* PKschema  = GetMetaPointer(szPKSchemaName, meta);
  unsigned char* PKtable   = GetMetaPointer(szPKTableName,  meta);
  unsigned char* FKcatalog = GetMetaPointer(szFKCatalogName,meta);
  unsigned char* FKschema  = GetMetaPointer(szFKSchemaName, meta);
  unsigned char* FKtable   = GetMetaPointer(szFKTableName,  meta);

  m_retCode = SQL_ERROR;
  ODBC_CALL_ONCE(SQLForeignKeys(m_hstmt
                               ,PKcatalog                  // Catalog name to search for
                               ,PKcatalog ? SQL_NTS : 0    // Catalog name length
                               ,PKschema                   // Schema name to search for
                               ,PKschema  ? SQL_NTS : 0    // Schema name length
                               ,PKtable                    // Table Name to search for
                               ,PKtable   ? SQL_NTS : 0    // Table name length
                               ,FKcatalog                  // Catalog name to search for
                               ,FKcatalog ? SQL_NTS : 0    // Catalog name length
                               ,FKschema                   // Schema name to search for
                               ,FKschema  ? SQL_NTS : 0    // Schema name length
                               ,FKtable                    // Table Name to search for
                               ,FKtable   ? SQL_NTS : 0    // Table name length
                               ));
  if(m_retCode == SQL_SUCCESS)
  {
     SQLBindCol(m_hstmt, 1, SQL_C_CHAR, szPKCatalogName,SQL_MAX_BUFFER, &cbPKCatalogName);
     SQLBindCol(m_hstmt, 2, SQL_C_CHAR, szPKSchemaName, SQL_MAX_BUFFER, &cbPKSchemaName);
     SQLBindCol(m_hstmt, 3, SQL_C_CHAR, szPKTableName,  SQL_MAX_BUFFER, &cbPKTableName);
     SQLBindCol(m_hstmt, 4, SQL_C_CHAR, szPKColumnName, SQL_MAX_BUFFER, &cbPKColumnName);
     SQLBindCol(m_hstmt, 5, SQL_C_CHAR, szFKCatalogName,SQL_MAX_BUFFER, &cbFKCatalogName);
     SQLBindCol(m_hstmt, 6, SQL_C_CHAR, szFKSchemaName, SQL_MAX_BUFFER, &cbFKSchemaName);
     SQLBindCol(m_hstmt, 7, SQL_C_CHAR, szFKTableName,  SQL_MAX_BUFFER, &cbFKTableName);
     SQLBindCol(m_hstmt, 8, SQL_C_CHAR, szFKColumnName, SQL_MAX_BUFFER, &cbFKColumnName);
     SQLBindCol(m_hstmt, 9, SQL_C_SSHORT,&KeySeq,       0,              &cbKeySeq);
     SQLBindCol(m_hstmt,10, SQL_C_SSHORT,&UpdateRule,   0,              &cbUpdateRule);
     SQLBindCol(m_hstmt,11, SQL_C_SSHORT,&DeleteRule,   0,              &cbDeleteRule);
     SQLBindCol(m_hstmt,12, SQL_C_CHAR, szFKKeyName,    SQL_MAX_BUFFER, &cbFKKeyName);
     SQLBindCol(m_hstmt,13, SQL_C_CHAR, szPKKeyName,    SQL_MAX_BUFFER, &cbPKKeyName);
     SQLBindCol(m_hstmt,14, SQL_C_SSHORT,&Deferrab,     0,              &cbDeferrab);
     while(true)
     {
       m_retCode = SqlFetch(m_hstmt);
       if(m_retCode == SQL_ERROR || m_retCode == SQL_SUCCESS_WITH_INFO)
       {
         CString err = m_database->GetErrorString(m_hstmt);
         AfxMessageBox(err,MB_OK);
         if(m_retCode == SQL_ERROR)
         {
           return false;
         }
       }
       if(m_retCode == SQL_SUCCESS || m_retCode == SQL_SUCCESS_WITH_INFO)
       {
         CString line;
         if(KeySeq == 1)
         {
           // Begin a new Foreign key
           if(p_list->size())
           {
             p_list->push_back("-");
           }

           // FK Keyname of PK Tablename
           if(ref)
           {
             line = "Table: ";
             if(cbFKTableName)
             {
               line += (char *)szFKTableName;
             }
           }
           else
           {
             line = "Name: ";
             if(cbFKKeyName)
             {
               line += (char*)szFKKeyName;
             }
           }
           //leave = tree->InsertItem(line,infoForeign);
           p_list->push_back(line);
           p_list->push_back("+");

           // Update Rule
           line = "Update rule: ";
           if(cbUpdateRule > 0)
           {
              switch(UpdateRule)
              {
                case SQL_CASCADE:     line += "CASCADE";     break;
                case SQL_NO_ACTION:   line += "NO ACTION";   break;
                case SQL_SET_NULL:    line += "SET NULL";    break;
                case SQL_SET_DEFAULT: line += "SET DEFAULT"; break;
                default:              line += "no info";     break;
              }
           }
           //tree->InsertItem(line,leave);
           p_list->push_back(line);

           // DELETE RULE
           line = "Delete rule: ";
           if(cbDeleteRule > 0)
           {
             switch(DeleteRule)
             {
               case SQL_CASCADE:     line += "CASCADE";     break;
               case SQL_NO_ACTION:   line += "NO ACTION";   break;
               case SQL_SET_NULL:    line += "SET NULL";    break;
               case SQL_SET_DEFAULT: line += "SET DEFAULT"; break;
               default:              line += "no info";     break;
             }
           }
           //tree->InsertItem(line,leave);
           p_list->push_back(line);

           // DEFERRABILITY
           line = "Deferrable: ";
           if(cbDeferrab)
           {
             switch(Deferrab)
             {
               case SQL_INITIALLY_DEFERRED:  line += "INITIALLY DEFERRED";  break;
               case SQL_INITIALLY_IMMEDIATE: line += "INITIALLY IMMEDIATE"; break;
               case SQL_NOT_DEFERRABLE:      line += "NOT DEFERRABLE";      break;
               default:                      line += "no info";             break;
             }
           }
           // tree->InsertItem(line,leave);
           p_list->push_back(line);

           // Leaves for columns and referenced primary key
           // FOREIGN KEY
           if(ref)
           {
             line  = "Foreign key: ";
             if(cbFKKeyName)
             {
               line += (char *)szFKKeyName;
             }
           }
           else
           {
             line = "Foreign key columns";
           }
           p_list->push_back(line);

           // PRIMARY KEY
           line = CString("Primary key: ") + 
                  CString(szPKTableName) + " (" +
                  CString(szPKKeyName)   + ")";
           p_list->push_back(line);
           p_list->push_back("+");
         }
         // All columns in the Foreign/Primary keys
         // FOREIGN KEY COLUMN -> PRIMARY KEY COLUMN
         line.Empty();
         if(cbFKColumnName)
         {
           line = (char *)szFKColumnName;
         }
         if(cbPKColumnName)
         {
           line += " -> ";
           line += (char *)szPKColumnName;
         }
         if(!line.IsEmpty())
         {
           p_list->push_back(line);
         }
       }
       else
       {
         break;
       }
     }
  }
  else
  {
    CString sitem = "Driver not capable to find foreign keys for: ";
    sitem += MakeObjectName((SQLCHAR *)m_searchCatalogName.GetString()
                           ,(SQLCHAR *)m_searchSchemaName.GetString()
                           ,(SQLCHAR *)m_searchTableName.GetString()
                           ,(SQLCHAR *)m_searchTableType.GetString());
    p_list->push_back(sitem);

    CString errorText = "Error in ODBC statement: ";
    errorText += m_database->GetErrorString(m_hstmt);
    p_list->push_back(errorText);
  }
  return true;
}

bool 
SQLInfo::MakeInfoTableStatistics(WordList*   p_list
                                ,CString&    keyName
                                ,PrimaryMap& keyMap)
{
  if(m_searchTableName.IsEmpty())
  {
    return false;
  }
  SQLCHAR      szCatalogName [SQL_MAX_BUFFER + 1] = "";
  SQLLEN       cbCatalogName = 0;
  SQLCHAR      szSchemaName  [SQL_MAX_BUFFER + 1] = "";
  SQLLEN       cbSchemaName  = 0;
  SQLCHAR      szTableName   [SQL_MAX_BUFFER + 1] = "";
  SQLLEN       cbTableName   = 0;
  SQLSMALLINT  NonUnique   = 0;
  SQLLEN       cbNonUnique   = 0;
  SQLCHAR      szIndexName   [SQL_MAX_BUFFER + 1] = "";
  SQLLEN       cbIndexName   = 0;
  SQLSMALLINT    IndexType   = 0;
  SQLLEN       cbIndexType   = 0;
  SQLSMALLINT    OrdinalPos  = 0;
  SQLLEN       cbOrdinalPos  = 0;
  SQLCHAR      szColumnName  [SQL_MAX_BUFFER + 1] = "";
  SQLLEN       cbColumnName  = 0;
  SQLCHAR        AscDesc     [10];
  SQLLEN       cbAscDesc     = 0;
  SQLINTEGER     Cardinality = 0;
  SQLLEN       cbCardinality = 0;
  SQLINTEGER     Pages       = 0;
  SQLLEN       cbPages       = 0;
  SQLCHAR      szFilter      [SQL_MAX_BUFFER + 1] = "";
  SQLLEN       cbFilter      = 0;
  SQLUSMALLINT searchType    = p_list ? SQL_INDEX_ALL : SQL_INDEX_UNIQUE;

  // Check whether we can do this
  if(!SupportedFunction(SQL_API_SQLSTATISTICS))
  {
    if(p_list)
    {
      p_list->push_back("SQLStatistics unsupported. Get a better ODBC driver!");
    }
    return true;
  }
  strcpy((char*)szCatalogName,m_searchCatalogName.GetString());
  strcpy((char*)szSchemaName, m_searchSchemaName.GetString());
  strcpy((char*)szTableName,  m_searchTableName.GetString());

  CloseStatement();
  bool meta = GetStatement(false);

  unsigned char* catalog = GetMetaPointer(szCatalogName,meta);
  unsigned char* schema  = GetMetaPointer(szSchemaName, meta);
  unsigned char* table   = GetMetaPointer(szTableName,  meta);

  m_retCode = SQL_ERROR;
  ODBC_CALL_ONCE(SQLStatistics(m_hstmt
                              ,catalog                  // Catalog name to search for
                              ,catalog ? SQL_NTS : 0    // Catalog name length
                              ,schema                   // Schema name to search for
                              ,schema  ? SQL_NTS : 0    // Schema name length
                              ,table                    // Table Name to search for
                              ,table   ? SQL_NTS : 0    // Table name length
                              ,searchType
                              ,SQL_QUICK));    // Make sure we get the right Cardinality and Pages (SQL_ENSURE)
  if(m_retCode == SQL_SUCCESS)
  {
    SQLBindCol(m_hstmt, 1, SQL_C_CHAR,   szCatalogName,SQL_MAX_BUFFER, &cbCatalogName);
    SQLBindCol(m_hstmt, 2, SQL_C_CHAR,   szSchemaName, SQL_MAX_BUFFER, &cbSchemaName);
    SQLBindCol(m_hstmt, 3, SQL_C_CHAR,   szTableName,  SQL_MAX_BUFFER, &cbTableName);
    SQLBindCol(m_hstmt, 4, SQL_C_SSHORT,&NonUnique,    0,              &cbNonUnique);
    SQLBindCol(m_hstmt, 6, SQL_C_CHAR,   szIndexName,  SQL_MAX_BUFFER, &cbIndexName);
    SQLBindCol(m_hstmt, 7, SQL_C_SSHORT,&IndexType,    0,              &cbIndexType);
    SQLBindCol(m_hstmt, 8, SQL_C_SSHORT,&OrdinalPos,   0,              &cbOrdinalPos);
    SQLBindCol(m_hstmt, 9, SQL_C_CHAR,   szColumnName, SQL_MAX_BUFFER, &cbColumnName);
    SQLBindCol(m_hstmt,10, SQL_C_CHAR,  &AscDesc,      2,              &cbAscDesc);
    SQLBindCol(m_hstmt,11, SQL_C_LONG,  &Cardinality,  0,              &cbCardinality);
    SQLBindCol(m_hstmt,12, SQL_C_LONG,  &Pages,        0,              &cbPages);
    SQLBindCol(m_hstmt,13, SQL_C_CHAR,   szFilter,     SQL_MAX_BUFFER, &cbFilter);
    while(true)
    {
      m_retCode = SqlFetch(m_hstmt);
      if(m_retCode == SQL_ERROR || m_retCode == SQL_SUCCESS_WITH_INFO)
      {
        CString err = m_database->GetErrorString(m_hstmt);
        AfxMessageBox(err,MB_OK);
        if(m_retCode == SQL_ERROR)
        {
          return false;
        }
      }
      if(m_retCode == SQL_SUCCESS || m_retCode == SQL_SUCCESS_WITH_INFO)
      {
        CString line;
        if(IndexType == SQL_TABLE_STAT)
        {
          if(p_list)
          {
            if(p_list->size())
            {
              p_list->push_back("-");
            }
            //index = tree->InsertItem("[Table statistics]",infoStatistics);
            p_list->push_back("[Table statistics]");
            p_list->push_back("+");
            line.Format("Cardinality: %d",Cardinality);
            //tree->InsertItem(line,index);
            p_list->push_back(line);
            line.Format("Data pages: %d",Pages);
            //tree->InsertItem(line,index);
            p_list->push_back(line);
            p_list->push_back("-");
          }
        }
        else
        {
          if(OrdinalPos == 1)
          {
            // Do once for the index
            line = "Index name: ";
            line += CString(szIndexName);
            if(cbNonUnique > 0)
            {
              switch(NonUnique)
              {
                case 0: line += " (Unique)";     break;
                case 1: line += " (Duplicates)"; break;
              }
            }
            switch(IndexType)
            {
              case SQL_INDEX_CLUSTERED: line += " [Clustered]";        break;
              case SQL_INDEX_HASHED:    line += " [Hashed]";           break;
              case SQL_INDEX_OTHER:
              case 4: /*SQL_INDEX_BTREE*/
              case 5: /*SQL_INDEX_CONTENT*/ break;
            }
            if(p_list)
            {
              // Start new index node
              if(p_list->size())
              {
                p_list->push_back("-");
              }
              p_list->push_back(line);
              p_list->push_back("+");

              if(cbCardinality > 0)
              {
                line.Format("Cardinality: %d",Cardinality);
                //tree->InsertItem(line,index);
                p_list->push_back(line);
              }
              if(cbPages > 0)
              {
                line.Format("Index pages: %d",Pages);
                //tree->InsertItem(line,index);
                p_list->push_back(line);
              }
              if(cbFilter > 0)
              {
                line.Format("Filter: %s",szFilter);
                // tree->InsertItem(line,index);
                p_list->push_back(line);
              }
            }
            else
            {
              // Report the key
              // Return Primary key info in a map
              if(keyName.IsEmpty())
              {
                keyName = CString(szIndexName);
              }
              else if(keyName.Compare(CString(szIndexName)))
              {
                // Return on second unique key
                return true;
              }
            }
          }

          // Do for every column
          if(IndexType != SQL_TABLE_STAT)
          {
            line.Format("%d: ",OrdinalPos);
            line += CString(szColumnName);
            if(cbAscDesc)
            {
              switch(AscDesc[0])
              {
                case 'A': line += " (Ascending)";  break;
                case 'D': line += " (Descending)"; break;
                default:  line += " (Unknown)";    break;
              }
            }
            if(p_list)
            {
              //tree->InsertItem(line,index);
              p_list->push_back(line);
            }
            else
            {
              // Record the columns of the unique key
              PrimKeyInfo info;
              info.m_colName   = CString(szColumnName);
              info.m_colPos    = OrdinalPos;
              info.m_queryPos  = 0;
              keyMap.insert(std::make_pair(OrdinalPos-1,info));
            }
          }
        }
      }
      else
      {
        break;
      }
    }
  }
  else
  {
    if(p_list)
    {
      CString sitem = "Driver not capable to find statistics for: ";
      sitem += MakeObjectName((SQLCHAR *)m_searchCatalogName.GetString()
                             ,(SQLCHAR *)m_searchSchemaName.GetString()
                             ,(SQLCHAR *)m_searchTableName.GetString()
                             ,(SQLCHAR *)m_searchTableType.GetString());
      p_list->push_back(sitem);

      CString errorText = "Error in ODBC statement: ";
      errorText += m_database->GetErrorString(m_hstmt);
      p_list->push_back(errorText);
    }
  }
  return true;
}

bool 
SQLInfo::MakeInfoTableSpecials(WordList* p_list)
{
  if(m_searchTableName.IsEmpty())
  {
    return false;
  }
  SQLCHAR      szCatalogName [SQL_MAX_BUFFER];
  SQLCHAR      szSchemaName  [SQL_MAX_BUFFER];
  SQLCHAR      szTableName   [SQL_MAX_BUFFER];
  SQLCHAR      szColumnName  [SQL_MAX_BUFFER];
  SQLLEN       cbColumnName  = 0;
  SWORD          Scope       = 0;
  SQLLEN       cbScope       = 0;
  SWORD          Pseudo      = 0;
  SQLLEN       cbPseudo      = 0;

  // Check whether we can do this
  if(!SupportedFunction(SQL_API_SQLSPECIALCOLUMNS))
  {
    p_list->push_back("SQLSpecialColumns unsupported. Get a better ODBC driver!");
    return true;
  }
  strcpy((char*)szCatalogName,m_searchCatalogName.GetString());
  strcpy((char*)szSchemaName, m_searchSchemaName.GetString());
  strcpy((char*)szTableName,  m_searchTableName.GetString());

  CloseStatement();
  bool meta = GetStatement(false);

  unsigned char* catalog = GetMetaPointer(szCatalogName,meta);
  unsigned char* schema  = GetMetaPointer(szSchemaName, meta);
  unsigned char* table   = GetMetaPointer(szTableName,  meta);

  m_retCode = SQL_ERROR;
  ODBC_CALL_ONCE(SQLSpecialColumns(m_hstmt
                                  ,SQL_BEST_ROWID           // SQL_ROWVER
                                  ,catalog                  // Catalog name to search for
                                  ,catalog ? SQL_NTS : 0    // Catalog name length
                                  ,schema                   // Schema name to search for
                                  ,schema  ? SQL_NTS : 0    // Schema name length
                                  ,table                    // Table Name to search for
                                  ,table   ? SQL_NTS : 0    // Table name length
                                  ,SQL_SCOPE_TRANSACTION    // SQL_SCOPE_SESSION, SQL_SCOPE_CURROW
                                  ,SQL_NO_NULLS));          // SQL_NULLABLE
  if(m_retCode == SQL_SUCCESS)
  {
     SQLBindCol(m_hstmt, 1, SQL_C_SSHORT,&Scope,      0,              &cbScope);
     SQLBindCol(m_hstmt, 2, SQL_C_CHAR,  szColumnName,SQL_MAX_BUFFER, &cbColumnName);
     SQLBindCol(m_hstmt, 8, SQL_C_SHORT, &Pseudo,     0,              &cbPseudo);
     while(true)
     {
       m_retCode = SqlFetch(m_hstmt);
       if(m_retCode == SQL_ERROR || m_retCode == SQL_SUCCESS_WITH_INFO)
       {
         CString err = m_database->GetErrorString(m_hstmt);
         AfxMessageBox(err,MB_OK);
         if(m_retCode == SQL_ERROR)
         {
           return false;
         }
       }
       if(m_retCode == SQL_SUCCESS || m_retCode == SQL_SUCCESS_WITH_INFO)
       {
         CString line;
         if(cbScope)
         {
           switch(Scope)
           {
             case SQL_SCOPE_CURROW:      line += "Current Row: "; break;
             case SQL_SCOPE_TRANSACTION: line += "Transaction: "; break;
             case SQL_SCOPE_SESSION:     line += "Session: ";     break;
             default:                    line += "Unknown";       break;
           }
         }
         if(cbColumnName)
         {
           line += (char*)szColumnName;
         }
         if(cbPseudo)
         {
           switch(Pseudo)
           {
             case SQL_PC_NOT_PSEUDO: line += " (not pseudo)"; break;
             case SQL_PC_PSEUDO:     line += " (pseudo)";     break;
             case SQL_PC_UNKNOWN:
             default:                break;
           }
         }
         p_list->push_back(line);
       }
       else
       {
         break;
       }
     }
  }
  else
  {
    CString sitem = "Driver not capable to find specials for: ";
    sitem += MakeObjectName((SQLCHAR *)m_searchCatalogName.GetString()
                           ,(SQLCHAR *)m_searchSchemaName.GetString()
                           ,(SQLCHAR *)m_searchTableName.GetString()
                           ,(SQLCHAR *)m_searchTableType.GetString());
    p_list->push_back(sitem);

    CString errorText = "Error in ODBC statement: ";
    errorText += m_database->GetErrorString(m_hstmt);
    p_list->push_back(errorText);
  }
  return true;
}

bool 
SQLInfo::MakeInfoTablePrivileges(WordList* p_list)
{
  if(m_searchTableName.IsEmpty())
  {
    return false;
  }
  SQLCHAR      szCatalogName [SQL_MAX_BUFFER];
  SQLLEN       cbCatalogName = 0;
  SQLCHAR      szSchemaName  [SQL_MAX_BUFFER];
  SQLLEN       cbSchemaName  = 0;
  SQLCHAR      szTableName   [SQL_MAX_BUFFER];
  SQLLEN       cbTableName   = 0;
  SQLCHAR      szGrantor     [SQL_MAX_BUFFER];
  SQLLEN       cbGrantor     = 0;
  SQLCHAR      szGrantee     [SQL_MAX_BUFFER];
  SQLLEN       cbGrantee     = 0;
  SQLCHAR      szPrivilege   [SQL_MAX_BUFFER];
  SQLLEN       cbPrivilege   = 0;
  SQLCHAR      szGrantable   [10];
  SQLLEN       cbGrantable   = 0;

  // Check whether we can do this
  if(!SupportedFunction(SQL_API_SQLTABLEPRIVILEGES))
  {
    p_list->push_back("SQLTablePrivileges unsupported. Get a better ODBC driver!");
    return true;
  }
  strcpy((char*)szCatalogName,m_searchCatalogName.GetString());
  strcpy((char*)szSchemaName, m_searchSchemaName.GetString());
  strcpy((char*)szTableName,  m_searchTableName.GetString());

  CloseStatement();
  bool meta = GetStatement(false);

  unsigned char* catalog = GetMetaPointer(szCatalogName,meta);
  unsigned char* schema  = GetMetaPointer(szSchemaName, meta);
  unsigned char* table   = GetMetaPointer(szTableName,  meta);

  m_retCode = SQL_ERROR;
  ODBC_CALL_ONCE(SQLTablePrivileges(m_hstmt
                                   ,catalog                  // Catalog name to search for
                                   ,catalog ? SQL_NTS : 0    // Catalog name length
                                   ,schema                   // Schema name to search for
                                   ,schema  ? SQL_NTS : 0    // Schema name length
                                   ,table                    // Table Name to search for
                                   ,table   ? SQL_NTS : 0    // Table name length
                                   ));
  if(m_retCode == SQL_SUCCESS)
  {
     SQLBindCol(m_hstmt, 1, SQL_C_CHAR, szCatalogName,SQL_MAX_BUFFER, &cbCatalogName);
     SQLBindCol(m_hstmt, 2, SQL_C_CHAR, szSchemaName, SQL_MAX_BUFFER, &cbSchemaName);
     SQLBindCol(m_hstmt, 3, SQL_C_CHAR, szTableName,  SQL_MAX_BUFFER, &cbTableName);
     SQLBindCol(m_hstmt, 4, SQL_C_CHAR, szGrantor,    SQL_MAX_BUFFER, &cbGrantor);
     SQLBindCol(m_hstmt, 5, SQL_C_CHAR, szGrantee,    SQL_MAX_BUFFER, &cbGrantee);
     SQLBindCol(m_hstmt, 6, SQL_C_CHAR, szPrivilege,  SQL_MAX_BUFFER, &cbPrivilege);
     SQLBindCol(m_hstmt, 7, SQL_C_CHAR, szGrantable,  SQL_MAX_BUFFER, &cbGrantable);
     while(true)
     {
       m_retCode = SqlFetch(m_hstmt);
       if(m_retCode == SQL_ERROR || m_retCode == SQL_SUCCESS_WITH_INFO)
       {
         CString err = m_database->GetErrorString(m_hstmt);
         AfxMessageBox(err,MB_OK);
         if(m_retCode == SQL_ERROR)
         {
           return false;
         }
       }
       if(m_retCode == SQL_SUCCESS || m_retCode == SQL_SUCCESS_WITH_INFO)
       {
         CString line;
         line  = (char *)szGrantee;
         line += " was granted ";
         line += (char *)szPrivilege;
         line += " by ";
         if(cbGrantor)
         {
           line += (char*)szGrantor;
         }
         if(cbGrantable)
         {
           if(strcmp((char*)szGrantable,"YES") == 0)
           {
             line += " (WITH GRANT OPTION)";
           }
         }
         p_list->push_back(line);
       }
       else
       {
         break;
       }
     }
  }
  else
  {
    CString sitem = "Driver not capable to find privileges for: ";
    sitem += MakeObjectName((SQLCHAR *)m_searchCatalogName.GetString()
                           ,(SQLCHAR *)m_searchSchemaName.GetString()
                           ,(SQLCHAR *)m_searchTableName.GetString()
                           ,(SQLCHAR *)m_searchTableType.GetString());
    p_list->push_back(sitem);

    CString errorText = "Error in ODBC statement: ";
    errorText += m_database->GetErrorString(m_hstmt);
    p_list->push_back(errorText);
  }
  return true;
}

bool
SQLInfo::MakeInfoProcedureProcedurepart(WordList* p_list
                                       ,CString&  p_procedure)
{
  bool         RetValue          = false;
  bool         doAllProcedures   = false;
  SQLCHAR      szCatalogName     [SQL_MAX_BUFFER];
  SQLLEN       cbCatalogName     = 0;
  SQLCHAR      szSchemaName      [SQL_MAX_BUFFER];
  SQLLEN       cbSchemaName      = 0;
  SQLCHAR      szProcedureName   [SQL_MAX_BUFFER];
  SQLLEN       cbProcedureName   = 0;
  SQLCHAR      szProcedureNType  [SQL_MAX_BUFFER];
  SQLSMALLINT    NumInputParams  = 0;  // Unreliable
  SQLLEN       cbNumInputParams  = 0;
  SQLSMALLINT    NumOutputParams = 0;  // Unreliable
  SQLLEN       cbNumOutputParams = 0;
  SQLSMALLINT    NumResultSets   = 0;  // Unreliable
  SQLLEN       cbNumResultSets   = 0;
  SQLCHAR      szRemarks         [2 * SQL_MAX_BUFFER];
  SQLLEN       cbRemarks         = 0;
  SQLSMALLINT    ProcedureType   = 0;
  SQLLEN       cbProcedureType   = 0;
  SQLCHAR      searchName[SQL_MAX_BUFFER];

  // Check whether we can do this
  if(!SupportedFunction(SQL_API_SQLPROCEDURES))
  {
    p_list->push_back("SQLProcedures unsupported. Get a better ODBC driver!");
    return false;
  }
  if(m_METADATA_ID_unsupported)
  {
    // Oops: Cannot search directly on table name!
    switch(m_identifierCase)
    {
      case SQL_IC_UPPER:     p_procedure.MakeUpper(); break;
      case SQL_IC_LOWER:     p_procedure.MakeLower(); break;
    }
  }
  // Split name in a maximum of three parts
  GetObjectName(p_procedure,szCatalogName,szSchemaName,szProcedureName,szProcedureNType);  // Get a statement handle for metadata use
  strcpy((char*)searchName,(char*)szProcedureName);
  if(strcmp((char*)szProcedureName,"*") == 0)
  {
    doAllProcedures = true;
    RetValue = false; // Do not continue with parameters
    strcpy((char*)szProcedureName,"%");
  }
  CloseStatement();
  bool meta = GetStatement(false);

  unsigned char* catalog   = GetMetaPointer(szCatalogName,  meta);
  unsigned char* schema    = GetMetaPointer(szSchemaName,   meta);
  unsigned char* procedure = GetMetaPointer(szProcedureName,meta);

  ODBC_CALL_ONCE(::SQLProcedures(m_hstmt
                                ,catalog 
                                ,catalog ? SQL_NTS   : 0
                                ,schema
                                ,schema  ? SQL_NTS   : 0
                                ,procedure
                                ,procedure ? SQL_NTS : 0));
  if(m_retCode == SQL_SUCCESS)
  {
    SQLBindCol(m_hstmt, 1, SQL_C_CHAR,  szCatalogName,   SQL_MAX_BUFFER, &cbCatalogName);
    SQLBindCol(m_hstmt, 2, SQL_C_CHAR,  szSchemaName,    SQL_MAX_BUFFER, &cbSchemaName);
    SQLBindCol(m_hstmt, 3, SQL_C_CHAR,  szProcedureName, SQL_MAX_BUFFER, &cbProcedureName);
    SQLBindCol(m_hstmt, 4, SQL_C_SSHORT,&NumInputParams, 0,              &cbNumInputParams);
    SQLBindCol(m_hstmt, 5, SQL_C_SSHORT,&NumOutputParams,0,              &cbNumOutputParams);
    SQLBindCol(m_hstmt, 6, SQL_C_SSHORT,&NumResultSets,  0,              &cbNumResultSets);
    SQLBindCol(m_hstmt, 7, SQL_C_CHAR,  szRemarks,     2*SQL_MAX_BUFFER, &cbRemarks);
    SQLBindCol(m_hstmt, 8, SQL_C_SSHORT,&ProcedureType,  0,              &cbProcedureType);
    while(true)
    {
      m_retCode = SqlFetch(m_hstmt);
      if(m_retCode == SQL_NO_DATA)
      {
        // No procedures found
        return RetValue;
      }
      if(m_retCode == SQL_ERROR || m_retCode == SQL_SUCCESS_WITH_INFO)
      {
        CString err = m_database->GetErrorString(m_hstmt);
        AfxMessageBox(err,MB_OK);
        if(m_retCode == SQL_ERROR)
        {
          return false;
        }
      }
      if(m_retCode == SQL_SUCCESS || m_retCode == SQL_SUCCESS_WITH_INFO)
      {
        if(stricmp((char*)searchName,(char*)szProcedureName) && !doAllProcedures)
        {
          // Oracle Propriety ODBC driver does this for synonym names
          // Search further on for the real function/procedure name
          continue;
        }
        // Record for search of parameters
        if(cbCatalogName > 0) m_searchCatalogName = szCatalogName;
        if(cbSchemaName  > 0) m_searchSchemaName  = szSchemaName;
        m_searchTableName = szProcedureName;

        // NAME
        CString line("PROCEDURE: ");
        if(cbProcedureName > 0)
        {
          line += MakeObjectName(cbCatalogName   > 0 ? szCatalogName   : (unsigned char *)""
                                ,cbSchemaName    > 0 ? szSchemaName    : (unsigned char *)""
                                ,cbProcedureName > 0 ? szProcedureName : (unsigned char *)""
                                ,(unsigned char*)"");
        }
        if(cbRemarks > 0)
        {
          line += CString(" (") + CString(szRemarks) + ")";
        }
        p_list->push_back(line);
        if(doAllProcedures)
        {
          continue;
        }
        // RETURNS
        line = "Returns: ";
        switch(ProcedureType)
        {
          case SQL_PT_PROCEDURE: line += "No value (real procedure)"; break;
          case SQL_PT_FUNCTION:  line += "Value (real function)";     break;
          case SQL_PT_UNKNOWN:   
          default:               line += "UNKNOWN / NOT DETERMINABLE"; break;
        }
        p_list->push_back(line);
        // Number of columns
        if(cbNumInputParams > 0)
        {
          line.Format("Input parameters: %d",NumInputParams);
          p_list->push_back(line);
        }
        if(cbNumOutputParams > 0)
        {
          line.Format("Output parameters: %d",NumOutputParams);
          p_list->push_back(line);
        }
        if(cbNumResultSets > 0)
        {
          line.Format("Result sets: %d",NumResultSets);
          p_list->push_back(line);
        }
        // Mark as found
        if(!doAllProcedures)
        {
          RetValue = true;
        }
      }
      else
      {
        break;
      }
    }
  }
  else
  {
    CString sitem = "Driver not capable to find procedures for: ";
    sitem += MakeObjectName((SQLCHAR *)m_searchCatalogName.GetString()
                           ,(SQLCHAR *)m_searchSchemaName.GetString()
                           ,(SQLCHAR *)m_searchTableName.GetString()
                           ,(SQLCHAR *)"");
    p_list->push_back(sitem);

    CString errorText = "Error in ODBC statement: ";
    errorText += m_database->GetErrorString(m_hstmt);
    p_list->push_back(errorText);
    return false;
  }
  return RetValue;
}

bool
SQLInfo::MakeInfoProcedureParameters(WordList* p_list)
{
  SQLCHAR      szCatalogName     [SQL_MAX_BUFFER];
  SQLLEN       cbCatalogName     = 0;
  SQLCHAR      szSchemaName      [SQL_MAX_BUFFER];
  SQLLEN       cbSchemaName      = 0;
  SQLCHAR      szProcedureName   [SQL_MAX_BUFFER];
  SQLLEN       cbProcedureName   = 0;
  SQLCHAR      szColumnName      [SQL_MAX_BUFFER];
  SQLLEN       cbColumnName      = 0;
  SQLSMALLINT    ColumnType      = 0;
  SQLLEN       cbColumnType      = 0;
  SQLSMALLINT    DataType        = 0;
  SQLLEN       cbDataType        = 0;
  SQLCHAR      szTypeName        [SQL_MAX_BUFFER];
  SQLLEN       cbTypeName        = 0;
  SQLINTEGER     ColumnSize      = 0;
  SQLSMALLINT    DecimalDigits   = 0;
  SQLLEN       cbDecimalDigits   = 0;
  SQLLEN       cbColumnSize      = 0;
  SQLSMALLINT    Nullable        = 0;
  SQLLEN       cbNullable        = 0;
  SQLCHAR      szRemarks         [2 * SQL_MAX_BUFFER];
  SQLLEN       cbRemarks         = 0;
  SQLCHAR      szDefaultValue    [2 * SQL_MAX_BUFFER];
  SQLLEN       cbDefaultValue    = 0;
  SQLINTEGER     OrdinalPos      = 0;
  SQLLEN       cbOrdinalPos      = 0;
  int pos = 0;

  // Check whether we can do this
  if(!SupportedFunction(SQL_API_SQLPROCEDURECOLUMNS))
  {
    p_list->push_back("SQLProcedureColumns unsupported. Get a better ODBC driver!");
    return true;
  }
  strcpy((char*)szCatalogName,  m_searchCatalogName.GetString());
  strcpy((char*)szSchemaName,   m_searchSchemaName.GetString());
  strcpy((char*)szProcedureName,m_searchTableName.GetString());
  szColumnName[0] = 0;

  CloseStatement();
  GetStatement();

  ODBC_CALL_ONCE(SQLProcedureColumns(m_hstmt
                                    ,szCatalogName[0] ? szCatalogName : NULL
                                    ,szCatalogName[0] ? SQL_NTS       : 0
                                    ,szSchemaName [0] ? szSchemaName  : NULL
                                    ,szSchemaName [0] ? SQL_NTS       : 0
                                    ,szProcedureName,SQL_NTS
                                    ,szColumnName,0)); // All columns
  if(m_retCode == SQL_SUCCESS)
  {
    SQLBindCol(m_hstmt, 1, SQL_C_CHAR, szCatalogName,  SQL_MAX_BUFFER, &cbCatalogName);
    SQLBindCol(m_hstmt, 2, SQL_C_CHAR, szSchemaName,   SQL_MAX_BUFFER, &cbSchemaName);
    SQLBindCol(m_hstmt, 3, SQL_C_CHAR, szProcedureName,SQL_MAX_BUFFER, &cbProcedureName);
    SQLBindCol(m_hstmt, 4, SQL_C_CHAR, szColumnName,   SQL_MAX_BUFFER, &cbColumnName);
    SQLBindCol(m_hstmt, 5, SQL_C_SSHORT,&ColumnType,   0,              &cbColumnType);
    SQLBindCol(m_hstmt, 6, SQL_C_SSHORT,&DataType,     0,              &cbDataType);
    SQLBindCol(m_hstmt, 7, SQL_C_CHAR, szTypeName,     SQL_MAX_BUFFER, &cbTypeName);
    SQLBindCol(m_hstmt, 8, SQL_C_LONG,  &ColumnSize,   0,              &cbColumnSize);
    SQLBindCol(m_hstmt,10, SQL_C_SSHORT,&DecimalDigits,0,              &cbDecimalDigits);
    SQLBindCol(m_hstmt,12, SQL_C_SSHORT,&Nullable,     0,              &cbNullable);
    SQLBindCol(m_hstmt,13, SQL_C_CHAR, szRemarks,     2*SQL_MAX_BUFFER,&cbRemarks);
    SQLBindCol(m_hstmt,14, SQL_C_CHAR, szDefaultValue,2*SQL_MAX_BUFFER,&cbDefaultValue);
    SQLBindCol(m_hstmt,18, SQL_C_LONG,  &OrdinalPos,   0,              &cbOrdinalPos);
    while(true)
    {
      m_retCode = SqlFetch(m_hstmt);
      if(m_retCode == SQL_ERROR || m_retCode == SQL_SUCCESS_WITH_INFO)
      {
        CString err = m_database->GetErrorString(m_hstmt);
        AfxMessageBox(err,MB_OK);
        if(m_retCode == SQL_ERROR)
        {
          return false;
        }
      }
      if(m_retCode == SQL_SUCCESS || m_retCode == SQL_SUCCESS_WITH_INFO)
      {
        CString line;
        if(cbOrdinalPos > 0)
        {
          line.Format("%d: ",OrdinalPos);
        }
        else
        {
          line.Format("%d: ", ++pos);
        }
        if(cbColumnName > 0)
        {
          line += CString(szColumnName);
        }
        else
        {
          line += "No parameter name reported";
        }
        if(cbNullable > 0)
        {
          switch(Nullable)
          {
            case SQL_NO_NULLS:         line += " [NOT NULL]"; break;
            case SQL_NULLABLE:         line += " [NULL]";     break;
            case SQL_NULLABLE_UNKNOWN: 
            default:                   line += " [Nulls unknown]";
                                       break;
          }
        }
        if(cbRemarks > 0)
        {
          line += CString(" (") + CString(szRemarks) + ")";
        }
        //HTREEITEM col = tree->InsertItem(line,item);
        p_list->push_back(line);
        p_list->push_back("+");

        if(cbColumnType > 0)
        {
          line = "TYPE: ";
          switch(ColumnType)
          {
            case SQL_PARAM_INPUT:        line += "Input parameter";        break;
            case SQL_PARAM_OUTPUT:       line += "Output parameter";       break;
            case SQL_PARAM_INPUT_OUTPUT: line += "Input/Output parameter"; break;
            case SQL_RESULT_COL:         line += "Result column";          break;
            case SQL_RETURN_VALUE:       line += "Return value";           
                                         pos = 0; // Reset counter
                                         break;
            case SQL_PARAM_TYPE_UNKNOWN: 
            default:                     line += "Parameter type is unknown"; 
                                         break;
          }
          //tree->InsertItem(line,col);
          p_list->push_back(line);
        }
        // ODBC DATA TYPE
        if(cbDataType > 0)
        {
          line = "ODBC Datatype: ";
          line += ODBCDataType(DataType);
          //tree->InsertItem(line,col);
          p_list->push_back(line);
        }
        // RDBMS DATA TYPE
        if(cbTypeName > 0)
        {
          line = "RDBMS Datatype: ";
          line += CString(szTypeName);
          // tree->InsertItem(line,col);
          p_list->push_back(line);
        }
        // COLUMN SIZE (If applicable)
        if(cbColumnSize > 0 || cbDecimalDigits > 0)
        {
          line.Format("Column size: %d",ColumnSize); 
          if(cbDecimalDigits > 0)
          {
            CString digits;
            digits.Format(",%d",DecimalDigits);
            line += digits;
          }
          // tree->InsertItem(line,col);
          p_list->push_back(line);
        }
        // DEFAULT VALUE
        if(cbDefaultValue > 0)
        {
          line = "Default: ";
          line += CString(szDefaultValue);
          //tree->InsertItem(line,col);
          p_list->push_back(line);
        }
        // Niveau terugzetten
        p_list->push_back("-");
      }
      else
      {
        break;
      }
    }
  }
  else
  {
    CString sitem = "Driver not capable to find procedures columns for: ";
    sitem += MakeObjectName((SQLCHAR *)m_searchCatalogName.GetString()
                           ,(SQLCHAR *)m_searchSchemaName.GetString()
                           ,(SQLCHAR *)m_searchTableName.GetString()
                           ,(SQLCHAR *)"");
    p_list->push_back(sitem);

    CString errorText = "Error in ODBC statement: ";
    errorText += m_database->GetErrorString(m_hstmt);
    p_list->push_back(errorText);
    return false;
  }
  return true;
}

// Return the native SQL command from an ODBC-escaped command
CString 
SQLInfo::NativeSQL(HDBC hdbc,CString& sqlCommand)
{
  // Check whether we can do this
  if(!SupportedFunction(SQL_API_SQLNATIVESQL))
  {
    AfxMessageBox("SQLNativeSQL unsupported. Get a better ODBC driver!",MB_OK|MB_ICONEXCLAMATION);
    return "";
  }
  SQLINTEGER retLen = 0;
  SQLCHAR buffer[30000];
  m_retCode = SqlNativeSql(hdbc
                         ,(SQLCHAR *)sqlCommand.GetString()
                         ,sqlCommand.GetLength()
                         ,buffer
                         ,(30000-1)
                         ,&retLen);
  if(m_retCode == SQL_SUCCESS)
  {
    if(retLen >= 0 && retLen < 30000)
    {
      buffer[retLen] = 0;
      return CString((char *)buffer);
    }
    else
    {
      // Overflow error
      CString error = "Buffer overflow (30.000 chars) on SQLNativeSQL";
      AfxMessageBox(error,MB_OK|MB_ICONERROR);
      return error;
    }
  }
  else
  {
    // SQLNativeSQL returned an error
    CString errorText = "Error while retrieving SQLNativeSQL:\n";
    errorText += m_database->GetErrorString(NULL);
    AfxMessageBox(errorText,MB_OK|MB_ICONERROR);
    return errorText;
  }
}

// Meta pointer to SQLGet<META> functions
// Catalog/Schema/Table/Column/Types fields
unsigned char*
SQLInfo::GetMetaPointer(unsigned char* p_buffer,bool p_meta)
{
  unsigned char* pointer = p_buffer;
  if(!pointer[0] && !p_meta)
  {
    pointer = NULL;
  }
  return pointer;
}

// GETTING ALL META TYPES
bool
SQLInfo::MakeInfoMetaTypes(WordList* p_list,int type)
{
  CString      sitem;
  bool         retValue      = true;
  SQLCHAR      szCatalogName [SQL_MAX_BUFFER];
  SQLLEN       cbCatalogName = 0;
  SQLCHAR      szSchemaName  [SQL_MAX_BUFFER];
  SQLLEN       cbSchemaName  = 0;
  SQLCHAR      szTableType   [SQL_MAX_BUFFER];
  SQLLEN       cbTableType   = 0;
  SQLCHAR      szRemarks     [SQL_MAX_BUFFER];
  SQLLEN       cbRemarks     = 0;
  // Where to search
  unsigned char search_catalog[10] = "";
  unsigned char search_schema [10] = "";
  unsigned char search_table  [10] = "";
  unsigned char search_type   [10] = "";
  // For duplicates
  CMapStringToString found;
  int numFound = 0;
  char* nameFound = NULL;

  // Check whether we can do this
  if(!SupportedFunction(SQL_API_SQLTABLES))
  {
    p_list->push_back("SQLTables unsupported. Get a better ODBC driver!");
    return false;
  }
  // Get a statement handle for metadata use
  CloseStatement();
  bool meta = GetStatement();

  switch(type)
  {
    case META_CATALOGS: strcpy((char*)search_catalog,SQL_ALL_CATALOGS);     break;
    case META_SCHEMAS:  strcpy((char*)search_schema, SQL_ALL_SCHEMAS);      break;
    case META_TABLES:   strcpy((char*)search_type,   SQL_ALL_TABLE_TYPES);  break;
    default: return false;
  }
  unsigned char* catalog = GetMetaPointer(search_catalog,meta);
  unsigned char* schema  = GetMetaPointer(search_schema, meta);
  unsigned char* table   = GetMetaPointer(search_table,  meta);
  unsigned char* stype   = GetMetaPointer(search_type,   meta);

  ODBC_CALL_ONCE(SQLTables(m_hstmt
                          ,catalog                   // Catalog name to search for
                          ,catalog ? SQL_NTS : 0     // Catalog name length
                          ,schema                    // Schema name to search for
                          ,schema  ? SQL_NTS : 0     // Schema name length
                          ,table                     // Table name to search for
                          ,table   ? SQL_NTS : 0     // Table Name length
                          ,stype                     // Table types to search for
                          ,stype   ? SQL_NTS : 0 )); // Table types length
  if(m_retCode == SQL_SUCCESS)
  {
    SqlBindCol(m_hstmt, 1, SQL_C_CHAR,szCatalogName,SQL_MAX_BUFFER, &cbCatalogName);
    SqlBindCol(m_hstmt, 2, SQL_C_CHAR,szSchemaName, SQL_MAX_BUFFER, &cbSchemaName);
    SqlBindCol(m_hstmt, 4, SQL_C_CHAR,szTableType,  SQL_MAX_BUFFER, &cbTableType);
    SqlBindCol(m_hstmt, 5, SQL_C_CHAR,szRemarks,    SQL_MAX_BUFFER, &cbRemarks);
    while(true)
    {
      m_retCode = SqlFetch(m_hstmt);
      if(m_retCode == SQL_ERROR)
      {
        break;
      }
      if(m_retCode == SQL_SUCCESS || m_retCode == SQL_SUCCESS_WITH_INFO)
      {
        nameFound = NULL;
        switch(type)
        {
          case META_CATALOGS: if(cbCatalogName > 0) 
                              {
                                nameFound = (char*) szCatalogName;
                              }
                              break;
          case META_SCHEMAS:  if(cbSchemaName > 0)
                              {
                                nameFound = (char*) szSchemaName;
                              }
                              break;
          case META_TABLES:   if(cbTableType)
                              {
                                nameFound = (char*) szTableType;
                              }
                              break;
        }
        if(nameFound)
        {
          CString val;
          if(found.Lookup(nameFound,val))
          {
            ++numFound;
          }
          else
          {
            found.SetAt(nameFound,nameFound);
            if(cbRemarks > 0)
            {
              strcat_s(nameFound,SQL_MAX_BUFFER," (");
              strcat_s(nameFound,SQL_MAX_BUFFER,(const char*)szRemarks);
              strcat_s(nameFound,SQL_MAX_BUFFER,")");
            }
            p_list->push_back(nameFound);
          }
        }
      }
      else if(m_retCode == SQL_NO_DATA)
      {
        switch(type)
        {
          case META_CATALOGS: m_canFindCatalogs = false; 
                              break;
          case META_SCHEMAS:  m_canFindSchemas  = false; 
                              break;
          case META_TABLES:   m_canFindTypes    = false;
          default:            break;
        }
        break;
      }
      else
      {
        if(numFound)
        {
          p_list->push_back("Duplicated meta-objects found. BUG in ODBC driver");
        }
        break;
      }
    }
  }
  if(m_retCode == SQL_ERROR)
  {
    p_list->push_back("Driver not capable to find meta-objects");

    CString errorText = "Error in ODBC statement: ";
    errorText += m_database->GetErrorString(m_hstmt);
    p_list->push_back(errorText);

    if(errorText.Find("HYC00") >= 0)
    {
      switch(type)
      {
        case META_CATALOGS: m_canFindCatalogs = false; 
                            break;
        case META_SCHEMAS:  m_canFindSchemas  = false; 
                            break;
        case META_TABLES:   m_canFindTypes    = false;
        default:            break;
      }
    }
    retValue = false;
  }
  CloseStatement();
  return retValue;
}

