////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoSQLServer.cpp
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

// For SQLServer the identifiers are transformed to LOWER case
// as the system catalog is stored in this case setting.
#define IdentifierCorrect(ident)   if(!p_quoted || !IsIdentifierMixedCase(ident)) ident.MakeLower()

// Constructor.
SQLInfoSQLServer::SQLInfoSQLServer(SQLDatabase* p_database)
                 :SQLInfoDB(p_database)
{
}

// Destructor. does nothing
SQLInfoSQLServer::~SQLInfoSQLServer()
{
}

// RDBMS Uses INDENTITY or SEQUENCE interface
void
SQLInfoSQLServer::SetUseSequences(bool p_sequences)
{
  m_useSequences = p_sequences;
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
  return _T("Microsoft");
}

// Get the physical database name
XString
SQLInfoSQLServer::GetRDBMSPhysicalDatabaseName() const
{
  XString query = _T("SELECT DB_NAME()");
  SQLQuery qry(m_database);
  qry.DoSQLStatement(query);
  if(qry.GetRecord())
  {
    return qry.GetColumn(1)->GetAsString();
  }
  return XString(_T(""));
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

// Supports the ODBC call procedure with named parameters
bool
SQLInfoSQLServer::GetRDBMSSupportsODBCCallNamedParameters() const
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

// Support for "as" in alias statements (FROM clause)
bool
SQLInfoSQLServer::GetRDBMSSupportsAsInAlias() const
{
  return true;
}

// Foreign key DDL defines the index and cannot reuse already existing ones
bool
SQLInfoSQLServer::GetRDBMSForeignKeyDefinesIndex() const
{
  // No: you must EXPLICITLY create an index, independent from the foreign key creation.
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
  if(p_scale >= (SQLSMALLINT) p_precision)
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

// Identifier rules differ per RDBMS
bool
SQLInfoSQLServer::IsIdentifier(XString p_identifier) const
{
  // Cannot be empty and cannot exceed this amount of characters
  if(p_identifier.GetLength() == 0 ||
     p_identifier.GetLength() > (int)GetMaxIdentifierNameLength())
  {
    return false;
  }
  // Must start with one alpha char OR AN UNDERSCORE (EXTENSION)
  if(!_istalpha(p_identifier.GetAt(0)) && p_identifier.GetAt(0) != '_')
  {
    return false;
  }
  for(int index = 0;index < p_identifier.GetLength();++index)
  {
    // Can be upper/lower alpha or a number OR an underscore
    TCHAR ch = p_identifier.GetAt(index);
    if(!_istalnum(ch) && ch != '_')
    {
      return false;
    }
  }
  return true;
}

// KEYWORDS

// Keyword for the current date and time
XString
SQLInfoSQLServer::GetKEYWORDCurrentTimestamp() const
{
  return _T("GETDATE()");
}

// String for the current date
XString
SQLInfoSQLServer::GetKEYWORDCurrentDate() const
{
  return _T("GETDATE()");
}

// Get the concatenation operator
XString
SQLInfoSQLServer::GetKEYWORDConcatanationOperator() const
{
  return _T("+");
}

// Get quote character for strings
XString
SQLInfoSQLServer::GetKEYWORDQuoteCharacter() const
{
  return _T("\'");
}

// Get quote character around reserved words as an identifier
XString
SQLInfoSQLServer::GetKEYWORDReservedWordQuote() const
{
  return _T("\"");
}

// Get default NULL for parameter list input
XString
SQLInfoSQLServer::GetKEYWORDParameterDefaultNULL() const
{
  return _T("= NULL");
}

// Parameter is for INPUT and OUTPUT in parameter list
XString
SQLInfoSQLServer::GetKEYWORDParameterINOUT() const
{
  return _T("OUTPUT");
}

// Parameter is for OUTPUT only in parameter list
XString
SQLInfoSQLServer::GetKEYWORDParameterOUT() const
{
  return _T("OUTPUT");
}

// Get datatype of the IDENTITY primary key in a Network database
XString
SQLInfoSQLServer::GetKEYWORDNetworkPrimaryKeyType() const
{
  return _T("INTEGER IDENTITY(1,1)");
}

// Get datatype for timestamp (year to second)
XString
SQLInfoSQLServer::GetKEYWORDTypeTimestamp() const
{
  return _T("DATETIME");
}

// Prefix for a parameter in a stored procedure
XString
SQLInfoSQLServer::GetKEYWORDParameterPrefix() const
{
  return _T("@");
}

// Get select part to add new record identity to a table
// Can be special column like 'OID' or a sequence select
XString
SQLInfoSQLServer::GetKEYWORDIdentityString(XString& p_tablename,XString p_postfix /*= "_seq"*/) const
{
  if(m_useSequences)
  {
    return _T("NEXT VALUE FOR ") + p_tablename + p_postfix;
  }
  else
  {
    return _T("IDENT_CURRENT('") + p_tablename + _T("') + ") + _T("IDENT_INCR('") + p_tablename + _T("')");
  }
}

// Gets the UPPER function
XString
SQLInfoSQLServer::GetKEYWORDUpper(XString& p_expression) const
{
  return _T("UPPER(") + p_expression + _T(")");
}

// Gets the construction for 1 minute ago
XString
SQLInfoSQLServer::GetKEYWORDInterval1MinuteAgo() const
{
  return _T("DATEADD(minute,-1,current_timestamp)");
}

// Gets the Not-NULL-Value statement of the database
XString
SQLInfoSQLServer::GetKEYWORDStatementNVL(XString& p_test,XString& p_isnull) const
{
  return XString(_T("NVL(")) + p_test + _T(",") + p_isnull + _T(")");
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
    case SQL_WVARCHAR:                  type = _T("VARCHAR");  break;
    case SQL_LONGVARCHAR:               type = _T("VARCHAR(max)");
                                        p_column->m_columnSize = 0;
                                        break;
    case SQL_WLONGVARCHAR:              type = _T("NVARCHAR(max)");
                                        p_column->m_columnSize = 0;
                                        break;
    case SQL_NUMERIC:                   [[fallthrough]];
    case SQL_DECIMAL:                   if((p_column->m_columnSize    == 38   || 
                                            p_column->m_columnSize    ==  0 ) &&
                                            p_column->m_decimalDigits ==  0 )
                                        {
                                          type = _T("INT");
                                          p_column->m_columnSize = 0;
                                        }
                                        else
                                        {
                                          type = _T("NUMERIC");
                                        }
                                        break;
    case SQL_INTEGER:                   type = _T("INT");      break;
    case SQL_SMALLINT:                  type = _T("SMALLINT"); break;
    case SQL_FLOAT:                     if(p_column->m_columnSize == 38 ||
                                           p_column->m_columnSize == 18)
                                        {
                                          if(p_column->m_decimalDigits == 0)
                                          {
                                            type = _T("INT");
                                            p_column->m_columnSize = 0;
                                          }
                                          else
                                          {
                                          type = _T("NUMERIC");
                                          }
                                          p_column->m_columnSize    = 18;
                                          p_column->m_decimalDigits = 0;
                                        }
                                        else
                                        {
                                          type = _T("FLOAT");
                                        }
                                        break;
    case SQL_REAL:                      // fall through
    case SQL_DOUBLE:                    type = _T("REAL");     break;
    case SQL_BIGINT:                    type = _T("BIGINT");   break;
    case SQL_TINYINT:                   type = _T("TINYINT");  break;
    case SQL_BIT:                       type = _T("TINYINT");
                                        p_column->m_columnSize    = 0;
                                        p_column->m_decimalDigits = 0;
                                        break;
  //case SQL_DATE:
    case SQL_DATETIME:                  // fall through
    case SQL_TYPE_DATE:                 // fall through
    case SQL_TIMESTAMP:                 // fall through
    case SQL_TYPE_TIMESTAMP:            type = _T("DATETIME");
                                        p_column->m_columnSize    = 0;
                                        p_column->m_decimalDigits = 0;
                                        break;
    case SQL_TIME:                      // fall through
    case SQL_TYPE_TIME:                 type = _T("TIME");
                                        p_column->m_columnSize    = 0;
                                        p_column->m_decimalDigits = 0;
                                        break;
    case SQL_BINARY:                    type = _T("BINARY");        break;
    case SQL_VARBINARY:                 type = _T("VARBINARY");     break;
    case SQL_LONGVARBINARY:             type = _T("VARBINARY");     break;
    case SQL_GUID:                      type = _T("UNIQUEIDENTIFIER"); break;
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
  return type;
}

// Gets the USER (current-user) keyword function
XString
SQLInfoSQLServer::GetKEYWORDCurrentUser() const
{
  return _T("CURRENT_USER");
}

// Connects to a default schema in the database/instance
XString
SQLInfoSQLServer::GetSQLDefaultSchema(XString p_user,XString p_schema) const
{
  return _T("ALTER USER ") + p_user + _T(" WITH DEFAULT_SCHEMA = ") + p_schema;
}

// Gets the construction for inline generating a key within an INSERT statement
XString
SQLInfoSQLServer::GetSQLNewSerial(XString p_table,XString p_sequence) const
{
  if(m_useSequences)
  {
    XString sequence(p_sequence);
    if(sequence.IsEmpty() && !p_table.IsEmpty())
    {
      sequence = p_table + _T("_seq");
    }
    return _T("NEXT VALUE FOR ") + sequence;
  }
  else
  {
    // Insert a zero in an IDENTITY column
    return _T("0");
  }
}

// Gets the construction / select for generating a new serial identity
XString
SQLInfoSQLServer::GetSQLGenerateSerial(XString p_table) const
{
  if(m_useSequences)
  {
    return _T("SELECT NEXT VALUE FOR ") + p_table + _T("_seq");
  }
  else
  {
    // Is generated by the IDENTITY system
    return _T("");
  }
}

XString
SQLInfoSQLServer::GetSQLGenerateSequence(XString p_sequence) const
{
  if(m_useSequences)
  {
    return _T("SELECT NEXT VALUE FOR ") + p_sequence;
  }
  else
  {
    // Is generated by the IDENTITY system
    return _T("");
  }
}

// Gets the construction / select for the resulting effective generated serial
XString
SQLInfoSQLServer::GetSQLEffectiveSerial(XString p_identity) const
{
  if(m_useSequences)
  {
    return p_identity;
  }
  else
  {
    return _T("SELECT @@IDENTITY");
  }
}

// Gets the sub transaction commands
XString
SQLInfoSQLServer::GetSQLStartSubTransaction(XString p_savepointName) const
{
  return XString(_T("SAVE TRANSACTION ")) + p_savepointName;
}

XString
SQLInfoSQLServer::GetSQLCommitSubTransaction(XString p_savepointName) const
{
  return XString(_T("COMMIT TRANSACTION ")) + p_savepointName;
}

XString
SQLInfoSQLServer::GetSQLRollbackSubTransaction(XString p_savepointName) const
{
  return XString(_T("ROLLBACK TRANSACTION ")) + p_savepointName;
}

// FROM-Part for a query to select only 1 (one) record / or empty!
XString
SQLInfoSQLServer::GetSQLFromDualClause() const
{
  return _T("");
}

// Get SQL to lock  a table 
XString
SQLInfoSQLServer::GetSQLLockTable(XString p_schema, XString p_tablename, bool p_exclusive,int p_waittime) const
{
  XString query = _T("SELECT TOP(1) 1 FROM ") + p_schema + _T(".") + p_tablename + _T(" WITH ");
  query += p_exclusive ? _T("(TABLOCKX)") : _T("(TABLOCK)");
  if(p_waittime)
  {
    query += _T(" WAITFOR TIMEOUT ");
    XString wait(_T("'00:"));
    if(p_waittime > 3600)
    {
      wait.Format(_T("%2.2d:"),p_waittime / 3600);
      p_waittime %= 3600;
    }
    wait.AppendFormat(_T("%2.2d:%2.2d'"),p_waittime / 60,p_waittime % 60);
    query += wait;
  }
  return query;
}

// Get query to optimize the table statistics
XString
SQLInfoSQLServer::GetSQLOptimizeTable(XString p_schema, XString p_tablename) const
{
  XString query(_T("UPDATE STATISTICS "));
  if (!p_schema.IsEmpty())
  {
    query += p_schema;
    query += _T(".");
  }
  query += p_tablename;
  query += _T(" WITH FULLSCAN");
  return query;
}

// Transform query to select top <n> rows
XString
SQLInfoSQLServer::GetSQLTopNRows(XString p_sql,int p_top,int p_skip /*= 0*/) const
{
  if(p_top > 0 && p_sql.Find(_T("SELECT ")) == 0)
  {
    if(p_skip)
    {
      p_sql.AppendFormat(_T("\n OFFSET %d ROWS FETCH NEXT %d ROWS ONLY"),p_skip,p_top);
    }
    else
    {
      XString selectFirst;
      selectFirst.Format(_T("SELECT TOP %d "),p_top);
      p_sql.Replace(_T("SELECT "),selectFirst);
    }
  }
  return p_sql;
}

// Expand a SELECT with an 'FOR UPDATE' lock clause
XString
SQLInfoSQLServer::GetSelectForUpdateTableClause(unsigned p_lockWaitTime) const
{
  XString clause(" WITH (ROWLOCK,UPDLOCK");
  if(p_lockWaitTime == 0)
  {
    clause += ",READPAST";
  }
  clause += ")";
  return clause;
}

XString
SQLInfoSQLServer::GetSelectForUpdateTrailer(XString p_select,unsigned /*p_lockWaitTime*/) const
{
  return p_select + "\nFOR UPDATE";
}

// Query to perform a keep alive ping
XString
SQLInfoSQLServer::GetPing() const
{
  // Getting the time does a ping
  return _T("SELECT current_timestamp");
}

// Pre- and postfix statements for a bulk import
XString
SQLInfoSQLServer::GetBulkImportPrefix(XString p_schema,XString p_tablename,bool p_identity /*= true*/,bool p_constraints /*= true*/) const
{
  XString sql;

  // Identity Insert
  if(p_identity)
  {
    sql = _T("SET IDENTITY_INSERT ");
    if(!p_schema.IsEmpty())
    {
      sql += QIQ(p_schema) + _T(".");
    }
    sql += QIQ(p_tablename);
    sql += _T(" ON;\n");
  }

  // Statement batch seperator
  if(p_identity && p_constraints)
  {
    sql += _T(SQL_STATEMENT_SEPARATOR);
  }

  // Check constraints uitschakelen
  if(p_constraints)
  {
    sql += _T("ALTER TABLE ");
    if(!p_schema.IsEmpty())
    {
      sql += QIQ(p_schema) + _T(".");
    }
    sql += QIQ(p_tablename);
    sql += _T(" NOCHECK CONSTRAINT ALL");
  }
  return sql;
}

XString
SQLInfoSQLServer::GetBulkImportPostfix(XString p_schema,XString p_tablename,bool p_identity /*= true*/,bool p_constraints /*= true*/) const
{
  XString sql(_T("SET ANSI_PADDING OFF"));
  sql +=  _T(SQL_STATEMENT_SEPARATOR);

  // Identity Insert
  if(p_identity)
  {
    sql = _T("SET IDENTITY_INSERT ");
    if(!p_schema.IsEmpty())
    {
      sql += QIQ(p_schema) + _T(".");
    }
    sql += QIQ(p_tablename);
    sql += _T(" OFF;\n");
  }

  // Statement batch seperator
  if(p_identity && p_constraints)
  {
    sql += _T(SQL_STATEMENT_SEPARATOR);
  }

  // Check constraints uitschakelen
  if(p_constraints)
  {
    sql += _T("ALTER TABLE ");
    if(!p_schema.IsEmpty())
    {
      sql += QIQ(p_schema) + _T(".");
    }
    sql += QIQ(p_tablename);
    sql += _T(" CHECK CONSTRAINT ALL");
  }
  return sql;
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
  s.Replace(_T("'"),_T("''"));
  XString kwoot = GetKEYWORDQuoteCharacter();
  return  kwoot + s + kwoot;
}

// Get date string in engine format
XString
SQLInfoSQLServer::GetSQLDateString(int p_year,int p_month,int p_day) const
{
  XString retval;
  retval.Format(_T("{d '%04d-%02d-%02d' }"),p_year,p_month,p_day);
  return retval;
}

// Get time string in database engine format
XString
SQLInfoSQLServer::GetSQLTimeString(int p_hour,int p_minute,int p_second) const
{
  XString retval;
  retval.Format(_T("{t '%02d:%02d:%02d' }"),p_hour,p_minute,p_second);
  return retval;
}

// Get date-time string in database engine format
XString
SQLInfoSQLServer::GetSQLDateTimeString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  XString retval;
  retval.Format(_T("{ts '%04d-%02d-%02d %02d:%02d:%02d' }"),p_year,p_month,p_day,p_hour,p_minute,p_second);
  return retval;
}

// Get date-time bound parameter string in database format
XString
SQLInfoSQLServer::GetSQLDateTimeBoundString() const
{
  return _T("{ts ?}");
}

// Stripped data for the parameter binding
XString
SQLInfoSQLServer::GetSQLDateTimeStrippedString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  XString retval;
  retval.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"),p_year,p_month,p_day,p_hour,p_minute,p_second);
  return retval;
}

