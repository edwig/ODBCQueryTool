////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoPostgreSQL.cpp
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

// For PostgreSQL the identifiers are transformed to LOWER case
// as the system catalog is stored in this case setting.
#define IdentifierCorrect(ident)   if(!p_quoted || !IsIdentifierMixedCase(ident)) ident.MakeLower()

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
    return sql.GetColumn(1)->GetAsString();
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

// Support for "as" in alias statements (FROM clause)
bool
SQLInfoPostgreSQL::GetRDBMSSupportsAsInAlias() const
{
  return true;
}

// Foreign key DDL defines the index and cannot reuse already existing ones
bool
SQLInfoPostgreSQL::GetRDBMSForeignKeyDefinesIndex() const
{
  // No: you must EXPLICITLY create an index, independent from the foreign key creation.
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

// Identifier rules differ per RDBMS
bool
SQLInfoPostgreSQL::IsIdentifier(XString p_identifier) const
{
  // Cannot be empty and cannot exceed this amount of characters
  if(p_identifier.GetLength() == 0 ||
     p_identifier.GetLength() > (int)GetMaxIdentifierNameLength())
  {
    return false;
  }
  // Must start with one alpha char
  if(!_istalpha(p_identifier.GetAt(0)))
  {
    return false;
  }
  for(int index = 0;index < p_identifier.GetLength();++index)
  {
    // Can be upper/lower alpha or a number OR an underscore
    TCHAR ch = p_identifier.GetAt(index);
    if(!_istalnum(ch) && ch != '_')
    {
      return false;
    }
  }
  return true;
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

XString
SQLInfoPostgreSQL::GetSQLGenerateSequence(XString p_sequence) const
{
  return _T("SELECT ") + p_sequence + _T(".nextval FROM DUAL");
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
SQLInfoPostgreSQL::GetSQLCommitSubTransaction(XString /*p_savepointName*/) const
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
    p_sql.AppendFormat(_T("\nLIMIT %d "),p_top);
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

// Pre- and postfix statements for a bulk import
XString
SQLInfoPostgreSQL::GetBulkImportPrefix(XString /*p_schema*/,XString /*p_tablename*/,bool /*p_identity = true*/,bool /*p_constraints = true*/) const
{
  return _T("");
}

XString
SQLInfoPostgreSQL::GetBulkImportPostfix(XString /*p_schema*/,XString /*p_tablename*/,bool /*p_identity = true*/,bool /*p_constraints = true*/) const
{
  return _T("");
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

// Changes to parameters before binding to an ODBC HSTMT handle (returning the At-Exec status)
bool
SQLInfoPostgreSQL::DoBindParameterFixup(SQLSMALLINT& /*p_dataType*/,SQLSMALLINT& /*p_sqlDatatype*/,SQLULEN& /*p_columnSize*/,SQLSMALLINT& /*p_scale*/,SQLLEN& /*p_bufferSize*/,SQLLEN* /*p_indicator*/) const
{
  return false;
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

// All user defined compound data types
XString
SQLInfoPostgreSQL::GetCATALOGTypeExists(XString& p_schema,XString& p_typename,bool p_quoted /*= false*/) const
{
  XString sql(_T("SELECT COUNT(*)\n"
                 "  FROM pg_type t\n"
                 "        JOIN pg_class c ON t.typrelid = c.oid\n"
                 "  WHERE n.nspname NOT IN ('pg_catalog', 'information_schema')\n"));
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("    AND n.nspname = ?\n");
  }
  IdentifierCorrect(p_typename);
  sql += _T("    AND t.typname = ?");
  return sql;
}

XString
SQLInfoPostgreSQL::GetCATALOGTypeList(XString& p_schema,XString& p_pattern,bool p_quoted /*= false*/) const
{
  return GetCATALOGTypeAttributes(p_schema,p_pattern,p_quoted);
}

XString
SQLInfoPostgreSQL::GetCATALOGTypeAttributes(XString& p_schema,XString& p_typename,bool p_quoted /*= false*/) const
{
  XString sql1(_T("SELECT current_catalog as catalogname\n"
                  "      ,n.nspname    as schemaname\n"
                  "      ,t.typname    as typename\n"
                  "      ,'C'          as typeusage\n"
                  "      ,a.attnum     as ordinal\n"
                  "      ,a.attname    as attribute\n"
                  "      ,pg_catalog.format_type(a.atttypid, a.atttypmod) as datatype\n"
                  "      ,t.typnotnull as notnull\n"
                  "      ,''           as defaultvalue"
                  "      ,''           as domaincheck\n"
                  "      ,''           as remarks\n"
                  "      ,'<@>'        as source\n"
                  "  FROM pg_type t\n"
                  "       JOIN pg_class     c ON c.oid = t.typrelid\n"
                  "       JOIN pg_namespace n ON n.oid = c.relnamespace\n"
                  "       JOIN pg_attribute a ON c.oid = a.attrelid\n"
                  " WHERE t.typtype     = 'c'\n"
                  "   AND c.relkind     = 'c'\n"
                  "   AND a.attnum      > 0 \n"
                  "   AND NOT a.attisdropped\n"
                  "   AND n.nspname NOT IN ('pg_catalog','information_schema')\n"));

  XString sql2(_T("SELECT current_catalog as catalogname\n"
                  "      ,n.nspname       as schemaname\n"
                  "   	  ,t.typname       as typename\n"
                  "      ,'E'             as typeusage\n"
                  "      ,e.enumsortorder as ordinal\n"
                  "      ,e.enumlabel     as attribute\n"
                  "      ,''              as datatype\n"
                  "      ,false           as notnull\n"
                  "      ,''              as defaultvalue\n"
                  "      ,''              as domaincheck\n"
                  "      ,''              as remarks\n"
                  "      ,'<@>'           as source\n"
                  "  FROM pg_type t\n"
                  "       JOIN pg_enum      e ON t.oid = e.enumtypid\n"
                  "       JOIN pg_namespace n ON n.oid = t.typnamespace\n"
                  " WHERE t.typtype     = 'e'\n"
                  "   AND n.nspname NOT IN ('pg_catalog', 'information_schema')\n"));

  XString sql3(_T("SELECT current_catalog as catalogname\n"
                  "      ,n.nspname       as schemaname\n"
                  "      ,t.typname       as typename\n"
                  "      ,'D'             as typeusage\n"
                  "      ,1               as ordinal\n"
                  "      ,''              as attribute\n"
                  "      ,pg_catalog.format_type(t.typbasetype, t.typtypmod) as datatype\n"
                  "      ,t.typnotnull    as notnull\n"
                  "      ,t.typdefault    as defaultvalue\n"
                  "      ,cc.checks       as domaincheck\n"
                  "      ,''              as remarks\n"
                  "      ,'<@>'           as source\n"
                  "  FROM pg_catalog.pg_type t\n"
                  "       INNER JOIN pg_catalog.pg_namespace n ON n.oid = t.typnamespace\n"
                  "       LEFT OUTER JOIN (\n"
                  "                  SELECT r.contypid, string_agg(pg_get_constraintdef(r.oid, true), ' AND ') AS checks\n"
                  "                    FROM pg_catalog.pg_constraint r\n"
                  "                   WHERE r.contype = 'c'\n"
                  "                   GROUP BY r.contypid\n"
                  "                 ) cc ON cc.contypid = t.oid\n"
                  " WHERE t.typtype = 'd'\n"
                  "   AND n.nspname NOT IN ('pg_catalog', 'information_schema')\n"));
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql1 += _T("   AND n.nspname = '") + p_schema + _T("'\n");
    sql2 += _T("   AND n.nspname = '") + p_schema + _T("'\n");
    sql3 += _T("   AND n.nspname = ?\n");
  }
  if(!p_typename.IsEmpty())
  {
    IdentifierCorrect(p_typename);
    sql1 += _T("   AND t.typname ");
    sql1 += p_typename.Find(_T("%")) >= 0 ? _T("LIKE") : _T("=");
    sql1 += _T("'") + p_typename + _T("'\n");
    sql2 += _T("   AND t.typname ");
    sql2 += p_typename.Find(_T("%")) >= 0 ? _T("LIKE") : _T("=");
    sql2 += _T("'") + p_typename + _T("'\n");
    sql3 += _T("   AND t.typname ");
    sql3 += p_typename.Find(_T("%")) >= 0 ? _T("LIKE") : _T("=");
    sql3 += _T(" ?\n");
  }
  XString sql = sql1 + _T("UNION ALL\n") + 
                sql2 + _T("UNION ALL\n") + 
                sql3;
  sql += _T(" ORDER by 1,2,3,5");
  return sql;
}

XString
SQLInfoPostgreSQL::GetCATALOGTypeSource(XString& p_schema,XString& p_typename,bool p_quoted /*= false*/) const
{
  // Composite types
  XString sql1(_T("SELECT 1 as object\n"
                  "      ,1 as line\n"
                  "      ,'CREATE TYPE \"' || n.nspname || '\".\"' || t.typname || '\" AS(' ||\n"
                  "       string_agg(a.attname || ' ' || pg_catalog.format_type(a.atttypid, a.atttypmod), ', ') ||\n"
                  "       ');' AS create_composite_statement\n"
                  "  FROM pg_type t\n"
                  "       JOIN pg_class     c ON t.typrelid     = c.oid\n"
                  "       JOIN pg_namespace n ON c.relnamespace = n.oid\n"
                  "       JOIN pg_attribute a ON a.attrelid     = c.oid\n"
                  " WHERE t.typtype = 'c'\n"
                  "   AND c.relkind = 'c'\n"
                  "   AND a.attnum  > 0 \n"
                  "   AND NOT a.attisdropped\n"
                  "   AND n.nspname NOT IN ('pg_catalog','information_schema')\n"));

  // Enumeration types
  XString sql2(_T("SELECT 1 as object\n"
                  "      ,1 as line\n"
                  "      ,'CREATE TYPE \"' || n.nspname || '\".\"' || t.typname || '\" AS ENUM (' ||\n"
                  "       string_agg(quote_literal(e.enumlabel), ', ') || ');' AS create_enum_statement\n"
                  "  FROM pg_type t\n"
                  "       JOIN pg_enum      e ON t.oid = e.enumtypid\n"
                  "       JOIN pg_namespace n ON t.typnamespace = n.oid\n"
                  " WHERE t.typtype = 'e'\n"
                  "   AND n.nspname NOT IN ('pg_catalog','information_schema')\n"));
  // Domein check types
  XString sql3(_T("SELECT 1 as object\n"
                  "      ,1 as line\n"
                  "      ,'CREATE DOMAIN \"' || n.nspname || '\".\"' || t.typname || '\" AS ' ||\n"
                  "       pg_catalog.format_type(t.typbasetype, t.typtypmod)  ||\n"
                  "       CASE WHEN t.typnotnull THEN ' NOT NULL' ELSE '' END ||\n"
                  "       CASE WHEN t.typdefault IS NOT NULL THEN ' DEFAULT ' || t.typdefault ELSE '' END ||\n"
                  "       CASE\n"
                  "          WHEN cc.checks IS NOT NULL THEN ' ' || cc.checks\n"
                  "          ELSE ''\n"
                  "       END ||\n"
                  "       ';' AS create_domain_statement\n"
                  "  FROM pg_catalog.pg_type t\n"
                  "       INNER JOIN pg_catalog.pg_namespace n ON n.oid = t.typnamespace\n"
                  "       LEFT OUTER JOIN (\n"
                  "                  SELECT r.contypid, string_agg(pg_get_constraintdef(r.oid, true), ' AND ') AS checks\n"
                  "                    FROM pg_catalog.pg_constraint r\n"
                  "                   WHERE r.contype = 'c'\n"
                  "                   GROUP BY r.contypid\n"
                  "                 ) cc ON cc.contypid = t.oid\n"
                  "  WHERE t.typtype = 'd'\n"
                  "   AND n.nspname NOT IN ('pg_catalog', 'information_schema')\n"));

  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql1 += _T("   AND n.nspname = '") + p_schema + _T("'\n");
    sql2 += _T("   AND n.nspname = '") + p_schema + _T("'\n");
    sql3 += _T("   AND n.nspname = '") + p_schema + _T("'\n");
  }
  IdentifierCorrect(p_typename);
  sql1 += _T("   AND t.typname = '") + p_typename + _T("'\n");
  sql2 += _T("   AND t.typname = '") + p_typename + _T("'\n");
  sql3 += _T("   AND t.typname = '") + p_typename + _T("'\n");

  // GROUP/ORDER
  sql1 += _T(" GROUP BY n.nspname, t.typname\n");
  sql2 += _T(" GROUP BY n.nspname, t.typname\n");
  sql3 += _T(" GROUP BY n.nspname, t.typname,t.typbasetype,t.typtypmod,t.typnotnull,t.typdefault,cc.checks");

  XString sql = sql1 + _T("UNION ALL\n") +
                sql2 + _T("UNION ALL\n") +
                sql3;
  return sql;
}

XString
SQLInfoPostgreSQL::GetCATALOGTypeCreate(MUserTypeMap& p_type) const
{
  for(auto& type : p_type)
  {
    if(!type.m_source.IsEmpty() && type.m_source.Compare(_T("<@>")))
    {
      return type.m_source;
    }
  }
  return XString();
}

XString
SQLInfoPostgreSQL::GetCATALOGTypeDrop(XString p_schema,XString p_typename) const
{
  XString sql(_T("DROP TYPE IF EXISTS "));
  if(!p_schema.IsEmpty())
  {
    sql += QIQ(p_schema) + _T(".");
  }
  sql += QIQ(p_typename);
  return sql;
}

// Get SQL to check if a table already exists in the database
XString
SQLInfoPostgreSQL::GetCATALOGTableExists(XString& p_schema,XString& p_tablename,bool p_quoted /*= false*/) const
{
  IdentifierCorrect(p_schema);
  IdentifierCorrect(p_tablename);

  XString query = _T("SELECT count(*)\n")
                  _T("  FROM pg_class cl\n")
                  _T("      ,pg_namespace ns\n")
                  _T(" WHERE cl.relnamespace = ns.oid\n")
                  _T("   AND ns.nspname = ?\n")
                  _T("   AND cl.relname = ?");
  return query;
}

XString
SQLInfoPostgreSQL::GetCATALOGTablesList(XString& p_schema,XString& p_pattern,bool p_quoted /*= false*/) const
{
  return GetCATALOGTableAttributes(p_schema,p_pattern,p_quoted);
}

XString
SQLInfoPostgreSQL::GetCATALOGTableAttributes(XString& p_schema,XString& p_tablename,bool p_quoted /*= false*/) const
{
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
    IdentifierCorrect(p_schema);
    query += _T("    AND sch.nspname = ?\n");
  }
  if(!p_tablename.IsEmpty())
  {
    IdentifierCorrect(p_tablename);
    query += _T("   AND tab.relname ");
    query += p_tablename.Find('%') >= 0 ? _T("LIKE") : _T("=");
    query += _T(" ?\n");
  }
  query += _T(" ORDER BY 1,2,3");
                  
  return query;
}

XString
SQLInfoPostgreSQL::GetCATALOGTableSynonyms(XString& /*p_schema*/,XString& /*p_tablename*/,bool /*p_quoted = false*/) const
{
  // PostgreSQL does not have table synonyms.
  XString sql = _T("SELECT * FROM pg_class WHERE 0 = 1");
  return sql;
}

XString
SQLInfoPostgreSQL::GetCATALOGTableCatalog(XString& p_schema,XString& p_tablename,bool p_quoted /*= false*/) const
{
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
    IdentifierCorrect(p_schema);
    query += _T("    AND sch.name = ?\n");
  }
  if(!p_tablename.IsEmpty())
  {
    IdentifierCorrect(p_tablename);
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
    sql += QIQ(p_table.m_schema);
    sql += _T(".");
  }
  sql += QIQ(p_table.m_table);
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
  XString sql(_T("RENAME ") + QIQ(p_schema) + _T(".") + QIQ(p_tablename) + _T(" TO ") + QIQ(p_newname));
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
    sql += QIQ(p_schema) + _T(".");
  }
  sql += QIQ(p_tablename);
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
  return _T("CREATE GLOBAL TEMPORARY TABLE ") + QIQ(p_schema) + _T(".") + QIQ(p_tablename) + _T(" WITHOUT OIDS\nAS ") + p_select;
}

