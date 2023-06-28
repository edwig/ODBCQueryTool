////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoSQLServer.cpp
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
#include "SQLInfoSQLServer.h"
#include "SQLQuery.h"
#include "sqlncli.h"

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
XString
SQLInfoSQLServer::GetRDBMSVendorName() const
{
  // The name of the database vendor
  return "Microsoft";
}

// Get the physical database name
XString
SQLInfoSQLServer::GetRDBMSPhysicalDatabaseName() const
{
  XString query = "SELECT DB_NAME()";
  SQLQuery qry(m_database);
  qry.DoSQLStatement(query);
  if(qry.GetRecord())
  {
    return qry.GetColumn(1)->GetAsChar();
  }
  return XString("");
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

// Supports functions at the place of table columns in create/alter index statement
bool
SQLInfoSQLServer::GetRDBMSSupportsFunctionalIndexes() const
{
  return false;
}

// Gets the maximum length of an SQL statement
unsigned long
SQLInfoSQLServer::GetRDBMSMaxStatementLength() const
{
  // Current limit = 1 MB
  return 1048576; // (1024 * 1024);
}

// Database must commit DDL commands in a transaction
bool
SQLInfoSQLServer::GetRDBMSMustCommitDDL() const
{
  return false;
}

// Correct maximum precision,scale for a NUMERIC datatype
void
SQLInfoSQLServer::GetRDBMSNumericPrecisionScale(SQLULEN& p_precision, SQLSMALLINT& p_scale) const
{
  // Max precision for numerics is 38
  if(p_precision > NUMERIC_MAX_PRECISION)
  {
    p_precision = NUMERIC_MAX_PRECISION;
  }

  // Default scale is also the max for parameters
  if(p_scale > NUMERIC_DEFAULT_SCALE)
  {
    p_scale = NUMERIC_DEFAULT_SCALE;
  }

  // In case of conversion from other databases
  if(p_precision == 0 && p_scale == 0)
  {
    p_precision = NUMERIC_MAX_PRECISION;
    p_scale     = NUMERIC_DEFAULT_SCALE;
  }

  // Scale MUST be smaller than the precision
  if(p_scale >= p_precision)
  {
    p_scale = (SQLSMALLINT) (p_precision - 1);
  }
}

// Maximum for a VARCHAR to be handled without AT-EXEC data. Assume NVARCHAR is half that size!
int
SQLInfoSQLServer::GetRDBMSMaxVarchar() const
{
  return (DBMAXCHAR - 1);
}

// KEYWORDS

// Keyword for the current date and time
XString
SQLInfoSQLServer::GetKEYWORDCurrentTimestamp() const
{
  return "GETDATE()";
}

// String for the current date
XString
SQLInfoSQLServer::GetKEYWORDCurrentDate() const
{
  return "GETDATE()";
}

// Get the concatenation operator
XString
SQLInfoSQLServer::GetKEYWORDConcatanationOperator() const
{
  return "+";
}

// Get quote character for strings
XString
SQLInfoSQLServer::GetKEYWORDQuoteCharacter() const
{
  return "\'";
}

// Get quote character around reserved words as an identifier
XString
SQLInfoSQLServer::GetKEYWORDReservedWordQuote() const
{
  return "\"";
}

// Get default NULL for parameter list input
XString
SQLInfoSQLServer::GetKEYWORDParameterDefaultNULL() const
{
  return "= NULL";
}

// Parameter is for INPUT and OUTPUT in parameter list
XString
SQLInfoSQLServer::GetKEYWORDParameterINOUT() const
{
  return "OUTPUT";
}

// Parameter is for OUTPUT only in parameter list
XString
SQLInfoSQLServer::GetKEYWORDParameterOUT() const
{
  return "OUTPUT";
}

// Get datatype of the IDENTITY primary key in a Network database
XString
SQLInfoSQLServer::GetKEYWORDNetworkPrimaryKeyType() const
{
  return "INTEGER IDENTITY(1,1)";
}

// Get datatype for timestamp (year to second)
XString
SQLInfoSQLServer::GetKEYWORDTypeTimestamp() const
{
  return "DATETIME";
}

// Prefix for a parameter in a stored procedure
XString
SQLInfoSQLServer::GetKEYWORDParameterPrefix() const
{
  return "@";
}

// Get select part to add new record identity to a table
// Can be special column like 'OID' or a sequence select
XString
SQLInfoSQLServer::GetKEYWORDIdentityString(XString& p_tablename,XString /*p_postfix*/ /*= "_seq"*/) const
{
  return "IDENT_CURRENT('" + p_tablename + "') + " + "IDENT_INCR('" + p_tablename + "')";
}

// Gets the UPPER function
XString
SQLInfoSQLServer::GetKEYWORDUpper(XString& p_expression) const
{
  return "UPPER(" + p_expression + ")";
}

// Gets the construction for 1 minute ago
XString
SQLInfoSQLServer::GetKEYWORDInterval1MinuteAgo() const
{
  return "DATEADD(minute,-1,current_timestamp)";
}

// Gets the Not-NULL-Value statement of the database
XString
SQLInfoSQLServer::GetKEYWORDStatementNVL(XString& p_test,XString& p_isnull) const
{
  return XString("NVL(") + p_test + "," + p_isnull + ")";
}

// Gets the RDBMS definition of the datatype
XString
SQLInfoSQLServer::GetKEYWORDDataType(MetaColumn* p_column)
{
  XString type;
  switch(p_column->m_datatype)
  {
    case SQL_CHAR:                      // fall through
    case SQL_VARCHAR:                   // fall through
    case SQL_WCHAR:                     // fall through
    case SQL_WVARCHAR:                  type = "VARCHAR";  break;
    case SQL_LONGVARCHAR:               type = "VARCHAR(max)";
                                        p_column->m_columnSize = 0;
                                        break;
    case SQL_WLONGVARCHAR:              type = "NVARCHAR(max)";
                                        p_column->m_columnSize = 0;
                                        break;
    case SQL_NUMERIC:                   [[fallthrough]];
    case SQL_DECIMAL:                   if((p_column->m_columnSize    == 38   || 
                                            p_column->m_columnSize    ==  0 ) &&
                                            p_column->m_decimalDigits ==  0 )
                                        {
                                          type = "INT";
                                          p_column->m_columnSize = 0;
                                        }
                                        else
                                        {
                                          type = "NUMERIC";
                                        }
                                        break;
    case SQL_INTEGER:                   type = "INT";      break;
    case SQL_SMALLINT:                  type = "SMALLINT"; break;
    case SQL_FLOAT:                     if(p_column->m_columnSize == 38 ||
                                           p_column->m_columnSize == 18)
                                        {
                                          if(p_column->m_decimalDigits == 0)
                                          {
                                            type = "INT";
                                            p_column->m_columnSize = 0;
                                          }
                                          else
                                          {
                                          type = "NUMERIC";
                                          }
                                          p_column->m_columnSize    = 18;
                                          p_column->m_decimalDigits = 0;
                                        }
                                        else
                                        {
                                          type = "FLOAT";
                                        }
                                        break;
    case SQL_REAL:                      // fall through
    case SQL_DOUBLE:                    type = "REAL";     break;
    case SQL_BIGINT:                    type = "BIGINT";   break;
    case SQL_TINYINT:                   type = "TINYINT";  break;
    case SQL_BIT:                       type = "TINYINT";
                                        p_column->m_columnSize    = 0;
                                        p_column->m_decimalDigits = 0;
                                        break;
  //case SQL_DATE:
    case SQL_DATETIME:                  // fall through
    case SQL_TYPE_DATE:                 // fall through
    case SQL_TIMESTAMP:                 // fall through
    case SQL_TYPE_TIMESTAMP:            type = "DATETIME";
                                        p_column->m_columnSize    = 0;
                                        p_column->m_decimalDigits = 0;
                                        break;
    case SQL_TIME:                      // fall through
    case SQL_TYPE_TIME:                 type = "TIME";
                                        p_column->m_columnSize    = 0;
                                        p_column->m_decimalDigits = 0;
                                        break;
    case SQL_BINARY:                    type = "BINARY";        break;
    case SQL_VARBINARY:                 type = "VARBINARY";     break;
    case SQL_LONGVARBINARY:             type = "VARBINARY";     break;
    case SQL_GUID:                      type = "UNIQUEIDENTIFIER"; break;
    case SQL_INTERVAL_YEAR:             // fall through
    case SQL_INTERVAL_YEAR_TO_MONTH:    // fall through
    case SQL_INTERVAL_MONTH:            type = "VARCHAR";
                                        p_column->m_columnSize    = 80;
                                        p_column->m_decimalDigits = 0;
                                        break;
    case SQL_INTERVAL_DAY:              // fall through
    case SQL_INTERVAL_HOUR:             // fall through
    case SQL_INTERVAL_MINUTE:           // fall through
    case SQL_INTERVAL_SECOND:           // fall through
    case SQL_INTERVAL_DAY_TO_HOUR:      // fall through
    case SQL_INTERVAL_DAY_TO_MINUTE:    // fall through
    case SQL_INTERVAL_HOUR_TO_MINUTE:   // fall through
    case SQL_INTERVAL_HOUR_TO_SECOND:   // fall through
    case SQL_INTERVAL_MINUTE_TO_SECOND: // fall through
    case SQL_INTERVAL_DAY_TO_SECOND:    type = "VARCHAR";
                                        p_column->m_columnSize    = 80;
                                        p_column->m_decimalDigits = 0;
                                        break;
    case SQL_UNKNOWN_TYPE:              // fall through
    default:                            break;
  }
  return type;
}

// Gets the USER (current-user) keyword function
XString
SQLInfoSQLServer::GetKEYWORDCurrentUser() const
{
  return "CURRENT_USER";
}

// Connects to a default schema in the database/instance
XString
SQLInfoSQLServer::GetSQLDefaultSchema(XString p_user,XString p_schema) const
{
  return "ALTER USER " + p_user + " WITH DEFAULT_SCHEMA = " + p_schema;
}

// Gets the construction for inline generating a key within an INSERT statement
XString
SQLInfoSQLServer::GetSQLNewSerial(XString /*p_table*/, XString /*p_sequence*/) const
{
  // Insert a zero in an IDENTITY column
  return "0";
}

// Gets the construction / select for generating a new serial identity
XString
SQLInfoSQLServer::GetSQLGenerateSerial(XString p_table) const
{
  // Is generated by the IDENTITY system
  return "";
}

// Gets the construction / select for the resulting effective generated serial
XString
SQLInfoSQLServer::GetSQLEffectiveSerial(XString p_identity) const
{
  return "SELECT @@IDENTITY";
}

// Gets the sub transaction commands
XString
SQLInfoSQLServer::GetSQLStartSubTransaction(XString p_savepointName) const
{
  return XString("BEGIN TRANSACTION ") + p_savepointName;
}

XString
SQLInfoSQLServer::GetSQLCommitSubTransaction(XString p_savepointName) const
{
  return XString("ROLLBACK TRANSACTION ") + p_savepointName;
}

XString
SQLInfoSQLServer::GetSQLRollbackSubTransaction(XString p_savepointName) const
{
  return XString("ROLLBACK TRANSACTION ") + p_savepointName;
}

// FROM-Part for a query to select only 1 (one) record / or empty!
XString
SQLInfoSQLServer::GetSQLFromDualClause() const
{
  return "";
}

// Get SQL to lock  a table 
XString
SQLInfoSQLServer::GetSQLLockTable(XString p_schema, XString p_tablename, bool p_exclusive,int p_waittime) const
{
  XString query = "SELECT TOP(1) 1 FROM " + p_schema + "." + p_tablename + " WITH ";
  query += p_exclusive ? "(TABLOCKX)" : "(TABLOCK)";
  if(p_waittime)
  {
    query += " WAITFOR TIMEOUT ";
    XString wait("'00:");
    if(p_waittime > 3600)
    {
      wait.Format("%2.2d:",p_waittime / 3600);
      p_waittime %= 3600;
    }
    wait.AppendFormat("%2.2d:%2.2d'",p_waittime / 60,p_waittime % 60);
    query += wait;
  }
  return query;
}

// Get query to optimize the table statistics
XString
SQLInfoSQLServer::GetSQLOptimizeTable(XString p_schema, XString p_tablename) const
{
  XString query("UPDATE STATISTICS ");
  if (!p_schema.IsEmpty())
  {
    query += p_schema;
    query += ".";
  }
  query += p_tablename;
  query += " WITH FULLSCAN";
  return query;
}

// Transform query to select top <n> rows
XString
SQLInfoSQLServer::GetSQLTopNRows(XString p_sql,int p_top,int p_skip /*= 0*/) const
{
  if(p_top > 0 && p_sql.Find("SELECT ") == 0)
  {
    if(p_skip)
    {
      p_sql.AppendFormat("\n OFFSET %d ROWS FETCH NEXT %d ROWS ONLY",p_skip,p_top);
    }
    else
    {
      XString selectFirst;
      selectFirst.Format("SELECT TOP %d ",p_top);
      p_sql.Replace("SELECT ",selectFirst);
    }
  }
  return p_sql;
}

// Query to perform a keep alive ping
XString
SQLInfoSQLServer::GetPing() const
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
SQLInfoSQLServer::GetSQLString(const XString& p_string) const
{
  XString s = p_string;
  s.Replace("'","''");
  XString kwoot = GetKEYWORDQuoteCharacter();
  return  kwoot + s + kwoot;
}

// Get date string in engine format
XString
SQLInfoSQLServer::GetSQLDateString(int p_year,int p_month,int p_day) const
{
  XString retval;
  retval.Format("{d '%04d-%02d-%02d' }",p_year,p_month,p_day);
  return retval;
}

// Get time string in database engine format
XString
SQLInfoSQLServer::GetSQLTimeString(int p_hour,int p_minute,int p_second) const
{
  XString retval;
  retval.Format("{t '%02d:%02d:%02d' }",p_hour,p_minute,p_second);
  return retval;
}

// Get date-time string in database engine format
XString
SQLInfoSQLServer::GetSQLDateTimeString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  XString retval;
  retval.Format("{ts '%04d-%02d-%02d %02d:%02d:%02d' }",p_year,p_month,p_day,p_hour,p_minute,p_second);
  return retval;
}

