////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoDB.h
//
// Copyright (c) 1998-2025 ir. W.E. Huisman
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

class SQLInfoDB : public SQLInfo
{
public:
  SQLInfoDB(SQLDatabase* p_database);
  virtual ~SQLInfoDB();

  // DB INTERFACE

  // Default list of granted users on a created object
  void    SetGrantedUsers(XString p_users);
  XString GetGrantedUsers() const;
  // We prefer the strict 'standard' ODBC drivers meta queries
  void    SetPreferODBC(bool p_prefer);
  bool    GetPreferODBC();

  // OVERRIDES AND EXTRAS OF THE ODBC MakeInfo<object> functions

  // Meta info about meta types: META_CATALOGS/META_SCHEMAS/META_TABLES
  virtual bool    MakeInfoMetaTypes       (MMetaMap&      p_objects,   XString& p_errors,int p_type) override;
  virtual bool    MakeInfoDefaultCharset  (XString&       p_default);
  virtual bool    MakeInfoDefaultCharsetNC(XString&       p_default);
  virtual bool    MakeInfoDefaultCollation(XString&       p_default);
  // Tables
  virtual bool    MakeInfoTableObject     (MTableMap&     p_tables,    XString& p_errors,XString p_schema,XString p_tablename);  // Not known which type!
  virtual bool    MakeInfoTableTable      (MTableMap&     p_tables,    XString& p_errors,XString p_schema,XString p_tablename);  // TABLE   only
  virtual bool    MakeInfoTableView       (MTableMap&     p_tables,    XString& p_errors,XString p_schema,XString p_tablename);  // VIEW    only
  virtual bool    MakeInfoTableSynonyms   (MTableMap&     p_tables,    XString& p_errors,XString p_schema,XString p_tablename);  // SYNONYM only
  virtual bool    MakeInfoTableCatalog    (MTableMap&     p_tables,    XString& p_errors,XString p_schema,XString p_tablename);  // CATALOG only
  // Attributes of a table
  virtual bool    MakeInfoTableColumns    (MColumnMap&    p_columns,   XString& p_errors,XString p_schema,XString p_tablename,XString p_columnname = _T("")) override;
  virtual bool    MakeInfoTablePrimary    (MPrimaryMap&   p_primary,   XString& p_errors,XString p_schema,XString p_tablename)                          override;
  virtual bool    MakeInfoTableForeign    (MForeignMap&   p_foreigns,  XString& p_errors,XString p_schema,XString p_tablename,bool p_referenced   = false) override;
  virtual bool    MakeInfoTableStatistics (MIndicesMap&   p_indices,   XString& p_errors,XString p_schema,XString p_tablename,MPrimaryMap* p_keymap,bool p_all = true) override;
  virtual bool    MakeInfoTableTriggers   (MTriggerMap&   p_triggers,  XString& p_errors,XString p_schema,XString p_tablename,XString p_trigger   = _T(""));
  virtual bool    MakeInfoTableSequences  (MSequenceMap&  p_sequences, XString& p_errors,XString p_schema,XString p_tablename);
  virtual bool    MakeInfoTablePrivileges (MPrivilegeMap& p_privileges,XString& p_errors,XString p_schema,XString p_tablename) override;
  virtual bool    MakeInfoColumnPrivileges(MPrivilegeMap& p_privileges,XString& p_errors,XString p_schema,XString p_tablename,XString p_columnname = _T("")) override;
  virtual bool    MakeInfoViewDefinition  (XString&       p_defintion, XString& p_errors,XString p_schema,XString p_viewname);

  // Procedures
  virtual bool    MakeInfoPSMProcedures(MProcedureMap& p_procedures,XString& p_errors,XString p_schema,XString p_procedure) override;
  virtual bool    MakeInfoPSMParameters(MParameterMap& p_parameters,XString& p_errors,XString p_schema,XString p_procedure) override;
  // Read extra source code for database that can only do it by an extra procedure
  virtual XString MakeInfoPSMSourcecode(XString p_schema, XString p_procedure);

