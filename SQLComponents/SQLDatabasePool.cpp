////////////////////////////////////////////////////////////////////////
//
// File: SQLDatabasePool.cpp
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
#include "stdafx.h"
#include "SQLComponents.h"
#include "SQLDatabasePool.h"
#include "SQLDatabase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{

class AutoCritSec
{
public:
  AutoCritSec(CRITICAL_SECTION* section) : m_section(section)
  {
    EnterCriticalSection(m_section);
  }
  ~AutoCritSec()
  {
    LeaveCriticalSection(m_section);
  }
private:
  CRITICAL_SECTION* m_section;
};

//////////////////////////////////////////////////////////////////////////
//
// DATABASE POOL
//
//////////////////////////////////////////////////////////////////////////

SQLDatabasePool::SQLDatabasePool()
{
  InitializeCriticalSection(&m_lock);
  m_isopen = true;
}

SQLDatabasePool::~SQLDatabasePool()
{
  CloseAll();
  DeleteCriticalSection(&m_lock);
}

//////////////////////////////////////////////////////////////////////////
//
// PUBLIEK
// All functions have an auto lock on the stack!
//
//////////////////////////////////////////////////////////////////////////

// Get or make a database for this connection
SQLDatabase*
SQLDatabasePool::GetDatabase(const XString& p_connectionName)
{
  // Lock the pool
  AutoCritSec lock(&m_lock);

  // Make lower case before we search
  XString name(p_connectionName);
  name.MakeLower();

  // Check whether we are already open
  if(m_isopen == false)
  {
    throw StdException("INTERNAL ERROR: Database pool called after closure of the pool.");
  }
  return GetDatabaseInternally(m_freeDatabases,name);
}

// Return a database connection to the pool
void
SQLDatabasePool::GiveUp(SQLDatabase* p_database)
{
  // Lock the pool
  AutoCritSec lock(&m_lock);

  // Check that we do receive a database pointer
  if(p_database == nullptr)
  {
    return;
  }
  // CHeck whether we are already/still open
  if(m_isopen == false)
  {
    throw StdException("INTERNAL ERROR: Database pool called after closure of the pool.");
  }

  // Grab our connection name
  XString name = p_database->GetConnectionName();
  name.MakeLower();

  GiveUpInternally(p_database,name);
}

// Clean-up all database connections
void
SQLDatabasePool::CloseAll()
{
  // Lock the pools
  AutoCritSec lock(&m_lock);

  // DatabasePool already closed. Do nothing any more
  if(m_isopen == false)
  {
    return;
  }

  // Close all databases
  CloseAllInternally();

  // Clear list of free databases
  CleanupAllInternally();

  // Reset counter
  m_openConnections = 0;

  // DatabasePool is now closed
  m_isopen = false;
}

// Read all database definitions from 'database.xml'
bool
SQLDatabasePool::ReadConnections(XString p_filename /*=""*/,bool p_reset /*=false*/)
{
  // Lock the pool
  AutoCritSec lock(&m_lock);

  return m_connections.LoadConnectionsFile(p_filename,p_reset);
}

// Set current max databases allowed
void
SQLDatabasePool::SetMaxDatabases(unsigned p_maximum)
{
  // Lock the pool
  AutoCritSec lock(&m_lock);

  if(p_maximum > MIN_DATABASES)
  {
    m_maxDatabases = p_maximum;
  }
}

// Cleanup: To be called in the cleanup process of the program
// if max-databases was reached, then cleanup aggressively
// Otherwise a normal cleanup on the heartbeat of the program
void
SQLDatabasePool::Cleanup(bool p_aggressive /*=false*/)
{
  // Lock the pool
  AutoCritSec lock(&m_lock);

  // Max databases almost reached and cleanup aggressively
  if(p_aggressive)
  {
    XString text;
    text.Format("Maximum number of databases reached and aggressive cleanup requested.\n"
                "Max databases is currently: %d", m_maxDatabases);
    LogPrint(text);
  }
  CleanupInternally(p_aggressive);
}

// Return current number of connections
unsigned
SQLDatabasePool::GetConnections()
{
  // Lock the pool
  AutoCritSec lock(&m_lock);

  return m_openConnections;
}

