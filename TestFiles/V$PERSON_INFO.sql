CREATE OR REPLACE
-------
///////////
VIEW v$_person_info AS
SELECT
	ind_const.id,
	ind_var.history_date,
	empl./*organization_unit_id*/	enterprise_id,
	add_inf.position_id,
	empl.table_number			tabnum,
	ind_var.lastname			firstname,
	ind_var.firstname			secondname,
	ind_var.secondname			lastname,
	ind_var.sex,
	/*
	Decode (tr_w.id, NULL, 'N', 'Y') sign_trade_union,
	*/
	'N' 						sign_married,
	ind_var.birthday,
	--To_Char (NULL)			  education,
	doc.document_series 		passport_series,
	doc.document_number 		passport_number,
	doc.document_foreign_author passport_source,
	doc.document_date			passport_date,
	--To_Char (NULL)			  telephone,
	--To_Char (NULL)			  address,
	ind_var.trunc_name short_name,
	doc_pens.document_number	pension_number
  FROM
	app$cadres.individuals_const		ind_const,
	app$cadres.individuals_var			ind_var,
	app$cadres.employees				empl,
	( SELECT id, individual_id
	  FROM app$cadres.trade_union_works
	  WHERE end_date IS NULL
	)						 tr_w,
	app$cadres.Documents				doc,
	app$cadres.t$additional_information add_inf,
	( SELECT ind_doc.individual_id, pens.document_number
	  FROM
		app$cadres.Documents				pens,
		app$cadres.individual_Documents 	ind_doc
	  WHERE ind_doc.document_id = pens.id
	  AND pens.document_class_id = 7
	) doc_pens
  WHERE ind_const.complete IS NOT NULL
  AND ind_var.individual_const_id = ind_const.id
  AND empl.individual_id = ind_const.id
  AND empl.complete IS NOT NULL
  AND ind_var.history_date =
  ( SELECT Max (history_date) FROM app$cadres.individuals_var
	WHERE individual_const_id = ind_var.individual_const_id
/*
	AND history_date <=
	(SELECT Pack$Storage_Parameters.Get_Period FROM dual)
*/
  )
  AND ind_const.id = tr_w.individual_id (+)
  AND ind_var.passport_document_id = doc.id
  AND ind_const.id = add_inf.individual_id
  AND ind_const.id = doc_pens.individual_id (+)
  AND empl.organization_unit_id = something.get_enterprise_id
  AND add_inf.enterprise_id = something.get_enterprise_id
/

DROP SNAPSHOT s$person_info;
CREATE SNAPSHOT s$person_info
  AS SELECT * FROM v$_person_info;
SELECT * FROM user_snapshots;
ALTER SNAPSHOT S$PERSON_INFO REFRESH FAST START WITH SYSDATE+1/600;

create or replace
VIEW v$person_info AS
SELECT
 v.*,
 To_Char (NULL) education,
 To_Char (NULL) telephone,
 To_Char (NULL) address
FROM v$_person_info v
/

CREATE OR REPLACE
TRIGGER tr$person_info_instead$iudr
INSTEAD OF INSERT OR DELETE OR UPDATE
ON v$person_info
FOR EACH ROW
DECLARE
  p_record_new Pack$DML_Person_Info.Type_rec;
  p_record_old Pack$DML_Person_Info.Type_rec;
BEGIN
  IF INSERTING THEN
	p_record_new.id 			  := :new.id			  ;  -- Системный код человека
	p_record_new.history_date	  := Something.Get_Date_Account;  -- Дата редакции
	p_record_new.enterprise_id	  := :new.enterprise_id   ;  -- Системный код предприяти
	p_record_new.position_id	  := :new.position_id	  ;  -- Id места приписки по месту работы
	p_record_new.tabnum 		  := :new.tabnum		  ;  -- Таб.н.
	p_record_new.firstname		  := :new.firstname 	  ;  -- Фамили
	p_record_new.secondname 	  := :new.secondname	  ;  -- Им
	p_record_new.lastname		  := :new.lastname		  ;  -- Отчество
	p_record_new.sex			  := :new.sex			  ;  -- Пол
	p_record_new.sign_trade_union := :new.sign_trade_union;  -- Член профсоюза
--	  p_record_new.sign_married 	:= :new.sign_married	;  -- Женат (замужем)
	p_record_new.birthday		  := :new.birthday		  ;  -- Дата рождени