  // PURE VIRTUAL INTERFACE

  // RDBMS Uses INDENTITY or SEQUENCE interface
  virtual void    SetUseSequences(bool p_sequences) = 0;

  //////////////////////////////////////////////////////////////////////////
  //
  // GENERALS (Strings & Booleans) 
  //   o GetRDBMS<x>              (boolean,string)
  //   o GetKEYWORD<x>            (string)
  //   o GetSQL<special-process>  (SQL-string)
  //
  //////////////////////////////////////////////////////////////////////////

  // Get the database type
  virtual DatabaseType GetRDBMSDatabaseType() const = 0;

  // The name of the database vendor
  virtual XString GetRDBMSVendorName() const = 0;

  // Get the physical database name
  virtual XString GetRDBMSPhysicalDatabaseName() const = 0;

  // System catalog is stored in uppercase in the database?
  virtual bool GetRDBMSIsCatalogUpper() const = 0;

  // System catalog supports full ISO schemas (same tables per schema)
  virtual bool GetRDBMSUnderstandsSchemas() const = 0;

  // Supports database/ODBCdriver comments in SQL
  virtual bool GetRDBMSSupportsComments() const = 0;

  // Database can defer constraints until the end of a transaction
  virtual bool GetRDBMSSupportsDeferredConstraints() const = 0;

  // Database has ORDER BY with an expression, e.g. ORDER BY UPPER(columnname)
  // Work-around is "SELECT UPPER(columnname) AS something.....ORDER BY something
  virtual bool GetRDBMSSupportsOrderByExpression() const = 0;

  // Supports the ODBC escape sequence {[?=] CALL procedure (?,?,?)}
  virtual bool GetRDBMSSupportsODBCCallEscapes() const = 0;

  // Supports the ODBC call procedure with named parameters
  virtual bool GetRDBMSSupportsODBCCallNamedParameters() const = 0;

  // If the database does not support the datatype TIME, it can be implemented as a DECIMAL
  // BEWARE BOOL INVERTED!!
  virtual bool GetRDBMSSupportsDatatypeTime() const = 0;

  // If the database does not support the datatype INTERVAL, it can be implemented as a DECIMAL
  // BEWARE BOOL INVERTED!!
  virtual bool GetRDBMSSupportsDatatypeInterval() const = 0;

  // Supports functions at the place of table columns in create/alter index statement
  virtual bool GetRDBMSSupportsFunctionalIndexes() const = 0;

  // Gets the maximum length of an SQL statement
  virtual unsigned long GetRDBMSMaxStatementLength() const = 0;

  // Database must commit DDL commands in a transaction
  virtual bool GetRDBMSMustCommitDDL() const = 0;

  // Correct maximum precision,scale for a NUMERIC datatype
  virtual void GetRDBMSNumericPrecisionScale(SQLULEN& p_precision,SQLSMALLINT& p_scale) const = 0;

  // Maximum for a VARCHAR to be handled without AT-EXEC data. Assume NVARCHAR is half that size!
  virtual int GetRDBMSMaxVarchar() const = 0;

  //////////////////////////////////////////////////////////////////////////
  // KEYWORDS

  // Keyword for the current date and time
  virtual XString GetKEYWORDCurrentTimestamp() const = 0;

  // String for the current date
  virtual XString GetKEYWORDCurrentDate() const = 0;

  // Get the concatenation operator
  virtual XString GetKEYWORDConcatanationOperator() const = 0;

  // Get quote character for strings
  virtual XString GetKEYWORDQuoteCharacter() const = 0;

  // Get quote character around reserved words as an identifier
  virtual XString GetKEYWORDReservedWordQuote() const = 0;

  // Get default NULL for parameter list input
  virtual XString GetKEYWORDParameterDefaultNULL() const = 0;

  // Parameter is for INPUT and OUTPUT in parameter list
  virtual XString GetKEYWORDParameterINOUT() const = 0;

  // Parameter is for OUTPUT only in parameter list
  virtual XString GetKEYWORDParameterOUT() const = 0;

