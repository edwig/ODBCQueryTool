////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoMySQL.cpp
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
CString
SQLInfoMySQL::GetRDBMSVendorName() const
{
  // The name of the database vendor
  return "MySQL A.B. / Oracle";
}

// Get the physical database name
CString
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

// KEYWORDS

// Keyword for the current date and time
CString
SQLInfoMySQL::GetKEYWORDCurrentTimestamp() const
{
  return "current_timestamp";
}

// String for the current date
CString
SQLInfoMySQL::GetKEYWORDCurrentDate() const
{
  return "current_date";
}

// Get the concatenation operator
CString
SQLInfoMySQL::GetKEYWORDConcatanationOperator() const
{
  // BEWARE: Officially the concatenation operator is "CONCAT(string1,string2)"
  // MYSQL supports 'one' 'two' concatenation of two strings (no operator)
  return "";
}

// Get quote character for strings
CString
SQLInfoMySQL::GetKEYWORDQuoteCharacter() const
{
  return "\'";
}

// Get default NULL for parameter list input
CString
SQLInfoMySQL::GetKEYWORDParameterDefaultNULL() const
{
  // Standard, no definition defines the NULL state
  return "";
}

// Parameter is for INPUT and OUTPUT in parameter list
CString
SQLInfoMySQL::GetKEYWORDParameterINOUT() const
{
  return "";
}

// Parameter is for OUTPUT only in parameter list
CString
SQLInfoMySQL::GetKEYWORDParameterOUT() const
{
  return "";
}

// Get datatype of the IDENTITY primary key in a Network database
CString
SQLInfoMySQL::GetKEYWORDNetworkPrimaryKeyType() const
{
  // Use SEQUENCE to fill!
  return "INTEGER";
}

// Get datatype for timestamp (year to second)
CString
SQLInfoMySQL::GetKEYWORDTypeTimestamp() const
{
  return "TIMESTAMP";
}

// Prefix for a parameter in a stored procedure
CString
SQLInfoMySQL::GetKEYWORDParameterPrefix() const
{
  return "";
}

// Get select part to add new record identity to a table
// Can be special column like 'OID' or a sequence select
CString
SQLInfoMySQL::GetKEYWORDIdentityString(CString& p_tablename,CString p_postfix /*= "_seq"*/) const
{
  return p_tablename + p_postfix + ".nextval";
}

// Gets the UPPER function
CString
SQLInfoMySQL::GetKEYWORDUpper(CString& p_expression) const
{
  return "UPPER(" + p_expression + ")";
}

// Gets the construction for 1 minute ago
CString
SQLInfoMySQL::GetKEYWORDInterval1MinuteAgo() const
{
  return "ERROR";
}

// Gets the Not-NULL-Value statement of the database
CString
SQLInfoMySQL::GetKEYWORDStatementNVL(CString& p_test,CString& p_isnull) const
{
  return "{fn IFNULL(" + p_test + "," + p_isnull + ")}";
}

// Gets the construction / select for generating a new serial identity
CString
SQLInfoMySQL::GetSQLGenerateSerial(CString p_table) const
{
  // NO WAY OF KNOWNING THIS
  return "0";
}

// Gets the construction / select for the resulting effective generated serial
CString
SQLInfoMySQL::GetSQLEffectiveSerial(CString p_identity) const
{
  // THIS IS MOST LIKELY NOT THE CORRECT VALUE.
  // NO WAY OF DETERMINING THIS
  return p_identity;
}

// Gets the sub-transaction commands
CString
SQLInfoMySQL::GetSQLStartSubTransaction(CString p_savepointName) const
{
  // Generic ODBC does not known about sub transactions!
  return CString("");
}

CString
SQLInfoMySQL::GetSQLCommitSubTransaction(CString p_savepointName) const
{
  // Generic ODBC does not known about sub transactions!
  return CString("");
}

CString
SQLInfoMySQL::GetSQLRollbackSubTransaction(CString p_savepointName) const
{
  // Generic ODBC does not known about sub transactions!
  return CString("");
}

// FROM-Part for a query to select only 1 (one) record / or empty!
CString
SQLInfoMySQL::GetSQLFromDualClause() const
{
  // MySQL does bare SELECT!
  return "";
}

