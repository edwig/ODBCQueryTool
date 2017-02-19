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

};
