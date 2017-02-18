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
=======

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