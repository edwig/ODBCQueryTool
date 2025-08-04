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

// For MariaDB the identifiers are transformed to LOWER case
// as the system catalog is stored in this case setting.
#define IdentifierCorrect(ident)   if(!p_quoted || !IsIdentifierMixedCase(ident)) ident.MakeLower()

// Constructor
SQLInfoMariaDB::SQLInfoMariaDB(SQLDatabase* p_database)
               :SQLInfoMySQL(p_database)
{
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
  DetectOracleMode();

  // See to it that "SQLDatabase:GetDatabaseName" does it's work
  return m_database->GetDatabaseName();
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

// Get select part to add new record identity to a table
// Can be special column like 'OID' or a sequence select
XString
SQLInfoMariaDB::GetKEYWORDIdentityString(XString& p_tablename,XString p_postfix /*= "_seq"*/) const
{
  return p_tablename + p_postfix + _T(".nextval");
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
  // MariaDB does bare SELECT!
  return _T("");
}

//////////////////////////////////////////////////////////////////////////
//
// SQL STRINGS
//
//////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////
// ALL SEQUENCE FUNCTIONS

XString
SQLInfoMariaDB::GetCATALOGSequenceExists(XString p_schema,XString p_sequence,bool p_quoted /*= false*/) const
{
  IdentifierCorrect(p_sequence);
  XString sql = _T("SELECT COUNT(*)\n")
                _T("  FROM information_schema.tables tab\n")
                _T(" WHERE tab.table_type = 'SEQUENCE'\n")
                _T("   AND sequence_name  = '");
  sql += p_sequence;
  sql += _T("\'");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("\n   AND table_schema = '");
    sql += p_schema;
    sql += _T("\'");
  }
  return sql;
}

XString
SQLInfoMariaDB::GetCATALOGSequenceList(XString& p_schema,XString& p_pattern,bool p_quoted /*= false*/) const
{
  if(!p_pattern.IsEmpty() && p_pattern.Find('%') < 0)
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
                _T("      ,'' AS remarks\n")
                _T("  FROM information_schema.tables tab\n")
                _T(" WHERE tab.table_type = 'SEQUENCE'");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("\n   AND table_schema = ?");
  }
  if(!p_pattern.IsEmpty())
  {
    IdentifierCorrect(p_pattern);
    sql += _T("\n   AND table_name LIKE ?");
  }
  return sql;
}

XString
SQLInfoMariaDB::GetCATALOGSequenceAttributes(XString& p_schema,XString& p_sequence,bool p_quoted /*= false*/) const
{
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
                _T("      ,''                  AS remarks\n")
                _T("  FROM information_schema.tables as tab\n")
                _T("      ,") + table + _T(" as seq\n")
                _T(" WHERE tab.table_type = 'SEQUENCE'");
  if(!p_schema.IsEmpty())
  {
    IdentifierCorrect(p_schema);
    sql += _T("\n   AND tab.table_schema = ?");
  }
  if(!p_sequence.IsEmpty())
  {
    IdentifierCorrect(p_sequence);
    sql += _T("\n   AND tab.table_name  = ?");
  }
  return sql;
}

XString
SQLInfoMariaDB::GetCATALOGSequenceCreate(MetaSequence& p_sequence) const
{
  XString sql(_T("CREATE OR REPLACE SEQUENCE "));

  sql += QIQ(p_sequence.m_sequenceName);
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
  XString sql(_T("DROP SEQUENCE ") + QIQ(p_sequence));
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

// SQL is almost the same as MySQL with two differences
// 'READS SQL DATA' - Obsolete and no longer needed
// 'GROUP BY'       - two extra columns or not needed

XString
SQLInfoMariaDB::GetPSMProcedureSourcecode(XString p_schema, XString p_procedure,bool p_quoted /*= false*/) const
{
  XString sql = _T("SELECT 0 as type\n"
                   "      ,0 as line\n"
                   "      ,CONCAT(\n"
                   "       'CREATE OR REPLACE ',r.routine_type,' `',r.routine_name,'`(',\n"
                   "       IFNULL(GROUP_CONCAT(\n"
                   "              CONCAT(p.parameter_mode,' ',p.parameter_name,' ',p.dtd_identifier)\n"
                   "              ORDER BY p.ordinal_position\n"
                   "              SEPARATOR ','),''),\n"
                   "       ')',CHAR(10),IFNULL(CONCAT('RETURNS ',r.dtd_identifier,CHAR(10)),''),r.routine_definition) AS source\n"
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
            "         ,r.routine_type\n");
  return sql;
}

//////////////////////////////////////////////////////////////////////////
//
// PRIVATE PART
//
//////////////////////////////////////////////////////////////////////////

void
SQLInfoMariaDB::DetectOracleMode() const
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
