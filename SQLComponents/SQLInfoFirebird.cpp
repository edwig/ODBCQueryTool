////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoFirebird.cpp
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
#include "SQLInfoFirebird.h"
#include "SQLQuery.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{

// For Firebird the identifiers are transformed to UPPER case
// as the system catalog is stored in this case setting.
#define IdentifierCorrect(ident)   if(!p_quoted || !IsIdentifierMixedCase(ident)) ident.MakeUpper()

// Constructor.
SQLInfoFirebird::SQLInfoFirebird(SQLDatabase* p_database)
                :SQLInfoDB(p_database)
{
}

// Destructor. Does nothing
SQLInfoFirebird::~SQLInfoFirebird()
{
}

// RDBMS Uses INDENTITY or SEQUENCE interface
void
SQLInfoFirebird::SetUseSequences(bool /*p_sequences*/)
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
SQLInfoFirebird::GetRDBMSDatabaseType() const
{
  return RDBMS_FIREBIRD;
}

// The name of the database vendor
XString
SQLInfoFirebird::GetRDBMSVendorName() const
{
  // The name of the database vendor
  return "Firebird";
}

// Get the physical database name
XString
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

// Supports the ODBC call procedure with named parameters
bool
SQLInfoFirebird::GetRDBMSSupportsODBCCallNamedParameters() const
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

// Supports functions at the place of table columns in create/alter index statement
bool
SQLInfoFirebird::GetRDBMSSupportsFunctionalIndexes() const
{
  return false;
}

// Support for "as" in alias statements (FROM clause)
bool
SQLInfoFirebird::GetRDBMSSupportsAsInAlias() const
{
  return true;
}

// Foreign key DDL defines the index and cannot reuse already existing ones
bool
SQLInfoFirebird::GetRDBMSForeignKeyDefinesIndex() const
{
  // If an index already exists, the foreign key definition will fail
  return true;
}

// Gets the maximum length of an SQL statement
unsigned long 
SQLInfoFirebird::GetRDBMSMaxStatementLength() const
{
  // Limit to a statement
  return 32765;
}

// Database must commit DDL commands in a transaction
bool
SQLInfoFirebird::GetRDBMSMustCommitDDL() const
{
  return true;
}

// Correct maximum precision,scale for a NUMERIC datatype
void
SQLInfoFirebird::GetRDBMSNumericPrecisionScale(SQLULEN& /*p_precision*/, SQLSMALLINT& /*p_scale*/) const
{
  // NO-OP
}

// Maximum for a VARCHAR to be handled without AT-EXEC data. Assume NVARCHAR is half that size!
int
SQLInfoFirebird::GetRDBMSMaxVarchar() const
{
  // See: http://www.firebirdmanual.com/firebird/en/firebird-manual/2/the-firebird-limits/36
  return 32765;
}

// Identifier rules differ per RDBMS
bool
SQLInfoFirebird::IsIdentifier(XString p_identifier) const
{
  if(p_identifier.GetLength() == 0 ||  // Cannot be empty
     p_identifier.GetLength() >= 64 )  // Cannot exceed 63 chars
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
    if(!_istalnum(p_identifier.GetAt(index)))
    {
      return false;
    }
  }
  return true;
}

// KEYWORDS

// Keyword for the current date and time
XString
SQLInfoFirebird::GetKEYWORDCurrentTimestamp() const
{
  return _T("current_timestamp");
}

// String for the current date
XString
SQLInfoFirebird::GetKEYWORDCurrentDate() const
{
  return _T("current_date");
}

// Get the concatenation operator
XString
SQLInfoFirebird::GetKEYWORDConcatanationOperator() const
{
  return _T("||");
}

// Get quote character for strings
XString
SQLInfoFirebird::GetKEYWORDQuoteCharacter() const
{
  return _T("\'");
}

// Get quote character around reserved words as an identifier
XString
SQLInfoFirebird::GetKEYWORDReservedWordQuote() const
{
  return _T("\"");
}

// Get default NULL for parameter list input
XString
SQLInfoFirebird::GetKEYWORDParameterDefaultNULL() const
{
  // Standard, no definition defines the NULL state
  return _T("");
}

// Parameter is for INPUT and OUTPUT in parameter list
XString
SQLInfoFirebird::GetKEYWORDParameterINOUT() const
{
  // Firebird works with the "RETURNS" list !
  return _T("");
}

// Parameter is for OUTPUT only in parameter list
XString
SQLInfoFirebird::GetKEYWORDParameterOUT() const
{
  return _T("");
}

// Get datatype of the IDENTITY primary key in a Network database
XString
SQLInfoFirebird::GetKEYWORDNetworkPrimaryKeyType() const
{
  // Use SEQUENCE / GENERATOR to fill!
  return _T("INTEGER");
}

// Get datatype for timestamp (year to second)
XString
SQLInfoFirebird::GetKEYWORDTypeTimestamp() const
{
  return _T("TIMESTAMP");
}

// Prefix for a parameter in a stored procedure
XString
SQLInfoFirebird::GetKEYWORDParameterPrefix() const
{
  return _T(":");
}

// Get select part to add new record identity to a table
// Can be special column like 'OID' or a sequence select
XString
SQLInfoFirebird::GetKEYWORDIdentityString(XString& p_tablename,XString p_postfix /*= "_seq"*/) const
{
  return _T("GEN_ID(") + p_tablename + p_postfix + _T(",1)");
}

// Gets the UPPER function
XString
SQLInfoFirebird::GetKEYWORDUpper(XString& p_expression) const
{
  return _T("UPPER(") + p_expression + _T(")");
}

// Gets the construction for 1 minute ago
XString
SQLInfoFirebird::GetKEYWORDInterval1MinuteAgo() const
{
  // Not supported by Firebird
  return _T("ERROR");
}

// Gets the Not-NULL-Value statement of the database
XString
SQLInfoFirebird::GetKEYWORDStatementNVL(XString& p_test,XString& p_isnull) const
{
  return _T("COALESCE(") + p_test + _T(",") + p_isnull + _T(")");
}

// Gets the RDBMS definition of the datatype
XString
SQLInfoFirebird::GetKEYWORDDataType(MetaColumn* p_column)
{
  XString type;
  switch(p_column->m_datatype)
  {
    case SQL_CHAR:                      // fall through
    case SQL_VARCHAR:                   // fall through
    case SQL_WCHAR:                     // fall through
    case SQL_WVARCHAR:                  type = _T("VARCHAR");
                                        break;
    case SQL_LONGVARCHAR:               // fall through
    case SQL_WLONGVARCHAR:              type = _T("VARCHAR");  // CLOB -> VARCHAR
                                        break;
    case SQL_INTEGER:                   type = _T("INTEGER");
                                        break;
    case SQL_TINYINT:                   // fall through
    case SQL_SMALLINT:                  type = _T("TINYINT");
                                        break;
    case SQL_BIGINT:                    type = _T("BIGINT");
                                        break;
    case SQL_NUMERIC:                   // fall through
    case SQL_DECIMAL:                   // fall through
    case SQL_FLOAT:                     // fall through
    case SQL_REAL:                      // fall through
    case SQL_DOUBLE:                    // fall through
    case SQL_BIT:                       type = _T("NUMERIC");
                                        if(p_column->m_columnSize == SQLNUM_MAX_PREC && p_column->m_decimalDigits == 0)
                                        {
                                          type = _T("INTEGER");
                                          p_column->m_columnSize    = 0;
                                          p_column->m_decimalDigits = 0;
                                        }
                                        else
                                        {
                                          // Firebird knows precisions up to 18
                                          if(p_column->m_columnSize > 18)
                                          {
                                            p_column->m_columnSize = 18;
                                            if(p_column->m_decimalDigits == 16)
                                            {
                                              p_column->m_decimalDigits = 2;
                                            }
                                          }
                                          if(p_column->m_decimalDigits > p_column->m_columnSize)
                                          {
                                            p_column->m_decimalDigits = p_column->m_columnSize - 1;
                                          }
                                          // Preserve scale!
                                          if(p_column->m_decimalDigits == 0)
                                          {
                                            p_column->m_decimalDigits = -1;
                                          }
                                        }
                                        break;
    //case SQL_DATE:
    case SQL_DATETIME:                  // fall through
    case SQL_TYPE_DATE:                 // fall through
    case SQL_TIMESTAMP:                 // fall through
    case SQL_TYPE_TIMESTAMP:            type = _T("TIMESTAMP");
                                        p_column->m_columnSize    = 0;
                                        p_column->m_decimalDigits = 0;
                                        break;
    case SQL_TIME:                      // fall through
    case SQL_TYPE_TIME:                 type = _T("TIME");
                                        p_column->m_columnSize    = 0;
                                        p_column->m_decimalDigits = 0;
                                        break;
    case SQL_BINARY:                    type = _T("BLOB");          break;
    case SQL_VARBINARY:                 type = _T("BLOB");          break;
    case SQL_LONGVARBINARY:             type = _T("BLOB");          break;
    case SQL_GUID:                      // fall through
    case SQL_INTERVAL_YEAR:             // fall through
    case SQL_INTERVAL_YEAR_TO_MONTH:    // fall through
    case SQL_INTERVAL_MONTH:            type = _T("VARCHAR");
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
    case SQL_INTERVAL_DAY_TO_SECOND:    type = _T("VARCHAR");
                                        p_column->m_columnSize    = 80;
                                        p_column->m_decimalDigits = 0;
                                        break;
    case SQL_UNKNOWN_TYPE:              // fall through
    default:                            break;
  }


  // QUERY VARCHAR(2147483647) NOT NULL      BLOB SUB_TYPE TEXT
  // CLOB CONTROLE IN FIREBIRD
  if(type == _T("VARCHAR") || type == _T("CHAR"))
  {
    if(p_column->m_columnSize == 2147483647)
    {
      type = _T("BLOB SUB_TYPE TEXT");
      p_column->m_columnSize = 0;
    }
    // Does sometimes occur in conversions from other databases
    // that reserve a minimum for varchar columns
    p_column->m_decimalDigits = 0;
  }

  return p_column->m_typename = type;
}

// Gets the USER (current-user) keyword function
XString
SQLInfoFirebird::GetKEYWORDCurrentUser() const
{
  return _T("CURRENT_USER");
}

// Connects to a default schema in the database/instance
XString
SQLInfoFirebird::GetSQLDefaultSchema(XString /*p_user*/,XString /*p_schema*/) const
{
  return _T("");
}

// Gets the construction for inline generating a key within an INSERT statement
XString
SQLInfoFirebird::GetSQLNewSerial(XString p_table, XString p_sequence) const
{
  XString sequence(p_sequence);
  if (sequence.IsEmpty() && !p_table.IsEmpty())
  {
    sequence = p_table + _T("_seq");
  }

  // Select next value from a generator sequence
  return _T("(NEXT VALUE FOR ") + QIQ(sequence) + _T(")");
}

// Gets the construction / select for generating a new serial identity
XString
SQLInfoFirebird::GetSQLGenerateSerial(XString p_table) const
{
  return _T("SELECT (next value for ") + QIQ(p_table) + _T("_seq) FROM RDB$DATABASE");
}

XString
SQLInfoFirebird::GetSQLGenerateSequence(XString p_sequence) const
{
  return _T("SELECT (next value for ") + QIQ(p_sequence) + _T(") FROM RDB$DATABASE");
}

// Gets the construction / select for the resulting effective generated serial
XString
SQLInfoFirebird::GetSQLEffectiveSerial(XString p_identity) const
{
  // Just return it, it's the correct value
  return p_identity;
}

// Gets the sub-transaction commands
XString
SQLInfoFirebird::GetSQLStartSubTransaction(XString p_savepointName) const
{
  return XString(_T("SAVEPOINT ")) + p_savepointName;
}

XString
SQLInfoFirebird::GetSQLCommitSubTransaction(XString p_savepointName) const
{
  return XString(_T("COMMIT TRANSACTION ")) + p_savepointName;
}

XString
SQLInfoFirebird::GetSQLRollbackSubTransaction(XString p_savepointName) const
{
  return XString(_T("ROLLBACK TO ")) + p_savepointName;
}

// FROM-Part for a query to select only 1 (one) record / or empty!
XString
SQLInfoFirebird::GetSQLFromDualClause() const
{
  return _T(" FROM rdb$database");
}

// Get SQL to lock  a table 
XString
SQLInfoFirebird::GetSQLLockTable(XString /*p_schema*/, XString /*p_tablename*/, bool /*p_exclusive*/,int /*p_waittime*/) const
{
  // Firebird does NOT have a LOCK-TABLE statement
  return _T("");
}

// Get query to optimize the table statistics
XString
SQLInfoFirebird::GetSQLOptimizeTable(XString /*p_schema*/, XString /*p_tablename*/) const
{
  // Firebird has no SQL for this, it uses the "GFIX.EXE -sweep <database>" tool
  return _T("");
}

// Transform query to select top <n> rows
XString
SQLInfoFirebird::GetSQLTopNRows(XString p_sql,int p_top,int p_skip /*= 0*/) const
{
  if(p_top > 0 && p_sql.Find(_T("SELECT ")) == 0)
  {
    // FIREBIRD: SELECT FIRST <top> [ SKIP <skip> ] ....
    XString selectFirst;
    selectFirst.Format(_T("SELECT FIRST %d "),p_top);

    if(p_skip > 0)
    {
      selectFirst.AppendFormat(_T("SKIP %d "),p_skip);
    }

    p_sql.Replace(_T("SELECT "),selectFirst);
  }
  return p_sql;
}

// Expand a SELECT with an 'FOR UPDATE' lock clause
XString
SQLInfoFirebird::GetSelectForUpdateTableClause(unsigned /*p_lockWaitTime*/) const
{
  return "";
}

XString
SQLInfoFirebird::GetSelectForUpdateTrailer(XString p_select,unsigned p_lockWaitTime) const
{
  XString sql = p_select + "\nFOR UPDATE";
  if(p_lockWaitTime)
  {
    sql += "\nWITH LOCK";
  }
  return sql;
}

// Query to perform a keep alive ping
XString
SQLInfoFirebird::GetPing() const
{
  // Not implemented yet
  return "SELECT CAST('now' AS timestamp) FROM rdb$database";
}

// Pre- and postfix statements for a bulk import
XString
SQLInfoFirebird::GetBulkImportPrefix(XString /*p_schema*/,XString /*p_tablename*/,bool /*p_identity = true*/,bool /*p_constraints = true*/) const
{
  return _T("");
}

XString
SQLInfoFirebird::GetBulkImportPostfix(XString /*p_schema*/,XString /*p_tablename*/,bool /*p_identity = true*/,bool /*p_constraints = true*/) const
{
  return _T("");
}

//////////////////////////////////////////////////////////////////////////
//
// SQL STRINGS
//
//////////////////////////////////////////////////////////////////////////

XString
SQLInfoFirebird::GetSQLString(const XString& p_string) const
{
  XString s = p_string;
  s.Replace(_T("'"),_T("''"));
  XString kwoot = GetKEYWORDQuoteCharacter();
  return  kwoot + s + kwoot;
}

XString
SQLInfoFirebird::GetSQLDateString(int p_year,int p_month,int p_day) const
{
  XString retval;
  retval.Format(_T("CAST '%02d/%02d/%04d' AS DATE)"),p_day,p_month,p_year); // American order!!
  return retval;
}

XString
SQLInfoFirebird::GetSQLTimeString(int p_hour,int p_minute,int p_second) const
{
  XString time;
  time.Format(_T("%2.2d:%2.2d:%2.2d"),p_hour,p_minute,p_second);
  return time;
}

XString
SQLInfoFirebird::GetSQLDateTimeString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  XString retval;
  retval.Format(_T("CAST('%02d/%02d/%04d %02d:%02d:%02d' AS TIMESTAMP)")
                ,p_day,p_month,p_year       // American order !!
                ,p_hour,p_minute,p_second); // 24 hour clock
  return retval;
}