// Makes an catalog identifier string (possibly quoted on both sides)
XString
SQLInfoSQLServer::GetSQLDDLIdentifier(XString p_identifier) const
{
  return QIQ(p_identifier);
}

// Get the name of a temp table (local temporary or global temporary)
XString
SQLInfoSQLServer::GetTempTablename(XString /*p_schema*/,XString p_tablename,bool p_local) const
{
  if(p_local)
  {
    // LOCAL TEMPORARY
    if(p_tablename.GetLength() > 1)
    {
      if(p_tablename.Left(1).Compare(_T("#")))
      {
        p_tablename = _T("#") + p_tablename;
      }
    }
  }
  else
  {
    // GLOBAL TEMPORARY
    if(p_tablename.GetLength() > 2)
    {
      if(p_tablename.Left(2).Compare(_T("##")))
      {
        p_tablename = _T("##") + p_tablename;
      }
    }
  }
  return p_tablename;
}

// Changes to parameters before binding to an ODBC HSTMT handle (returning the At-Exec status)
bool
SQLInfoSQLServer::DoBindParameterFixup(SQLSMALLINT& /*p_dataType*/,SQLSMALLINT& p_sqlDatatype,SQLULEN& p_columnSize,SQLSMALLINT& p_scale,SQLLEN& p_bufferSize,SQLLEN* p_indicator) const
{
  if(*p_indicator == SQL_NULL_DATA)
  {
    // SQLBindParameter only respects the NULL value if the columnsize is not zero
    if(p_columnSize == 0)
    {
      p_columnSize = 1;
    }
    return false;
  }

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
                              return true;
    case SQL_BINARY:          [[fallthrough]];
    case SQL_VARBINARY:       [[fallthrough]];
    case SQL_LONGVARCHAR:     [[fallthrough]];
    case SQL_LONGVARBINARY:   if(p_bufferSize <= 0 || p_bufferSize >= DBMAXCHAR)
                              {
                                // Leave columnSize and bufferSize intact for the driver
                                p_sqlDatatype = SQL_LONGVARBINARY; // Always VARBINARY
                                *p_indicator  = SQL_DATA_AT_EXEC;  // Cannot be NTS!!
                              }
                              return true;
    case SQL_TIME:            [[fallthrough]];
    case SQL_TIMESTAMP:       [[fallthrough]];
    case SQL_TYPE_TIME:       [[fallthrough]];
    case SQL_TYPE_TIMESTAMP:  break;
    case SQL_SS_TIME2:        // Time with sub-second precision
                              p_columnSize = SQL_SS_TIME_COLUMNSIZE;
                              p_scale      = SQL_SS_TIME_MAXSCALE; 
                              break;
    case SQL_NUMERIC:         [[fallthrough]];
    case SQL_DECIMAL:         p_columnSize = SQL_SS_MAXPRECISION;    // Max precision
                              p_bufferSize = SQL_SS_NUMERIC_BUFFER;  // 2 times the SQL_NUMERIC_STRUCT
                              break;
    default:                  break; // DO NOTHING for this datatype (e.g. integer, date, interval types)
  }
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
SQLInfoSQLServer::GetCATALOGMetaTypes(int p_type) const
{
  UNREFERENCED_PARAMETER(p_type);
  return _T("");
}

XString
SQLInfoSQLServer::GetCATALOGDefaultCharset() const
{
  return _T("iso-8859-1");
}

XString
SQLInfoSQLServer::GetCATALOGDefaultCharsetNCV() const
{
  return _T("utf-16");
}

XString
SQLInfoSQLServer::GetCATALOGDefaultCollation() const
{
  return _T("SELECT SERVERPROPERTY('Collation')");
}

// All user defined compound data types
XString
SQLInfoSQLServer::GetCATALOGTypeExists(XString& p_schema,XString& p_typename,bool p_quoted /*= false*/) const
{
  XString sql(_T("SELECT COUNT(*)\n"
                 "  FROM sys.types t\n"
                 "       JOIN sys.schemas s ON s.schema_id = t.schema_id\n"
                 " WHERE t.is_user_defined = 1\n"));
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND s.name = ?\n");
  }
  IdentifierCorrect(p_typename);
  sql += _T("   AND t.name = ?");
  return sql;
}

XString
SQLInfoSQLServer::GetCATALOGTypeList(XString& p_schema,XString& p_pattern,bool p_quoted /*= false*/) const
{
  XString sql(_T("SELECT db_name() AS catalogname\n"
                 "      ,s.name    AS schemaname\n"
                 "      ,t.name    AS typename\n"
                 "      ,CASE t.is_table_type\n"
                 "            WHEN 0 THEN 'D'\n"
                 "            WHEN 1 THEN 'C'\n"
                 "            ELSE ''\n"
                 "       END       AS typeusage\n"
                 "      ,1         AS ordinal\n"
                 "      ,''        AS attribute\n"
                 "      ,bt.name   AS datatype\n"
                 "      ,CASE WHEN t.is_nullable = 1 THEN 0 ELSE 1 END AS notnullable\n"
                 "      ,''        AS defaultvalue\n"
                 "      ,''        AS domaincheck\n"
                 "      ,CASE e.name\n"
                 "            WHEN N'MS_Description' THEN CAST (e.value AS VARCHAR(4000))\n"
                 "            ELSE ''\n"
                 "       END  AS remarks\n"
                 "      ,'<@>'     AS source\n"
                 "  FROM sys.types t\n"
                 "       JOIN sys.schemas s ON s.schema_id = t.schema_id\n"
                 "       LEFT OUTER JOIN sys.types  bt ON t.system_type_id = bt.user_type_id\n"
                 "                                    AND bt.is_user_defined = 0\n"
                 "       LEFT OUTER JOIN sys.extended_properties e ON\n"
                 "                     ((e.major_id  = t.user_type_id OR e.major_id IS null)\n"
                 "                  AND e.minor_id  = 0)\n"
                 " WHERE t.is_user_defined = 1\n"));
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND s.name = ?\n");
  }
  if(!p_pattern.IsEmpty())
  {
    IdentifierCorrect(p_pattern);
    sql += _T("   AND t.name ");
    sql += p_pattern.Find(_T('%')) < 0 ? _T(" = ?\n") : _T(" LIKE ?\n");
  }
  sql += _T("ORDER BY 1,2,3,4");
  return XString();
}

XString
SQLInfoSQLServer::GetCATALOGTypeAttributes(XString& p_schema,XString& p_typename,bool p_quoted /*= false*/) const
{
  // Domain types
  XString sql1(_T("SELECT db_name() AS catalogname\n"
                  "      ,s.name    AS schemaname\n"
                  "      ,t.name    AS typename\n"
                  "      ,'D'       AS typeusage\n"
                  "      ,1         AS ordinal\n"
                  "      ,''        AS attribute\n"
                  "      ,bt.name +\n"
                  "       CASE\n"
                  "          WHEN bt.name IN ('nvarchar', 'nchar', 'varchar', 'char', 'binary', 'varbinary')\n"
                  "               THEN '(' + CASE WHEN t.max_length = -1 THEN 'MAX' ELSE CAST(t.max_length AS VARCHAR) END + ')'\n"
                  "          WHEN bt.name IN ('decimal', 'numeric')\n"
                  "               THEN '(' + CAST(t.precision AS VARCHAR) + ',' + CAST(t.scale AS VARCHAR) + ')'\n"
                  "          ELSE ''\n"
                  "       END AS datatype\n"
                  "      ,CASE WHEN t.is_nullable = 1 THEN 0 ELSE 1 END AS notnullable\n"
                  "      ,''        AS defaultvalue\n"
                  "      ,''        AS domaincheck\n"
                  "      ,CASE e.name\n"
                  "            WHEN N'MS_Description' THEN CAST (e.value AS VARCHAR(4000))\n"
                  "            ELSE ''\n"
                  "       END  AS remarks\n"
                  "      ,'<@>'     AS source\n"
                  "  FROM sys.types t\n"
                  "       JOIN sys.schemas s ON s.schema_id = t.schema_id\n"
                  "       JOIN sys.types  bt ON t.system_type_id = bt.user_type_id AND bt.is_user_defined = 0\n"
                  "       LEFT OUTER JOIN sys.extended_properties e ON\n"
                  "                     ((e.major_id  = t.user_type_id OR e.major_id IS null)\n"
                  "                  AND e.minor_id  = 0)\n"
                  " WHERE t.is_user_defined = 1\n"
                  "   AND t.is_table_type   = 0\n"));
  // Compound types
  XString sql2(_T("SELECT db_name()   AS catalogname\n"
                  "      ,s.name      AS schemaname\n"
                  "      ,tt.name     AS typename\n"
                  "      ,'C'         AS typeusage\n"
                  "      ,c.column_id AS ordinal\n"
                  "      ,c.name      AS attribute\n"
                  "      ,ty.name +\n"
                  "       CASE\n"
                  "           WHEN ty.name IN ('nvarchar', 'nchar', 'varchar', 'char', 'binary', 'varbinary')\n"
                  "                THEN '(' + CASE WHEN c.max_length = -1 THEN 'MAX' ELSE CAST(c.max_length AS VARCHAR) END + ')'\n"
                  "           WHEN ty.name IN ('decimal', 'numeric')\n"
                  "                THEN '(' + CAST(c.precision AS VARCHAR) + ',' + CAST(c.scale AS VARCHAR) + ')'\n"
                  "           ELSE ''\n"
                  "       END AS datatype\n"
                  "      ,CASE WHEN c.is_nullable = 0 THEN 1 ELSE 0 END AS notnull\n"
                  "      ,''         AS defaultvalue\n"
                  "      ,''         AS domaincheck\n"
                  "      ,CASE e.name\n"
                  "            WHEN N'MS_Description' THEN CAST (e.value AS VARCHAR(4000))\n"
                  "            ELSE ''\n"
                  "       END  AS remarks\n"
                  "      ,'<@>'      AS source\n"
                  "  FROM sys.table_types tt\n"
                  "       JOIN sys.schemas s ON s.schema_id = tt.schema_id\n"
                  "       JOIN sys.columns c ON tt.type_table_object_id = c.object_id\n"
                  "       JOIN sys.types  ty ON c.user_type_id = ty.user_type_id\n"
                  "       LEFT OUTER JOIN sys.extended_properties e ON\n"
                  "                     ((e.major_id  = tt.user_type_id OR e.major_id IS null)\n"
                  "                   AND e.minor_id  = c.column_id)\n"
                  " WHERE tt.is_user_defined = 1\n"));
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql1 += _T("   AND s.name = '") + p_schema + _T("'\n");
    sql2 += _T("   AND s.name = ?\n");
  }
  if(!p_typename.IsEmpty())
  {
    IdentifierCorrect(p_typename);
    sql1 += _T("   AND t.name = '") + p_typename + _T("'\n");
    sql2 += _T("   AND t.name = ?\n");
  }
  XString sql = sql1 + _T("UNION ALL\n") + sql2 + _T(" ORDER BY 1,2,3,5");
  return sql;
}

XString
SQLInfoSQLServer::GetCATALOGTypeSource(XString& p_schema,XString& p_typename,bool p_quoted /*= false*/) const
{
  XString sql1(_T("SELECT t.name\n"
                  "      ,1 AS line\n"
                  "      ,'CREATE TYPE [' + s.name + '].[' + t.name + '] FROM ' +\n"
                  "       bt.name +\n"
                  "       CASE\n"
                  "          WHEN bt.name IN ('nvarchar', 'nchar', 'varchar', 'char', 'binary', 'varbinary')\n"
                  "               THEN '(' + CASE WHEN t.max_length = -1 THEN 'MAX' ELSE CAST(t.max_length AS VARCHAR) END + ')'\n"
                  "          WHEN bt.name IN ('decimal', 'numeric')\n"
                  "               THEN '(' + CAST(t.precision AS VARCHAR) + ',' + CAST(t.scale AS VARCHAR) + ')'\n"
                  "          ELSE ''\n"
                  "       END +\n"
                  "       CASE WHEN t.is_nullable = 0 THEN ' NOT NULL' ELSE ' NULL' END AS [CreateTypeStatement]\n"
                  "  FROM sys.types t\n"
                  "       JOIN sys.schemas s ON s.schema_id = t.schema_id\n"
                  "       JOIN sys.types  bt ON t.system_type_id = bt.user_type_id AND bt.is_user_defined = 0\n"
                  " WHERE t.is_user_defined = 1\n"
                  "   AND t.is_table_type   = 0\n"));
  XString sql2(_T("SELECT tt.name\n"
                  "      ,1 AS line\n"
                  "      ,'CREATE TYPE [' + s.name + '].[' + tt.name + '] AS TABLE (' + CHAR(13) +\n"
                  "       STRING_AGG(\n"
                  "           '    [' + c.name + '] ' +\n"
                  "           ty.name +\n"
                  "           CASE\n"
                  "               WHEN ty.name IN ('nvarchar', 'nchar', 'varchar', 'char', 'binary', 'varbinary')\n"
                  "                   THEN '(' + CASE WHEN c.max_length = -1 THEN 'MAX' ELSE CAST(c.max_length AS VARCHAR) END + ')'\n"
                  "               WHEN ty.name IN ('decimal', 'numeric')\n"
                  "                   THEN '(' + CAST(c.precision AS VARCHAR) + ',' + CAST(c.scale AS VARCHAR) + ')'\n"
                  "               ELSE ''\n"
                  "           END +\n"
                  "           CASE WHEN c.is_nullable = 0 THEN ' NOT NULL' ELSE ' NULL' END,\n"
                  "           ',' + CHAR(13)\n"
                  "       ) + CHAR(13) + ')' AS [CreateTableTypeStatement]\n"
                  "  FROM sys.table_types tt\n"
                  "       JOIN sys.schemas s ON s.schema_id = tt.schema_id\n"
                  "       JOIN sys.columns c ON tt.type_table_object_id = c.object_id\n"
                  "       JOIN sys.types ty ON c.user_type_id = ty.user_type_id\n"
                  " WHERE tt.is_user_defined = 1\n"));
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql1 += _T("   AND s.name = '") + p_schema + _T("'\n");
    sql2 += _T("   AND s.name = ?\n");
  }
  if(!p_typename.IsEmpty())
  {
    IdentifierCorrect(p_typename);
    sql1 += _T("   AND t.name = '") + p_typename + _T("'\n");
    sql2 += _T("   AND t.name = ?\n");
  }
  XString sql = sql1 + _T("UNION ALL\n") + sql2;
  sql += _T(" GROUP BY s.name\n"
            "        ,tt.name\n"
            " ORDER BY name,line");
  return sql;
}

