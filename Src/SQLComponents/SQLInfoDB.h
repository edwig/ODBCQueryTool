////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoDB.h
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
#pragma once
#include "SQLInfo.h"
#include "SQLDatabase.h"
#include <vector>

//////////////////////////////////////////////////////////////////////////
//
// DESIGN RULES FOR THE DBInfo interface
// A)  Only SQL returned(not run)
// B)  If table, also schema included
// C)  Override van de SQL<schema> functions
// D)  One action per SQL (no multiple constraints etc)
//
//////////////////////////////////////////////////////////////////////////

namespace SQLComponents
{

class SQLVariant;
class SQLQuery;

// Maximum supported columns in an index or a foreign key
// Does not constrict every SQL RDBMS's, but some have multiple!!
#define SQLINFO_MAX_COLUMNS  8

// DBForeign is used to store the current state 
// of the database foreign key constraints of a table
typedef struct _dbForeign
{
  CString m_constraintname;
  CString m_schema;
  CString m_tablename;
  CString m_column;
  CString m_primaryTable;
  CString m_primaryColumn;
  bool    m_enabled;            // yes/no
  bool    m_deferrable;         // yes/no
  bool    m_initiallyDeffered;  // yes/no
  int     m_match;              // 0=Full, 1=partial, 2=simple
  int     m_updateRule;         // 0=restrict, 1=cascade, 2=set null, 3=set default, 4=No action
  int     m_deleteRule;         // 0=restrict, 1=cascade, 2=set null, 3=set default, 4=No action
}
DBForeign;

// DBIndex is used to store the current state
// of the database indices and columns of a table
typedef struct _dbIndex
{
  CString m_indexName;    // Name of the total index
  CString m_column;       // Column name in the index
  int     m_position;     // Position of column in the index
  bool    m_unique;       // 1 = unique, null,0 if not
  bool    m_descending;   // true if DESC, otherwise ASC
  CString m_source;       // Optional expression source
}
DBIndex;

class SQLInfoDB : public SQLInfo
{
public:
  SQLInfoDB(SQLDatabase* p_database);
 ~SQLInfoDB();

  // DB INTERFACE

  void    SetGrantedUsers(CString p_users);
  CString GetGrantedUsers() const;

  // OVERRIDES AND EXTRAS OF THE ODBC SQL<object> functions

  bool    MakeInfoTableTriggers(MTriggerMap& p_triggers,CString& p_errors);

  // PURE VIRTUAL INTERFACE

  //////////////////////////////////////////////////////////////////////////
  //
  // GENERALS (Strings & Booleans) 
  //   o GetRDBMS<x>              (boolean,string)
  //   o GetKEYWORD<x>            (string)
  //   o GetSQL<special-process>  (sql-string)
  //
  //////////////////////////////////////////////////////////////////////////

  // Get the database type
  // virtual DatabaseType GetDatabaseType() const = 0;
  virtual DatabaseType GetRDBMSDatabaseType() const = 0;

  // The name of the database vendor
  // virtual CString GetDatabaseVendorName() const = 0;
  virtual CString GetRDBMSVendorName() const = 0;

  // Get the physical database name
  // virtual CString GetPhysicalDatabaseName() const = 0;
  virtual CString GetRDBMSPhysicalDatabaseName() const = 0;

  // System catalog is stored in uppercase in the database?
  // virtual bool IsCatalogUpper() const = 0;
  virtual bool GetRDBMSIsCatalogUpper() const = 0;

  // System catalog supports full ISO schemas (same tables per schema)
  // virtual bool GetUnderstandsSchemas() const = 0;
  virtual bool GetRDBMSUnderstandsSchemas() const = 0;

  // Supports database/ODBCdriver comments in SQL
  // virtual bool SupportsDatabaseComments() const = 0;
  virtual bool GetRDBMSSupportsComments() const = 0;

  // Database can defer constraints until the end of a transaction
  // virtual bool SupportsDeferredConstraints() const = 0;
  virtual bool GetRDBMSSupportsDeferredConstraints() const = 0;

  // Database has ORDER BY with an expression, e.g. ORDER BY UPPER(columnname)
  // Work-around is "SELECT UPPER(columnname) AS something.....ORDER BY something
  // virtual bool SupportsOrderByExpression() const = 0;
  virtual bool GetRDBMSSupportsOrderByExpression() const = 0;

  // Supports the ODBC escape sequence {[?=] CALL procedure (?,?,?)}
  // virtual bool SupportsODBCCallEscapes() const = 0;
  virtual bool GetRDBMSSupportsODBCCallEscapes() const = 0;

