////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoGenericODBC.cpp
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
#include "SQLInfoGenericODBC.h"
#include "SQLQuery.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{
  
// Constructor
SQLInfoGenericODBC::SQLInfoGenericODBC(SQLDatabase* p_database)
                   :SQLInfoDB(p_database)
{
}

// Destructor. Does nothing
SQLInfoGenericODBC::~SQLInfoGenericODBC()
{
}

// RDBMS Uses INDENTITY or SEQUENCE interface
void
SQLInfoGenericODBC::SetUseSequences(bool /*p_sequences*/)
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
SQLInfoGenericODBC::GetRDBMSDatabaseType() const
{
  return RDBMS_ODBC_STANDARD;
}

// The name of the database vendor
XString
SQLInfoGenericODBC::GetRDBMSVendorName() const
{
  // The name of the database vendor
  return _T("Generic ODBC Driver");
}

// Get the physical database name
XString
SQLInfoGenericODBC::GetRDBMSPhysicalDatabaseName() const
{
  // See to it that "SQLDatabase:GetDatabaseName" does it's work
  return m_database->GetDatabaseName();
}

// System catalog is stored in uppercase in the database?
bool
SQLInfoGenericODBC::GetRDBMSIsCatalogUpper() const
{
  return true;
}

// System catalog supports full ISO schemas (same tables per schema)
bool
SQLInfoGenericODBC::GetRDBMSUnderstandsSchemas() const
{
  return true;
}

// Supports database/ODBCdriver comments in SQL
bool
SQLInfoGenericODBC::GetRDBMSSupportsComments() const
{
  return true;
}

// Database can defer constraints until the end of a transaction
bool
SQLInfoGenericODBC::GetRDBMSSupportsDeferredConstraints() const
{
  // If transactions are supported we assume that constraints can be deferred
  // until the end of the transaction in the commit phase
  return m_txn_cap > 0;
}

// Database has ORDER BY with an expression, e.g. ORDER BY UPPER(columnname)
bool
SQLInfoGenericODBC::GetRDBMSSupportsOrderByExpression() const
{
  return false;
}

// Supports the ODBC escape sequence {[?=] CALL procedure (?,?,?)}
bool
SQLInfoGenericODBC::GetRDBMSSupportsODBCCallEscapes() const
{
  return true;
}

// Supports the ODBC call procedure with named parameters
bool
SQLInfoGenericODBC::GetRDBMSSupportsODBCCallNamedParameters() const
{
  return true;
}

// If the database does not support the datatype TIME, it can be implemented as a DECIMAL
bool
SQLInfoGenericODBC::GetRDBMSSupportsDatatypeTime() const
{
  // Time can be implemented as TIME
  return true;
}

// If the database does not support the datatype INTERVAL, it can be implemented as a DECIMAL
bool
SQLInfoGenericODBC::GetRDBMSSupportsDatatypeInterval() const
{
  // Interval supported
  return true;
}

// Supports functions at the place of table columns in create/alter index statement
bool
SQLInfoGenericODBC::GetRDBMSSupportsFunctionalIndexes() const
{
  return false;
}

// Gets the maximum length of an SQL statement
unsigned long 
SQLInfoGenericODBC::GetRDBMSMaxStatementLength() const
{
  // No limit
  return 0;
}

// Database must commit DDL commands in a transaction
bool
SQLInfoGenericODBC::GetRDBMSMustCommitDDL() const
{
  return false;
}

// Correct maximum precision,scale for a NUMERIC datatype
void
SQLInfoGenericODBC::GetRDBMSNumericPrecisionScale(SQLULEN& /*p_precision*/, SQLSMALLINT& /*p_scale*/) const
{
  // NO-OP
}

// Maximum for a VARCHAR to be handled without AT-EXEC data. Assume NVARCHAR is half that size!
int
SQLInfoGenericODBC::GetRDBMSMaxVarchar() const
{
  return 1000;
}

// KEYWORDS

// Keyword for the current date and time
XString
SQLInfoGenericODBC::GetKEYWORDCurrentTimestamp() const
{
  return _T("current_timestamp");
}

// String for the current date
XString
SQLInfoGenericODBC::GetKEYWORDCurrentDate() const
{
  return _T("current_date");
}

// Get the concatenation operator
XString
SQLInfoGenericODBC::GetKEYWORDConcatanationOperator() const
{
  return _T("||");
}

// Get quote character for strings
XString
SQLInfoGenericODBC::GetKEYWORDQuoteCharacter() const
{
  return _T("\'");
}

