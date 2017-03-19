////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoFirebird.cpp
//
// Copyright (c) 1998-2017 ir. W.E. Huisman
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
// Last Revision:   08-01-2017
// Version number:  1.4.0
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

// Gets the construction / select for generating a new serial identity
CString
SQLInfoFirebird::GetSQLGenerateSerial(CString p_table) const
{
  return "SELECT " + p_table + "_seq.nextval FROM DUAL";
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
  retval.Format("CAST '%02d/%02d/04d' AS DATE)",p_day,p_month,p_year); // American order!!
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
  p_pattern.MakeUpper();
  CString query = "SELECT rdb$relation_name\n"
                  "  FROM rdb$relations\n"
                  " WHERE rdb$relation_name like '" + p_pattern + "'";
  return query;
}

// Attributes can fill in 'm_temporary' 
bool
SQLInfoFirebird::GetCATALOGTableAttributes(CString /*p_schema*/,CString p_tablename,MetaTable& p_table) const
{
  p_tablename.MakeUpper();
  CString query = "SELECT rdb$relation_type\n"
                  "  FROM rdb$relations\n"
                  " WHERE rdb$relation_name = ?";
  SQLQuery qry(m_database);
  SQLVariant* var = qry.DoSQLStatementScalar(query,p_tablename);
  if(var)
  {
    p_table.m_temporary = var->GetAsBoolean();
    return true;
  }
  return false;
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
  CString sql(GetCATALOGColumnAttributes(p_schema,p_tablename,""));
  int pos = sql.ReverseFind('\n');
  // Remove field filter and add ordering
  sql = sql.Mid(pos + 1) + " ORDER BY col.rdb$field_position";

  return sql;
}

CString 
SQLInfoFirebird::GetCATALOGColumnAttributes(CString /*p_schema*/,CString p_tablename,CString p_columnname) const
{
  p_tablename.MakeUpper();
  p_columnname.MakeUpper();
  CString sql = "SELECT col.rdb$field_name\n"        // 0 -> Name
                "      ,col.rdb$field_position\n"    // 1 -> Position
                "      ,typ.rdb$field_type\n"        // 2 -> type
                "      ,typ.rdb$field_length\n"      // 3 -> length
                "      ,typ.rdb$null_flag\n"         // 4 -> nn 1 = not null / 0 is null
                "      ,typ.rdb$field_precision\n"   // 5 -> precision
                "      ,typ.rdb$field_scale\n"       // 6 -> scale (neg)
                "      ,col.rdb$default_source\n"    // 7 -> default value
                "  FROM rdb$relation_fields col\n"
                "      ,rdb$fields typ\n"
                " WHERE col.rdb$field_source  = typ.rdb$field_name\n"
                "   AND col.rdb$relation_name = '" + p_tablename  + "'\n"
                "   AND col.rdb$field_name    = '" + p_columnname + "'";
                // Beware: last filter line must be column name for previous method!
  return sql;
}

CString SQLInfoFirebird::GetCATALOGColumnCreate(MetaColumn& p_column) const
{
  CString sql  = "ALTER TABLE "  + p_column.m_table  + "\n";
                 "  ADD COLUMN " + p_column.m_column + " " + p_column.m_typename;
  p_column.GetPrecisionAndScale(sql);
  p_column.GetNullable(sql);
  return sql;
}

