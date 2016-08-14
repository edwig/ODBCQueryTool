////////////////////////////////////////////////////////////////////////
//
// File: SQLVariant.h
//
// Copyright (c) 1998- 2014 ir. W.E. Huisman
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
// Last Revision:   01-01-2015
// Version number:  1.1.0
//
#pragma  once
#include <sqlext.h>

// Truncation types in SQL_NUMERIC_STRUCT conversions
#define SQLNUM_TRUNC_FRAC  1
#define SQLNUM_TRUNC_WHOLE 2
// Max length of a NUMERIC in a char buffer
#define SQLNUM_MAXLEN      65

typedef struct _types
{
  char*  name;
  int    type;
}
DataTypes;

// All registrations of type names
extern DataTypes allTypes[];
extern DataTypes allOther[];
extern DataTypes allParams[];

// Forewared declarations
class SQLDate;
class SQLTime;
class SQLTimestamp;

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
   SQLVariant(SQLVariant* p_var);             // From another SQLVariant
   // Type constructors
   SQLVariant(const char* p_data);            // SQL_C_CHAR
   SQLVariant(CString& p_data);               // SQL_C_CHAR
   SQLVariant(short p_short);                 // SQL_C_SHORT / SQL_C_SSHORT
   SQLVariant(unsigned short p_short);        // SQL_C_USHORT
   SQLVariant(long p_long);                   // SQL_C_LONG / SQL_C_SLONG
   SQLVariant(unsigned long p_long);          // SQL_C_ULONG
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
   SQLVariant(int p_type,SQL_INTERVAL_STRUCT* p_inter);  // SQL_C_INTERVAL_YEAR -> SQL_C_INTERVAL_DAY_TO_SECOND
   // From complex constructors
   SQLVariant(SQLDate* p_date);               // SQLDate
   SQLVariant(SQLTime* p_time);               // SQLTime
   SQLVariant(SQLTimestamp* p_stamp);         // SQLTimestamp
   // Destructor
  ~SQLVariant();
   void Init(); // Init empty

   // STATUS
   bool    IsNULL();
   bool    IsEmpty();
   bool    IsNumericType();
   bool    IsIntervalType();
   bool    IsDateTimeType();
   // INFO about type names/values
   int     FindDatatype   (char* p_type);
   char*   FindDatatype   (int   p_type);
   int     FindParamtype  (char* p_type);
   char*   FindParamtype  (int   p_type);
   int     FindSQLDatatype(char* p_type);
   char*   FindSQLDatatype(int   p_type);
   int     FindDataTypeFromSQLType();

   // GETTERS
   int     GetDataType();
   int     GetDataSize();
   int     GetSQLDataType();
   bool    GetAtExec();
   int     GetBinaryPieceSize();
   SQLLEN* GetIndicatorPointer();
   int     GetColumnNumber();
   int     GetParameterType();
   int     GetFraction();
   // SETTERS
   void    SetSQLDataType(int p_type);
   void    SetAtExec(bool p_atExec);
   void    SetBinaryPieceSize(int p_size);
   void    SetColumnNumber(int p_column);
   void    SetParameterType(int p_type);
   void    SetSizeIndicator(bool p_realSize);
   void    SetNULL();
   void    ReserveSpace(int p_type,int p_space);

   // General access
   bool                 SetData(int p_type,const char* p_data);
   bool                 SetBinary(int p_length,void* p_data);
   void*                GetDataPointer();
   // Access per type
   char*                GetAsChar();
   void                 GetAsString(CString& p_result);
   void*                GetAsBinary();
   bool                 GetAsBoolean();
   short                GetAsSShort();
   unsigned short       GetAsUShort();
   long                 GetAsSLong();
   unsigned long        GetAsULong();
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
   CString              GetAsEuropeanTimestamp();
   SQL_INTERVAL_STRUCT* GetAsInterval();
   // Access per complex type
   SQLDate              GetAsSQLDate();
   SQLTime              GetAsSQLTime();
   SQLTimestamp         GetAsSQLTimestamp();

   // General operators
   SQLVariant& operator  =(const SQLVariant& p_origineel);
   bool        operator ==(const SQLVariant& p_right);
