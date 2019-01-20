////////////////////////////////////////////////////////////////////////
//
// File: DDLCreateTable.cpp
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
  GetTableStatements(p_tableName);

  CString ddl;
  for(auto& statement : m_statements)
  {
    ddl += statement;
    ddl += ";\n";
  }
  return ddl;
}

DDLS
DDLCreateTable::GetTableStatements(CString p_tableName)
{
  // Remember what was asked of us
  m_tableName = p_tableName;

  try
  {
    GetTableInfo();
    GetColumnInfo();
    GetIndexInfo();
    GetPrimaryKeyInfo();
    GetForeignKeyInfo();
    GetTriggerInfo();
    GetSequenceInfo();
    GetAccessInfo();
  }
  catch(StdException& error)
  {
    m_statements.clear();
    throw error;
  }
  return m_statements;
}

bool
DDLCreateTable::SaveDDL(CString p_filename)
{
  FILE* file = nullptr;
  fopen_s(&file,p_filename,"w");
  if(file)
  {
    for(auto& ddl : m_statements)
    {
      ddl += ";\n";
      fputs(ddl,file);
    }
    fclose(file);
    return true;
  }
  return false;
}

// Internal delivery of all table information
void
DDLCreateTable::SetTableInfoTable(MTableMap& p_info)
{
  m_tables = p_info;
  m_hasTable = true;
}

void
DDLCreateTable::SetTableInfoColumns(MColumnMap& p_info)
{
  m_columns = p_info;
  m_hasColumns = true;
}

void
DDLCreateTable::SetTableInfoIndices(MIndicesMap& p_info)
{
  m_indices = p_info;
  m_hasIndices = true;
}

void    
DDLCreateTable::SetTableInfoPrimary(MPrimaryMap& p_info)
{
  m_primaries = p_info;
  m_hasPrimary = true;
}

void
DDLCreateTable::SetTableInfoForeign(MForeignMap& p_info)
{
  m_foreigns = p_info;
  m_hasForeigns = true;
}

void
DDLCreateTable::SetTableInfoTrigger(MTriggerMap& p_info)
{
  m_triggers = p_info;
  m_hasTriggers = true;
}

void
DDLCreateTable::SetTableInfoSequence(MSequenceMap& p_info)
{
  m_sequences = p_info;
  m_hasSequence = true;
}

void
DDLCreateTable::SetTableInfoPrivilege(MPrivilegeMap& p_info)
{
  m_access = p_info;
  m_hasPrivileges = true;
}

//////////////////////////////////////////////////////////////////////////

void   
DDLCreateTable::GetTableInfo()
{
  // Get primary table info
  CString errors;
  CString ddl;

  if(!m_hasTable)
  {
    // Find table info
    m_tables.clear();
    if(!m_info->MakeInfoTableTable(m_tables,errors,"",m_tableName) || m_tables.empty())
    {
      throw StdException(CString("Cannot find table: ") + m_tableName + " : " + errors);
    }
  }

  // Some engines get a synonym AND a table/view record
  // The table record always comes last.
  MetaTable& table = m_tables.back();

  // Construct table name
  if(!table.m_schema.IsEmpty())
  {
    m_schema = table.m_schema;
  }
  m_tableName = table.m_table;

  // Optional remarks to begin with
  if(!table.m_remarks.IsEmpty())
  {
    ddl = "-- " + table.m_remarks;
  }

  // Do our DDL part
  ddl += "CREATE " + table.m_objectType + " ";
  if(!m_schema.IsEmpty())
  {
    ddl += m_schema + ".";
  }
  ddl += m_tableName;
  ddl += "\n(\n";

  m_createDDL = ddl;
}