CString SQLInfoFirebird::GetCATALOGColumnAlter(MetaColumn& p_column) const
{
  // The extra 'TYPE' keyword  is a-typical
  // The SET/DROP for the NULL is a-typical
  CString sql  = "ALTER TABLE "  + p_column.m_table  + "\n";
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
SQLInfoFirebird::GetCATALOGIndexList(CString /*p_schema*/,CString p_tablename)   const
{
  p_tablename.MakeUpper();

  CString query = "SELECT idx.rdb$index_name\n"
                  "      ,col.rdb$field_name\n"
                  "      ,col.rdb$field_position\n"
                  "      ,idx.rdb$unique_flag\n" // 1 if unique, null if not
                  "      ,idx.rdb$index_type\n"  // 1 if descending, null,0 on ascending
                  "      ,idx.rdb$expression_source\n"
                  "  FROM rdb$indices idx\n"
                  "      ,rdb$index_segments col\n"
                  " WHERE idx.rdb$index_name    = col.rdb$index_name\n"
                  "   AND idx.rdb$system_flag   = 0\n"
                  "   AND idx.rdb$relation_name = '" + p_tablename + "'\n"
                  " ORDER BY 1";
  return query;
}

CString
SQLInfoFirebird::GetCATALOGIndexAttributes(CString p_schema,CString p_tablename,CString p_indexname)  const
{
  return "";
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
  CString sql = "SELECT rdb$constraint_name\n"      // 1 -> Constraint name
                "      ,rdb$index_name\n"           // 2 -> Index name
                "      ,rdb$deferrable\n"           // 3 ->
                "      ,rdb$initially_deferred\n"
                "  FROM rdb$relation_constraints\n"
                " WHERE rdb$relation_name   = '" + p_tablename + "'\n"
                "   AND rdb$constraint_type = 'PRIMARY KEY'";
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
SQLInfoFirebird::GetCATALOGForeignAttributes(CString p_schema,CString p_tablename,CString p_constraint,int /*p_maxColumns*/ /*=SQLINFO_MAX_COLUMNS*/) const
{
  p_schema.MakeUpper();
  p_tablename.MakeUpper();
  p_constraint.MakeUpper();
  CString query;
  query.Format( "SELECT mon.mon$database_name           AS primary_key_catalog\n"
                "      ,'%s'                            AS primary_key_schema\n"
                "      ,idx.rdb$relation_name           AS primary_key_table\n"
                "      ,mon.mon$database_name           AS foreign_key_catalog\n"
                "      ,'%s'                            AS foreign_key_schema\n"
                "      ,con.rdb$relation_name           AS foreign_key_table\n"
                "      ,ref.rdb$const_name_uq           AS primary_key_constraint\n"
                "      ,con.rdb$constraint_name         AS foreign_key_constraint\n"
                "      ,%d                              AS key_sequence\n"
                "      ,seg.rdb$field_name              AS foreign_key_column_name\n"
                "      ,psg.rdb$field_name              AS primary_key_column_name\n"
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
    query += "   AND con.rdb$relation_name   = '" + p_tablename + "'\n";
  }
  if(!p_constraint.IsEmpty())
  {
    query += "   AND con.rdb$constraint_name = '" + p_constraint + "'\n";
  }

  // Add ordering upto column number
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

  CString sql;
  sql.Format("SELECT '' AS catalog_name\n"
             "      ,'%s' AS schema_name\n"
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
             "  FROM rdb$triggers\n"
             " WHERE rdb$relation_name = '%s'\n"
             "   AND rdb$system_flag   = 0\n"
            ,p_schema.GetString()
            ,p_tablename.GetString());

  if(!p_triggername.IsEmpty())
  {
    sql.AppendFormat("   AND rdb$trigger_name = '%s'\n",p_triggername.GetString());
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
SQLInfoFirebird::GetCATALOGSequenceAttributes(CString /*p_schema*/, CString p_sequence) const
{
  p_sequence.MakeUpper();

  CString sql = "SELECT '' as catalog_name\n"
                "       '' as schema_name\n"
                "      ,rdb$generator_name      as sequence_name\n" 
                "      ,rdb$initial_value       as current_value\n"
                "      ,0                       as minimal_value\n"
                "      ,rdb$generator_increment as increment\n"
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
  sql.Format("CREATE SEQUENCE %s START WITH %d"
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
  return "";
}

CString 
SQLInfoFirebird::GetCATALOGViewAttributes(CString p_schema,CString p_viewname) const
{
  return "";
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
  CString query = ( "SELECT COUNT(*) "
                    "  FROM RDB$PROCEDURES "
                    " WHERE RDB$PROCEDURE_NAME ='" + p_procedure + "';");
  return query;
}

CString
SQLInfoFirebird::GetPSMProcedureList(CString p_schema) const
{
  return "";
}

CString
SQLInfoFirebird::GetPSMProcedureAttributes(CString p_schema, CString p_procedure) const
{
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
  return "";
}

CString
SQLInfoFirebird::GetPSMProcedureErrors(CString p_schema,CString p_procedure) const
{
  // Firebird does not support procedure errors
  return "";
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
    if(param.m_type == 0 || param.m_type == 2)
    {
      line.AppendFormat(":%s",param.m_parameter.GetString());
    }
    // See if we must do 'returning' clause
    if(param.m_type == 1 || param.m_type == 2)
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

      if(param.m_type == 1 || param.m_type == 2)
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
          throw CString("Wrong number of output parameters for procedure call");
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


