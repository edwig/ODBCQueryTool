////////////////////////////////////////////////////////////////////////
//
// File: SQLMigrate.cpp
//
// Written by: ir. W.E. Huisman between 1998-2025 
// MIT License
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
#include "SQLMigrate.h"
#include "SQLMigrateApp.h"
#include "SQLMigrateDialog.h"
#include "SQLQuery.h"
#include "SQLInfoDB.h"
#include <sql.h>
#include <sqlext.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{

SQLMigrate::SQLMigrate(MigrateParameters& p_params,Logging& m_log)
           :m_params(p_params)
           ,m_log(m_log)
{
}

SQLMigrate::~SQLMigrate()
{
  if(m_databaseSource)
  {
    delete m_databaseSource;
    m_databaseSource = nullptr;
  }
  if(m_databaseTarget)
  {
    delete m_databaseTarget;
    m_databaseTarget = nullptr;
  }
}

bool 
SQLMigrate::Migrate()
{
  XString text;
  bool result = false;

  // Getting the starting moment
  m_start = clock();

  // Check the input parameters first.
  // Will throw StdException if parameters are incorrect and/or non-conforming
  CheckMigrateParameters();

  // Open the log files
  // Show all migration parameters in the logfile
  WriteMigrateParameters();

  // Prepare for migration
  m_totalTables     = 0;
  m_directMigration = m_params.v_direct;

  try
  {
    // Try to connect to both source and target databases
    if(!MakeDatabaseConnections())
    {
      return false;
    }

    // Find the number of tables to migrate (and which tables)
    if(FindTablesToMigrate() == 0)
    {
      // Check if nothing to do
      text = _T("No tables to migrate");
      m_log.WriteLog(text);
      AfxMessageBox(text,MB_OK|MB_ICONERROR);
    }
    else
    {
      if(m_params.v_do_tables)
      {
        DropTables();
      }

      if(m_params.v_do_tables || m_params.v_primarys  || m_params.v_indices   ||
         m_params.v_foreigns  || m_params.v_sequences || m_params.v_triggers  ||
         m_params.v_access    )
      {
        CreateTables();
      }
      
      if(m_params.v_do_views)
      {
        CreateViews();
      }

      // If minimum object-ID requested: do not truncate the contents
      // CXHibernate and network databases uses a "OID" primary key
      if(m_params.v_deletes && m_params.v_where.IsEmpty())
      {
        TruncateTables();
      }
      if(m_params.v_do_data)
      {
        switch(m_directMigration)
        {
          case MigrateType::DataPump:     // HIGH PERFORMANCE DATAPUMP 
                                          FillTablesViaPump(); 
                                          break;
          case MigrateType::SlowDataPump: // LOW PERFORMANCE DATAPUMP for broken drivers
                                          FillTablesViaSlowPump();
                                          break;
          case MigrateType::SelectInsert: // Fill database via SELECT/INSERT (Works always, but slower)
                                          FillTablesViaData(true);
                                          break;
          case MigrateType::SQLScripts:   // Fill Script files. Migrate later by running these scripts
                                          FillTablesViaData(false);
                                          break;
        }
      }
    }
    // Reached the end
    result = true;
  }
  catch(StdException& ex)
  {
    m_log.WriteLog(_T("ERROR in migration: ") + ex.GetErrorMessage());
  }
  catch(XString& ex)
  {
    m_log.WriteLog(_T("ERROR in migration: ") + ex);
  }
  catch(...)
  {
    m_log.WriteLog(_T("Unknown ERROR in migration"));
  }

  // Ultimate cleanup
  CleanUpMigration();

  // THE END!
  m_log.WriteLog(_T("*** End of migration. ***"));
  return result;
}

//////////////////////////////////////////////////////////////
//
// Check parameters. Can throw!
// Here are all the checks on the logic sanity of all
// filled in options. Does everything belong together?
// In the mean time we build the logfile header
//
//////////////////////////////////////////////////////////////

void
SQLMigrate::CheckMigrateParameters()
{
  XString extra;
  if(!m_params.v_table.IsEmpty())
  {
    extra = _T("Tables_") + m_params.v_table + _T("_");
    extra.Remove('%');
    extra.Remove('*');
    extra.Remove('?');
  }

  XString create   = m_params.v_directory + _T("\\") + extra + _T("Create_")  + m_params.v_createscript;
  XString dropping = m_params.v_directory + _T("\\") + extra + _T("Drop_")    + m_params.v_dropscript;
  XString logging  = m_params.v_directory + _T("\\") + extra + _T("Logfile_") + m_params.v_target_schema + _T(".txt");

  m_log.SetScript(create);
  m_log.SetDropScript(dropping);
  m_log.SetLogfile(logging);

  if (!m_log.Open(m_params.v_direct == MigrateType::SQLScripts))
  {
    throw StdException(_T("Cannot open a *.sql or *.txt file. File is read-only or no space left on file system!"));
  }

  // check source database 
  if (m_params.v_source_dsn ==_T(""))
  {
    throw StdException(_T("No source database found. Enter a source database DataSourceName (DSN)"));
  }
  if (m_params.v_source_user ==_T(""))
  {
    throw StdException(_T("No source user found for the source database. Enter a source user name"));
  }

  if (m_params.v_source_password ==_T(""))
  {
    throw StdException(_T("No source user password found. Enter a source password"));
  }


  if(m_params.v_direct == MigrateType::DataPump     || 
     m_params.v_direct == MigrateType::SlowDataPump ||
     m_params.v_direct == MigrateType::SelectInsert ) 
  {  
    // Check target database
    if(m_params.v_target_dsn ==_T(""))
    {
      throw StdException(_T("No target database found. Enter a target database DataSourceName (DSN)"));
    }
    // Check target user
    if (m_params.v_target_user ==_T(""))
    {
      throw StdException(_T("No target database user found. Enter a target database user"));
    }
    // Check target password
    if (m_params.v_target_password ==_T(""))
    {
      throw StdException(_T("No target user password found. Enter a target password"));
    }
  }

  if(m_params.v_direct == MigrateType::SQLScripts)
  {
    if ( m_params.v_dropscript ==_T(""))
    {
      throw StdException(_T("No file name for the dropscript given. Enter a file name..."));
    }
    if (m_params.v_createscript == _T(""))
    {
      throw StdException(_T("No file name for a create-script given. Enter a file name..."));
    }
  }
  if(m_params.v_allObjects == 0 && m_params.v_table == _T(""))
  {
    throw StdException(_T("No tablename given for a table migration. Enter a table name or table pattern..."));
  }

  // Check value of logging amount per rows
  if(m_params.v_logLines)
  {
    if(m_params.v_logLines < 10 || m_params.v_logLines > 100000)
    {
      throw StdException(_T("Number of rows per line in the logfile in an invalid range.\n")
                         _T("Fill in a number between 10 and 100.000"));
    }
  }

  // Check for views run
  if(m_params.v_do_views)
  {
    if(m_params.v_do_tables + m_params.v_do_data   +
       m_params.v_primarys  + m_params.v_indices   + 
       m_params.v_foreigns  + m_params.v_sequences + 
       m_params.v_triggers  + m_params.v_access    +
       m_params.v_deletes   != 0)
    {
      throw StdException(_T("Cannot convert anything other than views, if views are selected\n"));
      return;
    }
  }

  // Check if we have something to do!
  if(m_params.v_do_tables + m_params.v_do_data   +
     m_params.v_primarys  + m_params.v_indices   + 
     m_params.v_foreigns  + m_params.v_sequences + 
     m_params.v_triggers  + m_params.v_access    +
     m_params.v_deletes   + m_params.v_do_views  == 0)
  {
    throw StdException(_T("Nothing will be migrated because all options are in the OFF position.\n"));
    return;
  }
  // Hurray: Reached the end of all checks
  // We are good to go!
}

