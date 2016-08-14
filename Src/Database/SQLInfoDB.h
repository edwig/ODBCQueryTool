////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoDB.h
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
#pragma once
#include "SQLInfo.h"
#include "SQLDatabase.h"
#include <vector>

class SQLInfoDB : public SQLInfo
{
public:
  SQLInfoDB(SQLDatabase* p_database);
 ~SQLInfoDB();

  // DB INTERFACE

  void    SetGrantedUsers(CString& p_users);
  CString GetGrantedUsers() const;

  // PURE VIRTUAL INTERFACE

  // Get the database type
  virtual DatabaseType GetDatabaseType() const = 0;

  // BOOLEANS AND STRINGS
  
  // The name of the database vendor
  virtual CString GetDatabaseVendorName() const = 0;

  // Get the fysical database name
  virtual CString GetFysicalDatabaseName() const = 0;

  // System catalog is stored in uppercase in the database?
  virtual bool IsCatalogUpper () const = 0;
  
  // Supports database/ODBCdriver comments in sql
  virtual bool SupportsDatabaseComments() const = 0;
  
  // Database can deferr constraints until the end of a transaction
  virtual bool SupportsDeferredConstraints() const = 0;
  
  // Database has ORDER BY with an expression, e.g. ORDER BY UPPER(columnname)
  // Work-around is "SELECT UPPER(columname) AS something.....ORDER BY something
  virtual bool SupportsOrderByExpression() const = 0;

  // Catalogus query for the default value of a table's column
  virtual CString GetSQLStringDefaultValue(CString p_tableName,CString p_columnName) const = 0;

  // Keyword for the current date and time
  virtual CString GetSystemDateTimeKeyword() const = 0;
  
  // String for the current date
  virtual CString GetSystemDateString() const = 0;

  // If the database does not support the datatype TIME, it can be implemented as a DECIMAL
  virtual bool GetTimeIsDecimal() const = 0;

  // If the database does not suppoprt the datatype INTERVAL, it can be implemented as a DECIMAL
  virtual bool GetIntervalIsDecimal() const = 0;

  // Get the concatanation operator
  virtual CString GetConcatanationOperator() const = 0;

  // Get quote character for strings
  virtual CString GetQuoteCharacter() const = 0;

  // Get default NULL for parameterlist input
  virtual CString GetDefaultNULL() const = 0;
  
  // Parameter is for INPUT and OUTPUT in parameterlist
  virtual CString GetParameterINOUT() const = 0;

  // Parameter is for OUTPUT only in parameterlist
  virtual CString GetParameterOUT() const = 0;

  // Get the datatype of the auditted user (h_user) in a stored procedure
  virtual CString GetAuditUserDatatype() const = 0;

  // Get the datatype of the auditted user (h_user) as variable in a stored-procedure
  virtual CString GetAuditUserDatatypeAsVariable() const = 0;

  // Get datatype of the IDENTITY primary key
  virtual CString GetPrimaryKeyType() const = 0;
  
  // Get datatype for Moment
  virtual CString GetDatetimeYearToSecondType() const = 0;

  // Separator between two alter-constraints in an alter-table statement
  virtual CString GetAlterConstraintSeparator() const = 0;

  // Inner Join Keyword
  virtual CString GetInnerJoinKeyword() const = 0;

  // Outer join keyword
  virtual CString GetOuterJoinKeyword() const = 0;

  // Inner Join Keyword for use in views.
  virtual CString GetViewInnerJoinKeyword() const = 0;

  // Outer join keyword for use in views
  virtual CString GetViewOuterJoinKeyword() const = 0;

  // Get the closure for an outer-join
  virtual CString GetOuterJoinClosure() const = 0;

  // Get the special Outer Join sign for the while-conditions
  virtual CString GetOuterJoinSign() const = 0;

  // Prefix for a parameter in a stored procedure
  virtual CString  GetSPParamPrefix() const = 0;

  // Get select part to add new record identity to a table
  // Can be special column like 'OID'
  virtual CString GetIdentityString(CString& p_tablename,CString p_postfix = "_seq") const = 0;

  // Get a query to create a temporary table from a select statement
  virtual CString GetSQLCreateTemporaryTable(CString& p_tablename,CString p_select) const = 0;

  // Get the query to remove a temporary table indefinetly
  virtual CString GetSQLRemoveTemporaryTable(CString& p_tablename,int& p_number) const = 0;

  // Get a query to select into a temp table
  virtual CString GetSQLSelectIntoTemp(CString& p_tablename,CString& p_select) const = 0;

  // Replace the Identity column OID by a sequence.nextval
  virtual CString GetReplaceColumnOIDbySequence(CString p_columns,CString p_tablename,CString p_postfix = "_seq") const = 0;

