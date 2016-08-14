This is the README for ODBC QueryTool
=====================================

This ODBC Query tool is meant to be used against ODBC datasources.
It's primary purpose was to test various SQL commands againt an ODBC
connection instead of an Oracle OCI connection. This was needed because
1) Specific ODBC SQL could be tested like "{ oj ... }" outer join syntax
2) Specific escape sequences could be testen like "{ fn ...}" syntax
3) General proof that a command does work against an ODBC connection
4) General proof that the ODBC connection does work

More general: nowdays every database tend to come with it's own administration
and query tools, which means that a user has to have knowledge of all seperate
tools and interfaces. This tool provides 1 (one) interface for a general
ODBC database, so that a user only needs one tool to learn.
The downside of this choice is that the more sophisticated administration tasks
cannot be administred right away other than by typing SQL. There are no
fancy dialogs to administer database dependent objects like stored procs and
such. The good thing about this is that the user gets to know their database
pretty well.

HISTORY
=======

Version 1.4.3 (Build: 184)
--------------------------
- Fixed MARS: Multiple Active Record Sets
  This is a MS-SQLServer feature, needed to run more than one SQL statement,
  for instance a SQL Statement in an inner loop of another recordset.
  This feature should be in the 'on' state BEFORE connecting to a database.

Version 1.4.2 (Build: 183)
-------------------------
- Fixed parsing of SQL Found with F10/F11. The division operator ('/')
  was mistaken for the end-of-the-statement marker. The ODBCQuerytool now can
  correctly parse SQL scripts like:

  SELECT (field_b - field_a) / field_c
    FROM table
  /
  SELECT (field_b - field_a) / field_c
    FROM table
  ;
  SELECT {fn DATE() } FROM dual
  /

  As a result to this: ANY / marker at the first position will be seen as an
  end-of-the-statement marker. So a 'real' calculation with the division 
  operator must not have the division operator in the first text column!
  Thanks to: Ibo Boekestijn, Manfred Baumann

- NULL values are exported as '0' rather than an empty string, whil exporting to
  an Excel spreadsheet. This is corrected. Numbers and dates are exported as empty
  strings when NULL.
  Thanks to: Jerrad Pierce

Version 1.4.1 (Build: 182)
--------------------------
- Fixed a typo on the connection dialog "Safty" -> "Safety"
- Fixed a bug in the month of the date the connection was saved. The month 
  was off-by-one (januari = zero)
- Extended the functionality of the Excel spreadsheet export.
  The images (OK and ERROR) are now being exported to Excel as:
  "0" on a green field -> Means no error
  "1" on a red field   -> Means one error

Version 1.4.0 (Build: 181)
--------------------------
- Added the possibility to send a bug report or a feature request by email
  to the maintainers of the program.

- Fixed a bug in the connection dialog. The 'Test' function was broken for all
  who use the standard 'OQT_SAVE_PASSWORD' setting to zero (No passwords saved)
  As the test function uses the saved password's from the connection list.
  You can now type in a password above the 'Test' button and use that as an override
  for the testing of the connection.
  Alsoo fixed various bugs in the saving of the password in the list, and changed
  the default deployment to 'SAVE_PASSWORD's so that everyone can again enjoy the 
  save connections, including passwords.

  For paranoia organisations or persons, please undefine 'OQT_SAVE_PASSWORD' in the
  following include header: 'OpenEditorApp.h' on line 41.

- Fixed a bug in the search-next / search-previous functionality (F3 / SHIFT-F3).
  These functions where commented out, and not working, and sometimes threw errors
  or let the whole application hang. The bugs are solved in this version
  F3       -> Find the next instance of the search string
  SHIFT-F3 -> Finds the previous instance, but not if you are still on a found string!

- Updated the Database code to a higher version of the SQLComponents module

- Source code ported to VisualStudio 2015.
  Various extra warnings point to better type conversions to be done.
  All code in the database module is updated for this version.
  Warnings in the Open-Editor disregarded for now: it's still working :-)

- Compiled against MS-Windows SDK 8.1
  Application tested against MDAC 2.8 for Windows 7.x, 8.x and Windows 10
  Alsoo tested on MS-Windows Server 2008 and 2012 platforms.
  Everything is working fine on these platforms. 

Version 1.3.0 (Build: 180)
--------------------------
- Added 'BasicExcel' code to the codebase to be able to process 
  spreadsheets in both reading and writing mode.
- Writing 'real' *.XLS files as an export from the data grid
  It's a replacement of the writing of the *.SYLK files.
  Writes headers in bold and background colors.

Version 1.2.1 (Build: 179)
--------------------------
- Fixed bugs in SQLPrimaryKey
- Fixed bugs in SQLForeignKey
- Fixed leaking of database cursors

