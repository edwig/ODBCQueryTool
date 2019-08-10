////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoGenericODBC.cpp
//
// Copyright (c) 1998-2019 ir. W.E. Huisman
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
// Last Revision:  15-06-2019
// Version number: 1.5.5
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
CString
SQLInfoGenericODBC::GetRDBMSVendorName() const
{
  // The name of the database vendor
  return "Generic ODBC Driver";
}

// Get the physical database name
CString
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

// KEYWORDS

// Keyword for the current date and time
CString
SQLInfoGenericODBC::GetKEYWORDCurrentTimestamp() const
{
  return "current_timestamp";
}

// String for the current date
CString
SQLInfoGenericODBC::GetKEYWORDCurrentDate() const
{
  return "current_date";
}

// Get the concatenation operator
CString
SQLInfoGenericODBC::GetKEYWORDConcatanationOperator() const
{
  return "||";
}

// Get quote character for strings
CString
SQLInfoGenericODBC::GetKEYWORDQuoteCharacter() const
{
  return "\'";
}

// Get default NULL for parameter list input
CString
SQLInfoGenericODBC::GetKEYWORDParameterDefaultNULL() const
{
  // Standard, no definition defines the NULL state
  return "";
}

// Parameter is for INPUT and OUTPUT in parameter list
CString
SQLInfoGenericODBC::GetKEYWORDParameterINOUT() const
{
  return "";
}

// Parameter is for OUTPUT only in parameter list
CString
SQLInfoGenericODBC::GetKEYWORDParameterOUT() const
{
  return "";
}

// Get datatype of the IDENTITY primary key in a Network database
CString
SQLInfoGenericODBC::GetKEYWORDNetworkPrimaryKeyType() const
{
  // Use SEQUENCE to fill!
  return "integer";
}

// Get datatype for timestamp (year to second)
CString
SQLInfoGenericODBC::GetKEYWORDTypeTimestamp() const
{
  return "timestamp";
}

// Prefix for a parameter in a stored procedure
CString
SQLInfoGenericODBC::GetKEYWORDParameterPrefix() const
{
  return "";
}

// Get select part to add new record identity to a table
// Can be special column like 'OID' or a sequence select
CString
SQLInfoGenericODBC::GetKEYWORDIdentityString(CString& /*p_tablename*/,CString /*p_postfix*/ /*= "_seq"*/) const
{
  // Undetermined: return nothing
  return "";
}

// Gets the UPPER function
CString
SQLInfoGenericODBC::GetKEYWORDUpper(CString& p_expression) const
{
  return "{fn UCASE(" + p_expression + ")}";
}

// Gets the construction for 1 minute ago
CString
SQLInfoGenericODBC::GetKEYWORDInterval1MinuteAgo() const
{
  // Not supported
  return "ERROR";
}

// Gets the Not-NULL-Value statement of the database
CString
SQLInfoGenericODBC::GetKEYWORDStatementNVL(CString& p_test,CString& p_isnull) const
{
  return "{fn IFNULL(" + p_test + "," + p_isnull + ")}";
}

// Gets the construction for inline generating a key within an INSERT statement
CString
SQLInfoGenericODBC::GetSQLNewSerial(CString /*p_table*/, CString /*p_sequence*/) const
{
  // Insert a zero in an IDENTITY column
  return "0";
}

// Gets the construction / select for generating a new serial identity
CString
SQLInfoGenericODBC::GetSQLGenerateSerial(CString p_table) const
{
  // NO WAY OF KNOWNING THIS / And no need to
  return "0";
}

// Gets the construction / select for the resulting effective generated serial
CString
SQLInfoGenericODBC::GetSQLEffectiveSerial(CString p_identity) const
{
  // THIS IS MOST LIKELY NOT THE CORRECT VALUE.
  // NO WAY OF DETERMINING THIS
  return p_identity;
}

// Gets the sub-transaction commands
CString
SQLInfoGenericODBC::GetSQLStartSubTransaction(CString p_savepointName) const
{
  // Generic ODBC does not known about sub transactions!
  return CString("");
}