SQLConnection* 
SQLDatabasePool::GetConnection(const XString& p_connectionName)
{
  return m_connections.GetConnection(p_connectionName);
}

// Return current number of maximum databases
unsigned
SQLDatabasePool::GetMaxDatabases()
{
  // Lock the pool
  AutoCritSec lock(&m_lock);

  return m_maxDatabases;
}

// Get the number of free databases
unsigned
SQLDatabasePool::GetFreeDatabases()
{
  // Lock the pool
  AutoCritSec lock(&m_lock);
  
  // Counter
  unsigned total = 0;

  // Count number of free connections
  for(auto& it : m_freeDatabases)
  {
    total += (unsigned)it.second->size();
  }
  return total;
}

// List with current connections (meant for logging purposes only)
void
SQLDatabasePool::GetListOfConnections(XString& p_list)
{
  // Lock the pool
  AutoCritSec lock(&m_lock);
  XString text;

  // Clean the list up front
  p_list.Empty();

  // All databases with an active connection
  for(auto& it : m_allDatabases)
  {
    text.Format("Database pool - connection to    : %s\n",it.first.GetString());
    p_list += text;
  }
  // If so, reflect the empty status
  if(p_list.IsEmpty())
  {
    p_list = "Database pool - ODBC connections : No connections\n";
  }
}

// Register our pool for generic logging
void
SQLDatabasePool::RegisterLogContext(int p_level,LOGLEVEL p_loglevel,LOGPRINT p_logprinter,void* p_logContext)
{
  m_loggingLevel = p_level;
  m_logLevel     = p_loglevel;
  m_logPrinter   = p_logprinter;
  m_logContext   = p_logContext;
}

// Add a column rebind for this database session: No bounds checking!
void
SQLDatabasePool::AddColumnRebind(int p_sqlType,int p_cppType)
{
  m_rebindColumns[p_sqlType] = p_cppType;
}

// Add a parameter rebind for this database session: No bounds checking!
void
SQLDatabasePool::AddParameterRebind(int p_sqlType,int p_cppType)
{
  m_rebindParameters[p_sqlType] = p_cppType;
}

//////////////////////////////////////////////////////////////////////////
//
// PRIVATE
// NONE of these functions has an auto lock, must be already on the stack!
//
//////////////////////////////////////////////////////////////////////////

// Get OR make a logged in database connection
SQLDatabase*
SQLDatabasePool::GetDatabaseInternally(DbsPool& p_pool,XString& p_connectionName)
{
  DbsPool::iterator it;
  unsigned retry = CONN_RETRIES;

  while(retry)
  {
    // See if there is a free database for this DSN
    it = p_pool.find(p_connectionName);
    if(it == p_pool.end())
    {
      if(m_openConnections < m_maxDatabases)
      {
        return MakeDatabase(p_connectionName);
      }
      // If max-databases is reached, we enter the 'retry' loop
      if(retry == CONN_RETRIES)
      {
        // First time here: try to clean up some databases
        // and retry to find a free database entry
        Cleanup(true);
      }
      else
      {
        // Wait 1 second if any database becomes free
        LeaveCriticalSection(&m_lock);
        Sleep(1000);
        EnterCriticalSection(&m_lock);
      }
      if(--retry == 0)
      {
        XString error;
        error.Format("The maximum number of open databases has been reached [%d]",m_maxDatabases);
        LogPrint(error);
        throw StdException(error);
      }
      // Just try again
      continue;
    }
    else
    {
      // Done, this is the pool list we will use
      break;
    }
  }

  // Gets the last database of this list
  // It is the last returned database from the program
  // so its the most recent used.
  // The front of the list is older, and will be cleaned by "Cleanup()"
  DbsList* list = it->second;
  if(list->empty())
  {
    p_pool.erase(it);
    delete list;
    return MakeDatabase(p_connectionName);
  }
  SQLDatabase* dbs = list->back();
  list->pop_back();

  // If the list is now empty, remove the list
  if(list->empty())
  {
    p_pool.erase(it);
    delete list;
    list = NULL;
  }

  // Extra internal check that we really do have a database!
  // AutoDBS must have a real database object
  if(dbs == nullptr)
  {
    XString error("INTERN: No database found in the list with free databases");
    LogPrint(error);
    throw StdException(error);
  }

  // Probably still to open: can happen if 'database.xml' has been changed
  if(dbs->IsOpen() == false)
  {
    OpenDatabase(dbs,p_connectionName);
  }

  // This is the database to use
  return dbs;
}