XString
SQLInfoSQLServer::GetCATALOGTypeCreate(MUserTypeMap& p_types) const
{
  if(!p_types.empty() && p_types[0].m_source.Compare(_T("<@>")))
  {
    return p_types[0].m_source;
  }
  return _T("");
}

XString
SQLInfoSQLServer::GetCATALOGTypeDrop(XString p_schema,XString p_typename) const
{
  XString sql(_T(" DROP TYPE "));
  if(!p_schema.IsEmpty())
  {
    sql += QIQ(p_schema) + _T(".");
  }
  sql += QIQ(p_typename);
  return sql;
}

// Get SQL to check if a table already exists in the database
XString
SQLInfoSQLServer::GetCATALOGTableExists(XString& p_schema,XString& p_tablename,bool p_quoted /*= false*/) const
{
  XString orgtable(p_tablename);
  XString catalog = GetCatalogAndSchema(p_schema,p_tablename);
  bool istemp = p_tablename.GetAt(0) == '#';

  IdentifierCorrect(p_schema);
  IdentifierCorrect(p_tablename);

  XString query = _T("SELECT COUNT(*)\n")
                  _T("  FROM " + catalog + ".tables  tab\n")
                  _T("      ," + catalog + ".schemas sch\n")
                  _T(" WHERE tab.schema_id = sch.schema_id\n")
                  _T("   AND sch.name = ?\n");
  if(istemp)
  {
    query += _T("   AND tab.object_id = object_id('tempdb..?')");
  }
  else
  {
    query += _T("   AND tab.name = ?");
  }
  return query;
}

XString
SQLInfoSQLServer::GetCATALOGTablesList(XString& p_schema,XString& p_pattern,bool p_quoted /*= false*/) const
{
  return GetCATALOGTableAttributes(p_schema,p_pattern,p_quoted);
}

XString
SQLInfoSQLServer::GetCATALOGTableAttributes(XString& p_schema,XString& p_tablename,bool p_quoted /*= false*/) const
{
  XString query = _T("SELECT db_name() AS table_catalog\n")
                  _T("      ,s.name    AS table_schema\n")
                  _T("      ,o.name    AS table_name\n")
                  _T("      ,CASE o.type\n")
                  _T("            WHEN 'U'  THEN 'TABLE'\n")
                  _T("            WHEN 'TT' THEN 'TABLE'\n")
                  _T("            WHEN 'S'  THEN 'SYSTEM TABLE'\n")
                  _T("                      ELSE 'UNKNOWN'\n")
                  _T("       END AS table_type\n")
                  _T("      ,CASE e.name\n")
                  _T("            WHEN N'MS_Description' THEN CAST (e.value AS VARCHAR(4000))\n")
                  _T("            ELSE ''\n")
                  _T("       END  AS remarks\n")
                  _T("      ,s.name + '.' + o.name as fullname\n")
                  _T("      ,null AS tablespace\n")
                  _T("      ,0    AS temporary\n")
                  _T("  FROM sys.objects o\n")
                  _T("            INNER JOIN sys.schemas s  ON o.schema_id = s.schema_id\n")
                  _T("       LEFT OUTER JOIN sys.extended_properties e ON\n")
                  _T("                     ((e.major_id  = o.object_id OR e.major_id IS null)\n")
                  _T("                   AND e.minor_id  = 0\n")
                  _T("                   AND e.class     = 1)\n")
                  _T(" WHERE o.type IN ('U','TT','S')\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    query += _T("   AND s.name = ?\n");
  }
  if(p_tablename.IsEmpty())
  {
    p_tablename = _T("%");
  }
  else
  {
    IdentifierCorrect(p_tablename);
    query += _T("   AND o.name ");
    query += p_tablename.Find('%') >= 0 ? _T("LIKE") : _T("=");
    query += _T(" ?\n");
  }

  query += _T("UNION ALL\n")
           _T("SELECT db_name()\n")                 // AS table_catalog
           _T("      ,'dbo'\n")                     // AS table_schema
           _T("      ,o.name\n")                    // AS table_name
           _T("      ,'GLOBAL TEMPORARY'\n")        // AS table_type
           _T("      ,null\n")                      // AS remarks
           _T("      ,'#' + o.name as fullname\n")  // AS fullname
           _T("      ,null\n")                      // AS tablespace
           _T("      ,1\n")                         // AS temporary
           _T("  FROM tempdb.sys.objects o\n")
           _T(" WHERE o.type      = 'U'\n")
           _T("   AND o.schema_id = 1\n")
           _T("   AND o.name      LIKE '##") + p_tablename + _T("'\n")
           _T("UNION ALL\n")
           _T("SELECT db_name()\n")                                       // AS table_catalog
           _T("      ,'dbo'\n")                                           // AS table_schema
           _T("      ,substring(o.name,1,charindex('___',o.name) - 1)\n") // AS table_name
           _T("      ,'LOCAL TEMPORARY'\n")                               // AS table_type
           _T("      ,null\n")                                            // AS remarks
           _T("      ,'##' + substring(o.name,1,charindex('___',o.name) - 1)") // as fullname
           _T("      ,null\n")                                            // AS tablespace
           _T("      ,1\n")                                               // AS temporary
           _T("  FROM tempdb.sys.objects o\n")
           _T(" WHERE o.type      = 'U'\n")
           _T("   AND o.schema_id = 1\n")   // 1 = 'dbo'
           _T("   AND o.name LIKE '#") + p_tablename + _T("\\_\\_\\_%' ESCAPE '\\'\n")
           _T(" ORDER BY 1,2,3");
  return query;
}

XString
SQLInfoSQLServer::GetCATALOGTableSynonyms(XString& p_schema,XString& p_tablename,bool p_quoted /*= false*/) const
{
  XString query = _T("SELECT db_name() AS table_catalog\n")
                  _T("      ,s.name    AS table_schema\n")
                  _T("      ,o.name    AS table_name\n")
                  _T("      ,'SYNONYM' AS table_type\n")
                  _T("      ,CASE e.name\n")
                  _T("            WHEN N'MS_Description' THEN CAST (e.value AS VARCHAR(Max))\n")
                  _T("            ELSE ''\n")
                  _T("       END  AS remarks\n")
                  _T("      ,null AS tablespace\n")
                  _T("      ,0    AS temporary\n")
                  _T("  FROM sys.objects o\n")
                  _T("            INNER JOIN sys.schemas s ON o.schema_id = s.schema_id\n")
                  _T("       LEFT OUTER JOIN sys.extended_properties e ON\n")
                  _T("                     ((e.major_id  = o.object_id OR e.major_id IS null)\n")
                  _T("                   AND e.minor_id  = 0\n")
                  _T("                   AND e.class     = 1)\n")
                  _T(" WHERE o.type = 'SN'\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    query += _T("   AND s.name = ?\n");
  }
  if(!p_tablename.IsEmpty())
  {
    IdentifierCorrect(p_tablename);
    query += _T("   AND o.name ");
    query += p_tablename.Find('%') >= 0 ? _T("LIKE") : _T("=");
    query += _T(" ?\n");
  }
  query += _T(" ORDER BY 1,2,3");
  return query;
}

XString
SQLInfoSQLServer::GetCATALOGTableCatalog(XString& p_schema,XString& p_tablename,bool p_quoted /*= false*/) const
{
  XString query = _T("SELECT db_name()      AS table_catalog\n")
                  _T("      ,usr.name       AS schema_name\n")
                  _T("      ,obj.name       AS table_name\n")
                  _T("      ,'SYSTEM TABLE' AS object_type\n")
                  _T("      ,''             AS remarks\n")
                  _T("      ,'master.' + usr.name + '.' + obj.name as fullname\n")
                  _T("      ,db_name() as tablespace\n")
                  _T("      ,0         as temporary\n")
                  _T("  FROM master.sys.sysobjects obj\n")
                  _T("      ,master.sys.sysusers   usr\n")
                  _T(" WHERE xtype IN ('V','S')\n")
                  _T("   AND obj.uid = usr.uid\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    query += _T("   AND usr.name      = ?\n");
  }
  if(!p_tablename.IsEmpty())
  {
    IdentifierCorrect(p_tablename);
    query += _T("   AND obj.name ");
    query += p_tablename.Find('%') >= 0 ? _T("LIKE") : _T("=");
    query += _T(" ?\n");
  }
  query += _T(" ORDER BY 1,2,3");
  return query;
}

XString
SQLInfoSQLServer::GetCATALOGTableCreate(MetaTable& p_table,MetaColumn& /*p_column*/) const
{
  XString sql = _T("CREATE ");
  if (p_table.m_temporary)
  {
    sql += _T("TEMPORARY ");
  }
  sql += _T("TABLE ");
  XString schema = p_table.m_schema;
  XString table  = p_table.m_table;

  if(!schema.IsEmpty())
  {
    sql += QIQ(schema) + _T(".");
  }
  sql += QIQ(table);
  return sql;
}

XString
SQLInfoSQLServer::GetCATALOGTableCreatePostfix(MetaTable& /*p_table*/,MetaColumn& /*p_column*/) const
{
  return _T("");
}

XString
SQLInfoSQLServer::GetCATALOGTableRename(XString p_schema,XString p_tablename,XString p_newname) const
{
  // Beware: No 'TABLE' in the statement
  XString sql(_T("RENAME ") + QIQ(p_schema) + _T(".") + QIQ(p_tablename) + _T(" TO ") + QIQ(p_newname));
  return sql;
}

