////////////////////////////////////////////////////////////////////////
//
// File: SQLPrimaryKey.h
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
#include "SQLComponents.h"
#include "SQLVariant.h"
#include "SQLDataSet.h"

namespace SQLComponents
{

enum class PKStatus
{
  PK_Empty = 0        // Newly created PrimaryKey object
 ,PK_Value            // Value set by program
 ,PK_NotFound         // Searched in the database but not found
 ,PK_Created          // Newly created in the database
 ,PK_AsyncCreate      // No value. Will be created later / Accepted async transaction
 ,PK_Unknown          // Is created, but not returned by service or PSM
};

class SQLPrimaryKey
{
public:
  SQLPrimaryKey();
  explicit SQLPrimaryKey(WordList p_fields);
 ~SQLPrimaryKey();

  void        Reset();

  // SETTERS
  void        SetFields(const WordList    p_fields);
  void        SetValues(const VariantSet* p_values);
  void        SetStatus(const PKStatus    p_status);

  // GETTERS
  PKStatus    GetStatus()  { return m_status; }
  WordList&   GetFields()  { return m_fields; }
  VariantSet& GetValues()  { return m_values; }
  bool        GetIsNULL();
  int         GetNumberOfFields();
  int         GetNumberOfValues();
  XString     GetPrimaryFields();
  XString     GetPrimaryValue();
  int         GetOptimizedObject();
  XString     GetCondition();

  // Functions
  void        AddField(XString p_field,SQLVariant& p_value);
  void        AddValue(SQLVariant* p_val,bool p_replace = false);
  // Operators
  SQLPrimaryKey& operator=(const SQLPrimaryKey& p_other);

private:
  WordList   m_fields;        // Names of the primary key fields
  VariantSet m_values;        // Values of the primary key for an instance
  int        m_object { 0 };  // Optimization if the primary key is one (1) integer field
  PKStatus   m_status { PKStatus::PK_Empty };   // Current status
};

}
