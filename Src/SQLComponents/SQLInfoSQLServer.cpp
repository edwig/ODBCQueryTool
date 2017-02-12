////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoSQLServer.cpp
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
#include "SQLInfoSQLServer.h"
#include "SQLQuery.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{

// Constructor.
SQLInfoSQLServer::SQLInfoSQLServer(SQLDatabase* p_database)
                 :SQLInfoDB(p_database)
{
}

// Destructor. does nothing
SQLInfoSQLServer::~SQLInfoSQLServer()
{
}

// Get the database type
DatabaseType 
SQLInfoSQLServer::GetDatabaseType() const
{
  return RDBMS_SQLSERVER;
}

// BOOLEANS EN STRINGS
// ====================================================================

CString
SQLInfoSQLServer::GetDatabaseVendorName() const
{
  // The name of the database vendor
  return "Microsoft";
}

// Get the physical database name
CString 
SQLInfoSQLServer::GetPhysicalDatabaseName() const
{
  CString query = "SELECT name\n"
                  "  FROM master.dbo.sysdatabases\n"
                  " WHERE dbid = db_id()";
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
SQLInfoSQLServer::IsCatalogUpper() const
{
  return false;
}

// System catalog supports full ISO schemas (same tables per schema)
bool
SQLInfoSQLServer::GetUnderstandsSchemas() const
{
  return true;
}

// Supports database/ODBCdriver comments in sql
bool 
SQLInfoSQLServer::SupportsDatabaseComments() const
{
  return false;
}

// Database can defer constraints until the end of a transaction
bool 
SQLInfoSQLServer::SupportsDeferredConstraints() const
{
  // SET CONSTRAINTS DEFERRED is supported
  return true;
}

// Database has ORDER BY with an expression, e.g. ORDER BY UPPER(column-name)
// Work-around is "SELECT UPPER(column-name) AS something.....ORDER BY something
bool
SQLInfoSQLServer::SupportsOrderByExpression() const
{
  return true;
}

// Supports the ODBC escape sequence {[?=] CALL procedure (?,?,?)}
bool
SQLInfoSQLServer::SupportsODBCCallEscapes() const
{
  return true;
}

// Catalogs query for the default value of a table's column
CString 
SQLInfoSQLServer::GetSQLStringDefaultValue(CString p_tableName,CString p_columnName) const
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
SQLInfoSQLServer::GetSystemDateTimeKeyword () const
{
  return "GETDATE()"; // current_timestamp also reported
} 

// String for the current date
CString
SQLInfoSQLServer::GetSystemDateString() const
{
  return "GETDATE()";
}

// If the database does not support the datatype TIME, it can be implemented as a DECIMAL
bool 
SQLInfoSQLServer::GetTimeIsDecimal() const
{
  // Time can be implemented as DECIMAL(17,16)
  return false;
}

// If the database does not support the datatype INTERVAL, it can be implemented as a DECIMAL
bool 
SQLInfoSQLServer::GetIntervalIsDecimal() const
{
  // INTERVAL can be implemented as DECIMAL
  return true;
}

// Get the concatenation operator
CString 
SQLInfoSQLServer::GetConcatanationOperator() const
{
  return "+";    
}

// Get quote character for strings
CString 
SQLInfoSQLServer::GetQuoteCharacter() const
{
  return "'";    
}

// Get default NULL for parameter list input
CString 
SQLInfoSQLServer::GetDefaultNULL() const
{
  return " = NULL ";    
}

// Parameter is for INPUT and OUTPUT in parameter list
CString 
SQLInfoSQLServer::GetParameterINOUT() const
{
  return "OUTPUT ";    
}
  
// Parameter is for OUTPUT only in parameter list
CString 
SQLInfoSQLServer::GetParameterOUT() const
{
  return "OUTPUT ";    
}

// Get the datatype of the audited user (h_user) in a stored procedure
CString 
SQLInfoSQLServer::GetAuditUserDatatype() const
{
  return "VARCHAR(50)";
} 

// Get the datatype of the audited user (h_user) as variable in a stored-procedure
CString 
SQLInfoSQLServer::GetAuditUserDatatypeAsVariable() const
{
  return "VARCHAR(50)";
} 

// Get datatype of the IDENTITY primary key
CString 
SQLInfoSQLServer::GetPrimaryKeyType() const
{
  return "INTEGER IDENTITY(1, 1)";
}

// Get datatype for a timestamp
CString 
SQLInfoSQLServer::GetDatetimeYearToSecondType() const
{
  return "DATETIME";
}

// Separator between two alter-constraints in an alter-table statement
CString
SQLInfoSQLServer::GetAlterConstraintSeparator() const
{
  return ",";
}

// Inner Join Keyword
CString 
SQLInfoSQLServer::GetInnerJoinKeyword() const
{
  return "INNER JOIN ";
}

// Outer join keyword
CString  
SQLInfoSQLServer::GetOuterJoinKeyword() const
{
  return "LEFT OUTER JOIN ";
}

// Inner Join Keyword for use in views.
CString 
SQLInfoSQLServer::GetViewInnerJoinKeyword() const
{
  return "INNER JOIN";
}


// Outer join keyword for use in views
CString 
SQLInfoSQLServer::GetViewOuterJoinKeyword() const
{
  return "LEFT OUTER JOIN";
}

// Get the closure for an outer-join
CString 
SQLInfoSQLServer::GetOuterJoinClosure() const
{
  return "";
}

// Outer join sign
CString  
SQLInfoSQLServer::GetOuterJoinSign() const
{
  return "";
}

// Prefix for a parameter in a stored procedure
CString
SQLInfoSQLServer::GetSPParamPrefix() const
{
  return "@";
}

// Get select part to add new record identity to a table
// Can be special column like 'OID'
CString 
SQLInfoSQLServer::GetIdentityString(CString& p_tablename,CString /*p_postfix = "_seq"*/) const
{
  return "IDENT_CURRENT('" + p_tablename + "') + " + "IDENT_INCR('" + p_tablename + "')";
}

// Get a query to create a temporary table from a select statement
CString 
SQLInfoSQLServer::GetSQLCreateTemporaryTable(CString& p_tablename,CString p_select) const
{
  return "CREATE TABLE #" + p_tablename + "\nAS " + p_select;
}

// Get the query to remove a temporary table indefinetly
// BEWARE: Must be executed with a multi-statement stack!
CString
SQLInfoSQLServer::GetSQLRemoveTemporaryTable(CString& p_tablename,int& p_number) const
{
  // Needed on MS_SQLServer because otherwise we get an error message
  // that the temporary table is still in use, and so will leave a temporary
  // table in the catalog, conflicting with a new temp table
  p_number += 3;
  return "DELETE FROM #"    + p_tablename + ";\n"
         "TRUNCATE TABLE #" + p_tablename + ";\n"
         "DROP TABLE #"     + p_tablename + ";\n";
}

// Get a query to select into a temp table
CString 
SQLInfoSQLServer::GetSQLSelectIntoTemp(CString& p_tablename,CString& p_select) const
{
  return "INSERT INTO #" + p_tablename + "\n" + p_select + ";\n";
}

// Replace the Identity column OID by a sequence.nextval
CString 
SQLInfoSQLServer::GetReplaceColumnOIDbySequence(CString p_columns,CString p_tablename,CString p_postfix /*="_seq"*/) const
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
SQLInfoSQLServer::GetSQLRemoveProcedureDependencies(CString p_procname) const
{
  return "";
}

// Remove field dependencies for calculated fields (bug in Firebird)
// To be run after a 'DROP TABLE' or an 'ALTER TABLE DROP COLUMN' or 'ALTER TABLE MODIFY COLUMN'
CString 
SQLInfoSQLServer::GetSQLRemoveFieldDependencies(CString p_tablename) const
{
  // Not needed in SQL Server
  return "";
}

// Gets the table definition-form of a primary key
CString 
SQLInfoSQLServer::GetPrimaryKeyDefinition(CString p_schema,CString p_tableName,bool /*p_temporary*/) const
{
  // The primary key constraint is not directly generated after the column
  // to ensure it will use the named index in the correct tablespace
  // Otherwise the index name and tablespace cannot be defined and will be auto-generated
  return GetPrimaryKeyType() + " NOT NULL CONSTRAINT pk_" + p_tableName + " PRIMARY KEY\n";
}

// Get the constraint form of a primary key to be added to a table after creation of that table
CString
SQLInfoSQLServer::GetPrimaryKeyConstraint(CString p_schema,CString p_tablename,CString p_primary) const
{
  return "ALTER TABLE " + p_schema + "." + p_tablename + "\n"
         "  ADD CONSTRAINT pk_" + p_tablename + "\n"
         "      PRIMARY KEY (" + p_primary + ")";
}

// Get the sql to add a foreign key to a table
CString 
SQLInfoSQLServer::GetSQLForeignKeyConstraint(DBForeign& p_foreign) const
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
  switch(p_foreign.m_updateRule)
  {
    case 1: query += "\n      ON UPDATE CASCADE";     break;
    case 2: query += "\n      ON UPDATE SET NULL";    break;
    case 3: query += "\n      ON UPDATE SET DEFAULT"; break;
    case 4: query += "\n      ON UPDATE NO ACTION";   break;
    default:// In essence: ON UPDATE RESTRICT, but that's already the default
    case 0: break;
  }
  switch(p_foreign.m_deleteRule)
  {
    case 1: query += "\n      ON DELETE CASCADE";     break;
    case 2: query += "\n      ON DELETE SET NULL";    break;
    case 3: query += "\n      ON DELETE SET DEFAULT"; break;
    case 4: query += "\n      ON DELETE NO ACTION";   break;
    default:// In essence: ON DELETE RESTRICT, but that's already the default
    case 0: break;
  }
  return query;
}

