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

  XString sql1 = GetCATALOGTableAttributes(p_schema,p_tablename);
  XString sql2 = GetCATALOGViewAttributes (p_schema,p_tablename);
  XString sql3 = GetCATALOGTableSynonyms  (p_schema,p_tablename);
  XString sql4 = GetCATALOGTableCatalog   (p_schema,p_tablename);

  if(sql1.IsEmpty() || sql2.IsEmpty() || sql3.IsEmpty() || sql4.IsEmpty() || m_preferODBC)
  {
    // Ask ODBC to find all object types (empty search argument)
    return SQLInfo::MakeInfoTableTable(p_tables,p_errors,p_schema,p_tablename,"");
  }

  try
  {
    SQLQuery qry(m_database);

    if(!p_schema   .IsEmpty()) qry.SetParameter(p_schema);
    if(!p_tablename.IsEmpty()) qry.SetParameter(p_tablename);

    qry.DoSQLStatement(sql1);
    ReadTablesFromQuery(qry,p_tables);
    qry.DoSQLStatement(sql2);
    ReadTablesFromQuery(qry,p_tables);
    qry.DoSQLStatement(sql3);
    ReadTablesFromQuery(qry,p_tables);
    qry.DoSQLStatement(sql4);
    ReadTablesFromQuery(qry,p_tables);
    return !p_tables.empty();
  }
  catch(StdException& er)
  {
    ReThrowSafeException(er);
    p_errors += er.GetErrorMessage();
  }
  return 0;
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
  XString sql = GetCATALOGTableAttributes(p_schema,p_tablename);
  if(sql.IsEmpty() || m_preferODBC)
  {
    // Ask ODBC driver to find tables
    return SQLInfo::MakeInfoTableTable(p_tables,p_errors,p_schema,p_tablename,"TABLE");
  }

  try
  {
    SQLQuery qry(m_database);

    if(!p_schema   .IsEmpty()) qry.SetParameter(p_schema);
    if(!p_tablename.IsEmpty()) qry.SetParameter(p_tablename);

    qry.DoSQLStatement(sql);
    return ReadTablesFromQuery(qry,p_tables);
  }
  catch(StdException& er)
  {
    ReThrowSafeException(er);
    p_errors += er.GetErrorMessage();
  }
  return 0;
}

bool    
SQLInfoDB::MakeInfoTableView(MTableMap& p_tables
                            ,XString&   p_errors
                            ,XString    p_schema
                            ,XString    p_tablename)
{
  XString sql = GetCATALOGViewAttributes(p_schema,p_tablename);
  if(sql.IsEmpty() || m_preferODBC)
  {
    // Ask ODBC driver to find views
    return SQLInfo::MakeInfoTableTable(p_tables,p_errors,p_schema,p_tablename,"VIEW");
  }

  try
  {
    SQLQuery qry(m_database);

    if(!p_schema   .IsEmpty()) qry.SetParameter(p_schema);
    if(!p_tablename.IsEmpty()) qry.SetParameter(p_tablename);

    qry.DoSQLStatement(sql);
    return ReadTablesFromQuery(qry,p_tables);
  }
  catch(StdException& er)
  {
    ReThrowSafeException(er);
    p_errors += er.GetErrorMessage();
  }
  return 0;
}

bool    
SQLInfoDB::MakeInfoTableSynonyms(MTableMap& p_tables
                                ,XString&   p_errors
                                ,XString    p_schema
                                ,XString    p_tablename)
{
  XString sql = GetCATALOGTableSynonyms(p_schema,p_tablename);
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
    SQLQuery qry(m_database);

    if(!p_schema   .IsEmpty()) qry.SetParameter(p_schema);
    if(!p_tablename.IsEmpty()) qry.SetParameter(p_tablename);

    qry.DoSQLStatement(sql);
    return ReadTablesFromQuery(qry,p_tables);
  }
  catch(StdException& er)
  {
    ReThrowSafeException(er);
    p_errors += er.GetErrorMessage();
  }
  return 0;
}

