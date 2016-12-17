////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1998-2016 ir. W.E. Huisman
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
// Last Revision:   14-12-2016
// Version number:  1.3.0
//
#include "stdafx.h"
#include "SQLDataSetXLS.h"

#define WIN32_LEAN_AND_MEAN
#include <shellapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Open spreadsheet for reading and writing
SQLDataSetXLS::SQLDataSetXLS(CString p_file, CString p_sheetOrSeperator, bool p_backup) 
              :m_file(p_file)
              ,m_workbook(NULL)
              ,m_xmlWorkbook(NULL)
              ,m_append(false)
              ,m_backup(p_backup)
              ,m_transaction(false)
              ,m_excel(false)
              ,m_xmlExcel(false)
{
  // Set defaults
  m_delimLeft  = "\"";
  m_delimRight = "\"";
  // Detect whether file is an Excel spreadsheet or a text delimited file
  CString extensie;
  CString tempString1 = m_file.Right(4);
  CString tempString2 = m_file.Right(5);
  tempString1.MakeLower();
  tempString2.MakeLower();

  // File is an Excel spreadsheet
  if (tempString1 == ".xls") 
  {
    m_excel     = true;
    m_sheetName = p_sheetOrSeperator;
    extensie    = tempString1;
    m_workbook  = new BasicExcel();
  }
  if (tempString2 == ".xlsx")
  {
    m_xmlExcel    = true;
    m_sheetName   = p_sheetOrSeperator;
    extensie      = tempString2;
    m_xmlWorkbook = new BasicXmlExcel(p_file);
    m_lastError   = m_xmlWorkbook->GetError();
  }
  else 
  {
    // Treat file as a text delimited file
    m_separator = p_sheetOrSeperator;
  }
}

// Perform some cleanup functions
SQLDataSetXLS::~SQLDataSetXLS()
{
  if(m_workbook)
  {
    //m_workbook->Close();
    delete m_workbook;
    m_workbook = NULL;
  }
  if(m_xmlWorkbook)
  {
    delete m_xmlWorkbook;
    m_xmlWorkbook = NULL;
  }
}

// Read in a XLS and optionally make a backup
bool
SQLDataSetXLS::ReadXLS(CString p_sheet)
{
  if (m_excel) 
  {
    // If file is an Excel spreadsheet
    m_sheetName = p_sheet;
    if(Open())
    {
      if((m_backup) && (m_append))
      {
        CString tempSheetName = m_sheetName;
        m_sheetName = "XLSBackup";
        m_append = false;
        if (!Commit())
        {
          m_backup = false;
        }
        m_append    = true;
        m_sheetName = tempSheetName;
      }
      return true;
    }
  }
  else if(m_xmlExcel)
  {
    m_sheetName = p_sheet;
    if(m_xmlWorkbook->GetError().IsEmpty())
    {
      if(Open())
      {
        return true;
      }
    }
  }
  else 
  {
    // if file is a text delimited file
    if(Open())
    {
      if((m_backup) && (m_append))
      {
        CString tempString = m_file;
        m_file = m_file + ".bak";
        if(!Commit())
        {
          m_backup = false;
        }
        m_file = tempString;
      }
      return true;
    }
  }
  return false;
}

// Begin transaction
void 
SQLDataSetXLS::BeginTransaction()
{
  m_transaction = true;
}

// Save changes to spreadsheet
bool 
SQLDataSetXLS::Commit()
{
  if(m_excel) // If file is an Excel spreadsheet
  {
    m_workbook->Save();
    m_transaction = false;
    return true;
  }
  if(m_xmlExcel)
  {
    m_workbook->Save();
    m_transaction = false;
    return true;
  }
  else // if file is a text delimited file
  {
    try
    {
      CFile *File = NULL;
      File = new CFile(m_file, CFile::modeCreate | CFile::modeWrite  | CFile::shareDenyNone);
      if (File != NULL)
      {
        CArchive *Archive = NULL;
        Archive = new CArchive(File, CArchive::store);
        if (Archive != NULL)
        {
          // Write the header of the file
          CString text;
          for(unsigned ind = 0;ind < m_names.size();++ind)
          {
            if(ind) text += m_separator;
            text += "\"" + m_names[ind] + "\"";
          }
          Archive->WriteString(text);
          Archive->WriteString("\r\n");

          // Write all the rows of the file
          for (int i = 0; i < GetNumberOfRecords(); ++i)
          {
            text.Empty();
            SQLRecord* record = GetRecord(i);
            for(int ind = 0;ind < GetNumberOfFields(); ++ind)
            {
              if(ind) text += m_separator;
              text += CString("\"") + CString(record->GetField(ind)->GetAsChar()) + "\"";
            }
            Archive->WriteString(text);
            Archive->WriteString("\r\n");
          }
          delete Archive;
          delete File;
          m_transaction = false;
          return true;
        }
        delete File;
      }
    }
    catch(...)
    {
    }
    m_lastError = "Error writing file\n";
    return false;
  }
}

