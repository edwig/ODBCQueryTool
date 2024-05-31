////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoMySQL.cpp
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

// RDBMS Uses INDENTITY or SEQUENCE interface
void
SQLInfoMySQL::SetUseSequences(bool /*p_sequences*/)
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
SQLInfoMySQL::GetRDBMSDatabaseType() const
{
  return RDBMS_MYSQL;
}

// The name of the database vendor
XString
SQLInfoMySQL::GetRDBMSVendorName() const
{
  // The name of the database vendor
  return _T("MySQL A.B. / Oracle");
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

// Supports the ODBC call procedure with named parameters
bool
SQLInfoMySQL::GetRDBMSSupportsODBCCallNamedParameters() const
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

// Maximum for a VARCHAR to be handled without AT-EXEC data. Assume NVARCHAR is half that size!
int
SQLInfoMySQL::GetRDBMSMaxVarchar() const
{
  // See: https://dev.mysql.com/doc/refman/8.0/en/char.html#:~:text=Values%20in%20VARCHAR%20columns%20are,and%20the%20character%20set%20used.
  return 65535;
}

// KEYWORDS

// Keyword for the current date and time
XString
SQLInfoMySQL::GetKEYWORDCurrentTimestamp() const
{
  return _T("current_timestamp");
}

// String for the current date
XString
SQLInfoMySQL::GetKEYWORDCurrentDate() const
{
  return _T("current_date");
}

// Get the concatenation operator
XString
SQLInfoMySQL::GetKEYWORDConcatanationOperator() const
{
  // BEWARE: Officially the concatenation operator is "CONCAT(string1,string2)"
  // MYSQL supports 'one' 'two' concatenation of two strings (no operator)
  return _T("");
}

// Get quote character for strings
XString
SQLInfoMySQL::GetKEYWORDQuoteCharacter() const
{
  return _T("\'");
}

// Get quote character around reserved words as an identifier
XString
SQLInfoMySQL::GetKEYWORDReservedWordQuote() const
{
  return _T("`");
}

// Get default NULL for parameter list input
XString
SQLInfoMySQL::GetKEYWORDParameterDefaultNULL() const
{
  // Standard, no definition defines the NULL state
  return _T("");
}

// Parameter is for INPUT and OUTPUT in parameter list
XString
SQLInfoMySQL::GetKEYWORDParameterINOUT() const
{
  return _T("");
}

// Parameter is for OUTPUT only in parameter list
XString
SQLInfoMySQL::GetKEYWORDParameterOUT() const
{
  return _T("");
}

// Get datatype of the IDENTITY primary key in a Network database
XString
SQLInfoMySQL::GetKEYWORDNetworkPrimaryKeyType() const
{
  // Use SEQUENCE to fill!
  return _T("INTEGER");
}

// Get datatype for timestamp (year to second)
XString
SQLInfoMySQL::GetKEYWORDTypeTimestamp() const
{
  return _T("TIMESTAMP");
}

// Prefix for a parameter in a stored procedure
XString
SQLInfoMySQL::GetKEYWORDParameterPrefix() const
{
  return _T("");
}

// Get select part to add new record identity to a table
// Can be special column like 'OID' or a sequence select
XString
SQLInfoMySQL::GetKEYWORDIdentityString(XString& p_tablename,XString p_postfix /*= "_seq"*/) const
{
  return p_tablename + p_postfix + _T(".nextval");
}

// Gets the UPPER function
XString
SQLInfoMySQL::GetKEYWORDUpper(XString& p_expression) const
{
  return _T("UPPER(") + p_expression + _T(")");
}

// Gets the construction for 1 minute ago
XString
SQLInfoMySQL::GetKEYWORDInterval1MinuteAgo() const
{
  return _T("ERROR");
}

// Gets the Not-NULL-Value statement of the database
XString
SQLInfoMySQL::GetKEYWORDStatementNVL(XString& p_test,XString& p_isnull) const
{
  return _T("{fn IFNULL(") + p_test + _T(",") + p_isnull + _T(")}");
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
  return _T("CURRENT_USER");
}

// Connects to a default schema in the database/instance
XString
SQLInfoMySQL::GetSQLDefaultSchema(XString /*p_user*/,XString p_schema) const
{
  return _T("USE ") + p_schema;
}

// Gets the construction for inline generating a key within an INSERT statement
XString
SQLInfoMySQL::GetSQLNewSerial(XString /*p_table*/, XString /*p_sequence*/) const
{
  // Insert a zero in an IDENTITY column
  return _T("0");
}

// Gets the construction / select for generating a new serial identity
XString
SQLInfoMySQL::GetSQLGenerateSerial(XString p_table) const
{
  // NO WAY OF KNOWNING THIS
  return _T("0");
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
  return XString(_T(""));
}

XString
SQLInfoMySQL::GetSQLCommitSubTransaction(XString p_savepointName) const
{
  // Generic ODBC does not known about sub transactions!
  return XString(_T(""));
}

XString
SQLInfoMySQL::GetSQLRollbackSubTransaction(XString p_savepointName) const
{
  // Generic ODBC does not known about sub transactions!
  return XString(_T(""));
}

// FROM-Part for a query to select only 1 (one) record / or empty!
XString
SQLInfoMySQL::GetSQLFromDualClause() const
{
  // MySQL does bare SELECT!
  return _T("");
}

// Get SQL to lock  a table 
XString
SQLInfoMySQL::GetSQLLockTable(XString /*p_schema*/,XString p_tablename,bool p_exclusive,int /*p_waittime*/) const
{
  // Standard ISO SQL Syntax
  XString query = _T("LOCK TABLE ") + p_tablename + _T(" IN ");
  query += p_exclusive ? _T("EXCLUSIVE") : _T("SHARE");
  query += _T(" MODE");
  return query;
}

// Get query to optimize the table statistics
XString
SQLInfoMySQL::GetSQLOptimizeTable(XString p_schema, XString p_tablename) const
{
  // To be implemented
  return _T("");
}

// Transform query to select top <n> rows
XString
SQLInfoMySQL::GetSQLTopNRows(XString p_sql,int p_top,int p_skip /*= 0*/) const
{
  if(p_top > 0)
  {
    // MYSQL: " LIMIT <top> [ OFFSET <skip> ]
    XString limit;
    limit.Format(_T("\n LIMIT %d"),p_top);
    if(p_skip > 0)
    {
      limit.AppendFormat(_T(" OFFSET %d"),p_skip);
    }
    p_sql += limit;
  }
  return p_sql;
}

// Expand a SELECT with an 'FOR UPDATE' lock clause
XString
SQLInfoMySQL::GetSelectForUpdateTableClause(unsigned /*p_lockWaitTime*/) const
{
  return "";
}

XString
SQLInfoMySQL::GetSelectForUpdateTrailer(XString p_select,unsigned /*p_lockWaitTime*/) const
{
  return p_select + "\nFOR UPDATE";
}

// Query to perform a keep alive ping
XString
SQLInfoMySQL::GetPing() const
{
  // Getting the time does a ping
  return _T("SELECT current_timestamp");
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
  s.Replace(_T("'"),_T("''"));
  XString kwoot = GetKEYWORDQuoteCharacter();
  return  kwoot + s + kwoot;
}

// Get date string in engine format
XString
SQLInfoMySQL::GetSQLDateString(int p_year,int p_month,int p_day) const
{
  XString dateString;
  dateString.Format(_T("{d '%04d-%02d-%02d'}"),p_year,p_month,p_day);
  return dateString;
}

// Get time string in database engine format
XString
SQLInfoMySQL::GetSQLTimeString(int p_hour,int p_minute,int p_second) const
{
  XString retval;
  retval.Format(_T("{t '%02d:%02d:%02d'}"),p_hour,p_minute,p_second);
  return retval;
}

// Get date-time string in database engine format
XString
SQLInfoMySQL::GetSQLDateTimeString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  XString string;
  string.Format(_T("{ts '%04d-%02d-%02d %02d:%02d:%02d'}")
                ,p_year,p_month,p_day // ODBC Ordering !!
                ,p_hour,p_minute,p_second);
  return string;
}

