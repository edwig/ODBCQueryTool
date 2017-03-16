////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoInformix.h
//
// Copyright (c) 1992- 2014 ir. W.E. Huisman
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
#include "SQLInfoDB.h"

namespace SQLComponents
{

// IBM-Informix IDS has currently this number of columns for indices
// Older versions had 8 columns per index
#define INFORMIX_IDS_MAX_INDEX_COLUMNS 16

class SQLInfoInformix : public SQLInfoDB
{
public:
  SQLInfoInformix(SQLDatabase* p_database);
 ~SQLInfoInformix();

  //////////////////////////////////////////////////////////////////////////
  //
  // GENERALS (Strings & Booleans) 
  //
  //////////////////////////////////////////////////////////////////////////

  // Get the database type
  DatabaseType GetRDBMSDatabaseType() const;

  // The name of the database vendor
  CString GetRDBMSVendorName() const;

  // Get the physical database name
  CString GetRDBMSPhysicalDatabaseName() const;

  // System catalog is stored in uppercase in the database?
  bool    GetRDBMSIsCatalogUpper() const;

  // System catalog supports full ISO schemas (same tables per schema)
  bool    GetRDBMSUnderstandsSchemas() const;

  // Supports database/ODBCdriver comments in SQL
  bool    GetRDBMSSupportsComments() const;

  // Database can defer constraints until the end of a transaction
  bool    GetRDBMSSupportsDeferredConstraints() const;

  // Database has ORDER BY with an expression, e.g. ORDER BY UPPER(columnname)
  bool    GetRDBMSSupportsOrderByExpression() const;

  // Supports the ODBC escape sequence {[?=] CALL procedure (?,?,?)}
  bool    GetRDBMSSupportsODBCCallEscapes() const;

  // If the database does not support the datatype TIME, it can be implemented as a DECIMAL
  bool    GetRDBMSSupportsDatatypeTime() const;

  // If the database does not support the datatype INTERVAL, it can be implemented as a DECIMAL
  bool    GetRDBMSSupportsDatatypeInterval() const;

  // Gets the maximum length of an SQL statement
  unsigned long GetRDBMSMaxStatementLength() const;

  // KEYWORDS

  // Keyword for the current date and time
  CString GetKEYWORDCurrentTimestamp() const;

  // String for the current date
  CString GetKEYWORDCurrentDate() const;

  // Get the concatenation operator
  CString GetKEYWORDConcatanationOperator() const;

  // Get quote character for strings
  CString GetKEYWORDQuoteCharacter() const;

  // Get default NULL for parameter list input
  CString GetKEYWORDParameterDefaultNULL() const;

  // Parameter is for INPUT and OUTPUT in parameter list
  CString GetKEYWORDParameterINOUT() const;

  // Parameter is for OUTPUT only in parameter list
  CString GetKEYWORDParameterOUT() const;

  // Get datatype of the IDENTITY primary key in a Network database
  CString GetKEYWORDNetworkPrimaryKeyType() const;

  // Get datatype for timestamp (year to second)
  CString GetKEYWORDTypeTimestamp() const;

  // Prefix for a parameter in a stored procedure
  CString GetKEYWORDParameterPrefix() const;

  // Get select part to add new record identity to a table
  // Can be special column like 'OID' or a sequence select
  CString GetKEYWORDIdentityString(CString& p_tablename,CString p_postfix = "_seq") const;

  // Gets the UPPER function
  CString GetKEYWORDUpper(CString& p_expression) const;

  // Gets the construction for 1 minute ago
  CString GetKEYWORDInterval1MinuteAgo() const;

  // Gets the Not-NULL-Value statement of the database
  CString GetKEYWORDStatementNVL(CString& p_test,CString& p_isnull) const;

  // SQL

  // Code prefix for a select-into-temp
  CString GetSQLSelectIntoTempPrefix(CString p_tableName) const;

  // Code suffix for after a select-into-temp
  CString GetSQLSelectIntoTempSuffix(CString p_tableName) const;

