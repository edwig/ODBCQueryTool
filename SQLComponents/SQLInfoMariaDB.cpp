////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoMariaDB.cpp
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
#include "SQLInfoMariaDB.h"
#include "SQLQuery.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{

// Constructor
SQLInfoMariaDB::SQLInfoMariaDB(SQLDatabase* p_database)
               :SQLInfoDB(p_database)
{
  DetectOracleMode();

  AddSQLWord("database");
  AddSQLWord("databases");
}

// Destructor. Does nothing
SQLInfoMariaDB::~SQLInfoMariaDB()
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
SQLInfoMariaDB::GetRDBMSDatabaseType() const
{
  return RDBMS_MARIADB;
}

// The name of the database vendor
XString
SQLInfoMariaDB::GetRDBMSVendorName() const
{
  // The name of the database vendor
  return "MariaDB";
}

// Get the physical database name
XString
SQLInfoMariaDB::GetRDBMSPhysicalDatabaseName() const
{
  // See to it that "SQLDatabase:GetDatabaseName" does it's work
  return m_database->GetDatabaseName();
}

// System catalog is stored in uppercase in the database?
bool
SQLInfoMariaDB::GetRDBMSIsCatalogUpper() const
{
  return false;
}

// System catalog supports full ISO schemas (same tables per schema)
bool
SQLInfoMariaDB::GetRDBMSUnderstandsSchemas() const
{
  return true;
}

// Supports database/ODBCdriver comments in SQL
bool
SQLInfoMariaDB::GetRDBMSSupportsComments() const
{
  return true;
}

// Database can defer constraints until the end of a transaction
bool
SQLInfoMariaDB::GetRDBMSSupportsDeferredConstraints() const
{
  // NO CONSTRAINTS DEFERRED 
  // If transactions are supported we assume that constraints can be deferred
  // until the end of the transaction in the commit phase
  return false;
}

// Database has ORDER BY with an expression, e.g. ORDER BY UPPER(columnname)
bool
SQLInfoMariaDB::GetRDBMSSupportsOrderByExpression() const
{
  return true;
}

// Supports the ODBC escape sequence {[?=] CALL procedure (?,?,?)}
bool
SQLInfoMariaDB::GetRDBMSSupportsODBCCallEscapes() const
{
  return true;
}

// If the database does not support the datatype TIME, it can be implemented as a DECIMAL
bool
SQLInfoMariaDB::GetRDBMSSupportsDatatypeTime() const
{
  // Time can be implemented as TIME
  return true;
}

// If the database does not support the datatype INTERVAL, it can be implemented as a DECIMAL
bool
SQLInfoMariaDB::GetRDBMSSupportsDatatypeInterval() const
{
  // Interval not supported, can be implemented as DECIMALS
  return false;
}

// Supports functions at the place of table columns in create/alter index statement
bool
SQLInfoMariaDB::GetRDBMSSupportsFunctionalIndexes() const
{
  return false;
}

// Gets the maximum length of an SQL statement
unsigned long
SQLInfoMariaDB::GetRDBMSMaxStatementLength() const
{
  // No Limit
  return 0;
}

// Database must commit DDL commands in a transaction
bool
SQLInfoMariaDB::GetRDBMSMustCommitDDL() const
{
  return false;
}

// Correct maximum precision,scale for a NUMERIC datatype
void
SQLInfoMariaDB::GetRDBMSNumericPrecisionScale(SQLULEN& /*p_precision*/, SQLSMALLINT& /*p_scale*/) const
{
  // NO-OP
}

// Maximum for a VARCHAR to be handled without AT-EXEC data. Assume NVARCHAR is half that size!
int
SQLInfoMariaDB::GetRDBMSMaxVarchar() const
{
  // See: https://mariadb.com/kb/en/varchar/#:~:text=The%20range%20of%20M%20is,a%20maximum%20of%2021%2C844%20characters.
  return 65532;
}

// KEYWORDS

// Keyword for the current date and time
XString
SQLInfoMariaDB::GetKEYWORDCurrentTimestamp() const
{
  return "current_timestamp";
}

// String for the current date
XString
SQLInfoMariaDB::GetKEYWORDCurrentDate() const
{
  return "current_date";
}

// Get the concatenation operator
XString
SQLInfoMariaDB::GetKEYWORDConcatanationOperator() const
{
  // BEWARE: Officially the concatenation operator is "CONCAT(string1,string2)"
  // MariaDB supports 'one' 'two' concatenation of two strings (no operator)
  // Or in ORACLE mode the standard '||' operator
  if(m_oracleMode)
  {
    return "||";
  }
  else return "";
}

// Get quote character for strings
XString
SQLInfoMariaDB::GetKEYWORDQuoteCharacter() const
{
  return "\'";
}

// Get quote character around reserved words as an identifier
XString
SQLInfoMariaDB::GetKEYWORDReservedWordQuote() const
{
  return "`";
}

// Get default NULL for parameter list input
XString
SQLInfoMariaDB::GetKEYWORDParameterDefaultNULL() const
{
  // Standard, no definition defines the NULL state
  if (m_oracleMode)
  {
    return "DEFAULT NULL";
  }
  else return "";
}

// Parameter is for INPUT and OUTPUT in parameter list
XString
SQLInfoMariaDB::GetKEYWORDParameterINOUT() const
{
  if (m_oracleMode)
  {
    return "INOUT";
  }
  else return "";
}

// Parameter is for OUTPUT only in parameter list
XString
SQLInfoMariaDB::GetKEYWORDParameterOUT() const
{
  if (m_oracleMode)
  {
    return "OUT";
  }
  else return "";
}

// Get datatype of the IDENTITY primary key in a Network database
XString
SQLInfoMariaDB::GetKEYWORDNetworkPrimaryKeyType() const
{
  // Use SEQUENCE to fill!
  return "INTEGER";
}

// Get datatype for timestamp (year to second)
XString
SQLInfoMariaDB::GetKEYWORDTypeTimestamp() const
{
  return "TIMESTAMP";
}

// Prefix for a parameter in a stored procedure
XString
SQLInfoMariaDB::GetKEYWORDParameterPrefix() const
{
  return "";
}

// Get select part to add new record identity to a table
// Can be special column like 'OID' or a sequence select
XString
SQLInfoMariaDB::GetKEYWORDIdentityString(XString& p_tablename,XString p_postfix /*= "_seq"*/) const
{
  return p_tablename + p_postfix + ".nextval";
}

// Gets the UPPER function
XString
SQLInfoMariaDB::GetKEYWORDUpper(XString& p_expression) const
{
  return "UPPER(" + p_expression + ")";
}

// Gets the construction for 1 minute ago
XString
SQLInfoMariaDB::GetKEYWORDInterval1MinuteAgo() const
{
  return "TIMESTAMPADD(MINUTE,-1,CURRENT_TIMESTAMP)";
}

// Gets the Not-NULL-Value statement of the database
XString
SQLInfoMariaDB::GetKEYWORDStatementNVL(XString& p_test,XString& p_isnull) const
{
  return "{fn IFNULL(" + p_test + "," + p_isnull + ")}";
}

