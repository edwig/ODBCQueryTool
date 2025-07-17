////////////////////////////////////////////////////////////////////////
//
// File: SQLVariant.h
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
#pragma  once
#include <sqlext.h>
#include "SQLVariantOperator.h"
#include "SQLParameterType.h"
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

//////////////////////////////////////////////////////////////////////////
//
// THE VARIANT CLASS
//
//////////////////////////////////////////////////////////////////////////

class SQLVariant
{
public:
   // Generic constructors
   SQLVariant();                                          // Generic
   SQLVariant(int p_type,int p_space);                    // ODBC driver reserve precision
   SQLVariant(const SQLVariant* p_var);                   // From another SQLVariant pointer
   SQLVariant(const SQLVariant& p_var);                   // From another SQLVariant reference
   // Type constructors
   SQLVariant(LPCTSTR p_data,bool p_wide = false);        // SQL_C_CHAR / SQL_C_WCHAR
   SQLVariant(const XString& p_data,bool p_wide = false); // SQL_C_CHAR / SQL_C_WCHAR
   SQLVariant(short p_short);                             // SQL_C_SHORT / SQL_C_SSHORT
   SQLVariant(unsigned short p_short);                    // SQL_C_USHORT
   SQLVariant(int p_long);                                // SQL_C_LONG / SQL_C_SLONG
   SQLVariant(unsigned int p_long);                       // SQL_C_ULONG
   SQLVariant(float p_float);                             // SQL_C_FLOAT
   SQLVariant(double p_double);                           // SQL_C_DOUBLE
   SQLVariant(bool p_bit);                                // SQL_C_BIT
   SQLVariant(char p_tinyint);                            // SQL_C_TINYINT / SQL_C_STINYINT
   SQLVariant(unsigned char p_tinyint);                   // SQL_C_UTINYINT
   SQLVariant(__int64 p_bigint);                          // SQL_C_BIGINT
   SQLVariant(unsigned __int64 p_bigint);                 // SQL_C_UBIGINT
   SQLVariant(const SQL_NUMERIC_STRUCT* p_numeric);       // SQL_C_NUMERIC
   SQLVariant(const SQLGUID* p_guid);                     // SQL_C_GUID
   SQLVariant(const void* p_binary,size_t p_size);        // SQL_C_BINARY
   SQLVariant(const DATE_STRUCT* p_date);                 // SQL_C_DATE / SQL_C_TYPE_DATE
   SQLVariant(const TIME_STRUCT* p_time);                 // SQL_C_TIME / SQL_C_TYPE_TIME
   SQLVariant(const TIMESTAMP_STRUCT* p_stamp);           // SQL_C_TIMESTAMP / SQL_C_TYPE_TIMESTAMP
   SQLVariant(const SQL_INTERVAL_STRUCT* p_inter);        // SQL_C_INTERVAL_YEAR -> SQL_C_INTERVAL_DAY_TO_SECOND
   // From complex constructors
   SQLVariant(const SQLDate* p_date);                     // SQLDate
   SQLVariant(const SQLTime* p_time);                     // SQLTime
   SQLVariant(const SQLTimestamp* p_stamp);               // SQLTimestamp
   SQLVariant(const SQLInterval* p_interval);             // SQLInterval
   SQLVariant(const bcd* p_bcd);                          // Binary Coded Decimal
   SQLVariant(const SQLGuid* p_guid);                     // SQLGuid
   // Destructor
  ~SQLVariant();
   
   // Reset the current value. BEWARE: Frees the char and binary pointers!!
   void    Reset();

   // STATUS
   bool    IsNULL() const;
   bool    IsEmpty() const;
   bool    IsNumericType() const;
   bool    IsDecimalType() const;
   bool    IsIntervalType() const;
   bool    IsDateTimeType() const;
   bool    IsBinaryType() const;

   // GETTERS
   int     GetDataType() const;
   int     GetDataSize() const;
   int     GetSQLDataType() const;
   bool    GetAtExec() const;
   int     GetBinaryPieceSize() const;
   int     GetBinaryLength() const;
   SQLLEN* GetIndicatorPointer();
   int     GetColumnNumber() const;
   SQLParamType GetParameterType() const;
   int     GetFraction() const;
   int     GetNumericPrecision() const;  // Only for SQL_NUMERIC
   int     GetNumericScale() const;      // Only for SQL_NUMERIC
   // SETTERS
   void    SetSQLDataType(int p_type);
   void    SetAtExec(bool p_atExec);
   void    SetBinaryPieceSize(int p_size);
   void    SetColumnNumber(int p_column);
   void    SetParameterType(SQLParamType p_type);
   void    SetSizeIndicator(bool p_realSize,bool p_binary = false);
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
   void    TruncateCharacterReset();
   void    TruncateTimestamp(int p_decimals = 0);