--	  p_record_new.education		:= :new.education		;  -- Образование

	p_record_new.passport_series  := translations.Normalize_passport_series(:new.passport_series);	-- Серия паспорта

	p_record_new.passport_number  := :new.passport_number ;  -- № паспорта
	p_record_new.passport_source  := :new.passport_source ;  -- Паспорт выдан
	p_record_new.passport_date	  := :new.passport_date   ;  -- Дата выдачи паспорта
--	  p_record_new.address			:= :new.address 		;  -- Адрес
--	  p_record_new.telephone		:= :new.telephone		;  -- Телефон
--	  p_record_new.short_name		:= :new.short_name		;  -- Фамилия И.О.
	p_record_new.pension_number   := :new.pension_number  ;  -- Номер пенсионной карточки

	Pack$DML_Person_Info.Pr$insert_person_info (p_record_new);
  ELSIF UPDATING THEN

	p_record_new.id 			  := :new.id			  ;  -- Системный код человека
	p_record_new.history_date	  := :new.history_date	  ;  -- Дата редакции
	p_record_new.enterprise_id	  := :new.enterprise_id   ;  -- Системный код предприяти
	p_record_new.position_id	  := :new.position_id	  ;  -- Id места приписки по месту работы
	p_record_new.tabnum 		  := :new.tabnum		  ;  -- Таб.н.
	p_record_new.firstname		  := :new.firstname 	  ;  -- Фамили
	p_record_new.secondname 	  := :new.secondname	  ;  -- Им
	p_record_new.lastname		  := :new.lastname		  ;  -- Отчество
	p_record_new.sex			  := :new.sex			  ;  -- Пол
	p_record_new.sign_trade_union := :new.sign_trade_union;  -- Член профсоюза
	p_record_new.birthday		  := :new.birthday		  ;  -- Дата рождени

	p_record_new.passport_series  := translations.Normalize_passport_series(:new.passport_series);	-- Серия паспорта

	p_record_new.passport_number  := :new.passport_number ;  -- № паспорта
	p_record_new.passport_source  := :new.passport_source ;  -- Паспорт выдан
	p_record_new.passport_date	  := :new.passport_date   ;  -- Дата выдачи паспорта
	p_record_new.pension_number   := :new.pension_number  ;  -- Номер пенсионной карточки

	p_record_old.id 			  := :old.id			  ;  -- Системный код человека
	p_record_old.history_date	  := :old.history_date	  ;  -- Дата редакции
	p_record_old.enterprise_id	  := :old.enterprise_id   ;  -- Системный код предприяти
	p_record_old.position_id	  := :old.position_id	  ;  -- Id места приписки по месту работы
	p_record_old.tabnum 		  := :old.tabnum		  ;  -- Таб.н.
	p_record_old.firstname		  := :old.firstname 	  ;  -- Фамили
	p_record_old.secondname 	  := :old.secondname	  ;  -- Им
	p_record_old.lastname		  := :old.lastname		  ;  -- Отчество
	p_record_old.sex			  := :old.sex			  ;  -- Пол
	p_record_old.sign_trade_union := :old.sign_trade_union;  -- Член профсоюза
	p_record_old.birthday		  := :old.birthday		  ;  -- Дата рождени
	p_record_old.passport_series  := :old.passport_series ;  -- Серия паспорта
	p_record_old.passport_number  := :old.passport_number ;  -- № паспорта
	p_record_old.passport_source  := :old.passport_source ;  -- Паспорт выдан
	p_record_old.passport_date	  := :old.passport_date   ;  -- Дата выдачи паспорта
	p_record_old.pension_number   := :old.pension_number  ;  -- Номер пенсионной карточки

	Pack$DML_Person_Info.Pr$update_person_info (p_record_new, p_record_old);
  ELSE
	Raise_Application_Error(-20999, '(Нельзя удалять данные о сотруднике из задачи "З/П")');
  END IF;
END;
/

GRANT SELECT ON v$person_info TO r$cash$view;
GRANT SELECT, INSERT, UPDATE, DELETE ON v$person_info TO r$wage$accountant;
GRANT SELECT ON v$person_info TO r$wage$demo;
GRANT SELECT ON v$person_info TO r$wage$timekeeper;