// Gets the RDBMS definition of the datatype
XString
SQLInfoMariaDB::GetKEYWORDDataType(MetaColumn* p_column)
{
  XString type;

  switch(p_column->m_datatype)
  {
    case SQL_CHAR:                      type = "CHAR";          break;
    case SQL_VARCHAR:                   type = "VARCHAR";       break;
    case SQL_LONGVARCHAR:               type = "TEXT";          break;
    case SQL_WCHAR:                     type = "CHAR charset ucs2";         break;   // TBF
    case SQL_WVARCHAR:                  type = "VARCHAR charset ucs2";      break;   // TBF
    case SQL_WLONGVARCHAR:              type = "TEXT charset ucs2";         break;   // TBF
    case SQL_NUMERIC:                   // Fall through
    case SQL_DECIMAL:                   type = "DECIMAL";
                                        if(p_column->m_decimalDigits == 0)
                                        {
                                          if(p_column->m_columnSize <= 2)
                                          {
                                            type = "TINYINT";
                                            p_column->m_datatype  = SQL_TINYINT;
                                            p_column->m_datatype3 = SQL_TINYINT;
                                          }
                                          else if(p_column->m_columnSize <= 4)
                                          {
                                            type = "SMALLINT";
                                            p_column->m_datatype  = SQL_SMALLINT;
                                            p_column->m_datatype3 = SQL_SMALLINT;
                                          }
                                          else if(p_column->m_columnSize <= 9)
                                          {
                                            type = "INTEGER";
                                            p_column->m_datatype  = SQL_INTEGER;
                                            p_column->m_datatype3 = SQL_INTEGER;
                                          }
                                          else if(p_column->m_columnSize <= 18)
                                          {
                                            type = "BIGINT";
                                            p_column->m_datatype  = SQL_BIGINT;
                                            p_column->m_datatype3 = SQL_BIGINT;
                                          }
                                          else if(p_column->m_columnSize >= SQLNUM_MAX_PREC)
                                          {
                                            // Unspecified DECIMAL FOUND.
                                            // See to it that we get some decimals at least
                                            p_column->m_decimalDigits = SQLNUM_MAX_PREC / 2;
                                          }
                                        }
                                        break;
    case SQL_INTEGER:                   type = "INTEGER";
                                        p_column->m_columnSize = 10;
                                        break;
    case SQL_SMALLINT:                  type = "SMALLINT";
                                        p_column->m_columnSize = 5;
                                        break;
    case SQL_FLOAT:                     type = "FLOAT";         break;
    case SQL_REAL:                      type = "REAL";          break;
    case SQL_DOUBLE:                    type = "DOUBLE";        break;
    //case SQL_DATE:
    case SQL_DATETIME:                  type = "DATETIME";      break;
    case SQL_TYPE_DATE:                 type = "DATE";          break;
    case SQL_TIME:                      type = "TIME";          break;
    case SQL_TYPE_TIME:                 type = "TIME";          break;
    case SQL_TIMESTAMP:                 type = "TIMESTAMP";     break;
    case SQL_TYPE_TIMESTAMP:            type = "TIMESTAMP";     break;
    case SQL_BINARY:                    type = "BINARY";        break;
    case SQL_VARBINARY:                 type = "VARBINARY";     break;
    case SQL_LONGVARBINARY:             type = "LONGBLOB";      break;
    case SQL_BIGINT:                    type = "BIGINT";
                                        p_column->m_columnSize = 19;
                                        break;
    case SQL_TINYINT:                   type = "TINYINT";
                                        p_column->m_columnSize = 3;
                                        break;
    case SQL_BIT:                       type = "BIT";           break;
    case SQL_GUID:                      type = "VARCHAR";
                                        p_column->m_columnSize    = 45;
                                        p_column->m_decimalDigits = 0;
                                        break;
    case SQL_INTERVAL_YEAR:             // Fall through
    case SQL_INTERVAL_MONTH:            // Fall through
    case SQL_INTERVAL_DAY:              // Fall through
    case SQL_INTERVAL_HOUR:             // Fall through
    case SQL_INTERVAL_MINUTE:           // Fall through
    case SQL_INTERVAL_SECOND:           // Fall through
    case SQL_INTERVAL_YEAR_TO_MONTH:    // Fall through
    case SQL_INTERVAL_DAY_TO_HOUR:      // Fall through
    case SQL_INTERVAL_DAY_TO_MINUTE:    // Fall through
    case SQL_INTERVAL_DAY_TO_SECOND:    // Fall through
    case SQL_INTERVAL_HOUR_TO_MINUTE:   // Fall through
    case SQL_INTERVAL_HOUR_TO_SECOND:   // Fall through
    case SQL_INTERVAL_MINUTE_TO_SECOND: type = "VARCHAR";
                                        p_column->m_columnSize    = 40;
                                        p_column->m_decimalDigits = 0;
                                        break;
    case SQL_UNKNOWN_TYPE:
    default:                            type = "UNKNOWN ODBC DATA TYPE!";  break;
  }
  return p_column->m_typename = type;
}

// Gets the USER (current-user) keyword function
XString
SQLInfoMariaDB::GetKEYWORDCurrentUser() const
{
  return "CURRENT_USER";
}

// Connects to a default schema in the database/instance
XString
SQLInfoMariaDB::GetSQLDefaultSchema(XString /*p_user*/,XString p_schema) const
{
  return "USE " + p_schema;
}

// Gets the construction for inline generating a key within an INSERT statement
XString
SQLInfoMariaDB::GetSQLNewSerial(XString p_table, XString p_sequence) const
{
  if(!m_oracleMode)
  {
    return "";
  }

  XString sequence(p_sequence);
  if(sequence.IsEmpty() && !p_table.IsEmpty())
  {
    sequence = p_table + "_seq";
  }
  // Select next value from a generator sequence
  return sequence + ".NEXTVAL";
}

// Gets the construction / select for generating a new serial identity
XString
SQLInfoMariaDB::GetSQLGenerateSerial(XString p_table) const
{
  if (m_oracleMode)
  {
    return "SELECT " + p_table + "_seq.nextval FROM DUAL";
  }
  else return "";
}

// Gets the construction / select for the resulting effective generated serial
XString
SQLInfoMariaDB::GetSQLEffectiveSerial(XString p_identity) const
{
  return p_identity;
}

// Gets the sub-transaction commands
XString
SQLInfoMariaDB::GetSQLStartSubTransaction(XString p_savepointName) const
{
  return XString("SAVEPOINT ") + p_savepointName;
}

XString
SQLInfoMariaDB::GetSQLCommitSubTransaction(XString p_savepointName) const
{
  return XString("RELEASE SAVEPOINT") = p_savepointName;
}

XString
SQLInfoMariaDB::GetSQLRollbackSubTransaction(XString p_savepointName) const
{
  return XString("ROLLBACK TO SAVEPOINT ") + p_savepointName;
}

// FROM-Part for a query to select only 1 (one) record / or empty!
XString
SQLInfoMariaDB::GetSQLFromDualClause() const
{
  if(m_oracleMode)
  {
    return " FROM DUAL";
  }
  // MySQL does bare SELECT!
  return "";
}

// Get SQL to lock  a table 
XString
SQLInfoMariaDB::GetSQLLockTable(XString p_schema,XString p_tablename,bool p_exclusive,int /*p_waittime*/) const
{
  // Standard ISO SQL Syntax
  XString query = "LOCK TABLE " + p_schema + "." + p_tablename + " IN ";
  query += p_exclusive ? "EXCLUSIVE" : "SHARE";
  query += " MODE";
  return query;
}

// Get query to optimize the table statistics
XString
SQLInfoMariaDB::GetSQLOptimizeTable(XString p_schema, XString p_tablename) const
{
  return "OPTIMIZE TABLE " + p_schema + "." + p_tablename + " NOWAIT";
}

// Transform query to select top <n> rows
XString
SQLInfoMariaDB::GetSQLTopNRows(XString p_sql,int p_top,int p_skip /*= 0*/) const
{
  if(p_top > 0)
  {
    // MYSQL: " LIMIT <top> [ OFFSET <skip> ]
    XString limit;
    limit.Format("\n LIMIT %d",p_top);
    if(p_skip > 0)
    {
      limit.AppendFormat(" OFFSET %d",p_skip);
    }
    p_sql += limit;
  }
  return p_sql;
}

// Query to perform a keep alive ping
XString
SQLInfoMariaDB::GetPing() const
{
  // Getting the time does a ping
  return "SELECT current_timestamp";
}

//////////////////////////////////////////////////////////////////////////
//
// SQL STRINGS
//
//////////////////////////////////////////////////////////////////////////

// Makes a SQL string from a given string, with all the right quotes
XString
SQLInfoMariaDB::GetSQLString(const XString& p_string) const
{
  XString s = p_string;
  s.Replace("'","''");
  XString kwoot = GetKEYWORDQuoteCharacter();
  return  kwoot + s + kwoot;
}

// Get date string in engine format
XString
SQLInfoMariaDB::GetSQLDateString(int p_year,int p_month,int p_day) const
{
  XString dateString;
  dateString.Format("{d '%04d-%02d-%02d'}",p_year,p_month,p_day);
  return dateString;
}