XString 
SQLInfoPostgreSQL::GetCATALOGTemptableIntoTemp(XString p_schema,XString p_tablename,XString p_select) const
{
  return _T("INSERT INTO ") + QIQ(p_schema) + _T(".") + QIQ(p_tablename) + _T("\n") + QIQ(p_select);
}

XString 
SQLInfoPostgreSQL::GetCATALOGTemptableDrop(XString p_schema,XString p_tablename) const
{
  XString tablename = QIQ(p_schema) + _T(".") + QIQ(p_tablename);
  return _T("DELETE FROM ")    + tablename + _T(";\n")
         _T("<@>\n")
         _T("TRUNCATE TABLE ") + tablename + _T(";\n")
         _T("<@>\n")
         _T("DROP TABLE ")     + tablename + _T(";\n");
}

//////////////////////////////////////////////////////////////////////////
// ALL COLUMN FUNCTIONS

XString 
SQLInfoPostgreSQL::GetCATALOGColumnExists(XString p_schema,XString p_tablename,XString p_columnname,bool p_quoted /*= false*/) const
{
  IdentifierCorrect(p_schema);
  IdentifierCorrect(p_tablename);
  IdentifierCorrect(p_columnname);

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
SQLInfoPostgreSQL::GetCATALOGColumnList(XString& /*p_schema*/,XString& /*p_tablename*/,bool /*p_quoted /*= false*/) const
{
  // Standard ODBC driver suffices
  return _T("");
}

XString 
SQLInfoPostgreSQL::GetCATALOGColumnAttributes(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_columnname*/,bool /*p_quoted /*= false*/) const
{
  // Standard ODBC driver suffices
  return _T("");
}

XString 
SQLInfoPostgreSQL::GetCATALOGColumnCreate(MetaColumn& p_column) const
{
  XString sql = _T("ALTER TABLE ")  + QIQ(p_column.m_schema) + _T(".") + QIQ(p_column.m_table)  + _T("\n")
                _T("  ADD COLUMN ") + QIQ(p_column.m_column) + _T(" ") + QIQ(p_column.m_typename);
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
  XString sql(_T("ALTER TABLE  ") + QIQ(p_column.m_schema) + _T(".") + QIQ(p_column.m_table) + _T("\n")
              _T("ALTER COLUMN ") + QIQ(p_column.m_column) + _T(" SET DATA TYPE ") + p_column.m_typename);
  p_column.GetPrecisionAndScale(sql);

  sql += p_column.m_nullable ? _T(" DROP ") : _T(" SET ");
  sql += _T("NOT NULL");
  return sql;
}

XString 
SQLInfoPostgreSQL::GetCATALOGColumnRename(XString p_schema,XString p_tablename,XString p_columnname,XString p_newname,XString p_datatype) const
{
  XString sqlCode = _T("ALTER TABLE ") + QIQ(p_schema) + _T(".") + QIQ(p_tablename) + _T("\n")
                    _T("  ADD ")  + QIQ(p_newname) + _T(" ") + p_datatype + _T(";\n");
  sqlCode        += _T("UPDATE ") + QIQ(p_schema)  + _T(".")     + QIQ(p_tablename)  + _T("\n")
                    _T("   SET ") + QIQ(p_newname) + _T(" = ")   + QIQ(p_columnname) + _T(";\n");
  sqlCode        += _T("ALTER TABLE ") + QIQ(p_schema) + _T(".") + QIQ(p_tablename)  + _T("\n")
                    _T(" DROP COLUMN ") + QIQ(p_columnname) + _T(";");
  return sqlCode;
}

XString
SQLInfoPostgreSQL::GetCATALOGColumnDrop(XString p_schema,XString p_tablename,XString p_columnname) const
{
  XString sql(_T("ALTER TABLE ") + QIQ(p_schema) + _T(".") + QIQ(p_tablename) + _T("\n")
              _T(" DROP COLUMN ") + QIQ(p_columnname));
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL INDICES FUNCTIONS

// All index functions
XString
SQLInfoPostgreSQL::GetCATALOGIndexExists(XString /*p_schema*/,XString /*p_tablename*/,XString /*p_indexname*/,bool /*p_quoted = false*/) const
{
  return _T("");
}

XString
SQLInfoPostgreSQL::GetCATALOGIndexList(XString& p_schema,XString& p_tablename,bool p_quoted /*= false*/) const
{
  XString indexname;
  return GetCATALOGIndexAttributes(p_schema,p_tablename,indexname,p_quoted);
}

XString
SQLInfoPostgreSQL::GetCATALOGIndexAttributes(XString& p_schema,XString& p_tablename,XString& p_indexname,bool p_quoted /*= false*/) const
{
  if(p_indexname == _T("0"))
  {
    // Cannot get table statistics
    return XString();
  }
  XString sql = _T("SELECT current_database() AS catalog_name\n"
                   "      ,n.nspname          AS schema_name\n"
                   "      ,t.relname          AS table_name\n"
                   "      ,CASE\n"
                   "          WHEN i.indisunique THEN 0\n"
                   "          ELSE 1\n"
                   "       END                AS non_unique\n"
                   "      ,NULL::text         AS index_qualifier\n"  // Use schema name!
                   "      ,ci.relname         AS index_name\n"
                   "      ,1                  AS type\n"              // tableIndexOther
                   "      ,keys.ordinality    AS ordinal_position\n"  // a.attnum
                   "      ,a.attname          AS column_name\n"
                   "      ,CASE i.indoption[a.attnum - 1] & 1\n"
                   "          WHEN 1 THEN 'D'\n"  // DESCENDING
                   "          ELSE 'A'\n"         // ASCENDING
                   "       END                AS asc_or_desc\n"
                   "      ,NULL::integer      AS cardinality\n"
                   "      ,NULL::integer      AS pages\n"
                   "      ,NULL::text         AS filter_condition\n"
                   "  FROM pg_class t\n"
                   "       JOIN pg_index i     ON t.oid  = i.indrelid\n"
                   "       JOIN pg_class ci    ON ci.oid = i.indexrelid\n"
                   "       JOIN pg_namespace n ON n.oid  = t.relnamespace\n"
                   "       JOIN unnest(i.indkey) WITH ORDINALITY AS keys(attnum, ordinality) ON true\n"
                   "       JOIN pg_attribute a ON a.attrelid = t.oid AND a.attnum = keys.attnum\n"
                   " WHERE 1 = 1\n");
  if(m_filterPKFK)
  {
    // Select the indexes for re-creation of the table
    sql += _T("   AND NOT i.indisprimary\n"
              "   AND NOT EXISTS\n"
              "         ( SELECT 1 FROM pg_constraint c\n"
              "            WHERE c.conindid = i.indexrelid AND c.contype = 'f' )");
  }
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND n.nspname = ?\n");
  }
  if(!p_tablename.IsEmpty())
  {
    IdentifierCorrect(p_tablename);
    sql += _T("   AND t.relname = ?\n");
  }
  if(!p_indexname.IsEmpty())
  {
    IdentifierCorrect(p_indexname);
    sql += _T("   AND ci.relname = ?\n");
  }
  sql += _T(" ORDER BY 1,2,3,6,8");
  return sql;
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
      query += QIQ(index.m_indexName);
      query += _T(" ON ");
      if(!index.m_schemaName.IsEmpty())
      {
        query += QIQ(index.m_schemaName) + _T(".");
      }
      query += QIQ(index.m_tableName);
      query += _T("(");
    }
    else
    {
      query += _T(",");
    }
    query += QIQ(index.m_columnName);
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
  XString sql = _T("DROP INDEX IF EXISTS ") + QIQ(p_schema) + _T(".") + QIQ(p_indexname);
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
SQLInfoPostgreSQL::GetCATALOGPrimaryExists(XString p_schema,XString p_tablename,bool p_quoted /*= false*/) const
{
  IdentifierCorrect(p_schema);
  IdentifierCorrect(p_tablename);

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
SQLInfoPostgreSQL::GetCATALOGPrimaryAttributes(XString& p_schema,XString& p_tablename,bool p_quoted /*= false*/) const
{
  XString sql = _T("SELECT current_database() AS catalog_name\n"
                   "      ,n.nspname          AS table_schema\n"
                   "      ,c.relname          AS table_name\n"
                   "      ,a.attname          AS column_name\n"
                   "      ,k.ordinal_position AS key_sequence\n"
                   "      ,con.conname        AS pk_name\n"
                   "  FROM pg_catalog.pg_constraint con\n"
                   "       JOIN pg_catalog.pg_class     c ON c.oid = con.conrelid\n"
                   "       JOIN pg_catalog.pg_namespace n ON n.oid = c.relnamespace\n"
                   "       JOIN LATERAL unnest(con.conkey) WITH ORDINALITY AS k(attnum, ordinal_position) ON TRUE\n"
                   "       JOIN pg_catalog.pg_attribute a ON a.attrelid = c.oid AND a.attnum = k.attnum\n"
                   " WHERE con.contype = 'p'\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND n.nspname   = ?\n");
  }
  if(!p_tablename.IsEmpty())
  {
    IdentifierCorrect(p_tablename);
    sql += _T("   AND c.relname   = ?\n");
  }
  sql += _T(" ORDER BY 1,2,3,5\n");

  return sql;
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
        query += QIQ(prim.m_schema) + _T(".");
      }
      query += QIQ(prim.m_table) + _T("\n");
      query += _T("  ADD CONSTRAINT ") + QIQ(prim.m_constraintName) + _T("\n");
      query += _T("      PRIMARY KEY (");

    }
    else
    {
      query += _T(",");
    }
    query += QIQ(prim.m_columnName);
  }
  query += _T(")");
  return query;
}

