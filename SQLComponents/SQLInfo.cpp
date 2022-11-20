////////////////////////////////////////////////////////////////////////
//
// File: SQLInfo.cpp
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
#include "SQLQuery.h"
#include "SQLInfo.h"
#include "SQLWrappers.h"
#include "SQLMessage.h"
#include <sqlext.h>
#include <atltrace.h>
#include <map>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{

// Used for metasearches
#define META_SEARCH_LEN 10
// This macro is used for synchronous ODBC calls
#define ODBC_CALL_ONCE(SQLFunc) \
  try \
  { \
  m_retCode = SQL_ERROR; \
  m_retCode = (SQLFunc); \
  } catch(StdException& ex)\
  {     ReThrowSafeException(ex);\
  }

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
  m_maxTableName        = 0;
  m_maxColumnName        = 0;
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
  m_maxCatalogName   = SQL_MAX_IDENTIFIER;
  m_maxSchemaName    = SQL_MAX_IDENTIFIER;
  m_maxColumnName    = SQL_MAX_IDENTIFIER;
  m_maxTableName     = SQL_MAX_IDENTIFIER;

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

  // Remove all datatypes info
  for(auto& type : m_dataTypes)
  {
    delete type.second;
  }
  m_dataTypes.clear();
}

void
SQLInfo::InfoMessageBox(XString p_message,UINT p_type /*= MB_OK*/)
{
  SQLMessage(NULL,p_message,"ODBC Driver info",p_type);
}

// Add an ODBC SQL Keyword
bool
SQLInfo::AddSQLWord(XString sql)
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
      ATLTRACE("Cannot determine which ODBC functions are supported. Proceed with caution!");
      ATLTRACE("In effect: Get a better driver, because this is a basic ODBC function!\n");
    }
  }
}