  // Catalogs query for the default value of a table's column
  // virtual CString GetSQLStringDefaultValue(CString p_tableName,CString p_columnName) const = 0;
  // -> Goes to "GetCATALOGColumnAttributes"

  // If the database does not support the datatype TIME, it can be implemented as a DECIMAL
  //virtual bool GetTimeIsDecimal() const = 0;
  // BEWARE BOOL INVERTED!!
  virtual bool GetRDBMSSupportsDatatypeTime() const = 0;

  // If the database does not support the datatype INTERVAL, it can be implemented as a DECIMAL
  // virtual bool GetIntervalIsDecimal() const = 0;
  // BEWARE BOOL INVERTED!!
  virtual bool GetRDBMSSupportsDatatypeInterval() const = 0;

  // Gets the maximum length of an SQL statement
  // virtual unsigned long GetMaxStatementLength() const = 0;
  virtual unsigned long GetRDBMSMaxStatementLength() const = 0;

  // KEYWORDS

  // Keyword for the current date and time
  // virtual CString GetSystemDateTimeKeyword() const = 0;
  virtual CString GetKEYWORDCurrentTimestamp() const = 0;

  // String for the current date
  // virtual CString GetSystemDateString() const = 0;
  virtual CString GetKEYWORDCurrentDate() const = 0;

  // Get the concatenation operator
  // virtual CString GetConcatanationOperator() const = 0;
  virtual CString GetKEYWORDConcatanationOperator() const = 0;

  // Get quote character for strings
  // virtual CString GetQuoteCharacter() const = 0;
  virtual CString GetKEYWORDQuoteCharacter() const = 0;

  // Get default NULL for parameter list input
  // virtual CString GetDefaultNULL() const = 0;
  virtual CString GetKEYWORDParameterDefaultNULL() const = 0;

  // Parameter is for INPUT and OUTPUT in parameter list
  // virtual CString GetParameterINOUT() const = 0;
  virtual CString GetKEYWORDParameterINOUT() const = 0;

  // Parameter is for OUTPUT only in parameter list
  // virtual CString GetParameterOUT() const = 0;
  virtual CString GetKEYWORDParameterOUT() const = 0;

  // Get datatype of the IDENTITY primary key in a Network database
  // virtual CString GetPrimaryKeyType() const = 0;
  virtual CString GetKEYWORDNetworkPrimaryKeyType() const = 0;

  // Get datatype for Moment
  // virtual CString GetDatetimeYearToSecondType() const = 0;
  virtual CString GetKEYWORDTypeTimestamp() const = 0;

  // Separator between two alter-constraints in an alter-table statement
  // virtual CString GetAlterConstraintSeparator() const = 0;

  // DROPPED SUPPORT FOR OLD STYLE (+) LEFT OUTER JOIN SUPPORT
  // ALL DATABASES NOW SUPPORT ISO SYTLE INNER/OUTER JOIN
  // Inner Join Keyword
  // virtual CString GetInnerJoinKeyword() const = 0;
  // Outer join keyword
  // virtual CString GetOuterJoinKeyword() const = 0;
  // Inner Join Keyword for use in views.
  // virtual CString GetViewInnerJoinKeyword() const = 0;
  // Outer join keyword for use in views
  // virtual CString GetViewOuterJoinKeyword() const = 0;
  // Get the closure for an outer-join
  // virtual CString GetOuterJoinClosure() const = 0;
  // Get the special Outer Join sign for the while-conditions
  // virtual CString GetOuterJoinSign() const = 0;

  // Prefix for a parameter in a stored procedure
  // virtual CString  GetSPParamPrefix() const = 0;
  virtual CString  GetKEYWORDParameterPrefix() const = 0;

  // Get select part to add new record identity to a table
  // Can be special column like 'OID' or a sequence select
  // virtual CString GetIdentityString(CString& p_tablename,CString p_postfix = "_seq") const = 0;
  virtual CString GetKEYWORDIdentityString(CString& p_tablename,CString p_postfix = "_seq") const = 0;

  // Code to create a temporary table (qualifier)
  // virtual CString GetCodeTemporaryTable() const = 0;

  // Gets the UPPER function
  // virtual CString GetUpperFunction(CString& p_expression) const = 0;
  virtual CString GetKEYWORDUpper(CString& p_expression) const = 0;

  // Gets the construction for 1 minute ago
  // virtual CString GetInterval1MinuteAgo() const = 0;
  virtual CString GetKEYWORDInterval1MinuteAgo() const = 0;

  // Gets the Not-NULL-Value statement of the database
  // virtual CString GetNVLStatement(CString& p_test,CString& p_isnull) const = 0;
  virtual CString GetKEYWORDStatementNVL(CString& p_test,CString& p_isnull) const = 0;
  