bool    
SQLInfoDB::MakeInfoTableCatalog(MTableMap&  p_tables
                               ,XString&    p_errors
                               ,XString     p_schema
                               ,XString     p_tablename)
{
  XString sql = GetCATALOGTableCatalog(p_schema,p_tablename);
  if(sql.IsEmpty() || m_preferODBC)
  {
    p_schema    = _T("%");
    p_tablename = _T("%");
    // Ask ODBC driver to find system tables
    return SQLInfo::MakeInfoTableTable(p_tables,p_errors,p_schema,p_tablename,_T("SYSTEM TABLE"));
  }

  try
  {
    SQLQuery qry(m_database);

    if(!p_schema   .IsEmpty()) qry.SetParameter(p_schema);
    if(!p_tablename.IsEmpty()) qry.SetParameter(p_tablename);

    qry.DoSQLStatement(sql);
    return ReadTablesFromQuery(qry,p_tables);
  }
  catch(StdException& er)
  {
    ReThrowSafeException(er);
    p_errors += er.GetErrorMessage();
  }
  return 0;
}

bool    
SQLInfoDB::MakeInfoTableColumns(MColumnMap& p_columns
                               ,XString&    p_errors
                               ,XString     p_schema
                               ,XString     p_tablename
                               ,XString     p_columnname /*=""*/)
{
  XString sql = GetCATALOGColumnAttributes(p_schema,p_tablename,p_columnname);
  if(sql.IsEmpty() || m_preferODBC)
  {
    return SQLInfo::MakeInfoTableColumns(p_columns,p_errors,p_schema,p_tablename,p_columnname);
  }
  try
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
    return !p_columns.empty();
  }
  catch(StdException& er)
  {
    ReThrowSafeException(er);
    p_errors += er.GetErrorMessage();
  }
  return 0;
}

bool    
SQLInfoDB::MakeInfoTablePrimary(MPrimaryMap&  p_primaries
                               ,XString&      p_errors
                               ,XString       p_schema
                               ,XString       p_tablename)
{
  XString sql = GetCATALOGPrimaryAttributes(p_schema,p_tablename);
  if(sql.IsEmpty() || m_preferODBC)
  {
    return SQLInfo::MakeInfoTablePrimary(p_primaries,p_errors,p_schema,p_tablename);
  }

  try
  {
    SQLQuery qry(m_database);

    if(!p_schema   .IsEmpty()) qry.SetParameter(p_schema);
    if(!p_tablename.IsEmpty()) qry.SetParameter(p_tablename);

    qry.DoSQLStatement(sql);
    while(qry.GetRecord())
    {
      MetaPrimary prim;

      prim.m_catalog        = (XString) qry[1];
      prim.m_catalog        = qry.GetColumn(1)->GetAsString();
      prim.m_schema         = qry.GetColumn(2)->GetAsString();
      prim.m_table          = qry.GetColumn(3)->GetAsString();
      prim.m_columnName     = qry.GetColumn(4)->GetAsString();
      prim.m_columnPosition = qry.GetColumn(5)->GetAsSLong();
      prim.m_constraintName = qry.GetColumn(6)->GetAsString();

      p_primaries.push_back(prim);
    }
    return !p_primaries.empty();
  }
  catch(StdException& er)
  {
    ReThrowSafeException(er);
    p_errors += er.GetErrorMessage();
  }
  return 0;
}

bool    
SQLInfoDB::MakeInfoTableForeign(MForeignMap&  p_foreigns
                               ,XString&      p_errors
                               ,XString       p_schema
                               ,XString       p_tablename
                               ,bool          p_referenced /* = false */) 
{
  XString constraint;
  XString sql = GetCATALOGForeignAttributes(p_schema,p_tablename,constraint,p_referenced);
  if(sql.IsEmpty() || m_preferODBC)
  {
    return SQLInfo::MakeInfoTableForeign(p_foreigns,p_errors,p_schema,p_tablename,p_referenced);
  }
  try
  {
    SQLQuery query(m_database);

    if(!p_schema   .IsEmpty()) query.SetParameter(p_schema);
    if(!p_tablename.IsEmpty()) query.SetParameter(p_tablename);
    if(!constraint .IsEmpty()) query.SetParameter(constraint);

    query.DoSQLStatement(sql);
    while(query.GetRecord())
    {
      MetaForeign foreign;

      foreign.m_pkCatalogName     = (XString) query[1];
      foreign.m_pkSchemaName      = (XString) query[2];
      foreign.m_pkTableName       = (XString) query[3];
      foreign.m_fkCatalogName     = (XString) query[4];
      foreign.m_fkSchemaName      = (XString) query[5];
      foreign.m_fkTableName       = (XString) query[6];

      foreign.m_primaryConstraint = (XString) query[7];
      foreign.m_foreignConstraint = (XString) query[8];

      foreign.m_keySequence       = (int)     query[9];
      foreign.m_pkColumnName      = (XString) query[10];
      foreign.m_fkColumnName      = (XString) query[11];

      foreign.m_updateRule        = (int)     query[12];
      foreign.m_deleteRule        = (int)     query[13];
      foreign.m_deferrable        = (int)     query[14];
      foreign.m_match             = (int)     query[15];

      foreign.m_initiallyDeferred = (int)     query[16];
      foreign.m_enabled           = (int)     query[17];

      p_foreigns.push_back(foreign);
    }
    return !p_foreigns.empty();
  }
  catch(StdException& er)
  {
    ReThrowSafeException(er);
    p_errors += er.GetErrorMessage();
  }
  return 0;
}