  // Get the tablespace for the tables
  virtual CString GetTablesTablespace(CString p_tablespace = "") const = 0;

  // Get the tablespace for the indexes
  virtual CString GetIndexTablespace(CString p_tablespace = "") const = 0;

  // Get the storage name for indici
  virtual CString GetStorageSpaceNameForIndexes() const = 0;

  // Get the storage space for temporary tables
  virtual CString GetStorageSpaceNameForTempTables(CString p_tablename) const = 0;

  // Remove catalog dependencies for stored procedures
  virtual CString GetSQLRemoveProcedureDependencies(CString p_procname) const = 0;

  // Remove field dependencies for calculated fields (bug in Firebird)
  virtual CString GetSQLRemoveFieldDependencies(CString p_tablename) const = 0;

  // Gets the table definition-form of a primary key
  virtual CString GetPrimaryKeyDefinition(CString p_tableName,bool p_temporary) const = 0;

  // Get the constraint form of a primary key to be added to a table after creation of that table
  virtual CString GetPrimaryKeyConstraint(CString p_tablename,bool p_temporary) const = 0;

  // Performance parameters to be added to the database
  virtual CString GetSQLPerformanceSettings() const = 0;

  // SQL To set the caching mode of SQL results
  virtual CString GetSQLCacheModeSetting(const CString& p_mode) const = 0;

  // Needed for storing numbers in stored procedures
  virtual CString GetSQLNlsNumericCharacters() const = 0;

  // Gives the statement to alter a table columns' name
  virtual CString GetSQLModifyColumnName(CString p_tablename,CString p_oldName,CString p_newName,CString p_datatype) = 0;

  // Gets the maximum length of an SQL statement
  virtual unsigned long GetMaxStatementLength() const = 0;

  // Prefix for an add constraint DDL command in SQLAtlerTableGenerator
  virtual CString GetAddConstraintPrefix(CString p_constraintName) const = 0;

  // Suffix for an add constraint DDL command in SQLAtlerTableGenerator
  virtual CString GetAddConstraintSuffix(CString p_constraintName) const = 0;

  // Get the prefix for a drop constraint DDL command in the SQLAlterTableGenerator
  virtual CString GetDropConstraintPrefix() const = 0;

  // Get the suffix for a drop constraint DDL commando in the SQLAlterTableGenerator
  virtual CString GetDropConstraintSuffix() const = 0;

  // Clause separator between two ADD or DROP clauses in an ALTER TABLE
  virtual CString GetAlterTableClauseSeparator() const = 0;

  // Grouping of more than one column possible in an ADD/MODIFY/DROP clause
  virtual bool    GetClauseGroupingPossible() const = 0;

  // Gets the prefix needed for altering the datatype of a column in a MODIFY/ALTER
  virtual CString GetModifyDatatypePrefix() const = 0;

  // Code to create a temporary table (qualifier)
  virtual CString GetCodeTemporaryTable() const = 0;

  // Code to define a table in row-locking mode
  virtual CString GetCodeLockModeRow() const = 0;

  // Code to create a temporary table with no logging
  virtual CString GetCodeTempTableWithNoLog() const = 0;

  // Granting all rights on a table (In a NON-ANSI database)
  virtual CString GetSQLGrantAllOnTable(CString p_tableName) = 0;

  // Code prefix for a select-into-temp
  virtual CString GetSelectIntoTempClausePrefix(CString p_tableName) const = 0;

  // Code suffix for after a select-into-temp
  virtual CString GetSelectIntoTempClauseSuffix(CString p_tableName) const = 0;

  // Gets the fact if an IF statement needs to be bordered with BEGIN/END
  virtual bool    GetCodeIfStatementBeginEnd() const = 0;

  // Gets the end of an IF statement
  virtual CString GetCodeEndIfStatement() const = 0;

  // Gets a complete assignment statement.
  virtual CString GetAssignmentStatement(const CString& p_destiny,const CString& p_source) const = 0;

  // Get SQL keyword to alter a column in a table
  virtual CString GetCodeAlterColumn() const = 0;

  // Get the code to start a WHILE-loop
  virtual CString GetStartWhileLoop(CString p_condition) const = 0;

  // Get the code to end a WHILE-loop
  virtual CString GetEndWhileLoop() const = 0;

  // Gets the fact if a SELECT must be inbetween parenthesis for an assignment
  virtual bool    GetAssignmentSelectParenthesis() const = 0;

  // Gets the UPPER function
  virtual CString GetUpperFunction(CString& p_expression) const = 0;

  // Gets the construction for 1 minute ago
  virtual CString GetInterval1MinuteAgo() const = 0;

  // Gets the construction / select for generating a new serial identity
  virtual CString GetSQLGenerateSerial(CString p_table) const = 0;