// Get date-time bound parameter string in database format
XString
SQLInfoSQLServer::GetSQLDateTimeBoundString() const
{
  return "{ts ?}";
}

// Stripped data for the parameter binding
XString
SQLInfoSQLServer::GetSQLDateTimeStrippedString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  XString retval;
  retval.Format("%04d-%02d-%02d %02d:%02d:%02d",p_year,p_month,p_day,p_hour,p_minute,p_second);
  return retval;
}

// Makes an catalog identifier string (possibly quoted on both sides)
XString
SQLInfoSQLServer::GetSQLDDLIdentifier(XString p_identifier) const
{
  XString ident;
  p_identifier.MakeLower();
  ident.Format("[%s]",p_identifier.GetString());
  return ident;;
}

// Changes to parameters before binding to an ODBC HSTMT handle
void
SQLInfoSQLServer::DoBindParameterFixup(SQLSMALLINT& p_sqlDatatype,SQLULEN& p_columnSize,SQLSMALLINT& p_scale,SQLLEN& p_bufferSize,SQLLEN* p_indicator) const
{
  switch(p_sqlDatatype)
  {
    case SQL_CHAR:            [[fallthrough]];
    case SQL_VARCHAR:         if(p_bufferSize <= 0 || p_bufferSize >= DBMAXCHAR)
                              {
                                p_sqlDatatype = SQL_VARCHAR;      // Always VARCHAR
                                p_columnSize  = SQL_SS_LENGTH_UNLIMITED;
                                p_bufferSize  = SQL_SS_LENGTH_UNLIMITED;
                                *p_indicator  = SQL_NTS;
                              }
                              break;
    case SQL_WCHAR:           [[fallthrough]];
    case SQL_WVARCHAR:        if(p_bufferSize <= 0 || p_bufferSize >= DBMAXCHAR)
                              {
                                p_sqlDatatype = SQL_WVARCHAR;     // Always NVARCHAR
                                p_columnSize  = SQL_SS_LENGTH_UNLIMITED;
                                p_bufferSize  = SQL_SS_LENGTH_UNLIMITED;
                                *p_indicator  = SQL_DATA_AT_EXEC; // Cannot be NTS!!
                              }
                              break;
    case SQL_BINARY:          [[fallthrough]];
    case SQL_VARBINARY:       [[fallthrough]];
    case SQL_LONGVARCHAR:     [[fallthrough]];
    case SQL_LONGVARBINARY:   if(p_bufferSize <= 0 || p_bufferSize >= DBMAXCHAR)
                              {
                                p_sqlDatatype = SQL_VARBINARY;    // Always VARBINARY
                                p_columnSize  = SQL_SS_LENGTH_UNLIMITED;
                                p_bufferSize  = SQL_SS_LENGTH_UNLIMITED;
                                *p_indicator  = SQL_DATA_AT_EXEC; // Cannot be NTS!!
                              }
                              break;
    case SQL_TIME:            [[fallthrough]];
    case SQL_TIMESTAMP:       [[fallthrough]];
    case SQL_TYPE_TIME:       [[fallthrough]];
    case SQL_TYPE_TIMESTAMP:  p_columnSize = SQL_SS_TIME_COLUMNSIZE;
                              p_scale      = SQL_SS_TIME_MAXSCALE;
                              break;
    case SQL_NUMERIC:         [[fallthrough]];
    case SQL_DECIMAL:         p_columnSize = SQL_SS_MAXPRECISION;    // Max precision
                              p_bufferSize = SQL_SS_NUMERIC_BUFFER;  // 2 times the SQL_NUMERIC_STRUCT
                              break;
    default:                  break; // DO NOTHING for this datatype (e.g. integer, date, interval types)
  }

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
XString
SQLInfoSQLServer::GetCATALOGMetaTypes(int p_type) const
{
  UNREFERENCED_PARAMETER(p_type);
  return "";
}

// Get SQL to check if a table already exists in the database
XString
SQLInfoSQLServer::GetCATALOGTableExists(XString& p_schema,XString& p_tablename) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  XString query = "SELECT count(*)\n"
                  "  FROM dbo.sysobjects tab\n"
                  "      ,dbo.sysschemas sch\n"
                  " WHERE sch.name = ?\n"
                  "   AND tab.name = ?\n"
                  "   AND tab.schema_id = sch.schema_id\n";
  return query;
}

XString
SQLInfoSQLServer::GetCATALOGTablesList(XString& p_schema,XString& p_pattern) const
{
  return GetCATALOGTableAttributes(p_schema,p_pattern);
}

XString
SQLInfoSQLServer::GetCATALOGTableAttributes(XString& p_schema,XString& p_tablename) const
{
  XString query =
    "SELECT db_name() AS table_catalog\n"
    "      ,s.name    AS table_schema\n"
    "      ,o.name    AS table_name\n"
    "      ,CASE o.type\n"
    "            WHEN 'U'  THEN 'TABLE'\n"
    "            WHEN 'TT' THEN 'TABLE'\n"
    "            WHEN 'S'  THEN 'SYSTEM TABLE'\n"
    "                      ELSE 'UNKNOWN'\n"
    "       END AS table_type\n"
    "      ,CASE e.name\n"
    "            WHEN N'MS_Description' THEN CAST (e.value AS VARCHAR(4000))\n"
    "            ELSE ''\n"
    "       END  AS remarks\n"
    "      ,null AS tablespace\n"
    "      ,0    AS temporary\n"
    "  FROM sys.objects o\n"
    "            INNER JOIN sys.schemas s  ON o.schema_id = s.schema_id\n"
    "       LEFT OUTER JOIN sys.extended_properties e ON\n"
    "                     ((e.major_id  = o.object_id OR e.major_id IS null)\n"
    "                   AND e.minor_id  = 0\n"
    "                   AND e.class     = 1)\n"
    " WHERE o.type IN ('U','TT','S')\n";
  if(!p_schema.IsEmpty())
  {
    query += "   AND s.name = ?\n";
  }
  if(!p_tablename.IsEmpty())
  {
    query += "   AND o.name ";
    query += p_tablename.Find('%') >= 0 ? "LIKE" : "=";
    query += " ?\n";
  }

  if(p_tablename.IsEmpty())
  {
    p_tablename = "%";
  }

  query += "UNION ALL\n"
           "SELECT db_name()\n"             // AS table_catalog
           "      ,'dbo'\n"                 // AS table_schema
           "      ,o.name\n"                // AS table_name
           "      ,'LOCAL TEMPORARY'\n"     // AS table_type
           "      ,null\n"                  // AS remarks
           "      ,null\n"                  // AS tablespace
           "      ,1\n"                     // AS temporary
           "  FROM tempdb.sys.objects o\n"
           " WHERE o.type      = 'U'\n"
           "   AND o.schema_id = 1\n"
           "   AND o.name      LIKE '##" + p_tablename + "'\n"
           "UNION ALL\n"
           "SELECT db_name()\n"                                       // AS table_catalog
           "      ,'dbo'\n"                                           // AS table_schema
           "      ,substring(o.name,1,charindex('___',o.name) - 1)\n" // AS table_name
           "      ,'GLOBAL TEMPORARY'\n"                              // AS table_type
           "      ,null\n"                                            // AS remarks
           "      ,null\n"                                            // AS tablespace
           "      ,1\n"                                               // AS temporary
           "  FROM tempdb.sys.objects o\n"
           " WHERE o.type      = 'U'\n"
           "   AND o.schema_id = 1\n"   // 1 = 'dbo'
           "   AND o.name LIKE '#" + p_tablename + "\\_\\_\\_%' ESCAPE '\\'\n"
           " ORDER BY 1,2,3";
  return query;
}

XString
SQLInfoSQLServer::GetCATALOGTableSynonyms(XString& p_schema,XString& p_tablename) const
{
  XString query =
    "SELECT db_name() AS table_catalog\n"
    "      ,s.name    AS table_schema\n"
    "      ,o.name    AS table_name\n"
    "      ,'SYNONYM' AS table_type\n"
    "      ,CASE e.name\n"
    "            WHEN N'MS_Description' THEN CAST (e.value AS VARCHAR(Max))\n"
    "            ELSE ''\n"
    "       END  AS remarks\n"
    "      ,null AS tablespace\n"
    "      ,0    AS temporary\n"
    "  FROM sys.objects o\n"
    "            INNER JOIN sys.schemas s ON o.schema_id = s.schema_id\n"
    "       LEFT OUTER JOIN sys.extended_properties e ON\n"
    "                     ((e.major_id  = o.object_id OR e.major_id IS null)\n"
    "                   AND e.minor_id  = 0\n"
    "                   AND e.class     = 1)\n"
    " WHERE o.type = 'SN'\n";
  if(!p_schema.IsEmpty())
  {
    query += "   AND s.name = ?\n";
  }
  if(!p_tablename.IsEmpty())
  {
    query += "   AND o.name ";
    query += p_tablename.Find('%') >= 0 ? "LIKE" : "=";
    query += " ?\n";
  }
  query += " ORDER BY 1,2,3";
  return query;
}

