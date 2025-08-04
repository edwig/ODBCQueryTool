////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoMySQL.cpp
//
// Copyright (c) 1998-2025 ir. W.E. Huisman
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
#include "SQLInfoMySQL.h"
#include "SQLQuery.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{

// For MySQL the identifiers are transformed to LOWER case
// as the system catalog is stored in this case setting.
#define IdentifierCorrect(ident)   if(!p_quoted || !IsIdentifierMixedCase(ident)) ident.MakeLower()

// Constructor
SQLInfoMySQL::SQLInfoMySQL(SQLDatabase* p_database)
             :SQLInfoDB(p_database)
{
}

// Destructor. Does nothing
SQLInfoMySQL::~SQLInfoMySQL()
{
}

// RDBMS Uses INDENTITY or SEQUENCE interface
void
SQLInfoMySQL::SetUseSequences(bool /*p_sequences*/)
{
  // Does nothing
}

//////////////////////////////////////////////////////////////////////////
//
// GENERALS (Strings & Booleans) 
//
//////////////////////////////////////////////////////////////////////////

// Get the database type
// DatabaseType GetDatabaseType() const;
DatabaseType
SQLInfoMySQL::GetRDBMSDatabaseType() const
{
  return RDBMS_MYSQL;
}

// The name of the database vendor
XString
SQLInfoMySQL::GetRDBMSVendorName() const
{
  // The name of the database vendor
  return _T("MySQL A.B. / Oracle");
}

// Get the physical database name
XString
SQLInfoMySQL::GetRDBMSPhysicalDatabaseName() const
{
  // See to it that "SQLDatabase:GetDatabaseName" does it's work
  return m_database->GetDatabaseName();
}

// System catalog is stored in uppercase in the database?
bool
SQLInfoMySQL::GetRDBMSIsCatalogUpper() const
{
  return false;
}

// System catalog supports full ISO schemas (same tables per schema)
bool
SQLInfoMySQL::GetRDBMSUnderstandsSchemas() const
{
  return false;
}

// Supports database/ODBCdriver comments in SQL
bool
SQLInfoMySQL::GetRDBMSSupportsComments() const
{
  return true;
}

// Database can defer constraints until the end of a transaction
bool
SQLInfoMySQL::GetRDBMSSupportsDeferredConstraints() const
{
  // NO CONSTRAINTS DEFERRED 
  // If transactions are supported we assume that constraints can be deferred
  // until the end of the transaction in the commit phase
  return false;
}

// Database has ORDER BY with an expression, e.g. ORDER BY UPPER(columnname)
bool
SQLInfoMySQL::GetRDBMSSupportsOrderByExpression() const
{
  return true;
}

// Supports the ODBC escape sequence {[?=] CALL procedure (?,?,?)}
bool
SQLInfoMySQL::GetRDBMSSupportsODBCCallEscapes() const
{
  return true;
}

// Supports the ODBC call procedure with named parameters
bool
SQLInfoMySQL::GetRDBMSSupportsODBCCallNamedParameters() const
{
  return true;
}

// If the database does not support the datatype TIME, it can be implemented as a DECIMAL
bool
SQLInfoMySQL::GetRDBMSSupportsDatatypeTime() const
{
  // Time can be implemented as TIME
  return true;
}

// If the database does not support the datatype INTERVAL, it can be implemented as a DECIMAL
bool
SQLInfoMySQL::GetRDBMSSupportsDatatypeInterval() const
{
  // Interval not supported, can be implemented as DECIMALS
  return false;
}

// Supports functions at the place of table columns in create/alter index statement
bool
SQLInfoMySQL::GetRDBMSSupportsFunctionalIndexes() const
{
  return false;
}

// Support for "as" in alias statements (FROM clause)
bool
SQLInfoMySQL::GetRDBMSSupportsAsInAlias() const
{
  return true;
}

// Foreign key DDL defines the index and cannot reuse already existing ones
bool
SQLInfoMySQL::GetRDBMSForeignKeyDefinesIndex() const
{
  // If the index exists the creation of the foreign key does NOT fail!!
  return true;
}

// Gets the maximum length of an SQL statement
unsigned long
SQLInfoMySQL::GetRDBMSMaxStatementLength() const
{
  // No Limit
  return 0;
}

// Database must commit DDL commands in a transaction
bool
SQLInfoMySQL::GetRDBMSMustCommitDDL() const
{
  return false;
}

// Correct maximum precision,scale for a NUMERIC datatype
void
SQLInfoMySQL::GetRDBMSNumericPrecisionScale(SQLULEN& /*p_precision*/, SQLSMALLINT& /*p_scale*/) const
{
  // NO-OP
}

// Maximum for a VARCHAR to be handled without AT-EXEC data. Assume NVARCHAR is half that size!
int
SQLInfoMySQL::GetRDBMSMaxVarchar() const
{
  // See: https://dev.mysql.com/doc/refman/8.0/en/char.html#:~:text=Values%20in%20VARCHAR%20columns%20are,and%20the%20character%20set%20used.
  return 65535;
}

// Identifier rules differ per RDBMS
bool
SQLInfoMySQL::IsIdentifier(XString p_identifier) const
{
  // Cannot be empty and cannot exceed this amount of characters
  if(p_identifier.GetLength() == 0 ||
     p_identifier.GetLength() > (int)GetMaxIdentifierNameLength())
  {
    return false;
  }
  // Must start with one alpha char
  if(!_istalpha(p_identifier.GetAt(0)))
  {
    return false;
  }
  for(int index = 0;index < p_identifier.GetLength();++index)
  {
    // Can be upper/lower alpha or a number
    // MySQL does NOT allow an underscore!
    TCHAR ch = p_identifier.GetAt(index);
    if(!_istalnum(ch))
    {
      return false;
    }
  }
  return true;
}

// KEYWORDS

// Keyword for the current date and time
XString
SQLInfoMySQL::GetKEYWORDCurrentTimestamp() const
{
  return _T("current_timestamp");
}

// String for the current date
XString
SQLInfoMySQL::GetKEYWORDCurrentDate() const
{
  return _T("current_date");
}

// Get the concatenation operator
XString
SQLInfoMySQL::GetKEYWORDConcatanationOperator() const
{
  // BEWARE: Officially the concatenation operator is "CONCAT(string1,string2)"
  // MYSQL supports 'one' 'two' concatenation of two strings (no operator)
  return _T("");
}

// Get quote character for strings
XString
SQLInfoMySQL::GetKEYWORDQuoteCharacter() const
{
  return _T("\'");
}

// Get quote character around reserved words as an identifier
XString
SQLInfoMySQL::GetKEYWORDReservedWordQuote() const
{
  return _T("`");
}

// Get default NULL for parameter list input
XString
SQLInfoMySQL::GetKEYWORDParameterDefaultNULL() const
{
  // Standard, no definition defines the NULL state
  return _T("");
}

// Parameter is for INPUT and OUTPUT in parameter list
XString
SQLInfoMySQL::GetKEYWORDParameterINOUT() const
{
  return _T("");
}

// Parameter is for OUTPUT only in parameter list
XString
SQLInfoMySQL::GetKEYWORDParameterOUT() const
{
  return _T("");
}

// Get datatype of the IDENTITY primary key in a Network database
XString
SQLInfoMySQL::GetKEYWORDNetworkPrimaryKeyType() const
{
  // Use SEQUENCE to fill!
  return _T("INTEGER");
}

// Get datatype for timestamp (year to second)
XString
SQLInfoMySQL::GetKEYWORDTypeTimestamp() const
{
  return _T("TIMESTAMP");
}

// Prefix for a parameter in a stored procedure
XString
SQLInfoMySQL::GetKEYWORDParameterPrefix() const
{
  return _T("");
}

// Get select part to add new record identity to a table
// Can be special column like 'OID' or a sequence select
XString
SQLInfoMySQL::GetKEYWORDIdentityString(XString& /*p_tablename*/,XString /*p_postfix /*= "_seq"*/) const
{
  return _T("0");
}

// Gets the UPPER function
XString
SQLInfoMySQL::GetKEYWORDUpper(XString& p_expression) const
{
  return _T("UPPER(") + p_expression + _T(")");
}

// Gets the construction for 1 minute ago
XString
SQLInfoMySQL::GetKEYWORDInterval1MinuteAgo() const
{
  return _T("TIMESTAMPADD(MINUTE,-1,CURRENT_TIMESTAMP)");
}

// Gets the Not-NULL-Value statement of the database
XString
SQLInfoMySQL::GetKEYWORDStatementNVL(XString& p_test,XString& p_isnull) const
{
  return _T("IFNULL(") + p_test + _T(",") + p_isnull + _T(")");
}

