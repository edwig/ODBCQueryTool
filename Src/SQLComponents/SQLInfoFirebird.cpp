////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoFirebird.cpp
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
#include "SQLInfoFirebird.h"
#include "SQLQuery.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{
  
// Constructor.
SQLInfoFirebird::SQLInfoFirebird(SQLDatabase* p_database)
                :SQLInfoDB(p_database)
{
}

// Destructor. Does nothing
SQLInfoFirebird::~SQLInfoFirebird()
{
}

// Get the database type
DatabaseType 
SQLInfoFirebird::GetDatabaseType() const
{
  return RDBMS_FIREBIRD;
}

// BOOLEANS EN STRINGS
// ====================================================================

// The name of the database vendor
CString
SQLInfoFirebird::GetDatabaseVendorName() const
{
  return "Firebird";
}

// Get the physical database name
CString 
SQLInfoFirebird::GetPhysicalDatabaseName() const
{
  // See to it that "SQLDatabase:GetDatabaseName" does it's work
  return m_database->GetDatabaseName();
}

// System catalog is stored in uppercase in the database?
bool 
SQLInfoFirebird::IsCatalogUpper() const
{
  return true;
}

// System catalog supports full ISO schemas (same tables per schema)
bool
SQLInfoFirebird::GetUnderstandsSchemas() const
{
  return false;
}

// Supports database/ODBCdriver comments in sql
bool 
SQLInfoFirebird::SupportsDatabaseComments() const
{
  return false;
}

// Database can defer constraints until the end of a transaction
bool 
SQLInfoFirebird::SupportsDeferredConstraints() const
{
  // No SET CONSTRAINTS DEFERRED 
  return false;
}

// Database has ORDER BY with an expression, e.g. ORDER BY UPPER(column-name)
// Work-around is "SELECT UPPER(column-name) AS something.....ORDER BY something
bool
SQLInfoFirebird::SupportsOrderByExpression() const
{
  return true;
}

// Supports the ODBC escape sequence {[?=] CALL procedure (?,?,?)}
bool    
SQLInfoFirebird::SupportsODBCCallEscapes() const
{
  return false;
}

// Catalog table with all default values for a column in a table
CString 
SQLInfoFirebird::GetSQLStringDefaultValue(CString p_tableName,CString p_columnName) const
{
  p_tableName.MakeUpper();
  p_columnName.MakeUpper();
  
  CString query = "SELECT rdb$default_source\n"
                  "  FROM rdb$relation_fields\n"
                  " WHERE rdb$relation_name = '" + p_tableName   + "'\n"
                  "   AND rdb$field_name    = '" + p_columnName  + "'";  
  return query;
}

CString 
SQLInfoFirebird::GetSystemDateTimeKeyword() const
{
  return "current_timestamp";
}  

// String for the current date
CString
SQLInfoFirebird::GetSystemDateString() const
{
  return "current_date";
}

// Datatype TIME can be implemented as a decimal value on the database
bool 
SQLInfoFirebird::GetTimeIsDecimal() const
{
  // TIJD is implemented as DECIMAL(18,16)
  return true;
}

// Datatype INTERVAL can be implemented as a decimal value on the database
bool 
SQLInfoFirebird::GetIntervalIsDecimal() const
{
  // Interval is implemented in Firebird as a DECIMAL
  return true;
}

// Concatenation operator character string
CString 
SQLInfoFirebird::GetConcatanationOperator() const
{
  return "||";    
}

// Quote character for a string
CString 
SQLInfoFirebird::GetQuoteCharacter() const
{
  return "\'";    
}

// Default NULL for an input parameter list
CString 
SQLInfoFirebird::GetDefaultNULL() const
{
  return "";    
}

// INOUT prefix for an input/output parameter in a parameter list
CString 
SQLInfoFirebird::GetParameterINOUT() const
{
  return "";    
}

// OUT prefix for an input/output parameter in a parameter list
CString 
SQLInfoFirebird::GetParameterOUT() const
{
  return "";    
}

// Datatype for the last user in a stored procedure as a parameter
CString 
SQLInfoFirebird::GetAuditUserDatatype() const
{
  return "CHAR(8)";
}

// Datatype for a last-user in a stored procedure as a variable
CString 
SQLInfoFirebird::GetAuditUserDatatypeAsVariable() const
{
  return "CHAR(8)";
}

// Datatype of a primary key
CString 
SQLInfoFirebird::GetPrimaryKeyType() const
{
  return "INTEGER";
}

// Datatype for a timestamp (DATETIME YEAR TO SECOND)
CString 
SQLInfoFirebird::GetDatetimeYearToSecondType() const
{
  return "timestamp";
}

// Separator between two alter-constraint statements in one alter-table statement
CString 
SQLInfoFirebird::GetAlterConstraintSeparator() const
{
  return ",";
}

// Inner Join Keyword
CString 
SQLInfoFirebird::GetInnerJoinKeyword() const
{
  return "INNER JOIN ";
}

// Outer join keyword
CString  
SQLInfoFirebird::GetOuterJoinKeyword() const
{
  return "LEFT OUTER JOIN ";
}

// Inner Join Keyword for use in views.
CString 
SQLInfoFirebird::GetViewInnerJoinKeyword() const
{
  return "INNER JOIN";
}

// Outer join keyword for use in views
CString 
SQLInfoFirebird::GetViewOuterJoinKeyword() const
{
  return "LEFT OUTER JOIN";
}

// OUTER JOIN closer (+ in some databases)
CString 
SQLInfoFirebird::GetOuterJoinClosure() const
{
  return "";
}

// Outer join sign
CString  
SQLInfoFirebird::GetOuterJoinSign() const
{
  return "";
}

// Prefix before using a stored procedure parameter
CString
SQLInfoFirebird::GetSPParamPrefix() const
{
  return ":";
}

// Get the query to add a new record to the primary key column
CString 
SQLInfoFirebird::GetIdentityString(CString& p_tablename,CString p_postfix /*="_seq"*/) const
{
  return "GEN_ID(" + p_tablename + p_postfix + ",1)";
}

// Get the SQL Query to create a temporary table from a select statement;
CString 
SQLInfoFirebird::GetSQLCreateTemporaryTable(CString& p_tablename,CString p_select) const
{
  return "CREATE GLOBAL TEMPORARY TABLE " + p_tablename + "\nAS " + p_select +
         "ON COMMIT PRESERVE ROWS";
}

// Get the SQL query to remove a temporary table
CString
SQLInfoFirebird::GetSQLRemoveTemporaryTable(CString& p_tablename,int& p_number) const
{
  // 'Simply' a drop from a table
  p_number += 1;
  return "DROP TABLE "     + p_tablename + ";\n";
}

