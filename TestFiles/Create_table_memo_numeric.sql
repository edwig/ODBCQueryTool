DROP TABLE test;

CREATE TABLE test
(
   oid INT
  ,memo VARCHAR(Max)
  ,getal NUMERIC(16,2)
)
;

INSERT INTO test(oid,memo,getal)
VALUES (1,'Dit is een test met een onzinnige boodschap',1234.56);

INSERT INTO test(oid,memo,getal)
VALUES (2,'Dit is een een hele andere tekst met een rare boodschap

abcdefghijklmnopqrstuvwxyz01234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ
abcdefghijklmnopqrstuvwxyz01234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ
abcdefghijklmnopqrstuvwxyz01234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ
abcdefghijklmnopqrstuvwxyz01234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ
abcdefghijklmnopqrstuvwxyz01234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ
abcdefghijklmnopqrstuvwxyz01234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ
---------------------------------------------------------------
abcdefghijklmnopqrstuvwxyz01234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
',1.23);

SELECT oid,memo
  FROM test;

SELECT oid,memo,getal
  FROM test;


SELECT oid,getal,memo
  FROM test;