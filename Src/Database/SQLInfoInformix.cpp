////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoInformix.cpp
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
#include "SQLInfoInformix.h"
#include "SQLQuery.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Constructor. 
SQLInfoInformix::SQLInfoInformix(SQLDatabase* p_database)
                :SQLInfoDB(p_database)
{
}

// Destructor. Does nothing
SQLInfoInformix::~SQLInfoInformix()
{
}

// BOOLEANS AND STRINGS
// ===================================================================

// Get DatabaseType
DatabaseType 
SQLInfoInformix::GetDatabaseType() const
{
  return RDBMS_INFORMIX;
}

// The name of the database vendor
CString
SQLInfoInformix::GetDatabaseVendorName() const
{
  return "IBM Informix";
}

// Get the fysical database name
CString 
SQLInfoInformix::GetFysicalDatabaseName() const
{
  // Oops. We have an INFORMIX CLI bug
  // Versions of the INFORMIX CLI above 2.5 reflect in the connection string
  // only the name of the service of the DNS and not the 'real' database
  // That is why we read the database name from the sysmaster database
  // Beware that this only works for and above INFORMIX 7.x.y
  CString query = "SELECT scs_currdb\n"
                  "  FROM sysmaster:syssqlcurses\n"
                  " WHERE scs_sessionid = DBINFO('sessionid')";
  SQLQuery qry(m_database);
  qry.DoSQLStatement(query);
  if(qry.GetRecord())
  {
    return qry.GetColumn(1)->GetAsChar();
  }
  return CString("");
}

// System catalog is stored in uppercase in the database?
bool 
SQLInfoInformix::IsCatalogUpper () const
{
  return false;
}

// Supports database/ODBCdriver comments in sql
bool 
SQLInfoInformix::SupportsDatabaseComments() const
{
  // Does not work anymore from and above version 3.34 of KIT 2.70-TC3
  return false;
}

// Database can defer constraints until the end of a transaction
bool 
SQLInfoInformix::SupportsDeferredConstraints() const
{
  // SET CONSTRAINTS DEFERRED present
  return true;
}

// Database has ORDER BY with an expression, e.g. ORDER BY UPPER(columnname)
// Work-around is "SELECT UPPER(columname) AS something.....ORDER BY something
bool
SQLInfoInformix::SupportsOrderByExpression() const
{
  return false;
}

// Catalogus query for the default value of a table's column
CString 
SQLInfoInformix::GetSQLStringDefaultValue(CString p_tableName,CString p_columnName) const
{   
  return  "select default\n"
          "  from sysdefaults d\n"
          "      ,systables   t\n"
          "      ,syscolumns  c\n"
          " where t.tabid   = d.tabid\n" 
          "   and t.tabid   = c.tabid\n"
          "   and d.colno   = c.colno\n"
          "   and t.tabname = '" + p_tableName   + "'\n"
          "   and c.colname = '" + p_columnName  + "'";   
}

// Keyword for the current date and time
CString 
SQLInfoInformix::GetSystemDateTimeKeyword() const
{
  return "current";
}  

// String for the current date
CString
SQLInfoInformix::GetSystemDateString() const
{
  return "current_date";
}

// If the database does not support the datatype TIME, it can be implemented as a DECIMAL
bool 
SQLInfoInformix::GetTimeIsDecimal() const
{
  // TIJD is geimplementeerd als TIME
  return false;
}

// If the database does not suppoprt the datatype INTERVAL, it can be implemented as a DECIMAL
bool 
SQLInfoInformix::GetIntervalIsDecimal() const
{
  // Interval is in informix geimplenteerd als INTERVAL
  return false;
}

// Get the concatanation operator
CString 
SQLInfoInformix::GetConcatanationOperator() const
{
  return "||";    
}

// Get quote character for strings
CString 
SQLInfoInformix::GetQuoteCharacter() const
{
  return "'";    
}

// Get default NULL for parameterlist input
CString 
SQLInfoInformix::GetDefaultNULL() const
{
  return "";    
}

// Parameter is for INPUT and OUTPUT in parameterlist
CString 
SQLInfoInformix::GetParameterINOUT() const
{
  return "";    
}

// Parameter is for OUTPUT only in parameterlist
CString 
SQLInfoInformix::GetParameterOUT() const
{
  return "";    
}

// Get the datatype of the auditted user (h_user) in a stored procedure
CString 
SQLInfoInformix::GetAuditUserDatatype() const
{
  return "varchar(50)";
}

// Get the datatype of the auditted user (h_user) as variable in a stored-procedure
CString 
SQLInfoInformix::GetAuditUserDatatypeAsVariable() const
{
  return "varchar(50)";
}

// Get datatype of the IDENTITY primary key
CString 
SQLInfoInformix::GetPrimaryKeyType() const
{
  return "serial";
}

// Get datatype for timestamp
CString 
SQLInfoInformix::GetDatetimeYearToSecondType() const
{
  return "datetime year to second";
}

