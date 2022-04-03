////////////////////////////////////////////////////////////////////////
//
// File: SQLVariant.h
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
#pragma  once
#include <sqlext.h>
#include "SQLVariantOperator.h"
#include "bcd.h"

namespace SQLComponents
{

#ifndef SQLNUM_MAX_PREC
// Max precision of a NUMERIC as a bcd (Binary Coded Decimal)
#define SQLNUM_MAX_PREC    38
#endif 

#ifndef SQLNUM_DEF_SCALE
// The default scaling of a NUMERIC
#define SQLNUM_DEF_SCALE    2
#endif

// Forwarded declarations
class SQLDate;
class SQLTime;
class SQLTimestamp;
class SQLInterval;
class SQLGuid;

// Type of parameter for queries and persistent-stored-modules
typedef enum _param_type
{
   P_SQL_PARAM_TYPE_UNKNOWN = 0
  ,P_SQL_PARAM_INPUT        = 1
  ,P_SQL_PARAM_INPUT_OUTPUT = 2
  ,P_SQL_RESULT_COL         = 3
  ,P_SQL_PARAM_OUTPUT       = 4
  ,P_SQL_RETURN_VALUE       = 5
}
SQLParamType;

//////////////////////////////////////////////////////////////////////////
//
// THE VARIANT CLASS
//
//////////////////////////////////////////////////////////////////////////

class SQLVariant
{
public:
   // Generic constructors
   SQLVariant();                              // Generic
   SQLVariant(int p_type,int p_space);        // ODBC driver reserve precision
   SQLVariant(SQLVariant* p_var);             // From another SQLVariant pointer
   SQLVariant(const SQLVariant& p_var);       // From another SQLVariant reference
     // Type constructors
   SQLVariant(const char* p_data);            // SQL_C_CHAR
   SQLVariant(XString& p_data);               // SQL_C_CHAR
   SQLVariant(short p_short);                 // SQL_C_SHORT / SQL_C_SSHORT
   SQLVariant(unsigned short p_short);        // SQL_C_USHORT
   SQLVariant(int p_long);                    // SQL_C_LONG / SQL_C_SLONG
   SQLVariant(unsigned int p_long);           // SQL_C_ULONG
   SQLVariant(float p_float);                 // SQL_C_FLOAT
   SQLVariant(double p_double);               // SQL_C_DOUBLE
   SQLVariant(bool p_bit);                    // SQL_C_BIT
   SQLVariant(char p_tinyint);                // SQL_C_TINYINT / SQL_C_STINYINT
   SQLVariant(unsigned char p_tinyint);       // SQL_C_UTINYINT
   SQLVariant(__int64 p_bigint);              // SQL_C_BIGINT
   SQLVariant(unsigned __int64 p_bigint);     // SQL_C_UBIGINT
   SQLVariant(SQL_NUMERIC_STRUCT* p_numeric); // SQL_C_NUMERIC
   SQLVariant(SQLGUID* p_guid);               // SQL_C_GUID
   SQLVariant(void* p_binary,size_t p_size);  // SQL_C_BINARY
   SQLVariant(DATE_STRUCT* p_date);           // SQL_C_DATE / SQL_C_TYPE_DATE
   SQLVariant(TIME_STRUCT* p_time);           // SQL_C_TIME / SQL_C_TYPE_TIME
   SQLVariant(TIMESTAMP_STRUCT* p_stamp);     // SQL_C_TIMESTAMP / SQL_C_TYPE_TIMESTAMP
   SQLVariant(SQL_INTERVAL_STRUCT* p_inter);  // SQL_C_INTERVAL_YEAR -> SQL_C_INTERVAL_DAY_TO_SECOND
   // From complex constructors
   SQLVariant(SQLDate* p_date);               // SQLDate
   SQLVariant(SQLTime* p_time);               // SQLTime
   SQLVariant(SQLTimestamp* p_stamp);         // SQLTimestamp
   SQLVariant(SQLInterval* p_interval);       // SQLInterval
   SQLVariant(const bcd* p_bcd);              // Binary Coded Decimal
   SQLVariant(SQLGuid* p_guid);               // SQLGuid
   // Destructor
  ~SQLVariant();
   
   // Reset the current value. BEWARE: Frees the char and binary pointers!!
   void    Reset();

   // STATUS
   bool    IsNULL();
   bool    IsEmpty();
   bool    IsNumericType();
   bool    IsDecimalType();
   bool    IsIntervalType();
   bool    IsDateTimeType();

