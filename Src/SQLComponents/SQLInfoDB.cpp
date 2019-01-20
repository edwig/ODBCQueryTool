////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoDB.cpp
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
{
  // Granted users.
  // Comma separated list of granted users
  // e.g. "meta3", "meta2", "meta1", "model" and "data"

  // Must be 'PUBLIC' for an ANSI-compliant database
  m_grantedUsers = "PUBLIC";
}

SQLInfoDB::~SQLInfoDB()
{
}

// Searching for a 'table' like object
// Can be 'TABLE', 'VIEW', 'ALIAS', 'SYNONYM', 'SYSTEM TABLE' etc
bool    
SQLInfoDB::MakeInfoTableObject(MTableMap& p_tables
                               ,CString&  p_errors
                               ,CString   p_schema
                               ,CString   p_tablename)
{
  // Clear the results
  p_tables.clear();
  p_errors.Empty();

  CString sql1 = GetCATALOGTableAttributes(p_schema,p_tablename);
  CString sql2 = GetCATALOGViewAttributes (p_schema,p_tablename);
  CString sql3 = GetCATALOGTableSynonyms  (p_schema,p_tablename);
  CString sql4 = GetCATALOGTableCatalog   (p_schema,p_tablename);

  if(sql1.IsEmpty() || sql2.IsEmpty() || sql3.IsEmpty() || sql4.IsEmpty())
  {
    // Ask ODBC to find all object types (empty search argument)
    return SQLInfo::MakeInfoTableTable(p_tables,p_errors,p_schema,p_tablename,"");
  }

  // Search on all types
  CString uni = "UNION ALL\n";
  CString sql = sql1 + uni + sql2 + uni + sql3 + uni + sql4;

  try
  {
    SQLQuery qry(m_database);
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
SQLInfoDB::MakeInfoMetaTypes(MMetaMap& p_objects,CString& p_errors,int p_type)
{
  CString sql = GetCATALOGMetaTypes(p_type);
  if(sql.IsEmpty())
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
  catch(CString& er)
  {
    p_errors.Append(er);
  }
  return 0;
}

bool
SQLInfoDB::MakeInfoTableTable(MTableMap& p_tables
                             ,CString&   p_errors
                             ,CString    p_schema
                             ,CString    p_tablename)
{
  CString sql = GetCATALOGTableAttributes(p_schema,p_tablename);
  if(sql.IsEmpty())
  {
    // Ask ODBC driver to find tables
    return SQLInfo::MakeInfoTableTable(p_tables,p_errors,p_schema,p_tablename,"TABLE");
  }

  try
  {
    SQLQuery qry(m_database);
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
                            ,CString&   p_errors
                            ,CString    p_schema
                            ,CString    p_tablename)
{
  CString sql = GetCATALOGViewAttributes(p_schema,p_tablename);
  if(sql.IsEmpty())
  {
    // Ask ODBC driver to find views
    return SQLInfo::MakeInfoTableTable(p_tables,p_errors,p_schema,p_tablename,"VIEW");
  }

  try
  {
    SQLQuery qry(m_database);
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
                                ,CString&   p_errors
                                ,CString    p_schema
                                ,CString    p_tablename)
{
  CString sql = GetCATALOGTableSynonyms(p_schema,p_tablename);
  if(sql.IsEmpty())
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
                               ,CString&    p_errors
                               ,CString     p_schema
                               ,CString     p_tablename)
{
  CString sql = GetCATALOGTableCatalog(p_schema,p_tablename);
  if(sql.IsEmpty())
  {
    p_schema = "%";
    p_tablename = "%";
    // Ask ODBC driver to find system tables
    return SQLInfo::MakeInfoTableTable(p_tables,p_errors,p_schema,p_tablename,"SYSTEM TABLE");
  }

  try
  {
    SQLQuery qry(m_database);
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
                               ,CString&    p_errors
                               ,CString     p_schema
                               ,CString     p_tablename
                               ,CString     p_columname /*=""*/)
{
  CString sql = GetCATALOGColumnAttributes(p_schema,p_tablename,p_columname);
  if(sql.IsEmpty())
  {
    return SQLInfo::MakeInfoTableColumns(p_columns,p_errors,p_schema,p_tablename,p_columname);
  }
  try
  {
    SQLQuery qry(m_database);
    qry.DoSQLStatement(sql);
    while(qry.GetRecord())
    {
      MetaColumn column;

      column.m_catalog        = (CString) qry[1];
      column.m_schema         = (CString) qry[2];
      column.m_table          = (CString) qry[3];
      column.m_column         = (CString) qry[4];
      column.m_datatype       = (int)     qry[5];
      column.m_typename       = (CString) qry[6];
      column.m_columnSize     = (int)     qry[7];
      column.m_bufferLength   = (int)     qry[8];
      column.m_decimalDigits  = (int)     qry[9];
      column.m_numRadix       = (int)     qry[10];
      column.m_nullable       = (int)     qry[11];
      column.m_remarks        = (CString) qry[12];
      column.m_default        = (CString) qry[13];
      column.m_datatype3      = (int)     qry[14];
      column.m_sub_datatype   = (int)     qry[15];
      column.m_octet_length   = (int)     qry[16];
      column.m_position       = (int)     qry[17];
      column.m_isNullable     = (CString) qry[18];

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
                               ,CString&      p_errors
                               ,CString       p_schema
                               ,CString       p_tablename)
{
  CString sql = GetCATALOGPrimaryAttributes(p_schema,p_tablename);
  if(sql.IsEmpty())
  {
    return SQLInfo::MakeInfoTablePrimary(p_primaries,p_errors,p_schema,p_tablename);
  }

  try
  {
    SQLQuery qry(m_database);
    qry.DoSQLStatement(sql);
    while(qry.GetRecord())
    {
      MetaPrimary prim;

      prim.m_catalog        = (CString) qry[1];
      prim.m_catalog        = qry.GetColumn(1)->GetAsChar();
      prim.m_schema         = qry.GetColumn(2)->GetAsChar();
      prim.m_table          = qry.GetColumn(3)->GetAsChar();
      prim.m_columnName     = qry.GetColumn(4)->GetAsChar();
      prim.m_columnPosition = qry.GetColumn(5)->GetAsSLong();
      prim.m_constraintName = qry.GetColumn(6)->GetAsChar();

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
                               ,CString&      p_errors
                               ,CString       p_schema
                               ,CString       p_tablename
                               ,bool          p_referenced /* = false */) 
{
  CString sql = GetCATALOGForeignAttributes(p_schema,p_tablename,"",p_referenced);
  if(sql.IsEmpty())
  {
    return SQLInfo::MakeInfoTableForeign(p_foreigns,p_errors,p_schema,p_tablename,p_referenced);
  }
  try
  {
    SQLQuery query(m_database);
    query.DoSQLStatement(sql);
    while(query.GetRecord())
    {
      MetaForeign foreign;

      foreign.m_pkCatalogName     = (CString) query[1];
      foreign.m_pkSchemaName      = (CString) query[2];
      foreign.m_pkTableName       = (CString) query[3];
      foreign.m_fkCatalogName     = (CString) query[4];
      foreign.m_fkSchemaName      = (CString) query[5];
      foreign.m_fkTableName       = (CString) query[6];

      foreign.m_primaryConstraint = (CString) query[7];
      foreign.m_foreignConstraint = (CString) query[8];

      foreign.m_keySequence       = (int)     query[9];
      foreign.m_pkColumnName      = (CString) query[10];
      foreign.m_fkColumnName      = (CString) query[11];

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
                                  ,CString&     p_errors
                                  ,CString      p_schema
                                  ,CString      p_tablename
                                  ,MPrimaryMap* p_keymap
                                  ,bool         p_all /*= true*/)
{
  CString sql = GetCATALOGIndexAttributes(p_schema,p_tablename,"");
  if(sql.IsEmpty())
  {
    return SQLInfo::MakeInfoTableStatistics(p_indices,p_errors,p_schema,p_tablename,p_keymap,p_all);
  }

  try
  {
    SQLQuery query(m_database);
    query.DoSQLStatement(sql);
    while(query.GetRecord())
    {
      MetaIndex index;

      index.m_catalogName = (CString) query[1];
      index.m_schemaName  = (CString) query[2];
      index.m_tableName   = (CString) query[3];
      index.m_unique      = (bool)    query[4];
      index.m_indexName   = (CString) query[5];
      index.m_indexType   = (int)     query[6];
      index.m_position    = (int)     query[7];
      index.m_columnName  = (CString) query[8];
      index.m_ascending   = (CString) query[9];
      index.m_cardinality = (int)     query[10];
      index.m_pages       = (int)     query[11];
      index.m_filter      = (CString) query[12];

      p_indices.push_back(index);
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
                                ,CString&        p_errors
                                ,CString         p_schema
                                ,CString         p_procedure)
{
  CString sql;

  if(p_procedure.IsEmpty() || p_procedure.Compare("%") == 0)
  {
    sql = GetPSMProcedureList(p_schema);
  }
  else
  {
    sql = GetPSMProcedureAttributes(p_schema,p_procedure);
  }

  // Let ODBC handle the call
  if(sql.IsEmpty())
  {
    return SQLInfo::MakeInfoPSMProcedures(p_procedures,p_errors,p_schema,p_procedure);
  }

  try
  {
    SQLQuery qry(m_database);
    qry.DoSQLStatement(sql);
    
    if(qry.GetNumberOfColumns() == 3)
    {
      // Name only
      while(qry.GetRecord())
      {
        MetaProcedure proc;

        proc.m_catalogName   = qry.GetColumn(1)->GetAsChar();
        proc.m_schemaName    = qry.GetColumn(2)->GetAsChar();
        proc.m_procedureName = qry.GetColumn(3)->GetAsChar();

        p_procedures.push_back(proc);
      }
    }
    else
    {
      // List complete procedure
    while(qry.GetRecord())
    {
      MetaProcedure proc;

      proc.m_catalogName      = qry.GetColumn(1)->GetAsChar();
      proc.m_schemaName       = qry.GetColumn(2)->GetAsChar();
      proc.m_procedureName    = qry.GetColumn(3)->GetAsChar();
      proc.m_inputParameters  = qry.GetColumn(4)->GetAsSLong();
      proc.m_outputParameters = qry.GetColumn(5)->GetAsSLong();
      proc.m_resultSets       = qry.GetColumn(6)->GetAsSLong();
      proc.m_remarks          = qry.GetColumn(7)->GetAsChar();
      proc.m_procedureType    = qry.GetColumn(8)->GetAsSLong();
      proc.m_source           = qry.GetColumn(9)->GetAsChar();

        if(proc.m_source.IsEmpty() || proc.m_source.Compare("<@>") == 0)
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

CString
SQLInfoDB::MakeInfoPSMSourcecode(CString p_schema, CString p_procedure)
{
  CString sourcecode;
  CString sql = GetPSMProcedureSourcecode(p_schema, p_procedure);
  if(!sql.IsEmpty())
  {
    SQLQuery query(m_database);
    query.DoSQLStatement(sql);
    while (query.GetRecord())
    {
      sourcecode += (CString)query[3];
    }
  }
  return sourcecode;
}

bool    
SQLInfoDB::MakeInfoPSMParameters(MParameterMap& p_parameters
                                ,CString&       p_errors
                                ,CString        p_schema
                                ,CString        p_procedure)
{
  CString sql = GetPSMProcedureParameters(p_schema,p_procedure);
  if(sql.IsEmpty())
  {
    // No SQL, let ODBC handle the parameters
    return SQLInfo::MakeInfoPSMParameters(p_parameters,p_errors,p_schema,p_procedure);
  }
  try
  {
    SQLQuery qry(m_database);
    qry.DoSQLStatement(sql);
    while(qry.GetRecord())
    {
      MetaParameter param;

      param.m_catalog       = qry.GetColumn(1)->GetAsChar();
      param.m_schema        = qry.GetColumn(2)->GetAsChar();
      param.m_procedure     = qry.GetColumn(3)->GetAsChar();
      param.m_parameter     = qry.GetColumn(4)->GetAsChar();
      param.m_columnType    = qry.GetColumn(5)->GetAsSLong();
      param.m_datatype      = qry.GetColumn(6)->GetAsSLong();
      param.m_typeName      = qry.GetColumn(7)->GetAsChar();
      param.m_columnSize    = qry.GetColumn(8)->GetAsSLong();
      param.m_bufferLength  = qry.GetColumn(9)->GetAsSLong();
      param.m_decimalDigits = qry.GetColumn(10)->GetAsSLong();
      param.m_numRadix      = qry.GetColumn(11)->GetAsSLong();
      param.m_nullable      = qry.GetColumn(12)->GetAsSLong();
      param.m_remarks       = qry.GetColumn(13)->GetAsChar();
      param.m_default       = qry.GetColumn(14)->GetAsChar();
      param.m_datatype3     = qry.GetColumn(15)->GetAsSLong();
      param.m_subType       = qry.GetColumn(16)->GetAsSLong();
      param.m_octetLength   = qry.GetColumn(17)->GetAsSLong();
      param.m_position      = qry.GetColumn(18)->GetAsSLong();
      param.m_isNullable    = qry.GetColumn(19)->GetAsChar();
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
                                ,CString& p_errors
                                ,CString p_schema
                                ,CString p_tablename /*= ""*/
                                ,CString p_trigger   /*= ""*/)
{
  // Getting the database dependent SQL string
  CString sql;
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
    qry.DoSQLStatement(sql);
    while(qry.GetRecord())
    {
      MetaTrigger trigger;
      trigger.m_catalogName = qry.GetColumn(1)->GetAsChar();
      trigger.m_schemaName  = qry.GetColumn(2)->GetAsChar();
      trigger.m_tableName   = qry.GetColumn(3)->GetAsChar();
      trigger.m_triggerName = qry.GetColumn(4)->GetAsChar();
      trigger.m_remarks     = qry.GetColumn(5)->GetAsChar();
      trigger.m_position    = qry.GetColumn(6)->GetAsSLong();
      trigger.m_before      = qry.GetColumn(7)->GetAsBoolean();
      trigger.m_insert      = qry.GetColumn(8)->GetAsBoolean();
      trigger.m_update      = qry.GetColumn(9)->GetAsBoolean();
      trigger.m_delete      = qry.GetColumn(10)->GetAsBoolean();
      trigger.m_select      = qry.GetColumn(11)->GetAsBoolean();
      trigger.m_session     = qry.GetColumn(12)->GetAsBoolean();
      trigger.m_transaction = qry.GetColumn(13)->GetAsBoolean();
      trigger.m_rollback    = qry.GetColumn(14)->GetAsBoolean();
      trigger.m_referencing = qry.GetColumn(15)->GetAsChar();
      trigger.m_enabled     = qry.GetColumn(16)->GetAsBoolean();
      trigger.m_source      = qry.GetColumn(17)->GetAsChar();

      // Some RDBMS'es have extra trailing spaces in these fields
      trigger.m_triggerName.Trim();
      trigger.m_remarks.Trim();
      trigger.m_source.Trim();
      trigger.m_source.Replace("\r\n","\n");

      if(trigger.m_source.Compare("<@>") == 0)
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
SQLInfoDB::MakeInfoTableSequences(MSequenceMap& p_sequences,CString& p_errors,CString p_schema,CString p_tablename)
{
  // Getting the database dependent SQL string
  CString sql = GetCATALOGSequenceList(p_schema,p_tablename);
  if(sql.IsEmpty())
  {
    // No triggers to be gotten from this RDBMS
    return false;
  }

  try
  {
    SQLQuery qry(m_database);
    qry.DoSQLStatement(sql);
    while(qry.GetRecord())
  {
      MetaSequence sequence;

      sequence.m_catalogName  = qry.GetColumn(1)->GetAsChar();
      sequence.m_schemaName   = qry.GetColumn(2)->GetAsChar();
      sequence.m_sequenceName = qry.GetColumn(3)->GetAsChar();
      sequence.m_currentValue = qry.GetColumn(4)->GetAsDouble();
      sequence.m_minimalValue = qry.GetColumn(5)->GetAsDouble();
      sequence.m_increment    = qry.GetColumn(6)->GetAsSLong();
      sequence.m_cache        = qry.GetColumn(7)->GetAsSLong();
      sequence.m_cycle        = qry.GetColumn(8)->GetAsBoolean();
      sequence.m_order        = qry.GetColumn(9)->GetAsBoolean();

      p_sequences.push_back(sequence);
    }
    return !p_sequences.empty();
  }
  catch(StdException& er)
  {
    ReThrowSafeException(er);
    CString message = er.GetErrorMessage();
    if(message.Find("[42S02]") > 0)
    {
      // Older versions of MS-SQLServer return this SQLSTATE
      p_errors.Append("Version of RDBMS that does not support SEQUENCE feature (yet)!");
  }
    else
    {
      p_errors += message;
    }
  }
  return 0;
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
    object.m_objectName = (CString) p_query[1];
    object.m_remarks    = (CString) p_query[2];

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

    table.m_catalog    = (CString) p_query[1];
    table.m_schema     = (CString) p_query[2];
    table.m_table      = (CString) p_query[3];
    table.m_objectType = (CString) p_query[4];
    table.m_remarks    = (CString) p_query[5];
    table.m_tablespace = (CString) p_query[6];
    table.m_temporary  = (bool)    p_query[7];

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
