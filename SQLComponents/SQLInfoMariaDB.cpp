////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoMariaDB.cpp
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

  AddSQLWord(_T("database"));
  AddSQLWord(_T("databases"));
}

// Destructor. Does nothing
SQLInfoMariaDB::~SQLInfoMariaDB()
{
}

// RDBMS Uses INDENTITY or SEQUENCE interface
void
SQLInfoMariaDB::SetUseSequences(bool /*p_sequences*/)
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
SQLInfoMariaDB::GetRDBMSDatabaseType() const
{
  return RDBMS_MARIADB;
}

// The name of the database vendor
XString
SQLInfoMariaDB::GetRDBMSVendorName() const
{
  // The name of the database vendor
  return _T("MariaDB");
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

// Supports the ODBC call procedure with named parameters
bool
SQLInfoMariaDB::GetRDBMSSupportsODBCCallNamedParameters() const
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

// Support for "as" in alias statements (FROM clause)
bool
SQLInfoMariaDB::GetRDBMSSupportsAsInAlias() const
{
  return true;
}

// Foreign key DDL defines the index and cannot reuse already existing ones
bool
SQLInfoMariaDB::GetRDBMSForeignKeyDefinesIndex() const
{
  // If the index exists the creation of the foreign key does NOT fail!!
  return true;
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

// Identifier rules differ per RDBMS
bool
SQLInfoMariaDB::IsIdentifier(XString p_identifier) const
{
  // Cannot be empty and cannot exceed this amount of characters
  if(p_identifier.GetLength() == 0 ||
     p_identifier.GetLength() > (int)GetMaxIdentifierNameLength())
  {
    return false;
  }
  // EXTENSION: Can start with an alpha OR a numeric char
  if(!_istalnum(p_identifier.GetAt(0)))
  {
    return false;
  }
  bool alphaSeen = false;
  for(int index = 0;index < p_identifier.GetLength();++index)
  {
    // Can be upper/lower alpha or a number OR an underscore
    // EXTENSION: Identifiers can contain the '$' sign
    TCHAR ch = p_identifier.GetAt(index);
    if(_istalpha(ch))
    {
      alphaSeen = true;
    }
    else if(!_istalnum(ch) && ch != '_' && ch != '$')
    {
      return false;
    }
  }
  // EXTENSION: Must have at least 1 (one) alpha character
  return alphaSeen;
}

// KEYWORDS

// Keyword for the current date and time
XString
SQLInfoMariaDB::GetKEYWORDCurrentTimestamp() const
{
  return _T("current_timestamp");
}

// String for the current date
XString
SQLInfoMariaDB::GetKEYWORDCurrentDate() const
{
  return _T("current_date");
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
    return _T("||");
  }
  else return _T("");
}

// Get quote character for strings
XString
SQLInfoMariaDB::GetKEYWORDQuoteCharacter() const
{
  return _T("\'");
}

// Get quote character around reserved words as an identifier
XString
SQLInfoMariaDB::GetKEYWORDReservedWordQuote() const
{
  return _T("`");
}

// Get default NULL for parameter list input
XString
SQLInfoMariaDB::GetKEYWORDParameterDefaultNULL() const
{
  // Standard, no definition defines the NULL state
  if (m_oracleMode)
  {
    return _T("DEFAULT NULL");
  }
  else return _T("");
}

// Parameter is for INPUT and OUTPUT in parameter list
XString
SQLInfoMariaDB::GetKEYWORDParameterINOUT() const
{
  if (m_oracleMode)
  {
    return _T("INOUT");
  }
  else return _T("");
}

// Parameter is for OUTPUT only in parameter list
XString
SQLInfoMariaDB::GetKEYWORDParameterOUT() const
{
  if (m_oracleMode)
  {
    return _T("OUT");
  }
  else return _T("");
}

// Get datatype of the IDENTITY primary key in a Network database
XString
SQLInfoMariaDB::GetKEYWORDNetworkPrimaryKeyType() const
{
  // Use SEQUENCE to fill!
  return _T("INTEGER");
}

// Get datatype for timestamp (year to second)
XString
SQLInfoMariaDB::GetKEYWORDTypeTimestamp() const
{
  return _T("TIMESTAMP");
}

// Prefix for a parameter in a stored procedure
XString
SQLInfoMariaDB::GetKEYWORDParameterPrefix() const
{
  return _T("");
}

// Get select part to add new record identity to a table
// Can be special column like 'OID' or a sequence select
XString
SQLInfoMariaDB::GetKEYWORDIdentityString(XString& p_tablename,XString p_postfix /*= "_seq"*/) const
{
  return p_tablename + p_postfix + _T(".nextval");
}

// Gets the UPPER function
XString
SQLInfoMariaDB::GetKEYWORDUpper(XString& p_expression) const
{
  return _T("UPPER(") + p_expression + _T(")");
}

// Gets the construction for 1 minute ago
XString
SQLInfoMariaDB::GetKEYWORDInterval1MinuteAgo() const
{
  return _T("TIMESTAMPADD(MINUTE,-1,CURRENT_TIMESTAMP)");
}

// Gets the Not-NULL-Value statement of the database
XString
SQLInfoMariaDB::GetKEYWORDStatementNVL(XString& p_test,XString& p_isnull) const
{
  return _T("{fn IFNULL(") + p_test + _T(",") + p_isnull + _T(")}");
}

// Gets the RDBMS definition of the datatype
XString
SQLInfoMariaDB::GetKEYWORDDataType(MetaColumn* p_column)
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
SQLInfoMariaDB::GetKEYWORDCurrentUser() const
{
  return _T("CURRENT_USER");
}

// Connects to a default schema in the database/instance
XString
SQLInfoMariaDB::GetSQLDefaultSchema(XString /*p_user*/,XString p_schema) const
{
  return _T("USE ") + p_schema;
}

// Gets the construction for inline generating a key within an INSERT statement
XString
SQLInfoMariaDB::GetSQLNewSerial(XString p_table, XString p_sequence) const
{
  if(!m_oracleMode)
  {
    return _T("");
  }

  XString sequence(p_sequence);
  if(sequence.IsEmpty() && !p_table.IsEmpty())
  {
    sequence = p_table + _T("_seq");
  }
  // Select next value from a generator sequence
  return sequence + _T(".NEXTVAL");
}

// Gets the construction / select for generating a new serial identity
XString
SQLInfoMariaDB::GetSQLGenerateSerial(XString p_table) const
{
  if (m_oracleMode)
  {
    return _T("SELECT ") + p_table + _T("_seq.nextval FROM DUAL");
  }
  else return _T("");
}

XString
SQLInfoMariaDB::GetSQLGenerateSequence(XString p_sequence) const
{
  if(m_oracleMode)
  {
    return _T("SELECT ") + p_sequence + _T(".nextval FROM DUAL");
  }
  else return _T("");
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
  return XString(_T("SAVEPOINT ")) + p_savepointName;
}

XString
SQLInfoMariaDB::GetSQLCommitSubTransaction(XString p_savepointName) const
{
  return XString(_T("RELEASE SAVEPOINT")) = p_savepointName;
}

XString
SQLInfoMariaDB::GetSQLRollbackSubTransaction(XString p_savepointName) const
{
  return XString(_T("ROLLBACK TO SAVEPOINT ")) + p_savepointName;
}

// FROM-Part for a query to select only 1 (one) record / or empty!
XString
SQLInfoMariaDB::GetSQLFromDualClause() const
{
  if(m_oracleMode)
  {
    return _T(" FROM DUAL");
  }
  // MySQL does bare SELECT!
  return _T("");
}

// Get SQL to lock  a table 
XString
SQLInfoMariaDB::GetSQLLockTable(XString p_schema,XString p_tablename,bool p_exclusive,int /*p_waittime*/) const
{
  // Standard ISO SQL Syntax
  XString query = _T("LOCK TABLE ") + p_schema + _T(".") + p_tablename + _T(" IN ");
  query += p_exclusive ? _T("EXCLUSIVE") : _T("SHARE");
  query += _T(" MODE");
  return query;
}

// Get query to optimize the table statistics
XString
SQLInfoMariaDB::GetSQLOptimizeTable(XString p_schema, XString p_tablename) const
{
  return _T("OPTIMIZE TABLE ") + p_schema + _T(".") + p_tablename + _T(" NOWAIT");
}

// Transform query to select top <n> rows
XString
SQLInfoMariaDB::GetSQLTopNRows(XString p_sql,int p_top,int p_skip /*= 0*/) const
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
SQLInfoMariaDB::GetSelectForUpdateTableClause(unsigned /*p_lockWaitTime*/) const
{
  return "";
}

XString
SQLInfoMariaDB::GetSelectForUpdateTrailer(XString p_select,unsigned /*p_lockWaitTime*/) const
{
  return p_select + "\nFOR UPDATE";
}

// Query to perform a keep alive ping
XString
SQLInfoMariaDB::GetPing() const
{
  // Getting the time does a ping
  return _T("SELECT current_timestamp");
}

// Pre- and postfix statements for a bulk import
XString
SQLInfoMariaDB::GetBulkImportPrefix(XString /*p_schema*/,XString /*p_tablename*/,bool /*p_identity = true*/,bool /*p_constraints = true*/) const
{
  return _T("");
}

XString
SQLInfoMariaDB::GetBulkImportPostfix(XString /*p_schema*/,XString /*p_tablename*/,bool /*p_identity = true*/,bool /*p_constraints = true*/) const
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
SQLInfoMariaDB::GetSQLString(const XString& p_string) const
{
  XString s = p_string;
  s.Replace(_T("'"),_T("''"));
  XString kwoot = GetKEYWORDQuoteCharacter();
  return  kwoot + s + kwoot;
}

// Get date string in engine format
XString
SQLInfoMariaDB::GetSQLDateString(int p_year,int p_month,int p_day) const
{
  XString dateString;
  dateString.Format(_T("{d '%04d-%02d-%02d'}"),p_year,p_month,p_day);
  return dateString;
}

// Get time string in database engine format
XString
SQLInfoMariaDB::GetSQLTimeString(int p_hour,int p_minute,int p_second) const
{
  XString retval;
  retval.Format(_T("{t '%02d:%02d:%02d'}"),p_hour,p_minute,p_second);
  return retval;
}

// Get date-time string in database engine format
XString
SQLInfoMariaDB::GetSQLDateTimeString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  XString string;
  string.Format(_T("{ts '%04d-%02d-%02d %02d:%02d:%02d'}")
                ,p_year,p_month,p_day // ODBC Ordering !!
                ,p_hour,p_minute,p_second);
  return string;
}

