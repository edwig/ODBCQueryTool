DROP TABLE idba.test;

CREATE TABLE idba.test
(
   oid      INT
  ,memo     VARCHAR(Max)
  ,anumber  NUMERIC(16,5)
  ,approx   FLOAT(53)
)
;

INSERT INTO idba.test(oid,memo,anumber,approx)
VALUES (1,'This is a test with a nonsense message',1234.56321,0.876994433);

INSERT INTO idba.test(oid,memo,anumber,approx)
VALUES (2,'This is a complete different text with a different message

abcdefghijklmnopqrstuvwxyz01234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ
abcdefghijklmnopqrstuvwxyz01234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ
abcdefghijklmnopqrstuvwxyz01234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ
abcdefghijklmnopqrstuvwxyz01234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ
abcdefghijklmnopqrstuvwxyz01234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ
abcdefghijklmnopqrstuvwxyz01234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ
---------------------------------------------------------------
abcdefghijklmnopqrstuvwxyz01234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
',1.23239, 6666.77884432);

SELECT oid,memo
  FROM idba.test;

SELECT oid,memo,anumber,approx
  FROM idba.test;


SELECT oid,anumber,str(approx,32,9),memo
  FROM test;