// Gets the RDBMS definition of the datatype
XString
SQLInfoMySQL::GetKEYWORDDataType(MetaColumn* p_column)
{
  XString type;

  switch(p_column->m_datatype)
  {
    case SQL_CHAR:                      type = _T("CHAR");          break;
    case SQL_VARCHAR:                   type = _T("VARCHAR");       break;
    case SQL_LONGVARCHAR:               type = _T("TEXT");          break;
    case SQL_WCHAR:                     type = _T("CHAR charset ucs2");         break;   // TBF
    case SQL_WVARCHAR:                  type = _T("VARCHAR charset ucs2");      break;   // TBF
    case SQL_WLONGVARCHAR:              type = _T("TEXT charset ucs2");         break;   // TBF
    case SQL_NUMERIC:                   // Fall through
    case SQL_DECIMAL:                   type = _T("DECIMAL");
                                        if(p_column->m_decimalDigits == 0)
                                        {
                                          if(p_column->m_columnSize <= 2)
                                          {
                                            type = _T("TINYINT");
                                            p_column->m_datatype  = SQL_TINYINT;
                                            p_column->m_datatype3 = SQL_TINYINT;
                                          }
                                          else if(p_column->m_columnSize <= 4)
                                          {
                                            type = _T("SMALLINT");
                                            p_column->m_datatype  = SQL_SMALLINT;
                                            p_column->m_datatype3 = SQL_SMALLINT;
                                          }
                                          else if(p_column->m_columnSize <= 9)
                                          {
                                            type = _T("INTEGER");
                                            p_column->m_datatype  = SQL_INTEGER;
                                            p_column->m_datatype3 = SQL_INTEGER;
                                          }
                                          else if(p_column->m_columnSize <= 18)
                                          {
                                            type = _T("BIGINT");
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
    case SQL_INTEGER:                   type = _T("INTEGER");
                                        p_column->m_columnSize = 10;
                                        break;
    case SQL_SMALLINT:                  type = _T("SMALLINT");
                                        p_column->m_columnSize = 5;
                                        break;
    case SQL_FLOAT:                     type = _T("FLOAT");         break;
    case SQL_REAL:                      type = _T("REAL");          break;
    case SQL_DOUBLE:                    type = _T("DOUBLE");        break;
    //case SQL_DATE:
    case SQL_DATETIME:                  type = _T("DATETIME");      break;
    case SQL_TYPE_DATE:                 type = _T("DATE");          break;
    case SQL_TIME:                      type = _T("TIME");          break;
    case SQL_TYPE_TIME:                 type = _T("TIME");          break;
    case SQL_TIMESTAMP:                 type = _T("TIMESTAMP");     break;
    case SQL_TYPE_TIMESTAMP:            type = _T("TIMESTAMP");     break;
    case SQL_BINARY:                    type = _T("BINARY");        break;
    case SQL_VARBINARY:                 type = _T("VARBINARY");     break;
    case SQL_LONGVARBINARY:             type = _T("LONGBLOB");      break;
    case SQL_BIGINT:                    type = _T("BIGINT");
                                        p_column->m_columnSize = 19;
                                        break;
    case SQL_TINYINT:                   type = _T("TINYINT");
                                        p_column->m_columnSize = 3;
                                        break;
    case SQL_BIT:                       type = _T("BIT");           break;
    case SQL_GUID:                      type = _T("VARCHAR");
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
    case SQL_INTERVAL_MINUTE_TO_SECOND: type = _T("VARCHAR");
                                        p_column->m_columnSize    = 40;
                                        p_column->m_decimalDigits = 0;
                                        break;
    case SQL_UNKNOWN_TYPE:
    default:                            type = _T("UNKNOWN ODBC DATA TYPE!");  break;
  }
  return p_column->m_typename = type;
}

// Gets the USER (current-user) keyword function
XString
SQLInfoMySQL::GetKEYWORDCurrentUser() const
{
  return _T("CURRENT_USER");
}

// Connects to a default schema in the database/instance
XString
SQLInfoMySQL::GetSQLDefaultSchema(XString /*p_user*/,XString p_schema) const
{
  return _T("USE ") + p_schema;
}

// Gets the construction for inline generating a key within an INSERT statement
XString
SQLInfoMySQL::GetSQLNewSerial(XString /*p_table*/, XString /*p_sequence*/) const
{
  // Insert a zero in an IDENTITY column
  return _T("0");
}

// Gets the construction / select for generating a new serial identity
XString
SQLInfoMySQL::GetSQLGenerateSerial(XString /*p_table*/) const
{
  // NO WAY OF KNOWNING THIS
  return _T("0");
}

XString
SQLInfoMySQL::GetSQLGenerateSequence(XString /*p_sequence*/) const
{
  // Not supported
  return _T("");
}

// Gets the construction / select for the resulting effective generated serial
XString
SQLInfoMySQL::GetSQLEffectiveSerial(XString p_identity) const
{
  // THIS IS MOST LIKELY NOT THE CORRECT VALUE.
  // NO WAY OF DETERMINING THIS
  return p_identity;
}

// Gets the sub-transaction commands
XString
SQLInfoMySQL::GetSQLStartSubTransaction(XString /*p_savepointName*/) const
{
  // Generic ODBC does not known about sub transactions!
  return XString(_T(""));
}

XString
SQLInfoMySQL::GetSQLCommitSubTransaction(XString /*p_savepointName*/) const
{
  // Generic ODBC does not known about sub transactions!
  return XString(_T(""));
}

XString
SQLInfoMySQL::GetSQLRollbackSubTransaction(XString /*p_savepointName*/) const
{
  // Generic ODBC does not known about sub transactions!
  return XString(_T(""));
}

// FROM-Part for a query to select only 1 (one) record / or empty!
XString
SQLInfoMySQL::GetSQLFromDualClause() const
{
  // MySQL does bare SELECT!
  return _T("");
}

// Get SQL to lock  a table 
XString
SQLInfoMySQL::GetSQLLockTable(XString /*p_schema*/,XString p_tablename,bool p_exclusive,int /*p_waittime*/) const
{
  // Standard ISO SQL Syntax
  XString query = _T("LOCK TABLE ") + p_tablename + _T(" IN ");
  query += p_exclusive ? _T("EXCLUSIVE") : _T("SHARE");
  query += _T(" MODE");
  return query;
}

// Get query to optimize the table statistics
XString
SQLInfoMySQL::GetSQLOptimizeTable(XString /*p_schema*/, XString p_tablename) const
{
  return _T("OPTIMIZE TABLE ") + QIQ(p_tablename) + _T(" NOWAIT");
}

// Transform query to select top <n> rows
XString
SQLInfoMySQL::GetSQLTopNRows(XString p_sql,int p_top,int p_skip /*= 0*/) const
{
  if(p_top > 0)
  {
    // MYSQL: " LIMIT <top> [ OFFSET <skip> ]
    XString limit;
    limit.Format(_T("\n LIMIT %d"),p_top);
    if(p_skip > 0)
    {
      limit.AppendFormat(_T(" OFFSET %d"),p_skip);
    }
    p_sql += limit;
  }
  return p_sql;
}

// Expand a SELECT with an 'FOR UPDATE' lock clause
XString
SQLInfoMySQL::GetSelectForUpdateTableClause(unsigned /*p_lockWaitTime*/) const
{
  return "";
}

XString
SQLInfoMySQL::GetSelectForUpdateTrailer(XString p_select,unsigned /*p_lockWaitTime*/) const
{
  return p_select + "\nFOR UPDATE";
}

// Query to perform a keep alive ping
XString
SQLInfoMySQL::GetPing() const
{
  // Getting the time does a ping
  return _T("SELECT current_timestamp");
}

// Pre- and postfix statements for a bulk import
XString
SQLInfoMySQL::GetBulkImportPrefix(XString /*p_schema*/,XString /*p_tablename*/,bool /*p_identity = true*/,bool /*p_constraints = true*/) const
{
  return _T("");
}

XString
SQLInfoMySQL::GetBulkImportPostfix(XString /*p_schema*/,XString /*p_tablename*/,bool /*p_identity = true*/,bool /*p_constraints = true*/) const
{
  return _T("");
}

//////////////////////////////////////////////////////////////////////////
//
// SQL STRINGS
//
//////////////////////////////////////////////////////////////////////////

// Makes a SQL string from a given string, with all the right quotes
XString
SQLInfoMySQL::GetSQLString(const XString& p_string) const
{
  XString s = p_string;
  s.Replace(_T("'"),_T("''"));
  XString kwoot = GetKEYWORDQuoteCharacter();
  return  kwoot + s + kwoot;
}

// Get date string in engine format
XString
SQLInfoMySQL::GetSQLDateString(int p_year,int p_month,int p_day) const
{
  XString dateString;
  dateString.Format(_T("{d '%04d-%02d-%02d'}"),p_year,p_month,p_day);
  return dateString;
}

// Get time string in database engine format
XString
SQLInfoMySQL::GetSQLTimeString(int p_hour,int p_minute,int p_second) const
{
  XString retval;
  retval.Format(_T("{t '%02d:%02d:%02d'}"),p_hour,p_minute,p_second);
  return retval;
}

// Get date-time string in database engine format
XString
SQLInfoMySQL::GetSQLDateTimeString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  XString string;
  string.Format(_T("{ts '%04d-%02d-%02d %02d:%02d:%02d'}")
                ,p_year,p_month,p_day // ODBC Ordering !!
                ,p_hour,p_minute,p_second);
  return string;
}

// Get date-time bound parameter string in database format
XString
SQLInfoMySQL::GetSQLDateTimeBoundString() const
{
  return _T("{ts ?}");
}

// Stripped data for the parameter binding
XString
SQLInfoMySQL::GetSQLDateTimeStrippedString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  XString string;
  string.Format(_T("%04d-%02d-%02d %02d:%02d:%02d")
                ,p_year,p_month,p_day // ODBC Ordering !!
                ,p_hour,p_minute,p_second);
  return string;
}

// Makes an catalog identifier string (possibly quoted on both sides)
XString
SQLInfoMySQL::GetSQLDDLIdentifier(XString p_identifier) const
{
  return p_identifier;
}

// Get the name of a temp table (local temporary or global temporary)
XString
SQLInfoMySQL::GetTempTablename(XString /*p_schema*/,XString p_tablename,bool /*p_local*/) const
{
  return p_tablename;
}

// Changes to parameters before binding to an ODBC HSTMT handle (returning the At-Exec status)
bool
SQLInfoMySQL::DoBindParameterFixup(SQLSMALLINT& /*p_dataType*/,SQLSMALLINT& /*p_sqlDatatype*/,SQLULEN& /*p_columnSize*/,SQLSMALLINT& /*p_scale*/,SQLLEN& /*p_bufferSize*/,SQLLEN* /*p_indicator*/) const
{
  return false;
}

//////////////////////////////////////////////////////////////////////////
//
// CATALOG
// o GetCATALOG<Object[s]><Function>
//   Objects
//   - Catalog
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
SQLInfoMySQL::GetCATALOGMetaTypes(int p_type) const
{
  UNREFERENCED_PARAMETER(p_type);
  return _T("");
}

XString
SQLInfoMySQL::GetCATALOGDefaultCharset() const
{
  return _T("Latin1");
}

XString
SQLInfoMySQL::GetCATALOGDefaultCharsetNCV() const
{
  return _T("UTF-8");
}

XString
SQLInfoMySQL::GetCATALOGDefaultCollation() const
{
  return _T("latin1_swedish_ci");
}

// All user defined compound data types
XString
SQLInfoMySQL::GetCATALOGTypeExists(XString& /*p_schema*/,XString& /*p_typename*/,bool /*p_quoted = false*/) const
{
  return XString();
}

XString
SQLInfoMySQL::GetCATALOGTypeList(XString& /*p_schema*/,XString& /*p_pattern*/,bool /*p_quoted = false*/) const
{
  return XString();
}

XString
SQLInfoMySQL::GetCATALOGTypeAttributes(XString& /*p_schema*/,XString& /*p_typename*/,bool /*p_quoted = false*/) const
{
  return XString();
}

XString
SQLInfoMySQL::GetCATALOGTypeSource(XString& /*p_schema*/,XString& /*p_typename*/,bool /*p_quoted = false*/) const
{
  return XString();
}

XString
SQLInfoMySQL::GetCATALOGTypeCreate(MUserTypeMap& /*p_type*/) const
{
  return XString();
}

XString
SQLInfoMySQL::GetCATALOGTypeDrop(XString /*p_schema*/,XString /*p_typename*/) const
{
  return XString();
}

// Get SQL to check if a table already exists in the database
XString
SQLInfoMySQL::GetCATALOGTableExists(XString& p_schema,XString& p_tablename,bool p_quoted /*= false*/) const
{
  XString sql;
  sql = _T("SELECT COUNT(*)\n")
        _T("  FROM information_schema.tables\n")  
        _T(" WHERE table_type = 'BASE TABLE'\n")
        _T("   AND table_schema NOT IN ('mysql','sys','performance_schema')\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND table_schema = ?\n");
  }
  IdentifierCorrect(p_tablename);
  sql += _T("   AND table_name = ?");
  return sql;
}

XString
SQLInfoMySQL::GetCATALOGTablesList(XString& p_schema,XString& p_pattern,bool p_quoted /*= false*/) const
{
  return GetCATALOGTableAttributes(p_schema,p_pattern,p_quoted);
}