// Get date-time bound parameter string in database format
XString
SQLInfoMariaDB::GetSQLDateTimeBoundString() const
{
  return _T("{ts ?}");
}

// Stripped data for the parameter binding
XString
SQLInfoMariaDB::GetSQLDateTimeStrippedString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  XString string;
  string.Format(_T("%04d-%02d-%02d %02d:%02d:%02d")
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

// Get the name of a temp table (local temporary or global temporary)
XString
SQLInfoMariaDB::GetTempTablename(XString /*p_schema*/,XString p_tablename,bool /*p_local*/) const
{
  return p_tablename;
}

// Changes to parameters before binding to an ODBC HSTMT handle (returning the At-Exec status)
bool
SQLInfoMariaDB::DoBindParameterFixup(SQLSMALLINT& /*p_dataType*/,SQLSMALLINT& /*p_sqlDatatype*/,SQLULEN& /*p_columnSize*/,SQLSMALLINT& /*p_scale*/,SQLLEN& /*p_bufferSize*/,SQLLEN* /*p_indicator*/) const
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
SQLInfoMariaDB::GetCATALOGMetaTypes(int p_type) const
{
  UNREFERENCED_PARAMETER(p_type);
  return _T("");
}

XString
SQLInfoMariaDB::GetCATALOGDefaultCharset() const
{
  return _T("iso-8859-1");
}

XString
SQLInfoMariaDB::GetCATALOGDefaultCharsetNCV() const
{
  return _T("UTF-16");
}

XString
SQLInfoMariaDB::GetCATALOGDefaultCollation() const
{
  return _T("latin1_swedish_ci");
}

// Get SQL to check if a table already exists in the database
XString
SQLInfoMariaDB::GetCATALOGTableExists(XString& /*p_schema*/,XString& /*p_tablename*/,bool /*p_quoted = false*/) const
{
  // Still to do in MySQL
  return _T("");
}

XString
SQLInfoMariaDB::GetCATALOGTablesList(XString& p_schema,XString& p_pattern,bool p_quoted /*= false*/) const
{
  return GetCATALOGTableAttributes(p_schema,p_pattern,p_quoted);
}

XString
SQLInfoMariaDB::GetCATALOGTableAttributes(XString& p_schema,XString& p_tablename,bool /*p_quoted = false*/) const
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
    sql += _T("   AND table_schema = ?\n");
  }
  if(!p_tablename.IsEmpty())
  {
    sql += _T("   AND table_name ");
    sql += p_tablename.Find('%') >= 0 ? _T("LIKE") : _T("=");
    sql += _T(" ?\n");
  }
  sql += _T(" ORDER BY 1,2,3");
  return sql;
}

