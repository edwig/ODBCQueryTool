////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoInformix.cpp
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
#include "SQLInfoInformix.h"
#include "SQLQuery.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{
  
// Constructor. 
SQLInfoInformix::SQLInfoInformix(SQLDatabase* p_database)
                :SQLInfoDB(p_database)
{
}

// Destructor. Does nothing
SQLInfoInformix::~SQLInfoInformix()
{
}

// RDBMS Uses INDENTITY or SEQUENCE interface
void
SQLInfoInformix::SetUseSequences(bool /*p_sequences*/)
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
SQLInfoInformix::GetRDBMSDatabaseType() const
{
  return RDBMS_INFORMIX;
}

// The name of the database vendor
XString
SQLInfoInformix::GetRDBMSVendorName() const
{
  // The name of the database vendor
  return _T("IBM-Informix");
}

// Get the physical database name
XString
SQLInfoInformix::GetRDBMSPhysicalDatabaseName() const
{
  // Oops. We have an INFORMIX CLI bug
  // Versions of the INFORMIX CLI above 2.5 reflect in the connection string
  // only the name of the service of the DNS and not the 'real' database
  // That is why we read the database name from the sysmaster database
  // Beware that this only works for and above INFORMIX 7.x.y
  XString query = _T("SELECT scs_currdb\n")
                  _T("  FROM sysmaster:syssqlcurses\n")
                  _T(" WHERE scs_sessionid = DBINFO('sessionid')");
  SQLQuery qry(m_database);
  qry.DoSQLStatement(query);
  if(qry.GetRecord())
  {
    return qry.GetColumn(1)->GetAsString();
  }
  return XString("");
}

// System catalog is stored in uppercase in the database?
bool
SQLInfoInformix::GetRDBMSIsCatalogUpper() const
{
  return false;
}

// System catalog supports full ISO schemas (same tables per schema)
bool
SQLInfoInformix::GetRDBMSUnderstandsSchemas() const
{
  return false;
}

// Supports database/ODBCdriver comments in SQL
bool
SQLInfoInformix::GetRDBMSSupportsComments() const
{
  // Does not work anymore from and above version 3.34 of KIT 2.70-TC3
  return false;
}

// Database can defer constraints until the end of a transaction
bool
SQLInfoInformix::GetRDBMSSupportsDeferredConstraints() const
{
  // SET CONSTRAINTS DEFERRED is supported
  return true;
}

// Database has ORDER BY with an expression, e.g. ORDER BY UPPER(columnname)
bool
SQLInfoInformix::GetRDBMSSupportsOrderByExpression() const
{
  return false;
}

// Supports the ODBC escape sequence {[?=] CALL procedure (?,?,?)}
bool
SQLInfoInformix::GetRDBMSSupportsODBCCallEscapes() const
{
  return true;
}

// Supports the ODBC call procedure with named parameters
bool
SQLInfoInformix::GetRDBMSSupportsODBCCallNamedParameters() const
{
  return true;
}

// If the database does not support the datatype TIME, it can be implemented as a DECIMAL
bool
SQLInfoInformix::GetRDBMSSupportsDatatypeTime() const
{
  // Time can be implemented as TIME
  return true;
}

// If the database does not support the datatype INTERVAL, it can be implemented as a DECIMAL
bool
SQLInfoInformix::GetRDBMSSupportsDatatypeInterval() const
{
  // Interval supported
  return true;
}

// Supports functions at the place of table columns in create/alter index statement
bool
SQLInfoInformix::GetRDBMSSupportsFunctionalIndexes() const
{
  return false;
}

// Support for "as" in alias statements (FROM clause)
bool
SQLInfoInformix::GetRDBMSSupportsAsInAlias() const
{
  return true;
}

// Foreign key DDL defines the index and cannot reuse already existing ones
bool
SQLInfoInformix::GetRDBMSForeignKeyDefinesIndex() const
{
  // No: you must EXPLICITLY create an index, independent from the foreign key creation.
  return false;
}

// Gets the maximum length of an SQL statement
unsigned long 
SQLInfoInformix::GetRDBMSMaxStatementLength() const
{
  // The current limit of the INFORMIX ODBC driver is MAX_UINT16
  // We limit somewhat lower, just to be on the sure side
  return 64000;
}

// Database must commit DDL commands in a transaction
bool
SQLInfoInformix::GetRDBMSMustCommitDDL() const
{
  return false;
}

// Correct maximum precision,scale for a NUMERIC datatype
void
SQLInfoInformix::GetRDBMSNumericPrecisionScale(SQLULEN& /*p_precision*/, SQLSMALLINT& /*p_scale*/) const
{
  // NO-OP
}

// Maximum for a VARCHAR to be handled without AT-EXEC data. Assume NVARCHAR is half that size!
int
SQLInfoInformix::GetRDBMSMaxVarchar() const
{
  return 255;
}