XString
SQLInfoMySQL::GetCATALOGTableAttributes(XString& p_schema,XString& p_tablename,bool p_quoted /*= false*/) const
{
  XString sql;
  sql = _T("SELECT table_catalog\n")
        _T("      ,table_schema\n")
        _T("      ,table_name\n")
        _T("      ,case table_type\n")
        _T("            when 'BASE TABLE' THEN 'TABLE'\n")
        _T("            else 'UNKNOWN'\n")
        _T("       end as table_type\n")
        _T("      ,table_comment\n")
        _T("      ,table_catalog || '.' || table_schema || '.' || table_name as fullname\n")
        _T("      ,engine\n")
        _T("      ,0 as temporary\n") 
        _T("  FROM information_schema.tables\n")  
        _T(" WHERE table_type = 'BASE TABLE'\n")
        _T("   AND table_schema NOT IN ('mysql','sys','performance_schema')\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND table_schema = ?\n");
  }
  if(!p_tablename.IsEmpty())
  {
    IdentifierCorrect(p_tablename);
    sql += _T("   AND table_name ");
    sql += p_tablename.Find('%') >= 0 ? _T("LIKE") : _T("=");
    sql += _T(" ?\n");
  }
  sql += _T(" ORDER BY 1,2,3");
  return sql;
}

XString
SQLInfoMySQL::GetCATALOGTableSynonyms(XString& /*p_schema*/,XString& /*p_tablename*/,bool /*p_quoted = false*/) const
{
  // MySQL does not support SYNONYMS, but we must return an empty select statement
  // so that proceses as 'DropSchema' can continue
  XString sql = _T("SELECT *\n"
                   "  FROM information_schema.tables\n"
                   " WHERE 0 = 1");
  return sql;
}

XString
SQLInfoMySQL::GetCATALOGTableCatalog(XString& p_schema,XString& p_tablename,bool p_quoted /*= false*/) const
{
  XString sql;
  sql = _T("SELECT table_catalog\n")
        _T("      ,table_schema\n")
        _T("      ,table_name\n")
        _T("      ,'SYSTEM TABLE' as table_type\n")
        _T("      ,table_comment\n")
        _T("      ,table_catalog || '.' || table_schema || '.' || table_name as fullname\n")
        _T("      ,engine\n")
        _T("      ,0 as temporary\n") 
        _T("  FROM information_schema.tables\n")  
        _T(" WHERE ( table_type = 'SYSTEM VIEW'\n")
        _T("      OR table_schema IN ('mysql','sys','performance_schema'))\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND table_schema = ?\n");
  }
  if(!p_tablename.IsEmpty())
  {
    IdentifierCorrect(p_tablename);
    sql += _T("   AND table_name ");
    sql += p_tablename.Find('%') >= 0 ? _T("LIKE") : _T("=");
    sql += _T(" ?\n");
  }
  sql += _T(" ORDER BY 1,2,3");
  return sql;
}

XString
SQLInfoMySQL::GetCATALOGTableCreate(MetaTable& p_table,MetaColumn& /*p_column*/) const
{
  XString sql = _T("CREATE ");
  if(p_table.m_temporary)
  {
    sql += _T("TEMPORARY ");
  }
  sql += _T("TABLE ");
  sql += QIQ(p_table.m_table);
  return sql;
}

XString
SQLInfoMySQL::GetCATALOGTableCreatePostfix(MetaTable& p_table,MetaColumn& /*p_column*/) const
{
  XString sql;
  if(p_table.m_temporary)
  {
    sql += _T("ENGINE = MEMORY");
  }
  return sql;
}

XString
SQLInfoMySQL::GetCATALOGTableRename(XString /*p_schema*/,XString p_tablename,XString p_newname) const
{
  XString sql(_T("RENAME TABLE") + QIQ(p_tablename) + _T(" TO ") + QIQ(p_newname));
  return sql;
}

XString
SQLInfoMySQL::GetCATALOGTableDrop(XString /*p_schema*/,XString p_tablename,bool p_ifExist /*= false*/,bool p_restrict /*= false*/,bool p_cascade /*= false*/) const
{
  XString sql(_T("DROP TABLE "));
  if (p_ifExist)
  {
    sql += _T("IF EXISTS ");
  }
  // BEWARE: drop table cannot use the schema prefix !!
  sql += QIQ(p_tablename);
  if (p_restrict)
  {
    sql += _T(" RESTRICT");
  }
  else if (p_cascade)
  {
    sql += _T(" CASCADE");
  }
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL TEMPORARY TABLE FUNCTIONS

XString 
SQLInfoMySQL::GetCATALOGTemptableCreate(XString /*p_schema*/,XString p_tablename,XString p_select) const
{
  // BEWARE: THIS IS A GUESS. 
  return _T("CREATE TEMPORARY TABLE ") + QIQ(p_tablename) + _T("\nAS ") + p_select;
}

XString 
SQLInfoMySQL::GetCATALOGTemptableIntoTemp(XString /*p_schema*/,XString p_tablename,XString p_select) const
{
  XString sql(_T("INSERT INTO "));
  sql += QIQ(p_tablename) + _T("\n") + p_select;
  return sql;
}

XString 
SQLInfoMySQL::GetCATALOGTemptableDrop(XString /*p_schema*/,XString p_tablename) const
{
  // DROP table cannot use the schema name
  return _T("DROP TABLE ") + QIQ(p_tablename);
}

//////////////////////////////////////////////////////////////////////////
// ALL COLUMN FUNCTIONS

XString 
SQLInfoMySQL::GetCATALOGColumnExists(XString p_schema,XString p_tablename,XString p_columnname,bool p_quoted /*= false*/) const
{
  IdentifierCorrect(p_tablename);
  IdentifierCorrect(p_columnname);
  XString query = _T("SELECT COUNT(*)\n")
                  _T("  FROM information_schema.columns\n")
                  _T(" WHERE table_name  = '") + p_tablename  + _T("'\n")
                  _T("   AND column_name = '") + p_columnname + _T("'\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    query += _T("   AND table_schema = '") + p_schema + _T("'\n");
  }
  return query;
}

XString 
SQLInfoMySQL::GetCATALOGColumnList(XString& p_schema,XString& p_tablename,bool p_quoted /*= false*/) const
{
  XString column;
  return GetCATALOGColumnAttributes(p_schema,p_tablename,column,p_quoted);
}

XString 
SQLInfoMySQL::GetCATALOGColumnAttributes(XString& p_schema,XString& p_tablename,XString& p_columnname,bool p_quoted /*= false*/) const
{
  IdentifierCorrect(p_tablename);
  XString query = _T("SELECT table_catalog  AS table_cat\n")
                  _T("      ,table_schema   AS table_schem\n")
                  _T("      ,table_name\n")
                  _T("      ,column_name\n")
                  _T("      ,case data_type\n")
                  _T("            when 'char'               then   1\n")
                  _T("            when 'varchar'            then  12\n")
                  _T("            when 'bigint'   then case locate('unsigned',column_type) > 0\n")
                  _T("                                 when true then -27\n")   // UBIGINT
                  _T("                                           else -25\n")   // SBIGINT
                  _T("                                 end\n")
                  _T("            when 'binary'             then  -2\n")
                  _T("            when 'bit'                then  -7\n")
                  _T("            when 'blob'               then  -4\n")  // longvarbinary
                  _T("            when 'bool'               then  -7\n")  // bit
                  _T("            when 'date'               then   9\n")  // date
                  _T("            when 'datetime'           then  93\n")  // TYPE TIMESTAMP
                  _T("            when 'decimal'            then   2\n")  // NUMERIC
                  _T("            when 'double'             then   8\n")
                  _T("            when 'double precision'   then   8\n")
                  _T("            when 'enum'               then  12\n")  // VARCHAR
                  _T("            when 'float'              then   7\n")
                  _T("            when 'int'      then case locate('unsigned',column_type) > 0\n")
                  _T("                                 when true then -18\n")   // ULONG
                  _T("                                           else   4\n")   // SLONG
                  _T("                                 end\n")
                  _T("            when 'integer'  then case locate('unsigned',column_type) > 0\n")
                  _T("                                 when true then -18\n")   // ULONG
                  _T("                                           else   4\n")   // SLONG
                  _T("                                 end\n")
                  _T("            when 'long varbinary'     then  -4\n")  // BLOB
                  _T("            when 'long varchar'       then  -1\n")
                  _T("            when 'longblob'           then  -4\n")
                  _T("            when 'longtext'           then  -1\n")
                  _T("            when 'mediumblob'         then  -4\n")
                  _T("            when 'mediumint' then case locate('unsigned',column_type) > 0\n")
                  _T("                                  when true then -18\n")   // ULONG
                  _T("                                            else   4\n")   // SLONG
                  _T("                                  end\n")
                  _T("            when 'mediumtext'         then  -1\n")
                  _T("            when 'numeric'            then   2\n")
                  _T("            when 'real'               then   7\n")
                  _T("            when 'set'                then  12\n")
                  _T("            when 'smallint' then case locate('unsigned',column_type) > 0\n")
                  _T("                                 when true then -17\n")   // USMALLINT
                  _T("                                           else   5\n")   // SMALLINT
                  _T("                                 end\n")
                  _T("            when 'text'               then  -1\n")
                  _T("            when 'time'               then  92\n")  // TYPE TIME
                  _T("            when 'timestamp'          then  93\n")
                  _T("            when 'tinyblob'           then  -3\n")  // varbinary
                  _T("            when 'tinyint'  then case locate('unsigned',column_type) > 0\n")
                  _T("                                 when true then -16\n")   // UTINYINT
                  _T("                                           else  -6\n")   // TINYINT
                  _T("                                 end\n")
                  _T("            when 'tinytext'           then  -1\n")
                  _T("            when 'varbinary'          then  -3\n")
                  _T("       end       as data_type\n")
                  _T("      ,ucase(if(column_type like '%%(%%)%%',concat(substring(column_type,1,locate('(',column_type)-1),substring(column_type,1+locate(')',column_type))),column_type)) as type_name\n")
                  _T("      ,case when data_type = 'bit'    then (numeric_precision+7)/8\n")
                  _T("            when data_type in('tinyint','smallint','mediumint','int','bigint','decimal') then numeric_precision\n")
                  _T("            when data_type = 'float'  then if(numeric_scale IS NULL,7,numeric_precision)\n")
                  _T("            when data_type = 'double' then if(numeric_scale IS NULL,15,numeric_precision)\n")
                  _T("            when data_type = 'date'   then 10\n")
                  _T("            when data_type = 'time'   then  8\n")
                  _T("            when data_type = 'year'   then  4\n")
                  _T("            when data_type in('timestamp','datetime') then 19\n")
                  _T("            else character_maximum_length\n")
                  _T("       end  as column_size\n")
                  _T("      ,case data_type\n")
                  _T("            when 'bit'        then 1\n")
                  _T("            when 'tinyint'    then 1\n")
                  _T("            when 'smallint'   then 2\n")
                  _T("            when 'int'        then 4\n")
                  _T("            when 'integer'    then 4\n")
                  _T("            when 'mediumint'  then 3\n")
                  _T("            when 'bigint'     then 20\n")
                  _T("            when 'real'       then 4\n")
                  _T("            when 'float'      then 8\n")
                  _T("            when 'double'     then 8\n")
                  _T("            when 'date'       then 6\n")
                  _T("            when 'time'       then 6\n")
                  _T("            when 'timestamp'  then 16\n")
                  _T("            when 'datetime'   then 16\n")
                  _T("            when 'guid'       then 16\n")
                  _T("            when 'year'       then 2\n")
                  _T("            when 'decimal'    then (numeric_precision + 2)\n")
                  _T("            else  character_octet_length\n")
                  _T("       end  as buffer_length\n")
                  _T("      ,ifnull(numeric_scale,datetime_precision) AS decimal_digits\n")
                  _T("      ,if(character_octet_length is not null,null,10) as num_prec_radix\n")
                  _T("      ,if(data_type='timestamp',1,if(is_nullable='YES',1,if(extra='auto_increment',1,0))) as nullable\n")
                  _T("      ,column_comment     as remarks\n")
                  _T("      ,column_default     as column_def\n")
                  _T("      ,case data_type\n")
                  _T("            when 'date'      then 9\n")   // DATETIME
                  _T("            when 'time'      then 9\n")   // DATETIME
                  _T("            when 'datetime'  then 9\n")   // DATETIME
                  _T("            when 'timestamp' then 9\n")   // DATETIME
                  _T("       end  as sql_data_type\n")
                  _T("      ,case data_type\n")
                  _T("            when 'date'      then 1\n")   // SQL_DATE
                  _T("            when 'time'      then 2\n")   // SQL_TIME
                  _T("            when 'datetime'  then 3\n")   // SQL_CODE_TIMESTAMP
                  _T("            when 'timestamp' then 3\n")   // SQL_CODE_TIMESTAMP
                  _T("       end  as sql_datetime_sub\n")
                  _T("      ,character_octet_length\n")
                  _T("      ,ordinal_position\n")
                  _T("      ,if(data_type = 'timestamp','YES',if(is_nullable = 'YES','YES',if(extra = 'auto_increment','YES','NO'))) as is_nullable\n")
                  _T("  FROM information_schema.columns\n")
                  _T(" WHERE table_name   = '") + p_tablename + _T("'\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    query += _T("   AND table_schema = '") + p_schema + _T("'\n");
  }
  if(!p_columnname.IsEmpty())
  {
    IdentifierCorrect(p_columnname);
    query += _T("   AND column_name = '") + p_columnname + _T("'\n");
  }
  query += _T(" ORDER BY ordinal_position ASC");
  return query;
}

XString 
SQLInfoMySQL::GetCATALOGColumnCreate(MetaColumn& p_column) const
{
  XString sql = _T("ALTER TABLE ")  + QIQ(p_column.m_table)  + _T("\n")
                _T("  ADD COLUMN ") + QIQ(p_column.m_column) + _T(" ") + QIQ(p_column.m_typename);
  p_column.GetPrecisionAndScale(sql);
  p_column.GetNullable(sql);
  p_column.GetDefault(sql);
  // m_position not used
  // m_length   not used
  // m_remarks  not used
  return sql;}

XString 
SQLInfoMySQL::GetCATALOGColumnAlter(MetaColumn& p_column) const
{
  // The MODIFY keyword is a-typical
  XString sql = _T("ALTER  TABLE  ") + QIQ(p_column.m_table) + _T("\n")
                _T("MODIFY COLUMN ") + QIQ(p_column.m_column) + _T(" ") + QIQ(p_column.m_typename);
  p_column.GetPrecisionAndScale(sql);
  p_column.GetNullable(sql);
  p_column.GetDefault(sql);
  // m_position not used
  // m_length   not used
  // m_remarks  not used
  return sql;
}

XString 
SQLInfoMySQL::GetCATALOGColumnRename(XString /*p_schema*/,XString p_tablename,XString p_columnname,XString p_newname,XString /*p_datatype*/) const
{
  // General ISO syntax
  XString sql(_T("ALTER  TABLE  ") + QIQ(p_tablename) + _T("\n")
              _T("RENAME ") + QIQ(p_columnname) + _T(" TO ") + QIQ(p_newname) + _T("\n"));
  return sql;
}

XString 
SQLInfoMySQL::GetCATALOGColumnDrop(XString /*p_schema*/,XString p_tablename,XString p_columnname) const
{
  XString sql(_T("ALTER TABLE  ") + QIQ(p_tablename) + _T("\n")
              _T(" DROP COLUMN ") + QIQ(p_columnname));
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL INDICES FUNCTIONS

// All index functions
XString
SQLInfoMySQL::GetCATALOGIndexExists(XString p_schema,XString /*p_tablename*/,XString p_indexname,bool p_quoted /*= false*/) const
{
  XString sql = _T("SELECT COUNT(*)\n"
                   "  FROM information_schema.statistics\n"
                   " WHERE 1 = 1\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND table_schema = ?\n");
  }
  IdentifierCorrect(p_indexname);
  sql += _T("  AND index_name = ?");

  return sql;
}

XString
SQLInfoMySQL::GetCATALOGIndexList(XString& p_schema,XString& p_tablename,bool p_quoted /*= false*/) const
{
  XString index;
  return GetCATALOGIndexAttributes(p_schema,p_tablename,index,p_quoted);
}

XString
SQLInfoMySQL::GetCATALOGIndexAttributes(XString& p_schema,XString& p_tablename,XString& p_indexname,bool p_quoted /*= false*/) const
{
  XString sql = _T("SELECT table_schema AS catalog_name\n"
                   "      ,table_schema AS schema_name\n"
                   "      ,table_name\n"
                   "      ,non_unique\n"
                   "      ,''           AS index_qualifier\n"
                   "      ,index_name\n"
                   "      ,CASE index_type\n"
                   "            WHEN 'CLUSTERED' THEN 1\n"
                   "            WHEN 'HASHED'    THEN 2\n"
                   "            WHEN 'BTREE'     THEN 3\n"
                   "            ELSE 3\n"
                   "       END  AS index_type\n"
                   "      ,seq_in_index\n"
                   "      ,column_name\n"
                   "      ,'A'          AS ascending\n"
                   "      ,cardinality\n"
                   "      ,'' AS pages\n"
                   "      ,'' AS filter_condition\n"
                   "  FROM information_schema.statistics\n"
                   " WHERE 1 = 1\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND table_schema = ?\n");
  }
  if(!p_tablename.IsEmpty())
  {
    IdentifierCorrect(p_tablename);
    sql += _T("   AND table_name = ?\n");
  }
  if(!p_indexname.IsEmpty())
  {
    IdentifierCorrect(p_indexname);
    sql += _T("  AND index_name = ?\n");
  }
  if(m_filterPKFK)
  {
    sql += _T("   AND index_name <> 'PRIMARY'\n");
  }
  sql += _T(" ORDER BY index_name,seq_in_index");

  return sql;
}

