////////////////////////////////////////////////////////////////////////
//
// File: SQLTransaction.h
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
#pragma once
#include <sqltypes.h>
#include "Locker.h"

namespace SQLComponents
{ 

// Forward declaration
class SQLDatabase;

class SQLTransaction 
{
public:
  SQLTransaction(SQLDatabase* p_database, 
                 LPCTSTR      p_name,
                 bool         p_startImmediate   = true, 
                 bool         p_isSubTransaction = false);
  SQLTransaction(HDBC p_hdbc, bool p_startImmediate);

  // Destructor will rollback at an open transaction
 ~SQLTransaction();

  // Transaction on the database. If a transaction is already started and isSubTransaction
  // is requested, it will start a subtransaction's SAVEPOINT, that can be rollback'ed 
  // independently. Whenever a subtransaction is off, the transaction will be linked to 
  // earlier transactions, and thus influence the complete behaviour,
  // until the last commit/rollback of the main transaction
  void    Start(XString p_name,bool p_startSubtransaction = false);
  // Commit the transaction
  void    Commit();
  // Rollback the transaction
  void    Rollback();
  // Setting a transaction in a deferred state
  // so that constraints get only committed at the end
  bool    SetTransactionDeferred();
  // Setting a transaction in an immediate state
  // So that the constraints (uptil now) get checked immediately
  bool    SetTransactionImmediate();

  // GETTERS

  // Getting the name of the transaction
  XString GetName() const;
  // Name of the savepoint in the database
  XString GetSavePoint() const;
  // Database for which the transaction is active
  SQLDatabase* GetDatabase() const;
  // Is the transaction (still) active
  bool    IsActive() const;

private:
  // To do after a rollback. Only SQLDatabase may call this method
  friend class SQLDatabase;
  void    AfterRollback();
  void    SetSavepoint(XString p_savepoint);

private:
  SQLDatabase*  m_database;
  HDBC          m_hdbc;
  XString       m_name;
  XString       m_savepoint;
  bool          m_active;
  // Lock database for multi-access from other threads
  // For as long as the current transaction takes
  Locker<SQLDatabase> m_lock;
};

inline bool 
SQLTransaction::IsActive() const
{
  return m_active;
}

inline SQLDatabase*
SQLTransaction::GetDatabase() const
{
  return m_database;
}

inline XString 
SQLTransaction::GetSavePoint() const
{
  return m_savepoint;
}

inline XString
SQLTransaction::GetName() const
{
  return m_name;
}

inline void
SQLTransaction::SetSavepoint(XString p_savepoint)
{
  m_savepoint = p_savepoint;
}

// End of namespace
}
