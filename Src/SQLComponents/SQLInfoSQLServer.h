////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoSQLServer.h
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

class SQLInfoSQLServer : public SQLInfoDB
{
public:
  SQLInfoSQLServer(SQLDatabase* p_database);
 ~SQLInfoSQLServer();

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

  // Database must commit DDL commands in a transaction
  bool GetRDBMSMustCommitDDL() const;

  //////////////////////////////////////////////////////////////////////////
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

  //////////////////////////////////////////////////////////////////////////
  // SQL

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

  // Get SQL to lock  a table 
  CString GetSQLLockTable(CString p_schema, CString p_tablename, bool p_exclusive) const;

  // Get query to optimize the table statistics
  CString GetSQLOptimizeTable(CString p_schema, CString p_tablename) const;

  //////////////////////////////////////////////////////////////////////////
  // SQL STRINGS

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
  CString GetCATALOGTableExists       (CString p_schema,CString p_tablename) const;
  CString GetCATALOGTablesList        (CString p_schema,CString p_pattern) const;
  bool    GetCATALOGTableAttributes   (CString p_schema,CString p_tablename,MetaTable& p_table) const;
  CString GetCATALOGTableCreate       (MetaTable& p_table,MetaColumn& p_column) const;
  CString GetCATALOGTableRename       (CString p_schema,CString p_tablename,CString p_newname) const;
  CString GetCATALOGTableDrop         (CString p_schema,CString p_tablename) const;
  // All Temporary table functions
  CString GetCATALOGTemptableCreate   (CString p_schema,CString p_tablename,CString p_select) const;
  CString GetCATALOGTemptableIntoTemp (CString p_schema,CString p_tablename,CString p_select) const;
  CString GetCATALOGTemptableDrop     (CString p_schema,CString p_tablename) const;
  // All column functions
  CString GetCATALOGColumnExists      (CString p_schema,CString p_tablename,CString p_columnname) const;
  CString GetCATALOGColumnList        (CString p_schema,CString p_tablename) const;
  CString GetCATALOGColumnAttributes  (CString p_schema,CString p_tablename,CString p_columnname) const;
  CString GetCATALOGColumnCreate      (MetaColumn& p_column) const;
  CString GetCATALOGColumnAlter       (MetaColumn& p_column) const;
  CString GetCATALOGColumnRename      (CString p_schema,CString p_tablename,CString p_columnname,CString p_newname,CString p_datatype) const;
  CString GetCATALOGColumnDrop        (CString p_schema,CString p_tablename,CString p_columnname) const;
  // All index functions
  CString GetCATALOGIndexExists       (CString p_schema,CString p_tablename,CString p_indexname) const;
  CString GetCATALOGIndexList         (CString p_schema,CString p_tablename) const;
  CString GetCATALOGIndexAttributes   (CString p_schema,CString p_tablename,CString p_indexname) const;
  CString GetCATALOGIndexCreate       (MIndicesMap& p_index) const;
  CString GetCATALOGIndexDrop         (CString p_schema,CString p_tablename,CString p_indexname) const;
  CString GetCATALOGIndexFilter       (MetaIndex& p_index) const;
  // All primary key functions
  CString GetCATALOGPrimaryExists     (CString p_schema,CString p_tablename) const;
  CString GetCATALOGPrimaryAttributes (CString p_schema,CString p_tablename) const;
  CString GetCATALOGPrimaryCreate     (MPrimaryMap& p_primaries) const;
  CString GetCATALOGPrimaryDrop       (CString p_schema,CString p_tablename,CString p_constraintname) const;
  // All foreign key functions
  CString GetCATALOGForeignExists     (CString p_schema,CString p_tablename,CString p_constraintname) const;
  CString GetCATALOGForeignList       (CString p_schema,CString p_tablename,int p_maxColumns = SQLINFO_MAX_COLUMNS) const;
  CString GetCATALOGForeignAttributes (CString p_schema,CString p_tablename,CString p_constraintname,int p_maxColumns = SQLINFO_MAX_COLUMNS) const;
  CString GetCATALOGForeignCreate     (MForeignMap& p_foreigns) const;
  CString GetCATALOGForeignAlter      (MForeignMap& p_original,MForeignMap& p_requested) const;
  CString GetCATALOGForeignDrop       (CString p_schema,CString p_tablename,CString p_constraintname) const;
  // All trigger functions
  CString GetCATALOGTriggerExists     (CString p_schema,CString p_tablename,CString p_triggername) const;
  CString GetCATALOGTriggerList       (CString p_schema,CString p_tablename) const;
  CString GetCATALOGTriggerAttributes (CString p_schema,CString p_tablename,CString p_triggername) const;
  CString GetCATALOGTriggerCreate     (MetaTrigger& p_trigger) const;
  CString GetCATALOGTriggerDrop       (CString p_schema,CString p_tablename,CString p_triggername) const;
  // All sequence functions
  CString GetCATALOGSequenceExists    (CString p_schema,CString p_sequence) const;
  CString GetCATALOGSequenceAttributes(CString p_schema,CString p_sequence) const;
  CString GetCATALOGSequenceCreate    (MetaSequence& p_sequence) const;
  CString GetCATALOGSequenceDrop      (CString p_schema,CString p_sequence) const;
  // All view functions
  CString GetCATALOGViewExists        (CString p_schema,CString p_viewname) const;
  CString GetCATALOGViewList          (CString p_schema,CString p_pattern)  const;
  CString GetCATALOGViewAttributes    (CString p_schema,CString p_viewname) const;
  CString GetCATALOGViewCreate        (CString p_schema,CString p_viewname,CString p_contents)   const;
  CString GetCATALOGViewRename        (CString p_schema,CString p_viewname,CString p_newname)    const;
  CString GetCATALOGViewDrop          (CString p_schema,CString p_viewname,CString& p_precursor) const;

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