// Get time string in database engine format
XString
SQLInfoMariaDB::GetSQLTimeString(int p_hour,int p_minute,int p_second) const
{
  XString retval;
  retval.Format("{t '%02d:%02d:%02d'}",p_hour,p_minute,p_second);
  return retval;
}

// Get date-time string in database engine format
XString
SQLInfoMariaDB::GetSQLDateTimeString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  XString string;
  string.Format("{ts '%04d-%02d-%02d %02d:%02d:%02d'}"
                ,p_year,p_month,p_day // ODBC Ordering !!
                ,p_hour,p_minute,p_second);
  return string;
}

// Get date-time bound parameter string in database format
XString
SQLInfoMariaDB::GetSQLDateTimeBoundString() const
{
  return "{ts ?}";
}

// Stripped data for the parameter binding
XString
SQLInfoMariaDB::GetSQLDateTimeStrippedString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  XString string;
  string.Format("%04d-%02d-%02d %02d:%02d:%02d"
                ,p_year,p_month,p_day // ODBC Ordering !!
                ,p_hour,p_minute,p_second);
  return string;
}

// Makes an catalog identifier string (possibly quoted on both sides)
XString
SQLInfoMariaDB::GetSQLDDLIdentifier(XString p_identifier) const
{
  return p_identifier;
}

// Changes to parameters before binding to an ODBC HSTMT handle
void
SQLInfoMariaDB::DoBindParameterFixup(SQLSMALLINT& /*p_sqlDatatype*/,SQLULEN& /*p_columnSize*/,SQLSMALLINT& /*p_scale*/,SQLLEN& /*p_bufferSize*/,SQLLEN* /*p_indicator*/) const
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
SQLInfoMariaDB::GetCATALOGMetaTypes(int p_type) const
{
  UNREFERENCED_PARAMETER(p_type);
  return "";
}

// Get SQL to check if a table already exists in the database
XString
SQLInfoMariaDB::GetCATALOGTableExists(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  // Still to do in MySQL
  return "";
}

XString
SQLInfoMariaDB::GetCATALOGTablesList(XString& p_schema,XString& p_pattern) const
{
  return GetCATALOGTableAttributes(p_schema,p_pattern);
}

XString
SQLInfoMariaDB::GetCATALOGTableAttributes(XString& p_schema,XString& p_tablename) const
{
  XString sql;
  sql = "SELECT table_catalog\n"
        "      ,table_schema\n"
        "      ,table_name\n"
        "      ,case table_type\n"
        "            when 'BASE TABLE' THEN 'TABLE'\n"
        "            else 'UNKNOWN'\n"
        "       end as table_type\n"
        "      ,table_comment\n"
        "      ,table_catalog || '.' || table_schema || '.' || table_name as fullname\n"
        "      ,engine\n"
        "      ,0 as temporary\n" 
        "  FROM information_schema.tables\n"  
        " WHERE table_type = 'BASE TABLE'\n"
        "   AND table_schema NOT IN ('mysql','sys','performance_schema')\n";
  if(!p_schema.IsEmpty())
  {
    sql += "   AND table_schema = ?\n";
  }
  if(!p_tablename.IsEmpty())
  {
    sql += "   AND table_name ";
    sql += p_tablename.Find('%') >= 0 ? "LIKE" : "=";
    sql += " ?\n";
  }
  sql += " ORDER BY 1,2,3";
  return sql;
}

XString
SQLInfoMariaDB::GetCATALOGTableSynonyms(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  // MS-Access cannot do this
  return XString();
}

XString
SQLInfoMariaDB::GetCATALOGTableCatalog(XString& p_schema,XString& p_tablename) const
{
  p_schema.Empty(); // do not bind as a parameter

  XString sql;
  sql = "SELECT table_catalog\n"
        "      ,table_schema\n"
        "      ,table_name\n"
        "      ,'SYSTEM TABLE' as table_type\n"
        "      ,table_comment\n"
        "      ,table_catalog || '.' || table_schema || '.' || table_name as fullname\n"
        "      ,engine\n"
        "      ,0 as temporary\n" 
        "  FROM information_schema.tables\n"  
        " WHERE ( table_type = 'SYSTEM VIEW'\n"
        "      OR table_schema IN ('mysql','sys','performance_schema'))\n";
  if(!p_schema.IsEmpty())
  {
    sql += "   AND table_schema = ?\n";
  }
  if(!p_tablename.IsEmpty())
  {
    sql += "   AND table_name ";
    sql += p_tablename.Find('%') >= 0 ? "LIKE" : "=";
    sql += " ?\n";
  }
  sql += " ORDER BY 1,2,3";
  return sql;
}

XString
SQLInfoMariaDB::GetCATALOGTableCreate(MetaTable& p_table,MetaColumn& /*p_column*/) const
{
  XString sql = "CREATE ";
  if(p_table.m_temporary)
  {
    sql += "TEMPORARY ";
  }
  sql += "TABLE " + p_table.m_table;
  return sql;
}

XString
SQLInfoMariaDB::GetCATALOGTableCreatePostfix(MetaTable& p_table,MetaColumn& /*p_column*/) const
{
  XString sql;
  if(p_table.m_temporary)
  {
    sql += "ENGINE = MEMORY";
  }
  return sql;
}

XString
SQLInfoMariaDB::GetCATALOGTableRename(XString p_schema,XString p_tablename,XString p_newname) const
{
  XString sql("RENAME TABLE" + p_schema + "." + p_tablename + " TO " + p_newname);
  return sql;
}

