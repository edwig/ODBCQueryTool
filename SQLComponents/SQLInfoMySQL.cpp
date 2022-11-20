////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoMySQL.cpp
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
#include "SQLInfoMySQL.h"
#include "SQLQuery.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{

// Constructor
SQLInfoMySQL::SQLInfoMySQL(SQLDatabase* p_database)
             :SQLInfoDB(p_database)
{
}

// Destructor. Does nothing
SQLInfoMySQL::~SQLInfoMySQL()
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
SQLInfoMySQL::GetRDBMSDatabaseType() const
{
  return RDBMS_MYSQL;
}

// The name of the database vendor
XString
SQLInfoMySQL::GetRDBMSVendorName() const
{
  // The name of the database vendor
  return "MySQL A.B. / Oracle";
}

// Get the physical database name
XString
SQLInfoMySQL::GetRDBMSPhysicalDatabaseName() const
{
  // See to it that "SQLDatabase:GetDatabaseName" does it's work
  return m_database->GetDatabaseName();
}

// System catalog is stored in uppercase in the database?
bool
SQLInfoMySQL::GetRDBMSIsCatalogUpper() const
{
  return false;
}

// System catalog supports full ISO schemas (same tables per schema)
bool
SQLInfoMySQL::GetRDBMSUnderstandsSchemas() const
{
  return false;
}

// Supports database/ODBCdriver comments in SQL
bool
SQLInfoMySQL::GetRDBMSSupportsComments() const
{
  return true;
}

// Database can defer constraints until the end of a transaction
bool
SQLInfoMySQL::GetRDBMSSupportsDeferredConstraints() const
{
  // NO CONSTRAINTS DEFERRED 
  // If transactions are supported we assume that constraints can be deferred
  // until the end of the transaction in the commit phase
  return false;
}

// Database has ORDER BY with an expression, e.g. ORDER BY UPPER(columnname)
bool
SQLInfoMySQL::GetRDBMSSupportsOrderByExpression() const
{
  return true;
}

// Supports the ODBC escape sequence {[?=] CALL procedure (?,?,?)}
bool
SQLInfoMySQL::GetRDBMSSupportsODBCCallEscapes() const
{
  return true;
}

// If the database does not support the datatype TIME, it can be implemented as a DECIMAL
bool
SQLInfoMySQL::GetRDBMSSupportsDatatypeTime() const
{
  // Time can be implemented as TIME
  return true;
}

// If the database does not support the datatype INTERVAL, it can be implemented as a DECIMAL
bool
SQLInfoMySQL::GetRDBMSSupportsDatatypeInterval() const
{
  // Interval not supported, can be implemented as DECIMALS
  return false;
}

// Supports functions at the place of table columns in create/alter index statement
bool
SQLInfoMySQL::GetRDBMSSupportsFunctionalIndexes() const
{
  return true;
}

// Gets the maximum length of an SQL statement
unsigned long
SQLInfoMySQL::GetRDBMSMaxStatementLength() const
{
  // No Limit
  return 0;
}

// Database must commit DDL commands in a transaction
bool
SQLInfoMySQL::GetRDBMSMustCommitDDL() const
{
  return false;
}

// Correct maximum precision,scale for a NUMERIC datatype
void
SQLInfoMySQL::GetRDBMSNumericPrecisionScale(SQLULEN& /*p_precision*/, SQLSMALLINT& /*p_scale*/) const
{
  // NO-OP
}

// KEYWORDS

// Keyword for the current date and time
XString
SQLInfoMySQL::GetKEYWORDCurrentTimestamp() const
{
  return "current_timestamp";
}

// String for the current date
XString
SQLInfoMySQL::GetKEYWORDCurrentDate() const
{
  return "current_date";
}

// Get the concatenation operator
XString
SQLInfoMySQL::GetKEYWORDConcatanationOperator() const
{
  // BEWARE: Officially the concatenation operator is "CONCAT(string1,string2)"
  // MYSQL supports 'one' 'two' concatenation of two strings (no operator)
  return "";
}

// Get quote character for strings
XString
SQLInfoMySQL::GetKEYWORDQuoteCharacter() const
{
  return "\'";
}

// Get quote character around reserved words as an identifier
XString
SQLInfoMySQL::GetKEYWORDReservedWordQuote() const
{
  return "`";
}

// Get default NULL for parameter list input
XString
SQLInfoMySQL::GetKEYWORDParameterDefaultNULL() const
{
  // Standard, no definition defines the NULL state
  return "";
}

// Parameter is for INPUT and OUTPUT in parameter list
XString
SQLInfoMySQL::GetKEYWORDParameterINOUT() const
{
  return "";
}

// Parameter is for OUTPUT only in parameter list
XString
SQLInfoMySQL::GetKEYWORDParameterOUT() const
{
  return "";
}

// Get datatype of the IDENTITY primary key in a Network database
XString
SQLInfoMySQL::GetKEYWORDNetworkPrimaryKeyType() const
{
  // Use SEQUENCE to fill!
  return "INTEGER";
}

