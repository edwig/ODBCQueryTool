////////////////////////////////////////////////////////////////////////
//
// File: SQLRecord.cpp
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
#include "SQLRecord.h"
#include "SQLDataSet.h"
#include "SQLVariant.h"
#include "SQLDate.h"
#include "SQLGuid.h"

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
int 
SQLRecord::AddField(const SQLVariant* p_field
                   ,bool p_insert /*= false*/)
{
  SQLMutation* mut = new SQLMutation(p_field);
  m_fields.push_back(mut);

  // Optionally it can be a newly inserted record
  if(p_insert)
  {
    m_status |= SQL_Record_Insert;
    m_dataSet->SetStatus(SQL_Insertions);
  }
  return (int)m_fields.size() - 1;
}

// Mutate the field
bool
SQLRecord::SetField(int p_num,const SQLVariant* p_field,int p_mutationID /*=0*/)
{
  if(m_status & SQL_Record_Deleted)
  {
    return false;
  }
  if(p_num >= 0 && p_num < (int)m_fields.size())
  {
    if(m_fields[p_num]->Mutate(p_field,p_mutationID))
    {
      m_status |= SQL_Record_Updated;
      return true;
    }
  }
  return false;
}

bool
SQLRecord::SetField(XString p_name,const SQLVariant* p_field,int p_mutationID /*=0*/)
{
  return SetField(m_dataSet->GetFieldNumber(p_name),p_field,p_mutationID);
}

SQLVariant*     
SQLRecord::GetField(int p_num) const
{
  if(p_num >= 0 && p_num < (int)m_fields.size())
  {
    return m_fields[p_num]->Current();
  }
  return NULL;
}

SQLVariant* 
SQLRecord::GetField(XString p_name) const
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
// SQLRecord::ModifyField(LPCTSTR p_name,void* p_field,int p_mutationID /*=0*/)
// {
//   ModifyField(m_dataSet->GetFieldNumber(p_name),p_field,p_mutationID);
// }

void        
SQLRecord::ModifyField(int p_num,const SQLVariant* p_data,int p_mutationID /*=0*/)
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
SQLRecord::ModifyField(LPCTSTR p_name,const SQLVariant* p_field,int p_mutationID /*= 0*/)
{
  ModifyField(m_dataSet->GetFieldNumber(p_name),p_field,p_mutationID);
}

void
SQLRecord::ModifyField(LPCTSTR p_name,const bool& p_field,int p_mutationID /*= 0*/)
{
  SQLVariant value(p_field);
  ModifyField(m_dataSet->GetFieldNumber(p_name),&value,p_mutationID);
}

void
SQLRecord::ModifyField(LPCTSTR p_name,const TCHAR& p_field,int p_mutationID /*= 0*/)
{
  SQLVariant value(p_field);
  ModifyField(m_dataSet->GetFieldNumber(p_name),&value,p_mutationID);
}

void        
SQLRecord::ModifyField(LPCTSTR p_name,LPCTSTR p_field,int p_mutationID /*= 0*/)
{
  SQLVariant value(p_field);
  ModifyField(m_dataSet->GetFieldNumber(p_name),&value,p_mutationID);
}

#ifndef UNICODE
void
SQLRecord::ModifyField(LPCTSTR p_name,const _TUCHAR& p_field,int p_mutationID /*= 0*/)
{
  SQLVariant value(p_field);
  ModifyField(m_dataSet->GetFieldNumber(p_name), &value, p_mutationID);
}
#endif

void
SQLRecord::ModifyField(LPCTSTR p_name,const short& p_field,int p_mutationID /*= 0*/)
{
  SQLVariant value(p_field);
  ModifyField(m_dataSet->GetFieldNumber(p_name), &value, p_mutationID);
}

void
SQLRecord::ModifyField(LPCTSTR p_name,const unsigned short& p_field,int p_mutationID /*= 0*/)
{
  SQLVariant value(p_field);
  ModifyField(m_dataSet->GetFieldNumber(p_name), &value, p_mutationID);
}

void
SQLRecord::ModifyField(LPCTSTR p_name,const int& p_field,int p_mutationID /*= 0*/)
{
  SQLVariant value(p_field);
  ModifyField(m_dataSet->GetFieldNumber(p_name), &value, p_mutationID);
}

void
SQLRecord::ModifyField(LPCTSTR p_name,const unsigned int& p_field,int p_mutationID /*= 0*/)
{
  SQLVariant value((unsigned int)p_field);
  ModifyField(m_dataSet->GetFieldNumber(p_name), &value, p_mutationID);
}

void
SQLRecord::ModifyField(LPCTSTR p_name,const float& p_field,int p_mutationID /*= 0*/)
{
  SQLVariant value(p_field);
  ModifyField(m_dataSet->GetFieldNumber(p_name), &value, p_mutationID);
}

