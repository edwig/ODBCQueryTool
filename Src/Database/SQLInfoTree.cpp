////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoTree.cpp
//
// Copyright (c) 1998-2016 ir. W.E. Huisman
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
// Last Revision:   14-12-2016
// Version number:  1.3.0
//
#include "StdAfx.h"
#include "SQLInfoTree.h"
#include "SQLDatabase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

SQLInfoTree::SQLInfoTree(SQLDatabase* p_database)
            :SQLInfo(p_database)
{
}

SQLInfoTree::~SQLInfoTree()
{
}

//////////////////////////////////////////////////////////////////////////
//
// PUBLIC MAIN ROUTINES
//
//////////////////////////////////////////////////////////////////////////

// All ODBC Info to a treecontrol
void
SQLInfoTree::MakeTreeInfo(CTreeCtrl* tree)
{
  CString key;
  CString sitem;

  // Get the primary info for the tree
  GetInfo();

  HTREEITEM item = tree->GetRootItem();
  HTREEITEM child;
  HTREEITEM infoItemDB      = NULL;
  HTREEITEM infoItemDriver  = NULL;
  HTREEITEM infoItemDATAT   = NULL;
  HTREEITEM infoItemConv    = NULL;
  HTREEITEM infoItemCF      = NULL;
  HTREEITEM infoItemSQLCF   = NULL;
  HTREEITEM infoItemImpl    = NULL;
  HTREEITEM infoItemVersion = NULL;

  // Emtpy the tree
  while(true)
  {
    child = tree->GetChildItem(item);
    if(!child) break;
    tree->DeleteItem(child);
  }
  if(!SupportedFunction(SQL_API_SQLGETINFO))
  {
    tree->InsertItem("SQLGetInfo not supported: Get a better driver!",item);
    return;
  }
  infoItemDB      = tree->InsertItem("Database",              item);
  infoItemDriver  = tree->InsertItem("ODBC Driver",           item);
  infoItemDATAT   = tree->InsertItem("SQL Data types",        item);
  infoItemCF      = tree->InsertItem("SQL Conformance",       item);
  infoItemConv    = tree->InsertItem("SQL Type conversions",  item);
  infoItemSQLCF   = tree->InsertItem("SQL Supported standard",item);
  infoItemImpl    = tree->InsertItem("RDBMS Implementation",  item);
  infoItemVersion = tree->InsertItem("Version numbers",       item);

  // DATABASE NAME AND TYPE
  CString dbtype = "Database type: "  + m_database->GetDatabaseTypeName();
  CString dbname = "Database name: "  + m_database->GetDatabaseName();
  CString method = "Naming method: "  + m_database->GetNamingMethod();
  CString user   = "Logged in user: " + m_userName;

  tree->InsertItem(dbtype,infoItemDB);
  tree->InsertItem(dbname,infoItemDB);
  tree->InsertItem(method,infoItemDB);
  tree->InsertItem(user,  infoItemDB);

  WordList list;
  HTREEITEM meta = tree->InsertItem("Meta-objects in the database",infoItemDB);
  HTREEITEM catalogs = tree->InsertItem("Catalogs",meta);
  MakeInfoMetaTypes(&list,META_CATALOGS);
  WordListToTree(list,tree,catalogs);

  HTREEITEM schemas  = tree->InsertItem("Schemas",meta);
  MakeInfoMetaTypes(&list,META_SCHEMAS);
  WordListToTree(list,tree,schemas);

  HTREEITEM obtypes  = tree->InsertItem("Object types",meta);
  MakeInfoMetaTypes(&list,META_TABLES);
  WordListToTree(list,tree,obtypes);

  // ODBC DRIVER
  sitem.Format("ODBC Driver DLL: %s",m_driverName);
  tree->InsertItem(sitem,infoItemDriver);

  // ODBC SQL reserved words (static compile time)
  m_ODBCKeywords.sort();
  HTREEITEM odbcres = tree->InsertItem("ODBC SQL reserved words", infoItemDriver);
  for(WordList::iterator it = m_ODBCKeywords.begin(); it != m_ODBCKeywords.end(); ++it)
  {
    tree->InsertItem(*it,odbcres);
  }
  // Supported ODBC functions
  HTREEITEM odbcFun = tree->InsertItem("ODBC Supported functions",infoItemDriver);
  MakeTreeInfoFunctions(tree,odbcFun);

  // Driver implementation and extensions
  HTREEITEM odbcExt = tree->InsertItem("ODBC driver implementation",infoItemDriver);
  MakeTreeODBCExtensions(tree,odbcExt);

  // SQL datatypen
  bool didInfo = false;
  DataTypeMap::iterator it = m_dataTypes.begin();
  TypeInfo* ti;
  CString fmt;
  while(it != m_dataTypes.end())
  {
    didInfo = true;
    ti = it->second;
    HTREEITEM type = tree->InsertItem(ti->m_type_name,infoItemDATAT);

    sitem = "ODBC datatype name: " + ODBCDataType(ti->m_data_type);
    tree->InsertItem(sitem,type);
    sitem.Format("Maximum precision: %s",InfoNumberToString(ti->m_precision));
    tree->InsertItem(sitem,type);
    sitem = "Literal data prefix: " + ti->m_literal_prefix;
    tree->InsertItem(sitem,type);
    sitem = "Literal data suffix: " + ti->m_literal_suffix;
    tree->InsertItem(sitem,type);
    sitem = "How to create parameters: " + ti->m_create_params;
    tree->InsertItem(sitem,type);
    sitem = "Datatype is nullable: ";
    switch(ti->m_nullable)
    {
      case SQL_NO_NULLS:         sitem += "No nulls"; break;
      case SQL_NULLABLE:         sitem += "Nullable"; break;
      case SQL_NULLABLE_UNKNOWN: sitem += "Unknown";  break;
    }
    tree->InsertItem(sitem,type);
    sitem = CString("Datatype is case sensitive: ") + (ti->m_case_sensitive ? "Yes" : "No");
    tree->InsertItem(sitem,type);
    sitem = "Datatype is searchable: ";
    switch(ti->m_searchable)
    {
      case SQL_UNSEARCHABLE:    sitem += "Unsearchable";    break;
      case SQL_LIKE_ONLY:       sitem += "Like only";       break;
      case SQL_ALL_EXCEPT_LIKE: sitem += "All except like"; break;
      case SQL_SEARCHABLE:      sitem += "Searchable";      break;
    }
    tree->InsertItem(sitem,type);
    sitem = CString("Datatype is unsigned: ") + (ti->m_unsigned ? "Yes" : "No");
    tree->InsertItem(sitem,type);
    sitem = CString("Datatype is money: ") + (ti->m_money ? "Yes" : "No");
    tree->InsertItem(sitem,type);
    sitem = CString("Datatype is auto-increment: ") + (ti->m_autoincrement ? "Yes" : "No");
    tree->InsertItem(sitem,type);
    sitem = "Local type name for display: " + ti->m_local_type_name;
    tree->InsertItem(sitem,type);
    sitem.Format("Minimum scale: %s",InfoNumberToString(ti->m_minimum_scale));
    tree->InsertItem(sitem,type);
    sitem.Format("Maximum scale: %s",InfoNumberToString(ti->m_maximum_scale));
    tree->InsertItem(sitem,type);
    sitem.Format("Driver independent SQL datatype: %s",ODBCDataType(ti->m_sqlDatatype));
    tree->InsertItem(sitem,type);
    sitem.Format("Subtype for temporal types: %d",ti->m_sqlSubType);
    tree->InsertItem(sitem,type);
    sitem.Format("Decimal radix: %d",ti->m_radix);
    tree->InsertItem(sitem,type);
    sitem.Format("Interval leading precision: %d",ti->m_interval_precision);
    tree->InsertItem(sitem,type);
    // Next datatype
    ++it;
  }
  if(!didInfo)
  {
    tree->InsertItem("No info",infoItemDATAT);
  }
  // SQL CONVERSIONS
  sitem = "Conversion functions";
  HTREEITEM convfunc = tree->InsertItem(sitem,infoItemConv);
  DisplayBitfield(tree,convfunc,m_conversionFuncs,SQL_FN_CVT_CAST,   "CAST");
  DisplayBitfield(tree,convfunc,m_conversionFuncs,SQL_FN_CVT_CONVERT,"CONVERT");

  DisplayConversion(tree,infoItemConv,"BIGINT",             m_convertBigint);
  DisplayConversion(tree,infoItemConv,"BINARY",             m_convertBinary);
  DisplayConversion(tree,infoItemConv,"BIT",                m_convertBit);
  DisplayConversion(tree,infoItemConv,"CHAR",               m_convertChar);
  DisplayConversion(tree,infoItemConv,"DATE",               m_convertDate);
  DisplayConversion(tree,infoItemConv,"DECIMAL",            m_convertDecimal);
  DisplayConversion(tree,infoItemConv,"DOUBLE",             m_convertDouble);
  DisplayConversion(tree,infoItemConv,"FLOAT",              m_convertFloat);
  DisplayConversion(tree,infoItemConv,"INTEGER",            m_convertInteger);
  DisplayConversion(tree,infoItemConv,"INTERVAL YEAR-MONTH",m_convertIYM);
  DisplayConversion(tree,infoItemConv,"INTERVAL DAY-TIME",  m_convertIDT);
  DisplayConversion(tree,infoItemConv,"LONGVARBINARY",      m_convertLVarBinary);
  DisplayConversion(tree,infoItemConv,"LONGVARCHAR",        m_convertLVarchar);
  DisplayConversion(tree,infoItemConv,"NUMERIC",            m_convertNumeric);
  DisplayConversion(tree,infoItemConv,"REAL",               m_convertReal);
  DisplayConversion(tree,infoItemConv,"SMALLINT",           m_convertSmallint);
  DisplayConversion(tree,infoItemConv,"TIME",               m_convertTime);
  DisplayConversion(tree,infoItemConv,"TIMESTAMP",          m_convertTimestamp);
  DisplayConversion(tree,infoItemConv,"TINYINT",            m_convertTinyint);
  DisplayConversion(tree,infoItemConv,"VARBINARY",          m_convertVarBinary);
  DisplayConversion(tree,infoItemConv,"VARCHAR",            m_convertVarchar);

  // SQL CONFORMANCE
  sitem = "SQL conformance: ";
  switch(m_sql_conformance)
  {
    case SQL_SC_SQL92_ENTRY:             sitem += "Entry level SQL92 compliant";              break;
    case SQL_SC_FIPS127_2_TRANSITIONAL : sitem += "FIPS 127-2 transitional level compliant";  break;
    case SQL_SC_SQL92_FULL :             sitem += "Full SQL92 compliant";                     break;
    case SQL_SC_SQL92_INTERMEDIATE :     sitem += "Intermediate SQL92 compliant";             break;
    default:                             sitem += "No information";                           break;
  }
  tree->InsertItem(sitem, infoItemCF);

  sitem = "ODBC conformance: ";
  switch(m_odbc_conformance)
  {
    case SQL_OIC_CORE:   sitem += "CORE";           break;
    case SQL_OIC_LEVEL1: sitem += "LEVEL 1";        break;
    case SQL_OIC_LEVEL2: sitem += "LEVEL 2";        break;
    default:             sitem += "No information"; break;
  }
  tree->InsertItem(sitem, infoItemCF);

  sitem = "CLI conformance: ";
  switch(m_cli_conformance)
  {
    case SQL_SCC_XOPEN_CLI_VERSION1:   sitem += "X/Open CLI version 1"; break;
    case SQL_SCC_ISO92_CLI:            sitem += "ISO 92 CLI";           break;
    default:                           sitem += "No information";       break;
  }
  tree->InsertItem(sitem, infoItemCF);
  tree->InsertItem(CString("X/Open CLI Year: ")+m_cli_year, infoItemCF);

  // SQL Support
  sitem = "Aggregate functions";
  HTREEITEM agg = tree->InsertItem(sitem,infoItemSQLCF);
  if(m_aggr_functions & SQL_AF_ALL)      tree->InsertItem("All aggregates supported",agg);
  else                                   tree->InsertItem("Just these aggregates are supported",agg);
  DisplayBitfield(tree,agg,m_aggr_functions,SQL_AF_AVG,     "AVG");
  DisplayBitfield(tree,agg,m_aggr_functions,SQL_AF_COUNT,   "COUNT");
  DisplayBitfield(tree,agg,m_aggr_functions,SQL_AF_DISTINCT,"DISTINCT");
  DisplayBitfield(tree,agg,m_aggr_functions,SQL_AF_MAX,     "MAX");
  DisplayBitfield(tree,agg,m_aggr_functions,SQL_AF_MIN,     "MIN");
  DisplayBitfield(tree,agg,m_aggr_functions,SQL_AF_SUM,     "SUM");

  sitem = "Numeric functions";
  HTREEITEM numfunc = tree->InsertItem(sitem,infoItemSQLCF);
  DisplayBitfield(tree,numfunc,m_funcNumeric,SQL_FN_NUM_ABS,     "ABS");
  DisplayBitfield(tree,numfunc,m_funcNumeric,SQL_FN_NUM_ACOS,    "ACOS");
  DisplayBitfield(tree,numfunc,m_funcNumeric,SQL_FN_NUM_ASIN,    "ASIN");
  DisplayBitfield(tree,numfunc,m_funcNumeric,SQL_FN_NUM_ATAN,    "ATAN");
  DisplayBitfield(tree,numfunc,m_funcNumeric,SQL_FN_NUM_ATAN2,   "ATAN2");
  DisplayBitfield(tree,numfunc,m_funcNumeric,SQL_FN_NUM_CEILING, "CEILING");
  DisplayBitfield(tree,numfunc,m_funcNumeric,SQL_FN_NUM_COS,     "COS");
  DisplayBitfield(tree,numfunc,m_funcNumeric,SQL_FN_NUM_COT,     "COT");
  DisplayBitfield(tree,numfunc,m_funcNumeric,SQL_FN_NUM_DEGREES, "DEGREES");
  DisplayBitfield(tree,numfunc,m_funcNumeric,SQL_FN_NUM_EXP,     "EXP");
  DisplayBitfield(tree,numfunc,m_funcNumeric,SQL_FN_NUM_FLOOR,   "FLOOR");
  DisplayBitfield(tree,numfunc,m_funcNumeric,SQL_FN_NUM_LOG,     "LOG");
  DisplayBitfield(tree,numfunc,m_funcNumeric,SQL_FN_NUM_LOG10,   "LOG10");
  DisplayBitfield(tree,numfunc,m_funcNumeric,SQL_FN_NUM_MOD,     "MOD");
  DisplayBitfield(tree,numfunc,m_funcNumeric,SQL_FN_NUM_PI,      "PI");
  DisplayBitfield(tree,numfunc,m_funcNumeric,SQL_FN_NUM_POWER,   "POWER");
  DisplayBitfield(tree,numfunc,m_funcNumeric,SQL_FN_NUM_RADIANS, "RADIANS");
  DisplayBitfield(tree,numfunc,m_funcNumeric,SQL_FN_NUM_RAND,    "RAND");
  DisplayBitfield(tree,numfunc,m_funcNumeric,SQL_FN_NUM_ROUND,   "ROUND");
  DisplayBitfield(tree,numfunc,m_funcNumeric,SQL_FN_NUM_SIGN,    "SIGN");
  DisplayBitfield(tree,numfunc,m_funcNumeric,SQL_FN_NUM_SIN,     "SIN");
  DisplayBitfield(tree,numfunc,m_funcNumeric,SQL_FN_NUM_SQRT,    "SQRT");
  DisplayBitfield(tree,numfunc,m_funcNumeric,SQL_FN_NUM_TAN,     "TAN");
  DisplayBitfield(tree,numfunc,m_funcNumeric,SQL_FN_NUM_TRUNCATE,"TRUNCATE");

  sitem = "String functions";
  HTREEITEM sfunc = tree->InsertItem(sitem,infoItemSQLCF);
  DisplayBitfield(tree,sfunc,m_funcString, SQL_FN_STR_ASCII,        "ASCII");
  DisplayBitfield(tree,sfunc,m_funcString, SQL_FN_STR_BIT_LENGTH,   "BIT_LENGTH");
  DisplayBitfield(tree,sfunc,m_funcString, SQL_FN_STR_CHAR,         "CHAR");
  DisplayBitfield(tree,sfunc,m_funcString, SQL_FN_STR_CHAR_LENGTH,  "CHAR_LENGTH");
  DisplayBitfield(tree,sfunc,m_funcString, SQL_FN_STR_CHARACTER_LENGTH,"CHARACTER_LENGTH");
  DisplayBitfield(tree,sfunc,m_funcString, SQL_FN_STR_CONCAT,       "CONCAT");
  DisplayBitfield(tree,sfunc,m_funcString3,SQL_SSF_CONVERT,         "CONVERT");
  DisplayBitfield(tree,sfunc,m_funcString, SQL_FN_STR_DIFFERENCE,   "DIFFERENCE");
  DisplayBitfield(tree,sfunc,m_funcString, SQL_FN_STR_INSERT,       "INSERT");
  DisplayBitfield(tree,sfunc,m_funcString, SQL_FN_STR_LCASE,        "LCASE");
  DisplayBitfield(tree,sfunc,m_funcString, SQL_FN_STR_LEFT,         "LEFT");
  DisplayBitfield(tree,sfunc,m_funcString, SQL_FN_STR_LOCATE,       "LOCATE");
  DisplayBitfield(tree,sfunc,m_funcString3,SQL_SSF_LOWER,           "LOWER");
  DisplayBitfield(tree,sfunc,m_funcString, SQL_FN_STR_LTRIM,        "LTRIM");
  DisplayBitfield(tree,sfunc,m_funcString, SQL_FN_STR_OCTET_LENGTH, "OCTET_LENGTH");
  DisplayBitfield(tree,sfunc,m_funcString, SQL_FN_STR_POSITION,     "POSITION");
  DisplayBitfield(tree,sfunc,m_funcString, SQL_FN_STR_REPEAT,       "REPEAT");
  DisplayBitfield(tree,sfunc,m_funcString, SQL_FN_STR_REPLACE,      "REPLACE");
  DisplayBitfield(tree,sfunc,m_funcString, SQL_FN_STR_RIGHT,        "RIGHT");
  DisplayBitfield(tree,sfunc,m_funcString, SQL_FN_STR_RTRIM,        "RTRIM");
  DisplayBitfield(tree,sfunc,m_funcString, SQL_FN_STR_SOUNDEX,      "SOUNDEX");
  DisplayBitfield(tree,sfunc,m_funcString, SQL_FN_STR_SPACE,        "SPACE");
  DisplayBitfield(tree,sfunc,m_funcString, SQL_FN_STR_SUBSTRING,    "SUBSTRING");
  DisplayBitfield(tree,sfunc,m_funcString3,SQL_SSF_SUBSTRING,       "SQL92 SUBSTRING extensions");
  DisplayBitfield(tree,sfunc,m_funcString3,SQL_SSF_TRANSLATE,       "TRANSLATE");
  DisplayBitfield(tree,sfunc,m_funcString3,SQL_SSF_TRIM_BOTH,       "TRIM BOTH");
  DisplayBitfield(tree,sfunc,m_funcString3,SQL_SSF_TRIM_LEADING,    "TRIM LEADING");
  DisplayBitfield(tree,sfunc,m_funcString3,SQL_SSF_TRIM_TRAILING,   "TRIM TRAILING");
  DisplayBitfield(tree,sfunc,m_funcString, SQL_FN_STR_UCASE,        "UCASE");
  DisplayBitfield(tree,sfunc,m_funcString3,SQL_SSF_UPPER,           "UPPER");

  sitem = "Date/Time functions";
  HTREEITEM dtfunc = tree->InsertItem(sitem,infoItemSQLCF);
  DisplayBitfield(tree,dtfunc,m_funcDateTime,SQL_FN_TD_CURRENT_DATE     ,"CURRENT_DATE");
  DisplayBitfield(tree,dtfunc,m_funcDateTime,SQL_FN_TD_CURRENT_TIME     ,"CURRENT_TIME");
  DisplayBitfield(tree,dtfunc,m_funcDateTime,SQL_FN_TD_CURRENT_TIMESTAMP,"CURRENT_TIMESTAMP");
  DisplayBitfield(tree,dtfunc,m_funcDateTime,SQL_FN_TD_CURDATE          ,"CURDATE");
  DisplayBitfield(tree,dtfunc,m_funcDateTime,SQL_FN_TD_CURTIME          ,"CURTIME");
  DisplayBitfield(tree,dtfunc,m_funcDateTime,SQL_FN_TD_DAYNAME          ,"DAYNAME");
  DisplayBitfield(tree,dtfunc,m_funcDateTime,SQL_FN_TD_DAYOFMONTH       ,"DAYOFMONTH");
  DisplayBitfield(tree,dtfunc,m_funcDateTime,SQL_FN_TD_DAYOFWEEK        ,"DAYOFWEEK");
  DisplayBitfield(tree,dtfunc,m_funcDateTime,SQL_FN_TD_DAYOFYEAR        ,"DAYOFYEAR");
  DisplayBitfield(tree,dtfunc,m_funcDateTime,SQL_FN_TD_EXTRACT          ,"EXTRACT");
  DisplayBitfield(tree,dtfunc,m_funcDateTime,SQL_FN_TD_HOUR             ,"HOUR");
  DisplayBitfield(tree,dtfunc,m_funcDateTime,SQL_FN_TD_MINUTE           ,"MINUTE");
  DisplayBitfield(tree,dtfunc,m_funcDateTime,SQL_FN_TD_MONTH            ,"MONTH");
  DisplayBitfield(tree,dtfunc,m_funcDateTime,SQL_FN_TD_MONTHNAME        ,"MONTHNAME");
  DisplayBitfield(tree,dtfunc,m_funcDateTime,SQL_FN_TD_NOW              ,"NOW");
  DisplayBitfield(tree,dtfunc,m_funcDateTime,SQL_FN_TD_QUARTER          ,"QUARTER");
  DisplayBitfield(tree,dtfunc,m_funcDateTime,SQL_FN_TD_SECOND           ,"SECOND");
  HTREEITEM stampadd = DisplayBitfield(tree,dtfunc,m_funcDateTime,SQL_FN_TD_TIMESTAMPADD ,"TIMESTAMPADD");
  HTREEITEM stampdif = DisplayBitfield(tree,dtfunc,m_funcDateTime,SQL_FN_TD_TIMESTAMPDIFF,"TIMESTAMPDIFF");
  DisplayBitfield(tree,dtfunc,m_funcDateTime,SQL_FN_TD_WEEK             ,"WEEK");
  DisplayBitfield(tree,dtfunc,m_funcDateTime,SQL_FN_TD_YEAR             ,"YEAR");
  if((m_funcDateTime & SQL_FN_TD_TIMESTAMPADD) == 0)
  {
    m_funcTimestampadd = 0;
  }
  if((m_funcDateTime & SQL_FN_TD_TIMESTAMPDIFF) == 0)
  {
    m_funcTimestampdiff = 0;
  }
  // Options for timestamp ADD/DIFF
  DisplayBitfield(tree,stampadd,m_funcTimestampadd,SQL_FN_TSI_FRAC_SECOND,"Fractions of a second");
  DisplayBitfield(tree,stampadd,m_funcTimestampadd,SQL_FN_TSI_SECOND,     "SECOND");
  DisplayBitfield(tree,stampadd,m_funcTimestampadd,SQL_FN_TSI_MINUTE,     "MINUTE");
  DisplayBitfield(tree,stampadd,m_funcTimestampadd,SQL_FN_TSI_HOUR,       "HOUR");
  DisplayBitfield(tree,stampadd,m_funcTimestampadd,SQL_FN_TSI_DAY,        "DAY");
  DisplayBitfield(tree,stampadd,m_funcTimestampadd,SQL_FN_TSI_WEEK,       "WEEK");
  DisplayBitfield(tree,stampadd,m_funcTimestampadd,SQL_FN_TSI_MONTH,      "MONTH");
  DisplayBitfield(tree,stampadd,m_funcTimestampadd,SQL_FN_TSI_QUARTER,    "QUARTER");
  DisplayBitfield(tree,stampadd,m_funcTimestampadd,SQL_FN_TSI_YEAR,       "YEAR");

  DisplayBitfield(tree,stampdif,m_funcTimestampdiff,SQL_FN_TSI_FRAC_SECOND,"Fractions of a second");
  DisplayBitfield(tree,stampdif,m_funcTimestampdiff,SQL_FN_TSI_SECOND,     "SECOND");
  DisplayBitfield(tree,stampdif,m_funcTimestampdiff,SQL_FN_TSI_MINUTE,     "MINUTE");
  DisplayBitfield(tree,stampdif,m_funcTimestampdiff,SQL_FN_TSI_HOUR,       "HOUR");
  DisplayBitfield(tree,stampdif,m_funcTimestampdiff,SQL_FN_TSI_DAY,        "DAY");
  DisplayBitfield(tree,stampdif,m_funcTimestampdiff,SQL_FN_TSI_WEEK,       "WEEK");
  DisplayBitfield(tree,stampdif,m_funcTimestampdiff,SQL_FN_TSI_MONTH,      "MONTH");
  DisplayBitfield(tree,stampdif,m_funcTimestampdiff,SQL_FN_TSI_QUARTER,    "QUARTER");
  DisplayBitfield(tree,stampdif,m_funcTimestampdiff,SQL_FN_TSI_YEAR,       "YEAR");

  sitem = "System functions";
  HTREEITEM sysfunc = tree->InsertItem(sitem,infoItemSQLCF);
  DisplayBitfield(tree,sysfunc,m_funcSystem,SQL_FN_SYS_DBNAME,  "DATABASE");
  DisplayBitfield(tree,sysfunc,m_funcSystem,SQL_FN_SYS_IFNULL,  "IFNULL");
  DisplayBitfield(tree,sysfunc,m_funcSystem,SQL_FN_SYS_USERNAME,"USER");

  sitem = "Alter domain";
  HTREEITEM adom = tree->InsertItem(sitem,infoItemSQLCF);
  DisplayBitfield(tree,adom,m_alterDomain,SQL_AD_ADD_DOMAIN_CONSTRAINT     ,        "Add domain constraint");
  DisplayBitfield(tree,adom,m_alterDomain,SQL_AD_ADD_DOMAIN_DEFAULT        ,        "Add domain default");
  DisplayBitfield(tree,adom,m_alterDomain,SQL_AD_CONSTRAINT_NAME_DEFINITION,        "Constraint name definition");
  DisplayBitfield(tree,adom,m_alterDomain,SQL_AD_DROP_DOMAIN_CONSTRAINT    ,        "Drop domain constraint");
  DisplayBitfield(tree,adom,m_alterDomain,SQL_AD_DROP_DOMAIN_DEFAULT       ,        "Drop domain default");
  DisplayBitfield(tree,adom,m_alterDomain,SQL_AD_ADD_CONSTRAINT_DEFERRABLE ,        "Add constraint deferrable");
  DisplayBitfield(tree,adom,m_alterDomain,SQL_AD_ADD_CONSTRAINT_NON_DEFERRABLE     ,"Add constraint non deferrable");
  DisplayBitfield(tree,adom,m_alterDomain,SQL_AD_ADD_CONSTRAINT_INITIALLY_DEFERRED ,"Add constraint initially deferred");
  DisplayBitfield(tree,adom,m_alterDomain,SQL_AD_ADD_CONSTRAINT_INITIALLY_IMMEDIATE,"Add constraint initially immediate");

  sitem = "Alter table";
  HTREEITEM alt = tree->InsertItem(sitem,infoItemSQLCF);
  DisplayBitfield(tree,alt,m_alterTable,SQL_AT_ADD_COLUMN,                    "Add column");
  DisplayBitfield(tree,alt,m_alterTable,SQL_AT_DROP_COLUMN,                   "Drop column");
  DisplayBitfield(tree,alt,m_alterTable,SQL_AT_ADD_CONSTRAINT,                "Add constraint");
  DisplayBitfield(tree,alt,m_alterTable,SQL_AT_ADD_COLUMN_COLLATION,          "Add column with collation");
  DisplayBitfield(tree,alt,m_alterTable,SQL_AT_ADD_COLUMN_DEFAULT,            "Add column with default");
  DisplayBitfield(tree,alt,m_alterTable,SQL_AT_ADD_COLUMN_SINGLE,             "Add column (single)");
  DisplayBitfield(tree,alt,m_alterTable,SQL_AT_ADD_CONSTRAINT,                "Add column constraint");
  DisplayBitfield(tree,alt,m_alterTable,SQL_AT_ADD_TABLE_CONSTRAINT,          "Add table constraint");
  DisplayBitfield(tree,alt,m_alterTable,SQL_AT_CONSTRAINT_NAME_DEFINITION,    "Add constraint name definition");
  DisplayBitfield(tree,alt,m_alterTable,SQL_AT_DROP_COLUMN_CASCADE,           "Drop column cascade");
  DisplayBitfield(tree,alt,m_alterTable,SQL_AT_DROP_COLUMN_DEFAULT,           "Drop column default");
  DisplayBitfield(tree,alt,m_alterTable,SQL_AT_DROP_COLUMN_RESTRICT,          "Drop column restrict");
  DisplayBitfield(tree,alt,m_alterTable,SQL_AT_DROP_TABLE_CONSTRAINT_CASCADE, "Drop table constraint cascade");
  DisplayBitfield(tree,alt,m_alterTable,SQL_AT_DROP_TABLE_CONSTRAINT_RESTRICT,"Drop table constraint restrict");
  DisplayBitfield(tree,alt,m_alterTable,SQL_AT_SET_COLUMN_DEFAULT,            "Set column default");
  DisplayBitfield(tree,alt,m_alterTable,SQL_AT_CONSTRAINT_INITIALLY_DEFERRED, "Constraints are initially deferred");
  DisplayBitfield(tree,alt,m_alterTable,SQL_AT_CONSTRAINT_INITIALLY_IMMEDIATE,"Constraints are initially immediate");
  DisplayBitfield(tree,alt,m_alterTable,SQL_AT_CONSTRAINT_DEFERRABLE,         "Constraints are deferrable");
  DisplayBitfield(tree,alt,m_alterTable,SQL_AT_CONSTRAINT_NON_DEFERRABLE,     "Constraints are not deferrable");

  sitem = "Create schema";
  HTREEITEM crsch = tree->InsertItem(sitem,infoItemSQLCF);
  DisplayBitfield(tree,crsch,m_createSchema,SQL_CS_CREATE_SCHEMA,        "Create schema");
  DisplayBitfield(tree,crsch,m_createSchema,SQL_CS_AUTHORIZATION,        "Authorization clause for owner");
  DisplayBitfield(tree,crsch,m_createSchema,SQL_CS_DEFAULT_CHARACTER_SET,"Create default character set");

  sitem = "Create domain";
  HTREEITEM cdom = tree->InsertItem(sitem,infoItemSQLCF);
  DisplayBitfield(tree,cdom,m_createDomain,SQL_CDO_CREATE_DOMAIN,                  "Create domain");
  DisplayBitfield(tree,cdom,m_createDomain,SQL_CDO_DEFAULT,                        "Default value");
  DisplayBitfield(tree,cdom,m_createDomain,SQL_CDO_CONSTRAINT,                     "Constraint definition");
  DisplayBitfield(tree,cdom,m_createDomain,SQL_CDO_CONSTRAINT_NAME_DEFINITION,     "Constraint name definition");
  DisplayBitfield(tree,cdom,m_createDomain,SQL_CDO_COLLATION,                      "Collation definition");
  DisplayBitfield(tree,cdom,m_createDomain,SQL_CDO_CONSTRAINT_INITIALLY_DEFERRED,  "Constraint are initially deferred");
  DisplayBitfield(tree,cdom,m_createDomain,SQL_CDO_CONSTRAINT_INITIALLY_IMMEDIATE, "Constraint are initially immediate");
  DisplayBitfield(tree,cdom,m_createDomain,SQL_CDO_CONSTRAINT_NON_DEFERRABLE,      "Constraint are not deferrable");

  sitem = "Create table";
  HTREEITEM crea = tree->InsertItem(sitem,infoItemSQLCF);
  DisplayBitfield(tree,crea,m_createTable,SQL_CT_CREATE_TABLE,                  "Create table");
  DisplayBitfield(tree,crea,m_createTable,SQL_CT_TABLE_CONSTRAINT,              "Create table constraint");
  DisplayBitfield(tree,crea,m_createTable,SQL_CT_CONSTRAINT_NAME_DEFINITION,    "Constraint name definition");
  DisplayBitfield(tree,crea,m_createTable,SQL_CT_COMMIT_PRESERVE,               "Temporary tables: Deleted rows preserved on commit");
  DisplayBitfield(tree,crea,m_createTable,SQL_CT_COMMIT_DELETE,                 "Temporary tables: Deleted rows are deleted on commit");
  DisplayBitfield(tree,crea,m_createTable,SQL_CT_GLOBAL_TEMPORARY,              "Global temporary tables can be created");
  DisplayBitfield(tree,crea,m_createTable,SQL_CT_LOCAL_TEMPORARY,               "Local temporary tables can be created");
  DisplayBitfield(tree,crea,m_createTable,SQL_CT_COLUMN_CONSTRAINT,             "Column constraint definition");
  DisplayBitfield(tree,crea,m_createTable,SQL_CT_COLUMN_DEFAULT,                "Column default definition");
  DisplayBitfield(tree,crea,m_createTable,SQL_CT_COLUMN_COLLATION,              "Column collation definition");
  DisplayBitfield(tree,crea,m_createTable,SQL_CT_CONSTRAINT_INITIALLY_DEFERRED, "Constraints are initially deferred");
  DisplayBitfield(tree,crea,m_createTable,SQL_CT_CONSTRAINT_INITIALLY_IMMEDIATE,"Constraints are initially immediate");
  DisplayBitfield(tree,crea,m_createTable,SQL_CT_CONSTRAINT_DEFERRABLE,         "Constraints are deferrable");
  DisplayBitfield(tree,crea,m_createTable,SQL_CT_CONSTRAINT_NON_DEFERRABLE,     "Constraints are not deferrable");

  sitem = "Create view";
  HTREEITEM crvw = tree->InsertItem(sitem,infoItemSQLCF);
  DisplayBitfield(tree,crvw,m_createView,SQL_CV_CREATE_VIEW, "Create view");
  DisplayBitfield(tree,crvw,m_createView,SQL_CV_CHECK_OPTION,"Create view with check option");
  DisplayBitfield(tree,crvw,m_createView,SQL_CV_CASCADED,    "Cascaded views");
  DisplayBitfield(tree,crvw,m_createView,SQL_CV_LOCAL,       "Local views");

  sitem = "Delete";
  HTREEITEM fkdel = tree->InsertItem(sitem,infoItemSQLCF);
  DisplayBitfield(tree,fkdel,m_deleteRules,SQL_SFKD_CASCADE,    "Set foreign keys: CASCADE");
  DisplayBitfield(tree,fkdel,m_deleteRules,SQL_SFKD_NO_ACTION,  "Set foreign keys: NO ACTION");
  DisplayBitfield(tree,fkdel,m_deleteRules,SQL_SFKD_SET_DEFAULT,"Set foreign keys: DEFAULT");
  DisplayBitfield(tree,fkdel,m_deleteRules,SQL_SFKD_SET_NULL,   "Set foreign keys: NULL");

  sitem = "Drop domain";
  HTREEITEM ddom = tree->InsertItem(sitem,infoItemSQLCF);
  DisplayBitfield(tree,ddom,m_dropDomain,SQL_DD_DROP_DOMAIN,"Drop domain");
  DisplayBitfield(tree,ddom,m_dropDomain,SQL_DD_CASCADE,    "Drop domain cascade");
  DisplayBitfield(tree,ddom,m_dropDomain,SQL_DD_RESTRICT,   "Drop domain restrict");

  sitem = "Drop schema";
  HTREEITEM dsch = tree->InsertItem(sitem,infoItemSQLCF);
  DisplayBitfield(tree,dsch,m_dropSchema,SQL_DS_DROP_SCHEMA,"Drop schema");
  DisplayBitfield(tree,dsch,m_dropSchema,SQL_DS_CASCADE,    "Drop schema cascade");
  DisplayBitfield(tree,dsch,m_dropSchema,SQL_DS_RESTRICT,   "Drop schema restrict");

  sitem = "Drop table";
  HTREEITEM dtab = tree->InsertItem(sitem,infoItemSQLCF);
  DisplayBitfield(tree,dtab,m_dropTable,SQL_DT_DROP_TABLE, "Drop table");
  DisplayBitfield(tree,dtab,m_dropTable,SQL_DT_CASCADE,    "Drop table cascade");
  DisplayBitfield(tree,dtab,m_dropTable,SQL_DT_RESTRICT,   "Drop table restrict");

  sitem = "Drop view";
  HTREEITEM dvw = tree->InsertItem(sitem,infoItemSQLCF);
  DisplayBitfield(tree,dvw,m_dropView,SQL_DV_DROP_VIEW,   "Drop view");
  DisplayBitfield(tree,dvw,m_dropView,SQL_DV_CASCADE,     "Drop view cascade");
  DisplayBitfield(tree,dvw,m_dropView,SQL_DV_RESTRICT,    "Drop view restrict");

  sitem = "Grant";
  HTREEITEM grant = tree->InsertItem(sitem,infoItemSQLCF);
  DisplayBitfield(tree,grant,m_grantOptions,SQL_SG_INSERT_TABLE,          "Table: Insert");
  DisplayBitfield(tree,grant,m_grantOptions,SQL_SG_SELECT_TABLE,          "Table: Select");
  DisplayBitfield(tree,grant,m_grantOptions,SQL_SG_UPDATE_TABLE,          "Table: Update");
  DisplayBitfield(tree,grant,m_grantOptions,SQL_SG_DELETE_TABLE,          "Table: Delete");
  DisplayBitfield(tree,grant,m_grantOptions,SQL_SG_REFERENCES_TABLE,      "Table: References");
  DisplayBitfield(tree,grant,m_grantOptions,SQL_SG_INSERT_COLUMN,         "Column: Insert");
  DisplayBitfield(tree,grant,m_grantOptions,SQL_SG_UPDATE_COLUMN,         "Column: Update");
  DisplayBitfield(tree,grant,m_grantOptions,SQL_SG_REFERENCES_COLUMN,     "Column: References");
  DisplayBitfield(tree,grant,m_grantOptions,SQL_SG_WITH_GRANT_OPTION,     "WITH GRANT OPTION");
  DisplayBitfield(tree,grant,m_grantOptions,SQL_SG_USAGE_ON_DOMAIN,       "Usage: Domain");
  DisplayBitfield(tree,grant,m_grantOptions,SQL_SG_USAGE_ON_CHARACTER_SET,"Usage: Character set");
  DisplayBitfield(tree,grant,m_grantOptions,SQL_SG_USAGE_ON_COLLATION,    "Usage: Collation");
  DisplayBitfield(tree,grant,m_grantOptions,SQL_SG_USAGE_ON_TRANSLATION,  "Usage: Translation");

  sitem = "Index";
  HTREEITEM indx = tree->InsertItem(sitem,infoItemSQLCF);
  DisplayBitfield(tree,indx,m_indexOptions1,SQL_DI_CREATE_INDEX,"Create index");
  DisplayBitfield(tree,indx,m_indexOptions1,SQL_DI_DROP_INDEX,  "Drop index");
  DisplayBitfield(tree,indx,m_indexOptions2,SQL_IK_ASC,         "ASC ascending");
  DisplayBitfield(tree,indx,m_indexOptions2,SQL_IK_DESC,        "DESC descending");
  sitem.Format("Maximum columns in an index: %s",InfoNumberToString(m_maxColIndex));
  tree->InsertItem(sitem,indx);

  sitem = "Insert";
  HTREEITEM ins = tree->InsertItem(sitem,infoItemSQLCF);
  DisplayBitfield(tree,ins,m_insertOptions,SQL_IS_INSERT_LITERALS,"VALUES");
  DisplayBitfield(tree,ins,m_insertOptions,SQL_IS_INSERT_SEARCHED,"INSERT-SELECT");
  DisplayBitfield(tree,ins,m_insertOptions,SQL_IS_SELECT_INTO,    "SELECT-INTO");

  sitem = "Revoke";
  HTREEITEM rev = tree->InsertItem(sitem,infoItemSQLCF);
  DisplayBitfield(tree,rev,m_revokeOptions,SQL_SR_INSERT_TABLE,          "Table: Insert");
  DisplayBitfield(tree,rev,m_revokeOptions,SQL_SR_SELECT_TABLE,          "Table: Select");
  DisplayBitfield(tree,rev,m_revokeOptions,SQL_SR_UPDATE_TABLE,          "Table: Update");
  DisplayBitfield(tree,rev,m_revokeOptions,SQL_SR_DELETE_TABLE,          "Table: Delete");
  DisplayBitfield(tree,rev,m_revokeOptions,SQL_SR_REFERENCES_TABLE,      "Table: References");
  DisplayBitfield(tree,rev,m_revokeOptions,SQL_SR_INSERT_COLUMN,         "Column: Insert");
  DisplayBitfield(tree,rev,m_revokeOptions,SQL_SR_UPDATE_COLUMN,         "Column: Update");
  DisplayBitfield(tree,rev,m_revokeOptions,SQL_SR_REFERENCES_COLUMN,     "Column: References");
  DisplayBitfield(tree,rev,m_revokeOptions,SQL_SR_CASCADE,               "Cascade");
  DisplayBitfield(tree,rev,m_revokeOptions,SQL_SR_RESTRICT,              "Restrict");
  DisplayBitfield(tree,rev,m_revokeOptions,SQL_SR_GRANT_OPTION_FOR,      "GRANT OPTION FOR");
  DisplayBitfield(tree,rev,m_revokeOptions,SQL_SR_USAGE_ON_DOMAIN,       "Usage: Domain");
  DisplayBitfield(tree,rev,m_revokeOptions,SQL_SR_USAGE_ON_CHARACTER_SET,"Usage: Character set");
  DisplayBitfield(tree,rev,m_revokeOptions,SQL_SR_USAGE_ON_COLLATION,    "Usage: Collation");
  DisplayBitfield(tree,rev,m_revokeOptions,SQL_SR_USAGE_ON_TRANSLATION,  "Usage: Translation");

  // The select statement
  sitem = "Select";
  HTREEITEM select = tree->InsertItem(sitem,infoItemSQLCF);

  sitem = "Selected columns";
  HTREEITEM scols = tree->InsertItem(sitem,select);

  sitem.Format("Maximum columns in a select: %s",InfoNumberToString(m_maxColSelect));
  tree->InsertItem(sitem, scols);

  sitem.Format("Column aliases in select allowed: %s",m_columnAliases? "Yes":"No");
  tree->InsertItem(sitem, scols);

  sitem = "Row value constructors";
  HTREEITEM rowval = tree->InsertItem(sitem,scols);
  DisplayBitfield(tree,rowval,m_rowConstructors,SQL_SRVC_NULL,            "NULL");
  DisplayBitfield(tree,rowval,m_rowConstructors,SQL_SRVC_DEFAULT,         "DEFAULT");
  DisplayBitfield(tree,rowval,m_rowConstructors,SQL_SRVC_ROW_SUBQUERY,    "Row subquery");
  DisplayBitfield(tree,rowval,m_rowConstructors,SQL_SRVC_VALUE_EXPRESSION,"Value expression");

  sitem = "Value expressions";
  HTREEITEM valex = tree->InsertItem(sitem,scols);
  DisplayBitfield(tree,valex,m_valExpressions,SQL_SVE_CASE,    "CASE");
  DisplayBitfield(tree,valex,m_valExpressions,SQL_SVE_CAST,    "CAST");
  DisplayBitfield(tree,valex,m_valExpressions,SQL_SVE_COALESCE,"COALESCE");
  DisplayBitfield(tree,valex,m_valExpressions,SQL_SVE_NULLIF,  "NULLIF");

  // FROM clause in the SELECT
  sitem = "FROM options";
  HTREEITEM fromopt = tree->InsertItem(sitem,select);

  sitem = "Table aliases correlation: ";
  switch(m_correlationNames)
  {
    case SQL_CN_NONE:     sitem += "Table aliases are not supported";     
                          break;
    case SQL_CN_DIFFERENT:sitem += "Aliases must differ from tablenames"; 
                          break;
    case SQL_CN_ANY:      sitem += "Any table alias can be used";         
                          break;
  }
  tree->InsertItem(sitem,fromopt);

  sitem.Format("Maximum tables in from: %s",InfoNumberToString(m_maxTabSelect));
  tree->InsertItem(sitem,fromopt);

  HTREEITEM oj = tree->InsertItem("Outer join support",fromopt);
  DisplayBitfield(tree,oj,m_oj_cap,  SQL_OJ_LEFT,                  "JOIN: Left outer joins");
  DisplayBitfield(tree,oj,m_oj_cap,  SQL_OJ_RIGHT,                 "JOIN: Right outer joins");
  DisplayBitfield(tree,oj,m_oj_cap,  SQL_OJ_FULL,                  "JOIN: Full outer joins");
  DisplayBitfield(tree,oj,m_oj_cap,  SQL_OJ_NESTED,                "JOIN: Nested outer joins");
  DisplayBitfield(tree,oj,m_oj_cap,  SQL_OJ_NOT_ORDERED,           "OPTION: Join columns not ordered by tablenames");
  DisplayBitfield(tree,oj,m_oj_cap,  SQL_OJ_INNER,                 "OPTION: Inner table can be used in inner join");
  DisplayBitfield(tree,oj,m_oj_cap,  SQL_OJ_ALL_COMPARISON_OPS,    "OPTION: Operators other than '='");
  DisplayBitfield(tree,oj,m_oj_cap92,SQL_SRJO_INNER_JOIN,          "92: INNER JOIN");
  DisplayBitfield(tree,oj,m_oj_cap92,SQL_SRJO_NATURAL_JOIN,        "92: NATURAL JOIN");
  DisplayBitfield(tree,oj,m_oj_cap92,SQL_SRJO_LEFT_OUTER_JOIN,     "92: LEFT OUTER JOIN");
  DisplayBitfield(tree,oj,m_oj_cap92,SQL_SRJO_RIGHT_OUTER_JOIN,    "92: RIGHT OUTER JOIN");
  DisplayBitfield(tree,oj,m_oj_cap92,SQL_SRJO_FULL_OUTER_JOIN,     "92: FULL OUTER JOIN");
  DisplayBitfield(tree,oj,m_oj_cap92,SQL_SRJO_CROSS_JOIN,          "92: CROSS JOIN");
  DisplayBitfield(tree,oj,m_oj_cap92,SQL_SRJO_EXCEPT_JOIN,         "92: EXCEPT JOIN");
  DisplayBitfield(tree,oj,m_oj_cap92,SQL_SRJO_INTERSECT_JOIN,      "92: INTERSECT JOIN");
  DisplayBitfield(tree,oj,m_oj_cap92,SQL_SRJO_UNION_JOIN,          "92: UNION JOIN");
  DisplayBitfield(tree,oj,m_oj_cap92,SQL_SRJO_CORRESPONDING_CLAUSE,"92: CORRESPONDING CLAUSE");

  sitem = "WHERE options";
  HTREEITEM where = tree->InsertItem(sitem,select);

  sitem = "WHERE predicates";
  HTREEITEM pred = tree->InsertItem(sitem,where);
  DisplayBitfield(tree,pred,m_predicates,SQL_SP_BETWEEN,              "BETWEEN");
  DisplayBitfield(tree,pred,m_predicates,SQL_SP_COMPARISON,           "=,<>,<,>,<=,>=");
  DisplayBitfield(tree,pred,m_predicates,SQL_SP_EXISTS,               "EXISTS");
  DisplayBitfield(tree,pred,m_predicates,SQL_SP_IN,                   "IN");
  DisplayBitfield(tree,pred,m_predicates,SQL_SP_ISNOTNULL,            "IS NOT NULL");
  DisplayBitfield(tree,pred,m_predicates,SQL_SP_ISNULL,               "IS NULL");
  DisplayBitfield(tree,pred,m_predicates,SQL_SP_LIKE,                 "LIKE");
  DisplayBitfield(tree,pred,m_predicates,SQL_SP_MATCH_FULL,           "MATCH (full)");
  DisplayBitfield(tree,pred,m_predicates,SQL_SP_MATCH_PARTIAL,        "MATCH (partial)");
  DisplayBitfield(tree,pred,m_predicates,SQL_SP_MATCH_UNIQUE_FULL,    "MATCH UNIQUE (full)");
  DisplayBitfield(tree,pred,m_predicates,SQL_SP_MATCH_UNIQUE_PARTIAL, "MATCH UNIQUE (partial)");
  DisplayBitfield(tree,pred,m_predicates,SQL_SP_OVERLAPS,             "OVERLAPS");
  DisplayBitfield(tree,pred,m_predicates,SQL_SP_QUANTIFIED_COMPARISON,"=,<>,<,>,<=,>= (quantified)");
  DisplayBitfield(tree,pred,m_predicates,SQL_SP_UNIQUE,               "UNIQUE");

  sitem = "Subqueries";
  HTREEITEM subq = tree->InsertItem(sitem,where);
  DisplayBitfield(tree,subq,m_predSubqueries,SQL_SQ_CORRELATED_SUBQUERIES,"Correlated subqueries");
  DisplayBitfield(tree,subq,m_predSubqueries,SQL_SQ_COMPARISON,           "Comparison");
  DisplayBitfield(tree,subq,m_predSubqueries,SQL_SQ_EXISTS,               "EXISTS");
  DisplayBitfield(tree,subq,m_predSubqueries,SQL_SQ_IN,                   "IN");
  DisplayBitfield(tree,subq,m_predSubqueries,SQL_SQ_QUANTIFIED,           "Quantified");

  sitem = "GROUP BY options";
  HTREEITEM gb = tree->InsertItem(sitem,select);
  if(m_groupBy & SQL_GB_NOT_SUPPORTED)
  {
    tree->InsertItem("GROUP BY not supported",gb);
  }
  else
  {
    tree->InsertItem("GROUP BY supported",gb);
    sitem.Format("Maximum columns in 'group by': %s",InfoNumberToString(m_maxColGroupBy));
    tree->InsertItem(sitem,gb);
    DisplayBitfield(tree,gb,m_groupBy,SQL_GB_GROUP_BY_EQUALS_SELECT,  "All non-aggregated columns");
    DisplayBitfield(tree,gb,m_groupBy,SQL_GB_GROUP_BY_CONTAINS_SELECT,"Non aggregated columns + extra");
    DisplayBitfield(tree,gb,m_groupBy,SQL_GB_NO_RELATION,             "Columns not related to select");
    DisplayBitfield(tree,gb,m_groupBy,SQL_GB_COLLATE,                 "COLLATE clause at end");
  }

  sitem = "ORDER BY options";
  HTREEITEM orderby = tree->InsertItem(sitem,select);
  sitem.Format("Order by in select: %s",m_orderByInSelect?"Yes":"No");
  tree->InsertItem(sitem, orderby);
  sitem.Format("Expressions in the 'order by': %s",m_exprInOrderBy?"Yes":"No");
  tree->InsertItem(sitem, orderby);
  sitem.Format("Maximum columns in 'order by': %s",InfoNumberToString(m_maxColOrderBy));
  tree->InsertItem(sitem,orderby);

  sitem = "UNION options";
  HTREEITEM un = tree->InsertItem(sitem,select);
  DisplayBitfield(tree,un,m_unions,SQL_U_UNION,    "UNION");
  DisplayBitfield(tree,un,m_unions,SQL_U_UNION_ALL,"UNION ALL");

  sitem = "Update";
  HTREEITEM update = tree->InsertItem(sitem,infoItemSQLCF);
  DisplayBitfield(tree,update,m_updateRules,SQL_SFKU_CASCADE,    "Set foreign keys: CASCADE");
  DisplayBitfield(tree,update,m_updateRules,SQL_SFKU_NO_ACTION,  "Set foreign keys: NO ACTION");
  DisplayBitfield(tree,update,m_updateRules,SQL_SFKU_SET_DEFAULT,"Set foreign keys: DEFAULT");
  DisplayBitfield(tree,update,m_updateRules,SQL_SFKU_SET_NULL,   "Set foreign keys: NULL");

  // RDBMS Implementation
  sitem = "RDBMS storage: ";
  switch(m_fileUsage)
  {
    case SQL_FILE_TABLE:         sitem += "1 file = 1 table";    break;
    case SQL_FILE_CATALOG:       sitem += "1 file = catalog";    break;
    case SQL_FILE_NOT_SUPPORTED: sitem += "Multi-tier database"; break;
    default:                     sitem += "(No information)";    break;
  }
  tree->InsertItem(sitem,infoItemImpl);

  sitem  = "RDBMS catalog support: ";
  sitem += m_supportsCatalogs ? "Yes" : "No";
  tree->InsertItem(sitem,infoItemImpl);
  tree->InsertItem("Term for a catalog: "   + m_catalogName,     infoItemImpl);
  tree->InsertItem("Term for a schema: "    + m_schemaName,      infoItemImpl);
  tree->InsertItem("Term for a table: "     + m_tableName,       infoItemImpl);
  tree->InsertItem("Term for a procedure: " + m_procedureName,   infoItemImpl);
  CString line("Catalog identifiers: ");
  switch(m_identifierCase)
  {
    case SQL_IC_UPPER:     line += "Upper-case";    break;
    case SQL_IC_LOWER:     line += "Lower-case";    break;
    case SQL_IC_SENSITIVE: line += "Sensitive (?)"; break;
    case SQL_IC_MIXED:     line += "Mixed-case";    break;
    default:               line += "No information";break;
  }
  tree->InsertItem(line,infoItemImpl);
  sitem = "Catalog name location: ";
  sitem += (m_catalogLocation == SQL_CL_START) ? "Before schema" : 
           (m_catalogLocation == SQL_CL_END)   ? "After table"   : 
           "(Unknown)";
  tree->InsertItem(sitem,infoItemImpl);
  tree->InsertItem("Catalog name separator: " + m_catalogNameSeparator,infoItemImpl);
  HTREEITEM cu = tree->InsertItem("Catalog name usage",infoItemImpl);
  DisplayBitfield(tree,cu,m_catalogUsage,SQL_CU_DML_STATEMENTS,      "DML Statements");
  DisplayBitfield(tree,cu,m_catalogUsage,SQL_CU_PROCEDURE_INVOCATION,"Procedure invocation");
  DisplayBitfield(tree,cu,m_catalogUsage,SQL_CU_TABLE_DEFINITION,    "Table definition");
  DisplayBitfield(tree,cu,m_catalogUsage,SQL_CU_INDEX_DEFINITION,    "Index definition");
  DisplayBitfield(tree,cu,m_catalogUsage,SQL_CU_PRIVILEGE_DEFINITION,"Privilege definition");

  sitem.Format("Max. length of catalogname: %i",m_maxCatalogName);
  tree->InsertItem(sitem, infoItemImpl);
  sitem.Format("Max. length of schemaname: %i",m_maxSchemaName);
  tree->InsertItem(sitem, infoItemImpl);
  sitem.Format("Max. length of tablename: %i",m_maxTableNaam);
  tree->InsertItem(sitem, infoItemImpl);
  sitem.Format("Max. length of columnname: %i",m_maxColumNaam);
  tree->InsertItem(sitem, infoItemImpl);
  sitem.Format("Max. number of columns in a table: %i", m_maxColTable);
  tree->InsertItem(sitem, infoItemImpl);

  tree->InsertItem("Special characters: " + m_specialChars,    infoItemImpl);
  tree->InsertItem("Like escape char: "   + m_likeEscape,      infoItemImpl);
  tree->InsertItem("Literal quote: "      + m_identifierQuote, infoItemImpl);

  sitem = "Collation sequence: ";
  sitem += m_collationSequence;
  tree->InsertItem(sitem,infoItemImpl);

  sitem = "NULL Collation: ";
  switch(m_nullCollation)
  {
    case SQL_NC_HIGH:  sitem += "High end of resultset, depending on ASC/DESC";         break;
    case SQL_NC_LOW:   sitem += "Low end of resultset, depending on ASC/DESC";          break;
    case SQL_NC_START: sitem += "At the start of the resultset, regardless of ASC/DESC";break;
    case SQL_NC_END:   sitem += "At the end of the resultset, regardless of ASC/DESC";  break;
    default:           sitem += "(No information)";                                     break;
  }
  tree->InsertItem(sitem,infoItemImpl);

  sitem = "Concat NULL behaviour: ";
  switch(m_concatBehaviour)
  {
    case SQL_CB_NULL:     sitem += "Results in NULL";     break;
    case SQL_CB_NON_NULL: sitem += "Results in NON-NULL"; break;
    default:              sitem += "(No information)";    break;
  }
  tree->InsertItem(sitem,infoItemImpl);

  sitem = "Nullable columns: ";
  switch(m_nullableColumns)
  {
    case SQL_NNC_NULL:     sitem += "All columns are nullable"; break;
    case SQL_NNC_NON_NULL: sitem += "Columns can be non-null";  break;
    default:               sitem += "(No information)";         break;
  }
  tree->InsertItem(sitem,infoItemImpl);

  HTREEITEM dtl = tree->InsertItem("Date/Time/Interval literals",infoItemImpl);
  DisplayBitfield(tree,dtl,m_datetimeLiterals,SQL_DL_SQL92_DATE                     ,"Date");
  DisplayBitfield(tree,dtl,m_datetimeLiterals,SQL_DL_SQL92_TIME                     ,"Time");
  DisplayBitfield(tree,dtl,m_datetimeLiterals,SQL_DL_SQL92_TIMESTAMP                ,"Timestamp");
  DisplayBitfield(tree,dtl,m_datetimeLiterals,SQL_DL_SQL92_INTERVAL_YEAR            ,"Interval year");
  DisplayBitfield(tree,dtl,m_datetimeLiterals,SQL_DL_SQL92_INTERVAL_MONTH           ,"Interval month");
  DisplayBitfield(tree,dtl,m_datetimeLiterals,SQL_DL_SQL92_INTERVAL_DAY             ,"Interval day");
  DisplayBitfield(tree,dtl,m_datetimeLiterals,SQL_DL_SQL92_INTERVAL_HOUR            ,"Interval hour");
  DisplayBitfield(tree,dtl,m_datetimeLiterals,SQL_DL_SQL92_INTERVAL_MINUTE          ,"Interval minute");
  DisplayBitfield(tree,dtl,m_datetimeLiterals,SQL_DL_SQL92_INTERVAL_SECOND          ,"Interval second");
  DisplayBitfield(tree,dtl,m_datetimeLiterals,SQL_DL_SQL92_INTERVAL_YEAR_TO_MONTH   ,"Interval year to month");
  DisplayBitfield(tree,dtl,m_datetimeLiterals,SQL_DL_SQL92_INTERVAL_DAY_TO_HOUR     ,"Interval day to hour");
  DisplayBitfield(tree,dtl,m_datetimeLiterals,SQL_DL_SQL92_INTERVAL_DAY_TO_MINUTE   ,"Interval day to minute");
  DisplayBitfield(tree,dtl,m_datetimeLiterals,SQL_DL_SQL92_INTERVAL_DAY_TO_SECOND   ,"Interval day to second");
  DisplayBitfield(tree,dtl,m_datetimeLiterals,SQL_DL_SQL92_INTERVAL_HOUR_TO_MINUTE  ,"Interval hour to minute");
  DisplayBitfield(tree,dtl,m_datetimeLiterals,SQL_DL_SQL92_INTERVAL_HOUR_TO_SECOND  ,"Interval hour to second");
  DisplayBitfield(tree,dtl,m_datetimeLiterals,SQL_DL_SQL92_INTERVAL_MINUTE_TO_SECOND,"Interval minute to second");

  sitem = "Active environments: ";
  if(!m_activeEnvironments) sitem += " (no limit or unknown)";
  else 
  {
    CString num;
    num.Format("%s",InfoNumberToString(m_activeEnvironments));
    sitem += num;
  }
  tree->InsertItem(sitem,infoItemImpl);

  sitem.Format("Transaction integrity: %s",m_integrity?"Yes":"No");
  tree->InsertItem(sitem, infoItemImpl);

  sitem = "Transactions behaviour: ";
  switch(m_txn_cap)
  {
    case SQL_TC_DML:        sitem += "Only DML";    break;
    case SQL_TC_DDL_COMMIT: sitem += "DDL Commits"; break;
    case SQL_TC_DDL_IGNORE: sitem += "DDL Ignores"; break;
    case SQL_TC_ALL:        sitem += "DDL and DML"; break;
    case SQL_TC_NONE:
    default:                sitem += "None";        break;
  }
  tree->InsertItem(sitem, infoItemImpl);

  sitem = "Default transaction level: ";
  switch(m_defaultTransaction)
  {
    case SQL_TXN_READ_UNCOMMITTED: sitem += "Read uncommitted"; break;
    case SQL_TXN_READ_COMMITTED:   sitem += "Read committed";   break;
    case SQL_TXN_REPEATABLE_READ:  sitem += "Repeatable read";  break;
    case SQL_TXN_SERIALIZABLE:     sitem += "Serializable";     break;
    default:                       sitem += "(No information)"; break;
  }
  tree->InsertItem(sitem, infoItemImpl);

  sitem = "Cursor commit: ";
  if (m_cursor_commit == SQL_CB_DELETE)      sitem += "Close cursor and remove statement";
  if (m_cursor_commit == SQL_CB_CLOSE)       sitem += "Close cursor";
  if (m_cursor_commit == SQL_CB_PRESERVE)    sitem += "Keep cursor";
  tree->InsertItem(sitem, infoItemImpl);

  sitem = "Cursor rollback: ";
  if (m_cursor_rollback == SQL_CB_DELETE)      sitem += "Close cursor and remove statement";
  if (m_cursor_rollback == SQL_CB_CLOSE)       sitem += "Close cursor";
  if (m_cursor_rollback == SQL_CB_PRESERVE)    sitem += "Keep cursor";
  tree->InsertItem(sitem, infoItemImpl);

  tree->InsertItem(CString("Accessible tables: ")     + (m_accesibleTables     ? "Yes" : "No"), infoItemImpl);
  tree->InsertItem(CString("Accessible procedures: ") + (m_accesibleProcedures ? "Yes" : "No"), infoItemImpl);

  // RDBMS SQL gereserveerde woorden
  HTREEITEM words = tree->InsertItem("RDBMS SQL Reserved words",infoItemImpl);
  didInfo =  false;
  m_RDBMSkeywords.sort();
  for(WordList::iterator wr = m_RDBMSkeywords.begin(); wr != m_RDBMSkeywords.end(); ++wr)
  {
    CString word = *wr;
    if(!word.IsEmpty()) 
    {
      didInfo = true;
      tree->InsertItem(word,words);
    }
  }
  if(!didInfo)
  {
    tree->InsertItem("No info",words);
  }
  // RDBMS Supports INFORMATION_SCHEMA views
  HTREEITEM schviews = tree->InsertItem("INFORMATION_SCHEMA views",infoItemImpl);
  DisplayBitfield(tree,schviews,m_schemaViews,SQL_ISV_ASSERTIONS,             "ASSERTIONS");
  DisplayBitfield(tree,schviews,m_schemaViews,SQL_ISV_CHARACTER_SETS,         "CHARACTER_SETS");
  DisplayBitfield(tree,schviews,m_schemaViews,SQL_ISV_CHECK_CONSTRAINTS,      "CHECK_CONSTRAINTS");
  DisplayBitfield(tree,schviews,m_schemaViews,SQL_ISV_COLLATIONS,             "COLLATIONS");
  DisplayBitfield(tree,schviews,m_schemaViews,SQL_ISV_COLUMN_DOMAIN_USAGE,    "COLUMN_DOMAIN_USAGE");
  DisplayBitfield(tree,schviews,m_schemaViews,SQL_ISV_COLUMN_PRIVILEGES,      "COLUMN_PRIVILEGES");
  DisplayBitfield(tree,schviews,m_schemaViews,SQL_ISV_COLUMNS,                "COLUMNS");
  DisplayBitfield(tree,schviews,m_schemaViews,SQL_ISV_CONSTRAINT_COLUMN_USAGE,"CONSTRAINT_COLUMN_USAGE");
  DisplayBitfield(tree,schviews,m_schemaViews,SQL_ISV_CONSTRAINT_TABLE_USAGE, "CONSTRAINT_TABLE_USAGE");
  DisplayBitfield(tree,schviews,m_schemaViews,SQL_ISV_DOMAIN_CONSTRAINTS,     "DOMAIN_CONSTRAINTS");
  DisplayBitfield(tree,schviews,m_schemaViews,SQL_ISV_DOMAINS,                "DOMAINS");
  DisplayBitfield(tree,schviews,m_schemaViews,SQL_ISV_KEY_COLUMN_USAGE,       "KEY_COLUMN_USAGE");
  DisplayBitfield(tree,schviews,m_schemaViews,SQL_ISV_REFERENTIAL_CONSTRAINTS,"REFERENTIAL_CONSTRAINTS");
  DisplayBitfield(tree,schviews,m_schemaViews,SQL_ISV_SCHEMATA,               "SCHEMATA");
  DisplayBitfield(tree,schviews,m_schemaViews,SQL_ISV_SQL_LANGUAGES,          "SQL_LANGUAGES");
  DisplayBitfield(tree,schviews,m_schemaViews,SQL_ISV_TABLE_CONSTRAINTS,      "TABLE_CONSTRAINTS");
  DisplayBitfield(tree,schviews,m_schemaViews,SQL_ISV_TABLE_PRIVILEGES,       "TABLE_PRIVILEGES");
  DisplayBitfield(tree,schviews,m_schemaViews,SQL_ISV_TABLES,                 "TABLES");
  DisplayBitfield(tree,schviews,m_schemaViews,SQL_ISV_TRANSLATIONS,           "TRANSLATIONS");
  DisplayBitfield(tree,schviews,m_schemaViews,SQL_ISV_USAGE_PRIVILEGES,       "USAGE_PRIVILEGES");
  DisplayBitfield(tree,schviews,m_schemaViews,SQL_ISV_VIEW_COLUMN_USAGE,      "VIEW_COLUMN_USAGE");
  DisplayBitfield(tree,schviews,m_schemaViews,SQL_ISV_VIEW_TABLE_USAGE,       "VIEW_TABLE_USAGE");
  DisplayBitfield(tree,schviews,m_schemaViews,SQL_ISV_VIEWS,                  "VIEWS");

  // VERSION NUMBERS
  tree->InsertItem("ODBC Driver manager: "   + m_manager_version,     infoItemVersion);
  tree->InsertItem("ODBC Database driver: "  + m_odbc_driver_version, infoItemVersion);
  tree->InsertItem("ODBC Standard: "         + m_odbc_version,        infoItemVersion);
  tree->InsertItem("RDBMS version: "         + m_rdbms_version,       infoItemVersion);
}

