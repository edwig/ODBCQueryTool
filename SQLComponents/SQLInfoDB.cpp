////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoDB.cpp
//
// Copyright (c) 1998-2025 ir. W.E. Huisman
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
#include "stdafx.h"
#include "SQLComponents.h"
#include "SQLInfoDB.h"
#include "SQLQuery.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{

SQLInfoDB::SQLInfoDB(SQLDatabase* p_database)
          :SQLInfo(p_database)
          // Must be 'PUBLIC' for an ANSI-compliant database
          ,m_grantedUsers(_T("PUBLIC"))
{
  // Granted users.
  // Comma separated list of granted users
  // e.g. "meta3", "meta2", "meta1", "model" and "data"
}

SQLInfoDB::~SQLInfoDB()
{
}

// Searching for a 'table' like object
// Can be 'TABLE', 'VIEW', 'ALIAS', 'SYNONYM', 'SYSTEM TABLE' etc
bool    
SQLInfoDB::MakeInfoTableObject(MTableMap& p_tables
                              ,XString&  p_errors
                              ,XString   p_schema
                              ,XString   p_tablename)
{
  // Clear the results
  p_tables.clear();
  p_errors.Empty();

  XString sql1 = GetCATALOGTableAttributes(p_schema,p_tablename,true);
  XString sql2 = GetCATALOGViewAttributes (p_schema,p_tablename,true);
  XString sql3 = GetCATALOGTableSynonyms  (p_schema,p_tablename,true);
  XString sql4 = GetCATALOGTableCatalog   (p_schema,p_tablename,true);

  if(sql1.IsEmpty() || sql2.IsEmpty() || sql3.IsEmpty() || sql4.IsEmpty() || m_preferODBC)
  {
    // Ask ODBC to find all object types (empty search argument)
    return SQLInfo::MakeInfoTableTable(p_tables,p_errors,p_schema,p_tablename,"");
  }

  try
  {
    for(int ind = 0;ind < 2;++ind)
    {
      SQLQuery qry(m_database);

      if(!p_schema.IsEmpty()) qry.SetParameter(p_schema);
      if(!p_tablename.IsEmpty()) qry.SetParameter(p_tablename);

      qry.DoSQLStatement(sql1);
      ReadTablesFromQuery(qry,p_tables);
      qry.DoSQLStatement(sql2);
      ReadTablesFromQuery(qry,p_tables);
      qry.DoSQLStatement(sql3);
      ReadTablesFromQuery(qry,p_tables);
      qry.DoSQLStatement(sql4);
      ReadTablesFromQuery(qry,p_tables);
      if(!p_tables.empty())
      {
        return true;
      }
      // Try standard unquoted identifiers
      sql1 = GetCATALOGTableAttributes(p_schema,p_tablename);
      sql2 = GetCATALOGViewAttributes (p_schema,p_tablename);
      sql3 = GetCATALOGTableSynonyms  (p_schema,p_tablename);
      sql4 = GetCATALOGTableCatalog   (p_schema,p_tablename);
    }
  }
  catch(StdException& er)
  {
    ReThrowSafeException(er);
    p_errors += er.GetErrorMessage();
  }
  return false;
}

// Meta info about meta types: META_CATALOGS/META_SCHEMAS/META_TABLES
bool
SQLInfoDB::MakeInfoMetaTypes(MMetaMap& p_objects,XString& p_errors,int p_type)
{
  XString sql = GetCATALOGMetaTypes(p_type);
  if(sql.IsEmpty() || m_preferODBC)
  {
    // Ask ODBC driver to find the required meta types
    return SQLInfo::MakeInfoMetaTypes(p_objects,p_errors,p_type);
  }

  try
  {
    SQLQuery qry(m_database);
    qry.DoSQLStatement(sql);
    return ReadMetaTypesFromQuery(qry,p_objects,p_type);
  }
  catch(XString& er)
  {
    p_errors.Append(er);
  }
  return 0;
}

bool
SQLInfoDB::MakeInfoDefaultCharset(XString& p_default)
{
  p_default.Empty();

  if(!m_defaultCharset.IsEmpty())
  {
    p_default = m_defaultCharset;
    return true;
  }
  XString sql = GetCATALOGDefaultCharset();
  if(sql.IsEmpty() || sql.Find(' ') < 0)
  {
    m_defaultCharset = sql;
    p_default = m_defaultCharset;
    return true;
  }

  try
  {
    SQLQuery qry(m_database);
    qry.DoSQLStatement(sql);
    if(qry.GetRecord())
    {
      m_defaultCharset = qry[1].GetAsString();
      m_defaultCharset = m_defaultCharset.Trim();
      p_default = m_defaultCharset;
      return true;
    }
  }
  catch(...)
  {
    m_defaultCharset = _T("-");
  }
  return false;
}