// Separator between two alter-constraints in an alter-table statement
CString 
SQLInfoInformix::GetAlterConstraintSeparator() const
{
  return ",";
}

// Inner Join Keyword
CString 
SQLInfoInformix::GetInnerJoinKeyword() const
{
  return "";
}

// Outer join keyword
CString  
SQLInfoInformix::GetOuterJoinKeyword() const
{
  return "OUTER(";
}

// Inner Join Keyword for use in views.
CString 
SQLInfoInformix::GetViewInnerJoinKeyword() const
{
  return "INNER JOIN";
}

// Outer join keyword for use in views
CString 
SQLInfoInformix::GetViewOuterJoinKeyword() const
{
  return "LEFT OUTER JOIN";
}

// Get the closure for an outer-join
CString 
SQLInfoInformix::GetOuterJoinClosure() const
{
  return ")";
}

// Get the special Outer Join sign for the while-conditions
CString  
SQLInfoInformix::GetOuterJoinSign() const
{
  return "";
}

// Prefix for a parameter in a stored procedure
CString
SQLInfoInformix::GetSPParamPrefix() const
{
  return "";
}

// Get select part to add new record identity to a table
// Can be special column like 'OID'
CString 
SQLInfoInformix::GetIdentityString(CString& /*p_tablename*/,CString /*p_postfix /*="_seq"*/) const
{
  return "0";
}

// Get a query to create a temporary table from a select statement
CString 
SQLInfoInformix::GetSQLCreateTemporaryTable(CString& p_tablename,CString p_select) const
{
  return p_select + " INTO TEMP " + p_tablename + " WITH NO LOG";
}

// Get the query to remove a temporary table indefinetly
// BEWARE: Must be executed with a multi-statement stack!
CString
SQLInfoInformix::GetSQLRemoveTemporaryTable(CString& p_tablename,int& p_number) const
{
  // 'Gewoon' een drop van de tabel doen
  ++p_number;
  return "DROP TABLE " + p_tablename + ";\n";
}

// Get a query to select into a temp table
CString 
SQLInfoInformix::GetSQLSelectIntoTemp(CString& p_tablename,CString& p_select) const
{
  return p_select + " INTO TEMP " + p_tablename;
}

// Replace the Identity column OID by a sequence.nextval
CString 
SQLInfoInformix::GetReplaceColumnOIDbySequence(CString p_columns,CString /*p_tablename*/,CString /*p_postfix /*="_seq"*/) const
{
  // Doet niets in INFORMIX omdat hier een 0 gebruikt wordt.
  return p_columns;
}

// Get the tablespace for the tables
CString 
SQLInfoInformix::GetTablesTablespace(CString p_tablespace /*=""*/) const
{
  if(p_tablespace != "")
  {
    return " IN " + p_tablespace;
  }
  return "";
}

// Get the tablespace for the indexes
// If no default is givven, fallbacks to the DEFAULT tablespace
CString 
SQLInfoInformix::GetIndexTablespace(CString /*p_tablespace /*=""*/) const
{
  return "";
}

// Get the storage name for indici
CString
SQLInfoInformix::GetStorageSpaceNameForIndexes() const
{
  return "";
}

// Get the storage space for temporary tables
CString
SQLInfoInformix::GetStorageSpaceNameForTempTables(CString /*p_tablename*/) const
{
  // Informix gebruikt een DBSPACE met attribuut 'temp'
  return "";
}

// Remove catalog dependencies for stored procedures
// To be run after a 'DROP PROCEDURE' or 'DROP FUNCTION'
CString 
SQLInfoInformix::GetSQLRemoveProcedureDependencies(CString /*p_procname*/) const
{
  return "";
}

// Remove field dependencies for calculated fields (bug in Firebird)
// To be run after a 'DROP TABLE' or an 'ALTER TABLE DROP COLUMN' or 'ALTER TABLE MODIFY COLUMN'
CString 
SQLInfoInformix::GetSQLRemoveFieldDependencies(CString /*p_tablename*/) const
{
  // Niet nodig/bestaat niet in Informix
  return "";
}

// Gets the table definition-form of a primary key
CString
SQLInfoInformix::GetPrimaryKeyDefinition(CString p_tableName,bool p_temporary) const
{
  if(p_temporary)
  {
    return GetPrimaryKeyType() + " PRIMARY KEY\n";
  }
  else
  {
    return GetPrimaryKeyType() + " PRIMARY KEY CONSTRAINT pk_" + p_tableName + "\n";
  }
}

// Get the constraint form of a primary key to be added to a table after creation of that table
CString
SQLInfoInformix::GetPrimaryKeyConstraint(CString p_tablename, bool /*p_temporary*/) const
{
  return "ADD CONSTRAINT PRIMARY KEY(oid) CONSTRAINT pk_" + p_tablename + "\n";
}

// Performance parameters to be added to the database
CString
SQLInfoInformix::GetSQLPerformanceSettings() const
{
  return "";
}

