////////////////////////////////////////////////////////////////////////
//
// File: InfoTree.cpp
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
#include "pch.h"
#include "InfoTree.h"
#include "QueryTool.h"
#include "SQLComponents.h"
#include "SQLDatabase.h"
#include "sqlext.h"
#include "Resource.h"
#include "OEDocument.h"
#include <WinFile.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace SQLComponents;

InfoTree::InfoTree()
         :m_database(nullptr)
         ,m_info(nullptr)
{
  SetDatabase(nullptr);
}

InfoTree::InfoTree(SQLDatabase* p_database)
         :m_database(p_database)
         ,m_info(nullptr)
{
  SetDatabase(p_database);
}

InfoTree::~InfoTree()
{
}

// For database open/close
void 
InfoTree::SetDatabase(SQLDatabase* p_database)
{
  if(p_database && p_database->IsOpen())
  {
    m_database = p_database;
    m_info     = m_database->GetSQLInfoDB();
  }
  else
  {
    m_database = nullptr;
    m_info     = nullptr;
  }
}


//////////////////////////////////////////////////////////////////////////
//
// PUBLIC MAIN ROUTINES
//
//////////////////////////////////////////////////////////////////////////

// All ODBC Info to a treecontrol
void
InfoTree::MakeTreeInfo()
{

  // Do everything **anew**
  DeleteAllItems();
  SetImageList(nullptr,TVSIL_NORMAL);
  SetImageList(nullptr,TVSIL_STATE);

  // If database not open, return
  if(m_info == nullptr)
  {
    return;
  }


  // Get the primary info for the tree
  m_info->GetInfo();

  HTREEITEM item = GetRootItem();
  HTREEITEM child           = NULL;
  HTREEITEM infoItemDB      = NULL;
  HTREEITEM infoItemDriver  = NULL;
  HTREEITEM infoItemDATAT   = NULL;
  HTREEITEM infoItemConv    = NULL;
  HTREEITEM infoItemCF      = NULL;
  HTREEITEM infoItemSQLCF   = NULL;
  HTREEITEM infoItemImpl    = NULL;
  HTREEITEM infoItemVersion = NULL;
  CString   key;
  CString   sitem;

  // Emtpy the tree
  while(true)
  {
    child = GetChildItem(item);
    if(!child) break;
    DeleteItem(child);
  }
  if(!m_info->SupportedFunction(SQL_API_SQLGETINFO))
  {
    InsertItem(_T("SQLGetInfo not supported: Get a better driver!"),item);
    return;
  }
  infoItemDB      = InsertItem(_T("Database"),              item);
  infoItemDriver  = InsertItem(_T("ODBC Driver"),           item);
  infoItemDATAT   = InsertItem(_T("SQL Data types"),        item);
  infoItemCF      = InsertItem(_T("SQL Conformance"),       item);
  infoItemConv    = InsertItem(_T("SQL Type conversions"),  item);
  infoItemSQLCF   = InsertItem(_T("SQL Supported standard"),item);
  infoItemImpl    = InsertItem(_T("RDBMS Implementation"),  item);
  infoItemVersion = InsertItem(_T("Version numbers"),       item);

  // DATABASE NAME AND TYPE
  CString dbtype = _T("Database type: ")  + m_database->GetDatabaseTypeName();
  CString dbname = _T("Database name: ")  + m_database->GetDatabaseName();
  CString method = _T("Naming method: ")  + m_database->GetNamingMethod();
  CString user   = _T("Logged in user: ") + m_database->GetUserName();

  InsertItem(dbtype,infoItemDB);
  InsertItem(dbname,infoItemDB);
  InsertItem(method,infoItemDB);
  InsertItem(user,  infoItemDB);

  CString   errors;
  MMetaMap  objects;
  HTREEITEM meta     = InsertItem(_T("Meta-objects in the database"),infoItemDB);
  HTREEITEM catalogs = InsertItem(_T("Catalogs"),meta);
  m_info->MakeInfoMetaTypes(objects,errors,META_CATALOGS);
  MetaListToTree(objects,catalogs,errors);

  HTREEITEM schemas  = InsertItem(_T("Schemas"),meta);
  m_info->MakeInfoMetaTypes(objects,errors,META_SCHEMAS);
  MetaListToTree(objects,schemas,errors);

  HTREEITEM obtypes  = InsertItem(_T("Object types"),meta);
  m_info->MakeInfoMetaTypes(objects,errors,META_TABLES);
  MetaListToTree(objects,obtypes,errors);

  // ODBC DRIVER
  sitem.Format(_T("ODBC Driver DLL: %s"),m_info->GetDriverName());
  InsertItem(sitem,infoItemDriver);

  // ODBC SQL reserved words (static compile time)
  WordList& listODBC = m_info->GetODBCKeywords();
  listODBC.sort();
  HTREEITEM odbcres = InsertItem(_T("ODBC SQL reserved words"), infoItemDriver);
  for(auto& word : listODBC)
  {
    InsertItem(word,odbcres);
  }
  // Supported ODBC functions
  HTREEITEM odbcFun = InsertItem(_T("ODBC Supported functions"),infoItemDriver);
  MakeTreeInfoFunctions(odbcFun);

  // Driver implementation and extensions
  HTREEITEM odbcExt = InsertItem(_T("ODBC driver implementation"),infoItemDriver);
  MakeTreeODBCExtensions(odbcExt);

  // SQL datatypen
  bool didInfo = false;
  DataTypeMap& dataTypes = m_info->GetDataTypeMap();
  DataTypeMap::iterator it = dataTypes.begin();
  TypeInfo* ti;
  CString fmt;
  while(it != dataTypes.end())
  {
    didInfo = true;
    ti = it->second;
    HTREEITEM type = InsertItem(ti->m_type_name,infoItemDATAT);

    sitem = _T("ODBC datatype name: ") + m_info->ODBCDataType(ti->m_data_type);
    InsertItem(sitem,type);
    sitem.Format(_T("Maximum precision: %s"),InfoNumberToString(ti->m_precision));
    InsertItem(sitem,type);
    sitem = _T("Literal data prefix: ") + ti->m_literal_prefix;
    InsertItem(sitem,type);
    sitem = _T("Literal data suffix: ") + ti->m_literal_suffix;
    InsertItem(sitem,type);
    sitem = _T("How to create parameters: ") + ti->m_create_params;
    InsertItem(sitem,type);
    sitem = _T("Datatype is nullable: ");
    switch(ti->m_nullable)
    {
      case SQL_NO_NULLS:         sitem += _T("No nulls"); break;
      case SQL_NULLABLE:         sitem += _T("Nullable"); break;
      case SQL_NULLABLE_UNKNOWN: sitem += _T("Unknown");  break;
    }
    InsertItem(sitem,type);
    sitem = CString(_T("Datatype is case sensitive: ")) + (ti->m_case_sensitive ? _T("Yes") : _T("No"));
    InsertItem(sitem,type);
    sitem = _T("Datatype is searchable: ");
    switch(ti->m_searchable)
    {
      case SQL_UNSEARCHABLE:    sitem += _T("Unsearchable");    break;
      case SQL_LIKE_ONLY:       sitem += _T("Like only");       break;
      case SQL_ALL_EXCEPT_LIKE: sitem += _T("All except like"); break;
      case SQL_SEARCHABLE:      sitem += _T("Searchable");      break;
    }
    InsertItem(sitem,type);
    sitem = CString(_T("Datatype is unsigned: ")) + (ti->m_unsigned ? _T("Yes") : _T("No"));
    InsertItem(sitem,type);
    sitem = CString(_T("Datatype is money: ")) + (ti->m_money ? _T("Yes") : _T("No"));
    InsertItem(sitem,type);
    sitem = CString(_T("Datatype is auto-increment: ")) + (ti->m_autoincrement ? _T("Yes") : _T("No"));
    InsertItem(sitem,type);
    sitem = _T("Local type name for display: ") + ti->m_local_type_name;
    InsertItem(sitem,type);
    sitem.Format(_T("Minimum scale: %s"),InfoNumberToString(ti->m_minimum_scale));
    InsertItem(sitem,type);
    sitem.Format(_T("Maximum scale: %s"),InfoNumberToString(ti->m_maximum_scale));
    InsertItem(sitem,type);
    sitem.Format(_T("Driver independent SQL datatype: %s"),m_info->ODBCDataType(ti->m_sqlDatatype));
    InsertItem(sitem,type);
    sitem.Format(_T("Subtype for temporal types: %d"),ti->m_sqlSubType);
    InsertItem(sitem,type);
    sitem.Format(_T("Decimal radix: %d"),ti->m_radix);
    InsertItem(sitem,type);
    sitem.Format(_T("Interval leading precision: %d"),ti->m_interval_precision);
    InsertItem(sitem,type);
    // Next datatype
    ++it;
  }
  if(!didInfo)
  {
    InsertItem(_T("No info"),infoItemDATAT);
  }
  // SQL CONVERSIONS
  sitem = _T("Conversion functions");
  HTREEITEM convfunc = InsertItem(sitem,infoItemConv);
  SQLUINTEGER conversionFuncs = m_info->GetConversionFunctions();
  DisplayBitfield(convfunc,conversionFuncs,SQL_FN_CVT_CAST,   _T("CAST"));
  DisplayBitfield(convfunc,conversionFuncs,SQL_FN_CVT_CONVERT,_T("CONVERT"));

  DisplayConversion(infoItemConv,_T("BIGINT"),             m_info->GetConvertBigint());
  DisplayConversion(infoItemConv,_T("BINARY"),             m_info->GetConvertBinary());
  DisplayConversion(infoItemConv,_T("BIT"),                m_info->GetConvertBit());
  DisplayConversion(infoItemConv,_T("CHAR"),               m_info->GetConvertChar());
  DisplayConversion(infoItemConv,_T("DATE"),               m_info->GetConvertDate());
  DisplayConversion(infoItemConv,_T("DECIMAL"),            m_info->GetConvertDecimal());
  DisplayConversion(infoItemConv,_T("DOUBLE"),             m_info->GetConvertDouble());
  DisplayConversion(infoItemConv,_T("FLOAT"),              m_info->GetConvertFloat());
  DisplayConversion(infoItemConv,_T("INTEGER"),            m_info->GetConvertInteger());
  DisplayConversion(infoItemConv,_T("INTERVAL YEAR-MONTH"),m_info->GetConvertIYM());
  DisplayConversion(infoItemConv,_T("INTERVAL DAY-TIME"),  m_info->GetConvertIDT());
  DisplayConversion(infoItemConv,_T("LONGVARBINARY"),      m_info->GetConvertLVarBinary());
  DisplayConversion(infoItemConv,_T("LONGVARCHAR"),        m_info->GetConvertLVarchar());
  DisplayConversion(infoItemConv,_T("NUMERIC"),            m_info->GetConvertNumeric());
  DisplayConversion(infoItemConv,_T("REAL"),               m_info->GetConvertReal());
  DisplayConversion(infoItemConv,_T("SMALLINT"),           m_info->GetConvertSmallint());
  DisplayConversion(infoItemConv,_T("TIME"),               m_info->GetConvertTime());
  DisplayConversion(infoItemConv,_T("TIMESTAMP"),          m_info->GetConvertTimestamp());
  DisplayConversion(infoItemConv,_T("TINYINT"),            m_info->GetConvertTinyint());
  DisplayConversion(infoItemConv,_T("VARBINARY"),          m_info->GetConvertVarBinary());
  DisplayConversion(infoItemConv,_T("VARCHAR"),            m_info->GetConvertVarchar());

  // SQL CONFORMANCE
  sitem = _T("SQL conformance: ");
  switch(m_info->GetSQLConformance())
  {
    case SQL_SC_SQL92_ENTRY:             sitem += _T("Entry level SQL92 compliant");              break;
    case SQL_SC_FIPS127_2_TRANSITIONAL : sitem += _T("FIPS 127-2 transitional level compliant");  break;
    case SQL_SC_SQL92_FULL :             sitem += _T("Full SQL92 compliant");                     break;
    case SQL_SC_SQL92_INTERMEDIATE :     sitem += _T("Intermediate SQL92 compliant");             break;
    default:                             sitem += _T("No information");                           break;
  }
  InsertItem(sitem, infoItemCF);

  sitem = _T("ODBC conformance: ");
  switch(m_info->GetODBCConformance())
  {
    case SQL_OIC_CORE:   sitem += _T("CORE");           break;
    case SQL_OIC_LEVEL1: sitem += _T("LEVEL 1");        break;
    case SQL_OIC_LEVEL2: sitem += _T("LEVEL 2");        break;
    default:             sitem += _T("No information"); break;
  }
  InsertItem(sitem, infoItemCF);

  sitem = _T("CLI conformance: ");
  switch(m_info->GetCLIConformance())
  {
    case SQL_SCC_XOPEN_CLI_VERSION1:   sitem += _T("X/Open CLI version 1"); break;
    case SQL_SCC_ISO92_CLI:            sitem += _T("ISO 92 CLI");           break;
    default:                           sitem += _T("No information");       break;
  }
  InsertItem(sitem, infoItemCF);
  InsertItem(CString(_T("X/Open CLI Year: ")) + m_info->GetCLIYear(), infoItemCF);

  // SQL Support
  sitem = _T("Aggregate functions");
  HTREEITEM agg = InsertItem(sitem,infoItemSQLCF);
  SQLUINTEGER aggr_functions = m_info->GetAggregationFunctions();
  if(aggr_functions & SQL_AF_ALL)      InsertItem(_T("All aggregates supported"),agg);
  else                                 InsertItem(_T("Just these aggregates are supported"),agg);
  DisplayBitfield(agg,aggr_functions,SQL_AF_AVG,     _T("AVG"));
  DisplayBitfield(agg,aggr_functions,SQL_AF_COUNT,   _T("COUNT"));
  DisplayBitfield(agg,aggr_functions,SQL_AF_DISTINCT,_T("DISTINCT"));
  DisplayBitfield(agg,aggr_functions,SQL_AF_MAX,     _T("MAX"));
  DisplayBitfield(agg,aggr_functions,SQL_AF_MIN,     _T("MIN"));
  DisplayBitfield(agg,aggr_functions,SQL_AF_SUM,     _T("SUM"));

  sitem = _T("Numeric functions");
  HTREEITEM numfunc = InsertItem(sitem,infoItemSQLCF);
  SQLUINTEGER funcNumeric = m_info->GetNumericFunctions();
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_ABS,     _T("ABS"));
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_ACOS,    _T("ACOS"));
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_ASIN,    _T("ASIN"));
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_ATAN,    _T("ATAN"));
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_ATAN2,   _T("ATAN2"));
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_CEILING, _T("CEILING"));
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_COS,     _T("COS"));
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_COT,     _T("COT"));
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_DEGREES, _T("DEGREES"));
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_EXP,     _T("EXP"));
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_FLOOR,   _T("FLOOR"));
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_LOG,     _T("LOG"));
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_LOG10,   _T("LOG10"));
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_MOD,     _T("MOD"));
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_PI,      _T("PI"));
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_POWER,   _T("POWER"));
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_RADIANS, _T("RADIANS"));
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_RAND,    _T("RAND"));
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_ROUND,   _T("ROUND"));
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_SIGN,    _T("SIGN"));
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_SIN,     _T("SIN"));
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_SQRT,    _T("SQRT"));
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_TAN,     _T("TAN"));
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_TRUNCATE,_T("TRUNCATE"));

  sitem = _T("String functions");
  HTREEITEM sfunc = InsertItem(sitem,infoItemSQLCF);
  SQLUINTEGER funcString  = m_info->GetStringFunctions();
  SQLUINTEGER funcString3 = m_info->GetStringFunctions3();
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_ASCII,        _T("ASCII"));
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_BIT_LENGTH,   _T("BIT_LENGTH"));
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_CHAR,         _T("CHAR"));
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_CHAR_LENGTH,  _T("CHAR_LENGTH"));
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_CHARACTER_LENGTH,_T("CHARACTER_LENGTH"));
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_CONCAT,       _T("CONCAT"));
  DisplayBitfield(sfunc,funcString3,SQL_SSF_CONVERT,         _T("CONVERT"));
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_DIFFERENCE,   _T("DIFFERENCE"));
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_INSERT,       _T("INSERT"));
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_LCASE,        _T("LCASE"));
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_LEFT,         _T("LEFT"));
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_LOCATE,       _T("LOCATE"));
  DisplayBitfield(sfunc,funcString3,SQL_SSF_LOWER,           _T("LOWER"));
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_LTRIM,        _T("LTRIM"));
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_OCTET_LENGTH, _T("OCTET_LENGTH"));
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_POSITION,     _T("POSITION"));
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_REPEAT,       _T("REPEAT"));
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_REPLACE,      _T("REPLACE"));
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_RIGHT,        _T("RIGHT"));
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_RTRIM,        _T("RTRIM"));
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_SOUNDEX,      _T("SOUNDEX"));
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_SPACE,        _T("SPACE"));
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_SUBSTRING,    _T("SUBSTRING"));
  DisplayBitfield(sfunc,funcString3,SQL_SSF_SUBSTRING,       _T("SQL92 SUBSTRING extensions"));
  DisplayBitfield(sfunc,funcString3,SQL_SSF_TRANSLATE,       _T("TRANSLATE"));
  DisplayBitfield(sfunc,funcString3,SQL_SSF_TRIM_BOTH,       _T("TRIM BOTH"));
  DisplayBitfield(sfunc,funcString3,SQL_SSF_TRIM_LEADING,    _T("TRIM LEADING"));
  DisplayBitfield(sfunc,funcString3,SQL_SSF_TRIM_TRAILING,   _T("TRIM TRAILING"));
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_UCASE,        _T("UCASE"));
  DisplayBitfield(sfunc,funcString3,SQL_SSF_UPPER,           _T("UPPER"));

  sitem = _T("Date/Time functions");
  HTREEITEM dtfunc = InsertItem(sitem,infoItemSQLCF);
  SQLUINTEGER funcDateTime = m_info->GetDateTimeFunctions();
  DisplayBitfield(dtfunc,funcDateTime,SQL_FN_TD_CURRENT_DATE     ,_T("CURRENT_DATE"));
  DisplayBitfield(dtfunc,funcDateTime,SQL_FN_TD_CURRENT_TIME     ,_T("CURRENT_TIME"));
  DisplayBitfield(dtfunc,funcDateTime,SQL_FN_TD_CURRENT_TIMESTAMP,_T("CURRENT_TIMESTAMP"));
  DisplayBitfield(dtfunc,funcDateTime,SQL_FN_TD_CURDATE          ,_T("CURDATE"));
  DisplayBitfield(dtfunc,funcDateTime,SQL_FN_TD_CURTIME          ,_T("CURTIME"));
  DisplayBitfield(dtfunc,funcDateTime,SQL_FN_TD_DAYNAME          ,_T("DAYNAME"));
  DisplayBitfield(dtfunc,funcDateTime,SQL_FN_TD_DAYOFMONTH       ,_T("DAYOFMONTH"));
  DisplayBitfield(dtfunc,funcDateTime,SQL_FN_TD_DAYOFWEEK        ,_T("DAYOFWEEK"));
  DisplayBitfield(dtfunc,funcDateTime,SQL_FN_TD_DAYOFYEAR        ,_T("DAYOFYEAR"));
  DisplayBitfield(dtfunc,funcDateTime,SQL_FN_TD_EXTRACT          ,_T("EXTRACT"));
  DisplayBitfield(dtfunc,funcDateTime,SQL_FN_TD_HOUR             ,_T("HOUR"));
  DisplayBitfield(dtfunc,funcDateTime,SQL_FN_TD_MINUTE           ,_T("MINUTE"));
  DisplayBitfield(dtfunc,funcDateTime,SQL_FN_TD_MONTH            ,_T("MONTH"));
  DisplayBitfield(dtfunc,funcDateTime,SQL_FN_TD_MONTHNAME        ,_T("MONTHNAME"));
  DisplayBitfield(dtfunc,funcDateTime,SQL_FN_TD_NOW              ,_T("NOW"));
  DisplayBitfield(dtfunc,funcDateTime,SQL_FN_TD_QUARTER          ,_T("QUARTER"));
  DisplayBitfield(dtfunc,funcDateTime,SQL_FN_TD_SECOND           ,_T("SECOND"));
  HTREEITEM stampadd = DisplayBitfield(dtfunc,funcDateTime,SQL_FN_TD_TIMESTAMPADD ,_T("TIMESTAMPADD"));
  HTREEITEM stampdif = DisplayBitfield(dtfunc,funcDateTime,SQL_FN_TD_TIMESTAMPDIFF,_T("TIMESTAMPDIFF"));
  DisplayBitfield(dtfunc,funcDateTime,SQL_FN_TD_WEEK             ,_T("WEEK"));
  DisplayBitfield(dtfunc,funcDateTime,SQL_FN_TD_YEAR             ,_T("YEAR"));

  SQLUINTEGER funcTimestampadd  = m_info->GetTimestampAddFunctions();
  SQLUINTEGER funcTimestampdiff = m_info->GetTimestampDiffFunctions();
  if((funcDateTime & SQL_FN_TD_TIMESTAMPADD) == 0)
  {
    funcTimestampadd = 0;
  }
  if((funcDateTime & SQL_FN_TD_TIMESTAMPDIFF) == 0)
  {
    funcTimestampdiff = 0;
  }
  // Options for timestamp ADD/DIFF
  DisplayBitfield(stampadd,funcTimestampadd,SQL_FN_TSI_FRAC_SECOND,_T("Fractions of a second"));
  DisplayBitfield(stampadd,funcTimestampadd,SQL_FN_TSI_SECOND,     _T("SECOND"));
  DisplayBitfield(stampadd,funcTimestampadd,SQL_FN_TSI_MINUTE,     _T("MINUTE"));
  DisplayBitfield(stampadd,funcTimestampadd,SQL_FN_TSI_HOUR,       _T("HOUR"));
  DisplayBitfield(stampadd,funcTimestampadd,SQL_FN_TSI_DAY,        _T("DAY"));
  DisplayBitfield(stampadd,funcTimestampadd,SQL_FN_TSI_WEEK,       _T("WEEK"));
  DisplayBitfield(stampadd,funcTimestampadd,SQL_FN_TSI_MONTH,      _T("MONTH"));
  DisplayBitfield(stampadd,funcTimestampadd,SQL_FN_TSI_QUARTER,    _T("QUARTER"));
  DisplayBitfield(stampadd,funcTimestampadd,SQL_FN_TSI_YEAR,       _T("YEAR"));

  DisplayBitfield(stampdif,funcTimestampdiff,SQL_FN_TSI_FRAC_SECOND,_T("Fractions of a second"));
  DisplayBitfield(stampdif,funcTimestampdiff,SQL_FN_TSI_SECOND,     _T("SECOND"));
  DisplayBitfield(stampdif,funcTimestampdiff,SQL_FN_TSI_MINUTE,     _T("MINUTE"));
  DisplayBitfield(stampdif,funcTimestampdiff,SQL_FN_TSI_HOUR,       _T("HOUR"));
  DisplayBitfield(stampdif,funcTimestampdiff,SQL_FN_TSI_DAY,        _T("DAY"));
  DisplayBitfield(stampdif,funcTimestampdiff,SQL_FN_TSI_WEEK,       _T("WEEK"));
  DisplayBitfield(stampdif,funcTimestampdiff,SQL_FN_TSI_MONTH,      _T("MONTH"));
  DisplayBitfield(stampdif,funcTimestampdiff,SQL_FN_TSI_QUARTER,    _T("QUARTER"));
  DisplayBitfield(stampdif,funcTimestampdiff,SQL_FN_TSI_YEAR,       _T("YEAR"));

  sitem = _T("System functions");
  HTREEITEM sysfunc = InsertItem(sitem,infoItemSQLCF);
  SQLUINTEGER funcSystem = m_info->GetSystemFunctions();
  DisplayBitfield(sysfunc,funcSystem,SQL_FN_SYS_DBNAME,  _T("DATABASE"));
  DisplayBitfield(sysfunc,funcSystem,SQL_FN_SYS_IFNULL,  _T("IFNULL"));
  DisplayBitfield(sysfunc,funcSystem,SQL_FN_SYS_USERNAME,_T("USER"));

  sitem = _T("Alter domain");
  HTREEITEM adom = InsertItem(sitem,infoItemSQLCF);
  SQLUINTEGER alterDomain = m_info->GetAlterDomainFunctions();
  DisplayBitfield(adom,alterDomain,SQL_AD_ADD_DOMAIN_CONSTRAINT     ,        _T("Add domain constraint"));
  DisplayBitfield(adom,alterDomain,SQL_AD_ADD_DOMAIN_DEFAULT        ,        _T("Add domain default"));
  DisplayBitfield(adom,alterDomain,SQL_AD_CONSTRAINT_NAME_DEFINITION,        _T("Constraint name definition"));
  DisplayBitfield(adom,alterDomain,SQL_AD_DROP_DOMAIN_CONSTRAINT    ,        _T("Drop domain constraint"));
  DisplayBitfield(adom,alterDomain,SQL_AD_DROP_DOMAIN_DEFAULT       ,        _T("Drop domain default"));
  DisplayBitfield(adom,alterDomain,SQL_AD_ADD_CONSTRAINT_DEFERRABLE ,        _T("Add constraint deferrable"));
  DisplayBitfield(adom,alterDomain,SQL_AD_ADD_CONSTRAINT_NON_DEFERRABLE     ,_T("Add constraint non deferrable"));
  DisplayBitfield(adom,alterDomain,SQL_AD_ADD_CONSTRAINT_INITIALLY_DEFERRED ,_T("Add constraint initially deferred"));
  DisplayBitfield(adom,alterDomain,SQL_AD_ADD_CONSTRAINT_INITIALLY_IMMEDIATE,_T("Add constraint initially immediate"));

  sitem = _T("Alter table");
  HTREEITEM alt = InsertItem(sitem,infoItemSQLCF);
  SQLUINTEGER alterTable = m_info->GetAlterTableFunctions();
  DisplayBitfield(alt,alterTable,SQL_AT_ADD_COLUMN,                    _T("Add column"));
  DisplayBitfield(alt,alterTable,SQL_AT_DROP_COLUMN,                   _T("Drop column"));
  DisplayBitfield(alt,alterTable,SQL_AT_ADD_CONSTRAINT,                _T("Add constraint"));
  DisplayBitfield(alt,alterTable,SQL_AT_ADD_COLUMN_COLLATION,          _T("Add column with collation"));
  DisplayBitfield(alt,alterTable,SQL_AT_ADD_COLUMN_DEFAULT,            _T("Add column with default"));
  DisplayBitfield(alt,alterTable,SQL_AT_ADD_COLUMN_SINGLE,             _T("Add column (single)"));
  DisplayBitfield(alt,alterTable,SQL_AT_ADD_CONSTRAINT,                _T("Add column constraint"));
  DisplayBitfield(alt,alterTable,SQL_AT_ADD_TABLE_CONSTRAINT,          _T("Add table constraint"));
  DisplayBitfield(alt,alterTable,SQL_AT_CONSTRAINT_NAME_DEFINITION,    _T("Add constraint name definition"));
  DisplayBitfield(alt,alterTable,SQL_AT_DROP_COLUMN_CASCADE,           _T("Drop column cascade"));
  DisplayBitfield(alt,alterTable,SQL_AT_DROP_COLUMN_DEFAULT,           _T("Drop column default"));
  DisplayBitfield(alt,alterTable,SQL_AT_DROP_COLUMN_RESTRICT,          _T("Drop column restrict"));
  DisplayBitfield(alt,alterTable,SQL_AT_DROP_TABLE_CONSTRAINT_CASCADE, _T("Drop table constraint cascade"));
  DisplayBitfield(alt,alterTable,SQL_AT_DROP_TABLE_CONSTRAINT_RESTRICT,_T("Drop table constraint restrict"));
  DisplayBitfield(alt,alterTable,SQL_AT_SET_COLUMN_DEFAULT,            _T("Set column default"));
  DisplayBitfield(alt,alterTable,SQL_AT_CONSTRAINT_INITIALLY_DEFERRED, _T("Constraints are initially deferred"));
  DisplayBitfield(alt,alterTable,SQL_AT_CONSTRAINT_INITIALLY_IMMEDIATE,_T("Constraints are initially immediate"));
  DisplayBitfield(alt,alterTable,SQL_AT_CONSTRAINT_DEFERRABLE,         _T("Constraints are deferrable"));
  DisplayBitfield(alt,alterTable,SQL_AT_CONSTRAINT_NON_DEFERRABLE,     _T("Constraints are not deferrable"));

  sitem = _T("Create schema");
  HTREEITEM crsch = InsertItem(sitem,infoItemSQLCF);
  SQLUINTEGER createSchema = m_info->GetCreateSchemaFunctions();
  DisplayBitfield(crsch,createSchema,SQL_CS_CREATE_SCHEMA,        _T("Create schema"));
  DisplayBitfield(crsch,createSchema,SQL_CS_AUTHORIZATION,        _T("Authorization clause for owner"));
  DisplayBitfield(crsch,createSchema,SQL_CS_DEFAULT_CHARACTER_SET,_T("Create default character set"));

  sitem = _T("Create domain");
  HTREEITEM cdom = InsertItem(sitem,infoItemSQLCF);
  SQLUINTEGER createDomain = m_info->GetCreateDomainFunctions();
  DisplayBitfield(cdom,createDomain,SQL_CDO_CREATE_DOMAIN,                  _T("Create domain"));
  DisplayBitfield(cdom,createDomain,SQL_CDO_DEFAULT,                        _T("Default value"));
  DisplayBitfield(cdom,createDomain,SQL_CDO_CONSTRAINT,                     _T("Constraint definition"));
  DisplayBitfield(cdom,createDomain,SQL_CDO_CONSTRAINT_NAME_DEFINITION,     _T("Constraint name definition"));
  DisplayBitfield(cdom,createDomain,SQL_CDO_COLLATION,                      _T("Collation definition"));
  DisplayBitfield(cdom,createDomain,SQL_CDO_CONSTRAINT_INITIALLY_DEFERRED,  _T("Constraint are initially deferred"));
  DisplayBitfield(cdom,createDomain,SQL_CDO_CONSTRAINT_INITIALLY_IMMEDIATE, _T("Constraint are initially immediate"));
  DisplayBitfield(cdom,createDomain,SQL_CDO_CONSTRAINT_NON_DEFERRABLE,      _T("Constraint are not deferrable"));

  sitem = _T("Create table");
  HTREEITEM crea = InsertItem(sitem,infoItemSQLCF);
  SQLUINTEGER createTable = m_info->GetCreateTableFunctions();
  DisplayBitfield(crea,createTable,SQL_CT_CREATE_TABLE,                  _T("Create table"));
  DisplayBitfield(crea,createTable,SQL_CT_TABLE_CONSTRAINT,              _T("Create table constraint"));
  DisplayBitfield(crea,createTable,SQL_CT_CONSTRAINT_NAME_DEFINITION,    _T("Constraint name definition"));
  DisplayBitfield(crea,createTable,SQL_CT_COMMIT_PRESERVE,               _T("Temporary tables: Deleted rows preserved on commit"));
  DisplayBitfield(crea,createTable,SQL_CT_COMMIT_DELETE,                 _T("Temporary tables: Deleted rows are deleted on commit"));
  DisplayBitfield(crea,createTable,SQL_CT_GLOBAL_TEMPORARY,              _T("Global temporary tables can be created"));
  DisplayBitfield(crea,createTable,SQL_CT_LOCAL_TEMPORARY,               _T("Local temporary tables can be created"));
  DisplayBitfield(crea,createTable,SQL_CT_COLUMN_CONSTRAINT,             _T("Column constraint definition"));
  DisplayBitfield(crea,createTable,SQL_CT_COLUMN_DEFAULT,                _T("Column default definition"));
  DisplayBitfield(crea,createTable,SQL_CT_COLUMN_COLLATION,              _T("Column collation definition"));
  DisplayBitfield(crea,createTable,SQL_CT_CONSTRAINT_INITIALLY_DEFERRED, _T("Constraints are initially deferred"));
  DisplayBitfield(crea,createTable,SQL_CT_CONSTRAINT_INITIALLY_IMMEDIATE,_T("Constraints are initially immediate"));
  DisplayBitfield(crea,createTable,SQL_CT_CONSTRAINT_DEFERRABLE,         _T("Constraints are deferrable"));
  DisplayBitfield(crea,createTable,SQL_CT_CONSTRAINT_NON_DEFERRABLE,     _T("Constraints are not deferrable"));

  sitem = _T("Create view");
  HTREEITEM crvw = InsertItem(sitem,infoItemSQLCF);
  SQLUINTEGER createView = m_info->GetCreateViewFunctions();
  DisplayBitfield(crvw,createView,SQL_CV_CREATE_VIEW, _T("Create view"));
  DisplayBitfield(crvw,createView,SQL_CV_CHECK_OPTION,_T("Create view with check option"));
  DisplayBitfield(crvw,createView,SQL_CV_CASCADED,    _T("Cascaded views"));
  DisplayBitfield(crvw,createView,SQL_CV_LOCAL,       _T("Local views"));

  sitem = _T("Delete");
  HTREEITEM fkdel = InsertItem(sitem,infoItemSQLCF);
  SQLUINTEGER deleteRules = m_info->GetDeleteRuleFunctions();
  DisplayBitfield(fkdel,deleteRules,SQL_SFKD_CASCADE,    _T("Set foreign keys: CASCADE"));
  DisplayBitfield(fkdel,deleteRules,SQL_SFKD_NO_ACTION,  _T("Set foreign keys: NO ACTION"));
  DisplayBitfield(fkdel,deleteRules,SQL_SFKD_SET_DEFAULT,_T("Set foreign keys: DEFAULT"));
  DisplayBitfield(fkdel,deleteRules,SQL_SFKD_SET_NULL,   _T("Set foreign keys: NULL"));

  sitem = _T("Drop domain");
  HTREEITEM ddom = InsertItem(sitem,infoItemSQLCF);
  SQLUINTEGER dropDomain = m_info->GetDropDomainFunctions();
  DisplayBitfield(ddom,dropDomain,SQL_DD_DROP_DOMAIN,_T("Drop domain"));
  DisplayBitfield(ddom,dropDomain,SQL_DD_CASCADE,    _T("Drop domain cascade"));
  DisplayBitfield(ddom,dropDomain,SQL_DD_RESTRICT,   _T("Drop domain restrict"));

  sitem = _T("Drop schema");
  HTREEITEM dsch = InsertItem(sitem,infoItemSQLCF);
  SQLUINTEGER dropSchema = m_info->GetDropSchemaFunctions();
  DisplayBitfield(dsch,dropSchema,SQL_DS_DROP_SCHEMA,_T("Drop schema"));
  DisplayBitfield(dsch,dropSchema,SQL_DS_CASCADE,    _T("Drop schema cascade"));
  DisplayBitfield(dsch,dropSchema,SQL_DS_RESTRICT,   _T("Drop schema restrict"));

  sitem = _T("Drop table");
  HTREEITEM dtab = InsertItem(sitem,infoItemSQLCF);
  SQLUINTEGER dropTable = m_info->GetDropTableFunctions();
  DisplayBitfield(dtab,dropTable,SQL_DT_DROP_TABLE, _T("Drop table"));
  DisplayBitfield(dtab,dropTable,SQL_DT_CASCADE,    _T("Drop table cascade"));
  DisplayBitfield(dtab,dropTable,SQL_DT_RESTRICT,   _T("Drop table restrict"));

  sitem = _T("Drop view");
  HTREEITEM dvw = InsertItem(sitem,infoItemSQLCF);
  SQLUINTEGER dropView = m_info->GetDropViewFunctions();
  DisplayBitfield(dvw,dropView,SQL_DV_DROP_VIEW,   _T("Drop view"));
  DisplayBitfield(dvw,dropView,SQL_DV_CASCADE,     _T("Drop view cascade"));
  DisplayBitfield(dvw,dropView,SQL_DV_RESTRICT,    _T("Drop view restrict"));

  sitem = _T("Grant");
  HTREEITEM grant = InsertItem(sitem,infoItemSQLCF);
  SQLUINTEGER grantOptions = m_info->GetGrantOptions();
  DisplayBitfield(grant,grantOptions,SQL_SG_INSERT_TABLE,          _T("Table: Insert"));
  DisplayBitfield(grant,grantOptions,SQL_SG_SELECT_TABLE,          _T("Table: Select"));
  DisplayBitfield(grant,grantOptions,SQL_SG_UPDATE_TABLE,          _T("Table: Update"));
  DisplayBitfield(grant,grantOptions,SQL_SG_DELETE_TABLE,          _T("Table: Delete"));
  DisplayBitfield(grant,grantOptions,SQL_SG_REFERENCES_TABLE,      _T("Table: References"));
  DisplayBitfield(grant,grantOptions,SQL_SG_INSERT_COLUMN,         _T("Column: Insert"));
  DisplayBitfield(grant,grantOptions,SQL_SG_UPDATE_COLUMN,         _T("Column: Update"));
  DisplayBitfield(grant,grantOptions,SQL_SG_REFERENCES_COLUMN,     _T("Column: References"));
  DisplayBitfield(grant,grantOptions,SQL_SG_WITH_GRANT_OPTION,     _T("WITH GRANT OPTION"));
  DisplayBitfield(grant,grantOptions,SQL_SG_USAGE_ON_DOMAIN,       _T("Usage: Domain"));
  DisplayBitfield(grant,grantOptions,SQL_SG_USAGE_ON_CHARACTER_SET,_T("Usage: Character set"));
  DisplayBitfield(grant,grantOptions,SQL_SG_USAGE_ON_COLLATION,    _T("Usage: Collation"));
  DisplayBitfield(grant,grantOptions,SQL_SG_USAGE_ON_TRANSLATION,  _T("Usage: Translation"));

  sitem = _T("Index");
  HTREEITEM indx = InsertItem(sitem,infoItemSQLCF);
  SQLUINTEGER indexOptions1 = m_info->GetIndexOptions1();
  SQLUINTEGER indexOptions2 = m_info->GetIndexOptions2();
  SQLSMALLINT maxColIndex   = m_info->GetMaxColumnsInIndex();
  DisplayBitfield(indx,indexOptions1,SQL_DI_CREATE_INDEX,_T("Create index"));
  DisplayBitfield(indx,indexOptions1,SQL_DI_DROP_INDEX,  _T("Drop index"));
  DisplayBitfield(indx,indexOptions2,SQL_IK_ASC,         _T("ASC ascending"));
  DisplayBitfield(indx,indexOptions2,SQL_IK_DESC,        _T("DESC descending"));
  sitem.Format(_T("Maximum columns in an index: %s"),InfoNumberToString(maxColIndex));
  InsertItem(sitem,indx);

  sitem = _T("Insert");
  HTREEITEM ins = InsertItem(sitem,infoItemSQLCF);
  SQLUINTEGER insertOptions = m_info->GetInsertOptions();
  DisplayBitfield(ins,insertOptions,SQL_IS_INSERT_LITERALS,_T("VALUES"));
  DisplayBitfield(ins,insertOptions,SQL_IS_INSERT_SEARCHED,_T("INSERT-SELECT"));
  DisplayBitfield(ins,insertOptions,SQL_IS_SELECT_INTO,    _T("SELECT-INTO"));

  sitem = _T("Revoke");
  HTREEITEM rev = InsertItem(sitem,infoItemSQLCF);
  SQLUINTEGER revokeOptions = m_info->GetRevokeOptions();
  DisplayBitfield(rev,revokeOptions,SQL_SR_INSERT_TABLE,          _T("Table: Insert"));
  DisplayBitfield(rev,revokeOptions,SQL_SR_SELECT_TABLE,          _T("Table: Select"));
  DisplayBitfield(rev,revokeOptions,SQL_SR_UPDATE_TABLE,          _T("Table: Update"));
  DisplayBitfield(rev,revokeOptions,SQL_SR_DELETE_TABLE,          _T("Table: Delete"));
  DisplayBitfield(rev,revokeOptions,SQL_SR_REFERENCES_TABLE,      _T("Table: References"));
  DisplayBitfield(rev,revokeOptions,SQL_SR_INSERT_COLUMN,         _T("Column: Insert"));
  DisplayBitfield(rev,revokeOptions,SQL_SR_UPDATE_COLUMN,         _T("Column: Update"));
  DisplayBitfield(rev,revokeOptions,SQL_SR_REFERENCES_COLUMN,     _T("Column: References"));
  DisplayBitfield(rev,revokeOptions,SQL_SR_CASCADE,               _T("Cascade"));
  DisplayBitfield(rev,revokeOptions,SQL_SR_RESTRICT,              _T("Restrict"));
  DisplayBitfield(rev,revokeOptions,SQL_SR_GRANT_OPTION_FOR,      _T("GRANT OPTION FOR"));
  DisplayBitfield(rev,revokeOptions,SQL_SR_USAGE_ON_DOMAIN,       _T("Usage: Domain"));
  DisplayBitfield(rev,revokeOptions,SQL_SR_USAGE_ON_CHARACTER_SET,_T("Usage: Character set"));
  DisplayBitfield(rev,revokeOptions,SQL_SR_USAGE_ON_COLLATION,    _T("Usage: Collation"));
  DisplayBitfield(rev,revokeOptions,SQL_SR_USAGE_ON_TRANSLATION,  _T("Usage: Translation"));

  // The select statement
  sitem = _T("Select");
  HTREEITEM select = InsertItem(sitem,infoItemSQLCF);

  sitem = _T("Selected columns");
  HTREEITEM scols = InsertItem(sitem,select);

  sitem.Format(_T("Maximum columns in a select: %s"),InfoNumberToString(m_info->GetMaxColumnsInSelect()));
  InsertItem(sitem, scols);

  sitem.Format(_T("Column aliases in select allowed: %s"),m_info->GetColumnAliasAllowed() ? _T("Yes"):_T("No"));
  InsertItem(sitem, scols);

  sitem = _T("Row value constructors");
  HTREEITEM rowval = InsertItem(sitem,scols);
  SQLUINTEGER rowConstructors = m_info->GetRowConstructorOptions();
  DisplayBitfield(rowval,rowConstructors,SQL_SRVC_NULL,            _T("NULL"));
  DisplayBitfield(rowval,rowConstructors,SQL_SRVC_DEFAULT,         _T("DEFAULT"));
  DisplayBitfield(rowval,rowConstructors,SQL_SRVC_ROW_SUBQUERY,    _T("Row subquery"));
  DisplayBitfield(rowval,rowConstructors,SQL_SRVC_VALUE_EXPRESSION,_T("Value expression"));

  sitem = _T("Value expressions");
  HTREEITEM valex = InsertItem(sitem,scols);
  SQLUINTEGER valExpressions = m_info->GetValueExpressionOptions();
  DisplayBitfield(valex,valExpressions,SQL_SVE_CASE,    _T("CASE"));
  DisplayBitfield(valex,valExpressions,SQL_SVE_CAST,    _T("CAST"));
  DisplayBitfield(valex,valExpressions,SQL_SVE_COALESCE,_T("COALESCE"));
  DisplayBitfield(valex,valExpressions,SQL_SVE_NULLIF,  _T("NULLIF"));

  // FROM clause in the SELECT
  sitem = _T("FROM options");
  HTREEITEM fromopt = InsertItem(sitem,select);

  sitem = _T("Table aliases correlation: ");
  switch(m_info->GetTableCorrelationOptions())
  {
    case SQL_CN_NONE:     sitem += _T("Table aliases are not supported");     
                          break;
    case SQL_CN_DIFFERENT:sitem += _T("Aliases must differ from tablenames"); 
                          break;
    case SQL_CN_ANY:      sitem += _T("Any table alias can be used");         
                          break;
  }
  InsertItem(sitem,fromopt);

  sitem.Format(_T("Maximum tables in FROM: %s"),InfoNumberToString(m_info->GetMaxTablesInSelect()));
  InsertItem(sitem,fromopt);

  HTREEITEM oj = InsertItem(_T("Outer join support"),fromopt);
  SQLUINTEGER oj_cap   = m_info->GetOuterJoinOptions();
  SQLUINTEGER oj_cap92 = m_info->GetOuterJoin1992Options();
  DisplayBitfield(oj,oj_cap,  SQL_OJ_LEFT,                  _T("JOIN: Left outer joins"));
  DisplayBitfield(oj,oj_cap,  SQL_OJ_RIGHT,                 _T("JOIN: Right outer joins"));
  DisplayBitfield(oj,oj_cap,  SQL_OJ_FULL,                  _T("JOIN: Full outer joins"));
  DisplayBitfield(oj,oj_cap,  SQL_OJ_NESTED,                _T("JOIN: Nested outer joins"));
  DisplayBitfield(oj,oj_cap,  SQL_OJ_NOT_ORDERED,           _T("OPTION: Join columns not ordered by tablenames"));
  DisplayBitfield(oj,oj_cap,  SQL_OJ_INNER,                 _T("OPTION: Inner table can be used in inner join"));
  DisplayBitfield(oj,oj_cap,  SQL_OJ_ALL_COMPARISON_OPS,    _T("OPTION: Operators other than '='"));
  DisplayBitfield(oj,oj_cap92,SQL_SRJO_INNER_JOIN,          _T("92: INNER JOIN"));
  DisplayBitfield(oj,oj_cap92,SQL_SRJO_NATURAL_JOIN,        _T("92: NATURAL JOIN"));
  DisplayBitfield(oj,oj_cap92,SQL_SRJO_LEFT_OUTER_JOIN,     _T("92: LEFT OUTER JOIN"));
  DisplayBitfield(oj,oj_cap92,SQL_SRJO_RIGHT_OUTER_JOIN,    _T("92: RIGHT OUTER JOIN"));
  DisplayBitfield(oj,oj_cap92,SQL_SRJO_FULL_OUTER_JOIN,     _T("92: FULL OUTER JOIN"));
  DisplayBitfield(oj,oj_cap92,SQL_SRJO_CROSS_JOIN,          _T("92: CROSS JOIN"));
  DisplayBitfield(oj,oj_cap92,SQL_SRJO_EXCEPT_JOIN,         _T("92: EXCEPT JOIN"));
  DisplayBitfield(oj,oj_cap92,SQL_SRJO_INTERSECT_JOIN,      _T("92: INTERSECT JOIN"));
  DisplayBitfield(oj,oj_cap92,SQL_SRJO_UNION_JOIN,          _T("92: UNION JOIN"));
  DisplayBitfield(oj,oj_cap92,SQL_SRJO_CORRESPONDING_CLAUSE,_T("92: CORRESPONDING CLAUSE"));

  sitem = _T("WHERE options");
  HTREEITEM where = InsertItem(sitem,select);

  sitem = _T("WHERE predicates");
  HTREEITEM pred = InsertItem(sitem,where);
  SQLUINTEGER predicates = m_info->GetPredicateOptions();
  DisplayBitfield(pred,predicates,SQL_SP_BETWEEN,              _T("BETWEEN"));
  DisplayBitfield(pred,predicates,SQL_SP_COMPARISON,           _T("=,<>,<,>,<=,>="));
  DisplayBitfield(pred,predicates,SQL_SP_EXISTS,               _T("EXISTS"));
  DisplayBitfield(pred,predicates,SQL_SP_IN,                   _T("IN"));
  DisplayBitfield(pred,predicates,SQL_SP_ISNOTNULL,            _T("IS NOT NULL"));
  DisplayBitfield(pred,predicates,SQL_SP_ISNULL,               _T("IS NULL"));
  DisplayBitfield(pred,predicates,SQL_SP_LIKE,                 _T("LIKE"));
  DisplayBitfield(pred,predicates,SQL_SP_MATCH_FULL,           _T("MATCH (full)"));
  DisplayBitfield(pred,predicates,SQL_SP_MATCH_PARTIAL,        _T("MATCH (partial)"));
  DisplayBitfield(pred,predicates,SQL_SP_MATCH_UNIQUE_FULL,    _T("MATCH UNIQUE (full)"));
  DisplayBitfield(pred,predicates,SQL_SP_MATCH_UNIQUE_PARTIAL, _T("MATCH UNIQUE (partial)"));
  DisplayBitfield(pred,predicates,SQL_SP_OVERLAPS,             _T("OVERLAPS"));
  DisplayBitfield(pred,predicates,SQL_SP_QUANTIFIED_COMPARISON,_T("=,<>,<,>,<=,>= (quantified)"));
  DisplayBitfield(pred,predicates,SQL_SP_UNIQUE,               _T("UNIQUE"));

  sitem = _T("Subqueries");
  HTREEITEM subq = InsertItem(sitem,where);
  SQLUINTEGER predSubqueries = m_info->GetPredicateSubqueryOptions();
  DisplayBitfield(subq,predSubqueries,SQL_SQ_CORRELATED_SUBQUERIES,_T("Correlated subqueries"));
  DisplayBitfield(subq,predSubqueries,SQL_SQ_COMPARISON,           _T("Comparison"));
  DisplayBitfield(subq,predSubqueries,SQL_SQ_EXISTS,               _T("EXISTS"));
  DisplayBitfield(subq,predSubqueries,SQL_SQ_IN,                   _T("IN"));
  DisplayBitfield(subq,predSubqueries,SQL_SQ_QUANTIFIED,           _T("Quantified"));

  sitem = _T("GROUP BY options");
  HTREEITEM gb = InsertItem(sitem,select);
  SQLUINTEGER groupBy = m_info->GetGroupByOptions();
  if(groupBy & SQL_GB_NOT_SUPPORTED)
  {
    InsertItem(_T("GROUP BY not supported"),gb);
  }
  else
  {
    InsertItem(_T("GROUP BY supported"),gb);
    sitem.Format(_T("Maximum columns in 'group by': %s"),InfoNumberToString(m_info->GetMaxColumnsInGroupBy()));
    InsertItem(sitem,gb);
    DisplayBitfield(gb,groupBy,SQL_GB_GROUP_BY_EQUALS_SELECT,  _T("All non-aggregated columns"));
    DisplayBitfield(gb,groupBy,SQL_GB_GROUP_BY_CONTAINS_SELECT,_T("Non aggregated columns + extra"));
    DisplayBitfield(gb,groupBy,SQL_GB_NO_RELATION,             _T("Columns not related to select"));
    DisplayBitfield(gb,groupBy,SQL_GB_COLLATE,                 _T("COLLATE clause at end"));
  }

  sitem = _T("ORDER BY options");
  HTREEITEM orderby = InsertItem(sitem,select);
  sitem.Format(_T("Order by in select: %s"),m_info->GetOrderByInSelectAllowed() ? _T("Yes") : _T("No"));
  InsertItem(sitem, orderby);
  sitem.Format(_T("Expressions in the 'order by': %s"),m_info->GetExpressionsInOrderByAllowed() ? _T("Yes") : _T("No"));
  InsertItem(sitem, orderby);
  sitem.Format(_T("Maximum columns in 'order by': %s"),InfoNumberToString(m_info->GetMaxColumnsInOrderBy()));
  InsertItem(sitem,orderby);

  sitem = _T("UNION options");
  HTREEITEM un = InsertItem(sitem,select);
  SQLUINTEGER unions = m_info->GetUnionOptions();
  DisplayBitfield(un,unions,SQL_U_UNION,    _T("UNION"));
  DisplayBitfield(un,unions,SQL_U_UNION_ALL,_T("UNION ALL"));

  sitem = _T("Update");
  HTREEITEM update = InsertItem(sitem,infoItemSQLCF);
  SQLUINTEGER updateRules = m_info->GetUpdateOptions();
  DisplayBitfield(update,updateRules,SQL_SFKU_CASCADE,    _T("Set foreign keys: CASCADE"));
  DisplayBitfield(update,updateRules,SQL_SFKU_NO_ACTION,  _T("Set foreign keys: NO ACTION"));
  DisplayBitfield(update,updateRules,SQL_SFKU_SET_DEFAULT,_T("Set foreign keys: DEFAULT"));
  DisplayBitfield(update,updateRules,SQL_SFKU_SET_NULL,   _T("Set foreign keys: NULL"));

  // RDBMS Implementation
  sitem = _T("RDBMS storage: ");
  switch(m_info->GetDatabaseFileUsage())
  {
    case SQL_FILE_TABLE:         sitem += _T("1 file = 1 table");    break;
    case SQL_FILE_CATALOG:       sitem += _T("1 file = catalog");    break;
    case SQL_FILE_NOT_SUPPORTED: sitem += _T("Multi-tier database"); break;
    default:                     sitem += _T("(No information)");    break;
  }
  InsertItem(sitem,infoItemImpl);

  sitem  = _T("RDBMS catalog support: ");
  sitem += m_info->GetSupportsCatalogs() ? _T("Yes") : _T("No");
  InsertItem(sitem,infoItemImpl);
  InsertItem(_T("Term for a catalog: ")   + m_info->GetCatalogTerm(),   infoItemImpl);
  InsertItem(_T("Term for a schema: ")    + m_info->GetSchemaTerm(),    infoItemImpl);
  InsertItem(_T("Term for a table: ")     + m_info->GetTableTerm(),     infoItemImpl);
  InsertItem(_T("Term for a procedure: ") + m_info->GetProcedureTerm(), infoItemImpl);
  CString line(_T("Catalog identifiers: "));
  switch(m_info->GetIdentifierCase())
  {
    case SQL_IC_UPPER:     line += _T("Upper-case");    break;
    case SQL_IC_LOWER:     line += _T("Lower-case");    break;
    case SQL_IC_SENSITIVE: line += _T("Sensitive (?)"); break;
    case SQL_IC_MIXED:     line += _T("Mixed-case");    break;
    default:               line += _T("No information");break;
  }
  InsertItem(line,infoItemImpl);
  sitem = _T("Catalog name location: ");
  SQLSMALLINT catalogLocation = m_info->GetCatalogLocation();
  sitem += (catalogLocation == SQL_CL_START) ? _T("Before schema") : 
           (catalogLocation == SQL_CL_END)   ? _T("After table")   : 
           _T("(Unknown)");
  InsertItem(sitem,infoItemImpl);
  InsertItem(_T("Catalog name separator: ") + m_info->GetCatalogNameSeparator(),infoItemImpl);
  HTREEITEM cu = InsertItem(_T("Catalog name usage"),infoItemImpl);
  SQLUINTEGER catalogUsage = m_info->GetCatalogNameUsage();
  DisplayBitfield(cu,catalogUsage,SQL_CU_DML_STATEMENTS,      _T("DML Statements"));
  DisplayBitfield(cu,catalogUsage,SQL_CU_PROCEDURE_INVOCATION,_T("Procedure invocation"));
  DisplayBitfield(cu,catalogUsage,SQL_CU_TABLE_DEFINITION,    _T("Table definition"));
  DisplayBitfield(cu,catalogUsage,SQL_CU_INDEX_DEFINITION,    _T("Index definition"));
  DisplayBitfield(cu,catalogUsage,SQL_CU_PRIVILEGE_DEFINITION,_T("Privilege definition"));

  sitem.Format(_T("Maximum length of catalogname: %i"),m_info->GetMaxCatalogNameLength());
  InsertItem(sitem, infoItemImpl);
  sitem.Format(_T("Maximum length of schemaname: %i"), m_info->GetMaxSchemaNameLength());
  InsertItem(sitem, infoItemImpl);
  sitem.Format(_T("Maximum length of tablename: %i"),  m_info->GetMaxTableNameLength());
  InsertItem(sitem, infoItemImpl);
  sitem.Format(_T("Maximum length of columnname: %i"), m_info->GetMaxColumnNameLength());
  InsertItem(sitem, infoItemImpl);
  sitem.Format(_T("Maximum number of columns in a table: %i"), m_info->GetMaxColumnsInTable());
  InsertItem(sitem, infoItemImpl);

  InsertItem(_T("Special characters: ") + m_info->GetSpecialCharacters(),        infoItemImpl);
  InsertItem(_T("Like escape char: ")   + m_info->GetLikeEscapeCharacter(),      infoItemImpl);
  InsertItem(_T("Quoted identifiers: ") + m_info->GetIdentifierQuoteCharacter(), infoItemImpl);

  sitem = _T("Collation sequence: ");
  sitem += m_info->GetCollationSequence();
  InsertItem(sitem,infoItemImpl);

  sitem = _T("NULL Collation: ");
  switch(m_info->GetNULLCollation())
  {
    case SQL_NC_HIGH:  sitem += _T("High end of resultset, depending on ASC/DESC");         break;
    case SQL_NC_LOW:   sitem += _T("Low end of resultset, depending on ASC/DESC");          break;
    case SQL_NC_START: sitem += _T("At the start of the resultset, regardless of ASC/DESC");break;
    case SQL_NC_END:   sitem += _T("At the end of the resultset, regardless of ASC/DESC");  break;
    default:           sitem += _T("(No information)");                                     break;
  }
  InsertItem(sitem,infoItemImpl);

  sitem = _T("Concat NULL behaviour: ");
  switch(m_info->GetNULLConcatBehaviour())
  {
    case SQL_CB_NULL:     sitem += _T("Results in NULL");     break;
    case SQL_CB_NON_NULL: sitem += _T("Results in NON-NULL"); break;
    default:              sitem += _T("(No information)");    break;
  }
  InsertItem(sitem,infoItemImpl);

  sitem = _T("Nullable columns: ");
  switch(m_info->GetColumnsCanBeNULL())
  {
    case SQL_NNC_NULL:     sitem += _T("All columns are nullable"); break;
    case SQL_NNC_NON_NULL: sitem += _T("Columns can be non-null");  break;
    default:               sitem += _T("(No information)");         break;
  }
  InsertItem(sitem,infoItemImpl);

  HTREEITEM dtl = InsertItem(_T("Date/Time/Interval literals"),infoItemImpl);
  SQLUINTEGER datetimeLiterals = m_info->GetSupportedDateTimeLiterals();
  DisplayBitfield(dtl,datetimeLiterals,SQL_DL_SQL92_DATE                     ,_T("Date"));
  DisplayBitfield(dtl,datetimeLiterals,SQL_DL_SQL92_TIME                     ,_T("Time"));
  DisplayBitfield(dtl,datetimeLiterals,SQL_DL_SQL92_TIMESTAMP                ,_T("Timestamp"));
  DisplayBitfield(dtl,datetimeLiterals,SQL_DL_SQL92_INTERVAL_YEAR            ,_T("Interval year"));
  DisplayBitfield(dtl,datetimeLiterals,SQL_DL_SQL92_INTERVAL_MONTH           ,_T("Interval month"));
  DisplayBitfield(dtl,datetimeLiterals,SQL_DL_SQL92_INTERVAL_DAY             ,_T("Interval day"));
  DisplayBitfield(dtl,datetimeLiterals,SQL_DL_SQL92_INTERVAL_HOUR            ,_T("Interval hour"));
  DisplayBitfield(dtl,datetimeLiterals,SQL_DL_SQL92_INTERVAL_MINUTE          ,_T("Interval minute"));
  DisplayBitfield(dtl,datetimeLiterals,SQL_DL_SQL92_INTERVAL_SECOND          ,_T("Interval second"));
  DisplayBitfield(dtl,datetimeLiterals,SQL_DL_SQL92_INTERVAL_YEAR_TO_MONTH   ,_T("Interval year to month"));
  DisplayBitfield(dtl,datetimeLiterals,SQL_DL_SQL92_INTERVAL_DAY_TO_HOUR     ,_T("Interval day to hour"));
  DisplayBitfield(dtl,datetimeLiterals,SQL_DL_SQL92_INTERVAL_DAY_TO_MINUTE   ,_T("Interval day to minute"));
  DisplayBitfield(dtl,datetimeLiterals,SQL_DL_SQL92_INTERVAL_DAY_TO_SECOND   ,_T("Interval day to second"));
  DisplayBitfield(dtl,datetimeLiterals,SQL_DL_SQL92_INTERVAL_HOUR_TO_MINUTE  ,_T("Interval hour to minute"));
  DisplayBitfield(dtl,datetimeLiterals,SQL_DL_SQL92_INTERVAL_HOUR_TO_SECOND  ,_T("Interval hour to second"));
  DisplayBitfield(dtl,datetimeLiterals,SQL_DL_SQL92_INTERVAL_MINUTE_TO_SECOND,_T("Interval minute to second"));

  sitem = _T("Active environments: ");
  if(!m_info->GetMaxActiveEnvironments())
  {
    sitem += _T(" (no limit or unknown)");
  }
  else 
  {
    sitem += InfoNumberToString(m_info->GetMaxActiveEnvironments());
  }
  InsertItem(sitem,infoItemImpl);

  sitem.Format(_T("Transaction integrity: %s"),m_info->GetTransactionIntegrity() ? _T("Yes") : _T("No"));
  InsertItem(sitem, infoItemImpl);

  sitem = _T("Transactions behaviour: ");
  switch(m_info->GetTransactionCapabilities())
  {
    case SQL_TC_DML:        sitem += _T("Only DML");    break;
    case SQL_TC_DDL_COMMIT: sitem += _T("DDL Commits"); break;
    case SQL_TC_DDL_IGNORE: sitem += _T("DDL Ignores"); break;
    case SQL_TC_ALL:        sitem += _T("DDL and DML"); break;
    case SQL_TC_NONE:
    default:                sitem += _T("None");        break;
  }
  InsertItem(sitem, infoItemImpl);

  sitem = _T("Default transaction level: ");
  switch(m_info->GetDefaultTransactionLevel())
  {
    case SQL_TXN_READ_UNCOMMITTED: sitem += _T("Read uncommitted"); break;
    case SQL_TXN_READ_COMMITTED:   sitem += _T("Read committed");   break;
    case SQL_TXN_REPEATABLE_READ:  sitem += _T("Repeatable read");  break;
    case SQL_TXN_SERIALIZABLE:     sitem += _T("Serializable");     break;
    default:                       sitem += _T("(No information)"); break;
  }
  InsertItem(sitem, infoItemImpl);

  sitem = _T("Cursor commit behaviour: ");
  SQLSMALLINT cursor_commit = m_info->GetCursorCommitBehaviour();
  if (cursor_commit == SQL_CB_DELETE)      sitem += _T("Close cursor and remove statement");
  if (cursor_commit == SQL_CB_CLOSE)       sitem += _T("Close cursor");
  if (cursor_commit == SQL_CB_PRESERVE)    sitem += _T("Preserve cursor");
  InsertItem(sitem, infoItemImpl);

  sitem = _T("Cursor rollback behaviour: ");
  SQLSMALLINT cursor_rollback = m_info->GetCursorRollbackBehaviour();
  if (cursor_rollback == SQL_CB_DELETE)      sitem += _T("Close cursor and remove statement");
  if (cursor_rollback == SQL_CB_CLOSE)       sitem += _T("Close cursor");
  if (cursor_rollback == SQL_CB_PRESERVE)    sitem += _T("Preserve cursor");
  InsertItem(sitem, infoItemImpl);

  InsertItem(CString(_T("Accessible tables: "))     + (m_info->GetAccessibleTables()     ? _T("Yes") : _T("No")), infoItemImpl);
  InsertItem(CString(_T("Accessible procedures: ")) + (m_info->GetAccessibleProcedures() ? _T("Yes") : _T("No")), infoItemImpl);

  // RDBMS SQL gereserveerde woorden
  HTREEITEM words = InsertItem(_T("RDBMS SQL Reserved words"),infoItemImpl);
  didInfo =  false;
  WordList& RDBMSkeywords = m_info->GetRDBMSKeywords();
  RDBMSkeywords.sort();
  for(auto& word : RDBMSkeywords)
  {
    if(!word.IsEmpty()) 
    {
      didInfo = true;
      InsertItem(word,words);
    }
  }
  if(!didInfo)
  {
    InsertItem(_T("No info"),words);
  }
  // RDBMS Supports INFORMATION_SCHEMA views
  HTREEITEM schviews = InsertItem(_T("INFORMATION_SCHEMA views"),infoItemImpl);
  SQLINTEGER schemaViews = m_info->GetInformationSchemaViews();
  DisplayBitfield(schviews,schemaViews,SQL_ISV_ASSERTIONS,             _T("ASSERTIONS"));
  DisplayBitfield(schviews,schemaViews,SQL_ISV_CHARACTER_SETS,         _T("CHARACTER_SETS"));
  DisplayBitfield(schviews,schemaViews,SQL_ISV_CHECK_CONSTRAINTS,      _T("CHECK_CONSTRAINTS"));
  DisplayBitfield(schviews,schemaViews,SQL_ISV_COLLATIONS,             _T("COLLATIONS"));
  DisplayBitfield(schviews,schemaViews,SQL_ISV_COLUMN_DOMAIN_USAGE,    _T("COLUMN_DOMAIN_USAGE"));
  DisplayBitfield(schviews,schemaViews,SQL_ISV_COLUMN_PRIVILEGES,      _T("COLUMN_PRIVILEGES"));
  DisplayBitfield(schviews,schemaViews,SQL_ISV_COLUMNS,                _T("COLUMNS"));
  DisplayBitfield(schviews,schemaViews,SQL_ISV_CONSTRAINT_COLUMN_USAGE,_T("CONSTRAINT_COLUMN_USAGE"));
  DisplayBitfield(schviews,schemaViews,SQL_ISV_CONSTRAINT_TABLE_USAGE, _T("CONSTRAINT_TABLE_USAGE"));
  DisplayBitfield(schviews,schemaViews,SQL_ISV_DOMAIN_CONSTRAINTS,     _T("DOMAIN_CONSTRAINTS"));
  DisplayBitfield(schviews,schemaViews,SQL_ISV_DOMAINS,                _T("DOMAINS"));
  DisplayBitfield(schviews,schemaViews,SQL_ISV_KEY_COLUMN_USAGE,       _T("KEY_COLUMN_USAGE"));
  DisplayBitfield(schviews,schemaViews,SQL_ISV_REFERENTIAL_CONSTRAINTS,_T("REFERENTIAL_CONSTRAINTS"));
  DisplayBitfield(schviews,schemaViews,SQL_ISV_SCHEMATA,               _T("SCHEMATA"));
  DisplayBitfield(schviews,schemaViews,SQL_ISV_SQL_LANGUAGES,          _T("SQL_LANGUAGES"));
  DisplayBitfield(schviews,schemaViews,SQL_ISV_TABLE_CONSTRAINTS,      _T("TABLE_CONSTRAINTS"));
  DisplayBitfield(schviews,schemaViews,SQL_ISV_TABLE_PRIVILEGES,       _T("TABLE_PRIVILEGES"));
  DisplayBitfield(schviews,schemaViews,SQL_ISV_TABLES,                 _T("TABLES"));
  DisplayBitfield(schviews,schemaViews,SQL_ISV_TRANSLATIONS,           _T("TRANSLATIONS"));
  DisplayBitfield(schviews,schemaViews,SQL_ISV_USAGE_PRIVILEGES,       _T("USAGE_PRIVILEGES"));
  DisplayBitfield(schviews,schemaViews,SQL_ISV_VIEW_COLUMN_USAGE,      _T("VIEW_COLUMN_USAGE"));
  DisplayBitfield(schviews,schemaViews,SQL_ISV_VIEW_TABLE_USAGE,       _T("VIEW_TABLE_USAGE"));
  DisplayBitfield(schviews,schemaViews,SQL_ISV_VIEWS,                  _T("VIEWS"));

  // VERSION NUMBERS
  InsertItem(_T("ODBC Driver manager: ")   + m_info->GetVersionODBCManager(),   infoItemVersion);
  InsertItem(_T("ODBC Database driver: ")  + m_info->GetVersionODBCDriver(),    infoItemVersion);
  InsertItem(_T("ODBC Standard: ")         + m_info->GetVersionODBCStandard(),  infoItemVersion);
  InsertItem(_T("RDBMS version: ")         + m_info->GetVersionRDBMS(),         infoItemVersion);
}

