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
  CString GetTableDDL(CString p_tableName);
  bool    SaveDDL(CString p_filename);

private:
  SQLInfoDB* m_info;
  CString    m_tableName;
  CString    m_ddl;

  // Primary formatting of 'create table' DDL

  void   GetTableInfo();
  void   GetColumnInfo();
  void   GetPrimaryKeyInfo();
  void   GetForeignKeyInfo();
  void   GetIndexInfo();
  void   GetAccessInfo();

  // Service routines

  CString ReplaceLengthPrecScale(CString p_template,int p_length,int p_precision,int p_scale);

};

};