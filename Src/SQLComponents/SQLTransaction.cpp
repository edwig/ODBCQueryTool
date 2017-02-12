////////////////////////////////////////////////////////////////////////
//
// File: SQLTransaction.cpp
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
#include "SQLTransaction.h"
#include "SQLDatabase.h"
#include "SQLInfoDB.h"
#include "SQLQuery.h"
#include "SQLWrappers.h"
#include <atltrace.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{

SQLTransaction::SQLTransaction(SQLDatabase* p_database
                              ,const char*  p_name
                              ,bool         p_startImmediate
                              ,bool         p_isSubTransaction) 
               :m_database  (p_database)
               ,m_lock      (p_database,INFINITE)
               ,m_active    (false)
{
  // If asked for, start it right away
  if(p_startImmediate)
  {
    Start(p_name, p_isSubTransaction);
  }
}

SQLTransaction::SQLTransaction(HDBC p_hdbc,bool p_startImmediate)
               :m_hdbc(p_hdbc)
               ,m_database(NULL)
               ,m_lock(NULL,INFINITE)
               ,m_active(false)
{
  if(p_startImmediate)
  {
    Start("",false);
  }
}

SQLTransaction::~SQLTransaction()
{
  try
  {
    Rollback();
  }
  catch(CString& error)
  {
    CString message;
    message.Format("Error in rollback of transaction [%s] : %s\n",m_name,error);
    if(m_database)
    {
      m_database->LogPrint(LOGLEVEL_ERROR,message);
    }
    else
    {
      // No database. Poor man's logging
      ATLTRACE(message);
    }
    // Cannot throw in a destructor. Stops here
    // But we where already 'cornered', why would we otherwise need to rollback :-(
  }
}

void 
SQLTransaction::Start(CString p_name, bool p_startSubtransaction)
{
  // On transaction per instance
  if(m_active)
  {
    CString message;
    message.Format("Error in start-transaction [%s] : Already started a transaction",m_name);
    throw message;
  }

  // Try to start the transaction
  if(m_database)
  {
    m_savepoint = m_database->StartTransaction(this, p_startSubtransaction);
  }
  else
  {
    SQLRETURN ret = SqlSetConnectAttr(m_hdbc,SQL_ATTR_AUTOCOMMIT,(SQLPOINTER)SQL_AUTOCOMMIT_OFF,SQL_IS_UINTEGER);
    if(!SQL_SUCCEEDED(ret))
    {
      throw CString("Error setting autocommit mode to 'off', starting transaction: ") + m_name;
    }
  }
  // We are alive!
  m_name   = p_name;
  m_active = true;
}

void 
SQLTransaction::Commit()
{
  // Error if there is no active transaction
  if(!m_active)
  {
    CString message;
    message.Format("Error in commit of [%s] : transaction object is not opened",m_name);
    throw message;
  }

  // We are no longer started/active, so we do nothing else after destruction
  // so commit's are not tried double on the database
  // NOTE: Savepoints must remain till after the commits for the databagse
  m_active = false;
  
  // Do the commit, if it fails, the database will 
  // automatically do a rollback
  if(m_database)
  {
    m_database->CommitTransaction(this);
  }
  else
  {
    // Do the commit straigth away
    SQLRETURN ret = SqlEndTran(SQL_HANDLE_DBC,m_hdbc,SQL_COMMIT);
    if(!SQL_SUCCEEDED(ret))
    {
      // Throw something, so we reach the catch block
      throw CString("Error commiting transaction: " + m_name);
    }
    ret = SqlSetConnectAttr(m_hdbc,SQL_ATTR_AUTOCOMMIT,(SQLPOINTER)SQL_AUTOCOMMIT_ON,SQL_IS_UINTEGER);
    if(!SQL_SUCCEEDED(ret))
    {
      // Not an error in all RDBMS'es. In MS-Access this is default behaviour
      // So we log the error instead of throwing it. 
      // But as we do not have the database object to log it, we TRACE it :-(
      ATLTRACE("Error setting autocommit mode to 'on', after committed transaction [%s]\n",m_name);
    }
  }
  // Cleanup after use
  m_name      = "";
  m_savepoint = "";
}

void 
SQLTransaction::Rollback()
{
  // See if we are still an active transaction
  if(!m_active)
  {
    return;
  }

  // Do the rollback. Cleaning will be done by
  // the AfterRollback method, called by SQLDatabase
  if(m_database)
  {
    m_database->RollbackTransaction(this);
  }
  else
  {
    // Do the rollback
    SQLRETURN ret = SqlEndTran(SQL_HANDLE_DBC,m_hdbc,SQL_ROLLBACK);
    if(!SQL_SUCCEEDED(ret))
    {
      // Throw something, so we reach the catch block
      throw CString("Error commiting transaction: " + m_name);
    }
    ret = SqlSetConnectAttr(m_hdbc,SQL_ATTR_AUTOCOMMIT,(SQLPOINTER)SQL_AUTOCOMMIT_ON,SQL_IS_UINTEGER);
    if(!SQL_SUCCEEDED(ret))
    {
      // Not an error in all RDBMS'es. In MS-Access this is default behaviour
      // So we log the error instead of throwing it. 
      // But as we do not have the database object to log it, we TRACE it :-(
      ATLTRACE("Error setting autocommit mode to 'on', after committed transaction [%s]\n",m_name);
    }
  }
}

void 
SQLTransaction::AfterRollback()
{
  // After closing the transaction by a rollback
  m_active    = false;
  m_name      = "";
  m_savepoint = "";
}

// Setting a transaction in a deferred state
// so that constraints get only committed at the end
bool 
SQLTransaction::SetTransactionDeferred()
{
  if(m_database == nullptr)
  {
    return false;
  }
  CString sql = m_database->GetSQLInfoDB()->GetSQLDeferConstraints();
  if(!sql.IsEmpty())
  {
    SQLQuery query(m_database);
    query.DoSQLStatementNonQuery(sql);
    return true;
  }
  return false;
}

// Setting a transaction in an immediate state
// So that the constraints (uptil now) get checked immediatly
bool 
SQLTransaction::SetTransactionImmediate()
{
  if(m_database == nullptr)
  {
    return false;
  }
  CString sql = m_database->GetSQLInfoDB()->GetSQLConstraintsImmediate();
  if(!sql.IsEmpty())
  {
    SQLQuery query(m_database);
    query.DoSQLStatementNonQuery(sql);
    return true;
  }
  return false;
}

// End of namespace
}
