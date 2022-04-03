// //////////////////////////////////////////////////////////////////////
//
// File: SQLError.cpp
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
#include "stdafx.h"
#include "SQLComponents.h"
#include "SQLError.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{

SQLState sql_states[] =
{
  { "00000" ,"Successful completion"
            ,"Technically not an error code, no reason to call the error routines" },
  { "01000" ,"General warning"
            ,"Driver-specific informational message. (Function returns SQL_SUCCESS_WITH_INFO.)"  },
  { "01001" ,"Cursor operation conflict"
            ,"The prepared statement associated with the StatementHandle contained a positioned update or delete statement, and no rows or more than one row were updated or deleted. (For more information about updates to more than one row, see the description of the SQL_ATTR_SIMULATE_CURSOR Attribute in SQLSetStmtAttr.)\n"
             "(Function returns SQL_SUCCESS_WITH_INFO.)" },
  { "01002" ,"Disconnect error"
            ,"An error occurred during the disconnect. However, the disconnect succeeded. (Function returns SQL_SUCCESS_WITH_INFO.)" },
  { "01003" ,"NULL value eliminated in set function"
            ,"The prepared statement associated with StatementHandle contained a set function (such as AVG, MAX, MIN, and so on), but not the COUNT set function, and NULL argument values were eliminated before the function was applied. (Function returns SQL_SUCCESS_WITH_INFO.)" },
  { "01004" ,"String data, right truncated"
            ,"The buffer *CharacterAttributePtr was not large enough to return the entire string value, so the string value was truncated. The length of the untruncated string value is returned in *StringLengthPtr. (Function returns SQL_SUCCESS_WITH_INFO.)" },
  { "01005" ,"insufficient item descriptor areas"
            ,"Driver did not allocate enough space for item descriptors" },
  { "01006" ,"Privilege not revoked"
            ,"The prepared statement associated with the StatementHandle was a REVOKE statement, and the user did not have the specified privilege. (Function returns SQL_SUCCESS_WITH_INFO.)" },
  { "01007" ,"Privilege not granted"
            ,"The prepared statement associated with the StatementHandle was a GRANT statement, and the user could not be granted the specified privilege." },
  { "01008" ,"implicit zero-bit padding"
            ,"" },
  { "01009" ,"search condition too long for info schema"
            ,"" },
  { "0100A" ,"query expression too long for info schema "
            ,"" },
  { "01S00" ,"Invalid connection string attribute"
            ,"An invalid attribute keyword was specified in the connection string (InConnectionString), but the driver was able to connect to the data source anyway. (Function returns SQL_SUCCESS_WITH_INFO.)"},
  { "01S01" ,"Error in row"
            ,"An error occurred while fetching one or more rows.\n"
             "(If this SQLSTATE is returned when an ODBC 3.x application is working with an ODBC 2.x driver, it can be ignored.)" },
  { "01S02" ,"Option value changed"
            ,"The driver did not support the specified value pointed to by the ValuePtr argument in SQLSetConnectAttr and substituted a similar value. (Function returns SQL_SUCCESS_WITH_INFO.)" },
  { "01S07" ,"Fractional truncation"
            ,"The data returned for an input/output or output parameter was truncated such that the fractional part of a numeric data type was truncated or the fractional portion of the time component of a time, timestamp, or interval data type was truncated.\n"
             "(Function returns SQL_SUCCESS_WITH_INFO.)" },
  { "01S08" ,"Error saving file DSN"
            ,"The string in *InConnectionString contained a FILEDSN keyword, but the .dsn file was not saved. (Function returns SQL_SUCCESS_WITH_INFO.)" },
  { "01S09" ,"Invalid keyword"
            ,"(DM) The string in *InConnectionString contained a SAVEFILE keyword but not a DRIVER or a FILEDSN keyword. (Function returns SQL_SUCCESS_WITH_INFO.)" },
  { "02000" ,"No data"
            ,"" },
  { "07000" ,"Dynamic SQL error"
            ,"" },
  { "07001" ,"Using clause does not match parameter specs"
            ,"" },
  { "07002" ,"COUNT field incorrect"
            ,"The number of parameters specified in SQLBindParameter was less than the number of parameters in the SQL statement contained in *StatementText.\n"
             "SQLBindParameter was called with ParameterValuePtr set to a null pointer, StrLen_or_IndPtr not set to SQL_NULL_DATA or SQL_DATA_AT_EXEC, and InputOutputType not set to SQL_PARAM_OUTPUT, so that the number of parameters specified in SQLBindParameter was greater than the number of parameters in the SQL statement contained in *StatementText." },
  { "07003" ,"Cursor specification cannot be executed"
            ,"" },
  { "07004" ,"Using clause required for dynamic parameters"
            ,"" },
  { "07005" ,"Prepared statement not a cursor-specification"
            ,"The statement associated with the StatementHandle did not return a result set and FieldIdentifier was not SQL_DESC_COUNT. There were no columns to describe." },
  { "07006" ,"Restricted data type attribute violation"
            ,"The data value identified by the ValueType argument in SQLBindParameter for the bound parameter could not be converted to the data type identified by the ParameterType argument in SQLBindParameter.\n"
             "The data value returned for a parameter bound as SQL_PARAM_INPUT_OUTPUT or SQL_PARAM_OUTPUT could not be converted to the data type identified by the ValueType argument in SQLBindParameter.\n"
             "(If the data values for one or more rows could not be converted but one or more rows were successfully returned, this function returns SQL_SUCCESS_WITH_INFO.)" },
  { "07007" ,"Restricted parameter value violation"
            ,"The parameter type SQL_PARAM_INPUT_OUTPUT_STREAM is only used for a parameter that sends and receives data in parts. An input bound buffer is not allowed for this parameter type.\n"
             "This error will occur when the parameter type is SQL_PARAM_INPUT_OUTPUT, and when the *StrLen_or_IndPtr specified in SQLBindParameter is not equal to SQL_NULL_DATA, SQL_DEFAULT_PARAM, SQL_LEN_DATA_AT_EXEC(len), or SQL_DATA_AT_EXEC." },
  { "07008" ,"Invalid descriptor count"
            ,"" },
  { "07009" ,"Invalid descriptor index"
            ,"(DM) The value specified for ColumnNumber was equal to 0, and the SQL_ATTR_USE_BOOKMARKS statement attribute was SQL_UB_OFF.\n"
             "The value specified for the argument ColumnNumber was greater than the number of columns in the result set." },
  { "07S01" ,"Invalid use of default parameter"
            ,"A parameter value, set with SQLBindParameter, was SQL_DEFAULT_PARAM, and the corresponding parameter was not a parameter for an ODBC canonical procedure invocation." },
  { "08000" ,"Connection exception"
            ,"" },
  { "08001" ,"Client unable to establish connection"
            ,"The driver was unable to establish a connection with the data source." },
  { "08002" ,"Connection name in use"
            ,"(DM) The specified ConnectionHandle had already been used to establish a connection with a data source, and the connection was still open." },
  { "08003" ,"Connection not open"
            ,"(DM) The connection specified in the argument ConnectionHandle was not open." },
  { "08004" ,"Server rejected the connection"
            ,"The data source rejected the establishment of the connection for implementation-defined reasons." },
  { "08006" ,"Connection failure"
            ,"" },
  { "08007" ,"Connection failure during transaction"
            ,"The HandleType was SQL_HANDLE_DBC, and the connection associated with the Handle failed during the execution of the function, and it cannot be determined whether the requested COMMIT or ROLLBACK occurred before the failure." },
  { "08S01" ,"Communication link failure"
            ,"The communication link between the driver and the data source to which the driver was attempting to connect failed before the SQLDriverConnect function completed processing." },
  { "0A000" ,"Feature not supported"
            ,"" },
  { "0A001" ,"Error in multi-server transaction"
            ,"" },
  { "21000" ,"Cardinality violation"
            ,"" },
  { "21S01" ,"Insert value list does not match column list"
            ,"*StatementText contained an INSERT statement, and the number of values to be inserted did not match the degree of the derived table." },
  { "21S02" ,"Degree of derived table does not match column list"
            ,"*StatementText contained a CREATE VIEW statement, and the number of names specified is not the same degree as the derived table defined by the query specification." },
  { "22000" ,"Data exception"
            ,"" },
  { "22001" ,"String data, right truncation"
            ,"The assignment of a character or binary value to a column resulted in the truncation of nonblank (character) or non-null (binary) characters or bytes." },
  { "22002" ,"Indicator variable required but not supplied"
            ,"NULL data was bound to an output parameter whose StrLen_or_IndPtr set by SQLBindParameter was a null pointer." },
  { "22003" ,"Numeric value out of range"
            ,"The prepared statement associated with the StatementHandle contained a bound numeric parameter, and the parameter value caused the whole (as opposed to fractional) part of the number to be truncated when assigned to the associated table column.\n"
             "Returning a numeric value (as numeric or string) for one or more input/output or output parameters would have caused the whole (as opposed to fractional) part of the number to be truncated." },
  { "22005" ,"Error in assignment"
            ,"" },
  { "22007" ,"Invalid datetime format"
            ,"The prepared statement associated with the StatementHandle contained an SQL statement that contained a date, time, or timestamp structure as a bound parameter, and the parameter was, respectively, an invalid date, time, or timestamp.\n"
             "An input/output or output parameter was bound to a date, time, or timestamp C structure, and a value in the returned parameter was, respectively, an invalid date, time, or timestamp. (Function returns SQL_SUCCESS_WITH_INFO.)" },
  { "22008" ,"Datetime field overflow"
            ,"The prepared statement associated with the StatementHandle contained an SQL statement that contained a datetime expression that, when computed, resulted in a date, time, or timestamp structure that was invalid.\n"
             "A datetime expression computed for an input/output or output parameter resulted in a date, time, or timestamp C structure that was invalid." },
  { "22009" ,"Invalid time zone displacement value"
            ,"" },
  { "22011" ,"Substring error"
            ,"" },
  { "22012" ,"Division by zero"
            ,"The prepared statement associated with the StatementHandle contained an arithmetic expression that caused division by zero.\n"
             "An arithmetic expression calculated for an input/output or output parameter resulted in division by zero." },
  { "22015" ,"Interval field overflow"
            ,"*StatementText contained an exact numeric or interval parameter that, when converted to an interval SQL data type, caused a loss of significant digits.\n"
             "*StatementText contained an interval parameter with more than one field that, when converted to a numeric data type in a column, had no representation in the numeric data type.\n"
             "*StatementText contained parameter data that was assigned to an interval SQL type, and there was no representation of the value of the C type in the interval SQL type.\n"
             "Assigning an input/output or output parameter that was an exact numeric or interval SQL type to an interval C type caused a loss of significant digits.\n"
             "When an input/output or output parameter was assigned to an interval C structure, there was no representation of the data in the interval data structure." },
  { "22018" ,"Invalid character value for cast specification"
            ,"*StatementText contained an SQL statement that contained a literal or parameter, and the value was incompatible with the data type of the associated table column." },
  { "22019" ,"Invalid escape character"
            ,"The argument StatementText contained a LIKE predicate with an ESCAPE in the WHERE clause, and the length of the escape character following ESCAPE was not equal to 1." },
  { "22021" ,"Character not in repertoire"
            ,"" },
  { "22022" ,"Indicator overflow"
            ,"" },
  { "22023" ,"Invalid parameter value"
            ,""},
  { "22024" ,"Unterminated C String"
            ,"" },
  { "22025" ,"Invalid escape sequence"
            ,"The argument StatementText contained \"LIKE pattern value ESCAPE escape character\" in the WHERE clause, and the character following the escape character in the pattern value was neither \"%\" nor \"_\"." },
  { "22026" ,"String data - length mismatch"
            ,"" },
  { "22027" ,"Trim error"
            ,"" },
  { "23000" ,"Integrity constraint violation"
            ,"The prepared statement associated with the StatementHandle contained a parameter. The parameter value was NULL for a column defined as NOT NULL in the associated table column, a duplicate value was supplied for a column constrained to contain only unique values, or some other integrity constraint was violated.\n" },
  { "24000" ,"Invalid cursor state"
            ,"The Attribute argument was SQL_ATTR_CURRENT_CATALOG, and a result set was pending." },
  { "25000" ,"Invalid transaction state"
            ,"There was a transaction in process on the connection specified by the argument ConnectionHandle. The transaction remains active." },
  { "25S01" ,"Transaction state unknown"
            ,"One or more of the connections in Handle failed to complete the transaction with the outcome specified, and the outcome is unknown." },
  { "25S02" ,"Transaction is still active"
            ,"The driver was not able to guarantee that all work in the global transaction could be completed atomically, and the transaction is still active." },
  { "25S03" ,"Transaction is rolled back"
            ,"The driver was not able to guarantee that all work in the global transaction could be completed atomically, and all work in the transaction active in Handle was rolled back." },
  { "28000" ,"Invalid authorization specification"
            ,"Either the user identifier or the authorization string, or both, as specified in the connection string (InConnectionString), violated restrictions defined by the data source." },
  { "3D000" ,"Invalid catalog name"
            ,"The Attribute argument was SQL_CURRENT_CATALOG, and the specified catalog name was invalid." },
  { "34000" ,"Invalid cursor name"
            ,"*StatementText contained a positioned DELETE or a positioned UPDATE, and the cursor referenced by the statement being prepared was not open." },
  { "3D000" ,"Invalid catalog name"
            ,"The catalog name specified in StatementText was invalid." },
  { "3F000" ,"Invalid schema name"
            ,"The schema name specified in StatementText was invalid." },
  { "40001" ,"Serialization failure"
            ,"The transaction was rolled back due to a resource deadlock with another transaction." },
  { "40002" ,"Integrity constraint violation"
            ,"The CompletionType was SQL_COMMIT, and the commitment of changes caused integrity constraint violation. As a result, the transaction was rolled back." },
  { "40003" ,"Statement completion unknown"
            ,"The associated connection failed during the execution of this function, and the state of the transaction cannot be determined." },
  { "42000" ,"Syntax error or access violation"
            ,"*StatementText contained an SQL statement that was not preparable or contained a syntax error.\n"\
             "*StatementText contained a statement for which the user did not have the required privileges." },
  { "42S01" ,"Base table or view already exists"
            ,"*StatementText contained a CREATE TABLE or CREATE VIEW statement, and the table name or view name specified already exists." },
  { "42S02" ,"Base table or view not found"
            ,"*StatementText contained a DROP TABLE or a DROP VIEW statement, and the specified table name or view name did not exist.\n"
             "*StatementText contained an ALTER TABLE statement, and the specified table name did not exist.\n"
             "*StatementText contained a CREATE VIEW statement, and a table name or view name defined by the query specification did not exist.\n"
             "*StatementText contained a CREATE INDEX statement, and the specified table name did not exist.\n"
             "*StatementText contained a GRANT or REVOKE statement, and the specified table name or view name did not exist.\n"
             "*StatementText contained a SELECT statement, and a specified table name or view name did not exist.\n"
             "*StatementText contained a DELETE, INSERT, or UPDATE statement, and the specified table name did not exist.\n"
             "*StatementText contained a CREATE TABLE statement, and a table specified in a constraint (referencing a table other than the one being created) did not exist." },
  { "42S11" ,"Index already exists"
            ,"*StatementText contained a CREATE INDEX statement, and the specified index name already existed." },
  { "42S12" ,"Index not found"
            ,"*StatementText contained a DROP INDEX statement, and the specified index name did not exist." },
  { "42S21" ,"Column already exists"
            ,"*StatementText contained an ALTER TABLE statement, and the column specified in the ADD clause is not unique or identifies an existing column in the base table." },
  { "42S22" ,"Column not found"
            ,"*StatementText contained a CREATE INDEX statement, and one or more of the column names specified in the column list did not exist.\n"
             "*StatementText contained a GRANT or REVOKE statement, and a specified column name did not exist.\n"
             "*StatementText contained a SELECT, DELETE, INSERT, or UPDATE statement, and a specified column name did not exist.\n"
             "*StatementText contained a CREATE TABLE statement, and a column specified in a constraint (referencing a table other than the one being created) did not exist." },
  { "44000" ,"WITH CHECK OPTION violation"
            ,"The prepared statement associated with StatementHandle contained an INSERT statement performed on a viewed table or a table derived from the viewed table that was created by specifying WITH CHECK OPTION, such that one or more rows affected by the INSERT statement will no longer be present in the viewed table.\n"
             "The prepared statement associated with the StatementHandle contained an UPDATE statement performed on a viewed table or a table derived from the viewed table that was created by specifying WITH CHECK OPTION, such that one or more rows affected by the UPDATE statement will no longer be present in the viewed table." },
  { "HY000" ,"General error"
            ,"An error occurred for which there was no specific SQLSTATE and for which no implementation-specific SQLSTATE was defined. The error message returned by SQLGetDiagField from the diagnostic data structure describes the error and its cause" },
  { "HY001" ,"Memory allocation error"
            ,"The driver was unable to allocate memory required to support execution or completion of the function." },
  { "HY003" ,"Invalid application buffer type"
            ,"The argument TargetType was neither a valid data type nor SQL_C_DEFAULT." },
  { "HY004" ,"Invalid SQL data type"
            ,"The value specified for the argument ParameterType was neither a valid ODBC SQL data type identifier nor a driver-specific SQL data type identifier supported by the driver." },
  { "HY008" ,"Operation canceled"
            ,"Asynchronous processing was enabled for the StatementHandle. The function was called, and before it completed execution, SQLCancel or SQLCancelHandle was called on the StatementHandle. Then the function was called again on the StatementHandle.\n"
             "The function was called, and before it completed execution, SQLCancel or SQLCancelHandle was called on the StatementHandle from a different thread in a multithread application." },
  { "HY009" ,"Invalid use of null pointer"
            ,"(DM) The OutputHandlePtr argument was a null pointer." },
  { "HY010" ,"Function sequence error"
            ,"(DM) An asynchronously executing function was called for the connection handle that is associated with the StatementHandle. This aynchronous function was still executing when SQLColAttribute was called.\n"
             "(DM) SQLExecute, SQLExecDirect, or SQLMoreResults was called for the StatementHandle and returned SQL_PARAM_DATA_AVAILABLE. This function was called before data was retrieved for all streamed parameters.\n"
             "(DM) The function was called prior to calling SQLPrepare, SQLExecDirect, or a catalog function for the StatementHandle.\n"
             "(DM) An asynchronously executing function (not this one) was called for the StatementHandle and was still executing when this function was called.\n"
             "(DM) SQLExecute, SQLExecDirect, SQLBulkOperations, or SQLSetPos was called for the StatementHandle and returned SQL_NEED_DATA. This function was called before data was sent for all data-at-execution parameters or columns." },
  { "HY011" ,"Attribute cannot be set now"
            ,"The Attribute argument was SQL_ATTR_TXN_ISOLATION, and a transaction was open." },
  { "HY013" ,"Memory management error"
            ,"The function call could not be processed because the underlying memory objects could not be accessed, possibly because of low memory conditions." },
  { "HY017" ,"Invalid use of an automatically allocated descriptor handle."
            ,"(DM) The Handle argument was set to the handle for an automatically allocated descriptor." },
  { "HY018" ,"Server declined cancel request"
            ,"The server declined the cancel request." },
  { "HY019" ,"Non-character and non-binary data sent in pieces"
            ,"SQLPutData was called more than once for a parameter or column, and it was not being used to send character C data to a column with a character, binary, or data source–specific data type or to send binary C data to a column with a character, binary, or data source–specific data type." },
  { "HY020" ,"Attempt to concatenate a null value"
            ,"SQLPutData was called more than once since the call that returned SQL_NEED_DATA, and in one of those calls, the StrLen_or_Ind argument contained SQL_NULL_DATA or SQL_DEFAULT_PARAM." },
  { "HY021" ,"Inconsistent descriptor information"
            ,"The descriptor information checked during a consistency check was not consistent. (See the \"Consistency Checks\" section in SQLSetDescField.)\n"
             "The value specified for the argument DecimalDigits was outside the range of values supported by the data source for a column of the SQL data type specified by the ParameterType argument." },
  { "HY024" ,"Invalid attribute value"
            ,"(DM) The InfoType argument was SQL_DRIVER_HSTMT, and the value pointed to by InfoValuePtr was not a valid statement handle.\n"
             "(DM) The InfoType argument was SQL_DRIVER_HDESC, and the value pointed to by InfoValuePtr was not a valid descriptor handle." },
  { "HY090" ,"Invalid string or buffer length"
            ,"(DM) *CharacterAttributePtr is a character string, and BufferLength was less than 0 but not equal to SQL_NTS." },
  { "HY091" ,"Invalid descriptor field identifier"
            ,"The value specified for the argument FieldIdentifier was not one of the defined values and was not an implementation-defined value." },
  { "HY092" ,"Invalid attribute/option identifier"
            ,"(DM) The DriverCompletion argument was SQL_DRIVER_PROMPT, and the WindowHandle argument was a null pointer." },
  { "HY095" ,"Function type out of range"
            ,"(DM) An invalid FunctionId value was specified." },
  { "HY096" ,"Information type out of range"
            ,"The value specified for the argument InfoType was not valid for the version of ODBC supported by the driver." },
  { "HY103" ,"Invalid retrieval code"
            ,"(DM) The value specified for the argument Direction was not equal to SQL_FETCH_FIRST, SQL_FETCH_FIRST_USER, SQL_FETCH_FIRST_SYSTEM, or SQL_FETCH_NEXT." },
  { "HY104" ,"Invalid precision or scale value"
            ,"The value specified for the argument ColumnSize or DecimalDigits was outside the range of values supported by the data source for a column of the SQL data type specified by the ParameterType argument." },
  { "HY105" ,"Invalid parameter type"
            ,"The value specified for the argument InputOutputType in SQLBindParameter was SQL_PARAM_OUTPUT, and the parameter was an input parameter." },
  { "HY107" ,"Row value out of range"
            ,"The value specified with the SQL_ATTR_CURSOR_TYPE statement attribute was SQL_CURSOR_KEYSET_DRIVEN, but the value specified with the SQL_ATTR_KEYSET_SIZE statement attribute was greater than 0 and less than the value specified with the SQL_ATTR_ROW_ARRAY_SIZE statement attribute." },
  { "HY109" ,"Invalid cursor position"
            ,"The prepared statement was a positioned update or delete statement, and the cursor was positioned (by SQLSetPos or SQLFetchScroll) on a row that had been deleted or could not be fetched." },
  { "HY110" ,"Invalid driver completion"
            ,"(DM) The value specified for the argument DriverCompletion was not equal to SQL_DRIVER_PROMPT, SQL_DRIVER_COMPLETE, SQL_DRIVER_COMPLETE_REQUIRED, or SQL_DRIVER_NOPROMPT.\n"
             "(DM) Connection pooling was enabled, and the value specified for the argument DriverCompletion was not equal to SQL_DRIVER_NOPROMPT." },
  { "HY114" ,"Driver does not support connection-level asynchronous function execution"
            ,"(DM) An application attempted to enable asynchronous function execution with SQL_ATTR_ASYNC_DBC_FUNCTIONS_ENABLE for a driver that does not support asynchronous connection operations." },
  { "HY117" ,"Connection is suspended due to unknown transaction state. Only disconnect and read-only functions are allowed."
            ,"(DM) For more information about suspended state, see SQLEndTran Function." },
  { "HY121" ,"Cursor Library and Driver-Aware Pooling cannot be enabled at the same time"
            ,"For more information, see Driver-Aware Connection Pooling." },
  { "HYC00" ,"Driver not capable"
            ,"The value specified for the argument FieldIdentifier was not supported by the driver." },
  { "HYT00" ,"Timeout expired"
            ,"The login timeout period expired before the connection to the data source completed. The timeout period is set through SQLSetConnectAttr, SQL_ATTR_LOGIN_TIMEOUT." },
  { "HYT01" ,"Connection timeout expired"
            ,"The connection timeout period expired before the data source responded to the request. The connection timeout period is set through SQLSetConnectAttr, SQL_ATTR_CONNECTION_TIMEOUT." },
  { "IM001" ,"Driver does not support this function"
            ,"(DM) The driver associated with the StatementHandle does not support the function." },
  { "IM002" ,"Data source not found and no default driver specified"
            ,"(DM) The data source name specified in the connection string (InConnectionString) was not found in the system information, and there was no default driver specification.\n"
             "(DM) ODBC data source and default driver information could not be found in the system information." },
  { "IM003" ,"Specified driver could not be loaded"
            ,"(DM) The driver listed in the data source specification in the system information or specified by the DRIVER keyword was not found or could not be loaded for some other reason." },
  { "IM004" ,"Driver's SQLAllocHandle on SQL_HANDLE_ENV failed"
            ,"(DM) During SQLDriverConnect, the Driver Manager called the driver's SQLAllocHandle function with an fHandleType of SQL_HANDLE_ENV and the driver returned an error." },
  { "IM005" ,"Driver's SQLAllocHandle on SQL_HANDLE_DBC failed."
            ,"(DM) During SQLDriverConnect, the Driver Manager called the driver's SQLAllocHandle function with an fHandleType of SQL_HANDLE_DBC and the driver returned an error." },
  { "IM006" ,"Driver's SQLSetConnectAttr failed"
            ,"(DM) During SQLDriverConnect, the Driver Manager called the driver's SQLSetConnectAttr function and the driver returned an error." },
  { "IM007" ,"No data source or driver specified; dialog prohibited"
            ,"No data source name or driver was specified in the connection string, and DriverCompletion was SQL_DRIVER_NOPROMPT." },
  { "IM008" ,"Dialog failed"
            ,"The driver attempted to display its login dialog box and failed.\n"
             "WindowHandle was a null pointer, and DriverCompletion was not SQL_DRIVER_NO_PROMPT." },
  { "IM009" ,"Unable to load translation DLL"
            ,"The driver was unable to load the translation DLL that was specified for the data source or for the connection." },
  { "IM010" ,"Data source name too long"
            ,"(DM) The attribute value for the DSN keyword was longer than SQL_MAX_DSN_LENGTH characters." },
  { "IM011" ,"Driver name too long"
            ,"(DM) The attribute value for the DRIVER keyword was longer than 255 characters." },
  { "IM012" ,"DRIVER keyword syntax error"
            ,"(DM) The keyword-value pair for the DRIVER keyword contained a syntax error.\n"
             "(DM) The string in *InConnectionString contained a FILEDSN keyword, but the .dsn file did not contain a DRIVER keyword or a DSN keyword." },
  { "IM014" ,"The specified DSN contains an architecture mismatch between the Driver and Application"
            ,"(DM) 32-bit application uses a DSN connecting to a 64-bit driver; or vice versa." },
  { "IM015" ,"Driver's SQLDriverConnect on SQL_HANDLE_DBC_INFO_HANDLE failed"
            ,"If a driver returns SQL_ERROR, the Driver Manager will return SQL_ERROR to the application and the connection will fail.\n"
             "For more information about SQL_HANDLE_DBC_INFO_TOKEN, see Developing Connection-Pool Awareness in an ODBC Driver." },
  { "IM017" ,"Polling is disabled in asynchronous notification mode"
            ,"Whenever the notification model is used, polling is disabled." },
  { "IM018" ,"SQLCompleteAsync has not been called to complete the previous asynchronous operation on this handle."
            ,"If the previous function call on the handle returns SQL_STILL_EXECUTING and if notification mode is enabled, SQLCompleteAsync must be called on the handle to do post-processing and complete the operation." },
  { "S1118" ,"Driver does not support asynchronous notification"
            ,"When the driver does not support asynchronous notification, you cannot set SQL_ATTR_ASYNC_DBC_EVENT or SQL_ATTR_ASYNC_DBC_RETCODE_PTR."}

};

SQLError::SQLError()
{
  m_lastState = NULL;
}

SQLError::~SQLError()
{
}

SQLState*
SQLError::GetSQLState(XString p_sqlState)
{
  int size = sizeof(sql_states) / sizeof(SQLState);
  
  for(int ind = 0;ind < size; ++ind)
  {
    if(sql_states[ind].m_code.CompareNoCase(p_sqlState) == 0)
    {
      return (m_lastState = &sql_states[ind]);
    }
  }
  return NULL;
}

XString   
SQLError::GetSQLStateFormatted(XString p_sqlState)
{
  XString error;
  SQLState* state = GetSQLState(p_sqlState);
  if(state == NULL)
  {
    return error;
  }
  error.Format("SQLSTATE : %s\n"
               "Text     : %s\n"
               "---------------------\n"
               "%s"
              ,state->m_code.GetString()
              ,state->m_error.GetString()
              ,state->m_explanation.GetString());
  return error;
}

// End of namespace
}