// Get a string from GetInfo with extra security checks for overflow
XString
SQLInfo::GetInfoString(SQLUSMALLINT info)
{
  SQLSMALLINT len  = 0;
  bool    overflow = false;
  XString answer;
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
    InfoMessageBox("Buffer overflow on ::SQLGetInfo()\n\r"
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
  m_initDone = true;

  SupportedODBCFunctions();
  
  if(!SupportedFunction(SQL_API_SQLGETINFO))
  {
    ATLTRACE("Cannot get ODBC info with SQLGetInfo. Cannot determine capabilities\n");
    ATLTRACE("In effect: Get a better driver, because this is a basic ODBC function!\n");
    return;
  }
  // STATIC KEYWORDS FROM ODBC 3.5 as of compilation
  if (m_ODBCKeywords.size() == 0)
  {
    woord[0] = '\0';
    pw = woord;
    for(pb = (char*) SQL_ODBC_KEYWORDS;*pb != '\0';pb++)
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
  m_maxColumnName        = GetInfoShortInteger(SQL_MAX_COLUMN_NAME_LEN);
  m_maxTableName        = GetInfoShortInteger(SQL_MAX_TABLE_NAME_LEN);
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
  m_schemaUsage         = GetInfoInteger(SQL_SCHEMA_USAGE);
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
  char     buffer[5120] = { 0 };

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
        XString key;
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
        buffer[0] = 0;
        if(::SQLGetData(handle,4,SQL_C_CHAR,buffer,5120,&dataLen) == SQL_SUCCESS)
        {
          if(dataLen > 0) ti->m_literal_prefix = buffer;
        }
        // LITERAL SUFFIX FOR ODBC DRIVER, like '} for timestamp
        buffer[0] = 0;
        if(::SQLGetData(handle,5,SQL_C_CHAR,buffer,5120,&dataLen) == SQL_SUCCESS)
        {
          if(dataLen > 0) ti->m_literal_suffix = buffer;
        }
        // HOW TO CREATE PARAMETERS, like "(precision,scale)"
        buffer[0] = 0;
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
        buffer[0] = 0;
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
SQLInfo::GetTypeInfo(int p_sqlDatatype,XString p_typename /*=""*/) const
{
  TypeInfo* result = nullptr;

  for(auto& type : m_dataTypes)
  {
    if(type.second->m_data_type == p_sqlDatatype)
    {
      result = type.second;
      if(p_typename.GetLength())
      {
        if(type.second->m_type_name.CompareNoCase(p_typename) == 0)
        {
          return type.second;
        }
      }
    }
  }
  return result;
}

// Returns the fact whether an API function is supported
// by the ODBC driver, regardless of ODBC version
bool
SQLInfo::SupportedFunction(unsigned int api_function)
{
  GetInfo();
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
SQLInfo::GetAttributeInteger(LPCTSTR description,SQLINTEGER attrib)
{
  SQLULEN    value = 0;
  SQLINTEGER cbMax = 0;
  
  SQLRETURN nRetCode = SQLGetConnectAttr(m_hdbc
                              ,attrib
                              ,(SQLPOINTER)&value
                                        ,sizeof(value)
                              ,&cbMax);
  if(!m_database->Check(nRetCode))
  {
    XString error;
    error.Format("Cannot get connection attribute \"%s\": ",description);
    error += m_database->GetErrorString(NULL);
    XString state = m_database->GetSQLState();
    if(state.CompareNoCase("S1C00") == 0 || // Driver not capable
       state.CompareNoCase("S1092") == 0 || // Option-id not supported on your database
       state.CompareNoCase("HYC00") == 0 )  // Optional feature not implemented
    {
      // Driver not capable to get/set this attribute
      ATLTRACE("%s\n",error.GetString());
      return -1;
    }
    InfoMessageBox(error);
    return 0;
  }
  if(cbMax != sizeof(value))
  {
    ATLTRACE("Attribute \"%s\" not supported on your database\n",description);
    return 0;
  }
  ATLTRACE("Database connection attribute \"%s\" was: %d\n",description,value);
  return (int) (value & 0xFFFF);
}

// Getting a general STRING connection attribute
XString
SQLInfo::GetAttributeString(XString description,SQLINTEGER attrib)
{
  SQLCHAR    value[MAX_BUFFER + 1];
  SQLINTEGER cbMax = 0;
  RETCODE    nRetCode = SQL_ERROR;
  nRetCode = ::SQLGetConnectAttr(m_hdbc
                                ,attrib
                                ,(SQLPOINTER)&value
                                ,MAX_BUFFER
                                ,&cbMax);
  if(!m_database->Check(nRetCode))
  {
    XString error;
    error.Format("Cannot get connection attribute \"%s\": ",description.GetString());
    error += m_database->GetErrorString(NULL);
    InfoMessageBox(error);
    return "";
  }
  value[cbMax] = 0;
  ATLTRACE("Database connection attribute \"%s\" was: %s\n",description.GetString(),value);
  return XString(value);
}

// Setting an INTEGER attribute
bool 
SQLInfo::SetAttributeInteger(XString     description
                            ,SQLINTEGER  attrib
                            ,SQLUINTEGER value)
{
  RETCODE nRetCode = SQL_ERROR;
  nRetCode = SqlSetConnectAttr(m_hdbc
                              ,attrib
                              ,(SQLPOINTER)(DWORD_PTR)value
                              ,SQL_IS_UINTEGER);
  if(!m_database->Check(nRetCode))
  {
    XString error;
    error.Format("Cannot set connection attribute \"%s\": ",description.GetString());
    error += m_database->GetErrorString(NULL);
    InfoMessageBox(error);
    return false;
  }
  ATLTRACE("Database connection attribute \"%s\" set to: %d\n",description.GetString(),value);
  return true;
}

// Setting a STRING attribute
bool 
SQLInfo::SetAttributeString(XString    description
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
    XString error;
    error.Format("Cannot set connection attribute \"%s\": ",description.GetString());
    error += m_database->GetErrorString(NULL);
    InfoMessageBox(error);
    return false;
  }
  ATLTRACE("Database connection attribute \"%s\" set to: %s\n",description.GetString(),value);
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
XString       
SQLInfo::GetAttributeTraceFile()
{
  return GetAttributeString("tracefile",SQL_ATTR_TRACEFILE);
}

// Getting the current catalog name
XString       
SQLInfo::GetAttributeCatalog()
{
  XString catalog = GetAttributeString("current-catalog",SQL_ATTR_CURRENT_CATALOG);
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
XString
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
SQLInfo::SetAttributeTraceFile(XString p_traceFile)
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
SQLInfo::SetAttributeTranslib(XString p_transLib)
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
SQLInfo::IsCorrectName(XString p_name,int p_type)
{
  if(p_name.IsEmpty())
  {
    return true;
  }

  // Cannot be too long for table or column
  if(m_maxTableName  && p_type == 0 && p_name.GetLength() > m_maxTableName)
  {
    return false;
  }
  if(m_maxColumnName && p_type == 1 && p_name.GetLength() > m_maxColumnName)
  {
    return false;
  }

  // Must be an identifier, complying to ODBC rules
  for(int ind = 0;ind < p_name.GetLength(); ++ind)
  {
    int ch = p_name.GetAt(ind);
    if (ch == ' ' || ( !isalnum(ch) && ch != '_' && strchr(m_specialChars,ch) == NULL ))
    {
      return false;
    }
  }

  // Cannot be in the ODBC keywords list or in de RDBMS extra reserved words list
  if(IsReservedWord(p_name))
  {
    return false;
  }
  // Name OK
  return true;
}

// Is reserved word
bool
SQLInfo::IsReservedWord(XString p_name)
{
  // Cannot be in the ODBC keywords list
  for(auto& word : m_ODBCKeywords)
  {
    if(p_name.CompareNoCase(word) == 0)
    {
      return true;
    }
  }

  // Cannot be in the RDBMS extra reserved words list
  for(auto& word : m_RDBMSkeywords)
  {
    if(p_name.CompareNoCase(word) == 0)
    {
      return true;
    }
  }
  return false;
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
SQLInfo::GetPrimaryKeyInfo(XString&    p_tablename
                          ,XString&    p_primary
                          ,MPrimaryMap& p_primaries)
{
  // Make sure we have the info
  GetInfo();

  // Reset
  p_primary = "";
  p_primaries.clear();

  MTableMap tables;
  XString   errors;

  MakeInfoTablePrimary(p_primaries,errors,"",p_tablename);
  if(!p_primaries.size())
  {
    // If no primary key found, search for the first unique key
    MIndicesMap statistics;
    MakeInfoTableStatistics(statistics,errors,"",p_tablename,&p_primaries,false);
  }
  else
  {
    // Return the constraint name separately
    p_primary = p_primaries.front().m_constraintName;
  }
  return (p_primaries.size() > 0);
}

// Make a statement handle for METADATA query
bool
SQLInfo::GetStatement(bool p_metadataID /*= true*/)
{
  // Create statement handle in m_hstmt:
  m_retCode = m_database->GetSQLHandle(&m_hstmt,FALSE);
  if (!SQL_SUCCEEDED(m_retCode))
  {
    XString errorText = "Error in ODBC statement: ";
    errorText += m_database->GetErrorString(m_hstmt);
    throw StdException(errorText);
  }
  SQLUINTEGER meta = p_metadataID ? SQL_TRUE : SQL_FALSE;
  // On Various ODBC databases metadata is or is not case-sensitive. To work around
  // these differences, the statement should be aware that it is about metadata!
  m_retCode = SqlSetStmtAttr(m_hstmt,SQL_ATTR_METADATA_ID,(SQLPOINTER)(DWORD_PTR)meta,SQL_IS_UINTEGER);
  if(!SQL_SUCCEEDED(m_retCode))
  {
    m_retCode = SqlSetConnectAttr(m_hdbc,SQL_ATTR_METADATA_ID,(SQLPOINTER)(DWORD_PTR)meta,SQL_IS_UINTEGER);
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
  }
}

// Get the catalog.schema.table from a user string
void
SQLInfo::GetObjectName(XString  p_pattern
                      ,XString& p_catalog
                      ,XString& p_schema
                      ,XString& p_table)
{
  p_catalog.Empty();
  p_schema.Empty();
  p_table.Empty();

  int pos = 0;

  // Search for tablename
  pos = p_pattern.ReverseFind('.');
  if(pos < 0)
  {
    // Only a table name
    p_table = p_pattern;
  }
  else
  {
    p_table = p_pattern.Right(p_pattern.GetLength()-pos-1).GetString();
    XString qualifier = p_pattern.Left(pos);
    pos = qualifier.ReverseFind('.');
    if(pos < 0)
    {
      // Only a schema name
      p_schema = qualifier.GetString();
    }
    else
    {
      // Split in catalog / schema
      p_schema  = qualifier.Right(qualifier.GetLength()-pos-1).GetString();
      p_catalog = qualifier.Left(pos);
    }
  }
  if(m_maxCatalogName && p_catalog.GetLength() > (int)m_maxCatalogName)
  {
    InfoMessageBox("Requested catalog name is longer than this ODBC database supports!",MB_OK);
  }
  if(m_maxSchemaName  && p_schema.GetLength() > (int)m_maxSchemaName)
  {
    InfoMessageBox("Requested schema name is longer than this ODBC database supports!",MB_OK);
  }
  if(m_maxTableName   && p_table.GetLength() > (int)m_maxTableName)
  {
    InfoMessageBox("Requested table name is longer than this ODBC database supports!",MB_OK);
  }
}

// Reprint the catalog.schema.table combination
// in system settings of the RDBMS.
// so can be: "X: schema.table@catalog"
// Or ANSI:   "X: catalog:schema.table"
XString
SQLInfo::MakeObjectName(SQLCHAR* search_catalog
                       ,SQLCHAR* search_schema
                       ,SQLCHAR* search_table
                       ,SQLCHAR* search_type)
{
  XString objectName;

  if(strlen((char*)search_schema))
  {
    objectName += XString(search_schema);
    objectName += ".";
  }
  if(strlen((char*)search_table))
  {
    objectName += XString(search_table);
  }
  if(search_catalog && strlen((char*)search_catalog))
  {
    XString separator = m_catalogNameSeparator;
    if(separator.IsEmpty())
    {
      separator = ":"; // ANSI separator
    }
    if(m_catalogLocation == SQL_CL_END)
    {
      objectName += separator + XString(search_catalog);
    }
    else // m_catalogLocation == SQL_CL_START
    {
      objectName = XString(search_catalog) + separator + objectName;
    }
  }
  if(search_type && strlen((char*)search_type))
  {
    objectName = XString(search_type) + ": " + objectName;
  }
  return objectName;
}

XString
SQLInfo::ODBCDataType(int DataType)
{
  XString type;

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
SQLInfo::MakeInfoTableTable(MTableMap& p_tables
                           ,XString&   p_errors
                           ,XString    p_schema
                           ,XString    p_tablename
                           ,XString    p_type)
{
  SQLCHAR      szCatalogName [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbCatalogName = 0;
  SQLCHAR      szSchemaName  [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbSchemaName  = 0;
  SQLCHAR      szTableName   [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbTableName   = 0;
  SQLCHAR      szTableType   [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbTableType   = 0;
  SQLCHAR      szRemarks     [2 * SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbRemarks     = 0;
  // Where to search
  unsigned char search_catalog[SQL_MAX_BUFFER] = { 0 };
  unsigned char search_schema [SQL_MAX_BUFFER] = { 0 };
  unsigned char search_table  [SQL_MAX_BUFFER] = { 0 };
  unsigned char search_type   [SQL_MAX_BUFFER] = { 0 };

  // Check whether we can do this
  if(!SupportedFunction(SQL_API_SQLTABLES))
  {
    p_errors = "SQLTables unsupported. Get a better ODBC driver!";
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
      case SQL_IC_UPPER:     p_schema.MakeUpper();   // E.g. Oracle/Firebird
                             p_tablename.MakeUpper();
                             break;
      case SQL_IC_LOWER:     p_schema.MakeLower();   // E.g. Informix/PostgreSQL
                             p_tablename.MakeLower();
                             break;
      case SQL_IC_SENSITIVE: // Nothing to be done, Catalog is treated case-insensitive
      case SQL_IC_MIXED:     // but is physically stored case-sensitive only
                             // e.g. MS-SQLServer / MS-Access / mySQL
      default:               if(m_METADATA_ID_unsupported && (m_METADATA_ID_errorseen == false))
                             {
                               InfoMessageBox("Cannot guarantee to find object '" + p_tablename + "' for one of the following reasons:\r\n"
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

  // Setting the search arguments
  search_catalog[0] = 0;
  strcpy_s((char*)search_schema,SQL_MAX_BUFFER,p_schema);
  strcpy_s((char*)search_table, SQL_MAX_BUFFER,p_tablename);
  strcpy_s((char*)search_type,  SQL_MAX_BUFFER,p_type);

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
         p_errors = m_database->GetErrorString(m_hstmt);
         InfoMessageBox(p_errors,MB_OK);
         if(m_retCode == SQL_ERROR)
         {
           return false;
         }
       }
       if(m_retCode == SQL_SUCCESS || m_retCode == SQL_SUCCESS_WITH_INFO)
       {
         MetaTable theTable;
         theTable.m_temporary = false;

         // Put primary info in the MetaTable object
         if(cbCatalogName > 0) theTable.m_catalog = szCatalogName;
         if(cbSchemaName  > 0) theTable.m_schema  = szSchemaName;
         if(cbTableName   > 0) theTable.m_table   = szTableName;
         if(cbRemarks     > 0) theTable.m_remarks = szRemarks;
         if(cbTableType   > 0) theTable.m_objectType  = szTableType;

         // Build "TYPE: catalog.schema.table" object type name
         theTable.m_fullName = MakeObjectName((SQLCHAR*)theTable.m_catalog.GetString()
                                                   ,(SQLCHAR*)theTable.m_schema.GetString()
                                                   ,(SQLCHAR*)theTable.m_table.GetString()
                                                   ,(SQLCHAR*)theTable.m_objectType.GetString());
         // Remember this table as a result
         p_tables.push_back(theTable);
       }
       else
       {
         break;
       }
     }
  }
  else
  {
    p_errors  = "Driver not capable to find table/view: ";
    p_errors += MakeObjectName(search_catalog,search_schema,search_table,search_type);

    p_errors += ". Error in ODBC statement: ";
    p_errors += m_database->GetErrorString(m_hstmt);
    }
  CloseStatement();
  return p_tables.size() > 0;
}

bool
SQLInfo::MakeInfoTableColumns(MColumnMap& p_columns
                             ,XString&    p_errors
                             ,XString     p_schema
                             ,XString     p_tablename
                             ,XString     p_columnname /*=""*/)
{
  SQLCHAR      szCatalogName [SQL_MAX_BUFFER+1] = { 0 };
  SQLLEN       cbCatalogName = 0;
  SQLCHAR      szSchemaName  [SQL_MAX_BUFFER+1] = { 0 };
  SQLLEN       cbSchemaName  = 0;
  SQLCHAR      szTableName   [SQL_MAX_BUFFER+1] = { 0 };
  SQLLEN       cbTableName   = 0;
  SQLCHAR      szColumnName  [SQL_MAX_BUFFER+1] = { 0 };
  SQLLEN       cbColumnName  = 0;
  SQLCHAR      szTypeName    [SQL_MAX_BUFFER+1] = { 0 };
  SQLLEN       cbTypeName    = 0;
  SQLCHAR      szRemarks     [2 * SQL_MAX_BUFFER + 1] = { 0 };
  SQLLEN       cbRemarks     = 0;
  SQLCHAR      szDefault     [2 * SQL_MAX_BUFFER + 1] = { 0 };
  SQLLEN       cbDefault     = 0;
  SQLCHAR      szNullable    [SQL_MAX_BUFFER+1] = { 0 };
  SQLLEN       cbIsNullable  = 0;
  SQLSMALLINT  DataType    = 0;
  SQLLEN       cbDataType    = 0;
  SQLINTEGER   Precision     = 0;
  SQLINTEGER   Length        = 0;
  SQLSMALLINT  Scale         = 0;
  SQLSMALLINT  Nullable      = 0;
  SQLSMALLINT  NumRadix      = 0;
  SQLSMALLINT  DataType3     = 0;
  SQLSMALLINT  TypeSub       = 0;
  SQLINTEGER   OctetLength   = 0;
  SQLINTEGER   Position      = 0;
  SQLLEN       cbPrecision   = 0;
  SQLLEN       cbLength      = 0;
  SQLLEN       cbScale       = 0;
  SQLLEN       cbNullable    = 0;
  SQLLEN       cbNumRadix    = 0;
  SQLLEN       cbDataType3   = 0;
  SQLLEN       cbTypeSub     = 0;
  SQLLEN       cbOctetLength = 0;
  SQLLEN       cbPosition    = 0;

  // Check whether we can do this
  if(!SupportedFunction(SQL_API_SQLCOLUMNS))
  {
    p_errors = "SQLColumns unsupported. Get a better ODBC driver!";
    return false;
  }

  CloseStatement();
  bool meta = GetStatement(false);

  if(m_METADATA_ID_unsupported || !meta)
  {
    // Oops: Cannot search directly on table name!
    switch(m_identifierCase)
    {
      case SQL_IC_UPPER:     p_schema.MakeUpper();   // E.g. Oracle/Firebird
                             p_tablename.MakeUpper();
                             p_columnname.MakeUpper();
                             break;
      case SQL_IC_LOWER:     p_schema.MakeLower();   // E.g. Informix/PostgreSQL
                             p_tablename.MakeLower();
                             p_columnname.MakeLower();
                             break;
      case SQL_IC_SENSITIVE: // Nothing to be done, Catalog is treated case-insensitive
      case SQL_IC_MIXED:     // but is physically stored case-sensitive only
                             // e.g. MS-SQLServer / MS-Access / mySQL
      default:               if(m_METADATA_ID_unsupported && (m_METADATA_ID_errorseen == false))
                             {
                               m_METADATA_ID_errorseen = true;
                             }
                             break;
    }
  }

  //strcpy_s((char*)szCatalogName,SQL_MAX_IDENTIFIER,m_searchCatalogName.GetString());
  szCatalogName[0] = 0;
  strcpy_s((char*)szSchemaName,SQL_MAX_BUFFER,p_schema.GetString());
  strcpy_s((char*)szTableName, SQL_MAX_BUFFER,p_tablename.GetString());
  strcpy_s((char*)szColumnName,SQL_MAX_BUFFER,p_columnname.GetString());

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
     SQLBindCol(m_hstmt,10, SQL_C_SHORT, &NumRadix,     2,              &cbNumRadix);
     SQLBindCol(m_hstmt,11, SQL_C_SSHORT,&Nullable,     2,              &cbNullable);
     SQLBindCol(m_hstmt,12, SQL_C_CHAR,   szRemarks,  2*SQL_MAX_BUFFER, &cbRemarks);
     SQLBindCol(m_hstmt,13, SQL_C_CHAR,   szDefault,  2*SQL_MAX_BUFFER, &cbDefault);
     SQLBindCol(m_hstmt,14, SQL_C_SHORT, &DataType3,    2,              &cbDataType3);
     SQLBindCol(m_hstmt,15, SQL_C_SHORT, &TypeSub,      2,              &cbTypeSub);
     SQLBindCol(m_hstmt,16, SQL_C_SLONG, &OctetLength,  4,              &cbOctetLength);
     SQLBindCol(m_hstmt,17, SQL_C_SLONG, &Position,     4,              &cbPosition);
     SQLBindCol(m_hstmt,18, SQL_C_CHAR,  &szNullable,   SQL_MAX_BUFFER, &cbIsNullable);

     while(true)
     {
       m_retCode = SqlFetch(m_hstmt);
       if(m_retCode == SQL_ERROR || m_retCode == SQL_SUCCESS_WITH_INFO)
       {
         p_errors = m_database->GetErrorString(m_hstmt);
         InfoMessageBox(p_errors,MB_OK);
         if(m_retCode == SQL_ERROR)
         {
           return false;
         }
       }
       if(m_retCode == SQL_SUCCESS || m_retCode == SQL_SUCCESS_WITH_INFO)
       {
         XString type;
         MetaColumn theColumn;

         // Fill in the structure
         if(cbCatalogName > 0) theColumn.m_catalog    = szCatalogName; // 1
         if(cbSchemaName  > 0) theColumn.m_schema     = szSchemaName;  // 2
         if(cbTableName   > 0) theColumn.m_table      = szTableName;   // 3
         if(cbColumnName  > 0) theColumn.m_column     = szColumnName;  // 4
         if(cbRemarks     > 0) theColumn.m_remarks    = szRemarks;     // 12
         if(cbDefault     > 0) theColumn.m_default    = szDefault;     // 13
         if(cbIsNullable  > 0) theColumn.m_isNullable = szNullable;    // 18
         // Numbers
         if(cbLength      > 0) theColumn.m_bufferLength = Length;      // 8
         if(cbNumRadix    > 0) theColumn.m_numRadix     = NumRadix;    // 10
         if(cbNullable    > 0) theColumn.m_nullable     = Nullable;    // 11
         if(cbDataType3   > 0) theColumn.m_datatype3    = DataType3;   // 14
         if(cbTypeSub     > 0) theColumn.m_sub_datatype = TypeSub;     // 15
         if(cbOctetLength > 0) theColumn.m_octet_length = OctetLength; // 16
         if(cbPosition    > 0) theColumn.m_position     = Position;    // 17
         if(cbDataType > 0)
         {
           theColumn.m_datatype = DataType;                            // 5
           type = ODBCDataType(DataType);
           if(cbTypeName > 0)
           {
             if(type.CompareNoCase((char*)szTypeName))
             {
               type = szTypeName;                                      // 6
             }
           }
         }
         if(cbDataType <= 0 && cbTypeName <= 0)
         {
           type = "UNKNOWN-TYPE";
         }
         theColumn.m_typename = type;

         // PRECISION and SCALE
         if(cbPrecision > 0 && Precision > 0)
         {
           theColumn.m_columnSize = Precision;                        // 7
           if(cbScale > 0)
           {
             theColumn.m_decimalDigits = Scale;                       // 9
           }
         }
         // Save the result
         p_columns.push_back(theColumn);
       }
       else
       {
         break;
       }
     }
  }
  else
  {
    p_errors  = "Driver not capable to find columns for: ";
    p_errors += MakeObjectName(catalog,schema,table,(unsigned char*)"");
    p_errors += ". Error in ODBC statement: ";
    p_errors += m_database->GetErrorString(m_hstmt);
  }
  CloseStatement();
  return p_columns.size() > 0;
}

bool
SQLInfo::MakeInfoTablePrimary(MPrimaryMap& p_primaries,XString& p_errors,XString p_schema,XString p_tablename)
{
  SQLCHAR      szCatalogName [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbCatalogName = 0;
  SQLCHAR      szSchemaName  [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbSchemaName  = 0;
  SQLCHAR      szTableName   [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbTableName   = 0;
  SQLCHAR      szColumnName  [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbColumnName  = 0;
  SWORD          KeySeq      = 0;
  SQLLEN       cbKeySeq      = 0;
  SQLCHAR      szPkName      [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbPkName      = 0;

  // Check whether we can do this
  if(!SupportedFunction(SQL_API_SQLPRIMARYKEYS))
  {
    p_errors = "SQLPrimaryKeys unsupported. Get a better ODBC driver!";
    return false;
  }
  szCatalogName[0] = 0;
  strcpy_s((char*)szSchemaName, SQL_MAX_IDENTIFIER,p_schema.GetString());
  strcpy_s((char*)szTableName,  SQL_MAX_IDENTIFIER,p_tablename.GetString());

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
         XString err = m_database->GetErrorString(m_hstmt);
         InfoMessageBox(err,MB_OK);
         if(m_retCode == SQL_ERROR)
         {
           return false;
         }
       }
       if(m_retCode == SQL_SUCCESS || m_retCode == SQL_SUCCESS_WITH_INFO)
       {
         MetaPrimary primary;
         primary.m_columnPosition = 0;

         if(cbColumnName > 0) primary.m_catalog        = szCatalogName;
         if(cbSchemaName > 0) primary.m_schema         = szSchemaName;
         if(cbTableName  > 0) primary.m_table          = szTableName;
         if(cbColumnName > 0) primary.m_columnName     = szColumnName;
         if(cbPkName     > 0) primary.m_constraintName = szPkName;
         if(cbKeySeq     > 0) primary.m_columnPosition = (int)KeySeq;

         p_primaries.push_back(primary);
       }
       else
       {
         break;
       }
     }
  }
  else
  {
    p_errors  = "Driver not capable to find primary key for: ";
    p_errors += MakeObjectName(catalog,schema,table,(unsigned char*)"");
    p_errors += ". Error in ODBC statement: ";
    p_errors += m_database->GetErrorString(m_hstmt);
    }
  CloseStatement();
  return p_primaries.size() > 0;
}

bool 
SQLInfo::MakeInfoTableForeign(MForeignMap& p_foreigns
                             ,XString&     p_errors
                             ,XString      p_schema
                             ,XString      p_tablename
                             ,bool         p_referenced /* = false */)
{
  SQLCHAR      szPKCatalogName [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbPKCatalogName = 0;
  SQLCHAR      szPKSchemaName  [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbPKSchemaName  = 0;
  SQLCHAR      szPKTableName   [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbPKTableName   = 0;
  SQLCHAR      szFKCatalogName [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbFKCatalogName = 0;
  SQLCHAR      szFKSchemaName  [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbFKSchemaName  = 0;
  SQLCHAR      szFKTableName   [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbFKTableName   = 0;
  SQLCHAR      szPKColumnName  [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbPKColumnName  = 0;
  SQLCHAR      szFKColumnName  [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbFKColumnName  = 0;
  SWORD          KeySeq        = 0;
  SQLLEN       cbKeySeq        = 0;
  SWORD          UpdateRule    = 0;
  SQLLEN       cbUpdateRule    = 0;
  SWORD          DeleteRule    = 0;
  SQLLEN       cbDeleteRule    = 0;
  SWORD          Deferrab      = 0;
  SQLLEN       cbDeferrab      = 0;
  SQLCHAR      szFKKeyName    [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbFKKeyName     = 0;
  SQLCHAR      szPKKeyName    [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbPKKeyName     = 0;

  // Check whether we can do this
  if(!SupportedFunction(SQL_API_SQLFOREIGNKEYS))
  {
    p_errors = "SQLForeignKeys unsupported. Get a better ODBC driver!";
    return false;
  }
  CloseStatement();
  bool meta = GetStatement(false);

  if(p_referenced)
  {
    szPKCatalogName[0] = 0;
    strcpy_s((char*)szPKSchemaName, SQL_MAX_BUFFER,p_schema.GetString());
    strcpy_s((char*)szPKTableName,  SQL_MAX_BUFFER,p_tablename.GetString());
    szFKCatalogName[0] = 0;
    szFKSchemaName [0] = 0;
    szFKTableName  [0] = 0;
  }
  else
  {
    szPKCatalogName[0] = 0;
    szPKSchemaName [0] = 0;
    szPKTableName  [0] = 0;
    szFKCatalogName[0] = 0;
    strcpy_s((char*)szFKSchemaName, SQL_MAX_BUFFER,p_schema.GetString());
    strcpy_s((char*)szFKTableName,  SQL_MAX_BUFFER,p_tablename.GetString());
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
         XString err = m_database->GetErrorString(m_hstmt);
         InfoMessageBox(err,MB_OK);
         if(m_retCode == SQL_ERROR)
         {
           return false;
         }
       }
       if(m_retCode == SQL_SUCCESS || m_retCode == SQL_SUCCESS_WITH_INFO)
       {
        MetaForeign foreign;

        // Primary table size
        if(cbPKCatalogName > 0) foreign.m_pkCatalogName = szPKCatalogName;
        if(cbPKSchemaName  > 0) foreign.m_pkSchemaName  = szPKSchemaName;
        if(cbPKTableName   > 0) foreign.m_pkTableName   = szPKTableName;
        if(cbPKColumnName  > 0) foreign.m_pkColumnName  = szPKColumnName;
        // Foreign table size
        if(cbFKCatalogName > 0) foreign.m_fkCatalogName = szFKCatalogName;
        if(cbFKSchemaName  > 0) foreign.m_fkSchemaName  = szFKSchemaName;
        if(cbFKTableName   > 0) foreign.m_fkTableName   = szFKTableName;
        if(cbFKColumnName  > 0) foreign.m_fkColumnName  = szFKColumnName;
        // Constraint names
        if(cbFKKeyName > 0) foreign.m_foreignConstraint = szFKKeyName;
        if(cbPKKeyName > 0) foreign.m_primaryConstraint = szPKKeyName;
        // Statuses of the foreign key relation
        foreign.m_keySequence = cbKeySeq     > 0 ? KeySeq     : 0;
        foreign.m_updateRule  = cbUpdateRule > 0 ? UpdateRule : 0;
        foreign.m_deleteRule  = cbDeleteRule > 0 ? DeleteRule : 0;
        foreign.m_deferrable  = cbDeferrab   > 0 ? Deferrab   : 0;
        foreign.m_match       = SQL_MATCH_FULL;
         // Keep the foreign key
        p_foreigns.push_back(foreign);
       }
       else
       {
         break;
       }
     }
  }
  else
  {
    p_errors  = "Driver not capable to find foreign keys for: ";
    p_errors += MakeObjectName((unsigned char*)"",(unsigned char*)p_schema.GetString(),(unsigned char*)p_tablename.GetString(),(unsigned char*)"");
    p_errors += ". Error in ODBC statement: ";
    p_errors += m_database->GetErrorString(m_hstmt);
  }
  CloseStatement();
  return p_foreigns.size() > 0;
}

bool 
SQLInfo::MakeInfoTableStatistics(MIndicesMap& p_statistics
                                ,XString&     p_errors
                                ,XString      p_schema
                                ,XString      p_tablename
                                ,MPrimaryMap*    p_keymap
                                ,bool            p_all /*=true*/)
{
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
  SQLCHAR        AscDesc     [10] = "";
  SQLLEN       cbAscDesc     = 0;
  SQLINTEGER     Cardinality = 0;
  SQLLEN       cbCardinality = 0;
  SQLINTEGER     Pages       = 0;
  SQLLEN       cbPages       = 0;
  SQLCHAR      szFilter      [SQL_MAX_BUFFER + 1] = "";
  SQLLEN       cbFilter      = 0;
  SQLUSMALLINT searchType    = p_all ? SQL_INDEX_ALL : SQL_INDEX_UNIQUE;

  // Check whether we can do this
  if(!SupportedFunction(SQL_API_SQLSTATISTICS))
  {
    p_errors = "SQLStatistics unsupported. Get a better ODBC driver!";
    return true;
  }
  szCatalogName[0] = 0;
  strcpy_s((char*)szSchemaName, SQL_MAX_BUFFER,p_schema.GetString());
  strcpy_s((char*)szTableName,  SQL_MAX_BUFFER,p_tablename.GetString());

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
        XString err = m_database->GetErrorString(m_hstmt);
        InfoMessageBox(err,MB_OK);
        if(m_retCode == SQL_ERROR)
        {
          return false;
        }
      }
      if(m_retCode == SQL_SUCCESS || m_retCode == SQL_SUCCESS_WITH_INFO)
      {
        MetaIndex stat;
        // Names
        if(cbCatalogName > 0) stat.m_catalogName = szCatalogName;
        if(cbSchemaName  > 0) stat.m_schemaName  = szSchemaName;
        if(cbTableName   > 0) stat.m_tableName   = szTableName;
        if(cbIndexName   > 0) stat.m_indexName   = szIndexName;
        if(cbColumnName  > 0) stat.m_columnName  = szColumnName;
        if(cbFilter      > 0) stat.m_filter      = szFilter;
        if(cbAscDesc     > 0) stat.m_ascending   = AscDesc;
        // Numbers
        stat.m_nonunique   = cbNonUnique   > 0 ? NonUnique   : false;
        stat.m_indexType   = cbIndexType   > 0 ? IndexType   : 0;
        stat.m_position    = cbOrdinalPos  > 0 ? OrdinalPos  : 0;
        stat.m_cardinality = cbCardinality > 0 ? Cardinality : 0;
        stat.m_pages       = cbPages       > 0 ? Pages       : 0;

        p_statistics.push_back(stat);

        if(p_keymap)
            {
              // Record the columns of the unique key
          MetaPrimary primary;
          primary.m_catalog         = szCatalogName;
          primary.m_schema          = szSchemaName;
          primary.m_table           = szTableName;
          primary.m_columnName      = XString(szColumnName);
          primary.m_columnPosition  = OrdinalPos;
          primary.m_constraintName  = szIndexName;
          p_keymap->push_back(primary);
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
    p_errors  = "Driver not capable to find statistics for: ";
    p_errors += MakeObjectName(catalog,schema,table,(unsigned char*)"");
    p_errors += ". Error in ODBC statement: ";
    p_errors += m_database->GetErrorString(m_hstmt);
    }
  CloseStatement();
  return p_statistics.size() > 0;
}

bool 
SQLInfo::MakeInfoTableSpecials(MSpecialsMap& p_specials
                              ,XString&      p_errors
                              ,XString       p_schema
                              ,XString       p_tablename)
  {
  SQLCHAR      szCatalogName [SQL_MAX_BUFFER] = { 0 };
  SQLCHAR      szSchemaName  [SQL_MAX_BUFFER] = { 0 };
  SQLCHAR      szTableName   [SQL_MAX_BUFFER] = { 0 };
  SQLCHAR      szColumnName  [SQL_MAX_BUFFER] = { 0 };
  SQLCHAR      szTypeName    [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbColumnName  = 0;
  SQLLEN       cbTypeName    = 0;
  SWORD          Scope       = 0;
  SQLLEN       cbScope       = 0;
  SWORD          Pseudo      = 0;
  SQLLEN       cbPseudo      = 0;
  SQLSMALLINT    DataType    = 0;
  SQLLEN       cbDataType    = 0;
  SQLINTEGER     ColumnSize  = 0;
  SQLLEN       cbColumnSize  = 0;
  SQLINTEGER     BufferSize  = 0;
  SQLLEN       cbBufferSize  = 0;
  SQLSMALLINT    DecDigits   = 0;
  SQLLEN       cbDecDigits   = 0;

  // Check whether we can do this
  if(!SupportedFunction(SQL_API_SQLSPECIALCOLUMNS))
  {
    p_errors = "SQLSpecialColumns unsupported. Get a better ODBC driver!";
    return false;
  }
  szCatalogName[0] = 0;
  strcpy_s((char*)szSchemaName, SQL_MAX_BUFFER,p_schema.GetString());
  strcpy_s((char*)szTableName,  SQL_MAX_BUFFER,p_tablename.GetString());

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
     SQLBindCol(m_hstmt, 3, SQL_C_SHORT, &DataType,   0,              &cbDataType);
     SQLBindCol(m_hstmt, 4, SQL_C_CHAR,  szTypeName,  SQL_MAX_BUFFER, &cbTypeName);
     SQLBindCol(m_hstmt, 5, SQL_C_LONG,  &ColumnSize, 0,              &cbColumnSize);
     SQLBindCol(m_hstmt, 6, SQL_C_LONG,  &BufferSize, 0,              &cbBufferSize);
     SQLBindCol(m_hstmt, 7, SQL_C_SHORT, &DecDigits,  0,              &cbDecDigits);
     SQLBindCol(m_hstmt, 8, SQL_C_SHORT, &Pseudo,     0,              &cbPseudo);
     while(true)
     {
       m_retCode = SqlFetch(m_hstmt);
       if(m_retCode == SQL_ERROR || m_retCode == SQL_SUCCESS_WITH_INFO)
       {
         XString err = m_database->GetErrorString(m_hstmt);
         InfoMessageBox(err,MB_OK);
         if(m_retCode == SQL_ERROR)
         {
           return false;
         }
       }
       if(m_retCode == SQL_SUCCESS || m_retCode == SQL_SUCCESS_WITH_INFO)
       {
         MetaSpecialColumn special;

         // Strings
         if(cbColumnName > 0) special.m_columnName = szColumnName;
         if(cbTypeName   > 0) special.m_typeName   = szTypeName;
         // Numbers
         special.m_scope         = cbScope      > 0 ? Scope      : 0;
         special.m_datatype      = cbDataType   > 0 ? DataType   : 0;
         special.m_pseudo        = cbPseudo     > 0 ? Pseudo     : 0;
         special.m_columnSize    = cbColumnSize > 0 ? ColumnSize : 0;
         special.m_bufferSize    = cbBufferSize > 0 ? BufferSize : 0;
         special.m_decimalDigits = cbDecDigits  > 0 ? DecDigits  : 0;
         // Keep record
         p_specials.push_back(special);
       }
       else
       {
         break;
       }
     }
  }
  else
  {
    p_errors  = "Driver not capable to find specials for: ";
    p_errors += MakeObjectName(catalog,schema,table,(unsigned char*)"");
    p_errors += ". Error in ODBC statement: ";
    p_errors += m_database->GetErrorString(m_hstmt);
  }
  CloseStatement();
  return true;
}

bool 
SQLInfo::MakeInfoTablePrivileges(MPrivilegeMap& p_privileges
                                ,XString&       p_errors
                                ,XString        p_schema
                                ,XString        p_tablename)
{
  SQLCHAR      szCatalogName [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbCatalogName = 0;
  SQLCHAR      szSchemaName  [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbSchemaName  = 0;
  SQLCHAR      szTableName   [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbTableName   = 0;
  SQLCHAR      szGrantor     [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbGrantor     = 0;
  SQLCHAR      szGrantee     [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbGrantee     = 0;
  SQLCHAR      szPrivilege   [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbPrivilege   = 0;
  SQLCHAR      szGrantable   [10] = { 0 };
  SQLLEN       cbGrantable   = 0;

  // Check whether we can do this
  if(!SupportedFunction(SQL_API_SQLTABLEPRIVILEGES))
  {
    p_errors = "SQLTablePrivileges unsupported. Get a better ODBC driver!";
    return false;
  }
  szCatalogName[0] = 0;
  strcpy_s((char*)szSchemaName, SQL_MAX_BUFFER,p_schema.GetString());
  strcpy_s((char*)szTableName,  SQL_MAX_BUFFER,p_tablename.GetString());

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
         XString err = m_database->GetErrorString(m_hstmt);
         InfoMessageBox(err,MB_OK);
         if(m_retCode == SQL_ERROR)
         {
           break;
         }
       }
       if(m_retCode == SQL_SUCCESS || m_retCode == SQL_SUCCESS_WITH_INFO)
       {
         MetaPrivilege priv;

         if(cbCatalogName > 0) priv.m_catalogName = szCatalogName;
         if(cbSchemaName  > 0) priv.m_schemaName  = szSchemaName;
         if(cbTableName   > 0) priv.m_tableName   = szTableName;
         if(cbGrantor     > 0) priv.m_grantor     = szGrantor;
         if(cbGrantee     > 0) priv.m_grantee     = szGrantee;
         if(cbPrivilege   > 0) priv.m_privilege   = szPrivilege;

         priv.m_grantable = false;
         if(cbGrantable > 0)
         {
           if(_stricmp((char*)szGrantable,"YES") == 0)
           {
             priv.m_grantable = true;
           }
         }
         // Keep record
         p_privileges.push_back(priv);
       }
       else
       {
         break;
       }
     }
  }
  else
  {
    p_errors  = "Driver not capable to find privileges for: ";
    p_errors += MakeObjectName(catalog,schema,table,(SQLCHAR*)"");
    p_errors += ". Error in ODBC statement: ";
    p_errors += m_database->GetErrorString(m_hstmt);
  }
  CloseStatement();
  return p_privileges.size() > 0;
}

bool 
SQLInfo::MakeInfoColumnPrivileges(MPrivilegeMap&  p_privileges
                                 ,XString&        p_errors
                                 ,XString         p_schema
                                 ,XString         p_tablename
                                 ,XString         p_columnname /*= ""*/)
{
  SQLCHAR      szCatalogName [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbCatalogName = 0;
  SQLCHAR      szSchemaName  [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbSchemaName  = 0;
  SQLCHAR      szTableName   [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbTableName   = 0;
  SQLCHAR      szColumnName  [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbColumnName  = 0;
  SQLCHAR      szGrantor     [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbGrantor     = 0;
  SQLCHAR      szGrantee     [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbGrantee     = 0;
  SQLCHAR      szPrivilege   [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbPrivilege   = 0;
  SQLCHAR      szGrantable   [10] = { 0 };
  SQLLEN       cbGrantable   = 0;

  // Check whether we can do this
  if(!SupportedFunction(SQL_API_SQLCOLUMNPRIVILEGES))
  {
    p_errors = "SQLColumnPrivileges unsupported. Get a better ODBC driver!";
    return false;
  }
  szCatalogName[0] = 0;
  strcpy_s((char*)szSchemaName, SQL_MAX_BUFFER,p_schema.GetString());
  strcpy_s((char*)szTableName,  SQL_MAX_BUFFER,p_tablename.GetString());
  strcpy_s((char*)szColumnName, SQL_MAX_BUFFER,p_columnname.GetString());

  CloseStatement();
  bool meta = GetStatement(false);

  unsigned char* catalog = GetMetaPointer(szCatalogName,meta);
  unsigned char* schema  = GetMetaPointer(szSchemaName, meta);
  unsigned char* table   = GetMetaPointer(szTableName,  meta);
  unsigned char* column  = GetMetaPointer(szColumnName, meta);

  m_retCode = SQL_ERROR;
  ODBC_CALL_ONCE(SQLColumnPrivileges(m_hstmt
                                    ,catalog                  // Catalog name to search for
                                    ,catalog ? SQL_NTS : 0    // Catalog name length
                                    ,schema                   // Schema name to search for
                                    ,schema  ? SQL_NTS : 0    // Schema name length
                                    ,table                    // Table Name to search for
                                    ,table   ? SQL_NTS : 0    // Table name length
                                    ,column                   // Column name to search for
                                    ,column  ? SQL_NTS : 0    // Column name length
                                   ));
  if(m_retCode == SQL_SUCCESS)
  {
     SQLBindCol(m_hstmt, 1, SQL_C_CHAR, szCatalogName,SQL_MAX_BUFFER, &cbCatalogName);
     SQLBindCol(m_hstmt, 2, SQL_C_CHAR, szSchemaName, SQL_MAX_BUFFER, &cbSchemaName);
     SQLBindCol(m_hstmt, 3, SQL_C_CHAR, szTableName,  SQL_MAX_BUFFER, &cbTableName);
     SQLBindCol(m_hstmt, 4, SQL_C_CHAR, szColumnName, SQL_MAX_BUFFER, &cbColumnName);
     SQLBindCol(m_hstmt, 5, SQL_C_CHAR, szGrantor,    SQL_MAX_BUFFER, &cbGrantor);
     SQLBindCol(m_hstmt, 6, SQL_C_CHAR, szGrantee,    SQL_MAX_BUFFER, &cbGrantee);
     SQLBindCol(m_hstmt, 7, SQL_C_CHAR, szPrivilege,  SQL_MAX_BUFFER, &cbPrivilege);
     SQLBindCol(m_hstmt, 8, SQL_C_CHAR, szGrantable,  SQL_MAX_BUFFER, &cbGrantable);
     while(true)
     {
       m_retCode = SqlFetch(m_hstmt);
       if(m_retCode == SQL_ERROR || m_retCode == SQL_SUCCESS_WITH_INFO)
       {
         XString err = m_database->GetErrorString(m_hstmt);
         InfoMessageBox(err,MB_OK);
         if(m_retCode == SQL_ERROR)
         {
           break;
         }
       }
       if(m_retCode == SQL_SUCCESS || m_retCode == SQL_SUCCESS_WITH_INFO)
       {
         MetaPrivilege priv;

         if(cbCatalogName > 0) priv.m_catalogName = szCatalogName;
         if(cbSchemaName  > 0) priv.m_schemaName  = szSchemaName;
         if(cbTableName   > 0) priv.m_tableName   = szTableName;
         if(cbColumnName  > 0) priv.m_columnName  = szColumnName;
         if(cbGrantor     > 0) priv.m_grantor     = szGrantor;
         if(cbGrantee     > 0) priv.m_grantee     = szGrantee;
         if(cbPrivilege   > 0) priv.m_privilege   = szPrivilege;

         priv.m_grantable = false;
         if(cbGrantable > 0)
         {
           if(_stricmp((char*)szGrantable,"YES") == 0)
           {
             priv.m_grantable = true;
           }
         }
         // Keep record
         p_privileges.push_back(priv);
       }
       else
       {
         break;
       }
     }
  }
  else
  {
    p_errors  = "Driver not capable to find privileges for: ";
    p_errors += MakeObjectName(catalog,schema,table,column);
    p_errors += ". Error in ODBC statement: ";
    p_errors += m_database->GetErrorString(m_hstmt);
  }
  CloseStatement();
  return p_privileges.size() > 0;
}

bool
SQLInfo::MakeInfoPSMProcedures(MProcedureMap&  p_procedures
                              ,XString&        p_errors
                              ,XString         p_schema
                              ,XString         p_procedure)
{
  SQLCHAR      szCatalogName     [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbCatalogName     = 0;
  SQLCHAR      szSchemaName      [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbSchemaName      = 0;
  SQLCHAR      szProcedureName   [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbProcedureName   = 0;
  SQLSMALLINT    NumInputParams  = 0;  // Unreliable
  SQLLEN       cbNumInputParams  = 0;
  SQLSMALLINT    NumOutputParams = 0;  // Unreliable
  SQLLEN       cbNumOutputParams = 0;
  SQLSMALLINT    NumResultSets   = 0;  // Unreliable
  SQLLEN       cbNumResultSets   = 0;
  SQLCHAR      szRemarks         [2 * SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbRemarks         = 0;
  SQLSMALLINT    ProcedureType   = 0;
  SQLLEN       cbProcedureType   = 0;
  bool         findAll = false;

  // Check whether we can do this
  if(!SupportedFunction(SQL_API_SQLPROCEDURES))
  {
    p_errors = "SQLProcedures unsupported. Get a better ODBC driver!";
    return false;
  }
  if(m_METADATA_ID_unsupported)
  {
    // Oops: Cannot search directly on table name!
    switch(m_identifierCase)
    {
      case SQL_IC_UPPER:     p_schema.MakeUpper();
                             p_procedure.MakeUpper();
                             break;
      case SQL_IC_LOWER:     p_schema.MakeUpper();
                             p_procedure.MakeLower();
                             break;
    }
  }
  // Split name in a maximum of three parts
  szCatalogName[0] = 0;
  strcpy_s((char*)szSchemaName,   SQL_MAX_BUFFER,p_schema.GetString());
  strcpy_s((char*)szProcedureName,SQL_MAX_BUFFER,p_procedure.GetString());
  if(p_procedure == "%")
  {
    findAll = true;
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
        break;
      }
      if(m_retCode == SQL_ERROR || m_retCode == SQL_SUCCESS_WITH_INFO)
      {
        XString err = m_database->GetErrorString(m_hstmt);
        InfoMessageBox(err,MB_OK);
        if(m_retCode == SQL_ERROR)
        {
          break;
        }
      }
      if(m_retCode == SQL_SUCCESS || m_retCode == SQL_SUCCESS_WITH_INFO)
      {
        MetaProcedure proc;
        // Strings
        if(cbCatalogName   > 0) proc.m_catalogName   = szCatalogName;
        if(cbSchemaName    > 0) proc.m_schemaName    = szSchemaName;
        if(cbProcedureName > 0) proc.m_procedureName = szProcedureName;
        if(cbRemarks       > 0) proc.m_remarks       = szRemarks;
        // Numbers
        proc.m_inputParameters  = cbNumInputParams  > 0 ? NumInputParams  : 0;
        proc.m_outputParameters = cbNumOutputParams > 0 ? NumOutputParams : 0;
        proc.m_resultSets       = cbNumResultSets   > 0 ? NumResultSets   : 0;
        proc.m_procedureType    = cbProcedureType   > 0 ? ProcedureType   : 0;
        // Keep the record
        p_procedures.push_back(proc);

      }
      else
      {
        break;
      }
    }
  }
  else
  {
    p_errors  = "Driver not capable to find procedures for: ";
    p_errors += MakeObjectName(catalog,schema,procedure,(SQLCHAR*)"");
    p_errors += ". Error in ODBC statement: ";
    p_errors += m_database->GetErrorString(m_hstmt);
  }
  CloseStatement();
  return p_procedures.size() > 0;
}

bool
SQLInfo::MakeInfoPSMParameters(MParameterMap& p_parameters
                              ,XString&       p_errors
                              ,XString        p_schema
                              ,XString        p_procedure)
{
  SQLCHAR      szCatalogName     [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbCatalogName     = 0;
  SQLCHAR      szSchemaName      [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbSchemaName      = 0;
  SQLCHAR      szProcedureName   [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbProcedureName   = 0;
  SQLCHAR      szColumnName      [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbColumnName      = 0;
  SQLSMALLINT    ColumnType      = 0;
  SQLLEN       cbColumnType      = 0;
  SQLSMALLINT    DataType        = 0;
  SQLLEN       cbDataType        = 0;
  SQLCHAR      szTypeName        [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbTypeName        = 0;
  SQLINTEGER     ColumnSize      = 0;
  SQLINTEGER     BufferSize      = 0;
  SQLSMALLINT    DecimalDigits   = 0;
  SQLSMALLINT    Radix           = 0;
  SQLLEN       cbDecimalDigits   = 0;
  SQLLEN       cbColumnSize      = 0;
  SQLLEN       cbBufferSize      = 0;
  SQLLEN       cbRadix           = 0;
  SQLSMALLINT    Nullable        = 0;
  SQLLEN       cbNullable        = 0;
  SQLCHAR      szRemarks         [2 * SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbRemarks         = 0;
  SQLCHAR      szDefaultValue    [2 * SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbDefaultValue    = 0;
  SQLINTEGER     OrdinalPos      = 0;
  SQLLEN       cbOrdinalPos      = 0;
  SQLCHAR      szIsNullable      [10] = { 0 };
  SQLLEN       cbIsNullable      = 0;
  SQLSMALLINT    DataType3       = 0;
  SQLLEN       cbDataType3       = 0;
  SQLSMALLINT    SubType         = 0;
  SQLLEN       cbSubType         = 0;
  SQLINTEGER     OctetLength     = 0;
  SQLLEN       cbOctetLength     = 0;

  // Check whether we can do this
  if(!SupportedFunction(SQL_API_SQLPROCEDURECOLUMNS))
  {
    p_errors = "SQLProcedureColumns unsupported. Get a better ODBC driver!";
    return false;
  }
  // Init search arguments
  szCatalogName[0] = 0;
  strcpy_s((char*)szSchemaName,   SQL_MAX_BUFFER,p_schema.GetString());
  strcpy_s((char*)szProcedureName,SQL_MAX_BUFFER,p_procedure.GetString());
  szColumnName[0] = 0;

  CloseStatement();
  bool meta = GetStatement();

  unsigned char* catalog   = GetMetaPointer(szCatalogName,  meta);
  unsigned char* schema    = GetMetaPointer(szSchemaName,   meta);
  unsigned char* procedure = GetMetaPointer(szProcedureName,meta);
  unsigned char* column    = GetMetaPointer(szColumnName,   meta);

  ODBC_CALL_ONCE(SQLProcedureColumns(m_hstmt
                                    ,catalog
                                    ,catalog ? SQL_NTS : 0
                                    ,schema
                                    ,schema  ? SQL_NTS : 0
                                    ,procedure,SQL_NTS
                                    ,column   ,0)); // All columns
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
    SQLBindCol(m_hstmt, 9, SQL_C_LONG,  &BufferSize,   0,              &cbBufferSize);
    SQLBindCol(m_hstmt,10, SQL_C_SSHORT,&DecimalDigits,0,              &cbDecimalDigits);
    SQLBindCol(m_hstmt,11, SQL_C_SSHORT,&Radix,        0,              &cbRadix);
    SQLBindCol(m_hstmt,12, SQL_C_SSHORT,&Nullable,     0,              &cbNullable);
    SQLBindCol(m_hstmt,13, SQL_C_CHAR, szRemarks,     2*SQL_MAX_BUFFER,&cbRemarks);
    SQLBindCol(m_hstmt,14, SQL_C_CHAR, szDefaultValue,2*SQL_MAX_BUFFER,&cbDefaultValue);
    SQLBindCol(m_hstmt,15, SQL_C_SSHORT,&DataType3,    2,              &cbDataType3);
    SQLBindCol(m_hstmt,16, SQL_C_SSHORT,&SubType,      2,              &cbSubType);
    SQLBindCol(m_hstmt,17, SQL_C_LONG,  &OctetLength,  4,              &cbOctetLength);
    SQLBindCol(m_hstmt,18, SQL_C_LONG,  &OrdinalPos,   0,              &cbOrdinalPos);
    SQLBindCol(m_hstmt,19, SQL_C_CHAR, szIsNullable,   10,             &cbIsNullable);
    while(true)
    {
      m_retCode = SqlFetch(m_hstmt);
      if(m_retCode == SQL_ERROR || m_retCode == SQL_SUCCESS_WITH_INFO)
      {
        XString err = m_database->GetErrorString(m_hstmt);
        InfoMessageBox(err,MB_OK);
        if(m_retCode == SQL_ERROR)
        {
          return false;
        }
      }
      if(m_retCode == SQL_SUCCESS || m_retCode == SQL_SUCCESS_WITH_INFO)
      {
        MetaParameter par;
        // Strings
        if(cbCatalogName   > 0) par.m_catalog    = szCatalogName;
        if(cbSchemaName    > 0) par.m_schema     = szSchemaName;
        if(cbProcedureName > 0) par.m_procedure  = szProcedureName;
        if(cbColumnName    > 0) par.m_parameter  = szColumnName;
        if(cbTypeName      > 0) par.m_typeName      = szTypeName;
        if(cbRemarks       > 0) par.m_remarks       = szRemarks;
        if(cbDefaultValue  > 0) par.m_default    = szDefaultValue;
        if(cbIsNullable    > 0) par.m_isNullable    = szIsNullable;
        // Numbers
        par.m_columnType    = cbColumnType    > 0 ? ColumnType    : 0;
        par.m_datatype      = cbDataType      > 0 ? DataType      : 0;
        par.m_columnSize    = cbColumnSize    > 0 ? ColumnSize    : 0;
        par.m_bufferLength  = cbBufferSize    > 0 ? BufferSize    : 0;
        par.m_decimalDigits = cbDecimalDigits > 0 ? DecimalDigits : 0;
        par.m_numRadix      = cbRadix         > 0 ? Radix         : 0;
        par.m_nullable      = cbNullable      > 0 ? Nullable      : 0;
        par.m_position      = cbOrdinalPos    > 0 ? OrdinalPos    : 0;
        par.m_datatype3     = cbDataType3     > 0 ? DataType3     : 0;
        par.m_subType       = cbSubType       > 0 ? SubType       : 0;
        par.m_octetLength   = cbOctetLength   > 0 ? OctetLength   : 0;

        // Keep record
        p_parameters.push_back(par);
      }
      else
      {
        break;
      }
    }
  }
  else
  {
    p_errors  = "Driver not capable to find procedures columns for: ";
    p_errors += MakeObjectName(catalog,schema,procedure,(SQLCHAR*)"");
    p_errors += ". Error in ODBC statement: ";
    p_errors += m_database->GetErrorString(m_hstmt);
  }
  CloseStatement();
  return p_parameters.size() > 0;
}

// Return the native SQL command from an ODBC-escaped command
XString 
SQLInfo::NativeSQL(HDBC hdbc,XString& sqlCommand)
{
  // Check whether we can do this
  if(!SupportedFunction(SQL_API_SQLNATIVESQL))
  {
    InfoMessageBox("SQLNativeSQL unsupported. Get a better ODBC driver!",MB_OK|MB_ICONEXCLAMATION);
    return "";
  }
  // In case we need to get the HDBC handle from the database object
  if(!hdbc && m_database)
  {
    hdbc = m_database->GetDBHandle();
  }
  if(!hdbc)
  {
    throw StdException("NativeSQL called without an opened database!");
  }

  SQLINTEGER retLen = 0;
  SQLINTEGER buflen = sqlCommand.GetLength() * 2;
  SQLCHAR*   buffer = new SQLCHAR[(size_t)buflen + 1];

  // Perform the conversion call
  m_retCode = SqlNativeSql(hdbc
                         ,(SQLCHAR *)sqlCommand.GetString()
                         ,sqlCommand.GetLength()
                         ,buffer
                         ,buflen
                         ,&retLen);
  if(m_retCode == SQL_SUCCESS)
  {
    if(retLen >= 0 && retLen <= buflen)
    {
      buffer[retLen] = 0;
      XString result((char*) buffer);
      delete[] buffer;
      return result;
    }
    else
    {
      // Overflow error
      XString error = "Buffer overflow (30.000 chars) on SQLNativeSQL";
      InfoMessageBox(error,MB_OK|MB_ICONERROR);
      delete[] buffer;
      return error;
    }
  }
  // SQLNativeSQL returned an error
  XString errorText = "Error while retrieving SQLNativeSQL:\n";
  errorText += m_database->GetErrorString(NULL);
  InfoMessageBox(errorText,MB_OK|MB_ICONERROR);
  delete[] buffer;
  return errorText;
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
SQLInfo::MakeInfoMetaTypes(MMetaMap& p_objects,XString& p_errors,int p_type)
{
  XString      sitem;
  SQLCHAR      szCatalogName [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbCatalogName = 0;
  SQLCHAR      szSchemaName  [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbSchemaName  = 0;
  SQLCHAR      szTableType   [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbTableType   = 0;
  SQLCHAR      szRemarks     [SQL_MAX_BUFFER] = { 0 };
  SQLLEN       cbRemarks     = 0;
  // Where to search
  unsigned char search_catalog[META_SEARCH_LEN] = "";
  unsigned char search_schema [META_SEARCH_LEN] = "";
  unsigned char search_table  [META_SEARCH_LEN] = "";
  unsigned char search_type   [META_SEARCH_LEN] = "";
  // For duplicates
  std::map<XString,XString> found;
  char* nameFound = NULL;

  // Check whether we can do this
  if(!SupportedFunction(SQL_API_SQLTABLES))
  {
    p_errors = "SQLTables unsupported. Get a better ODBC driver!";
    return false;
  }
  // Get a statement handle for metadata use
  CloseStatement();
  bool meta = GetStatement();

  switch(p_type)
  {
    case META_CATALOGS: strcpy_s((char*)search_catalog,META_SEARCH_LEN,SQL_ALL_CATALOGS);     break;
    case META_SCHEMAS:  strcpy_s((char*)search_schema, META_SEARCH_LEN,SQL_ALL_SCHEMAS);      break;
    case META_TABLES:   strcpy_s((char*)search_type,   META_SEARCH_LEN,SQL_ALL_TABLE_TYPES);  break;
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
        MetaObject object;

        object.m_objectType = p_type;
        switch(p_type)
        {
          case META_CATALOGS: if(cbCatalogName > 0) nameFound = (char*)szCatalogName;
                              break;
          case META_SCHEMAS:  if(cbSchemaName  > 0) nameFound = (char*)szSchemaName;
                              break;
          case META_TABLES:   if(cbTableType   > 0) nameFound = (char*)szTableType;
                              break;
        }
        if(cbRemarks > 0)
        {
          object.m_remarks = szRemarks;
        }
        if(nameFound)
        {
          XString val;
          if(found.find(nameFound) == found.end())
          {
            found.insert(std::make_pair(nameFound,nameFound));
            object.m_objectName = nameFound;
            p_objects.push_back(object);
          }
        }
      }
      else if(m_retCode == SQL_NO_DATA)
      {
        switch(p_type)
        {
          case META_CATALOGS: m_canFindCatalogs = false; 
                              break;
          case META_SCHEMAS:  m_canFindSchemas  = false; 
                              break;
          case META_TABLES:   m_canFindTypes    = false;
                              break;
          default:            break;
        }
        break;
      }
    }
  }
  if(m_retCode == SQL_ERROR)
  {
    p_errors  = "Driver not capable to find meta-objects";
    p_errors += ". Error in ODBC statement: ";
    p_errors += m_database->GetErrorString(m_hstmt);

    if(p_errors.Find("HYC00") >= 0)
    {
      switch(p_type)
      {
        case META_CATALOGS: m_canFindCatalogs = false; 
                            break;
        case META_SCHEMAS:  m_canFindSchemas  = false; 
                            break;
        case META_TABLES:   m_canFindTypes    = false;
                            break;
        default:            break;
      }
    }
  }
  CloseStatement();
  return p_objects.size() > 0;
}

// End of namespace
}
