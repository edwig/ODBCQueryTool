////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoPostgreSQL.cpp
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
#include "SQLInfoPostgreSQL.h"
#include "SQLQuery.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{

// Constructor. 
SQLInfoPostgreSQL::SQLInfoPostgreSQL(SQLDatabase* p_database)
                  :SQLInfoDB(p_database)
{
}

// Destructor.
SQLInfoPostgreSQL::~SQLInfoPostgreSQL()
{
}

// RDBMS Uses INDENTITY or SEQUENCE interface
void
SQLInfoPostgreSQL::SetUseSequences(bool /*p_sequences*/)
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
SQLInfoPostgreSQL::GetRDBMSDatabaseType() const
{
  return RDBMS_POSTGRESQL;
}

// The name of the database vendor
XString
SQLInfoPostgreSQL::GetRDBMSVendorName() const
{
  // The name of the database vendor
  return _T("PostgreSQL");
}

// Get the physical database name
XString
SQLInfoPostgreSQL::GetRDBMSPhysicalDatabaseName() const
{
  SQLQuery sql(m_database);
  XString query = _T("SELECT current_database()");
  sql.DoSQLStatement(query);
  if(sql.GetRecord())
  {
    return sql.GetColumn(1)->GetAsChar();
  }
  return _T("");
}

// System catalog is stored in uppercase in the database?
bool
SQLInfoPostgreSQL::GetRDBMSIsCatalogUpper() const
{
  return false;
}

// System catalog supports full ISO schemas (same tables per schema)
bool
SQLInfoPostgreSQL::GetRDBMSUnderstandsSchemas() const
{
  return true;
}

// Supports database/ODBCdriver comments in SQL
bool
SQLInfoPostgreSQL::GetRDBMSSupportsComments() const
{
  return true;
}

// Database can defer constraints until the end of a transaction
bool
SQLInfoPostgreSQL::GetRDBMSSupportsDeferredConstraints() const
{
  // SET CONSTRAINTS DEFERRED is supported
  return true;
}

// Database has ORDER BY with an expression, e.g. ORDER BY UPPER(columnname)
bool
SQLInfoPostgreSQL::GetRDBMSSupportsOrderByExpression() const
{
  return true;
}

// Supports the ODBC escape sequence {[?=] CALL procedure (?,?,?)}
bool
SQLInfoPostgreSQL::GetRDBMSSupportsODBCCallEscapes() const
{
  // Does NOT support the [?=] return parameter
  // So our own plumbing is needed
  return false;
}

// Supports the ODBC call procedure with named parameters
bool
SQLInfoPostgreSQL::GetRDBMSSupportsODBCCallNamedParameters() const
{
  return false;
}

// If the database does not support the datatype TIME, it can be implemented as a DECIMAL
bool
SQLInfoPostgreSQL::GetRDBMSSupportsDatatypeTime() const
{
  // Time can be implemented as TIME
  return true;
}

// If the database does not support the datatype INTERVAL, it can be implemented as a DECIMAL
bool
SQLInfoPostgreSQL::GetRDBMSSupportsDatatypeInterval() const
{
  // Interval supported
  return true;
}

// Supports functions at the place of table columns in create/alter index statement
bool
SQLInfoPostgreSQL::GetRDBMSSupportsFunctionalIndexes() const
{
  return false;
}

// Gets the maximum length of an SQL statement
unsigned long
SQLInfoPostgreSQL::GetRDBMSMaxStatementLength() const
{
  // No Limit
  return 0;
}

// Database must commit DDL commands in a transaction
bool
SQLInfoPostgreSQL::GetRDBMSMustCommitDDL() const
{
  return true;
}

// Correct maximum precision,scale for a NUMERIC datatype
void
SQLInfoPostgreSQL::GetRDBMSNumericPrecisionScale(SQLULEN& /*p_precision*/, SQLSMALLINT& /*p_scale*/) const
{
  // NO-OP
}

// Maximum for a VARCHAR to be handled without AT-EXEC data. Assume NVARCHAR is half that size!
int
SQLInfoPostgreSQL::GetRDBMSMaxVarchar() const
{
  // See: https://www.postgresql.org/docs/current/datatype-character.html
  return 65535;
}

// KEYWORDS

// Keyword for the current date and time
XString
SQLInfoPostgreSQL::GetKEYWORDCurrentTimestamp() const
{
  return _T("current_timestamp(0)");
}

// String for the current date
XString
SQLInfoPostgreSQL::GetKEYWORDCurrentDate() const
{
  return _T("current_date");
}

// Get the concatenation operator
XString
SQLInfoPostgreSQL::GetKEYWORDConcatanationOperator() const
{
  return _T("||");
}

// Get quote character for strings
XString
SQLInfoPostgreSQL::GetKEYWORDQuoteCharacter() const
{
  return _T("\'");
}

// Get quote character around reserved words as an identifier
XString
SQLInfoPostgreSQL::GetKEYWORDReservedWordQuote() const
{
  return _T("\'");
}

// Get default NULL for parameter list input
XString
SQLInfoPostgreSQL::GetKEYWORDParameterDefaultNULL() const
{
  // Standard, no definition defines the NULL state
  return _T("");
}

// Parameter is for INPUT and OUTPUT in parameter list
XString
SQLInfoPostgreSQL::GetKEYWORDParameterINOUT() const
{
  return _T("");
}

// Parameter is for OUTPUT only in parameter list
XString
SQLInfoPostgreSQL::GetKEYWORDParameterOUT() const
{
  return _T("");
}

// Get datatype of the IDENTITY primary key in a Network database
XString
SQLInfoPostgreSQL::GetKEYWORDNetworkPrimaryKeyType() const
{
  // Use SEQUENCE to fill!
  return _T("INTEGER");
}

// Get datatype for timestamp (year to second)
XString
SQLInfoPostgreSQL::GetKEYWORDTypeTimestamp() const
{
  return _T("TIMESTAMP");
}

// Prefix for a parameter in a stored procedure
XString
SQLInfoPostgreSQL::GetKEYWORDParameterPrefix() const
{
  return _T("");
}

// Get select part to add new record identity to a table
// Can be special column like 'OID' or a sequence select
XString
SQLInfoPostgreSQL::GetKEYWORDIdentityString(XString& p_tablename,XString p_postfix /*= "_seq"*/) const
{
  return _T("nextval('") + p_tablename + p_postfix + _T("')");
}

// Gets the UPPER function
XString
SQLInfoPostgreSQL::GetKEYWORDUpper(XString& p_expression) const
{
  return _T("UPPER(") + p_expression + _T(")");
}

// Gets the construction for 1 minute ago
XString
SQLInfoPostgreSQL::GetKEYWORDInterval1MinuteAgo() const
{
  // Not supported by PostgreSQL
  return _T("ERROR");
}

// Gets the Not-NULL-Value statement of the database
XString
SQLInfoPostgreSQL::GetKEYWORDStatementNVL(XString& p_test,XString& p_isnull) const
{
  return _T("{fn IFNULL(") + p_test + _T(",") + p_isnull + _T(")}");
}

// Gets the RDBMS definition of the datatype
XString
SQLInfoPostgreSQL::GetKEYWORDDataType(MetaColumn* p_column)
{
  return p_column->m_typename;
}

// Gets the USER (current-user) keyword function
XString
SQLInfoPostgreSQL::GetKEYWORDCurrentUser() const
{
  return _T("CURRENT_USER");
}

// Connects to a default schema in the database/instance
XString
SQLInfoPostgreSQL::GetSQLDefaultSchema(XString /*p_user*/,XString p_schema) const
{
  return _T("SET SEARCH_PATH TO ") + p_schema;
}

// Gets the construction for inline generating a key within an INSERT statement
XString
SQLInfoPostgreSQL::GetSQLNewSerial(XString p_table, XString p_sequence) const
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
SQLInfoPostgreSQL::GetSQLGenerateSerial(XString p_table) const
{
  return _T("SELECT ") + p_table + _T("_seq.nextval FROM DUAL");
}

// Gets the construction / select for the resulting effective generated serial
XString
SQLInfoPostgreSQL::GetSQLEffectiveSerial(XString p_identity) const
{
  // Already the correct value
  return p_identity;
}

// Gets the sub transaction commands
XString
SQLInfoPostgreSQL::GetSQLStartSubTransaction(XString p_savepointName) const
{
  return XString(_T("SAVEPOINT ")) + p_savepointName;
}

XString
SQLInfoPostgreSQL::GetSQLCommitSubTransaction(XString p_savepointName) const
{
  // No commit for a sub transaction
  return XString(_T(""));
}

XString
SQLInfoPostgreSQL::GetSQLRollbackSubTransaction(XString p_savepointName) const
{
  return XString(_T("ROLLBACK TO SAVEPOINT ")) + p_savepointName;
}

// FROM-Part for a query to select only 1 (one) record / or empty!
XString
SQLInfoPostgreSQL::GetSQLFromDualClause() const
{
  // PostgreSQL knows the bare SELECT
  return _T("");
}

