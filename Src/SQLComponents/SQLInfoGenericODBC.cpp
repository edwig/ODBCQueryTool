////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoGenericODBC.cpp
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

// Code prefix for a select-into-temp
CString
SQLInfoGenericODBC::GetSQLSelectIntoTempPrefix(CString p_tableName) const
{
  return "CREATE TEMPORARY TABLE " + p_tableName + "\nAS\n";
}

// Code suffix for after a select-into-temp
CString
SQLInfoGenericODBC::GetSQLSelectIntoTempSuffix(CString p_tableName) const
{
  // No way to do this in the ODBC standard
  return "";
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

// ALL FUNCTIONS FOR TABLE(s)

CString
SQLInfoGenericODBC::GetCATALOGTableExists(CString p_schema,CString p_tablename) const
{
  WordList list;
  SQLInfo* info = (SQLInfo*)this;
  CString table(p_tablename);

  // Construct compounded name
  if(!p_schema.IsEmpty())
  {
    table = p_schema + "." + p_tablename;
  }
  // Get from SQLTables
  MTableMap tables;
  CString   errors;

  if(info->MakeInfoTableTablepart(p_tablename,tables,errors))
  {
    if(!tables.empty())
    {
      return tables.front().m_table;
    }
  }
  return "";
}

CString
SQLInfoGenericODBC::GetCATALOGTablesList(CString p_schema,CString p_pattern) const
{
  // cannot do this
  return "";
}

bool
SQLInfoGenericODBC::GetCATALOGTableAttributes(CString /*p_schema*/,CString /*p_tablename*/,MetaTable& /*p_table*/) const
{
  // cannot do this
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
  CString sql = "ALTER TABLE  " + p_column.m_schema + "." + p_column.m_table  + "\n";
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
  CString sql = "ALTER TABLE  " + p_column.m_schema + "." + p_column.m_table  + "\n";
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
  return "";
}

CString
SQLInfoGenericODBC::GetCATALOGForeignList(CString p_schema,CString p_tablename) const
{
  return "";
}

CString
SQLInfoGenericODBC::GetCATALOGForeignAttributes(CString p_schema,CString p_tablename,CString p_constraintname) const
{
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
SQLInfoGenericODBC::GetCATALOGForeignDrop(CString p_schema,CString p_tablename,CString p_constraintname) const
{
  return "";
}

//////////////////////////////////////////////////////////////////////////
//
// SQL/PSM
//
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
//
// OLD INTERFACE
//
//////////////////////////////////////////////////////////////////////////

// BOOLEANS EN STRINGS
// ====================================================================

// Get a query to create a temporary table from a select statement
CString 
SQLInfoGenericODBC::GetSQLCreateTemporaryTable(CString& p_tablename,CString p_select) const
{
  // BEWARE: THIS IS A GUESS. NO REAL DEFINITION IN ODBC
  return "CREATE TEMPORARY TABLE " + p_tablename + "\nAS " + p_select;
}

// Get the query to remove a temporary table indefinetly
// BEWARE: Must be executed with a multi-statement stack!
CString 
SQLInfoGenericODBC::GetSQLRemoveTemporaryTable(CString& p_tablename,int& p_number) const
{
  p_number += 1;
  return "DROP TABLE " + p_tablename + ";\n";
}

// Get a query to select into a temp table
CString 
SQLInfoGenericODBC::GetSQLSelectIntoTemp(CString& p_tablename,CString& p_select) const
{
  return "INSERT INTO " + p_tablename + "\n" + p_select + ";\n";
}

// Get the sql (if possible) to change the foreign key constraint
CString 
SQLInfoGenericODBC::GetSQLAlterForeignKey(DBForeign& p_origin,DBForeign& p_requested) const
{
  // Construct the correct tablename
  CString table(p_origin.m_tablename);
  if(!p_origin.m_schema.IsEmpty())
  {
    table = p_origin.m_schema + "." + table;
  }

  // The base foreign key command
  CString query = "ALTER TABLE " + table + "\n"
                  "ALTER CONSTRAINT " + p_origin.m_constraintname + "\n";

  // Add all relevant options
  if(p_origin.m_deferrable != p_requested.m_deferrable)
  {
    query.AppendFormat("\n      %sDEFERRABLE",p_requested.m_deferrable == 0 ? "NOT " : "");
  }
  if(p_origin.m_initiallyDeffered != p_requested.m_initiallyDeffered)
  {
    query += "\n      INITIALLY ";
    query += p_requested.m_initiallyDeffered ? "DEFERRED" : "IMMEDIATE";
  }
  if(p_origin.m_match != p_requested.m_match)
  {
    switch(p_requested.m_match)
    {
      case 0: query += "\n      MATCH FULL";    break;
      case 1: query += "\n      MATCH PARTIAL"; break;
      case 2: query += "\n      MATCH SIMPLE";  break;
    }
  }
  if(p_origin.m_updateRule != p_requested.m_updateRule)
  {
    switch(p_requested.m_updateRule)
    {
      case 1: query += "\n      ON UPDATE CASCADE";     break;
      case 2: query += "\n      ON UPDATE SET NULL";    break;
      case 3: query += "\n      ON UPDATE SET DEFAULT"; break;
      case 4: query += "\n      ON UPDATE NO ACTION";   break;
      default:// In essence: ON UPDATE RESTRICT, but that's already the default
      case 0: break;
    }
  }
  if(p_origin.m_deleteRule != p_requested.m_deleteRule)
  {
    switch(p_requested.m_deleteRule)
    {
      case 1: query += "\n      ON DELETE CASCADE";     break;
      case 2: query += "\n      ON DELETE SET NULL";    break;
      case 3: query += "\n      ON DELETE SET DEFAULT"; break;
      case 4: query += "\n      ON DELETE NO ACTION";   break;
      default:// In essence: ON DELETE RESTRICT, but that's already the default
      case 0: break;
    }
  }
  return query;
}

// Gets the fact if an IF statement needs to be bordered with BEGIN/END
bool
SQLInfoGenericODBC::GetCodeIfStatementBeginEnd() const
{
  // IF THEN ELSE END IF; does not need a BEGIN/END per se.
  return false;
}

// Gets the end of an IF statement
CString 
SQLInfoGenericODBC::GetCodeEndIfStatement() const
{
  return "END IF;\n";
}

// Gets a complete assignment statement.
CString 
SQLInfoGenericODBC::GetAssignmentStatement(const CString& p_destiny,const CString& p_source) const
{
  return p_destiny + " [?=] " + p_source + ";";
}

// Get the code to start a WHILE-loop
CString 
SQLInfoGenericODBC::GetStartWhileLoop(CString p_condition) const
{
  return "WHILE " + p_condition + " LOOP\n";
}

// Get the code to end a WHILE-loop
CString 
SQLInfoGenericODBC::GetEndWhileLoop() const
{
  return "END LOOP;\n";
}

// Gets the fact if a SELECT must be in between parenthesis for an assignment
bool    
SQLInfoGenericODBC::GetAssignmentSelectParenthesis() const
{
  // FALSE: value =  SELECT MAX(column) FROM table;
  // TRUE : value = (SELECT MAX(column) FROM table);
  return true;
}

// SQL CATALOG QUERIES
// ===================================================================

// Get SQL to check if a stored procedure already exists in the database
CString 
SQLInfoGenericODBC::GetSQLStoredProcedureExists(CString& /*p_name*/) const
{
  // No way to do this in the ODBC standard
  // To be implemented by way of the SQLInfo calls

  // SQLInfo* info = (SQLInfo*)this;
  return "";
}

// Gets DEFERRABLE for a constraint (or nothing)
CString 
SQLInfoGenericODBC::GetConstraintDeferrable() const
{
  if(m_txn_cap)
  {
    return " DEFERRABLE";
  }
  return "";
}

// Defer Constraints until the next COMMIT;
CString 
SQLInfoGenericODBC::GetSQLDeferConstraints() const
{
  // No way of knowing this in ODBC
  return "";
}

// Reset constraints back to immediate
CString 
SQLInfoGenericODBC::GetSQLConstraintsImmediate() const
{
  // No way of knowing this in ODBC
  return "";
}

// Get SQL to select all constraints on a table from the catalog
CString 
SQLInfoGenericODBC::GetSQLGetConstraintsForTable(CString& /*p_tableName*/) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLPrimaryKeys/SQLForeignKeys instead (see SQLInfo class)
  return "";
}

// Get SQL to read the referential constraints from the catalog
CString 
SQLInfoGenericODBC::GetSQLTableReferences(CString p_schema,CString p_tablename,CString p_constraint /*=""*/,int /*p_maxColumns = SQLINFO_MAX_COLUMNS*/) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLForeignKeys instead (see SQLInfo class)
  return "";
}