// Get date-time bound parameter string in database format
XString
SQLInfoMySQL::GetSQLDateTimeBoundString() const
{
  return _T("{ts ?}");
}

// Stripped data for the parameter binding
XString
SQLInfoMySQL::GetSQLDateTimeStrippedString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  XString string;
  string.Format(_T("%04d-%02d-%02d %02d:%02d:%02d")
                ,p_year,p_month,p_day // ODBC Ordering !!
                ,p_hour,p_minute,p_second);
  return string;
}

// Makes an catalog identifier string (possibly quoted on both sides)
XString
SQLInfoMySQL::GetSQLDDLIdentifier(XString p_identifier) const
{
  return p_identifier;
}

// Get the name of a temp table (local temporary or global temporary)
XString
SQLInfoMySQL::GetTempTablename(XString /*p_schema*/,XString p_tablename,bool /*p_local*/) const
{
  return p_tablename;
}

// Changes to parameters before binding to an ODBC HSTMT handle
void
SQLInfoMySQL::DoBindParameterFixup(SQLSMALLINT& /*p_sqlDatatype*/,SQLULEN& /*p_columnSize*/,SQLSMALLINT& /*p_scale*/,SQLLEN& /*p_bufferSize*/,SQLLEN* /*p_indicator*/) const
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
// Standard ODBC functions are good enough
XString
SQLInfoMySQL::GetCATALOGMetaTypes(int p_type) const
{
  UNREFERENCED_PARAMETER(p_type);
  return _T("");
}

XString
SQLInfoMySQL::GetCATALOGDefaultCharset() const
{
  return _T("Latin1");
}

XString
SQLInfoMySQL::GetCATALOGDefaultCharsetNCV() const
{
  return _T("UTF-8");
}

XString
SQLInfoMySQL::GetCATALOGDefaultCollation() const
{
  return _T("latin1_swedish_ci");
}

