////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoAccess.cpp
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
#include "stdafx.h"
#include "SQLInfoAccess.h"
#include "SQLQuery.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Constructor.
SQLInfoAccess::SQLInfoAccess(SQLDatabase* p_database)
              :SQLInfoDB(p_database)
{
}

// Destructor. does nothing
SQLInfoAccess::~SQLInfoAccess()
{
}

// Get the database type
DatabaseType 
SQLInfoAccess::GetDatabaseType() const
{
  return RDBMS_SQLSERVER;
}

// BOOLEANS EN STRINGS
// ====================================================================

CString
SQLInfoAccess::GetDatabaseVendorName() const
{
  // The name of the database vendor
  return "Microsoft";
}

// Get the fysical database name
CString 
SQLInfoAccess::GetFysicalDatabaseName() const
{
  return m_database->GetDatabaseName();
}

// System catalog is stored in uppercase in the database?
bool 
SQLInfoAccess::IsCatalogUpper() const
{
  return false;
}

// Supports database/ODBCdriver comments in sql
bool 
SQLInfoAccess::SupportsDatabaseComments() const
{
  return false;
}

// Database can defer constraints until the end of a transaction
bool 
SQLInfoAccess::SupportsDeferredConstraints() const
{
  // SET CONSTRAINTS DEFERRED aanwezig
  return true;
}

// Database has ORDER BY with an expression, e.g. ORDER BY UPPER(columnname)
// Work-around is "SELECT UPPER(columname) AS something.....ORDER BY something
bool
SQLInfoAccess::SupportsOrderByExpression() const
{
  return true;
}

// Catalogus query for the default value of a table's column
CString 
SQLInfoAccess::GetSQLStringDefaultValue(CString p_tableName,CString p_columnName) const
{
  CString query = "SELECT replace(replace('#' + text + '#', '#(', ''), ')#', '')\n"
    "  FROM dbo.sysobjects obj, dbo.syscolumns col, dbo.syscomments com\n"
    " WHERE obj.name = '" + p_tableName + "'\n"
    "   AND obj.Id = col.id\n"
    "   AND col.name = '" + p_columnName + "'\n"
    "   AND col.cdefault = com.id\n";
  return query;
}

// Keyword for the current date and time
CString 
SQLInfoAccess::GetSystemDateTimeKeyword () const
{
  return "GETDATE()"; // "now"
} 

// String for the current date
CString
SQLInfoAccess::GetSystemDateString() const
{
  CString dbLaatsteLogin;
  //dbLaatsteLogin = "CONVERT(datetime, '" + Datum::Vandaag().AlsString() + "')";
  return dbLaatsteLogin;
}

// If the database does not support the datatype TIME, it can be implemented as a DECIMAL
bool 
SQLInfoAccess::GetTimeIsDecimal() const
{
  // Time can be implemented as DECIMAL(17,16)
  return false;
}

// If the database does not suppoprt the datatype INTERVAL, it can be implemented as a DECIMAL
bool 
SQLInfoAccess::GetIntervalIsDecimal() const
{
  // INTERVAL can be implemented as DECIMAL
  return true;
}

// Get the concatanation operator
CString 
SQLInfoAccess::GetConcatanationOperator() const
{
  return "+";    
}

// Get quote character for strings
CString 
SQLInfoAccess::GetQuoteCharacter() const
{
  return "'";    
}

// Get default NULL for parameterlist input
CString 
SQLInfoAccess::GetDefaultNULL() const
{
  return " = NULL ";    
}

// Parameter is for INPUT and OUTPUT in parameterlist
CString 
SQLInfoAccess::GetParameterINOUT() const
{
  return "OUTPUT ";    
}

// Parameter is for OUTPUT only in parameterlist
CString 
SQLInfoAccess::GetParameterOUT() const
{
  return "OUTPUT ";    
}

// Get the datatype of the auditted user (h_user) in a stored procedure
CString 
SQLInfoAccess::GetAuditUserDatatype() const
{
  return "VARCHAR(50)";
} 

// Get the datatype of the auditted user (h_user) as variable in a stored-procedure
CString 
SQLInfoAccess::GetAuditUserDatatypeAsVariable() const
{
  return "VARCHAR(50)";
} 

