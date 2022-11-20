////////////////////////////////////////////////////////////////////////
//
// File: DDLCreateTable.h
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
#include "SQLInfoDB.h"
#include <deque>

namespace SQLComponents
{

using DDLS = std::deque<XString>;

class DDLCreateTable
{
public:
  DDLCreateTable(SQLInfoDB* p_info,SQLInfoDB* p_target = nullptr);

  // Request DDL for "table" or "schema.table" 
  // Where "table" can be of type: "TABLE" / "VIEW"
  XString GetTableDDL(XString p_tableName);
  bool    SaveDDL(XString p_filename);
  DDLS    GetTableStatements(XString p_tablename);
  DDLS    GetTableStatements(XString p_tableName
                            ,bool p_columns
                            ,bool p_options
                            ,bool p_indices
                            ,bool p_primaries
                            ,bool p_foreigns
                            ,bool p_triggers
                            ,bool p_sequences
                            ,bool p_access);
  DDLS    GetViewStatements(XString p_viewname);

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
  void    SetTablesSchema(XString p_schema);
  void    SetTableTablespace(XString p_tablespace);
  void    SetIndexTablespace(XString p_tablespace);

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

  bool    FindSchemaName(XString p_tableName);
  void    StashTheLine(XString p_line);
  XString ReplaceLengthPrecScale(XString p_template,int p_length,int p_precision,int p_scale);
  XString FormatColumnName(XString p_column,int p_length);
  int     CalculateColumnLength(MColumnMap& p_columns);
  void    FindIndexFilter(MetaIndex& p_index);
  bool    IsStrictODBCPrivilege(XString p_privilege);

  // Private data for the DDL creation
  SQLInfoDB* m_info;
  SQLInfoDB* m_target;
  XString    m_schema;
  XString    m_tableName;
  XString    m_indexTablespace;
  DDLS       m_statements;
  XString    m_createDDL;

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
