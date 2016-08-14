CREATE TABLE test12
(
   one  INTEGER
  ,ordqtysal DECIMAL(10,3)
  ,totqtydlv DECIMAL(10,3)
);


INSERT INTO test12 VALUES (1,300,812);


SELECT * FROM test12;

SELECT a.one
  FROM test12 a ;

SELECT a.one
      ,(a.ordqtysal - a.totqtydlv) / a.ordqtysal AS percent
  FROM test12 a
/

