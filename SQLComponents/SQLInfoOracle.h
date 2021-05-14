////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoOracle.h
//
// Copyright (c) 1998-2021 ir. W.E. Huisman
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

#define NUMERIC_MAX_PRECISION      38
#define NUMERIC_MIN_SCALE           0
#define NUMERIC_DEFAULT_SCALE      18

class SQLInfoOracle : public SQLInfoDB
{
public:
  SQLInfoOracle(SQLDatabase* p_database);
 ~SQLInfoOracle();
 
  //////////////////////////////////////////////////////////////////////////
  //
  // GENERALS (Strings & Booleans) 
  //
  //////////////////////////////////////////////////////////////////////////

  // Get the database type
  DatabaseType GetRDBMSDatabaseType() const override;

  // The name of the database vendor
  CString GetRDBMSVendorName() const override;

  // Get the physical database name
  CString GetRDBMSPhysicalDatabaseName() const override;

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

  // KEYWORDS

  // Keyword for the current date and time
  CString GetKEYWORDCurrentTimestamp() const override;

  // String for the current date
  CString GetKEYWORDCurrentDate() const override;

  // Get the concatenation operator
  CString GetKEYWORDConcatanationOperator() const override;

  // Get quote character for strings
  CString GetKEYWORDQuoteCharacter() const override;

  // Get quote character around reserved words as an identifier
  CString GetKEYWORDReservedWordQuote() const override;

  // Get default NULL for parameter list input
  CString GetKEYWORDParameterDefaultNULL() const override;

  // Parameter is for INPUT and OUTPUT in parameter list
  CString GetKEYWORDParameterINOUT() const override;

  // Parameter is for OUTPUT only in parameter list
  CString GetKEYWORDParameterOUT() const override;

  // Get datatype of the IDENTITY primary key in a Network database
  CString GetKEYWORDNetworkPrimaryKeyType() const override;

  // Get datatype for timestamp (year to second)
  CString GetKEYWORDTypeTimestamp() const override;

  // Prefix for a parameter in a stored procedure
  CString GetKEYWORDParameterPrefix() const override;

  // Get select part to add new record identity to a table
  // Can be special column like 'OID' or a sequence select
  CString GetKEYWORDIdentityString(CString& p_tablename,CString p_postfix = "_seq") const override;

  // Gets the UPPER function
  CString GetKEYWORDUpper(CString& p_expression) const override;

  // Gets the construction for 1 minute ago
  CString GetKEYWORDInterval1MinuteAgo() const override;

  // Gets the Not-NULL-Value statement of the database
  CString GetKEYWORDStatementNVL(CString& p_test,CString& p_isnull) const override;

  // Gets the RDBMS definition of the datatype
  CString GetKEYWORDDataType(MetaColumn* p_column);

  // Gets the USER (current-user) keyword function
  CString GetKEYWORDCurrentUser() const override;

  // SQL

  // Connects to a default schema in the database/instance
  CString GetSQLDefaultSchema(CString p_schema) const override;

  // Gets the construction for inline generating a key within an INSERT statement
  CString GetSQLNewSerial(CString p_table, CString p_sequence) const override;

  // Gets the construction / select for generating a new serial identity
  CString GetSQLGenerateSerial(CString p_table) const override;

  // Gets the construction / select for the resulting effective generated serial
  CString GetSQLEffectiveSerial(CString p_identity) const override;

  // Gets the subtransaction commands
  CString GetSQLStartSubTransaction(   CString p_savepointName) const override;
  CString GetSQLCommitSubTransaction  (CString p_savepointName) const override;
  CString GetSQLRollbackSubTransaction(CString p_savepointName) const override;

  // FROM-Part for a query to select only 1 (one) record / or empty!
  CString GetSQLFromDualClause() const override;

  // Get SQL to lock  a table 
  CString GetSQLLockTable(CString p_schema, CString p_tablename, bool p_exclusive) const override;

  // Get query to optimize the table statistics
  CString GetSQLOptimizeTable(CString p_schema, CString p_tablename) const override;

  // Transform query to select top <n> rows
  CString GetSQLTopNRows(CString p_sql,int p_top,int p_skip = 0) const override;

  //////////////////////////////////////////////////////////////////////////
  // SQL STRINGS

  // Makes a SQL string from a given string, with all the right quotes
  CString GetSQLString(const CString& p_string) const override;

  // Get date string in engine format
  CString GetSQLDateString(int p_year,int p_month,int p_day) const override;

  // Get time string in database engine format
  CString GetSQLTimeString(int p_hour,int p_minute,int p_second) const override;

  // Get date-time string in database engine format
  CString GetSQLDateTimeString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const override;

  // Get date-time bound parameter string in database format
  CString GetSQLDateTimeBoundString() const override;