// Get SQL to lock  a table 
XString
SQLInfoPostgreSQL::GetSQLLockTable(XString p_schema,XString p_tablename,bool p_exclusive,int /*p_waittime*/) const
{
  XString query = _T("LOCK TABLE ") + p_schema + _T(".") + p_tablename + _T(" IN ");
  query += p_exclusive ? _T("EXCLUSIVE") : _T("SHARE");
  query += _T(" MODE");
  return query;
}

// Get query to optimize the table statistics
XString
SQLInfoPostgreSQL::GetSQLOptimizeTable(XString p_schema, XString p_tablename) const
{
  XString optim = _T("VACUUM ANALYZE ") + p_schema + _T(".") + p_tablename;
  return optim;
}

// Transform query to select top <n> rows
XString
SQLInfoPostgreSQL::GetSQLTopNRows(XString p_sql,int p_top,int p_skip /*= 0*/) const
{
  if(p_top > 0)
  {
    p_sql.AppendFormat(_T("\nSELECT LIMIT %d "),p_top);
    if(p_skip > 0)
    {
      p_sql.AppendFormat(_T(" OFFSET %d"),p_skip);
    }
  }
  return p_sql;
}

// Expand a SELECT with an 'FOR UPDATE' lock clause
XString
SQLInfoPostgreSQL::GetSelectForUpdateTableClause(unsigned /*p_lockWaitTime*/) const
{
  return "";
}

XString
SQLInfoPostgreSQL::GetSelectForUpdateTrailer(XString p_select,unsigned p_lockWaitTime) const
{
  XString sql = p_select + "\nFOR UPDATE";
  if(!p_lockWaitTime)
  {
    sql += " SKIP LOCKED";
  }
  return sql;
}

// Query to perform a keep alive ping
XString
SQLInfoPostgreSQL::GetPing() const
{
  // Getting the time does a ping
  return _T("SELECT current_timestamp");
}

//////////////////////////////////////////////////////////////////////////
//
// SQL STRINGS
//
//////////////////////////////////////////////////////////////////////////

XString
SQLInfoPostgreSQL::GetSQLString(const XString& p_string) const
{
  XString s = p_string;
  s.Replace(_T("'"),_T("''"));
  XString kwoot = GetKEYWORDQuoteCharacter();
  return  kwoot + s + kwoot;
}

XString
SQLInfoPostgreSQL::GetSQLDateString(int p_year,int p_month,int p_day) const
{
  XString dateString;
  dateString.Format(_T("TO_DATE('%04d-%02d-%02d','YYYY-MM-DD')"),p_year,p_month,p_day);
  return dateString;
}

XString
SQLInfoPostgreSQL::GetSQLTimeString(int p_hour,int p_minute,int p_second) const
{
  XString timeString;
  timeString.Format(_T("'%02d:%02d:%02d'"),p_hour,p_minute,p_second);
  return timeString;
}

XString
SQLInfoPostgreSQL::GetSQLDateTimeString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  XString retval;
  retval.Format(_T("to_timestamp('%04d-%02d-%02d %02d:%02d:%02d','YYYY-MM-DD HH24:MI:SS')::timestamp")
                ,p_year,p_month,p_day,p_hour,p_minute,p_second);
  return retval;
}

// Get date-time bound parameter string in database format
XString
SQLInfoPostgreSQL::GetSQLDateTimeBoundString() const
{
  return _T("to_timestamp(?,'YYYY-MM-DD HH24:MI:SS')::timestamp");
}

// Stripped data for the parameter binding
XString
SQLInfoPostgreSQL::GetSQLDateTimeStrippedString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  XString retval;
  retval.Format(_T("%04d-%02d-%02d %02d:%02d:%02d")
                ,p_year,p_month,p_day,p_hour,p_minute,p_second);
  return retval;
}

// Makes an catalog identifier string (possibly quoted on both sides)
XString
SQLInfoPostgreSQL::GetSQLDDLIdentifier(XString p_identifier) const
{
  return p_identifier;
}

// Get the name of a temp table (local temporary or global temporary)
XString
SQLInfoPostgreSQL::GetTempTablename(XString /*p_schema*/,XString p_tablename,bool /*p_local*/) const
{
  return p_tablename;
}

// Changes to parameters before binding to an ODBC HSTMT handle
void
SQLInfoPostgreSQL::DoBindParameterFixup(SQLSMALLINT& /*p_sqlDatatype*/,SQLULEN& /*p_columnSize*/,SQLSMALLINT& /*p_scale*/,SQLLEN& /*p_bufferSize*/,SQLLEN* /*p_indicator*/) const
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
//   - Alter (where possible)
//   - Drop
//
//////////////////////////////////////////////////////////////////////////

// Meta info about meta types
// Standard ODBC functions are good enough
XString
SQLInfoPostgreSQL::GetCATALOGMetaTypes(int p_type) const
{
  UNREFERENCED_PARAMETER(p_type);
  return _T("");
}

XString
SQLInfoPostgreSQL::GetCATALOGDefaultCharset() const
{
  return _T("iso-8859-1");
}

XString
SQLInfoPostgreSQL::GetCATALOGDefaultCharsetNCV() const
{
  // NO Support for NVARCHAR or UTF-8/UTF-16
  return _T("-");
}

XString
SQLInfoPostgreSQL::GetCATALOGDefaultCollation() const
{
  return _T("ucs_basic");
}

// Get SQL to check if a table already exists in the database
XString
SQLInfoPostgreSQL::GetCATALOGTableExists(XString& p_schema,XString& p_tablename) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  XString query = _T("SELECT count(*)\n")
                  _T("  FROM pg_class cl\n")
                  _T("      ,pg_namespace ns\n")
                  _T(" WHERE cl.relnamespace = ns.oid\n")
                  _T("   AND ns.nspname = ?\n")
                  _T("   AND cl.relname = ?");
  return query;
}

XString
SQLInfoPostgreSQL::GetCATALOGTablesList(XString& p_schema,XString& p_pattern) const
{
  return GetCATALOGTableAttributes(p_schema,p_pattern);
}

XString
SQLInfoPostgreSQL::GetCATALOGTableAttributes(XString& p_schema,XString& p_tablename) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  XString query = _T("SELECT current_catalog as table_catalog\n")
                  _T("      ,sch.nspname     as table_schema\n")
                  _T("      ,tab.relname     as table_name\n")
                  _T("      ,'TABLE'         as object_type\n")
                  _T("      ,''              as remarks\n")
                  _T("      ,sch.nspname || '.' || tab.relname as fullname\n")
                  _T("      ,tsp.spcname     as tablespace\n")
                  _T("      ,CASE tab.relpersistence\n")
                  _T("            WHEN 'p' THEN 0\n")
                  _T("                     ELSE 1\n")
                  _T("       END as temporary\n")
                  _T("  FROM pg_catalog.pg_class tab\n")
                  _T("       left       join pg_catalog.pg_namespace  sch on tab.relnamespace  = sch.oid\n")
                  _T("       left outer join pg_catalog.pg_tablespace tsp ON tab.reltablespace = tsp.oid\n")
                  _T(" WHERE tab.relkind = 'r'\n")
                  _T("   AND substring(sch.nspname,1,3) <> 'pg_'\n")
                  _T("   AND sch.nspname <> 'information_schema'\n");
  if(!p_schema.IsEmpty())
  {
    query += _T("    AND sch.name = ?\n");
  }
  if(!p_tablename.IsEmpty())
  {
    query += _T("   AND tab.relname ");
    query += p_tablename.Find('%') >= 0 ? _T("LIKE") : _T("=");
    query += _T(" ?\n");
  }
  query += _T(" ORDER BY 1,2,3");
                  
  return query;
}

XString
SQLInfoPostgreSQL::GetCATALOGTableSynonyms(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  // PostgreSQL cannot do this
  return XString();
}

XString
SQLInfoPostgreSQL::GetCATALOGTableCatalog(XString& p_schema,XString& p_tablename) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  XString query = _T("SELECT current_catalog as table_catalog\n")
                  _T("      ,sch.nspname     as table_schema\n")
                  _T("      ,tab.relname     as table_name\n")
                  _T("      ,'TABLE'         as object_type\n")
                  _T("      ,''              as remarks\n")
                  _T("      ,sch.nspname || '.' || tab.relname as fullname\n")
                  _T("      ,tsp.spcname     as tablespace\n")
                  _T("      ,CASE tab.relpersistence\n")
                  _T("            WHEN 'p' THEN 0\n")
                  _T("                     ELSE 1\n")
                  _T("       END as temporary\n")
                  _T("  FROM pg_catalog.pg_class tab\n")
                  _T("       left       join pg_catalog.pg_namespace  sch on tab.relnamespace  = sch.oid\n")
                  _T("       left outer join pg_catalog.pg_tablespace tsp ON tab.reltablespace = tsp.oid\n")
                  _T(" WHERE tab.relkind IN ('t','v')\n")
                  _T("   AND (substring(sch.nspname,1,3) = 'pg_'\n")
                  _T("    OR sch.nspname = 'information_schema')\n")
                  _T("   AND sch.nspname <> 'pg_toast'\n");
  if(!p_schema.IsEmpty())
  {
    query += _T("    AND sch.name = ?\n");
  }
  if(!p_tablename.IsEmpty())
  {
    query += _T("   AND tab.relname ");
    query += p_tablename.Find('%') >= 0 ? _T("LIKE") : _T("=");
    query += _T(" ?\n");
  }
  query += _T(" ORDER BY 1,2,3");
                  
  return query;
}