   // GETTERS
   int     GetDataType();
   int     GetDataSize();
   int     GetSQLDataType();
   bool    GetAtExec();
   int     GetBinaryPieceSize();
   int     GetBinaryLength();
   SQLLEN* GetIndicatorPointer();
   int     GetColumnNumber();
   int     GetParameterType();
   int     GetFraction();
   int     GetNumericPrecision();  // Only for SQL_NUMERIC
   int     GetNumericScale();      // Only for SQL_NUMERIC
   // SETTERS
   void    SetSQLDataType(int p_type);
   void    SetAtExec(bool p_atExec);
   void    SetBinaryPieceSize(int p_size);
   void    SetColumnNumber(int p_column);
   void    SetParameterType(SQLParamType p_type);
   void    SetSizeIndicator(bool p_realSize);
   void    SetNumericPrecisionScale(int p_precision,int p_scale);
   void    SetFromBinaryStreamData(int p_type,int p_length,void* p_data,bool p_isnull);
   void    SetNULL();
   void    SetDataTypeDate();

   // Variable space functions
   void    ReserveSpace(int p_type,int p_space);
   void    ShrinkSpace();
   void    TruncateSpace(unsigned p_length);
   // Truncation of a field
   void    TruncateCharacter();
   void    TruncateTimestamp(int p_decimals = 0);

   // General access
   bool                 SetData(int p_type,const char* p_data);
   void*                GetDataPointer();
   void                 SetFromRawDataPointer(void* p_pointer,int p_size = 0);
   // BLOB Functions
   void                 AttachBinary(void* p_pointer,unsigned long p_size = 0);
   void                 DetachBinary();

   // Access per type
   const char*          GetAsChar();
   void                 GetAsString(XString& p_result);
   void*                GetAsBinary();
   bool                 GetAsBoolean();
   short                GetAsSShort();
   unsigned short       GetAsUShort();
   int                  GetAsSLong();
   unsigned int         GetAsULong();
   float                GetAsFloat();
   double               GetAsDouble();
   char                 GetAsBit();
   char                 GetAsSTinyInt();
   unsigned char        GetAsUTinyInt();
   SQLBIGINT            GetAsSBigInt();
   SQLUBIGINT           GetAsUBigInt();
   SQL_NUMERIC_STRUCT*  GetAsNumeric();
   SQLGUID*             GetAsGUID();
   DATE_STRUCT*         GetAsDate();
   TIME_STRUCT*         GetAsTime();
   TIMESTAMP_STRUCT*    GetAsTimestamp();
   XString              GetAsEuropeanTimestamp();
   SQL_INTERVAL_STRUCT* GetAsInterval();
   XString              GetAsSQLString();
   // Access per complex type
   SQLDate              GetAsSQLDate();
   SQLTime              GetAsSQLTime();
   SQLTimestamp         GetAsSQLTimestamp();
   SQLInterval          GetAsSQLInterval();
   SQLGuid              GetAsSQLGuid();
   bcd                  GetAsBCD();
   
   // INFO about type names/values
   static  int          FindDatatype   (char* p_type);
   static  const char*  FindDatatype   (int   p_type);
   static  int          FindParamtype  (char* p_type);
   static  const char*  FindParamtype  (int   p_type);
   static  int          FindSQLDatatype(char* p_type);
   static  const char*  FindSQLDatatype(int   p_type);
           int          FindDataTypeFromSQLType();

   // Assignment operator
   SQLVariant& operator  =(const SQLVariant& p_original);
   // Assignment operator from original data
   SQLVariant& operator  =(const char* p_data);              // SQL_C_CHAR
   SQLVariant& operator  =(XString& p_data);                 // SQL_C_CHAR
   SQLVariant& operator  =(XString  p_data);                 // SQL_C_CHAR
   SQLVariant& operator  =(short p_data);                    // SQL_C_SHORT / SQL_C_SSHORT
   SQLVariant& operator  =(unsigned short p_data);           // SQL_C_USHORT
   SQLVariant& operator  =(int p_data);                      // SQL_C_LONG  / SQL_C_SLONG
   SQLVariant& operator  =(unsigned int p_data);             // SQL_C_ULONG
   SQLVariant& operator  =(float p_data);                    // SQL_C_FLOAT
   SQLVariant& operator  =(double p_data);                   // SQL_C_DOUBLE
   SQLVariant& operator  =(bool p_data);                     // SQL_C_BIT
   SQLVariant& operator  =(char p_data);                     // SQL_C_TINYINT / SQL_C_STINYINT
   SQLVariant& operator  =(unsigned char p_data);            // SQL_C_UTINYINT
   SQLVariant& operator  =(__int64 p_data);                  // SQL_C_BIGINT / SQL_C_SBIGINT
   SQLVariant& operator  =(unsigned __int64 p_data);         // SQL_C_UBIGINT
   SQLVariant& operator  =(SQL_NUMERIC_STRUCT* p_data);      // SQL_C_NUMERIC
   SQLVariant& operator  =(SQLGUID* p_data);                 // SQL_C_GUID
   SQLVariant& operator  =(DATE_STRUCT* p_data);             // SQL_C_DATE / SQL_C_TYPE_DATE
   SQLVariant& operator  =(TIME_STRUCT* p_data);             // SQL_C_TIME / SQL_C_TYPE_TIME
   SQLVariant& operator  =(TIMESTAMP_STRUCT* p_data);        // SQL_C_TIMESTAMP / SQL_C_TYPE_TIMESTAMP
   SQLVariant& operator  =(SQL_INTERVAL_STRUCT* p_data);     // SQL_C_INTERVAL_YEAR -> SQL_C_INTERVAL_DAY_TO_SECOND
   // Assignments from complex constructors
   SQLVariant& operator  =(SQLDate& p_data);                 // SQLDate
   SQLVariant& operator  =(SQLTime& p_data);                 // SQLTime
   SQLVariant& operator  =(SQLTimestamp& p_data);            // SQLTimestamp
   SQLVariant& operator  =(SQLInterval& p_data);             // SQLInterval
   SQLVariant& operator  =(SQLGuid& p_guid);                 // SQLGuid
   SQLVariant& operator  =(bcd& p_bcd);                      // Binary Coded Decimal

