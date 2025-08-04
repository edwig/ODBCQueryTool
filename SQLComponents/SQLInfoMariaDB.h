////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoMariaDB.h
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
#include "SQLInfoMySQL.h"

namespace SQLComponents
{

class SQLInfoMariaDB : public SQLInfoMySQL
{
public:
  explicit SQLInfoMariaDB(SQLDatabase* p_database);
 ~SQLInfoMariaDB();

  // RDBMS Uses INDENTITY or SEQUENCE interface
  void    SetUseSequences(bool p_sequences) override;

  //////////////////////////////////////////////////////////////////////////
  //
  // GENERALS (Strings & Booleans) 
  //
  //////////////////////////////////////////////////////////////////////////

  // Get the database type
  DatabaseType GetRDBMSDatabaseType() const override;

  // The name of the database vendor
  XString GetRDBMSVendorName() const override;

  // Get the physical database name
  XString GetRDBMSPhysicalDatabaseName() const override;

  // Maximum for a VARCHAR to be handled without AT-EXEC data. Assume NVARCHAR is half that size!
  int GetRDBMSMaxVarchar() const override;

  // Identifier rules differ per RDBMS
  bool IsIdentifier(XString p_identifier) const override;

  // KEYWORDS

  // Get the concatenation operator
  XString GetKEYWORDConcatanationOperator() const override;

  // Get default NULL for parameter list input
  XString GetKEYWORDParameterDefaultNULL() const override;

  // Parameter is for INPUT and OUTPUT in parameter list
  XString GetKEYWORDParameterINOUT() const override;

  // Parameter is for OUTPUT only in parameter list
  XString GetKEYWORDParameterOUT() const override;

  // Get select part to add new record identity to a table
  // Can be special column like 'OID' or a sequence select
  XString GetKEYWORDIdentityString(XString& p_tablename,XString p_postfix = "_seq") const override;

  // SQL

  // Gets the construction for inline generating a key within an INSERT statement
  XString GetSQLNewSerial(XString p_table, XString p_sequence) const override;

  // Gets the construction / select for generating a new serial identity
  XString GetSQLGenerateSerial(XString p_table) const override;
  XString GetSQLGenerateSequence(XString p_sequence) const override;

  // Gets the sub-transaction commands
  XString GetSQLStartSubTransaction   (XString p_savepointName) const override;
  XString GetSQLCommitSubTransaction  (XString p_savepointName) const override;
  XString GetSQLRollbackSubTransaction(XString p_savepointName) const override;

  // FROM-Part for a query to select only 1 (one) record / or empty!
  XString GetSQLFromDualClause() const override;

  //////////////////////////////////////////////////////////////////////////
  // SQL STRINGS

  // IDENTICAL TO MySQL

  //////////////////////////////////////////////////////////////////////////
  //
  // CATALOG
  // 
  // Almost identical to MySQL
  // - Default charset differs
  // - MariaDB has the SEQUENCE !!
  // 
  //////////////////////////////////////////////////////////////////////////

  // Meta info about meta types
  XString GetCATALOGDefaultCharset() const override;
  XString GetCATALOGDefaultCharsetNCV() const override;
  // All sequence functions
  XString GetCATALOGSequenceExists    (XString  p_schema,XString  p_sequence,bool p_quoted = false) const override;
  XString GetCATALOGSequenceList      (XString& p_schema,XString& p_pattern, bool p_quoted = false) const override;
  XString GetCATALOGSequenceAttributes(XString& p_schema,XString& p_sequence,bool p_quoted = false) const override;
  XString GetCATALOGSequenceCreate    (MetaSequence& p_sequence) const override;
  XString GetCATALOGSequenceDrop      (XString  p_schema,XString  p_sequence) const override;
  
  //////////////////////////////////////////////////////////////////////////
  //
  // SQL/PSM PERSISTENT STORED MODULES 
  // 
  // in MariaDB the sourcecode of function/procedure differs slightly
  //
  //////////////////////////////////////////////////////////////////////////

  // All procedure functions
  XString GetPSMProcedureSourcecode(XString  p_schema,XString  p_procedure,bool p_quoted = false) const override;

  
private:
  void DetectOracleMode() const;

  // Since version 10.3 MariaDB can be set to Oracle mode
  mutable bool m_oracleMode{ false };
};

// End of namespace
}