XString
SQLInfoPostgreSQL::GetCATALOGTableCreate(MetaTable& p_table,MetaColumn& /*p_column*/) const
{
  XString sql = _T("CREATE ");
  if (p_table.m_temporary)
  {
    sql += _T("TEMPORARY ");
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
SQLInfoPostgreSQL::GetCATALOGTableCreatePostfix(MetaTable& /*p_table*/,MetaColumn& /*p_column*/) const
{
  return _T("");
}

XString
SQLInfoPostgreSQL::GetCATALOGTableRename(XString p_schema,XString p_tablename,XString p_newname) const
{
  // Beware: No 'TABLE' in the statement
  XString sql(_T("RENAME ") + p_schema + _T(".") + p_tablename + _T(" TO ") + p_newname);
  return sql;
}

XString
SQLInfoPostgreSQL::GetCATALOGTableDrop(XString p_schema,XString p_tablename,bool p_ifExist /*= false*/,bool p_restrict /*= false*/,bool p_cascade /*= false*/) const
{
  XString sql(_T("DROP TABLE "));
  if (p_ifExist)
  {
    sql += _T("IF EXISTS ");
  }
  if(!p_schema.IsEmpty())
  {
    sql += p_schema + _T(".");
  }
  sql += p_tablename;
  if (p_restrict)
  {
    sql += _T(" RESTRICT");
  }
  else if (p_cascade)
  {
    sql += _T(" CASCADE");
  }
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL TEMPORARY TABLE FUNCTIONS

XString 
SQLInfoPostgreSQL::GetCATALOGTemptableCreate(XString p_schema,XString p_tablename,XString p_select) const
{
  return _T("CREATE GLOBAL TEMPORARY TABLE ") + p_schema + _T(".") + p_tablename + _T(" WITHOUT OIDS\nAS ") + p_select;
}

XString 
SQLInfoPostgreSQL::GetCATALOGTemptableIntoTemp(XString p_schema,XString p_tablename,XString p_select) const
{
  return _T("INSERT INTO ") + p_schema + _T(".") + p_tablename + _T("\n") + p_select;
}

XString 
SQLInfoPostgreSQL::GetCATALOGTemptableDrop(XString p_schema,XString p_tablename) const
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
SQLInfoPostgreSQL::GetCATALOGColumnExists(XString p_schema,XString p_tablename,XString p_columnname) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  p_columnname.MakeLower();
  XString query = _T("SELECT count(*)\n")
                 _T("  FROM pg_class      tab\n")
                 _T("      ,pg_namespaces sch\n")
                 _T("      ,pg_attribute  att\n")
                 _T(" WHERE tab.relname = '") + p_tablename  + _T("'\n")
                 _T("   AND sch.name    = '") + p_schema     + _T("'\n")
                 _T("   AND att.attname = '") + p_columnname + _T("'\n")
                 _T("   AND tab.oid     = att.attrelid\n")
                 _T("   AND sch.oid     = tab.relnamespace\n");
  return query;
}

XString 
SQLInfoPostgreSQL::GetCATALOGColumnList(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  return _T("");
//   p_schema.MakeLower();
//   p_tablename.MakeLower();
// 
//   XString query =  "SELECT att.attname\n"
//                    "      ,att.attnum\n"
//                    "      ,typname\n"
//                    "      ,typlen\n"
//                    "      ,attnotnull\n"
//                    "      ,(atttypmod - 4) / 65536\n"
//                    "      ,(atttypmod - 4) % 65536\n"
//                    "  FROM pg_class      tab\n"
//                    "      ,pg_namespaces sch\n"
//                    "      ,pg_attribute  att\n"
//                    "      ,pg_type       typ\n"
//                    "      ,pg_attrdef    def\n"
//                    " WHERE tab.relname  = '" + p_tablename + "'\n"
//                    "   AND sch.name     = '" + p_schema + "'\n"
//                    "   AND sch.oid      = tab.relnamespace\n"
//                    "   AND tab.relkind  = 'r'\n"
//                    "   AND tab.oid      = att.attrelid\n"
//                    "   AND att.atttypid = pg_type.oid\n"
//                    "   AND def.adrelid  = att.attrelid\n"
//                    "   AND def.adnum    = att.attnum\n"
//                    "   AND attnum > 0\n"
//                    "   AND typlen < 0\n"
//                    "   AND typname = 'numeric'\n"
//                    "UNION\n"
//                    "SELECT attname\n"
//                    "      ,attnum\n"
//                    "      ,typname\n"
//                    "      ,(atttypmod - 4) AS typlen\n"
//                    "      ,attnotnull\n"
//                    "      ,0\n"
//                    "      ,0\n"
//                    "  FROM pg_class      tab\n"
//                    "      ,pg_namespaces sch\n"
//                    "      ,pg_attribute  att\n"
//                    "      ,pg_type       typ\n"
//                    "      ,pg_attrdef    def\n"
//                    " WHERE tab.relname  = '" + p_tablename + "'\n"
//                    "   AND sch.name     = '" + p_schema + "'\n"
//                    "   AND sch.oid      = tab.relnamespace\n"
//                    "   AND tab.relkind  = 'r'\n"
//                    "   AND tab.oid      = att.attrelid\n"
//                    "   AND att.atttypid = typ.oid\n"
//                    "   AND def.adrelid  = att.attrelid\n"
//                    "   AND def.adnum    = att.attnum\n"
//                    "   AND attnum > 0\n"
//                    "   AND typlen < 0\n"
//                    "   AND typname = 'varchar'\n"
//                    "UNION\n"
//                    "SELECT attname\n"
//                    "      ,attnum\n"
//                    "      ,typname\n"
//                    "      ,typlen\n"
//                    "      ,attnotnull\n"
//                    "      ,0\n"
//                    "      ,0\n"
//                    "  FROM pg_class      tab\n"
//                    "      ,pg_namespaces sch\n"
//                    "      ,pg_attribute  att\n"
//                    "      ,pg_type       typ\n"
//                    "      ,pg_attrdef    def\n"
//                    " WHERE tab.relname  = '" + p_tablename + "'\n"
//                    "   AND sch.name     = '" + p_schema + "'\n"
//                    "   AND sch.oid      = tab.relnamespace\n"
//                    "   AND tab.relkind  = 'r'\n"
//                    "   AND tab.oid      = att.attrelid\n"
//                    "   AND att.atttypid = typ.oid\n"
//                    "   AND def.adrelid  = att.attrelid\n"
//                    "   AND def.adnum    = att.attnum\n"
//                    "   AND attnum > 0\n"
//                    "   AND typlen > 0\n"
//                    "   AND typname not in ('numeric', 'varchar')\n"
//                    " ORDER BY attnum";
//   return query;
}

XString 
SQLInfoPostgreSQL::GetCATALOGColumnAttributes(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_columnname*/) const
{
  // Standard ODBC driver suffices
  return _T("");
}

XString 
SQLInfoPostgreSQL::GetCATALOGColumnCreate(MetaColumn& p_column) const
{
  XString sql = _T("ALTER TABLE ")  + p_column.m_schema + _T(".") + p_column.m_table  + _T("\n")
                _T("  ADD COLUMN ") + p_column.m_column + _T(" ") + p_column.m_typename;
  if(p_column.m_columnSize)
  {
    sql.AppendFormat(_T("(%d"),p_column.m_columnSize);
    if(p_column.m_decimalDigits)
    {
      sql.AppendFormat(_T(",%d"),p_column.m_decimalDigits);
    }
    sql += _T(")");
  }
  if(!p_column.m_nullable)
  {
    sql += _T(" NOT NULL");
  }
  if(!p_column.m_default.IsEmpty())
  {
    sql += _T(" DEFAULT ");
    sql += p_column.m_default;
  }
  // m_position not used
  // m_length   not used
  // m_remarks  not used
  return sql;
}

XString 
SQLInfoPostgreSQL::GetCATALOGColumnAlter(MetaColumn& p_column) const
{
  XString sql(_T("ALTER TABLE  ") + p_column.m_schema + _T(".") + p_column.m_table + _T("\n")
              _T("ALTER COLUMN ") + p_column.m_column + _T(" SET DATA TYPE ") + p_column.m_typename);
  p_column.GetPrecisionAndScale(sql);

  sql += p_column.m_nullable ? _T(" DROP ") : _T(" SET ");
  sql += _T("NOT NULL");
  return sql;
}

XString 
SQLInfoPostgreSQL::GetCATALOGColumnRename(XString p_schema,XString p_tablename,XString p_columnname,XString p_newname,XString p_datatype) const
{
  XString sqlCode = _T("ALTER TABLE ") + p_schema + _T(".") + p_tablename + _T("\n")
                    _T("  ADD ") + p_newname + _T(" ") + p_datatype + _T(";\n");
  sqlCode        += _T("UPDATE ") + p_schema + _T(".") + p_tablename + _T("\n")
                    _T("   SET ") + p_newname + _T(" = ") + p_columnname + _T(";\n");
  sqlCode        += _T("ALTER TABLE ") + p_schema + _T(".") + p_tablename + _T("\n")
                    _T(" DROP COLUMN ") + p_columnname + _T(";");
  return sqlCode;
}

XString
SQLInfoPostgreSQL::GetCATALOGColumnDrop(XString p_schema,XString p_tablename,XString p_columnname) const
{
  XString sql(_T("ALTER TABLE ") + p_schema + _T(".") + p_tablename + _T("\n")
              _T(" DROP COLUMN ") + p_columnname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL INDICES FUNCTIONS

// All index functions
XString
SQLInfoPostgreSQL::GetCATALOGIndexExists(XString p_schema,XString p_tablename,XString p_indexname) const
{
  return _T("");
}

XString
SQLInfoPostgreSQL::GetCATALOGIndexList(XString& p_schema,XString& p_tablename) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();

  XString query =  _T("SELECT cli.relname\n")
                   _T("      ,att.attname\n")
                   _T("      ,att.attnum\n")
                   _T("      ,idx.indisunique\n")
                   _T("      ,0  as ascending\n")
                   _T("      ,'' as expression_source")
                   _T("  FROM pg_user sha\n")
                   _T("      ,pg_class clr\n")
                   _T("      ,pg_index idx\n")
                   _T("      ,pg_class cli\n")
                   _T("      ,pg_attribute att\n")
                   _T(" WHERE sha.usename = ?\n")
                   _T("   AND clr.relname = ?\n")
                   _T("   AND sha.usesysid = clr.relowner\n")
                   _T("   AND clr.relkind = 'r'\n")
                   _T("   AND clr.oid = idx.indrelid\n")
                   _T("   AND idx.indexrelid = cli.oid\n")
                   _T("   AND cli.relkind = 'i'\n")
                   _T("   AND cli.oid = att.attrelid\n")
                   _T(" ORDER BY cli.relname, att.attnum");
  return query;
}

XString
SQLInfoPostgreSQL::GetCATALOGIndexAttributes(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_indexname*/) const
{
  return _T("");
}

XString
SQLInfoPostgreSQL::GetCATALOGIndexCreate(MIndicesMap& p_indices,bool /*p_duplicateNulls /*= false*/) const
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
    query += index.m_columnName;
    if(index.m_ascending != _T("A"))
    {
      query += _T(" DESC");
    }
  }
  query += _T(")");
  return query;
}

XString
SQLInfoPostgreSQL::GetCATALOGIndexDrop(XString p_schema,XString /*p_tablename*/,XString p_indexname) const
{
  XString sql = _T("DROP INDEX ") + p_schema + _T(".") + p_indexname;
  return sql;
}

// Get extra filter expression for an index column
XString
SQLInfoPostgreSQL::GetCATALOGIndexFilter(MetaIndex& /*p_index*/) const
{
  return _T("");
}

//////////////////////////////////////////////////////////////////////////
// ALL PRIMARY KEY FUNCTIONS

XString
SQLInfoPostgreSQL::GetCATALOGPrimaryExists(XString p_schema,XString p_tablename) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();

  XString query = _T("SELECT COUNT(*)\n")
                  _T("  FROM pg_class      tab\n")
                  _T("      ,pg_constraint con\n")
                  _T("      ,pg_namespaces sch\n")
                  _T(" WHERE tab.relnamespace = sch.oid\n")
                  _T("   AND tab.oid     = con.conrelid\n")
                  _T("   AND con.contype = 'p'\n")
                  _T("   AND sch.name    = '") + p_schema    + _T("'\n")
                  _T("   AND tab.relname = '") + p_tablename + _T("'");
  return query;
}

