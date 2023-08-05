////////////////////////////////////////////////////////////////////////
//
// File: SQLMutation.cpp
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
#include "SQLMutation.h"
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{

SQLMutation::SQLMutation()
{
}

SQLMutation::SQLMutation(const SQLVariant* p_base)
{
  // Bottom of the stack
  Mutation* mut = new Mutation();
  mut->m_value  = new SQLVariant(p_base);
  mut->m_mutationID = 0;
  m_stack.push_back(mut);
}

SQLMutation::~SQLMutation()
{
  MutationStack::iterator it;
  for(it = m_stack.begin();it != m_stack.end(); ++it)
  {
    Mutation* mut = *it;
    delete mut->m_value;
    delete mut;
  }
}

void
SQLMutation::Add(const SQLVariant* p_extra,int p_mutationID /*=0*/)
{
  Mutation* mut     = new Mutation();
  mut->m_mutationID = p_mutationID;
  mut->m_value      = new SQLVariant(p_extra);
  m_stack.push_back(mut);
}

bool
SQLMutation::Mutate(const SQLVariant* p_mutate,int p_mutationID /*=0*/)
{
  // Rely on the operator== from SQLVariant!!
  if(*(m_stack.back()->m_value) == *p_mutate)
  {
    return false;
  }
  // Not the original AND same mutation ID
  if(m_stack.size() > 1 && m_stack.back()->m_mutationID == p_mutationID)
  {
    delete m_stack.back()->m_value;
    m_stack.back()->m_value = new SQLVariant(p_mutate);
  }
  else
  {
    // Add new mutation to the stack
    Add(p_mutate,p_mutationID);
  }
  return true;
}

// Cancel mutation (remove from stack by canceling window)
unsigned
SQLMutation::Cancel(int p_mutationID)
{
  MutationStack::iterator it = m_stack.begin();
  // Skip first (original) value;
  ++it;
  // Walk the mutation stack
  while(it != m_stack.end())
  {
    Mutation* mut = *it;
    if(p_mutationID == 0 || mut->m_mutationID == p_mutationID)
    {
      SQLVariant* var = mut->m_value;
      delete var;
      delete mut;
      m_stack.erase(it);
      break;
    }
    ++it;
  }
  // Returns number of remaining values
  return (int)m_stack.size();
}

// Return the variant of a mutation
SQLVariant*
SQLMutation::MutationValue(int p_mutationID)
{
  MutationStack::iterator it = m_stack.end();
  while(m_stack.size())
  {
    // Begin at the back of the list
    --it;
    // Take last of the stack first
    if((*it)->m_mutationID == p_mutationID)
    {
      return (*it)->m_value;
    }
    if(it == m_stack.begin())
    {
      break;
    }
  }
  // Return the original value
  if(m_stack.size())
  {
    return m_stack.front()->m_value;
  }
  return NULL;
}

// Returns the current mutation id from TOS
int
SQLMutation::CurrentMutationID()
{
  if(m_stack.size())
  {
    return m_stack.back()->m_mutationID;
  }
  return 0;
}

// Contains mixed mutations
MutType
SQLMutation::MixedMutations(int p_mutationID)
{
  if(p_mutationID == 0)
  {
    return MUT_MyMutation;
  }

  MutType type = MUT_NoMutation;
  for(const auto mutate : m_stack)
  {
    if(mutate->m_mutationID > 0)
    {
      if(mutate->m_mutationID == p_mutationID)
      {
        if(type == MUT_OnlyOthers)
        {
          // Return directly so we can abort any mutations
          return MUT_Mixed;
        }
        type = MUT_MyMutation;
      }
      else
      {
        // Contains a mutation of another ID
        if(type == MUT_MyMutation)
        {
          // Return directly so we can abort any mutations
          return MUT_Mixed;
        }
        type = MUT_OnlyOthers;
      }
    }
  }
  // resulting mutation type
  return type;
}

// Reduce all mutations after synchronization with the database
// Leaving only the last mutation (TOS) intact.
void
SQLMutation::Reduce()
{
  // Leave only the TOS intact.
  while(m_stack.size() > 1)
  {
    Mutation* mutation = m_stack.front();
    SQLVariant*  field = mutation->m_value;

    m_stack.pop_front();
    delete mutation;
    delete field;
  }
  // Only one mutation left.
  // Set mutation to 'The Original'
  m_stack.front()->m_mutationID = 0;
}

// For reporting/analysis purposes: all mutationID's on the stack
int
SQLMutation::AllMixedMutations(MutationIDS& p_list,int p_mutationID)
{
  // Check if we have mixed mutations
  if(MixedMutations(p_mutationID) != MUT_Mixed)
  {
    return 0;
  }

  // Walk the stack for the other mutation ID's
  for(auto& mut : m_stack)
  {
    int mutationID = mut->m_mutationID;
    if(mutationID && mutationID != p_mutationID)
    {
      if(std::find(p_list.begin(),p_list.end(),mutationID) == p_list.end())
      {
        p_list.push_back(mutationID);
      }
    }
  }
  return (int)p_list.size();
}

// End of namespace
}