  // Gets the construction / select for generating a new serial identity
  CString GetSQLGenerateSerial(CString p_table) const;

  // Gets the construction / select for the resulting effective generated serial
  CString GetSQLEffectiveSerial(CString p_identity) const;

  // Gets the subtransaction commands
  CString GetSQLStartSubTransaction   (CString p_savepointName) const;
  CString GetSQLCommitSubTransaction  (CString p_savepointName) const;
  CString GetSQLRollbackSubTransaction(CString p_savepointName) const;

  // FROM-Part for a query to select only 1 (one) record / or empty!
  CString GetSQLFromDualClause() const;

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
  //   - Alter (where possible)
  //   - Rename (where possible)
  //   - Drop
  //
  //////////////////////////////////////////////////////////////////////////

  // All table functions
  CString GetCATALOGTableExists    (CString p_schema,CString p_tablename) const;
  CString GetCATALOGTablesList     (CString p_schema,CString p_pattern) const;
  bool    GetCATALOGTableAttributes(CString p_schema,CString p_tablename,MetaTable& p_table) const;
  CString GetCATALOGTableCreate    (MetaTable& p_table,MetaColumn& p_column) const;
  CString GetCATALOGTableRename    (CString p_schema,CString p_tablename,CString p_newname) const;
  CString GetCATALOGTableDrop      (CString p_schema,CString p_tablename) const;
  // All column functions
  CString GetCATALOGColumnExists    (CString p_schema,CString p_tablename,CString p_columname) const;
  CString GetCATALOGColumnList      (CString p_schema,CString p_tablename) const;
  CString GetCATALOGColumnAttributes(CString p_schema,CString p_tablename,CString p_columname) const;
  CString GetCATALOGColumnCreate    (MetaColumn& p_column) const;
  CString GetCATALOGColumnAlter     (MetaColumn& p_column) const;
  CString GetCATALOGColumnRename    (CString p_schema,CString p_tablename,CString p_columnname,CString p_newname,CString p_datatype) const;
  CString GetCATALOGColumnDrop      (CString p_schema,CString p_tablename,CString p_columname) const;
  // All index functions
  CString GetCATALOGIndexExists     (CString p_schema,CString p_tablename,CString p_indexname) const;
  CString GetCATALOGIndexList       (CString p_schema,CString p_tablename) const;
  CString GetCATALOGIndexAttributes (CString p_schema,CString p_tablename,CString p_indexname) const;
  CString GetCATALOGIndexCreate     (MIndicesMap& p_index) const;
  CString GetCATALOGIndexDrop       (CString p_schema,CString p_tablename,CString p_indexname) const;
  CString GetCATALOGIndexFilter     (MetaIndex& p_index) const;
  // All primary key functions
  CString GetCATALOGPrimaryExists    (CString p_schema,CString p_tablename) const;
  CString GetCATALOGPrimaryAttributes(CString p_schema,CString p_tablename) const;
  CString GetCATALOGPrimaryCreate    (MPrimaryMap& p_primary) const;
  CString GetCATALOGPrimaryDrop      (CString p_schema,CString p_tablename,CString p_constraintname) const;
  // All foreign key functions
  CString GetCATALOGForeignExists    (CString p_schema,CString p_tablename,CString p_constraintname) const;
  CString GetCATALOGForeignList      (CString p_schema,CString p_tablename,int p_maxColumns = SQLINFO_MAX_COLUMNS) const;
  CString GetCATALOGForeignAttributes(CString p_schema,CString p_tablename,CString p_constraintname,int p_maxColumns = SQLINFO_MAX_COLUMNS) const;
  CString GetCATALOGForeignCreate    (MForeignMap& p_foreigns) const;
  CString GetCATALOGForeignAlter     (MForeignMap& p_original,MForeignMap& p_requested) const;
  CString GetCATALOGForeignDrop      (CString p_schema,CString p_tablename,CString p_constraintname) const;
  // All trigger functions
  CString GetCATALOGTriggerExists    (CString p_schema,CString p_tablename,CString p_triggername) const;
  CString GetCATALOGTriggerList      (CString p_schema,CString p_tablename) const;
  CString GetCATALOGTriggerAttributes(CString p_schema,CString p_tablename,CString p_triggername) const;
  CString GetCATALOGTriggerCreate    (MetaTrigger& p_trigger) const;
  CString GetCATALOGTriggerDrop      (CString p_schema,CString p_tablename,CString p_triggername) const;
  // All sequence functions
  CString GetCATALOGSequenceExists    (CString p_schema,CString p_sequence) const;
  CString GetCATALOGSequenceAttributes(CString p_schema,CString p_sequence) const;
  CString GetCATALOGSequenceCreate    (MetaSequence& p_sequence) const;
  CString GetCATALOGSequenceDrop      (CString p_schema,CString p_sequence) const;

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