XString
SQLInfoSQLServer::GetCATALOGTableCatalog(XString& p_schema,XString& p_tablename) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();

  XString query = "SELECT db_name()      AS table_catalog\n"
                  "      ,usr.name       AS schema_name\n"
                  "      ,obj.name       AS table_name\n"
                  "      ,'SYSTEM TABLE' AS object_type\n"
                  "      ,''             AS remarks\n"
                  "      ,'master.' + usr.name + '.' + obj.name as fullname\n"
                  "      ,db_name() as tablespace\n"
                  "      ,0         as temporary\n"
                  "  FROM master.sys.sysobjects obj\n"
                  "      ,master.sys.sysusers   usr\n"
                  " WHERE xtype IN ('V','S')\n"
                  "   AND obj.uid = usr.uid\n";
  if(!p_schema.IsEmpty())
  {
    query += "   AND usr.name      = ?\n";
  }
  if(!p_tablename.IsEmpty())
  {
    query += "   AND obj.name ";
    query += p_tablename.Find('%') >= 0 ? "LIKE" : "=";
    query += " ?\n";
  }
  query += " ORDER BY 1,2,3";
  return query;
}

XString
SQLInfoSQLServer::GetCATALOGTableCreate(MetaTable& p_table,MetaColumn& /*p_column*/) const
{
  XString sql = "CREATE ";
  if (p_table.m_temporary)
  {
    sql += "TEMPORARY ";
  }
  sql += "TABLE ";
  XString schema = p_table.m_schema;
  XString table  = p_table.m_table;

  schema.MakeLower();
  table.MakeLower();

  if (!schema.IsEmpty())
  {
    sql.AppendFormat("[%s].",schema.GetString());
  }
  sql.AppendFormat("[%s]",table.GetString());
  return sql;
}

XString
SQLInfoSQLServer::GetCATALOGTableCreatePostfix(MetaTable& /*p_table*/,MetaColumn& /*p_column*/) const
{
  return "";
}

XString
SQLInfoSQLServer::GetCATALOGTableRename(XString p_schema,XString p_tablename,XString p_newname) const
{
  // Beware: No 'TABLE' in the statement
  XString sql("RENAME " + p_schema + "." + p_tablename + " TO " + p_newname);
  return sql;
}

