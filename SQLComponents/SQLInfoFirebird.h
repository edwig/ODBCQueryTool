////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoFirebird.h
//
// Copyright (c) 1998-2022 ir. W.E. Huisman
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
// Version number: See SQLComponents.h
//
#pragma once
#include "SQLInfoDB.h"

namespace SQLComponents
{

class SQLInfoFirebird : public SQLInfoDB
{
public:
  SQLInfoFirebird(SQLDatabase* p_database);
 ~SQLInfoFirebird();

  //////////////////////////////////////////////////////////////////////////
  //
  // GENERALS (Strings & Booleans) 
  //
  //////////////////////////////////////////////////////////////////////////

  // Get the database type
  DatabaseType GetRDBMSDatabaseType() const;

  // The name of the database vendor
  XString GetRDBMSVendorName() const;

  // Get the physical database name
  XString GetRDBMSPhysicalDatabaseName() const;

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

  // Supports functions at the place of table columns in create/alter index statement
  bool    GetRDBMSSupportsFunctionalIndexes() const;

  // Gets the maximum length of an SQL statement
  unsigned long GetRDBMSMaxStatementLength() const;

  // Database must commit DDL commands in a transaction
  bool GetRDBMSMustCommitDDL() const;

  // Correct maximum precision,scale for a NUMERIC datatype
  void GetRDBMSNumericPrecisionScale(SQLULEN& p_precision, SQLSMALLINT& p_scale) const;

  // Maximum for a VARCHAR to be handled without AT-EXEC data. Assume NVARCHAR is half that size!
  int GetRDBMSMaxVarchar() const;

  // KEYWORDS

  // Keyword for the current date and time
  XString GetKEYWORDCurrentTimestamp() const;

  // String for the current date
  XString GetKEYWORDCurrentDate() const;

  // Get the concatenation operator
  XString GetKEYWORDConcatanationOperator() const;

  // Get quote character for strings
  XString GetKEYWORDQuoteCharacter() const;

  // Get quote character around reserved words as an identifier
  XString GetKEYWORDReservedWordQuote() const;

  // Get default NULL for parameter list input
  XString GetKEYWORDParameterDefaultNULL() const;

  // Parameter is for INPUT and OUTPUT in parameter list
  XString GetKEYWORDParameterINOUT() const;

  // Parameter is for OUTPUT only in parameter list
  XString GetKEYWORDParameterOUT() const;

  // Get datatype of the IDENTITY primary key in a Network database
  XString GetKEYWORDNetworkPrimaryKeyType() const;

  // Get datatype for timestamp (year to second)
  XString GetKEYWORDTypeTimestamp() const;

  // Prefix for a parameter in a stored procedure
  XString GetKEYWORDParameterPrefix() const;

  // Get select part to add new record identity to a table
  // Can be special column like 'OID' or a sequence select
  XString GetKEYWORDIdentityString(XString& p_tablename,XString p_postfix = "_seq") const;

  // Gets the UPPER function
  XString GetKEYWORDUpper(XString& p_expression) const;

  // Gets the construction for 1 minute ago
  XString GetKEYWORDInterval1MinuteAgo() const;

  // Gets the Not-NULL-Value statement of the database
  XString GetKEYWORDStatementNVL(XString& p_test,XString& p_isnull) const;

  // Gets the RDBMS definition of the datatype
  XString GetKEYWORDDataType(MetaColumn* p_column);

  // Gets the USER (current-user) keyword function
  XString GetKEYWORDCurrentUser() const;

  // SQL

  // Connects to a default schema in the database/instance
  XString GetSQLDefaultSchema(XString p_user,XString p_schema) const;

  // Gets the construction for inline generating a key within an INSERT statement
  XString GetSQLNewSerial(XString p_table, XString p_sequence) const;

  // Gets the construction / select for generating a new serial identity
  XString GetSQLGenerateSerial(XString p_table) const;

  // Gets the construction / select for the resulting effective generated serial
  XString GetSQLEffectiveSerial(XString p_identity) const;

  // Gets the subtransaction commands
  XString GetSQLStartSubTransaction   (XString p_savepointName) const;
  XString GetSQLCommitSubTransaction  (XString p_savepointName) const;
  XString GetSQLRollbackSubTransaction(XString p_savepointName) const;

  // FROM-Part for a query to select only 1 (one) record / or empty!
  XString GetSQLFromDualClause() const;

  // Get SQL to lock  a table 
  XString GetSQLLockTable(XString p_schema, XString p_tablename,bool p_exclusive,int p_waittime) const;