// SQL To set the caching mode of SQL results
CString 
SQLInfoInformix::GetSQLCacheModeSetting(const CString& /*p_mode*/) const
{
  return "";
}

// Needed for storing numbers in stored procedures
CString
SQLInfoInformix::GetSQLNlsNumericCharacters() const
{
  return "";
}

// Gives the statement to alter a table columns' name
CString 
SQLInfoInformix::GetSQLModifyColumnName(CString p_tablename,CString p_oldName,CString p_newName,CString /*p_datatype*/)
{
  return "rename column " + p_tablename + "." + p_oldName + " to " + p_newName;
}

// Gets the maximum length of an SQL statement
unsigned long 
SQLInfoInformix::GetMaxStatementLength() const
{
  // The current limit of the INFORMIX ODBC driver is MAX_UINT16
  // We limit somewhat lower, just to be on the sure side
	return 64000;		
}

// Prefix for an add constraint DDL command in SQLAtlerTableGenerator
CString 
SQLInfoInformix::GetAddConstraintPrefix(CString /*p_constraintName*/) const
{
  return "ADD CONSTRAINT ";
}

// Suffix for an add constraint DDL command in SQLAtlerTableGenerator
CString 
SQLInfoInformix::GetAddConstraintSuffix(CString p_constraintName) const
{
  return "CONSTRAINT " + p_constraintName + "\n";
}

// Get the prefix for a drop constraint DDL command in the SQLAlterTableGenerator
CString 
SQLInfoInformix::GetDropConstraintPrefix() const
{
  return "DROP CONSTRAINT ";
}

// Get the suffix for a drop constraint DDL commando in the SQLAlterTableGenerator
CString 
SQLInfoInformix::GetDropConstraintSuffix() const
{
  return "";
}

// Clause separator between two ADD or DROP clauses in an ALTER TABLE
CString 
SQLInfoInformix::GetAlterTableClauseSeparator()  const
{
  return ",";
}

// Grouping of more than one column possible in an ADD/MODIFY/DROP clause
bool   
SQLInfoInformix::GetClauseGroupingPossible() const
{
  // Kan meerdere ADD (kolomdef ,.... ) doen
  return true;
}

// Gets the prefix needed for altering the datatype of a column in a MODIFY/ALTER
CString 
SQLInfoInformix::GetModifyDatatypePrefix() const
{
  // At the changing of the datatype, just specify the new 
  // SO: MODIFY <columname> <datatype>
  return "";
}

// Code to create a temporary table (qualifier)
CString 
SQLInfoInformix::GetCodeTemporaryTable() const
{
  return "TEMP";
}

// Code to define a table in row-locking mode
CString 
SQLInfoInformix::GetCodeLockModeRow() const
{
  return "LOCK MODE ROW";
}

// Code to create a temporary table with no logging
CString 
SQLInfoInformix::GetCodeTempTableWithNoLog() const
{
  return "WITH NO LOG";
}

// Granting all rights on a table (In a NON-ANSI database)
CString
SQLInfoInformix::GetSQLGrantAllOnTable(CString /*p_tableName*/)
{
  return "";
}

// Code prefix for a select-into-temp
CString
SQLInfoInformix::GetSelectIntoTempClausePrefix(CString /*p_tableName*/) const
{
  return "";
}

// Code suffix for after a select-into-temp
CString
SQLInfoInformix::GetSelectIntoTempClauseSuffix(CString p_tableName) const
{
  return "INTO TEMP " + p_tableName + " WITH NO LOG\n";
}

// Gets the fact if an IF statement needs to be bordered with BEGIN/END
bool
SQLInfoInformix::GetCodeIfStatementBeginEnd() const
{
  // IF THEN ELSE END IF; does not need a BEGIN/END per se.
  return false;
}

// Gets the end of an IF statement
CString 
SQLInfoInformix::GetCodeEndIfStatement() const
{
  return "END IF;\n";
}

// Gets a complete assignment statement.
CString 
SQLInfoInformix::GetAssignmentStatement(const CString& p_destiny,const CString& p_source) const
{
  return "LET " + p_destiny + " = " + p_source + ";";
}

// Get SQL keyword to alter a column in a table
CString 
SQLInfoInformix::GetCodeAlterColumn() const
{
  return "MODIFY ";
}

// Get the code to start a WHILE-loop
CString
SQLInfoInformix::GetStartWhileLoop(CString p_condition) const
{
  return "WHILE " + p_condition + "\n";
}

// Get the code to end a WHILE-loop
CString
SQLInfoInformix::GetEndWhileLoop() const
{
  return "END WHILE;\n";
}

// Gets the fact if a SELECT must be inbetween parenthesis for an assignment
bool 
SQLInfoInformix::GetAssignmentSelectParenthesis() const
{
  // waarde = (SELECT max kenmerk FROM tabel);
  return true;
}

// Gets the UPPER function
CString 
SQLInfoInformix::GetUpperFunction(CString& p_expression) const
{
  return "UPPER(" + p_expression + ")";
}