XString
SQLInfoMySQL::GetCATALOGIndexCreate(MIndicesMap& p_indices,bool /*p_duplicateNulls /*= false*/) const
{
  // Get SQL to create an index for a table
  // CREATE [UNIQUE] INDEX [<schema>.]indexname ON [<schema>.]tablename(column [ASC|DESC] [,...]);
  XString query;
  for(auto& index : p_indices)
  {
    if(index.m_position == 1)
    {
      // New index
      query = _T("CREATE ");
      if(index.m_nonunique == false)
      {
        query += _T("UNIQUE ");
      }
      query += _T("INDEX ");
      query += QIQ(index.m_indexName);
      query += _T(" ON ");
      query += QIQ(index.m_tableName);
      query += _T("(");
    }
    else
    {
      query += _T(",");
    }
    query += QIQ(index.m_columnName);

    // Descending column
    if(index.m_ascending != _T("A"))
    {
      query += _T(" DESC");
    }
  }
  query += _T(")");
  return query;
}

XString
SQLInfoMySQL::GetCATALOGIndexDrop(XString /*p_schema*/,XString p_tablename,XString p_indexname) const
{
  XString sql = _T("ALTER TABLE ") + QIQ(p_tablename) + _T("\n")
                _T(" DROP INDEX ") + QIQ(p_indexname);
  return sql;
}

// Get extra filter expression for an index column
XString
SQLInfoMySQL::GetCATALOGIndexFilter(MetaIndex& /*p_index*/) const
{
  return _T("");
}

//////////////////////////////////////////////////////////////////////////
// ALL PRIMARY KEY FUNCTIONS

XString
SQLInfoMySQL::GetCATALOGPrimaryExists(XString p_schema,XString p_tablename,bool p_quoted /*= false*/) const
{
  XString sql = _T("SELECT COUNT(*)\n"
                   "  FROM information_schema.table_constraints AS tc\n"
                   "       JOIN information_schema.key_column_usage AS kcu\n"
                   "              ON tc.constraint_name = kcu.constraint_name\n"
                   "             AND tc.table_name      = kcu.table_name\n"
                   "             AND tc.table_schema    = kcu.table_schema\n"
                   " WHERE tc.constraint_type = 'PRIMARY KEY'\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND tc.table_schema = ?\n");
  }
  IdentifierCorrect(p_tablename);
  sql += _T("   AND tc.table_name   = ?");
  return sql;
}

XString
SQLInfoMySQL::GetCATALOGPrimaryAttributes(XString& p_schema,XString& p_tablename,bool p_quoted /*= false*/) const
{
  XString sql = _T("SELECT tc.table_schema AS catalog_name\n"
                   "      ,tc.table_schema AS schema_name\n"
                   "      ,tc.table_name\n"
                   "      ,kcu.column_name\n"
                   "      ,kcu.ordinal_position\n"
                   "      ,tc.constraint_name AS pk_name\n"
                   "  FROM information_schema.table_constraints AS tc\n"
                   "       JOIN information_schema.key_column_usage AS kcu\n"
                   "              ON tc.constraint_name = kcu.constraint_name\n"
                   "             AND tc.table_name      = kcu.table_name\n"
                   "             AND tc.table_schema    = kcu.table_schema\n"
                   " WHERE tc.constraint_type = 'PRIMARY KEY'\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND tc.table_schema = ?\n");
  }
  if(!p_tablename.IsEmpty())
  {
    IdentifierCorrect(p_tablename);
    sql += _T("   AND tc.table_name   = ?\n");
  }
  sql += _T(" ORDER BY 1,2,3,5");

  return sql;
}

// In MySQL all primary keys are named "PRIMARY".
// You can add a constraint name, but it WILL be ignored!
XString
SQLInfoMySQL::GetCATALOGPrimaryCreate(MPrimaryMap& p_primaries) const
{
  XString query(_T("ALTER TABLE "));

  for(const auto& prim : p_primaries)
  {
    if(prim.m_columnPosition == 1)
    {
      query += QIQ(prim.m_table) + _T("\n");
      query += _T("  ADD PRIMARY KEY (");

    }
    else
    {
      query += _T(",");
    }
    query += QIQ(prim.m_columnName);
  }
  query += _T(")");
  return query;
}

