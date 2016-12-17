////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoOracle.cpp
//
// Copyright (c) 1998-2016 ir. W.E. Huisman
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
// Last Revision:   14-12-2016
// Version number:  1.3.0
//
#include "stdafx.h"
#include "SQLInfoOracle.h"
#include "SQLQuery.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Constructor
SQLInfoOracle::SQLInfoOracle(SQLDatabase* p_database)
              :SQLInfoDB(p_database)
{
}

// Destructor. Does nothing
SQLInfoOracle::~SQLInfoOracle()
{
}

// Get the database type
DatabaseType 
SQLInfoOracle::GetDatabaseType() const
{
  return RDBMS_ORACLE;
}

// BOOLEANS EN STRINGS
// ====================================================================

// The name of the database vendor
CString 
SQLInfoOracle::GetDatabaseVendorName() const
{
  return "Oracle";
}

// Get the fysical database name
CString 
SQLInfoOracle::GetFysicalDatabaseName() const
{
  // Oops. We have a Oracle bug. Name is effectivly one of
  // 1) the name in TNSNAMES.ORA file
  // 2) the DNS host name (as mirroring for the database)
  // 3) The SUN NIS name
  // 4) The DCE CDS name 
  // and **NOT** the database name from the server.
  // The names in 1) to 4) could be the same as the databasename 
  // but there are a great number of situations where this is not the case.

  CString query = "SELECT SYS_CONTEXT('USERENV','DB_NAME')\n"
                  "  FROM DUAL";
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
SQLInfoOracle::IsCatalogUpper() const
{
  return true;
}

// System catalog supports full ISO schemas (same tables per schema)
bool
SQLInfoOracle::GetUnderstandsSchemas() const
{
  return true;
}

// Supports database/ODBCdriver comments in sql
bool 
SQLInfoOracle::SupportsDatabaseComments() const
{
  return false;
}

// Database can defer constraints until the end of a transaction
bool 
SQLInfoOracle::SupportsDeferredConstraints() const
{
  // SET CONSTRAINTS DEFERRED is supported by database
  return true;
}

// Database has ORDER BY with an expression, e.g. ORDER BY UPPER(columnname)
// Work-around is "SELECT UPPER(columname) AS something.....ORDER BY something
bool 
SQLInfoOracle::SupportsOrderByExpression() const
{
  return true;
}

// Catalogus query for the default value of a table's column
CString 
SQLInfoOracle::GetSQLStringDefaultValue(CString p_tableName,CString p_columnName) const
{
  p_tableName .MakeUpper();
  p_columnName.MakeUpper();

  CString query = "SELECT data_default\n"
                  "  FROM all_tab_columns\n"
                  " WHERE table_name  = '" + p_tableName + "'\n"
                  "   AND column_name = '" + p_columnName + "'";                 
  return query;
}

// Keyword for the current date and time
CString 
SQLInfoOracle::GetSystemDateTimeKeyword() const
{
  return "SYSDATE";
}

// String for the current date
CString 
SQLInfoOracle::GetSystemDateString() const
{
  CString today = "TRUNC(SYSDATE)";
  return today;
}

// If the database does not support the datatype TIME, it can be implemented as a DECIMAL
bool 
SQLInfoOracle::GetTimeIsDecimal() const
{
  // Time can be implemented as DECIMAL(17,16)
  return true;
}

// If the database does not support the datatype INTERVAL, it can be implemented as a DECIMAL
bool 
SQLInfoOracle::GetIntervalIsDecimal() const
{
  // INTERVAL can be implemented as DECIMAL
  return true;
}

// Get the concatenation operator
CString 
SQLInfoOracle::GetConcatanationOperator() const
{
  return "||";
}

// Get quote character for strings
CString 
SQLInfoOracle::GetQuoteCharacter() const
{
  return "'";
}

// Get default NULL for parameter list input
CString 
SQLInfoOracle::GetDefaultNULL() const
{
  return " DEFAULT NULL ";
}

// Parameter is for INPUT and OUTPUT in parameter list
CString 
SQLInfoOracle::GetParameterINOUT() const
{
  return "IN OUT ";
}

// Parameter is for OUTPUT only in parameter list
CString 
SQLInfoOracle::GetParameterOUT() const
{
  return "OUT ";
}

// Get the datatype of the auditted user (h_user) in a stored procedure
CString 
SQLInfoOracle::GetAuditUserDatatype() const
{
  return "VARCHAR2";
}

// Get the datatype of the auditted user (h_user) as variable in a stored-procedure
CString 
SQLInfoOracle::GetAuditUserDatatypeAsVariable() const
{
  return "VARCHAR2(50)";
}

// Get datatype of the IDENTITY primary key
CString 
SQLInfoOracle::GetPrimaryKeyType() const
{
  return "INTEGER";
}

// Get datatype for Moment
CString 
SQLInfoOracle::GetDatetimeYearToSecondType() const
{
  return "DATE";
}

// Separator between two alter-constraints in an alter-table statement
CString 
SQLInfoOracle::GetAlterConstraintSeparator() const
{
  // In Oracle two alter constraints are not separated
  return "";
}

// Inner Join Keyword
CString 
SQLInfoOracle::GetInnerJoinKeyword() const
{
  // Before Oracle 9 it's empty
  // After Oracle 9 it's just regular "INNER JOIN"
  return "INNER JOIN";
}

// Outer join keyword
CString 
SQLInfoOracle::GetOuterJoinKeyword() const
{
  // Before Oracle 9 it's empty
  // After Oracle 9 its just regular "LEFT OUTER JOIN"
  return "LEFT OUTER JOIN";
}

// Inner Join Keyword for use in views.
CString 
SQLInfoOracle::GetViewInnerJoinKeyword() const
{
  return "INNER JOIN";
}

// Outer join keyword for use in views
CString 
SQLInfoOracle::GetViewOuterJoinKeyword() const
{
  return "LEFT OUTER JOIN";
}

// Get the closure for an outer-join
CString 
SQLInfoOracle::GetOuterJoinClosure() const
{
  return "";
}

// Get the special Outer Join sign for the while-conditions
CString 
SQLInfoOracle::GetOuterJoinSign() const
{
  return "(+)";
}

// Prefix for a parameter in a stored procedure
CString  
SQLInfoOracle::GetSPParamPrefix() const
{
  return "";
}

// Get select part to add new record identity to a table
// Can be special column like 'OID'
CString 
SQLInfoOracle::GetIdentityString(CString& p_tablename,CString p_postfix /*="_seq"*/) const
{
  return p_tablename + p_postfix + ".nextval";
}

// Get a query to create a temporary table from a select statement
CString 
SQLInfoOracle::GetSQLCreateTemporaryTable(CString& p_tablename,CString p_select) const
{
  return "CREATE GLOBAL TEMPORARY TABLE " + p_tablename + "\nAS " + p_select + 
         "ON COMMIT PRESERVE ROWS";
}

// Get the query to remove a temporary table indefinite
// BEWARE: Must be executed with a multi-statement stack!
CString 
SQLInfoOracle::GetSQLRemoveTemporaryTable(CString& p_tablename,int& p_number) const
{
  // Works only in Oracle in this way. If not an error will follow for the
  // fact that the temporary table is still in use. The table definition 
  // remains in the table catalog and can be different from and blocking
  // newly defined temporary tables under the same name
  p_number += 3;
  return "DELETE FROM "    + p_tablename + ";\n"
         "TRUNCATE TABLE " + p_tablename + ";\n"
         "DROP TABLE "     + p_tablename + ";\n";
}

// Get a query to select into a temp table
CString 
SQLInfoOracle::GetSQLSelectIntoTemp(CString& p_tablename,CString& p_select) const
{
   return "INSERT INTO " + p_tablename + "\n" + p_select + ";\n";
}

// Replace the Identity column OID by a sequence.nextval
CString 
SQLInfoOracle::GetReplaceColumnOIDbySequence(CString p_columns,CString p_tablename,CString p_postfix /*="_seq"*/) const
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

// Remove catalog dependencies for stored procedures
// To be run after a 'DROP PROCEDURE' or 'DROP FUNCTION'
CString 
SQLInfoOracle::GetSQLRemoveProcedureDependencies(CString p_procname) const
{
  // Is implicitly done by the database engine in Oracle
  return "";
}

// Remove field dependencies for calculated fields (bug in Firebird)
// To be run after a 'DROP TABLE' or an 'ALTER TABLE DROP COLUMN' or 'ALTER TABLE MODIFY COLUMN'
CString 
SQLInfoOracle::GetSQLRemoveFieldDependencies(CString p_tablename) const
{
  // Not necessary in Oracle
  return "";
}

// Gets the table definition-form of a primary key
CString 
SQLInfoOracle::GetPrimaryKeyDefinition(CString p_schema,CString p_tableName,bool /*p_temporary*/) const
{
  // The primary key constraint is not directly generated after the column
  // to ensure it will use the named index in the correct tablespace
  // Otherwise the index name and tablespace cannot be defined and will be auto-generated
  return GetPrimaryKeyType() + " NOT NULL CONSTRAINT pk_" + p_tableName + " PRIMARY KEY\n";
}

// Get the constraint form of a primary key to be added to a table after creation of that table
CString 
SQLInfoOracle::GetPrimaryKeyConstraint(CString p_schema,CString p_tablename,CString p_primary) const
{
  return "ALTER TABLE " + p_schema + "." + p_tablename + "\n"
         "  ADD CONSTRAINT pk_" + p_tablename + "\n"
         "      PRIMARY KEY (" + p_primary + ")";
}

// Get the sql to add a foreign key to a table
CString 
SQLInfoOracle::GetSQLForeignKeyConstraint(DBForeign& p_foreign) const
{
  // Construct the correct table names
  CString table  (p_foreign.m_tablename);
  CString primary(p_foreign.m_primaryTable);
  if(!p_foreign.m_schema.IsEmpty())
  {
    table   = p_foreign.m_schema + "." + table;
    primary = p_foreign.m_schema + "." + primary;
  }

  // The base foreign key command
  CString query = "ALTER TABLE " + table + "\n"
                  "  ADD CONSTRAINT " + p_foreign.m_constraintname + "\n"
                  "      FOREIGN KEY (" + p_foreign.m_column + ")\n"
                  "      REFERENCES " + primary + "(" + p_foreign.m_primaryColumn + ")";
  // Add all relevant options
  if(p_foreign.m_deferrable)
  {
    query += "\n      DEFERRABLE";
  }
  if(p_foreign.m_initiallyDeffered)
  {
    query += "\n      INITIALLY DEFERRED";
  }
  switch(p_foreign.m_deleteRule)
  {
    case 1: query += "\n      ON DELETE CASCADE";     break;
    case 2: query += "\n      ON DELETE SET NULL";    break;
    default:// In essence: ON DELETE RESTRICT, but that's already the default
    case 0: break;
  }
  return query;
}

// Get the sql (if possible) to change the foreign key constraint
CString 
SQLInfoOracle::GetSQLAlterForeignKey(DBForeign& p_origin,DBForeign& p_requested) const
{
  // Construct the correct tablenames
  CString table(p_origin.m_tablename);
  if(!p_origin.m_schema.IsEmpty())
  {
    table = p_origin.m_schema + "." + table;
  }

  // The base foreign key command
  CString query = "ALTER TABLE " + table + "\n"
                  "MODIFY CONSTRAINT " + p_origin.m_constraintname + "\n";

  // Add all relevant options
  if(p_origin.m_deferrable != p_requested.m_deferrable)
  {
    query.AppendFormat("\n      %sDEFERRABLE",p_requested.m_deferrable == 0 ? "NOT " : "");
  }
  if(p_origin.m_initiallyDeffered != p_requested.m_initiallyDeffered)
  {
    query += "\n      INITIALLY ";
    query += p_requested.m_initiallyDeffered ? "DEFERRED" : "IMMEDIATE";
  }
  if(p_origin.m_deleteRule != p_requested.m_deleteRule)
  {
    switch(p_requested.m_deleteRule)
    {
      case 1: query += "\n      ON DELETE CASCADE";     break;
      case 2: query += "\n      ON DELETE SET NULL";    break;
      default:// In essence: ON DELETE RESTRICT, 
              // But Oracle cannot do that, so return an empty string and drop the constraint
      case 0: query.Empty();
    }
  }
  return query;
}

// Performance parameters to be added to the database
CString 
SQLInfoOracle::GetSQLPerformanceSettings() const
{
  // To run optimized queries on views with outer-joins and filters on the view
  // the following parameter is of live-saving essence!
  return "ALTER SESSION SET \"_push_join_predicate\" = true";
}

// SQL To set the caching mode of SQL results
CString
SQLInfoOracle::GetSQLCacheModeSetting(const CString& p_mode) const
{
  // Check mode parameter for correct values
  CString query;
  if(p_mode.CompareNoCase("manual") == 0 ||
     p_mode.CompareNoCase("force")  == 0 ||
     p_mode.CompareNoCase("auto")   == 0  )
  {
    query = "ALTER SESSION SET RESULT_CACHE_MODE = " + p_mode;
  }
  return query;
}

// Needed for storing numbers in stored procedures
CString 
SQLInfoOracle::GetSQLNlsNumericCharacters() const
{
  return "ALTER SESSION SET NLS_NUMERIC_CHARACTERS='.,'";
}

// Gives the statement to alter a table columns' name
CString 
SQLInfoOracle::GetSQLModifyColumnName(CString p_tablename,CString p_oldName,CString p_newName,CString /*p_datatype*/)
{
  // datatype not used in Oracle
  return 
    "ALTER  TABLE  " + p_tablename + "\n"
    "RENAME COLUMN " + p_oldName   + " TO " + p_newName + "\n";
}

// Gets the maximum length of an SQL statement
unsigned long 
SQLInfoOracle::GetMaxStatementLength() const
{
  return 0;		// No limit in Oracle
}

// Gets the prefix needed for altering the datatype of a column in a MODIFY/ALTER
CString 
SQLInfoOracle::GetModifyDatatypePrefix() const
{
  // At the changing of the datatype, just specify the new 
  // SO: MODIFY <columname> <datatype>
  return "";
}

// Code to create a temporary table (qualifier)
CString 
SQLInfoOracle::GetCodeTemporaryTable() const
{
  return "GLOBAL TEMPORARY";
}

// Code to define a table in row-locking mode
CString 
SQLInfoOracle::GetCodeLockModeRow() const
{
  // Not needed in Oracle
  return "";
}

// Code to create a temporary table with no logging
CString 
SQLInfoOracle::GetCodeTempTableWithNoLog() const
{
  return "ON COMMIT PRESERVE ROWS";
}

// Granting all rights on a table (In a NON-ANSI database)
CString 
SQLInfoOracle::GetSQLGrantAllOnTable(CString p_schema,CString p_tableName,bool p_grantOption /*= false*/)
{
  CString sql = "GRANT ALL ON " + p_schema + "." + p_tableName + " TO " + GetGrantedUsers();
  if(p_grantOption)
  {
    sql += " WITH GRANT OPTION;\n";
  }
  return sql;
}

// Code prefix for a select-into-temp
CString 
SQLInfoOracle::GetSelectIntoTempClausePrefix(CString p_tableName) const
{
  return "CREATE GLOBAL TEMPORARY TABLE " + p_tableName + "\n" +
         "ON COMMIT PRESERVE ROWS\n" +
         "AS\n";
}

// Code suffix for after a select-into-temp
CString 
SQLInfoOracle::GetSelectIntoTempClauseSuffix(CString p_tableName) const
{
  return "";
}

// Gets the fact if an IF statement needs to be bordered with BEGIN/END
bool
SQLInfoOracle::GetCodeIfStatementBeginEnd() const
{
  // IF THEN ELSE END IF; does not need a BEGIN/END per se.
  return false;
}

// Gets the end of an IF statement
CString 
SQLInfoOracle::GetCodeEndIfStatement() const
{
  return "END IF;\n";
}

// Gets a complete assignment statement.
CString 
SQLInfoOracle::GetAssignmentStatement(const CString& p_destiny,const CString& p_source) const
{
  return p_destiny + " := " + p_source + ";";
}

// Get SQL keyword to alter a column in a table
CString 
SQLInfoOracle::GetCodeAlterColumn() const
{
  // Can be MODIFY or ALTER
  return "MODIFY ";
}

// Get the code to start a WHILE-loop
CString 
SQLInfoOracle::GetStartWhileLoop(CString p_condition) const
{
  return "WHILE " + p_condition + " LOOP\n";
}

// Get the code to end a WHILE-loop
CString 
SQLInfoOracle::GetEndWhileLoop() const
{
  return "END LOOP;\n";
}

// Gets the fact if a SELECT must be in between parenthesis for an assignment
bool    
SQLInfoOracle::GetAssignmentSelectParenthesis() const
{
  // FALSE: value =  SELECT MAX(column) FROM table;
  // TRUE : value = (SELECT MAX(column) FROM table);
  return false;
}

// Gets the UPPER function
CString 
SQLInfoOracle::GetUpperFunction(CString& p_expression) const
{
   return "UPPER(" + p_expression + ")";
}

// Gets the construction for 1 minute ago
CString 
SQLInfoOracle::GetInterval1MinuteAgo() const
{
  return "(SYSDATE - 0.0007)";
}

// Gets the construction / select for generating a new serial identity
CString 
SQLInfoOracle::GetSQLGenerateSerial(CString p_table) const
{
  return "SELECT " + p_table + "_seq.nextval FROM DUAL";
}

// Gets the construction / select for the resulting effective generated serial
CString 
SQLInfoOracle::GetSQLEffectiveSerial(CString p_identity) const
{
  // Just return it, it's the correct value
  return p_identity;
}

// Gets the Not-NULL-Value statement of the database
CString 
SQLInfoOracle::GetNVLStatement(CString& p_test,CString& p_isnull) const
{
  return CString("NVL(") + p_test + "," + p_isnull + ")";
}

// Gets the subtransaction commands
CString 
SQLInfoOracle::GetStartSubTransaction(CString p_savepointName) const
{
  return CString("SAVEPOINT ") + p_savepointName;
}

CString 
SQLInfoOracle::GetCommitSubTransaction(CString p_savepointName) const
{
  // There is no savepoint commit in Oracle!!
  return "";
}

CString 
SQLInfoOracle::GetRollbackSubTransaction(CString p_savepointName) const
{
  return CString("ROLLBACK TO ") + p_savepointName;
}


// SQL CATALOG QUERIES
// ===================================================================

// Get SQL to check if a stored procedure already exists in the database
CString 
SQLInfoOracle::GetSQLStoredProcedureExists(CString& p_name) const
{
  CString uname(p_name);
  uname.MakeUpper();
  return "SELECT COUNT(*)\n"
         "  FROM all_objects\n"
         " WHERE UPPER(object_name) = '" + uname + "'\n"
         "   AND object_type        = 'FUNCTION';";
}

// Part of a query to select only 1 (one) record
CString 
SQLInfoOracle::GetDualTableName() const
{
  return "DUAL";
}

// FROM-Part for a query to select only 1 (one) record
CString 
SQLInfoOracle::GetDualClause() const
{
  return " FROM DUAL";
}

// Gets DEFERRABLE for a constraint (or nothing)
CString 
SQLInfoOracle::GetConstraintDeferrable() const
{
  return " DEFERRABLE";
}

// Defer Constraints until the next COMMIT;
CString 
SQLInfoOracle::GetSQLDeferConstraints() const
{
  // Two SQL Forms for this
  // 1) SET CONSTRAINTS ALL DEFERRED
  // 2) ALTER SESSION SET CONSTRAINTS = DEFERRED
  // In Oracle always deferred in a transaction
  return "ALTER SESSION SET CONSTRAINTS = DEFERRED";
}

// Reset constraints back to immediate
CString 
SQLInfoOracle::GetSQLConstraintsImmediate() const
{
  // Two SQL forms for this:
  // 1) SET CONSTRAINTS ALL IMMEDIATE
  // 2) ALTER SESSION SET CONSTRAINTS = IMMEDIATE
  // In oracle always deferred in a transaction
  return "ALTER SESSION SET CONSTRAINTS = IMMEDIATE";
}

// Get SQL to check if a table already exists in the database
CString 
SQLInfoOracle::GetSQLTableExists(CString p_schema,CString p_tablename) const
{
  p_schema.MakeUpper();
  p_tablename.MakeUpper();
  CString query = "SELECT COUNT(*)\n"
                  "  FROM ALL_TABLES\n"
                  " WHERE owner      = '" + p_schema    + "'\n"
                  "   AND table_name = '" + p_tablename + "'";
  return query;
}

// Get SQL to select all columns of a table from the catalog
CString 
SQLInfoOracle::GetSQLGetColumns(CString& p_user,CString& p_tableName) const
{
  CString systemUser = p_user;
  CString upperName  = p_tableName;
  systemUser.MakeUpper();
  upperName .MakeUpper();

  // name, number, type, length, nullable, precision, scale
  CString select = "SELECT column_name\n"
                   "      ,column_id\n"
                   "      ,data_type\n"
                   "      ,data_length\n"
                   "      ,CASE WHEN nullable = 'Y' THEN 1 ELSE 0 END\n"
                   "      ,data_precision\n"
                   "      ,data_scale\n"
                   "  FROM all_tab_columns\n"
                   " WHERE table_name = '" + upperName  + "'\n"
                   "   AND owner      = '" + systemUser + "'\n"
                   " ORDER BY column_id";
  return select;
}

// Get SQL to select all constraints on a table from the catalog
CString 
SQLInfoOracle::GetSQLGetConstraintsForTable(CString& p_tableName) const
{
  // [EH] Expanded with a filter on SYS_ to prevent NOT NULL constraints
  // to be dropped twice in a row
  // [EH] Type 'R' filtered out, otherwise all reference constraints
  // will be dropped at dropping the check constraints
  CString upperName = p_tableName;
  upperName.MakeUpper();
  CString contabel = "SELECT con.constraint_name\n"
                     "      ,con.table_name\n"
                     "  FROM user_constraints con\n"
                     " WHERE con.constraint_type in ('C','U')\n"
                     "   AND con.table_name = '" + upperName + "'\n"
                     "   AND NOT constraint_name like 'SYS_%'"; 
  return contabel;
}

// Get SQL to read all indices for a table
CString 
SQLInfoOracle::GetSQLTableIndices(CString p_user,CString p_tableName) const
{
  p_user.MakeUpper();
  p_tableName.MakeUpper();

  // Query runs on the 'all_' variant, so owner name must be taken into account
  // for performance reasons on the system table
  CString query = "SELECT idx.index_name\n"
                  "      ,col.column_name\n"
                  "      ,col.column_position\n"
//                  "      ,idx.index_type\n"
                  "      ,idx.uniqueness\n"
                  "      ,col.descend\n"
                  "      ,'' as column_source"
                  "  FROM all_indexes     idx\n"
                  "      ,all_ind_columns col\n"
                  " WHERE idx.index_name  = col.index_name\n"
                  "   AND idx.table_name  = col.table_name\n"
                  "   AND idx.table_owner = col.table_owner\n"
                  "   AND idx.generated   = 'N'\n"
                  "   AND idx.table_name  = '" + p_tableName + "'\n"
                  "   AND idx.table_owner = '" + p_user + "'"
                  " ORDER BY index_name\n"
                  "         ,column_position\n";
  return query;
}

// Get SQL to create an index for a table
CString 
SQLInfoOracle::GetSQLCreateIndex(CString p_user,CString p_tableName,DBIndex* p_index) const
{
  CString sql("CREATE ");
  if(p_index->m_unique)
  {
    sql += "UNIQUE ";
  }
  sql += " INDEX ON ";
  sql += p_user + ".";
  sql += p_tableName + "(";

  int column = 0;
  while(!p_index->m_indexName.IsEmpty())
  {
    if(column)
    {
      sql += ",";
    }
    sql += p_index->m_column;
    sql += (p_index->m_descending) ? " DESC" : " ASC";
    // Next column
    ++column;
    ++p_index;
  }
  sql += ")";

  return sql;
}

// Get SQL to drop an index
CString 
SQLInfoOracle::GetSQLDropIndex(CString p_user,CString p_indexName) const
{
  CString sql = "DROP INDEX " + p_user + "." + p_indexName;
  return sql;
}

// Get SQL to read the referential constraints from the catalog
CString 
SQLInfoOracle::GetSQLTableReferences(CString p_schema
                                    ,CString p_tablename
                                    ,CString p_constraint /*=""*/
                                    ,int     /* p_maxColumns = SQLINFO_MAX_COLUMNS*/) const
{
  // Oracle catalog is in uppercase
  p_schema.MakeUpper();
  p_tablename.MakeUpper();
  p_constraint.MakeUpper();

  // Minimal requirements of the catalog
  if(p_schema.IsEmpty() || p_tablename.IsEmpty())
  {
    throw CString("Cannot get table references without schema/tablename");
  }

  CString query = "SELECT con.constraint_name  AS foreign_key_constraint\n"
                  "      ,con.owner            AS schema_name\n"
                  "      ,con.table_name       AS table_name\n"
                  "      ,col.column_name      AS column_name\n"
                  "      ,pri.table_name       AS primary_table_name\n"
                  "      ,pky.column_name      AS primary_column_name\n"
                  "      ,CASE con.deferrable  WHEN 'NOT DEFERRABLE' THEN 0\n"
                  "                            WHEN 'DEFERRABLE'     THEN 1\n"
                  "                            ELSE 0 END AS DEFERRABLE\n"
                  "      ,CASE con.deferred    WHEN 'IMMEDIATE'      THEN 0\n"
                  "                            ELSE 1 END AS initially_deferred\n"
                  "      ,CASE con.status      WHEN 'ENABLED' THEN 1\n"
                  "                            ELSE 0 END AS enabled\n"
                  "      ,0                    AS match_option\n"
                  "      ,0                    AS update_rule\n"
                  "      ,CASE con.delete_rule WHEN 'RESTRICT'    THEN 0\n"
                  "                            WHEN 'CASCADE'     THEN 1\n"
                  "                            WHEN 'SET NULL'    THEN 2\n"
                  "                            WHEN 'SET DEFAULT' THEN 3\n"
                  "                            WHEN 'NO ACTION'   THEN 4\n"
                  "                            ELSE 0 END AS delete_rule\n"
                  "  FROM dba_constraints  con\n"
                  "      ,dba_cons_columns col\n"
                  "      ,dba_constraints  pri\n"
                  "      ,dba_cons_columns pky\n"
                  " WHERE con.owner           = col.owner\n"
                  "   AND con.constraint_name = col.constraint_name\n"
                  "   AND con.table_name      = col.table_name\n"
                  "   AND pri.owner           = con.r_owner\n"
                  "   AND pri.constraint_name = con.r_constraint_name\n"
                  "   AND pri.owner           = pky.owner\n"
                  "   AND pri.constraint_name = pky.constraint_name\n"
                  "   AND pri.table_name      = pky.table_name\n"
                  "   AND col.position        = pky.position\n"
                  "   AND con.constraint_type = 'R'";
                  "   AND con.owner           = '" + p_schema    + "'\n"
                  "   AND con.table_name      = '" + p_tablename + "'";

  // Optionally a constraint name                
  if(!p_constraint.IsEmpty())
  {
    query += "\n   AND con.constraint_name = '" + p_constraint + "'";
  }
  return query;
}

// Get the SQL to determine the sequence state in the database
CString 
SQLInfoOracle::GetSQLSequence(CString p_schema,CString p_tablename,CString p_postfix /*= "_seq"*/) const
{
  p_schema.MakeUpper();
  CString sequence = p_tablename + p_postfix;
  sequence.MakeUpper();

  CString sql = "SELECT sequence_name\n"
                "      ,last_number AS current_value\n"
                "      ,Decode(min_value,   1,1,0) *\n"
                "       Decode(increment_by,1,1,0) *\n"
                "       Decode(cycle_flag,'N',1,0) *\n"
                "       Decode(cache_size,  0,1,0) AS is_correct\n"
                "  FROM dba_sequences\n"
                " WHERE sequence_owner = '" + p_schema + "'\n"
                "   AND sequence_name  = '" + sequence + "'";
  return sql;
}

// Create a sequence in the database
CString 
SQLInfoOracle::GetSQLCreateSequence(CString p_schema,CString p_tablename,CString p_postfix /*= "_seq"*/,int p_startpos) const
{
  CString sql("CREATE SEQUENCE ");

  if(!p_schema.IsEmpty())
  {
    sql += p_schema + ".";
  }
  sql += p_tablename + p_postfix;
  sql.AppendFormat(" START WITH %d",p_startpos);
  sql += " NOCYCLE NOCACHE ORDER";
  return sql;
}

// Remove a sequence from the database
CString 
SQLInfoOracle::GetSQLDropSequence(CString p_schema,CString p_tablename,CString p_postfix /*= "_seq"*/) const
{
  CString sql;
  CString sequence = p_tablename + p_postfix;

  sql = "DROP SEQUENCE " + p_schema + "." + sequence;
  return sql;
}

// Gets the SQL for the rights on the sequence
CString
SQLInfoOracle::GetSQLSequenceRights(CString p_schema,CString p_tableName,CString p_postfix /*="_seq"*/) const
{
  CString sequence = p_tableName + p_postfix;
  if(!p_schema.IsEmpty())
  {
    sequence = p_schema + "." + sequence;
  }
  return "GRANT SELECT, ALTER ON " + sequence + " TO " + GetGrantedUsers();
}

// Remove a stored procedure from the database
void    
SQLInfoOracle::DoRemoveProcedure(CString& p_procedureName) const
{
  SQLQuery query(m_database);
  query.TryDoSQLStatement("DROP FUNCTION " + p_procedureName);
}

// Get SQL for your session and controling terminal
CString 
SQLInfoOracle::GetSQLSessionAndTerminal() const
{
  CString query = "SELECT sys_context('userenv','sessionid')\n"
                  "      ,sys_context('userenv','terminal')\n"    
                  "  FROM DUAL";
  return query;
}

// Get SQL to check if session number exists
CString 
SQLInfoOracle::GetSQLSessionExists(CString sessieId) const
{
  return "SELECT COUNT(*)\n"
         "  FROM v$session\n"
         " WHERE audsid = " + sessieId;
}

// Get SQL for unique session ID
CString 
SQLInfoOracle::GetSQLUniqueSessionId(const CString& /*p_databaseName*/,const CString& /*p_sessionTable*/) const
{
  // In Oracle the database is the instance. Database name is irrelevant
  return "SELECT UNIQUE(audsid)\n"
         "  FROM V$SESSION\n"
         " WHERE audsid <> 0";
}

// Get SQL for searching a session
CString 
SQLInfoOracle::GetSQLSearchSession(const CString& /*p_databaseName*/,const CString& p_sessionTable) const
{
  // In Oracle the database is the instance. Database name is irrelevant
  return  "SELECT audsid\n"
          "       ,osuser\n"
          "       ,terminal\n"
          "  FROM V$SESSION\n"
          " WHERE audsid <> 0\n"
          "   AND NOT audsid IN\n"
          "     ( SELECT sessie_nr\n"
          "         FROM "+ p_sessionTable+ ")";
}

// See if a column exists within a table
bool   
SQLInfoOracle::DoesColumnExistsInTable(CString& p_owner,CString& p_tableName,CString& p_column) const
{
  int number = 0;

  CString tableName(p_tableName);
  tableName.MakeUpper();
  CString column(p_column);
  column.MakeUpper();
  CString owner(p_owner);
  owner.MakeUpper();

  CString query = "SELECT COUNT(*)\n"
                 "  FROM all_tab_columns tab\n"
                 " WHERE tab.table_name  = '" + tableName + "'\n"
                 "   AND tab.column_name = '" + column    + "'\n"
                 "   AND tab.owner       = '" + owner     + "'";
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
SQLInfoOracle::GetSQLPrimaryKeyConstraintInformation(CString p_schema,CString p_tableName) const
{
  p_schema.MakeUpper();
  p_tableName.MakeUpper();

  CString query = "SELECT c.constraint_name\n"
                  "      ,c.index_name\n"
                  "      ,(SELECT i.tablespace_name\n"
                  "          FROM all_indexes i\n"
                  "         WHERE i.index_name = c.index_name)\n"
                  "  FROM all_constraints c\n"
                  " WHERE c.owner           = '" + p_schema    + "'\n"
                  "   AND c.table_name      = '" + p_tableName + "'\n"
                  "   AND c.constraint_type = 'P'\n";
  return query;
}

// Does the named constraint exist in the database
bool    
SQLInfoOracle::DoesConstraintExist(CString p_constraintName) const
{
  CString constraint(p_constraintName);
  constraint.MakeUpper();

  SQLQuery qry(m_database);
  qry.DoSQLStatement("SELECT 1\n"
                     "  FROM all_constraints c\n"
                     " WHERE c.constraint_name = '" + constraint + "'\n");
  return qry.GetRecord();
}

// Gets the lock-table query
CString 
SQLInfoOracle::GetSQLLockTable(CString& p_tableName,bool p_exclusive) const
{
  CString query = "LOCK TABLE " + p_tableName + " IN "  ;
  query += p_exclusive ? "EXCLUSIVE" : "SHARE";
  query += " MODE";
  return query;
}

// Get query to optimize the table statistics
CString 
SQLInfoOracle::GetSQLOptimizeTable(CString& p_owner,CString& p_tableName,int& p_number)
{
  CString optim;
  // Optimize the table
  optim = "call dbms_stats.gather_table_stats('" + p_owner + "','" + p_tableName + "');\n";
  // Count number of statements
  ++p_number;

  return optim;
}

// Getting the fact that there is only **one** (1) user session in the database
bool
SQLInfoOracle::GetOnlyOneUserSession()
{
  // Yet to implement
  return true;
}

// SQL DDL STATEMENTS
// ==================

CString
SQLInfoOracle::GetCreateColumn(CString p_schema,CString p_tablename,CString p_columnName,CString p_typeDefinition,bool p_notNull)
{
  CString sql  = "ALTER TABLE "  + p_schema + "." + p_tablename  + "\n";
                 "  ADD COLUMN " + p_columnName + " " + p_typeDefinition;
  if(p_notNull)
  {
    sql += " NOT NULL";
  }
  return sql;
}

// Drop a column from a table
CString 
SQLInfoOracle::GetSQLDropColumn(CString p_schema,CString p_tablename,CString p_columnName) const
{
  return "ALTER TABLE " + p_schema + "." + p_tablename + "\n"
         " DROP COLUMN " + p_columnName;
}

// Add a foreign key to a table
CString
SQLInfoOracle::GetCreateForeignKey(CString p_tablename,CString p_constraintname,CString p_column,CString p_refTable,CString p_primary)
{
  CString sql = "ALTER TABLE " + p_tablename + "\n"
                "  ADD CONSTRAINT " + p_constraintname + "\n"
                "      FOREIGN KEY (" + p_column + ")\n"
                "      REFERENCES " + p_refTable + "(" + p_primary + ")";
  return sql;
}

CString 
SQLInfoOracle::GetModifyColumnType(CString p_schema,CString p_tablename,CString p_columnName,CString p_typeDefinition)
{
  CString sql = "ALTER  TABLE  " + p_schema + "." + p_tablename + "\n";
                "MODIFY COLUMN " + p_columnName + " " + p_typeDefinition;
  return sql;
}

CString 
SQLInfoOracle::GetModifyColumnNull(CString p_schema,CString p_tablename,CString p_columnName,bool p_notNull)
{
  CString sql = "ALTER  TABLE  " + p_schema + "." + p_tablename + "\n";
                "MODIFY COLUMN " + p_columnName + " " + (p_notNull ? "NOT " : "") + "NULL";
  return sql;
}

// Get the SQL to drop a view. If precursor is filled: run that SQL first!
CString 
SQLInfoOracle::GetSQLDropView(CString p_schema,CString p_view,CString& p_precursor)
{
  p_precursor.Empty();
  return "DROP VIEW " + p_schema + "." + p_view;
}

// Create or replace a database view
CString 
SQLInfoOracle::GetSQLCreateOrReplaceView(CString p_schema,CString p_view,CString p_asSelect) const
{
  return "CREATE OR REPLACE VIEW " + p_schema + "." + p_view + "\n" + p_asSelect;
}

// SQL DDL ACTIONS
// ===================================================================

// Do the commit for the DDL commands in the catalog
void    
SQLInfoOracle::DoCommitDDLcommands() const
{
  // Does NOTHING In ORACLE and should do nothing
  // commit for DDL is autmatic and always
}

// Do the commit for the DML commands in the database
// ODBC driver autocommit mode will go wrong!!
void
SQLInfoOracle::DoCommitDMLcommands() const
{
  SQLQuery qry(m_database);
  qry.DoSQLStatement("COMMIT");
}

// Remove a column from a table
void
SQLInfoOracle::DoDropColumn(CString p_tableName,CString p_columName)
{
  CString sql = "ALTER TABLE  " + p_tableName + "\n"
                " DROP COLUMN " + p_columName;
  SQLQuery query(m_database);
  query.TryDoSQLStatement(sql);
}

// Does the named view exists in the database
bool
SQLInfoOracle::DoesViewExists(CString& p_viewName)
{
  CString viewname(p_viewName);
  viewname.MakeUpper();
  SQLQuery query(m_database);
  query.DoSQLStatement("SELECT COUNT(*)\n"
                       "  FROM all_views\n"
                       " WHERE view_name = '" + viewname + "'");
  if(query.GetRecord())
  {
    return query.GetColumn(1)->GetAsSLong() == 1;
  }
  return false;
}

// Must create temporary tables runtime 
bool
SQLInfoOracle::GetMustMakeTemptablesAtRuntime() const
{
  // FALSE: GLOBAL TEMPORARY TABLES IN THE ENGINE
  // TRUE:  TRUE SESSION TEMPTABLES MUST ALWAYS BE CREATED (Informix)
  return false;
}

// Create a temporary table in an optimized manner with the given index column
void    
SQLInfoOracle::DoMakeTemporaryTable(CString& p_tableName,CString& p_content,CString& p_indexColumn) const
{
  SQLQuery query(m_database);
  p_tableName.MakeUpper();

  query.TryDoSQLStatement("TRUNCATE TABLE " + p_tableName);
  query.TryDoSQLStatement("DROP TABLE "     + p_tableName);
  CString create = "CREATE GLOBAL TEMPORARY TABLE " + p_tableName + " ON COMMIT PRESERVE ROWS\n" + p_content;
  try
  {
    query.DoSQLStatement(create);

    if(!p_indexColumn.IsEmpty())
    {
      create = "CREATE INDEX " + p_tableName + "_" + p_indexColumn + " ON " + p_tableName+ "(" + p_indexColumn+ ")";
      query.DoSQLStatement(create);
    }
  }
  catch(...)
  {
    throw CString("Cannot make a temporary table: ") + p_tableName;
  }
}

// Remove a temporary table
void
SQLInfoOracle::DoRemoveTemporaryTable(CString& p_tableName) const
{
  SQLQuery query(m_database);

  // Every error can be ignored. Can still be in use by another user and/or session
  // The table contents will then removed for this session
  query.TryDoSQLStatement("DELETE FROM "    + p_tableName);
  query.TryDoSQLStatement("TRUNCATE TABLE " + p_tableName);
  query.TryDoSQLStatement("DROP TABLE "     + p_tableName);
}

// Create a procedure in the database
void
SQLInfoOracle::DoMakeProcedure(CString& p_procName,CString p_table,bool /*p_noParameters*/,CString& p_codeBlock)
{
  SQLQuery query(m_database);

  query.DoSQLStatement(p_codeBlock);
  query.DoSQLStatement("GRANT EXECUTE ON " + p_procName + " TO " + GetGrantedUsers());
  query.TryDoSQLStatement("CREATE OR REPLACE PUBLIC SYNONYM " + p_procName + " FOR " + p_procName);
  query.DoSQLStatement("GRANT ALL ON " + p_procName + " TO " + GetGrantedUsers());
}

// Rename a database table 
void
SQLInfoOracle::DoRenameTable(CString& p_oldName,CString& p_newName) const
{
  SQLQuery query(m_database);

  // Beware: No 'TABLE' in the statement
  CString rename = "rename " + p_oldName + " to " + p_newName;
  query.DoSQLStatement(rename);
  DoCommitDDLcommands();
}

// PERSISTENT-STORED MODULES (SPL / PL/SQL)
// ====================================================================

// Get the user error text from the database
CString
SQLInfoOracle::GetUserErrorText(CString& p_procName) const
{
  CString query;
  CString errorText;
  CString procName(p_procName);
  procName.MakeUpper();
  query = "SELECT line\n"
          "      ,position\n"
          "      ,text\n"
          "  FROM user_errors\n"
          " WHERE name = '" + procName +"'";

  SQLQuery qry1(m_database);
  SQLQuery qry2(m_database);
  qry1.DoSQLStatement(query);

  while (qry1.GetRecord())
  {
    CString s = qry1.GetColumn(3)->GetAsChar();
    if(s.Find("Statement ignored") < 0) 
    {
      s.Format("Error in line %d, column %d: %s\n",qry1.GetColumn(1)->GetAsSLong(),qry1.GetColumn(2)->GetAsSLong());
      errorText += s;
      query.Format( "SELECT text\n"
                    "  FROM all_source\n"
                    " WHERE type = 'FUNCTION'\n"
                    "   AND name = '%s'\n"
                    "   AND line = %d"
                   ,procName
                   ,qry1.GetColumn(1)->GetAsSLong());
      qry2.DoSQLStatement(query);
      while(qry2.GetRecord())
      {
        s.Format("Line %d: %s\n",qry1.GetColumn(1)->GetAsSLong(),qry2.GetColumn(1)->GetAsChar());
        errorText += s;
      }
    }
  }
  return errorText;
}

// Get assignment to a variable in SPL
CString 
SQLInfoOracle::GetSPLAssignment(CString p_variable) const
{
  return p_variable + " := ";
}

// Get the start of a SPL While loop
CString 
SQLInfoOracle::GetSPLStartWhileLoop(CString p_condition) const
{
  return "WHILE " + p_condition + " LOOP\n";
}

// Get the end of a SPL while loop
CString 
SQLInfoOracle::GetSPLEndWhileLoop() const
{
  return "END LOOP;\n";
}

// Get stored procedure call
CString 
SQLInfoOracle::GetSQLSPLCall(CString p_procName) const
{
  // x used to be an integer. 
  // In combination with some ODBC drivers from Oracle
  // it created an Autonomous_transaction . 
  return "DECLARE x VARCHAR2(254); BEGIN x := " + p_procName + "; END;";
}

// Length of parameters in binding
int
SQLInfoOracle::GetParameterLength(int p_SQLType) const
{
  int retval;

  switch (p_SQLType)
  {
    case SQL_CHAR:                      retval =  2000;  break;
    case SQL_VARCHAR:                   retval =  4000;  break;
    case SQL_LONGVARCHAR:               retval = 32000;  break;
    case SQL_DECIMAL:                   retval = 32000;  break;
    case SQL_SMALLINT:                  retval = 0;      break;
    case SQL_INTEGER:                   retval = sizeof(long); break;
    case SQL_REAL:                      retval = 0;      break;
    case SQL_DOUBLE:                    retval = 0;      break;
    case SQL_FLOAT:                     retval = 0;      break;
    case SQL_BINARY:                    retval = 0;      break;
    case SQL_VARBINARY:                 retval = 0;      break;
    case SQL_LONGVARBINARY:             retval = 0;      break;
    case SQL_DATE:                      retval = 0;      break;
    case SQL_TIME:                      retval = 0;      break;
    case SQL_TIMESTAMP:                 retval = 19;     break;
    case SQL_NUMERIC:                   retval = 0;      break;
    case SQL_BIGINT:                    retval = 0;      break;
    case SQL_TINYINT:                   retval = 0;      break;
    case SQL_BIT:                       retval = 0;      break;
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
    case SQL_INTERVAL_SECOND:           retval = 25;      break;
    default:                            retval = 0;       break;
  }
  return retval;
}

// Build a parameter list for calling a stored procedure
CString
SQLInfoOracle::GetBuildedParameterList(size_t p_numOfParameters) const
{
  // The string of ? parameters for binding of a stored procedure
  // In ORACLE: If no parameters, no ellipsis either!
  CString strParamList;
  if(p_numOfParameters >= 0)
  {
    for (size_t i = 0; i < p_numOfParameters; i++)
    {
      if(i!=0) 
      {
        strParamList += ",";
      }
      else
      {
        strParamList += "(";
      }
      strParamList += "?";
    }
    if(p_numOfParameters > 0)
    {
      strParamList += ")";
    }
  }
  return strParamList;
}

// Parameter type for stored procedure for a given column type for parameters and return types
CString 
SQLInfoOracle::GetParameterType(CString& p_type) const
{
  // Beware: this is the spot where the ellipsis and width/precision
  // are removed from the datatype, these cannot occur in Oracle
  // parameters and return types

  if (p_type.Left(4).CompareNoCase("CHAR")    == 0 || 
      p_type.Left(7).CompareNoCase("VARCHAR") == 0  )
  {
    return "VARCHAR2";
  }
  else if (p_type.Left(7).CompareNoCase("DECIMAL") == 0 ||
           p_type.Left(6).CompareNoCase("NUMBER")  == 0  )
  {
    return "NUMBER";
  }
  return p_type;
}

// GENERAL SQL ACTIONS
// =================================================================

// Makes a SQL string from a given string, with all the right quotes
CString 
SQLInfoOracle::GetSQLString(const CString& p_string) const
{
  CString s = p_string;
  s.Replace("'","''");
  CString kwoot = GetQuoteCharacter();
  return  kwoot + s + kwoot;
}

// Get date string in engine format
CString 
SQLInfoOracle::GetSQLDateString(int p_year,int p_month,int p_day) const
{
  CString dateString;
  dateString.Format("TO_DATE('%04d-%02d-%02d','YYYY-MM-DD')",p_year,p_month,p_day);
  return dateString;
}  
  
// Get time string in database engine format
CString 
SQLInfoOracle::GetSQLTimeString(int p_hour,int p_minute,int p_second) const
{
  // The hour must be between 0 and 24
  p_hour = p_hour % 24;
  while(p_hour < 0) p_hour += 24;

  // Make a decimal part of a day
  int seconds = p_second + (p_minute * 60) + (p_hour * 60 * 60);
  int dag     = 24 * 60 * 60;
  double val  = (double)seconds / (double) dag;
  CString timeString;
  timeString.Format("%0.15f",val);
  
  return timeString;
}

// Get date-time string in database engine format
CString 
SQLInfoOracle::GetSQLDateTimeString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  CString string;
  string.Format("TO_DATE('%04d-%02d-%02d %02d:%02d:%02d','YYYY-MM-DD HH24:MI:SS')"
               ,p_year,p_month,p_day
               ,p_hour,p_minute,p_second);
  return string;
}

// For parameter binding of a date-time string
CString
SQLInfoOracle::GetSQLDateTimeBoundString() const
{
  return "TO_DATE(?,'YYYY-MM-DD HH24:MI:SS')";
}

// Stripped data for the parameter binding
CString
SQLInfoOracle::GetSQLDateTimeStrippedString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  CString string;
  string.Format("%04d-%02d-%02d %02d:%02d:%02d"
                ,p_year,p_month,p_day
                ,p_hour,p_minute,p_second);
  return string;
}
  