// Get date-time bound parameter string in database format
XString
SQLInfoFirebird::GetSQLDateTimeBoundString() const
{
  return _T("CAST(? AS TIMESTAMP)");
}

// Stripped data for the parameter binding
XString
SQLInfoFirebird::GetSQLDateTimeStrippedString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  XString retval;
  retval.Format(_T("%02d/%02d/%04d %02d:%02d:%02d")
                ,p_day,p_month,p_year       // American order !!
                ,p_hour,p_minute,p_second); // 24 hour clock
  return retval;
}

// Makes an catalog identifier string (possibly quoted on both sides)
XString
SQLInfoFirebird::GetSQLDDLIdentifier(XString p_identifier) const
{
  return p_identifier;
}

// Get the name of a temp table (local temporary or global temporary)
XString
SQLInfoFirebird::GetTempTablename(XString /*p_schema*/,XString p_tablename,bool /*p_local*/) const
{
  return p_tablename;
}

// Changes to parameters before binding to an ODBC HSTMT handle  (returning the At-Exec status)
bool
SQLInfoFirebird::DoBindParameterFixup(SQLSMALLINT& /*p_dataType*/
                                     ,SQLSMALLINT& /*p_sqlDatatype*/
                                     ,SQLULEN&     /*p_columnSize*/
                                     ,SQLSMALLINT& /*p_scale*/
                                     ,SQLLEN&      /*p_bufferSize*/
                                     ,SQLLEN*      /*p_indicator*/) const
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
//   - Alter (where possible)
//   - Drop
//
//////////////////////////////////////////////////////////////////////////

// Meta info about meta types
// Standard ODBC functions are good enough
XString 
SQLInfoFirebird::GetCATALOGMetaTypes(int p_type) const
{
  UNREFERENCED_PARAMETER(p_type);
  return "";
}

// Convert Firebird character set names to MS-Windows names
XString
SQLInfoFirebird::GetCATALOGDefaultCharset() const
{
 return _T("SELECT CASE(rdb$character_set_name)\n"
           "       WHEN 'UTF8'        THEN 'UTF-8'\n"
           "       WHEN 'UNICODE_FSS' THEN 'UTF-8'\n"
           "       WHEN 'ISO8859_1'   THEN 'ISO-8859-1'\n"
           "       WHEN 'ISO8859_2'   THEN 'ISO-8859-2'\n"
           "       WHEN 'ISO8859_3'   THEN 'ISO-8859-3'\n"
           "       WHEN 'ISO8859_4'   THEN 'ISO-8859-4'\n"
           "       WHEN 'ISO8859_5'   THEN 'ISO-8859-5'\n"
           "       WHEN 'ISO8859_6'   THEN 'ISO-8859-6'\n"
           "       WHEN 'ISO8859_7'   THEN 'ISO-8859-7'\n"
           "       WHEN 'ISO8859_8'   THEN 'ISO-8859-8'\n"
           "       WHEN 'ISO8859_9'   THEN 'ISO-8859-9'\n"
           "       WHEN 'ISO8859_13'  THEN 'ISO-8859-13'\n"
           "       WHEN 'ISO8859_15'  THEN 'ISO-8859-15'\n"
           "       WHEN 'WIN1250'     THEN 'Windows-1250'\n"
           "       WHEN 'WIN1251'     THEN 'Windows-1251'\n"
           "       WHEN 'WIN1252'     THEN 'Windows-1252'\n"
           "       WHEN 'WIN1253'     THEN 'Windows-1253'\n"
           "       WHEN 'WIN1254'     THEN 'Windows-1254'\n"
           "       WHEN 'WIN1255'     THEN 'Windows-1255'\n"
           "       WHEN 'WIN1256'     THEN 'Windows-1256'\n"
           "       WHEN 'WIN1257'     THEN 'Windows-1257'\n"
           "       WHEN 'SJIS_0208'   THEN 'EUC-JP'\n"
           "       WHEN 'DOS437'      THEN 'IBM437'\n"
           "       WHEN 'DOS850'      THEN 'IBM850'\n"
           "       WHEN 'DOS852'      THEN 'IBM852'\n"
           "       WHEN 'DOS855'      THEN 'IBM855'\n"
           "       WHEN 'DOS857'      THEN 'IBM857'\n"
           "       WHEN 'DOS858'      THEN 'IBM00858'\n"
           "       WHEN 'DOS860'      THEN 'IBM860'\n"
           "       WHEN 'DOS861'      THEN 'IBM861'\n"
           "       WHEN 'DOS862'      THEN 'DOS-862'\n"
           "       WHEN 'DOS863'      THEN 'IBM863'\n"
           "       WHEN 'DOS864'      THEN 'IBM864'\n"
           "       WHEN 'DOS865'      THEN 'IBM865'\n"
           "       WHEN 'DOS866'      THEN 'CP66'\n"
           "       WHEN 'DOS869'      THEN 'IBM869'\n"
           "       WHEN 'BIG_5'       THEN 'big5'\n"
           "                          ELSE 'NONE'\n"
           "       END\n"
           "  FROM rdb$database");
}

XString
SQLInfoFirebird::GetCATALOGDefaultCharsetNCV() const
{
  // Firebird does *NOT* support NVARCHAR
  return _T("-");
}

XString
SQLInfoFirebird::GetCATALOGDefaultCollation() const
{
  return GetCATALOGDefaultCharset();
}

// All user defined compound data types
XString
SQLInfoFirebird::GetCATALOGTypeExists(XString& p_schema,XString& p_typename,bool p_quoted /*= false*/) const
{
  XString sql(_T("SELECT COUNT(*)\n"
                 "  FROM rdb$fields fld\n"
                 " WHERE rdb$validation_source IS NOT NULL\n"));
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND fld.rdb$owner_name = ?\n");
  }
  IdentifierCorrect(p_typename);
  sql += _T("   AND fld.rdb$field_name = ?");
  return sql;
}

XString
SQLInfoFirebird::GetCATALOGTypeList(XString& p_schema,XString& p_pattern,bool p_quoted /*= false*/) const
{
  XString sql(_T("SELECT Trim(dbs.mon$database_name) AS catalog_name\n"
                 "      ,Trim(fld.rdb$owner_name)    AS schema_name\n"
                 "      ,Trim(fld.rdb$field_name)    AS type_name\n"
                 "      ,'D'                         AS typeusage\n"
                 "      ,1                           AS ordinal\n"
                 "  FROM mon$database dbs\n"
                 "      ,rdb$fields fld\n"
                 " WHERE rdb$validation_source IS NOT NULL\n"));
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND fld.rdb$owner_name = ?\n");
  }
  if(!p_pattern.IsEmpty())
  {
    IdentifierCorrect(p_pattern);
    sql += _T("   AND fld.rdb$field_name ");
    sql += p_pattern.Find(_T('%')) >= 0 ? _T("LIKE") : _T("=");
    sql += _T(" ?\n");
  }
  sql += _T(" ORDER by 1,2,3");
  return sql;
}

XString
SQLInfoFirebird::GetCATALOGTypeAttributes(XString& p_schema,XString& p_typename,bool p_quoted /*= false*/) const
{
  XString sql(_T("SELECT Trim(dbs.mon$database_name) AS catalog_name\n"
                 "      ,Trim(fld.rdb$owner_name)    AS schema_name\n"
                 "      ,Trim(fld.rdb$field_name)    AS type_name\n"
                 "      ,'D'                         AS typeusage\n"
                 "      ,1                           AS ordinal\n"
                 "      ,''                          AS attributename\n"
                 "      ,CASE fld.rdb$field_type\n"
                 "            WHEN 7  THEN 'SMALLINT'\n"
                 "            WHEN 8  THEN 'INTEGER'\n"
                 "            WHEN 9  THEN 'QUAD'\n"
                 "            WHEN 10 THEN 'FLOAT'\n"
                 "            WHEN 11 THEN 'D_FLOAT'\n"
                 "            WHEN 12 THEN 'DATE'\n"
                 "            WHEN 13 THEN 'TIME'\n"
                 "            WHEN 14 THEN 'CHAR'\n"
                 "            WHEN 16 THEN CASE fld.rdb$field_sub_type\n"
                 "                              WHEN 0 THEN 'BIGINT'\n"
                 "                              WHEN 1 THEN 'NUMERIC'\n"
                 "                              WHEN 2 THEN 'DECIMAL'\n"
                 "                                     ELSE 'BIGINT'\n"
                 "                         END\n"
                 "            WHEN 23 THEN 'BOOLEAN'\n"
                 "            WHEN 27 THEN 'DOUBLE PRECISION'\n"
                 "            WHEN 35 THEN 'TIMESTAMP'\n"
                 "            WHEN 37 THEN 'VARCHAR'\n"
                 "            WHEN 40 THEN 'CSTRING'\n"
                 "            WHEN 261 THEN CASE fld.rdb$field_sub_type\n"
                 "                               WHEN 0 THEN 'BLOB SUB_TYPE 0'\n"
                 "                               WHEN 1 THEN 'BLOB SUB_TYPE TEXT'\n"
                 "                                      ELSE 'BLOB'\n"
                 "                          END\n"
                 "                     ELSE 'UNKNOWN'\n"
                 "       END ||\n"
                 "       CASE WHEN rdb$field_type = 16 AND rdb$field_sub_type IN (1,2)\n"
                 "            THEN '(' || rdb$field_precision || ',' || (rdb$field_scale * -1) || ')'\n"
                 "            ELSE '' END ||\n"
                 "       CASE WHEN fld.rdb$field_type IN (14,37,40)\n"
                 "            THEN '(' || rdb$field_length || ')'\n"
                 "            ELSE ''\n"
                 "       END\n"
                 "      ,rdb$null_flag               AS notnullable\n"
                 "      ,rdb$default_source          AS defaultvalue\n"
                 "      ,rdb$validation_source       AS domaincheck\n"
                 "      ,rdb$description             AS remarks\n"
                 "      ,'<@>'                       AS source\n"
                 "  FROM mon$database dbs\n"
                 "      ,rdb$fields fld\n"
                 " WHERE rdb$validation_source IS NOT NULL\n"));
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND fld.rdb$owner_name = ?\n");
  }
  if(!p_typename.IsEmpty())
  {
    IdentifierCorrect(p_typename);
    sql += _T("   AND fld.rdb$field_name ");
    sql += p_typename.Find(_T('%')) >= 0 ? _T("LIKE") : _T("=");
    sql += _T(" ?\n");
  }
  sql += _T(" ORDER by 1,2,3,5");
  return sql;
}

XString
SQLInfoFirebird::GetCATALOGTypeSource(XString& p_schema,XString& p_typename,bool p_quoted /*= false*/) const
{
  XString sql(_T("SELECT 1 as object\n"
                 "      ,1 as line\n"
                 "      ,'CREATE DOMAIN \"' || Trim(fld.rdb$field_name) || '\" AS ' ||\n"
                 "       CASE fld.rdb$field_type\n"
                 "            WHEN 7  THEN 'SMALLINT'\n"
                 "            WHEN 8  THEN 'INTEGER'\n"
                 "            WHEN 9  THEN 'QUAD'\n"
                 "            WHEN 10 THEN 'FLOAT'\n"
                 "            WHEN 11 THEN 'D_FLOAT'\n"
                 "            WHEN 12 THEN 'DATE'\n"
                 "            WHEN 13 THEN 'TIME'\n"
                 "            WHEN 14 THEN 'CHAR'\n"
                 "            WHEN 16 THEN CASE fld.rdb$field_sub_type\n"
                 "                              WHEN 0 THEN 'BIGINT'\n"
                 "                              WHEN 1 THEN 'NUMERIC'\n"
                 "                              WHEN 2 THEN 'DECIMAL'\n"
                 "                                     ELSE 'BIGINT'\n"
                 "                         END\n"
                 "            WHEN 23 THEN 'BOOLEAN'\n"
                 "            WHEN 27 THEN 'DOUBLE PRECISION'\n"
                 "            WHEN 35 THEN 'TIMESTAMP'\n"
                 "            WHEN 37 THEN 'VARCHAR'\n"
                 "            WHEN 40 THEN 'CSTRING'\n"
                 "            WHEN 261 THEN CASE fld.rdb$field_sub_type\n"
                 "                               WHEN 0 THEN 'BLOB SUB_TYPE 0'\n"
                 "                               WHEN 1 THEN 'BLOB SUB_TYPE TEXT'\n"
                 "                                      ELSE 'BLOB'\n"
                 "                          END\n"
                 "                     ELSE 'UNKNOWN'\n"
                 "       END ||\n"
                 "       CASE WHEN rdb$field_type = 16 AND rdb$field_sub_type IN (1,2)\n"
                 "            THEN '(' || rdb$field_precision || ',' || (rdb$field_scale * -1) || ')'\n"
                 "            ELSE '' END ||\n"
                 "       CASE WHEN fld.rdb$field_type IN (14,37,40)\n"
                 "            THEN '(' || rdb$field_length || ')'\n"
                 "            ELSE ''\n"
                 "       END ||\n"
                 "       COALESCE(' ' || rdb$default_source,'') || ' ' ||\n"
                 "       CASE rdb$null_flag\n"
                 "            WHEN 1 THEN 'NOT NULL '\n"
                 "            ELSE ''\n"
                 "       END || rdb$validation_source AS source\n"
                 "  FROM rdb$fields fld\n"));
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T(" WHERE fld.rdb$owner_name = '") + p_schema + _T("'\n");
  }
  IdentifierCorrect(p_typename);
  sql += p_schema.IsEmpty() ? _T(" WHERE ") : _T("   AND ");
  sql += _T("fld.rdb$field_name = '") + p_typename + _T("'");
  return sql;
}

XString
SQLInfoFirebird::GetCATALOGTypeCreate(MUserTypeMap& p_type) const
{
  if(!p_type.empty())
  {
    return p_type[0].m_source;
  }
  return _T("");
}

XString
SQLInfoFirebird::GetCATALOGTypeDrop(XString /*p_schema*/,XString p_typename) const
{
  XString sql(_T("DROP DOMAIN "));
  sql += QIQ(p_typename);
  return sql;
}

// Get SQL to check if a table already exists in the database
XString
SQLInfoFirebird::GetCATALOGTableExists(XString& p_schema,XString& p_tablename,bool p_quoted /*= false*/) const
{
  XString query = _T("SELECT COUNT(*)\n"
                     "  FROM rdb$relations rel\n"
                     " WHERE rel.rdb$relation_type IN (0,2,3,4,5)\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    query += _T(" AND rel.rdb$owner_name = ?\n");
  }
  IdentifierCorrect(p_tablename);
  query += _T(" AND rel.rdb$relation_name = ?");

  return query;
}

XString
SQLInfoFirebird::GetCATALOGTablesList(XString& p_schema,XString& p_pattern,bool p_quoted /*= false*/) const
{
  return GetCATALOGTableAttributes(p_schema,p_pattern,p_quoted);
}

