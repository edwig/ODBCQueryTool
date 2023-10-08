//////////////////////////////////////////////////////////////////////////
//
// BasicExcel
//
// Original Version: Yap Chun Wei  (2006)
// Expanded Version: Martin Fuchs  (2009) and Ami Castonguay
// Expanded Version: Long Wenbiao  (2010)
// Expanded Version: Edwig Huisman (2012)
// For more credits: See the .h interface file
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
// Version number: See SQLComponents.h
//
#include "stdafx.h"
#include "SQLComponents.h"
#include "BasicXmlExcel.h"
#include <comutil.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace SQLComponents
{

//////////////////////////////////////////////////////////////////////////
//
// BasicXmlCell
//
//////////////////////////////////////////////////////////////////////////

BasicXmlCell::BasicXmlCell(int p_row,int p_col)
{
  m_row  = p_row;
  m_col  = p_col;
  m_type = XCT_EMPTY;
  m_val.m_intval = 0;
}

// Can be called for p_type == CT_INTEGER / CT_STRING
BasicXmlCell::BasicXmlCell(int p_row,int p_col,int p_value,CellType p_type)
{
  m_row = p_row;
  m_col = p_col;
  m_val.m_intval = p_value;
  m_type = p_type;
}

BasicXmlCell::BasicXmlCell(int p_row,int p_col,double p_value)
{
  m_row = p_row;
  m_col = p_col;
  m_val.m_number = p_value;
  m_type = XCT_DOUBLE;
}

BasicXmlCell::~BasicXmlCell()
{
}

bool     
BasicXmlCell::GetCellValue(int& p_value)
{
  if(m_type == XCT_INTEGER || m_type == XCT_DATE)
  {
    p_value = m_val.m_intval;
    return true;
  }
  return false;
}

bool     
BasicXmlCell::GetCellValue(double& p_value)
{
  if(m_type == XCT_DOUBLE)
  {
    p_value = m_val.m_number;
    return true;
  }
  return false;
}

bool     
BasicXmlCell::GetCellString(int& p_value)
{
  if(m_type == XCT_STRING)
  {
    p_value = m_val.m_string;
    return true;
  }
  return false;
}

CellType
BasicXmlCell::GetCellType()
{
  return m_type;
}

//////////////////////////////////////////////////////////////////////////
//
// BasicXmlWorksheet
//
//////////////////////////////////////////////////////////////////////////

// XTOR for the sheet
BasicXmlWorksheet::BasicXmlWorksheet(BasicXmlExcel* p_workbook,XString p_sheetname)
                  :m_workbook(p_workbook)
                  ,m_sheetname(p_sheetname)
                  ,m_maxCol(0)
                  ,m_maxRow(0)
{
}

// DTOR for the sheet
BasicXmlWorksheet::~BasicXmlWorksheet()
{
  // Clean-up all the cells
  XmlCells::iterator it;
  for(it = m_cells.begin();it != m_cells.end(); ++it)
  {
    BasicXmlCell* cell = it->second;
    delete cell;
  }
  m_cells.clear();
}

XString
BasicXmlWorksheet::GetSheetName()
{
  return m_sheetname;
}

int
BasicXmlWorksheet::GetMaxRows()
{
  return m_maxRow;
}

int
BasicXmlWorksheet::GetMaxCols()
{
  return m_maxCol;
}

BasicXmlCell* 
BasicXmlWorksheet::GetCell(int p_row,int p_col)
{
  ulong cellnum = CELLNUM(p_row,p_col);
  XmlCells::iterator it = m_cells.find(cellnum);
  if(it != m_cells.end())
  {
    return it->second;
  }
  // Cell did not exist, make an empty cell
  BasicXmlCell* cell = new BasicXmlCell(p_row,p_col);
  m_cells.insert(std::make_pair(cellnum,cell));
  return cell;
}

void
BasicXmlWorksheet::Load(XMLMessage& p_msg,XMLElement* p_root)
{
  // See if there is data in the worksheet
  XMLElement* data = p_msg.FindElement(p_root,_T("sheetData"));
  if(data == NULL)
  {
    return;
  }
  // Finding all the rows of data
  XMLElement* row = p_msg.FindElement(data,_T("row"));
  while(row)
  {
    // Get the row number and preserve
    XString rowName = p_msg.GetAttribute(row,_T("r"));
    int   rowNumber = p_msg.GetAttributeInteger(row,_T("r"));
    if(rowNumber > m_maxRow)
    {
      m_maxRow = rowNumber;
    }

    // Finding the columns
    XMLElement* col = p_msg.FindElement(row,_T("c"));
    while(col)
    {
      // Getting the column name/number and preserve
      XString colName = p_msg.GetAttribute(col,_T("r"));
      int colNumber = CalculateColumnNumber(colName,rowName);
      if(colNumber > m_maxCol)
      {
        m_maxCol = colNumber;
      }
      // Find datatype
      bool isString = false;
      XString dataType = p_msg.GetAttribute(col,_T("t"));
      if(dataType && dataType.GetAt(0) == 's')
      {
        isString = true;
      }
      // Find the value of the cell
      XMLElement* val = p_msg.FindElement(col,_T("v"));
      if(val)
      {
        XString value = val->GetValue();
        BasicXmlCell* cell = NULL;
        if(isString)
        {
          int stringNum = _ttoi(value);
          cell = new BasicXmlCell(rowNumber,colNumber,stringNum,XCT_STRING);
        }
        else
        {
          if(_tcschr(value,'.') != NULL)
          {
            // Floating point
            double fl = _ttof(value);
            cell = new BasicXmlCell(rowNumber,colNumber,fl);
          }
          else
          {
            // Integer number
            int intNum = _ttoi(value);
            // It is a number, now still check if it is a date
            // // with the attribute we can get to the style reference
            dataType = p_msg.GetAttribute(col,_T("s"));
            if(dataType && !dataType.IsEmpty())
            {
              // If the value is in the style array with a date formatting, it is a date :-)
              XString formatCode =  m_workbook->GetStyleCode(_ttoi(dataType));
              if (formatCode.Compare(_T("M/D/YY")) == 0 ||
                  formatCode.Compare(_T("[$-F800]dddd\\,\\ mmmm\\ dd\\,\\ yyyy")) == 0 ||
                  formatCode.Compare(_T("yyyy/mm/dd;@")) == 0 ||
                  formatCode.Compare(_T("d/m;@")) == 0 ||
                  formatCode.Compare(_T("d/mm/yy;@")) == 0 ||
                  formatCode.Compare(_T("dd/mm/yy;@")) == 0 ||
                  formatCode.Compare(_T("[$-413]d/mmm;@")) == 0 ||
                  formatCode.Compare(_T("[$-413]dd/mmm/yy;@")) == 0 ||
                  formatCode.Compare(_T("[$-413]mmm/yy;@")) == 0 ||
                  formatCode.Compare(_T("[$-413]mmmm/yy;@")) == 0 ||
                  formatCode.Compare(_T("[$-413]d\\ mmmm\\ yyyy;@")) == 0 ||
                  formatCode.Compare(_T("[$-413]d/mmm/yyyy;@")) == 0 ||
                  formatCode.Compare(_T("[$-413]d/mmm/yy;@")) == 0 ||
                  formatCode.Compare(_T("[$-413]mmmmm;@")) == 0 ||
                  formatCode.Compare(_T("[$-413]mmmmm/yy;@")) == 0 ||
                  formatCode.Compare(_T("m/d/yyyy;@")) == 0
                )
              {
                cell = new BasicXmlCell(rowNumber, colNumber, intNum, XCT_DATE);
              }
              else
              {
                // Not a date after all, so we make an integer call :-(
                cell = new BasicXmlCell(rowNumber, colNumber, intNum, XCT_INTEGER);
              }
            }
            else
            {
            	cell = new BasicXmlCell(rowNumber,colNumber,intNum,XCT_INTEGER);
          	}
        	}
        }
        ulong cellnum = CELLNUM(rowNumber,colNumber);
        m_cells.insert(std::make_pair(cellnum,cell));
      }
      // Next column of data
      col = p_msg.GetElementSibling(col);
    }
    // Next row of data
    row = p_msg.GetElementSibling(row);
  }
}

// Convert Excel column in alphabet into column number
// A  -> 1
// IV -> 255
int
BasicXmlWorksheet::CalculateColumnNumber(XString p_column,XString p_row)
{
  int firstLetter, secondLetter;

  // Strip rownumber from column name
  p_column = p_column.Mid(0,p_column.GetLength() - p_row.GetLength());
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
  XString error;
  error.Format(_T("Invalid column alphabet name: %s"),p_column.GetString());
  m_workbook->SetError(error);
  return 0;	
}

XString  
BasicXmlWorksheet::GetCellValue(int p_row,int p_col)
{
  XString value;
  int     intValue;
  double  dblValue;

  BasicXmlCell* cell = GetCell(p_row,p_col);
  if(cell)
  {
    CellType type = cell->GetCellType();

    switch(type)
    {
      case XCT_INTEGER: if(cell->GetCellValue(intValue))
                        {
                          value.Format(_T("%d"),intValue);
                        }
                        break;
      case XCT_DOUBLE:   if(cell->GetCellValue(dblValue))
                        {
                          value.Format(_T("%f"),dblValue);
                        }
                        break;
      case XCT_STRING:  if(cell->GetCellString(intValue))
                        {
                          value = m_workbook->GetSharedString(intValue);
                        }
                        break;
      case XCT_DATE:    if(cell->GetCellValue(intValue))
                        {
                          // Converting an integer to a string date
                          BSTR bstr = NULL;
                          VarBstrFromDate(intValue, LANG_USER_DEFAULT, VAR_FOURDIGITYEARS, &bstr); 
                          value = bstr;
                          SysFreeString(bstr);
                        }
                        break;
      case XCT_EMPTY:   // Fall through
      default:          break;
    }
  }
  return value;
}
  
//////////////////////////////////////////////////////////////////////////
//
// BasicXmlExcel
//
//////////////////////////////////////////////////////////////////////////

BasicXmlExcel::BasicXmlExcel(XString p_filename)
              :m_filename(p_filename)
{
  m_namesRead = false;
  m_sheetRead = false;
  const char passwd[2] = {0};
  // Try to open zipfile without a password
  m_zip = OpenZip(const_cast<TCHAR*>(m_filename.GetString()),passwd);
  if(m_zip == NULL)
  {
    m_error.Format(_T("Cannot open the file: %s"),m_filename.GetString());
    m_error.AppendFormat(_T("\nOS Error: %d"),GetLastError());
  }
}

BasicXmlExcel::~BasicXmlExcel()
{
  // Closing ZIP file
  if(m_zip)
  {
    CloseZip(m_zip);
    m_zip = NULL;
  }

  // Dealloc memory
  for(unsigned ind = 0;ind < m_worksheets.size(); ++ind)
  {
    BasicXmlWorksheet* sheet = m_worksheets[ind];
    delete sheet;
  }
  m_worksheets.clear();
  m_sheetnames.clear();
  m_styles.clear();
  m_sharedStrings.clear();
}

XString
BasicXmlExcel::GetError()
{
  return m_error;
}

bool
BasicXmlExcel::GetSheetNames(Names& p_names)
{
  if(m_sheetRead == false)
  {
    ReadSheetNames();
  }
  p_names.clear();
  for(unsigned ind = 0;ind < m_sheetnames.size();++ind)
  {
    XString& name = m_sheetnames[ind];
    p_names.push_back(name);
  }
  return true;
}

BasicXmlWorksheet*
BasicXmlExcel::GetWorksheet(int p_index)
{
  if(p_index >= 0 || p_index < (int)m_worksheets.size())
  {
    return m_worksheets[p_index];
  }
  return NULL;
}

BasicXmlWorksheet*
BasicXmlExcel::GetWorksheet(XString p_name)
{
  for(unsigned ind = 0;ind < m_worksheets.size();++ind)
  {
    BasicXmlWorksheet* sheet = m_worksheets[ind];
    if(p_name.CompareNoCase(sheet->GetSheetName()) == 0)
    {
      return sheet;
    }
  }
  return NULL;
}

void
BasicXmlExcel::ReadSheetNames()
{
  if(m_zip == NULL)
  {
    return;
  }
  ZIPENTRY ze;
  ZRESULT  res = ZR_OK;
  int entries = 0;
  // get the number of entries
  res = GetZipItem(m_zip,-1,&ze);
  if(res == ZR_OK)
  {
    entries = ze.index;
  }
  else
  {
    SetError(res);
    return;
  }
  // Loop through all entries
  for(int ind = 0; ind < entries; ++ind)
  {
    res = GetZipItem(m_zip,ind,&ze);
    if(res != ZR_OK)
    {
      m_error.Format(_T("Cannot get entries from file: %s"),m_filename.GetString());
      SetError(res);
      break;
    }
    if(_tcsicmp(ze.name,_T("xl/workbook.xml")) == 0)
    {
      int   bufflen = ze.unc_size + 1;
      char* buffer  = reinterpret_cast<char*>(malloc(bufflen));
      if(!buffer)
      {
        SetError(ZR_MEMSIZE);
        return;
      }
      // workbook xml contains the names of the sheets
      res = UnzipItem(m_zip,ind,buffer,bufflen);
      if(res != ZR_OK)
      {
        m_error.Format(_T("Cannot read the workbook definition of: %s"),m_filename.GetString());
        SetError(res);
        free(buffer);
        break;
      }
      // delimit the buffer
      buffer[ze.unc_size] = 0;

      // BUFFER IS IN UTF-8
#ifdef UNICODE
      XString sheetInfo = DecodeUTF8ZipItem(buffer);
#else
      XString sheetInfo = DecodeStringFromTheWire(buffer);
#endif

      XMLMessage doc;
      doc.ParseMessage(sheetInfo);
      XMLElement* root = doc.GetRoot();
      if(root == NULL)
      {
        m_error.Format(_T("Workbook definition incorrect in: %s"),m_filename.GetString());
        free(buffer);
        break;
      }
      XMLElement* sheets = doc.FindElement(root,_T("sheets"));
      if(sheets == NULL)
      {
        SetError(_T("Workbook is empty. No worksheets in spreadsheet"));
        free(buffer);
        break;
      }
      XMLElement* sheet  = doc.GetElementFirstChild(sheets);
      while(sheet)
      {
        XString sheetname = doc.GetAttribute(sheet,_T("name"));
        m_sheetnames.push_back(sheetname);
        // Get next sheet
        sheet = doc.GetElementSibling(sheet);
      }
      // Ready with the buffer
      free(buffer);
      break;
    }
  }
}

#ifdef UNICODE
XString
BasicXmlExcel::DecodeUTF8ZipItem(char* p_buffer)
{
  XString info;

  int length = MultiByteToWideChar(65001,0,(LPCCH) p_buffer,-1,NULL,NULL);
  LPWSTR infobuf = info.GetBufferSetLength(length + 1);
  // Doing the 'real' conversion
  MultiByteToWideChar(65001,0,(LPCCH) p_buffer,-1,infobuf,length);
  info.ReleaseBuffer();

  return info;
}
#endif

void
BasicXmlExcel::SetError(ZRESULT p_result)
{
  TCHAR buffer[1024];
  FormatZipMessage(p_result,buffer,1024);
  if(m_error.IsEmpty())
  {
    m_error = buffer;
  }
  else
  {
    m_error += _T(" : ");
    m_error += XString(buffer);
  }
  m_error.Remove('\r');
  m_error.Replace(_T('\n'),' ');
}

void
BasicXmlExcel::SetError(XString p_error)
{
  if(m_error.IsEmpty())
  {
    m_error = p_error;
  }
  else
  {
    m_error += _T(" : ");
    m_error += p_error;
  }
  m_error.Remove('\r');
  m_error.Replace(_T('\n'),' ');
}

bool
BasicXmlExcel::Load()
{
  // Ignore return value from LoadStrings
  // After Office-2013 it shared-strings are not mandatory any more
  LoadStrings();
  // The XLSX files have an internal styles.xml, where we can find e.g. a date formatting
  LoadStyles();
  return LoadWorksheets();
}

bool
BasicXmlExcel::LoadStrings()
{
  if(m_zip == NULL)
  {
    return false;
  }
  if(m_sheetRead)
  {
    return true;
  }
  ZIPENTRY ze;
  ZRESULT  res     = ZR_OK;
  int      entries = 0;
  XString  sstName = _T("xl/sharedStrings.xml");

  // get the number of entries
  res = GetZipItem(m_zip,-1,&ze);
  if(res == ZR_OK)
  {
    entries = ze.index;
  }
  else
  {
    SetError(res);
    return false;
  }
  // Loop through all entries
  for(int ind = 0; ind < entries; ++ind)
  {
    res = GetZipItem(m_zip,ind,&ze);
    if(res != ZR_OK)
    {
      m_error.Format(_T("Cannot get entries from file: %s"),m_filename.GetString());
      SetError(res);
      return false;
    }
    if(sstName.CompareNoCase(ze.name) == 0)
    {
      int   bufflen = ze.unc_size + 1;
      char* buffer  = (char*) malloc(bufflen);
      if(!buffer)
      {
        SetError(ZR_MEMSIZE);
        return false;
      }
      // workbook xml contains the names of the sheets
      res = UnzipItem(m_zip,ind,buffer,bufflen);
      if(res != ZR_OK)
      {
        m_error.Format(_T("Cannot read the shared-strings of: %s"),m_filename.GetString());
        SetError(res);
        free(buffer);
        return false;
      }
      // delimit the buffer
      buffer[ze.unc_size] = 0;
      // BUFFER IS IN UTF-8
#ifdef UNICODE
      XString stringInfo = DecodeUTF8ZipItem(buffer);
#else
      XString stringInfo = DecodeStringFromTheWire(buffer);
#endif
      XMLMessage doc;
      doc.ParseMessage(stringInfo);
      XMLElement* root = doc.GetRoot();
      if(root == NULL)
      {
        m_error.Format(_T("Shared-strings definition incorrect in: %s"),m_filename.GetString());
        free(buffer);
        return false;
      }
      // Reading the strings
      XMLElement* si = doc.FindElement(root,_T("si"));
      while(si)
      {
        XMLElement* tt = doc.GetElementFirstChild(si);
        XString text = tt->GetValue();
        m_sharedStrings.push_back(text);
        // next string
        si = doc.GetElementSibling(si);
      }
      // Ready with the buffer
      free(buffer);
      // Ready reading strings
      return true;
    }
  }
  return false;
}

bool
BasicXmlExcel::LoadStyles()
{
  if(m_zip == NULL)
  {
    return false;
  }
  if(m_sheetRead)
  {
    return true;
  }
  ZIPENTRY ze;
  ZRESULT  res     = ZR_OK;
  int      entries = 0;
  XString  sstName = _T("xl/styles.xml");

  // get the number of entries
  res = GetZipItem(m_zip,-1,&ze);
  if(res == ZR_OK)
  {
    entries = ze.index;
  }
  else
  {
    SetError(res);
    return false;
  }
  // Loop through all entries
  for(int ind = 0; ind < entries; ++ind)
  {
    res = GetZipItem(m_zip,ind,&ze);
    if(res != ZR_OK)
    {
      m_error.Format(_T("Cannot get entries from file: %s"),m_filename.GetString());
      SetError(res);
      return false;
    }
    if(sstName.CompareNoCase(ze.name) == 0)
    {
      int   bufflen = ze.unc_size + 1;
      char* buffer  = (char*) malloc(bufflen);
      res = UnzipItem(m_zip,ind,buffer,bufflen);
      if(res != ZR_OK)
      {
        m_error.Format(_T("Cannot read the styles of: %s"),m_filename.GetString());
        SetError(res);
        free(buffer);
        return false;
      }
      // delimit the buffer
      buffer[ze.unc_size] = 0;

      // BUFFER IS IN UTF-8
#ifdef UNICODE
      XString styleInfo = DecodeUTF8ZipItem(buffer);
#else
      XString styleInfo = DecodeStringFromTheWire(buffer);
#endif
      XMLMessage doc;
      doc.ParseMessage(styleInfo);
      XMLElement* root = doc.GetRoot();
      if(root == NULL)
      {
        m_error.Format(_T("Styles definition incorrect in: %s"),m_filename.GetString());
        free(buffer);
        return false;
      }
      std::vector<XString> styleFormats;
      // Reading the styles
      // The element 'cellxfs' have the 's' attribute values for a cell's formatting ID
      // that we can use to look up the formatting code
      XMLElement* cellformat = doc.FindElement(root,_T("cellXfs"));
      if (cellformat)
      {
        for (auto& elem : cellformat->GetChildren())
        {
          for (auto& attrib : elem->GetAttributes())
          {
            if (attrib.m_name == _T("numFmtId"))
            {
              styleFormats.push_back(attrib.m_value);
              break;
            }
          }
        }
      }
      // Find the cell formats for the corresponding code
      for (int i = 0; i < (int)styleFormats.size(); i++)
      {
        // Some standard values do not have a separate code, so we make one for them
        if (styleFormats[i] == _T("14") || styleFormats[i] == _T("15")||styleFormats[i] == _T("16")||styleFormats[i] == _T("17") )
        {
          // A date format, convert to string
          m_styles.insert(std::make_pair(i, _T("dd/mm/yy;@")));
        }
        else
        {
          XMLElement* nummerFormatId = doc.FindElementByAttribute(_T("numFmtId"), styleFormats[i]);
          if (nummerFormatId)
          {
            XMLAttribute* FormatCode = doc.FindAttribute(nummerFormatId, _T("formatCode"));
            if (FormatCode)
            {
              m_styles.insert(std::make_pair(i, FormatCode->m_value));
            }
            else
            {
              // Should not happen!
              m_styles.insert(std::make_pair(i, _T("No format code found: ") + styleFormats[i]));
            }
          }
          else
          {
            // SHould not happen!
            m_styles.insert(std::make_pair(i, _T("No format code found: ") + styleFormats[i]));
          }
        }
      }
      // Ready with the buffer
      free(buffer);
      // Ready reading styles
      return true;
    }
  }
  return false;
}

bool
BasicXmlExcel::LoadWorksheets()
{
  if(m_zip == NULL)
  {
    return false;
  }
  if(m_sheetRead)
  {
    return true;
  }
  // Make sure we have the sheetnames
  ReadSheetNames();

  // Variables
  ZIPENTRY ze;
  ZRESULT  res = ZR_OK;
  int entries = 0;

  // get the number of entries
  res = GetZipItem(m_zip,-1,&ze);
  if(res == ZR_OK)
  {
    entries = ze.index;
  }
  else
  {
    SetError(res);
    return false;
  }
  // Loop through all worksheets
  for(unsigned sheetnum = 0; sheetnum < m_sheetnames.size(); ++sheetnum)
  {
    XString sheetName;
    sheetName.Format(_T("xl/worksheets/sheet%u.xml"),sheetnum + 1);

    // Loop through all entries
    for(int ind = 0; ind < entries; ++ind)
    {
      res = GetZipItem(m_zip,ind,&ze);
      if(res != ZR_OK)
      {
        m_error.Format(_T("Cannot get entries from file: %s"),m_filename.GetString());
        SetError(res);
        return false;
      }
      if(sheetName.CompareNoCase(ze.name) == 0)
      {
        int   bufflen = ze.unc_size + 1;
        char* buffer  = (char*) malloc(bufflen);
        if(!buffer)
        {
          SetError(ZR_MEMSIZE);
          return false;
        }
        // workbook xml contains the names of the sheets
        res = UnzipItem(m_zip,ind,buffer,bufflen);
        if(res != ZR_OK)
        {
          m_error.Format(_T("Cannot read the worksheet definition of: %s"),m_filename.GetString());
          SetError(res);
          free(buffer);
          return false;
        }
        // delimit the buffer
        buffer[ze.unc_size] = 0;
        // BUFFER IS IN UTF-8
#ifdef UNICODE
        XString sheetInfo = DecodeUTF8ZipItem(buffer);
#else
        XString sheetInfo = DecodeStringFromTheWire(buffer);
#endif
        XMLMessage doc;
        doc.ParseMessage(sheetInfo);
        XMLElement* root = doc.GetRoot();
        if(root == NULL)
        {
          m_error.Format(_T("Worksheet definition incorrect in: %s"),m_filename.GetString());
          free(buffer);
          return false;
        }
        // Make a new worksheet
        BasicXmlWorksheet* sheet = new BasicXmlWorksheet(this,m_sheetnames[sheetnum]);
        m_worksheets.push_back(sheet);
        
        // Load data in the worksheet
        sheet->Load(doc,root);

        // Ready with the buffer
        free(buffer);
        // Break inner loop: next worksheet please
        break;
      }
    }
  }
  m_sheetRead = true;
  // See if we did read some worksheets
  return m_worksheets.size() > 0;
}

XString
BasicXmlExcel::GetSharedString(int p_string)
{
  XString value;
  if(p_string >= 0 && p_string < (int)m_sharedStrings.size())
  {
    value = m_sharedStrings[p_string];
  }
  return value;
}

XString
BasicXmlExcel::GetStyleCode(int id)
{
  return m_styles.find(id)->second;
}

// End of namespace
}
