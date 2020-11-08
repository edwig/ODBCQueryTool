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
