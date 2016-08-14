////////////////////////////////////////////////////////////////////////
//
// $Source: ?/WOCO - BRIEF4all/Brief/BriefLibrary/SQLDataSetXLS.h $
// $Author: ehuisman $
// 
// Copyright (c) 1995 - 2015 Centric Netherlands B.V.
// Alle rechten voorbehouden
//
// Loosly based on the article by: Yap Chun Wei
// http://www.codeproject.com/Articles/1636/CSpreadSheet-A-Class-to-Read-and-Write-to-Excel-an
// 
#pragma once

#include "SQLDataSet.h"
#include "ExcelFormat.h"
#include "BasicXmlExcel.h"
#include <list>

using namespace ExcelFormat;

typedef std::list<CString> WordList;

class SQLDataSetXLS : public SQLDataSet
{
public:
  // Open spreadsheet for reading and writing
	SQLDataSetXLS(CString File, CString SheetOrSeparator, bool Backup = true); 
  // Perform cleanup
 ~SQLDataSetXLS(); 

  // Read in a XLS and optionally make a backup
  bool ReadXLS(CString p_sheet);
  // Begin transaction
  void BeginTransaction(); 
  // Save changes to spreadsheet
  bool Commit(); 
  // Undo changes to spreadsheet
  bool RollBack(); 

  // Add header row to spreadsheet
	bool AddHeaders(CStringArray &FieldNames, bool replace = false); 
  // Insert or replace a row into spreadsheet. Default is add new row. 
  bool AddRow(CStringArray &RowValues, long row = 0, bool replace = false); 
  // Read a cell from Excel spreadsheet using header row or column alphabet. 
  bool ReadCell (CString &CellValue, CString column, long row, bool p_name = true); 
  // Read a cell from spreadsheet using column number. Default is read the next cell in next row.
  bool ReadCell (CString &CellValue, short column, long row); 
  // Replace a cell into Excel spreadsheet using header row or column alphabet. 
  // Set name to true if want to force column to be used as header name
	bool SetCell(CString CellValue, CString column, long row, bool p_name = true); 
  // Set a cell value into spreadsheet using column number, row number
	bool SetCell(CString CellValue, short column, long row); 
  // Clear text delimited file content
  bool DeleteSheet(); 
  // Clear entire Excel spreadsheet content. The sheet itself is not deleted
  bool DeleteSheet(CString p_sheetName); 

  // SETTERS
  void    SetDelimiterLeft (CString p_delim);
  void    SetDelimiterRight(CString p_delim);

  // GETTERS

  // Get a list of all worksheets in the XLS (xls only!)
  bool    GetListOfWorksheets(WordList* p_sheets);
  // Get last error message
  CString GetLastError();
  // Get status of backup. True if backup is successful, 
  // False if spreadsheet is not backup
	bool    GetBackupStatus();
  // Get status of Transaction. True if Transaction is started, 
  // False if Transaction is not started or has error in starting
	bool    GetTransactionStatus();
  // Is it an XLS type of file
  bool    GetIsXLS();

private:
  // Open a text delimited file for reading or writing
	bool  Open(); 
  bool  OpenWorksheet();
  bool  OpenXmlWorksheet();
  bool  OpenCsvWorksheet();

  // Close and forget the spreadsheet
  void  Close();
  // Convert Excel column in alphabet into column number
	int   CalculateColumnNumber(CString column, bool p_name = true); 
  // Read a row from spreadsheet. Default is read the next row
  bool  SplitRow(CString& p_input,CStringArray &RowValues); 
  // Trim whitespace in between delimiters
  void  TrimRow(CString& p_row);

  BasicExcel*     m_workbook;    // Excel workbook instance
  BasicXmlExcel*  m_xmlWorkbook; // New OOXML Workbook
  bool    m_append;          // Internal flag to denote newly created spreadsheet or previously created spreadsheet
	bool    m_backup;          // Internal flag to denote status of Backup
	bool    m_excel;           // Internal flag to denote whether file is Excel spreadsheet or text delimited spreadsheet
  bool    m_xmlExcel;        // Internal flag to denote new OOXML Excel spreadsheet
	bool    m_transaction;     // Internal flag to denote status of Transaction

  CString m_file;            // Spreadsheet file / CSV Filename
	CString m_sheetName;       // Sheet name of Excel spreadsheet
	CString m_separator;       // Separator in text delimited spreadsheet
  CString m_delimLeft;       // Delimiter for text on the left  side, default = "
  CString m_delimRight;      // Delimiter for text on the right side, default = "
	CString m_lastError;       // Last error message
};

inline void
SQLDataSetXLS::SetDelimiterLeft(CString p_delim)
{
  m_delimLeft = p_delim;
}

inline void    
SQLDataSetXLS::SetDelimiterRight(CString p_delim)
{
  m_delimRight = p_delim;
}

inline bool 
SQLDataSetXLS::GetBackupStatus()
{
  return m_backup;      
} 

inline bool 
SQLDataSetXLS::GetTransactionStatus() 
{ 
  return m_transaction; 
}

inline bool
SQLDataSetXLS::GetIsXLS()
{
  return (m_excel || m_xmlExcel);
}