// Gets the construction for 1 minute ago
CString 
SQLInfoInformix::GetInterval1MinuteAgo() const
{
  return "(CURRENT - INTERVAL (1) MINUTE TO MINUTE)";
}

// Gets the construction / select for generating a new serial identity
CString 
SQLInfoInformix::GetSQLGenerateSerial(CString p_table) const
{
  // Is generated by the database type SERIAL
  return "0";
}

// Gets the construction / select for the resulting effective generated serial
CString 
SQLInfoInformix::GetSQLEffectiveSerial(CString p_identity) const
{
  return "SELECT DBINFO('sqlca.sqlerrd1')\n"
         "  FROM systables WHERE tabid = 1";
}

// Gets the Not-NULL-Value statement of the database
CString 
SQLInfoInformix::GetNVLStatement(CString& p_test,CString& p_isnull) const
{
  return CString("NVL(") + p_test + "," + p_isnull + ")";
}

// Gets the subtransaction commands
CString 
SQLInfoInformix::GetStartSubTransaction(CString p_savepointName) const
{
  return CString("SAVEPOINT ") + p_savepointName;
}

CString 
SQLInfoInformix::GetCommitSubTransaction(CString p_savepointName) const
{
  return CString("COMMIT TRANSACTION ") + p_savepointName;
}

CString 
SQLInfoInformix::GetRollbackSubTransaction(CString p_savepointName) const
{
  return CString("ROLLBACK TO SAVEPOINT ") + p_savepointName;
}

// SQL CATALOG QUERIES
// ==================================================================

// Get SQL to check if a storedprocedure already exists in the database
CString
SQLInfoInformix::GetSQLStoredProcedureExists(CString& p_name) const
{
  CString nameLower(p_name);
  nameLower.MakeLower();
  return    "select count(*)\n"
            "  from sysprocedures\n"
            " where procname='" + nameLower + "'";
}

// Part of a query to select only 1 (one) record
CString 
SQLInfoInformix::GetDualTableName() const
{
  return "systables WHERE tabid=1";
}

// FROM-Part for a query to select only 1 (one) record
CString 
SQLInfoInformix::GetDualClause() const
{
  return " FROM systables WHERE tabid=1";
}

// Gets DEFERRABLE for a constraint (or nothing)
CString
SQLInfoInformix::GetConstraintDeferrable() const
{
  return "";
}

// Defer Constraints until the next COMMIT;
CString 
SQLInfoInformix::GetSQLDeferConstraints() const
{
  return "SET CONSTRAINTS ALL DEFERRED";
}

// Reset constraints back to immediate
CString 
SQLInfoInformix::GetSQLConstraintsImmediate() const
{
  return "SET CONSTRAINTS ALL IMMEDIATE";
}

// Get SQL to check if a table already exists in the database
CString 
SQLInfoInformix::GetSQLTableExists(CString p_tablename) const
{
  CString lowerName(p_tablename);
  lowerName.MakeLower();
  CString query = "SELECT count(*)\n"
                  "  FROM systables\n"
                  " WHERE tabname = '" + lowerName + "'";
  return query;
}

// Get SQL to select all columns of a table from the catalog
CString 
SQLInfoInformix::GetSQLGetColumns(CString& /*p_user*/,CString& p_tableName) const
{
  CString lowerName(p_tableName);
  lowerName.MakeLower();
  CString select = "SELECT col.colname\n"
                   "      ,col.colno\n"
                   "      ,col.coltype\n"
                   "      ,col.collength\n"
                   "  FROM systables tab\n"
                   "      ,syscolumns col\n"
                   " WHERE tab.tabname = '" + lowerName + "'\n"
                   "   AND col.tabid   = tab.tabid\n"
                   " ORDER BY colno";
  return select;
}

// Get SQL to select all constraints on a table from the catalog
CString 
SQLInfoInformix::GetSQLGetConstraintsForTable(CString& p_tableName) const
{
  CString lowerName(p_tableName);
  lowerName.MakeLower();
  CString contabel = "SELECT con.constrname "
		 	              "      ,con.tabid "
			              "      ,con.constrid "
                    "  FROM sysconstraints con "
		                "      ,systables tab "
		                " WHERE tab.tabid   = con.tabid "
		                "   AND con.constrtype in ('C', 'U') "
                    "   AND tab.tabname = '" + lowerName + "'";
  return contabel;
}

// Get SQL to read all indici for a table
CString 
SQLInfoInformix::GetSQLTableIndexes(CString& /*p_user*/,CString& p_tableName) const
{
  CString lowerName(p_tableName);
  lowerName.MakeLower();

  // Reads all current indici in the database in a list
  // So we can figure out if an index need to be generated
	CString query = "select idx.idxname\n"
							    "      ,idx.tabid\n"      // Table
                  "      ,idx.idxtype\n"		 // U=Uniek, D=Duplicates
							    "      ,part1,part2, part3, part4, part5, part6, part7, part8\n"
							    "      ,part9,part10,part11,part12,part13,part14,part15,part16\n"
					        "  from sysindexes idx\n"
					        "      ,systables  tab\n"
                  " where tab.tabid = idx.tabid\n"
						      "   and tab.tabname ='" + lowerName + "'\n " 
                  "   and idx.idxname not matches ' [0-9]*'";
  return query;
}