void
SQLInfoTree::WordListToTree(WordList& p_list,CTreeCtrl* p_tree,HTREEITEM p_item)
{
  HTREEITEM item = p_item;
  HTREEITEM last = NULL;
  for(WordList::iterator it = p_list.begin(); it != p_list.end(); ++it)
  {
    CString line = *it;
    if(line == "+" && last)
    {
      // één inspringen
      item = last;
      continue;
    }
    if(line == "-")
    {
      // Reset to original
      item = p_item;
    }
    else
    {
      last = p_tree->InsertItem(line,item);
    }
  }
  p_list.clear();
}

// All table info to a treecontrol
// HINT: Call in advance: CWaitCursor take_a_deep_sigh;
void
SQLInfoTree::MakeTableInfo(CTreeCtrl* tree,CString& table)
{
  CString   sitem;
  HTREEITEM item            = NULL;
  HTREEITEM infoColumns     = NULL;
  HTREEITEM infoPrimary     = NULL;
  HTREEITEM infoForeign     = NULL;
  HTREEITEM infoIndici      = NULL;
  HTREEITEM infoSpecial     = NULL;
  HTREEITEM infoReferenced  = NULL;
  HTREEITEM infoTabPrivilege = NULL;

  GetInfo();

  if(table.CompareNoCase(m_searchTableName) == 0)
  {
    return;
  }

  // This outperforms the DeleteAllItems of the treecontrol BY FAR!!!
  // Just search all items and delete them individually
  item = tree->GetRootItem();
  while(true)
  {
    HTREEITEM leave = tree->GetNextSiblingItem(item);
    if(leave)
    {
      tree->DeleteItem(leave);
    }
    else break;
  }
  tree->DeleteItem(item);
  item  = tree->GetRootItem();

  // THE TABLE ITSELF
  //Common::SetStatusText("Getting table info",true);
  //infoTable = tree->InsertItem("Tableinfo",item);
  WordList list;
  if(!MakeInfoTableTablepart(&list,NULL,table))
  {
    // Fill errors in tree
    WordListToTree(list,tree,item);
    return;
  }
  // Fill tree with wordlist
  WordListToTree(list,tree,item);

  if(m_searchTableName.IsEmpty())
  {
    if(MakeProcedureInfo(tree,table))
    {
      // Displayed as a procedure
      return;
    }
    if(m_searchTableName.IsEmpty())
    {
      tree->InsertItem("NOT FOUND: " + table,item);
      sitem  = "Searched for: "; 
      // Split name in a maximum of three parts
      unsigned char search_catalog[SQL_MAX_BUFFER];
      unsigned char search_schema [SQL_MAX_BUFFER];
      unsigned char search_table  [SQL_MAX_BUFFER];
      unsigned char search_type   [SQL_MAX_BUFFER];
      GetObjectName(table,search_catalog,search_schema,search_table,search_type);
      sitem += MakeObjectName((SQLCHAR *)search_catalog
        ,(SQLCHAR *)search_schema
        ,(SQLCHAR *)search_table
        ,(SQLCHAR *)search_type);
      tree->InsertItem(sitem,item);
    }
    return;
  }
  tree->UpdateWindow();
  // COLUMNS
  //Common::SetStatusText("Getting column info",true);
  infoColumns = tree->InsertItem("Columns",item);
  if(!MakeInfoTableColumns(&list))
  {
    // Fill errors in tree
    WordListToTree(list,tree,infoColumns);
    return;
  }
  WordListToTree(list,tree,infoColumns);

  tree->Expand(infoColumns,TVE_EXPAND);
  tree->UpdateWindow();
  tree->ScrollWindow(0,-2000);
  tree->LockWindowUpdate();
  // PRIMARY KEY
  //Common::SetStatusText("Getting primary key info",true);
  infoPrimary = tree->InsertItem("Primary key",item);
  PrimaryMap keymap;
  CString primName;
  if(!MakeInfoTablePrimary(&list,primName,keymap))
  {
    // Fill errors in tree
    WordListToTree(list,tree,infoPrimary);
    tree->UnlockWindowUpdate();
    return;
  }
  WordListToTree(list,tree,infoPrimary);

  // FOREIGN KEYS
  //Common::SetStatusText("Getting foreign keys info",true);
  infoForeign = tree->InsertItem("Foreign keys",item);
  if(!MakeInfoTableForeign(&list))
  {
    // Fill errors in tree
    WordListToTree(list,tree,infoForeign);
    tree->UnlockWindowUpdate();
    return;
  }
  WordListToTree(list,tree,infoForeign);

  // STATISTICS INDICI/TABLE
  //Common::SetStatusText("Getting statistics info",true);
  infoIndici = tree->InsertItem("Statistics",item);
  CString keyName;
  PrimaryMap keyMap;
  if(!MakeInfoTableStatistics(&list,keyName,keyMap))
  {
    // Fill errors in tree
    WordListToTree(list,tree,infoIndici);
    tree->UnlockWindowUpdate();
    return;
  }
  WordListToTree(list,tree,infoIndici);

  // SPECIAL COLUMNS
  //Common::SetStatusText("Getting special columns info",true);
  infoSpecial = tree->InsertItem("Special columns",item);
  if(!MakeInfoTableSpecials(&list))
  {
    // fill errors in tree
    WordListToTree(list,tree,infoSpecial);
    tree->UnlockWindowUpdate();
    return;
  }
  WordListToTree(list,tree,infoSpecial);

  // REFERENCED BY
  //Common::SetStatusText("Getting referenced by info",true);
  infoReferenced = tree->InsertItem("Referenced by",item);
  if(!MakeInfoTableForeign(&list,true))
  {
    // Fill errors in tree
    WordListToTree(list,tree,infoReferenced);
    tree->UnlockWindowUpdate();
    return;
  }
  WordListToTree(list,tree,infoReferenced);

  // TABLE PRIVILEGES
  //Common::SetStatusText("Getting privileges info",true);
  infoTabPrivilege = tree->InsertItem("Privileges",item);
  if(!MakeInfoTablePrivileges(&list))
  {
    // fill errors in tree
    WordListToTree(list,tree,infoTabPrivilege);
    tree->UnlockWindowUpdate();
    return;
  }
  WordListToTree(list,tree,infoTabPrivilege);

  //Common::SetStatusText("");
  tree->UnlockWindowUpdate();
}

