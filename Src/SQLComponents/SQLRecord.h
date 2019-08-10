////////////////////////////////////////////////////////////////////////
//
// File: SQLRecord.h
//
// Copyright (c) 1998-2019 ir. W.E. Huisman
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
// Last Revision:  15-06-2019
// Version number: 1.5.5
//
#pragma once
#include "SQLMutation.h"
#include "XMLMessage.h"
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
typedef std::vector<SQLMutation*> SQLFields;
typedef unsigned long ulong;

// Forward declaration
class SQLDataSet;
class SQLVariant;

class SQLRecord
{
public:
  SQLRecord(SQLDataSet* p_set,bool p_modifiable = false);
 ~SQLRecord();
  // Get the status of the record
  int         GetStatus();
  SQLVariant* GetField(int p_num);
  SQLVariant* GetField(CString p_name);
  int         GetGenerator();
  // Setting a generator column
  void        SetGenerator(int p_generator);
  // Adding a field to the record
  void        AddField(SQLVariant* p_field,bool p_insert = false);
  // Setting different value without changing status
  void        SetField   (int     p_num, SQLVariant* p_field,int p_mutationID = 0);
  void        SetField   (CString p_name,SQLVariant* p_field,int p_mutationID = 0);
  // Setting different value AND changing record/set status
//   void        ModifyField(int         p_num, void*       p_field,int p_mutationID = 0);
//   void        ModifyField(const char* p_name,void*       p_field,int p_mutationID = 0);
  void        ModifyField(int         p_num, SQLVariant* p_field,int p_mutationID = 0);
  void        ModifyField(const char* p_name,SQLVariant* p_field,int p_mutationID = 0);

  // Modify a field from these base datatypes (Everything in a SQLVariant)
  void        ModifyField(const char* p_name,bool&              p_field,int p_mutationID = 0);
  void        ModifyField(const char* p_name,char&              p_field,int p_mutationID = 0);
  void        ModifyField(const char* p_name,const char*        p_field,int p_mutationID = 0);
  void        ModifyField(const char* p_name,unsigned char&     p_field,int p_mutationID = 0);
  void        ModifyField(const char* p_name,short&             p_field,int p_mutationID = 0);
  void        ModifyField(const char* p_name,unsigned short&    p_field,int p_mutationID = 0);
  void        ModifyField(const char* p_name,int&               p_field,int p_mutationID = 0);
  void        ModifyField(const char* p_name,unsigned int&      p_field,int p_mutationID = 0);
  void        ModifyField(const char* p_name,float&             p_field,int p_mutationID = 0);
  void        ModifyField(const char* p_name,double&            p_field,int p_mutationID = 0);
  void        ModifyField(const char* p_name,__int64&           p_field,int p_mutationID = 0);
  void        ModifyField(const char* p_name,unsigned __int64&  p_field,int p_mutationID = 0);
  void        ModifyField(const char* p_name,SQLDate&           p_field,int p_mutationID = 0);
  void        ModifyField(const char* p_name,SQLTime&           p_field,int p_mutationID = 0);
  void        ModifyField(const char* p_name,SQLTimestamp&      p_field,int p_mutationID = 0);
  void        ModifyField(const char* p_name,SQLInterval&       p_field,int p_mutationID = 0);
  void        ModifyField(const char* p_name,SQLGuid&           p_field,int p_mutationID = 0);
  void        ModifyField(const char* p_name,CString&           p_field,int p_mutationID = 0);
  void        ModifyField(const char* p_name,bcd&               p_field,int p_mutationID = 0);

  // See if the record has been changed
  bool        IsModified();
  // See if a field is modified
  bool        IsModified(int p_num);
  bool        IsModified(CString p_name);
  // Set the status of the record to 'Inserted'
  void        Inserted();
  // Set the status of the record to 'Deleted'
  void        Delete();
  // Reset the mutation stacks after database upgrade of the record
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

  // Locking of the record
  void        Acquire();
  bool        Release();

  // Getting contents of the record as a SQLVariant pointer
  SQLVariant& operator[](int p_index);
  SQLVariant& operator[](const char* p_name);

private:
  SQLDataSet* m_dataSet;
  bool        m_modifiable;
  int         m_status;
  ulong       m_reference;
  SQLFields   m_fields;
  int         m_generator;
};

inline int
SQLRecord::GetStatus()
{
  return m_status;
}

inline void
SQLRecord::SetGenerator(int p_generator)
{
  m_generator = p_generator;
}

inline int
SQLRecord::GetGenerator()
{
  return m_generator;
}

// End of namespace
}