  //////////////////////////////////////////////////////////////////////////
  //
  // CATALOG
  // o GetCATALOG<Object[s]><Function>
  //   Objects
  //   - Sequence
  //   - Table
  //   - View
  //   - Column
  //   - Index
  //   - PrimaryKey
  //   - ForeignKey
  //   - Trigger
  //   - TemporaryTable 
  //  Functions per object type
  //   - Exists
  //   - List
  //   - Attributes
  //   - Create
  //   - Alter (where possible)
  //   - Drop
  //
  //////////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////////
  //
  // SESSIONS
  // -Sessions (No create and drop)
  //   - GetSessionMyself
  //   - GetSessionExists
  //   - GetSessionList
  //   - GetSessionAttributes
  //     (was GetSessionAndTerminal)
  //     (was GetSessionUniqueID)
  //
  //////////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////////
  //
  // SQL/PSM PERSISTENT STORED MODULES 
  //         Also called SPL or PL/SQL
  // o GetPSM<Object[s]><Function>
  //   -Procedures / Functions
  //   - Exists					GetPSMProcedureExists
  //   - List					  GetPSMProcedureList
  //   - Create
  //   - Drop
  //   - GetAttributes
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



  //////////////////////////////////////////////////////////////////////////
  //
  // OLD INTERFACE
  //
  //////////////////////////////////////////////////////////////////////////


  // BOOLEANS AND STRINGS

  // Replace the Identity column OID by a sequence.nextval
  virtual CString GetReplaceColumnOIDbySequence(CString p_columns,CString p_tablename,CString p_postfix = "_seq") const = 0;

  // Remove catalog dependencies for stored procedures
  virtual CString GetSQLRemoveProcedureDependencies(CString p_procname) const = 0;

  // Remove field dependencies for calculated fields (bug in Firebird)
  virtual CString GetSQLRemoveFieldDependencies(CString p_tablename) const = 0;

  // Performance parameters to be added to the database
  virtual CString GetSQLPerformanceSettings() const = 0;

  // SQL To set the caching mode of SQL results
  virtual CString GetSQLCacheModeSetting(const CString& p_mode) const = 0;

  // Needed for storing numbers in stored procedures
  virtual CString GetSQLNlsNumericCharacters() const = 0;

  // Code to define a table in row-locking mode
  virtual CString GetCodeLockModeRow() const = 0;

  // Code to create a temporary table with no logging
  virtual CString GetCodeTempTableWithNoLog() const = 0;

  // Granting all rights on a table (In a NON-ANSI database)
  virtual CString GetSQLGrantAllOnTable(CString p_schema,CString p_tableName,bool p_grantOption = false) = 0;

  // Code prefix for a select-into-temp
  virtual CString GetSelectIntoTempClausePrefix(CString p_tableName) const = 0;

  // Code suffix for after a select-into-temp
  virtual CString GetSelectIntoTempClauseSuffix(CString p_tableName) const = 0;

  // Gets the construction / select for generating a new serial identity
  virtual CString GetSQLGenerateSerial(CString p_table) const = 0;

  // Gets the construction / select for the resulting effective generated serial
  virtual CString GetSQLEffectiveSerial(CString p_identity) const = 0;
  
  // Gets the subtransaction commands
  virtual CString GetStartSubTransaction   (CString p_savepointName) const = 0;
  virtual CString GetCommitSubTransaction  (CString p_savepointName) const = 0;
  virtual CString GetRollbackSubTransaction(CString p_savepointName) const = 0;

  // SQL CATALOG QUERIES
  // ===================

  // Get SQL to check if a stored procedure already exists in the database
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
  virtual CString GetSQLTableExists(CString p_schema,CString p_tablename) const = 0;

  // Get SQL to select all columns of a table from the catalog
  virtual CString GetSQLGetColumns(CString& p_user,CString& p_tableName) const = 0;

  // Get SQL to select all constraints on a table from the catalog
  virtual CString GetSQLGetConstraintsForTable(CString& p_tableName) const = 0;

  // Get SQL to read all indices for a table
  virtual CString GetSQLTableIndices(CString p_user,CString p_tableName) const = 0;

  // Get SQL to create an index for a table
  virtual CString GetSQLCreateIndex(CString p_user,CString p_tableName,DBIndex* p_index) const = 0;
  virtual CString GetSQLCreateIndex(MStatisticsMap& p_indices) const = 0;

  // Get extra filter expression for an index column
  virtual CString GetIndexFilter(MetaStatistics& p_index) const = 0;