// Get datatype for timestamp (year to second)
XString
SQLInfoMySQL::GetKEYWORDTypeTimestamp() const
{
  return "TIMESTAMP";
}

// Prefix for a parameter in a stored procedure
XString
SQLInfoMySQL::GetKEYWORDParameterPrefix() const
{
  return "";
}

// Get select part to add new record identity to a table
// Can be special column like 'OID' or a sequence select
XString
SQLInfoMySQL::GetKEYWORDIdentityString(XString& p_tablename,XString p_postfix /*= "_seq"*/) const
{
  return p_tablename + p_postfix + ".nextval";
}

// Gets the UPPER function
XString
SQLInfoMySQL::GetKEYWORDUpper(XString& p_expression) const
{
  return "UPPER(" + p_expression + ")";
}

// Gets the construction for 1 minute ago
XString
SQLInfoMySQL::GetKEYWORDInterval1MinuteAgo() const
{
  return "ERROR";
}

// Gets the Not-NULL-Value statement of the database
XString
SQLInfoMySQL::GetKEYWORDStatementNVL(XString& p_test,XString& p_isnull) const
{
  return "{fn IFNULL(" + p_test + "," + p_isnull + ")}";
}

// Gets the RDBMS definition of the datatype
XString
SQLInfoMySQL::GetKEYWORDDataType(MetaColumn* p_column)
{
  return p_column->m_typename;
}

// Gets the USER (current-user) keyword function
XString
SQLInfoMySQL::GetKEYWORDCurrentUser() const
{
  return "CURRENT_USER";
}

// Connects to a default schema in the database/instance
XString
SQLInfoMySQL::GetSQLDefaultSchema(XString /*p_user*/,XString p_schema) const
{
  return "USE " + p_schema;
}

// Gets the construction for inline generating a key within an INSERT statement
XString
SQLInfoMySQL::GetSQLNewSerial(XString /*p_table*/, XString /*p_sequence*/) const
{
  // Insert a zero in an IDENTITY column
  return "0";
}

// Gets the construction / select for generating a new serial identity
XString
SQLInfoMySQL::GetSQLGenerateSerial(XString p_table) const
{
  // NO WAY OF KNOWNING THIS
  return "0";
}

// Gets the construction / select for the resulting effective generated serial
XString
SQLInfoMySQL::GetSQLEffectiveSerial(XString p_identity) const
{
  // THIS IS MOST LIKELY NOT THE CORRECT VALUE.
  // NO WAY OF DETERMINING THIS
  return p_identity;
}

// Gets the sub-transaction commands
XString
SQLInfoMySQL::GetSQLStartSubTransaction(XString p_savepointName) const
{
  // Generic ODBC does not known about sub transactions!
  return XString("");
}

XString
SQLInfoMySQL::GetSQLCommitSubTransaction(XString p_savepointName) const
{
  // Generic ODBC does not known about sub transactions!
  return XString("");
}

XString
SQLInfoMySQL::GetSQLRollbackSubTransaction(XString p_savepointName) const
{
  // Generic ODBC does not known about sub transactions!
  return XString("");
}

// FROM-Part for a query to select only 1 (one) record / or empty!
XString
SQLInfoMySQL::GetSQLFromDualClause() const
{
  // MySQL does bare SELECT!
  return "";
}

// Get SQL to lock  a table 
XString
SQLInfoMySQL::GetSQLLockTable(XString /*p_schema*/, XString p_tablename, bool p_exclusive) const
{
  // Standard ISO SQL Syntax
  XString query = "LOCK TABLE " + p_tablename + " IN ";
  query += p_exclusive ? "EXCLUSIVE" : "SHARE";
  query += " MODE";
  return query;
}

// Get query to optimize the table statistics
XString
SQLInfoMySQL::GetSQLOptimizeTable(XString p_schema, XString p_tablename) const
{
  // To be implemented
  return "";
}