// Get the sql (if possible) to change the foreign key constraint
CString 
SQLInfoSQLServer::GetSQLAlterForeignKey(DBForeign& /*p_origin*/,DBForeign& /*p_requested*/) const
{
  // MS-SQL Server cannot alter a foreign-key constraint.
  // You must drop and then re-create your foreign key constraint
  // So return an empty string to signal this!
  return "";
}

// Performance parameters to be added to the database
CString
SQLInfoSQLServer::GetSQLPerformanceSettings() const
{
  return "";
}

// SQL To set the caching mode of SQL results
CString 
SQLInfoSQLServer::GetSQLCacheModeSetting(const CString& /*p_mode*/) const
{
  return "";
}

// Needed for storing numbers in stored procedures
CString
SQLInfoSQLServer::GetSQLNlsNumericCharacters() const
{
  // Not used in MS-SQLServer
  return "";
}

// Gives the statement to alter a table columns' name
CString
SQLInfoSQLServer::GetSQLModifyColumnName(CString p_tablename,CString p_oldName,CString p_newName,CString p_datatype)
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
SQLInfoSQLServer::GetMaxStatementLength() const
{
  return 0;		// No limit
}

// Gets the prefix needed for altering the datatype of a column in a MODIFY/ALTER
CString 
SQLInfoSQLServer::GetModifyDatatypePrefix() const
{
  // Just provide the new datatype without the TYPE word
  // SO: MODIFY <kolomnaam> <datatypenaam>
  return "";
}

