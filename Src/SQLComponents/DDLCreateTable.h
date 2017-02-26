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
  void    GetAccessInfo();

  // Service routines

  void    StashTheLine(CString p_line,CString p_extraEnd = "",int p_newlines = 1);
  CString ReplaceLengthPrecScale(CString p_template,int p_length,int p_precision,int p_scale);
  CString FormatColumnName(CString p_column,int p_length);
  int     CalculateColumnLength(MColumnMap& p_columns);
  void    FindIndexFilter(MetaStatistics& p_index);

  // Private data for the DDL creation
  SQLInfoDB* m_info;
  CString    m_tableName;
  CString    m_ddl;

  // Mappings
  MTableMap       m_tables;
  MColumnMap      m_columns;
  MStatisticsMap  m_indices;
  MPrimaryMap     m_primaries;
  MForeignMap     m_foreigns;
  MTriggerMap     m_triggers;
  MPrivilegeMap   m_access;
};

};
