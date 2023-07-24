////////////////////////////////////////////////////////////////////////
//
// File: DDLCreateTable.cpp
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
#include "StdAfx.h"
#include "DDLCreateTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{

DDLCreateTable::DDLCreateTable(SQLInfoDB* p_info,SQLInfoDB* p_target /*=nullptr*/)
               :m_info(p_info)
               ,m_target(p_target)
{
}

XString 
DDLCreateTable::GetTableDDL(XString p_tableName)
{
  GetTableStatements(p_tableName);

  XString ddl;
  for(auto& statement : m_statements)
  {
    ddl += statement;
    ddl += ";\n";
  }
  return ddl;
}

DDLS
DDLCreateTable::GetTableStatements(XString p_tableName)
{
  // Split schema name and table name
  FindSchemaName(p_tableName);

  GetTableInfo();
  GetColumnInfo();
  GetOptionsInfo();
  GetPrimaryKeyInfo();
  GetIndexInfo();
  GetForeignKeyInfo();
  GetTriggerInfo();
  GetSequenceInfo();
  GetAccessInfo();

  return m_statements;
}

DDLS
DDLCreateTable::GetTableStatements(XString p_tableName
                                  ,bool p_columns
                                  ,bool p_options
                                  ,bool p_indices
                                  ,bool p_primaries
                                  ,bool p_foreigns
                                  ,bool p_triggers
                                  ,bool p_sequences
                                  ,bool p_access)
{
  // Split schema name and table name
  FindSchemaName(p_tableName);

  GetTableInfo();
  if(p_columns)   GetColumnInfo();      else m_didColumns    = true;
  if(p_options)   GetOptionsInfo();     else m_didOptions    = true;
  if(p_primaries) GetPrimaryKeyInfo();  else m_didPrimary    = true;
  if(p_indices)   GetIndexInfo();       else m_didIndices    = true;
  if(p_foreigns)  GetForeignKeyInfo();  else m_didForeigns   = true;
  if(p_triggers)  GetTriggerInfo();     else m_didTriggers   = true;
  if(p_sequences) GetSequenceInfo();    else m_didSequence   = true;
  if(p_access)    GetAccessInfo();      else m_didPrivileges = true;

  return m_statements;
}

DDLS    
DDLCreateTable::GetViewStatements(XString p_viewname)
{
  // Split schema name and view name
  FindSchemaName(p_viewname);

  GetViewInfo();
  GetAccessInfo(true);

  return m_statements;
}

bool
DDLCreateTable::SaveDDL(XString p_filename)
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
  m_didTable = true;
}

void
DDLCreateTable::SetTableInfoColumns(MColumnMap& p_info)
{
  m_columns = p_info;
  m_didColumns = true;
}

void
DDLCreateTable::SetTableInfoIndices(MIndicesMap& p_info)
{
  m_indices = p_info;
  m_didIndices = true;
}

void    
DDLCreateTable::SetTableInfoPrimary(MPrimaryMap& p_info)
{
  m_primaries = p_info;
  m_didPrimary = true;
}

void
DDLCreateTable::SetTableInfoForeign(MForeignMap& p_info)
{
  m_foreigns = p_info;
  m_didForeigns = true;
}

void
DDLCreateTable::SetTableInfoTrigger(MTriggerMap& p_info)
{
  m_triggers = p_info;
  m_didTriggers = true;
}

void
DDLCreateTable::SetTableInfoSequence(MSequenceMap& p_info)
{
  m_sequences = p_info;
  m_didSequence = true;
}

void
DDLCreateTable::SetTableInfoPrivilege(MPrivilegeMap& p_info)
{
  m_access = p_info;
  m_didPrivileges = true;
}

// Set to a different SQLInfoDB to create tables
// for another database. Use with extreme care!!
void
DDLCreateTable::SetInfoDB(SQLInfoDB* p_info)
{
  if(p_info)
  {
    m_info = p_info;
    m_statements.clear();
  }
}

// Change the schema of the table
void
DDLCreateTable::SetTablesSchema(XString p_schema)
{
  for(auto& table : m_tables)
  {
    table.m_schema = p_schema;
  }
  for(auto& column : m_columns)
  {
    column.m_schema = p_schema;
  }
  for(auto& index : m_indices)
  {
    index.m_schemaName = p_schema;
  }
  for(auto& primary : m_primaries)
  {
    primary.m_schema = p_schema;
  }
  for(auto& foreign : m_foreigns)
  {
    foreign.m_fkSchemaName = p_schema;
    foreign.m_pkSchemaName = p_schema;
  }
  for(auto& trigger : m_triggers)
  {
    trigger.m_schemaName = p_schema;
  }
  for(auto& sequence : m_sequences)
  {
    sequence.m_schemaName = p_schema;
  }
  for(auto& access : m_access)
  {
    access.m_schemaName = p_schema;
  }
}