void
SQLRecord::ModifyField(LPCTSTR p_name,const double& p_field,int p_mutationID /*= 0*/)
{
  SQLVariant value(p_field);
  ModifyField(m_dataSet->GetFieldNumber(p_name), &value, p_mutationID);
}

void
SQLRecord::ModifyField(LPCTSTR p_name,const __int64& p_field,int p_mutationID /*= 0*/)
{
  SQLVariant value(p_field);
  ModifyField(m_dataSet->GetFieldNumber(p_name), &value, p_mutationID);
}

void
SQLRecord::ModifyField(LPCTSTR p_name,const unsigned __int64& p_field,int p_mutationID /*= 0*/)
{
  SQLVariant value(p_field);
  ModifyField(m_dataSet->GetFieldNumber(p_name), &value, p_mutationID);
}

void
SQLRecord::ModifyField(LPCTSTR p_name,const SQLDate& p_field,int p_mutationID /*= 0*/)
{
  SQLVariant value(&p_field);
  ModifyField(m_dataSet->GetFieldNumber(p_name), &value, p_mutationID);
}

void
SQLRecord::ModifyField(LPCTSTR p_name,const SQLTime& p_field,int p_mutationID /*= 0*/)
{
  SQLVariant value(&p_field);
  ModifyField(m_dataSet->GetFieldNumber(p_name), &value, p_mutationID);
}

void
SQLRecord::ModifyField(LPCTSTR p_name,const SQLTimestamp& p_field,int p_mutationID /*= 0*/)
{
  SQLVariant value(&p_field);
  ModifyField(m_dataSet->GetFieldNumber(p_name), &value, p_mutationID);
}

void
SQLRecord::ModifyField(LPCTSTR p_name,const SQLInterval& p_field,int p_mutationID /*= 0*/)
{
  SQLVariant value(&p_field);
  ModifyField(m_dataSet->GetFieldNumber(p_name), &value, p_mutationID);
}

void
SQLRecord::ModifyField(LPCTSTR p_name,const SQLGuid& p_field,int p_mutationID /*= 0*/)
{
  SQLVariant value(&p_field);
  ModifyField(m_dataSet->GetFieldNumber(p_name), &value, p_mutationID);
}

void
SQLRecord::ModifyField(LPCTSTR p_name,const XString& p_field,int p_mutationID /*= 0*/)
{
  SQLVariant value(p_field);
  ModifyField(m_dataSet->GetFieldNumber(p_name), &value, p_mutationID);
}

void
SQLRecord::ModifyField(LPCTSTR p_name,const bcd& p_field,int p_mutationID /*= 0*/)
{
  SQLVariant value(&p_field);
  ModifyField(m_dataSet->GetFieldNumber(p_name), &value, p_mutationID);
}

bool
SQLRecord::GetFieldBool(LPCTSTR p_name)
{
  if(m_dataSet)
  {
    return GetField(m_dataSet->GetFieldNumber(p_name))->GetAsBoolean();
  }
  return false;
}

char
SQLRecord::GetFieldChar(LPCTSTR p_name)
{
  if(m_dataSet)
  {
    return (char) GetField(m_dataSet->GetFieldNumber(p_name))->GetAsUShort();
  }
  return 0;
}

unsigned char
SQLRecord::GetFieldUnsignedChar(LPCTSTR p_name)
{
  if(m_dataSet)
  {
    return (unsigned char) GetField(m_dataSet->GetFieldNumber(p_name))->GetAsUShort();
  }
  return 0;
}

short
SQLRecord::GetFieldShort(LPCTSTR p_name)
{
  if(m_dataSet)
  {
    return GetField(m_dataSet->GetFieldNumber(p_name))->GetAsSShort();
  }
  return 0;
}

unsigned short
SQLRecord::GetFieldUnsignedShort(LPCTSTR p_name)
{
  if(m_dataSet)
  {
    return GetField(m_dataSet->GetFieldNumber(p_name))->GetAsUShort();
  }
  return 0;
}

int
SQLRecord::GetFieldInteger(LPCTSTR p_name)
{
  if(m_dataSet)
  {
    return GetField(m_dataSet->GetFieldNumber(p_name))->GetAsSLong();
  }
  return 0;
}

unsigned int
SQLRecord::GetFieldUnsignedInteger(LPCTSTR p_name)
{
  if(m_dataSet)
  {
    return GetField(m_dataSet->GetFieldNumber(p_name))->GetAsULong();
  }
  return 0;
}

float
SQLRecord::GetFieldFloat(LPCTSTR p_name)
{
  if(m_dataSet)
  {
    return GetField(m_dataSet->GetFieldNumber(p_name))->GetAsFloat();
  }
  return 0;
}

double
SQLRecord::GetFieldDouble(LPCTSTR p_name)
{
  if(m_dataSet)
  {
    return GetField(m_dataSet->GetFieldNumber(p_name))->GetAsDouble();
  }
  return 0;
}