// Getting the SQL query to select into a temporary table
CString 
SQLInfoFirebird::GetSQLSelectIntoTemp(CString& p_tablename,CString& p_select) const
{
  return "INSERT INTO " + p_tablename + "\n" + p_select + ";\n";
}

// Replace primary key column OID by a sequence.nextval / GENID()
CString 
SQLInfoFirebird::GetReplaceColumnOIDbySequence(CString p_columns,CString p_tablename,CString p_postfix /*="_seq"*/) const
{
  int pos;
  int len = p_columns.GetLength();
  pos = p_columns.Find("OID,");
  if(pos == 0)
  {
    CString newkolom = ( p_tablename + p_postfix + ".genid()");
    newkolom += p_columns.Right(len - 3);
    return newkolom;
  }
  if(pos > 0)
  {
    pos = p_columns.Find(",OID,");
    CString newkolom = p_columns.Left(pos) + "," + p_tablename + p_postfix + ".genid()";
    newkolom += p_columns.Right(len - pos - 5);
    return newkolom;
  }
  return p_columns;
}

// Remove catalog dependencies
CString 
SQLInfoFirebird::GetSQLRemoveProcedureDependencies(CString p_procname) const
{
  CString upperName = p_procname;
  upperName.MakeUpper();
  CString query = "DELETE FROM rdb$dependencies\n"
                  " WHERE rdb$depended_on_name = '" + upperName + "'\n"
                  "   AND rdb$depended_on_type = 5\n"
                  "   AND rdb$dependent_type   = 5";
  return query;
}

CString
SQLInfoFirebird::GetSQLRemoveFieldDependencies(CString p_tablename) const
{
  CString upperName = p_tablename;
  upperName.MakeUpper();
  CString query = "DELETE FROM rdb$dependencies\n"
                  " WHERE rdb$depended_on_name = '" + upperName + "'\n"
                  "   AND rdb$dependent_type   = 3";
  return query;
}

// Getting the create table form for a primary key
CString 
SQLInfoFirebird::GetPrimaryKeyDefinition(CString p_schema,CString p_tableName,bool p_temporary) const
{
  CString keyDefinitie = GetPrimaryKeyType() + " NOT NULL\n";
  keyDefinitie += p_temporary ? CString("\n") : " CONSTRAINT pk_" + p_tableName + " PRIMARY KEY\n";
  return keyDefinitie;
}

// Getting the constraint form for adding a primary key to a table
CString
SQLInfoFirebird::GetPrimaryKeyConstraint(CString /*p_schema*/,CString p_tablename,CString p_primary) const
{
  return "ALTER TABLE " + p_tablename + "\n"
         "  ADD CONSTRAINT pk_" + p_tablename + "\n"
         "      PRIMARY KEY (" + p_primary + ")";
}

CString
SQLInfoFirebird::GetPrimaryKeyConstraint(MPrimaryMap& p_primaries) const
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

// Get the sql to add a foreign key to a table
CString 
SQLInfoFirebird::GetSQLForeignKeyConstraint(DBForeign& p_foreign) const
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
  switch(p_foreign.m_updateRule)
  {
    case 0: query += "\n      ON UPDATE CASCADE";     break;
    case 2: query += "\n      ON UPDATE SET NULL";    break;
    case 4: query += "\n      ON UPDATE SET DEFAULT"; break;
    case 3: query += "\n      ON UPDATE NO ACTION";   break;
    default:// In essence: ON UPDATE RESTRICT, but that's already the default
    case 1: break;
  }
  switch(p_foreign.m_deleteRule)
  {
    case 0: query += "\n      ON DELETE CASCADE";     break;
    case 2: query += "\n      ON DELETE SET NULL";    break;
    case 4: query += "\n      ON DELETE SET DEFAULT"; break;
    case 3: query += "\n      ON DELETE NO ACTION";   break;
    default:// In essence: ON DELETE RESTRICT, but that's already the default
    case 1: break;
  }
  return query;
}

CString 
SQLInfoFirebird::GetSQLForeignKeyConstraint(MForeignMap& p_foreigns) const
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

  // Add all relevant options
  switch(foreign.m_updateRule)
  {
    case SQL_CASCADE:     query += "\n      ON UPDATE CASCADE";     break;
    case SQL_SET_NULL:    query += "\n      ON UPDATE SET NULL";    break;
    case SQL_SET_DEFAULT: query += "\n      ON UPDATE SET DEFAULT"; break;
    case SQL_NO_ACTION:   query += "\n      ON UPDATE NO ACTION";   break;
    default:              // In essence: ON UPDATE RESTRICT, but that's already the default
                          break;
  }
  switch(foreign.m_deleteRule)
  {
    case SQL_CASCADE:     query += "\n      ON DELETE CASCADE";     break;
    case SQL_SET_NULL:    query += "\n      ON DELETE SET NULL";    break;
    case SQL_SET_DEFAULT: query += "\n      ON DELETE SET DEFAULT"; break;
    case SQL_NO_ACTION:   query += "\n      ON DELETE NO ACTION";   break;
    default:              // In essence: ON DELETE RESTRICT, but that's already the default
                          break;
  }
  return query;
}

// Get the sql (if possible) to change the foreign key constraint
CString 
SQLInfoFirebird::GetSQLAlterForeignKey(DBForeign& /*p_origin*/,DBForeign& /*p_requested*/) const
{
  // Firebird cannot alter a foreign-key constraint.
  // You must drop and then re-create your foreign key constraint
  // So return an empty string to signal this!
  return "";
} 

// Setting SQL performance parameters in the database
CString
SQLInfoFirebird::GetSQLPerformanceSettings() const
{
  return "";
}

// SQL To set the caching mode of SQL results
CString 
SQLInfoFirebird::GetSQLCacheModeSetting(const CString& /*p_mode*/) const
{
  return "";
}

// Settings for NUMERIC/DECIMAL numbers in stored procedures
CString
SQLInfoFirebird::GetSQLNlsNumericCharacters() const
{
  return "";
}

// Getting the RENAME column (can contain more than 1 SQL)
CString
SQLInfoFirebird::GetSQLModifyColumnName(CString p_tablename,CString p_oldName,CString p_newName,CString /*p_datatype*/)
{
  CString rename = "rename column " + p_tablename + "." + p_oldName + " to " + p_newName;
  return rename;
}

// Max length of an SQL statement
unsigned long 
SQLInfoFirebird::GetMaxStatementLength() const
{
  return 0;		// NO limit
}

