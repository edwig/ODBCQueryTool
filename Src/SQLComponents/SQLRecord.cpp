////////////////////////////////////////////////////////////////////////
//
// File: SQLRercord.cpp
//
// Copyright (c) 1998-2018 ir. W.E. Huisman
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
// Last Revision:   20-01-2019
// Version number:  1.5.4
//
#include "stdafx.h"
#include "SQLComponents.h"
#include "SQLRecord.h"
#include "SQLDataSet.h"
#include "SQLVariant.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{

SQLRecord::SQLRecord(SQLDataSet* p_set,bool p_modifiable)
          :m_dataSet(p_set)
          ,m_modifiable(p_modifiable)
          ,m_status(SQL_Record_NULL)
          ,m_reference(0)
          ,m_generator(-1)
{
  Acquire();
}

SQLRecord::~SQLRecord()
{
  for(unsigned ind = 0;ind < m_fields.size(); ++ind)
  {
    delete m_fields[ind];
  }
  m_status = SQL_Record_NULL;
}

void
SQLRecord::Acquire()
{
  InterlockedIncrement(&m_reference);
}

bool
SQLRecord::Release()
{
  if(InterlockedDecrement(&m_reference) == 0)
  {
    delete this;
    return true;
  }
  return false;
}

// Set the status of the record to 'Inserted'
void
SQLRecord::Inserted()
{
  m_status = SQL_Record_Insert;
  m_dataSet->SetStatus(SQL_Insertions,SQL_Record_Updated);
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

void
SQLRecord::SetField(CString p_name,SQLVariant* p_field,int p_mutationID /*=0*/)
{
  SetField(m_dataSet->GetFieldNumber(p_name),p_field,p_mutationID);
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

SQLVariant* 
SQLRecord::GetField(CString p_name)
{
  if(m_dataSet)
  {
    return GetField(m_dataSet->GetFieldNumber(p_name));
  }
  return nullptr;
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
// void
// SQLRecord::ModifyField(int p_num,void* p_data,int p_mutationID /*=0*/)
// {
//   if(m_modifiable == false || (m_status & SQL_Record_Deleted))
//   {
//     return;
//   }
//   if(p_num < 0 || p_num >= (int)m_fields.size())
//   {
//     return;
//   }
// 
//   // Compare values, and see if we really must modify something
// 
//   // Get the last value from the mutation stack
//   SQLVariant* field = m_fields[p_num]->MutationValue(p_mutationID);
//   // Make the new value
//   SQLVariant value(field->GetDataType(),0);
//   value.SetFromRawDataPointer(p_data);
// 
//   // If equal, bail out
//   if(*field == value)
//   {
//     return;
//   }
// 
//   // If really different, set updated
//   m_status |= SQL_Record_Updated;
//   m_dataSet->SetStatus(SQL_Updates);
// 
//   if(m_fields[p_num]->Original())
//   {
//     // Make the first mutation
//     m_fields[p_num]->Add(&value,p_mutationID);
//   }
//   else if(m_fields[p_num]->CurrentMutationID() == p_mutationID)
//   {
//     // Same mutation id: add it directly to the SQLVariant
//     field->SetFromRawDataPointer(p_data);
//   }
//   else
//   {
//     // Make a new mutation of the same type
//     m_fields[p_num]->Add(&value,p_mutationID);
//   }
// }

// void
// SQLRecord::ModifyField(const char* p_name,void* p_field,int p_mutationID /*=0*/)
// {
//   ModifyField(m_dataSet->GetFieldNumber(p_name),p_field,p_mutationID);
// }

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
  // Save the mutation
  if(m_fields[p_num]->Mutate(p_data,p_mutationID))
  {
    m_status |= SQL_Record_Updated;
    m_dataSet->SetStatus(SQL_Updates);
  }
}

void
SQLRecord::ModifyField(const char* p_name,SQLVariant* p_field,int p_mutationID /*= 0*/)
{
  ModifyField(m_dataSet->GetFieldNumber(p_name),p_field,p_mutationID);
}

void
SQLRecord::ModifyField(const char* p_name,bool& p_field,int p_mutationID /*= 0*/)
{
  SQLVariant value(p_field);
  ModifyField(m_dataSet->GetFieldNumber(p_name),&value,p_mutationID);
}

void
SQLRecord::ModifyField(const char* p_name,char& p_field,int p_mutationID /*= 0*/)
{
  SQLVariant value(p_field);
  ModifyField(m_dataSet->GetFieldNumber(p_name),&value,p_mutationID);
}

void        
SQLRecord::ModifyField(const char* p_name,const char* p_field,int p_mutationID /*= 0*/)
{
  SQLVariant value(p_field);
  ModifyField(m_dataSet->GetFieldNumber(p_name),&value,p_mutationID);
}

void
SQLRecord::ModifyField(const char* p_name,unsigned char& p_field,int p_mutationID /*= 0*/)
{
  SQLVariant value(p_field);
  ModifyField(m_dataSet->GetFieldNumber(p_name), &value, p_mutationID);
}

void
SQLRecord::ModifyField(const char* p_name,short& p_field,int p_mutationID /*= 0*/)
{
  SQLVariant value(p_field);
  ModifyField(m_dataSet->GetFieldNumber(p_name), &value, p_mutationID);
}

void
SQLRecord::ModifyField(const char* p_name,unsigned short& p_field,int p_mutationID /*= 0*/)
{
  SQLVariant value(p_field);
  ModifyField(m_dataSet->GetFieldNumber(p_name), &value, p_mutationID);
}

void
SQLRecord::ModifyField(const char* p_name,int& p_field,int p_mutationID /*= 0*/)
{
  SQLVariant value((long)p_field);
  ModifyField(m_dataSet->GetFieldNumber(p_name), &value, p_mutationID);
}

void
SQLRecord::ModifyField(const char* p_name,unsigned int& p_field,int p_mutationID /*= 0*/)
{
  SQLVariant value((unsigned int)p_field);
  ModifyField(m_dataSet->GetFieldNumber(p_name), &value, p_mutationID);
}

void
SQLRecord::ModifyField(const char* p_name,float& p_field,int p_mutationID /*= 0*/)
{
  SQLVariant value(p_field);
  ModifyField(m_dataSet->GetFieldNumber(p_name), &value, p_mutationID);
}

void
SQLRecord::ModifyField(const char* p_name,double& p_field,int p_mutationID /*= 0*/)
{
  SQLVariant value(p_field);
  ModifyField(m_dataSet->GetFieldNumber(p_name), &value, p_mutationID);
}

void
SQLRecord::ModifyField(const char* p_name,__int64& p_field,int p_mutationID /*= 0*/)
{
  SQLVariant value(p_field);
  ModifyField(m_dataSet->GetFieldNumber(p_name), &value, p_mutationID);
}

void
SQLRecord::ModifyField(const char* p_name,unsigned __int64& p_field,int p_mutationID /*= 0*/)
{
  SQLVariant value(p_field);
  ModifyField(m_dataSet->GetFieldNumber(p_name), &value, p_mutationID);
}

void
SQLRecord::ModifyField(const char* p_name,SQLDate& p_field,int p_mutationID /*= 0*/)
{
  SQLVariant value(&p_field);
  ModifyField(m_dataSet->GetFieldNumber(p_name), &value, p_mutationID);
}

void
SQLRecord::ModifyField(const char* p_name,SQLTime& p_field,int p_mutationID /*= 0*/)
{
  SQLVariant value(&p_field);
  ModifyField(m_dataSet->GetFieldNumber(p_name), &value, p_mutationID);
}

void
SQLRecord::ModifyField(const char* p_name,SQLTimestamp& p_field,int p_mutationID /*= 0*/)
{
  SQLVariant value(&p_field);
  ModifyField(m_dataSet->GetFieldNumber(p_name), &value, p_mutationID);
}

void
SQLRecord::ModifyField(const char* p_name,SQLInterval& p_field,int p_mutationID /*= 0*/)
{
  SQLVariant value(&p_field);
  ModifyField(m_dataSet->GetFieldNumber(p_name), &value, p_mutationID);
}

void
SQLRecord::ModifyField(const char* p_name,SQLGuid& p_field,int p_mutationID /*= 0*/)
{
  SQLVariant value(&p_field);
  ModifyField(m_dataSet->GetFieldNumber(p_name), &value, p_mutationID);
}

void
SQLRecord::ModifyField(const char* p_name,CString& p_field,int p_mutationID /*= 0*/)
{
  SQLVariant value(p_field);
  ModifyField(m_dataSet->GetFieldNumber(p_name), &value, p_mutationID);
}

void
SQLRecord::ModifyField(const char* p_name,bcd& p_field,int p_mutationID /*= 0*/)
{
  SQLVariant value(&p_field);
  ModifyField(m_dataSet->GetFieldNumber(p_name), &value, p_mutationID);
}

// Record is changed?
bool
SQLRecord::IsModified()
{
  return (m_status & (SQL_Record_Insert | SQL_Record_Updated | SQL_Record_Deleted)) > 0;
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

bool
SQLRecord::IsModified(CString p_name)
{
  return IsModified(m_dataSet->GetFieldNumber(p_name));
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

// Has mixed mutationID mutations
MutType
SQLRecord::MixedMutations(int p_mutationID)
{
  MutType type = MUT_NoMutation;

  for(unsigned ind = 0; ind < m_fields.size(); ++ind)
  {
    MutType mut = m_fields[ind]->MixedMutations(p_mutationID);
    switch(mut)
    {
      case MUT_NoMutation: break;
      case MUT_MyMutation: if(type == MUT_OnlyOthers)
                           {
                             return MUT_Mixed;
                           }
                           type = mut;
                           break;
      case MUT_OnlyOthers: if(type == MUT_MyMutation)
                           {
                             return MUT_Mixed;
                           }
                           type = mut;
                           break;
      case MUT_Mixed:      return mut;
      default:             type = mut;
                           break;
    }
  }
  // Record has only these mutations or none at all
  return type;
}

// For reporting/analysis purposes: all mutationID's on the stack
int
SQLRecord::AllMixedMutations(MutationIDS& p_list,int p_mutationID)
{
  int total = 0;
  for(unsigned ind = 0; ind < m_fields.size(); ++ind)
  {
    total += m_fields[ind]->AllMixedMutations(p_list,p_mutationID);
  }
  return total;
}

static SQLVariant null;

// Getting contents of the record as a SQLVariant pointer
SQLVariant&
SQLRecord::operator[](int p_index)
{
  SQLVariant* var = GetField(p_index);
  if(var)
  {
    return *var;
  }
  return null;
}

SQLVariant&
SQLRecord::operator[](const char* p_name)
{
  SQLVariant* var = GetField(p_name);
  if(var)
  {
    return *var;
  }
  return null;
}

//////////////////////////////////////////////////////////////////////////
//
// XML Saving and loading
//
//////////////////////////////////////////////////////////////////////////

void
SQLRecord::XMLSave(XMLMessage* p_msg,XMLElement* p_base)
{
  CString recName(dataset_names[g_defaultLanguage][DATASET_RECORD]);
  CString fldName(dataset_names[g_defaultLanguage][DATASET_FIELD]);

  CString idName  = dataset_names[g_defaultLanguage][DATASET_ID];
  CString typName = dataset_names[g_defaultLanguage][DATASET_TYPE];
  CString attName = dataset_names[g_defaultLanguage][DATASET_NAME];

  XMLElement* p_elem = p_msg->AddElement(p_base,recName,XDT_String,"");
  if(p_elem)
  {
    for(unsigned int ind = 0; ind < m_fields.size(); ++ind)
    {
      SQLVariant* var = m_fields[ind]->Current();
      CString fieldName = m_dataSet->GetFieldName(ind);
      int type = var->GetDataType();

      XMLElement* fld = p_msg->AddElement(p_elem,fldName,XDT_String,"");
      p_msg->SetAttribute(fld,idName, (int)ind);
      p_msg->SetAttribute(fld,typName,type);
      p_msg->SetAttribute(fld,attName,fieldName);

      CString value;
      var->GetAsString(value);
      fld->SetValue(value);
    }
  }
}

void
SQLRecord::XMLLoad(XMLMessage* p_msg,XMLElement* p_base)
{
  CString typeName(dataset_names[g_defaultLanguage][DATASET_TYPE]);
  CString idName  (dataset_names[g_defaultLanguage][DATASET_ID]);

  int ind = 0;
  XMLElement* field = p_msg->GetElementFirstChild(p_base);
  while(field)
  {
    int type = p_msg->GetAttributeInteger(field,typeName);
    int id   = p_msg->GetAttributeInteger(field,idName);
    if(id != ind)
    {
      throw StdException("Invalid field sequence in in record of: " + m_dataSet->GetName());
    }
    SQLVariant value;
    value.SetData(type,field->GetValue());
    // Save in this record
    AddField(&value);

    // Next field
    field = p_msg->GetElementSibling(field);
    ++ind;
  }
}

// End of namespace
}
