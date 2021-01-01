-- TESTING THE BINDING AND SCRIPTING

:file open 'C:\TMP\TextOutput.txt';
:variable1 INPUT LONG;
:variable1 = 2;
:select on;

SELECT * FROM detail WHERE mast_id = ?;

:if variable5 <> 100;
:print 'Last value was NOT a hundred';

:if variable5 <> 99;
:print 'Last value was NOT ninetynine';

:select off;
:file close;



:variable1 input LONG;
:variable1 = 4;
:variable2 input LONG;
:variable2 = 2;
:variable3 input LONG;
:variable3 = 1;
:variable4 input DECIMAL;
:variable4 = 50;
SELECT *
  FROM detail
 WHERE id      = ?
   AND mast_id = ?
   AND line    = ?
   AND amount  = ?;

:variable1 input LONG;
:variable1 = 4;
:variable2 input LONG;
:variable2 = 2;
:variable3 LONG;
:variable3 = 1;
:variable4 CHAR;
:variable4 = First line of second invoice;
SELECT *
  FROM detail
 WHERE id      = ?
   AND mast_id = ?
   AND line    = ?
   AND description = ?;