XString
SQLInfoSQLServer::GetCATALOGTableDrop(XString p_schema,XString p_tablename,bool p_ifExist /*= false*/,bool /*p_restrict = false*/,bool /*p_cascade = false*/) const
{
  XString sql("DROP TABLE "); 
  if (p_ifExist)
  {
    sql += "IF EXISTS ";
  }
  p_schema.MakeLower();
  p_tablename.MakeLower();

  if(!p_schema.IsEmpty())
  {
    sql.AppendFormat("[%s].",p_schema.GetString());
  }
  sql.AppendFormat("[%s]",p_tablename.GetString());
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL TEMPORARY TABLE FUNCTIONS

XString 
SQLInfoSQLServer::GetCATALOGTemptableCreate(XString p_schema,XString p_tablename,XString p_select) const
{
  return "CREATE TABLE #" + p_schema + "." + p_tablename + "\nAS " + p_select;
}

XString 
SQLInfoSQLServer::GetCATALOGTemptableIntoTemp(XString p_schema,XString p_tablename,XString p_select) const
{
  return "INSERT INTO #" + p_schema + "." + p_tablename + "\n" + p_select;
}

XString 
SQLInfoSQLServer::GetCATALOGTemptableDrop(XString p_schema,XString p_tablename) const
{
  XString tablename = p_schema + "." + p_tablename;
  return "DELETE FROM #"    + tablename + ";\n"
         "<@>\n"
         "TRUNCATE TABLE #" + tablename + ";\n"
         "<@>\n"
         "DROP TABLE #"     + tablename + ";\n";
}

//////////////////////////////////////////////////////////////////////////
// ALL COLUMN FUNCTIONS

XString 
SQLInfoSQLServer::GetCATALOGColumnExists(XString p_schema,XString p_tablename,XString p_columnname) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  p_columnname.MakeLower();
  XString query = "SELECT count(*)\n"
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

XString 
SQLInfoSQLServer::GetCATALOGColumnList(XString& p_schema,XString& p_tablename) const
{
  XString columns;
  return GetCATALOGColumnAttributes(p_schema,p_tablename,columns);
}

XString 
SQLInfoSQLServer::GetCATALOGColumnAttributes(XString& p_schema,XString& p_tablename,XString& p_columnname) const
{
  // In case of a NON-TEMPORARY table, the standard ODBC driver is better
  if(p_tablename.Left(1).Compare("#"))
  {
    return "";
  }
  // GLOBAL AND LOCAL TEMPORARY TABLES ONLY!!!

  // All temporary tables are stored under this schema!
  p_schema = "dbo";
  // Globals must always be searched with a like 
  // because of the storage of the global number in the tablename
  if(p_tablename.Left(2).Compare("##"))
  {
    p_tablename += "%";
  }

  XString query = "SELECT db_name() as table_catalog\n"
                  "      ,table_schema\n"
                  "      ,CASE substring(table_name,1,2)\n"
                  "            WHEN '##' THEN table_name\n"
                  "                      ELSE '#' + substring(table_name,2,charindex('___',table_name) - 2)\n"
                  "       END AS table_name\n"
                  "      ,column_name\n"
                  "      ,CASE data_type\n"
                  "            WHEN 'char'      THEN 1\n"
                  "            WHEN 'numeric'   THEN 2\n"
                  "            WHEN 'decimal'   THEN 3\n"
                  "            WHEN 'int'       THEN 4\n"
                  "            WHEN 'smallint'  THEN 5\n"
                  "            WHEN 'float'     THEN 6\n"
                  "            WHEN 'real'      THEN 7\n"
                  "            WHEN 'varchar'   THEN 12\n"
                  "            WHEN 'date'      THEN 91\n"
                  "            WHEN 'datetime'  THEN 93\n"
                  "            WHEN 'datetime2' THEN 93\n"
                  "            WHEN 'time'      THEN -154\n"
                  "            WHEN 'uniqueidentifier' THEN -11\n"
                  "            WHEN 'ntext'     THEN -10\n"
                  "            WHEN 'nvarchar'  THEN -9\n"
                  "            WHEN 'nchar'     THEN -8\n"
                  "            WHEN 'bit'       THEN -7\n"
                  "            WHEN 'tinyint'   THEN -6\n"
                  "            WHEN 'bigint'    THEN -5\n"
                  "            WHEN 'varbinary' THEN -4\n"
                  "            WHEN 'binary'    THEN -2\n"
                  "            WHEN 'text'      THEN -1\n"
                  "       END AS datatype\n"
                  "      ,data_type AS type_name\n"
                  "      ,CASE data_type\n"
                  "            WHEN 'char'      THEN character_maximum_length\n"
                  "            WHEN 'numeric'   THEN 38\n"
                  "            WHEN 'decimal'   THEN 38\n"
                  "            WHEN 'int'       THEN 10\n"
                  "            WHEN 'smallint'  THEN 5\n"
                  "            WHEN 'float'     THEN 53\n"
                  "            WHEN 'real'      THEN 24\n"
                  "            WHEN 'varchar'   THEN character_maximum_length\n"
                  "            WHEN 'date'      THEN 10\n"
                  "            WHEN 'datetime'  THEN 23\n"
                  "            WHEN 'datetime2' THEN 27\n"
                  "            WHEN 'time'      THEN 16\n"
                  "            WHEN 'uniqueidentifier' THEN 36\n"
                  "            WHEN 'ntext'     THEN character_maximum_length\n"
                  "            WHEN 'nvarchar'  THEN character_maximum_length\n"
                  "            WHEN 'nchar'     THEN character_maximum_length\n"
                  "            WHEN 'bit'       THEN 1\n"
                  "            WHEN 'tinyint'   THEN 3\n"
                  "            WHEN 'bigint'    THEN 19\n"
                  "            WHEN 'varbinary' THEN character_maximum_length\n"
                  "            WHEN 'binary'    THEN character_octet_length\n"
                  "            WHEN 'text'      THEN 2147483647\n"
                  "       END AS column_size\n"
                  "      ,CASE data_type\n"
                  "            WHEN 'char'      THEN character_maximum_length\n"
                  "            WHEN 'numeric'   THEN 40\n"
                  "            WHEN 'decimal'   THEN 40\n"
                  "            WHEN 'int'       THEN 4\n"
                  "            WHEN 'smallint'  THEN 2\n"
                  "            WHEN 'float'     THEN 8\n"
                  "            WHEN 'real'      THEN 4\n"
                  "            WHEN 'varchar'   THEN character_maximum_length\n"
                  "            WHEN 'date'      THEN 6\n"
                  "            WHEN 'datetime'  THEN 16\n"
                  "            WHEN 'datetime2' THEN 16\n"
                  "            WHEN 'time'      THEN 12\n"
                  "            WHEN 'uniqueidentifier' THEN 16\n"
                  "            WHEN 'ntext'     THEN character_octet_length\n"
                  "            WHEN 'nvarchar'  THEN character_octet_length\n"
                  "            WHEN 'nchar'     THEN character_octet_length\n"
                  "            WHEN 'bit'       THEN 1\n"
                  "            WHEN 'tinyint'   THEN 1\n"
                  "            WHEN 'bigint'    THEN 8\n"
                  "            WHEN 'varbinary' THEN character_octet_length\n"
                  "            WHEN 'binary'    THEN character_octet_length\n"
                  "            WHEN 'text'      THEN 2147483647\n"
                  "       END AS buffer_length\n"
                  "      ,numeric_scale AS decimal_digits\n"
                  "      ,numeric_precision_radix\n"
                  "      ,CASE is_nullable\n"
                  "            WHEN 'NO'  THEN 0\n"
                  "            WHEN 'YES' THEN 1\n"
                  "                       ELSE 2\n"
                  "       END AS nullable\n"
                  "      ,''  AS remarks\n"
                  "      ,column_default\n"
                  "      ,CASE data_type\n"
                  "            WHEN 'char'      THEN 1\n"
                  "            WHEN 'numeric'   THEN 2\n"
                  "            WHEN 'decimal'   THEN 3\n"
                  "            WHEN 'int'       THEN 4\n"
                  "            WHEN 'smallint'  THEN 5\n"
                  "            WHEN 'float'     THEN 6\n"
                  "            WHEN 'real'      THEN 7\n"
                  "            WHEN 'varchar'   THEN 12\n"
                  "            WHEN 'date'      THEN 9\n"
                  "            WHEN 'datetime'  THEN 9\n"
                  "            WHEN 'datetime2' THEN 93\n"
                  "            WHEN 'time'      THEN -154\n"
                  "            WHEN 'uniqueidentifier' THEN -11\n"
                  "            WHEN 'ntext'     THEN -10\n"
                  "            WHEN 'nvarchar'  THEN -9\n"
                  "            WHEN 'nchar'     THEN -8\n"
                  "            WHEN 'bit'       THEN -7\n"
                  "            WHEN 'tinyint'   THEN -6\n"
                  "            WHEN 'bigint'    THEN -5\n"
                  "            WHEN 'varbinary' THEN -3\n"
                  "            WHEN 'binary'    THEN -2\n"
                  "            WHEN 'text'      THEN -1\n"
                  "       END AS sql_datatype\n"
                  "      ,CASE data_type\n"
                  "            WHEN 'date'      THEN 1\n"
                  "            WHEN 'datetime'  THEN 3\n"
                  "                             ELSE 0\n"
                  "       END AS datatype_sub\n"
                  "      ,character_octet_length\n"
                  "      ,ordinal_position\n"
                  "      ,is_nullable\n"
                  "  FROM tempdb.information_schema.columns\n"
                  " WHERE table_schema = ?\n";
  if(!p_tablename.IsEmpty())
  {
    query += "   AND table_name ";
    query += p_tablename.Find('%') >= 0 ? "LIKE" : "=";
    query += " ?\n";
  }
  if(!p_columnname.IsEmpty())
  {
    query += "   AND column_name ";
    query += p_columnname.Find('%') >= 0 ? "LIKE" : "=";
    query += " ?\n";
  }
  query += " ORDER BY 1,2,3\n";

  return query;
}

XString 
SQLInfoSQLServer::GetCATALOGColumnCreate(MetaColumn& p_column) const
{
  XString sql = "ALTER TABLE  " + p_column.m_schema + "." + p_column.m_table  + "\n"
                "  ADD COLUMN " + p_column.m_column + " " + p_column.m_typename;
  p_column.GetPrecisionAndScale(sql);
  p_column.GetNullable(sql);
  p_column.GetDefault(sql);
  // m_position not used
  // m_length   not used
  // m_remarks  not used
  return sql;
}

XString 
SQLInfoSQLServer::GetCATALOGColumnAlter(MetaColumn& p_column) const
{
  XString sql = "ALTER TABLE  " + p_column.m_schema + "." + p_column.m_table  + "\n"
                "ALTER COLUMN " + p_column.m_column + " " + p_column.m_typename;
  p_column.GetPrecisionAndScale(sql);
  p_column.GetNullable(sql);
  p_column.GetDefault(sql);
  return sql;
}

XString 
SQLInfoSQLServer::GetCATALOGColumnRename(XString p_schema,XString p_tablename,XString p_columnname,XString p_newname,XString p_datatype) const
{
  XString sqlCode;
  sqlCode  = "ALTER TABLE " + p_schema + "." + p_tablename + "\n"
             "        ADD " + p_newname + " " + p_datatype + ";\n";
  sqlCode += "UPDATE " + p_schema + "." + p_tablename + "\n"
             "   SET " + p_newname   + " = " + p_columnname + ";\n";
  sqlCode += "ALTER TABLE " + p_schema + "." + p_tablename + "\n"
             " DROP COLUMN " + p_columnname + ";";
  return sqlCode;
}

XString 
SQLInfoSQLServer::GetCATALOGColumnDrop(XString p_schema,XString p_tablename,XString p_columnname) const
{
  XString sql("ALTER TABLE " + p_schema + "." + p_tablename + "\n"
              " DROP COLUMN " + p_columnname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL INDICES FUNCTIONS

// All index functions
XString
SQLInfoSQLServer::GetCATALOGIndexExists(XString p_schema,XString p_tablename,XString p_indexname) const
{
  return "";
}

XString
SQLInfoSQLServer::GetCATALOGIndexList(XString& p_schema,XString& p_tablename) const
{
  p_schema.Empty(); // Do not use
  p_tablename.MakeLower();
  XString query = "SELECT idx.name\n"
                  "      ,col.name\n"
                  "      ,ixk.keyno\n"
                  "      ,indexproperty(obj.Id, idx.name, 'IsUnique')\n"
                  "      ,indexkey_property(obj.Id, idx.indid, ixk.keyno, 'IsDescending')\n"
                  "      ,'' as index_source"
                  "  FROM dbo.sysindexes idx\n"
                  "      ,dbo.sysindexkeys ixk\n"
                  "      ,dbo.sysobjects obj\n"
                  "      ,dbo.syscolumns col\n"
                  " WHERE obj.name = ?\n"  // <== PARAMETER
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

XString
SQLInfoSQLServer::GetCATALOGIndexAttributes(XString& p_schema,XString& p_tablename,XString& p_indexname) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();

  // In case of a NON-TEMPORARY table, the standard ODBC driver is better
  if(p_tablename.Left(1).Compare("#"))
  {
    return "";
  }
  // GLOBAL AND LOCAL TEMPORARY TABLES ONLY!!!
  // All temporary tables are stored under this schema!
  p_schema = "dbo";
  // Globals must always be searched with a like 
  // because of the storage of the global number in the tablename
  if(p_tablename.Left(2).Compare("##"))
  {
    p_tablename += "%";
  }
  // Getting the tables statistics
  if(p_indexname.Compare("0") == 0)
  {
    XString query = "SELECT db_name() AS catalog_name\n"
                    "      ,s.name    AS schema_name\n"
                    "      ,t.name    AS table_name\n"
                    "      ,1         AS non_unique\n"
                    "      ,''        AS index_name\n"
                    "      ,0         AS type\n" // table_stats
                    "      ,0         AS ordinal_position\n"
                    "      ,''        AS column_name\n"
                    "      ,''        AS asc_or_desc\n"
                    "      ,p.rows    AS cardinality\n"
                    "      ,Max(a.data_pages) AS pages\n"
                    "      ,''        AS filter\n"
                    "  FROM tempdb.sys.tables t\n"
                    "       INNER JOIN tempdb.sys.partitions p       ON t.object_id    = p.object_id\n"
                    "       INNER JOIN tempdb.sys.allocation_units a ON p.partition_id = a.container_id\n"
                    "       INNER JOIN tempdb.sys.schemas s          ON t.schema_id    = s.schema_id\n"
                    " WHERE s.name = ?\n";
    if(!p_tablename.IsEmpty())
    {
      query += "   AND t.name ";
      query += p_tablename.Find('%') >= 0 ? "LIKE" : "=";
      query += " ?\n";
    }
    query += " GROUP BY s.name,t.name,p.rows\n"
             " ORDER BY 1,2,3,4\n";
    return query;
  }
  XString query = "SELECT db_name() AS catalog_name\n"
                  "      ,s.name    AS table_schema\n"
                  "      ,o.name    AS table_name\n"
                  "      ,CASE i.is_unique\n"
                  "            WHEN 1 THEN 0\n"
                  "                   ELSE 1\n"
                  "       END       AS non_unique\n"
//                "      ,s.name    AS index_qualifier\n"
                  "      ,i.name    AS index_name\n"
                  "      ,1         AS TYPE\n"
                  "      ,x.index_column_id AS ordinal_position\n"
                  "      ,c.name    AS column_name\n"
                  "      ,CASE x.is_descending_key\n"
                  "            WHEN 0 THEN 'A'\n"
                  "            WHEN 1 THEN 'D'\n"
                  "                   ELSE ''\n"
                  "       END       AS asc_or_desc\n"
                  "      ,y.rowcnt  AS cardinality\n"
                  "      ,y.dpages  AS pages\n"
                  "      ,CAST(i.filter_definition AS varchar) AS filter\n"
                  "  FROM tempdb.sys.indexes i\n"
                  "       INNER JOIN tempdb.sys.objects       o ON  o.object_id = i.object_id\n"
                  "       INNER JOIN tempdb.sys.index_columns x ON (x.object_id = i.object_id AND x.index_id  = i.index_id)\n"
                  "       INNER JOIN tempdb.sys.columns       c ON (c.object_id = o.object_id AND x.column_id = c.column_id)\n"
                  "       INNER JOIN tempdb.sys.schemas       s ON  s.schema_id = o.schema_id\n"
                  "       INNER JOIN tempdb.sys.sysindexes    y ON (i.object_id = y.id AND i.name = y.name)\n"
                  " WHERE s.name = ?\n";

  if(!p_tablename.IsEmpty())
  {
    query += "   AND o.name ";
    query += p_tablename.Find('%') >= 0 ? "LIKE" : "=";
    query += " ?\n";
  }
  return query;
}

// Get SQL to create an index for a table
// CREATE [UNIQUE] INDEX indexname ON [<schema>.]tablename(column [ASC|DESC] [,...]);
// Beware: no schema name for indexname. Automatically in the table schema
XString
SQLInfoSQLServer::GetCATALOGIndexCreate(MIndicesMap& p_indices,bool p_duplicateNulls /*=false*/) const
{
  XString query;
  for(auto& index : p_indices)
  {
    if(index.m_position == 1)
    {
      XString schema = index.m_schemaName;
      XString idname = index.m_indexName;
      XString tablen = index.m_tableName;
      schema.MakeLower();
      idname.MakeLower();
      tablen.MakeLower();

      // New index
      query = "CREATE ";
      if(index.m_nonunique == false)
      {
        query += "UNIQUE ";
      }
      // If duplicate nulls allowed, it must be a NON-CLUSTERED index
      if(p_duplicateNulls)
      {
        query += "NONCLUSTERED ";
      }
      query.AppendFormat("INDEX [%s] ON ",idname.GetString());

      if(!schema.IsEmpty())
      {
        query.AppendFormat("[%s].",schema.GetString());
      }
      query.AppendFormat("[%s](",tablen.GetString());
    }
    else
    {
      query += ",";
    }
    XString column = index.m_columnName;
    column.MakeLower();
    query.AppendFormat("[%s]",column.GetString());
    if(index.m_ascending != "A")
    {
      query += " DESC";
    }
  }
  query += ")";

  // If duplicate nulls allowed, at least one of the columns must be non-null
  if(p_duplicateNulls)
  {
    query += "\n WHERE ";
    for(auto& index : p_indices)
    {
      if(index.m_position != 1)
      {
        query += " AND ";
      }
      XString column = index.m_columnName;
      column.MakeLower();
      query.AppendFormat("[%s] IS NOT NULL", column.GetString());
    }
  }
  return query;
}

XString
SQLInfoSQLServer::GetCATALOGIndexDrop(XString p_schema,XString /*p_tablename*/,XString p_indexname) const
{
  p_schema.MakeLower();
  p_indexname.MakeLower();

  XString sql = "DROP INDEX [" + p_schema + "].[" + p_indexname + "]";
  return sql;
}

// Get extra filter expression for an index column
XString
SQLInfoSQLServer::GetCATALOGIndexFilter(MetaIndex& /*p_index*/) const
{
  return "";
}

//////////////////////////////////////////////////////////////////////////
// ALL PRIMARY KEY FUNCTIONS

XString
SQLInfoSQLServer::GetCATALOGPrimaryExists(XString p_schema,XString p_tablename) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();

  XString query = "SELECT COUNT(*)\n"
                  "  FROM dbo.sysobjects tab\n"
                  "      ,dbo.sysobjects con\n"
                  "      ,dbo.sysusers   use\n"
                  " WHERE tab.id    = con.parent_obj\n"
                  "   AND tab.uid   = use.uid\n"
                  "   AND con.xtype = 'PK'\n"
                  "   AND con.type  = 'K'"
                  "   AND use.name  = '" + p_schema    + "'\n"
                  "   AND tab.name  = '" + p_tablename + "'";
  return query;
}

XString
SQLInfoSQLServer::GetCATALOGPrimaryAttributes(XString& p_schema,XString& p_tablename) const
{
  // In case of a NON-TEMPORARY table, the standard ODBC driver is better
  if(p_tablename.Left(1).Compare("#"))
  {
    return "";
  }
  // GLOBAL AND LOCAL TEMPORARY TABLES ONLY!!!

  // All temporary tables are stored under this schema!
  // From now we search in 'tempdb' under the 'dbo' schema
  p_schema = "dbo";
  // Globals must always be searched with a like 
  // because of the storage of the global number in the tablename
  if(p_tablename.Left(2).Compare("##"))
  {
    p_tablename += "%";
  }
  XString query = "SELECT db_name()     AS table_catalog\n"
                  "      ,s.name        AS table_schema\n"
                  "      ,o.name        AS table_name\n"
                  "      ,c.name        AS column_name\n"
                  "      ,x.key_ordinal AS key_sequence\n"
                  "      ,k.name        AS pk_name\n"
                  "  FROM tempdb.sys.key_constraints k\n"
                  "       inner join tempdb.sys.indexes i       ON  i.object_id = k.parent_object_id\n"
                  "       inner join tempdb.sys.objects o       ON  i.object_id = o.object_id\n"
                  "       inner join tempdb.sys.schemas s       ON  s.schema_id = o.schema_id\n"
                  "       inner join tempdb.sys.index_columns x ON (i.object_id = x.object_id AND i.index_id  = x.index_id)\n"
                  "       inner join tempdb.sys.columns c       ON (c.object_id = o.object_id AND x.column_id = c.column_id)\n"
                  " WHERE k.type           = 'PK'\n"
                  "   AND i.is_primary_key = 1\n"
                  "   AND s.name           = ?\n";

  if(!p_tablename.IsEmpty())
  {
    query += "   AND o.name ";
    query += p_tablename.Find('%') >= 0 ? "LIKE" : "=";
    query += " ?\n";
  }
  return query;
}

XString
SQLInfoSQLServer::GetCATALOGPrimaryCreate(MPrimaryMap& p_primaries) const
{
  XString query("ALTER TABLE ");

  for(auto& prim : p_primaries)
  {
    if(prim.m_columnPosition == 1)
    {
      XString schema = prim.m_schema;
      XString table  = prim.m_table;
      XString constr = prim.m_constraintName;

      schema.MakeLower();
      table .MakeLower();
      constr.MakeLower();

      if(!schema.IsEmpty())
      {
        query.AppendFormat("[%s].",schema.GetString());
      }
      query.AppendFormat("[%s]\n",table.GetString());
      query.AppendFormat("  ADD CONSTRAINT [%s]\n",constr.GetString());
      query += "      PRIMARY KEY (";

    }
    else
    {
      query += ",";
    }
    XString column = prim.m_columnName;
    column.MakeLower();
    query.AppendFormat("[%s]",column.GetString());
  }
  query += ")";
  return query;
}

XString
SQLInfoSQLServer::GetCATALOGPrimaryDrop(XString p_schema,XString p_tablename,XString p_constraintname) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  p_constraintname.MakeLower();

  XString sql("ALTER TABLE [" + p_schema + "].[" + p_tablename + "]\n"
              " DROP CONSTRAINT [" + p_constraintname + "]");
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL FOREIGN KEY FUNCTIONS

XString
SQLInfoSQLServer::GetCATALOGForeignExists(XString p_schema,XString p_tablename,XString p_constraintname) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  p_constraintname.MakeLower();

  XString sql;
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

XString
SQLInfoSQLServer::GetCATALOGForeignList(XString& p_schema,XString& p_tablename,int p_maxColumns /*=SQLINFO_MAX_COLUMNS*/) const
{
  XString constraint;
  return GetCATALOGForeignAttributes(p_schema,p_tablename,constraint,p_maxColumns);
}

XString
SQLInfoSQLServer::GetCATALOGForeignAttributes(XString& p_schema
                                             ,XString& p_tablename
                                             ,XString& p_constraint
                                             ,bool     p_referenced /*=false*/
                                             ,int    /*p_maxColumns*/ /*=SQLINFO_MAX_COLUMNS*/) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  p_constraint.MakeLower();
  XString query = "SELECT db_name()         as primary_catalog_name\n"
                  "      ,sch.name          as primary_schema_name\n"
                  "      ,pri.name          as primary_table_name\n"
                  "      ,db_name()         as foreign_catalog_name\n"
                  "      ,sch.name          as foreign_schema_name\n"
                  "      ,tab.name          as foreign_table_name\n"
                  "      ,prk.name          as primary_key_constraint\n"
                  "      ,fok.name          as foreign_key_constraint\n"
                  "      ,fkc.constraint_column_id as key_sequence\n"
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
                  "      ,sys.schemas    sch\n"
                  "      ,sys.tables     tab\n"
                  "      ,sys.columns    col\n"
                  "      ,sys.tables     pri\n"
                  "      ,sys.columns    pky\n"
                  "      ,sys.sysobjects prk\n"
                  " WHERE fok.type = 'F'\n"
                  "   AND fok.parent_object_id     = tab.object_id\n"
                  "   AND tab.schema_id            = sch.schema_id\n"
                  "   AND fkc.constraint_object_id = fok.object_id\n"
                  "   AND fkc.parent_object_id     = col.object_id\n"
                  "   AND fkc.parent_column_id     = col.column_id\n"
                  "   AND fkc.referenced_object_id = pri.object_id\n"
                  "   AND fkc.referenced_object_id = pky.object_id\n"
                  "   AND fkc.referenced_column_id = pky.column_id\n"
                  "   AND prk.parent_obj           = pri.object_id\n"
                  "   AND prk.xtype                = 'PK'\n"
                  "   AND prk.type                 = 'K '\n";
  if(!p_schema.IsEmpty())
  {
    query += "   AND sch.name = ?\n";
  }
  if(!p_tablename.IsEmpty())
  {
    if(p_referenced)
    {
      query += "   AND pri.name = ?\n";
    }
    else
    {
      query += "   AND tab.name = ?\n";
    }
  }
  if(!p_constraint.IsEmpty())
  {
    if(p_referenced)
    {
      query += "   AND prk.name = ?\n";
    }
    else
    {
      query += "   AND fok.name = ?\n";
    }
  }
  // Order up to column number
  query += " ORDER BY 1,2,3,4,5,6,7,8,9";

  return query;
}

XString
SQLInfoSQLServer::GetCATALOGForeignCreate(MForeignMap& p_foreigns) const
{
  // Get first record
  MetaForeign& foreign = p_foreigns.front();

  // Construct the correct tablename
  XString table(foreign.m_fkTableName);
  XString primary(foreign.m_pkTableName);
  XString fkschema(foreign.m_fkSchemaName);
  XString pkschema(foreign.m_pkSchemaName);
  XString constraint(foreign.m_foreignConstraint);

  table.MakeLower();
  primary.MakeLower();
  fkschema.MakeLower();
  pkschema.MakeLower();
  constraint.MakeLower();

  if(!foreign.m_fkSchemaName.IsEmpty())
  {
    table = fkschema + "].[" + table;
  }
  if(!foreign.m_pkSchemaName.IsEmpty())
  {
    primary = pkschema + "].[" + primary;
  }

  // The base foreign key command
  XString query = "ALTER TABLE [" + table + "]\n"
                  "  ADD CONSTRAINT [" + constraint + "]\n"
                  "      FOREIGN KEY (";

  // Add the foreign key columns
  bool extra = false;
  for(auto& key : p_foreigns)
  {
    if(extra) query += ",";
    XString column = key.m_fkColumnName;
    column.MakeLower();
    query.AppendFormat("[%s]",column.GetString());
    extra  = true;
  }

  // Add references primary table
  query += ")\n      REFERENCES [" + primary + "] (";

  // Add the primary key columns
  extra = false;
  for(auto& key : p_foreigns)
  {
    if(extra) query += ",";
    XString column = key.m_pkColumnName;
    column.MakeLower();
    query.AppendFormat("[%s]",column.GetString());
    extra  = true;
  }
  query += ")";

  // Add all relevant options
  switch(foreign.m_updateRule)
  {
    case SQL_CASCADE :    query += "\n      ON UPDATE NO ACTION";   break; // CASCADE is unreliable
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

XString
SQLInfoSQLServer::GetCATALOGForeignAlter(MForeignMap& /*p_original*/, MForeignMap& /*p_requested*/) const
{
  // MS-SQL Server cannot alter a foreign-key constraint.
  // You must drop and then re-create your foreign key constraint
  // So return an empty string to signal this!

  // THIS IS POSSIBLE!
  // ALTER TABLE schema.table { NOCHECK | CHECK } CONSTRAINT <constraintname>

  return "";
}

XString
SQLInfoSQLServer::GetCATALOGForeignDrop(XString p_schema,XString p_tablename,XString p_constraintname) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  p_constraintname.MakeLower();

  XString sql("ALTER TABLE [" + p_schema + "].[" + p_tablename + "]\n"
              " DROP CONSTRAINT [" + p_constraintname + "]");
  return sql;
}

//////////////////////////
// All default constraints
XString
SQLInfoSQLServer::GetCATALOGDefaultExists(XString& p_schema,XString& p_tablename,XString& p_column) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  p_column.MakeLower();

  XString sql =
    "SELECT 1\n"
    "  FROM sys.default_constraints d\n"
    "       inner join sys.objects o ON o.object_id = d.parent_object_id\n"
    "       inner join sys.schemas s ON s.schema_id = o.schema_id\n"
    "       inner join sys.columns c ON o.object_id = c.object_id AND c.column_id = d.parent_column_id\n"
    " WHERE s.name = '" + p_schema    + "'\n"
    "   AND o.name = '" + p_tablename + "'\n"
    "   AND c.name = '" + p_column    + "'\n";

  return sql;
}

XString
SQLInfoSQLServer::GetCATALOGDefaultList(XString& p_schema,XString& p_tablename) const
{
  XString all;
  return GetCATALOGDefaultAttributes(p_schema,p_tablename,all);
}

XString
SQLInfoSQLServer::GetCATALOGDefaultAttributes(XString& p_schema,XString& p_tablename,XString& p_column) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  p_column.MakeLower();

  XString sql =
    "SELECT db_name() AS constraint_catalog\n"
    "      ,s.name    AS constraint_schema\n"
    "      ,o.name    AS constraint_table\n"
    "      ,d.name    AS constraint_name\n"
    "      ,c.name    AS constraint_column\n"
    "      ,CAST(definition AS VARCHAR(Max)) AS constraint_code\n"
    "  FROM sys.default_constraints d\n"
    "       inner join sys.objects o ON o.object_id = d.parent_object_id\n"
    "       inner join sys.schemas s ON s.schema_id = o.schema_id\n"
    "       inner join sys.columns c ON o.object_id = c.object_id AND c.column_id = d.parent_column_id\n"
    " WHERE s.name = ?\n";
  if(!p_tablename.IsEmpty())
  {
    sql += "   AND o.name ";
    sql += p_tablename.Find('%') < 0 ? "=" : "LIKE";
    sql += " ?\n";
  }
  if(!p_column.IsEmpty())
  {
    sql += "   AND c.name = ";
    sql += p_column.Find('%') < 0 ? "=" : "LIKE";
    sql += " ?\n";
  }
  sql += " ORDER BY 1,2,3,4";

  return sql;
}