void
SQLMigrate::WriteMigrateParameters()
{
  m_log.WriteLog(_T("MIGRATION"));
  // Extra empty line
  m_log.WriteLog(_T(" "));

  // check sourace database
  if(!m_params.v_source_dsn.IsEmpty())
  {
    // Ruler       "------------------- : "
    m_log.WriteLog(_T("Source database     : ") + m_params.v_source_dsn);
  }
  if(!m_params.v_source_user.IsEmpty())
  {
    // Ruler       "------------------- : "
    m_log.WriteLog(_T("Source user name    : ") + m_params.v_source_user);
  }
  m_log.WriteLog(_T(" "));


  if(m_params.v_direct == MigrateType::DataPump     || 
     m_params.v_direct == MigrateType::SlowDataPump ||
     m_params.v_direct == MigrateType::SelectInsert )
  {
    // Check target database
    if(!m_params.v_target_dsn.IsEmpty())
    {
      // Ruler       "------------------- : "
      m_log.WriteLog(_T("Target database     : ") + m_params.v_target_dsn);
    }
    // Check target user
    if(!m_params.v_target_user.IsEmpty())
    {
      // Ruler       "------------------- : "
      m_log.WriteLog(_T("Target user name    : ") + m_params.v_target_user);
    }
  }
  m_log.WriteLog(_T(""));

  if(m_params.v_direct == MigrateType::SQLScripts)
  {
    // Ruler       "-------------------- : "
    m_log.WriteLog(_T("Direct migration     : No"));
    if(!m_params.v_dropscript.IsEmpty())
    {
      // Ruler       "------------------- : "
      m_log.WriteLog(_T("Dropscript          : ") + m_params.v_dropscript);
    }
    if(!m_params.v_createscript.IsEmpty())
    {
      // Ruler       "------------------ : "
      m_log.WriteLog(_T("Create script      : ") + m_params.v_createscript);
    }
  }
  else
  {
    // Ruler               "------------------- : "
    switch(m_params.v_direct)
    {
      case MigrateType::DataPump:     m_log.WriteLog(_T("Direct migration    : FAST DATAPUMP"));       break;
      case MigrateType::SlowDataPump: m_log.WriteLog(_T("Direct migration    : SAFE DATAPUMP"));  break;
      case MigrateType::SelectInsert: m_log.WriteLog(_T("Direct migration    : SELECT/INSERT"));  break;
      case MigrateType::SQLScripts:   m_log.WriteLog(_T("Direct migration    : SQL-Scripts"));    break;
    }
  }
  if(!(m_params.v_allObjects == 0 && m_params.v_table == _T("")))
  {
    if(!m_params.v_table.IsEmpty())
    {
      // Ruler       "------------------- : "
      m_log.WriteLog(_T("Tables              : All"));
    }
    else
    {
      // Ruler       "------------------- : "
      m_log.WriteLog(_T("Table(s)            : ") + m_params.v_table);
    }
  }
  // Ruler       "------------------- : "
  m_log.WriteLog(_T("Tablespace          : ") + m_params.v_tablespace);


  if(!m_params.v_where.IsEmpty())
  {
    // Ruler       "------------------- : "
    m_log.WriteLog(_T("Selection clause    : ") + m_params.v_where);
  }

  // Logging per number of rows converted
  if(m_params.v_logLines > 0)
  {
    XString num;
    num.Format(_T("%d"),m_params.v_logLines);
    // Ruler       "------------------- : "
    m_log.WriteLog(_T("Log line after rows : ") + num);
  }

  // Logging of all migration options
  // Ruler               "------------------- : "
  m_log.WriteLog(XString(_T("Create new tables   : ")) + (m_params.v_do_tables ? _T("yes") : _T("no")));
  m_log.WriteLog(XString(_T("Delete  table data  : ")) + (m_params.v_deletes   ? _T("yes") : _T("no")));
  m_log.WriteLog(XString(_T("Convert table data  : ")) + (m_params.v_do_data   ? _T("yes") : _T("no")));
  m_log.WriteLog(XString(_T("Convert views       : ")) + (m_params.v_do_views  ? _T("yes") : _T("no")));
  m_log.WriteLog(XString(_T("Truncate char fields: ")) + (m_params.v_truncate  ? _T("yes") : _T("no")));
  m_log.WriteLog(XString(_T("Create new indices  : ")) + (m_params.v_indices   ? _T("yes") : _T("no")));
  m_log.WriteLog(XString(_T("Create primary keys : ")) + (m_params.v_primarys  ? _T("yes") : _T("no")));
  m_log.WriteLog(XString(_T("Create foreign keys : ")) + (m_params.v_foreigns  ? _T("yes") : _T("no")));
  m_log.WriteLog(XString(_T("Create sequences    : ")) + (m_params.v_sequences ? _T("yes") : _T("no")));
  m_log.WriteLog(XString(_T("Create triggers     : ")) + (m_params.v_triggers  ? _T("yes") : _T("no")));
  m_log.WriteLog(XString(_T("Grant access rights : ")) + (m_params.v_access    ? _T("yes") : _T("no")));
  m_log.WriteLog(_T(""));
}

//////////////////////////////////////////////////////////////////////////
//
// MAKING CONNECTIONS TO THE DATABASES
//
//////////////////////////////////////////////////////////////////////////

bool
SQLMigrate::MakeDatabaseConnections()
{
  CString text;

  if(!m_databaseSource)  m_databaseSource = new SQLDatabase();
  if(!m_databaseTarget)  m_databaseTarget = new SQLDatabase();

  XString connectSource;
  XString connectTarget;
  connectSource.Format(_T("DSN=%s;UID=%s;PWD=%s;"),m_params.v_source_dsn.GetString(),m_params.v_source_user.GetString(),m_params.v_source_password.GetString());
  connectTarget.Format(_T("DSN=%s;UID=%s;PWD=%s;"),m_params.v_target_dsn.GetString(),m_params.v_target_user.GetString(),m_params.v_target_password.GetString());

  // Always connect to the source as a minimum
  //             "--------------------: "
  m_log.WriteLog(_T("Make connection with: ") + m_params.v_source_dsn);
  m_log.WriteLog(_T("Logged in user      : ") + m_params.v_source_user);
  if(m_databaseSource->Open(connectSource,true) == false)  // Read-only connect
  {
    text.Format(_T("Cannot make connection with: ") + m_params.v_source_dsn);
    m_log.WriteLog(text);
    AfxMessageBox (text,MB_OK|MB_ICONERROR);
    return false;
  }
  m_log.SetDBType(true,m_databaseSource->GetDatabaseTypeName());
  m_params.v_sourceType = m_databaseSource->GetDatabaseType();

  // And always make connection to the target database (also when not directly converting)
  // This is necessary to read in the target database capabilities
  m_log.WriteLog(_T("Make connection with: ") + m_params.v_target_dsn);
  m_log.WriteLog(_T("Logged in user      : ") + m_params.v_target_user);
  if(m_databaseTarget->Open(connectTarget) == false)
  {
    text.Format(_T("Cannot make connection with: ") + m_params.v_target_dsn);
    m_log.WriteLog(text);
    AfxMessageBox (text,MB_OK|MB_ICONERROR);
    return false;
  }

  // Try to use the target schema
  bool defschema = m_databaseTarget->SetDefaultSchema(m_params.v_target_user,m_params.v_target_schema);
  m_log.WriteLog(XString(_T("Connect to schema   : ")) + (defschema ? m_params.v_target_schema : XString(_T("NO SCHEMA!!"))));

  m_log.SetDBType(false,m_databaseTarget->GetDatabaseTypeName());
  m_params.v_targetType  = m_databaseTarget->GetDatabaseType();
  m_log.WriteLog(_T(""));

  return true;
}

//////////////////////////////////////////////////////////////////////////
//
// READING THE TABLES TO CONVERT
//
//////////////////////////////////////////////////////////////////////////

int
SQLMigrate::FindTablesToMigrate()
{
  // Find the number of tables to migrate (and which tables)
  m_log.WriteLog(_T("Tables to migrate   : ") + m_params.v_table);
  m_totalTables = ReadTableStructures(m_params.v_source_user, m_params.v_table, m_databaseSource);
  XString num;
  num.Format(_T("%d"), m_totalTables);
  m_log.WriteLog(_T("Tables to migrate   : ") + num);

  // Register the number of tables for the gauges
  m_log.SetTables(m_totalTables);

  return m_totalTables;
}

