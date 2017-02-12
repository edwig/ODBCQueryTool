////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoPostgreSQL.cpp
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
#include "SQLInfoPostgreSQL.h"
#include "SQLQuery.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{

// Constructor. 
SQLInfoPostgreSQL::SQLInfoPostgreSQL(SQLDatabase* p_database)
                  :SQLInfoDB(p_database)
{
}

// Destructor.
SQLInfoPostgreSQL::~SQLInfoPostgreSQL()
{
}

// Getting the SQLDatabaseType
DatabaseType 
SQLInfoPostgreSQL::GetDatabaseType() const
{
  return RDBMS_POSTGRESQL;
}

// BOOLEANS AND STRINGS
// ====================================================================

CString
SQLInfoPostgreSQL::GetDatabaseVendorName() const
{
  return "PostgreSQL";
}

// Get physical database name
CString 
SQLInfoPostgreSQL::GetPhysicalDatabaseName() const
{
  SQLQuery sql(m_database);
  CString query = "SELECT current_database()";
  sql.DoSQLStatement(query);
  if(sql.GetRecord())
  {
    return sql.GetColumn(1)->GetAsChar();
  }
  return "";
}

// System catalog is stored in uppercase in the database?
bool 
SQLInfoPostgreSQL::IsCatalogUpper () const
{
  return false;
}

// System catalog supports full ISO schemas (same tables per schema)
bool
SQLInfoPostgreSQL::GetUnderstandsSchemas() const
{
  return true;
}

// Supports database/ODBCdriver comments in sql
bool 
SQLInfoPostgreSQL::SupportsDatabaseComments() const
{
  return false;
}

// Database can defer constraints until the end of a transaction
bool 
SQLInfoPostgreSQL::SupportsDeferredConstraints() const
{
  // SET CONSTRAINTS DEFERRED is supported
  return true;
}

// Database has ORDER BY with an expression, e.g. ORDER BY UPPER(column-name)
// Work-around is "SELECT UPPER(column-name) AS something.....ORDER BY something
bool
SQLInfoPostgreSQL::SupportsOrderByExpression() const
{
  return true;
}

// Supports the ODBC escape sequence {[?=] CALL procedure (?,?,?)}
bool
SQLInfoPostgreSQL::SupportsODBCCallEscapes() const
{
  // Does NOT support the [?=] return parameter
  // So our own plumbing is needed
  return false;
}

// Catalog table with all default values of a column in the database
CString 
SQLInfoPostgreSQL::GetSQLStringDefaultValue(CString p_tableName,CString p_columnName) const
{
  CString query = "SELECT adsrc\n"
                  "  FROM pg_attrdef, pg_attribute, pg_class\n"
                  " WHERE pg_attrdef.adrelid    = pg_attribute.attrelid\n"
                  "   AND pg_attrdef.adnum      = pg_attribute.attnum\n"
                  "   AND pg_attribute.attrelid = pg_class.oid\n"
                  "   AND pg_class.relname      = '" + p_tableName + "'\n"
                  "   AND pg_attribute.attname  = '" + p_columnName + "'";                 
  return query;
}

// Keyword for current date and time
CString 
SQLInfoPostgreSQL::GetSystemDateTimeKeyword() const
{
  return "current_timestamp(0)";
} 

// Gtetign the current date
CString
SQLInfoPostgreSQL::GetSystemDateString() const
{
  return "current_date";
}

// True if TIME is implemented in the database as a DECIMAL
bool 
SQLInfoPostgreSQL::GetTimeIsDecimal() const
{
  // TIJD is implemented as TIME in PostgreSQL :-)
  return false;
}

// True if INTERVAL is impelemented as a DECIMAL
bool 
SQLInfoPostgreSQL::GetIntervalIsDecimal() const
{
  // Interval is implemented as INTERVAL in PostgreSQL :-)
  return false;
}

// Character for the string concatanation operator
CString 
SQLInfoPostgreSQL::GetConcatanationOperator() const
{
  return "||";    
}

// Quote character for string literals
CString 
SQLInfoPostgreSQL::GetQuoteCharacter() const
{
  return "'";    
}

// Default NULL for parameters
CString 
SQLInfoPostgreSQL::GetDefaultNULL() const
{
  return "";    
}

// INOUT prefix for input/output parameter in a parameter list
CString 
SQLInfoPostgreSQL::GetParameterINOUT() const
{
  return "";    
}
  
// OUT prefix for an output parameter in a parameter list
CString 
SQLInfoPostgreSQL::GetParameterOUT() const
{
  return "";    
}

// Get datatype for last user in a stored procedure as a parameter
CString 
SQLInfoPostgreSQL::GetAuditUserDatatype() const
{
  return "VARCHAR";
} 

// Get datatype for last user in a stored procedure as a variable
CString 
SQLInfoPostgreSQL::GetAuditUserDatatypeAsVariable() const
{
  return "VARCHAR(50)";
} 

// Datatype of the primary key
CString 
SQLInfoPostgreSQL::GetPrimaryKeyType() const
{
  return "INTEGER";
}

// Datatype for a timestamp (DATETIME YEAR TO SECOND)
CString 
SQLInfoPostgreSQL::GetDatetimeYearToSecondType() const
{
  return "TIMESTAMP";
}

// Separator between to alter constraints in a alter table statemetn
CString
SQLInfoPostgreSQL::GetAlterConstraintSeparator() const
{
  return ",";
}

// Inner Join Keyword
CString 
SQLInfoPostgreSQL::GetInnerJoinKeyword() const
{
  return "INNER JOIN ";
}

// Outer join keyword
CString  
SQLInfoPostgreSQL::GetOuterJoinKeyword() const
{
  return "LEFT OUTER JOIN ";
}

// Inner Join Keyword for use in views.
CString 
SQLInfoPostgreSQL::GetViewInnerJoinKeyword() const
{
  return "INNER JOIN";
}

// Outer join keyword for use in views
CString 
SQLInfoPostgreSQL::GetViewOuterJoinKeyword() const
{
  return "LEFT OUTER JOIN";
}

// Closer character for an outer join (+ in some databases)
CString 
SQLInfoPostgreSQL::GetOuterJoinClosure() const
{
  return "";
}

// OUTER JOIN sign
CString  
SQLInfoPostgreSQL::GetOuterJoinSign() const
{
  return "";
}

// Prefix for using a parameter in a stored procedure
CString
SQLInfoPostgreSQL::GetSPParamPrefix() const
{
  return "";
}