XString
SQLInfoMariaDB::GetCATALOGTableSynonyms(XString& /*p_schema*/,XString& /*p_tablename*/,bool /*p_quoted = false*/) const
{
  // MS-Access cannot do this
  return XString();
}

XString
SQLInfoMariaDB::GetCATALOGTableCatalog(XString& p_schema,XString& p_tablename,bool /*p_quoted = false*/) const
{
  p_schema.Empty(); // do not bind as a parameter

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
    sql += _T("   AND table_schema = ?\n");
  }
  if(!p_tablename.IsEmpty())
  {
    sql += _T("   AND table_name ");
    sql += p_tablename.Find('%') >= 0 ? _T("LIKE") : _T("=");
    sql += _T(" ?\n");
  }
  sql += _T(" ORDER BY 1,2,3");
  return sql;
}

XString
SQLInfoMariaDB::GetCATALOGTableCreate(MetaTable& p_table,MetaColumn& /*p_column*/) const
{
  XString sql = _T("CREATE ");
  if(p_table.m_temporary)
  {
    sql += _T("TEMPORARY ");
  }
  sql += _T("TABLE ") + p_table.m_table;
  return sql;
}

XString
SQLInfoMariaDB::GetCATALOGTableCreatePostfix(MetaTable& p_table,MetaColumn& /*p_column*/) const
{
  XString sql;
  if(p_table.m_temporary)
  {
    sql += _T("ENGINE = MEMORY");
  }
  return sql;
}

XString
SQLInfoMariaDB::GetCATALOGTableRename(XString p_schema,XString p_tablename,XString p_newname) const
{
  XString sql(_T("RENAME TABLE") + p_schema + _T(".") + p_tablename + _T(" TO ") + p_newname);
  return sql;
}

XString
SQLInfoMariaDB::GetCATALOGTableDrop(XString p_schema,XString p_tablename,bool p_ifExist /*= false*/,bool p_restrict /*= false*/,bool p_cascade /*= false*/) const
{
  XString sql(_T("DROP TABLE "));
  if (p_ifExist)
  {
    sql += _T("IF EXISTS ");
  }
  sql += p_schema + _T(".") + p_tablename;
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
SQLInfoMariaDB::GetCATALOGTemptableCreate(XString p_schema,XString p_tablename,XString p_select) const
{
  // BEWARE: THIS IS A GUESS. 
  return _T("CREATE TEMPORARY TABLE ") + p_schema + _T(".") + p_tablename + _T("\nAS ") + p_select;
}

XString 
SQLInfoMariaDB::GetCATALOGTemptableIntoTemp(XString /*p_schema*/,XString p_tablename,XString p_select) const
{
  return _T("INSERT INTO ") + p_tablename + _T("\n") + p_select;
}

XString 
SQLInfoMariaDB::GetCATALOGTemptableDrop(XString /*p_schema*/,XString p_tablename) const
{
  return _T("DROP TABLE ") + p_tablename;
}

//////////////////////////////////////////////////////////////////////////
// ALL COLUMN FUNCTIONS

XString 
SQLInfoMariaDB::GetCATALOGColumnExists(XString /*p_schema*/,XString /*p_tablename*/,XString /*p_columnname*/,bool /*p_quoted /*= false*/) const
{
  return _T("");
}

XString 
SQLInfoMariaDB::GetCATALOGColumnList(XString& p_schema,XString& p_tablename,bool p_quoted /*= false*/) const
{
  XString column;
  return GetCATALOGColumnAttributes(p_schema,p_tablename,column,p_quoted);
}

XString
SQLInfoMariaDB::GetCATALOGColumnAttributes(XString& p_schema,XString& p_tablename,XString& p_columnname,bool /*p_quoted /*= false*/) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  p_columnname.MakeLower();

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
                  _T("      ,Nvl(numeric_scale,datetime_precision) AS decimal_digits\n")
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
    query += _T("   AND table_schema = '") + p_schema + _T("'\n");
  }
  if(!p_columnname.IsEmpty())
  {
    query += _T("   AND column_name = '") + p_columnname + _T("'\n");
  }
  query += _T(" ORDER BY ordinal_position ASC");
  return query;
}