// Code to create a temporary table (qualifier)
CString 
SQLInfoSQLServer::GetCodeTemporaryTable() const
{
  return "";
}

// Code to define a table in row-locking mode
CString 
SQLInfoSQLServer::GetCodeLockModeRow() const
{
  return "";
}

// Code to create a temporary table with no logging
CString 
SQLInfoSQLServer::GetCodeTempTableWithNoLog() const
{
  return "";
}

// Granting all rights on a table (In a NON-ANSI database)
CString 
SQLInfoSQLServer::GetSQLGrantAllOnTable(CString p_schema,CString p_tableName,bool p_grantOption /*= false*/)
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
SQLInfoSQLServer::GetSelectIntoTempClausePrefix(CString p_tableName) const
{
  return "";
}

// Code suffix for after a select-into-temp
CString
SQLInfoSQLServer::GetSelectIntoTempClauseSuffix(CString p_tableName) const
{
  return "";
}

// Gets the fact if an IF statement needs to be bordered with BEGIN/END
bool
SQLInfoSQLServer::GetCodeIfStatementBeginEnd() const
{
  // IF THEN ELSE END IF; does not need a BEGIN/END per se.
  return true;
}

// Gets the end of an IF statement
CString 
SQLInfoSQLServer::GetCodeEndIfStatement() const
{
  return "";
}

// Gets a complete assignment statement.
CString 
SQLInfoSQLServer::GetAssignmentStatement(const CString& p_destiny,const CString& p_source) const
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
SQLInfoSQLServer::GetCodeAlterColumn() const
{
  return "ALTER COLUMN ";
}

// Get the code to start a WHILE-loop
CString
SQLInfoSQLServer::GetStartWhileLoop(CString p_condition) const
{
  return "WHILE " + p_condition + " LOOP\n";
}

// Get the code to end a WHILE-loop
CString
SQLInfoSQLServer::GetEndWhileLoop() const
{
  return "END LOOP;\n";
}

// Gets the fact if a SELECT must be in between parenthesis for an assignment
bool 
SQLInfoSQLServer::GetAssignmentSelectParenthesis() const
{
  return false;
}

