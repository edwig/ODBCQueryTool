#include "StdAfx.h"
#include "DDLCreateTable.h"

namespace SQLComponents
{

DDLCreateTable::DDLCreateTable(SQLInfoDB* p_info)
               :m_info(p_info)
{
}

CString 
DDLCreateTable::GetTableDDL(CString p_tableName)
{
  // Remember what was asked of us
  m_tableName = p_tableName;

  try
  {
    GetTableInfo();
    GetColumnInfo();
  }
  catch(CString& error)
  {
    m_ddl.Empty();
    throw CString("ERROR in getting table DDL: ") + error;
  }
  return m_ddl;
}

bool
DDLCreateTable::SaveDDL(CString p_filename)
{
  FILE* file = nullptr;
  fopen_s(&file,p_filename,"w");
  if(file)
  {
    fputs(m_ddl,file);
    fclose(file);
    return true;
  }
  return false;
}

void   
DDLCreateTable::GetTableInfo()
{
  // Get primary table info
  MTableMap tables;
  CString errors;

  // Find table info
  if(!m_info->MakeInfoTableTablepart(m_tableName,tables,errors) || tables.empty())
  {
    throw CString("Cannot find table: ") + m_tableName + " : " + errors;
  }

  // Construct table name
  if(!tables.front().m_schema.IsEmpty())
  {
    m_tableName = tables.front().m_schema + ".";
  }
  else
  {
    m_tableName = tables.front().m_table;
  }

  // Do our DDL part
  m_ddl  = "CREATE TABLE " + m_tableName;
  m_ddl += "\n(\n";
}

void   
DDLCreateTable::GetColumnInfo()
{
  MColumnMap columns;
  CString errors;
  bool    first = true;

  // Find column info
  if(!m_info->MakeInfoTableColumns(columns,errors) || columns.empty())
  {
    throw CString("Cannot find columns for table: ") + m_tableName + " : " + errors;
  }

  // Add columns
  for(auto& column : columns)
  {
    CString line("   ");
    line += first ? " " : ",";

    // Column name and type
    line += column.m_column;
    line += " ";
    line += column.m_typename;

    TypeInfo* type = m_info->GetTypeInfo(column.m_datatype);
    if(type)
    {
      line += ReplaceLengthPrecScale(type->m_create_params
                                    ,column.m_length
                                    ,column.m_precision
                                    ,column.m_scale);
    }
    // optional default value
    if(!column.m_default.IsEmpty() && column.m_default.CompareNoCase("null"))
    {
      line += " DEFAULT ";
      line += column.m_default;
    }
    // optional NOT NULL status
    if(column.m_nullable == SQL_NO_NULLS)
    {
      line += " NOT NULL";
    }
    m_ddl += line;
    m_ddl += "\n";
    first = false;
  }
  m_ddl += ");\n\n";
}

CString
DDLCreateTable::ReplaceLengthPrecScale(CString p_template
                                      ,int p_length
                                      ,int p_precision
                                      ,int p_scale)
{
  p_template.MakeLower();

  CString length,precision,scale;
  length   .Format("%d",p_length);
  precision.Format("%d",p_precision);
  scale    .Format("%d",p_scale);

  // Confusion in some drivers
  if(p_length == 0 && p_precision > 0 && p_template.Find("length") >= 0)
  {
    p_length = p_precision;
    p_precision = 0;
  }

  p_template.Replace("length",   length);
  p_template.Replace("precision",precision);
  p_template.Replace("scale" ,   scale);

  if(!p_template.IsEmpty() && p_template.Left(1) != "(")
  {
    p_template = "(" + p_template + ")";
  }
  return p_template;
}

};