// void
// InfoTree::WordListToTree(WordList& p_list,HTREEITEM p_item)
// {
//   HTREEITEM item = p_item;
//   HTREEITEM last = NULL;
//   for(WordList::iterator it = p_list.begin(); it != p_list.end(); ++it)
//   {
//     CString line = *it;
//     if(line == "+" && last)
//     {
//       // één inspringen
//       item = last;
//       continue;
//     }
//     if(line == "-")
//     {
//       // Reset to original
//       item = p_item;
//     }
//     else
//     {
//       last = InsertItem(line,item);
//     }
//   }
//   p_list.clear();
// }

void
InfoTree::MetaListToTree(MMetaMap& p_list,HTREEITEM p_item,CString& p_errors)
{
  // Put all meta-objects in the tree
  for(auto& object : p_list)
  {
    CString line = object.m_objectName;
    if(!object.m_remarks.IsEmpty())
    {
      line.AppendFormat(_T(" (%s)"),object.m_remarks);
    }
    InsertItem(line,p_item);
  }

  // Eventually all errors
  if(!p_errors.IsEmpty())
  {
    InsertItem(p_errors,p_item);
  }

  // Ready for next query
  p_list.clear();
  p_errors.Empty();
}

void
InfoTree::ReportAllCapabilities(CString& filename)
{
  if(!m_info)
  {
    return;
  }

  m_info->GetInfo();

  WinFile file(filename);


  if(!file.Open(winfile_write | open_trans_text,FAttributes::attrib_none,Encoding::UTF8))
  {
    AfxMessageBox(_T("Cannot open file for ODBC report: ") + filename);
    return;
  }
  file.Write(CString(_T("ODBC Driver report\n")));
  file.Write(CString(_T("==================\n\n")));

  HTREEITEM root  = GetRootItem();
  ReportCapabilities(root,0,file);

  // END OF REPORT
  file.Write(CString(_T("\n*** End of report ***\n")));
  file.Close();
}

