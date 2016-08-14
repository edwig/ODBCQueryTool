-- SQL SERVER test script

CREATE TABLE master
(
  id    INTEGER NOT NULL
 ,naam  VARCHAR(100)
);

ALTER TABLE master
  ADD CONSTRAINT pk_master PRIMARY KEY (id);

CREATE TABLE detail
(
  id        INTEGER NOT NULL
 ,master_id INTEGER NOT NULL
 ,naam      VARCHAR(100)
);

ALTER TABLE detail
  ADD CONSTRAINT pk_detail PRIMARY KEY (id);

ALTER TABLE detail
  ADD CONSTRAINT fk_det_master FOREIGN KEY(master_id) REFERENCES master(id);

-- Populate tables

INSERT INTO master VALUES (1,'een');
INSERT INTO master VALUES (2,'twee');
INSERT INTO master VALUES (3,'drie');
INSERT INTO master VALUES (4,'vier');

INSERT INTO detail VALUES (1,1,'een - detail een');
INSERT INTO detail VALUES (2,1,'een - detail twee');

INSERT INTO detail VALUES (3,2,'twee - detail een');
INSERT INTO detail VALUES (4,2,'twee - detail twee');

INSERT INTO detail VALUES (5,3,'drie - detail een');
INSERT INTO detail VALUES (6,3,'drie - detail twee');

INSERT INTO detail VALUES (7,4,'vier - detail een');
INSERT INTO detail VALUES (8,4,'vier - detail twee');


-- TEST: Selections from these tables

SELECT m.id
      ,m.naam
      ,d.id   AS det_id
      ,d.naam AS det_naam
  FROM master m
       INNER JOIN detail d
       ON d.master_id = m.id
;

SELECT * FROM master;

SELECT * FROM detail;