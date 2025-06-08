////////////////////////////////////////////////////////////////////////
//
// File: SQLDataSet.h
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
#include "SQLRecord.h"
#include "SQLVariant.h"
#include "SQLFilter.h"
#include "XMLMessage.h"
#include <vector>

namespace SQLComponents
{

// SET status is a OR-ed bitmap status
#define SQL_Empty       0x00
#define SQL_Selections  0x01
#define SQL_Updates     0x02
#define SQL_Deletions   0x04
#define SQL_Insertions  0x08

// Names for saving the dataset to XML structures
#define DATASET_NAME      0
#define DATASET_STRUCTURE 1
#define DATASET_FIELD     2
#define DATASET_RECORD    3
#define DATASET_RECORDS   4
#define DATASET_ID        5
#define DATASET_TYPE      6
#define DATASET_TYPENAME  7
#define NUM_DATASET_NAMES 8

// Default waittime for a record lock (seconds!)
#define DEFAULT_LOCK_TIMEOUT  15

// Names for saving datasets to XML in various languages
extern LPCTSTR dataset_names[LN_NUMLANG][NUM_DATASET_NAMES];

// Parameter for the query
typedef struct _sql_parameter
{
  XString    m_name;
  SQLVariant m_value;
}
SQLParameter;

typedef void (*LPFN_CALLBACK)(void*);

#define MAX_BCD  _T("1E+300");
#define MIN_BCD _T("-1E+300");

class AggregateInfo
{
public:
  AggregateInfo() 
  { 
    m_min = MAX_BCD;
    m_max = MIN_BCD;
  };

  bcd  m_sum;
  bcd  m_max;
  bcd  m_min;
  bcd  m_mean;
};

class SQLDatabase;
class SQLQuery;

typedef std::vector<SQLRecord*>     RecordSet;
typedef std::vector<SQLVariant*>    VariantSet;
typedef std::vector<SQLParameter>   ParameterSet;
typedef std::vector<XString>        NamenMap;
typedef std::vector<int>            TypenMap;
typedef std::map<XString,int>       ObjectMap;
typedef std::list<XString>          WordList;

class SQLDataSet
{
public:
  SQLDataSet();
  explicit SQLDataSet(XString p_name,SQLDatabase* p_database = NULL);
  virtual ~SQLDataSet();

  // Perform query: Do SetQuery first
  virtual bool Open();
  // Perform query on dataset (transaction not in scope)
  virtual bool Open(SQLQuery& p_query);
  // Append (read extra) into the dataset (new query)
  virtual bool Append();
  // Perform append on open query
  virtual bool Append(SQLQuery& p_query);
  // Remove result set
  virtual void Close();
  // Query done and records gotten?
  bool IsOpen();

  // Navigate in the records
  int  First();
  int  Next();
  int  Prev();
  int  Last();
  int  Goto(int p_record);
  bool IsFirst();
  bool IsLast();
  int  Current();

  // OPERATIONS

  // Insert new record (Manually)
  SQLRecord*   InsertRecord();
  // Insert new field in new record (manually)
  int          InsertField(XString p_name,const SQLVariant* p_value);
  // Calculate aggregate functions
  int          Aggregate(int p_num,AggregateInfo& p_info);
  // Cancel the mutations of this mutation ID
  void         CancelMutation(int p_mutationID);
  // Insert / Update / delete records from the database
  bool         Synchronize(int p_mutationID = 0,bool p_throw = false);
  // In case synchronize doesn't work, ask for mixed mutations
  int          AllMixedMutations(MutationIDS& p_list,int p_mutationID);
  // Find the object record of a primary key
  int          FindObjectRecNum(int p_primary);               // If your primary is an INTEGER     (Fast!!)
  int          FindObjectRecNum(const VariantSet& p_primary); // If your primary is a compound key (Slower)
  SQLRecord*   FindObjectRecord(int p_primary);               // If your primary is an INTEGER     (Fast!!)
  SQLRecord*   FindObjectRecord(const VariantSet& p_primary); // If your primary is a compound key (Slower)
  SQLRecord*   FindObjectFilter(bool p_primary = false);      // Fast & slow
  RecordSet*   FindRecordSet();                               // Always slow
  // Forget the records
  bool         Forget(bool p_force = false);
  // Forget just one record AND reset current cursor to first position
  bool         ForgetObject(int p_primary,              bool p_force = false); // Forget 1 record and primary is an INTEGER     (Fast!!)
  bool         ForgetObject(const VariantSet& p_primary,bool p_force = false); // Forget 1 record and primary is a compound key (Slower)
  bool         ForgetRecord(SQLRecord* p_record,bool p_force);