__int64
SQLRecord::GetFieldInteger64(LPCTSTR p_name)
{
  if(m_dataSet)
  {
    return GetField(m_dataSet->GetFieldNumber(p_name))->GetAsSBigInt();
  }
  return 0;
}

unsigned __int64
SQLRecord::GetFieldUnsignedInt64(LPCTSTR p_name)
{
  if(m_dataSet)
  {
    return GetField(m_dataSet->GetFieldNumber(p_name))->GetAsUBigInt();
  }
  return 0;
}

SQLDate
SQLRecord::GetFieldSQLDate(LPCTSTR p_name)
{
  if(m_dataSet)
  {
    return GetField(m_dataSet->GetFieldNumber(p_name))->GetAsSQLDate();
  }
  return SQLDate(0,0,0);
}

SQLTime
SQLRecord::GetFieldSQLTime(LPCTSTR p_name)
{
  if(m_dataSet)
  {
    return GetField(m_dataSet->GetFieldNumber(p_name))->GetAsSQLTime();
  }
  return SQLTime(0,0,0);
}

SQLTimestamp
SQLRecord::GetFieldSQLTimestamp(LPCTSTR p_name)
{
  if(m_dataSet)
  {
    return GetField(m_dataSet->GetFieldNumber(p_name))->GetAsSQLTimestamp();
  }
  return SQLTimestamp(0,0,0,0,0,0);
}

SQLInterval
SQLRecord::GetFieldSQLInterval(LPCTSTR p_name)
{
  if(m_dataSet)
  {
    return GetField(m_dataSet->GetFieldNumber(p_name))->GetAsSQLInterval();
  }
  return SQLInterval();
}

SQLGuid
SQLRecord::GetFieldSQLGuid(LPCTSTR p_name)
{
  if(m_dataSet)
  {
    return GetField(m_dataSet->GetFieldNumber(p_name))->GetAsSQLGuid();
  }
  return SQLGuid();
}

XString
SQLRecord::GetFieldXString(LPCTSTR p_name)
{
  XString string;
  if(m_dataSet)
  {
    GetField(m_dataSet->GetFieldNumber(p_name))->GetAsString(string);
  }
  return string;
}

bcd
SQLRecord::GetFieldBcd(LPCTSTR p_name)
{
  if(m_dataSet)
  {
    GetField(m_dataSet->GetFieldNumber(p_name))->GetAsBCD();
  }
  return bcd();
}

// Record is changed?
bool
SQLRecord::IsModified() const
{
  return (m_status & (SQL_Record_Insert | SQL_Record_Updated | SQL_Record_Deleted)) > 0;
}

// Field is modified
bool
SQLRecord::IsModified(int p_num) const
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
SQLRecord::IsModified(XString p_name) const
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
      case MUT_MyMutation: if(type & MutType::MUT_OnlyOthers)
                           {
                             return MutType::MUT_Mixed;
                           }
                           type = mut;
                           break;
      case MUT_OnlyOthers: if(type & MutType::MUT_MyMutation)
                           {
                             return MutType::MUT_Mixed;
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
SQLRecord::operator[](LPCTSTR p_name)
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
  XString recName(dataset_names[g_defaultLanguage][DATASET_RECORD]);
  XString fldName(dataset_names[g_defaultLanguage][DATASET_FIELD]);

  XString idName  = dataset_names[g_defaultLanguage][DATASET_ID];
  XString typName = dataset_names[g_defaultLanguage][DATASET_TYPE];
  XString attName = dataset_names[g_defaultLanguage][DATASET_NAME];

  XMLElement* p_elem = p_msg->AddElement(p_base,recName,XDT_String,"");
  if(p_elem)
  {
    for(unsigned int ind = 0; ind < m_fields.size(); ++ind)
    {
      const SQLVariant* var = m_fields[ind]->Current();
      XString fieldName = m_dataSet->GetFieldName(ind);
      int type = var->GetDataType();

      XMLElement* fld = p_msg->AddElement(p_elem,fldName,XDT_String,"");
      p_msg->SetAttribute(fld,idName, (int)ind);
      p_msg->SetAttribute(fld,typName,type);
      p_msg->SetAttribute(fld,attName,fieldName);

      XString value;
      var->GetAsString(value);
      fld->SetValue(value);
    }
  }
}

void
SQLRecord::XMLLoad(XMLMessage* p_msg,XMLElement* p_base)
{
  XString typeName(dataset_names[g_defaultLanguage][DATASET_TYPE]);
  XString idName  (dataset_names[g_defaultLanguage][DATASET_ID]);

  int ind = 0;
  XMLElement* field = p_msg->GetElementFirstChild(p_base);
  while(field)
  {
    int type = p_msg->GetAttributeInteger(field,typeName);
    int id   = p_msg->GetAttributeInteger(field,idName);
    if(id != ind)
    {
      throw StdException(_T("Invalid field sequence in in record of: ") + m_dataSet->GetName());
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
