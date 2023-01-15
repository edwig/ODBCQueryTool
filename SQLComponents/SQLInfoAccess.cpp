////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoAccess.cpp
//
// Copyright (c) 1998-2022 ir. W.E. Huisman
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
XString 
SQLInfoAccess::GetRDBMSVendorName() const
{
  // The name of the database vendor
  return "Microsoft";
}

// Get the physical database name
XString 
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

// Supports functions at the place of table columns in create/alter index statement
bool
SQLInfoAccess::GetRDBMSSupportsFunctionalIndexes() const
{
  return false;
}

// Gets the maximum length of an SQL statement
unsigned long 
SQLInfoAccess::GetRDBMSMaxStatementLength() const
{
  // No limit
  return 0; 
}

// Database must commit DDL commands in a transaction
bool 
SQLInfoAccess::GetRDBMSMustCommitDDL() const
{
  return false;
}

// Correct maximum precision,scale for a NUMERIC datatype
void 
SQLInfoAccess::GetRDBMSNumericPrecisionScale(SQLULEN& /*p_precision*/, SQLSMALLINT& /*p_scale*/) const
{
  // NO-OP
}

// Maximum for a VARCHAR to be handled without AT-EXEC data. Assume NVARCHAR is half that size!
int
SQLInfoAccess::GetRDBMSMaxVarchar() const
{
  return 8000;
}

// KEYWORDS

// Keyword for the current date and time
XString 
SQLInfoAccess::GetKEYWORDCurrentTimestamp() const
{
  return "GETDATE()";
}

// String for the current date
XString 
SQLInfoAccess::GetKEYWORDCurrentDate() const
{
  return "GETDATE()";
}

// Get the concatenation operator
XString 
SQLInfoAccess::GetKEYWORDConcatanationOperator() const
{
  return "+";
}

// Get quote character for strings
XString 
SQLInfoAccess::GetKEYWORDQuoteCharacter() const
{
  return "\'";
}

// Get quote character around reserved words as an identifier
XString 
SQLInfoAccess::GetKEYWORDReservedWordQuote() const
{
  return "\"";
}

// Get default NULL for parameter list input
XString 
SQLInfoAccess::GetKEYWORDParameterDefaultNULL() const
{
  return "= NULL";
}

// Parameter is for INPUT and OUTPUT in parameter list
XString 
SQLInfoAccess::GetKEYWORDParameterINOUT() const
{
  return "OUTPUT";
}

// Parameter is for OUTPUT only in parameter list
XString 
SQLInfoAccess::GetKEYWORDParameterOUT() const
{
  return "OUTPUT";
}

// Get datatype of the IDENTITY primary key in a Network database
XString 
SQLInfoAccess::GetKEYWORDNetworkPrimaryKeyType() const
{
  return "INTEGER IDENTITY(1,1)";
}

// Get datatype for timestamp (year to second)
XString 
SQLInfoAccess::GetKEYWORDTypeTimestamp() const
{
  return "DATETIME";
}

// Prefix for a parameter in a stored procedure
XString 
SQLInfoAccess::GetKEYWORDParameterPrefix() const
{
  return "@";
}

// Get select part to add new record identity to a table
// Can be special column like 'OID' or a sequence select
XString 
SQLInfoAccess::GetKEYWORDIdentityString(XString& p_tablename,XString /*p_postfix*/ /*= "_seq"*/) const
{
  return "IDENT_CURRENT('" + p_tablename + "') + " + "IDENT_INCR('" + p_tablename + "')";
}

// Gets the UPPER function
XString 
SQLInfoAccess::GetKEYWORDUpper(XString& p_expression) const
{
  return "{fn UCASE(" + p_expression + ")}";
}

// Gets the construction for 1 minute ago
XString 
SQLInfoAccess::GetKEYWORDInterval1MinuteAgo() const
{
  // Not supported by MS-Access
  return "ERROR";
}

// Gets the Not-NULL-Value statement of the database
XString 
SQLInfoAccess::GetKEYWORDStatementNVL(XString& p_test,XString& p_isnull) const
{
  return XString("IIF(ISNULL(") + p_test + ")," + p_isnull + "," + p_test + ")";
}

