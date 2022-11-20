////////////////////////////////////////////////////////////////////////
//
// File: SQLMetaInfo.h
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
#pragma once
#include "SQLComponents.h"
#include <vector>

namespace SQLComponents
{

// All object types for SQLTables

#define OBJECT_TYPE_TABLE         "TABLE"             // Real physical table of data
#define OBJECT_TYPE_VIEW          "VIEW"              // View over one or more physical tables
#define OBJECT_TYPE_SYSTABLE      "SYSTEM TABLE"      // System catalog table
#define OBJECT_TYPE_GLOBALTEMP    "GLOBAL TEMPORARY"  // Global temporary table (visible to other sessions)
#define OBJECT_TYPE_LOCALTEMP     "LOCAL TEMPORARY"   // Local temporary table only visible to current session
#define OBJECT_TYPE_ALIAS         "ALIAS"             // MSSQL server like alias to another database/table
#define OBJECT_TYPE_SYNONYM       "SYNONYM"           // Oracle/Informix like alias to another database/table

// Foreign key options, not in sqlext.h

#define SQL_MATCH_FULL    0
#define SQL_MATCH_PARTIAL 1 
#define SQL_MATCH_SIMPLE  2

// Contains all types for META SQLInfo inquiries to the ODBC Driver
// About the objects in the database system catalog.

// Results from "SQLTables"

typedef struct _metaInfoTable
{
  XString   m_catalog;                // Catalog name (database name)
  XString   m_schema;                 // Schema name (owner of the object)
  XString   m_table;                  // Table/view/synonym name
  XString   m_objectType;             // Type of the object
  XString   m_remarks;                // Using COMMENTS command
  XString   m_fullName;               // Full object name, conforming RDBMS rules
                                      // eg: type:catalog.schema.table
                                      // or: type:schema.table@catalog
  XString   m_tablespace;             // Some engines need the storage space
  bool      m_temporary { false };    // Temporary table (true) or persistent (false)
}
MetaTable;

using MTableMap = std::vector<MetaTable>;

// Results from "SQLColumns"

typedef struct _metaInfoColumn
{
  XString   m_catalog;                // 01 Catalog name (database name)
  XString   m_schema;                 // 02 Schema name (owner of the object)
  XString   m_table;                  // 03 Table/view/synonym name
  XString   m_column;                 // 04 Column name
  int       m_datatype      { 0 };    // 05 SQL Data type
  XString   m_typename;               // 06 Data type name (according to RDBMS)
  int       m_columnSize    { 0 };    // 07 Display column size or precision
  INT64     m_bufferLength  { 0 };    // 08 Internal buffer size
  int       m_decimalDigits { 0 };    // 09 Also known as scale
  int       m_numRadix      { 0 };    // 10 Radix of the column size
  int       m_nullable      { 2 };    // 11 Nullable SQL_NO_NULLS(0) / SQL_NULLABLE (1) / SQL_NULLABLE_UNKNOWN (2)
  XString   m_remarks;                // 12 Using COMMENTS command
  XString   m_default;                // 13 Default value of the column
  int       m_datatype3     { 0 };    // 14 ODBC3 datatype (mostly m_datatype)
  int       m_sub_datatype  { 0 };    // 15 Datetime sub type 
  INT64     m_octet_length  { 0 };    // 16 (VAR)CHAR octet length for Unicode
  int       m_position      { 0 };    // 17 Ordinal position in the table
  XString   m_isNullable;             // 18 'YES', 'NO' or 'UNKNOWN'

  void GetPrecisionAndScale(XString& p_sql)
  {
    if(m_columnSize > 0)
    {
      p_sql.AppendFormat("(%d",m_columnSize);
      if(m_decimalDigits > 0)
      {
        p_sql.AppendFormat(",%d",m_decimalDigits);
      }
      p_sql += ")";
    }
  };
  void GetNullable(XString& p_sql)
  {
    if(!m_nullable)
    {
      p_sql += " NOT NULL";
    }
  };
  void GetDefault(XString& p_sql)
  {
    if(!m_default.IsEmpty())
    {
      //p_sql += " DEFAULT ";
      p_sql += m_default;
    }
  }
}
MetaColumn;

using MColumnMap = std::vector<MetaColumn>;

// Results from "SQLPrimaryKeys" 

typedef struct _metaInfoPrimary
{
  XString  m_catalog;                 // Catalog of the primary key table
  XString  m_schema;                  // Schema  of the primary key table
  XString  m_table;                   // Name    of the primary key table
  XString  m_columnName;              // Name of the column of the primary key
  int      m_columnPosition { 0 };    // Position of the column in the table
  XString  m_constraintName;          // Name of the primary key constraint
  // Extra by SQL
  int      m_deferrable       { 0 };  // SQL_INITIALLY_DEFERRED(5) / SQL_INITIALLY_IMMEDIATE(6) / SQL_NOT_DEFERRABLE(7)
  int      m_initiallyDeferred{ 0 };  // 0=Immediate, 1=initially deferred
}
MetaPrimary;

using MPrimaryMap = std::vector<MetaPrimary>;


// Results from "SQLForeignKeys"

typedef struct _metaInfoForeign
{
  // Primary table
  XString   m_pkCatalogName;            // Primary key catalog name
  XString   m_pkSchemaName;             // Primary key schema name
  XString   m_pkTableName;              // Primary key table name
  // Foreign key table
  XString   m_fkCatalogName;            // Foreign key catalog name
  XString   m_fkSchemaName;             // Foreign key schema name
  XString   m_fkTableName;              // Foreign key table name
  // Constraints naming
  XString   m_primaryConstraint;        // Name of the primary key constraint
  XString   m_foreignConstraint;        // Name of the foreign key constraint
  // The column
  int       m_keySequence       { 0 };  // Ordinal sequence number
  XString   m_pkColumnName;             // Corresponding column in the primary key table
  XString   m_fkColumnName;             // Corresponding column of the foreign key
  // Options of the foreign key
  int       m_updateRule        { 0 };  // SQL_CASCADE(0) / SQL_NO_ACTION(3) / SQL_SET_NULL(2) / SQL_SET_DEFAULT(4) / SQL_RESTRICT(1)
  int       m_deleteRule        { 0 };  // SQL_CASCADE(0) / SQL_NO_ACTION(3) / SQL_SET_NULL(2) / SQL_SET_DEFAULT(4) / SQL_RESTRICT(1)
  int       m_deferrable        { 0 };  // SQL_INITIALLY_DEFERRED(5) / SQL_INITIALLY_IMMEDIATE(6) / SQL_NOT_DEFERRABLE(7)
  int       m_match             { 0 };  // SQL_MATCH_FULL(0) / SQL_MATCH_PARTIAL(1) / SQL_MATCH_SIMPLE(2)
  // Extra info by SQL
  int       m_initiallyDeferred { 0 };  // 0=Immediate, 1=initially deferred
  int       m_enabled           { 1 };  // 1=Disabled,  0=enabled
}
MetaForeign;

using MForeignMap = std::vector<MetaForeign>;

// Results from "SQLStatistics"

typedef struct _metaInfoIndex
{
  XString   m_catalogName;              // 01 Catalog (database name)
  XString   m_schemaName;               // 02 Schema owner
  XString   m_tableName;                // 03 Table name
  bool      m_nonunique   { true };     // 04 false = UNIQUE
  XString   m_indexName;                // 05 Index name
  int       m_indexType   { 0 };        // 06 SQL_TABLE_STAT / SQL_INDEX_CLUSTERED / SQL_INDEX_HASHED / SQL_INDEX_OTHER
  int       m_position    { 0 };        // 07 Ordinal position (start at 1)
  XString   m_columnName;               // 08 Column in the index
  XString   m_ascending;                // 09 'A', 'D', '' = unknown
  long      m_cardinality { 0 };        // 10 cardinality of index
  long      m_pages       { 0 };        // 11 Pages of the index
  XString   m_filter;                   // 12 filter or expression
}
MetaIndex;

using MIndicesMap = std::vector<MetaIndex>;

// Results from "SQLSpecialColumns"

typedef struct _metaSpecialColumns
{
  XString   m_columnName;               // Extra column name by RDBMS
  XString   m_typeName;                 // Different datatype by RDBMS
  int       m_datatype      { 0 };      // Corresponding ODBC Datatype
  int       m_columnSize    { 0 };      // Display column size
  int       m_bufferSize    { 0 };      // Binary buffer size
  int       m_decimalDigits { 0 };      // Decimal digits of the column
  int       m_scope         { 0 };      // SQL_SCOPE_CURROW  / SQL_SCOPE_TRANSACTION / SQL_SCOPE_SESSION
  int       m_pseudo        { 0 };      // SQL_PC_NOT_PSEUDO / SQL_PC_PSEUDO / SQL_PC_UNKNOWN
}
MetaSpecialColumn;

using MSpecialsMap = std::vector<MetaSpecialColumn>;

// Results from "SQLTablePrivileges" 

typedef struct _metaInfoPrivilege
{
  XString   m_catalogName;              // Catalog of the object
  XString   m_schemaName;               // Schema of the object
  XString   m_tableName;                // Table/object name
  XString   m_columnName;               // Column with specific privileges
  XString   m_grantor;                  // Grantor granting the privilege
  XString   m_grantee;                  // Grantee receiving the privilege
  XString   m_privilege;                // The privilege (CRUD etc)
  bool      m_grantable { false };      // Can pass on the privilege to others
}
MetaPrivilege;

using MPrivilegeMap = std::vector<MetaPrivilege>;

// Results from "SQLProcedures"

typedef struct _metaInfoProcedure
{
  XString   m_catalogName;              // Catalog of the procedure / function
  XString   m_schemaName;               // Schema  of the procedure / function
  XString   m_procedureName;            // Name    of the procedure / function
  int       m_inputParameters   { 0 };  // Number  of input parameters
  int       m_outputParameters  { 0 };  // Number  of output parameters
  int       m_resultSets        { 0 };  // Number  of result sets
  XString   m_remarks;                  // As in COMMENT command
  int       m_procedureType     { 0 };  // SQL_PT_PROCEDURE(1) / SQL_PT_FUNCTION(2) / SQL_PT_UNKNOWN(3)
  // EXTRA INFO: NOT IN ODBC
  XString   m_source;                   // Actual source of the procedure/function
}
MetaProcedure;

using MProcedureMap = std::vector<MetaProcedure>;

// Results from "SQLProcedureColumns"

// Parameters for a procedure

typedef struct _metaParameter
{
  XString  m_catalog;               // 01 Catalog of the procedure of the parameter
  XString  m_schema;                // 02 Schema  of the procedure of the parameter
  XString  m_procedure;             // 03 Procedure or function name
  XString  m_parameter;             // 04 Name of the parameters
  int      m_columnType    { 0 };   // 05 SQL_PARAM_INPUT (1) / SQL_PARAM_INPUT_OUTPUT (2) / SQL_PARAM_OUTPUT (4) / SQL_RETURN_VALUE (5) / SQL_RESULT_COL (3)
  int      m_datatype      { 0 };   // 06 ODBC datatype 
  XString  m_typeName;              // 07 Type name
  int      m_columnSize    { 0 };   // 08 Column size or display size
  int      m_bufferLength  { 0 };   // 09 Database buffer length
  int      m_decimalDigits { 0 };   // 10 Decimal digits after the comma
  int      m_numRadix      { 0 };   // 11 Decimal radix
  int      m_nullable      { 0 };   // 12 Is Nullable
  XString  m_remarks;               // 13 From the COMMENT command
  XString  m_default;               // 14 Default value of the parameter
  int      m_datatype3     { 0 };   // 15 ODBC 3 datatype
  int      m_subType       { 0 };   // 16 Date time subtype of ODBC 3
  int      m_octetLength   { 0 };   // 17 Octet length for Unicode
  int      m_position      { 0 };   // 18 Ordinal position of the parameter (0 = return value)
  XString  m_isNullable;            // 19 'YES' or 'NO'
}
MetaParameter;

using MParameterMap = std::vector<MetaParameter>;

// Results for Meta objects in the catalog(s)

typedef struct _metaInfoObject
{
  int       m_objectType  { 0 };        // META_CATALOGS / META_SCHEMAS / META_TABLES
  XString   m_objectName;               // Name of the object
  XString   m_remarks;                  // From the COMMENT command
}
MetaObject;

using MMetaMap = std::vector<MetaObject>;

// Results for triggers - by SQLInfoDB!

typedef struct _metaTrigger
{
  XString   m_catalogName;              // Catalog name
  XString   m_schemaName;               // Schema name
  XString   m_tableName;                // Table name
  XString   m_triggerName;              // Trigger name
  XString   m_remarks;                  // Remarks from COMMENT ON
  int       m_position    { 0 };        // Position in firing
  bool      m_before      { false };    // true = BEFORE, false = AFTER
  bool      m_insert      { false };    // INSERT into table
  bool      m_update      { false };    // UPDATE of a table
  bool      m_delete      { false };    // DELETE from a table
  bool      m_select      { false };    // SELECT from a table
  bool      m_session     { false };    // ON CONNECT / ON DISCONNECT
  bool      m_transaction { false };    // ON BEGIN   / ON COMMIT
  bool      m_rollback    { false };    // ON ROLLBACK
  XString   m_referencing;              // Referencing names OLD and NEW
  bool      m_enabled     { false };    // Enabled or disabled
  XString   m_source;                   // Source of the trigger
}
MetaTrigger;

using MTriggerMap = std::vector<MetaTrigger>;

// Results for Sequences - By SQLInfoDB!

typedef struct _metaSequence
{
  XString   m_catalogName;              // Catalog name
  XString   m_schemaName;               // Schema name
  XString   m_sequenceName;             // Sequence name
  double    m_currentValue  { 1 };      // Current value (the sequence!)
  double    m_minimalValue  { 1 };      // Minimal value in the series
  long      m_increment     { 1 };      // Incremented each step
  long      m_cache         { 0 };      // Caching in the database
  bool      m_cycle         { false };  // Cycling of sets
  bool      m_order         { true  };  // In-order over multiple machines
}
MetaSequence;

using MSequenceMap = std::vector<MetaSequence>;

// Results for sessions - By SQLInfoDB!

typedef struct _metaSession
{
  XString     m_session;      // UNIQUE session number within RDBMS
  XString     m_user;         // User (OS or RDBMS user)
  XString     m_terminal;     // OS terminal, IP address or Net name
  // Newer fields
  XString     m_logonMoment;  // Timestamp of login
  XString     m_remoteIP;     // Remote IP address
  XString     m_processName;  // Remote process name
  XString     m_processID;    // Remote process ID
}
MetaSession;

using MSessionMap = std::vector<MetaSession>;

};
