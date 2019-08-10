////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoFirebird.cpp
//
// Copyright (c) 1998-2019 ir. W.E. Huisman
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
// Last Revision:  15-06-2019
// Version number: 1.5.5
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

//////////////////////////////////////////////////////////////////////////
//
// GENERALS (Strings & Booleans) 
//
//////////////////////////////////////////////////////////////////////////

// Get the database type
// DatabaseType GetDatabaseType() const;
DatabaseType
SQLInfoFirebird::GetRDBMSDatabaseType() const
{
  return RDBMS_FIREBIRD;
}

// The name of the database vendor
CString
SQLInfoFirebird::GetRDBMSVendorName() const
{
  // The name of the database vendor
  return "Firebird";
}

// Get the physical database name
CString
SQLInfoFirebird::GetRDBMSPhysicalDatabaseName() const
{
  // See to it that "SQLDatabase:GetDatabaseName" does it's work
  return m_database->GetDatabaseName();
}

// System catalog is stored in uppercase in the database?
bool
SQLInfoFirebird::GetRDBMSIsCatalogUpper() const
{
  return true;
}

// System catalog supports full ISO schemas (same tables per schema)
bool
SQLInfoFirebird::GetRDBMSUnderstandsSchemas() const
{
  return false;
}

// Supports database/ODBCdriver comments in SQL
bool
SQLInfoFirebird::GetRDBMSSupportsComments() const
{
  return false;
}

// Database can defer constraints until the end of a transaction
bool
SQLInfoFirebird::GetRDBMSSupportsDeferredConstraints() const
{
  // NO CONSTRAINTS DEFERRED
  return true;
}

// Database has ORDER BY with an expression, e.g. ORDER BY UPPER(columnname)
bool
SQLInfoFirebird::GetRDBMSSupportsOrderByExpression() const
{
  return true;
}

// Supports the ODBC escape sequence {[?=] CALL procedure (?,?,?)}
bool
SQLInfoFirebird::GetRDBMSSupportsODBCCallEscapes() const
{
  return false;
}

// If the database does not support the datatype TIME, it can be implemented as a DECIMAL
bool
SQLInfoFirebird::GetRDBMSSupportsDatatypeTime() const
{
  // Time can be implemented as TIME
  return true;
}

// If the database does not support the datatype INTERVAL, it can be implemented as a DECIMAL
bool
SQLInfoFirebird::GetRDBMSSupportsDatatypeInterval() const
{
  // Interval not supported, can be implemented as DECIMALS
  return false;
}

// Gets the maximum length of an SQL statement
unsigned long 
SQLInfoFirebird::GetRDBMSMaxStatementLength() const
{
  // No Limit
  return 0;
}

// Database must commit DDL commands in a transaction
bool
SQLInfoFirebird::GetRDBMSMustCommitDDL() const
{
  return true;
}

// KEYWORDS

// Keyword for the current date and time
CString
SQLInfoFirebird::GetKEYWORDCurrentTimestamp() const
{
  return "current_timestamp";
}

// String for the current date
CString
SQLInfoFirebird::GetKEYWORDCurrentDate() const
{
  return "current_date";
}

// Get the concatenation operator
CString
SQLInfoFirebird::GetKEYWORDConcatanationOperator() const
{
  return "||";
}

// Get quote character for strings
CString
SQLInfoFirebird::GetKEYWORDQuoteCharacter() const
{
  return "\'";
}

// Get default NULL for parameter list input
CString
SQLInfoFirebird::GetKEYWORDParameterDefaultNULL() const
{
  // Standard, no definition defines the NULL state
  return "";
}

// Parameter is for INPUT and OUTPUT in parameter list
CString
SQLInfoFirebird::GetKEYWORDParameterINOUT() const
{
  // Firebird works with the "RETURNS" list !
  return "";
}

// Parameter is for OUTPUT only in parameter list
CString
SQLInfoFirebird::GetKEYWORDParameterOUT() const
{
  return "";
}

// Get datatype of the IDENTITY primary key in a Network database
CString
SQLInfoFirebird::GetKEYWORDNetworkPrimaryKeyType() const
{
  // Use SEQUENCE / GENERATOR to fill!
  return "INTEGER";
}

// Get datatype for timestamp (year to second)
CString
SQLInfoFirebird::GetKEYWORDTypeTimestamp() const
{
  return "TIMESTAMP";
}

// Prefix for a parameter in a stored procedure
CString
SQLInfoFirebird::GetKEYWORDParameterPrefix() const
{
  return ":";
}

// Get select part to add new record identity to a table
// Can be special column like 'OID' or a sequence select
CString
SQLInfoFirebird::GetKEYWORDIdentityString(CString& p_tablename,CString p_postfix /*= "_seq"*/) const
{
  return "GEN_ID(" + p_tablename + p_postfix + ",1)";
}

// Gets the UPPER function
CString
SQLInfoFirebird::GetKEYWORDUpper(CString& p_expression) const
{
  return "UPPER(" + p_expression + ")";
}

// Gets the construction for 1 minute ago
CString
SQLInfoFirebird::GetKEYWORDInterval1MinuteAgo() const
{
  // Not supported by Firebird
  return "ERROR";
}

// Gets the Not-NULL-Value statement of the database
CString
SQLInfoFirebird::GetKEYWORDStatementNVL(CString& p_test,CString& p_isnull) const
{
  return "{fn IFNULL(" + p_test + "," + p_isnull + ")}";
}

// Gets the construction for inline generating a key within an INSERT statement
CString
SQLInfoFirebird::GetSQLNewSerial(CString p_table, CString p_sequence) const
{
  CString sequence(p_sequence);
  if (sequence.IsEmpty() && !p_table.IsEmpty())
  {
    sequence = p_table + "_seq";
  }

  // Select next value from a generator sequence
  return "(next value for " + sequence + ")";
}

// Gets the construction / select for generating a new serial identity
CString
SQLInfoFirebird::GetSQLGenerateSerial(CString p_table) const
{
  return "SELECT (next value for " + p_table + "_seq) FROM RDB$DATABASE";
}

// Gets the construction / select for the resulting effective generated serial
CString
SQLInfoFirebird::GetSQLEffectiveSerial(CString p_identity) const
{
  // Just return it, it's the correct value
  return p_identity;
}

// Gets the sub-transaction commands
CString
SQLInfoFirebird::GetSQLStartSubTransaction(CString p_savepointName) const
{
  return CString("SAVEPOINT ") + p_savepointName;
}

CString
SQLInfoFirebird::GetSQLCommitSubTransaction(CString p_savepointName) const
{
  return CString("COMMIT TRANSACTION ") + p_savepointName;
}

CString
SQLInfoFirebird::GetSQLRollbackSubTransaction(CString p_savepointName) const
{
  return CString("ROLLBACK TO ") + p_savepointName;
}

// FROM-Part for a query to select only 1 (one) record / or empty!
CString
SQLInfoFirebird::GetSQLFromDualClause() const
{
  return " FROM rdb$database";
}

// Get SQL to lock  a table 
CString
SQLInfoFirebird::GetSQLLockTable(CString p_schema, CString p_tablename, bool /*p_exclusive*/) const
{
  // Firebird does NOT have a LOCK-TABLE statement
  return "";
}

// Get query to optimize the table statistics
CString
SQLInfoFirebird::GetSQLOptimizeTable(CString p_schema, CString p_tablename) const
{
  // Firebird has no SQL for this, it uses the "GFIX.EXE -sweep <database>" tool
  return "";
}

// Transform query to select top <n> rows
CString
SQLInfoFirebird::GetSQLTopNRows(CString p_sql,int p_top) const
{
  if(p_top > 0 && p_sql.Find("SELECT ") == 0)
  {
    CString selectFirst;
    selectFirst.Format("SELECT FIRST %d ",p_top);

    p_sql.Replace("SELECT ",selectFirst);
  }
  return p_sql;
}

//////////////////////////////////////////////////////////////////////////
//
// SQL STRINGS
//
//////////////////////////////////////////////////////////////////////////

CString
SQLInfoFirebird::GetSQLString(const CString& p_string) const
{
  CString s = p_string;
  s.Replace("'","''");
  CString kwoot = GetKEYWORDQuoteCharacter();
  return  kwoot + s + kwoot;
}

