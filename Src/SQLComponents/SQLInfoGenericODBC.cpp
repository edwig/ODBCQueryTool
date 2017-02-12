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

// Get the database type
DatabaseType 
SQLInfoGenericODBC::GetDatabaseType() const
{
  return RDBMS_ODBC_STANDARD;
}

// BOOLEANS EN STRINGS
// ====================================================================

// The name of the database vendor
CString 
SQLInfoGenericODBC::GetDatabaseVendorName() const
{
  return "Generic ODBC Driver";
}

// Get the physical database name
CString 
SQLInfoGenericODBC::GetPhysicalDatabaseName() const
{
  // See to it that "SQLDatabase:GetDatabaseName" does it's work
  return m_database->GetDatabaseName();
}

// System catalog is stored in uppercase in the database?
bool 
SQLInfoGenericODBC::IsCatalogUpper() const
{
  return true;
}

// System catalog supports full ISO schemas (same tables per schema)
bool
SQLInfoGenericODBC::GetUnderstandsSchemas() const
{
  return true;
}

// Supports database/ODBCdriver comments in sql
bool 
SQLInfoGenericODBC::SupportsDatabaseComments() const
{
  return true;
}

// Database can defer constraints until the end of a transaction
bool 
SQLInfoGenericODBC::SupportsDeferredConstraints() const
{
  // If transactions are supported we assume that constraints can be deferred
  // until the end of the transaction in the commit phase
  return m_txn_cap > 0;
}

// Database has ORDER BY with an expression, e.g. ORDER BY UPPER(column-name)
// Work-around is "SELECT UPPER(column-name) AS something.....ORDER BY something
bool 
SQLInfoGenericODBC::SupportsOrderByExpression() const
{
  // ORDER BY Expressions are not supported by GetSQLInfo
  // So do not assume that we can do this
  return false;
}

// Supports the ODBC escape sequence {[?=] CALL procedure (?,?,?)}
bool
SQLInfoGenericODBC::SupportsODBCCallEscapes() const
{
  return true;
}

// Catalog query for the default value of a table's column
CString 
SQLInfoGenericODBC::GetSQLStringDefaultValue(CString p_tableName,CString p_columnName) const
{
  // There is no way in the ODBC driver to get the default value of a column name of a table
  // So return a null SQL statement, we cannot do this
  return "";
}

// Keyword for the current date and time
CString 
SQLInfoGenericODBC::GetSystemDateTimeKeyword() const
{
  return "current_timestamp";
}

// String for the current date
CString 
SQLInfoGenericODBC::GetSystemDateString() const
{
  CString today = "current_date";
  return today;
}

// If the database does not support the datatype TIME, it can be implemented as a DECIMAL
bool 
SQLInfoGenericODBC::GetTimeIsDecimal() const
{
  // Time should be implementable
  return false;
}

// If the database does not support the datatype INTERVAL, it can be implemented as a DECIMAL
bool 
SQLInfoGenericODBC::GetIntervalIsDecimal() const
{
  // INTERVAL should be implementable
  return false;
}

// Get the concatenation operator
CString 
SQLInfoGenericODBC::GetConcatanationOperator() const
{
  // BEWARE: Officially the concatenation operator is "CONCAT(string1,string2)"
  return "||";
}

// Get quote character for strings
CString 
SQLInfoGenericODBC::GetQuoteCharacter() const
{
  return "'";
}

// Get default NULL for parameter list input
CString 
SQLInfoGenericODBC::GetDefaultNULL() const
{
  // Standard, no definition defines the NULL state
  return "";
}

// Parameter is for INPUT and OUTPUT in parameter list
CString 
SQLInfoGenericODBC::GetParameterINOUT() const
{
  return "";
}

// Parameter is for OUTPUT only in parameter list
CString 
SQLInfoGenericODBC::GetParameterOUT() const
{
  return "";
}

// Get the datatype of the audited user (h_user) in a stored procedure
CString 
SQLInfoGenericODBC::GetAuditUserDatatype() const
{
  return "VARCHAR(50)";
}

