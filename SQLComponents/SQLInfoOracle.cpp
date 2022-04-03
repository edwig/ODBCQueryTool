////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoOracle.cpp
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
  return "Oracle";
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

  XString query = "SELECT SYS_CONTEXT('USERENV','DB_NAME')\n"
                  "  FROM DUAL";
  SQLQuery qry(m_database);
  qry.DoSQLStatement(query);
  if(qry.GetRecord())
  {
    return qry.GetColumn(1)->GetAsChar();
  }
  return XString("");
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
  if(p_precision == NUMERIC_MAX_PRECISION &&
     p_scale     == NUMERIC_MIN_SCALE)
  {
    p_scale = NUMERIC_DEFAULT_SCALE;
  }
}

// KEYWORDS

// Keyword for the current date and time
XString
SQLInfoOracle::GetKEYWORDCurrentTimestamp() const
{
  return "SYSDATE";
}

// String for the current date
XString
SQLInfoOracle::GetKEYWORDCurrentDate() const
{
  return "TRUNC(SYSDATE)";
}

// Get the concatenation operator
XString
SQLInfoOracle::GetKEYWORDConcatanationOperator() const
{
  return "||";
}

// Get quote character for strings
XString
SQLInfoOracle::GetKEYWORDQuoteCharacter() const
{
  return "\'";
}

// Get quote character around reserved words as an identifier
XString
SQLInfoOracle::GetKEYWORDReservedWordQuote() const
{
  return "\"";
}

// Get default NULL for parameter list input
XString
SQLInfoOracle::GetKEYWORDParameterDefaultNULL() const
{
  return "DEFAULT NULL";
}

// Parameter is for INPUT and OUTPUT in parameter list
XString
SQLInfoOracle::GetKEYWORDParameterINOUT() const
{
  return "IN OUT";
}

// Parameter is for OUTPUT only in parameter list
XString
SQLInfoOracle::GetKEYWORDParameterOUT() const
{
  return "OUT";
}

// Get datatype of the IDENTITY primary key in a Network database
XString
SQLInfoOracle::GetKEYWORDNetworkPrimaryKeyType() const
{
  // Use SEQUENCE to fill!
  return "INTEGER";
}

// Get datatype for timestamp (year to second)
XString
SQLInfoOracle::GetKEYWORDTypeTimestamp() const
{
  return "DATE";
}

// Prefix for a parameter in a stored procedure
XString
SQLInfoOracle::GetKEYWORDParameterPrefix() const
{
  return "";
}

// Get select part to add new record identity to a table
// Can be special column like 'OID' or a sequence select
XString
SQLInfoOracle::GetKEYWORDIdentityString(XString& p_tablename,XString p_postfix /*= "_seq"*/) const
{
  return p_tablename + p_postfix + ".nextval";
}

// Gets the UPPER function
XString
SQLInfoOracle::GetKEYWORDUpper(XString& p_expression) const
{
  return "UPPER(" + p_expression + ")";
}

// Gets the construction for 1 minute ago
XString
SQLInfoOracle::GetKEYWORDInterval1MinuteAgo() const
{
  return "(SYSDATE - 0.0007)";
}

// Gets the Not-NULL-Value statement of the database
XString
SQLInfoOracle::GetKEYWORDStatementNVL(XString& p_test,XString& p_isnull) const
{
  return XString("NVL(") + p_test + "," + p_isnull + ")";
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
    case SQL_WVARCHAR:                  type = "VARCHAR2";      
                                        break;
    case SQL_LONGVARCHAR:               // fall through
    case SQL_WLONGVARCHAR:              type = "CLOB";          
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
    case SQL_BIT:                       type = "NUMBER";
                                        p_column->m_columnSize    = 0;
                                        p_column->m_decimalDigits = 0;
                                        break;
  //case SQL_DATE:
    case SQL_DATETIME:                  // fall through
    case SQL_TYPE_DATE:                 // fall through
    case SQL_TIMESTAMP:                 // fall through
    case SQL_TYPE_TIMESTAMP:            type = "DATE";
                                        p_column->m_columnSize    = 0;
                                        p_column->m_decimalDigits = 0;
                                        break;
    case SQL_TIME:                      // fall through
    case SQL_TYPE_TIME:                 type = "TIMESTAMP";
                                        p_column->m_columnSize = 0;
                                        p_column->m_decimalDigits = 0;
                                        break;
    case SQL_BINARY:                    type = "BLOB";          break;
    case SQL_VARBINARY:                 type = "BLOB";          break;
    case SQL_LONGVARBINARY:             type = "BLOB";          break;
    case SQL_GUID:                      type = "GUID";          break;
    case SQL_INTERVAL_YEAR:             // fall through
    case SQL_INTERVAL_YEAR_TO_MONTH:    // fall through
    case SQL_INTERVAL_MONTH:            type = "VARCHAR2";
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
    case SQL_INTERVAL_DAY_TO_SECOND:    type = "VARCHAR2";
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
  return "USER";
}

// Connects to a default schema in the database/instance
XString
SQLInfoOracle::GetSQLDefaultSchema(XString p_schema) const
{
  return "ALTER SESSION SET CURRENT_SCHEMA = " + p_schema;
}

// Gets the construction for inline generating a key within an INSERT statement
XString
SQLInfoOracle::GetSQLNewSerial(XString p_table, XString p_sequence) const
{
  XString sequence(p_sequence);
  if (sequence.IsEmpty() && !p_table.IsEmpty())
  {
    sequence = p_table + "_seq";
  }

  // Select next value from a generator sequence
  return sequence + ".NEXTVAL";
}

// Gets the construction / select for generating a new serial identity
XString
SQLInfoOracle::GetSQLGenerateSerial(XString p_table) const
{
  return "SELECT " + p_table + "_seq.nextval FROM DUAL";
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
  return XString("SAVEPOINT ") + p_savepointName;
}

XString
SQLInfoOracle::GetSQLCommitSubTransaction(XString p_savepointName) const
{
  // There is no savepoint commit in Oracle!!
  return "";
}

XString
SQLInfoOracle::GetSQLRollbackSubTransaction(XString p_savepointName) const
{
  return XString("ROLLBACK TO ") + p_savepointName;
}

// FROM-Part for a query to select only 1 (one) record / or empty!
XString
SQLInfoOracle::GetSQLFromDualClause() const
{
  return " FROM dual";
}

// Get SQL to lock  a table 
XString
SQLInfoOracle::GetSQLLockTable(XString p_schema, XString p_tablename, bool p_exclusive) const
{
  XString query = "LOCK TABLE " + p_schema + "." + p_tablename + " IN ";
  query += p_exclusive ? "EXCLUSIVE" : "SHARE";
  query += " MODE";
  return query;
}

// Get query to optimize the table statistics
XString
SQLInfoOracle::GetSQLOptimizeTable(XString p_schema, XString p_tablename) const
{
  XString optim;
  // Optimize the table
  optim = "call dbms_stats.gather_table_stats('" + p_schema + "','" + p_tablename + "')";
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
      limit.Format("\n OFFSET %d ROWS"
                   "\n FETCH NEXT %d ROWS ONLY"
                   ,p_skip
                   ,p_top);
    }
    else
    {
      limit.Format("\n FETCH FIRST %d ROWS ONLY",p_top);
    }
    p_sql += limit;
  }
  return p_sql;
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
  s.Replace("'","''");
  XString kwoot = GetKEYWORDQuoteCharacter();
  return  kwoot + s + kwoot;
}

// Get date string in engine format
XString
SQLInfoOracle::GetSQLDateString(int p_year,int p_month,int p_day) const
{
  XString dateString;
  dateString.Format("TO_DATE('%04d-%02d-%02d','YYYY-MM-DD')",p_year,p_month,p_day);
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
  timeString.Format("%0.15f",val);

  return timeString;
}

// Get date-time string in database engine format
XString
SQLInfoOracle::GetSQLDateTimeString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  XString string;
  string.Format("TO_DATE('%04d-%02d-%02d %02d:%02d:%02d','YYYY-MM-DD HH24:MI:SS')"
                ,p_year,p_month,p_day
                ,p_hour,p_minute,p_second);
  return string;
}

// For parameter binding of a date-time string
XString
SQLInfoOracle::GetSQLDateTimeBoundString() const
{
  return "TO_DATE(?,'YYYY-MM-DD HH24:MI:SS')";
}