XString
SQLInfoMariaDB::GetCATALOGTableDrop(XString p_schema,XString p_tablename,bool p_ifExist /*= false*/,bool p_restrict /*= false*/,bool p_cascade /*= false*/) const
{
  XString sql("DROP TABLE ");
  if (p_ifExist)
  {
    sql += "IF EXISTS ";
  }
  sql += p_schema + "." + p_tablename;
  if (p_restrict)
  {
    sql += " RESTRICT";
  }
  else if (p_cascade)
  {
    sql += " CASCADE";
  }
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL TEMPORARY TABLE FUNCTIONS

XString 
SQLInfoMariaDB::GetCATALOGTemptableCreate(XString p_schema,XString p_tablename,XString p_select) const
{
  // BEWARE: THIS IS A GUESS. 
  return "CREATE TEMPORARY TABLE " + p_schema + "." + p_tablename + "\nAS " + p_select;
}

XString 
SQLInfoMariaDB::GetCATALOGTemptableIntoTemp(XString /*p_schema*/,XString p_tablename,XString p_select) const
{
  return "INSERT INTO " + p_tablename + "\n" + p_select;
}

XString 
SQLInfoMariaDB::GetCATALOGTemptableDrop(XString /*p_schema*/,XString p_tablename) const
{
  return "DROP TABLE " + p_tablename;
}

//////////////////////////////////////////////////////////////////////////
// ALL COLUMN FUNCTIONS

XString 
SQLInfoMariaDB::GetCATALOGColumnExists(XString /*p_schema*/,XString p_tablename,XString p_columnname) const
{
  return "";
}

XString 
SQLInfoMariaDB::GetCATALOGColumnList(XString& p_schema,XString& p_tablename) const
{
  XString column;
  return GetCATALOGColumnAttributes(p_schema,p_tablename,column);
}

XString
SQLInfoMariaDB::GetCATALOGColumnAttributes(XString& p_schema,XString& p_tablename,XString& p_columnname) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  p_columnname.MakeLower();

  XString query = "SELECT table_catalog  AS table_cat\n"
                  "      ,table_schema   AS table_schem\n"
                  "      ,table_name\n"
                  "      ,column_name\n"
                  "      ,case data_type\n"
                  "            when 'char'               then   1\n"
                  "            when 'varchar'            then  12\n"
                  "            when 'bigint'   then case locate('unsigned',column_type) > 0\n"
                  "                                 when true then -27\n"   // UBIGINT
                  "                                           else -25\n"   // SBIGINT
                  "                                 end\n"
                  "            when 'binary'             then  -2\n"
                  "            when 'bit'                then  -7\n"
                  "            when 'blob'               then  -4\n"  // longvarbinary
                  "            when 'bool'               then  -7\n"  // bit
                  "            when 'date'               then   9\n"  // date
                  "            when 'datetime'           then  93\n"  // TYPE TIMESTAMP
                  "            when 'decimal'            then   2\n"  // NUMERIC
                  "            when 'double'             then   8\n"
                  "            when 'double precision'   then   8\n"
                  "            when 'enum'               then  12\n"  // VARCHAR
                  "            when 'float'              then   7\n"
                  "            when 'int'      then case locate('unsigned',column_type) > 0\n"
                  "                                 when true then -18\n"   // ULONG
                  "                                           else   4\n"   // SLONG
                  "                                 end\n"
                  "            when 'integer'  then case locate('unsigned',column_type) > 0\n"
                  "                                 when true then -18\n"   // ULONG
                  "                                           else   4\n"   // SLONG
                  "                                 end\n"
                  "            when 'long varbinary'     then  -4\n"  // BLOB
                  "            when 'long varchar'       then  -1\n"
                  "            when 'longblob'           then  -4\n"
                  "            when 'longtext'           then  -1\n"
                  "            when 'mediumblob'         then  -4\n"
                  "            when 'mediumint' then case locate('unsigned',column_type) > 0\n"
                  "                                  when true then -18\n"   // ULONG
                  "                                            else   4\n"   // SLONG
                  "                                  end\n"
                  "            when 'mediumtext'         then  -1\n"
                  "            when 'numeric'            then   2\n"
                  "            when 'real'               then   7\n"
                  "            when 'set'                then  12\n"
                  "            when 'smallint' then case locate('unsigned',column_type) > 0\n"
                  "                                 when true then -17\n"   // USMALLINT
                  "                                           else   5\n"   // SMALLINT
                  "                                 end\n"
                  "            when 'text'               then  -1\n"
                  "            when 'time'               then  92\n"  // TYPE TIME
                  "            when 'timestamp'          then  93\n"
                  "            when 'tinyblob'           then  -3\n"  // varbinary
                  "            when 'tinyint'  then case locate('unsigned',column_type) > 0\n"
                  "                                 when true then -16\n"   // UTINYINT
                  "                                           else  -6\n"   // TINYINT
                  "                                 end\n"
                  "            when 'tinytext'           then  -1\n"
                  "            when 'varbinary'          then  -3\n"
                  "       end       as data_type\n"
                  "      ,ucase(if(column_type like '%%(%%)%%',concat(substring(column_type,1,locate('(',column_type)-1),substring(column_type,1+locate(')',column_type))),column_type)) as type_name\n"
                  "      ,case when data_type = 'bit'    then (numeric_precision+7)/8\n"
                  "            when data_type in('tinyint','smallint','mediumint','int','bigint','decimal') then numeric_precision\n"
                  "            when data_type = 'float'  then if(numeric_scale IS NULL,7,numeric_precision)\n"
                  "            when data_type = 'double' then if(numeric_scale IS NULL,15,numeric_precision)\n"
                  "            when data_type = 'date'   then 10\n"
                  "            when data_type = 'time'   then  8\n"
                  "            when data_type = 'year'   then  4\n"
                  "            when data_type in('timestamp','datetime') then 19\n"
                  "            else character_maximum_length\n"
                  "       end  as column_size\n"
                  "      ,case data_type\n"
                  "            when 'bit'        then 1\n"
                  "            when 'tinyint'    then 1\n"
                  "            when 'smallint'   then 2\n"
                  "            when 'int'        then 4\n"
                  "            when 'integer'    then 4\n"
                  "            when 'mediumint'  then 3\n"
                  "            when 'bigint'     then 20\n"
                  "            when 'real'       then 4\n"
                  "            when 'float'      then 8\n"
                  "            when 'double'     then 8\n"
                  "            when 'date'       then 6\n"
                  "            when 'time'       then 6\n"
                  "            when 'timestamp'  then 16\n"
                  "            when 'datetime'   then 16\n"
                  "            when 'guid'       then 16\n"
                  "            when 'year'       then 2\n"
                  "            when 'decimal'    then (numeric_precision + 2)\n"
                  "            else  character_octet_length\n"
                  "       end  as buffer_length\n"
                  "      ,Nvl(numeric_scale,datetime_precision) AS decimal_digits\n"
                  "      ,if(character_octet_length is not null,null,10) as num_prec_radix\n"
                  "      ,if(data_type='timestamp',1,if(is_nullable='YES',1,if(extra='auto_increment',1,0))) as nullable\n"
                  "      ,column_comment     as remarks\n"
                  "      ,column_default     as column_def\n"
                  "      ,case data_type\n"
                  "            when 'date'      then 9\n"   // DATETIME
                  "            when 'time'      then 9\n"   // DATETIME
                  "            when 'datetime'  then 9\n"   // DATETIME
                  "            when 'timestamp' then 9\n"   // DATETIME
                  "       end  as sql_data_type\n"
                  "      ,case data_type\n"
                  "            when 'date'      then 1\n"   // SQL_DATE
                  "            when 'time'      then 2\n"   // SQL_TIME
                  "            when 'datetime'  then 3\n"   // SQL_CODE_TIMESTAMP
                  "            when 'timestamp' then 3\n"   // SQL_CODE_TIMESTAMP
                  "       end  as sql_datetime_sub\n"
                  "      ,character_octet_length\n"
                  "      ,ordinal_position\n"
                  "      ,if(data_type = 'timestamp','YES',if(is_nullable = 'YES','YES',if(extra = 'auto_increment','YES','NO'))) as is_nullable\n"
                  "  FROM information_schema.columns\n"
                  " WHERE table_name   = '" + p_tablename + "'\n";
  if(!p_schema.IsEmpty())
  {
    query += "   AND table_schema = '" + p_schema + "'\n";
  }
  if(!p_columnname.IsEmpty())
  {
    query += "   AND column_name = '" + p_columnname + "'\n";
  }
  query += " ORDER BY ordinal_position ASC";
  return query;
}

XString 
SQLInfoMariaDB::GetCATALOGColumnCreate(MetaColumn& p_column) const
{
  XString sql = "ALTER TABLE "  + p_column.m_table  + "\n"
                "  ADD COLUMN " + p_column.m_column + " " + p_column.m_typename;
  p_column.GetPrecisionAndScale(sql);
  p_column.GetNullable(sql);
  p_column.GetDefault(sql);
  // m_position not used
  // m_length   not used
  // m_remarks  not used
  return sql;}

XString 
SQLInfoMariaDB::GetCATALOGColumnAlter(MetaColumn& p_column) const
{
  // The MODIFY keyword is a-typical
  XString sql = "ALTER  TABLE  " + p_column.m_table + "\n"
                "MODIFY COLUMN " + p_column.m_column + " " + p_column.m_typename;
  p_column.GetPrecisionAndScale(sql);
  p_column.GetNullable(sql);
  p_column.GetDefault(sql);
  // m_position not used
  // m_length   not used
  // m_remarks  not used
  return sql;
}

XString 
SQLInfoMariaDB::GetCATALOGColumnRename(XString p_schema,XString p_tablename,XString p_columnname,XString p_newname,XString /*p_datatype*/) const
{
  // General ISO syntax
  XString sql("ALTER  TABLE  " + p_tablename + "\n"
              "RENAME " + p_columnname + " TO " + p_newname + "\n");
  return sql;
}

XString 
SQLInfoMariaDB::GetCATALOGColumnDrop(XString p_schema,XString p_tablename,XString p_columnname) const
{
  XString sql("ALTER TABLE  " + p_tablename + "\n"
              " DROP COLUMN " + p_columnname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL INDICES FUNCTIONS

// All index functions
XString
SQLInfoMariaDB::GetCATALOGIndexExists(XString /*p_schema*/,XString p_tablename,XString p_indexname) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLStatistics instead (see SQLInfo class)
  return "";
}

XString
SQLInfoMariaDB::GetCATALOGIndexList(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLStatistics instead (see SQLInfo class)
  return "";
}

XString
SQLInfoMariaDB::GetCATALOGIndexAttributes(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_indexname*/) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLStatistics instead (see SQLInfo class)
  return "";
}