// Get the datatype of the audited user (h_user) as variable in a stored-procedure
CString 
SQLInfoGenericODBC::GetAuditUserDatatypeAsVariable() const
{
  return "VARCHAR(50)";
}

// Get datatype of the IDENTITY primary key
CString 
SQLInfoGenericODBC::GetPrimaryKeyType() const
{
  return "INTEGER";
}

// Get datatype for Moment
CString 
SQLInfoGenericODBC::GetDatetimeYearToSecondType() const
{
  return "TIMESTAMP";
}

// Separator between two alter-constraints in an alter-table statement
CString 
SQLInfoGenericODBC::GetAlterConstraintSeparator() const
{
  // Not defined, so assume most used
  return ",";
}

// Inner Join Keyword
CString 
SQLInfoGenericODBC::GetInnerJoinKeyword() const
{
  return "INNER JOIN";
}

// Outer join keyword
CString 
SQLInfoGenericODBC::GetOuterJoinKeyword() const
{
  return "LEFT OUTER JOIN";
}

// Inner Join Keyword for use in views.
CString 
SQLInfoGenericODBC::GetViewInnerJoinKeyword() const
{
  return "INNER JOIN";
}

// Outer join keyword for use in views
CString 
SQLInfoGenericODBC::GetViewOuterJoinKeyword() const
{
  return "LEFT OUTER JOIN";
}

// Get the closure for an outer-join
CString 
SQLInfoGenericODBC::GetOuterJoinClosure() const
{
  return "";
}

// Get the special Outer Join sign for the while-conditions
CString 
SQLInfoGenericODBC::GetOuterJoinSign() const
{
  // The standard only uses LEFT OUTER JOIN
  return "";
}

// Prefix for a parameter in a stored procedure
CString  
SQLInfoGenericODBC::GetSPParamPrefix() const
{
  return "";
}

// Get select part to add new record identity to a table
// Can be special column like 'OID'
CString 
SQLInfoGenericODBC::GetIdentityString(CString& /*p_tablename*/,CString /*p_postfix /*="_seq"*/) const
{
  // No definition of the identity in the standard
  return "";
}
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

// Replace the Identity column OID by a sequence.nextval
CString 
SQLInfoGenericODBC::GetReplaceColumnOIDbySequence(CString p_columns,CString p_tablename,CString p_postfix /*="_seq"*/) const
{
  // No way to do this in the ODBC standard
  return p_columns;
}

// Remove catalog dependencies for stored procedures
// To be run after a 'DROP PROCEDURE' or 'DROP FUNCTION'
CString 
SQLInfoGenericODBC::GetSQLRemoveProcedureDependencies(CString p_procname) const
{
  // No way to do this in the ODBC standard
  return "";
}

// Remove field dependencies for calculated fields (bug in Firebird)
// To be run after a 'DROP TABLE' or an 'ALTER TABLE DROP COLUMN' or 'ALTER TABLE MODIFY COLUMN'
CString 
SQLInfoGenericODBC::GetSQLRemoveFieldDependencies(CString p_tablename) const
{
  // No way to do this in the ODBC standard
  return "";
}

// Gets the table definition-form of a primary key
CString 
SQLInfoGenericODBC::GetPrimaryKeyDefinition(CString p_schema,CString p_tableName,bool /*p_temporary*/) const
{
  // The primary key constraint is not directly generated after the column
  // to ensure it will use the named index in the correct tablespace
  // Otherwise the index name and tablespace cannot be defined and will be auto-generated
  return GetPrimaryKeyType() + " NOT NULL\n";
}

// Get the constraint form of a primary key to be added to a table after creation of that table
CString 
SQLInfoGenericODBC::GetPrimaryKeyConstraint(CString p_schema,CString p_tablename,CString p_primary) const
{
  // General ISO definition of a primary key
  return "ALTER TABLE " + p_schema + "." + p_tablename + "\n"
         "  ADD CONSTRAINT pk_" + p_tablename + "\n"
         "      PRIMARY KEY (" + p_primary + ")";
}

