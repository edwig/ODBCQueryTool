////////////////////////////////////////////////////////////////////////
//
// File: SQLDataSet.cpp
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
#include "stdafx.h"
#include "SQLComponents.h"
#include "SQLDataSet.h"
#include "SQLDataType.h"
#include "SQLQuery.h"
#include "SQLVariantFormat.h"
#include "SQLInfoDB.h"
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{

// Names for saving datasets to XML in various languages
LPCTSTR dataset_names[LN_NUMLANG][NUM_DATASET_NAMES] =
{
  // LN_DUTCH
  { _T("Naam")          // DATASET_NAME
   ,_T("Structuur")     // DATASET_STRUCTURE
   ,_T("Veld")          // DATASET_FIELD
   ,_T("Record")        // DATASET_RECORD
   ,_T("Records")       // DATASET_RECORDS
   ,_T("id")            // DATASET_ID
   ,_T("type")          // DATASET_TYPE
   ,_T("typenaam")      // DATASET_TYPENAME
  },
  // LN_ENGLISH
  { _T("Name")          // DATASET_NAME
   ,_T("Structure")     // DATASET_STRUCTURE
   ,_T("Field")         // DATASET_FIELD
   ,_T("Record")        // DATASET_RECORD
   ,_T("Records")       // DATASET_RECORDS
   ,_T("ID")            // DATASET_ID
   ,_T("Type")          // DATASET_TYPE
   ,_T("TypeName")      // DATASET_TYPENAME
  },
  // LN_GERMAN
  { _T("Name")          // DATASET_NAME
   ,_T("Struktur")      // DATASET_STRUCTURE
   ,_T("Feld")          // DATASET_FIELD
   ,_T("Rekord")        // DATASET_RECORD
   ,_T("Rekords")       // DATASET_RECORDS
   ,_T("ID")            // DATASET_ID
   ,_T("Typ")           // DATASET_TYPE
   ,_T("Typename")      // DATASET_TYPENAME
  },
  // LN_FRENCH
  { _T("Nom")           // DATASET_NAME
   ,_T("Structure")     // DATASET_STRUCTURE
   ,_T("Domaine")       // DATASET_FIELD
   ,_T("Record")        // DATASET_RECORD
   ,_T("Records")       // DATASET_RECORDS
   ,_T("ID")            // DATASET_ID
   ,_T("Type")          // DATASET_TYPE
   ,_T("NomDeType")     // DATASET_TYPENAME
  },
};

// The DATASET class

SQLDataSet::SQLDataSet()
           :m_database(NULL)
{
}

SQLDataSet::SQLDataSet(XString p_name,SQLDatabase* p_database /*=NULL*/)
           :m_name(p_name)
           ,m_database(p_database)
{
}

SQLDataSet::~SQLDataSet()
{
  SQLDataSet::Close();
}

void
SQLDataSet::InitCounter()
{
  LARGE_INTEGER timing{ 0, 0 };
  QueryPerformanceFrequency(&timing);
  m_frequency = timing.QuadPart / CLOCKS_PER_SEC;
}

ULONG64
SQLDataSet::GetCounter()
{
  LARGE_INTEGER timing = { 0, 0 };
  QueryPerformanceCounter(&timing);
  return timing.QuadPart / m_frequency;
}

// Navigate in the records
int
SQLDataSet::First()
{
  return (m_current = (m_records.size() > 0) ? 0 : -1);
}

int  
SQLDataSet::Next()
{
  if(m_current < (int)m_records.size() - 1)
  {
    return ++m_current;
  }
  return -1;
}

int  
SQLDataSet::Prev()
{
  if(m_current > 0)
  {
    --m_current;
  }
  return m_current;
}

int  
SQLDataSet::Last()
{
  m_current = ((int)m_records.size() > 0) ? (int)m_records.size() - 1 : -1;
  return m_current;
}

int
SQLDataSet::Goto(int p_record)
{
  if(p_record >= 0 && p_record < (int)m_records.size())
  {
    return m_current = p_record;
  }
  return -1;
}

// Is it the first of the current set, or an empty set
bool
SQLDataSet::IsFirst()
{
  return (m_current < 1);
}

// Last of a dataset, or an empty set
bool
SQLDataSet::IsLast()
{
  return m_current == (int)(m_records.size() - 1);
}

void
SQLDataSet::Close()
{
  // Forget the data
  Forget(true);
  ResetFilters();

  // Forget all caches
  m_names.clear();
  m_types.clear();
  m_parameters.clear();
  m_primaryKey.clear();

  // Forget the query
  m_name.Empty();
  m_query.Empty();
  m_selection.Empty();
  m_fromTables.Empty();
  m_primaryTableName.Empty();
  m_primaryAlias.Empty();

  // Forget the cancellation
  m_cancelFunction = nullptr;

  // Reset the open status
  m_open = false;
}

// Forget the records
// BUT NOT REMOVING THE INFO, Append can be used!
bool         
SQLDataSet::Forget(bool p_force /*=false*/)
{
  if(p_force == false)
  {
    // Still changes to the database pending
    // Cannot simply forget all records
    if(m_status & (SQL_Updates | SQL_Deletions | SQL_Insertions))
    {
      return false;
    }
  }
  // Deleting all records
  for(auto& record : m_records)
  {
    record->Release();
  }
  // Forget the caches
  m_records.clear();
  m_objects.clear();
  // Set status to empty
  m_status  = SQL_Empty;
  m_current = -1;

  return true;
}

// Forget 1 record and primary is an INTEGER (Fast!!)
bool
SQLDataSet::ForgetObject(int p_primary,bool p_force /*=false*/)
{
  // Find the record to forget
  SQLRecord* record = FindObjectRecord(p_primary);
  if(!record)
  {
    return false;
  }
  return ForgetRecord(record,p_force);
}

// Forget 1 record and primary is a compound key (Slower)
bool
SQLDataSet::ForgetObject(const VariantSet& p_primary,bool p_force /*=false*/)
{
  // Find the record to forget
  SQLRecord* record = FindObjectRecord(p_primary);
  if(!record)
  {
    return false;
  }
  return ForgetRecord(record,p_force);
}

void
SQLDataSet::SetStatus(int m_add,int m_delete /*=0*/)
{
  m_status |= m_add;
  if(m_delete)
  {
    m_status &= ~m_delete;
  }
}

void 
SQLDataSet::SetParameter(const SQLParameter& p_parameter)
{
  // See if the parameter is already there
  for(unsigned int ind = 0;ind < m_parameters.size(); ++ind)
  {
    if(m_parameters[ind].m_name == p_parameter.m_name)
    {
      // Found it: set a new value
      m_parameters[ind].m_value = p_parameter.m_value;
      return;
    }
  }
  // New parameter
  m_parameters.push_back(p_parameter);
}

void 
SQLDataSet::SetParameter(const XString& p_name,const SQLVariant& p_value)
{
  // See if the parameter is already there
  for(unsigned int ind = 0;ind < m_parameters.size();++ind)
  {
    if(m_parameters[ind].m_name == p_name)
    {
      // Found it: set a new value
      m_parameters[ind].m_value = p_value;
      return;
    }
  }
  // New parameter
  SQLParameter par;
  par.m_name   = p_name;
  par.m_value  = p_value;
  m_parameters.push_back(par);
}

void
SQLDataSet::ResetFilters()
{
  if(m_filters && m_ownFilters)
  {
    delete m_filters;
  }
  m_filters    = nullptr;
  m_ownFilters = false;
}

// Set filters for a query
// Forgetting the previous set of filters
void
SQLDataSet::SetFilters(SQLFilterSet* p_filters)
{
  ResetFilters();
  m_filters = p_filters;
}

// Add filter to current set of filters
void
SQLDataSet::SetFilter(const SQLFilter& p_filter)
{
  if(!m_filters)
  {
    m_filters = new SQLFilterSet();
    m_ownFilters = true;
  }
  m_filters->AddFilter(p_filter);
}

// Set top <n> records selection
void
SQLDataSet::SetTopNRecords(int p_top,int p_skip /*=0*/)
{
  if(p_top > 0)
  {
    m_topRecords  = p_top;
    m_skipRecords = p_skip;
  }
}

void         
SQLDataSet::SetPrimaryKeyColumn(WordList& p_list)
{
  m_primaryKey.clear();
  for(auto& column : p_list)
  {
    m_primaryKey.push_back(column);
  }
}

SQLVariant*  
SQLDataSet::GetParameter(const XString& p_name)
{
  for(unsigned int ind = 0;ind < m_parameters.size(); ++ind)
  {
    if(m_parameters[ind].m_name == p_name)
    {
      return &(m_parameters[ind].m_value);
    }
  }
  return NULL;
}

// Getting the sequence name
XString
SQLDataSet::GetSequenceName()
{
  // Special sequence name set
  if(!m_sequenceName.IsEmpty())
  {
    return m_sequenceName;
  }
  // Mostly enough
  if(!m_primaryTableName.IsEmpty())
  {
    return m_primaryTableName + "_seq";
  }
  // Give up :-(
  return "";
}

void
SQLDataSet::SetQuery(XString& p_query)
{
  // Setting the total query supersedes these
  m_selection.Empty();
  m_fromTables.Empty();
  m_whereCondition.Empty();
  m_groupby.Empty();
  m_orderby.Empty();
  m_filters = nullptr;
  m_havings = nullptr;

  // Setting the query at once
  m_query = p_query;
}

void
SQLDataSet::SetSelection(XString p_selection)
{
  m_query.Empty();
  m_selection = p_selection;
}

void
SQLDataSet::SetFromTables(XString p_from)
{
  m_query.Empty();
  m_fromTables = p_from;
}

void
SQLDataSet::SetWhereCondition(XString p_condition)
{
  m_query.Empty();
  m_whereCondition = p_condition;
}

void
SQLDataSet::SetGroupBy(XString p_groupby)
{
  m_query.Empty();
  m_groupby = p_groupby;
}

void
SQLDataSet::SetOrderBy(XString p_orderby)
{
  m_query.Empty();
  m_orderby = p_orderby;
}

void
SQLDataSet::AddGroupby(XString p_property)
{
  if(!m_groupby.IsEmpty())
  {
    m_groupby += ", ";
  }
  m_groupby += p_property;
}

void
SQLDataSet::SetHavings(SQLFilterSet* p_havings)
{
  m_query.Empty();
  m_havings = p_havings;
}

// Maximum time alloted to the Open() method
void
SQLDataSet::SetQueryTime(int p_milliseconds)
{
  if(p_milliseconds >= 0)
  {
    m_queryTime = p_milliseconds;
  }
}

// Replace $name for the value of a parameter
// $ signs within 'string$chain' or "String$chain" can NOT be replaced
// This makes it possible to write queries like
// SELECT t.name
//       ,s.number
//       ,'Constant with an $ in it'
//   FROM table t
//       ,$parameter1 s
//  WHERE t.id   =    $parameter2
//    AND t.name LIKE $parameter3
// ;
XString
SQLDataSet::ParseQuery()
{
  bool inQuote = false;
  bool inAphos = false;
  XString query;
  int  pos = 0;

  while(pos < m_query.GetLength())
  {
    int c = m_query.GetAt(pos);
    if(c == '\'')
    {
      inAphos = !inAphos;
    }
    else if(c == '\"')
    {
      inQuote = !inQuote;
    }
    if(c == '$' && !inAphos && !inQuote)
    {
      XString parNaam;
      do 
      {
        c = m_query.GetAt(++pos);
        parNaam += (char) c;
      } 
      while (isalnum(c) || c == '_');
      parNaam = parNaam.Left(parNaam.GetLength() - 1);
      // Found parameter name
      SQLVariant* par = GetParameter(parNaam);
      if(par)
      {
        SQLVariantFormat form(par);
        query += form.FormatVariantForSQL(m_database);
      }
    }
    else
    {
      query += (char)c;
      ++pos;
    }
  }
  return query;
}

// Parse the selection
XString
SQLDataSet::ParseSelection(SQLQuery& p_query)
{
  XString sql(_T("SELECT "));
  
  sql += m_selection.IsEmpty() ? XString(_T("*")) : m_selection;
  sql += _T("\n  FROM ");

  if(!m_fromTables.IsEmpty())
  {
    sql += m_fromTables;
  }
  else
  {
    if(!m_primarySchema.IsEmpty())
    {
      sql += m_primarySchema + _T(".");
    }
    sql += m_primaryTableName;
    if(!m_primaryAlias.IsEmpty())
    {
      sql += _T(" ");
      sql += m_primaryAlias;
    }
    if(m_lockForUpdate && m_database)
    {
      sql += m_database->GetSQLInfoDB()->GetSelectForUpdateTableClause(m_lockWaitTime);
    }
  }
  int count = 0;
  int number = 0;
  ParameterSet::iterator it;
  for(it = m_parameters.begin();it != m_parameters.end(); ++it)
  {
    sql += (count++ == 0) ? _T("\n WHERE ") : _T("\n   AND ");
    sql += it->m_name;
    if(it->m_value.IsNULL())
    {
      sql += _T(" IS NULL");
    }
    else
    {
      sql += _T(" = ?");
      p_query.SetParameter(++number,&(it->m_value));
    }
  }

  return sql;
}

// Parse the filters (m_filters must be non-null)
XString
SQLDataSet::ParseFilters(SQLQuery& p_query,XString p_sql)
{
  XString query(p_sql);
  query.MakeUpper();
  query.Replace(_T("\t"),_T(" "));
  bool whereFound = m_query.Find(_T("WHERE ")) > 0;

  // Offset in the WHERE clause
  query += whereFound ? _T("\n   AND ") : _T("\n WHERE ");

  // Add all filters
  query += m_filters->ParseFiltersToCondition(p_query);

  return query;
}

// Construct the selection SQL for opening the dataset
// Getting the total query in effect
XString
SQLDataSet::GetSelectionSQL(SQLQuery& p_qry)
{
  XString sql(m_query);

  // If parameters, parse them
  if(!m_query.IsEmpty())
  {
    if(m_parameters.size())
    {
      sql = ParseQuery();
    }
  }
  else
  {
    // Replace empty query from selection columns
    // If no selection columns: does a "SELECT *"
    sql = ParseSelection(p_qry);
  }

  // Apply all the filters
  if (m_filters && !m_filters->Empty())
  {
    sql = ParseFilters(p_qry, sql);
  }
  else if(!m_whereCondition.IsEmpty())
  {
    if(m_parameters.size())
    {
      sql += _T("\n   AND ") + m_whereCondition;
    }
    else
    {
      sql += _T("\n WHERE ") + m_whereCondition;
    }
  }

  if(!m_groupby.IsEmpty())
  {
    sql += _T("\n GROUP BY ") + m_groupby;
  }

  if(m_havings && !m_havings->Empty())
  {
    sql += _T("\n HAVING ");
    sql += m_havings->ParseFiltersToCondition(p_qry);
  }

  if(!m_orderby.IsEmpty())
  {
    sql += _T("\n ORDER BY ") + m_orderby;
  }

  if(m_lockForUpdate && m_database)
  {
    sql = m_database->GetSQLInfoDB()->GetSelectForUpdateTrailer(sql,m_lockWaitTime);
  }

  return sql;
}

bool
SQLDataSet::Open()
{
  bool result = false;

  if(m_query.IsEmpty() && m_selection.IsEmpty())
  {
    return false;
  }
  if(m_open)
  {
    Close();
  }
  // Test if possibly modifiable if primary table name and key are givven
  bool modifiable = false;
  if(!m_primaryTableName.IsEmpty() && m_primaryKey.size())
  {
    modifiable = true;
  }
  try
  {
    SQLQuery qry(m_database);
    SQLTransaction trans(m_database,m_name);
    ULONG64 begin = 0;

    // Set a trap to stop an action that will take too much time...
    if(m_cancelFunction)
    {
      (*m_cancelFunction)(qry.GetStatementHandle());
    }

    // Get the select query
    XString query = GetSelectionSQL(qry);

    // Apply top <N> records selection
    if(m_topRecords)
    {
      query = m_database->GetSQLInfoDB()->GetSQLTopNRows(query,m_topRecords,m_skipRecords);
    }

    // Probably record the begin time
    if(m_queryTime)
    {
      // Use high-performance counter to get the clock ticks in long-range resolution
      InitCounter();
      begin = GetCounter();
    }

    // Do the SELECT query
    qry.DoSQLStatement(query);
    if (m_stopNoColumns && qry.GetNumberOfColumns() == 0)
    {
      return false;
    }

    // Read the names of the fields
    ReadNames(qry);
    ReadTypes(qry);

    // Read all the records
    while(qry.GetRecord())
    {
      ReadRecordFromQuery(qry,modifiable);

      // See if we can read a next record
      if(begin && ((GetCounter() - begin) > (ULONG64)m_queryTime))
      {
        throw StdException(_T("Querytime exceeded"));
      }
    }
    // Reached the end: we are OPEN!
    m_open = true;
    result = true;

    trans.Commit();
  }
  catch(StdException& er)
  {
    Close();
    throw StdException(er.GetErrorMessage());
  }
  if(m_records.size())
  {
    m_current = 0;
    m_status |= SQL_Selections;
  }
  return result;
}

bool
SQLDataSet::Open(SQLQuery& p_query)
{
  // Check that we *CAN* query
  if(m_query.IsEmpty() && m_selection.IsEmpty())
  {
    return false;
  }
  if(m_open)
  {
    Close();
  }
  // Test if possibly modifiable if primary table name and key are given
  bool modifiable = false;
  if(!m_isolation && !m_primaryTableName.IsEmpty() && m_primaryKey.size())
  {
    modifiable = true;
  }
  try
  {
    ULONG64 begin = 0;

    // Set a trap to stop an action that will take too much time...
    if(m_cancelFunction)
    {
      (*m_cancelFunction)(p_query.GetStatementHandle());
    }

    // Get the select query
    XString sql = GetSelectionSQL(p_query);

    // Apply top <N> records selection
    if(m_topRecords && !m_isolation)
    {
      sql = m_database->GetSQLInfoDB()->GetSQLTopNRows(sql,m_topRecords,m_skipRecords);
    }

    // Probably record the begin time
    if(m_queryTime)
    {
      // Use high-performance counter to get the clock ticks in long-range resolution
      InitCounter();
      begin = GetCounter();
    }

    // Do the SELECT query
    p_query.DoSQLStatement(sql);
    if(m_stopNoColumns && p_query.GetNumberOfColumns() == 0)
    {
      return false;
    }

    // Read the names of the fields
    ReadNames(p_query);
    ReadTypes(p_query);

    // Read all the records
    long records = 0;
    while(p_query.GetRecord())
    {
      ReadRecordFromQuery(p_query,modifiable);

      // See if we can read a next record
      if(begin && ((GetCounter() - begin) > (ULONG64)m_queryTime))
      {
        throw StdException(_T("Querytime exceeded"));
      }
      ++records;
      if(m_isolation)
      {
        ++m_skipRecords;
        if(records == m_topRecords)
        {
          break;
        }
      }
    }
    // Reached the end: we are OPEN!
    m_open = true;
  }
  catch(StdException& er)
  {
    Close();
    throw StdException(er.GetErrorMessage());
  }
  if(m_records.size())
  {
    m_current = 0;
    m_status |= SQL_Selections;
  }
  return m_open;
}

// Append (read extra) into the dataset
// Precondition: Dataset must already be opened by 'Open()'
// Precondition: Different parameter values set to read different data
// Precondition: Results must be of same result set (columns and datatypes)
bool 
SQLDataSet::Append()
{
  bool result = false;

  // See if already opened
  if(!m_open)
  {
    return false;
  }
  // Test if possibly modifiable if primary table name and key are given
  bool modifiable = false;
  if(!m_primaryTableName.IsEmpty() && m_primaryKey.size())
  {
    modifiable = true;
  }
  // Set last record to last current, setting m_current
  Last();
  // Remember size before reading
  size_t sizeBefore = m_records.size();

  try
  {
    SQLQuery qry(m_database);
    SQLTransaction trans(m_database,m_name);
    ULONG64 begin = 0;

    // Get the select query
    XString query = GetSelectionSQL(qry);

    // Apply top <N> records selection
    if(m_topRecords)
    {
      query = m_database->GetSQLInfoDB()->GetSQLTopNRows(query,m_topRecords,m_skipRecords);
    }

    // Probably record the begin time
    if(m_queryTime)
    {
      // Use high-performance counter to get the clock ticks in long-range resolution
      InitCounter();
      begin = GetCounter();
    }

    // Do the SELECT query
    qry.DoSQLStatement(query);

    // Names and types must be the same as previous queries
    CheckNames(qry);
    CheckTypes(qry);

    while(qry.GetRecord())
    {
      ReadRecordFromQuery(qry,modifiable,true);

      // See if we can read a next record
      if(begin && ((GetCounter() - begin) > (ULONG64)m_queryTime))
      {
        throw StdException(_T("Querytime exceeded"));
      }
    }
    // Legal 0 or more records
    result = true;
    trans.Commit();
  }
  catch(StdException& er)
  {
    ReThrowSafeException(er);
    Close();
    throw StdException(er.GetErrorMessage());
  }
  // Goto the first freshly read record
  if(m_records.size() > sizeBefore)
  {
    m_status |= SQL_Selections;
    Next();
  }
  return result;
}

// Appending to an isolated set is something like reading the next page
// Forgetting all our previous records, and read in a new set from the RDBMS
bool
SQLDataSet::Append(SQLQuery& p_query)
{
  bool result = false;

  if(!m_open || !m_isolation)
  {
    return false;
  }

  // Clean out the previous records
  Forget(true);

  // Test if possibly modifiable if primary table name and key are givven
  try
  {
    // Read all the records
    long records = 0;
    while(p_query.GetRecord())
    {
      ReadRecordFromQuery(p_query,false,false);

      ++records;
      ++m_skipRecords;
      if(records == m_topRecords)
      {
        break;
      }
    }
    // Reached the end: we are OPEN!
    result = true;
  }
  catch(StdException& er)
  {
    Close();
    throw StdException(er.GetErrorMessage());
  }
  if(m_records.size())
  {
    m_current = 0;
    m_status |= SQL_Selections;
  }
  return result;
}

// Read in a record from a SQLQuery
bool
SQLDataSet::ReadRecordFromQuery(SQLQuery& p_query,bool p_modifiable,bool p_append /*=false*/)
{
  // Make a new record
  SQLRecord* record = new SQLRecord(this,p_modifiable);

  // Get all the columns of the record
  int num = p_query.GetNumberOfColumns();
  for(int ind = 1; ind <= num; ++ind)
  {
    const SQLVariant* var = p_query.GetColumn(ind);
    record->AddField(var);
  }

  // Construct the primary key (possibly from more than 1 field)
  XString key = MakePrimaryKey(record);

  if(key.IsEmpty())
  {
    // New record, no primary key info, just keep it
    m_records.push_back(record);
  }
  else
  {
    bool extra = true;
    if(p_append)
    {
      ObjectMap::iterator it = m_objects.find(key);
      extra = (it == m_objects.end());
    }
    if(extra)
    {
      // New record: keep it along with the primary key info
      m_records.push_back(record);
      int recnum = (int)m_records.size() - 1;
      m_objects.insert(std::make_pair(key,recnum));
    }
    else
    {
      // We already had the record
      delete record;
    }
  }
  return true;
}

// Make a primary key record
XString
SQLDataSet::MakePrimaryKey(const SQLRecord* p_record)
{
  XString key;
  XString value;

  for(const auto& field : m_primaryKey)
  {
    const SQLVariant* var = p_record->GetField(field);
    if (var != nullptr)
    {
      var->GetAsString(value);

      key += value;
      key += _T("\0x1E");  // ASCII UNIT Separator
    }
  }
  return key;
}

XString
SQLDataSet::MakePrimaryKey(const VariantSet& p_primary)
{
  XString key;
  XString value;

  for(const auto val : p_primary)
  {
    val->GetAsString(value);

    key += value;
    key += _T("\0x1E");  // ASCII UNIT Separator
  }
  return key;
}

// Get all the columns of the record
void
SQLDataSet::ReadNames(SQLQuery& qr)
{
  XString naam;
  int num = qr.GetNumberOfColumns();
  for(int ind = 1; ind <= num; ++ind)
  {
    qr.GetColumnName(ind,naam);
    m_names.push_back(naam);
  }
}

// Get all  the types of the record
void
SQLDataSet::ReadTypes(SQLQuery& qr)
{
  int num = qr.GetNumberOfColumns();
  for(int ind = 1; ind <= num; ++ind)
  {
    int type = qr.GetColumnType(ind);
    m_types.push_back(type);
  }
}

// Check that all names are the same
void
SQLDataSet::CheckNames(SQLQuery& p_query)
{
  XString name;
  int num = p_query.GetNumberOfColumns();
  for(int ind = 1; ind <= num; ++ind)
  {
    p_query.GetColumnName(ind,name);
    if(m_names[(size_t)ind-1].CompareNoCase(name))
    {
      throw StdException(_T("Append needs exactly the same query column names"));
    }
  }
}

// Check that all the types are the same
void
SQLDataSet::CheckTypes(SQLQuery& p_query)
{
  int num = p_query.GetNumberOfColumns();
  for(int ind = 1; ind <= num; ++ind)
  {
    int type = p_query.GetColumnType(ind);
    if(m_types[(size_t)ind-1] != type)
    {
      throw StdException(_T("Append needs exactly the same datatypes for the query columns."));
    }
  }
}

// Check that all datatypes are the same
SQLRecord*
SQLDataSet::GetRecord(int p_recnum)
{
  if(p_recnum >= 0 && p_recnum < (int)m_records.size())
  {
    return m_records[p_recnum];
  }
  return NULL;
}

// Find the object record of an integer primary key
int
SQLDataSet::FindObjectRecNum(int p_primary)
{
  XString key;
  key.Format(_T("%d\0x1E"),p_primary);
  ObjectMap::iterator it = m_objects.find(key);
  if(it != m_objects.end())
  {
    return it->second;
  }
  return -1;
}

// Find the object record of an integer primary key
SQLRecord*
SQLDataSet::FindObjectRecord(int p_primary)
{
  XString key;
  key.Format(_T("%d\0x1E"),p_primary);
  ObjectMap::iterator it = m_objects.find(key);
  if(it != m_objects.end())
  {
    return m_records[it->second];
  }
  return NULL;
}

// If your primary is a compound key or not INTEGER (Slower)
int
SQLDataSet::FindObjectRecNum(const VariantSet& p_primary)
{
  if(!GetPrimaryKeyInfo())
  {
    // No primary key, cannot do the find
    return -1;
  }
  if(!CheckPrimaryKeyColumns())
  {
    // Not all primary key columns are present
    return -1;
  }

  XString key = MakePrimaryKey(p_primary);

  ObjectMap::iterator it = m_objects.find(key);
  if(it != m_objects.end())
  {
    return it->second;
  }
  return -1;
}

// If your primary is a compound key or not INTEGER (Slower)
SQLRecord*
SQLDataSet::FindObjectRecord(const VariantSet& p_primary)
{
  if(!GetPrimaryKeyInfo())
  {
    // No primary key, cannot do the find
    return nullptr;
  }
  if(!CheckPrimaryKeyColumns())
  {
    // Not all primary key columns are present
    return nullptr;
  }

  XString key = MakePrimaryKey(p_primary);

  ObjectMap::iterator it = m_objects.find(key);
  if(it != m_objects.end())
  {
    return m_records[it->second];
  }
  return nullptr;
}

// Finding an object through a filter set
// Finds the first object. In case of an unique record, it will be the only one
SQLRecord*
SQLDataSet::FindObjectFilter(bool p_primary /*=false*/)
{
  SQLRecord* record = nullptr;

  // Optimize for network databases
  if(p_primary && m_filters->Size() == 1)
  {
    const SQLFilter* filter = m_filters->GetFilters().front();
    const SQLVariant* prim  = filter->GetValue();
    if(filter->GetOperator() == OP_Equal && prim->GetDataType() == SQL_C_SLONG )
    {
      return FindObjectRecord(prim->GetAsSLong());
    }
  }

  // Walk the chain of records
  for(auto& rec : m_records)
  {
    record = rec;
    // Walk the chain of filters
    for(auto& filt : m_filters->GetFilters())
    {
      if(! filt->MatchRecord(record))
      {
        record = nullptr;
        break;
      }
    }
    // Result reached at first matching record
    if(record)
    {
      return record;
    }
  }
  return nullptr;
}

// Finding a set of records through a filter set
// Searches the complete recordset for all matches
// Caller must delete the resulting set!!
RecordSet* 
SQLDataSet::FindRecordSet()
{
  RecordSet* records = new RecordSet();

  // Walk the chain of records
  for(auto& rec : m_records)
  {
    SQLRecord* record = rec;
    // Walk the chain of filters
    for(auto& filt : m_filters->GetFilters())
    {
      if(! filt->MatchRecord(record))
      {
        record = nullptr;
        break;
      }
    }
    // If record found, keep it
    if(record)
    {
      records->push_back(record);
    }
  }
  // Return complete recordset
  return records;
}

// Get a fieldname
XString    
SQLDataSet::GetFieldName(int p_num)
{
  if(p_num >= 0 && (unsigned)p_num < m_names.size())
  {
    return m_names[p_num];
  }
  return "";
}

int
SQLDataSet::GetFieldType(int p_num)
{
  if(p_num >= 0 && (unsigned)p_num < m_types.size())
  {
    return m_types[p_num];
  }
  return -1;
}

// Get a field number
int
SQLDataSet::GetFieldNumber(XString p_name)
{
  for(unsigned int ind = 0; ind < m_names.size(); ++ind)
  {
    if(p_name.CompareNoCase(m_names[ind]) == 0)
    {
      return ind;
    }
  }
  return -1;
}

// Get a field of the current record
SQLVariant*
SQLDataSet::GetCurrentField(int p_num)
{
  if(m_current >= 0 && m_current < (int)m_records.size())
  {
    if(p_num >= 0 && p_num < (int)m_names.size())
    {
      return m_records[m_current]->GetField(p_num);
    }
  }
  return NULL;
}

// Insert new record
// If the set was previously closed, it is now OPEN for transactions
// because there is at least one record in the dataset
SQLRecord* 
SQLDataSet::InsertRecord()
{
  SQLRecord* record = new SQLRecord(this,true);
  m_records.push_back(record);
  m_current = (int)(m_records.size() - 1);
  m_status |= SQL_Insertions;
  m_open    = true;
  return record;
}

// Insert new field in new record
int
SQLDataSet::InsertField(XString p_name,const SQLVariant* p_value)
{
  if(m_current >= 0 && m_current < (int)m_records.size())
  {
    m_names.push_back(p_name);
    m_types.push_back(p_value->GetDataType());

    SQLRecord* record = m_records[m_current];
    return record->AddField(p_value,true);
  }
  // Internal programming error
  return -1;
}

// Set a field value in the current record
bool
SQLDataSet::SetField(const XString& p_name,const SQLVariant* p_value,int p_mutationID /*=0*/)
{
  int num = GetFieldNumber(p_name);
  if(num >= 0)
  {
    return SetField(num,p_value,p_mutationID);
  }
  return false;
}

// Set a field value in the current record
bool
SQLDataSet::SetField(int p_num,const SQLVariant* p_value,int p_mutationID /*=0*/)
{
  if(m_current >= 0)
  {
    if(m_records[m_current]->SetField(p_num,p_value,p_mutationID))
    {
      m_status |= SQL_Updates;
      return true;
    }
  }
  return false;
}

// Cancel the mutations of this mutation ID
void
SQLDataSet::CancelMutation(int p_mutationID)
{
  bool mutated = false; 
  for(unsigned ind = 0;ind < m_records.size();++ind)
  {
    if(m_records[ind]->CancelMutation(p_mutationID))
    {
      mutated = true;
    }
  }
  if(!mutated)
  {
    // Remove the mutated status (leaving empty and selections alone)
    m_status &= ~(SQL_Updates | SQL_Insertions | SQL_Deletions);
  }
}

// Calculate aggregate functions
// COUNTS NULL VALUES AS 0 for the mean-result
int
SQLDataSet::Aggregate(int p_num,AggregateInfo& p_info)
{
  unsigned int total   = (int)m_records.size();
  for(unsigned int ind = 0;ind < total; ++ind)
  {
    const SQLVariant* var = m_records[ind]->GetField(p_num);
    if(var && var->IsNULL() == false)
    {
      double waarde = var->GetAsDouble();
      if(p_info.m_max < waarde)
      {
        p_info.m_max = waarde;
      }
      if(p_info.m_min > waarde)
      {
        p_info.m_min = waarde;
      }
      p_info.m_sum += waarde;
      p_info.m_mean = p_info.m_sum / total;
    }
  }
  // Return the number of records processed
  return total;
}

//////////////////////////////////////////////////////////////////////////
//
// THE SYNCHRONIZATION PROCESS
//
//////////////////////////////////////////////////////////////////////////

// Insert / Update / delete records from the database
bool
SQLDataSet::Synchronize(int p_mutationID /*=0*/,bool p_throw /*=false*/)
{
  // Needs the primary table name of the dataset
  if(m_primaryTableName.IsEmpty())
  {
    return false;
  }
  // Check if we have mutations
  if((m_status & (SQL_Record_Insert | SQL_Record_Deleted | SQL_Record_Updated)) == 0)
  {
    // Nothing to do: all OK.
    return true;
  }
  // Check preliminary conditions
  if(m_primaryTableName.IsEmpty())
  {
    // Needs the primary table name of the dataset
    return false;
  }
  if(m_status & (SQL_Record_Deleted | SQL_Record_Updated))
  {
    if(!GetPrimaryKeyInfo()     ||    // Needs primary key info for doing updates/deletes
       !CheckPrimaryKeyColumns())     // Needs all of the primary key columns
    {
      // No primary key, cannot do updates/deletes
      return false;
    }
  }

  // Save status before a possible throw
  int oldStatus = m_status;

  try
  {
    // Transaction on the stack for all mutations
    // Defer all constraint checking to the 'real' commit
    SQLTransaction trans(m_database,m_name);
    trans.SetTransactionDeferred();

    // Do all operations or throw
    if(m_status & SQL_Record_Deleted)
    {
      Deletes(p_mutationID);
    }
    if(m_status & SQL_Record_Updated)
    {
      Updates(p_mutationID);
    }
    if(m_status & SQL_Record_Insert)
    {
      Inserts(p_mutationID);
    }

    // Commit the changes to the database
    trans.Commit();

    // After the commit we throw away our changes
    Reduce(p_mutationID);
  }
  catch(StdException& er)
  {
    ReThrowSafeException(er);
    // Automatic rollback will be done now
    XString error = _T("Database synchronization stopped: ") + er.GetErrorMessage();
    if(p_throw)
    {
      throw StdException(error);
    }
    m_database->LogPrint(error);
    // Restore original status of the dataset, reduce never done
    m_status = oldStatus;
    return false;
  }
  // Ready
  return true;
}

// Getting the primary key for a table
bool
SQLDataSet::GetPrimaryKeyInfo()
{
  if(m_primaryKey.size() == 0)
  {
    SQLInfoDB* info = m_database->GetSQLInfoDB();
    MPrimaryMap primaries;
    XString     constraintName;
    if(info->GetPrimaryKeyInfo(m_primaryTableName,constraintName,primaries) == false)
    {
      // No primary key given, and cannot get it from the ODBC driver
      return false;
    }
    // Remember the primary key info
    for(auto& key : primaries)
    {
      m_primaryKey.push_back(key.m_columnName);
    }
  }
  return m_primaryKey.size() > 0;
}

// Check that all primary key columns are in the dataset
bool
SQLDataSet::CheckPrimaryKeyColumns()
{
  NamenMap::iterator key,field;

  for(key = m_primaryKey.begin();key != m_primaryKey.end();++key)
  {
    bool foundKeypart = false;
    for(field = m_names.begin();field != m_names.end();++field)
    {
      if(field->CompareNoCase(*key) == 0)
      {
        foundKeypart = true;
        break;
      }
    }
    if(foundKeypart == false)
    {
      return false;
    }
  }
  // All key parts found
  return true;
}

void
SQLDataSet::Deletes(int p_mutationID)
{
  RecordSet::iterator it = m_records.begin();
  SQLQuery query(m_database);
  int total   = 0;
  int deletes = 0;

  // Loop through all the records
  while(it != m_records.end())
  {
    XString sql;
    SQLRecord* record = *it;
    if(record->GetStatus() & SQL_Record_Deleted)
    {
      ++total;
      MutType type = record->MixedMutations(p_mutationID);
      switch(type)
      {
        case MUT_OnlyOthers: ++it;  // do nothing with record
                             break;
        case MUT_Mixed:      throw StdException(_T("Mixed mutations"));
        case MUT_NoMutation: // Fall through: Remove record
        case MUT_MyMutation: sql = GetSQLDelete(&query,record);
                             query.DoSQLStatement(sql);
                             // Delete this record, continuing to the next
//                              delete record;
//                              it = m_records.erase(it);
                             ForgetRecord(record,true);
                             ++deletes;
                             break;
      }
      // Iterator cannot continue after last removed item
      if(m_records.empty())
      {
        break;
      }
    }
    else
    {
      ++it;
    }
  }

  // Adjust the current record if necessary
  if(m_current >= (int)m_records.size())
  {
    m_current = (int)m_records.size() - 1;
  }

  // If we did all records, no more deletes are present
  if(total == deletes)
  {
    m_status &= ~SQL_Deletions;
  }
}

void
SQLDataSet::Updates(int p_mutationID)
{
  RecordSet::iterator it;
  SQLQuery query(m_database);
  int total  = 0;
  int update = 0;

  for(it = m_records.begin();it != m_records.end();++it)
  {
    SQLRecord* record = *it;
    if(record->GetStatus() & SQL_Record_Updated)
    {
      ++total;
      XString sql;
      MutType type = record->MixedMutations(p_mutationID);
      switch(type)
      {
        case MUT_NoMutation: // Fall through: do nothing
        case MUT_OnlyOthers: break;
        case MUT_Mixed:      throw StdException(_T("Mixed mutations"));
        case MUT_MyMutation: sql = GetSQLUpdate(&query,record);
                             query.DoSQLStatement(sql);
                             ++update;
                             break;
      }
    }
  }

  // If we did all records, no more updates are present
  if(total == update)
  {
    m_status &= ~SQL_Updates;
  }
}

void
SQLDataSet::Inserts(int p_mutationID)
{
  RecordSet::iterator it;
  SQLQuery query(m_database);
  int total  = 0;
  int insert = 0;

  for(it = m_records.begin();it != m_records.end();++it)
  {
    SQLRecord* record = *it;
    if(record->GetStatus() & SQL_Record_Insert)
    {
      ++total;
      XString sql;
      MutType type = record->MixedMutations(p_mutationID);
      switch(type)
      {
        case MUT_NoMutation: // Fall through: Do nothing
        case MUT_OnlyOthers: break;
        case MUT_Mixed:      throw StdException(_T("Mixed mutations"));
        case MUT_MyMutation: sql = GetSQLInsert(&query,record);
                             query.DoSQLStatement(sql);
                             ++insert;
                             break;
      }
      // For an active generator, fill in the retrieved value
      if(record->GetGenerator() >= 0 && !m_serial.IsEmpty())
      {
        int value = m_database->GetSQL_EffectiveSerial(m_serial);
        SQLVariant val(value);
        record->SetField(record->GetGenerator(),&val,0);
      }
    }
  }

  // If we did all records, no more inserts are present
  if(total == insert)
  {
    m_status &= ~SQL_Insertions;
  }
}

// Throws away my changes from the dataset
// Call only after all database synchronization has been done!
void
SQLDataSet::Reduce(int p_mutationID)
{
  RecordSet::iterator it;
  for(it = m_records.begin(); it != m_records.end(); ++it)
  {
    SQLRecord* record = *it;
    if(record->GetStatus() & (SQL_Record_Insert | SQL_Record_Deleted | SQL_Record_Updated))
    {
      MutType type = record->MixedMutations(p_mutationID);
      switch(type)
      {
        case MUT_NoMutation: break;
        case MUT_OnlyOthers: break;
        case MUT_Mixed:      break;
        case MUT_MyMutation: record->Reduce();
                             break;
      }
    }
  }
}

// In case synchronize doesn't work, ask for mixed mutations
int
SQLDataSet::AllMixedMutations(MutationIDS& p_list,int p_mutationID)
{
  int total = 0;
  RecordSet::iterator it;
  for(it = m_records.begin(); it != m_records.end(); ++it)
  {
    SQLRecord* record = *it;
    total += record->AllMixedMutations(p_list,p_mutationID);
  }
  return total;
}

XString
SQLDataSet::GetSQLDelete(SQLQuery* p_query,const SQLRecord* p_record)
{
  // New set of parameters
  p_query->ResetParameters();

  XString sql(_T("DELETE FROM ") + m_primaryTableName + _T("\n"));
  int parameter = 1;
  sql += GetWhereClause(p_query,p_record,parameter);
  return sql;
}

XString
SQLDataSet::GetSQLUpdate(SQLQuery* p_query,const SQLRecord* p_record)
{
  int parameter = 1;
  XString sql(_T("UPDATE ") + m_primaryTableName + _T("\n"));
  WordList::iterator it;

  // New set of parameters
  p_query->ResetParameters();

  // Check for all fields
  bool first = true;
  for(unsigned ind = 0;ind < m_names.size(); ++ind)
  {
    // Filter for specific columns that are allowed to be updated
    bool update = true;
    if(!m_updateColumns.empty())
    {
      update = false;
      for(auto& column : m_updateColumns)
      {
        if (m_names[ind].CompareNoCase(column) == 0)
        {
          update = true;
          break;
        }
      }
    }

    // If we may update, and only if the field in the record IS updated!
    // This reduces the number of columns to write back to the database!
    if(update && p_record->IsModified(ind))
    {
      SQLVariant* value = p_record->GetField(ind);

      sql += first ? _T("   SET ") : _T("      ,");
      sql += m_names[ind];
      if(value->IsNULL())
      {
        sql += _T(" = NULL\n");
      }
      else
      {
        sql += _T(" = ?\n");
        p_query->SetParameter(parameter++,value);
      }
      first = false;
    }
  }
  // Adding the WHERE clause
  sql += GetWhereClause(p_query,p_record,parameter);

  return sql;
}

XString
SQLDataSet::GetSQLInsert(SQLQuery* p_query,const SQLRecord* p_record)
{
  int parameter = 1;
  XString sql(_T("INSERT INTO ") + m_primaryTableName);

  XString fields(_T("("));
  XString params(_T("("));

  // New set of parameters
  p_query->ResetParameters();

  // Do for all fields in the record
  for(unsigned ind = 0;ind < m_names.size(); ++ind)
  {
    SQLVariant* value = p_record->GetField(ind);
    if((int)ind == p_record->GetGenerator() && value->IsEmpty())
    {
      fields  += m_names[ind] + _T(",");
      m_serial = m_database->GetSQL_GenerateSerial(m_primaryTableName,m_sequenceName);
      params  += m_serial;
      params  += _T(",");
    }
    else
    {
      if(value->IsNULL() == false)
      {
        fields += m_names[ind] + _T(",");
        params += _T("?,");
        p_query->SetParameter(parameter++,value);
      }
    }
  }
  // Closing
  fields.TrimRight(',');
  params.TrimRight(',');
  fields += _T(")\n");
  params += _T(")");
  // Put it together
  sql += fields;
  sql += _T("VALUES ");
  sql += params;

  return sql;
}

XString
SQLDataSet::GetWhereClause(SQLQuery* p_query,const SQLRecord* p_record,int& p_parameter)
{
  XString sql(_T(" WHERE "));

  bool more = false;
  for(unsigned ind = 0;ind < m_primaryKey.size();++ind)
  {
    if(more)
    {
      sql += _T("\n   AND ");
    }
    more = true;
    int column = GetFieldNumber(m_primaryKey[ind]);
    sql += m_primaryKey[ind];
    SQLVariant* value = p_record->GetField(column);
    if(value->IsNULL())
    {
      sql += _T(" = NULL");
    }
    else
    {
      sql += _T(" = ?");
      p_query->SetParameter(p_parameter++,value);
    }
  }
  return sql;
}

//////////////////////////////////////////////////////////////////////////
//
// Store in XML format
//
//////////////////////////////////////////////////////////////////////////

bool
SQLDataSet::XMLSave(XString p_filename,XString p_name,Encoding p_encoding /*= Encoding::UTF8*/)
{
  XMLMessage msg;
  msg.SetRootNodeName(p_name);

  XMLSave(&msg,msg.GetRoot());
  return msg.SaveFile(p_filename,p_encoding);
}

bool
SQLDataSet::XMLLoad(XString p_filename)
{
  XMLMessage msg;
  if(msg.LoadFile(p_filename))
  {
    XMLElement* root = msg.GetRoot();
    if(root)
    {
      XMLLoad(&msg,root);
      return true;
    }
  }
  return false;
}

void
SQLDataSet::XMLSave(XMLMessage* p_msg,XMLElement* p_dataset)
{
                          p_msg->AddElement(p_dataset,dataset_names[g_defaultLanguage][DATASET_NAME],XDT_String,m_name);
  XMLElement* structure = p_msg->AddElement(p_dataset,dataset_names[g_defaultLanguage][DATASET_STRUCTURE],XDT_String,"");
  XString nameField = dataset_names[g_defaultLanguage][DATASET_FIELD];

  // Add record structure
  const SQLRecord* record = GetRecord(0);
  if(record)
  {
    for(unsigned int ind = 0;ind < m_names.size(); ++ind)
    {
      XString fieldname = GetFieldName(ind);
      const SQLVariant* var = record->GetField(ind);
      int type = var->GetDataType();

      XMLElement* field = p_msg->AddElement(structure,nameField,XDT_String,fieldname);
      p_msg->SetAttribute(field,dataset_names[g_defaultLanguage][DATASET_ID],(int)ind);
      p_msg->SetAttribute(field,dataset_names[g_defaultLanguage][DATASET_TYPE],type);
      p_msg->SetAttribute(field,dataset_names[g_defaultLanguage][DATASET_TYPENAME],SQLDataType::FindDatatype(type));
    }
  }
  else if(m_types.size() > 0)
  {
    for(unsigned int ind = 0;ind < m_names.size(); ++ind)
    {
      XString fieldname = GetFieldName(ind);
      int type = m_types[ind];
      SQLVariant var;

      XMLElement* field = p_msg->AddElement(structure,nameField,XDT_String,fieldname);
      p_msg->SetAttribute(field,dataset_names[g_defaultLanguage][DATASET_ID],(int)ind);
      p_msg->SetAttribute(field,dataset_names[g_defaultLanguage][DATASET_TYPE],type);
      p_msg->SetAttribute(field,dataset_names[g_defaultLanguage][DATASET_TYPENAME],SQLDataType::FindDatatype(type));
    }
  }


  // Add records of the dataset
  XMLElement* records = p_msg->AddElement(p_dataset,dataset_names[g_defaultLanguage][DATASET_RECORDS],XDT_String,"");
  for(unsigned int ind = 0;ind < m_records.size(); ++ind)
  {
    m_records[ind]->XMLSave(p_msg,records);
  }
}

void
SQLDataSet::XMLLoad(XMLMessage* p_msg,XMLElement* p_dataset,const LONG* p_abort /*=nullptr*/)
{
  XMLElement* structur = p_msg->FindElement(p_dataset,dataset_names[g_defaultLanguage][DATASET_STRUCTURE],false);
  XMLElement* records  = p_msg->FindElement(p_dataset,dataset_names[g_defaultLanguage][DATASET_RECORDS],  false);
  if(structur == NULL) throw StdException(_T("Structure part missing in the XML dataset.") + m_name);
  if(records  == NULL) throw StdException(_T("Records part missing in the XML dataset")    + m_name);

  // Read the structure
  XMLElement* field = p_msg->GetElementFirstChild(structur);
  while(field)
  {
    if(p_abort != nullptr && *p_abort > 0)
    {
      return;
    }
    // Remember the name of the field
    XString name = field->GetValue();
    m_names.push_back(name);
    // Datatype of the field
    int type = p_msg->GetAttributeInteger(field,dataset_names[g_defaultLanguage][DATASET_TYPE]);
    m_types.push_back(type);
    // Next field
    field = p_msg->GetElementSibling(field);
  }

  // Read records
  XMLElement* record = p_msg->GetElementFirstChild(records);
  while(record)
  {
    if(p_abort != nullptr && *p_abort > 0)
    {
      return;
    }
    SQLRecord* rec = InsertRecord();
    rec->XMLLoad(p_msg,record);
    // Next record
    record = p_msg->GetElementSibling(record);
  }
}

//////////////////////////////////////////////////////////////////////////
//
// FORGETTING ABOUT A RECORD
//
//////////////////////////////////////////////////////////////////////////

// Record pointer cannot be a nullptr
bool
SQLDataSet::ForgetRecord(SQLRecord* p_record,bool p_force)
{
  if(p_force == false)
  {
    if(p_record->GetStatus() & (SQL_Record_Insert | SQL_Record_Updated | SQL_Record_Deleted))
    {
      // Do the synchronization first!
      return false;
    }
  }
  RecordSet::iterator it = find(m_records.begin(),m_records.end(),p_record);
  if(it != m_records.end())
  {
    // Remove from m_objects. Maybe does nothing!
    ForgetPrimaryObject(p_record);

    // Try to release the record
    if(p_record->Release())
    {
      // Remove from m_records
      m_records.erase(it);

      // Reset the current pointer
      First();
    }
    return true;
  }
  return false;
}

// Forget the registration of the optimized primary key
void
SQLDataSet::ForgetPrimaryObject(const SQLRecord* p_record)
{
  XString key = MakePrimaryKey(p_record);

  if(!key.IsEmpty())
  {
    ObjectMap::iterator it = m_objects.find(key);
    if(it != m_objects.end())
    {
      m_objects.erase(it);
    }
  }
}

// End of namespace
}