XString
SQLInfoMariaDB::GetCATALOGIndexCreate(MIndicesMap& p_indices,bool /*p_duplicateNulls /*= false*/) const
{
  // Get SQL to create an index for a table
  // CREATE [UNIQUE] INDEX [<schema>.]indexname ON [<schema>.]tablename(column [ASC|DESC] [,...]);
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

    // Descending column
    if(index.m_ascending != "A")
    {
      query += " DESC";
    }
  }
  query += ")";
  return query;
}

XString
SQLInfoMariaDB::GetCATALOGIndexDrop(XString p_schema,XString p_tablename,XString p_indexname) const
{
  XString sql = "ALTER TABLE " + p_schema + "." + p_tablename + "\n"
                " DROP INDEX " + p_indexname;
  return sql;
}

// Get extra filter expression for an index column
XString
SQLInfoMariaDB::GetCATALOGIndexFilter(MetaIndex& /*p_index*/) const
{
  return "";
}

//////////////////////////////////////////////////////////////////////////
// ALL PRIMARY KEY FUNCTIONS

XString
SQLInfoMariaDB::GetCATALOGPrimaryExists(XString /*p_schema*/,XString p_tablename) const
{
  // To be implemented
  return "";
}

XString
SQLInfoMariaDB::GetCATALOGPrimaryAttributes(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  // To be implemented
  return "";
}

// In MariaDB all primary keys are named "PRIMARY".
// You can add a constraint name, but it WILL be ignored!
XString
SQLInfoMariaDB::GetCATALOGPrimaryCreate(MPrimaryMap& p_primaries) const
{
  XString query("ALTER TABLE ");

  for(auto& prim : p_primaries)
  {
    if(prim.m_columnPosition == 1)
    {
      query += prim.m_schema + "." + prim.m_table + "\n";
      query += "  ADD PRIMARY KEY (";

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

// In MariaDB all primary keys are named "PRIMARY".
XString
SQLInfoMariaDB::GetCATALOGPrimaryDrop(XString p_schema,XString p_tablename,XString /*p_constraintname*/) const
{
  XString sql("ALTER TABLE " + p_schema + "." + p_tablename + "\n"
              " DROP CONSTRAINT PRIMARY");
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL FOREIGN KEY FUNCTIONS

XString
SQLInfoMariaDB::GetCATALOGForeignExists(XString /*p_schema*/,XString p_tablename,XString p_constraintname) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLForeignKeys instead (see SQLInfo class)
  return "";
}

XString
SQLInfoMariaDB::GetCATALOGForeignList(XString& /*p_schema*/,XString& /*p_tablename*/,int /*p_maxColumns*/ /*=SQLINFO_MAX_COLUMNS*/) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLForeignKeys instead (see SQLInfo class)
  return "";
}

XString
SQLInfoMariaDB::GetCATALOGForeignAttributes(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_constraintname*/,bool /*p_referenced = false*/,int /*p_maxColumns*/ /*=SQLINFO_MAX_COLUMNS*/) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLForeignKeys instead (see SQLInfo class)
  return "";
}

XString
SQLInfoMariaDB::GetCATALOGForeignCreate(MForeignMap& p_foreigns) const
{
  // Get first record
  MetaForeign& foreign = p_foreigns.front();

  // Construct the correct tablename
  XString table(foreign.m_fkTableName);
  XString primary(foreign.m_pkTableName);

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
    extra = true;
  }

  // Add references primary table
  query += ")\n      REFERENCES " + primary + "(";

  // Add the primary key columns
  extra = false;
  for(auto& key : p_foreigns)
  {
    if(extra) query += ",";
    query += key.m_pkColumnName;
    extra = true;
  }
  query += ")";

  // Add all relevant options
  switch(foreign.m_match)
  {
    case SQL_MATCH_PARTIAL: query += "\n      MATCH PARTIAL"; break;
    case SQL_MATCH_SIMPLE:  query += "\n      MATCH SIMPLE";  break;
    case SQL_MATCH_FULL:    query += "\n      MATCH FULL";    break;
    default:                // In essence: MATCH FULL, but that's already the default
                            break;
  }
  switch(foreign.m_updateRule)
  {
    case SQL_CASCADE:     query += "\n      ON UPDATE CASCADE";     break;
    case SQL_SET_NULL:    query += "\n      ON UPDATE SET NULL";    break;
    case SQL_SET_DEFAULT: query += "\n      ON UPDATE SET DEFAULT"; break;
    case SQL_NO_ACTION:   query += "\n      ON UPDATE NO ACTION";   break;
    case SQL_RESTRICT:    query += "\n      ON UPDATE NO RESTRICT"; break;
    default:              // In essence: ON UPDATE RESTRICT, but that's already the default
                          break;
  }
  switch(foreign.m_deleteRule)
  {
    case SQL_CASCADE:     query += "\n      ON DELETE CASCADE";     break;
    case SQL_SET_NULL:    query += "\n      ON DELETE SET NULL";    break;
    case SQL_SET_DEFAULT: query += "\n      ON DELETE SET DEFAULT"; break;
    case SQL_NO_ACTION:   query += "\n      ON DELETE NO ACTION";   break;
    case SQL_RESTRICT:    query += "\n      ON DELETE NO RESTRICT"; break;
    default:              // In essence: ON DELETE RESTRICT, but that's already the default
                          break;
  }
  return query;
}

XString
SQLInfoMariaDB::GetCATALOGForeignAlter(MForeignMap& p_original, MForeignMap& p_requested) const
{
  // Make sure we have both
  if(p_original.empty() || p_requested.empty())
  {
    return "";
  }

  MetaForeign& original  = p_original.front();
  MetaForeign& requested = p_requested.front();

  // Construct the correct tablename
  XString table(original.m_fkTableName);

  // The base foreign key command
  XString query = "ALTER TABLE " + table + "\n"
                  "ALTER CONSTRAINT " + original.m_foreignConstraint + "\n";

  // Add all relevant options
  if(original.m_deferrable != requested.m_deferrable)
  {
    switch(requested.m_deferrable)
    {
      case SQL_INITIALLY_DEFERRED:  query += "\n      INITIALLY DEFERRED"; break;
      case SQL_INITIALLY_IMMEDIATE: query += "\n      DEFERRABLE";         break;
      case SQL_NOT_DEFERRABLE:      query += "\n      NOT DEFERRABLE";     break;
      default:                      break;
    }
  }
  if(original.m_match != requested.m_match)
  {
    switch(requested.m_match)
    {
      case SQL_MATCH_FULL:    query += "\n      MATCH FULL";    break;
      case SQL_MATCH_PARTIAL: query += "\n      MATCH PARTIAL"; break;
      case SQL_MATCH_SIMPLE:  query += "\n      MATCH SIMPLE";  break;
    }
  }
  if(original.m_updateRule != requested.m_updateRule)
  {
    switch(requested.m_updateRule)
    {
      case SQL_CASCADE:     query += "\n      ON UPDATE CASCADE";     break;
      case SQL_SET_NULL:    query += "\n      ON UPDATE SET NULL";    break;
      case SQL_SET_DEFAULT: query += "\n      ON UPDATE SET DEFAULT"; break;
      case SQL_NO_ACTION:   query += "\n      ON UPDATE NO ACTION";   break;
      default:              // In essence: ON UPDATE RESTRICT, but that's already the default
      case SQL_RESTRICT:    break;
    }
  }
  if(original.m_deleteRule != requested.m_deleteRule)
  {
    switch(requested.m_deleteRule)
    {
      case SQL_CASCADE:     query += "\n      ON DELETE CASCADE";     break;
      case SQL_SET_NULL:    query += "\n      ON DELETE SET NULL";    break;
      case SQL_SET_DEFAULT: query += "\n      ON DELETE SET DEFAULT"; break;
      case SQL_NO_ACTION:   query += "\n      ON DELETE NO ACTION";   break;
      default:              // In essence: ON DELETE RESTRICT, but that's already the default
      case SQL_RESTRICT:    break;
    }
  }
  return query;
}