// Get the query to add a new record in a table by the column OID
CString 
SQLInfoPostgreSQL::GetIdentityString(CString& p_tablename,CString p_postfix /*="_seq"*/) const
{
  return "nextval('" + p_tablename + p_postfix + "')";
}

// Get the query to create a temporary table from a SELECT statement
CString 
SQLInfoPostgreSQL::GetSQLCreateTemporaryTable(CString& p_tablename,CString p_select) const
{
  return "CREATE GLOBAL TEMPORARY TABLE " + p_tablename + "\nAS " + p_select;
}

// Get the query to remove a temporary table completely
CString
SQLInfoPostgreSQL::GetSQLRemoveTemporaryTable(CString& p_tablename,int& p_number) const
{
  p_number += 3;
  return "DELETE FROM "    + p_tablename + ";\n"
         "TRUNCATE TABLE " + p_tablename + ";\n"
         "DROP TABLE "     + p_tablename + ";\n";
}

// Get the SQL query to select into a temporary table
CString 
SQLInfoPostgreSQL::GetSQLSelectIntoTemp(CString& p_tablename,CString& p_select) const
{
   return "INSERT INTO " + p_tablename + "\n" + p_select + ";\n";
}

// Replace the primary key column by a sequence NEXTVAL
CString 
SQLInfoPostgreSQL::GetReplaceColumnOIDbySequence(CString p_columns,CString p_tablename,CString p_postfix /*="_seq"*/) const
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

// Remove catalg dependencies
CString 
SQLInfoPostgreSQL::GetSQLRemoveProcedureDependencies(CString p_procname) const
{
  return "";
}

// Remove catalog dependencies for calculated fields (bug in Firebird)
CString 
SQLInfoPostgreSQL::GetSQLRemoveFieldDependencies(CString p_tablename) const
{
  // Not needed in PostgreSQL
  return "";
}

// Getting the create-table form of a primary key constraint
CString 
SQLInfoPostgreSQL::GetPrimaryKeyDefinition(CString p_schema,CString p_tableName,bool /*p_temporary*/) const
{
  return GetPrimaryKeyType() + " NOT NULL CONSTRAINT pk_" + p_tableName + " PRIMARY KEY\n";
}

// Getting the primary key constraint in the form of an ALTER TABLE statement
CString
SQLInfoPostgreSQL::GetPrimaryKeyConstraint(CString p_schema,CString p_tablename,CString p_primary) const
{
  return "ALTER TABLE " + p_schema + "." + p_tablename + "\n"
         "  ADD CONSTRAINT pk_" + p_tablename + "\n"
         "      PRIMARY KEY (" + p_primary + ")";
}

// Get the sql to add a foreign key to a table
CString 
SQLInfoPostgreSQL::GetSQLForeignKeyConstraint(DBForeign& p_foreign) const
{
  // Construct the correct tablename
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
  switch(p_foreign.m_match)
  {
    case 0: query += "\n      MATCH FULL";    break;
    case 1: query += "\n      MATCH PARTIAL"; break;
    case 2: query += "\n      MATCH SIMPLE";  break;
  }
  switch(p_foreign.m_updateRule)
  {
    case 1: query += "\n      ON UPDATE CASCADE";     break;
    case 2: query += "\n      ON UPDATE SET NULL";    break;
    case 3: query += "\n      ON UPDATE SET DEFAULT"; break;
    case 4: query += "\n      ON UPDATE NO ACTION";   break;
    default:// The default
    case 0: query += "\n      ON UPDATE RESTRICT";    break;
  }
  switch(p_foreign.m_deleteRule)
  {
    case 1: query += "\n      ON DELETE CASCADE";     break;
    case 2: query += "\n      ON DELETE SET NULL";    break;
    case 3: query += "\n      ON DELETE SET DEFAULT"; break;
    case 4: query += "\n      ON DELETE NO ACTION";   break;
    default:// The default
    case 0: query += "\n      ON DELETE RESTRICT";    break;
  }
  return query;
}

// Get the sql (if possible) to change the foreign key constraint
CString 
SQLInfoPostgreSQL::GetSQLAlterForeignKey(DBForeign& p_origin,DBForeign& p_requested) const
{
  // Construct the correct tablename
  CString table(p_origin.m_tablename);
  if(!p_origin.m_schema.IsEmpty())
  {
    table = p_origin.m_schema + "." + table;
  }

  // The base foreign key command
  CString query = "ALTER TABLE " + table + "\n"
                  "ALTER CONSTRAINT " + p_origin.m_constraintname + "\n";

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
  if((p_origin.m_match      != p_requested.m_match)      ||
     (p_origin.m_updateRule != p_requested.m_updateRule) ||
     (p_origin.m_deleteRule != p_requested.m_deleteRule) )
  {
    // PostgreSQL not capable of altering these attributes, so re-create the constraint
    query.Empty();
  }
  return query;
}

// Performance parameters in the database
CString
SQLInfoPostgreSQL::GetSQLPerformanceSettings() const
{
  return "";
}

// SQL To set the caching mode of SQL results
CString 
SQLInfoPostgreSQL::GetSQLCacheModeSetting(const CString& /*p_mode*/) const
{
  return "";
}

// Formatting of numbers in stored procedures
CString
SQLInfoPostgreSQL::GetSQLNlsNumericCharacters() const
{
  return "";
}

// Altering the datatype of a column
CString
SQLInfoPostgreSQL::GetSQLModifyColumnName(CString p_tablename,CString p_oldName,CString p_newName,CString p_datatype)
{
  CString sqlCode = "ALTER TABLE " + p_tablename + "\n"
                    "  ADD " + p_newName + " " + p_datatype + ";\n";
  sqlCode += "UPDATE " + p_tablename + "\n"
             "   SET " + p_newName + " = " + p_oldName + ";\n";
  sqlCode += "ALTER TABLE " + p_tablename + "\n"
             " DROP COLUMN " + p_oldName + ";";
  return sqlCode;
}

// Maximum length of an SQL statement
unsigned long 
SQLInfoPostgreSQL::GetMaxStatementLength() const
{
  return 0;//	No limit
}

// Prefix for altering the datatype in a MODIFY/ALTER statement
CString 
SQLInfoPostgreSQL::GetModifyDatatypePrefix() const
{
  // SO: MODIFY <kolomnaam> TYPE <datatypenaam>
  return "TYPE ";
}

// Definition / Qualifier of a temporary table
CString 
SQLInfoPostgreSQL::GetCodeTemporaryTable() const
{
  return "GLOBAL TEMPORARY";
}