// Get SQL to check if a table already exists in the database
XString
SQLInfoMySQL::GetCATALOGTableExists(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  // Still to do in MySQL
  return _T("");
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
  sql = _T("SELECT table_catalog\n")
        _T("      ,table_schema\n")
        _T("      ,table_name\n")
        _T("      ,case table_type\n")
        _T("            when 'BASE TABLE' THEN 'TABLE'\n")
        _T("            else 'UNKNOWN'\n")
        _T("       end as table_type\n")
        _T("      ,table_comment\n")
        _T("      ,table_catalog || '.' || table_schema || '.' || table_name as fullname\n")
        _T("      ,engine\n")
        _T("      ,0 as temporary\n") 
        _T("  FROM information_schema.tables\n")  
        _T(" WHERE table_type = 'BASE TABLE'\n")
        _T("   AND table_schema NOT IN ('mysql','sys','performance_schema')\n");
  if(!p_schema)
  {
    sql += _T("   AND table_schema = ?\n");
  }
  if(!p_tablename.IsEmpty())
  {
    sql += _T("   AND table_name ");
    sql += p_tablename.Find('%') >= 0 ? _T("LIKE") : _T("=");
    sql += _T(" ?\n");
  }
  sql += _T(" ORDER BY 1,2,3");
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
  sql = _T("SELECT table_catalog\n")
        _T("      ,table_schema\n")
        _T("      ,table_name\n")
        _T("      ,'SYSTEM TABLE' as table_type\n")
        _T("      ,table_comment\n")
        _T("      ,table_catalog || '.' || table_schema || '.' || table_name as fullname\n")
        _T("      ,engine\n")
        _T("      ,0 as temporary\n") 
        _T("  FROM information_schema.tables\n")  
        _T(" WHERE ( table_type = 'SYSTEM VIEW'\n")
        _T("      OR table_schema IN ('mysql','sys','performance_schema'))\n");
  if(!p_tablename.IsEmpty())
  {
    sql += _T("   AND table_name ");
    sql += p_tablename.Find('%') >= 0 ? _T("LIKE") : _T("=");
    sql += _T(" ?\n");
  }
  sql += _T(" ORDER BY 1,2,3");
  return sql;}

XString
SQLInfoMySQL::GetCATALOGTableCreate(MetaTable& p_table,MetaColumn& /*p_column*/) const
{
  XString sql = _T("CREATE ");
  if (p_table.m_temporary)
  {
    sql += _T("TEMPORARY ");
  }
  sql += _T("TABLE ") + p_table.m_table;
  return sql;
}

XString
SQLInfoMySQL::GetCATALOGTableCreatePostfix(MetaTable& /*p_table*/,MetaColumn& /*p_column*/) const
{
  return _T("");
}

XString
SQLInfoMySQL::GetCATALOGTableRename(XString /*p_schema*/,XString p_tablename,XString p_newname) const
{
  XString sql(_T("RENAME TABLE") + p_tablename + _T(" TO ") + p_newname);
  return sql;
}