   // General access
   bool         SetData(int p_type,LPCTSTR p_data);
   void         SetFromRawDataPointer(void* p_pointer,int p_size = 0);
   const void*  GetDataPointer() const;
   // BLOB Functions
   void    AttachBinary(void* p_pointer,unsigned long p_size = 0);
   void    DetachBinary();
   // Find special char type
   int     FindDataTypeFromSQLType() const;

   // Access per type
   char*                GetAsChar() const;
   XString              GetAsString() const;
   void                 GetAsString(XString& p_result) const;
   void*                GetAsBinary() const;
   bool                 GetAsBoolean() const;
   short                GetAsSShort() const;
   unsigned short       GetAsUShort() const;
   int                  GetAsSLong() const;
   unsigned int         GetAsULong() const;
   float                GetAsFloat() const;
   double               GetAsDouble() const;
   char                 GetAsBit() const;
   char                 GetAsSTinyInt() const;
   unsigned char        GetAsUTinyInt() const;
   SQLBIGINT            GetAsSBigInt() const;
   SQLUBIGINT           GetAsUBigInt() const;
   // Access per complex type
   XString              GetAsSQLString() const;
   SQLDate              GetAsSQLDate() const;
   SQLTime              GetAsSQLTime() const;
   SQLTimestamp         GetAsSQLTimestamp() const;
   SQLInterval          GetAsSQLInterval() const;
   SQLGuid              GetAsSQLGuid() const;
   bcd                  GetAsBCD() const;
   const SQL_NUMERIC_STRUCT*  GetAsNumeric() const;
   const SQLGUID*             GetAsGUID() const;
   const DATE_STRUCT*         GetAsDate() const;
   const TIME_STRUCT*         GetAsTime() const;
   const TIMESTAMP_STRUCT*    GetAsTimestamp() const;
   const XString              GetAsEuropeanTimestamp() const;
   const SQL_INTERVAL_STRUCT* GetAsInterval() const;

   // SET VALUE PER TYPE
   void                 Set(LPCTSTR       p_string,bool p_wide = false);
   void                 Set(const XString p_string,bool p_wide = false);
   void                 Set(void* p_pointer,int p_length);
   void                 Set(bool p_boolean);
   void                 Set(short p_short);
   void                 Set(unsigned short p_short);
   void                 Set(int p_slong);
   void                 Set(unsigned int p_ulong);
   void                 Set(float p_float);
   void                 Set(double p_double);
   void                 Set(char p_stinyint);
   void                 Set(unsigned char p_utinyint);
   void                 Set(bcd p_bcd);
   void                 Set(SQLBIGINT p_sbigint);
   void                 Set(SQLUBIGINT p_ubigint);
   void                 Set(const SQL_NUMERIC_STRUCT* p_nu1meric);
   void                 Set(const SQLGUID* p_guid);
   void                 Set(const DATE_STRUCT* p_date);
   void                 Set(const TIME_STRUCT* p_time);
   void                 Set(const TIMESTAMP_STRUCT* p_timestamp);
   void                 Set(const SQL_INTERVAL_STRUCT* p_interval);
   void                 Set(const SQLDate* p_date);
   void                 Set(const SQLTime* p_time);
   void                 Set(const SQLTimestamp* p_timestamp);
   void                 Set(const SQLInterval* p_interval);
   void                 Set(const SQLGuid* p_guid);
   void                 SetFromEuropeanTimestamp(const XString p_stamp);