// Get datatype of the IDENTITY primary key
CString 
SQLInfoAccess::GetPrimaryKeyType() const
{
  return "INTEGER IDENTITY(1, 1)";
}

// Get datatype for Moment
CString 
SQLInfoAccess::GetDatetimeYearToSecondType() const
{
  return "DATETIME";
}

// Separator between two alter-constraints in an alter-table statement
CString
SQLInfoAccess::GetAlterConstraintSeparator() const
{
  return ",";
}

// Inner Join Keyword
CString 
SQLInfoAccess::GetInnerJoinKeyword() const
{
  return "INNER JOIN ";
}

// Outer join keyword
CString  
SQLInfoAccess::GetOuterJoinKeyword() const
{
  return "LEFT OUTER JOIN ";
}

// Inner Join Keyword for use in views.
CString 
SQLInfoAccess::GetViewInnerJoinKeyword() const
{
  return "INNER JOIN";
}


// Outer join keyword for use in views
CString 
SQLInfoAccess::GetViewOuterJoinKeyword() const
{
  return "LEFT OUTER JOIN";
}

// Get the closure for an outer-join
CString 
SQLInfoAccess::GetOuterJoinClosure() const
{
  return "";
}

// Outer join sign
CString  
SQLInfoAccess::GetOuterJoinSign() const
{
  return "";
}

// Prefix for a parameter in a stored procedure
CString
SQLInfoAccess::GetSPParamPrefix() const
{
  return "@";
}

// Get select part to add new record identity to a table
// Can be special column like 'OID'
CString 
SQLInfoAccess::GetIdentityString(CString& p_tablename,CString /*p_postfix = "_seq"*/) const
{
  return "IDENT_CURRENT('" + p_tablename + "') + " + "IDENT_INCR('" + p_tablename + "')";
}

// Get a query to create a temporary table from a select statement
CString 
SQLInfoAccess::GetSQLCreateTemporaryTable(CString& p_tablename,CString p_select) const
{
  return "CREATE TABLE #" + p_tablename + "\nAS " + p_select;
}

// Get the query to remove a temporary table indefinetly
// BEWARE: Must be executed with a multi-statement stack!
CString
SQLInfoAccess::GetSQLRemoveTemporaryTable(CString& p_tablename,int& p_number) const
{
  // Dit werkt in MS_SQLServer alleen zo, omdat er anders een foutmelding komt dat
  // de tijdelijke tabel nog in gebruik is en de tabeldefinitie dan in de
  // catalog van de RDBMS blijft staan. De definitie is dan strijdig met een
  // mogelijke volgende definitie onder deze naam
  p_number += 3;
  return "DELETE FROM #"    + p_tablename + ";\n"
    "TRUNCATE TABLE #" + p_tablename + ";\n"
    "DROP TABLE #"     + p_tablename + ";\n";
}

// Get a query to select into a temp table
CString 
SQLInfoAccess::GetSQLSelectIntoTemp(CString& p_tablename,CString& p_select) const
{
  return "INSERT INTO #" + p_tablename + "\n" + p_select + ";\n";
}

// Replace the Identity column OID by a sequence.nextval
CString 
SQLInfoAccess::GetReplaceColumnOIDbySequence(CString p_columns,CString p_tablename,CString p_postfix /*="_seq"*/) const
{
  int pos;
  int len = p_columns.GetLength();
  pos = p_columns.Find("OID,");
  if(pos == 0)
  {
    CString newkolom = ( p_tablename + p_postfix + ".nextval");
    newkolom += p_columns.Right(len - 3);
    return newkolom;
  }
  if(pos > 0)
  {
    pos = p_columns.Find(",OID,");
    CString newkolom = p_columns.Left(pos) + "," + p_tablename + p_postfix + ".nextval";
    newkolom += p_columns.Right(len - pos - 5);
    return newkolom;
  }
  return p_columns;
}

// Get the tablespace for the tables
CString
SQLInfoAccess::GetTablesTablespace(CString p_tablespace /*=""*/) const
{
  return "";
}

// Get the tablespace for the indexes
// If no default is givven, fallbacks to the DEFAULT tablespace
CString
SQLInfoAccess::GetIndexTablespace(CString p_tablespace /*=""*/) const
{
  return "";
}