// Locking a table in row mode
CString 
SQLInfoPostgreSQL::GetCodeLockModeRow() const
{
  return "";
}

// To create a table without logging
CString 
SQLInfoPostgreSQL::GetCodeTempTableWithNoLog() const
{
  return "\nWITHOUT OIDS\nON COMMIT PRESERVE ROWS";
}

// Code to grant rights on configured users
CString 
SQLInfoPostgreSQL::GetSQLGrantAllOnTable(CString p_schema,CString p_tableName,bool p_grantOption /*= false*/)
{
  CString sql = "GRANT ALL ON " + p_schema + "." + p_tableName + " TO " + GetGrantedUsers();
  if(p_grantOption)
  {
    sql += " WITH GRANT OPTION";
  }
  return sql;
}


// Code BEFORE a select into temp
CString
SQLInfoPostgreSQL::GetSelectIntoTempClausePrefix(CString p_tableName) const
{
  return "CREATE GLOBAL TEMPORARY TABLE " + p_tableName + "\nWITHOUT OIDS AS\n";
}

// Code AFTER the select into temp
CString
SQLInfoPostgreSQL::GetSelectIntoTempClauseSuffix(CString p_tableName) const
{
  return "";
}

bool
SQLInfoPostgreSQL::GetCodeIfStatementBeginEnd() const
{
  // IF THEN ELSE END IF; does not necessarily need a BEGIN/END
  return false;
}

// At the end of an IF statement
CString 
SQLInfoPostgreSQL::GetCodeEndIfStatement() const
{
  return "END IF;\n";
}

CString 
SQLInfoPostgreSQL::GetAssignmentStatement(const CString& p_destiny,const CString& p_source) const
{
  return p_destiny + " := " + p_source + ";";
}

// SQL To alter a column (MODIFY / ALTER)
CString 
SQLInfoPostgreSQL::GetCodeAlterColumn() const
{
  return "ALTER ";
}

// Code to start an SPL WHIL loop
CString
SQLInfoPostgreSQL::GetStartWhileLoop(CString p_condition) const
{
  return "WHILE " + p_condition + " LOOP\n";
}

// Code to end a WHILE loop
CString
SQLInfoPostgreSQL::GetEndWhileLoop() const
{
  return "END LOOP;\n";
}

// Get the fact that we need to place a SELECT in parenthesis for an assignment in SPL
bool 
SQLInfoPostgreSQL::GetAssignmentSelectParenthesis() const
{
  // waarde = SELECT max kenmerk FROM tabel;
  return false;
}

// Gets the UPPER function
CString 
SQLInfoPostgreSQL::GetUpperFunction(CString& p_expression) const
{
  return "UPPER(" + p_expression + ")";
}

// Gets the construction for 1 minute ago
CString 
SQLInfoPostgreSQL::GetInterval1MinuteAgo() const
{
  return "ERROR";
}

// Gets the construction / select for generating a new serial identity
CString 
SQLInfoPostgreSQL::GetSQLGenerateSerial(CString p_table) const
{
  return "SELECT " + p_table + "_seq.nextval FROM DUAL";
}

// Gets the construction / select for the resulting effective generated serial
CString 
SQLInfoPostgreSQL::GetSQLEffectiveSerial(CString p_identity) const
{
  // Already the correct value
  return p_identity;
}

// Gets the Not-NULL-Value statement of the database
CString 
SQLInfoPostgreSQL::GetNVLStatement(CString& p_test,CString& p_isnull) const
{
  return "{fn IFNULL(" + p_test + "," + p_isnull + ")}";
}

// Gets the sub transaction commands
CString 
SQLInfoPostgreSQL::GetStartSubTransaction(CString p_savepointName) const
{
  return CString("SAVEPOINT ") + p_savepointName;
}

CString 
SQLInfoPostgreSQL::GetCommitSubTransaction(CString p_savepointName) const
{
  // No commit for a sub transaction
  return CString("");
}

CString 
SQLInfoPostgreSQL::GetRollbackSubTransaction(CString p_savepointName) const
{
  return CString("ROLLBACK TO SAVEPOINT ") + p_savepointName;
}

// SQL CATALOG QUERIES
// ===================================================================

// Get SQL to see if a stored procedure exists in the database
CString 
SQLInfoPostgreSQL::GetSQLStoredProcedureExists(CString& p_name) const
{
  return "SELECT count(*)\n"
         "  FROM pg_proc\n"
         " WHERE proname = '" + p_name + "'\n;";
}

// Part of a query to select 1 record 
CString
SQLInfoPostgreSQL::GetDualTableName() const
{
  // Not needed, can do "SELECT <expression>"
  return "";
}

// FROM-Part for a query to select only 1 (one) record
CString 
SQLInfoPostgreSQL::GetDualClause() const
{
  return "";
}

// Get DEFERRABLE clause to defer constraints
CString
SQLInfoPostgreSQL::GetConstraintDeferrable() const
{
  return " DEFERRABLE";
}

// Constraints deferred until the next commit
CString 
SQLInfoPostgreSQL::GetSQLDeferConstraints() const
{
  return "set transaction deferrable";
}

// Constraints back to immediate check
CString 
SQLInfoPostgreSQL::GetSQLConstraintsImmediate() const
{
  return "set transaction not deferrable";
}

// Gets the SQL query to check if a table exists in the database
CString 
SQLInfoPostgreSQL::GetSQLTableExists(CString p_schema,CString p_tablename) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  CString query = "SELECT count(*)\n"
                  "  FROM pg_class cl\n"
                  "      ,pg_namespace ns"
                  " WHERE cl.relnamespace = ns.oid\n"
                  "   AND cl.name = '" + p_schema    + "'\n"
                  "   AND relname = '" + p_tablename + "'";
  return query;
}

