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

//////////////////////////////////////////////////////////////////////////
//
// GENERALS (Strings & Booleans) 
//
//////////////////////////////////////////////////////////////////////////

// Get the database type
// DatabaseType GetDatabaseType() const;
DatabaseType
SQLInfoSQLServer::GetRDBMSDatabaseType() const
{
  return RDBMS_SQLSERVER;
}

// The name of the database vendor
CString
SQLInfoSQLServer::GetRDBMSVendorName() const
{
  // The name of the database vendor
  return "Microsoft";
}

// Get the physical database name
CString
SQLInfoSQLServer::GetRDBMSPhysicalDatabaseName() const
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
SQLInfoSQLServer::GetRDBMSIsCatalogUpper() const
{
  return false;
}

// System catalog supports full ISO schemas (same tables per schema)
bool
SQLInfoSQLServer::GetRDBMSUnderstandsSchemas() const
{
  return true;
}

// Supports database/ODBCdriver comments in SQL
bool
SQLInfoSQLServer::GetRDBMSSupportsComments() const
{
  return true;
}

// Database can defer constraints until the end of a transaction
bool
SQLInfoSQLServer::GetRDBMSSupportsDeferredConstraints() const
{
  // SET CONSTRAINTS DEFERRED is supported
  return true;
}

// Database has ORDER BY with an expression, e.g. ORDER BY UPPER(columnname)
bool
SQLInfoSQLServer::GetRDBMSSupportsOrderByExpression() const
{
  return true;
}

// Supports the ODBC escape sequence {[?=] CALL procedure (?,?,?)}
bool
SQLInfoSQLServer::GetRDBMSSupportsODBCCallEscapes() const
{
  return true;
}

// If the database does not support the datatype TIME, it can be implemented as a DECIMAL
bool
SQLInfoSQLServer::GetRDBMSSupportsDatatypeTime() const
{
  // Time can be implemented as TIME
  return true;
}

// If the database does not support the datatype INTERVAL, it can be implemented as a DECIMAL
bool
SQLInfoSQLServer::GetRDBMSSupportsDatatypeInterval() const
{
  // Interval not supported, can be implemented as DECIMALS
  return false;
}

// Gets the maximum length of an SQL statement
unsigned long
SQLInfoSQLServer::GetRDBMSMaxStatementLength() const
{
  // No Limit
  return 0;
}

// KEYWORDS

// Keyword for the current date and time
CString
SQLInfoSQLServer::GetKEYWORDCurrentTimestamp() const
{
  return "GETDATE()";
}

// String for the current date
CString
SQLInfoSQLServer::GetKEYWORDCurrentDate() const
{
  return "GETDATE()";
}

// Get the concatenation operator
CString
SQLInfoSQLServer::GetKEYWORDConcatanationOperator() const
{
  return "+";
}

// Get quote character for strings
CString
SQLInfoSQLServer::GetKEYWORDQuoteCharacter() const
{
  return "\'";
}

// Get default NULL for parameter list input
CString
SQLInfoSQLServer::GetKEYWORDParameterDefaultNULL() const
{
  return "= NULL";
}

// Parameter is for INPUT and OUTPUT in parameter list
CString
SQLInfoSQLServer::GetKEYWORDParameterINOUT() const
{
  return "OUTPUT";
}

// Parameter is for OUTPUT only in parameter list
CString
SQLInfoSQLServer::GetKEYWORDParameterOUT() const
{
  return "OUTPUT";
}

// Get datatype of the IDENTITY primary key in a Network database
CString
SQLInfoSQLServer::GetKEYWORDNetworkPrimaryKeyType() const
{
  return "INTEGER IDENTITY(1,1)";
}

// Get datatype for timestamp (year to second)
CString
SQLInfoSQLServer::GetKEYWORDTypeTimestamp() const
{
  return "DATETIME";
}

// Prefix for a parameter in a stored procedure
CString
SQLInfoSQLServer::GetKEYWORDParameterPrefix() const
{
  return "@";
}

// Get select part to add new record identity to a table
// Can be special column like 'OID' or a sequence select
CString
SQLInfoSQLServer::GetKEYWORDIdentityString(CString& p_tablename,CString /*p_postfix*/ /*= "_seq"*/) const
{
  return "IDENT_CURRENT('" + p_tablename + "') + " + "IDENT_INCR('" + p_tablename + "')";
}