//////////////////////////////////////////////////////////////////////////
//
// PRIVATE / HELPERS
//
//////////////////////////////////////////////////////////////////////////

// Helper for displaying SQLGetInfo information
HTREEITEM
InfoTree::DisplayBitfield(HTREEITEM   item
                         ,SQLUINTEGER var
                         ,SQLUINTEGER macro
                         ,TCHAR*       text)
{
  if(var & macro)
  {
    return InsertItem(text,item);
  }
  else
  {
    return InsertItem(CString(text) + _T(" (not supported)"),item);
  }
}

CString
InfoTree::InfoNumberToString(SQLINTEGER number)
{
  CString num;
  if(num == -1)
  {
    num = _T("(no info from driver)");
  }
  else
  {
    num.Format(_T("%d"),number);
  }
  return num;
}

// Display a datatype CONVERT
void
InfoTree::DisplayConversion(HTREEITEM  item
                           ,CString    type
                           ,SQLINTEGER totype)
{
  if(!totype)
  {
    // Data type not supported or no conversions defined
    return;
  }
  HTREEITEM datatype = InsertItem(type,item);
  if(totype & SQL_CVT_BIGINT)              InsertItem(_T("BIGINT"), datatype);
  if(totype & SQL_CVT_BINARY)              InsertItem(_T("BINARY"), datatype);
  if(totype & SQL_CVT_BIT)                 InsertItem(_T("BIT"),    datatype);
  if(totype & SQL_CVT_CHAR)                InsertItem(_T("CHAR"),   datatype);
  if(totype & SQL_CVT_DATE)                InsertItem(_T("DATE"),   datatype);
  if(totype & SQL_CVT_DECIMAL)             InsertItem(_T("DECIMAL"),datatype);
  if(totype & SQL_CVT_DOUBLE)              InsertItem(_T("DOUBLE"), datatype);
  if(totype & SQL_CVT_FLOAT)               InsertItem(_T("FLOAT"),  datatype);
  if(totype & SQL_CVT_INTEGER)             InsertItem(_T("INTEGER"),datatype);
  if(totype & SQL_CVT_INTERVAL_YEAR_MONTH) InsertItem(_T("INTERVAL YEAR-MONTH"),datatype);
  if(totype & SQL_CVT_INTERVAL_DAY_TIME)   InsertItem(_T("INTERVAL DAY-TIME"),  datatype);
  if(totype & SQL_CVT_LONGVARBINARY)       InsertItem(_T("LONGVARBINARY"),      datatype);
  if(totype & SQL_CVT_LONGVARCHAR)         InsertItem(_T("LONGVARCHAR"),        datatype);
  if(totype & SQL_CVT_NUMERIC)             InsertItem(_T("NUMERIC"),  datatype);
  if(totype & SQL_CVT_REAL)                InsertItem(_T("REAL"),     datatype);
  if(totype & SQL_CVT_SMALLINT)            InsertItem(_T("SMALLINT"), datatype);
  if(totype & SQL_CVT_TIME)                InsertItem(_T("TIME"),     datatype);
  if(totype & SQL_CVT_TIMESTAMP)           InsertItem(_T("TIMESTAMP"),datatype);
  if(totype & SQL_CVT_TINYINT)             InsertItem(_T("TINYINT"),  datatype);
  if(totype & SQL_CVT_VARBINARY)           InsertItem(_T("VARBINARY"),datatype);
  if(totype & SQL_CVT_VARCHAR)             InsertItem(_T("VARCHAR"),  datatype);
}