void   
DDLCreateTable::GetColumnInfo()
{
  CString errors;
  bool    first = true;

  if(!m_hasColumns)
  {
    // Find column info
    m_columns.clear();
    if(!m_info->MakeInfoTableColumns(m_columns,errors,m_schema,m_tableName) || m_columns.empty())
    {
      throw StdException(CString("Cannot find columns for table: ") + m_tableName + " : " + errors);
    }
  }

  // Calculate max length of a column
  int length = CalculateColumnLength(m_columns);

  // Add columns
  for(auto& column : m_columns)
  {
    CString line("   ");
    line += first ? " " : ",";

    // Column name and type
    line += FormatColumnName(column.m_column,length);
    line += " ";
    line += column.m_typename;

    TypeInfo* type = m_info->GetTypeInfo(column.m_datatype);
    if(type)
    {
      line += ReplaceLengthPrecScale(type->m_create_params
                                    ,column.m_columnSize
                                    ,column.m_columnSize
                                    ,column.m_decimalDigits);
    }
    // optional default value
    if(!column.m_default.IsEmpty() && 
        column.m_default.CompareNoCase("null") &&
        column.m_default.Compare("''") &&
        column.m_default.Compare("0"))
    {
      line += " DEFAULT ";
      if(column.m_default.Left(8).CompareNoCase("default ") == 0)
      {
        line += column.m_default.Mid(8);
      }
      else
      {
      line += column.m_default;
      }
    }
    // optional NOT NULL status
    if(column.m_nullable == SQL_NO_NULLS)
    {
      line += " NOT NULL";
    }
    // Optional remarks
    if(!column.m_remarks.IsEmpty())
    {
      line += " -- " + column.m_remarks;
    }

    // Stash the line
    m_createDDL += line + "\n";
    first = false;
  }
  m_createDDL += ")";
  StashTheLine(m_createDDL);
  m_createDDL.Empty();
}

void
DDLCreateTable::GetIndexInfo()
{
  CString errors;
  CString line;

  if(!m_hasIndices)
  {
    // Find column info
    m_indices.clear();
    m_info->MakeInfoTableStatistics(m_indices,errors,m_schema,m_tableName,nullptr);
    if(!errors.IsEmpty())
    {
      throw StdException(CString("Cannot find indices for table: ") + m_tableName + " : " + errors);
    }
  }

  // Walk the list of indices
  MIndicesMap theIndex;
  for(auto& index : m_indices)
  {
    // Skip the table statistics record
    if(index.m_indexType == SQL_TABLE_STAT)
    {
      continue;
    }

    // Eventually create a new index
    if(index.m_position == 1)
    {
      if(!theIndex.empty())
      {
        line = m_info->GetCATALOGIndexCreate(theIndex);
        StashTheLine(line);
      }
      // Create new index statement
      theIndex.clear();
    }
    // Add the column line
    FindIndexFilter(index);
    theIndex.push_back(index);
  }
  if(!theIndex.empty())
  {
    line = m_info->GetCATALOGIndexCreate(theIndex);
    StashTheLine(line);
  }
}

void
DDLCreateTable::GetPrimaryKeyInfo()
{
  CString errors;

  if(!m_hasPrimary)
  {
    // Find column info
    m_primaries.clear();
    m_info->MakeInfoTablePrimary(m_primaries,errors,m_schema,m_tableName);
    if(!errors.IsEmpty())
    {
      throw StdException(CString("Cannot find the primary key for table: ") + m_tableName + " : " + errors);
    }
  }

  if(!m_primaries.empty())
  {
    // Getting the alter table statement
    CString line = m_info->GetCATALOGPrimaryCreate(m_primaries);
    StashTheLine(line);
  }
}

void
DDLCreateTable::GetForeignKeyInfo()
{
  CString errors;
  CString line;

  if(!m_hasForeigns)
  {
    // Find column info
    m_foreigns.clear();
    m_info->MakeInfoTableForeign(m_foreigns,errors,m_schema,m_tableName);
    if(!errors.IsEmpty())
    {
      throw StdException(CString("Cannot find the foreign keys for table: ") + m_tableName + " : " + errors);
    }
  }

  // Do all foreign keys
  MForeignMap oneFKey;
  CString lastPrimTable;
  for(auto& foreign : m_foreigns)
  {
    if(foreign.m_pkTableName.Compare(lastPrimTable))
    {
      // start a new foreign key
      lastPrimTable = foreign.m_pkTableName;

      if(!oneFKey.empty())
      {
        line = m_info->GetCATALOGForeignCreate(oneFKey);
        StashTheLine(line);
      }
      oneFKey.clear();
    }
    // Keep this column
    oneFKey.push_back(foreign);
  }
  if(!oneFKey.empty())
  {
    line = m_info->GetCATALOGForeignCreate(oneFKey);
    StashTheLine(line);
  }
}