// Attributes of a table
XString
SQLInfoFirebird::GetCATALOGTableAttributes(XString& p_schema,XString& p_tablename,bool p_quoted /*= false*/) const
{
  XString sql = _T("SELECT TRIM(dbs.mon$database_name)  AS table_catalog\n"
                   "      ,TRIM(rel.rdb$owner_name)     AS table_schema\n"
                   "      ,TRIM(rel.rdb$relation_name)  AS table_name\n"
                   "      ,CASE rel.rdb$relation_type\n"
                   "            WHEN 0 THEN 'TABLE'\n"
                   "            WHEN 2 THEN 'TABLE'\n"
                   "            WHEN 4 THEN 'GLOBAL TEMPORARY'\n"
                   "            WHEN 5 THEN 'LOCAL TEMPORARY'\n"
                   "                   ELSE 'UNKNOWN'\n"
                   "       END               AS table_type\n"
                   "      ,TRIM(rel.rdb$description) AS remarks\n"
                   "      ,TRIM(rel.rdb$owner_name) || '.' || TRIM(rel.rdb$relation_name) AS full_name\n"
                   "      ,cast('' as varchar(31)) as storage_space\n"
                   "      ,CASE rel.rdb$relation_type\n"
                   "            WHEN 4 THEN 1\n"  // 4 = GTT (preserve rows)
                   "            WHEN 5 THEN 1\n"  // 5 = GTT (delete rows)
                   "                   ELSE 0\n"
                   "       END  AS temporary_table\n"
                   "  FROM rdb$relations rel\n"
                   "      ,mon$database  dbs"
                   " WHERE rel.rdb$system_flag = 0\n"
                   "   AND rel.rdb$relation_type IN (0,2,3,4,5)\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND rel.rdb$owner_name = ?\n");
  }
  if(!p_tablename.IsEmpty())
  {
    IdentifierCorrect(p_tablename);
    sql += _T("   AND rel.rdb$relation_name ");
    sql += (p_tablename.Find(_T("%")) >= 0) ? _T("LIKE") : _T("=");
    sql += " ?";
  }
  return sql;
}

XString
SQLInfoFirebird::GetCATALOGTableSynonyms(XString& /*p_schema*/,XString& /*p_tablename*/,bool /*p_quoted = false*/) const
{
  // Firebird does not have/support SYNONYMS. So we select nothing!
  XString sql = _T("SELECT * FROM rdb$database\n")
                _T(" WHERE 0 = 1");
  return sql;
}

XString
SQLInfoFirebird::GetCATALOGTableCatalog(XString& p_schema,XString& p_tablename,bool p_quoted /*= false*/) const
{
  XString sql = _T("SELECT TRIM(dbs.mon$database_name)         AS table_catalog\n"
                   "      ,TRIM(rel.rdb$owner_name)            AS table_schema\n"
                   "      ,TRIM(rel.rdb$relation_name)         AS table_name\n"
                   "      ,CAST('SYSTEM TABLE' as varchar(31)) AS table_type\n"
                   "      ,rel.rdb$description                 AS remarks\n"
                   "      ,trim(rel.rdb$owner_name) || '.' || trim(rel.rdb$relation_name) AS full_name\n"
                   "      ,cast('' as varchar(31))             as storage_space\n"
                   "      ,CASE rdb$relation_type\n"
                   "            WHEN 4 THEN 1\n"  // 4 = GTT (preserve rows)
                   "            WHEN 5 THEN 1\n"  // 5 = GTT (delete rows)
                   "            ELSE 0\n"
                   "       END  as temporary\n"
                   "  FROM rdb$relations rel\n"
                   "      ,mon$database  dbs\n"
                   " WHERE rel.rdb$relation_type IN (0,2,3,4,5)\n"  // 1 = view
                   "   AND rel.rdb$system_flag = 1\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND rel.rdb$owner_name = ?\n");
  }
  if(!p_tablename.IsEmpty())
  {
    IdentifierCorrect(p_tablename);
    sql += _T("   AND rel.rdb$relation_name ");
    sql += (p_tablename.Find(_T("%")) >= 0) ? _T("LIKE") : _T("=");
    sql += _T(" ?");
  }
  return sql;
}

XString
SQLInfoFirebird::GetCATALOGTableCreate(MetaTable& p_table,MetaColumn& /*p_column*/) const
{
  XString sql = _T("CREATE ");
  if (p_table.m_temporary)
  {
    sql += _T("GLOBAL TEMPORARY ");
  }
  sql += _T("TABLE ") + QIQ(p_table.m_table);
  return sql;
}

XString
SQLInfoFirebird::GetCATALOGTableCreatePostfix(MetaTable& /*p_table*/,MetaColumn& /*p_column*/) const
{
  return XString();
}

XString
SQLInfoFirebird::GetCATALOGTableRename(XString /*p_schema*/,XString /*p_oldName*/,XString /*p_newName*/) const
{
  // Not supported by Firebird
  // Changing database table names is not supported by this database type
  return _T("");
}

XString
SQLInfoFirebird::GetCATALOGTableDrop(XString /*p_schema*/
                                    ,XString   p_tablename
                                    ,bool    /*p_ifExist = false*/
                                    ,bool    /*p_restrict = false*/
                                    ,bool    /*p_cascade = false*/) const
{
  return _T("DROP TABLE ") + QIQ(p_tablename);
}

//////////////////////////////////////////////////////////////////////////
// ALL TEMPORARY TABLE FUNCTIONS

XString 
SQLInfoFirebird::GetCATALOGTemptableCreate(XString /*p_schema*/,XString p_tablename,XString p_select) const
{
  return _T("CREATE GLOBAL TEMPORARY TABLE ") + QIQ(p_tablename) + _T("\nAS ") + p_select +
         _T("ON COMMIT PRESERVE ROWS");
}

XString 
SQLInfoFirebird::GetCATALOGTemptableIntoTemp(XString /*p_schema*/,XString p_tablename,XString p_select) const
{
  return _T("INSERT INTO ") + QIQ(p_tablename) + _T("\n") + p_select;
}

XString 
SQLInfoFirebird::GetCATALOGTemptableDrop(XString /*p_schema*/,XString p_tablename) const
{
  return _T("DROP TABLE ") + QIQ(p_tablename);
}

//////////////////////////////////////////////////////////////////////////
// ALL COLUMN FUNCTIONS

XString 
SQLInfoFirebird::GetCATALOGColumnExists(XString /*p_schema*/,XString p_tablename,XString p_columnname,bool p_quoted /*= false*/) const
{
  IdentifierCorrect(p_tablename);
  IdentifierCorrect(p_columnname);
  XString query = _T("SELECT COUNT(*)\n")
                  _T("  FROM rdb$relation_fields\n")
                  _T(" WHERE rdb$relation_name = '") + p_tablename  + _T("'\n")
                  _T("   AND rdb$field_name    = '") + p_columnname + _T("'");
  return query;
}

XString 
SQLInfoFirebird::GetCATALOGColumnList(XString& p_schema,XString& p_tablename,bool p_quoted /*= false*/) const
{
  XString column;
  return GetCATALOGColumnAttributes(p_schema,p_tablename,column,p_quoted);
}

XString 
SQLInfoFirebird::GetCATALOGColumnAttributes(XString& p_schema,XString& p_tablename,XString& p_columnname,bool p_quoted /*= false*/) const
{
  XString sql;
  sql = _T("SELECT TRIM(dbs.mon$database_name) as table_catalog\n"         // 1  - VARCHAR
           "      ,trim(tbl.rdb$owner_name)    as table_schema\n"	         // 2  - VARCHAR
           "      ,trim(col.rdb$relation_name) as table_name\n"            // 3  - VARCHAR NOT NULL
           "      ,trim(col.rdb$field_name)    as column_name\n"           // 4  - VARCHAR NOT NULL
           "      ,CASE fld.rdb$field_type\n"
           "            WHEN 7  THEN 5\n"
           "            WHEN 8  THEN 4\n"
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
           "            WHEN 7  THEN 'SMALLINT'\n"
           "            WHEN 8  THEN 'INTEGER'\n"
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
           "            WHEN 7  THEN 5\n"
           "            WHEN 8  THEN 11\n"
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
           "       END                                        as column_size\n"              // 7  - INTEGER
           "      ,cast(fld.rdb$field_length as integer)      as buffer_length\n"            // 8  - INTEGER
           "      ,cast (fld.rdb$field_scale as smallint)*-1  as decimal_digits\n"           // 9  - SMALLINT
           "      ,10                                         as num_prec_radix\n"           // 10 - SMALLINT
           "      ,CASE coalesce(col.rdb$null_flag,0)\n"
           "            WHEN 0 THEN 1\n"
           "                   ELSE 0\n"
           "       END                                        as nullable\n"                 // 11 - SMALLINT NOT NULL
           "      ,trim(col.rdb$description)                  as remarks\n"                  // 12 - VARCHAR
           "      ,trim(col.rdb$default_source)               as column_def\n"               // 13 - VARCHAR
           "      ,CASE fld.rdb$field_type\n"
           "            WHEN 7  THEN 5\n"
           "            WHEN 8  THEN 4\n"
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
           "      ,CASE coalesce(col.rdb$null_flag,0)\n"
           "            WHEN 0 THEN 'YES'\n"
           "            WHEN 1 THEN 'NO'\n"
           "                   ELSE 'UNKNOWN'\n"
           "       END                                         AS is_nullable\n"             // 18 - VARCHAR
           "  FROM rdb$relation_fields  col\n"
           "      ,rdb$fields           fld\n"
           "      ,rdb$relations        tbl\n"
            "     ,mon$database         dbs\n"
           " WHERE col.rdb$field_source  = fld.rdb$field_name\n"
           "   AND col.rdb$relation_name = tbl.rdb$relation_name\n");
  // Schema name
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND tbl.rdb$owner_name = ?\n");
  }
  // Table name
  if(!p_tablename.IsEmpty())
  {
    IdentifierCorrect(p_tablename);
    sql += _T("   AND tbl.rdb$relation_name = ?\n");
  }
  // Optionally add the column name
  if(!p_columnname.IsEmpty())
  {
    IdentifierCorrect(p_columnname);
    sql += _T("   AND col.rdb$field_name = ?\n");
  }
  sql += _T(" ORDER BY col.rdb$field_position");
  return sql;
}

XString SQLInfoFirebird::GetCATALOGColumnCreate(MetaColumn& p_column) const
{
  XString sql  = _T("ALTER TABLE ")  + QIQ(p_column.m_table)  + _T("\n")
                 _T("  ADD COLUMN ") + QIQ(p_column.m_column) + _T(" ") + p_column.m_typename;
  p_column.GetPrecisionAndScale(sql);
  p_column.GetNullable(sql);
  return sql;
}

XString SQLInfoFirebird::GetCATALOGColumnAlter(MetaColumn& p_column) const
{
  // The extra 'TYPE' keyword  is a-typical
  // The SET/DROP for the NULL is a-typical
  XString sql  = _T("ALTER TABLE ")         + QIQ(p_column.m_table)  + _T("\n")
                 _T("      MODIFY COLUMN ") + QIQ(p_column.m_column) + _T(" TYPE ") + p_column.m_typename;
  p_column.GetPrecisionAndScale(sql);
  if(p_column.m_nullable)
  {
    sql += _T(" DROP NOT NULL");
  }
  else
  {
    sql += _T(" SET NOT NULL");
  }
  return sql;
}

XString 
SQLInfoFirebird::GetCATALOGColumnRename(XString /*p_schema*/,XString p_tablename,XString p_columnname,XString p_newname,XString /*p_datatype*/) const
{
  XString sql(_T("ALTER  TABLE ") + QIQ(p_tablename)  + _T("\n")
              _T("      RENAME ") + QIQ(p_columnname) + _T(" TO ") + QIQ(p_newname) + "\n");
  return sql;
}


XString 
SQLInfoFirebird::GetCATALOGColumnDrop(XString /*p_schema*/,XString p_tablename,XString p_columnname) const
{
  XString sql(_T("ALTER TABLE ")  + QIQ(p_tablename) + _T("\n")
              _T(" DROP COLUMN ") + QIQ(p_columnname));
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL INDICES FUNCTIONS

// All index functions
XString
SQLInfoFirebird::GetCATALOGIndexExists(XString /*p_schema*/,XString /*p_tablename*/,XString p_indexname,bool p_quoted /*= false*/) const
{
  IdentifierCorrect(p_indexname);
  XString sql = _T("SELECT COUNT(*)\n")
                _T("  FROM rdb$indices\n")
                _T(" WHERE rdb$index_name = '") + p_indexname + _T("'");
  return sql;
}

XString
SQLInfoFirebird::GetCATALOGIndexList(XString& p_schema,XString& p_tablename,bool p_quoted /*= false*/) const
{
  XString indexname;
  return GetCATALOGIndexAttributes(p_schema,p_tablename,indexname,p_quoted);
}

XString
SQLInfoFirebird::GetCATALOGIndexAttributes(XString& p_schema,XString& p_tablename,XString& p_indexname,bool p_quoted /*= false*/) const
{
  // No table statistics (YET)
  if(p_indexname.Compare(_T("0")) == 0)
  {
    return _T("");
  }
  XString query = _T("SELECT TRIM(dbs.mon$database_name) as index_catalog\n"
                     "      ,TRIM(tab.rdb$owner_name)    as index_schema\n"
                     "      ,TRIM(idx.rdb$relation_name) as index_table\n"
                     "      ,CASE idx.rdb$unique_flag\n"
                     "            WHEN 0 THEN 1\n"
                     "            WHEN 1 THEN 0\n"
                     "            ELSE 1 END             as index_nonunique\n"
                     "      ,CAST('' as varchar(31))     as drop_qualifier\n"
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
                     "      ,mon$database       dbs\n"
                     " WHERE idx.rdb$index_name    = col.rdb$index_name\n"
                     "   AND idx.rdb$relation_name = tab.rdb$relation_name\n"
                     "   AND idx.rdb$system_flag   = 0\n"
                     "   AND NOT EXISTS\n"
                     "     ( SELECT 1\n"
                     "         FROM rdb$relation_constraints rc\n"
                     "        WHERE col.rdb$index_name = rc.rdb$index_name\n"
                     "          AND rdb$constraint_type IN ('PRIMARY KEY','FOREIGN KEY'))\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    query += _T("   AND tab.rdb$owner_name = ?\n");
  }
  if(!p_tablename.IsEmpty())
  {
    IdentifierCorrect(p_tablename);
    query += _T("   AND tab.rdb$relation_name = ?\n");
  }
  if(!p_indexname.IsEmpty())
  {
    IdentifierCorrect(p_indexname);
    query += _T("   AND idx.rdb$index_name = ?\n");
  }
  query += _T(" ORDER BY 6"); // Index name
  return query;
}

XString
SQLInfoFirebird::GetCATALOGIndexCreate(MIndicesMap& p_indices,bool /*p_duplicateNulls /*= false*/) const
{
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
      if(index.m_ascending != _T("A"))
      {
        query += _T("DESC ");
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
    if(index.m_columnName.Left(1) == _T("("))
    {
      query.TrimRight(_T("("));
      query += _T(" COMPUTED BY ");
      query += QIQ(index.m_columnName);
      query.TrimRight(_T(")"));
    }
    else if(!index.m_filter.IsEmpty())
    {
      query += index.m_filter;
    }
    else
    {
      query += QIQ(index.m_columnName);
    }
  }
  query += ")";
  return query;
}

XString
SQLInfoFirebird::GetCATALOGIndexDrop(XString /*p_schema*/,XString /*p_tablename*/,XString p_indexname) const
{
  XString sql = _T("DROP INDEX ") + QIQ(p_indexname);
  return sql;
}

// Get extra filter expression for an index column
XString
SQLInfoFirebird::GetCATALOGIndexFilter(MetaIndex& /*p_index*/) const
{
  return _T("");
}

//////////////////////////////////////////////////////////////////////////
// ALL PRIMARY KEY FUNCTIONS

XString
SQLInfoFirebird::GetCATALOGPrimaryExists(XString p_schema,XString p_tablename,bool p_quoted /*= false*/) const
{
  IdentifierCorrect(p_tablename);
  XString query = _T("SELECT COUNT(*)\n"
                     "  FROM rdb$relation_constraints\n"
                     " WHERE rdb$relation_name   = '") + p_tablename + _T("'\n")
                  _T("   AND rdb$constraint_type = 'PRIMARY KEY'");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    query += _T("   AND rdb$owner_name = '") + p_schema + _T("'");
  }
  return query;
}