XString
SQLInfoSQLServer::GetCATALOGDefaultCreate(XString p_schema,XString p_tablename,XString p_constraint,XString p_column,XString p_code) const
{
  p_schema    .MakeLower();
  p_tablename .MakeLower();
  p_constraint.MakeLower();
  p_column    .MakeLower();

  return "ALTER TABLE [" + p_schema + "].[" + p_tablename + "]\n"
         "  ADD CONSTRAINT [" + p_constraint + "]\n"
         "      DEFAULT " + p_code + " FOR [" + p_column + "]";
}

XString
SQLInfoSQLServer::GetCATALOGDefaultDrop(XString p_schema,XString p_tablename,XString p_constraint) const
{
  p_schema    .MakeLower();
  p_tablename .MakeLower();
  p_constraint.MakeLower();

  return "ALTER TABLE [" + p_schema + "].[" + p_tablename + "]\n"
         " DROP CONSTRAINT [" + p_constraint + "]";
}

/////////////////////////
// All check constraints

XString
SQLInfoSQLServer::GetCATALOGCheckExists(XString p_schema,XString p_tablename,XString p_constraint) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  p_constraint.MakeLower();

  XString sql = "SELECT 1\n"
                "  FROM sys.check_constraints c\n"
                "       inner join sys.objects o ON o.object_id = c.parent_object_id\n"
                "       inner join sys.schemas s ON o.schema_id = s.schema_id\n"
                " WHERE s.name = '" + p_schema     + "'\n";
                "   AND o.name = '" + p_tablename  + "'\n"
                "   AND c.name = '" + p_constraint + "'";
  return sql;
}

XString
SQLInfoSQLServer::GetCATALOGCheckList(XString p_schema,XString  p_tablename) const
{
  XString constraint;
  return GetCATALOGCheckAttributes(p_schema,p_tablename,constraint);
}