  // Get datatype of the IDENTITY primary key in a Network database
  virtual XString GetKEYWORDNetworkPrimaryKeyType() const = 0;

  // Get datatype for Moment
  virtual XString GetKEYWORDTypeTimestamp() const = 0;

  // Prefix for a parameter in a stored procedure
  virtual XString  GetKEYWORDParameterPrefix() const = 0;

  // Get select part to add new record identity to a table
  // Can be special column like 'OID' or a sequence select
  virtual XString GetKEYWORDIdentityString(XString& p_tablename,XString p_postfix = _T("_seq")) const = 0;

  // Gets the UPPER function
  virtual XString GetKEYWORDUpper(XString& p_expression) const = 0;

  // Gets the construction for 1 minute ago
  virtual XString GetKEYWORDInterval1MinuteAgo() const = 0;

  // Gets the Not-NULL-Value statement of the database
  virtual XString GetKEYWORDStatementNVL(XString& p_test,XString& p_isnull) const = 0;

  // Gets the RDBMS definition of the datatype
  virtual XString GetKEYWORDDataType(MetaColumn* p_column) = 0;

  // Gets the USER (current-user) keyword function
  virtual XString GetKEYWORDCurrentUser() const = 0;

  //////////////////////////////////////////////////////////////////////////
  // SQL FOR SUB-PROCESSING

  // Connects to a default schema in the database/instance
  virtual XString GetSQLDefaultSchema(XString p_user,XString p_schema) const = 0;

  // Gets the construction for inline generating a key within an INSERT statement
  virtual XString GetSQLNewSerial(XString p_table,XString p_sequence) const = 0;

  // Gets the construction / select for generating a new serial identity
  virtual XString GetSQLGenerateSerial  (XString p_table)    const = 0;
  virtual XString GetSQLGenerateSequence(XString p_sequence) const = 0;

  // Gets the construction / select for the resulting effective generated serial
  virtual XString GetSQLEffectiveSerial(XString p_identity) const = 0;

  // Gets the sub-transaction commands
  virtual XString GetSQLStartSubTransaction   (XString p_savepointName) const = 0;
  virtual XString GetSQLCommitSubTransaction  (XString p_savepointName) const = 0;
  virtual XString GetSQLRollbackSubTransaction(XString p_savepointName) const = 0;

  // FROM-Part for a query to select only 1 (one) record
  virtual XString GetSQLFromDualClause() const = 0;

  // Get SQL to lock  a table 
  virtual XString GetSQLLockTable(XString p_schema,XString p_tablename,bool p_exclusive,int p_waittime) const = 0;

  // Get query to optimize the table statistics
  virtual XString GetSQLOptimizeTable(XString p_schema, XString p_tablename) const = 0;

  // Transform query to select top <n> rows
  virtual XString GetSQLTopNRows(XString p_sql,int p_top,int p_skip = 0) const = 0;

  // Expand a SELECT with an 'FOR UPDATE' lock clause
  virtual XString GetSelectForUpdateTableClause(unsigned p_lockWaitTime) const = 0;
  virtual XString GetSelectForUpdateTrailer(XString p_select,unsigned p_lockWaitTime) const = 0;

  // Query to perform a keep alive ping
  virtual XString GetPing() const = 0;

  //////////////////////////////////////////////////////////////////////////
  // SQL STRINGS

  // Makes a SQL string from a given string, with all the right quotes
  virtual XString GetSQLString(const XString& p_string) const = 0;

  // Get date string in engine format
  virtual XString GetSQLDateString(int p_year,int p_month,int p_day) const = 0;

  // Get time string in database engine format
  virtual XString GetSQLTimeString(int p_hour,int p_minute,int p_second) const = 0;

  // Get date-time string in database engine format
  virtual XString GetSQLDateTimeString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const = 0;

  // Get date-time bound parameter string in database format
  virtual XString GetSQLDateTimeBoundString() const = 0;

  // Stripped data for the parameter binding
  virtual XString GetSQLDateTimeStrippedString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const = 0;

