////////////////////////////////////////////////////////////////////////
//
// File: SQLPrimaryKey.cpp
//
// Copyright (c) 1998-2022 ir. W.E. Huisman
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
#include "SQLPrimaryKey.h"
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{

SQLPrimaryKey::SQLPrimaryKey()
{
}

SQLPrimaryKey::SQLPrimaryKey(WordList p_fields)
{
  m_fields = p_fields;
}

SQLPrimaryKey::~SQLPrimaryKey()
{
  Reset();
}

void
SQLPrimaryKey::Reset()
{
  for (auto& var : m_values)
  {
    delete var;
  }
  m_values.clear();
  m_fields.clear();
  m_object = 0;
  m_status = PKStatus::PK_Empty;
}

// SETTERS
void
SQLPrimaryKey::SetFields(WordList p_fields)
{
  m_fields = p_fields;
}

void
SQLPrimaryKey::SetValues(VariantSet* p_values)
{
  m_values = *p_values;
  GetOptimizedObject();
  m_status = PKStatus::PK_Value;
}

// Really? Are you sure? Should you do this?
// Only touch this when processing PSM!
void
SQLPrimaryKey::SetStatus(PKStatus p_status)
{
  m_status = p_status;
}

// GETTERS

bool
SQLPrimaryKey::GetIsNULL()
{
  return m_values.empty();
}

int
SQLPrimaryKey::GetNumberOfFields()
{
  return (int)m_fields.size();
}

int
SQLPrimaryKey::GetNumberOfValues()
{
  return (int)m_values.size();
}

// Functions

XString
SQLPrimaryKey::GetPrimaryFields()
{
  XString fields;

  for(auto& field : m_fields)
  {
    if(fields.GetLength())
    {
      fields += ',';
    }
    fields += field;
  }
  return fields;
}

// Currently only working for numbers and strings
XString
SQLPrimaryKey::GetPrimaryValue()
{
  XString total;

  for(auto& val : m_values)
  {
    if(total.GetLength())
    {
      total += ',';
    }

    CString key;
    val->GetAsString(key);
    if(val->IsNumericType())
    {
      total += key;
    }
    else
    {
      total += '\'';
      total += key;
      total += '\'';
    }
  }
  return total;
}

// Network databases work with a primary key of one INTEGER
int
SQLPrimaryKey::GetOptimizedObject()
{
  if(m_object == 0)
  {
    if(m_values.size() == 1)
    {
      if(m_values.front()->GetDataType() == SQL_C_SLONG)
      {
        m_object = m_values.front()->GetAsSLong();
      }
    }
  }
  return m_object;
}

XString
SQLPrimaryKey::GetCondition()
{
  XString condition;
  XString key;

  WordList::iterator it = m_fields.begin();
  VariantSet::iterator vl = m_values.begin();
  while (it != m_fields.end() && vl != m_values.end())
  {
    if(!condition.IsEmpty())
    {
      condition += " AND ";
    }

    (*vl)->GetAsString(key);
    condition += *it;
    condition += " = ";
    if((*vl)->IsNumericType())
    {
      condition += key;
    }
    else
    {
      condition += '\'';
      condition += key;
      condition += '\'';
    }
    // Next field
    ++it;
    ++vl;
  }
  return condition;
}

void
SQLPrimaryKey::AddValue(SQLVariant* p_val,bool p_replace /*=false*/)
{
  if(p_replace && !m_values.empty())
  {
    for(auto& val : m_values)
    {
      delete val;
    }
    m_values.clear();
    m_object = 0;
  }
  m_values.push_back(p_val);
  m_status = PKStatus::PK_Created;
}

SQLPrimaryKey& 
SQLPrimaryKey::operator=(SQLPrimaryKey& p_other)
{
  m_fields = p_other.m_fields;
  m_object = p_other.m_object;
  m_status = p_other.m_status;

  for(auto& var : p_other.m_values)
  {
    m_values.push_back(new SQLVariant(var));
  }

  return *this;
}

}