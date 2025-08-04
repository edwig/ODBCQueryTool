////////////////////////////////////////////////////////////////////////
//
// File: SQLMetaInfo.h
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

#define MetaTable_catalogname    1
#define MetaTable_schemaname     2
#define MetaTable_tablename      3
#define MetaTable_objecttype     4
#define MetaTable_remarks        5
#define MetaTable_fullname       6
#define MetaTable_tablespace     7
#define MetaTable_temporary      8

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

  void GetPrecisionAndScale(XString& p_sql) const
  {
    if(m_columnSize > 0)
    {
      p_sql.AppendFormat(_T("(%d"),m_columnSize);
      if(m_decimalDigits > 0)
      {
        p_sql.AppendFormat(_T(",%d"),m_decimalDigits);
      }
      p_sql += _T(")");
    }
  };
  void GetNullable(XString& p_sql) const
  {
    if(!m_nullable)
    {
      p_sql += _T(" NOT NULL");
    }
  };
  void GetDefault(XString& p_sql) const
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

#define MetaColumn_catalogname      1
#define MetaColumn_schemaname       2
#define MetaColumn_tablename        3
#define MetaColumn_columnname       4
#define MetaColumn_datatype         5
#define MetaColumn_typename         6
#define MetaColumn_columnsize       7
#define MetaColumn_bufferlength     8
#define MetaColumn_decimaldigits    9
#define MetaColumn_radix           10
#define MetaColumn_nullable        11
#define MetaColumn_remarks         12
#define MetaColumn_default         13
#define MetaColumn_datatype3       14
#define MetaColumn_subdatatype     15
#define MetaColumn_octetlength     16
#define MetaColumn_position        17
#define MetaColumn_isnullable      18

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

// MetaPrimary fields
#define MetaPrimary_catalogname   1
#define MetaPrimary_schemaname    2
#define MetaPrimary_tablename     3
#define MetaPrimary_columname     4
#define MetaPrimary_keysequence   5
#define MetaPrimary_constraint    6

// Results from "SQLForeignKeys"

typedef struct _metaInfoForeign
{
  // Primary table
  XString   m_pkCatalogName;            // 01 Primary key catalog name
  XString   m_pkSchemaName;             // 02 Primary key schema name
  XString   m_pkTableName;              // 03 Primary key table name
  XString   m_pkColumnName;             // 04 Corresponding column in the primary key table
  // Foreign key table
  XString   m_fkCatalogName;            // 05 Foreign key catalog name
  XString   m_fkSchemaName;             // 06 Foreign key schema name
  XString   m_fkTableName;              // 07 Foreign key table name
  XString   m_fkColumnName;             // 08 Corresponding column of the foreign key
  // The column
  int       m_keySequence       { 0 };  // 09 Ordinal sequence number
  // Options of the foreign key
  int       m_updateRule        { 0 };  // 10 SQL_CASCADE(0) / SQL_NO_ACTION(3) / SQL_SET_NULL(2) / SQL_SET_DEFAULT(4) / SQL_RESTRICT(1)
  int       m_deleteRule        { 0 };  // 11 SQL_CASCADE(0) / SQL_NO_ACTION(3) / SQL_SET_NULL(2) / SQL_SET_DEFAULT(4) / SQL_RESTRICT(1)
  // Constraints naming
  XString   m_foreignConstraint;        // 12 Name of the foreign key constraint
  XString   m_primaryConstraint;        // 13 Name of the primary key constraint
  int       m_deferrable        { 0 };  // 14 SQL_INITIALLY_DEFERRED(5) / SQL_INITIALLY_IMMEDIATE(6) / SQL_NOT_DEFERRABLE(7)
  // Extra info by SQL
  int       m_match             { 0 };  // 15 SQL_MATCH_FULL(0) / SQL_MATCH_PARTIAL(1) / SQL_MATCH_SIMPLE(2)
  int       m_initiallyDeferred { 0 };  // 16 0=Immediate, 1=initially deferred
  int       m_enabled           { 1 };  // 17 1=Disabled,  0=enabled
}
MetaForeign;

using MForeignMap = std::vector<MetaForeign>;