Version 1.2.0 (Build: 178)
--------------------------
- Port to Visual Studio 2013 (With MBCS option pack!!)
- Higher stability of the SQL Query results
- Support for the datatypes DATE, TIME, TIMESTAMP
- Mutable SQLDataSet with autmatic insert/update/delete
- XML Export of the dataset -> In order to be able to export 
  query results to XML. Can now be added to this program at last.
- Support for the following databases vendors:
  - Oracle 
  - IBM Informix
  - Microsoft SQL-Server
  - Microsoft MS-Access
  - PostgreSQL
  - Firebird
  - Generic ODBC typed databases
- Plugin Database-part in directory "Database" in the source tree
- The database part of the program is now under the MIT license

Version 1.0.5  (Build: 35)
- Resolved a issue for databases with fractions in the second field
  of timestamp types. The fractions where not correctly calculated and 
  concatenated to the seconds-field without an intermediate '.' sign.
  All fractional parts are 1.000.000.000 part of a second (1 nanosecond)
  See the ODBC documentation from Microsoft for this.
  This means there is a another bug in the ODBC driver from Oracle !!
- Port to Visual Studio 2010.

Version 1.0.4. (Build: 34)
- Solution for some XP computers giving streaming-errors at startup.
  Due to incorrect optimalisation settings in some source files in the project
  and a MSVCRT dependency in the installer.
- This is a servicepack for the retracted 1.0.3 version

Version 1.0.3 (build: 33)
- Recurse to Windows Version 5.xx in Stdafx.h
  because of support problems with Windows-XP
  (fonts and SHGetFolderPath)
  Tested on XP-Mode in 32 bits mode, and works fine

Version 1.0.2 (build: 32)
- New routine to do the discovery of %APPDATA%
- Fonts of connection dialog and query result grid can be chosen
  Misfits on some XP machines to fonts smaller than 4 px
  Make your choise at the menu "Text / Permanent settings / Query setttings"

Version 1.0.1 (build: 31)
- use "#define OQT_SAVE_PASSWORD" to save passwords in a build.
- Ini's with current connections will be saved to 
  "%APP_DATA%\EDO\OpenODBCQuerytool\ODBC_querytool.ini"
  Will work for Windows-XP / Vista / W7
  On Vista/W7 it will be saved in the roaming profile.
- Update on the helpfile. Slightly more documentation :-)

Version 1.0 (build: 30)
- Find Repaired for multilines
- Save-as feature crash after "CTRL-S" combination fixed
- SQL-Server features repaired. Querytool runs on 1 cursor only.
- Stable release after multi tests
- Stable release works for Oracle 9/10/11; IBM-Informix 9/11; 
  SQL-Server 2005 and higher
- True 64Bits and 32Bits implementations

Version 0.78 (build: 29)
- Find&Replace did crash in the find. Replace alsoo didn't work as a
  result of the reworking in the regexp class. Both are fixed.
  Searching and replacing now works fine again.

Version 0.77 (build: 28)
- Fixed problem of empty query. If a query returns no result,
  there was no header line in the output view pane. This is now
  solved. If a query returns no lines, the output query view still
  shows the names of the queried fields.
  
Version 0.76 (build: 27)
- Fixed problem in repeat script. Grids will no longer grow,
  so measured times are linear to tested SQL commands.

Version 0.75 (build: 26)
- New feature: script elements to do performance testing
  :repeat command to repeatedly perform the same SQL 
- New feature: command line /CONNECT feature. See script_syntax.sql
  Allows for scripts to run from the command line
- Fixed problem with DML under Informix 3.50 driver in case of 
  zero (0) affected rows in an insert/update/delete statement

Version 0.74 (build: 25)
- Fixed problem in SQL-Server without MARS option. Could not have more
  than one recordset open at any one time.
- Fixed problem with info discovery on combination of databases
  Now works fine for INFORMIX/ORACLE/MS-ACCESS/MS-SQLServer/
  Firebird/MySQL etc.

Version 0.73 (Build: 24)
- Fixed alphanumeric sort of ODBC and RDBMS keywords in the Info tree
- Saving of datasources and connections now works under MS-Vista and
  also under non-local-adminstration accounts on Windows XP
- Weakly coded passwords are removed from the ini file under production
  builds. Users must now always provide a password.
  C++ Programmers can re-enable this as a debug build feature by defining
  the "OQT_SAVE_PASSWORD" macro in "OpenEditorApp.h"
  But mind you: debug builds only. Not on production builds!

Version 0.72 (build: 23)
- Fixed: Object discovery did not work in MS-Access (Jet engine 4.x)
  object names cannot be passed as a empty string to the ODBC driver
  but must be passed as NULL pointers if SQL_ATTR_METADATA_ID does not work
- New feature: Port and test to MS-Vista.
  Added WideMessageBox for message-box redemption under Vista!