XString
SQLInfoSQLServer::GetCATALOGCheckAttributes(XString  p_schema,XString  p_tablename,XString  p_constraint) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  p_constraint.MakeLower();

  XString sql = "SELECT db_name() AS catalog\n"
                "      ,s.name    AS constraint_schema\n"
                "      ,o.name    AS constraint_table\n"
                "      ,c.name    AS constraint_name\n"
                "      ,CAST(c.definition as VARCHAR) AS constraint_code\n"
                "  FROM sys.check_constraints c\n"
                "       inner join sys.objects o ON o.object_id = c.parent_object_id\n"
                "       inner join sys.schemas s ON o.schema_id = s.schema_id\n"
                " WHERE s.name = ?\n";
  if(!p_tablename.IsEmpty())
  {
    sql += "   AND o.name ";
    sql += p_tablename.Find('%') >= 0 ? "LIKE" : "=";
    sql += " ?\n";
  }
  if(!p_constraint.IsEmpty())
  {
    sql += "   AND c.name ";
    sql += p_constraint.Find('%') >= 0 ? "LIKE" : "=";
    sql += " ?\n";
  }
  sql += " ORDER  BY 1,2,3";
  return sql;
}

XString
SQLInfoSQLServer::GetCATALOGCheckCreate(XString p_schema,XString p_tablename,XString p_constraint,XString p_condition) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  p_constraint.MakeLower();

  if((p_condition.Left(1) != "(") || (p_condition.Right(1) != ")"))
  {
    p_condition = "(" + p_condition + ")";
  }

  XString sql = "ALTER TABLE ";
  if(!p_schema.IsEmpty())
  {
    sql += p_schema + ".";
  }
  sql += p_tablename + "\n";
  sql += "  ADD CONSTRAINT " + p_constraint + "\n";
  sql += "      CHECK " + p_condition;
  return sql;
}

XString
SQLInfoSQLServer::GetCATALOGCheckDrop(XString p_schema,XString p_tablename,XString p_constraint) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  p_constraint.MakeLower();

  XString sql = "ALTER TABLE ";
  if(!p_schema.IsEmpty())
  {
    sql += p_schema + ".";
  }
  sql += p_tablename + "\n";
  sql += "  DROP CONSTRAINT " + p_constraint;

  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL TRIGGER FUNCTIONS

XString
SQLInfoSQLServer::GetCATALOGTriggerExists(XString p_schema, XString p_tablename, XString p_triggername) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  p_triggername.MakeLower();

  XString sql;
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

XString
SQLInfoSQLServer::GetCATALOGTriggerList(XString& p_schema,XString& p_tablename) const
{
  XString triggername;
  return GetCATALOGTriggerAttributes(p_schema,p_tablename,triggername);
}

XString
SQLInfoSQLServer::GetCATALOGTriggerAttributes(XString& p_schema,XString& p_tablename,XString& p_triggername) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  p_triggername.MakeLower();

  XString sql("SELECT ''       AS catalog_name\n"
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
              " WHERE trg.object_id = mod.object_id\n"
              "   AND tab.object_id = trg.parent_id\n"
              "   AND tab.schema_id = sch.schema_id\n");
  if(!p_schema.IsEmpty())
  {
    sql += "   AND sch.name = ?\n";
  }
  if(!p_tablename.IsEmpty())
  {
    sql += "   AND tab.name ";
    sql += p_tablename.Find('%') >= 0 ? "LIKE" : "=";
    sql += " ?\n";
  }
  if(!p_triggername.IsEmpty())
  {
    sql += "   AND trg.name = ?";
  }
  return sql;
}

XString
SQLInfoSQLServer::GetCATALOGTriggerCreate(MetaTrigger& p_trigger) const
{
  // Simply return the SYS.SQL_MODULE.definition block
  XString sql(p_trigger.m_source);
  sql.TrimRight(';');
  return sql;
}

