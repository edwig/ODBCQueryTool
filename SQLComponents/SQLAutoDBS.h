/////////////////////////////////////////////////////////////////////////////////
//
// SourceFile: SQLAutoDBS.h
//
// Copyright (c) 1998-2025 ir. W.E. Huisman
// All rights reserved
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// Version number: See SQLComponents.h
//
#pragma once
#include "SQLDatabase.h"
#include "SQLDatabasePool.h"

namespace SQLComponents
{

class SQLAutoDBS
{
public:
  SQLAutoDBS(SQLDatabasePool& p_pool,XString p_connection,SQLDatabase* p_prior = nullptr)
            :m_pool(p_pool)
  {
    if(p_prior)
    {
      m_database = p_prior;
      m_poolDbs  = false;
    }
    else
    {
      m_database = m_pool.GetDatabase(p_connection);
      m_poolDbs  = true;
    }
  }

 ~SQLAutoDBS()
  {
    if(m_database && m_poolDbs)
    {
      m_database->RegisterLogContext(0,nullptr,nullptr,NULL);
      m_pool.GiveUp(m_database);
    }
  }

  bool Invalid()             { return (m_database == nullptr);  };
  bool Valid()               { return (m_database != nullptr);  };
  operator SQLDatabase*()    { return  m_database; };
  SQLDatabase* get()         { return  m_database; };
  SQLDatabase* operator->()  { return  m_database; };
  SQLDatabase& operator*()   { return *m_database; };
private:
  bool             m_poolDbs;
  SQLDatabasePool& m_pool;
  SQLDatabase*     m_database;
};

}