// Getting the prefix for altering the datatyp in MODIFY/ALTER
CString 
SQLInfoFirebird::GetModifyDatatypePrefix() const
{
  return "TYPE ";
}

// Code to create a temporary table
CString 
SQLInfoFirebird::GetCodeTemporaryTable() const
{
  return "";
}

// Getting the correct locking mode
CString 
SQLInfoFirebird::GetCodeLockModeRow() const
{
  return "";
}

// Extra create table clause for creating a table with no logging
CString 
SQLInfoFirebird::GetCodeTempTableWithNoLog() const
{
  return "";
}

// Giving rights to configured users in a NON-ANSI database
CString 
SQLInfoFirebird::GetSQLGrantAllOnTable(CString /*p_schema*/,CString p_tableName,bool p_grantOption /*= false*/)
{
  CString sql = "GRANT ALL ON "+ p_tableName + " TO" + GetGrantedUsers();
  if(p_grantOption)
  {
    sql += " WITH GRANT OPTION;\n";
  }
  return sql;
}

// Code BEFORE a 'select into temp' clause
CString
SQLInfoFirebird::GetSelectIntoTempClausePrefix(CString p_tableName) const
{
  return "";
}

// Code AFTER a 'select into temp' clause
CString
SQLInfoFirebird::GetSelectIntoTempClauseSuffix(CString p_tableName) const
{
  return "";
}

bool
SQLInfoFirebird::GetCodeIfStatementBeginEnd() const
{
  // IF THEN ELSE END IF; always needs a BEGIN/END
  return true;
}

// End of the IF statement
CString 
SQLInfoFirebird::GetCodeEndIfStatement() const
{
  return "\n";
}

CString 
SQLInfoFirebird::GetAssignmentStatement(const CString& p_destiny,const CString& p_source) const
{
  return p_destiny + " = " + p_source + ";";
}


// SQL keyword to alter a column in a table
CString 
SQLInfoFirebird::GetCodeAlterColumn() const
{
  return "ALTER COLUMN ";
}

// Code to start a while loop
CString
SQLInfoFirebird::GetStartWhileLoop(CString p_condition) const
{
  return "WHILE " + p_condition + " LOOP\n";
}

// Code to end a WHILE loop
CString
SQLInfoFirebird::GetEndWhileLoop() const
{
  return "END LOOP;\n";
}

// The fact that a SELECT **must** be between parenthesis for an assignment
bool 
SQLInfoFirebird::GetAssignmentSelectParenthesis() const
{
  // waarde = (SELECT max kenmerk FROM tabel);
  return true;
}

// Gets the UPPER function
CString 
SQLInfoFirebird::GetUpperFunction(CString& p_expression) const
{
  return "UPPER(" + p_expression + ")";
}

// Gets the construction for 1 minute ago
CString 
SQLInfoFirebird::GetInterval1MinuteAgo() const
{
  return "ERROR";
}

// Gets the construction / select for generating a new serial identity
CString 
SQLInfoFirebird::GetSQLGenerateSerial(CString p_table) const
{
  return "SELECT " + p_table + "_seq.nextval FROM DUAL";
}

// Gets the construction / select for the resulting effective generated serial
CString 
SQLInfoFirebird::GetSQLEffectiveSerial(CString p_identity) const
{
  // Just return it, it's the correct value
  return p_identity;
}

// Gets the Not-NULL-Value statement of the database
CString 
SQLInfoFirebird::GetNVLStatement(CString& p_test,CString& p_isnull) const
{
  return "{fn IFNULL(" + p_test + "," + p_isnull + ")}";
}

// Gets the sub-transaction commands
CString 
SQLInfoFirebird::GetStartSubTransaction(CString p_savepointName) const
{
  return CString("SAVEPOINT ") + p_savepointName;
}

CString 
SQLInfoFirebird::GetCommitSubTransaction(CString p_savepointName) const
{
  return CString("COMMIT TRANSACTION ") + p_savepointName;
}

CString 
SQLInfoFirebird::GetRollbackSubTransaction(CString p_savepointName) const
{
  return CString("ROLLBACK TO ") + p_savepointName;
}

// SQL CATALOG QUERIES
// ====================================================================

// SQL to find out whether a stored procedure does exist in the database
CString 
SQLInfoFirebird::GetSQLStoredProcedureExists(CString& p_name) const
{
  CString nameUpper(p_name);
  nameUpper.MakeUpper();
  CString query = ( "SELECT COUNT(*) "
                    "  FROM RDB$PROCEDURES "
                    " WHERE RDB$PROCEDURE_NAME ='" + nameUpper + "';");
  return query;
}

// To select one (1) record in a query
CString 
SQLInfoFirebird::GetDualTableName() const
{
  return "rdb$database";
}

// FROM-Part for a query to select only 1 (one) record
CString 
SQLInfoFirebird::GetDualClause() const
{
  return " FROM rdb$database";
}

// CONSTRAINTS DEFERRABLE clause
CString
SQLInfoFirebird::GetConstraintDeferrable() const
{
  return "";
}

// Defer constraints until the next commit
CString 
SQLInfoFirebird::GetSQLDeferConstraints() const
{
  return "";
}

// Constraints back to immediate checking
CString 
SQLInfoFirebird::GetSQLConstraintsImmediate() const
{
  return "";
}

// Get the SQL to see if a table exists in the database
CString 
SQLInfoFirebird::GetSQLTableExists(CString /*p_schema*/,CString p_tablename) const
{
  CString upperName(p_tablename);
  upperName.MakeUpper();
  CString query = "SELECT COUNT(*)\n"
                  "  FROM rdb$relations\n"
                  " WHERE rdb$relation_name = '" + upperName + "'";
  return query;
}

// Get the SQL query to get the columns of a table from the catalog
CString 
SQLInfoFirebird::GetSQLGetColumns(CString& /*p_user*/,CString& p_tableName) const
{
  CString upperName(p_tableName);
  upperName.MakeUpper();

  CString table = "SELECT col.rdb$field_name\n"        // 0 -> Naam
                  "      ,col.rdb$field_position\n"    // 1 -> Positie
                  "      ,typ.rdb$field_type\n"        // 2 -> type
                  "      ,typ.rdb$field_length\n"      // 3 -> lengte
                  "      ,typ.rdb$null_flag\n"         // 4 -> nn 1 = not null / 0 is null
                  "      ,typ.rdb$field_precision\n"   // 5 -> precision
                  "      ,typ.rdb$field_scale\n"       // 6 -> scale (neg)
                  "  FROM rdb$relation_fields col\n"
                  "      ,rdb$fields typ\n"
                  " WHERE col.rdb$relation_name = '" + upperName + "'\n"
                  "   AND col.rdb$field_source  = typ.rdb$field_name\n"
                  " ORDER BY col.rdb$field_position";
  return table;
}