XString
SQLInfoSQLServer::GetCATALOGTriggerDrop(XString p_schema, XString /*p_tablename*/, XString p_triggername) const
{
  p_schema.MakeLower();
  p_triggername.MakeLower();

  XString sql("DROP TRIGGER ");
  if(!p_schema.IsEmpty())
  {
    sql.AppendFormat("[%s].",p_schema.GetString());
  }
  sql.AppendFormat("[%s]",p_triggername.GetString());
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL SEQUENCE FUNCTIONS

XString
SQLInfoSQLServer::GetCATALOGSequenceExists(XString p_schema, XString p_sequence) const
{
  p_schema.MakeLower();
  p_sequence.MakeLower();
  XString sql = "SELECT COUNT(*)\n"
                "  FROM sys.sequences seq\n"
                "      ,sys.schemas   sch\n"
                " WHERE sch.object_id = seq.schema_id\n"
                "   AND seq.name = '" + p_sequence + "'\n"
                "   AND sch.name = '" + p_schema   + "'";
  return sql;
}

XString
SQLInfoSQLServer::GetCATALOGSequenceList(XString& p_schema,XString& p_pattern) const
{
  p_schema.MakeLower();
  p_pattern.MakeLower();
  if(!p_pattern.IsEmpty() && (p_pattern.Find('%') < 0))
  {
    p_pattern = "%" + p_pattern + "%";
  }
  XString sql = "SELECT db_name() AS catalog_name\n"
                "      ,sch.name  AS schema_name\n"
                "      ,seq.name  AS sequence_name\n"
                "      ,CAST(current_value AS BIGINT) AS current_value\n"
                "      ,CAST(minimum_value AS BIGINT) AS minimum_value\n"
                "      ,CAST(increment     AS BIGINT) AS increment_value\n"
                "      ,cache_size AS CACHE\n"
                "      ,is_cycling\n"
                "      ,0 ordering\n"
                "  FROM sys.sequences seq\n"
                "      ,sys.schemas   sch\n"
                " WHERE sch.schema_id = seq.schema_id\n";
  if(!p_schema.IsEmpty())
  {
    sql += "   AND sch.name = ?\n";
  }
  if(!p_pattern.IsEmpty())
  {
    sql += "   AND seq.name LIKE ?\n";
  }
  sql += " ORDER BY 1,2,3";
  return sql;
}

XString
SQLInfoSQLServer::GetCATALOGSequenceAttributes(XString& p_schema,XString& p_sequence) const
{
  p_schema.MakeLower();
  p_sequence.MakeLower();
  XString sql = "SELECT ''       AS catalog_name\n"
                "      ,sch.name AS schema_name\n"
                "      ,seq.name AS sequence_name\n"
                "      ,CAST(current_value AS BIGINT) AS current_value\n"
                "      ,CAST(minimum_value AS BIGINT) AS minimum_value\n"
                "      ,CAST(increment     AS BIGINT) AS increment_value\n"
                "      ,cache_size AS CACHE\n"
                "      ,is_cycling\n"
                "      ,0 ordering\n"
                "  FROM sys.sequences seq\n"
                "      ,sys.schemas   sch\n"
                " WHERE sch.schema_id = seq.schema_id\n";
  if(!p_schema.IsEmpty())
  {
    sql += "   AND sch.name = ?\n";
  }
  if(!p_sequence.IsEmpty())
  {
    sql += "   AND seq.name ";
    sql += p_sequence.Find('%') >= 0 ? "LIKE" : "=";
    sql += " ?\n";
  }
  sql += " ORDER BY 1,2,3";
  return sql;
}

XString
SQLInfoSQLServer::GetCATALOGSequenceCreate(MetaSequence& p_sequence) const
{
  XString sql("CREATE SEQUENCE ");
  XString schemanm = p_sequence.m_schemaName;
  XString sequence = p_sequence.m_sequenceName;
  schemanm.MakeLower();
  sequence.MakeLower();

  if(!schemanm.IsEmpty())
  {
    sql.AppendFormat("[%s].",schemanm.GetString());
  }
  sql.AppendFormat("[%s]",sequence.GetString());
  sql.AppendFormat("\n START WITH %d INCREMENT BY 1", (int) p_sequence.m_currentValue);

  sql += p_sequence.m_cycle ? "\n CYCLE" : "\n NO CYCLE";
  if (p_sequence.m_cache > 0)
  {
    sql.AppendFormat("\n CACHE %d",p_sequence.m_cache);
  }
  else
  {
    sql += "\n NO CACHE";
  }
  return sql;
}

XString
SQLInfoSQLServer::GetCATALOGSequenceDrop(XString p_schema, XString p_sequence) const
{
  p_schema.MakeLower();
  p_sequence.MakeLower();

  XString sql;
  sql.Format("DROP SEQUENCE IF EXISTS [%s].[%s]",p_schema.GetString(),p_sequence.GetString());
  return  sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL VIEW FUNCTIONS

XString 
SQLInfoSQLServer::GetCATALOGViewExists(XString& /*p_schema*/,XString& /*p_viewname*/) const
{
  return "";
}

XString 
SQLInfoSQLServer::GetCATALOGViewList(XString& p_schema,XString& p_pattern) const
{
  return GetCATALOGViewAttributes(p_schema, p_pattern);
}

XString 
SQLInfoSQLServer::GetCATALOGViewAttributes(XString& p_schema,XString& p_viewname) const
{
  p_schema.MakeLower();
  p_viewname.MakeLower();

  XString query = "SELECT db_name()  AS table_catalog\n"
                  "      ,usr.name   AS schema_name\n"
                  "      ,obj.name   AS table_name\n"
                  "      ,'VIEW'     AS object_type\n"
                  "      ,''         AS remarks\n"
                  "      ,'master.' + usr.name + '.' + obj.name as fullname\n"
                  "      ,db_name() as tablespace\n"
                  "      ,0         as temporary\n"
                  "  FROM sys.sysobjects obj\n"
                  "      ,sys.sysusers   usr\n"
                  " WHERE xtype   = 'V'\n"
                  "   AND obj.uid = usr.uid\n";
  if (!p_schema.IsEmpty())
  {
    query += "   AND usr.name = ?\n";
  }
  if (!p_viewname.IsEmpty())
  {
    query += "   AND obj.name ";
    query += p_viewname.Find('%') >= 0 ? "LIKE" : "=";
    query += " ?\n";
  }
  query += " ORDER BY 1,2,3";
  return query;
}

XString
SQLInfoSQLServer::GetCATALOGViewText(XString& p_schema,XString& p_viewname) const
{
  p_schema.MakeLower();
  p_viewname.MakeLower();

  CString sql = "SELECT CAST(definition AS varchar(max)) view_text\n"
                "  FROM sys.objects     o\n"
                "       inner join sys.sql_modules m on m.object_id = o.object_id\n"
                "       inner join sys.schemas s     on s.schema_id = o.schema_id\n"
                " WHERE o.type = 'V'\n"
                "   AND s.name = '" + p_schema   + "'\n"
                "   AND o.name = '" + p_viewname + "'";
  return sql;
}

XString
SQLInfoSQLServer::GetCATALOGViewCreate(XString p_schema,XString p_viewname,XString p_contents,bool p_ifexists /*= true*/) const
{
  p_schema.MakeLower();
  p_viewname.MakeLower();

  XString sql("CREATE ");
  if(p_ifexists)
  {
    sql += "OR ALTER ";
  }
  sql += "VIEW [" + p_schema + "].[" + p_viewname + "]\n" + p_contents;
  return sql;
}

XString 
SQLInfoSQLServer::GetCATALOGViewRename(XString p_schema,XString p_viewname,XString p_newname)    const
{
  return "";
}

XString 
SQLInfoSQLServer::GetCATALOGViewDrop(XString p_schema,XString p_viewname,XString& p_precursor) const
{
  p_precursor.Empty();
  p_schema.MakeLower();
  p_viewname.MakeLower();

  return "DROP VIEW [" + p_schema + "].[" + p_viewname + "]";
}

// All Privilege functions
XString
SQLInfoSQLServer::GetCATALOGTablePrivileges(XString& p_schema,XString& p_tablename) const
{
  CString sql = "SELECT db_name() AS table_catalog\n"
                "      ,s.name    AS table_schema\n"
                "      ,o.name    AS table_name\n"
                "      ,g.name    AS grantor\n"
                "      ,u.name    AS grantee\n"
                "      ,p.permission_name AS privilege\n"
                "      ,CASE state_desc\n"
                "            WHEN 'GRANT' THEN 'NO'\n"
                "            WHEN 'GRANT_WITH_GRANT_OPTION' THEN 'YES'\n"
                "            ELSE 'NO'\n"
                "       END AS grantable\n"
                "  FROM sys.database_permissions p\n"
                "       inner join sys.objects o ON o.object_id = p.major_id\n"
                "       inner join sys.schemas s ON s.schema_id = o.schema_id\n"
                "       inner join sys.database_principals u ON u.principal_id = p.grantee_principal_id\n"
                "       inner join sys.database_principals g ON g.principal_id = p.grantor_principal_id\n"
                " WHERE p.minor_id = 0\n"; // Only table privileges
  // Add the filter
  if(!p_schema.IsEmpty())
  {
    sql += "   AND s.name = '" + p_schema + "'\n";
  }
  if(!p_tablename.IsEmpty())
  {
    sql += "   AND o.name = '" + p_tablename + "'\n";
  }
  sql += " ORDER BY 1,2,3,4,5";
  return sql;
}

XString
SQLInfoSQLServer::GetCATALOGColumnPrivileges(XString& p_schema,XString& p_tablename,XString& p_columnname) const
{
  CString sql = "SELECT db_name() AS table_catalog\n"
                "      ,s.name    AS table_schema\n"
                "      ,o.name    AS table_name\n"
                "      ,c.name    AS column_name\n"
                "      ,g.name    AS grantor\n"
                "      ,u.name    AS grantee\n"
                "      ,p.permission_name AS privilege\n"
                "      ,CASE state_desc\n"
                "            WHEN 'GRANT' THEN 'NO'\n"
                "            WHEN 'GRANT_WITH_GRANT_OPTION' THEN 'YES'\n"
                "            ELSE 'NO'\n"
                "       END AS grantable\n"
                "  FROM sys.database_permissions p\n"
                "       inner join sys.objects o ON o.object_id = p.major_id\n"
                "       inner join sys.schemas s ON s.schema_id = o.schema_id\n"
                "       inner join sys.database_principals u ON u.principal_id = p.grantee_principal_id\n"
                "       inner join sys.database_principals g ON g.principal_id = p.grantor_principal_id\n"
                "       inner join sys.columns c ON c.object_id = o.object_id AND c.column_id = p.minor_id\n"
                " WHERE p.minor_id > 0\n";
  // Add the filter
  if(!p_schema.IsEmpty())
  {
    sql += "   AND s.name = ?\n";
  }
  if(!p_tablename.IsEmpty())
  {
    sql += "   AND o.name = ?\n";
  }
  if(!p_columnname.IsEmpty())
  {
    sql += "   AND c.name = ?\n";
  }
  sql += " ORDER BY 1,2,3,4,5,6";
  return sql;
}

XString
SQLInfoSQLServer::GetCATALOGSequencePrivilege(XString& p_schema,XString& p_sequence) const
{
  p_schema.MakeLower();
  p_sequence.MakeLower();

  XString sql =
    "SELECT db_name() AS sequence_catalog\n"
    "      ,s.name    AS sequence_schema\n"
    "      ,q.name    AS sequence_name\n"
    "      ,p.permission_name AS privilege\n"
    "      ,pe.name   AS grantee\n"
    "      ,CASE p.state\n"
    "            WHEN 'G' THEN 'NO'\n"
    "            WHEN 'W' THEN 'YES'\n"
    "                     ELSE 'NO'\n"
    "       END AS grant_option\n"
    "  FROM sys.sequences q\n"
    "       inner join sys.objects o ON o.object_id = q.object_id\n"
    "       inner join sys.schemas s ON s.schema_id = o.schema_id\n"
    "       inner join sys.database_permissions p ON o.object_id = p.major_id\n"
    "       inner join sys.database_principals pg ON p.grantor_principal_id = pg.principal_id\n"
    "       inner join sys.database_principals pe ON p.grantee_principal_id = pe.principal_id\n"
    " WHERE q.type = 'SO'\n"
    "   AND s.name = ?\n";

  if(!p_sequence.IsEmpty())
  {
    sql += "   AND q.name ";
    sql += p_sequence.Find('%') < 0 ? "=" : "LIKE";
    sql += " ?";
  }
  return sql;
}

XString 
SQLInfoSQLServer::GetCATALOGGrantPrivilege(XString p_schema,XString p_objectname,XString p_privilege,XString p_grantee,bool p_grantable)
{
  XString sql;

  // Filter out unwanted/not-supported privileges
  if((p_privilege.CompareNoCase("create") == 0) ||
     (p_privilege.CompareNoCase("index")  == 0))
  {
    return sql;
  }

  p_schema.MakeLower();
  p_grantee.MakeLower();
  p_objectname.MakeLower();

  // Create grant
  sql.Format("GRANT %s ON [%s].[%s] TO [%s]",p_privilege.GetString(),p_schema.GetString(),p_objectname.GetString(),p_grantee.GetString());
  if(p_grantable)
  {
    sql += " WITH GRANT OPTION";
  }
  return sql;
}

XString 
SQLInfoSQLServer::GetCATALOGRevokePrivilege(XString p_schema,XString p_objectname,XString p_privilege,XString p_grantee)
{
  XString sql;
  sql.Format("REVOKE %s ON %s.%s FROM %s",p_privilege.GetString(),p_schema.GetString(),p_objectname.GetString(),p_grantee.GetString());
  return sql;
}

// All Synonym functions
XString
SQLInfoSQLServer::GetCATALOGSynonymList(XString& p_schema,XString& p_pattern) const
{
  p_schema.MakeLower();
  p_pattern.MakeLower();
  if(p_pattern.Find('%') < 0)
  {
    p_pattern += "%";
  }
  return GetCATALOGSynonymAttributes(p_schema,p_pattern);
}

XString
SQLInfoSQLServer::GetCATALOGSynonymAttributes(XString& p_schema,XString& p_synonym) const
{
  p_schema.MakeLower();
  p_synonym.MakeLower();

  XString sql = "SELECT db_name()  AS synonym_catalog\n"
                "      ,s.name     AS synonym_schema\n"
                "      ,y.name     AS synonym_name\n"
                "      ,y.base_object_name AS definition\n"
                "  FROM sys.synonyms y\n"
                "       INNER JOIN sys.schemas s ON s.schema_id = y.schema_id\n"
                " WHERE y.type = 'SN'\n"
                "   AND s.name = ?\n";
  if(!p_synonym.IsEmpty())
  {
    sql += "   AND y.name ";
    sql += p_synonym.Find('%') >= 0 ? "LIKE ?\n" : "= ?\n";
  }
  return sql;
}

XString
SQLInfoSQLServer::GetCATALOGSynonymCreate(XString& p_schema,XString& p_synonym,XString p_forObject,bool /*p_private = true*/) const
{
  p_schema.MakeLower();
  p_synonym.MakeLower();

  XString sql = "CREATE SYNONYM [" + p_schema + "].[" + p_synonym + "] FOR " + p_forObject;
  return sql;
}

XString
SQLInfoSQLServer::GetCATALOGSynonymDrop(XString& p_schema,XString& p_synonym,bool /*p_private = true*/) const
{
  p_schema.MakeLower();
  p_synonym.MakeLower();

  XString sql = "DROP SYNONYM [" + p_schema + "].[" + p_synonym + "]";
  return sql;
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
SQLInfoSQLServer::GetPSMProcedureExists(XString p_schema, XString p_procedure) const
{
  XString query =

    "SELECT COUNT(*)\n"
    "  FROM sys.objects o\n"
    "       INNER JOIN sys.schemas s ON o.schema_id = s.schema_id\n"
    " WHERE (type_desc LIKE '%PROCEDURE%' OR type_desc LIKE '%FUNCTION%')\n"
    "   AND o.name = '" + p_procedure + "'";

  if(!p_schema.IsEmpty())
  {
    query += "\n   AND s.name = '" + p_schema + "'";
  }
  return query;
}

XString
SQLInfoSQLServer::GetPSMProcedureList(XString& p_schema) const
{
  XString query =
    "SELECT db_name() as catalog_name\n"
    "      ,s.name    as schema_name\n"
    "      ,o.name    as procedure_name\n"
    "      ,CASE o.type\n"
    "            WHEN 'P' THEN 1\n"
    "            ELSE 2\n"
    "       END       as procedure_type\n"
    "  FROM sys.objects o\n"
    "       INNER JOIN sys.schemas s ON o.schema_id = s.schema_id\n"
    " WHERE type IN ('P','FN')\n";
  if(!p_schema.IsEmpty())
  {
    query += "   AND s.name = ?\n";
  }
  query += " ORDER BY 1,2,3";
  return query;
}

XString
SQLInfoSQLServer::GetPSMProcedureAttributes(XString& p_schema,XString& p_procedure) const
{
  XString sql =
    "SELECT db_name() as catalog_name\n"
    "      ,s.name    as schema_name\n"
    "      ,o.name    as procedure_name\n"
    "      ,(SELECT Count(*)\n"
    "          FROM sys.parameters p1\n"
    "         WHERE p1.object_id = o.object_id\n"
    "           AND p1.parameter_id > 0\n"
    "           AND p1.is_output = 0) AS input_parameters\n"
    "      ,(SELECT Count(*)\n"
    "          FROM sys.parameters p2\n"
    "         WHERE p2.object_id = o.object_id\n"
    "           AND p2.parameter_id > 0\n"
    "           AND p2.is_output = 1)  AS output_parameters\n"
    "      ,0    as result_sets\n"
    "      ,NULL AS remarks\n"
    "      ,CASE type\n"
    "            WHEN 'P'  THEN 1\n"
    "            WHEN 'FN' THEN 2\n"
    "                      ELSE 3\n"
    "       END AS procedure_type\n"
    "      ,m.definition AS source\n"
    "  FROM sys.objects o\n"
    "       INNER JOIN sys.schemas     s ON o.schema_id = s.schema_id\n"
    "       INNER JOIN sys.sql_modules m ON m.object_id = o.object_id\n"
    " WHERE o.type IN ('P','FN')\n";

  if(!p_schema.IsEmpty())
  {
    sql += "   AND s.name = ?\n";
  }
  if(!p_procedure.IsEmpty())
  {
    sql += "   AND o.name ";
    sql += p_procedure.Find('%') >= 0 ? "LIKE" : "=";
    sql += " ?\n";
  }
  sql += "ORDER BY 1,2,3";

  return sql;
}

XString
SQLInfoSQLServer::GetPSMProcedureSourcecode(XString p_schema, XString p_procedure) const
{
  XString query =
    "SELECT s.name as source_schema\n"
    "      ,o.name as source_procedure\n"
    "      ,CAST(m.definition AS VARCHAR(max)) as source_code\n"
    "  FROM sys.sql_modules m\n"
    "       INNER JOIN sys.objects o  ON m.object_id = o.object_id\n"
    "       INNER JOIN sys.schemas s  ON o.schema_id = s.schema_id\n"
    "WHERE o.name = '" + p_procedure + "'";

  if(!p_schema.IsEmpty())
  {
    query += "\n   AND s.name = '" + p_schema + "'";
  }
  return query;
}

XString
SQLInfoSQLServer::GetPSMProcedureCreate(MetaProcedure& /*p_procedure*/) const
{
  return "";
}

XString
SQLInfoSQLServer::GetPSMProcedureDrop(XString p_schema,XString p_procedure,bool p_function /*=false*/) const
{
  p_schema.MakeLower();
  p_procedure.MakeLower();
  XString object = p_function ? "FUNCTION" : "PROCEDURE";
  XString sql = "DROP " + object + " IF EXISTS [" + p_schema + "].[" + p_procedure + "]";
  return sql;
}

XString
SQLInfoSQLServer::GetPSMProcedureErrors(XString p_schema,XString p_procedure) const
{
  // SQL-Server does not support procedure errors
  return "";
}

XString
SQLInfoSQLServer::GetPSMProcedurePrivilege(XString& p_schema,XString& p_procedure) const
{
  CString sql = "SELECT db_name() AS table_catalog\n"
                "      ,s.name    AS table_schema\n"
                "      ,o.name    AS table_name\n"
                "      ,g.name    AS grantor\n"
                "      ,u.name    AS grantee\n"
                "      ,p.permission_name AS privilege\n"
                "      ,CASE state_desc\n"
                "            WHEN 'GRANT' THEN 'NO'\n"
                "            WHEN 'GRANT_WITH_GRANT_OPTION' THEN 'YES'\n"
                "            ELSE 'NO'\n"
                "       END AS grantable\n"
                "  FROM sys.database_permissions p\n"
                "       inner join sys.objects o ON o.object_id = p.major_id\n"
                "       inner join sys.schemas s ON s.schema_id = o.schema_id\n"
                "       inner join sys.database_principals u ON u.principal_id = p.grantee_principal_id\n"
                "       inner join sys.database_principals g ON g.principal_id = p.grantor_principal_id\n"
                " WHERE o.type IN ('P','FN')\n";
  // Add the filter
  if(!p_schema.IsEmpty())
  {
    sql += "   AND s.name = '" + p_schema + "'\n";
  }
  if(!p_procedure.IsEmpty())
  {
    sql += "   AND o.name = '" + p_procedure + "'\n";
  }
  sql += " ORDER BY 1,2,3,4,5";
  return sql;
}

// And it's parameters
XString
SQLInfoSQLServer::GetPSMProcedureParameters(XString& p_schema,XString& p_procedure) const
{
  XString query = 
    "SELECT specific_catalog AS procedure_cat\n"
    "      ,specific_schema  AS procedure_schem\n"
    "      ,specific_name    AS procedure_name\n"
    "      ,coalesce(parameter_name,'RETURNS')  AS column_name\n"
    "      ,CASE parameter_mode\n"
    "            WHEN 'IN'    THEN 1\n"
    "            WHEN 'INOUT' THEN 2\n"
    "            WHEN 'OUT'   THEN 3\n"
    "            ELSE 4\n"
    "       END AS column_type\n"
    "      ,CASE data_type\n"
    "            WHEN 'numeric'   THEN 2\n"
    "            WHEN 'varchar'   THEN 1\n"
    "            WHEN 'datetime'  THEN 11\n"
    "            WHEN 'datetime2' THEN 11\n"
    "            WHEN 'int'       THEN 4\n"
    "       END AS data_type\n"
    "      ,data_type AS type_name\n"
    "      ,character_maximum_length AS column_size\n"
    "      ,CASE data_type\n"
    "            WHEN 'numeric'   THEN 36\n"
    "            WHEN 'varchar'   THEN character_maximum_length\n"
    "            WHEN 'datetime'  THEN 20\n"
    "            WHEN 'datetime2' THEN 20\n"
    "            WHEN 'int'       THEN 8\n"
    "       END AS buffer_length\n"
    "      ,numeric_scale           AS decimal_digits\n"
    "      ,numeric_precision_radix AS num_prec_radix\n"
    "      ,3  AS nullable\n"
    "      ,null AS remarks\n"
    "      ,null AS column_default\n"
    "      ,null as sql_data_type\n"
    "      ,null AS sql_datetime_sub\n"
    "      ,character_octet_length AS char_octet_length\n"
    "      ,ordinal_position\n"
    "      ,'YES' AS is_nullable \n"
    "  FROM information_schema.parameters\n"
    " WHERE specific_name = '" + p_procedure + "'";

  if(!p_schema.IsEmpty())
  {
    query += "\n   AND specific_schema = '" + p_schema + "'";
  }
  return query;
}

//////////////////////////////////////////////////////////////////////////
//
// ALL PSM LANGUAGE ELEMENTS
//
//////////////////////////////////////////////////////////////////////////

XString
SQLInfoSQLServer::GetPSMDeclaration(bool    /*p_first*/
                                   ,XString p_variable
                                   ,int     p_datatype
                                   ,int     p_precision /*= 0 */
                                   ,int     p_scale     /*= 0 */
                                   ,XString p_default   /*= ""*/
                                   ,XString p_domain    /*= ""*/
                                   ,XString p_asColumn  /*= ""*/) const
{
  XString line;
  line.Format("DECLARE @%s AS ",p_variable.GetString());

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
      line += " = " + p_default;
    }
  }
  line += ";\n";
  return line;
}

XString
SQLInfoSQLServer::GetPSMAssignment(XString p_variable,XString p_statement /*=""*/) const
{
  if(p_statement.Find("EXECUTE") == -1)
  {
    XString line = "SET " + p_variable + "=";
    if(!p_statement.IsEmpty())
    {
      line += p_statement;
      line += ";";
    }
    return line;
  }
  else
  {
    p_statement.Replace("(EXECUTE","EXECUTE");
    p_statement.Replace("<@@@var@@@>",p_variable);
    p_statement.Replace(";)",";");
    return p_statement;
  }
}

XString
SQLInfoSQLServer::GetPSMIF(XString p_condition) const
{
  XString line("IF (");
  line += p_condition;
  line += ") THEN\n"
          "  BEGIN\n";
  return line;
}

XString
SQLInfoSQLServer::GetPSMIFElse() const
{
  return "  END\n"
         "ELSE\n"
         "  BEGIN\n";
}

XString
SQLInfoSQLServer::GetPSMIFEnd() const
{
  return "  END;\n";
}

XString
SQLInfoSQLServer::GetPSMWhile(XString p_condition) const
{
  return "WHILE (" + p_condition + ")\n";
}

XString
SQLInfoSQLServer::GetPSMWhileEnd() const
{
  return "";
}

XString
SQLInfoSQLServer::GetPSMLOOP() const
{
  return "WHILE true\n";
}

XString
SQLInfoSQLServer::GetPSMLOOPEnd() const
{
  return "";
}

XString
SQLInfoSQLServer::GetPSMBREAK() const
{
  return "BREAK;\n";
}

XString
SQLInfoSQLServer::GetPSMRETURN(XString p_statement /*= ""*/) const
{
  XString line("RETURN");
  if(!p_statement.IsEmpty())
  {
    line += " " + p_statement;
  }
  line += ";\n";
  return line;
}

XString
SQLInfoSQLServer::GetPSMExecute(XString p_procedure,MParameterMap& p_parameters) const
{
  XString line;
  line.Format("EXECUTE %s ",p_procedure.GetString());
  bool cont = false;

  for(auto& param : p_parameters)
  {
    // Continuing
    if(cont) line += ",";
    cont = true;

    line += "@";
    line += param.m_parameter;
    line += " ";
    switch(param.m_columnType)
    {
      case 0: break;
      case SQL_PARAM_OUTPUT:       line += "OUTPUT"; break;
      case SQL_PARAM_INPUT_OUTPUT: line += "INOUT";  break;
    }
    line += ";\n";
  }
  return line;
}

// The CURSOR
XString
SQLInfoSQLServer::GetPSMCursorDeclaration(XString p_cursorname,XString p_select) const
{
  return "CURSOR " + p_cursorname + " IS " + p_select + ";";
}

XString
SQLInfoSQLServer::GetPSMCursorFetch(XString p_cursorname,std::vector<XString>& p_columnnames,std::vector<XString>& p_variablenames) const
{
  XString query = "FETCH " + p_cursorname + " INTO ";

  std::vector<XString>::iterator cNames;
  std::vector<XString>::iterator vNames;
  bool moreThenOne = false;

  for(cNames  = p_columnnames.begin(), vNames  = p_variablenames.begin();
      cNames != p_columnnames.end() && vNames != p_variablenames.end();
      ++cNames,++vNames)
  {
    query += (moreThenOne ? "," : "") + *vNames;
  }
  query += ";";
  return query;
}

//////////////////////////////////////////////////////////////////////////
// PSM Exceptions

XString
SQLInfoSQLServer::GetPSMExceptionCatchNoData() const
{
  // SQL SERVER has BEGIN TRY .... END TRY -> BEGIN CATCH ... END CATCH
  return "";
}

XString
SQLInfoSQLServer::GetPSMExceptionCatch(XString p_sqlState) const
{
  // SQL SERVER has BEGIN TRY .... END TRY -> BEGIN CATCH ... END CATCH
  return "";
}

XString
SQLInfoSQLServer::GetPSMExceptionRaise(XString p_sqlState) const
{
  return "RAISE " + p_sqlState + ";\n";
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
SQLInfoSQLServer::GetSESSIONMyself() const
{
  XString query = "SELECT rtrim(hostprocess)\n"
                  "      ,user\n"
                  "      ,rtrim(hostname)\n"    
                  "      ,getdata()\n"  // moment
                  "      ,''\n"         // remote address
                  "      ,''\n"         // remote process name
                  "      ,''\n"         // remote process ID
                  "  FROM master.dbo.sysprocesses\n"
                  " WHERE hostprocess = host_id()\n"
                  "   AND hostname    = host_name()";
  return query;
}

XString
SQLInfoSQLServer::GetSESSIONExists(XString p_sessionID) const
{
  return "SELECT COUNT(*)\n"
         "  FROM master.dbo.sysprocesses\n"
         " WHERE rtrim(hostprocess) = " + p_sessionID;
}

XString
SQLInfoSQLServer::GetSESSIONList() const
{
  return GetSESSIONAttributes("");
}

XString
SQLInfoSQLServer::GetSESSIONAttributes(XString p_sessionID) const
{
  XString sql = "SELECT rtrim(hostprocess)\n"
                "      ,user\n"
                "      ,rtrim(hostname)\n"    
                "      ,getdata()\n"  // moment
                "      ,''\n"         // remote address
                "      ,''\n"         // remote process name
                "      ,''\n"         // remote process ID
                "  FROM master.dbo.sysprocesses\n";
  if(!p_sessionID.IsEmpty())
  {
    sql += "\n WHERE rtrim(hostprocess) = " + p_sessionID;
  }
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// Transactions

XString
SQLInfoSQLServer::GetSESSIONConstraintsDeferred() const
{
  // SQL-Server cannot defer constraints
  return "";
}

XString
SQLInfoSQLServer::GetSESSIONConstraintsImmediate() const
{
  // SQL-Server constraints are always active
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
SQLInfoSQLServer::DoSQLCall(SQLQuery* /*p_query*/,XString& /*p_schema*/,XString& /*p_procedure*/)
{
  return nullptr;
}

// Calling a stored function with named parameters, returning a value
SQLVariant*
SQLInfoSQLServer::DoSQLCallNamedParameters(SQLQuery* /*p_query*/,XString& /*p_schema*/,XString& /*p_procedure*/)
{
  return nullptr;
}

//////////////////////////////////////////////////////////////////////////
//
// PRIVATE
//
//////////////////////////////////////////////////////////////////////////

// Adjust catalog for temporary objects
XString 
SQLInfoSQLServer::GetCatalogAndSchema(XString& p_schema,XString p_table)
{
  if(p_table.Left(1).Compare("#") == 0)
  {
    // Temp tables are stored in the 'dbo' schema in the 'tempdbs' database
    p_schema = "dbo";
    return "tempdbs.sys.";
  }
  // Normal object, use the sys schema
  return "sys.";
}

// End of namespace
}