// Gets the UPPER function
CString
SQLInfoSQLServer::GetKEYWORDUpper(CString& p_expression) const
{
  return "UPPER(" + p_expression + ")";
}

// Gets the construction for 1 minute ago
CString
SQLInfoSQLServer::GetKEYWORDInterval1MinuteAgo() const
{
  return "dateadd(minute,-1,current_timestamp)";
}

// Gets the Not-NULL-Value statement of the database
CString
SQLInfoSQLServer::GetKEYWORDStatementNVL(CString& p_test,CString& p_isnull) const
{
  return CString("NVL(") + p_test + "," + p_isnull + ")";
}

// Code prefix for a select-into-temp
CString
SQLInfoSQLServer::GetSQLSelectIntoTempPrefix(CString p_tableName) const
{
  return "";
}

// Code suffix for after a select-into-temp
CString
SQLInfoSQLServer::GetSQLSelectIntoTempSuffix(CString p_tableName) const
{
  return "";
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

// Gets the sub transaction commands
CString
SQLInfoSQLServer::GetSQLStartSubTransaction(CString p_savepointName) const
{
  return CString("BEGIN TRANSACTION ") + p_savepointName;
}

CString
SQLInfoSQLServer::GetSQLCommitSubTransaction(CString p_savepointName) const
{
  return CString("ROLLBACK TRANSACTION ") + p_savepointName;
}

CString
SQLInfoSQLServer::GetSQLRollbackSubTransaction(CString p_savepointName) const
{
  return CString("ROLLBACK TRANSACTION ") + p_savepointName;
}

// FROM-Part for a query to select only 1 (one) record / or empty!
CString
SQLInfoSQLServer::GetSQLFromDualClause() const
{
  return "";
}

//////////////////////////////////////////////////////////////////////////
//
// CATALOG
//
//////////////////////////////////////////////////////////////////////////

// Get SQL to check if a table already exists in the database
CString
SQLInfoSQLServer::GetCATALOGTableExists(CString p_schema,CString p_tablename) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  CString query = "SELECT count(*)\n"
                  "  FROM dbo.sysobjects tab\n"
                  "      ,dbo.sysschemas sch\n"
                  " WHERE sch.name = '" + p_schema    + "'\n"
                  "   AND tab.name = '" + p_tablename + "'\n"
                  "   AND tab.schema_id = sch.schema_id\n";
  return query;
}

CString
SQLInfoSQLServer::GetCATALOGTablesList(CString p_schema,CString p_pattern) const
{
  p_schema.MakeLower();
  p_pattern.MakeLower();
  CString query = "SELECT count(*)\n"
                  "  FROM dbo.sysobjects tab\n"
                  "      ,dbo.sysschemas sch\n"
                  " WHERE sch.name      = '" + p_schema  + "'\n"
                  "   AND tab.name   like '" + p_pattern + "'\n"
                  "   AND tab.schema_id = sch.schema_id\n";
  return query;
}

bool
SQLInfoSQLServer::GetCATALOGTableAttributes(CString /*p_schema*/,CString /*p_tablename*/,MetaTable& /*p_table*/) const
{
  // Getting the temp table status
  return false;
}

CString
SQLInfoSQLServer::GetCATALOGTableCreate(MetaTable& /*p_table*/,MetaColumn& /*p_column*/) const
{
  return "";
}

CString
SQLInfoSQLServer::GetCATALOGTableRename(CString p_schema,CString p_tablename,CString p_newname) const
{
  // Beware: No 'TABLE' in the statement
  CString sql("RENAME " + p_schema + "." + p_tablename + " TO " + p_newname);
  return sql;
}

CString
SQLInfoSQLServer::GetCATALOGTableDrop(CString /*p_schema*/,CString p_tablename) const
{
  return "DROP TABLE " + p_tablename;
}

//////////////////////////////////////////////////////////////////////////
// ALL COLUMN FUNCTIONS