// Get the SQL query to get the CHECK and UNIQUE constraints for a table
CString 
SQLInfoFirebird::GetSQLGetConstraintsForTable(CString& p_tableName) const
{
  CString upperName = p_tableName;
  upperName.MakeUpper();
  CString contabel = "SELECT con.rdb$constraint_name\n"
                     "      ,con.rdb$relation_name\n"
                     "  FROM rdb$relation_constraints con\n"
                     " WHERE rdb$relation_name = '" + upperName + "'"
                     "   AND con.rdb$constraint_type IN ('CHECK','UNIQUE')";
  return contabel;
}

// Read all existing indices of a table
CString 
SQLInfoFirebird::GetSQLTableIndices(CString /*p_user*/,CString p_tableName) const
{
  p_tableName.MakeUpper();
  CString query = "SELECT idx.rdb$index_name\n"
                  "      ,col.rdb$field_name\n"
                  "      ,col.rdb$field_position\n"
                  "      ,idx.rdb$unique_flag\n" // 1 if unique, null if not
                  "      ,idx.rdb$index_type\n"  // 1 if descending, null,0 on ascending
                  "      ,idx.rdb$expression_source\n"
                  "  FROM rdb$indices idx\n"
                  "      ,rdb$index_segments col\n"
                  " WHERE idx.rdb$index_name    = col.rdb$index_name\n"
                  "   AND idx.rdb$relation_name = '" + p_tableName + "'\n"
                  "   AND idx.rdb$system_flag   = 0\n"
                  " ORDER BY 1,5";
  return query;
}

// Get SQL to create an index for a table
CString 
SQLInfoFirebird::GetSQLCreateIndex(CString p_user,CString p_tableName,DBIndex* p_index) const
{
  CString sql("CREATE ");
  if(p_index->m_unique)
  {
    sql += "UNIQUE ";
  }
  sql += (p_index->m_descending) ? "DESC" : "ASC";
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
    // Next column
    ++column;
    ++p_index;
  }
  sql += ")";

  return sql;
}

CString
SQLInfoFirebird::GetSQLCreateIndex(MStatisticsMap& p_indices) const
{
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
    if(index.m_columnName.Left(1) == "(")
    {
      query.TrimRight("(");
      query += " COMPUTED BY ";
      query += index.m_columnName;
      query.TrimRight(")");
    }
    else if(!index.m_filter.IsEmpty())
    {
      query += index.m_filter;
    }
    else
    {
      query += index.m_columnName;
    }
  }
  query += ")";
  return query;
}

// Get extra filter expression for an index column
CString
SQLInfoFirebird::GetIndexFilter(MetaStatistics& /*p_index*/) const
{
  return "";
}

// Get SQL to drop an index
CString 
SQLInfoFirebird::GetSQLDropIndex(CString p_user,CString p_indexName) const
{
  CString sql = "DROP INDEX " + p_indexName;
  return sql;
}

// Get the SQL Query to read the referential constraints for a table
CString 
SQLInfoFirebird::GetSQLTableReferences(CString /*p_schema*/
                                      ,CString p_tablename
                                      ,CString p_constraint /*=""*/
                                      ,int     /* p_maxColumns /*= SQLINFO_MAX_COLUMNS*/) const
{
  p_tablename.MakeUpper();
  p_constraint.MakeUpper();
  CString query = "SELECT con.rdb$constraint_name         AS foreign_key_constraint\n"
                  "      ,''                              AS schema_name\n"
                  "      ,con.rdb$relation_name           AS table_name\n"
                  "      ,seg.rdb$field_name              AS column_name\n"
                  "      ,idx.rdb$relation_name           AS primary_table_name\n"
                  "      ,psg.rdb$field_name              AS primary_key_column\n"
                  "      ,case con.rdb$deferrable         WHEN 'YES'  THEN 1 ELSE 0 END as deferrable\n"
                  "      ,case con.rdb$initially_deferred WHEN 'YES'  THEN 1 ELSE 0 END as initially_deferred\n"
                  "      ,1                               as enabled\n"
                  "      ,case ref.rdb$match_option       WHEN 'FULL' THEN 1 ELSE 0 END as match_option\n"
                  "      ,case ref.rdb$update_rule        WHEN 'RESTRICT'     THEN 1\n"
                  "                                       WHEN 'CASCADE'      THEN 0\n"
                  "                                       WHEN 'SET NULL'     THEN 2\n"
                  "                                       WHEN 'SET DEFAULT'  THEN 4\n"
                  "                                       WHEN 'NO ACTION'    THEN 3\n"
                  "                                       ELSE 0\n"
                  "                                       END as update_rule\n"
                  "      ,case ref.rdb$delete_Rule        WHEN 'RESTRICT'     THEN 1\n"
                  "                                       WHEN 'CASCADE'      THEN 0\n"
                  "                                       WHEN 'SET NULL'     THEN 2\n"
                  "                                       WHEN 'SET DEFAULT'  THEN 4\n"
                  "                                       WHEN 'NO ACTION'    THEN 3\n"
                  "                                       ELSE 0\n"
                  "                                       END as delete_rule\n"
                  "  FROM rdb$relation_constraints con\n"
                  "      ,rdb$ref_constraints ref\n"
                  "      ,rdb$indices         idx\n"
                  "      ,rdb$indices         cix\n"
                  "      ,rdb$index_segments  seg\n"
                  "      ,rdb$index_segments  psg\n"
                  " WHERE con.rdb$constraint_name = ref.rdb$constraint_name\n"
                  "   AND ref.rdb$const_name_uq   = idx.rdb$index_name\n"
                  "   AND con.rdb$index_name      = cix.rdb$index_name\n"
                  "   AND seg.rdb$index_name      = cix.rdb$index_name\n"
                  "   AND psg.rdb$index_name      = idx.rdb$index_name\n"
                  "   AND seg.rdb$field_position  = psg.rdb$field_position\n"
                  "   AND con.rdb$constraint_type = 'FOREIGN KEY'";
  if(!p_tablename.IsEmpty())
  {
    query += "\n   AND con.rdb$relation_name   = '" + p_tablename + "'";
  }
  if(!p_constraint.IsEmpty())
  {
    query += "\n   AND con.rdb$constraint_name = '" + p_constraint + "'";
  }
  return query;
}

