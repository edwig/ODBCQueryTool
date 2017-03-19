////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoPostgreSQL.cpp
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
CString
SQLInfoPostgreSQL::GetRDBMSVendorName() const
{
  // The name of the database vendor
  return "PostgreSQL";
}

// Get the physical database name
CString
SQLInfoPostgreSQL::GetRDBMSPhysicalDatabaseName() const
{
  SQLQuery sql(m_database);
  CString query = "SELECT current_database()";
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

// KEYWORDS

// Keyword for the current date and time
CString
SQLInfoPostgreSQL::GetKEYWORDCurrentTimestamp() const
{
  return "current_timestamp(0)";
}

// String for the current date
CString
SQLInfoPostgreSQL::GetKEYWORDCurrentDate() const
{
  return "current_date";
}

// Get the concatenation operator
CString
SQLInfoPostgreSQL::GetKEYWORDConcatanationOperator() const
{
  return "||";
}

// Get quote character for strings
CString
SQLInfoPostgreSQL::GetKEYWORDQuoteCharacter() const
{
  return "\'";
}

// Get default NULL for parameter list input
CString
SQLInfoPostgreSQL::GetKEYWORDParameterDefaultNULL() const
{
  // Standard, no definition defines the NULL state
  return "";
}

// Parameter is for INPUT and OUTPUT in parameter list
CString
SQLInfoPostgreSQL::GetKEYWORDParameterINOUT() const
{
  return "";
}

// Parameter is for OUTPUT only in parameter list
CString
SQLInfoPostgreSQL::GetKEYWORDParameterOUT() const
{
  return "";
}

// Get datatype of the IDENTITY primary key in a Network database
CString
SQLInfoPostgreSQL::GetKEYWORDNetworkPrimaryKeyType() const
{
  // Use SEQUENCE to fill!
  return "INTEGER";
}

// Get datatype for timestamp (year to second)
CString
SQLInfoPostgreSQL::GetKEYWORDTypeTimestamp() const
{
  return "TIMESTAMP";
}

// Prefix for a parameter in a stored procedure
CString
SQLInfoPostgreSQL::GetKEYWORDParameterPrefix() const
{
  return "";
}

// Get select part to add new record identity to a table
// Can be special column like 'OID' or a sequence select
CString
SQLInfoPostgreSQL::GetKEYWORDIdentityString(CString& p_tablename,CString p_postfix /*= "_seq"*/) const
{
  return "nextval('" + p_tablename + p_postfix + "')";
}

// Gets the UPPER function
CString
SQLInfoPostgreSQL::GetKEYWORDUpper(CString& p_expression) const
{
  return "UPPER(" + p_expression + ")";
}

// Gets the construction for 1 minute ago
CString
SQLInfoPostgreSQL::GetKEYWORDInterval1MinuteAgo() const
{
  // Not supported by PostgreSQL
  return "ERROR";
}

// Gets the Not-NULL-Value statement of the database
CString
SQLInfoPostgreSQL::GetKEYWORDStatementNVL(CString& p_test,CString& p_isnull) const
{
  return "{fn IFNULL(" + p_test + "," + p_isnull + ")}";
}

// Gets the construction / select for generating a new serial identity
CString
SQLInfoPostgreSQL::GetSQLGenerateSerial(CString p_table) const
{
  return "SELECT " + p_table + "_seq.nextval FROM DUAL";
}

// Gets the construction / select for the resulting effective generated serial
CString
SQLInfoPostgreSQL::GetSQLEffectiveSerial(CString p_identity) const
{
  // Already the correct value
  return p_identity;
}

// Gets the sub transaction commands
CString
SQLInfoPostgreSQL::GetSQLStartSubTransaction(CString p_savepointName) const
{
  return CString("SAVEPOINT ") + p_savepointName;
}

CString
SQLInfoPostgreSQL::GetSQLCommitSubTransaction(CString p_savepointName) const
{
  // No commit for a sub transaction
  return CString("");
}

CString
SQLInfoPostgreSQL::GetSQLRollbackSubTransaction(CString p_savepointName) const
{
  return CString("ROLLBACK TO SAVEPOINT ") + p_savepointName;
}

// FROM-Part for a query to select only 1 (one) record / or empty!
CString
SQLInfoPostgreSQL::GetSQLFromDualClause() const
{
  // PostgreSQL knows the bare SELECT
  return "";
}

// Get SQL to lock  a table 
CString
SQLInfoPostgreSQL::GetSQLLockTable(CString p_schema, CString p_tablename, bool p_exclusive) const
{
  CString query = "LOCK TABLE " + p_schema + "." + p_tablename + " IN ";
  query += p_exclusive ? "EXCLUSIVE" : "SHARE";
  query += " MODE";
  return query;
}

// Get query to optimize the table statistics
CString
SQLInfoPostgreSQL::GetSQLOptimizeTable(CString p_schema, CString p_tablename) const
{
  CString optim = "VACUUM ANALYZE " + p_schema + "." + p_tablename;
  return optim;
}

//////////////////////////////////////////////////////////////////////////
//
// SQL STRINGS
//
//////////////////////////////////////////////////////////////////////////

CString
SQLInfoPostgreSQL::GetSQLString(const CString& p_string) const
{
  CString s = p_string;
  s.Replace("'","''");
  CString kwoot = GetKEYWORDQuoteCharacter();
  return  kwoot + s + kwoot;
}

CString
SQLInfoPostgreSQL::GetSQLDateString(int p_year,int p_month,int p_day) const
{
  CString dateString;
  dateString.Format("TO_DATE('%04d-%02d-%02d','YYYY-MM-DD')",p_year,p_month,p_day);
  return dateString;
}

CString
SQLInfoPostgreSQL::GetSQLTimeString(int p_hour,int p_minute,int p_second) const
{
  CString timeString;
  timeString.Format("'%02d:%02d:%02d'",p_hour,p_minute,p_second);
  return timeString;
}

CString
SQLInfoPostgreSQL::GetSQLDateTimeString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  CString retval;
  retval.Format("to_timestamp('%04d-%02d-%02d %02d:%02d:%02d','YYYY-MM-DD HH24:MI:SS')::timestamp"
                ,p_year,p_month,p_day,p_hour,p_minute,p_second);
  return retval;
}