bool
SQLInfoDB::MakeInfoDefaultCharsetNC(XString& p_default)
{
  p_default.Empty();

  if(!m_defaultCharsetNCV.IsEmpty())
  {
    p_default = m_defaultCharsetNCV;
    return true;
  }
  XString sql = GetCATALOGDefaultCharsetNCV();
  if(sql.IsEmpty() || sql.Find(' ') < 0)
  {
    m_defaultCharsetNCV = sql;
    p_default = m_defaultCharsetNCV;
    return true;
  }

  try
  {
    SQLQuery qry(m_database);
    qry.DoSQLStatement(sql);
    if(qry.GetRecord())
    {
      m_defaultCharsetNCV = qry[1].GetAsString();
      p_default = m_defaultCharsetNCV;
      return true;
    }
  }
  catch(...)
  {
    m_defaultCharsetNCV.Empty();
  }
  return false;
}

bool
SQLInfoDB::MakeInfoDefaultCollation(XString& p_default)
{
  p_default.Empty();
  if(!m_defaultCollation.IsEmpty())
  {
    p_default = m_defaultCollation;
    return true;
  }
  XString sql = GetCATALOGDefaultCollation();
  if(sql.IsEmpty() || sql.Find(' ') < 0)
  {
    m_defaultCollation = sql;
    p_default = m_defaultCollation;
    return true;
  }

  try
  {
    SQLQuery qry(m_database);
    qry.DoSQLStatement(sql);
    if(qry.GetRecord())
    {
      m_defaultCollation = qry[1].GetAsString();
      p_default = m_defaultCollation;
      return true;
    }
  }
  catch(...)
  {
    m_defaultCollation.Empty();
  }
  return false;
}

bool
SQLInfoDB::MakeInfoTableTable(MTableMap& p_tables
                             ,XString&   p_errors
                             ,XString    p_schema
                             ,XString    p_tablename)
{
  XString sql = GetCATALOGTableAttributes(p_schema,p_tablename,true);
  if(sql.IsEmpty() || m_preferODBC)
  {
    // Ask ODBC driver to find tables
    return SQLInfo::MakeInfoTableTable(p_tables,p_errors,p_schema,p_tablename,"TABLE");
  }

  try
  {
    for(int ind = 0;ind < 2;++ind)
    {
      SQLQuery qry(m_database);

      if(!p_schema.IsEmpty()) qry.SetParameter(p_schema);
      if(!p_tablename.IsEmpty()) qry.SetParameter(p_tablename);

      qry.DoSQLStatement(sql);
      if(ReadTablesFromQuery(qry,p_tables))
      {
        return true;
      }
      // Try standard identifiers
      sql = GetCATALOGTableAttributes(p_schema,p_tablename);
    }
  }
  catch(StdException& er)
  {
    ReThrowSafeException(er);
    p_errors += er.GetErrorMessage();
  }
  return false;
}

bool    
SQLInfoDB::MakeInfoTableView(MTableMap& p_tables
                            ,XString&   p_errors
                            ,XString    p_schema
                            ,XString    p_tablename)
{
  // Try quoted identifiers first
  XString sql = GetCATALOGViewAttributes(p_schema,p_tablename,true);
  if(sql.IsEmpty() || m_preferODBC)
  {
    // Ask ODBC driver to find views
    return SQLInfo::MakeInfoTableTable(p_tables,p_errors,p_schema,p_tablename,"VIEW");
  }

  try
  {
    for(int ind = 0;ind < 2;++ind)
    {
      SQLQuery qry(m_database);

      if(!p_schema.IsEmpty()) qry.SetParameter(p_schema);
      if(!p_tablename.IsEmpty()) qry.SetParameter(p_tablename);

      qry.DoSQLStatement(sql);
      if(ReadTablesFromQuery(qry,p_tables))
      {
        return true;
      }
      // Try standard identifiers
      sql = GetCATALOGViewAttributes(p_schema,p_tablename);
    }
  }
  catch(StdException& er)
  {
    ReThrowSafeException(er);
    p_errors += er.GetErrorMessage();
  }
  return false;
}

bool    
SQLInfoDB::MakeInfoTableSynonyms(MTableMap& p_tables
                                ,XString&   p_errors
                                ,XString    p_schema
                                ,XString    p_tablename)
{
  XString sql = GetCATALOGTableSynonyms(p_schema,p_tablename,true);
  if(sql.IsEmpty() || m_preferODBC)
  {
    // Ask ODBC driver to find synonyms
    bool result = SQLInfo::MakeInfoTableTable(p_tables,p_errors,p_schema,p_tablename,"SYNONYM");
    if(!result)
    {
      // Second guess: some RDBMS'es have aliases instead of synonyms
      result = SQLInfo::MakeInfoTableTable(p_tables,p_errors,p_schema,p_tablename,"ALIAS");
    }
    return result;
  }

  try
  {
    for(int ind = 0;ind < 2;++ind)
    {
      SQLQuery qry(m_database);

      if(!p_schema.IsEmpty()) qry.SetParameter(p_schema);
      if(!p_tablename.IsEmpty()) qry.SetParameter(p_tablename);

      qry.DoSQLStatement(sql);
      if(ReadTablesFromQuery(qry,p_tables))
      {
        return true;
      }
      // Try standard identifiers
      sql = GetCATALOGTableSynonyms(p_schema,p_tablename);
    }
  }
  catch(StdException& er)
  {
    ReThrowSafeException(er);
    p_errors += er.GetErrorMessage();
  }
  return false;
}

