////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoInformix.cpp
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
#include "SQLInfoInformix.h"
#include "SQLQuery.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{
  
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

// Get the physical database name
CString 
SQLInfoInformix::GetPhysicalDatabaseName() const
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

// System catalog supports full ISO schemas (same tables per schema)
bool
SQLInfoInformix::GetUnderstandsSchemas() const
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
  // SET CONSTRAINTS DEFERRED is supported
  return true;
}

// Database has ORDER BY with an expression, e.g. ORDER BY UPPER(column-name)
// Work-around is "SELECT UPPER(column-name) AS something.....ORDER BY something
bool
SQLInfoInformix::SupportsOrderByExpression() const
{
  return false;
}

// Supports the ODBC escape sequence {[?=] CALL procedure (?,?,?)}
bool
SQLInfoInformix::SupportsODBCCallEscapes() const
{
  return true;
}

// Catalog query for the default value of a table's column
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
  // TIJD is implemented as TIME :-)
  return false;
}

// If the database does not support the datatype INTERVAL, it can be implemented as a DECIMAL
bool 
SQLInfoInformix::GetIntervalIsDecimal() const
{
  // Interval is implemeted in Informix as INTERVAL :-)
  return false;
}

// Get the concatenation operator
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

// Get default NULL for parameter list input
CString 
SQLInfoInformix::GetDefaultNULL() const
{
  return "";    
}

// Parameter is for INPUT and OUTPUT in parameter list
CString 
SQLInfoInformix::GetParameterINOUT() const
{
  return "";    
}

// Parameter is for OUTPUT only in parameter list
CString 
SQLInfoInformix::GetParameterOUT() const
{
  return "";    
}

// Get the datatype of the audited user (h_user) in a stored procedure
CString 
SQLInfoInformix::GetAuditUserDatatype() const
{
  return "varchar(50)";
}

// Get the datatype of the audited user (h_user) as variable in a stored-procedure
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
  // 'Simply' a drop of a table
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
  // Does nothing in Informix because we use a 0 on a SERIAL datatype
  return p_columns;
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
  // Not needed in Informix
  return "";
}

// Gets the table definition-form of a primary key
CString
SQLInfoInformix::GetPrimaryKeyDefinition(CString p_schema,CString p_tableName,bool p_temporary) const
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
SQLInfoInformix::GetPrimaryKeyConstraint(CString /*p_schema*/,CString p_tablename,CString p_primary) const
{
  return "ALTER TABLE " + p_tablename + "\n"
         "  ADD CONSTRAINT PRIMARY KEY (" + p_primary + ")\n"
         "      CONSTRAINT pk_" + p_tablename;
}

CString
SQLInfoInformix::GetPrimaryKeyConstraint(MPrimaryMap& p_primaries) const
{
  CString query("ALTER TABLE ");
  CString constraintName;

  for(auto& prim : p_primaries)
  {
    if(prim.m_columnPosition == 1)
    {
      if(!prim.m_schema.IsEmpty())
      {
        query += prim.m_schema + ".";
      }
      query += prim.m_table + "\n";
      query += "  ADD CONSTRAINT PRIMARY KEY (";
    }
    else
    {
      query += ",";
    }
    query += prim.m_columnName;
    constraintName = prim.m_constraintName;
  }
  query += ")\n";
  query += "      CONSTRAINT " + constraintName;

  return query;
}