  // Makes an catalog identifier string (possibly quoted on both sides)
  virtual XString GetSQLDDLIdentifier(XString p_identifier) const = 0;

  // Get the name of a temp table (local temporary or global temporary)
  virtual XString GetTempTablename(XString p_schema,XString p_tablename,bool p_local) const = 0;

  //////////////////////////////////////////////////////////////////////////
  //
  //  SQLQuery service functions for tinkering with bindings and buffers
  //

  // Changes to parameters before binding to an ODBC HSTMT handle
  virtual void DoBindParameterFixup(SQLSMALLINT& p_dataType,SQLSMALLINT& p_sqlDatatype,SQLULEN& p_columnSize,SQLSMALLINT& p_scale,SQLLEN& p_bufferSize,SQLLEN* p_indicator) const = 0;

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
  //   - Sequence
  //   - TemporaryTable 
  //   - View
  //   - Privileges
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

  // Meta info about meta types
  virtual XString GetCATALOGMetaTypes(int p_type) const = 0;
  virtual XString GetCATALOGDefaultCharset() const = 0;
  virtual XString GetCATALOGDefaultCharsetNCV() const = 0;
  virtual XString GetCATALOGDefaultCollation() const = 0;
  // All table functions
  virtual XString GetCATALOGTableExists       (XString& p_schema,XString& p_tablename) const = 0;
  virtual XString GetCATALOGTablesList        (XString& p_schema,XString& p_pattern)   const = 0;
  virtual XString GetCATALOGTableAttributes   (XString& p_schema,XString& p_tablename) const = 0;
  virtual XString GetCATALOGTableSynonyms     (XString& p_schema,XString& p_tablename) const = 0;
  virtual XString GetCATALOGTableCatalog      (XString& p_schema,XString& p_tablename) const = 0;
  virtual XString GetCATALOGTableCreate       (MetaTable& p_table,MetaColumn& p_column) const = 0;
  virtual XString GetCATALOGTableCreatePostfix(MetaTable& p_table,MetaColumn& p_column) const = 0;
  virtual XString GetCATALOGTableRename       (XString  p_schema,XString  p_tablename,XString p_newname) const = 0;
  virtual XString GetCATALOGTableDrop         (XString  p_schema,XString  p_tablename,bool p_ifExist = false,bool p_restrict = false,bool p_cascade = false) const = 0;
  // All Temporary table functions
  virtual XString GetCATALOGTemptableCreate   (XString p_schema,XString p_tablename,XString p_select) const = 0;
  virtual XString GetCATALOGTemptableIntoTemp (XString p_schema,XString p_tablename,XString p_select) const = 0;
  virtual XString GetCATALOGTemptableDrop     (XString p_schema,XString p_tablename) const = 0;
  // All column functions
  virtual XString GetCATALOGColumnExists      (XString  p_schema,XString  p_tablename,XString  p_columnname) const = 0;
  virtual XString GetCATALOGColumnList        (XString& p_schema,XString& p_tablename) const = 0;
  virtual XString GetCATALOGColumnAttributes  (XString& p_schema,XString& p_tablename,XString& p_columnname) const = 0;
  virtual XString GetCATALOGColumnCreate      (MetaColumn& p_column) const = 0;
  virtual XString GetCATALOGColumnAlter       (MetaColumn& p_column) const = 0;
  virtual XString GetCATALOGColumnRename      (XString  p_schema,XString  p_tablename,XString  p_columnname,XString p_newname,XString p_datatype) const = 0;
  virtual XString GetCATALOGColumnDrop        (XString  p_schema,XString  p_tablename,XString  p_columnname) const = 0;
  // All index functions
  virtual XString GetCATALOGIndexExists       (XString  p_schema,XString  p_tablename,XString  p_indexname) const = 0;
  virtual XString GetCATALOGIndexList         (XString& p_schema,XString& p_tablename)                      const = 0;
  virtual XString GetCATALOGIndexAttributes   (XString& p_schema,XString& p_tablename,XString& p_indexname) const = 0;
  virtual XString GetCATALOGIndexCreate       (MIndicesMap& p_index,bool p_duplicateNulls = false) const = 0;
  virtual XString GetCATALOGIndexDrop         (XString  p_schema,XString  p_tablename,XString  p_indexname) const = 0;
  virtual XString GetCATALOGIndexFilter       (MetaIndex& p_index) const = 0;
  // All primary key functions
  virtual XString GetCATALOGPrimaryExists     (XString  p_schema,XString  p_tablename) const = 0;
  virtual XString GetCATALOGPrimaryAttributes (XString& p_schema,XString& p_tablename) const = 0;
  virtual XString GetCATALOGPrimaryCreate     (MPrimaryMap& p_primaries) const = 0;
  virtual XString GetCATALOGPrimaryDrop       (XString  p_schema,XString  p_tablename,XString p_constraintname) const = 0;
  // All foreign key functions
  virtual XString GetCATALOGForeignExists     (XString  p_schema,XString  p_tablename,XString  p_constraintname) const = 0;
  virtual XString GetCATALOGForeignList       (XString& p_schema,XString& p_tablename,int p_maxColumns = SQLINFO_MAX_COLUMNS) const = 0;
  virtual XString GetCATALOGForeignAttributes (XString& p_schema,XString& p_tablename,XString& p_constraintname,bool p_referenced = false,int p_maxColumns = SQLINFO_MAX_COLUMNS) const = 0;
  virtual XString GetCATALOGForeignCreate     (MForeignMap& p_foreigns) const = 0;
  virtual XString GetCATALOGForeignAlter      (MForeignMap& p_original,MForeignMap& p_requested) const = 0;
  virtual XString GetCATALOGForeignDrop       (XString  p_schema,XString  p_tablename,XString  p_constraintname) const = 0;
  // All default constraints
  virtual XString GetCATALOGDefaultExists     (XString& p_schema,XString& p_tablename,XString& p_column) const = 0;
  virtual XString GetCATALOGDefaultList       (XString& p_schema,XString& p_tablename) const = 0;
  virtual XString GetCATALOGDefaultAttributes (XString& p_schema,XString& p_tablename,XString& p_column) const = 0;
  virtual XString GetCATALOGDefaultCreate     (XString  p_schema,XString  p_tablename,XString  p_constraint,XString p_column,XString p_code) const = 0;
  virtual XString GetCATALOGDefaultDrop       (XString  p_schema,XString  p_tablename,XString  p_constraint) const = 0;
  // All check constraints
  virtual XString GetCATALOGCheckExists       (XString  p_schema,XString  p_tablename,XString  p_constraint) const = 0;
  virtual XString GetCATALOGCheckList         (XString  p_schema,XString  p_tablename) const = 0;
  virtual XString GetCATALOGCheckAttributes   (XString  p_schema,XString  p_tablename,XString  p_constraint) const = 0;
  virtual XString GetCATALOGCheckCreate       (XString  p_schema,XString  p_tablename,XString  p_constraint,XString p_condition) const = 0;
  virtual XString GetCATALOGCheckDrop         (XString  p_schema,XString  p_tablename,XString  p_constraint) const = 0;
  // All trigger functions
  virtual XString GetCATALOGTriggerExists     (XString  p_schema,XString  p_tablename,XString  p_triggername) const = 0;
  virtual XString GetCATALOGTriggerList       (XString& p_schema,XString& p_tablename) const = 0;
  virtual XString GetCATALOGTriggerAttributes (XString& p_schema,XString& p_tablename,XString& p_triggername) const = 0;
  virtual XString GetCATALOGTriggerCreate     (MetaTrigger& p_trigger) const = 0;
  virtual XString GetCATALOGTriggerDrop       (XString  p_schema,XString  p_tablename,XString  p_triggername) const = 0;
  // All sequence functions
  virtual XString GetCATALOGSequenceExists    (XString  p_schema,XString  p_sequence) const = 0;
  virtual XString GetCATALOGSequenceList      (XString& p_schema,XString& p_pattern)  const = 0;
  virtual XString GetCATALOGSequenceAttributes(XString& p_schema,XString& p_sequence) const = 0;
  virtual XString GetCATALOGSequenceCreate    (MetaSequence& p_sequence) const = 0;
  virtual XString GetCATALOGSequenceDrop      (XString  p_schema,XString  p_sequence) const = 0;
  // All view functions
  virtual XString GetCATALOGViewExists        (XString& p_schema,XString& p_viewname) const = 0;
  virtual XString GetCATALOGViewList          (XString& p_schema,XString& p_pattern)  const = 0;
  virtual XString GetCATALOGViewAttributes    (XString& p_schema,XString& p_viewname) const = 0;
  virtual XString GetCATALOGViewText          (XString& p_schema,XString& p_viewname) const = 0;
  virtual XString GetCATALOGViewCreate        (XString  p_schema,XString  p_viewname,XString p_contents,bool p_ifexists = true)   const = 0;
  virtual XString GetCATALOGViewRename        (XString  p_schema,XString  p_viewname,XString p_newname)    const = 0;
  virtual XString GetCATALOGViewDrop          (XString  p_schema,XString  p_viewname,XString& p_precursor) const = 0;
  // All Privilege functions
  virtual XString GetCATALOGTablePrivileges   (XString& p_schema,XString& p_tablename) const = 0;
  virtual XString GetCATALOGColumnPrivileges  (XString& p_schema,XString& p_tablename,XString& p_columnname) const = 0;
  virtual XString GetCATALOGSequencePrivilege (XString& p_schema,XString& p_sequence) const = 0;
  virtual XString GetCATALOGGrantPrivilege    (XString  p_schema,XString  p_objectname,XString p_privilege,XString p_grantee,bool p_grantable) = 0;
  virtual XString GetCATALOGRevokePrivilege   (XString  p_schema,XString  p_objectname,XString p_privilege,XString p_grantee) = 0;
  // All Synonym functions
  virtual XString GetCATALOGSynonymList       (XString& p_schema,XString& p_pattern) const = 0;
  virtual XString GetCATALOGSynonymAttributes (XString& p_schema,XString& p_synonym) const = 0;
  virtual XString GetCATALOGSynonymCreate     (XString& p_schema,XString& p_synonym,XString p_forObject,bool p_private = true) const = 0;
  virtual XString GetCATALOGSynonymDrop       (XString& p_schema,XString& p_synonym,bool p_private = true) const = 0;