  // Get SQL to drop an index
  virtual CString GetSQLDropIndex(CString p_user,CString p_indexName) const = 0;

  // Get SQL to read the referential constraints from the catalog
  virtual CString GetSQLTableReferences(CString p_schema,CString p_tablename,CString p_constraint = "",int p_maxColumns = SQLINFO_MAX_COLUMNS) const = 0;

  // Get the SQL to determine the sequence state in the database
  virtual CString GetSQLSequence(CString p_schema,CString p_tablename,CString p_postfix = "_seq") const = 0;

  // Create a sequence in the database
  virtual CString GetSQLCreateSequence(CString p_schema,CString p_tablename,CString p_postfix = "_seq",int p_startpos = 1) const = 0;

  // Remove a sequence from the database
  virtual CString GetSQLDropSequence(CString p_schema,CString p_tablename,CString p_postfix = "_seq") const = 0;

  // Gets the SQL for the rights on the sequence
  virtual CString GetSQLSequenceRights(CString p_schema,CString p_tableName,CString p_postfix = "_seq") const = 0;

  // Remove a stored procedure from the database
  virtual void    DoRemoveProcedure(CString& p_procedureName) const = 0;

  // Get SQL for your session and controlling terminal
  virtual CString GetSQLSessionAndTerminal() const = 0;

  // Get SQL to check if session number exists
  virtual CString GetSQLSessionExists(CString sessieId) const = 0;

  // Get SQL for unique session ID
  virtual CString GetSQLUniqueSessionId(const CString& p_databaseName,const CString& p_sessionTable) const = 0;

  // Get SQL for searching a session
  virtual CString GetSQLSearchSession(const CString& p_databaseName,const CString& p_sessionTable) const = 0;

  // See if a column exists within a table
  virtual bool    DoesColumnExistsInTable(CString& p_owner,CString& p_tableName,CString& p_column) const = 0;

  // Get SQL to get all the information about a Primary Key constraint
  virtual CString GetSQLPrimaryKeyConstraintInformation(CString p_schema,CString p_tableName) const = 0;

  // Does the named constraint exist in the database
  virtual bool    DoesConstraintExist(CString p_constraintName) const = 0;

  // Get SQL to lock  a table 
  virtual CString GetSQLLockTable(CString& p_tableName,bool p_exclusive) const = 0;

  // Get query to optimize the table statistics
  virtual CString GetSQLOptimizeTable(CString& p_owner,CString& p_tableName,int& p_number) = 0;

  // Getting the fact that there is only **one** (1) user session in the database
  virtual bool    GetOnlyOneUserSession() = 0;

  // Gets the triggers for a table
  virtual CString GetSQLTriggers(CString m_schema,CString p_table) const = 0;

  // NEW INTERFACE
  // GetCATALOGColumnAlter

  // Gives the statement to alter a table columns' name
  virtual CString GetSQLModifyColumnName(CString p_tablename,CString p_oldName,CString p_newName,CString p_datatype) = 0;

  // Get SQL keyword to alter a column in a table
  virtual CString GetCodeAlterColumn() const = 0;

  // Gets the prefix needed for altering the datatype of a column in a MODIFY/ALTER
  virtual CString GetModifyDatatypePrefix() const = 0;



  // New Interface PrimaryKey / ForeignKey
  // GetCATALOGPrimaryKeyList
  // GetCATALOGPrimaryKeyCreate
  // GetCATALOGForeignKeyCreate
  // GetCATALOGForeignKeyAlter

  // Gets the table definition-form of a primary key
  virtual CString GetPrimaryKeyDefinition(CString p_schema,CString p_tableName,bool p_temporary) const = 0;

  // Get the constraint form of a primary key to be added to a table after creation of that table
  virtual CString GetPrimaryKeyConstraint(CString p_schema,CString p_tablename,CString p_primary) const = 0;
  virtual CString GetPrimaryKeyConstraint(MPrimaryMap& p_primaries) const = 0;

  // Get the sql to add a foreign key to a table
  virtual CString GetSQLForeignKeyConstraint(DBForeign&   p_foreign) const = 0;
  virtual CString GetSQLForeignKeyConstraint(MForeignMap& p_foreign) const = 0;

  // Get the sql (if possible) to change the foreign key constraint
  virtual CString GetSQLAlterForeignKey(DBForeign& p_origin,DBForeign& p_requested) const = 0;


  // New interface: Obect = TempTable
  // GetCATALOGTempTableCreate
  // GetCATALOGTempTableDrop
  // GetCATALOGTempTableInsert

  // Get a query to create a temporary table from a select statement
  virtual CString GetSQLCreateTemporaryTable(CString& p_tablename,CString p_select) const = 0;