XString
SQLInfoPostgreSQL::GetCATALOGPrimaryAttributes(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
//   p_schema.MakeLower();
//   p_tablename.MakeLower();
// 
//   XString query = "SELECT con.conname       as constraint_name\n"
//                   "      ,con.conname       as index_name\n"
//                   "      ,con.condeferrable as deferrable\n"
//                   "      ,con.deferred      as initially_deferred\n"
//                   "  FROM pg_class      tab\n"
//                   "      ,pg_constraint con\n"
//                   "      ,pg_namespaces sch\n"
//                   " WHERE tab.relnamespace = sch.oid\n"
//                   "   AND tab.oid          = con.conrelid\n"
//                   "   AND con.contype      = 'p'\n"
//                   "   AND sch.name         = '" + p_schema    + "'\n"
//                   "   AND tab.relname      = '" + p_tablename + "'";
//   return query;
  return _T("");
}

XString
SQLInfoPostgreSQL::GetCATALOGPrimaryCreate(MPrimaryMap& p_primaries) const
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
SQLInfoPostgreSQL::GetCATALOGPrimaryDrop(XString p_schema,XString p_tablename,XString p_constraintname) const
{
  XString sql(_T("ALTER TABLE ") + p_schema + _T(".") + p_tablename + _T("\n")
              _T(" DROP CONSTRAINT ") + p_constraintname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL FOREIGN KEY FUNCTIONS

XString
SQLInfoPostgreSQL::GetCATALOGForeignExists(XString p_schema,XString p_tablename,XString p_constraintname) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  p_constraintname.MakeLower();

  XString sql;
  sql.Format(_T("SELECT COUNT(*)\n")
             _T("  FROM pg_constraint con\n")
             _T("      ,pg_class      cla\n")
             _T("      ,pg_namespace  sch\n")
             _T(" WHERE con.contype      = 'f'\n")
             _T("   AND con.conrelid     = cla.oid\n")
             _T("   AND cla.relnamespace = sch.oid\n")
             _T("   AND sch.nspname      = '") + p_schema + _T("'\n")
             _T("   AND cla.relname      = '") + p_tablename + _T("'\n")
             _T("   AND con.conname      = '") + p_constraintname + _T("'")
            ,p_schema.GetString()
            ,p_tablename.GetString()
            ,p_constraintname.GetString());
  return sql;
}

XString
SQLInfoPostgreSQL::GetCATALOGForeignList(XString& p_schema,XString& p_tablename,int p_maxColumns /*=SQLINFO_MAX_COLUMNS*/) const
{
  XString constraint;
  return GetCATALOGForeignAttributes(p_schema,p_tablename,constraint,p_maxColumns);
}

XString
SQLInfoPostgreSQL::GetCATALOGForeignAttributes(XString& p_schema,XString& p_tablename,XString& p_constraint,bool p_referenced /*=false*/,int p_maxColumns /*=SQLINFO_MAX_COLUMNS*/) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  p_constraint.MakeLower();

  XString query;

  for(int ind = 1; ind <= p_maxColumns; ++ind)
  {
    XString part;
    part.Format(_T("SELECT current_database() as primary_catalog_name\n")
                _T("      ,psc.nspname        as primary_schema_name\n")
                _T("      ,pri.relname        as primary_table_name\n")
                _T("      ,current_database() as foreign_catalog_name\n")
                _T("      ,sch.nspname        as foreign_schema_name\n")
                _T("      ,cla.relname        as foreign_table_name\n")
                _T("      ,prc.conname        as primary_constraint_name\n")
                _T("      ,con.conname        as foreign_constraint_name\n")
                _T("      ,%d                 as key_sequence")
                _T("      ,pky.attname        as primary_key_column\n")
                _T("      ,att.attname        as foreign_key_column\n")
                _T("      ,case con.confupdtype   WHEN 'r' THEN 1\n")
                _T("                              WHEN 'c' THEN 0\n")
                _T("                              WHEN 'n' THEN 2\n")
                _T("                              WHEN 'd' THEN 4\n")
                _T("                              WHEN 'a' THEN 3\n")
                _T("                              ELSE  0\n")
                _T("                              END as update_rule\n")
                _T("      ,case con.confdeltype   WHEN 'r' THEN 1\n")
                _T("                              WHEN 'c' THEN 0\n")
                _T("                              WHEN 'n' THEN 2\n")
                _T("                              WHEN 'd' THEN 4\n")
                _T("                              WHEN 'a' THEN 3\n")
                _T("                              ELSE  0\n")
                _T("                              END as delete_rule\n")
                _T("      ,con.condeferrable as deferrable\n")
                _T("      ,case con.confmatchtype WHEN 's' THEN 1\n")
                _T("                              WHEN 'f' THEN 1\n")
                _T("                              ELSE  0\n")
                _T("                              END as match_option\n")
                _T("      ,con.condeferred   as initially_deferred\n")
                _T("      ,1                 as enabled\n")
                _T("  FROM pg_constraint con\n")
                _T("      ,pg_class      cla\n")
                _T("      ,pg_attribute  att\n")
                _T("      ,pg_namespace  sch\n")
                _T("      ,pg_class      pri\n")
                _T("      ,pg_attribute  pky\n")
                _T("      ,pg_namespace  psc\n")
                _T("      ,pg_constraint prc\n")
                _T(" WHERE con.contype      = 'f'\n")
                _T("   AND con.conrelid     = cla.oid\n")
                _T("   and cla.relnamespace = sch.oid\n")
                _T("   and con.confrelid    = pri.oid\n")
                _T("   and pri.relnamespace = psc.oid\n")
                _T("   and prc.conrelid     = pri.oid\n")
                _T("   and prc.contype      = 'p'\n")
                _T("   and att.attrelid     = cla.oid\n")
                _T("   and att.attnum       = con.conkey[%d]\n")
                _T("   and pky.attrelid     = pri.oid\n")
                _T("   and pky.attnum       = con.confkey[%d]\n")
               ,ind
               ,ind
               ,ind);
    if(!p_schema.IsEmpty())
    {
      if(p_referenced)
      {
        part += _T("\n   AND psc.nspname = '") + p_schema + _T("'");
      }
      else
      {
        part += _T("\n   AND sch.nspname = '") + p_schema + _T("'");
      }
    }
    if(!p_tablename.IsEmpty())
    {
      if(p_referenced)
      {
        part += _T("\n   AND pri.relname = '") + p_tablename + _T("'");
      }
      else
      {
        part += _T("\n   AND cla.relname = '") + p_tablename + _T("'");
      }
    }
    if(!p_constraint.IsEmpty())
    {
      if(p_referenced)
      {
        part += _T("\n   AND prc.conname = '") + p_constraint + _T("'");
      }
      else
      {
        part += _T("\n   AND con.conname = '") + p_constraint + _T("'");
      }
    }

    // Append to query, multiple for multiple columns
    if(!query.IsEmpty())
    {
      query += _T("\nUNION ALL\n");
    }
    query += part;
  }

  // Do NOT bind as parameters
  p_schema.Empty();
  p_tablename.Empty();
  p_constraint.Empty();

  return query;
}