// Get quote character around reserved words as an identifier
XString
SQLInfoGenericODBC::GetKEYWORDReservedWordQuote() const
{
  return _T("\"");
}

// Get default NULL for parameter list input
XString
SQLInfoGenericODBC::GetKEYWORDParameterDefaultNULL() const
{
  // Standard, no definition defines the NULL state
  return XString();
}

// Parameter is for INPUT and OUTPUT in parameter list
XString
SQLInfoGenericODBC::GetKEYWORDParameterINOUT() const
{
  return XString();
}

// Parameter is for OUTPUT only in parameter list
XString
SQLInfoGenericODBC::GetKEYWORDParameterOUT() const
{
  return XString();
}

// Get datatype of the IDENTITY primary key in a Network database
XString
SQLInfoGenericODBC::GetKEYWORDNetworkPrimaryKeyType() const
{
  // Use SEQUENCE to fill!
  return _T("integer");
}

// Get datatype for timestamp (year to second)
XString
SQLInfoGenericODBC::GetKEYWORDTypeTimestamp() const
{
  return _T("timestamp");
}

// Prefix for a parameter in a stored procedure
XString
SQLInfoGenericODBC::GetKEYWORDParameterPrefix() const
{
  return XString();
}

// Get select part to add new record identity to a table
// Can be special column like 'OID' or a sequence select
XString
SQLInfoGenericODBC::GetKEYWORDIdentityString(XString& /*p_tablename*/,XString /*p_postfix*/ /*= "_seq"*/) const
{
  // Undetermined: return nothing
  return XString();
}

// Gets the UPPER function
XString
SQLInfoGenericODBC::GetKEYWORDUpper(XString& p_expression) const
{
  return _T("{fn UCASE(") + p_expression + _T(")}");
}

// Gets the construction for 1 minute ago
XString
SQLInfoGenericODBC::GetKEYWORDInterval1MinuteAgo() const
{
  // Not supported
  return _T("ERROR");
}

// Gets the Not-NULL-Value statement of the database
XString
SQLInfoGenericODBC::GetKEYWORDStatementNVL(XString& p_test,XString& p_isnull) const
{
  return _T("{fn IFNULL(") + p_test + _T(",") + p_isnull + _T(")}");
}

// Gets the RDBMS definition of the datatype
XString
SQLInfoGenericODBC::GetKEYWORDDataType(MetaColumn* p_column)
{
  return p_column->m_typename = SQLInfo::ODBCDataType(p_column->m_datatype);
}

// Gets the USER (current-user) keyword function
XString
SQLInfoGenericODBC::GetKEYWORDCurrentUser() const
{
  return _T("CURRENT_USER");
}

// Connects to a default schema in the database/instance
XString
SQLInfoGenericODBC::GetSQLDefaultSchema(XString /*p_user*/,XString /*p_schema*/) const
{
  return XString();
}

// Gets the construction for inline generating a key within an INSERT statement
XString
SQLInfoGenericODBC::GetSQLNewSerial(XString /*p_table*/, XString /*p_sequence*/) const
{
  // Insert a zero in an IDENTITY column
  return _T("0");
}

// Gets the construction / select for generating a new serial identity
XString
SQLInfoGenericODBC::GetSQLGenerateSerial(XString p_table) const
{
  // NO WAY OF KNOWNING THIS / And no need to
  return _T("0");
}

XString
SQLInfoGenericODBC::GetSQLGenerateSequence(XString p_sequence) const
{
  // Not supported
  return _T("");
}

// Gets the construction / select for the resulting effective generated serial
XString
SQLInfoGenericODBC::GetSQLEffectiveSerial(XString p_identity) const
{
  // THIS IS MOST LIKELY NOT THE CORRECT VALUE.
  // NO WAY OF DETERMINING THIS
  return p_identity;
}

// Gets the sub-transaction commands
XString
SQLInfoGenericODBC::GetSQLStartSubTransaction(XString p_savepointName) const
{
  // Generic ODBC does not known about sub transactions!
  return XString();
}

XString
SQLInfoGenericODBC::GetSQLCommitSubTransaction(XString p_savepointName) const
{
  // Generic ODBC does not known about sub transactions!
  return XString();
}

XString
SQLInfoGenericODBC::GetSQLRollbackSubTransaction(XString p_savepointName) const
{
  // Generic ODBC does not known about sub transactions!
  return XString();
}

// FROM-Part for a query to select only 1 (one) record / or empty!
XString
SQLInfoGenericODBC::GetSQLFromDualClause() const
{
  // No way of knowing this in standard ODBC
  return XString();
}

