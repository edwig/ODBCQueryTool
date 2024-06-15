-- Test selection with variables and printing to file

-- INPUT
:variable1 INOUT LONG;
:variable1 = 1;  -- Trye to change to 2 !

-- OUTPUT
:variable2 OUTPUT CHAR;
:variable3 OUTPUT NUMERIC;

-- GET WHAT YOU WANT
:select on;

SELECT id
      ,description
      ,total
  FROM Master
 WHERE id = ?;

:select off;

-- Check variables in dialog: MENU "Script / Variables"

-- WRITE RESULT TO FILE
:file write 'C:\tmp\Testfile_2.txt';
:print 'Description';
:print variable2;
:print 'Total';
:if variable3 <= 750;
:print variable3;
:file close;