// Gets the SQL query to get the columns of a table from the catalog
CString 
SQLInfoPostgreSQL::GetSQLGetColumns(CString& /*p_user*/,CString& p_tableName) const
{
  CString tableName(p_tableName);
  tableName.MakeLower();

  CString query =  "SELECT attname\n"
                   "      ,attnum\n"
                   "      ,typname\n"
                   "      ,typlen\n"
                   "      ,attnotnull\n"
                   "      ,(atttypmod - 4) / 65536\n"
                   "      ,(atttypmod - 4) % 65536\n"
                   "  FROM pg_class, pg_attribute, pg_type\n"
                   " WHERE pg_class.relname = '" + tableName  + "'\n"
                   "   AND pg_class.relkind = 'r'\n"
                   "   AND pg_class.oid = pg_attribute.attrelid\n"
                   "   AND pg_attribute.atttypid = pg_type.oid\n"
                   "   AND attnum > 0\n"
                   "   AND typlen < 0\n"
                   "   AND typname = 'numeric'\n"
                   "UNION\n"
                   "SELECT attname\n"
                   "      ,attnum\n"
                   "      ,typname\n"
                   "      ,(atttypmod - 4) AS typlen\n"
                   "      ,attnotnull\n"
                   "      ,0\n"
                   "      ,0\n"
                   "  FROM pg_class, pg_attribute, pg_type\n"
                   " WHERE pg_class.relname = '" + tableName  + "'\n"
                   "   AND pg_class.relkind = 'r'\n"
                   "   AND pg_class.oid = pg_attribute.attrelid\n"
                   "   AND pg_attribute.atttypid = pg_type.oid\n"
                   "   AND attnum > 0\n"
                   "   AND typlen < 0\n"
                   "   AND typname = 'varchar'\n"
                   "UNION\n"
                   "SELECT attname\n"
                   "      ,attnum\n"
                   "      ,typname\n"
                   "      ,typlen\n"
                   "      ,attnotnull\n"
                   "      ,0\n"
                   "      ,0\n"
                   "  FROM pg_class, pg_attribute, pg_type\n"
                   " WHERE pg_class.relname = '" + tableName  + "'\n"
                   "   AND pg_class.relkind = 'r'\n"
                   "   AND pg_class.oid = pg_attribute.attrelid\n"
                   "   AND pg_attribute.atttypid = pg_type.oid\n"
                   "   AND attnum > 0\n"
                   "   AND typlen > 0\n"
                   "   AND typname not in ('numeric', 'varchar')\n"
                   " ORDER BY attnum";
  return query;
}

// Get the SQL query to get the CHECK and UNIQUE constraints from the catalog
CString 
SQLInfoPostgreSQL::GetSQLGetConstraintsForTable(CString& p_tableName) const
{
  CString lowerName(p_tableName);
  lowerName.MakeLower();
  CString contabel = "SELECT con.conname\n"
                     "      ,cla.relname\n"
                     "  FROM pg_constraint con, pg_class cla\n"
                     " WHERE con.contype in ('c','u')\n"
                     "   AND con.conrelid = cla.oid\n"
                     "   AND cla.relname = '" + lowerName + "'\n";
  return contabel;
}

// Read all existing indices of a table
CString 
SQLInfoPostgreSQL::GetSQLTableIndices(CString p_user,CString p_tableName) const
{
  p_user.MakeLower();
  p_tableName.MakeLower();

  CString query =  "SELECT cli.relname\n"
                   "      ,att.attname\n"
                   "      ,att.attnum\n"
                   "      ,idx.indisunique\n"
                   "      ,0  as ascending\n"
                   "      ,'' as expression_source"
                   "  FROM pg_user sha\n"
                   "      ,pg_class clr\n"
                   "      ,pg_index idx\n"
                   "      ,pg_class cli\n"
                   "      ,pg_attribute att\n"
                   " WHERE sha.usename = '" + p_user + "'\n"
                   "   AND sha.usesysid = clr.relowner\n"
                   "   AND clr.relname = '" + p_tableName+ "'\n"
                   "   AND clr.relkind = 'r'\n"
                   "   AND clr.oid = idx.indrelid\n"
                   "   AND idx.indexrelid = cli.oid\n"
                   "   AND cli.relkind = 'i'\n"
                   "   AND cli.oid = att.attrelid\n"
                   " ORDER BY cli.relname, att.attnum";
  return query;
}

// Get SQL to create an index for a table
CString 
SQLInfoPostgreSQL::GetSQLCreateIndex(CString p_user,CString p_tableName,DBIndex* p_index) const
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
SQLInfoPostgreSQL::GetSQLDropIndex(CString p_user,CString p_indexName) const
{
  CString sql = "DROP INDEX " + p_user + "." + p_indexName;
  return sql;
}

// Get the SQL queyr to get all the referential constraints from the catalog
CString 
SQLInfoPostgreSQL::GetSQLTableReferences(CString p_schema
                                        ,CString p_tablename
                                        ,CString p_constraint /*=""*/
                                        ,int     p_maxColumns /*=SQLINFO_MAX_COLUMNS*/) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  p_constraint.MakeLower();

  CString query;

  for(int ind = 1; ind <= p_maxColumns; ++ind)
  {
    CString part;
    part.Format("SELECT con.conname       as constraint_name\n"
                "      ,sch.nspname       as schema\n"
                "      ,cla.relname       as table_name\n"
                "      ,att.attname       as column_name\n"
                "      ,pri.relname       as primary_table_name\n"
                "      ,fky.attname       AS primary_column_name\n"
                "      ,con.condeferrable as deferrable\n"
                "      ,con.condeferred   as initially_deferred\n"
                "      ,1                 as enabled\n"
                "      ,case con.confmatchtype WHEN 's' THEN 1\n"
                "                              WHEN 'f' THEN 1\n"
                "                              ELSE  0\n"
                "                              END as match_option\n"
                "      ,case con.confdeltype   WHEN 'r' THEN 0\n"
                "                              WHEN 'c' THEN 1\n"
                "                              WHEN 'n' THEN 2\n"
                "                              WHEN 'd' THEN 3\n"
                "                              WHEN 'a' THEN 4\n"
                "                              ELSE  0\n"
                "                              END as update_rule\n"
                "      ,case con.confupdtype   WHEN 'r' THEN 0\n"
                "                              WHEN 'c' THEN 1\n"
                "                              WHEN 'n' THEN 2\n"
                "                              WHEN 'd' THEN 3\n"
                "                              WHEN 'a' THEN 4\n"
                "                              ELSE  0\n"
                "                              END as delete_rule\n"
                "  FROM pg_constraint con\n"
                "      ,pg_class      cla\n"
                "      ,pg_attribute  att\n"
                "      ,pg_namespace  sch\n"
                "      ,pg_class      pri\n"
                "      ,pg_attribute  fky\n"
                " WHERE con.contype      = 'f'\n"
                "   AND con.conrelid     = cla.oid\n"
                "   and cla.relnamespace = sch.oid\n"
                "   and con.confrelid    = pri.oid\n"
                "   and att.attrelid     = cla.oid\n"
                "   and att.attnum       = con.conkey[%d]\n"
                "   and fky.attrelid     = pri.oid\n"
                "   and fky.attnum       = con.confkey[%d]\n"
                "   AND cla.relname      = 'part'"
               ,ind
               ,ind);
    if(!p_schema.IsEmpty())
    {
      part += "\n   AND sch.nspname = '" + p_schema + "'";
    }
    if(!p_tablename.IsEmpty())
    {
      part += "\n   AND cla.relname = '" + p_tablename + "'";
    }
    if(!p_constraint.IsEmpty())
    {
      part += "\n   AND con.conname = '" + p_constraint + "'";
    }

    // Append to query, multiple for multiple columns
    if(!query.IsEmpty())
    {
      query += "\nUNION ALL\n";
    }
    query += part;
  }
  return query;
}