void
InfoTree::MakeTreeODBCExtensions(HTREEITEM item)
{
  CString item_any   = _T("SQLGetData can only be called on columns AFTER the last bound column");
  CString item_order = _T("SQLGetData can only be called in ascending order");
  CString item_block = _T("SQLGetData can *NOT* be called for unbound columns in block cursors");
  CString item_bound = _T("SQLGetData can *NOT* be called for bound columns");
  CString item_param = _T("SQLGetData can *NOT* be called to retrieve output parameters");

  SQLUINTEGER getdata_extensions = m_info->GetGetDataExtensions();

  if(getdata_extensions & SQL_GD_ANY_COLUMN)
  {
    item_any = _T("SQLGetData can be called for ANY unbound column");
  }
  if(getdata_extensions & SQL_GD_ANY_ORDER)
  {
    item_order = _T("SQLGetData can be called on columns in ANY order");
  }
  if(getdata_extensions & SQL_GD_BLOCK)
  {
    item_block = _T("SQLGetData can be called on unbound columns in block cursors");
  }
  if(getdata_extensions & SQL_GD_BOUND)
  {
    item_bound = _T("SQLGetData can be called for bound columns");
  }
  if(getdata_extensions & SQL_GD_OUTPUT_PARAMS)
  {
    item_param = _T("SQLGetData can be called to retrieve output parameters");
  }
  InsertItem(item_any,  item);
  InsertItem(item_order,item);
  InsertItem(item_block,item);
  InsertItem(item_bound,item);
  InsertItem(item_param,item);
}