// Get date-time bound parameter string in database format
CString
SQLInfoPostgreSQL::GetSQLDateTimeBoundString() const
{
  return "to_timestamp(?,'YYYY-MM-DD HH24:MI:SS')::timestamp";
}

// Stripped data for the parameter binding
CString
SQLInfoPostgreSQL::GetSQLDateTimeStrippedString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  CString retval;
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

// Get SQL to check if a table already exists in the database
CString
SQLInfoPostgreSQL::GetCATALOGTableExists(CString p_schema,CString p_tablename) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  CString query = "SELECT count(*)\n"
                  "  FROM pg_class cl\n"
                  "      ,pg_namespace ns\n"
                  " WHERE cl.relnamespace = ns.oid\n"
                  "   AND ns.nspname = '" + p_schema    + "'\n"
                  "   AND cl.relname = '" + p_tablename + "'";
  return query;
}

CString
SQLInfoPostgreSQL::GetCATALOGTablesList(CString p_schema,CString p_pattern) const
{
  p_schema.MakeLower();
  p_pattern.MakeLower();
  CString query = "SELECT count(*)\n"
                  "  FROM pg_class cl\n"
                  "      ,pg_namespace ns\n"
                  " WHERE cl.relnamespace = ns.oid\n"
                  "   AND ns.nspname = '" + p_schema    + "'\n"
                  "   AND cl.relname like '" + p_pattern + "'";
  return query;
}

bool
SQLInfoPostgreSQL::GetCATALOGTableAttributes(CString /*p_schema*/,CString /*p_tablename*/,MetaTable& /*p_table*/) const
{
  // Getting the temp table status
  return false;
}

CString
SQLInfoPostgreSQL::GetCATALOGTableCreate(MetaTable& /*p_table*/,MetaColumn& /*p_column*/) const
{
  return "";
}

CString
SQLInfoPostgreSQL::GetCATALOGTableRename(CString p_schema,CString p_tablename,CString p_newname) const
{
  // Beware: No 'TABLE' in the statement
  CString sql("RENAME " + p_schema + "." + p_tablename + " TO " + p_newname);
  return sql;
}

