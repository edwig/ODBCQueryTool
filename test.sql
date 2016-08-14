-- TESTSCRIPT VOOR COMMAND LINE STARTUP

-- Naar believen aanpassen. Denk om de ';'
:file open C:\Develop\ODBCQueryTool\testscript_out.txt;

-- Repeat is op 9 uur gezet (540 minuten)
--:repeat 10 wholeminutes TIMES 540 ;
:repeat 3 times 1000;
SELECT
     sysdate,
     npe.object PERS_NR,
     npe.titel TITEL,
     '' RECHTSPERSOON,
     npe.voorletters VOORLETT,
     npe.tussenvoegsel TUSSENV,
     npe.geboortedatum GEBDATUM,
     npe.overl_datum DATUM_OVERL,  npe.naam NAAM,
     npe.afg_naam AFGNAAM,
     npe.geslacht GESLACHT,
     npe.tel_prive TEL_PRIVE,
     npe.tel_zakelijk TEL_ZAAK,
     npe.tel_mobiel TEL_MOBIEL,
     (SELECT bgo.bank_gironr FROM bankgiro bgo 
       WHERE  bgo.subheeftbgo = npe.object AND bgo.standaard = 1) BANKGIRO,
     adr.afg_adres FEIT_ADRES,
     adr.afg_postcode FEIT_POST,
     adr.straatnaam  FEIT_STRAAT,
     adr.nummer FEIT_NUMMER,
     adr.letter FEIT_LETTER,
     adr.toevoeging FEIT_TOEV,
     adr.postcode FEIT_PCODE,
     adr.plaats FEIT_PLAATS,
     (SELECT cadr.afg_adres FROM adres cadr 
       WHERE  cadr.object = npe.adrcorrsub) CORR_ADRES,
     (SELECT cadr.afg_postcode FROM adres cadr 
       WHERE cadr.object = npe.adrcorrsub) CORR_POST,
     (SELECT cadr.straatnaam FROM adres cadr 
       WHERE cadr.object = npe.adrcorrsub) CORR_STRAAT,
     (SELECT cadr.nummer FROM adres cadr 
       WHERE cadr.object = npe.adrcorrsub) CORR_NUMMER,
     (SELECT cadr.letter FROM adres cadr 
       WHERE cadr.object = npe.adrcorrsub)  CORR_LETTER,
     (SELECT cadr.toevoeging FROM adres cadr 
       WHERE cadr.object = npe.adrcorrsub) CORR_TOEV,
     (SELECT cadr.postcode FROM adres cadr 
       WHERE cadr.object = npe.adrcorrsub) CORR_PCODE,
     (SELECT  cadr.plaats FROM adres cadr 
       WHERE cadr.object =npe.adrcorrsub) CORR_PLAATS,
     (SELECT padr.afg_adres FROM adres padr 
       WHERE padr.object = dcn.adrheeftdcn) POST_ADRES,
     (SELECT padr.afg_postcode FROM  adres padr 
       WHERE padr.object = dcn.adrheeftdcn)POST_POST,
     (SELECT padr.straatnaam FROM adres padr 
       WHERE padr.object = dcn.adrheeftdcn) POST_STRAAT,
     (SELECT padr.nummer FROM adres padr 
       WHERE  padr.object = dcn.adrheeftdcn) POST_NUMMER,
     (SELECT padr.letter FROM adres padr 
       WHERE padr.object = dcn.adrheeftdcn) POST_LETTER,
     (SELECT padr.toevoeging FROM adres padr 
       WHERE padr.object = dcn.adrheeftdcn) POST_TOEV,
     (SELECT padr.postcode FROM adres padr 
       WHERE padr.object = dcn.adrheeftdcn) POST_PCODE,
     (SELECT padr.plaats FROM adres padr 
       WHERE padr.object = dcn.adrheeftdcn)  POST_PLAATS,
     npe.ter_attentie TAV_NAAM,
     npe.btwnummer BTW_NR
    FROM
     deelcontract_n dcn,
     contract_n cnr,
     natpersoon_n npe,
     adres adr
    WHERE
     dcn.dcnnummer = 2093813
     AND dcn.cnrheeftdcn = cnr.object
     AND cnr.subheeftcnr = npe.object
     AND npe.adrfeitsub = adr.object
     AND dcn.h_actief is null
    UNION
    SELECT
     sysdate,
     npe.object PERS_NR,
     npe.titel  TITEL, 
     rea.naam RECHTSPERSOON,
     npe.voorletters VOORLETT,
     npe.tussenvoegsel TUSSENV,
     npe.geboortedatum GEBDATUM,
     npe.overl_datum OVERL_DATUM,
     npe.naam NAAM,
     npe.afg_naam AFGNAAM,
     npe.geslacht  GESLACHT,
     npe.tel_prive TEL_PRIVE,
     npe.tel_zakelijk TEL_ZAAK,
     npe.tel_mobiel TEL_MOBIEL,
     (SELECT bgo.bank_gironr FROM bankgiro bgo 
       WHERE bgo.subheeftbgo = rea.object AND bgo.standaard = 1)  BANKGIRO,
     adr.afg_adres FEIT_ADRES,
     adr.afg_postcode FEIT_POST,
     adr.straatnaam FEIT_STRAAT,
     adr.nummer FEIT_NUMMER,
     adr.letter  FEIT_LETTER,
     adr.toevoeging FEIT_TOEV,
     adr.postcode FEIT_PCODE,
     adr.plaats FEIT_PLAATS,
     (SELECT cadr.afg_adres FROM adres cadr 
       WHERE cadr.object = rea.adrcorrsub) CORR_ADRES,
     (SELECT cadr.afg_postcode FROM adres cadr 
       WHERE cadr.object = rea.adrcorrsub) CORR_POST,
     (SELECT cadr.straatnaam FROM adres cadr 
       WHERE cadr.object = rea.adrcorrsub) CORR_STRAAT,
     (SELECT cadr.nummer FROM  adres cadr 
       WHERE cadr.object = rea.adrcorrsub) CORR_NUMMER,
     (SELECT cadr.letter FROM adres cadr 
       WHERE cadr.object = rea.adrcorrsub) CORR_LETTER,
     (SELECT cadr.toevoeging FROM adres cadr 
       WHERE  cadr.object = rea.adrcorrsub) CORR_TOEV,
     (SELECT cadr.postcode FROM adres cadr 
       WHERE cadr.object = rea.adrcorrsub)CORR_PCODE,
     (SELECT cadr.plaats FROM adres cadr 
       WHERE cadr.object =  rea.adrcorrsub) CORR_PLAATS,
     (SELECT padr.afg_adres FROM adres padr 
       WHERE padr.object = dcn.adrheeftdcn) POST_ADRES,
     (SELECT padr.afg_postcode FROM adres padr 
       WHERE padr.object = dcn.adrheeftdcn)  POST_POST,
     (SELECT padr.straatnaam FROM adres padr 
       WHERE padr.object = dcn.adrheeftdcn) POST_STRAAT,
     (SELECT padr.nummer FROM adres padr 
       WHERE padr.object = dcn.adrheeftdcn) POST_NUMMER,  
     (SELECT padr.letter FROM adres padr 
       WHERE padr.object = dcn.adrheeftdcn) POST_LETTER,
     (SELECT padr.toevoeging FROM adres padr 
       WHERE padr.object = dcn.adrheeftdcn) POST_TOEV,
     (SELECT padr.postcode  FROM adres padr 
       WHERE padr.object = dcn.adrheeftdcn) POST_PCODE,
     (SELECT padr.plaats FROM adres padr 
       WHERE padr.object = dcn.adrheeftdcn) POST_PLAATS,
     rea.ter_attentie TAV_NAAM,
     rea.btwnummer  BTW_NR
    FROM
     deelcontract_n dcn,
     contract_n cnr,
     rechtspersoon_n rea,
     adres adr,
     natpersoon_h npe,
     assreanat arn
    WHERE
     dcn.dcnnummer = 2093813
     AND  dcn.cnrheeftdcn = cnr.object
     AND cnr.subheeftcnr = rea.object
     AND rea.adrfeitsub = adr.object
     AND dcn.h_actief is NULL
     AND rea.object = arn.reaheeftnat(+)
     AND arn.natheeftrea  =  npe.object(+) 