// Gets the UPPER function
CString 
SQLInfoSQLServer::GetUpperFunction(CString& p_expression) const
{
  return "UPPER(" + p_expression + ")";
}

// Gets the construction for 1 minute ago
CString 
SQLInfoSQLServer::GetInterval1MinuteAgo() const
{
  return "dateadd(minute,-1,current_timestamp)";
}

// Gets the construction / select for generating a new serial identity
CString 
SQLInfoSQLServer::GetSQLGenerateSerial(CString p_table) const
{
  // Is generated by the IDENTITY system
  return "";
}

// Gets the construction / select for the resulting effective generated serial
CString 
SQLInfoSQLServer::GetSQLEffectiveSerial(CString p_identity) const
{
  return "SELECT @@IDENTITY";
}

// Gets the Not-NULL-Value statement of the database
CString 
SQLInfoSQLServer::GetNVLStatement(CString& p_test,CString& p_isnull) const
{
  return CString("NVL(") + p_test + "," + p_isnull + ")";
}

// Gets the sub transaction commands
CString 
SQLInfoSQLServer::GetStartSubTransaction(CString p_savepointName) const
{
  return CString("BEGIN TRANSACTION ") + p_savepointName;
}

CString 
SQLInfoSQLServer::GetCommitSubTransaction(CString p_savepointName) const
{
  return CString("ROLLBACK TRANSACTION ") + p_savepointName;
}

CString 
SQLInfoSQLServer::GetRollbackSubTransaction(CString p_savepointName) const
{
  return CString("ROLLBACK TRANSACTION ") + p_savepointName;
}


// SQL CATALOG QUERIES
// ===================================================================

// Get SQL to check if a stored procedure already exists in the database
CString 
SQLInfoSQLServer::GetSQLStoredProcedureExists(CString& p_name) const
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
SQLInfoSQLServer::GetDualTableName() const
{
  return "";
}

// FROM-Part for a query to select only 1 (one) record
CString 
SQLInfoSQLServer::GetDualClause() const
{
  return "";
}

// Gets DEFERRABLE for a constraint (or nothing)
CString
SQLInfoSQLServer::GetConstraintDeferrable() const
{
  return "";
}

// Defer Constraints until the next COMMIT;
CString 
SQLInfoSQLServer::GetSQLDeferConstraints() const
{
  return "";
}

// Reset constraints back to immediate
CString 
SQLInfoSQLServer::GetSQLConstraintsImmediate() const
{
  return "";
}

// Get SQL to check if a table already exists in the database
CString 
SQLInfoSQLServer::GetSQLTableExists(CString p_schema,CString p_tablename) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  CString query = "SELECT count(*)\n"
                 "  FROM dbo.sysobjects\n"
                 " WHERE name = '" + p_tablename + "'";
  return query;
}