void
DDLCreateTable::SetTableTablespace(XString p_tablespace)
{
  for (auto& table : m_tables)
  {
    table.m_tablespace = p_tablespace;
  }
}

void
DDLCreateTable::SetIndexTablespace(XString p_tablespace)
{
  m_indexTablespace = p_tablespace;
}

void
DDLCreateTable::SetOptionIndexDuplicateNulls(bool p_duplicate)
{
  m_indexDuplicateNulls = p_duplicate;
}

void
DDLCreateTable::SetOptionDropIfExists(bool p_drop)
{
  m_dropIfExists = p_drop;
}

//////////////////////////////////////////////////////////////////////////

void   
DDLCreateTable::GetTableInfo()
{
  // Get primary table info
  XString errors;
  XString ddl;

  if(!m_didTable)
  {
    // Find table info
    m_tables.clear();
    if(!m_info->MakeInfoTableTable(m_tables,errors,m_schema,m_tableName) || m_tables.empty())
    {
      throw StdException(XString("Cannot find table: ") + m_tableName + " : " + errors);
    }
    m_didTable = !m_tables.empty();
  }

  // Some engines get a synonym AND a table/view record
  // The table record always comes last.
  MetaTable& table = m_tables.back();

  // Construct table name
  if(m_schema.IsEmpty() && !table.m_schema.IsEmpty())
  {
    m_schema = table.m_schema;
  }

  // Optional remarks to begin with
  if(!table.m_remarks.IsEmpty())
  {
    ddl = "-- " + table.m_remarks;
  }

  // Do our DDL part
  MetaColumn column;
  ddl += m_info->GetCATALOGTableCreate(table,column);
  ddl += "\n";

  m_createDDL = ddl;
}

