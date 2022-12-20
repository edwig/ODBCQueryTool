CREATE OR ALTER TRIGGER [mdba].[kbs_gb_insert_trigger]
ON mdba.kbs_gb
AFTER INSERT
AS
BEGIN
    DECLARE @oid INT, @object INT, @naam VARCHAR(1000)
    SET @oid    = (SELECT oid    FROM inserted)
    SET @object = (SELECT object FROM inserted)
    SET @naam   = (SELECT naam   FROM inserted)

    INSERT INTO idba.mbs_gb
    (oid,object,naam,maatwerk,in_brief,ww_verloopt,debuggen,debug_boom,debug_boom_aan,ads_inloggen)
    VALUES
    (@oid,@object,@naam,1,1,0,1,1,1,1)
END

DROP TRIGGER mdba.kbs_gb_insert_trigger;