// Get the SQL to determine the sequence state in the database
CString 
SQLInfoGenericODBC::GetSQLSequence(CString /*p_schema*/,CString /*p_tablename*/,CString /*p_postfix*/) const
{
  return "";
}

// Create a sequence in the database
CString 
SQLInfoGenericODBC::GetSQLCreateSequence(CString /*p_schema*/,CString /*p_tablename*/,CString /*p_postfix = "_seq"*/,int /*p_startpos*/) const
{
  return "";
}

// Remove a sequence from the database
CString 
SQLInfoGenericODBC::GetSQLDropSequence(CString /*p_schema*/,CString /*p_tablename*/,CString /*p_postfix = "_seq"*/) const
{
  return "";
}

// Gets the SQL for the rights on the sequence
CString
SQLInfoGenericODBC::GetSQLSequenceRights(CString /*p_schema*/,CString /*p_tableName*/,CString /*p_postfix*/ /*="_seq"*/) const
{
  // To be implemented
  return "";
}

// Remove a stored procedure from the database
void    
SQLInfoGenericODBC::DoRemoveProcedure(CString& /*p_procedureName*/) const
{
  // To be implemented
}

// Get SQL for your session and controlling terminal
CString 
SQLInfoGenericODBC::GetSQLSessionAndTerminal() const
{
  // To be implemented
  return "";
}