// Get the SQL to determine the sequence state in the database
CString 
SQLInfoPostgreSQL::GetSQLSequence(CString p_schema,CString p_tablename,CString p_postfix /*= "_seq"*/) const
{
  CString sequence = p_tablename + p_postfix;
  sequence.MakeLower();
  p_schema.MakeLower();

  CString sql = "SELECT sequence_name\n"
                "      ,start_value as current_value\n"
                "      ,decode(increment,'1',1,0) *\n"
                "       decode(cycle_option,'NO',1,0) as is_correct\n"
                "  FROM information_schema.sequences\n"
                " WHERE sequence_schema = '" + p_schema + "'\n"
                "   AND sequence_name   = '" + sequence + "'\n";
  return sql;
}

// Create a sequence in the database
CString 
SQLInfoPostgreSQL::GetSQLCreateSequence(CString p_schema,CString p_tablename,CString p_postfix /*= "_seq"*/,int p_startpos) const
{
  CString sql("CREATE SEQUENCE ");

  if(!p_schema.IsEmpty())
  {
    sql += p_schema + ".";
  }
  sql += p_tablename + p_postfix;
  sql.AppendFormat(" START WITH %d",p_startpos);
  sql += " CACHE 1 NO CYCLE";
  return sql;
}

// Remove a sequence from the database
CString 
SQLInfoPostgreSQL::GetSQLDropSequence(CString p_schema,CString p_tablename,CString p_postfix /*= "_seq"*/) const
{
  CString sql;
  sql = "DROP SEQUENCE " + p_schema + "." + p_tablename + p_postfix;
  return sql;
}

// Get the query to add rights to a sequence
CString
SQLInfoPostgreSQL::GetSQLSequenceRights(CString p_schema,CString p_tableName,CString p_postfix /*="_seq"*/) const
{
  CString sequence = p_tableName + p_postfix;
  if(!p_schema.IsEmpty())
  {
    sequence = p_schema + "." + sequence;
  }
  return "GRANT SELECT, UPDATE ON " + sequence + " TO " + GetGrantedUsers();
}

// Removing a stored procedure
void 
SQLInfoPostgreSQL::DoRemoveProcedure(CString& /*p_procedureName*/) const
{
  // To be implemented
}

CString
SQLInfoPostgreSQL::GetSQLSessionAndTerminal() const
{
  CString query = "SELECT procpid\n"
                  "      ,datname\n"
                  "  FROM pg_stat_activity\n"
                  " WHERE procpid = pg_backend_pid()";    
  return query;
}

// Get SQL to check if session number exists
CString 
SQLInfoPostgreSQL::GetSQLSessionExists(CString sessieId) const
{
  return "";
}

// Get SQL query for the unique session id
CString 
SQLInfoPostgreSQL::GetSQLUniqueSessionId(const CString& /*p_databaseName*/,const CString& /*p_sessionTable*/) const
{
  CString query = "SELECT DISTINCT procpid\n"
                  "  FROM pg_stat_activity\n"
                  " WHERE procpid <> 0";
  return query;
}

// Finding the extra sessions
CString 
SQLInfoPostgreSQL::GetSQLSearchSession(const CString& /*p_databaseName*/,const CString& p_sessionTable) const
{
  CString query = "SELECT  procpid\n"
                  "       ,usename\n"
                  "       ,datname\n"
                  "  FROM pg_stat_activity\n"
                  " WHERE procpid <> 0\n"
                  "   AND NOT procpid IN\n"
                  "     ( SELECT sessie_nr\n"
                  "         FROM "+ p_sessionTable + ")";
  return query;
}

// See if a column exists in a table
bool   
SQLInfoPostgreSQL::DoesColumnExistsInTable(CString& p_owner,CString& p_tableName,CString& p_column) const
{
  int number = 0;
  CString lowerOwner(p_owner);
  CString lowerTable(p_tableName);
  CString lowerColumn(p_column);
  lowerOwner.MakeLower();
  lowerTable.MakeLower();
  lowerColumn.MakeLower();

  CString query = "SELECT count(*)\n"
                 "  FROM pg_class a, pg_attribute b, pg_user c\n"
                 " WHERE a.relname  = '" + lowerTable + "'\n"
                 "   AND a.OID = b.attrelid\n"
                 "   AND b.attname = '" + lowerColumn + "'\n"
                 "   AND a.relowner = c.usesysid\n"
                 "   AND c.usename = '" + lowerOwner  + "'";
  SQLQuery sql(m_database);
  sql.DoSQLStatement(query);
  if (sql.GetRecord())
  {
    number = sql.GetColumn(1)->GetAsSLong();
  }
  return (number == 1);
}

// Get SQL query to see if a table already has a primary key
CString
SQLInfoPostgreSQL::GetSQLPrimaryKeyConstraintInformation(CString p_schema,CString p_tableName) const
{
  p_schema.MakeLower();
  p_tableName.MakeLower();

  CString query = "SELECT count(*)\n"
                  "  FROM pg_class a\n"
                  "      ,pg_constraint b\n"
                  "      ,pg_namespaces n\n"
                  " WHERE a.relnamespace = n.oid\n"
                  "   AND a.oid     = b.conrelid\n"
                  "   AND b.contype = 'p'"
                  "   AND n.name    = '" + p_schema    + "'\n"
                  "   AND a.relname = '" + p_tableName + "'\n";
  return query;
}

// Does the named constraint exist in the database
bool    
SQLInfoPostgreSQL::DoesConstraintExist(CString /*p_constraintName*/) const
{
  return true;
}

// Get the lock-table query
CString 
SQLInfoPostgreSQL::GetSQLLockTable(CString& p_tableName,bool p_exclusive) const
{
  CString query = "LOCK TABLE " + p_tableName + " IN "  ;
  query += p_exclusive ? "EXCLUSIVE" : "SHARE";
  query += " MODE";
  return query;
}