// Get the sql to add a foreign key to a table
CString 
SQLInfoInformix::GetSQLForeignKeyConstraint(DBForeign& p_foreign) const
{
  // Construct the correct tablename
  CString table  (p_foreign.m_tablename);
  CString primary(p_foreign.m_primaryTable);
  if(!p_foreign.m_schema.IsEmpty())
  {
    table   = p_foreign.m_schema + "." + table;
    primary = p_foreign.m_schema + "." + primary;
  }

  // The base foreign key command (non-standard!)
  CString query = "ALTER TABLE " + table + "\n"
                  "  ADD CONSTRAINT FOREIGN KEY (" + p_foreign.m_column + ")\n"
                  "      REFERENCES " + primary + "(" + p_foreign.m_primaryColumn + ")"
                  "      CONSTRAINT " + p_foreign.m_constraintname;
  if(p_foreign.m_initiallyDeffered)
  {
    query += "\n      NOVALIDATE";
  }
  switch(p_foreign.m_deleteRule)
  {
    case 0: query += "\n      ON DELETE CASCADE"; 
            break;
    default:// In essence: ON DELETE RESTRICT, but that's already the default
    case 1: break;
  }
  return query;
}

CString
SQLInfoInformix::GetSQLForeignKeyConstraint(MForeignMap& p_foreigns) const
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
                  "  ADD CONSTRAINT FOREIGN KEY (";

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
  query += ")\n";
  query += "      CONSTRAINT " + foreign.m_foreignConstraint;

  // Add all relevant options
  switch(foreign.m_deferrable)
  {
    case SQL_INITIALLY_DEFERRED:  query += "\n      NOVALIDATE"; break;
    default:                      break;
  }
  switch(foreign.m_deleteRule)
  {
    case SQL_CASCADE: query += "\n      ON DELETE CASCADE"; break;
    default:          // In essence: ON DELETE RESTRICT, but that's already the default
                      break;
  }
  return query;
}