// Get the SPL source code for a stored procedure as registered in the database
CString 
SQLInfoOracle::GetSPLSourcecodeFromDatabase(const CString& p_owner,const CString& p_procName) const
{
  CString sQuery;  
  sQuery = "SELECT text\n"
           "  FROM all_source\n"
           " WHERE type  = 'FUNCTION'\n"
           "   AND name  = UPPER('" + p_procName + "') "
           "   AND owner = UPPER('" + p_owner    + "')";
  SQLQuery qry(m_database);
  qry.TryDoSQLStatement(sQuery);
  CString sProcBody = "CREATE OR REPLACE ";
  while (qry.GetRecord())
  {
    sProcBody += qry.GetColumn(1)->GetAsChar();
  }
  return sProcBody;
}

// Get the SPL datatype for integer
CString 
SQLInfoOracle::GetSPLIntegerType() const
{
  // Integers get generated as NUMBER(10) in the database, 
  // but this is the form as they appear in stored procedure language
  return "NUMBER";
}

// Get the SPL datatype for a decimal
CString 
SQLInfoOracle::GetSPLDecimalType() const
{
  // Decimals are generated as DECIMAL(n,m)
  // But this is the form as they appear in stored procedure language
  return "NUMBER";
}

// Get the SPL declaration for a cursor
CString 
SQLInfoOracle::GetSPLCursorDeclaratie(CString& p_variableName,CString& p_query) const
{
  return "CURSOR " + p_variableName + " IS " + p_query + ";";
}