XString
SQLInfoFirebird::GetCATALOGPrimaryAttributes(XString& p_schema,XString& p_tablename,bool p_quoted /*= false*/) const
{
  IdentifierCorrect(p_tablename);
  XString sql = _T("SELECT TRIM(dbs.mon$database_name)   as catalog_name\n"
                   "      ,TRIM(rel.rdb$owner_name)      as schema_name\n"
                   "      ,trim(con.rdb$relation_name)   as table_name\n"
                   "      ,trim(ind.rdb$field_name)      as column_name\n"
                   "      ,ind.rdb$field_position + 1    as col_position\n"
                   "      ,trim(con.rdb$constraint_name) as col_constraint\n"
                   "      ,trim(con.rdb$index_name)      as index_name\n"
                   "      ,con.rdb$deferrable\n"
                   "      ,con.rdb$initially_deferred\n"
                   "  FROM rdb$relations            rel\n"
                   "      ,rdb$relation_constraints con\n"
                   "      ,rdb$index_segments       ind\n"
                   "      ,mon$database             dbs\n"
                   " WHERE rel.rdb$relation_name   = con.rdb$relation_name\n"
                   "   AND ind.rdb$index_name      = con.rdb$index_name\n"
                   "   AND con.rdb$constraint_type = 'PRIMARY KEY'\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND rel.rdb$owner_name      = ?");
  }
  sql += _T("   AND rel.rdb$relation_name   = ?");
  return sql;
}

XString
SQLInfoFirebird::GetCATALOGPrimaryCreate(MPrimaryMap& p_primaries) const
{
  XString query(_T("ALTER TABLE "));

  for(auto& prim : p_primaries)
  {
    if(prim.m_columnPosition == 1)
    {
      query += QIQ(prim.m_table) + _T("\n");
      query += _T("  ADD CONSTRAINT ") + QIQ(prim.m_constraintName) + _T("\n");
      query += _T("      PRIMARY KEY (");
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

XString
SQLInfoFirebird::GetCATALOGPrimaryDrop(XString /*p_schema*/,XString p_tablename,XString p_constraintname) const
{
  XString sql(_T("ALTER TABLE ")      + QIQ(p_tablename) + _T("\n")
              _T(" DROP CONSTRAINT ") + QIQ(p_constraintname));
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL FOREIGN KEY FUNCTIONS

XString
SQLInfoFirebird::GetCATALOGForeignExists(XString p_schema,XString p_tablename,XString p_constraintname,bool p_quoted /*= false*/) const
{
  IdentifierCorrect(p_tablename);
  IdentifierCorrect(p_constraintname);

  XString sql(_T("SELECT COUNT(*)\n"
                 "  FROM rdb$relations rel\n"
                 "   AND rdb$relation_constraints con\n"
                 " WHERE con.rdb$constraint_type = 'FOREIGN KEY'"));
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND rel.owner_name          = '") + p_schema + _T("'\n");
  }
  sql += _T("   AND con.rdb$relation_name   = '") + p_tablename + _T("'\n");
  sql += _T("   AND con.rdb$constraint_name = '") + p_constraintname + _T("'");

  return sql;
}

XString
SQLInfoFirebird::GetCATALOGForeignList(XString& p_schema,XString& p_tablename,bool p_quoted /*= false*/) const
{
  XString constraint;
  return GetCATALOGForeignAttributes(p_schema,p_tablename,constraint,p_quoted);
}

XString
SQLInfoFirebird::GetCATALOGForeignAttributes(XString& p_schema
                                            ,XString& p_tablename
                                            ,XString& p_constraint
                                            ,bool     p_referenced /*=false*/
                                            ,bool     p_quoted     /*= false*/) const
{
  XString query(_T("SELECT (SELECT Trim(mon$database_name) FROM mon$database) AS primary_key_catalog\n"
                   "      ,TRIM(pkrel.rdb$owner_name)      AS primary_key_schema\n"
                   "      ,TRIM(pkidx.rdb$relation_name)   AS primary_key_table\n"
                   "      ,TRIM(pkseg.rdb$field_name)      AS primary_key_column_name\n"
                   "      ,(SELECT Trim(mon$database_name) FROM mon$database) AS foreign_key_catalog\n"
                   "      ,TRIM(fkrel.rdb$owner_name)      AS foreign_key_schema\n"
                   "      ,TRIM(fkrel.rdb$relation_name)   AS foreign_key_table\n"
                   "      ,TRIM(fkseg.rdb$field_name)      AS foreign_key_column_name\n"
                   "      ,fkseg.rdb$field_position + 1    AS key_sequence\n"
                   "      ,CASE fkref.rdb$update_rule      WHEN 'RESTRICT'     THEN 1\n"
                   "                                       WHEN 'CASCADE'      THEN 0\n"
                   "                                       WHEN 'SET NULL'     THEN 2\n"
                   "                                       WHEN 'SET DEFAULT'  THEN 4\n"
                   "                                       WHEN 'NO ACTION'    THEN 3\n"
                   "                                       ELSE 0\n"
                   "       END AS update_rule\n"
                   "      ,CASE fkref.rdb$delete_Rule      WHEN 'RESTRICT'     THEN 1\n"
                   "                                       WHEN 'CASCADE'      THEN 0\n"
                   "                                       WHEN 'SET NULL'     THEN 2\n"
                   "                                       WHEN 'SET DEFAULT'  THEN 4\n"
                   "                                       WHEN 'NO ACTION'    THEN 3\n"
                   "                                       ELSE 0\n"
                   "       END AS delete_rule\n"
                   "      ,TRIM(fkcon.rdb$constraint_name) AS foreign_key_constraint\n"
                   "      ,TRIM (fkref.rdb$const_name_uq)  AS primary_key_constraint\n"
                   "      ,CASE fkcon.rdb$deferrable         WHEN 'YES'  THEN 1 ELSE 0 END as deferrable\n"
                   "      ,CASE fkref.rdb$match_option       WHEN 'FULL' THEN 1 ELSE 0 END as match_option\n"
                   "      ,CASE fkcon.rdb$initially_deferred WHEN 'YES'  THEN 1 ELSE 0 END as initially_deferred\n"
                   "      ,1                               AS enabled\n"
                   "  FROM rdb$relations fkrel\n"
                   "       JOIN rdb$relation_constraints fkcon ON fkrel.rdb$relation_name   = fkcon.rdb$relation_name\n"
                   "       JOIN rdb$ref_constraints      fkref ON fkcon.rdb$constraint_name = fkref.rdb$constraint_name\n"
                   "       JOIN rdb$indices              fkidx ON fkref.rdb$constraint_name = fkidx.rdb$index_name\n"
                   "       JOIN rdb$index_segments       fkseg ON fkidx.rdb$index_name      = fkseg.rdb$index_name\n"
                   "       JOIN rdb$indices              pkidx ON fkref.rdb$const_name_uq   = pkidx.rdb$index_name\n"
                   "       JOIN rdb$index_segments       pkseg ON pkidx.rdb$index_name      = pkseg.rdb$index_name\n"
                   "       JOIN rdb$relations            pkrel ON pkidx.rdb$relation_name   = pkrel.rdb$relation_name\n"
                   "   AND fkcon.rdb$constraint_type = 'FOREIGN KEY'\n"));
  
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    if(p_referenced)
    {
      query += _T("   AND pkrel.rdb$owner_name = ?\n");
    }
    else
    {
      query += _T("   AND fkrel.rdb$owner_name = ?\n");
    }
  }
  if(!p_tablename.IsEmpty())
  {
    IdentifierCorrect(p_tablename);
    if(p_referenced)
    {
      query += _T("   AND pkrel.rdb$relation_name = ?\n");
    }
    else
    {
      query += _T("   AND fkrel.rdb$relation_name = ?\n");
    }
  }
  if(!p_constraint.IsEmpty())
  {
    IdentifierCorrect(p_constraint);
    if(p_referenced)
    {
      query += _T("   AND fkref.rdb$const_name_uq = ?\n");
    }
    else
    {
      query += _T("   AND fkcon.rdb$constraint_name = ?\n");
    }
  }
  // Add ordering up to column number
  query += _T(" ORDER BY 1,2,3,4,5,6,7,8,9");

  return query;
}

XString
SQLInfoFirebird::GetCATALOGForeignCreate(MForeignMap& p_foreigns) const
{
  // Get first record
  MetaForeign& foreign = p_foreigns.front();

  // Construct the correct tablename
  XString table(foreign.m_fkTableName);
  XString primary(foreign.m_pkTableName);
  table   = QIQ(table);
  primary = QIQ(primary);

  // The base foreign key command
  XString query = _T("ALTER TABLE ") + table + _T("\n")
                  _T("  ADD CONSTRAINT ") + QIQ(foreign.m_foreignConstraint) + _T("\n")
                  _T("      FOREIGN KEY (");

  // Add the foreign key columns
  bool extra = false;
  for(const auto& key : p_foreigns)
  {
    if(extra) query += _T(",");
    query += QIQ(key.m_fkColumnName);
    extra  = true;
  }

  // Add references primary table
  query += _T(")\n      REFERENCES ") + primary + _T("(");

  // Add the primary key columns
  extra = false;
  for(const auto& key : p_foreigns)
  {
    if(extra) query += _T(",");
    query += QIQ(key.m_pkColumnName);
    extra  = true;
  }
  query += _T(")");

  // Add all relevant options
  switch(foreign.m_updateRule)
  {
    case SQL_CASCADE:     query += _T("\n      ON UPDATE CASCADE");     break;
    case SQL_SET_NULL:    query += _T("\n      ON UPDATE SET NULL");    break;
    case SQL_SET_DEFAULT: query += _T("\n      ON UPDATE SET DEFAULT"); break;
    case SQL_NO_ACTION:   query += _T("\n      ON UPDATE NO ACTION");   break;
    default:              // In essence: ON UPDATE RESTRICT, but that's already the default
                          break;
  }
  switch(foreign.m_deleteRule)
  {
    case SQL_CASCADE:     query += _T("\n      ON DELETE CASCADE");     break;
    case SQL_SET_NULL:    query += _T("\n      ON DELETE SET NULL");    break;
    case SQL_SET_DEFAULT: query += _T("\n      ON DELETE SET DEFAULT"); break;
    case SQL_NO_ACTION:   query += _T("\n      ON DELETE NO ACTION");   break;
    default:              // In essence: ON DELETE RESTRICT, but that's already the default
                          break;
  }
  return query;
}

XString
SQLInfoFirebird::GetCATALOGForeignAlter(MForeignMap& /*p_original*/, MForeignMap& /*p_requested*/) const
{
  // Firebird cannot alter a foreign-key constraint.
  // You must drop and then re-create your foreign key constraint
  // So return an empty string to signal this!
  return XString();
}

XString
SQLInfoFirebird::GetCATALOGForeignDrop(XString /*p_schema*/,XString p_tablename,XString p_constraintname) const
{
  XString sql(_T("ALTER TABLE ")      + QIQ(p_tablename) + _T("\n")
              _T(" DROP CONSTRAINT ") + QIQ(p_constraintname));
  return sql;
}

//////////////////////////
// All default constraints
XString
SQLInfoFirebird::GetCATALOGDefaultExists(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_column*/) const
{
  return _T("");
}

XString
SQLInfoFirebird::GetCATALOGDefaultList(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  return _T("");
}

XString
SQLInfoFirebird::GetCATALOGDefaultAttributes(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_column*/) const
{
  return _T("");
}

XString
SQLInfoFirebird::GetCATALOGDefaultCreate(XString /*p_schema*/,XString /*p_tablename*/,XString /*p_constraint*/,XString /*p_column*/,XString /*p_code*/) const
{
  return _T("");
}

XString
SQLInfoFirebird::GetCATALOGDefaultDrop(XString /*p_schema*/,XString /*p_tablename*/,XString /*p_constraint*/) const
{
  return _T("");
}

/////////////////////////
// All check constraints

XString
SQLInfoFirebird::GetCATALOGCheckExists(XString  p_schema,XString p_tablename,XString p_constraint,bool p_quoted /*= false*/) const
{
  IdentifierCorrect(p_tablename);
  IdentifierCorrect(p_constraint);

  XString sql = _T("SELECT COUNT(DISTINCT\n")
                _T("       trim(rc.rdb$constraint_name)) as number\n")
                _T("  FROM rdb$relations rl\n")
                _T("       join rdb$relation_constraints rc ON rc.rdb$relation_name   = rl.rdb$relation_name\n")
                _T("       join rdb$check_constraints    cc on cc.rdb$constraint_name = rc.rdb$constraint_name\n")
                _T("       join rdb$triggers             tr on tr.rdb$trigger_name    = cc.rdb$trigger_name\n")
                _T(" WHERE rc.rdb$constraint_type = 'CHECK'\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND rl.rdb$owner_name      = ?\n");
  }
  sql += _T("   AND rl.rdb$relation_name   = ?")
         _T("   AND rc.rdb$constraint_name = ?")
         _T(" GROUP BY 1,2,3,4,5");
  return sql;
}

XString
SQLInfoFirebird::GetCATALOGCheckList(XString  p_schema,XString p_tablename,bool p_quoted /*= false*/) const
{
  XString constraint;
  return GetCATALOGCheckAttributes(p_schema,p_tablename,constraint,p_quoted);
}

XString
SQLInfoFirebird::GetCATALOGCheckAttributes(XString  p_schema,XString p_tablename,XString p_constraint,bool p_quoted /*= false*/) const
{
  XString sql = _T("SELECT DISTINCT\n")
                _T("       (SELECT trim(mon$database_name) from mon$database) as catalog_name\n")
                _T("      ,trim(rl.rdb$owner_name)      as schema_name\n")
                _T("      ,trim(rc.rdb$relation_name)   as table_name\n")
                _T("      ,trim(rc.rdb$constraint_name) as constraint_name\n")
                _T("      ,tr.rdb$trigger_source        as source\n")
                _T("  FROM rdb$relations rl\n")
                _T("       JOIN rdb$relation_constraints rc ON rc.rdb$relation_name   = rl.rdb$relation_name\n")
                _T("       JOIN rdb$check_constraints    cc ON cc.rdb$constraint_name = rc.rdb$constraint_name\n")
                _T("       JOIN rdb$triggers             tr ON tr.rdb$trigger_name    = cc.rdb$trigger_name\n")
                _T(" WHERE rc.rdb$constraint_type = 'CHECK'\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND rl.rdb$owner_name = ?\n");
  }
  if(!p_tablename.IsEmpty())
  {
    IdentifierCorrect(p_tablename);
    sql += _T("   AND rl.rdb$relation_name = ?");
  }
  if(!p_constraint.IsEmpty())
  {
    IdentifierCorrect(p_constraint);
    sql += _T("   AND rc.rdb$constraint_name = ?");
  }
  sql += _T(" GROUP BY 1,2,3,4,5");
   
  return sql;
}

XString
SQLInfoFirebird::GetCATALOGCheckCreate(XString /*p_schema*/,XString p_tablename,XString p_constraint,XString p_condition) const
{
  // In Firebird the 'condition' already contains the keyword 'CHECK'
  XString sql  = _T("ALTER TABLE ") + QIQ(p_tablename) + _T("\n");
          sql += _T("  ADD CONSTRAINT ") + QIQ(p_constraint) + _T(" ") + p_condition;
  return sql;
}