XString
SQLInfoMariaDB::GetCATALOGForeignDrop(XString p_schema,XString p_tablename,XString p_constraintname) const
{
  XString sql("ALTER TABLE " + p_schema + "." + p_tablename + "\n"
              " DROP CONSTRAINT " + p_constraintname);
  return sql;
}

//////////////////////////
// All default constraints
XString
SQLInfoMariaDB::GetCATALOGDefaultExists(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_column*/) const
{
  return "";
}

XString
SQLInfoMariaDB::GetCATALOGDefaultList(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  return "";
}

XString
SQLInfoMariaDB::GetCATALOGDefaultAttributes(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_column*/) const
{
  return "";
}

XString
SQLInfoMariaDB::GetCATALOGDefaultCreate(XString /*p_schema*/,XString /*p_tablename*/,XString /*p_constraint*/,XString /*p_column*/,XString /*p_code*/) const
{
  return "";
}

XString
SQLInfoMariaDB::GetCATALOGDefaultDrop(XString /*p_schema*/,XString /*p_tablename*/,XString /*p_constraint*/) const
{
  return "";
}

/////////////////////////
// All check constraints

XString
SQLInfoMariaDB::GetCATALOGCheckExists(XString  /*p_schema*/,XString  /*p_tablename*/,XString  /*p_constraint*/) const
{
  return "";
}

XString
SQLInfoMariaDB::GetCATALOGCheckList(XString  /*p_schema*/,XString  /*p_tablename*/) const
{
  return "";
}

XString
SQLInfoMariaDB::GetCATALOGCheckAttributes(XString  /*p_schema*/,XString  /*p_tablename*/,XString  /*p_constraint*/) const
{
  return "";
}

XString
SQLInfoMariaDB::GetCATALOGCheckCreate(XString  /*p_schema*/,XString  /*p_tablename*/,XString  /*p_constraint*/,XString /*p_condition*/) const
{
  return "";
}

XString
SQLInfoMariaDB::GetCATALOGCheckDrop(XString  /*p_schema*/,XString  /*p_tablename*/,XString  /*p_constraint*/) const
{
  return "";
}

//////////////////////////////////////////////////////////////////////////
// ALL TRIGGER FUNCTIONS

XString
SQLInfoMariaDB::GetCATALOGTriggerExists(XString /*p_schema*/, XString p_tablename, XString p_triggername) const
{
  return "";
}

XString
SQLInfoMariaDB::GetCATALOGTriggerList(XString& p_schema,XString& p_tablename) const
{
  XString triggername;
  return GetCATALOGTriggerAttributes(p_schema,p_tablename,triggername);
}

XString
SQLInfoMariaDB::GetCATALOGTriggerAttributes(XString& p_schema,XString& p_tablename,XString& p_triggername) const
{
  XString sql;
  sql = "SELECT event_object_catalog\n"
        "      ,event_object_schema\n"
        "      ,event_object_table\n"
        "      ,trigger_name\n"
        "      ,'' AS trigger_remarks\n"
        "      ,action_order  AS trigger_position\n"
        "      ,CASE action_timing\n"
        "            WHEN 'AFTER' THEN FALSE ELSE TRUE\n"
        "       END AS trigger_before\n"
        "      ,CASE event_manipulation\n"
        "            WHEN 'INSERT' THEN TRUE ELSE FALSE\n"
        "       END AS trigger_insert\n"
        "      ,CASE event_manipulation\n"
        "            WHEN 'UPDATE' THEN TRUE ELSE FALSE\n"
        "       END AS trigger_update\n"
        "      ,CASE event_manipulation\n"
        "            WHEN 'DELETE' THEN TRUE ELSE FALSE\n"
        "       END AS trigger_delete\n"
        "      ,FALSE AS trigger_select\n"
        "      ,FALSE AS trigger_session\n"
        "      ,FALSE AS trigger_transaction\n"
        "      ,FALSE AS trigger_rollback\n"
        "      ,''    AS trigger_referencing\n"
        "      ,TRUE  AS trigger_enabled\n"
        "      ,action_statement AS trigger_source\n"
        "  FROM information_schema.triggers\n";
  if(!p_schema.IsEmpty())
  {
    sql += " WHERE event_object_schema = ?\n";
  }
  if(!p_tablename.IsEmpty())
  {
    sql += p_schema.IsEmpty() ? " WHERE " : "   AND ";
    sql += "event_object_table = ?\n";
  }
  if(!p_triggername.IsEmpty())
  {
    sql += p_schema.IsEmpty() && p_tablename.IsEmpty() ? " WHERE " : "   AND ";
    sql += "trigger_name = ?\n";
  }
  sql += " ORDER BY 1,2,3,4";
  return sql;
}

XString
SQLInfoMariaDB::GetCATALOGTriggerCreate(MetaTrigger& /*p_trigger*/) const
{
  return "";
}

XString
SQLInfoMariaDB::GetCATALOGTriggerDrop(XString /*p_schema*/, XString p_tablename, XString p_triggername) const
{
  return "";
}

//////////////////////////////////////////////////////////////////////////
// ALL SEQUENCE FUNCTIONS

XString
SQLInfoMariaDB::GetCATALOGSequenceExists(XString p_schema, XString p_sequence) const
{
  p_schema.Empty();
  p_sequence.MakeLower();

  XString sql = "SELECT COUNT(*)\n"
                "  FROM information_schema.tables tab\n"
                " WHERE tab.table_type = 'SEQUENCE'\n";
                "   AND sequence_name  = '";
  sql += p_sequence;
  sql += "\'";
  return sql;
}

XString
SQLInfoMariaDB::GetCATALOGSequenceList(XString& p_schema,XString& p_pattern) const
{
  p_schema.MakeLower();
  p_pattern.MakeLower();
  if(p_pattern.Find('%') < 0)
  {
    p_pattern = "%" + p_pattern + "%";
  }

  XString sql = "SELECT tab.table_catalog as catalog_name\n"
                "      ,tab.table_schema  as schema_name\n"
                "      ,tab.table_name    as sequence_name\n"
                "      ,0 AS current_value\n"
                "      ,0 AS minimal_value\n"
                "      ,0 AS seq_increment\n"
                "      ,0 AS cache\n"
                "      ,0 AS cycle\n"
                "      ,0 AS ordering\n"
                "  FROM information_schema.tables tab\n"
                " WHERE tab.table_type = 'SEQUENCE'";
  if(!p_schema.IsEmpty())
  {
    sql += "\n   AND table_schema = ?";
  }
  if(!p_pattern.IsEmpty())
  {
    sql += "\n   AND table_name LIKE ?";
  }
  return sql;
}

XString
SQLInfoMariaDB::GetCATALOGSequenceAttributes(XString& p_schema,XString& p_sequence) const
{
  p_schema.MakeLower();
  p_sequence.MakeLower();
  XString table;
  if(!p_schema.IsEmpty())
  {
    table = p_schema + ".";
  }
  table += p_sequence;

  XString sql = "SELECT tab.table_catalog   AS catalog_name\n"
                "      ,tab.table_schema    AS schema_name\n"
                "      ,tab.table_name      AS sequence_name\n"
                "      ,seq.start_value     AS current_value\n"
                "      ,seq.minimum_value   AS minimal_value\n"
                "      ,seq.increment       AS seq_increment\n"
                "      ,seq.cache_size      AS cache\n"
                "      ,seq.cycle_option    AS cycle\n"
                "      ,0                   AS ordering\n"
                "  FROM information_schema.tables as tab\n"
                "      ," + table + " as seq\n"
                " WHERE tab.table_type = 'SEQUENCE'";
  if(!p_schema.IsEmpty())
  {
    sql += "\n   AND tab.table_schema = ?";
  }
  if(!p_sequence.IsEmpty())
  {
    sql += "\n   AND tab.table_name  = ?";
  }
  return sql;
}