XString 
SQLInfoMariaDB::GetCATALOGColumnCreate(MetaColumn& p_column) const
{
  XString sql = _T("ALTER TABLE ")  + p_column.m_table  + _T("\n")
                _T("  ADD COLUMN ") + p_column.m_column + _T(" ") + p_column.m_typename;
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
  XString sql = _T("ALTER  TABLE  ") + p_column.m_table + _T("\n")
                _T("MODIFY COLUMN ") + p_column.m_column + _T(" ") + p_column.m_typename;
  p_column.GetPrecisionAndScale(sql);
  p_column.GetNullable(sql);
  p_column.GetDefault(sql);
  // m_position not used
  // m_length   not used
  // m_remarks  not used
  return sql;
}

XString 
SQLInfoMariaDB::GetCATALOGColumnRename(XString /*p_schema*/,XString p_tablename,XString p_columnname,XString p_newname,XString /*p_datatype*/) const
{
  // General ISO syntax
  XString sql(_T("ALTER  TABLE  ") + p_tablename + _T("\n")
              _T("RENAME ") + p_columnname + _T(" TO ") + p_newname + _T("\n"));
  return sql;
}

XString 
SQLInfoMariaDB::GetCATALOGColumnDrop(XString /*p_schema*/,XString p_tablename,XString p_columnname) const
{
  XString sql(_T("ALTER TABLE  ") + p_tablename + _T("\n")
              _T(" DROP COLUMN ") + p_columnname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL INDICES FUNCTIONS

// All index functions
XString
SQLInfoMariaDB::GetCATALOGIndexExists(XString /*p_schema*/,XString /*p_tablename*/,XString /*p_indexname*/,bool /*p_quoted = false*/) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLStatistics instead (see SQLInfo class)
  return _T("");
}

XString
SQLInfoMariaDB::GetCATALOGIndexList(XString& /*p_schema*/,XString& /*p_tablename*/,bool /*p_quoted = false*/) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLStatistics instead (see SQLInfo class)
  return _T("");
}

XString
SQLInfoMariaDB::GetCATALOGIndexAttributes(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_indexname*/,bool /*p_quoted = false*/) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLStatistics instead (see SQLInfo class)
  return _T("");
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
      query = _T("CREATE ");
      if(index.m_nonunique == false)
      {
        query += _T("UNIQUE ");
      }
      query += _T("INDEX ");
      query += index.m_indexName;
      query += _T(" ON ");
      if(!index.m_schemaName.IsEmpty())
      {
        query += index.m_schemaName + _T(".");
      }
      query += index.m_tableName;
      query += _T("(");
    }
    else
    {
      query += _T(",");
    }
    query += index.m_columnName;

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
SQLInfoMariaDB::GetCATALOGIndexDrop(XString p_schema,XString p_tablename,XString p_indexname) const
{
  XString sql = _T("ALTER TABLE ") + p_schema + _T(".") + p_tablename + _T("\n")
                _T(" DROP INDEX ") + p_indexname;
  return sql;
}

// Get extra filter expression for an index column
XString
SQLInfoMariaDB::GetCATALOGIndexFilter(MetaIndex& /*p_index*/) const
{
  return _T("");
}

//////////////////////////////////////////////////////////////////////////
// ALL PRIMARY KEY FUNCTIONS

XString
SQLInfoMariaDB::GetCATALOGPrimaryExists(XString /*p_schema*/,XString /*p_tablename*/,bool /*p_quoted = false*/) const
{
  // To be implemented
  return _T("");
}

XString
SQLInfoMariaDB::GetCATALOGPrimaryAttributes(XString& /*p_schema*/,XString& /*p_tablename*/,bool /*p_quoted = false*/) const
{
  // To be implemented
  return _T("");
}

// In MariaDB all primary keys are named "PRIMARY".
// You can add a constraint name, but it WILL be ignored!
XString
SQLInfoMariaDB::GetCATALOGPrimaryCreate(MPrimaryMap& p_primaries) const
{
  XString query(_T("ALTER TABLE "));

  for(const auto& prim : p_primaries)
  {
    if(prim.m_columnPosition == 1)
    {
      query += prim.m_schema + _T(".") + prim.m_table + _T("\n");
      query += _T("  ADD PRIMARY KEY (");

    }
    else
    {
      query += _T(",");
    }
    query += prim.m_columnName;
  }
  query += _T(")");
  return query;
}

