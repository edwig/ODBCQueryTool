////////////////////////////////////////////////////////////////////////
//
// File: DDLCreateTable.h
//
// Copyright (c) 1998-2021 ir. W.E. Huisman
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
#include "SQLInfoDB.h"
#include <deque>

namespace SQLComponents
{

using DDLS = std::deque<CString>;

class DDLCreateTable
{
public:
  DDLCreateTable(SQLInfoDB* p_info);

  // Request DDL for "table" or "schema.table" 
  // Where "table" can be of type: "TABLE" / "VIEW"
  CString GetTableDDL(CString p_tableName);
  bool    SaveDDL(CString p_filename);
  DDLS    GetTableStatements(CString p_tablename);
  DDLS    GetTableStatements(CString p_tableName
                            ,bool p_columns
                            ,bool p_options
                            ,bool p_indices
                            ,bool p_primaries
                            ,bool p_foreigns
                            ,bool p_triggers
                            ,bool p_sequences
                            ,bool p_access);
  DDLS    GetViewStatements(CString p_viewname);

  // Internal delivery of all table information
  void    SetTableInfoTable    (MTableMap&     p_info);
  void    SetTableInfoColumns  (MColumnMap&    p_info);
  void    SetTableInfoIndices  (MIndicesMap&   p_info);
  void    SetTableInfoPrimary  (MPrimaryMap&   p_info);
  void    SetTableInfoForeign  (MForeignMap&   p_info);
  void    SetTableInfoTrigger  (MTriggerMap&   p_info);
  void    SetTableInfoSequence (MSequenceMap&  p_info);
  void    SetTableInfoPrivilege(MPrivilegeMap& p_info);

  // Setting of special members
  void    SetInfoDB(SQLInfoDB* p_info);
  void    SetTablesSchema(CString p_schema);
  void    SetTableTablespace(CString p_tablespace);
  void    SetIndexTablespace(CString p_tablespace);

private:
  // Primary formatting of 'create table' DDL
  void    GetTableInfo();
  void    GetViewInfo();
  void    GetColumnInfo();
  void    GetOptionsInfo();
  void    GetIndexInfo();
  void    GetPrimaryKeyInfo();
  void    GetForeignKeyInfo();
  void    GetTriggerInfo();
  void    GetSequenceInfo();
  void    GetAccessInfo(bool p_strict = false);

  // Service routines

  bool    FindSchemaName(CString p_tableName);
  void    StashTheLine(CString p_line);
  CString ReplaceLengthPrecScale(CString p_template,int p_length,int p_precision,int p_scale);
  CString FormatColumnName(CString p_column,int p_length);
  int     CalculateColumnLength(MColumnMap& p_columns);
  void    FindIndexFilter(MetaIndex& p_index);
  bool    IsStrictODBCPrivilege(CString p_privilege);

  // Private data for the DDL creation
  SQLInfoDB* m_info;
  CString    m_schema;
  CString    m_tableName;
  CString    m_indexTablespace;
  DDLS       m_statements;
  CString    m_createDDL;

  // Info gotten
  bool m_didTable      { false };
  bool m_didColumns    { false };
  bool m_didOptions    { false };
  bool m_didIndices    { false };
  bool m_didPrimary    { false };
  bool m_didForeigns   { false };
  bool m_didTriggers   { false };
  bool m_didSequence   { false };
  bool m_didPrivileges { false };

public:
  // Mappings are public. We may examine/change them directly!
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