XString
SQLInfoFirebird::GetCATALOGCheckDrop(XString /*p_schema*/,XString p_tablename,XString p_constraint) const
{
  XString sql  = _T("ALTER TABLE ") + QIQ(p_tablename) + _T("\n");
          sql += _T(" DROP CONSTRAINT ") + QIQ(p_constraint);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL TRIGGER FUNCTIONS

XString
SQLInfoFirebird::GetCATALOGTriggerExists(XString p_schema,XString p_tablename,XString p_triggername,bool p_quoted /*= false*/) const
{
  IdentifierCorrect(p_tablename);
  IdentifierCorrect(p_triggername);

  XString sql(_T("SELECT COUNT(*)\n"
                 "  FROM rdb$relations     rel\n"
                 "       JOIN rdb$triggers trg ON rel.rdb$relation_name = trg.rdb$relation_name\n"
                 " WHERE trg.rdb$system_flag  = 0\n"));
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql.AppendFormat(_T("   AND rel.rdb$owner_name = '%s'\n"),p_schema.GetString());
  }
  sql.AppendFormat(_T("   AND rdb$relation_name = '%s'\n"),p_tablename.GetString());
  sql.AppendFormat(_T("   AND rdb$trigger_name  = '%s'"),p_triggername.GetString());
  return sql;
}

XString
SQLInfoFirebird::GetCATALOGTriggerList(XString& p_schema,XString& p_tablename,bool p_quoted /*= false*/) const
{
  XString triggername;
  return GetCATALOGTriggerAttributes(p_schema,p_tablename,triggername,p_quoted);
}

XString
SQLInfoFirebird::GetCATALOGTriggerAttributes(XString& p_schema,XString& p_tablename,XString& p_triggername,bool p_quoted /*= false*/) const
{
  XString sql(_T("SELECT TRIM(dbs.mon$database_name)  AS catalog_name\n"
                 "      ,TRIM(rel.rdb$owner_name)     AS schema_name\n"
                 "      ,TRIM(rel.rdb$relation_name)  AS table_name\n"
                 "      ,TRIM(trg.rdb$trigger_name)   AS trigger_name\n"
                 "      ,TRIM(trg.rdb$description)    AS description\n"
                 "      ,trg.rdb$trigger_sequence\n"
                 "      ,CASE trg.rdb$trigger_type\n"
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
                 "      ,CASE trg.rdb$trigger_type\n"
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
                 "      ,CASE trg.rdb$trigger_type\n"
                 "            WHEN    3 THEN true\n"
                 "            WHEN    4 THEN true\n"
                 "            WHEN   17 THEN true\n"
                 "            WHEN   18 THEN true\n"
                 "            WHEN  113 THEN true\n"
                 "            WHEN  114 THEN true\n"
                 "                      ELSE false\n"
                 "       END AS trigger_update\n"
                 "      ,CASE trg.rdb$trigger_type\n"
                 "            WHEN    5 THEN true\n"
                 "            WHEN    6 THEN true\n"
                 "            WHEN   25 THEN true\n"
                 "            WHEN   26 THEN true\n"
                 "            WHEN  113 THEN true\n"
                 "            WHEN  114 THEN true\n"
                 "                      ELSE false\n"
                 "       END AS trigger_delete\n"
                 "      ,false as trigger_select\n"
                 "      ,CASE trg.rdb$trigger_type\n"
                 "            WHEN 8192 THEN true\n"
                 "            WHEN 8993 THEN true\n"
                 "                      ELSE false\n"
                 "       END AS trigger_session\n"
                 "      ,CASE trg.rdb$trigger_type\n"
                 "            WHEN 8194 THEN true\n"
                 "            WHEN 8995 THEN true\n"
                 "                      ELSE false\n"
                 "       END AS trigger_transaction\n"
                 "      ,CASE trg.rdb$trigger_type\n"
                 "            WHEN 8196 THEN true\n"
                 "                      ELSE false\n"
                 "       END AS trigger_rollback\n"
                 "      ,'' AS trigger_referencing\n"
                 "      ,CASE trg.rdb$trigger_inactive\n"
                 "            WHEN 1 THEN false\n"
                 "                   ELSE true\n"
                 "       END AS trigger_enabled\n"
                 "      ,'CREATE OR ALTER TRIGGER \"' || TRIM(rdb$trigger_name) || '\"' || ASCII_CHAR(10) ||\n"
                 "       CASE trg.rdb$trigger_inactive\n"
                 "          WHEN 0 THEN 'ACTIVE'\n"
                 "          WHEN 1 THEN 'INACTIVE'\n"
                 "                 ELSE ''\n"
                 "        END || ASCII_CHAR(10) ||\n"
                 "        CASE \n"
                 "          -- Table-level triggers\n"
                 "          WHEN trg.rdb$trigger_type BETWEEN 1 AND 114 THEN\n"
                 "            CASE trg.rdb$trigger_type\n"
                 "              WHEN   1 THEN 'BEFORE INSERT'\n"
                 "              WHEN   2 THEN 'AFTER INSERT'\n"
                 "              WHEN   3 THEN 'BEFORE UPDATE'\n"
                 "              WHEN   4 THEN 'AFTER UPDATE'\n"
                 "              WHEN   5 THEN 'BEFORE DELETE'\n"
                 "              WHEN   6 THEN 'AFTER DELETE'\n"
                 "              WHEN  17 THEN 'BEFORE INSERT OR UPDATE'\n"
                 "              WHEN  18 THEN 'AFTER INSERT OR UPDATE'\n"
                 "              WHEN  25 THEN 'BEFORE INSERT OR DELETE'\n"
                 "              WHEN  26 THEN 'AFTER INSERT OR DELETE'\n"
                 "              WHEN  27 THEN 'BEFORE UPDATE OR DELETE'\n"
                 "              WHEN  28 THEN 'AFTER UPDATE OR DELETE'\n"
                 "              WHEN 113 THEN 'BEFORE INSERT OR UPDATE OR DELETE'\n"
                 "              WHEN 114 THEN 'AFTER INSERT OR UPDATE OR DELETE'\n"
                 "            END || ' ON \"' || TRIM(rel.rdb$relation_name) || '\"'\n"
                 "          -- Database-level triggers\n"
                 "          WHEN trg.rdb$trigger_type BETWEEN 8192 AND 8196 THEN\n"
                 "            CASE trg.rdb$trigger_type\n"
                 "              WHEN 8192 THEN 'ON CONNECT'\n"
                 "              WHEN 8193 THEN 'ON DISCONNECT'\n"
                 "              WHEN 8194 THEN 'ON TRANSACTION START'\n"
                 "              WHEN 8195 THEN 'ON TRANSACTION COMMIT'\n"
                 "              WHEN 8196 THEN 'ON TRANSACTION ROLLBACK'\n"
                 "            END\n"
                 "          -- DDL triggers\n"
                 "          WHEN trg.rdb$trigger_type > 0x4000 THEN\n"
                 "            CASE\n"
                 "              WHEN MOD(trg.rdb$trigger_type,1) = 1 THEN 'AFTER '\n"
                 "              ELSE 'BEFORE '\n"
                 "            END ||\n"
                 "            CASE (trg.rdb$trigger_type - 0x4000)\n"
                 "              WHEN 0x2 THEN 'CREATE TABLE'\n"
                 "              WHEN 0x4 THEN 'ALTER TABLE'\n"
                 "              WHEN 0x8 THEN 'DROP TABLE'\n"
                 "              WHEN 0x10 THEN 'CREATE PROCEDURE'\n"
                 "              WHEN 0x20 THEN 'ALTER PROCEDURE'\n"
                 "              WHEN 0x40 THEN 'DROP PROCEDURE'\n"
                 "              WHEN 0x80 THEN 'CREATE FUNCTION'\n"
                 "              WHEN 0x100 THEN 'ALTER FUNCTION'\n"
                 "              WHEN 0x200 THEN 'DROP FUNCTION'\n"
                 "              WHEN 0x400 THEN 'CREATE TRIGGER'\n"
                 "              WHEN 0x800 THEN 'ALTER TRIGGER'\n"
                 "              WHEN 0x1000 THEN 'DROP TRIGGER'\n"
                 "              WHEN 0x10000 THEN 'CREATE EXCEPTION'\n"
                 "              WHEN 0x20000 THEN 'ALTER EXCEPTION'\n"
                 "              WHEN 0x40000 THEN 'DROP EXCEPTION'\n"
                 "              WHEN 0x80000 THEN 'CREATE VIEW'\n"
                 "              WHEN 0x100000 THEN 'ALTER VIEW'\n"
                 "              WHEN 0x200000 THEN 'DROP VIEW'\n"
                 "              WHEN 0x400000 THEN 'CREATE DOMAIN'\n"
                 "              WHEN 0x800000 THEN 'ALTER DOMAIN'\n"
                 "              WHEN 0x1000000 THEN 'DROP DOMAIN'\n"
                 "              WHEN 0x2000000 THEN 'CREATE ROLE'\n"
                 "              WHEN 0x4000000 THEN 'ALTER ROLE'\n"
                 "              WHEN 0x8000000 THEN 'DROP ROLE'\n"
                 "              WHEN 0x10000000 THEN 'CREATE INDEX'\n"
                 "              WHEN 0x20000000 THEN 'ALTER INDEX'\n"
                 "              WHEN 0x40000000 THEN 'DROP INDEX'\n"
                 "              WHEN 0x80000000 THEN 'CREATE SEQUENCE'\n"
                 "              WHEN 0x100000000 THEN 'ALTER SEQUENCE'\n"
                 "              WHEN 0x200000000 THEN 'DROP SEQUENCE'\n"
                 "              WHEN 0x400000000 THEN 'CREATE USER'\n"
                 "              WHEN 0x800000000 THEN 'ALTER USER'\n"
                 "              WHEN 0x1000000000 THEN 'DROP USER'\n"
                 "              WHEN 0x2000000000 THEN 'CREATE COLLATION'\n"
                 "              WHEN 0x4000000000 THEN 'DROP COLLATION'\n"
                 "              WHEN 0x8000000000 THEN 'ALTER CHARACTER SET'\n"
                 "              WHEN 0x10000000000 THEN 'CREATE PACKAGE'\n"
                 "              WHEN 0x20000000000 THEN 'ALTER PACKAGE'\n"
                 "              WHEN 0x40000000000 THEN 'DROP PACKAGE'\n"
                 "              WHEN 0x80000000000 THEN 'CREATE PACKAGE BODY'\n"
                 "              WHEN 0x100000000000 THEN 'DROP PACKAGE BODY'\n"
                 "              WHEN 0x200000000000 THEN 'CREATE MAPPING'\n"
                 "              WHEN 0x400000000000 THEN 'ALTER MAPPING'\n"
                 "              WHEN 0x800000000000 THEN 'DROP MAPPING'\n"
                 "            END\n"
                 "          ELSE\n"
                 "            CASE trg.rdb$trigger_type\n"
                 "	             WHEN 0x7FFFFFFFFFFFDFFE THEN 'BEFORE ANY DDL STATEMENT'\n"
                 "	             WHEN 0x7FFFFFFFFFFFDFFF THEN 'AFTER ANY DDL STATEMENT'\n"
                 "                                         ELSE 'UNKNOWN DDL STATMENT'\n"
                 "	        END\n"
                 "       END || ASCII_CHAR(10) ||\n"
                 "       CASE\n"
                 "         WHEN trg.rdb$trigger_sequence IS NOT NULL AND trg.rdb$trigger_sequence > 0\n"
                 "              THEN 'POSITION ' || trg.rdb$trigger_sequence || ASCII_CHAR(10)\n"
                 "              ELSE ''\n"
                 "       END ||\n"
                 "       trg.rdb$trigger_source AS recreate_statement\n"
                 "  FROM rdb$relations rel\n"
                 "      ,rdb$triggers  trg\n"
                 "      ,mon$database  dbs\n"
                 " WHERE rel.rdb$relation_name = trg.rdb$relation_name\n"
                 "   AND (trg.rdb$system_flag  = 0 OR\n" 
                 "        trg.rdb$system_flag IS NULL)\n"));
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND rel.rdb$owner_name = ?\n");
  }
  // Add tablename filter
  if(!p_tablename.IsEmpty())
  {
    IdentifierCorrect(p_tablename);
    if(p_tablename.Find('%') >= 0)
    {
      sql += _T("   AND rel.rdb$relation_name LIKE ?\n");
    }
    else
    {
      sql += _T("   AND rel.rdb$relation_name = ?\n");
    }
  }

  // Add trigger name filter
  if(!p_triggername.IsEmpty())
  {
    IdentifierCorrect(p_triggername);
    if(p_triggername.Find('%') >= 0)
    {
      sql += _T("   AND trg.rdb$trigger_name LIKE ?\n");
    }
    else
    {
      sql += _T("   AND trg.rdb$trigger_name = ?\n");
    }
  }
  sql += _T(" ORDER BY trg.rdb$trigger_sequence");
  return sql;
}

XString
SQLInfoFirebird::GetCATALOGTriggerCreate(MetaTrigger& p_trigger) const
{
  // Now very simple as TriggerAttributes recreate the complete source!
  return p_trigger.m_source;
}

XString
SQLInfoFirebird::GetCATALOGTriggerDrop(XString /*p_schema*/, XString /*p_tablename*/, XString p_triggername) const
{
  XString sql(_T("DROP TRIGGER ") + QIQ(p_triggername));
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL SEQUENCE FUNCTIONS

XString
SQLInfoFirebird::GetCATALOGSequenceExists(XString p_schema, XString p_sequence,bool p_quoted /*= false*/) const
{
  IdentifierCorrect(p_sequence);

  XString sql = _T("SELECT COUNT(*)\n"
                   "  FROM rdb$generators\n"
                   " WHERE rdb$system_flag    = 0\n"
                   "   AND rdb$generator_name = '") + p_sequence + _T("'");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("\n   AND rdb$owner_name = '") + p_schema + _T("'");
  }
  return sql;
}

XString
SQLInfoFirebird::GetCATALOGSequenceList(XString& p_schema,XString& p_pattern,bool p_quoted /*= false*/) const
{
  XString sql = _T("SELECT cast('' as varchar(31))  as catalog_name\n"
                   "      ,trim(rdb$owner_name)     as schema_name\n"
                   "      ,trim(rdb$generator_name) as sequence_name\n" 
                   "      ,rdb$initial_value        as current_value\n"
                   "      ,0                        as minimal_value\n"
                   "      ,rdb$generator_increment  as increment\n"
                   "      ,0  as cache\n"
                   "      ,0  as cycle\n"
                   "      ,0  as ordering\n"
                   "      ,rdb$description as remarks\n"
                   "  FROM rdb$generators\n"
                   " WHERE rdb$system_flag    = 0\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND rdb$owner_name = ?\n");
  }
  if(!p_pattern.IsEmpty())
  {
    IdentifierCorrect(p_pattern);
    p_pattern = _T("%") + p_pattern + _T("%");
    sql += _T("   AND rdb$generator_name LIKE ?");
  }
  return sql;
}

XString
SQLInfoFirebird::GetCATALOGSequenceAttributes(XString& p_schema,XString& p_sequence,bool p_quoted /*= false*/) const
{
  IdentifierCorrect(p_sequence);

  XString sql = _T("SELECT trim(dbs.mon$database_name)  as catalog_name\n"
                   "      ,trim(gen.rdb$owner_name)     as schema_name\n"
                   "      ,trim(gen.rdb$generator_name) as sequence_name\n" 
                   "      ,gen.rdb$initial_value        as current_value\n"
                   "      ,0                            as minimal_value\n"
                   "      ,gen.rdb$generator_increment  as increment\n"
                   "      ,0  as cache\n"
                   "      ,0  as cycle\n"
                   "      ,0  as ordering\n"
                   "      ,gen.rdb$description as remarks\n"
                   "  FROM rdb$generators gen\n"
                   "      ,mon$database   dbs\n"
                   " WHERE gen.rdb$system_flag = 0\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND gen.rdb$owner_name = ?\n");
  }
  sql += _T("   AND gen.rdb$generator_name = ?");
  return sql;
}