int
SQLMigrate::ReadTableStructures(XString p_owner,XString p_pattern,SQLDatabase* p_database)
{
  XString errors;
  XString catalog;

  // Set status line of current step
  m_log.SetStatus(_T("Read source object structures"));

  p_database->GetSQLInfoDB()->SetPreferODBC(false);

  if(p_pattern.IsEmpty())
  {
    if(m_params.v_do_views)
    {
      m_log.WriteLog(_T("To migrate          : ALL VIEWS"));
      p_database->GetSQLInfoDB()->MakeInfoTableView(m_tables,errors,catalog,p_owner,p_pattern);
    }
    else
    {
      m_log.WriteLog(_T("To migrate          : ALL TABLES"));
      p_database->GetSQLInfoDB()->MakeInfoTableTable(m_tables,errors,catalog,p_owner,p_pattern);
      RemoveTemporaries();
    }
  }
  else if(p_pattern.Right(1) == _T("%"))
  {
    if(m_params.v_do_views)
    {
      m_log.WriteLog(_T("To migrate          : Selection from source database"));
      p_database->GetSQLInfoDB()->MakeInfoTableView(m_tables,errors,catalog,p_owner,p_pattern);
    }
    else
    {
      m_log.WriteLog(_T("To migrate          : Selection from source database"));
      p_database->GetSQLInfoDB()->MakeInfoTableTable(m_tables,errors,catalog,p_owner,p_pattern);
      RemoveTemporaries();
    }
  }
  else if(p_pattern.Right(4).CompareNoCase(_T(".txt")) == 0)
  {
    m_log.WriteLog(_T("To migrate          : Selection from text file"));
    ReadTablesFromFile(p_pattern);
  }
  else
  {
    // If all else fails: It MUST be a table name
    m_log.WriteLog(_T("One table with name : ") + p_pattern);
    MetaTable table;
    table.m_schema = p_owner;
    table.m_table = p_pattern;
    m_tables.push_back(table);
  }
  return (int) m_tables.size();
}

// Reading the tables to migrate from a file makes it possible to 
// determine our own order of creation, independent from the sorting
// order of the ODBC driver
void
SQLMigrate::ReadTablesFromFile(XString& p_file)
{
  TCHAR buffer[101];
  FILE* file = nullptr;
  _tfopen_s(&file,p_file,_T("r"));
  if(file)
  {
    while(_fgetts(buffer,100,file))
    {
      int len = (int) _tcslen(buffer);
      if(len && buffer[len-1] == '\n')
      {
        buffer[len-1] = 0;
      }
      MetaTable table;
      table.m_table = buffer;
      m_tables.push_back(table);
    }
    fclose(file);
  }
  else
  {
    XString msg;
    msg.Format(_T("Cannot open the text file with table names: %s"),p_file.GetString());
    AfxMessageBox(msg,MB_OK|MB_ICONERROR);
  }
}

void
SQLMigrate::RemoveTemporaries()
{
  // If target database supports global temporary tables
  // then we leave the temporaries in the data set
  if(m_params.v_targetType == DatabaseType::RDBMS_ORACLE   ||
     m_params.v_targetType == DatabaseType::RDBMS_FIREBIRD ||
     m_params.v_targetType == DatabaseType::RDBMS_SQLSERVER )
  {
    return;
  }

  // Remove temp tables
  MTableMap::iterator it = m_tables.begin();
  while(it != m_tables.end())
  {
    if(it->m_temporary)
    {
      m_log.WriteLog(XString(_T("Temporary not done  : ")) + it->m_table);
      it = m_tables.erase(it);
      continue;
    }
    // Next table
    ++it;
  }
}

// After all is 'said and done'
void
SQLMigrate::CleanUpMigration()
{
  // Closing the databases again
  m_databaseSource->Close();
  m_databaseTarget->Close();

  // Clear tables list
  m_tables.clear();

  // Mark end in scripts (so we know we reached the end)
  if(m_directMigration == MigrateType::SQLScripts)
  {
    m_log.WriteDrop(_T("-- End of script"));
    m_log.WriteOut (_T("-- End of script"));
  }

  // Result of the migration
  XString text;
  text.Format(_T("Result of the migration: %d Errors."),m_params.v_errors);
  m_log.WriteLog(_T(""));
  m_log.WriteLog(text);

  // Report the running time
  long miliseconds = clock() - m_start;
  text.Format(_T("Total running time = %d:%d.%03d")
             ,(miliseconds / CLOCKS_PER_SEC) / 60
             ,(miliseconds / CLOCKS_PER_SEC) % 60
             ,(miliseconds % CLOCKS_PER_SEC));
  m_log.WriteLog(text);
}

//////////////////////////////////////////////////////////////////////////
//
// DROP   THE TARGET TABLES in the TARGET DATABASE
// CREATE THE TARGET TABLES in the TARGET DATABASE
//
//////////////////////////////////////////////////////////////////////////

void
SQLMigrate::DropTables()
{
  int numTables = 0;

  XString comment(_T("-- "));
  XString header1(_T("DROPPING THE TABLES IN THE TARGET DATABASE"));
  XString header2(_T("=========================================="));

  m_log.WriteLog(_T(""));
  m_log.WriteLog(header1);
  m_log.WriteLog(header2);

  if(m_directMigration == MigrateType::SQLScripts)
  {
    m_log.WriteDrop(comment + header1);
    m_log.WriteDrop(comment + header2);
    m_log.WriteDrop(_T(""));
  }

  // Set status for current step
  XString status;
  status.Format(_T("Dropping %d tables"),(int) m_tables.size());
  m_log.SetStatus(status);

  for(unsigned int ind = 0; ind < m_tables.size(); ++ind)
  {
    XString table     = m_tables[ind].m_table;
    XString statement = m_databaseTarget->GetSQLInfoDB()->GetCATALOGTableDrop(m_params.v_target_schema,table,true,true,false);

    // Process the statement
    m_log.WriteLog(statement);
    if(m_directMigration == MigrateType::DataPump     ||
       m_directMigration == MigrateType::SlowDataPump ||
       m_directMigration == MigrateType::SelectInsert )
    {
      try
      {
        SQLQuery query(m_databaseTarget);
        SQLTransaction trans(m_databaseTarget,_T("Drop"));
        query.DoSQLStatement(statement);
        trans.Commit();
        CommitDDL(query,m_databaseTarget->GetSQLInfoDB());
      }
      catch(StdException& ex)
      {
        m_log.WriteLog(_T("ERROR : Dropping of table failed."));
        m_log.WriteLog(ex.GetErrorMessage());
        ++m_params.v_errors;
      }
    }
    else
    {
      m_log.WriteDrop(statement,true);
    }
    // Record number of tables in gouge
    m_log.SetTablesGauge(++numTables);
  }
}

