BEGIN TRANSACTION;

-- Local temporary table
CREATE TABLE ##myresults
(
  oid  INT NOT NULL
 ,name VARCHAR(250)
 ,val  NUMERIC(16,4)
);

CREATE INDEX myresults_name ON ##myresults(name);

ALTER TABLE ##myresults
  ADD CONSTRAINT pk_myresults
      PRIMARY KEY(oid);

-- Local temporary table
CREATE TABLE ##result_details
(
   oid    INT PRIMARY KEY
  ,master INT
  ,val    NUMERIC(16,4)
);


ALTER TABLE ##result_details
  ADD CONSTRAINT fk_details_results
      FOREIGN KEY (master)
      REFERENCES ##myresults(oid)
      ON DELETE CASCADE;

DROP   SEQUENCE myresults_seq;
CREATE SEQUENCE myresults_seq
       START WITH 1
       NO CYCLE
;

-- GLOBAL TEMPORARY TABLE

CREATE TABLE #alltests
(
   oid    INT PRIMARY KEY
  ,name   VARCHAR(250)
  ,result tinyint DEFAULT 0
);

CREATE TABLE #test_details
(
  oid     INT PRIMARY KEY
 ,master  INT
 ,subs    NUMERIC(10,2)
)

ALTER TABLE #test_details
  ADD CONSTRAINT fk_detail_alltests
      FOREIGN KEY (master)
      REFERENCES #alltests(oid)
      ON DELETE CASCADE;


CREATE INDEX test_details_subs ON #test_details(subs);

INSERT INTO ##myresults
VALUES (1,'test',56.789)
;
INSERT INTO ##result_details
VALUES (2,2,16.123)
;

SELECT * FROM ##myresults;
SELECT * FROM ##result_details;

COMMIT TRANSACTION;