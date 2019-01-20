////////////////////////////////////////////////////////////////////////
//
// File: DDLCreateTable.h
//
// Copyright (c) 1998-2018 ir. W.E. Huisman
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
// Last Revision:   20-01-2019
// Version number:  1.5.4
//
#pragma once
#include "SQLComponents.h"
#include "SQLInfoDB.h"
#include <vector>

namespace SQLComponents
{

using DDLS = std::vector<CString>;

class DDLCreateTable
{
public:
  DDLCreateTable(SQLInfoDB* p_info);

  // Request DDL for "table" or "schema.table" or "catalog.schema.table"
  // Where "table" can be of type: "TABLE" / "VIEW"
  CString GetTableDDL(CString p_tableName);
  DDLS    GetTableStatements(CString p_tablename);
  bool    SaveDDL(CString p_filename);

  // Internal delivery of all table information
  void    SetTableInfoTable    (MTableMap&     p_info);
  void    SetTableInfoColumns  (MColumnMap&    p_info);
  void    SetTableInfoIndices  (MIndicesMap&   p_info);
  void    SetTableInfoPrimary  (MPrimaryMap&   p_info);
  void    SetTableInfoForeign  (MForeignMap&   p_info);
  void    SetTableInfoTrigger  (MTriggerMap&   p_info);
  void    SetTableInfoSequence (MSequenceMap&  p_info);
  void    SetTableInfoPrivilege(MPrivilegeMap& p_info);

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

  void    StashTheLine(CString p_line);
  CString ReplaceLengthPrecScale(CString p_template,int p_length,int p_precision,int p_scale);
  CString FormatColumnName(CString p_column,int p_length);
  int     CalculateColumnLength(MColumnMap& p_columns);
  void    FindIndexFilter(MetaIndex& p_index);

  // Private data for the DDL creation
  SQLInfoDB* m_info;
  CString    m_schema;
  CString    m_tableName;
  DDLS       m_statements;
  CString    m_createDDL;

  // Mappings
  MTableMap       m_tables;
  MColumnMap      m_columns;
  MIndicesMap     m_indices;
  MPrimaryMap     m_primaries;
  MForeignMap     m_foreigns;
  MTriggerMap     m_triggers;
  MSequenceMap    m_sequences;
  MPrivilegeMap   m_access;

  // Info gotten
  bool m_hasTable      { false };
  bool m_hasColumns    { false };
  bool m_hasIndices    { false };
  bool m_hasPrimary    { false };
  bool m_hasForeigns   { false };
  bool m_hasTriggers   { false };
  bool m_hasSequence   { false };
  bool m_hasPrivileges { false };
};

};