// Get the storage name for indici
CString 
SQLInfoAccess::GetStorageSpaceNameForIndexes() const
{
  return "INDEX"; // Settings::OpslagRuimte::OracleIndexTablespace;
}

// Get the storage space for temporary tables
CString
SQLInfoAccess::GetStorageSpaceNameForTempTables(CString p_tablename) const
{
  // MS_SQLServer uses implicit TEMP TABLESPACE for the user
  return "";
}

// Remove catalog dependencies for stored procedures
// To be run after a 'DROP PROCEDURE' or 'DROP FUNCTION'
CString 
SQLInfoAccess::GetSQLRemoveProcedureDependencies(CString p_procname) const
{
  return "";
}

// Remove field dependencies for calculated fields (bug in Firebird)
// To be run after a 'DROP TABLE' or an 'ALTER TABLE DROP COLUMN' or 'ALTER TABLE MODIFY COLUMN'
CString 
SQLInfoAccess::GetSQLRemoveFieldDependencies(CString p_tablename) const
{
  // Niet nodig/bestaat niet in Informix
  return "";
}

// Gets the table definition-form of a primary key
CString 
SQLInfoAccess::GetPrimaryKeyDefinition(CString p_tableName,bool /*p_temporary*/) const
{
  // The primary key constraint is not directly generated after the column
  // to ensure it wil use the named index in the correct tablespace
  // Otherwise the index name and tablespace cannot be definied and will be auto-generated
  return GetPrimaryKeyType() + " NOT NULL\n";
}

// Get the constraint form of a primary key to be added to a table after creation of that table
CString
SQLInfoAccess::GetPrimaryKeyConstraint(CString p_tablename,bool /*p_temporary*/) const
{
  return "ADD CONSTRAINT pk_" + p_tablename + " PRIMARY KEY(oid)\n";
}

// Performance parameters to be added to the database
CString
SQLInfoAccess::GetSQLPerformanceSettings() const
{
  return "";
}

// SQL To set the caching mode of SQL results
CString 
SQLInfoAccess::GetSQLCacheModeSetting(const CString& /*p_mode*/) const
{
  return "";
}

// Needed for storing numbers in stored procedures
CString
SQLInfoAccess::GetSQLNlsNumericCharacters() const
{
  // Not used in MS-SQLServer
  return "";
}

// Gives the statement to alter a table columns' name
CString
SQLInfoAccess::GetSQLModifyColumnName(CString p_tablename,CString p_oldName,CString p_newName,CString p_datatype)
{
  CString sqlCode;
  sqlCode  = "ALTER TABLE " + p_tablename + "\n"
    "        ADD " + p_newName + " " + p_datatype + ";\n";
  sqlCode += "UPDATE " + p_tablename + "\n"
    "   SET " + p_newName   + " = " + p_oldName + ";\n";
  sqlCode += "ALTER TABLE " + p_tablename + "\n"
    " DROP COLUMN " + p_oldName + ";";
  return sqlCode;
}

// Gets the maximum length of an SQL statement
unsigned long 
SQLInfoAccess::GetMaxStatementLength() const
{
  return 0;		// No limit
}

// Prefix for an add constraint DDL command in SQLAtlerTableGenerator
CString 
SQLInfoAccess::GetAddConstraintPrefix(CString p_constraintName) const
{
  return "ADD CONSTRAINT " + p_constraintName + " ";
}

// Suffix for an add constraint DDL command in SQLAtlerTableGenerator
CString 
SQLInfoAccess::GetAddConstraintSuffix(CString /*p_constraintName*/) const
{
  return "";
}

// Get the prefix for a drop constraint DDL command in the SQLAlterTableGenerator
CString 
SQLInfoAccess::GetDropConstraintPrefix() const
{
  return "DROP CONSTRAINT ";
}

// Get the suffix for a drop constraint DDL commando in the SQLAlterTableGenerator
CString 
SQLInfoAccess::GetDropConstraintSuffix() const
{
  return "";
}

// Clause separator between two ADD or DROP clauses in an ALTER TABLE
CString 
SQLInfoAccess::GetAlterTableClauseSeparator() const
{
  return ", ";
}

