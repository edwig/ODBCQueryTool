////////////////////////////////////////////////////////////////////////
//
// File: SQLInfoFirebird.cpp
//
// Copyright (c) 1998- 2014 ir. W.E. Huisman
// All rights reserved
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of 
// this software and associated documentation files (the "Software"), 
// to deal in the Software without restriction, including without limitation the rights 
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies 
// or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION 
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// Last Revision:   01-01-2015
// Version number:  1.1.0
//
#include "stdafx.h"
#include "SQLInfoFirebird.h"
#include "SQLQuery.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Verwijderen na volledige implementatie
#pragma warning (disable: 4100)

// Constructor.
SQLInfoFirebird::SQLInfoFirebird(SQLDatabase* p_database)
                :SQLInfoDB(p_database)
{
}

// Destructor. Doet nog niets.
SQLInfoFirebird::~SQLInfoFirebird()
{
}

// Get the database type
DatabaseType 
SQLInfoFirebird::GetDatabaseType() const
{
  return RDBMS_FIREBIRD;
}

// BOOLEANS EN STRINGS
// ====================================================================

// The name of the database vendor
CString
SQLInfoFirebird::GetDatabaseVendorName() const
{
  return "Firebird";
}

// Geef de fysieke database naam
CString 
SQLInfoFirebird::GetFysicalDatabaseName() const
{
  // See to it that "SQLDatabase:GetDatabaseName" does it's work
  return m_database->GetDatabaseName();
}

// Wordt de inhoud van de systeemcatalogus in uppercase opgeslagen?
bool 
SQLInfoFirebird::IsCatalogUpper() const
{
  return true;
}

// Ondersteund database/ODBCdriver commentaar in sql
bool 
SQLInfoFirebird::SupportsDatabaseComments() const
{
  return false;
}

// Database is in staat constraints op te schorten tot aan het eind v.d. transactie
bool 
SQLInfoFirebird::SupportsDeferredConstraints() const
{
  // Geen SET CONSTRAINTS DEFERRED aanwezig
  // Moet alle constraints droppen en aanmaken!
  return false;
}

// Database kan ORDER BY met een expressie aan, dus bijv. UPPER(kolom). Alternatief
// is het selecteren van een kolom expressie AS naampje en dan ORDER BY naampje.
bool
SQLInfoFirebird::SupportsOrderByExpression() const
{
  return true;
}

// Catalogus tabel met alle default waarden van een kolom in de database
CString 
SQLInfoFirebird::GetSQLStringDefaultValue(CString p_tableName,CString p_columnName) const
{
  p_tableName.MakeUpper();
  p_columnName.MakeUpper();
  
  CString query = "SELECT rdb$default_source\n"
                  "  FROM rdb$relation_fields\n"
                  " WHERE rdb$relation_name = '" + p_tableName   + "'\n"
                  "   AND rdb$field_name    = '" + p_columnName  + "'";  
  return query;
}

CString 
SQLInfoFirebird::GetSystemDateTimeKeyword() const
{
  return "current_timestamp";
}  

// String voor huidige datum
CString
SQLInfoFirebird::GetSystemDateString() const
{
  return "current_date";
}

// Geeft het feit of TIJD op de database als een DECIMAL is geimplementeerd
bool 
SQLInfoFirebird::GetTimeIsDecimal() const
{
  // TIJD is geimplementeerd als DECIMAL(18,16)
  return true;
}

// Geeft het feit of INTERVAL op de database als een DECIMAL is geimlementeerd
bool 
SQLInfoFirebird::GetIntervalIsDecimal() const
{
  // Interval is in Firebird geimplenteerd als DECIMAL
  return true;
}

// Geef het karakter voor string concatenatie
CString 
SQLInfoFirebird::GetConcatanationOperator() const
{
  return "||";    
}

// Geef quote (hoe schrijf je dat) karakter voor strings
CString 
SQLInfoFirebird::GetQuoteCharacter() const
{
  return "\'";    
}


// Geef default NULL voor invoer parameterlijst
CString 
SQLInfoFirebird::GetDefaultNULL() const
{
  return "";    
}

// Geef INOUT voor invoer/uitvoer parameterlijst
CString 
SQLInfoFirebird::GetParameterINOUT() const
{
  return "";    
}

// Geef OUT voor invoer/uitvoer parameterlijst
CString 
SQLInfoFirebird::GetParameterOUT() const
{
  return "";    
}

// Geef datatype van "h_gebruiker" in een stored procedure als parameter
CString 
SQLInfoFirebird::GetAuditUserDatatype() const
{
  return "CHAR(8)";
}

// Geef datatype van "h_gebruiker" in een stored procedure (extern) als variabele
CString 
SQLInfoFirebird::GetAuditUserDatatypeAsVariable() const
{
  return "CHAR(8)";
}

// Geef datatype van de primary key
CString 
SQLInfoFirebird::GetPrimaryKeyType() const
{
  return "INTEGER";
}

// Geef datatype voor Moment
CString 
SQLInfoFirebird::GetDatetimeYearToSecondType() const
{
  return "timestamp";
}

// Seperator tussen twee alter-constraints in een alter-table statement
CString 
SQLInfoFirebird::GetAlterConstraintSeparator() const
{
  return ",";
}

// Inner Join Keyword
CString 
SQLInfoFirebird::GetInnerJoinKeyword() const
{
  return "INNER JOIN ";
}

// Outer join keyword
CString  
SQLInfoFirebird::GetOuterJoinKeyword() const
{
  return "LEFT OUTER JOIN ";
}

// Inner Join Keyword for use in views.
CString 
SQLInfoFirebird::GetViewInnerJoinKeyword() const
{
  return "INNER JOIN";
}

// Outer join keyword for use in views
CString 
SQLInfoFirebird::GetViewOuterJoinKeyword() const
{
  return "LEFT OUTER JOIN";
}

// Geef closure voor outer-join
CString 
SQLInfoFirebird::GetOuterJoinClosure() const
{
  return "";
}

// Outer join teken
CString  
SQLInfoFirebird::GetOuterJoinSign() const
{
  return "";
}