   // Assignment operator
   SQLVariant& operator  =(const SQLVariant& p_original);
   // Assignment operator from original data
   SQLVariant& operator  =(LPCTSTR p_data);                        // SQL_C_CHAR
   SQLVariant& operator  =(const XString& p_data);                 // SQL_C_CHAR
   SQLVariant& operator  =(const XString  p_data);                 // SQL_C_CHAR
   SQLVariant& operator  =(const short p_data);                    // SQL_C_SHORT / SQL_C_SSHORT
   SQLVariant& operator  =(const unsigned short p_data);           // SQL_C_USHORT
   SQLVariant& operator  =(const int p_data);                      // SQL_C_LONG  / SQL_C_SLONG
   SQLVariant& operator  =(const unsigned int p_data);             // SQL_C_ULONG
   SQLVariant& operator  =(const float p_data);                    // SQL_C_FLOAT
   SQLVariant& operator  =(const double p_data);                   // SQL_C_DOUBLE
   SQLVariant& operator  =(const bool p_data);                     // SQL_C_BIT
   SQLVariant& operator  =(const char p_data);                     // SQL_C_TINYINT / SQL_C_STINYINT
   SQLVariant& operator  =(const unsigned char p_data);            // SQL_C_UTINYINT
   SQLVariant& operator  =(const __int64 p_data);                  // SQL_C_BIGINT / SQL_C_SBIGINT
   SQLVariant& operator  =(const unsigned __int64 p_data);         // SQL_C_UBIGINT
   SQLVariant& operator  =(const SQL_NUMERIC_STRUCT* p_data);      // SQL_C_NUMERIC
   SQLVariant& operator  =(const SQLGUID* p_data);                 // SQL_C_GUID
   SQLVariant& operator  =(const DATE_STRUCT* p_data);             // SQL_C_DATE / SQL_C_TYPE_DATE
   SQLVariant& operator  =(const TIME_STRUCT* p_data);             // SQL_C_TIME / SQL_C_TYPE_TIME
   SQLVariant& operator  =(const TIMESTAMP_STRUCT* p_data);        // SQL_C_TIMESTAMP / SQL_C_TYPE_TIMESTAMP
   SQLVariant& operator  =(const SQL_INTERVAL_STRUCT* p_data);     // SQL_C_INTERVAL_YEAR -> SQL_C_INTERVAL_DAY_TO_SECOND
   // Assignments from complex constructors
   SQLVariant& operator  =(const SQLDate& p_data);                 // SQLDate
   SQLVariant& operator  =(const SQLTime& p_data);                 // SQLTime
   SQLVariant& operator  =(const SQLTimestamp& p_data);            // SQLTimestamp
   SQLVariant& operator  =(const SQLInterval& p_data);             // SQLInterval
   SQLVariant& operator  =(const SQLGuid& p_guid);                 // SQLGuid
   SQLVariant& operator  =(const bcd& p_bcd);                      // Binary Coded Decimal

   // Comparison operators
   bool        operator ==(const SQLVariant& p_right) const;
   bool        operator !=(const SQLVariant& p_right) const;
   bool        operator  >(const SQLVariant& p_right) const;
   bool        operator  <(const SQLVariant& p_right) const;
   bool        operator >=(const SQLVariant& p_right) const;
   bool        operator <=(const SQLVariant& p_right) const;
 
   // Arithmetic operators
   SQLVariant  operator  +(const SQLVariant& p_right);
   SQLVariant  operator  -(const SQLVariant& p_right);
   SQLVariant  operator  *(const SQLVariant& p_right);
   SQLVariant  operator  /(const SQLVariant& p_right);
   SQLVariant  operator  %(const SQLVariant& p_right);

   // Arithmetic assignment operators
   SQLVariant& operator  +=(const SQLVariant& p_right);
   SQLVariant& operator  -=(const SQLVariant& p_right);
   SQLVariant& operator  *=(const SQLVariant& p_right);
   SQLVariant& operator  /=(const SQLVariant& p_right);
   SQLVariant& operator  %=(const SQLVariant& p_right);

   // Unary increment/decrement operators
   SQLVariant& operator ++();       // Prefix  increment
   SQLVariant& operator --();       // Prefix  decrement
   SQLVariant  operator ++(int);    // Postfix increment
   SQLVariant  operator --(int);    // Postfix decrement

   // Cast operators
   operator bool();
   operator char();
   operator LPCSTR();
   operator uchar();
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
   bool    StringToBinary (const char* p_data) const;
   bool    BinaryToString (unsigned char* buffer,int buflen) const;
   // Throw error as a result of internal trimming
   void*   ThrowErrorDatatype(int p_getas) const;
   // Throw error as a result of an impossible operator
   void    ThrowErrorOperator(SQLVarOperator p_operator);