  // Get the query to remove a temporary table indefinetly
  virtual CString GetSQLRemoveTemporaryTable(CString& p_tablename,int& p_number) const = 0;

  // Get a query to select into a temp table
  virtual CString GetSQLSelectIntoTemp(CString& p_tablename,CString& p_select) const = 0;


  // SQL DDL STATEMENTS
  // ==================

  // Add a column to a table
  virtual CString GetCreateColumn(CString p_schema,CString p_tablename,CString p_columnName,CString p_typeDefinition,bool p_notNull) = 0;

  // Drop a column from a table
  virtual CString GetSQLDropColumn(CString p_schema,CString p_tablename,CString p_columnName) const = 0;

  // Add a foreign key to a table
  virtual CString GetCreateForeignKey(CString p_tablename,CString p_constraintname,CString p_column,CString p_refTable,CString p_primary) = 0;

  // Modify a column's definition in a table
  virtual CString GetModifyColumnType(CString p_schema,CString p_tablename,CString p_columnName,CString p_typeDefinition) = 0;
  virtual CString GetModifyColumnNull(CString p_schema,CString p_tablename,CString p_columnName,bool p_notNull) = 0;

  // Get the SQL to drop a view. If precursor is filled: run that SQL first!
  virtual CString GetSQLDropView(CString p_schema,CString p_view,CString& p_precursor) = 0;

  // Create or replace a database view
  virtual CString GetSQLCreateOrReplaceView(CString p_schema,CString p_view,CString p_asSelect) const = 0;

  // Create or replace a trigger
  virtual CString CreateOrReplaceTrigger(MetaTrigger& p_trigger) const = 0;

  // SQL DDL OPERATIONS
  // ==================

  // Do the commit for the DDL commands in the catalog
  virtual void    DoCommitDDLcommands() const = 0;

  // Do the commit for the DML commands in the database
  virtual void    DoCommitDMLcommands() const = 0;

  // Does the named view exists in the database
  virtual bool    DoesViewExists(CString& p_viewName) = 0;

  // Must create temoporary tables runtime 
  virtual bool    GetMustMakeTemptablesAtRuntime() const = 0;

  // Create a temporary table in an optimized manner with the given index column
  virtual void    DoMakeTemporaryTable(CString& p_tableName,CString& p_content,CString& p_indexColumn) const = 0;

  // Remove a temporary table
  virtual void    DoRemoveTemporaryTable(CString& p_tableName) const = 0;

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

  // Gets the fact if an IF statement needs to be bordered with BEGIN/END
  virtual bool    GetCodeIfStatementBeginEnd() const = 0;

  // Gets the end of an IF statement
  virtual CString GetCodeEndIfStatement() const = 0;

  // Gets a complete assignment statement.
  virtual CString GetAssignmentStatement(const CString& p_destiny,const CString& p_source) const = 0;

  // Get the code to start a WHILE-loop
  virtual CString GetStartWhileLoop(CString p_condition) const = 0;

  // Get the code to end a WHILE-loop
  virtual CString GetEndWhileLoop() const = 0;

  // Gets the fact if a SELECT must be in between parenthesis for an assignment
  virtual bool    GetAssignmentSelectParenthesis() const = 0;

  // Get the start of a SPL While loop
  virtual CString GetSPLStartWhileLoop(CString p_condition) const = 0;

  // Get the end of a SPL while loop
  virtual CString GetSPLEndWhileLoop() const = 0;

  // Get stored procedure call
  virtual CString GetSQLSPLCall(CString p_procName) const = 0;

  // Build a parameter list for calling a stored procedure
  virtual CString GetBuildedParameterList(size_t p_numOfParameters) const = 0;

  // Parameter type for stored procedure for a given column type for parameters and return types
  virtual CString GetParameterType(CString &p_type) const = 0;

  // Makes a SQL string from a given string, with all the right quotes
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

  // Get the SPL source code for a stored procedure as registered in the database
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

  // Calling a stored function or procedure if the RDBMS does not support ODBC call escapes
  virtual SQLVariant* DoSQLCall(SQLQuery* p_query,CString& p_schema,CString& p_procedure) = 0;
  
  // SPECIALS
  // ==========================================================================

  // Translate database-errors to a human readable form
  virtual CString TranslateErrortext(int p_error,CString p_errorText) const = 0;

private:
  CString m_grantedUsers;
};

inline void    
SQLInfoDB::SetGrantedUsers(CString p_users) 
{
  m_grantedUsers = p_users;
}

inline CString 
SQLInfoDB::GetGrantedUsers() const
{
  return m_grantedUsers;
}

// End of namespace
}