bool
SQLInfoDB::MakeInfoTableStatistics(MIndicesMap& p_indices
                                  ,XString&     p_errors
                                  ,XString      p_schema
                                  ,XString      p_tablename
                                  ,MPrimaryMap* p_keymap
                                  ,bool         p_all         /*=true*/)
{
  XString column;
  XString sql1 = GetCATALOGIndexAttributes(p_schema,p_tablename,column);   // Indices
  column = "0";
  XString sql2 = GetCATALOGIndexAttributes(p_schema,p_tablename,column);   // Table statistics
  if(sql1.IsEmpty() || m_preferODBC)
  {
    return SQLInfo::MakeInfoTableStatistics(p_indices,p_errors,p_schema,p_tablename,p_keymap,p_all);
  }

  try
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

        index.m_catalogName = (XString) query[1];
        index.m_schemaName  = (XString) query[2];
        index.m_tableName   = (XString) query[3];
        index.m_nonunique   = (bool)    query[4];
        index.m_indexName   = (XString) query[5];
        index.m_indexType   = (int)     query[6];
        index.m_position    = (int)     query[7];
        index.m_columnName  = (XString) query[8];
        index.m_ascending   = (XString) query[9];
        index.m_cardinality = (int)     query[10];
        index.m_pages       = (int)     query[11];
        index.m_filter      = (XString) query[12];

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
    return !p_indices.empty();
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
    sql = GetPSMProcedureList(p_schema);
    p_procedure.Empty();
  }
  else
  {
    sql = GetPSMProcedureAttributes(p_schema,p_procedure);
  }

  // Let ODBC handle the call
  if(sql.IsEmpty() || m_preferODBC)
  {
    return SQLInfo::MakeInfoPSMProcedures(p_procedures,p_errors,p_schema,p_procedure);
  }

  try
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

        proc.m_catalogName   = qry.GetColumn(1)->GetAsString();
        proc.m_schemaName    = qry.GetColumn(2)->GetAsString();
        proc.m_procedureName = qry.GetColumn(3)->GetAsString();
        proc.m_procedureType = qry.GetColumn(4)->GetAsSLong();

        p_procedures.push_back(proc);
      }
    }
    else
    {
      // List complete procedure
      while(qry.GetRecord())
      {
        MetaProcedure proc;

        proc.m_catalogName      = qry.GetColumn(1)->GetAsString();
        proc.m_schemaName       = qry.GetColumn(2)->GetAsString();
        proc.m_procedureName    = qry.GetColumn(3)->GetAsString();
        proc.m_inputParameters  = qry.GetColumn(4)->GetAsSLong();
        proc.m_outputParameters = qry.GetColumn(5)->GetAsSLong();
        proc.m_resultSets       = qry.GetColumn(6)->GetAsSLong();
        proc.m_remarks          = qry.GetColumn(7)->GetAsString();
        proc.m_procedureType    = qry.GetColumn(8)->GetAsSLong();
        proc.m_source           = qry.GetColumn(9)->GetAsString();

        if(proc.m_source.IsEmpty() || proc.m_source.Compare(_T("<@>")) == 0)
        {
          proc.m_source = MakeInfoPSMSourcecode(proc.m_schemaName, proc.m_procedureName);
        }
        p_procedures.push_back(proc);
      }
    }
    return !p_procedures.empty();
  }
  catch(StdException& er)
  {
    ReThrowSafeException(er);
    p_errors += er.GetErrorMessage();
  }
  return 0;
}