// Undo changes to spreadsheet
bool 
SQLDataSetXLS::RollBack()
{
  // Re-read the original file
  if(Open())
  {
    m_transaction = false;
    return true;
  }
  m_lastError = "Error in returning to previous state\n";
  return false;
}

// Add header row to spreadsheet
bool 
SQLDataSetXLS::AddHeaders(CStringArray &p_fieldNames, bool p_replace)
{
  if(m_append == 0) 
  {
    // Make a new sheet
    Close();
  }
  if(p_replace) // Replacing header row rather than adding new columns
  {
    m_names.clear();
  }
  // New header row values
  for (int i = 0; i < p_fieldNames.GetSize(); i++)
  {
    m_names.push_back(p_fieldNames.GetAt(i));
  }
  return true;
}

// Insert or replace a row into spreadsheet. 
// Default is add new row.
bool
SQLDataSetXLS::AddRow(CStringArray &p_rowValues, long /*p_row*/, bool /*p_replace*/)
{
  int ind  = 0;
  int cols = GetNumberOfFields();
  int vals = (int)p_rowValues.GetSize();
  SQLRecord* record = InsertRecord();

  while(cols-- && vals--)
  {
    SQLVariant* var = new SQLVariant(p_rowValues.GetAt(ind++));
    record->AddField(var);
  }
  if(!m_transaction)
  {
    Commit();
  }
  return true;
}

// Read a cell from Excel spreadsheet using header row or column alphabet. 
// Default is read the next cell in next row. 
// Set Auto to false if want to force column to be used as header name
bool 
SQLDataSetXLS::ReadCell (CString &p_cellValue, CString p_column, long p_row, bool p_name /*=true*/)
{
  int columnIndex = CalculateColumnNumber(p_column,p_name);
  if(columnIndex == 0)
  {
    return false;
  }

  if(ReadCell(p_cellValue,(short) columnIndex, p_row))
  {
    return true;
  }
  return false;
}

// Read a cell from spreadsheet using column number. 
// Default is read the next cell in next row.
bool 
SQLDataSetXLS::ReadCell(CString &p_cellValue, short p_column, long p_row)
{
  if (p_column == 0)
  {
    m_lastError = "Column cannot be zero";
    return false;
  }
  if(p_row == 0)
  {
    m_lastError = "Row cannot be zero";
    return false;
  }
  int col = p_column - 1;
  int rec = p_row    - 2;

  if(col >= GetNumberOfFields())
  {
    m_lastError = "Column number too great";
    return false;
  }
  if(rec >= GetNumberOfRecords())
  {
    m_lastError = "Row number too great";
    return false;
  }
  // Reading a header value
  if(rec == -1)
  {
    p_cellValue = m_names[col];
    return true;
  }
  // Reading a field value
  SQLRecord* record = GetRecord(rec);
  record->GetField(col)->GetAsString(p_cellValue);
  return true;
}

// Clear text delimited file content
bool 
SQLDataSetXLS::DeleteSheet()
{
  if(m_excel)
  {
    if(DeleteSheet(m_sheetName))
    {
      return true;
    }
    else
    {
      m_lastError = "Error deleting sheet\n";
      return false;
    }
  }
  else
  {
    Close();
    if(!m_transaction)
    {
      Commit();			
    }
    m_append = false; // Set flag to new sheet
    return true;		
  }
}