CString 
SQLInfoSQLServer::GetCATALOGColumnExists(CString p_schema,CString p_tablename,CString p_columnname) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  p_columnname.MakeLower();
  CString query = "SELECT count(*)\n"
                 "  FROM sys.sysobjects tab\n"
                 "      ,sys.schemas    sch\n"
                 "     , dbo.syscolumns att\n"
                 " WHERE tab.name  = '" + p_tablename  + "'\n"
                 "   AND sch.name  = '" + p_schema     + "'"
                 "   AND att.name  = '" + p_columnname + "'\n"
                 "   AND tab.id    = att.id\n"
                 "   AND tab.schema_id = sch.schema_id\n";
  return query;
}

CString 
SQLInfoSQLServer::GetCATALOGColumnList(CString p_schema,CString p_tablename) const
{
  CString sql = GetCATALOGColumnAttributes(p_schema,p_tablename,"");

  int pos = sql.ReverseFind('\n');
  sql = sql.Mid(pos + 1) + " ORDER BY col.colid";

  return sql;
}

CString 
SQLInfoSQLServer::GetCATALOGColumnAttributes(CString p_schema,CString p_tablename,CString p_columnname) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  p_columnname.MakeLower();

  CString sql = "SELECT col.name\n"         // 1 -> column name
                "      ,col.colid\n"        // 2 -> position
                "      ,typ.name\n"         // 3 -> typename
                "      ,col.length\n"       // 4 -> length
                "      ,col.isnullable\n"   // 5 -> Nullable
                "      ,col.prec\n"         // 6 -> Precision
                "      ,col.scale\n"        // 7 -> Scale
                "      ,replace(replace('#' + com.text + '#', '#(', ''), ')#', '')\n" // 8 -> Default
                "  FROM sys.sysobjects  obj\n"
                "      ,sys.schemas     sch\n"
                "      ,sys.syscolumns  col\n"
                "      ,sys.systypes    typ\n"
                "      ,sys.syscomments com\n"
                " WHERE obj.schema_id = sch.schema_id\n"
                "   AND obj.Id        = col.id\n"
                "   AND col.xtype     = typ.xtype\n"
                "   AND col.cdefault  = com.id\n"
                "   AND sch.name      = '" + p_schema     + "'\n"
                "   AND obj.name      = '" + p_tablename  + "'\n"
                "   AND col.name      = '" + p_columnname + "'";
  return sql;
}

CString 
SQLInfoSQLServer::GetCATALOGColumnCreate(MetaColumn& p_column) const
{
  CString sql = "ALTER TABLE  " + p_column.m_schema + "." + p_column.m_table  + "\n";
                "  ADD COLUMN " + p_column.m_column + " " + p_column.m_typename;
  p_column.GetPrecisionAndScale(sql);
  p_column.GetNullable(sql);
  p_column.GetDefault(sql);
  // m_position not used
  // m_length   not used
  // m_remarks  not used
  return sql;
}

CString 
SQLInfoSQLServer::GetCATALOGColumnAlter(MetaColumn& p_column) const
{
  CString sql = "ALTER TABLE  " + p_column.m_schema + "." + p_column.m_table  + "\n"
                "ALTER COLUMN " + p_column.m_column + " " + p_column.m_typename;
  p_column.GetPrecisionAndScale(sql);
  p_column.GetNullable(sql);
  p_column.GetDefault(sql);
  return sql;
}

CString 
SQLInfoSQLServer::GetCATALOGColumnRename(CString p_schema,CString p_tablename,CString p_columnname,CString p_newname,CString p_datatype) const
{
  CString sqlCode;
  sqlCode  = "ALTER TABLE " + p_schema + "." + p_tablename + "\n"
             "        ADD " + p_newname + " " + p_datatype + ";\n";
  sqlCode += "UPDATE " + p_schema + "." + p_tablename + "\n"
             "   SET " + p_newname   + " = " + p_columnname + ";\n";
  sqlCode += "ALTER TABLE " + p_schema + "." + p_tablename + "\n"
             " DROP COLUMN " + p_columnname + ";";
  return sqlCode;
}

