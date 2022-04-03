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
#pragma once
#include "unzip.h"
#include <vector>
#include <map>
#include "XMLMessage.h"

// Macro's for finding the cell's row/col
#define CELLNUM(row,col)  (((col) << 20) | (row))
#define ROWNUM(cellnum)   (cellnum & 0xFFFFF)
#define COLNUM(cellnum)   (cellnum >> 20)

namespace SQLComponents
{

// Forward definition
class BasicXmlExcel;

// Type of cells
typedef enum _celltype
{
  XCT_EMPTY
 ,XCT_INTEGER
 ,XCT_DOUBLE
 ,XCT_STRING
 ,XCT_DATE
 ,XCT_FORMULA
}
CellType;

class BasicXmlCell
{
public:
  // new empty cell
  BasicXmlCell(int p_row,int p_col);                    
  // new integer cell or string cell
  BasicXmlCell(int p_row,int p_col,int p_value,CellType p_type);
  // new double cell
  BasicXmlCell(int p_row,int p_col,double p_value);   
  // DTOR
 ~BasicXmlCell();

  // Getters
  CellType GetCellType();
  bool     GetCellValue (int&    p_value);
  bool     GetCellValue (double& p_value);
  bool     GetCellString(int&    p_value);
private:
  int       m_row;
  int       m_col;
  CellType  m_type;
  union _val
  {
    int      m_intval;
    double   m_number;
    int      m_string;  // Shared string number
  }
  m_val;
};

typedef unsigned long                 ulong;
typedef std::map<ulong,BasicXmlCell*> XmlCells;
typedef std::vector<XString>          Names;

class BasicXmlWorksheet
{
public:
  BasicXmlWorksheet(BasicXmlExcel* p_workbook,XString p_sheetname);
 ~BasicXmlWorksheet();

  // OPERATIONS
  void          Load(XMLMessage& p_msg,XMLElement* p_root);

  // GETTERS
  XString       GetSheetName();
  int           GetMaxRows();
  int           GetMaxCols();
  BasicXmlCell* GetCell     (int p_row,int p_col);
  XString       GetCellValue(int p_row,int p_col);
private:
  int           CalculateColumnNumber(XString p_column,XString p_row);

  BasicXmlExcel*  m_workbook;
  XString         m_sheetname;
  int             m_maxRow;
  int             m_maxCol;
  // Storage of the sheet
  XmlCells        m_cells;
};

typedef std::vector<BasicXmlWorksheet*> WorkSheets;

class BasicXmlExcel
{
public:
  BasicXmlExcel(XString p_filename);
 ~BasicXmlExcel();

  // OPERATIONS
  bool Load();

  // SETTERS
  void SetError(ZRESULT p_result);
  void SetError(XString p_error);

  // GETTERS
  bool                GetSheetNames(Names& p_names);
  BasicXmlWorksheet*  GetWorksheet(int p_index);
  BasicXmlWorksheet*  GetWorksheet(XString p_name);
  XString             GetSharedString(int p_string);
  XString             GetStyleCode(int id);
  XString             GetError();
  
private:
  // Read in the worksheetnames
  void       ReadSheetNames();
  bool       LoadStrings();
  bool       LoadStyles();
  bool       LoadWorksheets();

  XString    m_filename;      // Filename
  HZIP       m_zip;           // ZIP handle for unzipping
  Names      m_sheetnames;
  Names      m_sharedStrings;
  std::map <int,XString> m_styles;
  WorkSheets m_worksheets;
  // status
  bool       m_namesRead;
  bool       m_sheetRead;
  XString    m_error;
};

// End of namespace
}