   // Comparison operators
   bool        operator ==(SQLVariant& p_right);
   bool        operator !=(SQLVariant& p_right);
   bool        operator  >(SQLVariant& p_right);
   bool        operator  <(SQLVariant& p_right);
   bool        operator >=(SQLVariant& p_right);
   bool        operator <=(SQLVariant& p_right);
 
   // Arithmetic operators
   SQLVariant  operator  +(SQLVariant& p_right);
   SQLVariant  operator  -(SQLVariant& p_right);
   SQLVariant  operator  *(SQLVariant& p_right);
   SQLVariant  operator  /(SQLVariant& p_right);
   SQLVariant  operator  %(SQLVariant& p_right);

   // Arithmetic assignment operators
   SQLVariant& operator  +=(SQLVariant& p_right);
   SQLVariant& operator  -=(SQLVariant& p_right);
   SQLVariant& operator  *=(SQLVariant& p_right);
   SQLVariant& operator  /=(SQLVariant& p_right);
   SQLVariant& operator  %=(SQLVariant& p_right);

   // Unary increment/decrement operators
   SQLVariant& operator ++();       // Prefix  increment
   SQLVariant& operator --();       // Prefix  decrement
   SQLVariant  operator ++(int);    // Postfix increment
   SQLVariant  operator --(int);    // Postfix decrement

   // Cast operators
   operator bool();
   operator char();
   operator const char*();
   operator unsigned char();
   operator short();
   operator unsigned short();
   operator int();
   operator unsigned int();
   operator long();
   operator unsigned long();
   operator float();
   operator double();
   operator __int64();
   operator unsigned __int64();
   operator SQLDate();
   operator SQLTime();
   operator SQLTimestamp();
   operator SQLInterval();
   operator SQLGuid();
   operator XString();
   operator bcd();

private:
   // Init empty variant
   void    Init();
   // Total internal reset (type and data store)
   void    ResetDataType(int p_type);
   // Internal conversions
   bool    StringToBinary (const char* p_data);
   bool    BinaryToString (unsigned char* buffer,int buflen);
   // Throw error as a result of internal trimming
   void*   ThrowErrorDatatype(int p_getas);
   // Throw error as a result of an impossible operator
   void    ThrowErrorOperator(SQLVarOperator p_operator);