CString 
SQLInfoSQLServer::GetCATALOGColumnDrop(CString p_schema,CString p_tablename,CString p_columnname) const
{
  CString sql("ALTER TABLE " + p_schema + "." + p_tablename + "\n"
              " DROP COLUMN " + p_columnname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL INDICES FUNCTIONS

// All index functions
CString
SQLInfoSQLServer::GetCATALOGIndexExists(CString p_schema,CString p_tablename,CString p_indexname) const
{
  return "";
}

CString
SQLInfoSQLServer::GetCATALOGIndexList(CString p_schema,CString p_tablename) const
{
  p_tablename.MakeLower();
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
                  " WHERE obj.name = '" + p_tablename + "'\n"
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

CString
SQLInfoSQLServer::GetCATALOGIndexAttributes(CString p_schema,CString p_tablename,CString p_indexname) const
{
  return "";
}

// Get SQL to create an index for a table
// CREATE [UNIQUE] INDEX indexname ON [<schema>.]tablename(column [ASC|DESC] [,...]);
// Beware: no schema name for indexname. Automatically in the table schema
CString
SQLInfoSQLServer::GetCATALOGIndexCreate(MIndicesMap& p_indices) const
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

CString
SQLInfoSQLServer::GetCATALOGIndexDrop(CString p_schema,CString /*p_tablename*/,CString p_indexname) const
{
  CString sql = "DROP INDEX " + p_schema + "." + p_indexname;
  return sql;
}

// Get extra filter expression for an index column
CString
SQLInfoSQLServer::GetCATALOGIndexFilter(MetaIndex& /*p_index*/) const
{
  return "";
}

//////////////////////////////////////////////////////////////////////////
// ALL PRIMARY KEY FUNCTIONS

CString
SQLInfoSQLServer::GetCATALOGPrimaryExists(CString p_schema,CString p_tablename) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();

  CString query = "SELECT COUNT(*)\n"
                  "  FROM dbo.sysobjects tab\n"
                  "      ,dbo.sysobjects con\n"
                  "      ,dbo.sysusers   use\n"
                  " WHERE tab.id    = con.parent_obj\n"
                  "   AND tab.uid   = use.uid\n"
                  "   AND con.xtype = 'PK'\n"
                  "   AND con.type  = 'K '"
                  "   AND use.name  = '" + p_schema    + "'\n"
                  "   AND tab.name  = '" + p_tablename + "'";
  return query;
}

CString
SQLInfoSQLServer::GetCATALOGPrimaryAttributes(CString p_schema,CString p_tablename) const
{
  // TO BE IMPLEMENTED
  return "";
}

CString
SQLInfoSQLServer::GetCATALOGPrimaryCreate(MPrimaryMap& p_primaries) const
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
SQLInfoSQLServer::GetCATALOGPrimaryDrop(CString p_schema,CString p_tablename,CString p_constraintname) const
{
  CString sql("ALTER TABLE " + p_schema + "." + p_tablename + "\n"
              " DROP CONSTRAINT " + p_constraintname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL FOREIGN KEY FUNCTIONS

CString
SQLInfoSQLServer::GetCATALOGForeignExists(CString p_schema,CString p_tablename,CString p_constraintname) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  p_constraintname.MakeLower();

  CString sql;
  sql.Format("SELECT COUNT(*)\n"
             "  FROM sys.foreign_keys fok\n"
             "      ,sys.schemas      sch\n"
             "      ,sys.tables       tab\n"
             " WHERE fok.type = 'F'\n"
             "   AND fok.parent_object_id = tab.object_id\n"
             "   AND tab.schema_id        = sch.schema_id\n"
             "   AND sch.name             = '" + p_schema + "'\n"
             "   AND tab.name             = '" + p_tablename + "'\n"
             "   AND fok.name             = '" + p_constraintname + "'"
            ,p_schema.GetString()
            ,p_tablename.GetString()
            ,p_constraintname.GetString());
  return sql;
}

CString
SQLInfoSQLServer::GetCATALOGForeignList(CString p_schema,CString p_tablename,int p_maxColumns /*=SQLINFO_MAX_COLUMNS*/) const
{
  return GetCATALOGForeignAttributes(p_schema,p_tablename,"",p_maxColumns);
}

CString
SQLInfoSQLServer::GetCATALOGForeignAttributes(CString p_schema,CString p_tablename,CString p_constraint,int /*p_maxColumns*/ /*=SQLINFO_MAX_COLUMNS*/) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  p_constraint.MakeLower();
  CString query = "SELECT db_name()         as primary_catalog_name\n"
                  "      ,sch.name          as primary_schema_name\n"
                  "      ,pri.name          as primary_table_name\n"
                  "      ,db_name()         as foreign_catalog_name\n"
                  "      ,sch.name          as foreign_schema_name\n"
                  "      ,tab.name          as foreign_table_name\n"
                  "      ,''                as primary_key_constraint\n"
                  "      ,fok.name          as foreign_key_constraint\n"
                  "      ,fkc.constraint_column_id      as key_sequence"
                  "      ,pky.name          as primary_key_column\n"
                  "      ,col.name          as foreign_key_column\n"
                  "      ,fok.update_referential_action as update_rule\n"
                  "      ,fok.delete_referential_action as delete_rule\n"
                  "      ,0                 as deferrable\n"
                  "      ,1                 as match_option\n"
                  "      ,0                 as initially_deferred\n"
                  "      ,fok.is_disabled   as disabled\n"
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
                  "   AND fkc.referenced_column_id = pky.column_id\n";
  if(!p_schema.IsEmpty())
  {
    query += "   AND sch.name = '" + p_schema + "'\n";
  }
  if(!p_tablename.IsEmpty())
  {
    query += "   AND tab.name = '" + p_tablename + "'\n";
  }
  if(!p_constraint.IsEmpty())
  {
    query += "   AND fok.name = '" + p_constraint + "'\n";
  }

  // Order upto column number
  query += " ORDER BY 1,2,3,4,5,6,7,8,9";

  return query;
}

CString
SQLInfoSQLServer::GetCATALOGForeignCreate(MForeignMap& p_foreigns) const
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
    case SQL_CASCADE :    query += "\n      ON UPDATE CASCADE";     break;
    case SQL_SET_NULL:    query += "\n      ON UPDATE SET NULL";    break;
    case SQL_SET_DEFAULT: query += "\n      ON UPDATE SET DEFAULT"; break;
    case SQL_NO_ACTION:   query += "\n      ON UPDATE NO ACTION";   break;
    case SQL_RESTRICT:    // Fall through
    default:              // In essence: ON UPDATE RESTRICT, but that's already the default
                          break;
  }
  switch(foreign.m_deleteRule)
  {
    case SQL_CASCADE:     query += "\n      ON DELETE CASCADE";     break;
    case SQL_SET_NULL:    query += "\n      ON DELETE SET NULL";    break;
    case SQL_SET_DEFAULT: query += "\n      ON DELETE SET DEFAULT"; break;
    case SQL_NO_ACTION:   query += "\n      ON DELETE NO ACTION";   break;
    case SQL_RESTRICT:    // Fall through
    default:              // In essence: ON DELETE RESTRICT, but that's already the default
                          break;
  }
  return query;
}