bool    
SQLInfoDB::MakeInfoTableCatalog(MTableMap&  p_tables
                               ,XString&    p_errors
                               ,XString     p_schema
                               ,XString     p_tablename)
{
  // Try quoted identifiers
  XString sql = GetCATALOGTableCatalog(p_schema,p_tablename,true);
  if(sql.IsEmpty() || m_preferODBC)
  {
    p_schema    = _T("%");
    p_tablename = _T("%");
    // Ask ODBC driver to find system tables
    return SQLInfo::MakeInfoTableTable(p_tables,p_errors,p_schema,p_tablename,_T("SYSTEM TABLE"));
  }

  try
  {
    for(int ind = 0;ind < 2;++ind)
    {
      SQLQuery qry(m_database);

      if(!p_schema.IsEmpty()) qry.SetParameter(p_schema);
      if(!p_tablename.IsEmpty()) qry.SetParameter(p_tablename);

      qry.DoSQLStatement(sql);
      if(ReadTablesFromQuery(qry,p_tables))
      {
        return true;
      }
      // Try standard identifiers
      sql = GetCATALOGTableCatalog(p_schema,p_tablename);
    }
  }
  catch(StdException& er)
  {
    ReThrowSafeException(er);
    p_errors += er.GetErrorMessage();
  }
  return false;
}

bool    
SQLInfoDB::MakeInfoTableColumns(MColumnMap& p_columns
                               ,XString&    p_errors
                               ,XString     p_schema
                               ,XString     p_tablename
                               ,XString     p_columnname /*=""*/)
{
  // First try in quoted identifier format
  XString sql = GetCATALOGColumnAttributes(p_schema,p_tablename,p_columnname,true);
  if(sql.IsEmpty() || m_preferODBC)
  {
    return SQLInfo::MakeInfoTableColumns(p_columns,p_errors,p_schema,p_tablename,p_columnname);
  }
  try
  {
    // Try our own discovery queries
    for(int ind = 0;ind < 2;++ind)
    {
      SQLQuery qry(m_database);

      if(!p_schema    .IsEmpty()) qry.SetParameter(p_schema);
      if(!p_tablename .IsEmpty()) qry.SetParameter(p_tablename);
      if(!p_columnname.IsEmpty()) qry.SetParameter(p_columnname);

      qry.DoSQLStatement(sql);
      while(qry.GetRecord())
      {
        MetaColumn column;

        column.m_catalog        = (XString)  qry[1];
        column.m_schema         = (XString)  qry[2];
        column.m_table          = (XString)  qry[3];
        column.m_column         = (XString)  qry[4];
        column.m_datatype       = (int)      qry[5];
        column.m_typename       = (XString)  qry[6];
        column.m_columnSize     = (unsigned) qry[7];
        column.m_bufferLength   = (INT64)    qry[8];
        column.m_decimalDigits  = (int)      qry[9];
        column.m_numRadix       = (int)      qry[10];
        column.m_nullable       = (int)      qry[11];
        column.m_remarks        = (XString)  qry[12];
        column.m_default        = (XString)  qry[13];
        column.m_datatype3      = (int)      qry[14];
        column.m_sub_datatype   = (int)      qry[15];
        column.m_octet_length   = (INT64)    qry[16];
        column.m_position       = (int)      qry[17];
        column.m_isNullable     = (XString)  qry[18];

        column.m_catalog    = column.m_catalog.Trim();
        column.m_table      = column.m_table.Trim();
        column.m_column     = column.m_column.Trim();
        column.m_typename   = column.m_typename.Trim();
        column.m_remarks    = column.m_remarks.Trim();
        column.m_default    = column.m_default.Trim();
        column.m_isNullable = column.m_isNullable.Trim();

        p_columns.push_back(column);
      }
      if(!p_columns.empty())
      {
        return true;
      }
    }
    // Second try, now in standard storage format
    sql = GetCATALOGColumnAttributes(p_schema,p_tablename,p_columnname);
  }
  catch(StdException& er)
  {
    ReThrowSafeException(er);
    p_errors += er.GetErrorMessage();
  }
  return false;
}

bool    
SQLInfoDB::MakeInfoTablePrimary(MPrimaryMap&  p_primaries
                               ,XString&      p_errors
                               ,XString       p_schema
                               ,XString       p_tablename)
{
  // First try with quoted identifiers
  XString sql = GetCATALOGPrimaryAttributes(p_schema,p_tablename,true);
  if(sql.IsEmpty() || m_preferODBC)
  {
    return SQLInfo::MakeInfoTablePrimary(p_primaries,p_errors,p_schema,p_tablename);
  }

  try
  {
    for(int ind = 0;ind < 2;++ind)
    {
      SQLQuery qry(m_database);

      if(!p_schema   .IsEmpty()) qry.SetParameter(p_schema);
      if(!p_tablename.IsEmpty()) qry.SetParameter(p_tablename);

      qry.DoSQLStatement(sql);
      while(qry.GetRecord())
      {
        MetaPrimary prim;

        prim.m_catalog        = (XString) qry[MetaPrimary_catalogname];
        prim.m_schema         = (XString) qry[MetaPrimary_schemaname];
        prim.m_table          = (XString) qry[MetaPrimary_tablename];
        prim.m_columnName     = (XString) qry[MetaPrimary_columname];
        prim.m_columnPosition = (long)    qry[MetaPrimary_keysequence];
        prim.m_constraintName = (XString) qry[MetaPrimary_constraint];

        p_primaries.push_back(prim);
      }
      if(!p_primaries.empty())
      {
        return true;
      }
      // Second try in standard catalog format
      sql = GetCATALOGPrimaryAttributes(p_schema,p_tablename);
    }
  }
  catch(StdException& er)
  {
    ReThrowSafeException(er);
    p_errors += er.GetErrorMessage();
  }
  return false;
}