// Identifier rules differ per RDBMS
bool
SQLInfoInformix::IsIdentifier(XString p_identifier) const
{
  // Cannot be empty and cannot exceed this amount of characters
  if(p_identifier.GetLength() == 0 ||
     p_identifier.GetLength() > (int)GetMaxIdentifierNameLength())
  {
    return false;
  }
  // Must start with one alpha char OR AN UNDERSCORE
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
SQLInfoInformix::GetKEYWORDCurrentTimestamp() const
{
  return _T("current");
}

// String for the current date
XString
SQLInfoInformix::GetKEYWORDCurrentDate() const
{
  return _T("current_date");
}

// Get the concatenation operator
XString
SQLInfoInformix::GetKEYWORDConcatanationOperator() const
{
  return _T("||");
}

// Get quote character for strings
XString
SQLInfoInformix::GetKEYWORDQuoteCharacter() const
{
  return _T("\'");
}

// Get quote character around reserved words as an identifier
XString
SQLInfoInformix::GetKEYWORDReservedWordQuote() const
{
  return _T("\"");
}

// Get default NULL for parameter list input
XString
SQLInfoInformix::GetKEYWORDParameterDefaultNULL() const
{
  // Standard, no definition defines the NULL state
  return _T("");
}

// Parameter is for INPUT and OUTPUT in parameter list
XString
SQLInfoInformix::GetKEYWORDParameterINOUT() const
{
  return _T("");
}

// Parameter is for OUTPUT only in parameter list
XString
SQLInfoInformix::GetKEYWORDParameterOUT() const
{
  return _T("");
}

// Get datatype of the IDENTITY primary key in a Network database
XString
SQLInfoInformix::GetKEYWORDNetworkPrimaryKeyType() const
{
  return _T("serial");
}

// Get datatype for timestamp (year to second)
XString
SQLInfoInformix::GetKEYWORDTypeTimestamp() const
{
  return _T("datetime year to second");
}

// Prefix for a parameter in a stored procedure
XString
SQLInfoInformix::GetKEYWORDParameterPrefix() const
{
  return _T("");
}

// Get select part to add new record identity to a table
// Can be special column like 'OID' or a sequence select
XString
SQLInfoInformix::GetKEYWORDIdentityString(XString& /*p_tablename*/,XString /*p_postfix*/ /*= "_seq"*/) const
{
  // Insert 0 for a SERIAL column
  return _T("0");
}

// Gets the UPPER function
XString
SQLInfoInformix::GetKEYWORDUpper(XString& p_expression) const
{
  return _T("UPPER(") + p_expression + _T(")");
}

// Gets the construction for 1 minute ago
XString
SQLInfoInformix::GetKEYWORDInterval1MinuteAgo() const
{
  return _T("(CURRENT - INTERVAL (1) MINUTE TO MINUTE)");
}

// Gets the Not-NULL-Value statement of the database
XString
SQLInfoInformix::GetKEYWORDStatementNVL(XString& p_test,XString& p_isnull) const
{
  return XString(_T("NVL(")) + p_test + "," + p_isnull + _T(")");
}

// Gets the RDBMS definition of the datatype
XString
SQLInfoInformix::GetKEYWORDDataType(MetaColumn* p_column)
{
  return p_column->m_typename;
}

// Gets the USER (current-user) keyword function
XString
SQLInfoInformix::GetKEYWORDCurrentUser() const
{
  // 'USER' and 'CURRENT_USER' are both valid
  return _T("USER");
}

// Connects to a default schema in the database/instance
XString
SQLInfoInformix::GetSQLDefaultSchema(XString /*p_user*/,XString /*p_schema*/) const
{
  return _T("");
}

// Gets the construction for inline generating a key within an INSERT statement
XString
SQLInfoInformix::GetSQLNewSerial(XString /*p_table*/, XString /*p_sequence*/) const
{
  // Insert a zero in an SERIAL column
  return _T("0");
}

// Gets the construction / select for generating a new serial identity
XString 
SQLInfoInformix::GetSQLGenerateSerial(XString /*p_table*/) const
{
  // Is generated by the database type SERIAL
  return _T("0");
}

XString
SQLInfoInformix::GetSQLGenerateSequence(XString /*p_sequence*/) const
{
  // Not supported by MS-Access
  return _T("");
}

// Gets the construction / select for the resulting effective generated serial
XString 
SQLInfoInformix::GetSQLEffectiveSerial(XString /*p_identity*/) const
{
  return _T("SELECT DBINFO('sqlca.sqlerrd1')\n")
         _T("  FROM systables WHERE tabid = 1");
}

// Gets the sub transaction commands
XString
SQLInfoInformix::GetSQLStartSubTransaction(XString p_savepointName) const
{
  return XString(_T("SAVEPOINT ")) + p_savepointName;
}

XString
SQLInfoInformix::GetSQLCommitSubTransaction(XString p_savepointName) const
{
  return XString(_T("COMMIT TRANSACTION ")) + p_savepointName;
}

XString
SQLInfoInformix::GetSQLRollbackSubTransaction(XString p_savepointName) const
{
  return XString(_T("ROLLBACK TO SAVEPOINT ")) + p_savepointName;
}

// FROM-Part for a query to select only 1 (one) record / or empty!
XString
SQLInfoInformix::GetSQLFromDualClause() const
{
  // Systables entry in systables is guaranteed to always be there!
  return _T(" FROM systables WHERE tabid = 1");
}

// Get SQL to lock  a table 
XString
SQLInfoInformix::GetSQLLockTable(XString /*p_schema*/, XString p_tablename,bool p_exclusive,int /*p_waittime*/) const
{
  XString query = _T("LOCK TABLE ") + p_tablename + _T(" IN ");
  query += p_exclusive ? _T("EXCLUSIVE") : _T("SHARE");
  query += _T(" MODE");
  return query;
}

// Get query to optimize the table statistics
XString
SQLInfoInformix::GetSQLOptimizeTable(XString /*p_schema*/,XString p_tablename) const
{
  XString optim;
  optim = _T("UPDATE STATISTICS LOW  FOR TABLE ") + p_tablename + _T(" DROP DISTRIBUTIONS;\n")
          _T("<@>\n")
          _T("UPDATE STATISTICS HIGH FOR TABLE ") + p_tablename + _T(";\n");
  return optim;
}

// Transform query to select top <n> rows
XString
SQLInfoInformix::GetSQLTopNRows(XString p_sql,int p_top,int p_skip /*= 0*/) const
{
  if(p_top > 0 && p_sql.Find(_T("SELECT ")) == 0)
  {
    // INFORMIX: "SELECT [SKIP <skip>] FIRST <top> ....
    XString selectFirst(_T("SELECT "));
    if(p_skip)
    {
      selectFirst.AppendFormat(_T("SKIP %d "),p_skip);
    }
    selectFirst.AppendFormat(_T("FIRST %d "),p_top);

    p_sql.Replace(_T("SELECT "),selectFirst);
  }
  return p_sql;
}

// Expand a SELECT with an 'FOR UPDATE' lock clause
XString
SQLInfoInformix::GetSelectForUpdateTableClause(unsigned /*p_lockWaitTime*/) const
{
  return "";
}

XString
SQLInfoInformix::GetSelectForUpdateTrailer(XString p_select,unsigned /*p_lockWaitTime*/) const
{
  return p_select + "\nFOR UPDATE";
}

// Query to perform a keep alive ping
XString
SQLInfoInformix::GetPing() const
{
  // Getting the time does a ping
  return _T("SELECT current_timestamp\n")
         _T("  FROM systables\n")
         _T(" WHERE tabid = 1");
}

// Pre- and postfix statements for a bulk import
XString
SQLInfoInformix::GetBulkImportPrefix(XString /*p_schema*/,XString /*p_tablename*/,bool /*p_identity = true*/,bool /*p_constraints = true*/) const
{
  return _T("");
}

XString
SQLInfoInformix::GetBulkImportPostfix(XString /*p_schema*/,XString /*p_tablename*/,bool /*p_identity = true*/,bool /*p_constraints = true*/) const
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
SQLInfoInformix::GetSQLString(const XString& p_string) const
{
  XString str = p_string;
  str.Replace(_T("'"),_T("''"));
  XString kwoot = GetKEYWORDQuoteCharacter();
  return kwoot + str + kwoot;
}

// Get date string in engine format
XString
SQLInfoInformix::GetSQLDateString(int p_year,int p_month,int p_day) const
{
  // Informix used to be depended on the DBFORMAT parameter
  // This form is independent of it's setting!
  XString dateString;
  dateString.Format(_T("DATETIME(%04d-%02d-%02d) YEAR TO DAY"),p_year,p_month,p_day);
  return dateString;
}

// Get time string in database engine format
XString
SQLInfoInformix::GetSQLTimeString(int p_hour,int p_minute,int p_second) const
{
  XString retval;
  retval.Format(_T("DATETIME (%02d:%02d:%02d) HOUR TO SECOND"),p_hour,p_minute,p_second);
  return retval;
}

// Get date-time string in database engine format
XString
SQLInfoInformix::GetSQLDateTimeString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  // Informix used to be depended on the DBFORMAT parameter
  // This form is independent of it's setting!
  XString string;
  string.Format(_T("DATETIME(%04d-%02d-%02d %02d:%02d:%02d) YEAR TO SECOND")
                ,p_day,p_month,p_year
                ,p_hour,p_minute,p_second);
  return string;
}

// Get date-time bound parameter string in database format
XString
SQLInfoInformix::GetSQLDateTimeBoundString() const
{
  return _T("TO_DATE(?,'%d-%m-%Y %H:%M:%S')");
}

// Stripped data for the parameter binding
XString
SQLInfoInformix::GetSQLDateTimeStrippedString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  XString string;
  string.Format(_T("%04d-%02d-%02d %02d:%02d:%02d")
                ,p_day,p_month,p_year
                ,p_hour,p_minute,p_second);
  return string;
}

// Makes an catalog identifier string (possibly quoted on both sides)
XString
SQLInfoInformix::GetSQLDDLIdentifier(XString p_identifier) const
{
  return p_identifier;
}

// Get the name of a temp table (local temporary or global temporary)
XString
SQLInfoInformix::GetTempTablename(XString /*p_schema*/,XString p_tablename,bool /*p_local*/) const
{
  return p_tablename;
}

// Changes to parameters before binding to an ODBC HSTMT handle (returning the At-Exec status)
bool
SQLInfoInformix::DoBindParameterFixup(SQLSMALLINT& /*p_dataType*/,SQLSMALLINT& /*p_sqlDatatype*/,SQLULEN& /*p_columnSize*/,SQLSMALLINT& /*p_scale*/,SQLLEN& /*p_bufferSize*/,SQLLEN* /*p_indicator*/) const
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
SQLInfoInformix::GetCATALOGMetaTypes(int p_type) const
{
  UNREFERENCED_PARAMETER(p_type);
  return _T("");
}

XString
SQLInfoInformix::GetCATALOGDefaultCharset() const
{
  return _T("iso-8859-1");
}

XString
SQLInfoInformix::GetCATALOGDefaultCharsetNCV() const
{
  return _T("UTF-16");
}

XString
SQLInfoInformix::GetCATALOGDefaultCollation() const
{
  return _T("-");
}

// All user defined compound data types
XString
SQLInfoInformix::GetCATALOGTypeExists(XString& /*p_schema*/,XString& /*p_typename*/,bool /*p_quoted = false*/) const
{
  return XString();
}

XString
SQLInfoInformix::GetCATALOGTypeList(XString& /*p_schema*/,XString& /*p_pattern*/,bool /*p_quoted = false*/) const
{
  return XString();
}

XString
SQLInfoInformix::GetCATALOGTypeAttributes(XString& /*p_schema*/,XString& /*p_typename*/,bool /*p_quoted = false*/) const
{
  return XString();
}

XString
SQLInfoInformix::GetCATALOGTypeSource(XString& /*p_schema*/,XString& /*p_typename*/,bool /*p_quoted = false*/) const
{
  return XString();
}

XString
SQLInfoInformix::GetCATALOGTypeCreate(MUserTypeMap& /*p_type*/) const
{
  return XString();
}

XString
SQLInfoInformix::GetCATALOGTypeDrop(XString /*p_schema*/,XString /*p_typename*/) const
{
  return XString();
}

