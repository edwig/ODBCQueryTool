////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoOracle.cpp
//
// Copyright (c) 1998-2024 ir. W.E. Huisman
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
#include "SQLInfoOracle.h"
#include "SQLQuery.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{

// Constructor
SQLInfoOracle::SQLInfoOracle(SQLDatabase* p_database)
              :SQLInfoDB(p_database)
{
}

// Destructor. Does nothing
SQLInfoOracle::~SQLInfoOracle()
{
}

// RDBMS Uses INDENTITY or SEQUENCE interface
void
SQLInfoOracle::SetUseSequences(bool /*p_sequences*/)
{
  // Does nothing
}

//////////////////////////////////////////////////////////////////////////
//
// GENERALS (Strings & Booleans) 
//
//////////////////////////////////////////////////////////////////////////

// Get the database type
// DatabaseType GetDatabaseType() const;
DatabaseType
SQLInfoOracle::GetRDBMSDatabaseType() const
{
  return RDBMS_ORACLE;
}

// The name of the database vendor
XString
SQLInfoOracle::GetRDBMSVendorName() const
{
  // The name of the database vendor
  return _T("Oracle");
}

// Get the physical database name
XString
SQLInfoOracle::GetRDBMSPhysicalDatabaseName() const
{
  // Oops. We have a Oracle bug. Name is effectively one of
  // 1) the name in TNSNAMES.ORA file
  // 2) the DNS host name (as mirroring for the database)
  // 3) The SUN NIS name
  // 4) The DCE CDS name 
  // and **NOT** the database name from the server.
  // The names in 1) to 4) could be the same as the database name 
  // but there are a great number of situations where this is not the case.

  XString query = _T("SELECT SYS_CONTEXT('USERENV','DB_NAME')\n")
                  _T("  FROM DUAL");
  SQLQuery qry(m_database);
  qry.DoSQLStatement(query);
  if(qry.GetRecord())
  {
    return qry.GetColumn(1)->GetAsChar();
  }
  return XString(_T(""));
}

// System catalog is stored in uppercase in the database?
bool
SQLInfoOracle::GetRDBMSIsCatalogUpper() const
{
  return true;
}

// System catalog supports full ISO schemas (same tables per schema)
bool
SQLInfoOracle::GetRDBMSUnderstandsSchemas() const
{
  return true;
}

// Supports database/ODBCdriver comments in SQL
bool
SQLInfoOracle::GetRDBMSSupportsComments() const
{
  return true;
}

// Database can defer constraints until the end of a transaction
bool
SQLInfoOracle::GetRDBMSSupportsDeferredConstraints() const
{
  // SET CONSTRAINTS DEFERRED is supported
  return true;
}

// Database has ORDER BY with an expression, e.g. ORDER BY UPPER(columnname)
bool
SQLInfoOracle::GetRDBMSSupportsOrderByExpression() const
{
  return true;
}

// Supports the ODBC escape sequence {[?=] CALL procedure (?,?,?)}
bool
SQLInfoOracle::GetRDBMSSupportsODBCCallEscapes() const
{
  return true;
}

// Supports the ODBC call procedure with named parameters
bool
SQLInfoOracle::GetRDBMSSupportsODBCCallNamedParameters() const
{
  // Must use the "paramname => value [,...]" convention !!
  return false;
}

// If the database does not support the datatype TIME, it can be implemented as a DECIMAL
bool
SQLInfoOracle::GetRDBMSSupportsDatatypeTime() const
{
  // Time can be implemented as DECIMAL(17,16)
  return false;
}

// If the database does not support the datatype INTERVAL, it can be implemented as a DECIMAL
bool
SQLInfoOracle::GetRDBMSSupportsDatatypeInterval() const
{
  // Interval not supported, can be implemented as DECIMALS
  return false;
}

// Supports functions at the place of table columns in create/alter index statement
bool
SQLInfoOracle::GetRDBMSSupportsFunctionalIndexes() const
{
  return true;
}

// Gets the maximum length of an SQL statement
unsigned long
SQLInfoOracle::GetRDBMSMaxStatementLength() const
{
  // No Limit
  return 0;
}

// Database must commit DDL commands in a transaction
bool
SQLInfoOracle::GetRDBMSMustCommitDDL() const
{
  return false;
}

// Correct maximum precision,scale for a NUMERIC datatype
// If no precision and scale are given, Oracle assumes NUMERIC(38,0)
// This does not work for database views with a different scale in the backing table.
// So we change this to NUMERIC(38,18)
void
SQLInfoOracle::GetRDBMSNumericPrecisionScale(SQLULEN& p_precision, SQLSMALLINT& p_scale) const
{
  // ORACLE SPECIFIC CHECKS
  if(p_precision == (SQLULEN)     NUMERIC_MAX_PRECISION &&
     p_scale     == (SQLSMALLINT) NUMERIC_MIN_SCALE)
  {
    p_scale = NUMERIC_DEFAULT_SCALE;
  }


  // GENERAL CHECKS

  // Max precision for numerics is 38
  if(p_precision > (SQLULEN) NUMERIC_MAX_PRECISION)
  {
    p_precision = NUMERIC_MAX_PRECISION;
  }

  // Default scale is also the max for parameters (16)
  if(p_scale > NUMERIC_DEFAULT_SCALE)
  {
    p_scale = NUMERIC_DEFAULT_SCALE;
  }

  // In case of conversion from other databases
  if(p_precision == 0 && p_scale == 0)
  {
    p_precision = NUMERIC_MAX_PRECISION;
    p_scale     = NUMERIC_DEFAULT_SCALE;
  }

  // Scale MUST be smaller than the precision
  if(p_scale >= (SQLSMALLINT) p_precision)
  {
    p_scale = (SQLSMALLINT) (p_precision - 1);
  }
}

// Maximum for a VARCHAR to be handled without AT-EXEC data. Assume NVARCHAR is half that size!
int
SQLInfoOracle::GetRDBMSMaxVarchar() const
{
  return 4000;
}

// KEYWORDS

// Keyword for the current date and time
XString
SQLInfoOracle::GetKEYWORDCurrentTimestamp() const
{
  return _T("SYSDATE");
}

// String for the current date
XString
SQLInfoOracle::GetKEYWORDCurrentDate() const
{
  return _T("TRUNC(SYSDATE)");
}

// Get the concatenation operator
XString
SQLInfoOracle::GetKEYWORDConcatanationOperator() const
{
  return _T("||");
}

// Get quote character for strings
XString
SQLInfoOracle::GetKEYWORDQuoteCharacter() const
{
  return _T("\'");
}

// Get quote character around reserved words as an identifier
XString
SQLInfoOracle::GetKEYWORDReservedWordQuote() const
{
  return _T("\"");
}

// Get default NULL for parameter list input
XString
SQLInfoOracle::GetKEYWORDParameterDefaultNULL() const
{
  return _T("DEFAULT NULL");
}

// Parameter is for INPUT and OUTPUT in parameter list
XString
SQLInfoOracle::GetKEYWORDParameterINOUT() const
{
  return _T("IN OUT");
}

// Parameter is for OUTPUT only in parameter list
XString
SQLInfoOracle::GetKEYWORDParameterOUT() const
{
  return _T("OUT");
}

// Get datatype of the IDENTITY primary key in a Network database
XString
SQLInfoOracle::GetKEYWORDNetworkPrimaryKeyType() const
{
  // Use SEQUENCE to fill!
  return _T("INTEGER");
}

// Get datatype for timestamp (year to second)
XString
SQLInfoOracle::GetKEYWORDTypeTimestamp() const
{
  return _T("DATE");
}

// Prefix for a parameter in a stored procedure
XString
SQLInfoOracle::GetKEYWORDParameterPrefix() const
{
  return _T("");
}

// Get select part to add new record identity to a table
// Can be special column like 'OID' or a sequence select
XString
SQLInfoOracle::GetKEYWORDIdentityString(XString& p_tablename,XString p_postfix /*= "_seq"*/) const
{
  return p_tablename + p_postfix + _T(".nextval");
}

// Gets the UPPER function
XString
SQLInfoOracle::GetKEYWORDUpper(XString& p_expression) const
{
  return _T("UPPER(") + p_expression + _T(")");
}

// Gets the construction for 1 minute ago
XString
SQLInfoOracle::GetKEYWORDInterval1MinuteAgo() const
{
  return _T("(SYSDATE - 0.0007)");
}

// Gets the Not-NULL-Value statement of the database
XString
SQLInfoOracle::GetKEYWORDStatementNVL(XString& p_test,XString& p_isnull) const
{
  return XString(_T("NVL(")) + p_test + _T(",") + p_isnull + _T(")");
}

// Gets the RDBMS definition of the datatype
XString
SQLInfoOracle::GetKEYWORDDataType(MetaColumn* p_column)
{
  XString type;
  switch(p_column->m_datatype)
  {
    case SQL_CHAR:                      // fall through
    case SQL_VARCHAR:                   // fall through
    case SQL_WCHAR:                     // fall through
    case SQL_WVARCHAR:                  type = _T("VARCHAR2");      
                                        break;
    case SQL_LONGVARCHAR:               // fall through
    case SQL_WLONGVARCHAR:              type = _T("CLOB");          
                                        break;
    case SQL_NUMERIC:                   // fall through
    case SQL_DECIMAL:                   // fall through
    case SQL_INTEGER:                   // fall through
    case SQL_SMALLINT:                  // fall through
    case SQL_FLOAT:                     // fall through
    case SQL_REAL:                      // fall through
    case SQL_DOUBLE:                    // fall through
    case SQL_BIGINT:                    // fall through
    case SQL_TINYINT:                   // fall through
    case SQL_BIT:                       type = _T("NUMBER");
                                        p_column->m_columnSize    = 0;
                                        p_column->m_decimalDigits = 0;
                                        break;
  //case SQL_DATE:
    case SQL_DATETIME:                  // fall through
    case SQL_TYPE_DATE:                 // fall through
    case SQL_TIMESTAMP:                 // fall through
    case SQL_TYPE_TIMESTAMP:            type = _T("DATE");
                                        p_column->m_columnSize    = 0;
                                        p_column->m_decimalDigits = 0;
                                        break;
    case SQL_TIME:                      // fall through
    case SQL_TYPE_TIME:                 type = _T("TIMESTAMP");
                                        p_column->m_columnSize = 0;
                                        p_column->m_decimalDigits = 0;
                                        break;
    case SQL_BINARY:                    type = _T("BLOB");          break;
    case SQL_VARBINARY:                 type = _T("BLOB");          break;
    case SQL_LONGVARBINARY:             type = _T("BLOB");          break;
    case SQL_GUID:                      type = _T("GUID");          break;
    case SQL_INTERVAL_YEAR:             // fall through
    case SQL_INTERVAL_YEAR_TO_MONTH:    // fall through
    case SQL_INTERVAL_MONTH:            type = _T("VARCHAR2");
                                        p_column->m_columnSize    = 80;
                                        p_column->m_decimalDigits = 0;
                                        break;
    case SQL_INTERVAL_DAY:              // fall through
    case SQL_INTERVAL_HOUR:             // fall through
    case SQL_INTERVAL_MINUTE:           // fall through
    case SQL_INTERVAL_SECOND:           // fall through
    case SQL_INTERVAL_DAY_TO_HOUR:      // fall through
    case SQL_INTERVAL_DAY_TO_MINUTE:    // fall through
    case SQL_INTERVAL_HOUR_TO_MINUTE:   // fall through
    case SQL_INTERVAL_HOUR_TO_SECOND:   // fall through
    case SQL_INTERVAL_MINUTE_TO_SECOND: // fall through
    case SQL_INTERVAL_DAY_TO_SECOND:    type = _T("VARCHAR2");
                                        p_column->m_columnSize    = 80;
                                        p_column->m_decimalDigits = 0;
                                        break;
    case SQL_UNKNOWN_TYPE:              // fall through
    default:                            break;
  }
  return p_column->m_typename = type;
}