// Get the SQL query to optimize a table
CString 
SQLInfoPostgreSQL::GetSQLOptimizeTable(CString& p_owner,CString& p_tableName,int& p_number)
{
  CString optim = "VACUUM ANALYZE " + p_owner + "." + p_tableName + ";\n";
  p_number += 1;
  return optim;
}

// Getting the fact that there is only **one** (1) user session in the database
bool
SQLInfoPostgreSQL::GetOnlyOneUserSession()
{
  // Yet to implement
  return true;
}

// SQL DDL STATEMENTS
// ==================

CString
SQLInfoPostgreSQL::GetCreateColumn(CString p_schema,CString p_tablename,CString p_columnName,CString p_typeDefinition,bool p_notNull)
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
SQLInfoPostgreSQL::GetSQLDropColumn(CString p_schema,CString p_tablename,CString p_columnName) const
{
  return "ALTER TABLE " + p_schema + "." + p_tablename + "\n"
         " DROP COLUMN " + p_columnName;
}

// Add a foreign key to a table
CString
SQLInfoPostgreSQL::GetCreateForeignKey(CString p_tablename,CString p_constraintname,CString p_column,CString p_refTable,CString p_primary)
{
  CString sql = "ALTER TABLE " + p_tablename + "\n"
                "  ADD CONSTRAINT " + p_constraintname + "\n"
                "      FOREIGN KEY (" + p_column + ")\n"
                "      REFERENCES " + p_refTable + "(" + p_primary + ")";
  return sql;
}

CString 
SQLInfoPostgreSQL::GetModifyColumnType(CString p_schema,CString p_tablename,CString p_columnName,CString p_typeDefinition)
{
  CString sql = "ALTER TABLE "   + p_schema + "." + p_tablename + "\n"
                " ALTER COLUMN " + p_columnName + 
                " SET DATA TYPE " + p_typeDefinition;
  return sql;
}

CString 
SQLInfoPostgreSQL::GetModifyColumnNull(CString p_schema,CString p_tablename,CString p_columnName,bool p_notNull)
{
  CString sql = "ALTER TABLE  " + p_schema + "." + p_tablename + "\n"
                "ALTER COLUMN " + p_columnName + (p_notNull ? " SET " : " DROP ") + "NOT NULL";
  return sql;
}

// Get the SQL to drop a view. If precursor is filled: run that SQL first!
CString 
SQLInfoPostgreSQL::GetSQLDropView(CString p_schema,CString p_view,CString& p_precursor)
{
  p_precursor.Empty();
  return "DROP VIEW " + p_schema + "." + p_view;
}

// Create or replace a database view
CString 
SQLInfoPostgreSQL::GetSQLCreateOrReplaceView(CString p_schema,CString p_view,CString p_asSelect) const
{
  return "CREATE OR REPLACE VIEW " + p_schema + "." + p_view + "\n" + p_asSelect;
}

// SQL DDL ACTIONS
// ===================================================================

// Committing the DDL commands explicitly
void 
SQLInfoPostgreSQL::DoCommitDDLcommands() const
{
  SQLQuery sql(m_database);
  sql.DoSQLStatement("commit");
}

// ODBC driver auto commit goes wrong for this engine!
void
SQLInfoPostgreSQL::DoCommitDMLcommands() const
{
  SQLQuery sql(m_database);
  sql.DoSQLStatement("commit");
}

// Remove a column from a table
void
SQLInfoPostgreSQL::DoDropColumn(CString p_tableName,CString p_columName)
{
  CString sql = "ALTER TABLE  " + p_tableName + "\n"
                " DROP COLUMN " + p_columName;
  SQLQuery query(m_database);
  query.TryDoSQLStatement(sql);
  DoCommitDDLcommands();
}

// Does the named view exists in the database
bool
SQLInfoPostgreSQL::DoesViewExists(CString& p_viewName)
{
  CString viewname(p_viewName);
  viewname.MakeUpper();
  SQLQuery query(m_database);
  query.DoSQLStatement("SELECT COUNT(*)\n"
                       "  FROM pg_views\n"  // TODO: Check the correct name
                       " WHERE view_name = '" + viewname + "'");
  if(query.GetRecord())
  {
    return query.GetColumn(1)->GetAsSLong() == 1;
  }
  return false;
}

// Can create temporary tables at runtime?
bool 
SQLInfoPostgreSQL::GetMustMakeTemptablesAtRuntime() const
{
  return false;
}

// Create a temporary table in an optimized way
void
SQLInfoPostgreSQL::DoMakeTemporaryTable(CString& p_tableName,CString& p_content,CString& p_indexColumn) const
{
  SQLQuery sql(m_database);

  sql.TryDoSQLStatement("TRUNCATE TABLE " + p_tableName);
  sql.TryDoSQLStatement("DROP TABLE " + p_tableName);
  CString create = "CREATE GLOBAL TEMPORARY TABLE " + p_tableName + "\n"
                   "WITHOUT OIDS\n"
                   "ON COMMIT PRESERVE ROWS\n" + p_content;
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
    throw CString("Cannot create temporary table: " + p_tableName);
  }
}

// Remove temporary tabel
void
SQLInfoPostgreSQL::DoRemoveTemporaryTable(CString& p_tableName) const
{
  SQLQuery sql(m_database);
  sql.TryDoSQLStatement("DELETE FROM "    + p_tableName);
  sql.TryDoSQLStatement("TRUNCATE TABLE " + p_tableName);
  sql.TryDoSQLStatement("DROP TABLE "     + p_tableName);
}

// Create a stored procedure in the database
void 
SQLInfoPostgreSQL::DoMakeProcedure(CString& p_procName,CString p_table,bool p_noParameters,CString& p_codeBlock)
{
  SQLQuery sql(m_database);
  sql.DoSQLStatement(p_codeBlock);
  CString grant_statement = "GRANT EXECUTE ON FUNCTION " + p_procName + "(";
  if (p_procName.Find("_ins_proc")  > 0 || 
      p_procName.Find("_upd_proc")  > 0 || 
      p_procName.Find("_fupd_proc") > 0 )
  {
    grant_statement += p_table + ")";
  }
  else
  {
    if (p_procName.Find("_del_proc") > 0)
    {
      grant_statement += "int4, \"timestamp\", \"timestamp\", \"varchar\")";
    }
    else if (p_noParameters)
    {
      grant_statement += ")";
    }
    else
    {
      grant_statement += p_procName + "_proc_in_par)";
    }
  }
  grant_statement += " TO PUBLIC;";
  sql.DoSQLStatement(grant_statement);
}