XString
SQLInfoFirebird::GetCATALOGSequenceCreate(MetaSequence& p_sequence) const
{
  XString sql;
  sql.Format(_T("CREATE SEQUENCE %s START WITH %-12.0f")
            ,QIQ(p_sequence.m_sequenceName).GetString()
            ,p_sequence.m_currentValue);
  return sql;
}

XString
SQLInfoFirebird::GetCATALOGSequenceDrop(XString /*p_schema*/, XString p_sequence) const
{
  XString sql(_T("DROP SEQUENCE ") + QIQ(p_sequence));
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL VIEW FUNCTIONS

XString 
SQLInfoFirebird::GetCATALOGViewExists(XString& p_schema,XString& p_viewname,bool p_quoted /*= false*/) const
{
  IdentifierCorrect(p_viewname);

  XString sql = _T("SELECT count(*)\n"
                   "  FROM rdb$relations\n"
                   " WHERE rdb$relation_type = 1\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND rdb$owner_name = ?\n");
  }
  sql += _T("   AND rdb$relation_name = ?");
  return sql;
}

XString 
SQLInfoFirebird::GetCATALOGViewList(XString& p_schema,XString& p_pattern,bool p_quoted /*= false*/) const
{
  return GetCATALOGViewAttributes(p_schema,p_pattern,p_quoted);
}

XString
SQLInfoFirebird::GetCATALOGViewText(XString& p_schema,XString& p_viewname,bool p_quoted /*= false*/) const
{
  XString viewname(p_viewname);
  if(p_quoted)
  {
    viewname = QIQ(viewname);
  }
  else
  {
    IdentifierCorrect(p_viewname);
  }
  XString sql = _T("SELECT 'CREATE VIEW ") + viewname + _T(" AS\n' ||")
                _T("       rdb$view_source\n")
                _T("  FROM rdb$relations\n")
                _T(" WHERE rdb$relation_name = '") + p_viewname + _T("'");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("\n   AND rdb$owner_name = '") + p_schema + _T("'");
  }
  return sql;
}

XString
SQLInfoFirebird::GetCATALOGViewAttributes(XString& p_schema,XString& p_viewname,bool p_quoted /*= false*/) const
{
  XString sql = _T("SELECT trim(dbs.mon$database_name)  AS table_catalog\n"
                   "      ,trim(rel.rdb$owner_name)     AS table_schema\n"
                   "      ,trim(rel.rdb$relation_name)  AS table_name\n"
                   "      ,CAST('VIEW' as varchar(31))  AS table_type\n"
                   "      ,rel.rdb$description   AS remarks\n"
                   "      ,trim(rel.rdb$owner_name) || '.' || trim(rel.rdb$relation_name) AS full_name\n"
                   "      ,cast('' as varchar(31))      as storage_space\n"
                   "      ,0                            as temporary\n"
                   "  FROM rdb$relations rel\n"
                   "      ,mon$database  dbs\n"
                   " WHERE rel.rdb$relation_type = 1\n"
                   "   AND rel.rdb$system_flag   = 0\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND rel.rdb$owner_name = ?\n");
  }
  if(!p_viewname.IsEmpty())
  {
    IdentifierCorrect(p_viewname);
    sql += _T("\n AND rel.rdb$relation_name ");
    sql += (p_viewname.Find(_T("%")) >= 0) ? _T("LIKE") : _T("=");
    sql += " ?";
  }
  return sql;
}

XString 
SQLInfoFirebird::GetCATALOGViewCreate(XString /*p_schema*/,XString p_viewname,XString p_contents,bool /*p_ifexists /*= true*/) const
{
  return _T("RECREATE VIEW ") + QIQ(p_viewname) + _T("\n") + p_contents;
}

XString 
SQLInfoFirebird::GetCATALOGViewRename(XString /*p_schema*/,XString /*p_viewname*/,XString /*p_newname*/)    const
{
  return _T("");
}

XString 
SQLInfoFirebird::GetCATALOGViewDrop(XString /*p_schema*/,XString p_viewname,XString& p_precursor) const
{
  XString viewname = QIQ(p_viewname);

  if(!IsIdentifierMixedCase(p_viewname))
  {
    p_viewname.MakeUpper();
  }
  // Firebird cannot drop a view if dependencies from stored procedures or functions
  // still exist in the dependencies table. After Firebird 3.0 we need modification
  // rights on this system catalog table!!
  // Chances being that we re-create the view right away after the drop.
  p_precursor = _T("DELETE FROM rdb$dependencies\n")
                _T(" WHERE rdb$depended_on_name = '") + p_viewname + _T("'\n")
                _T("   AND rdb$depended_on_type = 0");
  return _T("DROP VIEW ") + viewname;
}

// All Privilege functions
XString
SQLInfoFirebird::GetCATALOGTablePrivileges(XString& p_schema,XString& p_tablename) const
{
  if(!IsIdentifierMixedCase(p_schema))
  {
    p_schema.MakeUpper();
  }
  if(!IsIdentifierMixedCase(p_tablename))
  {
    p_tablename.MakeUpper();
  }
  XString sql = _T("SELECT (SELECT trim(mon$database_name) from mon$database) as catalog_name\n")
                _T("      ,trim(rel.rdb$owner_name)     as table_schema\n")
                _T("      ,trim(priv.rdb$relation_name) as table_name\n")
                _T("      ,CASE\n")
                _T("            WHEN priv.rdb$user = priv.rdb$grantor THEN '_SYSTEM'\n")
                _T("            ELSE trim(priv.rdb$grantor)\n")
                _T("       END as grantor\n")
                _T("      ,trim(priv.rdb$user)          as grantee\n")
                _T("      ,trim(CASE priv.rdb$privilege\n")
                _T("                 WHEN 'I' THEN 'INSERT'\n")
                _T("                 WHEN 'S' THEN 'SELECT'\n")
                _T("                 WHEN 'U' THEN 'UPDATE'\n")
                _T("                 WHEN 'D' THEN 'DELETE'\n")
                _T("                 WHEN 'R' THEN 'REFERENCES'\n")
                _T("       END) AS privilege\n")
                _T("      ,CASE priv.rdb$grant_option\n")
                _T("           WHEN  0 THEN 'NO'\n")
                _T("           WHEN  1 THEN 'YES'\n")
                _T("           ELSE 'NO'\n")
                _T("       END as grantable\n")
                _T("  FROM rdb$user_privileges priv\n")
                _T("       inner join rdb$relations rel ON priv.rdb$relation_name = rel.rdb$relation_name\n")
                _T(" WHERE priv.rdb$field_name IS NULL\n");
  // Add the filter
  if(!p_schema.IsEmpty())
  {
    sql += _T("   AND rel.rdb$owner_name = '") + p_schema + _T("'\n");
  }
  if(!p_tablename.IsEmpty())
  {
    sql += _T("   AND priv.rdb$relation_name = '") + p_tablename + _T("'\n");
  }
  sql += _T(" ORDER BY 1,2,3,4,5");

  return sql;
}

XString 
SQLInfoFirebird::GetCATALOGColumnPrivileges(XString& p_schema,XString& p_tablename,XString& p_columnname) const
{
  bool p_quoted(true);
  XString sql = _T("SELECT (SELECT trim(mon$database_name) FROM mon$database) as catalog_name\n")
                _T("      ,trim(rel .rdb$owner_name)    as table_schema\n")
                _T("      ,trim(priv.rdb$relation_name) as table_name\n")
                _T("      ,trim(fld .rdb$field_name)    as column_name\n")
                _T("      ,CASE\n")
                _T("            WHEN priv.rdb$user = priv.rdb$grantor THEN trim('_SYSTEM')\n")
                _T("            ELSE trim(priv.rdb$grantor)\n")
                _T("       END as grantor\n")
                _T("      ,trim(priv.rdb$user)          as grantee\n")
                _T("      ,trim(CASE priv.rdb$privilege\n")
                _T("                 WHEN 'I' THEN 'INSERT'\n")
                _T("                 WHEN 'S' THEN 'SELECT'\n")
                _T("                 WHEN 'U' THEN 'UPDATE'\n")
                _T("                 WHEN 'D' THEN 'DELETE'\n")
                _T("                 WHEN 'R' THEN 'REFERENCES'\n")
                _T("       END) AS privilege\n")
                _T("      ,CASE priv.rdb$grant_option\n")
                _T("           WHEN  0 THEN 'NO'\n")
                _T("           WHEN  1 THEN 'YES'\n")
                _T("           ELSE 'NO'\n")
                _T("       END as grantable\n")
                _T("  FROM rdb$user_privileges priv\n")
                _T("       inner join rdb$relations       rel ON priv.rdb$relation_name = rel.rdb$relation_name\n")
                _T("       inner join rdb$relation_fields fld ON fld .rdb$relation_name = rel.rdb$relation_name\n")
                _T("                                         AND priv.rdb$field_name    = fld.rdb$field_name\n")
                _T(" WHERE 1 = 1\n");

  // Add the filters
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND rel.rdb$owner_name = '") + p_schema + _T("'\n");
  }
  if(!p_tablename.IsEmpty())
  {
    IdentifierCorrect(p_tablename);
    sql += _T("   AND priv.rdb$relation_name = '") + p_tablename + _T("'\n");
  }
  if(!p_columnname.IsEmpty())
  {
    IdentifierCorrect(p_columnname);
    sql += _T("   AND priv.rdb$field_name = ") + p_columnname + _T("'\n");
  }
  // Add together and order the results
  sql += _T(" ORDER BY 1,2,6,7,3,4,5");

  return sql;
}

XString
SQLInfoFirebird::GetCATALOGSequencePrivilege(XString& /*p_schema*/,XString& /*p_sequence*/) const
{
  return _T("");
}

XString
SQLInfoFirebird::GetCATALOGGrantPrivilege(XString /*p_schema*/
                                         ,XString p_objectname
                                         ,XString p_subObject
                                         ,XString p_privilege
                                         ,XString p_grantee
                                         ,bool    p_grantable)
{
  XString sql;
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
  return sql;
}

XString 
SQLInfoFirebird::GetCATALOGRevokePrivilege(XString /*p_schema*/
                                          ,XString p_objectname
                                          ,XString p_subObject
                                          ,XString p_privilege
                                          ,XString p_grantee)
{
  XString sql;
  sql.Format(_T("REVOKE %s"),p_privilege.GetString());
  if(!p_subObject.IsEmpty())
  {
    sql.AppendFormat(_T("(%s)"),QIQ(p_subObject).GetString());
  }
  sql.AppendFormat(_T(" ON %s FROM %s"),QIQ(p_objectname).GetString(),QIQ(p_grantee).GetString());
  return sql;
}

// All Synonym functions
XString
SQLInfoFirebird::GetCATALOGSynonymList(XString& /*p_schema*/,XString& /*p_pattern*/) const
{
  // Firebird does not implement SYNONYM
  return _T("");
}

XString
SQLInfoFirebird::GetCATALOGSynonymAttributes(XString& /*p_schema*/,XString& /*p_synonym*/) const
{
  // Firebird does not implement SYNONYM
  return _T("");
}

XString
SQLInfoFirebird::GetCATALOGSynonymCreate(XString& /*p_schema*/,XString& /*p_synonym*/,XString /*p_forObject*/,bool /*p_private = true*/) const
{
  // Firebird does not implement SYNONYM
  return _T("");
}

XString
SQLInfoFirebird::GetCATALOGSynonymDrop(XString& /*p_schema*/,XString& /*p_synonym*/,bool /*p_private = true*/) const
{
  // Firebird does not implement SYNONYM
  return _T("");
}

// For ALL objects
XString
SQLInfoFirebird::GetCATALOGCommentCreate(XString /*p_schema*/,XString p_object,XString p_name,XString p_subObject,XString p_remark) const
{
  XString sql;
  if(!p_object.IsEmpty() && !p_name.IsEmpty() && !p_remark.IsEmpty())
  {
    // User types in Firebird are DOMAIN's
    if(p_object.CompareNoCase(_T("TYPE")) == 0)
    {
      p_object = _T("DOMAIN");
    }
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
SQLInfoFirebird::GetPSMProcedureExists(XString p_schema, XString p_procedure,bool p_quoted /*= false*/) const
{
  IdentifierCorrect(p_procedure);

  XString extra;
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    extra = _T("   AND rdb$owner_name = '") + p_schema + _T("'\n");
  }
  XString query = (_T("SELECT (SELECT COUNT(*)\n")
                   _T("  FROM rdb$functions\n")
                   _T(" WHERE rdb$function_name  = '") + p_procedure + _T("'\n")
                   + extra +
                   _T("   AND rdb$function_type IS NOT NULL)\n")
                   _T("     + (SELECT COUNT(*)\n")
                   _T("  FROM rdb$procedures\n")
                   _T(" WHERE rdb$procedure_name = '") + p_procedure + _T("'\n")
                   + extra + 
                   _T("   AND rdb$procedure_source IS NOT NULL) as total\n")
                   _T("  FROM rdb$database"));
  return query;
}

XString
SQLInfoFirebird::GetPSMProcedureList(XString& p_schema,XString p_procedure,bool p_quoted /*= false*/) const
{
  IdentifierCorrect(p_schema);
  IdentifierCorrect(p_procedure);
  XString extra;

  if(!p_schema.IsEmpty())
  {
    extra = _T("   AND rdb$owner_name = '") + p_schema + _T("'\n");
  }
  XString sql1(_T("SELECT '' as catalog_name\n")
               _T("      ,trim(rdb$owner_name) as schema_name\n")
               _T("      ,trim(rdb$procedure_name)\n")
               _T("      ,1\n")  // PROCEDURE
               _T("  FROM rdb$procedures pro\n")
               _T(" WHERE rdb$procedure_source IS NOT NULL\n"));
  sql1 += extra;         
  if(!p_procedure.IsEmpty())
  {
    sql1 += _T("   AND rdb$procedure_name ");
    sql1 += (p_procedure.Find(_T("%")) >= 0) ? _T("LIKE") : _T("=");
    sql1 += _T(" ?");
  }
  XString sql2(_T("SELECT '' as catalog_name\n")
               _T("      ,trim(rdb$owner_name) as schema_name\n")
               _T("      ,trim(rdb$function_name)\n")
               _T("      ,2\n")  // FUNCTION
               _T("  FROM rdb$functions fun\n")
               _T(" WHERE rdb$function_type IS NOT NULL\n"));
  sql2 += extra;
  if(!p_procedure.IsEmpty())
  {
    sql2 += _T("   AND rdb$function_name \n");
    sql2 += (p_procedure.Find(_T("%")) >= 0) ? _T("LIKE") : _T("=");
    sql2 += _T(" ?");
  }
  return sql1 + _T(" UNION ALL\n") + sql2 + _T(" ORDER BY 1,2,3");
}

XString
SQLInfoFirebird::GetPSMProcedureAttributes(XString& p_schema,XString& p_procedure,bool p_quoted /*= false*/) const
{
  XString sql1(_T("SELECT trim(dbs.mon$database_name) as catalog_name\n")
               _T("      ,trim(pro.rdb$owner_name) as schema_name\n")
               _T("      ,trim(pro.rdb$procedure_name)\n")
               _T("      ,(SELECT COUNT(*)\n")
               _T("          FROM rdb$procedure_parameters par\n")
               _T("         WHERE par.rdb$procedure_name = pro.rdb$procedure_name\n")
               _T("           AND par.rdb$parameter_type = 0) as input_parameters\n")
               _T("      ,(SELECT COUNT(*)\n")
               _T("          FROM rdb$procedure_parameters par\n")
               _T("         WHERE par.rdb$procedure_name = pro.rdb$procedure_name\n")
               _T("           AND par.rdb$parameter_type = 1) as output_parameters\n")
               _T("      ,(SELECT COUNT(*)\n")
               _T("          FROM rdb$procedure_parameters par\n")
               _T("         WHERE par.rdb$procedure_name = pro.rdb$procedure_name\n")
               _T("           AND par.rdb$parameter_type = 1) as result_sets\n")
               _T("      ,pro.rdb$description\n")
               _T("      ,1 as procedure_type\n") // SQL_PROCEDURE
               _T("      ,'<@>'\n")
               _T("  FROM rdb$procedures pro\n")
               _T("      ,mon$database   dbs\n")
               _T(" WHERE rdb$procedure_source IS NOT NULL\n"));

  XString sql2 (_T("SELECT trim(dbs.mon$database_name) as catalog_name\n")
                _T("      ,trim(fun.rdb$owner_name) as schema_name\n")
                _T("      ,trim(fun.rdb$function_name)\n")
                _T("      ,(SELECT COUNT(*)\n")
                _T("          FROM rdb$function_arguments arg\n")
                _T("         WHERE fun.rdb$function_name = arg.rdb$function_name\n")
                _T("           AND arg.rdb$argument_position > 0) as input_parameters\n")
                _T("      ,(SELECT COUNT(*)\n")
                _T("          FROM rdb$function_arguments arg\n")
                _T("         WHERE fun.rdb$function_name = arg.rdb$function_name\n")
                _T("           AND arg.rdb$argument_position = 0) as output_parameters\n")
                _T("      ,(SELECT COUNT(*)\n")
                _T("          FROM rdb$function_arguments arg\n")
                _T("         WHERE fun.rdb$function_name = arg.rdb$function_name\n")
                _T("           AND arg.rdb$argument_position > 0) as result_sets\n")
                _T("      ,fun.rdb$description\n")
                _T("      ,2 as procedure_type\n") // SQL_FUNCTION
                _T("      ,'<@>'\n")
                _T("  FROM rdb$functions fun\n")
                _T("      ,mon$database  dbs\n")
                _T(" WHERE rdb$function_type IS NOT NULL\n"));
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql1 += _T("   AND pro.rdb$owner_name = '") + p_schema + _T("'\n");
    sql2 += _T("   AND fun.rdb$owner_name = ?\n");
  }
  if(!p_procedure.IsEmpty())
  {
    IdentifierCorrect(p_procedure);
    sql1 += _T("   AND pro.rdb$procedure_name = '") + p_procedure + _T("'\n");
    sql2 += _T("   AND fun.rdb$function_name  = ?\n");
  }
  return sql1 + _T(" UNION ALL\n") + sql2 + _T(" ORDER BY 1,2,3");;
}