// Gets the RDBMS definition of the datatype
XString 
SQLInfoAccess::GetKEYWORDDataType(MetaColumn* p_column)
{
  return p_column->m_typename;
}

// Gets the USER (current-user) keyword function
XString 
SQLInfoAccess::GetKEYWORDCurrentUser() const
{
  return "CurrentUser()";
}

// Connects to a default schema in the database/instance
XString 
SQLInfoAccess::GetSQLDefaultSchema(XString /*p_user*/,XString /*p_schema*/) const
{
  return "";
}

// Gets the construction for inline generating a key within an INSERT statement
XString 
SQLInfoAccess::GetSQLNewSerial(XString /*p_table*/, XString /*p_sequence*/) const
{
  // Insert a zero in an IDENTITY column
  return "0";
}

// Gets the construction / select for generating a new serial identity
XString
SQLInfoAccess::GetSQLGenerateSerial(XString p_table) const
{
  // Is generated by the IDENTITY system
  return "";
}

// Gets the construction / select for the resulting effective generated serial
XString
SQLInfoAccess::GetSQLEffectiveSerial(XString p_identity) const
{
  return "SELECT @@IDENTITY";
}

// Gets the sub-transaction commands
XString
SQLInfoAccess::GetSQLStartSubTransaction(XString p_savepointName) const
{
  // Does not know how to do sub-transactions
  return "";
}

XString
SQLInfoAccess::GetSQLCommitSubTransaction(XString p_savepointName) const
{
  // Does not know how to do sub-transactions
  return "";
}

XString
SQLInfoAccess::GetSQLRollbackSubTransaction(XString p_savepointName) const
{
  // Does not know how to do sub-transactions
  return "";
}

// FROM-Part for a query to select only 1 (one) record / or empty!
XString 
SQLInfoAccess::GetSQLFromDualClause() const
{
  return "";
}

// Get SQL to lock  a table 
XString 
SQLInfoAccess::GetSQLLockTable(XString /*p_schema*/, XString p_tablename, bool p_exclusive,int /*p_waittime*/) const
{
  XString query = "SELECT * FROM " + p_tablename + " WITH ";
  query += p_exclusive ? "(TABLOCKX)" : "(TABLOCK)";
  return query;
}

// Get query to optimize the table statistics
XString 
SQLInfoAccess::GetSQLOptimizeTable(XString p_schema, XString p_tablename) const
{
  return "";
}

// Transform query to select top <n> rows
XString 
SQLInfoAccess::GetSQLTopNRows(XString p_sql,int /*p_top*/,int /*p_skip = 0*/) const
{
  // Does nothing for now
  return p_sql;
}

// Query to perform a keep alive ping
XString
SQLInfoAccess::GetPing() const
{
  // Not implemented yet
  return "";
}

//////////////////////////////////////////////////////////////////////////
//
// SQL STRINGS
//
//////////////////////////////////////////////////////////////////////////

// Makes a SQL string from a given string, with all the right quotes
XString
SQLInfoAccess::GetSQLString(const XString& p_string) const
{
  XString s = p_string;
  s.Replace("'","''");
  XString kwoot = GetKEYWORDQuoteCharacter();
  return  kwoot + s + kwoot;
}

// Get date string in engine format
XString
SQLInfoAccess::GetSQLDateString(int p_year,int p_month,int p_day) const
{
  XString retval;
  retval.Format("{ d '%04d-%02d-%02d' }",p_year,p_month,p_day);
  return retval;
}

// Get time string in database engine format
XString
SQLInfoAccess::GetSQLTimeString(int p_hour,int p_minute,int p_second) const
{
  XString retval;
  retval.Format("{ t '%02d:%02d:%02d' }",p_hour,p_minute,p_second);
  return retval;
}

// Get date-time string in database engine format
XString
SQLInfoAccess::GetSQLDateTimeString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  XString retval;
  retval.Format("{ ts '%04d-%02d-%02d %02d:%02d:%02d' }",p_year,p_month,p_day,p_hour,p_minute,p_second);
  return retval;
}

// Get date-time bound parameter string in database format
XString
SQLInfoAccess::GetSQLDateTimeBoundString() const
{
  return "{ts ?}";
}