// Stripped data for the parameter binding
XString
SQLInfoOracle::GetSQLDateTimeStrippedString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  XString string;
  string.Format("%04d-%02d-%02d %02d:%02d:%02d"
                ,p_year,p_month,p_day
                ,p_hour,p_minute,p_second);
  return string;
}

//////////////////////////////////////////////////////////////////////////
//
// CATALOG
// o GetCATALOG<Object[s]><Function>
//   Objects
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
    case META_CATALOGS: sql = "SELECT sys_context('USERENV','DB_NAME') AS catalog\n"
                              "      ,'Current catalog'                AS remarks\n"
                              "  FROM dual\n"
                              "UNION\n"
                              "SELECT CASE InStr(db_link,'.')\n"
                              "            WHEN 0 THEN db_link\n"
                              "                   ELSE SubStr(db_link, 1, InStr(db_link, '.') - 1)\n"
                              "       END\n"
                              "      ,db_link\n"
                              "  FROM dba_db_links\n"
                              " WHERE owner <> 'SYS'";
                        break;
    case META_SCHEMAS:  sql = "SELECT DISTINCT username\n"
                              "      ,'' AS remarks\n"
                              "  FROM all_users\n"
                              " WHERE oracle_maintained = 'N'";
                        break;
    case META_TABLES:   sql = "SELECT 'TABLE' AS table_type\n"
                              "      ,''      AS remarks\n"
                              "  FROM dual\n"
                              "UNION\n"
                              "SELECT 'VIEW'\n"
                              "      ,''\n"
                              "  FROM dual";
                        break;
  }
  return sql;
}

// Get SQL to check if a table already exists in the database
XString
SQLInfoOracle::GetCATALOGTableExists(XString& p_schema,XString& p_tablename) const
{
  p_schema.MakeUpper();
  p_tablename.MakeUpper();
  XString query = "SELECT COUNT(*)\n"
                  "  FROM all_tables\n"
                  " WHERE owner      = ?\n"
                  "   AND table_name = ?";
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
  sql  = "SELECT sys_context('USERENV','DB_NAME') AS table_catalog\n"
         "      ,tab.owner         AS table_schema\n"
         "      ,tab.table_name    AS table_name\n"
         "      ,CASE tab.TEMPORARY\n"
         "            WHEN 'Y' THEN 'GLOBAL TEMPORARY'\n"
         "            WHEN 'N' THEN 'TABLE'\n"
         "                     ELSE 'UNKNOWN'\n"
         "       END               AS object_type\n"
         "      ,com.comments      AS remarks\n"
         "      ,tab.tablespace_name\n"
         "      ,CASE tab.TEMPORARY\n"
         "            WHEN 'Y' THEN 1\n"
         "            WHEN 'N' THEN 0\n"
         "                     ELSE -1\n"
         "       END AS temporary\n"
         "  FROM all_tables tab LEFT OUTER JOIN all_tab_comments com\n"
         "                      ON (com.owner      = tab.owner\n"
         "                     AND  com.table_name = tab.table_name)\n"
         " WHERE com.table_type = 'TABLE'\n";


  if(!p_schema.IsEmpty())
  {
    sql += "   AND tab.owner ";
    sql += p_schema.Find('%') >= 0 ? "LIKE" : "=";
    sql += " ?\n";
  }

  if(!p_tablename.IsEmpty())
  {
    sql += "   AND tab.table_name ";
    sql += p_tablename.Find('%') >= 0 ? "LIKE" : "=";
    sql += " ?\n";
  }
  sql += " ORDER BY 1,2,3";

  return sql;
}

XString 
SQLInfoOracle::GetCATALOGTableSynonyms(XString& p_schema,XString& p_tablename) const
{
  p_schema.MakeUpper();
  p_tablename.MakeUpper();

  XString sql = "SELECT sys_context('USERENV','DB_NAME') AS table_catalog\n"
                "      ,table_owner  AS table_schema\n"
                "      ,synonym_name AS table_name\n"
                "      ,'SYNONYM'    AS object_type\n"
                "      ,''           AS remarks\n"
                "      ,''           AS tablespace_name\n"
                "      ,0            AS temporary\n"
                "  FROM all_synonyms syn\n";

  if(!p_schema.IsEmpty())
  {
    sql += " WHERE table_owner ";
    sql += p_schema.Find('%') >= 0 ? "LIKE" : "=";
    sql += " ?\n";
  }

  if(!p_tablename.IsEmpty())
  {
    sql += p_schema.IsEmpty() ? " WHERE " : "   AND ";
    sql += "synonym_name ";
    sql += p_tablename.Find('%') >= 0 ? "LIKE" : "=";
    sql += " ?\n";
  }
  sql += " ORDER BY 1,2,3";
  return sql;
}

XString
SQLInfoOracle::GetCATALOGTableCatalog(XString& p_schema,XString& p_tablename) const
{
  p_schema.MakeUpper();
  p_tablename.MakeUpper();

  XString sql = "SELECT sys_context('USERENV','DB_NAME') AS table_catalog\n"
                "      ,obj.owner         AS table_schema\n"
                "      ,obj.object_name   AS table_name\n"
                "      ,'SYSTEM TABLE'    AS object_type\n"
                "      ,com.comments      AS remarks\n"
                "      ,''                AS tablespace_name\n"
                "      ,0                 AS temporary\n"
                "  FROM all_objects obj LEFT OUTER JOIN all_tab_comments com\n"
                "                       ON (com.owner      = obj.owner\n"
                "                      AND  com.table_name = obj.object_name)\n"
                " WHERE obj.owner IN ('SYS','SYSTEM')\n"
                "   AND obj.object_type IN ('TABLE','VIEW')\n"
                "   AND obj.object_name NOT LIKE ('%$%')\n";

  if(!p_schema.IsEmpty())
  {
    if(p_schema.Find('%') >= 0)
    {
      sql += "  AND obj.owner   LIKE ?\n";
    }
    else
    {
      sql += "  AND obj.owner   = ?\n";
    }
  }

  if(!p_tablename.IsEmpty())
  {
    if(p_tablename.Find('%') >= 0)
    {
      sql += "   AND obj.object_name LIKE ?\n";
    }
    else
    {
      sql += "   AND obj.object_name = ?\n";
    }
  }

  sql += " ORDER BY 1,2,3";
  return sql;
}


XString
SQLInfoOracle::GetCATALOGTableCreate(MetaTable& p_table,MetaColumn& /*p_column*/) const
{
  XString sql = "CREATE ";
  if (p_table.m_temporary)
  {
    sql += "GLOBAL TEMPORARY ";
  }
  sql += "TABLE ";
  if (!p_table.m_schema.IsEmpty())
  {
    sql += p_table.m_schema;
    sql += ".";
  }
  sql += p_table.m_table;
  return sql;
}

XString
SQLInfoOracle::GetCATALOGTableCreatePostfix(MetaTable& /*p_table*/,MetaColumn& /*p_column*/) const
{
  return "";
}

XString
SQLInfoOracle::GetCATALOGTableRename(XString p_schema,XString p_tablename,XString p_newname) const
{
  // Beware: No 'TABLE' in the statement
  XString sql("RENAME " + p_schema + "." + p_tablename + " TO " + p_newname);
  return sql;
}