// Get SQL to select all columns of a table from the catalog
CString 
SQLInfoSQLServer::GetSQLGetColumns(CString& /*p_user*/,CString& p_tableName) const
{
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
SQLInfoSQLServer::GetSQLGetConstraintsForTable(CString& p_tableName) const
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

// Get SQL to read all indices for a table
CString 
SQLInfoSQLServer::GetSQLTableIndices(CString /*p_user*/,CString p_tableName) const
{
  p_tableName.MakeLower();
  CString query = "SELECT idx.name\n"
                  "      ,col.name\n"
                  "      ,ixk.keyno\n"
                  "      ,indexproperty(obj.Id, idx.name, 'IsUnique')\n"
                  "      ,indexkey_property(obj.Id, idx.indid, ixk.keyno, 'IsDescending')\n"
                  "      ,'' as index_source"
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

// Get SQL to create an index for a table
CString 
SQLInfoSQLServer::GetSQLCreateIndex(CString p_user,CString p_tableName,DBIndex* p_index) const
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
SQLInfoSQLServer::GetSQLDropIndex(CString p_user,CString p_indexName) const
{
  CString sql = "DROP INDEX " + p_user + "." + p_indexName;
  return sql;
}

// Get SQL to read the referential constraints from the catalog
CString 
SQLInfoSQLServer::GetSQLTableReferences(CString p_schema
                                       ,CString p_tablename
                                       ,CString p_constraint /*=""*/
                                       ,int     /*p_maxColumns = SQLINFO_MAX_COLUMNS*/) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  CString query = "SELECT fok.name          as foreign_key_constraint\n"
                  "      ,sch.name          as schema_name\n"
                  "      ,tab.name          as table_name\n"
                  "      ,col.name          as column_name\n"
                  "      ,pri.name          as primary_table_name\n"
                  "      ,pky.name          as primary_key_column\n"
                  "      ,0                 as deferrable\n"
                  "      ,0                 as initially_deferred\n"
                  "      ,fok.is_disabled   as disabled\n"
                  "      ,1                 as match_option\n"
                  "      ,fok.delete_referential_action as delete_rule\n"
                  "      ,fok.update_referential_action as update_rule\n"
                  "  FROM sys.foreign_keys        fok\n"
                  "      ,sys.foreign_key_columns fkc\n"
                  "      ,sys.schemas sch\n"
                  "      ,sys.tables  tab\n"
                  "      ,sys.columns col\n"
                  "      ,sys.tables  pri\n"
                  "      ,sys.columns pky\n"
                  " WHERE fok.type = 'F'\n"
                  "   AND fok.parent_object_id     = tab.object_id\n"
                  "   AND tab.schema_id            = sch.schema_id\n"
                  "   AND fkc.constraint_object_id = fok.object_id\n"
                  "   AND fkc.parent_object_id     = col.object_id\n"
                  "   AND fkc.parent_column_id     = col.column_id\n"
                  "   AND fkc.referenced_object_id = pri.object_id\n"
                  "   AND fkc.referenced_object_id = pky.object_id\n"
                  "   AND fkc.referenced_column_id = pky.column_id";
  if(!p_schema.IsEmpty())
  {
    query += "\n   AND sch.name = '" + p_schema + "'";
  }
  if(!p_tablename.IsEmpty())
  {
    query += "\n   AND tab.name = '" + p_tablename + "'";
  }
  if(!p_constraint.IsEmpty())
  {
    query += "\n   AND fok.name = '" + p_constraint + "'";
  }
  return query;
}

// Get the SQL to determine the sequence state in the database
CString 
SQLInfoSQLServer::GetSQLSequence(CString p_schema,CString p_tablename,CString p_postfix /*= "_seq"*/) const
{
  CString sequence = p_tablename + p_postfix;
  sequence.MakeLower();
  p_schema.MakeLower();
  CString sql = "SELECT name as sequence_name\n"
                "      ,current_value\n"
                "      ,decode(is_cycling,0,1,0) *\n"
                "       decode(is_cached, 0,1,0) as is_correct"
                "  FROM sys.sequences seq\n"
                "      ,sys.schemas   sch\n"
                " WHERE sch.object_id = seq.schema_id\n"
                "   AND seq.name = '" + sequence + "'\n"
                "   AND sch.name = '" + p_schema + "'";
  return sql;
}

// Create a sequence in the database
CString 
SQLInfoSQLServer::GetSQLCreateSequence(CString p_schema,CString p_tablename,CString p_postfix /*= "_seq"*/,int p_startpos) const
{
  CString sql("CREATE SEQUENCE ");

  if(!p_schema.IsEmpty())
  {
    sql += p_schema + ".";
  }
  sql += p_tablename + p_postfix;
  sql.AppendFormat(" START WITH %d",p_startpos);
  sql += " NO CYCLE NO CACHE";
  return sql;
}

// Remove a sequence from the database
CString 
SQLInfoSQLServer::GetSQLDropSequence(CString p_schema,CString p_tablename,CString p_postfix /*= "_seq"*/) const
{
  CString sql;
  sql = "DROP SEQUENCE " + p_schema + "." + p_tablename + p_postfix;
  return sql;
}

// Gets the SQL for the rights on the sequence
CString
SQLInfoSQLServer::GetSQLSequenceRights(CString p_schema,CString p_tableName,CString p_postfix /*="_seq"*/) const
{
  CString sequence = p_tableName + p_postfix;
  if(!p_schema.IsEmpty())
  {
    sequence = p_schema + "." + sequence;
  }
  return "GRANT SELECT ON " + sequence + " TO " + GetGrantedUsers();
}

// Remove a stored procedure from the database
void 
SQLInfoSQLServer::DoRemoveProcedure(CString& p_procedureName) const
{
  SQLQuery query(m_database);
  query.DoSQLStatement("DROP PROCEDURE " + p_procedureName);

}

// Get SQL for your session and controlling terminal
CString
SQLInfoSQLServer::GetSQLSessionAndTerminal() const
{
  CString query = "SELECT rtrim(hostprocess)\n"
                  "      ,rtrim(hostname)\n"    
                  "  FROM master.dbo.sysprocesses\n"
                  " WHERE hostprocess = host_id()\n"
                  "   AND hostname = host_name()\n"
                  "   AND rtrim(program_name) = 'Pronto'";
  return query;
}

// Get SQL to check if session number exists
CString 
SQLInfoSQLServer::GetSQLSessionExists(CString p_sessionID) const
{
  return "SELECT DISTINCT rtrim(hostprocess)\n"
         "  FROM master.dbo.sysprocesses\n"
         " WHERE rtrim(hostprocess) <> " + p_sessionID;
}

// Get SQL for unique session ID
CString 
SQLInfoSQLServer::GetSQLUniqueSessionId(const CString& /*p_databaseName*/,const CString& /*p_sessionTable*/) const
{
  return "SELECT DISTINCT rtrim(hostprocess)\n"
         "  FROM master.dbo.sysprocesses\n"
         " WHERE rtrim(hostprocess) <> '0'\n"
         "   AND dbid = db_id()";
}

// Get SQL for searching a session
CString 
SQLInfoSQLServer::GetSQLSearchSession(const CString& /*p_databaseName*/,const CString& p_sessionTable) const
{
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


// See if a column exists within a table
bool   
SQLInfoSQLServer::DoesColumnExistsInTable(CString& p_owner,CString& p_tableName,CString& p_column) const
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
SQLInfoSQLServer::GetSQLPrimaryKeyConstraintInformation(CString p_schema,CString p_tableName) const
{
  p_schema.MakeLower();
  p_tableName.MakeLower();

  CString query = "SELECT count(*)\n"
                 "  FROM dbo.sysobjects tab\n"
                 "      ,dbo.sysobjects con\n"
                 "      ,dbo.sysusers   use\n"
                 " WHERE tab.id    = con.parent_obj\n"
                 "   AND tab.uid   = use.uid\n"
                 "   AND con.xtype = 'PK'\n"
                 "   AND con.type  = 'K '"
                 "   AND use.name  = '" + p_schema    + "'\n"
                 "   AND tab.name  = '" + p_tableName + "'";
  return query;
}

// Does the named constraint exist in the database
bool    
SQLInfoSQLServer::DoesConstraintExist(CString p_constraintName) const
{
// To be implemented
  return false;
}

// Gets the lock-table query
CString 
SQLInfoSQLServer::GetSQLLockTable(CString& p_tableName,bool p_exclusive) const
{
  CString query = "SELECT * FROM " + p_tableName + " WITH "  ;
  query += p_exclusive ? "(TABLOCKX)" : "(TABLOCK)";
  return query;
}

// Get query to optimize the table statistics
CString 
SQLInfoSQLServer::GetSQLOptimizeTable(CString& /*p_owner*/,CString& /*p_tableName*/,int& /*p_number*/)
{
  return "";
}

// Getting the fact that there is only **one** (1) user session in the database
bool
SQLInfoSQLServer::GetOnlyOneUserSession()
{
  // Yet to implement
  return true;
}

// SQL DDL STATEMENTS
// ==================

CString
SQLInfoSQLServer::GetCreateColumn(CString p_schema,CString p_tablename,CString p_columnName,CString p_typeDefinition,bool p_notNull)
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
SQLInfoSQLServer::GetSQLDropColumn(CString p_schema,CString p_tablename,CString p_columnName) const
{
  return "ALTER TABLE " + p_schema + "." + p_tablename + "\n"
         " DROP COLUMN " + p_columnName;
}

// Add a foreign key to a table
CString
SQLInfoSQLServer::GetCreateForeignKey(CString p_tablename,CString p_constraintname,CString p_column,CString p_refTable,CString p_primary)
{
  CString sql = "ALTER TABLE " + p_tablename + "\n"
                "  ADD CONSTRAINT " + p_constraintname + "\n"
                "      FOREIGN KEY (" + p_column + ")\n"
                "      REFERENCES " + p_refTable + "(" + p_primary + ")";
  return sql;
}

CString 
SQLInfoSQLServer::GetModifyColumnType(CString p_schema,CString p_tablename,CString p_columnName,CString p_typeDefinition)
{
  CString sql = "ALTER TABLE  " + p_schema + "." + p_tablename  + "\n"
                "ALTER COLUMN " + p_columnName + " " + p_typeDefinition;
  return sql;
}

CString 
SQLInfoSQLServer::GetModifyColumnNull(CString p_schema,CString p_tablename,CString p_columnName,bool p_notNull)
{
  CString sql = "ALTER TABLE  " + p_schema + "." + p_tablename  + "\n"
                "ALTER COLUMN " + p_columnName + (p_notNull ? " NOT " : " ") + "NULL";
  return sql;
}

// Get the SQL to drop a view. If precursor is filled: run that SQL first!
CString 
SQLInfoSQLServer::GetSQLDropView(CString p_schema,CString p_view,CString& p_precursor)
{
  p_precursor.Empty();
  return "DROP VIEW " + p_schema + "." + p_view;
}

// Create or replace a database view
CString 
SQLInfoSQLServer::GetSQLCreateOrReplaceView(CString p_schema,CString p_view,CString p_asSelect) const
{
  return "CREATE VIEW " + p_schema + "." + p_view + "\n" + p_asSelect;
}

// SQL DDL ACTIONS
// ===================================================================

// Do the commit for the DDL commands in the catalog
void 
SQLInfoSQLServer::DoCommitDDLcommands() const
{
  SQLQuery query(m_database);
  query.DoSQLStatement("COMMIT WORK");
}

// Do the commit for the DML commands in the database
// ODBC driver auto commit mode will go wrong!!
void
SQLInfoSQLServer::DoCommitDMLcommands() const
{
//   SQLQuery query(m_database);
//   query.DoSQLStatement("COMMIT WORK");
}

// Remove a column from a table
void
SQLInfoSQLServer::DoDropColumn(CString p_tableName,CString p_columName)
{
  CString sql = "ALTER TABLE  " + p_tableName + "\n"
                " DROP COLUMN " + p_columName;
  SQLQuery query(m_database);
  query.TryDoSQLStatement(sql);
}

// Does the named view exists in the database
bool
SQLInfoSQLServer::DoesViewExists(CString& /*p_viewName*/)
{
  // To be implemented
  return true;
}

// Must create temporary tables runtime 
bool 
SQLInfoSQLServer::GetMustMakeTemptablesAtRuntime() const
{
  // FALSE: GLOBAL TEMPORARY TABLES IN THE ENGINE
  // TRUE:  TRUE SESSION TEMPTABLES MUST ALWAYS BE CREATED (Informix)
  return false;
}

// Create a temporary table in an optimized manner with the givven index column
void
SQLInfoSQLServer::DoMakeTemporaryTable(CString& p_tableName,CString& p_content,CString& p_indexColumn) const
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
SQLInfoSQLServer::DoRemoveTemporaryTable(CString& p_tableName) const
{
  SQLQuery query(m_database);

  // Every error can be ignored. Can still be in use by another user and/or session
  // The table contents will then removed for this session
  query.TryDoSQLStatement("DELETE FROM #"    + p_tableName);
  query.TryDoSQLStatement("TRUNCATE TABLE #" + p_tableName);
  query.TryDoSQLStatement("DROP TABLE #"     + p_tableName);
}

// Create a procedure in the database
void 
SQLInfoSQLServer::DoMakeProcedure(CString& p_procName,CString p_table,bool /*p_noParameters*/,CString& p_codeBlock)
{
  SQLQuery query(m_database);
  query.DoSQLStatement(p_codeBlock);
  query.DoSQLStatement("GRANT EXECUTE ON " + p_procName + " TO PUBLIC");
}

// Rename a database table 
void 
SQLInfoSQLServer::DoRenameTable(CString& p_oldName,CString& p_newName) const
{
  // Beware: without 'TABLE' in the statement
  CString rename = "RENAME " + p_oldName + " to " + p_newName;
  SQLQuery query(m_database);
  query.DoSQLStatement(rename);
  DoCommitDDLcommands();
}

// PERSISTENT-STORED MODULES (SPL / PL/SQL)
// ====================================================================

// Get the user error text from the database
CString 
SQLInfoSQLServer::GetUserErrorText(CString& /*p_procName*/) const
{
  return "";
}

// Get assignment to a variable in SPL
CString 
SQLInfoSQLServer::GetSPLAssignment(CString p_variable) const
{
  return "SET " + p_variable + " = ";
}

// Get the start of a SPL While loop
CString 
SQLInfoSQLServer::GetSPLStartWhileLoop(CString p_condition) const
{
  return "WHILE " + p_condition + "\n  BEGIN\n";
}

// Get the end of a SPL while loop
CString 
SQLInfoSQLServer::GetSPLEndWhileLoop() const
{
  return "END;\n";
}

// Get stored procedure call
CString 
SQLInfoSQLServer::GetSQLSPLCall(CString p_procName) const
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
SQLInfoSQLServer::GetBuildedParameterList(size_t p_numOfParameters) const
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
SQLInfoSQLServer::GetParameterType(CString& p_type) const
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
SQLInfoSQLServer::GetSQLString(const CString& p_string) const
{
  CString s = p_string;
  s.Replace("'","''");
  CString kwoot = GetQuoteCharacter();
  return  kwoot + s + kwoot;
}

// Get date string in engine format
CString 
SQLInfoSQLServer::GetSQLDateString(int p_year,int p_month,int p_day) const
{
  CString retval;
  retval.Format("{d '%04d-%02d-%02d' }",p_year,p_month,p_day);
  return retval;
}  

// Get time string in database engine format
CString 
SQLInfoSQLServer::GetSQLTimeString(int p_hour,int p_minute,int p_second) const
{
  CString retval;
  retval.Format("{t '%02d:%02d:%02d' }",p_hour,p_minute,p_second);
  return retval;
}  
  
// Get date-time string in database engine format
CString 
SQLInfoSQLServer::GetSQLDateTimeString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  CString retval;
  retval.Format("{ts '%04d-%02d-%02d %02d:%02d:%02d' }",p_year,p_month,p_day,p_hour,p_minute,p_second);
  return retval;
}  

// Get date-time bound parameter string in database format
CString 
SQLInfoSQLServer::GetSQLDateTimeBoundString() const
{
  return "{ts ?}";
}

// Stripped data for the parameter binding
CString
SQLInfoSQLServer::GetSQLDateTimeStrippedString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  CString retval;
  retval.Format("%04d-%02d-%02d %02d:%02d:%02d",p_year,p_month,p_day,p_hour,p_minute,p_second);
  return retval;
}