// Get SQL to lock  a table 
XString
SQLInfoGenericODBC::GetSQLLockTable(XString p_schema, XString p_tablename,bool p_exclusive,int /*p_waittime*/) const
{
  // Standard ISO SQL Syntax
  XString query = _T("LOCK TABLE ") + p_schema + _T(".") + p_tablename + _T(" IN ");
  query += p_exclusive ? _T("EXCLUSIVE") : _T("SHARE");
  query += _T(" MODE");
  return query;
}

// Get query to optimize the table statistics
XString
SQLInfoGenericODBC::GetSQLOptimizeTable(XString p_schema, XString p_tablename) const
{
  return XString();
}

// Transform query to select top <n> rows
XString
SQLInfoGenericODBC::GetSQLTopNRows(XString p_sql,int /*p_top*/,int /*p_skip = 0*/) const
{
  // Does nothing
  return p_sql;
}

// Expand a SELECT with an 'FOR UPDATE' lock clause
XString
SQLInfoGenericODBC::GetSelectForUpdateTableClause(unsigned /*p_lockWaitTime*/) const
{
  return "";
}

XString
SQLInfoGenericODBC::GetSelectForUpdateTrailer(XString p_select,unsigned /*p_lockWaitTime*/) const
{
  return p_select + "\nFOR UPDATE";
}

// Query to perform a keep alive ping
XString
SQLInfoGenericODBC::GetPing() const
{
  // Not implemented yet
  return XString();
}

//////////////////////////////////////////////////////////////////////////
//
// SQL STRINGS
//
//////////////////////////////////////////////////////////////////////////

// Makes a SQL string from a given string, with all the right quotes
XString
SQLInfoGenericODBC::GetSQLString(const XString& p_string) const
{
  XString s = p_string;
  s.Replace(_T("'"),_T("''"));
  XString kwoot = GetKEYWORDQuoteCharacter();
  return  kwoot + s + kwoot;
}

// Get date string in engine format
XString
SQLInfoGenericODBC::GetSQLDateString(int p_year,int p_month,int p_day) const
{
  XString dateString;
  dateString.Format(_T("{d '%04d-%02d-%02d'}"),p_year,p_month,p_day);
  return dateString;
}

// Get time string in database engine format
XString
SQLInfoGenericODBC::GetSQLTimeString(int p_hour,int p_minute,int p_second) const
{
  XString retval;
  retval.Format(_T("{t '%02d:%02d:%02d'}"),p_hour,p_minute,p_second);
  return retval;
}

// Get date-time string in database engine format
XString
SQLInfoGenericODBC::GetSQLDateTimeString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  XString string;
  string.Format(_T("{ts '%04d-%02d-%02d %02d:%02d:%02d'}")
                ,p_year,p_month,p_day // ODBC Ordering !!
                ,p_hour,p_minute,p_second);
  return string;
}

// Get date-time bound parameter string in database format
XString
SQLInfoGenericODBC::GetSQLDateTimeBoundString() const
{
  return _T("{ts ?}");
}

// Stripped data for the parameter binding
XString
SQLInfoGenericODBC::GetSQLDateTimeStrippedString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  XString string;
  string.Format(_T("%04d-%02d-%02d %02d:%02d:%02d")
                ,p_year,p_month,p_day // ODBC Ordering !!
                ,p_hour,p_minute,p_second);
  return string;
}

// Makes an catalog identifier string (possibly quoted on both sides)
XString
SQLInfoGenericODBC::GetSQLDDLIdentifier(XString p_identifier) const
{
  return p_identifier;
}

// Get the name of a temp table (local temporary or global temporary)
XString
SQLInfoGenericODBC::GetTempTablename(XString /*p_schema*/,XString p_tablename,bool /*p_local*/) const
{
  return p_tablename;
}

// Changes to parameters before binding to an ODBC HSTMT handle
void
SQLInfoGenericODBC::DoBindParameterFixup(SQLSMALLINT& /*p_dataType*/,SQLSMALLINT& /*p_sqlDatatype*/,SQLULEN& /*p_columnSize*/,SQLSMALLINT& /*p_scale*/,SQLLEN& /*p_bufferSize*/,SQLLEN* /*p_indicator*/) const
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
SQLInfoGenericODBC::GetCATALOGMetaTypes(int p_type) const
{
  UNREFERENCED_PARAMETER(p_type);
  return XString();
}

XString
SQLInfoGenericODBC::GetCATALOGDefaultCharset() const
{
  return _T("iso-8859-1");
}

XString
SQLInfoGenericODBC::GetCATALOGDefaultCharsetNCV() const
{
  return _T("utf-16");
}