  //////////////////////////////////////////////////////////////////////////
  //
  // PSM = PERSISTENT STORED MODULES (Also called SPL, PL/SQL, Transact-SQL)
  //
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
  virtual XString GetPSMProcedureExists    (XString  p_schema,XString  p_procedure) const = 0;
  virtual XString GetPSMProcedureList      (XString& p_schema) const = 0;
  virtual XString GetPSMProcedureAttributes(XString& p_schema,XString& p_procedure) const = 0;
  virtual XString GetPSMProcedureSourcecode(XString  p_schema,XString  p_procedure) const = 0;
  virtual XString GetPSMProcedureCreate    (MetaProcedure& p_procedure) const = 0;
  virtual XString GetPSMProcedureDrop      (XString  p_schema,XString  p_procedure,bool p_function = false) const = 0;
  virtual XString GetPSMProcedureErrors    (XString  p_schema,XString  p_procedure) const = 0;
  virtual XString GetPSMProcedurePrivilege (XString& p_schema,XString& p_procedure) const = 0;
  // And it's parameters
  virtual XString GetPSMProcedureParameters(XString& p_schema,XString& p_procedure) const = 0;

  // All Language elements
  virtual XString GetPSMDeclaration(bool p_first,XString p_variable,int p_datatype,int p_precision = 0,int p_scale = 0,
                                    XString p_default = _T(""),XString p_domain = _T(""),XString p_asColumn = _T("")) const = 0;
  virtual XString GetPSMAssignment (XString p_variable,XString p_statement = _T("")) const = 0;
  virtual XString GetPSMIF         (XString p_condition) const = 0;
  virtual XString GetPSMIFElse     () const = 0;
  virtual XString GetPSMIFEnd      () const = 0;
  virtual XString GetPSMWhile      (XString p_condition) const = 0;
  virtual XString GetPSMWhileEnd   () const = 0;
  virtual XString GetPSMLOOP       () const = 0;
  virtual XString GetPSMLOOPEnd    () const = 0;
  virtual XString GetPSMBREAK      () const = 0;
  virtual XString GetPSMRETURN     (XString p_statement = _T("")) const = 0;
  virtual XString GetPSMExecute    (XString p_procedure,MParameterMap& p_parameters) const = 0;