// Get the SQL to determine the sequence state in the database
CString 
SQLInfoFirebird::GetSQLSequence(CString p_schema,CString p_tablename,CString p_postfix /*= "_seq"*/) const
{
  CString sequence = p_tablename + p_postfix;
  sequence.MakeUpper();

  CString sql = "SELECT rdb$generator_name as sequence_name\n" 
                "      ,rdb$initial_value as current_value\n"
                "      ,decode(rdb$generator_increment,1,1,0) as is_correct\n"
                "  FROM rdb$generators\n"
                " WHERE rdb$system_flag    = 0\n"
                "   AND rdb$generator_name = '" + sequence + "'";
  return sql;
}

// Create a sequence in the database
CString 
SQLInfoFirebird::GetSQLCreateSequence(CString /*p_schema*/,CString p_tablename,CString p_postfix /*= "_seq"*/,int p_startpos /*=1*/) const
{
  CString sequence = p_tablename + p_postfix;
  CString sql;
  sql.Format("CREATE SEQUENCE %s START WITH %d",sequence,p_startpos);
  return sql;
}

// Remove a sequence from the database
CString 
SQLInfoFirebird::GetSQLDropSequence(CString p_schema,CString p_tablename,CString p_postfix /*= "_seq"*/) const
{
  CString sequence = p_tablename + p_postfix;
  CString sql = "DROP SEQUENCE " + sequence;
  return sql;
}

// Grant rights on the sequence
CString
SQLInfoFirebird::GetSQLSequenceRights(CString /*p_schema*/,CString p_tableName,CString p_postfix /*="_seq"*/) const
{
  CString sql;
  CString sequence = p_tableName + p_postfix;
  sql.Format("GRANT USAGE ON %s TO %s",sequence,GetGrantedUsers());
  return sql;
}

// Query to remove a stored procedure
void
SQLInfoFirebird::DoRemoveProcedure(CString& p_procedureName) const
{
  SQLQuery query(m_database);
  query.TryDoSQLStatement("DROP PROCEDURE " + p_procedureName);
}

CString
SQLInfoFirebird::GetSQLSessionAndTerminal() const
{
  CString query = "SELECT mon.mon$attachment_id\n"
                  "      ,mon.mon$timestamp\n"
                  "      ,mon.mon$remote_address\n"
                  "      ,mon.mon$remote_host\n"
                  "      ,mon.mon$remote_process\n"
                  "      ,mon.mon$remote_pid\n"
                  "      ,mon.mon$remote_os_user\n"
                  "  FROM mon$attachments mon\n"
                  " WHERE mon$attachment_id = current_connection";
  return query;
}

// Get SQL to check if session number exists
CString 
SQLInfoFirebird::GetSQLSessionExists(CString p_sessionID) const
{
  CString query;
  query.Format("SELECT COUNT(*)\n"
               "  FROM mon$attachments\n"
               " WHERE mon$system_flag = 0\n"
               "   AND mon$attachment_id = %d",p_sessionID);
  return query;
}

// Get a query for an unique session id
CString
SQLInfoFirebird::GetSQLUniqueSessionId(const CString& /*p_databaseName*/,const CString& /*p_sessionTable*/) const
{
  CString query = "SELECT distinct(mon$attachment_id)\n"
                  "  FROM mon$attachments\n"
                  " WHERE mon$system_flag = 0";
  return query;
}

// Get query to find the extra logged in sessions
CString
SQLInfoFirebird::GetSQLSearchSession(const CString& /*p_databaseName*/,const CString& /*p_sessionTable*/) const
{
  // No way in Firebird to find that
  return "";
}

// See if a column exists in a table
bool   
SQLInfoFirebird::DoesColumnExistsInTable(CString& /*p_owner*/,CString& p_tableName,CString& p_column) const
{
  CString table(p_tableName);
  CString column(p_column);
  table.MakeUpper();
  column.MakeUpper();
  
  CString query = "SELECT COUNT(*)\n"
                  "  FROM rdb$relation_fields\n"
                  " WHERE rdb$relation_name = '" + p_tableName + "'\n"
                  "   AND rdb$field_name    = '" + p_column + "'";
  SQLQuery qry(m_database);
  return qry.DoSQLStatementScalar(query)->GetAsBoolean();
}

// See if a table already has a primary key
CString
SQLInfoFirebird::GetSQLPrimaryKeyConstraintInformation(CString /*p_schema*/,CString p_tableName) const
{
  CString table(p_tableName);
  table.MakeUpper();
  CString query = "select count(*)\n"
                  "  from rdb$indices\n"
                  " where rdb$relation_name = '" + table + "'\n"
                  "   and rdb$index_name = 'PK_" + table + "'";
  return query;
}

// Does the named constraint exist in the database
bool    
SQLInfoFirebird::DoesConstraintExist(CString p_constraintName) const
{
  return false;
}

// returns a lock table statement
CString 
SQLInfoFirebird::GetSQLLockTable(CString& /*p_tableName*/,bool /*p_exclusive*/) const
{
  // Firebird does NOT have a LOCK-TABLE statement
  return "";
}

// Query to optimize a table
CString 
SQLInfoFirebird::GetSQLOptimizeTable(CString& /*p_owner*/,CString& /*p_tableName*/,int& /*p_number*/)
{
  // Firebird has no SQL for this, it uses the "gfix -sweep <database>" tool
  return "";
}

// Getting the fact that there is only **one** (1) user session in the database
bool
SQLInfoFirebird::GetOnlyOneUserSession()
{
  CString sql("SELECT COUNT(*)\n"
              "  FROM MON$ATTACHMENTS\n"
              " WHERE MON$SYSTEM_FLAG = 0");

  SQLQuery query(m_database);
  SQLVariant* sessions = query.DoSQLStatementScalar(sql);
  return sessions->GetAsSLong() <= 1;
}