XString
SQLInfoMySQL::GetCATALOGTableDrop(XString /*p_schema*/,XString p_tablename,bool p_ifExist /*= false*/,bool p_restrict /*= false*/,bool p_cascade /*= false*/) const
{
  XString sql(_T("DROP TABLE "));
  if (p_ifExist)
  {
    sql += _T("IF EXISTS ");
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
SQLInfoMySQL::GetCATALOGTemptableCreate(XString /*p_schema*/,XString p_tablename,XString p_select) const
{
  // BEWARE: THIS IS A GUESS. 
  return _T("CREATE TEMPORARY TABLE ") + p_tablename + _T("\nAS ") + p_select;
}

XString 
SQLInfoMySQL::GetCATALOGTemptableIntoTemp(XString /*p_schema*/,XString p_tablename,XString p_select) const
{
  return _T("INSERT INTO ") + p_tablename + _T("\n") + p_select;
}

XString 
SQLInfoMySQL::GetCATALOGTemptableDrop(XString /*p_schema*/,XString p_tablename) const
{
  return _T("DROP TABLE ") + p_tablename;
}

//////////////////////////////////////////////////////////////////////////
// ALL COLUMN FUNCTIONS

XString 
SQLInfoMySQL::GetCATALOGColumnExists(XString /*p_schema*/,XString p_tablename,XString p_columnname) const
{
  return _T("");
}

XString 
SQLInfoMySQL::GetCATALOGColumnList(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  // Standard ODBC driver suffices
  return _T("");
}

XString 
SQLInfoMySQL::GetCATALOGColumnAttributes(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_columnname*/) const
{
  // Standard ODBC driver suffices
  return _T("");
}

XString 
SQLInfoMySQL::GetCATALOGColumnCreate(MetaColumn& p_column) const
{
  XString sql = _T("ALTER TABLE ")  + p_column.m_table  + _T("\n")
                _T("  ADD COLUMN ") + p_column.m_column + _T(" ") + p_column.m_typename;
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
  XString sql = _T("ALTER  TABLE  ") + p_column.m_table + _T("\n")
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
SQLInfoMySQL::GetCATALOGColumnRename(XString p_schema,XString p_tablename,XString p_columnname,XString p_newname,XString /*p_datatype*/) const
{
  // General ISO syntax
  XString sql(_T("ALTER  TABLE  ") + p_tablename + _T("\n")
              _T("RENAME ") + p_columnname + _T(" TO ") + p_newname + _T("\n"));
  return sql;
}

XString 
SQLInfoMySQL::GetCATALOGColumnDrop(XString p_schema,XString p_tablename,XString p_columnname) const
{
  XString sql(_T("ALTER TABLE  ") + p_tablename + _T("\n")
              _T(" DROP COLUMN ") + p_columnname);
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
  return _T("");
}

XString
SQLInfoMySQL::GetCATALOGIndexList(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLStatistics instead (see SQLInfo class)
  return _T("");
}

XString
SQLInfoMySQL::GetCATALOGIndexAttributes(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_indexname*/) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLStatistics instead (see SQLInfo class)
  return _T("");
}

XString
SQLInfoMySQL::GetCATALOGIndexCreate(MIndicesMap& p_indices,bool /*p_duplicateNulls /*= false*/) const
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
SQLInfoMySQL::GetCATALOGIndexDrop(XString /*p_schema*/,XString /*p_tablename*/,XString p_indexname) const
{
  XString sql = _T("DROP INDEX ") + p_indexname;
  return sql;
}

// Get extra filter expression for an index column
XString
SQLInfoMySQL::GetCATALOGIndexFilter(MetaIndex& /*p_index*/) const
{
  return _T("");
}

//////////////////////////////////////////////////////////////////////////
// ALL PRIMARY KEY FUNCTIONS

XString
SQLInfoMySQL::GetCATALOGPrimaryExists(XString /*p_schema*/,XString p_tablename) const
{
  // To be implemented
  return _T("");
}

XString
SQLInfoMySQL::GetCATALOGPrimaryAttributes(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  // To be implemented
  return _T("");
}

XString
SQLInfoMySQL::GetCATALOGPrimaryCreate(MPrimaryMap& p_primaries) const
{
  XString query(_T("ALTER TABLE "));

  for(const auto& prim : p_primaries)
  {
    if(prim.m_columnPosition == 1)
    {
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
SQLInfoMySQL::GetCATALOGPrimaryDrop(XString /*p_schema*/,XString p_tablename,XString p_constraintname) const
{
  XString sql(_T("ALTER TABLE ") + p_tablename + _T("\n")
              _T(" DROP CONSTRAINT ") + p_constraintname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL FOREIGN KEY FUNCTIONS

XString
SQLInfoMySQL::GetCATALOGForeignExists(XString /*p_schema*/,XString p_tablename,XString p_constraintname) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLForeignKeys instead (see SQLInfo class)
  return _T("");
}

XString
SQLInfoMySQL::GetCATALOGForeignList(XString& /*p_schema*/,XString& /*p_tablename*/,int /*p_maxColumns*/ /*=SQLINFO_MAX_COLUMNS*/) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLForeignKeys instead (see SQLInfo class)
  return _T("");
}

XString
SQLInfoMySQL::GetCATALOGForeignAttributes(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_constraintname*/,bool /*p_referenced = false*/,int /*p_maxColumns*/ /*=SQLINFO_MAX_COLUMNS*/) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLForeignKeys instead (see SQLInfo class)
  return _T("");
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
  XString query = _T("ALTER TABLE ") + table + _T("\n")
                  _T("  ADD CONSTRAINT ") + foreign.m_foreignConstraint + _T("\n")
                  _T("      FOREIGN KEY (");

  // Add the foreign key columns
  bool extra = false;
  for(const auto& key : p_foreigns)
  {
    if(extra) query += _T(",");
    query += key.m_fkColumnName;
    extra = true;
  }

  // Add references primary table
  query += _T(")\n      REFERENCES ") + primary + _T("(");

  // Add the primary key columns
  extra = false;
  for(const auto& key : p_foreigns)
  {
    if(extra) query += _T(",");
    query += key.m_pkColumnName;
    extra = true;
  }
  query += _T(")");

  // Add all relevant options
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
    case SQL_CASCADE:     query += _T("\n      ON UPDATE CASCADE");     break;
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
SQLInfoMySQL::GetCATALOGForeignAlter(MForeignMap& p_original, MForeignMap& p_requested) const
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

  // The base foreign key command
  XString query = _T("ALTER TABLE ") + table + _T("\n")
                  _T("ALTER CONSTRAINT ") + original.m_foreignConstraint + _T("\n");

  // Add all relevant options
  if(original.m_deferrable != requested.m_deferrable)
  {
    switch(requested.m_deferrable)
    {
      case SQL_INITIALLY_DEFERRED:  query += _T("\n      INITIALLY DEFERRED"); break;
      case SQL_INITIALLY_IMMEDIATE: query += _T("\n      DEFERRABLE");         break;
      case SQL_NOT_DEFERRABLE:      query += _T("\n      NOT DEFERRABLE");     break;
      default:                      break;
    }
  }
  if(original.m_match != requested.m_match)
  {
    switch(requested.m_match)
    {
      case SQL_MATCH_FULL:    query += _T("\n      MATCH FULL");    break;
      case SQL_MATCH_PARTIAL: query += _T("\n      MATCH PARTIAL"); break;
      case SQL_MATCH_SIMPLE:  query += _T("\n      MATCH SIMPLE");  break;
    }
  }
  if(original.m_updateRule != requested.m_updateRule)
  {
    switch(requested.m_updateRule)
    {
      case SQL_CASCADE:     query += _T("\n      ON UPDATE CASCADE");     break;
      case SQL_SET_NULL:    query += _T("\n      ON UPDATE SET NULL");    break;
      case SQL_SET_DEFAULT: query += _T("\n      ON UPDATE SET DEFAULT"); break;
      case SQL_NO_ACTION:   query += _T("\n      ON UPDATE NO ACTION");   break;
      default:              // In essence: ON UPDATE RESTRICT, but that's already the default
      case SQL_RESTRICT:    break;
    }
  }
  if(original.m_deleteRule != requested.m_deleteRule)
  {
    switch(requested.m_deleteRule)
    {
      case SQL_CASCADE:     query += _T("\n      ON DELETE CASCADE");     break;
      case SQL_SET_NULL:    query += _T("\n      ON DELETE SET NULL");    break;
      case SQL_SET_DEFAULT: query += _T("\n      ON DELETE SET DEFAULT"); break;
      case SQL_NO_ACTION:   query += _T("\n      ON DELETE NO ACTION");   break;
      default:              // In essence: ON DELETE RESTRICT, but that's already the default
      case SQL_RESTRICT:    break;
    }
  }
  return query;
}

XString
SQLInfoMySQL::GetCATALOGForeignDrop(XString /*p_schema*/,XString p_tablename,XString p_constraintname) const
{
  XString sql(_T("ALTER TABLE ") + p_tablename + _T("\n")
              _T(" DROP CONSTRAINT ") + p_constraintname);
  return sql;
}

//////////////////////////
// All default constraints
XString
SQLInfoMySQL::GetCATALOGDefaultExists(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_column*/) const
{
  return _T("");
}

XString
SQLInfoMySQL::GetCATALOGDefaultList(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  return _T("");
}

XString
SQLInfoMySQL::GetCATALOGDefaultAttributes(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_column*/) const
{
  return _T("");
}

XString
SQLInfoMySQL::GetCATALOGDefaultCreate(XString /*p_schema*/,XString /*p_tablename*/,XString /*p_constraint*/,XString /*p_column*/,XString /*p_code*/) const
{
  return _T("");
}

XString
SQLInfoMySQL::GetCATALOGDefaultDrop(XString /*p_schema*/,XString /*p_tablename*/,XString /*p_constraint*/) const
{
  return _T("");
}

/////////////////////////
// All check constraints

XString
SQLInfoMySQL::GetCATALOGCheckExists(XString  /*p_schema*/,XString  /*p_tablename*/,XString  /*p_constraint*/) const
{
  return _T("");
}

XString
SQLInfoMySQL::GetCATALOGCheckList(XString  /*p_schema*/,XString  /*p_tablename*/) const
{
  return _T("");
}

XString
SQLInfoMySQL::GetCATALOGCheckAttributes(XString  /*p_schema*/,XString  /*p_tablename*/,XString  /*p_constraint*/) const
{
  return _T("");
}

XString
SQLInfoMySQL::GetCATALOGCheckCreate(XString  /*p_schema*/,XString  /*p_tablename*/,XString  /*p_constraint*/,XString /*p_condition*/) const
{
  return _T("");
}

XString
SQLInfoMySQL::GetCATALOGCheckDrop(XString  /*p_schema*/,XString  /*p_tablename*/,XString  /*p_constraint*/) const
{
  return _T("");
}

//////////////////////////////////////////////////////////////////////////
// ALL TRIGGER FUNCTIONS

XString
SQLInfoMySQL::GetCATALOGTriggerExists(XString /*p_schema*/, XString p_tablename, XString p_triggername) const
{
  return _T("");
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
  sql = _T("SELECT event_object_catalog\n")
        _T("      ,event_object_schema\n")
        _T("      ,event_object_table\n")
        _T("      ,trigger_name\n")
        _T("      ,'' AS trigger_remarks\n")
        _T("      ,action_order  AS trigger_position\n")
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
        _T("      ,FALSE AS trigger_select\n")
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
SQLInfoMySQL::GetCATALOGTriggerCreate(MetaTrigger& /*p_trigger*/) const
{
  return _T("");
}

XString
SQLInfoMySQL::GetCATALOGTriggerDrop(XString /*p_schema*/, XString p_tablename, XString p_triggername) const
{
  return _T("");
}

//////////////////////////////////////////////////////////////////////////
// ALL SEQUENCE FUNCTIONS

XString
SQLInfoMySQL::GetCATALOGSequenceExists(XString /*p_schema*/, XString p_sequence) const
{
  return _T("");
}

XString
SQLInfoMySQL::GetCATALOGSequenceList(XString& /*p_schema*/,XString& /*p_pattern*/) const
{
  return _T("");
}

XString
SQLInfoMySQL::GetCATALOGSequenceAttributes(XString& /*p_schema*/,XString& /*p_sequence*/) const
{
  return _T("");
}

XString
SQLInfoMySQL::GetCATALOGSequenceCreate(MetaSequence& /*p_sequence*/) const
{
  return _T("");
}

XString
SQLInfoMySQL::GetCATALOGSequenceDrop(XString /*p_schema*/, XString p_sequence) const
{
  return _T("");
}

//////////////////////////////////////////////////////////////////////////
// ALL VIEW FUNCTIONS

XString 
SQLInfoMySQL::GetCATALOGViewExists(XString& /*p_schema*/,XString& /*p_viewname*/) const
{
  return _T("");
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
  sql = _T("SELECT table_catalog\n")
        _T("      ,table_schema\n")
        _T("      ,table_name\n")
        _T("      ,'VIEW' as table_type\n")
        _T("      ,table_comment\n")
        _T("      ,table_catalog || '.' || table_schema || '.' || table_name as fullname\n")
        _T("      ,engine\n")
        _T("      ,0 as temporary\n") 
        _T("  FROM information_schema.tables\n")  
        _T(" WHERE table_type = 'VIEW'\n")
        _T("   AND table_schema NOT IN ('mysql','sys','performance_schema')\n");
  if(!p_schema)
  {
    sql += _T("   AND table_schema = ?\n");
  }
  if(!p_viewname.IsEmpty())
  {
    sql += _T("   AND table_name ");
    sql += p_viewname.Find('%') >= 0 ? _T("LIKE") : _T("=");
    sql += _T(" ?\n");
  }
  sql += _T(" ORDER BY 1,2,3");
  return sql;
}

XString
SQLInfoMySQL::GetCATALOGViewText(XString& /*p_schema*/,XString& /*p_viewname*/) const
{
  // Cannot query this, Use ODBC functions
  return _T("");
}

XString
SQLInfoMySQL::GetCATALOGViewCreate(XString /*p_schema*/,XString p_viewname,XString p_contents,bool /*p_ifexists /*= true*/) const
{
  return _T("CREATE VIEW ") + p_viewname + _T("\n") + p_contents;
}

XString 
SQLInfoMySQL::GetCATALOGViewRename(XString p_schema,XString p_viewname,XString p_newname)    const
{
  return _T("");
}

XString 
SQLInfoMySQL::GetCATALOGViewDrop(XString /*p_schema*/,XString p_viewname,XString& p_precursor) const
{
  p_precursor.Empty();
  return _T("DROP VIEW ") + p_viewname;
}

// All Privilege functions
XString
SQLInfoMySQL::GetCATALOGTablePrivileges(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  return _T("");
}

XString 
SQLInfoMySQL::GetCATALOGColumnPrivileges(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_columnname*/) const
{
  return _T("");
}

XString
SQLInfoMySQL::GetCATALOGSequencePrivilege(XString& /*p_schema*/,XString& /*p_sequence*/) const
{
  return _T("");
}

XString 
SQLInfoMySQL::GetCATALOGGrantPrivilege(XString /*p_schema*/,XString p_objectname,XString p_privilege,XString p_grantee,bool p_grantable)
{
  XString sql;
  p_grantee.MakeLower();

  // MySQL does not know the concept of "PUBLIC"
  if(p_grantee.Compare(_T("public")))
  {
    sql.Format(_T("GRANT %s ON %s TO %s"),p_privilege.GetString(),p_objectname.GetString(),p_grantee.GetString());
    if(p_grantable)
    {
      sql += _T(" WITH GRANT OPTION");
    }
  }
  return sql;
}

XString 
SQLInfoMySQL::GetCATALOGRevokePrivilege(XString /*p_schema*/,XString p_objectname,XString p_privilege,XString p_grantee)
{
  XString sql;
  p_grantee.MakeLower();

  // MySQL does not know the concept of "PUBLIC"
  if(p_grantee.Compare(_T("public")))
  {
    sql.Format(_T("REVOKE %s ON %s FROM %s"), p_privilege.GetString(), p_objectname.GetString(), p_grantee.GetString());
  }
  return sql;
}

// All Synonym functions
XString
SQLInfoMySQL::GetCATALOGSynonymList(XString& /*p_schema*/,XString& /*p_pattern*/) const
{
  // Not implemented yet
  return _T("");
}

XString
SQLInfoMySQL::GetCATALOGSynonymAttributes(XString& /*p_schema*/,XString& /*p_synonym*/) const
{
  // Not implemented yet
  return _T("");
}

XString
SQLInfoMySQL::GetCATALOGSynonymCreate(XString& /*p_schema*/,XString& /*p_synonym*/,XString /*p_forObject*/,bool /*p_private = true*/) const
{
  // Not implemented yet
  return _T("");
}

XString
SQLInfoMySQL::GetCATALOGSynonymDrop(XString& /*p_schema*/,XString& /*p_synonym*/,bool /*p_private = true*/) const
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
SQLInfoMySQL::GetPSMProcedureExists(XString p_schema, XString p_procedure) const
{
  XString sql;
  sql = _T("SELECT SELECT COUNT(*)\n")
        _T("  FROM information_schema.routines\n");
  if(!p_schema.IsEmpty())
  { 
    sql += _T("   AND routine_schema = '") + p_schema + _T("'\n");
  }
  if(!p_procedure.IsEmpty())
  {
    sql += _T("   AND routine_name = '") + p_procedure + _T("'");
  }
  return sql;
}

XString
SQLInfoMySQL::GetPSMProcedureList(XString& p_schema) const
{
  XString sql;
  sql = _T("SELECT routine_catalog\n")
        _T("      ,routine_schema\n")
        _T("      ,routine_name\n")
        _T("      ,CASE routine_type\n")
        _T("            WHEN 'PROCEDURE' THEN 1\n")
        _T("            WHEN 'FUNCTION'  THEN 2\n")
        _T("                             ELSE 3\n")
        _T("       end\n")
        _T("  FROM information_schema.routines fun\n");
  if (!p_schema.IsEmpty())
  {
    sql += _T(" WHERE routine_schema = ?\n");
  }
  sql += _T(" ORDER BY 1,2,3");

  return sql;
}

XString
SQLInfoMySQL::GetPSMProcedureAttributes(XString& p_schema,XString& p_procedure) const
{
  XString sql;
  sql = _T("SELECT routine_catalog\n")
        _T("      ,routine_schema\n")
        _T("      ,routine_name\n")
        _T("      ,(SELECT COUNT(*)\n")
        _T("          FROM information_schema.parameters par\n")
        _T("         WHERE par.specific_name    = fun.specific_name\n")
        _T("           AND par.specific_catalog = fun.routine_catalog\n")
        _T("           AND par.specific_schema  = fun.routine_schema\n")
        _T("           AND par.parameter_mode IN ('IN','INOUT')) as input_parameters\n")
        _T("      ,(SELECT COUNT(*)\n")
        _T("          FROM information_schema.parameters par\n")
        _T("         WHERE par.specific_name    = fun.specific_name\n")
        _T("           AND par.specific_catalog = fun.routine_catalog\n")
        _T("           AND par.specific_schema  = fun.routine_schema\n")
        _T("           AND par.parameter_mode IN ('OUT','INOUT')) as output_parameters\n")
        _T("      ,(SELECT COUNT(*)\n")
        _T("          FROM information_schema.parameters par\n")
        _T("         WHERE par.specific_name    = fun.specific_name\n")
        _T("           AND par.specific_catalog = fun.routine_catalog\n")
        _T("           AND par.specific_schema  = fun.routine_schema\n")
        _T("           AND par.data_type        = 'set') as result_sets\n")
        _T("      ,'' as remarks\n")
        _T("      ,CASE routine_type\n")
        _T("            WHEN 'PROCEDURE' THEN 1\n")
        _T("            WHEN 'FUNCTION'  THEN 2\n")
        _T("                             ELSE 3\n")
        _T("       end as procedure_type\n")
        _T("      ,routine_definition\n")
        _T("  FROM information_schema.routines fun\n");
  if(!p_schema.IsEmpty())
  { 
    sql += _T(" WHERE routine_schema = ?\n");
  }
  if(!p_procedure.IsEmpty())
  {
    sql += p_schema.IsEmpty() ? _T(" WHERE ") : _T("   AND ");
    sql += _T("routine_name ");
    sql += p_procedure.Find('%') >= 0 ? _T("LIKE") : _T("=");
    sql += _T(" ?\n");
  }
  sql += _T(" ORDER BY 1,2,3");
  return sql;
}

XString
SQLInfoMySQL::GetPSMProcedureSourcecode(XString p_schema, XString p_procedure) const
{
  // Source-code already gotten with attributes
  return _T("");
}

XString
SQLInfoMySQL::GetPSMProcedureCreate(MetaProcedure& /*p_procedure*/) const
{
  return _T("");
}

XString
SQLInfoMySQL::GetPSMProcedureDrop(XString /*p_schema*/, XString p_procedure,bool /*p_function /*=false*/) const
{
  return _T("");
}

XString
SQLInfoMySQL::GetPSMProcedureErrors(XString p_schema,XString p_procedure) const
{
  return _T("");
}

XString
SQLInfoMySQL::GetPSMProcedurePrivilege(XString& /*p_schema*/,XString& /*p_procedure*/) const
{
  return _T("");
}

// And it's parameters
XString
SQLInfoMySQL::GetPSMProcedureParameters(XString& p_schema,XString& p_procedure) const
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
        _T("      ,case par.data_type\n")
        _T("            when 'varchar'  then 1\n")
        _T("            when 'int'      then 4\n")
        _T("            when 'decimal'  then 2\n")
        _T("            when 'tinyint'  then -6\n")
        _T("            when 'bigint'   then -5\n")
        _T("            when 'text'     then 1\n")
        _T("            when 'longtext' then -1\n")
        _T("            when 'datetime' then 11\n")
        _T("                            else 1\n")
        _T("       end as datatype\n")
        _T("      ,par.data_type as typename\n")
        _T("      ,par.character_maximum_length\n")
        _T("      ,par.numeric_precision\n")
        _T("      ,par.numeric_scale\n")
        _T("      ,10 as numeric_precision_radix\n")
        _T("      ,1 as is_nullable\n")
        _T("      ,'' as remarks\n")
        _T("      ,'' as parameter_default\n")
        _T("      ,1 as datatype3\n")
        _T("      ,par.datetime_precision as subtype\n")
        _T("      ,par.character_octet_length\n")
        _T("      ,par.ordinal_position\n")
        _T("      ,'YES' as isNullable\n")
        _T("  FROM information_schema.parameters par\n")
        _T("      ,information_schema.routines fun\n")
        _T(" WHERE par.specific_catalog = fun.routine_catalog\n")
        _T("   AND par.specific_schema  = fun.routine_schema\n")
        _T("   AND par.specific_name    = fun.specific_name\n");
  if(!p_schema.IsEmpty())
  {
    sql += _T("   AND fun.routine_schema = ?\n");
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
  line.Format(_T("DECLARE %s "),p_variable.GetString());

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
  line += _T(";\n");
  return line;
}

XString
SQLInfoMySQL::GetPSMAssignment(XString p_variable,XString p_statement /*=""*/) const
{
  XString line(p_variable);
  line += _T(" [?=] ");
  if(!p_statement.IsEmpty())
  {
    line += p_statement;
    line += _T(";");
  }
  return line;
}

XString
SQLInfoMySQL::GetPSMIF(XString p_condition) const
{
  XString line(_T("IF ("));
  line += p_condition;
  line += _T(") THEN\n");
  return line;
}

XString
SQLInfoMySQL::GetPSMIFElse() const
{
  return _T("ELSE\n");
}

XString
SQLInfoMySQL::GetPSMIFEnd() const
{
  return _T("END IF;\n");
}

XString
SQLInfoMySQL::GetPSMWhile(XString p_condition) const
{
  return _T("WHILE (") + p_condition + _T(") DO\n");
}

XString
SQLInfoMySQL::GetPSMWhileEnd() const
{
  return _T("END WHILE;\n");
}

XString
SQLInfoMySQL::GetPSMLOOP() const
{
  return _T("LOOP\n");
}

XString
SQLInfoMySQL::GetPSMLOOPEnd() const
{
  return _T("END LOOP;\n");
}

XString
SQLInfoMySQL::GetPSMBREAK() const
{
  return _T("LEAVE\n");
}

XString
SQLInfoMySQL::GetPSMRETURN(XString p_statement /*= ""*/) const
{
  return _T("RETURN ") + p_statement;
}

XString
SQLInfoMySQL::GetPSMExecute(XString p_procedure,MParameterMap& p_parameters) const
{
  XString line;
  line.Format(_T("EXECUTE %s USING "),p_procedure.GetString());
  bool doMore = false;

  for(const auto& param : p_parameters)
  {
    if(doMore) line += _T(",");
    doMore = true;

    line += _T("@");
    line += param.m_parameter;
  }
  line += _T(";\n");
  return line;
}

// The CURSOR
XString
SQLInfoMySQL::GetPSMCursorDeclaration(XString p_cursorname,XString p_select) const
{
  return _T("DECLARE ") + p_cursorname + _T(" CURSOR FOR ") + p_select + _T(";");
}

XString
SQLInfoMySQL::GetPSMCursorFetch(XString p_cursorname,std::vector<XString>& /*p_columnnames*/,std::vector<XString>& p_variablenames) const
{
  XString query = _T("FETCH ") + p_cursorname + _T(" INTO ");
  bool moreThenOne = false;

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
SQLInfoMySQL::GetPSMExceptionCatchNoData() const
{
  return _T("DECLARE EXIT HANDLER FOR SQLSTATE '02000'\n");
  // Must be followed by a (block)statement
}

XString
SQLInfoMySQL::GetPSMExceptionCatch(XString p_sqlState) const
{
  return _T("DECLARE EXIT HANDLER FOR SQLSTATE '") + p_sqlState + _T("'\n");
  // Must be followed by a (block)statement
}

XString
SQLInfoMySQL::GetPSMExceptionRaise(XString p_sqlState) const
{
  return _T("SIGNAL SQLSTATE '") + p_sqlState + _T("'\n");
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
  return _T("");
}

XString
SQLInfoMySQL::GetSESSIONExists(XString p_sessionID) const
{
  return _T("");
}

XString
SQLInfoMySQL::GetSESSIONList() const
{
  return _T("");
}

XString
SQLInfoMySQL::GetSESSIONAttributes(XString p_sessionID) const
{
  return _T("");
}

//////////////////////////////////////////////////////////////////////////
// Transactions

XString
SQLInfoMySQL::GetSESSIONConstraintsDeferred() const
{
  // MySQL cannot defer constraints
  return _T("");
}

XString
SQLInfoMySQL::GetSESSIONConstraintsImmediate() const
{
  // MySQL constraints are always active
  return _T("");
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