// Clear entire Excel spreadsheet content. The sheet itself is not deleted
bool 
SQLDataSetXLS::DeleteSheet(CString p_sheetName)
{
  if(m_excel) // If file is an Excel spreadsheet
  {
    // Delete sheet
    m_workbook->DeleteWorksheet(p_sheetName);
    return true;
  }
  else // if file is a text delimited file
  {
    return DeleteSheet();
  }
}

// Replace a cell into Excel spreadsheet using header row or column alphabet. 
// Set name to true if want to force column to be used as header name
bool 
SQLDataSetXLS::SetCell(CString p_cellValue, CString p_column, long p_row, bool p_name /*=true*/)
{
  int columnIndex = CalculateColumnNumber(p_column, p_name);
  if(columnIndex == 0)
  {
    return false;
  }
  if(SetCell(p_cellValue, (short)columnIndex, p_row))
  {
    return true;
  }
  return false;
}

// Set a cell value into spreadsheet using column number, row number
bool 
SQLDataSetXLS::SetCell(CString p_cellValue, short p_column, long p_row)
{
  if (p_column == 0)
  {
    m_lastError = "Column cannot be zero";
    return false;
  }
  if(p_row == 0)
  {
    m_lastError = "Row cannot be zero";
    return false;
  }
  int col = p_column - 1;
  int rec = p_row    - 2;

  if(col >= GetNumberOfFields())
  {
    m_lastError = "Column number too great";
    return false;
  }
  if(rec >= GetNumberOfRecords())
  {
    m_lastError = "Row number too great";
    return false;
  }
  // setting a header value
  if(rec == 0)
  {
    m_names[col] = p_cellValue;
  }
  else
  {
    // setting a field value
    SQLRecord* record = GetRecord(rec);
    SQLVariant* variant = record->GetField(col);
    variant->SetData(SQL_C_CHAR,p_cellValue);
  }
  if(!m_transaction)
  {
    Commit();
  }
  return true;
}

// Get a list of all worksheets in the XLS (xls only!)
bool 
SQLDataSetXLS::GetListOfWorksheets(WordList* p_sheets)
{
  if(m_excel)
  {
    int num = m_workbook->GetTotalWorkSheets(m_file);
    for(int ind = 0;ind < num; ++ind)
    {
      char name[MAX_PATH];
      m_workbook->GetSheetName(ind,name);
      CString sheetname(name);
      p_sheets->push_back(sheetname);
    }
    if(m_workbook->GetError())
    {
      m_lastError = m_workbook->GetError();
    }
    return true;
  }
  if(m_xmlExcel)
  {
    Names names;
    m_xmlWorkbook->GetSheetNames(names);
    for(unsigned ind = 0;ind < names.size();++ind)
    {
       p_sheets->push_back(names[ind]);
    }
    return true;
  }
  // Does not work for ".csv" files
  return false;
}

//////////////////////////////////////////////////////////////////////////
//
// PRIVATE METHODS
//
//////////////////////////////////////////////////////////////////////////