// Prefix voor gebruik van een parameter in een stored procedure
CString
SQLInfoFirebird::GetSPParamPrefix() const
{
  return ":";
}

// Geef de query om een nieuw record toe te voegen in de kolom OID
CString 
SQLInfoFirebird::GetIdentityString(CString& p_tablename,CString p_postfix /*="_seq"*/) const
{
  return "GEN_ID(" + p_tablename + p_postfix + ",1)";
}

// Geef de query om een tijdelijke tabel uit een select statement te maken
CString 
SQLInfoFirebird::GetSQLCreateTemporaryTable(CString& p_tablename,CString p_select) const
{
  // return "CREATE TABLE " + p_tablename + "\nAS " + p_select;
  // Firebird ondersteunt dit niet!
  return "";
}

// Geef de query om een tijdelijke tabel definitief te verwijderen
CString
SQLInfoFirebird::GetSQLRemoveTemporaryTable(CString& p_tablename,int& p_number) const
{
  // 'Gewoon' een drop van de tabel doen
  p_number += 1;
  return "DROP TABLE "     + p_tablename + ";\n";
}

// Geef de query om in een tijdelijke tabel te selecteren
CString 
SQLInfoFirebird::GetSQLSelectIntoTemp(CString& p_tablename,CString& p_select) const
{
  // Firebird ondersteunt dit niet!
  return "";
}

// Vervang de OID kolom door een sequence.nextval
CString 
SQLInfoFirebird::GetReplaceColumnOIDbySequence(CString p_columns,CString p_tablename,CString p_postfix /*="_seq"*/) const
{
  int pos;
  int len = p_columns.GetLength();
  pos = p_columns.Find("OID,");
  if(pos == 0)
  {
    CString newkolom = ( p_tablename + p_postfix + ".genid()");
    newkolom += p_columns.Right(len - 3);
    return newkolom;
  }
  if(pos > 0)
  {
    pos = p_columns.Find(",OID,");
    CString newkolom = p_columns.Left(pos) + "," + p_tablename + p_postfix + ".genid()";
    newkolom += p_columns.Right(len - pos - 5);
    return newkolom;
  }
  return p_columns;
}

// Geef opslagruimte clausule voor tabellen
CString 
SQLInfoFirebird::GetTablesTablespace(CString p_tablespace /*=""*/) const
{
  return "";
}

// Geef opslagruimte clausule voor indexen
CString 
SQLInfoFirebird::GetIndexTablespace(CString p_tablespace /*=""*/) const
{
  return "";
}

// Get the storage name for indici
CString 
SQLInfoFirebird::GetStorageSpaceNameForIndexes() const
{
  return "INDEX"; // Settings::OpslagRuimte::OracleIndexTablespace;
}

// Geef de opslagruimte voor tijdelijke tabellen
// Dit is de manier van Firebird om een tijdelijke tabel aan te maken
// buiten de database om en zonder logging.
CString
SQLInfoFirebird::GetStorageSpaceNameForTempTables(CString p_tablename) const
{
  CString tempTablePath = "C:\\TMP\\";
//   if (m_temptabelPad == "")
//   {
//     m_temptabelPad = acc->GeefInstellingString(INI_FirebirdTempTables); // Default "C:\\TMP"
//     if(m_temptabelPad.Right(1) != "\\")
//     {
//       m_temptabelPad += "\\";
//     }
//   }
   
  return "EXTERNAL FILE '" + tempTablePath + p_tablename + ".fb";
}

// Verwijder catalog afhankelijkheden
CString 
SQLInfoFirebird::GetSQLRemoveProcedureDependencies(CString p_procname) const
{
  CString upperName = p_procname;
  upperName.MakeUpper();
  CString query = "DELETE FROM rdb$dependencies\n"
                  " WHERE rdb$depended_on_name = '" + upperName + "'\n"
                  "   AND rdb$depended_on_type = 5\n"
                  "   AND rdb$dependent_type   = 5";
  return query;
}

CString
SQLInfoFirebird::GetSQLRemoveFieldDependencies(CString p_tablename) const
{
  CString upperName = p_tablename;
  upperName.MakeUpper();
  CString query = "DELETE FROM rdb$dependencies\n"
                  " WHERE rdb$depended_on_name = '" + upperName + "'\n"
                  "   AND rdb$dependent_type   = 3";
  return query;
}

// Geeft de tabeldefinitie-vorm van een primary key en constraint
CString 
SQLInfoFirebird::GetPrimaryKeyDefinition(CString p_tableName,bool p_temporary) const
{
  CString keyDefinitie = GetPrimaryKeyType() + " NOT NULL\n";
  keyDefinitie += p_temporary ? CString("\n") : " CONSTRAINT pk_" + p_tableName + " PRIMARY KEY\n";
  return keyDefinitie;
}

// Geef de constraint-vorm van een primary key definitie (achteraf toevoegen aan tabel)
CString
SQLInfoFirebird::GetPrimaryKeyConstraint(CString p_tablename, bool p_temporary) const
{
  return "ADD CONSTRAINT pk_" + p_tablename + " PRIMARY KEY(oid)\n";
}

// Performance parameters in de database zetten
CString
SQLInfoFirebird::GetSQLPerformanceSettings() const
{
  // Om geoptimaliseerde queries uit te kunnen voeren op views met outer-joins
  // en filters die op die views gelegd worden is de volgende parameter
  // van levensbelang voor de performance
  return "";
}

// SQL To set the caching mode of SQL results
CString 
SQLInfoFirebird::GetSQLCacheModeSetting(const CString& /*p_mode*/) const
{
  return "";
}

// Getallen opslaan in stored-procs hebben dit nodig
CString
SQLInfoFirebird::GetSQLNlsNumericCharacters() const
{
  // Indien de ,. combinatie niet op Nederlandse wijze wordt uitgevoerd kan
  // Pronto geen numerieke waarden in *_ins_proc, *_upd_proc en *_del_procs
  // uitvoeren. Omdat dit soms fout gaat bij het installeren van de database
  // wordt deze parameter vooraf voor de sessie gezet.
  return "";
}

