////////////////////////////////////////////////////////////////////////
//
// File: SQLMutation.h
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

#include "SQLVariant.h"
#include <vector>
#include <list>

namespace SQLComponents
{

// Status of mutation against the mutationID
typedef enum _mutType
{
  MUT_NoMutation   = 0
 ,MUT_MyMutation   = 1
 ,MUT_OnlyOthers   = 2
 ,MUT_Mixed        = 4 // My mutations and other mutations
}
MutType;

//////////////////////////////////////////////////////////////////////////
//
// HELPER CLASS FOR THE MUTATION STACK
//
//////////////////////////////////////////////////////////////////////////

class Mutation
{
public:
  Mutation(); 
  int         m_mutationID;
  SQLVariant* m_value;
};

inline Mutation::Mutation()
{
  m_mutationID = 0;
  m_value = NULL;
}

// The mutation stack is a list: a limited number of mutations may exist at any one time
// So the list is two-sided reachable and searchable
typedef std::list<Mutation*> MutationStack;
typedef std::vector<int>     MutationIDS;

class SQLMutation
{
public:
  SQLMutation();
  explicit SQLMutation(const SQLVariant* p_base);
 ~SQLMutation();

  // Add new mutated state of last known SQLVariant
  void        Add(const SQLVariant* p_extra,int p_mutationID = 0);
  // Possibly mutated variant
  bool        Mutate(const SQLVariant* p_mutate,int p_mutationID = 0);
  // Cancel mutation (remove from stack by canceling window)
  unsigned    Cancel(int p_mutationID);
  // Return the current top of stack: used as last-recent-state
  SQLVariant* Current();
  // Return the original SQLVariant; bottom of stack
  SQLVariant* Original();
  // Return the variant of a mutation
  SQLVariant* MutationValue(int p_mutationID);
  // Return the mutation id of the top of stack (current mutation)
  int         CurrentMutationID();
  // Still original value (no mutations)
  bool        IsOriginal();
  // Contains mutations
  bool        IsMutated();
  // Contains mixed mutations
  MutType     MixedMutations(int p_mutationID);
  // Reduce the mutations, after a database synchronization
  void        Reduce();
  // For reporting/analysis purposes: all mutationID's on the stack
  int         AllMixedMutations(MutationIDS& p_list,int p_mutationID);

private:
  MutationStack m_stack;
};

inline SQLVariant*
SQLMutation::Original()
{
  return m_stack.front()->m_value;
}

inline SQLVariant*
SQLMutation::Current()
{
  return m_stack.back()->m_value;
}

inline bool
SQLMutation::IsOriginal()
{
  return m_stack.size() == 1;
}

inline bool 
SQLMutation::IsMutated()
{
  return m_stack.size() > 1;
}

// End of namespace
}