void
SQLMigrate::CreateTables()
{
  int numTables = 0;

  XString comment(_T("-- "));
  XString header1(_T("CREATING TABLES IN THE TARGET DATABASE"));
  XString header2(_T("======================================"));

  m_log.WriteLog(_T(""));
  m_log.WriteLog(header1);
  m_log.WriteLog(header2);

  if(m_directMigration == MigrateType::SQLScripts)
  {
    m_log.WriteOut(comment + header1);
    m_log.WriteOut(comment + header2);
    m_log.WriteOut(_T(""));
  }

  // Set status of current step
  XString status;
  status.Format(_T("Creating %d tables"),(int) m_tables.size());
  m_log.SetStatus(status);

  // Try to get the optimal result
  SQLInfoDB* source = m_databaseSource->GetSQLInfoDB();
  SQLInfoDB* target = m_databaseTarget->GetSQLInfoDB();
  source->SetPreferODBC(true);
  target->SetPreferODBC(true);

  for(unsigned int ind = 0; ind < m_tables.size(); ++ind)
  {
    DDLCreateTable create(source,target);

    // Getting the table to migrate
    XString tablename = m_tables[ind].m_table;
    if (!m_params.v_source_schema.IsEmpty() && source->GetRDBMSUnderstandsSchemas())
    {
      tablename = m_params.v_source_schema + _T(".") + tablename;
    }

    // If we migrate to another type, PSM cannot be converted
    if(m_params.v_triggers && (m_databaseSource->GetDatabaseType() != m_databaseTarget->GetDatabaseType()))
    {
      m_params.v_triggers = false;
      m_log.WriteLog(_T("ERROR: Cannot convert triggers to another database type!"));
    }

    // Getting the table info (and throw away the results!)
    create.GetTableStatements(tablename
                             ,m_params.v_do_tables || m_params.v_do_data
                             ,m_params.v_do_tables
                             ,m_params.v_indices
                             ,m_params.v_primarys
                             ,m_params.v_foreigns
                             ,m_params.v_triggers
                             ,m_params.v_sequences
                             ,m_params.v_access);

    // Set the target database
    create.SetInfoDB(target);

    // Create target database table name
    XString targettable = m_tables[ind].m_table;
    if(!m_params.v_target_schema.IsEmpty() && target->GetRDBMSUnderstandsSchemas())
    {
      targettable = m_params.v_target_schema + _T(".") + targettable;
    }

    // Tweak the columns from source to target database !!
    if(source->GetRDBMSDatabaseType() != target->GetRDBMSDatabaseType())
    {
      OrderTableColumns(create);
      FixupTableColumns(create);
      FixupTableIndices(create);
    }

    // Check for special options
    if(source->GetRDBMSDatabaseType() == DatabaseType::RDBMS_ORACLE &&
       target->GetRDBMSDatabaseType() == DatabaseType::RDBMS_SQLSERVER)
    {
      create.SetOptionIndexDuplicateNulls(true);
    }

    // Set the schema/tablespace
    create.SetTablesSchema   (m_params.v_target_schema);
    create.SetTableTablespace(m_params.v_tablespace);
    create.SetIndexTablespace(m_params.v_tablespace);

    // Re-Create the table in the target database type
    target->SetPreferODBC(true);
    DDLS statements = create.GetTableStatements(targettable);

    // Now hold on the first statement, if no re-create tables
    if(m_params.v_do_tables == false)
    {
      statements.pop_front();
    }

    // Do something with the statement
    for(auto& statement : statements)
    {
      m_log.WriteLog(statement);
      if(m_directMigration == MigrateType::DataPump     ||
         m_directMigration == MigrateType::SlowDataPump ||
         m_directMigration == MigrateType::SelectInsert )
      {
        try
        {
          SQLQuery query(m_databaseTarget);
          SQLTransaction trans(m_databaseTarget,_T("CreateTable"));
          query.DoSQLStatement(statement);
          trans.Commit();
        }
        catch(StdException& ex)
        {
          m_log.WriteLog(_T("ERROR: Table creation failed"));
          m_log.WriteLog(ex.GetErrorMessage());
          m_params.v_errors++;
        }
      }
      else
      {
        statement.Replace(_T("\r"),_T(""));
        m_log.WriteOut(statement,true);
      }
    }
    // Show number of tables
    m_log.SetTablesGauge(++numTables);
  }
}

// Hard coded special columns for Pronto MDA applications
const TCHAR* specials[]
{
   _T("oid")
  ,_T("object")
  ,_T("av_oid")
  ,_T("h_trans_van")
  ,_T("h_geldig_van")
  ,_T("h_geldig_tot")
  ,_T("h_gebruiker")
  ,_T("h_actief")
  ,_T("naam")
  ,_T("soort")
};

void
SQLMigrate::OrderTableColumns(DDLCreateTable& p_create)
{
  // Quick check if we should order all columns
  if(FindColumn(p_create.m_columns,_T("oid")) < 0)
  {
    return;
  }

  // Order with largest columns on the end (CLOB's etc)
  MColumnMap sorted = SortColumnsBySize(p_create.m_columns);

  // Specials at the front in this order
  MColumnMap columns;
  for(int index = 0; index < ((sizeof specials) / sizeof(const TCHAR*)); ++index)
  {
    int num = FindColumn(sorted,specials[index]);
    if(num >= 0)
    {
      columns.push_back(sorted[num]);
    }
  }

  // Other columns in any order
  for(auto& column : sorted)
  {
    if(FindColumn(columns,column.m_column) < 0)
    {
      columns.push_back(column);
    }
  }

  // Replace
  p_create.m_columns.clear();
  p_create.m_columns = columns;
}

void
SQLMigrate::FixupTableColumns(DDLCreateTable& p_create)
{
  SQLInfoDB* source = m_databaseSource->GetSQLInfoDB();
  SQLInfoDB* target = m_databaseTarget->GetSQLInfoDB();

  for(auto& column : p_create.m_columns)
  {
    // Fix the datatype for the target database
    target->GetKEYWORDDataType(&column);

    // Fix default values
    if(!column.m_default.IsEmpty())
    {
      if(column.m_default.CompareNoCase(source->GetKEYWORDCurrentDate()) == 0)
      {
        column.m_default = target->GetKEYWORDCurrentDate();
      }
      else if(column.m_default.CompareNoCase(source->GetKEYWORDCurrentTimestamp()) == 0)
      {
        column.m_default = target->GetKEYWORDCurrentTimestamp();
      }
      else if(column.m_default.CompareNoCase(source->GetKEYWORDCurrentUser()) == 0)
      {
        column.m_default = target->GetKEYWORDCurrentUser();
      }
      else if(column.m_default.Find('(') > 0)
      {
        // HACK: Functions cannot be converted
        // Incompatible: Make empty, but WARN the user
        XString warning;
        warning.Format(_T("-- WARNING: Default on column [%s] removed: %s"),column.m_column.GetString(),column.m_default.GetString());
        m_log.WriteLog(warning);

        // Must remove the default
        column.m_default.Empty();
      }
    }
  }
}

void
SQLMigrate::FixupTableIndices(DDLCreateTable& p_create)
{
  SQLInfoDB* source = m_databaseSource->GetSQLInfoDB();
  SQLInfoDB* target = m_databaseTarget->GetSQLInfoDB();

  for(auto& index : p_create.m_indices)
  {
    if(target->GetRDBMSSupportsFunctionalIndexes() == false)
    {
      if(!index.m_filter.IsEmpty())
      {
        // AUCH: Source table has a function index, but we do not support it
        //       Try to get the first column name from the index
        //       It's not the same, but for UPPER/LOWER functionals it works!!
        int pos1 = index.m_filter.Find('(');
        int pos2 = index.m_filter.Find(')');
        if(pos1 >= 0 && pos2 >= 0 && pos2 > pos1)
        {
          XString naam = index.m_filter.Mid(pos1+1,(pos2 - pos1 - 1));
          naam = naam.Trim(source->GetIdentifierQuoteCharacter());
          index.m_columnName = naam;

          XString warning;
          warning.Format(_T("-- WARNING: Functional index removed from index [%s] : %s"), index.m_indexName.GetString(),index.m_filter.GetString());
          m_log.WriteLog(warning);

          // Must remove the filter
          index.m_filter.Empty();
        }
      }
    }
  }
}

int
SQLMigrate::FindColumn(MColumnMap& p_columns,XString p_name)
{
  int index = 0;
  for(auto& column : p_columns)
  {
    if(column.m_column.CompareNoCase(p_name) == 0)
    {
      return index;
    }
    ++index;
  }
  return -1;
}