// Get SQL to read the referential constaints from the catalog
CString 
SQLInfoInformix::GetSQLTableReferences(CString& p_tablename) const
{
  CString lowerName(p_tablename);
  lowerName.MakeLower();
	// EH Reads all current references
  CString query = "select con.constrname "
		              "      ,con.tabid "
                  "      ,con.constrid "
                  "  from sysconstraints con "
                  "      ,systables tab "
                  " where tab.tabid      = con.tabid "
                  "   and con.constrtype = 'R' "
                  "   and tab.tabname    = '" + lowerName + "'";
  return query;
}

// Get the SQL Query to create a synonym
CString 
SQLInfoInformix::GetSQLMakeSynonym(CString& /*p_objectName*/) const
{
  return "";
}

// Get SQL to drop the synonym
CString 
SQLInfoInformix::GetSQLDropSynonym(CString& /*p_objectName*/) const
{
  return "";
}

// Create a sequence in the database
void 
SQLInfoInformix::DoCreateSequence(CString& /*p_sequenceName*/,int /*p_startpos*/) 
{
  // INFORMIX uses SERIAL's in stead of sequences
}

// Remove a sequence from the database
void
SQLInfoInformix::DoRemoveSequence(CString& /*p_sequenceName*/) const
{
  // INFORMIX uses SERIAL's in stead of sequences
}

// Re-Creates a sequence in a database from the OID column
void 
SQLInfoInformix::DoCreateNextSequence(const CString& /*p_tableName*/,CString p_postfix /*="_seq"*/)
{
}

// Remove a stored procedure from the database
void
SQLInfoInformix::DoRemoveProcedure(CString& p_procedureName) const
{
  SQLQuery sql(m_database);
  sql.TryDoSQLStatement("DROP PROCEDURE " + p_procedureName);
}

// Gets the SQL for the rights on the sequence
CString 
SQLInfoInformix::GetSQLSequenceRights(const CString& /*p_tableName*/,CString /*p_postfix /*="_seq"*/) const
{
  // INFORMIX uses SERIAL's in stead of sequences
  return "";
}

// Get SQL for your session and controling terminal
CString
SQLInfoInformix::GetSQLSessionAndTerminal() const
{
  CString query = "SELECT sid\n"
                  "      ,tty\n"
                  "  FROM sysmaster:syssessions\n"
                  " WHERE sid =\n"
                  "     ( SELECT dbinfo('sessionid')\n"
                  "         FROM systables\n"
                  "        WHERE tabid=1)";
  return query;
}

// Get SQL to check if sessionnumber exists
CString 
SQLInfoInformix::GetSQLSessionExists(CString sessieId) const
{
  return "SELECT COUNT(*)\n"
         "  FROM sysmaster:syssessions\n"
         " WHERE sid = " + sessieId;
}

// Get SQL for unique session ID
CString 
SQLInfoInformix::GetSQLUniqueSessionId(const CString& p_databaseName,const CString& /*p_sessionTable*/) const
{
  // INFORMIX has sessions on more than one database in the sysmaster table
  // so we must limit on the current database that's in use
  return "SELECT UNIQUE(odb_sessionid)\n"
         "  FROM sysmaster:sysopendb\n"
         " WHERE odb_dbname = '" + p_databaseName + "'";
}

// Get SQL for searching a session
CString 
SQLInfoInformix::GetSQLSearchSession(const CString& p_databaseName,const CString& p_sessionTable) const
{
  return "SELECT  sid\n"
         "       ,username\n"
         "       ,tty\n"
         "  FROM  sysmaster:syssessions\n"
         " WHERE  sid IN\n"
         "      ( SELECT odb_sessionid\n"
         "          FROM sysmaster:sysopendb\n"
		     "         WHERE odb_dbname = '" + p_databaseName + "'\n"
         "           AND NOT odb_sessionid IN\n"
         "             ( SELECT sessie_nr\n"
         "                 FROM " + p_sessionTable +"))";
}

