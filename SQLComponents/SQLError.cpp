// //////////////////////////////////////////////////////////////////////
//
// File: SQLError.cpp
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
  { _T("00000") ,_T("Successful completion")
                ,_T("Technically not an error code, no reason to call the error routines") },
  { _T("01000") ,_T("General warning")
                ,_T("Driver-specific informational message. (Function returns SQL_SUCCESS_WITH_INFO.)")  },
  { _T("01001") ,_T("Cursor operation conflict")
                ,_T("The prepared statement associated with the StatementHandle contained a positioned update or delete statement, and no rows or more than one row were updated or deleted. (For more information about updates to more than one row, see the description of the SQL_ATTR_SIMULATE_CURSOR Attribute in SQLSetStmtAttr.)\n"
                    "(Function returns SQL_SUCCESS_WITH_INFO.)") },
  { _T("01002") ,_T("Disconnect error")
                ,_T("An error occurred during the disconnect. However, the disconnect succeeded. (Function returns SQL_SUCCESS_WITH_INFO.)") },
  { _T("01003") ,_T("NULL value eliminated in set function")
                ,_T("The prepared statement associated with StatementHandle contained a set function (such as AVG, MAX, MIN, and so on), but not the COUNT set function, and NULL argument values were eliminated before the function was applied. (Function returns SQL_SUCCESS_WITH_INFO.)") },
  { _T("01004") ,_T("String data, right truncated")
                ,_T("The buffer *CharacterAttributePtr was not large enough to return the entire string value, so the string value was truncated. The length of the untruncated string value is returned in *StringLengthPtr. (Function returns SQL_SUCCESS_WITH_INFO.)") },
  { _T("01005") ,_T("insufficient item descriptor areas")
                ,_T("Driver did not allocate enough space for item descriptors") },
  { _T("01006") ,_T("Privilege not revoked")
                ,_T("The prepared statement associated with the StatementHandle was a REVOKE statement, and the user did not have the specified privilege. (Function returns SQL_SUCCESS_WITH_INFO.)") },
  { _T("01007") ,_T("Privilege not granted")
                ,_T("The prepared statement associated with the StatementHandle was a GRANT statement, and the user could not be granted the specified privilege.") },
  { _T("01008") ,_T("implicit zero-bit padding")
                ,_T("") },
  { _T("01009") ,_T("search condition too long for info schema")
                ,_T("") },
  { _T("0100A") ,_T("query expression too long for info schema ")
                ,_T("") },
  { _T("01S00") ,_T("Invalid connection string attribute")
                ,_T("An invalid attribute keyword was specified in the connection string (InConnectionString), but the driver was able to connect to the data source anyway. (Function returns SQL_SUCCESS_WITH_INFO.)") },
  { _T("01S01") ,_T("Error in row")
                ,_T("An error occurred while fetching one or more rows.\n"
                    "(If this SQLSTATE is returned when an ODBC 3.x application is working with an ODBC 2.x driver, it can be ignored.)") },
  { _T("01S02") ,_T("Option value changed")
                ,_T("The driver did not support the specified value pointed to by the ValuePtr argument in SQLSetConnectAttr and substituted a similar value. (Function returns SQL_SUCCESS_WITH_INFO.)") },
  { _T("01S07") ,_T("Fractional truncation")
                ,_T("The data returned for an input/output or output parameter was truncated such that the fractional part of a numeric data type was truncated or the fractional portion of the time component of a time, timestamp, or interval data type was truncated.\n"
                    "(Function returns SQL_SUCCESS_WITH_INFO.)") },
  { _T("01S08") ,_T("Error saving file DSN")
                ,_T("The string in *InConnectionString contained a FILEDSN keyword, but the .dsn file was not saved. (Function returns SQL_SUCCESS_WITH_INFO.)") },
  { _T("01S09") ,_T("Invalid keyword")
                ,_T("(DM) The string in *InConnectionString contained a SAVEFILE keyword but not a DRIVER or a FILEDSN keyword. (Function returns SQL_SUCCESS_WITH_INFO.)") },
  { _T("02000") ,_T("No data")
                ,_T("") },
  { _T("07000") ,_T("Dynamic SQL error")
                ,_T("") },
  { _T("07001") ,_T("Using clause does not match parameter specs")
                ,_T("") },
  { _T("07002") ,_T("COUNT field incorrect")
                ,_T("The number of parameters specified in SQLBindParameter was less than the number of parameters in the SQL statement contained in *StatementText.\n"
                    "SQLBindParameter was called with ParameterValuePtr set to a null pointer, StrLen_or_IndPtr not set to SQL_NULL_DATA or SQL_DATA_AT_EXEC, and InputOutputType not set to SQL_PARAM_OUTPUT, so that the number of parameters specified in SQLBindParameter was greater than the number of parameters in the SQL statement contained in *StatementText.") },
  { _T("07003") ,_T("Cursor specification cannot be executed")
                ,_T("") },
  { _T("07004") ,_T("Using clause required for dynamic parameters")
                ,_T("") },
  { _T("07005") ,_T("Prepared statement not a cursor-specification")
                ,_T("The statement associated with the StatementHandle did not return a result set and FieldIdentifier was not SQL_DESC_COUNT. There were no columns to describe.") },
  { _T("07006") ,_T("Restricted data type attribute violation")
                ,_T("The data value identified by the ValueType argument in SQLBindParameter for the bound parameter could not be converted to the data type identified by the ParameterType argument in SQLBindParameter.\n"
                    "The data value returned for a parameter bound as SQL_PARAM_INPUT_OUTPUT or SQL_PARAM_OUTPUT could not be converted to the data type identified by the ValueType argument in SQLBindParameter.\n"
                    "(If the data values for one or more rows could not be converted but one or more rows were successfully returned, this function returns SQL_SUCCESS_WITH_INFO.)") },
  { _T("07007") ,_T("Restricted parameter value violation")
                ,_T("The parameter type SQL_PARAM_INPUT_OUTPUT_STREAM is only used for a parameter that sends and receives data in parts. An input bound buffer is not allowed for this parameter type.\n"
                    "This error will occur when the parameter type is SQL_PARAM_INPUT_OUTPUT, and when the *StrLen_or_IndPtr specified in SQLBindParameter is not equal to SQL_NULL_DATA, SQL_DEFAULT_PARAM, SQL_LEN_DATA_AT_EXEC(len), or SQL_DATA_AT_EXEC.") },
  { _T("07008") ,_T("Invalid descriptor count")
                ,_T("") },
  { _T("07009") ,_T("Invalid descriptor index")
                ,_T("(DM) The value specified for ColumnNumber was equal to 0, and the SQL_ATTR_USE_BOOKMARKS statement attribute was SQL_UB_OFF.\n"
                    "The value specified for the argument ColumnNumber was greater than the number of columns in the result set.") },
  { _T("07S01") ,_T("Invalid use of default parameter")
                ,_T("A parameter value, set with SQLBindParameter, was SQL_DEFAULT_PARAM, and the corresponding parameter was not a parameter for an ODBC canonical procedure invocation.") },
  { _T("08000") ,_T("Connection exception")
                ,_T("") },
  { _T("08001") ,_T("Client unable to establish connection")
                ,_T("The driver was unable to establish a connection with the data source.") },
  { _T("08002") ,_T("Connection name in use")
                ,_T("(DM) The specified ConnectionHandle had already been used to establish a connection with a data source, and the connection was still open.") },
  { _T("08003") ,_T("Connection not open")
                ,_T("(DM) The connection specified in the argument ConnectionHandle was not open.") },
  { _T("08004") ,_T("Server rejected the connection")
                ,_T("The data source rejected the establishment of the connection for implementation-defined reasons.") },
  { _T("08006") ,_T("Connection failure")
                ,_T("") },
  { _T("08007") ,_T("Connection failure during transaction")
                ,_T("The HandleType was SQL_HANDLE_DBC, and the connection associated with the Handle failed during the execution of the function, and it cannot be determined whether the requested COMMIT or ROLLBACK occurred before the failure.") },
  { _T("08S01") ,_T("Communication link failure")
                ,_T("The communication link between the driver and the data source to which the driver was attempting to connect failed before the SQLDriverConnect function completed processing.") },
  { _T("0A000") ,_T("Feature not supported")
                ,_T("") },
  { _T("0A001") ,_T("Error in multi-server transaction")
                ,_T("") },
  { _T("21000") ,_T("Cardinality violation")
                ,_T("") },
  { _T("21S01") ,_T("Insert value list does not match column list")
                ,_T("*StatementText contained an INSERT statement, and the number of values to be inserted did not match the degree of the derived table.") },
  { _T("21S02") ,_T("Degree of derived table does not match column list")
                ,_T("*StatementText contained a CREATE VIEW statement, and the number of names specified is not the same degree as the derived table defined by the query specification.") },
  { _T("22000") ,_T("Data exception")
                ,_T("") },
  { _T("22001") ,_T("String data, right truncation")
                ,_T("The assignment of a character or binary value to a column resulted in the truncation of nonblank (character) or non-null (binary) characters or bytes.") },
  { _T("22002") ,_T("Indicator variable required but not supplied")
                ,_T("NULL data was bound to an output parameter whose StrLen_or_IndPtr set by SQLBindParameter was a null pointer.") },
  { _T("22003") ,_T("Numeric value out of range")
                ,_T("The prepared statement associated with the StatementHandle contained a bound numeric parameter, and the parameter value caused the whole (as opposed to fractional) part of the number to be truncated when assigned to the associated table column.\n"
                    "Returning a numeric value (as numeric or string) for one or more input/output or output parameters would have caused the whole (as opposed to fractional) part of the number to be truncated.") },
  { _T("22005") ,_T("Error in assignment")
                ,_T("") },
  { _T("22007") ,_T("Invalid datetime format")
                ,_T("The prepared statement associated with the StatementHandle contained an SQL statement that contained a date, time, or timestamp structure as a bound parameter, and the parameter was, respectively, an invalid date, time, or timestamp.\n"
                    "An input/output or output parameter was bound to a date, time, or timestamp C structure, and a value in the returned parameter was, respectively, an invalid date, time, or timestamp. (Function returns SQL_SUCCESS_WITH_INFO.)") },
  { _T("22008") ,_T("Datetime field overflow")
                ,_T("The prepared statement associated with the StatementHandle contained an SQL statement that contained a datetime expression that, when computed, resulted in a date, time, or timestamp structure that was invalid.\n"
                    "A datetime expression computed for an input/output or output parameter resulted in a date, time, or timestamp C structure that was invalid.") },
  { _T("22009") ,_T("Invalid time zone displacement value")
                ,_T("") },
  { _T("22011") ,_T("Substring error")
                ,_T("") },
  { _T("22012") ,_T("Division by zero")
                ,_T("The prepared statement associated with the StatementHandle contained an arithmetic expression that caused division by zero.\n"
                    "An arithmetic expression calculated for an input/output or output parameter resulted in division by zero.") },
  { _T("22015") ,_T("Interval field overflow")
                ,_T("*StatementText contained an exact numeric or interval parameter that, when converted to an interval SQL data type, caused a loss of significant digits.\n"
                    "*StatementText contained an interval parameter with more than one field that, when converted to a numeric data type in a column, had no representation in the numeric data type.\n"
                    "*StatementText contained parameter data that was assigned to an interval SQL type, and there was no representation of the value of the C type in the interval SQL type.\n"
                    "Assigning an input/output or output parameter that was an exact numeric or interval SQL type to an interval C type caused a loss of significant digits.\n"
                    "When an input/output or output parameter was assigned to an interval C structure, there was no representation of the data in the interval data structure.") },
  { _T("22018") ,_T("Invalid character value for cast specification")
                ,_T("*StatementText contained an SQL statement that contained a literal or parameter, and the value was incompatible with the data type of the associated table column.") },
  { _T("22019") ,_T("Invalid escape character")
                ,_T("The argument StatementText contained a LIKE predicate with an ESCAPE in the WHERE clause, and the length of the escape character following ESCAPE was not equal to 1.") },
  { _T("22021") ,_T("Character not in repertoire")
                ,_T("") },
  { _T("22022") ,_T("Indicator overflow")
                ,_T("") },
  { _T("22023") ,_T("Invalid parameter value")
                ,_T("") },
  { _T("22024") ,_T("Unterminated C String")
                ,_T("") },
  { _T("22025") ,_T("Invalid escape sequence")
                ,_T("The argument StatementText contained \"LIKE pattern value ESCAPE escape character\" in the WHERE clause, and the character following the escape character in the pattern value was neither \"%\" nor \"_\".") },
  { _T("22026") ,_T("String data - length mismatch")
                ,_T("") },
  { _T("22027") ,_T("Trim error")
                ,_T("") },
  { _T("23000") ,_T("Integrity constraint violation")
                ,_T("The prepared statement associated with the StatementHandle contained a parameter. The parameter value was NULL for a column defined as NOT NULL in the associated table column, a duplicate value was supplied for a column constrained to contain only unique values, or some other integrity constraint was violated.\n") },
  { _T("24000") ,_T("Invalid cursor state")
                ,_T("The Attribute argument was SQL_ATTR_CURRENT_CATALOG, and a result set was pending.") },
  { _T("25000") ,_T("Invalid transaction state")
                ,_T("There was a transaction in process on the connection specified by the argument ConnectionHandle. The transaction remains active.") },
  { _T("25S01") ,_T("Transaction state unknown")
                ,_T("One or more of the connections in Handle failed to complete the transaction with the outcome specified, and the outcome is unknown.") },
  { _T("25S02") ,_T("Transaction is still active")
                ,_T("The driver was not able to guarantee that all work in the global transaction could be completed atomically, and the transaction is still active.") },
  { _T("25S03") ,_T("Transaction is rolled back")
                ,_T("The driver was not able to guarantee that all work in the global transaction could be completed atomically, and all work in the transaction active in Handle was rolled back.") },
  { _T("28000") ,_T("Invalid authorization specification")
                ,_T("Either the user identifier or the authorization string, or both, as specified in the connection string (InConnectionString), violated restrictions defined by the data source.") },
  { _T("3D000") ,_T("Invalid catalog name")
                ,_T("The Attribute argument was SQL_CURRENT_CATALOG, and the specified catalog name was invalid.") },
  { _T("34000") ,_T("Invalid cursor name")
                ,_T("*StatementText contained a positioned DELETE or a positioned UPDATE, and the cursor referenced by the statement being prepared was not open.") },
  { _T("3D000") ,_T("Invalid catalog name")
                ,_T("The catalog name specified in StatementText was invalid.") },
  { _T("3F000") ,_T("Invalid schema name")
                ,_T("The schema name specified in StatementText was invalid.") },
  { _T("40001") ,_T("Serialization failure")
                ,_T("The transaction was rolled back due to a resource deadlock with another transaction.") },
  { _T("40002") ,_T("Integrity constraint violation")
                ,_T("The CompletionType was SQL_COMMIT, and the commitment of changes caused integrity constraint violation. As a result, the transaction was rolled back.") },
  { _T("40003") ,_T("Statement completion unknown")
                ,_T("The associated connection failed during the execution of this function, and the state of the transaction cannot be determined.") },
  { _T("42000") ,_T("Syntax error or access violation")
                ,_T("*StatementText contained an SQL statement that was not preparable or contained a syntax error.\n"\
                    "*StatementText contained a statement for which the user did not have the required privileges.") },
  { _T("42S01") ,_T("Base table or view already exists")
                ,_T("*StatementText contained a CREATE TABLE or CREATE VIEW statement, and the table name or view name specified already exists.") },
  { _T("42S02") ,_T("Base table or view not found")
                ,_T("*StatementText contained a DROP TABLE or a DROP VIEW statement, and the specified table name or view name did not exist.\n"
                    "*StatementText contained an ALTER TABLE statement, and the specified table name did not exist.\n"
                    "*StatementText contained a CREATE VIEW statement, and a table name or view name defined by the query specification did not exist.\n"
                    "*StatementText contained a CREATE INDEX statement, and the specified table name did not exist.\n"
                    "*StatementText contained a GRANT or REVOKE statement, and the specified table name or view name did not exist.\n"
                    "*StatementText contained a SELECT statement, and a specified table name or view name did not exist.\n"
                    "*StatementText contained a DELETE, INSERT, or UPDATE statement, and the specified table name did not exist.\n"
                    "*StatementText contained a CREATE TABLE statement, and a table specified in a constraint (referencing a table other than the one being created) did not exist.") },
  { _T("42S11") ,_T("Index already exists")
                ,_T("*StatementText contained a CREATE INDEX statement, and the specified index name already existed.") },
  { _T("42S12") ,_T("Index not found")
                ,_T("*StatementText contained a DROP INDEX statement, and the specified index name did not exist.") },
  { _T("42S21") ,_T("Column already exists")
                ,_T("*StatementText contained an ALTER TABLE statement, and the column specified in the ADD clause is not unique or identifies an existing column in the base table.") },
  { _T("42S22") ,_T("Column not found")
                ,_T("*StatementText contained a CREATE INDEX statement, and one or more of the column names specified in the column list did not exist.\n"
                    "*StatementText contained a GRANT or REVOKE statement, and a specified column name did not exist.\n"
                    "*StatementText contained a SELECT, DELETE, INSERT, or UPDATE statement, and a specified column name did not exist.\n"
                    "*StatementText contained a CREATE TABLE statement, and a column specified in a constraint (referencing a table other than the one being created) did not exist.") },
  { _T("44000") ,_T("WITH CHECK OPTION violation")
                ,_T("The prepared statement associated with StatementHandle contained an INSERT statement performed on a viewed table or a table derived from the viewed table that was created by specifying WITH CHECK OPTION, such that one or more rows affected by the INSERT statement will no longer be present in the viewed table.\n"
                    "The prepared statement associated with the StatementHandle contained an UPDATE statement performed on a viewed table or a table derived from the viewed table that was created by specifying WITH CHECK OPTION, such that one or more rows affected by the UPDATE statement will no longer be present in the viewed table.") },
  { _T("HY000") ,_T("General error")
                ,_T("An error occurred for which there was no specific SQLSTATE and for which no implementation-specific SQLSTATE was defined. The error message returned by SQLGetDiagField from the diagnostic data structure describes the error and its cause") },
  { _T("HY001") ,_T("Memory allocation error")
                ,_T("The driver was unable to allocate memory required to support execution or completion of the function.") },
  { _T("HY003") ,_T("Invalid application buffer type")
                ,_T("The argument TargetType was neither a valid data type nor SQL_C_DEFAULT.") },
  { _T("HY004") ,_T("Invalid SQL data type")
                ,_T("The value specified for the argument ParameterType was neither a valid ODBC SQL data type identifier nor a driver-specific SQL data type identifier supported by the driver.") },
  { _T("HY008") ,_T("Operation canceled")
                ,_T("Asynchronous processing was enabled for the StatementHandle. The function was called, and before it completed execution, SQLCancel or SQLCancelHandle was called on the StatementHandle. Then the function was called again on the StatementHandle.\n"
                    "The function was called, and before it completed execution, SQLCancel or SQLCancelHandle was called on the StatementHandle from a different thread in a multithread application.") },
  { _T("HY009") ,_T("Invalid use of null pointer")
                ,_T("(DM) The OutputHandlePtr argument was a null pointer.") },
  { _T("HY010") ,_T("Function sequence error")
                ,_T("(DM) An asynchronously executing function was called for the connection handle that is associated with the StatementHandle. This aynchronous function was still executing when SQLColAttribute was called.\n"
                    "(DM) SQLExecute, SQLExecDirect, or SQLMoreResults was called for the StatementHandle and returned SQL_PARAM_DATA_AVAILABLE. This function was called before data was retrieved for all streamed parameters.\n"
                    "(DM) The function was called prior to calling SQLPrepare, SQLExecDirect, or a catalog function for the StatementHandle.\n"
                    "(DM) An asynchronously executing function (not this one) was called for the StatementHandle and was still executing when this function was called.\n"
                    "(DM) SQLExecute, SQLExecDirect, SQLBulkOperations, or SQLSetPos was called for the StatementHandle and returned SQL_NEED_DATA. This function was called before data was sent for all data-at-execution parameters or columns.") },
  { _T("HY011") ,_T("Attribute cannot be set now")
                ,_T("The Attribute argument was SQL_ATTR_TXN_ISOLATION, and a transaction was open.") },
  { _T("HY013") ,_T("Memory management error")
                ,_T("The function call could not be processed because the underlying memory objects could not be accessed, possibly because of low memory conditions.") },
  { _T("HY017") ,_T("Invalid use of an automatically allocated descriptor handle.")
                ,_T("(DM) The Handle argument was set to the handle for an automatically allocated descriptor.") },
  { _T("HY018") ,_T("Server declined cancel request")
                ,_T("The server declined the cancel request.") },
  { _T("HY019") ,_T("Non-character and non-binary data sent in pieces")
                ,_T("SQLPutData was called more than once for a parameter or column, and it was not being used to send character C data to a column with a character, binary, or data source–specific data type or to send binary C data to a column with a character, binary, or data source–specific data type.") },
  { _T("HY020") ,_T("Attempt to concatenate a null value")
                ,_T("SQLPutData was called more than once since the call that returned SQL_NEED_DATA, and in one of those calls, the StrLen_or_Ind argument contained SQL_NULL_DATA or SQL_DEFAULT_PARAM.") },
  { _T("HY021") ,_T("Inconsistent descriptor information")
                ,_T("The descriptor information checked during a consistency check was not consistent. (See the \"Consistency Checks\" section in SQLSetDescField.)\n"
                    "The value specified for the argument DecimalDigits was outside the range of values supported by the data source for a column of the SQL data type specified by the ParameterType argument.") },
  { _T("HY024") ,_T("Invalid attribute value")
                ,_T("(DM) The InfoType argument was SQL_DRIVER_HSTMT, and the value pointed to by InfoValuePtr was not a valid statement handle.\n"
                    "(DM) The InfoType argument was SQL_DRIVER_HDESC, and the value pointed to by InfoValuePtr was not a valid descriptor handle.") },
  { _T("HY090") ,_T("Invalid string or buffer length")
                ,_T("(DM) *CharacterAttributePtr is a character string, and BufferLength was less than 0 but not equal to SQL_NTS.") },
  { _T("HY091") ,_T("Invalid descriptor field identifier")
                ,_T("The value specified for the argument FieldIdentifier was not one of the defined values and was not an implementation-defined value.") },
  { _T("HY092") ,_T("Invalid attribute/option identifier")
                ,_T("(DM) The DriverCompletion argument was SQL_DRIVER_PROMPT, and the WindowHandle argument was a null pointer.") },
  { _T("HY095") ,_T("Function type out of range")
                ,_T("(DM) An invalid FunctionId value was specified.") },
  { _T("HY096") ,_T("Information type out of range")
                ,_T("The value specified for the argument InfoType was not valid for the version of ODBC supported by the driver.") },
  { _T("HY103") ,_T("Invalid retrieval code")
                ,_T("(DM) The value specified for the argument Direction was not equal to SQL_FETCH_FIRST, SQL_FETCH_FIRST_USER, SQL_FETCH_FIRST_SYSTEM, or SQL_FETCH_NEXT.") },
  { _T("HY104") ,_T("Invalid precision or scale value")
                ,_T("The value specified for the argument ColumnSize or DecimalDigits was outside the range of values supported by the data source for a column of the SQL data type specified by the ParameterType argument.") },
  { _T("HY105") ,_T("Invalid parameter type")
                ,_T("The value specified for the argument InputOutputType in SQLBindParameter was SQL_PARAM_OUTPUT, and the parameter was an input parameter.") },
  { _T("HY107") ,_T("Row value out of range")
                ,_T("The value specified with the SQL_ATTR_CURSOR_TYPE statement attribute was SQL_CURSOR_KEYSET_DRIVEN, but the value specified with the SQL_ATTR_KEYSET_SIZE statement attribute was greater than 0 and less than the value specified with the SQL_ATTR_ROW_ARRAY_SIZE statement attribute.") },
  { _T("HY109") ,_T("Invalid cursor position")
                ,_T("The prepared statement was a positioned update or delete statement, and the cursor was positioned (by SQLSetPos or SQLFetchScroll) on a row that had been deleted or could not be fetched.") },
  { _T("HY110") ,_T("Invalid driver completion")
                ,_T("(DM) The value specified for the argument DriverCompletion was not equal to SQL_DRIVER_PROMPT, SQL_DRIVER_COMPLETE, SQL_DRIVER_COMPLETE_REQUIRED, or SQL_DRIVER_NOPROMPT.\n"
                    "(DM) Connection pooling was enabled, and the value specified for the argument DriverCompletion was not equal to SQL_DRIVER_NOPROMPT.") },
  { _T("HY114") ,_T("Driver does not support connection-level asynchronous function execution")
                ,_T("(DM) An application attempted to enable asynchronous function execution with SQL_ATTR_ASYNC_DBC_FUNCTIONS_ENABLE for a driver that does not support asynchronous connection operations.") },
  { _T("HY117") ,_T("Connection is suspended due to unknown transaction state. Only disconnect and read-only functions are allowed.")
                ,_T("(DM) For more information about suspended state, see SQLEndTran Function.") },
  { _T("HY121") ,_T("Cursor Library and Driver-Aware Pooling cannot be enabled at the same time")
                ,_T("For more information, see Driver-Aware Connection Pooling.") },
  { _T("HYC00") ,_T("Driver not capable")
                ,_T("The value specified for the argument FieldIdentifier was not supported by the driver.") },
  { _T("HYT00") ,_T("Timeout expired")
                ,_T("The login timeout period expired before the connection to the data source completed. The timeout period is set through SQLSetConnectAttr, SQL_ATTR_LOGIN_TIMEOUT.") },
  { _T("HYT01") ,_T("Connection timeout expired")
                ,_T("The connection timeout period expired before the data source responded to the request. The connection timeout period is set through SQLSetConnectAttr, SQL_ATTR_CONNECTION_TIMEOUT.") },
  { _T("IM001") ,_T("Driver does not support this function")
                ,_T("(DM) The driver associated with the StatementHandle does not support the function.") },
  { _T("IM002") ,_T("Data source not found and no default driver specified")
                ,_T("(DM) The data source name specified in the connection string (InConnectionString) was not found in the system information, and there was no default driver specification.\n"
                    "(DM) ODBC data source and default driver information could not be found in the system information.") },
  { _T("IM003") ,_T("Specified driver could not be loaded")
                ,_T("(DM) The driver listed in the data source specification in the system information or specified by the DRIVER keyword was not found or could not be loaded for some other reason.") },
  { _T("IM004") ,_T("Driver's SQLAllocHandle on SQL_HANDLE_ENV failed")
                ,_T("(DM) During SQLDriverConnect, the Driver Manager called the driver's SQLAllocHandle function with an fHandleType of SQL_HANDLE_ENV and the driver returned an error.") },
  { _T("IM005") ,_T("Driver's SQLAllocHandle on SQL_HANDLE_DBC failed.")
                ,_T("(DM) During SQLDriverConnect, the Driver Manager called the driver's SQLAllocHandle function with an fHandleType of SQL_HANDLE_DBC and the driver returned an error.") },
  { _T("IM006") ,_T("Driver's SQLSetConnectAttr failed")
                ,_T("(DM) During SQLDriverConnect, the Driver Manager called the driver's SQLSetConnectAttr function and the driver returned an error.") },
  { _T("IM007") ,_T("No data source or driver specified; dialog prohibited")
                ,_T("No data source name or driver was specified in the connection string, and DriverCompletion was SQL_DRIVER_NOPROMPT.") },
  { _T("IM008") ,_T("Dialog failed")
                ,_T("The driver attempted to display its login dialog box and failed.\n"
                    "WindowHandle was a null pointer, and DriverCompletion was not SQL_DRIVER_NO_PROMPT.") },
  { _T("IM009") ,_T("Unable to load translation DLL")
                ,_T("The driver was unable to load the translation DLL that was specified for the data source or for the connection.") },
  { _T("IM010") ,_T("Data source name too long")
                ,_T("(DM) The attribute value for the DSN keyword was longer than SQL_MAX_DSN_LENGTH characters.") },
  { _T("IM011") ,_T("Driver name too long")
                ,_T("(DM) The attribute value for the DRIVER keyword was longer than 255 characters.") },
  { _T("IM012") ,_T("DRIVER keyword syntax error")
                ,_T("(DM) The keyword-value pair for the DRIVER keyword contained a syntax error.\n"
                    "(DM) The string in *InConnectionString contained a FILEDSN keyword, but the .dsn file did not contain a DRIVER keyword or a DSN keyword.") },
  { _T("IM014") ,_T("The specified DSN contains an architecture mismatch between the Driver and Application")
                ,_T("(DM) 32-bit application uses a DSN connecting to a 64-bit driver; or vice versa.") },
  { _T("IM015") ,_T("Driver's SQLDriverConnect on SQL_HANDLE_DBC_INFO_HANDLE failed")
                ,_T("If a driver returns SQL_ERROR, the Driver Manager will return SQL_ERROR to the application and the connection will fail.\n"
                    "For more information about SQL_HANDLE_DBC_INFO_TOKEN, see Developing Connection-Pool Awareness in an ODBC Driver.") },
  { _T("IM017") ,_T("Polling is disabled in asynchronous notification mode")
                ,_T("Whenever the notification model is used, polling is disabled.") },
  { _T("IM018") ,_T("SQLCompleteAsync has not been called to complete the previous asynchronous operation on this handle.")
                ,_T("If the previous function call on the handle returns SQL_STILL_EXECUTING and if notification mode is enabled, SQLCompleteAsync must be called on the handle to do post-processing and complete the operation.") },
  { _T("S1118") ,_T("Driver does not support asynchronous notification")
                ,_T("When the driver does not support asynchronous notification, you cannot set SQL_ATTR_ASYNC_DBC_EVENT or SQL_ATTR_ASYNC_DBC_RETCODE_PTR.") }
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
  error.Format(_T("SQLSTATE : %s\n")
               _T("Text     : %s\n")
               _T("---------------------\n")
               _T("%s")
              ,state->m_code.GetString()
              ,state->m_error.GetString()
              ,state->m_explanation.GetString());
  return error;
}

// End of namespace
}