// Gets the USER (current-user) keyword function
XString
SQLInfoOracle::GetKEYWORDCurrentUser() const
{
  return _T("USER");
}

// Connects to a default schema in the database/instance
XString
SQLInfoOracle::GetSQLDefaultSchema(XString /*p_user*/,XString p_schema) const
{
  return _T("ALTER SESSION SET CURRENT_SCHEMA = ") + p_schema;
}

// Gets the construction for inline generating a key within an INSERT statement
XString
SQLInfoOracle::GetSQLNewSerial(XString p_table, XString p_sequence) const
{
  XString sequence(p_sequence);
  if (sequence.IsEmpty() && !p_table.IsEmpty())
  {
    sequence = p_table + _T("_seq");
  }

  // Select next value from a generator sequence
  return sequence + _T(".NEXTVAL");
}

// Gets the construction / select for generating a new serial identity
XString
SQLInfoOracle::GetSQLGenerateSerial(XString p_table) const
{
  return _T("SELECT ") + p_table + _T("_seq.nextval FROM DUAL");
}

// Gets the construction / select for the resulting effective generated serial
XString
SQLInfoOracle::GetSQLEffectiveSerial(XString p_identity) const
{
  // Just return it, it's the correct value
  return p_identity;
}

// Gets the sub transaction commands
XString
SQLInfoOracle::GetSQLStartSubTransaction(XString p_savepointName) const
{
  return XString(_T("SAVEPOINT ")) + p_savepointName;
}

XString
SQLInfoOracle::GetSQLCommitSubTransaction(XString p_savepointName) const
{
  // There is no savepoint commit in Oracle!!
  return _T("");
}

XString
SQLInfoOracle::GetSQLRollbackSubTransaction(XString p_savepointName) const
{
  return XString(_T("ROLLBACK TO ")) + p_savepointName;
}

// FROM-Part for a query to select only 1 (one) record / or empty!
XString
SQLInfoOracle::GetSQLFromDualClause() const
{
  return _T(" FROM dual");
}

// Get SQL to lock  a table 
XString
SQLInfoOracle::GetSQLLockTable(XString p_schema,XString p_tablename,bool p_exclusive,int /*p_waittime*/) const
{
  XString query = _T("LOCK TABLE ") + p_schema + _T(".") + p_tablename + _T(" IN ");
  query += p_exclusive ? _T("EXCLUSIVE") : _T("SHARE");
  query += _T(" MODE");
  return query;
}

// Get query to optimize the table statistics
XString
SQLInfoOracle::GetSQLOptimizeTable(XString p_schema, XString p_tablename) const
{
  XString optim;
  // Optimize the table
  optim = _T("call dbms_stats.gather_table_stats('") + p_schema + _T("','") + p_tablename + _T("' CASCADE => TRUE)");
  return optim;
}

// Transform query to select top <n> rows:
// Works from Oracle 12c and upward!!!
XString
SQLInfoOracle::GetSQLTopNRows(XString p_sql,int p_top,int p_skip /*= 0*/) const
{
  if(p_top > 0)
  {
    XString limit;
    if(p_skip > 0)
    {
      limit.Format(_T("\n OFFSET %d ROWS")
                   _T("\n FETCH NEXT %d ROWS ONLY")
                   ,p_skip
                   ,p_top);
    }
    else
    {
      limit.Format(_T("\n FETCH FIRST %d ROWS ONLY"),p_top);
    }
    p_sql += limit;
  }
  return p_sql;
}

// Expand a SELECT with an 'FOR UPDATE' lock clause
XString
SQLInfoOracle::GetSelectForUpdateTableClause(unsigned /*p_lockWaitTime*/) const
{
  return "";
}

XString
SQLInfoOracle::GetSelectForUpdateTrailer(XString p_select,unsigned p_lockWaitTime) const
{
  XString sql = p_select + _T("\nFOR UPDATE");
  if(p_lockWaitTime)
  {
    sql.AppendFormat(_T(" WAIT %d"),p_lockWaitTime);
  }
  else
  {
    sql += _T(" SKIP LOCKED");
  }
  return sql;
}

// Query to perform a keep alive ping
XString
SQLInfoOracle::GetPing() const
{
  // Getting the time does a ping
  return _T("SELECT current_timestamp FROM DUAL");
}

//////////////////////////////////////////////////////////////////////////
//
// SQL STRINGS
//
//////////////////////////////////////////////////////////////////////////

// Makes a SQL string from a given string, with all the right quotes
XString
SQLInfoOracle::GetSQLString(const XString& p_string) const
{
  XString s = p_string;
  s.Replace(_T("'"),_T("''"));
  XString kwoot = GetKEYWORDQuoteCharacter();
  return  kwoot + s + kwoot;
}

// Get date string in engine format
XString
SQLInfoOracle::GetSQLDateString(int p_year,int p_month,int p_day) const
{
  XString dateString;
  dateString.Format(_T("TO_DATE('%04d-%02d-%02d','YYYY-MM-DD')"),p_year,p_month,p_day);
  return dateString;
}

// Get time string in database engine format
XString
SQLInfoOracle::GetSQLTimeString(int p_hour,int p_minute,int p_second) const
{
  // The hour must be between 0 and 24
  p_hour = p_hour % 24;
  while(p_hour < 0) p_hour += 24;

  // Make a decimal part of a day
  int seconds = p_second + (p_minute * 60) + (p_hour * 60 * 60);
  int dag = 24 * 60 * 60;
  double val = (double)seconds / (double)dag;
  XString timeString;
  timeString.Format(_T("%0.15f"),val);

  return timeString;
}

// Get date-time string in database engine format
XString
SQLInfoOracle::GetSQLDateTimeString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  XString string;
  string.Format(_T("TO_DATE('%04d-%02d-%02d %02d:%02d:%02d','YYYY-MM-DD HH24:MI:SS')")
                ,p_year,p_month,p_day
                ,p_hour,p_minute,p_second);
  return string;
}

// For parameter binding of a date-time string
XString
SQLInfoOracle::GetSQLDateTimeBoundString() const
{
  return _T("TO_DATE(?,'YYYY-MM-DD HH24:MI:SS')");
}

// Stripped data for the parameter binding
XString
SQLInfoOracle::GetSQLDateTimeStrippedString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  XString string;
  string.Format(_T("%04d-%02d-%02d %02d:%02d:%02d")
                ,p_year,p_month,p_day
                ,p_hour,p_minute,p_second);
  return string;
}

// Makes an catalog identifier string (possibly quoted on both sides)
XString
SQLInfoOracle::GetSQLDDLIdentifier(XString p_identifier) const
{
  return p_identifier;
}

// Get the name of a temp table (local temporary or global temporary)
XString
SQLInfoOracle::GetTempTablename(XString /*p_schema*/,XString p_tablename,bool /*p_local*/) const
{
  return p_tablename;
}

// Changes to parameters before binding to an ODBC HSTMT handle
void
SQLInfoOracle::DoBindParameterFixup(SQLSMALLINT& /*p_sqlDatatype*/,SQLULEN& /*p_columnSize*/,SQLSMALLINT& /*p_scale*/,SQLLEN& /*p_bufferSize*/,SQLLEN* /*p_indicator*/) const
{
}

//////////////////////////////////////////////////////////////////////////
//
// CATALOG
// o GetCATALOG<Object[s]><Function>
//   Objects
//   - Catalog
//   - Table
//   - Column
//   - Index
//   - PrimaryKey
//   - ForeignKey
//   - Trigger
//   - TemporaryTable 
//   - View
//   - Sequence
//  Functions per object type
//   - Exists
//   - List
//   - Attributes
//   - Create
//   - Alter  (where possible)
//   - Rename (where possible)
//   - Drop
//
//////////////////////////////////////////////////////////////////////////

// Meta info about meta types
// Standard ODBC functions are ***NOT*** good enough (6 seconds per connect)
// So we provide our own queries for these functions
XString
SQLInfoOracle::GetCATALOGMetaTypes(int p_type) const
{
  XString sql;

  switch(p_type)
  {
    // Beware: all_db_links does not show PUBLIC database links
    case META_CATALOGS: sql = _T("SELECT sys_context('USERENV','DB_NAME') AS catalog\n")
                              _T("      ,'Current catalog'                AS remarks\n")
                              _T("  FROM dual\n")
                              _T("UNION\n")
                              _T("SELECT CASE InStr(db_link,'.')\n")
                              _T("            WHEN 0 THEN db_link\n")
                              _T("                   ELSE SubStr(db_link, 1, InStr(db_link, '.') - 1)\n")
                              _T("       END\n")
                              _T("      ,db_link\n")
                              _T("  FROM dba_db_links\n")
                              _T(" WHERE owner <> 'SYS'");
                        break;
    case META_SCHEMAS:  sql = _T("SELECT DISTINCT username\n")
                              _T("      ,'' AS remarks\n")
                              _T("  FROM all_users\n")
                              _T(" WHERE oracle_maintained = 'N'");
                        break;
    case META_TABLES:   sql = _T("SELECT 'TABLE' AS table_type\n")
                              _T("      ,''      AS remarks\n")
                              _T("  FROM dual\n")
                              _T("UNION\n")
                              _T("SELECT 'VIEW'\n")
                              _T("      ,''\n")
                              _T("  FROM dual");
                        break;
  }
  return sql;
}

XString
SQLInfoOracle::GetCATALOGDefaultCharset() const
{
  return _T("SELECT value\n")
         _T("  FROM nls_database_parameters\n")
         _T(" WHERE parameter = 'NLS_CHARACTERSET'");
}

XString
SQLInfoOracle::GetCATALOGDefaultCharsetNCV() const
{
  return _T("SELECT value\n")
         _T("  FROM nls_database_parameters\n")
         _T(" WHERE parameter = 'NLS_NCHAR_CHARACTERSET'");
}

XString
SQLInfoOracle::GetCATALOGDefaultCollation() const
{
  XString nlslang;
  nlslang.GetEnvironmentVariable(_T("NLS_LANG"));
  return nlslang;
}

// Get SQL to check if a table already exists in the database
XString
SQLInfoOracle::GetCATALOGTableExists(XString& p_schema,XString& p_tablename) const
{
  p_schema.MakeUpper();
  p_tablename.MakeUpper();
  XString query = _T("SELECT COUNT(*)\n")
                  _T("  FROM all_tables\n")
                  _T(" WHERE owner      = ?\n")
                  _T("   AND table_name = ?");
  return query;
}

XString
SQLInfoOracle::GetCATALOGTablesList(XString& p_schema,XString& p_pattern) const
{
  return GetCATALOGTableAttributes(p_schema,p_pattern);
}

XString
SQLInfoOracle::GetCATALOGTableAttributes(XString& p_schema,XString& p_tablename) const
{
  XString sql;
  p_schema.MakeUpper();
  p_tablename.MakeUpper();

  // TABLES
  sql  = _T("SELECT sys_context('USERENV','DB_NAME') AS table_catalog\n")
         _T("      ,tab.owner         AS table_schema\n")
         _T("      ,tab.table_name    AS table_name\n")
         _T("      ,CASE tab.TEMPORARY\n")
         _T("            WHEN 'Y' THEN 'GLOBAL TEMPORARY'\n")
         _T("            WHEN 'N' THEN 'TABLE'\n")
         _T("                     ELSE 'UNKNOWN'\n")
         _T("       END               AS object_type\n")
         _T("      ,com.comments      AS remarks\n")
         _T("      ,tab.tablespace_name\n")
         _T("      ,CASE tab.TEMPORARY\n")
         _T("            WHEN 'Y' THEN 1\n")
         _T("            WHEN 'N' THEN 0\n")
         _T("                     ELSE -1\n")
         _T("       END AS temporary\n")
         _T("  FROM all_tables tab LEFT OUTER JOIN all_tab_comments com\n")
         _T("                      ON (com.owner      = tab.owner\n")
         _T("                     AND  com.table_name = tab.table_name)\n")
         _T(" WHERE com.table_type = 'TABLE'\n");


  if(!p_schema.IsEmpty())
  {
    sql += _T("   AND tab.owner ");
    sql += p_schema.Find('%') >= 0 ? _T("LIKE") : _T("=");
    sql += _T(" ?\n");
  }

  if(!p_tablename.IsEmpty())
  {
    sql += _T("   AND tab.table_name ");
    sql += p_tablename.Find('%') >= 0 ? _T("LIKE") : _T("=");
    sql += _T(" ?\n");
  }
  sql += _T(" ORDER BY 1,2,3");

  return sql;
}