// Grouping of more than one column possible in an ADD/MODIFY/DROP clause
bool   
SQLInfoAccess::GetClauseGroupingPossible() const
{
  // Kan meerdere ADD (kolomdef ,.... ) doen
  return true;
}

// Gets the prefix needed for altering the datatype of a column in a MODIFY/ALTER
CString 
SQLInfoAccess::GetModifyDatatypePrefix() const
{
  // Bij het veranderen van het datatype gewoon het nieuwe type opgeven
  // DUS: MODIFY <kolomnaam> <datatypenaam>
  return "";
}

// Code to create a temporary table (qualifier)
CString 
SQLInfoAccess::GetCodeTemporaryTable() const
{
  return "";
}

// Code to define a table in row-locking mode
CString 
SQLInfoAccess::GetCodeLockModeRow() const
{
  return "";
}

// Code to create a temporary table with no logging
CString 
SQLInfoAccess::GetCodeTempTableWithNoLog() const
{
  return "";
}

// Granting all rights on a table (In a NON-ANSI database)
CString 
SQLInfoAccess::GetSQLGrantAllOnTable(CString p_tableName)
{
  return "GRANT ALL ON " + p_tableName + " TO " + GetGrantedUsers() + " WITH GRANT OPTION;\n";
}


// Code prefix for a select-into-temp
CString
SQLInfoAccess::GetSelectIntoTempClausePrefix(CString p_tableName) const
{
  return "";
}

// Code suffix for after a select-into-temp
CString
SQLInfoAccess::GetSelectIntoTempClauseSuffix(CString p_tableName) const
{
  return "";
}

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