XString
SQLInfoGenericODBC::GetCATALOGDefaultCollation() const
{
  return _T("-");
}

// ALL FUNCTIONS FOR TABLE(s)

XString
SQLInfoGenericODBC::GetCATALOGTableExists(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  // Cannot do this, let ODBC handle this
  return XString();
}

XString
SQLInfoGenericODBC::GetCATALOGTablesList(XString& /*p_schema*/,XString& /*p_pattern*/) const
{
  // Cannot do this, let ODBC handle this
  return XString();
}

XString
SQLInfoGenericODBC::GetCATALOGTableAttributes(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  // Cannot do this, let ODBC handle this
  return XString();
}

XString
SQLInfoGenericODBC::GetCATALOGTableSynonyms(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  // Cannot do this, let ODBC handle this
  return XString();
}

XString
SQLInfoGenericODBC::GetCATALOGTableCatalog(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  // Cannot do this, let ODBC handle this
  return XString();
}

XString
SQLInfoGenericODBC::GetCATALOGTableCreate(MetaTable& p_table,MetaColumn& /*p_column*/) const
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
SQLInfoGenericODBC::GetCATALOGTableCreatePostfix(MetaTable& /*p_table*/,MetaColumn& /*p_column*/) const
{
  return XString();
}

XString
SQLInfoGenericODBC::GetCATALOGTableRename(XString p_schema,XString p_tablename,XString p_newname) const
{
  XString sql(_T("RENAME TABLE") + p_schema + _T(".") + p_tablename + _T(" TO ") + p_newname);
  return sql;
}