  // SETTERS

  // Set database connection
  virtual void SetDatabase(SQLDatabase* p_database);
  // Set SELECT one or more columns to select
  virtual void SetSelection(XString p_selection);
  // Set the isolated query status
  virtual void SetIsolation(bool p_isolation);
  // Set the lock-for-update / Waittime
  virtual void SetLockForUpdate(bool p_lock);
  virtual void SetLockWaitTime(unsigned p_seconds);
  // Set FROM selection of several tables (more than one!)
  virtual void SetFromTables(XString p_from);
  // Set FROM  primary table (for updates)
  virtual void SetPrimaryTable(XString p_schema, XString p_tableName, XString p_alias = _T(""));
  // Set WHERE condition by hand
  virtual void SetWhereCondition(XString p_condition);
  // Set WHERE filters for a query
  virtual void ResetFilters();
  virtual void SetFilters(SQLFilterSet* p_filters);
  // Add filter to current SQLFilterSet
  virtual void SetFilter(const SQLFilter& p_filter);
  // Set GROUP BY 
  virtual void SetGroupBy(XString p_groupby);
  virtual void AddGroupby(XString p_groupby);
  // Set ORDER BY
  virtual void SetOrderBy(XString p_orderby);
  virtual void AddOrderBy(XString p_orderby);

  // Set HAVING
  virtual void SetHavings(SQLFilterSet* p_havings);
  // Set SQL-QUERY:  a new full query (Supersedes SetSelection, -Where, -groupby, -orderby and -having)
  virtual void SetQuery(XString& p_query);
  // Set primary key column name (for updates)
  virtual void SetPrimaryKeyColumn(XString p_name);
  virtual void SetPrimaryKeyColumn(WordList& p_list);
  // Set if we whish to keep duplicates in the recordset
  virtual void SetKeepDuplicates(bool p_keep);

  // Open will not take action if no columns selected
  void         SetStopIfNoColumns(bool p_stop);
  // Setting the sequence/generator name to something different than "<tablename>_seq"
  void         SetSequenceName(XString p_sequence);
  // Set parameter for a query
  void         SetParameter(const SQLParameter& p_parameter);
  void         SetParameter(const XString& p_name,const SQLVariant& p_value);
  // Set maximum number of milliseconds alloted to the Open query
  void         SetQueryTime(int p_milliseconds);
  // Set top <n> records selection
  void         SetTopNRecords(int p_top,int p_skip = 0);
  // Set columns that can be updated
  void         SetUpdateColumns(const WordList& p_list);
  // Set the status to modified/saved
  void         SetStatus(int m_add,int m_delete = 0);
  // Set a field value in the current record
  bool         SetField(int p_num,const SQLVariant* p_value,int p_mutationID = 0);
  // Set a field value in the current record
  bool         SetField(const XString& p_name,const SQLVariant* p_value,int p_mutationID = 0);

  // GETTERS

  // Get the name of the dataset
  XString      GetName();
  // Getting the database used
  SQLDatabase* GetDatabase();
  // Get the number of records
  int          GetNumberOfRecords();
  int          GetSkippedRecords();
  // Get number of fields
  int          GetNumberOfFields();
  // Get the current record (number)
  int          GetCurrentRecord();
  // Get a specific record
  SQLRecord*   GetRecord(int p_recnum);
  // Gets the status of records of the dataset
  int          GetStatus();
  // Get a field name
  XString      GetFieldName(int p_num);
  // Get datatype of a field
  int          GetFieldType(int p_num);
  // Get a field number
  int          GetFieldNumber(XString p_name);
  // Get a field from the current record
  SQLVariant*  GetCurrentField(int p_num);
  // Getting info about the primary key
  XString      GetPrimarySchema();
  XString      GetPrimaryTableName();
  XString      GetPrimaryAlias();
  // Getting the sequence name
  XString      GetSequenceName();
  // Last inserted serial by our generator or sequence
  XString      GetLastInsertedSerial();
  // Getting the query settings
  XString      GetSelection();
  XString      GetFromTables();
  XString      GetWhereCondition();
  XString      GetGroupBy();
  XString      GetOrderBy();
  SQLFilterSet* GetHavings();
  // Exposing the statement for a SQLCancel
  void         SetCancelCallback(LPFN_CALLBACK p_cancelFunction);
  // Getting the status
  bool         GetLockForUpdate();
  unsigned     GetLockWaitTime();
  // Duplicates
  bool         GetKeepDuplicates();