// Return a connection to the pool
void
SQLDatabasePool::GiveUpInternally(SQLDatabase* p_database,XString& p_connectionName)
{
  // Last time we had an action on this database
  p_database->SetLastActionTime();

  // Find the list of free databases
  DbsPool::iterator it = m_freeDatabases.find(p_connectionName);
  if(it == m_freeDatabases.end())
  {
    // Create a new free list
    DbsList* list = new DbsList();
    list->push_back(p_database);
    m_freeDatabases.insert(std::make_pair(p_connectionName,list));
    return;
  }
  // Simply place in this list
  DbsList* list = it->second;
  list->push_back(p_database);
}

// Cleanup in a list of databases
void
SQLDatabasePool::CleanupInternally(bool p_aggressive)
{
  DbsPool::iterator it = m_freeDatabases.begin();
  while(it != m_freeDatabases.end())
  {
    // For all lists of databases
    DbsList* list = it->second;

    while(list->size())
    {
      // Start at the oldest side of the queue
      SQLDatabase* db = list->front();

      // Can the database be cleaned out?
      if(db->PastWaitingTime() || p_aggressive)
      {
        XString name = db->GetConnectionName();

        // Close the database en remove it from the list of free databases
        db->Close();
        XString text;
        text.Format("Closed database connection for [%s/%s]",name.GetString(),db->GetUserName().GetString());
        LogPrint(text);
        list->pop_front();

        // Reduce counter
        --m_openConnections;

        // Zoek in de lijst met *alle* databases
        // Find it in the list of *all* databases
        DbsPool::iterator lit = m_allDatabases.find(name);
        if(lit != m_allDatabases.end())
        {
          // Remove the database
          DbsList* all = lit->second;
          if(all)
          {
            DbsList::iterator dbl = std::find(all->begin(),all->end(),db);
            if(dbl != all->end())
            {
              all->erase(dbl);
            }
            // Optionally remove the whole list, if it was the last one
            if(all->empty())
            {
              delete all;
              m_allDatabases.erase(lit);
            }
          }
        }
        // Now destruct the database object
        delete db;
      }
      else
      {
        // From the front of the queue, we will now never
        // find a database that exceeds the waiting limit
        // because these are pushed back later
        break;
      }
    }
    // Optionally remove the complete list
    if(list->empty())
    {
      delete list;
      it = m_freeDatabases.erase(it);
    }
    else
    {
      ++it;
    }
  }
}

// Create a new database object
SQLDatabase*
SQLDatabasePool::MakeDatabase(XString p_connectionName)
{
  // Create the database
  SQLDatabase* dbs = new SQLDatabase();
  dbs->SetConnectionName(p_connectionName);

  // Preset logging: derive it from the database pool
  if(m_logPrinter)
  {
    dbs->RegisterLogContext(m_loggingLevel,m_logLevel,m_logPrinter,m_logContext);
  }

  // Place in the list for all databases
  // Gets its own scope for a temporary lock
  {
    AutoCritSec lock(&m_lock);

    // Place in the list of all databases
    DbsPool::iterator it = m_allDatabases.find(p_connectionName);
    if(it == m_allDatabases.end())
    {
      DbsList* list = new DbsList();
      list->push_back(dbs);
      m_allDatabases.insert(std::make_pair(p_connectionName,list));
    }
    else
    {
      // Simply adds to this list
      it->second->push_back(dbs);
      // #ifdef DEBUG
      //       // In debug mode we can test whether we program the databases serial
      //       // if a datasource gets more than 1 connection, we can generate an error report
      //       // so that we can find that point by following this stack trace and do a repair
      //       if(it->second->size() > 1)
      //       {
      //         char* crashMe = nullptr;
      //         strcpy_s(crashMe,2000,"Shit: you have multiple databases running");
      //       }
      // #endif
    }
  }
  OpenDatabase(dbs,p_connectionName);
  return dbs;
}