  // Stripped data for the parameter binding
  CString GetSQLDateTimeStrippedString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const override;

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
  //   - Privileges
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
  CString GetCATALOGMetaTypes(int p_type) const override;
  // All table functions
  CString GetCATALOGTableExists       (CString& p_schema,CString& p_tablename)  const override;
  CString GetCATALOGTablesList        (CString& p_schema,CString& p_pattern)    const override;
  CString GetCATALOGTableAttributes   (CString& p_schema,CString& p_tablename)  const override;
  CString GetCATALOGTableSynonyms     (CString& p_schema,CString& p_tablename)  const override;
  CString GetCATALOGTableCatalog      (CString& p_schema,CString& p_tablename)  const override;
  CString GetCATALOGTableCreate       (MetaTable& p_table,MetaColumn& p_column) const override;
  CString GetCATALOGTableCreatePostfix(MetaTable& p_table,MetaColumn& p_column) const override;
  CString GetCATALOGTableRename       (CString  p_schema,CString  p_tablename,CString p_newname) const override;
  CString GetCATALOGTableDrop         (CString  p_schema,CString  p_tablename,bool p_ifExist = false,bool p_restrict = false,bool p_cascade = false) const override;
  // All Temporary table functions
  CString GetCATALOGTemptableCreate  (CString p_schema,CString p_tablename,CString p_select) const override;
  CString GetCATALOGTemptableIntoTemp(CString p_schema,CString p_tablename,CString p_select) const override;
  CString GetCATALOGTemptableDrop    (CString p_schema,CString p_tablename) const override;
  // All column functions
  CString GetCATALOGColumnExists    (CString  p_schema,CString  p_tablename,CString  p_columnname) const override;
  CString GetCATALOGColumnList      (CString& p_schema,CString& p_tablename) const override;
  CString GetCATALOGColumnAttributes(CString& p_schema,CString& p_tablename,CString& p_columnname) const override;
  CString GetCATALOGColumnCreate    (MetaColumn& p_column) const override;
  CString GetCATALOGColumnAlter     (MetaColumn& p_column) const override;
  CString GetCATALOGColumnRename    (CString  p_schema,CString  p_tablename,CString  p_columnname,CString p_newname,CString p_datatype) const override;
  CString GetCATALOGColumnDrop      (CString  p_schema,CString  p_tablename,CString  p_columnname) const override;
  // All index functions
  CString GetCATALOGIndexExists     (CString  p_schema,CString  p_tablename,CString  p_indexname) const override;
  CString GetCATALOGIndexList       (CString& p_schema,CString& p_tablename) const override;
  CString GetCATALOGIndexAttributes (CString& p_schema,CString& p_tablename,CString& p_indexname) const override;
  CString GetCATALOGIndexCreate     (MIndicesMap& p_index) const override;
  CString GetCATALOGIndexDrop       (CString  p_schema,CString  p_tablename,CString  p_indexname) const override;
  CString GetCATALOGIndexFilter     (MetaIndex& p_index) const override;
  // All primary key functions
  CString GetCATALOGPrimaryExists    (CString  p_schema,CString  p_tablename) const override;
  CString GetCATALOGPrimaryAttributes(CString& p_schema,CString& p_tablename) const override;
  CString GetCATALOGPrimaryCreate    (MPrimaryMap& p_primaries) const override;
  CString GetCATALOGPrimaryDrop      (CString  p_schema,CString  p_tablename,CString p_constraintname) const override;
  // All foreign key functions
  CString GetCATALOGForeignExists    (CString  p_schema,CString  p_tablename,CString  p_constraintname) const override;
  CString GetCATALOGForeignList      (CString& p_schema,CString& p_tablename,int p_maxColumns = SQLINFO_MAX_COLUMNS) const override;
  CString GetCATALOGForeignAttributes(CString& p_schema,CString& p_tablename,CString& p_constraintname,bool p_referenced = false,int p_maxColumns = SQLINFO_MAX_COLUMNS) const override;
  CString GetCATALOGForeignCreate    (MForeignMap& p_foreigns) const override;
  CString GetCATALOGForeignAlter     (MForeignMap& p_original,MForeignMap& p_requested) const override;
  CString GetCATALOGForeignDrop      (CString  p_schema,CString  p_tablename,CString  p_constraintname) const override;
  // All trigger functions
  CString GetCATALOGTriggerExists    (CString  p_schema,CString  p_tablename,CString  p_triggername) const override;
  CString GetCATALOGTriggerList      (CString& p_schema,CString& p_tablename) const override;
  CString GetCATALOGTriggerAttributes(CString& p_schema,CString& p_tablename,CString& p_triggername) const override;
  CString GetCATALOGTriggerCreate    (MetaTrigger& p_trigger) const override;
  CString GetCATALOGTriggerDrop      (CString  p_schema,CString  p_tablename,CString  p_triggername) const override;
    // All sequence functions
  CString GetCATALOGSequenceExists    (CString  p_schema,CString  p_sequence) const override;
  CString GetCATALOGSequenceList      (CString& p_schema,CString& p_pattern)  const override;
  CString GetCATALOGSequenceAttributes(CString& p_schema,CString& p_sequence) const override;
  CString GetCATALOGSequenceCreate    (MetaSequence& p_sequence) const override;
  CString GetCATALOGSequenceDrop      (CString  p_schema,CString  p_sequence) const override;
  // All view functions
  CString GetCATALOGViewExists       (CString& p_schema,CString& p_viewname) const override;
  CString GetCATALOGViewList         (CString& p_schema,CString& p_pattern)  const override;
  CString GetCATALOGViewAttributes   (CString& p_schema,CString& p_viewname) const override;
  CString GetCATALOGViewText         (CString& p_schema,CString& p_viewname) const override;
  CString GetCATALOGViewCreate       (CString  p_schema,CString  p_viewname,CString p_contents)   const override;
  CString GetCATALOGViewRename       (CString  p_schema,CString  p_viewname,CString p_newname)    const override;
  CString GetCATALOGViewDrop         (CString  p_schema,CString  p_viewname,CString& p_precursor) const override;
  // All Privilege functions
  CString GetCATALOGTablePrivileges  (CString& p_schema,CString& p_tablename) const override;
  CString GetCATALOGColumnPrivileges (CString& p_schema,CString& p_tablename,CString& p_columnname) const override;
  CString GetCatalogGrantPrivilege   (CString  p_schema,CString  p_objectname,CString p_privilege,CString p_grantee,bool p_grantable);
  CString GetCatalogRevokePrivilege  (CString  p_schema,CString  p_objectname,CString p_privilege,CString p_grantee);

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
  CString GetPSMProcedureExists    (CString  p_schema,CString  p_procedure) const override;
  CString GetPSMProcedureList      (CString& p_schema) const override;
  CString GetPSMProcedureAttributes(CString& p_schema,CString& p_procedure) const override;
  CString GetPSMProcedureSourcecode(CString  p_schema,CString  p_procedure) const override;
  CString GetPSMProcedureCreate    (MetaProcedure& p_procedure) const override;
  CString GetPSMProcedureDrop      (CString  p_schema,CString  p_procedure) const override;
  CString GetPSMProcedureErrors    (CString  p_schema,CString  p_procedure) const override;
  // And it's parameters
  CString GetPSMProcedureParameters(CString& p_schema,CString& p_procedure) const override;