// Get SQL to check if a table already exists in the database
XString
SQLInfoInformix::GetCATALOGTableExists(XString& p_schema,XString& p_tablename,bool /*p_quoted = false*/) const
{
  p_schema.Empty(); // Do not bind as a parameter
  p_tablename.MakeLower();
  XString query = _T("SELECT count(*)\n")
                  _T("  FROM systables\n")
                  _T(" WHERE tabname = ?");
  return query;
}

XString
SQLInfoInformix::GetCATALOGTablesList(XString& p_schema,XString& p_pattern,bool /*p_quoted = false*/) const
{
  return GetCATALOGTableAttributes(p_schema,p_pattern);
}

XString
SQLInfoInformix::GetCATALOGTableAttributes(XString& /*p_schema*/,XString& /*p_tablename*/,bool /*p_quoted = false*/) const
{
  // Getting the temp table status
  return _T("");
}

XString
SQLInfoInformix::GetCATALOGTableSynonyms(XString& p_schema,XString& p_tablename,bool /*p_quoted = false*/) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  XString sql = _T("SELECT Trim(DBINFO('dbname')) AS synonym_catalog\n"
                   "      ,Trim(tab.owner)   AS synonym_schema\n"
                   "      ,Trim(tab.tabname) AS synonym_table\n"
                   "      ,'SYNONYM'         AS object_type\n"
                   "      ,''                AS remarks\n"
                   "      ,Trim(tab.owner) || '.' || Trim(tab.tabname) AS fullname\n"
                   "      ,''                AS storage\n"
                   "      ,0                 AS temporary\n"
                   "  FROM systables tab\n"
                   " WHERE tabtype = 'S'\n");
  if(!p_schema.IsEmpty())
  {
    sql += _T("   AND owner = ?\n");
  }
  if(!p_tablename.IsEmpty())
  {
    sql += _T("   and tabname ");
    sql += p_tablename.Find('%') >= 0 ? _T("LIKE") : _T("=");
    sql += _T(" ?\n");
  }
  sql += _T(" ORDER BY 1,2,3");
  return sql;
}

XString
SQLInfoInformix::GetCATALOGTableCatalog(XString& /*p_schema*/,XString& /*p_tablename*/,bool /*p_quoted = false*/) const
{
  return _T("");
}

XString
SQLInfoInformix::GetCATALOGTableCreate(MetaTable& p_table,MetaColumn& /*p_column*/) const
{
  XString sql = _T("CREATE ");
  if (p_table.m_temporary)
  {
    sql += _T("TEMPORARY ");
  }
  sql += _T("TABLE ") + p_table.m_table;
  return sql;
}

XString
SQLInfoInformix::GetCATALOGTableCreatePostfix(MetaTable& /*p_table*/,MetaColumn& /*p_column*/) const
{
  return _T("");
}

// Rename a database table 
XString
SQLInfoInformix::GetCATALOGTableRename(XString /*p_schema*/,XString p_tablename,XString p_newname) const
{
  // Beware: 'TABLE' keyword in the statement
  XString sql(_T("RENAME TABLE ") + p_tablename + _T(" TO ") + p_newname);
  return sql;
}