// Way to rename a table
void 
SQLInfoPostgreSQL::DoRenameTable(CString& p_oldName,CString& p_newName) const
{
  SQLQuery sql(m_database);
  // Beware: no 'TABLE' in the statement
  CString rename = "rename " + p_oldName + " to " + p_newName;
  sql.DoSQLStatement(rename);
  DoCommitDDLcommands();
}

// PERSISTENT-STORED MODULES (SPL / PL/SQL)
// ====================================================================

// Getting the user errors from the database
CString 
SQLInfoPostgreSQL::GetUserErrorText(CString& p_procName) const
{
  (void)p_procName;   // Not supported in postgreSQL.
  return "";
}

// Getting the assignment to a variable in SPL
CString 
SQLInfoPostgreSQL::GetSPLAssignment(CString p_variable) const
{
  return p_variable + " := ";
}

// Getting the start of a WHILE loop
CString 
SQLInfoPostgreSQL::GetSPLStartWhileLoop(CString p_condition) const
{
  return "WHILE " + p_condition + " LOOP\n";
}

// At the end of a WHILE loop
CString 
SQLInfoPostgreSQL::GetSPLEndWhileLoop() const
{
  return "END LOOP;\n";
}

// Calling a stored procedure in SPL
CString 
SQLInfoPostgreSQL::GetSQLSPLCall(CString p_procName) const
{
  CString subsys = p_procName.Left(p_procName.Find("?"));
  return "SELECT " + p_procName + ";";
}