// In MariaDB all primary keys are named "PRIMARY".
XString
SQLInfoMariaDB::GetCATALOGPrimaryDrop(XString p_schema,XString p_tablename,XString /*p_constraintname*/) const
{
  XString sql(_T("ALTER TABLE ") + p_schema + _T(".") + p_tablename + _T("\n")
              _T(" DROP CONSTRAINT PRIMARY"));
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL FOREIGN KEY FUNCTIONS

XString
SQLInfoMariaDB::GetCATALOGForeignExists(XString /*p_schema*/,XString /*p_tablename*/,XString /*p_constraintname*/,bool /*p_quoted = false*/) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLForeignKeys instead (see SQLInfo class)
  return _T("");
}

XString
SQLInfoMariaDB::GetCATALOGForeignList(XString& /*p_schema*/,XString& /*p_tablename*/,bool /*p_quoted = false*/) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLForeignKeys instead (see SQLInfo class)
  return _T("");
}

XString
SQLInfoMariaDB::GetCATALOGForeignAttributes(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_constraintname*/,bool /*p_referenced = false*/,bool /*p_quoted = false*/) const
{
  // Cannot be implemented for generic ODBC
  // Use SQLForeignKeys instead (see SQLInfo class)
  return _T("");
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
  XString query = _T("ALTER TABLE ") + table + _T("\n")
                  _T("  ADD CONSTRAINT ") + foreign.m_foreignConstraint + _T("\n")
                  _T("      FOREIGN KEY (");

  // Add the foreign key columns
  bool extra = false;
  for(const auto& key : p_foreigns)
  {
    if(extra) query += _T(",");
    query += key.m_fkColumnName;
    extra = true;
  }

  // Add references primary table
  query += _T(")\n      REFERENCES ") + primary + _T("(");

  // Add the primary key columns
  extra = false;
  for(const auto& key : p_foreigns)
  {
    if(extra) query += _T(",");
    query += key.m_pkColumnName;
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
SQLInfoMariaDB::GetCATALOGForeignAlter(MForeignMap& p_original, MForeignMap& p_requested) const
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
  XString query = _T("ALTER TABLE ") + table + _T("\n")
                  _T("ALTER CONSTRAINT ") + original.m_foreignConstraint + _T("\n");

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
SQLInfoMariaDB::GetCATALOGForeignDrop(XString p_schema,XString p_tablename,XString p_constraintname) const
{
  XString sql(_T("ALTER TABLE ") + p_schema + _T(".") + p_tablename + _T("\n")
              _T(" DROP CONSTRAINT ") + p_constraintname);
  return sql;
}

//////////////////////////
// All default constraints
XString
SQLInfoMariaDB::GetCATALOGDefaultExists(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_column*/) const
{
  return _T("");
}

XString
SQLInfoMariaDB::GetCATALOGDefaultList(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  return _T("");
}

XString
SQLInfoMariaDB::GetCATALOGDefaultAttributes(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_column*/) const
{
  return _T("");
}

XString
SQLInfoMariaDB::GetCATALOGDefaultCreate(XString /*p_schema*/,XString /*p_tablename*/,XString /*p_constraint*/,XString /*p_column*/,XString /*p_code*/) const
{
  return _T("");
}

XString
SQLInfoMariaDB::GetCATALOGDefaultDrop(XString /*p_schema*/,XString /*p_tablename*/,XString /*p_constraint*/) const
{
  return _T("");
}

/////////////////////////
// All check constraints

XString
SQLInfoMariaDB::GetCATALOGCheckExists(XString  /*p_schema*/,XString  /*p_tablename*/,XString  /*p_constraint*/,bool /*p_quoted = false*/) const
{
  return _T("");
}

XString
SQLInfoMariaDB::GetCATALOGCheckList(XString  /*p_schema*/,XString  /*p_tablename*/,bool /*p_quoted = false*/) const
{
  return _T("");
}

XString
SQLInfoMariaDB::GetCATALOGCheckAttributes(XString  /*p_schema*/,XString  /*p_tablename*/,XString  /*p_constraint*/,bool /*p_quoted = false*/) const
{
  return _T("");
}

XString
SQLInfoMariaDB::GetCATALOGCheckCreate(XString  /*p_schema*/,XString  /*p_tablename*/,XString  /*p_constraint*/,XString /*p_condition*/) const
{
  return _T("");
}

XString
SQLInfoMariaDB::GetCATALOGCheckDrop(XString  /*p_schema*/,XString  /*p_tablename*/,XString  /*p_constraint*/) const
{
  return _T("");
}

//////////////////////////////////////////////////////////////////////////
// ALL TRIGGER FUNCTIONS

XString
SQLInfoMariaDB::GetCATALOGTriggerExists(XString /*p_schema*/, XString /*p_tablename*/, XString /*p_triggername*/,bool /*p_quoted = false*/) const
{
  return _T("");
}

XString
SQLInfoMariaDB::GetCATALOGTriggerList(XString& p_schema,XString& p_tablename,bool p_quoted /*= false*/) const
{
  XString triggername;
  return GetCATALOGTriggerAttributes(p_schema,p_tablename,triggername,p_quoted);
}

XString
SQLInfoMariaDB::GetCATALOGTriggerAttributes(XString& p_schema,XString& p_tablename,XString& p_triggername,bool /*p_quoted = false*/) const
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
    sql += _T(" WHERE event_object_schema = ?\n");
  }
  if(!p_tablename.IsEmpty())
  {
    sql += p_schema.IsEmpty() ? _T(" WHERE ") : _T("   AND ");
    sql += _T("event_object_table = ?\n");
  }
  if(!p_triggername.IsEmpty())
  {
    sql += p_schema.IsEmpty() && p_tablename.IsEmpty() ? _T(" WHERE ") : _T("   AND ");
    sql += _T("trigger_name = ?\n");
  }
  sql += _T(" ORDER BY 1,2,3,4");
  return sql;
}