// Get the SPL sourcecode for a stored procedure as registered in the database
CString 
SQLInfoSQLServer::GetSPLSourcecodeFromDatabase(const CString& /*p_owner*/,const CString& /*p_procName*/) const
{
  return "";
}

// Get the SPL datatype for integer
CString 
SQLInfoSQLServer::GetSPLIntegerType() const
{
  return "integer";
}

// Get the SPL datatype for a decimal
CString 
SQLInfoSQLServer::GetSPLDecimalType() const
{
  return "number";
}

// Get the SPL declaration for a cursor
CString 
SQLInfoSQLServer::GetSPLCursorDeclaratie(CString& p_variableName,CString& p_query) const
{
  // TODO: Check
  return "CURSOR " + p_variableName + " IS " + p_query + ";";
}

// Get the SPL cursor found row parameter
CString 
SQLInfoSQLServer::GetSPLCursorFound(CString& /*p_cursorName*/) const
{
  // TODO: Implement
  return "";
}

// Get the SPL cursor row-count variable
CString 
SQLInfoSQLServer::GetSPLCursorRowCount(CString& /*p_variable*/) const
{
  // TODO: Implement
  return "";
}

// Get the SPL datatype for a declaration of a row-variable
CString 
SQLInfoSQLServer::GetSPLCursorRowDeclaration(CString& /*p_cursorName*/,CString& /*p_variableName*/) const
{
  // TODO: Implement
  return "";
}