// MetaForeign fields
#define MetaForeign_pk_catalogname  1
#define MetaForeign_pk_schemaname   2
#define MetaForeign_pk_tablename    3
#define MetaForeign_pk_columnname   4
#define MetaForeign_fk_catalogname  5
#define MetaForeign_fk_schemaname   6
#define MetaForeign_fk_tablename    7
#define MetaForeign_fk_columname    8
#define MetaForeign_keysequence     9
#define MetaForeign_updaterule     10
#define MetaForeign_deleterule     11
#define MetaForeign_fk_constraint  12
#define MetaForeign_pk_constraint  13
#define MetaForeign_deferrable     14
// Extra via SQL
#define MetaForeign_matching       15
#define MetaForeign_initdeferred   16
#define MetaForeign_enabled        17

// Results from "SQLStatistics"

typedef struct _metaInfoIndex
{
  XString   m_catalogName;              // 01 Catalog (database name)
  XString   m_schemaName;               // 02 Schema owner
  XString   m_tableName;                // 03 Table name
  bool      m_nonunique   { true };     // 04 false = UNIQUE
  XString   m_dropQualifier;            // 05 NULL=catalog, otherwise use in drop
  XString   m_indexName;                // 06 Index name
  int       m_indexType   { 0 };        // 07 SQL_TABLE_STAT / SQL_INDEX_CLUSTERED / SQL_INDEX_HASHED / SQL_INDEX_OTHER
  int       m_position    { 0 };        // 08 Ordinal position (start at 1)
  XString   m_columnName;               // 09 Column in the index
  XString   m_ascending;                // 10 'A', 'D', '' = unknown
  long      m_cardinality { 0 };        // 11 cardinality of index
  long      m_pages       { 0 };        // 12 Pages of the index
  XString   m_filter;                   // 13 filter or expression
}
MetaIndex;

using MIndicesMap = std::vector<MetaIndex>;

// MetaIndex fields
#define MetaIndex_catalogname   1
#define MetaIndex_schemaname    2
#define MetaIndex_tablename     3
#define MetaIndex_nonunique     4
#define MetaIndex_dropqualifier 5
#define MetaIndex_indexname     6
#define MetaIndex_type          7
#define MetaIndex_position      8
#define MetaIndex_columname     9
#define MetaIndex_ascending    10
#define MetaIndex_cardinality  11
#define MetaIndex_pages        12
#define MetaIndex_filter       13

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

#define MetaSpecial_columname     1
#define MetaSpecial_typename      2
#define MetaSpecial_datatype      3
#define MetaSpecial_columnsize    4
#define MetaSpecial_buffersize    5
#define MetaSpecial_decimaldigits 6
#define MetaSpecial_scope         7
#define MetaSpecail_pseudo        8

// Check Constraints (NOT IN THE ODBC STANDARD (yet))

typedef struct _metaCheckConstraints
{
  XString m_catalogName;                // Catalog of the constraint
  XString m_schemaName;                 // Schema of the constraint
  XString m_tableName;                  // Table/object of the constraint
  XString m_costraintName;              // Constraint name in ADD/DROP
  XString m_checkCondition;             // The condition (with or without 'CHECK')
}
MetaCheckConstraint;

using MCheckConstraintMap = std::vector<MetaCheckConstraint>;

// Column numbers
#define MetaCheck_catalogname    1
#define MetaCheck_schemaname     2
#define MetaCheck_tablename      3
#define MetaCheck_constraintname 4
#define MetaCheck_condition      5

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

#define MetaPrivilege_catalogname     1
#define MetaPrivilege_schemaname      2
#define MetaPrivilege_tablename       3
#define MetaPrivilege_columname       4
#define MetaPrivilege_grantor         5
#define MetaPrivilege_grantee         6
#define MetaPrivilege_privilege       7
#define MetaPrivilege_grantable       8

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

// MetaProcedure fields
#define MetaProcedure_catalogname     1
#define MetaProcedure_schemaname      2
#define MetaProcedure_procedurename   3
#define MetaProcedure_inputargumetns  4
#define MetaProcedure_outputargumetns 5
#define MetaProcedure_resultsets      6
#define MetaProcedure_remarks         7
#define MetaProcedure_type            8
#define MetaProcedure_source          9

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