// In MySQL all primary keys are named "PRIMARY".
XString
SQLInfoMySQL::GetCATALOGPrimaryDrop(XString /*p_schema*/,XString p_tablename,XString /*p_constraintname*/) const
{
  XString sql(_T("ALTER TABLE ") + QIQ(p_tablename) + _T("\n")
              _T(" DROP CONSTRAINT PRIMARY"));
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL FOREIGN KEY FUNCTIONS

XString
SQLInfoMySQL::GetCATALOGForeignExists(XString p_schema,XString p_tablename,XString p_constraintname,bool p_quoted /*= false*/) const
{
  XString sql = _T("SELECT COUNT(*)\n"
                   "  FROM information_schema.referential_constraints rc\n"
                   " WHERE 1 = 1\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND rc.constraint_schema = ?\n");
  }
  IdentifierCorrect(p_tablename);
  sql += _T("   AND rc.table_name = ?\n");
  IdentifierCorrect(p_constraintname);
  sql += _T("   AND rc.constraint_name = ?\n");
  return sql;
}

XString
SQLInfoMySQL::GetCATALOGForeignList(XString& p_schema,XString& p_tablename,bool p_quoted /*= false*/) const
{
  XString constraint;
  return GetCATALOGForeignAttributes(p_schema,p_tablename,constraint,p_quoted);
}

XString
SQLInfoMySQL::GetCATALOGForeignAttributes(XString& p_schema,XString& p_tablename,XString& p_constraintname,bool p_referenced /*= false*/,bool p_quoted /*= false*/) const
{
  XString sql = _T("SELECT rc.unique_constraint_schema AS pktable_catalog\n"
                   "      ,rc.unique_constraint_schema AS pktable_schema\n"
                   "      ,rc.referenced_table_name    AS pktable_name\n"
                   "      ,kcu.referenced_column_name  AS pkcolumn_name\n"
                   "      ,rc.constraint_schema        AS fktable_catalog\n"
                   "      ,rc.constraint_schema        AS fktable_schema\n"
                   "      ,rc.table_name               AS fktable_name\n"
                   "      ,kcu.column_name             AS fkcolumn_name\n"
                   "      ,kcu.position_in_unique_constraint AS key_sequence\n"
                   "      ,rc.update_rule\n"
                   "      ,rc.delete_rule\n"
                   "      ,rc.constraint_name          AS fk_constraint_name\n"
                   "      ,rc.unique_constraint_name   AS pk_constraint_name\n"
                   "      ,NULL                        AS deferrability\n"
                   "      ,CASE rc.match_option\n"
                   "            WHEN 'NONE'    THEN 0\n"
                   "            WHEN 'PARTIAL' THEN 1\n"
                   "            WHEN 'SIMPLE'  THEN 2\n"
                   "            ELSE 0\n"
                   "       END                         as match_option\n"
                   "      ,0                           AS initially_deferred\n"
                   "      ,1                           AS enabled\n"
                   "  FROM information_schema.referential_constraints rc\n"
                   "       JOIN information_schema.key_column_usage kcu\n"
                   "                         ON rc.constraint_name   = kcu.constraint_name\n"
                   "                        AND rc.constraint_schema = kcu.constraint_schema\n"
                   "                        AND rc.table_name        = kcu.table_name\n"
                   "        JOIN information_schema.table_constraints tc\n"
                   "                         ON tc.table_name        = rc.referenced_table_name\n"
                   "                        AND tc.constraint_schema = rc.constraint_schema\n"
                   "                        AND tc.constraint_name   = 'PRIMARY'\n"
                   " WHERE 1 = 1\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    if(p_referenced)
    {
      sql += _T("   AND rc.unique_constraint_schema = ?\n");
    }
    else
    {
      sql += _T("   AND rc.constraint_schema = ?\n");
    }
  }
  if(!p_tablename.IsEmpty())
  {
    IdentifierCorrect(p_tablename);
    if(p_referenced)
    {
      sql += _T("   AND rc.referenced_table_name = ?\n");
    }
    else
    {
      sql += _T("   AND rc.table_name = ?\n");
    }
  }
  if(!p_constraintname.IsEmpty())
  {
    IdentifierCorrect(p_constraintname);
    if(p_referenced)
    {
      sql += _T("   AND tc.constraint_name = ?\n");
    }
    else
    {
      sql += _T("   AND rc.constraint_name = ?\n");
    }
  }
  sql += _T(" ORDER BY 12,9");
  return sql;
}

XString
SQLInfoMySQL::GetCATALOGForeignCreate(MForeignMap& p_foreigns) const
{
  // Get first record
  MetaForeign& foreign = p_foreigns.front();

  // Construct the correct tablename
  XString table(foreign.m_fkTableName);
  XString primary(foreign.m_pkTableName);

  // The base foreign key command
  XString query = _T("ALTER TABLE ") + QIQ(table) + _T("\n")
                  _T("  ADD CONSTRAINT ") + QIQ(foreign.m_foreignConstraint) + _T("\n")
                  _T("      FOREIGN KEY (");

  // Add the foreign key columns
  bool extra = false;
  for(const auto& key : p_foreigns)
  {
    if(extra) query += _T(",");
    query += QIQ(key.m_fkColumnName);
    extra = true;
  }

  // Add references primary table
  query += _T(")\n      REFERENCES ") + QIQ(primary) + _T("(");

  // Add the primary key columns
  extra = false;
  for(const auto& key : p_foreigns)
  {
    if(extra) query += _T(",");
    query += QIQ(key.m_pkColumnName);
    extra = true;
  }
  query += _T(")");

  // Add all relevant options
  switch(foreign.m_match)
  {
    case SQL_MATCH_PARTIAL: query += _T("\n      MATCH PARTIAL"); break;
    case SQL_MATCH_SIMPLE:  query += _T("\n      MATCH SIMPLE");  break;
    case SQL_MATCH_FULL:    query += _T("\n      MATCH FULL");    break;
    default:                // In essence: MATCH FULL, but that's already the default
                            break;
  }
  switch(foreign.m_updateRule)
  {
    case SQL_CASCADE:     query += _T("\n      ON UPDATE CASCADE");     break;
    case SQL_SET_NULL:    query += _T("\n      ON UPDATE SET NULL");    break;
    case SQL_SET_DEFAULT: query += _T("\n      ON UPDATE SET DEFAULT"); break;
    case SQL_NO_ACTION:   query += _T("\n      ON UPDATE NO ACTION");   break;
    case SQL_RESTRICT:    query += _T("\n      ON UPDATE NO RESTRICT"); break;
    default:              // In essence: ON UPDATE RESTRICT, but that's already the default
                          break;
  }
  switch(foreign.m_deleteRule)
  {
    case SQL_CASCADE:     query += _T("\n      ON DELETE CASCADE");     break;
    case SQL_SET_NULL:    query += _T("\n      ON DELETE SET NULL");    break;
    case SQL_SET_DEFAULT: query += _T("\n      ON DELETE SET DEFAULT"); break;
    case SQL_NO_ACTION:   query += _T("\n      ON DELETE NO ACTION");   break;
    case SQL_RESTRICT:    query += _T("\n      ON DELETE NO RESTRICT"); break;
    default:              // In essence: ON DELETE RESTRICT, but that's already the default
                          break;
  }
  return query;
}

XString
SQLInfoMySQL::GetCATALOGForeignAlter(MForeignMap& p_original, MForeignMap& p_requested) const
{
  // Make sure we have both
  if(p_original.empty() || p_requested.empty())
  {
    return _T("");
  }

  const MetaForeign& original  = p_original.front();
  const MetaForeign& requested = p_requested.front();

  // Construct the correct tablename
  XString table(original.m_fkTableName);

  // The base foreign key command
  XString query = _T("ALTER TABLE ") + QIQ(table) + _T("\n")
                  _T("ALTER CONSTRAINT ") + QIQ(original.m_foreignConstraint) + _T("\n");

  // Add all relevant options
  if(original.m_deferrable != requested.m_deferrable)
  {
    switch(requested.m_deferrable)
    {
      case SQL_INITIALLY_DEFERRED:  query += _T("\n      INITIALLY DEFERRED"); break;
      case SQL_INITIALLY_IMMEDIATE: query += _T("\n      DEFERRABLE");         break;
      case SQL_NOT_DEFERRABLE:      query += _T("\n      NOT DEFERRABLE");     break;
      default:                      break;
    }
  }
  if(original.m_match != requested.m_match)
  {
    switch(requested.m_match)
    {
      case SQL_MATCH_FULL:    query += _T("\n      MATCH FULL");    break;
      case SQL_MATCH_PARTIAL: query += _T("\n      MATCH PARTIAL"); break;
      case SQL_MATCH_SIMPLE:  query += _T("\n      MATCH SIMPLE");  break;
    }
  }
  if(original.m_updateRule != requested.m_updateRule)
  {
    switch(requested.m_updateRule)
    {
      case SQL_CASCADE:     query += _T("\n      ON UPDATE CASCADE");     break;
      case SQL_SET_NULL:    query += _T("\n      ON UPDATE SET NULL");    break;
      case SQL_SET_DEFAULT: query += _T("\n      ON UPDATE SET DEFAULT"); break;
      case SQL_NO_ACTION:   query += _T("\n      ON UPDATE NO ACTION");   break;
      default:              // In essence: ON UPDATE RESTRICT, but that's already the default
      case SQL_RESTRICT:    break;
    }
  }
  if(original.m_deleteRule != requested.m_deleteRule)
  {
    switch(requested.m_deleteRule)
    {
      case SQL_CASCADE:     query += _T("\n      ON DELETE CASCADE");     break;
      case SQL_SET_NULL:    query += _T("\n      ON DELETE SET NULL");    break;
      case SQL_SET_DEFAULT: query += _T("\n      ON DELETE SET DEFAULT"); break;
      case SQL_NO_ACTION:   query += _T("\n      ON DELETE NO ACTION");   break;
      default:              // In essence: ON DELETE RESTRICT, but that's already the default
      case SQL_RESTRICT:    break;
    }
  }
  return query;
}

XString
SQLInfoMySQL::GetCATALOGForeignDrop(XString /*p_schema*/,XString p_tablename,XString p_constraintname) const
{
  XString sql(_T("ALTER TABLE ") + QIQ(p_tablename) + _T("\n")
              _T(" DROP CONSTRAINT ") + QIQ(p_constraintname));
  return sql;
}

//////////////////////////
// All default constraints
XString
SQLInfoMySQL::GetCATALOGDefaultExists(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_column*/) const
{
  return _T("");
}

XString
SQLInfoMySQL::GetCATALOGDefaultList(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  return _T("");
}

XString
SQLInfoMySQL::GetCATALOGDefaultAttributes(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_column*/) const
{
  return _T("");
}