void   
DDLCreateTable::GetColumnInfo()
{
  XString errors;
  bool    first = true;

  if(!m_didColumns)
  {
    // Find column info
    m_columns.clear();
    if(!m_info->MakeInfoTableColumns(m_columns,errors,m_schema,m_tableName) || m_columns.empty())
    {
      throw StdException(XString("Cannot find columns for table: ") + m_tableName + " : " + errors);
    }
    m_didColumns = !m_columns.empty();
  }

  // Calculate max length of a column
  int spacing = CalculateColumnLength(m_columns);

  // Add columns
  m_createDDL += "(\n";

  for(auto& column : m_columns)
  {
    XString line("   ");
    line += first ? " " : ",";

    // Column name and spacing
    line += FormatColumnName(column.m_column,spacing);
    line += " ";

    // Getting the datatype
    if(m_target)
    {
      line += m_target->GetKEYWORDDataType(&column);
    }
    else
    {
      line += column.m_typename;
    }

    m_info->GetInfo();
    TypeInfo* type = m_info->GetTypeInfo(column.m_datatype,column.m_typename);
    if(type)
    {
      line += ReplaceLengthPrecScale(type
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
}

void
DDLCreateTable::GetOptionsInfo()
{
  if(m_didOptions == false)
  {
    DatabaseType type = m_info->GetRDBMSDatabaseType();
    if(type == RDBMS_ORACLE     ||
       type == RDBMS_POSTGRESQL ||
       type == RDBMS_MARIADB    ||
       type == RDBMS_MYSQL)
    {
      XString seperator;
      XString tablespace = m_tables[0].m_tablespace;
      if(!tablespace.IsEmpty())
      {
        m_createDDL += XString("\nTABLESPACE ") + tablespace;
        seperator = ",";
      }

      MetaColumn column;
      XString postfix = m_info->GetCATALOGTableCreatePostfix(m_tables[0],column);
      if(!postfix.IsEmpty())
      {
        m_createDDL += seperator + "\n";
        m_createDDL += postfix;
      }
    }
    m_didOptions = true;
  }
  StashTheLine(m_createDDL);
  m_createDDL.Empty();
}

void
DDLCreateTable::GetViewInfo()
{
  // Get primary table info
  XString errors;
  XString ddl;

  if(!m_didTable)
  {
    // Find table info
    m_tables.clear();
    if(!m_info->MakeInfoTableView(m_tables,errors,m_schema,m_tableName) || m_tables.empty())
    {
      throw StdException(XString("Cannot find view: ") + m_tableName + " : " + errors);
    }
    m_didTable = !m_tables.empty();
  }

  // Some engines get a synonym AND a table/view record
  // The table record always comes last.
  MetaTable& table = m_tables.back();

  // Construct table name
  if(m_schema.IsEmpty() && !table.m_schema.IsEmpty())
  {
    m_schema = table.m_schema;
  }

  // Optional remarks to begin with
  if(!table.m_remarks.IsEmpty())
  {
    ddl = "-- " + table.m_remarks;
  }

  XString definition;
  if(m_info->MakeInfoViewDefinition(definition,errors,m_schema,m_tableName))
  {
    // Do our DDL part
    ddl += m_info->GetCATALOGViewCreate(m_schema,table.m_table,definition,m_dropIfExists);
    ddl += "\n";
  }
  StashTheLine(ddl);
}

void
DDLCreateTable::GetIndexInfo()
{
  XString errors;
  XString line;

  if(!m_didIndices)
  {
    // Find column info
    m_indices.clear();
    m_info->MakeInfoTableStatistics(m_indices,errors,m_schema,m_tableName,nullptr);
    if(!errors.IsEmpty())
    {
      throw StdException(XString("Cannot find indices for table: ") + m_tableName + " : " + errors);
    }
    m_didIndices = !m_indices.empty();
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
        // But only if it's not the already generated primary key
        if(m_primaries.empty() || m_primaries[0].m_constraintName.CompareNoCase(theIndex[0].m_indexName))
        {
          line = m_info->GetCATALOGIndexCreate(theIndex,m_indexDuplicateNulls);
          StashTheLine(line);
        }
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
    if(m_primaries.empty() || m_primaries[0].m_constraintName.CompareNoCase(theIndex[0].m_indexName))
    {
      line = m_info->GetCATALOGIndexCreate(theIndex,m_indexDuplicateNulls);
      StashTheLine(line);
    }
  }
}

void
DDLCreateTable::GetPrimaryKeyInfo()
{
  XString errors;

  if(!m_didPrimary)
  {
    // Find column info
    m_primaries.clear();
    m_info->MakeInfoTablePrimary(m_primaries,errors,m_schema,m_tableName);
    if(!errors.IsEmpty())
    {
      throw StdException(XString("Cannot find the primary key for table: ") + m_tableName + " : " + errors);
    }
    m_didPrimary = !m_primaries.empty();
  }

  if(!m_primaries.empty())
  {
    // Getting the alter table statement
    XString line = m_info->GetCATALOGPrimaryCreate(m_primaries);
    StashTheLine(line);
  }
}

void
DDLCreateTable::GetForeignKeyInfo()
{
  XString errors;
  XString line;

  if(!m_didForeigns)
  {
    // Find column info
    m_foreigns.clear();
    m_info->MakeInfoTableForeign(m_foreigns,errors,m_schema,m_tableName);
    if(!errors.IsEmpty())
    {
      throw StdException(XString("Cannot find the foreign keys for table: ") + m_tableName + " : " + errors);
    }
    m_didForeigns = !m_foreigns.empty();
  }

  // Do all foreign keys
  MForeignMap oneFKey;
  XString lastPrimTable;
  XString lastConstraint;
  for(auto& foreign : m_foreigns)
  {
    // Other table OR another foreign key in the same table as the last one!
    if(foreign.m_pkTableName.Compare(lastPrimTable) || foreign.m_foreignConstraint.Compare(lastConstraint))
    {
      // start a new foreign key
      lastPrimTable  = foreign.m_pkTableName;
      lastConstraint = foreign.m_foreignConstraint;

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
  XString errors;

  if(m_didTriggers)
  {
    m_triggers.clear();
    m_info->MakeInfoTableTriggers(m_triggers,errors,m_schema,m_tableName);
    if(!errors.IsEmpty())
    {
      throw StdException(XString("Cannot find the triggers for table: ") + m_tableName + " : " + errors);
    }
    m_didTriggers = !m_triggers.empty();
  }

  // Print all triggers
  for(auto& trigger : m_triggers)
  {
    XString line = m_info->GetCATALOGTriggerCreate(trigger);
    StashTheLine(line);
  }
}

void
DDLCreateTable::GetSequenceInfo()
{
  XString errors;
  XString line;

  if(!m_didSequence)
  {
    m_sequences.clear();
    m_info->MakeInfoTableSequences(m_sequences,errors,m_schema,m_tableName);
    if(!errors.IsEmpty())
    {
      throw StdException(XString("Cannot find the sequences for table: ") + m_tableName + " : " + errors);
    }
    m_didSequence = !m_sequences.empty();
  }

  // Print all found sequences
  for(auto& seq : m_sequences)
  {
    line = m_target ? m_target->GetCATALOGSequenceDrop(seq.m_schemaName,seq.m_sequenceName)
                    :   m_info->GetCATALOGSequenceDrop(seq.m_schemaName,seq.m_sequenceName);
    StashTheLine(line);
    line = m_target ? m_target->GetCATALOGSequenceCreate(seq)
                    :   m_info->GetCATALOGSequenceCreate(seq);
    StashTheLine(line);
  }
}

void
DDLCreateTable::GetAccessInfo(bool p_strict /*=false*/)
{
  XString errors;
  XString line;

  if(!m_didPrivileges)
  {
    // Find column info
    m_access.clear();
    m_info->MakeInfoTablePrivileges(m_access,errors,m_schema,m_tableName);
    if(!errors.IsEmpty())
    {
      throw StdException(XString("Cannot find the privileges for table: ") + m_tableName + " : " + errors);
    }
    m_didPrivileges = !m_access.empty();
  }

  bool strict = p_strict || m_info->GetPreferODBC();

  // Print all privileges
  for(auto& priv : m_access)
  {
    if(!strict || IsStrictODBCPrivilege(priv.m_privilege))
    {
      line = m_target ? m_target->GetCATALOGGrantPrivilege(priv.m_schemaName,priv.m_tableName,priv.m_privilege,priv.m_grantee,priv.m_grantable)
                      :   m_info->GetCATALOGGrantPrivilege(priv.m_schemaName,priv.m_tableName,priv.m_privilege,priv.m_grantee,priv.m_grantable);
      if(!line.IsEmpty())
      {
        StashTheLine(line);
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
// SERVICE ROUTINES
//
//////////////////////////////////////////////////////////////////////////

bool
DDLCreateTable::FindSchemaName(XString p_tableName)
{
  int pos = p_tableName.Find('.');
  if (pos > 0)
  {
    m_schema    = p_tableName.Left(pos);
    m_tableName = p_tableName.Mid(pos + 1);
    return true;
  }
  m_tableName = p_tableName;
  return false;
}

void
DDLCreateTable::StashTheLine(XString p_line)
{
  // See if we have work to do
  if(p_line.IsEmpty())
  {
    return;
  }

  m_statements.push_back(p_line);
}

XString
DDLCreateTable::ReplaceLengthPrecScale(TypeInfo*  p_type
                                      ,int p_length
                                      ,int p_precision
                                      ,int p_scale)
{
  bool max = false;
  XString params = p_type->m_create_params;

  // Set in lowercase for replacing
  params.MakeLower();

  // Confusion in some drivers
  if(p_length == 0 && p_precision > 0 && params.Find("length") >= 0)
  {
    p_length = p_precision;
    p_precision = 0;
  }

  // Format as strings
  XString length,precision,scale;
  if(p_length > 0)
  {
   length.Format("%d",p_length);
  }
  if(p_precision > 0 || p_scale > 0)
  {
    precision.Format("%d",p_precision);
    scale.Format("%d",p_scale);
  }

  // Replace as strings
  if(p_length > 0)
  {
    params.Replace("max length",length);    // ORACLE DOES THIS!!
    params.Replace("length",    length);
  }
  else if(p_type->m_type_name.CompareNoCase("varchar") == 0)
  {
    // SQL-Server does this as a replacement for CLOB
    params.Replace("max length","max");
    max = true;
  }

  if(p_precision > 0)
  {
    params.Replace("precision", precision);
    params.Replace("scale",     scale);
  }
  else if(!max)
  {
    params.Empty();
  }
  // Make sure we have parenthesis
  if(!params.IsEmpty() && params.Left(1) != "(" && params != ",")
  {
    params = "(" + params + ")";
  }
  if(params != ",")
  {
    return params;
  }
  return "";
}

XString
DDLCreateTable::FormatColumnName(XString p_column,int p_length)
{
  // Adhere to catalog storage
  if(m_info->GetRDBMSIsCatalogUpper())
  {
    p_column.MakeUpper();
  }
  else
  {
    p_column.MakeLower();
  }

  // Circumvent locally reserved words
  if(m_target && m_target->GetRDBMSDatabaseType() == DatabaseType::RDBMS_SQLSERVER)
  {
    p_column = "[" + p_column + "]";
    p_length += 2;
  }
  else if(!m_info->IsCorrectName(p_column))
  {
    XString quote = m_info->GetKEYWORDReservedWordQuote();
    p_column = quote + p_column + quote;
  }

  // Pretty-print adjust datatype
  while (p_column.GetLength() < p_length)
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
  // Index already found
  if(!p_index.m_filter.IsEmpty())
  {
    return;
  }

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

bool
DDLCreateTable::IsStrictODBCPrivilege(XString p_privilege)
{
  if(p_privilege.CompareNoCase("SELECT")     == 0) return true;
  if(p_privilege.CompareNoCase("INSERT")     == 0) return true;
  if(p_privilege.CompareNoCase("UPDATE")     == 0) return true;
  if(p_privilege.CompareNoCase("DELETE")     == 0) return true;
  if(p_privilege.CompareNoCase("CREATE")     == 0) return true;
  if(p_privilege.CompareNoCase("REFERENCES") == 0) return true;
  if(p_privilege.CompareNoCase("INDEX")      == 0) return true;

  return false;
}

};