// All procedure info to a treecontrol
bool
SQLInfoTree::MakeProcedureInfo(CTreeCtrl* tree,CString& procedure)
{
  HTREEITEM item = tree->GetRootItem();
  HTREEITEM infoParameters;
  WordList  list;

  // THE PROCEDURE ITSELF
  //Common::SetStatusText("Getting procedure info",true);
  if(!MakeInfoProcedureProcedurepart(&list,procedure))
  {
    // It was not a procedure either
    return false;
  }
  WordListToTree(list,tree,item);
  tree->UpdateWindow();
  // COLUMNS
  //Common::SetStatusText("Getting parameter info",true);
  infoParameters = tree->InsertItem("Parameters",item);
  if(!MakeInfoProcedureParameters(&list))
  {
    // We did a procedure anyhow (without parameters)
    return true;
  }
  WordListToTree(list,tree,infoParameters);
  tree->Expand(infoParameters,TVE_EXPAND);
  return true;
}

void
SQLInfoTree::ReportAllCapabilities(CTreeCtrl* p_tree,CString& filename)
{
  GetInfo();

  FILE* file = NULL;
  fopen_s(&file,filename,"w");
  if(!file)
  {
    AfxMessageBox("Cannot open file for ODBC report: " + filename);
    return;
  }
  fprintf(file,"ODBC Driver report\n");
  fprintf(file,"==================\n\n");

  HTREEITEM root  = p_tree->GetRootItem();
  HTREEITEM first = p_tree->GetNextItem(root,TVGN_CHILD);
  ReportCapabilities(p_tree,first,0,file);

  // END OF REPORT
  fprintf(file,"\n*** End of report ***\n");
  fclose(file);
}