private:
   // Total internal reset (type and data store)
   void    ResetDataType(int p_type);
   // Internal conversions
   bool    StringToBinary (const char* p_data);
   bool    BinaryToString (unsigned char* buffer,int buflen);
   void    StringToNumeric(const char* p_data,SQL_NUMERIC_STRUCT* p_numeric);
   void    NumericToString(SQL_NUMERIC_STRUCT* p_numeric,CString& p_string);
   void    StringToNumericMantissa(const char*         numstr
                                  ,SQL_NUMERIC_STRUCT* sqlnum
                                  ,int*                overflow_ptr);
   void    NumericToStringMantissa(SQL_NUMERIC_STRUCT* sqlnum
                                  ,SQLCHAR*            numstr
                                  ,SQLCHAR**           numbegin
                                  ,SQLCHAR             reqprec
                                  ,SQLSCHAR            reqscale
                                  ,int*                truncptr);
   // Internal conversion and trimming
   long           ULongToLong(unsigned long p_value);
   long           FloatToLong(float p_value);
   long           DoubleToLong(double p_value);
   long           BIGINTToLong(SQLBIGINT p_value);
   long           UBIGINTToLong(SQLUBIGINT p_value);
   short          UShortToShort(unsigned short p_value);
   short          SLongToShort(long p_value);
   short          ULongToShort(unsigned long p_value);
   short          FloatToShort(float p_value);
   short          DoubleToShort(double p_value);
   short          BIGINTToShort(SQLBIGINT p_value);
   short          UBIGINTToShort(SQLUBIGINT p_value);
   float          DoubleToFloat(double p_value);
   SQLBIGINT      FloatToBIGINT(float p_value);
   SQLBIGINT      DoubleToBIGINT(double p_value);
   SQLBIGINT      UBIGINTToBIGINT(SQLUBIGINT p_value);
   char           ShortToTinyInt(short p_value);
   char           UShortToTinyInt(unsigned short p_value);
   char           SLongToTinyInt(long p_value);
   char           ULongToTinyInt(long p_value);
   char           FloatToTinyInt(float p_value);
   char           DoubleToTinyInt(double p_value);
   char           UTinyIntToTinyInt(unsigned char p_value);
   char           SBIGINTToTinyInt(SQLBIGINT p_value);
   char           UBIGINTToTinyInt(SQLUBIGINT p_value);
   unsigned long  ShortToULong(short p_value);
   unsigned long  LongToULong(long p_value);
   unsigned long  FloatToULong(float p_value);
   unsigned long  DoubleToULong(double p_value);
   unsigned long  TinyIntToULong(char p_value);
   unsigned long  BIGINTToULong(SQLBIGINT p_value);
   unsigned long  UBIGINTToULong(SQLUBIGINT p_value);
   SQLUBIGINT     ShortToUBIGINT(short p_value);
   SQLUBIGINT     LongToUBIGINT(long p_value);
   SQLUBIGINT     FloatToUBIGINT(float p_value);
   SQLUBIGINT     DoubleToUBIGINT(double p_value);
   SQLUBIGINT     TinyIntToUBIGINT(char p_value);
   SQLUBIGINT     SBIGINTToUBIGINT(SQLBIGINT p_value);
   unsigned short ShortToUShort(short p_value);
   unsigned short SLongToUShort(long p_value);
   unsigned short ULongToUShort(unsigned long p_value);
   unsigned short FloatToUShort(float p_value);
   unsigned short DoubleToUShort(double p_value);
   unsigned short TinyIntToUShort(char p_value);
   unsigned short BIGINTToUShort(SQLBIGINT p_value);
   unsigned short UBIGINTToUShort(SQLUBIGINT p_value);
   unsigned char  ShortToUTinyInt(short p_value);
   unsigned char  UShortToUTinyInt(short p_value);
   unsigned char  SLongToUTinyInt(long p_value);
   unsigned char  ULongToUTinyInt(unsigned long p_value);
   unsigned char  FloatToUTinyInt(float p_value);
   unsigned char  DoubleToUTinyInt(double p_value);
   unsigned char  TinyIntToUTinyInt(char p_value);
   unsigned char  SBIGINTToUTinyInt(SQLBIGINT p_value);
   unsigned char  UBIGINTToUTinyInt(SQLUBIGINT p_value);

   // Throw error as a result of internal trimming
   void  ThrowErrorDatatype(int p_getas);
   void  ThrowErrorTruncate(int p_from,int p_to);

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
      unsigned char*                  m_dataVARBOOKMARK;  // SQL_C_VARBOOKMARK        SQL_VARBOOKMARK
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
      unsigned long                   m_dataBOOKMARK;     // SQL_C_BOOKMARK           SQL_BOOKMARK
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