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
  
// Contains all types for META SQLInfo inquiries to the ODBC Driver
// About the objects in the database system catalog.

// Results from "SQLTables"

typedef struct _metaInfoTable
{
  CString   m_catalog;          // Catalog name (database name)
  CString   m_schema;           // Schema name (owner of the object)
  CString   m_table;            // Table/view/synonym name
  CString   m_objectType;       // Type of the object
  CString   m_remarks;          // Using COMMENTS command
  CString   m_fullObjectName;   // Full object name, conforming RDBMS rules
                                // eg: type:catalog.schema.table
                                // or: type:schema.table@catalog
}
MetaTable;

using MTableMap = std::vector<MetaTable>;

// Results from "SQLColumns"

typedef struct _metaInfoColumn
{
  CString   m_catalog;          // Catalog name (database name)
  CString   m_schema;           // Schema name (owner of the object)
  CString   m_table;            // Table/view/synonym name
  CString   m_column;           // Column name
  CString   m_remarks;          // Using COMMENTS command
  int       m_datatype;         // SQL Data type
  CString   m_typename;         // Data type name (according to RDBMS)
  int       m_precision;        // Precision
  int       m_length;           // Total length in bytes
  int       m_scale;            // Decimal/Numeric scale
  int       m_nullable;         // Nullable SQL_NO_NULLS / SQL_NULLABLE / SQL_NULLABLE_UNKNOWN
}
MetaColumn;

using MColumnMap = std::vector<MetaColumn>;

// Results from "SQLPrimaryKeys" 

typedef struct _metaInfoPrimary
{
  CString  m_catalog;
  CString  m_schema;
  CString  m_table;
  CString  m_columnName;        // Name of the column of the primary key
  int      m_columnPosition;    // Position of the column in the table
  CString  m_constraintName;    // Name of the primary key constraint
}
MetaPrimary;

using MPrimaryMap = std::vector<MetaPrimary>;


// Results from "SQLForeignKeys"

typedef struct _metaInfoForeign
{
  CString   m_pkCatalogName;
  CString   m_pkSchemaName;
  CString   m_pkTableName;
  CString   m_pkColumnName;

  CString   m_fkCatalogName;
  CString   m_fkSchemaName;
  CString   m_fkTableName;
  CString   m_fkColumnName;
  
  int       m_keySequence;        // Ordinal sequence number
  int       m_updateRule;         // SQL_CASCADE / SQL_NO_ACTION / SQL_SET_NULL / SQL_SET_DEFAULT
  int       m_deleteRule;         // SQL_CASCADE / SQL_NO_ACTION / SQL_SET_NULL / SQL_SET_DEFAULT
  CString   m_foreignConstraint;
  CString   m_primaryConstraint;
  int       m_deferrable;         // SQL_INITIALLY_DEFERRED / SQL_INITIALLY_IMMEDIATE / SQL_NOT_DEFERRABLE
}
MetaForeign;

using MForeignMap = std::vector<MetaForeign>;

// Results from "SQLStatistics"

typedef struct _metaInfoStatistics
{
  CString   m_catalogName;    // Catalog (database name)
  CString   m_schemaName;     // Schema owner
  CString   m_tableName;      // Tablename
  bool      m_unique;         // true = UNIQUE
  CString   m_indexName;      // Index name
  int       m_indexType;      // SQL_TABLE_STAT / SQL_INDEX_CLUSTERED / SQL_INDEX_HASHED / SQL_INDEX_OTHER
  int       m_position;       // Ordinal position (start at 1)
  CString   m_columnName;     // Column in the index
  CString   m_ascending;      // 'A', 'D', '' = unknown
  long      m_cardinality;    // cardinality of index
  long      m_pages;          // Pages of the index
  CString   m_filter;         // filter or expression
}
MetaStatistics;

using MStatisticsMap = std::vector<MetaStatistics>;

// Results from "SQLSpecialColumns"

typedef struct _metaSpecialColumns
{
  CString   m_columnName;     // Extra column name by RDBMS
  CString   m_typeName;       // Different datatype by RDBMS
  int       m_datatype;       // Corresponding ODBC Datatype
  int       m_columnSize;
  int       m_bufferSize;
  int       m_decimalDigits;
  int       m_scope;          // SQL_SCOPE_CURROW  / SQL_SCOPE_TRANSACTION / SQL_SCOPE_SESSION
  int       m_pseudo;         // SQL_PC_NOT_PSEUDO / SQL_PC_PSEUDO / SQL_PC_UNKNOWN
}
MetaSpecialColumn;

using MSpecialColumnMap = std::vector<MetaSpecialColumn>;

// Results from "SQLTablePrivileges" 

typedef struct _metaInfoPrivilege
{
  CString   m_catalogName;
  CString   m_schemaName;
  CString   m_tableName;
  CString   m_grantor;
  CString   m_grantee;
  CString   m_privilege;
  bool      m_grantable;
}
MetaPrivilege;

using MPrivilegeMap = std::vector<MetaPrivilege>;

// Results from "SQLProcedures"

typedef struct _metaInfoProcedure
{
  CString   m_catalogName;
  CString   m_schemaName;
  CString   m_procedureName;
  int       m_inputParameters;    // Number of input parameters
  int       m_outputParameters;   // Number of output parameters
  int       m_resultSets;         // Number of result sets
  CString   m_remarks;            // As in COMMENT command
  int       m_procedureType;      // SQL_PT_PROCEDURE / SQL_PT_FUNCTION / SQL_PT_UNKNOWN
}
MetaProcedure;

using MProcedureMap = std::vector<MetaProcedure>;

// Results from "SQLProcedureColumns"

typedef struct _metaInfoProcColumns
{
  CString   m_catalogName;
  CString   m_schemaName;
  CString   m_procedureName;
  CString   m_columnName;
  int       m_ordinalPosition;
  int       m_columnType;       // SQL_PARAM_INPUT / SQL_PARAM_OUTPUT etc. etc.
  int       m_dataType;         // ODBC standard type name
  CString   m_typeName;         // RDBMS type name
  int       m_columnSize;
  int       m_bufferSize;
  int       m_decimalDigits;
  int       m_radix;
  int       m_nullable;         // SQL_NO_NULLS / SQL_NULLABLE / SQL_NULLABLE_UNKNOWN
  CString   m_remarks;          // From the COMMENT command
  CString   m_defaultValue;     // Default value
  CString   m_isNullable;       // "NO" or "YES" (can include nulls)
}
MetaProcedureColumn;

using MProcColumnMap = std::vector<MetaProcedureColumn>;

// Results for Meta objects in the catalog(s)

typedef struct _metaInfoObject
{
  int       m_objectType;       // META_CATALOGS / META_SCHEMAS / META_TABLES
  CString   m_objectName;       // Name of the object
  CString   m_remarks;          // From the COMMENT command
}
MetaObject;

using MMetaMap = std::vector<MetaObject>;

};
