////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoAccess.cpp
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
#include "stdafx.h"
#include "SQLComponents.h"
#include "SQLInfoAccess.h"
#include "SQLQuery.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{

// Constructor.
SQLInfoAccess::SQLInfoAccess(SQLDatabase* p_database)
              :SQLInfoDB(p_database)
{
}

// Destructor. does nothing
SQLInfoAccess::~SQLInfoAccess()
{
}

//////////////////////////////////////////////////////////////////////////
//
// GENERALS (Strings & Booleans) 
//
//////////////////////////////////////////////////////////////////////////

// Get the database type
DatabaseType 
SQLInfoAccess::GetRDBMSDatabaseType() const
{
  return RDBMS_SQLSERVER;
}

// The name of the database vendor
CString 
SQLInfoAccess::GetRDBMSVendorName() const
{
  // The name of the database vendor
  return "Microsoft";
}

// Get the physical database name
CString 
SQLInfoAccess::GetRDBMSPhysicalDatabaseName() const
{
  return m_database->GetDatabaseName();
}

// System catalog is stored in uppercase in the database?
bool
SQLInfoAccess::GetRDBMSIsCatalogUpper() const
{
  return false;
}

// System catalog supports full ISO schemas (same tables per schema)
bool
SQLInfoAccess::GetRDBMSUnderstandsSchemas() const
{
  return false;
}

// Supports database/ODBCdriver comments in SQL
bool
SQLInfoAccess::GetRDBMSSupportsComments() const
{
  return false;
}

// Database can defer constraints until the end of a transaction
bool
SQLInfoAccess::GetRDBMSSupportsDeferredConstraints() const
{
  // SET CONSTRAINTS DEFERRED is supported
  return true;
}

// Database has ORDER BY with an expression, e.g. ORDER BY UPPER(columnname)
bool
SQLInfoAccess::GetRDBMSSupportsOrderByExpression() const
{
  return true;
}

// Supports the ODBC escape sequence {[?=] CALL procedure (?,?,?)}
bool
SQLInfoAccess::GetRDBMSSupportsODBCCallEscapes() const
{
  return true;
}

// If the database does not support the datatype TIME, it can be implemented as a DECIMAL
bool
SQLInfoAccess::GetRDBMSSupportsDatatypeTime() const
{
  // Time can be implemented as TIME
  return true;
}

// If the database does not support the datatype INTERVAL, it can be implemented as a DECIMAL
bool
SQLInfoAccess::GetRDBMSSupportsDatatypeInterval() const
{
  // Interval not supported, can be implemented as DECIMALS
  return false;
}

// Gets the maximum length of an SQL statement
unsigned long 
SQLInfoAccess::GetRDBMSMaxStatementLength() const
{
  // No limit
  return 0; 
}

// KEYWORDS

// Keyword for the current date and time
CString 
SQLInfoAccess::GetKEYWORDCurrentTimestamp() const
{
  return "GETDATE()";
}

// String for the current date
CString 
SQLInfoAccess::GetKEYWORDCurrentDate() const
{
  return "GETDATE()";
}

// Get the concatenation operator
CString 
SQLInfoAccess::GetKEYWORDConcatanationOperator() const
{
  return "+";
}

// Get quote character for strings
CString 
SQLInfoAccess::GetKEYWORDQuoteCharacter() const
{
  return "\'";
}

// Get default NULL for parameter list input
CString 
SQLInfoAccess::GetKEYWORDParameterDefaultNULL() const
{
  return "= NULL";
}

// Parameter is for INPUT and OUTPUT in parameter list
CString 
SQLInfoAccess::GetKEYWORDParameterINOUT() const
{
  return "OUTPUT";
}

// Parameter is for OUTPUT only in parameter list
CString 
SQLInfoAccess::GetKEYWORDParameterOUT() const
{
  return "OUTPUT";
}

// Get datatype of the IDENTITY primary key in a Network database
CString 
SQLInfoAccess::GetKEYWORDNetworkPrimaryKeyType() const
{
  return "INTEGER IDENTITY(1,1)";
}

// Get datatype for timestamp (year to second)
CString 
SQLInfoAccess::GetKEYWORDTypeTimestamp() const
{
  return "DATETIME";
}

