#include "pch.h"
#include "InfoTree.h"
#include "QueryTool.h"
#include "SQLComponents.h"
#include "SQLDatabase.h"
#include "sqlext.h"
#include "Resource.h"
#include "OEDocument.h"

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
    InsertItem("SQLGetInfo not supported: Get a better driver!",item);
    return;
  }
  infoItemDB      = InsertItem("Database",              item);
  infoItemDriver  = InsertItem("ODBC Driver",           item);
  infoItemDATAT   = InsertItem("SQL Data types",        item);
  infoItemCF      = InsertItem("SQL Conformance",       item);
  infoItemConv    = InsertItem("SQL Type conversions",  item);
  infoItemSQLCF   = InsertItem("SQL Supported standard",item);
  infoItemImpl    = InsertItem("RDBMS Implementation",  item);
  infoItemVersion = InsertItem("Version numbers",       item);

  // DATABASE NAME AND TYPE
  CString dbtype = "Database type: "  + m_database->GetDatabaseTypeName();
  CString dbname = "Database name: "  + m_database->GetDatabaseName();
  CString method = "Naming method: "  + m_database->GetNamingMethod();
  CString user   = "Logged in user: " + m_database->GetUserName();

  InsertItem(dbtype,infoItemDB);
  InsertItem(dbname,infoItemDB);
  InsertItem(method,infoItemDB);
  InsertItem(user,  infoItemDB);

  CString   errors;
  MMetaMap  objects;
  HTREEITEM meta     = InsertItem("Meta-objects in the database",infoItemDB);
  HTREEITEM catalogs = InsertItem("Catalogs",meta);
  m_info->MakeInfoMetaTypes(objects,errors,META_CATALOGS);
  MetaListToTree(objects,catalogs,errors);

  HTREEITEM schemas  = InsertItem("Schemas",meta);
  m_info->MakeInfoMetaTypes(objects,errors,META_SCHEMAS);
  MetaListToTree(objects,schemas,errors);

  HTREEITEM obtypes  = InsertItem("Object types",meta);
  m_info->MakeInfoMetaTypes(objects,errors,META_TABLES);
  MetaListToTree(objects,obtypes,errors);

  // ODBC DRIVER
  sitem.Format("ODBC Driver DLL: %s",m_info->GetDriverName());
  InsertItem(sitem,infoItemDriver);

  // ODBC SQL reserved words (static compile time)
  WordList& listODBC = m_info->GetODBCKeywords();
  listODBC.sort();
  HTREEITEM odbcres = InsertItem("ODBC SQL reserved words", infoItemDriver);
  for(auto& word : listODBC)
  {
    InsertItem(word,odbcres);
  }
  // Supported ODBC functions
  HTREEITEM odbcFun = InsertItem("ODBC Supported functions",infoItemDriver);
  MakeTreeInfoFunctions(odbcFun);

  // Driver implementation and extensions
  HTREEITEM odbcExt = InsertItem("ODBC driver implementation",infoItemDriver);
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

    sitem = "ODBC datatype name: " + m_info->ODBCDataType(ti->m_data_type);
    InsertItem(sitem,type);
    sitem.Format("Maximum precision: %s",InfoNumberToString(ti->m_precision));
    InsertItem(sitem,type);
    sitem = "Literal data prefix: " + ti->m_literal_prefix;
    InsertItem(sitem,type);
    sitem = "Literal data suffix: " + ti->m_literal_suffix;
    InsertItem(sitem,type);
    sitem = "How to create parameters: " + ti->m_create_params;
    InsertItem(sitem,type);
    sitem = "Datatype is nullable: ";
    switch(ti->m_nullable)
    {
      case SQL_NO_NULLS:         sitem += "No nulls"; break;
      case SQL_NULLABLE:         sitem += "Nullable"; break;
      case SQL_NULLABLE_UNKNOWN: sitem += "Unknown";  break;
    }
    InsertItem(sitem,type);
    sitem = CString("Datatype is case sensitive: ") + (ti->m_case_sensitive ? "Yes" : "No");
    InsertItem(sitem,type);
    sitem = "Datatype is searchable: ";
    switch(ti->m_searchable)
    {
      case SQL_UNSEARCHABLE:    sitem += "Unsearchable";    break;
      case SQL_LIKE_ONLY:       sitem += "Like only";       break;
      case SQL_ALL_EXCEPT_LIKE: sitem += "All except like"; break;
      case SQL_SEARCHABLE:      sitem += "Searchable";      break;
    }
    InsertItem(sitem,type);
    sitem = CString("Datatype is unsigned: ") + (ti->m_unsigned ? "Yes" : "No");
    InsertItem(sitem,type);
    sitem = CString("Datatype is money: ") + (ti->m_money ? "Yes" : "No");
    InsertItem(sitem,type);
    sitem = CString("Datatype is auto-increment: ") + (ti->m_autoincrement ? "Yes" : "No");
    InsertItem(sitem,type);
    sitem = "Local type name for display: " + ti->m_local_type_name;
    InsertItem(sitem,type);
    sitem.Format("Minimum scale: %s",InfoNumberToString(ti->m_minimum_scale));
    InsertItem(sitem,type);
    sitem.Format("Maximum scale: %s",InfoNumberToString(ti->m_maximum_scale));
    InsertItem(sitem,type);
    sitem.Format("Driver independent SQL datatype: %s",m_info->ODBCDataType(ti->m_sqlDatatype));
    InsertItem(sitem,type);
    sitem.Format("Subtype for temporal types: %d",ti->m_sqlSubType);
    InsertItem(sitem,type);
    sitem.Format("Decimal radix: %d",ti->m_radix);
    InsertItem(sitem,type);
    sitem.Format("Interval leading precision: %d",ti->m_interval_precision);
    InsertItem(sitem,type);
    // Next datatype
    ++it;
  }
  if(!didInfo)
  {
    InsertItem("No info",infoItemDATAT);
  }
  // SQL CONVERSIONS
  sitem = "Conversion functions";
  HTREEITEM convfunc = InsertItem(sitem,infoItemConv);
  SQLUINTEGER conversionFuncs = m_info->GetConversionFunctions();
  DisplayBitfield(convfunc,conversionFuncs,SQL_FN_CVT_CAST,   "CAST");
  DisplayBitfield(convfunc,conversionFuncs,SQL_FN_CVT_CONVERT,"CONVERT");

  DisplayConversion(infoItemConv,"BIGINT",             m_info->GetConvertBigint());
  DisplayConversion(infoItemConv,"BINARY",             m_info->GetConvertBinary());
  DisplayConversion(infoItemConv,"BIT",                m_info->GetConvertBit());
  DisplayConversion(infoItemConv,"CHAR",               m_info->GetConvertChar());
  DisplayConversion(infoItemConv,"DATE",               m_info->GetConvertDate());
  DisplayConversion(infoItemConv,"DECIMAL",            m_info->GetConvertDecimal());
  DisplayConversion(infoItemConv,"DOUBLE",             m_info->GetConvertDouble());
  DisplayConversion(infoItemConv,"FLOAT",              m_info->GetConvertFloat());
  DisplayConversion(infoItemConv,"INTEGER",            m_info->GetConvertInteger());
  DisplayConversion(infoItemConv,"INTERVAL YEAR-MONTH",m_info->GetConvertIYM());
  DisplayConversion(infoItemConv,"INTERVAL DAY-TIME",  m_info->GetConvertIDT());
  DisplayConversion(infoItemConv,"LONGVARBINARY",      m_info->GetConvertLVarBinary());
  DisplayConversion(infoItemConv,"LONGVARCHAR",        m_info->GetConvertLVarchar());
  DisplayConversion(infoItemConv,"NUMERIC",            m_info->GetConvertNumeric());
  DisplayConversion(infoItemConv,"REAL",               m_info->GetConvertReal());
  DisplayConversion(infoItemConv,"SMALLINT",           m_info->GetConvertSmallint());
  DisplayConversion(infoItemConv,"TIME",               m_info->GetConvertTime());
  DisplayConversion(infoItemConv,"TIMESTAMP",          m_info->GetConvertTimestamp());
  DisplayConversion(infoItemConv,"TINYINT",            m_info->GetConvertTinyint());
  DisplayConversion(infoItemConv,"VARBINARY",          m_info->GetConvertVarBinary());
  DisplayConversion(infoItemConv,"VARCHAR",            m_info->GetConvertVarchar());

  // SQL CONFORMANCE
  sitem = "SQL conformance: ";
  switch(m_info->GetSQLConformance())
  {
    case SQL_SC_SQL92_ENTRY:             sitem += "Entry level SQL92 compliant";              break;
    case SQL_SC_FIPS127_2_TRANSITIONAL : sitem += "FIPS 127-2 transitional level compliant";  break;
    case SQL_SC_SQL92_FULL :             sitem += "Full SQL92 compliant";                     break;
    case SQL_SC_SQL92_INTERMEDIATE :     sitem += "Intermediate SQL92 compliant";             break;
    default:                             sitem += "No information";                           break;
  }
  InsertItem(sitem, infoItemCF);

  sitem = "ODBC conformance: ";
  switch(m_info->GetODBCConformance())
  {
    case SQL_OIC_CORE:   sitem += "CORE";           break;
    case SQL_OIC_LEVEL1: sitem += "LEVEL 1";        break;
    case SQL_OIC_LEVEL2: sitem += "LEVEL 2";        break;
    default:             sitem += "No information"; break;
  }
  InsertItem(sitem, infoItemCF);

  sitem = "CLI conformance: ";
  switch(m_info->GetCLIConformance())
  {
    case SQL_SCC_XOPEN_CLI_VERSION1:   sitem += "X/Open CLI version 1"; break;
    case SQL_SCC_ISO92_CLI:            sitem += "ISO 92 CLI";           break;
    default:                           sitem += "No information";       break;
  }
  InsertItem(sitem, infoItemCF);
  InsertItem(CString("X/Open CLI Year: ") + m_info->GetCLIYear(), infoItemCF);

  // SQL Support
  sitem = "Aggregate functions";
  HTREEITEM agg = InsertItem(sitem,infoItemSQLCF);
  SQLUINTEGER aggr_functions = m_info->GetAggregationFunctions();
  if(aggr_functions & SQL_AF_ALL)      InsertItem("All aggregates supported",agg);
  else                                 InsertItem("Just these aggregates are supported",agg);
  DisplayBitfield(agg,aggr_functions,SQL_AF_AVG,     "AVG");
  DisplayBitfield(agg,aggr_functions,SQL_AF_COUNT,   "COUNT");
  DisplayBitfield(agg,aggr_functions,SQL_AF_DISTINCT,"DISTINCT");
  DisplayBitfield(agg,aggr_functions,SQL_AF_MAX,     "MAX");
  DisplayBitfield(agg,aggr_functions,SQL_AF_MIN,     "MIN");
  DisplayBitfield(agg,aggr_functions,SQL_AF_SUM,     "SUM");

  sitem = "Numeric functions";
  HTREEITEM numfunc = InsertItem(sitem,infoItemSQLCF);
  SQLUINTEGER funcNumeric = m_info->GetNumericFunctions();
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_ABS,     "ABS");
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_ACOS,    "ACOS");
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_ASIN,    "ASIN");
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_ATAN,    "ATAN");
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_ATAN2,   "ATAN2");
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_CEILING, "CEILING");
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_COS,     "COS");
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_COT,     "COT");
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_DEGREES, "DEGREES");
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_EXP,     "EXP");
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_FLOOR,   "FLOOR");
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_LOG,     "LOG");
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_LOG10,   "LOG10");
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_MOD,     "MOD");
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_PI,      "PI");
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_POWER,   "POWER");
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_RADIANS, "RADIANS");
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_RAND,    "RAND");
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_ROUND,   "ROUND");
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_SIGN,    "SIGN");
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_SIN,     "SIN");
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_SQRT,    "SQRT");
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_TAN,     "TAN");
  DisplayBitfield(numfunc,funcNumeric,SQL_FN_NUM_TRUNCATE,"TRUNCATE");

  sitem = "String functions";
  HTREEITEM sfunc = InsertItem(sitem,infoItemSQLCF);
  SQLUINTEGER funcString  = m_info->GetStringFunctions();
  SQLUINTEGER funcString3 = m_info->GetStringFunctions3();
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_ASCII,        "ASCII");
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_BIT_LENGTH,   "BIT_LENGTH");
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_CHAR,         "CHAR");
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_CHAR_LENGTH,  "CHAR_LENGTH");
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_CHARACTER_LENGTH,"CHARACTER_LENGTH");
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_CONCAT,       "CONCAT");
  DisplayBitfield(sfunc,funcString3,SQL_SSF_CONVERT,         "CONVERT");
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_DIFFERENCE,   "DIFFERENCE");
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_INSERT,       "INSERT");
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_LCASE,        "LCASE");
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_LEFT,         "LEFT");
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_LOCATE,       "LOCATE");
  DisplayBitfield(sfunc,funcString3,SQL_SSF_LOWER,           "LOWER");
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_LTRIM,        "LTRIM");
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_OCTET_LENGTH, "OCTET_LENGTH");
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_POSITION,     "POSITION");
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_REPEAT,       "REPEAT");
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_REPLACE,      "REPLACE");
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_RIGHT,        "RIGHT");
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_RTRIM,        "RTRIM");
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_SOUNDEX,      "SOUNDEX");
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_SPACE,        "SPACE");
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_SUBSTRING,    "SUBSTRING");
  DisplayBitfield(sfunc,funcString3,SQL_SSF_SUBSTRING,       "SQL92 SUBSTRING extensions");
  DisplayBitfield(sfunc,funcString3,SQL_SSF_TRANSLATE,       "TRANSLATE");
  DisplayBitfield(sfunc,funcString3,SQL_SSF_TRIM_BOTH,       "TRIM BOTH");
  DisplayBitfield(sfunc,funcString3,SQL_SSF_TRIM_LEADING,    "TRIM LEADING");
  DisplayBitfield(sfunc,funcString3,SQL_SSF_TRIM_TRAILING,   "TRIM TRAILING");
  DisplayBitfield(sfunc,funcString, SQL_FN_STR_UCASE,        "UCASE");
  DisplayBitfield(sfunc,funcString3,SQL_SSF_UPPER,           "UPPER");

  sitem = "Date/Time functions";
  HTREEITEM dtfunc = InsertItem(sitem,infoItemSQLCF);
  SQLUINTEGER funcDateTime = m_info->GetDateTimeFunctions();
  DisplayBitfield(dtfunc,funcDateTime,SQL_FN_TD_CURRENT_DATE     ,"CURRENT_DATE");
  DisplayBitfield(dtfunc,funcDateTime,SQL_FN_TD_CURRENT_TIME     ,"CURRENT_TIME");
  DisplayBitfield(dtfunc,funcDateTime,SQL_FN_TD_CURRENT_TIMESTAMP,"CURRENT_TIMESTAMP");
  DisplayBitfield(dtfunc,funcDateTime,SQL_FN_TD_CURDATE          ,"CURDATE");
  DisplayBitfield(dtfunc,funcDateTime,SQL_FN_TD_CURTIME          ,"CURTIME");
  DisplayBitfield(dtfunc,funcDateTime,SQL_FN_TD_DAYNAME          ,"DAYNAME");
  DisplayBitfield(dtfunc,funcDateTime,SQL_FN_TD_DAYOFMONTH       ,"DAYOFMONTH");
  DisplayBitfield(dtfunc,funcDateTime,SQL_FN_TD_DAYOFWEEK        ,"DAYOFWEEK");
  DisplayBitfield(dtfunc,funcDateTime,SQL_FN_TD_DAYOFYEAR        ,"DAYOFYEAR");
  DisplayBitfield(dtfunc,funcDateTime,SQL_FN_TD_EXTRACT          ,"EXTRACT");
  DisplayBitfield(dtfunc,funcDateTime,SQL_FN_TD_HOUR             ,"HOUR");
  DisplayBitfield(dtfunc,funcDateTime,SQL_FN_TD_MINUTE           ,"MINUTE");
  DisplayBitfield(dtfunc,funcDateTime,SQL_FN_TD_MONTH            ,"MONTH");
  DisplayBitfield(dtfunc,funcDateTime,SQL_FN_TD_MONTHNAME        ,"MONTHNAME");
  DisplayBitfield(dtfunc,funcDateTime,SQL_FN_TD_NOW              ,"NOW");
  DisplayBitfield(dtfunc,funcDateTime,SQL_FN_TD_QUARTER          ,"QUARTER");
  DisplayBitfield(dtfunc,funcDateTime,SQL_FN_TD_SECOND           ,"SECOND");
  HTREEITEM stampadd = DisplayBitfield(dtfunc,funcDateTime,SQL_FN_TD_TIMESTAMPADD ,"TIMESTAMPADD");
  HTREEITEM stampdif = DisplayBitfield(dtfunc,funcDateTime,SQL_FN_TD_TIMESTAMPDIFF,"TIMESTAMPDIFF");
  DisplayBitfield(dtfunc,funcDateTime,SQL_FN_TD_WEEK             ,"WEEK");
  DisplayBitfield(dtfunc,funcDateTime,SQL_FN_TD_YEAR             ,"YEAR");

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
  DisplayBitfield(stampadd,funcTimestampadd,SQL_FN_TSI_FRAC_SECOND,"Fractions of a second");
  DisplayBitfield(stampadd,funcTimestampadd,SQL_FN_TSI_SECOND,     "SECOND");
  DisplayBitfield(stampadd,funcTimestampadd,SQL_FN_TSI_MINUTE,     "MINUTE");
  DisplayBitfield(stampadd,funcTimestampadd,SQL_FN_TSI_HOUR,       "HOUR");
  DisplayBitfield(stampadd,funcTimestampadd,SQL_FN_TSI_DAY,        "DAY");
  DisplayBitfield(stampadd,funcTimestampadd,SQL_FN_TSI_WEEK,       "WEEK");
  DisplayBitfield(stampadd,funcTimestampadd,SQL_FN_TSI_MONTH,      "MONTH");
  DisplayBitfield(stampadd,funcTimestampadd,SQL_FN_TSI_QUARTER,    "QUARTER");
  DisplayBitfield(stampadd,funcTimestampadd,SQL_FN_TSI_YEAR,       "YEAR");

  DisplayBitfield(stampdif,funcTimestampdiff,SQL_FN_TSI_FRAC_SECOND,"Fractions of a second");
  DisplayBitfield(stampdif,funcTimestampdiff,SQL_FN_TSI_SECOND,     "SECOND");
  DisplayBitfield(stampdif,funcTimestampdiff,SQL_FN_TSI_MINUTE,     "MINUTE");
  DisplayBitfield(stampdif,funcTimestampdiff,SQL_FN_TSI_HOUR,       "HOUR");
  DisplayBitfield(stampdif,funcTimestampdiff,SQL_FN_TSI_DAY,        "DAY");
  DisplayBitfield(stampdif,funcTimestampdiff,SQL_FN_TSI_WEEK,       "WEEK");
  DisplayBitfield(stampdif,funcTimestampdiff,SQL_FN_TSI_MONTH,      "MONTH");
  DisplayBitfield(stampdif,funcTimestampdiff,SQL_FN_TSI_QUARTER,    "QUARTER");
  DisplayBitfield(stampdif,funcTimestampdiff,SQL_FN_TSI_YEAR,       "YEAR");

  sitem = "System functions";
  HTREEITEM sysfunc = InsertItem(sitem,infoItemSQLCF);
  SQLUINTEGER funcSystem = m_info->GetSystemFunctions();
  DisplayBitfield(sysfunc,funcSystem,SQL_FN_SYS_DBNAME,  "DATABASE");
  DisplayBitfield(sysfunc,funcSystem,SQL_FN_SYS_IFNULL,  "IFNULL");
  DisplayBitfield(sysfunc,funcSystem,SQL_FN_SYS_USERNAME,"USER");

  sitem = "Alter domain";
  HTREEITEM adom = InsertItem(sitem,infoItemSQLCF);
  SQLUINTEGER alterDomain = m_info->GetAlterDomainFunctions();
  DisplayBitfield(adom,alterDomain,SQL_AD_ADD_DOMAIN_CONSTRAINT     ,        "Add domain constraint");
  DisplayBitfield(adom,alterDomain,SQL_AD_ADD_DOMAIN_DEFAULT        ,        "Add domain default");
  DisplayBitfield(adom,alterDomain,SQL_AD_CONSTRAINT_NAME_DEFINITION,        "Constraint name definition");
  DisplayBitfield(adom,alterDomain,SQL_AD_DROP_DOMAIN_CONSTRAINT    ,        "Drop domain constraint");
  DisplayBitfield(adom,alterDomain,SQL_AD_DROP_DOMAIN_DEFAULT       ,        "Drop domain default");
  DisplayBitfield(adom,alterDomain,SQL_AD_ADD_CONSTRAINT_DEFERRABLE ,        "Add constraint deferrable");
  DisplayBitfield(adom,alterDomain,SQL_AD_ADD_CONSTRAINT_NON_DEFERRABLE     ,"Add constraint non deferrable");
  DisplayBitfield(adom,alterDomain,SQL_AD_ADD_CONSTRAINT_INITIALLY_DEFERRED ,"Add constraint initially deferred");
  DisplayBitfield(adom,alterDomain,SQL_AD_ADD_CONSTRAINT_INITIALLY_IMMEDIATE,"Add constraint initially immediate");

  sitem = "Alter table";
  HTREEITEM alt = InsertItem(sitem,infoItemSQLCF);
  SQLUINTEGER alterTable = m_info->GetAlterTableFunctions();
  DisplayBitfield(alt,alterTable,SQL_AT_ADD_COLUMN,                    "Add column");
  DisplayBitfield(alt,alterTable,SQL_AT_DROP_COLUMN,                   "Drop column");
  DisplayBitfield(alt,alterTable,SQL_AT_ADD_CONSTRAINT,                "Add constraint");
  DisplayBitfield(alt,alterTable,SQL_AT_ADD_COLUMN_COLLATION,          "Add column with collation");
  DisplayBitfield(alt,alterTable,SQL_AT_ADD_COLUMN_DEFAULT,            "Add column with default");
  DisplayBitfield(alt,alterTable,SQL_AT_ADD_COLUMN_SINGLE,             "Add column (single)");
  DisplayBitfield(alt,alterTable,SQL_AT_ADD_CONSTRAINT,                "Add column constraint");
  DisplayBitfield(alt,alterTable,SQL_AT_ADD_TABLE_CONSTRAINT,          "Add table constraint");
  DisplayBitfield(alt,alterTable,SQL_AT_CONSTRAINT_NAME_DEFINITION,    "Add constraint name definition");
  DisplayBitfield(alt,alterTable,SQL_AT_DROP_COLUMN_CASCADE,           "Drop column cascade");
  DisplayBitfield(alt,alterTable,SQL_AT_DROP_COLUMN_DEFAULT,           "Drop column default");
  DisplayBitfield(alt,alterTable,SQL_AT_DROP_COLUMN_RESTRICT,          "Drop column restrict");
  DisplayBitfield(alt,alterTable,SQL_AT_DROP_TABLE_CONSTRAINT_CASCADE, "Drop table constraint cascade");
  DisplayBitfield(alt,alterTable,SQL_AT_DROP_TABLE_CONSTRAINT_RESTRICT,"Drop table constraint restrict");
  DisplayBitfield(alt,alterTable,SQL_AT_SET_COLUMN_DEFAULT,            "Set column default");
  DisplayBitfield(alt,alterTable,SQL_AT_CONSTRAINT_INITIALLY_DEFERRED, "Constraints are initially deferred");
  DisplayBitfield(alt,alterTable,SQL_AT_CONSTRAINT_INITIALLY_IMMEDIATE,"Constraints are initially immediate");
  DisplayBitfield(alt,alterTable,SQL_AT_CONSTRAINT_DEFERRABLE,         "Constraints are deferrable");
  DisplayBitfield(alt,alterTable,SQL_AT_CONSTRAINT_NON_DEFERRABLE,     "Constraints are not deferrable");

  sitem = "Create schema";
  HTREEITEM crsch = InsertItem(sitem,infoItemSQLCF);
  SQLUINTEGER createSchema = m_info->GetCreateSchemaFunctions();
  DisplayBitfield(crsch,createSchema,SQL_CS_CREATE_SCHEMA,        "Create schema");
  DisplayBitfield(crsch,createSchema,SQL_CS_AUTHORIZATION,        "Authorization clause for owner");
  DisplayBitfield(crsch,createSchema,SQL_CS_DEFAULT_CHARACTER_SET,"Create default character set");

  sitem = "Create domain";
  HTREEITEM cdom = InsertItem(sitem,infoItemSQLCF);
  SQLUINTEGER createDomain = m_info->GetCreateDomainFunctions();
  DisplayBitfield(cdom,createDomain,SQL_CDO_CREATE_DOMAIN,                  "Create domain");
  DisplayBitfield(cdom,createDomain,SQL_CDO_DEFAULT,                        "Default value");
  DisplayBitfield(cdom,createDomain,SQL_CDO_CONSTRAINT,                     "Constraint definition");
  DisplayBitfield(cdom,createDomain,SQL_CDO_CONSTRAINT_NAME_DEFINITION,     "Constraint name definition");
  DisplayBitfield(cdom,createDomain,SQL_CDO_COLLATION,                      "Collation definition");
  DisplayBitfield(cdom,createDomain,SQL_CDO_CONSTRAINT_INITIALLY_DEFERRED,  "Constraint are initially deferred");
  DisplayBitfield(cdom,createDomain,SQL_CDO_CONSTRAINT_INITIALLY_IMMEDIATE, "Constraint are initially immediate");
  DisplayBitfield(cdom,createDomain,SQL_CDO_CONSTRAINT_NON_DEFERRABLE,      "Constraint are not deferrable");

  sitem = "Create table";
  HTREEITEM crea = InsertItem(sitem,infoItemSQLCF);
  SQLUINTEGER createTable = m_info->GetCreateTableFunctions();
  DisplayBitfield(crea,createTable,SQL_CT_CREATE_TABLE,                  "Create table");
  DisplayBitfield(crea,createTable,SQL_CT_TABLE_CONSTRAINT,              "Create table constraint");
  DisplayBitfield(crea,createTable,SQL_CT_CONSTRAINT_NAME_DEFINITION,    "Constraint name definition");
  DisplayBitfield(crea,createTable,SQL_CT_COMMIT_PRESERVE,               "Temporary tables: Deleted rows preserved on commit");
  DisplayBitfield(crea,createTable,SQL_CT_COMMIT_DELETE,                 "Temporary tables: Deleted rows are deleted on commit");
  DisplayBitfield(crea,createTable,SQL_CT_GLOBAL_TEMPORARY,              "Global temporary tables can be created");
  DisplayBitfield(crea,createTable,SQL_CT_LOCAL_TEMPORARY,               "Local temporary tables can be created");
  DisplayBitfield(crea,createTable,SQL_CT_COLUMN_CONSTRAINT,             "Column constraint definition");
  DisplayBitfield(crea,createTable,SQL_CT_COLUMN_DEFAULT,                "Column default definition");
  DisplayBitfield(crea,createTable,SQL_CT_COLUMN_COLLATION,              "Column collation definition");
  DisplayBitfield(crea,createTable,SQL_CT_CONSTRAINT_INITIALLY_DEFERRED, "Constraints are initially deferred");
  DisplayBitfield(crea,createTable,SQL_CT_CONSTRAINT_INITIALLY_IMMEDIATE,"Constraints are initially immediate");
  DisplayBitfield(crea,createTable,SQL_CT_CONSTRAINT_DEFERRABLE,         "Constraints are deferrable");
  DisplayBitfield(crea,createTable,SQL_CT_CONSTRAINT_NON_DEFERRABLE,     "Constraints are not deferrable");

  sitem = "Create view";
  HTREEITEM crvw = InsertItem(sitem,infoItemSQLCF);
  SQLUINTEGER createView = m_info->GetCreateViewFunctions();
  DisplayBitfield(crvw,createView,SQL_CV_CREATE_VIEW, "Create view");
  DisplayBitfield(crvw,createView,SQL_CV_CHECK_OPTION,"Create view with check option");
  DisplayBitfield(crvw,createView,SQL_CV_CASCADED,    "Cascaded views");
  DisplayBitfield(crvw,createView,SQL_CV_LOCAL,       "Local views");

  sitem = "Delete";
  HTREEITEM fkdel = InsertItem(sitem,infoItemSQLCF);
  SQLUINTEGER deleteRules = m_info->GetDeleteRuleFunctions();
  DisplayBitfield(fkdel,deleteRules,SQL_SFKD_CASCADE,    "Set foreign keys: CASCADE");
  DisplayBitfield(fkdel,deleteRules,SQL_SFKD_NO_ACTION,  "Set foreign keys: NO ACTION");
  DisplayBitfield(fkdel,deleteRules,SQL_SFKD_SET_DEFAULT,"Set foreign keys: DEFAULT");
  DisplayBitfield(fkdel,deleteRules,SQL_SFKD_SET_NULL,   "Set foreign keys: NULL");

  sitem = "Drop domain";
  HTREEITEM ddom = InsertItem(sitem,infoItemSQLCF);
  SQLUINTEGER dropDomain = m_info->GetDropDomainFunctions();
  DisplayBitfield(ddom,dropDomain,SQL_DD_DROP_DOMAIN,"Drop domain");
  DisplayBitfield(ddom,dropDomain,SQL_DD_CASCADE,    "Drop domain cascade");
  DisplayBitfield(ddom,dropDomain,SQL_DD_RESTRICT,   "Drop domain restrict");

  sitem = "Drop schema";
  HTREEITEM dsch = InsertItem(sitem,infoItemSQLCF);
  SQLUINTEGER dropSchema = m_info->GetDropSchemaFunctions();
  DisplayBitfield(dsch,dropSchema,SQL_DS_DROP_SCHEMA,"Drop schema");
  DisplayBitfield(dsch,dropSchema,SQL_DS_CASCADE,    "Drop schema cascade");
  DisplayBitfield(dsch,dropSchema,SQL_DS_RESTRICT,   "Drop schema restrict");

  sitem = "Drop table";
  HTREEITEM dtab = InsertItem(sitem,infoItemSQLCF);
  SQLUINTEGER dropTable = m_info->GetDropTableFunctions();
  DisplayBitfield(dtab,dropTable,SQL_DT_DROP_TABLE, "Drop table");
  DisplayBitfield(dtab,dropTable,SQL_DT_CASCADE,    "Drop table cascade");
  DisplayBitfield(dtab,dropTable,SQL_DT_RESTRICT,   "Drop table restrict");

  sitem = "Drop view";
  HTREEITEM dvw = InsertItem(sitem,infoItemSQLCF);
  SQLUINTEGER dropView = m_info->GetDropViewFunctions();
  DisplayBitfield(dvw,dropView,SQL_DV_DROP_VIEW,   "Drop view");
  DisplayBitfield(dvw,dropView,SQL_DV_CASCADE,     "Drop view cascade");
  DisplayBitfield(dvw,dropView,SQL_DV_RESTRICT,    "Drop view restrict");

  sitem = "Grant";
  HTREEITEM grant = InsertItem(sitem,infoItemSQLCF);
  SQLUINTEGER grantOptions = m_info->GetGrantOptions();
  DisplayBitfield(grant,grantOptions,SQL_SG_INSERT_TABLE,          "Table: Insert");
  DisplayBitfield(grant,grantOptions,SQL_SG_SELECT_TABLE,          "Table: Select");
  DisplayBitfield(grant,grantOptions,SQL_SG_UPDATE_TABLE,          "Table: Update");
  DisplayBitfield(grant,grantOptions,SQL_SG_DELETE_TABLE,          "Table: Delete");
  DisplayBitfield(grant,grantOptions,SQL_SG_REFERENCES_TABLE,      "Table: References");
  DisplayBitfield(grant,grantOptions,SQL_SG_INSERT_COLUMN,         "Column: Insert");
  DisplayBitfield(grant,grantOptions,SQL_SG_UPDATE_COLUMN,         "Column: Update");
  DisplayBitfield(grant,grantOptions,SQL_SG_REFERENCES_COLUMN,     "Column: References");
  DisplayBitfield(grant,grantOptions,SQL_SG_WITH_GRANT_OPTION,     "WITH GRANT OPTION");
  DisplayBitfield(grant,grantOptions,SQL_SG_USAGE_ON_DOMAIN,       "Usage: Domain");
  DisplayBitfield(grant,grantOptions,SQL_SG_USAGE_ON_CHARACTER_SET,"Usage: Character set");
  DisplayBitfield(grant,grantOptions,SQL_SG_USAGE_ON_COLLATION,    "Usage: Collation");
  DisplayBitfield(grant,grantOptions,SQL_SG_USAGE_ON_TRANSLATION,  "Usage: Translation");

  sitem = "Index";
  HTREEITEM indx = InsertItem(sitem,infoItemSQLCF);
  SQLUINTEGER indexOptions1 = m_info->GetIndexOptions1();
  SQLUINTEGER indexOptions2 = m_info->GetIndexOptions2();
  SQLSMALLINT maxColIndex   = m_info->GetMaxColumnsInIndex();
  DisplayBitfield(indx,indexOptions1,SQL_DI_CREATE_INDEX,"Create index");
  DisplayBitfield(indx,indexOptions1,SQL_DI_DROP_INDEX,  "Drop index");
  DisplayBitfield(indx,indexOptions2,SQL_IK_ASC,         "ASC ascending");
  DisplayBitfield(indx,indexOptions2,SQL_IK_DESC,        "DESC descending");
  sitem.Format("Maximum columns in an index: %s",InfoNumberToString(maxColIndex));
  InsertItem(sitem,indx);

  sitem = "Insert";
  HTREEITEM ins = InsertItem(sitem,infoItemSQLCF);
  SQLUINTEGER insertOptions = m_info->GetInsertOptions();
  DisplayBitfield(ins,insertOptions,SQL_IS_INSERT_LITERALS,"VALUES");
  DisplayBitfield(ins,insertOptions,SQL_IS_INSERT_SEARCHED,"INSERT-SELECT");
  DisplayBitfield(ins,insertOptions,SQL_IS_SELECT_INTO,    "SELECT-INTO");

  sitem = "Revoke";
  HTREEITEM rev = InsertItem(sitem,infoItemSQLCF);
  SQLUINTEGER revokeOptions = m_info->GetRevokeOptions();
  DisplayBitfield(rev,revokeOptions,SQL_SR_INSERT_TABLE,          "Table: Insert");
  DisplayBitfield(rev,revokeOptions,SQL_SR_SELECT_TABLE,          "Table: Select");
  DisplayBitfield(rev,revokeOptions,SQL_SR_UPDATE_TABLE,          "Table: Update");
  DisplayBitfield(rev,revokeOptions,SQL_SR_DELETE_TABLE,          "Table: Delete");
  DisplayBitfield(rev,revokeOptions,SQL_SR_REFERENCES_TABLE,      "Table: References");
  DisplayBitfield(rev,revokeOptions,SQL_SR_INSERT_COLUMN,         "Column: Insert");
  DisplayBitfield(rev,revokeOptions,SQL_SR_UPDATE_COLUMN,         "Column: Update");
  DisplayBitfield(rev,revokeOptions,SQL_SR_REFERENCES_COLUMN,     "Column: References");
  DisplayBitfield(rev,revokeOptions,SQL_SR_CASCADE,               "Cascade");
  DisplayBitfield(rev,revokeOptions,SQL_SR_RESTRICT,              "Restrict");
  DisplayBitfield(rev,revokeOptions,SQL_SR_GRANT_OPTION_FOR,      "GRANT OPTION FOR");
  DisplayBitfield(rev,revokeOptions,SQL_SR_USAGE_ON_DOMAIN,       "Usage: Domain");
  DisplayBitfield(rev,revokeOptions,SQL_SR_USAGE_ON_CHARACTER_SET,"Usage: Character set");
  DisplayBitfield(rev,revokeOptions,SQL_SR_USAGE_ON_COLLATION,    "Usage: Collation");
  DisplayBitfield(rev,revokeOptions,SQL_SR_USAGE_ON_TRANSLATION,  "Usage: Translation");

  // The select statement
  sitem = "Select";
  HTREEITEM select = InsertItem(sitem,infoItemSQLCF);

  sitem = "Selected columns";
  HTREEITEM scols = InsertItem(sitem,select);

  sitem.Format("Maximum columns in a select: %s",InfoNumberToString(m_info->GetMaxColumnsInSelect()));
  InsertItem(sitem, scols);

  sitem.Format("Column aliases in select allowed: %s",m_info->GetColumnAliasAllowed() ? "Yes":"No");
  InsertItem(sitem, scols);

  sitem = "Row value constructors";
  HTREEITEM rowval = InsertItem(sitem,scols);
  SQLUINTEGER rowConstructors = m_info->GetRowConstructorOptions();
  DisplayBitfield(rowval,rowConstructors,SQL_SRVC_NULL,            "NULL");
  DisplayBitfield(rowval,rowConstructors,SQL_SRVC_DEFAULT,         "DEFAULT");
  DisplayBitfield(rowval,rowConstructors,SQL_SRVC_ROW_SUBQUERY,    "Row subquery");
  DisplayBitfield(rowval,rowConstructors,SQL_SRVC_VALUE_EXPRESSION,"Value expression");

  sitem = "Value expressions";
  HTREEITEM valex = InsertItem(sitem,scols);
  SQLUINTEGER valExpressions = m_info->GetValueExpressionOptions();
  DisplayBitfield(valex,valExpressions,SQL_SVE_CASE,    "CASE");
  DisplayBitfield(valex,valExpressions,SQL_SVE_CAST,    "CAST");
  DisplayBitfield(valex,valExpressions,SQL_SVE_COALESCE,"COALESCE");
  DisplayBitfield(valex,valExpressions,SQL_SVE_NULLIF,  "NULLIF");

  // FROM clause in the SELECT
  sitem = "FROM options";
  HTREEITEM fromopt = InsertItem(sitem,select);

  sitem = "Table aliases correlation: ";
  switch(m_info->GetTableCorrelationOptions())
  {
    case SQL_CN_NONE:     sitem += "Table aliases are not supported";     
                          break;
    case SQL_CN_DIFFERENT:sitem += "Aliases must differ from tablenames"; 
                          break;
    case SQL_CN_ANY:      sitem += "Any table alias can be used";         
                          break;
  }
  InsertItem(sitem,fromopt);

  sitem.Format("Maximum tables in FROM: %s",InfoNumberToString(m_info->GetMaxTablesInSelect()));
  InsertItem(sitem,fromopt);

  HTREEITEM oj = InsertItem("Outer join support",fromopt);
  SQLUINTEGER oj_cap   = m_info->GetOuterJoinOptions();
  SQLUINTEGER oj_cap92 = m_info->GetOuterJoin1992Options();
  DisplayBitfield(oj,oj_cap,  SQL_OJ_LEFT,                  "JOIN: Left outer joins");
  DisplayBitfield(oj,oj_cap,  SQL_OJ_RIGHT,                 "JOIN: Right outer joins");
  DisplayBitfield(oj,oj_cap,  SQL_OJ_FULL,                  "JOIN: Full outer joins");
  DisplayBitfield(oj,oj_cap,  SQL_OJ_NESTED,                "JOIN: Nested outer joins");
  DisplayBitfield(oj,oj_cap,  SQL_OJ_NOT_ORDERED,           "OPTION: Join columns not ordered by tablenames");
  DisplayBitfield(oj,oj_cap,  SQL_OJ_INNER,                 "OPTION: Inner table can be used in inner join");
  DisplayBitfield(oj,oj_cap,  SQL_OJ_ALL_COMPARISON_OPS,    "OPTION: Operators other than '='");
  DisplayBitfield(oj,oj_cap92,SQL_SRJO_INNER_JOIN,          "92: INNER JOIN");
  DisplayBitfield(oj,oj_cap92,SQL_SRJO_NATURAL_JOIN,        "92: NATURAL JOIN");
  DisplayBitfield(oj,oj_cap92,SQL_SRJO_LEFT_OUTER_JOIN,     "92: LEFT OUTER JOIN");
  DisplayBitfield(oj,oj_cap92,SQL_SRJO_RIGHT_OUTER_JOIN,    "92: RIGHT OUTER JOIN");
  DisplayBitfield(oj,oj_cap92,SQL_SRJO_FULL_OUTER_JOIN,     "92: FULL OUTER JOIN");
  DisplayBitfield(oj,oj_cap92,SQL_SRJO_CROSS_JOIN,          "92: CROSS JOIN");
  DisplayBitfield(oj,oj_cap92,SQL_SRJO_EXCEPT_JOIN,         "92: EXCEPT JOIN");
  DisplayBitfield(oj,oj_cap92,SQL_SRJO_INTERSECT_JOIN,      "92: INTERSECT JOIN");
  DisplayBitfield(oj,oj_cap92,SQL_SRJO_UNION_JOIN,          "92: UNION JOIN");
  DisplayBitfield(oj,oj_cap92,SQL_SRJO_CORRESPONDING_CLAUSE,"92: CORRESPONDING CLAUSE");

  sitem = "WHERE options";
  HTREEITEM where = InsertItem(sitem,select);

  sitem = "WHERE predicates";
  HTREEITEM pred = InsertItem(sitem,where);
  SQLUINTEGER predicates = m_info->GetPredicateOptions();
  DisplayBitfield(pred,predicates,SQL_SP_BETWEEN,              "BETWEEN");
  DisplayBitfield(pred,predicates,SQL_SP_COMPARISON,           "=,<>,<,>,<=,>=");
  DisplayBitfield(pred,predicates,SQL_SP_EXISTS,               "EXISTS");
  DisplayBitfield(pred,predicates,SQL_SP_IN,                   "IN");
  DisplayBitfield(pred,predicates,SQL_SP_ISNOTNULL,            "IS NOT NULL");
  DisplayBitfield(pred,predicates,SQL_SP_ISNULL,               "IS NULL");
  DisplayBitfield(pred,predicates,SQL_SP_LIKE,                 "LIKE");
  DisplayBitfield(pred,predicates,SQL_SP_MATCH_FULL,           "MATCH (full)");
  DisplayBitfield(pred,predicates,SQL_SP_MATCH_PARTIAL,        "MATCH (partial)");
  DisplayBitfield(pred,predicates,SQL_SP_MATCH_UNIQUE_FULL,    "MATCH UNIQUE (full)");
  DisplayBitfield(pred,predicates,SQL_SP_MATCH_UNIQUE_PARTIAL, "MATCH UNIQUE (partial)");
  DisplayBitfield(pred,predicates,SQL_SP_OVERLAPS,             "OVERLAPS");
  DisplayBitfield(pred,predicates,SQL_SP_QUANTIFIED_COMPARISON,"=,<>,<,>,<=,>= (quantified)");
  DisplayBitfield(pred,predicates,SQL_SP_UNIQUE,               "UNIQUE");

  sitem = "Subqueries";
  HTREEITEM subq = InsertItem(sitem,where);
  SQLUINTEGER predSubqueries = m_info->GetPredicateSubqueryOptions();
  DisplayBitfield(subq,predSubqueries,SQL_SQ_CORRELATED_SUBQUERIES,"Correlated subqueries");
  DisplayBitfield(subq,predSubqueries,SQL_SQ_COMPARISON,           "Comparison");
  DisplayBitfield(subq,predSubqueries,SQL_SQ_EXISTS,               "EXISTS");
  DisplayBitfield(subq,predSubqueries,SQL_SQ_IN,                   "IN");
  DisplayBitfield(subq,predSubqueries,SQL_SQ_QUANTIFIED,           "Quantified");

  sitem = "GROUP BY options";
  HTREEITEM gb = InsertItem(sitem,select);
  SQLUINTEGER groupBy = m_info->GetGroupByOptions();
  if(groupBy & SQL_GB_NOT_SUPPORTED)
  {
    InsertItem("GROUP BY not supported",gb);
  }
  else
  {
    InsertItem("GROUP BY supported",gb);
    sitem.Format("Maximum columns in 'group by': %s",InfoNumberToString(m_info->GetMaxColumnsInGroupBy()));
    InsertItem(sitem,gb);
    DisplayBitfield(gb,groupBy,SQL_GB_GROUP_BY_EQUALS_SELECT,  "All non-aggregated columns");
    DisplayBitfield(gb,groupBy,SQL_GB_GROUP_BY_CONTAINS_SELECT,"Non aggregated columns + extra");
    DisplayBitfield(gb,groupBy,SQL_GB_NO_RELATION,             "Columns not related to select");
    DisplayBitfield(gb,groupBy,SQL_GB_COLLATE,                 "COLLATE clause at end");
  }

  sitem = "ORDER BY options";
  HTREEITEM orderby = InsertItem(sitem,select);
  sitem.Format("Order by in select: %s",m_info->GetOrderByInSelectAllowed() ? "Yes" : "No");
  InsertItem(sitem, orderby);
  sitem.Format("Expressions in the 'order by': %s",m_info->GetExpressionsInOrderByAllowed() ? "Yes" : "No");
  InsertItem(sitem, orderby);
  sitem.Format("Maximum columns in 'order by': %s",InfoNumberToString(m_info->GetMaxColumnsInOrderBy()));
  InsertItem(sitem,orderby);

  sitem = "UNION options";
  HTREEITEM un = InsertItem(sitem,select);
  SQLUINTEGER unions = m_info->GetUnionOptions();
  DisplayBitfield(un,unions,SQL_U_UNION,    "UNION");
  DisplayBitfield(un,unions,SQL_U_UNION_ALL,"UNION ALL");

  sitem = "Update";
  HTREEITEM update = InsertItem(sitem,infoItemSQLCF);
  SQLUINTEGER updateRules = m_info->GetUpdateOptions();
  DisplayBitfield(update,updateRules,SQL_SFKU_CASCADE,    "Set foreign keys: CASCADE");
  DisplayBitfield(update,updateRules,SQL_SFKU_NO_ACTION,  "Set foreign keys: NO ACTION");
  DisplayBitfield(update,updateRules,SQL_SFKU_SET_DEFAULT,"Set foreign keys: DEFAULT");
  DisplayBitfield(update,updateRules,SQL_SFKU_SET_NULL,   "Set foreign keys: NULL");

  // RDBMS Implementation
  sitem = "RDBMS storage: ";
  switch(m_info->GetDatabaseFileUsage())
  {
    case SQL_FILE_TABLE:         sitem += "1 file = 1 table";    break;
    case SQL_FILE_CATALOG:       sitem += "1 file = catalog";    break;
    case SQL_FILE_NOT_SUPPORTED: sitem += "Multi-tier database"; break;
    default:                     sitem += "(No information)";    break;
  }
  InsertItem(sitem,infoItemImpl);

  sitem  = "RDBMS catalog support: ";
  sitem += m_info->GetSupportsCatalogs() ? "Yes" : "No";
  InsertItem(sitem,infoItemImpl);
  InsertItem("Term for a catalog: "   + m_info->GetCatalogTerm(),   infoItemImpl);
  InsertItem("Term for a schema: "    + m_info->GetSchemaTerm(),    infoItemImpl);
  InsertItem("Term for a table: "     + m_info->GetTableTerm(),     infoItemImpl);
  InsertItem("Term for a procedure: " + m_info->GetProcedureTerm(), infoItemImpl);
  CString line("Catalog identifiers: ");
  switch(m_info->GetIdentifierCase())
  {
    case SQL_IC_UPPER:     line += "Upper-case";    break;
    case SQL_IC_LOWER:     line += "Lower-case";    break;
    case SQL_IC_SENSITIVE: line += "Sensitive (?)"; break;
    case SQL_IC_MIXED:     line += "Mixed-case";    break;
    default:               line += "No information";break;
  }
  InsertItem(line,infoItemImpl);
  sitem = "Catalog name location: ";
  SQLSMALLINT catalogLocation = m_info->GetCatalogLocation();
  sitem += (catalogLocation == SQL_CL_START) ? "Before schema" : 
           (catalogLocation == SQL_CL_END)   ? "After table"   : 
           "(Unknown)";
  InsertItem(sitem,infoItemImpl);
  InsertItem("Catalog name separator: " + m_info->GetCatalogNameSeparator(),infoItemImpl);
  HTREEITEM cu = InsertItem("Catalog name usage",infoItemImpl);
  SQLUINTEGER catalogUsage = m_info->GetCatalogNameUsage();
  DisplayBitfield(cu,catalogUsage,SQL_CU_DML_STATEMENTS,      "DML Statements");
  DisplayBitfield(cu,catalogUsage,SQL_CU_PROCEDURE_INVOCATION,"Procedure invocation");
  DisplayBitfield(cu,catalogUsage,SQL_CU_TABLE_DEFINITION,    "Table definition");
  DisplayBitfield(cu,catalogUsage,SQL_CU_INDEX_DEFINITION,    "Index definition");
  DisplayBitfield(cu,catalogUsage,SQL_CU_PRIVILEGE_DEFINITION,"Privilege definition");

  sitem.Format("Maximum length of catalogname: %i",m_info->GetMaxCatalogNameLength());
  InsertItem(sitem, infoItemImpl);
  sitem.Format("Maximum length of schemaname: %i", m_info->GetMaxSchemaNameLength());
  InsertItem(sitem, infoItemImpl);
  sitem.Format("Maximum length of tablename: %i",  m_info->GetMaxTableNameLength());
  InsertItem(sitem, infoItemImpl);
  sitem.Format("Maximum length of columnname: %i", m_info->GetMaxColumnNameLength());
  InsertItem(sitem, infoItemImpl);
  sitem.Format("Maximum number of columns in a table: %i", m_info->GetMaxColumnsInTable());
  InsertItem(sitem, infoItemImpl);

  InsertItem("Special characters: " + m_info->GetSpecialCharacters(),        infoItemImpl);
  InsertItem("Like escape char: "   + m_info->GetLikeEscapeCharacter(),      infoItemImpl);
  InsertItem("Literal quote: "      + m_info->GetIdentifierQuoteCharacter(), infoItemImpl);

  sitem = "Collation sequence: ";
  sitem += m_info->GetCollationSequence();
  InsertItem(sitem,infoItemImpl);

  sitem = "NULL Collation: ";
  switch(m_info->GetNULLCollation())
  {
    case SQL_NC_HIGH:  sitem += "High end of resultset, depending on ASC/DESC";         break;
    case SQL_NC_LOW:   sitem += "Low end of resultset, depending on ASC/DESC";          break;
    case SQL_NC_START: sitem += "At the start of the resultset, regardless of ASC/DESC";break;
    case SQL_NC_END:   sitem += "At the end of the resultset, regardless of ASC/DESC";  break;
    default:           sitem += "(No information)";                                     break;
  }
  InsertItem(sitem,infoItemImpl);

  sitem = "Concat NULL behaviour: ";
  switch(m_info->GetNULLConcatBehaviour())
  {
    case SQL_CB_NULL:     sitem += "Results in NULL";     break;
    case SQL_CB_NON_NULL: sitem += "Results in NON-NULL"; break;
    default:              sitem += "(No information)";    break;
  }
  InsertItem(sitem,infoItemImpl);

  sitem = "Nullable columns: ";
  switch(m_info->GetColumnsCanBeNULL())
  {
    case SQL_NNC_NULL:     sitem += "All columns are nullable"; break;
    case SQL_NNC_NON_NULL: sitem += "Columns can be non-null";  break;
    default:               sitem += "(No information)";         break;
  }
  InsertItem(sitem,infoItemImpl);

  HTREEITEM dtl = InsertItem("Date/Time/Interval literals",infoItemImpl);
  SQLUINTEGER datetimeLiterals = m_info->GetSupportedDateTimeLiterals();
  DisplayBitfield(dtl,datetimeLiterals,SQL_DL_SQL92_DATE                     ,"Date");
  DisplayBitfield(dtl,datetimeLiterals,SQL_DL_SQL92_TIME                     ,"Time");
  DisplayBitfield(dtl,datetimeLiterals,SQL_DL_SQL92_TIMESTAMP                ,"Timestamp");
  DisplayBitfield(dtl,datetimeLiterals,SQL_DL_SQL92_INTERVAL_YEAR            ,"Interval year");
  DisplayBitfield(dtl,datetimeLiterals,SQL_DL_SQL92_INTERVAL_MONTH           ,"Interval month");
  DisplayBitfield(dtl,datetimeLiterals,SQL_DL_SQL92_INTERVAL_DAY             ,"Interval day");
  DisplayBitfield(dtl,datetimeLiterals,SQL_DL_SQL92_INTERVAL_HOUR            ,"Interval hour");
  DisplayBitfield(dtl,datetimeLiterals,SQL_DL_SQL92_INTERVAL_MINUTE          ,"Interval minute");
  DisplayBitfield(dtl,datetimeLiterals,SQL_DL_SQL92_INTERVAL_SECOND          ,"Interval second");
  DisplayBitfield(dtl,datetimeLiterals,SQL_DL_SQL92_INTERVAL_YEAR_TO_MONTH   ,"Interval year to month");
  DisplayBitfield(dtl,datetimeLiterals,SQL_DL_SQL92_INTERVAL_DAY_TO_HOUR     ,"Interval day to hour");
  DisplayBitfield(dtl,datetimeLiterals,SQL_DL_SQL92_INTERVAL_DAY_TO_MINUTE   ,"Interval day to minute");
  DisplayBitfield(dtl,datetimeLiterals,SQL_DL_SQL92_INTERVAL_DAY_TO_SECOND   ,"Interval day to second");
  DisplayBitfield(dtl,datetimeLiterals,SQL_DL_SQL92_INTERVAL_HOUR_TO_MINUTE  ,"Interval hour to minute");
  DisplayBitfield(dtl,datetimeLiterals,SQL_DL_SQL92_INTERVAL_HOUR_TO_SECOND  ,"Interval hour to second");
  DisplayBitfield(dtl,datetimeLiterals,SQL_DL_SQL92_INTERVAL_MINUTE_TO_SECOND,"Interval minute to second");

  sitem = "Active environments: ";
  if(!m_info->GetMaxActiveEnvironments())
  {
    sitem += " (no limit or unknown)";
  }
  else 
  {
    sitem += InfoNumberToString(m_info->GetMaxActiveEnvironments());
  }
  InsertItem(sitem,infoItemImpl);

  sitem.Format("Transaction integrity: %s",m_info->GetTransactionIntegrity() ? "Yes" : "No");
  InsertItem(sitem, infoItemImpl);

  sitem = "Transactions behaviour: ";
  switch(m_info->GetTransactionCapabilities())
  {
    case SQL_TC_DML:        sitem += "Only DML";    break;
    case SQL_TC_DDL_COMMIT: sitem += "DDL Commits"; break;
    case SQL_TC_DDL_IGNORE: sitem += "DDL Ignores"; break;
    case SQL_TC_ALL:        sitem += "DDL and DML"; break;
    case SQL_TC_NONE:
    default:                sitem += "None";        break;
  }
  InsertItem(sitem, infoItemImpl);

  sitem = "Default transaction level: ";
  switch(m_info->GetDefaultTransactionLevel())
  {
    case SQL_TXN_READ_UNCOMMITTED: sitem += "Read uncommitted"; break;
    case SQL_TXN_READ_COMMITTED:   sitem += "Read committed";   break;
    case SQL_TXN_REPEATABLE_READ:  sitem += "Repeatable read";  break;
    case SQL_TXN_SERIALIZABLE:     sitem += "Serializable";     break;
    default:                       sitem += "(No information)"; break;
  }
  InsertItem(sitem, infoItemImpl);

  sitem = "Cursor commit behaviour: ";
  SQLSMALLINT cursor_commit = m_info->GetCursorCommitBehaviour();
  if (cursor_commit == SQL_CB_DELETE)      sitem += "Close cursor and remove statement";
  if (cursor_commit == SQL_CB_CLOSE)       sitem += "Close cursor";
  if (cursor_commit == SQL_CB_PRESERVE)    sitem += "Preserve cursor";
  InsertItem(sitem, infoItemImpl);

  sitem = "Cursor rollback behaviour: ";
  SQLSMALLINT cursor_rollback = m_info->GetCursorRollbackBehaviour();
  if (cursor_rollback == SQL_CB_DELETE)      sitem += "Close cursor and remove statement";
  if (cursor_rollback == SQL_CB_CLOSE)       sitem += "Close cursor";
  if (cursor_rollback == SQL_CB_PRESERVE)    sitem += "Preserve cursor";
  InsertItem(sitem, infoItemImpl);

  InsertItem(CString("Accessible tables: ")     + (m_info->GetAccessibleTables()     ? "Yes" : "No"), infoItemImpl);
  InsertItem(CString("Accessible procedures: ") + (m_info->GetAccessibleProcedures() ? "Yes" : "No"), infoItemImpl);

  // RDBMS SQL gereserveerde woorden
  HTREEITEM words = InsertItem("RDBMS SQL Reserved words",infoItemImpl);
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
    InsertItem("No info",words);
  }
  // RDBMS Supports INFORMATION_SCHEMA views
  HTREEITEM schviews = InsertItem("INFORMATION_SCHEMA views",infoItemImpl);
  SQLINTEGER schemaViews = m_info->GetInformationSchemaViews();
  DisplayBitfield(schviews,schemaViews,SQL_ISV_ASSERTIONS,             "ASSERTIONS");
  DisplayBitfield(schviews,schemaViews,SQL_ISV_CHARACTER_SETS,         "CHARACTER_SETS");
  DisplayBitfield(schviews,schemaViews,SQL_ISV_CHECK_CONSTRAINTS,      "CHECK_CONSTRAINTS");
  DisplayBitfield(schviews,schemaViews,SQL_ISV_COLLATIONS,             "COLLATIONS");
  DisplayBitfield(schviews,schemaViews,SQL_ISV_COLUMN_DOMAIN_USAGE,    "COLUMN_DOMAIN_USAGE");
  DisplayBitfield(schviews,schemaViews,SQL_ISV_COLUMN_PRIVILEGES,      "COLUMN_PRIVILEGES");
  DisplayBitfield(schviews,schemaViews,SQL_ISV_COLUMNS,                "COLUMNS");
  DisplayBitfield(schviews,schemaViews,SQL_ISV_CONSTRAINT_COLUMN_USAGE,"CONSTRAINT_COLUMN_USAGE");
  DisplayBitfield(schviews,schemaViews,SQL_ISV_CONSTRAINT_TABLE_USAGE, "CONSTRAINT_TABLE_USAGE");
  DisplayBitfield(schviews,schemaViews,SQL_ISV_DOMAIN_CONSTRAINTS,     "DOMAIN_CONSTRAINTS");
  DisplayBitfield(schviews,schemaViews,SQL_ISV_DOMAINS,                "DOMAINS");
  DisplayBitfield(schviews,schemaViews,SQL_ISV_KEY_COLUMN_USAGE,       "KEY_COLUMN_USAGE");
  DisplayBitfield(schviews,schemaViews,SQL_ISV_REFERENTIAL_CONSTRAINTS,"REFERENTIAL_CONSTRAINTS");
  DisplayBitfield(schviews,schemaViews,SQL_ISV_SCHEMATA,               "SCHEMATA");
  DisplayBitfield(schviews,schemaViews,SQL_ISV_SQL_LANGUAGES,          "SQL_LANGUAGES");
  DisplayBitfield(schviews,schemaViews,SQL_ISV_TABLE_CONSTRAINTS,      "TABLE_CONSTRAINTS");
  DisplayBitfield(schviews,schemaViews,SQL_ISV_TABLE_PRIVILEGES,       "TABLE_PRIVILEGES");
  DisplayBitfield(schviews,schemaViews,SQL_ISV_TABLES,                 "TABLES");
  DisplayBitfield(schviews,schemaViews,SQL_ISV_TRANSLATIONS,           "TRANSLATIONS");
  DisplayBitfield(schviews,schemaViews,SQL_ISV_USAGE_PRIVILEGES,       "USAGE_PRIVILEGES");
  DisplayBitfield(schviews,schemaViews,SQL_ISV_VIEW_COLUMN_USAGE,      "VIEW_COLUMN_USAGE");
  DisplayBitfield(schviews,schemaViews,SQL_ISV_VIEW_TABLE_USAGE,       "VIEW_TABLE_USAGE");
  DisplayBitfield(schviews,schemaViews,SQL_ISV_VIEWS,                  "VIEWS");

  // VERSION NUMBERS
  InsertItem("ODBC Driver manager: "   + m_info->GetVersionODBCManager(),   infoItemVersion);
  InsertItem("ODBC Database driver: "  + m_info->GetVersionODBCDriver(),    infoItemVersion);
  InsertItem("ODBC Standard: "         + m_info->GetVersionODBCStandard(),  infoItemVersion);
  InsertItem("RDBMS version: "         + m_info->GetVersionRDBMS(),         infoItemVersion);
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
      line.AppendFormat(" (%s)",object.m_remarks);
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

  FILE* file = NULL;
  fopen_s(&file,filename,"w");
  if(!file)
  {
    AfxMessageBox("Cannot open file for ODBC report: " + filename);
    return;
  }
  fprintf(file,"ODBC Driver report\n");
  fprintf(file,"==================\n\n");

  HTREEITEM root  = GetRootItem();
  ReportCapabilities(root,0,file);

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
InfoTree::DisplayBitfield(HTREEITEM   item
                         ,SQLUINTEGER var
                         ,SQLUINTEGER macro
                         ,char*       text)
{
  if(var & macro)
  {
    return InsertItem(text,item);
  }
  else
  {
    return InsertItem(CString(text) + " (not supported)",item);
  }
}

