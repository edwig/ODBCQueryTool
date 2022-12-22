////////////////////////////////////////////////////////////////////////
//
// File: SQLMigrate.h
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
#pragma once
#include "SQLComponents.h"
#include "Logging.h"
#include "MigrateParameters.h"
#include "SQLDatabase.h"
#include "SQLQuery.h"
#include "SQLMetaInfo.h"
#include "DDLCreateTable.h"
#include <vector>

namespace SQLComponents
{

class SQLMigrate  
{
public:
  SQLMigrate(MigrateParameters& p_params,Logging& p_log);
 ~SQLMigrate();
  
  // Migrate the database
  bool     Migrate();

private:
  // Check parameters. Can throw!
  void     CheckMigrateParameters();
  void     WriteMigrateParameters();

  bool     MakeDatabaseConnections();
  int      FindTablesToMigrate();
  int      ReadTableStructures(XString p_owner,XString p_patroon,SQLDatabase* p_database);
  void     ReadTablesFromFile (XString& p_bestand);
  void     RemoveTemporaries();
  void     CleanUpMigration();
  // Processing
  void     DropTables();
  void     CreateTables();
  void     CreateViews();
  void     FillTablesViaPump();
  void     FillTablesViaData(bool p_process);
  void     TruncateTables();
  int      FindColumn(MColumnMap& p_columns,XString p_name);

  void     OrderTableColumns(DDLCreateTable& p_create);
  void     FixupTableColumns(DDLCreateTable& p_create);
  void     FixupTableIndices(DDLCreateTable& p_create);

  // Statements
  XString  MakeSelectStatement    (XString& p_tabel,XString& p_user);
  XString  MakeInsertStatement    (XString& p_tabel,XString& p_user,XString& p_doel_user);
  XString  MakeInsertDataStatement(XString& p_tabel,XString& p_target_schema,SQLQuery& p_input,MColumnMap& kolommen);

  XString  VariantToInsertString  (SQLVariant* p_var,int p_datatype);
  long     CountTableContents     (XString  p_owner,XString& tabel);
  void     CommitDDL(SQLQuery& p_query,SQLInfoDB* p_info);
  void     DatatypeExceptions(RebindMap& p_map);

  // Record missing records
  void     LogMissingRecord(SQLQuery& p_query,XString& p_error);

  typedef std::vector<XString> TablesMap;

  // DATA
  MigrateParameters m_params;
  Logging&          m_log;

  int          m_directMigration { 0 };
  int          m_totalTables     { 0 };
  clock_t      m_start           { 0 };
  SQLDatabase* m_databaseSource  { nullptr };
  SQLDatabase* m_databaseTarget  { nullptr };
  MTableMap    m_tables;
  RebindMap    m_rebinds;
};

}