// Get the sql to add a foreign key to a table
// This is the full ISO 9075 Implementation
CString 
SQLInfoGenericODBC::GetSQLForeignKeyConstraint(DBForeign& p_foreign) const
{
  // Construct the correct tablename
  CString table  (p_foreign.m_tablename);
  CString primary(p_foreign.m_primaryTable);
  if(!p_foreign.m_schema.IsEmpty())
  {
    table   = p_foreign.m_schema + "." + table;
    primary = p_foreign.m_schema + "." + primary;
  }

  // The base foreign key command
  CString query = "ALTER TABLE " + table + "\n"
                  "  ADD CONSTRAINT " + p_foreign.m_constraintname + "\n"
                  "      FOREIGN KEY (" + p_foreign.m_column + ")\n"
                  "      REFERENCES " + primary + "(" + p_foreign.m_primaryColumn + ")";
  // Add all relevant options
  if(p_foreign.m_deferrable)
  {
    query += "\n      DEFERRABLE";
  }
  if(p_foreign.m_initiallyDeffered)
  {
    query += "\n      INITIALLY DEFERRED";
  }
  if(p_foreign.m_match > 0)
  {
    if(p_foreign.m_match == 1)
    {
      query += "\n      MATCH PARTIAL";
    }
    if(p_foreign.m_match == 2)
    {
      query += "\n      MATCH SIMPLE";
    }
  }
  switch(p_foreign.m_updateRule)
  {
    case 1: query += "\n      ON UPDATE CASCADE";     break;
    case 2: query += "\n      ON UPDATE SET NULL";    break;
    case 3: query += "\n      ON UPDATE SET DEFAULT"; break;
    case 4: query += "\n      ON UPDATE NO ACTION";   break;
    default:// In essence: ON UPDATE RESTRICT, but that's already the default
    case 0: break;
  }
  switch(p_foreign.m_deleteRule)
  {
    case 1: query += "\n      ON DELETE CASCADE";     break;
    case 2: query += "\n      ON DELETE SET NULL";    break;
    case 3: query += "\n      ON DELETE SET DEFAULT"; break;
    case 4: query += "\n      ON DELETE NO ACTION";   break;
    default:// In essence: ON DELETE RESTRICT, but that's already the default
    case 0: break;
  }
  return query;
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

// Performance parameters to be added to the database
CString 
SQLInfoGenericODBC::GetSQLPerformanceSettings() const
{
  // No way to do this in the ODBC standard
  return "";
}

// SQL To set the caching mode of SQL results
CString 
SQLInfoGenericODBC::GetSQLCacheModeSetting(const CString& /*p_mode*/) const
{
  return "";
}

// Needed for storing numbers in stored procedures
CString 
SQLInfoGenericODBC::GetSQLNlsNumericCharacters() const
{
  // No way to do this in the ODBC standard
  return "";
}

// Gives the statement to alter a table columns' name
CString 
SQLInfoGenericODBC::GetSQLModifyColumnName(CString p_tablename,CString p_oldName,CString p_newName,CString /*p_datatype*/)
{
  // General ISO syntax
  return "ALTER  TABLE  " + p_tablename + "\n"
         "RENAME " + p_oldName   + " TO " + p_newName + "\n";
}

// Gets the maximum length of an SQL statement
unsigned long 
SQLInfoGenericODBC::GetMaxStatementLength() const
{
  return 0;		// No limit
}

// Gets the prefix needed for altering the datatype of a column in a MODIFY/ALTER
CString 
SQLInfoGenericODBC::GetModifyDatatypePrefix() const
{
  // General ISO syntax
  return "";
}

// Code to create a temporary table (qualifier)
CString 
SQLInfoGenericODBC::GetCodeTemporaryTable() const
{
  // No way of knowing this: assume general 'temporary'
  return "TEMPORARY";
}

// Code to define a table in row-locking mode
CString 
SQLInfoGenericODBC::GetCodeLockModeRow() const
{
  // No way to do this in the ODBC standard
  return "";
}

// Code to create a temporary table with no logging
CString 
SQLInfoGenericODBC::GetCodeTempTableWithNoLog() const
{
  // No way to do this in the ODBC standard
  return "";
}

// Granting all rights on a table
CString 
SQLInfoGenericODBC::GetSQLGrantAllOnTable(CString p_schema,CString p_tableName,bool p_grantOption /*= false*/)
{
  // General ISO SQL syntax
  CString sql = "GRANT ALL ON " + p_schema + "." + p_tableName + " TO " + GetGrantedUsers();
  if(p_grantOption)
  {
    sql += " WITH GRANT OPTION;\n";
  }
  return sql;
}

// Code prefix for a select-into-temp
CString 
SQLInfoGenericODBC::GetSelectIntoTempClausePrefix(CString p_tableName) const
{
  return "CREATE TEMPORARY TABLE " + p_tableName + "\nAS\n";
}

// Code suffix for after a select-into-temp
CString 
SQLInfoGenericODBC::GetSelectIntoTempClauseSuffix(CString p_tableName) const
{
  // No way to do this in the ODBC standard
  return "";
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

// Get SQL keyword to alter a column in a table
CString 
SQLInfoGenericODBC::GetCodeAlterColumn() const
{
  // Can be MODIFY or ALTER
  return "MODIFY ";
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

// Gets the UPPER function
CString 
SQLInfoGenericODBC::GetUpperFunction(CString& p_expression) const
{
  return "{fn UCASE(" + p_expression + ")}";
}

// Gets the construction for 1 minute ago
CString 
SQLInfoGenericODBC::GetInterval1MinuteAgo() const
{
  return "ERROR";
}

// Gets the construction / select for generating a new serial identity
CString 
SQLInfoGenericODBC::GetSQLGenerateSerial(CString p_table) const
{
  // NO WAY OF KNOWNING THIS
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

// Gets the Not-NULL-Value statement of the database
CString 
SQLInfoGenericODBC::GetNVLStatement(CString& p_test,CString& p_isnull) const
{
  return "{fn IFNULL(" + p_test + "," + p_isnull + ")}";
}

// Gets the sub-transaction commands
CString 
SQLInfoGenericODBC::GetStartSubTransaction(CString p_savepointName) const
{
  // Generic ODBC does not known about sub transactions!
  return CString("");
}

CString 
SQLInfoGenericODBC::GetCommitSubTransaction(CString p_savepointName) const
{
  // Generic ODBC does not known about sub transactions!
  return CString("");
}

CString 
SQLInfoGenericODBC::GetRollbackSubTransaction(CString p_savepointName) const
{
  // Generic ODBC does not known about sub transactions!
  return CString("");
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

// Part of a query to select only 1 (one) record
CString 
SQLInfoGenericODBC::GetDualTableName() const
{
  // No way of knowing this in ODBC
  return "";
}

// FROM-Part for a query to select only 1 (one) record
CString 
SQLInfoGenericODBC::GetDualClause() const
{
  // No way of knowing this in ODBC
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

// Get SQL to check if a table already exists in the database
CString 
SQLInfoGenericODBC::GetSQLTableExists(CString p_schema,CString p_tablename) const
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
  if(info->MakeInfoTableTablepart(&list,NULL,p_tablename))
  {
    if(!list.empty())
    {
      return list.front();
    }
  }
  return "";
}

// Get SQL to select all columns of a table from the catalog
CString 
SQLInfoGenericODBC::GetSQLGetColumns(CString& /*p_user*/,CString& /*p_tableName*/) const
{
  // To be implemented
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

// Get SQL to read all indices for a table
CString 
SQLInfoGenericODBC::GetSQLTableIndices(CString /*p_user*/,CString /*p_tableName*/) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLStatistics instead (see SQLInfo class)
  return "";
}

// Get SQL to create an index for a table
CString 
SQLInfoGenericODBC::GetSQLCreateIndex(CString p_user,CString p_tableName,DBIndex* p_index) const
{
  CString sql("CREATE ");
  if(p_index->m_unique)
  {
    sql += "UNIQUE ";
  }
  sql += (p_index->m_descending) ? "DESC" : "ASC";
  sql += " INDEX ON ";
  sql += p_user + ".";
  sql += p_tableName + "(";

  int column = 0;
  while(!p_index->m_indexName.IsEmpty())
  {
    if(column)
    {
      sql += ",";
    }
    sql += p_index->m_column;
    // Next column
    ++column;
    ++p_index;
  }
  sql += ")";

  return sql;
}

// Get SQL to drop an index
CString 
SQLInfoGenericODBC::GetSQLDropIndex(CString p_user,CString p_indexName) const
{
  CString sql = "DROP INDEX " + p_user + "." + p_indexName;
  return sql;
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

// See if a column exists within a table
bool   
SQLInfoGenericODBC::DoesColumnExistsInTable(CString& p_owner,CString& p_tableName,CString& p_column) const
{
  WordList tables;
  WordList columns;
  CString pattern(p_tableName);
  SQLInfo* info = (SQLInfo*)this;

  // Construct standard ODBC table pattern
  if(!p_owner.IsEmpty())
  {
    pattern = p_owner + "." + p_tableName;
  }

  if(info->MakeInfoTableTablepart(&tables,NULL,pattern))
  {
    if(info->MakeInfoTableColumns(&columns))
    {
      WordList::iterator it;
      for(it = columns.begin();it != columns.end();++it)
      {
        if(it->Find(p_column) >= 0)
        {
          return true;
        }
      }
    }
  }
  return false;
}

// Get SQL to get all the information about a Primary Key constraint
CString 
SQLInfoGenericODBC::GetSQLPrimaryKeyConstraintInformation(CString /*p_schema*/,CString /*p_tableName*/) const
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

// SQL DDL STATEMENTS
// ==================

CString
SQLInfoGenericODBC::GetCreateColumn(CString p_schema,CString p_tablename,CString p_columnName,CString p_typeDefinition,bool p_notNull)
{
  CString sql  = "ALTER TABLE "  + p_schema + "." + p_tablename  + "\n";
                 "  ADD COLUMN " + p_columnName + " " + p_typeDefinition;
  if(p_notNull)
  {
    sql += " NOT NULL";
  }
  return sql;
}

// Drop a column from a table
CString 
SQLInfoGenericODBC::GetSQLDropColumn(CString p_schema,CString p_tablename,CString p_columnName) const
{
  return "ALTER TABLE " + p_schema + "." + p_tablename + "\n"
         " DROP COLUMN " + p_columnName;
}


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

CString 
SQLInfoGenericODBC::GetModifyColumnType(CString p_schema,CString p_tablename,CString p_columnName,CString p_typeDefinition)
{
  CString sql  = "ALTER TABLE "  + p_schema + "." + p_tablename  + "\n";
                 "      MODIFY " + p_columnName + " " + p_typeDefinition;
  return sql;
}

CString 
SQLInfoGenericODBC::GetModifyColumnNull(CString p_schema,CString p_tablename,CString p_columnName,bool p_notNull)
{
  CString sql = "ALTER TABLE " + p_schema + "." + p_tablename + "\n";
                "      MODIFY " + p_columnName + (p_notNull ? "NOT " : " ") + "NULL";
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

// Remove a column from a table
void    
SQLInfoGenericODBC::DoDropColumn(CString p_tableName,CString p_columName)
{

}

// Does the named view exists in the database
bool
SQLInfoGenericODBC::DoesViewExists(CString& p_viewName)
{
  WordList list;

  if(MakeInfoTableTablepart(&list,NULL,p_viewName))
  {
    if(!list.empty())
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

// Rename a database table 
void
SQLInfoGenericODBC::DoRenameTable(CString& p_oldName,CString& p_newName) const
{
  SQLQuery query(m_database);

  CString rename = "RENAME TABLE" + p_oldName + " to " + p_newName;
  query.DoSQLStatement(rename);
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
  CString kwoot = GetQuoteCharacter();
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
  errorText.Format("ODBC error [%d:%s]",p_error,p_errorText);
  return errorText;
}

// End of namespace
}