bool    
SQLInfoDB::MakeInfoTableForeign(MForeignMap&  p_foreigns
                               ,XString&      p_errors
                               ,XString       p_schema
                               ,XString       p_tablename
                               ,bool          p_referenced /* = false */) 
{
  XString constraint;
  // Try quoted identifiers
  XString sql = GetCATALOGForeignAttributes(p_schema,p_tablename,constraint,p_referenced,true);
  if(sql.IsEmpty() || m_preferODBC)
  {
    return SQLInfo::MakeInfoTableForeign(p_foreigns,p_errors,p_schema,p_tablename,p_referenced);
  }
  try
  {
    for(int ind = 0;ind < 2;++ind)
    {
      SQLQuery query(m_database);

      if(!p_schema   .IsEmpty()) query.SetParameter(p_schema);
      if(!p_tablename.IsEmpty()) query.SetParameter(p_tablename);
      if(!constraint .IsEmpty()) query.SetParameter(constraint);

      query.DoSQLStatement(sql);
      while(query.GetRecord())
      {
        MetaForeign foreign;

        foreign.m_pkCatalogName     = (XString) query[MetaForeign_pk_catalogname];
        foreign.m_pkSchemaName      = (XString) query[MetaForeign_pk_schemaname];
        foreign.m_pkTableName       = (XString) query[MetaForeign_pk_tablename];
        foreign.m_pkColumnName      = (XString) query[MetaForeign_pk_columnname];
        foreign.m_fkCatalogName     = (XString) query[MetaForeign_fk_catalogname];
        foreign.m_fkSchemaName      = (XString) query[MetaForeign_fk_schemaname];
        foreign.m_fkTableName       = (XString) query[MetaForeign_fk_tablename];
        foreign.m_fkColumnName      = (XString) query[MetaForeign_fk_columname];
        foreign.m_keySequence       = (int)     query[MetaForeign_keysequence];
        foreign.m_updateRule        = (int)     query[MetaForeign_updaterule];
        foreign.m_deleteRule        = (int)     query[MetaForeign_deleterule];
        foreign.m_foreignConstraint = (XString) query[MetaForeign_fk_constraint];
        foreign.m_primaryConstraint = (XString) query[MetaForeign_pk_constraint];
        foreign.m_deferrable        = (int)     query[MetaForeign_deferrable];
        // Extra info
        foreign.m_match             = (int)     query[15];
        foreign.m_initiallyDeferred = (int)     query[16];
        foreign.m_enabled           = (int)     query[17];

        p_foreigns.push_back(foreign);
      }
      if(!p_foreigns.empty())
      {
        return true;
      }
      // Try standard identifiers
      sql = GetCATALOGForeignAttributes(p_schema,p_tablename,constraint,p_referenced);
    }
  }
  catch(StdException& er)
  {
    ReThrowSafeException(er);
    p_errors += er.GetErrorMessage();
  }
  return false;
}

bool
SQLInfoDB::MakeInfoTableStatistics(MIndicesMap& p_indices
                                  ,XString&     p_errors
                                  ,XString      p_schema
                                  ,XString      p_tablename
                                  ,MPrimaryMap* p_keymap
                                  ,bool         p_all         /*=true*/)
{
  // Try quoted identifiers first
  XString column;
  XString sql1 = GetCATALOGIndexAttributes(p_schema,p_tablename,column,true);   // Indices
  column = "0";
  XString sql2 = GetCATALOGIndexAttributes(p_schema,p_tablename,column,true);   // Table statistics
  if(sql1.IsEmpty() || m_preferODBC)
  {
    return SQLInfo::MakeInfoTableStatistics(p_indices,p_errors,p_schema,p_tablename,p_keymap,p_all);
  }
  try
  {
    for(int ind = 0;ind < 2;++ind)
    {
      SQLQuery query(m_database);

      if (!p_schema   .IsEmpty()) query.SetParameter(p_schema);
      if (!p_tablename.IsEmpty()) query.SetParameter(p_tablename);
      if (!column     .IsEmpty()) query.SetParameter(column);

      while(sql1.GetLength())
      {
        query.DoSQLStatement(sql1);
        while(query.GetRecord())
        {
          MetaIndex index;

          index.m_catalogName   = (XString) query[MetaIndex_catalogname];
          index.m_schemaName    = (XString) query[MetaIndex_schemaname];
          index.m_tableName     = (XString) query[MetaIndex_tablename];
          index.m_nonunique     = (bool)    query[MetaIndex_nonunique];
          index.m_dropQualifier = (XString) query[MetaIndex_dropqualifier];
          index.m_indexName     = (XString) query[MetaIndex_indexname];
          index.m_indexType     = (int)     query[MetaIndex_type];
          index.m_position      = (int)     query[MetaIndex_position];
          index.m_columnName    = (XString) query[MetaIndex_columname];
          index.m_ascending     = (XString) query[MetaIndex_ascending];
          index.m_cardinality   = (int)     query[MetaIndex_cardinality];
          index.m_pages         = (int)     query[MetaIndex_pages];
          index.m_filter        = (XString) query[MetaIndex_filter];

          p_indices.push_back(index);
        }
        // Possibly an extra loop to get the table statistics
        if(sql2.GetLength())
        {
          sql1 = sql2;
          sql2.Empty();
        }
        else sql1.Empty();
      }
      if(!p_indices.empty())
      {
        return true;
      }
      // Standard identifiers
      sql1 = GetCATALOGIndexAttributes(p_schema,p_tablename,column);   // Indices
      column = "0";
      sql2 = GetCATALOGIndexAttributes(p_schema,p_tablename,column);   // Table statistics
    }
  }
  catch(StdException& er)
  {
    ReThrowSafeException(er);
    p_errors += er.GetErrorMessage();
  }
  return false;
}

