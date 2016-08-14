////////////////////////////////////////////////////////////////////////
//
// File: SQLRercord.cpp
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
#include "SQLRecord.h"
#include "SQLDataSet.h"
#include "SQLVariant.h"
#include "xml.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

SQLRecord::SQLRecord(SQLDataSet* p_set,bool p_modifiable)
          :m_dataSet(p_set)
          ,m_modifiable(p_modifiable)
          ,m_status(SQL_Record_NULL)
{
}

SQLRecord::~SQLRecord()
{
  for(unsigned ind = 0;ind < m_fields.size(); ++ind)
  {
    delete m_fields[ind];
  }
  m_status = SQL_Record_NULL;
}

// Set this record's status to a deleted record
void
SQLRecord::Delete()
{
  m_status |= SQL_Record_Deleted;
  m_dataSet->SetStatus(SQL_Deletions);
}

// Add a new field upon reading in the record
// or upon entering a new record (outside database read-in)
void
SQLRecord::AddField(SQLVariant* p_field
                   ,bool        p_insert /*= false*/)
{
  SQLMutation* mut = new SQLMutation(p_field);
  m_fields.push_back(mut);

  // Optionally it can be a newly inserted record
  if(p_insert)
  {
    m_status |= SQL_Record_Insert;
    m_dataSet->SetStatus(SQL_Insertions);
  }
}

// Mutate the field
void
SQLRecord::SetField(int p_num,SQLVariant* p_field,int p_mutationID /*=0*/)
{
  if(m_status & SQL_Record_Deleted)
  {
    return;
  }
  if(p_num >= 0 && p_num < (int)m_fields.size())
  {
    m_fields[p_num]->Mutate(p_field,p_mutationID);
  }
}

SQLVariant*     
SQLRecord::GetField(int p_num)
{
  if(p_num >= 0 && p_num < (int)m_fields.size())
  {
    return m_fields[p_num]->Current();
  }
  return NULL;
}

// Rollback of all field updates
void
SQLRecord::Rollback()
{
  if(m_modifiable == false)
  {
    return;
  }
  for(unsigned ind = 0;ind < m_fields.size();++ind)
  {
    // Cancel all mutation ID's values
    m_fields[ind]->Cancel(0);
  }
  // Revert to inserted or selected
  m_status &= ~SQL_Record_Updated;
  m_status &= ~SQL_Record_Deleted;
}

// Modify a field on the basis of the raw data pointer
// Expects the same datatype as the value of the field!!!!!
void
SQLRecord::ModifyField(int p_num,void* p_data,int p_mutationID /*=0*/)
{
  if(m_modifiable == false || (m_status & SQL_Record_Deleted))
  {
    return;
  }
  if(p_num < 0 || p_num >= (int)m_fields.size())
  {
    return;
  }
  SQLVariant* field = m_fields[p_num]->MutationValue(p_mutationID);
  if(memcmp(field->GetDataPointer(),p_data,field->GetDataSize()))
  {
    // If really different, set updated
    m_status |= SQL_Record_Updated;
    m_dataSet->SetStatus(SQL_Updates);
  }
  if(m_fields[p_num]->Original())
  {
    // Make the first mutation
    SQLVariant value;
    value.SetData(field->GetDataType(),(const char*)p_data);
    m_fields[p_num]->Add(&value,p_mutationID);
  }
  else if(m_fields[p_num]->CurrentMutationID() == p_mutationID)
  {
    // Same mutation id: add it directly to the SQLVariant
    field->SetData(field->GetDataType(),(char*)p_data);
  }
  else
  {
    // Make a new mutation of the same type
    SQLVariant value;
    value.SetData(field->GetDataType(),(const char*)p_data);
    m_fields[p_num]->Add(&value,p_mutationID);
  }
}

void        
SQLRecord::ModifyField(int p_num,SQLVariant* p_data,int p_mutationID /*=0*/)
{
  if(m_modifiable == false || (m_status & SQL_Record_Deleted))
  {
    return;
  }
  if(p_num < 0 || p_num >= (int)m_fields.size())
  {
    return;
  }
  if(p_data == NULL)
  {
    // Need a data pointer
    return;
  }
  SQLVariant* field = m_fields[p_num]->MutationValue(p_mutationID);
  size_t size = max(field->GetDataSize(),p_data->GetDataSize());
  if(memcmp(field->GetDataPointer(),p_data->GetDataPointer(),size))
  {
    m_status |= SQL_Record_Updated;
    m_dataSet->SetStatus(SQL_Updates);
  }
  m_fields[p_num]->Mutate(p_data,p_mutationID);
}

// Field is modified
bool
SQLRecord::IsModified(int p_num)
{
  if(m_modifiable == false)
  {
    // Column cannot be modified
    return false;
  }
  if(p_num < 0 || p_num >= (int)m_fields.size())
  {
    // Incorrect column number
    return false;
  }
  return m_fields[p_num]->IsMutated();
}


void
SQLRecord::Reduce()
{
  // Reduce all fields
  for(unsigned ind = 0;ind < m_fields.size(); ++ind)
  {
    m_fields[ind]->Reduce();
  }
  // Reset status to selected-from-database
  m_status = SQL_Record_Selected;
}

// Cancel the mutations of specified id
// Returns the fact if it contains still mutations
bool
SQLRecord::CancelMutation(int p_mutationID)
{
  // The original value cannot be cancelled
  if(p_mutationID <= 0)
  {
    return ((m_status & (SQL_Record_Insert | SQL_Record_Updated | SQL_Record_Deleted)) > 0);
  }
  bool mutated = false;
  for(unsigned ind = 0;ind < m_fields.size(); ++ind)
  {
    if(m_fields[ind]->Cancel(p_mutationID) > 1)
    {
      mutated = true;
    }
  }
  if(!mutated)
  {
    // Reset the mutation status
    // Leaving selected/original status intact
    m_status &= ~(SQL_Record_Insert | SQL_Record_Updated | SQL_Record_Deleted);
  }
  return mutated;
}

//////////////////////////////////////////////////////////////////////////
//
// XML Saving and loading
//
//////////////////////////////////////////////////////////////////////////

void
SQLRecord::XMLSave(XmlElement* p_records)
{
  XmlElement* record = new XmlElement(DATASET_RECORD);
  p_records->LinkEndChild(record);

  for(unsigned int ind = 0;ind < m_fields.size(); ++ind)
  {
    SQLVariant* var = m_fields[ind]->Current();
    CString naam = m_dataSet->GetFieldName(ind);
    int type = var->GetDataType();

    XmlElement* veld = new XmlElement(DATASET_FIELD);
    record->LinkEndChild(veld);
    veld->SetAttribute("id",ind);
    veld->SetAttribute("type",type);
    veld->SetAttribute("naam",naam);

    CString val;
    var->GetAsString(val);
    XmlText* value = new XmlText(val);
    veld->LinkEndChild(value);
  }
}

void
SQLRecord::XMLLoad(XmlElement* p_record)
{
  int ind = 0;
  XmlElement* field = p_record->FirstChildElement();
  while(field)
  {
    int type = 0;
    int id   = 0;
    field->Attribute(DATASET_TYPE,&type);
    field->Attribute(DATASET_ID,  &id);
    if(ind != id)
    {
      throw CString("Ongeldige veld sequence in record of: ") + m_dataSet->GetName();
    }
    char* text = (char*)field->GetText();
    if(!text) 
    {
      // No empty nodes
      text = "";
    }
    SQLVariant value;
    value.SetData(type,text);
    AddField(&value);

    // Next field
    field = field->NextSiblingElement();
    ++ind;
  }
}