  // XML Saving and loading
  bool         XMLSave(XString p_filename,XString p_name,Encoding p_encoding = Encoding::UTF8);
  bool         XMLLoad(XString p_filename);
  void         XMLSave(XMLMessage* p_msg,XMLElement* p_dataset);
  void         XMLLoad(XMLMessage* p_msg,XMLElement* p_dataset,const LONG* p_abort = nullptr);

protected:
  // Set parameters in the query
  virtual XString ParseQuery();
  // Construct the selection SQL for opening the dataset
  virtual XString GetSelectionSQL(SQLQuery& p_qry);
  // Parse the selection
  virtual XString ParseSelection(SQLQuery& p_query);
  // Parse the filters
  virtual XString ParseFilters(SQLQuery& p_query,XString p_sql);

  // Get the variant of a parameter
  SQLVariant*  GetParameter(const XString& p_name);
  // Get all the columns of the record
  void         ReadNames(SQLQuery& qr);
  // Get all the datatypes of the columns
  void         ReadTypes(SQLQuery& qr);
  // Check that all names are the same
  void         CheckNames(SQLQuery& p_query);
    // Check that all the types are the same
  void         CheckTypes(SQLQuery& p_query);
  // Getting the primary key for a table
  bool         GetPrimaryKeyInfo();
  // Check that all primary key columns are in the dataset
  bool         CheckPrimaryKeyColumns();
  // Read in a record from a SQLQuery
  bool         ReadRecordFromQuery(SQLQuery& p_query,bool p_modifiable,bool p_append = false);
  // Make a primary key record
  XString      MakePrimaryKey(const SQLRecord*  p_record);
  XString      MakePrimaryKey(const VariantSet& p_primary);
  // Forget about a record
  void         ForgetPrimaryObject(const SQLRecord* p_record);
  // Init the high performance counter
  void         InitCounter();
  ULONG64      GetCounter();

  // WRITEBACK OPERATIONS

  void         Deletes(int p_mutationID);
  void         Updates(int p_mutationID);
  void         Inserts(int p_mutationID);
  void         Reduce (int p_mutationID);

  XString      GetSQLDelete  (SQLQuery* p_query,const SQLRecord* p_record);
  XString      GetSQLUpdate  (SQLQuery* p_query,const SQLRecord* p_record);
  XString      GetSQLInsert  (SQLQuery* p_query,const SQLRecord* p_record);
  XString      GetWhereClause(SQLQuery* p_query,const SQLRecord* p_record,int& p_parameter);

  // Base class data of the dataset

