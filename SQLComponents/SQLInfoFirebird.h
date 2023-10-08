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
  explicit SQLInfoFirebird(SQLDatabase* p_database);
 ~SQLInfoFirebird();

  // RDBMS Uses INDENTITY or SEQUENCE interface
  void    SetUseSequences(bool p_sequences) override;
 
  //////////////////////////////////////////////////////////////////////////
  //
  // GENERALS (Strings & Booleans) 
  //
  //////////////////////////////////////////////////////////////////////////

  // Get the database type
  DatabaseType GetRDBMSDatabaseType() const override;

  // The name of the database vendor
  XString GetRDBMSVendorName() const override;

  // Get the physical database name
  XString GetRDBMSPhysicalDatabaseName() const override;

  // System catalog is stored in uppercase in the database?
  bool    GetRDBMSIsCatalogUpper() const override;

  // System catalog supports full ISO schemas (same tables per schema)
  bool    GetRDBMSUnderstandsSchemas() const override;

  // Supports database/ODBCdriver comments in SQL
  bool    GetRDBMSSupportsComments() const override;

  // Database can defer constraints until the end of a transaction
  bool    GetRDBMSSupportsDeferredConstraints() const override;

  // Database has ORDER BY with an expression, e.g. ORDER BY UPPER(columnname)
  bool    GetRDBMSSupportsOrderByExpression() const override;

  // Supports the ODBC escape sequence {[?=] CALL procedure (?,?,?)}
  bool    GetRDBMSSupportsODBCCallEscapes() const override;

  // If the database does not support the datatype TIME, it can be implemented as a DECIMAL
  bool    GetRDBMSSupportsDatatypeTime() const override;

  // If the database does not support the datatype INTERVAL, it can be implemented as a DECIMAL
  bool    GetRDBMSSupportsDatatypeInterval() const override;

  // Supports functions at the place of table columns in create/alter index statement
  bool    GetRDBMSSupportsFunctionalIndexes() const override;

  // Gets the maximum length of an SQL statement
  unsigned long GetRDBMSMaxStatementLength() const override;

  // Database must commit DDL commands in a transaction
  bool GetRDBMSMustCommitDDL() const override;

  // Correct maximum precision,scale for a NUMERIC datatype
  void GetRDBMSNumericPrecisionScale(SQLULEN& p_precision, SQLSMALLINT& p_scale) const override;

  // Maximum for a VARCHAR to be handled without AT-EXEC data. Assume NVARCHAR is half that size!
  int GetRDBMSMaxVarchar() const override;

  // KEYWORDS

  // Keyword for the current date and time
  XString GetKEYWORDCurrentTimestamp() const override;

  // String for the current date
  XString GetKEYWORDCurrentDate() const override;

  // Get the concatenation operator
  XString GetKEYWORDConcatanationOperator() const override;

  // Get quote character for strings
  XString GetKEYWORDQuoteCharacter() const override;

  // Get quote character around reserved words as an identifier
  XString GetKEYWORDReservedWordQuote() const override;

  // Get default NULL for parameter list input
  XString GetKEYWORDParameterDefaultNULL() const override;

  // Parameter is for INPUT and OUTPUT in parameter list
  XString GetKEYWORDParameterINOUT() const override;

  // Parameter is for OUTPUT only in parameter list
  XString GetKEYWORDParameterOUT() const override;

  // Get datatype of the IDENTITY primary key in a Network database
  XString GetKEYWORDNetworkPrimaryKeyType() const override;

  // Get datatype for timestamp (year to second)
  XString GetKEYWORDTypeTimestamp() const override;

  // Prefix for a parameter in a stored procedure
  XString GetKEYWORDParameterPrefix() const override;

  // Get select part to add new record identity to a table
  // Can be special column like 'OID' or a sequence select
  XString GetKEYWORDIdentityString(XString& p_tablename,XString p_postfix = "_seq") const override;

  // Gets the UPPER function
  XString GetKEYWORDUpper(XString& p_expression) const override;

  // Gets the construction for 1 minute ago
  XString GetKEYWORDInterval1MinuteAgo() const override;

  // Gets the Not-NULL-Value statement of the database
  XString GetKEYWORDStatementNVL(XString& p_test,XString& p_isnull) const override;

  // Gets the RDBMS definition of the datatype
  XString GetKEYWORDDataType(MetaColumn* p_column) override;

  // Gets the USER (current-user) keyword function
  XString GetKEYWORDCurrentUser() const override;

  // SQL

  // Connects to a default schema in the database/instance
  XString GetSQLDefaultSchema(XString p_user,XString p_schema) const override;

  // Gets the construction for inline generating a key within an INSERT statement
  XString GetSQLNewSerial(XString p_table, XString p_sequence) const override;

  // Gets the construction / select for generating a new serial identity
  XString GetSQLGenerateSerial(XString p_table) const override;

  // Gets the construction / select for the resulting effective generated serial
  XString GetSQLEffectiveSerial(XString p_identity) const override;

  // Gets the subtransaction commands
  XString GetSQLStartSubTransaction   (XString p_savepointName) const override;
  XString GetSQLCommitSubTransaction  (XString p_savepointName) const override;
  XString GetSQLRollbackSubTransaction(XString p_savepointName) const override;

  // FROM-Part for a query to select only 1 (one) record / or empty!
  XString GetSQLFromDualClause() const override;

  // Get SQL to lock  a table 
  XString GetSQLLockTable(XString p_schema, XString p_tablename,bool p_exclusive,int p_waittime) const override;

  // Get query to optimize the table statistics
  XString GetSQLOptimizeTable(XString p_schema, XString p_tablename) const override;

  // Transform query to select top <n> rows
  XString GetSQLTopNRows(XString p_sql,int p_top,int p_skip = 0) const override;

  // Query to perform a keep alive ping
  XString GetPing() const override;

  //////////////////////////////////////////////////////////////////////////
  // SQL STRINGS

  // Makes a SQL string from a given string, with all the right quotes
  XString GetSQLString(const XString& p_string) const override;

  // Get date string in engine format
  XString GetSQLDateString(int p_year,int p_month,int p_day) const override;

  // Get time string in database engine format
  XString GetSQLTimeString(int p_hour,int p_minute,int p_second) const override;

  // Get date-time string in database engine format
  XString GetSQLDateTimeString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const override;

  // Get date-time bound parameter string in database format
  XString GetSQLDateTimeBoundString() const override;

  // Stripped data for the parameter binding
  XString GetSQLDateTimeStrippedString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const override;

  // Makes an catalog identifier string (possibly quoted on both sides)
  virtual XString GetSQLDDLIdentifier(XString p_identifier) const override;

  // Changes to parameters before binding to an ODBC HSTMT handle
  void DoBindParameterFixup(SQLSMALLINT& p_sqlDatatype,SQLULEN& p_columnSize,SQLSMALLINT& p_scale,SQLLEN& p_bufferSize,SQLLEN* p_indicator) const override;

  //////////////////////////////////////////////////////////////////////////
  //
  // CATALOG
  // o GetCATALOG<Object[s]><Function>
  //   Objects
  //   - Catalog
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
  XString GetCATALOGMetaTypes(int p_type) const override;
  XString GetCATALOGDefaultCharset() const override;
  XString GetCATALOGDefaultCharsetNCV() const override;
  XString GetCATALOGDefaultCollation() const override;
  // All table functions
  XString GetCATALOGTableExists       (XString& p_schema,XString& p_tablename)  const override;
  XString GetCATALOGTablesList        (XString& p_schema,XString& p_pattern)    const override;
  XString GetCATALOGTableAttributes   (XString& p_schema,XString& p_tablename)  const override;
  XString GetCATALOGTableSynonyms     (XString& p_schema,XString& p_tablename)  const override;
  XString GetCATALOGTableCatalog      (XString& p_schema,XString& p_tablename)  const override;
  XString GetCATALOGTableCreate       (MetaTable& p_table,MetaColumn& p_column) const override;
  XString GetCATALOGTableCreatePostfix(MetaTable& p_table,MetaColumn& p_column) const override;
  XString GetCATALOGTableRename       (XString  p_schema,XString  p_tablename,XString p_newname) const override;
  XString GetCATALOGTableDrop         (XString  p_schema,XString  p_tablename,bool p_ifExist = false,bool p_restrict = false,bool p_cascade = false) const override;
  // All Temporary table functions
  XString GetCATALOGTemptableCreate   (XString p_schema,XString p_tablename,XString p_select) const override;
  XString GetCATALOGTemptableIntoTemp (XString p_schema,XString p_tablename,XString p_select) const override;
  XString GetCATALOGTemptableDrop     (XString p_schema,XString p_tablename) const override;
  // All column functions
  XString GetCATALOGColumnExists    (XString  p_schema,XString  p_tablename,XString  p_columnname) const override;
  XString GetCATALOGColumnList      (XString& p_schema,XString& p_tablename) const override;
  XString GetCATALOGColumnAttributes(XString& p_schema,XString& p_tablename,XString& p_columnname) const override;
  XString GetCATALOGColumnCreate    (MetaColumn& p_column) const override;
  XString GetCATALOGColumnAlter     (MetaColumn& p_column) const override;
  XString GetCATALOGColumnRename    (XString  p_schema,XString  p_tablename,XString  p_columnname,XString p_newname,XString p_datatype) const override;
  XString GetCATALOGColumnDrop      (XString  p_schema,XString  p_tablename,XString  p_columnname) const override;
  // All index functions
  XString GetCATALOGIndexExists     (XString  p_schema,XString  p_tablename,XString  p_indexname) const override;
  XString GetCATALOGIndexList       (XString& p_schema,XString& p_tablename) const override;
  XString GetCATALOGIndexAttributes (XString& p_schema,XString& p_tablename,XString& p_indexname) const override;
  XString GetCATALOGIndexCreate     (MIndicesMap& p_index,bool p_duplicateNulls = false) const override;
  XString GetCATALOGIndexDrop       (XString  p_schema,XString  p_tablename,XString  p_indexname) const override;
  XString GetCATALOGIndexFilter     (MetaIndex& p_index) const override;
  // All primary key functions
  XString GetCATALOGPrimaryExists    (XString  p_schema,XString  p_tablename) const override;
  XString GetCATALOGPrimaryAttributes(XString& p_schema,XString& p_tablename) const override;
  XString GetCATALOGPrimaryCreate    (MPrimaryMap& p_primaries) const override;
  XString GetCATALOGPrimaryDrop      (XString  p_schema,XString  p_tablename,XString p_constraintname) const override;
  // All foreign key functions
  XString GetCATALOGForeignExists    (XString  p_schema,XString  p_tablename,XString  p_constraintname) const override;
  XString GetCATALOGForeignList      (XString& p_schema,XString& p_tablename,int p_maxColumns = SQLINFO_MAX_COLUMNS) const override;
  XString GetCATALOGForeignAttributes(XString& p_schema,XString& p_tablename,XString& p_constraintname,bool p_referenced = false,int p_maxColumns = SQLINFO_MAX_COLUMNS) const override;
  XString GetCATALOGForeignCreate    (MForeignMap& p_foreigns) const override;
  XString GetCATALOGForeignAlter     (MForeignMap& p_original,MForeignMap& p_requested) const override;
  XString GetCATALOGForeignDrop      (XString  p_schema,XString  p_tablename,XString  p_constraintname) const override;
  // All default constraints
  XString GetCATALOGDefaultExists    (XString& p_schema,XString& p_tablename,XString& p_column) const override;
  XString GetCATALOGDefaultList      (XString& p_schema,XString& p_tablename) const override;
  XString GetCATALOGDefaultAttributes(XString& p_schema,XString& p_tablename,XString& p_column) const override;
  XString GetCATALOGDefaultCreate    (XString  p_schema,XString  p_tablename,XString  p_constraint,XString p_column,XString p_code) const override;
  XString GetCATALOGDefaultDrop      (XString  p_schema,XString  p_tablename,XString  p_constraint) const override;
  // All check constraints
  XString GetCATALOGCheckExists      (XString  p_schema,XString  p_tablename,XString  p_constraint) const override;
  XString GetCATALOGCheckList        (XString  p_schema,XString  p_tablename) const override;
  XString GetCATALOGCheckAttributes  (XString  p_schema,XString  p_tablename,XString  p_constraint) const override;
  XString GetCATALOGCheckCreate      (XString  p_schema,XString  p_tablename,XString  p_constraint,XString p_condition) const override;
  XString GetCATALOGCheckDrop        (XString  p_schema,XString  p_tablename,XString  p_constraint) const override;
  // All trigger functions
  XString GetCATALOGTriggerExists    (XString  p_schema,XString  p_tablename,XString  p_triggername) const override;
  XString GetCATALOGTriggerList      (XString& p_schema,XString& p_tablename) const override;
  XString GetCATALOGTriggerAttributes(XString& p_schema,XString& p_tablename,XString& p_triggername) const override;
  XString GetCATALOGTriggerCreate    (MetaTrigger& p_trigger) const override;
  XString GetCATALOGTriggerDrop      (XString  p_schema,XString  p_tablename,XString  p_triggername) const override;
  // All sequence functions
  XString GetCATALOGSequenceExists    (XString  p_schema,XString  p_sequence) const override;
  XString GetCATALOGSequenceList      (XString& p_schema,XString& p_pattern)  const override;
  XString GetCATALOGSequenceAttributes(XString& p_schema,XString& p_sequence) const override;
  XString GetCATALOGSequenceCreate    (MetaSequence& p_sequence) const override;
  XString GetCATALOGSequenceDrop      (XString  p_schema,XString  p_sequence) const override;
  // All view functions
  XString GetCATALOGViewExists       (XString& p_schema,XString& p_viewname) const override;
  XString GetCATALOGViewList         (XString& p_schema,XString& p_pattern)  const override;
  XString GetCATALOGViewAttributes   (XString& p_schema,XString& p_viewname) const override;
  XString GetCATALOGViewText         (XString& p_schema,XString& p_viewname) const override;
  XString GetCATALOGViewCreate       (XString  p_schema,XString  p_viewname,XString p_contents,bool p_ifexists = true) const override;
  XString GetCATALOGViewRename       (XString  p_schema,XString  p_viewname,XString p_newname)    const override;
  XString GetCATALOGViewDrop         (XString  p_schema,XString  p_viewname,XString& p_precursor) const override;
  // All Privilege functions
  XString GetCATALOGTablePrivileges  (XString& p_schema,XString& p_tablename) const override;
  XString GetCATALOGColumnPrivileges (XString& p_schema,XString& p_tablename,XString& p_columnname) const override;
  XString GetCATALOGSequencePrivilege(XString& p_schema,XString& p_sequence) const override;
  XString GetCATALOGGrantPrivilege   (XString  p_schema,XString  p_objectname,XString p_privilege,XString p_grantee,bool p_grantable) override;
  XString GetCATALOGRevokePrivilege  (XString  p_schema,XString  p_objectname,XString p_privilege,XString p_grantee) override;
  // All Synonym functions
  XString GetCATALOGSynonymList      (XString& p_schema,XString& p_pattern) const override;
  XString GetCATALOGSynonymAttributes(XString& p_schema,XString& p_synonym) const override;
  XString GetCATALOGSynonymCreate    (XString& p_schema,XString& p_synonym,XString p_forObject,bool p_private = true) const override;
  XString GetCATALOGSynonymDrop      (XString& p_schema,XString& p_synonym,bool p_private = true) const override;

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
  XString GetPSMProcedureExists    (XString  p_schema,XString  p_procedure) const override;
  XString GetPSMProcedureList      (XString& p_schema) const override;
  XString GetPSMProcedureAttributes(XString& p_schema,XString& p_procedure) const override;
  XString GetPSMProcedureSourcecode(XString  p_schema,XString  p_procedure) const override;
  XString GetPSMProcedureCreate    (MetaProcedure& p_procedure) const override;
  XString GetPSMProcedureDrop      (XString  p_schema,XString  p_procedure,bool p_function = false) const override;
  XString GetPSMProcedureErrors    (XString  p_schema,XString  p_procedure) const override;
  XString GetPSMProcedurePrivilege (XString& p_schema,XString& p_procedure) const override;
  // And it's parameters
  XString GetPSMProcedureParameters(XString& p_schema,XString& p_procedure) const override;

  // All Language elements
  XString GetPSMDeclaration(bool p_first,XString p_variable,int p_datatype,int p_precision = 0,int p_scale = 0,
                            XString p_default = "",XString p_domain = "",XString p_asColumn = "") const override;
  XString GetPSMAssignment (XString p_variable,XString p_statement = "") const override;
  XString GetPSMIF         (XString p_condition) const override;
  XString GetPSMIFElse     () const override;
  XString GetPSMIFEnd      () const override;
  XString GetPSMWhile      (XString p_condition) const override;
  XString GetPSMWhileEnd   () const override;
  XString GetPSMLOOP       () const override;
  XString GetPSMLOOPEnd    () const override;
  XString GetPSMBREAK      () const override;
  XString GetPSMRETURN     (XString p_statement = "") const override;
  XString GetPSMExecute    (XString p_procedure,MParameterMap& p_parameters) const override;

  // The CURSOR
  XString GetPSMCursorDeclaration(XString p_cursorname,XString p_select) const override;
  XString GetPSMCursorFetch      (XString p_cursorname,std::vector<XString>& p_columnnames,std::vector<XString>& p_variablenames) const override;

  // PSM Exceptions
  XString GetPSMExceptionCatchNoData() const override;
  XString GetPSMExceptionCatch(XString p_sqlState) const override;
  XString GetPSMExceptionRaise(XString p_sqlState) const override;

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

  XString GetSESSIONMyself() const override;
  XString GetSESSIONExists(XString p_sessionID) const override;
  XString GetSESSIONList() const override;
  XString GetSESSIONAttributes(XString p_sessionID) const override;
  // Transactions
  XString GetSESSIONConstraintsDeferred()  const override;
  XString GetSESSIONConstraintsImmediate() const override;


  //////////////////////////////////////////////////////////////////////////
  //
  // Call FUNCTION/PROCEDURE from within program
  // As a RDBMS dependent extension of "DoSQLCall" of the SQLQuery object
  //
  //////////////////////////////////////////////////////////////////////////

  // Calling a stored function or procedure if the RDBMS does not support ODBC call escapes
  SQLVariant* DoSQLCall(SQLQuery* p_query,XString& p_schema,XString& p_procedure) override;
  // Calling a stored function with named parameters, returning a value
  SQLVariant* DoSQLCallNamedParameters(SQLQuery* p_query,XString& p_schema,XString& p_procedure) override;

private:
  // IMPLEMENTATION OF the DoSQLCall interface
  int     GetCountReturnParameters(SQLQuery* p_query);
  bool    DoSQLCallFunction (SQLQuery* p_query,const XString& p_function);
  bool    DoSQLCallProcedure(SQLQuery* p_query,const XString& p_procedure);
  XString ConstructSQLForFunctionCall (SQLQuery* p_query,SQLQuery* p_thecall,const XString& p_function);
  XString ConstructSQLForProcedureCall(SQLQuery* p_query,SQLQuery* p_thecall,const XString& p_procedure);
};

// End of namespace
}