XString
SQLInfoOracle::GetCATALOGTableDrop(XString p_schema,XString p_tablename,bool /*p_ifExist = false*/,bool /*p_restrict = false*/,bool p_cascade /*= false*/) const
{
  XString sql("DROP TABLE ");
  if(!p_schema.IsEmpty())
  {
    sql += p_schema + ".";
  }
  sql += p_tablename;
  if(p_cascade)
  {
    sql += " CASCADE CONSTRAINTS";
  }
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL TEMPORARY TABLE FUNCTIONS

XString 
SQLInfoOracle::GetCATALOGTemptableCreate(XString p_schema,XString p_tablename,XString p_select) const
{
  return "CREATE GLOBAL TEMPORARY TABLE " + p_schema + "." + p_tablename + "\nAS " + p_select +
         "ON COMMIT PRESERVE ROWS";
}

XString 
SQLInfoOracle::GetCATALOGTemptableIntoTemp(XString p_schema,XString p_tablename,XString p_select) const
{
  return "INSERT INTO " + p_schema + "." + p_tablename + "\n" + p_select;
}

XString 
SQLInfoOracle::GetCATALOGTemptableDrop(XString p_schema,XString p_tablename) const
{
  XString tablename = p_schema + "." + p_tablename;
  return "DELETE FROM "    + tablename + ";\n"
         "<@>\n"
         "TRUNCATE TABLE " + tablename + ";\n"
         "<@>\n"
         "DROP TABLE "     + tablename + ";\n";
}

//////////////////////////////////////////////////////////////////////////
// ALL COLUMN FUNCTIONS

XString 
SQLInfoOracle::GetCATALOGColumnExists(XString p_schema,XString p_tablename,XString p_columnname) const
{
  p_schema.MakeUpper();
  p_tablename.MakeUpper();
  p_columnname.MakeUpper();
  XString query = "SELECT COUNT(*)\n"
                 "  FROM all_tab_columns tab\n"
                 " WHERE tab.table_name  = '" + p_tablename  + "'\n"
                 "   AND tab.column_name = '" + p_columnname + "'\n"
                 "   AND tab.owner       = '" + p_schema     + "'";
  return query;
}

XString 
SQLInfoOracle::GetCATALOGColumnList(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  // Standard ODBC driver suffices
  return "";
}

XString 
SQLInfoOracle::GetCATALOGColumnAttributes(XString& p_schema,XString& p_tablename,XString& p_columnname) const
{
  p_schema.MakeUpper();
  p_tablename.MakeUpper();
  p_columnname.MakeUpper();

  XString sql = "SELECT sys_context('USERENV','DB_NAME') AS col_catalog\n"
                "      ,col.owner                        AS col_owner\n"
                "      ,col.table_name                   AS col_table\n"
                "      ,col.column_name                  AS col_column_name\n"
                "      ,CASE SubStr(col.data_type,1,10)\n"
                "            WHEN 'CHAR'          THEN  1\n"
                "            WHEN 'VARCHAR2'      THEN 12\n"
                "            WHEN 'NCHAR'         THEN -8\n"
                "            WHEN 'NVARCHAR'      THEN -9\n"
                "            WHEN 'NUMBER'        THEN CASE data_scale\n"
                "                                           WHEN 0   THEN 4\n"
                "                                                    ELSE 2\n"
                "                                      END\n"
                "            WHEN 'FLOAT'         THEN CASE data_precision\n"
                "                                           WHEN 63  THEN 7\n"
                "                                                    ELSE 8\n"
                "                                      END\n"
                "            WHEN 'BINARY_FLO'    THEN  7\n"
                "            WHEN 'BINARY_DOU'    THEN  8\n"
                "            WHEN 'DATE'          THEN 93\n"
                "            WHEN 'TIMESTAMP('    THEN 93\n"
                "            WHEN 'INTERVAL Y'    THEN 107\n"
                "            WHEN 'INTERVAL D'    THEN 110\n"
                "            WHEN 'CLOB'          THEN -1\n"
                "            WHEN 'RAW'           THEN -3\n"
                "            WHEN 'LONG'          THEN -1\n"
                "            WHEN 'BLOB'          THEN -4\n"
                "            WHEN 'NCLOB'         THEN -10\n"
                "                                 ELSE  0\n"
                "       END                             AS col_datatype\n"
                "      ,col.data_type                   AS col_typename\n"
                "      ,CASE SubStr(col.data_type,1,10)\n"
                "            WHEN 'LONG'       THEN 2147483647\n"
                "            WHEN 'CLOB'       THEN 2147483647\n"
                "            WHEN 'BLOB'       THEN 2147483647\n"
                "            WHEN 'NCLOB'      THEN 2147483647\n"
                "            WHEN 'DATE'       THEN 19\n"
                "            WHEN 'TIMESTAMP(' THEN 19\n"
                "            WHEN 'INTERVAL Y' THEN 52\n"
                "            WHEN 'INTERVAL D' THEN 52\n"
                "            WHEN 'BINARY_DOU' THEN 15\n"
                "            WHEN 'BINARY_FLO' THEN  7\n"
                "            WHEN 'NCHAR'      THEN char_col_decl_length\n"
                "            WHEN 'NVARCHAR'   THEN char_col_decl_length\n"
                "            WHEN 'NUMBER'     THEN NVL(data_precision,38)\n"
                "                              ELSE col.data_length\n"
                "       END                             AS col_columnsize\n"
                "      ,CASE SubStr(col.data_type,1,8)\n"
                "            WHEN 'LONG'     THEN 2147483647\n"
                "            WHEN 'CLOB'     THEN 2147483647\n"
                "            WHEN 'BLOB'     THEN 2147483647\n"
                "            WHEN 'NCLOB'    THEN 2147483647\n"
                "            WHEN 'DATE'     THEN 16\n"
                "            WHEN 'TIMESTAM' THEN 16\n"
                "                            ELSE col.data_length\n"
                "       END                             AS col_bufferlength\n"
                "      ,col.data_scale                  AS col_decimaldigits\n"
                "      ,CASE col.data_type\n"
                "            WHEN 'NUMBER'        THEN 10\n"
                "            WHEN 'FLOAT'         THEN 10\n"
                "            WHEN 'BINARY_FLOAT'  THEN 10\n"
                "            WHEN 'BINARY_DOUBLE' THEN 10\n"
                "                                 ELSE  0\n"
                "       END                             AS col_numradix\n"
                "      ,CASE nullable\n"
                "            WHEN 'N'      THEN 0\n"   //-- SQL_NO_NULLS
                "            WHEN 'Y'      THEN 1\n"   //-- SQL_NULLABLE
                "                          ELSE 2\n"   //-- SQL_NULLABLE_UNKNOWN
                "       END                             AS col_nullable\n"
                "      ,com.comments                    AS col_remarks\n"
                "      ,col.data_default                AS col_default\n"
                "      ,CASE SubStr(col.data_type,1,8)\n"
                "            WHEN 'CHAR'     THEN  1\n"
                "            WHEN 'VARCHAR2' THEN 12\n"
                "            WHEN 'NCHAR'    THEN -8\n"
                "            WHEN 'NVARCHAR' THEN -9\n"
                "            WHEN 'NUMBER'   THEN CASE data_scale\n"
                "                                      WHEN 0   THEN 4\n"
                "                                               ELSE 2\n"
                "                                 END\n"
                "            WHEN 'FLOAT'    THEN CASE data_precision\n"
                "                                      WHEN 63  THEN 7\n"
                "                                               ELSE 8\n"
                "                                 END\n"
                "            WHEN 'BINARY_F' THEN  7\n"
                "            WHEN 'BINARY_D' THEN  8\n"
                "            WHEN 'DATE'     THEN  9\n"
                "            WHEN 'TIMESTAM' THEN  9\n"
                "            WHEN 'INTERVAL' THEN -4\n"
                "            WHEN 'CLOB'     THEN -1\n"
                "            WHEN 'LONG'     THEN -1\n"
                "            WHEN 'BLOB'     THEN -4\n"
                "            WHEN 'NCLOB'    THEN -10\n"
                "                            ELSE  0\n"
                "       END                             AS col_datatype3\n"
                "      ,CASE SubStr(col.data_type,1,8)\n"
                "            WHEN 'DATE'     THEN 3\n"
                "            WHEN 'TIMESTAM' THEN 3\n"
                "                            ELSE 0\n"
                "       END                             AS col_subdatatype\n"
                "      ,CASE col.data_type\n"
                "            WHEN 'CHAR'      THEN data_length\n"
                "            WHEN 'VARCHAR2'  THEN data_length\n"
                "            WHEN 'NCHAR'     THEN data_length\n"
                "            WHEN 'NVARCHAR2' THEN data_length\n"
                "                             ELSE 0\n"
                "       END                             AS col_octetlength\n"
                "      ,col.column_id                   AS col_position\n"
                "      ,CASE col.nullable\n"
                "            WHEN 'Y' THEN 'YES'\n"
                "                     ELSE 'NO'\n"
                "       END                            AS col_isnullable\n"
                "  FROM all_tab_columns col\n"
                "       LEFT OUTER JOIN all_col_comments com ON (col.owner       = com.owner\n"
                "                                            AND col.table_name  = com.table_name\n"
                "                                            AND col.column_name = com.column_name)\n";

  if(!p_schema.IsEmpty())
  {
    sql.AppendFormat(" WHERE col.owner      = '%s'\n",p_schema.GetString());
  }
  if(!p_tablename.IsEmpty())
  {
    sql += p_schema.IsEmpty() ? " WHERE " : "   AND ";
    sql.AppendFormat("col.table_name = '%s'\n",p_tablename.GetString());
  }
  if(!p_columnname.IsEmpty())
  {
    sql += p_schema.IsEmpty() && p_tablename.IsEmpty() ? " WHERE " : "   AND ";
    sql.AppendFormat("col.column_name = '%s'\n",p_columnname.GetString());
  }
  sql += " ORDER BY 1,2,3,17";
  return sql;
}

XString 
SQLInfoOracle::GetCATALOGColumnCreate(MetaColumn& p_column) const
{
  XString sql = "ALTER TABLE "  + p_column.m_schema + "." + p_column.m_table  + "\n"
                "  ADD COLUMN " + p_column.m_column + " " + p_column.m_typename;
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
  XString sql = "ALTER  TABLE  " + p_column.m_schema + "." + p_column.m_table  + "\n"
                "MODIFY COLUMN " + p_column.m_column + " " + p_column.m_typename;
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
  XString sql("ALTER  TABLE  " + p_schema + "." + p_tablename + "\n"
              "RENAME COLUMN " + p_columnname + " TO " + p_newname + "\n");
  return sql;
}

XString
SQLInfoOracle::GetCATALOGColumnDrop(XString p_schema,XString p_tablename,XString p_columnname) const
{
  XString sql("ALTER TABLE " + p_schema + "." + p_tablename + "\n"
              " DROP COLUMN " + p_columnname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL INDICES FUNCTIONS

// All index functions
XString
SQLInfoOracle::GetCATALOGIndexExists(XString p_schema,XString p_tablename,XString p_indexname) const
{
  return "";
}

XString
SQLInfoOracle::GetCATALOGIndexList(XString& p_schema,XString& p_tablename)   const
{
  p_schema.MakeUpper();
  p_tablename.MakeUpper();

  // Query runs on the 'all_' variant, so owner name must be taken into account
  // for performance reasons on the system table
  XString query = "SELECT idx.index_name\n"
                  "      ,col.column_name\n"
                  "      ,col.column_position\n"
//                "      ,idx.index_type\n"
                  "      ,idx.uniqueness\n"
                  "      ,col.descend\n"
                  "      ,'' as column_source"
                  "  FROM all_indexes     idx\n"
                  "      ,all_ind_columns col\n"
                  " WHERE idx.index_name  = col.index_name\n"
                  "   AND idx.table_name  = col.table_name\n"
                  "   AND idx.table_owner = col.table_owner\n"
                  "   AND idx.generated   = 'N'\n"
                  "   AND idx.table_owner = ?\n"
                  "   AND idx.table_name  = ?\n"
                  " ORDER BY index_name\n"
                  "         ,column_position\n";
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
  if(p_indexname.Compare("0") == 0)
  {
    // Create the TABLE_STATISTICS QUERY
    tablestatistics = true;
    p_indexname.Empty();
    query = "SELECT sys_context('USERENV','DB_NAME') AS table_catalog\n"
            "      ,ind.owner                        AS table_schema\n"
            "      ,ind.table_name                   AS table_name\n"
            "      ,NULL          AS non_unique\n"
            "      ,NULL          AS index_name\n"
            "      ,0             AS index_type\n"  //-- SQL_TABLE_STAT
            "      ,NULL          AS ordinal_position\n"
            "      ,NULL          AS column_name\n"
            "      ,NULL          AS asc_or_desc\n"
            "      ,ind.num_rows  AS cardinality\n"
            "      ,ind.blocks    AS pages\n"
            "      ,NULL          AS filter\n"
            "  FROM all_tables ind";
  }
  else
  {
    // Create the TABLE_INDEXES QUERY
    query = "SELECT sys_context('USERENV','DB_NAME') AS table_catalog\n"
            "      ,ind.table_owner                  AS table_schema\n"
            "      ,ind.table_name                   AS table_name\n"
            "      ,CASE ind.uniqueness\n"
            "            WHEN 'UNIQUE' THEN 0\n"
            "                          ELSE 1\n"
            "       END                     AS non_unique\n"
            "      ,ind.index_name          AS index_name\n"
            "      ,3                       AS index_type\n"  //  -- SQL_INDEX_OTHER
            "      ,col.column_position     AS ordinal_position\n"
            "      ,col.column_name         AS column_name\n"
            "      ,substr(col.descend,1,1) AS asc_or_desc\n"
            "      ,ind.num_rows            AS cardinality\n"
            "      ,ind.leaf_blocks         AS pages\n"
            "      ,exp.column_expression   AS filter\n"
            "  FROM all_indexes ind\n"
            "       inner join\n"
            "       all_ind_columns col ON (col.table_name = ind.table_name\n"
            "                           AND ind.index_name = col.index_name)\n"
            "       left OUTER JOIN\n"
            "       all_ind_expressions Exp ON (Exp.table_name      = col.table_name\n"
            "                               AND Exp.index_name      = col.index_name\n"
            "                               AND Exp.column_position = col.column_position)\n";
  }
  if(!p_schema.IsEmpty())
  {
    if(tablestatistics)
    {
      query += " WHERE ind.owner = ?\n";
    }
    else
    {
      query += " WHERE ind.table_owner = ?\n";
    }
  }
  if(!p_tablename.IsEmpty())
  {
    query += p_schema.IsEmpty() ? " WHERE " : "   AND ";
    query += "ind.table_name = ?\n";
  }
  if(!p_indexname.IsEmpty())
  {
    query += p_schema.IsEmpty() && p_tablename.IsEmpty() ? " WHERE " : "   AND ";
    query += "ind.index_name = ?\n";
  }
  query += " ORDER BY 1,2,3,5,7";
  return query;
}

XString
SQLInfoOracle::GetCATALOGIndexCreate(MIndicesMap& p_indices) const
{
  // Get SQL to create an index for a table
  // CREATE [UNIQUE] INDEX [<schema>.]indexname ON [<schema>.]tablename(column [ASC|DESC] [,...]);
  XString query;
  for(auto& index : p_indices)
  {
    if(index.m_position == 1)
    {
      // New index
      query = "CREATE ";
      if(index.m_nonunique == false)
      {
        query += "UNIQUE ";
      }
      query += "INDEX ";
      if(!index.m_schemaName.IsEmpty())
      {
        query += index.m_schemaName + ".";
      }
      query += index.m_indexName;
      query += " ON ";
      if(!index.m_schemaName.IsEmpty())
      {
        query += index.m_schemaName + ".";
      }
      query += index.m_tableName;
      query += "(";
    }
    else
    {
      query += ",";
    }
    if(!index.m_filter.IsEmpty())
    {
      query += index.m_filter;
    }
    else
    {
      query += index.m_columnName;
      if(index.m_ascending != "A")
      {
        query += " DESC";
      }
    }
  }
  query += ")";
  return query;
}

XString
SQLInfoOracle::GetCATALOGIndexDrop(XString p_schema,XString /*p_tablename*/,XString p_indexname) const
{
  XString sql = "DROP INDEX " + p_schema + "." + p_indexname;
  return sql;
}

// Get extra filter expression for an index column
XString
SQLInfoOracle::GetCATALOGIndexFilter(MetaIndex& p_index) const
{
  XString expression;
  XString sql;
  sql.Format("SELECT column_expression\n"
             "  FROM all_ind_expressions\n"
             " WHERE index_owner = '" + p_index.m_schemaName + "'\n"
             "   AND index_name  = '" + p_index.m_indexName + "'\n"
             "   AND table_owner = '" + p_index.m_schemaName + "'\n"
             "   AND table_name  = '" + p_index.m_tableName + "'\n"
             "   AND column_position = %d",p_index.m_position);
  try
  {
    SQLQuery qry(m_database);
    SQLVariant* var = qry.DoSQLStatementScalar(sql);
    if(var)
    {
      var->GetAsString(expression);
    }
  }
  catch(StdException& error)
  {
    ReThrowSafeException(error);
    XString message = ("Cannot find index filter: ") + error.GetErrorMessage();
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

  XString query = "SELECT c.constraint_name\n"
                  "      ,c.index_name\n"
                  "      ,'Y' as deferrable\n"
                  "      ,'N' as initially_deferred\n"
                  "  FROM all_constraints c\n"
                  " WHERE c.owner           = '" + p_schema    + "'\n"
                  "   AND c.table_name      = '" + p_tablename + "'\n"
                  "   AND c.constraint_type = 'P'\n";
  return query;
}

XString
SQLInfoOracle::GetCATALOGPrimaryAttributes(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  // To be implemented
  return "";
}

XString
SQLInfoOracle::GetCATALOGPrimaryCreate(MPrimaryMap& p_primaries) const
{
  XString query("ALTER TABLE ");

  for(auto& prim : p_primaries)
  {
    if(prim.m_columnPosition == 1)
    {
      if(!prim.m_schema.IsEmpty())
      {
        query += prim.m_schema + ".";
      }
      query += prim.m_table + "\n";
      query += "  ADD CONSTRAINT " + prim.m_constraintName + "\n";
      query += "      PRIMARY KEY (";

    }
    else
    {
      query += ",";
    }
    query += prim.m_columnName;
  }
  query += ")";
  return query;
}

XString
SQLInfoOracle::GetCATALOGPrimaryDrop(XString p_schema,XString p_tablename,XString p_constraintname) const
{
  XString sql("ALTER TABLE " + p_schema + "." + p_tablename + "\n"
              " DROP CONSTRAINT " + p_constraintname);
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
  sql.Format("SELECT COUNT(*)\n"
             "  FROM all_constraints con\n"
             " WHERE con.constraint_type = 'R'"
             "   AND con.owner           = '" + p_schema + "'\n"
             "   AND con.table_name      = '" + p_tablename + "'\n"
             "   AND con.constraint_name = '" + p_constraintname + "'"
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
    throw StdException("Cannot get table references without schema/tablename");
  }

  XString query = "SELECT sys_context('USERENV','DB_NAME') AS primary_catalog_name\n"
                  "      ,pri.owner                        AS primary_schema_name\n"
                  "      ,pri.table_name                   AS primary_table_name\n"
                  "      ,sys_context('USERENV','DB_NAME') AS foreign_catalog_name\n"
                  "      ,con.owner            AS foreign_schema_name\n"
                  "      ,con.table_name       AS table_name\n"
                  "      ,pri.constraint_name  AS primary_key_constraint\n"
                  "      ,con.constraint_name  AS foreign_key_constraint\n"
                  "      ,col.position         AS key_sequence\n"
                  "      ,pky.column_name      AS primary_key_column\n"
                  "      ,col.column_name      AS foreign_key_column\n"
                  "      ,0                    AS update_rule\n"
                  "      ,CASE con.delete_rule WHEN 'RESTRICT'    THEN 1\n"
                  "                            WHEN 'CASCADE'     THEN 0\n"
                  "                            WHEN 'SET NULL'    THEN 2\n"
                  "                            WHEN 'SET DEFAULT' THEN 4\n"
                  "                            WHEN 'NO ACTION'   THEN 3\n"
                  "                            ELSE 0 END AS delete_rule\n"
                  "      ,CASE con.deferrable  WHEN 'NOT DEFERRABLE' THEN 0\n"
                  "                            WHEN 'DEFERRABLE'     THEN 1\n"
                  "                            ELSE 0 END AS DEFERRABLE\n"
                  "      ,0                    AS match_option\n"
                  "      ,CASE con.deferred    WHEN 'IMMEDIATE'      THEN 0\n"
                  "                            ELSE 1 END AS initially_deferred\n"
                  "      ,CASE con.status      WHEN 'ENABLED' THEN 1\n"
                  "                            ELSE 0 END AS enabled\n"
                  "  FROM all_constraints  con\n"
                  "      ,all_cons_columns col\n"
                  "      ,all_constraints  pri\n"
                  "      ,all_cons_columns pky\n"
                  " WHERE con.owner           = col.owner\n"
                  "   AND con.constraint_name = col.constraint_name\n"
                  "   AND con.table_name      = col.table_name\n"
                  "   AND pri.owner           = con.r_owner\n"
                  "   AND pri.constraint_name = con.r_constraint_name\n"
                  "   AND pri.owner           = pky.owner\n"
                  "   AND pri.constraint_name = pky.constraint_name\n"
                  "   AND pri.table_name      = pky.table_name\n"
                  "   AND col.position        = pky.position\n"
                  "   AND con.constraint_type = 'R'";

  // Optionally add our filters
  if(!p_schema.IsEmpty())
  {
    if(p_referenced)
    {
      query += "   AND pri.owner           = ?\n";
    }
    else
    {
      query += "   AND con.owner           = ?\n";
    }
  }
  if(!p_tablename.IsEmpty())
  {
    if(p_referenced)
    {
      query += "   AND pri.table_name      = ?\n";
    }
    else
    {
      query += "   AND con.table_name      = ?\n";
    }

  }
  if(!p_constraint.IsEmpty())
  {
    if(p_referenced)
    {
      query += "   AND pri.constraint_name = ?\n";
    }
    else
    {
      query += "   AND con.constraint_name = ?\n";
    }
  }

  // Order upto the column number
  query += " ORDER BY 1,2,3,4,5,6,7,8,9";

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
    table = foreign.m_fkSchemaName + "." + table;
  }
  if(!foreign.m_pkSchemaName.IsEmpty())
  {
    primary = foreign.m_pkSchemaName + "." + primary;
  }

  // The base foreign key command
  XString query = "ALTER TABLE " + table + "\n"
                  "  ADD CONSTRAINT " + foreign.m_foreignConstraint + "\n"
                  "      FOREIGN KEY (";

  // Add the foreign key columns
  bool extra = false;
  for(auto& key : p_foreigns)
  {
    if(extra) query += ",";
    query += key.m_fkColumnName;
    extra  = true;
  }

  // Add references primary table
  query += ")\n      REFERENCES " + primary + "(";

  // Add the primary key columns
  extra = false;
  for(auto& key : p_foreigns)
  {
    if(extra) query += ",";
    query += key.m_pkColumnName;
    extra  = true;
  }
  query += ")";

  // Add all relevant options
  switch(foreign.m_deferrable)
  {
    case SQL_INITIALLY_DEFERRED:  query += "\n      INITIALLY DEFERRED"; break;
    case SQL_INITIALLY_IMMEDIATE: query += "\n      DEFERRABLE";         break;
    case SQL_NOT_DEFERRABLE:      // Already the default
                                  break;
  }
  switch(foreign.m_deleteRule)
  {
    case SQL_CASCADE:  query += "\n      ON DELETE CASCADE";     break;
    case SQL_SET_NULL: query += "\n      ON DELETE SET NULL";    break;
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
    return "";
  }

  MetaForeign& original = p_original.front();
  MetaForeign& requested = p_requested.front();

  // Construct the correct tablename
  XString table(original.m_fkTableName);
  if(!original.m_fkSchemaName.IsEmpty())
  {
    table = original.m_fkSchemaName + "." + table;
  }

  // The base foreign key command
  XString query = "ALTER  TABLE " + table + "\n"
                  "MODIFY CONSTRAINT " + original.m_foreignConstraint + "\n";

  // Add all relevant options
  if(original.m_deferrable != requested.m_deferrable)
  {
    query.AppendFormat("\n      %sDEFERRABLE", requested.m_deferrable == 0 ? "NOT " : "");
  }
  if(original.m_initiallyDeferred != requested.m_initiallyDeferred)
  {
    query += "\n      INITIALLY ";
    query += requested.m_initiallyDeferred ? "DEFERRED" : "IMMEDIATE";
  }
  if (original.m_deleteRule != requested.m_deleteRule)
  {
    switch(requested.m_deleteRule)
    {
      case SQL_CASCADE:   query += "\n      ON DELETE CASCADE";     break;
      case SQL_SET_NULL:  query += "\n      ON DELETE SET NULL";    break;
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
  XString sql("ALTER TABLE " + p_schema + "." + p_tablename + "\n"
              " DROP CONSTRAINT " + p_constraintname);
  return sql;
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
  sql.Format("SELECT COUNT(*)\n"
             "  FROM all_triggers\n"
             " WHERE table_owner  = '%s'\n"
             "   AND table_name   = '%s'\n"
             "   AND trigger_name = '%s'"
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

  XString sql = "SELECT sys_context('USERENV','DB_NAME')  AS catalog_name\n"
                "      ,owner                             AS schema_name\n"
                "      ,table_name\n"
                "      ,trigger_name\n"
                "      ,triggering_event || ' ON ' || table_name AS description\n"
                "      ,0     AS position\n"
                "      ,CASE WHEN (InStr(trigger_type,    'BEFORE') > 0) THEN 1 ELSE 0 END AS trigger_before\n"
                "      ,CASE WHEN (InStr(triggering_event,'INSERT') > 0) THEN 1 ELSE 0 END AS trigger_insert\n"
                "      ,CASE WHEN (InStr(triggering_event,'UPDATE') > 0) THEN 1 ELSE 0 END AS trigger_update\n" 
                "      ,CASE WHEN (InStr(triggering_event,'DELETE') > 0) THEN 1 ELSE 0 END AS trigger_delete\n" 
                "      ,CASE WHEN (InStr(triggering_event,'SELECT') > 0) THEN 1 ELSE 0 END AS trigger_select\n"
                "      ,CASE WHEN (InStr(triggering_event,'LOGON')  > 0 OR\n"
                "                  InStr(triggering_event,'LOGOFF') > 0 ) THEN 1 ELSE 0 END AS trigger_session\n"
                "      ,0  AS trigger_transaction\n"
                "      ,0  AS trigger_rollback\n"
                "      ,referencing_names\n"
                "      ,CASE status\n"
                "            WHEN 'DISABLED' THEN 0\n"
                "                            ELSE 1\n"
                "            END AS trigger_status\n"
                "      ,trigger_body AS source\n"
                "  FROM all_triggers\n";
  if(!p_schema.IsEmpty())
  {
    sql += " WHERE table_owner = ?\n";
  }
  if(!p_tablename.IsEmpty())
  {
    sql += p_schema.IsEmpty() ? " WHERE " : "   AND ";
    sql += "table_name ";
    sql += p_tablename.Find('%') > 0 ? "LIKE" : "=";
    sql += " ?\n";
  } 
  if(!p_triggername.IsEmpty())
  {
    sql += p_schema.IsEmpty() && p_tablename.IsEmpty() ? " WHERE " : "   AND ";
    sql += "trigger_name ";
    sql += p_triggername.Find('%') > 0 ? "LIKE" : "=";
    sql += " ?\n";
  }
  sql += " ORDER BY 1,2,3,4";
  return sql;
}

XString
SQLInfoOracle::GetCATALOGTriggerCreate(MetaTrigger& p_trigger) const
{
  // Command + trigger name
  XString sql("CREATE OR REPLACE TRIGGER ");
  sql += p_trigger.m_schemaName;
  sql += ".";
  sql += p_trigger.m_triggerName;
  sql += "\n";

  // Before or after
  sql += p_trigger.m_before ? "BEFORE " : "AFTER ";

  // Trigger actions
  if(p_trigger.m_insert)
  {
    sql += "INSERT";
  }
  if(p_trigger.m_update)
  {
    if(p_trigger.m_insert)
    {
      sql += " OR ";
    }
    sql += "UPDATE";
  }
  if(p_trigger.m_delete)
  {
    if(p_trigger.m_insert || p_trigger.m_update)
    {
      sql += " OR ";
    }
    sql += "DELETE";
  }

  // Add trigger table
  sql += " ON ";
  sql += p_trigger.m_tableName;
  sql += "\n";

  // Referencing clause
  if(!p_trigger.m_referencing.IsEmpty())
  {
    sql += p_trigger.m_referencing;
    sql += "\nFOR EACH ROW\n";
  }

  // Add trigger body
  sql += p_trigger.m_source;

  return sql;
}

XString
SQLInfoOracle::GetCATALOGTriggerDrop(XString p_schema, XString p_tablename, XString p_triggername) const
{
  return "";
}

//////////////////////////////////////////////////////////////////////////
// ALL SEQUENCE FUNCTIONS

XString
SQLInfoOracle::GetCATALOGSequenceExists(XString p_schema, XString p_sequence) const
{
  p_schema.MakeUpper();
  p_sequence.MakeUpper();

  XString sql = "SELECT COUNT(*)\n"
                "  FROM all_sequences\n"
                " WHERE sequence_owner = '" + p_schema + "'\n"
                "   AND sequence_name  = '" + p_sequence + "'";
  return sql;
}

XString
SQLInfoOracle::GetCATALOGSequenceList(XString& p_schema,XString& p_pattern) const
{
  p_schema.MakeUpper();
  p_pattern.MakeUpper();
  if(!p_pattern.IsEmpty() && p_pattern != "%")
  {
    p_pattern = "%" + p_pattern + "%";
  }
  XString sql = "SELECT ''              AS catalog_name\n"
                "      ,sequence_owner  AS schema_name\n"
                "      ,sequence_name\n"
                "      ,last_number     AS current_value\n"
                "      ,min_value       AS minimal_value\n"
                "      ,increment_by    AS seq_increment\n"
                "      ,cache_size                 AS cache\n"
                "      ,decode(cycle_flag,'N',0,1) AS cycle\n"
                "      ,decode(order_flag,'N',0,1) AS ordering\n"
                "  FROM all_sequences\n";
  if (!p_schema.IsEmpty())
  {
    sql += " WHERE sequence_owner = ?\n";
  }
  if (!p_pattern.IsEmpty())
  {
    sql += p_schema.IsEmpty() ? " WHERE " : "   AND ";
    sql += "sequence_name  LIKE ?\n";
  }
  sql += " ORDER BY 1,2,3";
  return sql;
}

XString
SQLInfoOracle::GetCATALOGSequenceAttributes(XString& p_schema,XString& p_sequence) const
{
  p_schema.MakeUpper();
  p_sequence.MakeUpper();

  XString sql = "SELECT ''              AS catalog_name\n"
                "      ,sequence_owner  AS schema_name\n"
                "      ,sequence_name\n"
                "      ,last_number     AS current_value\n"
                "      ,min_value       AS minimaml_value\n"
                "      ,increment_by    AS seq_increment\n"
                "      ,cache_size                 AS cache\n"
                "      ,decode(cycle_flag,'N',0,1) AS cycle\n"
                "      ,decode(order_flag,'N',0,1) AS ordering\n"
                "  FROM all_sequences\n";
  if(!p_schema.IsEmpty())
  {
    sql += " WHERE sequence_owner = ?\n";
  }
  if(!p_sequence.IsEmpty())
  {
    sql += p_schema.IsEmpty() ? " WHERE " : "   AND ";
    sql += "sequence_name  = ?\n";
  }
  sql += " ORDER BY 1,2,3";
  return sql;
}

XString
SQLInfoOracle::GetCATALOGSequenceCreate(MetaSequence& p_sequence) const
{
  XString sql("CREATE SEQUENCE ");

  if (!p_sequence.m_schemaName.IsEmpty())
  {
    sql += p_sequence.m_schemaName + ".";
  }
  sql += p_sequence.m_sequenceName;
  sql.AppendFormat("\n START WITH %-12.0f", p_sequence.m_currentValue);
  sql.AppendFormat("\n INCREMENT BY %d", p_sequence.m_increment);

  sql += p_sequence.m_cycle ? "\n CYCLE" : "\n NOCYCLE";
  sql += p_sequence.m_order ? "\n ORDER" : "\n NOORDER";
  if (p_sequence.m_cache > 0)
  {
    sql.AppendFormat("\n CACHE %d",p_sequence.m_cache);
  }
  else
  {
    sql += "\n NOCACHE";
  }
  return sql;
}

XString
SQLInfoOracle::GetCATALOGSequenceDrop(XString p_schema, XString p_sequence) const
{
  XString sql("DROP SEQUENCE " + p_schema + "." + p_sequence);
  return  sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL VIEW FUNCTIONS

XString 
SQLInfoOracle::GetCATALOGViewExists(XString& p_schema,XString& p_viewname) const
{
  p_schema.MakeUpper();
  p_viewname.MakeUpper();
  XString sql("SELECT COUNT(*)\n"
              "  FROM all_views\n"
              " WHERE owner     = ?\n"
              "   AND view_name = ?");
  return sql;
}

XString 
SQLInfoOracle::GetCATALOGViewList(XString& /*p_schema*/,XString& /*p_pattern*/) const
{
  return "";
}

XString 
SQLInfoOracle::GetCATALOGViewAttributes(XString& p_schema,XString& p_viewname) const
{
  p_schema.MakeUpper();
  p_viewname.MakeUpper();

  // VIEWS
  XString sql = "SELECT sys_context('USERENV','DB_NAME') AS table_catalog\n"
                "      ,viw.owner         AS table_schema\n"
                "      ,viw.view_name     AS table_name\n"
                "      ,'VIEW'            AS object_type\n"
                "      ,com.comments      AS remarks\n"
                "      ,''                AS tablespace_name\n"
                "      ,0                 AS TEMPORARY\n"
                "  FROM all_views viw LEFT OUTER JOIN all_tab_comments com\n"
                "                     ON (viw.owner     = com.owner\n"
                "                    AND  viw.view_name = com.table_name)\n"
                " WHERE com.table_type = 'VIEW'\n";
  if(!p_schema.IsEmpty())
  {
    if(p_schema.Find('%') >= 0)
    {
      sql += "  AND viw.owner   LIKE ?\n";
    }
    else
    {
      sql += "  AND viw.owner   = ?\n";
    }
  }

  if(!p_viewname.IsEmpty())
  {
    if(p_viewname.Find('%') >= 0)
    {
      sql += "   AND viw.view_name   LIKE ?\n";
    }
    else
    {
      sql += "   AND viw.view_name   = ?\n";
    }
  }

  sql += " ORDER BY 1,2,3";
  return sql;
}

XString
SQLInfoOracle::GetCATALOGViewText(XString& p_schema,XString& p_viewname) const
{
  p_schema.MakeUpper();
  p_viewname.MakeUpper();

  // BEWARE works from Oracle 19 onwards
  XString sql = "SELECT text\n"
                "  FROM all_views\n"
                " WHERE owner = '" + p_schema + "'\n"
                "   AND view_name = '" + p_viewname + "'";
  return sql;
}

XString
SQLInfoOracle::GetCATALOGViewCreate(XString p_schema,XString p_viewname,XString p_contents) const
{
  return "CREATE OR REPLACE VIEW " + p_schema + "." + p_viewname + " AS\n" + p_contents;
}

XString 
SQLInfoOracle::GetCATALOGViewRename(XString p_schema,XString p_viewname,XString p_newname)    const
{
  return "";
}

XString 
SQLInfoOracle::GetCATALOGViewDrop(XString p_schema,XString p_viewname,XString& p_precursor) const
{
  p_precursor.Empty();
  return "DROP VIEW " + p_schema + "." + p_viewname;
}

// All Privilege functions
XString
SQLInfoOracle::GetCATALOGTablePrivileges(XString& p_schema,XString& p_tablename) const
{
  p_schema.MakeUpper();
  p_tablename.MakeUpper();

  XString sql = "SELECT sys_context('USERENV','DB_NAME') AS table_catalog\n"
                "      ,pri.table_schema\n"
                "      ,pri.table_name\n"
                "      ,pri.grantor\n"
                "      ,pri.grantee\n"
                "      ,pri.privilege\n"
                "      ,pri.grantable\n"
                "  FROM all_tab_privs pri\n";

  // Add the filter
  if(!p_schema.IsEmpty())
  {
    sql += " WHERE table_schema = '" + p_schema + "'\n";
  }
  if(!p_tablename.IsEmpty())
  {
    sql += p_schema.IsEmpty() ? " WHERE " : "   AND ";
    sql += "table_name = '" + p_tablename + "'\n";
  }

  // Add owner privileges in the form of a union with the mapping
  if(!p_schema.IsEmpty() && !p_tablename.IsEmpty())
  {
    sql.AppendFormat("UNION\n"
                     "SELECT sys_context('USERENV','DB_NAME')\n"
                     "      ,'%s'\n"
                     "      ,'%s'\n"
                     "      ,'_SYSTEM'\n"
                     "      ,'%s'\n"
                     "      ,name\n"
                     "      ,'YES'\n"
                     "  FROM table_privilege_map\n"
                    ,p_schema   .GetString()
                    ,p_tablename.GetString()
                    ,p_schema   .GetString());
  }
  // Special order
  sql += " ORDER BY 1,2,3,5,6,4";
  return sql;
}

XString 
SQLInfoOracle::GetCATALOGColumnPrivileges(XString& p_schema,XString& p_tablename,XString& p_columnname) const
{
  p_schema.MakeUpper();
  p_tablename.MakeUpper();
  p_columnname.MakeUpper();

  XString sql = "SELECT sys_context('USERENV','DB_NAME') AS table_catalog\n"
                "      ,pri.table_schema\n"
                "      ,pri.table_name\n"
                "      ,pri.column_name\n"
                "      ,pri.grantor\n"
                "      ,pri.grantee\n"
                "      ,pri.privilege\n"
                "      ,pri.grantable\n"
                "  FROM all_col_privs pri\n";

  // Add the filter
  if(!p_schema.IsEmpty())
  {
    sql += " WHERE table_schema = ?\n";
  }
  if(!p_tablename.IsEmpty())
  {
    sql += p_schema.IsEmpty() ? " WHERE " : "   AND ";
    sql += "table_name = ?\n";
  }
  if(!p_columnname.IsEmpty())
  {
    sql += (p_schema.IsEmpty() && p_tablename.IsEmpty()) ? " WHERE " : "   AND ";
    sql += "column_name = ?\n";
  }
  sql += " ORDER BY 1,2,3,4,6,5,7";
  return sql;
}

XString 
SQLInfoOracle::GetCatalogGrantPrivilege(XString p_schema,XString p_objectname,XString p_privilege,XString p_grantee,bool p_grantable)
{
  XString sql;
  sql.Format("GRANT %s ON %s.%s TO %s",p_privilege.GetString(),p_schema.GetString(),p_objectname.GetString(),p_grantee.GetString());
  if(p_grantable)
  {
    sql += " WITH GRANT OPTION";
  }
  return sql;
}

XString 
SQLInfoOracle::GetCatalogRevokePrivilege(XString p_schema,XString p_objectname,XString p_privilege,XString p_grantee)
{
  XString sql;
  sql.Format("REVOKE %s ON %s.%s FROM %s",p_privilege.GetString(),p_schema.GetString(),p_objectname.GetString(),p_grantee.GetString());
  return sql;
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
  return "SELECT COUNT(*)\n"
         "  FROM all_procedures\n"
         " WHERE owner       = '" + p_schema + "'\n"
         "   AND object_name = '" + p_procedure + "'\n"
         "   AND object_type IN ('PROCEDURE','FUNCTION')";
}

XString
SQLInfoOracle::GetPSMProcedureList(XString& p_schema) const
{
  p_schema.MakeUpper();
  XString sql;

  sql = "SELECT sys_context('USERENV','DB_NAME') AS procedure_catalog\n"
        "      ,owner             AS procedure_schema\n"
        "      ,object_name       AS procedure_name\n"
        "  FROM all_procedures\n";
  if(!p_schema.IsEmpty())
  {
    sql += " WHERE owner = ?\n";
  }
  sql += " ORDER BY 1,2,3";
  return sql;
}

XString
SQLInfoOracle::GetPSMProcedureAttributes(XString& p_schema,XString& p_procedure) const
{
  p_schema.MakeUpper();
  p_procedure.MakeUpper();
  XString sql;
  sql = "SELECT sys_context('USERENV','DB_NAME') AS procedure_catalog\n"
        "      ,owner             AS procedure_schema\n"
        "      ,object_name       AS procedure_name\n"
        "      ,(SELECT COUNT(*)\n"
        "           FROM all_arguments par\n"
        "         WHERE par.owner       = pro.owner\n"
        "           AND par.object_name = pro.object_name\n"
        "           AND par.object_id   = pro.object_id\n"
        "           AND par.in_out IN ('IN','IN/OUT')) as input_params\n"
        "      ,(SELECT COUNT(*)\n"
        "          FROM all_arguments par\n"
        "         WHERE par.owner       = pro.owner\n"
        "           AND par.object_name = pro.object_name\n"
        "           AND par.object_id   = pro.object_id\n"
        "           AND par.in_out IN ('OUT','IN/OUT')) as output_params\n"
        "      ,0   AS result_sets\n"
        "      ,''  AS remarks\n"
        "      ,CASE object_type \n"
        "            WHEN 'PROCEDURE' THEN 1\n"
        "            WHEN 'FUNCTION'  THEN 2\n"
        "                             ELSE 3\n"
        "       END AS procedure_type\n"
        "      ,'<@>' as source\n"
        "  FROM all_procedures pro\n"
        " WHERE object_type IN ('PROCEDURE','FUNCTION')\n";
  if(!p_schema.IsEmpty())
  {
    sql += "   AND owner = ?\n";
  }
  if(!p_procedure.IsEmpty())
  {
    sql += "   AND object_name ";
    sql += p_procedure.Find('%') >= 0 ? "LIKE" : "=";
    sql += " ?\n";
  }
  sql += "ORDER BY 1,2,3";
  return sql;
}

XString
SQLInfoOracle::GetPSMProcedureSourcecode(XString p_schema, XString p_procedure) const
{
  p_schema.MakeUpper();
  p_procedure.MakeUpper();
  XString sql;

  sql = "SELECT 0 as object_id\n"
        "      ,0 as object_line\n"
        "      ,text\n"
        "  FROM all_source\n";
  if(!p_schema.IsEmpty())
  {
    sql += " WHERE owner = '" + p_schema + "'\n";
  }
  if(!p_procedure.IsEmpty())
  {
    sql += p_schema.IsEmpty() ? " WHERE " : "   AND ";
    sql += " name  = '" + p_procedure + "'\n";
  }
  sql += " ORDER BY owner,name,line";
  return sql;
}

XString
SQLInfoOracle::GetPSMProcedureCreate(MetaProcedure& /*p_procedure*/) const
{
  return "";
}

XString
SQLInfoOracle::GetPSMProcedureDrop(XString p_schema, XString p_procedure) const
{
  return "";
}

XString
SQLInfoOracle::GetPSMProcedureErrors(XString p_schema,XString p_procedure) const
{
  XString query;
  XString errorText;
  p_schema.MakeUpper();
  p_procedure.MakeUpper();
  query = "SELECT line\n"
          "      ,position\n"
          "      ,text\n"
          "  FROM all_errors\n"
          " WHERE name = '" + p_procedure +"'";

  SQLQuery qry1(m_database);
  SQLQuery qry2(m_database);
  qry1.DoSQLStatement(query);

  while (qry1.GetRecord())
  {
    XString s = qry1.GetColumn(3)->GetAsChar();
    if(s.Find("Statement ignored") < 0) 
    {
      s.Format("Error in line %d, column %d: %s\n",qry1.GetColumn(1)->GetAsSLong()
	                                              ,qry1.GetColumn(2)->GetAsSLong()
						                          ,qry1.GetColumn(3)->GetAsChar());
      errorText += s;
      query.Format( "SELECT text\n"
                    "  FROM all_source\n"
                    " WHERE type = 'FUNCTION'\n"
                    "   AND name = '%s'\n"
                    "   AND line = %d"
                   ,p_procedure.GetString()
                   ,qry1.GetColumn(1)->GetAsSLong());
      qry2.DoSQLStatement(query);
      while(qry2.GetRecord())
      {
        s.Format("Line %d: %s\n"
                 ,qry1.GetColumn(1)->GetAsSLong()
                 ,qry2.GetColumn(1)->GetAsChar());
        errorText += s;
      }
    }
  }
  return errorText;
}

// And it's parameters
XString
SQLInfoOracle::GetPSMProcedureParameters(XString& /*p_schema*/,XString& /*p_procedure*/) const
{
  return "";
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
    line = "DECLARE\n";
  }
  line += p_variable + " ";

  if(p_datatype)
  {
    // Getting type info and name
    TypeInfo* info = GetTypeInfo(p_datatype);
    line += info->m_type_name;

    if(p_precision > 0)
    {
      line.AppendFormat("(%d",p_precision);
      if(p_scale > 0)
      {
        line.AppendFormat("%d",p_scale);
      }
      line += ")";
    }

    if(!p_default.IsEmpty())
    {
      line += " DEFAULT " + p_default;
    }
  }
  else if(!p_domain.IsEmpty())
  {
    line += p_domain;
  }
  else if(!p_asColumn)
  {
    line += p_asColumn + "%TYPE";
  }
  line += ";\n";
  return line;
}

XString
SQLInfoOracle::GetPSMAssignment(XString p_variable,XString p_statement /*=""*/) const
{
  XString line(p_variable);
  line += " := ";
  if(!p_statement.IsEmpty())
  {
    line += p_statement;
    line += ";";
  }
  return line;
}

XString
SQLInfoOracle::GetPSMIF(XString p_condition) const
{
  XString line("IF (");
  line += p_condition;
  line += ") THEN\n";
  return line;
}

XString
SQLInfoOracle::GetPSMIFElse() const
{
  return "ELSE\n";
}

XString
SQLInfoOracle::GetPSMIFEnd() const
{
  return "END IF;\n";
}

XString
SQLInfoOracle::GetPSMWhile(XString p_condition) const
{
  return "WHILE (" + p_condition + ") LOOP\n";
}

XString
SQLInfoOracle::GetPSMWhileEnd() const
{
  return "END LOOP;\n";
}

XString
SQLInfoOracle::GetPSMLOOP() const
{
  return "LOOP\n";
}

XString
SQLInfoOracle::GetPSMLOOPEnd() const
{
  return "END LOOP;\n";
}

XString
SQLInfoOracle::GetPSMBREAK() const
{
  return "EXIT;\n";
}

XString
SQLInfoOracle::GetPSMRETURN(XString p_statement /*= ""*/) const
{
  XString line("RETURN");
  if(!p_statement.IsEmpty())
  {
    line += " (" + p_statement + ")";
  }
  line += ";\n";
  return line;
}

XString
SQLInfoOracle::GetPSMExecute(XString p_procedure,MParameterMap& p_parameters) const
{
  XString line;
  line.Format("EXECUTE %s(",p_procedure.GetString());
  bool doMore = false;

  for(auto& param : p_parameters)
  {
    if(doMore) line += ",";
    doMore = true;

    line += param.m_parameter;
  }
  line += ");\n";
  return line;
}

// The CURSOR
XString
SQLInfoOracle::GetPSMCursorDeclaration(XString p_cursorname,XString p_select) const
{
  return "CURSOR " + p_cursorname + " IS " + p_select + ";";
}

XString
SQLInfoOracle::GetPSMCursorFetch(XString p_cursorname,std::vector<XString>& /*p_columnnames*/,std::vector<XString>& p_variablenames) const
{
  bool moreThenOne = false;
  XString query = "OPEN  " + p_cursorname + ";\n"
                  "FETCH " + p_cursorname + " INTO ";

  for(auto& var : p_variablenames)
  {
    if(moreThenOne) query += ",";
    moreThenOne = true;
    query += var;
  }
  query += ";";
  return query;
}

//////////////////////////////////////////////////////////////////////////
// PSM Exceptions

XString
SQLInfoOracle::GetPSMExceptionCatchNoData() const
{
  return "EXCEPTION WHEN NO_DATA_FOUND THEN\n";
  // followed by STATEMENTBLOCK and "END;"
}

XString
SQLInfoOracle::GetPSMExceptionCatch(XString p_sqlState) const
{
  return "EXCEPTION WHEN " + p_sqlState + " THEN\n";
  // followed by STATEMENTBLOCK and "END;"
}

XString
SQLInfoOracle::GetPSMExceptionRaise(XString p_sqlState) const
{
  return "RAISE " + p_sqlState;
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
  XString sql = "SELECT audsid\n"
                "      ,osuser\n"
                "      ,terminal\n"
                "      ,prev_exec_start\n" // moment
                "      ,machine\n"         // remote 
                "      ,program\n"         // process name
                "      ,process\n"         // process ID
                "  FROM v$session\n"
                " WHERE type   = 'USER'\n"
                "   AND audsid = sys_context('userenv','sessionid')";
  return sql;
}

XString
SQLInfoOracle::GetSESSIONExists(XString p_sessionID) const
{
  return "SELECT COUNT(*)\n"
         "  FROM v$session\n"
         " WHERE audsid = " + p_sessionID;
}

XString
SQLInfoOracle::GetSESSIONList() const
{
  return GetSESSIONAttributes("");
}

XString
SQLInfoOracle::GetSESSIONAttributes(XString p_sessionID) const
{
  XString sql = "SELECT audsid\n"
                "      ,osuser\n"
                "      ,terminal\n"
                "      ,prev_exec_start\n" // moment
                "      ,machine\n"         // remote 
                "      ,program\n"         // process name
                "      ,process\n"         // process ID
                "  FROM v$session\n"
                " WHERE type = 'USER'\n";
  if(!p_sessionID.IsEmpty())
  {
    sql += "\n   AND audsid = " + p_sessionID;
  }
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// Transactions

XString
SQLInfoOracle::GetSESSIONConstraintsDeferred() const
{
  // Set to DEFERRED for Oracle
  return "ALTER SESSION SET CONSTRAINTS = DEFERRED";
}

XString
SQLInfoOracle::GetSESSIONConstraintsImmediate() const
{
  // Set to IMMEDIATE for Oracle
  return "ALTER SESSION SET CONSTRAINTS = IMMEDIATE";
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
  XString sql = "BEGIN DECLARE x NUMBER; BEGIN ? := ";
  if(!p_schema.IsEmpty())
  {
    sql += p_schema;
    sql += ".";
  }
  sql += p_procedure;
  sql += "(";

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
        sql += ",";
      }
      sql += name;
      sql += " => ? ";
    }
  }
  sql += "); END; END;";

  // Add parameter 0 as result parameter
  p_query->SetParameter(0,0);

  // Now find the result
  p_query->DoSQLStatement(sql);
  return p_query->GetParameter(0);
}

// End of namespace
}