// See if a column exsists within a table
bool   
SQLInfoInformix::DoesColumnExistsInTable(CString& /*p_owner*/,CString& p_tableName,CString& p_column) const
{
  int number = 0;

  CString query = "SELECT count(*)\n"
                  "  FROM systables  tab\n"
                  "      ,syscolumns col\n"
                  " WHERE tab.tabid = col.tabid\n"
                  "   AND tab.tabname = '" + p_tableName + "'\n"
                  "   AND col.colname = '" + p_column + "'";
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
SQLInfoInformix::GetSQLPrimaryKeyConstraintInformation(CString& p_tableName) const
{
  CString query = "SELECT count(*)\n"
                  "  FROM systables t\n"
                  "      ,sysconstraints c\n"
                  " WHERE t.tabid      = c.tabid\n"
                  "   AND t.tabname    = '" + p_tableName + "'\n"
                  "   AND c.constrtype = 'P'";
  return query;
}

// Does the named constraint exist in the database
bool    
SQLInfoInformix::DoesConstraintExist(CString p_constraintName) const
{
  CString constraint(p_constraintName);
  constraint.MakeLower();

  SQLQuery qry(m_database);
  qry.DoSQLStatement("SELECT 1\n"
                     "  FROM sysconstraints\n"
                     " WHERE constrname = '" + p_constraintName + "'\n");
  return qry.GetRecord();
}

// Gets the lock-table query
CString 
SQLInfoInformix::GetSQLLockTable(CString& p_tableName,bool p_exclusive) const
{
  CString query = "LOCK TABLE " + p_tableName + " IN "  ;
  query += p_exclusive ? "EXCLUSIVE" : "SHARE";
  query += " MODE";
  return query;
}

// Get query to optimize the table statistics
CString 
SQLInfoInformix::GetSQLOptimizeTable(CString& /*p_owner*/,CString& p_tableName,int& p_number)
{
  CString optim;
  optim = "UPDATE STATISTICS LOW  FOR TABLE " + p_tableName + " DROP DISTRIBUTIONS;\n"
          "UPDATE STATISTICS HIGH FOR TABLE " + p_tableName + ";\n";
  p_number += 2;
  return optim;
}

// Getting the fact that there is only **one** (1) user session in the database
bool
SQLInfoInformix::GetOnlyOneUserSession()
{
  // Yet to implement
  return true;
}

// SQL DDL ACTIONS
// ====================================================================

// Do the commit for the DDL commands in the catalog
void 
SQLInfoInformix::DoCommitDDLcommands() const
{
  // Does NOTHING in INFOMRIX and cannot do something
  // Commit follows from a BEGIN/COMMIT transaction by AUTOCOMMIT from the driver
}

// Do the commit for the DML commands in the database
// ODBC driver autocommit mode will go wrong!!
void
SQLInfoInformix::DoCommitDMLcommands() const
{
  // Does NOTHING in INFOMRIX and cannot do something
  // Commit follows from a BEGIN/COMMIT transaction by AUTOCOMMIT from the driver
}

// Create a view from the select code and the name
void 
SQLInfoInformix::DoCreateOrReplaceView(CString p_code,CString p_viewName)
{
  SQLQuery sql(m_database);
  sql.DoSQLStatement(p_code);
  sql.DoSQLStatement("GRANT ALL ON " + p_viewName + " TO " + GetGrantedUsers());
}

// Remove a view from the database
void 
SQLInfoInformix::DoDropView(CString p_viewName)
{
  SQLQuery sql(m_database);
  sql.TryDoSQLStatement("DROP VIEW " + p_viewName);
}

// Does the named view exists in the database
bool
SQLInfoInformix::DoesViewExists(CString& p_viewName)
{
  CString lowerName(p_viewName);
  lowerName.MakeLower();

  CString query = "SELECT count(*)\n"
                  "  FROM sysviews\n"
                  " WHERE viewname = '" + lowerName + "'";
  SQLQuery sql(m_database);
  sql.DoSQLStatement(query);
  if(sql.GetRecord())
  {
    return sql.GetColumn(1)->GetAsSLong() > 0;
  }
  return false;
}

// Must create temoporary tables runtime 
bool 
SQLInfoInformix::GetMustMakeTemptablesAtRuntime() const
{
  // FALSE: GLOBAL TEMPORARY TABLES IN THE ENGINE
  // TRUE:  TRUE SESSION TEMPTABLES MUST ALWAYS BE CREATED (Informix)
  return true;
}

// Create a temporary table in an optimized manner with the givven index column
void
SQLInfoInformix::DoMakeTemporaryTable(CString& p_tableName,CString& p_content,CString& p_indexColumn) const
{
  SQLQuery sql(m_database);
  sql.TryDoSQLStatement("DROP TABLE " + p_tableName);
  CString create = "CREATE TEMP TABLE " + p_tableName + " " + p_content;
  try
  {
    sql.DoSQLStatement(create);
    if(p_indexColumn != "")
    {
      create = "CREATE INDEX " + p_tableName + "_" + p_indexColumn + " ON " + p_tableName + "(" + p_indexColumn + ")";
      sql.DoSQLStatement(create);
    }
  }
  catch(...)
  {
    throw CString("Cannot make temporary table: " + p_tableName);
  }
}

// Remove a temporary table
void
SQLInfoInformix::DoRemoveTemporaryTable(CString& p_tableName) const
{
  SQLQuery sql(m_database);
  sql.TryDoSQLStatement("DROP TABLE " + p_tableName);
}

// If the temporary table exists, remove it
void
SQLInfoInformix::DoRemoveTemporaryTableWithCheck(CString& p_tableName) const
{
  CString tabelName(p_tableName);
  int number = 0;
  tabelName.MakeLower();

  CString query = "SELECT count(*)\n"
                  "  FROM systables sys\n"
                  " WHERE tabname = '" + tabelName + "'";
  SQLQuery sql(m_database);
  sql.DoSQLStatement(query);
  if(sql.GetRecord())
  {
    number = sql.GetColumn(1)->GetAsSLong();
  }
  // IF the table is NOT in the system catalog, THEN we can try to drop it
  // If it was in the systables, it's persistent, so we can NOT drop it!!!
  if(number == 0)
  {
    DoRemoveTemporaryTable(tabelName);
  }
}

// Create a procedure in the database
void 
SQLInfoInformix::DoMakeProcedure(CString& p_procName,CString /*p_table*/,bool /*p_noParameters*/,CString& p_codeBlock)
{
  SQLQuery sql(m_database);
  sql.DoSQLStatement(p_codeBlock);
  sql.DoSQLStatement("GRANT EXECUTE ON " + p_procName + " TO " + GetGrantedUsers());
}

// Rename a database table 
void
SQLInfoInformix::DoRenameTable(CString& p_oldName,CString& p_newName) const
{
  SQLQuery sql(m_database);
  // Beware: 'TABLE' keyword in the statement
  CString rename = "rename table " + p_oldName + " to " + p_newName;
  sql.DoSQLStatement(rename);
}

// PERSISTENT-STORED MODULES (SPL / PL/SQL)
// =====================================================================

// Get the user error text from the database
CString 
SQLInfoInformix::GetUserErrorText(CString& p_procName) const
{
  (void)p_procName;   // Not in informix.
  return "";
}

// Get assignment to a variable in SPL
CString 
SQLInfoInformix::GetSPLAssignment(CString p_variable) const
{
  return "LET " + p_variable + " = ";
}

// Get the start of a SPL While loop
CString 
SQLInfoInformix::GetSPLStartWhileLoop(CString p_condition) const
{
  return "WHILE " + p_condition + "\n";
}

// Get the end of a SPL while loop
CString 
SQLInfoInformix::GetSPLEndWhileLoop() const
{
  return "END WHILE;\n";
}

// Get stored procedure call
CString 
SQLInfoInformix::GetSQLSPLCall(CString p_procName) const
{
  return "execute procedure " + p_procName;
}

// Length of paramters in binding
int 
SQLInfoInformix::GetParameterLength(int p_SQLType) const
{
  int retval;

  switch (p_SQLType)
  {
    case SQL_CHAR:              retval = 32000;    break;
    case SQL_VARCHAR:           retval = 32000;    break;
    case SQL_LONGVARCHAR:       retval = 32000;    break;
    case SQL_DECIMAL:           retval = 32000;    break;
	  case SQL_SMALLINT:          retval = 0;    break;
	  case SQL_INTEGER:           retval = 0;    break;
	  case SQL_REAL:              retval = 0;    break;
	  case SQL_DOUBLE:            retval = 0;    break;
	  case SQL_FLOAT:             retval = 0;    break;
	  case SQL_BINARY:            retval = 0;    break;
	  case SQL_VARBINARY:         retval = 0;    break;
	  case SQL_LONGVARBINARY:     retval = 0;    break;
	  case SQL_DATE:              retval = 0;    break;
    case SQL_TIME:              retval = 0;    break;
    case SQL_TIMESTAMP:         retval = 19;   break;
	  case SQL_NUMERIC:           retval = 0;    break;
	  case SQL_BIGINT:            retval = 0;    break;
	  case SQL_TINYINT:           retval = 0;    break;
	  case SQL_BIT:               retval = 0;    break;
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
    case SQL_INTERVAL_SECOND:           retval = 25;    break;
    default:                            retval = 0;     break;
  }
  return retval;
}

// Build a parameter list for calling a stored procedure
CString 
SQLInfoInformix::GetBuildedParameterList(size_t p_numOfParameters) const
{
  // Stands for the '?' binding parameters in a stored-procedire
  // ALWAYS an ellipsis, even if there are no parameters!
  CString strParamLijst = "(";
  for (size_t i = 0; i < p_numOfParameters; i++)
  {
    if(i) 
    {
      strParamLijst += ",";
    }
    strParamLijst += "?";
  }
  strParamLijst += ")";

  return strParamLijst;
}

// Parametertype for stored procedure for a givven columntype for parameters and return types
CString
SQLInfoInformix::GetParameterType(CString& p_type) const
{
  return p_type;
}

// GENERAL SQL ACTIONS
// =================================================================

// Makes a SQL string from a givven string, with all the right quotes
CString 
SQLInfoInformix::GetSQLString(const CString& p_string) const
{
  CString str = p_string;
  str.Replace("'","''");
  CString kwoot = GetQuoteCharacter();
  return kwoot + str + kwoot;
}

// Get date string in engine format
CString 
SQLInfoInformix::GetSQLDateString(int p_year,int p_month,int p_day) const
{
  // Informix used to be dependend on the DBFORMAT parameter
  // This form is independent of it's setting!
  CString dateString;
  dateString.Format("DATETIME(%04d-%02d-%02d) YEAR TO DAY",p_year,p_month,p_day);
  return dateString;
}

// Get time string in database engine format
CString 
SQLInfoInformix::GetSQLTimeString(int p_hour,int p_minute,int p_second) const
{
  CString retval;
  retval.Format("DATETIME (%02d:%02d:%02d) HOUR TO SECOND",p_hour,p_minute,p_second);
  return retval;
}

// Get date-time string in database engine format
CString 
SQLInfoInformix::GetSQLDateTimeString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  // Informix used to be dependend on the DBFORMAT parameter
  // This form is independent of it's setting!
  CString string;
  string.Format("DATETIME(%04d-%02d-%02d %02d:%02d:%02d) YEAR TO SECOND"
                ,p_day,p_month,p_year
                ,p_hour,p_minute,p_second);
  return string;
}