CString
InfoTree::InfoNumberToString(SQLINTEGER number)
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
  if(totype & SQL_CVT_BIGINT)              InsertItem("BIGINT", datatype);
  if(totype & SQL_CVT_BINARY)              InsertItem("BINARY", datatype);
  if(totype & SQL_CVT_BIT)                 InsertItem("BIT",    datatype);
  if(totype & SQL_CVT_CHAR)                InsertItem("CHAR",   datatype);
  if(totype & SQL_CVT_DATE)                InsertItem("DATE",   datatype);
  if(totype & SQL_CVT_DECIMAL)             InsertItem("DECIMAL",datatype);
  if(totype & SQL_CVT_DOUBLE)              InsertItem("DOUBLE", datatype);
  if(totype & SQL_CVT_FLOAT)               InsertItem("FLOAT",  datatype);
  if(totype & SQL_CVT_INTEGER)             InsertItem("INTEGER",datatype);
  if(totype & SQL_CVT_INTERVAL_YEAR_MONTH) InsertItem("INTERVAL YEAR-MONTH",datatype);
  if(totype & SQL_CVT_INTERVAL_DAY_TIME)   InsertItem("INTERVAL DAY-TIME",  datatype);
  if(totype & SQL_CVT_LONGVARBINARY)       InsertItem("LONGVARBINARY",      datatype);
  if(totype & SQL_CVT_LONGVARCHAR)         InsertItem("LONGVARCHAR",        datatype);
  if(totype & SQL_CVT_NUMERIC)             InsertItem("NUMERIC",  datatype);
  if(totype & SQL_CVT_REAL)                InsertItem("REAL",     datatype);
  if(totype & SQL_CVT_SMALLINT)            InsertItem("SMALLINT", datatype);
  if(totype & SQL_CVT_TIME)                InsertItem("TIME",     datatype);
  if(totype & SQL_CVT_TIMESTAMP)           InsertItem("TIMESTAMP",datatype);
  if(totype & SQL_CVT_TINYINT)             InsertItem("TINYINT",  datatype);
  if(totype & SQL_CVT_VARBINARY)           InsertItem("VARBINARY",datatype);
  if(totype & SQL_CVT_VARCHAR)             InsertItem("VARCHAR",  datatype);
}