#define MetaParameter_catalogname       1
#define MetaParameter_schemaname        2
#define MetaParameter_procedurename     3
#define MetaParameter_parametername     4
#define MetaParameter_columntype        5
#define MetaParameter_datatype          6
#define MetaParameter_typename          7
#define MetaParameter_columnsize        8
#define MetaParameter_bufferlength      9
#define MetaParameter_decimaldigits    10
#define MetaParameter_radix            11
#define MetaParameter_nullable         12
#define MetaParameter_remarks          13
#define MetaParameter_default          14
#define MetaParameter_datatype3        15
#define MetaParameter_subtype          16
#define MetaParameter_octetlength      17
#define MetaParameter_position         18
#define MetaParameter_isnullable       19

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

// MetaTrigger fields
#define MetaTrigger_catalogname     1
#define MetaTrigger_schemaname      2
#define MetaTrigger_tablename       3
#define MetaTrigger_triggername     4
#define MetaTrigger_remarks         5
#define MetaTrigger_position        6
#define MetaTrigger_before          7
#define MetaTrigger_insert          8
#define MetaTrigger_update          9
#define MetaTrigger_delete         10
#define MetaTrigger_select         11
#define MetaTrigger_session        12
#define MetaTrigger_transaction    13
#define MetaTrigger_rollback       14
#define MetaTrigger_referencing    15
#define MetaTrigger_enabled        16
#define MetaTrigger_source         17

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
  XString   m_remarks;
}
MetaSequence;

using MSequenceMap = std::vector<MetaSequence>;

#define MetaSequence_catalogname     1
#define MetaSequence_schemaname      2
#define MetaSequence_sequencename    3
#define MetaSequence_currentvalue    4
#define MetaSequence_minimalvalue    5
#define MetaSequence_increment       6
#define MetaSequence_cache           7
#define MetaSequence_cycle           8
#define MetaSequence_order           9
#define MetaSequence_remarks        10

// Results for synonyms

typedef struct _metaSynonym
{
  XString m_catalogName;        // Catalog name
  XString m_schemaName;         // Schema name
  XString m_synonymName;        // Synonym name
  XString m_objectType;         // Object type ('SYNONYM' if found)
  XString m_remarks;            // Comment ON
  XString m_tablespace;         // Tablespace
  int     m_temporary;          // 1 = temporary, 0 = permanent
}
MetaSynonym;

using MSynonymMap = std::vector<MetaSynonym>;

#define MetaSynonym_catalogname       1
#define MetaSynonym_schemaname        2
#define MetaSynonym_synonymname       3
#define MetaSynonym_objecttype        4
#define MetaSynonym_remarks           5
#define MetaSynonym_tablespace        6
#define MetaSynonym_temporary         7

// Results for datatypes

typedef struct _metaUserType
{
  XString m_catalogName; // Catalog name
  XString m_schemaName;  // Schema name
  XString m_typeName;    // Type name
  XString m_usage;       // 'C'= 'Composite', 'E'= 'Enum', 'D'= 'Domain-check'
  long    m_ordinal;     // Ordinal sequence within the datatype (1-based) (for 'C' and 'E')
  XString m_attribute;   // attribute name or enumerator value !!
  XString m_datatype;    // datatype for attribute
  bool    m_notnull;     // notnull  for attribute
  XString m_default;     // default  for domain
  XString m_domaincheck; // check()  for domain
  XString m_remarks;     // Comment ON
  XString m_source;      // Source for reconstruction
}
MetaUserType;

using MUserTypeMap = std::vector<MetaUserType>;

#define MetaType_catalogname          1
#define MetaType_schemaname           2
#define MetaType_typename             3
#define MetaType_usage                4
#define MetaType_ordinal              5
#define MetaType_attribute            6
#define MetaType_datatype             7
#define MetaType_notnull              8
#define MetaType_default              9
#define MetaType_domaincheck         10
#define MetaType_remarks             11
#define MetaType_source              12

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