// Open a text delimited file for reading or writing
bool 
SQLDataSetXLS::Open()
{
  if (m_excel) // If file is an Excel spreadsheet
  {
    try
    {
      // Buffer voor de cellen
      char buffer[100];

      if(m_workbook->Load(m_file,true) ==false)
      {
        m_lastError = m_workbook->GetError();
        return false;
      }

      BasicExcelWorksheet* sheet = m_workbook->GetWorksheet(m_sheetName);
      if(sheet == NULL)
      {
        m_lastError.Format("Cannot find worksheet [%s] in XLS file: %s",m_sheetName,m_file);
        return false;
      }
      int cols = sheet->GetTotalCols();
      int rows = sheet->GetTotalRows();

      if(cols == 0 || rows <= 1)
      {
        m_lastError.Format("Empty worksheet [%s]",m_sheetName);
        return false;
      }

      // Lees de header row met cell namen
      for(int col = 0; col < cols; ++col)
      {
        CString value(sheet->CellValue(0,col,buffer,100));
        m_names.push_back(value);
        m_types.push_back(SQL_C_CHAR);
      }

      // Lees de data rows
      for(int row = 1; row < rows; ++row)
      {
        SQLRecord* record = InsertRecord();
        for(int col = 0; col < cols; ++col)
        {
          char* value = sheet->CellValue(row,col,buffer,100);
          SQLVariant* var = new SQLVariant(value);
          record->AddField(var);
        }
      }
    }
    catch(CString str)
    {
      m_lastError = "Error reading spreadhseet: " + str;
      return false;
    }
    catch (...)
    {
      m_lastError = "Error reading spreadsheet\n";
      return false;
    }
    if(m_records.size())
    {
      m_current = 0;
      m_status |= SQL_Selections;
    }
    return true;
  }
  else if(m_xmlExcel)
  {
    // 
    if(m_xmlWorkbook->Load() == false)
    {
      m_lastError = m_xmlWorkbook->GetError();
      return false;
    }
    BasicXmlWorksheet* sheet = m_xmlWorkbook->GetWorksheet(m_sheetName);
    if(sheet == NULL)
    {
      m_lastError.Format("Cannot find worksheet [%s] in XLS file: %s",m_sheetName,m_file);
      return false;
    }
    int cols = sheet->GetMaxCols();
    int rows = sheet->GetMaxRows();

    // Lees de header row met cell namen
    for(int col = 1; col <= cols; ++col)
    {
      CString value(sheet->GetCellValue(1,col));
      m_names.push_back(value);
      m_types.push_back(SQL_C_CHAR);
    }

    // Lees de data rows
    for(int row = 2; row <= rows; ++row)
    {
      SQLRecord* record = InsertRecord();
      for(int col = 1; col <= cols; ++col)
      {
        CString value = sheet->GetCellValue(row,col);
        SQLVariant var(value.GetString());
        record->AddField(&var,true); // It's for an insertion
      }
    }
    return true;

  }
  else // if file is a text delimited file
  {
    try
    {
      CFile *File = NULL;
      File = new CFile(m_file, CFile::modeRead | CFile::shareDenyNone);
      if (File != NULL)
      {
        CString tempString;
        CArchive *Archive = NULL;
        Archive = new CArchive(File, CArchive::load);
        if (Archive != NULL)
        {
          Close();
          int rows = 0;
          bool result = true;
          // Read and store all rows in memory
          m_transaction = true;
          // Read and store all rows in memory
          while(Archive->ReadString(tempString))
          {
            TRACE("INPUT: %s\n",tempString);
            CStringArray values;
            if(SplitRow(tempString,values) == false)
            {
              result = false;
              break;
            }
            if(rows++ == 0)
            {
              // Add header fields
              AddHeaders(values,true);
              continue;
            }
            AddRow(values);
            values.RemoveAll();
          }
          delete Archive;
          delete File;

          if(GetNumberOfFields() != 0)
          {
            m_append = true;
          }
          m_transaction = false;
          return result;
        }
        delete File;
      }
    }
    catch(...)
    {
    }
    m_lastError = "Error in opening file\n";
    return false;
  }
}

bool
SQLDataSetXLS::Close()
{
  m_append = false;
  SQLDataSet::Close();

  return true;
}

// Convert Excel column in alphabet into column number
// A  -> 1
// IV -> 255
int 
SQLDataSetXLS::CalculateColumnNumber(CString p_column, bool p_name /*=true*/)
{
  if(p_name)
  {
    // Check if it is a valid field name
    for (unsigned i = 0; i < m_names.size(); i++)
    {
      if(!p_column.Compare(m_names[i]))
      {
        return (i + 1);
      }
    }
    m_lastError = "Invalid field name";
    return 0;	
  }
  else
  {
    int firstLetter, secondLetter;
    p_column.MakeUpper();

    if (p_column.GetLength() == 1)
    {
      firstLetter = p_column.GetAt(0);
      // 65 is A in ascii
      return (firstLetter - 65 + 1); 
    }
    else if (p_column.GetLength() == 2)
    {
      firstLetter = p_column.GetAt(0);
      secondLetter = p_column.GetAt(1);
      // 65 is A in ascii
      return ((firstLetter - 65 + 1)*26 + (secondLetter - 65 + 1)); 
    }
  }
  m_lastError = "Invalid column alphabet";
  return 0;	
}