// Get the SPL cursor found row parameter
CString 
SQLInfoOracle::GetSPLCursorFound(CString& p_cursorName) const
{
  return p_cursorName + "%FOUND";
}

// Get the SPL cursor row-count variable
CString 
SQLInfoOracle::GetSPLCursorRowCount(CString& p_variable) const
{
  return p_variable + " := SQL%ROWCOUNT;\n";
}

// Get the SPL datatype for a declaration of a row-variable
CString 
SQLInfoOracle::GetSPLCursorRowDeclaration(CString& p_cursorName,CString& p_variableName) const
{
  return p_variableName + " " + p_cursorName + "%ROWTYPE;";
}

CString 
SQLInfoOracle::GetSPLFetchCursorIntoVariables(CString               p_cursorName
                                             ,CString             /*p_variableName*/
                                             ,std::vector<CString>& p_columnNames
                                             ,std::vector<CString>& p_variableNames) const
{
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
SQLInfoOracle::GetSPLFetchCursorIntoRowVariable(CString& p_cursorName,CString p_variableName) const
{ 
  return "FETCH " + p_cursorName + " INTO " + p_variableName+ ";";
}

// Get the SPL no-data exception clause
CString 
SQLInfoOracle::GetSPLNoDataFoundExceptionClause() const
{
  return "WHEN NO_DATA_FOUND THEN";
}

// Get the SPL form of raising an exception
CString 
SQLInfoOracle::GetSPLRaiseException(CString p_exceptionName) const
{
  return "RAISE " + p_exceptionName + ";";
}

// Get the fact that the SPL has server functions that return more than 1 value
bool    
SQLInfoOracle::GetSPLServerFunctionsWithReturnValues() const
{
  return true;
}

// SPECIALS
// ==========================================================================

// Translate database-errors to a human readable form
CString 
SQLInfoOracle::TranslateErrortext(int p_error,CString p_errorText) const
{
  // Check if we have work to do
  if(p_error == 0)
  {
    return p_errorText;
  }

  // Read vendor error number
  if(p_error == -1 && p_errorText.GetAt(3) == '-' && p_errorText.GetLength() > 10)
  {
    p_error = atoi(p_errorText.Mid(4, 5));
  }

  // Database error = %d : 
  CString errorText = "Database error = ";

  // Get error number -> error text
  switch(p_error)
  {
    case   -95: errorText += "Removing of an object trough a logical update stored procedure '%s' is not possible"; break;
    case   -96: break; // Generated by stored procedure
    case   -97: errorText += "Removal denied: there are still references to the object%s."; break;
    case   -99: errorText += "Wrong object ID in procedure '%s'.";  break;
    case   942: errorText += "Table not found: '%s'.";              break;
    case   904: errorText += "Column not found: '%s'.";             break;
    case     1: errorText += "Cannot make a new object: Double value in unique index."; break;
    case  2290: errorText += "Check constraint violation: '%s'.";   break;
    case  2292: errorText += "Tried to delete an object to which a reference is still made from another table."; break;
  }
  return errorText;
}