// Get SQL to check if session number exists
CString 
SQLInfoGenericODBC::GetSQLSessionExists(CString sessieId) const
{
  // To be implemented
  return "";
}

// Get SQL for unique session ID
CString 
SQLInfoGenericODBC::GetSQLUniqueSessionId(const CString& /*p_databaseName*/,const CString& /*p_sessionTable*/) const
{
  // To be implemented
  return "";
}

// Get SQL for searching a session
CString 
SQLInfoGenericODBC::GetSQLSearchSession(const CString& /*p_databaseName*/,const CString& /*p_sessionTable*/) const
{
  // To be implemented
  return "";
}

// Does the named constraint exist in the database
bool    
SQLInfoGenericODBC::DoesConstraintExist(CString /*p_constraintName*/) const
{
  // To be implemented
  return "";
}

// Get a lock-table query
CString 
SQLInfoGenericODBC::GetSQLLockTable(CString& p_tableName,bool p_exclusive) const
{
  // Standard ISO SQL Syntax
  CString query = "LOCK TABLE " + p_tableName + " IN "  ;
  query += p_exclusive ? "EXCLUSIVE" : "SHARE";
  query += " MODE";
  return query;
}

// Get query to optimize the table statistics
CString 
SQLInfoGenericODBC::GetSQLOptimizeTable(CString& /*p_owner*/,CString& /*p_tableName*/,int& /*p_number*/)
{
  // To be implemented
  return "";
}

// Getting the fact that there is only **one** (1) user session in the database
bool
SQLInfoGenericODBC::GetOnlyOneUserSession()
{
  // No way to get the number of user sessions!
  // So always continue with management functions
  return true;
}

// Gets the triggers for a table
CString
SQLInfoGenericODBC::GetSQLTriggers(CString p_schema,CString p_table) const
{
  return "";
}

// SQL DDL STATEMENTS
// ==================

// Add a foreign key to a table
CString 
SQLInfoGenericODBC::GetCreateForeignKey(CString p_tablename,CString p_constraintname,CString p_column,CString p_refTable,CString p_primary)
{
  CString sql = "ALTER TABLE " + p_tablename + "\n"
                "  ADD CONSTRAINT " + p_constraintname + "\n"
                "      FOREIGN KEY (" + p_column + ")\n"
                "      REFERENCES " + p_refTable + "(" + p_primary + ")";
  return sql;
}