  // Gets the construction / select for the resulting effective generated serial
  virtual CString GetSQLEffectiveSerial(CString p_identity) const = 0;
  
  // Gets the Not-NULL-Value statement of the database
  virtual CString GetNVLStatement(CString& p_test,CString& p_isnull) const = 0;

  // SQL CATALOG QUERIES
  // ===================

  // Get SQL to check if a storedprocedure already exists in the database
  virtual CString GetSQLStoredProcedureExists(CString& p_name) const = 0;

  // Part of a query to select only 1 (one) record
  virtual CString GetDualTableName () const = 0;

  // FROM-Part for a query to select only 1 (one) record
  virtual CString GetDualClause() const = 0;

  // Gets DEFERRABLE for a constraint (or nothing)
  virtual CString GetConstraintDeferrable () const = 0;

  // Defer Constraints until the next COMMIT;
  virtual CString GetSQLDeferConstraints() const = 0;

  // Reset constraints back to immediate
  virtual CString GetSQLConstraintsImmediate() const = 0;

  // Get SQL to check if a table already exists in the database
  virtual CString GetSQLTableExists(CString p_tablename) const = 0;

  // Get SQL to select all columns of a table from the catalog
  virtual CString GetSQLGetColumns(CString& p_user,CString& p_tableName) const = 0;

  // Get SQL to select all constraints on a table from the catalog
  virtual CString GetSQLGetConstraintsForTable(CString& p_tableName) const = 0;

  // Get SQL to read all indici for a table
  virtual CString GetSQLTableIndexes(CString& p_user,CString& p_tableName) const = 0;

  // Get SQL to read the referential constaints from the catalog
  virtual CString GetSQLTableReferences(CString& p_tablename) const = 0;

  // Get the SQL Query to create a synonym
  virtual CString GetSQLMakeSynonym(CString& p_objectName) const = 0;

  // Get SQL to drop the synonym
  virtual CString GetSQLDropSynonym(CString& p_objectname) const = 0;

  // Create a sequence in the database
  virtual void    DoCreateSequence(CString& p_sequenceName,int p_startpos) = 0;

  // Remove a sequence from the database
  virtual void    DoRemoveSequence(CString& p_sequenceName) const = 0;

  // Remove a stored procedure from the database
  virtual void    DoRemoveProcedure(CString& p_procedureName) const = 0;

  // Re-Creates a sequence in a database from the OID column
  virtual void    DoCreateNextSequence(const CString& p_tableName,CString p_postfix = "_seq") = 0;

  // Gets the SQL for the rights on the sequence
  virtual CString GetSQLSequenceRights(const CString& p_tableName,CString p_postfix = "_seq") const = 0;

  // Get SQL for your session and controling terminal
  virtual CString GetSQLSessionAndTerminal() const = 0;

  // Get SQL to check if sessionnumber exists
  virtual CString GetSQLSessionExists(CString sessieId) const = 0;

  // Get SQL for unique session ID
  virtual CString GetSQLUniqueSessionId(const CString& p_databaseName,const CString& p_sessionTable) const = 0;

  // Get SQL for searching a session
  virtual CString GetSQLSearchSession(const CString& p_databaseName,const CString& p_sessionTable) const = 0;

  // See if a column exsists within a table
  virtual bool    DoesColumnExistsInTable(CString& p_owner,CString& p_tableName,CString& p_column) const = 0;

  // Get SQL to get all the information about a Primary Key constraint
  virtual CString GetSQLPrimaryKeyConstraintInformation(CString& p_tableName) const = 0;

  // Does the named constraint exist in the database
  virtual bool    DoesConstraintExist(CString p_constraintName) const = 0;

  // Get SQL to lock  a table 
  virtual CString GetSQLLockTable(CString& p_tableName,bool p_exclusive) const = 0;

  // Get query to optimize the table statistics
  virtual CString GetSQLOptimizeTable(CString& p_owner,CString& p_tableName,int& p_number) = 0;

  // SQL DDL HANDELINGEN
  // ===================

  // Do the commit for the DDL commands in the catalog
  virtual void    DoCommitDDLcommands() const = 0;

  // Do the commit for the DML commands in the database
  virtual void    DoCommitDMLcommands() const = 0;

  // Create a view from the select code and the name
  virtual void    DoCreateOrReplaceView(CString p_code,CString p_viewName) = 0;

  // Remove a view from the database
  virtual void    DoDropView(CString p_viewName) = 0;

  // Does the named view exists in the database
  virtual bool    DoesViewExists(CString& p_viewName) = 0;

  // Must create temoporary tables runtime 
  virtual bool    GetMustMakeTemptablesAtRuntime() const = 0;

