////////////////////////////////////////////////////////////////////////
//
// File: SQLMetaInfo.h
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
#define OBJECT_TYPE_SYSVIEW       "SYSTEM VIEW"       // Added for ORACLE: View over one or more system tables

// Foreign key options, not in sqlext.h

#define SQL_MATCH_FULL    0
#define SQL_MATCH_PARTIAL 1 
#define SQL_MATCH_SIMPLE  2

// Contains all types for META SQLInfo inquiries to the ODBC Driver
// About the objects in the database system catalog.

// Results from "SQLTables"

typedef struct _metaInfoTable
{
  CString   m_catalog;                // Catalog name (database name)
  CString   m_schema;                 // Schema name (owner of the object)
  CString   m_table;                  // Table/view/synonym name
  CString   m_objectType;             // Type of the object
  CString   m_remarks;                // Using COMMENTS command
  CString   m_fullObjectName;         // Full object name, conforming RDBMS rules
                                      // eg: type:catalog.schema.table
                                      // or: type:schema.table@catalog
  CString   m_tablespace;             // Some engines need the storage space
  bool      m_temporary { false };    // Temporary table (true) or persistent (false)
}
MetaTable;

using MTableMap = std::vector<MetaTable>;

// Results from "SQLColumns"

typedef struct _metaInfoColumn
{
  CString   m_catalog;                // Catalog name (database name)
  CString   m_schema;                 // Schema name (owner of the object)
  CString   m_table;                  // Table/view/synonym name
  CString   m_column;                 // Column name
  CString   m_remarks;                // Using COMMENTS command
  int       m_datatype    { 0 };      // SQL Data type
  CString   m_typename;               // Data type name (according to RDBMS)
  int       m_precision   { 0 };      // Precision
  int       m_length      { 0 };      // Total length in bytes
  int       m_scale       { 0 };      // Decimal/Numeric scale
  int       m_nullable    { 0 };      // Nullable SQL_NO_NULLS / SQL_NULLABLE / SQL_NULLABLE_UNKNOWN
  CString   m_default;                // Default value of the column
  int       m_position    { 0 };      // Ordinal position in the table

  void GetPrecisionAndScale(CString& p_sql)
  {
    if(m_precision > 0)
    {
      p_sql.AppendFormat("(%d",m_precision);
      if(m_scale > 0)
      {
        p_sql.AppendFormat(",%d",m_scale);
      }
      p_sql += ")";
    }
  };
  void GetNullable(CString& p_sql)
  {
    if(!m_nullable)
    {
      p_sql += " NOT NULL";
    }
  };
  void GetDefault(CString& p_sql)
  {
    if(!m_default.IsEmpty())
    {
      p_sql += " DEFAULT ";
      p_sql += m_default;
    }
  }
}
MetaColumn;

using MColumnMap = std::vector<MetaColumn>;

// Results from "SQLPrimaryKeys" 

typedef struct _metaInfoPrimary
{
  CString  m_catalog;                 // Catalog of the primary key table
  CString  m_schema;                  // Schema  of the primary key table
  CString  m_table;                   // Name    of the primary key table
  CString  m_columnName;              // Name of the column of the primary key
  int      m_columnPosition { 0 };    // Position of the column in the table
  CString  m_constraintName;          // Name of the primary key constraint
}
MetaPrimary;

using MPrimaryMap = std::vector<MetaPrimary>;


// Results from "SQLForeignKeys"

typedef struct _metaInfoForeign
{
  // Primary table
  CString   m_pkCatalogName;            // Primary key catalog name
  CString   m_pkSchemaName;             // Primary key schema name
  CString   m_pkTableName;              // Primary key table name
  // Foreign key table
  CString   m_fkCatalogName;            // Foreign key catalog name
  CString   m_fkSchemaName;             // Foreign key schema name
  CString   m_fkTableName;              // Foreign key table name
  // Constraints naming
  CString   m_primaryConstraint;        // Name of the primary key constraint
  CString   m_foreignConstraint;        // Name of the foreign key constraint
  // The column
  int       m_keySequence       { 0 };  // Ordinal sequence number
  CString   m_pkColumnName;             // Corresponding column in the primary key table
  CString   m_fkColumnName;             // Corresponding column of the foreign key
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
  CString   m_catalogName;              // Catalog (database name)
  CString   m_schemaName;               // Schema owner
  CString   m_tableName;                // Table name
  bool      m_unique      { false };    // true = UNIQUE
  CString   m_indexName;                // Index name
  int       m_indexType   { 0 };        // SQL_TABLE_STAT / SQL_INDEX_CLUSTERED / SQL_INDEX_HASHED / SQL_INDEX_OTHER
  int       m_position    { 0 };        // Ordinal position (start at 1)
  CString   m_columnName;               // Column in the index
  CString   m_ascending;                // 'A', 'D', '' = unknown
  long      m_cardinality { 0 };        // cardinality of index
  long      m_pages       { 0 };        // Pages of the index
  CString   m_filter;                   // filter or expression
}
MetaIndex;

using MIndicesMap = std::vector<MetaIndex>;

// Results from "SQLSpecialColumns"

typedef struct _metaSpecialColumns
{
  CString   m_columnName;               // Extra column name by RDBMS
  CString   m_typeName;                 // Different datatype by RDBMS
  int       m_datatype      { 0 };      // Corresponding ODBC Datatype
  int       m_columnSize    { 0 };      // Display column size
  int       m_bufferSize    { 0 };      // Binary buffer size
  int       m_decimalDigits { 0 };      // Decimal digits of the column
  int       m_scope         { 0 };      // SQL_SCOPE_CURROW  / SQL_SCOPE_TRANSACTION / SQL_SCOPE_SESSION
  int       m_pseudo        { 0 };      // SQL_PC_NOT_PSEUDO / SQL_PC_PSEUDO / SQL_PC_UNKNOWN
}
MetaSpecialColumn;