CString
SQLInfoFirebird::GetSQLDateString(int p_year,int p_month,int p_day) const
{
  CString retval;
  retval.Format("CAST '%02d/%02d/%04d' AS DATE)",p_day,p_month,p_year); // American order!!
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
//   - Alter (where possible)
//   - Drop
//
//////////////////////////////////////////////////////////////////////////

// Meta info about meta types
// Standard ODBC functions are good enough
CString 
SQLInfoFirebird::GetCATALOGMetaTypes(int p_type) const
{
  UNREFERENCED_PARAMETER(p_type);
  return "";
}

// Get SQL to check if a table already exists in the database
CString
SQLInfoFirebird::GetCATALOGTableExists(CString /*p_schema*/,CString p_tablename) const
{
  p_tablename.MakeUpper();
  CString query = "SELECT COUNT(*)\n"
                  "  FROM rdb$relations\n"
                  " WHERE rdb$relation_name = '" + p_tablename + "'";
  return query;
}

CString
SQLInfoFirebird::GetCATALOGTablesList(CString p_schema,CString p_pattern) const
{
  return GetCATALOGTableAttributes(p_schema,p_pattern);
}

// Attributes can fill in 'm_temporary' 
CString
SQLInfoFirebird::GetCATALOGTableAttributes(CString /*p_schema*/,CString p_tablename) const
{
  p_tablename.MakeUpper();
  CString sql = "SELECT CAST('' AS VARCHAR(31))  AS table_catalog\n"
                "      ,trim(rdb$owner_name)     AS table_schema\n"
                "      ,trim(rdb$relation_name)  AS table_name\n"
                "      ,CASE rdb$relation_type\n"
                "            WHEN 0 THEN 'TABLE'\n"
                "            WHEN 2 THEN 'TABLE'\n"
                "            WHEN 4 THEN 'GLOBAL TEMPORARY'\n"
                "            WHEN 5 THEN 'LOCAL TEMPORARY'\n"
                "                   ELSE 'UNKNOWN'\n"
                "       END               AS table_type\n"
                "      ,trim(rdb$description) AS remarks\n"
                "      ,trim(rdb$owner_name) || '.' || trim(rdb$relation_name) AS full_name\n"
                "      ,cast('' as varchar(31)) as storage_space\n"
                "      ,CASE rdb$relation_type\n"
                "            WHEN 4 THEN 1\n"
                "            WHEN 5 THEN 1\n"
                "                   ELSE 0\n"
                "       END  AS temporary_table\n"
                "  FROM rdb$relations\n"
                " WHERE rdb$system_flag = 0\n"
                "   AND rdb$relation_type IN (0,2,4,5)\n";
  if(!p_tablename.IsEmpty())
  {
    sql += "\n   AND rdb$relation_name ";
    sql += (p_tablename.Find("%") >= 0) ? "LIKE '" : "= '";
    sql += p_tablename + "'";
  }
  return sql;
}

CString
SQLInfoFirebird::GetCATALOGTableSynonyms(CString /*p_schema*/,CString p_tablename) const
{
  p_tablename.MakeUpper();
  CString sql = "SELECT CAST('' AS varchar(31))             AS table_catalog\n"
                "      ,trim(rdb$owner_name)                AS table_schema\n"
                "      ,trim(rdb$relation_name)             AS table_name\n"
                "      ,CAST('SYSTEM TABLE' as varchar(31)) AS table_type\n"
                "      ,rdb$description                     AS remarks\n"
                "      ,trim(rdb$owner_name) || '.' || trim(rdb$relation_name) AS full_name\n"
                "      ,cast('' as varchar(31))             AS storage_space\n"
                "  FROM rdb$relations";
  if(!p_tablename.IsEmpty())
  {
    sql += "\n WHERE rdb$relation_name ";
    sql += (p_tablename.Find("%") >= 0) ? "LIKE '" : "= '";
    sql += p_tablename + "'";
  }
  return sql;
}

CString
SQLInfoFirebird::GetCATALOGTableCatalog(CString /*p_schema*/,CString p_tablename) const
{
  p_tablename.MakeUpper();
  CString sql = "SELECT CAST('' AS varchar(31))             AS table_catalog\n"
                "      ,trim(rdb$owner_name)                AS table_schema\n"
                "      ,trim(rdb$relation_name)             AS table_name\n"
                "      ,CAST('SYSTEM TABLE' as varchar(31)) AS table_type\n"
                "      ,rdb$description                     AS remarks\n"
                "      ,trim(rdb$owner_name) || '.' || trim(rdb$relation_name) AS full_name\n"
                "      ,cast('' as varchar(31)) as storage_space\n"
                "  FROM rdb$relations\n"
                " WHERE rdb$relation_type IN (0,3)\n"
                "   AND rdb$system_flag = 1\n";
  if(!p_tablename.IsEmpty())
  {
    sql += "\n   AND rdb$relation_name ";
    sql += (p_tablename.Find("%") >= 0) ? "LIKE '" : "= '";
    sql += p_tablename + "'";
  }
  return sql;
}

CString
SQLInfoFirebird::GetCATALOGTableCreate(MetaTable& /*p_table*/,MetaColumn& /*p_column*/) const
{
  return "";
}

CString
SQLInfoFirebird::GetCATALOGTableRename(CString /*p_schema*/,CString /*p_oldName*/,CString /*p_newName*/) const
{
  // Not supported by Firebird
  // Changing database table names is not supported by this database type
  return "";
}

CString
SQLInfoFirebird::GetCATALOGTableDrop(CString /*p_schema*/,CString p_tablename) const
{
  return "DROP TABLE " + p_tablename;
}

//////////////////////////////////////////////////////////////////////////
// ALL TEMPORARY TABLE FUNCTIONS

CString 
SQLInfoFirebird::GetCATALOGTemptableCreate(CString /*p_schema*/,CString p_tablename,CString p_select) const
{
  return "CREATE GLOBAL TEMPORARY TABLE " + p_tablename + "\nAS " + p_select +
         "ON COMMIT PRESERVE ROWS";
}

CString 
SQLInfoFirebird::GetCATALOGTemptableIntoTemp(CString /*p_schema*/,CString p_tablename,CString p_select) const
{
  return "INSERT INTO " + p_tablename + "\n" + p_select;
}

CString 
SQLInfoFirebird::GetCATALOGTemptableDrop(CString /*p_schema*/,CString p_tablename) const
{
  return "DROP TABLE " + p_tablename;
}

//////////////////////////////////////////////////////////////////////////
// ALL COLUMN FUNCTIONS

CString 
SQLInfoFirebird::GetCATALOGColumnExists(CString p_schema,CString p_tablename,CString p_columnname) const
{
  p_tablename.MakeUpper();
  p_columnname.MakeUpper();
  
  CString query = "SELECT COUNT(*)\n"
                  "  FROM rdb$relation_fields\n"
                  " WHERE rdb$relation_name = '" + p_tablename  + "'\n"
                  "   AND rdb$field_name    = '" + p_columnname + "'";
  return query;
}

CString 
SQLInfoFirebird::GetCATALOGColumnList(CString p_schema,CString p_tablename) const
{
  return GetCATALOGColumnAttributes(p_schema,p_tablename,"");
}

CString 
SQLInfoFirebird::GetCATALOGColumnAttributes(CString /*p_schema*/,CString p_tablename,CString p_columnname) const
{
  p_tablename.MakeUpper();
  p_columnname.MakeUpper();
  CString sql;

  sql = "SELECT cast('' as varchar(255))    as table_catalog\n"         // 1  - VARCHAR
        "      ,trim(tbl.rdb$owner_name)    as table_schema\n"	         // 2  - VARCHAR
        "      ,trim(col.rdb$relation_name) as table_name\n"            // 3  - VARCHAR NOT NULL
        "      ,trim(col.rdb$field_name)    as column_name\n"           // 4  - VARCHAR NOT NULL
        "      ,CASE fld.rdb$field_type\n"
        "            WHEN 7  THEN CASE fld.rdb$field_sub_type\n"
        "                              WHEN 1 THEN 2\n"
        "                              WHEN 2 THEN 2\n"
        "                                     ELSE 5\n"
        "                              END\n"
        "            WHEN 8  THEN CASE fld.rdb$field_sub_type\n"
        "                              WHEN 1 THEN 2\n"
        "                              WHEN 2 THEN 2\n"
        "                                     ELSE 4\n"
        "                              END\n"
        "            WHEN 10 THEN 7\n"
        "            WHEN 12 THEN 10\n"
        "            WHEN 13 THEN 13\n"
        "            WHEN 14 THEN 1\n"
        "            WHEN 16 THEN CASE fld.rdb$field_sub_type\n"
        "                              WHEN 1 THEN 2\n"
        "                              WHEN 2 THEN 2\n"
        "                                     ELSE -5\n"
        "                         END\n"
        "            WHEN 23 THEN -7\n"
        "            WHEN 27 THEN 8\n"
        "            WHEN 35 THEN 11\n"
        "            WHEN 37 THEN 12\n"
        "            WHEN 261 THEN CASE fld.rdb$field_sub_type\n"
        "                               WHEN 0 THEN -4\n"
        "                               WHEN 1 THEN -1\n"
        "                                      ELSE -10\n"
        "                          END\n"
        "                     ELSE 0\n"
        "       END                           as data_type\n"	          // 5  - SMALLINT NOT NULL
        "      ,CASE fld.rdb$field_type\n"
        "            WHEN 7  THEN CASE fld.rdb$field_sub_type\n"
        "                              WHEN 1 THEN 'NUMERIC'\n"
        "                              WHEN 2 THEN 'DECIMAL'\n"
        "                                     ELSE 'SMALLINT'\n"
        "                              END\n"
        "            WHEN 8  THEN CASE fld.rdb$field_sub_type\n"
        "                              WHEN 1 THEN 'NUMERIC'\n"
        "                              WHEN 2 THEN 'DECIMAL'\n"
        "                                     ELSE 'INTEGER'\n"
        "                              END\n"
        "            WHEN 10 THEN 'FLOAT'\n"
        "            WHEN 12 THEN 'DATE'\n"
        "            WHEN 13 THEN 'TIME'\n"
        "            WHEN 14 THEN 'CHAR'\n"
        "            WHEN 16 THEN CASE fld.rdb$field_sub_type\n"
        "                              WHEN 1 THEN 'NUMERIC'\n"
        "                              WHEN 2 THEN 'DECIMAL'\n"
        "                                     ELSE 'BIGINT'\n"
        "                         END\n"
        "            WHEN 23 THEN 'BOOLEAN'\n"
        "            WHEN 27 THEN 'DOUBLE PRECISION'\n"
        "            WHEN 35 THEN 'TIMESTAMP'\n"
        "            WHEN 37 THEN 'VARCHAR'\n"
        "            WHEN 261 THEN CASE fld.rdb$field_sub_type\n"
        "                               WHEN 0 THEN 'BLOB SUB_TYPE 0'\n"
        "                               WHEN 1 THEN 'BLOB SUB_TYPE TEXT'\n"
        "                                      ELSE 'BLOB'\n"
        "                          END\n"
        "                     ELSE 'UNKNOWN'\n"
        "       END                                        as type_name\n"		// 6  - VARCHAR NOT NULL
        "      ,CASE fld.rdb$field_type\n"
        "            WHEN 7  THEN CASE fld.rdb$field_sub_type\n"
        "                              WHEN 1 THEN fld.rdb$field_precision\n"
        "                              WHEN 2 THEN fld.rdb$field_precision\n"
        "                                     ELSE 5\n"
        "                              END\n"
        "            WHEN 8  THEN CASE fld.rdb$field_sub_type\n"
        "                              WHEN 1 THEN fld.rdb$field_precision\n"
        "                              WHEN 2 THEN fld.rdb$field_precision\n"
        "                                     ELSE 11\n"
        "                              END\n"
        "            WHEN 10 THEN 12\n"
        "            WHEN 12 THEN 10\n"
        "            WHEN 13 THEN 13\n"
        "            WHEN 14 THEN fld.rdb$field_length\n"
        "            WHEN 16 THEN CASE fld.rdb$field_sub_type\n"
        "                              WHEN 1 THEN fld.rdb$field_precision\n"
        "                              WHEN 2 THEN fld.rdb$field_precision\n"
        "                                     ELSE 22\n"
        "                         END\n"
        "            WHEN 23 THEN 1\n"
        "            WHEN 27 THEN 20\n"
        "            WHEN 35 THEN 26\n"
        "            WHEN 37 THEN fld.rdb$field_length\n"
        "            WHEN 261 THEN 2147483647\n"
        "       END                                        as column_size\n"      		    // 7  - INTEGER
        "      ,cast(fld.rdb$field_length as integer)      as buffer_length\n"	          // 8  - INTEGER
        "      ,cast (fld.rdb$field_scale as smallint)*-1  as decimal_digits\n"		        // 9  - SMALLINT
        "      ,10                                         as num_prec_radix\n"           // 10 - SMALLINT
        "      ,(coalesce(col.rdb$null_flag,0,0)-1)*-1     as nullable\n"        				  // 11 - SMALLINT NOT NULL
        "      ,trim(col.rdb$description)                  as remarks\n"                  // 12 - VARCHAR
        "      ,trim(col.rdb$default_source)               as column_def\n"               // 13 - VARCHAR
        "      ,CASE fld.rdb$field_type\n"
        "            WHEN 7  THEN CASE fld.rdb$field_sub_type\n"
        "                              WHEN 1 THEN 2\n"
        "                              WHEN 2 THEN 2\n"
        "                                     ELSE 5\n"
        "                              END\n"
        "            WHEN 8  THEN CASE fld.rdb$field_sub_type\n"
        "                              WHEN 1 THEN 2\n"
        "                              WHEN 2 THEN 2\n"
        "                                     ELSE 4\n"
        "                              END\n"
        "            WHEN 10 THEN 7\n"
        "            WHEN 12 THEN 9\n"
        "            WHEN 13 THEN 10\n"
        "            WHEN 14 THEN 1\n"
        "            WHEN 16 THEN CASE fld.rdb$field_sub_type\n"
        "                              WHEN 1 THEN 2\n"
        "                              WHEN 2 THEN 2\n"
        "                                     ELSE -5\n"
        "                         END\n"
        "            WHEN 23 THEN -7\n"
        "            WHEN 27 THEN 8\n"
        "            WHEN 35 THEN 11\n"
        "            WHEN 37 THEN 12\n"
        "            WHEN 261 THEN CASE fld.rdb$field_sub_type\n"
        "                               WHEN 0 THEN -4\n"
        "                               WHEN 1 THEN -1\n"
        "                                      ELSE -10\n"
        "                          END\n"
        "                     ELSE 0\n"
        "       END                                         as sql_data_type\n" 			    // 14 - SMALLINT NOT NULL
        "      ,CAST(0 AS SMALLINT)                         as sql_datetime_sub\n"        // 15 - SMALLINT
        "      ,fld.rdb$field_length / rdb$character_length as char_octet_length\n"    		// 16 - INTEGER
        "      ,col.rdb$field_position + 1                  as ordinal_position\n"				// 17 - INTEGER NOT NULL
        "      ,CASE (coalesce(col.rdb$null_flag,0,0)-1)*-1\n"
        "            WHEN 0 THEN 'NO'\n"
        "            WHEN 1 THEN 'YES'\n"
        "                   ELSE 'UNKNOWN'\n"
        "       END                                         AS is_nullable\n"             // 18 - VARCHAR
        "  FROM rdb$relation_fields  col\n"
        "      ,rdb$fields           fld\n"
        "      ,rdb$relations        tbl\n"
        " WHERE col.rdb$field_source  = fld.rdb$field_name\n"
        "     AND col.rdb$relation_name = tbl.rdb$relation_name\n"
        "     AND tbl.rdb$relation_name = '" + p_tablename + "'\n";
  // Optionally add the column name
  if(!p_columnname.IsEmpty())
  {
    sql += "   AND col.rdb$field_name = '" + p_columnname + "'\n";
  }
  sql += " ORDER BY col.rdb$field_position";
  return sql;
}

CString SQLInfoFirebird::GetCATALOGColumnCreate(MetaColumn& p_column) const
{
  CString sql  = "ALTER TABLE "  + p_column.m_table  + "\n"
                 "  ADD COLUMN " + p_column.m_column + " " + p_column.m_typename;
  p_column.GetPrecisionAndScale(sql);
  p_column.GetNullable(sql);
  return sql;
}

CString SQLInfoFirebird::GetCATALOGColumnAlter(MetaColumn& p_column) const
{
  // The extra 'TYPE' keyword  is a-typical
  // The SET/DROP for the NULL is a-typical
  CString sql  = "ALTER TABLE "  + p_column.m_table  + "\n"
                 "      MODIFY COLUMN " + p_column.m_column + " TYPE " + p_column.m_typename;
  p_column.GetPrecisionAndScale(sql);
  if(p_column.m_nullable)
  {
    sql += " DROP NOT NULL";
  }
  else
  {
    sql += " SET NOT NULL";
  }
  return sql;
}

CString 
SQLInfoFirebird::GetCATALOGColumnRename(CString /*p_schema*/,CString p_tablename,CString p_columnname,CString p_newname,CString /*p_datatype*/) const
{
  CString sql("ALTER  TABLE  " + p_tablename + "\n"
              "RENAME " + p_columnname + " TO " + p_newname + "\n");
  return sql;
}


CString 
SQLInfoFirebird::GetCATALOGColumnDrop(CString p_schema,CString p_tablename,CString p_columnname) const
{
  CString sql("ALTER TABLE " + p_tablename + "\n"
              " DROP COLUMN " + p_columnname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL INDICES FUNCTIONS

// All index functions
CString
SQLInfoFirebird::GetCATALOGIndexExists(CString /*p_schema*/,CString /*p_tablename*/,CString p_indexname) const
{
  p_indexname.MakeUpper();
  CString sql = "SELECT COUNT(*)\n"
                "  FROM rdb$indices\n"
                " WHERE rdb$index_name = '" + p_indexname + "'";
  return sql;
}

CString
SQLInfoFirebird::GetCATALOGIndexList(CString p_schema,CString p_tablename) const
{
  return GetCATALOGIndexAttributes(p_schema,p_tablename,"");
}

CString
SQLInfoFirebird::GetCATALOGIndexAttributes(CString /*p_schema*/,CString p_tablename,CString p_indexname) const
{
  p_tablename.MakeUpper();
  p_indexname.MakeUpper();

  CString query = "SELECT CAST('' as varchar(31))     as index_catalog\n"
                  "      ,TRIM(tab.rdb$owner_name)    as index_schema\n"
                  "      ,TRIM(idx.rdb$relation_name) as index_table\n"
                  "      ,idx.rdb$unique_flag         as index_unique\n"
                  "      ,TRIM(idx.rdb$index_name)    as index_name\n"
                  "      ,1                           as index_type\n"
                  "      ,col.rdb$field_position + 1  as index_position\n"
                  "      ,TRIM(col.rdb$field_name)    as column_name\n"
                  "      ,CASE idx.rdb$index_type\n"
                  "            WHEN 0 THEN 'A'\n"
                  "            WHEN 1 THEN 'D'\n"
                  "                   ELSE 'A'\n"
                  "       END                         as index_ascending\n"
                  "      ,idx.rdb$statistics          as cardinality\n"
                  "      ,col.rdb$statistics          as index_pages\n"
                  "      ,trim(idx.rdb$expression_source) as index_filter\n"
                  "  FROM rdb$indices        idx\n"
                  "      ,rdb$index_segments col\n"
                  "      ,rdb$relations      tab\n"
                  " WHERE idx.rdb$index_name    = col.rdb$index_name\n"
                  "   AND idx.rdb$relation_name = tab.rdb$relation_name\n"
                  "   AND idx.rdb$system_flag   = 0\n"
                  "   AND idx.rdb$relation_name = '" + p_tablename + "'\n";
  if(!p_indexname.IsEmpty())
  {
    query += "   AND idx.rdb$index_name = '" + p_indexname + "'\n";
  }
  query += " ORDER BY 6";
  return query;
}

CString
SQLInfoFirebird::GetCATALOGIndexCreate(MIndicesMap& p_indices) const
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

CString
SQLInfoFirebird::GetCATALOGIndexDrop(CString /*p_schema*/,CString /*p_tablename*/,CString p_indexname) const
{
  CString sql = "DROP INDEX " + p_indexname;
  return sql;
}

// Get extra filter expression for an index column
CString
SQLInfoFirebird::GetCATALOGIndexFilter(MetaIndex& /*p_index*/) const
{
  return "";
}

//////////////////////////////////////////////////////////////////////////
// ALL PRIMARY KEY FUNCTIONS

CString
SQLInfoFirebird::GetCATALOGPrimaryExists(CString /*p_schema*/,CString p_tablename) const
{
  p_tablename.MakeUpper();
  CString query = "SELECT COUNT(*)\n"
                  "  FROM rdb$relation_constraints\n"
                  " WHERE rdb$relation_name   = '" + p_tablename + "'\n"
                  "   AND rdb$constraint_type = 'PRIMARY KEY'";
  return query;
}

CString
SQLInfoFirebird::GetCATALOGPrimaryAttributes(CString /*p_schema*/,CString p_tablename) const
{
  p_tablename.MakeUpper();
  CString sql = "SELECT cast('' as varchar(31))       as catalog_name\n"
                "      ,cast('' as varchar(31))       as schema_name\n"
                "      ,trim(con.rdb$relation_name)   as table_name\n"
                "      ,trim(ind.rdb$field_name)      as column_name\n"
                "      ,ind.rdb$field_position + 1    as col_position\n"
                "      ,trim(con.rdb$constraint_name) as col_constraint\n"
                "      ,trim(con.rdb$index_name)      as index_name\n"
                "      ,con.rdb$deferrable\n"
                "      ,con.rdb$initially_deferred\n"
                "  FROM rdb$relation_constraints con\n"
                "      ,rdb$index_segments ind\n"
                " WHERE ind.rdb$index_name = con.rdb$index_name\n"
                "   AND con.rdb$constraint_type = 'PRIMARY KEY'\n"
                "   AND con.rdb$relation_name   = '" + p_tablename + "'";
  return sql;
}

CString
SQLInfoFirebird::GetCATALOGPrimaryCreate(MPrimaryMap& p_primaries) const
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
SQLInfoFirebird::GetCATALOGPrimaryDrop(CString /*p_schema*/,CString p_tablename,CString p_constraintname) const
{
  CString sql("ALTER TABLE " + p_tablename + "\n"
              " DROP CONSTRAINT " + p_constraintname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL FOREIGN KEY FUNCTIONS

CString
SQLInfoFirebird::GetCATALOGForeignExists(CString /*p_schema*/,CString p_tablename,CString p_constraintname) const
{
  p_tablename.MakeUpper();
  p_constraintname.MakeUpper();

  CString sql;
  sql.Format("SELECT COUNT(*)\n"
             "  FROM rdb$relation_constraints con\n"
             " WHERE con.rdb$constraint_name = '%s'\n"
             "   AND con.rdb$relation_name   = '%s'\n"
             "   AND con.rdb$constraint_type = 'FOREIGN KEY'"
            ,p_constraintname.GetString()
            ,p_tablename.GetString());
  return sql;
}

CString
SQLInfoFirebird::GetCATALOGForeignList(CString p_schema,CString p_tablename,int p_maxColumns /*=SQLINFO_MAX_COLUMNS*/) const
{
  return GetCATALOGForeignAttributes(p_schema,p_tablename,"",p_maxColumns);
}

CString
SQLInfoFirebird::GetCATALOGForeignAttributes(CString p_schema
                                            ,CString p_tablename
                                            ,CString p_constraint
                                            ,bool    p_referenced /*=false*/
                                            ,int    /*p_maxColumns =SQLINFO_MAX_COLUMNS*/) const
{
  p_schema.MakeUpper();
  p_tablename.MakeUpper();
  p_constraint.MakeUpper();
  CString query;
  query.Format( "SELECT trim(mon.mon$database_name)     AS primary_key_catalog\n"
                "      ,'%s'                            AS primary_key_schema\n"
                "      ,trim(idx.rdb$relation_name)     AS primary_key_table\n"
                "      ,trim(mon.mon$database_name)     AS foreign_key_catalog\n"
                "      ,'%s'                            AS foreign_key_schema\n"
                "      ,trim(con.rdb$relation_name)     AS foreign_key_table\n"
                "      ,trim(ref.rdb$const_name_uq)     AS primary_key_constraint\n"
                "      ,trim(con.rdb$constraint_name)   AS foreign_key_constraint\n"
                "      ,seg.rdb$field_position + 1      AS key_sequence\n"
                "      ,trim(psg.rdb$field_name)        AS foreign_key_column_name\n"
                "      ,trim(seg.rdb$field_name)        AS primary_key_column_name\n"
                "      ,case ref.rdb$update_rule        WHEN 'RESTRICT'     THEN 1\n"
                "                                       WHEN 'CASCADE'      THEN 0\n"
                "                                       WHEN 'SET NULL'     THEN 2\n"
                "                                       WHEN 'SET DEFAULT'  THEN 4\n"
                "                                       WHEN 'NO ACTION'    THEN 3\n"
                "                                       ELSE 0\n"
                "                                       END AS update_rule\n"
                "      ,case ref.rdb$delete_Rule        WHEN 'RESTRICT'     THEN 1\n"
                "                                       WHEN 'CASCADE'      THEN 0\n"
                "                                       WHEN 'SET NULL'     THEN 2\n"
                "                                       WHEN 'SET DEFAULT'  THEN 4\n"
                "                                       WHEN 'NO ACTION'    THEN 3\n"
                "                                       ELSE 0\n"
                "                                       END AS delete_rule\n"
                "      ,case con.rdb$deferrable         WHEN 'YES'  THEN 1 ELSE 0 END as deferrable\n"
                "      ,case ref.rdb$match_option       WHEN 'FULL' THEN 1 ELSE 0 END as match_option\n"
                "      ,case con.rdb$initially_deferred WHEN 'YES'  THEN 1 ELSE 0 END as initially_deferred\n"
                "      ,1                               AS enabled\n"
                "  FROM rdb$relation_constraints con\n"
                "      ,rdb$ref_constraints ref\n"
                "      ,rdb$indices         idx\n"
                "      ,rdb$indices         cix\n"
                "      ,rdb$index_segments  seg\n"
                "      ,rdb$index_segments  psg\n"
                "      ,mon$database        mon\n"
                " WHERE con.rdb$constraint_name = ref.rdb$constraint_name\n"
                "   AND ref.rdb$const_name_uq   = idx.rdb$index_name\n"
                "   AND con.rdb$index_name      = cix.rdb$index_name\n"
                "   AND seg.rdb$index_name      = cix.rdb$index_name\n"
                "   AND psg.rdb$index_name      = idx.rdb$index_name\n"
                "   AND seg.rdb$field_position  = psg.rdb$field_position\n"
                "   AND con.rdb$constraint_type = 'FOREIGN KEY'\n"
                ,p_schema.GetString()
                ,p_schema.GetString());
  if(!p_tablename.IsEmpty())
  {
    if(p_referenced)
    {
      query += "   AND idx.rdb$relation_name   = '" + p_tablename + "'\n";
    }
    else
    {
      query += "   AND con.rdb$relation_name   = '" + p_tablename + "'\n";
    }
  }
  if(!p_constraint.IsEmpty())
  {
    if(p_referenced)
    {
      query += "   AND ref.rdb$constraint_name = '" + p_constraint + "'\n";
    }
    else
    {
      query += "   AND con.rdb$constraint_name = '" + p_constraint + "'\n";
    }
  }

  // Add ordering up to column number
  query += " ORDER BY 1,2,3,4,5,6,7,8,9";

  return query;
}

CString
SQLInfoFirebird::GetCATALOGForeignCreate(MForeignMap& p_foreigns) const
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

CString
SQLInfoFirebird::GetCATALOGForeignAlter(MForeignMap& /*p_original*/, MForeignMap& /*p_requested*/) const
{
  // Firebird cannot alter a foreign-key constraint.
  // You must drop and then re-create your foreign key constraint
  // So return an empty string to signal this!
  return "";
}

CString
SQLInfoFirebird::GetCATALOGForeignDrop(CString /*p_schema*/,CString p_tablename,CString p_constraintname) const
{
  CString sql("ALTER TABLE " + p_tablename + "\n"
              " DROP CONSTRAINT " + p_constraintname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL TRIGGER FUNCTIONS

CString
SQLInfoFirebird::GetCATALOGTriggerExists(CString /*p_schema*/, CString p_tablename, CString p_triggername) const
{
  p_tablename.MakeUpper();
  p_triggername.MakeUpper();

  CString sql;
  sql.Format("SELECT COUNT(*)\n"
             "  FROM rdb$triggers\n"
             " WHERE rdb$relation_name = '%s'\n"
             "   AND rdb$trigger_name  = '%s'\n"
             "   AND rdb$system_flag   = 0"
            ,p_tablename.GetString()
            ,p_triggername.GetString());
  return sql;
}

CString
SQLInfoFirebird::GetCATALOGTriggerList(CString p_schema, CString p_tablename) const
{
  return GetCATALOGTriggerAttributes(p_schema,p_tablename,"");
}

CString
SQLInfoFirebird::GetCATALOGTriggerAttributes(CString p_schema, CString p_tablename, CString p_triggername) const
{
  p_schema.MakeUpper();
  p_triggername.MakeUpper();
  p_triggername.MakeUpper();

  CString sql("SELECT cast('' as varchar(31)) AS catalog_name\n"
             "      ,(SELECT trim(tab.rdb$owner_name)\n"
             "          FROM rdb$relations tab\n"
             "         WHERE tab.rdb$relation_name = trg.rdb$relation_name) AS schema_name\n"
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
             "  FROM rdb$triggers trg\n"
             " WHERE rdb$system_flag   = 0\n");

  // Add tablename filter
  if(!p_tablename.IsEmpty())
  {
    if(p_tablename.Find('%') >= 0)
    {
      sql.AppendFormat("   AND rdb$relation_name LIKE '%s'\n",p_tablename.GetString());
    }
    else
    {
      sql.AppendFormat("   AND rdb$relation_name = '%s'\n",p_tablename.GetString());
    }
  }

  // Add trigger name filter
  if(!p_triggername.IsEmpty())
  {
    if(p_triggername.Find('%') >= 0)
    {
      sql.AppendFormat("   AND rdb$trigger_name LIKE '%s'\n",p_triggername.GetString());
    }
    else
    {
      sql.AppendFormat("   AND rdb$trigger_name = '%s'\n",p_triggername.GetString());
    }
  }

  sql += " ORDER BY rdb$trigger_sequence";
  return sql;
}

CString
SQLInfoFirebird::GetCATALOGTriggerCreate(MetaTrigger& p_trigger) const
{
  CString sql;
  sql.Format("CREATE OR ALTER TRIGGER %s FOR %s\n"
            ,p_trigger.m_triggerName.GetString()
            ,p_trigger.m_tableName.GetString());

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

CString
SQLInfoFirebird::GetCATALOGTriggerDrop(CString /*p_schema*/, CString /*p_tablename*/, CString p_triggername) const
{
  CString sql("DROP TRIGGER " + p_triggername);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL SEQUENCE FUNCTIONS

CString
SQLInfoFirebird::GetCATALOGSequenceExists(CString /*p_schema*/, CString p_sequence) const
{
  p_sequence.MakeUpper();

  CString sql = "SELECT COUNT(*)\n"
                "  FROM rdb$generators\n"
                " WHERE rdb$system_flag    = 0\n"
                "   AND rdb$generator_name = '" + p_sequence + "'";
  return sql;
}

CString
SQLInfoFirebird::GetCATALOGSequenceList(CString /*p_schema*/,CString p_pattern) const
{
  p_pattern.MakeUpper();
  p_pattern = "%" + p_pattern + "%";

  CString sql = "SELECT cast('' as varchar(31))  as catalog_name\n"
                "      ,trim(rdb$owner_name)     as schema_name\n"
                "      ,trim(rdb$generator_name) as sequence_name\n" 
                "      ,rdb$initial_value        as current_value\n"
                "      ,0                        as minimal_value\n"
                "      ,rdb$generator_increment  as increment\n"
                "      ,0  as cache\n"
                "      ,0  as cycle\n"
                "      ,0  as ordering\n"
                "  FROM rdb$generators\n"
                " WHERE rdb$system_flag    = 0\n"
                "   AND rdb$generator_name LIKE '" + p_pattern + "'";
  return sql;
}

CString
SQLInfoFirebird::GetCATALOGSequenceAttributes(CString /*p_schema*/, CString p_sequence) const
{
  p_sequence.MakeUpper();

  CString sql = "SELECT cast('' as varchar(31))  as catalog_name\n"
                "      ,trim(rdb$owner_name)     as schema_name\n"
                "      ,trim(rdb$generator_name) as sequence_name\n" 
                "      ,rdb$initial_value        as current_value\n"
                "      ,0                        as minimal_value\n"
                "      ,rdb$generator_increment  as increment\n"
                "      ,0  as cache\n"
                "      ,0  as cycle\n"
                "      ,0  as ordering\n"
                "  FROM rdb$generators\n"
                " WHERE rdb$system_flag    = 0\n"
                "   AND rdb$generator_name = '" + p_sequence + "'";
  return sql;
}

CString
SQLInfoFirebird::GetCATALOGSequenceCreate(MetaSequence& p_sequence) const
{
  CString sql;
  sql.Format("CREATE SEQUENCE %s START WITH %-12.0f"
            ,p_sequence.m_sequenceName.GetString()
            ,p_sequence.m_currentValue);
  return sql;
}

CString
SQLInfoFirebird::GetCATALOGSequenceDrop(CString /*p_schema*/, CString p_sequence) const
{
  CString sql("DROP SEQUENCE " + p_sequence);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL VIEW FUNCTIONS

CString 
SQLInfoFirebird::GetCATALOGViewExists(CString /*p_schema*/,CString p_viewname) const
{
  p_viewname.MakeUpper();

  CString sql = "SELECT count(*)\n"
                "  FROM rdb$relations\n"
                " WHERE rdb$relation_name = '" + p_viewname + "'\n"
                "   AND rdb$relation_type = 1";
  return sql;
}

CString 
SQLInfoFirebird::GetCATALOGViewList(CString p_schema,CString p_pattern) const
{
  return GetCATALOGViewAttributes(p_schema,p_pattern);
}

CString 
SQLInfoFirebird::GetCATALOGViewAttributes(CString /*p_schema*/,CString p_viewname) const
{
  p_viewname.MakeUpper();
  CString sql = "SELECT CAST('' AS VARCHAR(31))             AS table_catalog\n"
                "      ,CAST(rdb$owner_name AS VARCHAR(31)) AS table_schema\n"
                "      ,trim(rdb$relation_name)             AS table_name\n"
                "      ,CAST('VIEW' as varchar(31))         AS table_type\n"
                "      ,rdb$description   AS remarks\n"
                "      ,trim(rdb$owner_name) || '.' || trim(rdb$relation_name) AS full_name\n"
                "      ,cast('' as varchar(31)) as storage_space\n"
                "  FROM rdb$relations\n"
                " WHERE rdb$relation_type = 1\n"
                "   AND rdb$system_flag   = 0\n";
  if(!p_viewname.IsEmpty())
  {
    sql += "\n AND rdb$relation_name ";
    sql += (p_viewname.Find("%") >= 0) ? "LIKE '" : "= '";
    sql += p_viewname + "'";
  }
  return sql;
}

CString 
SQLInfoFirebird::GetCATALOGViewCreate(CString /*p_schema*/,CString p_viewname,CString p_contents) const
{
  return "RECREATE VIEW " + p_viewname + "\n" + p_contents;
}

CString 
SQLInfoFirebird::GetCATALOGViewRename(CString p_schema,CString p_viewname,CString p_newname)    const
{
  return "";
}

CString 
SQLInfoFirebird::GetCATALOGViewDrop(CString /*p_schema*/,CString p_viewname,CString& p_precursor) const
{
  p_viewname.MakeUpper();

  // Firebird cannot drop a view if dependencies from stored procedures or functions
  // still exist in the dependencies table. After Firebird 3.0 we need modification
  // rights on this system catalog table!!
  // Chances being that we re-create the view right away after the drop.
  p_precursor = "DELETE FROM rdb$dependencies\n"
                " WHERE rdb$depended_on_name = '" + p_viewname + "'\n"
                "   AND rdb$depended_on_type = 0";
  return "DROP VIEW " + p_viewname;
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
SQLInfoFirebird::GetPSMProcedureExists(CString /*p_schema*/, CString p_procedure) const
{
  p_procedure.MakeUpper();
  CString query = ("SELECT (SELECT COUNT(*) FROM rdb$functions  WHERE rdb$function_name  = '" + p_procedure + "')\n"
                   "     + (SELECT COUNT(*) FROM rdb$procedures WHERE rdb$procedure_name = '" + p_procedure + "') as total\n"
                   "  FROM rdb$database");
  return query;
}

CString
SQLInfoFirebird::GetPSMProcedureList(CString p_schema) const
{
  p_schema.MakeUpper();
  CString sql1("SELECT '' as catalog_name\n"
               "      ,trim(rdb$owner_name) as schema_name\n"
               "      ,trim(rdb$procedure_name)\n"
               "  FROM rdb$procedures pro\n");
  if(!p_schema.IsEmpty())
  {
    sql1 += " WHERE pro.rdb$owner = '" + p_schema + "'\n";
  }

  CString sql2("SELECT '' as catalog_name\n"
               "      ,trim(rdb$owner_name) as schema_name\n"
               "      ,trim(rdb$function_name)\n"
               "  FROM rdb$functions fun\n");

  if(!p_schema.IsEmpty())
  {
    sql2 += " WHERE fun.rdb$owner = '" + p_schema + "'\n";
  }

  return sql1 + " UNION ALL\n" + sql2 + " ORDER BY 1,2,3";
}

CString
SQLInfoFirebird::GetPSMProcedureAttributes(CString /*p_schema*/, CString p_procedure) const
{
  p_procedure.MakeUpper();
  CString sql1 ("SELECT '' as catalog_name\n"
                "      ,trim(rdb$owner_name) as schema_name\n"
                "      ,trim(rdb$procedure_name)\n"
                "      ,(SELECT COUNT(*)\n"
                "          FROM rdb$procedure_parameters par\n"
                "         WHERE par.rdb$procedure_name = pro.rdb$procedure_name\n"
                "           AND par.rdb$parameter_type = 0) as input_parameters\n"
                "      ,(SELECT COUNT(*)\n"
                "          FROM rdb$procedure_parameters par\n"
                "         WHERE par.rdb$procedure_name = pro.rdb$procedure_name\n"
                "           AND par.rdb$parameter_type = 1) as output_parameters\n"
                "      ,(SELECT COUNT(*)\n"
                "          FROM rdb$procedure_parameters par\n"
                "         WHERE par.rdb$procedure_name = pro.rdb$procedure_name\n"
                "           AND par.rdb$parameter_type = 1) as result_sets\n"
                "      ,rdb$description\n"
                "      ,1 as procedure_type\n" // SQL_PROCEDURE
                "      ,rdb$procedure_source as source\n"
                "  FROM rdb$procedures pro\n");

  CString sql2 ("SELECT '' as catalog_name\n"
                "      ,trim(rdb$owner_name) as schema_name\n"
                "      ,trim(rdb$function_name)\n"
                "      ,(SELECT COUNT(*)\n"
                "          FROM rdb$function_arguments arg\n"
                "         WHERE fun.rdb$function_name = arg.rdb$function_name\n"
                "           AND arg.rdb$argument_position > 0) as input_parameters\n"
                "      ,(SELECT COUNT(*)\n"
                "          FROM rdb$function_arguments arg\n"
                "         WHERE fun.rdb$function_name = arg.rdb$function_name\n"
                "           AND arg.rdb$argument_position = 0) as output_parameters\n"
                "      ,(SELECT COUNT(*)\n"
                "          FROM rdb$function_arguments arg\n"
                "         WHERE fun.rdb$function_name = arg.rdb$function_name\n"
                "           AND arg.rdb$argument_position > 0) as result_sets\n"
                "      ,rdb$description\n"
                "      ,2 as procedure_type\n" // SQL_FUNCTION
                "      ,rdb$function_source as source\n"
                "  FROM rdb$functions fun\n");

  if(!p_procedure.IsEmpty())
  {
    sql1 += " WHERE rdb$procedure_name = '" + p_procedure + "'\n";
    sql2 += " WHERE rdb$function_name  = '" + p_procedure + "'\n";
  }
  return sql1 + " UNION ALL\n" + sql2;
}

CString
SQLInfoFirebird::GetPSMProcedureSourcecode(CString p_schema, CString p_procedure) const
{
  // Source code already gotten with attributes
  return "";
}

CString
SQLInfoFirebird::GetPSMProcedureCreate(MetaProcedure& /*p_procedure*/) const
{
  return "";
}

CString
SQLInfoFirebird::GetPSMProcedureDrop(CString p_schema, CString p_procedure) const
{
  CString sql("DROP PROCEDURE " + p_procedure);
  return sql;
}

CString
SQLInfoFirebird::GetPSMProcedureErrors(CString p_schema,CString p_procedure) const
{
  // Firebird does not support procedure errors
  return "";
}

// And it's parameters
CString
SQLInfoFirebird::GetPSMProcedureParameters(CString /*p_schema*/,CString p_procedure) const
{
  p_procedure.MakeUpper();

  CString sql = "SELECT '' as catalog_name\n"
                "      ,(SELECT trim(rdb$owner_name)\n"
                "          FROM rdb$procedures pro\n"
                "         WHERE pro.rdb$procedure_name = par.rdb$procedure_name) as schema_name\n"
                "      ,trim(par.rdb$procedure_name) as procedure_name\n"
                "      ,trim(par.rdb$parameter_name) as column_name\n"
                "      ,(par.rdb$parameter_type * 3) + 1 as column_type\n"
                "      ,CASE fld.rdb$field_type\n"
                "            WHEN 7  THEN CASE fld.rdb$field_sub_type\n"
                "                              WHEN 1 THEN 2\n"
                "                              WHEN 2 THEN 2\n"
                "                                     ELSE 5\n"
                "                              END\n"
                "            WHEN 8  THEN CASE fld.rdb$field_sub_type\n"
                "                              WHEN 1 THEN 2\n"
                "                              WHEN 2 THEN 2\n"
                "                                     ELSE 4\n"
                "                              END\n"
                "            WHEN 10 THEN 7\n"
                "            WHEN 12 THEN 10\n"
                "            WHEN 13 THEN 13\n"
                "            WHEN 14 THEN 1\n"
                "            WHEN 16 THEN CASE fld.rdb$field_sub_type\n"
                "                              WHEN 1 THEN 2\n"
                "                              WHEN 2 THEN 2\n"
                "                                     ELSE -5\n"
                "                         END\n"
                "            WHEN 23 THEN -7\n"
                "            WHEN 27 THEN 8\n"
                "            WHEN 35 THEN 11\n"
                "            WHEN 37 THEN 12\n"
                "            WHEN 261 THEN CASE fld.rdb$field_sub_type\n"
                "                               WHEN 0 THEN -4\n"
                "                               WHEN 1 THEN -1\n"
                "                                      ELSE -10\n"
                "                          END\n"
                "                     ELSE 0\n"
                "       END                           as data_type\n"
                "      ,CASE fld.rdb$field_type\n"
                "            WHEN 7  THEN CASE fld.rdb$field_sub_type\n"
                "                              WHEN 1 THEN 'NUMERIC'\n"
                "                              WHEN 2 THEN 'DECIMAL'\n"
                "                                     ELSE 'SMALLINT'\n"
                "                              END\n"
                "            WHEN 8  THEN CASE fld.rdb$field_sub_type\n"
                "                              WHEN 1 THEN 'NUMERIC'\n"
                "                              WHEN 2 THEN 'DECIMAL'\n"
                "                                     ELSE 'INTEGER'\n"
                "                              END\n"
                "            WHEN 10 THEN 'FLOAT'\n"
                "            WHEN 12 THEN 'DATE'\n"
                "            WHEN 13 THEN 'TIME'\n"
                "            WHEN 14 THEN 'CHAR'\n"
                "            WHEN 16 THEN CASE fld.rdb$field_sub_type\n"
                "                              WHEN 1 THEN 'NUMERIC'\n"
                "                              WHEN 2 THEN 'DECIMAL'\n"
                "                                     ELSE 'BIGINT'\n"
                "                         END\n"
                "            WHEN 23 THEN 'BOOLEAN'\n"
                "            WHEN 27 THEN 'DOUBLE PRECISION'\n"
                "            WHEN 35 THEN 'TIMESTAMP'\n"
                "            WHEN 37 THEN 'VARCHAR'\n"
                "            WHEN 261 THEN CASE fld.rdb$field_sub_type\n"
                "                               WHEN 0 THEN 'BLOB SUB_TYPE 0'\n"
                "                               WHEN 1 THEN 'BLOB SUB_TYPE TEXT'\n"
                "                                      ELSE 'BLOB'\n"
                "                          END\n"
                "                     ELSE 'UNKNOWN'\n"
                "       END                                        as type_name\n"		// 6  - VARCHAR NOT NULL
                "      ,CASE fld.rdb$field_type\n"
                "            WHEN 7  THEN CASE fld.rdb$field_sub_type\n"
                "                              WHEN 1 THEN fld.rdb$field_precision\n"
                "                              WHEN 2 THEN fld.rdb$field_precision\n"
                "                                     ELSE 5\n"
                "                              END\n"
                "            WHEN 8  THEN CASE fld.rdb$field_sub_type\n"
                "                              WHEN 1 THEN fld.rdb$field_precision\n"
                "                              WHEN 2 THEN fld.rdb$field_precision\n"
                "                                     ELSE 11\n"
                "                              END\n"
                "            WHEN 10 THEN 12\n"
                "            WHEN 12 THEN 10\n"
                "            WHEN 13 THEN 13\n"
                "            WHEN 14 THEN fld.rdb$field_length\n"
                "            WHEN 16 THEN CASE fld.rdb$field_sub_type\n"
                "                              WHEN 1 THEN fld.rdb$field_precision\n"
                "                              WHEN 2 THEN fld.rdb$field_precision\n"
                "                                     ELSE 22\n"
                "                         END\n"
                "            WHEN 23 THEN 1\n"
                "            WHEN 27 THEN 20\n"
                "            WHEN 35 THEN 26\n"
                "            WHEN 37 THEN fld.rdb$field_length\n"
                "            WHEN 261 THEN 2147483647\n"
                "       END                                        as column_size\n"
                "      ,cast(fld.rdb$field_length as integer)      as buffer_length\n"
                "      ,cast (fld.rdb$field_scale as smallint)*-1  as scale\n"
                "      ,10                    as radix\n"
                "      ,(coalesce(par.rdb$null_flag,0,0)-1)*-1     as nullable\n"
                "      ,par.RDB$DESCRIPTION   as remarks\n"
                "      ,par.RDB$DEFAULT_source as default_value\n"
                "      ,CASE fld.rdb$field_type\n"
                "            WHEN 7  THEN CASE fld.rdb$field_sub_type\n"
                "                              WHEN 1 THEN 2\n"
                "                              WHEN 2 THEN 2\n"
                "                                     ELSE 5\n"
                "                              END\n"
                "            WHEN 8  THEN CASE fld.rdb$field_sub_type\n"
                "                              WHEN 1 THEN 2\n"
                "                              WHEN 2 THEN 2\n"
                "                                     ELSE 4\n"
                "                              END\n"
                "            WHEN 10 THEN 7\n"
                "            WHEN 12 THEN 9\n"
                "            WHEN 13 THEN 10\n"
                "            WHEN 14 THEN 1\n"
                "            WHEN 16 THEN CASE fld.rdb$field_sub_type\n"
                "                              WHEN 1 THEN 2\n"
                "                              WHEN 2 THEN 2\n"
                "                                     ELSE -5\n"
                "                         END\n"
                "            WHEN 23 THEN -7\n"
                "            WHEN 27 THEN 8\n"
                "            WHEN 35 THEN 11\n"
                "            WHEN 37 THEN 12\n"
                "            WHEN 261 THEN CASE fld.rdb$field_sub_type\n"
                "                               WHEN 0 THEN -4\n"
                "                               WHEN 1 THEN -1\n"
                "                                      ELSE -10\n"
                "                          END\n"
                "                     ELSE 0\n"
                "       END                                         as sql_data_type\n"
                "      ,CAST(0 AS SMALLINT)                         as sql_datetime_sub\n"
                "      ,fld.rdb$field_length / rdb$character_length as char_octet_length\n"
                "      ,par.rdb$parameter_number + par.rdb$parameter_type as ordinal_position\n"
                "      ,CASE (coalesce(par.rdb$null_flag,0,0)-1)*-1\n"
                "            WHEN 0 THEN 'NO'\n"
                "            WHEN 1 THEN 'YES'\n"
                "                   ELSE 'UNKNOWN'\n"
                "        END        AS is_nullable\n"
                "  FROM rdb$procedure_parameters par\n"
                "      ,rdb$fields fld\n"
                " WHERE fld.rdb$field_name = par.rdb$field_source\n"
                "   AND par.rdb$procedure_name = '" + p_procedure + "'\n"
                "UNION ALL\n"
                "SELECT '' as catalog_name\n"
                "      ,(SELECT trim(rdb$owner_name)\n"
                "          FROM rdb$functions fun\n"
                "         WHERE fun.rdb$function_name = par.rdb$function_name)\n"
                "      ,trim(par.rdb$function_name) as procedure_name\n"
                "      ,trim(par.rdb$argument_name) as column_name\n"
                "      ,CASE par.rdb$argument_name IS NULL\n"
                "            WHEN TRUE THEN 5\n"
                "                      ELSE 2 \n"
                "       END as column_type\n"
                "      ,CASE fld.rdb$field_type\n"
                "            WHEN 7  THEN CASE fld.rdb$field_sub_type\n"
                "                              WHEN 1 THEN 2\n"
                "                              WHEN 2 THEN 2\n"
                "                                     ELSE 5\n"
                "                              END\n"
                "            WHEN 8  THEN CASE fld.rdb$field_sub_type\n"
                "                              WHEN 1 THEN 2\n"
                "                              WHEN 2 THEN 2\n"
                "                                     ELSE 4\n"
                "                              END\n"
                "            WHEN 10 THEN 7\n"
                "            WHEN 12 THEN 10\n"
                "            WHEN 13 THEN 13\n"
                "            WHEN 14 THEN 1\n"
                "            WHEN 16 THEN CASE fld.rdb$field_sub_type\n"
                "                              WHEN 1 THEN 2\n"
                "                              WHEN 2 THEN 2\n"
                "                                     ELSE -5\n"
                "                         END\n"
                "            WHEN 23 THEN -7\n"
                "            WHEN 27 THEN 8\n"
                "            WHEN 35 THEN 11\n"
                "            WHEN 37 THEN 12\n"
                "            WHEN 261 THEN CASE fld.rdb$field_sub_type\n"
                "                               WHEN 0 THEN -4\n"
                "                               WHEN 1 THEN -1\n"
                "                                      ELSE -10\n"
                "                          END\n"
                "                     ELSE 0\n"
                "       END                           as data_type\n"
                "      ,CASE fld.rdb$field_type\n"
                "            WHEN 7  THEN CASE fld.rdb$field_sub_type\n"
                "                              WHEN 1 THEN 'NUMERIC'\n"
                "                              WHEN 2 THEN 'DECIMAL'\n"
                "                                     ELSE 'SMALLINT'\n"
                "                              END\n"
                "            WHEN 8  THEN CASE fld.rdb$field_sub_type\n"
                "                              WHEN 1 THEN 'NUMERIC'\n"
                "                              WHEN 2 THEN 'DECIMAL'\n"
                "                                     ELSE 'INTEGER'\n"
                "                              END\n"
                "            WHEN 10 THEN 'FLOAT'\n"
                "            WHEN 12 THEN 'DATE'\n"
                "            WHEN 13 THEN 'TIME'\n"
                "            WHEN 14 THEN 'CHAR'\n"
                "            WHEN 16 THEN CASE fld.rdb$field_sub_type\n"
                "                              WHEN 1 THEN 'NUMERIC'\n"
                "                              WHEN 2 THEN 'DECIMAL'\n"
                "                                     ELSE 'BIGINT'\n"
                "                         END\n"
                "            WHEN 23 THEN 'BOOLEAN'\n"
                "            WHEN 27 THEN 'DOUBLE PRECISION'\n"
                "            WHEN 35 THEN 'TIMESTAMP'\n"
                "            WHEN 37 THEN 'VARCHAR'\n"
                "            WHEN 261 THEN CASE fld.rdb$field_sub_type\n"
                "                               WHEN 0 THEN 'BLOB SUB_TYPE 0'\n"
                "                               WHEN 1 THEN 'BLOB SUB_TYPE TEXT'\n"
                "                                      ELSE 'BLOB'\n"
                "                          END\n"
                "                     ELSE 'UNKNOWN'\n"
                "       END                                        as type_name\n"		// 6  - VARCHAR NOT NULL
                "      ,CASE fld.rdb$field_type\n"
                "            WHEN 7  THEN CASE fld.rdb$field_sub_type\n"
                "                              WHEN 1 THEN fld.rdb$field_precision\n"
                "                              WHEN 2 THEN fld.rdb$field_precision\n"
                "                                     ELSE 5\n"
                "                              END\n"
                "            WHEN 8  THEN CASE fld.rdb$field_sub_type\n"
                "                              WHEN 1 THEN fld.rdb$field_precision\n"
                "                              WHEN 2 THEN fld.rdb$field_precision\n"
                "                                     ELSE 11\n"
                "                              END\n"
                "            WHEN 10 THEN 12\n"
                "            WHEN 12 THEN 10\n"
                "            WHEN 13 THEN 13\n"
                "            WHEN 14 THEN fld.rdb$field_length\n"
                "            WHEN 16 THEN CASE fld.rdb$field_sub_type\n"
                "                              WHEN 1 THEN fld.rdb$field_precision\n"
                "                              WHEN 2 THEN fld.rdb$field_precision\n"
                "                                     ELSE 22\n"
                "                         END\n"
                "            WHEN 23 THEN 1\n"
                "            WHEN 27 THEN 20\n"
                "            WHEN 35 THEN 26\n"
                "            WHEN 37 THEN fld.rdb$field_length\n"
                "            WHEN 261 THEN 2147483647\n"
                "       END           as column_size\n"
                "      ,cast(fld.rdb$field_length as integer)      as buffer_length\n"
                "      ,cast (fld.rdb$field_scale as smallint)*-1  as scale\n"
                "      ,10                    as radix\n"
                "      ,(coalesce(par.rdb$null_flag,0,0)-1)*-1     as nullable\n"
                "      ,par.rdb$description   as remarks\n"
                "      ,par.rdb$default_source as default_value\n"
                "      ,CASE fld.rdb$field_type\n"
                "            WHEN 7  THEN CASE fld.rdb$field_sub_type\n"
                "                              WHEN 1 THEN 2\n"
                "                              WHEN 2 THEN 2\n"
                "                                     ELSE 5\n"
                "                              END\n"
                "            WHEN 8  THEN CASE fld.rdb$field_sub_type\n"
                "                              WHEN 1 THEN 2\n"
                "                              WHEN 2 THEN 2\n"
                "                                     ELSE 4\n"
                "                              END\n"
                "            WHEN 10 THEN 7\n"
                "            WHEN 12 THEN 9\n"
                "            WHEN 13 THEN 10\n"
                "            WHEN 14 THEN 1\n"
                "            WHEN 16 THEN CASE fld.rdb$field_sub_type\n"
                "                              WHEN 1 THEN 2\n"
                "                              WHEN 2 THEN 2\n"
                "                                     ELSE -5\n"
                "                         END\n"
                "            WHEN 23 THEN -7\n"
                "            WHEN 27 THEN 8\n"
                "            WHEN 35 THEN 11\n"
                "            WHEN 37 THEN 12\n"
                "            WHEN 261 THEN CASE fld.rdb$field_sub_type\n"
                "                               WHEN 0 THEN -4\n"
                "                               WHEN 1 THEN -1\n"
                "                                      ELSE -10\n"
                "                          END\n"
                "                     ELSE 0\n"
                "       END                  as sql_data_type\n"
                "      ,CAST(0 AS SMALLINT)  as sql_datetime_sub\n"
                "      ,fld.rdb$field_length / fld.rdb$character_length as char_octet_length\n"
                "      ,par.rdb$argument_position as ordinal_position\n"
                "      ,CASE (coalesce(par.rdb$null_flag,0,0)-1)*-1\n"
                "            WHEN 0 THEN 'NO'\n"
                "            WHEN 1 THEN 'YES'\n"
                "                   ELSE 'UNKNOWN'\n"
                "        END        AS is_nullable\n"
                "  FROM rdb$function_arguments par\n"
                "      ,rdb$fields fld\n"
                " WHERE fld.rdb$field_name = par.rdb$field_source\n"
                "   AND par.rdb$function_name = '" + p_procedure + "'";
  return sql;
}

//////////////////////////////////////////////////////////////////////////
//
// ALL PSM LANGUAGE ELEMENTS
//
//////////////////////////////////////////////////////////////////////////

CString
SQLInfoFirebird::GetPSMDeclaration(bool    /*p_first*/
                                  ,CString p_variable
                                  ,int     p_datatype
                                  ,int     p_precision /*= 0 */
                                  ,int     p_scale     /*= 0 */
                                  ,CString p_default   /*= ""*/
                                  ,CString p_domain    /*= ""*/
                                  ,CString p_asColumn  /*= ""*/) const
{
  CString line;
  line.Format("DECLARE %s ",p_variable.GetString());

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
    line += " TYPE OF " + p_domain;
  }
  else if(!p_asColumn)
  {
    line += " TYPE OF COLUMN " + p_asColumn;
  }
  line += ";\n";
  return line;
}

CString
SQLInfoFirebird::GetPSMAssignment(CString p_variable,CString p_statement /*= ""*/) const
{
  CString line(p_variable);
  line += " = ";
  if(!p_statement.IsEmpty())
  {
    line += p_statement;
    line += ";";
  }
  return line;
}

CString
SQLInfoFirebird::GetPSMIF(CString p_condition) const
{
  CString line("IF (");
  line += p_condition;
  line += ")\n  BEGIN\n";
  return line;
}

CString
SQLInfoFirebird::GetPSMIFElse() const
{
  CString line("  END\n"
               "ELSE\n"
               "  BEGIN\n");
  return line;
}

CString
SQLInfoFirebird::GetPSMIFEnd() const
{
  CString line("  END;\n");
  return line;
}

CString
SQLInfoFirebird::GetPSMWhile(CString p_condition) const
{
  return "WHILE (" + p_condition + ") LOOP\n";
}

CString
SQLInfoFirebird::GetPSMWhileEnd() const
{
  return "END LOOP;\n";
}

CString
SQLInfoFirebird::GetPSMLOOP() const
{
  return "WHILE (true) DO BEGIN\n";
}

CString
SQLInfoFirebird::GetPSMLOOPEnd() const
{
  return "END;\n";
}

CString
SQLInfoFirebird::GetPSMBREAK() const
{
  return "BREAK;\n";
}

CString
SQLInfoFirebird::GetPSMRETURN(CString p_statement /*= ""*/) const
{
  return "SUSPEND;\n";
}

CString
SQLInfoFirebird::GetPSMExecute(CString p_procedure,MParameterMap& p_parameters) const
{
  // EXECUTE PROCEDURE name[(:param[,:param …])] [RETURNING_VALUES:param[,:param …]];
  CString line;
  line.Format("EXECUTE PROCEDURE %s (",p_procedure.GetString());
  bool doReturning = false;
  bool doMore = false;

  for(auto& param : p_parameters)
  {
    // Extra ,
    if(doMore) line += ",";
    doMore = true;

    // Append input and in/out parameters
    if(param.m_columnType == SQL_PARAM_INPUT || param.m_columnType == SQL_PARAM_INPUT_OUTPUT)
    {
      line.AppendFormat(":%s",param.m_parameter.GetString());
    }
    // See if we must do 'returning' clause
    if(param.m_columnType == SQL_PARAM_OUTPUT || param.m_columnType == SQL_PARAM_INPUT_OUTPUT)
    {
      doReturning = true;
    }
  }
  line += ")";

  // Do the returning clause
  if(doReturning)
  {
    line += " RETURNING VALUES ";
    doMore = false;
    for(auto& param : p_parameters)
    {
      if(doMore) line += ",";
      doMore = true;

      if(param.m_columnType == SQL_PARAM_OUTPUT || param.m_columnType == SQL_PARAM_INPUT_OUTPUT)
      {
        line.AppendFormat(":%s",param.m_parameter.GetString());
      }
    }
  }
  line += ";\n";
  return line;
}

// The CURSOR
CString
SQLInfoFirebird::GetPSMCursorDeclaration(CString p_cursorname,CString p_select) const
{
  return "DECLARE " + p_cursorname +" SCROLL CURSOR FOR (" + p_select + ");";
}

CString
SQLInfoFirebird::GetPSMCursorFetch(CString p_cursorname,std::vector<CString>& /*p_columnnames*/,std::vector<CString>& p_variablenames) const
{
  CString query = "FETCH " + p_cursorname + " INTO ";
  bool moreThenOne = false;

  for(auto& var : p_variablenames)
  {
    if(moreThenOne) query += ",";
    moreThenOne = true;

    query += ":" + var;
  }
  query += ";";
  return query;
}

//////////////////////////////////////////////////////////////////////////
// PSM Exceptions

CString
SQLInfoFirebird::GetPSMExceptionCatchNoData() const
{
  // SQLSTATE 02000 equals the error 100 Data not found
//return "WHEN SQLCODE 100 DO\n";
  return "WHEN SQLSTATE 02000 DO\n";
}

CString
SQLInfoFirebird::GetPSMExceptionCatch(CString p_sqlState) const
{
  return "WHEN SQLSTATE " + p_sqlState + " DO\n";
}

CString
SQLInfoFirebird::GetPSMExceptionRaise(CString p_sqlState) const
{
  return "EXCEPTION " + p_sqlState;
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
SQLInfoFirebird::GetSESSIONMyself() const
{
  CString query = "SELECT mon.mon$attachment_id\n"
                  "      ,mon.mon$remote_os_user\n"
                  "      ,mon.mon$remote_host\n"
                  "      ,mon.mon$timestamp\n"
                  "      ,mon.mon$remote_address\n"
                  "      ,mon.mon$remote_process\n"
                  "      ,mon.mon$remote_pid\n"
                  "  FROM mon$attachments mon\n"
                  " WHERE mon$attachment_id = current_connection";
  return query;
}

CString
SQLInfoFirebird::GetSESSIONExists(CString p_sessionID) const
{
  CString query;
  query.Format("SELECT COUNT(*)\n"
               "  FROM mon$attachments\n"
               " WHERE mon$system_flag = 0\n"
               "   AND mon$attachment_id = %s",p_sessionID.GetString());
  return query;
}

CString
SQLInfoFirebird::GetSESSIONList() const
{
  return GetSESSIONAttributes("");
}

CString
SQLInfoFirebird::GetSESSIONAttributes(CString p_sessionID) const
{
  CString sql = "SELECT mon.mon$attachment_id\n"
                "      ,mon.mon$remote_os_user\n"
                "      ,mon.mon$remote_host\n"
                "      ,mon.mon$timestamp\n"
                "      ,mon.mon$remote_address\n"
                "      ,mon.mon$remote_process\n"
                "      ,mon.mon$remote_pid\n"
                "  FROM mon$attachments mon";
  if(!p_sessionID.IsEmpty())
  {
    sql += "\n WHERE mon$attachment_id = " + p_sessionID;
  }
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// Transactions

CString
SQLInfoFirebird::GetSESSIONConstraintsDeferred() const
{
  // Firebird cannot defer constraints
  return "";
}

CString
SQLInfoFirebird::GetSESSIONConstraintsImmediate() const
{
  // Firebird constraints are always active
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
          throw StdException("Wrong number of output parameters for procedure call");
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

// Remove catalog dependencies
// CString 
// SQLInfoFirebird::GetSQLRemoveProcedureDependencies(CString p_procname) const
// {
//   CString upperName = p_procname;
//   upperName.MakeUpper();
//   CString query = "DELETE FROM rdb$dependencies\n"
//                   " WHERE rdb$depended_on_name = '" + upperName + "'\n"
//                   "   AND rdb$depended_on_type = 5\n"
//                   "   AND rdb$dependent_type   = 5";
//   return query;
// }
// 
// CString
// SQLInfoFirebird::GetSQLRemoveFieldDependencies(CString p_tablename) const
// {
//   CString upperName = p_tablename;
//   upperName.MakeUpper();
//   CString query = "DELETE FROM rdb$dependencies\n"
//                   " WHERE rdb$depended_on_name = '" + upperName + "'\n"
//                   "   AND rdb$dependent_type   = 3";
//   return query;
// }
// 