// Prefix for a parameter in a stored procedure
CString 
SQLInfoAccess::GetKEYWORDParameterPrefix() const
{
  return "@";
}

// Get select part to add new record identity to a table
// Can be special column like 'OID' or a sequence select
CString 
SQLInfoAccess::GetKEYWORDIdentityString(CString& p_tablename,CString /*p_postfix*/ /*= "_seq"*/) const
{
  return "IDENT_CURRENT('" + p_tablename + "') + " + "IDENT_INCR('" + p_tablename + "')";
}

// Gets the UPPER function
CString 
SQLInfoAccess::GetKEYWORDUpper(CString& p_expression) const
{
  return "{fn UCASE(" + p_expression + ")}";
}

// Gets the construction for 1 minute ago
CString 
SQLInfoAccess::GetKEYWORDInterval1MinuteAgo() const
{
  // Not supported by MS-Access
  return "ERROR";
}

// Gets the Not-NULL-Value statement of the database
CString 
SQLInfoAccess::GetKEYWORDStatementNVL(CString& p_test,CString& p_isnull) const
{
  return CString("IIF(ISNULL(") + p_test + ")," + p_isnull + "," + p_test + ")";
}

// Gets the construction / select for generating a new serial identity
CString
SQLInfoAccess::GetSQLGenerateSerial(CString p_table) const
{
  // Is generated by the IDENTITY system
  return "";
}

// Gets the construction / select for the resulting effective generated serial
CString
SQLInfoAccess::GetSQLEffectiveSerial(CString p_identity) const
{
  return "SELECT @@IDENTITY";
}

// Gets the sub-transaction commands
CString
SQLInfoAccess::GetSQLStartSubTransaction(CString p_savepointName) const
{
  // Does not know how to do sub-transactions
  return "";
}

CString
SQLInfoAccess::GetSQLCommitSubTransaction(CString p_savepointName) const
{
  // Does not know how to do sub-transactions
  return "";
}

CString
SQLInfoAccess::GetSQLRollbackSubTransaction(CString p_savepointName) const
{
  // Does not know how to do sub-transactions
  return "";
}

// FROM-Part for a query to select only 1 (one) record / or empty!
CString 
SQLInfoAccess::GetSQLFromDualClause() const
{
  return "";
}

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
//   - Alter  (where possible)
//   - Rename (where possible)
//   - Drop
//
//////////////////////////////////////////////////////////////////////////

// Get SQL to check if a table already exists in the database
CString
SQLInfoAccess::GetCATALOGTableExists(CString /*p_schema*/,CString /*p_tablename*/) const
{
  // MS-Access cannot do this
  return "";
}

CString 
SQLInfoAccess::GetCATALOGTablesList(CString /*p_schema*/,CString /*p_pattern*/) const
{
  // MS-Access cannot do this
  return "";
}

bool 
SQLInfoAccess::GetCATALOGTableAttributes(CString /*p_schema*/,CString /*p_tablename*/,MetaTable& /*p_table*/) const
{
  // MS-Access cannot do this
  return false;
}

CString 
SQLInfoAccess::GetCATALOGTableCreate(MetaTable& /*p_table*/,MetaColumn& /*p_column*/) const
{
  return "";
}

// Rename a database table 
CString
SQLInfoAccess::GetCATALOGTableRename(CString /*p_schema*/,CString p_tablename,CString p_newname) const
{
  // Beware: No 'TABLE' in the statement
  CString sql("RENAME " + p_tablename + " to " + p_newname);
  return sql;
}

CString 
SQLInfoAccess::GetCATALOGTableDrop(CString /*p_schema*/,CString p_tablename) const
{
  return "DROP TABLE " + p_tablename;
}

//////////////////////////////////////////////////////////////////////////
// ALL TEMPORARY TABLE FUNCTIONS

CString 
SQLInfoAccess::GetCATALOGTemptableCreate(CString /*p_schema*/,CString p_tablename,CString p_select) const
{
  return "CREATE TABLE #" + p_tablename + "\nAS " + p_select;
}