// Build a parameter list for a call of a stored procedure within SPL
CString 
SQLInfoPostgreSQL::GetBuildedParameterList(size_t p_numOfParameters) const
{
  // IF NO parameters: NO ellipsis
  CString strParamLijst;
  if(p_numOfParameters >= 0)
  {
    for (size_t i = 0; i < p_numOfParameters; i++)
    {
      if(i != 0) 
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

CString
SQLInfoPostgreSQL::GetParameterType(CString& p_type) const
{
  CString retval;
  if (_strnicmp(p_type,"char",4) == 0 ||
      _strnicmp(p_type,"varchar",7) == 0 )
  {
    retval = "varchar";
  }
  else if (_strnicmp(p_type,"decimal",7) == 0 )
  {
    retval = "decimal";
  }
  else 
  {
    retval = p_type;
  }
  return retval;
}

CString 
SQLInfoPostgreSQL::GetSQLString(const CString& p_string) const
{
  CString s = p_string;
  s.Replace("'","''");
  CString kwoot = GetQuoteCharacter();
  return  kwoot + s + kwoot;
}

CString 
SQLInfoPostgreSQL::GetSQLDateString(int p_year,int p_month,int p_day) const
{
  CString dateString;
  dateString.Format("TO_DATE('%04d-%02d-%02d','YYYY-MM-DD')",p_year,p_month,p_day);
  return dateString;
}  
  
CString 
SQLInfoPostgreSQL::GetSQLTimeString(int p_hour,int p_minute,int p_second) const
{
  CString timeString;
  timeString.Format("'%02d:%02d:%02d'",p_hour,p_minute,p_second);
  return timeString;
}  
  
CString 
SQLInfoPostgreSQL::GetSQLDateTimeString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  CString retval;
  retval.Format("to_timestamp('%04d-%02d-%02d %02d:%02d:%02d','YYYY-MM-DD HH24:MI:SS')::timestamp"
               ,p_year,p_month,p_day,p_hour,p_minute,p_second);
  return retval;
}  
  
// Get date-time bound parameter string in database format
CString 
SQLInfoPostgreSQL::GetSQLDateTimeBoundString() const
{
  return "to_timestamp(?,'YYYY-MM-DD HH24:MI:SS')::timestamp";
}

// Stripped data for the parameter binding
CString
SQLInfoPostgreSQL::GetSQLDateTimeStrippedString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  CString retval;
  retval.Format("%04d-%02d-%02d %02d:%02d:%02d"
                ,p_year,p_month,p_day,p_hour,p_minute,p_second);
  return retval;
}

CString 
SQLInfoPostgreSQL::GetSPLSourcecodeFromDatabase(const CString& p_owner,const CString& p_procName) const
{
  CString sQuery;  
  sQuery = "SELECT TEXT from ALL_SOURCE "
           "WHERE type = 'FUNCTION' "
           "AND name  = LOWER('" + p_procName +  "') "
           "AND owner = LOWER('" + p_owner + "')";
  SQLQuery sql(m_database);
  sql.TryDoSQLStatement(sQuery);
  CString sProcBody = "CREATE OR REPLACE ";
  while (sql.GetRecord())
  {
    sProcBody += sql.GetColumn(1)->GetAsChar();
  }
  return sProcBody;
}

// Get the SPL datatype for integer
CString 
SQLInfoPostgreSQL::GetSPLIntegerType() const
{
  return "integer";
}

// Get the SPL datatype for a decimal
CString 
SQLInfoPostgreSQL::GetSPLDecimalType() const
{
  return "decimal";
}

// Get the SPL declaration for a cursor
CString 
SQLInfoPostgreSQL::GetSPLCursorDeclaratie(CString& p_variableName,CString& p_query) const
{
  return "cursor " + p_variableName + " is " + p_query + ";";
}

// Get the SPL cursor found row parameter
CString 
SQLInfoPostgreSQL::GetSPLCursorFound(CString& /*p_cursorName*/) const
{
  // TODO: To be implemented
  return "";
}

// Get the SPL cursor row-count variable
CString 
SQLInfoPostgreSQL::GetSPLCursorRowCount(CString& /*p_variable*/) const
{
  // TODO: To be implemented
  return "";
}

// Get the SPL datatype for a declaration of a row-variable
CString 
SQLInfoPostgreSQL::GetSPLCursorRowDeclaration(CString& /*p_cursorName*/,CString& /*p_variableName*/) const
{
  // TODO: To be implemented
  return "";
}

CString 
SQLInfoPostgreSQL::GetSPLFetchCursorIntoVariables(CString               p_cursorName
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
SQLInfoPostgreSQL::GetSPLFetchCursorIntoRowVariable(CString& p_cursorName,CString p_variableName) const
{ 
  // TODO: Check
  return "FETCH " + p_cursorName + " INTO " + p_variableName+ ";";
}

// Get the SPL no-data exception clause
CString 
SQLInfoPostgreSQL::GetSPLNoDataFoundExceptionClause() const
{
  // TODO: Check
  return "WHEN NO_DATA_FOUND THEN";
}

// Get the SPL form of raising an exception
CString 
SQLInfoPostgreSQL::GetSPLRaiseException(CString p_exceptionName) const
{
  // TODO: Check
  return "RAISE " + p_exceptionName + ";";
}

// Get the fact that the SPL has server functions that return more than 1 value
bool    
SQLInfoPostgreSQL::GetSPLServerFunctionsWithReturnValues() const
{
  return true;
}

// Calling a stored function or procedure if the RDBMS does not support ODBC call escapes
SQLVariant*
SQLInfoPostgreSQL::DoSQLCall(SQLQuery* p_query,CString& p_schema,CString& p_procedure)
{
  // PostgreSQL does not support the return parameter of the "{[?=]CALL procedure(?,?)}" sequence
  // instead you have to do a "SELECT procedure(?,?)" 
  // The result set is the set of output parameters
  // DOES ONLY SUPPORT A SINGLE ROW RESULT SET!!
  SQLQuery query(m_database);
  CString sql   = ConstructSQLForProcedureCall(p_query,&query,p_schema,p_procedure);
  int numReturn = GetCountReturnParameters(p_query);

  query.DoSQLStatement(sql);
  if(query.GetRecord())
  {
    // Processing the result
    int setIndex = -1;
    int recIndex = 0;
    for(int resIndex = 1; resIndex <= query.GetNumberOfColumns(); ++resIndex)
    {
      SQLVariant var;
      int dataType = 0;
      int type = 0;
      bool ready = false;

      // Finding the next OUTPUT parameter in the original query call
      do
      {
        SQLVariant* target = p_query->GetParameter(++setIndex);
        if(target == nullptr)
        {
          throw CString("Wrong number of output parameters for procedure call");
        }
        type = target->GetParameterType();
        dataType = target->GetDataType();
      }
      while(type != SQL_PARAM_OUTPUT && type != SQL_PARAM_INPUT_OUTPUT);

      // Getting the next result from the result set
      SQLVariant* result = query.GetColumn(resIndex);
      if(result->GetDataType() == SQL_C_CHAR)
      {
        const char* resPointer = result->GetAsChar();
        if(resPointer && *resPointer == '(' && numReturn)
        {
          var = GetVarFromRecord(dataType,(char*)resPointer,recIndex++,ready);
          resIndex = 0;
          result = &var;
        }
      }

      // Storing the result;
      p_query->SetParameter(setIndex,result);

      // At the end of a multi-parameter record?
      if(ready)
      {
        break;
      }
    }
    // Returning the first return column as the result of the procedure
    return p_query->GetParameter(0);
  }
  return nullptr;
}

// SPECIALS
// ==========================================================================

// Translate database-errors to a human readable form
CString 
SQLInfoPostgreSQL::TranslateErrortext(int p_error,CString p_errorText) const
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

//////////////////////////////////////////////////////////////////////////
//
// PRIVATE METHODS
//
//////////////////////////////////////////////////////////////////////////

// Get the number of OUTPUT or INPUT_OUTPUT parameters
// In the parameter list (disregarding the 0th parameter)
int
SQLInfoPostgreSQL::GetCountReturnParameters(SQLQuery* p_query)
{
  int count = 0;
  int index = 1;

  while(true)
  {
    var* parameter = p_query->GetParameter(index++);
    if(parameter == nullptr) break;
    int type = parameter->GetParameterType();
    if((type == SQL_PARAM_OUTPUT || type == SQL_PARAM_INPUT_OUTPUT))
    {
      ++count;
    }
  }
  return count;
}

// Construct the "SELECT * FROM procedure(?,?)" (input parameters ONLY!)
CString
SQLInfoPostgreSQL::ConstructSQLForProcedureCall(SQLQuery* p_query
                                               ,SQLQuery* p_thecall
                                               ,CString&  p_schema
                                               ,CString&  p_procedure)
{
  // Start with select form
  CString sql = "SELECT ";
  if(!p_schema.IsEmpty())
  {
    sql += p_schema;
    sql += ".";
  }
  sql += p_procedure;

  // Opening parenthesis
  sql += "(";

  // Build list of markers
  int ind = 1;
  int res = 1;
  while(true)
  {
    // Try get the next parameter
    var* parameter = p_query->GetParameter(ind);
    if(parameter == nullptr) break;

    // Input parameters ONLY!!
    int type = parameter->GetParameterType();
    if(type == SQL_PARAM_INPUT)
    {
      // Add marker
      if(ind > 1) sql += ",";
      sql += "?";

      // Add the parameter with the result counter!
      p_thecall->SetParameter(res++,parameter);
    }
    // Next parameter
    ++ind;
  }

  // CLosing parenthesis
  sql += ")";

  // The procedure IS the singular object
  // Procedure **MUST** end with "SUSPEND;" 
  return sql;
}

// Get column from PostgreSQL result set
// (123,"string",21.12)
SQLVariant
SQLInfoPostgreSQL::GetVarFromRecord(int p_type,char* p_pointer,int p_column,bool& p_ready)
{
  int beginPos  = 0;
  int endPos    = 0;
  int curColumn = 0;
  SQLVariant variant;

  // Skip first '('
  if(*p_pointer == '(') ++beginPos;

  while(p_pointer[beginPos])
  {
    // Find begin next column
    bool isString = p_pointer[beginPos] == '\"';
    if(isString) ++beginPos;
    endPos = beginPos;

    // Find end of the field
    while(p_pointer[endPos] && p_pointer[endPos] != ','
                            && p_pointer[endPos] != ')' 
                            && p_pointer[endPos] != '\"')
    {
      ++endPos;
    }

    // If column found
    if(p_column == curColumn)
    {
      char temp = p_pointer[endPos];
      p_pointer[endPos] = 0;
      variant.SetData(p_type,&p_pointer[beginPos]);
      p_pointer[endPos] = temp;
      break;
    }

    // Next column
    beginPos = endPos;
    if(isString && p_pointer[beginPos] == '\"') ++beginPos;
    if(p_pointer[beginPos] == ',' || p_pointer[beginPos] == ')') ++beginPos;
    ++curColumn;
  }

  // Are we ready with the record
  if(p_pointer[endPos] == '\"') ++ endPos;
  if(p_pointer[endPos] == ')')
  {
    p_ready = true;
  }
  return variant;
}

// End of namespace
}