XString
SQLInfoPostgreSQL::GetCATALOGForeignCreate(MForeignMap& p_foreigns) const
{
  // Get first record
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
    default:                      break;
  }
  switch(foreign.m_match)
  {
    case SQL_MATCH_PARTIAL: query += _T("\n      MATCH PARTIAL"); break;
    case SQL_MATCH_SIMPLE:  query += _T("\n      MATCH SIMPLE");  break;
    case SQL_MATCH_FULL:    query += _T("\n      MATCH FULL");    break;
    default:                // In essence: MATCH FULL, but that's already the default
                            break;
  }
  switch(foreign.m_updateRule)
  {
    case SQL_CASCADE :    query += _T("\n      ON UPDATE CASCADE");     break;
    case SQL_SET_NULL:    query += _T("\n      ON UPDATE SET NULL");    break;
    case SQL_SET_DEFAULT: query += _T("\n      ON UPDATE SET DEFAULT"); break;
    case SQL_NO_ACTION:   query += _T("\n      ON UPDATE NO ACTION");   break;
    case SQL_RESTRICT:    query += _T("\n      ON UPDATE NO RESTRICT"); break;
    default:              // In essence: ON UPDATE RESTRICT, but that's already the default
                          break;
  }
  switch(foreign.m_deleteRule)
  {
    case SQL_CASCADE:     query += _T("\n      ON DELETE CASCADE");     break;
    case SQL_SET_NULL:    query += _T("\n      ON DELETE SET NULL");    break;
    case SQL_SET_DEFAULT: query += _T("\n      ON DELETE SET DEFAULT"); break;
    case SQL_NO_ACTION:   query += _T("\n      ON DELETE NO ACTION");   break;
    case SQL_RESTRICT:    query += _T("\n      ON DELETE NO RESTRICT"); break;
    default:              // In essence: ON DELETE RESTRICT, but that's already the default
                          break;
  }
  return query;
}

XString
SQLInfoPostgreSQL::GetCATALOGForeignAlter(MForeignMap& p_original,MForeignMap& p_requested) const
{
  // Make sure we have both
  if(p_original.empty() || p_requested.empty())
  {
    return _T("");
  }

  const MetaForeign& original  = p_original.front();
  const MetaForeign& requested = p_requested.front();

  // Construct the correct tablename
  XString table(original.m_fkTableName);
  if(!original.m_fkSchemaName.IsEmpty())
  {
    table = original.m_fkSchemaName + _T(".") + table;
  }

  // The base foreign key command
  XString query = _T("ALTER TABLE ") + table + _T("\n")
                  _T("ALTER CONSTRAINT ") + original.m_foreignConstraint + _T("\n");

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
  if((original.m_match      != requested.m_match) ||
     (original.m_updateRule != requested.m_updateRule) ||
     (original.m_deleteRule != requested.m_deleteRule))
  {
    // PostgreSQL not capable of altering these attributes, so re-create the constraint
    query.Empty();
  }
  return query;
}

XString
SQLInfoPostgreSQL::GetCATALOGForeignDrop(XString p_schema,XString p_tablename,XString p_constraintname) const
{
  XString sql(_T("ALTER TABLE ") + p_schema + _T(".") + p_tablename + _T("\n")
              _T(" DROP CONSTRAINT ") + p_constraintname);
  return sql;
}

//////////////////////////
// All default constraints
XString
SQLInfoPostgreSQL::GetCATALOGDefaultExists(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_column*/) const
{
  return _T("");
}

XString
SQLInfoPostgreSQL::GetCATALOGDefaultList(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  return _T("");
}

XString
SQLInfoPostgreSQL::GetCATALOGDefaultAttributes(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_column*/) const
{
  return _T("");
}

XString
SQLInfoPostgreSQL::GetCATALOGDefaultCreate(XString /*p_schema*/,XString /*p_tablename*/,XString /*p_constraint*/,XString /*p_column*/,XString /*p_code*/) const
{
  return _T("");
}

XString
SQLInfoPostgreSQL::GetCATALOGDefaultDrop(XString /*p_schema*/,XString /*p_tablename*/,XString /*p_constraint*/) const
{
  return _T("");
}

/////////////////////////
// All check constraints

XString
SQLInfoPostgreSQL::GetCATALOGCheckExists(XString  /*p_schema*/,XString  /*p_tablename*/,XString  /*p_constraint*/) const
{
  return _T("");
}

XString
SQLInfoPostgreSQL::GetCATALOGCheckList(XString  /*p_schema*/,XString  /*p_tablename*/) const
{
  return _T("");
}

XString
SQLInfoPostgreSQL::GetCATALOGCheckAttributes(XString  /*p_schema*/,XString  /*p_tablename*/,XString  /*p_constraint*/) const
{
  return _T("");
}

XString
SQLInfoPostgreSQL::GetCATALOGCheckCreate(XString  /*p_schema*/,XString  /*p_tablename*/,XString  /*p_constraint*/,XString /*p_condition*/) const
{
  return _T("");
}

XString
SQLInfoPostgreSQL::GetCATALOGCheckDrop(XString  /*p_schema*/,XString  /*p_tablename*/,XString  /*p_constraint*/) const
{
  return _T("");
}

//////////////////////////////////////////////////////////////////////////
// ALL TRIGGER FUNCTIONS

XString
SQLInfoPostgreSQL::GetCATALOGTriggerExists(XString p_schema, XString p_tablename, XString p_triggername) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  p_triggername.MakeLower();
  XString sql = _T("SELECT COUNT(*)\n")
                _T("  FROM information_schema.triggers\n")
                _T(" WHERE event_object_schema = '") + p_schema + _T("'\n")
                _T("   AND event_object_table  = '") + p_tablename + _T("'\n")
                _T("   AND trigger_name        = '") + p_triggername + _T("'");
  return sql;
}

XString
SQLInfoPostgreSQL::GetCATALOGTriggerList(XString& p_schema,XString& p_tablename) const
{
  XString triggername;
  return GetCATALOGTriggerAttributes(p_schema,p_tablename,triggername);
}