CString
SQLInfoPostgreSQL::GetCATALOGTableDrop(CString p_schema,CString p_tablename) const
{
  CString sql("DROP TABLE ");
  if(!p_schema.IsEmpty())
  {
    sql += p_schema + ".";
  }
  sql += p_tablename;
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL TEMPORARY TABLE FUNCTIONS

CString 
SQLInfoPostgreSQL::GetCATALOGTemptableCreate(CString p_schema,CString p_tablename,CString p_select) const
{
  return "CREATE GLOBAL TEMPORARY TABLE " + p_schema + "." + p_tablename + " WITHOUT OIDS\nAS " + p_select;
}

CString 
SQLInfoPostgreSQL::GetCATALOGTemptableIntoTemp(CString p_schema,CString p_tablename,CString p_select) const
{
  return "INSERT INTO " + p_schema + "." + p_tablename + "\n" + p_select;
}

CString 
SQLInfoPostgreSQL::GetCATALOGTemptableDrop(CString p_schema,CString p_tablename) const
{
  CString tablename = p_schema + "." + p_tablename;
  return "DELETE FROM "    + tablename + ";\n"
         "<@>\n"
         "TRUNCATE TABLE " + tablename + ";\n"
         "<@>\n"
         "DROP TABLE "     + tablename + ";\n";
}

//////////////////////////////////////////////////////////////////////////
// ALL COLUMN FUNCTIONS

CString 
SQLInfoPostgreSQL::GetCATALOGColumnExists(CString p_schema,CString p_tablename,CString p_columnname) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  p_columnname.MakeLower();
  CString query = "SELECT count(*)\n"
                 "  FROM pg_class      tab\n"
                 "      ,pg_namespaces sch\n"
                 "      ,pg_attribute  att\n"
                 " WHERE tab.relname = '" + p_tablename  + "'\n"
                 "   AND sch.name    = '" + p_schema     + "'\n";
                 "   AND att.attname = '" + p_columnname + "'\n"
                 "   AND tab.oid     = att.attrelid\n"
                 "   AND sch.oid     = tab.relnamespace\n";
  return query;
}

CString 
SQLInfoPostgreSQL::GetCATALOGColumnList(CString p_schema,CString p_tablename) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();

  CString query =  "SELECT att.attname\n"
                   "      ,att.attnum\n"
                   "      ,typname\n"
                   "      ,typlen\n"
                   "      ,attnotnull\n"
                   "      ,(atttypmod - 4) / 65536\n"
                   "      ,(atttypmod - 4) % 65536\n"
                   "  FROM pg_class      tab\n"
                   "      ,pg_namespaces sch\n"
                   "      ,pg_attribute  att\n"
                   "      ,pg_type       typ\n"
                   "      ,pg_attrdef    def\n"
                   " WHERE tab.relname  = '" + p_tablename + "'\n"
                   "   AND sch.name     = '" + p_schema + "'\n"
                   "   AND sch.oid      = tab.relnamespace\n"
                   "   AND tab.relkind  = 'r'\n"
                   "   AND tab.oid      = att.attrelid\n"
                   "   AND att.atttypid = pg_type.oid\n"
                   "   AND def.adrelid  = att.attrelid\n"
                   "   AND def.adnum    = att.attnum\n"
                   "   AND attnum > 0\n"
                   "   AND typlen < 0\n"
                   "   AND typname = 'numeric'\n"
                   "UNION\n"
                   "SELECT attname\n"
                   "      ,attnum\n"
                   "      ,typname\n"
                   "      ,(atttypmod - 4) AS typlen\n"
                   "      ,attnotnull\n"
                   "      ,0\n"
                   "      ,0\n"
                   "  FROM pg_class      tab\n"
                   "      ,pg_namespaces sch\n"
                   "      ,pg_attribute  att\n"
                   "      ,pg_type       typ\n"
                   "      ,pg_attrdef    def\n"
                   " WHERE tab.relname  = '" + p_tablename + "'\n"
                   "   AND sch.name     = '" + p_schema + "'\n"
                   "   AND sch.oid      = tab.relnamespace\n"
                   "   AND tab.relkind  = 'r'\n"
                   "   AND tab.oid      = att.attrelid\n"
                   "   AND att.atttypid = typ.oid\n"
                   "   AND def.adrelid  = att.attrelid\n"
                   "   AND def.adnum    = att.attnum\n"
                   "   AND attnum > 0\n"
                   "   AND typlen < 0\n"
                   "   AND typname = 'varchar'\n"
                   "UNION\n"
                   "SELECT attname\n"
                   "      ,attnum\n"
                   "      ,typname\n"
                   "      ,typlen\n"
                   "      ,attnotnull\n"
                   "      ,0\n"
                   "      ,0\n"
                   "  FROM pg_class      tab\n"
                   "      ,pg_namespaces sch\n"
                   "      ,pg_attribute  att\n"
                   "      ,pg_type       typ\n"
                   "      ,pg_attrdef    def\n"
                   " WHERE tab.relname  = '" + p_tablename + "'\n"
                   "   AND sch.name     = '" + p_schema + "'\n"
                   "   AND sch.oid      = tab.relnamespace\n"
                   "   AND tab.relkind  = 'r'\n"
                   "   AND tab.oid      = att.attrelid\n"
                   "   AND att.atttypid = typ.oid\n"
                   "   AND def.adrelid  = att.attrelid\n"
                   "   AND def.adnum    = att.attnum\n"
                   "   AND attnum > 0\n"
                   "   AND typlen > 0\n"
                   "   AND typname not in ('numeric', 'varchar')\n"
                   " ORDER BY attnum";
  return query;
}