// All ODBC Functions to the tree
void
InfoTree::MakeTreeInfoFunctions(HTREEITEM item)
{
  CString sitem;

  SQLUSMALLINT* ODBCFunctions_3 = m_info->GetFunctionArrayV3();
  SQLUSMALLINT* ODBCFunctions_2 = m_info->GetFunctionArrayV2();

  // ODBC Supported functions
  if(m_info->GetSupportsV3Functions())
  {
    InsertItem(_T("Driver reports ODBC 3.x Functions"),item);

    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLALLOCCONNECT) ? _T("X") : _T("O")) + _T(" SQLAllocConnect");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLALLOCENV)     ? _T("X") : _T("O")) + _T(" SQLAllocEnv");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLALLOCHANDLE)  ? _T("X") : _T("O")) + _T(" SQLAllocHandle");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLALLOCHANDLESTD)?_T("X") : _T("O")) + _T(" SQLAllocHandleStd");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLALLOCSTMT)    ? _T("X") : _T("O")) + _T(" SQLAllocStmt");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLBINDCOL)      ? _T("X") : _T("O")) + _T(" SQLBindCol");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLBINDPARAM)    ? _T("X") : _T("O")) + _T(" SQLBindParam");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLBINDPARAMETER)? _T("X") : _T("O")) + _T(" SQLBindParameter");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLBROWSECONNECT)? _T("X") : _T("O")) + _T(" SQLBrowseConnect");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLBULKOPERATIONS)?_T("X") : _T("O")) + _T(" SQLBulkOperations");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLCANCEL)       ? _T("X") : _T("O")) + _T(" SQLCancel");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLCLOSECURSOR)  ? _T("X") : _T("O")) + _T(" SQLCloseCursor");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLCOLATTRIBUTE) ? _T("X") : _T("O")) + _T(" SQLColAttribute");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLCOLATTRIBUTES)? _T("X") : _T("O")) + _T(" SQLColAttributes");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLCOLUMNS)      ? _T("X") : _T("O")) + _T(" SQLColumns");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLCOLUMNPRIVILEGES)?_T("X"):_T("O")) + _T(" SQLColumnPrivileges");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLCONNECT)      ? _T("X") : _T("O")) + _T(" SQLConnect");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLCOPYDESC)     ? _T("X") : _T("O")) + _T(" SQLCopyDesc");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLDATASOURCES)  ? _T("X") : _T("O")) + _T(" SQLDataSources");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLDESCRIBECOL)  ? _T("X") : _T("O")) + _T(" SQLDescribeCol");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLDESCRIBEPARAM)? _T("X") : _T("O")) + _T(" SQLDescribeParam");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLDISCONNECT)   ? _T("X") : _T("O")) + _T(" SQLDisconnect");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLDRIVERCONNECT)? _T("X") : _T("O")) + _T(" SQLDriverConnect");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLDRIVERS)      ? _T("X") : _T("O")) + _T(" SQLDrivers");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLENDTRAN)      ? _T("X") : _T("O")) + _T(" SQLEndTran");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLERROR)        ? _T("X") : _T("O")) + _T(" SQLError");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLEXECDIRECT)   ? _T("X") : _T("O")) + _T(" SQLExecDirect");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLEXECUTE)      ? _T("X") : _T("O")) + _T(" SQLExecute");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLEXTENDEDFETCH)? _T("X") : _T("O")) + _T(" SQLExetendedFetch");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLFETCH)        ? _T("X") : _T("O")) + _T(" SQLFetch");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLFETCHSCROLL)  ? _T("X") : _T("O")) + _T(" SQLFetchScroll");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLFOREIGNKEYS)  ? _T("X") : _T("O")) + _T(" SQLForeignKeys");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLFREECONNECT)  ? _T("X") : _T("O")) + _T(" SQLFreeConnect");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLFREEENV)      ? _T("X") : _T("O")) + _T(" SQLFreeEnv");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLFREEHANDLE)   ? _T("X") : _T("O")) + _T(" SQLFreeHandle");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLFREESTMT)     ? _T("X") : _T("O")) + _T(" SQLFreeStmt");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLGETCONNECTATTR)   ? _T("X") : _T("O")) + _T(" SQLGetConnectAttr");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLGETCONNECTOPTION) ? _T("X") : _T("O")) + _T(" SQLGetConnectOption");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLGETCURSORNAME)    ? _T("X") : _T("O")) + _T(" SQLGetCursorName");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLGETDATA)          ? _T("X") : _T("O")) + _T(" SQLGetData");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLGETDESCFIELD)     ? _T("X") : _T("O")) + _T(" SQLGetDescField");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLGETDESCREC)       ? _T("X") : _T("O")) + _T(" SQLGetDescRec");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLGETDIAGFIELD)     ? _T("X") : _T("O")) + _T(" SQLGetDiagField");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLGETDIAGREC)       ? _T("X") : _T("O")) + _T(" SQLGetDiagRec");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLGETENVATTR)       ? _T("X") : _T("O")) + _T(" SQLGetEnvAttr");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLGETFUNCTIONS)     ? _T("X") : _T("O")) + _T(" SQLGetFunctions");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLGETINFO)          ? _T("X") : _T("O")) + _T(" SQLGetInfo");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLGETSTMTATTR)      ? _T("X") : _T("O")) + _T(" SQLGetStmtAttr");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLGETSTMTOPTION)    ? _T("X") : _T("O")) + _T(" SQLGetStmtOption");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLGETTYPEINFO)      ? _T("X") : _T("O")) + _T(" SQLGetTypeInfo");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLMORERESULTS)      ? _T("X") : _T("O")) + _T(" SQLMoreResults");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLNATIVESQL)        ? _T("X") : _T("O")) + _T(" SQLNativeSQL");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLNUMPARAMS)        ? _T("X") : _T("O")) + _T(" SQLNumParams");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLNUMRESULTCOLS)    ? _T("X") : _T("O")) + _T(" SQLNumResultCols");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLPARAMDATA)        ? _T("X") : _T("O")) + _T(" SQLParamData");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLPARAMOPTIONS)     ? _T("X") : _T("O")) + _T(" SQLParamOptions");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLPREPARE)          ? _T("X") : _T("O")) + _T(" SQLPrepare");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLPRIMARYKEYS)      ? _T("X") : _T("O")) + _T(" SQLPrimaryKeys");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLPROCEDURECOLUMNS) ? _T("X") : _T("O")) + _T(" SQLProcedureColumns");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLPROCEDURES)       ? _T("X") : _T("O")) + _T(" SQLProcedures");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLPUTDATA)          ? _T("X") : _T("O")) + _T(" SQLPutData");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLROWCOUNT)         ? _T("X") : _T("O")) + _T(" SQLRowCount");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLSETSCROLLOPTIONS) ? _T("X") : _T("O")) + _T(" SQLSetScrollOptions");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLSETCONNECTATTR)   ? _T("X") : _T("O")) + _T(" SQLSetConnectAttr");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLSETCONNECTOPTION) ? _T("X") : _T("O")) + _T(" SQLSetConnectOption");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLSETCURSORNAME)    ? _T("X") : _T("O")) + _T(" SQLSetCursorName");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLSETDESCFIELD)     ? _T("X") : _T("O")) + _T(" SQLSetDescField");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLSETDESCREC)       ? _T("X") : _T("O")) + _T(" SQLSetDescRec");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLSETENVATTR)       ? _T("X") : _T("O")) + _T(" SQLSetEnvAttr");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLSETPARAM)         ? _T("X") : _T("O")) + _T(" SQLSetParam");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLSETPOS)           ? _T("X") : _T("O")) + _T(" SQLSetPos");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLSETSTMTATTR)      ? _T("X") : _T("O")) + _T(" SQLSetStmtAttr");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLSETSTMTOPTION)    ? _T("X") : _T("O")) + _T(" SQLSetStmtOption");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLSPECIALCOLUMNS)   ? _T("X") : _T("O")) + _T(" SQLSpecialColumns");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLSTATISTICS)       ? _T("X") : _T("O")) + _T(" SQLStatistics");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLTABLES)           ? _T("X") : _T("O")) + _T(" SQLTables");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLTABLEPRIVILEGES)  ? _T("X") : _T("O")) + _T(" SQLTablePrivileges");
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLTRANSACT)         ? _T("X") : _T("O")) + _T(" SQLTransact");
    InsertItem(sitem,item);
  }
  else
  {
    InsertItem(_T("Driver reports ODBC 1.x/2.x Functions"),item);

    sitem = CString(ODBCFunctions_2[SQL_API_SQLALLOCCONNECT] ? _T("X") : _T("O")) + _T("SQLAllocConnect");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLALLOCENV]     ? _T("X") : _T("O")) + _T("SQLAllocEnv");
    InsertItem(sitem,item);
    sitem = _T("# SQLAllocHandle");
    InsertItem(sitem,item);
    sitem = _T("# SQLAllocHandleStd");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLALLOCSTMT]    ? _T("X") : _T("O")) + _T("SQLAllocStmt");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLBINDCOL]      ? _T("X") : _T("O")) + _T("SQLBindCol");
    InsertItem(sitem,item);
    sitem = _T("# SQLBindParam");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLBINDPARAMETER]? _T("X") : _T("O")) + _T("SQLBindParameter");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLBROWSECONNECT]? _T("X") : _T("O")) + _T("SQLBrowseConnect");
    InsertItem(sitem,item);
    sitem = _T("# SQLBulkOperations");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLCANCEL]       ? _T("X") : _T("O")) + _T("SQLCancel");
    InsertItem(sitem,item);
    sitem = _T("# SQLCloseCursor");
    InsertItem(sitem,item);
    sitem = _T("# SQLColAttribute");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLCOLATTRIBUTES]? _T("X") : _T("O")) + _T("SQLColAttributes");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLCOLUMNS]      ? _T("X") : _T("O")) + _T("SQLColumns");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLCOLUMNPRIVILEGES]?_T("X"):_T("O")) + _T("SQLColumnPrivileges");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLCONNECT]      ? _T("X") : _T("O")) + _T("SQLConnect");
    InsertItem(sitem,item);
    sitem = _T("# SQLCopyDesc");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLDATASOURCES]  ? _T("X") : _T("O")) + _T("SQLDataSources");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLDESCRIBECOL]  ? _T("X") : _T("O")) + _T("SQLDescribeCol");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLDESCRIBEPARAM]? _T("X") : _T("O")) + _T("SQLDescribeParam");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLDISCONNECT]   ? _T("X") : _T("O")) + _T("SQLDisconnect");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLDRIVERCONNECT]? _T("X") : _T("O")) + _T("SQLDriverConnect");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLDRIVERS]      ? _T("X") : _T("O")) + _T("SQLDrivers");
    InsertItem(sitem,item);
    sitem = _T("# SQLEndTran");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLERROR]        ? _T("X") : _T("O")) + _T("SQLError");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLEXECDIRECT]   ? _T("X") : _T("O")) + _T("SQLExecDirect");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLEXECUTE]      ? _T("X") : _T("O")) + _T("SQLExecute");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLEXTENDEDFETCH]? _T("X") : _T("O")) + _T("SQLExtendedFetch");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLFETCH]        ? _T("X") : _T("O")) + _T("SQLFetch");
    InsertItem(sitem,item);
    sitem = _T("# SQLFetchScroll");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLFOREIGNKEYS]  ? _T("X") : _T("O")) + _T("SQLForeignKeys");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLFREECONNECT]  ? _T("X") : _T("O")) + _T("SQLFreeConnect");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLFREEENV]      ? _T("X") : _T("O")) + _T("SQLFreeEnv");
    InsertItem(sitem,item);
    sitem = _T("# SQLFreeHandle");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLFREESTMT]      ? _T("X") : _T("O")) + _T("SQLFreeStmt");
    InsertItem(sitem,item);
    sitem = _T("# SQLGetConnectAttr");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLGETCONNECTOPTION]  ? _T("X") : _T("O")) + _T("SQLGetConnectOption");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLGETCURSORNAME]     ? _T("X") : _T("O")) + _T("SQLGetCursorName");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLGETDATA]           ? _T("X") : _T("O")) + _T("SQLGetData");
    InsertItem(sitem,item);
    sitem = _T("# SQLGetDescField");
    InsertItem(sitem,item);
    sitem = _T("# SQLGetDescRec");
    InsertItem(sitem,item);
    sitem = _T("# SQLGetDiagField");
    InsertItem(sitem,item);
    sitem = _T("# SQLGetDiagRec");
    InsertItem(sitem,item);
    sitem = _T("# SQLGetEnvAttr");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLGETFUNCTIONS]      ? _T("X") : _T("O")) + _T("SQLGetFunctions");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLGETINFO]           ? _T("X") : _T("O")) + _T("SQLGetInfo");
    InsertItem(sitem,item);
    sitem = _T("# SQLGetStmtAttr");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLGETSTMTOPTION]     ? _T("X") : _T("O")) + _T("SQLGetStmtOption");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLGETTYPEINFO]       ? _T("X") : _T("O")) + _T("SQLGetTypeInfo");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLMORERESULTS]       ? _T("X") : _T("O")) + _T("SQLMoreResults");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLNATIVESQL]         ? _T("X") : _T("O")) + _T("SQLNativeSQL");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLNUMPARAMS]         ? _T("X") : _T("O")) + _T("SQLNumParams");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLNUMRESULTCOLS]     ? _T("X") : _T("O")) + _T("SQLNumResultCols");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLPARAMDATA]         ? _T("X") : _T("O")) + _T("SQLParamData");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLPARAMOPTIONS]      ? _T("X") : _T("O")) + _T("SQLParamOptions");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLPREPARE]           ? _T("X") : _T("O")) + _T("SQLPrepare");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLPRIMARYKEYS]       ? _T("X") : _T("O")) + _T("SQLPrimaryKeys");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLPROCEDURECOLUMNS]  ? _T("X") : _T("O")) + _T("SQLProcedureColumns");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLPROCEDURES]        ? _T("X") : _T("O")) + _T("SQLProcedures");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLPUTDATA]           ? _T("X") : _T("O")) + _T("SQLPutData");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLROWCOUNT]          ? _T("X") : _T("O")) + _T("SQLRowCount");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLSETSCROLLOPTIONS]  ? _T("X") : _T("O")) + _T("SQLSetScrollOptions");
    InsertItem(sitem,item);
    sitem = _T("# SQLSetConnectAttr");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLSETCONNECTOPTION]  ? _T("X") : _T("O")) + _T("SQLSetConnectOption");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLSETCURSORNAME]     ? _T("X") : _T("O")) + _T("SQLSetCursorName");
    InsertItem(sitem,item);
    sitem = _T("# SQLSetDescField");
    InsertItem(sitem,item);
    sitem = _T("# SQLSetDescRec");
    InsertItem(sitem,item);
    sitem = _T("# SQLSetEnvAttr");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLSETPARAM]          ? _T("X") : _T("O")) + _T("SQLSetParam");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLSETPOS]            ? _T("X") : _T("O")) + _T("SQLSetPos");
    InsertItem(sitem,item);
    sitem = _T("# SQLSetStmtAttr");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLGETCONNECTOPTION]  ? _T("X") : _T("O")) + _T("SQLGetConnectOption");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLSPECIALCOLUMNS]    ? _T("X") : _T("O")) + _T("SQLSpecialColumns");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLSTATISTICS]        ? _T("X") : _T("O")) + _T("SQLStatistics");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLTABLES]            ? _T("X") : _T("O")) + _T("SQLTables");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLTABLEPRIVILEGES]   ? _T("X") : _T("O")) + _T("SQLTablePrivileges");
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLTRANSACT]          ? _T("X") : _T("O")) + _T("SQLTransact");
    InsertItem(sitem,item);
  }
}

void
InfoTree::ReportCapabilities(HTREEITEM  p_branch
                            ,int        p_level
                            ,WinFile&   p_file)
{
  HTREEITEM branch = p_branch;
  do 
  {
    // Print level to file
    for(int ind = 0; ind < p_level; ++ind)
    {
      p_file.Write(CString(_T("    ")));
    }
    // Print our info
    p_file.Format(_T("%s\n"),GetItemText(branch).GetString());
    // Do all of our children
    HTREEITEM child = GetNextItem(branch,TVGN_CHILD);
    if(child)
    {
      ReportCapabilities(child,p_level + 1,p_file);
    }
    // Next branch of the tree
    branch = GetNextItem(branch,TVGN_NEXT);
  } 
  while(branch);
}