XString
SQLInfoMySQL::GetCATALOGDefaultCreate(XString /*p_schema*/,XString /*p_tablename*/,XString /*p_constraint*/,XString /*p_column*/,XString /*p_code*/) const
{
  return _T("");
}

XString
SQLInfoMySQL::GetCATALOGDefaultDrop(XString /*p_schema*/,XString /*p_tablename*/,XString /*p_constraint*/) const
{
  return _T("");
}

/////////////////////////
// All check constraints

XString
SQLInfoMySQL::GetCATALOGCheckExists(XString  p_schema,XString p_tablename,XString p_constraint,bool p_quoted /*= false*/) const
{
  XString sql = _T("SELECT COUNT(*)\n"
                   "  FROM information_schema.check_constraints\n"
                   " WHERE 1 = 1\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("  AND constraint_schema = ?\n");
  }
  IdentifierCorrect(p_tablename);
  sql += _T("   AND table_name = ?\n");
  IdentifierCorrect(p_constraint);
  sql += _T("   AND constraint_name = ?");
  return sql;
}

XString
SQLInfoMySQL::GetCATALOGCheckList(XString p_schema,XString p_tablename,bool p_quoted /*= false*/) const
{
  XString constraint;
  return GetCATALOGCheckAttributes(p_schema,p_tablename,constraint,p_quoted);
}

XString
SQLInfoMySQL::GetCATALOGCheckAttributes(XString p_schema,XString p_tablename,XString p_constraint,bool p_quoted /*= false*/) const
{
  XString sql = _T("SELECT constraint_schema AS catalog_name\n"
                   "      ,constraint_schema\n"
                   "      ,table_name\n"
                   "      ,constraint_name\n"
                   "      ,check_clause\n"
                   "  FROM information_schema.check_constraints\n"
                   " WHERE 1 = 1\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("  AND constraint_schema = ?\n");
  }
  if(!p_tablename.IsEmpty())
  {
    IdentifierCorrect(p_tablename);
    sql += _T("   AND table_name = ?\n");
  }
  if(!p_constraint.IsEmpty())
  {
    IdentifierCorrect(p_constraint);
    sql += _T("   AND constraint_name = ?\n");
  }
  sql += _T(" ORDER BY 1,2,3,4");
  return sql;
}

XString
SQLInfoMySQL::GetCATALOGCheckCreate(XString /*p_schema*/,XString p_tablename,XString p_constraint,XString p_condition) const
{
  XString sql(_T("ALTER TABLE "));
  sql += QIQ(p_tablename) + _T("\n");
  sql += _T("  ADD CONSTRAINT ") + QIQ(p_constraint);
  sql += _T(" CHECK (") + p_condition + _T(")");

  return sql;
}

XString
SQLInfoMySQL::GetCATALOGCheckDrop(XString /*p_schema*/,XString p_tablename,XString p_constraint) const
{
  XString sql(_T("ALTER TABLE "));
  sql += QIQ(p_tablename) + _T("\n");
  sql += _T("  DROP CONSTRAINT ") + QIQ(p_constraint);

  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL TRIGGER FUNCTIONS

XString
SQLInfoMySQL::GetCATALOGTriggerExists(XString p_schema,XString p_tablename,XString p_triggername,bool p_quoted /*= false*/) const
{
  XString sql(_T("SELECT COUNT(*)\n"
                 "  FROM information_schema.triggers\n"));
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T(" WHERE event_object_schema = ?\n");
  }
  if(!p_tablename.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += p_schema.IsEmpty() ? _T(" WHERE ") : _T("   AND ");
    sql += _T("event_object_table = ?\n");
  }
  IdentifierCorrect(p_triggername);
  sql += p_schema.IsEmpty() && p_tablename.IsEmpty() ? _T(" WHERE ") : _T("   AND ");
  sql += _T("trigger_name = ?\n");

  return sql;
}

XString
SQLInfoMySQL::GetCATALOGTriggerList(XString& p_schema,XString& p_tablename,bool p_quoted /*= false*/) const
{
  XString triggername;
  return GetCATALOGTriggerAttributes(p_schema,p_tablename,triggername,p_quoted);
}

XString
SQLInfoMySQL::GetCATALOGTriggerAttributes(XString& p_schema,XString& p_tablename,XString& p_triggername,bool p_quoted /*= false*/) const
{
  XString sql;
  sql = _T("SELECT event_object_catalog\n")
        _T("      ,event_object_schema\n")
        _T("      ,event_object_table\n")
        _T("      ,trigger_name\n")
        _T("      ,'' AS trigger_remarks\n")
        _T("      ,action_order  AS trigger_position\n")
        _T("      ,CASE action_timing\n")
        _T("            WHEN 'AFTER' THEN FALSE ELSE TRUE\n")
        _T("       END AS trigger_before\n")
        _T("      ,CASE event_manipulation\n")
        _T("            WHEN 'INSERT' THEN TRUE ELSE FALSE\n")
        _T("       END AS trigger_insert\n")
        _T("      ,CASE event_manipulation\n")
        _T("            WHEN 'UPDATE' THEN TRUE ELSE FALSE\n")
        _T("       END AS trigger_update\n")
        _T("      ,CASE event_manipulation\n")
        _T("            WHEN 'DELETE' THEN TRUE ELSE FALSE\n")
        _T("       END AS trigger_delete\n")
        _T("      ,FALSE AS trigger_select\n")
        _T("      ,FALSE AS trigger_session\n")
        _T("      ,FALSE AS trigger_transaction\n")
        _T("      ,FALSE AS trigger_rollback\n")
        _T("      ,''    AS trigger_referencing\n")
        _T("      ,TRUE  AS trigger_enabled\n")
        _T("      ,action_statement AS trigger_source\n")
        _T("  FROM information_schema.triggers\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T(" WHERE event_object_schema = ?\n");
  }
  if(!p_tablename.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += p_schema.IsEmpty() ? _T(" WHERE ") : _T("   AND ");
    sql += _T("event_object_table ");
    sql += p_tablename.Find(_T("%")) >= 0 ? _T("LIKE") : _T("=");
    sql += _T(" ?\n");
  }
  if(!p_triggername.IsEmpty())
  {
    IdentifierCorrect(p_triggername);
    sql += p_schema.IsEmpty() && p_tablename.IsEmpty() ? _T(" WHERE ") : _T("   AND ");
    sql += _T("trigger_name ");
    sql += p_triggername.Find(_T("%")) >= 0 ? _T("LIKE") : _T("=");
    sql += _T(" ?\n");
  }
  sql += _T(" ORDER BY 1,2,3,6");
  return sql;
}

XString
SQLInfoMySQL::GetCATALOGTriggerCreate(MetaTrigger& p_trigger) const
{
  XString sql(_T("CREATE TRIGGER IF NOT EXISTS "));
  sql += QIQ(p_trigger.m_triggerName) + _T("\n");

  // Before or after
  sql += p_trigger.m_before ? _T("BEFORE ") : _T("AFTER ");

  // Trigger actions
       if(p_trigger.m_insert) sql += _T("INSERT");
  else if(p_trigger.m_update) sql += _T("UPDATE");
  else if(p_trigger.m_delete) sql += _T("DELETE");

  // Add trigger table
  sql += _T("\nON ");
  sql += QIQ(p_trigger.m_tableName);
  sql += _T(" FOR EACH ROW\n");

  // Add trigger body
  sql += p_trigger.m_source;
  return sql;
}

XString
SQLInfoMySQL::GetCATALOGTriggerDrop(XString /*p_schema*/, XString /*p_tablename*/, XString p_triggername) const
{
  XString sql(_T("DROP TRIGGER IF EXISTS "));
  sql += QIQ(p_triggername);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL SEQUENCE FUNCTIONS

XString
SQLInfoMySQL::GetCATALOGSequenceExists(XString /*p_schema*/,XString /*p_sequence*/,bool /*p_quoted /*= false*/) const
{
  // MySQL does NOT support SEQUENCE
  return _T("");
}

XString
SQLInfoMySQL::GetCATALOGSequenceList(XString& /*p_schema*/,XString& /*p_pattern*/,bool /*p_quoted /*= false*/) const
{
  // MySQL does NOT support SEQUENCE
  return _T("");
}

XString
SQLInfoMySQL::GetCATALOGSequenceAttributes(XString& /*p_schema*/,XString& /*p_sequence*/,bool /*p_quoted /*= false*/) const
{
  // MySQL does NOT support SEQUENCE
  return _T("");
}

XString
SQLInfoMySQL::GetCATALOGSequenceCreate(MetaSequence& /*p_sequence*/) const
{
  // MySQL does NOT support SEQUENCE
  return _T("");
}

XString
SQLInfoMySQL::GetCATALOGSequenceDrop(XString /*p_schema*/, XString /*p_sequence*/) const
{
  // MySQL does NOT support SEQUENCE
  return _T("");
}

//////////////////////////////////////////////////////////////////////////
// ALL VIEW FUNCTIONS

XString 
SQLInfoMySQL::GetCATALOGViewExists(XString& p_schema,XString& p_viewname,bool p_quoted /*= false*/) const
{
  XString sql = _T("SELECT COUNT(*)\n")
                _T("  FROM information_schema.tables\n")  
                _T(" WHERE table_type = 'VIEW'\n")
                _T("   AND table_schema NOT IN ('mysql','sys','performance_schema')\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND table_schema = ?\n");
  }
  if(!p_viewname.IsEmpty())
  {
    IdentifierCorrect(p_viewname);
    sql += _T("   AND table_name  = ?");
  }
  return sql;
}

XString 
SQLInfoMySQL::GetCATALOGViewList(XString& p_schema,XString& p_pattern,bool p_quoted /*= false*/) const
{
  return GetCATALOGViewAttributes(p_schema,p_pattern,p_quoted);
}

XString 
SQLInfoMySQL::GetCATALOGViewAttributes(XString& p_schema,XString& p_viewname,bool p_quoted /*= false*/) const
{
  XString sql;
  sql = _T("SELECT table_catalog\n")
        _T("      ,table_schema\n")
        _T("      ,table_name\n")
        _T("      ,'VIEW' as table_type\n")
        _T("      ,table_comment\n")
        _T("      ,table_catalog || '.' || table_schema || '.' || table_name as fullname\n")
        _T("      ,engine\n")
        _T("      ,0 as temporary\n") 
        _T("  FROM information_schema.tables\n")  
        _T(" WHERE table_type = 'VIEW'\n")
        _T("   AND table_schema NOT IN ('mysql','sys','performance_schema')\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND table_schema = ?\n");
  }
  if(!p_viewname.IsEmpty())
  {
    IdentifierCorrect(p_viewname);
    sql += _T("   AND table_name ");
    sql += p_viewname.Find('%') >= 0 ? _T("LIKE") : _T("=");
    sql += _T(" ?\n");
  }
  sql += _T(" ORDER BY 1,2,3");
  return sql;
}