CString
SQLInfoGenericODBC::GetSQLCommitSubTransaction(CString p_savepointName) const
{
  // Generic ODBC does not known about sub transactions!
  return CString("");
}

CString
SQLInfoGenericODBC::GetSQLRollbackSubTransaction(CString p_savepointName) const
{
  // Generic ODBC does not known about sub transactions!
  return CString("");
}

// FROM-Part for a query to select only 1 (one) record / or empty!
CString
SQLInfoGenericODBC::GetSQLFromDualClause() const
{
  // No way of knowing this in standard ODBC
  return "";
}

// Get SQL to lock  a table 
CString
SQLInfoGenericODBC::GetSQLLockTable(CString p_schema, CString p_tablename, bool p_exclusive) const
{
  // Standard ISO SQL Syntax
  CString query = "LOCK TABLE " + p_schema + "." + p_tablename + " IN ";
  query += p_exclusive ? "EXCLUSIVE" : "SHARE";
  query += " MODE";
  return query;
}

// Get query to optimize the table statistics
CString
SQLInfoGenericODBC::GetSQLOptimizeTable(CString p_schema, CString p_tablename) const
{
  return "";
}

// Transform query to select top <n> rows
CString
SQLInfoGenericODBC::GetSQLTopNRows(CString p_sql,int /*p_top*/) const
{
  // Does nothing
  return p_sql;
}

//////////////////////////////////////////////////////////////////////////
//
// SQL STRINGS
//
//////////////////////////////////////////////////////////////////////////

// Makes a SQL string from a given string, with all the right quotes
CString
SQLInfoGenericODBC::GetSQLString(const CString& p_string) const
{
  CString s = p_string;
  s.Replace("'","''");
  CString kwoot = GetKEYWORDQuoteCharacter();
  return  kwoot + s + kwoot;
}

// Get date string in engine format
CString
SQLInfoGenericODBC::GetSQLDateString(int p_year,int p_month,int p_day) const
{
  CString dateString;
  dateString.Format("{d '%04d-%02d-%02d'}",p_year,p_month,p_day);
  return dateString;
}

// Get time string in database engine format
CString
SQLInfoGenericODBC::GetSQLTimeString(int p_hour,int p_minute,int p_second) const
{
  CString retval;
  retval.Format("{t '%02d:%02d:%02d'}",p_hour,p_minute,p_second);
  return retval;
}

// Get date-time string in database engine format
CString
SQLInfoGenericODBC::GetSQLDateTimeString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  CString string;
  string.Format("{ts '%04d-%02d-%02d %02d:%02d:%02d'}"
                ,p_year,p_month,p_day // ODBC Ordering !!
                ,p_hour,p_minute,p_second);
  return string;
}

// Get date-time bound parameter string in database format
CString
SQLInfoGenericODBC::GetSQLDateTimeBoundString() const
{
  return "{ts ?}";
}

// Stripped data for the parameter binding
CString
SQLInfoGenericODBC::GetSQLDateTimeStrippedString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  CString string;
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
CString
SQLInfoGenericODBC::GetCATALOGMetaTypes(int p_type) const
{
  UNREFERENCED_PARAMETER(p_type);
  return "";
}

// ALL FUNCTIONS FOR TABLE(s)

CString
SQLInfoGenericODBC::GetCATALOGTableExists(CString p_schema,CString p_tablename) const
{
  // Cannot do this, let ODBC handle this
  return "";
}

CString
SQLInfoGenericODBC::GetCATALOGTablesList(CString p_schema,CString p_pattern) const
{
  // Cannot do this, let ODBC handle this
  return "";
}

CString
SQLInfoGenericODBC::GetCATALOGTableAttributes(CString /*p_schema*/,CString /*p_tablename*/) const
{
  // Cannot do this, let ODBC handle this
  return "";
}

CString
SQLInfoGenericODBC::GetCATALOGTableSynonyms(CString /*p_schema*/,CString /*p_tablename*/) const
{
  // Cannot do this, let ODBC handle this
  return false;
}