XString
SQLInfoGenericODBC::GetCATALOGTableDrop(XString p_schema,XString p_tablename,bool /*p_ifExist = false*/,bool /*p_restrict = false*/,bool /*p_cascade = false*/) const
{
  XString sql = _T("DROP TABLE ");
  if(!p_schema.IsEmpty())
  {
    sql += p_schema;
    sql += _T(".");
  }
  sql += p_tablename;
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL TEMPORARY TABLE FUNCTIONS

XString 
SQLInfoGenericODBC::GetCATALOGTemptableCreate(XString p_schema,XString p_tablename,XString p_select) const
{
  // BEWARE: THIS IS A GUESS. NO REAL DEFINITION IN ODBC
  return _T("CREATE TEMPORARY TABLE ") + p_schema + _T(".") + p_tablename + _T("\nAS ") + p_select;
}

XString 
SQLInfoGenericODBC::GetCATALOGTemptableIntoTemp(XString p_schema,XString p_tablename,XString p_select) const
{
  return _T("INSERT INTO ") + p_schema + _T(".") + p_tablename + _T("\n") + p_select + _T(";\n");
}

XString 
SQLInfoGenericODBC::GetCATALOGTemptableDrop(XString p_schema,XString p_tablename) const
{
  return _T("DROP TABLE ") + p_schema + _T(".") + p_tablename;
}

//////////////////////////////////////////////////////////////////////////
// ALL COLUMN FUNCTIONS

XString 
SQLInfoGenericODBC::GetCATALOGColumnExists(XString p_schema,XString p_tablename,XString p_columnname) const
{
  // Cannot now that, use ODBC!
  return XString();
}

XString 
SQLInfoGenericODBC::GetCATALOGColumnList(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  // Cannot now that, use ODBC!
  return XString();
}

XString 
SQLInfoGenericODBC::GetCATALOGColumnAttributes(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_columnname*/) const
{
  // Cannot now that, use ODBC!
  return XString();
}

XString 
SQLInfoGenericODBC::GetCATALOGColumnCreate(MetaColumn& p_column) const
{
  // General ISO 9075E syntax
  XString sql = _T("ALTER TABLE  ") + p_column.m_schema + _T(".") + p_column.m_table  + _T("\n")
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
SQLInfoGenericODBC::GetCATALOGColumnAlter(MetaColumn& p_column) const
{
  // General ISO 9075E syntax
  XString sql = _T("ALTER TABLE  ") + p_column.m_schema + _T(".") + p_column.m_table  + _T("\n")
                _T("ALTER COLUMN ") + p_column.m_column + _T(" ") + p_column.m_typename;
  p_column.GetPrecisionAndScale(sql);
  p_column.GetNullable(sql);
  p_column.GetDefault(sql);
  // m_position not used
  // m_length   not used
  // m_remarks  not used
  return sql;
}

XString 
SQLInfoGenericODBC::GetCATALOGColumnRename(XString p_schema,XString p_tablename,XString p_columnname,XString p_newname,XString /*p_datatype*/) const
{
  // General ISO 9075E syntax
  XString sql(_T("ALTER  TABLE  ") + p_schema + _T(".") + p_tablename + _T("\n")
              _T("RENAME ") + p_columnname + _T(" TO ") + p_newname + _T("\n"));
  return sql;
}

XString 
SQLInfoGenericODBC::GetCATALOGColumnDrop(XString p_schema,XString p_tablename,XString p_columnname) const
{
  // General ISO 9075E syntax
  XString sql(_T("ALTER TABLE  ") + p_schema + _T(".") + p_tablename + _T("\n")
              _T("DROP  COLUMN ") + p_columnname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL INDICES FUNCTIONS

// All index functions
XString
SQLInfoGenericODBC::GetCATALOGIndexExists(XString p_schema,XString p_tablename,XString p_indexname) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLStatistics instead (see SQLInfo class)
  return XString();
}

XString
SQLInfoGenericODBC::GetCATALOGIndexList(XString& /*p_schema*/,XString& /*p_tablename*/)   const
{
  // Cannot be implemented for generic ODBC
  // Use SQLStatistics instead (see SQLInfo class)
  return XString();
}

XString
SQLInfoGenericODBC::GetCATALOGIndexAttributes(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_indexname*/)  const
{
  // Cannot be implemented for generic ODBC
  // Use SQLStatistics instead (see SQLInfo class)
  return XString();
}

XString
SQLInfoGenericODBC::GetCATALOGIndexCreate(MIndicesMap& p_indices,bool /*p_duplicateNulls /*= false*/) const
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
SQLInfoGenericODBC::GetCATALOGIndexDrop(XString p_schema,XString /*p_tablename*/,XString p_indexname) const
{
  XString sql = _T("DROP INDEX ") + p_schema + _T(".") + p_indexname;
  return sql;
}

// Get extra filter expression for an index column
XString
SQLInfoGenericODBC::GetCATALOGIndexFilter(MetaIndex& /*p_index*/) const
{
  return XString();
}

//////////////////////////////////////////////////////////////////////////
// ALL PRIMARY KEY FUNCTIONS

XString
SQLInfoGenericODBC::GetCATALOGPrimaryExists(XString /*p_schema*/,XString /*p_tablename*/) const
{
  // Cannot do this, Use ODBC functions!
  return XString();
}

XString
SQLInfoGenericODBC::GetCATALOGPrimaryAttributes(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  // Cannot do this, Use ODBC functions!
  return XString();
}

XString
SQLInfoGenericODBC::GetCATALOGPrimaryCreate(MPrimaryMap& p_primaries) const
{
  // General ISO Primary key constraint
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
SQLInfoGenericODBC::GetCATALOGPrimaryDrop(XString p_schema,XString p_tablename,XString p_constraintname) const
{
  XString sql(_T("ALTER TABLE ") + p_schema + _T(".") + p_tablename + _T("\n")
              _T(" DROP CONSTRAINT ") + p_constraintname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL FOREIGN KEY FUNCTIONS

XString
SQLInfoGenericODBC::GetCATALOGForeignExists(XString p_schema,XString p_tablename,XString p_constraintname) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLForeignKeys instead (see SQLInfo class)
  return XString();
}

XString
SQLInfoGenericODBC::GetCATALOGForeignList(XString& /*p_schema*/,XString& /*p_tablename*/,int /*p_maxColumns*/ /*=SQLINFO_MAX_COLUMNS*/) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLForeignKeys instead (see SQLInfo class)
  return XString();
}

XString
SQLInfoGenericODBC::GetCATALOGForeignAttributes(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_constraintname*/,bool /*p_referenced = false*/,int /*p_maxColumns*/ /*=SQLINFO_MAX_COLUMNS*/) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLForeignKeys instead (see SQLInfo class)
  return XString();
}

XString
SQLInfoGenericODBC::GetCATALOGForeignCreate(MForeignMap& p_foreigns) const
{
  // Get first record
  MetaForeign& foreign = p_foreigns.front();

  // Construct the correct table name
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
    case SQL_NOT_DEFERRABLE:      query += _T("\n      NOT DEFERRABLE");     break;
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
SQLInfoGenericODBC::GetCATALOGForeignAlter(MForeignMap& p_original,MForeignMap& p_requested) const
{
  // Make sure we have both
  if(p_original.empty() || p_requested.empty())
  {
    return XString();
  }

  const MetaForeign& original  = p_original.front();
  const MetaForeign& requested = p_requested.front();

  // Construct the correct table name
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
SQLInfoGenericODBC::GetCATALOGForeignDrop(XString p_schema,XString p_tablename,XString p_constraintname) const
{
  XString sql(_T("ALTER TABLE ") + p_schema + _T(".") + p_tablename + _T("\n")
              _T(" DROP CONSTRAINT ") + p_constraintname);
  return sql;
}

//////////////////////////
// All default constraints
XString
SQLInfoGenericODBC::GetCATALOGDefaultExists(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_column*/) const
{
  return XString();
}

XString
SQLInfoGenericODBC::GetCATALOGDefaultList(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  return XString();
}

XString
SQLInfoGenericODBC::GetCATALOGDefaultAttributes(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_column*/) const
{
  return XString();
}

XString
SQLInfoGenericODBC::GetCATALOGDefaultCreate(XString /*p_schema*/,XString /*p_tablename*/,XString /*p_constraint*/,XString /*p_column*/,XString /*p_code*/) const
{
  return XString();
}

XString
SQLInfoGenericODBC::GetCATALOGDefaultDrop(XString /*p_schema*/,XString /*p_tablename*/,XString /*p_constraint*/) const
{
  return XString();
}

/////////////////////////
// All check constraints

XString
SQLInfoGenericODBC::GetCATALOGCheckExists(XString  /*p_schema*/,XString  /*p_tablename*/,XString  /*p_constraint*/) const
{
  return XString();
}

XString
SQLInfoGenericODBC::GetCATALOGCheckList(XString  /*p_schema*/,XString  /*p_tablename*/) const
{
  return XString();
}

XString
SQLInfoGenericODBC::GetCATALOGCheckAttributes(XString  /*p_schema*/,XString  /*p_tablename*/,XString  /*p_constraint*/) const
{
  return XString();
}

XString
SQLInfoGenericODBC::GetCATALOGCheckCreate(XString  /*p_schema*/,XString  /*p_tablename*/,XString  /*p_constraint*/,XString /*p_condition*/) const
{
  return XString();
}

XString
SQLInfoGenericODBC::GetCATALOGCheckDrop(XString  /*p_schema*/,XString  /*p_tablename*/,XString  /*p_constraint*/) const
{
  return XString();
}

//////////////////////////////////////////////////////////////////////////
// ALL TRIGGER FUNCTIONS

XString
SQLInfoGenericODBC::GetCATALOGTriggerExists(XString p_schema, XString p_tablename, XString p_triggername) const
{
  // Not standard enough
  return XString();
}

XString
SQLInfoGenericODBC::GetCATALOGTriggerList(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  // Not standard enough
  return XString();
}

XString
SQLInfoGenericODBC::GetCATALOGTriggerAttributes(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_triggername*/) const
{
  // Not standard enough
  return XString();
}

XString
SQLInfoGenericODBC::GetCATALOGTriggerCreate(MetaTrigger& /*p_trigger*/) const
{
  // Not standard enough
  return XString();
}

XString
SQLInfoGenericODBC::GetCATALOGTriggerDrop(XString p_schema, XString p_tablename, XString p_triggername) const
{
  // Not standard enough
  return XString();
}

//////////////////////////////////////////////////////////////////////////
// ALL SEQUENCE FUNCTIONS

XString
SQLInfoGenericODBC::GetCATALOGSequenceExists(XString p_schema, XString p_sequence) const
{
  return XString();
}

XString
SQLInfoGenericODBC::GetCATALOGSequenceList(XString& /*p_schema*/,XString& /*p_pattern*/) const
{
  return XString();
}

XString
SQLInfoGenericODBC::GetCATALOGSequenceAttributes(XString& /*p_schema*/,XString& /*p_sequence*/) const
{
  return XString();
}

XString
SQLInfoGenericODBC::GetCATALOGSequenceCreate(MetaSequence& /*p_sequence*/) const
{
  return XString();
}

XString
SQLInfoGenericODBC::GetCATALOGSequenceDrop(XString p_schema, XString p_sequence) const
{
  return XString();
}

//////////////////////////////////////////////////////////////////////////
// ALL VIEW FUNCTIONS

XString 
SQLInfoGenericODBC::GetCATALOGViewExists(XString& /*p_schema*/,XString& /*p_viewname*/) const
{
  return XString();
}

XString 
SQLInfoGenericODBC::GetCATALOGViewList(XString& /*p_schema*/,XString& /*p_pattern*/) const
{
  return XString();
}

XString 
SQLInfoGenericODBC::GetCATALOGViewAttributes(XString& /*p_schema*/,XString& /*p_viewname*/) const
{
  return XString();
}

XString
SQLInfoGenericODBC::GetCATALOGViewText(XString& /*p_schema*/,XString& /*p_viewname*/) const
{
  // Cannot query this, Use ODBC functions
  return XString();
}

XString
SQLInfoGenericODBC::GetCATALOGViewCreate(XString p_schema,XString p_viewname,XString p_contents,bool /*p_ifexists = true*/) const
{
  return _T("CREATE VIEW ") + p_schema + _T(".") + p_viewname + _T("\n") + p_contents;
}

XString 
SQLInfoGenericODBC::GetCATALOGViewRename(XString p_schema,XString p_viewname,XString p_newname)    const
{
  return XString();
}

XString 
SQLInfoGenericODBC::GetCATALOGViewDrop(XString p_schema,XString p_viewname,XString& p_precursor) const
{
  p_precursor.Empty();
  return _T("DROP VIEW ") + p_schema + _T(".") + p_viewname;
}

// All Privilege functions
XString
SQLInfoGenericODBC::GetCATALOGTablePrivileges(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  return XString();
}

XString 
SQLInfoGenericODBC::GetCATALOGColumnPrivileges(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_columnname*/) const
{
  return XString();
}

XString 
SQLInfoGenericODBC::GetCATALOGSequencePrivilege(XString& /*p_schema*/,XString& /*p_sequence*/) const
{
  return XString();
}

XString 
SQLInfoGenericODBC::GetCATALOGGrantPrivilege(XString p_schema,XString p_objectname,XString p_privilege,XString p_grantee,bool p_grantable)
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
SQLInfoGenericODBC::GetCATALOGRevokePrivilege(XString p_schema,XString p_objectname,XString p_privilege,XString p_grantee)
{
  XString sql;
  sql.Format(_T("REVOKE %s ON %s.%s FROM %s"),p_privilege.GetString(),p_schema.GetString(),p_objectname.GetString(),p_grantee.GetString());
  return sql;
}

// All Synonym functions
XString
SQLInfoGenericODBC::GetCATALOGSynonymList(XString& /*p_schema*/,XString& /*p_pattern*/) const
{
  // Not implemented yet
  return XString();
}

XString
SQLInfoGenericODBC::GetCATALOGSynonymAttributes(XString& /*p_schema*/,XString& /*p_synonym*/) const
{
  // Not implemented yet
  return XString();
}

XString
SQLInfoGenericODBC::GetCATALOGSynonymCreate(XString& /*p_schema*/,XString& /*p_synonym*/,XString /*p_forObject*/,bool /*p_private = true*/) const
{
  // Not implemented yet
  return XString();
}

XString
SQLInfoGenericODBC::GetCATALOGSynonymDrop(XString& /*p_schema*/,XString& /*p_synonym*/,bool /*p_private = true*/) const
{
  // Not implemented yet
  return XString();
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
SQLInfoGenericODBC::GetPSMProcedureExists(XString p_schema, XString p_procedure) const
{
  return XString();
}

XString
SQLInfoGenericODBC::GetPSMProcedureList(XString& /*p_schema*/) const
{
  return XString();
}

XString
SQLInfoGenericODBC::GetPSMProcedureAttributes(XString& /*p_schema*/,XString& /*p_procedure*/) const
{
  return XString();
}

XString
SQLInfoGenericODBC::GetPSMProcedureSourcecode(XString p_schema, XString p_procedure) const
{
  return XString();
}

XString
SQLInfoGenericODBC::GetPSMProcedureCreate(MetaProcedure& /*p_procedure*/) const
{
  return XString();
}

XString
SQLInfoGenericODBC::GetPSMProcedureDrop(XString p_schema, XString p_procedure,bool /* p_function /*=false*/) const
{
  return XString();
}

XString
SQLInfoGenericODBC::GetPSMProcedureErrors(XString p_schema,XString p_procedure) const
{
  // ISO SQL does not support procedure errors
  return XString();
}

XString
SQLInfoGenericODBC::GetPSMProcedurePrivilege(XString& /*p_schema*/,XString& /*p_procedure*/) const
{
  return XString();
}

// And it's parameters
XString
SQLInfoGenericODBC::GetPSMProcedureParameters(XString& /*p_schema*/,XString& /*p_procedure*/) const
{
  return XString();
}

//////////////////////////////////////////////////////////////////////////
//
// ALL PSM LANGUAGE ELEMENTS
//
//////////////////////////////////////////////////////////////////////////

XString
SQLInfoGenericODBC::GetPSMDeclaration(bool    /*p_first*/
                                     ,XString /*p_variable*/
                                     ,int     /*p_datatype*/
                                     ,int     /*p_precision*/ /*= 0 */
                                     ,int     /*p_scale*/     /*= 0 */
                                     ,XString /*p_default*/   /*= ""*/
                                     ,XString /*p_domain*/    /*= ""*/
                                     ,XString /*p_asColumn*/  /*= ""*/) const
{
  return XString();
}

XString
SQLInfoGenericODBC::GetPSMAssignment(XString p_variable,XString p_statement /*=""*/) const
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
SQLInfoGenericODBC::GetPSMIF(XString p_condition) const
{
  XString line(_T("IF ("));
  line += p_condition;
  line += _T(") THEN\n");
  return XString();
}

XString
SQLInfoGenericODBC::GetPSMIFElse() const
{
  return _T("  ELSE\n");
}

XString
SQLInfoGenericODBC::GetPSMIFEnd() const
{
  return _T("END IF;\n");
}

XString
SQLInfoGenericODBC::GetPSMWhile(XString p_condition) const
{
  return _T("WHILE (") + p_condition + _T(") LOOP\n");
}

XString
SQLInfoGenericODBC::GetPSMWhileEnd() const
{
  return _T("END LOOP;\n");
}

XString
SQLInfoGenericODBC::GetPSMLOOP() const
{
  return XString();
}

XString
SQLInfoGenericODBC::GetPSMLOOPEnd() const
{
  return XString();
}

XString
SQLInfoGenericODBC::GetPSMBREAK() const
{
  return XString();
}

XString
SQLInfoGenericODBC::GetPSMRETURN(XString p_statement /*= ""*/) const
{
  return XString();
}

XString
SQLInfoGenericODBC::GetPSMExecute(XString /*p_procedure*/,MParameterMap& /*p_parameters*/) const
{
  return XString();
}

// The CURSOR
XString
SQLInfoGenericODBC::GetPSMCursorDeclaration(XString /*p_cursorname*/,XString /*p_select*/) const
{
  return XString();
}

XString
SQLInfoGenericODBC::GetPSMCursorFetch(XString /*p_cursorname*/,std::vector<XString>& /*p_columnnames*/,std::vector<XString>& /*p_variablenames*/) const
{
  return XString();
}

//////////////////////////////////////////////////////////////////////////
// PSM Exceptions

XString
SQLInfoGenericODBC::GetPSMExceptionCatchNoData() const
{
  return XString();
}

XString
SQLInfoGenericODBC::GetPSMExceptionCatch(XString p_sqlState) const
{
  return XString();
}

XString
SQLInfoGenericODBC::GetPSMExceptionRaise(XString p_sqlState) const
{
  return XString();
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
SQLInfoGenericODBC::GetSESSIONMyself() const
{
  // Generic ISO ODBC has no info about other sessions
  return XString();
}

XString
SQLInfoGenericODBC::GetSESSIONExists(XString p_sessionID) const
{
  // Generic ISO ODBC has no info about other sessions
  return XString();
}

XString
SQLInfoGenericODBC::GetSESSIONList() const
{
  // Generic ISO ODBC has no info about other sessions
  return XString();
}

XString
SQLInfoGenericODBC::GetSESSIONAttributes(XString p_sessionID) const
{
  // Generic ISO ODBC has no info about other sessions
  return XString();
}

//////////////////////////////////////////////////////////////////////////
// Transactions

XString
SQLInfoGenericODBC::GetSESSIONConstraintsDeferred() const
{
  // ISO SQL does not know how to defer constraints
  return XString();
}

XString
SQLInfoGenericODBC::GetSESSIONConstraintsImmediate() const
{
  // ISO SQL constraints are always active
  return XString();
}

//////////////////////////////////////////////////////////////////////////
//
// Call FUNCTION/PROCEDURE from within program
// As a RDBMS dependent extension of "DoSQLCall" of the SQLQuery object
//
//////////////////////////////////////////////////////////////////////////

// Calling a stored function or procedure if the RDBMS does not support ODBC call escapes
SQLVariant*
SQLInfoGenericODBC::DoSQLCall(SQLQuery* /*p_query*/,XString& /*p_schema*/,XString& /*p_procedure*/)
{
  return nullptr;
}

// Calling a stored function with named parameters, returning a value
SQLVariant* 
SQLInfoGenericODBC::DoSQLCallNamedParameters(SQLQuery* /*p_query*/,XString& /*p_schema*/,XString& /*p_procedure*/,bool /*p_function = true*/)
{
  return nullptr;
}

// End of namespace
}