// Get SQL to lock  a table 
CString
SQLInfoMySQL::GetSQLLockTable(CString /*p_schema*/, CString p_tablename, bool p_exclusive) const
{
  // Standard ISO SQL Syntax
  CString query = "LOCK TABLE " + p_tablename + " IN ";
  query += p_exclusive ? "EXCLUSIVE" : "SHARE";
  query += " MODE";
  return query;
}

// Get query to optimize the table statistics
CString
SQLInfoMySQL::GetSQLOptimizeTable(CString p_schema, CString p_tablename) const
{
  // To be implemented
  return "";
}

//////////////////////////////////////////////////////////////////////////
//
// SQL STRINGS
//
//////////////////////////////////////////////////////////////////////////

// Makes a SQL string from a given string, with all the right quotes
CString
SQLInfoMySQL::GetSQLString(const CString& p_string) const
{
  CString s = p_string;
  s.Replace("'","''");
  CString kwoot = GetKEYWORDQuoteCharacter();
  return  kwoot + s + kwoot;
}

// Get date string in engine format
CString
SQLInfoMySQL::GetSQLDateString(int p_year,int p_month,int p_day) const
{
  CString dateString;
  dateString.Format("{d '%04d-%02d-%02d'}",p_year,p_month,p_day);
  return dateString;
}

// Get time string in database engine format
CString
SQLInfoMySQL::GetSQLTimeString(int p_hour,int p_minute,int p_second) const
{
  CString retval;
  retval.Format("{t '%02d:%02d:%02d'}",p_hour,p_minute,p_second);
  return retval;
}

// Get date-time string in database engine format
CString
SQLInfoMySQL::GetSQLDateTimeString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  CString string;
  string.Format("{ts '%04d-%02d-%02d %02d:%02d:%02d'}"
                ,p_year,p_month,p_day // ODBC Ordering !!
                ,p_hour,p_minute,p_second);
  return string;
}

// Get date-time bound parameter string in database format
CString
SQLInfoMySQL::GetSQLDateTimeBoundString() const
{
  return "{ts ?}";
}

// Stripped data for the parameter binding
CString
SQLInfoMySQL::GetSQLDateTimeStrippedString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
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

// Get SQL to check if a table already exists in the database
CString
SQLInfoMySQL::GetCATALOGTableExists(CString /*p_schema*/,CString p_tablename) const
{
  // Still to do in MySQL
  return "";
}

CString
SQLInfoMySQL::GetCATALOGTablesList(CString /*p_schema*/,CString p_pattern) const
{
  // MS-Access cannot do this
  return "";
}

bool
SQLInfoMySQL::GetCATALOGTableAttributes(CString /*p_schema*/,CString /*p_tablename*/,MetaTable& /*p_table*/) const
{
  // Getting the temp table status
  return false;
}

CString
SQLInfoMySQL::GetCATALOGTableCreate(MetaTable& /*p_table*/,MetaColumn& /*p_column*/) const
{
  return "";
}

CString
SQLInfoMySQL::GetCATALOGTableRename(CString /*p_schema*/,CString p_tablename,CString p_newname) const
{
  CString sql("RENAME TABLE" + p_tablename + " TO " + p_newname);
  return sql;
}