using MSpecialColumnMap = std::vector<MetaSpecialColumn>;

// Results from "SQLTablePrivileges" 

typedef struct _metaInfoPrivilege
{
  CString   m_catalogName;              // Catalog of the object
  CString   m_schemaName;               // Schema of the object
  CString   m_tableName;                // Table/object name
  CString   m_grantor;                  // Grantor granting the privilege
  CString   m_grantee;                  // Grantee receiving the privilege
  CString   m_privilege;                // The privilege (CRUD etc)
  bool      m_grantable { false };      // Can pass on the privilege to others
}
MetaPrivilege;

using MPrivilegeMap = std::vector<MetaPrivilege>;

// Results from "SQLProcedures"

typedef struct _metaInfoProcedure
{
  CString   m_catalogName;              // Catalog of the procedure / function
  CString   m_schemaName;               // Schema  of the procedure / function
  CString   m_procedureName;            // Name    of the procedure / function
  int       m_inputParameters   { 0 };  // Number  of input parameters
  int       m_outputParameters  { 0 };  // Number  of output parameters
  int       m_resultSets        { 0 };  // Number  of result sets
  CString   m_remarks;                  // As in COMMENT command
  int       m_procedureType     { 0 };  // SQL_PT_PROCEDURE(1) / SQL_PT_FUNCTION(2) / SQL_PT_UNKNOWN(3)
}
MetaProcedure;

using MProcedureMap = std::vector<MetaProcedure>;

// Results from "SQLProcedureColumns"

typedef struct _metaInfoProcColumns
{
  CString   m_catalogName;              // Catalog of the procedure / function
  CString   m_schemaName;               // Schema  of the procedure / function
  CString   m_procedureName;            // Name    of the procedure / function
  CString   m_columnName;               // Parameter / column name 
  int       m_ordinalPosition { 0 };    // Positioning of the parameter
  int       m_columnType      { 0 };    // SQL_PARAM_INPUT / SQL_PARAM_OUTPUT etc. etc.
  int       m_dataType        { 0 };    // ODBC standard type name
  CString   m_typeName;                 // RDBMS type name
  int       m_columnSize      { 0 };    // Display size
  int       m_bufferSize      { 0 };    // Binary buffer size
  int       m_decimalDigits   { 0 };    // Number of decimal digits
  int       m_radix           { 0 };    // Radix display (2, 8, 10, 16)
  int       m_nullable        { 0 };    // SQL_NO_NULLS / SQL_NULLABLE / SQL_NULLABLE_UNKNOWN
  CString   m_remarks;                  // From the COMMENT command
  CString   m_defaultValue;             // Default value
  CString   m_isNullable;               // "NO" or "YES" (can include nulls)
}
MetaProcedureColumn;

using MProcColumnMap = std::vector<MetaProcedureColumn>;

// Results for Meta objects in the catalog(s)

typedef struct _metaInfoObject
{
  int       m_objectType  { 0 };        // META_CATALOGS / META_SCHEMAS / META_TABLES
  CString   m_objectName;               // Name of the object
  CString   m_remarks;                  // From the COMMENT command
}
MetaObject;

using MMetaMap = std::vector<MetaObject>;

// Results for triggers - by SQLInfoDB!

typedef struct _metaTrigger
{
  CString   m_catalogName;              // Catalog name
  CString   m_schemaName;               // Schema name
  CString   m_tableName;                // Table name
  CString   m_triggerName;              // Trigger name
  CString   m_remarks;                  // Remarks from COMMENT ON
  int       m_position    { 0 };        // Position in firing
  bool      m_before      { false };    // true = BEFORE, false = AFTER
  bool      m_insert      { false };    // INSERT into table
  bool      m_update      { false };    // UPDATE of a table
  bool      m_delete      { false };    // DELETE from a table
  bool      m_select      { false };    // SELECT from a table
  bool      m_session     { false };    // ON CONNECT / ON DISCONNECT
  bool      m_transaction { false };    // ON BEGIN   / ON COMMIT
  bool      m_rollback    { false };    // ON ROLLBACK
  CString   m_referencing;              // Referencing names OLD and NEW
  bool      m_enabled     { false };    // Enabled or disabled
  CString   m_source;                   // Source of the trigger
}
MetaTrigger;

using MTriggerMap = std::vector<MetaTrigger>;

// Results for Sequences - By SQLInfoDB!

typedef struct _metaSequence
{
  CString   m_catalogName;              // Catalog name
  CString   m_schemaName;               // Schema name
  CString   m_sequenceName;             // Sequence name
  long      m_currentValue  { 1 };      // Current value (the sequence!)
  long      m_minimalValue  { 1 };      // Minimal value in the series
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
  CString     m_session;      // UNIQUE session number within RDBMS
  CString     m_user;         // User (OS or RDBMS user)
  CString     m_terminal;     // OS terminal, IP address or Net name
  // Newer fields
  CString     m_logonMoment;  // Timestamp of logon
  CString     m_remoteIP;     // Remote IP address
  CString     m_processName;  // Remote process name
  CString     m_processID;    // Remote process ID
}
MetaSession;

using MSessionMap = std::vector<MetaSession>;

typedef struct _metaParameter
{
  CString  m_parameter;       // Name of the parameters
  int      m_type;            // 0=input, 1=output, 2=in/out
}
MetaParameter;

using MParameterMap = std::vector<MetaParameter>;

};
