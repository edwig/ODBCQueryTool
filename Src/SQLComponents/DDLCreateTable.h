////////////////////////////////////////////////////////////////////////
//
// File: DDLCreateTable.h
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
#include "SQLInfoDB.h"

namespace SQLComponents
{

class DDLCreateTable
{
public:
  DDLCreateTable(SQLInfoDB* p_info);

  // Request DDL for "table" or "schema.table" or "catalog.schema.table"
  // Where "table" can be of type: "TABLE" / "VIEW"
  CString GetTableDDL(CString p_tableName);
  bool    SaveDDL(CString p_filename);

private:
  // Primary formatting of 'create table' DDL
  void    GetTableInfo();
  void    GetColumnInfo();
  void    GetIndexInfo();
  void    GetPrimaryKeyInfo();
  void    GetForeignKeyInfo();
  void    GetTriggerInfo();
  void    GetSequenceInfo();
  void    GetAccessInfo();

  // Service routines

  void    StashTheLine(CString p_line,CString p_extraEnd = "",int p_newlines = 1);
  CString ReplaceLengthPrecScale(CString p_template,int p_length,int p_precision,int p_scale);
  CString FormatColumnName(CString p_column,int p_length);
  int     CalculateColumnLength(MColumnMap& p_columns);
  void    FindIndexFilter(MetaIndex& p_index);

  // Private data for the DDL creation
  SQLInfoDB* m_info;
  CString    m_schema;
  CString    m_tableName;
  CString    m_ddl;

  // Mappings
  MTableMap       m_tables;
  MColumnMap      m_columns;
  MIndicesMap     m_indices;
  MPrimaryMap     m_primaries;
  MForeignMap     m_foreigns;
  MTriggerMap     m_triggers;
  MSequenceMap    m_sequences;
  MPrivilegeMap   m_access;
};

};
