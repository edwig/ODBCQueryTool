////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoPostgreSQL.cpp
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
  return "PostgreSQL";
}

// Get the physical database name
XString
SQLInfoPostgreSQL::GetRDBMSPhysicalDatabaseName() const
{
  SQLQuery sql(m_database);
  XString query = "SELECT current_database()";
  sql.DoSQLStatement(query);
  if(sql.GetRecord())
  {
    return sql.GetColumn(1)->GetAsChar();
  }
  return "";
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

// KEYWORDS

// Keyword for the current date and time
XString
SQLInfoPostgreSQL::GetKEYWORDCurrentTimestamp() const
{
  return "current_timestamp(0)";
}

// String for the current date
XString
SQLInfoPostgreSQL::GetKEYWORDCurrentDate() const
{
  return "current_date";
}

// Get the concatenation operator
XString
SQLInfoPostgreSQL::GetKEYWORDConcatanationOperator() const
{
  return "||";
}

// Get quote character for strings
XString
SQLInfoPostgreSQL::GetKEYWORDQuoteCharacter() const
{
  return "\'";
}

// Get quote character around reserved words as an identifier
XString
SQLInfoPostgreSQL::GetKEYWORDReservedWordQuote() const
{
  return "\'";
}

// Get default NULL for parameter list input
XString
SQLInfoPostgreSQL::GetKEYWORDParameterDefaultNULL() const
{
  // Standard, no definition defines the NULL state
  return "";
}

// Parameter is for INPUT and OUTPUT in parameter list
XString
SQLInfoPostgreSQL::GetKEYWORDParameterINOUT() const
{
  return "";
}

// Parameter is for OUTPUT only in parameter list
XString
SQLInfoPostgreSQL::GetKEYWORDParameterOUT() const
{
  return "";
}

// Get datatype of the IDENTITY primary key in a Network database
XString
SQLInfoPostgreSQL::GetKEYWORDNetworkPrimaryKeyType() const
{
  // Use SEQUENCE to fill!
  return "INTEGER";
}

// Get datatype for timestamp (year to second)
XString
SQLInfoPostgreSQL::GetKEYWORDTypeTimestamp() const
{
  return "TIMESTAMP";
}

// Prefix for a parameter in a stored procedure
XString
SQLInfoPostgreSQL::GetKEYWORDParameterPrefix() const
{
  return "";
}

// Get select part to add new record identity to a table
// Can be special column like 'OID' or a sequence select
XString
SQLInfoPostgreSQL::GetKEYWORDIdentityString(XString& p_tablename,XString p_postfix /*= "_seq"*/) const
{
  return "nextval('" + p_tablename + p_postfix + "')";
}

// Gets the UPPER function
XString
SQLInfoPostgreSQL::GetKEYWORDUpper(XString& p_expression) const
{
  return "UPPER(" + p_expression + ")";
}

// Gets the construction for 1 minute ago
XString
SQLInfoPostgreSQL::GetKEYWORDInterval1MinuteAgo() const
{
  // Not supported by PostgreSQL
  return "ERROR";
}

// Gets the Not-NULL-Value statement of the database
XString
SQLInfoPostgreSQL::GetKEYWORDStatementNVL(XString& p_test,XString& p_isnull) const
{
  return "{fn IFNULL(" + p_test + "," + p_isnull + ")}";
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
  return "CURRENT_USER";
}

// Connects to a default schema in the database/instance
XString
SQLInfoPostgreSQL::GetSQLDefaultSchema(XString p_schema) const
{
  return "SET SEARCH_PATH TO " + p_schema;
}

// Gets the construction for inline generating a key within an INSERT statement
XString
SQLInfoPostgreSQL::GetSQLNewSerial(XString p_table, XString p_sequence) const
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
SQLInfoPostgreSQL::GetSQLGenerateSerial(XString p_table) const
{
  return "SELECT " + p_table + "_seq.nextval FROM DUAL";
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
  return XString("SAVEPOINT ") + p_savepointName;
}

XString
SQLInfoPostgreSQL::GetSQLCommitSubTransaction(XString p_savepointName) const
{
  // No commit for a sub transaction
  return XString("");
}

XString
SQLInfoPostgreSQL::GetSQLRollbackSubTransaction(XString p_savepointName) const
{
  return XString("ROLLBACK TO SAVEPOINT ") + p_savepointName;
}

// FROM-Part for a query to select only 1 (one) record / or empty!
XString
SQLInfoPostgreSQL::GetSQLFromDualClause() const
{
  // PostgreSQL knows the bare SELECT
  return "";
}

// Get SQL to lock  a table 
XString
SQLInfoPostgreSQL::GetSQLLockTable(XString p_schema, XString p_tablename, bool p_exclusive) const
{
  XString query = "LOCK TABLE " + p_schema + "." + p_tablename + " IN ";
  query += p_exclusive ? "EXCLUSIVE" : "SHARE";
  query += " MODE";
  return query;
}

// Get query to optimize the table statistics
XString
SQLInfoPostgreSQL::GetSQLOptimizeTable(XString p_schema, XString p_tablename) const
{
  XString optim = "VACUUM ANALYZE " + p_schema + "." + p_tablename;
  return optim;
}

// Transform query to select top <n> rows
XString
SQLInfoPostgreSQL::GetSQLTopNRows(XString p_sql,int p_top,int p_skip /*= 0*/) const
{
  if(p_top > 0)
  {
    p_sql.AppendFormat("\nSELECT LIMIT %d ",p_top);
    if(p_skip > 0)
    {
      p_sql.AppendFormat(" OFFSET %d",p_skip);
    }
  }
  return p_sql;
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
  s.Replace("'","''");
  XString kwoot = GetKEYWORDQuoteCharacter();
  return  kwoot + s + kwoot;
}

XString
SQLInfoPostgreSQL::GetSQLDateString(int p_year,int p_month,int p_day) const
{
  XString dateString;
  dateString.Format("TO_DATE('%04d-%02d-%02d','YYYY-MM-DD')",p_year,p_month,p_day);
  return dateString;
}

XString
SQLInfoPostgreSQL::GetSQLTimeString(int p_hour,int p_minute,int p_second) const
{
  XString timeString;
  timeString.Format("'%02d:%02d:%02d'",p_hour,p_minute,p_second);
  return timeString;
}

XString
SQLInfoPostgreSQL::GetSQLDateTimeString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  XString retval;
  retval.Format("to_timestamp('%04d-%02d-%02d %02d:%02d:%02d','YYYY-MM-DD HH24:MI:SS')::timestamp"
                ,p_year,p_month,p_day,p_hour,p_minute,p_second);
  return retval;
}