CString
SQLInfoSQLServer::GetCATALOGForeignAlter(MForeignMap& /*p_original*/, MForeignMap& /*p_requested*/) const
{
  // MS-SQL Server cannot alter a foreign-key constraint.
  // You must drop and then re-create your foreign key constraint
  // So return an empty string to signal this!

  // THIS IS POSSIBLE!
  // ALTER TABLE schema.table { NOCHECK | CHECK } CONSTRAINT <constraintname>

  return "";
}

CString
SQLInfoSQLServer::GetCATALOGForeignDrop(CString p_schema,CString p_tablename,CString p_constraintname) const
{
  CString sql("ALTER TABLE " + p_schema + "." + p_tablename + "\n"
              " DROP CONSTRAINT " + p_constraintname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL TRIGGER FUNCTIONS

CString
SQLInfoSQLServer::GetCATALOGTriggerExists(CString p_schema, CString p_tablename, CString p_triggername) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  p_triggername.MakeLower();

  CString sql;
  sql.Format("SELECT COUNT(*)\n"
             "  FROM sys.triggers trg\n"
             "      ,sys.objects  tab\n"
             "      ,sys.schemas  sch\n"
             " WHERE sch.name      = '%s'\n"
             "   AND tab.name      = '%s'\n"
             "   AND trg.name      = '%s'\n"
             "   AND tab.object_id = trg.parent_id\n"
             "   AND tab.schema_id = sch.schema_id"
            ,p_schema.GetString()
            ,p_tablename.GetString()
            ,p_triggername.GetString());
  return sql;
}

CString
SQLInfoSQLServer::GetCATALOGTriggerList(CString p_schema, CString p_tablename) const
{
  return GetCATALOGTriggerAttributes(p_schema,p_tablename,"");
}

CString
SQLInfoSQLServer::GetCATALOGTriggerAttributes(CString p_schema, CString p_tablename, CString p_triggername) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  p_triggername.MakeLower();

  CString sql;
  sql.Format("SELECT ''       AS catalog_name\n"
             "      ,sch.name AS schema_name\n"
             "      ,tab.name AS table_name\n"
             "      ,trg.name AS trigger_name\n"
             "      ,trg.name + ' ON TABLE ' + tab.name AS trigger_description\n"
             "      ,0        AS position\n"
             "      ,0        AS trigger_before\n"
             "      ,(SELECT CASE type_desc WHEN 'INSERT' THEN 1 ELSE 0 END\n"
             "          FROM sys.trigger_events ev\n"
             "         WHERE ev.object_id = trg.object_id) AS trigger_insert\n"
             "      ,(SELECT CASE type_desc WHEN 'UPDATE' THEN 1 ELSE 0 END\n"
             "          FROM sys.trigger_events ev\n"
             "         WHERE ev.object_id = trg.object_id) AS trigger_update\n"
             "      ,(SELECT CASE type_desc WHEN 'DELETE' THEN 1 ELSE 0 END\n"
             "          FROM sys.trigger_events ev\n"
             "         WHERE ev.object_id = trg.object_id) AS trigger_delete\n"
             "      ,(SELECT CASE type_desc WHEN 'SELECT' THEN 1 ELSE 0 END\n"
             "          FROM sys.trigger_events ev\n"
             "         WHERE ev.object_id = trg.object_id) AS trigger_select\n"
             "      ,0  AS trigger_session\n"
             "   	  ,0  AS trigger_transaction\n"
             "      ,0  AS trigger_rollback\n"
             "      ,'' AS trigger_referencing\n"
             "  	  ,CASE trg.is_disabled WHEN 0 THEN 1 ELSE 0 END AS trigger_enabled\n"
             "  	  ,mod.definition AS trigger_source\n"
             "  FROM sys.triggers    trg\n"
             "      ,sys.sql_modules mod\n"
             "      ,sys.objects     tab\n"
             "      ,sys.schemas     sch\n"
             " WHERE sch.name      = '%s'\n"
             "   AND tab.name      = '%s'\n"
             "   AND trg.object_id = mod.object_id\n"
             "   AND tab.object_id = trg.parent_id\n"
             "   AND tab.schema_id = sch.schema_id"
            ,p_schema.GetString()
            ,p_tablename.GetString());
  if(!p_triggername.IsEmpty())
  {
    sql += "   AND trg.name = '" + p_triggername + "'";
  }
  return sql;
}