CString 
SQLInfoSQLServer::GetSPLFetchCursorIntoVariables(CString               p_cursorName
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
SQLInfoSQLServer::GetSPLFetchCursorIntoRowVariable(CString& p_cursorName,CString p_variableName) const
{ 
  // TODO: Check
  return "FETCH " + p_cursorName + " INTO " + p_variableName+ ";";
}

// Get the SPL no-data exception clause
CString 
SQLInfoSQLServer::GetSPLNoDataFoundExceptionClause() const
{
  // TODO: Check
  return "WHEN NO_DATA THEN";
}

// Get the SPL form of raising an exception
CString 
SQLInfoSQLServer::GetSPLRaiseException(CString p_exceptionName) const
{
  // TODO: Check
  return "RAISE " + p_exceptionName + ";";
}

// Get the fact that the SPL has server functions that return more than 1 value
bool    
SQLInfoSQLServer::GetSPLServerFunctionsWithReturnValues() const
{
  return true;
}

// Calling a stored function or procedure if the RDBMS does not support ODBC call escapes
SQLVariant*
SQLInfoSQLServer::DoSQLCall(SQLQuery* /*p_query*/,CString& /*p_schema*/,CString& /*p_procedure*/)
{
  return nullptr;
}

// SPECIALS
// ==========================================================================

// Translate database-errors to a human readable form
CString 
SQLInfoSQLServer::TranslateErrortext(int p_error,CString p_errorText) const
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

// End of namespace
}