// Gets the triggers for a table
CString
SQLInfoFirebird::GetSQLTriggers(CString p_schema,CString p_table) const
{
  CString sql;
  sql.Format("SELECT '' AS catalog_name\n"
             "      ,'%s' AS schema_name\n"
             "      ,rdb$relation_name\n"
             "      ,rdb$trigger_name\n"
             "      ,rdb$description\n"
             "      ,rdb$trigger_sequence\n"
             "      ,CASE rdb$trigger_type\n"
             "            WHEN    1 THEN true\n"
             "            WHEN    3 THEN true\n"
             "            WHEN    5 THEN true\n"
             "            WHEN   17 THEN true\n"
             "            WHEN   25 THEN true\n"
             "            WHEN   27 THEN true\n"
             "            WHEN  113 THEN true\n"
             "            WHEN 8192 THEN true\n"
             "            WHEN 8194 THEN true\n"
             "                      ELSE false\n"
             "       END AS trigger_before\n"
             "      ,CASE rdb$trigger_type\n"
             "            WHEN    1 THEN true\n"
             "            WHEN    2 THEN true\n"
             "            WHEN   17 THEN true\n"
             "            WHEN   18 THEN true\n"
             "            WHEN   25 THEN true\n"
             "            WHEN   26 THEN true\n"
             "            WHEN  113 THEN true\n"
             "            WHEN  114 THEN true\n"
             "                      ELSE false\n"
             "       END AS trigger_insert\n"
             "      ,CASE rdb$trigger_type\n"
             "            WHEN    3 THEN true\n"
             "            WHEN    4 THEN true\n"
             "            WHEN   17 THEN true\n"
             "            WHEN   18 THEN true\n"
             "            WHEN  113 THEN true\n"
             "            WHEN  114 THEN true\n"
             "                      ELSE false\n"
             "       END AS trigger_update\n"
             "      ,CASE rdb$trigger_type\n"
             "            WHEN    5 THEN true\n"
             "            WHEN    6 THEN true\n"
             "            WHEN   25 THEN true\n"
             "            WHEN   26 THEN true\n"
             "            WHEN  113 THEN true\n"
             "            WHEN  114 THEN true\n"
             "                      ELSE false\n"
             "       END AS trigger_delete\n"
             "      ,false as trigger_select\n"
             "      ,CASE rdb$trigger_type\n"
             "            WHEN 8192 THEN true\n"
             "            WHEN 8993 THEN true\n"
             "                      ELSE false\n"
             "       END AS trigger_session\n"
             "      ,CASE rdb$trigger_type\n"
             "            WHEN 8194 THEN true\n"
             "            WHEN 8995 THEN true\n"
             "                      ELSE false\n"
             "       END AS trigger_transaction\n"
             "      ,CASE rdb$trigger_type\n"
             "            WHEN 8196 THEN true\n"
             "                      ELSE false\n"
             "       END AS trigger_rollback\n"
             "      ,'' AS trigger_referencing\n"
             "      ,CASE rdb$trigger_inactive\n"
             "            WHEN 0 THEN true\n"
             "                   ELSE false\n"
             "       END AS trigger_enabled\n"
             "      ,rdb$trigger_source\n"
             "  FROM rdb$triggers\n"
             " WHERE rdb$relation_name = '%s'\n"
             "   AND rdb$system_flag   = 0\n"
             " ORDER BY rdb$trigger_sequence"
            ,p_schema
            ,p_table);
  return sql;
}

// SQL DDL STATEMENTS
// ==================

CString
SQLInfoFirebird::GetCreateColumn(CString /*p_schema*/,CString p_tablename,CString p_columnName,CString p_typeDefinition,bool p_notNull)
{
  CString sql  = "ALTER TABLE "  + p_tablename  + "\n";
                 "  ADD COLUMN " + p_columnName + " " + p_typeDefinition;
  if(p_notNull)
  {
    sql += " NOT NULL";
  }
  return sql;
}

// Drop a column from a table
CString 
SQLInfoFirebird::GetSQLDropColumn(CString p_schema,CString p_tablename,CString p_columnName) const
{
  return "ALTER TABLE " + p_tablename + "\n"
         " DROP COLUMN " + p_columnName;
}

CString
SQLInfoFirebird::GetCreateForeignKey(CString p_tablename,CString p_constraintname,CString p_column,CString p_refTable,CString p_primary)
{
  CString sql = "ALTER TABLE " + p_tablename + "\n"
                "  ADD CONSTRAINT " + p_constraintname + "\n"
                "      FOREIGN KEY (" + p_column + ")\n"
                "      REFERENCES " + p_refTable + "(" + p_primary + ")";
  return sql;
}

CString 
SQLInfoFirebird::GetModifyColumnType(CString /*p_schema*/,CString p_tablename,CString p_columnName,CString p_typeDefinition)
{
  CString sql = "ALTER TABLE "    + p_tablename + "\n"
                " MODIFY COLUMN " + p_columnName + "\n"
                " TYPE   " + p_typeDefinition;
  return sql;
}

CString 
SQLInfoFirebird::GetModifyColumnNull(CString /*p_schema*/,CString p_tablename,CString p_columnName,bool p_notNull)
{
  CString sql = "ALTER TABLE "    + p_tablename + "\n"
                " MODIFY COLUMN " + p_columnName + (p_notNull ? " SET " : " DROP ") + "NOT NULL";
  return sql;
}

// Get the SQL to drop a view. If precursor is filled: run that SQL first!
CString 
SQLInfoFirebird::GetSQLDropView(CString /*p_schema*/,CString p_view,CString& p_precursor)
{
  p_view.MakeUpper();

  // Firebird cannot drop a view if dependencies from stored procedures or functions
  // still exist in the dependencies table. After Firebird 3.0 we need modification
  // rights on this system catalog table!!
  // Changes being that we re-create the view right away after the drop.
  p_precursor = "DELETE FROM rdb$dependencies\n"
                " WHERE rdb$depended_on_name = '" + p_view + "'\n"
                "   AND rdb$depended_on_type = 0";
  return "DROP VIEW " + p_view;
}

// Create or replace a database view
CString 
SQLInfoFirebird::GetSQLCreateOrReplaceView(CString /*p_schema*/,CString p_view,CString p_asSelect) const
{
  return "RECREATE VIEW " + p_view + "\n" + p_asSelect;
}

// Create or replace a trigger
CString
SQLInfoFirebird::CreateOrReplaceTrigger(MetaTrigger& p_trigger) const
{
  CString sql;
  sql.Format("CREATE OR ALTER TRIGGER %s FOR %s\n"
            ,p_trigger.m_triggerName
            ,p_trigger.m_tableName);

  // Do the table level trigger
  if(p_trigger.m_insert || p_trigger.m_update || p_trigger.m_delete)
  {
    sql += p_trigger.m_before ? "BEFORE " : "AFTER ";
    if(p_trigger.m_insert)
    {
      sql += "INSERT ";
    }
    if(p_trigger.m_update)
    {
      if(p_trigger.m_insert)
      {
        sql += "OR ";
      }
      sql += "UPDATE ";
    }
    if(p_trigger.m_delete)
    {
      if(p_trigger.m_insert || p_trigger.m_update)
      {
        sql += "OR ";
      }
      sql += "DELETE ";
    }
  }
  else if(p_trigger.m_session)
  {
    sql += p_trigger.m_before ? "ON CONNECT" : "ON DISCONNECT";
  }
  else // transaction
  {
    sql += "ON TRANSACTION ";
    if(p_trigger.m_before)
    {
      sql += "START";
    }
    else
    {
      sql += p_trigger.m_rollback ? "ROLLBACK" : "COMMIT";
    }
  }

  // Now add the trigger PSM source
  sql += "\n";
  sql += p_trigger.m_source;

  return sql;
}