CString
SQLInfoMySQL::GetCATALOGTableDrop(CString /*p_schema*/,CString p_tablename) const
{
  CString sql("DROP TABLE " + p_tablename);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL TEMPORARY TABLE FUNCTIONS

CString 
SQLInfoMySQL::GetCATALOGTemptableCreate(CString /*p_schema*/,CString p_tablename,CString p_select) const
{
  // BEWARE: THIS IS A GUESS. 
  return "CREATE TEMPORARY TABLE " + p_tablename + "\nAS " + p_select;
}

CString 
SQLInfoMySQL::GetCATALOGTemptableIntoTemp(CString /*p_schema*/,CString p_tablename,CString p_select) const
{
  return "INSERT INTO " + p_tablename + "\n" + p_select;
}

CString 
SQLInfoMySQL::GetCATALOGTemptableDrop(CString /*p_schema*/,CString p_tablename) const
{
  return "DROP TABLE " + p_tablename;
}

//////////////////////////////////////////////////////////////////////////
// ALL COLUMN FUNCTIONS

CString 
SQLInfoMySQL::GetCATALOGColumnExists(CString /*p_schema*/,CString p_tablename,CString p_columname) const
{
  return "";
}

CString 
SQLInfoMySQL::GetCATALOGColumnList(CString /*p_schema*/,CString p_tablename) const
{
  return "";
}

CString 
SQLInfoMySQL::GetCATALOGColumnAttributes(CString /*p_schema*/,CString p_tablename,CString p_columname) const
{
  return "";
}

CString 
SQLInfoMySQL::GetCATALOGColumnCreate(MetaColumn& p_column) const
{
  CString sql = "ALTER TABLE "  + p_column.m_table  + "\n";
                "  ADD COLUMN " + p_column.m_column + " " + p_column.m_typename;
  p_column.GetPrecisionAndScale(sql);
  p_column.GetNullable(sql);
  p_column.GetDefault(sql);
  // m_position not used
  // m_length   not used
  // m_remarks  not used
  return sql;}

CString 
SQLInfoMySQL::GetCATALOGColumnAlter(MetaColumn& p_column) const
{
  // The MODIFY keyword is a-typical
  CString sql = "ALTER  TABLE  " + p_column.m_table + "\n";
                "MODIFY COLUMN " + p_column.m_column + " " + p_column.m_typename;
  p_column.GetPrecisionAndScale(sql);
  p_column.GetNullable(sql);
  p_column.GetDefault(sql);
  // m_position not used
  // m_length   not used
  // m_remarks  not used
  return sql;
}

CString 
SQLInfoMySQL::GetCATALOGColumnRename(CString p_schema,CString p_tablename,CString p_columnname,CString p_newname,CString /*p_datatype*/) const
{
  // General ISO syntax
  CString sql("ALTER  TABLE  " + p_tablename + "\n"
              "RENAME " + p_columnname + " TO " + p_newname + "\n");
  return sql;
}

CString 
SQLInfoMySQL::GetCATALOGColumnDrop(CString p_schema,CString p_tablename,CString p_columnname) const
{
  CString sql("ALTER TABLE  " + p_tablename + "\n"
              " DROP COLUMN " + p_columnname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL INDICES FUNCTIONS

// All index functions
CString
SQLInfoMySQL::GetCATALOGIndexExists(CString /*p_schema*/,CString p_tablename,CString p_indexname) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLStatistics instead (see SQLInfo class)
  return "";
}

CString
SQLInfoMySQL::GetCATALOGIndexList(CString /*p_schema*/,CString p_tablename) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLStatistics instead (see SQLInfo class)
  return "";
}

CString
SQLInfoMySQL::GetCATALOGIndexAttributes(CString /*p_schema*/,CString p_tablename,CString p_indexname) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLStatistics instead (see SQLInfo class)
  return "";
}

CString
SQLInfoMySQL::GetCATALOGIndexCreate(MIndicesMap& p_indices) const
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
SQLInfoMySQL::GetCATALOGIndexDrop(CString /*p_schema*/,CString /*p_tablename*/,CString p_indexname) const
{
  CString sql = "DROP INDEX " + p_indexname;
  return sql;
}

// Get extra filter expression for an index column
CString
SQLInfoMySQL::GetCATALOGIndexFilter(MetaIndex& /*p_index*/) const
{
  return "";
}

//////////////////////////////////////////////////////////////////////////
// ALL PRIMARY KEY FUNCTIONS

CString
SQLInfoMySQL::GetCATALOGPrimaryExists(CString /*p_schema*/,CString p_tablename) const
{
  // To be implemented
  return "";
}

CString
SQLInfoMySQL::GetCATALOGPrimaryAttributes(CString /*p_schema*/,CString p_tablename) const
{
  // To be implemented
  return "";
}