  // The CURSOR
  virtual XString GetPSMCursorDeclaration(XString p_cursorname,XString p_select) const = 0;
  virtual XString GetPSMCursorFetch      (XString p_cursorname,std::vector<XString>& p_columnnames,std::vector<XString>& p_variablenames) const = 0;

  // PSM Exceptions
  virtual XString GetPSMExceptionCatchNoData() const = 0;
  virtual XString GetPSMExceptionCatch      (XString p_sqlState) const = 0;
  virtual XString GetPSMExceptionRaise      (XString p_sqlState) const = 0;

  //////////////////////////////////////////////////////////////////////////
  //
  // SESSIONS
  // - Sessions (No create and drop)
  //   - GetSessionMyself
  //   - GetSessionExists
  //   - GetSessionList
  //   - GetSessionAttributes
  // - Transactions
  //   - GetSessionConstraintsDeferred
  //   - GetSessionConstraintsImmediate
  //
  //////////////////////////////////////////////////////////////////////////

  virtual XString GetSESSIONMyself() const = 0;
  virtual XString GetSESSIONExists(XString p_sessionID) const = 0;
  virtual XString GetSESSIONList() const = 0;
  virtual XString GetSESSIONAttributes(XString p_sessionID) const = 0;
  // Transactions
  virtual XString GetSESSIONConstraintsDeferred()  const = 0;
  virtual XString GetSESSIONConstraintsImmediate() const = 0;