CString
SQLInfoSQLServer::GetCATALOGTriggerCreate(MetaTrigger& p_trigger) const
{
  // Simply return the SYS.SQL_MODULE.definition block
  CString sql(p_trigger.m_source);
  sql.TrimRight(';');
  return sql;
}

CString
SQLInfoSQLServer::GetCATALOGTriggerDrop(CString p_schema, CString p_tablename, CString p_triggername) const
{
  return "";
}

//////////////////////////////////////////////////////////////////////////
// ALL SEQUENCE FUNCTIONS

CString
SQLInfoSQLServer::GetCATALOGSequenceExists(CString p_schema, CString p_sequence) const
{
  p_schema.MakeLower();
  p_sequence.MakeLower();
  CString sql = "SELECT COUNT(*)\n"
                "  FROM sys.sequences seq\n"
                "      ,sys.schemas   sch\n"
                " WHERE sch.object_id = seq.schema_id\n"
                "   AND seq.name = '" + p_sequence + "'\n"
                "   AND sch.name = '" + p_schema   + "'";
  return sql;
}

CString
SQLInfoSQLServer::GetCATALOGSequenceAttributes(CString p_schema, CString p_sequence) const
{
  p_schema.MakeLower();
  p_sequence.MakeLower();
  CString sql = "SELECT ''       AS catalog_name\n"
                "      ,sch.name AS schema_name\n"
                "      ,seq.name AS sequence_name\n"
                "      ,seq.current_value\n"
                "      ,0        AS minimal_value\n"
                "      ,1        AS increment\n"
                "      ,seq.is_cached AS cache\n"
                "      ,seq.is_cycling AS cycle\n"
                "      ,0        AS ordering\n"
                "  FROM sys.sequences seq\n"
                "      ,sys.schemas   sch\n"
                " WHERE sch.object_id = seq.schema_id\n"
                "   AND seq.name = '" + p_sequence + "'\n"
                "   AND sch.name = '" + p_schema   + "'";
  return sql;
}

