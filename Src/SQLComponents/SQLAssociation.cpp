////////////////////////////////////////////////////////////////////////
//
// File: SQLAssociation.cpp
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
#include "Stdafx.h"
#include "SQLComponents.h"
#include "SQLAssociation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{

SQLAssociation::SQLAssociation()
{

}

SQLAssociation::~SQLAssociation()
{
  FreeAssocs();
}

void
SQLAssociation::FreeAssocs()
{
  for(unsigned ind = 0;ind < m_assocs.size();++ind)
  {
    delete m_assocs[ind];
  }
  m_assocs.clear();
}

void 
SQLAssociation::SetAssociation(CString     p_primaryColumn
                              ,CString     p_foreignColumn
                              ,SQLVariant* p_value /*= NULL*/)
{
  PrimaryForeign* pf = new PrimaryForeign();
  pf->m_primary = p_primaryColumn;
  pf->m_foreign = p_foreignColumn;
  pf->m_value   = p_value;
  m_assocs.push_back(pf);
}

void 
SQLAssociation::SetAssociation(int p_num,SQLVariant* p_value)
{
  if(p_num >= 0 && p_num < (int)m_assocs.size())
  {
    delete m_assocs[p_num]->m_value;
    m_assocs[p_num]->m_value = p_value;
  }
}

// Check if everything needed is givven
// 1) From master to details (p_toDetails = true)
// 2) From details to master (p_toDetails = false)
bool
SQLAssociation::BasicChecks(bool p_toDetails)
{
  bool result = false;

  // Check 1: Master and detail both there
  if(m_master && m_detail)
  {
    // Check 2: Association columns filled
    if(m_assocs.size() > 0)
    {
      if(p_toDetails)
      {
        // Check 3: Master record must be givven
        if(m_masterRecord)
        {
          result = true;
        }
      }
      else
      {
        // From details to master: this is enough
        result = true;
      }
    }
  }
  return result;
}

bool 
SQLAssociation::UpdateMaster()
{
  // Check if we can do the update
  if(!BasicChecks(false))
  {
    return false;
  }
  for(unsigned ind = 0;ind < m_assocs.size();++ind)
  {
    m_master->SetParameter(m_assocs[ind]->m_primary,*(m_assocs[ind]->m_value));
  }
  if(m_master->IsOpen())
  {
    return m_master->Append();
  }
  return m_master->Open();
}

bool 
SQLAssociation::UpdateDetails()
{
  // Check if we can do the update
  if(!BasicChecks(true))
  {
    return false;
  }
  for(unsigned ind = 0;ind < m_assocs.size();++ind)
  {
    m_detail->SetParameter(m_assocs[ind]->m_foreign,*(m_assocs[ind]->m_value));
  }
  if(m_detail->IsOpen())
  {
    return m_detail->Append();
  }
  return m_detail->Open();
}

// End of namespace
}