XString
SQLInfoMySQL::GetCATALOGViewText(XString& p_schema,XString& p_viewname,bool p_quoted /*= false*/) const
{
  XString sql = _T("SELECT view_definition\n"
                   "  FROM information_schema.views\n"
                   " WHERE 1 = 1\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND table_schema = '") + p_schema + _T("'\n");
  }
  if(!p_viewname.IsEmpty())
  {
    IdentifierCorrect(p_viewname);
    sql += _T("   AND table_name   = '") + p_viewname +_T("'");
  }
  return sql;
}

XString
SQLInfoMySQL::GetCATALOGViewCreate(XString /*p_schema*/,XString p_viewname,XString p_contents,bool p_ifexists /*= true*/) const
{
  XString sql = _T("CREATE "); 
  if(p_ifexists)
  {
    sql += _T("OR REPLACE ");
  }
  sql += _T("VIEW ");
  sql += QIQ(p_viewname) + _T(" AS\n") + p_contents;

  return sql;
}

XString 
SQLInfoMySQL::GetCATALOGViewRename(XString /*p_schema*/,XString /*p_viewname*/,XString /*p_newname*/)    const
{
  return _T("");
}

XString 
SQLInfoMySQL::GetCATALOGViewDrop(XString /*p_schema*/,XString p_viewname,XString& p_precursor) const
{
  p_precursor.Empty();
  XString sql = _T("DROP VIEW ");
  sql += QIQ(p_viewname);
  return sql;
}

// All Privilege functions

XString
SQLInfoMySQL::GetCATALOGTablePrivileges(XString& p_schema,XString& p_tablename) const
{
  bool p_quoted(true);
  XString sql = _T("SELECT table_schema AS table_catalog\n"
                   "      ,table_schema\n"
                   "      ,table_name\n"
                   "      ,'_SYSTEM' AS grantor\n"
                   "      ,Trim(both '''' FROM substring_index(grantee,'@',1)) as grantee\n"
                   "      ,privilege_type\n"
                   "      ,is_grantable\n"
                   "  FROM information_schema.table_privileges\n"
                   " WHERE 1 = 1\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND table_schema = ?\n");
  }
  if(!p_tablename.IsEmpty())
  {
    IdentifierCorrect(p_tablename);
    sql += _T("   AND table_name   = ?\n");
  }
  sql += _T(" ORDER BY 1,2,3,5,6");
  return sql;
}

XString 
SQLInfoMySQL::GetCATALOGColumnPrivileges(XString& p_schema,XString& p_tablename,XString& p_columnname) const
{
  bool p_quoted(true);
  XString sql = _T("SELECT table_schema AS table_catalog\n"
                   "      ,table_schema\n"
                   "      ,table_name\n"
                   "      ,column_name\n"
                   "      ,'_SYSTEM' AS grantor\n"
                   "      ,Trim(both '''' FROM substring_index(grantee,'@',1)) as grantee\n"
                   "      ,privilege_type\n"
                   "      ,is_grantable\n"
                   "  FROM information_schema.column_privileges\n"
                   " WHERE 1 = 1\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND table_schema = ?\n");
  }
  if(!p_tablename.IsEmpty())
  {
    IdentifierCorrect(p_tablename);
    sql += _T("   AND table_name   = ?\n");
  }
  if(!p_columnname.IsEmpty())
  {
    IdentifierCorrect(p_columnname);
    sql += _T("   AND column_name  = ?\n");
  }
  sql += _T(" ORDER BY 1,2,3,5,6");
  return sql;
}

XString
SQLInfoMySQL::GetCATALOGSequencePrivilege(XString& /*p_schema*/,XString& /*p_sequence*/) const
{
  return _T("");
}

XString 
SQLInfoMySQL::GetCATALOGGrantPrivilege(XString /*p_schema*/,XString p_objectname,XString p_subObject,XString p_privilege,XString p_grantee,bool p_grantable)
{
  XString sql;

  // MySQL does not know the concept of "PUBLIC"
  if(p_grantee.Compare(_T("public")))
  {
    sql.Format(_T("GRANT %s"),p_privilege.GetString());
    if(!p_subObject.IsEmpty())
    {
      sql.AppendFormat(_T("(%s)"),QIQ(p_subObject).GetString());
    }
    sql.AppendFormat(_T(" ON %s TO %s"),QIQ(p_objectname).GetString(),QIQ(p_grantee).GetString());
    if(p_grantable)
    {
      sql += _T(" WITH GRANT OPTION");
    }
  }
  return sql;
}

XString 
SQLInfoMySQL::GetCATALOGRevokePrivilege(XString /*p_schema*/,XString p_objectname,XString p_subObject,XString p_privilege,XString p_grantee)
{
  XString sql;
  p_grantee.MakeLower();

  // MySQL does not know the concept of "PUBLIC"
  if(p_grantee.Compare(_T("public")))
  {
    sql.Format(_T("REVOKE %s"),p_privilege.GetString());
    if(!p_subObject.IsEmpty())
    {
      sql.AppendFormat(_T("(%s)"),QIQ(p_subObject).GetString());
    }
    sql += _T(" ON ");
    sql.AppendFormat(_T(" ON %s FROM %s"),QIQ(p_objectname).GetString(),QIQ(p_grantee).GetString());
  }
  return sql;
}

// All Synonym functions
XString
SQLInfoMySQL::GetCATALOGSynonymList(XString& /*p_schema*/,XString& /*p_pattern*/) const
{
  // Not implemented yet
  return _T("");
}

XString
SQLInfoMySQL::GetCATALOGSynonymAttributes(XString& /*p_schema*/,XString& /*p_synonym*/) const
{
  // Not implemented yet
  return _T("");
}

XString
SQLInfoMySQL::GetCATALOGSynonymCreate(XString& /*p_schema*/,XString& /*p_synonym*/,XString /*p_forObject*/,bool /*p_private = true*/) const
{
  // Not implemented yet
  return _T("");
}

XString
SQLInfoMySQL::GetCATALOGSynonymDrop(XString& /*p_schema*/,XString& /*p_synonym*/,bool /*p_private = true*/) const
{
  // Not implemented yet
  return _T("");
}

// For ALL objects
XString
SQLInfoMySQL::GetCATALOGCommentCreate(XString /*p_schema*/,XString p_object,XString p_name,XString p_subObject,XString p_remark) const
{
  XString sql;
  if(!p_object.IsEmpty() && !p_name.IsEmpty() && !p_remark.IsEmpty())
  {
    sql.Format(_T("COMMENT ON %s "),p_object.GetString());
    sql += QIQ(p_name);
    if(!p_subObject.IsEmpty())
    {
      sql += _T(".") + QIQ(p_subObject);
    }
    if(p_remark.CompareNoCase(_T("NULL")))
    {
      sql.AppendFormat(_T(" IS '%s'"),p_remark.GetString());
    }
    else
    {
      sql += _T(" IS NULL");
    }
  }
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
SQLInfoMySQL::GetPSMProcedureExists(XString p_schema, XString p_procedure,bool p_quoted /*= false*/) const
{
  XString sql;
  sql = _T("SELECT SELECT COUNT(*)\n")
        _T("  FROM information_schema.routines\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND routine_schema = '") + p_schema + _T("'\n");
  }
  if(!p_procedure.IsEmpty())
  {
    IdentifierCorrect(p_procedure);
    sql += _T("   AND routine_name = '") + p_procedure + _T("'");
  }
  return sql;
}

XString
SQLInfoMySQL::GetPSMProcedureList(XString& p_schema,XString p_procedure,bool p_quoted /*= false*/) const
{
  XString sql;
  sql = _T("SELECT routine_schema as catalog\n")
        _T("      ,routine_schema\n")
        _T("      ,routine_name\n")
        _T("      ,CASE routine_type\n")
        _T("            WHEN 'PROCEDURE' THEN 1\n")
        _T("            WHEN 'FUNCTION'  THEN 2\n")
        _T("                             ELSE 3\n")
        _T("       end\n")
        _T("  FROM information_schema.routines fun\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T(" WHERE routine_schema = ?\n");
  }
  if(!p_procedure.IsEmpty())
  {
    IdentifierCorrect(p_procedure);
    sql += _T("   AND routine_name ");
    sql += (p_procedure.Find(_T("%")) >= 0) ? _T("LIKE") : _T("=");
    sql += _T(" ?\n");
  }
  sql += _T(" ORDER BY 1,2,3");

  return sql;
}

XString
SQLInfoMySQL::GetPSMProcedureAttributes(XString& p_schema,XString& p_procedure,bool p_quoted /*= false*/) const
{
  XString sql;
  sql = _T("SELECT routine_schema as catalog\n")
        _T("      ,routine_schema\n")
        _T("      ,routine_name\n")
        _T("      ,(SELECT COUNT(*)\n")
        _T("          FROM information_schema.parameters par\n")
        _T("         WHERE par.specific_name    = fun.specific_name\n")
        _T("           AND par.specific_catalog = fun.routine_catalog\n")
        _T("           AND par.specific_schema  = fun.routine_schema\n")
        _T("           AND par.parameter_mode IN ('IN','INOUT')) as input_parameters\n")
        _T("      ,(SELECT COUNT(*)\n")
        _T("          FROM information_schema.parameters par\n")
        _T("         WHERE par.specific_name    = fun.specific_name\n")
        _T("           AND par.specific_catalog = fun.routine_catalog\n")
        _T("           AND par.specific_schema  = fun.routine_schema\n")
        _T("           AND par.parameter_mode IN ('OUT','INOUT')) as output_parameters\n")
        _T("      ,(SELECT COUNT(*)\n")
        _T("          FROM information_schema.parameters par\n")
        _T("         WHERE par.specific_name    = fun.specific_name\n")
        _T("           AND par.specific_catalog = fun.routine_catalog\n")
        _T("           AND par.specific_schema  = fun.routine_schema\n")
        _T("           AND par.data_type        = 'set') as result_sets\n")
        _T("      ,'' as remarks\n")
        _T("      ,CASE routine_type\n")
        _T("            WHEN 'PROCEDURE' THEN 1\n")
        _T("            WHEN 'FUNCTION'  THEN 2\n")
        _T("                             ELSE 3\n")
        _T("       end as procedure_type\n")
        _T("      ,'<@>'\n")
        _T("  FROM information_schema.routines fun\n");
  if(!p_schema.IsEmpty())
  { 
    IdentifierCorrect(p_schema);
    sql += _T(" WHERE routine_schema = ?\n");
  }
  if(!p_procedure.IsEmpty())
  {
    IdentifierCorrect(p_procedure);
    sql += p_schema.IsEmpty() ? _T(" WHERE ") : _T("   AND ");
    sql += _T("routine_name ");
    sql += p_procedure.Find('%') >= 0 ? _T("LIKE") : _T("=");
    sql += _T(" ?\n");
  }
  sql += _T(" ORDER BY 1,2,3");
  return sql;
}