// SQL DDL ACTIONS
// ====================================================================

// Process DDL commandos in the catalog
void 
SQLInfoFirebird::DoCommitDDLcommands() const
{
  SQLQuery sql(m_database);
  try
  {
    sql.DoSQLStatement("COMMIT");
  }
  catch(...)
  {
    sql.TryDoSQLStatement("ROLLBACK");
    throw CString("Rollback transaction");
  }
}

// Explicit DML commando committing
void
SQLInfoFirebird::DoCommitDMLcommands() const
{
  SQLQuery sql(m_database);
  sql.DoSQLStatement("COMMIT");
}

// Remove a column from a table
void
SQLInfoFirebird::DoDropColumn(CString p_tableName,CString p_columName)
{
  CString sql = "ALTER TABLE  " + p_tableName + "\n"
                " DROP COLUMN " + p_columName;
  SQLQuery query(m_database);
  query.TryDoSQLStatement(sql);
  DoCommitDDLcommands();
}

// Does the named view exists in the database
bool
SQLInfoFirebird::DoesViewExists(CString& p_viewName)
{
  CString view(p_viewName);
  view.MakeUpper();

  CString sql = "SELECT count(*)\n"
                "  FROM rdb$relations\n"
                " WHERE rdb$relation_name = '" + view + "'\n"
                "   AND rdb$relation_type = 1";
  SQLQuery query(m_database);
  return query.DoSQLStatementScalar(sql)->GetAsBoolean();
}

// Can create temporary tables at runtime
bool 
SQLInfoFirebird::GetMustMakeTemptablesAtRuntime() const
{
  return true;
}

// Creates a temporary table 
void
SQLInfoFirebird::DoMakeTemporaryTable(CString& p_tableName,CString& p_content,CString& /*p_indexColumn*/) const
{
  SQLQuery sql(m_database);
  CString create = "CREATE GLOBAL TEMPORARY TABLE " + p_tableName + p_content;
  try
  {
    sql.DoSQLStatement(create);
  }
  catch(CString& error)
  {
    // throw error if not succeeded
    throw CString("Cannot create temporary table: " + p_tableName + " : " + error);
  }
}

// Removal of the temporary table
void
SQLInfoFirebird::DoRemoveTemporaryTable(CString& p_tableName) const
{
  SQLQuery query(m_database);
  // If table is not there, no error...
  query.TryDoSQLStatement("DROP TABLE " + p_tableName);
}

// Making a stored procedure in the database
void 
SQLInfoFirebird::DoMakeProcedure(CString& p_procName,CString p_table,bool /*p_noParameters*/,CString& p_codeBlock)
{
  SQLQuery sql(m_database);
  sql.DoSQLStatement(p_codeBlock);
  sql.DoSQLStatement("GRANT EXECUTE ON PROCEDURE " + p_procName + " TO " + GetGrantedUsers());
  DoCommitDDLcommands();
}

// Not supported by Firebird
void
SQLInfoFirebird::DoRenameTable(CString& /*p_oldName*/,CString& /*p_newName*/) const
{
  throw CString("Changing database tablenames is not supported by this database type.");
}

// PERSISTENT-STORED MODULES (SPL / PL/SQL)
// ===================================================================

// Getting the user errors from the database
CString 
SQLInfoFirebird::GetUserErrorText(CString& p_procName) const
{
  (void)p_procName;   // Not supported in Firebird.
  return "";
}

// Getting assignment to a variable in SPL
CString 
SQLInfoFirebird::GetSPLAssignment(CString p_variable) const
{
  return p_variable + "=";
}

// Getting the start of a SPL while loop
CString 
SQLInfoFirebird::GetSPLStartWhileLoop(CString p_condition) const
{
  CString sql = "WHILE " + p_condition + " DO ";
  return sql;
}

// The end of a while loop
CString 
SQLInfoFirebird::GetSPLEndWhileLoop() const
{
  // uses the BEGIN END after the DO
  return "";
}

// Getting a stored procedure call from within SPL
CString 
SQLInfoFirebird::GetSQLSPLCall(CString p_procName) const
{
  return "SELECT vv_result,vv_sqlerror,vv_isamerror,vv_errordata FROM " + p_procName;
}

// Building a parameter list for calling a stored procedure
CString 
SQLInfoFirebird::GetBuildedParameterList(size_t p_numOfParameters) const
{
  // Always doing an ellipsis, even if no parameters are used
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

CString
SQLInfoFirebird::GetParameterType(CString& p_type) const
{
  return p_type;
}

// GENERAL SQL ACTIONS
// =================================================================

CString 
SQLInfoFirebird::GetSQLString(const CString& p_string) const
{
  CString s = p_string;
  s.Replace("'","''");
  CString kwoot = GetQuoteCharacter();
  return  kwoot + s + kwoot;
}

CString 
SQLInfoFirebird::GetSQLDateString(int p_year,int p_month,int p_day) const
{
  CString retval;
  retval.Format("CAST '%02d/%02d/04d' AS DATE)",p_day,p_month,p_year); // American order!!
  return retval;
}

CString 
SQLInfoFirebird::GetSQLTimeString(int p_hour,int p_minute,int p_second) const
{
  CString time;
  time.Format("%2.2d:%2.2d:%2.2d",p_hour,p_minute,p_second);
  return time;
}

CString 
SQLInfoFirebird::GetSQLDateTimeString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  CString retval;
  retval.Format("CAST('%02d/%02d/%04d %02d:%02d:%02d' AS TIMESTAMP)"
               ,p_day,p_month,p_year       // American order !!
               ,p_hour,p_minute,p_second); // 24 hour clock
  return retval;
}

// Get date-time bound parameter string in database format
CString 
SQLInfoFirebird::GetSQLDateTimeBoundString() const
{
  return "CAST(? AS TIMESTAMP)";
}

// Stripped data for the parameter binding
CString
SQLInfoFirebird::GetSQLDateTimeStrippedString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  CString retval;
  retval.Format("%02d/%02d/%04d %02d:%02d:%02d"
                ,p_day,p_month,p_year       // American order !!
                ,p_hour,p_minute,p_second); // 24 hour clock
  return retval;
}