void
SQLMigrate::CreateViews()
{
  int numViews = 0;

  XString text;
  XString comment(_T("-- "));
  XString header1(_T("CREATING VIEWS IN THE TARGET DATABASE"));
  XString header2(_T("====================================="));

  m_log.WriteLog(_T(""));
  m_log.WriteLog(header1);
  m_log.WriteLog(header2);

  // Set status of current step
  XString status;
  status.Format(_T("Creating %d views"),(int)m_tables.size());
  m_log.SetStatus(status);

  // Try to get the optimal result
  SQLInfoDB* source = m_databaseSource->GetSQLInfoDB();
  SQLInfoDB* target = m_databaseTarget->GetSQLInfoDB();
  source->SetPreferODBC(false);
  target->SetPreferODBC(false);

  for(unsigned int ind = 0; ind < m_tables.size(); ++ind)
  {
    DDLCreateTable create(source);

    // Getting the table to migrate
    XString viewName = m_tables[ind].m_table;
    if(!m_params.v_source_schema.IsEmpty() && source->GetRDBMSUnderstandsSchemas())
    {
      viewName = m_params.v_source_schema + _T(".") + viewName;
    }

    // Getting the view info (DO NOT THROW AWAY AS WITH TABLES!)
    // But do not generate RDBMS dependent drop info
    create.SetOptionDropIfExists(false);
    DDLS statements = create.GetViewStatements(viewName);

     // Set the target database
    create.SetInfoDB(target);

    // Do something with the statement
    bool first = true;
    for(auto& statement : statements)
    {
      statement.Replace(_T("OR REPLACE "),_T(""));
      statement.Replace(_T("OR ALTER "),  _T(""));

      // Filter out the schemas
      if(source->GetRDBMSUnderstandsSchemas() && !target->GetRDBMSUnderstandsSchemas())
      {
        XString remove = m_params.v_target_schema + _T(".");
        statement.Replace(remove,_T(""));
      }

      // Handle the DUAL table
      CString sourceSingle = source->GetSQLFromDualClause();
      CString targetSingle = target->GetSQLFromDualClause();

      // Set to the correct casing
      if(target->GetRDBMSIsCatalogUpper())
      {
        statement.MakeUpper();
        sourceSingle.MakeUpper();
        targetSingle.MakeUpper();
      }
      else
      {
        statement.MakeLower();
        sourceSingle.MakeLower();
        targetSingle.MakeLower();
      }

      // Rewrite the single select
      statement.Replace(sourceSingle,targetSingle);

      m_log.WriteLog(statement);
      if(m_directMigration == MigrateType::DataPump     ||
         m_directMigration == MigrateType::SlowDataPump ||
         m_directMigration == MigrateType::SelectInsert )
      {
        try
        {
          SQLQuery query(m_databaseTarget);
          SQLTransaction trans(m_databaseTarget,_T("CreateView"));
          query.DoSQLStatement(statement);
          trans.Commit();
        }
        catch(StdException& ex)
        {
          m_log.WriteLog(_T("ERROR: View creation failed"));
          m_log.WriteLog(ex.GetErrorMessage());
          m_params.v_errors++;

          // If the first statement does NOT succeed,
          // the others (GRANTS) have no use, so continue to next view
          if(first)
          {
            break;
          }
        }
      }
      else
      {
        statement.Replace(_T("\r"),_T(""));
        m_log.WriteOut(statement,true);
      }
      first = false;
    }
    // Show number of tables
    m_log.SetTablesGauge(++numViews);
  }
}

//////////////////////////////////////////////////////////////////////////
//
// MAKE TABLES INITIALLY EMPTY
//
//////////////////////////////////////////////////////////////////////////

void
SQLMigrate::TruncateTables()
{
  int numTables = 0;

  XString text;
  XString comment(_T("-- "));
  XString header1(_T("EMPTY THE TABLES IN THE TARGET DATABASE"));
  XString header2(_T("======================================="));

  m_log.WriteLog(_T(""));
  m_log.WriteLog(header1);
  m_log.WriteLog(header2);

  if(m_directMigration == MigrateType::SQLScripts)
  {
    m_log.WriteOut(comment + header1);
    m_log.WriteOut(comment + header2);
    m_log.WriteOut(_T(""));
  }

  // Current status
  XString status;
  status.Format(_T("Empty %d tables"),(int)m_tables.size());
  m_log.SetStatus(status);

  for(unsigned int ind = 0; ind < m_tables.size(); ++ind)
  {
    XString table = m_tables[ind].m_table;
    if(!m_params.v_target_schema.IsEmpty() && m_databaseTarget->GetSQLInfoDB()->GetRDBMSUnderstandsSchemas())
    {
      table = m_params.v_target_schema + _T(".") + table;
    }

    XString statement = _T("DELETE FROM ") + table;
    // Process result
    m_log.WriteLog(statement);
    if(m_directMigration == MigrateType::DataPump     ||
       m_directMigration == MigrateType::SlowDataPump ||
       m_directMigration == MigrateType::SelectInsert )
    {
      try
      {
        SQLQuery query(m_databaseTarget);
        SQLTransaction trans(m_databaseTarget,_T("truncate"));
        query.DoSQLStatement(statement);
        int rows = query.GetNumberOfRows();
        trans.Commit();

        text.Format(_T("Number of rows removed: [%d]"),rows);
        m_log.WriteLog(text);
      }
      catch(StdException& ex)
      {
        m_log.WriteLog(_T("ERROR: Failed to empty the table"));
        m_log.WriteLog(ex.GetErrorMessage());
        m_params.v_errors++;
      }
    }
    else
    {
      m_log.WriteOut(statement,true);
    }
    // Show number of tables
    m_log.SetTablesGauge(++numTables);
  }
}

//////////////////////////////////////////////////////////////////////////
//
// FILLING THE TABLES
//
//////////////////////////////////////////////////////////////////////////

void
SQLMigrate::FillTablesViaPump()
{
  int  numTables = 0;
  XString text;

  m_log.WriteLog(_T(""));
  m_log.WriteLog(_T("MIGRATING TABLE CONTENT TO TARGET DATABASE"));
  m_log.WriteLog(_T("=========================================="));

  // Current step
  XString status;
  status.Format(_T("Migrating the contents of %d tables"),(int)m_tables.size());
  m_log.SetStatus(status);

  // Create rebind mapping with known exceptions
  RebindMap rebinds;
  DatatypeExceptions(rebinds);

  for(unsigned int ind = 0; ind < m_tables.size(); ++ind)
  {
    XString table  = m_tables[ind].m_table;
    XString schema = m_params.v_target_schema;

    long rows    = 0;
    long totrows = 0;
    int  missing = 0;

    // Start processing
    clock_t start = clock();
    clock_t einde = 0;

    try
    {
      text.Format(_T("Migrating table     : %s"), table.GetString());
      m_log.WriteLog(text);

      totrows = CountTableContents(schema,table);
      text.Format(_T("Total number of rows: %ld"), totrows);
      m_log.WriteLog(text);

      if(totrows)
      {
        SQLQuery query1(m_databaseSource);
        SQLQuery query2(m_databaseTarget);

        // Source->Target database datatype migration
        XString select = MakeSelectStatement(table,m_params.v_source_schema);
        XString insert = MakeInsertStatement(table,m_params.v_source_schema,m_params.v_target_schema);

        SQLTransaction trans(m_databaseTarget,_T("Migration"));

        // Prepare the query objects with the correct rebind mappings
        query1.SetRebindMap(&rebinds);
        query2.SetRebindMap(&rebinds);

        // Prepare the queries, including the rebinds
        query1.DoSQLPrepare(select);
        query2.DoSQLPrepare(insert);
   
        // There we go!
        query1.DoSQLExecute();

        bool ready = false;
        while(!ready)
        {
          try
          {
            query1.SetFetchPolicy(true);

            if(query1.GetRecord())
            {
              if(m_params.v_truncate)
              {
                // Data field truncate
                query1.TruncateCharFields();
                // Most databases do not support TIMESTAMP fractions
                query1.TruncateTimestamps();
              }

              // Bind output from query1 to the input of query2
              SetExecParameters(query2.GetParameterMap(),query1.GetBoundedColumns());

              // GO EXECUTE!
              // Execute the prepared statement, while re-binding the parameters
              query2.DoSQLExecute(true);

              if(m_params.v_truncate)
              {
                // Reset the char data length to the original buffer length
                query1.TruncateCharFieldsReset();
              }

              // Increment rows and potentially show in the dialog
              if(++rows % 10 == 0)
              {
                // Show progress (but not every row or we will flicker)
                m_log.SetTableGauge(rows,totrows);
              }
              if(rows % m_params.v_logLines == 0)
              {
                // Show if requested in the log
                text.Format(_T("Table: %s Rows: %ld [%6.2f %%]"),table.GetString(),rows,((double) rows / (double)totrows * 100.0));
                m_log.WriteLog(text);
              }
            }
            else
            {
              // Read past last record in the select
              ready = true;
            }
          }
          catch(StdException& ex)
          {
            // Show missing records for first 100 rows
            if(++missing < 100)
            {
              XString error = ex.GetErrorMessage();
              LogMissingRecord(query1,error);
            }
            else if(missing == 100)
            {
              m_log.WriteLog(_T(""));
              m_log.WriteLog(_T("More than 100 missing records. Other records are not individually shown in the logfile!!"));
              m_log.WriteLog(_T(""));
            }
            ++m_params.v_errors;
          }
        }
        trans.Commit();

        // End
        m_log.SetTableGauge(rows,totrows);
        text.Format(_T("Table: %s Rows: %ld [%6.2f %%]"),table.GetString(),rows,((double)rows / (double)totrows * 100.0));
        m_log.WriteLog(text);
      }
    }
    catch(StdException& ex)
    {
      m_log.WriteLog(_T("ERROR: Migration of table has failed"));
      m_log.WriteLog(ex.GetErrorMessage());
    }
    catch(...)
    {
      m_log.WriteLog(_T("ERROR: Migration of table has failed"));
      m_log.WriteLog(_T("Unknown error while migrating"));
    }


    // Show tables
    m_log.SetTablesGauge(++numTables);
    XString overgezet;
    overgezet.Format(_T("Table [%s.%s] migrated with [%ld of %ld] rows"),m_params.v_source_user.GetString(),table.GetString(),rows,totrows);
    m_log.WriteLog(overgezet);
    // Extra missing records log
    if(rows < totrows)
    {
      overgezet.Format(_T("MISSING: Table [%s.%s] missing rows: %d"),m_params.v_source_user.GetString(),table.GetString(),(totrows - rows));
      m_log.WriteLog(overgezet);
    }
    // End in the logfile
    einde = clock();
    overgezet.Format(_T("Table [%s.%s] Total processing time: %.2f seconds"),m_params.v_source_user.GetString(),table.GetString(),(double)(einde - start) / CLOCKS_PER_SEC);
    m_log.WriteLog(overgezet);
  }
}