Version 0.71 (build: 22)
- New feature to test SQLGetData for incoming piecewise interface
  of large SQL_C_CHAR and SQL_C_BINARY fields

Version 0.70 (build: 21)
- Fixed bug in freeing ODBC statement handles
- Fixed bug in HDBC error handling
- Fixed bug in more than 32 digits fraction in SQL_NUMERIC

Version 0.69 (build: 20)
- Fixed memory bugs in HistoryLines
- Added script support for testing of ODBC drivers
  See "script_syntax.txt"
- Added the SQLVariant class to cache values from an ODBC driver
- Rewrote the "Recordset.cpp" to use SQLVariant for columns
- Fixed serveral minor bugs in DBInfo in the use of SQLGetInfo
- Fixed recordset to use the SQLParamData/SQLPutData for the
  SQL_AT_EXEC piecewise inserts.

Version 0.68
- Fixed diverse SQL info functions (SQLTables etc)

Version 0.67
- Fixed the parsing of strings in SQL statements
  "" '' -> embedded quotes within strings, especially when spread over
  more than one (1) line. Such as in:
INSERT INTO table1 values (23,'this is a long ''string with embedded''
quotes in it 
');
Version 0.66
- Changed workbookbar under query tekst windows
  -> Bar replaced to the top of the documents
  -> Bar is a now TabControl
- Fixed the bug that an update in the grid of the primary key 
  did not warn the user that it's illegal!!
  The user is not only warned, but the original value is restored alsoo.
  
Version 0.65
- Changed ODBC driver report to a full print of the ODBC Info tree
  so the report looks the same as on screen. The tree-level is printed
  as 4 spaces instead of the treeview's lines.

Version 0.64
- Added: It is now posssible to edit the results grid of a query
  if all parts of the primary key, or in the absence of a primary key
  of the first found unique key are present, editted results wil be
  updated to the underlying table.
  This feature is alsoo known as "The Marco Brattinga option"
  After each successful edit the name of the key is reflected in the
  message zone.

Version 0.63
- Added a text report on ODBC capabilities. Accesible via the main menu
  under "Session / ODBC Report" after a successful login. (CTRL-Q, R)
- Added text report output from grid (extra button on query-grid)
- Added: Meta types in the ODBC Info-tree under 'Database'.
- Added: Extra logon with details. See button "Detail" at logon and 
  the extra menu entry under 'Session / Session status"
  - Extra logon attributes ODBC standard specific
  - Extra logon attributes driver specific
    - ODBC tracing on/off
    - ODBC tracing file settings
    - ODBC Translation library and option
    - Transaction isolation
    - ODBC Cursor library
    - Quiet mode
    - Auto commit on/off
    - Dead-connection detection
    - Connection read-only settings
    - Metadata ID settings
    - Auto IPD for SQL Server
    - Network packet size
    - Connection timeout
    - Login timeout
    - Login via *.dsn file
    - Recording of succesful login via *.dsn file

Version 0.62
- Help is on the way! A On-line CHM is beeing written.
- Completed the SQLInfo tree. Added Type conversions / All statement info
  Take care: Some newer info (odbc 3.x) are unreliable on older databases 
  (Informix/Oracle) Obviously these databases can do grant/revoke but it 
  is not reported!?
- Renamed the tab 'Table' to 'Object' because it alsoo reports procedures and views
- Added 'Native SQL' command (F9) on a SQL statement shows how the ODBC driver 
  would translate '{' escape sequences to native database SQL. Alsoo on the main
  menu under 'Script / Native SQL'.
  eg: "{fn LEFT (string,3)}" is translated "SUBSTR(string,1,3)" on Oracle,
  and "{fn RIGHT(string,3)}" is translated "SUBSTR(string, -3)"
- Added the capability to search for object types in the object pane/tab
  syntax: "[<Type>:][<Catalog>.][<Schema>.][<Table>]"
  <Type> can be one of:
  - T : Search for a real table e.g: "T:persons"
  - V : Search for a real view  e.g: "V:persons_with_contract"
  - C : Search for a catalog table : "C:syscolumns"
  - S : Search for a synonym       : "S:persons_in_other_database"
  - A : Search for an alias        : "A:persons_in_other_database"
  - A combination of the above     : "TVS:clients"
  - nothing: searching for all types.
  <Catalog> can be one of:
  - A name of a catalog            : "lin6rnd4.mdba.ibs_sc"
  - A search pattern               : "%.mdba.ibs_sc" (not supported on all databases)
  - nothing: Searching in the current catalog
  <Schema> can be one of
  - A name of an user or schema    : "mdba.ibs_sc"
  - A search pattern               : "%.ibs_sc"  (not supported on all databases)
  - nothing: searching in the schema of the current logged in user in all schema's
    depending on the implementation of the database driver.
  <Table> can be one of
  - A name of a table or view      : "ibs_sc"
  - A search pattern               : "mbs%"  (not supported on all databases)
  - ALL database must support the special search pattern "%"
  - A name of a procedure          : "ibs_sc_ins_proc"
  - The special search pattern for ALL procedures "*"
  REMARK: Some databases allow more complex patterns like: "%.my%", meaning: 
  all tables from all owners in the database for which the names begins with 'my'.
  Databases allowing this are PostgreSQL and Firebird.
  BEWARE: Although the ODBC standard states that drivers must be able to search on
  table-types in a 'comma-separated way', this is mostly not the case. Most
  drivers can only search on simple 'T'able and 'V'iew type only! Even though
  the returned capabilities indicate otherwise! (Oracle/Interbase/MSSQLServer)

Version 0.61
- bug 10: removed memory leak from display of datatypes in PostgreSQL (duplicate names)
- bug 11: Incorrect info (grant/revoke/drop) corrected
- Improved datatype info 
- Improved supported SQL info

Version 0.60
- Implemented info on Procedures. Info for the procedure
  and the procedure columns (parameters, output values, return values)
  are displayed in the object tree
  NB: This fails for some ODBC drivers. 
  eg: Informix does not support these functions, while SQLGetFunctions 
      reports that it is supported
  
Version 0.59
- Added much more supported for ODBC tree and supported features
  Namly info on 
  - supported ODBC escape sequences and functions
  - supported create schema/table/view options
  - aggregate functions
  - outer join features
  - usage of the catalog naming methods
- Added status line info for long retreval of table info
- Added object naming logic in the table discovery window. 
  End user can write something like "catalog.schema.table"
  and the application will use the ODBCGetInfo to make a database specific qualifier
  Informix => catalog:schema.table
  Oracle   => SCHEMA.TABLE@CATALOG

Version 0.58
- Added database naming method. For databases that do not report their databasename
  correctly via the SQLGetInfo method, but by a rdbms specific method (Oracle!)
- Added supported ODBC function calls in the ODBC tree.
  - For the ODBC 1.x and 2.x support of SQLGetFunctions (array of 100 ints)
  - For the ODBC 3.x support of SQLGetFunctions (4000 bits)
- Added support for case-specific catalogs for databases that do not correctly
  implement the SQL_ATTR_METADTA_ID method of SQLTables/SQLColumns etc.
  For these databases (Firebird/InterBase) SQL_IDENTIFIER_CASE of SQLGetInfo
  is used to determine wheter the catalog stores in Upper or Lower case.
  NB: There are two more methods (MIXED and SENSITIVE). If neither 
  SQL_ATTR_METADATA_ID and SQL_IDENTIFIER_CASE is supported to upper/lower than
  the extraction of metadata is a 'best guess' on basis of user-input.

Version 0.57
- Added a Table object-info window with ODBC info about a table
  (Columns, Primary/Foreign keys, Columns, Indici, Referenced by other tables, 
  special columns, Statistics)
- Table can be queried by F12 from within a script
- Added a file filter for the explorer tree. Filters are extensions.
  e.g.: Fill in 'sql' and only *.sql files will be displayed in the tree

Version 0.56
- 3: Fixed: Crash problem with empty 'last-login' list
- 4: Fixed: Crash problem with scrolling in 'last-login' list
- 5: Fixed: Delete ODBC connection from connect list is 'strange' (needed refresh of list)
- 6: Fixed: Try to open a non-existing file must give messagebox i.e. Exception thrown
- 7: Fixed: Bug in STL of ConnectionDialog
- 8: Create a stored procedure does not work. This was mainly because all text ran on one line
     it now parses all newlines correctly and preserves white space.
- 9: Run ODBC SQL command from selected text does not work.
     Selected text now has precedence over SQL text parsing.
     Works now alsoo in other database engines than Oracle alone.
- SQL Query terminator (default '/') on the properties page 'Text / Permanent settings / SQL settings'
  A query gets terminated with this terminator and then executed.
- Added newer CLI conformance codes to "ODBC panel" Oracle 8/9 now displays correctly!

Version 0.55
- 2: Fixed: Script does not run beyond the fysical screen!
- Statistics/Output cleaned on each run/run-next cycle

Version 0.54
- Max prefetch lines for grid on extra properties page 'Text / Permanent settings / SQL settings'

Version 0.52
- 1: Fixed the ' Vincent' bug of an extra space

Version 0.50
- Cloned the OpenEditor project
- Added ODBC database control
- Added everything needed for a query tool
  - Connection dialog
  - Connection toolbar
  - Chris Maunder Grid
  - Query result grid
  - Statistics result grid
  - Output results grid for batch processing
  - History results grid
  - Excel export
  - HTML export

