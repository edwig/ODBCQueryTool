////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoDB.h
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
  void    SetGrantedUsers(CString p_users);
  CString GetGrantedUsers() const;
  // We prefer the strict 'standard' ODBC drivers meta queries
  void    SetPreferODBC(bool p_prefer);
  bool    GetPreferODBC();

  // OVERRIDES AND EXTRAS OF THE ODBC MakeInfo<object> functions

  // Meta info about meta types: META_CATALOGS/META_SCHEMAS/META_TABLES
  virtual bool    MakeInfoMetaTypes       (MMetaMap&      p_objects,   CString& p_errors,int p_type) override;
  // Tables
  virtual bool    MakeInfoTableObject     (MTableMap&     p_tables,    CString& p_errors,CString p_schema,CString p_tablename);  // Not known which type!
  virtual bool    MakeInfoTableTable      (MTableMap&     p_tables,    CString& p_errors,CString p_schema,CString p_tablename);  // TABLE   only
  virtual bool    MakeInfoTableView       (MTableMap&     p_tables,    CString& p_errors,CString p_schema,CString p_tablename);  // VIEW    only
  virtual bool    MakeInfoTableSynonyms   (MTableMap&     p_tables,    CString& p_errors,CString p_schema,CString p_tablename);  // SYNONYM only
  virtual bool    MakeInfoTableCatalog    (MTableMap&     p_tables,    CString& p_errors,CString p_schema,CString p_tablename);  // CATALOG only
  // Attributes of a table
  virtual bool    MakeInfoTableColumns    (MColumnMap&    p_columns,   CString& p_errors,CString p_schema,CString p_tablename,CString p_columnname = "") override;
  virtual bool    MakeInfoTablePrimary    (MPrimaryMap&   p_primary,   CString& p_errors,CString p_schema,CString p_tablename)                          override;
  virtual bool    MakeInfoTableForeign    (MForeignMap&   p_foreigns,  CString& p_errors,CString p_schema,CString p_tablename,bool p_referenced   = false) override;
  virtual bool    MakeInfoTableStatistics (MIndicesMap&   p_indices,   CString& p_errors,CString p_schema,CString p_tablename,MPrimaryMap* p_keymap,bool p_all = true) override;
  virtual bool    MakeInfoTableTriggers   (MTriggerMap&   p_triggers,  CString& p_errors,CString p_schema,CString p_tablename,CString p_trigger   = "");
  virtual bool    MakeInfoTableSequences  (MSequenceMap&  p_sequences, CString& p_errors,CString p_schema,CString p_tablename);
  virtual bool    MakeInfoTablePrivileges (MPrivilegeMap& p_privileges,CString& p_errors,CString p_schema,CString p_tablename);
  virtual bool    MakeInfoColumnPrivileges(MPrivilegeMap& p_privileges,CString& p_errors,CString p_schema,CString p_tablename,CString p_columnname = "");
  virtual bool    MakeInfoViewDefinition  (CString&       p_defintion, CString& p_errors,CString p_schema,CString p_viewname);

  // Procedures
  virtual bool    MakeInfoPSMProcedures(MProcedureMap& p_procedures,CString& p_errors,CString p_schema,CString p_procedure) override;
  virtual bool    MakeInfoPSMParameters(MParameterMap& p_parameters,CString& p_errors,CString p_schema,CString p_procedure) override;
  // Read extra source code for database that can only do it by an extra procedure
  virtual CString MakeInfoPSMSourcecode(CString p_schema, CString p_procedure);

  // PURE VIRTUAL INTERFACE

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
  virtual CString GetRDBMSVendorName() const = 0;

  // Get the physical database name
  virtual CString GetRDBMSPhysicalDatabaseName() const = 0;

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

  //////////////////////////////////////////////////////////////////////////
  // KEYWORDS

  // Keyword for the current date and time
  virtual CString GetKEYWORDCurrentTimestamp() const = 0;

  // String for the current date
  virtual CString GetKEYWORDCurrentDate() const = 0;

  // Get the concatenation operator
  virtual CString GetKEYWORDConcatanationOperator() const = 0;

  // Get quote character for strings
  virtual CString GetKEYWORDQuoteCharacter() const = 0;

  // Get quote character around reserved words as an identifier
  virtual CString GetKEYWORDReservedWordQuote() const = 0;

  // Get default NULL for parameter list input
  virtual CString GetKEYWORDParameterDefaultNULL() const = 0;

  // Parameter is for INPUT and OUTPUT in parameter list
  virtual CString GetKEYWORDParameterINOUT() const = 0;

  // Parameter is for OUTPUT only in parameter list
  virtual CString GetKEYWORDParameterOUT() const = 0;

  // Get datatype of the IDENTITY primary key in a Network database
  virtual CString GetKEYWORDNetworkPrimaryKeyType() const = 0;

  // Get datatype for Moment
  virtual CString GetKEYWORDTypeTimestamp() const = 0;

  // Prefix for a parameter in a stored procedure
  virtual CString  GetKEYWORDParameterPrefix() const = 0;

  // Get select part to add new record identity to a table
  // Can be special column like 'OID' or a sequence select
  virtual CString GetKEYWORDIdentityString(CString& p_tablename,CString p_postfix = "_seq") const = 0;

  // Gets the UPPER function
  virtual CString GetKEYWORDUpper(CString& p_expression) const = 0;

  // Gets the construction for 1 minute ago
  virtual CString GetKEYWORDInterval1MinuteAgo() const = 0;

  // Gets the Not-NULL-Value statement of the database
  virtual CString GetKEYWORDStatementNVL(CString& p_test,CString& p_isnull) const = 0;

  // Gets the RDBMS definition of the datatype
  virtual CString GetKEYWORDDataType(MetaColumn* p_column) = 0;

  // Gets the USER (current-user) keyword function
  virtual CString GetKEYWORDCurrentUser() const = 0;

  //////////////////////////////////////////////////////////////////////////
  // SQL FOR SUB-PROCESSING

  // Connects to a default schema in the database/instance
  virtual CString GetSQLDefaultSchema(CString p_schema) const = 0;

  // Gets the construction for inline generating a key within an INSERT statement
  virtual CString GetSQLNewSerial(CString p_table,CString p_sequence) const = 0;

  // Gets the construction / select for generating a new serial identity
  virtual CString GetSQLGenerateSerial(CString p_table) const = 0;

  // Gets the construction / select for the resulting effective generated serial
  virtual CString GetSQLEffectiveSerial(CString p_identity) const = 0;

  // Gets the sub-transaction commands
  virtual CString GetSQLStartSubTransaction   (CString p_savepointName) const = 0;
  virtual CString GetSQLCommitSubTransaction  (CString p_savepointName) const = 0;
  virtual CString GetSQLRollbackSubTransaction(CString p_savepointName) const = 0;

  // FROM-Part for a query to select only 1 (one) record
  virtual CString GetSQLFromDualClause() const = 0;

  // Get SQL to lock  a table 
  virtual CString GetSQLLockTable(CString p_schema,CString p_tablename, bool p_exclusive) const = 0;

  // Get query to optimize the table statistics
  virtual CString GetSQLOptimizeTable(CString p_schema, CString p_tablename) const = 0;

  // Transform query to select top <n> rows
  virtual CString GetSQLTopNRows(CString p_sql,int p_top,int p_skip = 0) const = 0;

  //////////////////////////////////////////////////////////////////////////
  // SQL STRINGS

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
  virtual CString GetCATALOGMetaTypes(int p_type) const = 0;
  // All table functions
  virtual CString GetCATALOGTableExists       (CString& p_schema,CString& p_tablename) const = 0;
  virtual CString GetCATALOGTablesList        (CString& p_schema,CString& p_pattern)   const = 0;
  virtual CString GetCATALOGTableAttributes   (CString& p_schema,CString& p_tablename) const = 0;
  virtual CString GetCATALOGTableSynonyms     (CString& p_schema,CString& p_tablename) const = 0;
  virtual CString GetCATALOGTableCatalog      (CString& p_schema,CString& p_tablename) const = 0;
  virtual CString GetCATALOGTableCreate       (MetaTable& p_table,MetaColumn& p_column) const = 0;
  virtual CString GetCATALOGTableCreatePostfix(MetaTable& p_table,MetaColumn& p_column) const = 0;
  virtual CString GetCATALOGTableRename       (CString  p_schema,CString  p_tablename,CString p_newname) const = 0;
  virtual CString GetCATALOGTableDrop         (CString  p_schema,CString  p_tablename,bool p_ifExist = false,bool p_restrict = false,bool p_cascade = false) const = 0;
  // All Temporary table functions
  virtual CString GetCATALOGTemptableCreate   (CString p_schema,CString p_tablename,CString p_select) const = 0;
  virtual CString GetCATALOGTemptableIntoTemp (CString p_schema,CString p_tablename,CString p_select) const = 0;
  virtual CString GetCATALOGTemptableDrop     (CString p_schema,CString p_tablename) const = 0;
  // All column functions
  virtual CString GetCATALOGColumnExists      (CString  p_schema,CString  p_tablename,CString  p_columnname) const = 0;
  virtual CString GetCATALOGColumnList        (CString& p_schema,CString& p_tablename) const = 0;
  virtual CString GetCATALOGColumnAttributes  (CString& p_schema,CString& p_tablename,CString& p_columnname) const = 0;
  virtual CString GetCATALOGColumnCreate      (MetaColumn& p_column) const = 0;
  virtual CString GetCATALOGColumnAlter       (MetaColumn& p_column) const = 0;
  virtual CString GetCATALOGColumnRename      (CString  p_schema,CString  p_tablename,CString  p_columnname,CString p_newname,CString p_datatype) const = 0;
  virtual CString GetCATALOGColumnDrop        (CString  p_schema,CString  p_tablename,CString  p_columnname) const = 0;
  // All index functions
  virtual CString GetCATALOGIndexExists       (CString  p_schema,CString  p_tablename,CString  p_indexname) const = 0;
  virtual CString GetCATALOGIndexList         (CString& p_schema,CString& p_tablename)                      const = 0;
  virtual CString GetCATALOGIndexAttributes   (CString& p_schema,CString& p_tablename,CString& p_indexname) const = 0;
  virtual CString GetCATALOGIndexCreate       (MIndicesMap& p_index) const = 0;
  virtual CString GetCATALOGIndexDrop         (CString  p_schema,CString  p_tablename,CString  p_indexname) const = 0;
  virtual CString GetCATALOGIndexFilter       (MetaIndex& p_index) const = 0;
  // All primary key functions
  virtual CString GetCATALOGPrimaryExists     (CString  p_schema,CString  p_tablename) const = 0;
  virtual CString GetCATALOGPrimaryAttributes (CString& p_schema,CString& p_tablename) const = 0;
  virtual CString GetCATALOGPrimaryCreate     (MPrimaryMap& p_primaries) const = 0;
  virtual CString GetCATALOGPrimaryDrop       (CString  p_schema,CString  p_tablename,CString p_constraintname) const = 0;
  // All foreign key functions
  virtual CString GetCATALOGForeignExists     (CString  p_schema,CString  p_tablename,CString  p_constraintname) const = 0;
  virtual CString GetCATALOGForeignList       (CString& p_schema,CString& p_tablename,int p_maxColumns = SQLINFO_MAX_COLUMNS) const = 0;
  virtual CString GetCATALOGForeignAttributes (CString& p_schema,CString& p_tablename,CString& p_constraintname,bool p_referenced = false,int p_maxColumns = SQLINFO_MAX_COLUMNS) const = 0;
  virtual CString GetCATALOGForeignCreate     (MForeignMap& p_foreigns) const = 0;
  virtual CString GetCATALOGForeignAlter      (MForeignMap& p_original,MForeignMap& p_requested) const = 0;
  virtual CString GetCATALOGForeignDrop       (CString  p_schema,CString  p_tablename,CString  p_constraintname) const = 0;
  // All trigger functions
  virtual CString GetCATALOGTriggerExists     (CString  p_schema,CString  p_tablename,CString  p_triggername) const = 0;
  virtual CString GetCATALOGTriggerList       (CString& p_schema,CString& p_tablename) const = 0;
  virtual CString GetCATALOGTriggerAttributes (CString& p_schema,CString& p_tablename,CString& p_triggername) const = 0;
  virtual CString GetCATALOGTriggerCreate     (MetaTrigger& p_trigger) const = 0;
  virtual CString GetCATALOGTriggerDrop       (CString  p_schema,CString  p_tablename,CString  p_triggername) const = 0;
  // All sequence functions
  virtual CString GetCATALOGSequenceExists    (CString  p_schema,CString  p_sequence) const = 0;
  virtual CString GetCATALOGSequenceList      (CString& p_schema,CString& p_pattern)  const = 0;
  virtual CString GetCATALOGSequenceAttributes(CString& p_schema,CString& p_sequence) const = 0;
  virtual CString GetCATALOGSequenceCreate    (MetaSequence& p_sequence) const = 0;
  virtual CString GetCATALOGSequenceDrop      (CString  p_schema,CString  p_sequence) const = 0;
  // All view functions
  virtual CString GetCATALOGViewExists        (CString& p_schema,CString& p_viewname) const = 0;
  virtual CString GetCATALOGViewList          (CString& p_schema,CString& p_pattern)  const = 0;
  virtual CString GetCATALOGViewAttributes    (CString& p_schema,CString& p_viewname) const = 0;
  virtual CString GetCATALOGViewText          (CString& p_schema,CString& p_viewname) const = 0;
  virtual CString GetCATALOGViewCreate        (CString  p_schema,CString  p_viewname,CString p_contents)   const = 0;
  virtual CString GetCATALOGViewRename        (CString  p_schema,CString  p_viewname,CString p_newname)    const = 0;
  virtual CString GetCATALOGViewDrop          (CString  p_schema,CString  p_viewname,CString& p_precursor) const = 0;
  // All Privilege functions
  virtual CString GetCATALOGTablePrivileges   (CString& p_schema,CString& p_tablename) const = 0;
  virtual CString GetCATALOGColumnPrivileges  (CString& p_schema,CString& p_tablename,CString& p_columnname) const = 0;
  virtual CString GetCatalogGrantPrivilege    (CString  p_schema,CString  p_objectname,CString p_privilege,CString p_grantee,bool p_grantable) = 0;
  virtual CString GetCatalogRevokePrivilege   (CString  p_schema,CString  p_objectname,CString p_privilege,CString p_grantee) = 0;

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
  virtual CString GetPSMProcedureExists    (CString  p_schema,CString  p_procedure) const = 0;
  virtual CString GetPSMProcedureList      (CString& p_schema) const = 0;
  virtual CString GetPSMProcedureAttributes(CString& p_schema,CString& p_procedure) const = 0;
  virtual CString GetPSMProcedureSourcecode(CString  p_schema,CString  p_procedure) const = 0;
  virtual CString GetPSMProcedureCreate    (MetaProcedure& p_procedure) const = 0;
  virtual CString GetPSMProcedureDrop      (CString  p_schema,CString  p_procedure) const = 0;
  virtual CString GetPSMProcedureErrors    (CString  p_schema,CString  p_procedure) const = 0;
  // And it's parameters
  virtual CString GetPSMProcedureParameters(CString& p_schema,CString& p_procedure) const = 0;

  // All Language elements
  virtual CString GetPSMDeclaration(bool p_first,CString p_variable,int p_datatype,int p_precision = 0,int p_scale = 0,
                                    CString p_default = "",CString p_domain = "",CString p_asColumn = "") const = 0;
  virtual CString GetPSMAssignment (CString p_variable,CString p_statement = "") const = 0;
  virtual CString GetPSMIF         (CString p_condition) const = 0;
  virtual CString GetPSMIFElse     () const = 0;
  virtual CString GetPSMIFEnd      () const = 0;
  virtual CString GetPSMWhile      (CString p_condition) const = 0;
  virtual CString GetPSMWhileEnd   () const = 0;
  virtual CString GetPSMLOOP       () const = 0;
  virtual CString GetPSMLOOPEnd    () const = 0;
  virtual CString GetPSMBREAK      () const = 0;
  virtual CString GetPSMRETURN     (CString p_statement = "") const = 0;
  virtual CString GetPSMExecute    (CString p_procedure,MParameterMap& p_parameters) const = 0;

  // The CURSOR
  virtual CString GetPSMCursorDeclaration(CString p_cursorname,CString p_select) const = 0;
  virtual CString GetPSMCursorFetch      (CString p_cursorname,std::vector<CString>& p_columnnames,std::vector<CString>& p_variablenames) const = 0;

  // PSM Exceptions
  virtual CString GetPSMExceptionCatchNoData() const = 0;
  virtual CString GetPSMExceptionCatch      (CString p_sqlState) const = 0;
  virtual CString GetPSMExceptionRaise      (CString p_sqlState) const = 0;

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

  virtual CString GetSESSIONMyself() const = 0;
  virtual CString GetSESSIONExists(CString p_sessionID) const = 0;
  virtual CString GetSESSIONList() const = 0;
  virtual CString GetSESSIONAttributes(CString p_sessionID) const = 0;
  // Transactions
  virtual CString GetSESSIONConstraintsDeferred()  const = 0;
  virtual CString GetSESSIONConstraintsImmediate() const = 0;

  //////////////////////////////////////////////////////////////////////////
  //
  // Call FUNCTION/PROCEDURE from within program
  // As a RDBMS dependent extension of "DoSQLCall" of the SQLQuery object
  //
  //////////////////////////////////////////////////////////////////////////

  // Calling a stored function or procedure if the RDBMS does not support ODBC call escapes
  virtual SQLVariant* DoSQLCall(SQLQuery* p_query,CString& p_schema,CString& p_procedure) = 0;
  // Calling a stored function with named parameters, returning a value
  virtual SQLVariant* DoSQLCallNamedParameters(SQLQuery* p_query,CString& p_schema,CString& p_procedure) = 0;
  
private:
  // Read a tables cursor from the database
  bool    ReadMetaTypesFromQuery(SQLQuery& p_query,MMetaMap&  p_objects,int p_type);
  bool    ReadTablesFromQuery   (SQLQuery& p_query,MTableMap& p_tables);

  // All default granted users for GRANT statements
  CString m_grantedUsers;
  // Prefer ODBC metaqueries above hand crafted ones
  bool    m_preferODBC { true };
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