XString
SQLInfoMariaDB::GetCATALOGSequenceCreate(MetaSequence& p_sequence) const
{
  XString sql("CREATE OR REPLACE SEQUENCE ");

  sql += p_sequence.m_sequenceName;
  sql.AppendFormat("\n START WITH %-12.0f", p_sequence.m_currentValue);
  sql.AppendFormat("\n INCREMENT BY %d",    p_sequence.m_increment);

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

XString
SQLInfoMariaDB::GetCATALOGSequenceDrop(XString /*p_schema*/,XString p_sequence) const
{
  XString sql("DROP SEQUENCE " + p_sequence);
  return  sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL VIEW FUNCTIONS

XString 
SQLInfoMariaDB::GetCATALOGViewExists(XString& /*p_schema*/,XString& /*p_viewname*/) const
{
  return "";
}

XString 
SQLInfoMariaDB::GetCATALOGViewList(XString& p_schema,XString& p_pattern) const
{
  return GetCATALOGViewAttributes(p_schema,p_pattern);
}

XString
SQLInfoMariaDB::GetCATALOGViewAttributes(XString& p_schema,XString& p_viewname) const
{
  XString sql;
  sql = "SELECT table_catalog\n"
        "      ,table_schema\n"
        "      ,table_name\n"
        "      ,'VIEW' as table_type\n"
        "      ,table_comment\n"
        "      ,table_catalog || '.' || table_schema || '.' || table_name as fullname\n"
        "      ,engine\n"
        "      ,0 as temporary\n" 
        "  FROM information_schema.tables\n"  
        " WHERE table_type = 'VIEW'\n"
        "   AND table_schema NOT IN ('mysql','sys','performance_schema')\n";
  if(!p_schema.IsEmpty())
  {
    sql += "   AND table_schema = ?\n";
  }
  if(!p_viewname.IsEmpty())
  {
    sql += "   AND table_name ";
    sql += p_viewname.Find('%') >= 0 ? "LIKE" : "=";
    sql += " ?\n";
  }
  sql += " ORDER BY 1,2,3";
  return sql;
}

XString
SQLInfoMariaDB::GetCATALOGViewText(XString& /*p_schema*/,XString& /*p_viewname*/) const
{
  // Cannot query this, Use ODBC functions
  return "";
}

XString
SQLInfoMariaDB::GetCATALOGViewCreate(XString /*p_schema*/,XString p_viewname,XString p_contents,bool /*p_ifexists /*= true*/) const
{
  return "CREATE VIEW " + p_viewname + "\n" + p_contents;
}

XString 
SQLInfoMariaDB::GetCATALOGViewRename(XString p_schema,XString p_viewname,XString p_newname)    const
{
  return "";
}

XString 
SQLInfoMariaDB::GetCATALOGViewDrop(XString /*p_schema*/,XString p_viewname,XString& p_precursor) const
{
  p_precursor.Empty();
  return "DROP VIEW " + p_viewname;
}

// All Privilege functions
XString
SQLInfoMariaDB::GetCATALOGTablePrivileges(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  return "";
}

XString 
SQLInfoMariaDB::GetCATALOGColumnPrivileges(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_columnname*/) const
{
  return "";
}

XString
SQLInfoMariaDB::GetCATALOGSequencePrivilege(XString& /*p_schema*/,XString& /*p_sequence*/) const
{
  return "";
}

XString 
SQLInfoMariaDB::GetCATALOGGrantPrivilege(XString /*p_schema*/,XString p_objectname,XString p_privilege,XString p_grantee,bool p_grantable)
{
  XString sql;
  p_grantee.MakeLower();

  // MariaDB does not know the concept of "PUBLIC"
  if(p_grantee.Compare("public"))
  {
    sql.Format("GRANT %s ON %s TO %s", p_privilege.GetString(), p_objectname.GetString(), p_grantee.GetString());
    if (p_grantable)
    {
      sql += " WITH GRANT OPTION";
    }
  }
  return sql;
}

XString 
SQLInfoMariaDB::GetCATALOGRevokePrivilege(XString /*p_schema*/,XString p_objectname,XString p_privilege,XString p_grantee)
{
  XString sql;
  p_grantee.MakeLower();

  // MariaDB does not know the concept of "PUBLIC"
  if(p_grantee.Compare("public"))
  {
    sql.Format("REVOKE %s ON %s FROM %s", p_privilege.GetString(), p_objectname.GetString(), p_grantee.GetString());
  }
  return sql;
}

// All Synonym functions
XString
SQLInfoMariaDB::GetCATALOGSynonymList(XString& /*p_schema*/,XString& /*p_pattern*/) const
{
  // Not implemented yet
  return "";
}

XString
SQLInfoMariaDB::GetCATALOGSynonymAttributes(XString& /*p_schema*/,XString& /*p_synonym*/) const
{
  // Not implemented yet
  return "";
}

XString
SQLInfoMariaDB::GetCATALOGSynonymCreate(XString& /*p_schema*/,XString& /*p_synonym*/,XString /*p_forObject*/,bool /*p_private = true*/) const
{
  // Not implemented yet
  return "";
}

XString
SQLInfoMariaDB::GetCATALOGSynonymDrop(XString& /*p_schema*/,XString& /*p_synonym*/,bool /*p_private = true*/) const
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
SQLInfoMariaDB::GetPSMProcedureExists(XString p_schema, XString p_procedure) const
{
  XString sql;
  sql = "SELECT SELECT COUNT(*)\n"
        "  FROM information_schema.routines\n";
  if(!p_schema.IsEmpty())
  { 
    sql += "   AND routine_schema = '" + p_schema + "'\n";
  }
  if(!p_procedure.IsEmpty())
  {
    sql += "   AND routine_name = '" + p_procedure + "'";
  }
  return sql;
}

XString
SQLInfoMariaDB::GetPSMProcedureList(XString& p_schema) const
{
  XString sql;
  sql = "SELECT routine_catalog\n"
        "      ,routine_schema\n"
        "      ,routine_name\n"
        "  FROM information_schema.routines fun\n";
  if (!p_schema.IsEmpty())
  {
    sql += " WHERE routine_schema = ?\n";
  }
  sql += " ORDER BY 1,2,3";

  return sql;
}

XString
SQLInfoMariaDB::GetPSMProcedureAttributes(XString& p_schema,XString& p_procedure) const
{
  XString sql;
  sql = "SELECT routine_catalog\n"
        "      ,routine_schema\n"
        "      ,routine_name\n"
        "      ,(SELECT COUNT(*)\n"
        "          FROM information_schema.parameters par\n"
        "         WHERE par.specific_name    = fun.specific_name\n"
        "           AND par.specific_catalog = fun.routine_catalog\n"
        "           AND par.specific_schema  = fun.routine_schema\n"
        "           AND par.parameter_mode IN ('IN','INOUT')) as input_parameters\n"
        "      ,(SELECT COUNT(*)\n"
        "          FROM information_schema.parameters par\n"
        "         WHERE par.specific_name    = fun.specific_name\n"
        "           AND par.specific_catalog = fun.routine_catalog\n"
        "           AND par.specific_schema  = fun.routine_schema\n"
        "           AND par.parameter_mode IN ('OUT','INOUT')) as output_parameters\n"
        "      ,(SELECT COUNT(*)\n"
        "          FROM information_schema.parameters par\n"
        "         WHERE par.specific_name    = fun.specific_name\n"
        "           AND par.specific_catalog = fun.routine_catalog\n"
        "           AND par.specific_schema  = fun.routine_schema\n"
        "           AND par.data_type        = 'set') as result_sets\n"
        "      ,'' as remarks\n"
        "      ,CASE routine_type\n"
        "            WHEN 'PROCEDURE' THEN 1\n"
        "            WHEN 'FUNCTION'  THEN 2\n"
        "                             ELSE 3\n"
        "       end as procedure_type\n"
        "      ,routine_definition\n"
        "  FROM information_schema.routines fun\n";
  if(!p_schema.IsEmpty())
  { 
    sql += " WHERE routine_schema = ?\n";
  }
  if(!p_procedure.IsEmpty())
  {
    sql += p_schema.IsEmpty() ? " WHERE " : "   AND ";
    sql += "routine_name ";
    sql += p_procedure.Find('%') >= 0 ? "LIKE" : "=";
    sql += " ?\n";
  }
  sql += " ORDER BY 1,2,3";
  return sql;
}

XString
SQLInfoMariaDB::GetPSMProcedureSourcecode(XString p_schema, XString p_procedure) const
{
  // Source-code already gotten with attributes
  return "";
}

XString
SQLInfoMariaDB::GetPSMProcedureCreate(MetaProcedure& /*p_procedure*/) const
{
  return "";
}

XString
SQLInfoMariaDB::GetPSMProcedureDrop(XString /*p_schema*/, XString p_procedure,bool /*p_function /*=false*/) const
{
  return "";
}

XString
SQLInfoMariaDB::GetPSMProcedureErrors(XString p_schema,XString p_procedure) const
{
  return "";
}

XString
SQLInfoMariaDB::GetPSMProcedurePrivilege(XString& /*p_schema*/,XString& /*p_procedure*/) const
{
  return "";
}

// And it's parameters
XString
SQLInfoMariaDB::GetPSMProcedureParameters(XString& p_schema,XString& p_procedure) const
{
  XString sql;

  sql = "SELECT par.specific_catalog\n"
        "      ,par.specific_schema\n"
        "      ,fun.routine_name\n"
        "      ,par.parameter_name\n"
        "      ,case par.parameter_mode\n"
        "            when 'IN'    then 1\n"
        "            when 'OUT'   then 4\n"
        "            when 'INOUT' then 2\n"
        "       end as columntype\n"
        "      ,case par.data_type\n"
        "            when 'varchar'  then 1\n"
        "            when 'int'      then 4\n"
        "            when 'decimal'  then 2\n"
        "            when 'tinyint'  then -6\n"
        "            when 'bigint'   then -5\n"
        "            when 'text'     then 1\n"
        "            when 'longtext' then -1\n"
        "            when 'datetime' then 11\n"
        "                            else 1\n"
        "       end as datatype\n"
        "      ,par.data_type as typename\n"
        "      ,par.character_maximum_length\n"
        "      ,par.numeric_precision\n"
        "      ,par.numeric_scale\n"
        "      ,10 as numeric_precision_radix\n"
        "      ,1 as is_nullable\n"
        "      ,'' as remarks\n"
        "      ,'' as parameter_default\n"
        "      ,1 as datatype3\n"
        "      ,par.datetime_precision as subtype\n"
        "      ,par.character_octet_length\n"
        "      ,par.ordinal_position\n"
        "      ,'YES' as isNullable\n"
        "  FROM information_schema.parameters par\n"
        "      ,information_schema.routines fun\n"
        " WHERE par.specific_catalog = fun.routine_catalog\n"
        "   AND par.specific_schema  = fun.routine_schema\n"
        "   AND par.specific_name    = fun.specific_name\n";
  if(!p_schema.IsEmpty())
  {
    sql += "   AND fun.routine_schema = ?\n";
  }
  if(!p_procedure.IsEmpty())
  {
    sql += "   AND fun.routine_name    = ?\n";
  }
  sql += " ORDER BY 1,2,3,18";

  return sql;
}

//////////////////////////////////////////////////////////////////////////
//
// ALL PSM LANGUAGE ELEMENTS
//
//////////////////////////////////////////////////////////////////////////

XString
SQLInfoMariaDB::GetPSMDeclaration(bool    /*p_first*/
                               ,XString p_variable
                               ,int     p_datatype
                               ,int     p_precision /*= 0 */
                               ,int     p_scale     /*= 0 */
                               ,XString p_default   /*= ""*/
                               ,XString /*p_domain    = ""*/
                               ,XString /*p_asColumn  = ""*/) const
{
  XString line;
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
  line += ";\n";
  return line;
}

XString
SQLInfoMariaDB::GetPSMAssignment(XString p_variable,XString p_statement /*=""*/) const
{
  XString line(p_variable);
  line += " [?=] ";
  if(!p_statement.IsEmpty())
  {
    line += p_statement;
    line += ";";
  }
  return line;
}

XString
SQLInfoMariaDB::GetPSMIF(XString p_condition) const
{
  XString line("IF (");
  line += p_condition;
  line += ") THEN\n";
  return line;
}

XString
SQLInfoMariaDB::GetPSMIFElse() const
{
  return "ELSE\n";
}

XString
SQLInfoMariaDB::GetPSMIFEnd() const
{
  return "END IF;\n";
}

XString
SQLInfoMariaDB::GetPSMWhile(XString p_condition) const
{
  return "WHILE (" + p_condition + ") DO\n";
}

XString
SQLInfoMariaDB::GetPSMWhileEnd() const
{
  return "END WHILE;\n";
}

XString
SQLInfoMariaDB::GetPSMLOOP() const
{
  return "LOOP\n";
}

XString
SQLInfoMariaDB::GetPSMLOOPEnd() const
{
  return "END LOOP;\n";
}

XString
SQLInfoMariaDB::GetPSMBREAK() const
{
  return "LEAVE\n";
}

XString
SQLInfoMariaDB::GetPSMRETURN(XString p_statement /*= ""*/) const
{
  return "RETURN " + p_statement;
}

XString
SQLInfoMariaDB::GetPSMExecute(XString p_procedure,MParameterMap& p_parameters) const
{
  XString line;
  line.Format("EXECUTE %s USING ",p_procedure.GetString());
  bool doMore = false;

  for(auto& param : p_parameters)
  {
    if(doMore) line += ",";
    doMore = true;

    line += "@";
    line += param.m_parameter;
  }
  line += ";\n";
  return line;
}

// The CURSOR
XString
SQLInfoMariaDB::GetPSMCursorDeclaration(XString p_cursorname,XString p_select) const
{
  return "DECLARE " + p_cursorname + " CURSOR FOR " + p_select + ";";
}

XString
SQLInfoMariaDB::GetPSMCursorFetch(XString p_cursorname,std::vector<XString>& /*p_columnnames*/,std::vector<XString>& p_variablenames) const
{
  XString query = "FETCH " + p_cursorname + " INTO ";
  bool moreThenOne = false;

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

XString
SQLInfoMariaDB::GetPSMExceptionCatchNoData() const
{
  return "DECLARE EXIT HANDLER FOR SQLSTATE '02000'\n";
  // Must be followed by a (block)statement
}

XString
SQLInfoMariaDB::GetPSMExceptionCatch(XString p_sqlState) const
{
  return "DECLARE EXIT HANDLER FOR SQLSTATE '" + p_sqlState + "'\n";
  // Must be followed by a (block)statement
}

XString
SQLInfoMariaDB::GetPSMExceptionRaise(XString p_sqlState) const
{
  return "SIGNAL SQLSTATE '" + p_sqlState + "'\n";
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
SQLInfoMariaDB::GetSESSIONMyself() const
{
  return "";
}

XString
SQLInfoMariaDB::GetSESSIONExists(XString p_sessionID) const
{
  return "";
}

XString
SQLInfoMariaDB::GetSESSIONList() const
{
  return "";
}

XString
SQLInfoMariaDB::GetSESSIONAttributes(XString p_sessionID) const
{
  return "";
}

//////////////////////////////////////////////////////////////////////////
// Transactions

XString
SQLInfoMariaDB::GetSESSIONConstraintsDeferred() const
{
  // MySQL cannot defer constraints
  return "";
}

XString
SQLInfoMariaDB::GetSESSIONConstraintsImmediate() const
{
  // MySQL constraints are always active
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
SQLInfoMariaDB::DoSQLCall(SQLQuery* /*p_query*/,XString& /*p_schema*/,XString& /*p_procedure*/)
{
  return nullptr;
}

// Calling a stored function with named parameters, returning a value
SQLVariant*
SQLInfoMariaDB::DoSQLCallNamedParameters(SQLQuery* /*p_query*/,XString& /*p_schema*/,XString& /*p_procedure*/)
{
  return nullptr;
}

//////////////////////////////////////////////////////////////////////////
//
// PRIVATE PART
//
//////////////////////////////////////////////////////////////////////////

void
SQLInfoMariaDB::DetectOracleMode()
{
  XString sql = "SELECT INSTR(@@sql_mode,'ORACLE') > 0 AS IsOracle";

  try
  {
    SQLQuery query(m_database);
    m_oracleMode = query.DoSQLStatementScalar(sql)->GetAsBoolean();
  }
  catch(StdException& /*ex*/)
  {
    m_oracleMode = false;
  }
}

// End of namespace
}