CString
SQLInfoMySQL::GetCATALOGPrimaryCreate(MPrimaryMap& p_primaries) const
{
  CString query("ALTER TABLE ");

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

CString
SQLInfoMySQL::GetCATALOGPrimaryDrop(CString /*p_schema*/,CString p_tablename,CString p_constraintname) const
{
  CString sql("ALTER TABLE " + p_tablename + "\n"
              " DROP CONSTRAINT " + p_constraintname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL FOREIGN KEY FUNCTIONS

CString
SQLInfoMySQL::GetCATALOGForeignExists(CString /*p_schema*/,CString p_tablename,CString p_constraintname) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLForeignKeys instead (see SQLInfo class)
  return "";
}

CString
SQLInfoMySQL::GetCATALOGForeignList(CString /*p_schema*/,CString p_tablename,int /*p_maxColumns*/ /*=SQLINFO_MAX_COLUMNS*/) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLForeignKeys instead (see SQLInfo class)
  return "";
}

CString
SQLInfoMySQL::GetCATALOGForeignAttributes(CString /*p_schema*/,CString p_tablename,CString p_constraintname,int /*p_maxColumns*/ /*=SQLINFO_MAX_COLUMNS*/) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLForeignKeys instead (see SQLInfo class)
  return "";
}

CString
SQLInfoMySQL::GetCATALOGForeignCreate(MForeignMap& p_foreigns) const
{
  // Get first record
  MetaForeign& foreign = p_foreigns.front();

  // Construct the correct tablename
  CString table(foreign.m_fkTableName);
  CString primary(foreign.m_pkTableName);

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

CString
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
  CString table(original.m_fkTableName);

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
SQLInfoMySQL::GetCATALOGForeignDrop(CString /*p_schema*/,CString p_tablename,CString p_constraintname) const
{
  CString sql("ALTER TABLE " + p_tablename + "\n"
              " DROP CONSTRAINT " + p_constraintname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL TRIGGER FUNCTIONS

CString
SQLInfoMySQL::GetCATALOGTriggerExists(CString /*p_schema*/, CString p_tablename, CString p_triggername) const
{
  return "";
}

CString
SQLInfoMySQL::GetCATALOGTriggerList(CString /*p_schema*/, CString p_tablename) const
{
  return "";
}

CString
SQLInfoMySQL::GetCATALOGTriggerAttributes(CString /*p_schema*/, CString p_tablename, CString p_triggername) const
{
  return "";
}

CString
SQLInfoMySQL::GetCATALOGTriggerCreate(MetaTrigger& /*p_trigger*/) const
{
  return "";
}

CString
SQLInfoMySQL::GetCATALOGTriggerDrop(CString /*p_schema*/, CString p_tablename, CString p_triggername) const
{
  return "";
}

//////////////////////////////////////////////////////////////////////////
// ALL SEQUENCE FUNCTIONS

CString
SQLInfoMySQL::GetCATALOGSequenceExists(CString /*p_schema*/, CString p_sequence) const
{
  return "";
}

CString
SQLInfoMySQL::GetCATALOGSequenceAttributes(CString /*p_schema*/, CString p_sequence) const
{
  return "";
}

CString
SQLInfoMySQL::GetCATALOGSequenceCreate(MetaSequence& /*p_sequence*/) const
{
  return "";
}

CString
SQLInfoMySQL::GetCATALOGSequenceDrop(CString /*p_schema*/, CString p_sequence) const
{
  return "";
}

//////////////////////////////////////////////////////////////////////////
// ALL VIEW FUNCTIONS

CString 
SQLInfoMySQL::GetCATALOGViewExists(CString p_schema,CString p_viewname) const
{
  return "";
}

CString 
SQLInfoMySQL::GetCATALOGViewList(CString p_schema,CString p_pattern) const
{
  return "";
}

CString 
SQLInfoMySQL::GetCATALOGViewAttributes(CString p_schema,CString p_viewname) const
{
  return "";
}

CString 
SQLInfoMySQL::GetCATALOGViewCreate(CString /*p_schema*/,CString p_viewname,CString p_contents) const
{
  return "CREATE VIEW " + p_viewname + "\n" + p_contents;
}

CString 
SQLInfoMySQL::GetCATALOGViewRename(CString p_schema,CString p_viewname,CString p_newname)    const
{
  return "";
}

CString 
SQLInfoMySQL::GetCATALOGViewDrop(CString /*p_schema*/,CString p_viewname,CString& p_precursor) const
{
  p_precursor.Empty();
  return "DROP VIEW " + p_viewname;
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
SQLInfoMySQL::GetPSMProcedureExists(CString /*p_schema*/, CString p_procedure) const
{
  return "";
}

CString
SQLInfoMySQL::GetPSMProcedureList(CString /*p_schema*/) const
{
  return "";
}

CString
SQLInfoMySQL::GetPSMProcedureAttributes(CString /*p_schema*/, CString p_procedure) const
{
  return "";
}

CString
SQLInfoMySQL::GetPSMProcedureCreate(MetaProcedure& /*p_procedure*/) const
{
  return "";
}

CString
SQLInfoMySQL::GetPSMProcedureDrop(CString /*p_schema*/, CString p_procedure) const
{
  return "";
}

CString
SQLInfoMySQL::GetPSMProcedureErrors(CString p_schema,CString p_procedure) const
{
  return "";
}

//////////////////////////////////////////////////////////////////////////
//
// ALL PSM LANGUAGE ELEMENTS
//
//////////////////////////////////////////////////////////////////////////

CString
SQLInfoMySQL::GetPSMDeclaration(bool    /*p_first*/
                               ,CString p_variable
                               ,int     p_datatype
                               ,int     p_precision /*= 0 */
                               ,int     p_scale     /*= 0 */
                               ,CString p_default   /*= ""*/
                               ,CString /*p_domain    = ""*/
                               ,CString /*p_asColumn  = ""*/) const
{
  CString line;
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

CString
SQLInfoMySQL::GetPSMAssignment(CString p_variable,CString p_statement /*=""*/) const
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
SQLInfoMySQL::GetPSMIF(CString p_condition) const
{
  CString line("IF (");
  line += p_condition;
  line += ") THEN\n";
  return line;
}

CString
SQLInfoMySQL::GetPSMIFElse() const
{
  return "ELSE\n";
}

CString
SQLInfoMySQL::GetPSMIFEnd() const
{
  return "END IF;\n";
}

CString
SQLInfoMySQL::GetPSMWhile(CString p_condition) const
{
  return "WHILE (" + p_condition + ") DO\n";
}

CString
SQLInfoMySQL::GetPSMWhileEnd() const
{
  return "END WHILE;\n";
}

CString
SQLInfoMySQL::GetPSMLOOP() const
{
  return "LOOP\n";
}

CString
SQLInfoMySQL::GetPSMLOOPEnd() const
{
  return "END LOOP;\n";
}

CString
SQLInfoMySQL::GetPSMBREAK() const
{
  return "LEAVE\n";
}

CString
SQLInfoMySQL::GetPSMRETURN(CString p_statement /*= ""*/) const
{
  return "RETURN " + p_statement;
}

CString
SQLInfoMySQL::GetPSMExecute(CString p_procedure,MParameterMap& p_parameters) const
{
  CString line;
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
CString
SQLInfoMySQL::GetPSMCursorDeclaration(CString p_cursorname,CString p_select) const
{
  return "DECLARE " + p_cursorname + " CURSOR FOR " + p_select + ";";
}

CString
SQLInfoMySQL::GetPSMCursorFetch(CString p_cursorname,std::vector<CString>& /*p_columnnames*/,std::vector<CString>& p_variablenames) const
{
  CString query = "FETCH " + p_cursorname + " INTO ";
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

CString
SQLInfoMySQL::GetPSMExceptionCatchNoData() const
{
  return "DECLARE EXIT HANDLER FOR SQLSTATE '02000'\n";
  // Must be followed by a (block)statement
}

CString
SQLInfoMySQL::GetPSMExceptionCatch(CString p_sqlState) const
{
  return "DECLARE EXIT HANDLER FOR SQLSTATE '" + p_sqlState + "'\n";
  // Must be followed by a (block)statement
}

CString
SQLInfoMySQL::GetPSMExceptionRaise(CString p_sqlState) const
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

CString
SQLInfoMySQL::GetSESSIONMyself() const
{
  return "";
}

CString
SQLInfoMySQL::GetSESSIONExists(CString p_sessionID) const
{
  return "";
}

CString
SQLInfoMySQL::GetSESSIONList() const
{
  return "";
}

CString
SQLInfoMySQL::GetSESSIONAttributes(CString p_sessionID) const
{
  return "";
}

//////////////////////////////////////////////////////////////////////////
// Transactions

CString
SQLInfoMySQL::GetSESSIONConstraintsDeferred() const
{
  // MySQL cannot defer constraints
  return "";
}

CString
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
SQLInfoMySQL::DoSQLCall(SQLQuery* /*p_query*/,CString& /*p_schema*/,CString& /*p_procedure*/)
{
  return nullptr;
}

// End of namespace
}