void
SQLMigrate::FillTablesViaSlowPump()
{
  int  numTables = 0;
  XString text;

  m_log.WriteLog(_T(""));
  m_log.WriteLog(_T("MIGRATING TABLE CONTENT TO TARGET DATABASE (SLOW PUMP)"));
  m_log.WriteLog(_T("======================================================"));

  // Current step
  XString status;
  status.Format(_T("Migrating the contents of %d tables"),(int)m_tables.size());
  m_log.SetStatus(status);

  // Create rebind mapping with known exceptions
  RebindMap rebinds;
  DatatypeExceptions(rebinds);

  for(unsigned int ind = 0; ind < m_tables.size(); ++ind)
  {
    XString table = m_tables[ind].m_table;
    XString schema = m_params.v_target_schema;

    long rows    = 0;
    long totrows = 0;
    int  missing = 0;

    // Start processing
    clock_t start = clock();
    clock_t einde = 0;

    try
    {
      text.Format(_T("Migrating table     : %s"),table.GetString());
      m_log.WriteLog(text);

      totrows = CountTableContents(schema,table);
      text.Format(_T("Total number of rows: %ld"),totrows);
      m_log.WriteLog(text);

      if(totrows)
      {
        SQLQuery query1(m_databaseSource);
        query1.SetFetchPolicy(true);

        // Source->Target database datatype migration
        XString select = MakeSelectStatement(table,m_params.v_source_schema);
        XString insert = MakeInsertStatement(table,m_params.v_source_schema,m_params.v_target_schema);

        SQLTransaction trans(m_databaseTarget,_T("Migration"));

        // Prepare the query objects with the correct rebind mappings
        query1.SetRebindMap(&rebinds);

        // There we go!
        query1.DoSQLStatement(select);

        bool ready = false;
        while(!ready)
        {
          try
          {
            if(query1.GetRecord())
            {
              if(m_params.v_truncate)
              {
                // Data field truncate
                query1.TruncateCharFields();
                // Most databases do not support TIMESTAMP fractions
                query1.TruncateTimestamps();
              }
              SQLQuery query2(m_databaseTarget);
              query2.SetRebindMap(&rebinds);

              // Bind output from query1 to the input of query2
              SetExecParameters(query2,query1.GetBoundedColumns());


//               SQLVariant* oid = query1.GetColumn(17);
//               TRACE("OID = %d\n",oid->GetAsSLong());
//               CString test("TEST");
//               LogMissingRecord(query1,test);
//               int col = query1.GetColumnNumber("EI_OID");
//               SQLVariant* eioid = query1.GetColumn(col);
//               TRACE("EI_OID = %d\n",eioid->GetAsBCD().AsLong());

              // GO EXECUTE!
              // Execute the insert statement with these parameters
              query2.DoSQLStatement(insert);

              if(m_params.v_truncate)
              {
                // Reset the char data length to the original buffer length
                query1.TruncateCharFieldsReset();
              }
              query2.ResetParameters();

              // Increment rows and potentially show in the dialog
              if(++rows % 10 == 0)
              {
                // Show progress (but not every row or we will flicker)
                m_log.SetTableGauge(rows,totrows);
              }
              if(rows % m_params.v_logLines == 0)
              {
                // Show if requested in the log
                text.Format(_T("Table: %s Rows: %ld [%6.2f %%]"),table.GetString(),rows,((double)rows / (double)totrows * 100.0));
                m_log.WriteLog(text);
              }
            }
            else
            {
              // Read past last record in the select
              ready = true;
            }
          }
          catch(StdException& ex)
          {
            // Show missing records for first 100 rows
            if(++missing < 100)
            {
              XString error = ex.GetErrorMessage();
              LogMissingRecord(query1,error);
            }
            else if(missing == 100)
            {
              m_log.WriteLog(_T(""));
              m_log.WriteLog(_T("More than 100 missing records. Other records are not individually shown in the logfile!!"));
              m_log.WriteLog(_T(""));
            }
            ++m_params.v_errors;
          }
        }
        trans.Commit();

        // End
        m_log.SetTableGauge(rows,totrows);
        text.Format(_T("Table: %s Rows: %ld [%6.2f %%]"),table.GetString(),rows,((double)rows / (double)totrows * 100.0));
        m_log.WriteLog(text);
      }
    }
    catch(StdException& ex)
    {
      m_log.WriteLog(_T("ERROR: Migration of table has failed"));
      m_log.WriteLog(ex.GetErrorMessage());
    }
    catch(...)
    {
      m_log.WriteLog(_T("ERROR: Migration of table has failed"));
      m_log.WriteLog(_T("Unknown error while migrating"));
    }

    // Show tables
    m_log.SetTablesGauge(++numTables);
    XString overgezet;
    overgezet.Format(_T("Table [%s.%s] migrated with [%ld of %ld] rows"),m_params.v_source_user.GetString(),table.GetString(),rows,totrows);
    m_log.WriteLog(overgezet);
    // Extra missing records log
    if(rows < totrows)
    {
      overgezet.Format(_T("MISSING: Table [%s.%s] missing rows: %d"),m_params.v_source_user.GetString(),table.GetString(),(totrows - rows));
      m_log.WriteLog(overgezet);
    }
    // End in the logfile
    einde = clock();
    overgezet.Format(_T("Table [%s.%s] Total processing time: %.2f seconds"),m_params.v_source_user.GetString(),table.GetString(),(double)(einde - start) / CLOCKS_PER_SEC);
    m_log.WriteLog(overgezet);
  }
}