  // All procedure functions
  CString GetPSMProcedureExists    (CString p_schema,CString p_procedure) const;
  CString GetPSMProcedureList      (CString p_schema) const;
  CString GetPSMProcedureAttributes(CString p_schema,CString p_procedure) const;
  CString GetPSMProcedureCreate    (MetaProcedure& p_procedure) const;
  CString GetPSMProcedureDrop      (CString p_schema,CString p_procedure) const;
  CString GetPSMProcedureErrors    (CString p_schema,CString p_procedure) const;

  // All Language elements
  CString GetPSMDeclaration(bool p_first,CString p_variable,int p_datatype,int p_precision = 0,int p_scale = 0,
                            CString p_default = "",CString p_domain = "",CString p_asColumn = "") const;
  CString GetPSMAssignment (CString p_variable,CString p_statement = "") const;
  CString GetPSMIF         (CString p_condition) const;
  CString GetPSMIFElse     () const;
  CString GetPSMIFEnd      () const;
  CString GetPSMWhile      (CString p_condition) const;
  CString GetPSMWhileEnd   () const;
  CString GetPSMLOOP       () const;
  CString GetPSMLOOPEnd    () const;
  CString GetPSMBREAK      () const;
  CString GetPSMRETURN     (CString p_statement = "") const;
  CString GetPSMExecute    (CString p_procedure,MParameterMap& p_parameters) const;

  // The CURSOR
  CString GetPSMCursorDeclaration(CString p_cursorname,CString p_select) const;
  CString GetPSMCursorFetch      (CString p_cursorname,std::vector<CString>& p_columnnames,std::vector<CString>& p_variablenames) const;

  // PSM Exceptions
  CString GetPSMExceptionCatchNoData() const;
  CString GetPSMExceptionCatch(CString p_sqlState) const;
  CString GetPSMExceptionRaise(CString p_sqlState) const;

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

  CString GetSESSIONMyself() const;
  CString GetSESSIONExists(CString p_sessionID) const;
  CString GetSESSIONList() const;
  CString GetSESSIONAttributes(CString p_sessionID) const;
  // Transactions
  CString GetSESSIONConstraintsDeferred()  const;
  CString GetSESSIONConstraintsImmediate() const;

  //////////////////////////////////////////////////////////////////////////
  //
  // Call FUNCTION/PROCEDURE from within program
  // As a RDBMS dependent extension of "DoSQLCall" of the SQLQuery object
  //
  //////////////////////////////////////////////////////////////////////////

  // Calling a stored function or procedure if the RDBMS does not support ODBC call escapes
  SQLVariant* DoSQLCall(SQLQuery* p_query,CString& p_schema,CString& p_procedure);
};

// End of namespace
}