// Get date-time bound parameter string in database format
CString 
SQLInfoInformix::GetSQLDateTimeBoundString() const
{
  return "TO_DATE(?,'%d-%m-%Y %H:%M:%S')";
}

// Stripped data for the parameter binding
CString
SQLInfoInformix::GetSQLDateTimeStrippedString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  CString string;
  string.Format("%04d-%02d-%02d %02d:%02d:%02d"
                ,p_day,p_month,p_year
                ,p_hour,p_minute,p_second);
  return string;
}

// Get the SPL sourcecode for a stored procedure as registered in the database
CString 
SQLInfoInformix::GetSPLSourcecodeFromDatabase(const CString& /*p_owner*/,const CString& p_procName) const
{
  CString sQuery;  
  CString sName(p_procName);
  sName.MakeLower();
  sQuery = "SELECT sbody.data\n"
           "  FROM sysprocbody sbody\n"
           "      ,sysprocedures sproc\n"
	         " WHERE sbody.procid   = sproc.procid\n"
           "   AND sproc.procname = '" + sName + "'\n"
           "   AND datakey        = 'T'";
  SQLQuery sql(m_database);
  sql.TryDoSQLStatement(sQuery);
  CString sProcBody;
  while (sql.GetRecord())
  {
	  sProcBody = sql.GetColumn(1)->GetAsChar();
	}
  return sProcBody;
}