  // Get query to optimize the table statistics
  XString GetSQLOptimizeTable(XString p_schema, XString p_tablename) const;

  // Transform query to select top <n> rows
  XString GetSQLTopNRows(XString p_sql,int p_top,int p_skip = 0) const;

  // Query to perform a keep alive ping
  XString GetPing() const;

  //////////////////////////////////////////////////////////////////////////
  // SQL STRINGS

  // Makes a SQL string from a given string, with all the right quotes
  XString GetSQLString(const XString& p_string) const;

  // Get date string in engine format
  XString GetSQLDateString(int p_year,int p_month,int p_day) const;

  // Get time string in database engine format
  XString GetSQLTimeString(int p_hour,int p_minute,int p_second) const;

  // Get date-time string in database engine format
  XString GetSQLDateTimeString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const;

  // Get date-time bound parameter string in database format
  XString GetSQLDateTimeBoundString() const;

  // Stripped data for the parameter binding
  XString GetSQLDateTimeStrippedString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const;

  // Makes an catalog identifier string (possibly quoted on both sides)
  virtual XString GetSQLDDLIdentifier(XString p_identifier) const override;

  // Changes to parameters before binding to an ODBC HSTMT handle
  void DoBindParameterFixup(SQLSMALLINT& p_sqlDatatype,SQLULEN& p_columnSize,SQLSMALLINT& p_scale,SQLLEN& p_bufferSize,SQLLEN* p_indicator) const;

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