XString
SQLInfoPostgreSQL::GetCATALOGTriggerAttributes(XString& p_schema,XString& p_tablename,XString& p_triggername) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  p_triggername.MakeLower();
  XString sql;
  sql = _T("SELECT event_object_catalog\n")
        _T("      ,event_object_schema\n")
        _T("      ,event_object_table\n")
        _T("      ,trigger_name\n")
        _T("      ,'' AS trigger_remarks\n")
        _T("      ,0  AS trigger_position\n")
        _T("      ,CASE action_timing\n")
        _T("            WHEN 'AFTER' THEN FALSE ELSE TRUE\n")
        _T("       END AS trigger_before\n")
        _T("      ,CASE event_manipulation\n")
        _T("            WHEN 'INSERT' THEN TRUE ELSE FALSE\n")
        _T("       END AS trigger_insert\n")
        _T("      ,CASE event_manipulation\n")
        _T("            WHEN 'UPDATE' THEN TRUE ELSE FALSE\n")
        _T("       END AS trigger_update\n")
        _T("      ,CASE event_manipulation\n")
        _T("            WHEN 'DELETE' THEN TRUE ELSE FALSE\n")
        _T("       END AS trigger_delete\n")
        _T("      ,CASE event_manipulation\n")
        _T("            WHEN 'SELECT' THEN TRUE ELSE FALSE\n")
        _T("       END AS trigger_select\n")
        _T("      ,FALSE AS trigger_session\n")
        _T("      ,FALSE AS trigger_transaction\n")
        _T("      ,FALSE AS trigger_rollback\n")
        _T("      ,''    AS trigger_referencing\n")
        _T("      ,TRUE  AS trigger_enabled\n")
        _T("      ,action_statement AS trigger_source\n")
        _T("  FROM information_schema.triggers\n");
  if(!p_schema.IsEmpty())
  {
    sql += _T(" WHERE event_object_schema = ?\n");
  }
  if(!p_tablename.IsEmpty())
  {
    sql += p_schema.IsEmpty() ? _T(" WHERE ") : _T("   AND ");
    sql += _T("event_object_table = ?\n");
  }
  if(!p_triggername.IsEmpty())
  {
    sql += p_schema.IsEmpty() && p_tablename.IsEmpty() ? _T(" WHERE ") : _T("   AND ");
    sql += _T("trigger_name = ?\n");
  }
  sql += _T(" ORDER BY 1,2,3,4");
  return sql;
}

XString
SQLInfoPostgreSQL::GetCATALOGTriggerCreate(MetaTrigger& /*p_trigger*/) const
{
  return _T("");
}

XString
SQLInfoPostgreSQL::GetCATALOGTriggerDrop(XString p_schema, XString p_tablename, XString p_triggername) const
{
  return _T("");
}

//////////////////////////////////////////////////////////////////////////
// ALL SEQUENCE FUNCTIONS

XString
SQLInfoPostgreSQL::GetCATALOGSequenceExists(XString p_schema, XString p_sequence) const
{
  p_schema.MakeLower();
  p_sequence.MakeLower();

  XString sql(_T("SELECT COUNT(*)\n")
              _T("  FROM information_schema.sequences\n")
              _T(" WHERE sequence_schema = '") + p_schema   + _T("'\n")
              _T("   AND sequence_name   = '") + p_sequence + _T("'"));
  return sql;
}

XString
SQLInfoPostgreSQL::GetCATALOGSequenceList(XString& p_schema,XString& p_pattern) const
{
  p_schema.MakeLower();
  p_pattern.MakeLower();
  if(!p_pattern.IsEmpty() && p_pattern.Compare(_T("%")))
  {
    p_pattern = _T("%") + p_pattern + _T("%");
  }

  XString sql = _T("SELECT ''              AS catalog_name\n")
                _T("      ,sequence_schema AS schema_name\n")
                _T("      ,sequence_name\n")
                _T("      ,start_value     AS current_value\n")
                _T("      ,0               AS minimal_value\n")
                _T("      ,increment\n")
                _T("      ,0               AS cache\n")
                _T("      ,case cycle_option\n")
                _T("            when 'NO' then 1\n")
                _T("                      else 0\n")
                _T("       end             AS cycle\n")
                _T("      ,0               AS ordering\n")
                _T("  FROM information_schema.sequences\n");
  if(!p_schema.IsEmpty())
  {
    sql += _T(" WHERE sequence_schema  = ?\n");
  }
  if(!p_pattern.IsEmpty())
  {
    sql += p_schema.IsEmpty() ? _T(" WHERE ") : _T("   AND ");
    sql += _T("sequence_name LIKE ?");
  }
  return sql;
}

XString
SQLInfoPostgreSQL::GetCATALOGSequenceAttributes(XString& p_schema,XString& p_sequence) const
{
  p_schema.MakeLower();
  p_sequence.MakeLower();

  XString sql = _T("SELECT ''              AS catalog_name\n")
                _T("      ,sequence_schema AS schema_name\n")
                _T("      ,sequence_name\n")
                _T("      ,start_value     AS current_value\n")
                _T("      ,0               AS minimal_value\n")
                _T("      ,increment\n")
                _T("      ,0               AS cache\n")
                _T("      ,case cycle_option\n")
                _T("            when 'NO' then 1\n")
                _T("                      else 0\n")
                _T("       end             AS cycle\n")
                _T("      ,0               AS ordering\n")
                _T("  FROM information_schema.sequences\n");
  if(!p_schema.IsEmpty())
  {
    sql += _T(" WHERE sequence_schema = ?\n");
  }
  if(!p_sequence.IsEmpty())
  {
    sql += p_schema.IsEmpty() ? _T(" WHERE ") : _T("   AND ");
    sql += _T("sequence_name   = ?");
  }
  return sql;
}

XString
SQLInfoPostgreSQL::GetCATALOGSequenceCreate(MetaSequence& p_sequence) const
{
  XString sql(_T("CREATE SEQUENCE "));

  if (!p_sequence.m_schemaName.IsEmpty())
  {
    sql += p_sequence.m_schemaName + _T(".");
  }
  sql += p_sequence.m_sequenceName;
  sql.AppendFormat(_T(" START WITH %-12.0f"), p_sequence.m_currentValue);
  if(p_sequence.m_cache)
  {
    sql.AppendFormat(_T(" CACHE %d"),p_sequence.m_cache);
  }
  sql += p_sequence.m_cycle ? _T(" CYCLE") : _T(" NO CYCLE");
  return sql;
}