void
SQLMigrate::SetExecParameters(ParameterMap& p_params,VarMap* p_columns)
{
  for(auto& column : *p_columns)
  {
    int index = column.first;
    SQLVariant* var = column.second;
    SQLVariant* nwv = nullptr;

    ParameterMap::iterator it = p_params.find(index);
    if(it == p_params.end())
    {
      SQLParameter param;
      param.m_number  = index;
      param.m_maxSize = 0;
      param.m_type    = SQLParamType::P_SQL_PARAM_INPUT;
      param.m_value   = new SQLVariant(var);

      nwv = param.m_value;
      p_params.insert(std::make_pair(index,param));
    }
    else
    {
      delete it->second.m_value;
      nwv = new SQLVariant(var);
      it->second.m_value = nwv;
    }
    nwv->SetColumnNumber(index);
    nwv->SetParameterType(SQLParamType::P_SQL_PARAM_INPUT);
    if(nwv->GetAtExec())
    {
       nwv->SetAtExec(false);
      *nwv->GetIndicatorPointer() = SQL_NTS;
    }
  }
}

void
SQLMigrate::SetExecParameters(SQLQuery& p_target,VarMap* p_columns)
{
  int index = 1;
  for(auto& column : *p_columns)
  {
    SQLVariant* var = p_target.SetParameter(index++,column.second);
    if(var->GetAtExec())
    {
      var->SetAtExec(false);
      *var->GetIndicatorPointer() = SQL_NTS;
    }
  }
}

XString
SQLMigrate::FormatColumnName(SQLInfoDB* p_info,XString p_column)
{
  // Circumvent locally reserved words
  if(p_info->GetRDBMSDatabaseType() == DatabaseType::RDBMS_SQLSERVER && p_column.GetAt(0) != '[')
  {
    p_column = _T("[") + p_column + _T("]");
  }
  else if(!p_info->IsCorrectName(p_column))
  {
    XString quote = p_info->GetKEYWORDReservedWordQuote();
    p_column = quote + p_column + quote;
  }
  else
  {
    // Possibly a quoted identifier
    p_column = p_info->QueryIdentifierQuotation(p_column);
  }
  return p_column;
}

XString
SQLMigrate::MakeSelectStatement(XString& p_tabel,XString& p_user)
{
  XString    statement(_T("SELECT "));
  MColumnMap columns;
  XString    catalog;
  XString    schema;
  XString    errors;
  SQLInfoDB* source = m_databaseSource->GetSQLInfoDB();

  source->MakeInfoTableColumns(columns,errors,catalog,p_user,p_tabel);
  columns = SortColumnsBySize(columns);
  for(unsigned int regel = 0; regel < columns.size(); ++regel)
  {
    MetaColumn* info = &(columns[regel]);

    if(regel > 0) 
    {
      statement += _T(",");
    }
    statement += FormatColumnName(source,info->m_column);
  }
  statement += _T(" FROM ");
  if(!p_user.IsEmpty() && source->GetRDBMSUnderstandsSchemas())
  {
    statement += p_user + _T(".");
  }
  statement += p_tabel;

  // Optional begin on the object-id primary key of the table
  // Used in CXHibernate and network databases
  if(!m_params.v_where.IsEmpty())
  {
    statement += _T("\n WHERE ");
    statement += m_params.v_where;
  }
  return statement;
}

XString
SQLMigrate::MakeInsertStatement(XString& p_tabel,XString& p_user,XString& p_doel_user)
{
  XString    data;
  XString    statement(_T("INSERT INTO "));
  MColumnMap columns;
  XString    catalog;
  XString    schema;
  XString    errors;
  SQLInfoDB* target = m_databaseTarget->GetSQLInfoDB();

  if(!p_doel_user.IsEmpty() && target->GetRDBMSUnderstandsSchemas())
  {
    statement += target->GetSQLDDLIdentifier(p_doel_user) + _T(".");
  }
  statement += target->GetSQLDDLIdentifier(p_tabel) + _T(" (");

  m_databaseSource->GetSQLInfoDB()->MakeInfoTableColumns(columns,errors,catalog,p_user,p_tabel);
  columns = SortColumnsBySize(columns);
  for(unsigned int regel = 0; regel < columns.size(); ++regel)
  {
    MetaColumn* info = &(columns[regel]);
    if(regel > 0) 
    {
      statement += _T(",");
      data      += _T(",");
    }
    statement +=  FormatColumnName(target,info->m_column);
    data      += _T("?");
  }
  statement += _T(") VALUES (") + data + _T(")");
  return statement;
}

long
SQLMigrate::CountTableContents(XString p_owner,XString& tabel)
{
  XString  name(_T("count"));
  SQLQuery query(m_databaseSource);
  XString  statement = _T("SELECT COUNT(*) FROM ");

  if(m_databaseTarget->GetSQLInfoDB()->GetRDBMSUnderstandsSchemas())
  {
    statement += p_owner + _T(".");
  }
  statement += tabel;

  // Optional begin on the primary key of the table
  if(!m_params.v_where.IsEmpty())
  {
    statement += _T("\n WHERE ");
    statement += m_params.v_where;
  }

  SQLVariant* var = query.DoSQLStatementScalar(statement);
  if(var)
  {
    return var->GetAsSLong();
  }
  return 0;
}

void
SQLMigrate::LogMissingRecord(SQLQuery& p_query,XString& p_error)
{
  XString text(_T("ERROR: "));
  m_log.WriteLog(_T("MISSING RECORD"));
  // Reason for the error
  text += p_error;
  m_log.WriteLog(text);

  int aantal = p_query.GetNumberOfColumns();
  for(int ind = 1; ind <= aantal; ++ind)
  {
    XString naam;
    XString waarde;
    SQLVariant* var = p_query.GetColumn(ind);
    var->GetAsString(waarde);
    p_query.GetColumnName(ind,naam);

    text.Format(_T("%d: %s [%s]"),ind,naam.GetString(),waarde.GetString());
    m_log.WriteLog(text);
  }
}

void
SQLMigrate::CommitDDL(SQLQuery& p_query,SQLInfoDB* p_info)
{
  if(p_info->GetRDBMSMustCommitDDL())
  {
    XString statement(_T("COMMIT"));
    XString name(_T("Commit DDL"));
    p_query.DoSQLStatement(statement);
  }
}

//////////////////////////////////////////////////////////////////////////
//
// ALTERNATIVE (OLD) WAY OF INSERTING DATA
//
//////////////////////////////////////////////////////////////////////////