CString 
SQLInfoAccess::GetCATALOGTemptableIntoTemp(CString /*p_schema*/,CString p_tablename,CString p_select) const
{
  return "INSERT INTO #" + p_tablename + "\n" + p_select;
}
CString 
SQLInfoAccess::GetCATALOGTemptableDrop(CString /*p_schema*/,CString p_tablename) const
{
  return "DELETE FROM #"    + p_tablename + "\n"
         "<@>\n"
         "DROP TABLE #"     + p_tablename;
}

//////////////////////////////////////////////////////////////////////////
// ALL COLUMN FUNCTIONS

CString 
SQLInfoAccess::GetCATALOGColumnExists(CString /*p_schema*/,CString /*p_tablename*/,CString /*p_columname*/) const
{
  // MS-Access cannot do this
  return "";
}

CString 
SQLInfoAccess::GetCATALOGColumnList(CString /*p_schema*/,CString /*p_tablename*/) const
{
  // MS-Access cannot do this
  return "";
}

CString 
SQLInfoAccess::GetCATALOGColumnAttributes(CString /*p_schema*/,CString /*p_tablename*/,CString /*p_columname*/) const
{
  // MS-Access cannot do this
  return "";
}

CString 
SQLInfoAccess::GetCATALOGColumnCreate(MetaColumn& p_column) const
{
  CString sql = "ALTER TABLE "  + p_column.m_table  + "\n";
                "  ADD COLUMN " + p_column.m_column + " " + p_column.m_typename;
  if(p_column.m_precision)
  {
    sql.AppendFormat("(%d",p_column.m_precision);
    if(p_column.m_scale)
    {
      sql.AppendFormat(",%d",p_column.m_scale);
    }
    sql += ")";
  }
  if(!p_column.m_nullable)
  {
    sql += " NOT NULL";
  }
  return sql;
}

CString 
SQLInfoAccess::GetCATALOGColumnAlter(MetaColumn& p_column) const
{
  CString sql = "ALTER TABLE  " + p_column.m_table + "\n";
                "ALTER COLUMN " + p_column.m_column + " " + p_column.m_typename;
  p_column.GetPrecisionAndScale(sql);
  p_column.GetNullable(sql);
  // m_position not used
  // m_length   not used
  // m_remarks  not used
  return sql;
}

CString 
SQLInfoAccess::GetCATALOGColumnRename(CString p_schema,CString p_tablename,CString p_columnname,CString p_newname,CString p_datatype) const
{
  CString sqlCode;
  sqlCode  = "ALTER TABLE " + p_tablename + "\n"
             "  ADD " + p_newname + " " + p_datatype + ";\n";
  sqlCode += "UPDATE " + p_tablename + "\n"
             "   SET " + p_newname   + " = " + p_columnname + ";\n";
  sqlCode += "ALTER TABLE " + p_tablename + "\n"
             " DROP COLUMN " + p_columnname + ";";
  return sqlCode;
}

