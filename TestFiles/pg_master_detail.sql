------------------------------------------------------
-- TEST SCRIPT FORT PostgreSQL databases
--
-- W.E. Huisman (c) 2025
--
-------------------------------------------------------

-- SCHEMA

create schema sysdba authorization sysdba;

-- Oracle like DUAL

create table sysdba.dual
(
   X varchar(1)
);

-- MASTER

create table sysdba.master
(
   id          integer not null
  ,invoice     integer
  ,description varchar(250)
  ,total       decimal(18,2)
);

alter table sysdba.master
  add constraint pk_master
      primary key(id);

-- DETAIL

create table sysdba.detail
(
   id           integer not null
  ,mast_id      integer not null
  ,line   		integer not null
  ,description 	varchar(250)
  ,amount 	 	decimal(18,2)
);

alter table sysdba.detail
  add constraint pk_detail
      primary key(id);

alter table sysdba.detail
  add constraint fk_detail_master
  foreign key (mast_id) references sysdba.master(id);

create unique index detail_02
    on sysdba.detail(mast_id,line);

-- TEST_NUMBER

create table sysdba.test_number
(
    id integer not null
   ,field1 integer
   ,field2 double precision
   ,field3 decimal(18,2)
);

alter table sysdba.test_number
  add constraint pk_test_number
      primary key(id);

-- TEST_RECORD	  

create table sysdba.test_record
(
   name varchar(4000)
);

-------------------------------------------------------
--
-- INSERTING DATA
--
-------------------------------------------------------

-- DUAL

INSERT into sysdba.dual(x) values ('X');

-- MASTER

INSERT into sysdba.master (id,invoice,description,total) values (1,1001,'First invoice', 750);
INSERT into sysdba.master (id,invoice,description,total) values (2,1002,'Second invoice',600);

-- DETAIL

INSERT INTO sysdba.detail (id,mast_id,line,description,amount) values (1,1,1,'First line of first invoice', 100);
INSERT INTO sysdba.detail (id,mast_id,line,description,amount) values (2,1,2,'Second line of first invoice',500);
INSERT INTO sysdba.detail (id,mast_id,line,description,amount) values (3,1,3,'Third line of first invoice', 150);

INSERT INTO sysdba.detail (id,mast_id,line,description,amount) values (4,2,1,'First line of second invoice',  50);
INSERT INTO sysdba.detail (id,mast_id,line,description,amount) values (5,2,2,'Second line of second invoice', 50);
INSERT INTO sysdba.detail (id,mast_id,line,description,amount) values (6,2,3,'Third line of second invoice',  50);
INSERT INTO sysdba.detail (id,mast_id,line,description,amount) values (7,2,4,'Fourth line of second invoice', 50);
INSERT INTO sysdba.detail (id,mast_id,line,description,amount) values (8,2,5,'Fifth line of second invoice', 300);
INSERT INTO sysdba.detail (id,mast_id,line,description,amount) values (9,2,6,'VAT for the second invoice',   100);

-- TEST_NUMBER

INSERT INTO sysdba.test_number (id,field1,field2,field3) values (1,  1,    1,    100);
INSERT INTO sysdba.test_number (id,field1,field2,field3) values (2,200,  200.34, 200.34);
INSERT INTO sysdba.test_number (id,field1,field2,field3) values (3,400,  400.68, 400.68);
INSERT INTO sysdba.test_number (id,field1,field2,field3) values (6, 42,89975.123,300.77);
INSERT INTO sysdba.test_number (id,field1,field2,field3) values (7, 42,89975.123,300.77);
INSERT INTO sysdba.test_number (id,field1,field2,field3) values (8, 42,89975.123,300.77);

-- TEST_RECORD

INSERT INTO sysdba.test_record (name) values ('Testing from within a program');
INSERT INTO sysdba.test_record (name) values ('Multiple duplicate testing.Multiple duplicate testing.');

-- VIEWS

CREATE VIEW sysdba.new_view
    AS SELECT * from sysdba.detail;

-- SEQUENCES

CREATE SEQUENCE sysdba.master_seq AS integer 
       INCREMENT by 1
	   MINVALUE 1
	   MAXVALUE 999999999
	   START WITH 1
	   NO CYCLE
	   OWNED BY sysdba.master.id;

CREATE SEQUENCE sysdba.test_number_seq AS integer 
       INCREMENT by 1
	   MINVALUE 1
	   MAXVALUE 999999999
	   START WITH 1
	   NO CYCLE
	   OWNED BY sysdba.test_number.id;

-- TRIGGERS

CREATE OR REPLACE FUNCTION sysdba.insert_test_record()
RETURNS TRIGGER
AS $$
BEGIN
  INSERT INTO sysdba.test_record(name) values (newtable.description);
  RETURN 0;
END  
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION sysdba.insert_test_record(a varchar(4000),b varchar(4000))
RETURNS INTEGER
AS $$
BEGIN
  INSERT INTO sysdba.test_record(name) values (a);
  INSERT INTO sysdba.test_record(name) values (b);
  RETURN 0;
END  
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION sysdba.GETDEDCIMAL(x varchar(200))
RETURNS decimal(18,2) 
AS $$
  DECLARE 
      num DECIMAL(18,2);
  BEGIN
    num = x;
	return num;
  END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION sysdba.TESTMUL2(x integer)
RETURNS integer
AS $$
  BEGIN
    RETURN (X * 2);
  END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE TRIGGER detail_sync
   AFTER INSERT ON sysdba.detail
   FOR EACH ROW 
   EXECUTE FUNCTION sysdba.insert_test_record();

-- END OF SQL SCRIPT