// Geeft de manier om een kolomnaam te wijzigen, kan meerdere SQL's bevatten
CString
SQLInfoFirebird::GetSQLModifyColumnName(CString p_tablename,CString p_oldName,CString p_newName,CString /*p_datatype*/)
{
  CString rename = "rename column " + p_tablename + "." + p_oldName + " to " + p_newName;
  return rename;
}

// Geeft de maximale lengte van een sql statement
unsigned long 
SQLInfoFirebird::GetMaxStatementLength() const
{
	return 0;		//	Geen limiet
}

// Prefix voor een add constraint DDL commando voor SQLAtlerTableGenerator
CString 
SQLInfoFirebird::GetAddConstraintPrefix(CString p_constraintName) const
{
  return "ADD CONSTRAINT " + p_constraintName + " ";
}

// Suffix voor een add constraint DDL commando voor SQLAtlerTableGenerator
CString 
SQLInfoFirebird::GetAddConstraintSuffix(CString p_constraintName) const
{
  return "";
}

// Prefix voor een drop constraint DDL commando voor SQLAlterTableGenerator
CString 
SQLInfoFirebird::GetDropConstraintPrefix() const
{
  return "DROP CONSTRAINT ";
}

// Suffix voor een drop constraint DDL commando voor SQLAlterTableGenerator
CString 
SQLInfoFirebird::GetDropConstraintSuffix() const
{
  return ",";
}

// Clausule separator tussen twee ADD of DROP clausules in een ALTER TABLE
CString 
SQLInfoFirebird::GetAlterTableClauseSeparator() const
{
  return ",";
}

// Groupering van meerdere kolommen in een ADD/MODIFY/DROP clausule
bool   
SQLInfoFirebird::GetClauseGroupingPossible() const
{
  // Alle kolomdefs moeten apart staan ADD kolomdef, ADD kolomdef etc
  return false;
}

// Geeft de prefix voor het wijzigen van het datatype in MODIFY/ALTER
CString 
SQLInfoFirebird::GetModifyDatatypePrefix() const
{
  // Bij het veranderen van het datatype gewoon het nieuwe type opgeven
  // DUS: ALTER <kolomnaam> TYPE <datatypenaam>
  return "TYPE ";
}

// Code om een tijdelijke tabel aan te maken (qualifier)
CString 
SQLInfoFirebird::GetCodeTemporaryTable() const
{
  return "";
}

// Code om de locking in de tabel op rij niveau te zetten
CString 
SQLInfoFirebird::GetCodeLockModeRow() const
{
  return "";
}

// Code om de tabel zonder logging aan te maken
CString 
SQLInfoFirebird::GetCodeTempTableWithNoLog() const
{
  return "";
}

// Code om alle rechten op de tabel open te zetten (NON-ANSI database)
CString 
SQLInfoFirebird::GetSQLGrantAllOnTable(CString p_tableName)
{
  return "GRANT ALL ON "+ p_tableName + " TO" + GetGrantedUsers() + " WITH GRANT OPTION;\n";
}

// Code voor de select into temp
CString
SQLInfoFirebird::GetSelectIntoTempClausePrefix(CString p_tableName) const
{
  return "";
}

// Code na de select into temp
CString
SQLInfoFirebird::GetSelectIntoTempClauseSuffix(CString p_tableName) const
{
  return "";
}

bool
SQLInfoFirebird::GetCodeIfStatementBeginEnd() const
{
  // IF THEN ELSE END IF; heeft perse wel BEGIN/END nodig
  return true;
}

// Geef het einde van een IF statement
CString 
SQLInfoFirebird::GetCodeEndIfStatement() const
{
  return "\n";
}

CString 
SQLInfoFirebird::GetAssignmentStatement(const CString& p_destiny,const CString& p_source) const
{
  return p_destiny + " = " + p_source + ";";
}


// Geef SQL keyword om een kolom te wijzigen
CString 
SQLInfoFirebird::GetCodeAlterColumn() const
{
  return "ALTER COLUMN ";
}

// Geef de code om een WHILE-loop te starten
CString
SQLInfoFirebird::GetStartWhileLoop(CString p_condition) const
{
  return "WHILE " + p_condition + " LOOP\n";
}

// Geef de code om een WHILE-loop te eindigen
CString
SQLInfoFirebird::GetEndWhileLoop() const
{
  return "END LOOP;\n";
}

// Geef het feit of een SELECT tussen parenthesis moet staan voor een assignment
bool 
SQLInfoFirebird::GetAssignmentSelectParenthesis() const
{
  // waarde = (SELECT max kenmerk FROM tabel);
  return true;
}

// Gets the UPPER function
CString 
SQLInfoFirebird::GetUpperFunction(CString& p_expression) const
{
  return "UPPER(" + p_expression + ")";
}

// Gets the construction for 1 minute ago
CString 
SQLInfoFirebird::GetInterval1MinuteAgo() const
{
  return "ERROR";
}

// Gets the construction / select for generating a new serial identity
CString 
SQLInfoFirebird::GetSQLGenerateSerial(CString p_table) const
{
  return "SELECT " + p_table + "_seq.nextval FROM DUAL";
}

// Gets the construction / select for the resulting effective generated serial
CString 
SQLInfoFirebird::GetSQLEffectiveSerial(CString p_identity) const
{
  // Just return it, it's the correct value
  return p_identity;
}

// Gets the Not-NULL-Value statement of the database
CString 
SQLInfoFirebird::GetNVLStatement(CString& p_test,CString& p_isnull) const
{
  return "{fn IFNULL(" + p_test + "," + p_isnull + ")}";
}

// Gets the subtransaction commands
CString 
SQLInfoFirebird::GetStartSubTransaction(CString p_savepointName) const
{
  return CString("SAVEPOINT ") + p_savepointName;
}

CString 
SQLInfoFirebird::GetCommitSubTransaction(CString p_savepointName) const
{
  return CString("COMMIT TRANSACTION ") + p_savepointName;
}

CString 
SQLInfoFirebird::GetRollbackSubTransaction(CString p_savepointName) const
{
  return CString("ROLLBACK TO ") + p_savepointName;
}