//////////////////////////////////////////////////////////////////////////
//
// PRIVATE / HELPERS
//
//////////////////////////////////////////////////////////////////////////

// Helper for displaying SQLGetInfo information
HTREEITEM
SQLInfoTree::DisplayBitfield(CTreeCtrl*  tree
                            ,HTREEITEM   item
                            ,SQLUINTEGER var
                            ,SQLUINTEGER macro
                            ,char*       text)
{
  if(var & macro)
  {
    return tree->InsertItem(text,item);
  }
  else
  {
    return tree->InsertItem(CString(text) + " (not supported)",item);
  }
}

CString
SQLInfoTree::InfoNumberToString(SQLINTEGER number)
{
  CString num;
  if(num == -1)
  {
    num = "(no info from driver)";
  }
  else
  {
    num.Format("%d",number);
  }
  return num;
}

// Display a datatype CONVERT
void
SQLInfoTree::DisplayConversion(CTreeCtrl* tree 
                              ,HTREEITEM  item
                              ,CString    type
                              ,SQLINTEGER totype)
{
  if(!totype)
  {
    // Data type not supported or no conversions defined
    return;
  }
  HTREEITEM datatype = tree->InsertItem(type,item);
  if(totype & SQL_CVT_BIGINT)              tree->InsertItem("BIGINT", datatype);
  if(totype & SQL_CVT_BINARY)              tree->InsertItem("BINARY", datatype);
  if(totype & SQL_CVT_BIT)                 tree->InsertItem("BIT",    datatype);
  if(totype & SQL_CVT_CHAR)                tree->InsertItem("CHAR",   datatype);
  if(totype & SQL_CVT_DATE)                tree->InsertItem("DATE",   datatype);
  if(totype & SQL_CVT_DECIMAL)             tree->InsertItem("DECIMAL",datatype);
  if(totype & SQL_CVT_DOUBLE)              tree->InsertItem("DOUBLE", datatype);
  if(totype & SQL_CVT_FLOAT)               tree->InsertItem("FLOAT",  datatype);
  if(totype & SQL_CVT_INTEGER)             tree->InsertItem("INTEGER",datatype);
  if(totype & SQL_CVT_INTERVAL_YEAR_MONTH) tree->InsertItem("INTERVAL YEAR-MONTH",datatype);
  if(totype & SQL_CVT_INTERVAL_DAY_TIME)   tree->InsertItem("INTERVAL DAY-TIME",  datatype);
  if(totype & SQL_CVT_LONGVARBINARY)       tree->InsertItem("LONGVARBINARY",      datatype);
  if(totype & SQL_CVT_LONGVARCHAR)         tree->InsertItem("LONGVARCHAR",        datatype);
  if(totype & SQL_CVT_NUMERIC)             tree->InsertItem("NUMERIC",  datatype);
  if(totype & SQL_CVT_REAL)                tree->InsertItem("REAL",     datatype);
  if(totype & SQL_CVT_SMALLINT)            tree->InsertItem("SMALLINT", datatype);
  if(totype & SQL_CVT_TIME)                tree->InsertItem("TIME",     datatype);
  if(totype & SQL_CVT_TIMESTAMP)           tree->InsertItem("TIMESTAMP",datatype);
  if(totype & SQL_CVT_TINYINT)             tree->InsertItem("TINYINT",  datatype);
  if(totype & SQL_CVT_VARBINARY)           tree->InsertItem("VARBINARY",datatype);
  if(totype & SQL_CVT_VARCHAR)             tree->InsertItem("VARCHAR",  datatype);
}