XString
SQLInfoMariaDB::GetCATALOGTriggerCreate(MetaTrigger& /*p_trigger*/) const
{
  return _T("");
}

XString
SQLInfoMariaDB::GetCATALOGTriggerDrop(XString /*p_schema*/, XString /*p_tablename*/, XString /*p_triggername*/) const
{
  return _T("");
}

//////////////////////////////////////////////////////////////////////////
// ALL SEQUENCE FUNCTIONS

XString
SQLInfoMariaDB::GetCATALOGSequenceExists(XString p_schema, XString p_sequence,bool /*p_quoted = false*/) const
{
  p_schema.Empty();
  p_sequence.MakeLower();

  XString sql = _T("SELECT COUNT(*)\n")
                _T("  FROM information_schema.tables tab\n")
                _T(" WHERE tab.table_type = 'SEQUENCE'\n")
                _T("   AND sequence_name  = '");
  sql += p_sequence;
  sql += _T("\'");
  return sql;
}

XString
SQLInfoMariaDB::GetCATALOGSequenceList(XString& p_schema,XString& p_pattern,bool /*p_quoted = false*/) const
{
  p_schema.MakeLower();
  p_pattern.MakeLower();
  if(p_pattern.Find('%') < 0)
  {
    p_pattern = _T("%") + p_pattern + _T("%");
  }

  XString sql = _T("SELECT tab.table_catalog as catalog_name\n")
                _T("      ,tab.table_schema  as schema_name\n")
                _T("      ,tab.table_name    as sequence_name\n")
                _T("      ,0 AS current_value\n")
                _T("      ,0 AS minimal_value\n")
                _T("      ,0 AS seq_increment\n")
                _T("      ,0 AS cache\n")
                _T("      ,0 AS cycle\n")
                _T("      ,0 AS ordering\n")
                _T("  FROM information_schema.tables tab\n")
                _T(" WHERE tab.table_type = 'SEQUENCE'");
  if(!p_schema.IsEmpty())
  {
    sql += _T("\n   AND table_schema = ?");
  }
  if(!p_pattern.IsEmpty())
  {
    sql += _T("\n   AND table_name LIKE ?");
  }
  return sql;
}

XString
SQLInfoMariaDB::GetCATALOGSequenceAttributes(XString& p_schema,XString& p_sequence,bool /*p_quoted = false*/) const
{
  p_schema.MakeLower();
  p_sequence.MakeLower();
  XString table;
  if(!p_schema.IsEmpty())
  {
    table = p_schema + _T(".");
  }
  table += p_sequence;

  XString sql = _T("SELECT tab.table_catalog   AS catalog_name\n")
                _T("      ,tab.table_schema    AS schema_name\n")
                _T("      ,tab.table_name      AS sequence_name\n")
                _T("      ,seq.start_value     AS current_value\n")
                _T("      ,seq.minimum_value   AS minimal_value\n")
                _T("      ,seq.increment       AS seq_increment\n")
                _T("      ,seq.cache_size      AS cache\n")
                _T("      ,seq.cycle_option    AS cycle\n")
                _T("      ,0                   AS ordering\n")
                _T("  FROM information_schema.tables as tab\n")
                _T("      ,") + table + _T(" as seq\n")
                _T(" WHERE tab.table_type = 'SEQUENCE'");
  if(!p_schema.IsEmpty())
  {
    sql += _T("\n   AND tab.table_schema = ?");
  }
  if(!p_sequence.IsEmpty())
  {
    sql += _T("\n   AND tab.table_name  = ?");
  }
  return sql;
}

XString
SQLInfoMariaDB::GetCATALOGSequenceCreate(MetaSequence& p_sequence) const
{
  XString sql(_T("CREATE OR REPLACE SEQUENCE "));

  sql += p_sequence.m_sequenceName;
  sql.AppendFormat(_T("\n START WITH %-12.0f"), p_sequence.m_currentValue);
  sql.AppendFormat(_T("\n INCREMENT BY %d"),    p_sequence.m_increment);

  sql += p_sequence.m_cycle ? _T("\n CYCLE") : _T("\n NOCYCLE");
  if (p_sequence.m_cache > 0)
  {
    sql.AppendFormat(_T("\n CACHE %d"),p_sequence.m_cache);
  }
  else
  {
    sql += _T("\n NOCACHE");
  }
  return sql;
}

XString
SQLInfoMariaDB::GetCATALOGSequenceDrop(XString /*p_schema*/,XString p_sequence) const
{
  XString sql(_T("DROP SEQUENCE ") + p_sequence);
  return  sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL VIEW FUNCTIONS

XString 
SQLInfoMariaDB::GetCATALOGViewExists(XString& /*p_schema*/,XString& /*p_viewname*/,bool /*p_quoted = false*/) const
{
  return _T("");
}

XString 
SQLInfoMariaDB::GetCATALOGViewList(XString& p_schema,XString& p_pattern,bool /*p_quoted = false*/) const
{
  return GetCATALOGViewAttributes(p_schema,p_pattern);
}

XString
SQLInfoMariaDB::GetCATALOGViewAttributes(XString& p_schema,XString& p_viewname,bool /*p_quoted = false*/) const
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
    sql += _T("   AND table_schema = ?\n");
  }
  if(!p_viewname.IsEmpty())
  {
    sql += _T("   AND table_name ");
    sql += p_viewname.Find('%') >= 0 ? _T("LIKE") : _T("=");
    sql += _T(" ?\n");
  }
  sql += _T(" ORDER BY 1,2,3");
  return sql;
}