  CString GetPSMProcedureExists    (CString p_schema,CString p_procedure) const;
  CString GetPSMProcedureList      (CString p_schema) const;
  CString GetPSMProcedureAttributes(CString p_schema,CString p_procedure) const;
  CString GetPSMProcedureCreate    (MetaProcedure& p_procedure) const;
  CString GetPSMProcedureDrop      (CString p_schema,CString p_procedure) const;

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

  CString GetSESSIONConstraintsDeferred()  const;
  CString GetSESSIONConstraintsImmediate() const;


  //////////////////////////////////////////////////////////////////////////
  //
  // OLD INTERFACE
  //
  //////////////////////////////////////////////////////////////////////////

  // BOOLEANS EN STRINGS
  // ===================

  // Get a query to create a temporary table from a select statement
  CString GetSQLCreateTemporaryTable(CString& p_tablename,CString p_select) const;

  // Get the query to remove a temporary table indefinetly
  CString GetSQLRemoveTemporaryTable(CString& p_tablename,int& p_number) const;

  // Get a query to select into a temp table
  CString GetSQLSelectIntoTemp(CString& p_tablename,CString& p_select) const;

  // Gets the fact if an IF statement needs to be bordered with BEGIN/END
  bool    GetCodeIfStatementBeginEnd() const;

  // Gets the end of an IF statement
  CString GetCodeEndIfStatement() const;

  // Gets a complete assignment statement.
  CString GetAssignmentStatement(const CString& p_destiny,const CString& p_source) const;

  // Get the code to start a WHILE-loop
  CString GetStartWhileLoop(CString p_condition) const;

  // Get the code to end a WHILE-loop
  CString GetEndWhileLoop() const;

  // Gets the fact if a SELECT must be in between parenthesis for an assignment
  bool    GetAssignmentSelectParenthesis() const;

  // SQL CATALOG QUERIES
  // ===================


  // Get SQL for your session and controlling terminal
  CString GetSQLSessionAndTerminal() const;

  // Get SQL to check if session number exists
  CString GetSQLSessionExists(CString sessieId) const;

  // Get SQL for unique session ID
  CString GetSQLUniqueSessionId(const CString& p_databaseName,const CString& p_sessionTable) const;

  // Get SQL for searching a session
  CString GetSQLSearchSession(const CString& p_databaseName,const CString& p_sessionTable) const;

  // Get SQL to lock a table
  CString GetSQLLockTable(CString& p_tableName,bool p_exclusive) const;

  // Get query to optimize the table statistics
  CString GetSQLOptimizeTable(CString& p_owner,CString& p_tableName,int& p_number);

  // Getting the fact that there is only **one** (1) user session in the database
  bool    GetOnlyOneUserSession();

  // SQL DDL STATEMENTS
  // ==================

  // Get the SQL to drop a view. If precursor is filled: run that SQL first!
  CString GetSQLDropView(CString p_schema,CString p_view,CString& p_precursor);

  // Create or replace a database view
  CString GetSQLCreateOrReplaceView(CString p_schema,CString p_view,CString p_asSelect) const;

  // SQL DDL OPERATIONS
  // ==================