// Read a row from spreadsheet. 
bool 
SQLDataSetXLS::SplitRow(CString& p_input,CStringArray &p_rowValues)
{
  CString temp;
  CString tempString(p_input);
  // Read the desired row
  p_rowValues.RemoveAll();

  // Trim the row first
  TrimRow(tempString);

  // Check op voorkomen delimiter/seperator
  if(!m_separator.IsEmpty() && tempString.Find(m_separator) < 0)
  {
    m_lastError += "Kan geen scheidingsteken vinden in het CSV bestand";
    return false;
  }
  if(!m_delimLeft.IsEmpty() && tempString.Find(m_delimLeft) < 0)
  {
    m_lastError += "Kan geen linker string begrenzingsteken vinden in het CSV bestand";
    return false;
  }
  if(!m_delimRight.IsEmpty() && tempString.Find(m_delimRight) < 0)
  {
    m_lastError += "Kan geen rechter string begrenzingsteken vinden in het CSV bestand";
    return false;
  }
  if(m_delimLeft.IsEmpty() && m_separator.IsEmpty() && m_delimRight.IsEmpty())
  {
    m_lastError += "Een CSV bestand moet minimaal of een scheidingsteken of een begrenzingsteken hebben!";
    return false;
  }

  // Handel geval af waarin we alleen een scheidingsteken hebben
  // en alles ertussen gewoon een string is
  if(m_delimLeft.IsEmpty())
  {
    int pos = tempString.Find(m_separator);
    while(pos > 0)
    {
      CString deel = tempString.Left(pos);
      tempString = tempString.Mid(pos + 1);
      p_rowValues.Add(deel.Trim());
      pos = tempString.Find(m_separator);
    }
    // Hele overige string is een veld. Maar potentieel leeg, als regel op scheidingsteken eindigt
    tempString = tempString.Trim();
    if(tempString.GetLength())
    {
      p_rowValues.Add(tempString.Trim());
    }
    return true;
  }
  // Nu hebben we het geval met begrenzingstekens
  // Deze per stuk vinden en het scheidingsteken doorskippen
  int pos = tempString.Find(m_delimLeft);
  while(pos >= 0)
  {
    int pos_rechts = tempString.Find(m_delimRight,pos+1);
    if(pos_rechts < 0)
    {
      // Wel linker delimter gevonden, maar geen rechter
      m_lastError += "CSV Bestand: wel linker begrenzingsteken gevonden, maar geen rechter begrenzingsteken";
      return false;
    }
    // Deelstring er uit halen
    CString deel = tempString.Mid(pos + 1,pos_rechts - pos -1);
    tempString   = tempString.Mid(pos_rechts + 1);
    p_rowValues.Add(deel);

    // Nu over scheidingsteken heen gaan
    if(!m_separator.IsEmpty() && !tempString.IsEmpty())
    {
      pos = tempString.Find(m_separator);
      if(pos >= 0)
      {
        tempString = tempString.Mid(pos + 1);
      }
      else
      {
        m_lastError += "Geen scheidingsteken gevonden in CSV bestand";
        return false;
      }
    }
    // Volgende string
    pos = tempString.Find(m_delimLeft);
  }

  return true;
}

// Trim whitespace in between delimiters
void
SQLDataSetXLS::TrimRow(CString& p_row)
{
  bool inString = false;
  int  index    = 0;

  // Nothing to do
  if(m_delimLeft.IsEmpty() || m_delimRight.IsEmpty())
  {
    return;
  }
  while(index < p_row.GetLength())
  {
    int c = p_row.GetAt(index);
    if(inString)
    {
      if(c == m_delimRight.GetAt(0) && p_row.GetAt(index+1) == m_delimLeft.GetAt(0))
      {
        index += 2;
        c = p_row.GetAt(index);
      }
      if(c == m_delimRight.GetAt(0))
      {
        inString = false;
      }
    }
    else
    {
      if(c == m_delimLeft.GetAt(0))
      {
        inString = true;
      }
    }

    if((c == ' ' || c == '\t') && !inString)
    {
      p_row.Delete(index,1);
      continue;
    }
    // Volgende pos
    ++index;
  }
}

// Get last error message
CString 
SQLDataSetXLS::GetLastError()
{
  int pos = m_lastError.Find('\n');
  if(pos >= 0)
  {
    m_lastError.SetAt(pos,0);
  }
  return m_lastError;   
} 