XString 
SQLInfoOracle::GetCATALOGTableSynonyms(XString& p_schema,XString& p_tablename) const
{
  p_schema.MakeUpper();
  p_tablename.MakeUpper();

  XString sql = _T("SELECT sys_context('USERENV','DB_NAME') AS table_catalog\n")
                _T("      ,table_owner  AS table_schema\n")
                _T("      ,synonym_name AS table_name\n")
                _T("      ,'SYNONYM'    AS object_type\n")
                _T("      ,''           AS remarks\n")
                _T("      ,''           AS tablespace_name\n")
                _T("      ,0            AS temporary\n")
                _T("  FROM all_synonyms syn\n");

  if(!p_schema.IsEmpty())
  {
    sql += _T(" WHERE table_owner ");
    sql += p_schema.Find('%') >= 0 ? _T("LIKE") : _T("=");
    sql += _T(" ?\n");
  }

  if(!p_tablename.IsEmpty())
  {
    sql += p_schema.IsEmpty() ? _T(" WHERE ") : _T("   AND ");
    sql += _T("synonym_name ");
    sql += p_tablename.Find('%') >= 0 ? _T("LIKE") : _T("=");
    sql += _T(" ?\n");
  }
  sql += _T(" ORDER BY 1,2,3");
  return sql;
}

XString
SQLInfoOracle::GetCATALOGTableCatalog(XString& p_schema,XString& p_tablename) const
{
  p_schema.MakeUpper();
  p_tablename.MakeUpper();

  XString sql = _T("SELECT sys_context('USERENV','DB_NAME') AS table_catalog\n")
                _T("      ,obj.owner         AS table_schema\n")
                _T("      ,obj.object_name   AS table_name\n")
                _T("      ,'SYSTEM TABLE'    AS object_type\n")
                _T("      ,com.comments      AS remarks\n")
                _T("      ,''                AS tablespace_name\n")
                _T("      ,0                 AS temporary\n")
                _T("  FROM all_objects obj LEFT OUTER JOIN all_tab_comments com\n")
                _T("                       ON (com.owner      = obj.owner\n")
                _T("                      AND  com.table_name = obj.object_name)\n")
                _T(" WHERE obj.owner IN ('SYS','SYSTEM')\n")
                _T("   AND obj.object_type IN ('TABLE','VIEW')\n")
                _T("   AND obj.object_name NOT LIKE ('%$%')\n");

  if(!p_schema.IsEmpty())
  {
    if(p_schema.Find('%') >= 0)
    {
      sql += _T("  AND obj.owner   LIKE ?\n");
    }
    else
    {
      sql += _T("  AND obj.owner   = ?\n");
    }
  }

  if(!p_tablename.IsEmpty())
  {
    if(p_tablename.Find('%') >= 0)
    {
      sql += _T("   AND obj.object_name LIKE ?\n");
    }
    else
    {
      sql += _T("   AND obj.object_name = ?\n");
    }
  }

  sql += _T(" ORDER BY 1,2,3");
  return sql;
}


XString
SQLInfoOracle::GetCATALOGTableCreate(MetaTable& p_table,MetaColumn& /*p_column*/) const
{
  XString sql = _T("CREATE ");
  if (p_table.m_temporary)
  {
    sql += _T("GLOBAL TEMPORARY ");
  }
  sql += _T("TABLE ");
  if (!p_table.m_schema.IsEmpty())
  {
    sql += p_table.m_schema;
    sql += _T(".");
  }
  sql += p_table.m_table;
  return sql;
}

XString
SQLInfoOracle::GetCATALOGTableCreatePostfix(MetaTable& /*p_table*/,MetaColumn& /*p_column*/) const
{
  return _T("");
}

XString
SQLInfoOracle::GetCATALOGTableRename(XString p_schema,XString p_tablename,XString p_newname) const
{
  // Beware: No 'TABLE' in the statement
  XString sql(_T("RENAME ") + p_schema + _T(".") + p_tablename + _T(" TO ") + p_newname);
  return sql;
}