// Get date-time bound parameter string in database format
XString
SQLInfoPostgreSQL::GetSQLDateTimeBoundString() const
{
  return "to_timestamp(?,'YYYY-MM-DD HH24:MI:SS')::timestamp";
}

// Stripped data for the parameter binding
XString
SQLInfoPostgreSQL::GetSQLDateTimeStrippedString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  XString retval;
  retval.Format("%04d-%02d-%02d %02d:%02d:%02d"
                ,p_year,p_month,p_day,p_hour,p_minute,p_second);
  return retval;
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
  return "";
}

// Get SQL to check if a table already exists in the database
XString
SQLInfoPostgreSQL::GetCATALOGTableExists(XString& p_schema,XString& p_tablename) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  XString query = "SELECT count(*)\n"
                  "  FROM pg_class cl\n"
                  "      ,pg_namespace ns\n"
                  " WHERE cl.relnamespace = ns.oid\n"
                  "   AND ns.nspname = ?\n"
                  "   AND cl.relname = ?";
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
  XString query = "SELECT current_catalog as table_catalog\n"
                  "      ,sch.nspname     as table_schema\n"
                  "      ,tab.relname     as table_name\n"
                  "      ,'TABLE'         as object_type\n"
                  "      ,''              as remarks\n"
                  "      ,sch.nspname || '.' || tab.relname as fullname\n"
                  "      ,tsp.spcname     as tablespace\n"
                  "      ,CASE tab.relpersistence\n"
                  "            WHEN 'p' THEN 0\n"
                  "                     ELSE 1\n"
                  "       END as temporary\n"
                  "  FROM pg_catalog.pg_class tab\n"
                  "       left       join pg_catalog.pg_namespace  sch on tab.relnamespace  = sch.oid\n"
                  "       left outer join pg_catalog.pg_tablespace tsp ON tab.reltablespace = tsp.oid\n"
                  " WHERE tab.relkind = 'r'\n"
                  "   AND substring(sch.nspname,1,3) <> 'pg_'\n"
                  "   AND sch.nspname <> 'information_schema'\n";
  if(!p_schema.IsEmpty())
  {
    query += "    AND sch.name = ?\n";
  }
  if(!p_tablename.IsEmpty())
  {
    query += "   AND tab.relname ";
    query += p_tablename.Find('%') >= 0 ? "LIKE" : "=";
    query += " ?\n";
  }
  query += " ORDER BY 1,2,3";
                  
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
  XString query = "SELECT current_catalog as table_catalog\n"
                  "      ,sch.nspname     as table_schema\n"
                  "      ,tab.relname     as table_name\n"
                  "      ,'TABLE'         as object_type\n"
                  "      ,''              as remarks\n"
                  "      ,sch.nspname || '.' || tab.relname as fullname\n"
                  "      ,tsp.spcname     as tablespace\n"
                  "      ,CASE tab.relpersistence\n"
                  "            WHEN 'p' THEN 0\n"
                  "                     ELSE 1\n"
                  "       END as temporary\n"
                  "  FROM pg_catalog.pg_class tab\n"
                  "       left       join pg_catalog.pg_namespace  sch on tab.relnamespace  = sch.oid\n"
                  "       left outer join pg_catalog.pg_tablespace tsp ON tab.reltablespace = tsp.oid\n"
                  " WHERE tab.relkind IN ('t','v')\n"
                  "   AND (substring(sch.nspname,1,3) = 'pg_'\n"
                  "    OR sch.nspname = 'information_schema')\n"
                  "   AND sch.nspname <> 'pg_toast'\n";
  if(!p_schema.IsEmpty())
  {
    query += "    AND sch.name = ?\n";
  }
  if(!p_tablename.IsEmpty())
  {
    query += "   AND tab.relname ";
    query += p_tablename.Find('%') >= 0 ? "LIKE" : "=";
    query += " ?\n";
  }
  query += " ORDER BY 1,2,3";
                  
  return query;
}