bool    
SQLInfoDB::MakeInfoPSMProcedures(MProcedureMap&  p_procedures
                                ,XString&        p_errors
                                ,XString         p_schema
                                ,XString         p_procedure)
{
  XString sql;

  if(p_procedure.IsEmpty() || p_procedure.Compare(_T("%")) == 0)
  {
    // We want a list only
    sql = GetPSMProcedureList(p_schema,p_procedure,true);
  }
  else
  {
    // Try quoted identifier case
    sql = GetPSMProcedureAttributes(p_schema,p_procedure,true);
  }

  // Let ODBC handle the call
  if(sql.IsEmpty() || m_preferODBC)
  {
    return SQLInfo::MakeInfoPSMProcedures(p_procedures,p_errors,p_schema,p_procedure);
  }

  try
  {
    for(int ind = 0;ind < 2;++ind)
    {
      SQLQuery qry(m_database);

      if(!p_schema   .IsEmpty()) qry.SetParameter(p_schema);
      if(!p_procedure.IsEmpty()) qry.SetParameter(p_procedure);

      qry.DoSQLStatement(sql);
    
      if(qry.GetNumberOfColumns() == 4)
      {
        // Name only
        while(qry.GetRecord())
        {
          MetaProcedure proc;

          proc.m_catalogName   = (XString) qry[MetaProcedure_catalogname];
          proc.m_schemaName    = (XString) qry[MetaProcedure_schemaname];
          proc.m_procedureName = (XString) qry[MetaProcedure_procedurename];
          proc.m_procedureType = (long)qry[4];

          p_procedures.push_back(proc);
        }
      }
      else
      {
        // List complete procedure
        while(qry.GetRecord())
        {
          MetaProcedure proc;

          proc.m_catalogName      = (XString) qry[MetaProcedure_catalogname];
          proc.m_schemaName       = (XString) qry[MetaProcedure_schemaname];
          proc.m_procedureName    = (XString) qry[MetaProcedure_procedurename];
          proc.m_inputParameters  = (long)    qry[MetaProcedure_inputargumetns];
          proc.m_outputParameters = (long)    qry[MetaProcedure_outputargumetns];
          proc.m_resultSets       = (long)    qry[MetaProcedure_resultsets];
          proc.m_remarks          = (XString) qry[MetaProcedure_remarks];
          proc.m_procedureType    = (long)    qry[MetaProcedure_type];
          proc.m_source           = (XString) qry[MetaProcedure_source];

          if(proc.m_source.IsEmpty() || proc.m_source.Compare(_T("<@>")) == 0)
          {
            proc.m_source = MakeInfoPSMSourcecode(proc.m_schemaName, proc.m_procedureName);
          }
          p_procedures.push_back(proc);
        }
      }
      if(!p_procedures.empty())
      {
        return true;
      }
      // Try standard identifier case
      if(p_procedure.IsEmpty() || p_procedure.Compare(_T("%")) == 0)
      {
        sql = GetPSMProcedureList(p_schema,p_procedure);
      }
      else
      {
        sql = GetPSMProcedureAttributes(p_schema,p_procedure);
      }
    }
  }
  catch(StdException& er)
  {
    ReThrowSafeException(er);
    p_errors += er.GetErrorMessage();
  }
  return false;
}

XString
SQLInfoDB::MakeInfoPSMSourcecode(XString p_schema, XString p_procedure)
{
  XString sourcecode;
  XString sql = GetPSMProcedureSourcecode(p_schema,p_procedure,true);
  if(!sql.IsEmpty())
  {
    for(int ind = 0;ind < 2;++ind)
    {
      SQLQuery query(m_database);
      query.DoSQLStatement(sql);
      while(query.GetRecord())
      {
        sourcecode += query.GetColumn(3)->GetAsString();
      }
      if(!sourcecode.IsEmpty())
      {
        return sourcecode;
      }
      // Try standard unquoted identifiers
      sql = GetPSMProcedureSourcecode(p_schema,p_procedure);
    }
  }
  return sourcecode;
}