   // Private Data
   int          m_datatype;         // Primary datatype SQL_C_XXXX
   int          m_sqlDatatype;      // Secondary datatype SQL_XXXX for CHAR and BINARY types
   int          m_binaryLength;     // Buffer length of CHAR types and BINARY types
   bool         m_useAtExec;        // As atExec procedure to put/get
   int          m_binaryPieceSize;  // Buffer piece at a time to put/get
   SQLLEN       m_indicator;        // Null indicator at length of gotten data
   int          m_columnNumber;     // Column number in the result set
   SQLParamType m_paramType;        // Input/output/input-output/result/column
   union _data
   {
      // HEAP POINTER TYPES
      char*                           m_dataCHAR;         // SQL_C_CHAR                       SQL_CHAR
      wchar_t*                        m_dataWCHAR;        // SQL_C_WCHAR                      SQL_WCHAR
      void*                           m_dataBINARY;       // SQL_C_BINARY                     SQL_BINARY
      // STORAGE TYPES
      short                           m_dataSHORT;        // SQL_C_SHORT                      SQL_SMALLINT
      signed short                    m_dataSSHORT;       // SQL_C_SSHORT                     SQL_SHORT
      unsigned short                  m_dataUSHORT;       // SQL_C_USHORT                     SQL_USHORT
      long                            m_dataLONG;         // SQL_C_LONG                       SQL_INTEGER
      signed long                     m_dataSLONG;        // SQL_C_SLONG                      SQL_LONG
      unsigned long                   m_dataULONG;        // SQL_C_ULONG                      SQL_ULONG
      float                           m_dataFLOAT;        // SQL_C_FLOAT                      SQL_REAL
      double                          m_dataDOUBLE;       // SQL_C_DOUBLE                     SQL_DOUBLE
      char                            m_dataBIT;          // SQL_C_BIT                        SQL_BIT
      char                            m_dataTINYINT;      // SQL_C_TINYINT                    SQL_TINYINT
      signed char                     m_dataSTINYINT;     // SQL_C_STINYINT                   SQL_TINYINT
      unsigned char                   m_dataUTINYINT;     // SQL_C_UTINYINT                   SQL_UTINYINT
      SQLBIGINT                       m_dataSBIGINT;      // SQL_C_SBIGINT                    SQL_BIGINT
      SQLUBIGINT                      m_dataUBIGINT;      // SQL_C_UBIGINT                    SQL_UBIGINT
      SQL_NUMERIC_STRUCT              m_dataNUMERIC;      // SQL_C_NUMERIC                    SQL_NUMERIC
      SQLGUID                         m_dataGUID;         // SQL_C_GUID                       SQL_GUID
      DATE_STRUCT                     m_dataDATE;         // SQL_C_DATE                       SQL_DATE
      TIME_STRUCT                     m_dataTIME;         // SQL_C_TIME                       SQL_TIME
      TIMESTAMP_STRUCT                m_dataTIMESTAMP;    // SQL_C_TIMESTAMP                  SQL_TIMESTAMP
      DATE_STRUCT                     m_dataTYPEDATE;     // SQL_C_TYPE_DATE                  SQL_TYPE_DATE
      TIME_STRUCT                     m_dataTYPETIME;     // SQL_C_TYPE_TIME                  SQL_TYPE_TIME
      TIMESTAMP_STRUCT                m_dataTYPETIMESTAMP;// SQL_C_TYPE_TIMESTAMP             SQL_TYPE_TIMESTAMP
      SQL_INTERVAL_STRUCT             m_dataINTERVAL;     // SQL_C_INTERVAL_DAY               SQL_INTERVAL_DAY
                                                          // SQL_C_INTERVAL_DAY_TO_HOUR       SQL_INTERVAL_DAY_TO_HOUR
                                                          // SQL_C_INTERVAL_DAY_TO_MINUTE     SQL_INTERVAL_DAY_TO_MINUTE
                                                          // SQL_C_INTERVAL_DAY_TO_SECOND     SQL_INTERVAL_DAY_TO_SECOND
                                                          // SQL_C_INTERVAL_YEAR              SQL_INTERVAL_YEAR
                                                          // SQL_C_INTERVAL_YEAR_TO_MONTH     SQL_INTERVAL_YEAR_TO_MONTH
                                                          // SQL_C_INTERVAL_MONTH             SQL_INTERVAL_MONTH
                                                          // SQL_C_INTERVAL_HOUR              SQL_INTERVAL_HOUR
                                                          // SQL_C_INTERVAL_HOUR_TO_MINUTE    SQL_INTERVAL_HOUR_TO_MINUTE
                                                          // SQL_C_INTERVAL_HOUR_TO_SECOND    SQL_INTERVAL_HOUR_TO_SECOND
                                                          // SQL_C_INTERVAL_MINUTE            SQL_INTERVAL_MINUTE
                                                          // SQL_C_INTERVAL_MINUTE_TO_SECOND  SQL_INTERVAL_MINUTE_TO_SECOND
                                                          // SQL_C_INTERVAL_SECOND            SQL_INTERVAL_SECOND
   }
   m_data;
};

inline bool
SQLVariant::GetAtExec() const
{
  return m_useAtExec;
}

inline void
SQLVariant::SetAtExec(bool p_atExec)
{
  m_useAtExec = p_atExec;
}

inline int
SQLVariant::GetBinaryPieceSize() const
{
  return m_binaryPieceSize;
}

inline int
SQLVariant::GetBinaryLength() const
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
SQLVariant::GetColumnNumber() const
{
  return m_columnNumber;
}

inline void
SQLVariant::SetColumnNumber(int p_column)
{
  m_columnNumber = p_column;
}

inline SQLParamType
SQLVariant::GetParameterType() const
{
  return m_paramType;
}

inline int     
SQLVariant::GetDataType() const
{
  return m_datatype;
}

inline bool
SQLVariant::IsNULL() const
{
  return (m_indicator == SQL_NULL_DATA);
}

inline int
SQLVariant::GetSQLDataType() const
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
