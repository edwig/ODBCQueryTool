////////////////////////////////////////////////////////////////////////
//
// File: SQLOperator.h
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

namespace SQLComponents
{

// Operator for a condition filter
typedef enum _sqlOperator
{
   OP_NOP   = 0
  ,OP_Equal = 1
  ,OP_NotEqual
  ,OP_Greater
  ,OP_GreaterEqual
  ,OP_Smaller
  ,OP_SmallerEqual
  ,OP_IsNULL
  ,OP_IsNotNULL
  ,OP_LikeBegin
  ,OP_LikeMiddle
  ,OP_LikeEnd
  ,OP_IN
  ,OP_Between
  ,OP_Exists
  ,OP_OR               // OR chaining to next filter(s)
}
SQLOperator;

// Word translation of operators for filters, needed for XMLMessages
typedef struct _operatorName
{
  LPCTSTR m_name;
  int     m_operator;
}
OperatorName;

extern OperatorName operatornames[];


}