XString
SQLInfoSQLServer::GetCATALOGTableDrop(XString p_schema,XString p_tablename,bool p_ifExist /*= false*/,bool /*p_restrict = false*/,bool /*p_cascade = false*/) const
{
  XString sql(_T("DROP TABLE ")); 
  if (p_ifExist)
  {
    sql += _T("IF EXISTS ");
  }
  if(!p_schema.IsEmpty())
  {
    sql += QIQ(p_schema) + _T(".");
  }
  sql += QIQ(p_tablename);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL TEMPORARY TABLE FUNCTIONS

XString 
SQLInfoSQLServer::GetCATALOGTemptableCreate(XString /*p_schema*/,XString p_tablename,XString p_select) const
{
  CString sel(p_select);
  sel.MakeLower();
  int pos = sel.Find(_T("from "));

  if(pos > 0)
  {
    if(p_tablename.Left(1).Compare(_T("#")))
    {
      p_tablename = _T("#") + p_tablename;
    }
    sel = p_select.Left(pos) + _T("INTO ") + QIQ(p_tablename) + _T("\n") + p_select.Mid(pos);
    return sel;
  }
  // Don't know how to do this!!
  return _T("");
}

XString 
SQLInfoSQLServer::GetCATALOGTemptableIntoTemp(XString p_schema,XString p_tablename,XString p_select) const
{
  return _T("INSERT INTO #") + p_schema + _T(".") + p_tablename + _T("\n") + p_select;
}

XString 
SQLInfoSQLServer::GetCATALOGTemptableDrop(XString p_schema,XString p_tablename) const
{
  XString tablename = p_schema + _T(".") + p_tablename;
  return _T("DELETE FROM #")    + tablename + _T(";\n")
         _T("<@>\n")
         _T("TRUNCATE TABLE #") + tablename + _T(";\n")
         _T("<@>\n")
         _T("DROP TABLE #")     + tablename + _T(";\n");
}

//////////////////////////////////////////////////////////////////////////
// ALL COLUMN FUNCTIONS

XString 
SQLInfoSQLServer::GetCATALOGColumnExists(XString p_schema,XString p_tablename,XString p_columnname,bool p_quoted /*= false*/) const
{
  IdentifierCorrect(p_schema);
  IdentifierCorrect(p_tablename);
  IdentifierCorrect(p_columnname);

  XString query = _T("SELECT count(*)\n")
                  _T("  FROM sys.sysobjects tab\n")
                  _T("      ,sys.schemas    sch\n")
                  _T("     , dbo.syscolumns att\n")
                  _T(" WHERE tab.name  = '") + p_tablename  + _T("'\n")
                  _T("   AND sch.name  = '") + p_schema     + _T("'")
                  _T("   AND att.name  = '") + p_columnname + _T("'\n")
                  _T("   AND tab.id    = att.id\n")
                  _T("   AND tab.schema_id = sch.schema_id\n");
  return query;
}

XString 
SQLInfoSQLServer::GetCATALOGColumnList(XString& p_schema,XString& p_tablename,bool p_quoted /*= false*/) const
{
  XString columns;
  return GetCATALOGColumnAttributes(p_schema,p_tablename,columns,p_quoted);
}

XString 
SQLInfoSQLServer::GetCATALOGColumnAttributes(XString& p_schema,XString& p_tablename,XString& p_columnname,bool p_quoted /*= false*/) const
{
  // In case of a NON-TEMPORARY table, the standard ODBC driver is better
  if(p_tablename.Left(1).Compare(_T("#")))
  {
    return _T("");
  }
  // GLOBAL AND LOCAL TEMPORARY TABLES ONLY!!!

  // All temporary tables are stored under this schema!
  p_schema = _T("dbo");
  // Globals must always be searched with a like 
  // because of the storage of the global number in the tablename
  if(p_tablename.Left(2).Compare(_T("##")))
  {
    // Local global table must (one '#') must be searched
    p_tablename += _T("%");
  }

  XString query = _T("SELECT db_name() as table_catalog\n")
                  _T("      ,table_schema\n")
                  _T("      ,CASE substring(table_name,1,2)\n")
                  _T("            WHEN '##' THEN table_name\n")
                  _T("                      ELSE '#' + substring(table_name,2,charindex('___',table_name) - 2)\n")
                  _T("       END AS table_name\n")
                  _T("      ,column_name\n")
                  _T("      ,CASE data_type\n")
                  _T("            WHEN 'char'      THEN 1\n")
                  _T("            WHEN 'numeric'   THEN 2\n")
                  _T("            WHEN 'decimal'   THEN 3\n")
                  _T("            WHEN 'int'       THEN 4\n")
                  _T("            WHEN 'smallint'  THEN 5\n")
                  _T("            WHEN 'float'     THEN 6\n")
                  _T("            WHEN 'real'      THEN 7\n")
                  _T("            WHEN 'varchar'   THEN 12\n")
                  _T("            WHEN 'date'      THEN 91\n")
                  _T("            WHEN 'datetime'  THEN 93\n")
                  _T("            WHEN 'datetime2' THEN 93\n")
                  _T("            WHEN 'time'      THEN -154\n")
                  _T("            WHEN 'uniqueidentifier' THEN -11\n")
                  _T("            WHEN 'ntext'     THEN -10\n")
                  _T("            WHEN 'nvarchar'  THEN -9\n")
                  _T("            WHEN 'nchar'     THEN -8\n")
                  _T("            WHEN 'bit'       THEN -7\n")
                  _T("            WHEN 'tinyint'   THEN -6\n")
                  _T("            WHEN 'bigint'    THEN -5\n")
                  _T("            WHEN 'varbinary' THEN -4\n")
                  _T("            WHEN 'binary'    THEN -2\n")
                  _T("            WHEN 'text'      THEN -1\n")
                  _T("       END AS datatype\n")
                  _T("      ,data_type AS type_name\n")
                  _T("      ,CASE data_type\n")
                  _T("            WHEN 'char'      THEN character_maximum_length\n")
                  _T("            WHEN 'numeric'   THEN 38\n")
                  _T("            WHEN 'decimal'   THEN 38\n")
                  _T("            WHEN 'int'       THEN 10\n")
                  _T("            WHEN 'smallint'  THEN 5\n")
                  _T("            WHEN 'float'     THEN 53\n")
                  _T("            WHEN 'real'      THEN 24\n")
                  _T("            WHEN 'varchar'   THEN character_maximum_length\n")
                  _T("            WHEN 'date'      THEN 10\n")
                  _T("            WHEN 'datetime'  THEN 23\n")
                  _T("            WHEN 'datetime2' THEN 27\n")
                  _T("            WHEN 'time'      THEN 16\n")
                  _T("            WHEN 'uniqueidentifier' THEN 36\n")
                  _T("            WHEN 'ntext'     THEN character_maximum_length\n")
                  _T("            WHEN 'nvarchar'  THEN character_maximum_length\n")
                  _T("            WHEN 'nchar'     THEN character_maximum_length\n")
                  _T("            WHEN 'bit'       THEN 1\n")
                  _T("            WHEN 'tinyint'   THEN 3\n")
                  _T("            WHEN 'bigint'    THEN 19\n")
                  _T("            WHEN 'varbinary' THEN character_maximum_length\n")
                  _T("            WHEN 'binary'    THEN character_octet_length\n")
                  _T("            WHEN 'text'      THEN 2147483647\n")
                  _T("       END AS column_size\n")
                  _T("      ,CASE data_type\n")
                  _T("            WHEN 'char'      THEN character_maximum_length\n")
                  _T("            WHEN 'numeric'   THEN 40\n")
                  _T("            WHEN 'decimal'   THEN 40\n")
                  _T("            WHEN 'int'       THEN 4\n")
                  _T("            WHEN 'smallint'  THEN 2\n")
                  _T("            WHEN 'float'     THEN 8\n")
                  _T("            WHEN 'real'      THEN 4\n")
                  _T("            WHEN 'varchar'   THEN character_maximum_length\n")
                  _T("            WHEN 'date'      THEN 6\n")
                  _T("            WHEN 'datetime'  THEN 16\n")
                  _T("            WHEN 'datetime2' THEN 16\n")
                  _T("            WHEN 'time'      THEN 12\n")
                  _T("            WHEN 'uniqueidentifier' THEN 16\n")
                  _T("            WHEN 'ntext'     THEN character_octet_length\n")
                  _T("            WHEN 'nvarchar'  THEN character_octet_length\n")
                  _T("            WHEN 'nchar'     THEN character_octet_length\n")
                  _T("            WHEN 'bit'       THEN 1\n")
                  _T("            WHEN 'tinyint'   THEN 1\n")
                  _T("            WHEN 'bigint'    THEN 8\n")
                  _T("            WHEN 'varbinary' THEN character_octet_length\n")
                  _T("            WHEN 'binary'    THEN character_octet_length\n")
                  _T("            WHEN 'text'      THEN 2147483647\n")
                  _T("       END AS buffer_length\n")
                  _T("      ,numeric_scale AS decimal_digits\n")
                  _T("      ,numeric_precision_radix\n")
                  _T("      ,CASE is_nullable\n")
                  _T("            WHEN 'NO'  THEN 0\n")
                  _T("            WHEN 'YES' THEN 1\n")
                  _T("                       ELSE 2\n")
                  _T("       END AS nullable\n")
                  _T("      ,''  AS remarks\n")
                  _T("      ,column_default\n")
                  _T("      ,CASE data_type\n")
                  _T("            WHEN 'char'      THEN 1\n")
                  _T("            WHEN 'numeric'   THEN 2\n")
                  _T("            WHEN 'decimal'   THEN 3\n")
                  _T("            WHEN 'int'       THEN 4\n")
                  _T("            WHEN 'smallint'  THEN 5\n")
                  _T("            WHEN 'float'     THEN 6\n")
                  _T("            WHEN 'real'      THEN 7\n")
                  _T("            WHEN 'varchar'   THEN 12\n")
                  _T("            WHEN 'date'      THEN 9\n")
                  _T("            WHEN 'datetime'  THEN 9\n")
                  _T("            WHEN 'datetime2' THEN 93\n")
                  _T("            WHEN 'time'      THEN -154\n")
                  _T("            WHEN 'uniqueidentifier' THEN -11\n")
                  _T("            WHEN 'ntext'     THEN -10\n")
                  _T("            WHEN 'nvarchar'  THEN -9\n")
                  _T("            WHEN 'nchar'     THEN -8\n")
                  _T("            WHEN 'bit'       THEN -7\n")
                  _T("            WHEN 'tinyint'   THEN -6\n")
                  _T("            WHEN 'bigint'    THEN -5\n")
                  _T("            WHEN 'varbinary' THEN -3\n")
                  _T("            WHEN 'binary'    THEN -2\n")
                  _T("            WHEN 'text'      THEN -1\n")
                  _T("       END AS sql_datatype\n")
                  _T("      ,CASE data_type\n")
                  _T("            WHEN 'date'      THEN 1\n")
                  _T("            WHEN 'datetime'  THEN 3\n")
                  _T("                             ELSE 0\n")
                  _T("       END AS datatype_sub\n")
                  _T("      ,character_octet_length\n")
                  _T("      ,ordinal_position\n")
                  _T("      ,is_nullable\n")
                  _T("  FROM tempdb.information_schema.columns\n")
                  _T(" WHERE table_schema = ?\n");
  if(!p_tablename.IsEmpty())
  {
    IdentifierCorrect(p_tablename);
    query += _T("   AND table_name ");
    query += p_tablename.Find('%') >= 0 ? _T("LIKE") : _T("=");
    query += _T(" ?\n");
  }
  if(!p_columnname.IsEmpty())
  {
    IdentifierCorrect(p_columnname);
    query += _T("   AND column_name ");
    query += p_columnname.Find('%') >= 0 ? _T("LIKE") : _T("=");
    query += _T(" ?\n");
  }
  query += _T(" ORDER BY 1,2,3\n");

  return query;
}

XString 
SQLInfoSQLServer::GetCATALOGColumnCreate(MetaColumn& p_column) const
{
  XString sql = _T("ALTER TABLE  ") + QIQ(p_column.m_schema) + _T(".") + QIQ(p_column.m_table)  + _T("\n")
                _T("  ADD COLUMN ") + QIQ(p_column.m_column) + _T(" ") + QIQ(p_column.m_typename);
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
  XString sql = _T("ALTER TABLE  ") + QIQ(p_column.m_schema) + _T(".") + QIQ(p_column.m_table)  + _T("\n")
                _T("ALTER COLUMN ") + QIQ(p_column.m_column) + _T(" ") + QIQ(p_column.m_typename);
  p_column.GetPrecisionAndScale(sql);
  p_column.GetNullable(sql);
  p_column.GetDefault(sql);
  return sql;
}

XString 
SQLInfoSQLServer::GetCATALOGColumnRename(XString p_schema,XString p_tablename,XString p_columnname,XString p_newname,XString p_datatype) const
{
  XString sqlCode;
  sqlCode  = _T("ALTER TABLE ")  + QIQ(p_schema)     + _T(".")   + QIQ(p_tablename)  + _T("\n")
             _T("        ADD ")  + QIQ(p_newname)    + _T(" ")   + QIQ(p_datatype)   + _T(";\n");
  sqlCode += _T("UPDATE ")       + QIQ(p_schema)     + _T(".")   + QIQ(p_tablename)  + _T("\n")
             _T("   SET ")       + QIQ(p_newname)    + _T(" = ") + QIQ(p_columnname) + _T(";\n");
  sqlCode += _T("ALTER TABLE ")  + QIQ(p_schema)     + _T(".")   + QIQ(p_tablename)  + _T("\n")
             _T(" DROP COLUMN ") + QIQ(p_columnname) + _T(";");
  return sqlCode;
}

XString 
SQLInfoSQLServer::GetCATALOGColumnDrop(XString p_schema,XString p_tablename,XString p_columnname) const
{
  XString sql(_T("ALTER TABLE ")  + QIQ(p_schema) + _T(".") + QIQ(p_tablename) + _T("\n")
              _T(" DROP COLUMN ") + QIQ(p_columnname));
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL INDICES FUNCTIONS

// All index functions
XString
SQLInfoSQLServer::GetCATALOGIndexExists(XString /*p_schema*/,XString /*p_tablename*/,XString /*p_indexname*/,bool /*p_quoted = false*/) const
{
  return _T("");
}

XString
SQLInfoSQLServer::GetCATALOGIndexList(XString& p_schema,XString& p_tablename,bool p_quoted /*= false*/) const
{
  p_schema.Empty(); // Do not use
  IdentifierCorrect(p_tablename);

  XString query = _T("SELECT idx.name\n")
                  _T("      ,col.name\n")
                  _T("      ,ixk.keyno\n")
                  _T("      ,indexproperty(obj.Id, idx.name, 'IsUnique')\n")
                  _T("      ,indexkey_property(obj.Id, idx.indid, ixk.keyno, 'IsDescending')\n")
                  _T("      ,'' as index_source")
                  _T("  FROM dbo.sysindexes idx\n")
                  _T("      ,dbo.sysindexkeys ixk\n")
                  _T("      ,dbo.sysobjects obj\n")
                  _T("      ,dbo.syscolumns col\n")
                  _T(" WHERE obj.name = ?\n")  // <== PARAMETER
                  _T("   AND obj.Id = idx.id\n")
                  _T("   AND obj.Id = ixk.id\n")
                  _T("   AND idx.indid = ixk.indid\n")
                  _T("   AND ixk.colid = col.colid\n")
                  _T("   AND col.id = obj.Id\n")
                  _T("   AND NOT idx.name LIKE '\\_WA\\_Sys\\_%\\_%' ESCAPE '\\'\n")
//                       "   AND idx.first <> 0\n"
                  _T(" ORDER BY idx.name\n")
                  _T("         ,ixk.keyno\n");
  return query;
}

XString
SQLInfoSQLServer::GetCATALOGIndexAttributes(XString& p_schema,XString& p_tablename,XString& p_indexname,bool p_quoted /*= false*/) const
{
  IdentifierCorrect(p_schema);
  IdentifierCorrect(p_tablename);

  // Getting the tables statistics
  if(p_indexname.Compare(_T("0")) == 0)
  {
    XString query = _T("SELECT db_name() AS catalog_name\n")
                    _T("      ,s.name    AS schema_name\n")
                    _T("      ,t.name    AS table_name\n")
                    _T("      ,1         AS non_unique\n")
                    _T("      ,''        AS drop_qualifier\n")
                    _T("      ,''        AS index_name\n")
                    _T("      ,0         AS type\n") // table_stats
                    _T("      ,0         AS ordinal_position\n")
                    _T("      ,''        AS column_name\n")
                    _T("      ,''        AS asc_or_desc\n")
                    _T("      ,p.rows    AS cardinality\n")
                    _T("      ,SUM(a.data_pages) AS pages\n")
                    _T("      ,''        AS filter\n")
                    _T("  FROM tempdb.sys.tables t\n")
                    _T("       INNER JOIN tempdb.sys.partitions p       ON t.object_id    = p.object_id\n")
                    _T("       INNER JOIN tempdb.sys.allocation_units a ON p.partition_id = a.container_id\n")
                    _T("       INNER JOIN tempdb.sys.schemas s          ON t.schema_id    = s.schema_id\n")
                    _T(" WHERE s.name = ?\n");
    if(!p_tablename.IsEmpty())
    {
      query += _T("   AND t.name ");
      query += p_tablename.Find('%') >= 0 ? _T("LIKE") : _T("=");
      query += _T(" ?\n");
    }
    query += _T(" GROUP BY s.name,t.name,p.rows\n")
             _T(" ORDER BY 1,2,3,4\n");
    return query;
  }

  XString query = _T("SELECT db_name() AS catalog_name\n")
                  _T("      ,s.name    AS table_schema\n")
                  _T("      ,o.name    AS table_name\n")
                  _T("      ,CASE i.is_unique\n")
                  _T("            WHEN 1 THEN 0\n")
                  _T("                   ELSE 1\n")
                  _T("       END       AS non_unique\n")
                  _T("      ,''        AS DROP_qualifier\n")
                  _T("      ,i.name    AS index_name\n")
                  _T("      ,1         AS TYPE\n")
                  _T("      ,x.index_column_id AS ordinal_position\n")
                  _T("      ,c.name    AS column_name\n")
                  _T("      ,CASE x.is_descending_key\n")
                  _T("            WHEN 0 THEN 'A'\n")
                  _T("            WHEN 1 THEN 'D'\n")
                  _T("                   ELSE ''\n")
                  _T("       END       AS asc_or_desc\n")
                  _T("      ,y.rowcnt  AS cardinality\n")
                  _T("      ,y.dpages  AS pages\n")
                  _T("      ,CAST(i.filter_definition AS varchar) AS filter\n")
                  _T("  FROM tempdb.sys.indexes i\n")
                  _T("       INNER JOIN tempdb.sys.objects       o ON  o.object_id = i.object_id\n")
                  _T("       INNER JOIN tempdb.sys.index_columns x ON (x.object_id = i.object_id AND x.index_id  = i.index_id)\n")
                  _T("       INNER JOIN tempdb.sys.columns       c ON (c.object_id = o.object_id AND x.column_id = c.column_id)\n")
                  _T("       INNER JOIN tempdb.sys.schemas       s ON  s.schema_id = o.schema_id\n")
                  _T("       INNER JOIN tempdb.sys.sysindexes    y ON (i.object_id = y.id AND i.name = y.name)\n")
                  _T(" WHERE s.name = ?\n");

  if(!p_tablename.IsEmpty())
  {
    query += _T("   AND o.name ");
    query += p_tablename.Find('%') >= 0 ? _T("LIKE") : _T("=");
    query += _T(" ?\n");
  }
  if(m_filterPKFK)
  {
    // Select indexes for re-creation of the table
    query += _T("   AND i.is_primary_key = 0");
  }

  // In case of a NON-TEMPORARY table, the standard ODBC driver is better
  if(p_tablename.Left(1).Compare(_T("#")))
  {
    query.Replace(_T("tempdb."),_T(""));
  }
  else
  {
    // GLOBAL AND LOCAL TEMPORARY TABLES ONLY!!!
    // All temporary tables are stored under this schema!
    p_schema = _T("dbo");
    // Globals must always be searched with a like 
    // because of the storage of the global number in the tablename
    if(p_tablename.Left(2).Compare(_T("##")))
    {
      // Local global table must (one '#') must be searched
      p_tablename += _T("%");
    }
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
  for(const auto& index : p_indices)
  {
    if(index.m_position == 1)
    {
      XString schema = index.m_schemaName;
      XString idname = index.m_indexName;
      XString tablen = index.m_tableName;

      // New index
      query = _T("CREATE ");
      if(index.m_nonunique == false)
      {
        query += _T("UNIQUE ");
      }
      // If duplicate nulls allowed, it must be a NON-CLUSTERED index
      if(p_duplicateNulls)
      {
        query += _T("NONCLUSTERED ");
      }
      query.AppendFormat(_T("INDEX %s ON "),QIQ(idname).GetString());

      if(!schema.IsEmpty())
      {
        query.AppendFormat(_T("%s."),QIQ(schema).GetString());
      }
      query.AppendFormat(_T("%s("),QIQ(tablen).GetString());
    }
    else
    {
      query += _T(",");
    }
    query += QIQ(index.m_columnName);
    if(index.m_ascending != _T("A"))
    {
      query += _T(" DESC");
    }
  }
  query += _T(")");

  // If duplicate nulls allowed, at least one of the columns must be non-null
  if(p_duplicateNulls)
  {
    query += _T("\n WHERE ");
    for(const auto& index : p_indices)
    {
      if(index.m_position != 1)
      {
        query += _T(" AND ");
      }
      query.AppendFormat(_T("%s IS NOT NULL"), QIQ(index.m_columnName).GetString());
    }
  }
  return query;
}

XString
SQLInfoSQLServer::GetCATALOGIndexDrop(XString p_schema,XString /*p_tablename*/,XString p_indexname) const
{
  XString sql = _T("DROP INDEX ") + QIQ(p_schema) + _T(".") + QIQ(p_indexname);
  return sql;
}

// Get extra filter expression for an index column
XString
SQLInfoSQLServer::GetCATALOGIndexFilter(MetaIndex& /*p_index*/) const
{
  return _T("");
}

//////////////////////////////////////////////////////////////////////////
// ALL PRIMARY KEY FUNCTIONS

XString
SQLInfoSQLServer::GetCATALOGPrimaryExists(XString p_schema,XString p_tablename,bool p_quoted /*= false*/) const
{
  IdentifierCorrect(p_schema);
  IdentifierCorrect(p_tablename);

  XString query = _T("SELECT COUNT(*)\n")
                  _T("  FROM dbo.sysobjects tab\n")
                  _T("      ,dbo.sysobjects con\n")
                  _T("      ,dbo.sysusers   use\n")
                  _T(" WHERE tab.id    = con.parent_obj\n")
                  _T("   AND tab.uid   = use.uid\n")
                  _T("   AND con.xtype = 'PK'\n")
                  _T("   AND con.type  = 'K'")
                  _T("   AND use.name  = '") + p_schema    + _T("'\n")
                  _T("   AND tab.name  = '") + p_tablename + _T("'");
  return query;
}

XString
SQLInfoSQLServer::GetCATALOGPrimaryAttributes(XString& p_schema,XString& p_tablename,bool p_quoted /*= false*/) const
{
  XString query;

  // In case of a NON-TEMPORARY table, the standard ODBC driver is better
  if(p_tablename.Left(1).Compare(_T("#")))
  {
    // REGULAR PERSISTENT TABLES
    IdentifierCorrect(p_schema);
    query = _T("SELECT db_name()     AS table_catalog\n")
            _T("      ,s.name        AS table_schema\n")
            _T("      ,o.name        AS table_name\n")
            _T("      ,c.name        AS column_name\n")
            _T("      ,x.key_ordinal AS key_sequence\n")
            _T("      ,k.name        AS pk_name\n")
            _T("  FROM sys.key_constraints k\n")
            _T("       inner join sys.indexes i       ON  i.object_id = k.parent_object_id\n")
            _T("       inner join sys.objects o       ON  i.object_id = o.object_id\n")
            _T("       inner join sys.schemas s       ON  s.schema_id = o.schema_id\n")
            _T("       inner join sys.index_columns x ON (i.object_id = x.object_id AND i.index_id  = x.index_id)\n")
            _T("       inner join sys.columns c       ON (c.object_id = o.object_id AND x.column_id = c.column_id)\n")
            _T(" WHERE k.type           = 'PK'\n")
            _T("   AND i.is_primary_key = 1\n")
            _T("   AND s.name           = ?\n");
  }
  else
  {
    // GLOBAL AND LOCAL TEMPORARY TABLES ONLY!!!

    // All temporary tables are stored under this schema!
    // From now we search in 'tempdb' under the 'dbo' schema
    p_schema = _T("dbo");
    // Globals must always be searched with a like 
    // because of the storage of the global number in the tablename
    if(p_tablename.Left(2).Compare(_T("##")))
    {
      // Local global table must (one '#') must be searched
      p_tablename += _T("%");
    }
    query = _T("SELECT db_name()     AS table_catalog\n")
            _T("      ,s.name        AS table_schema\n")
            _T("      ,o.name        AS table_name\n")
            _T("      ,c.name        AS column_name\n")
            _T("      ,x.key_ordinal AS key_sequence\n")
            _T("      ,k.name        AS pk_name\n")
            _T("  FROM tempdb.sys.key_constraints k\n")
            _T("       inner join tempdb.sys.indexes i       ON  i.object_id = k.parent_object_id\n")
            _T("       inner join tempdb.sys.objects o       ON  i.object_id = o.object_id\n")
            _T("       inner join tempdb.sys.schemas s       ON  s.schema_id = o.schema_id\n")
            _T("       inner join tempdb.sys.index_columns x ON (i.object_id = x.object_id AND i.index_id  = x.index_id)\n")
            _T("       inner join tempdb.sys.columns c       ON (c.object_id = o.object_id AND x.column_id = c.column_id)\n")
            _T(" WHERE k.type           = 'PK'\n")
            _T("   AND i.is_primary_key = 1\n")
            _T("   AND s.name           = ?\n");
  }
  if(!p_tablename.IsEmpty())
  {
    IdentifierCorrect(p_tablename);
    query += _T("   AND o.name ");
    query += p_tablename.Find('%') >= 0 ? _T("LIKE") : _T("=");
    query += _T(" ?\n");
  }
  return query;
}

XString
SQLInfoSQLServer::GetCATALOGPrimaryCreate(MPrimaryMap& p_primaries) const
{
  XString query(_T("ALTER TABLE "));

  for(const auto& prim : p_primaries)
  {
    if(prim.m_columnPosition == 1)
    {
      XString schema = prim.m_schema;
      XString table  = prim.m_table;
      XString constr = prim.m_constraintName;

      if(!schema.IsEmpty())
      {
        query += QIQ(schema) + _T(".");
      }
      query += QIQ(table) + _T("\n");
      query.AppendFormat(_T("  ADD CONSTRAINT %s\n"),QIQ(constr).GetString());
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
SQLInfoSQLServer::GetCATALOGPrimaryDrop(XString p_schema,XString p_tablename,XString p_constraintname) const
{
  XString sql(_T("ALTER TABLE ") + QIQ(p_schema) + _T(".") + QIQ(p_tablename) + _T("\n")
              _T(" DROP CONSTRAINT ") + QIQ(p_constraintname));
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL FOREIGN KEY FUNCTIONS

XString
SQLInfoSQLServer::GetCATALOGForeignExists(XString p_schema,XString p_tablename,XString p_constraintname,bool p_quoted /*= false*/) const
{
  IdentifierCorrect(p_schema);
  IdentifierCorrect(p_tablename);
  IdentifierCorrect(p_constraintname);

  XString sql;
  sql.Format(_T("SELECT COUNT(*)\n")
             _T("  FROM sys.foreign_keys fok\n")
             _T("      ,sys.schemas      sch\n")
             _T("      ,sys.tables       tab\n")
             _T(" WHERE fok.type = 'F'\n")
             _T("   AND fok.parent_object_id = tab.object_id\n")
             _T("   AND tab.schema_id        = sch.schema_id\n")
             _T("   AND sch.name             = '") + p_schema + _T("'\n")
             _T("   AND tab.name             = '") + p_tablename + _T("'\n")
             _T("   AND fok.name             = '") + p_constraintname + _T("'")
            ,p_schema.GetString()
            ,p_tablename.GetString()
            ,p_constraintname.GetString());
  return sql;
}

XString
SQLInfoSQLServer::GetCATALOGForeignList(XString& p_schema,XString& p_tablename,bool p_quoted /*= false*/) const
{
  XString constraint;
  return GetCATALOGForeignAttributes(p_schema,p_tablename,constraint,p_quoted);
}

XString
SQLInfoSQLServer::GetCATALOGForeignAttributes(XString& p_schema
                                             ,XString& p_tablename
                                             ,XString& p_constraint
                                             ,bool     p_referenced /*=false*/
                                             ,bool     p_quoted     /*=false*/) const
{
  XString query = _T("SELECT db_name()         as primary_catalog_name\n")
                  _T("      ,sch.name          as primary_schema_name\n")
                  _T("      ,pri.name          as primary_table_name\n")
                  _T("      ,pky.name          as primary_key_column\n")
                  _T("      ,db_name()         as foreign_catalog_name\n")
                  _T("      ,sch.name          as foreign_schema_name\n")
                  _T("      ,tab.name          as foreign_table_name\n")
                  _T("      ,col.name          as foreign_key_column\n")
                  _T("      ,fkc.constraint_column_id as key_sequence\n")
                  _T("      ,fok.update_referential_action as update_rule\n")
                  _T("      ,fok.delete_referential_action as delete_rule\n")
                  _T("      ,fok.name          as foreign_key_constraint\n")
                  _T("      ,prk.name          as primary_key_constraint\n")
                  _T("      ,0                 as deferrable\n")
                  _T("      ,1                 as match_option\n")
                  _T("      ,0                 as initially_deferred\n")
                  _T("      ,fok.is_disabled   as disabled\n")
                  _T("  FROM sys.tables tab\n")
                  _T("       INNER JOIN sys.schemas             sch ON sch.schema_id = tab.schema_id\n")
                  _T("       INNER JOIN sys.columns             col ON col.object_id = tab.object_id\n")
                  _T("       INNER JOIN sys.foreign_keys        fok ON tab.object_id = fok.parent_object_id\n")
                  _T("       INNER JOIN sys.foreign_key_columns fkc ON fok.object_id = fkc.constraint_object_id\n")
                  _T("                                             AND col.column_id = fkc.parent_column_id\n")
                  _T("       INNER JOIN sys.tables              pri ON pri.object_id = fkc.referenced_object_id\n")
                  _T("       INNER JOIN sys.columns             pky ON pky.object_id = pri.object_id\n")
                  _T("                                             AND pky.column_id = fkc.referenced_column_id\n")
                  _T("       LEFT OUTER JOIN sys.sysobjects     prk ON pri.object_id = prk.parent_obj \n")
                  _T("                                             AND prk.xtype     = 'PK'\n")
                  _T("                                             AND prk.type      = 'K'\n")
                  _T(" WHERE fok.type = 'F'\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    query += _T("   AND sch.name = ?\n");
  }
  if(!p_tablename.IsEmpty())
  {
    IdentifierCorrect(p_tablename);
    if(p_referenced)
    {
      query += _T("   AND pri.name = ?\n");
    }
    else
    {
      query += _T("   AND tab.name = ?\n");
    }
  }
  if(!p_constraint.IsEmpty())
  {
    IdentifierCorrect(p_constraint);
    if(p_referenced)
    {
      query += _T("   AND prk.name = ?\n");
    }
    else
    {
      query += _T("   AND fok.name = ?\n");
    }
  }
  // Order up to column number
  query += _T(" ORDER BY 1,2,3,4,5,6,7,8,9");

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

  if(!foreign.m_fkSchemaName.IsEmpty())
  {
    table = QIQ(fkschema) + _T(".") + QIQ(table);
  }
  if(!foreign.m_pkSchemaName.IsEmpty())
  {
    primary = QIQ(pkschema) + _T(".") + QIQ(primary);
  }

  // The base foreign key command
  XString query = _T("ALTER TABLE ") + table + _T("\n")
                  _T("  ADD CONSTRAINT ") + QIQ(constraint) + _T("\n")
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
  query += _T(")\n      REFERENCES ") + primary + _T(" (");

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
    case SQL_CASCADE :    query += _T("\n      ON UPDATE NO ACTION");   break; // CASCADE is unreliable
    case SQL_SET_NULL:    query += _T("\n      ON UPDATE SET NULL");    break;
    case SQL_SET_DEFAULT: query += _T("\n      ON UPDATE SET DEFAULT"); break;
    case SQL_NO_ACTION:   query += _T("\n      ON UPDATE NO ACTION");   break;
    case SQL_RESTRICT:    // Fall through
    default:              // In essence: ON UPDATE RESTRICT, but that's already the default
                          break;
  }
  switch(foreign.m_deleteRule)
  {
    case SQL_CASCADE:     query += _T("\n      ON DELETE NO ACTION");   break; // CASCADE is refused by RDBMS !!
    case SQL_SET_NULL:    query += _T("\n      ON DELETE SET NULL");    break;
    case SQL_SET_DEFAULT: query += _T("\n      ON DELETE SET DEFAULT"); break;
    case SQL_NO_ACTION:   query += _T("\n      ON DELETE NO ACTION");   break;
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

  return _T("");
}

XString
SQLInfoSQLServer::GetCATALOGForeignDrop(XString p_schema,XString p_tablename,XString p_constraintname) const
{
  XString sql(_T("ALTER TABLE ") + QIQ(p_schema) + _T(".") + QIQ(p_tablename) + _T("\n")
              _T(" DROP CONSTRAINT ") + QIQ(p_constraintname));
  return sql;
}

//////////////////////////
// All default constraints
XString
SQLInfoSQLServer::GetCATALOGDefaultExists(XString& p_schema,XString& p_tablename,XString& p_column) const
{
  bool p_quoted(true);
  IdentifierCorrect(p_schema);
  IdentifierCorrect(p_tablename);
  IdentifierCorrect(p_column);

  XString sql = _T("SELECT 1\n")
                _T("  FROM sys.default_constraints d\n")
                _T("       inner join sys.objects o ON o.object_id = d.parent_object_id\n")
                _T("       inner join sys.schemas s ON s.schema_id = o.schema_id\n")
                _T("       inner join sys.columns c ON o.object_id = c.object_id AND c.column_id = d.parent_column_id\n")
                _T(" WHERE s.name = '") + p_schema    + _T("'\n")
                _T("   AND o.name = '") + p_tablename + _T("'\n")
                _T("   AND c.name = '") + p_column    + _T("'\n");
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
  bool p_quoted(true);
  IdentifierCorrect(p_schema);

  XString sql = _T("SELECT db_name() AS constraint_catalog\n")
                _T("      ,s.name    AS constraint_schema\n")
                _T("      ,o.name    AS constraint_table\n")
                _T("      ,d.name    AS constraint_name\n")
                _T("      ,c.name    AS constraint_column\n")
                _T("      ,CAST(definition AS VARCHAR(Max)) AS constraint_code\n")
                _T("  FROM sys.default_constraints d\n")
                _T("       inner join sys.objects o ON o.object_id = d.parent_object_id\n")
                _T("       inner join sys.schemas s ON s.schema_id = o.schema_id\n")
                _T("       inner join sys.columns c ON o.object_id = c.object_id AND c.column_id = d.parent_column_id\n")
                _T(" WHERE s.name = ?\n");
  if(!p_tablename.IsEmpty())
  {
    IdentifierCorrect(p_tablename);
    sql += _T("   AND o.name ");
    sql += p_tablename.Find('%') < 0 ? _T("=") : _T("LIKE");
    sql += _T(" ?\n");
  }
  if(!p_column.IsEmpty())
  {
    IdentifierCorrect(p_column);
    sql += _T("   AND c.name = ");
    sql += p_column.Find('%') < 0 ? _T("=") : _T("LIKE");
    sql += _T(" ?\n");
  }
  sql += _T(" ORDER BY 1,2,3,4");

  return sql;
}

XString
SQLInfoSQLServer::GetCATALOGDefaultCreate(XString p_schema,XString p_tablename,XString p_constraint,XString p_column,XString p_code) const
{
  return _T("ALTER TABLE ") + QIQ(p_schema) + _T(".") + QIQ(p_tablename) + _T("\n")
         _T("  ADD CONSTRAINT ") + QIQ(p_constraint) + _T("\n")
         _T("      DEFAULT ") + p_code + _T(" FOR ") + QIQ(p_column);
}

XString
SQLInfoSQLServer::GetCATALOGDefaultDrop(XString p_schema,XString p_tablename,XString p_constraint) const
{
  return _T("ALTER TABLE ") + QIQ(p_schema) + _T(".") + QIQ(p_tablename) + _T("\n")
         _T(" DROP CONSTRAINT ") + QIQ(p_constraint);
}

/////////////////////////
// All check constraints

XString
SQLInfoSQLServer::GetCATALOGCheckExists(XString p_schema,XString p_tablename,XString p_constraint,bool p_quoted /*= false*/) const
{
  IdentifierCorrect(p_schema);
  IdentifierCorrect(p_tablename);
  IdentifierCorrect(p_constraint);

  XString sql = _T("SELECT 1\n")
                _T("  FROM sys.check_constraints c\n")
                _T("       inner join sys.objects o ON o.object_id = c.parent_object_id\n")
                _T("       inner join sys.schemas s ON o.schema_id = s.schema_id\n")
                _T(" WHERE s.name = '") + p_schema     + _T("'\n");
                _T("   AND o.name = '") + p_tablename  + _T("'\n")
                _T("   AND c.name = '") + p_constraint + _T("'");
  return sql;
}

XString
SQLInfoSQLServer::GetCATALOGCheckList(XString p_schema,XString  p_tablename,bool p_quoted /*= false*/) const
{
  XString constraint;
  return GetCATALOGCheckAttributes(p_schema,p_tablename,constraint,p_quoted);
}

XString
SQLInfoSQLServer::GetCATALOGCheckAttributes(XString p_schema,XString p_tablename,XString p_constraint,bool p_quoted /*= false*/) const
{
  IdentifierCorrect(p_schema);

  XString sql = _T("SELECT db_name()    AS catalog\n")
                _T("      ,s.name       AS constraint_schema\n")
                _T("      ,o.name       AS constraint_table\n")
                _T("      ,c.name       AS constraint_name\n")
                _T("      ,c.definition AS constraint_code\n")
                _T("  FROM sys.check_constraints c\n")
                _T("       inner join sys.objects o ON o.object_id = c.parent_object_id\n")
                _T("       inner join sys.schemas s ON o.schema_id = s.schema_id\n")
                _T(" WHERE s.name = ?\n");
  if(!p_tablename.IsEmpty())
  {
    IdentifierCorrect(p_tablename);
    sql += _T("   AND o.name ");
    sql += p_tablename.Find('%') >= 0 ? _T("LIKE") : _T("=");
    sql += _T(" ?\n");
  }
  if(!p_constraint.IsEmpty())
  {
    IdentifierCorrect(p_constraint);
    sql += _T("   AND c.name ");
    sql += p_constraint.Find('%') >= 0 ? _T("LIKE") : _T("=");
    sql += _T(" ?\n");
  }
  sql += _T(" ORDER  BY 1,2,3");
  return sql;
}

XString
SQLInfoSQLServer::GetCATALOGCheckCreate(XString p_schema,XString p_tablename,XString p_constraint,XString p_condition) const
{
  if((p_condition.Left(1) != _T("(")) || (p_condition.Right(1) != _T(")")))
  {
    p_condition = _T("(") + p_condition + _T(")");
  }

  XString sql = _T("ALTER TABLE ");
  if(!p_schema.IsEmpty())
  {
    sql += QIQ(p_schema) + _T(".");
  }
  sql += QIQ(p_tablename) + _T("\n");
  sql += _T("  ADD CONSTRAINT ") + QIQ(p_constraint) + _T("\n");
  sql += _T("      CHECK ") + p_condition;
  return sql;
}

XString
SQLInfoSQLServer::GetCATALOGCheckDrop(XString p_schema,XString p_tablename,XString p_constraint) const
{
  XString sql = _T("ALTER TABLE ");
  if(!p_schema.IsEmpty())
  {
    sql += QIQ(p_schema) + _T(".");
  }
  sql += p_tablename + _T("\n");
  sql += _T("  DROP CONSTRAINT ") + QIQ(p_constraint);

  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL TRIGGER FUNCTIONS

XString
SQLInfoSQLServer::GetCATALOGTriggerExists(XString p_schema, XString p_tablename, XString p_triggername,bool p_quoted /*= false*/) const
{
  IdentifierCorrect(p_schema);
  IdentifierCorrect(p_tablename);
  IdentifierCorrect(p_triggername);

  XString sql;
  sql.Format(_T("SELECT COUNT(*)\n")
             _T("  FROM sys.triggers trg\n")
             _T("      ,sys.objects  tab\n")
             _T("      ,sys.schemas  sch\n")
             _T(" WHERE sch.name      = '%s'\n")
             _T("   AND tab.name      = '%s'\n")
             _T("   AND trg.name      = '%s'\n")
             _T("   AND tab.object_id = trg.parent_id\n")
             _T("   AND tab.schema_id = sch.schema_id")
            ,p_schema.GetString()
            ,p_tablename.GetString()
            ,p_triggername.GetString());
  return sql;
}

XString
SQLInfoSQLServer::GetCATALOGTriggerList(XString& p_schema,XString& p_tablename,bool p_quoted /*= false*/) const
{
  XString triggername;
  return GetCATALOGTriggerAttributes(p_schema,p_tablename,triggername,p_quoted);
}

XString
SQLInfoSQLServer::GetCATALOGTriggerAttributes(XString& p_schema,XString& p_tablename,XString& p_triggername,bool p_quoted /*= false*/) const
{
  XString sql(_T("SELECT ''       AS catalog_name\n")
              _T("      ,sch.name AS schema_name\n")
              _T("      ,tab.name AS table_name\n")
              _T("      ,trg.name AS trigger_name\n")
              _T("      ,trg.name + ' ON TABLE ' + tab.name AS trigger_description\n")
              _T("      ,0        AS position\n")
              _T("      ,0        AS trigger_before\n")
              _T("      ,(SELECT CASE type_desc WHEN 'INSERT' THEN 1 ELSE 0 END\n")
              _T("          FROM sys.trigger_events ev\n")
              _T("         WHERE ev.object_id = trg.object_id) AS trigger_insert\n")
              _T("      ,(SELECT CASE type_desc WHEN 'UPDATE' THEN 1 ELSE 0 END\n")
              _T("          FROM sys.trigger_events ev\n")
              _T("         WHERE ev.object_id = trg.object_id) AS trigger_update\n")
              _T("      ,(SELECT CASE type_desc WHEN 'DELETE' THEN 1 ELSE 0 END\n")
              _T("          FROM sys.trigger_events ev\n")
              _T("         WHERE ev.object_id = trg.object_id) AS trigger_delete\n")
              _T("      ,(SELECT CASE type_desc WHEN 'SELECT' THEN 1 ELSE 0 END\n")
              _T("          FROM sys.trigger_events ev\n")
              _T("         WHERE ev.object_id = trg.object_id) AS trigger_select\n")
              _T("      ,0  AS trigger_session\n")
              _T("   	  ,0  AS trigger_transaction\n")
              _T("      ,0  AS trigger_rollback\n")
              _T("      ,'' AS trigger_referencing\n")
              _T("  	  ,CASE trg.is_disabled WHEN 0 THEN 1 ELSE 0 END AS trigger_enabled\n")
              _T("  	  ,mod.definition AS trigger_source\n")
              _T("  FROM sys.triggers    trg\n")
              _T("      ,sys.sql_modules mod\n")
              _T("      ,sys.objects     tab\n")
              _T("      ,sys.schemas     sch\n")
              _T(" WHERE trg.object_id = mod.object_id\n")
              _T("   AND tab.object_id = trg.parent_id\n")
              _T("   AND tab.schema_id = sch.schema_id\n"));
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND sch.name = ?\n");
  }
  if(!p_tablename.IsEmpty())
  {
    IdentifierCorrect(p_tablename);
    sql += _T("   AND tab.name ");
    sql += p_tablename.Find('%') >= 0 ? _T("LIKE") : _T("=");
    sql += _T(" ?\n");
  }
  if(!p_triggername.IsEmpty())
  {
    IdentifierCorrect(p_triggername);
    sql += _T("   AND trg.name = ?");
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
  XString sql(_T("DROP TRIGGER "));
  if(!p_schema.IsEmpty())
  {
    sql += QIQ(p_schema) + _T(".");
  }
  sql += QIQ(p_triggername);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL SEQUENCE FUNCTIONS

XString
SQLInfoSQLServer::GetCATALOGSequenceExists(XString p_schema, XString p_sequence,bool p_quoted /*= false*/) const
{
  IdentifierCorrect(p_schema);
  IdentifierCorrect(p_sequence);

  XString sql = _T("SELECT COUNT(*)\n")
                _T("  FROM sys.sequences seq\n")
                _T("      ,sys.schemas   sch\n")
                _T(" WHERE sch.object_id = seq.schema_id\n")
                _T("   AND seq.name = '") + p_sequence + _T("'\n")
                _T("   AND sch.name = '") + p_schema   + _T("'");
  return sql;
}

XString
SQLInfoSQLServer::GetCATALOGSequenceList(XString& p_schema,XString& p_pattern,bool p_quoted /*= false*/) const
{
  if(!p_pattern.IsEmpty() && (p_pattern.Find('%') < 0))
  {
    p_pattern = _T("%") + p_pattern + _T("%");
  }
  XString sql = _T("SELECT db_name() AS catalog_name\n")
                _T("      ,sch.name  AS schema_name\n")
                _T("      ,seq.name  AS sequence_name\n")
                _T("      ,CAST(current_value AS BIGINT) AS current_value\n")
                _T("      ,CAST(minimum_value AS BIGINT) AS minimum_value\n")
                _T("      ,CAST(increment     AS BIGINT) AS increment_value\n")
                _T("      ,cache_size AS CACHE\n")
                _T("      ,is_cycling\n")
                _T("      ,0 ordering\n")
                _T("      ,'' as remarks\n")
                _T("  FROM sys.sequences seq\n")
                _T("      ,sys.schemas   sch\n")
                _T(" WHERE sch.schema_id = seq.schema_id\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND sch.name = ?\n");
  }
  if(!p_pattern.IsEmpty())
  {
    IdentifierCorrect(p_pattern);
    sql += _T("   AND seq.name LIKE ?\n");
  }
  sql += _T(" ORDER BY 1,2,3");
  return sql;
}

XString
SQLInfoSQLServer::GetCATALOGSequenceAttributes(XString& p_schema,XString& p_sequence,bool p_quoted /*= false*/) const
{
  XString sql = _T("SELECT ''       AS catalog_name\n")
                _T("      ,sch.name AS schema_name\n")
                _T("      ,seq.name AS sequence_name\n")
                _T("      ,CAST(current_value AS BIGINT) AS current_value\n")
                _T("      ,CAST(minimum_value AS BIGINT) AS minimum_value\n")
                _T("      ,CAST(increment     AS BIGINT) AS increment_value\n")
                _T("      ,cache_size AS CACHE\n")
                _T("      ,is_cycling\n")
                _T("      ,0 ordering\n")
                _T("      ,'' as remarks\n")
                _T("  FROM sys.sequences seq\n")
                _T("      ,sys.schemas   sch\n")
                _T(" WHERE sch.schema_id = seq.schema_id\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND sch.name = ?\n");
  }
  if(!p_sequence.IsEmpty())
  {
    IdentifierCorrect(p_sequence);
    sql += _T("   AND seq.name ");
    sql += p_sequence.Find('%') >= 0 ? _T("LIKE") : _T("=");
    sql += _T(" ?\n");
  }
  sql += _T(" ORDER BY 1,2,3");
  return sql;
}

XString
SQLInfoSQLServer::GetCATALOGSequenceCreate(MetaSequence& p_sequence) const
{
  XString sql(_T("CREATE SEQUENCE "));
  XString schemanm = p_sequence.m_schemaName;

  if(!schemanm.IsEmpty())
  {
    sql += QIQ(schemanm) + _T(".");
  }
  sql += QIQ(p_sequence.m_sequenceName);
  sql.AppendFormat(_T("\n START WITH %d INCREMENT BY 1"), (int) p_sequence.m_currentValue);

  sql += p_sequence.m_cycle ? _T("\n CYCLE") : _T("\n NO CYCLE");
  if (p_sequence.m_cache > 0)
  {
    sql.AppendFormat(_T("\n CACHE %d"),p_sequence.m_cache);
  }
  else
  {
    sql += _T("\n NO CACHE");
  }
  return sql;
}

XString
SQLInfoSQLServer::GetCATALOGSequenceDrop(XString p_schema, XString p_sequence) const
{
  XString sql;
  sql.Format(_T("DROP SEQUENCE IF EXISTS %s.%s"),QIQ(p_schema).GetString(),QIQ(p_sequence).GetString());
  return  sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL VIEW FUNCTIONS

XString 
SQLInfoSQLServer::GetCATALOGViewExists(XString& /*p_schema*/,XString& /*p_viewname*/,bool /*p_quoted = false*/) const
{
  return _T("");
}

XString 
SQLInfoSQLServer::GetCATALOGViewList(XString& p_schema,XString& p_pattern,bool p_quoted /*= false*/) const
{
  return GetCATALOGViewAttributes(p_schema,p_pattern,p_quoted);
}

XString 
SQLInfoSQLServer::GetCATALOGViewAttributes(XString& p_schema,XString& p_viewname,bool p_quoted /*= false*/) const
{
  XString query = _T("SELECT db_name()  AS table_catalog\n")
                  _T("      ,usr.name   AS schema_name\n")
                  _T("      ,obj.name   AS table_name\n")
                  _T("      ,'VIEW'     AS object_type\n")
                  _T("      ,''         AS remarks\n")
                  _T("      ,'master.' + usr.name + '.' + obj.name as fullname\n")
                  _T("      ,db_name() as tablespace\n")
                  _T("      ,0         as temporary\n")
                  _T("  FROM sys.sysobjects obj\n")
                  _T("      ,sys.sysusers   usr\n")
                  _T(" WHERE xtype   = 'V'\n")
                  _T("   AND obj.uid = usr.uid\n");
  if (!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    query += _T("   AND usr.name = ?\n");
  }
  if (!p_viewname.IsEmpty())
  {
    IdentifierCorrect(p_viewname);
    query += _T("   AND obj.name ");
    query += p_viewname.Find('%') >= 0 ? _T("LIKE") : _T("=");
    query += _T(" ?\n");
  }
  query += _T(" ORDER BY 1,2,3");
  return query;
}

XString
SQLInfoSQLServer::GetCATALOGViewText(XString& p_schema,XString& p_viewname,bool p_quoted /*= false*/) const
{
  IdentifierCorrect(p_schema);
  IdentifierCorrect(p_viewname);

  CString sql = _T("SELECT CAST(definition AS varchar(max)) view_text\n")
                _T("  FROM sys.objects     o\n")
                _T("       inner join sys.sql_modules m on m.object_id = o.object_id\n")
                _T("       inner join sys.schemas s     on s.schema_id = o.schema_id\n")
                _T(" WHERE o.type = 'V'\n")
                _T("   AND s.name = '") + p_schema   + _T("'\n")
                _T("   AND o.name = '") + p_viewname + _T("'");
  return sql;
}

XString
SQLInfoSQLServer::GetCATALOGViewCreate(XString p_schema,XString p_viewname,XString p_contents,bool p_ifexists /*= true*/) const
{
  XString sql(_T("CREATE "));
  if(p_ifexists)
  {
    sql += _T("OR ALTER ");
  }
  sql += _T("VIEW ") + QIQ(p_schema) + _T(".") + QIQ(p_viewname) + _T("\n") + p_contents;
  return sql;
}

XString 
SQLInfoSQLServer::GetCATALOGViewRename(XString /*p_schema*/,XString /*p_viewname*/,XString /*p_newname*/)    const
{
  return _T("");
}

XString 
SQLInfoSQLServer::GetCATALOGViewDrop(XString p_schema,XString p_viewname,XString& /*p_precursor*/) const
{
  return _T("DROP VIEW ") + QIQ(p_schema) + _T(".") + QIQ(p_viewname);
}

// All Privilege functions
XString
SQLInfoSQLServer::GetCATALOGTablePrivileges(XString& p_schema,XString& p_tablename) const
{
  bool pattern = p_tablename.Find('%') >= 0;
  bool p_quoted(true);
  IdentifierCorrect(p_schema);
  IdentifierCorrect(p_tablename);

  CString sql = _T("SELECT db_name() AS table_catalog\n")
                _T("      ,s.name    AS table_schema\n")
                _T("      ,o.name    AS table_name\n")
                _T("      ,g.name    AS grantor\n")
                _T("      ,u.name    AS grantee\n")
                _T("      ,p.permission_name AS privilege\n")
                _T("      ,CASE state_desc\n")
                _T("            WHEN 'GRANT' THEN 'NO'\n")
                _T("            WHEN 'GRANT_WITH_GRANT_OPTION' THEN 'YES'\n")
                _T("            ELSE 'NO'\n")
                _T("       END AS grantable\n")
                _T("  FROM sys.database_permissions p\n")
                _T("       inner join sys.objects o ON o.object_id = p.major_id\n")
                _T("       inner join sys.schemas s ON s.schema_id = o.schema_id\n")
                _T("       inner join sys.database_principals u ON u.principal_id = p.grantee_principal_id\n")
                _T("       inner join sys.database_principals g ON g.principal_id = p.grantor_principal_id\n")
                _T(" WHERE p.minor_id = 0\n"); // Only table privileges
  // Add the filter
  if(!p_schema.IsEmpty())
  {
    sql += _T("   AND s.name = '") + p_schema + _T("'\n");
  }
  if(!p_tablename.IsEmpty())
  {
    sql += XString(_T("   AND o.name ")) + (pattern ? _T("LIKE '") : _T("= '")) + p_tablename + _T("'\n");
  }
  sql += _T(" ORDER BY 1,2,3,4,5");
  return sql;
}

XString
SQLInfoSQLServer::GetCATALOGColumnPrivileges(XString& p_schema,XString& p_tablename,XString& p_columnname) const
{
  bool p_quoted(true);
  CString sql = _T("SELECT db_name() AS table_catalog\n")
                _T("      ,s.name    AS table_schema\n")
                _T("      ,o.name    AS table_name\n")
                _T("      ,c.name    AS column_name\n")
                _T("      ,g.name    AS grantor\n")
                _T("      ,u.name    AS grantee\n")
                _T("      ,p.permission_name AS privilege\n")
                _T("      ,CASE state_desc\n")
                _T("            WHEN 'GRANT' THEN 'NO'\n")
                _T("            WHEN 'GRANT_WITH_GRANT_OPTION' THEN 'YES'\n")
                _T("            ELSE 'NO'\n")
                _T("       END AS grantable\n")
                _T("  FROM sys.database_permissions p\n")
                _T("       inner join sys.objects o ON o.object_id = p.major_id\n")
                _T("       inner join sys.schemas s ON s.schema_id = o.schema_id\n")
                _T("       inner join sys.database_principals u ON u.principal_id = p.grantee_principal_id\n")
                _T("       inner join sys.database_principals g ON g.principal_id = p.grantor_principal_id\n")
                _T("       inner join sys.columns c ON c.object_id = o.object_id AND c.column_id = p.minor_id\n")
                _T(" WHERE p.minor_id > 0\n");
  // Add the filter
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND s.name = ?\n");
  }
  if(!p_tablename.IsEmpty())
  {
    IdentifierCorrect(p_tablename);
    sql += _T("   AND o.name = ?\n");
  }
  if(!p_columnname.IsEmpty())
  {
    IdentifierCorrect(p_columnname);
    sql += _T("   AND c.name = ?\n");
  }
  sql += _T(" ORDER BY 1,2,3,4,5,6");
  return sql;
}

XString
SQLInfoSQLServer::GetCATALOGSequencePrivilege(XString& p_schema,XString& p_sequence) const
{
  bool p_quoted(true);
  IdentifierCorrect(p_schema);
  IdentifierCorrect(p_sequence);

  XString sql =
    _T("SELECT db_name() AS sequence_catalog\n")
    _T("      ,s.name    AS sequence_schema\n")
    _T("      ,q.name    AS sequence_name\n")
    _T("      ,p.permission_name AS privilege\n")
    _T("      ,pe.name   AS grantee\n")
    _T("      ,CASE p.state\n")
    _T("            WHEN 'G' THEN 'NO'\n")
    _T("            WHEN 'W' THEN 'YES'\n")
    _T("                     ELSE 'NO'\n")
    _T("       END AS grant_option\n")
    _T("  FROM sys.sequences q\n")
    _T("       inner join sys.objects o ON o.object_id = q.object_id\n")
    _T("       inner join sys.schemas s ON s.schema_id = o.schema_id\n")
    _T("       inner join sys.database_permissions p ON o.object_id = p.major_id\n")
    _T("       inner join sys.database_principals pg ON p.grantor_principal_id = pg.principal_id\n")
    _T("       inner join sys.database_principals pe ON p.grantee_principal_id = pe.principal_id\n")
    _T(" WHERE q.type = 'SO'\n")
    _T("   AND s.name = ?\n");

  if(!p_sequence.IsEmpty())
  {
    sql += _T("   AND q.name ");
    sql += p_sequence.Find('%') < 0 ? _T("=") : _T("LIKE");
    sql += _T(" ?");
  }
  return sql;
}

XString 
SQLInfoSQLServer::GetCATALOGGrantPrivilege(XString p_schema,XString p_objectname,XString p_subObject,XString p_privilege,XString p_grantee,bool p_grantable)
{
  XString sql;

  // Filter out unwanted/not-supported privileges
  if((p_privilege.CompareNoCase(_T("create")) == 0) ||
     (p_privilege.CompareNoCase(_T("index"))  == 0))
  {
    return sql;
  }
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
SQLInfoSQLServer::GetCATALOGRevokePrivilege(XString p_schema,XString p_objectname,XString p_subObject,XString p_privilege,XString p_grantee)
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
SQLInfoSQLServer::GetCATALOGSynonymList(XString& p_schema,XString& p_pattern) const
{
  if(p_pattern.Find('%') < 0)
  {
    p_pattern += _T("%");
  }
  return GetCATALOGSynonymAttributes(p_schema,p_pattern);
}

XString
SQLInfoSQLServer::GetCATALOGSynonymAttributes(XString& p_schema,XString& p_synonym) const
{
  bool p_quoted(true);
  IdentifierCorrect(p_schema);
  IdentifierCorrect(p_synonym);

  XString sql = _T("SELECT db_name()  AS synonym_catalog\n")
                _T("      ,s.name     AS synonym_schema\n")
                _T("      ,y.name     AS synonym_name\n")
                _T("      ,y.base_object_name AS definition\n")
                _T("  FROM sys.synonyms y\n")
                _T("       INNER JOIN sys.schemas s ON s.schema_id = y.schema_id\n")
                _T(" WHERE y.type = 'SN'\n")
                _T("   AND s.name = ?\n");
  if(!p_synonym.IsEmpty())
  {
    sql += _T("   AND y.name ");
    sql += p_synonym.Find('%') >= 0 ? _T("LIKE ?\n") : _T("= ?\n");
  }
  return sql;
}

XString
SQLInfoSQLServer::GetCATALOGSynonymCreate(XString& p_schema,XString& p_synonym,XString p_forObject,bool /*p_private = true*/) const
{
  XString sql = _T("CREATE SYNONYM ") + QIQ(p_schema) + _T(".") + QIQ(p_synonym) + _T(" FOR ") + QIQ(p_forObject);
  return sql;
}

XString
SQLInfoSQLServer::GetCATALOGSynonymDrop(XString& p_schema,XString& p_synonym,bool /*p_private = true*/) const
{
  XString sql = _T("DROP SYNONYM ") + QIQ(p_schema) + _T(".") + QIQ(p_synonym);
  return sql;
}

// For ALL objects
XString
SQLInfoSQLServer::GetCATALOGCommentCreate(XString p_schema,XString p_object,XString p_name,XString p_subObject,XString p_remark) const
{
  XString sql(_T("EXEC sp_addextendedproperty\n"));
  if(p_remark.IsEmpty())
  {
    sql = _T("EXEC sp_dropextendedproperty\n");
  }
  sql += _T("  @name = N'MS_Description',\n");
  if(!p_remark.IsEmpty())
    {
    sql += _T("  @value = N'") + p_remark + _T("',\n");
    }
  p_object.MakeUpper();
  sql += _T("  @level0type = N'SCHEMA', @level0name = N'")               + QIQ(p_schema) + _T("',\n");
  sql += _T("  @level1type = N'") + p_object + _T("', @level1name = N'") + QIQ(p_name)   + _T("',\n");
    if(!p_subObject.IsEmpty())
    {
    sql += _T("  @level2type = N'COLUMN', @level2name = N'") + QIQ(p_subObject) + _T("'");
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
SQLInfoSQLServer::GetPSMProcedureExists(XString p_schema, XString p_procedure,bool p_quoted /*= false*/) const
{
  IdentifierCorrect(p_procedure);

  XString query = _T("SELECT COUNT(*)\n")
                  _T("  FROM sys.objects o\n")
                  _T("       INNER JOIN sys.schemas s ON o.schema_id = s.schema_id\n")
                  _T(" WHERE (type_desc LIKE '%PROCEDURE%' OR type_desc LIKE '%FUNCTION%')\n")
                  _T("   AND o.name = '") + p_procedure + _T("'");

  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    query += _T("\n   AND s.name = '") + p_schema + _T("'");
  }
  return query;
}

XString
SQLInfoSQLServer::GetPSMProcedureList(XString& p_schema,XString p_procedure,bool p_quoted /*= false*/) const
{
  XString query = _T("SELECT db_name() as catalog_name\n")
                  _T("      ,s.name    as schema_name\n")
                  _T("      ,o.name    as procedure_name\n")
                  _T("      ,CASE o.type\n")
                  _T("            WHEN 'P' THEN 1\n")
                  _T("            ELSE 2\n")
                  _T("       END       as procedure_type\n")
                  _T("  FROM sys.objects o\n")
                  _T("       INNER JOIN sys.schemas s ON o.schema_id = s.schema_id\n")
                  _T(" WHERE type IN ('P','FN')\n");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    query += _T("   AND s.name = ?\n");
  }
  if(!p_procedure.IsEmpty())
  {
    IdentifierCorrect(p_procedure);
    query += _T("   AND o.name ");
    query += p_procedure.Find(_T("%")) >= 0 ? _T("LIKE") : _T("=");
    query += _T("?\n");
  }
  query += _T(" ORDER BY 1,2,3");
  return query;
}

XString
SQLInfoSQLServer::GetPSMProcedureAttributes(XString& p_schema,XString& p_procedure,bool p_quoted /*= false*/) const
{
  XString sql = _T("SELECT db_name() as catalog_name\n")
                _T("      ,s.name    as schema_name\n")
                _T("      ,o.name    as procedure_name\n")
                _T("      ,(SELECT Count(*)\n")
                _T("          FROM sys.parameters p1\n")
                _T("         WHERE p1.object_id = o.object_id\n")
                _T("           AND p1.parameter_id > 0\n")
                _T("           AND p1.is_output = 0) AS input_parameters\n")
                _T("      ,(SELECT Count(*)\n")
                _T("          FROM sys.parameters p2\n")
                _T("         WHERE p2.object_id = o.object_id\n")
                _T("           AND p2.parameter_id > 0\n")
                _T("           AND p2.is_output = 1)  AS output_parameters\n")
                _T("      ,0    as result_sets\n")
                _T("      ,NULL AS remarks\n")
                _T("      ,CASE type\n")
                _T("            WHEN 'P'  THEN 1\n")
                _T("            WHEN 'FN' THEN 2\n")
                _T("                      ELSE 3\n")
                _T("       END AS procedure_type\n")
                _T("      ,m.definition AS source\n")
                _T("  FROM sys.objects o\n")
                _T("       INNER JOIN sys.schemas     s ON o.schema_id = s.schema_id\n")
                _T("       INNER JOIN sys.sql_modules m ON m.object_id = o.object_id\n")
                _T(" WHERE o.type IN ('P','FN')\n");

  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND s.name = ?\n");
  }
  if(!p_procedure.IsEmpty())
  {
    IdentifierCorrect(p_procedure);
    sql += _T("   AND o.name ");
    sql += p_procedure.Find('%') >= 0 ? _T("LIKE") : _T("=");
    sql += _T(" ?\n");
  }
  sql += _T("ORDER BY 1,2,3");

  return sql;
}

XString
SQLInfoSQLServer::GetPSMProcedureSourcecode(XString p_schema, XString p_procedure,bool p_quoted /*= false*/) const
{
  IdentifierCorrect(p_procedure);
  XString query = _T("SELECT s.name as source_schema\n")
                  _T("      ,o.name as source_procedure\n")
                  _T("      ,CAST(m.definition AS VARCHAR(max)) as source_code\n")
                  _T("  FROM sys.sql_modules m\n")
                  _T("       INNER JOIN sys.objects o  ON m.object_id = o.object_id\n")
                  _T("       INNER JOIN sys.schemas s  ON o.schema_id = s.schema_id\n")
                  _T("WHERE o.name = '") + p_procedure + _T("'");

  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    query += _T("\n   AND s.name = '") + p_schema + _T("'");
  }
  return query;
}

XString
SQLInfoSQLServer::GetPSMProcedureCreate(MetaProcedure& /*p_procedure*/) const
{
  return _T("");
}

XString
SQLInfoSQLServer::GetPSMProcedureDrop(XString p_schema,XString p_procedure,bool p_function /*=false*/) const
{
  XString object = p_function ? _T("FUNCTION") : _T("PROCEDURE");
  XString sql = _T("DROP ") + object + _T(" IF EXISTS ") + QIQ(p_schema) + _T(".") + QIQ(p_procedure);
  return sql;
}

XString
SQLInfoSQLServer::GetPSMProcedureErrors(XString /*p_schema*/,XString /*p_procedure*/,bool /*p_quoted = false*/) const
{
  // SQL-Server does not support procedure errors
  return _T("");
}

XString
SQLInfoSQLServer::GetPSMProcedurePrivilege(XString& p_schema,XString& p_procedure,bool p_quoted /*= false*/) const
{
  CString sql = _T("SELECT db_name() AS table_catalog\n")
                _T("      ,s.name    AS table_schema\n")
                _T("      ,o.name    AS table_name\n")
                _T("      ,g.name    AS grantor\n")
                _T("      ,u.name    AS grantee\n")
                _T("      ,p.permission_name AS privilege\n")
                _T("      ,CASE state_desc\n")
                _T("            WHEN 'GRANT' THEN 'NO'\n")
                _T("            WHEN 'GRANT_WITH_GRANT_OPTION' THEN 'YES'\n")
                _T("            ELSE 'NO'\n")
                _T("       END AS grantable\n")
                _T("  FROM sys.database_permissions p\n")
                _T("       inner join sys.objects o ON o.object_id = p.major_id\n")
                _T("       inner join sys.schemas s ON s.schema_id = o.schema_id\n")
                _T("       inner join sys.database_principals u ON u.principal_id = p.grantee_principal_id\n")
                _T("       inner join sys.database_principals g ON g.principal_id = p.grantor_principal_id\n")
                _T(" WHERE o.type IN ('P','FN')\n");
  // Add the filter
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("   AND s.name = '") + p_schema + _T("'\n");
  }
  if(!p_procedure.IsEmpty())
  {
    IdentifierCorrect(p_procedure);
    sql += _T("   AND o.name = '") + p_procedure + _T("'\n");
  }
  sql += _T(" ORDER BY 1,2,3,4,5");
  return sql;
}

// And it's parameters
XString
SQLInfoSQLServer::GetPSMProcedureParameters(XString& p_schema,XString& p_procedure,bool p_quoted /*= false*/) const
{
  IdentifierCorrect(p_procedure);
  XString query = _T("SELECT specific_catalog AS procedure_cat\n")
                  _T("      ,specific_schema  AS procedure_schem\n")
                  _T("      ,specific_name    AS procedure_name\n")
                  _T("      ,coalesce(parameter_name,'RETURNS')  AS column_name\n")
                  _T("      ,CASE parameter_mode\n")
                  _T("            WHEN 'IN'    THEN 1\n")
                  _T("            WHEN 'INOUT' THEN 2\n")
                  _T("            WHEN 'OUT'   THEN 3\n")
                  _T("            ELSE 4\n")
                  _T("       END AS column_type\n")
                  _T("      ,CASE data_type\n")
                  _T("            WHEN 'numeric'   THEN 2\n")
                  _T("            WHEN 'varchar'   THEN 1\n")
                  _T("            WHEN 'datetime'  THEN 11\n")
                  _T("            WHEN 'datetime2' THEN 11\n")
                  _T("            WHEN 'int'       THEN 4\n")
                  _T("       END AS data_type\n")
                  _T("      ,data_type AS type_name\n")
                  _T("      ,character_maximum_length AS column_size\n")
                  _T("      ,CASE data_type\n")
                  _T("            WHEN 'numeric'   THEN 36\n")
                  _T("            WHEN 'varchar'   THEN character_maximum_length\n")
                  _T("            WHEN 'datetime'  THEN 20\n")
                  _T("            WHEN 'datetime2' THEN 20\n")
                  _T("            WHEN 'int'       THEN 8\n")
                  _T("       END AS buffer_length\n")
                  _T("      ,numeric_scale           AS decimal_digits\n")
                  _T("      ,numeric_precision_radix AS num_prec_radix\n")
                  _T("      ,3  AS nullable\n")
                  _T("      ,null AS remarks\n")
                  _T("      ,null AS column_default\n")
                  _T("      ,null as sql_data_type\n")
                  _T("      ,null AS sql_datetime_sub\n")
                  _T("      ,character_octet_length AS char_octet_length\n")
                  _T("      ,ordinal_position\n")
                  _T("      ,'YES' AS is_nullable \n")
                  _T("  FROM information_schema.parameters\n")
                  _T(" WHERE specific_name = '") + p_procedure + _T("'");

  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    query += _T("\n   AND specific_schema = '") + p_schema + _T("'");
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
                                   ,XString /*p_domain*/    /*= ""*/
                                   ,XString /*p_asColumn*/  /*= ""*/) const
{
  XString line;
  line.Format(_T("DECLARE @%s AS "),p_variable.GetString());

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
      line += _T(" = ") + p_default;
    }
  }
  line += _T(";\n");
  return line;
}

XString
SQLInfoSQLServer::GetPSMAssignment(XString p_variable,XString p_statement /*=""*/) const
{
  if(p_statement.Find(_T("EXECUTE")) == -1)
  {
    XString line = _T("SET ") + p_variable + _T("=");
    if(!p_statement.IsEmpty())
    {
      line += p_statement;
      line += _T(";");
    }
    return line;
  }
  else
  {
    p_statement.Replace(_T("(EXECUTE"),_T("EXECUTE"));
    p_statement.Replace(_T("<@@@var@@@>"),p_variable);
    p_statement.Replace(_T(";)"),_T(";"));
    return p_statement;
  }
}

XString
SQLInfoSQLServer::GetPSMIF(XString p_condition) const
{
  XString line(_T("IF ("));
  line += p_condition;
  line += _T(") THEN\n")
          _T("  BEGIN\n");
  return line;
}

XString
SQLInfoSQLServer::GetPSMIFElse() const
{
  return _T("  END\n")
         _T("ELSE\n")
         _T("  BEGIN\n");
}

XString
SQLInfoSQLServer::GetPSMIFEnd() const
{
  return _T("  END;\n");
}

XString
SQLInfoSQLServer::GetPSMWhile(XString p_condition) const
{
  return _T("WHILE (") + p_condition + _T(")\n");
}

XString
SQLInfoSQLServer::GetPSMWhileEnd() const
{
  return _T("");
}

XString
SQLInfoSQLServer::GetPSMLOOP() const
{
  return _T("WHILE true\n");
}

XString
SQLInfoSQLServer::GetPSMLOOPEnd() const
{
  return _T("");
}

XString
SQLInfoSQLServer::GetPSMBREAK() const
{
  return _T("BREAK;\n");
}

XString
SQLInfoSQLServer::GetPSMRETURN(XString p_statement /*= ""*/) const
{
  XString line(_T("RETURN"));
  if(!p_statement.IsEmpty())
  {
    line += _T(" ") + p_statement;
  }
  line += _T(";\n");
  return line;
}

XString
SQLInfoSQLServer::GetPSMExecute(XString p_procedure,MParameterMap& p_parameters) const
{
  XString line;
  line.Format(_T("EXECUTE %s "),p_procedure.GetString());
  bool cont = false;

  for(const auto& param : p_parameters)
  {
    // Continuing
    if(cont) line += _T(",");
    cont = true;

    line += _T("@");
    line += param.m_parameter;
    line += _T(" ");
    switch(param.m_columnType)
    {
      case 0: break;
      case SQL_PARAM_OUTPUT:       line += _T("OUTPUT"); break;
      case SQL_PARAM_INPUT_OUTPUT: line += _T("INOUT");  break;
    }
    line += _T(";\n");
  }
  return line;
}

// The CURSOR
XString
SQLInfoSQLServer::GetPSMCursorDeclaration(XString p_cursorname,XString p_select) const
{
  return _T("CURSOR ") + p_cursorname + _T(" IS ") + p_select + _T(";");
}

XString
SQLInfoSQLServer::GetPSMCursorFetch(XString p_cursorname,std::vector<XString>& p_columnnames,std::vector<XString>& p_variablenames) const
{
  XString query = _T("FETCH ") + p_cursorname + _T(" INTO ");

  std::vector<XString>::iterator cNames;
  std::vector<XString>::iterator vNames;
  bool moreThenOne = false;

  for(cNames  = p_columnnames.begin(), vNames  = p_variablenames.begin();
      cNames != p_columnnames.end() && vNames != p_variablenames.end();
      ++cNames,++vNames)
  {
    query += (moreThenOne ? _T(",") : _T("")) + *vNames;
    moreThenOne = true;
  }
  query += _T(";");
  return query;
}

//////////////////////////////////////////////////////////////////////////
// PSM Exceptions

XString
SQLInfoSQLServer::GetPSMExceptionCatchNoData() const
{
  // SQL SERVER has BEGIN TRY .... END TRY -> BEGIN CATCH ... END CATCH
  return _T("");
}

XString
SQLInfoSQLServer::GetPSMExceptionCatch(XString /*p_sqlState*/) const
{
  // SQL SERVER has BEGIN TRY .... END TRY -> BEGIN CATCH ... END CATCH
  return _T("");
}

XString
SQLInfoSQLServer::GetPSMExceptionRaise(XString p_sqlState) const
{
  return _T("RAISE ") + p_sqlState + _T(";\n");
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
  XString query = _T("SELECT rtrim(hostprocess)\n")
                  _T("      ,user\n")
                  _T("      ,rtrim(hostname)\n")    
                  _T("      ,getdata()\n")  // moment
                  _T("      ,''\n")         // remote address
                  _T("      ,''\n")         // remote process name
                  _T("      ,''\n")         // remote process ID
                  _T("  FROM master.dbo.sysprocesses\n")
                  _T(" WHERE hostprocess = host_id()\n")
                  _T("   AND hostname    = host_name()");
  return query;
}

XString
SQLInfoSQLServer::GetSESSIONExists(XString p_sessionID) const
{
  return _T("SELECT COUNT(*)\n")
         _T("  FROM master.dbo.sysprocesses\n")
         _T(" WHERE rtrim(hostprocess) = ") + p_sessionID;
}

XString
SQLInfoSQLServer::GetSESSIONList() const
{
  return GetSESSIONAttributes(_T(""));
}

XString
SQLInfoSQLServer::GetSESSIONAttributes(XString p_sessionID) const
{
  XString sql = _T("SELECT rtrim(hostprocess)\n")
                _T("      ,user\n")
                _T("      ,rtrim(hostname)\n")    
                _T("      ,getdata()\n")  // moment
                _T("      ,''\n")         // remote address
                _T("      ,''\n")         // remote process name
                _T("      ,''\n")         // remote process ID
                _T("  FROM master.dbo.sysprocesses\n");
  if(!p_sessionID.IsEmpty())
  {
    sql += _T("\n WHERE rtrim(hostprocess) = ") + p_sessionID;
  }
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// Transactions

XString
SQLInfoSQLServer::GetSESSIONConstraintsDeferred() const
{
  // SQL-Server cannot defer constraints
  return _T("");
}

XString
SQLInfoSQLServer::GetSESSIONConstraintsImmediate() const
{
  // SQL-Server constraints are always active
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
SQLInfoSQLServer::DoSQLCall(SQLQuery* /*p_query*/,XString& /*p_schema*/,XString& /*p_procedure*/)
{
  // MS-SQLServer supports standard calls
  return nullptr;
}

// Calling a stored function with named parameters, returning a value
SQLVariant*
SQLInfoSQLServer::DoSQLCallNamedParameters(SQLQuery* /*p_query*/,XString& /*p_schema*/,XString& /*p_procedure*/,bool /*p_function = true*/)
{
  // MS-SQLServer supports standard calls with named parameters
  return nullptr;
}

//////////////////////////////////////////////////////////////////////////
//
// PRIVATE
//
//////////////////////////////////////////////////////////////////////////

// Adjust catalog for temporary objects
XString
SQLInfoSQLServer::GetCatalogAndSchema(XString& p_schema,XString& p_table) const
{
  if(p_table.GetLength() > 1 && p_table.Left(1).Compare(_T("#")) == 0)
  {
    // Temp tables are stored in the 'dbo' schema in the 'tempdb' database
    p_schema = _T("dbo");
    return _T("tempdb.sys");
  }
  // Normal object, use the sys schema
  return _T("sys");
}

// End of namespace
}