void
InfoTree::MakeTreeODBCExtensions(HTREEITEM item)
{
  CString item_any   = "SQLGetData can only be called for unbound columns";
  CString item_order = "SQLGetData can only be called in ascending order";
  CString item_block = "SQLGetData cannot be called for unbound columns in block cursors";
  CString item_bound = "SQLGetData can only be called for unbound columns";

  SQLUINTEGER getdata_extensions = m_info->GetGetDataExtensions();

  if(getdata_extensions & SQL_GD_ANY_COLUMN)
  {
    item_any = "SQLGetData can be called for any bound/unbound column";
  }
  if(getdata_extensions & SQL_GD_ANY_ORDER)
  {
    item_order = "SQLGetData can be called on columns in ANY order";
  }
  if(getdata_extensions & SQL_GD_BLOCK)
  {
    item_block = "SQLGetData can be called on unbound columns in block cursors";
  }
  if(getdata_extensions & SQL_GD_BOUND)
  {
    item_bound = "SQLGetData can be called for bound columns";
  }
  InsertItem(item_any,  item);
  InsertItem(item_order,item);
  InsertItem(item_block,item);
  InsertItem(item_bound,item);
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
    InsertItem("Driver reports ODBC 3.x Functions",item);

    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLALLOCCONNECT) ? "X" : "O") + " SQLAllocConnect";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLALLOCENV)     ? "X" : "O") + " SQLAllocEnv";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLALLOCHANDLE)  ? "X" : "O") + " SQLAllocHandle";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLALLOCHANDLESTD)?"X" : "O") + " SQLAllocHandleStd";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLALLOCSTMT)    ? "X" : "O") + " SQLAllocStmt";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLBINDCOL)      ? "X" : "O") + " SQLBindCol";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLBINDPARAM)    ? "X" : "O") + " SQLBindParam";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLBINDPARAMETER)? "X" : "O") + " SQLBindParameter";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLBROWSECONNECT)? "X" : "O") + " SQLBrowseConnect";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLBULKOPERATIONS)?"X" : "O") + " SQLBulkOperations";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLCANCEL)       ? "X" : "O") + " SQLCancel";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLCLOSECURSOR)  ? "X" : "O") + " SQLCloseCursor";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLCOLATTRIBUTE) ? "X" : "O") + " SQLColAttribute";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLCOLATTRIBUTES)? "X" : "O") + " SQLColAttributes";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLCOLUMNS)      ? "X" : "O") + " SQLColumns";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLCOLUMNPRIVILEGES)?"X":"O") + " SQLColumnPrivileges";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLCONNECT)      ? "X" : "O") + " SQLConnect";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLCOPYDESC)     ? "X" : "O") + " SQLCopyDesc";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLDATASOURCES)  ? "X" : "O") + " SQLDataSources";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLDESCRIBECOL)  ? "X" : "O") + " SQLDescribeCol";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLDESCRIBEPARAM)? "X" : "O") + " SQLDescribeParam";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLDISCONNECT)   ? "X" : "O") + " SQLDisconnect";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLDRIVERCONNECT)? "X" : "O") + " SQLDriverConnect";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLDRIVERS)      ? "X" : "O") + " SQLDrivers";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLENDTRAN)      ? "X" : "O") + " SQLEndTran";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLERROR)        ? "X" : "O") + " SQLError";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLEXECDIRECT)   ? "X" : "O") + " SQLExecDirect";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLEXECUTE)      ? "X" : "O") + " SQLExecute";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLEXTENDEDFETCH)? "X" : "O") + " SQLExetendedFetch";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLFETCH)        ? "X" : "O") + " SQLFetch";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLFETCHSCROLL)  ? "X" : "O") + " SQLFetchScroll";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLFOREIGNKEYS)  ? "X" : "O") + " SQLForeignKeys";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLFREECONNECT)  ? "X" : "O") + " SQLFreeConnect";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLFREEENV)      ? "X" : "O") + " SQLFreeEnv";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLFREEHANDLE)   ? "X" : "O") + " SQLFreeHandle";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLFREESTMT)     ? "X" : "O") + " SQLFreeStmt";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLGETCONNECTATTR)   ? "X" : "O") + " SQLGetConnectAttr";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLGETCONNECTOPTION) ? "X" : "O") + " SQLGetConnectOption";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLGETCURSORNAME)    ? "X" : "O") + " SQLGetCursorName";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLGETDATA)          ? "X" : "O") + " SQLGetData";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLGETDESCFIELD)     ? "X" : "O") + " SQLGetDescField";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLGETDESCREC)       ? "X" : "O") + " SQLGetDescRec";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLGETDIAGFIELD)     ? "X" : "O") + " SQLGetDiagField";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLGETDIAGREC)       ? "X" : "O") + " SQLGetDiagRec";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLGETENVATTR)       ? "X" : "O") + " SQLGetEnvAttr";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLGETFUNCTIONS)     ? "X" : "O") + " SQLGetFunctions";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLGETINFO)          ? "X" : "O") + " SQLGetInfo";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLGETSTMTATTR)      ? "X" : "O") + " SQLGetStmtAttr";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLGETSTMTOPTION)    ? "X" : "O") + " SQLGetStmtOption";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLGETTYPEINFO)      ? "X" : "O") + " SQLGetTypeInfo";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLMORERESULTS)      ? "X" : "O") + " SQLMoreResults";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLNATIVESQL)        ? "X" : "O") + " SQLNativeSQL";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLNUMPARAMS)        ? "X" : "O") + " SQLNumParams";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLNUMRESULTCOLS)    ? "X" : "O") + " SQLNumResultCols";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLPARAMDATA)        ? "X" : "O") + " SQLParamData";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLPARAMOPTIONS)     ? "X" : "O") + " SQLParamOptions";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLPREPARE)          ? "X" : "O") + " SQLPrepare";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLPRIMARYKEYS)      ? "X" : "O") + " SQLPrimaryKeys";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLPROCEDURECOLUMNS) ? "X" : "O") + " SQLProcedureColumns";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLPROCEDURES)       ? "X" : "O") + " SQLProcedures";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLPUTDATA)          ? "X" : "O") + " SQLPutData";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLROWCOUNT)         ? "X" : "O") + " SQLRowCount";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLSETSCROLLOPTIONS) ? "X" : "O") + " SQLSetScrollOptions";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLSETCONNECTATTR)   ? "X" : "O") + " SQLSetConnectAttr";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLSETCONNECTOPTION) ? "X" : "O") + " SQLSetConnectOption";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLSETCURSORNAME)    ? "X" : "O") + " SQLSetCursorName";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLSETDESCFIELD)     ? "X" : "O") + " SQLSetDescField";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLSETDESCREC)       ? "X" : "O") + " SQLSetDescRec";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLSETENVATTR)       ? "X" : "O") + " SQLSetEnvAttr";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLSETPARAM)         ? "X" : "O") + " SQLSetParam";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLSETPOS)           ? "X" : "O") + " SQLSetPos";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLSETSTMTATTR)      ? "X" : "O") + " SQLSetStmtAttr";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLSETSTMTOPTION)    ? "X" : "O") + " SQLSetStmtOption";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLSPECIALCOLUMNS)   ? "X" : "O") + " SQLSpecialColumns";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLSTATISTICS)       ? "X" : "O") + " SQLStatistics";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLTABLES)           ? "X" : "O") + " SQLTables";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLTABLEPRIVILEGES)  ? "X" : "O") + " SQLTablePrivileges";
    InsertItem(sitem,item);
    sitem = CString(SQL_FUNC_EXISTS(ODBCFunctions_3,SQL_API_SQLTRANSACT)         ? "X" : "O") + " SQLTransact";
    InsertItem(sitem,item);
  }
  else
  {
    InsertItem("Driver reports ODBC 1.x/2.x Functions",item);

    sitem = CString(ODBCFunctions_2[SQL_API_SQLALLOCCONNECT] ? "X" : "O") + "SQLAllocConnect";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLALLOCENV]     ? "X" : "O") + "SQLAllocEnv";
    InsertItem(sitem,item);
    sitem = "# SQLAllocHandle";
    InsertItem(sitem,item);
    sitem = "# SQLAllocHandleStd";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLALLOCSTMT]    ? "X" : "O") + "SQLAllocStmt";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLBINDCOL]      ? "X" : "O") + "SQLBindCol";
    InsertItem(sitem,item);
    sitem = "# SQLBindParam";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLBINDPARAMETER]? "X" : "O") + "SQLBindParameter";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLBROWSECONNECT]? "X" : "O") + "SQLBrowseConnect";
    InsertItem(sitem,item);
    sitem = "# SQLBulkOperations";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLCANCEL]       ? "X" : "O") + "SQLCancel";
    InsertItem(sitem,item);
    sitem = "# SQLCloseCursor";
    InsertItem(sitem,item);
    sitem = "# SQLColAttribute";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLCOLATTRIBUTES]? "X" : "O") + "SQLColAttributes";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLCOLUMNS]      ? "X" : "O") + "SQLColumns";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLCOLUMNPRIVILEGES]?"X":"O") + "SQLColumnPrivileges";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLCONNECT]      ? "X" : "O") + "SQLConnect";
    InsertItem(sitem,item);
    sitem = "# SQLCopyDesc";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLDATASOURCES]  ? "X" : "O") + "SQLDataSources";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLDESCRIBECOL]  ? "X" : "O") + "SQLDescribeCol";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLDESCRIBEPARAM]? "X" : "O") + "SQLDescribeParam";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLDISCONNECT]   ? "X" : "O") + "SQLDisconnect";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLDRIVERCONNECT]? "X" : "O") + "SQLDriverConnect";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLDRIVERS]      ? "X" : "O") + "SQLDrivers";
    InsertItem(sitem,item);
    sitem = "# SQLEndTran";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLERROR]        ? "X" : "O") + "SQLError";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLEXECDIRECT]   ? "X" : "O") + "SQLExecDirect";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLEXECUTE]      ? "X" : "O") + "SQLExecute";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLEXTENDEDFETCH]? "X" : "O") + "SQLExtendedFetch";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLFETCH]        ? "X" : "O") + "SQLFetch";
    InsertItem(sitem,item);
    sitem = "# SQLFetchScroll";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLFOREIGNKEYS]  ? "X" : "O") + "SQLForeignKeys";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLFREECONNECT]  ? "X" : "O") + "SQLFreeConnect";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLFREEENV]      ? "X" : "O") + "SQLFreeEnv";
    InsertItem(sitem,item);
    sitem = "# SQLFreeHandle";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLFREESTMT]      ? "X" : "O") + "SQLFreeStmt";
    InsertItem(sitem,item);
    sitem = "# SQLGetConnectAttr";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLGETCONNECTOPTION]  ? "X" : "O") + "SQLGetConnectOption";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLGETCURSORNAME]     ? "X" : "O") + "SQLGetCursorName";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLGETDATA]           ? "X" : "O") + "SQLGetData";
    InsertItem(sitem,item);
    sitem = "# SQLGetDescField";
    InsertItem(sitem,item);
    sitem = "# SQLGetDescRec";
    InsertItem(sitem,item);
    sitem = "# SQLGetDiagField";
    InsertItem(sitem,item);
    sitem = "# SQLGetDiagRec";
    InsertItem(sitem,item);
    sitem = "# SQLGetEnvAttr";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLGETFUNCTIONS]      ? "X" : "O") + "SQLGetFunctions";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLGETINFO]           ? "X" : "O") + "SQLGetInfo";
    InsertItem(sitem,item);
    sitem = "# SQLGetStmtAttr";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLGETSTMTOPTION]     ? "X" : "O") + "SQLGetStmtOption";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLGETTYPEINFO]       ? "X" : "O") + "SQLGetTypeInfo";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLMORERESULTS]       ? "X" : "O") + "SQLMoreResults";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLNATIVESQL]         ? "X" : "O") + "SQLNativeSQL";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLNUMPARAMS]         ? "X" : "O") + "SQLNumParams";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLNUMRESULTCOLS]     ? "X" : "O") + "SQLNumResultCols";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLPARAMDATA]         ? "X" : "O") + "SQLParamData";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLPARAMOPTIONS]      ? "X" : "O") + "SQLParamOptions";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLPREPARE]           ? "X" : "O") + "SQLPrepare";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLPRIMARYKEYS]       ? "X" : "O") + "SQLPrimaryKeys";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLPROCEDURECOLUMNS]  ? "X" : "O") + "SQLProcedureColumns";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLPROCEDURES]        ? "X" : "O") + "SQLProcedures";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLPUTDATA]           ? "X" : "O") + "SQLPutData";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLROWCOUNT]          ? "X" : "O") + "SQLRowCount";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLSETSCROLLOPTIONS]  ? "X" : "O") + "SQLSetScrollOptions";
    InsertItem(sitem,item);
    sitem = "# SQLSetConnectAttr";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLSETCONNECTOPTION]  ? "X" : "O") + "SQLSetConnectOption";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLSETCURSORNAME]     ? "X" : "O") + "SQLSetCursorName";
    InsertItem(sitem,item);
    sitem = "# SQLSetDescField";
    InsertItem(sitem,item);
    sitem = "# SQLSetDescRec";
    InsertItem(sitem,item);
    sitem = "# SQLSetEnvAttr";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLSETPARAM]          ? "X" : "O") + "SQLSetParam";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLSETPOS]            ? "X" : "O") + "SQLSetPos";
    InsertItem(sitem,item);
    sitem = "# SQLSetStmtAttr";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLGETCONNECTOPTION]  ? "X" : "O") + "SQLGetConnectOption";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLSPECIALCOLUMNS]    ? "X" : "O") + "SQLSpecialColumns";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLSTATISTICS]        ? "X" : "O") + "SQLStatistics";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLTABLES]            ? "X" : "O") + "SQLTables";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLTABLEPRIVILEGES]   ? "X" : "O") + "SQLTablePrivileges";
    InsertItem(sitem,item);
    sitem = CString(ODBCFunctions_2[SQL_API_SQLTRANSACT]          ? "X" : "O") + "SQLTransact";
    InsertItem(sitem,item);
  }
}

void
InfoTree::ReportCapabilities(HTREEITEM  p_branch
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
    fprintf(p_file,"%s\n",GetItemText(branch).GetString());
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