void
DDLCreateTable::GetTriggerInfo()
{
  CString errors;

  if(m_hasTriggers)
  {
    m_triggers.clear();
    m_info->MakeInfoTableTriggers(m_triggers,errors,m_schema,m_tableName);
    if(!errors.IsEmpty())
    {
      throw StdException(CString("Cannot find the triggers for table: ") + m_tableName + " : " + errors);
    }
  }

  // Print all triggers
  for(auto& trigger : m_triggers)
  {
    CString line = m_info->GetCATALOGTriggerCreate(trigger);
    StashTheLine(line);
  }
}

void
DDLCreateTable::GetSequenceInfo()
{
  CString errors;
  CString line;

  if(!m_hasSequence)
  {
    m_sequences.clear();
    m_info->MakeInfoTableSequences(m_sequences,errors,m_schema,m_tableName);
    if(!errors.IsEmpty())
    {
      throw StdException(CString("Cannot find the sequences for table: ") + m_tableName + " : " + errors);
    }
  }

  // Print all found sequences
  for(auto& seq : m_sequences)
  {
    line = m_info->GetCATALOGSequenceCreate(seq);
    StashTheLine(line);
  }
}

void
DDLCreateTable::GetAccessInfo()
{
  CString errors;
  CString line;

  if(!m_hasPrivileges)
  {
    // Find column info
    m_access.clear();
    m_info->MakeInfoTablePrivileges(m_access,errors,m_schema,m_tableName);
    if(!errors.IsEmpty())
    {
      throw StdException(CString("Cannot find the privileges for table: ") + m_tableName + " : " + errors);
    }
  }

  // Print all privileges
  for(auto& priv : m_access)
  {
    CString object;
    if(priv.m_schemaName.IsEmpty())
    {
      object = priv.m_tableName;
    }
    else
    {
      object = priv.m_schemaName + "." + priv.m_tableName;
    }

    // Primary privilege
    line.Format("GRANT %s ON %s TO %s",priv.m_privilege.GetString(),object.GetString(),priv.m_grantee.GetString());
    if(priv.m_grantable)
    {
      line += " WITH GRANT OPTION";
    }
    StashTheLine(line);
  }
}

//////////////////////////////////////////////////////////////////////////
//
// SERVICE ROUTINES
//
//////////////////////////////////////////////////////////////////////////

void
DDLCreateTable::StashTheLine(CString p_line)
{
  // See if we have work to do
  if(p_line.IsEmpty())
  {
    return;
  }

  m_statements.push_back(p_line);
}

CString
DDLCreateTable::ReplaceLengthPrecScale(CString p_template
                                       ,int p_length
                                       ,int p_precision
                                       ,int p_scale)
{
  // Set in lowercase for replacing
  p_template.MakeLower();

  // Confusion in some drivers
  if(p_length == 0 && p_precision > 0 && p_template.Find("length") >= 0)
  {
    p_length = p_precision;
    p_precision = 0;
  }

  // Format as strings
  CString length,precision,scale;
  length.Format("%d",p_length);
  precision.Format("%d",p_precision);
  scale.Format("%d",p_scale);

  // Replace as strings
  p_template.Replace("max length",length);    // ORACLE DOES THIS!!
  p_template.Replace("length",    length);
  p_template.Replace("precision", precision);
  p_template.Replace("scale",     scale);

  // Make sure we have parenthesis
  if(!p_template.IsEmpty() && p_template.Left(1) != "(")
  {
    p_template = "(" + p_template + ")";
  }
  return p_template;
}

CString
DDLCreateTable::FormatColumnName(CString p_column,int p_length)
{
  while(p_column.GetLength() < p_length)
  {
    p_column += " ";
  }
  return p_column;
}

int
DDLCreateTable::CalculateColumnLength(MColumnMap& p_columns)
{
  int length = 0;

  for(auto& column : p_columns)
  {
    if(column.m_column.GetLength() > length)
    {
      length = column.m_column.GetLength();
    }
  }
  return length;
}

void
DDLCreateTable::FindIndexFilter(MetaIndex& p_index)
{
  // See if column name in index exists in the table
  for(auto& column : m_columns)
  {
    if(p_index.m_columnName.CompareNoCase(column.m_column) == 0)
    {
      return;
    }
  }
  // Not existing column. Find index filter
  p_index.m_filter = m_info->GetCATALOGIndexFilter(p_index);
}

};