  // All Language elements
  CString GetPSMDeclaration(bool p_first,CString p_variable,int p_datatype,int p_precision = 0,int p_scale = 0,
                            CString p_default = "",CString p_domain = "",CString p_asColumn = "") const override;
  CString GetPSMAssignment (CString p_variable,CString p_statement = "") const override;
  CString GetPSMIF         (CString p_condition) const override;
  CString GetPSMIFElse     () const override;
  CString GetPSMIFEnd      () const override;
  CString GetPSMWhile      (CString p_condition) const override;
  CString GetPSMWhileEnd   () const override;
  CString GetPSMLOOP       () const override;
  CString GetPSMLOOPEnd    () const override;
  CString GetPSMBREAK      () const override;
  CString GetPSMRETURN     (CString p_statement = "") const override;
  CString GetPSMExecute    (CString p_procedure,MParameterMap& p_parameters) const override;

  // The CURSOR
  CString GetPSMCursorDeclaration(CString p_cursorname,CString p_select) const override;
  CString GetPSMCursorFetch      (CString p_cursorname,std::vector<CString>& p_columnnames,std::vector<CString>& p_variablenames) const override;

  // PSM Exceptions
  CString GetPSMExceptionCatchNoData() const override;
  CString GetPSMExceptionCatch(CString p_sqlState) const override;
  CString GetPSMExceptionRaise(CString p_sqlState) const override;

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

  CString GetSESSIONMyself() const override;
  CString GetSESSIONExists(CString p_sessionID) const override;
  CString GetSESSIONList() const override;
  CString GetSESSIONAttributes(CString p_sessionID) const override;
  // Transactions
  CString GetSESSIONConstraintsDeferred()  const override;
  CString GetSESSIONConstraintsImmediate() const override;

  //////////////////////////////////////////////////////////////////////////
  //
  // Call FUNCTION/PROCEDURE from within program
  // As a RDBMS dependent extension of "DoSQLCall" of the SQLQuery object
  //
  //////////////////////////////////////////////////////////////////////////

  // Calling a stored function or procedure if the RDBMS does not support ODBC call escapes
  SQLVariant* DoSQLCall(SQLQuery* p_query,CString& p_schema,CString& p_procedure) override;
  // Calling a stored function with named parameters, returning a value
  SQLVariant* DoSQLCallNamedParameters(SQLQuery* p_query,CString& p_schema,CString& p_procedure) override;
};

// End of namespace
}