// Get the sql (if possible) to change the foreign key constraint
CString 
SQLInfoInformix::GetSQLAlterForeignKey(DBForeign& p_origin,DBForeign& p_requested) const
{
  CString query = "SET CONSTRAINTS " + p_origin.m_constraintname + " ";

  // Add all relevant options
  if(p_origin.m_initiallyDeffered != p_requested.m_initiallyDeffered)
  {
    query += p_requested.m_initiallyDeffered ? "DEFERRED" : "IMMEDIATE";
  }
  else if(p_origin.m_enabled != p_requested.m_enabled)
  {
    query += p_requested.m_enabled ? "ENABLED" : "DISABLED";
  }
  return query;
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
SQLInfoInformix::GetSQLGrantAllOnTable(CString /*p_schema*/,CString p_tableName,bool p_grantOption /*= false*/)
{
  CString sql = "GRANT ALL ON " + p_tableName + " TO " + GetGrantedUsers();
  if(p_grantOption)
  {
    sql += " WITH GRANT OPTION";
  }
  return sql;
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

// Gets the fact if a SELECT must be in between parenthesis for an assignment
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

// Gets the sub transaction commands
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

// Get SQL to check if a stored procedure already exists in the database
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
SQLInfoInformix::GetSQLTableExists(CString /*p_schema*/,CString p_tablename) const
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

// Get SQL to read all indices for a table
CString 
SQLInfoInformix::GetSQLTableIndices(CString /*p_user*/,CString p_tableName) const
{
  CString query;
  p_tableName.MakeLower();

  // Reads all current indices in the database in a list
  // So we can figure out if an index need to be generated
  for(int ind = 1; ind <= 8; ++ind)
  {
    if(!query.IsEmpty())
    {
      query += "\nUNION ALL\n";
    }
    query.AppendFormat("SELECT idx.idxname AS index_name\n"
                       "      ,col.colname\n"
                       "      ,%d   AS index_column\n"
                       "      ,CASE WHEN idx.idxtype = 'D' THEN 0\n"
                       "            WHEN idx.idxtype = 'U' THEN 1\n"
                       "            ELSE 0 END AS is_unique\n"
                       "      ,CASE WHEN idx.part%d < 0 THEN 1\n"
                       "            ELSE 0 END AS descending\n"
                       "  FROM sysindexes idx\n"
                       "      ,systables  tab\n"
                       "      ,syscolumns col\n"
                       " WHERE tab.tabid = idx.tabid\n"
                       "   AND tab.tabname = '%s'\n"
                       "   AND col.tabid   = idx.tabid\n"
                       "   AND col.colno   = abs(idx.part%d)\n"
                       "   AND idx.idxname[1] != ' '"
                      ,ind,ind,p_tableName,ind);
  }
  return query;
}

// Get SQL to create an index for a table
CString 
SQLInfoInformix::GetSQLCreateIndex(CString /*p_user*/,CString p_tableName,DBIndex* p_index) const
{
  CString sql("CREATE ");
  if(p_index->m_unique)
  {
    sql += "UNIQUE ";
  }
  sql += " INDEX ON ";
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

// Get SQL to create an index for a table
// CREATE [UNIQUE] INDEX [<schema>.]indexname ON [<schema>.]tablename(column [ASC|DESC] [,...]);
CString
SQLInfoInformix::GetSQLCreateIndex(MStatisticsMap& p_indices) const
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
    if(index.m_ascending != "A")
    {
      query += " DESC";
    }
  }
  query += ")";
  return query;
}

// Get extra filter expression for an index column
CString
SQLInfoInformix::GetIndexFilter(MetaStatistics& /*p_index*/) const
{
  return "";
}

// Get SQL to drop an index
CString 
SQLInfoInformix::GetSQLDropIndex(CString p_user,CString p_indexName) const
{
  CString sql = "DROP INDEX " + p_indexName;
  return sql;
}


// Get SQL to read the referential constraints from the catalog
CString 
SQLInfoInformix::GetSQLTableReferences(CString p_schema
                                      ,CString p_tablename
                                      ,CString p_constraint /*=""*/
                                      ,int     p_maxColumns /*= SQLINFO_MAX_COLUMNS*/) const
{
  CString query;
  p_schema.MakeLower();
  p_tablename.MakeLower();
  p_constraint.MakeLower();

  for(int ind = 1;ind <= p_maxColumns; ++ind)
  {
    CString part;
    part.Format("SELECT con.constrname  AS foreign_key_constraint\n"
                "      ,tab.owner       AS schema_name\n"
                "      ,tab.tabname     AS table_name\n"
                "      ,col.colname     AS column_name\n"
                "      ,pri.tabname     AS primary_table_name\n"
                "      ,pcl.colname     AS primary_column_name\n"
                "      ,1               AS deferrable\n"
                "      ,0               AS initially_deferred\n"
                "      ,1               AS enabled\n"
                "      ,CASE WHEN ref.matchtype = 'N' THEN 0\n"
                "            WHEN ref.matchtype = 'P' THEN 1\n"
                "            ELSE 0\n"
                "       END  AS match_option\n"
                "      ,CASE WHEN ref.updrule = 'R' THEN 1\n"
                "            WHEN ref.updrule = 'C' THEN 0\n"
                "            WHEN ref.updrule = 'N' THEN 2\n"
                "            WHEN ref.updrule = 'D' THEN 4\n"
                "            ELSE 0\n"
                "       END  AS update_rule\n"
                "      ,CASE WHEN ref.delrule = 'R' THEN 1\n"
                "            WHEN ref.delrule = 'C' THEN 0\n"
                "            WHEN ref.delrule = 'N' THEN 2\n"
                "            WHEN ref.delrule = 'D' THEN 4\n"
                "            ELSE 0\n"
                "       END  AS delete_rule\n"
                "  FROM sysconstraints con\n"
                "      ,systables      tab\n"
                "      ,syscolumns     col\n"
                "      ,sysreferences  ref\n"
                "      ,systables      pri\n"
                "      ,sysindexes     idx\n"
                "      ,sysconstraints pcn\n"
                "      ,sysindexes     pix\n"
                "      ,syscolumns     pcl\n"
                " WHERE tab.tabid      = con.tabid\n"
                "   AND con.constrid   = ref.constrid\n"
                "   AND ref.ptabid     = pri.tabid\n"
                "   AND con.idxname    = idx.idxname\n"
                "   AND col.tabid      = tab.tabid\n"
                "   AND col.colno      = idx.part%d\n"
                "   AND pcn.tabid      = pri.tabid\n"
                "   AND pix.idxname    = pcn.idxname\n"
                "   AND pcl.tabid      = pri.tabid\n"
                "   AND pcl.colno      = pix.part%d\n"
                "   and con.constrtype = 'R'\n"
                "   AND pcn.constrtype = 'P'\n"
               ,ind
               ,ind);
    if(!p_schema.IsEmpty())
    {
      part += "\n    AND tab.owner = '" + p_schema + "'";
    }
    if(!p_tablename.IsEmpty())
    {
      part += "\n   AND tab.tabname    = '" + p_tablename + "'";
    }
    if(!p_constraint.IsEmpty())
    {
      part += "\n    AND con.constrname = '" + p_constraint + "'";
    }
    // Add to the query
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
SQLInfoInformix::GetSQLSequence(CString p_schema,CString p_tablename,CString p_postfix /*= "_seq"*/) const
{
  p_schema.MakeLower();
  CString sequence = p_tablename + p_postfix;
  sequence.MakeLower();
  CString sql = "SELECT dom.name as sequence_name\n"
                "      ,seq.start_val as current_value"
                "      ,decode(seq.inc_val,1,1,0) *\n"
                "       decode(seq.cycle,'0',1,0) *\n"
                "       decode(seq.cache, 0, 1,0) *\n"
                "       decode(seq.order,'1',1,0) as is_correct\n"
                "  FROM syssequences seq\n"
                "      ,sysdomains   dom\n"
                " WHERE dom.id = seq.id\n"
                "   AND dom.owner = '" + p_schema + "'\n"
                "   AND dom.name  = '" + sequence + "'\n";
              //"   AND dom.type  = 3"; ??
  return sql;
}

// Create a sequence in the database
CString 
SQLInfoInformix::GetSQLCreateSequence(CString p_schema,CString p_tablename,CString p_postfix /*= "_seq"*/,int p_startpos) const
{
  CString sql("CREATE SEQUENCE ");
  CString sequence = p_tablename + p_postfix;

  if(!p_schema.IsEmpty())
  {
    sql += p_schema + ".";
  }
  sql += sequence;
  sql.AppendFormat(" START WITH %d",p_startpos);
  sql += " NOCYCLE NOCACHE ORDER";
  return sql;
}

// Remove a sequence from the database
CString 
SQLInfoInformix::GetSQLDropSequence(CString p_schema,CString p_tablename,CString p_postfix /*= "_seq"*/) const
{
  CString sequence = p_tablename + p_postfix;
  CString sql;
  sql = "DROP SEQUENCE " + p_schema + "." + sequence;
  return sql;
}

// Gets the SQL for the rights on the sequence
CString
SQLInfoInformix::GetSQLSequenceRights(CString p_schema,CString p_tablename,CString p_postfix /*="_seq"*/) const
{
  CString sequence = p_tablename + p_postfix;
  if(!p_schema.IsEmpty())
  {
    sequence = p_schema + "." + sequence;
  }
  CString sql = "GRANT SELECT ON " + sequence + " TO " + GetGrantedUsers();
  return sql;
}

// Remove a stored procedure from the database
void
SQLInfoInformix::DoRemoveProcedure(CString& p_procedureName) const
{
  SQLQuery sql(m_database);
  sql.TryDoSQLStatement("DROP PROCEDURE " + p_procedureName);

}

// Get SQL for your session and controlling terminal
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

// Get SQL to check if session number exists
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

// See if a column exists within a table
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
SQLInfoInformix::GetSQLPrimaryKeyConstraintInformation(CString /*p_schema*/,CString p_tableName) const
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

// Gets the triggers for a table
CString
SQLInfoInformix::GetSQLTriggers(CString p_schema,CString p_table) const
{
  return "";
}

// SQL DDL STATEMENTS
// ==================

CString
SQLInfoInformix::GetCreateColumn(CString /*p_schema*/,CString p_tablename,CString p_columnName,CString p_typeDefinition,bool p_notNull)
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
SQLInfoInformix::GetSQLDropColumn(CString /*p_schema*/,CString p_tablename,CString p_columnName) const
{
  return "ALTER TABLE " + p_tablename + "\n"
         " DROP COLUMN " + p_columnName;
}

// Add a foreign key to a table
CString 
SQLInfoInformix::GetCreateForeignKey(CString p_tablename,CString p_constraintname,CString p_column,CString p_refTable,CString p_primary)
{
  CString sql = "ALTER TABLE " + p_tablename + "\n"
                "  ADD CONSTRAINT FOREIGN KEY (" + p_column + ")\n"
                "      REFERENCES " + p_refTable + "(" + p_primary + ")\n"
                "      CONSTRAINT + " + p_constraintname;
  return sql;
}

CString 
SQLInfoInformix::GetModifyColumnType(CString /*p_schema*/,CString p_tablename,CString p_columnName,CString p_typeDefinition)
{
  CString sql = "ALTER  TABLE  " + p_tablename + "\n";
                "MODIFY COLUMN " + p_columnName + " " + p_typeDefinition;
  return sql;
}

CString 
SQLInfoInformix::GetModifyColumnNull(CString /*p_schema*/,CString p_tablename,CString p_columnName,bool p_notNull)
 {
  CString sql = "ALTER  TABLE  " + p_tablename + "\n";
                "MODIFY COLUMN " + p_columnName + " " + (p_notNull ? "NOT " : "") + "NULL";
  return sql;
}

// Get the SQL to drop a view. If precursor is filled: run that SQL first!
CString 
SQLInfoInformix::GetSQLDropView(CString /*p_schema*/,CString p_view,CString& p_precursor)
{
  p_precursor.Empty();
  return "DROP VIEW " + p_view;
}

// Create or replace a database view
CString 
SQLInfoInformix::GetSQLCreateOrReplaceView(CString /*p_schema*/,CString p_view,CString p_asSelect) const
{
  return "CREATE VIEW " + p_view + "\n" + p_asSelect;
}

// Create or replace a trigger
CString
SQLInfoInformix::CreateOrReplaceTrigger(MetaTrigger& /*p_trigger*/) const
{
  return "";
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
// ODBC driver auto commit mode will go wrong!!
void
SQLInfoInformix::DoCommitDMLcommands() const
{
  // Does NOTHING in INFOMRIX and cannot do something
  // Commit follows from a BEGIN/COMMIT transaction by AUTOCOMMIT from the driver
}

// Remove a column from a table
void
SQLInfoInformix::DoDropColumn(CString p_tableName,CString p_columName)
{
  CString sql = "ALTER TABLE  " + p_tableName + "\n"
                " DROP COLUMN " + p_columName;
  SQLQuery query(m_database);
  query.TryDoSQLStatement(sql);
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

// Must create temporary tables runtime 
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
  (void)p_procName;   // Not in Informix.
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

// Build a parameter list for calling a stored procedure
CString 
SQLInfoInformix::GetBuildedParameterList(size_t p_numOfParameters) const
{
  // Stands for the '?' binding parameters in a stored-procedure
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

// Parameter type for stored procedure for a given column type for parameters and return types
CString
SQLInfoInformix::GetParameterType(CString& p_type) const
{
  return p_type;
}

// GENERAL SQL ACTIONS
// =================================================================

// Makes a SQL string from a given string, with all the right quotes
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
  // Informix used to be depended on the DBFORMAT parameter
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
  // Informix used to be depended on the DBFORMAT parameter
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

// Calling a stored function or procedure if the RDBMS does not support ODBC call escapes
SQLVariant*
SQLInfoInformix::DoSQLCall(SQLQuery* /*p_query*/,CString& /*p_schema*/,CString& /*p_procedure*/)
{
  return nullptr;
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

// End of namespace
}
