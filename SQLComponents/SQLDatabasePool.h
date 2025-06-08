////////////////////////////////////////////////////////////////////////
//
// File: SQLDatabasePool.cpp
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
#include "SQLDatabase.h"
#include "SQLConnections.h"
#include <XString.h>
#include <deque>
#include <map>

namespace SQLComponents
{

 // Hard coded minimum amount of database connections
#define MIN_DATABASES 10
// Hard coded number of retries if max-databases is reached
// Every retry will take 1 second of waiting time
// Within this term (60 seconds) the cleanup process will come by
#define CONN_RETRIES  60

// Lists and maps
typedef std::deque<SQLDatabase*>     DbsList;
typedef std::map<XString,DbsList*>   DbsPool;


class SQLDatabasePool
{
public:
  SQLDatabasePool();
 ~SQLDatabasePool();

  // Get or make a database for this connection
  SQLDatabase*    GetDatabase(const XString& p_connectionName);
  // Get the connection by name
  SQLConnection*  GetConnection(const XString& p_connectionName);
  SQLConnection*  GetConnection(const int p_index);
  // Return current number of connections
  unsigned        GetConnections();
  // Return current number of maximum databases
  unsigned        GetMaxDatabases();
  // Get the number of free databases
  unsigned        GetFreeDatabases();
  // List with current connections (meant for logging purposes only)
  void            GetListOfConnections(XString& p_list);

    // Return a database connection to the pool
  void            GiveUp(SQLDatabase* p_database);
  // Clean-up all database connections
  void            CloseAll();
  // Cleanup: To be called in the cleanup process of the program
  void            Cleanup(bool p_aggressive = false);
  // Set current max databases allowed
  void            SetMaxDatabases(unsigned p_maximum);
  // Read all database definitions from 'database.xml'
  bool            ReadConnections(XString p_filename = _T(""),bool p_reset = false);

  // Add a column rebind for this database session: No bounds checking!
  void            AddColumnRebind(int p_sqlType, int p_cppType);
  // Add a parameter rebind for this database session: No bounds checking!
  void            AddParameterRebind(int p_sqlType, int p_cppType);
  // Adding / Deleting connections to the connections list
  bool            AddConnection(XString p_name,XString p_datasource,XString p_username,XString p_password,XString p_options);
  bool            DelConnection(XString p_name);

  // Support of logging functions (for all databases in the pool)
  void            RegisterLogContext(int p_level, LOGLEVEL p_loglevel, LOGPRINT p_logprinter, void* p_logContext);
  void            LogPrint(LPCTSTR p_text);
  int             LogLevel();
  bool            WilLog();
  void            SetLoggingActivation(int p_loglevel);
  void            SetEncryptionKey(XString p_key);

private:
  // Get OR make a logged in database connection
  SQLDatabase* GetDatabaseInternally(DbsPool& p_pool,XString& p_connectionName);
  // Create a new database object
  SQLDatabase* MakeDatabase(XString p_connectionName);
  // Open the connection to the RDBMS server
  void         OpenDatabase(SQLDatabase* p_dbs,XString& p_connectionName);
  // Return a connection to the pool
  void         GiveUpInternally(SQLDatabase* p_database, XString& p_connectionName);
  // Cleanup in a list of databases
  void         CleanupInternally(bool p_aggressive);
  // Close all databases in the list an clean up the list
  void         CloseAllInternally();
  // Clean up the free lists
  void         CleanupAllInternally();
  // Add our rebind mappings to a newly opened database
  void         AddRebindsToDatabase(SQLDatabase* p_database);


  // Data
  bool            m_isopen          { false };          // If database pool is currently open for business
  unsigned        m_maxDatabases    { MIN_DATABASES };  // Maximum number of concurrently open database
  unsigned        m_openConnections { 0 };              // Currently open connections
  SQLConnections  m_connections;                        // Connection names out of "database.xml"
  DbsPool         m_allDatabases;                       // List with lists of all databases
  DbsPool         m_freeDatabases;                      // List with lists of currently unused databases

  // Generic logging
  LOGPRINT        m_logPrinter   { nullptr };           // Printing a line to the logger
  LOGLEVEL        m_logLevel     { nullptr };           // Getting the log level
  void*           m_logContext   { nullptr };           // Logging context (e.g. and object)
  int             m_loggingLevel { 0       };           // Current level
  int             m_logActive    {LOGLEVEL_MAX};        // Threshold: Log only above this loglevel

  // General rebind mapping for new databases
  RebindMap       m_rebindParameters;                   // Rebinding of parameters for SQLBindParam
  RebindMap       m_rebindColumns;                      // Rebinding of result columns for SQLBindCol

  // Thread synchronization
  CRITICAL_SECTION m_lock;
};

inline void
SQLDatabasePool::SetLoggingActivation(int p_loglevel)
{
  m_logActive = p_loglevel;
}

inline void
SQLDatabasePool::SetEncryptionKey(XString p_key)
{
  m_connections.SetEncryptionKey(p_key);
}

}
