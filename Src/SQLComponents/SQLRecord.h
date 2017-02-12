////////////////////////////////////////////////////////////////////////
//
// File: SQLRecord.h
//
// Copyright (c) 1998-2017 ir. W.E. Huisman
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
// Last Revision:   08-01-2017
// Version number:  1.4.0
//
#pragma once
#include "SQLMutation.h"
#include <vector>

namespace SQLComponents
{

// Current status of the record
#define SQL_Record_NULL      0x00
#define SQL_Record_Selected  0x01
#define SQL_Record_Updated   0x02
#define SQL_Record_Deleted   0x04
#define SQL_Record_Insert    0x08

// Fields are mutation stacks 
typedef std::vector<SQLMutation*> FieldMap;

// Foreward declaration
class SQLDataSet;
class XMLMessage;
class XMLElement;
class SQLVariant;

class SQLRecord
{
public:
  SQLRecord(SQLDataSet* p_set,bool p_modifiable = false);
 ~SQLRecord();
  // Get the status of the record
  int         GetStatus();
  SQLVariant* GetField(int p_num);
  void        AddField(SQLVariant* p_field,bool p_insert = false);
  void        SetField(int p_num,SQLVariant* p_field,int p_mutationID = 0);
  void        ModifyField(int p_num,void*       p_data,int p_mutationID = 0);
  void        ModifyField(int p_num,SQLVariant* p_data,int p_mutationID = 0);
  // See if a field is modified
  bool        IsModified (int p_num);
  // Set the status of the record to 'Deleted'
  void        Delete();
  // Reset the status after database upgrade of the record
  void        Reduce();
  // Rollback all mutations on all fields
  void        Rollback();
  // Cancel the mutations of specified id
  bool        CancelMutation(int p_mutationID);
  // Has mixed mutationID mutations
  MutType     MixedMutations(int p_mutationID);
  // For reporting/analysis purposes: all mutationID's on the stack
  int         AllMixedMutations(MutationIDS& p_list,int p_mutationID);

  // XML Saving and reading
  void        XMLSave(XMLMessage* p_msg,XMLElement* p_base);
  void        XMLLoad(XMLMessage* p_msg,XMLElement* p_base);

private:
  SQLDataSet* m_dataSet;
  bool        m_modifiable;
  int         m_status;
  FieldMap    m_fields;
};

inline int
SQLRecord::GetStatus()
{
  return m_status;
}

// End of namespace
}