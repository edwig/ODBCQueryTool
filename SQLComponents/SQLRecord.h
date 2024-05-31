////////////////////////////////////////////////////////////////////////
//
// File: SQLRecord.h
//
// Copyright (c) 1998-2024 ir. W.E. Huisman
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
  explicit SQLRecord(SQLDataSet* p_set,bool p_modifiable = false);
 ~SQLRecord();
  // Get the status of the record
  int         GetStatus() const;
  SQLVariant* GetField(int p_num) const;
  SQLVariant* GetField(XString p_name) const;
  int         GetGenerator() const;
  // Setting a generator column
  void        SetGenerator(int p_generator);
  // Adding a field to the record
  int         AddField(const SQLVariant* p_field,bool p_insert = false);
  // Setting different value without changing status
  bool        SetField   (int     p_num, const SQLVariant* p_field,int p_mutationID = 0);
  bool        SetField   (XString p_name,const SQLVariant* p_field,int p_mutationID = 0);
  // Setting different value AND changing record/set status
  void        ModifyField(int         p_num, const SQLVariant* p_field,int p_mutationID = 0);
  void        ModifyField(LPCTSTR p_name,const SQLVariant* p_field,int p_mutationID = 0);

  // Modify a field from these base datatypes (Everything in a SQLVariant)
  void        ModifyField(LPCTSTR p_name,const bool&              p_field,int p_mutationID = 0);
  void        ModifyField(LPCTSTR p_name,const TCHAR&             p_field,int p_mutationID = 0);
  void        ModifyField(LPCTSTR p_name,LPCTSTR                  p_field,int p_mutationID = 0);
#ifndef UNICODE
  void        ModifyField(LPCTSTR p_name,const _TUCHAR&           p_field,int p_mutationID = 0);
#endif
  void        ModifyField(LPCTSTR p_name,const short&             p_field,int p_mutationID = 0);
  void        ModifyField(LPCTSTR p_name,const unsigned short&    p_field,int p_mutationID = 0);
  void        ModifyField(LPCTSTR p_name,const int&               p_field,int p_mutationID = 0);
  void        ModifyField(LPCTSTR p_name,const unsigned int&      p_field,int p_mutationID = 0);
  void        ModifyField(LPCTSTR p_name,const float&             p_field,int p_mutationID = 0);
  void        ModifyField(LPCTSTR p_name,const double&            p_field,int p_mutationID = 0);
  void        ModifyField(LPCTSTR p_name,const __int64&           p_field,int p_mutationID = 0);
  void        ModifyField(LPCTSTR p_name,const unsigned __int64&  p_field,int p_mutationID = 0);
  void        ModifyField(LPCTSTR p_name,const SQLDate&           p_field,int p_mutationID = 0);
  void        ModifyField(LPCTSTR p_name,const SQLTime&           p_field,int p_mutationID = 0);
  void        ModifyField(LPCTSTR p_name,const SQLTimestamp&      p_field,int p_mutationID = 0);
  void        ModifyField(LPCTSTR p_name,const SQLInterval&       p_field,int p_mutationID = 0);
  void        ModifyField(LPCTSTR p_name,const SQLGuid&           p_field,int p_mutationID = 0);
  void        ModifyField(LPCTSTR p_name,const XString&           p_field,int p_mutationID = 0);
  void        ModifyField(LPCTSTR p_name,const bcd&               p_field,int p_mutationID = 0);

  bool              GetFieldBool           (LPCTSTR p_name);
  char              GetFieldChar           (LPCTSTR p_name);
  unsigned char     GetFieldUnsignedChar   (LPCTSTR p_name);
  short             GetFieldShort          (LPCTSTR p_name);
  unsigned short    GetFieldUnsignedShort  (LPCTSTR p_name);
  int               GetFieldInteger        (LPCTSTR p_name);
  unsigned int      GetFieldUnsignedInteger(LPCTSTR p_name);
  float             GetFieldFloat          (LPCTSTR p_name);
  double            GetFieldDouble         (LPCTSTR p_name);
  __int64           GetFieldInteger64      (LPCTSTR p_name);
  unsigned __int64  GetFieldUnsignedInt64  (LPCTSTR p_name);
  SQLDate           GetFieldSQLDate        (LPCTSTR p_name);
  SQLTime           GetFieldSQLTime        (LPCTSTR p_name);
  SQLTimestamp      GetFieldSQLTimestamp   (LPCTSTR p_name);
  SQLInterval       GetFieldSQLInterval    (LPCTSTR p_name);
  SQLGuid           GetFieldSQLGuid        (LPCTSTR p_name);
  XString           GetFieldXString        (LPCTSTR p_name);
  bcd               GetFieldBcd            (LPCTSTR p_name);

  // See if the record has been changed
  bool        IsModified() const;
  // See if a field is modified
  bool        IsModified(int p_num) const;
  bool        IsModified(XString p_name) const;
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
  SQLVariant& operator[](LPCTSTR p_name);

private:
  SQLDataSet* m_dataSet;
  bool        m_modifiable;
  int         m_status;
  ulong       m_reference;
  SQLFields   m_fields;
  int         m_generator;
};

inline int
SQLRecord::GetStatus() const
{
  return m_status;
}

inline void
SQLRecord::SetGenerator(int p_generator)
{
  m_generator = p_generator;
}

inline int
SQLRecord::GetGenerator() const
{
  return m_generator;
}

// End of namespace
}