CString 
SQLInfoAccess::GetCATALOGColumnDrop(CString p_schema,CString p_tablename,CString p_columnname) const
{
  CString sql("ALTER TABLE " + p_tablename + "\n"
              " DROP COLUMN " + p_columnname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL INDICES FUNCTIONS

// All index functions
CString 
SQLInfoAccess::GetCATALOGIndexExists(CString p_schema,CString p_tablename,CString p_indexname) const
{
  // Cannot query MS-Access for the index configuration
  return "";
}

CString 
SQLInfoAccess::GetCATALOGIndexList(CString p_schema,CString p_tablename)   const
{
  // Cannot query MS-Access for the index configuration
  return "";
}

CString 
SQLInfoAccess::GetCATALOGIndexAttributes(CString p_schema,CString p_tablename,CString p_indexname)  const
{
  // Cannot query MS-Access for the index configuration
  return "";
}

CString
SQLInfoAccess::GetCATALOGIndexCreate(MIndicesMap& p_indices) const
{
  // Get SQL to create an index for a table
  // CREATE [UNIQUE] [ASC|DESC] INDEX [<schema>.]indexname ON [<schema>.]tablename(column [,...]);
  CString query;
  for(auto& index : p_indices)
  {
    if(index.m_position == 1)
    {
      // New index
      query = "CREATE ";
      if(index.m_unique)
      {
        query += "UNIQUE ";
      }
      if(index.m_ascending != "A")
      {
        query += "DESC ";
      }
      query += "INDEX ";
      if(!index.m_schemaName.IsEmpty())
      {
        query += index.m_schemaName + ".";
      }
      query += index.m_indexName;
      query += " ON ";
      if(!index.m_schemaName.IsEmpty())
      {
        query += index.m_schemaName + ".";
      }
      query += index.m_tableName;
      query += "(";
    }
    else
    {
      query += ",";
    }
    query += index.m_columnName;
  }
  query += ")";
  return query;
}

CString 
SQLInfoAccess::GetCATALOGIndexDrop(CString /*p_schema*/,CString /*p_tablename*/,CString p_indexname) const
{
  CString sql = "DROP INDEX " + p_indexname;
  return sql;
}

// Get extra filter expression for an index column
CString
SQLInfoAccess::GetCATALOGIndexFilter(MetaIndex& /*p_index*/) const
{
  return "";
}

//////////////////////////////////////////////////////////////////////////
// ALL PRIMARY KEY FUNCTIONS

CString 
SQLInfoAccess::GetCATALOGPrimaryExists(CString p_schema,CString p_tablename) const
{
  // MS Access cannot get this info
  return "";
}

CString 
SQLInfoAccess::GetCATALOGPrimaryAttributes(CString p_schema,CString p_tablename) const
{
  // MS Access cannot get this info
  return "";
}

CString 
SQLInfoAccess::GetCATALOGPrimaryCreate(MPrimaryMap& p_primaries) const
{
  CString query("ALTER TABLE ");

  for(auto& prim : p_primaries)
  {
    if(prim.m_columnPosition == 1)
    {
      if(!prim.m_schema.IsEmpty())
      {
        query += prim.m_schema + ".";
      }
      query += prim.m_table + "\n";
      query += "  ADD CONSTRAINT " + prim.m_constraintName + "\n";
      query += "      PRIMARY KEY (";

    }
    else
    {
      query += ",";
    }
    query += prim.m_columnName;
  }
  query += ")";
  return query;
}

CString 
SQLInfoAccess::GetCATALOGPrimaryDrop(CString /*p_schema*/,CString p_tablename,CString p_constraintname) const
{
  CString sql("ALTER TABLE " + p_tablename + "\n"
              " DROP CONSTRAINT " + p_constraintname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL FOREIGN KEY FUNCTIONS

CString 
SQLInfoAccess::GetCATALOGForeignExists(CString /*p_schema*/,CString /*p_tablename*/,CString /*p_constraintname*/) const
{
  // MS-Access cannot get this information, Use ODBC functions
  return "";
}

CString 
SQLInfoAccess::GetCATALOGForeignList(CString /*p_schema*/,CString /*p_tablename*/,int /*p_maxColumns*/ /*=SQLINFO_MAX_COLUMNS*/) const
{ 
  // MS-Access cannot get this information, Use ODBC functions
  return "";
}

CString 
SQLInfoAccess::GetCATALOGForeignAttributes(CString /*p_schema*/,CString /*p_tablename*/,CString /*p_constraintname*/,int /*p_maxColumns*/ /*=SQLINFO_MAX_COLUMNS*/) const
{
  // MS-Access cannot get this information, Use ODBC functions
  return "";
}

CString 
SQLInfoAccess::GetCATALOGForeignCreate(MForeignMap& p_foreigns) const
{
 // Get first record
  MetaForeign& foreign = p_foreigns.front();

  // Construct the correct tablename
  CString table(foreign.m_fkTableName);
  CString primary(foreign.m_pkTableName);
  if(!foreign.m_fkSchemaName.IsEmpty())
  {
    table = foreign.m_fkSchemaName + "." + table;
  }
  if(!foreign.m_pkSchemaName.IsEmpty())
  {
    primary = foreign.m_pkSchemaName + "." + primary;
  }

  // The base foreign key command
  CString query = "ALTER TABLE " + table + "\n"
    "  ADD CONSTRAINT " + foreign.m_foreignConstraint + "\n"
    "      FOREIGN KEY (";

  // Add the foreign key columns
  bool extra = false;
  for(auto& key : p_foreigns)
  {
    if(extra) query += ",";
    query += key.m_fkColumnName;
    extra  = true;
  }

  // Add references primary table
  query += ")\n      REFERENCES " + primary + "(";

  // Add the primary key columns
  extra = false;
  for(auto& key : p_foreigns)
  {
    if(extra) query += ",";
    query += key.m_pkColumnName;
    extra  = true;
  }
  query += ")";

  switch(foreign.m_updateRule)
  {
    case SQL_CASCADE :    query += "\n      ON UPDATE CASCADE";     break;
    case SQL_SET_NULL:    query += "\n      ON UPDATE SET NULL";    break;
    default:              // In essence: ON UPDATE RESTRICT, but that's already the default
                          break;
  }
  switch(foreign.m_deleteRule)
  {
    case SQL_CASCADE:     query += "\n      ON DELETE CASCADE";     break;
    case SQL_SET_NULL:    query += "\n      ON DELETE SET NULL";    break;
    default:              // In essence: ON DELETE RESTRICT, but that's already the default
                          break;
  }
  return query;
}

CString 
SQLInfoAccess::GetCATALOGForeignAlter(MForeignMap& /*p_original*/, MForeignMap& /*p_requested*/) const
{
	// MS-Access cannot alter a foreign-key constraint.
	// You must drop and then re-create your foreign key constraint
	// So return an empty string to signal this!
	return "";
}

CString 
SQLInfoAccess::GetCATALOGForeignDrop(CString /*p_schema*/,CString p_tablename,CString p_constraintname) const
{
  CString sql("ALTER TABLE " + p_tablename + "\n"
              " DROP CONSTRAINT " + p_constraintname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL TRIGGER FUNCTIONS

CString 
SQLInfoAccess::GetCATALOGTriggerExists(CString p_schema, CString p_tablename, CString p_triggername) const
{
  // No triggers in MS-Access
  return "";
}

CString 
SQLInfoAccess::GetCATALOGTriggerList(CString p_schema, CString p_tablename) const
{
  // No triggers in MS-Access
  return "";
}

CString 
SQLInfoAccess::GetCATALOGTriggerAttributes(CString p_schema, CString p_tablename, CString p_triggername) const
{
  // No triggers in MS-Access
  return "";
}

CString 
SQLInfoAccess::GetCATALOGTriggerCreate(MetaTrigger& /*p_trigger*/) const
{
  // Cannot create a trigger. Does not exist in MS-Access
  return "";
}

CString 
SQLInfoAccess::GetCATALOGTriggerDrop(CString p_schema, CString p_tablename, CString p_triggername) const
{
  // Cannot drop a trigger. Does not exist in MS-Access
  return "";
}

//////////////////////////////////////////////////////////////////////////
// ALL SEQUENCE FUNCTIONS

CString 
SQLInfoAccess::GetCATALOGSequenceExists(CString p_schema, CString p_sequence) const
{
  // MS-Access does not have sequences
  return "";
}

CString 
SQLInfoAccess::GetCATALOGSequenceAttributes(CString p_schema, CString p_sequence) const
{
  // MS-Access does not have sequences
  return "";
}

CString 
SQLInfoAccess::GetCATALOGSequenceCreate(MetaSequence& /*p_sequence*/) const
{
  // MS-Access does not have sequences
  return "";
}

CString 
SQLInfoAccess::GetCATALOGSequenceDrop(CString p_schema, CString p_sequence) const
{
  // MS-Access does not have sequences
  return "";
}

//////////////////////////////////////////////////////////////////////////
// ALL VIEW FUNCTIONS

CString 
SQLInfoAccess::GetCATALOGViewExists(CString p_schema,CString p_viewname) const
{
  // Cannot query this, Use ODBC functions
  return "";
}

CString 
SQLInfoAccess::GetCATALOGViewList(CString p_schema,CString p_pattern) const
{
  // Cannot query this, Use ODBC functions
  return "";
}

CString 
SQLInfoAccess::GetCATALOGViewAttributes(CString p_schema,CString p_viewname) const
{
  // Cannot query this, Use ODBC functions
  return "";
}

CString 
SQLInfoAccess::GetCATALOGViewCreate(CString /*p_schema*/,CString p_viewname,CString p_contents) const
{
  return "CREATE VIEW " + p_viewname + "\n" + p_contents;
}

CString 
SQLInfoAccess::GetCATALOGViewRename(CString p_schema,CString p_viewname,CString p_newname)    const
{
  return "";
}

CString 
SQLInfoAccess::GetCATALOGViewDrop(CString /*p_schema*/,CString p_viewname,CString& p_precursor) const
{
  p_precursor.Empty();
  return "DROP VIEW " + p_viewname;
}

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

CString 
SQLInfoAccess::GetPSMProcedureExists(CString p_schema,CString p_procedure) const
{
  // MS-Access does not support PSM
  return "";
}
  
CString 
SQLInfoAccess::GetPSMProcedureList(CString p_schema) const
{
  // MS-Access does not support PSM
  return "";
}
  
CString 
SQLInfoAccess::GetPSMProcedureAttributes(CString p_schema, CString p_procedure) const
{
  // MS-Access does not support PSM
  return "";
}
  
CString 
SQLInfoAccess::GetPSMProcedureCreate(MetaProcedure& /*p_procedure*/) const
{
  // MS-Access does not support PSM
  return "";
}
  
CString 
SQLInfoAccess::GetPSMProcedureDrop(CString p_schema,CString p_procedure) const
{
  // MS-Access does not support PSM
  return "";
}

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

CString 
SQLInfoAccess::GetSESSIONMyself() const
{
  // MS-Access has no info about processes
  return "";
}

CString 
SQLInfoAccess::GetSESSIONExists(CString p_sessionID) const
{
  // MS-Access has no info about processes
  return "";
}

CString 
SQLInfoAccess::GetSESSIONList() const
{
  // MS-Access has no info about processes
  return "";
}

CString 
SQLInfoAccess::GetSESSIONAttributes(CString p_sessionID) const
{
  // MS-Access has no info about processes
  return "";
}

//////////////////////////////////////////////////////////////////////////
// Transactions

CString
SQLInfoAccess::GetSESSIONConstraintsDeferred() const
{
  // MS-Access cannot defer constraints
  return "";
}

CString 
SQLInfoAccess::GetSESSIONConstraintsImmediate() const
{
  // MS-Access constraints are always active
  return "";
}


//////////////////////////////////////////////////////////////////////////
//
// OLD INTERFACE
//
//////////////////////////////////////////////////////////////////////////

// BOOLEANS EN STRINGS
// ====================================================================

// Gets the fact if an IF statement needs to be bordered with BEGIN/END
bool
SQLInfoAccess::GetCodeIfStatementBeginEnd() const
{
  // IF THEN ELSE END IF; does not need a BEGIN/END per se.
  return true;
}

// Gets the end of an IF statement
CString 
SQLInfoAccess::GetCodeEndIfStatement() const
{
  return "";
}

// Gets a complete assignment statement.
CString 
SQLInfoAccess::GetAssignmentStatement(const CString& p_destiny,const CString& p_source) const
{
  if(p_source.Find("EXECUTE") == -1)
  {
    return "SET " + p_destiny + " = " + p_source + ";";
  }
  else
  {
    CString sqlCode = p_source;
    sqlCode.Replace("(EXECUTE", "EXECUTE");
    sqlCode.Replace("<@@@var@@@>", p_destiny);
    sqlCode.Replace(";)", ";");
    return sqlCode;
  }
}

// Get the code to start a WHILE-loop
CString
SQLInfoAccess::GetStartWhileLoop(CString p_condition) const
{
  return "WHILE " + p_condition + " LOOP\n";
}

// Get the code to end a WHILE-loop
CString
SQLInfoAccess::GetEndWhileLoop() const
{
  return "END LOOP;\n";
}

// Gets the fact if a SELECT must be in between parenthesis for an assignment
bool 
SQLInfoAccess::GetAssignmentSelectParenthesis() const
{
  return false;
}

// SQL CATALOG QUERIES
// ===================================================================

// Gets the lock-table query
CString 
SQLInfoAccess::GetSQLLockTable(CString& p_tableName,bool p_exclusive) const
{
  CString query = "SELECT * FROM " + p_tableName + " WITH "  ;
  query += p_exclusive ? "(TABLOCKX)" : "(TABLOCK)";
  return query;
}

// Get query to optimize the table statistics
CString 
SQLInfoAccess::GetSQLOptimizeTable(CString& /*p_owner*/,CString& /*p_tableName*/,int& /*p_number*/)
{
  return "";
}

// SQL DDL STATEMENTS
// ==================

// SQL DDL ACTIONS
// ===================================================================

// Do the commit for the DDL commands in the catalog
void 
SQLInfoAccess::DoCommitDDLcommands() const
{
  SQLQuery query(m_database);
  query.DoSQLStatement("COMMIT WORK");
}

// Do the commit for the DML commands in the database
// ODBC driver auto commit mode will go wrong!!
void
SQLInfoAccess::DoCommitDMLcommands() const
{
  // Not needed in MS-Access
}

// PERSISTENT-STORED MODULES (SPL / PL/SQL)
// ====================================================================

// Get the user error text from the database
CString 
SQLInfoAccess::GetUserErrorText(CString& /*p_procName*/) const
{
  return "";
}

// Get assignment to a variable in SPL
CString 
SQLInfoAccess::GetSPLAssignment(CString p_variable) const
{
  return "SET " + p_variable + " = ";
}

// Get the start of a SPL While loop
CString 
SQLInfoAccess::GetSPLStartWhileLoop(CString p_condition) const
{
  return "WHILE " + p_condition + "\n  BEGIN\n";
}

// Get the end of a SPL while loop
CString 
SQLInfoAccess::GetSPLEndWhileLoop() const
{
  return "END;\n";
}

// Get stored procedure call
CString 
SQLInfoAccess::GetSQLSPLCall(CString p_procName) const
{

  if (p_procName.Find("@out") == -1)
  {
    p_procName.Replace("(", " ");
    p_procName.Replace(")", " ");
    return "DECLARE\n"
      "@vv_result INTEGER,\n"
      "@vv_sqlerror INTEGER,\n"
      "@vv_isamerror INTEGER,\n"
      "@vv_errordata VARCHAR(255);\n"
      "EXECUTE " + p_procName + ", @vv_result OUTPUT, @vv_sqlerror OUTPUT, @vv_isamerror OUTPUT, @vv_errordata OUTPUT;\n"
      "SELECT @vv_result";
  }
  else
  {
    CString return_type = p_procName.Left(p_procName.Find('|'));
    CString statement   = p_procName.Mid (p_procName.Find('|')+1);
    return "DECLARE @out " + return_type + ";\n"
      "EXECUTE " + statement + ";\n"
      "SELECT @out";
  }
}

// Build a parameter list for calling a stored procedure
CString 
SQLInfoAccess::GetBuildedParameterList(size_t p_numOfParameters) const
{
  // IF NO PARAMETERS, NO ELLIPSIS EITHER!!
  CString strParamLijst;
  if(p_numOfParameters >= 0)
  {
    for (size_t i = 0; i < p_numOfParameters; i++)
    {
      if(i!=0) 
      {
        strParamLijst += ",";
      }
      else
      {
        strParamLijst += "(";
      }
      strParamLijst += "?";
    }
    if(p_numOfParameters > 0)
    {
      strParamLijst += ")";
    }
  }
  return strParamLijst;
}

// Parameter type for stored procedure for a given column type for parameters and return types
CString
SQLInfoAccess::GetParameterType(CString& p_type) const
{
  // char, varchar -> varchar
  // decimal -> number

  CString retval;
  if (_strnicmp((LPCSTR)p_type,"char",4) == 0 ||
    _strnicmp((LPCSTR)p_type,"varchar",7) == 0 )
    retval = "varchar";
  else if (_strnicmp((LPCSTR)p_type,"decimal",7) == 0 )
    retval = "decimal";
  else 
    retval = p_type;
  return p_type;
}

// Makes a SQL string from a given string, with all the right quotes
CString 
SQLInfoAccess::GetSQLString(const CString& p_string) const
{
  CString s = p_string;
  s.Replace("'","''");
  CString kwoot = GetKEYWORDQuoteCharacter();
  return  kwoot + s + kwoot;
}

// Get date string in engine format
CString 
SQLInfoAccess::GetSQLDateString(int p_year,int p_month,int p_day) const
{
  CString retval;
  retval.Format("{ d '%04d-%02d-%02d' }",p_year,p_month,p_day);
  return retval;
}  

// Get time string in database engine format
CString 
SQLInfoAccess::GetSQLTimeString(int p_hour,int p_minute,int p_second) const
{
  CString retval;
  retval.Format("{ t '%02d:%02d:%02d' }",p_hour,p_minute,p_second);
  return retval;
}  

// Get date-time string in database engine format
CString 
SQLInfoAccess::GetSQLDateTimeString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  CString retval;
  retval.Format("{ ts '%04d-%02d-%02d %02d:%02d:%02d' }",p_year,p_month,p_day,p_hour,p_minute,p_second);
  return retval;
}  

// Get date-time bound parameter string in database format
CString 
SQLInfoAccess::GetSQLDateTimeBoundString() const
{
  return "{ts ?}";
}

// Stripped data for the parameter binding
CString
SQLInfoAccess::GetSQLDateTimeStrippedString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  CString retval;
  retval.Format("%04d-%02d-%02d %02d:%02d:%02d",p_year,p_month,p_day,p_hour,p_minute,p_second);
  return retval;
}

// Get the SPL datatype for integer
CString 
SQLInfoAccess::GetSPLIntegerType() const
{
  return "integer";
}

// Get the SPL datatype for a decimal
CString 
SQLInfoAccess::GetSPLDecimalType() const
{
  return "number";
}

// Get the SPL declaration for a cursor
CString 
SQLInfoAccess::GetSPLCursorDeclaratie(CString& p_variableName,CString& p_query) const
{
  // TODO: Check
  return "CURSOR " + p_variableName + " IS " + p_query + ";";
}

// Get the SPL cursor found row parameter
CString 
SQLInfoAccess::GetSPLCursorFound(CString& /*p_cursorName*/) const
{
  // TODO: Implement
  return "";
}

// Get the SPL cursor row-count variable
CString 
SQLInfoAccess::GetSPLCursorRowCount(CString& /*p_variable*/) const
{
  // TODO: Implement
  return "";
}

// Get the SPL datatype for a declaration of a row-variable
CString 
SQLInfoAccess::GetSPLCursorRowDeclaration(CString& /*p_cursorName*/,CString& /*p_variableName*/) const
{
  // TODO: Implement
  return "";
}

CString 
SQLInfoAccess::GetSPLFetchCursorIntoVariables(CString               p_cursorName
                                                 ,CString             /*p_variableName*/
                                                 ,std::vector<CString>& p_columnNames
                                                 ,std::vector<CString>& p_variableNames) const
{
  // TODO: Check
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
SQLInfoAccess::GetSPLFetchCursorIntoRowVariable(CString& p_cursorName,CString p_variableName) const
{ 
  // TODO: Check
  return "FETCH " + p_cursorName + " INTO " + p_variableName+ ";";
}

// Get the SPL no-data exception clause
CString 
SQLInfoAccess::GetSPLNoDataFoundExceptionClause() const
{
  // TODO: Check
  return "WHEN NO_DATA THEN";
}

// Get the SPL form of raising an exception
CString 
SQLInfoAccess::GetSPLRaiseException(CString p_exceptionName) const
{
  // TODO: Check
  return "RAISE " + p_exceptionName + ";";
}

// Get the fact that the SPL has server functions that return more than 1 value
bool    
SQLInfoAccess::GetSPLServerFunctionsWithReturnValues() const
{
  return true;
}

// Calling a stored function or procedure if the RDBMS does not support ODBC call escapes
SQLVariant*
SQLInfoAccess::DoSQLCall(SQLQuery* /*p_query*/,CString& /*p_schema*/,CString& /*p_procedure*/)
{
  return nullptr;
}

// End of namespace
}