void
SQLMigrate::FillTablesViaData(bool p_process)
{
  int numTables = 0;
  int missing   = 0;
  XString comment(_T("-- "));
  XString header1(_T("TABLE CONTENTS MIGRATION TO INSERT SCRIPTS"));
  XString header2(_T("=========================================="));

  if(p_process)
  {
    header1 = _T("TABLE CONTENTS MIGRATION VIA SELECT/INSERT");
  }

  m_log.WriteLog(_T(""));
  m_log.WriteLog(header1);
  m_log.WriteLog(header2);

  m_log.WriteOut(comment + header1);
  m_log.WriteOut(comment + header2);
  m_log.WriteOut(_T(""));

  // Current step to status line
  XString status;
  status.Format(_T("Migrating the contents of %d tables"),(int) m_tables.size());
  m_log.SetStatus(status);

  for(unsigned int ind = 0; ind < m_tables.size(); ++ind)
  {
    long rows      = 0;
    long totalrows = 0;

    // Get pattern from source database
    // TABLE: catalog:schema.table 
    XString table = m_tables[ind].m_table;

    // Process
    clock_t start = clock(), einde;
    try
    {
      m_log.WriteOut(_T(""));
      m_log.WriteOut(_T("-- TABLE: ") + table);
      m_log.WriteOut(_T(""));

      // Get the columns
      XString    errors;
      XString    catalog;
      XString    schema;
      MColumnMap columns;

      m_databaseSource->GetSQLInfoDB()->MakeInfoTableColumns(columns,errors,catalog,m_params.v_source_schema,table);
      columns = SortColumnsBySize(columns);

      totalrows = CountTableContents(m_params.v_source_user,table);
      if(totalrows)
      {
        XString meld;
        meld.Format(_T("Migrating table    : %s"),table.GetString());
        m_log.WriteLog(meld);
        meld.Format(_T("Number of records  : %ld"),totalrows);
        m_log.WriteLog(meld);

        SQLQuery query1(m_databaseSource);
        SQLQuery query2(m_databaseTarget);
        SQLTransaction trans(m_databaseSource,_T("Migration"));

        XString select = MakeSelectStatement(table,m_params.v_source_schema);
        query1.DoSQLStatement(select);

        while(query1.GetRecord())
        {
          XString insert;
          int extra = 0;

          try
          {
            insert = MakeInsertDataStatement(table,m_params.v_target_schema,query1,columns);
            if (p_process)
            {
              query2.DoSQLStatement(insert);
            }
            else
            {
              m_log.WriteOut(insert, true);
            }
            extra = 1;
          }
          catch(StdException& ex)
          {
            // Show missing records for first 100 rows
            if(++missing < 100)
            {
              XString error = ex.GetErrorMessage();
              LogMissingRecord(query1,error);
              m_log.WriteLog(insert);
            }
            else if(missing == 100)
            {
              m_log.WriteLog(_T(""));
              m_log.WriteLog(_T("More than 100 missing records. Other records are not individually shown in the logfile!!"));
              m_log.WriteLog(_T(""));
            }
            ++m_params.v_errors;
          }

          // Next row
          rows += extra;

          // Mark progress in gauge
          m_log.SetTableGauge(rows,totalrows);
          if(rows % m_params.v_logLines == 0)
          {
            XString text;
            text.Format(_T("Table: %s Rows: %ld [%5.2f %%]"),table.GetString(),rows,((double)rows / (double)totalrows * 100.0));
            m_log.WriteLog(text);
          }
        }
        trans.Commit();
      }
    }
    catch(StdException& ex)
    {
      m_log.WriteLog(_T("ERROR: Failed to list table content"));
      m_log.WriteLog(ex.GetErrorMessage());
    }
    catch(...)
    {
      m_log.WriteLog(_T("ERROR: Failed to list table content"));
      m_log.WriteLog(_T("Unknown error at migration"));
    }
    // Show tables
    m_log.SetTablesGauge(++numTables);
    XString processed;
    processed.Format(_T("Table [%s.%s] processed with [%ld of %ld] rows"),m_params.v_source_user.GetString(),table.GetString(),rows,totalrows);
    m_log.WriteLog(processed);
    // Extra missing rows in logfile
    if(rows < totalrows)
    {
      processed.Format(_T("MISSING ROWS: Table [%s.%s] total missing rows: %d"),m_params.v_source_user.GetString(),table.GetString(),(totalrows - rows));
      m_log.WriteLog(processed);
    }
    // End in logfile
    einde = clock();
    processed.Format(_T("Table [%s.%s] Total running time: %.2f seconds"),m_params.v_source_user.GetString(),table.GetString(),(double)(einde - start) / CLOCKS_PER_SEC);
    m_log.WriteLog(processed);
  }
}

XString
SQLMigrate::MakeInsertDataStatement(XString& p_table,XString& p_target_schema,SQLQuery& p_input,MColumnMap& columns)
{
  XString data;
  XString statement(_T("INSERT INTO "));
  bool seperator = false;
  SQLInfoDB* target = m_databaseTarget->GetSQLInfoDB();

  if(!p_target_schema.IsEmpty() && target->GetRDBMSUnderstandsSchemas())
  {
    statement += target->GetSQLDDLIdentifier(p_target_schema) + _T(".");
  }
  statement += target->GetSQLDDLIdentifier(p_table) + _T(" (");

  for(unsigned int row = 0; row < columns.size(); ++row)
  {
    MetaColumn* info = &(columns[row]);
    SQLVariant* var  = p_input.GetColumn(row + 1);
    XString datum    = VariantToInsertString(var,info->m_datatype);
  
    if(!datum.IsEmpty() && datum != _T("\'\'"))
    {
      if(seperator) 
      {
        data      += _T(",");
        statement += _T(",");
      }
      data      += datum;
      statement += FormatColumnName(target,info->m_column);
      seperator  = true;
    }
  }

  statement += _T(") VALUES (") + data + _T(")");
  return statement;
}

// Sort column on ascending size
MColumnMap 
SQLMigrate::SortColumnsBySize(MColumnMap& columns)
{
  MColumnMap sorted;

  while(!columns.empty())
  {
    int shortest = INT_MAX;
    MColumnMap::iterator found = columns.begin();
    for(MColumnMap::iterator it = columns.begin();it != columns.end();++it)
    {
      if(it->m_columnSize < shortest)
      {
        shortest = it->m_columnSize;
        found    = it;
      }
    }

    sorted.push_back(*found);
    columns.erase(found);
  }

  return sorted;
}


XString
SQLMigrate::VariantToInsertString(SQLVariant* p_var,int p_datatype)
{
        XString           result;
  const TIMESTAMP_STRUCT* stamp;
  const DATE_STRUCT*      date;
  const TIME_STRUCT*      time;
  const SQLGUID*          guid;

  switch(p_datatype)
  {
    case SQL_CHAR:        // Fall through
    case SQL_VARCHAR:
    case SQL_LONGVARCHAR: p_var->GetAsString(result);
                          result.Replace(_T("\'"),_T("\'\'"));
                          result = _T("\'") + result + _T("\'");
                          break;
    case SQL_TIME:
    case SQL_TYPE_TIME:   time = p_var->GetAsTime();
                          result.Format(_T("{t '%2.2d:%2.2d:%2.2d'}")
                                       ,time->hour
                                       ,time->minute
                                       ,time->second);
                          break;
    case SQL_DATE:        
    case SQL_TYPE_DATE:   date = p_var->GetAsDate();
                          if(date->day && date->month && date->year)
                          {
                            result.Format(_T("{d '%4.4d-%2.2d-%2.2d'}")
                                         ,date->year
                                         ,date->month
                                         ,date->day);
                          }
                          break;
    case SQL_TIMESTAMP:
    case SQL_TYPE_TIMESTAMP:
                          stamp = p_var->GetAsTimestamp();
                          if(stamp->day && stamp->month && stamp->year)
                          {
                            result.Format(_T("{ts '%4.4d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d'}")
                                         ,stamp->year
                                         ,stamp->month
                                         ,stamp->day
                                         ,stamp->hour
                                         ,stamp->minute
                                         ,stamp->second);
                          }
                          break;
    case SQL_GUID:        guid = p_var->GetAsGUID();
                          result.Format(_T("{guid '%8.8X-%4.4X-%4.4X-%X%X%X%X%X%X%X%X'}")
                                        ,guid->Data1
                                        ,guid->Data2
                                        ,guid->Data3
                                        ,guid->Data4[0]
                                        ,guid->Data4[1]
                                        ,guid->Data4[2]
                                        ,guid->Data4[3]
                                        ,guid->Data4[4]
                                        ,guid->Data4[5]
                                        ,guid->Data4[6]
                                        ,guid->Data4[7]);
                          break;
    // Most datatypes (numeric)
    // BEWARE: Intervals not implemented
    default:              p_var->GetAsString(result);
                          break;
  }
  return result;
}

// These are the exceptions found throughout the years
// Conversions that cannot be done in the 'regular' way
void
SQLMigrate::DatatypeExceptions(RebindMap& p_map)
{
  // ORACLE -> Firebird
  if(m_params.v_sourceType == DatabaseType::RDBMS_ORACLE &&
     m_params.v_targetType == DatabaseType::RDBMS_FIREBIRD)
  {
    // CLOB -> BLOB SUB_TYPE TEXT
    p_map.insert(std::make_pair(SQL_LONGVARCHAR,SQL_LONGVARBINARY));
  }

  // ORACLE -> MS-SQLServer. 
  // Only way to convert the timestamps
  // Otherwise we get a 'timestamp field overflow' error
  // And fractions of timestamps not convertable!!
  if(m_params.v_sourceType == DatabaseType::RDBMS_ORACLE &&
     m_params.v_targetType == DatabaseType::RDBMS_SQLSERVER)
  {
    p_map.insert(std::make_pair(SQL_TYPE_TIMESTAMP, SQL_VARCHAR));
    p_map.insert(std::make_pair(SQL_FLOAT,          SQL_DOUBLE));
  }
}

}
