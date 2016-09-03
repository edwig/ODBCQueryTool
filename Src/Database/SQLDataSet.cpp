////////////////////////////////////////////////////////////////////////
//
// File: SQLDataSet.cpp
//
// Copyright (c) 1998- 2014 ir. W.E. Huisman
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
// Last Revision:   01-01-2015
// Version number:  1.1.0
//
#include "stdafx.h"
#include "SQLDataSet.h"
#include "SQLQuery.h"
#include "SQLVariantFormat.h"
#include "SQLInfo.h"
#include "xml.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

SQLDataSet::SQLDataSet()
           :m_database(NULL)
           ,m_status(SQL_Empty)
           ,m_current(-1)
           ,m_open(false)
{
}

SQLDataSet::SQLDataSet(CString p_name,SQLDatabase* p_database /*=NULL*/)
           :m_name(p_name)
           ,m_database(p_database)
           ,m_status(SQL_Empty)
           ,m_current(-1)
           ,m_open(false)
{
}

SQLDataSet::~SQLDataSet()
{
  Close();
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

  // Forget all caches
  m_names.clear();
  m_types.clear();
  m_parameters.clear();
  m_primaryKey.clear();

  // Forget the query
  m_name.Empty();
  m_query.Empty();
  m_selection.Empty();
  m_searchColumn.Empty();
  m_primaryTableName.Empty();

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
  for(unsigned int ind = 0; ind < m_records.size(); ++ind)
  {
    SQLRecord* record = m_records[ind];
    delete record;
  }
  // Forget the caches
  m_records.clear();
  m_objects.clear();
  // Set status to empty
  m_status  = SQL_Empty;
  m_current = -1;

  return true;
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
SQLDataSet::SetParameter(SQLParameter p_parameter)
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
SQLDataSet::SetParameter(CString p_naam,SQLVariant p_waarde)
{
  // See if the parameter is already there
  for(unsigned int ind = 0;ind < m_parameters.size();++ind)
  {
    if(m_parameters[ind].m_name == p_naam)
    {
      // Found it: set a new value
      m_parameters[ind].m_value = p_waarde;
      return;
    }
  }
  // New parameter
  SQLParameter par;
  par.m_name   = p_naam;
  par.m_value  = p_waarde;
  m_parameters.push_back(par);
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
SQLDataSet::GetParameter(CString& p_name)
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
CString
SQLDataSet::ParseQuery()
{
  bool inQuote = false;
  bool inAphos = false;
  CString query;
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
      CString parNaam;
      do 
      {
        c = m_query.GetAt(++pos);
        parNaam += (char) c;
      } 
      while (isalnum(c) || c == '_');
      parNaam = parNaam.Left(parNaam.GetLength() - 1);
      // naam gevonden
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
CString
SQLDataSet::ParseSelection(SQLQuery& p_query)
{
  CString sql("SELECT ");
  sql += m_selection;
  sql += "\n  FROM ";
  sql += m_primaryTableName;

  int number = 0;
  ParameterSet::iterator it;
  for(it = m_parameters.begin();it != m_parameters.end(); ++it)
  {
    sql += (it == m_parameters.begin()) ? "\n WHERE " : "\n   AND ";
    sql += it->m_name;
    if(it->m_value.IsNULL())
    {
      sql += " IS NULL";
    }
    else
    {
      sql += " = ?";
      p_query.SetParameter(++number,&(it->m_value));
    }
  }
  return sql;
}

bool
SQLDataSet::Open(bool p_stopIfNoColumns /*= false*/)
{
  bool   result = false;
  CString query = m_query;
  int    number = 0;

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
    SQLQuery qr(m_database);
    SQLTransaction trans(m_database,m_name);

    // If parameters, parse them
    if(!m_query.IsEmpty())
    {
      if(m_parameters.size())
      {
        query = ParseQuery();
      }
    }
    else
    {
      query = ParseSelection(qr);
    }

    // Do the SELECT query
    qr.DoSQLStatement(query);
    if (p_stopIfNoColumns && qr.GetNumberOfColumns() == 0)
    {
      return false;
    }

    // Read the names of the fields
    ReadNames(qr);
    ReadTypes(qr);

    // Searchable sets prepare primary key column
    int primary = FindSearchColumn();

    // Read all the records
    while(qr.GetRecord())
    {
      // Make a new record
      SQLRecord* record = new SQLRecord(this,modifiable);
      m_records.push_back(record);
      // Get all the columns of the record
      SQLVariant* var = NULL;
      int num = qr.GetNumberOfColumns();
      for(int ind = 1; ind <= num; ++ind)
      {
        var = qr.GetColumn(ind);
        record->AddField(var);

        // Record position of the primary
        if(ind == primary)
        {
          unsigned long prim = var->GetAsULong();
          m_objects[prim] = number++;
        }

// #ifdef DEBUG
//         // Debugging e.g. for reading the data from the dataset
//         // In case of format problems with DOUBLE/NUMERIC/DECIMAL
//         CString res;
//         var->GetAsString(res);
//         TRACE("Dataset: %s Field: %d Value: %s\n",m_name,ind,res);
// #endif
      }
    }
    // Reached the end: we are OPEN!
    m_open = true;
    result = true;
    // Remember the query
    m_query = query;

    trans.Commit();
  }
  catch(CString s)
  {
    Close();
    throw s;
  }
  if(m_records.size())
  {
    m_current = 0;
    m_status |= SQL_Selections;
  }
  return result;
}

// Append (read extra) into the dataset
// Precondition: Dataset must already be opened by 'Open()'
// Precondition: Different parameter values set to read different data
// Precondition: Results must be of same resultset (columns and datatypes)
bool 
SQLDataSet::Append()
{
  bool result = false;

  // See if already opened
  if(!m_open)
  {
    return false;
  }
  // Test if possibly modifiable if primary table name and key are givven
  bool modifiable = false;
  if(!m_primaryTableName.IsEmpty() && m_primaryKey.size())
  {
    modifiable = true;
  }
  // Set last record to last current, setting m_current
  Last();
  // Remember size before reading
  size_t sizeBefore = m_records.size();
  int    number     = 1;

  try
  {
    SQLQuery qr(m_database);
    SQLTransaction trans(m_database,m_name);

    // Use the parameters (if any)
    for(unsigned ind = 0;ind < m_parameters.size();++ind)
    {
      qr.SetParameter(ind + 1,&m_parameters[ind].m_value);
    }

    // Do the SELECT query
    qr.DoSQLStatement(m_query);

    // Names and types must be the same as previous queries
    CheckNames(qr);
    CheckTypes(qr);

    // Finding the primary
    int primary = FindSearchColumn();

    while(qr.GetRecord())
    {
      // Make a new record
      SQLRecord* record = new SQLRecord(this,modifiable);
      m_records.push_back(record);
      // Get all the columns of the record
      SQLVariant* var = NULL;
      int num = qr.GetNumberOfColumns();
      for(int ind = 1; ind <= num; ++ind)
      {
        var = qr.GetColumn(ind);
        record->AddField(var);

        // Record primary key in object cache
        if(ind == primary)
        {
          unsigned long prim = var->GetAsULong();
          m_objects[prim] = m_current + number++;
        }
      }
    }
    // Legal 0 or more records
    result = true;
    trans.Commit();
  }
  catch(CString s)
  {
    Close();
    throw s;
  }
  if(m_records.size() > sizeBefore)
  {
    m_status |= SQL_Selections;
    // Goto first read record
    Next();
  }
  return result;
}

// Get all the columns of the record
void
SQLDataSet::ReadNames(SQLQuery& qr)
{
  CString naam;
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
  int type;
  int num = qr.GetNumberOfColumns();
  for(int ind = 1; ind <= num; ++ind)
  {
    type = qr.GetColumnType(ind);
    m_types.push_back(type);
  }
}

// Check that all names are the same
void
SQLDataSet::CheckNames(SQLQuery& p_query)
{
  CString name;
  int num = p_query.GetNumberOfColumns();
  for(int ind = 1; ind <= num; ++ind)
  {
    p_query.GetColumnName(ind,name);
    if(m_names[ind-1].CompareNoCase(name))
    {
      throw CString("Append needs exactly the same query column names");
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
    if(m_types[ind-1] != type)
    {
      throw CString("Append needs exactly the same datatypes for the query columns.");
    }
  }
}

// Find primary key in the column names
int
SQLDataSet::FindSearchColumn()
{
  // Searchable primary requested?
  if(m_searchColumn.IsEmpty())
  {
    return 0;
  }
  // Find the column in the selected set
  for(unsigned ind = 0;ind < m_names.size(); ++ind)
  {
    if(m_searchColumn.CompareNoCase(m_names[ind]) == 0)
    {
      if(m_types[ind] == SQL_C_SLONG ||
         m_types[ind] == SQL_C_ULONG ||
         m_types[ind] == SQL_C_LONG  )
      {
        return ind + 1;
      }
    }
  }
  return 0;
}

// Check that all datatypes are the same
SQLRecord*
SQLDataSet::GetRecord(int p_recnum)
{
  if((unsigned)p_recnum >= 0 && (unsigned)p_recnum < m_records.size())
  {
    return m_records[p_recnum];
  }
  return NULL;
}

// Find the object record of an integer primary key
int
SQLDataSet::FindObjectRecNum(int p_primary)
{
  ObjectMap::iterator it = m_objects.find(p_primary);
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
  ObjectMap::iterator it = m_objects.find(p_primary);
  if(it != m_objects.end())
  {
    return m_records[it->second];
  }
  return NULL;
}

// If your primary is a compound key or not INTEGER (Slower)
int
SQLDataSet::FindObjectRecNum(VariantSet& p_primary)
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

  int searching = 0;
  for(int ind = 0;ind < (int) m_primaryKey.size(); ++ind)
  {
    int fieldnum = GetFieldNumber(m_primaryKey[ind]);
    SQLVariant* var = p_primary[ind];

    for(int search = searching; search < (int)m_records.size(); ++search)
    {
      SQLVariant* field = m_records[search]->GetField(fieldnum);
      if(field == var)
      {
        if(ind+1 == (int) m_primaryKey.size())
        {
          // Last field in the primary key. Stop here
          return search;
        }
        else
        {
          // Value found, get on to the next searchfield
          break;
        }
      }
      ++searching;
    }
  }
  return -1;
}

// If your primary is a compound key or not INTEGER (Slower)
SQLRecord*
SQLDataSet::FindObjectRecord(VariantSet& p_primary)
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

  int searching = 0;
  for(int ind = 0; ind < (int)m_primaryKey.size(); ++ind)
  {
    int fieldnum = GetFieldNumber(m_primaryKey[ind]);
    SQLVariant* var = p_primary[ind];

    for(int search = searching; search < (int)m_records.size(); ++search)
    {
      SQLVariant* field = m_records[search]->GetField(fieldnum);
      if(field == var)
      {
        if(ind + 1 == (int)m_primaryKey.size())
        {
          // Last field in the primary key. Stop here
          return m_records[search];
        }
        else
        {
          // Value found, get on to the next searchfield
          break;
        }
      }
      ++searching;
    }
  }
  return nullptr;
}

// Get a fieldname
CString    
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
SQLDataSet::GetFieldNumber(CString p_name)
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
SQLRecord* 
SQLDataSet::InsertRecord()
{
  SQLRecord* record = new SQLRecord(this);
  m_records.push_back(record);
  m_current = (int)(m_records.size() - 1);
  m_status |= SQL_Insertions;
  return record;
}

// Insert new field in new record
int
SQLDataSet::InsertField(CString& p_name,SQLVariant* p_value)
{
  m_names.push_back(p_name);
  m_types.push_back(p_value->GetDataType());
  if(m_current >= 0 && m_current < (int)m_records.size())
  {
    SQLRecord* record = m_records[m_current];
    record->AddField(p_value);
    return 0;
  }
  // Internal programming error
  return -1;
}

// Set a field value in the current record
void
SQLDataSet::SetField(CString& p_name,SQLVariant* p_value,int p_mutationID /*=0*/)
{
  int num = GetFieldNumber(p_name);
  if(num >= 0)
  {
    SetField(num,p_value,p_mutationID);
  }
}

// Set a field value in the current record
void       
SQLDataSet::SetField(int p_num,SQLVariant* p_value,int p_mutationID /*=0*/)
{
  if(m_current >= 0)
  {
    m_records[m_current]->SetField(p_num,p_value,p_mutationID);
  }
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
    SQLVariant* var = m_records[ind]->GetField(p_num);
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
// THE SYNCHRONIZE PROCESS
//
//////////////////////////////////////////////////////////////////////////

// Insert / Update / delete records from the database
bool
SQLDataSet::Synchronize()
{
  // Needs the primary table name of the dataset
  if(m_primaryTableName.IsEmpty())
  {
    return false;
  }
  if((m_status & (SQL_Record_Insert | SQL_Record_Deleted | SQL_Record_Updated)) == 0)
  {
    // Nothing to do: all OK.
    return true;
  }
  if(!GetPrimaryKeyInfo())
  {
    // No primary key, cannot do updates/deletes
    return false;
  }
  if(!CheckPrimaryKeyColumns())
  {
    // Not all primary key columns are present
    return false;
  }
  // Do all operations or throw
  if(m_status & SQL_Record_Deleted)
  {
    Deletes();
  }
  if(m_status & SQL_Record_Updated)
  {
    Updates();
  }
  if(m_status & SQL_Record_Insert)
  {
    Inserts();
  }
  // Reset the status field
  m_status = m_records.size() > 0 ? SQL_Selections : 0;

  // Ready
  return true;
}

// Getting the primary key for a table
bool
SQLDataSet::GetPrimaryKeyInfo()
{
  if(m_primaryKey.size() == 0)
  {
    SQLInfo* info = m_database->GetSQLInfo();
    CString primaryConstraintName;
    PrimaryMap map;
    if(info->GetPrimaryKeyInfo(m_primaryTableName,primaryConstraintName,map) == false)
    {
      // No primary key givven, and cannot get it from the ODBC driver
      return false;
    }
    // Remember the primary key info
    PrimaryMap::iterator it;
    for(it = map.begin();it != map.end(); ++it)
    {
      m_primaryKey.push_back(it->second.m_colName);
    }
  }
  return m_primaryKey.size() > 0;
}

// Check that all primary key colums are in the dataset
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
  // All keyparts found
  return true;
}

void
SQLDataSet::Deletes()
{
  RecordSet::iterator it = m_records.begin();
  SQLTransaction trans(m_database,m_name);
  SQLQuery       query(m_database);

  // Loop through all the records
  while(it != m_records.end())
  {
    SQLRecord* record = *it;
    if(record->GetStatus() & SQL_Record_Deleted)
    {
      CString sql = GetSQLDelete(&query,record);
      query.DoSQLStatement(sql);
      query.ResetParameters();

      // Delete this record, continuing to the next
      delete record;
      it = m_records.erase(it);
    }
    else
    {
      ++it;
    }
  }
  trans.Commit();

  // Adjust the current record if neccesary
  if(m_current > (int)m_records.size())
  {
    m_current = (int)m_records.size();
  }
}

void
SQLDataSet::Updates()
{
  RecordSet::iterator it;
  SQLTransaction trans(m_database,m_name);
  SQLQuery       query(m_database);

  for(it = m_records.begin();it != m_records.end();++it)
  {
    SQLRecord* record = *it;
    if(record->GetStatus() & SQL_Record_Updated)
    {
      CString sql = GetSQLUpdate(&query,record);
      query.DoSQLStatement(sql);
      query.ResetParameters();

      record->Reduce();
    }
  }
  trans.Commit();
}

void
SQLDataSet::Inserts()
{
  RecordSet::iterator it;
  SQLTransaction trans(m_database,m_name);
  SQLQuery       query(m_database);

  for(it = m_records.begin();it != m_records.end();++it)
  {
    SQLRecord* record = *it;
    if(record->GetStatus() & SQL_Record_Insert)
    {
      CString sql = GetSQLInsert(&query,record);
      query.DoSQLStatement(sql);
      query.ResetParameters();

      record->Reduce();
    }
  }
  trans.Commit();
}

CString
SQLDataSet::GetSQLDelete(SQLQuery* p_query,SQLRecord* p_record)
{
  CString sql("DELETE FROM " + m_primaryTableName + "\n");
  int parameter = 1;
  sql += GetWhereClause(p_query,p_record,parameter);
  return sql;
}

CString
SQLDataSet::GetSQLUpdate(SQLQuery* p_query,SQLRecord* p_record)
{
  int parameter = 1;
  CString sql("UPDATE " + m_primaryTableName + "\n");

  // Check for all fields
  bool first = true;
  for(unsigned ind = 0;ind < m_names.size(); ++ind)
  {
    if(p_record->IsModified(ind))
    {
      SQLVariant* value = p_record->GetField(ind);

      sql += first ? "   SET " : "      ,";
      sql += m_names[ind];
      if(value->IsNULL())
      {
        sql += " = NULL\n";
      }
      else
      {
        sql += " = ?\n";
        p_query->SetParameter(parameter++,value);
      }
    }
  }
  // WHERE clausule toevoegen
  sql += GetWhereClause(p_query,p_record,parameter);

  return sql;
}

CString
SQLDataSet::GetSQLInsert(SQLQuery* p_query,SQLRecord* p_record)
{
  int parameter = 1;
  CString sql("INSERT INTO " + m_primaryTableName);

  CString fields("(");
  CString params("(");

  for(unsigned ind = 0;ind < m_names.size(); ++ind)
  {
    SQLVariant* value = p_record->GetField(ind);
    if(value->IsNULL() == false)
    {
      fields += m_names[ind] + ",";
      params += "?,";
      p_query->SetParameter(parameter++,value);
    }
  }
  // Closing
  fields.TrimRight(',');
  params.TrimRight(',');
  fields += ")\n";
  params += ")";
  // Put it together
  sql += fields;
  sql += "VALUES ";
  sql += params;

  return sql;
}

CString
SQLDataSet::GetWhereClause(SQLQuery* p_query,SQLRecord* p_record,int& p_parameter)
{
  CString sql(" WHERE ");

  bool more = false;
  for(unsigned ind = 0;ind < m_primaryKey.size();++ind)
  {
    if(more)
    {
      sql += "\n   AND ";
    }
    more = true;
    int column = GetFieldNumber(m_primaryKey[ind]);
    sql += m_primaryKey[ind];
    SQLVariant* value = p_record->GetField(column);
    if(value->IsNULL())
    {
      sql += " = NULL";
    }
    else
    {
      sql += " = ?";
      p_query->SetParameter(p_parameter++,value);
    }
  }
  return sql;
}

//////////////////////////////////////////////////////////////////////////
//
// Store in XBDF format
//
//////////////////////////////////////////////////////////////////////////

void
SQLDataSet::XMLSave(XmlElement* p_dataset)
{
  // Name of the dataset
  XmlElement* naam = new XmlElement(DATASET_NAME);
  XmlText* ntext = new XmlText(m_name);
  p_dataset->LinkEndChild(naam);
  naam->LinkEndChild(ntext);

  // Fieldstructure of the dataset
  XmlElement* structur = new XmlElement(DATASET_STRUCTURE);
  p_dataset->LinkEndChild(structur);

  SQLRecord* record = GetRecord(0);
  if(record)
  {
    for(unsigned int ind = 0; ind < m_names.size(); ++ind)
    {
      CString fieldname = GetFieldName(ind);
      SQLVariant* var = record->GetField(ind);
      int type = var->GetDataType();
      
      XmlElement* veld = new XmlElement(DATASET_FIELD);
      structur->LinkEndChild(veld);
      veld->SetAttribute(DATASET_ID,ind);
      veld->SetAttribute(DATASET_TYPE,type);
      veld->SetAttribute(DATASET_TYPENAME,var->FindDatatype(type));

      XmlText* newtext = new XmlText(fieldname);
      veld->LinkEndChild(newtext);
    }
  }

  // All records of the dataset
  XmlElement* records = new XmlElement(DATASET_RECORDS);
  p_dataset->LinkEndChild(records);
  for(unsigned int ind = 0; ind < m_records.size(); ++ind)
  {
    m_records[ind]->XMLSave(records);
  }
}

void
SQLDataSet::XMLLoad(XmlElement* p_dataset)
{
  XmlElement* structur = p_dataset->FindElement(DATASET_STRUCTURE);
  XmlElement* records  = p_dataset->FindElement(DATASET_RECORDS);
  if(structur == NULL) throw CString("Structure part missing in the XML dataset.") + m_name;
  if(records  == NULL) throw CString("Records part missing in the XML dataset") + m_name;

  // Read the structure
  XmlElement* veld = structur->FirstChildElement();
  while(veld)
  {
    // Remember the name of the field
    CString naam = veld->GetText();
    m_names.push_back(naam);
    // Datatype of the field
    int type = atoi(veld->Attribute(DATASET_TYPE));
    m_types.push_back(type);
    // Next field
    veld = veld->NextSiblingElement();
  }

  // Read records
  XmlElement* record = records->FirstChildElement();
  while(record)
  {
    // Make record and load it
    SQLRecord* rec = InsertRecord();
    rec->XMLLoad(record);
    // Next record
    record = record->NextSiblingElement();
  }
}