XString
SQLInfoOracle::GetCATALOGTableDrop(XString p_schema,XString p_tablename,bool /*p_ifExist = false*/,bool /*p_restrict = false*/,bool p_cascade /*= false*/) const
{
  XString sql(_T("DROP TABLE "));
  if(!p_schema.IsEmpty())
  {
    sql += p_schema + _T(".");
  }
  sql += p_tablename;
  if(p_cascade)
  {
    sql += _T(" CASCADE CONSTRAINTS");
  }
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL TEMPORARY TABLE FUNCTIONS

XString 
SQLInfoOracle::GetCATALOGTemptableCreate(XString p_schema,XString p_tablename,XString p_select) const
{
  return _T("CREATE GLOBAL TEMPORARY TABLE ") + p_schema + _T(".") + p_tablename + _T("\nAS ") + p_select +
         _T("ON COMMIT PRESERVE ROWS");
}

XString 
SQLInfoOracle::GetCATALOGTemptableIntoTemp(XString p_schema,XString p_tablename,XString p_select) const
{
  return _T("INSERT INTO ") + p_schema + _T(".") + p_tablename + _T("\n") + p_select;
}

XString 
SQLInfoOracle::GetCATALOGTemptableDrop(XString p_schema,XString p_tablename) const
{
  XString tablename = p_schema + _T(".") + p_tablename;
  return _T("DELETE FROM ")    + tablename + _T(";\n")
         _T("<@>\n")
         _T("TRUNCATE TABLE ") + tablename + _T(";\n")
         _T("<@>\n")
         _T("DROP TABLE ")     + tablename + _T(";\n");
}

//////////////////////////////////////////////////////////////////////////
// ALL COLUMN FUNCTIONS

XString 
SQLInfoOracle::GetCATALOGColumnExists(XString p_schema,XString p_tablename,XString p_columnname) const
{
  p_schema.MakeUpper();
  p_tablename.MakeUpper();
  p_columnname.MakeUpper();
  XString query = _T("SELECT COUNT(*)\n")
                 _T("  FROM all_tab_columns tab\n")
                 _T(" WHERE tab.table_name  = '") + p_tablename  + _T("'\n")
                 _T("   AND tab.column_name = '") + p_columnname + _T("'\n")
                 _T("   AND tab.owner       = '") + p_schema     + _T("'");
  return query;
}

XString 
SQLInfoOracle::GetCATALOGColumnList(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  // Standard ODBC driver suffices
  return _T("");
}

XString 
SQLInfoOracle::GetCATALOGColumnAttributes(XString& p_schema,XString& p_tablename,XString& p_columnname) const
{
  p_schema.MakeUpper();
  p_tablename.MakeUpper();
  p_columnname.MakeUpper();

  XString sql = _T("SELECT sys_context('USERENV','DB_NAME') AS col_catalog\n")
                _T("      ,col.owner                        AS col_owner\n")
                _T("      ,col.table_name                   AS col_table\n")
                _T("      ,col.column_name                  AS col_column_name\n")
                _T("      ,CASE SubStr(col.data_type,1,10)\n")
                _T("            WHEN 'CHAR'          THEN  1\n")
                _T("            WHEN 'VARCHAR2'      THEN 12\n")
                _T("            WHEN 'NCHAR'         THEN -8\n")
                _T("            WHEN 'NVARCHAR'      THEN -9\n")
                _T("            WHEN 'NUMBER'        THEN CASE data_scale\n")
                _T("                                           WHEN 0   THEN 4\n")
                _T("                                                    ELSE 2\n")
                _T("                                      END\n")
                _T("            WHEN 'FLOAT'         THEN CASE data_precision\n")
                _T("                                           WHEN 63  THEN 7\n")
                _T("                                                    ELSE 8\n")
                _T("                                      END\n")
                _T("            WHEN 'BINARY_FLO'    THEN  7\n")
                _T("            WHEN 'BINARY_DOU'    THEN  8\n")
                _T("            WHEN 'DATE'          THEN 93\n")
                _T("            WHEN 'TIMESTAMP('    THEN 93\n")
                _T("            WHEN 'INTERVAL Y'    THEN 107\n")
                _T("            WHEN 'INTERVAL D'    THEN 110\n")
                _T("            WHEN 'CLOB'          THEN -1\n")
                _T("            WHEN 'RAW'           THEN -3\n")
                _T("            WHEN 'LONG'          THEN -1\n")
                _T("            WHEN 'BLOB'          THEN -4\n")
                _T("            WHEN 'NCLOB'         THEN -10\n")
                _T("                                 ELSE  0\n")
                _T("       END                             AS col_datatype\n")
                _T("      ,col.data_type                   AS col_typename\n")
                _T("      ,CASE SubStr(col.data_type,1,10)\n")
                _T("            WHEN 'LONG'       THEN 2147483647\n")
                _T("            WHEN 'CLOB'       THEN 2147483647\n")
                _T("            WHEN 'BLOB'       THEN 2147483647\n")
                _T("            WHEN 'NCLOB'      THEN 2147483647\n")
                _T("            WHEN 'DATE'       THEN 19\n")
                _T("            WHEN 'TIMESTAMP(' THEN 19\n")
                _T("            WHEN 'INTERVAL Y' THEN 52\n")
                _T("            WHEN 'INTERVAL D' THEN 52\n")
                _T("            WHEN 'BINARY_DOU' THEN 15\n")
                _T("            WHEN 'BINARY_FLO' THEN  7\n")
                _T("            WHEN 'NCHAR'      THEN char_col_decl_length\n")
                _T("            WHEN 'NVARCHAR'   THEN char_col_decl_length\n")
                _T("            WHEN 'NUMBER'     THEN NVL(data_precision,38)\n")
                _T("                              ELSE col.data_length\n")
                _T("       END                             AS col_columnsize\n")
                _T("      ,CASE SubStr(col.data_type,1,8)\n")
                _T("            WHEN 'LONG'     THEN 2147483647\n")
                _T("            WHEN 'CLOB'     THEN 2147483647\n")
                _T("            WHEN 'BLOB'     THEN 2147483647\n")
                _T("            WHEN 'NCLOB'    THEN 2147483647\n")
                _T("            WHEN 'DATE'     THEN 16\n")
                _T("            WHEN 'TIMESTAM' THEN 16\n")
                _T("                            ELSE col.data_length\n")
                _T("       END                             AS col_bufferlength\n")
                _T("      ,col.data_scale                  AS col_decimaldigits\n")
                _T("      ,CASE col.data_type\n")
                _T("            WHEN 'NUMBER'        THEN 10\n")
                _T("            WHEN 'FLOAT'         THEN 10\n")
                _T("            WHEN 'BINARY_FLOAT'  THEN 10\n")
                _T("            WHEN 'BINARY_DOUBLE' THEN 10\n")
                _T("                                 ELSE  0\n")
                _T("       END                             AS col_numradix\n")
                _T("      ,CASE nullable\n")
                _T("            WHEN 'N'      THEN 0\n")   //-- SQL_NO_NULLS
                _T("            WHEN 'Y'      THEN 1\n")   //-- SQL_NULLABLE
                _T("                          ELSE 2\n")   //-- SQL_NULLABLE_UNKNOWN
                _T("       END                             AS col_nullable\n")
                _T("      ,com.comments                    AS col_remarks\n")
                _T("      ,col.data_default                AS col_default\n")
                _T("      ,CASE SubStr(col.data_type,1,8)\n")
                _T("            WHEN 'CHAR'     THEN  1\n")
                _T("            WHEN 'VARCHAR2' THEN 12\n")
                _T("            WHEN 'NCHAR'    THEN -8\n")
                _T("            WHEN 'NVARCHAR' THEN -9\n")
                _T("            WHEN 'NUMBER'   THEN CASE data_scale\n")
                _T("                                      WHEN 0   THEN 4\n")
                _T("                                               ELSE 2\n")
                _T("                                 END\n")
                _T("            WHEN 'FLOAT'    THEN CASE data_precision\n")
                _T("                                      WHEN 63  THEN 7\n")
                _T("                                               ELSE 8\n")
                _T("                                 END\n")
                _T("            WHEN 'BINARY_F' THEN  7\n")
                _T("            WHEN 'BINARY_D' THEN  8\n")
                _T("            WHEN 'DATE'     THEN  9\n")
                _T("            WHEN 'TIMESTAM' THEN  9\n")
                _T("            WHEN 'INTERVAL' THEN -4\n")
                _T("            WHEN 'CLOB'     THEN -1\n")
                _T("            WHEN 'LONG'     THEN -1\n")
                _T("            WHEN 'BLOB'     THEN -4\n")
                _T("            WHEN 'NCLOB'    THEN -10\n")
                _T("                            ELSE  0\n")
                _T("       END                             AS col_datatype3\n")
                _T("      ,CASE SubStr(col.data_type,1,8)\n")
                _T("            WHEN 'DATE'     THEN 3\n")
                _T("            WHEN 'TIMESTAM' THEN 3\n")
                _T("                            ELSE 0\n")
                _T("       END                             AS col_subdatatype\n")
                _T("      ,CASE col.data_type\n")
                _T("            WHEN 'CHAR'      THEN data_length\n")
                _T("            WHEN 'VARCHAR2'  THEN data_length\n")
                _T("            WHEN 'NCHAR'     THEN data_length\n")
                _T("            WHEN 'NVARCHAR2' THEN data_length\n")
                _T("                             ELSE 0\n")
                _T("       END                             AS col_octetlength\n")
                _T("      ,col.column_id                   AS col_position\n")
                _T("      ,CASE col.nullable\n")
                _T("            WHEN 'Y' THEN 'YES'\n")
                _T("                     ELSE 'NO'\n")
                _T("       END                            AS col_isnullable\n")
                _T("  FROM all_tab_columns col\n")
                _T("       LEFT OUTER JOIN all_col_comments com ON (col.owner       = com.owner\n")
                _T("                                            AND col.table_name  = com.table_name\n")
                _T("                                            AND col.column_name = com.column_name)\n");

  if(!p_schema.IsEmpty())
  {
    sql.AppendFormat(_T(" WHERE col.owner      = '%s'\n"),p_schema.GetString());
  }
  if(!p_tablename.IsEmpty())
  {
    sql += p_schema.IsEmpty() ? _T(" WHERE ") : _T("   AND ");
    sql.AppendFormat(_T("col.table_name = '%s'\n"),p_tablename.GetString());
  }
  if(!p_columnname.IsEmpty())
  {
    sql += p_schema.IsEmpty() && p_tablename.IsEmpty() ? _T(" WHERE ") : _T("   AND ");
    sql.AppendFormat(_T("col.column_name = '%s'\n"),p_columnname.GetString());
  }
  sql += _T(" ORDER BY 1,2,3,17");
  return sql;
}

XString 
SQLInfoOracle::GetCATALOGColumnCreate(MetaColumn& p_column) const
{
  XString sql = _T("ALTER TABLE ")  + p_column.m_schema + _T(".") + p_column.m_table  + _T("\n")
                _T("  ADD COLUMN ") + p_column.m_column + _T(" ") + p_column.m_typename;
  p_column.GetPrecisionAndScale(sql);
  p_column.GetNullable(sql);
  p_column.GetDefault(sql);
  // m_position not used
  // m_length   not used
  // m_remarks  not used
  return sql;
}

XString 
SQLInfoOracle::GetCATALOGColumnAlter(MetaColumn& p_column) const
{
  XString sql = _T("ALTER  TABLE  ") + p_column.m_schema + _T(".") + p_column.m_table  + _T("\n")
                _T("MODIFY COLUMN ") + p_column.m_column + _T(" ") + p_column.m_typename;
  p_column.GetPrecisionAndScale(sql);
  p_column.GetNullable(sql);
  p_column.GetDefault(sql);
  // m_position not used
  // m_length   not used
  // m_remarks  not used
  return sql;
}

XString
SQLInfoOracle::GetCATALOGColumnRename(XString p_schema,XString p_tablename,XString p_columnname,XString p_newname,XString /*p_datatype*/) const
{
  XString sql(_T("ALTER  TABLE  ") + p_schema + _T(".") + p_tablename + _T("\n")
              _T("RENAME COLUMN ") + p_columnname + _T(" TO ") + p_newname + _T("\n"));
  return sql;
}

XString
SQLInfoOracle::GetCATALOGColumnDrop(XString p_schema,XString p_tablename,XString p_columnname) const
{
  XString sql(_T("ALTER TABLE ") + p_schema + _T(".") + p_tablename + _T("\n")
              _T(" DROP COLUMN ") + p_columnname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL INDICES FUNCTIONS

// All index functions
XString
SQLInfoOracle::GetCATALOGIndexExists(XString p_schema,XString p_tablename,XString p_indexname) const
{
  return _T("");
}

XString
SQLInfoOracle::GetCATALOGIndexList(XString& p_schema,XString& p_tablename)   const
{
  p_schema.MakeUpper();
  p_tablename.MakeUpper();

  // Query runs on the 'all_' variant, so owner name must be taken into account
  // for performance reasons on the system table
  XString query = _T("SELECT idx.index_name\n")
                  _T("      ,col.column_name\n")
                  _T("      ,col.column_position\n")
//                "      ,idx.index_type\n"
                  _T("      ,idx.uniqueness\n")
                  _T("      ,col.descend\n")
                  _T("      ,'' as column_source")
                  _T("  FROM all_indexes     idx\n")
                  _T("      ,all_ind_columns col\n")
                  _T(" WHERE idx.index_name  = col.index_name\n")
                  _T("   AND idx.table_name  = col.table_name\n")
                  _T("   AND idx.table_owner = col.table_owner\n")
                  _T("   AND idx.generated   = 'N'\n")
                  _T("   AND idx.table_owner = ?\n")
                  _T("   AND idx.table_name  = ?\n")
                  _T(" ORDER BY index_name\n")
                  _T("         ,column_position\n");
  return query;
}

XString
SQLInfoOracle::GetCATALOGIndexAttributes(XString& p_schema,XString& p_tablename,XString& p_indexname) const
{
  XString query;
  p_schema.MakeUpper();
  p_tablename.MakeUpper();
  p_indexname.MakeUpper();
  bool tablestatistics = false;

  // BEWARE: These two SQL queries cannot be joined in a UNION select
  //         Oracle's LONG column definition of the "column_expression" prevents that
  if(p_indexname.Compare(_T("0")) == 0)
  {
    // Create the TABLE_STATISTICS QUERY
    tablestatistics = true;
    p_indexname.Empty();
    query = _T("SELECT sys_context('USERENV','DB_NAME') AS table_catalog\n")
            _T("      ,ind.owner                        AS table_schema\n")
            _T("      ,ind.table_name                   AS table_name\n")
            _T("      ,NULL          AS non_unique\n")
            _T("      ,NULL          AS index_name\n")
            _T("      ,0             AS index_type\n")  //-- SQL_TABLE_STAT
            _T("      ,NULL          AS ordinal_position\n")
            _T("      ,NULL          AS column_name\n")
            _T("      ,NULL          AS asc_or_desc\n")
            _T("      ,ind.num_rows  AS cardinality\n")
            _T("      ,ind.blocks    AS pages\n")
            _T("      ,NULL          AS filter\n")
            _T("  FROM all_tables ind");
  }
  else
  {
    // Create the TABLE_INDEXES QUERY
    query = _T("SELECT sys_context('USERENV','DB_NAME') AS table_catalog\n")
            _T("      ,ind.table_owner                  AS table_schema\n")
            _T("      ,ind.table_name                   AS table_name\n")
            _T("      ,CASE ind.uniqueness\n")
            _T("            WHEN 'UNIQUE' THEN 0\n")
            _T("                          ELSE 1\n")
            _T("       END                     AS non_unique\n")
            _T("      ,ind.index_name          AS index_name\n")
            _T("      ,3                       AS index_type\n")  //  -- SQL_INDEX_OTHER
            _T("      ,col.column_position     AS ordinal_position\n")
            _T("      ,col.column_name         AS column_name\n")
            _T("      ,substr(col.descend,1,1) AS asc_or_desc\n")
            _T("      ,ind.num_rows            AS cardinality\n")
            _T("      ,ind.leaf_blocks         AS pages\n")
            _T("      ,exp.column_expression   AS filter\n")
            _T("  FROM all_indexes ind\n")
            _T("       inner join\n")
            _T("       all_ind_columns col ON (col.table_name = ind.table_name\n")
            _T("                           AND ind.index_name = col.index_name)\n")
            _T("       left OUTER JOIN\n")
            _T("       all_ind_expressions Exp ON (Exp.table_name      = col.table_name\n")
            _T("                               AND Exp.index_name      = col.index_name\n")
            _T("                               AND Exp.column_position = col.column_position)\n");
  }
  if(!p_schema.IsEmpty())
  {
    if(tablestatistics)
    {
      query += _T(" WHERE ind.owner = ?\n");
    }
    else
    {
      query += _T(" WHERE ind.table_owner = ?\n");
    }
  }
  if(!p_tablename.IsEmpty())
  {
    query += p_schema.IsEmpty() ? _T(" WHERE ") : _T("   AND ");
    query += _T("ind.table_name = ?\n");
  }
  if(!p_indexname.IsEmpty())
  {
    query += p_schema.IsEmpty() && p_tablename.IsEmpty() ? _T(" WHERE ") : _T("   AND ");
    query += _T("ind.index_name = ?\n");
  }
  query += _T(" ORDER BY 1,2,3,5,7");
  return query;
}

XString
SQLInfoOracle::GetCATALOGIndexCreate(MIndicesMap& p_indices,bool /*p_duplicateNulls /*= false*/) const
{
  // Get SQL to create an index for a table
  // CREATE [UNIQUE] INDEX [<schema>.]indexname ON [<schema>.]tablename(column [ASC|DESC] [,...]);
  XString query;
  for(auto& index : p_indices)
  {
    if(index.m_position == 1)
    {
      // New index
      query = _T("CREATE ");
      if(index.m_nonunique == false)
      {
        query += _T("UNIQUE ");
      }
      query += _T("INDEX ");
      if(!index.m_schemaName.IsEmpty())
      {
        query += index.m_schemaName + _T(".");
      }
      query += index.m_indexName;
      query += _T(" ON ");
      if(!index.m_schemaName.IsEmpty())
      {
        query += index.m_schemaName + _T(".");
      }
      query += index.m_tableName;
      query += _T("(");
    }
    else
    {
      query += _T(",");
    }
    if(!index.m_filter.IsEmpty())
    {
      query += index.m_filter;
    }
    else
    {
      query += index.m_columnName;
      if(index.m_ascending != _T("A"))
      {
        query += _T(" DESC");
      }
    }
  }
  query += _T(")");
  return query;
}

XString
SQLInfoOracle::GetCATALOGIndexDrop(XString p_schema,XString /*p_tablename*/,XString p_indexname) const
{
  XString sql = _T("DROP INDEX ") + p_schema + _T(".") + p_indexname;
  return sql;
}

// Get extra filter expression for an index column
XString
SQLInfoOracle::GetCATALOGIndexFilter(MetaIndex& p_index) const
{
  XString expression;
  XString sql;
  sql.Format(_T("SELECT column_expression\n")
             _T("  FROM all_ind_expressions\n")
             _T(" WHERE index_owner = '") + p_index.m_schemaName + _T("'\n")
             _T("   AND index_name  = '") + p_index.m_indexName + _T("'\n")
             _T("   AND table_owner = '") + p_index.m_schemaName + _T("'\n")
             _T("   AND table_name  = '") + p_index.m_tableName + _T("'\n")
             _T("   AND column_position = %d"),p_index.m_position);
  try
  {
    SQLQuery qry(m_database);
    const SQLVariant* var = qry.DoSQLStatementScalar(sql);
    if(var)
    {
      var->GetAsString(expression);
    }
  }
  catch(StdException& error)
  {
    ReThrowSafeException(error);
    XString message = (_T("Cannot find index filter: ")) + error.GetErrorMessage();
    throw StdException(message);
  }
  return expression;
}

//////////////////////////////////////////////////////////////////////////
// ALL PRIMARY KEY FUNCTIONS

XString
SQLInfoOracle::GetCATALOGPrimaryExists(XString p_schema,XString p_tablename) const
{
  p_schema.MakeUpper();
  p_tablename.MakeUpper();

  XString query = _T("SELECT c.constraint_name\n")
                  _T("      ,c.index_name\n")
                  _T("      ,'Y' as deferrable\n")
                  _T("      ,'N' as initially_deferred\n")
                  _T("  FROM all_constraints c\n")
                  _T(" WHERE c.owner           = '") + p_schema    + _T("'\n")
                  _T("   AND c.table_name      = '") + p_tablename + _T("'\n")
                  _T("   AND c.constraint_type = 'P'\n");
  return query;
}

XString
SQLInfoOracle::GetCATALOGPrimaryAttributes(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  // To be implemented
  return _T("");
}

XString
SQLInfoOracle::GetCATALOGPrimaryCreate(MPrimaryMap& p_primaries) const
{
  XString query(_T("ALTER TABLE "));

  for(auto& prim : p_primaries)
  {
    if(prim.m_columnPosition == 1)
    {
      if(!prim.m_schema.IsEmpty())
      {
        query += prim.m_schema + _T(".");
      }
      query += prim.m_table + _T("\n");
      query += _T("  ADD CONSTRAINT ") + prim.m_constraintName + _T("\n");
      query += _T("      PRIMARY KEY (");

    }
    else
    {
      query += _T(",");
    }
    query += prim.m_columnName;
  }
  query += _T(")");
  return query;
}

XString
SQLInfoOracle::GetCATALOGPrimaryDrop(XString p_schema,XString p_tablename,XString p_constraintname) const
{
  XString sql(_T("ALTER TABLE ") + p_schema + _T(".") + p_tablename + _T("\n")
              _T(" DROP CONSTRAINT ") + p_constraintname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL FOREIGN KEY FUNCTIONS

XString
SQLInfoOracle::GetCATALOGForeignExists(XString p_schema,XString p_tablename,XString p_constraintname) const
{
  p_schema.MakeUpper();
  p_tablename.MakeUpper();
  p_constraintname.MakeUpper();

  XString sql;
  sql.Format(_T("SELECT COUNT(*)\n")
             _T("  FROM all_constraints con\n")
             _T(" WHERE con.constraint_type = 'R'")
             _T("   AND con.owner           = '") + p_schema + _T("'\n")
             _T("   AND con.table_name      = '") + p_tablename + _T("'\n")
             _T("   AND con.constraint_name = '") + p_constraintname + _T("'")
            ,p_schema.GetString()
            ,p_tablename.GetString()
            ,p_constraintname.GetString());

  return sql;
}

XString
SQLInfoOracle::GetCATALOGForeignList(XString& p_schema,XString& p_tablename,int p_maxColumns /*=SQLINFO_MAX_COLUMNS*/) const
{
  XString constraint;
  return GetCATALOGForeignAttributes(p_schema,p_tablename,constraint,p_maxColumns);
}

XString
SQLInfoOracle::GetCATALOGForeignAttributes(XString& p_schema
                                          ,XString& p_tablename
                                          ,XString& p_constraint
                                          ,bool     p_referenced /*=false*/
                                          ,int    /*p_maxColumns*/ /*=SQLINFO_MAX_COLUMNS*/) const
{
  // Oracle catalog is in uppercase
  p_schema.MakeUpper();
  p_tablename.MakeUpper();
  p_constraint.MakeUpper();

  // Minimal requirements of the catalog
  if(p_schema.IsEmpty() || p_tablename.IsEmpty())
  {
    throw StdException(_T("Cannot get table references without schema/tablename"));
  }

  XString query = _T("SELECT sys_context('USERENV','DB_NAME') AS primary_catalog_name\n")
                  _T("      ,pri.owner                        AS primary_schema_name\n")
                  _T("      ,pri.table_name                   AS primary_table_name\n")
                  _T("      ,sys_context('USERENV','DB_NAME') AS foreign_catalog_name\n")
                  _T("      ,con.owner            AS foreign_schema_name\n")
                  _T("      ,con.table_name       AS table_name\n")
                  _T("      ,pri.constraint_name  AS primary_key_constraint\n")
                  _T("      ,con.constraint_name  AS foreign_key_constraint\n")
                  _T("      ,col.position         AS key_sequence\n")
                  _T("      ,pky.column_name      AS primary_key_column\n")
                  _T("      ,col.column_name      AS foreign_key_column\n")
                  _T("      ,0                    AS update_rule\n")
                  _T("      ,CASE con.delete_rule WHEN 'RESTRICT'    THEN 1\n")
                  _T("                            WHEN 'CASCADE'     THEN 0\n")
                  _T("                            WHEN 'SET NULL'    THEN 2\n")
                  _T("                            WHEN 'SET DEFAULT' THEN 4\n")
                  _T("                            WHEN 'NO ACTION'   THEN 3\n")
                  _T("                            ELSE 0 END AS delete_rule\n")
                  _T("      ,CASE con.deferrable  WHEN 'NOT DEFERRABLE' THEN 0\n")
                  _T("                            WHEN 'DEFERRABLE'     THEN 1\n")
                  _T("                            ELSE 0 END AS DEFERRABLE\n")
                  _T("      ,0                    AS match_option\n")
                  _T("      ,CASE con.deferred    WHEN 'IMMEDIATE'      THEN 0\n")
                  _T("                            ELSE 1 END AS initially_deferred\n")
                  _T("      ,CASE con.status      WHEN 'ENABLED' THEN 1\n")
                  _T("                            ELSE 0 END AS enabled\n")
                  _T("  FROM all_constraints  con\n")
                  _T("      ,all_cons_columns col\n")
                  _T("      ,all_constraints  pri\n")
                  _T("      ,all_cons_columns pky\n")
                  _T(" WHERE con.owner           = col.owner\n")
                  _T("   AND con.constraint_name = col.constraint_name\n")
                  _T("   AND con.table_name      = col.table_name\n")
                  _T("   AND pri.owner           = con.r_owner\n")
                  _T("   AND pri.constraint_name = con.r_constraint_name\n")
                  _T("   AND pri.owner           = pky.owner\n")
                  _T("   AND pri.constraint_name = pky.constraint_name\n")
                  _T("   AND pri.table_name      = pky.table_name\n")
                  _T("   AND col.position        = pky.position\n")
                  _T("   AND con.constraint_type = 'R'");

  // Optionally add our filters
  if(!p_schema.IsEmpty())
  {
    if(p_referenced)
    {
      query += _T("   AND pri.owner           = ?\n");
    }
    else
    {
      query += _T("   AND con.owner           = ?\n");
    }
  }
  if(!p_tablename.IsEmpty())
  {
    if(p_referenced)
    {
      query += _T("   AND pri.table_name      = ?\n");
    }
    else
    {
      query += _T("   AND con.table_name      = ?\n");
    }

  }
  if(!p_constraint.IsEmpty())
  {
    if(p_referenced)
    {
      query += _T("   AND pri.constraint_name = ?\n");
    }
    else
    {
      query += _T("   AND con.constraint_name = ?\n");
    }
  }

  // Order upto the column number
  query += _T(" ORDER BY 1,2,3,4,5,6,7,8,9");

  return query;
}

XString
SQLInfoOracle::GetCATALOGForeignCreate(MForeignMap& p_foreigns) const
{
  MetaForeign& foreign = p_foreigns.front();

  // Construct the correct tablename
  XString table(foreign.m_fkTableName);
  XString primary(foreign.m_pkTableName);
  if(!foreign.m_fkSchemaName.IsEmpty())
  {
    table = foreign.m_fkSchemaName + _T(".") + table;
  }
  if(!foreign.m_pkSchemaName.IsEmpty())
  {
    primary = foreign.m_pkSchemaName + _T(".") + primary;
  }

  // The base foreign key command
  XString query = _T("ALTER TABLE ") + table + _T("\n")
                  _T("  ADD CONSTRAINT ") + foreign.m_foreignConstraint + _T("\n")
                  _T("      FOREIGN KEY (");

  // Add the foreign key columns
  bool extra = false;
  for(const auto& key : p_foreigns)
  {
    if(extra) query += _T(",");
    query += key.m_fkColumnName;
    extra  = true;
  }

  // Add references primary table
  query += _T(")\n      REFERENCES ") + primary + _T("(");

  // Add the primary key columns
  extra = false;
  for(const auto& key : p_foreigns)
  {
    if(extra) query += _T(",");
    query += key.m_pkColumnName;
    extra  = true;
  }
  query += _T(")");

  // Add all relevant options
  switch(foreign.m_deferrable)
  {
    case SQL_INITIALLY_DEFERRED:  query += _T("\n      INITIALLY DEFERRED"); break;
    case SQL_INITIALLY_IMMEDIATE: query += _T("\n      DEFERRABLE");         break;
    case SQL_NOT_DEFERRABLE:      // Already the default
                                  break;
  }
  switch(foreign.m_deleteRule)
  {
    case SQL_CASCADE:  query += _T("\n      ON DELETE CASCADE");     break;
    case SQL_SET_NULL: query += _T("\n      ON DELETE SET NULL");    break;
    default:           // In essence: ON DELETE RESTRICT, but that's already the default
                       break;
  }
  return query;
}

XString
SQLInfoOracle::GetCATALOGForeignAlter(MForeignMap& p_original, MForeignMap& p_requested) const
{
  // Make sure we have both
  if (p_original.empty() || p_requested.empty())
  {
    return _T("");
  }

  const MetaForeign& original = p_original.front();
  const MetaForeign& requested = p_requested.front();

  // Construct the correct tablename
  XString table(original.m_fkTableName);
  if(!original.m_fkSchemaName.IsEmpty())
  {
    table = original.m_fkSchemaName + _T(".") + table;
  }

  // The base foreign key command
  XString query = _T("ALTER  TABLE ") + table + _T("\n")
                  _T("MODIFY CONSTRAINT ") + original.m_foreignConstraint + _T("\n");

  // Add all relevant options
  if(original.m_deferrable != requested.m_deferrable)
  {
    query.AppendFormat(_T("\n      %sDEFERRABLE"), requested.m_deferrable == 0 ? _T("NOT ") : _T(""));
  }
  if(original.m_initiallyDeferred != requested.m_initiallyDeferred)
  {
    query += _T("\n      INITIALLY ");
    query += requested.m_initiallyDeferred ? _T("DEFERRED") : _T("IMMEDIATE");
  }
  if (original.m_deleteRule != requested.m_deleteRule)
  {
    switch(requested.m_deleteRule)
    {
      case SQL_CASCADE:   query += _T("\n      ON DELETE CASCADE");     break;
      case SQL_SET_NULL:  query += _T("\n      ON DELETE SET NULL");    break;
      default:            // In essence: ON DELETE RESTRICT, 
                          // But Oracle cannot do that, so return an empty string and drop the constraint
      case SQL_RESTRICT:  query.Empty();
    }
  }
  return query;
}

XString
SQLInfoOracle::GetCATALOGForeignDrop(XString p_schema,XString p_tablename,XString p_constraintname) const
{
  XString sql(_T("ALTER TABLE ") + p_schema + _T(".") + p_tablename + _T("\n")
              _T(" DROP CONSTRAINT ") + p_constraintname);
  return sql;
}

//////////////////////////
// All default constraints
XString
SQLInfoOracle::GetCATALOGDefaultExists(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_column*/) const
{
  return _T("");
}

XString
SQLInfoOracle::GetCATALOGDefaultList(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  return _T("");
}

XString
SQLInfoOracle::GetCATALOGDefaultAttributes(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_column*/) const
{
  return _T("");
}

XString
SQLInfoOracle::GetCATALOGDefaultCreate(XString /*p_schema*/,XString /*p_tablename*/,XString /*p_constraint*/,XString /*p_column*/,XString /*p_code*/) const
{
  return _T("");
}

XString
SQLInfoOracle::GetCATALOGDefaultDrop(XString /*p_schema*/,XString /*p_tablename*/,XString /*p_constraint*/) const
{
  return _T("");
}

/////////////////////////
// All check constraints

XString
SQLInfoOracle::GetCATALOGCheckExists(XString  /*p_schema*/,XString  /*p_tablename*/,XString  /*p_constraint*/) const
{
  return _T("");
}

XString
SQLInfoOracle::GetCATALOGCheckList(XString  /*p_schema*/,XString  /*p_tablename*/) const
{
  return _T("");
}

XString
SQLInfoOracle::GetCATALOGCheckAttributes(XString  /*p_schema*/,XString  /*p_tablename*/,XString  /*p_constraint*/) const
{
  return _T("");
}

XString
SQLInfoOracle::GetCATALOGCheckCreate(XString  /*p_schema*/,XString  /*p_tablename*/,XString  /*p_constraint*/,XString /*p_condition*/) const
{
  return _T("");
}

XString
SQLInfoOracle::GetCATALOGCheckDrop(XString  /*p_schema*/,XString  /*p_tablename*/,XString  /*p_constraint*/) const
{
  return _T("");
}

//////////////////////////////////////////////////////////////////////////
// ALL TRIGGER FUNCTIONS

XString
SQLInfoOracle::GetCATALOGTriggerExists(XString p_schema, XString p_tablename, XString p_triggername) const
{
  p_schema.MakeUpper();
  p_tablename.MakeUpper();
  p_triggername.MakeUpper();

  XString sql;
  sql.Format(_T("SELECT COUNT(*)\n")
             _T("  FROM all_triggers\n")
             _T(" WHERE table_owner  = '%s'\n")
             _T("   AND table_name   = '%s'\n")
             _T("   AND trigger_name = '%s'")
            ,p_schema.GetString()
            ,p_tablename.GetString()
            ,p_triggername.GetString());
  return sql;
}

XString
SQLInfoOracle::GetCATALOGTriggerList(XString& p_schema,XString& p_tablename) const
{
  XString triggername;
  return GetCATALOGTriggerAttributes(p_schema,p_tablename,triggername);
}

XString
SQLInfoOracle::GetCATALOGTriggerAttributes(XString& p_schema,XString& p_tablename,XString& p_triggername) const
{
  p_schema.MakeUpper();
  p_tablename.MakeUpper();
  p_triggername.MakeUpper();

  XString sql = _T("SELECT sys_context('USERENV','DB_NAME')  AS catalog_name\n")
                _T("      ,owner                             AS schema_name\n")
                _T("      ,table_name\n")
                _T("      ,trigger_name\n")
                _T("      ,triggering_event || ' ON ' || table_name AS description\n")
                _T("      ,0     AS position\n")
                _T("      ,CASE WHEN (InStr(trigger_type,    'BEFORE') > 0) THEN 1 ELSE 0 END AS trigger_before\n")
                _T("      ,CASE WHEN (InStr(triggering_event,'INSERT') > 0) THEN 1 ELSE 0 END AS trigger_insert\n")
                _T("      ,CASE WHEN (InStr(triggering_event,'UPDATE') > 0) THEN 1 ELSE 0 END AS trigger_update\n") 
                _T("      ,CASE WHEN (InStr(triggering_event,'DELETE') > 0) THEN 1 ELSE 0 END AS trigger_delete\n") 
                _T("      ,CASE WHEN (InStr(triggering_event,'SELECT') > 0) THEN 1 ELSE 0 END AS trigger_select\n")
                _T("      ,CASE WHEN (InStr(triggering_event,'LOGON')  > 0 OR\n")
                _T("                  InStr(triggering_event,'LOGOFF') > 0 ) THEN 1 ELSE 0 END AS trigger_session\n")
                _T("      ,0  AS trigger_transaction\n")
                _T("      ,0  AS trigger_rollback\n")
                _T("      ,referencing_names\n")
                _T("      ,CASE status\n")
                _T("            WHEN 'DISABLED' THEN 0\n")
                _T("                            ELSE 1\n")
                _T("            END AS trigger_status\n")
                _T("      ,trigger_body AS source\n")
                _T("  FROM all_triggers\n");
  if(!p_schema.IsEmpty())
  {
    sql += _T(" WHERE table_owner = ?\n");
  }
  if(!p_tablename.IsEmpty())
  {
    sql += p_schema.IsEmpty() ? _T(" WHERE ") : _T("   AND ");
    sql += _T("table_name ");
    sql += p_tablename.Find('%') > 0 ? _T("LIKE") : _T("=");
    sql += _T(" ?\n");
  } 
  if(!p_triggername.IsEmpty())
  {
    sql += p_schema.IsEmpty() && p_tablename.IsEmpty() ? _T(" WHERE ") : _T("   AND ");
    sql += _T("trigger_name ");
    sql += p_triggername.Find('%') > 0 ? _T("LIKE") : _T("=");
    sql += _T(" ?\n");
  }
  sql += _T(" ORDER BY 1,2,3,4");
  return sql;
}

XString
SQLInfoOracle::GetCATALOGTriggerCreate(MetaTrigger& p_trigger) const
{
  // Command + trigger name
  XString sql(_T("CREATE OR REPLACE TRIGGER "));
  sql += p_trigger.m_schemaName;
  sql += _T(".");
  sql += p_trigger.m_triggerName;
  sql += _T("\n");

  // Before or after
  sql += p_trigger.m_before ? _T("BEFORE ") : _T("AFTER ");

  // Trigger actions
  if(p_trigger.m_insert)
  {
    sql += _T("INSERT");
  }
  if(p_trigger.m_update)
  {
    if(p_trigger.m_insert)
    {
      sql += _T(" OR ");
    }
    sql += _T("UPDATE");
  }
  if(p_trigger.m_delete)
  {
    if(p_trigger.m_insert || p_trigger.m_update)
    {
      sql += _T(" OR ");
    }
    sql += _T("DELETE");
  }

  // Add trigger table
  sql += _T(" ON ");
  sql += p_trigger.m_tableName;
  sql += _T("\n");

  // Referencing clause
  if(!p_trigger.m_referencing.IsEmpty())
  {
    sql += p_trigger.m_referencing;
    sql += _T("\nFOR EACH ROW\n");
  }

  // Add trigger body
  sql += p_trigger.m_source;

  return sql;
}

XString
SQLInfoOracle::GetCATALOGTriggerDrop(XString p_schema, XString p_tablename, XString p_triggername) const
{
  return _T("");
}

//////////////////////////////////////////////////////////////////////////
// ALL SEQUENCE FUNCTIONS

XString
SQLInfoOracle::GetCATALOGSequenceExists(XString p_schema, XString p_sequence) const
{
  p_schema.MakeUpper();
  p_sequence.MakeUpper();

  XString sql = _T("SELECT COUNT(*)\n")
                _T("  FROM all_sequences\n")
                _T(" WHERE sequence_owner = '") + p_schema + _T("'\n")
                _T("   AND sequence_name  = '") + p_sequence + _T("'");
  return sql;
}

XString
SQLInfoOracle::GetCATALOGSequenceList(XString& p_schema,XString& p_pattern) const
{
  p_schema.MakeUpper();
  p_pattern.MakeUpper();
  if(!p_pattern.IsEmpty() && p_pattern != _T("%"))
  {
    p_pattern = _T("%") + p_pattern + _T("%");
  }
  XString sql = _T("SELECT ''              AS catalog_name\n")
                _T("      ,sequence_owner  AS schema_name\n")
                _T("      ,sequence_name\n")
                _T("      ,last_number     AS current_value\n")
                _T("      ,min_value       AS minimal_value\n")
                _T("      ,increment_by    AS seq_increment\n")
                _T("      ,cache_size                 AS cache\n")
                _T("      ,decode(cycle_flag,'N',0,1) AS cycle\n")
                _T("      ,decode(order_flag,'N',0,1) AS ordering\n")
                _T("  FROM all_sequences\n");
  if (!p_schema.IsEmpty())
  {
    sql += _T(" WHERE sequence_owner = ?\n");
  }
  if (!p_pattern.IsEmpty())
  {
    sql += p_schema.IsEmpty() ? _T(" WHERE ") : _T("   AND ");
    sql += _T("sequence_name  LIKE ?\n");
  }
  sql += _T(" ORDER BY 1,2,3");
  return sql;
}

XString
SQLInfoOracle::GetCATALOGSequenceAttributes(XString& p_schema,XString& p_sequence) const
{
  p_schema.MakeUpper();
  p_sequence.MakeUpper();

  XString sql = _T("SELECT ''              AS catalog_name\n")
                _T("      ,sequence_owner  AS schema_name\n")
                _T("      ,sequence_name\n")
                _T("      ,last_number     AS current_value\n")
                _T("      ,min_value       AS minimaml_value\n")
                _T("      ,increment_by    AS seq_increment\n")
                _T("      ,cache_size                 AS cache\n")
                _T("      ,decode(cycle_flag,'N',0,1) AS cycle\n")
                _T("      ,decode(order_flag,'N',0,1) AS ordering\n")
                _T("  FROM all_sequences\n");
  if(!p_schema.IsEmpty())
  {
    sql += _T(" WHERE sequence_owner = ?\n");
  }
  if(!p_sequence.IsEmpty())
  {
    sql += p_schema.IsEmpty() ? _T(" WHERE ") : _T("   AND ");
    sql += _T("sequence_name  = ?\n");
  }
  sql += _T(" ORDER BY 1,2,3");
  return sql;
}

XString
SQLInfoOracle::GetCATALOGSequenceCreate(MetaSequence& p_sequence) const
{
  XString sql(_T("CREATE SEQUENCE "));

  if (!p_sequence.m_schemaName.IsEmpty())
  {
    sql += p_sequence.m_schemaName + _T(".");
  }
  sql += p_sequence.m_sequenceName;
  sql.AppendFormat(_T("\n START WITH %-12.0f"), p_sequence.m_currentValue);
  sql.AppendFormat(_T("\n INCREMENT BY %d"), p_sequence.m_increment);

  sql += p_sequence.m_cycle ? _T("\n CYCLE") : _T("\n NOCYCLE");
  sql += p_sequence.m_order ? _T("\n ORDER") : _T("\n NOORDER");
  if (p_sequence.m_cache > 0)
  {
    sql.AppendFormat(_T("\n CACHE %d"),p_sequence.m_cache);
  }
  else
  {
    sql += _T("\n NOCACHE");
  }
  return sql;
}

XString
SQLInfoOracle::GetCATALOGSequenceDrop(XString p_schema, XString p_sequence) const
{
  XString sql(_T("DROP SEQUENCE ") + p_schema + _T(".") + p_sequence);
  return  sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL VIEW FUNCTIONS

XString 
SQLInfoOracle::GetCATALOGViewExists(XString& p_schema,XString& p_viewname) const
{
  p_schema.MakeUpper();
  p_viewname.MakeUpper();
  XString sql(_T("SELECT COUNT(*)\n")
              _T("  FROM all_views\n")
              _T(" WHERE owner     = ?\n")
              _T("   AND view_name = ?"));
  return sql;
}

XString 
SQLInfoOracle::GetCATALOGViewList(XString& p_schema,XString& p_pattern) const
{
  return GetCATALOGViewAttributes(p_schema,p_pattern);
}

XString 
SQLInfoOracle::GetCATALOGViewAttributes(XString& p_schema,XString& p_viewname) const
{
  p_schema.MakeUpper();
  p_viewname.MakeUpper();

  // VIEWS
  XString sql = _T("SELECT sys_context('USERENV','DB_NAME') AS table_catalog\n")
                _T("      ,viw.owner         AS table_schema\n")
                _T("      ,viw.view_name     AS table_name\n")
                _T("      ,'VIEW'            AS object_type\n")
                _T("      ,com.comments      AS remarks\n")
                _T("      ,''                AS tablespace_name\n")
                _T("      ,0                 AS TEMPORARY\n")
                _T("  FROM all_views viw LEFT OUTER JOIN all_tab_comments com\n")
                _T("                     ON (viw.owner     = com.owner\n")
                _T("                    AND  viw.view_name = com.table_name)\n")
                _T(" WHERE com.table_type = 'VIEW'\n");
  if(!p_schema.IsEmpty())
  {
    if(p_schema.Find('%') >= 0)
    {
      sql += _T("  AND viw.owner   LIKE ?\n");
    }
    else
    {
      sql += _T("  AND viw.owner   = ?\n");
    }
  }

  if(!p_viewname.IsEmpty())
  {
    if(p_viewname.Find('%') >= 0)
    {
      sql += _T("   AND viw.view_name   LIKE ?\n");
    }
    else
    {
      sql += _T("   AND viw.view_name   = ?\n");
    }
  }

  sql += _T(" ORDER BY 1,2,3");
  return sql;
}

XString
SQLInfoOracle::GetCATALOGViewText(XString& p_schema,XString& p_viewname) const
{
  p_schema.MakeUpper();
  p_viewname.MakeUpper();

  // BEWARE works from Oracle 19 onwards
  XString sql = _T("SELECT text\n")
                _T("  FROM all_views\n")
                _T(" WHERE owner = '") + p_schema + _T("'\n")
                _T("   AND view_name = '") + p_viewname + _T("'");
  return sql;
}

XString
SQLInfoOracle::GetCATALOGViewCreate(XString p_schema,XString p_viewname,XString p_contents,bool /*p_ifexists = true*/) const
{
  return _T("CREATE OR REPLACE VIEW ") + p_schema + _T(".") + p_viewname + _T(" AS\n") + p_contents;
}

XString 
SQLInfoOracle::GetCATALOGViewRename(XString p_schema,XString p_viewname,XString p_newname)    const
{
  return _T("");
}

XString 
SQLInfoOracle::GetCATALOGViewDrop(XString p_schema,XString p_viewname,XString& p_precursor) const
{
  p_precursor.Empty();
  return _T("DROP VIEW ") + p_schema + _T(".") + p_viewname;
}

// All Privilege functions
XString
SQLInfoOracle::GetCATALOGTablePrivileges(XString& p_schema,XString& p_tablename) const
{
  p_schema.MakeUpper();
  p_tablename.MakeUpper();
  bool pattern = p_tablename.Find('%') > 0;

  XString sql = _T("SELECT sys_context('USERENV','DB_NAME') AS table_catalog\n")
                _T("      ,pri.table_schema\n")
                _T("      ,pri.table_name\n")
                _T("      ,pri.grantor\n")
                _T("      ,pri.grantee\n")
                _T("      ,pri.privilege\n")
                _T("      ,pri.grantable\n")
                _T("  FROM all_tab_privs pri\n");

  // Add the filter
  if(!p_schema.IsEmpty())
  {
    sql += _T(" WHERE table_schema = '") + p_schema + _T("'\n");
  }
  if(!p_tablename.IsEmpty())
  {
    sql += p_schema.IsEmpty() ? _T(" WHERE ") : _T("   AND ");
    sql += XString(_T("table_name ")) + (pattern ? _T("LIKE '") : _T("= '")) + p_tablename + _T("'\n");
  }

  // Add owner privileges in the form of a union with the mapping
  if(!pattern)
  {
    // Add owner privileges in the form of a union with the mapping
    if(!p_schema.IsEmpty() && !p_tablename.IsEmpty())
    {
      sql.AppendFormat(_T("UNION\n")
                       _T("SELECT sys_context('USERENV','DB_NAME')\n")
                       _T("      ,'%s'\n")
                       _T("      ,'%s'\n")
                       _T("      ,'_SYSTEM'\n")
                       _T("      ,'%s'\n")
                       _T("      ,name\n")
                       _T("      ,'YES'\n")
                       _T("  FROM table_privilege_map\n")
                       ,p_schema.GetString()
                       ,p_tablename.GetString()
                       ,p_schema.GetString());
    }
  }
  // Special order
  sql += _T(" ORDER BY 1,2,3,5,6,4");
  return sql;
}

XString 
SQLInfoOracle::GetCATALOGColumnPrivileges(XString& p_schema,XString& p_tablename,XString& p_columnname) const
{
  p_schema.MakeUpper();
  p_tablename.MakeUpper();
  p_columnname.MakeUpper();

  XString sql = _T("SELECT sys_context('USERENV','DB_NAME') AS table_catalog\n")
                _T("      ,pri.table_schema\n")
                _T("      ,pri.table_name\n")
                _T("      ,pri.column_name\n")
                _T("      ,pri.grantor\n")
                _T("      ,pri.grantee\n")
                _T("      ,pri.privilege\n")
                _T("      ,pri.grantable\n")
                _T("  FROM all_col_privs pri\n");

  // Add the filter
  if(!p_schema.IsEmpty())
  {
    sql += _T(" WHERE table_schema = ?\n");
  }
  if(!p_tablename.IsEmpty())
  {
    sql += p_schema.IsEmpty() ? _T(" WHERE ") : _T("   AND ");
    sql += _T("table_name = ?\n");
  }
  if(!p_columnname.IsEmpty())
  {
    sql += (p_schema.IsEmpty() && p_tablename.IsEmpty()) ? _T(" WHERE ") : _T("   AND ");
    sql += _T("column_name = ?\n");
  }
  sql += _T(" ORDER BY 1,2,3,4,6,5,7");
  return sql;
}

XString
SQLInfoOracle::GetCATALOGSequencePrivilege(XString& /*p_schema*/,XString& /*p_sequence*/) const
{
  return _T("");
}

XString 
SQLInfoOracle::GetCATALOGGrantPrivilege(XString p_schema,XString p_objectname,XString p_privilege,XString p_grantee,bool p_grantable)
{
  XString sql;
  sql.Format(_T("GRANT %s ON %s.%s TO %s"),p_privilege.GetString(),p_schema.GetString(),p_objectname.GetString(),p_grantee.GetString());
  if(p_grantable)
  {
    sql += _T(" WITH GRANT OPTION");
  }
  return sql;
}

XString 
SQLInfoOracle::GetCATALOGRevokePrivilege(XString p_schema,XString p_objectname,XString p_privilege,XString p_grantee)
{
  XString sql;
  sql.Format(_T("REVOKE %s ON %s.%s FROM %s"),p_privilege.GetString(),p_schema.GetString(),p_objectname.GetString(),p_grantee.GetString());
  return sql;
}

// All Synonym functions
XString
SQLInfoOracle::GetCATALOGSynonymList(XString& /*p_schema*/,XString& /*p_pattern*/) const
{
  // Not implemented yet
  return _T("");
}

XString
SQLInfoOracle::GetCATALOGSynonymAttributes(XString& /*p_schema*/,XString& /*p_synonym*/) const
{
  // Not implemented yet
  return _T("");
}

XString
SQLInfoOracle::GetCATALOGSynonymCreate(XString& /*p_schema*/,XString& /*p_synonym*/,XString /*p_forObject*/,bool /*p_private = true*/) const
{
  // Not implemented yet
  return _T("");
}

XString
SQLInfoOracle::GetCATALOGSynonymDrop(XString& /*p_schema*/,XString& /*p_synonym*/,bool /*p_private = true*/) const
{
  // Not implemented yet
  return _T("");
}

//////////////////////////////////////////////////////////////////////////
//
// SQL/PSM PERSISTENT STORED MODULES 
//         Also called SPL or PL/SQL
// o GetPSM<Object[s]><Function>
//   -Procedures / Functions
//   - Exists					GetPSMProcedureExists
//   - List					  GetPSMProcedureList
//   - Attributes
//   - Create
//   - Drop
//
// o PSM<Element>[End]
//   - PSM Declaration(first,variable,datatype[,precision[,scale]])
//   - PSM Assignment (variable,statement)
//   - PSM IF         (condition)
//   - PSM IFElse 
//   - PSM IFEnd
//   - PSM WHILE      (condition)
//   - PSM WHILEEnd
//   - PSM LOOP
//   - PSM LOOPEnd
//   - PSM BREAK
//   - PSM RETURN     ([statement])
//
// o CALL the FUNCTION/PROCEDURE
//
//////////////////////////////////////////////////////////////////////////

XString
SQLInfoOracle::GetPSMProcedureExists(XString p_schema, XString p_procedure) const
{
  p_schema.MakeUpper();
  p_procedure.MakeUpper();
  return _T("SELECT COUNT(*)\n")
         _T("  FROM all_procedures\n")
         _T(" WHERE owner       = '") + p_schema + _T("'\n")
         _T("   AND object_name = '") + p_procedure + _T("'\n")
         _T("   AND object_type IN ('PROCEDURE','FUNCTION')");
}

XString
SQLInfoOracle::GetPSMProcedureList(XString& p_schema) const
{
  p_schema.MakeUpper();
  XString sql;

  sql = _T("SELECT sys_context('USERENV','DB_NAME') AS procedure_catalog\n")
        _T("      ,owner             AS procedure_schema\n")
        _T("      ,object_name       AS procedure_name\n")
        _T("      ,CASE object_type \n")
        _T("            WHEN 'PROCEDURE' THEN 1\n")
        _T("            WHEN 'FUNCTION'  THEN 2\n")
        _T("                             ELSE 3\n")
        _T("       END AS procedure_type\n")
        _T("  FROM all_procedures\n");
  if(!p_schema.IsEmpty())
  {
    sql += _T(" WHERE owner = ?\n");
  }
  sql += _T(" ORDER BY 1,2,3");
  return sql;
}

XString
SQLInfoOracle::GetPSMProcedureAttributes(XString& p_schema,XString& p_procedure) const
{
  p_schema.MakeUpper();
  p_procedure.MakeUpper();
  XString sql;
  sql = _T("SELECT sys_context('USERENV','DB_NAME') AS procedure_catalog\n")
        _T("      ,owner             AS procedure_schema\n")
        _T("      ,object_name       AS procedure_name\n")
        _T("      ,(SELECT COUNT(*)\n")
        _T("           FROM all_arguments par\n")
        _T("         WHERE par.owner       = pro.owner\n")
        _T("           AND par.object_name = pro.object_name\n")
        _T("           AND par.object_id   = pro.object_id\n")
        _T("           AND par.in_out IN ('IN','IN/OUT')) as input_params\n")
        _T("      ,(SELECT COUNT(*)\n")
        _T("          FROM all_arguments par\n")
        _T("         WHERE par.owner       = pro.owner\n")
        _T("           AND par.object_name = pro.object_name\n")
        _T("           AND par.object_id   = pro.object_id\n")
        _T("           AND par.in_out IN ('OUT','IN/OUT')) as output_params\n")
        _T("      ,0   AS result_sets\n")
        _T("      ,''  AS remarks\n")
        _T("      ,CASE object_type \n")
        _T("            WHEN 'PROCEDURE' THEN 1\n")
        _T("            WHEN 'FUNCTION'  THEN 2\n")
        _T("                             ELSE 3\n")
        _T("       END AS procedure_type\n")
        _T("      ,'<@>' as source\n")
        _T("  FROM all_procedures pro\n")
        _T(" WHERE object_type IN ('PROCEDURE','FUNCTION')\n");
  if(!p_schema.IsEmpty())
  {
    sql += _T("   AND owner = ?\n");
  }
  if(!p_procedure.IsEmpty())
  {
    sql += _T("   AND object_name ");
    sql += p_procedure.Find('%') >= 0 ? _T("LIKE") : _T("=");
    sql += _T(" ?\n");
  }
  sql += _T("ORDER BY 1,2,3");
  return sql;
}

XString
SQLInfoOracle::GetPSMProcedureSourcecode(XString p_schema, XString p_procedure) const
{
  p_schema.MakeUpper();
  p_procedure.MakeUpper();
  XString sql;

  sql = _T("SELECT 0 as object_id\n")
        _T("      ,0 as object_line\n")
        _T("      ,text\n")
        _T("  FROM all_source\n");
  if(!p_schema.IsEmpty())
  {
    sql += _T(" WHERE owner = '") + p_schema + _T("'\n");
  }
  if(!p_procedure.IsEmpty())
  {
    sql += p_schema.IsEmpty() ? _T(" WHERE ") : _T("   AND ");
    sql += _T(" name  = '") + p_procedure + _T("'\n");
  }
  sql += _T(" ORDER BY owner,name,line");
  return sql;
}

XString
SQLInfoOracle::GetPSMProcedureCreate(MetaProcedure& /*p_procedure*/) const
{
  return _T("");
}

XString
SQLInfoOracle::GetPSMProcedureDrop(XString p_schema, XString p_procedure,bool /*p_function /*=false*/) const
{
  return _T("");
}

XString
SQLInfoOracle::GetPSMProcedureErrors(XString p_schema,XString p_procedure) const
{
  XString query;
  XString errorText;
  p_schema.MakeUpper();
  p_procedure.MakeUpper();
  query = _T("SELECT line\n")
          _T("      ,position\n")
          _T("      ,text\n")
          _T("  FROM all_errors\n")
          _T(" WHERE name = '") + p_procedure +_T("'");

  SQLQuery qry1(m_database);
  SQLQuery qry2(m_database);
  qry1.DoSQLStatement(query);

  while (qry1.GetRecord())
  {
    XString s = qry1.GetColumn(3)->GetAsChar();
    if(s.Find(_T("Statement ignored")) < 0) 
    {
      s.Format(_T("Error in line %d, column %d: %s\n"),qry1.GetColumn(1)->GetAsSLong()
	                                              ,qry1.GetColumn(2)->GetAsSLong()
                                                  ,qry1.GetColumn(3)->GetAsChar());
      errorText += s;
      query.Format( _T("SELECT text\n")
                    _T("  FROM all_source\n")
                    _T(" WHERE type = 'FUNCTION'\n")
                    _T("   AND name = '%s'\n")
                    _T("   AND line = %d")
                   ,p_procedure.GetString()
                   ,qry1.GetColumn(1)->GetAsSLong());
      qry2.DoSQLStatement(query);
      while(qry2.GetRecord())
      {
        s.Format(_T("Line %d: %s\n")
                 ,qry1.GetColumn(1)->GetAsSLong()
                 ,qry2.GetColumn(1)->GetAsChar());
        errorText += s;
      }
    }
  }
  return errorText;
}

XString
SQLInfoOracle::GetPSMProcedurePrivilege(XString& /*p_schema*/,XString& /*p_procedure*/) const
{
  return _T("");
}

// And it's parameters
XString
SQLInfoOracle::GetPSMProcedureParameters(XString& /*p_schema*/,XString& /*p_procedure*/) const
{
  return _T("");
}

//////////////////////////////////////////////////////////////////////////
//
// ALL PSM LANGUAGE ELEMENTS
//
//////////////////////////////////////////////////////////////////////////

XString
SQLInfoOracle::GetPSMDeclaration(bool    p_first
                                ,XString p_variable
                                ,int     p_datatype
                                ,int     p_precision /*= 0 */
                                ,int     p_scale     /*= 0 */
                                ,XString p_default   /*= ""*/
                                ,XString p_domain    /*= ""*/
                                ,XString p_asColumn  /*= ""*/) const
{
  XString line;

  if(p_first)
  {
    line = _T("DECLARE\n");
  }
  line += p_variable + _T(" ");

  if(p_datatype)
  {
    // Getting type info and name
    const TypeInfo* info = GetTypeInfo(p_datatype);
    line += info->m_type_name;

    if(p_precision > 0)
    {
      line.AppendFormat(_T("(%d"),p_precision);
      if(p_scale > 0)
      {
        line.AppendFormat(_T("%d"),p_scale);
      }
      line += _T(")");
    }

    if(!p_default.IsEmpty())
    {
      line += _T(" DEFAULT ") + p_default;
    }
  }
  else if(!p_domain.IsEmpty())
  {
    line += p_domain;
  }
  else if(!p_asColumn)
  {
    line += p_asColumn + _T("%TYPE");
  }
  line += _T(";\n");
  return line;
}

XString
SQLInfoOracle::GetPSMAssignment(XString p_variable,XString p_statement /*=""*/) const
{
  XString line(p_variable);
  line += _T(" := ");
  if(!p_statement.IsEmpty())
  {
    line += p_statement;
    line += _T(";");
  }
  return line;
}

XString
SQLInfoOracle::GetPSMIF(XString p_condition) const
{
  XString line(_T("IF ("));
  line += p_condition;
  line += _T(") THEN\n");
  return line;
}

XString
SQLInfoOracle::GetPSMIFElse() const
{
  return _T("ELSE\n");
}

XString
SQLInfoOracle::GetPSMIFEnd() const
{
  return _T("END IF;\n");
}

XString
SQLInfoOracle::GetPSMWhile(XString p_condition) const
{
  return _T("WHILE (") + p_condition + _T(") LOOP\n");
}

XString
SQLInfoOracle::GetPSMWhileEnd() const
{
  return _T("END LOOP;\n");
}

XString
SQLInfoOracle::GetPSMLOOP() const
{
  return _T("LOOP\n");
}

XString
SQLInfoOracle::GetPSMLOOPEnd() const
{
  return _T("END LOOP;\n");
}

XString
SQLInfoOracle::GetPSMBREAK() const
{
  return _T("EXIT;\n");
}

XString
SQLInfoOracle::GetPSMRETURN(XString p_statement /*= ""*/) const
{
  XString line(_T("RETURN"));
  if(!p_statement.IsEmpty())
  {
    line += _T(" (") + p_statement + _T(")");
  }
  line += _T(";\n");
  return line;
}

XString
SQLInfoOracle::GetPSMExecute(XString p_procedure,MParameterMap& p_parameters) const
{
  XString line;
  line.Format(_T("EXECUTE %s("),p_procedure.GetString());
  bool doMore = false;

  for(const auto& param : p_parameters)
  {
    if(doMore) line += _T(",");
    doMore = true;

    line += param.m_parameter;
  }
  line += _T(");\n");
  return line;
}

// The CURSOR
XString
SQLInfoOracle::GetPSMCursorDeclaration(XString p_cursorname,XString p_select) const
{
  return _T("CURSOR ") + p_cursorname + _T(" IS ") + p_select + _T(";");
}

XString
SQLInfoOracle::GetPSMCursorFetch(XString p_cursorname,std::vector<XString>& /*p_columnnames*/,std::vector<XString>& p_variablenames) const
{
  bool moreThenOne = false;
  XString query = _T("OPEN  ") + p_cursorname + _T(";\n")
                  _T("FETCH ") + p_cursorname + _T(" INTO ");

  for(const auto& var : p_variablenames)
  {
    if(moreThenOne) query += _T(",");
    moreThenOne = true;
    query += var;
  }
  query += _T(";");
  return query;
}

//////////////////////////////////////////////////////////////////////////
// PSM Exceptions

XString
SQLInfoOracle::GetPSMExceptionCatchNoData() const
{
  return _T("EXCEPTION WHEN NO_DATA_FOUND THEN\n");
  // followed by STATEMENTBLOCK and "END;"
}

XString
SQLInfoOracle::GetPSMExceptionCatch(XString p_sqlState) const
{
  return _T("EXCEPTION WHEN ") + p_sqlState + _T(" THEN\n");
  // followed by STATEMENTBLOCK and "END;"
}

XString
SQLInfoOracle::GetPSMExceptionRaise(XString p_sqlState) const
{
  return _T("RAISE ") + p_sqlState;
}


//////////////////////////////////////////////////////////////////////////
//
// SESSIONS
// - Sessions (No create and drop)
//   - GetSessionMyself
//   - GetSessionExists
//   - GetSessionList
//   - GetSessionAttributes
// - Transactions
//   - GetSessionDeferredConstraints
//   - GetSessionImmediateConstraints
//
//////////////////////////////////////////////////////////////////////////

XString
SQLInfoOracle::GetSESSIONMyself() const
{
  XString sql = _T("SELECT audsid\n")
                _T("      ,osuser\n")
                _T("      ,terminal\n")
                _T("      ,prev_exec_start\n") // moment
                _T("      ,machine\n")         // remote 
                _T("      ,program\n")         // process name
                _T("      ,process\n")         // process ID
                _T("  FROM v$session\n")
                _T(" WHERE type   = 'USER'\n")
                _T("   AND audsid = sys_context('userenv','sessionid')");
  return sql;
}

XString
SQLInfoOracle::GetSESSIONExists(XString p_sessionID) const
{
  return _T("SELECT COUNT(*)\n")
         _T("  FROM v$session\n")
         _T(" WHERE audsid = ") + p_sessionID;
}

XString
SQLInfoOracle::GetSESSIONList() const
{
  return GetSESSIONAttributes(_T(""));
}

XString
SQLInfoOracle::GetSESSIONAttributes(XString p_sessionID) const
{
  XString sql = _T("SELECT audsid\n")
                _T("      ,osuser\n")
                _T("      ,terminal\n")
                _T("      ,prev_exec_start\n") // moment
                _T("      ,machine\n")         // remote 
                _T("      ,program\n")         // process name
                _T("      ,process\n")         // process ID
                _T("  FROM v$session\n")
                _T(" WHERE type = 'USER'\n");
  if(!p_sessionID.IsEmpty())
  {
    sql += _T("\n   AND audsid = ") + p_sessionID;
  }
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// Transactions

XString
SQLInfoOracle::GetSESSIONConstraintsDeferred() const
{
  // Set to DEFERRED for Oracle
  return _T("ALTER SESSION SET CONSTRAINTS = DEFERRED");
}

XString
SQLInfoOracle::GetSESSIONConstraintsImmediate() const
{
  // Set to IMMEDIATE for Oracle
  return _T("ALTER SESSION SET CONSTRAINTS = IMMEDIATE");
}

//////////////////////////////////////////////////////////////////////////
//
// Call FUNCTION/PROCEDURE from within program
// As a RDBMS dependent extension of "DoSQLCall" of the SQLQuery object
//
//////////////////////////////////////////////////////////////////////////

// Calling a stored function or procedure if the RDBMS does not support ODBC call escapes
SQLVariant*
SQLInfoOracle::DoSQLCall(SQLQuery* /*p_query*/,XString& /*p_schema*/,XString& /*p_procedure*/)
{
  return nullptr;
}

// Calling a stored function with named parameters, returning a value
SQLVariant*
SQLInfoOracle::DoSQLCallNamedParameters(SQLQuery* p_query,XString& p_schema,XString& p_procedure)
{
  XString sql = _T("BEGIN DECLARE x NUMBER; BEGIN ? := ");
  if(!p_schema.IsEmpty())
  {
    sql += p_schema;
    sql += _T(".");
  }
  sql += p_procedure;
  sql += _T("(");

  bool found = true;
  int index = 1;
  while(found)
  {
    XString name;
    found = p_query->GetColumnName(index++,name);
    if(found)
    {
      if(index > 2)
      {
        sql += _T(",");
      }
      sql += name;
      sql += _T(" => ? ");
    }
  }
  sql += _T("); END; END;");

  // Add parameter 0 as result parameter
  p_query->SetParameter(0,0);

  // Now find the result
  p_query->DoSQLStatement(sql);
  return p_query->GetParameter(0);
}

// End of namespace
}