// SQL CATALOG QUERIES
// ====================================================================

// Geef SQL om te kijken of storedprocedure in de database bestaat
CString 
SQLInfoFirebird::GetSQLStoredProcedureExists(CString& p_name) const
{
  CString nameUpper(p_name);
  nameUpper.MakeUpper();
  CString query = ( "SELECT COUNT(*) "
                    "  FROM RDB$PROCEDURES "
                    " WHERE RDB$PROCEDURE_NAME ='" + nameUpper + "';");
  return query;
}

// Deel van query om 1 record te selecteren
CString 
SQLInfoFirebird::GetDualTableName() const
{
  return "dual";
}

// FROM-Part for a query to select only 1 (one) record
CString 
SQLInfoFirebird::GetDualClause() const
{
  return " FROM DUAL";
}

// Geef DEFERRABLE (of niet dus)
CString
SQLInfoFirebird::GetConstraintDeferrable() const
{
  return "";
}

// Constraints tijdelijk opschorten tot aan de volgende COMMIT;
CString 
SQLInfoFirebird::GetSQLDeferConstraints() const
{
  return "";
}

// Constraints weer op onmiddellijk terugzetten
CString 
SQLInfoFirebird::GetSQLConstraintsImmediate() const
{
  return "";
}

// Geef de query die controleert of de tabel al bestaat in de database
CString 
SQLInfoFirebird::GetSQLTableExists(CString p_tablename) const
{
  CString upperName(p_tablename);
  upperName.MakeUpper();
  CString query = "SELECT COUNT(*)\n"
                  "  FROM rdb$relations\n"
                  " WHERE rdb$relation_name = '" + upperName + "'";
  return query;
}

// Geef de query die de kolommen van een tabel uit de catalogus haalt
CString 
SQLInfoFirebird::GetSQLGetColumns(CString& p_user,CString& p_tableName) const
{
  CString upperName(p_tableName);
  upperName.MakeUpper();

  // naam, nummer, type, lengte,nullable
  CString table = "SELECT col.rdb$field_name\n"        // 0 -> Naam
                  "      ,col.rdb$field_position\n"    // 1 -> Positie
                  "      ,typ.rdb$field_type\n"        // 2 -> type
                  "      ,typ.rdb$field_length\n"      // 3 -> lengte
                  "      ,typ.rdb$null_flag\n"         // 4 -> nn 1 = not null / 0 is null
                  "      ,typ.rdb$field_precision\n"   // 5 -> precision
                  "      ,typ.rdb$field_scale\n"       // 6 -> scale (neg)
                  "  FROM rdb$relation_fields col\n"
                  "      ,rdb$fields typ\n"
                  " WHERE col.rdb$relation_name = '" + upperName + "'\n"
                  "   AND col.rdb$field_source  = typ.rdb$field_name\n"
                  " ORDER BY col.rdb$field_position";
  return table;
}

// Geef de query om de Check en Unique constraints op te halen
CString 
SQLInfoFirebird::GetSQLGetConstraintsForTable(CString& p_tableName) const
{
  CString upperName = p_tableName;
  upperName.MakeUpper();
  CString contabel = "SELECT con.rdb$constraint_name\n"
                     "      ,con.rdb$relation_name\n"
                     "  FROM rdb$relation_constraints con\n"
                     " WHERE rdb$relation_name = '" + upperName + "'"
                     "   AND con.rdb$constraint_type IN ('CHECK','UNIQUE')";
  return contabel;
}

// Lees de bestaande indexen van een tabel
CString 
SQLInfoFirebird::GetSQLTableIndexes(CString& p_user,CString& p_tableName) const
{
  CString upperName(p_tableName);
  upperName.MakeUpper(); 
	CString query = "SELECT idx.rdb$index_name\n"
                  "      ,idx.rdb$unique_flag\n" // 1 if unique, null if not
                  "      ,col.rdb$field_name\n"
                  "      ,idx.rdb$index_type\n"  // 1 if descending, null on ascending
                  "      ,col.rdb$field_position\n"
                  "  FROM rdb$indices idx\n"
                  "      ,rdb$index_segments col\n"
                  " WHERE idx.rdb$index_name    = col.rdb$index_name\n"
                  "   AND idx.rdb$relation_name = '" + upperName + "'\n"
                  "   AND idx.rdb$index_name NOT IN\n"
                  "     ( SELECT con.rdb$index_name\n"
                  "         FROM rdb$relation_constraints con\n"
                  "        WHERE con.rdb$relation_name = idx.rdb$relation_name\n"
                  "          AND con.rdb$index_name IS NOT NULL)\n"
                  " ORDER BY 1,5";
  return query;
}

// Geef de query om de referential constaints uit de catalogus te lezen.
CString 
SQLInfoFirebird::GetSQLTableReferences(CString& p_tablename) const
{
  CString upperName(p_tablename);
  upperName.MakeUpper();  
  CString query = "SELECT rdb$constraint_name\n"
                  "      ,rdb$relation_name\n"
                  "  FROM rdb$relation_constraints\n"
                  " WHERE rdb$relation_name   = '" + upperName + "'"
                  "   AND rdb$constraint_type = 'FOREIGN KEY'";
  return query;
}

// Lijst met queries om referentiele integriteiten te verwijderen
// nodig voor databases die geen "set constraints all deferred" hebben
// CString
// SQLInfoFirebird::GeefRefConstraintVerwijderenLijst(String patroon) const
// {
//   String resultaat;
//   String query = "SELECT rdb$constraint_name\n"
//                  "      ,rdb$relation_name\n"
//                  "  FROM rdb$relation_constraints\n"
//                  " WHERE rdb$constraint_type = 'FOREIGN KEY'";
//   if(patroon != "")
//   {
//     query += "   AND rdb$relation_name like '" + patroon + "%'";
//   }
//   DBrecordset rs(GeefDatabase());
//   rs.VoerSqlUit(query);
//   while(rs.GetRecord())
//   {
//     String res = (String) "alter table " + rs.GetCol_LPCSTR(1) +
//                           " drop constraint " + rs.GetCol_LPCSTR(0) + ";";
//     resultaat += res;
//   }
//   return resultaat;
// }
  