// Get the SQL to drop a view. If precursor is filled: run that SQL first!
CString 
SQLInfoGenericODBC::GetSQLDropView(CString p_schema,CString p_view,CString& p_precursor)
{
  p_precursor.Empty();
  return "DROP VIEW " + p_schema + "." + p_view;
}

// Create or replace a database view
CString 
SQLInfoGenericODBC::GetSQLCreateOrReplaceView(CString p_schema,CString p_view,CString p_asSelect) const
{
  return "CREATE VIEW " + p_schema + "." + p_view + "\n" + p_asSelect;
}

// Create or replace a trigger
CString
SQLInfoGenericODBC::CreateOrReplaceTrigger(MetaTrigger& /*p_trigger*/) const
{
  return "";
}

// SQL DDL ACTIONS
// ===================================================================

// Do the commit for the DDL commands in the catalog
void    
SQLInfoGenericODBC::DoCommitDDLcommands() const
{
  // Does NOTHING In ORACLE and should do nothing
  // commit for DDL is automatic and always
}

// Do the commit for the DML commands in the database
// ODBC driver auto commit mode will go wrong!!
void
SQLInfoGenericODBC::DoCommitDMLcommands() const
{
}

// Does the named view exists in the database
bool
SQLInfoGenericODBC::DoesViewExists(CString& p_viewName)
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
SQLInfoGenericODBC::GetMustMakeTemptablesAtRuntime() const
{
  // No way to know this: it's safe to assume we must create one
  return true;
}

