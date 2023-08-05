////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1998-2022 ir. W.E. Huisman
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
// CREDITS:
// Loosely based on the article by: Yap Chun Wei
// http://www.codeproject.com/Articles/1636/CSpreadSheet-A-Class-to-Read-and-Write-to-Excel-an
// 
// Version number: See SQLComponents.h
//
#pragma once

#include "SQLDataSet.h"
#include "ExcelFormat.h"
#include "BasicXmlExcel.h"
#include <list>

using YExcel::BasicExcel;
using YExcel::BasicExcelWorksheet;

namespace SQLComponents
{

class SQLDataSetXLS : public SQLDataSet
{
public:
  // Open spreadsheet for reading and writing
  SQLDataSetXLS(XString File, XString SheetOrSeparator, bool Backup = true); 
  // Perform cleanup
 ~SQLDataSetXLS(); 

 // Read in a XLS and optionally make a backup
  bool ReadXLS(XString p_sheet);
  // Begin transaction
  void BeginTransaction(); 
  // Save changes to spreadsheet
  bool Commit(); 
  // Undo changes to spreadsheet
  bool RollBack(); 
  // Close worksheet
  void CloseWorksheet();

  // Add header row to spreadsheet
  bool AddHeaders(WordList &FieldNames, bool replace = false); 
  // Insert or replace a row into spreadsheet. Default is add new row. 
  bool AddRow(WordList &RowValues, long row = 0, bool replace = false); 
  // Read a cell from Excel spreadsheet using header row or column alphabet. 
  bool ReadCell (XString &CellValue, XString column, long row, bool p_name = true); 
  // Read a cell from spreadsheet using column number. Default is read the next cell in next row.
  bool ReadCell (XString &CellValue, short column, long row); 
  // Replace a cell into Excel spreadsheet using header row or column alphabet. 
  // Set name to true if want to force column to be used as header name
  bool SetCell(XString CellValue, XString column, long row, bool p_name = true); 
  // Set a cell value into spreadsheet using column number, row number
  bool SetCell(XString CellValue, short column, long row); 
  // Clear text delimited file content
  bool DeleteSheet(); 
  // Clear entire Excel spreadsheet content. The sheet itself is not deleted
  bool DeleteSheet(XString p_sheetName); 

  // SETTERS
  void    SetDelimiterLeft (XString p_delim);
  void    SetDelimiterRight(XString p_delim);

  // GETTERS

  // Get a list of all worksheets in the XLS (xls only!)
  bool    GetListOfWorksheets(WordList* p_sheets);
  // Get last error message
  XString GetLastError();
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
  virtual bool Open() override; 
  // Close and forget the spreadsheet
  virtual void Close() override;
  // Convert Excel column in alphabet into column number
  int   CalculateColumnNumber(XString column, bool p_name = true); 
  // Read a row from spreadsheet. Default is read the next row
  bool  SplitRow(XString& p_input,WordList &RowValues); 
  // Trim whitespace in between delimiters
  void  TrimRow(XString& p_row);
  // Read an ASCII string from a file
  bool  ReadString(FILE* p_file,XString& p_string);
  // Write an ASCII string to a file
  bool  WriteString(FILE* p_file,XString& p_string,bool p_appendCRLF = false);

  BasicExcel*     m_workbook;    // Excel workbook instance
  BasicXmlExcel*  m_xmlWorkbook; // New OOXML Workbook
  bool    m_append;          // Internal flag to denote newly created spreadsheet or previously created spreadsheet
  bool    m_backup;          // Internal flag to denote status of Backup
  bool    m_excel;           // Internal flag to denote whether file is Excel spreadsheet or text delimited spreadsheet
  bool    m_xmlExcel;        // Internal flag to denote new OOXML Excel spreadsheet
  bool    m_transaction;     // Internal flag to denote status of Transaction

  XString m_file;            // Spreadsheet file / CSV Filename
  XString m_sheetName;       // Sheet name of Excel spreadsheet
  XString m_separator;       // Separator in text delimited spreadsheet
  XString m_delimLeft;       // Delimiter for text on the left  side, default = "
  XString m_delimRight;      // Delimiter for text on the right side, default = "
  XString m_lastError;       // Last error message

};

inline void
SQLDataSetXLS::SetDelimiterLeft(XString p_delim)
{
  m_delimLeft = p_delim;
}

inline void    
SQLDataSetXLS::SetDelimiterRight(XString p_delim)
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

inline void  
SQLDataSetXLS::CloseWorksheet()
{
  Close();
}

// End of namespace
}