XString
SQLInfoMariaDB::GetCATALOGViewText(XString& /*p_schema*/,XString& /*p_viewname*/,bool /*p_quoted = false*/) const
{
  // Cannot query this, Use ODBC functions
  return _T("");
}

XString
SQLInfoMariaDB::GetCATALOGViewCreate(XString /*p_schema*/,XString p_viewname,XString p_contents,bool /*p_ifexists /*= true*/) const
{
  return _T("CREATE VIEW ") + p_viewname + _T("\n") + p_contents;
}

XString 
SQLInfoMariaDB::GetCATALOGViewRename(XString /*p_schema*/,XString /*p_viewname*/,XString /*p_newname*/)    const
{
  return _T("");
}

XString 
SQLInfoMariaDB::GetCATALOGViewDrop(XString /*p_schema*/,XString p_viewname,XString& p_precursor) const
{
  p_precursor.Empty();
  return _T("DROP VIEW ") + p_viewname;
}

// All Privilege functions
XString
SQLInfoMariaDB::GetCATALOGTablePrivileges(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  return _T("");
}

XString 
SQLInfoMariaDB::GetCATALOGColumnPrivileges(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_columnname*/) const
{
  return _T("");
}

XString
SQLInfoMariaDB::GetCATALOGSequencePrivilege(XString& /*p_schema*/,XString& /*p_sequence*/) const
{
  return _T("");
}

XString 
SQLInfoMariaDB::GetCATALOGGrantPrivilege(XString p_schema,XString p_objectname,XString p_subObject,XString p_privilege,XString p_grantee,bool p_grantable)
{
  XString sql;
  sql.Format(_T("GRANT %s"),p_privilege.GetString());
  if(!p_subObject.IsEmpty())
  {
    sql.AppendFormat(_T("(%s)"),QIQ(p_subObject).GetString());
  }
  sql += _T(" ON ");
  if(!p_schema.IsEmpty())
  {
    sql.AppendFormat(_T("%s."),QIQ(p_schema).GetString());
  }
  sql.AppendFormat(_T("%s TO %s"),QIQ(p_objectname).GetString(),QIQ(p_grantee).GetString());
  if(p_grantable)
  {
    sql += _T(" WITH GRANT OPTION");
  }
  return sql;
}

XString 
SQLInfoMariaDB::GetCATALOGRevokePrivilege(XString p_schema,XString p_objectname,XString p_subObject,XString p_privilege,XString p_grantee)
{
  XString sql;
  sql.Format(_T("REVOKE %s"),p_privilege.GetString());
  if(!p_subObject.IsEmpty())
  {
    sql.AppendFormat(_T("(%s)"),QIQ(p_subObject).GetString());
  }
  sql += _T(" ON ");
  if(!p_schema.IsEmpty())
  {
    sql.AppendFormat(_T("%s."),QIQ(p_schema).GetString());
  }
  sql.AppendFormat(_T("%s FROM %s"),QIQ(p_objectname).GetString(),QIQ(p_grantee).GetString());
  return sql;
}

// All Synonym functions
XString
SQLInfoMariaDB::GetCATALOGSynonymList(XString& /*p_schema*/,XString& /*p_pattern*/) const
{
  // Not implemented yet
  return _T("");
}

XString
SQLInfoMariaDB::GetCATALOGSynonymAttributes(XString& /*p_schema*/,XString& /*p_synonym*/) const
{
  // Not implemented yet
  return _T("");
}

XString
SQLInfoMariaDB::GetCATALOGSynonymCreate(XString& /*p_schema*/,XString& /*p_synonym*/,XString /*p_forObject*/,bool /*p_private = true*/) const
{
  // Not implemented yet
  return _T("");
}

XString
SQLInfoMariaDB::GetCATALOGSynonymDrop(XString& /*p_schema*/,XString& /*p_synonym*/,bool /*p_private = true*/) const
{
  // Not implemented yet
  return _T("");
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
SQLInfoMariaDB::GetPSMProcedureExists(XString p_schema, XString p_procedure,bool /*p_quoted = false*/) const
{
  XString sql;
  sql = _T("SELECT SELECT COUNT(*)\n")
        _T("  FROM information_schema.routines\n");
  if(!p_schema.IsEmpty())
  { 
    sql += _T("   AND routine_schema = '") + p_schema + _T("'\n");
  }
  if(!p_procedure.IsEmpty())
  {
    sql += _T("   AND routine_name = '") + p_procedure + _T("'");
  }
  return sql;
}

XString
SQLInfoMariaDB::GetPSMProcedureList(XString& p_schema,XString p_procedure,bool /*p_quoted = false*/) const
{
  XString sql;
  sql = _T("SELECT routine_catalog\n")
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
    sql += _T(" WHERE routine_schema = ?\n");
  }
  if(!p_procedure.IsEmpty())
  {
    sql += _T("   AND routine_name ");
    sql += (p_procedure.Find(_T("%")) >= 0) ? _T("LIKE") : _T("=");
    sql += _T("\n");
  }
  sql += _T(" ORDER BY 1,2,3");

  return sql;
}