// Transform query to select top <n> rows
XString
SQLInfoMySQL::GetSQLTopNRows(XString p_sql,int p_top,int p_skip /*= 0*/) const
{
  if(p_top > 0)
  {
    // MYSQL: " LIMIT <top> [ OFFSET <skip> ]
    XString limit;
    limit.Format("\n LIMIT %d",p_top);
    if(p_skip > 0)
    {
      limit.AppendFormat(" OFFSET %d",p_skip);
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
SQLInfoMySQL::GetSQLString(const XString& p_string) const
{
  XString s = p_string;
  s.Replace("'","''");
  XString kwoot = GetKEYWORDQuoteCharacter();
  return  kwoot + s + kwoot;
}

// Get date string in engine format
XString
SQLInfoMySQL::GetSQLDateString(int p_year,int p_month,int p_day) const
{
  XString dateString;
  dateString.Format("{d '%04d-%02d-%02d'}",p_year,p_month,p_day);
  return dateString;
}

// Get time string in database engine format
XString
SQLInfoMySQL::GetSQLTimeString(int p_hour,int p_minute,int p_second) const
{
  XString retval;
  retval.Format("{t '%02d:%02d:%02d'}",p_hour,p_minute,p_second);
  return retval;
}

// Get date-time string in database engine format
XString
SQLInfoMySQL::GetSQLDateTimeString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  XString string;
  string.Format("{ts '%04d-%02d-%02d %02d:%02d:%02d'}"
                ,p_year,p_month,p_day // ODBC Ordering !!
                ,p_hour,p_minute,p_second);
  return string;
}

// Get date-time bound parameter string in database format
XString
SQLInfoMySQL::GetSQLDateTimeBoundString() const
{
  return "{ts ?}";
}

// Stripped data for the parameter binding
XString
SQLInfoMySQL::GetSQLDateTimeStrippedString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  XString string;
  string.Format("%04d-%02d-%02d %02d:%02d:%02d"
                ,p_year,p_month,p_day // ODBC Ordering !!
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
// Standard ODBC functions are good enough
XString
SQLInfoMySQL::GetCATALOGMetaTypes(int p_type) const
{
  UNREFERENCED_PARAMETER(p_type);
  return "";
}

// Get SQL to check if a table already exists in the database
XString
SQLInfoMySQL::GetCATALOGTableExists(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  // Still to do in MySQL
  return "";
}

XString
SQLInfoMySQL::GetCATALOGTablesList(XString& p_schema,XString& p_pattern) const
{
  return GetCATALOGTableAttributes(p_schema,p_pattern);
}

XString
SQLInfoMySQL::GetCATALOGTableAttributes(XString& p_schema,XString& p_tablename) const
{
  XString sql;
  sql = "SELECT table_catalog\n"
        "      ,table_schema\n"
        "      ,table_name\n"
        "      ,case table_type\n"
        "            when 'BASE TABLE' THEN 'TABLE'\n"
        "            else 'UNKNOWN'\n"
        "       end as table_type\n"
        "      ,table_comment\n"
        "      ,table_catalog || '.' || table_schema || '.' || table_name as fullname\n"
        "      ,engine\n"
        "      ,0 as temporary\n" 
        "  FROM information_schema.tables\n"  
        " WHERE table_type = 'BASE TABLE'\n"
        "   AND table_schema NOT IN ('mysql','sys','performance_schema')\n";
  if(!p_schema)
  {
    sql += "   AND table_schema = ?\n";
  }
  if(!p_tablename.IsEmpty())
  {
    sql += "   AND table_name ";
    sql += p_tablename.Find('%') >= 0 ? "LIKE" : "=";
    sql += " ?\n";
  }
  sql += " ORDER BY 1,2,3";
  return sql;
}

XString
SQLInfoMySQL::GetCATALOGTableSynonyms(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  // MS-Access cannot do this
  return XString();
}

XString
SQLInfoMySQL::GetCATALOGTableCatalog(XString& p_schema,XString& p_tablename) const
{
  p_schema.Empty(); // do not bind as a parameter

  XString sql;
  sql = "SELECT table_catalog\n"
        "      ,table_schema\n"
        "      ,table_name\n"
        "      ,'SYSTEM TABLE' as table_type\n"
        "      ,table_comment\n"
        "      ,table_catalog || '.' || table_schema || '.' || table_name as fullname\n"
        "      ,engine\n"
        "      ,0 as temporary\n" 
        "  FROM information_schema.tables\n"  
        " WHERE ( table_type = 'SYSTEM VIEW'\n"
        "      OR table_schema IN ('mysql','sys','performance_schema'))\n";
  if(!p_tablename.IsEmpty())
  {
    sql += "   AND table_name ";
    sql += p_tablename.Find('%') >= 0 ? "LIKE" : "=";
    sql += " ?\n";
  }
  sql += " ORDER BY 1,2,3";
  return sql;}

XString
SQLInfoMySQL::GetCATALOGTableCreate(MetaTable& p_table,MetaColumn& /*p_column*/) const
{
  XString sql = "CREATE ";
  if (p_table.m_temporary)
  {
    sql += "TEMPORARY ";
  }
  sql += "TABLE " + p_table.m_table;
  return sql;
}

XString
SQLInfoMySQL::GetCATALOGTableCreatePostfix(MetaTable& /*p_table*/,MetaColumn& /*p_column*/) const
{
  return "";
}

XString
SQLInfoMySQL::GetCATALOGTableRename(XString /*p_schema*/,XString p_tablename,XString p_newname) const
{
  XString sql("RENAME TABLE" + p_tablename + " TO " + p_newname);
  return sql;
}

XString
SQLInfoMySQL::GetCATALOGTableDrop(XString /*p_schema*/,XString p_tablename,bool p_ifExist /*= false*/,bool p_restrict /*= false*/,bool p_cascade /*= false*/) const
{
  XString sql("DROP TABLE ");
  if (p_ifExist)
  {
    sql += "IF EXISTS ";
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
SQLInfoMySQL::GetCATALOGTemptableCreate(XString /*p_schema*/,XString p_tablename,XString p_select) const
{
  // BEWARE: THIS IS A GUESS. 
  return "CREATE TEMPORARY TABLE " + p_tablename + "\nAS " + p_select;
}

XString 
SQLInfoMySQL::GetCATALOGTemptableIntoTemp(XString /*p_schema*/,XString p_tablename,XString p_select) const
{
  return "INSERT INTO " + p_tablename + "\n" + p_select;
}

XString 
SQLInfoMySQL::GetCATALOGTemptableDrop(XString /*p_schema*/,XString p_tablename) const
{
  return "DROP TABLE " + p_tablename;
}

//////////////////////////////////////////////////////////////////////////
// ALL COLUMN FUNCTIONS

XString 
SQLInfoMySQL::GetCATALOGColumnExists(XString /*p_schema*/,XString p_tablename,XString p_columnname) const
{
  return "";
}

XString 
SQLInfoMySQL::GetCATALOGColumnList(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  // Standard ODBC driver suffices
  return "";
}

XString 
SQLInfoMySQL::GetCATALOGColumnAttributes(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_columnname*/) const
{
  // Standard ODBC driver suffices
  return "";
}

XString 
SQLInfoMySQL::GetCATALOGColumnCreate(MetaColumn& p_column) const
{
  XString sql = "ALTER TABLE "  + p_column.m_table  + "\n"
                "  ADD COLUMN " + p_column.m_column + " " + p_column.m_typename;
  p_column.GetPrecisionAndScale(sql);
  p_column.GetNullable(sql);
  p_column.GetDefault(sql);
  // m_position not used
  // m_length   not used
  // m_remarks  not used
  return sql;}

XString 
SQLInfoMySQL::GetCATALOGColumnAlter(MetaColumn& p_column) const
{
  // The MODIFY keyword is a-typical
  XString sql = "ALTER  TABLE  " + p_column.m_table + "\n"
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
SQLInfoMySQL::GetCATALOGColumnRename(XString p_schema,XString p_tablename,XString p_columnname,XString p_newname,XString /*p_datatype*/) const
{
  // General ISO syntax
  XString sql("ALTER  TABLE  " + p_tablename + "\n"
              "RENAME " + p_columnname + " TO " + p_newname + "\n");
  return sql;
}

XString 
SQLInfoMySQL::GetCATALOGColumnDrop(XString p_schema,XString p_tablename,XString p_columnname) const
{
  XString sql("ALTER TABLE  " + p_tablename + "\n"
              " DROP COLUMN " + p_columnname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL INDICES FUNCTIONS

// All index functions
XString
SQLInfoMySQL::GetCATALOGIndexExists(XString /*p_schema*/,XString p_tablename,XString p_indexname) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLStatistics instead (see SQLInfo class)
  return "";
}

XString
SQLInfoMySQL::GetCATALOGIndexList(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLStatistics instead (see SQLInfo class)
  return "";
}

XString
SQLInfoMySQL::GetCATALOGIndexAttributes(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_indexname*/) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLStatistics instead (see SQLInfo class)
  return "";
}

XString
SQLInfoMySQL::GetCATALOGIndexCreate(MIndicesMap& p_indices) const
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
SQLInfoMySQL::GetCATALOGIndexDrop(XString /*p_schema*/,XString /*p_tablename*/,XString p_indexname) const
{
  XString sql = "DROP INDEX " + p_indexname;
  return sql;
}

// Get extra filter expression for an index column
XString
SQLInfoMySQL::GetCATALOGIndexFilter(MetaIndex& /*p_index*/) const
{
  return "";
}

//////////////////////////////////////////////////////////////////////////
// ALL PRIMARY KEY FUNCTIONS

XString
SQLInfoMySQL::GetCATALOGPrimaryExists(XString /*p_schema*/,XString p_tablename) const
{
  // To be implemented
  return "";
}

XString
SQLInfoMySQL::GetCATALOGPrimaryAttributes(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  // To be implemented
  return "";
}

XString
SQLInfoMySQL::GetCATALOGPrimaryCreate(MPrimaryMap& p_primaries) const
{
  XString query("ALTER TABLE ");

  for(auto& prim : p_primaries)
  {
    if(prim.m_columnPosition == 1)
    {
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
SQLInfoMySQL::GetCATALOGPrimaryDrop(XString /*p_schema*/,XString p_tablename,XString p_constraintname) const
{
  XString sql("ALTER TABLE " + p_tablename + "\n"
              " DROP CONSTRAINT " + p_constraintname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL FOREIGN KEY FUNCTIONS

XString
SQLInfoMySQL::GetCATALOGForeignExists(XString /*p_schema*/,XString p_tablename,XString p_constraintname) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLForeignKeys instead (see SQLInfo class)
  return "";
}

XString
SQLInfoMySQL::GetCATALOGForeignList(XString& /*p_schema*/,XString& /*p_tablename*/,int /*p_maxColumns*/ /*=SQLINFO_MAX_COLUMNS*/) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLForeignKeys instead (see SQLInfo class)
  return "";
}

XString
SQLInfoMySQL::GetCATALOGForeignAttributes(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_constraintname*/,bool /*p_referenced = false*/,int /*p_maxColumns*/ /*=SQLINFO_MAX_COLUMNS*/) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLForeignKeys instead (see SQLInfo class)
  return "";
}

XString
SQLInfoMySQL::GetCATALOGForeignCreate(MForeignMap& p_foreigns) const
{
  // Get first record
  MetaForeign& foreign = p_foreigns.front();

  // Construct the correct tablename
  XString table(foreign.m_fkTableName);
  XString primary(foreign.m_pkTableName);

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
    extra = true;
  }

  // Add references primary table
  query += ")\n      REFERENCES " + primary + "(";

  // Add the primary key columns
  extra = false;
  for(auto& key : p_foreigns)
  {
    if(extra) query += ",";
    query += key.m_pkColumnName;
    extra = true;
  }
  query += ")";

  // Add all relevant options
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
    case SQL_CASCADE:     query += "\n      ON UPDATE CASCADE";     break;
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
SQLInfoMySQL::GetCATALOGForeignAlter(MForeignMap& p_original, MForeignMap& p_requested) const
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

  // The base foreign key command
  XString query = "ALTER TABLE " + table + "\n"
                  "ALTER CONSTRAINT " + original.m_foreignConstraint + "\n";

  // Add all relevant options
  if(original.m_deferrable != requested.m_deferrable)
  {
    switch(requested.m_deferrable)
    {
      case SQL_INITIALLY_DEFERRED:  query += "\n      INITIALLY DEFERRED"; break;
      case SQL_INITIALLY_IMMEDIATE: query += "\n      DEFERRABLE";         break;
      case SQL_NOT_DEFERRABLE:      query += "\n      NOT DEFERRABLE";     break;
      default:                      break;
    }
  }
  if(original.m_match != requested.m_match)
  {
    switch(requested.m_match)
    {
      case SQL_MATCH_FULL:    query += "\n      MATCH FULL";    break;
      case SQL_MATCH_PARTIAL: query += "\n      MATCH PARTIAL"; break;
      case SQL_MATCH_SIMPLE:  query += "\n      MATCH SIMPLE";  break;
    }
  }
  if(original.m_updateRule != requested.m_updateRule)
  {
    switch(requested.m_updateRule)
    {
      case SQL_CASCADE:     query += "\n      ON UPDATE CASCADE";     break;
      case SQL_SET_NULL:    query += "\n      ON UPDATE SET NULL";    break;
      case SQL_SET_DEFAULT: query += "\n      ON UPDATE SET DEFAULT"; break;
      case SQL_NO_ACTION:   query += "\n      ON UPDATE NO ACTION";   break;
      default:              // In essence: ON UPDATE RESTRICT, but that's already the default
      case SQL_RESTRICT:    break;
    }
  }
  if(original.m_deleteRule != requested.m_deleteRule)
  {
    switch(requested.m_deleteRule)
    {
      case SQL_CASCADE:     query += "\n      ON DELETE CASCADE";     break;
      case SQL_SET_NULL:    query += "\n      ON DELETE SET NULL";    break;
      case SQL_SET_DEFAULT: query += "\n      ON DELETE SET DEFAULT"; break;
      case SQL_NO_ACTION:   query += "\n      ON DELETE NO ACTION";   break;
      default:              // In essence: ON DELETE RESTRICT, but that's already the default
      case SQL_RESTRICT:    break;
    }
  }
  return query;
}

XString
SQLInfoMySQL::GetCATALOGForeignDrop(XString /*p_schema*/,XString p_tablename,XString p_constraintname) const
{
  XString sql("ALTER TABLE " + p_tablename + "\n"
              " DROP CONSTRAINT " + p_constraintname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL TRIGGER FUNCTIONS

XString
SQLInfoMySQL::GetCATALOGTriggerExists(XString /*p_schema*/, XString p_tablename, XString p_triggername) const
{
  return "";
}

XString
SQLInfoMySQL::GetCATALOGTriggerList(XString& p_schema,XString& p_tablename) const
{
  XString triggername;
  return GetCATALOGTriggerAttributes(p_schema,p_tablename,triggername);
}

XString
SQLInfoMySQL::GetCATALOGTriggerAttributes(XString& p_schema,XString& p_tablename,XString& p_triggername) const
{
  XString sql;
  sql = "SELECT event_object_catalog\n"
        "      ,event_object_schema\n"
        "      ,event_object_table\n"
        "      ,trigger_name\n"
        "      ,'' AS trigger_remarks\n"
        "      ,action_order  AS trigger_position\n"
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
        "      ,FALSE AS trigger_select\n"
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
SQLInfoMySQL::GetCATALOGTriggerCreate(MetaTrigger& /*p_trigger*/) const
{
  return "";
}

XString
SQLInfoMySQL::GetCATALOGTriggerDrop(XString /*p_schema*/, XString p_tablename, XString p_triggername) const
{
  return "";
}

//////////////////////////////////////////////////////////////////////////
// ALL SEQUENCE FUNCTIONS

XString
SQLInfoMySQL::GetCATALOGSequenceExists(XString /*p_schema*/, XString p_sequence) const
{
  return "";
}

XString
SQLInfoMySQL::GetCATALOGSequenceList(XString& /*p_schema*/,XString& /*p_pattern*/) const
{
  return "";
}

XString
SQLInfoMySQL::GetCATALOGSequenceAttributes(XString& /*p_schema*/,XString& /*p_sequence*/) const
{
  return "";
}

XString
SQLInfoMySQL::GetCATALOGSequenceCreate(MetaSequence& /*p_sequence*/) const
{
  return "";
}

XString
SQLInfoMySQL::GetCATALOGSequenceDrop(XString /*p_schema*/, XString p_sequence) const
{
  return "";
}

//////////////////////////////////////////////////////////////////////////
// ALL VIEW FUNCTIONS

XString 
SQLInfoMySQL::GetCATALOGViewExists(XString& /*p_schema*/,XString& /*p_viewname*/) const
{
  return "";
}

XString 
SQLInfoMySQL::GetCATALOGViewList(XString& p_schema,XString& p_pattern) const
{
  return GetCATALOGViewAttributes(p_schema,p_pattern);
}

XString 
SQLInfoMySQL::GetCATALOGViewAttributes(XString& p_schema,XString& p_viewname) const
{
  XString sql;
  sql = "SELECT table_catalog\n"
        "      ,table_schema\n"
        "      ,table_name\n"
        "      ,'VIEW' as table_type\n"
        "      ,table_comment\n"
        "      ,table_catalog || '.' || table_schema || '.' || table_name as fullname\n"
        "      ,engine\n"
        "      ,0 as temporary\n" 
        "  FROM information_schema.tables\n"  
        " WHERE table_type = 'VIEW'\n"
        "   AND table_schema NOT IN ('mysql','sys','performance_schema')\n";
  if(!p_schema)
  {
    sql += "   AND table_schema = ?\n";
  }
  if(!p_viewname.IsEmpty())
  {
    sql += "   AND table_name ";
    sql += p_viewname.Find('%') >= 0 ? "LIKE" : "=";
    sql += " ?\n";
  }
  sql += " ORDER BY 1,2,3";
  return sql;
}

XString
SQLInfoMySQL::GetCATALOGViewText(XString& /*p_schema*/,XString& /*p_viewname*/) const
{
  // Cannot query this, Use ODBC functions
  return "";
}

XString
SQLInfoMySQL::GetCATALOGViewCreate(XString /*p_schema*/,XString p_viewname,XString p_contents) const
{
  return "CREATE VIEW " + p_viewname + "\n" + p_contents;
}

XString 
SQLInfoMySQL::GetCATALOGViewRename(XString p_schema,XString p_viewname,XString p_newname)    const
{
  return "";
}

XString 
SQLInfoMySQL::GetCATALOGViewDrop(XString /*p_schema*/,XString p_viewname,XString& p_precursor) const
{
  p_precursor.Empty();
  return "DROP VIEW " + p_viewname;
}

// All Privilege functions
XString
SQLInfoMySQL::GetCATALOGTablePrivileges(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  return "";
}

XString 
SQLInfoMySQL::GetCATALOGColumnPrivileges(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_columnname*/) const
{
  return "";
}

XString 
SQLInfoMySQL::GetCatalogGrantPrivilege(XString /*p_schema*/,XString p_objectname,XString p_privilege,XString p_grantee,bool p_grantable)
{
  XString sql;
  p_grantee.MakeLower();

  // MySQL does not know the concept of "PUBLIC"
  if(p_grantee.Compare("public"))
  {
    sql.Format("GRANT %s ON %s TO %s",p_privilege.GetString(),p_objectname.GetString(),p_grantee.GetString());
    if(p_grantable)
    {
      sql += " WITH GRANT OPTION";
    }
  }
  return sql;
}

XString 
SQLInfoMySQL::GetCatalogRevokePrivilege(XString /*p_schema*/,XString p_objectname,XString p_privilege,XString p_grantee)
{
  XString sql;
  p_grantee.MakeLower();

  // MySQL does not know the concept of "PUBLIC"
  if(p_grantee.Compare("public"))
  {
    sql.Format("REVOKE %s ON %s FROM %s", p_privilege.GetString(), p_objectname.GetString(), p_grantee.GetString());
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
SQLInfoMySQL::GetPSMProcedureExists(XString p_schema, XString p_procedure) const
{
  XString sql;
  sql = "SELECT SELECT COUNT(*)\n"
        "  FROM information_schema.routines\n";
  if(!p_schema.IsEmpty())
  { 
    sql += "   AND routine_schema = '" + p_schema + "'\n";
  }
  if(!p_procedure.IsEmpty())
  {
    sql += "   AND routine_name = '" + p_procedure + "'";
  }
  return sql;
}

XString
SQLInfoMySQL::GetPSMProcedureList(XString& p_schema) const
{
  XString sql;
  sql = "SELECT routine_catalog\n"
        "      ,routine_schema\n"
        "      ,routine_name\n"
        "  FROM information_schema.routines fun\n";
  if (!p_schema.IsEmpty())
  {
    sql += " WHERE routine_schema = ?\n";
  }
  sql += " ORDER BY 1,2,3";

  return sql;
}

XString
SQLInfoMySQL::GetPSMProcedureAttributes(XString& p_schema,XString& p_procedure) const
{
  XString sql;
  sql = "SELECT routine_catalog\n"
        "      ,routine_schema\n"
        "      ,routine_name\n"
        "      ,(SELECT COUNT(*)\n"
        "          FROM information_schema.parameters par\n"
        "         WHERE par.specific_name    = fun.specific_name\n"
        "           AND par.specific_catalog = fun.routine_catalog\n"
        "           AND par.specific_schema  = fun.routine_schema\n"
        "           AND par.parameter_mode IN ('IN','INOUT')) as input_parameters\n"
        "      ,(SELECT COUNT(*)\n"
        "          FROM information_schema.parameters par\n"
        "         WHERE par.specific_name    = fun.specific_name\n"
        "           AND par.specific_catalog = fun.routine_catalog\n"
        "           AND par.specific_schema  = fun.routine_schema\n"
        "           AND par.parameter_mode IN ('OUT','INOUT')) as output_parameters\n"
        "      ,(SELECT COUNT(*)\n"
        "          FROM information_schema.parameters par\n"
        "         WHERE par.specific_name    = fun.specific_name\n"
        "           AND par.specific_catalog = fun.routine_catalog\n"
        "           AND par.specific_schema  = fun.routine_schema\n"
        "           AND par.data_type        = 'set') as result_sets\n"
        "      ,'' as remarks\n"
        "      ,CASE routine_type\n"
        "            WHEN 'PROCEDURE' THEN 1\n"
        "            WHEN 'FUNCTION'  THEN 2\n"
        "                             ELSE 3\n"
        "       end as procedure_type\n"
        "      ,routine_definition\n"
        "  FROM information_schema.routines fun\n";
  if(!p_schema.IsEmpty())
  { 
    sql += " WHERE routine_schema = ?\n";
  }
  if(!p_procedure.IsEmpty())
  {
    sql += p_schema.IsEmpty() ? " WHERE " : "   AND ";
    sql += "routine_name ";
    sql += p_procedure.Find('%') >= 0 ? "LIKE" : "=";
    sql += " ?\n";
  }
  sql += " ORDER BY 1,2,3";
  return sql;
}

XString
SQLInfoMySQL::GetPSMProcedureSourcecode(XString p_schema, XString p_procedure) const
{
  // Source-code already gotten with attributes
  return "";
}

XString
SQLInfoMySQL::GetPSMProcedureCreate(MetaProcedure& /*p_procedure*/) const
{
  return "";
}

XString
SQLInfoMySQL::GetPSMProcedureDrop(XString /*p_schema*/, XString p_procedure) const
{
  return "";
}

XString
SQLInfoMySQL::GetPSMProcedureErrors(XString p_schema,XString p_procedure) const
{
  return "";
}

// And it's parameters
XString
SQLInfoMySQL::GetPSMProcedureParameters(XString& p_schema,XString& p_procedure) const
{
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
        "      ,case par.data_type\n"
        "            when 'varchar'  then 1\n"
        "            when 'int'      then 4\n"
        "            when 'decimal'  then 2\n"
        "            when 'tinyint'  then -6\n"
        "            when 'bigint'   then -5\n"
        "            when 'text'     then 1\n"
        "            when 'longtext' then -1\n"
        "            when 'datetime' then 11\n"
        "                            else 1\n"
        "       end as datatype\n"
        "      ,par.data_type as typename\n"
        "      ,par.character_maximum_length\n"
        "      ,par.numeric_precision\n"
        "      ,par.numeric_scale\n"
        "      ,10 as numeric_precision_radix\n"
        "      ,1 as is_nullable\n"
        "      ,'' as remarks\n"
        "      ,'' as parameter_default\n"
        "      ,1 as datatype3\n"
        "      ,par.datetime_precision as subtype\n"
        "      ,par.character_octet_length\n"
        "      ,par.ordinal_position\n"
        "      ,'YES' as isNullable\n"
        "  FROM information_schema.parameters par\n"
        "      ,information_schema.routines fun\n"
        " WHERE par.specific_catalog = fun.routine_catalog\n"
        "   AND par.specific_schema  = fun.routine_schema\n"
        "   AND par.specific_name    = fun.specific_name\n";
  if(!p_schema.IsEmpty())
  {
    sql += "   AND fun.routine_schema = ?\n";
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
SQLInfoMySQL::GetPSMDeclaration(bool    /*p_first*/
                               ,XString p_variable
                               ,int     p_datatype
                               ,int     p_precision /*= 0 */
                               ,int     p_scale     /*= 0 */
                               ,XString p_default   /*= ""*/
                               ,XString /*p_domain    = ""*/
                               ,XString /*p_asColumn  = ""*/) const
{
  XString line;
  line.Format("DECLARE %s ",p_variable.GetString());

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
  line += ";\n";
  return line;
}

XString
SQLInfoMySQL::GetPSMAssignment(XString p_variable,XString p_statement /*=""*/) const
{
  XString line(p_variable);
  line += " [?=] ";
  if(!p_statement.IsEmpty())
  {
    line += p_statement;
    line += ";";
  }
  return line;
}

XString
SQLInfoMySQL::GetPSMIF(XString p_condition) const
{
  XString line("IF (");
  line += p_condition;
  line += ") THEN\n";
  return line;
}

XString
SQLInfoMySQL::GetPSMIFElse() const
{
  return "ELSE\n";
}

XString
SQLInfoMySQL::GetPSMIFEnd() const
{
  return "END IF;\n";
}

XString
SQLInfoMySQL::GetPSMWhile(XString p_condition) const
{
  return "WHILE (" + p_condition + ") DO\n";
}

XString
SQLInfoMySQL::GetPSMWhileEnd() const
{
  return "END WHILE;\n";
}

XString
SQLInfoMySQL::GetPSMLOOP() const
{
  return "LOOP\n";
}

XString
SQLInfoMySQL::GetPSMLOOPEnd() const
{
  return "END LOOP;\n";
}

XString
SQLInfoMySQL::GetPSMBREAK() const
{
  return "LEAVE\n";
}

XString
SQLInfoMySQL::GetPSMRETURN(XString p_statement /*= ""*/) const
{
  return "RETURN " + p_statement;
}

XString
SQLInfoMySQL::GetPSMExecute(XString p_procedure,MParameterMap& p_parameters) const
{
  XString line;
  line.Format("EXECUTE %s USING ",p_procedure.GetString());
  bool doMore = false;

  for(auto& param : p_parameters)
  {
    if(doMore) line += ",";
    doMore = true;

    line += "@";
    line += param.m_parameter;
  }
  line += ";\n";
  return line;
}

// The CURSOR
XString
SQLInfoMySQL::GetPSMCursorDeclaration(XString p_cursorname,XString p_select) const
{
  return "DECLARE " + p_cursorname + " CURSOR FOR " + p_select + ";";
}

XString
SQLInfoMySQL::GetPSMCursorFetch(XString p_cursorname,std::vector<XString>& /*p_columnnames*/,std::vector<XString>& p_variablenames) const
{
  XString query = "FETCH " + p_cursorname + " INTO ";
  bool moreThenOne = false;

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
SQLInfoMySQL::GetPSMExceptionCatchNoData() const
{
  return "DECLARE EXIT HANDLER FOR SQLSTATE '02000'\n";
  // Must be followed by a (block)statement
}

XString
SQLInfoMySQL::GetPSMExceptionCatch(XString p_sqlState) const
{
  return "DECLARE EXIT HANDLER FOR SQLSTATE '" + p_sqlState + "'\n";
  // Must be followed by a (block)statement
}

XString
SQLInfoMySQL::GetPSMExceptionRaise(XString p_sqlState) const
{
  return "SIGNAL SQLSTATE '" + p_sqlState + "'\n";
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
SQLInfoMySQL::GetSESSIONMyself() const
{
  return "";
}

XString
SQLInfoMySQL::GetSESSIONExists(XString p_sessionID) const
{
  return "";
}

XString
SQLInfoMySQL::GetSESSIONList() const
{
  return "";
}

XString
SQLInfoMySQL::GetSESSIONAttributes(XString p_sessionID) const
{
  return "";
}

//////////////////////////////////////////////////////////////////////////
// Transactions

XString
SQLInfoMySQL::GetSESSIONConstraintsDeferred() const
{
  // MySQL cannot defer constraints
  return "";
}

XString
SQLInfoMySQL::GetSESSIONConstraintsImmediate() const
{
  // MySQL constraints are always active
  return "";
}

//////////////////////////////////////////////////////////////////////////
//
// Call FUNCTION/PROCEDURE from within program
// As a RDBMS dependent extension of "DoSQLCall" of the SQLQuery object
//
//////////////////////////////////////////////////////////////////////////

// Calling a stored function or procedure if the RDBMS does not support ODBC call escapes
SQLVariant*
SQLInfoMySQL::DoSQLCall(SQLQuery* /*p_query*/,XString& /*p_schema*/,XString& /*p_procedure*/)
{
  return nullptr;
}

// Calling a stored function with named parameters, returning a value
SQLVariant*
SQLInfoMySQL::DoSQLCallNamedParameters(SQLQuery* /*p_query*/,XString& /*p_schema*/,XString& /*p_procedure*/)
{
  return nullptr;
}

// End of namespace
}