// Getting the sourcecode: 3th column yields the source
XString
SQLInfoFirebird::GetPSMProcedureSourcecode(XString p_schema,XString p_procedure,bool p_quoted /*= false*/) const
{
  IdentifierCorrect(p_schema);
  IdentifierCorrect(p_procedure);

  // PROCEDURE en FUNCTION queries
  XString sql = _T("SELECT -1 as pos1\n")
                _T("      ,-1 AS pos2\n")
                _T("      ,'CREATE OR ALTER PROCEDURE \"' || Trim(rdb$procedure_name) || '\"' || ASCII_CHAR(10) AS text\n")
                _T("  FROM rdb$procedures\n")
                _T(" WHERE rdb$procedure_name = '") + p_procedure + _T("'\n")
                _T("UNION\n")
                _T("SELECT rdb$parameter_number AS pos1\n")
                _T("      ,rdb$parameter_number AS pos2\n")
                _T("      ,CASE WHEN rdb$parameter_number = min_number THEN '(' ELSE ' ' END ||\n")
                _T("       '\"' || Trim(rdb$parameter_name) || '\" ' ||\n")
                _T("       CASE Trim(typ.rdb$type_name)\n")
                _T("            WHEN 'SHORT'   THEN 'NUMERIC'\n")
                _T("            WHEN 'LONG'    THEN 'NUMERIC'\n")
                _T("            WHEN 'INT64'   THEN 'NUMERIC'\n")
                _T("            WHEN 'TEXT'    THEN 'CHAR'\n")
                _T("            WHEN 'VARYING' THEN 'VARCHAR'\n")
                _T("           ELSE Trim(typ.rdb$type_name)\n")
                _T("       END || \n")
                _T("       CASE WHEN coalesce(fld.rdb$field_precision,fld.rdb$character_length,0) <> 0\n")
                _T("                 THEN '(' || CAST(coalesce(fld.rdb$field_precision,fld.rdb$character_length) AS VARCHAR(12)) ||\n")
                _T("       CASE WHEN coalesce(fld.rdb$field_scale,0) <> 0\n")
                _T("                 THEN ',' || CAST(Abs(fld.rdb$field_scale)  AS VARCHAR(12)) || ')' ELSE ')' END\n")
                _T("            ELSE '' END || \n")
                _T("       CASE WHEN rdb$parameter_number = max_number THEN ')' ELSE ',' END ||\n") 
                _T("       ASCII_CHAR(10) AS param_name\n")
                _T("  FROM (SELECT par.rdb$parameter_name\n")
                _T("              ,par.rdb$parameter_number\n")
                _T("              ,par.rdb$field_source\n")
                _T("              ,(SELECT Min(rdb$parameter_number)\n")
                _T("                  FROM rdb$procedure_parameters pi\n")
                _T("                 WHERE pi.rdb$procedure_name = '") + p_procedure + _T("'\n")
                _T("                   AND pi.rdb$parameter_type = 0) AS min_number\n")
                _T("              ,(SELECT Max(rdb$parameter_number)\n")
                _T("                  FROM rdb$procedure_parameters pi\n")
                _T("                 WHERE pi.rdb$procedure_name = '") + p_procedure + _T("'\n")
                _T("                   AND pi.rdb$parameter_type = 0) AS max_number\n")
                _T("          FROM rdb$procedure_parameters par\n")
                _T("         WHERE rdb$procedure_name = '") + p_procedure + _T("'\n")
                _T("           AND rdb$parameter_type = 0\n")
                _T("         ORDER BY par.rdb$parameter_number)\n")
                _T("      INNER JOIN rdb$fields fld ON  fld.rdb$field_name = rdb$field_source\n")
                _T("      INNER JOIN rdb$types  typ ON(fld.rdb$field_type = typ.rdb$type\n")
                _T("                               AND  typ.rdb$field_name = 'RDB$FIELD_TYPE')\n")
                _T("UNION\n")
                _T("SELECT rdb$parameter_number + 5000 AS pos1\n")
                _T("      ,rdb$parameter_number + 5000 AS pos2\n")
                _T("      ,Trim(CASE WHEN rdb$parameter_number = min_number THEN 'RETURNS' || ASCII_CHAR(10) || '(' ELSE '' END) ||\n")
                _T("       '\"' || Trim(rdb$parameter_name) || '\" ' ||\n")
                _T("       CASE Trim(typ.rdb$type_name)\n")
                _T("            WHEN 'SHORT'   THEN 'NUMERIC'\n")
                _T("            WHEN 'LONG'    THEN 'NUMERIC'\n")
                _T("            WHEN 'INT64'   THEN 'NUMERIC'\n")
                _T("            WHEN 'TEXT'    THEN 'CHAR'\n")
                _T("            WHEN 'VARYING' THEN 'VARCHAR'\n")
                _T("           ELSE Trim(typ.rdb$type_name)\n")
                _T("       END ||\n")
                _T("       CASE WHEN coalesce(fld.rdb$field_precision,fld.rdb$character_length,0) <> 0\n")
                _T("                 THEN '(' || CAST(coalesce(fld.rdb$field_precision,fld.rdb$character_length) AS VARCHAR(12)) ||\n")
                _T("       CASE WHEN coalesce(fld.rdb$field_scale,0) <> 0\n")
                _T("                 THEN ',' || CAST(Abs(fld.rdb$field_scale)  AS VARCHAR(12)) || ')' ELSE ')' END\n")
                _T("            ELSE '' END ||\n")
                _T("       CASE WHEN rdb$parameter_number = max_number THEN ')' ELSE ',' END ||\n") 
                _T("       ASCII_CHAR(10) AS param_name\n")
                _T("  FROM (SELECT par.rdb$parameter_name\n")
                _T("              ,par.rdb$parameter_number\n")
                _T("              ,par.rdb$field_source\n")
                _T("              ,(SELECT Min(rdb$parameter_number)\n")
                _T("                  FROM rdb$procedure_parameters pi\n")
                _T("                 WHERE pi.rdb$procedure_name = '") + p_procedure + _T("'\n")
                _T("                   AND pi.rdb$parameter_type = 1) AS min_number\n")
                _T("              ,(SELECT Max(rdb$parameter_number)\n")
                _T("                  FROM rdb$procedure_parameters pi\n")
                _T("                 WHERE pi.rdb$procedure_name = '") + p_procedure + _T("'\n")
                _T("                   AND pi.rdb$parameter_type = 1) AS max_number\n")
                _T("          FROM rdb$procedure_parameters par\n")
                _T("         WHERE rdb$procedure_name = '") + p_procedure + _T("'\n")
                _T("           AND rdb$parameter_type = 1\n")
                _T("          ORDER BY par.rdb$parameter_number)\n")
                _T("       INNER JOIN rdb$fields fld ON  fld.rdb$field_name = rdb$field_source\n")
                _T("       INNER JOIN rdb$types  typ ON (fld.rdb$field_type = typ.rdb$type\n")
                _T("                                AND  typ.rdb$field_name = 'RDB$FIELD_TYPE')\n")
                _T("UNION\n")
                _T("SELECT 10000 AS pos1\n")
                _T("      ,10000 AS pos2\n")
                _T("      ,'AS' || ASCII_CHAR(10) || \n")
                _T("       rdb$procedure_source AS text\n")
                _T("  FROM rdb$procedures\n")
                _T(" WHERE rdb$procedure_name = '") + p_procedure + _T("'\n")
                _T("UNION\n")
                //
                // FUNCTIONS
                //
                _T("SELECT -1 AS pos1\n")
                _T("      ,-1 AS pos2\n")
                _T("      ,'CREATE OR ALTER FUNCTION \"' || Trim(rdb$functions.rdb$function_name) || '\"' || ASCII_CHAR(10) AS text\n")
                _T("  FROM rdb$functions\n")
                _T(" WHERE rdb$function_name = '") + p_procedure + _T("'\n")
                _T("UNION\n")
                _T("SELECT rdb$argument_position AS pos1\n")
                _T("      ,rdb$argument_position AS pos2\n")
                _T("      ,CASE WHEN rdb$argument_position = min_number THEN '(' ELSE ' ' END ||\n")
                _T("       '\"' || Trim(rdb$argument_name) || '\" ' ||\n")
                _T("       CASE Trim(typ.rdb$type_name)\n")
                _T("            WHEN 'SHORT'   THEN 'NUMERIC'\n")
                _T("            WHEN 'LONG'    THEN 'NUMERIC'\n")
                _T("            WHEN 'INT64'   THEN 'NUMERIC'\n")
                _T("            WHEN 'TEXT'    THEN 'CHAR'\n")
                _T("            WHEN 'VARYING' THEN 'VARCHAR'\n")
                _T("           ELSE Trim(typ.rdb$type_name)\n")
                _T("       END ||\n")
                _T("       CASE WHEN coalesce(fld.rdb$field_precision,fld.rdb$character_length,0) <> 0\n")
                _T("                 THEN '(' || CAST(coalesce(fld.rdb$field_precision,fld.rdb$character_length) AS VARCHAR(12)) ||\n")
                _T("       CASE WHEN coalesce(fld.rdb$field_scale,0) <> 0\n")
                _T("                 THEN ',' || CAST(Abs(fld.rdb$field_scale)  AS VARCHAR(12)) || ')' ELSE ')' END\n")
                _T("            ELSE '' END ||\n")
                _T("       CASE WHEN rdb$argument_position = max_number THEN ')' ELSE ',' END ||\n") 
                _T("       ASCII_CHAR(10) AS param_name\n")
                _T("  FROM (SELECT arg.rdb$argument_name\n")
                _T("              ,arg.rdb$argument_position\n")
                _T("              ,arg.rdb$field_source\n")
                _T("              ,(SELECT Min(rdb$argument_position)\n")
                _T("                  FROM rdb$function_arguments ai\n")
                _T("                 WHERE rdb$function_name = '") + p_procedure + _T("'\n")
                _T("                   AND ai.rdb$argument_position > 0) AS min_number\n")
                _T("              ,(SELECT Max(rdb$argument_position)\n")
                _T("                  FROM rdb$function_arguments ai\n")
                _T("                 WHERE rdb$function_name = '") + p_procedure +_T("'\n")
                _T("                   AND ai.rdb$argument_position > 0) AS max_number\n")
                _T("         FROM rdb$function_arguments arg\n")
                _T("        WHERE rdb$function_name = '") + p_procedure + _T("'\n")
                _T("          AND rdb$argument_position > 0\n")
                _T("        ORDER BY rdb$argument_position)\n")
                _T("      INNER JOIN rdb$fields fld ON  fld.rdb$field_name = rdb$field_source\n")
                _T("      INNER JOIN rdb$types  typ ON (fld.rdb$field_type = typ.rdb$type\n")
                _T("                               AND  typ.rdb$field_name = 'RDB$FIELD_TYPE')\n")
                _T("UNION\n")
                _T("SELECT 5000 AS pos1\n")
                _T("      ,5000 AS pos2\n")
                _T("      ,'RETURNS ' ||\n")
                _T("       CASE Trim(typ.rdb$type_name)\n")
                _T("            WHEN 'SHORT'   THEN 'NUMERIC'\n")
                _T("            WHEN 'LONG'    THEN 'NUMERIC'\n")
                _T("            WHEN 'INT64'   THEN 'NUMERIC'\n")
                _T("            WHEN 'TEXT'    THEN 'CHAR'\n")
                _T("            WHEN 'VARYING' THEN 'VARCHAR'\n")
                _T("           ELSE Trim(typ.rdb$type_name)\n")
                _T("       END ||\n")
                _T("       CASE WHEN coalesce(fld.rdb$field_precision,fld.rdb$character_length,0) <> 0\n")
                _T("                 THEN '(' || CAST(coalesce(fld.rdb$field_precision,fld.rdb$character_length) AS VARCHAR(12)) ||\n")
                _T("       CASE WHEN coalesce(fld.rdb$field_scale,0) <> 0\n")
                _T("                 THEN ',' || CAST(Abs(fld.rdb$field_scale)  AS VARCHAR(12)) || ')' ELSE ')' END\n")
                _T("            ELSE '' END ||\n") 
                _T("       ASCII_CHAR(10) AS text\n")
                _T("  FROM rdb$function_arguments arg\n")
                _T("       INNER JOIN rdb$fields fld ON  fld.rdb$field_name = arg.rdb$field_source\n")
                _T("       INNER JOIN rdb$types  typ ON (fld.rdb$field_type = typ.rdb$type\n")
                _T("                                AND  typ.rdb$field_name = 'RDB$FIELD_TYPE')\n")
                _T(" WHERE rdb$function_name = '") + p_procedure + _T("'\n")
                _T("   AND arg.rdb$argument_position = 0\n")
                _T("UNION\n")
                _T("SELECT 10000 AS pos1\n")
                _T("      ,10000 AS pos2\n")
                _T("      ,'AS' || ASCII_CHAR(10) || rdb$function_source AS text\n")
                _T("  FROM rdb$functions\n")
                _T(" WHERE rdb$function_name = '") + p_procedure + _T("'\n")
                _T(" ORDER BY 1\n");

  return sql;
}

XString
SQLInfoFirebird::GetPSMProcedureCreate(MetaProcedure& p_procedure) const
{
  return p_procedure.m_source;
}

XString
SQLInfoFirebird::GetPSMProcedureDrop(XString /*p_schema*/, XString p_procedure,bool /*p_function /*=false*/) const
{
  XString sql(_T("DROP PROCEDURE ") + QIQ(p_procedure));
  return sql;
}