// Lijst met queries om de referentiele integriteiten weer te herstellen
// nodig voor databases die geen "set cosntraints all deferred" hebben
// String
// SQLInfoFirebird::GeefRefConstraintToevoegenLijst(String patroon) const
// {
//   String resultaat = "";
//   enum {ref_detailtabel=0,ref_constraintnaam,ref_detailkolom,ref_mastertabel};
//   String query = "select rc.rdb$relation_name   as detail_tabel\n"
//                  "      ,rc.rdb$constraint_name as constraint_naam\n"
//                  "      ,ic.rdb$field_name      as detail_kolom\n"
//                  "      ,mc.rdb$relation_name   as master_tabel\n"
//                  "  from rdb$relation_constraints rc\n"
//                  "      ,rdb$index_segments       ic\n"
//                  "      ,rdb$ref_constraints      rt\n"
//                  "      ,rdb$relation_constraints mc\n"
//                  " where rc.rdb$constraint_type = 'FOREIGN KEY'\n"
//                  "   and ic.rdb$index_name      = rc.rdb$index_name\n"
//                  "   and rc.rdb$constraint_name = rt.rdb$constraint_name\n"
//                  "   and rt.rdb$const_name_uq   = mc.rdb$constraint_name";
//   if(patroon != "")
//   {
//     query += "   and rdb$relation_name like '" + patroon + "%'";
//   }
//   DBrecordset rs(GeefDatabase());
//   rs.VoerSqlUit(query);
//   while(rs.GetRecord())
//   {
//     String res = (String)
//                  "alter table "     + rs.GetCol_LPCSTR(ref_detailtabel) +
//                  " add constraint " + rs.GetCol_LPCSTR(ref_constraintnaam) +
//                  " foreign key ("   + rs.GetCol_LPCSTR(ref_detailkolom) +
//                  ") references "    + rs.GetCol_LPCSTR(ref_mastertabel) +
//                  " (oid);";
//     resultaat += res;
//   }
//   return resultaat;
// }


// Firebird werkt niet met synoniemen
CString 
SQLInfoFirebird::GetSQLMakeSynonym(CString& /*p_objectName*/) const
{
  return "";
}

// Firebird werkt niet met synoniemen
CString 
SQLInfoFirebird::GetSQLDropSynonym(CString& p_objectName) const
{
  return "";
}

void 
SQLInfoFirebird::DoCreateSequence(CString& p_sequenceName,int p_startpos) 
{
  // Rechtstreeks uit de systeemtabellen verwijderen omdat er geen
  // DROP GENERATOR statement bestaat
  SQLQuery sql(m_database);
  DoRemoveSequence(p_sequenceName);
  // Creeer een verse generator
  CString pos;
  pos.Format("%d",p_startpos);
  sql.TryDoSQLStatement("CREATE GENERATOR " + p_sequenceName + " TO " + pos);
}

// Verwijder een sequence
void
SQLInfoFirebird::DoRemoveSequence(CString& p_sequenceName) const
{
  CString upperNaam = p_sequenceName;
  upperNaam.MakeUpper();
  CString query = "DELETE FROM RDB$GENERATORS\n"
    " WHERE RDB$GENERATOR_NAME = '" + upperNaam + "'"; 
  SQLQuery sql(m_database);
  sql.TryDoSQLStatement(query);
}

// Geef de query om een stored procedure te verwijderen
void
SQLInfoFirebird::DoRemoveProcedure(CString& p_procedureName) const
{
  SQLQuery query(m_database);
  query.TryDoSQLStatement("DROP PROCEDURE " + p_procedureName);
}

void 
SQLInfoFirebird::DoCreateNextSequence(const CString& p_tableName,CString p_postfix /*="_seq"*/)
{
  // In Firebird worden de oid's gegenereerd uit een GENERATOR. Deze moet eerst worden
  // aangemaakt. Niet meer droppen, want dan zijn er al oid's uitgegeven
  long maxOid  = 0;
  SQLQuery sql(m_database);
  CString exists = GetSQLTableExists(p_tableName);
  sql.DoSQLStatement(exists);
  if(sql.GetRecord() && sql.GetColumn(1)->GetAsSLong() == 1)
  {
    // We zoeken het maximum oid in de tabel plus 1 als beginwaarde.
    CString query = "SELECT MAX(oid) FROM " + p_tableName;

    try
    {
      sql.DoSQLStatement(query);
      if(sql.GetRecord())
      {
        maxOid = sql.GetColumn(1)->GetAsSLong();
      }
    }
    catch(CString& s)
    {
      // Dit kan op de een of andere manier nog mis gaan indien de tabel een op 
      // dit moment niet aanwezige temptable is. We negeren deze fouten.
      throw CString("Unknown table: " + s);
    }
  }
  ++maxOid;
  // Hoogste oid meegeven aan de create sequence als start pos.
  {
    // Deze mag fout gaan (als hij niet bestaat)
    CString tableName(p_tableName);
    tableName.MakeUpper();
    DoCreateSequence(tableName,maxOid);
    DoCommitDDLcommands();
  }
}

// Geef de query om rechten op een sequence toe te kennen
CString 
SQLInfoFirebird::GetSQLSequenceRights(const CString& /*p_tableName*/,CString /*p_postfix /*="_seq"*/) const 
{
  // In Firebird hoeven geen sequence rechten te worden toegeken
  return "";
}

CString
SQLInfoFirebird::GetSQLSessionAndTerminal() const
{
  CString query = "SELECT current_connection\n"
                  "      ,'PC'\n"
                  "  FROM rdb$relations\n"
                  " WHERE rdb$relation_name = 'RDB$RELATIONS'";
  return query;
}

// Get SQL to check if sessionnumber exists
CString 
SQLInfoFirebird::GetSQLSessionExists(CString sessieId) const
{
  // To be implemented
  return "";
}

