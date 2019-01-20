////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoOracle.cpp
//
// Copyright (c) 1998-2018 ir. W.E. Huisman
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
// Last Revision:   20-01-2019
// Version number:  1.5.4
//
#include "stdafx.h"
#include "SQLComponents.h"
#include "SQLInfoOracle.h"
#include "SQLQuery.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{

// Constructor
SQLInfoOracle::SQLInfoOracle(SQLDatabase* p_database)
              :SQLInfoDB(p_database)
{
}

// Destructor. Does nothing
SQLInfoOracle::~SQLInfoOracle()
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
SQLInfoOracle::GetRDBMSDatabaseType() const
{
  return RDBMS_ORACLE;
}

// The name of the database vendor
CString
SQLInfoOracle::GetRDBMSVendorName() const
{
  // The name of the database vendor
  return "Oracle";
}

// Get the physical database name
CString
SQLInfoOracle::GetRDBMSPhysicalDatabaseName() const
{
  // Oops. We have a Oracle bug. Name is effectively one of
  // 1) the name in TNSNAMES.ORA file
  // 2) the DNS host name (as mirroring for the database)
  // 3) The SUN NIS name
  // 4) The DCE CDS name 
  // and **NOT** the database name from the server.
  // The names in 1) to 4) could be the same as the database name 
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
SQLInfoOracle::GetRDBMSIsCatalogUpper() const
{
  return true;
}

// System catalog supports full ISO schemas (same tables per schema)
bool
SQLInfoOracle::GetRDBMSUnderstandsSchemas() const
{
  return true;
}

// Supports database/ODBCdriver comments in SQL
bool
SQLInfoOracle::GetRDBMSSupportsComments() const
{
  return true;
}

// Database can defer constraints until the end of a transaction
bool
SQLInfoOracle::GetRDBMSSupportsDeferredConstraints() const
{
  // SET CONSTRAINTS DEFERRED is supported
  return true;
}

// Database has ORDER BY with an expression, e.g. ORDER BY UPPER(columnname)
bool
SQLInfoOracle::GetRDBMSSupportsOrderByExpression() const
{
  return true;
}

// Supports the ODBC escape sequence {[?=] CALL procedure (?,?,?)}
bool
SQLInfoOracle::GetRDBMSSupportsODBCCallEscapes() const
{
  return true;
}

// If the database does not support the datatype TIME, it can be implemented as a DECIMAL
bool
SQLInfoOracle::GetRDBMSSupportsDatatypeTime() const
{
  // Time can be implemented as DECIMAL(17,16)
  return false;
}

// If the database does not support the datatype INTERVAL, it can be implemented as a DECIMAL
bool
SQLInfoOracle::GetRDBMSSupportsDatatypeInterval() const
{
  // Interval not supported, can be implemented as DECIMALS
  return false;
}

// Gets the maximum length of an SQL statement
unsigned long
SQLInfoOracle::GetRDBMSMaxStatementLength() const
{
  // No Limit
  return 0;
}

// Database must commit DDL commands in a transaction
bool
SQLInfoOracle::GetRDBMSMustCommitDDL() const
{
  return false;
}

// KEYWORDS

// Keyword for the current date and time
CString
SQLInfoOracle::GetKEYWORDCurrentTimestamp() const
{
  return "SYSDATE";
}

// String for the current date
CString
SQLInfoOracle::GetKEYWORDCurrentDate() const
{
  return "TRUNC(SYSDATE)";
}

// Get the concatenation operator
CString
SQLInfoOracle::GetKEYWORDConcatanationOperator() const
{
  return "||";
}

// Get quote character for strings
CString
SQLInfoOracle::GetKEYWORDQuoteCharacter() const
{
  return "\'";
}

// Get default NULL for parameter list input
CString
SQLInfoOracle::GetKEYWORDParameterDefaultNULL() const
{
  return "DEFAULT NULL";
}

// Parameter is for INPUT and OUTPUT in parameter list
CString
SQLInfoOracle::GetKEYWORDParameterINOUT() const
{
  return "IN OUT";
}

// Parameter is for OUTPUT only in parameter list
CString
SQLInfoOracle::GetKEYWORDParameterOUT() const
{
  return "OUT";
}

// Get datatype of the IDENTITY primary key in a Network database
CString
SQLInfoOracle::GetKEYWORDNetworkPrimaryKeyType() const
{
  // Use SEQUENCE to fill!
  return "INTEGER";
}

// Get datatype for timestamp (year to second)
CString
SQLInfoOracle::GetKEYWORDTypeTimestamp() const
{
  return "DATE";
}

// Prefix for a parameter in a stored procedure
CString
SQLInfoOracle::GetKEYWORDParameterPrefix() const
{
  return "";
}

// Get select part to add new record identity to a table
// Can be special column like 'OID' or a sequence select
CString
SQLInfoOracle::GetKEYWORDIdentityString(CString& p_tablename,CString p_postfix /*= "_seq"*/) const
{
  return p_tablename + p_postfix + ".nextval";
}

// Gets the UPPER function
CString
SQLInfoOracle::GetKEYWORDUpper(CString& p_expression) const
{
  return "UPPER(" + p_expression + ")";
}

// Gets the construction for 1 minute ago
CString
SQLInfoOracle::GetKEYWORDInterval1MinuteAgo() const
{
  return "(SYSDATE - 0.0007)";
}

// Gets the Not-NULL-Value statement of the database
CString
SQLInfoOracle::GetKEYWORDStatementNVL(CString& p_test,CString& p_isnull) const
{
  return CString("NVL(") + p_test + "," + p_isnull + ")";
}

// Gets the construction for inline generating a key within an INSERT statement
CString
SQLInfoOracle::GetSQLNewSerial(CString p_table, CString p_sequence) const
{
  CString sequence(p_sequence);
  if (sequence.IsEmpty() && !p_table.IsEmpty())
  {
    sequence = p_table + "_seq";
  }

  // Select next value from a generator sequence
  return sequence + ".NEXTVAL";
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

// Gets the sub transaction commands
CString
SQLInfoOracle::GetSQLStartSubTransaction(CString p_savepointName) const
{
  return CString("SAVEPOINT ") + p_savepointName;
}

CString
SQLInfoOracle::GetSQLCommitSubTransaction(CString p_savepointName) const
{
  // There is no savepoint commit in Oracle!!
  return "";
}

CString
SQLInfoOracle::GetSQLRollbackSubTransaction(CString p_savepointName) const
{
  return CString("ROLLBACK TO ") + p_savepointName;
}

// FROM-Part for a query to select only 1 (one) record / or empty!
CString
SQLInfoOracle::GetSQLFromDualClause() const
{
  return " FROM dual";
}

// Get SQL to lock  a table 
CString
SQLInfoOracle::GetSQLLockTable(CString p_schema, CString p_tablename, bool p_exclusive) const
{
  CString query = "LOCK TABLE " + p_schema + "." + p_tablename + " IN ";
  query += p_exclusive ? "EXCLUSIVE" : "SHARE";
  query += " MODE";
  return query;
}

// Get query to optimize the table statistics
CString
SQLInfoOracle::GetSQLOptimizeTable(CString p_schema, CString p_tablename) const
{
  CString optim;
  // Optimize the table
  optim = "call dbms_stats.gather_table_stats('" + p_schema + "','" + p_tablename + "')";
  return optim;
}

//////////////////////////////////////////////////////////////////////////
//
// SQL STRINGS
//
//////////////////////////////////////////////////////////////////////////

// Makes a SQL string from a given string, with all the right quotes
CString
SQLInfoOracle::GetSQLString(const CString& p_string) const
{
  CString s = p_string;
  s.Replace("'","''");
  CString kwoot = GetKEYWORDQuoteCharacter();
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
  int dag = 24 * 60 * 60;
  double val = (double)seconds / (double)dag;
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
// Standard ODBC functions are ***NOT*** good enough (6 seconds per connect)
// So we provide our own queries for these functions
CString
SQLInfoOracle::GetCATALOGMetaTypes(int p_type) const
{
  CString sql;

  switch(p_type)
  {
    case META_CATALOGS: sql = "SELECT UPPER(Value) AS catalog\n"
                              "      ,''           AS remarks\n"
                              "  FROM v$parameter\n"
                              " WHERE name = 'db_name'\n"
                              "UNION\n"
                              "SELECT db_link\n"
                              "      ,''\n"
                              "  FROM dba_db_links";
                        break;
    case META_SCHEMAS:  sql = "SELECT DISTINCT username\n"
                              "      ,'' AS remarks\n"
                              "  FROM all_users";
                        break;
    case META_TABLES:   sql = "SELECT 'TABLE' AS table_type\n"
                              "      ,''      AS remarks\n"
                              "  FROM dual\n"
                              "UNION\n"
                              "SELECT 'VIEW'\n"
                              "      ,''\n"
                              "  FROM dual";
                        break;
  }
  return sql;
}

// Get SQL to check if a table already exists in the database
CString
SQLInfoOracle::GetCATALOGTableExists(CString p_schema,CString p_tablename) const
{
  p_schema.MakeUpper();
  p_tablename.MakeUpper();
  CString query = "SELECT COUNT(*)\n"
                  "  FROM dba_tables\n"
                  " WHERE owner      = '" + p_schema    + "'\n"
                  "   AND table_name = '" + p_tablename + "'";
  return query;
}

CString
SQLInfoOracle::GetCATALOGTablesList(CString p_schema,CString p_pattern) const
{
  return GetCATALOGTableAttributes(p_schema,p_pattern);
}

CString
SQLInfoOracle::GetCATALOGTableAttributes(CString p_schema,CString p_tablename) const
{
  CString sql;
  p_schema.MakeUpper();
  p_tablename.MakeUpper();

  // TABLES
  sql  = "SELECT ora_database_name AS table_catalog\n"
         "      ,tab.owner         AS table_schema\n"
         "      ,tab.table_name    AS table_name\n"
         "      ,CASE tab.TEMPORARY\n"
         "            WHEN 'Y' THEN 'GLOBAL TEMPORARY'\n"
         "            WHEN 'N' THEN 'TABLE'\n"
         "                     ELSE 'UNKNOWN'\n"
         "       END               AS object_type\n"
         "      ,com.comments      AS remarks\n"
         "      ,tab.tablespace_name\n"
         "      ,CASE tab.TEMPORARY\n"
         "            WHEN 'Y' THEN 1\n"
         "            WHEN 'N' THEN 0\n"
         "                     ELSE -1\n"
         "       END AS temporary\n"
         "  FROM dba_tables tab LEFT OUTER JOIN dba_tab_comments com\n"
         "                      ON (com.owner      = tab.owner\n"
         "                     AND  com.table_name = tab.table_name)\n"
         " WHERE com.table_type = 'TABLE'\n";


  if(!p_schema.IsEmpty())
  {
    sql += "   AND tab.owner ";
    sql += p_schema.Find('%') >= 0 ? "LIKE '" : "= '";
    sql += p_schema + "'\n";
  }

  if(!p_tablename.IsEmpty())
  {
    sql += "   AND tab.table_name ";
    sql += p_tablename.Find('%') >= 0 ? "LIKE '" : "= '";
    sql += p_tablename + "'\n";
  }
  sql += " ORDER BY 1,2,3";

  return sql;
}

CString 
SQLInfoOracle::GetCATALOGTableSynonyms(CString p_schema,CString p_tablename) const
{
  p_schema.MakeUpper();
  p_tablename.MakeUpper();

  CString sql = "SELECT ora_database_name AS table_catalog\n"
                "      ,table_owner  AS table_schema\n"
                "      ,synonym_name AS table_name\n"
                "      ,'SYNONYM'    AS object_type\n"
                "      ,''           AS remarks\n"
                "      ,''           AS tablespace_name\n"
                "      ,0            AS temporary\n"
                "  FROM dba_synonyms syn\n";

  if(!p_schema.IsEmpty())
  {
    sql += " WHERE table_owner ";
    sql += p_schema.Find('%') >= 0 ? "LIKE '" : " = '";
    sql += p_schema + "'\n";
  }

  if(!p_tablename.IsEmpty())
  {
    sql += p_schema.IsEmpty() ? " WHERE " : "   AND ";
    sql += "synonym_name ";
    sql += p_tablename.Find('%') >= 0 ? "LIKE '" : "= '";
    sql += p_tablename + "'\n";
  }
  sql += " ORDER BY 1,2,3";
  return sql;
}

CString
SQLInfoOracle::GetCATALOGTableCatalog(CString p_schema,CString p_tablename) const
{
  p_schema.MakeUpper();
  p_tablename.MakeUpper();

  CString sql = "SELECT ora_database_name AS table_catalog\n"
                "      ,obj.owner         AS table_schema\n"
                "      ,obj.object_name   AS table_name\n"
                "      ,'SYSTEM TABLE'    AS object_type\n"
                "      ,com.comments      AS remarks\n"
                "      ,''                AS tablespace_name\n"
                "      ,0                 AS temporary\n"
                "  FROM dba_objects obj LEFT OUTER JOIN dba_tab_comments com\n"
                "                       ON (com.owner      = obj.owner\n"
                "                      AND  com.table_name = obj.object_name)\n"
                " WHERE obj.owner IN ('SYS','SYSTEM')\n"
                "   AND obj.object_type IN ('TABLE','VIEW')\n"
                "   AND obj.object_name NOT LIKE ('%$%')\n";

  if(!p_schema.IsEmpty())
  {
    if(p_schema.Find('%') >= 0)
    {
      sql += "  AND obj.owner   LIKE '" + p_schema + "'\n";
    }
    else
    {
      sql += "  AND obj.owner   = '" + p_schema + "'\n";
    }
  }

  if(!p_tablename.IsEmpty())
  {
    if(p_tablename.Find('%') >= 0)
    {
      sql += "   AND obj.object_name LIKE '" + p_tablename + "'\n";
    }
    else
    {
      sql += "   AND obj.object_name = '" + p_tablename + "'\n";
    }
  }

  sql += " ORDER BY 1,2,3";
  return sql;
}


CString
SQLInfoOracle::GetCATALOGTableCreate(MetaTable& /*p_table*/,MetaColumn& /*p_column*/) const
{
  return "";
}

CString
SQLInfoOracle::GetCATALOGTableRename(CString p_schema,CString p_tablename,CString p_newname) const
{
  // Beware: No 'TABLE' in the statement
  CString sql("RENAME " + p_schema + "." + p_tablename + " TO " + p_newname);
  return sql;
}

CString
SQLInfoOracle::GetCATALOGTableDrop(CString p_schema,CString p_tablename) const
{
  CString sql("DROP TABLE ");
  if(!p_schema.IsEmpty())
  {
    sql += p_schema + ".";
  }
  sql += p_tablename;
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL TEMPORARY TABLE FUNCTIONS

CString 
SQLInfoOracle::GetCATALOGTemptableCreate(CString p_schema,CString p_tablename,CString p_select) const
{
  return "CREATE GLOBAL TEMPORARY TABLE " + p_schema + "." + p_tablename + "\nAS " + p_select +
         "ON COMMIT PRESERVE ROWS";
}

CString 
SQLInfoOracle::GetCATALOGTemptableIntoTemp(CString p_schema,CString p_tablename,CString p_select) const
{
  return "INSERT INTO " + p_schema + "." + p_tablename + "\n" + p_select;
}

CString 
SQLInfoOracle::GetCATALOGTemptableDrop(CString p_schema,CString p_tablename) const
{
  CString tablename = p_schema + "." + p_tablename;
  return "DELETE FROM "    + tablename + ";\n"
         "<@>\n"
         "TRUNCATE TABLE " + tablename + ";\n"
         "<@>\n"
         "DROP TABLE "     + tablename + ";\n";
}

//////////////////////////////////////////////////////////////////////////
// ALL COLUMN FUNCTIONS

CString 
SQLInfoOracle::GetCATALOGColumnExists(CString p_schema,CString p_tablename,CString p_columnname) const
{
  p_schema.MakeUpper();
  p_tablename.MakeUpper();
  p_columnname.MakeUpper();
  CString query = "SELECT COUNT(*)\n"
                 "  FROM dba_tab_columns tab\n"
                 " WHERE tab.table_name  = '" + p_tablename  + "'\n"
                 "   AND tab.column_name = '" + p_columnname + "'\n"
                 "   AND tab.owner       = '" + p_schema     + "'";
  return query;
}

CString 
SQLInfoOracle::GetCATALOGColumnList(CString p_schema,CString p_tablename) const
{
  return "";
//   CString sql = GetCATALOGColumnAttributes(p_schema,p_tablename,"");
// 
//   int pos = sql.ReverseFind('\n');
//   sql = sql.Mid(pos + 1) + " ORDER BY column_id";
// 
//   return sql;
}

CString 
SQLInfoOracle::GetCATALOGColumnAttributes(CString p_schema,CString p_tablename,CString p_columnname) const
{
  return "";
//   p_schema.MakeUpper();
//   p_tablename.MakeUpper();
//   p_columnname.MakeUpper();
// 
//   // name, number, type, length, nullable, precision, scale
//   CString sql = "SELECT column_name\n"          // 1 -> Columnname
//                 "      ,column_id\n"            // 2 -> Position
//                 "      ,data_type\n"            // 3 -> Datatype
//                 "      ,data_length\n"          // 4 -> Length
//                 "      ,CASE WHEN nullable = 'Y' THEN 1 ELSE 0 END\n" // 5-> Nullable
//                 "      ,data_precision\n"       // 5 -> Precision
//                 "      ,data_scale\n"           // 6 -> Scale
//                 "      ,data_default\n"         // 7 -> Default
//                 "  FROM dba_tab_columns\n"
//                 " WHERE owner       = '" + p_schema     + "'\n"
//                 "   AND table_name  = '" + p_tablename  + "'\n"
//                 "   AND column_name = '" + p_columnname + "'";
//                 // BEWARE: Columnname filter must be last for previous method!
//   return sql;
}

CString 
SQLInfoOracle::GetCATALOGColumnCreate(MetaColumn& p_column) const
{
  CString sql = "ALTER TABLE "  + p_column.m_schema + "." + p_column.m_table  + "\n"
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
SQLInfoOracle::GetCATALOGColumnAlter(MetaColumn& p_column) const
{
  CString sql = "ALTER  TABLE  " + p_column.m_schema + "." + p_column.m_table  + "\n"
                "MODIFY COLUMN " + p_column.m_column + " " + p_column.m_typename;
  p_column.GetPrecisionAndScale(sql);
  p_column.GetNullable(sql);
  p_column.GetDefault(sql);
  // m_position not used
  // m_length   not used
  // m_remarks  not used
  return sql;
}

CString
SQLInfoOracle::GetCATALOGColumnRename(CString p_schema,CString p_tablename,CString p_columnname,CString p_newname,CString /*p_datatype*/) const
{
  CString sql("ALTER  TABLE  " + p_schema + "." + p_tablename + "\n"
              "RENAME COLUMN " + p_columnname + " TO " + p_newname + "\n");
  return sql;
}

CString
SQLInfoOracle::GetCATALOGColumnDrop(CString p_schema,CString p_tablename,CString p_columnname) const
{
  CString sql("ALTER TABLE " + p_schema + "." + p_tablename + "\n"
              " DROP COLUMN " + p_columnname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL INDICES FUNCTIONS

// All index functions
CString
SQLInfoOracle::GetCATALOGIndexExists(CString p_schema,CString p_tablename,CString p_indexname) const
{
  return "";
}

CString
SQLInfoOracle::GetCATALOGIndexList(CString p_schema,CString p_tablename)   const
{
  p_schema.MakeUpper();
  p_tablename.MakeUpper();

  // Query runs on the 'dba_' variant, so owner name must be taken into account
  // for performance reasons on the system table
  CString query = "SELECT idx.index_name\n"
                  "      ,col.column_name\n"
                  "      ,col.column_position\n"
//                "      ,idx.index_type\n"
                  "      ,idx.uniqueness\n"
                  "      ,col.descend\n"
                  "      ,'' as column_source"
                  "  FROM dba_indexes     idx\n"
                  "      ,dba_ind_columns col\n"
                  " WHERE idx.index_name  = col.index_name\n"
                  "   AND idx.table_name  = col.table_name\n"
                  "   AND idx.table_owner = col.table_owner\n"
                  "   AND idx.generated   = 'N'\n"
                  "   AND idx.table_name  = '" + p_tablename + "'\n"
                  "   AND idx.table_owner = '" + p_schema     + "'\n"
                  " ORDER BY index_name\n"
                  "         ,column_position\n";
  return query;
}

CString
SQLInfoOracle::GetCATALOGIndexAttributes(CString p_schema,CString p_tablename,CString p_indexname) const
{
  return "";
}

CString
SQLInfoOracle::GetCATALOGIndexCreate(MIndicesMap& p_indices) const
{
  // Get SQL to create an index for a table
  // CREATE [UNIQUE] INDEX [<schema>.]indexname ON [<schema>.]tablename(column [ASC|DESC] [,...]);
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
    if(!index.m_filter.IsEmpty())
    {
      query += index.m_filter;
    }
    else
    {
      query += index.m_columnName;
      if(index.m_ascending != "A")
      {
        query += " DESC";
      }
    }
  }
  query += ")";
  return query;
}

CString
SQLInfoOracle::GetCATALOGIndexDrop(CString p_schema,CString /*p_tablename*/,CString p_indexname) const
{
  CString sql = "DROP INDEX " + p_schema + "." + p_indexname;
  return sql;
}

// Get extra filter expression for an index column
CString
SQLInfoOracle::GetCATALOGIndexFilter(MetaIndex& p_index) const
{
  CString expression;
  CString sql;
  sql.Format("SELECT column_expression\n"
             "  FROM dba_ind_expressions\n"
             " WHERE index_owner = '" + p_index.m_schemaName + "'\n"
             "   AND index_name  = '" + p_index.m_indexName + "'\n"
             "   AND table_owner = '" + p_index.m_schemaName + "'\n"
             "   AND table_name  = '" + p_index.m_tableName + "'\n"
             "   AND column_position = %d",p_index.m_position);
  try
  {
    SQLQuery qry(m_database);
    SQLVariant* var = qry.DoSQLStatementScalar(sql);
    if(var)
    {
      var->GetAsString(expression);
    }
  }
  catch(StdException& error)
  {
    ReThrowSafeException(error);
    CString message = ("Cannot find index filter: ") + error.GetErrorMessage();
    throw StdException(message);
  }
  return expression;
}

//////////////////////////////////////////////////////////////////////////
// ALL PRIMARY KEY FUNCTIONS

CString
SQLInfoOracle::GetCATALOGPrimaryExists(CString p_schema,CString p_tablename) const
{
  p_schema.MakeUpper();
  p_tablename.MakeUpper();

  CString query = "SELECT c.constraint_name\n"
                  "      ,c.index_name\n"
                  "      ,'Y' as deferrable\n"
                  "      ,'N' as initially_deferred\n"
                  "  FROM dba_constraints c\n"
                  " WHERE c.owner           = '" + p_schema    + "'\n"
                  "   AND c.table_name      = '" + p_tablename + "'\n"
                  "   AND c.constraint_type = 'P'\n";
  return query;
}

CString
SQLInfoOracle::GetCATALOGPrimaryAttributes(CString p_schema,CString p_tablename) const
{
  // To be implemented
  return "";
}

CString
SQLInfoOracle::GetCATALOGPrimaryCreate(MPrimaryMap& p_primaries) const
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
SQLInfoOracle::GetCATALOGPrimaryDrop(CString p_schema,CString p_tablename,CString p_constraintname) const
{
  CString sql("ALTER TABLE " + p_schema + "." + p_tablename + "\n"
              " DROP CONSTRAINT " + p_constraintname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL FOREIGN KEY FUNCTIONS

CString
SQLInfoOracle::GetCATALOGForeignExists(CString p_schema,CString p_tablename,CString p_constraintname) const
{
  p_schema.MakeUpper();
  p_tablename.MakeUpper();
  p_constraintname.MakeUpper();

  CString sql;
  sql.Format("SELECT COUNT(*)\n"
             "  FROM dba_constraints  con\n"
             " WHERE con.constraint_type = 'R'"
             "   AND con.owner           = '" + p_schema + "'\n"
             "   AND con.table_name      = '" + p_tablename + "'\n"
             "   AND con.constraint_name = '" + p_constraintname + "'"
            ,p_schema.GetString()
            ,p_tablename.GetString()
            ,p_constraintname.GetString());

  return sql;
}

CString
SQLInfoOracle::GetCATALOGForeignList(CString p_schema,CString p_tablename,int p_maxColumns /*=SQLINFO_MAX_COLUMNS*/) const
{
  return GetCATALOGForeignAttributes(p_schema,p_tablename,"",p_maxColumns);
}

CString
SQLInfoOracle::GetCATALOGForeignAttributes(CString  p_schema
                                          ,CString  p_tablename
                                          ,CString  p_constraint
                                          ,bool     p_referenced /*=false*/
                                          ,int    /*p_maxColumns*/ /*=SQLINFO_MAX_COLUMNS*/) const
{
  // Oracle catalog is in uppercase
  p_schema.MakeUpper();
  p_tablename.MakeUpper();
  p_constraint.MakeUpper();

  // Minimal requirements of the catalog
  if(p_schema.IsEmpty() || p_tablename.IsEmpty())
  {
    throw StdException("Cannot get table references without schema/tablename");
  }

  CString query = "SELECT ora_database_name    AS primary_catalog_name\n"
                  "      ,pri.owner            AS primary_schema_name\n"
                  "      ,pri.table_name       AS primary_table_name\n"
                  "      ,ora_database_name    AS foreign_catalog_name\n"
                  "      ,con.owner            AS foreign_schema_name\n"
                  "      ,con.table_name       AS table_name\n"
                  "      ,pri.constraint_name  AS primary_key_constraint\n"
                  "      ,con.constraint_name  AS foreign_key_constraint\n"
                  "      ,col.position         AS key_sequence\n"
                  "      ,pky.column_name      AS primary_key_column\n"
                  "      ,col.column_name      AS foreign_key_column\n"
                  "      ,0                    AS update_rule\n"
                  "      ,CASE con.delete_rule WHEN 'RESTRICT'    THEN 1\n"
                  "                            WHEN 'CASCADE'     THEN 0\n"
                  "                            WHEN 'SET NULL'    THEN 2\n"
                  "                            WHEN 'SET DEFAULT' THEN 4\n"
                  "                            WHEN 'NO ACTION'   THEN 3\n"
                  "                            ELSE 0 END AS delete_rule\n"
                  "      ,CASE con.deferrable  WHEN 'NOT DEFERRABLE' THEN 0\n"
                  "                            WHEN 'DEFERRABLE'     THEN 1\n"
                  "                            ELSE 0 END AS DEFERRABLE\n"
                  "      ,0                    AS match_option\n"
                  "      ,CASE con.deferred    WHEN 'IMMEDIATE'      THEN 0\n"
                  "                            ELSE 1 END AS initially_deferred\n"
                  "      ,CASE con.status      WHEN 'ENABLED' THEN 1\n"
                  "                            ELSE 0 END AS enabled\n"
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

  // Optionally add our filters
  if(!p_schema.IsEmpty())
  {
    if(p_referenced)
    {
      query += "   AND pri.owner           = '" + p_schema + "'\n";
    }
    else
    {
      query += "   AND con.owner           = '" + p_schema + "'\n";
    }
  }
  if(!p_tablename.IsEmpty())
  {
    if(p_referenced)
    {
      query += "   AND pri.table_name      = '" + p_tablename + "'\n";
    }
    else
    {
      query += "   AND con.table_name      = '" + p_tablename + "'\n";
    }

  }
  if(!p_constraint.IsEmpty())
  {
    if(p_referenced)
    {
      query += "   AND pri.constraint_name = '" + p_constraint + "'\n";
    }
    else
    {
      query += "   AND con.constraint_name = '" + p_constraint + "'\n";
    }
  }

  // Order upto the column number
  query += " ORDER BY 1,2,3,4,5,6,7,8,9";

  return query;
}

CString
SQLInfoOracle::GetCATALOGForeignCreate(MForeignMap& p_foreigns) const
{
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
  switch(foreign.m_deferrable)
  {
    case SQL_INITIALLY_DEFERRED:  query += "\n      INITIALLY DEFERRED"; break;
    case SQL_INITIALLY_IMMEDIATE: query += "\n      DEFERRABLE";         break;
    case SQL_NOT_DEFERRABLE:      // Already the default
                                  break;
  }
  switch(foreign.m_deleteRule)
  {
    case SQL_CASCADE:  query += "\n      ON DELETE CASCADE";     break;
    case SQL_SET_NULL: query += "\n      ON DELETE SET NULL";    break;
    default:           // In essence: ON DELETE RESTRICT, but that's already the default
                       break;
  }
  return query;
}

CString
SQLInfoOracle::GetCATALOGForeignAlter(MForeignMap& p_original, MForeignMap& p_requested) const
{
  // Make sure we have both
  if (p_original.empty() || p_requested.empty())
  {
    return "";
  }

  MetaForeign& original = p_original.front();
  MetaForeign& requested = p_requested.front();

  // Construct the correct tablename
  CString table(original.m_fkTableName);
  if(!original.m_fkSchemaName.IsEmpty())
  {
    table = original.m_fkSchemaName + "." + table;
  }

  // The base foreign key command
  CString query = "ALTER  TABLE " + table + "\n"
                  "MODIFY CONSTRAINT " + original.m_foreignConstraint + "\n";

  // Add all relevant options
  if(original.m_deferrable != requested.m_deferrable)
  {
    query.AppendFormat("\n      %sDEFERRABLE", requested.m_deferrable == 0 ? "NOT " : "");
  }
  if(original.m_initiallyDeferred != requested.m_initiallyDeferred)
  {
    query += "\n      INITIALLY ";
    query += requested.m_initiallyDeferred ? "DEFERRED" : "IMMEDIATE";
  }
  if (original.m_deleteRule != requested.m_deleteRule)
  {
    switch(requested.m_deleteRule)
    {
      case SQL_CASCADE:   query += "\n      ON DELETE CASCADE";     break;
      case SQL_SET_NULL:  query += "\n      ON DELETE SET NULL";    break;
      default:            // In essence: ON DELETE RESTRICT, 
                          // But Oracle cannot do that, so return an empty string and drop the constraint
      case SQL_RESTRICT:  query.Empty();
    }
  }
  return query;
}

CString
SQLInfoOracle::GetCATALOGForeignDrop(CString p_schema,CString p_tablename,CString p_constraintname) const
{
  CString sql("ALTER TABLE " + p_schema + "." + p_tablename + "\n"
              " DROP CONSTRAINT " + p_constraintname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL TRIGGER FUNCTIONS

CString
SQLInfoOracle::GetCATALOGTriggerExists(CString p_schema, CString p_tablename, CString p_triggername) const
{
  p_schema.MakeUpper();
  p_tablename.MakeUpper();
  p_triggername.MakeUpper();

  CString sql;
  sql.Format("SELECT COUNT(*)\n"
             "  FROM dba_triggers\n"
             " WHERE table_owner  = '%s'\n"
             "   AND table_name   = '%s'\n"
             "   AND trigger_name = '%s'"
            ,p_schema.GetString()
            ,p_tablename.GetString()
            ,p_triggername.GetString());
  return sql;
}

CString
SQLInfoOracle::GetCATALOGTriggerList(CString p_schema, CString p_tablename) const
{
  return GetCATALOGTriggerAttributes(p_schema,p_tablename,"");
}

CString
SQLInfoOracle::GetCATALOGTriggerAttributes(CString p_schema, CString p_tablename, CString p_triggername) const
{
  p_schema.MakeUpper();
  p_tablename.MakeUpper();
  p_triggername.MakeUpper();

  CString sql = "SELECT ''    AS catalog_name\n"
                "      ,owner AS schema_name\n"
                "      ,table_name\n"
                "      ,trigger_name\n"
                "      ,triggering_event || ' ON ' || table_name AS description\n"
                "      ,0     AS position\n"
                "      ,CASE WHEN (InStr(trigger_type,    'BEFORE') > 0) THEN 1 ELSE 0 END AS trigger_before\n"
                "      ,CASE WHEN (InStr(triggering_event,'INSERT') > 0) THEN 1 ELSE 0 END AS trigger_insert\n"
                "      ,CASE WHEN (InStr(triggering_event,'UPDATE') > 0) THEN 1 ELSE 0 END AS trigger_update\n" 
                "      ,CASE WHEN (InStr(triggering_event,'DELETE') > 0) THEN 1 ELSE 0 END AS trigger_delete\n" 
                "      ,CASE WHEN (InStr(triggering_event,'SELECT') > 0) THEN 1 ELSE 0 END AS trigger_select\n"
                "      ,CASE WHEN (InStr(triggering_event,'LOGON')  > 0 OR\n"
                "                  InStr(triggering_event,'LOGOFF') > 0 ) THEN 1 ELSE 0 END AS trigger_session\n"
                "      ,0  AS trigger_transaction\n"
                "      ,0  AS trigger_rollback\n"
                "      ,referencing_names\n"
                "      ,CASE status\n"
                "            WHEN 'DISABLED' THEN 0\n"
                "                            ELSE 1\n"
                "            END AS trigger_status\n"
                "      ,trigger_body AS source\n"
                "  FROM dba_triggers\n";
  if(!p_schema.IsEmpty())
  {
    sql += " WHERE table_owner = '" + p_schema + "'\n";
  }
  if(!p_tablename.IsEmpty())
  {
    sql += p_schema.IsEmpty() ? " WHERE " : "   AND ";
    sql += "table_name = '" + p_tablename + "'\n";
  } 
  if(!p_triggername.IsEmpty())
  {
    sql += p_schema.IsEmpty() && p_tablename.IsEmpty() ? " WHERE " : "   AND ";
    sql += "trigger_name = '" + p_triggername + "'\n";
  }
  sql += " ORDER BY 1,2,3,4";
  return sql;
}

CString
SQLInfoOracle::GetCATALOGTriggerCreate(MetaTrigger& p_trigger) const
{
  // Command + trigger name
  CString sql("CREATE OR REPLACE TRIGGER ");
  sql += p_trigger.m_schemaName;
  sql += ".";
  sql += p_trigger.m_triggerName;
  sql += "\n";

  // Before or after
  sql += p_trigger.m_before ? "BEFORE " : "AFTER ";

  // Trigger actions
  if(p_trigger.m_insert)
  {
    sql += "INSERT";
  }
  if(p_trigger.m_update)
  {
    if(p_trigger.m_insert)
    {
      sql += " OR ";
    }
    sql += "UPDATE";
  }
  if(p_trigger.m_delete)
  {
    if(p_trigger.m_insert || p_trigger.m_update)
    {
      sql += " OR ";
    }
    sql += "DELETE";
  }

  // Add trigger table
  sql += " ON ";
  sql += p_trigger.m_tableName;
  sql += "\n";

  // Referencing clause
  if(!p_trigger.m_referencing.IsEmpty())
  {
    sql += p_trigger.m_referencing;
    sql += "\nFOR EACH ROW\n";
  }

  // Add trigger body
  sql += p_trigger.m_source;

  return sql;
}

CString
SQLInfoOracle::GetCATALOGTriggerDrop(CString p_schema, CString p_tablename, CString p_triggername) const
{
  return "";
}

//////////////////////////////////////////////////////////////////////////
// ALL SEQUENCE FUNCTIONS

CString
SQLInfoOracle::GetCATALOGSequenceExists(CString p_schema, CString p_sequence) const
{
  p_schema.MakeUpper();
  p_sequence.MakeUpper();

  CString sql = "SELECT COUNT(*)\n"
                "  FROM dba_sequences\n"
                " WHERE sequence_owner = '" + p_schema + "'\n"
                "   AND sequence_name  = '" + p_sequence + "'";
  return sql;
}

CString
SQLInfoOracle::GetCATALOGSequenceList(CString p_schema,CString p_pattern) const
{
  p_schema.MakeUpper();
  p_pattern.MakeUpper();
  if(!p_pattern.IsEmpty() && p_pattern != "%")
  {
    p_pattern = "%" + p_pattern + "%";
  }
  CString sql = "SELECT ''              AS catalog_name\n"
                "      ,sequence_owner  AS schema_name\n"
                "      ,sequence_name\n"
                "      ,last_number     AS current_value\n"
                "      ,min_value       AS minimal_value\n"
                "      ,increment_by    AS seq_increment\n"
                "      ,cache_size                 AS cache\n"
                "      ,decode(cycle_flag,'N',0,1) AS cycle\n"
                "      ,decode(order_flag,'N',0,1) AS ordering\n"
                "  FROM dba_sequences\n";
  if (!p_schema.IsEmpty())
  {
    sql += " WHERE sequence_owner = '" + p_schema + "'\n";
  }
  if (!p_pattern.IsEmpty())
  {
    sql += p_schema.IsEmpty() ? " WHERE " : "   AND ";
    sql += "sequence_name  LIKE '" + p_pattern + "'\n";
  }
  sql += " ORDER BY 1,2,3";
  return sql;
}

CString
SQLInfoOracle::GetCATALOGSequenceAttributes(CString p_schema, CString p_sequence) const
{
  p_schema.MakeUpper();
  p_sequence.MakeUpper();

  CString sql = "SELECT ''              AS catalog_name\n"
                "      ,sequence_owner  AS schema_name\n"
                "      ,sequence_name\n"
                "      ,last_number     AS current_value\n"
                "      ,min_value       AS minimaml_value\n"
                "      ,increment_by    AS seq_increment\n"
                "      ,cache_size                 AS cache\n"
                "      ,decode(cycle_flag,'N',0,1) AS cycle\n"
                "      ,decode(order_flag,'N',0,1) AS ordering\n"
                "  FROM dba_sequences\n";
  if(!p_schema.IsEmpty())
  {
    sql += " WHERE sequence_owner = '" + p_schema + "'\n";
  }
  if(!p_sequence.IsEmpty())
  {
    sql += p_schema.IsEmpty() ? " WHERE " : "   AND ";
    sql += "sequence_name  = '" + p_sequence + "'\n";
  }
  sql += " ORDER BY 1,2,3";
  return sql;
}

CString
SQLInfoOracle::GetCATALOGSequenceCreate(MetaSequence& p_sequence) const
{
  CString sql("CREATE SEQUENCE ");

  if (!p_sequence.m_schemaName.IsEmpty())
  {
    sql += p_sequence.m_schemaName + ".";
  }
  sql += p_sequence.m_sequenceName;
  sql.AppendFormat("\n START WITH %-12.0f", p_sequence.m_currentValue);
  sql.AppendFormat("\n INCREMENT BY %d", p_sequence.m_increment);

  sql += p_sequence.m_cycle ? "\n CYCLE" : "\n NOCYCLE";
  sql += p_sequence.m_order ? "\n ORDER" : "\n NOORDER";
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
SQLInfoOracle::GetCATALOGSequenceDrop(CString p_schema, CString p_sequence) const
{
  CString sql("DROP SEQUENCE " + p_schema + "." + p_sequence);
  return  sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL VIEW FUNCTIONS

CString 
SQLInfoOracle::GetCATALOGViewExists(CString p_schema,CString p_viewname) const
{
  p_schema.MakeUpper();
  p_viewname.MakeUpper();
  CString sql("SELECT COUNT(*)\n"
              "  FROM dba_views\n"
              " WHERE view_name = '" + p_viewname + "'\n"
              "   AND owner     = '" + p_schema   + "'");
  return sql;
}

CString 
SQLInfoOracle::GetCATALOGViewList(CString p_schema,CString p_pattern) const
{
  return "";
}

CString 
SQLInfoOracle::GetCATALOGViewAttributes(CString p_schema,CString p_viewname) const
{
  // VIEWS
  CString sql = "SELECT ora_database_name AS table_catalog\n"
                "      ,viw.owner         AS table_schema\n"
                "      ,viw.view_name     AS table_name\n"
                "      ,'VIEW'            AS object_type\n"
                "      ,com.comments      AS remarks\n"
                "      ,''                AS tablespace_name\n"
                "      ,0                 AS TEMPORARY\n"
                "  FROM dba_views viw LEFT OUTER JOIN dba_tab_comments com\n"
                "                     ON (viw.owner     = com.owner\n"
                "                    AND  viw.view_name = com.table_name)\n"
                " WHERE com.table_type = 'VIEW'\n";
  if(!p_schema.IsEmpty())
  {
    if(p_schema.Find('%') >= 0)
    {
      sql += "  AND viw.owner   LIKE '" + p_schema + "'\n";
    }
    else
    {
      sql += "  AND viw.owner   = '" + p_schema + "'\n";
    }
  }

  if(!p_viewname.IsEmpty())
  {
    if(p_viewname.Find('%') >= 0)
    {
      sql += "   AND viw.view_name   LIKE '" + p_viewname + "'\n";
    }
    else
    {
      sql += "   AND viw.view_name   = '" + p_viewname + "'\n";
    }
  }

  sql += " ORDER BY 1,2,3";
  return sql;
}

CString 
SQLInfoOracle::GetCATALOGViewCreate(CString p_schema,CString p_viewname,CString p_contents) const
{
  return "CREATE OR REPLACE VIEW " + p_schema + "." + p_viewname + "\n" + p_contents;
}

CString 
SQLInfoOracle::GetCATALOGViewRename(CString p_schema,CString p_viewname,CString p_newname)    const
{
  return "";
}

CString 
SQLInfoOracle::GetCATALOGViewDrop(CString p_schema,CString p_viewname,CString& p_precursor) const
{
  p_precursor.Empty();
  return "DROP VIEW " + p_schema + "." + p_viewname;
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

CString
SQLInfoOracle::GetPSMProcedureExists(CString p_schema, CString p_procedure) const
{
  p_schema.MakeUpper();
  p_procedure.MakeUpper();
  return "SELECT COUNT(*)\n"
         "  FROM dba_procedures\n"
         " WHERE owner       = '" + p_schema + "'\n"
         "   AND object_name = '" + p_procedure + "'\n"
         "   AND object_type IN ('PROCEDURE','FUNCTION')";
}

CString
SQLInfoOracle::GetPSMProcedureList(CString p_schema) const
{
  p_schema.MakeUpper();
  CString sql;

  sql = "SELECT ora_database_name AS procedure_catalog\n"
        "      ,owner             AS procedure_schema\n"
        "      ,object_name       AS procedure_name\n"
        "  FROM dba_procedures\n";
  if(!p_schema.IsEmpty())
  {
    sql += " WHERE owner = '" + p_schema + "'\n";
  }
  sql += " ORDER BY 1,2,3";
  return sql;
}

CString
SQLInfoOracle::GetPSMProcedureAttributes(CString p_schema, CString p_procedure) const
{
  p_schema.MakeUpper();
  p_procedure.MakeUpper();
  CString sql;
  sql = "SELECT ora_database_name AS procedure_catalog\n"
        "      ,owner             AS procedure_schema\n"
        "      ,object_name       AS procedure_name\n"
        "      ,(SELECT COUNT(*)\n"
        "           FROM dba_arguments par\n"
        "         WHERE par.owner       = pro.owner\n"
        "           AND par.object_name = pro.object_name\n"
        "           AND par.object_id   = pro.object_id\n"
        "           AND par.in_out IN ('IN','IN/OUT')) as input_params\n"
        "      ,(SELECT COUNT(*)\n"
        "          FROM dba_arguments par\n"
        "         WHERE par.owner       = pro.owner\n"
        "           AND par.object_name = pro.object_name\n"
        "           AND par.object_id   = pro.object_id\n"
        "           AND par.in_out IN ('OUT','IN/OUT')) as output_params\n"
        "      ,0   AS result_sets\n"
        "      ,''  AS remarks\n"
        "      ,CASE object_type \n"
        "            WHEN 'PROCEDURE' THEN 1\n"
        "            WHEN 'FUNCTION'  THEN 2\n"
        "                             ELSE 3\n"
        "       END AS procedure_type\n"
        "      ,'<@>' as source\n"
        "  FROM dba_procedures pro\n"
        " WHERE object_type IN ('PROCEDURE','FUNCTION')\n";
  if(!p_schema.IsEmpty())
  {
    sql += "   AND owner = '" + p_schema + "'\n";
  }
  if(!p_procedure.IsEmpty())
  {
    sql += "   AND object_name ";
    sql += p_procedure.Find('%') >= 0 ? "LIKE '" : "= '";
    sql += p_procedure + "'\n";
  }
  sql += "ORDER BY 1,2,3";
  return sql;
}

CString
SQLInfoOracle::GetPSMProcedureSourcecode(CString p_schema, CString p_procedure) const
{
  p_schema.MakeUpper();
  p_procedure.MakeUpper();
  CString sql;

  sql = "SELECT 0 as object_id\n"
        "      ,0 as object_line\n"
        "      ,text\n"
        "  FROM dba_source\n";
  if(!p_schema.IsEmpty())
  {
    sql += " WHERE owner = '" + p_schema + "'\n";
  }
  if(!p_procedure.IsEmpty())
  {
    sql += p_schema.IsEmpty() ? " WHERE " : "   AND ";
    sql += " name  = '" + p_procedure + "'\n";
  }
  sql += " ORDER BY owner,name,line";
  return sql;
}

CString
SQLInfoOracle::GetPSMProcedureCreate(MetaProcedure& /*p_procedure*/) const
{
  return "";
}

CString
SQLInfoOracle::GetPSMProcedureDrop(CString p_schema, CString p_procedure) const
{
  return "";
}

CString
SQLInfoOracle::GetPSMProcedureErrors(CString p_schema,CString p_procedure) const
{
  CString query;
  CString errorText;
  p_schema.MakeUpper();
  p_procedure.MakeUpper();
  query = "SELECT line\n"
          "      ,position\n"
          "      ,text\n"
          "  FROM user_errors\n"
          " WHERE name = '" + p_procedure +"'";

  SQLQuery qry1(m_database);
  SQLQuery qry2(m_database);
  qry1.DoSQLStatement(query);

  while (qry1.GetRecord())
  {
    CString s = qry1.GetColumn(3)->GetAsChar();
    if(s.Find("Statement ignored") < 0) 
    {
      s.Format("Error in line %d, column %d: %s\n",qry1.GetColumn(1)->GetAsSLong()
	                                              ,qry1.GetColumn(2)->GetAsSLong()
						                          ,qry1.GetColumn(3)->GetAsChar());
      errorText += s;
      query.Format( "SELECT text\n"
                    "  FROM dba_source\n"
                    " WHERE type = 'FUNCTION'\n"
                    "   AND name = '%s'\n"
                    "   AND line = %d"
                   ,p_procedure.GetString()
                   ,qry1.GetColumn(1)->GetAsSLong());
      qry2.DoSQLStatement(query);
      while(qry2.GetRecord())
      {
        s.Format("Line %d: %s\n"
                 ,qry1.GetColumn(1)->GetAsSLong()
                 ,qry2.GetColumn(1)->GetAsChar());
        errorText += s;
      }
    }
  }
  return errorText;
}

// And it's parameters
CString
SQLInfoOracle::GetPSMProcedureParameters(CString p_schema,CString p_procedure) const
{
  return "";
}

//////////////////////////////////////////////////////////////////////////
//
// ALL PSM LANGUAGE ELEMENTS
//
//////////////////////////////////////////////////////////////////////////

CString
SQLInfoOracle::GetPSMDeclaration(bool    p_first
                                ,CString p_variable
                                ,int     p_datatype
                                ,int     p_precision /*= 0 */
                                ,int     p_scale     /*= 0 */
                                ,CString p_default   /*= ""*/
                                ,CString p_domain    /*= ""*/
                                ,CString p_asColumn  /*= ""*/) const
{
  CString line;

  if(p_first)
  {
    line = "DECLARE\n";
  }
  line += p_variable + " ";

  if(p_datatype)
  {
    // Getting type info and name
    TypeInfo* info = GetTypeInfo(p_datatype);
    line += info->m_type_name;

    if(p_precision > 0)
    {
      line.AppendFormat("(%d",p_precision);
      if(p_scale > 0)
      {
        line.AppendFormat("%d",p_scale);
      }
      line += ")";
    }

    if(!p_default.IsEmpty())
    {
      line += " DEFAULT " + p_default;
    }
  }
  else if(!p_domain.IsEmpty())
  {
    line += p_domain;
  }
  else if(!p_asColumn)
  {
    line += p_asColumn + "%TYPE";
  }
  line += ";\n";
  return line;
}

CString
SQLInfoOracle::GetPSMAssignment(CString p_variable,CString p_statement /*=""*/) const
{
  CString line(p_variable);
  line += " := ";
  if(!p_statement.IsEmpty())
  {
    line += p_statement;
    line += ";";
  }
  return line;
}

CString
SQLInfoOracle::GetPSMIF(CString p_condition) const
{
  CString line("IF (");
  line += p_condition;
  line += ") THEN\n";
  return line;
}

CString
SQLInfoOracle::GetPSMIFElse() const
{
  return "ELSE\n";
}

CString
SQLInfoOracle::GetPSMIFEnd() const
{
  return "END IF;\n";
}

CString
SQLInfoOracle::GetPSMWhile(CString p_condition) const
{
  return "WHILE (" + p_condition + ") LOOP\n";
}

CString
SQLInfoOracle::GetPSMWhileEnd() const
{
  return "END LOOP;\n";
}

CString
SQLInfoOracle::GetPSMLOOP() const
{
  return "LOOP\n";
}

CString
SQLInfoOracle::GetPSMLOOPEnd() const
{
  return "END LOOP;\n";
}

CString
SQLInfoOracle::GetPSMBREAK() const
{
  return "EXIT;\n";
}

CString
SQLInfoOracle::GetPSMRETURN(CString p_statement /*= ""*/) const
{
  CString line("RETURN");
  if(!p_statement.IsEmpty())
  {
    line += " (" + p_statement + ")";
  }
  line += ";\n";
  return line;
}

CString
SQLInfoOracle::GetPSMExecute(CString p_procedure,MParameterMap& p_parameters) const
{
  CString line;
  line.Format("EXECUTE %s(",p_procedure.GetString());
  bool doMore = false;

  for(auto& param : p_parameters)
  {
    if(doMore) line += ",";
    doMore = true;

    line += param.m_parameter;
  }
  line += ");\n";
  return line;
}

// The CURSOR
CString
SQLInfoOracle::GetPSMCursorDeclaration(CString p_cursorname,CString p_select) const
{
  return "CURSOR " + p_cursorname + " IS " + p_select + ";";
}

CString
SQLInfoOracle::GetPSMCursorFetch(CString p_cursorname,std::vector<CString>& /*p_columnnames*/,std::vector<CString>& p_variablenames) const
{
  bool moreThenOne = false;
  CString query = "OPEN  " + p_cursorname + ";\n"
                  "FETCH " + p_cursorname + " INTO ";

  for(auto& var : p_variablenames)
  {
    if(moreThenOne) query += ",";
    moreThenOne = true;
    query += var;
  }
  query += ";";
  return query;
}

//////////////////////////////////////////////////////////////////////////
// PSM Exceptions

CString
SQLInfoOracle::GetPSMExceptionCatchNoData() const
{
  return "EXCEPTION WHEN NO_DATA_FOUND THEN\n";
  // followed by STATEMENTBLOCK and "END;"
}

CString
SQLInfoOracle::GetPSMExceptionCatch(CString p_sqlState) const
{
  return "EXCEPTION WHEN " + p_sqlState + " THEN\n";
  // followed by STATEMENTBLOCK and "END;"
}

CString
SQLInfoOracle::GetPSMExceptionRaise(CString p_sqlState) const
{
  return "RAISE " + p_sqlState;
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
SQLInfoOracle::GetSESSIONMyself() const
{
  CString sql = "SELECT audsid\n"
                "      ,osuser\n"
                "      ,terminal\n"
                "      ,prev_exec_start\n" // moment
                "      ,machine\n"         // remote 
                "      ,program\n"         // process name
                "      ,process\n"         // process ID
                "  FROM v$session\n"
                " WHERE type   = 'USER'\n"
                "   AND audsid = sys_context('userenv','sessionid')";
  return sql;
}

CString
SQLInfoOracle::GetSESSIONExists(CString p_sessionID) const
{
  return "SELECT COUNT(*)\n"
         "  FROM v$session\n"
         " WHERE audsid = " + p_sessionID;
}

CString
SQLInfoOracle::GetSESSIONList() const
{
  return GetSESSIONAttributes("");
}

CString
SQLInfoOracle::GetSESSIONAttributes(CString p_sessionID) const
{
  CString sql = "SELECT audsid\n"
                "      ,osuser\n"
                "      ,terminal\n"
                "      ,prev_exec_start\n" // moment
                "      ,machine\n"         // remote 
                "      ,program\n"         // process name
                "      ,process\n"         // process ID
                "  FROM v$session\n"
                " WHERE type = 'USER'\n";
  if(!p_sessionID.IsEmpty())
  {
    sql += "\n   AND audsid = " + p_sessionID;
  }
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// Transactions

CString
SQLInfoOracle::GetSESSIONConstraintsDeferred() const
{
  // Set to DEFERRED for Oracle
  return "ALTER SESSION SET CONSTRAINTS = DEFERRED";
}

CString
SQLInfoOracle::GetSESSIONConstraintsImmediate() const
{
  // Set to IMMEDIATE for Oracle
  return "ALTER SESSION SET CONSTRAINTS = IMMEDIATE";
}

//////////////////////////////////////////////////////////////////////////
//
// Call FUNCTION/PROCEDURE from within program
// As a RDBMS dependent extension of "DoSQLCall" of the SQLQuery object
//
//////////////////////////////////////////////////////////////////////////

// Calling a stored function or procedure if the RDBMS does not support ODBC call escapes
SQLVariant*
SQLInfoOracle::DoSQLCall(SQLQuery* /*p_query*/,CString& /*p_schema*/,CString& /*p_procedure*/)
{
  return nullptr;
}

// End of namespace
}