CString
SQLInfoSQLServer::GetCATALOGSequenceCreate(MetaSequence& p_sequence) const
{
  CString sql("CREATE SEQUENCE ");

  if (!p_sequence.m_schemaName.IsEmpty())
  {
    sql += p_sequence.m_schemaName + ".";
  }
  sql += p_sequence.m_sequenceName;
  sql.AppendFormat("\n START WITH %d", p_sequence.m_currentValue);

  sql += p_sequence.m_cycle ? "\n CYCLE" : "\n NOCYCLE";
  if (p_sequence.m_cache > 0)
  {
    sql.AppendFormat("\n CACHE %d",p_sequence.m_cache);
  }
  else
  {
    sql += "\n NOCACHE";
  }
  return sql;
}

CString
SQLInfoSQLServer::GetCATALOGSequenceDrop(CString p_schema, CString p_sequence) const
{
  CString sql("DROP SEQUENCE " + p_schema + "." + p_sequence);
  return  sql;
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
SQLInfoSQLServer::GetPSMProcedureExists(CString p_schema, CString p_procedure) const
{
  p_procedure.MakeUpper();
  return "SELECT count(*)\n"
         "  FROM all_objects\n"
         " WHERE UPPER(object_name) = '" + p_procedure + "'\n"
         "   AND object_type        = 'FUNCTION';";
}

CString
SQLInfoSQLServer::GetPSMProcedureList(CString p_schema) const
{
  return "";
}

CString
SQLInfoSQLServer::GetPSMProcedureAttributes(CString p_schema, CString p_procedure) const
{
  return "";
}

CString
SQLInfoSQLServer::GetPSMProcedureCreate(MetaProcedure& /*p_procedure*/) const
{
  return "";
}

CString
SQLInfoSQLServer::GetPSMProcedureDrop(CString p_schema, CString p_procedure) const
{
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
//     (was GetSessionAndTerminal)
//     (was GetSessionUniqueID)
// - Transactions
//   - GetSessionDeferredConstraints
//   - GetSessionImmediateConstraints
//
//////////////////////////////////////////////////////////////////////////

CString
SQLInfoSQLServer::GetSESSIONConstraintsDeferred() const
{
  // SQL-Server cannot defer constraints
  return "";
}

CString
SQLInfoSQLServer::GetSESSIONConstraintsImmediate() const
{
  // SQL-Server constraints are always active
  return "";
}

//////////////////////////////////////////////////////////////////////////
//
// OLD INTERFACE
//
//////////////////////////////////////////////////////////////////////////

// BOOLEANS EN STRINGS
// ====================================================================

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

// SQL CATALOG QUERIES
// ===================================================================

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
SQLInfoSQLServer::GetSQLOptimizeTable(CString& p_owner,CString& p_tableName,int& /*p_number*/)
{
  CString query("UPDATE STATISTICS ");
  if(!p_owner.IsEmpty())
  {
    query += p_owner;
    query += ".";
  }
  query += p_tableName;
  query += " WITH FULLSCAN";
  return query;
}

// Getting the fact that there is only **one** (1) user session in the database
bool
SQLInfoSQLServer::GetOnlyOneUserSession()
{
  // Yet to implement
  CString sql = "SELECT COUNT(*)\n"
                "  FROM sys.sysprocesses\n"
                " WHERE dbid = db_id()";
  SQLQuery qry(m_database);
  SQLVariant* var = qry.DoSQLStatementScalar(sql);
  return (var->GetAsSLong() <= 1);
}


// SQL DDL STATEMENTS
// ==================

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
  CString kwoot = GetKEYWORDQuoteCharacter();
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
  errorText.Format("ODBC error [%d:%s]",p_error,p_errorText.GetString());
  return errorText;
}

// End of namespace
}