// Get SQL keyword to alter a column in a table
CString 
SQLInfoAccess::GetCodeAlterColumn() const
{
  return "ALTER COLUMN ";
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

// Gets the fact if a SELECT must be inbetween parenthesis for an assignment
bool 
SQLInfoAccess::GetAssignmentSelectParenthesis() const
{
  return false;
}

// Gets the UPPER function
CString 
SQLInfoAccess::GetUpperFunction(CString& p_expression) const
{
  return "{fn UCASE(" + p_expression + ")}";
}

// Gets the construction for 1 minute ago
CString 
SQLInfoAccess::GetInterval1MinuteAgo() const
{
  return "ERROR";
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

// Gets the Not-NULL-Value statement of the database
CString 
SQLInfoAccess::GetNVLStatement(CString& p_test,CString& p_isnull) const
{
  return CString("IIF(ISNULL(") + p_test + ")," + p_isnull + "," + p_test + ")";
}

// Gets the subtransaction commands
CString 
SQLInfoAccess::GetStartSubTransaction(CString p_savepointName) const
{
  // Does not know how to do sub-transactions
  return "";
}

CString 
SQLInfoAccess::GetCommitSubTransaction(CString p_savepointName) const
{
  // Does not know how to do sub-transactions
  return "";
}

CString 
SQLInfoAccess::GetRollbackSubTransaction(CString p_savepointName) const
{
  // Does not know how to do sub-transactions
  return "";
}

// SQL CATALOG QUERIES
// ===================================================================

// Get SQL to check if a storedprocedure already exists in the database
CString 
SQLInfoAccess::GetSQLStoredProcedureExists(CString& p_name) const
{
  CString unaam(p_name);
  unaam.MakeUpper();
  return "SELECT count(*)\n"
    "  FROM all_objects\n"
    " WHERE UPPER(object_name) = '" + unaam + "'\n"
    "   AND object_type        = 'FUNCTION';";
}

// Part of a query to select only 1 (one) record
CString
SQLInfoAccess::GetDualTableName() const
{
  return "";
}

// FROM-Part for a query to select only 1 (one) record
CString 
SQLInfoAccess::GetDualClause() const
{
  return "";
}

// Gets DEFERRABLE for a constraint (or nothing)
CString
SQLInfoAccess::GetConstraintDeferrable() const
{
  return "";
}

// Defer Constraints until the next COMMIT;
CString 
SQLInfoAccess::GetSQLDeferConstraints() const
{
  return "";
}

// Reset constraints back to immediate
CString 
SQLInfoAccess::GetSQLConstraintsImmediate() const
{
  return "";
}

// Get SQL to check if a table already exists in the database
CString 
SQLInfoAccess::GetSQLTableExists(CString p_tablename) const
{
  CString query = "SELECT count(*)\n"
    "  FROM dbo.sysobjects\n"
    " WHERE name = '" + p_tablename + "'";
  return query;
}

// Get SQL to select all columns of a table from the catalog
CString 
SQLInfoAccess::GetSQLGetColumns(CString& /*p_user*/,CString& p_tableName) const
{
  // naam, nummer, type, lengte,nullable
  CString select = "SELECT col.name\n"
    "      ,col.colid\n"
    "      ,typ.name\n"
    "      ,col.length\n"
    "      ,col.isnullable\n"
    "      ,col.prec\n"
    "      ,col.scale\n"
    "  FROM dbo.sysobjects obj, dbo.syscolumns col, dbo.systypes typ\n"
    " WHERE obj.name = '" + p_tableName  + "'\n"
    "   AND obj.Id = col.id\n"
    "   AND col.xtype = typ.xtype\n"
    " ORDER BY col.colid";
  return select;
}

// Get SQL to select all constraints on a table from the catalog
CString 
SQLInfoAccess::GetSQLGetConstraintsForTable(CString& p_tableName) const
{
  CString contabel = "SELECT con.name\n"
    "      ,tab.name\n"
    "  FROM dbo.sysobjects con, dbo.sysobjects tab\n"
    " WHERE tab.name = '" + p_tableName + "'\n"
    "   AND tab.Id = con.parent_obj\n"
    "   AND con.xtype in ('C ', 'UQ')\n"
    "   AND con.type  in ('C ', 'K ')\n";
  return contabel;
}

// Get SQL to read all indici for a table
CString 
SQLInfoAccess::GetSQLTableIndexes(CString& /*p_user*/,CString& p_tableName) const
{
  CString query = "SELECT idx.name\n"
    "      ,indexproperty(obj.Id, idx.name, 'IsClustered')\n"
    "      ,indexproperty(obj.Id, idx.name, 'IsUnique')\n"
    "      ,ixk.keyno\n"
    "      ,col.name\n"
    "      ,indexkey_property(obj.Id, idx.indid, ixk.keyno, 'IsDescending')\n"
    "  FROM dbo.sysindexes idx\n"
    "      ,dbo.sysindexkeys ixk\n"
    "      ,dbo.sysobjects obj\n"
    "      ,dbo.syscolumns col\n"
    " WHERE obj.name = '" + p_tableName + "'\n"
    "   AND obj.Id = idx.id\n"
    "   AND obj.Id = ixk.id\n"
    "   AND idx.indid = ixk.indid\n"
    "   AND ixk.colid = col.colid\n"
    "   AND col.id = obj.Id\n"
    "   AND NOT idx.name LIKE '\\_WA\\_Sys\\_%\\_%' ESCAPE '\\'\n"
    //                       "   AND idx.first <> 0\n"
    " ORDER BY idx.name\n"
    "         ,ixk.keyno\n";
  return query;
}

// Get SQL to read the referential constaints from the catalog
CString 
SQLInfoAccess::GetSQLTableReferences(CString& p_tablename) const
{
  CString query = "SELECT con.name\n"
    "      ,tab.name\n"
    "  FROM dbo.sysobjects con, dbo.sysobjects tab\n"
    " WHERE tab.name = '" + p_tablename + "'"
    "   AND tab.Id = con.parent_obj\n"
    "   AND con.xtype = 'F'\n"
    "   AND con.type = 'F'\n";
  return query;
}

// Get the SQL Query to create a synonym
CString
SQLInfoAccess::GetSQLMakeSynonym(CString& /*p_objectName*/) const
{
  return "";
}

// Get SQL to drop the synonym
CString 
SQLInfoAccess::GetSQLDropSynonym(CString& /*p_objectName*/) const
{
  return "";
}

// Create a sequence in the database
void 
SQLInfoAccess::DoCreateSequence(CString& /*p_sequenceName*/,int /*p_startpos*/) 
{
}

// Remove a sequence from the database
void
SQLInfoAccess::DoRemoveSequence(CString& /*p_sequenceName*/) const
{
}

// Re-Creates a sequence in a database from the OID column
void 
SQLInfoAccess::DoCreateNextSequence(const CString& /*p_tableName*/,CString /*p_postfix /*="_seq"*/)
{
}

// Remove a stored procedure from the database
void 
SQLInfoAccess::DoRemoveProcedure(CString& p_procedureName) const
{
  SQLQuery query(m_database);
  query.DoSQLStatement("DROP PROCEDURE " + p_procedureName);
}

// Gets the SQL for the rights on the sequence
CString 
SQLInfoAccess::GetSQLSequenceRights(const CString& /*p_tableName*/,CString /*p_postfix /*="_seq"*/) const
{
  return "";
}

// Get SQL for your session and controling terminal
CString
SQLInfoAccess::GetSQLSessionAndTerminal() const
{
  CString query = "SELECT rtrim(hostprocess)\n"
    "      ,rtrim(hostname)\n"    
    "  FROM master.dbo.sysprocesses\n"
    " WHERE hostprocess = host_id()\n"
    "   AND hostname = host_name()\n"
    "   AND rtrim(program_name) = 'Pronto'";
  return query;
}

// Get SQL to check if sessionnumber exists
CString 
SQLInfoAccess::GetSQLSessionExists(CString p_sessionID) const
{
  return "SELECT DISTINCT rtrim(hostprocess)\n"
    "  FROM master.dbo.sysprocesses\n"
    " WHERE rtrim(hostprocess) <> " + p_sessionID;
}

// Get SQL for unique session ID
CString 
SQLInfoAccess::GetSQLUniqueSessionId(const CString& /*p_databaseName*/,const CString& /*p_sessionTable*/) const
{
  return "SELECT DISTINCT rtrim(hostprocess)\n"
    "  FROM master.dbo.sysprocesses\n"
    " WHERE rtrim(hostprocess) <> '0'\n"
    "   AND dbid = db_id()";
}

// Get SQL for searching a session
CString 
SQLInfoAccess::GetSQLSearchSession(const CString& /*p_databaseName*/,const CString& p_sessionTable) const
{
  // In MS_SQLServer is de database de engine. Databasenaam is dus niet relevant
  return "SELECT rtrim(hostprocess)\n"
    "      ,rtrim(nt_username)\n"
    "      ,rtrim(hostname)\n"
    "  FROM master.dbo.sysprocesses\n"
    " WHERE rtrim(hostprocess) <> '0'\n"
    "   AND dbid = db_id()\n"
    "   AND NOT rtrim(hostprocess) IN\n"
    "     ( SELECT sessie_nr\n"
    "         FROM "+ p_sessionTable + ")";
}


// See if a column exsists within a table
bool   
SQLInfoAccess::DoesColumnExistsInTable(CString& p_owner,CString& p_tableName,CString& p_column) const
{
  int number = 0;

  CString tableName(p_tableName);
  tableName.MakeLower();
  CString column(p_column);
  column.MakeLower();
  CString owner(p_owner);
  owner.MakeLower();

  CString query = "SELECT count(*)\n"
    "  FROM dbo.sysobjects sobj, dbo.syscolumns scol, dbo.sysusers suse\n"
    " WHERE sobj.name  = '" + tableName + "'\n"
    "   AND sobj.xtype = 'U'\n"
    "   AND sobj.id    = scol.id\n"
    "   AND scol.name  = '" + column + "'\n"
    "   AND sobj.uid   = suse.uid\n"
    "   AND suse.name  = '" + owner + "'";
  SQLQuery qry(m_database);
  qry.DoSQLStatement(query);
  if(qry.GetRecord())
  {
    number = qry.GetColumn(1)->GetAsSLong();
  }
  return (number == 1);
}

// Get SQL to get all the information about a Primary Key constraint
CString
SQLInfoAccess::GetSQLPrimaryKeyConstraintInformation(CString& p_tableName) const
{
  CString query = "SELECT count(*)\n"
    "  FROM dbo.sysobjects tab, dbo.sysobjects con\n"
    " WHERE tab.name = '" + p_tableName + "'\n"
    "   AND tab.Id = con.parent_obj\n"
    "   AND con.xtype = 'PK'\n"
    "   AND con.type  = 'K '";
  return query;
}

// Does the named constraint exist in the database
bool    
SQLInfoAccess::DoesConstraintExist(CString p_constraintName) const
{
  // To be implemented
  return false;
}

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

// Getting the fact that there is only **one** (1) user session in the database
bool    
SQLInfoAccess::GetOnlyOneUserSession()
{
  // No way to get the number of user sessions!
  // So always continue with management functions
  return true;
}

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
// ODBC driver autocommit mode will go wrong!!
void
SQLInfoAccess::DoCommitDMLcommands() const
{
  //   SQLQuery query(m_database);
  //   query.DoSQLStatement("COMMIT WORK");
}

// Create a view from the select code and the name
void 
SQLInfoAccess::DoCreateOrReplaceView(CString p_code,CString p_viewName)
{
  SQLQuery query(m_database);
  query.DoSQLStatement(p_code);
  query.DoSQLStatement("GRANT SELECT ON " + p_viewName + " TO PUBLIC");
}

// Remove a view from the database
void 
SQLInfoAccess::DoDropView(CString p_viewName)
{
  SQLQuery query(m_database);
  query.TryDoSQLStatement("DROP VIEW " + p_viewName);
}

// Does the named view exists in the database
bool
SQLInfoAccess::DoesViewExists(CString& /*p_viewName*/)
{
  // To be implemented
  return true;
}

// Must create temoporary tables runtime 
bool 
SQLInfoAccess::GetMustMakeTemptablesAtRuntime() const
{
  // FALSE: GLOBAL TEMPORARY TABLES IN THE ENGINE
  // TRUE:  TRUE SESSION TEMPTABLES MUST ALWAYS BE CREATED (Informix)
  return false;
}

// Create a temporary table in an optimized manner with the givven index column
void
SQLInfoAccess::DoMakeTemporaryTable(CString& p_tableName,CString& p_content,CString& p_indexColumn) const
{
  SQLQuery query(m_database);

  query.TryDoSQLStatement("TRUNCATE TABLE #" + p_tableName);
  query.TryDoSQLStatement("DROP TABLE #"     + p_tableName);
  CString create = "CREATE TABLE #" + p_tableName + p_content;
  try
  {
    query.DoSQLStatement(create);
    if(p_indexColumn!= "")
    {
      create = "CREATE INDEX " + p_tableName + "_" + p_indexColumn + " ON #" + p_tableName + "(" + p_indexColumn + ")";
      query.DoSQLStatement(create);
    }
  }
  catch(...)
  {
    throw CString("Cannot create a temporary table: " + p_tableName);
  }
}

// Remove a temporary table
void
SQLInfoAccess::DoRemoveTemporaryTable(CString& p_tableName) const
{
  SQLQuery query(m_database);

  // Every error can be ignored. Can still be in use by another user and/or session
  // The table contents will then removed for this session
  query.TryDoSQLStatement("DELETE FROM #"    + p_tableName);
  query.TryDoSQLStatement("TRUNCATE TABLE #" + p_tableName);
  query.TryDoSQLStatement("DROP TABLE #"     + p_tableName);
}

// If the temporary table exists, remove it
void
SQLInfoAccess::DoRemoveTemporaryTableWithCheck(CString& p_tableName) const
{
  int number = 0;

  CString query = "SELECT count(*)\n"
    "  FROM tempdb.dbo.sysobjects\n"
    " WHERE name  = '#" + p_tableName + "'\n"
    "   AND xtype = 'U'";
  SQLQuery qry(m_database);
  qry.DoSQLStatement(query);
  if(qry.GetRecord())
  {
    number = qry.GetColumn(1)->GetAsSLong();
  }
  if(number == 1)
  {
    DoRemoveTemporaryTable(p_tableName);
  }
}

// Create a procedure in the database
void 
SQLInfoAccess::DoMakeProcedure(CString& p_procName,CString p_table,bool /*p_noParameters*/,CString& p_codeBlock)
{
  SQLQuery query(m_database);
  query.DoSQLStatement(p_codeBlock);
  query.DoSQLStatement("GRANT EXECUTE ON " + p_procName + " TO PUBLIC");
}

// Rename a database table 
void 
SQLInfoAccess::DoRenameTable(CString& p_oldName,CString& p_newName) const
{
  // Let op: Zonder 'TABLE' in het statement
  CString rename = "RENAME " + p_oldName + " to " + p_newName;
  SQLQuery query(m_database);
  query.DoSQLStatement(rename);
  DoCommitDDLcommands();
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

// Length of paramters in binding
int 
SQLInfoAccess::GetParameterLength(int p_SQLType) const
{
  int retval;

  switch (p_SQLType)
  {
  case SQL_CHAR:            retval =  2000;      break;
  case SQL_VARCHAR:         retval =  4000;      break;
  case SQL_LONGVARCHAR:     retval = 32000;      break;
  case SQL_DECIMAL:         retval = 32000;      break;
  case SQL_SMALLINT:        retval =     0;      break;
  case SQL_INTEGER:         retval = sizeof(long); break;
  case SQL_REAL:            retval = 0;      break;
  case SQL_DOUBLE:          retval = 0;      break;
  case SQL_FLOAT:           retval = 0;      break;
  case SQL_BINARY:          retval = 0;      break;
  case SQL_VARBINARY:       retval = 0;      break;
  case SQL_LONGVARBINARY:   retval = 0;      break;
  case SQL_DATE:            retval = 0;      break;
  case SQL_TIME:            retval = 0;      break;
  case SQL_TIMESTAMP:       retval = 19;     break;
  case SQL_NUMERIC:         retval = 0;      break;
  case SQL_BIGINT:          retval = 0;      break;
  case SQL_TINYINT:         retval = 0;      break;
  case SQL_BIT:             retval = 0;      break;
  case SQL_INTERVAL_YEAR:
  case SQL_INTERVAL_YEAR_TO_MONTH:
  case SQL_INTERVAL_MONTH:
  case SQL_INTERVAL_DAY:
  case SQL_INTERVAL_DAY_TO_HOUR:
  case SQL_INTERVAL_DAY_TO_MINUTE:
  case SQL_INTERVAL_DAY_TO_SECOND:
  case SQL_INTERVAL_HOUR:
  case SQL_INTERVAL_HOUR_TO_MINUTE:
  case SQL_INTERVAL_HOUR_TO_SECOND:
  case SQL_INTERVAL_MINUTE:
  case SQL_INTERVAL_MINUTE_TO_SECOND:
  case SQL_INTERVAL_SECOND:             retval = 25;      break;
  default:                              retval = 0;       break;
  }
  return retval;
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

// Parametertype for stored procedure for a givven columntype for parameters and return types
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

// Makes a SQL string from a givven string, with all the right quotes
CString 
SQLInfoAccess::GetSQLString(const CString& p_string) const
{
  CString s = p_string;
  s.Replace("'","''");
  CString kwoot = GetQuoteCharacter();
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

// Get the SPL sourcecode for a stored procedure as registered in the database
CString 
SQLInfoAccess::GetSPLSourcecodeFromDatabase(const CString& /*p_owner*/,const CString& /*p_procName*/) const
{
  //   CString sQuery;  
  //   sQuery = "SELECT TEXT from ALL_SOURCE "
  //            "WHERE type = 'FUNCTION' "
  //            "AND LOWER(NAME)  = LOWER('" + p_procName +  "') "
  //            "AND LOWER(OWNER) = LOWER('" + p_owner    + "')";
  //   DBrecordset rs(GeefDatabase());
  //   rs.VoerSqlUitZonderThrow(sQuery);
  //   String sProcBody="CREATE OR REPLACE ";
  //   while (rs.GetRecord())
  //   {
  // 	  sProcBody +=rs.GetCol_LPCSTR(0);
  // 	}
  //   return sProcBody;
  return "";
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

// SPECIALS
// ==========================================================================

// Translate database-errors to a human readable form
CString 
SQLInfoAccess::TranslateErrortext(int p_error,CString p_errorText) const
{
  // Check if we have work to do
  if(p_error == 0)
  {
    return p_errorText;
  }

  CString errorText;
  errorText.Format("ODBC error [%d:%s]",p_error,p_errorText);
  return errorText;
}