   // Private Data
   int    m_datatype;         // Primary datatype SQL_C_XXXX
   int    m_sqlDatatype;      // Secondary datatype SQL_XXXX for CHAR and BINARY types
   int    m_binaryLength;     // Buffer length of CHAR types and BINARY types
   bool   m_useAtExec;        // As atExec procedure to put/get
   int    m_binaryPieceSize;  // Buffer piece at a time to put/get
   SQLLEN m_indicator;        // Null indicator at length of gotten data
   int    m_columnNumber;     // Column number in the result set
   int    m_paramType;        // Input/output/input-output/result/column
   union _data
   {
      // POINTER TYPES
      char*                           m_dataCHAR;         // SQL_C_CHAR               SQL_CHAR
//    wchar*                          m_dataWCHAR;        // SQL_C_WCHAR              SQL_WCHAR
      void*                           m_dataBINARY;       // SQL_C_BINARY             SQL_BINARY
      // STORAGE TYPES
      short                           m_dataSHORT;        // SQL_C_SHORT              SQL_SMALLINT
      signed short                    m_dataSSHORT;       // SQL_C_SSHORT             signed
      unsigned short                  m_dataUSHORT;       // SQL_C_USHORT             unsigned
      long                            m_dataLONG;         // SQL_C_LONG               SQL_INTEGER
      signed long                     m_dataSLONG;        // SQL_C_SLONG              signed
      unsigned long                   m_dataULONG;        // SQL_C_ULONG              unsigned
      float                           m_dataFLOAT;        // SQL_C_FLOAT              SQL_REAL
      double                          m_dataDOUBLE;       // SQL_C_DOUBLE             SQL_DOUBLE
      char                            m_dataBIT;          // SQL_C_BIT                SQL_BIT
      char                            m_dataTINYINT;      // SQL_C_TINYINT            SQL_TINYINT
      signed char                     m_dataSTINYINT;     // SQL_C_STINYINT           signed
      unsigned char                   m_dataUTINYINT;     // SQL_C_UTINYINT           unsigned
      SQLBIGINT                       m_dataSBIGINT;      // SQL_C_SBIGINT            SQL_BIGINT
      SQLUBIGINT                      m_dataUBIGINT;      // SQL_C_UBIGINT            SQL_UBIGINT
      SQL_NUMERIC_STRUCT              m_dataNUMERIC;      // SQL_C_NUMERIC            SQL_NUMERIC
      SQLGUID                         m_dataGUID;         // SQL_C_GUID               SQL_GUID
      DATE_STRUCT                     m_dataDATE;         // SQL_C_DATE               SQL_DATE
      TIME_STRUCT                     m_dataTIME;         // SQL_C_TIME               SQL_TIME
      TIMESTAMP_STRUCT                m_dataTIMESTAMP;    // SQL_C_TIMESTAMP          SQL_TIMESTAMP
      DATE_STRUCT                     m_dataTYPEDATE;     // SQL_C_TYPE_DATE          SQL_TYPE_DATE
      TIME_STRUCT                     m_dataTYPETIME;     // SQL_C_TYPE_TIME          SQL_TYPE_TIME
      TIMESTAMP_STRUCT                m_dataTYPETIMESTAMP;// SQL_C_TYPE_TIMESTAMP     SQL_TYPE_TIMESTAMP
      SQL_INTERVAL_STRUCT             m_dataINTERVAL;     // SQL_C_INTERVAL_DAY       SQL_INTERVAL_DAY
                                                          // SQL_C_INTERVAL_DAY_TO_HOUR      
                                                          // SQL_C_INTERVAL_DAY_TO_MINUTE
                                                          // SQL_C_INTERVAL_DAY_TO_SECOND
                                                          // SQL_C_INTERVAL_YEAR         
                                                          // SQL_C_INTERVAL_YEAR_TO_MONTH
                                                          // SQL_C_INTERVAL_MONTH        
                                                          // SQL_C_INTERVAL_HOUR         
                                                          // SQL_C_INTERVAL_HOUR_TO_MINUTE
                                                          // SQL_C_INTERVAL_HOUR_TO_SECOND
                                                          // SQL_C_INTERVAL_MINUTE        
                                                          // SQL_C_INTERVAL_MINUTE_TO_SECOND
                                                          // SQL_C_INTERVAL_SECOND
   }
   m_data;
};

inline bool
SQLVariant::GetAtExec()
{
  return m_useAtExec;
}

inline void
SQLVariant::SetAtExec(bool p_atExec)
{
  m_useAtExec = p_atExec;
}

inline int
SQLVariant::GetBinaryPieceSize()
{
  return m_binaryPieceSize;
}

inline int
SQLVariant::GetBinaryLength()
{
  return m_binaryLength;
}

inline void
SQLVariant::SetBinaryPieceSize(int p_size)
{
  m_binaryPieceSize = p_size;
}

inline SQLLEN*
SQLVariant::GetIndicatorPointer()
{
  return &m_indicator;
}

inline int
SQLVariant::GetColumnNumber()
{
  return m_columnNumber;
}

inline void
SQLVariant::SetColumnNumber(int p_column)
{
  m_columnNumber = p_column;
}

inline int
SQLVariant::GetParameterType()
{
  return m_paramType;
}

inline int     
SQLVariant::GetDataType()
{
  return m_datatype;
}

inline bool
SQLVariant::IsNULL()
{
  return (m_indicator == SQL_NULL_DATA);
}

inline int
SQLVariant::GetSQLDataType()
{
  return m_sqlDatatype;
}

//////////////////////////////////////////////////////////////////////////
//
// USING short form of SQLVariant in your code
//
//////////////////////////////////////////////////////////////////////////

using var = SQLVariant;

// End of namespace
}