XString
SQLInfoFirebird::GetPSMProcedureErrors(XString /*p_schema*/,XString /*p_procedure*/,bool /*p_quoted = false*/) const
{
  // Firebird does not support procedure errors
  return _T("");
}

XString
SQLInfoFirebird::GetPSMProcedurePrivilege(XString& /*p_schema*/,XString& /*p_procedure*/,bool /*p_quoted = false*/) const
{
  return _T("");
}

// And it's parameters
XString
SQLInfoFirebird::GetPSMProcedureParameters(XString& p_schema,XString& p_procedure,bool p_quoted /*= false*/) const
{
  XString sql1 = _T("SELECT TRIM(dbs.mon$database_name)  as catalog_name\n"
                    "      ,TRIM(pro.rdb$owner_name)     as schema_name\n"
                    "      ,TRIM(pro.rdb$procedure_name) as procedure_name\n"
                    "      ,TRIM(par.rdb$parameter_name) as column_name\n"
                    "      ,(par.rdb$parameter_type * 3) + 1 as column_type\n"
                    "      ,CASE fld.rdb$field_type\n"
                    "            WHEN 7  THEN 5\n"
                    "            WHEN 8  THEN 4\n"
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
                    "            WHEN 7  THEN 'SMALLINT'\n"
                    "            WHEN 8  THEN 'INTEGER'\n"
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
                    "            WHEN 7  THEN 5\n"
                    "            WHEN 8  THEN 11\n"
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
                    "      ,CASE coalesce(par.rdb$null_flag,0)\n"
                    "            WHEN 0 THEN 'YES'\n"
                    "            WHEN 1 THEN 'NO'\n"
                    "                   ELSE 'UNKNOWN'\n"
                    "       END                                         AS is_nullable\n"
                    "      ,par.RDB$DESCRIPTION   as remarks\n"
                    "      ,par.RDB$DEFAULT_source as default_value\n"
                    "      ,CASE fld.rdb$field_type\n"
                    "            WHEN 7  THEN 5\n"
                    "            WHEN 8  THEN 4\n"
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
                    "      ,par.rdb$parameter_number + par.rdb$parameter_type + 1 as ordinal_position\n"
                    "      ,CASE (coalesce(par.rdb$null_flag,0,0)-1)*-1\n"
                    "            WHEN 0 THEN 'NO'\n"
                    "            WHEN 1 THEN 'YES'\n"
                    "                   ELSE 'UNKNOWN'\n"
                    "        END        AS is_nullable\n"
                    "  FROM rdb$procedures pro\n"
                    "      ,rdb$procedure_parameters par\n"
                    "      ,rdb$fields   fld\n"
                    "      ,mon$database dbs\n"
                    " WHERE pro.rdb$procedure_name = par.rdb$procedure_name\n"
                    "   AND fld.rdb$field_name     = par.rdb$field_source\n");

  XString sql2 = _T("SELECT TRIM(dbs.mon$database_name) as catalog_name\n"
                    "      ,TRIM(fun.rdb$owner_name)    as schema_name\n"
                    "      ,trim(par.rdb$function_name) as procedure_name\n"
                    "      ,trim(par.rdb$argument_name) as column_name\n"
                    "      ,CASE par.rdb$argument_name IS NULL\n"
                    "            WHEN TRUE THEN 5\n"
                    "                      ELSE 2 \n"
                    "       END as column_type\n"
                    "      ,CASE fld.rdb$field_type\n"
                    "            WHEN 7  THEN 5\n"
                    "            WHEN 8  THEN 4\n"
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
                    "            WHEN 7  THEN 'SMALLINT'\n"
                    "            WHEN 8  THEN 'INTEGER'\n"
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
                    "            WHEN 7  THEN 5\n"
                    "            WHEN 8  THEN 11\n"
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
                    "            WHEN 7  THEN 5\n"
                    "            WHEN 8  THEN 4\n"
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
                    "  FROM rdb$functions fun\n"
                    "      ,rdb$function_arguments par\n"
                    "      ,rdb$fields    fld\n"
                    "      ,mon$database  dbs\n"
                    " WHERE fun.rdb$function_name = par.rdb$function_name\n"
                    "   AND fld.rdb$field_name    = par.rdb$field_source\n");

  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql1 += _T("   AND pro.rdb$owner_name = '") + p_schema + _T("'\n");
    sql2 += _T("   AND fun.rdb$owner_name = ?\n");
  }
  if(!p_procedure.IsEmpty())
  {
    IdentifierCorrect(p_procedure);
    sql1 += _T("   AND pro.rdb$procedure_name = '") + p_procedure + _T("'\n");
    sql2 += _T("   AND fun.rdb$function_name = ?\n");
  }
  sql1 += _T("UNION ALL\n");
  sql1 += sql2;
  sql1 += _T("ORDER BY 1,2,3,18");
  
  return sql1;
}

//////////////////////////////////////////////////////////////////////////
//
// ALL PSM LANGUAGE ELEMENTS
//
//////////////////////////////////////////////////////////////////////////

XString
SQLInfoFirebird::GetPSMDeclaration(bool    /*p_first*/
                                  ,XString p_variable
                                  ,int     p_datatype
                                  ,int     p_precision /*= 0 */
                                  ,int     p_scale     /*= 0 */
                                  ,XString p_default   /*= ""*/
                                  ,XString p_domain    /*= ""*/
                                  ,XString p_asColumn  /*= ""*/) const
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
  else if(!p_domain.IsEmpty())
  {
    line += _T(" TYPE OF ") + p_domain;
  }
  else if(!p_asColumn)
  {
    line += _T(" TYPE OF COLUMN ") + p_asColumn;
  }
  line += _T(";\n");
  return line;
}

XString
SQLInfoFirebird::GetPSMAssignment(XString p_variable,XString p_statement /*= ""*/) const
{
  XString line(p_variable);
  line += _T(" = ");
  if(!p_statement.IsEmpty())
  {
    line += p_statement;
    line += _T(";");
  }
  return line;
}

XString
SQLInfoFirebird::GetPSMIF(XString p_condition) const
{
  XString line(_T("IF ("));
  line += p_condition;
  line += _T(")\n  BEGIN\n");
  return line;
}

XString
SQLInfoFirebird::GetPSMIFElse() const
{
  XString line(_T("  END\n"
                  "ELSE\n"
                  "  BEGIN\n"));
  return line;
}

XString
SQLInfoFirebird::GetPSMIFEnd() const
{
  XString line(_T("  END;\n"));
  return line;
}

XString
SQLInfoFirebird::GetPSMWhile(XString p_condition) const
{
  return _T("WHILE (") + p_condition + _T(") LOOP\n");
}

XString
SQLInfoFirebird::GetPSMWhileEnd() const
{
  return _T("END LOOP;\n");
}

XString
SQLInfoFirebird::GetPSMLOOP() const
{
  return _T("WHILE (true) DO BEGIN\n");
}

XString
SQLInfoFirebird::GetPSMLOOPEnd() const
{
  return _T("END;\n");
}

XString
SQLInfoFirebird::GetPSMBREAK() const
{
  return _T("BREAK;\n");
}

XString
SQLInfoFirebird::GetPSMRETURN(XString /* p_statement /*= ""*/) const
{
  return _T("SUSPEND;\n");
}

XString
SQLInfoFirebird::GetPSMExecute(XString p_procedure,MParameterMap& p_parameters) const
{
  // EXECUTE PROCEDURE name[(:param[,:param …])] [RETURNING_VALUES:param[,:param …]];
  XString line;
  line.Format(_T("EXECUTE PROCEDURE %s ("),QIQ(p_procedure).GetString());
  bool doReturning = false;
  bool doMore = false;

  for(auto& param : p_parameters)
  {
    // Extra ,
    if(doMore) line += _T(",");
    doMore = true;

    // Append input and in/out parameters
    if(param.m_columnType == SQL_PARAM_INPUT || param.m_columnType == SQL_PARAM_INPUT_OUTPUT)
    {
      line.AppendFormat(_T(":%s"),QIQ(param.m_parameter).GetString());
    }
    // See if we must do 'returning' clause
    if(param.m_columnType == SQL_PARAM_OUTPUT || param.m_columnType == SQL_PARAM_INPUT_OUTPUT)
    {
      doReturning = true;
    }
  }
  line += _T(")");

  // Do the returning clause
  if(doReturning)
  {
    line += _T(" RETURNING VALUES ");
    doMore = false;
    for(auto& param : p_parameters)
    {
      if(doMore) line += _T(",");
      doMore = true;

      if(param.m_columnType == SQL_PARAM_OUTPUT || param.m_columnType == SQL_PARAM_INPUT_OUTPUT)
      {
        line.AppendFormat(_T(":%s"),QIQ(param.m_parameter).GetString());
      }
    }
  }
  line += _T(";\n");
  return line;
}

// The CURSOR
XString
SQLInfoFirebird::GetPSMCursorDeclaration(XString p_cursorname,XString p_select) const
{
  return _T("DECLARE ") + p_cursorname + _T(" SCROLL CURSOR FOR (") + p_select + _T(");");
}

XString
SQLInfoFirebird::GetPSMCursorFetch(XString p_cursorname,std::vector<XString>& /*p_columnnames*/,std::vector<XString>& p_variablenames) const
{
  XString query = _T("FETCH ") + p_cursorname + _T(" INTO ");
  bool moreThenOne = false;

  for(const auto& var : p_variablenames)
  {
    if(moreThenOne) query += _T(",");
    moreThenOne = true;

    query += _T(":") + var;
  }
  query += _T(";");
  return query;
}

//////////////////////////////////////////////////////////////////////////
// PSM Exceptions

XString
SQLInfoFirebird::GetPSMExceptionCatchNoData() const
{
  // SQLSTATE 02000 equals the error 100 Data not found
  return _T("WHEN SQLSTATE 02000 DO\n");
}

XString
SQLInfoFirebird::GetPSMExceptionCatch(XString p_sqlState) const
{
  return _T("WHEN SQLSTATE ") + p_sqlState + _T(" DO\n");
}

XString
SQLInfoFirebird::GetPSMExceptionRaise(XString p_sqlState) const
{
  return _T("EXCEPTION ") + p_sqlState;
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
SQLInfoFirebird::GetSESSIONMyself() const
{
  XString query = _T("SELECT mon.mon$attachment_id\n"
                     "      ,mon.mon$remote_os_user\n"
                     "      ,mon.mon$remote_host\n"
                     "      ,mon.mon$timestamp\n"
                     "      ,mon.mon$remote_address\n"
                     "      ,mon.mon$remote_process\n"
                     "      ,mon.mon$remote_pid\n"
                     "  FROM mon$attachments mon\n"
                     " WHERE mon$attachment_id = current_connection");
  return query;
}

XString
SQLInfoFirebird::GetSESSIONExists(XString p_sessionID) const
{
  XString query;
  query.Format(_T("SELECT COUNT(*)\n"
                  "  FROM mon$attachments\n"
                  " WHERE mon$system_flag = 0\n"
                  "   AND mon$attachment_id = %s"),p_sessionID.GetString());
  return query;
}

XString
SQLInfoFirebird::GetSESSIONList() const
{
  return GetSESSIONAttributes("");
}

XString
SQLInfoFirebird::GetSESSIONAttributes(XString p_sessionID) const
{
  XString sql = _T("SELECT mon.mon$attachment_id\n"
                   "      ,mon.mon$remote_os_user\n"
                   "      ,mon.mon$remote_host\n"
                   "      ,mon.mon$timestamp\n"
                   "      ,mon.mon$remote_address\n"
                   "      ,mon.mon$remote_process\n"
                   "      ,mon.mon$remote_pid\n"
                   "  FROM mon$attachments mon");
  if(!p_sessionID.IsEmpty())
  {
    sql += _T("\n WHERE mon$attachment_id = ") + p_sessionID;
  }
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// Transactions

XString
SQLInfoFirebird::GetSESSIONConstraintsDeferred() const
{
  // Firebird cannot defer constraints
  return XString();
}

XString
SQLInfoFirebird::GetSESSIONConstraintsImmediate() const
{
  // Firebird constraints are always active
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
SQLInfoFirebird::DoSQLCall(SQLQuery* p_query,XString& /*p_schema*/,XString& p_procedure)
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

// Calling a stored function with named parameters, returning a value
SQLVariant*
SQLInfoFirebird::DoSQLCallNamedParameters(SQLQuery* /*p_query*/,XString& /*p_schema*/,XString& /*p_procedure*/,bool /*p_function = true*/)
{
  return nullptr;
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
SQLInfoFirebird::DoSQLCallFunction(SQLQuery* p_query,const XString& p_function)
{
  SQLQuery query(m_database);
  XString sql = ConstructSQLForFunctionCall(p_query,&query,p_function);

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
SQLInfoFirebird::DoSQLCallProcedure(SQLQuery* p_query,const XString& p_procedure)
{
  SQLQuery query(m_database);
  XString sql = ConstructSQLForProcedureCall(p_query,&query,p_procedure);

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
        const SQLVariant* target = p_query->GetParameter(++setIndex);
        if(target == nullptr)
        {
          throw StdException(_T("Wrong number of output parameters for procedure call"));
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
XString
SQLInfoFirebird::ConstructSQLForFunctionCall(SQLQuery*      p_query
                                            ,SQLQuery*      p_thecall
                                            ,const XString& p_function)
{
  // Start with select from function
  XString sql = _T("SELECT ");
  sql += QIQ(p_function);

  // Opening parenthesis
  sql += _T("(");

  // Build list of markers
  int ind = 1;
  while(true)
  {
    // Try get the next parameter
    var* parameter = p_query->GetParameter(ind);
    if(parameter == nullptr) break;

    // Add marker
    if(ind > 1) sql += _T(",");
    sql += _T("?");

    // Add the parameter
    p_thecall->SetParameter(ind,parameter);

    // Next parameter
    ++ind;
  }

  // Closing parenthesis
  sql += _T(")");

  // From singular object
  sql += _T(" FROM rdb$database");
  return sql;
}

// Construct the "SELECT * FROM procedure(?,?)" (input parameters ONLY!)
XString
SQLInfoFirebird::ConstructSQLForProcedureCall(SQLQuery*      p_query
                                             ,SQLQuery*      p_thecall
                                             ,const XString& p_procedure)
{
  // Start with select form
  XString sql = _T("SELECT * FROM ");
  sql += p_procedure;

  // Opening parenthesis
  sql += _T("(");

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
      if(ind > 1) sql += _T(",");
      sql += _T("?");

      // Add the parameter with the result counter!
      p_thecall->SetParameter(res++,parameter);
    }
    // Next parameter
    ++ind;
  }

  // CLosing parenthesis
  sql += _T(")");

  // The procedure IS the singular object
  // Procedure **MUST** end with "SUSPEND;" 
  return sql;
}

// End of namespace
}

// Remove catalog dependencies
// XString 
// SQLInfoFirebird::GetSQLRemoveProcedureDependencies(XString p_procname) const
// {
//   XString upperName = p_procname;
//   upperName.MakeUpper();
//   XString query = "DELETE FROM rdb$dependencies\n"
//                   " WHERE rdb$depended_on_name = '" + upperName + "'\n"
//                   "   AND rdb$depended_on_type = 5\n"
//                   "   AND rdb$dependent_type   = 5";
//   return query;
// }
// 
// XString
// SQLInfoFirebird::GetSQLRemoveFieldDependencies(XString p_tablename) const
// {
//   XString upperName = p_tablename;
//   upperName.MakeUpper();
//   XString query = "DELETE FROM rdb$dependencies\n"
//                   " WHERE rdb$depended_on_name = '" + upperName + "'\n"
//                   "   AND rdb$dependent_type   = 3";
//   return query;
// }
// 