CString 
SQLInfoPostgreSQL::GetCATALOGColumnAttributes(CString p_schema,CString p_tablename,CString p_columname) const
{
  return "";
}

CString 
SQLInfoPostgreSQL::GetCATALOGColumnCreate(MetaColumn& p_column) const
{
  CString sql = "ALTER TABLE "  + p_column.m_schema + "." + p_column.m_table  + "\n";
                "  ADD COLUMN " + p_column.m_column + " " + p_column.m_typename;
  if(p_column.m_precision)
  {
    sql.AppendFormat("(%d",p_column.m_precision);
    if(p_column.m_scale)
    {
      sql.AppendFormat(",%d",p_column.m_scale);
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

CString 
SQLInfoPostgreSQL::GetCATALOGColumnAlter(MetaColumn& p_column) const
{
  CString sql("ALTER TABLE  " + p_column.m_schema + "." + p_column.m_table + "\n"
              "ALTER COLUMN " + p_column.m_column + " SET DATA TYPE " + p_column.m_typename);
  p_column.GetPrecisionAndScale(sql);

  sql += p_column.m_nullable ? " DROP " : " SET ";
  sql += "NOT NULL";
  return sql;
}

CString 
SQLInfoPostgreSQL::GetCATALOGColumnRename(CString p_schema,CString p_tablename,CString p_columnname,CString p_newname,CString p_datatype) const
{
  CString sqlCode = "ALTER TABLE " + p_schema + "." + p_tablename + "\n"
                    "  ADD " + p_newname + " " + p_datatype + ";\n";
  sqlCode        += "UPDATE " + p_schema + "." + p_tablename + "\n"
                    "   SET " + p_newname + " = " + p_columnname + ";\n";
  sqlCode        += "ALTER TABLE " + p_schema + "." + p_tablename + "\n"
                    " DROP COLUMN " + p_columnname + ";";
  return sqlCode;
}

CString
SQLInfoPostgreSQL::GetCATALOGColumnDrop(CString p_schema,CString p_tablename,CString p_columnname) const
{
  CString sql("ALTER TABLE " + p_schema + "." + p_tablename + "\n"
              " DROP COLUMN " + p_columnname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL INDICES FUNCTIONS

// All index functions
CString
SQLInfoPostgreSQL::GetCATALOGIndexExists(CString p_schema,CString p_tablename,CString p_indexname) const
{
  return "";
}

CString
SQLInfoPostgreSQL::GetCATALOGIndexList(CString p_schema,CString p_tablename) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();

  CString query =  "SELECT cli.relname\n"
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
                   " WHERE sha.usename = '" + p_schema + "'\n"
                   "   AND clr.relname = '" + p_tablename+ "'\n"
                   "   AND sha.usesysid = clr.relowner\n"
                   "   AND clr.relkind = 'r'\n"
                   "   AND clr.oid = idx.indrelid\n"
                   "   AND idx.indexrelid = cli.oid\n"
                   "   AND cli.relkind = 'i'\n"
                   "   AND cli.oid = att.attrelid\n"
                   " ORDER BY cli.relname, att.attnum";
  return query;
}

CString
SQLInfoPostgreSQL::GetCATALOGIndexAttributes(CString p_schema,CString p_tablename,CString p_indexname) const
{
  return "";
}

CString
SQLInfoPostgreSQL::GetCATALOGIndexCreate(MIndicesMap& p_indices) const
{
  // Get SQL to create an index for a table
  // CREATE [UNIQUE] INDEX [<schema>.]indexname ON [<schema>.]tablename(column [ASC|DESC] [,...]);
  CString query;
  for(auto& index : p_indices)
  {
    if(index.m_position == 1)
    {
      // New index
      query = "CREATE ";
      if(index.m_unique)
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

CString
SQLInfoPostgreSQL::GetCATALOGIndexDrop(CString p_schema,CString /*p_tablename*/,CString p_indexname) const
{
  CString sql = "DROP INDEX " + p_schema + "." + p_indexname;
  return sql;
}

// Get extra filter expression for an index column
CString
SQLInfoPostgreSQL::GetCATALOGIndexFilter(MetaIndex& /*p_index*/) const
{
  return "";
}

//////////////////////////////////////////////////////////////////////////
// ALL PRIMARY KEY FUNCTIONS

CString
SQLInfoPostgreSQL::GetCATALOGPrimaryExists(CString p_schema,CString p_tablename) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();

  CString query = "SELECT COUNT(*)\n"
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

CString
SQLInfoPostgreSQL::GetCATALOGPrimaryAttributes(CString p_schema,CString p_tablename) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();

  CString query = "SELECT con.conname       as constraint_name\n"
                  "      ,con.conname       as index_name\n"
                  "      ,con.condeferrable as deferrable\n"
                  "      ,con.deferred      as initially_deferred\n"
                  "  FROM pg_class      tab\n"
                  "      ,pg_constraint con\n"
                  "      ,pg_namespaces sch\n"
                  " WHERE tab.relnamespace = sch.oid\n"
                  "   AND tab.oid          = con.conrelid\n"
                  "   AND con.contype      = 'p'\n"
                  "   AND sch.name         = '" + p_schema    + "'\n"
                  "   AND tab.relname      = '" + p_tablename + "'";
  return query;
}

CString
SQLInfoPostgreSQL::GetCATALOGPrimaryCreate(MPrimaryMap& p_primaries) const
{
  CString query("ALTER TABLE ");

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

CString
SQLInfoPostgreSQL::GetCATALOGPrimaryDrop(CString p_schema,CString p_tablename,CString p_constraintname) const
{
  CString sql("ALTER TABLE " + p_schema + "." + p_tablename + "\n"
              " DROP CONSTRAINT " + p_constraintname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL FOREIGN KEY FUNCTIONS

CString
SQLInfoPostgreSQL::GetCATALOGForeignExists(CString p_schema,CString p_tablename,CString p_constraintname) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  p_constraintname.MakeLower();

  CString sql;
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

CString
SQLInfoPostgreSQL::GetCATALOGForeignList(CString p_schema,CString p_tablename,int p_maxColumns /*=SQLINFO_MAX_COLUMNS*/) const
{
  return GetCATALOGForeignAttributes(p_schema,p_tablename,"",p_maxColumns);
}

CString
SQLInfoPostgreSQL::GetCATALOGForeignAttributes(CString p_schema,CString p_tablename,CString p_constraint,int p_maxColumns /*=SQLINFO_MAX_COLUMNS*/) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  p_constraint.MakeLower();

  CString query;

  for(int ind = 1; ind <= p_maxColumns; ++ind)
  {
    CString part;
    part.Format("SELECT current_database() as primary_catalog_name\n"
                "      ,sch.nspname        as primary_schema_name\n"
                "      ,pri.relname        as primary_table_name\n"
                "      ,current_database() as foreign_catalog_name\n"
                "      ,sch.nspname        as foreign_schema_name\n"
                "      ,cla.relname        as foreign_table_name\n"
                "      ,''                 as primary_constraint_name\n"
                "      ,con.conname        as constraint_name\n"
                "      ,%d                 as key_sequence"
                "      ,fky.attname        as primary_key_column\n"
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
                "      ,pg_attribute  fky\n"
                " WHERE con.contype      = 'f'\n"
                "   AND con.conrelid     = cla.oid\n"
                "   and cla.relnamespace = sch.oid\n"
                "   and con.confrelid    = pri.oid\n"
                "   and att.attrelid     = cla.oid\n"
                "   and att.attnum       = con.conkey[%d]\n"
                "   and fky.attrelid     = pri.oid\n"
                "   and fky.attnum       = con.confkey[%d]\n"
                "   AND cla.relname      = 'part'"
               ,ind
               ,ind
               ,ind);
    if(!p_schema.IsEmpty())
    {
      part += "\n   AND sch.nspname = '" + p_schema + "'";
    }
    if(!p_tablename.IsEmpty())
    {
      part += "\n   AND cla.relname = '" + p_tablename + "'";
    }
    if(!p_constraint.IsEmpty())
    {
      part += "\n   AND con.conname = '" + p_constraint + "'";
    }

    // Append to query, multiple for multiple columns
    if(!query.IsEmpty())
    {
      query += "\nUNION ALL\n";
    }
    query += part;
  }
  return query;
}

CString
SQLInfoPostgreSQL::GetCATALOGForeignCreate(MForeignMap& p_foreigns) const
{
  // Get first record
  MetaForeign& foreign = p_foreigns.front();

  // Construct the correct tablename
  CString table(foreign.m_fkTableName);
  CString primary(foreign.m_pkTableName);
  if(!foreign.m_fkSchemaName.IsEmpty())
  {
    table = foreign.m_fkSchemaName + "." + table;
  }
  if(!foreign.m_pkSchemaName.IsEmpty())
  {
    primary = foreign.m_pkSchemaName + "." + primary;
  }

  // The base foreign key command
  CString query = "ALTER TABLE " + table + "\n"
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

CString
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
  CString table(original.m_fkTableName);
  if(!original.m_fkSchemaName.IsEmpty())
  {
    table = original.m_fkSchemaName + "." + table;
  }

  // The base foreign key command
  CString query = "ALTER TABLE " + table + "\n"
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

CString
SQLInfoPostgreSQL::GetCATALOGForeignDrop(CString p_schema,CString p_tablename,CString p_constraintname) const
{
  CString sql("ALTER TABLE " + p_schema + "." + p_tablename + "\n"
              " DROP CONSTRAINT " + p_constraintname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL TRIGGER FUNCTIONS

CString
SQLInfoPostgreSQL::GetCATALOGTriggerExists(CString p_schema, CString p_tablename, CString p_triggername) const
{
  return "";
}

CString
SQLInfoPostgreSQL::GetCATALOGTriggerList(CString p_schema, CString p_tablename) const
{
  return "";
}

CString
SQLInfoPostgreSQL::GetCATALOGTriggerAttributes(CString p_schema, CString p_tablename, CString p_triggername) const
{
  return "";
}

CString
SQLInfoPostgreSQL::GetCATALOGTriggerCreate(MetaTrigger& /*p_trigger*/) const
{
  return "";
}

CString
SQLInfoPostgreSQL::GetCATALOGTriggerDrop(CString p_schema, CString p_tablename, CString p_triggername) const
{
  return "";
}

//////////////////////////////////////////////////////////////////////////
// ALL SEQUENCE FUNCTIONS

CString
SQLInfoPostgreSQL::GetCATALOGSequenceExists(CString p_schema, CString p_sequence) const
{
  p_schema.MakeLower();
  p_sequence.MakeLower();

  CString sql("SELECT COUNT(*)\n"
              "  FROM information_schema.sequences\n"
              " WHERE sequence_schema = '" + p_schema   + "'\n"
              "   AND sequence_name   = '" + p_sequence + "'");
  return sql;
}

CString
SQLInfoPostgreSQL::GetCATALOGSequenceAttributes(CString p_schema, CString p_sequence) const
{
  p_schema.MakeLower();
  p_sequence.MakeLower();

  CString sql = "SELECT ''              AS catalog_name\n"
                "      ,sequence_schema AS schema_name\n"
                "      ,sequence_name\n"
                "      ,start_value     AS current_value\n"
                "      ,0               AS minimal_value\n"
                "      ,increment\n"
                "      ,0               AS cache\n"
                "      ,decode(cycle_option,'NO',1,0) AS cycle\n"
                "      ,0               AS ordering\n"
                "  FROM information_schema.sequences\n"
                " WHERE sequence_schema = '" + p_schema   + "'\n"
                "   AND sequence_name   = '" + p_sequence + "'";
  return sql;
}

CString
SQLInfoPostgreSQL::GetCATALOGSequenceCreate(MetaSequence& p_sequence) const
{
  CString sql("CREATE SEQUENCE ");

  if (!p_sequence.m_schemaName.IsEmpty())
  {
    sql += p_sequence.m_schemaName + ".";
  }
  sql += p_sequence.m_sequenceName;
  sql.AppendFormat(" START WITH %d", p_sequence.m_currentValue);
  if(p_sequence.m_cache)
  {
    sql.AppendFormat(" CACHE %d",p_sequence.m_cache);
  }
  sql += p_sequence.m_cycle ? " CYCLE" : " NO CYCLE";
  return sql;
}

CString
SQLInfoPostgreSQL::GetCATALOGSequenceDrop(CString p_schema, CString p_sequence) const
{
  CString sql("DROP SEQUENCE " + p_schema + "." + p_sequence);
  return  sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL VIEW FUNCTIONS

CString 
SQLInfoPostgreSQL::GetCATALOGViewExists(CString p_schema,CString p_viewname) const
{
  p_schema.MakeLower();
  p_viewname.MakeLower();
  CString sql("SELECT COUNT(*)\n"
              "  FROM pg_views\n"  // TODO: Check the correct name
              " WHERE view_name = '" + p_viewname + "'");
  return sql;
}

CString 
SQLInfoPostgreSQL::GetCATALOGViewList(CString p_schema,CString p_pattern) const
{
  return "";
}

CString 
SQLInfoPostgreSQL::GetCATALOGViewAttributes(CString p_schema,CString p_viewname) const
{
  return "";
}

CString 
SQLInfoPostgreSQL::GetCATALOGViewCreate(CString p_schema,CString p_viewname,CString p_contents) const
{
  return "CREATE OR REPLACE VIEW " + p_schema + "." + p_viewname + "\n" + p_contents;
}

CString 
SQLInfoPostgreSQL::GetCATALOGViewRename(CString p_schema,CString p_viewname,CString p_newname)    const
{
  return "";
}

CString 
SQLInfoPostgreSQL::GetCATALOGViewDrop(CString p_schema,CString p_viewname,CString& p_precursor) const
{
  p_precursor.Empty();
  return "DROP VIEW " + p_schema + "." + p_viewname;
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

CString
SQLInfoPostgreSQL::GetPSMProcedureExists(CString p_schema, CString p_procedure) const
{
  p_schema.MakeLower();
  p_procedure.MakeLower();
  return "SELECT count(*)\n"
         "  FROM pg_proc\n"
         " WHERE proname = '" + p_procedure + "'\n;";
}

CString
SQLInfoPostgreSQL::GetPSMProcedureList(CString p_schema) const
{
  return "";
}

CString
SQLInfoPostgreSQL::GetPSMProcedureAttributes(CString p_schema, CString p_procedure) const
{
  p_schema.MakeLower();
  p_procedure.MakeLower();

  CString sql = "SELECT TEXT from ALL_SOURCE "
                "WHERE type = 'FUNCTION' "
                "AND name  = '" + p_procedure + "'\n"
                "AND owner = '" + p_schema    + "'";
  return sql;
}

CString
SQLInfoPostgreSQL::GetPSMProcedureCreate(MetaProcedure& /*p_procedure*/) const
{
  return "";
}

CString
SQLInfoPostgreSQL::GetPSMProcedureDrop(CString p_schema, CString p_procedure) const
{
  return "";
}

CString
SQLInfoPostgreSQL::GetPSMProcedureErrors(CString p_schema,CString p_procedure) const
{
  return "";
}

//////////////////////////////////////////////////////////////////////////
//
// ALL PSM LANGUAGE ELEMENTS
//
//////////////////////////////////////////////////////////////////////////

CString
SQLInfoPostgreSQL::GetPSMDeclaration(bool    /*p_first*/
                                    ,CString p_variable
                                    ,int     p_datatype
                                    ,int     p_precision /*= 0 */
                                    ,int     p_scale     /*= 0 */
                                    ,CString p_default   /*= ""*/
                                    ,CString p_domain    /*= ""*/
                                    ,CString p_asColumn  /*= ""*/) const
{
  CString line;
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

CString
SQLInfoPostgreSQL::GetPSMAssignment(CString p_variable,CString p_statement /*=""*/) const
{
  CString line(p_variable);
  line += " := ";
  if(!p_statement.IsEmpty())
  {
    line += p_statement;
    line += ";";
  }
  return line;
}

CString
SQLInfoPostgreSQL::GetPSMIF(CString p_condition) const
{
  CString line("IF (");
  line += p_condition;
  line += ") THEN\n";
  return line;
}

CString
SQLInfoPostgreSQL::GetPSMIFElse() const
{
  return "ELSE\n";
}

CString
SQLInfoPostgreSQL::GetPSMIFEnd() const
{
  return "END IF;\n";
}

CString
SQLInfoPostgreSQL::GetPSMWhile(CString p_condition) const
{
  return "WHILE (" + p_condition + ") LOOP\n";
}

CString
SQLInfoPostgreSQL::GetPSMWhileEnd() const
{
  return "END LOOP;\n";
}

CString
SQLInfoPostgreSQL::GetPSMLOOP() const
{
  return "LOOP\n";
}

CString
SQLInfoPostgreSQL::GetPSMLOOPEnd() const
{
  return "END LOOP;\n";
}

CString
SQLInfoPostgreSQL::GetPSMBREAK() const
{
  return "EXIT;\n";
}

CString
SQLInfoPostgreSQL::GetPSMRETURN(CString p_statement /*= ""*/) const
{
  CString line("RETURN");
  if(!p_statement.IsEmpty())
  {
    line += " " + p_statement;
  }
  line += ";\n";
  return line;
}

CString
SQLInfoPostgreSQL::GetPSMExecute(CString p_procedure,MParameterMap& p_parameters) const
{
  CString line;
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
CString
SQLInfoPostgreSQL::GetPSMCursorDeclaration(CString p_cursorname,CString p_select) const
{
  return "DECLARE " + p_cursorname + " CURSOR FOR " + p_select + ";";
}

CString
SQLInfoPostgreSQL::GetPSMCursorFetch(CString p_cursorname,std::vector<CString>& /*p_columnnames*/,std::vector<CString>& p_variablenames) const
{
  bool moreThenOne = false;
  CString query = "OPEN  " + p_cursorname + ";\n"
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

CString
SQLInfoPostgreSQL::GetPSMExceptionCatchNoData() const
{
  return "EXCEPTION WHEN 100 THEN\n";
  // Followed by block and 'END;'
}

CString
SQLInfoPostgreSQL::GetPSMExceptionCatch(CString p_sqlState) const
{
  return "EXCEPTION WHEN " + p_sqlState + " THEN\n";
  // Followed by block and 'END;'
}

CString
SQLInfoPostgreSQL::GetPSMExceptionRaise(CString p_sqlState) const
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

CString
SQLInfoPostgreSQL::GetSESSIONMyself() const
{
  CString query = "SELECT procpid\n"
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

CString
SQLInfoPostgreSQL::GetSESSIONExists(CString p_sessionID) const
{
  CString sql = "SELECT COUNT(*)\n"
                "  FROM pg_stat_activity\n"
                " WHERE procpid = " + p_sessionID;
  return sql;
}

CString
SQLInfoPostgreSQL::GetSESSIONList() const
{
  return GetSESSIONAttributes("");
}

CString
SQLInfoPostgreSQL::GetSESSIONAttributes(CString p_sessionID) const
{
  CString sql = "SELECT procpid\n"
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

CString
SQLInfoPostgreSQL::GetSESSIONConstraintsDeferred() const
{
  return "set transaction deferrable";
}

CString
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
SQLInfoPostgreSQL::DoSQLCall(SQLQuery* p_query,CString& p_schema,CString& p_procedure)
{
  // PostgreSQL does not support the return parameter of the "{[?=]CALL procedure(?,?)}" sequence
  // instead you have to do a "SELECT procedure(?,?)" 
  // The result set is the set of output parameters
  // DOES ONLY SUPPORT A SINGLE ROW RESULT SET!!
  SQLQuery query(m_database);
  CString sql   = ConstructSQLForProcedureCall(p_query,&query,p_schema,p_procedure);
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
          throw CString("Wrong number of output parameters for procedure call");
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
CString
SQLInfoPostgreSQL::ConstructSQLForProcedureCall(SQLQuery* p_query
                                               ,SQLQuery* p_thecall
                                               ,CString&  p_schema
                                               ,CString&  p_procedure)
{
  // Start with select form
  CString sql = "SELECT ";
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