CString
SQLInfoGenericODBC::GetCATALOGTableCatalog(CString /*p_schema*/,CString /*p_tablename*/) const
{
  // Cannot do this, let ODBC handle this
  return false;
}

CString
SQLInfoGenericODBC::GetCATALOGTableCreate(MetaTable& /*p_table*/,MetaColumn& /*p_column*/) const
{
  return "";
}

CString
SQLInfoGenericODBC::GetCATALOGTableRename(CString p_schema,CString p_tablename,CString p_newname) const
{
  CString sql("RENAME TABLE" + p_schema + "." + p_tablename + " TO " + p_newname);
  return sql;
}

CString
SQLInfoGenericODBC::GetCATALOGTableDrop(CString p_schema,CString p_tablename) const
{
  CString sql = "DROP TABLE ";
  if(!p_schema.IsEmpty())
  {
    sql += p_schema;
    sql += ".";
  }
  sql += p_tablename;
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL TEMPORARY TABLE FUNCTIONS

CString 
SQLInfoGenericODBC::GetCATALOGTemptableCreate(CString p_schema,CString p_tablename,CString p_select) const
{
  // BEWARE: THIS IS A GUESS. NO REAL DEFINITION IN ODBC
  return "CREATE TEMPORARY TABLE " + p_schema + "." + p_tablename + "\nAS " + p_select;
}

CString 
SQLInfoGenericODBC::GetCATALOGTemptableIntoTemp(CString p_schema,CString p_tablename,CString p_select) const
{
  return "INSERT INTO " + p_schema + "." + p_tablename + "\n" + p_select + ";\n";
}

CString 
SQLInfoGenericODBC::GetCATALOGTemptableDrop(CString p_schema,CString p_tablename) const
{
  return "DROP TABLE " + p_schema + "." + p_tablename;
}

//////////////////////////////////////////////////////////////////////////
// ALL COLUMN FUNCTIONS

CString 
SQLInfoGenericODBC::GetCATALOGColumnExists(CString p_schema,CString p_tablename,CString p_columname) const
{
  // Cannot now that, use ODBC!
  return "";
}

CString 
SQLInfoGenericODBC::GetCATALOGColumnList(CString p_schema,CString p_tablename) const
{
  // Cannot now that, use ODBC!
  return "";
}

CString 
SQLInfoGenericODBC::GetCATALOGColumnAttributes(CString p_schema,CString p_tablename,CString p_columname) const
{
  // Cannot now that, use ODBC!
  return "";
}

CString 
SQLInfoGenericODBC::GetCATALOGColumnCreate(MetaColumn& p_column) const
{
  // General ISO 9075E syntax
  CString sql = "ALTER TABLE  " + p_column.m_schema + "." + p_column.m_table  + "\n"
                "  ADD COLUMN " + p_column.m_column + " " + p_column.m_typename;
  p_column.GetPrecisionAndScale(sql);
  p_column.GetNullable(sql);
  p_column.GetDefault(sql);
  // m_position not used
  // m_length   not used
  // m_remarks  not used
  return sql;
}

CString 
SQLInfoGenericODBC::GetCATALOGColumnAlter(MetaColumn& p_column) const
{
  // General ISO 9075E syntax
  CString sql = "ALTER TABLE  " + p_column.m_schema + "." + p_column.m_table  + "\n"
                "ALTER COLUMN " + p_column.m_column + " " + p_column.m_typename;
  p_column.GetPrecisionAndScale(sql);
  p_column.GetNullable(sql);
  p_column.GetDefault(sql);
  // m_position not used
  // m_length   not used
  // m_remarks  not used
  return sql;
}

CString 
SQLInfoGenericODBC::GetCATALOGColumnRename(CString p_schema,CString p_tablename,CString p_columnname,CString p_newname,CString /*p_datatype*/) const
{
  // General ISO 9075E syntax
  CString sql("ALTER  TABLE  " + p_schema + "." + p_tablename + "\n"
              "RENAME " + p_columnname + " TO " + p_newname + "\n");
  return sql;
}

CString 
SQLInfoGenericODBC::GetCATALOGColumnDrop(CString p_schema,CString p_tablename,CString p_columnname) const
{
  // General ISO 9075E syntax
  CString sql("ALTER TABLE  " + p_schema + "." + p_tablename + "\n"
              "DROP  COLUMN " + p_columnname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL INDICES FUNCTIONS

// All index functions
CString
SQLInfoGenericODBC::GetCATALOGIndexExists(CString p_schema,CString p_tablename,CString p_indexname) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLStatistics instead (see SQLInfo class)
  return "";
}

CString
SQLInfoGenericODBC::GetCATALOGIndexList(CString p_schema,CString p_tablename)   const
{
  // Cannot be implemented for generic ODBC
  // Use SQLStatistics instead (see SQLInfo class)
  return "";
}

CString
SQLInfoGenericODBC::GetCATALOGIndexAttributes(CString p_schema,CString p_tablename,CString p_indexname)  const
{
  // Cannot be implemented for generic ODBC
  // Use SQLStatistics instead (see SQLInfo class)
  return "";
}

CString
SQLInfoGenericODBC::GetCATALOGIndexCreate(MIndicesMap& p_indices) const
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
SQLInfoGenericODBC::GetCATALOGIndexDrop(CString p_schema,CString /*p_tablename*/,CString p_indexname) const
{
  CString sql = "DROP INDEX " + p_schema + "." + p_indexname;
  return sql;
}

// Get extra filter expression for an index column
CString
SQLInfoGenericODBC::GetCATALOGIndexFilter(MetaIndex& /*p_index*/) const
{
  return "";
}

//////////////////////////////////////////////////////////////////////////
// ALL PRIMARY KEY FUNCTIONS

CString
SQLInfoGenericODBC::GetCATALOGPrimaryExists(CString /*p_schema*/,CString /*p_tablename*/) const
{
  // Cannot do this, Use ODBC functions!
  return "";
}

CString
SQLInfoGenericODBC::GetCATALOGPrimaryAttributes(CString p_schema,CString p_tablename) const
{
  // Cannot do this, Use ODBC functions!
  return "";
}

CString
SQLInfoGenericODBC::GetCATALOGPrimaryCreate(MPrimaryMap& p_primaries) const
{
  // General ISO Primary key constraint
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
SQLInfoGenericODBC::GetCATALOGPrimaryDrop(CString p_schema,CString p_tablename,CString p_constraintname) const
{
  CString sql("ALTER TABLE " + p_schema + "." + p_tablename + "\n"
              " DROP CONSTRAINT " + p_constraintname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL FOREIGN KEY FUNCTIONS

CString
SQLInfoGenericODBC::GetCATALOGForeignExists(CString p_schema,CString p_tablename,CString p_constraintname) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLForeignKeys instead (see SQLInfo class)
  return "";
}

CString
SQLInfoGenericODBC::GetCATALOGForeignList(CString p_schema,CString p_tablename,int /*p_maxColumns*/ /*=SQLINFO_MAX_COLUMNS*/) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLForeignKeys instead (see SQLInfo class)
  return "";
}

CString
SQLInfoGenericODBC::GetCATALOGForeignAttributes(CString p_schema,CString p_tablename,CString p_constraintname,bool /*p_referenced = false*/,int /*p_maxColumns*/ /*=SQLINFO_MAX_COLUMNS*/) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLForeignKeys instead (see SQLInfo class)
  return "";
}

CString
SQLInfoGenericODBC::GetCATALOGForeignCreate(MForeignMap& p_foreigns) const
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
    case SQL_NOT_DEFERRABLE:      query += "\n      NOT DEFERRABLE";     break;
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
SQLInfoGenericODBC::GetCATALOGForeignAlter(MForeignMap& p_original,MForeignMap& p_requested) const
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

CString
SQLInfoGenericODBC::GetCATALOGForeignDrop(CString p_schema,CString p_tablename,CString p_constraintname) const
{
  CString sql("ALTER TABLE " + p_schema + "." + p_tablename + "\n"
              " DROP CONSTRAINT " + p_constraintname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL TRIGGER FUNCTIONS

CString
SQLInfoGenericODBC::GetCATALOGTriggerExists(CString p_schema, CString p_tablename, CString p_triggername) const
{
  // Not standard enough
  return "";
}

CString
SQLInfoGenericODBC::GetCATALOGTriggerList(CString p_schema, CString p_tablename) const
{
  // Not standard enough
  return "";
}

CString
SQLInfoGenericODBC::GetCATALOGTriggerAttributes(CString p_schema, CString p_tablename, CString p_triggername) const
{
  // Not standard enough
  return "";
}

CString
SQLInfoGenericODBC::GetCATALOGTriggerCreate(MetaTrigger& /*p_trigger*/) const
{
  // Not standard enough
  return "";
}

CString
SQLInfoGenericODBC::GetCATALOGTriggerDrop(CString p_schema, CString p_tablename, CString p_triggername) const
{
  // Not standard enough
  return "";
}

//////////////////////////////////////////////////////////////////////////
// ALL SEQUENCE FUNCTIONS

CString
SQLInfoGenericODBC::GetCATALOGSequenceExists(CString p_schema, CString p_sequence) const
{
  return "";
}

CString
SQLInfoGenericODBC::GetCATALOGSequenceList(CString p_schema,CString p_pattern) const
{
  return "";
}

CString
SQLInfoGenericODBC::GetCATALOGSequenceAttributes(CString p_schema, CString p_sequence) const
{
  return "";
}

CString
SQLInfoGenericODBC::GetCATALOGSequenceCreate(MetaSequence& /*p_sequence*/) const
{
  return "";
}

CString
SQLInfoGenericODBC::GetCATALOGSequenceDrop(CString p_schema, CString p_sequence) const
{
  return "";
}

//////////////////////////////////////////////////////////////////////////
// ALL VIEW FUNCTIONS

CString 
SQLInfoGenericODBC::GetCATALOGViewExists(CString p_schema,CString p_viewname) const
{
  return "";
}

CString 
SQLInfoGenericODBC::GetCATALOGViewList(CString p_schema,CString p_pattern) const
{
  return "";
}

CString 
SQLInfoGenericODBC::GetCATALOGViewAttributes(CString p_schema,CString p_viewname) const
{
  return "";
}

CString 
SQLInfoGenericODBC::GetCATALOGViewCreate(CString p_schema,CString p_viewname,CString p_contents) const
{
  return "CREATE VIEW " + p_schema + "." + p_viewname + "\n" + p_contents;
}

CString 
SQLInfoGenericODBC::GetCATALOGViewRename(CString p_schema,CString p_viewname,CString p_newname)    const
{
  return "";
}

CString 
SQLInfoGenericODBC::GetCATALOGViewDrop(CString p_schema,CString p_viewname,CString& p_precursor) const
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
SQLInfoGenericODBC::GetPSMProcedureExists(CString p_schema, CString p_procedure) const
{
  return "";
}

CString
SQLInfoGenericODBC::GetPSMProcedureList(CString p_schema) const
{
  return "";
}

CString
SQLInfoGenericODBC::GetPSMProcedureAttributes(CString p_schema, CString p_procedure) const
{
  return "";
}

CString
SQLInfoGenericODBC::GetPSMProcedureSourcecode(CString p_schema, CString p_procedure) const
{
  return "";
}

CString
SQLInfoGenericODBC::GetPSMProcedureCreate(MetaProcedure& /*p_procedure*/) const
{
  return "";
}

CString
SQLInfoGenericODBC::GetPSMProcedureDrop(CString p_schema, CString p_procedure) const
{
  return "";
}

CString
SQLInfoGenericODBC::GetPSMProcedureErrors(CString p_schema,CString p_procedure) const
{
  // ISO SQL does not support procedure errors
  return "";
}

// And it's parameters
CString
SQLInfoGenericODBC::GetPSMProcedureParameters(CString p_schema,CString p_procedure) const
{
  return "";
}

//////////////////////////////////////////////////////////////////////////
//
// ALL PSM LANGUAGE ELEMENTS
//
//////////////////////////////////////////////////////////////////////////

CString
SQLInfoGenericODBC::GetPSMDeclaration(bool    /*p_first*/
                                     ,CString /*p_variable*/
                                     ,int     /*p_datatype*/
                                     ,int     /*p_precision*/ /*= 0 */
                                     ,int     /*p_scale*/     /*= 0 */
                                     ,CString /*p_default*/   /*= ""*/
                                     ,CString /*p_domain*/    /*= ""*/
                                     ,CString /*p_asColumn*/  /*= ""*/) const
{
  return "";
}

CString
SQLInfoGenericODBC::GetPSMAssignment(CString p_variable,CString p_statement /*=""*/) const
{
  CString line(p_variable);
  line += " [?=] ";
  if(!p_statement.IsEmpty())
  {
    line += p_statement;
    line += ";";
  }
  return line;
}

CString
SQLInfoGenericODBC::GetPSMIF(CString p_condition) const
{
  CString line("IF (");
  line += p_condition;
  line += ") THEN\n";
  return "";
}

CString
SQLInfoGenericODBC::GetPSMIFElse() const
{
  return "  ELSE\n";
}

CString
SQLInfoGenericODBC::GetPSMIFEnd() const
{
  return "END IF;\n";
}

CString
SQLInfoGenericODBC::GetPSMWhile(CString p_condition) const
{
  return "WHILE (" + p_condition + ") LOOP\n";
}

CString
SQLInfoGenericODBC::GetPSMWhileEnd() const
{
  return "END LOOP;\n";
}

CString
SQLInfoGenericODBC::GetPSMLOOP() const
{
  return "";
}

CString
SQLInfoGenericODBC::GetPSMLOOPEnd() const
{
  return "";
}

CString
SQLInfoGenericODBC::GetPSMBREAK() const
{
  return "";
}

CString
SQLInfoGenericODBC::GetPSMRETURN(CString p_statement /*= ""*/) const
{
  return "";
}

CString
SQLInfoGenericODBC::GetPSMExecute(CString /*p_procedure*/,MParameterMap& /*p_parameters*/) const
{
  return "";
}

// The CURSOR
CString
SQLInfoGenericODBC::GetPSMCursorDeclaration(CString /*p_cursorname*/,CString /*p_select*/) const
{
  return "";
}

CString
SQLInfoGenericODBC::GetPSMCursorFetch(CString /*p_cursorname*/,std::vector<CString>& /*p_columnnames*/,std::vector<CString>& /*p_variablenames*/) const
{
  return "";
}

//////////////////////////////////////////////////////////////////////////
// PSM Exceptions

CString
SQLInfoGenericODBC::GetPSMExceptionCatchNoData() const
{
  return "";
}

CString
SQLInfoGenericODBC::GetPSMExceptionCatch(CString p_sqlState) const
{
  return "";
}

CString
SQLInfoGenericODBC::GetPSMExceptionRaise(CString p_sqlState) const
{
  return "";
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
SQLInfoGenericODBC::GetSESSIONMyself() const
{
  // Generic ISO ODBC has no info about other sessions
  return "";
}

CString
SQLInfoGenericODBC::GetSESSIONExists(CString p_sessionID) const
{
  // Generic ISO ODBC has no info about other sessions
  return "";
}

CString
SQLInfoGenericODBC::GetSESSIONList() const
{
  // Generic ISO ODBC has no info about other sessions
  return "";
}

CString
SQLInfoGenericODBC::GetSESSIONAttributes(CString p_sessionID) const
{
  // Generic ISO ODBC has no info about other sessions
  return "";
}

//////////////////////////////////////////////////////////////////////////
// Transactions

CString
SQLInfoGenericODBC::GetSESSIONConstraintsDeferred() const
{
  // ISO SQL does not know how to defer constraints
  return "";
}

CString
SQLInfoGenericODBC::GetSESSIONConstraintsImmediate() const
{
  // ISO SQL constraints are always active
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
SQLInfoGenericODBC::DoSQLCall(SQLQuery* /*p_query*/,CString& /*p_schema*/,CString& /*p_procedure*/)
{
  return nullptr;
}

// End of namespace
}