XString
SQLInfoPostgreSQL::GetCATALOGPrimaryDrop(XString p_schema,XString p_tablename,XString p_constraintname) const
{
  XString sql(_T("ALTER TABLE ") + QIQ(p_schema) + _T(".") + QIQ(p_tablename) + _T("\n")
              _T(" DROP CONSTRAINT ") + QIQ(p_constraintname));
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL FOREIGN KEY FUNCTIONS

XString
SQLInfoPostgreSQL::GetCATALOGForeignExists(XString p_schema,XString p_tablename,XString p_constraintname,bool p_quoted /*= false*/) const
{
  IdentifierCorrect(p_schema);
  IdentifierCorrect(p_tablename);
  IdentifierCorrect(p_constraintname);

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
SQLInfoPostgreSQL::GetCATALOGForeignList(XString& p_schema,XString& p_tablename,bool p_quoted /*= false*/) const
{
  XString constraint;
  return GetCATALOGForeignAttributes(p_schema,p_tablename,constraint,p_quoted);
}

XString
SQLInfoPostgreSQL::GetCATALOGForeignAttributes(XString& p_schema
                                              ,XString& p_tablename
                                              ,XString& p_constraint
                                              ,bool     p_referenced /*=false*/
                                              ,bool     p_quoted     /*= false*/) const
{
  XString query = _T("SELECT current_database() AS pktable_catalog\n")
                  _T("      ,pk_ns.nspname      AS pktable_schema\n")
                  _T("      ,pk_tbl.relname     AS pktable_name\n")
                  _T("      ,pk_col.attname     AS pktable_columnname\n")
                  _T("      ,current_database() AS fktable_catalog\n")
                  _T("      ,fk_ns.nspname      AS fktable_schema\n")
                  _T("      ,fk_tbl.relname     AS fktable_name\n")
                  _T("      ,fk_col.attname     AS fktable_columname\n")
                  _T("      ,pos.n              AS key_sequence\n")
                  _T("      ,CASE conf.confupdtype\n")
                  _T("           WHEN 'c' THEN 0  -- CASCADE\n")
                  _T("           WHEN 'n' THEN 1  -- SET NULL\n")
                  _T("           WHEN 'd' THEN 2  -- SET DEFAULT\n")
                  _T("           WHEN 'r' THEN 3  -- RESTRICT\n")
                  _T("           WHEN 'a' THEN 4  -- NO ACTION\n")
                  _T("       END AS update_rule\n")
                  _T("      ,CASE conf.confdeltype\n")
                  _T("           WHEN 'c' THEN 0\n")
                  _T("           WHEN 'n' THEN 1\n")
                  _T("           WHEN 'd' THEN 2\n")
                  _T("           WHEN 'r' THEN 3\n")
                  _T("           WHEN 'a' THEN 4\n")
                  _T("       END AS delete_rule\n")
                  _T("      ,conf.conname    AS fk_constraint_name\n")
                  _T("      ,pk_conf.conname AS pk_constraint_name\n")
                  _T("      ,CASE conf.condeferrable\n")
                  _T("           WHEN true THEN\n")
                  _T("               CASE conf.condeferred\n")
                  _T("                   WHEN true THEN 5  -- INITIALLY DEFERRED\n")
                  _T("                   ELSE 6            -- INITIALLY IMMEDIATE\n")
                  _T("               END\n")
                  _T("           ELSE 7                    -- NOT DEFERRABLE\n")
                  _T("       END AS deferrability\n")
                  _T("      ,case conf.confmatchtype WHEN 's' THEN 1\n")
                  _T("                               WHEN 'f' THEN 1\n")
                  _T("                               ELSE  0\n")
                  _T("                               END as match_option\n")
                  _T("      ,conf.condeferred   as initially_deferred\n")
                  _T("      ,1                  as enabled\n")
                  _T("  FROM pg_constraint conf\n")
                  _T("       JOIN pg_class     fk_tbl ON fk_tbl.oid = conf.conrelid\n")
                  _T("       JOIN pg_namespace fk_ns  ON fk_ns.oid  = fk_tbl.relnamespace\n")
                  _T("       JOIN pg_class     pk_tbl ON pk_tbl.oid = conf.confrelid\n")
                  _T("       JOIN pg_namespace pk_ns  ON pk_ns.oid  = pk_tbl.relnamespace\n")
                  _T("       JOIN pg_attribute fk_col ON fk_col.attrelid = conf.conrelid  AND fk_col.attnum = ANY (conf.conkey)\n")
                  _T("       JOIN pg_attribute pk_col ON pk_col.attrelid = conf.confrelid AND pk_col.attnum = ANY (conf.confkey)\n")
                  _T("       LEFT JOIN pg_constraint pk_conf ON pk_conf.conrelid = conf.confrelid AND pk_conf.contype IN ('p', 'u')\n")
                  _T("       JOIN generate_series(1, array_length(conf.conkey, 1)) AS pos(n) ON true\n")
                  _T(" WHERE conf.contype = 'f'\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    if(p_referenced)
    {
      query += _T("   AND pk_ns.nspname = ?\n");
    }
    else
    {
      query += _T("   AND fk_ns.nspname = ?\n");
    }
  }
  if(!p_tablename.IsEmpty())
  {
    IdentifierCorrect(p_tablename);
    if(p_referenced)
    {
      query += _T("   AND pk_tbl.relname = ?\n");
    }
    else
    {
      query += _T("   AND fk_tbl.relname = ?");
    }
  }
  if(!p_constraint.IsEmpty())
  {
    IdentifierCorrect(p_constraint);
    if(p_referenced)
    {
      query += _T("   AND pk_col.attname = ?");
    }
    else
    {
      query += _T("   AND fk_col.attname = ?");
    }
  }
  query += _T(" ORDER BY 2,3,5,9");
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
    table = QIQ(foreign.m_fkSchemaName) + _T(".") + QIQ(table);
  }
  if(!foreign.m_pkSchemaName.IsEmpty())
  {
    primary = QIQ(foreign.m_pkSchemaName) + _T(".") + QIQ(primary);
  }

  // The base foreign key command
  XString query = _T("ALTER TABLE ") + table + _T("\n")
                  _T("  ADD CONSTRAINT ") + QIQ(foreign.m_foreignConstraint) + _T("\n")
                  _T("      FOREIGN KEY (");

  // Add the foreign key columns
  bool extra = false;
  for(const auto& key : p_foreigns)
  {
    if(extra) query += _T(",");
    query += QIQ(key.m_fkColumnName);
    extra  = true;
  }

  // Add references primary table
  query += _T(")\n      REFERENCES ") + primary + _T("(");

  // Add the primary key columns
  extra = false;
  for(const auto& key : p_foreigns)
  {
    if(extra) query += _T(",");
    query += QIQ(key.m_pkColumnName);
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
    case SQL_MATCH_PARTIAL: break; // "MATCH PARTIAL" Not yet implemented
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
  XString table(QIQ(original.m_fkTableName));
  if(!original.m_fkSchemaName.IsEmpty())
  {
    table = QIQ(original.m_fkSchemaName) + _T(".") + table;
  }

  // The base foreign key command
  XString query = _T("ALTER TABLE ") + table + _T("\n")
                  _T("ALTER CONSTRAINT ") + QIQ(original.m_foreignConstraint) + _T("\n");

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
  XString sql(_T("ALTER TABLE ") + QIQ(p_schema) + _T(".") + QIQ(p_tablename) + _T("\n")
              _T(" DROP CONSTRAINT ") + QIQ(p_constraintname));
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
SQLInfoPostgreSQL::GetCATALOGCheckExists(XString p_schema,XString p_tablename,XString p_constraint,bool p_quoted /*= false*/) const
{
  XString sql = _T("SELECT COUNT(*)\n"
                   "  FROM pg_constraint con\n"
                   "       JOIN pg_class     rel ON rel.oid = con.conrelid\n"
                   "       JOIN pg_namespace nsp ON nsp.oid = rel.relnamespace\n"
                   " WHERE con.contype = 'c'\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND nsp.nspname = ?\n");
  }
  if(!p_tablename.IsEmpty())
  {
    IdentifierCorrect(p_tablename);
    sql += _T("   AND rel.relname = ?\n");
  }
  if(!p_constraint.IsEmpty())
  {
    IdentifierCorrect(p_constraint);
    sql += _T("   AND con.conname = ?");
  }
  return sql;
}

XString
SQLInfoPostgreSQL::GetCATALOGCheckList(XString p_schema,XString p_tablename,bool p_quoted /*= false*/) const
{
  XString constraint;
  return GetCATALOGCheckAttributes(p_schema,p_tablename,constraint,p_quoted);
}

XString
SQLInfoPostgreSQL::GetCATALOGCheckAttributes(XString  p_schema,XString p_tablename,XString p_constraint,bool p_quoted /*= false*/) const
{
  XString sql = _T("SELECT current_database() AS catalog_name\n"
                   "      ,nsp.nspname        AS schema_name\n"
                   "      ,rel.relname        AS table_name\n"
                   "      ,con.conname        AS constraint_name\n"
                   "      ,pg_get_constraintdef(con.oid) AS constraint_definition\n"
                   "  FROM pg_constraint con\n"
                   "       JOIN pg_class     rel ON rel.oid = con.conrelid\n"
                   "       JOIN pg_namespace nsp ON nsp.oid = rel.relnamespace\n"
                   " WHERE con.contype = 'c'\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND nsp.nspname = ?\n");
  }
  if(!p_tablename.IsEmpty())
  {
    IdentifierCorrect(p_tablename);
    sql += _T("   AND rel.relname = ?\n");
  }
  if(!p_constraint.IsEmpty())
  {
    IdentifierCorrect(p_constraint);
    sql += _T("   AND con.conname = ?\n");
  }
  sql += _T(" ORDER BY 1,2,3,4");
  return sql;
}

XString
SQLInfoPostgreSQL::GetCATALOGCheckCreate(XString p_schema,XString p_tablename,XString p_constraint,XString p_condition) const
{
  // In PostgreSQL the 'condition' already contains the keyword 'CHECK'
  XString sql = _T("ALTER TABLE ") + QIQ(p_schema) + _T(".") + QIQ(p_tablename) + _T("\n");
  sql += _T("  ADD CONSTRAINT ") + QIQ(p_constraint) + _T(" ") + p_condition;
  return sql;
}

XString
SQLInfoPostgreSQL::GetCATALOGCheckDrop(XString p_schema,XString p_tablename,XString p_constraint) const
{
  XString sql = _T("ALTER TABLE ") + QIQ(p_schema) + _T(".") + QIQ(p_tablename) + _T("\n");
  sql += _T(" DROP CONSTRAINT ") + QIQ(p_constraint);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL TRIGGER FUNCTIONS

XString
SQLInfoPostgreSQL::GetCATALOGTriggerExists(XString p_schema, XString p_tablename, XString p_triggername,bool p_quoted /*= false*/) const
{
  IdentifierCorrect(p_schema);
  IdentifierCorrect(p_tablename);
  IdentifierCorrect(p_triggername);

  XString sql = _T("SELECT COUNT(*)\n")
                _T("  FROM information_schema.triggers\n")
                _T(" WHERE event_object_schema = '") + p_schema      + _T("'\n")
                _T("   AND event_object_table  = '") + p_tablename   + _T("'\n")
                _T("   AND trigger_name        = '") + p_triggername + _T("'");
  return sql;
}

XString
SQLInfoPostgreSQL::GetCATALOGTriggerList(XString& p_schema,XString& p_tablename,bool p_quoted /*= false*/) const
{
  XString triggername;
  return GetCATALOGTriggerAttributes(p_schema,p_tablename,triggername,p_quoted);
}

XString
SQLInfoPostgreSQL::GetCATALOGTriggerAttributes(XString& p_schema,XString& p_tablename,XString& p_triggername,bool p_quoted /*= false*/) const
{
  XString sql = _T("SELECT current_database()   as trigger_catalog\n")
                _T("      ,ns.nspname           as trigger_schema\n")
                _T("      ,cl.relname           as trigger_table\n")
                _T("      ,tr.tgname            as trigger_name\n")
                _T("      ,co.description       as remark\n")
                _T("      ,row_number() over () as position\n")
                _T("      ,(tgtype & 2)  >> 1   as is_before\n")
                _T("      ,(tgtype & 4)  >> 2   as fires_on_insert\n")
                _T("      ,(tgtype & 16) >> 4   as fires_on_update\n")
                _T("      ,(tgtype & 8)  >> 3   as fires_on_delete\n")
                _T("      ,0                    as fires_on_select\n")
                _T("      ,0                    as fires_on_session\n")
                _T("      ,0                    as fires_on_transaction\n")
                _T("      ,0                    as fires_on_rollback\n")
                _T("      ,rl.relname           as trigger_referencing\n")
                _T("      ,CASE tr.tgenabled\n")
                _T("            WHEN 'O' THEN 1\n")
                _T("            ELSE 0\n")
                _T("       END                  as enabled\n")
                _T("      ,pg_get_triggerdef(tr.oid) as source\n")
                _T("  FROM pg_catalog.pg_trigger tr\n")
                _T("       JOIN pg_catalog.pg_class     cl ON cl.oid = tr.tgrelid\n")
                _T("       JOIN pg_catalog.pg_namespace ns ON ns.oid = cl.relnamespace\n")
                _T("       LEFT OUTER JOIN pg_catalog.pg_description co ON tr.oid = co.objoid\n")
                _T("       LEFT OUTER JOIN pg_catalog.pg_class       rl On rl.oid = tr.tgconstrrelid\n")
                _T(" WHERE NOT tr.tgisinternal\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND ns.nspname = ?\n");
  }
  if(!p_tablename.IsEmpty())
  {
    IdentifierCorrect(p_tablename);
    sql += _T("   AND cl.relname = ?\n");
  }
  if(!p_triggername.IsEmpty())
  {
    IdentifierCorrect(p_triggername);
    sql += _T("   AND tr.tgname = ?\n");
  }
  // Alphabetically ordered by triggername fills row_number() with the positioning!
  sql += _T(" ORDER BY 1,2,3,4");
  return sql;
}

XString
SQLInfoPostgreSQL::GetCATALOGTriggerCreate(MetaTrigger& p_trigger) const
{
  // Already retrieved by TriggerAttributes!
  return p_trigger.m_source;
}

XString
SQLInfoPostgreSQL::GetCATALOGTriggerDrop(XString p_schema,XString p_tablename,XString p_triggername) const
{
  XString sql = _T("DROP TRIGGER IF EXISTS ");
  sql += QIQ(p_triggername) + _T(" ON ");
  if(!p_schema.IsEmpty())
  {
    sql += QIQ(p_schema) + _T(".");
  }
  sql += QIQ(p_tablename) + _T(" CASCADE");
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL SEQUENCE FUNCTIONS

XString
SQLInfoPostgreSQL::GetCATALOGSequenceExists(XString p_schema, XString p_sequence,bool p_quoted /*= false*/) const
{
  if(!p_quoted)
  {
    p_schema.MakeLower();
    p_sequence.MakeLower();
  }
  XString sql(_T("SELECT COUNT(*)\n")
              _T("  FROM information_schema.sequences\n")
              _T(" WHERE sequence_schema = '") + p_schema   + _T("'\n")
              _T("   AND sequence_name   = '") + p_sequence + _T("'"));
  return sql;
}

XString
SQLInfoPostgreSQL::GetCATALOGSequenceList(XString& p_schema,XString& p_pattern,bool p_quoted /*= false*/) const
{
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
                _T("      ,''              AS remarks\n")
                _T("  FROM information_schema.sequences\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T(" WHERE sequence_schema  = ?\n");
  }
  if(!p_pattern.IsEmpty())
  {
    IdentifierCorrect(p_pattern);
    sql += p_schema.IsEmpty() ? _T(" WHERE ") : _T("   AND ");
    sql += _T("sequence_name LIKE ?");
  }
  return sql;
}

XString
SQLInfoPostgreSQL::GetCATALOGSequenceAttributes(XString& p_schema,XString& p_sequence,bool p_quoted /*= false*/) const
{
  XString sql = _T("SELECT current_database() AS catalog_name\n")
                _T("      ,sequence_schema    AS schema_name\n")
                _T("      ,sequence_name\n")
                _T("      ,start_value        AS current_value\n")
                _T("      ,0                  AS minimal_value\n")
                _T("      ,increment\n")
                _T("      ,0                  AS cache\n")
                _T("      ,case cycle_option\n")
                _T("            when 'NO' then 1\n")
                _T("                      else 0\n")
                _T("       end                AS cycle\n")
                _T("      ,0                  AS ordering\n")
                _T("      ,''                 AS remarks\n")
                _T("  FROM information_schema.sequences\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T(" WHERE sequence_schema = ?\n");
  }
  if(!p_sequence.IsEmpty())
  {
    IdentifierCorrect(p_sequence);
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
    sql += QIQ(p_sequence.m_schemaName) + _T(".");
  }
  sql += QIQ(p_sequence.m_sequenceName);
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
  XString sql(_T("DROP SEQUENCE IF EXISTS ") + QIQ(p_schema) + _T(".") + QIQ(p_sequence));
  return  sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL VIEW FUNCTIONS

XString 
SQLInfoPostgreSQL::GetCATALOGViewExists(XString& p_schema,XString& p_viewname,bool p_quoted /*= false*/) const
{
  XString sql(_T("SELECT COUNT(*)\n")
              _T("  FROM pg_catalog.pg_class cl\n") 
              _T("       JOIN pg_catalog.pg_namespace ns ON ns.oid = cl.relnamespace\n")
              _T(" WHERE cl.relkind = 'v'\n"));
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("    AND ns.nspname = ?\n");
  }
  if(!p_viewname.IsEmpty())
  {
    IdentifierCorrect(p_viewname);
    sql += _T("   AND cl.relname = ?");
  }
  return sql;
}

XString 
SQLInfoPostgreSQL::GetCATALOGViewList(XString& p_schema,XString& p_pattern,bool p_quoted /*= false*/) const
{
  return GetCATALOGViewAttributes(p_schema,p_pattern,p_quoted);
}

XString 
SQLInfoPostgreSQL::GetCATALOGViewAttributes(XString& p_schema,XString& p_viewname,bool p_quoted /*= false*/) const
{
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
    IdentifierCorrect(p_schema);
    query += _T("    AND sch.nspname = ?\n");
  }
  if(!p_viewname.IsEmpty())
  {
    IdentifierCorrect(p_viewname);
    query += _T("   AND tab.relname ");
    query += p_viewname.Find('%') >= 0 ? _T("LIKE") : _T("=");
    query += _T(" ?\n");
  }
  query += _T(" ORDER BY 1,2,3");
                  
  return query;
}

XString
SQLInfoPostgreSQL::GetCATALOGViewText(XString& p_schema,XString& p_viewname,bool p_quoted /*= false*/) const
{
  XString query = _T("SELECT 'CREATE OR REPLACE VIEW \"' || sch.nspname || '\".\"' || tab.relname || '\" AS ' ||\n")
                  _T("       pg_get_viewdef(tab.oid) as viewtext\n")
                  _T("  FROM pg_catalog.pg_class tab\n")
                  _T("       left join pg_catalog.pg_namespace sch on tab.relnamespace = sch.oid\n")
                  _T(" WHERE tab.relkind = 'v'\n")
                  _T("   AND substring(sch.nspname,1,3) <> 'pg_'\n")
                  _T("   AND sch.nspname <> 'information_schema'\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    query += _T("    AND sch.nspname = '") + p_schema + _T("'\n");
  }
  if(!p_viewname.IsEmpty())
  {
    IdentifierCorrect(p_viewname);
    query += _T("   AND tab.relname = '") + p_viewname + _T("'");
  }
  return query;
}

XString
SQLInfoPostgreSQL::GetCATALOGViewCreate(XString p_schema,XString p_viewname,XString p_contents,bool /*p_ifexists = true*/) const
{
  return _T("CREATE OR REPLACE VIEW ") + QIQ(p_schema) + _T(".") + QIQ(p_viewname) + _T("\n") + p_contents;
}

XString 
SQLInfoPostgreSQL::GetCATALOGViewRename(XString /*p_schema*/,XString /*p_viewname*/,XString /*p_newname*/)    const
{
  return _T("");
}

XString 
SQLInfoPostgreSQL::GetCATALOGViewDrop(XString p_schema,XString p_viewname,XString& p_precursor) const
{
  p_precursor.Empty();
  return _T("DROP VIEW IF EXISTS ") + QIQ(p_schema) + _T(".") + QIQ(p_viewname) + _T(" CASCADE");
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
SQLInfoPostgreSQL::GetCATALOGGrantPrivilege(XString p_schema,XString p_objectname,XString p_subObject,XString p_privilege,XString p_grantee,bool p_grantable)
{
  XString sql;
  sql.Format(_T("GRANT %s"),p_privilege.GetString());
  if(!p_subObject.IsEmpty())
  {
    sql.AppendFormat(_T("(%s)"),QIQ(p_subObject).GetString());
  }
  sql += _T(" ON ");
  if(!p_schema.IsEmpty())
  {
    sql.AppendFormat(_T("%s."),QIQ(p_schema).GetString());
  }
  sql.AppendFormat(_T("%s TO %s"),QIQ(p_objectname).GetString(),QIQ(p_grantee).GetString());
  if(p_grantable)
  {
    sql += _T(" WITH GRANT OPTION");
  }
  return sql;
}

XString 
SQLInfoPostgreSQL::GetCATALOGRevokePrivilege(XString p_schema,XString p_objectname,XString p_subObject,XString p_privilege,XString p_grantee)
{
  XString sql;
  sql.Format(_T("REVOKE %s"),p_privilege.GetString());
  if(!p_subObject.IsEmpty())
  {
    sql.AppendFormat(_T("(%s)"),QIQ(p_subObject).GetString());
  }
  sql += _T(" ON ");
  if(!p_schema.IsEmpty())
  {
    sql.AppendFormat(_T("%s."),QIQ(p_schema).GetString());
  }
  sql.AppendFormat(_T("%s FROM %s"),QIQ(p_objectname).GetString(),QIQ(p_grantee).GetString());
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

// For ALL objects
XString
SQLInfoPostgreSQL::GetCATALOGCommentCreate(XString p_schema,XString p_object,XString p_name,XString p_subObject,XString p_remark) const
{
  XString sql;
  if(!p_object.IsEmpty() && !p_name.IsEmpty() && !p_remark.IsEmpty())
  {
    sql.Format(_T("COMMENT ON %s "),p_object.GetString());
    if(!p_schema.IsEmpty())
    {
      sql += QIQ(p_schema) + _T(".");
    }
    sql += QIQ(p_name);
    if(!p_subObject.IsEmpty())
    {
      sql += _T(".") + QIQ(p_subObject);
    }
    if(p_remark.CompareNoCase(_T("NULL")))
    {
      sql.AppendFormat(_T(" IS '%s'"),p_remark.GetString());
    }
    else
    {
      sql += _T(" IS NULL");
    }
  }
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
SQLInfoPostgreSQL::GetPSMProcedureExists(XString p_schema, XString p_procedure,bool p_quoted /*= false*/) const
{
  IdentifierCorrect(p_procedure);

  XString sql = _T("SELECT count(*)\n")
                _T("  FROM information_schema.routines\n")
                _T(" WHERE routine_name = '") + p_procedure + _T("'");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("\n   AND specific_schema = '") + p_schema + _T("'");
  }
  return sql;
}

XString
SQLInfoPostgreSQL::GetPSMProcedureList(XString& p_schema,XString p_procedure,bool p_quoted /*= false*/) const
{
  XString sql = _T("SELECT current_database() as procedure_catalog\n")
                _T("      ,ns.nspname         as procedure_schema\n")
                _T("      ,pr.proname         as procedure_name\n")
                _T("      ,CASE pr.prokind\n")
                _T("           WHEN 'f' THEN 2\n")
                _T("           WHEN 'p' THEN 1\n")
                _T("           WHEN 'a' THEN 3\n")
                _T("       END as procedure_type\n")
                _T("  FROM pg_catalog.pg_proc pr\n")
                _T("       JOIN pg_catalog.pg_namespace ns ON ns.oid = pr.pronamespace\n")
                _T(" WHERE pr.prokind IN ('f','p','a')\n");
  // schema name
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND ns.nspname = ?\n");
  }
  // routine name
  if(!p_procedure.IsEmpty())
  {
    IdentifierCorrect(p_procedure);
    if(p_procedure.Find('%') >= 0)
    {
      sql += _T("   AND pr.proname LIKE ?\n");
    }
    else
    {
      sql += _T("   AND pr.proname = ?\n");
    }
  }
  // Sorting
  sql += _T(" ORDER BY 1,2,3");

  return sql;
}

XString
SQLInfoPostgreSQL::GetPSMProcedureAttributes(XString& p_schema,XString& p_procedure,bool p_quoted /*= false*/) const
{
  XString sql = _T("SELECT current_database() as procedure_catalog\n")
                _T("      ,ns.nspname         as procedure_schema\n")
                _T("      ,pr.proname         as procedure_name\n")
                _T("      ,pr.pronargs        as input_arguments\n")
                _T("      ,CASE pr.prokind\n")
                _T("	         WHEN 'f' THEN 1\n")
                _T("           WHEN 'p' THEN 0\n")
                _T("           WHEN 'a' THEN 1\n")
                _T("       END as output_arguments\n")
                _T("      ,0 as resultsets\n")
                _T("      ,co.description as remarks\n")
                _T("      ,CASE pr.prokind\n")
                _T("           WHEN 'f' THEN 2\n")
                _T("           WHEN 'p' THEN 1\n")
                _T("           WHEN 'a' THEN 3\n")
                _T("       END as procedure_type\n")
                _T("      ,pg_get_functiondef(pr.oid) as source\n")
                _T("  FROM pg_catalog.pg_proc pr\n")
                _T("       LEFT OUTER JOIN pg_catalog.pg_description co ON pr.oid = co.objoid\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("            INNER JOIN pg_catalog.pg_namespace   ns ON ns.oid = pr.pronamespace\n");
    sql += _T(" WHERE ns.nspname = ?\n");
  }
  if(!p_procedure.IsEmpty())
  {
    IdentifierCorrect(p_procedure);
    sql += p_schema.IsEmpty() ? _T(" WHERE ") : _T("   AND ");
    sql += _T("pr.proname ");
    // routine name
    sql += (p_procedure.Find('%') >= 0) ? _T("LIKE ?\n") : _T("= ?\n");
  }
  // Sorting
  sql += _T(" ORDER BY 1,2,3");

  return sql;
}

XString
SQLInfoPostgreSQL::GetPSMProcedureSourcecode(XString p_schema,XString p_procedure,bool p_quoted /*= false*/) const
{
  XString sql = _T("SELECT 1 as type\n")
                _T("      ,1 as line\n")
                _T("      ,pg_get_functiondef(pr.oid) as source\n")
                _T("  FROM pg_catalog.pg_proc pr\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("            INNER JOIN pg_catalog.pg_namespace ns ON ns.oid = pr.pronamespace\n");
    sql += _T(" WHERE ns.nspname = '") + p_schema + _T("'\n");
  }
  if(!p_procedure.IsEmpty())
  {
    IdentifierCorrect(p_procedure);
    sql += _T("   AND pr.proname = '") + p_procedure + _T("'");
  }
  return sql;
}

XString
SQLInfoPostgreSQL::GetPSMProcedureCreate(MetaProcedure& p_procedure) const
{
  // Source already gotten in GetCATALOGProcedureAttributes
  return p_procedure.m_source;
}

XString
SQLInfoPostgreSQL::GetPSMProcedureDrop(XString p_schema, XString p_procedure,bool p_function /*=false*/) const
{
  XString sql(_T("DROP "));
  sql += p_function ? _T("FUNCTION ") : _T("PROCEDURE ");
  if(!p_schema.IsEmpty())
  {
    sql += QIQ(p_schema) + _T(".");
  }
  sql += QIQ(p_procedure) + _T(" CASCADE");
  return sql;
}

XString
SQLInfoPostgreSQL::GetPSMProcedureErrors(XString /*p_schema*/,XString /*p_procedure*/,bool /*p_quoted = false*/) const
{
  return _T("");
}

XString
SQLInfoPostgreSQL::GetPSMProcedurePrivilege(XString& /*p_schema*/,XString& /*p_procedure*/,bool /*p_quoted = false*/) const
{
  return _T("");
}

// And it's parameters
XString
SQLInfoPostgreSQL::GetPSMProcedureParameters(XString& p_schema,XString& p_procedure,bool p_quoted /*= false*/) const
{
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
    IdentifierCorrect(p_schema);
    sql += _T("   AND fun.specific_schema = ?\n");
  }
  if(!p_procedure.IsEmpty())
  {
    IdentifierCorrect(p_procedure);
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
                                    ,XString /*p_domain*/    /*= ""*/
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
        XString resstring = result->GetAsString();
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
SQLInfoPostgreSQL::DoSQLCallNamedParameters(SQLQuery* /*p_query*/,XString& /*p_schema*/,XString& /*p_procedure*/,bool /*p_function = true*/)
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