  // Do the commit for the DDL commands in the catalog
  void    DoCommitDDLcommands() const;

  // Do the commit for the DML commands in the database
  void    DoCommitDMLcommands() const;

  // Does the named view exists in the database
  bool    DoesViewExists(CString& p_viewName);

  // Must create temporary tables runtime 
  bool    GetMustMakeTemptablesAtRuntime() const;

  // Create a temporary table in an optimized manner with the given index column
  void    DoMakeTemporaryTable(CString& p_tableName,CString& p_content,CString& p_indexColumn) const;

  // Remove a temporary table
  void    DoRemoveTemporaryTable(CString& p_tableName) const;

  // PERSISTENT-STORED MODULES (SPL / PL/SQL)
  // ====================================================================

  // Get the user error text from the database
  CString GetUserErrorText(CString& p_procName) const;

  // Get assignment to a variable in SPL
  CString GetSPLAssignment(CString p_variable) const;

  // Get the start of a SPL While loop
  CString GetSPLStartWhileLoop(CString p_condition) const;

  // Get the end of a SPL while loop
  CString GetSPLEndWhileLoop() const;

  // Get stored procedure call
  CString GetSQLSPLCall(CString p_procName) const;

  // Build a parameter list for calling a stored procedure
  CString GetBuildedParameterList(size_t p_numOfParameters) const;

  // Parameter type for stored procedure for a given column type for parameters and return types
  CString GetParameterType(CString &p_type) const;

  // Makes a SQL string from a given string, with all the right quotes
  CString GetSQLString(const CString& p_string) const;

  // Get date string in engine format
  CString GetSQLDateString(int p_year,int p_month,int p_day) const;

  // Get time string in database engine format
  CString GetSQLTimeString(int p_hour,int p_minute,int p_second) const;

  // Get date-time string in database engine format
  CString GetSQLDateTimeString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const;

  // Get date-time bound parameter string in database format
  CString GetSQLDateTimeBoundString() const;

  // Stripped data for the parameter binding
  CString GetSQLDateTimeStrippedString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const;

  // Get the SPL sourcecode for a stored procedure as registered in the database
  CString GetSPLSourcecodeFromDatabase(const CString& p_owner,const CString& p_procName) const;

  // Get the SPL datatype for integer
  CString GetSPLIntegerType() const;

  // Get the SPL datatype for a decimal
  CString GetSPLDecimalType() const;

  // Get the SPL declaration for a cursor
  CString GetSPLCursorDeclaratie(CString& p_variableName,CString& p_query) const;

  // Get the SPL cursor found row variable
  CString GetSPLCursorFound(CString& p_cursorName) const;

  // Get the SPL cursor row-count variable
  CString GetSPLCursorRowCount(CString& p_variable) const;

  // Get the SPL datatype for a declaration of a row-variable
  CString GetSPLCursorRowDeclaration(CString& p_cursorName,CString& p_variableName) const;

  CString GetSPLFetchCursorIntoVariables(CString                p_cursorName
                                        ,CString                p_variableName
                                        ,std::vector<CString>&  p_columnNames
                                        ,std::vector<CString>&  p_variableNames) const;

  // Fetch the current SPL cursor row into the row variable
  CString GetSPLFetchCursorIntoRowVariable(CString& p_cursorName,CString p_variableName) const;

  // Get the SPL no-data exception clause
  CString GetSPLNoDataFoundExceptionClause() const;

  // Get the SPL form of raising an exception
  CString GetSPLRaiseException(CString p_exceptionName) const;

  // Get the fact that the SPL has server functions that return more than 1 value
  bool    GetSPLServerFunctionsWithReturnValues() const;

  // Calling a stored function or procedure if the RDBMS does not support ODBC call escapes
  SQLVariant* DoSQLCall(SQLQuery* p_query,CString& p_schema,CString& p_procedure);

  // SPECIALS
  // ==========================================================================

  // Translate database-errors to a human readable form
  CString TranslateErrortext(int p_error,CString p_errorText) const;
};

// End of namespace
}