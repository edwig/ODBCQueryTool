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

// Code prefix for a select-into-temp
CString
SQLInfoMySQL::GetSQLSelectIntoTempPrefix(CString p_tableName) const
{
  return "CREATE TEMPORARY TABLE " + p_tableName + "\nAS\n";
}

// Code suffix for after a select-into-temp
CString
SQLInfoMySQL::GetSQLSelectIntoTempSuffix(CString p_tableName) const
{
  return "";
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
SQLInfoMySQL::GetCATALOGTableExists(CString p_schema,CString p_tablename) const
{
  // Still to do in MySQL
  return "";
}

CString
SQLInfoMySQL::GetCATALOGTablesList(CString p_schema,CString p_pattern) const
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
SQLInfoMySQL::GetCATALOGTableRename(CString p_schema,CString p_tablename,CString p_newname) const
{
  CString sql("RENAME TABLE" + p_schema + "." + p_tablename + " TO " + p_newname);
  return sql;
}

CString
SQLInfoMySQL::GetCATALOGTableDrop(CString p_schema,CString p_tablename) const
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
// ALL COLUMN FUNCTIONS

CString 
SQLInfoMySQL::GetCATALOGColumnExists(CString p_schema,CString p_tablename,CString p_columname) const
{
  return "";
}

CString 
SQLInfoMySQL::GetCATALOGColumnList(CString p_schema,CString p_tablename) const
{
  return "";
}

CString 
SQLInfoMySQL::GetCATALOGColumnAttributes(CString p_schema,CString p_tablename,CString p_columname) const
{
  return "";
}

CString 
SQLInfoMySQL::GetCATALOGColumnCreate(MetaColumn& p_column) const
{
  CString sql = "ALTER TABLE "  + p_column.m_schema + "." + p_column.m_table  + "\n";
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
  CString sql = "ALTER  TABLE  " + p_column.m_schema + "." + p_column.m_table + "\n";
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
  CString sql("ALTER  TABLE  " + p_schema + "." + p_tablename + "\n"
              "RENAME " + p_columnname + " TO " + p_newname + "\n");
  return sql;
}

CString 
SQLInfoMySQL::GetCATALOGColumnDrop(CString p_schema,CString p_tablename,CString p_columnname) const
{
  CString sql("ALTER TABLE " + p_schema + "." + p_tablename + "\n"
              " DROP COLUMN " + p_columnname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL INDICES FUNCTIONS

// All index functions
CString
SQLInfoMySQL::GetCATALOGIndexExists(CString p_schema,CString p_tablename,CString p_indexname) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLStatistics instead (see SQLInfo class)
  return "";
}

CString
SQLInfoMySQL::GetCATALOGIndexList(CString p_schema,CString p_tablename) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLStatistics instead (see SQLInfo class)
  return "";
}

CString
SQLInfoMySQL::GetCATALOGIndexAttributes(CString p_schema,CString p_tablename,CString p_indexname) const
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
SQLInfoMySQL::GetCATALOGIndexDrop(CString p_schema,CString /*p_tablename*/,CString p_indexname) const
{
  CString sql = "DROP INDEX " + p_schema + "." + p_indexname;
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
SQLInfoMySQL::GetCATALOGPrimaryExists(CString p_schema,CString p_tablename) const
{
  // To be implemented
  return "";
}

CString
SQLInfoMySQL::GetCATALOGPrimaryAttributes(CString p_schema,CString p_tablename) const
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
SQLInfoMySQL::GetCATALOGPrimaryDrop(CString p_schema,CString p_tablename,CString p_constraintname) const
{
  CString sql("ALTER TABLE " + p_schema + "." + p_tablename + "\n"
              " DROP CONSTRAINT " + p_constraintname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL FOREIGN KEY FUNCTIONS

CString
SQLInfoMySQL::GetCATALOGForeignExists(CString p_schema,CString p_tablename,CString p_constraintname) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLForeignKeys instead (see SQLInfo class)
  return "";
}

CString
SQLInfoMySQL::GetCATALOGForeignList(CString p_schema,CString p_tablename,int /*p_maxColumns*/ /*=SQLINFO_MAX_COLUMNS*/) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLForeignKeys instead (see SQLInfo class)
  return "";
}

CString
SQLInfoMySQL::GetCATALOGForeignAttributes(CString p_schema,CString p_tablename,CString p_constraintname,int /*p_maxColumns*/ /*=SQLINFO_MAX_COLUMNS*/) const
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
SQLInfoMySQL::GetCATALOGForeignDrop(CString p_schema,CString p_tablename,CString p_constraintname) const
{
  CString sql("ALTER TABLE " + p_schema + "." + p_tablename + "\n"
              " DROP CONSTRAINT " + p_constraintname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL TRIGGER FUNCTIONS

CString
SQLInfoMySQL::GetCATALOGTriggerExists(CString p_schema, CString p_tablename, CString p_triggername) const
{
  return "";
}

CString
SQLInfoMySQL::GetCATALOGTriggerList(CString p_schema, CString p_tablename) const
{
  return "";
}

CString
SQLInfoMySQL::GetCATALOGTriggerAttributes(CString p_schema, CString p_tablename, CString p_triggername) const
{
  return "";
}

CString
SQLInfoMySQL::GetCATALOGTriggerCreate(MetaTrigger& /*p_trigger*/) const
{
  return "";
}

CString
SQLInfoMySQL::GetCATALOGTriggerDrop(CString p_schema, CString p_tablename, CString p_triggername) const
{
  return "";
}

//////////////////////////////////////////////////////////////////////////
// ALL SEQUENCE FUNCTIONS

CString
SQLInfoMySQL::GetCATALOGSequenceExists(CString p_schema, CString p_sequence) const
{
  return "";
}

CString
SQLInfoMySQL::GetCATALOGSequenceAttributes(CString p_schema, CString p_sequence) const
{
  return "";
}

CString
SQLInfoMySQL::GetCATALOGSequenceCreate(MetaSequence& /*p_sequence*/) const
{
  return "";
}

CString
SQLInfoMySQL::GetCATALOGSequenceDrop(CString p_schema, CString p_sequence) const
{
  return "";
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
// o PSMWORDS
//   - Declare
//   - Assignment(LET)
//   - IF statement
//   - FOR statement
//   - WHILE / LOOP statement
//   - CURSOR and friends
//
// o CALL the FUNCTION/PROCEDURE
//
//////////////////////////////////////////////////////////////////////////

CString
SQLInfoMySQL::GetPSMProcedureExists(CString p_schema, CString p_procedure) const
{
  return "";
}

CString
SQLInfoMySQL::GetPSMProcedureList(CString p_schema) const
{
  return "";
}

CString
SQLInfoMySQL::GetPSMProcedureAttributes(CString p_schema, CString p_procedure) const
{
  return "";
}

CString
SQLInfoMySQL::GetPSMProcedureCreate(MetaProcedure& /*p_procedure*/) const
{
  return "";
}

CString
SQLInfoMySQL::GetPSMProcedureDrop(CString p_schema, CString p_procedure) const
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
//     (was GetSessionAndTerminal)
//     (was GetSessionUniqueID)
// - Transactions
//   - GetSessionDeferredConstraints
//   - GetSessionImmediateConstraints
//
//////////////////////////////////////////////////////////////////////////

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
// OLD INTERFACE
//
//////////////////////////////////////////////////////////////////////////

// BOOLEANS EN STRINGS
// ====================================================================


// Get a query to create a temporary table from a select statement
CString
SQLInfoMySQL::GetSQLCreateTemporaryTable(CString& p_tablename,CString p_select) const
{
  // BEWARE: THIS IS A GUESS. NO REAL DEFINITION IN ODBC
  return "CREATE TEMPORARY TABLE " + p_tablename + "\nAS " + p_select;
}

// Get the query to remove a temporary table indefinetly
// BEWARE: Must be executed with a multi-statement stack!
CString
SQLInfoMySQL::GetSQLRemoveTemporaryTable(CString& p_tablename,int& p_number) const
{
  p_number += 1;
  return "DROP TABLE " + p_tablename + ";\n";
}

// Get a query to select into a temp table
CString
SQLInfoMySQL::GetSQLSelectIntoTemp(CString& p_tablename,CString& p_select) const
{
  return "INSERT INTO " + p_tablename + "\n" + p_select + ";\n";
}

// Gets the fact if an IF statement needs to be bordered with BEGIN/END
bool
SQLInfoMySQL::GetCodeIfStatementBeginEnd() const
{
  // IF THEN ELSE END IF; does not need a BEGIN/END per se.
  return false;
}

// Gets the end of an IF statement
CString
SQLInfoMySQL::GetCodeEndIfStatement() const
{
  return "END IF;\n";
}

// Gets a complete assignment statement.
CString
SQLInfoMySQL::GetAssignmentStatement(const CString& p_destiny,const CString& p_source) const
{
  return p_destiny + " [?=] " + p_source + ";";
}

// Get the code to start a WHILE-loop
CString
SQLInfoMySQL::GetStartWhileLoop(CString p_condition) const
{
  return "WHILE " + p_condition + " LOOP\n";
}

// Get the code to end a WHILE-loop
CString
SQLInfoMySQL::GetEndWhileLoop() const
{
  return "END LOOP;\n";
}

// Gets the fact if a SELECT must be in between parenthesis for an assignment
bool
SQLInfoMySQL::GetAssignmentSelectParenthesis() const
{
  // FALSE: value =  SELECT MAX(column) FROM table;
  // TRUE : value = (SELECT MAX(column) FROM table);
  return true;
}

// SQL CATALOG QUERIES
// ===================================================================

// Remove a stored procedure from the database
void
SQLInfoMySQL::DoRemoveProcedure(CString& /*p_procedureName*/) const
{
  // To be implemented
}

// Get SQL for your session and controlling terminal
CString
SQLInfoMySQL::GetSQLSessionAndTerminal() const
{
  // To be implemented
  return "";
}

// Get SQL to check if session number exists
CString
SQLInfoMySQL::GetSQLSessionExists(CString sessieId) const
{
  // To be implemented
  return "";
}

// Get SQL for unique session ID
CString
SQLInfoMySQL::GetSQLUniqueSessionId(const CString& /*p_databaseName*/,const CString& /*p_sessionTable*/) const
{
  // To be implemented
  return "";
}

// Get SQL for searching a session
CString
SQLInfoMySQL::GetSQLSearchSession(const CString& /*p_databaseName*/,const CString& /*p_sessionTable*/) const
{
  // To be implemented
  return "";
}

// Get a lock-table query
CString
SQLInfoMySQL::GetSQLLockTable(CString& p_tableName,bool p_exclusive) const
{
  // Standard ISO SQL Syntax
  CString query = "LOCK TABLE " + p_tableName + " IN ";
  query += p_exclusive ? "EXCLUSIVE" : "SHARE";
  query += " MODE";
  return query;
}

// Get query to optimize the table statistics
CString
SQLInfoMySQL::GetSQLOptimizeTable(CString& /*p_owner*/,CString& /*p_tableName*/,int& /*p_number*/)
{
  // To be implemented
  return "";
}

// Getting the fact that there is only **one** (1) user session in the database
bool
SQLInfoMySQL::GetOnlyOneUserSession()
{
  // No way to get the number of user sessions!
  // So always continue with management functions
  return true;
}

// SQL DDL STATEMENTS
// ==================

// Get the SQL to drop a view. If precursor is filled: run that SQL first!
CString
SQLInfoMySQL::GetSQLDropView(CString p_schema,CString p_view,CString& p_precursor)
{
  p_precursor.Empty();
  return "DROP VIEW " + p_schema + "." + p_view;
}

// Create or replace a database view
CString
SQLInfoMySQL::GetSQLCreateOrReplaceView(CString p_schema,CString p_view,CString p_asSelect) const
{
  return "CREATE VIEW " + p_schema + "." + p_view + "\n" + p_asSelect;
}

// SQL DDL ACTIONS
// ===================================================================

// Do the commit for the DDL commands in the catalog
void
SQLInfoMySQL::DoCommitDDLcommands() const
{
  // Does NOTHING In ORACLE and should do nothing
  // commit for DDL is automatic and always
}

// Do the commit for the DML commands in the database
// ODBC driver auto commit mode will go wrong!!
void
SQLInfoMySQL::DoCommitDMLcommands() const
{
}

// Does the named view exists in the database
bool
SQLInfoMySQL::DoesViewExists(CString& p_viewName)
{
  CString   errors;
  MTableMap tables;

  if(MakeInfoTableTablepart(p_viewName,tables,errors))
  {
    if(errors.IsEmpty() && !tables.empty())
    {
      return true;
    }
  }
  return false;
}

// Must create temporary tables runtime 
bool
SQLInfoMySQL::GetMustMakeTemptablesAtRuntime() const
{
  // No way to know this: it's safe to assume we must create one
  return true;
}

// Create a temporary table in an optimized manner with the given index column
void
SQLInfoMySQL::DoMakeTemporaryTable(CString& p_tableName,CString& p_content,CString& p_indexColumn) const
{
  SQLQuery query(m_database);
  p_tableName.MakeUpper();

  // Generic ISO SQL syntax
  query.TryDoSQLStatement("DROP TABLE " + p_tableName);
  CString create = "CREATE TEMPORARY TABLE " + p_tableName + "\n" + p_content;
  try
  {
    query.DoSQLStatement(create);

    if(!p_indexColumn.IsEmpty())
    {
      create = "CREATE INDEX " + p_tableName + "_" + p_indexColumn + " ON " + p_tableName + "(" + p_indexColumn + ")";
      query.DoSQLStatement(create);
    }
  }
  catch(...)
  {
    throw CString("Cannot make a temporary table: ") + p_tableName;
  }
}

// Remove a temporary table
void
SQLInfoMySQL::DoRemoveTemporaryTable(CString& p_tableName) const
{
  SQLQuery query(m_database);

  // Every error can be ignored. Can still be in use by another user and/or session
  // The table contents will then removed for this session
  query.TryDoSQLStatement("DROP TABLE " + p_tableName);
}

// Create a stored procedure in the database
void
SQLInfoMySQL::DoMakeProcedure(CString& p_procName,CString p_table,bool /*p_noParameters*/,CString& p_codeBlock)
{
  SQLQuery query(m_database);

  query.DoSQLStatement(p_codeBlock);
  query.DoSQLStatement("GRANT EXECUTE ON " + p_procName + " TO " + GetGrantedUsers());
}

// PERSISTENT-STORED MODULES (SPL / PL/SQL)
// ====================================================================

// Get the user error text from the database
CString
SQLInfoMySQL::GetUserErrorText(CString& /*p_procName*/) const
{
  return "";
}

// Get assignment to a variable in SPL
CString
SQLInfoMySQL::GetSPLAssignment(CString p_variable) const
{
  return p_variable + " [?=] ";
}

// Get the start of a SPL While loop
CString
SQLInfoMySQL::GetSPLStartWhileLoop(CString p_condition) const
{
  return "WHILE " + p_condition + " LOOP\n";
}

// Get the end of a SPL while loop
CString
SQLInfoMySQL::GetSPLEndWhileLoop() const
{
  return "END LOOP;\n";
}

// Get stored procedure call
CString
SQLInfoMySQL::GetSQLSPLCall(CString p_procName) const
{
  // General ODBC escape syntax
  return "{[?=] " + p_procName + ";}";
}

// Build a parameter list for calling a stored procedure
CString
SQLInfoMySQL::GetBuildedParameterList(size_t p_numOfParameters) const
{
  // The string of ? parameters for binding of a stored procedure
  // In ORACLE: If no parameters, no ellipsis either!
  CString strParamList;
  if(p_numOfParameters >= 0)
  {
    for(size_t i = 0; i < p_numOfParameters; i++)
    {
      if(i != 0)
      {
        strParamList += ",";
      }
      else
      {
        strParamList += "(";
      }
      strParamList += "?";
    }
    if(p_numOfParameters > 0)
    {
      strParamList += ")";
    }
  }
  return strParamList;
}


// Parameter type for stored procedure for a given column type for parameters and return types
CString
SQLInfoMySQL::GetParameterType(CString& p_type) const
{
  // No way of knowing this
  return p_type;
}

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

// Get the SPL sourcecode for a stored procedure as registered in the database
CString
SQLInfoMySQL::GetSPLSourcecodeFromDatabase(const CString& /*p_owner*/,const CString& /*p_procName*/) const
{
  // No way of knowing this
  return "";
}


// Get the SPL datatype for integer
CString
SQLInfoMySQL::GetSPLIntegerType() const
{
  return "INTEGER";
}

// Get the SPL datatype for a decimal
CString
SQLInfoMySQL::GetSPLDecimalType() const
{
  return "DECIMAL";
}

// Get the SPL declaration for a cursor
CString
SQLInfoMySQL::GetSPLCursorDeclaratie(CString& p_variableName,CString& p_query) const
{
  return "CURSOR " + p_variableName + " IS " + p_query + ";";
}

// Get the SPL cursor found row parameter
CString
SQLInfoMySQL::GetSPLCursorFound(CString& /*p_cursorName*/) const
{
  return "";
}

// Get the SPL cursor row-count variable
CString
SQLInfoMySQL::GetSPLCursorRowCount(CString& /*p_variable*/) const
{
  return "";
}

// Get the SPL datatype for a declaration of a row-variable
CString
SQLInfoMySQL::GetSPLCursorRowDeclaration(CString& /*p_cursorName*/,CString& /*p_variableName*/) const
{
  return "";;
}

CString
SQLInfoMySQL::GetSPLFetchCursorIntoVariables(CString               p_cursorName
                                                      ,CString             /*p_variableName*/
                                                      ,std::vector<CString>& p_columnNames
                                                      ,std::vector<CString>& p_variableNames) const
{
  // General ISO SYNTAX
  CString query = "FETCH " + p_cursorName + " INTO ";

  std::vector<CString>::iterator cNames;
  std::vector<CString>::iterator vNames;
  bool moreThenOne = false;

  for(cNames = p_columnNames.begin(),vNames = p_variableNames.begin();
      cNames != p_columnNames.end() && vNames != p_variableNames.end();
      ++cNames,++vNames)
  {
    query += (moreThenOne ? "," : "") + *vNames;
  }
  query += ";";
  return query;
}

// Fetch the current SPL cursor row into the row variable
CString
SQLInfoMySQL::GetSPLFetchCursorIntoRowVariable(CString& p_cursorName,CString p_variableName) const
{
  // Generic ISO SQL syntax
  return "FETCH " + p_cursorName + " INTO " + p_variableName + ";";
}

// Get the SPL no-data exception clause
CString
SQLInfoMySQL::GetSPLNoDataFoundExceptionClause() const
{
  return "";
}

// Get the SPL form of raising an exception
CString
SQLInfoMySQL::GetSPLRaiseException(CString /*p_exceptionName*/) const
{
  return "";
}

// Get the fact that the SPL has server functions that return more than 1 value
bool
SQLInfoMySQL::GetSPLServerFunctionsWithReturnValues() const
{
  // No way of knowing this, 
  // so it's safe to assume we cannot return more than 1 value
  return false;
}

// Calling a stored function or procedure if the RDBMS does not support ODBC call escapes
SQLVariant*
SQLInfoMySQL::DoSQLCall(SQLQuery* /*p_query*/,CString& /*p_schema*/,CString& /*p_procedure*/)
{
  return nullptr;
}

// SPECIALS
// ==========================================================================

// Translate database-errors to a human readable form
CString
SQLInfoMySQL::TranslateErrortext(int p_error,CString p_errorText) const
{
  CString errorText;
  errorText.Format("ODBC error [%d:%s]",p_error,p_errorText.GetString());
  return errorText;
}

// End of namespace
}