// Open the connection to the RDBMS server
void
SQLDatabasePool::OpenDatabase(SQLDatabase* p_dbs,XString& p_connectionName)
{
  // Find the database connection definition
  SQLConnection* conn = m_connections.GetConnection(p_connectionName);

  if(conn == nullptr)
  {
    // File 'database.xml' could be changed **AFTER** starting the program
    // So try to read it again, and see if the datasource is really not there
    ReadConnections();
    conn = m_connections.GetConnection(p_connectionName);
  }
  if(conn)
  {
    // Try to open the database right away
    XString connString = m_connections.GetConnectionString(p_connectionName);
    p_dbs->Open(connString);
    p_dbs->SetDatasource(conn->m_datasource);
    p_dbs->SetUserName(conn->m_username);

    // Tell it what the standard rebind info is (if any)
    // Can only be done after the open (when SQLDatabase has cleared and set it' rebound info)
    AddRebindsToDatabase(p_dbs);
    
    // Tell it the logfile
    XString text;
    text.Format("Database created and opened: [%s:%s]"
               ,conn->m_datasource.GetString()
               ,conn->m_username.GetString());
    LogPrint(text);

    // One extra!
    ++m_openConnections;
  }
  else
  {
    XString error;
    error.Format("Database [%s] selected, but no connection found in 'database.xml'",p_connectionName.GetString());
    LogPrint(error);
    throw StdException(error);
  }
}

// Close all databases in the list an clean up the list
void
SQLDatabasePool::CloseAllInternally()
{
  // Walk through the total pool
  for(auto& pool : m_allDatabases)
  {
    DbsList* dblist = pool.second;

    // Loop through a database list
    for(auto& database : *dblist)
    {
      // Report the closing
      XString text;
      text.Format("Database [%s] connection closed. User: %s"
                 ,database->GetConnectionName().GetString()
                 ,database->GetUserName().GetString());
      LogPrint(text);

      // Close the database connection to the RDBMS server 
      database->Close();

      // Reduce open counter
      --m_openConnections;

      // Destruct the database object
      delete database;
    }

    // Clean out the list
    dblist->clear();
    delete dblist;
  }
  // Clean out the complete pool
  m_allDatabases.clear();
}

// Clean up the free lists
void
SQLDatabasePool::CleanupAllInternally()
{
  // Cleanup all lists of free databases.
  // They do **NOT** own the database objects
  for(auto& it : m_freeDatabases)
  {
    DbsList* list = it.second;
    delete list;
  }
  m_freeDatabases.clear();
}

//////////////////////////////////////////////////////////////////////////
//
// LOGGING SUPPORT
//
//////////////////////////////////////////////////////////////////////////

// Support printing to generic logfile
void
SQLDatabasePool::LogPrint(const char* p_text)
{
  // If the loglevel is above the activation level
  if(m_loggingLevel >= m_logActive)
  {
    // If we have both the printing routine AND the context
    if(m_logPrinter && m_logContext)
    {
      // Print to the logfile
      (*m_logPrinter)(m_logContext,p_text);
    }
  }
}

// Getting the current loglevel
int
SQLDatabasePool::LogLevel()
{
  if(m_logContext)
  {
    m_loggingLevel = (*m_logLevel)(m_logContext);
    return m_loggingLevel;
  }
  return -1;
}

bool
SQLDatabasePool::WilLog()
{
  // If we have both the loglevel routine AND the context
  if(m_logLevel && m_logContext)
  {
    // Refresh the loglevel
    m_loggingLevel = (*m_logLevel)(m_logContext);
    // True if at logactive threshold or above
    if(m_loggingLevel >= m_logActive)
    {
      return true;
    }
  }
  return false;
}

// Add our rebind mappings to a newly opened database
void
SQLDatabasePool::AddRebindsToDatabase(SQLDatabase* p_database)
{
  // Add all our rebound columns
  for(auto& rebind : m_rebindColumns)
  {
    p_database->AddColumnRebind(rebind.first,rebind.second);
  }
  // Add all our rebound parameters
  for(auto& rebind : m_rebindParameters)
  {
    p_database->AddParameterRebind(rebind.first,rebind.second);
  }
}

}