  XString      m_name;
  SQLDatabase* m_database;
  bool         m_open { false };
  // The query to run
  XString      m_query;
  XString      m_selection;
  XString      m_fromTables;
  XString      m_whereCondition;
  XString      m_orderby;
  XString      m_groupby;
  // Parts of the query
  XString      m_primarySchema;
  XString      m_primaryTableName;
  XString      m_primaryAlias;
  XString      m_sequenceName;
  ParameterSet m_parameters;
  NamenMap     m_primaryKey;
  WordList     m_updateColumns;
  int          m_topRecords    { 0 };
  int          m_skipRecords   { 0 };
  bool         m_keepDuplicates{ false };
  bool         m_stopNoColumns { false };
  bool         m_isolation     { false };
  bool         m_lockForUpdate { false };
  unsigned     m_lockWaitTime  { DEFAULT_LOCK_TIMEOUT };
  // Filter sets
  SQLFilterSet* m_filters      { nullptr };
  SQLFilterSet* m_havings      { nullptr };
  bool          m_ownFilters   { false   };
  // Records and objects
  int          m_status    { SQL_Empty };
  int          m_current   { -1 };
  NamenMap     m_names;
  TypenMap     m_types;
  RecordSet    m_records;
  ObjectMap    m_objects;
  XString      m_serial;
  // Maximum query timing
  int          m_queryTime { 0 };
  ULONG64      m_frequency { 0 };
  // For canceling the select operation
  LPFN_CALLBACK m_cancelFunction { nullptr };
};

inline void 
SQLDataSet::SetDatabase(SQLDatabase* p_database)
{
  m_database = p_database;
}

inline SQLDatabase*
SQLDataSet::GetDatabase()
{
  return m_database;
}

inline bool
SQLDataSet::IsOpen()
{
  return m_open;
}

inline void
SQLDataSet::SetPrimaryTable(XString p_schema,XString p_tableName,XString p_alias /*= ""*/)
{
  m_primarySchema    = p_schema;
  m_primaryTableName = p_tableName;
  m_primaryAlias     = p_alias;
}

inline void
SQLDataSet::SetSequenceName(XString p_sequence)
{
  m_sequenceName = p_sequence;
}

inline int
SQLDataSet::GetNumberOfRecords()
{
  return (int)m_records.size();
}

inline int
SQLDataSet::GetNumberOfFields()
{
  return (int)m_names.size();
}

inline int
SQLDataSet::GetCurrentRecord()
{
  return m_current;
}

inline int
SQLDataSet::GetStatus()
{
  return m_status;
}

inline XString    
SQLDataSet::GetName()
{
  return m_name;
}

inline XString
SQLDataSet::GetPrimarySchema()
{
  return m_primarySchema;
}

inline XString
SQLDataSet::GetPrimaryTableName()
{
  return m_primaryTableName;
}

inline XString
SQLDataSet::GetPrimaryAlias()
{
  return m_primaryAlias;
}

inline void
SQLDataSet::SetPrimaryKeyColumn(XString p_name)
{
  m_primaryKey.push_back(p_name);
}

inline int
SQLDataSet::Current()
{
  return m_current;
}

inline void
SQLDataSet::SetUpdateColumns(const WordList& p_list)
{
  m_updateColumns = p_list;
}

inline XString
SQLDataSet::GetSelection()
{
  return m_selection;
}

inline XString
SQLDataSet::GetFromTables()
{
  return m_fromTables;
}

inline XString
SQLDataSet::GetWhereCondition()
{
  return m_whereCondition;
}

inline XString
SQLDataSet::GetGroupBy()
{
  return m_groupby;
}

inline XString
SQLDataSet::GetOrderBy()
{
  return m_orderby;
}

inline SQLFilterSet*
SQLDataSet::GetHavings()
{
  return m_havings;
}

inline void
SQLDataSet::SetCancelCallback(LPFN_CALLBACK p_cancelFunction)
{
  m_cancelFunction = p_cancelFunction;
}

inline void
SQLDataSet::SetStopIfNoColumns(bool p_stop)
{
  m_stopNoColumns = p_stop;
}

inline void
SQLDataSet::SetIsolation(bool p_isolation)
{
  m_isolation = p_isolation;
}

inline int
SQLDataSet::GetSkippedRecords()
{
  return m_skipRecords;
}

inline bool
SQLDataSet::GetLockForUpdate()
{
  return m_lockForUpdate;
}

inline void
SQLDataSet::SetLockForUpdate(bool p_lock)
{
  m_lockForUpdate = p_lock;
}

inline unsigned
SQLDataSet::GetLockWaitTime()
{
  return m_lockWaitTime;
}

inline void
SQLDataSet::SetLockWaitTime(unsigned p_wait)
{
  m_lockWaitTime = p_wait;
}

inline XString
SQLDataSet::GetLastInsertedSerial()
{
  return m_serial;
}

inline bool
SQLDataSet::GetKeepDuplicates()
{
  return m_keepDuplicates;
}

inline void 
SQLDataSet::SetKeepDuplicates(bool p_keep)
{
  m_keepDuplicates = p_keep;
}

// End of namespace
}