////////////////////////////////////////////////////////////////////////
//
// File: MigrateParameters.h
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
#pragma once
#include "SQLComponents.h"
#include "SQLDatabase.h"

namespace SQLComponents
{

enum class MigrateType
{
  DataPump        = 0
 ,SlowDataPump    = 1
 ,SelectInsert    = 2
 ,SQLScripts      = 3
};

class MigrateParameters
{
public:
  // Database types
  DatabaseType  v_sourceType  { DatabaseType::RDBMS_UNKNOWN };
  DatabaseType  v_targetType  { DatabaseType::RDBMS_UNKNOWN };

  // String parameters
  XString v_source_dsn;     // Source database ODBC DataSourceName
  XString v_source_user;    // Source database user to log in
  XString v_source_password;// Source database user password
  XString v_source_schema;  // Source schema of tables to migrate

  XString v_target_dsn;     // Target database ODBC DataSourceName
  XString v_target_user;    // Target database user to login in
  XString v_target_password;// Target database user password
  XString v_target_schema;  // Target schema of tables to migrate

  XString v_tablespace;     // Optional tablespace when creating a table
  XString v_table;          // Optional table to convert, all if empty

  XString v_directory;      // Directory for the scripts
  XString v_dropscript;     // Script with all DROP statements
  XString v_createscript;   // Script with all CREATE and INSERT statements

  // Options
  int     v_allObjects      {    0 };    // Explicitly convert all tables/views/indexes/synonyms etc
  int     v_do_tables       {    0 };    // Convert tables (drop and re-create)
  int     v_do_data         {    0 };    // Convert data in the tables
  int     v_do_views        {    0 };    // Convert views instead of tables
  int     v_truncate        {    0 };    // Truncate char fields in table before converting
  int     v_deletes         {    0 };    // Truncate/delete table contents before converting
  int     v_primarys        {    0 };    // Add primary key to tables
  int     v_indices         {    0 };    // Add all other indices to tables
  int     v_foreigns        {    0 };    // add foreign keys to other tables
  int     v_sequences       {    0 };    // Add sequence for primary key
  int     v_triggers        {    0 };    // Add triggers on the tables
  int     v_access          {    0 };    // Grant access on tables
  int     v_logLines        { 1000 };    // Logfile line per x lines
  XString v_where;                       // Extra where clause on the migration
  // Type of migration
  MigrateType v_direct      { MigrateType::DataPump };    // direct migration into database 
  // Keep track of total
  int     v_errors          {    0 };    // Total number of errors
};

}