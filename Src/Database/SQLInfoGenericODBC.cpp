////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoGenericODBC.cpp
//
// Copyright (c) 1998- 2014 ir. W.E. Huisman
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
// Last Revision:   01-01-2015
// Version number:  1.1.0
//
#include "stdafx.h"
#include "SQLInfoGenericODBC.h"
#include "SQLQuery.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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

// Get the fysical database name
CString 
SQLInfoGenericODBC::GetFysicalDatabaseName() const
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
  SQLInfo* info = (SQLInfo*)this;
  info->GetInfo();
  // If transactions are supported we assume that constraints can be deferred
  // until the end of the transaction in the commit fase
  return m_txn_cap > 0;
}

// Database has ORDER BY with an expression, e.g. ORDER BY UPPER(columnname)
// Work-around is "SELECT UPPER(columname) AS something.....ORDER BY something
bool 
SQLInfoGenericODBC::SupportsOrderByExpression() const
{
  // ORDER BY Expressions are not supported by GetSQLInfo
  // So do not assume that we can do this
  return false;
}

// Catalogus query for the default value of a table's column
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

// If the database does not suppoprt the datatype INTERVAL, it can be implemented as a DECIMAL
bool 
SQLInfoGenericODBC::GetIntervalIsDecimal() const
{
  // INTERVAL should be implementable
  return false;
}

// Get the concatanation operator
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

// Get default NULL for parameterlist input
CString 
SQLInfoGenericODBC::GetDefaultNULL() const
{
  // Standard, no definition defines the NULL state
  return "";
}

// Parameter is for INPUT and OUTPUT in parameterlist
CString 
SQLInfoGenericODBC::GetParameterINOUT() const
{
  return "";
}

// Parameter is for OUTPUT only in parameterlist
CString 
SQLInfoGenericODBC::GetParameterOUT() const
{
  return "";
}

// Get the datatype of the auditted user (h_user) in a stored procedure
CString 
SQLInfoGenericODBC::GetAuditUserDatatype() const
{
  return "VARCHAR(50)";
}

// Get the datatype of the auditted user (h_user) as variable in a stored-procedure
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

// Get the tablespace for the tables
// Indien er geen initialisatie is ingesteld, dan wordt teruggevallen op
// de defaultwaarde "PRONTODATA". Dit is niet discriminatoir t.o.v. de 
// te bouwen applicaties.
CString 
SQLInfoGenericODBC::GetTablesTablespace(CString p_tablespace /*=""*/) const
{
  // No way to do this in the ODBC standard
  return "";
}

// Get the tablespace for the indexes
// If no default is givven, fallbacks to the DEFAULT tablespace
CString 
SQLInfoGenericODBC::GetIndexTablespace(CString p_tablespace /*=""*/) const
{
  // No way to do this in the ODBC standard
  return "";
}

// Get the storage name for indici
CString 
SQLInfoGenericODBC::GetStorageSpaceNameForIndexes() const
{
  // No way to do this in the ODBC standard
  return "";
}