XString
SQLInfoMariaDB::GetPSMProcedureAttributes(XString& p_schema,XString& p_procedure,bool /*p_quoted = false*/) const
{
  XString sql;
  sql = _T("SELECT routine_catalog\n")
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
        _T("      ,routine_definition\n")
        _T("  FROM information_schema.routines fun\n");
  if(!p_schema.IsEmpty())
  { 
    sql += _T(" WHERE routine_schema = ?\n");
  }
  if(!p_procedure.IsEmpty())
  {
    sql += p_schema.IsEmpty() ? _T(" WHERE ") : _T("   AND ");
    sql += _T("routine_name ");
    sql += p_procedure.Find('%') >= 0 ? _T("LIKE") : _T("=");
    sql += _T(" ?\n");
  }
  sql += _T(" ORDER BY 1,2,3");
  return sql;
}

XString
SQLInfoMariaDB::GetPSMProcedureSourcecode(XString /*p_schema*/, XString /*p_procedure*/,bool /*p_quoted = false*/) const
{
  // Source-code already gotten with attributes
  return _T("");
}

XString
SQLInfoMariaDB::GetPSMProcedureCreate(MetaProcedure& /*p_procedure*/) const
{
  return _T("");
}

XString
SQLInfoMariaDB::GetPSMProcedureDrop(XString /*p_schema*/, XString /*p_procedure*/,bool /*p_function /*=false*/) const
{
  return _T("");
}

XString
SQLInfoMariaDB::GetPSMProcedureErrors(XString /*p_schema*/,XString /*p_procedure*/,bool /*p_quoted = false*/) const
{
  return _T("");
}

XString
SQLInfoMariaDB::GetPSMProcedurePrivilege(XString& /*p_schema*/,XString& /*p_procedure*/,bool /*p_quoted = false*/) const
{
  return _T("");
}

// And it's parameters
XString
SQLInfoMariaDB::GetPSMProcedureParameters(XString& p_schema,XString& p_procedure,bool /*p_quoted = false*/) const
{
  XString sql;

  sql = _T("SELECT par.specific_catalog\n")
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
    sql += _T("   AND fun.routine_schema = ?\n");
  }
  if(!p_procedure.IsEmpty())
  {
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
SQLInfoMariaDB::GetPSMAssignment(XString p_variable,XString p_statement /*=""*/) const
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
SQLInfoMariaDB::GetPSMIF(XString p_condition) const
{
  XString line(_T("IF ("));
  line += p_condition;
  line += _T(") THEN\n");
  return line;
}

XString
SQLInfoMariaDB::GetPSMIFElse() const
{
  return _T("ELSE\n");
}

XString
SQLInfoMariaDB::GetPSMIFEnd() const
{
  return _T("END IF;\n");
}

XString
SQLInfoMariaDB::GetPSMWhile(XString p_condition) const
{
  return _T("WHILE (") + p_condition + _T(") DO\n");
}

XString
SQLInfoMariaDB::GetPSMWhileEnd() const
{
  return _T("END WHILE;\n");
}

XString
SQLInfoMariaDB::GetPSMLOOP() const
{
  return _T("LOOP\n");
}

XString
SQLInfoMariaDB::GetPSMLOOPEnd() const
{
  return _T("END LOOP;\n");
}

XString
SQLInfoMariaDB::GetPSMBREAK() const
{
  return _T("LEAVE\n");
}

XString
SQLInfoMariaDB::GetPSMRETURN(XString p_statement /*= ""*/) const
{
  return _T("RETURN ") + p_statement;
}

XString
SQLInfoMariaDB::GetPSMExecute(XString p_procedure,MParameterMap& p_parameters) const
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
SQLInfoMariaDB::GetPSMCursorDeclaration(XString p_cursorname,XString p_select) const
{
  return _T("DECLARE ") + p_cursorname + _T(" CURSOR FOR ") + p_select + _T(";");
}

XString
SQLInfoMariaDB::GetPSMCursorFetch(XString p_cursorname,std::vector<XString>& /*p_columnnames*/,std::vector<XString>& p_variablenames) const
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
SQLInfoMariaDB::GetPSMExceptionCatchNoData() const
{
  return _T("DECLARE EXIT HANDLER FOR SQLSTATE '02000'\n");
  // Must be followed by a (block)statement
}

XString
SQLInfoMariaDB::GetPSMExceptionCatch(XString p_sqlState) const
{
  return _T("DECLARE EXIT HANDLER FOR SQLSTATE '") + p_sqlState + _T("'\n");
  // Must be followed by a (block)statement
}

XString
SQLInfoMariaDB::GetPSMExceptionRaise(XString p_sqlState) const
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
SQLInfoMariaDB::GetSESSIONMyself() const
{
  return _T("");
}

XString
SQLInfoMariaDB::GetSESSIONExists(XString /*p_sessionID*/) const
{
  return _T("");
}

XString
SQLInfoMariaDB::GetSESSIONList() const
{
  return _T("");
}

XString
SQLInfoMariaDB::GetSESSIONAttributes(XString /*p_sessionID*/) const
{
  return _T("");
}

//////////////////////////////////////////////////////////////////////////
// Transactions

XString
SQLInfoMariaDB::GetSESSIONConstraintsDeferred() const
{
  // MySQL cannot defer constraints
  return _T("");
}

XString
SQLInfoMariaDB::GetSESSIONConstraintsImmediate() const
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
SQLInfoMariaDB::DoSQLCall(SQLQuery* /*p_query*/,XString& /*p_schema*/,XString& /*p_procedure*/)
{
  return nullptr;
}

// Calling a stored function with named parameters, returning a value
SQLVariant*
SQLInfoMariaDB::DoSQLCallNamedParameters(SQLQuery* /*p_query*/,XString& /*p_schema*/,XString& /*p_procedure*/,bool /*p_function = true*/)
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
  XString sql = _T("SELECT INSTR(@@sql_mode,'ORACLE') > 0 AS IsOracle");

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