// Geef query voor uniek sessie id.
CString
SQLInfoFirebird::GetSQLUniqueSessionId(const CString& /*p_databaseName*/,const CString& p_sessionTable) const
{
  // Er is geen manier in Firebird om alle lopende sessies op te vragen
  // Daarom vragen we simpelweg alle sessies op voor vandaag
  // Dit houdt in dat we vermoeden dat oudere sessies zijn blijven hangen
  CString query = "SELECT distinct(sessie_nr)\n"
                  "  FROM " + p_sessionTable + "\n"
                  " WHERE h_trans_van > current_date";
  return query;
}

// Geef query voor opzoeken sessie.
CString
SQLInfoFirebird::GetSQLSearchSession(const CString& /*p_databaseName*/,const CString& /*p_sessionTable*/) const
{
  // Er is geen manier in Firebird om andere sessies te zoeken of te registeren
  return "";
}

// Kijk of tabel/kolom combinatie bestaat
bool   
SQLInfoFirebird::DoesColumnExistsInTable(CString& p_owner,CString& p_tableName,CString& p_column) const
{
  // Nog te implementeren
  return 0;
}

// Query om te bepalen of de tabel al een primary key heeft
CString
SQLInfoFirebird::GetSQLPrimaryKeyConstraintInformation(CString& p_tableName) const
{
  CString table(p_tableName);
  table.MakeUpper();
  CString query = "select count(*)\n"
                 "  from rdb$indices\n"
                 " where rdb$relation_name = '" + table + "'\n"
                 "   and rdb$index_name = 'PK_" + table + "'";
  return query;
}

// Does the named constraint exist in the database
bool    
SQLInfoFirebird::DoesConstraintExist(CString p_constraintName) const
{
  return false;
}

// Geeft querytekst om alle foreign keys van een schema te verwijderen
// String 
// SQLInfoFirebird::GeefAlleForeignKeysVerwijderen(String eigenaar) const
// {
//   String sqlCode;
//   int    nummer;
// 
//   nummer = 1;
//   eigenaar.MakeUpper();
//   String query = "SELECT rel.rdb$relation_name\n"
//                  "      ,con.rdb$constraint_name\n"
//                  "      ,ind.rdb$field_name\n"
//                  "  FROM rdb$relations rel\n" 
//                  "      ,rdb$relation_constraints con\n"
//                  "      ,rdb$index_segments ind\n"
//                  " WHERE rel.rdb$owner_name    = '" + eigenaar + "'\n"
//                  "   AND con.rdb$relation_name = rel.rdb$relation_name\n"
//                  "   AND con.rdb$relation_type = 'FOREIGN KEY'";
//   DBrecordset rs(GeefDatabase());
//   rs.VoerSqlUit(query);
//   while(rs.GetRecord())
//   {
//     String tabel  = rs.GetCol_LPCSTR(0);
//     String constr = rs.GetCol_LPCSTR(1);
//     String veld   = rs.GetCol_LPCSTR(2);
//     String num    = String::LongNaarString(nummer++);
//     sqlCode += (String)"ALTER  TABLE    " + tabel + " DROP CONSTRAINT " + constr + ";\n";
//     sqlCode += (String)"CREATE INDEX FK_" + tabel + "_" + num + " ON " + tabel + "(" + veld + ");\n";
//   }
//   return sqlCode;
// }

// Geeft een lock-table query
CString 
SQLInfoFirebird::GetSQLLockTable(CString& p_tableName,bool p_exclusive) const
{
  // Firebird kent geen LOCK-TABLE statement
  return "";
}

// Geef query om de statistics te optimaliseren / analyseren
CString 
SQLInfoFirebird::GetSQLOptimizeTable(CString& p_owner,CString& p_tableName,int& p_number)
{
  return "";
}

// Getting the fact that there is only **one** (1) user session in the database
bool
SQLInfoFirebird::GetOnlyOneUserSession()
{
  CString sql("SELECT COUNT(*)\n"
              "  FROM MON$ATTACHMENTS\n"
              " WHERE MON$SYSTEM_FLAG = 0");

  SQLQuery query(m_database);
  SQLVariant* sessions = query.DoSQLStatementScalar(sql);
  return sessions->GetAsSLong() <= 1;
}

// SQL DDL ACTIONS
// ====================================================================

// Verwerk DDL commandos in de catalog
void 
SQLInfoFirebird::DoCommitDDLcommands() const
{
  SQLQuery sql(m_database);
  try
  {
    sql.DoSQLStatement("COMMIT");
  }
  catch(...)
  {
    sql.TryDoSQLStatement("ROLLBACK");
    throw CString("Rollback transaction");
  }
}

// Expliciet DML commandos verwerken
void
SQLInfoFirebird::DoCommitDMLcommands() const
{
  SQLQuery sql(m_database);
  sql.DoSQLStatement("COMMIT");
}

// Creeer een view op een geoptimaliseerde manier vanuit een bestaande tabel
void 
SQLInfoFirebird::DoCreateOrReplaceView(CString p_code,CString p_viewName)
{
  SQLQuery sql(m_database);
  sql.DoSQLStatement(p_code);
  sql.DoSQLStatement("GRANT ALL ON " + p_viewName + " TO " + GetGrantedUsers());
}

// Verwijder een view uit de database
void 
SQLInfoFirebird::DoDropView(CString p_viewName)
{
  p_viewName.MakeUpper();
  SQLQuery sql(m_database);
  // Eerst dependencies wegwerken. In Firebird willen dependencies op allerlei views
  // (bijvoorbeeld van een cascaderende del_proc van een andere klasse op de _g-view
  // van een historische klasse) nog wel eens het genereren breken.
  sql.TryDoSQLStatement("DELETE FROM rdb$dependencies\n"
                        " WHERE rdb$depended_on_name = '" + p_viewName + "'\n"
                        "   AND rdb$depended_on_type = 0");
  sql.TryDoSQLStatement("DROP VIEW " + p_viewName);
}

// Does the named view exists in the database
bool
SQLInfoFirebird::DoesViewExists(CString& p_viewName)
{
  // To be implemented
  return true;
}

// Creeer tijdelijke klassen runtime
bool 
SQLInfoFirebird::GetMustMakeTemptablesAtRuntime() const
{
  return true;
}

