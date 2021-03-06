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

LATEST HISTORY
==============

Releasenotes version 3.0.1
--------------------------
- Fixed a bug in the login dialog when defining a new connection.

Releasenotes version 3.0.0
--------------------------
- Completely rewritten interface in the latest modern MFC style.

Releasenotes version 2.5.1
--------------------------
- Fixed various bugs in the handling of the MariaDB database in Oracle mode
  Prefer to turn OFF the "Strict ODBC" handling in the general SQL settings
  for this database type, as the replacement SQL's work better for this database
  - Columns and other attributes of tables in other schema's can be discovered
    than just the currently active database schema.
  - All database types are tested and some minor bugs in the handling of 'text'
    and 'longtext' columns have been fixed.

Releasenotes version 2.5.0
--------------------------
- Fixed a bug in the saving of the permanent file settings causing the application
  to crash after changing the file settings (e.g. tabs and space inserts).
  See: Menu / Text / File settings & information
- Fixed a bug in the parameter binding of a query with the ":variable<n>" settings
  for a "?" binding in a query.
  See the file "script_syntax.txt" for more details
- Fixed a bug in the ":select on" command of the scripting syntax
  Columnnames and row values are now correctly written to the script output file, 
  provided that you have used the ":file" command previously.
  See the file "script_syntax.txt" for more details


Releasenotes version 2.4.0
--------------------------
- The source code of the SQLComponents are now replaced by a deployment ZIP file
  in the root directory of the project. The ZIP contains the include directory
  of that component and the four library files.
  For a full explanation, see: https://github.com/edwig/SQLComponents
- Various discovery methods for the info tree have been improved in speed for the
  Oracle database. The methods are:
  - SQLColumns          -> Discovery of all columns of a table
  - SQLStatistics       -> Discovery of all indices of a table
  - SQLTablePrivileges  -> Discovery of all table privileges 
  - SQLColumnPrivileges -> Discovery of all column privileges (NEW !)
- New setting. See menu: "Text / Permanent settings / SQL Settings"
  A new checkbox is added "Prefer ODBC Meta-SQL". These are the settings:
  off : If a meta-sql is present in SQLComponents, that one is used, 
        otherwise the ODBC discovery is used;
  on  : The ODBC discovery routines are always used.
  
  So now it is possible to switch between the on-board SQL meta queries or the 
  ones provided by the standard ODBC driver (even if they are slower an more incorrect).

Releasenotes version 2.3.3
--------------------------
- Fixed a bug in the startup cycle on the first discovery queries.
  It turned out that there where a few tables for in there that not all users
  have rights on to see (e.g. Oracle: "dba_db_links")
  Similar bugs for Informix, Firebird, PostgreSQL and SQLServer have been fixed
- Errors in SQL now turn up always with the erring SQL statement in them, so we
  can always see what went wrong.

Releasenotes version 2.3.2
--------------------------
- Fixed more exception errors 
- Upgraded to latest Windows 10 SDK

Releasenotes version 2.3.1
--------------------------
- Fixed the integration of the SQLComponents StdException into the exception 
  handling of the OpenEditor source code. This makes it better in catching user
  mistakes in SQL and logging in into databases.

Releasenotes version 2.3.0
--------------------------
- Fixed performance bug in starting of Oracle 12 database. Knocked of 6 seconds
  of each login for meta info of tables and catalogs.
- Oracle databases now also display all linked catalogs in the driver page
- Connection errors at login are now shown
- Upgraded to the latest MS-Windows 10 SDK build (17763)

Releasenotes version 2.2.2
--------------------------
- Compileable in Visual Studio 2017 (fixed std::string throws)
- Fixed 'no error on F5 after end-of-file' bug
- Fixed CTRL-F12 file error after installation
- Fixed CTRL-F12 error after END-OF-FILE in the buffer
- NEW FEATURE: right click on a ODBC table in the table tree to
  - SELECT first 100 rows of the table
  - Create a DDL script of the table
  Both right-click menu items only work on a table or a view node

Releasenotes version 2.2.1
--------------------------
- It is now possible to login on a database without a user/password combination
  e.g. a spreadsheet or a MS-Access database

Releasenotes version 2.2.0
--------------------------
- Sequences and procedures in the treeview are working in Oracle 12.2c
- Sequences metadata current and start values have been changed from long to double
  to accomodate Oracle 12.2 changes.

Releasenotes version 2.1.0
--------------------------
In this release much work has been done for the object tree. The object tree 
is now fully functional with the folllowing objects
- Tables 
	- Table columns
    - Primary and foreign keys
    - Indices and table statistics
    - pecial (unique) columns not visible as table columns
    - Other table referencing this table
    - Triggers for the table
    - Sequences for the table
    - Granted rights on the table
- Views
- Catalog tables
- Synonyms
- Sequences
- Triggers
- Stored procedures

Sequences, triggers and catalog tables are objects that are not directly 
supported by ODBC. For these objects the tree will only fully function:

1.Firebird 3.x and higher
2.PostgreSQL 9.x and higher
3.MySQL 5.7 and higher
4.Oracle 11.2 and higher
5.IBM-Informix 12.x and higher

For triggers and stored procedures: you can see the full source code 
of the persistent-stored-module by double clicking on the 'S'ourcecode 
node under the trigger or stored procedure.


Releasenotes version 2.0.0
--------------------------
- This is the 10th year anniversary edition of the Open ODBC Querytool!
  The following have been changed / extended:
- The object tree was previously a tree with just one table at the most. 
  It is now a tree with tables, views, catalogs, and procedures you can 
  expand any object at will, the control will then try to read in extra 
  information for you. Thusly reading the performance hit over more than 
  just one action. 
- The implmentation info tree (odbc options) has been moved from the 
  SQLComponents module to the ODBCQuerytool module (no functional changes).
- The installer has been moved to the standard open source NSIS installer
- The SQLComponents library has been upgraded to version 1.4.2
- Repaired the number format export in the XLS Excel export.