XString
SQLInfoPostgreSQL::GetCATALOGTableCreate(MetaTable& p_table,MetaColumn& /*p_column*/) const
{
  XString sql = "CREATE ";
  if (p_table.m_temporary)
  {
    sql += "TEMPORARY ";
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
SQLInfoPostgreSQL::GetCATALOGTableCreatePostfix(MetaTable& /*p_table*/,MetaColumn& /*p_column*/) const
{
  return "";
}

XString
SQLInfoPostgreSQL::GetCATALOGTableRename(XString p_schema,XString p_tablename,XString p_newname) const
{
  // Beware: No 'TABLE' in the statement
  XString sql("RENAME " + p_schema + "." + p_tablename + " TO " + p_newname);
  return sql;
}

XString
SQLInfoPostgreSQL::GetCATALOGTableDrop(XString p_schema,XString p_tablename,bool p_ifExist /*= false*/,bool p_restrict /*= false*/,bool p_cascade /*= false*/) const
{
  XString sql("DROP TABLE ");
  if (p_ifExist)
  {
    sql += "IF EXISTS ";
  }
  if(!p_schema.IsEmpty())
  {
    sql += p_schema + ".";
  }
  sql += p_tablename;
  if (p_restrict)
  {
    sql += " RESTRICT";
  }
  else if (p_cascade)
  {
    sql += " CASCADE";
  }
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL TEMPORARY TABLE FUNCTIONS

XString 
SQLInfoPostgreSQL::GetCATALOGTemptableCreate(XString p_schema,XString p_tablename,XString p_select) const
{
  return "CREATE GLOBAL TEMPORARY TABLE " + p_schema + "." + p_tablename + " WITHOUT OIDS\nAS " + p_select;
}

XString 
SQLInfoPostgreSQL::GetCATALOGTemptableIntoTemp(XString p_schema,XString p_tablename,XString p_select) const
{
  return "INSERT INTO " + p_schema + "." + p_tablename + "\n" + p_select;
}

XString 
SQLInfoPostgreSQL::GetCATALOGTemptableDrop(XString p_schema,XString p_tablename) const
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
SQLInfoPostgreSQL::GetCATALOGColumnExists(XString p_schema,XString p_tablename,XString p_columnname) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  p_columnname.MakeLower();
  XString query = "SELECT count(*)\n"
                 "  FROM pg_class      tab\n"
                 "      ,pg_namespaces sch\n"
                 "      ,pg_attribute  att\n"
                 " WHERE tab.relname = '" + p_tablename  + "'\n"
                 "   AND sch.name    = '" + p_schema     + "'\n"
                 "   AND att.attname = '" + p_columnname + "'\n"
                 "   AND tab.oid     = att.attrelid\n"
                 "   AND sch.oid     = tab.relnamespace\n";
  return query;
}

XString 
SQLInfoPostgreSQL::GetCATALOGColumnList(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  return "";
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
  return "";
}

XString 
SQLInfoPostgreSQL::GetCATALOGColumnCreate(MetaColumn& p_column) const
{
  XString sql = "ALTER TABLE "  + p_column.m_schema + "." + p_column.m_table  + "\n"
                "  ADD COLUMN " + p_column.m_column + " " + p_column.m_typename;
  if(p_column.m_columnSize)
  {
    sql.AppendFormat("(%d",p_column.m_columnSize);
    if(p_column.m_decimalDigits)
    {
      sql.AppendFormat(",%d",p_column.m_decimalDigits);
    }
    sql += ")";
  }
  if(!p_column.m_nullable)
  {
    sql += " NOT NULL";
  }
  if(!p_column.m_default.IsEmpty())
  {
    sql += " DEFAULT ";
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
  XString sql("ALTER TABLE  " + p_column.m_schema + "." + p_column.m_table + "\n"
              "ALTER COLUMN " + p_column.m_column + " SET DATA TYPE " + p_column.m_typename);
  p_column.GetPrecisionAndScale(sql);

  sql += p_column.m_nullable ? " DROP " : " SET ";
  sql += "NOT NULL";
  return sql;
}

XString 
SQLInfoPostgreSQL::GetCATALOGColumnRename(XString p_schema,XString p_tablename,XString p_columnname,XString p_newname,XString p_datatype) const
{
  XString sqlCode = "ALTER TABLE " + p_schema + "." + p_tablename + "\n"
                    "  ADD " + p_newname + " " + p_datatype + ";\n";
  sqlCode        += "UPDATE " + p_schema + "." + p_tablename + "\n"
                    "   SET " + p_newname + " = " + p_columnname + ";\n";
  sqlCode        += "ALTER TABLE " + p_schema + "." + p_tablename + "\n"
                    " DROP COLUMN " + p_columnname + ";";
  return sqlCode;
}

XString
SQLInfoPostgreSQL::GetCATALOGColumnDrop(XString p_schema,XString p_tablename,XString p_columnname) const
{
  XString sql("ALTER TABLE " + p_schema + "." + p_tablename + "\n"
              " DROP COLUMN " + p_columnname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL INDICES FUNCTIONS

// All index functions
XString
SQLInfoPostgreSQL::GetCATALOGIndexExists(XString p_schema,XString p_tablename,XString p_indexname) const
{
  return "";
}

XString
SQLInfoPostgreSQL::GetCATALOGIndexList(XString& p_schema,XString& p_tablename) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();

  XString query =  "SELECT cli.relname\n"
                   "      ,att.attname\n"
                   "      ,att.attnum\n"
                   "      ,idx.indisunique\n"
                   "      ,0  as ascending\n"
                   "      ,'' as expression_source"
                   "  FROM pg_user sha\n"
                   "      ,pg_class clr\n"
                   "      ,pg_index idx\n"
                   "      ,pg_class cli\n"
                   "      ,pg_attribute att\n"
                   " WHERE sha.usename = ?\n"
                   "   AND clr.relname = ?\n"
                   "   AND sha.usesysid = clr.relowner\n"
                   "   AND clr.relkind = 'r'\n"
                   "   AND clr.oid = idx.indrelid\n"
                   "   AND idx.indexrelid = cli.oid\n"
                   "   AND cli.relkind = 'i'\n"
                   "   AND cli.oid = att.attrelid\n"
                   " ORDER BY cli.relname, att.attnum";
  return query;
}

XString
SQLInfoPostgreSQL::GetCATALOGIndexAttributes(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_indexname*/) const
{
  return "";
}

XString
SQLInfoPostgreSQL::GetCATALOGIndexCreate(MIndicesMap& p_indices) const
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
    query += index.m_columnName;
    if(index.m_ascending != "A")
    {
      query += " DESC";
    }
  }
  query += ")";
  return query;
}

XString
SQLInfoPostgreSQL::GetCATALOGIndexDrop(XString p_schema,XString /*p_tablename*/,XString p_indexname) const
{
  XString sql = "DROP INDEX " + p_schema + "." + p_indexname;
  return sql;
}

// Get extra filter expression for an index column
XString
SQLInfoPostgreSQL::GetCATALOGIndexFilter(MetaIndex& /*p_index*/) const
{
  return "";
}

//////////////////////////////////////////////////////////////////////////
// ALL PRIMARY KEY FUNCTIONS

XString
SQLInfoPostgreSQL::GetCATALOGPrimaryExists(XString p_schema,XString p_tablename) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();

  XString query = "SELECT COUNT(*)\n"
                  "  FROM pg_class      tab\n"
                  "      ,pg_constraint con\n"
                  "      ,pg_namespaces sch\n"
                  " WHERE tab.relnamespace = sch.oid\n"
                  "   AND tab.oid     = con.conrelid\n"
                  "   AND con.contype = 'p'\n"
                  "   AND sch.name    = '" + p_schema    + "'\n"
                  "   AND tab.relname = '" + p_tablename + "'";
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
  return "";
}

XString
SQLInfoPostgreSQL::GetCATALOGPrimaryCreate(MPrimaryMap& p_primaries) const
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
SQLInfoPostgreSQL::GetCATALOGPrimaryDrop(XString p_schema,XString p_tablename,XString p_constraintname) const
{
  XString sql("ALTER TABLE " + p_schema + "." + p_tablename + "\n"
              " DROP CONSTRAINT " + p_constraintname);
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
  sql.Format("SELECT COUNT(*)\n"
             "  FROM pg_constraint con\n"
             "      ,pg_class      cla\n"
             "      ,pg_namespace  sch\n"
             " WHERE con.contype      = 'f'\n"
             "   AND con.conrelid     = cla.oid\n"
             "   AND cla.relnamespace = sch.oid\n"
             "   AND sch.nspname      = '" + p_schema + "'\n"
             "   AND cla.relname      = '" + p_tablename + "'\n"
             "   AND con.conname      = '" + p_constraintname + "'"
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
    part.Format("SELECT current_database() as primary_catalog_name\n"
                "      ,psc.nspname        as primary_schema_name\n"
                "      ,pri.relname        as primary_table_name\n"
                "      ,current_database() as foreign_catalog_name\n"
                "      ,sch.nspname        as foreign_schema_name\n"
                "      ,cla.relname        as foreign_table_name\n"
                "      ,prc.conname        as primary_constraint_name\n"
                "      ,con.conname        as foreign_constraint_name\n"
                "      ,%d                 as key_sequence"
                "      ,pky.attname        as primary_key_column\n"
                "      ,att.attname        as foreign_key_column\n"
                "      ,case con.confupdtype   WHEN 'r' THEN 1\n"
                "                              WHEN 'c' THEN 0\n"
                "                              WHEN 'n' THEN 2\n"
                "                              WHEN 'd' THEN 4\n"
                "                              WHEN 'a' THEN 3\n"
                "                              ELSE  0\n"
                "                              END as update_rule\n"
                "      ,case con.confdeltype   WHEN 'r' THEN 1\n"
                "                              WHEN 'c' THEN 0\n"
                "                              WHEN 'n' THEN 2\n"
                "                              WHEN 'd' THEN 4\n"
                "                              WHEN 'a' THEN 3\n"
                "                              ELSE  0\n"
                "                              END as delete_rule\n"
                "      ,con.condeferrable as deferrable\n"
                "      ,case con.confmatchtype WHEN 's' THEN 1\n"
                "                              WHEN 'f' THEN 1\n"
                "                              ELSE  0\n"
                "                              END as match_option\n"
                "      ,con.condeferred   as initially_deferred\n"
                "      ,1                 as enabled\n"
                "  FROM pg_constraint con\n"
                "      ,pg_class      cla\n"
                "      ,pg_attribute  att\n"
                "      ,pg_namespace  sch\n"
                "      ,pg_class      pri\n"
                "      ,pg_attribute  pky\n"
                "      ,pg_namespace  psc\n"
                "      ,pg_constraint prc\n"
                " WHERE con.contype      = 'f'\n"
                "   AND con.conrelid     = cla.oid\n"
                "   and cla.relnamespace = sch.oid\n"
                "   and con.confrelid    = pri.oid\n"
                "   and pri.relnamespace = psc.oid\n"
                "   and prc.conrelid     = pri.oid\n"
                "   and prc.contype      = 'p'\n"
                "   and att.attrelid     = cla.oid\n"
                "   and att.attnum       = con.conkey[%d]\n"
                "   and pky.attrelid     = pri.oid\n"
                "   and pky.attnum       = con.confkey[%d]\n"
               ,ind
               ,ind
               ,ind);
    if(!p_schema.IsEmpty())
    {
      if(p_referenced)
      {
        part += "\n   AND psc.nspname = '" + p_schema + "'";
      }
      else
      {
        part += "\n   AND sch.nspname = '" + p_schema + "'";
      }
    }
    if(!p_tablename.IsEmpty())
    {
      if(p_referenced)
      {
        part += "\n   AND pri.relname = '" + p_tablename + "'";
      }
      else
      {
        part += "\n   AND cla.relname = '" + p_tablename + "'";
      }
    }
    if(!p_constraint.IsEmpty())
    {
      if(p_referenced)
      {
        part += "\n   AND prc.conname = '" + p_constraint + "'";
      }
      else
      {
        part += "\n   AND con.conname = '" + p_constraint + "'";
      }
    }

    // Append to query, multiple for multiple columns
    if(!query.IsEmpty())
    {
      query += "\nUNION ALL\n";
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
    default:                      break;
  }
  switch(foreign.m_match)
  {
    case SQL_MATCH_PARTIAL: query += "\n      MATCH PARTIAL"; break;
    case SQL_MATCH_SIMPLE:  query += "\n      MATCH SIMPLE";  break;
    case SQL_MATCH_FULL:    query += "\n      MATCH FULL";    break;
    default:                // In essence: MATCH FULL, but that's already the default
                            break;
  }
  switch(foreign.m_updateRule)
  {
    case SQL_CASCADE :    query += "\n      ON UPDATE CASCADE";     break;
    case SQL_SET_NULL:    query += "\n      ON UPDATE SET NULL";    break;
    case SQL_SET_DEFAULT: query += "\n      ON UPDATE SET DEFAULT"; break;
    case SQL_NO_ACTION:   query += "\n      ON UPDATE NO ACTION";   break;
    case SQL_RESTRICT:    query += "\n      ON UPDATE NO RESTRICT"; break;
    default:              // In essence: ON UPDATE RESTRICT, but that's already the default
                          break;
  }
  switch(foreign.m_deleteRule)
  {
    case SQL_CASCADE:     query += "\n      ON DELETE CASCADE";     break;
    case SQL_SET_NULL:    query += "\n      ON DELETE SET NULL";    break;
    case SQL_SET_DEFAULT: query += "\n      ON DELETE SET DEFAULT"; break;
    case SQL_NO_ACTION:   query += "\n      ON DELETE NO ACTION";   break;
    case SQL_RESTRICT:    query += "\n      ON DELETE NO RESTRICT"; break;
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
    return "";
  }

  MetaForeign& original  = p_original.front();
  MetaForeign& requested = p_requested.front();

  // Construct the correct tablename
  XString table(original.m_fkTableName);
  if(!original.m_fkSchemaName.IsEmpty())
  {
    table = original.m_fkSchemaName + "." + table;
  }

  // The base foreign key command
  XString query = "ALTER TABLE " + table + "\n"
                  "ALTER CONSTRAINT " + original.m_foreignConstraint + "\n";

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
  XString sql("ALTER TABLE " + p_schema + "." + p_tablename + "\n"
              " DROP CONSTRAINT " + p_constraintname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL TRIGGER FUNCTIONS

XString
SQLInfoPostgreSQL::GetCATALOGTriggerExists(XString p_schema, XString p_tablename, XString p_triggername) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  p_triggername.MakeLower();
  XString sql = "SELECT COUNT(*)\n"
                "  FROM information_schema.triggers\n"
                " WHERE event_object_schema = '" + p_schema + "'\n"
                "   AND event_object_table  = '" + p_tablename + "'\n"
                "   AND trigger_name        = '" + p_triggername + "'";
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
  sql = "SELECT event_object_catalog\n"
        "      ,event_object_schema\n"
        "      ,event_object_table\n"
        "      ,trigger_name\n"
        "      ,'' AS trigger_remarks\n"
        "      ,0  AS trigger_position\n"
        "      ,CASE action_timing\n"
        "            WHEN 'AFTER' THEN FALSE ELSE TRUE\n"
        "       END AS trigger_before\n"
        "      ,CASE event_manipulation\n"
        "            WHEN 'INSERT' THEN TRUE ELSE FALSE\n"
        "       END AS trigger_insert\n"
        "      ,CASE event_manipulation\n"
        "            WHEN 'UPDATE' THEN TRUE ELSE FALSE\n"
        "       END AS trigger_update\n"
        "      ,CASE event_manipulation\n"
        "            WHEN 'DELETE' THEN TRUE ELSE FALSE\n"
        "       END AS trigger_delete\n"
        "      ,CASE event_manipulation\n"
        "            WHEN 'SELECT' THEN TRUE ELSE FALSE\n"
        "       END AS trigger_select\n"
        "      ,FALSE AS trigger_session\n"
        "      ,FALSE AS trigger_transaction\n"
        "      ,FALSE AS trigger_rollback\n"
        "      ,''    AS trigger_referencing\n"
        "      ,TRUE  AS trigger_enabled\n"
        "      ,action_statement AS trigger_source\n"
        "  FROM information_schema.triggers\n";
  if(!p_schema.IsEmpty())
  {
    sql += " WHERE event_object_schema = ?\n";
  }
  if(!p_tablename.IsEmpty())
  {
    sql += p_schema.IsEmpty() ? " WHERE " : "   AND ";
    sql += "event_object_table = ?\n";
  }
  if(!p_triggername.IsEmpty())
  {
    sql += p_schema.IsEmpty() && p_tablename.IsEmpty() ? " WHERE " : "   AND ";
    sql += "trigger_name = ?\n";
  }
  sql += " ORDER BY 1,2,3,4";
  return sql;
}

XString
SQLInfoPostgreSQL::GetCATALOGTriggerCreate(MetaTrigger& /*p_trigger*/) const
{
  return "";
}

XString
SQLInfoPostgreSQL::GetCATALOGTriggerDrop(XString p_schema, XString p_tablename, XString p_triggername) const
{
  return "";
}

//////////////////////////////////////////////////////////////////////////
// ALL SEQUENCE FUNCTIONS

XString
SQLInfoPostgreSQL::GetCATALOGSequenceExists(XString p_schema, XString p_sequence) const
{
  p_schema.MakeLower();
  p_sequence.MakeLower();

  XString sql("SELECT COUNT(*)\n"
              "  FROM information_schema.sequences\n"
              " WHERE sequence_schema = '" + p_schema   + "'\n"
              "   AND sequence_name   = '" + p_sequence + "'");
  return sql;
}

XString
SQLInfoPostgreSQL::GetCATALOGSequenceList(XString& p_schema,XString& p_pattern) const
{
  p_schema.MakeLower();
  p_pattern.MakeLower();
  if(!p_pattern.IsEmpty() && p_pattern.Compare("%"))
  {
    p_pattern = "%" + p_pattern + "%";
  }

  XString sql = "SELECT ''              AS catalog_name\n"
                "      ,sequence_schema AS schema_name\n"
                "      ,sequence_name\n"
                "      ,start_value     AS current_value\n"
                "      ,0               AS minimal_value\n"
                "      ,increment\n"
                "      ,0               AS cache\n"
                "      ,case cycle_option\n"
                "            when 'NO' then 1\n"
                "                      else 0\n"
                "       end             AS cycle\n"
                "      ,0               AS ordering\n"
                "  FROM information_schema.sequences\n";
  if(!p_schema.IsEmpty())
  {
    sql += " WHERE sequence_schema  = ?\n";
  }
  if(!p_pattern.IsEmpty())
  {
    sql += p_schema.IsEmpty() ? " WHERE " : "   AND ";
    sql += "sequence_name LIKE ?";
  }
  return sql;
}

XString
SQLInfoPostgreSQL::GetCATALOGSequenceAttributes(XString& p_schema,XString& p_sequence) const
{
  p_schema.MakeLower();
  p_sequence.MakeLower();

  XString sql = "SELECT ''              AS catalog_name\n"
                "      ,sequence_schema AS schema_name\n"
                "      ,sequence_name\n"
                "      ,start_value     AS current_value\n"
                "      ,0               AS minimal_value\n"
                "      ,increment\n"
                "      ,0               AS cache\n"
                "      ,case cycle_option\n"
                "            when 'NO' then 1\n"
                "                      else 0\n"
                "       end             AS cycle\n"
                "      ,0               AS ordering\n"
                "  FROM information_schema.sequences\n";
  if(!p_schema.IsEmpty())
  {
    sql += " WHERE sequence_schema = ?\n";
  }
  if(!p_sequence.IsEmpty())
  {
    sql += p_schema.IsEmpty() ? " WHERE " : "   AND ";
    sql += "sequence_name   = ?";
  }
  return sql;
}

XString
SQLInfoPostgreSQL::GetCATALOGSequenceCreate(MetaSequence& p_sequence) const
{
  XString sql("CREATE SEQUENCE ");

  if (!p_sequence.m_schemaName.IsEmpty())
  {
    sql += p_sequence.m_schemaName + ".";
  }
  sql += p_sequence.m_sequenceName;
  sql.AppendFormat(" START WITH %-12.0f", p_sequence.m_currentValue);
  if(p_sequence.m_cache)
  {
    sql.AppendFormat(" CACHE %d",p_sequence.m_cache);
  }
  sql += p_sequence.m_cycle ? " CYCLE" : " NO CYCLE";
  return sql;
}

XString
SQLInfoPostgreSQL::GetCATALOGSequenceDrop(XString p_schema, XString p_sequence) const
{
  XString sql("DROP SEQUENCE " + p_schema + "." + p_sequence);
  return  sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL VIEW FUNCTIONS

XString 
SQLInfoPostgreSQL::GetCATALOGViewExists(XString& p_schema,XString& p_viewname) const
{
  p_schema.Empty();  // do not bind as parameter
  p_viewname.MakeLower();
  XString sql("SELECT COUNT(*)\n"
              "  FROM pg_views\n" 
              " WHERE view_name = ?");
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
  XString query = "SELECT current_catalog as table_catalog\n"
                  "      ,sch.nspname     as table_schema\n"
                  "      ,tab.relname     as table_name\n"
                  "      ,'TABLE'         as object_type\n"
                  "      ,''              as remarks\n"
                  "      ,sch.nspname || '.' || tab.relname as fullname\n"
                  "      ,''              as tablespace\n"
                  "      ,CASE tab.relpersistence\n"
                  "            WHEN 'p' THEN 0\n"
                  "                     ELSE 1\n"
                  "       END as temporary\n"
                  "  FROM pg_catalog.pg_class tab\n"
                  "       left join pg_catalog.pg_namespace sch on tab.relnamespace = sch.oid\n"
                  " WHERE tab.relkind = 'v'\n"
                  "   AND substring(sch.nspname,1,3) <> 'pg_'\n"
                  "   AND sch.nspname <> 'information_schema'\n";
  if(!p_schema.IsEmpty())
  {
    query += "    AND sch.name = ?\n";
  }
  if(!p_viewname.IsEmpty())
  {
    query += "   AND tab.relname ";
    query += p_viewname.Find('%') >= 0 ? "LIKE" : "=";
    query += " ?\n";
  }
  query += " ORDER BY 1,2,3";
                  
  return query;
}

XString
SQLInfoPostgreSQL::GetCATALOGViewText(XString& /*p_schema*/,XString& /*p_viewname*/) const
{
  // Cannot query this, Use ODBC functions
  return "";
}

XString
SQLInfoPostgreSQL::GetCATALOGViewCreate(XString p_schema,XString p_viewname,XString p_contents) const
{
  return "CREATE OR REPLACE VIEW " + p_schema + "." + p_viewname + "\n" + p_contents;
}

XString 
SQLInfoPostgreSQL::GetCATALOGViewRename(XString p_schema,XString p_viewname,XString p_newname)    const
{
  return "";
}

XString 
SQLInfoPostgreSQL::GetCATALOGViewDrop(XString p_schema,XString p_viewname,XString& p_precursor) const
{
  p_precursor.Empty();
  return "DROP VIEW " + p_schema + "." + p_viewname;
}

// All Privilege functions
XString
SQLInfoPostgreSQL::GetCATALOGTablePrivileges(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  return "";
}

XString 
SQLInfoPostgreSQL::GetCATALOGColumnPrivileges(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_columnname*/) const
{
  return "";
}

XString 
SQLInfoPostgreSQL::GetCatalogGrantPrivilege(XString p_schema,XString p_objectname,XString p_privilege,XString p_grantee,bool p_grantable)
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
SQLInfoPostgreSQL::GetCatalogRevokePrivilege(XString p_schema,XString p_objectname,XString p_privilege,XString p_grantee)
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
SQLInfoPostgreSQL::GetPSMProcedureExists(XString p_schema, XString p_procedure) const
{
  p_schema.MakeLower();
  p_procedure.MakeLower();
  return "SELECT count(*)\n"
         "  FROM pg_proc\n"
         " WHERE proname = '" + p_procedure + "'\n;";
}

XString
SQLInfoPostgreSQL::GetPSMProcedureList(XString& /*p_schema*/) const
{
  return "";
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
  return "";
}

XString
SQLInfoPostgreSQL::GetPSMProcedureSourcecode(XString p_schema, XString p_procedure) const
{
  return "";
}

XString
SQLInfoPostgreSQL::GetPSMProcedureCreate(MetaProcedure& /*p_procedure*/) const
{
  return "";
}

XString
SQLInfoPostgreSQL::GetPSMProcedureDrop(XString p_schema, XString p_procedure) const
{
  return "";
}

XString
SQLInfoPostgreSQL::GetPSMProcedureErrors(XString p_schema,XString p_procedure) const
{
  return "";
}

// And it's parameters
XString
SQLInfoPostgreSQL::GetPSMProcedureParameters(XString& p_schema,XString& p_procedure) const
{
  p_schema.MakeLower();
  p_procedure.MakeLower();
  XString sql;

  sql = "SELECT par.specific_catalog\n"
        "      ,par.specific_schema\n"
        "      ,fun.routine_name\n"
        "      ,par.parameter_name\n"
        "      ,case par.parameter_mode\n"
        "            when 'IN'    then 1\n"
        "            when 'OUT'   then 4\n"
        "            when 'INOUT' then 2\n"
        "       end as columntype\n"
        "      ,1 as datatype\n"
        "      ,par.data_type as typename\n"
        "      ,par.character_maximum_length\n"
        "      ,par.numeric_precision\n"
        "      ,par.numeric_scale\n"
        "      ,par.numeric_precision_radix\n"
        "      ,1 as is_nullable\n"
        "      ,'' as remarks\n"
        "      ,par.parameter_default\n"
        "      ,1 as datatype3\n"
        "      ,par.interval_precision as subtype\n"
        "      ,par.character_octet_length\n"
        "      ,par.ordinal_position\n"
        "      ,'YES' as isNullable\n"
        "  FROM information_schema.parameters par\n"
        "      ,information_schema.routines fun\n"
        " WHERE par.specific_catalog = fun.specific_catalog\n"
        "   AND par.specific_schema  = fun.specific_schema\n"
        "   AND par.specific_name    = fun.specific_name\n";
  if(!p_schema.IsEmpty())
  {
    sql += "   AND fun.specific_schema = ?\n";
  }
  if(!p_procedure.IsEmpty())
  {
    sql += "   AND fun.routine_name    = ?\n";
  }
  sql += " ORDER BY 1,2,3,18";

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
  line.Format("%s ",p_variable.GetString());

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
  else if(!p_asColumn)
  {
    line += p_asColumn + "%TYPE";
  }
  line += ";\n";
  return line;
}

XString
SQLInfoPostgreSQL::GetPSMAssignment(XString p_variable,XString p_statement /*=""*/) const
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
SQLInfoPostgreSQL::GetPSMIF(XString p_condition) const
{
  XString line("IF (");
  line += p_condition;
  line += ") THEN\n";
  return line;
}

XString
SQLInfoPostgreSQL::GetPSMIFElse() const
{
  return "ELSE\n";
}

XString
SQLInfoPostgreSQL::GetPSMIFEnd() const
{
  return "END IF;\n";
}

XString
SQLInfoPostgreSQL::GetPSMWhile(XString p_condition) const
{
  return "WHILE (" + p_condition + ") LOOP\n";
}

XString
SQLInfoPostgreSQL::GetPSMWhileEnd() const
{
  return "END LOOP;\n";
}

XString
SQLInfoPostgreSQL::GetPSMLOOP() const
{
  return "LOOP\n";
}

XString
SQLInfoPostgreSQL::GetPSMLOOPEnd() const
{
  return "END LOOP;\n";
}

XString
SQLInfoPostgreSQL::GetPSMBREAK() const
{
  return "EXIT;\n";
}

XString
SQLInfoPostgreSQL::GetPSMRETURN(XString p_statement /*= ""*/) const
{
  XString line("RETURN");
  if(!p_statement.IsEmpty())
  {
    line += " " + p_statement;
  }
  line += ";\n";
  return line;
}

XString
SQLInfoPostgreSQL::GetPSMExecute(XString p_procedure,MParameterMap& p_parameters) const
{
  XString line;
  line.Format("SELECT %s(",p_procedure.GetString());
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
SQLInfoPostgreSQL::GetPSMCursorDeclaration(XString p_cursorname,XString p_select) const
{
  return "DECLARE " + p_cursorname + " CURSOR FOR " + p_select + ";";
}

XString
SQLInfoPostgreSQL::GetPSMCursorFetch(XString p_cursorname,std::vector<XString>& /*p_columnnames*/,std::vector<XString>& p_variablenames) const
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
SQLInfoPostgreSQL::GetPSMExceptionCatchNoData() const
{
  return "EXCEPTION WHEN 100 THEN\n";
  // Followed by block and 'END;'
}

XString
SQLInfoPostgreSQL::GetPSMExceptionCatch(XString p_sqlState) const
{
  return "EXCEPTION WHEN " + p_sqlState + " THEN\n";
  // Followed by block and 'END;'
}

XString
SQLInfoPostgreSQL::GetPSMExceptionRaise(XString p_sqlState) const
{
  return "RAISE EXCEPTION SQLSTATE " + p_sqlState + ";\n";
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
  XString query = "SELECT procpid\n"
                  "      ,user\n"
                  "      ,datname\n"
                  "      ,current_timestamp\n"  // moment
                  "      ,''\n"         // remote address
                  "      ,''\n"         // remote process name
                  "      ,''\n"         // remote process ID
                  "  FROM pg_stat_activity\n"
                  " WHERE procpid = pg_backend_pid()";    
  return query;
}

XString
SQLInfoPostgreSQL::GetSESSIONExists(XString p_sessionID) const
{
  XString sql = "SELECT COUNT(*)\n"
                "  FROM pg_stat_activity\n"
                " WHERE procpid = " + p_sessionID;
  return sql;
}

XString
SQLInfoPostgreSQL::GetSESSIONList() const
{
  return GetSESSIONAttributes("");
}

XString
SQLInfoPostgreSQL::GetSESSIONAttributes(XString p_sessionID) const
{
  XString sql = "SELECT procpid\n"
                "      ,user\n"
                "      ,datname\n"
                "      ,current_timestamp\n"  // moment
                "      ,''\n"         // remote address
                "      ,''\n"         // remote process name
                "      ,''\n"         // remote process ID
                "  FROM pg_stat_activity";
  if (!p_sessionID.IsEmpty())
  {
    sql += "\n WHERE procpid = " + p_sessionID;
  }
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// Transactions

XString
SQLInfoPostgreSQL::GetSESSIONConstraintsDeferred() const
{
  return "set transaction deferrable";
}

XString
SQLInfoPostgreSQL::GetSESSIONConstraintsImmediate() const
{
  return "set transaction not deferrable";
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
        SQLVariant* target = p_query->GetParameter(++setIndex);
        if(target == nullptr)
        {
          throw StdException("Wrong number of output parameters for procedure call");
        }
        type = target->GetParameterType();
        dataType = target->GetDataType();
      }
      while(type != SQL_PARAM_OUTPUT && type != SQL_PARAM_INPUT_OUTPUT);

      // Getting the next result from the result set
      SQLVariant* result = query.GetColumn(resIndex);
      if(result->GetDataType() == SQL_C_CHAR)
      {
        const char* resPointer = result->GetAsChar();
        if(resPointer && *resPointer == '(' && numReturn)
        {
          var = GetVarFromRecord(dataType,(char*)resPointer,recIndex++,ready);
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
                                               ,XString&  p_procedure)
{
  // Start with select form
  XString sql = "SELECT ";
  if(!p_schema.IsEmpty())
  {
    sql += p_schema;
    sql += ".";
  }
  sql += p_procedure;

  // Opening parenthesis
  sql += "(";

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
      if(ind > 1) sql += ",";
      sql += "?";

      // Add the parameter with the result counter!
      p_thecall->SetParameter(res++,parameter);
    }
    // Next parameter
    ++ind;
  }

  // CLosing parenthesis
  sql += ")";

  // The procedure IS the singular object
  // Procedure **MUST** end with "SUSPEND;" 
  return sql;
}

// Get column from PostgreSQL result set
// (123,"string",21.12)
SQLVariant
SQLInfoPostgreSQL::GetVarFromRecord(int p_type,char* p_pointer,int p_column,bool& p_ready)
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
    bool isString = p_pointer[beginPos] == '\"';
    if(isString) ++beginPos;
    endPos = beginPos;

    // Find end of the field
    while(p_pointer[endPos] && p_pointer[endPos] != ','
                            && p_pointer[endPos] != ')' 
                            && p_pointer[endPos] != '\"')
    {
      ++endPos;
    }

    // If column found
    if(p_column == curColumn)
    {
      char temp = p_pointer[endPos];
      p_pointer[endPos] = 0;
      variant.SetData(p_type,&p_pointer[beginPos]);
      p_pointer[endPos] = temp;
      break;
    }

    // Next column
    beginPos = endPos;
    if(isString && p_pointer[beginPos] == '\"') ++beginPos;
    if(p_pointer[beginPos] == ',' || p_pointer[beginPos] == ')') ++beginPos;
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