// Creeer een tijdelijke tabel op een geoptimaliseerde manier vanuit een meegegeven info
void
SQLInfoFirebird::DoMakeTemporaryTable(CString& p_tableName,CString& p_content,CString& p_indexColumn) const
{
  // @EH Firebird
  // Firebird kent geen echte 'tijdelijke' tabellen
  // Support voor beheer van temp-dbspaces nog te bouwen
  SQLQuery sql(m_database);
  CString create = "CREATE TABLE " + p_tableName + GetStorageSpaceNameForTempTables(p_tableName) + p_content;
  try
  {
    sql.DoSQLStatement(create);
  }
  catch(...)
  {
    // Kan geen tijdelijke tabel maken: %s
    throw CString("Cannot create temporary table: " + p_tableName);
  }
}

// Verwijder tijdelijke tabel weer
void
SQLInfoFirebird::DoRemoveTemporaryTable(CString& p_tableName) const
{
  SQLQuery query(m_database);
  // Als de tijdelijke tabel er niet is, dan is het niet erg.
  query.TryDoSQLStatement("DROP TABLE " + p_tableName);

  // Voor de zekerheid: Bestand eerst wissen in de temp-tablespace
  // Negeer eventuele foutcode. Is immers niet erg als het bestand er niet is
  CString file = GetStorageSpaceNameForTempTables(p_tableName);
  DeleteFile(file);
}

// Indien de tabel een tijdelijke tabel is, verwijder hem
void
SQLInfoFirebird::DoRemoveTemporaryTableWithCheck(CString& p_tableName) const
{
  CString tableName(p_tableName);
  int number = 0;
  tableName.MakeUpper();

  CString query = "SELECT count(*)\n"
                  "  FROM rdb$relations\n"
                  " WHERE rdb$relation_name = '" + tableName + "'\n"
                  "   AND rdb$external_file IS NOT NULL";
  SQLQuery sql(m_database);
  sql.DoSQLStatement(query);
  if(sql.GetRecord())
  {
    number = sql.GetColumn(1)->GetAsSLong();
  }
  // Als de tabel WEL voorkomt in de catalog, dan mag je hem proberen te droppen
  // Als hij NIET voorkomt, dan bestaat hij niet, en hoeft hij niet gedropped te worden
  if(number == 1)
  {
    DoRemoveTemporaryTable(tableName);
  }
}

// Maak een procedure aan in de database
void 
SQLInfoFirebird::DoMakeProcedure(CString& p_procName,CString p_table,bool /*p_noParameters*/,CString& p_codeBlock)
{
  SQLQuery sql(m_database);
  sql.DoSQLStatement(p_codeBlock);
  sql.DoSQLStatement("GRANT EXECUTE ON PROCEDURE " + p_procName + " TO " + GetGrantedUsers());
  DoCommitDDLcommands();
}

// Wijze van de database om een tabel van naam te veranderen
void
SQLInfoFirebird::DoRenameTable(CString& p_oldName,CString& p_newName) const
{
  throw CString("Changing database tablenames is not supported by this database type.");
}

// PERSISTENT-STORED MODULES (SPL / PL/SQL)
// ===================================================================

// Geef de user-errors uit de database:
CString 
SQLInfoFirebird::GetUserErrorText(CString& p_procName) const
{
  (void)p_procName;   // niet beschikbaar in Firebird.
  return "";
}

// Geef toekenning aan een variabele in SPL
CString 
SQLInfoFirebird::GetSPLAssignment(CString p_variable) const
{
  // CBS nog te doen voor Firebird
  return "";
}

// Geef de start van een SPL While loop
CString 
SQLInfoFirebird::GetSPLStartWhileLoop(CString p_condition) const
{
  // CBS nog te doen voor Firebird
  return "";
}

// Geef het einde van een SPL While loop
CString 
SQLInfoFirebird::GetSPLEndWhileLoop() const
{
  // CBS nog te doen voor Firebird
  return "";
}

// Geef stored procedureaanroep regel
CString 
SQLInfoFirebird::GetSQLSPLCall(CString p_procName) const
{
  return "SELECT vv_result,vv_sqlerror,vv_isamerror,vv_errordata FROM " + p_procName;
}

// Lengte parameters bij binden:
int 
SQLInfoFirebird::GetParameterLength(int p_SQLType) const
{
  int retval;

  switch (p_SQLType)
  {
    case SQL_CHAR:          retval = 32000;    break;
    case SQL_VARCHAR:       retval = 32000;    break;
    case SQL_LONGVARCHAR:   retval = 32000;    break;
    case SQL_DECIMAL:       retval = 32000;    break;
	  case SQL_SMALLINT:      retval = 0;    break;
	  case SQL_INTEGER:       retval = 0;    break;
	  case SQL_REAL:          retval = 0;    break;
	  case SQL_DOUBLE:        retval = 0;    break;
	  case SQL_FLOAT:         retval = 0;    break;
	  case SQL_BINARY:        retval = 0;    break;
	  case SQL_VARBINARY:     retval = 0;    break;
	  case SQL_LONGVARBINARY: retval = 0;    break;
	  case SQL_DATE:          retval = 0;    break;
    case SQL_TIME:          retval = 0;    break;
    case SQL_TIMESTAMP:     retval = 19;   break;
	  case SQL_NUMERIC:       retval = 0;    break;
	  case SQL_BIGINT:        retval = 0;    break;
	  case SQL_TINYINT:       retval = 0;    break;
	  case SQL_BIT:           retval = 0;    break;
    case SQL_INTERVAL_YEAR:
    case SQL_INTERVAL_YEAR_TO_MONTH:
    case SQL_INTERVAL_MONTH:
    case SQL_INTERVAL_DAY:
    case SQL_INTERVAL_DAY_TO_HOUR:
    case SQL_INTERVAL_DAY_TO_MINUTE:
    case SQL_INTERVAL_DAY_TO_SECOND:
    case SQL_INTERVAL_HOUR:
    case SQL_INTERVAL_HOUR_TO_MINUTE:
    case SQL_INTERVAL_HOUR_TO_SECOND:
    case SQL_INTERVAL_MINUTE:
    case SQL_INTERVAL_MINUTE_TO_SECOND:
    case SQL_INTERVAL_SECOND:     retval = 25;    break;
    default:                      retval = 0;     break;
  }
  return retval;
}

