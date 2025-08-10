-- Test transcript using 'Northwind' test database
:file WRITE 'C:\tmp\ODBC_QTool.log';
:print 'Testing ODBC querytool variables';

-- result variables
:variable1 COLUMN LONG;
:variable2 COLUMN LONG;
:variable3 INPUT  LONG;
:variable4 RETURN LONG;    -- RESULT

:SELECT ON;
SELECT "EmployeeID","ShipVia" FROM "Orders" WHERE "OrderID" = 10250;
-- HANAR,4,2

-- Is it the right value?
:if variable2 = 0;
:print 'Error shipping absent from order';
-- Exit if error
:if variable2 = 0;
:file close;
:if variable2 = 0;
:exit;


:print 'End reached OK';
:file close;
:exit;