// Get the SPL datatype for integer
CString 
SQLInfoInformix::GetSPLIntegerType() const
{
  return "integer";
}

// Get the SPL datatype for a decimal
CString 
SQLInfoInformix::GetSPLDecimalType() const
{
  return "decimal";
}

// Get the SPL declaration for a cursor
CString 
SQLInfoInformix::GetSPLCursorDeclaratie(CString& p_variableName,CString& p_query) const
{
  return "CURSOR " + p_variableName + " IS " + p_query + ";";
}

// Get the SPL cursor found row parameter
CString 
SQLInfoInformix::GetSPLCursorFound(CString& /*p_cursorName*/) const
{
  // TODO: To be implemented
  return "";
}

// Get the SPL cursor row-count variable
CString 
SQLInfoInformix::GetSPLCursorRowCount(CString& /*p_variable*/) const
{
  // TODO: To be implemented
  return "";
}

// Get the SPL datatype for a declaration of a row-variable
CString 
SQLInfoInformix::GetSPLCursorRowDeclaration(CString& /*p_cursorName*/,CString& /*p_variableName*/) const
{
  // TODO: To be implemented
  return "";
}

CString 
SQLInfoInformix::GetSPLFetchCursorIntoVariables(CString               p_cursorName
                                               ,CString             /*p_variableName*/
                                               ,std::vector<CString>& p_columnNames
                                               ,std::vector<CString>& p_variableNames) const
{
  // TODO: CHeck
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
SQLInfoInformix::GetSPLFetchCursorIntoRowVariable(CString& p_cursorName,CString p_variableName) const
{ 
  return "FETCH " + p_cursorName + " INTO " + p_variableName+ ";";
}

// Get the SPL no-data exception clause
CString 
SQLInfoInformix::GetSPLNoDataFoundExceptionClause() const
{
  // TODO: Check
  return "WHEN NO_DATA THEN";
}

// Get the SPL form of raising an exception
CString 
SQLInfoInformix::GetSPLRaiseException(CString p_exceptionName) const
{
  // TODO: Check
  return "RAISE " + p_exceptionName + ";";
}

// Get the fact that the SPL has server functions that return more than 1 value
bool    
SQLInfoInformix::GetSPLServerFunctionsWithReturnValues() const
{
  return true;
}

// SPECIALS
// ==========================================================================

// Translate database-errors to a human readable form
CString 
SQLInfoInformix::TranslateErrortext(int p_error,CString p_errorText) const
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