XString
SQLInfoInformix::GetCATALOGTableDrop(XString /*p_schema*/,XString p_tablename,bool p_ifExist /*= false*/,bool p_restrict /*= false*/,bool p_cascade /*= false*/) const
{
  XString sql(_T("DROP TABLE "));
  if(p_ifExist)
  {
    sql += _T("IF EXISTS ");
  }
  sql += p_tablename;
  if(p_restrict)
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
SQLInfoInformix::GetCATALOGTemptableCreate(XString /*p_schema*/,XString p_tablename,XString p_select) const
{
  return p_select + _T(" INTO TEMP ") + p_tablename + _T(" WITH NO LOG");
}

XString 
SQLInfoInformix::GetCATALOGTemptableIntoTemp(XString /*p_schema*/,XString p_tablename,XString p_select) const
{
  return p_select + _T(" INTO TEMP ") + p_tablename;
}

XString 
SQLInfoInformix::GetCATALOGTemptableDrop(XString /*p_schema*/,XString p_tablename) const
{
  return _T("DROP TABLE ") + p_tablename;
}

//////////////////////////////////////////////////////////////////////////
// ALL COLUMN FUNCTIONS

XString 
SQLInfoInformix::GetCATALOGColumnExists(XString /*p_schema*/,XString p_tablename,XString p_columnname,bool /*p_quoted /*= false*/) const
{
  p_tablename.MakeLower();
  p_columnname.MakeLower();
  XString query = _T("SELECT count(*)\n")
                  _T("  FROM systables  tab\n")
                  _T("      ,syscolumns col\n")
                  _T(" WHERE tab.tabid = col.tabid\n")
                  _T("   AND tab.tabname = '") + p_tablename  + _T("'\n")
                  _T("   AND col.colname = '") + p_columnname + _T("'");
  return query;
}

XString 
SQLInfoInformix::GetCATALOGColumnList(XString& /*p_schema*/,XString& /*p_tablename*/,bool /*p_quoted /*= false*/) const
{
  // Standard ODBC driver suffices
  return _T("");
}

XString 
SQLInfoInformix::GetCATALOGColumnAttributes(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_columnname*/,bool /*p_quoted /*= false*/) const
{
  // Standard ODBC driver suffices
  return _T("");
}

XString 
SQLInfoInformix::GetCATALOGColumnCreate(MetaColumn& p_column) const
{
  XString sql = _T("ALTER TABLE ")  + p_column.m_table  + _T("\n")
                _T("  ADD COLUMN ") + p_column.m_column + _T(" ") + p_column.m_typename;
  p_column.GetPrecisionAndScale(sql);
  p_column.GetNullable(sql);
  // m_default  not used
  // m_position not used
  // m_length   not used
  // m_remarks  not used
  return sql;
}

XString 
SQLInfoInformix::GetCATALOGColumnAlter(MetaColumn& p_column) const
{
  // The MODIFY keyword is a-typical
  XString sql = _T("ALTER  TABLE  ") + p_column.m_table  + _T("\n")
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
SQLInfoInformix::GetCATALOGColumnRename(XString /*p_schema*/,XString p_tablename,XString p_columnname,XString p_newname,XString /*p_datatype*/) const
{
  XString sql(_T("ALTER  TABLE  ") + p_tablename + _T("\n")
              _T("RENAME ") + p_columnname + _T(" TO ") + p_newname + _T("\n"));
  return sql;
}

XString
SQLInfoInformix::GetCATALOGColumnDrop(XString /*p_schema*/,XString p_tablename,XString p_columnname) const
{
  XString sql(_T("ALTER TABLE ")  + p_tablename + _T("\n")
              _T(" DROP COLUMN ") + p_columnname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL INDICES FUNCTIONS

// All index functions
XString
SQLInfoInformix::GetCATALOGIndexExists(XString /*p_schema*/,XString /*p_tablename*/,XString /*p_indexname*/,bool /*p_quoted = false*/) const
{
  return _T("");
}

XString
SQLInfoInformix::GetCATALOGIndexList(XString& p_schema,XString& p_tablename,bool /*p_quoted = false*/) const
{
  XString query;
  p_schema.Empty(); // Do not use
  p_tablename.MakeLower();

  // Reads all current indices in the database in a list
  // So we can figure out if an index need to be generated
  for(int ind = 1; ind <= INFORMIX_IDS_MAX_INDEX_COLUMNS; ++ind)
  {
    if(!query.IsEmpty())
    {
      query += _T("\nUNION ALL\n");
    }
    query.AppendFormat(_T("SELECT idx.idxname AS index_name\n"
                          "      ,col.colname\n"
                          "      ,%d   AS index_column\n"
                          "      ,CASE WHEN idx.idxtype = 'D' THEN 0\n"
                          "            WHEN idx.idxtype = 'U' THEN 1\n"
                          "            ELSE 0 END AS is_unique\n"
                          "      ,CASE WHEN idx.part%d < 0 THEN 1\n"
                          "            ELSE 0 END AS descending\n"
                          "  FROM sysindexes idx\n"
                          "      ,systables  tab\n"
                          "      ,syscolumns col\n"
                          " WHERE tab.tabid = idx.tabid\n"
                          "   AND tab.tabname = '%s'\n"
                          "   AND col.tabid   = idx.tabid\n"
                          "   AND col.colno   = abs(idx.part%d)\n"
                          "   AND idx.idxname[1] != ' '")
                          ,ind
                          ,ind
                          ,p_tablename.GetString()
                          ,ind);
  }

  // Cannot use bounded parameters
  p_tablename.Empty();
  return query;
}

XString
SQLInfoInformix::GetCATALOGIndexAttributes(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_indexname*/,bool /*p_quoted = false*/) const
{
  return _T("");
}

XString
SQLInfoInformix::GetCATALOGIndexCreate(MIndicesMap& p_indices,bool /*p_duplicateNulls /*= false*/) const
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
      if(!index.m_schemaName.IsEmpty())
      {
        query += index.m_schemaName + _T(".");
      }
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
    if(index.m_ascending != _T("A"))
    {
      query += _T(" DESC");
    }
  }
  query += _T(")");
  return query;
}

XString
SQLInfoInformix::GetCATALOGIndexDrop(XString /*p_schema*/,XString /*p_tablename*/,XString p_indexname) const
{
  XString sql = _T("DROP INDEX ") + p_indexname;
  return sql;
}

// Get extra filter expression for an index column
XString
SQLInfoInformix::GetCATALOGIndexFilter(MetaIndex& /*p_index*/) const
{
  return _T("");
}

//////////////////////////////////////////////////////////////////////////
// ALL PRIMARY KEY FUNCTIONS

XString
SQLInfoInformix::GetCATALOGPrimaryExists(XString /*p_schema*/,XString p_tablename,bool /*p_quoted = false*/) const
{
  p_tablename.MakeLower();
  XString query = _T("SELECT COUNT(*)\n"
                     "  FROM systables      tab\n"
                     "      ,sysconstraints con\n"
                     " WHERE tab.tabid      = con.tabid\n"
                     "   AND tab.tabname    = '") + p_tablename + _T("'\n"
                     "   AND con.constrtype = 'P'");
  return query;
}

XString
SQLInfoInformix::GetCATALOGPrimaryAttributes(XString& /*p_schema*/,XString& /*p_tablename*/,bool /*p_quoted = false*/) const
{
//   p_tablename.MakeLower();
//   XString sql = "SELECT constrname\n"
//                 "      ,'' as indexname\n"
//                 "      ,'Y' as deferrable\n"
//                 "      ,'N' as initially_deferred\n"
//                 "  FROM systables      tab\n"
//                 "      ,sysconstraints con\n"
//                 " WHERE con.constrtype = 'P'\n"
//                 "   AND con.tabid      = tab.tabid\n"
//                 "   AND tab.tabname    = '" + p_tablename + "'";
//   return sql;
  return _T("");
}

XString
SQLInfoInformix::GetCATALOGPrimaryCreate(MPrimaryMap& p_primaries) const
{
  XString query(_T("ALTER TABLE "));
  XString constraintName;

  for(auto& prim : p_primaries)
  {
    if(prim.m_columnPosition == 1)
    {
      if(!prim.m_schema.IsEmpty())
      {
        query += prim.m_schema + _T(".");
      }
      query += prim.m_table + _T("\n");
      query += _T("  ADD CONSTRAINT PRIMARY KEY (");
    }
    else
    {
      query += _T(",");
    }
    query += prim.m_columnName;
    constraintName = prim.m_constraintName;
  }
  query += _T(")\n");
  query += _T("      CONSTRAINT ") + constraintName;

  return query;
}

XString
SQLInfoInformix::GetCATALOGPrimaryDrop(XString /*p_schema*/,XString p_tablename,XString p_constraintname) const
{
  XString sql(_T("ALTER TABLE ") + p_tablename + _T("\n")
              _T(" DROP CONSTRAINT ") + p_constraintname);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL FOREIGN KEY FUNCTIONS

XString
SQLInfoInformix::GetCATALOGForeignExists(XString /*p_schema*/,XString p_tablename,XString p_constraintname,bool /*p_quoted = false*/) const
{
  p_tablename.MakeLower();
  p_constraintname.MakeLower();

  XString sql;
  sql.Format(_T("SELECT COUNT(*)\n"
                "  FROM sysconstraints con\n"
                "      ,systables      tab\n"
                " WHERE tab.tabid      = con.tabid\n"
                "   AND con.constrtype = 'R'\n"
                "   AND tab.tablename  = '%s'\n"
                "   AND con.constrname = '%s'")
               ,p_tablename.GetString()
               ,p_constraintname.GetString());
  return sql;
}

// Get all attributes in order of MetaForeign for ALL FK constraints
XString
SQLInfoInformix::GetCATALOGForeignList(XString& p_schema,XString& p_tablename,bool /*p_quoted = false*/) const
{
  XString constraint;
  return GetCATALOGForeignAttributes(p_schema,p_tablename,constraint);
}

// Get all attributes in order of MetaForeign for 1 FK constraint
XString
SQLInfoInformix::GetCATALOGForeignAttributes(XString& p_schema
                                            ,XString& p_tablename
                                            ,XString& p_constraint
                                            ,bool     p_referenced /*= false*/
                                            ,bool   /*p_quoted       = false*/) const
{
  XString query;
  p_schema.MakeLower();
  p_tablename.MakeLower();
  p_constraint.MakeLower();

  for(int ind = 1;ind <= SQLINFO_MAX_COLUMNS; ++ind)
  {
    XString part;
    part.Format(_T("SELECT trim(DBINFO('dbname')) AS primary_catalog_name\n"
                   "      ,trim(pri.owner)        AS primary_schema_name\n"
                   "      ,trim(pri.tabname)      AS primary_table_name\n"
                   "      ,trim(pcl.colname)      AS primary_column_name\n"
                   "      ,trim(DBINFO('dbname')) AS foreign_catalog_name\n"
                   "      ,trim(tab.owner)        AS foreign_schema_name\n"
                   "      ,trim(tab.tabname)      AS foreign_table_name\n"
                   "      ,trim(col.colname)      AS foreign_column_name\n"
                   "      ,%d                     AS key_sequence\n"
                   "      ,CASE WHEN ref.updrule = 'R' THEN 1\n"
                   "            WHEN ref.updrule = 'C' THEN 0\n"
                   "            WHEN ref.updrule = 'N' THEN 2\n"
                   "            WHEN ref.updrule = 'D' THEN 4\n"
                   "            ELSE 0\n"
                   "       END  AS update_rule\n"
                   "      ,CASE WHEN ref.delrule = 'R' THEN 1\n"
                   "            WHEN ref.delrule = 'C' THEN 0\n"
                   "            WHEN ref.delrule = 'N' THEN 2\n"
                   "            WHEN ref.delrule = 'D' THEN 4\n"
                   "            ELSE 0\n"
                   "       END  AS delete_rule\n"
                   "      ,trim(con.constrname)   AS foreign_key_constraint\n"
                   "      ,trim(pcn.constrname)   AS primary_key_constraint\n"
                   "      ,1    AS deferrable\n"
                   "      ,CASE WHEN ref.matchtype = 'N' THEN 0\n"
                   "            WHEN ref.matchtype = 'P' THEN 1\n"
                   "            ELSE 0\n"
                   "       END  AS match_option\n"
                   "      ,0    AS initially_deferred\n"
                   "      ,1    AS enabled\n"
                   "  FROM sysconstraints con\n"
                   "      ,systables      tab\n"
                   "      ,syscolumns     col\n"
                   "      ,sysreferences  ref\n"
                   "      ,systables      pri\n"
                   "      ,sysindexes     idx\n"
                   "      ,sysconstraints pcn\n"
                   "      ,sysindexes     pix\n"
                   "      ,syscolumns     pcl\n"
                   " WHERE tab.tabid      = con.tabid\n"
                   "   AND con.constrid   = ref.constrid\n"
                   "   AND ref.ptabid     = pri.tabid\n"
                   "   AND con.idxname    = idx.idxname\n"
                   "   AND col.tabid      = tab.tabid\n"
                   "   AND col.colno      = idx.part%d\n"
                   "   AND pcn.tabid      = pri.tabid\n"
                   "   AND pix.idxname    = pcn.idxname\n"
                   "   AND pcl.tabid      = pri.tabid\n"
                   "   AND pcl.colno      = pix.part%d\n"
                   "   and con.constrtype = 'R'\n"
                   "   AND pcn.constrtype = 'P'\n")
                   ,ind
                   ,ind
                   ,ind);
    if(!p_schema.IsEmpty())
    {
      if(p_referenced)
      {
        part += _T("   AND pri.owner      = '") + p_schema + _T("'\n");
      }
      else
      {
        part += _T("   AND tab.owner      = '") + p_schema + _T("'\n");
      }
    }
    if(!p_tablename.IsEmpty())
    {
      if(p_referenced)
      {
        part += _T("   AND pri.tabname    = '") + p_tablename + _T("'\n");
      }
      else
      {
        part += _T("   AND tab.tabname    = '") + p_tablename + _T("'\n");
      }
    }
    if(!p_constraint.IsEmpty())
    {
      if(p_referenced)
      {
        part += _T("    AND pcn.constrname = '") + p_constraint + _T("'\n");
      }
      else
      {
        part += _T("    AND con.constrname = '") + p_constraint + _T("'\n");
      }
    }
    // Add to the query
    if(!query.IsEmpty())
    {
      query += _T("\nUNION ALL\n\n");
    }
    query += part;
  }

  // Add ordering up to column number
  query += _T(" ORDER BY 1,2,3,4,5,6,7,8,9");

  // Do not bind as parameters
  p_schema.Empty();
  p_tablename.Empty();
  p_constraint.Empty();

  return query;
}

XString
SQLInfoInformix::GetCATALOGForeignCreate(MForeignMap& p_foreigns) const
{
  // Get first record
  MetaForeign& foreign = p_foreigns.front();

  // Construct the correct table name
  XString table(foreign.m_fkTableName);
  XString primary(foreign.m_pkTableName);
  if(!foreign.m_fkSchemaName.IsEmpty())
  {
    table = foreign.m_fkSchemaName + _T(".") + table;
  }
  if(!foreign.m_pkSchemaName.IsEmpty())
  {
    primary = foreign.m_pkSchemaName + _T(".") + primary;
  }

  // The base foreign key command
  XString query = _T("ALTER TABLE ") + table + _T("\n")
                  _T("  ADD CONSTRAINT FOREIGN KEY (");

  // Add the foreign key columns
  bool extra = false;
  for(const auto& key : p_foreigns)
  {
    if(extra) query += _T(",");
    query += key.m_fkColumnName;
    extra  = true;
  }

  // Add references primary table
  query += _T(")\n      REFERENCES ") + primary + _T("(");

  // Add the primary key columns
  extra = false;
  for(const auto& key : p_foreigns)
  {
    if(extra) query +=_T( ",");
    query += key.m_pkColumnName;
    extra  = true;
  }
  query += _T(")\n");
  query += _T("      CONSTRAINT ") + foreign.m_foreignConstraint;

  // Add all relevant options
  switch(foreign.m_deferrable)
  {
    case SQL_INITIALLY_DEFERRED:  query += _T("\n      NOVALIDATE"); break;
    default:                      break;
  }
  switch(foreign.m_deleteRule)
  {
    case SQL_CASCADE: query += _T("\n      ON DELETE CASCADE"); break;
    default:          // In essence: ON DELETE RESTRICT, but that's already the default
                      break;
  }
  return query;
}

XString
SQLInfoInformix::GetCATALOGForeignAlter(MForeignMap& p_original, MForeignMap& p_requested) const
{
  // Make sure we have both
  if (p_original.empty() || p_requested.empty())
  {
    return _T("");
  }

  const MetaForeign& original = p_original.front();
  const MetaForeign& requested = p_requested.front();

  // Construct the correct tablename (NO schema)
  XString table(original.m_fkTableName);

  // The base foreign key command
  XString query = _T("ALTER TABLE ") + table + _T("\n")
                  _T("  SET CONSTRAINTS ") + original.m_foreignConstraint + _T("\n");

  // Add all relevant options
  if (original.m_initiallyDeferred != requested.m_initiallyDeferred)
  {
    query += requested.m_initiallyDeferred ? _T("DEFERRED") : _T("IMMEDIATE");
  }
  else if (original.m_enabled != requested.m_enabled)
  {
    query += requested.m_enabled ? _T("ENABLED") : _T("DISABLED");
  }
  return query;
}

XString
SQLInfoInformix::GetCATALOGForeignDrop(XString /*p_schema*/,XString p_tablename,XString p_constraintname) const
{
  XString sql(_T("ALTER TABLE ") + p_tablename + _T("\n")
              _T(" DROP CONSTRAINT ") + p_constraintname);
  return sql;
}

//////////////////////////
// All default constraints
XString
SQLInfoInformix::GetCATALOGDefaultExists(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_column*/) const
{
  return _T("");
}

XString
SQLInfoInformix::GetCATALOGDefaultList(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  return _T("");
}

XString
SQLInfoInformix::GetCATALOGDefaultAttributes(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_column*/) const
{
  return _T("");
}

XString
SQLInfoInformix::GetCATALOGDefaultCreate(XString /*p_schema*/,XString /*p_tablename*/,XString /*p_constraint*/,XString /*p_column*/,XString /*p_code*/) const
{
  return _T("");
}

XString
SQLInfoInformix::GetCATALOGDefaultDrop(XString /*p_schema*/,XString /*p_tablename*/,XString /*p_constraint*/) const
{
  return _T("");
}

/////////////////////////
// All check constraints

XString
SQLInfoInformix::GetCATALOGCheckExists(XString  /*p_schema*/,XString  /*p_tablename*/,XString  /*p_constraint*/,bool /*p_quoted = false*/) const
{
  return _T("");
}

XString
SQLInfoInformix::GetCATALOGCheckList(XString  /*p_schema*/,XString  /*p_tablename*/,bool /*p_quoted = false*/) const
{
  return _T("");
}

XString
SQLInfoInformix::GetCATALOGCheckAttributes(XString  /*p_schema*/,XString  /*p_tablename*/,XString  /*p_constraint*/,bool /*p_quoted = false*/) const
{
  return _T("");
}

XString
SQLInfoInformix::GetCATALOGCheckCreate(XString  /*p_schema*/,XString  /*p_tablename*/,XString  /*p_constraint*/,XString /*p_condition*/) const
{
  return _T("");
}

XString
SQLInfoInformix::GetCATALOGCheckDrop(XString  /*p_schema*/,XString  /*p_tablename*/,XString  /*p_constraint*/) const
{
  return _T("");
}

//////////////////////////////////////////////////////////////////////////
// ALL TRIGGER FUNCTIONS

XString
SQLInfoInformix::GetCATALOGTriggerExists(XString /*p_schema*/, XString /*p_tablename*/, XString /*p_triggername*/,bool /*p_quoted = false*/) const
{
  return _T("");
}

XString
SQLInfoInformix::GetCATALOGTriggerList(XString& p_schema,XString& p_tablename,bool /*p_quoted = false*/) const
{
  XString triggername;
  return GetCATALOGTriggerAttributes(p_schema,p_tablename,triggername);
}

XString
SQLInfoInformix::GetCATALOGTriggerAttributes(XString& p_schema,XString& p_tablename,XString& p_triggername,bool /*p_quoted = false*/) const
{
  p_schema.MakeLower();
  p_tablename.MakeLower();
  p_triggername.MakeLower();
  XString sql;
  sql = _T("SELECT Trim(DBINFO('dbname')) AS trigger_catalog\n"
           "      ,Trim(tri.owner)        AS trigger_schema\n"
           "      ,trim(tab.tabname)      AS trigger_table\n"
           "      ,Trim(tri.trigname)     AS trigger_name\n"
           "      ,''    AS remarks\n"
           "      ,0     AS position\n"
           "      ,0     AS before\n"
           "      ,CASE tri.event WHEN 'I' THEN 1 ELSE 0 END AS trigger_insert\n"
           "      ,CASE tri.event WHEN 'U' THEN 1 ELSE 0 END AS trigger_update\n"
           "      ,CASE tri.event WHEN 'D' THEN 1 ELSE 0 END AS trigger_delete\n"
           "      ,0     AS trigger_select\n"
           "      ,0     AS trigger_session\n"
           "      ,0     AS trigger_transaction\n"
           "      ,0     AS trigger_rollback\n"
           "      ,'old as ' || tri.old || ' new as ' || tri.new  AS trigger_referencing\n"
           "      ,1     AS enabled\n"
           "      ,'<@>' AS source\n"
           "  FROM systriggers tri\n"
           "      ,systables   tab\n"
           " WHERE tab.tabid = tri.tabid\n");
  if(!p_schema.IsEmpty())
  {
    sql += _T("   AND tri.owner = ?\n");
  }
  if(!p_tablename.IsEmpty())
  {
    sql += _T("   AND tab.tabname = ?\n");
  }
  if(!p_triggername.IsEmpty())
  {
    sql += _T("   AND tri.trigname ");
    sql += p_triggername.Find('%') >= 0 ? _T("LIKE") : _T("=");
    sql += _T(" ?\n");
  }
  sql += _T(" ORDER BY 1,2,3,4");
  return sql;
}

XString
SQLInfoInformix::GetCATALOGTriggerCreate(MetaTrigger& /*p_trigger*/) const
{
  return _T("");
}

XString
SQLInfoInformix::GetCATALOGTriggerDrop(XString /*p_schema*/, XString /*p_tablename*/, XString /*p_triggername*/) const
{
  return _T("");
}

//////////////////////////////////////////////////////////////////////////
// ALL SEQUENCE FUNCTIONS

XString
SQLInfoInformix::GetCATALOGSequenceExists(XString p_schema, XString p_sequence,bool /*p_quoted = false*/) const
{
  p_schema.MakeLower();
  p_sequence.MakeLower();
  XString sql = _T("SELECT COUNT(*)\n"
                   "  FROM syssequences seq\n"
                   "      ,sysdomains   dom\n"
                   " WHERE dom.id    = seq.id\n"
                   "   AND dom.owner = '") + p_schema   + _T("'\n"
                   "   AND dom.name  = '") + p_sequence + _T("'\n");
  return sql;
}

XString
SQLInfoInformix::GetCATALOGSequenceList(XString& p_schema,XString& p_pattern,bool p_quoted /*= false*/) const
{
  return GetCATALOGSequenceAttributes(p_schema,p_pattern,p_quoted);
}

XString
SQLInfoInformix::GetCATALOGSequenceAttributes(XString& p_schema,XString& p_sequence,bool /*p_quoted = false*/) const
{
  p_schema.MakeLower();
  p_sequence.MakeLower();
  XString sql = _T("SELECT trim(DBINFO('dbname')) as catalog_name\n"
                   "      ,trim(tab.owner)        as schema_name\n"
                   "      ,trim(tab.tabname)      as sequence_name\n"
                   "      ,seq.start_val as current_value\n"
                   "      ,0             as minimal_value\n"
                   "      ,seq.inc_val   as increment\n"
                   "      ,seq.cache     as cache\n"
                   "      ,seq.cycle     as cycle\n"
                   "      ,seq.order     as ordering\n"
                   "      ,''            as remarks\n"
                   "  FROM syssequences seq\n"
                   "      ,systables    tab\n"
                   " WHERE tab.tabid   = seq.tabid\n"
                   "   AND tab.tabtype = 'Q'\n");
  if(!p_schema.IsEmpty())
  {
    sql += _T("   AND tab.owner = ?\n");
  }
  if(!p_sequence.IsEmpty())
  {
    sql += _T("   AND tab.tabname ");
    sql += p_sequence.Find('%') >= 0 ? _T("LIKE") : _T("=");
    sql += _T(" ? \n");
  }
  return sql;
}

XString
SQLInfoInformix::GetCATALOGSequenceCreate(MetaSequence& p_sequence) const
{
  XString sql(_T("CREATE SEQUENCE "));

  if (!p_sequence.m_schemaName.IsEmpty())
  {
    sql += p_sequence.m_schemaName + _T(".");
  }
  sql += p_sequence.m_sequenceName;
  sql.AppendFormat(_T("\n START WITH %-12.0f"), p_sequence.m_currentValue);
  sql.AppendFormat(_T("\n INCREMENT BY %d"), p_sequence.m_increment);

  sql += p_sequence.m_cycle ? _T("\n CYCLE") : _T("\n NOCYCLE");
  sql += p_sequence.m_order ? _T("\n ORDER") : _T("\n NOORDER");
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
SQLInfoInformix::GetCATALOGSequenceDrop(XString /*p_schema*/, XString p_sequence) const
{
  XString sql(_T("DROP SEQUENCE ") + p_sequence);
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// ALL VIEW FUNCTIONS

XString 
SQLInfoInformix::GetCATALOGViewExists(XString& p_schema,XString& p_viewname,bool /*p_quoted = false*/) const
{
  p_schema.Empty(); // do not bind as a parameter
  p_viewname.MakeLower();

  XString sql = _T("SELECT count(*)\n"
                   "  FROM sysviews\n"
                   " WHERE viewname = ?");
  return sql;
}

XString 
SQLInfoInformix::GetCATALOGViewList(XString& /*p_schema*/,XString& /*p_pattern*/,bool /*p_quoted = false*/) const
{
  return _T("");
}

XString 
SQLInfoInformix::GetCATALOGViewAttributes(XString& /*p_schema*/,XString& /*p_viewname*/,bool /*p_quoted = false*/) const
{
  return _T("");
}

XString
SQLInfoInformix::GetCATALOGViewText(XString& /*p_schema*/,XString& /*p_viewname*/,bool /*p_quoted = false*/) const
{
  // Cannot query this, Use ODBC functions
  return _T("");
}

XString
SQLInfoInformix::GetCATALOGViewCreate(XString /*p_schema*/,XString p_viewname,XString p_contents,bool /*p_ifexists = true*/) const
{
  return _T("CREATE VIEW ") + p_viewname + _T("\n") + p_contents;
}

XString 
SQLInfoInformix::GetCATALOGViewRename(XString /*p_schema*/,XString /*p_viewname*/,XString /*p_newname*/)    const
{
  return _T("");
}

XString 
SQLInfoInformix::GetCATALOGViewDrop(XString /*p_schema*/,XString p_viewname,XString& p_precursor) const
{
  p_precursor.Empty();
  return _T("DROP VIEW ") + p_viewname;
}

// All Privilege functions
XString
SQLInfoInformix::GetCATALOGTablePrivileges(XString& /*p_schema*/,XString& /*p_tablename*/) const
{
  return _T("");
}

XString 
SQLInfoInformix::GetCATALOGColumnPrivileges(XString& /*p_schema*/,XString& /*p_tablename*/,XString& /*p_columnname*/) const
{
  return _T("");
}

XString 
SQLInfoInformix::GetCATALOGSequencePrivilege(XString& /*p_schema*/,XString& /*p_sequence*/) const
{
  return _T("");
}

XString 
SQLInfoInformix::GetCATALOGGrantPrivilege(XString /*p_schema*/,XString p_objectname,XString p_subObject,XString p_privilege,XString p_grantee,bool p_grantable)
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
SQLInfoInformix::GetCATALOGRevokePrivilege(XString /*p_schema*/,XString p_objectname,XString p_subObject,XString p_privilege,XString p_grantee)
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
SQLInfoInformix::GetCATALOGSynonymList(XString& /*p_schema*/,XString& /*p_pattern*/) const
{
  // Not implemented yet
  return _T("");
}

XString
SQLInfoInformix::GetCATALOGSynonymAttributes(XString& /*p_schema*/,XString& /*p_synonym*/) const
{
  // Not implemented yet
  return _T("");
}

XString
SQLInfoInformix::GetCATALOGSynonymCreate(XString& /*p_schema*/,XString& /*p_synonym*/,XString /*p_forObject*/,bool /*p_private = true*/) const
{
  // Not implemented yet
  return _T("");
}

XString
SQLInfoInformix::GetCATALOGSynonymDrop(XString& /*p_schema*/,XString& /*p_synonym*/,bool /*p_private = true*/) const
{
  // Not implemented yet
  return _T("");
}

// For ALL objects
XString
SQLInfoInformix::GetCATALOGCommentCreate(XString p_schema,XString p_object,XString p_name,XString p_subObject,XString p_remark) const
{
  XString sql;
  if(!p_object.IsEmpty() && !p_name.IsEmpty() && !p_remark.IsEmpty())
  {
    sql.Format(_T("COMMENT ON %s "),p_object.GetString());
    if(!p_schema.IsEmpty())
    {
      sql += QIQ(p_schema) + _T(".");
    }
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
SQLInfoInformix::GetPSMProcedureExists(XString /*p_schema*/, XString p_procedure,bool /*p_quoted = false*/) const
{
  p_procedure.MakeLower();
  return _T("select count(*)\n"
            "  from sysprocedures\n"
            " where procname='" + p_procedure + "'");
}

XString
SQLInfoInformix::GetPSMProcedureList(XString& /*p_schema*/,XString /*p_procedure*/,bool /*p_quoted = false*/) const
{
  return _T("");
}

XString
SQLInfoInformix::GetPSMProcedureAttributes(XString& /*p_schema*/,XString& /*p_procedure*/,bool /*p_quoted = false*/) const
{
  return _T("");
//   p_procedure.MakeLower();
//   XString sql = "SELECT sbody.data\n"
//                 "  FROM sysprocbody sbody\n"
//                 "      ,sysprocedures sproc\n"
//                 " WHERE sbody.procid   = sproc.procid\n"
//                 "   AND sproc.procname = '" + p_procedure + "'\n"
//                 "   AND datakey        = 'T'";
//   return sql;
}

XString
SQLInfoInformix::GetPSMProcedureSourcecode(XString p_schema, XString p_procedure,bool /*p_quoted = false*/) const
{
  p_schema.MakeLower();
  p_procedure.MakeLower();

  // TRIGGER PART
  XString sql;
  sql = _T("SELECT bod.datakey\n"
           "      ,bod.seqno\n"
           "      ,bod.data\n"
           "  FROM systrigbody bod\n"
           "      ,systriggers tri\n"
           " WHERE bod.trigid = tri.trigid\n"
           "   AND bod.datakey IN ('A','D')\n");
  if(!p_schema.IsEmpty())
  {
    sql += _T("   AND tri.owner = '") + p_schema + _T("'\n");
  }
  if(!p_procedure.IsEmpty())
  {
    sql += _T("   AND tri.trigname = '") + p_procedure + _T("'\n");
  }
  // UNION
  sql += _T("UNION ALL\n");
  // PROCEDURE PART
  sql += _T("  SELECT bod.datakey\n"
            "      ,bod.seqno\n"
            "      ,bod.data\n"
            "  FROM sysprocbody   bod\n"
            "      ,sysprocedures pro\n"
            " WHERE bod.procid   = pro.procid\n"
            "   AND bod.datakey  = 'T'\n");
  if(!p_schema.IsEmpty())
  {
    sql += _T("   AND pro.owner    = '") + p_schema + _T("'\n");
  }
  if(!p_procedure.IsEmpty())
  {
    sql += _T("   AND pro.procname = '") + p_procedure + _T("'\n");
  }
  // ORDERING
  sql += _T(" ORDER BY datakey DESC\n")
         _T("         ,seqno");
  return sql;
}

XString
SQLInfoInformix::GetPSMProcedureCreate(MetaProcedure& /*p_procedure*/) const
{
  return _T("");
}

XString
SQLInfoInformix::GetPSMProcedureDrop(XString /*p_schema*/, XString /*p_procedure*/,bool /* p_function /*=false*/) const
{
  return _T("");
}

XString
SQLInfoInformix::GetPSMProcedureErrors(XString /*p_schema*/,XString /*p_procedure*/,bool /*p_quoted = false*/) const
{
  // Informix does not support procedure errors
  return _T("");
}

XString
SQLInfoInformix::GetPSMProcedurePrivilege(XString& /*p_schema*/,XString& /*p_procedure*/,bool /*p_quoted = false*/) const
{
  return _T("");
}

// And it's parameters
XString
SQLInfoInformix::GetPSMProcedureParameters(XString& p_schema,XString& p_procedure,bool /*p_quoted = false*/) const
{
  p_schema.MakeLower();
  p_procedure.MakeLower();
  XString sql;
 sql=_T("SELECT Trim(DBINFO('dbname')) AS procedure_catalog\n"
        "      ,Trim(pro.owner)        AS procedure_schema\n"
        "      ,Trim(pro.procname)     AS procedure_name\n"
        "      ,Trim(col.paramname)    AS parameter_name\n"
        "      ,paramattr              AS column_type\n"
        "      ,CASE paramtype\n"
        "            WHEN 0  THEN 1 -- char\n"
        "            WHEN 1  THEN 5 -- smallint\n"
        "            WHEN 2  THEN 4 -- integer\n"
        "            WHEN 3  THEN 8 -- float\n"
        "            WHEN 4  THEN 7 -- float\n"
        "            WHEN 5  THEN 2 -- decimal\n"
        "            WHEN 6  THEN 4 -- serial\n"
        "            WHEN 7  THEN 9 -- date\n"
        "            WHEN 8  THEN 2 -- money/decimal\n"
        "            WHEN 10 THEN 11 -- timestmap\n"
        "            WHEN 11 THEN -6 -- byte\n"
        "            WHEN 12 THEN -3 -- text\n"
        "            WHEN 14 THEN -89 -- interval\n"
        "            WHEN 15 THEN -9  -- nchar\n"
        "            WHEN 17 THEN -5  -- bigint\n"
        "            WHEN 18 THEN -5  -- bigint serial\n"
        "            WHEN 41 THEN -4  -- blob\n"
        "            WHEN 43 THEN -1  -- lvarchar\n"
        "            WHEN 45 THEN -7  -- bit/boolean\n"
        "            WHEN 52 THEN -5  -- bigint\n"
        "            WHEN 53 THEN -5  -- bigserial\n"
        "                    ELSE 0   -- unknown\n"
        "       END AS dataype\n"
        "      ,CASE paramtype\n"
        "            WHEN 0  THEN 'CHAR'\n"
        "            WHEN 1  THEN 'SMALLINT'\n"
        "            WHEN 2  THEN 'INTEGER'\n"
        "            WHEN 3  THEN 'DOUBLE'\n"
        "            WHEN 4  THEN 'FLOAT'\n"
        "            WHEN 5  THEN 'DECIMAL'\n"
        "            WHEN 6  THEN 'INTEGER'\n"
        "            WHEN 7  THEN 'DATE'\n"
        "            WHEN 8  THEN 'DECIMAL'\n"
        "            WHEN 10 THEN 'TIMESTAMP'\n"
        "            WHEN 11 THEN 'BYTE'\n"
        "            WHEN 12 THEN 'VARCHAR'\n"
        "            WHEN 14 THEN 'INTERVAL'\n"
        "            WHEN 15 THEN 'WCHAR'\n"
        "            WHEN 17 THEN 'BIGINT'\n"
        "            WHEN 18 THEN 'SERIAL'\n"
        "            WHEN 41 THEN 'LONGVARBINARY'\n"
        "            WHEN 43 THEN 'WVARCHAR'\n"
        "            WHEN 45 THEN 'BIT'\n"
        "            WHEN 52 THEN 'BIGINT'\n"
        "            WHEN 53 THEN 'BIGINT'\n"
        "                    ELSE 'UNKNOWN'\n"
        "       END AS type_name\n"
        "      ,CASE paramtype\n"
        "            WHEN 0  THEN paramlen  -- char\n"
        "            WHEN 1  THEN 5 -- smallint\n"
        "            WHEN 2  THEN 11 -- integer\n"
        "            WHEN 3  THEN 13 -- float\n"
        "            WHEN 4  THEN 21 -- float\n"
        "            WHEN 5  THEN 40 -- decimal\n"
        "            WHEN 6  THEN 11 -- serial\n"
        "            WHEN 7  THEN 10 -- date\n"
        "            WHEN 8  THEN 40 -- money/decimal\n"
        "            WHEN 10 THEN 22 -- timestmap\n"
        "            WHEN 11 THEN paramlen -- byte\n"
        "            WHEN 12 THEN paramlen -- text\n"
        "            WHEN 14 THEN 20 -- interval\n"
        "            WHEN 15 THEN paramlen  -- nchar\n"
        "            WHEN 17 THEN 22  -- bigint\n"
        "            WHEN 18 THEN 22  -- bigint serial\n"
        "            WHEN 41 THEN paramlen  -- blob\n"
        "            WHEN 43 THEN paramlen  -- lvarchar\n"
        "            WHEN 45 THEN 1  -- bit/boolean\n"
        "            WHEN 52 THEN 22  -- bigint\n"
        "            WHEN 53 THEN 22  -- bigserial\n"
        "                    ELSE 0   -- unknown\n"
        "       END AS column_size\n"
        "      ,CASE paramtype\n"
        "            WHEN 0  THEN paramlen -- char\n"
        "            WHEN 1  THEN 2 -- smallint\n"
        "            WHEN 2  THEN 4 -- integer\n"
        "            WHEN 3  THEN 8 -- double\n"
        "            WHEN 4  THEN 4 -- float\n"
        "            WHEN 5  THEN Trunc(paramlen/256) -- decimal\n"
        "            WHEN 6  THEN 4 -- serial\n"
        "            WHEN 7  THEN Trunc(paramlen/256) -- date\n"
        "            WHEN 8  THEN Trunc(paramlen/256) -- money/decimal\n"
        "            WHEN 10 THEN Trunc(paramlen/256) -- timestmap\n"
        "            WHEN 11 THEN paramlen     -- byte\n"
        "            WHEN 12 THEN paramlen     -- text\n"
        "            WHEN 14 THEN Trunc(paramlen/256) -- interval\n"
        "            WHEN 15 THEN paramlen  -- nchar\n"
        "            WHEN 17 THEN 8   -- bigint\n"
        "            WHEN 18 THEN 8   -- bigint serial\n"
        "            WHEN 41 THEN paramlen  -- blob\n"
        "            WHEN 43 THEN paramlen  -- lvarchar\n"
        "            WHEN 45 THEN 1   -- bit/boolean\n"
        "            WHEN 52 THEN 8   -- bigint\n"
        "            WHEN 53 THEN 8   -- bigserial\n"
        "                    ELSE 0   -- unknown\n"
        "       END  AS buffer_length\n"
        "      ,Mod(paramlen,16) AS decimal_digits\n"
        "      ,10       AS numRadix\n"
        "      ,1        AS nullable\n"
        "      ,''       AS remarks\n"
        "      ,''       AS defaults\n"
        "      ,Trunc(Mod(paramlen,256)/16)  AS datatype3\n"
        "      ,Mod(paramlen,16)             AS subtype3\n"
        "      ,paramlen AS octetlength\n"
        "      ,paramid  AS position\n"
        "      ,'YES'    AS isnullable\n"
        "  FROM sysproccolumns col\n"
        "      ,sysprocedures  pro\n"
        " WHERE pro.procid   = col.procid\n");
  if(!p_schema.IsEmpty())
  {
    sql += _T("   AND pro.owner    = ?\n");
  }
  if(!p_procedure.IsEmpty())
  {
    sql += _T("   AND pro.procname ");
    sql += p_procedure.Find('%') >= 0 ? _T("LIKE") : _T("=");
    sql += _T(" ?\n");
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
SQLInfoInformix::GetPSMDeclaration(bool    /*p_first*/
                                  ,XString p_variable
                                  ,int     p_datatype
                                  ,int     p_precision /*= 0 */
                                  ,int     p_scale     /*= 0 */
                                  ,XString p_default   /*= ""*/
                                  ,XString /*p_domain    = ""*/
                                  ,XString p_asColumn  /*= ""*/) const
{
  XString line;
  line.Format(_T("DEFINE %s "),p_variable.GetString());

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
  else if(!p_asColumn)
  {
    line += _T(" LIKE ") + p_asColumn;
  }
  line += _T(";\n");
  return line;
}

XString
SQLInfoInformix::GetPSMAssignment(XString p_variable,XString p_statement /*=""*/) const
{
  XString line(_T("LET "));
  line += p_variable;
  line += _T(" = ");
  if(!p_statement.IsEmpty())
  {
    line += p_statement;
    line += _T(";");
  }
  return line;
}

XString
SQLInfoInformix::GetPSMIF(XString p_condition) const
{
  return _T("IF (") + p_condition + _T(") THEN\n");
}

XString
SQLInfoInformix::GetPSMIFElse() const
{
  return _T("ELSE\n");
}

XString
SQLInfoInformix::GetPSMIFEnd() const
{
  return _T("END IF;\n");
}

XString
SQLInfoInformix::GetPSMWhile(XString p_condition) const
{
  return _T("WHILE (") + p_condition + _T(")\n");
}

XString
SQLInfoInformix::GetPSMWhileEnd() const
{
  return _T("END WHILE;\n");
}

XString
SQLInfoInformix::GetPSMLOOP() const
{
  return _T("LOOP\n");
}

XString
SQLInfoInformix::GetPSMLOOPEnd() const
{
  return _T("END LOOP\n");
}

XString
SQLInfoInformix::GetPSMBREAK() const
{
  return _T("EXIT\n"); // [FOR][LOOP][WHILE][FOREACH]
}

XString
SQLInfoInformix::GetPSMRETURN(XString p_statement /*= ""*/) const
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
SQLInfoInformix::GetPSMExecute(XString p_procedure,MParameterMap& p_parameters) const
{
  // EXECUTE PROCEDURE name[(:param[,:param …])] [RETURNING_VALUES:param[,:param …]];
  XString line;
  line.Format(_T("EXECUTE PROCEDURE %s ("),p_procedure.GetString());
  bool doReturning = false;
  bool doMore = false;

  for(const auto& param : p_parameters)
  {
    // Extra ,
    if(doMore) line += _T(",");
    doMore = true;

    // Append input and in/out parameters
    if(param.m_columnType == P_SQL_PARAM_INPUT || param.m_columnType == P_SQL_PARAM_INPUT_OUTPUT)
    {
      line += param.m_parameter;
    }
    // See if we must do 'returning' clause
    if(param.m_columnType == P_SQL_PARAM_OUTPUT || param.m_columnType == P_SQL_PARAM_INPUT_OUTPUT)
    {
      doReturning = true;
    }
  }
  line += _T(")");

  // Do the returning clause
  if(doReturning)
  {
    line += _T(" INTO ");
    doMore = false;
    for(const auto& param : p_parameters)
    {
      // Extra ,
      if(doMore) line += _T(",");
      doMore = true;

      if(param.m_columnType == P_SQL_PARAM_OUTPUT || param.m_columnType == P_SQL_PARAM_INPUT_OUTPUT)
      {
        line += param.m_parameter;
      }
    }
  }
  line += _T(";\n");
  return line;
}

// The CURSOR
XString
SQLInfoInformix::GetPSMCursorDeclaration(XString p_cursorname,XString p_select) const
{
  return _T("FOREACH ") + p_cursorname + _T(" FOR ") + p_select + _T("\n");
}

XString
SQLInfoInformix::GetPSMCursorFetch(XString /*p_cursorname*/,std::vector<XString>& /*p_columnnames*/,std::vector<XString>& /*p_variablenames*/) const
{
  return _T("");
}

// END FOREACH; !!


//////////////////////////////////////////////////////////////////////////
// PSM Exceptions

XString
SQLInfoInformix::GetPSMExceptionCatchNoData() const
{
  return _T("ON EXCEPTION (100)\n");
}

XString
SQLInfoInformix::GetPSMExceptionCatch(XString p_sqlState) const
{
  return _T("ON EXCEPTION (") + p_sqlState + _T(")\n");
}

XString
SQLInfoInformix::GetPSMExceptionRaise(XString p_sqlState) const
{
  return _T("RAISE EXCEPTION ") + p_sqlState;
  // "[,isam-error [,'error text']]
}

// END EXCEPTION [WITH RESUME];

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
SQLInfoInformix::GetSESSIONMyself() const
{
  XString query = _T("SELECT sid\n"
                     "      ,user\n"
                     "      ,tty\n"
                     "      ,current_timestamp\n"  // timestamp
                     "      ,''\n"                 // remote address
                     "      ,''\n"                 // remote process name
                     "      ,''\n"                 // remote process ID
                     "  FROM sysmaster:syssessions\n"
                     " WHERE sid =\n"
                     "     ( SELECT dbinfo('sessionid')\n"
                     "         FROM systables\n"
                     "        WHERE tabid=1)");
  return query;
}

XString
SQLInfoInformix::GetSESSIONExists(XString p_sessionID) const
{
  return _T("SELECT COUNT(*)\n"
            "  FROM sysmaster:syssessions\n"
            " WHERE sid = ") + p_sessionID;
}

XString
SQLInfoInformix::GetSESSIONList() const
{
  return GetSESSIONAttributes(_T(""));
}

XString
SQLInfoInformix::GetSESSIONAttributes(XString p_sessionID) const
{
  XString sql = _T("SELECT sid\n"
                   "      ,user\n"
                   "      ,tty\n"
                   "      ,current_timestamp\n"  // timestamp
                   "      ,''\n"                 // remote address
                   "      ,''\n"                 // remote process name
                   "      ,''\n"                 // remote process ID
                   "  FROM sysmaster:syssessions");
  if (!p_sessionID.IsEmpty())
  {
    sql += _T("\n WHERE sid = ") + p_sessionID;
  }
  return sql;
}

//////////////////////////////////////////////////////////////////////////
// Transactions

XString
SQLInfoInformix::GetSESSIONConstraintsDeferred() const
{
  return _T("SET CONSTRAINTS ALL DEFERRED");
}

XString
SQLInfoInformix::GetSESSIONConstraintsImmediate() const
{
  return _T("SET CONSTRAINTS ALL IMMEDIATE");
}

//////////////////////////////////////////////////////////////////////////
//
// Call FUNCTION/PROCEDURE from within program
// As a RDBMS dependent extension of "DoSQLCall" of the SQLQuery object
//
//////////////////////////////////////////////////////////////////////////

// Calling a stored function or procedure if the RDBMS does not support ODBC call escapes
SQLVariant*
SQLInfoInformix::DoSQLCall(SQLQuery* /*p_query*/,XString& /*p_schema*/,XString& /*p_procedure*/)
{
  return nullptr;
}

// Calling a stored function with named parameters, returning a value
SQLVariant*
SQLInfoInformix::DoSQLCallNamedParameters(SQLQuery* /*p_query*/,XString& /*p_schema*/,XString& /*p_procedure*/,bool /*p_function = true*/)
{
  return nullptr;
}

// End of namespace
}