XString
SQLInfoPostgreSQL::GetCATALOGSequenceDrop(XString p_schema, XString p_sequence) const
{
  XString sql(_T("DROP SEQUENCE ") + p_schema + _T(".") + p_sequence);
  return  sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL VIEW FUNCTIONS

XString 
SQLInfoPostgreSQL::GetCATALOGViewExists(XString& p_schema,XString& p_viewname) const
{
  p_schema.Empty();  // do not bind as parameter
  p_viewname.MakeLower();
  XString sql(_T("SELECT COUNT(*)\n")
              _T("  FROM pg_views\n") 
              _T(" WHERE view_name = ?"));
  return sql;
}

XString 
SQLInfoPostgreSQL::GetCATALOGViewList(XString& p_schema,XString& p_pattern) const
{
  return GetCATALOGViewAttributes(p_schema,p_pattern);
}

XString 
SQLInfoPostgreSQL::GetCATALOGViewAttributes(XString& p_schema,XString& p_viewname) const
{
  p_schema.MakeLower();
  p_viewname.MakeLower();
  XString query = _T("SELECT current_catalog as table_catalog\n")
                  _T("      ,sch.nspname     as table_schema\n")
                  _T("      ,tab.relname     as table_name\n")
                  _T("      ,'TABLE'         as object_type\n")
                  _T("      ,''              as remarks\n")
                  _T("      ,sch.nspname || '.' || tab.relname as fullname\n")
                  _T("      ,''              as tablespace\n")
                  _T("      ,CASE tab.relpersistence\n")
                  _T("            WHEN 'p' THEN 0\n")
                  _T("                     ELSE 1\n")
                  _T("       END as temporary\n")
                  _T("  FROM pg_catalog.pg_class tab\n")
                  _T("       left join pg_catalog.pg_namespace sch on tab.relnamespace = sch.oid\n")
                  _T(" WHERE tab.relkind = 'v'\n")
                  _T("   AND substring(sch.nspname,1,3) <> 'pg_'\n")
                  _T("   AND sch.nspname <> 'information_schema'\n");
  if(!p_schema.IsEmpty())
  {
    query += _T("    AND sch.name = ?\n");
  }
  if(!p_viewname.IsEmpty())
  {
    query += _T("   AND tab.relname ");
    query += p_viewname.Find('%') >= 0 ? _T("LIKE") : _T("=");
    query += _T(" ?\n");
  }
  query += _T(" ORDER BY 1,2,3");
                  
  return query;
}

XString
SQLInfoPostgreSQL::GetCATALOGViewText(XString& /*p_schema*/,XString& /*p_viewname*/) const
{
  // Cannot query this, Use ODBC functions
  return _T("");
}

XString
SQLInfoPostgreSQL::GetCATALOGViewCreate(XString p_schema,XString p_viewname,XString p_contents,bool /*p_ifexists = true*/) const
{
  return _T("CREATE OR REPLACE VIEW ") + p_schema + _T(".") + p_viewname + _T("\n") + p_contents;
}

XString 
SQLInfoPostgreSQL::GetCATALOGViewRename(XString p_schema,XString p_viewname,XString p_newname)    const
{
  return _T("");
}

XString 
SQLInfoPostgreSQL::GetCATALOGViewDrop(XString p_schema,XString p_viewname,XString& p_precursor) const
{
  p_precursor.Empty();
  return _T("DROP VIEW ") + p_schema + _T(".") + p_viewname;
}

// All Privilege functions
XString
SQLInfoPostgreSQL::GetCATALOGTablePrivileges(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  return _T("");
}

XString 
SQLInfoPostgreSQL::GetCATALOGColumnPrivileges(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_columnname*/) const
{
  return _T("");
}

XString
SQLInfoPostgreSQL::GetCATALOGSequencePrivilege(XString& /*p_schema*/,XString& /*p_sequence*/) const
{
  return _T("");
}

XString 
SQLInfoPostgreSQL::GetCATALOGGrantPrivilege(XString p_schema,XString p_objectname,XString p_privilege,XString p_grantee,bool p_grantable)
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
SQLInfoPostgreSQL::GetCATALOGRevokePrivilege(XString p_schema,XString p_objectname,XString p_privilege,XString p_grantee)
{
  XString sql;
  sql.Format(_T("REVOKE %s ON %s.%s FROM %s"),p_privilege.GetString(),p_schema.GetString(),p_objectname.GetString(),p_grantee.GetString());
  return sql;
}

// All Synonym functions
XString
SQLInfoPostgreSQL::GetCATALOGSynonymList(XString& /*p_schema*/,XString& /*p_pattern*/) const
{
  // Not implemented yet
  return _T("");
}

XString
SQLInfoPostgreSQL::GetCATALOGSynonymAttributes(XString& /*p_schema*/,XString& /*p_synonym*/) const
{
  // Not implemented yet
  return _T("");
}

XString
SQLInfoPostgreSQL::GetCATALOGSynonymCreate(XString& /*p_schema*/,XString& /*p_synonym*/,XString /*p_forObject*/,bool /*p_private = true*/) const
{
  // Not implemented yet
  return _T("");
}

XString
SQLInfoPostgreSQL::GetCATALOGSynonymDrop(XString& /*p_schema*/,XString& /*p_synonym*/,bool /*p_private = true*/) const
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
SQLInfoPostgreSQL::GetPSMProcedureExists(XString p_schema, XString p_procedure) const
{
  p_schema.MakeLower();
  p_procedure.MakeLower();
  return _T("SELECT count(*)\n")
         _T("  FROM pg_proc\n")
         _T(" WHERE proname = '") + p_procedure + _T("'\n;");
}

XString
SQLInfoPostgreSQL::GetPSMProcedureList(XString& /*p_schema*/) const
{
  return _T("");
}

XString
SQLInfoPostgreSQL::GetPSMProcedureAttributes(XString& /*p_schema*/,XString& /*p_procedure*/) const
{
//   p_schema.MakeLower();
//   p_procedure.MakeLower();
// 
//   XString sql = "SELECT TEXT from ALL_SOURCE "
//                 "WHERE type = 'FUNCTION' "
//                 "AND name  = '" + p_procedure + "'\n"
//                 "AND owner = '" + p_schema    + "'";
//   return sql;
  return _T("");
}

XString
SQLInfoPostgreSQL::GetPSMProcedureSourcecode(XString p_schema, XString p_procedure) const
{
  return _T("");
}

XString
SQLInfoPostgreSQL::GetPSMProcedureCreate(MetaProcedure& /*p_procedure*/) const
{
  return _T("");
}

XString
SQLInfoPostgreSQL::GetPSMProcedureDrop(XString p_schema, XString p_procedure,bool /*p_function /*=false*/) const
{
  return _T("");
}

XString
SQLInfoPostgreSQL::GetPSMProcedureErrors(XString p_schema,XString p_procedure) const
{
  return _T("");
}

XString
SQLInfoPostgreSQL::GetPSMProcedurePrivilege(XString& /*p_schema*/,XString& /*p_procedure*/) const
{
  return _T("");
}

// And it's parameters
XString
SQLInfoPostgreSQL::GetPSMProcedureParameters(XString& p_schema,XString& p_procedure) const
{
  p_schema.MakeLower();
  p_procedure.MakeLower();
  XString sql;

  sql = _T("SELECT par.specific_catalog\n")
        _T("      ,par.specific_schema\n")
        _T("      ,fun.routine_name\n")
        _T("      ,par.parameter_name\n")
        _T("      ,case par.parameter_mode\n")
        _T("            when 'IN'    then 1\n")
        _T("            when 'OUT'   then 4\n")
        _T("            when 'INOUT' then 2\n")
        _T("       end as columntype\n")
        _T("      ,1 as datatype\n")
        _T("      ,par.data_type as typename\n")
        _T("      ,par.character_maximum_length\n")
        _T("      ,par.numeric_precision\n")
        _T("      ,par.numeric_scale\n")
        _T("      ,par.numeric_precision_radix\n")
        _T("      ,1 as is_nullable\n")
        _T("      ,'' as remarks\n")
        _T("      ,par.parameter_default\n")
        _T("      ,1 as datatype3\n")
        _T("      ,par.interval_precision as subtype\n")
        _T("      ,par.character_octet_length\n")
        _T("      ,par.ordinal_position\n")
        _T("      ,'YES' as isNullable\n")
        _T("  FROM information_schema.parameters par\n")
        _T("      ,information_schema.routines fun\n")
        _T(" WHERE par.specific_catalog = fun.specific_catalog\n")
        _T("   AND par.specific_schema  = fun.specific_schema\n")
        _T("   AND par.specific_name    = fun.specific_name\n");
  if(!p_schema.IsEmpty())
  {
    sql += _T("   AND fun.specific_schema = ?\n");
  }
  if(!p_procedure.IsEmpty())
  {
    sql += _T("   AND fun.routine_name    = ?\n");
  }
  sql += _T(" ORDER BY 1,2,3,18");

  return sql;
}

//////////////////////////////////////////////////////////////////////////
//
// ALL PSM LANGUAGE ELEMENTS
//
//////////////////////////////////////////////////////////////////////////

XString
SQLInfoPostgreSQL::GetPSMDeclaration(bool    /*p_first*/
                                    ,XString p_variable
                                    ,int     p_datatype
                                    ,int     p_precision /*= 0 */
                                    ,int     p_scale     /*= 0 */
                                    ,XString p_default   /*= ""*/
                                    ,XString p_domain    /*= ""*/
                                    ,XString p_asColumn  /*= ""*/) const
{
  XString line;
  line.Format(_T("%s "),p_variable.GetString());

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
  else if(!p_asColumn)
  {
    line += p_asColumn + _T("%TYPE");
  }
  line += _T(";\n");
  return line;
}

XString
SQLInfoPostgreSQL::GetPSMAssignment(XString p_variable,XString p_statement /*=""*/) const
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
SQLInfoPostgreSQL::GetPSMIF(XString p_condition) const
{
  XString line(_T("IF ("));
  line += p_condition;
  line += _T(") THEN\n");
  return line;
}

XString
SQLInfoPostgreSQL::GetPSMIFElse() const
{
  return _T("ELSE\n");
}

XString
SQLInfoPostgreSQL::GetPSMIFEnd() const
{
  return _T("END IF;\n");
}

XString
SQLInfoPostgreSQL::GetPSMWhile(XString p_condition) const
{
  return _T("WHILE (") + p_condition + _T(") LOOP\n");
}

XString
SQLInfoPostgreSQL::GetPSMWhileEnd() const
{
  return _T("END LOOP;\n");
}

XString
SQLInfoPostgreSQL::GetPSMLOOP() const
{
  return _T("LOOP\n");
}

XString
SQLInfoPostgreSQL::GetPSMLOOPEnd() const
{
  return _T("END LOOP;\n");
}

XString
SQLInfoPostgreSQL::GetPSMBREAK() const
{
  return _T("EXIT;\n");
}

XString
SQLInfoPostgreSQL::GetPSMRETURN(XString p_statement /*= ""*/) const
{
  XString line(_T("RETURN"));
  if(!p_statement.IsEmpty())
  {
    line += _T(" ") + p_statement;
  }
  line += _T(";\n");
  return line;
}

XString
SQLInfoPostgreSQL::GetPSMExecute(XString p_procedure,MParameterMap& p_parameters) const
{
  XString line;
  line.Format(_T("SELECT %s("),p_procedure.GetString());
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
SQLInfoPostgreSQL::GetPSMCursorDeclaration(XString p_cursorname,XString p_select) const
{
  return _T("DECLARE ") + p_cursorname + _T(" CURSOR FOR ") + p_select + _T(";");
}

XString
SQLInfoPostgreSQL::GetPSMCursorFetch(XString p_cursorname,std::vector<XString>& /*p_columnnames*/,std::vector<XString>& p_variablenames) const
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
SQLInfoPostgreSQL::GetPSMExceptionCatchNoData() const
{
  return _T("EXCEPTION WHEN 100 THEN\n");
  // Followed by block and 'END;'
}

XString
SQLInfoPostgreSQL::GetPSMExceptionCatch(XString p_sqlState) const
{
  return _T("EXCEPTION WHEN ") + p_sqlState + _T(" THEN\n");
  // Followed by block and 'END;'
}

XString
SQLInfoPostgreSQL::GetPSMExceptionRaise(XString p_sqlState) const
{
  return _T("RAISE EXCEPTION SQLSTATE ") + p_sqlState + _T(";\n");
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
SQLInfoPostgreSQL::GetSESSIONMyself() const
{
  XString query = _T("SELECT procpid\n")
                  _T("      ,user\n")
                  _T("      ,datname\n")
                  _T("      ,current_timestamp\n")  // moment
                  _T("      ,''\n")         // remote address
                  _T("      ,''\n")         // remote process name
                  _T("      ,''\n")         // remote process ID
                  _T("  FROM pg_stat_activity\n")
                  _T(" WHERE procpid = pg_backend_pid()");    
  return query;
}

XString
SQLInfoPostgreSQL::GetSESSIONExists(XString p_sessionID) const
{
  XString sql = _T("SELECT COUNT(*)\n")
                _T("  FROM pg_stat_activity\n")
                _T(" WHERE procpid = ") + p_sessionID;
  return sql;
}

XString
SQLInfoPostgreSQL::GetSESSIONList() const
{
  return GetSESSIONAttributes(_T(""));
}

XString
SQLInfoPostgreSQL::GetSESSIONAttributes(XString p_sessionID) const
{
  XString sql = _T("SELECT procpid\n")
                _T("      ,user\n")
                _T("      ,datname\n")
                _T("      ,current_timestamp\n")  // moment
                _T("      ,''\n")         // remote address
                _T("      ,''\n")         // remote process name
                _T("      ,''\n")         // remote process ID
                _T("  FROM pg_stat_activity");
  if (!p_sessionID.IsEmpty())
  {
    sql += _T("\n WHERE procpid = ") + p_sessionID;
  }
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// Transactions

XString
SQLInfoPostgreSQL::GetSESSIONConstraintsDeferred() const
{
  return _T("set transaction deferrable");
}

XString
SQLInfoPostgreSQL::GetSESSIONConstraintsImmediate() const
{
  return _T("set transaction not deferrable");
}

//////////////////////////////////////////////////////////////////////////
//
// Call FUNCTION/PROCEDURE from within program
// As a RDBMS dependent extension of "DoSQLCall" of the SQLQuery object
//
//////////////////////////////////////////////////////////////////////////

// Calling a stored function or procedure if the RDBMS does not support ODBC call escapes
SQLVariant*
SQLInfoPostgreSQL::DoSQLCall(SQLQuery* p_query,XString& p_schema,XString& p_procedure)
{
  // PostgreSQL does not support the return parameter of the "{[?=]CALL procedure(?,?)}" sequence
  // instead you have to do a "SELECT procedure(?,?)" 
  // The result set is the set of output parameters
  // DOES ONLY SUPPORT A SINGLE ROW RESULT SET!!
  SQLQuery query(m_database);
  XString sql   = ConstructSQLForProcedureCall(p_query,&query,p_schema,p_procedure);
  int numReturn = GetCountReturnParameters(p_query);

  query.DoSQLStatement(sql);
  if(query.GetRecord())
  {
    // Processing the result
    int setIndex = -1;
    int recIndex = 0;
    for(int resIndex = 1; resIndex <= query.GetNumberOfColumns(); ++resIndex)
    {
      SQLVariant var;
      int dataType = 0;
      int type = 0;
      bool ready = false;

      // Finding the next OUTPUT parameter in the original query call
      do
      {
        const SQLVariant* target = p_query->GetParameter(++setIndex);
        if(target == nullptr)
        {
          throw StdException(_T("Wrong number of output parameters for procedure call"));
        }
        type = target->GetParameterType();
        dataType = target->GetDataType();
      }
      while(type != SQL_PARAM_OUTPUT && type != SQL_PARAM_INPUT_OUTPUT);

      // Getting the next result from the result set
      SQLVariant* result = query.GetColumn(resIndex);
      if(result->GetDataType() == SQL_C_CHAR ||
         result->GetDataType() == SQL_C_WCHAR)
      {
        XString resstring = result->GetAsChar();
        if(!resstring.IsEmpty() && resstring.GetAt(0) == _T('(') && numReturn)
        {
          var = GetVarFromRecord(dataType,(LPTSTR)resstring.GetString(),recIndex++,ready);
          resIndex = 0;
          result = &var;
        }
      }

      // Storing the result;
      p_query->SetParameter(setIndex,result);

      // At the end of a multi-parameter record?
      if(ready)
      {
        break;
      }
    }
    // Returning the first return column as the result of the procedure
    return p_query->GetParameter(0);
  }
  return nullptr;
}

// Calling a stored function with named parameters, returning a value
SQLVariant*
SQLInfoPostgreSQL::DoSQLCallNamedParameters(SQLQuery* /*p_query*/,XString& /*p_schema*/,XString& /*p_procedure*/)
{
  return nullptr;
}

//////////////////////////////////////////////////////////////////////////
//
// PRIVATE METHODS
//
//////////////////////////////////////////////////////////////////////////

// Get the number of OUTPUT or INPUT_OUTPUT parameters
// In the parameter list (disregarding the 0th parameter)
int
SQLInfoPostgreSQL::GetCountReturnParameters(SQLQuery* p_query)
{
  int count = 0;
  int index = 1;

  while(true)
  {
    var* parameter = p_query->GetParameter(index++);
    if(parameter == nullptr) break;
    int type = parameter->GetParameterType();
    if((type == SQL_PARAM_OUTPUT || type == SQL_PARAM_INPUT_OUTPUT))
    {
      ++count;
    }
  }
  return count;
}

// Construct the "SELECT * FROM procedure(?,?)" (input parameters ONLY!)
XString
SQLInfoPostgreSQL::ConstructSQLForProcedureCall(SQLQuery* p_query
                                               ,SQLQuery* p_thecall
                                               ,XString&  p_schema
                                               ,const XString& p_procedure)
{
  // Start with select form
  XString sql = _T("SELECT ");
  if(!p_schema.IsEmpty())
  {
    sql += p_schema;
    sql += _T(".");
  }
  sql += p_procedure;

  // Opening parenthesis
  sql += _T("(");

  // Build list of markers
  int ind = 1;
  int res = 1;
  while(true)
  {
    // Try get the next parameter
    var* parameter = p_query->GetParameter(ind);
    if(parameter == nullptr) break;

    // Input parameters ONLY!!
    int type = parameter->GetParameterType();
    if(type == SQL_PARAM_INPUT)
    {
      // Add marker
      if(ind > 1) sql += _T(",");
      sql += _T("?");

      // Add the parameter with the result counter!
      p_thecall->SetParameter(res++,parameter);
    }
    // Next parameter
    ++ind;
  }

  // CLosing parenthesis
  sql += _T(")");

  // The procedure IS the singular object
  // Procedure **MUST** end with "SUSPEND;" 
  return sql;
}

// Get column from PostgreSQL result set
// (123,"string",21.12)
SQLVariant
SQLInfoPostgreSQL::GetVarFromRecord(int p_type,LPTSTR p_pointer,int p_column,bool& p_ready)
{
  int beginPos  = 0;
  int endPos    = 0;
  int curColumn = 0;
  SQLVariant variant;

  // Skip first '('
  if(*p_pointer == '(') ++beginPos;

  while(p_pointer[beginPos])
  {
    // Find begin next column
    bool isString = p_pointer[beginPos] == _T('\"');
    if(isString) ++beginPos;
    endPos = beginPos;

    // Find end of the field
    while(p_pointer[endPos] && p_pointer[endPos] != _T(',')
                            && p_pointer[endPos] != _T(')') 
                            && p_pointer[endPos] != '\"')
    {
      ++endPos;
    }

    // If column found
    if(p_column == curColumn)
    {
      TCHAR temp = p_pointer[endPos];
      p_pointer[endPos] = 0;
      variant.SetData(p_type,&p_pointer[beginPos]);
      p_pointer[endPos] = temp;
      break;
    }

    // Next column
    beginPos = endPos;
    if(isString && p_pointer[beginPos] == '\"') ++beginPos;
    if(p_pointer[beginPos] == _T(',') || p_pointer[beginPos] == ')') ++beginPos;
    ++curColumn;
  }

  // Are we ready with the record
  if(p_pointer[endPos] == '\"') ++ endPos;
  if(p_pointer[endPos] == ')')
  {
    p_ready = true;
  }
  return variant;
}

// End of namespace
}