bool    
SQLInfoDB::MakeInfoPSMParameters(MParameterMap& p_parameters
                                ,XString&       p_errors
                                ,XString        p_schema
                                ,XString        p_procedure)
{
  // Try quoted identifiers
  XString sql = GetPSMProcedureParameters(p_schema,p_procedure,true);
  if(sql.IsEmpty() || m_preferODBC)
  {
    // No SQL, let ODBC handle the parameters
    return SQLInfo::MakeInfoPSMParameters(p_parameters,p_errors,p_schema,p_procedure);
  }
  try
  {
    for(int ind = 0;ind < 2;++ind)
    {
      SQLQuery qry(m_database);

      if(!p_schema   .IsEmpty()) qry.SetParameter(p_schema);
      if(!p_procedure.IsEmpty()) qry.SetParameter(p_procedure);

      qry.DoSQLStatement(sql);
      while(qry.GetRecord())
      {
        MetaParameter param;

        param.m_catalog       = qry.GetColumn(1)->GetAsString();
        param.m_schema        = qry.GetColumn(2)->GetAsString();
        param.m_procedure     = qry.GetColumn(3)->GetAsString();
        param.m_parameter     = qry.GetColumn(4)->GetAsString();
        param.m_columnType    = qry.GetColumn(5)->GetAsSLong();
        param.m_datatype      = qry.GetColumn(6)->GetAsSLong();
        param.m_typeName      = qry.GetColumn(7)->GetAsString();
        param.m_columnSize    = qry.GetColumn(8)->GetAsSLong();
        param.m_bufferLength  = qry.GetColumn(9)->GetAsSLong();
        param.m_decimalDigits = qry.GetColumn(10)->GetAsSLong();
        param.m_numRadix      = qry.GetColumn(11)->GetAsSLong();
        param.m_nullable      = qry.GetColumn(12)->GetAsSLong();
        param.m_remarks       = qry.GetColumn(13)->GetAsString();
        param.m_default       = qry.GetColumn(14)->GetAsString();
        param.m_datatype3     = qry.GetColumn(15)->GetAsSLong();
        param.m_subType       = qry.GetColumn(16)->GetAsSLong();
        param.m_octetLength   = qry.GetColumn(17)->GetAsSLong();
        param.m_position      = qry.GetColumn(18)->GetAsSLong();
        param.m_isNullable    = qry.GetColumn(19)->GetAsString();
        // Trimming
        param.m_typeName = param.m_typeName.Trim();

        p_parameters.push_back(param);
      }
      if(!p_parameters.empty())
      {
        return true;
      }
      // Try standard identifiers
      sql = GetPSMProcedureParameters(p_schema,p_procedure);
    }
  }
  catch(StdException& er)
  {
    ReThrowSafeException(er);
    p_errors += er.GetErrorMessage();
  }
  return false;
}

// Getting the info for the triggers is not a part of the standard
// ODBC function set. It needs a RDBMS driver, so it is not in DBInfo but in DBInfoDB
bool
SQLInfoDB::MakeInfoTableTriggers(MTriggerMap& p_triggers
                                ,XString& p_errors
                                ,XString p_schema
                                ,XString p_tablename /*= ""*/
                                ,XString p_trigger   /*= ""*/)
{
  // Getting the database dependent SQL string
  XString sql;
  if(!p_tablename.IsEmpty() && p_trigger.IsEmpty())
  {
    sql = GetCATALOGTriggerList(p_schema,p_tablename,true);
  }
  else
  {
    sql = GetCATALOGTriggerAttributes(p_schema,p_tablename,p_trigger,true);
  }
  if(sql.IsEmpty())
  {
    // No triggers to be gotten from this RDBMS
    return false;
  }

  try
  {
    for(int ind = 0;ind < 2;++ind)
    {
      SQLQuery qry(m_database);

      if(!p_schema   .IsEmpty()) qry.SetParameter(p_schema);
      if(!p_tablename.IsEmpty()) qry.SetParameter(p_tablename);
      if(!p_trigger  .IsEmpty()) qry.SetParameter(p_trigger);

      qry.DoSQLStatement(sql);
      while(qry.GetRecord())
      {
        MetaTrigger trigger;
        trigger.m_catalogName = (XString) qry[MetaTrigger_catalogname];
        trigger.m_schemaName  = (XString) qry[MetaTrigger_schemaname];
        trigger.m_tableName   = (XString) qry[MetaTrigger_tablename];
        trigger.m_triggerName = (XString) qry[MetaTrigger_triggername];
        trigger.m_remarks     = (XString) qry[MetaTrigger_remarks];
        trigger.m_position    = (long)    qry[MetaTrigger_position];
        trigger.m_before      = (bool)    qry[MetaTrigger_before];
        trigger.m_insert      = (bool)    qry[MetaTrigger_insert];
        trigger.m_update      = (bool)    qry[MetaTrigger_update];
        trigger.m_delete      = (bool)    qry[MetaTrigger_delete];
        trigger.m_select      = (bool)    qry[MetaTrigger_select];
        trigger.m_session     = (bool)    qry[MetaTrigger_session];
        trigger.m_transaction = (bool)    qry[MetaTrigger_transaction];
        trigger.m_rollback    = (bool)    qry[MetaTrigger_rollback];
        trigger.m_referencing = (XString) qry[MetaTrigger_referencing];
        trigger.m_enabled     = (bool)    qry[MetaTrigger_enabled];
        trigger.m_source      = (XString) qry[MetaTrigger_source];

        // Some RDBMS'es have extra trailing spaces in these fields
        trigger.m_triggerName.Trim();
        trigger.m_remarks.Trim();
        trigger.m_source.Trim();
        trigger.m_source.Replace(_T("\r\n"),_T("\n"));

        if(trigger.m_source.Compare(_T("<@>")) == 0)
        {
          trigger.m_source = MakeInfoPSMSourcecode(trigger.m_schemaName,trigger.m_triggerName);
        }
        p_triggers.push_back(trigger);
      }
      if(!p_triggers.empty())
      {
        return true;
      }
      // Try default catalog identifiers
      sql = GetCATALOGTriggerAttributes(p_schema,p_tablename,p_trigger);
    }
  }
  catch(StdException& er)
  {
    ReThrowSafeException(er);
    p_errors += er.GetErrorMessage();
  }
  return false;
}