XString
SQLInfoDB::MakeInfoPSMSourcecode(XString p_schema, XString p_procedure)
{
  XString sourcecode;
  XString sql = GetPSMProcedureSourcecode(p_schema, p_procedure);
  if(!sql.IsEmpty())
  {
    SQLQuery query(m_database);
    query.DoSQLStatement(sql);
    while(query.GetRecord())
    {
      sourcecode += query.GetColumn(3)->GetAsString();
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
  XString sql = GetPSMProcedureParameters(p_schema,p_procedure);
  if(sql.IsEmpty() || m_preferODBC)
  {
    // No SQL, let ODBC handle the parameters
    return SQLInfo::MakeInfoPSMParameters(p_parameters,p_errors,p_schema,p_procedure);
  }
  try
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
    return !p_parameters.empty();
  }
  catch(StdException& er)
  {
    ReThrowSafeException(er);
    p_errors += er.GetErrorMessage();
  }
  return 0;
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
    sql = GetCATALOGTriggerList(p_schema,p_tablename);
  }
  else
  {
    sql = GetCATALOGTriggerAttributes(p_schema,p_tablename,p_trigger);
  }
  if(sql.IsEmpty())
  {
    // No triggers to be gotten from this RDBMS
    return false;
  }

  try
  {
    SQLQuery qry(m_database);

    if(!p_schema.IsEmpty())    qry.SetParameter(p_schema);
    if(!p_tablename.IsEmpty()) qry.SetParameter(p_tablename);
    if(!p_trigger.IsEmpty())   qry.SetParameter(p_trigger);

    qry.DoSQLStatement(sql);
    while(qry.GetRecord())
    {
      MetaTrigger trigger;
      trigger.m_catalogName = qry.GetColumn(1)->GetAsString();
      trigger.m_schemaName  = qry.GetColumn(2)->GetAsString();
      trigger.m_tableName   = qry.GetColumn(3)->GetAsString();
      trigger.m_triggerName = qry.GetColumn(4)->GetAsString();
      trigger.m_remarks     = qry.GetColumn(5)->GetAsString();
      trigger.m_position    = qry.GetColumn(6)->GetAsSLong();
      trigger.m_before      = qry.GetColumn(7)->GetAsBoolean();
      trigger.m_insert      = qry.GetColumn(8)->GetAsBoolean();
      trigger.m_update      = qry.GetColumn(9)->GetAsBoolean();
      trigger.m_delete      = qry.GetColumn(10)->GetAsBoolean();
      trigger.m_select      = qry.GetColumn(11)->GetAsBoolean();
      trigger.m_session     = qry.GetColumn(12)->GetAsBoolean();
      trigger.m_transaction = qry.GetColumn(13)->GetAsBoolean();
      trigger.m_rollback    = qry.GetColumn(14)->GetAsBoolean();
      trigger.m_referencing = qry.GetColumn(15)->GetAsString();
      trigger.m_enabled     = qry.GetColumn(16)->GetAsBoolean();
      trigger.m_source      = qry.GetColumn(17)->GetAsString();

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
    return !p_triggers.empty();
  }
  catch(StdException& er)
  {
    ReThrowSafeException(er);
    p_errors += er.GetErrorMessage();
  }
  return 0;
}

bool 
SQLInfoDB::MakeInfoTableSequences(MSequenceMap& p_sequences,XString& p_errors,XString p_schema,XString p_tablename)
{
  // Getting the database dependent SQL string
  XString sql = GetCATALOGSequenceList(p_schema,p_tablename);
  if(sql.IsEmpty())
  {
    // No sequences to be gotten from this RDBMS
    return false;
  }

  try
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
        XString sql2 = GetCATALOGSequenceAttributes(p_schema,sequence.m_sequenceName);
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
    return !p_sequences.empty();
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
  return 0;
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
  XString sql = GetCATALOGViewText(p_schema,p_viewname);
  if(!sql.IsEmpty())
  {
    try
    {
      // RDBMS might store view definition in multiple catalog records
      SQLQuery query(m_database);
      query.DoSQLStatement(sql);
      while(query.GetRecord())
      {
        p_defintion += query.GetColumn(1)->GetAsString();
      }
      result = true;
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