// Bouw een parameterlijst op voor aanroep van de stored-procedure
CString 
SQLInfoFirebird::GetBuildedParameterList(size_t p_numOfParameters) const
{
  // dit staat voor de parameters, de vraagtekens die worden meegegeven aan de
  // stored procedure.
  // ALTIJD Ellipsis, ook als er geen parameters zijn.
  CString strParamLijst = "(";
  for (size_t i = 0; i < p_numOfParameters; i++)
  {
    if(i) 
    {
      strParamLijst += ",";
    }
    strParamLijst += "?";
  }
  strParamLijst += ")";

  return strParamLijst;
}

CString
SQLInfoFirebird::GetParameterType(CString& p_type) const
{
  return p_type;
}

// GENERAL SQL ACTIONS
// =================================================================

CString 
SQLInfoFirebird::GetSQLString(const CString& p_string) const
{
  CString s = p_string;
  s.Replace("'","''");
  CString kwoot = GetQuoteCharacter();
  return  kwoot + s + kwoot;
}

CString 
SQLInfoFirebird::GetSQLDateString(int p_year,int p_month,int p_day) const
{
  CString retval;
  retval.Format("CAST '%02d/%02d/04d' AS DATE)",p_day,p_month,p_year); // American order!!
  return retval;
}

CString 
SQLInfoFirebird::GetSQLTimeString(int p_hour,int p_minute,int p_second) const
{
  // To be implemented
  return "";
}

CString 
SQLInfoFirebird::GetSQLDateTimeString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  CString retval;
  retval.Format("CAST('%02d/%02d/%04d %02d:%02d:%02d' AS TIMESTAMP)"
               ,p_day,p_month,p_year       // American order !!
               ,p_hour,p_minute,p_second); // 24 hour clock
  return retval;
}

// Get date-time bound parameter string in database format
CString 
SQLInfoFirebird::GetSQLDateTimeBoundString() const
{
  return "CAST(? AS TIMESTAMP)";
}

// Stripped data for the parameter binding
CString
SQLInfoFirebird::GetSQLDateTimeStrippedString(int p_year,int p_month,int p_day,int p_hour,int p_minute,int p_second) const
{
  CString retval;
  retval.Format("%02d/%02d/%04d %02d:%02d:%02d"
                ,p_day,p_month,p_year       // American order !!
                ,p_hour,p_minute,p_second); // 24 hour clock
  return retval;
}

CString 
SQLInfoFirebird::GetSPLSourcecodeFromDatabase(const CString& p_owner,const CString& p_procName) const
{
  // @EH Nog implementeren
  return "";
}

// Get the SPL datatype for integer
CString 
SQLInfoFirebird::GetSPLIntegerType() const
{
  return "integer";
}

// Get the SPL datatype for a decimal
CString 
SQLInfoFirebird::GetSPLDecimalType() const
{
  return "decimal";
}

// Get the SPL declaration for a cursor
CString 
SQLInfoFirebird::GetSPLCursorDeclaratie(CString& p_variableName,CString& p_query) const
{
  return "CURSOR " + p_variableName + " IS " + p_query + ";";
}

// Get the SPL cursor found row parameter
CString 
SQLInfoFirebird::GetSPLCursorFound(CString& p_cursorName) const
{
  return p_cursorName + "_found";
}

// Get the SPL cursor row-count variable
CString 
SQLInfoFirebird::GetSPLCursorRowCount(CString& p_variable) const
{
  // Not supported
  return "";
}

// Get the SPL datatype for a declaration of a row-variable
CString 
SQLInfoFirebird::GetSPLCursorRowDeclaration(CString& p_cursorName,CString& p_variableName) const
{
  // TODO: Check
  return "";
}

CString 
SQLInfoFirebird::GetSPLFetchCursorIntoVariables(CString               p_cursorName
                                               ,CString             /*p_variableName*/
                                               ,std::vector<CString>& p_columnNames
                                               ,std::vector<CString>& p_variableNames) const
{
  // TODO: Check
  CString query = "FETCH " + p_cursorName + " INTO ";  

  std::vector<CString>::iterator cNames;
  std::vector<CString>::iterator vNames;
  bool moreThenOne = false;

  for(cNames  = p_columnNames.begin(), vNames  = p_variableNames.begin();
    cNames != p_columnNames.end() && vNames != p_variableNames.end();
    ++cNames, ++vNames)
  {
    query += (moreThenOne ? "," : "") + *vNames;
  }
  query += ";";
  return query;
}

// Fetch the current SPL cursor row into the row variable
CString 
SQLInfoFirebird::GetSPLFetchCursorIntoRowVariable(CString& p_cursorName,CString p_variableName) const
{ 
  // TODO: CHeck
  return "FETCH " + p_cursorName + " INTO " + p_variableName+ ";";
}

// Get the SPL no-data exception clause
CString 
SQLInfoFirebird::GetSPLNoDataFoundExceptionClause() const
{
  // TODO: Check
  return "WHEN NO_DATA THEN";
}

// Get the SPL form of raising an exception
CString 
SQLInfoFirebird::GetSPLRaiseException(CString p_exceptionName) const
{
  // TODO Check
  return "RAISE " + p_exceptionName + ";";
}

// Get the fact that the SPL has server functions that return more than 1 value
bool    
SQLInfoFirebird::GetSPLServerFunctionsWithReturnValues() const
{
  return true;
}

// SPECIALS
// ==========================================================================

// Translate database-errors to a human readable form
CString 
SQLInfoFirebird::TranslateErrortext(int p_error,CString p_errorText) const
{
  // Check if we have work to do
  if(p_error == 0)
  {
    return p_errorText;
  }

  CString errorText;
  errorText.Format("ODBC error [%d:%s]",p_error,p_errorText);
  return errorText;
}