// Stripped data for the parameter binding
XString
SQLInfoAccess::GetSQLDateTimeStrippedString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  XString retval;
  retval.Format("%04d-%02d-%02d %02d:%02d:%02d",p_year,p_month,p_day,p_hour,p_minute,p_second);
  return retval;
}

// Makes an catalog identifier string (possibly quoted on both sides)
XString 
SQLInfoAccess::GetSQLDDLIdentifier(XString p_identifier) const
{
  return p_identifier;
}

// Changes to parameters before binding to an ODBC HSTMT handle
void 
SQLInfoAccess::DoBindParameterFixup(SQLSMALLINT& /*p_sqlDatatype*/,SQLULEN& /*p_columnSize*/,SQLSMALLINT& /*p_scale*/,SQLLEN& /*p_bufferSize*/,SQLLEN* /*p_indicator*/) const
{
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

// Meta info about meta types
// Standard ODBC functions are good enough
XString
SQLInfoAccess::GetCATALOGMetaTypes(int p_type) const
{
  UNREFERENCED_PARAMETER(p_type);
  return "";
}

// Get SQL to check if a table already exists in the database
XString
SQLInfoAccess::GetCATALOGTableExists(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  // MS-Access cannot do this
  return XString();
}

XString 
SQLInfoAccess::GetCATALOGTablesList(XString& /*p_schema*/,XString& /*p_pattern*/) const
{
  // MS-Access cannot do this
  return XString();
}

XString 
SQLInfoAccess::GetCATALOGTableAttributes(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  // MS-Access cannot do this
  return XString();
}

XString 
SQLInfoAccess::GetCATALOGTableSynonyms(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  // MS-Access cannot do this
  return XString();
}

XString 
SQLInfoAccess::GetCATALOGTableCatalog(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  // MS-Access cannot do this
  return XString();
}

XString 
SQLInfoAccess::GetCATALOGTableCreate(MetaTable& p_table,MetaColumn& /*p_column*/) const
{
  XString sql = "CREATE TABLE ";
  sql += p_table.m_table;
  return sql;
}

XString 
SQLInfoAccess::GetCATALOGTableCreatePostfix(MetaTable& /*p_table*/,MetaColumn& /*p_column*/) const
{
  return XString();
}

// Rename a database table 
XString
SQLInfoAccess::GetCATALOGTableRename(XString /*p_schema*/,XString p_tablename,XString p_newname) const
{
  // Beware: No 'TABLE' in the statement
  XString sql("RENAME " + p_tablename + " to " + p_newname);
  return sql;
}

XString 
SQLInfoAccess::GetCATALOGTableDrop(XString /*p_schema*/,XString p_tablename,bool /*p_ifExist = false*/,bool /*p_restrict = false*/,bool /*p_cascade = false*/) const
{
  return XString("DROP TABLE ") + p_tablename;
}

//////////////////////////////////////////////////////////////////////////
// ALL TEMPORARY TABLE FUNCTIONS

XString 
SQLInfoAccess::GetCATALOGTemptableCreate(XString /*p_schema*/,XString p_tablename,XString p_select) const
{
  return "CREATE TABLE #" + p_tablename + "\nAS " + p_select;
}

XString 
SQLInfoAccess::GetCATALOGTemptableIntoTemp(XString /*p_schema*/,XString p_tablename,XString p_select) const
{
  return "INSERT INTO #" + p_tablename + "\n" + p_select;
}
XString 
SQLInfoAccess::GetCATALOGTemptableDrop(XString /*p_schema*/,XString p_tablename) const
{
  return "DELETE FROM #"    + p_tablename + "\n"
         "<@>\n"
         "DROP TABLE #"     + p_tablename;
}

//////////////////////////////////////////////////////////////////////////
// ALL COLUMN FUNCTIONS

XString 
SQLInfoAccess::GetCATALOGColumnExists(XString /*p_schema*/,XString /*p_tablename*/,XString /*p_columnname*/) const
{
  // MS-Access cannot do this
  return "";
}

XString 
SQLInfoAccess::GetCATALOGColumnList(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  // MS-Access cannot do this
  return "";
}

XString 
SQLInfoAccess::GetCATALOGColumnAttributes(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_columnname*/) const
{
  // MS-Access cannot do this
  return "";
}

XString 
SQLInfoAccess::GetCATALOGColumnCreate(MetaColumn& p_column) const
{
  XString sql = "ALTER TABLE "  + p_column.m_table  + "\n"
                "  ADD COLUMN " + p_column.m_column + " " + p_column.m_typename;
  if(p_column.m_columnSize)
  {
    sql.AppendFormat("(%d",p_column.m_columnSize);
    if(p_column.m_decimalDigits)
    {
      sql.AppendFormat(",%d",p_column.m_decimalDigits);
    }
    sql += ")";
  }
  if(!p_column.m_nullable)
  {
    sql += " NOT NULL";
  }
  return sql;
}

XString 
SQLInfoAccess::GetCATALOGColumnAlter(MetaColumn& p_column) const
{
  XString sql = "ALTER TABLE  " + p_column.m_table + "\n"
                "ALTER COLUMN " + p_column.m_column + " " + p_column.m_typename;
  p_column.GetPrecisionAndScale(sql);
  p_column.GetNullable(sql);
  // m_position not used
  // m_length   not used
  // m_remarks  not used
  return sql;
}

XString 
SQLInfoAccess::GetCATALOGColumnRename(XString p_schema,XString p_tablename,XString p_columnname,XString p_newname,XString p_datatype) const
{
  XString sqlCode;
  sqlCode  = "ALTER TABLE " + p_tablename + "\n"
             "  ADD " + p_newname + " " + p_datatype + ";\n";
  sqlCode += "UPDATE " + p_tablename + "\n"
             "   SET " + p_newname   + " = " + p_columnname + ";\n";
  sqlCode += "ALTER TABLE " + p_tablename + "\n"
             " DROP COLUMN " + p_columnname + ";";
  return sqlCode;
}

XString 
SQLInfoAccess::GetCATALOGColumnDrop(XString p_schema,XString p_tablename,XString p_columnname) const
{
  XString sql("ALTER TABLE " + p_tablename + "\n"
              " DROP COLUMN " + p_columnname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL INDICES FUNCTIONS

// All index functions
XString 
SQLInfoAccess::GetCATALOGIndexExists(XString p_schema,XString p_tablename,XString p_indexname) const
{
  // Cannot query MS-Access for the index configuration
  return "";
}

XString 
SQLInfoAccess::GetCATALOGIndexList(XString& /*p_schema*/,XString& /*p_tablename*/)   const
{
  // Cannot query MS-Access for the index configuration
  return "";
}

XString 
SQLInfoAccess::GetCATALOGIndexAttributes(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_indexname*/)  const
{
  // Cannot query MS-Access for the index configuration
  return "";
}

XString
SQLInfoAccess::GetCATALOGIndexCreate(MIndicesMap& p_indices,bool /*p_duplicateNulls /*= false*/) const
{
  // Get SQL to create an index for a table
  // CREATE [UNIQUE] [ASC|DESC] INDEX [<schema>.]indexname ON [<schema>.]tablename(column [,...]);
  XString query;
  for(auto& index : p_indices)
  {
    if(index.m_position == 1)
    {
      // New index
      query = "CREATE ";
      if(index.m_nonunique == false)
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

XString 
SQLInfoAccess::GetCATALOGIndexDrop(XString /*p_schema*/,XString /*p_tablename*/,XString p_indexname) const
{
  XString sql = "DROP INDEX " + p_indexname;
  return sql;
}

// Get extra filter expression for an index column
XString
SQLInfoAccess::GetCATALOGIndexFilter(MetaIndex& /*p_index*/) const
{
  return "";
}

//////////////////////////////////////////////////////////////////////////
// ALL PRIMARY KEY FUNCTIONS

XString 
SQLInfoAccess::GetCATALOGPrimaryExists(XString p_schema,XString p_tablename) const
{
  // MS Access cannot get this info
  return "";
}

XString 
SQLInfoAccess::GetCATALOGPrimaryAttributes(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  // MS Access cannot get this info
  return "";
}

XString 
SQLInfoAccess::GetCATALOGPrimaryCreate(MPrimaryMap& p_primaries) const
{
  XString query("ALTER TABLE ");

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

XString 
SQLInfoAccess::GetCATALOGPrimaryDrop(XString /*p_schema*/,XString p_tablename,XString p_constraintname) const
{
  XString sql("ALTER TABLE " + p_tablename + "\n"
              " DROP CONSTRAINT " + p_constraintname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL FOREIGN KEY FUNCTIONS

XString 
SQLInfoAccess::GetCATALOGForeignExists(XString /*p_schema*/,XString /*p_tablename*/,XString /*p_constraintname*/) const
{
  // MS-Access cannot get this information, Use ODBC functions
  return "";
}

XString 
SQLInfoAccess::GetCATALOGForeignList(XString& /*p_schema*/,XString& /*p_tablename*/,int /*p_maxColumns*/ /*=SQLINFO_MAX_COLUMNS*/) const
{ 
  // MS-Access cannot get this information, Use ODBC functions
  return "";
}

XString 
SQLInfoAccess::GetCATALOGForeignAttributes(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_constraintname*/,bool/* p_referenced = false*/,int /*p_maxColumns*/ /*=SQLINFO_MAX_COLUMNS*/) const
{
  // MS-Access cannot get this information, Use ODBC functions
  return "";
}

XString 
SQLInfoAccess::GetCATALOGForeignCreate(MForeignMap& p_foreigns) const
{
 // Get first record
  MetaForeign& foreign = p_foreigns.front();

  // Construct the correct tablename
  XString table(foreign.m_fkTableName);
  XString primary(foreign.m_pkTableName);
  if(!foreign.m_fkSchemaName.IsEmpty())
  {
    table = foreign.m_fkSchemaName + "." + table;
  }
  if(!foreign.m_pkSchemaName.IsEmpty())
  {
    primary = foreign.m_pkSchemaName + "." + primary;
  }

  // The base foreign key command
  XString query = "ALTER TABLE " + table + "\n"
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

XString 
SQLInfoAccess::GetCATALOGForeignAlter(MForeignMap& /*p_original*/, MForeignMap& /*p_requested*/) const
{
	// MS-Access cannot alter a foreign-key constraint.
	// You must drop and then re-create your foreign key constraint
	// So return an empty string to signal this!
	return "";
}

XString 
SQLInfoAccess::GetCATALOGForeignDrop(XString /*p_schema*/,XString p_tablename,XString p_constraintname) const
{
  XString sql("ALTER TABLE " + p_tablename + "\n"
              " DROP CONSTRAINT " + p_constraintname);
  return sql;
}

//////////////////////////
// All default constraints
XString 
SQLInfoAccess::GetCATALOGDefaultExists(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_column*/) const
{
  return "";
}

XString 
SQLInfoAccess::GetCATALOGDefaultList(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  return "";
}

XString 
SQLInfoAccess::GetCATALOGDefaultAttributes(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_column*/) const
{
  return "";
}

XString 
SQLInfoAccess::GetCATALOGDefaultCreate(XString /*p_schema*/,XString /*p_tablename*/,XString /*p_constraint*/,XString /*p_column*/,XString /*p_code*/) const
{
  return "";
}

XString 
SQLInfoAccess::GetCATALOGDefaultDrop(XString /*p_schema*/,XString /*p_tablename*/,XString /*p_constraint*/) const
{
  return "";
}

/////////////////////////
// All check constraints

XString
SQLInfoAccess::GetCATALOGCheckExists(XString  /*p_schema*/,XString  /*p_tablename*/,XString  /*p_constraint*/) const
{
  return "";
}

XString 
SQLInfoAccess::GetCATALOGCheckList(XString  /*p_schema*/,XString  /*p_tablename*/) const
{
  return "";
}

XString 
SQLInfoAccess::GetCATALOGCheckAttributes(XString  /*p_schema*/,XString  /*p_tablename*/,XString  /*p_constraint*/) const
{
  return "";
}

XString
SQLInfoAccess::GetCATALOGCheckCreate(XString  /*p_schema*/,XString  /*p_tablename*/,XString  /*p_constraint*/,XString /*p_condition*/) const
{
  return "";
}

XString
SQLInfoAccess::GetCATALOGCheckDrop(XString  /*p_schema*/,XString  /*p_tablename*/,XString  /*p_constraint*/) const
{
  return "";
}

//////////////////////////////////////////////////////////////////////////
// ALL TRIGGER FUNCTIONS

XString 
SQLInfoAccess::GetCATALOGTriggerExists(XString p_schema, XString p_tablename, XString p_triggername) const
{
  // No triggers in MS-Access
  return "";
}

XString 
SQLInfoAccess::GetCATALOGTriggerList(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  // No triggers in MS-Access
  return "";
}

XString 
SQLInfoAccess::GetCATALOGTriggerAttributes(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_triggername*/) const
{
  // No triggers in MS-Access
  return "";
}

XString 
SQLInfoAccess::GetCATALOGTriggerCreate(MetaTrigger& /*p_trigger*/) const
{
  // Cannot create a trigger. Does not exist in MS-Access
  return "";
}

XString 
SQLInfoAccess::GetCATALOGTriggerDrop(XString p_schema, XString p_tablename, XString p_triggername) const
{
  // Cannot drop a trigger. Does not exist in MS-Access
  return "";
}

//////////////////////////////////////////////////////////////////////////
// ALL SEQUENCE FUNCTIONS

XString 
SQLInfoAccess::GetCATALOGSequenceExists(XString /*p_schema*/,XString /*p_sequence*/) const
{
  // MS-Access does not have sequences
  return "";
}

XString 
SQLInfoAccess::GetCATALOGSequenceList(XString& /*p_schema*/,XString& /*p_pattern*/) const
{
  // MS-Access does not have sequences
  return "";
}

XString 
SQLInfoAccess::GetCATALOGSequenceAttributes(XString& /*p_schema*/,XString& /*p_sequence*/) const
{
  // MS-Access does not have sequences
  return "";
}

XString 
SQLInfoAccess::GetCATALOGSequenceCreate(MetaSequence& /*p_sequence*/) const
{
  // MS-Access does not have sequences
  return "";
}

XString 
SQLInfoAccess::GetCATALOGSequenceDrop(XString /*p_schema*/, XString /*p_sequence*/) const
{
  // MS-Access does not have sequences
  return "";
}

//////////////////////////////////////////////////////////////////////////
// ALL VIEW FUNCTIONS

XString 
SQLInfoAccess::GetCATALOGViewExists(XString& /*p_schema*/,XString& /*p_viewname*/) const
{
  // Cannot query this, Use ODBC functions
  return "";
}

XString 
SQLInfoAccess::GetCATALOGViewList(XString& /*p_schema*/,XString& /*p_pattern*/) const
{
  // Cannot query this, Use ODBC functions
  return "";
}

XString 
SQLInfoAccess::GetCATALOGViewAttributes(XString& /*p_schema*/,XString& /*p_viewname*/) const
{
  // Cannot query this, Use ODBC functions
  return "";
}

XString 
SQLInfoAccess::GetCATALOGViewText(XString& /*p_schema*/,XString& /*p_viewname*/) const
{
  // Cannot query this, Use ODBC functions
  return "";
}

XString
SQLInfoAccess::GetCATALOGViewCreate(XString /*p_schema*/,XString p_viewname,XString p_contents,bool /*p_ifexists = true*/) const
{
  return "CREATE VIEW " + p_viewname + "\n" + p_contents;
}

XString 
SQLInfoAccess::GetCATALOGViewRename(XString p_schema,XString p_viewname,XString p_newname)    const
{
  return "";
}

XString 
SQLInfoAccess::GetCATALOGViewDrop(XString /*p_schema*/,XString p_viewname,XString& p_precursor) const
{
  p_precursor.Empty();
  return "DROP VIEW " + p_viewname;
}

// All Privilege functions
XString 
SQLInfoAccess::GetCATALOGTablePrivileges(XString& /*p_schema*/, XString& /*p_tablename*/) const
{
  return "";
}

XString 
SQLInfoAccess::GetCATALOGColumnPrivileges(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_columnname*/) const
{
  return "";
}

XString 
SQLInfoAccess::GetCATALOGSequencePrivilege(XString& /*p_schema*/,XString& /*p_sequence*/) const
{
  return "";
}

XString 
SQLInfoAccess::GetCATALOGGrantPrivilege(XString /*p_schema*/,XString p_objectname,XString p_privilege,XString p_grantee,bool p_grantable)
{
  XString sql;
  sql.Format("GRANT %s ON %s TO %s",p_privilege.GetString(),p_objectname.GetString(),p_grantee.GetString());
  if(p_grantable)
  {
    sql += " WITH GRANT OPTION";
  }
  return sql;
}

XString 
SQLInfoAccess::GetCATALOGRevokePrivilege(XString p_schema,XString p_objectname,XString p_privilege,XString p_grantee)
{
  XString sql;
  sql.Format("REVOKE %s ON %s FROM %s",p_privilege.GetString(),p_objectname.GetString(),p_grantee.GetString());
  return sql;
}

// All Synonym functions
XString 
SQLInfoAccess::GetCATALOGSynonymList(XString& /*p_schema*/,XString& /*p_pattern*/) const
{
  // Not implemented yet
  return "";
}

XString 
SQLInfoAccess::GetCATALOGSynonymAttributes(XString& /*p_schema*/,XString& /*p_synonym*/) const
{
  // Not implemented yet
  return "";
}

XString 
SQLInfoAccess::GetCATALOGSynonymCreate(XString& /*p_schema*/,XString& /*p_synonym*/,XString /*p_forObject*/,bool /*p_private = true*/) const
{
  // Not implemented yet
  return "";
}

XString 
SQLInfoAccess::GetCATALOGSynonymDrop(XString& /*p_schema*/,XString& /*p_synonym*/,bool /*p_private = true*/) const
{
  // Not implemented yet
  return "";
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

XString 
SQLInfoAccess::GetPSMProcedureExists(XString p_schema,XString p_procedure) const
{
  // MS-Access does not support PSM
  return "";
}
  
XString 
SQLInfoAccess::GetPSMProcedureList(XString& /*p_schema*/) const
{
  // MS-Access does not support PSM
  return "";
}
  
XString 
SQLInfoAccess::GetPSMProcedureAttributes(XString& /*p_schema*/,XString& /*p_procedure*/) const
{
  // MS-Access does not support PSM
  return "";
}
  
XString 
SQLInfoAccess::GetPSMProcedureSourcecode(XString p_schema, XString p_procedure) const
{
  // MS-Access does not support PSM
  return "";
}
  
XString 
SQLInfoAccess::GetPSMProcedureCreate(MetaProcedure& /*p_procedure*/) const
{
  // MS-Access does not support PSM
  return "";
}
  
XString 
SQLInfoAccess::GetPSMProcedureDrop(XString p_schema,XString p_procedure,bool /*p_function /*=false*/) const
{
  // MS-Access does not support PSM
  return "";
}

XString 
SQLInfoAccess::GetPSMProcedureErrors(XString p_schema,XString p_procedure) const
{
  // MS-Access does not support PSM
  return "";
}

XString 
SQLInfoAccess::GetPSMProcedurePrivilege(XString& /*p_schema*/,XString& /*p_procedure*/) const
{
  return "";
}

// And it's parameters
XString 
SQLInfoAccess::GetPSMProcedureParameters(XString& /*p_schema*/,XString& /*p_procedure*/) const
{
  // MS-Access does not support PSM
  return "";
}

//////////////////////////////////////////////////////////////////////////
//
// ALL PSM LANGUAGE ELEMENTS
//
//////////////////////////////////////////////////////////////////////////

XString
SQLInfoAccess::GetPSMDeclaration(bool    /*p_first*/
                                ,XString /*p_variable*/
                                ,int     /*p_datatype*/
                                ,int     /*p_precision = 0*/
                                ,int     /*p_scale     = 0*/
                                ,XString /*p_default   = ""*/
                                ,XString /*p_domain    = ""*/
                                ,XString /*p_asColumn  = ""*/) const
{
  // MS-Access does not use PSM, but Visual Basic
  return "";
}
XString 
SQLInfoAccess::GetPSMAssignment(XString /*p_variable*/,XString /*p_statement = ""*/) const
{
  // MS-Access does not use PSM, but Visual Basic
  return "";
}

XString 
SQLInfoAccess::GetPSMIF(XString /*p_condition*/) const
{
  // MS-Access does not use PSM, but Visual Basic
  return "";
}

XString 
SQLInfoAccess::GetPSMIFElse() const
{
  // MS-Access does not use PSM, but Visual Basic
  return "";
}

XString 
SQLInfoAccess::GetPSMIFEnd() const
{
  // MS-Access does not use PSM, but Visual Basic
  return "";
}

XString 
SQLInfoAccess::GetPSMWhile(XString /*p_condition*/) const
{
  // MS-Access does not use PSM, but Visual Basic
  return "";
}

XString 
SQLInfoAccess::GetPSMWhileEnd() const
{
  // MS-Access does not use PSM, but Visual Basic
  return "";
}

XString 
SQLInfoAccess::GetPSMLOOP() const
{
  // MS-Access does not use PSM, but Visual Basic
  return "";
}

XString 
SQLInfoAccess::GetPSMLOOPEnd() const
{
  // MS-Access does not use PSM, but Visual Basic
  return "";
}

XString 
SQLInfoAccess::GetPSMBREAK() const
{
  // MS-Access does not use PSM, but Visual Basic
  return "";
}

XString 
SQLInfoAccess::GetPSMRETURN(XString /*p_statement*/ /*= ""*/) const
{
  // MS-Access does not use PSM, but Visual Basic
  return "";
}

XString 
SQLInfoAccess::GetPSMExecute(XString /*p_procedure*/,MParameterMap& /*p_parameters*/) const
{
  // MS-Access does not use PSM, but Visual Basic
  return "";
}

//////////////////////////////////////////////////////////////////////////
// The CURSOR

XString 
SQLInfoAccess::GetPSMCursorDeclaration(XString p_cursorname,XString p_select) const
{
  // MS-Access does not use PSM, but Visual Basic
  return "";
}

XString 
SQLInfoAccess::GetPSMCursorFetch(XString /*p_cursorname*/,std::vector<XString>& /*p_columnnames*/,std::vector<XString>& /*p_variablenames*/) const
{
  // MS-Access does not use PSM, but Visual Basic
  return "";
}

//////////////////////////////////////////////////////////////////////////
// PSM Exceptions

XString 
SQLInfoAccess::GetPSMExceptionCatchNoData() const
{
  // MS-Access does not use PSM, but Visual Basic
  return "";
}

XString 
SQLInfoAccess::GetPSMExceptionCatch(XString p_sqlState) const
{
  // MS-Access does not use PSM, but Visual Basic
  return "";
}

XString 
SQLInfoAccess::GetPSMExceptionRaise(XString p_sqlState) const
{
  // MS-Access does not use PSM, but Visual Basic
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

XString 
SQLInfoAccess::GetSESSIONMyself() const
{
  // MS-Access has no info about processes
  return "";
}

XString 
SQLInfoAccess::GetSESSIONExists(XString p_sessionID) const
{
  // MS-Access has no info about processes
  return "";
}

XString 
SQLInfoAccess::GetSESSIONList() const
{
  // MS-Access has no info about processes
  return "";
}

XString 
SQLInfoAccess::GetSESSIONAttributes(XString p_sessionID) const
{
  // MS-Access has no info about processes
  return "";
}

//////////////////////////////////////////////////////////////////////////
// Transactions

XString
SQLInfoAccess::GetSESSIONConstraintsDeferred() const
{
  // MS-Access cannot defer constraints
  return "";
}

XString 
SQLInfoAccess::GetSESSIONConstraintsImmediate() const
{
  // MS-Access constraints are always active
  return "";
}

//////////////////////////////////////////////////////////////////////////
//
// Call FUNCTION/PROCEDURE from within program
// As a RDBMS dependent extension of "DoSQLCall" of the SQLQuery object
//
//////////////////////////////////////////////////////////////////////////

// Calling a stored function or procedure if the RDBMS does not support ODBC call escapes
SQLVariant*
SQLInfoAccess::DoSQLCall(SQLQuery* /*p_query*/,XString& /*p_schema*/,XString& /*p_procedure*/)
{
  return nullptr;
}

// Calling a stored function with named parameters, returning a value
SQLVariant*
SQLInfoAccess::DoSQLCallNamedParameters(SQLQuery* /*p_query*/,XString& /*p_schema*/,XString& /*p_procedure*/)
{
  return nullptr;
}

// End of namespace
}