bool 
SQLInfoDB::MakeInfoTableSequences(MSequenceMap& p_sequences,XString& p_errors,XString p_schema,XString p_tablename)
{
  // Getting the database dependent SQL string (quoted identifiers first)
  XString sql = GetCATALOGSequenceList(p_schema,p_tablename,true);
  if(sql.IsEmpty())
  {
    // No sequences to be gotten from this RDBMS
    return false;
  }

  try
  {
    for(int ind = 0;ind < 2;++ind)
    {
      SQLQuery qry(m_database);

      if(!p_schema.IsEmpty())    qry.SetParameter(p_schema);
      if(!p_tablename.IsEmpty()) qry.SetParameter(p_tablename);

      qry.DoSQLStatement(sql);
      while(qry.GetRecord())
      {
        MetaSequence sequence;

        sequence.m_catalogName  = qry.GetColumn(1)->GetAsString();
        sequence.m_schemaName   = qry.GetColumn(2)->GetAsString();
        sequence.m_sequenceName = qry.GetColumn(3)->GetAsString();
        sequence.m_currentValue = qry.GetColumn(4)->GetAsDouble();
        sequence.m_minimalValue = qry.GetColumn(5)->GetAsDouble();
        sequence.m_increment    = qry.GetColumn(6)->GetAsSLong();
        sequence.m_cache        = qry.GetColumn(7)->GetAsSLong();
        sequence.m_cycle        = qry.GetColumn(8)->GetAsBoolean();
        sequence.m_order        = qry.GetColumn(9)->GetAsBoolean();

        if(sequence.m_increment == 0)
        {
          // A sequence without an increment is NO sequence.
          // Try to get hold of the values through the attributes call
          XString sql2 = GetCATALOGSequenceAttributes(p_schema,sequence.m_sequenceName,ind == 0 ? true : false);
          if(!sql2.IsEmpty())
          {
            SQLQuery query2(m_database);
            if(!p_schema.IsEmpty())    query2.SetParameter(p_schema);
            query2.SetParameter(sequence.m_sequenceName);
            query2.DoSQLStatement(sql2);
            if(query2.GetRecord())
            {
              sequence.m_currentValue = query2.GetColumn(4)->GetAsDouble();
              sequence.m_minimalValue = query2.GetColumn(5)->GetAsDouble();
              sequence.m_increment    = query2.GetColumn(6)->GetAsSLong();
              sequence.m_cache        = query2.GetColumn(7)->GetAsSLong();
              sequence.m_cycle        = query2.GetColumn(8)->GetAsBoolean();
              sequence.m_order        = query2.GetColumn(9)->GetAsBoolean();
            }
          }
        }
        p_sequences.push_back(sequence);
      }
      if(!p_sequences.empty())
      {
        return true;
      }
      // Now try standard identifiers
      sql = GetCATALOGSequenceList(p_schema,p_tablename);
    }
  }
  catch(StdException& er)
  {
    ReThrowSafeException(er);
    XString message = er.GetErrorMessage();
    if(message.Find(_T("[42S02]")) > 0)
    {
      // Older versions of MS-SQLServer return this SQLSTATE
      p_errors.Append(_T("Version of RDBMS that does not support SEQUENCE feature (yet)!"));
  }
    else
    {
      p_errors += message;
    }
  }
  return false;
}