CString 
SQLInfoFirebird::GetSPLSourcecodeFromDatabase(const CString& /*p_owner*/,const CString& /*p_procName*/) const
{
  // @EH To be implemented
  return "";
}

// Get the SPL datatype for integer
CString 
SQLInfoFirebird::GetSPLIntegerType() const
{
  return "integer";
}

// Get the SPL datatype for a decimal
CString 
SQLInfoFirebird::GetSPLDecimalType() const
{
  return "decimal";
}

// Get the SPL declaration for a cursor
CString 
SQLInfoFirebird::GetSPLCursorDeclaratie(CString& p_variableName,CString& p_query) const
{
  return "CURSOR " + p_variableName + " IS " + p_query + ";";
}

// Get the SPL cursor found row parameter
CString 
SQLInfoFirebird::GetSPLCursorFound(CString& p_cursorName) const
{
  return p_cursorName + "_found";
}

// Get the SPL cursor row-count variable
CString 
SQLInfoFirebird::GetSPLCursorRowCount(CString& /*p_variable*/) const
{
  // Not supported
  return "";
}

// Get the SPL datatype for a declaration of a row-variable
CString 
SQLInfoFirebird::GetSPLCursorRowDeclaration(CString& /*p_cursorName*/,CString& /*p_variableName*/) const
{
  // TODO: Check
  return "";
}

CString 
SQLInfoFirebird::GetSPLFetchCursorIntoVariables(CString               p_cursorName
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
SQLInfoFirebird::GetSPLFetchCursorIntoRowVariable(CString& p_cursorName,CString p_variableName) const
{ 
  // TODO: CHeck
  return "FETCH " + p_cursorName + " INTO " + p_variableName+ ";";
}

// Get the SPL no-data exception clause
CString 
SQLInfoFirebird::GetSPLNoDataFoundExceptionClause() const
{
  // TODO: Check
  return "WHEN NO_DATA THEN";
}

// Get the SPL form of raising an exception
CString 
SQLInfoFirebird::GetSPLRaiseException(CString p_exceptionName) const
{
  // TODO Check
  return "RAISE " + p_exceptionName + ";";
}

// Get the fact that the SPL has server functions that return more than 1 value
bool    
SQLInfoFirebird::GetSPLServerFunctionsWithReturnValues() const
{
  return true;
}

// Calling a stored function or procedure if the RDBMS does not support ODBC call escapes
SQLVariant* 
SQLInfoFirebird::DoSQLCall(SQLQuery* p_query,CString& /*p_schema*/,CString& p_procedure)
{
  bool result = false;
  int returns = GetCountReturnParameters(p_query);

  // See if we have a single return parameter in function style
  if(p_query->GetParameter(0) && returns == 0)
  {
    result = DoSQLCallFunction(p_query,p_procedure);
  }
  else
  {
    result = DoSQLCallProcedure(p_query,p_procedure);
  }
  // If result, return 0th parameter as the result
  return result ? p_query->GetParameter(0) : nullptr;
}


// SPECIALS
// ==========================================================================

// Translate database-errors to a human readable form
CString 
SQLInfoFirebird::TranslateErrortext(int p_error,CString p_errorText) const
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
// PRIVATE PART
//
//////////////////////////////////////////////////////////////////////////


// Firebird does not support the "{[?=]CALL function(?,?)}" sequence
// instead you have to do a "SELECT function(?,?) FROM rdb$database" 
// and use the result of the select as the return parameter
bool
SQLInfoFirebird::DoSQLCallFunction(SQLQuery* p_query,CString& p_function)
{
  SQLQuery query(m_database);
  CString sql = ConstructSQLForFunctionCall(p_query,&query,p_function);

  var* retval = query.DoSQLStatementScalar(sql);
  if(retval)
  {
    p_query->SetParameter(0,retval);
  }
  return retval != nullptr;
}

// Firebird does not support the "{[?=]CALL procedure(?,?)}" sequence
// instead you have to do a "SELECT * FROM procedure(?,?)" 
// The result set is the set of output parameters
// DOES ONLY SUPPORT A SINGLE ROW RESULT SET!!
bool
SQLInfoFirebird::DoSQLCallProcedure(SQLQuery* p_query,CString& p_procedure)
{
  SQLQuery query(m_database);
  CString sql = ConstructSQLForProcedureCall(p_query,&query,p_procedure);

  query.DoSQLStatement(sql);
  if(query.GetRecord())
  {
    // Processing the result
    int type     = 0;
    int setIndex = -1;
    for(int resIndex = 1;resIndex <= query.GetNumberOfColumns();++resIndex)
    {
      // Getting the next result from the result set
      SQLVariant* result = query.GetColumn(resIndex);

      // Finding the next OUTPUT parameter in the original query call
      do
      {
        SQLVariant* target = p_query->GetParameter(++setIndex);
        if(target == nullptr)
        {
          throw CString("Wrong number of output parameters for procedure call");
        }
        type = target->GetParameterType();
      }
      while(type != SQL_PARAM_OUTPUT && type != SQL_PARAM_INPUT_OUTPUT);

      // Storing the result;
      p_query->SetParameter(setIndex,result);
    }
    // Returning the first return column as the result of the procedure
    return true;
  }
  return false;
}

// Get the number of OUTPUT or INPUT_OUTPUT parameters
// In the parameter list (disregarding the 0th parameter)
int
SQLInfoFirebird::GetCountReturnParameters(SQLQuery* p_query)
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

// Construct the "SELECT function(?,?) FROM rdb$database"
CString
SQLInfoFirebird::ConstructSQLForFunctionCall(SQLQuery* p_query
                                            ,SQLQuery* p_thecall
                                            ,CString&  p_function)
{
  // Start with select from function
  CString sql = "SELECT ";
  sql += p_function;

  // Opening parenthesis
  sql += "(";

  // Build list of markers
  int ind = 1;
  while(true)
  {
    // Try get the next parameter
    var* parameter = p_query->GetParameter(ind);
    if(parameter == nullptr) break;

    // Add marker
    if(ind > 1) sql += ",";
    sql += "?";

    // Add the parameter
    p_thecall->SetParameter(ind,parameter);

    // Next parameter
    ++ind;
  }

  // Closing parenthesis
  sql += ")";

  // From singular object
  sql += " FROM rdb$database";
  return sql;
}

// Construct the "SELECT * FROM procedure(?,?)" (input parameters ONLY!)
CString
SQLInfoFirebird::ConstructSQLForProcedureCall(SQLQuery* p_query
                                             ,SQLQuery* p_thecall
                                             ,CString&  p_procedure)
{
  // Start with select form
  CString sql = "SELECT * FROM ";
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

// End of namespace
}