  // Meta info about meta types
  XString GetCATALOGMetaTypes(int p_type) const;
  // All table functions
  XString GetCATALOGTableExists       (XString& p_schema,XString& p_tablename)  const;
  XString GetCATALOGTablesList        (XString& p_schema,XString& p_pattern)    const;
  XString GetCATALOGTableAttributes   (XString& p_schema,XString& p_tablename)  const;
  XString GetCATALOGTableSynonyms     (XString& p_schema,XString& p_tablename)  const;
  XString GetCATALOGTableCatalog      (XString& p_schema,XString& p_tablename)  const;
  XString GetCATALOGTableCreate       (MetaTable& p_table,MetaColumn& p_column) const;
  XString GetCATALOGTableCreatePostfix(MetaTable& p_table,MetaColumn& p_column) const;
  XString GetCATALOGTableRename       (XString  p_schema,XString  p_tablename,XString p_newname) const;
  XString GetCATALOGTableDrop         (XString  p_schema,XString  p_tablename,bool p_ifExist = false,bool p_restrict = false,bool p_cascade = false) const;
  // All Temporary table functions
  XString GetCATALOGTemptableCreate   (XString p_schema,XString p_tablename,XString p_select) const;
  XString GetCATALOGTemptableIntoTemp (XString p_schema,XString p_tablename,XString p_select) const;
  XString GetCATALOGTemptableDrop     (XString p_schema,XString p_tablename) const;
  // All column functions
  XString GetCATALOGColumnExists    (XString  p_schema,XString  p_tablename,XString  p_columnname) const;
  XString GetCATALOGColumnList      (XString& p_schema,XString& p_tablename) const;
  XString GetCATALOGColumnAttributes(XString& p_schema,XString& p_tablename,XString& p_columnname) const;
  XString GetCATALOGColumnCreate    (MetaColumn& p_column) const;
  XString GetCATALOGColumnAlter     (MetaColumn& p_column) const;
  XString GetCATALOGColumnRename    (XString  p_schema,XString  p_tablename,XString  p_columnname,XString p_newname,XString p_datatype) const;
  XString GetCATALOGColumnDrop      (XString  p_schema,XString  p_tablename,XString  p_columnname) const;
  // All index functions
  XString GetCATALOGIndexExists     (XString  p_schema,XString  p_tablename,XString  p_indexname) const;
  XString GetCATALOGIndexList       (XString& p_schema,XString& p_tablename) const;
  XString GetCATALOGIndexAttributes (XString& p_schema,XString& p_tablename,XString& p_indexname) const;
  XString GetCATALOGIndexCreate     (MIndicesMap& p_index,bool p_duplicateNulls = false) const;
  XString GetCATALOGIndexDrop       (XString  p_schema,XString  p_tablename,XString  p_indexname) const;
  XString GetCATALOGIndexFilter     (MetaIndex& p_index) const;
  // All primary key functions
  XString GetCATALOGPrimaryExists    (XString  p_schema,XString  p_tablename) const;
  XString GetCATALOGPrimaryAttributes(XString& p_schema,XString& p_tablename) const;
  XString GetCATALOGPrimaryCreate    (MPrimaryMap& p_primaries) const;
  XString GetCATALOGPrimaryDrop      (XString  p_schema,XString  p_tablename,XString p_constraintname) const;
  // All foreign key functions
  XString GetCATALOGForeignExists    (XString  p_schema,XString  p_tablename,XString  p_constraintname) const;
  XString GetCATALOGForeignList      (XString& p_schema,XString& p_tablename,int p_maxColumns = SQLINFO_MAX_COLUMNS) const;
  XString GetCATALOGForeignAttributes(XString& p_schema,XString& p_tablename,XString& p_constraintname,bool p_referenced = false,int p_maxColumns = SQLINFO_MAX_COLUMNS) const;
  XString GetCATALOGForeignCreate    (MForeignMap& p_foreigns) const;
  XString GetCATALOGForeignAlter     (MForeignMap& p_original,MForeignMap& p_requested) const;
  XString GetCATALOGForeignDrop      (XString  p_schema,XString  p_tablename,XString  p_constraintname) const;
  // All default constraints
  XString GetCATALOGDefaultExists    (XString& p_schema,XString& p_tablename,XString& p_column) const;
  XString GetCATALOGDefaultList      (XString& p_schema,XString& p_tablename) const;
  XString GetCATALOGDefaultAttributes(XString& p_schema,XString& p_tablename,XString& p_column) const;
  XString GetCATALOGDefaultCreate    (XString  p_schema,XString  p_tablename,XString  p_constraint,XString p_column,XString p_code) const;
  XString GetCATALOGDefaultDrop      (XString  p_schema,XString  p_tablename,XString  p_constraint) const;
  // All check constraints
  XString GetCATALOGCheckExists      (XString  p_schema,XString  p_tablename,XString  p_constraint) const;
  XString GetCATALOGCheckList        (XString  p_schema,XString  p_tablename) const;
  XString GetCATALOGCheckAttributes  (XString  p_schema,XString  p_tablename,XString  p_constraint) const;
  XString GetCATALOGCheckCreate      (XString  p_schema,XString  p_tablename,XString  p_constraint,XString p_condition) const;
  XString GetCATALOGCheckDrop        (XString  p_schema,XString  p_tablename,XString  p_constraint) const;
  // All trigger functions
  XString GetCATALOGTriggerExists    (XString  p_schema,XString  p_tablename,XString  p_triggername) const;
  XString GetCATALOGTriggerList      (XString& p_schema,XString& p_tablename) const;
  XString GetCATALOGTriggerAttributes(XString& p_schema,XString& p_tablename,XString& p_triggername) const;
  XString GetCATALOGTriggerCreate    (MetaTrigger& p_trigger) const;
  XString GetCATALOGTriggerDrop      (XString  p_schema,XString  p_tablename,XString  p_triggername) const;
  // All sequence functions
  XString GetCATALOGSequenceExists    (XString  p_schema,XString  p_sequence) const;
  XString GetCATALOGSequenceList      (XString& p_schema,XString& p_pattern)  const;
  XString GetCATALOGSequenceAttributes(XString& p_schema,XString& p_sequence) const;
  XString GetCATALOGSequenceCreate    (MetaSequence& p_sequence) const;
  XString GetCATALOGSequenceDrop      (XString  p_schema,XString  p_sequence) const;
  // All view functions
  XString GetCATALOGViewExists       (XString& p_schema,XString& p_viewname) const;
  XString GetCATALOGViewList         (XString& p_schema,XString& p_pattern)  const;
  XString GetCATALOGViewAttributes   (XString& p_schema,XString& p_viewname) const;
  XString GetCATALOGViewText         (XString& p_schema,XString& p_viewname) const;
  XString GetCATALOGViewCreate       (XString  p_schema,XString  p_viewname,XString p_contents,bool p_ifexists = true) const;
  XString GetCATALOGViewRename       (XString  p_schema,XString  p_viewname,XString p_newname)    const;
  XString GetCATALOGViewDrop         (XString  p_schema,XString  p_viewname,XString& p_precursor) const;
  // All Privilege functions
  XString GetCATALOGTablePrivileges  (XString& p_schema,XString& p_tablename) const;
  XString GetCATALOGColumnPrivileges (XString& p_schema,XString& p_tablename,XString& p_columnname) const;
  XString GetCATALOGSequencePrivilege(XString& p_schema,XString& p_sequence) const;
  XString GetCATALOGGrantPrivilege   (XString  p_schema,XString  p_objectname,XString p_privilege,XString p_grantee,bool p_grantable);
  XString GetCATALOGRevokePrivilege  (XString  p_schema,XString  p_objectname,XString p_privilege,XString p_grantee);
  // All Synonym functions
  XString GetCATALOGSynonymList      (XString& p_schema,XString& p_pattern) const;
  XString GetCATALOGSynonymAttributes(XString& p_schema,XString& p_synonym) const;
  XString GetCATALOGSynonymCreate    (XString& p_schema,XString& p_synonym,XString p_forObject,bool p_private = true) const;
  XString GetCATALOGSynonymDrop      (XString& p_schema,XString& p_synonym,bool p_private = true) const;

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
  XString GetPSMProcedureExists    (XString  p_schema,XString  p_procedure) const;
  XString GetPSMProcedureList      (XString& p_schema) const;
  XString GetPSMProcedureAttributes(XString& p_schema,XString& p_procedure) const;
  XString GetPSMProcedureSourcecode(XString  p_schema,XString  p_procedure) const;
  XString GetPSMProcedureCreate    (MetaProcedure& p_procedure) const;
  XString GetPSMProcedureDrop      (XString  p_schema,XString  p_procedure,bool p_function = false) const;
  XString GetPSMProcedureErrors    (XString  p_schema,XString  p_procedure) const;
  XString GetPSMProcedurePrivilege (XString& p_schema,XString& p_procedure) const;
  // And it's parameters
  XString GetPSMProcedureParameters(XString& p_schema,XString& p_procedure) const;