void
SQLInfoTree::MakeTreeODBCExtensions(CTreeCtrl* tree,HTREEITEM item)
{
  CString item_any   = "SQLGetData can only be called for unbound columns";
  CString item_order = "SQLGetData can only be called in ascending order";
  CString item_block = "SQLGetData cannot be called for unbound columns in block cursors";
  CString item_bound = "SQLGetData can only be called for unbound columns";

  if(m_getdata_extensions & SQL_GD_ANY_COLUMN)
  {
    item_any = "SQLGetData can be called for any bound/unbound column";
  }
  if(m_getdata_extensions & SQL_GD_ANY_ORDER)
  {
    item_order = "SQLGetData can be called on columns in ANY order";
  }
  if(m_getdata_extensions & SQL_GD_BLOCK)
  {
    item_block = "SQLGetData can be called on unbound columns in block cursors";
  }
  if(m_getdata_extensions & SQL_GD_BOUND)
  {
    item_bound = "SQLGetData can be called for bound columns";
  }
  tree->InsertItem(item_any,  item);
  tree->InsertItem(item_order,item);
  tree->InsertItem(item_block,item);
  tree->InsertItem(item_bound,item);
}

// All ODBC Functions to the tree
void
SQLInfoTree::MakeTreeInfoFunctions(CTreeCtrl* tree,HTREEITEM item)
{
  CString sitem;

  // ODBC Supported functions
  if(m_functions_use_3)
  {
    tree->InsertItem("Driver reports ODBC 3.x Functions",item);

    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLALLOCCONNECT) ? "X" : "O") + " SQLAllocConnect";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLALLOCENV)     ? "X" : "O") + " SQLAllocEnv";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLALLOCHANDLE)  ? "X" : "O") + " SQLAllocHandle";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLALLOCHANDLESTD)?"X" : "O") + " SQLAllocHandleStd";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLALLOCSTMT)    ? "X" : "O") + " SQLAllocStmt";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLBINDCOL)      ? "X" : "O") + " SQLBindCol";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLBINDPARAM)    ? "X" : "O") + " SQLBindParam";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLBINDPARAMETER)? "X" : "O") + " SQLBindParameter";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLBROWSECONNECT)? "X" : "O") + " SQLBrowseConnect";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLBULKOPERATIONS)?"X" : "O") + " SQLBulkOperations";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLCANCEL)       ? "X" : "O") + " SQLCancel";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLCLOSECURSOR)  ? "X" : "O") + " SQLCloseCursor";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLCOLATTRIBUTE) ? "X" : "O") + " SQLColAttribute";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLCOLATTRIBUTES)? "X" : "O") + " SQLColAttributes";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLCOLUMNS)      ? "X" : "O") + " SQLColumns";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLCOLUMNPRIVILEGES)?"X":"O") + " SQLColumnPrivileges";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLCONNECT)      ? "X" : "O") + " SQLConnect";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLCOPYDESC)     ? "X" : "O") + " SQLCopyDesc";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLDATASOURCES)  ? "X" : "O") + " SQLDataSources";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLDESCRIBECOL)  ? "X" : "O") + " SQLDescribeCol";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLDESCRIBEPARAM)? "X" : "O") + " SQLDescribeParam";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLDISCONNECT)   ? "X" : "O") + " SQLDisconnect";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLDRIVERCONNECT)? "X" : "O") + " SQLDriverConnect";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLDRIVERS)      ? "X" : "O") + " SQLDrivers";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLENDTRAN)      ? "X" : "O") + " SQLEndTran";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLERROR)        ? "X" : "O") + " SQLError";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLEXECDIRECT)   ? "X" : "O") + " SQLExecDirect";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLEXECUTE)      ? "X" : "O") + " SQLExecute";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLEXTENDEDFETCH)? "X" : "O") + " SQLExetendedFetch";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLFETCH)        ? "X" : "O") + " SQLFetch";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLFETCHSCROLL)  ? "X" : "O") + " SQLFetchScroll";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLFOREIGNKEYS)  ? "X" : "O") + " SQLForeignKeys";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLFREECONNECT)  ? "X" : "O") + " SQLFreeConnect";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLFREEENV)      ? "X" : "O") + " SQLFreeEnv";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLFREEHANDLE)   ? "X" : "O") + " SQLFreeHandle";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLFREESTMT)     ? "X" : "O") + " SQLFreeStmt";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLGETCONNECTATTR)   ? "X" : "O") + " SQLGetConnectAttr";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLGETCONNECTOPTION) ? "X" : "O") + " SQLGetConnectOption";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLGETCURSORNAME)    ? "X" : "O") + " SQLGetCursorName";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLGETDATA)          ? "X" : "O") + " SQLGetData";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLGETDESCFIELD)     ? "X" : "O") + " SQLGetDescField";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLGETDESCREC)       ? "X" : "O") + " SQLGetDescRec";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLGETDIAGFIELD)     ? "X" : "O") + " SQLGetDiagField";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLGETDIAGREC)       ? "X" : "O") + " SQLGetDiagRec";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLGETENVATTR)       ? "X" : "O") + " SQLGetEnvAttr";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLGETFUNCTIONS)     ? "X" : "O") + " SQLGetFunctions";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLGETINFO)          ? "X" : "O") + " SQLGetInfo";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLGETSTMTATTR)      ? "X" : "O") + " SQLGetStmtAttr";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLGETSTMTOPTION)    ? "X" : "O") + " SQLGetStmtOption";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLGETTYPEINFO)      ? "X" : "O") + " SQLGetTypeInfo";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLMORERESULTS)      ? "X" : "O") + " SQLMoreResults";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLNATIVESQL)        ? "X" : "O") + " SQLNativeSQL";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLNUMPARAMS)        ? "X" : "O") + " SQLNumParams";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLNUMRESULTCOLS)    ? "X" : "O") + " SQLNumResultCols";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLPARAMDATA)        ? "X" : "O") + " SQLParamData";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLPARAMOPTIONS)     ? "X" : "O") + " SQLParamOptions";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLPREPARE)          ? "X" : "O") + " SQLPrepare";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLPRIMARYKEYS)      ? "X" : "O") + " SQLPrimaryKeys";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLPROCEDURECOLUMNS) ? "X" : "O") + " SQLProcedureColumns";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLPROCEDURES)       ? "X" : "O") + " SQLProcedures";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLPUTDATA)          ? "X" : "O") + " SQLPutData";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLROWCOUNT)         ? "X" : "O") + " SQLRowCount";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLSETSCROLLOPTIONS) ? "X" : "O") + " SQLSetScrollOptions";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLSETCONNECTATTR)   ? "X" : "O") + " SQLSetConnectAttr";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLSETCONNECTOPTION) ? "X" : "O") + " SQLSetConnectOption";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLSETCURSORNAME)    ? "X" : "O") + " SQLSetCursorName";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLSETDESCFIELD)     ? "X" : "O") + " SQLSetDescField";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLSETDESCREC)       ? "X" : "O") + " SQLSetDescRec";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLSETENVATTR)       ? "X" : "O") + " SQLSetEnvAttr";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLSETPARAM)         ? "X" : "O") + " SQLSetParam";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLSETPOS)           ? "X" : "O") + " SQLSetPos";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLSETSTMTATTR)      ? "X" : "O") + " SQLSetStmtAttr";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLSETSTMTOPTION)    ? "X" : "O") + " SQLSetStmtOption";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLSPECIALCOLUMNS)   ? "X" : "O") + " SQLSpecialColumns";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLSTATISTICS)       ? "X" : "O") + " SQLStatistics";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLTABLES)           ? "X" : "O") + " SQLTables";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLTABLEPRIVILEGES)  ? "X" : "O") + " SQLTablePrivileges";
    tree->InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(m_ODBCFunctions_3,SQL_API_SQLTRANSACT)         ? "X" : "O") + " SQLTransact";
    tree->InsertItem(sitem,item);
  }
  else
  {
    tree->InsertItem("Driver reports ODBC 1.x/2.x Functions",item);

    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLALLOCCONNECT] ? "X" : "O") + "SQLAllocConnect";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLALLOCENV]     ? "X" : "O") + "SQLAllocEnv";
    tree->InsertItem(sitem,item);
    sitem = "# SQLAllocHandle";
    tree->InsertItem(sitem,item);
    sitem = "# SQLAllocHandleStd";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLALLOCSTMT]    ? "X" : "O") + "SQLAllocStmt";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLBINDCOL]      ? "X" : "O") + "SQLBindCol";
    tree->InsertItem(sitem,item);
    sitem = "# SQLBindParam";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLBINDPARAMETER]? "X" : "O") + "SQLBindParameter";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLBROWSECONNECT]? "X" : "O") + "SQLBrowseConnect";
    tree->InsertItem(sitem,item);
    sitem = "# SQLBulkOperations";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLCANCEL]       ? "X" : "O") + "SQLCancel";
    tree->InsertItem(sitem,item);
    sitem = "# SQLCloseCursor";
    tree->InsertItem(sitem,item);
    sitem = "# SQLColAttribute";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLCOLATTRIBUTES]? "X" : "O") + "SQLColAttributes";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLCOLUMNS]      ? "X" : "O") + "SQLColumns";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLCOLUMNPRIVILEGES]?"X":"O") + "SQLColumnPrivileges";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLCONNECT]      ? "X" : "O") + "SQLConnect";
    tree->InsertItem(sitem,item);
    sitem = "# SQLCopyDesc";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLDATASOURCES]  ? "X" : "O") + "SQLDataSources";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLDESCRIBECOL]  ? "X" : "O") + "SQLDescribeCol";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLDESCRIBEPARAM]? "X" : "O") + "SQLDescribeParam";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLDISCONNECT]   ? "X" : "O") + "SQLDisconnect";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLDRIVERCONNECT]? "X" : "O") + "SQLDriverConnect";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLDRIVERS]      ? "X" : "O") + "SQLDrivers";
    tree->InsertItem(sitem,item);
    sitem = "# SQLEndTran";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLERROR]        ? "X" : "O") + "SQLError";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLEXECDIRECT]   ? "X" : "O") + "SQLExecDirect";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLEXECUTE]      ? "X" : "O") + "SQLExecute";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLEXTENDEDFETCH]? "X" : "O") + "SQLExtendedFetch";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLFETCH]        ? "X" : "O") + "SQLFetch";
    tree->InsertItem(sitem,item);
    sitem = "# SQLFetchScroll";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLFOREIGNKEYS]  ? "X" : "O") + "SQLForeignKeys";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLFREECONNECT]  ? "X" : "O") + "SQLFreeConnect";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLFREEENV]      ? "X" : "O") + "SQLFreeEnv";
    tree->InsertItem(sitem,item);
    sitem = "# SQLFreeHandle";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLFREESTMT]      ? "X" : "O") + "SQLFreeStmt";
    tree->InsertItem(sitem,item);
    sitem = "# SQLGetConnectAttr";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLGETCONNECTOPTION]  ? "X" : "O") + "SQLGetConnectOption";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLGETCURSORNAME]     ? "X" : "O") + "SQLGetCursorName";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLGETDATA]           ? "X" : "O") + "SQLGetData";
    tree->InsertItem(sitem,item);
    sitem = "# SQLGetDescField";
    tree->InsertItem(sitem,item);
    sitem = "# SQLGetDescRec";
    tree->InsertItem(sitem,item);
    sitem = "# SQLGetDiagField";
    tree->InsertItem(sitem,item);
    sitem = "# SQLGetDiagRec";
    tree->InsertItem(sitem,item);
    sitem = "# SQLGetEnvAttr";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLGETFUNCTIONS]      ? "X" : "O") + "SQLGetFunctions";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLGETINFO]           ? "X" : "O") + "SQLGetInfo";
    tree->InsertItem(sitem,item);
    sitem = "# SQLGetStmtAttr";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLGETSTMTOPTION]     ? "X" : "O") + "SQLGetStmtOption";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLGETTYPEINFO]       ? "X" : "O") + "SQLGetTypeInfo";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLMORERESULTS]       ? "X" : "O") + "SQLMoreResults";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLNATIVESQL]         ? "X" : "O") + "SQLNativeSQL";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLNUMPARAMS]         ? "X" : "O") + "SQLNumParams";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLNUMRESULTCOLS]     ? "X" : "O") + "SQLNumResultCols";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLPARAMDATA]         ? "X" : "O") + "SQLParamData";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLPARAMOPTIONS]      ? "X" : "O") + "SQLParamOptions";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLPREPARE]           ? "X" : "O") + "SQLPrepare";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLPRIMARYKEYS]       ? "X" : "O") + "SQLPrimaryKeys";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLPROCEDURECOLUMNS]  ? "X" : "O") + "SQLProcedureColumns";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLPROCEDURES]        ? "X" : "O") + "SQLProcedures";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLPUTDATA]           ? "X" : "O") + "SQLPutData";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLROWCOUNT]          ? "X" : "O") + "SQLRowCount";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLSETSCROLLOPTIONS]  ? "X" : "O") + "SQLSetScrollOptions";
    tree->InsertItem(sitem,item);
    sitem = "# SQLSetConnectAttr";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLSETCONNECTOPTION]  ? "X" : "O") + "SQLSetConnectOption";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLSETCURSORNAME]     ? "X" : "O") + "SQLSetCursorName";
    tree->InsertItem(sitem,item);
    sitem = "# SQLSetDescField";
    tree->InsertItem(sitem,item);
    sitem = "# SQLSetDescRec";
    tree->InsertItem(sitem,item);
    sitem = "# SQLSetEnvAttr";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLSETPARAM]          ? "X" : "O") + "SQLSetParam";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLSETPOS]            ? "X" : "O") + "SQLSetPos";
    tree->InsertItem(sitem,item);
    sitem = "# SQLSetStmtAttr";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLGETCONNECTOPTION]  ? "X" : "O") + "SQLGetConnectOption";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLSPECIALCOLUMNS]    ? "X" : "O") + "SQLSpecialColumns";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLSTATISTICS]        ? "X" : "O") + "SQLStatistics";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLTABLES]            ? "X" : "O") + "SQLTables";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLTABLEPRIVILEGES]   ? "X" : "O") + "SQLTablePrivileges";
    tree->InsertItem(sitem,item);
    sitem = CString(m_ODBCFunctions_2[SQL_API_SQLTRANSACT]          ? "X" : "O") + "SQLTransact";
    tree->InsertItem(sitem,item);
  }
}

void
SQLInfoTree::ReportCapabilities(CTreeCtrl* p_tree
                               ,HTREEITEM  p_branch
                               ,int        p_level
                               ,FILE*      p_file)
{
  HTREEITEM branch = p_branch;
  do 
  {
    // Print level to file
    for(int ind = 0; ind < p_level; ++ind)
    {
      fprintf(p_file,"    ");
    }
    // Print our info
    fprintf(p_file,"%s\n",p_tree->GetItemText(branch).GetString());
    // Do all of our children
    HTREEITEM child = p_tree->GetNextItem(branch,TVGN_CHILD);
    if(child)
    {
      ReportCapabilities(p_tree,child,p_level + 1,p_file);
    }
    // Next branch of the tree
    branch = p_tree->GetNextItem(branch,TVGN_NEXT);
  } 
  while(branch);
}
