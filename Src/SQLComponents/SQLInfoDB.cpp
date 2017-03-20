////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoDB.cpp
//
// Copyright (c) 1998-2017 ir. W.E. Huisman
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
// Last Revision:   08-01-2017
// Version number:  1.4.0
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


bool    
SQLInfoDB::MakeInfoProcedureProcedurepart(CString         p_schema
                                         ,CString         p_procedure
                                         ,MProcedureMap&  p_procedures
                                         ,CString&        p_errors)
{
  p_procedures.clear();

  if(p_procedure.Compare("%") == 0)
  {
    CString sql = GetPSMProcedureList("");
    if(!sql.IsEmpty())
    {
      SQLQuery qry(m_database);
      qry.DoSQLStatement(sql);
      while(qry.GetRecord())
      {
        MetaProcedure proc;
        proc.m_schemaName    = qry.GetColumn(1)->GetAsChar();
        proc.m_procedureName = qry.GetColumn(2)->GetAsChar();
        p_procedures.push_back(proc);
      }
    }
  }

  // Let ODBC handle the call
  if(p_procedures.empty())
  {
    return SQLInfo::MakeInfoProcedureProcedurepart(p_schema,p_procedure,p_procedures,p_errors);
  }

  // Call attributes per record
  for(auto& proc : p_procedures)
  {
    CString sql = GetPSMProcedureAttributes(proc.m_schemaName,proc.m_procedureName);
    if(!sql.IsEmpty())
    {
      SQLQuery qry(m_database);
      qry.DoSQLStatement(sql);
      while(qry.GetRecord())
      {
        proc.m_inputParameters  = qry.GetColumn(4)->GetAsSLong();
        proc.m_outputParameters = qry.GetColumn(5)->GetAsSLong();
        proc.m_resultSets       = qry.GetColumn(6)->GetAsSLong();
        proc.m_remarks          = qry.GetColumn(7)->GetAsChar();
        proc.m_procedureType    = qry.GetColumn(8)->GetAsSLong();
        proc.m_source           = qry.GetColumn(9)->GetAsChar();
      }
    }
  }
  return !p_procedures.empty();
}

bool    
SQLInfoDB::MakeInfoProcedureParameters(MProcColumnMap& p_parameters,CString& p_errors)
{
  CString sql = GetPSMProcedureParameters(m_searchSchemaName,m_searchTableName);
  if(sql.IsEmpty())
  {
    // No SQL, let ODBC handle the parameters
    return SQLInfo::MakeInfoProcedureParameters(p_parameters,p_errors);
  }
  SQLQuery qry(m_database);
  qry.DoSQLStatement(sql);
  while(qry.GetRecord())
  {
    MetaParameter param;
  }
  return !p_parameters.empty();
}


// Getting the info for the triggers is not a part of the standard
// ODBC function set. It needs a RDBMS driver, so it is not in DBInfo but in DBInfoDB
bool
SQLInfoDB::MakeInfoTableTriggers(MTriggerMap& p_triggers,CString& p_errors)
{
  // Must have searched on a table first
  if(m_searchTableName.IsEmpty())
  {
    p_errors = "Search for a table first!";
    return false;
  }

  // Getting the database dependent SQL string
  CString sql = GetCATALOGTriggerList(m_searchSchemaName,m_searchTableName);
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

      p_triggers.push_back(trigger);
    }
  }
  catch(CString& error)
  {
    // Pass on the database error
    p_errors = error;
  }
  return !p_triggers.empty();
}

bool 
SQLInfoDB::MakeInfoTableSequences(MSequenceMap& p_sequences,CString& p_errors)
{
  // Must have searched on a table first
  if(m_searchTableName.IsEmpty())
  {
    p_errors = "Search for a table first!";
    return false;
  }

  // Getting the database dependent SQL string
  CString sql = GetCATALOGSequenceList(m_searchSchemaName,m_searchTableName);
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
      sequence.m_currentValue = qry.GetColumn(4)->GetAsSLong();
      sequence.m_minimalValue = qry.GetColumn(5)->GetAsSLong();
      sequence.m_increment    = qry.GetColumn(6)->GetAsSLong();
      sequence.m_cache        = qry.GetColumn(7)->GetAsSLong();
      sequence.m_cycle        = qry.GetColumn(8)->GetAsBoolean();
      sequence.m_order        = qry.GetColumn(9)->GetAsBoolean();

      p_sequences.push_back(sequence);
    }
  }
  catch(CString& error)
  {
    // Pass on the database error
    p_errors = error;
  }
  return !p_sequences.empty();
}

// End of namespace
}