  //////////////////////////////////////////////////////////////////////////
  //
  // Call FUNCTION/PROCEDURE from within program
  // As a RDBMS dependent extension of "DoSQLCall" of the SQLQuery object
  //
  //////////////////////////////////////////////////////////////////////////

  // Calling a stored function or procedure if the RDBMS does not support ODBC call escapes
  virtual SQLVariant* DoSQLCall(SQLQuery* p_query,XString& p_schema,XString& p_procedure) = 0;
  // Calling a stored function with named parameters, returning a value
  virtual SQLVariant* DoSQLCallNamedParameters(SQLQuery* p_query,XString& p_schema,XString& p_procedure,bool p_function = true) = 0;
  
private:
  // Read a tables cursor from the database
  bool    ReadMetaTypesFromQuery(SQLQuery& p_query,MMetaMap&  p_objects,int p_type);
  bool    ReadTablesFromQuery   (SQLQuery& p_query,MTableMap& p_tables);

  // All default granted users for GRANT statements
  XString m_grantedUsers;
  // Catalog default charset/collation
  XString m_defaultCharset;
  XString m_defaultCharsetNCV;
  XString m_defaultCollation;
  // Prefer ODBC meta-queries above hand crafted ones
  bool    m_preferODBC { true };
};

inline void    
SQLInfoDB::SetGrantedUsers(XString p_users) 
{
  m_grantedUsers = p_users;
}

inline XString 
SQLInfoDB::GetGrantedUsers() const
{
  return m_grantedUsers;
}

inline void
SQLInfoDB::SetPreferODBC(bool p_prefer)
{
  m_preferODBC = p_prefer;
}

inline bool
SQLInfoDB::GetPreferODBC()
{
  return m_preferODBC;
}

// End of namespace
}