bool    
SQLInfoDB::MakeInfoTablePrivileges(MPrivilegeMap& p_privileges,XString& p_errors,XString p_schema,XString p_tablename)
{
  XString sql = GetCATALOGTablePrivileges(p_schema,p_tablename);   // Table privileges query
  if(sql.IsEmpty() || m_preferODBC)
  {
    return SQLInfo::MakeInfoTablePrivileges(p_privileges,p_errors,p_schema,p_tablename);
  }

  try
  {
    SQLQuery query(m_database);

    if(!p_schema    .IsEmpty()) query.SetParameter(p_schema);
    if(!p_tablename .IsEmpty()) query.SetParameter(p_tablename);

    query.DoSQLStatement(sql);
    while(query.GetRecord())
    {
      MetaPrivilege priv;

      priv.m_catalogName = (XString) query[1];
      priv.m_schemaName  = (XString) query[2];
      priv.m_tableName   = (XString) query[3];
      priv.m_grantor     = (XString) query[4];
      priv.m_grantee     = (XString) query[5];
      priv.m_privilege   = (XString) query[6];
      priv.m_grantable   = ((XString)query[7]).Compare(_T("YES")) == 0;

      p_privileges.push_back(priv);
    }
    return !p_privileges.empty();
  }
  catch(StdException& er)
  {
    ReThrowSafeException(er);
    p_errors += er.GetErrorMessage();
  }
  return false;
}

bool
SQLInfoDB::MakeInfoColumnPrivileges(MPrivilegeMap& p_privileges,XString& p_errors,XString p_schema,XString p_tablename,XString p_columnname /*= ""*/)
{
  XString sql = GetCATALOGColumnPrivileges(p_schema,p_tablename,p_columnname);   // Column privileges query
  if(sql.IsEmpty() || m_preferODBC)
  {
    return SQLInfo::MakeInfoColumnPrivileges(p_privileges,p_errors,p_schema,p_tablename,p_columnname);
  }

  try
  {
    SQLQuery query(m_database);

    if(!p_schema    .IsEmpty()) query.SetParameter(p_schema);
    if(!p_tablename .IsEmpty()) query.SetParameter(p_tablename);
    if(!p_columnname.IsEmpty()) query.SetParameter(p_columnname);

    query.DoSQLStatement(sql);
    while(query.GetRecord())
    {
      MetaPrivilege priv;

      priv.m_catalogName = (XString) query[1];
      priv.m_schemaName  = (XString) query[2];
      priv.m_tableName   = (XString) query[3];
      priv.m_columnName  = (XString) query[4];
      priv.m_grantor     = (XString) query[5];
      priv.m_grantee     = (XString) query[6];
      priv.m_privilege   = (XString) query[7];
      priv.m_grantable   = ((XString)query[8]).Compare(_T("YES")) == 0;

      p_privileges.push_back(priv);
    }
    return !p_privileges.empty();
  }
  catch(StdException& er)
  {
    ReThrowSafeException(er);
    p_errors += er.GetErrorMessage();
  }
  return false;
}

bool    
SQLInfoDB::MakeInfoViewDefinition(XString& p_defintion,XString& p_errors,XString p_schema,XString p_viewname)
{
  bool result = false;
  // Try quoted identifiers first
  XString sql = GetCATALOGViewText(p_schema,p_viewname,true);
  if(!sql.IsEmpty())
  {
    try
    {
      for(int ind = 0;ind < 2;++ind)
      {
        // RDBMS might store view definition in multiple catalog records
        SQLQuery query(m_database);
        query.DoSQLStatement(sql);
        while(query.GetRecord())
        {
          p_defintion += query.GetColumn(1)->GetAsString();
          result = true;
        }
        if(result)
        {
          return true;
        }
        // Try standard identifiers
        sql = GetCATALOGViewText(p_schema,p_viewname);
      }
    }
    catch(StdException& er)
    {
      ReThrowSafeException(er);
      p_errors += er.GetErrorMessage();
    }
  }
  return result;
}

//////////////////////////////////////////////////////////////////////////
//
// PRIVATE
//
//////////////////////////////////////////////////////////////////////////

bool
SQLInfoDB::ReadMetaTypesFromQuery(SQLQuery& p_query,MMetaMap& p_objects,int p_type)
{
  while(p_query.GetRecord())
  {
    MetaObject object;
    object.m_objectType = p_type;
    object.m_objectName = (XString) p_query[1];
    object.m_remarks    = (XString) p_query[2];

    p_objects.push_back(object);
  }
  return true;
}

bool
SQLInfoDB::ReadTablesFromQuery(SQLQuery& p_query,MTableMap& p_tables)
{
  while(p_query.GetRecord())
  {
    MetaTable table;

    table.m_catalog    = (XString) p_query[1];
    table.m_schema     = (XString) p_query[2];
    table.m_table      = (XString) p_query[3];
    table.m_objectType = (XString) p_query[4];
    table.m_remarks    = (XString) p_query[5];
    table.m_tablespace = (XString) p_query[6];
    table.m_temporary  = (bool)    p_query[7];

    // Some RDBMS's still have CHAR catalog fields, padded with spaces
    table.m_catalog.Trim();
    table.m_schema.Trim();
    table.m_table.Trim();
    table.m_objectType.Trim();
    table.m_remarks.Trim();
    table.m_tablespace.Trim();

    p_tables.push_back(table);
  }
  return !p_tables.empty();
}

// End of namespace
}