  // All Language elements
  XString GetPSMDeclaration(bool p_first,XString p_variable,int p_datatype,int p_precision = 0,int p_scale = 0,
                            XString p_default = "",XString p_domain = "",XString p_asColumn = "") const;
  XString GetPSMAssignment (XString p_variable,XString p_statement = "") const;
  XString GetPSMIF         (XString p_condition) const;
  XString GetPSMIFElse     () const;
  XString GetPSMIFEnd      () const;
  XString GetPSMWhile      (XString p_condition) const;
  XString GetPSMWhileEnd   () const;
  XString GetPSMLOOP       () const;
  XString GetPSMLOOPEnd    () const;
  XString GetPSMBREAK      () const;
  XString GetPSMRETURN     (XString p_statement = "") const;
  XString GetPSMExecute    (XString p_procedure,MParameterMap& p_parameters) const;

  // The CURSOR
  XString GetPSMCursorDeclaration(XString p_cursorname,XString p_select) const;
  XString GetPSMCursorFetch      (XString p_cursorname,std::vector<XString>& p_columnnames,std::vector<XString>& p_variablenames) const;

  // PSM Exceptions
  XString GetPSMExceptionCatchNoData() const;
  XString GetPSMExceptionCatch(XString p_sqlState) const;
  XString GetPSMExceptionRaise(XString p_sqlState) const;

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

  XString GetSESSIONMyself() const;
  XString GetSESSIONExists(XString p_sessionID) const;
  XString GetSESSIONList() const;
  XString GetSESSIONAttributes(XString p_sessionID) const;
  // Transactions
  XString GetSESSIONConstraintsDeferred()  const;
  XString GetSESSIONConstraintsImmediate() const;


  //////////////////////////////////////////////////////////////////////////
  //
  // Call FUNCTION/PROCEDURE from within program
  // As a RDBMS dependent extension of "DoSQLCall" of the SQLQuery object
  //
  //////////////////////////////////////////////////////////////////////////

  // Calling a stored function or procedure if the RDBMS does not support ODBC call escapes
  SQLVariant* DoSQLCall(SQLQuery* p_query,XString& p_schema,XString& p_procedure);
  // Calling a stored function with named parameters, returning a value
  SQLVariant* DoSQLCallNamedParameters(SQLQuery* p_query,XString& p_schema,XString& p_procedure);

private:
  // IMPLEMENTATION OF the DoSQLCall interface
  int     GetCountReturnParameters(SQLQuery* p_query);
  bool    DoSQLCallFunction (SQLQuery* p_query,XString& p_function);
  bool    DoSQLCallProcedure(SQLQuery* p_query,XString& p_procedure);
  XString ConstructSQLForFunctionCall (SQLQuery* p_query,SQLQuery* p_thecall,XString& p_function);
  XString ConstructSQLForProcedureCall(SQLQuery* p_query,SQLQuery* p_thecall,XString& p_procedure);
};

// End of namespace
}