  // Create a temporary table in an optimized manner with the givven index column
  virtual void    DoMakeTemporaryTable(CString& p_tableName,CString& p_content,CString& p_indexColumn) const = 0;

  // Remove a temporary table
  virtual void    DoRemoveTemporaryTable(CString& p_tableName) const = 0;

  // If the temporary table exists, remove it
  virtual void    DoRemoveTemporaryTableWithCheck(CString& p_tableName) const = 0;

  // Maak een procedure aan in de database
  virtual void    DoMakeProcedure(CString& p_procName,CString p_table,bool p_noParameters,CString& p_codeBlock) = 0;

  // Rename a database table 
  virtual void    DoRenameTable(CString& p_oldName,CString& p_newName) const = 0;

  // PERSISTENT-STORED MODULES (SPL / PL/SQL)
  // ====================================================================

  // Get the user error text from the database
  virtual CString GetUserErrorText(CString& p_procName) const = 0;

  // Get assignment to a variable in SPL
  virtual CString GetSPLAssignment(CString p_variable) const = 0;

  // Get the start of a SPL While loop
  virtual CString GetSPLStartWhileLoop(CString p_condition) const = 0;

  // Get the end of a SPL while loop
  virtual CString GetSPLEndWhileLoop() const = 0;

  // Get stored procedure call
  virtual CString GetSQLSPLCall(CString p_procName) const = 0;

  // Length of paramters in binding
  virtual int     GetParameterLength(int p_SQLType) const = 0;

  // Build a parameter list for calling a stored procedure
  virtual CString GetBuildedParameterList(size_t p_numOfParameters) const = 0;

  // Parametertype for stored procedure for a givven columntype for parameters and return types
  virtual CString GetParameterType(CString &p_type) const = 0;

  // Makes a SQL string from a givven string, with all the right quotes
  virtual CString GetSQLString(const CString& p_string) const = 0;
  
  // Get date string in engine format
  virtual CString GetSQLDateString(int p_year,int p_month,int p_day) const = 0;

  // Get time string in database engine format
  virtual CString GetSQLTimeString(int p_hour,int p_minute,int p_second) const = 0;

  // Get date-time string in database engine format
  virtual CString GetSQLDateTimeString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const = 0;
  
  // Get date-time bound parameter string in database format
  virtual CString GetSQLDateTimeBoundString() const = 0;

  // Stripped data for the parameter binding
  virtual CString GetSQLDateTimeStrippedString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const = 0;

  // Get the SPL sourcecode for a stored procedure as registered in the database
  virtual CString GetSPLSourcecodeFromDatabase(const CString& p_owner,const CString& p_procName) const = 0;
  
  // Get the SPL datatype for integer
  virtual CString GetSPLIntegerType() const = 0;
  
  // Get the SPL datatype for a decimal
  virtual CString GetSPLDecimalType() const = 0;
  
  // Get the SPL declaration for a cursor
  virtual CString GetSPLCursorDeclaratie(CString& p_variableName,CString& p_query) const = 0;

  // Get the SPL cursor found row parameter
  virtual CString GetSPLCursorFound(CString& p_cursorName) const = 0;

  // Get the SPL cursor row-count variable
  virtual CString GetSPLCursorRowCount(CString& p_variable) const = 0;

  // Get the SPL datatype for a declaration of a row-variable
  virtual CString GetSPLCursorRowDeclaration(CString& p_cursorName,CString& p_variableName) const = 0;

  virtual CString GetSPLFetchCursorIntoVariables(CString               p_cursorName
                                                ,CString               p_variableName
                                                ,std::vector<CString>& p_columnNames
                                                ,std::vector<CString>& p_variableNames) const = 0;
                                                
  // Fetch the current SPL cursor row into the row variable
  virtual CString GetSPLFetchCursorIntoRowVariable(CString& p_cursorName,CString p_variableName) const = 0;

  // Get the SPL no-data exception clause
  virtual CString GetSPLNoDataFoundExceptionClause() const = 0;
  
  // Get the SPL form of raising an exception
  virtual CString GetSPLRaiseException(CString p_exceptionName) const = 0;

  // Get the fact that the SPL has server functions that return more than 1 value
  virtual bool    GetSPLServerFunctionsWithReturnValues() const = 0;
  
  // SPECIALS
  // ==========================================================================

  // Translate database-errors to a human readable form
  virtual CString TranslateErrortext(int p_error,CString p_errorText) const = 0;


private:
  CString m_grantedUsers;
};

inline void    
SQLInfoDB::SetGrantedUsers(CString& p_users) 
{
  m_grantedUsers = p_users;
}

inline CString 
SQLInfoDB::GetGrantedUsers() const
{
  return m_grantedUsers;
}

