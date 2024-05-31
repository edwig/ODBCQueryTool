////////////////////////////////////////////////////////////////////////
//
// File: SQLVariantFormatOperator.h
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

namespace SQLComponents
{

class SQLVariant;

// Operator types 
// (Only used to produce the correct exceptions)
typedef enum _variantOperator
{
  SVO_PreIncrement
 ,SVO_PreDecrement
 ,SVO_PostIncrement
 ,SVO_PostDecrement
 ,SVO_AssignAdd
 ,SVO_AssignSubtract
 ,SVO_AssignMultiply
 ,SVO_AssignDivide
 ,SVO_AssignModulo
}
SQLVarOperator;

// Datatypes, same as SQL_C_* but now as an ordinal type
// so we can create a function pointer array from the values

typedef enum _conciseType
{
  CT_CHAR     = 0             // SQL_C_CHAR
 ,CT_SSHORT                   // SQL_C_SHORT     / SQL_C_SSHORT
 ,CT_USHORT                   // SQL_C_USHORT
 ,CT_SLONG                    // SQL_C_LONG      / SQL_C_SLONG
 ,CT_ULONG                    // SQL_C_ULONG
 ,CT_FLOAT                    // SQL_C_FLOAT
 ,CT_DOUBLE                   // SQL_C_DOUBLE
 ,CT_BIT                      // SQL_C_BIT
 ,CT_STINYINT                 // SQL_C_TINYINT   / SQL_C_STINYINT
 ,CT_UTINYINT                 // SQL_C_UTINYINT
 ,CT_SBIGINT                  // SQL_C_SBIGINT
 ,CT_UBIGINT                  // SQL_C_UBIGINT
 ,CT_NUMERIC                  // SQL_C_NUMERIC
 ,CT_GUID                     // SQL_C_GUID
 ,CT_BINARY                   // SQL_C_BINARY
 ,CT_DATE                     // SQL_C_DATE      / SQL_C_TYPE_DATE
 ,CT_TIME                     // SQL_C_TIME      / SQL_C_TYPE_TIME
 ,CT_TIMESTAMP                // SQL_C_TIMESTAMP / SQL_C_TYPE_TIMESTAMP
 ,CT_INTERVAL_YM              // SQL_C_INTERVAL_YEAR / SQL_C_INTERVAL_MONTH / SQL_C_INTERVAL_YEAR_TO_MONTH
 ,CT_INTERVAL_DS              // SQL_C_INTERVAL_DAY -> SQL_C_INTERVAL_DAY_TO_SECOND
 ,CT_LAST                     // LAST = ARRAY SIZE
}
SQLConciseType;

SQLConciseType SQLTypeToConciseType(int p_datatype);

typedef SQLVariant (*OperatorCalculate)(const SQLVariant& p_left,const SQLVariant& p_right);
typedef bool       (*OperatorCompare  )(const SQLVariant& p_left,const SQLVariant& p_right);

struct _CompareFunc
{
  OperatorCompare function;
};

struct _CalculateFunc
{
  OperatorCalculate function;
};

typedef struct _CompareFunc   CompareFunctionArray;
typedef struct _CalculateFunc CalculateFunctionArray;

// End of namespace
}