// Create a temporary table in an optimized manner with the given index column
void    
SQLInfoGenericODBC::DoMakeTemporaryTable(CString& p_tableName,CString& p_content,CString& p_indexColumn) const
{
  SQLQuery query(m_database);
  p_tableName.MakeUpper();

  // Generic ISO SQL syntax
  query.TryDoSQLStatement("DROP TABLE "     + p_tableName);
  CString create = "CREATE TEMPORARY TABLE " + p_tableName + "\n" + p_content;
  try
  {
    query.DoSQLStatement(create);

    if(!p_indexColumn.IsEmpty())
    {
      create = "CREATE INDEX " + p_tableName + "_" + p_indexColumn + " ON " + p_tableName+ "(" + p_indexColumn+ ")";
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
SQLInfoGenericODBC::DoRemoveTemporaryTable(CString& p_tableName) const
{
  SQLQuery query(m_database);

  // Every error can be ignored. Can still be in use by another user and/or session
  // The table contents will then removed for this session
  query.TryDoSQLStatement("DROP TABLE " + p_tableName);
}

// Create a stored procedure in the database
void
SQLInfoGenericODBC::DoMakeProcedure(CString& p_procName,CString p_table,bool /*p_noParameters*/,CString& p_codeBlock)
{
  SQLQuery query(m_database);

  query.DoSQLStatement(p_codeBlock);
  query.DoSQLStatement("GRANT EXECUTE ON " + p_procName + " TO " + GetGrantedUsers());
}

// PERSISTENT-STORED MODULES (SPL / PL/SQL)
// ====================================================================

// Get the user error text from the database
CString
SQLInfoGenericODBC::GetUserErrorText(CString& /*p_procName*/) const
{
  return "";
}

// Get assignment to a variable in SPL
CString 
SQLInfoGenericODBC::GetSPLAssignment(CString p_variable) const
{
  return p_variable + " [?=] ";
}

// Get the start of a SPL While loop
CString 
SQLInfoGenericODBC::GetSPLStartWhileLoop(CString p_condition) const
{
  return "WHILE " + p_condition + " LOOP\n";
}

// Get the end of a SPL while loop
CString 
SQLInfoGenericODBC::GetSPLEndWhileLoop() const
{
  return "END LOOP;\n";
}

// Get stored procedure call
CString 
SQLInfoGenericODBC::GetSQLSPLCall(CString p_procName) const
{
  // General ODBC escape syntax
  return "{[?=] " + p_procName + ";}";
}

// Build a parameter list for calling a stored procedure
CString
SQLInfoGenericODBC::GetBuildedParameterList(size_t p_numOfParameters) const
{
  // The string of ? parameters for binding of a stored procedure
  // In ORACLE: If no parameters, no ellipsis either!
  CString strParamList;
  if(p_numOfParameters >= 0)
  {
    for (size_t i = 0; i < p_numOfParameters; i++)
    {
      if(i!=0) 
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
SQLInfoGenericODBC::GetParameterType(CString& p_type) const
{
  // No way of knowing this
  return p_type;
}

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

// Get the SPL sourcecode for a stored procedure as registered in the database
CString 
SQLInfoGenericODBC::GetSPLSourcecodeFromDatabase(const CString& /*p_owner*/,const CString& /*p_procName*/) const
{
  // No way of knowing this
  return "";
}


// Get the SPL datatype for integer
CString 
SQLInfoGenericODBC::GetSPLIntegerType() const
{
  return "INTEGER";
}

// Get the SPL datatype for a decimal
CString 
SQLInfoGenericODBC::GetSPLDecimalType() const
{
  return "DECIMAL";
}

// Get the SPL declaration for a cursor
CString 
SQLInfoGenericODBC::GetSPLCursorDeclaratie(CString& p_variableName,CString& p_query) const
{
  return "CURSOR " + p_variableName + " IS " + p_query + ";";
}

// Get the SPL cursor found row parameter
CString 
SQLInfoGenericODBC::GetSPLCursorFound(CString& /*p_cursorName*/) const
{
  return "";
}

// Get the SPL cursor row-count variable
CString 
SQLInfoGenericODBC::GetSPLCursorRowCount(CString& /*p_variable*/) const
{
  return "";
}

// Get the SPL datatype for a declaration of a row-variable
CString 
SQLInfoGenericODBC::GetSPLCursorRowDeclaration(CString& /*p_cursorName*/,CString& /*p_variableName*/) const
{
  return "";;
}

CString 
SQLInfoGenericODBC::GetSPLFetchCursorIntoVariables(CString               p_cursorName
                                              ,CString             /*p_variableName*/
                                              ,std::vector<CString>& p_columnNames
                                              ,std::vector<CString>& p_variableNames) const
{
  // General ISO SYNTAX
  CString query = "FETCH " + p_cursorName + " INTO ";  

  std::vector<CString>::iterator cNames;
  std::vector<CString>::iterator vNames;
  bool moreThenOne = false;

  for(cNames  = p_columnNames.begin(), vNames  = p_variableNames.begin();
    cNames != p_columnNames.end() && vNames != p_variableNames.end();
    ++cNames, ++vNames)
  {
    query += (moreThenOne ? "," : "") + *vNames;
  }
  query += ";";
  return query;
}

// Fetch the current SPL cursor row into the row variable
CString 
SQLInfoGenericODBC::GetSPLFetchCursorIntoRowVariable(CString& p_cursorName,CString p_variableName) const
{ 
  // Generic ISO SQL syntax
  return "FETCH " + p_cursorName + " INTO " + p_variableName+ ";";
}

// Get the SPL no-data exception clause
CString 
SQLInfoGenericODBC::GetSPLNoDataFoundExceptionClause() const
{
  return "";
}

// Get the SPL form of raising an exception
CString 
SQLInfoGenericODBC::GetSPLRaiseException(CString /*p_exceptionName*/) const
{
  return "";
}

// Get the fact that the SPL has server functions that return more than 1 value
bool    
SQLInfoGenericODBC::GetSPLServerFunctionsWithReturnValues() const
{
  // No way of knowing this, 
  // so it's safe to assume we cannot return more than 1 value
  return false;
}

// Calling a stored function or procedure if the RDBMS does not support ODBC call escapes
SQLVariant*
SQLInfoGenericODBC::DoSQLCall(SQLQuery* /*p_query*/,CString& /*p_schema*/,CString& /*p_procedure*/)
{
  return nullptr;
}

// SPECIALS
// ==========================================================================

// Translate database-errors to a human readable form
CString 
SQLInfoGenericODBC::TranslateErrortext(int p_error,CString p_errorText) const
{
  CString errorText;
  errorText.Format("ODBC error [%d:%s]",p_error,p_errorText.GetString());
  return errorText;
}

// End of namespace
}