// Get the storage space for temporary tables
CString 
SQLInfoGenericODBC::GetStorageSpaceNameForTempTables(CString p_tablename) const
{
  // No way to do this in the ODBC standard
  return "";
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
SQLInfoGenericODBC::GetPrimaryKeyDefinition(CString p_tableName,bool /*p_temporary*/) const
{
  // The primary key constraint is not directly generated after the column
  // to ensure it wil use the named index in the correct tablespace
  // Otherwise the index name and tablespace cannot be definied and will be auto-generated
  return GetPrimaryKeyType() + " NOT NULL\n";
}

// Get the constraint form of a primary key to be added to a table after creation of that table
CString 
SQLInfoGenericODBC::GetPrimaryKeyConstraint(CString p_tablename,bool /*p_temporary*/) const
{
  // General ISO definition of a primary key
  return "ADD CONSTRAINT pk_" + p_tablename + " PRIMARY KEY(oid)\n";
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

// Prefix for an add constraint DDL command in SQLAtlerTableGenerator
CString 
SQLInfoGenericODBC::GetAddConstraintPrefix(CString p_constraintName) const
{
  return "ADD CONSTRAINT " + p_constraintName + " ";
}

// Suffix for an add constraint DDL command in SQLAtlerTableGenerator
CString 
SQLInfoGenericODBC::GetAddConstraintSuffix(CString p_constraintName) const
{
  return "";
}

// Get the prefix for a drop constraint DDL command in the SQLAlterTableGenerator
CString 
SQLInfoGenericODBC::GetDropConstraintPrefix() const
{
  return "DROP CONSTRAINT ";
}

// Get the suffix for a drop constraint DDL commando in the SQLAlterTableGenerator
CString 
SQLInfoGenericODBC::GetDropConstraintSuffix() const
{
  return "";
}

// Clause separator between two ADD or DROP clauses in an ALTER TABLE
CString 
SQLInfoGenericODBC::GetAlterTableClauseSeparator() const
{
  return "";
}

// Grouping of more than one column possible in an ADD/MODIFY/DROP clause
bool    
SQLInfoGenericODBC::GetClauseGroupingPossible() const
{
  // NO way of knowing this through GetSQLInfo
  // So it's safe to asume that we cannot do it
  return false;
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

// Granting all rights on a table (In a NON-ANSI database)
CString 
SQLInfoGenericODBC::GetSQLGrantAllOnTable(CString p_tableName)
{
  // General ISO SQL syntax
  return "GRANT ALL ON " + p_tableName + " TO " + GetGrantedUsers() + " WITH GRANT OPTION;\n";
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

// Gets the fact if a SELECT must be inbetween parenthesis for an assignment
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

// Gets the subtransaction commands
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

// Get SQL to check if a storedprocedure already exists in the database
CString 
SQLInfoGenericODBC::GetSQLStoredProcedureExists(CString& p_name) const
{
  // No way to do this in the ODBC standard
  // To be implemeted by way of the SQLInfo calls
  SQLInfo* info = (SQLInfo*)this;

  WordList list;
  if(info->MakeInfoProcedureProcedurepart(&list,p_name))
  {
    if(!list.empty())
    {
      return list.front();
    }
  }
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
SQLInfoGenericODBC::GetSQLTableExists(CString p_tablename) const
{
  WordList list;
  SQLInfo* info = (SQLInfo*)this;

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
  // To be implemented
  return "";
}

// Get SQL to read all indici for a table
CString 
SQLInfoGenericODBC::GetSQLTableIndexes(CString& /*p_user*/,CString& /*p_tableName*/) const
{
  // To be implemented
  return "";
}

// Get SQL to read the referential constaints from the catalog
CString 
SQLInfoGenericODBC::GetSQLTableReferences(CString& /*p_tablename*/) const
{
  // To be implemented
  return "";
}

// Get the SQL Query to create a synonym
CString 
SQLInfoGenericODBC::GetSQLMakeSynonym(CString& /*p_objectName*/) const
{
  // To be implemented
  return "";
}

// Get SQL to drop the synonym
CString 
SQLInfoGenericODBC::GetSQLDropSynonym(CString& /*p_objectName*/) const
{
  // To be implemented
  return "";
}

// Create a sequence in the database
void
SQLInfoGenericODBC::DoCreateSequence(CString& /*p_sequenceName*/,int /*p_startpos*/) 
{
  // To be implemented
}

// Remove a sequence from the database
void
SQLInfoGenericODBC::DoRemoveSequence(CString& /*p_sequenceName*/) const
{
  // To be implemented
}

// Remove a stored procedure from the database
void    
SQLInfoGenericODBC::DoRemoveProcedure(CString& /*p_procedureName*/) const
{
  // To be implemented
}

// Re-Creates a sequence in a database from the OID column
void    
SQLInfoGenericODBC::DoCreateNextSequence(const CString& /*p_tableName*/,CString /*p_postfix*/ /*="_seq"*/)
{
  // To be implemented
}

// Gets the SQL for the rights on the sequence
CString 
SQLInfoGenericODBC::GetSQLSequenceRights(const CString& /*p_tableName*/,CString /*p_postfix*/ /*="_seq"*/) const
{
  // To be implemented
  return "";
}

// Get SQL for your session and controling terminal
CString 
SQLInfoGenericODBC::GetSQLSessionAndTerminal() const
{
  // To be implemented
  return "";
}

// Get SQL to check if sessionnumber exists
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

// See if a column exsists within a table
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
SQLInfoGenericODBC::GetSQLPrimaryKeyConstraintInformation(CString& /*p_tableName*/) const
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

// Geeft een lock-table query
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

// SQL DDL ACTIONS
// ===================================================================

// Do the commit for the DDL commands in the catalog
void    
SQLInfoGenericODBC::DoCommitDDLcommands() const
{
  // Does NOTHING In ORACLE and should do nothing
  // commit for DDL is autmatic and always
}

// Do the commit for the DML commands in the database
// ODBC driver autocommit mode will go wrong!!
void
SQLInfoGenericODBC::DoCommitDMLcommands() const
{
}

// Create a view from the select code and the name
void
SQLInfoGenericODBC::DoCreateOrReplaceView(CString p_code,CString p_viewName)
{
  SQLQuery query(m_database);
  query.DoSQLStatement(p_code);
}

// Remove a view from the database
void
SQLInfoGenericODBC::DoDropView(CString p_viewName)
{
  SQLQuery query(m_database);
  query.TryDoSQLStatement("DROP VIEW " + p_viewName);
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

// Must create temoporary tables runtime 
bool
SQLInfoGenericODBC::GetMustMakeTemptablesAtRuntime() const
{
  // No way to know this: it's safe to assume we must create one
  return true;
}

// Create a temporary table in an optimized manner with the givven index column
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
    // Kan geen tijdelijke tabel maken: %s
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

// If the temporary table exists, remove it
void
SQLInfoGenericODBC::DoRemoveTemporaryTableWithCheck(CString& p_tableName) const
{
  if(!GetSQLTableExists(p_tableName).IsEmpty())
  {
    DoRemoveTemporaryTable(p_tableName);
  }
}

// Maak een procedure aan in de database
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

// Length of paramters in binding
int
SQLInfoGenericODBC::GetParameterLength(int p_SQLType) const
{
  int retval;

  switch (p_SQLType)
  {
    case SQL_CHAR:                      retval = 32000;  break;
    case SQL_VARCHAR:                   retval = 32000;  break;
    case SQL_LONGVARCHAR:               retval = 32000;  break;
    case SQL_DECIMAL:                   retval = 32000;  break;
    case SQL_SMALLINT:                  retval = 0;      break;
    case SQL_INTEGER:                   retval = sizeof(long); break;
    case SQL_REAL:                      retval = 0;      break;
    case SQL_DOUBLE:                    retval = 0;      break;
    case SQL_FLOAT:                     retval = 0;      break;
    case SQL_BINARY:                    retval = 0;      break;
    case SQL_VARBINARY:                 retval = 0;      break;
    case SQL_LONGVARBINARY:             retval = 0;      break;
    case SQL_DATE:                      retval = 0;      break;
    case SQL_TIME:                      retval = 0;      break;
    case SQL_TIMESTAMP:                 retval = 19;     break;
    case SQL_NUMERIC:                   retval = 0;      break;
    case SQL_BIGINT:                    retval = 0;      break;
    case SQL_TINYINT:                   retval = 0;      break;
    case SQL_BIT:                       retval = 0;      break;
    case SQL_INTERVAL_YEAR:
    case SQL_INTERVAL_YEAR_TO_MONTH:
    case SQL_INTERVAL_MONTH:
    case SQL_INTERVAL_DAY:
    case SQL_INTERVAL_DAY_TO_HOUR:
    case SQL_INTERVAL_DAY_TO_MINUTE:
    case SQL_INTERVAL_DAY_TO_SECOND:
    case SQL_INTERVAL_HOUR:
    case SQL_INTERVAL_HOUR_TO_MINUTE:
    case SQL_INTERVAL_HOUR_TO_SECOND:
    case SQL_INTERVAL_MINUTE:
    case SQL_INTERVAL_MINUTE_TO_SECOND:
    case SQL_INTERVAL_SECOND:           retval = 25;      break;
    default:                            retval = 0;       break;
  }
  return retval;
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


// Parametertype for stored procedure for a givven columntype for parameters and return types
CString 
SQLInfoGenericODBC::GetParameterType(CString& p_type) const
{
  // No way of knowing this
  return p_type;
}

// Makes a SQL string from a givven string, with all the right quotes
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
  // so it's safe to asume we cannot return more than 1 value
  return false;
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