// SQL is almost the same as MariaDB with two extensions
// 'READS SQL DATA' - must always be specified
// 'GROUP BY'       - always has two extra columns
XString
SQLInfoMySQL::GetPSMProcedureSourcecode(XString p_schema, XString p_procedure,bool p_quoted /*= false*/) const
{
  XString sql = _T("SELECT 0 as type\n"
                   "      ,0 as line\n"
                   "      ,CONCAT(\n"
                   "       'CREATE OR REPLACE ',r.routine_type,' `',r.routine_name,'`(',\n"
                   "       IFNULL(GROUP_CONCAT(\n"
                   "              CONCAT(p.parameter_mode,' ',p.parameter_name,' ',p.dtd_identifier)\n"
                   "              ORDER BY p.ordinal_position\n"
                   "              SEPARATOR ','),''),\n"
                   "       ')',CHAR(10),IFNULL(CONCAT('RETURNS ',r.dtd_identifier,CHAR(10)),''),\n"
                   "       'READS SQL DATA',CHAR(10),r.routine_definition) AS source\n"
                   "  FROM information_schema.routines r\n"
                   "       LEFT OUTER JOIN information_schema.parameters p ON r.specific_name  = p.specific_name\n"
                   "                                                      AND r.routine_schema = p.specific_schema\n");
  if(!p_schema.IsEmpty())
  { 
    IdentifierCorrect(p_schema);
    sql += _T(" WHERE routine_schema = '") + p_schema + _T("'\n");
  }
  if(!p_procedure.IsEmpty())
  {
    IdentifierCorrect(p_procedure);
    sql += p_schema.IsEmpty() ? _T(" WHERE ") : _T("   AND ");
    sql += _T("routine_name  = '") + p_procedure + _T("'\n");
  }
  sql += _T(" GROUP BY r.routine_schema\n"
            "         ,r.routine_name\n"
            "         ,r.routine_type\n"
            "         ,r.dtd_identifier\n"
            "         ,r.routine_definition");
  return sql;
}

XString
SQLInfoMySQL::GetPSMProcedureCreate(MetaProcedure& p_procedure) const
{
  // Souce fully gotten by previous method
  return p_procedure.m_source;
}

XString
SQLInfoMySQL::GetPSMProcedureDrop(XString /*p_schema*/, XString p_procedure,bool p_function /*=false*/) const
{
  XString sql(_T("DROP "));
  sql += p_function ? _T("FUNCTION ") : _T("PROCEDURE ");
  sql += QIQ(p_procedure);
  return sql;
}

XString
SQLInfoMySQL::GetPSMProcedureErrors(XString /*p_schema*/,XString /*p_procedure*/,bool /*p_quoted = false*/) const
{
  return _T("");
}

XString
SQLInfoMySQL::GetPSMProcedurePrivilege(XString& /*p_schema*/,XString& /*p_procedure*/,bool /*p_quoted = false*/) const
{
  return _T("");
}

// And it's parameters
XString
SQLInfoMySQL::GetPSMProcedureParameters(XString& p_schema,XString& p_procedure,bool p_quoted /*= false*/) const
{
  XString sql;

  sql = _T("SELECT par.specific_schema as catalog\n")
        _T("      ,par.specific_schema\n")
        _T("      ,fun.routine_name\n")
        _T("      ,par.parameter_name\n")
        _T("      ,case par.parameter_mode\n")
        _T("            when 'IN'    then 1\n")
        _T("            when 'OUT'   then 4\n")
        _T("            when 'INOUT' then 2\n")
        _T("       end as columntype\n")
        _T("      ,case par.data_type\n")
        _T("            when 'varchar'  then 1\n")
        _T("            when 'int'      then 4\n")
        _T("            when 'decimal'  then 2\n")
        _T("            when 'tinyint'  then -6\n")
        _T("            when 'bigint'   then -5\n")
        _T("            when 'text'     then 1\n")
        _T("            when 'longtext' then -1\n")
        _T("            when 'datetime' then 11\n")
        _T("                            else 1\n")
        _T("       end as datatype\n")
        _T("      ,par.data_type as typename\n")
        _T("      ,par.character_maximum_length\n")
        _T("      ,par.numeric_precision\n")
        _T("      ,par.numeric_scale\n")
        _T("      ,10 as numeric_precision_radix\n")
        _T("      ,1 as is_nullable\n")
        _T("      ,'' as remarks\n")
        _T("      ,'' as parameter_default\n")
        _T("      ,1 as datatype3\n")
        _T("      ,par.datetime_precision as subtype\n")
        _T("      ,par.character_octet_length\n")
        _T("      ,par.ordinal_position\n")
        _T("      ,'YES' as isNullable\n")
        _T("  FROM information_schema.parameters par\n")
        _T("      ,information_schema.routines fun\n")
        _T(" WHERE par.specific_catalog = fun.routine_catalog\n")
        _T("   AND par.specific_schema  = fun.routine_schema\n")
        _T("   AND par.specific_name    = fun.specific_name\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND fun.routine_schema = ?\n");
  }
  if(!p_procedure.IsEmpty())
  {
    IdentifierCorrect(p_procedure);
    sql += _T("   AND fun.routine_name    = ?\n");
  }
  sql += _T(" ORDER BY 1,2,3,18");

  return sql;
}

//////////////////////////////////////////////////////////////////////////
//
// ALL PSM LANGUAGE ELEMENTS
//
//////////////////////////////////////////////////////////////////////////

XString
SQLInfoMySQL::GetPSMDeclaration(bool    /*p_first*/
                               ,XString p_variable
                               ,int     p_datatype
                               ,int     p_precision /*= 0 */
                               ,int     p_scale     /*= 0 */
                               ,XString p_default   /*= ""*/
                               ,XString /*p_domain    = ""*/
                               ,XString /*p_asColumn  = ""*/) const
{
  XString line;
  line.Format(_T("DECLARE %s "),p_variable.GetString());

  if(p_datatype)
  {
    // Getting type info and name
    const TypeInfo* info = GetTypeInfo(p_datatype);
    line += info->m_type_name;

    if(p_precision > 0)
    {
      line.AppendFormat(_T("(%d"),p_precision);
      if(p_scale > 0)
      {
        line.AppendFormat(_T("%d"),p_scale);
      }
      line += _T(")");
    }

    if(!p_default.IsEmpty())
    {
      line += _T(" DEFAULT ") + p_default;
    }
  }
  line += _T(";\n");
  return line;
}

XString
SQLInfoMySQL::GetPSMAssignment(XString p_variable,XString p_statement /*=""*/) const
{
  XString line(p_variable);
  line += _T(" [?=] ");
  if(!p_statement.IsEmpty())
  {
    line += p_statement;
    line += _T(";");
  }
  return line;
}

XString
SQLInfoMySQL::GetPSMIF(XString p_condition) const
{
  XString line(_T("IF ("));
  line += p_condition;
  line += _T(") THEN\n");
  return line;
}

XString
SQLInfoMySQL::GetPSMIFElse() const
{
  return _T("ELSE\n");
}

XString
SQLInfoMySQL::GetPSMIFEnd() const
{
  return _T("END IF;\n");
}

XString
SQLInfoMySQL::GetPSMWhile(XString p_condition) const
{
  return _T("WHILE (") + p_condition + _T(") DO\n");
}

XString
SQLInfoMySQL::GetPSMWhileEnd() const
{
  return _T("END WHILE;\n");
}

XString
SQLInfoMySQL::GetPSMLOOP() const
{
  return _T("LOOP\n");
}

XString
SQLInfoMySQL::GetPSMLOOPEnd() const
{
  return _T("END LOOP;\n");
}

XString
SQLInfoMySQL::GetPSMBREAK() const
{
  return _T("LEAVE\n");
}

XString
SQLInfoMySQL::GetPSMRETURN(XString p_statement /*= ""*/) const
{
  return _T("RETURN ") + p_statement;
}

XString
SQLInfoMySQL::GetPSMExecute(XString p_procedure,MParameterMap& p_parameters) const
{
  XString line;
  line.Format(_T("EXECUTE %s USING "),p_procedure.GetString());
  bool doMore = false;

  for(const auto& param : p_parameters)
  {
    if(doMore) line += _T(",");
    doMore = true;

    line += _T("@");
    line += param.m_parameter;
  }
  line += _T(";\n");
  return line;
}

// The CURSOR
XString
SQLInfoMySQL::GetPSMCursorDeclaration(XString p_cursorname,XString p_select) const
{
  return _T("DECLARE ") + p_cursorname + _T(" CURSOR FOR ") + p_select + _T(";");
}

XString
SQLInfoMySQL::GetPSMCursorFetch(XString p_cursorname,std::vector<XString>& /*p_columnnames*/,std::vector<XString>& p_variablenames) const
{
  XString query = _T("FETCH ") + p_cursorname + _T(" INTO ");
  bool moreThenOne = false;

  for(const auto& var : p_variablenames)
  {
    if(moreThenOne) query += _T(",");
    moreThenOne = true;
    query += var;
  }
  query += _T(";");
  return query;
}

//////////////////////////////////////////////////////////////////////////
// PSM Exceptions

XString
SQLInfoMySQL::GetPSMExceptionCatchNoData() const
{
  return _T("DECLARE EXIT HANDLER FOR SQLSTATE '02000'\n");
  // Must be followed by a (block)statement
}

XString
SQLInfoMySQL::GetPSMExceptionCatch(XString p_sqlState) const
{
  return _T("DECLARE EXIT HANDLER FOR SQLSTATE '") + p_sqlState + _T("'\n");
  // Must be followed by a (block)statement
}

XString
SQLInfoMySQL::GetPSMExceptionRaise(XString p_sqlState) const
{
  return _T("SIGNAL SQLSTATE '") + p_sqlState + _T("'\n");
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
SQLInfoMySQL::GetSESSIONMyself() const
{
  return _T("");
}

XString
SQLInfoMySQL::GetSESSIONExists(XString /*p_sessionID*/) const
{
  return _T("");
}

XString
SQLInfoMySQL::GetSESSIONList() const
{
  return _T("");
}

XString
SQLInfoMySQL::GetSESSIONAttributes(XString /*p_sessionID*/) const
{
  return _T("");
}

//////////////////////////////////////////////////////////////////////////
// Transactions

XString
SQLInfoMySQL::GetSESSIONConstraintsDeferred() const
{
  // MySQL cannot defer constraints
  return _T("");
}

XString
SQLInfoMySQL::GetSESSIONConstraintsImmediate() const
{
  // MySQL constraints are always active
  return _T("");
}

//////////////////////////////////////////////////////////////////////////
//
// Call FUNCTION/PROCEDURE from within program
// As a RDBMS dependent extension of "DoSQLCall" of the SQLQuery object
//
//////////////////////////////////////////////////////////////////////////

// Calling a stored function or procedure if the RDBMS does not support ODBC call escapes
SQLVariant*
SQLInfoMySQL::DoSQLCall(SQLQuery* /*p_query*/,XString& /*p_schema*/,XString& /*p_procedure*/)
{
  return nullptr;
}

// Calling a stored function with named parameters, returning a value
SQLVariant*
SQLInfoMySQL::DoSQLCallNamedParameters(SQLQuery* /*p_query*/,XString& /*p_schema*/,XString& /*p_procedure*/,bool /*p_function = true*/)
{
  return nullptr;
}

// End of namespace
}
