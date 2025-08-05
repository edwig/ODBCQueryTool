////////////////////////////////////////////////////////////////////////
//
// File: GridView.cpp
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
#include "pch.h"
#include "OEDocument.h"
#include "QueryTool.h"
#include "GridView.h"
#include "resource.h"
#include "OEView.h"
#include "OESettings.h"
#include "BasicExcel.h"
#include "ExcelFormat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_FNAME_LEN 256

using namespace SQLComponents;
using namespace ExcelFormat;

const GridType initGrid[] =
{
  // Query Grid
  2,  { {  60, 1, _T("Query")   },
        {  60, 0, _T("Column 1")} 
      },
  // Statistics
  3,  { {  60, 1, _T("Step")          },
        {  60, 1, _T("Seconds")       }, 
        { 300, 0, _T("Completed step")} 
      },
  // Output
  4,  { {  40, 1, _T("Line")          },
        {  30, 0, _T("Status")        },
        {  60, 1, _T("Affected rows") },
        { 200, 0, _T("Result")        }
      },
  // History
  3,  { {   40, 1, _T("Number")          },
        {   40, 1, _T("Rerun")           },
        { 1000, 0, _T("ODBC SQL Command")}
      }
};

/////////////////////////////////////////////////////////////////////////////
// CGridView

//IMPLEMENT_DYNCREATE(CGridView, CView)

BEGIN_MESSAGE_MAP(CGridView, CView)
	//{{AFX_MSG_MAP(CGridView)
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
  ON_WM_DESTROY()
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
//ON_COMMAND(ID_TOGGLE_READONLY, OnToggleReadonly)
  ON_NOTIFY(GVN_ENDLABELEDIT,  ID_QUERY_VIEW,OnEndInPlaceEdit)
  ON_NOTIFY(GVN_BEGINLABELEDIT,ID_QUERY_VIEW,OnBeginEdit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridView construction/destruction

CGridView::CGridView()
{
	m_pGridCtrl       = NULL;
  m_type            = TYPE_QUERY;
  m_autoExpand      = true;
  m_prefetchLines   = 0;
  m_queryTerminator = _T("");
}

void 
CGridView::PostNcDestroy()
{
  // No self destruct;
}

void
CGridView::OnDestroy()
{
 if (m_pGridCtrl)
 {
   delete m_pGridCtrl;
  }
}

BOOL CGridView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CGridView drawing

void CGridView::OnDraw(CDC* pDC)
{
	//COEDocument* pDoc = GetDocument();
	//ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CGridView printing

BOOL CGridView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGridView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	if (m_pGridCtrl)
		m_pGridCtrl->OnBeginPrinting(pDC, pInfo);
}

void CGridView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	if (m_pGridCtrl)
		m_pGridCtrl->OnPrint(pDC, pInfo);
}

void CGridView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	if (m_pGridCtrl)
		m_pGridCtrl->OnEndPrinting(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CGridView diagnostics

#ifdef _DEBUG
void CGridView::AssertValid() const
{
	CView::AssertValid();
}

void CGridView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

COEDocument* CGridView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COEDocument)));
	return (COEDocument *)m_pDocument;
}

/////////////////////////////////////////////////////////////////////////////
// CGridView message handlers

void CGridView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	if (m_pGridCtrl == NULL)
	{
		// Create the Gridctrl object
		m_pGridCtrl = new MCGridCtrl;
		if (!m_pGridCtrl) 
    {
      return;
    }
    m_pGridCtrl->RegisterView(this);
    
		// Create the Gridctrl window
		CRect rect;
		GetClientRect(rect);
    
    int id = GetDlgCtrlID();
		m_pGridCtrl->Create(rect, this, id);

    m_imageList.Create(MAKEINTRESOURCE(IDB_QUERYSTATES), 16, 1, RGB(255,255,255));
  	m_pGridCtrl->SetImageList(&m_imageList);

    SetInitialFont();
	}
  InitGridEmpty(m_type);
}

void
CGridView::SetInitialFont()
{
  // Get QueryFont
  const OpenEditor::SettingsManager& manager  = COEDocument::GetSettingsManager();
  const OpenEditor::GlobalSettings&  settings = manager.GetGlobalSettings();
  CString queryFont = settings.GetSQLQueryFont();

  // Split current font registry in name and size
  CString fontName;
  int     fontSize = 0;
  int pos = queryFont.Find(';');
  if(pos)
  {
    fontName = queryFont.Left(pos);
    fontSize = _ttoi(queryFont.Mid(pos+1));
  }
  if(fontName.IsEmpty())
  {
    fontName = _T("Tahoma");
  }
  if(fontSize < 8 ) fontSize = 8;
  if(fontSize > 24) fontSize = 24;

  // Set the font
  CFont font;
  font.CreatePointFont(fontSize * 10,fontName);
  m_pGridCtrl->SetFont(&font,TRUE);
}

void CGridView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	if (m_pGridCtrl->GetSafeHwnd())
	{
		CRect rect;
		GetClientRect(rect);
		m_pGridCtrl->MoveWindow(rect);
    m_pGridCtrl->ExpandLastColumn();
	}
}

BOOL CGridView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
    if (m_pGridCtrl && IsWindow(m_pGridCtrl->m_hWnd))
        if (m_pGridCtrl->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
            return TRUE;

	return CView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CGridView::OnToggleReadonly()
{
  if (m_pGridCtrl && IsWindow(m_pGridCtrl->m_hWnd))
  {
    MCCellID cell = m_pGridCtrl->GetFocusCell();
    if (m_pGridCtrl->IsValid(cell))
    {
      int nState = m_pGridCtrl->GetItemState(cell.row, cell.col);
      if (m_pGridCtrl->IsCellEditable(cell))
      {
        m_pGridCtrl->SetItemState(cell.row, cell.col, nState | GVIS_READONLY);
      }
      else
      {
        m_pGridCtrl->SetItemState(cell.row, cell.col, nState & ~GVIS_READONLY);
      }
    }
  }
}

BOOL CGridView::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
	//return CView::OnEraseBkgnd(pDC);
}


void 
CGridView::RegisterEditor(CWnd *editor,int p_type)
{
  m_editorView = (COEditorView *) editor;
  m_type = p_type;
}

void 
CGridView::InitGridEmpty(int p_type,bool nofirst /* = false */)
{
  if(!m_pGridCtrl)
  {
    return;
  }
	// Set standard
	m_pGridCtrl->SetEditable(TRUE); // DEBUG
	m_pGridCtrl->EnableDragAndDrop(TRUE);
	m_pGridCtrl->SetHeaderSort(TRUE);

  // Delete previous results
  while(m_pGridCtrl->GetRowCount() > 1)
  {
    m_pGridCtrl->DeleteRow(m_pGridCtrl->GetRowCount() - 1);
  }
  while(m_pGridCtrl->GetColumnCount() > 1)
  {
    m_pGridCtrl->DeleteColumn(m_pGridCtrl->GetColumnCount() - 1);
  }

  // Set empty grid
  try
  {
    m_pGridCtrl->SetRowCount(1); //nofirst ? 1 : 2);
    m_pGridCtrl->SetColumnCount(initGrid[p_type].cols);
		m_pGridCtrl->SetFixedRowCount(1);
		m_pGridCtrl->SetFixedColumnCount(1);
  }
  catch(CMemoryException* e)
  {
    e->ReportError();
    e->Delete();
    return;
  }
  
  for(int col = 0; col < initGrid[p_type].cols; ++col)
  {
    if(nofirst)
    {
      if(col) break;
    }
    MC_ITEM item;
    item.mask = GVIF_TEXT | GVIF_FORMAT;
    item.row = 0;
    item.col = col;
		item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX;
    item.strText = initGrid[p_type].inital[col].name;
    if(initGrid[p_type].inital[col].sortNum)
    {
      item.nFormat |= DT_SORT_NUMERIC;
    }
    m_pGridCtrl->SetItem(&item);
    m_pGridCtrl->SetColumnWidth(col,initGrid[p_type].inital[col].size);
  }
  // Sync with the outside world
	m_pGridCtrl->AutoSize();
  m_pGridCtrl->ExpandLastColumn();
}

void
CGridView::InsertColumn(LPCTSTR p_heading,UINT p_format,int p_column)
{
  m_pGridCtrl->InsertColumn(p_heading,p_format,p_column);
}

void
CGridView::InsertRow(LPCTSTR p_heading,int p_row)
{
  m_pGridCtrl->InsertRow(p_heading,p_row);
}

void
CGridView::DeleteColumn(int p_column)
{
  m_pGridCtrl->DeleteColumn(p_column);
}

void
CGridView::SetRowCount(int p_rows)
{
  m_pGridCtrl->SetRowCount(p_rows);
}

int
CGridView::AppendRow(LPCTSTR p_heading)
{
  int rows = m_pGridCtrl->GetRowCount();
  m_pGridCtrl->InsertRow(p_heading,-1);
  return rows;
}

void
CGridView::InsertItem(int p_row,int p_col,CString& p_text,UINT p_format /* = 0 */,int p_sqlType /*=0*/)
{
  MC_ITEM item;
  item.mask    = GVIF_TEXT | GVIF_FORMAT;
  item.row     = p_row;
  item.col     = p_col;
  item.strText = p_text;
  item.nFormat = p_format ? p_format : DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX;
  item.sqlType = 0;
  if(p_sqlType)
  {
    item.mask   |= GVIF_SQLTYPE;
    item.sqlType = p_sqlType;
  }
  m_pGridCtrl->SetItem(&item);
}

void
CGridView::InsertImage(int p_row,int p_col,int p_image)
{
  MC_ITEM item;
  item.mask   = GVIF_IMAGE;
  item.row    = p_row;
  item.col    = p_col;
  item.iImage = p_image;
  m_pGridCtrl->SetItem(&item);
}

CString
CGridView::GetItemText(int p_row,int p_col)
{
  return m_pGridCtrl->GetItemText(p_row,p_col);
}

int
CGridView::GetColumnCount()
{
  return m_pGridCtrl->GetColumnCount();
}

void
CGridView::SetItemText(int p_row,int p_col,CString p_text)
{
  m_pGridCtrl->SetItemText(p_row,p_col,(LPCTSTR)p_text);
}

void
CGridView::SizeColumns(int p_style)
{
  CWaitCursor take_a_deep_sigh;

  m_pGridCtrl->AutoSizeColumns(p_style); // GVS_HEADER);
  m_pGridCtrl->Refresh();
}

void
CGridView::AutoSize()
{
  m_pGridCtrl->AutoSize();
  if(m_autoExpand)
  {
    m_pGridCtrl->ExpandLastColumn();
  }
}

void    
CGridView::GotoQueryBegin()
{
  int row  = 1;
  int col;
  MCCellID cell = m_pGridCtrl->GetFocusCell();
  col = (cell.col == -1) ? 0 : cell.col;
  m_pGridCtrl->SetFocusCell(row,col);
  cell = m_pGridCtrl->GetFocusCell();
  m_pGridCtrl->EnsureVisible(cell);

  // Set this cell to the selected state
  row = cell.row;
  col = cell.col;
  m_pGridCtrl->SetSelectedRange(row,col,row,col,true,true);
}

void    
CGridView::GotoQueryPrevPage()
{
  CRect rect;
  GetClientRect(&rect);
  int totHigh  = rect.bottom;
  int celHigh  = m_pGridCtrl->GetRowHeight(0);
  int pageHigh = totHigh / celHigh;
  while(pageHigh > 1)
  {
    GotoQueryPrev();
    --pageHigh;
  }
}

void    
CGridView::GotoQueryPrev()
{
  int row,col;
  int rows = m_pGridCtrl->GetRowCount();
  MCCellID cell = m_pGridCtrl->GetFocusCell();
  row = (cell.row == -1) ? rows - 1 : cell.row;
  col = (cell.col == -1) ? 1 : cell.col;
  if(row > 1)
  {
    --row;
    m_pGridCtrl->SetFocusCell(row,col);
    cell = m_pGridCtrl->GetFocusCell();
    m_pGridCtrl->EnsureVisible(cell);

    // Set this cell to the selected state
    row = cell.row;
    col = cell.col;
    m_pGridCtrl->SetSelectedRange(row,col,row,col,true,true);
  }
}


void    
CGridView::GotoQueryNext()
{
  int row,col;
  int rows = m_pGridCtrl->GetRowCount();

  MCCellID cell = m_pGridCtrl->GetFocusCell();
  row = (cell.row == -1) ? rows - 1 : cell.row;
  col = (cell.col == -1) ? 1 : cell.col;
  if(row < rows-1)
  {
    ++row;
    m_pGridCtrl->SetFocusCell(row,col);
    cell = m_pGridCtrl->GetFocusCell();
    m_pGridCtrl->EnsureVisible(cell);

    // Set this cell to the selected state
    row = cell.row;
    col = cell.col;
    m_pGridCtrl->SetSelectedRange(row,col,row,col,true,true);
  }
  else
  {
    // Detect query to read in
    if(m_editorView->LinesFetchedSoFar())
    {
      m_editorView->ReadRestOfQuery();
    }
  }
}

void    
CGridView::GotoQueryNextPage()
{
  CRect rect;
  GetClientRect(&rect);
  int totHigh  = rect.bottom;
  int celHigh  = m_pGridCtrl->GetRowHeight(0);
  int pageHigh = totHigh / celHigh;
  while(pageHigh > 1)
  {
    GotoQueryNext();
    --pageHigh;
  }
}

void
CGridView::GotoQueryEnd()
{
  // Detect query to read in
  if(m_editorView->LinesFetchedSoFar())
  {
    m_editorView->ReadRestOfQuery();
  }

  // Goto end
  int rows = m_pGridCtrl->GetRowCount();
  int row  = (rows >= 1) ? rows - 1 : 0;
  int col;
  MCCellID cell = m_pGridCtrl->GetFocusCell();

  col = (cell.col == -1) ? 0 : cell.col;
  m_pGridCtrl->SetFocusCell(row,col);
  cell = m_pGridCtrl->GetFocusCell();
  m_pGridCtrl->EnsureVisible(cell);

  // Set this cell to the selected state
  row = cell.row;
  col = cell.col;
  m_pGridCtrl->SetSelectedRange(row,col,row,col,true,true);
}

bool
CGridView::AtBegin()
{
  if(m_pGridCtrl)
  {
    MCCellID cell = m_pGridCtrl->GetFocusCell();
    int rows = m_pGridCtrl->GetRowCount() - 1;
    if(! rows || (cell.row == -1)) return true;
    return (cell.row <= 1);
  }
  return false;
}

bool
CGridView::AtEnd()
{
  if(m_pGridCtrl)
  {
    if(m_prefetchLines)
    {
      // Still lines to be fetched
      return false;
    }
    MCCellID cell = m_pGridCtrl->GetFocusCell();
    int rows = m_pGridCtrl->GetRowCount() -1;
    if(! rows || (cell.row == -1)) return true;
    return (cell.row >= rows);
  }
  return false;
}

// Is it filled? (beside of a header)
bool
CGridView::IsFilled()
{
  if(m_pGridCtrl)
  {
    return (m_pGridCtrl->GetRowCount() > 1);
  }
  return false;
}

bool
CGridView::ReadRestOfQuery()
{
  // Detect query to read in
  if(m_editorView->LinesFetchedSoFar())
  {
    m_editorView->ReadRestOfQuery();
    m_prefetchLines = 0;
    return true;
  }
  return false;
}

/////////////////////////////////////////////////////////////////////////////////////
//
// EXPORT TO MS-Excel
// Using General SYLK format
//
static TCHAR *excel_header = 
  _T("ID;PWXL;N;E\n")
  _T("P;PGeneral\n")
  _T("P;P0\n")
  _T("P;P0.00\n")
  _T("P;P#,##0\n")
  _T("P;P#,##0.00\n")
  _T("P;P#,##0_-;;#,##0\\-\n")
  _T("P;P#,##0_-;;[Red]#,##0\\-\n")
  _T("P;P#,##0.00_-;;#,##0.00\\-\n")
  _T("P;P#,##0.00_-;;[Red]#,##0.00\\-\n")
  _T("P;P\"$\"\\ #,##0_-;;\"$\"\\ #,##0\\-\n")
  _T("P;P\"$\"\\ #,##0_-;;[Red]\"$\"\\ #,##0\\-\n")
  _T("P;P\"$\"\\ #,##0.00_-;;\"$\"\\ #,##0.00\\-\n")
  _T("P;P\"$\"\\ #,##0.00_-;;[Red]\"$\"\\ #,##0.00\\-\n")
  _T("P;P0%\n")
  _T("P;P0.00%\n")
  _T("P;P0.00E+00\n")
  _T("P;P##0.0E+0\n")
  _T("P;P#\\ ?/?\n")
  _T("P;P#\\ ??/??\n")
  _T("P;Pd/m/yyyy\n")
  _T("P;Pd/mmm/yy\n")
  _T("P;Pd/mmm\n")
  _T("P;Pmmm/yy\n")
  _T("P;Ph:mm\\ AM/PM\n")
  _T("P;Ph:mm:ss\\ AM/PM\n")
  _T("P;Ph:mm\n")
  _T("P;Ph:mm:ss\n")
  _T("P;Pd/m/yyyy\\ h:mm\n")
  _T("P;Pmm:ss\n")
  _T("P;Pmm:ss.0\n")
  _T("P;P@\n")
  _T("P;P[h]:mm:ss\n")
  _T("P;P_-\"$\"\\ * #,##0_-;;_-\"$\"\\ * #,##0\\-;;_-\"$\"\\ * \"-\"_-;;_-@_-\n")
  _T("P;P_-* #,##0_-;;_-* #,##0\\-;;_-* \"-\"_-;;_-@_-\n")
  _T("P;P_-\"$\"\\ * #,##0.00_-;;_-\"$\"\\ * #,##0.00\\-;;_-\"$\"\\ * \"-\"??_-;;_-@_-\n")
  _T("P;P_-* #,##0.00_-;;_-* #,##0.00\\-;;_-* \"-\"??_-;;_-@_-\n")
  _T("P;FArial;M200\n")
  _T("P;FArial;M200\n")
  _T("P;FArial;M200\n")
  _T("P;FArial;M200\n")
  _T("F;P0;DG0G8;M255\n")
  _T("B;Y3;X7;D0 0 2 6\n")
  _T("O;L;D;V0;K47;G100 0.001\n")
;

static TCHAR* excel_footer = _T("E\n");

void
CGridView::ExportToExcel()
{
  ExportToExcelXLS();
}

using namespace YExcel;
using namespace ExcelFormat;

void
CGridView::ExportToExcelXLS()
{
  TCHAR tempdir [MAX_FNAME_LEN + 10];
  TCHAR filename[MAX_FNAME_LEN + 10];
  TCHAR newname [MAX_FNAME_LEN + 10];
  CWaitCursor take_a_deep_sigh;

  if (!GetTempPath(MAX_FNAME_LEN, tempdir))
  {
    AfxMessageBox(_T("Cannot find a temporary directory for 'Excel-Export'"), MB_OK);
    return;
  }
  if (!GetTempFileName(tempdir, _T("ODBC_Excel"), 0, filename))
  {
    AfxMessageBox(_T("Cannot find a temporary file for 'Excel-Export'"), MB_OK);
  }

  BasicExcel excel;
  excel.New(1);
  BasicExcelWorksheet* sheet = excel.GetWorksheet(0);
  XLSFormatManager mngr(excel);
  ExcelFont bold_font;
  bold_font._weight = FW_BOLD;

  CellFormat dateFormat(mngr);
  dateFormat.set_format_string(_T("D-M-YY"));
  CellFormat timeFormat(mngr);
  timeFormat.set_format_string(_T("h:mm:ss"));
  CellFormat stampFormat(mngr);
  stampFormat.set_format_string(_T("YY-M-D h:mm:ss"));

  CellFormat resultOK(mngr);
  resultOK.set_color1(COLOR1_PAT_SOLID);
  resultOK.set_color2(MAKE_COLOR2(EGA_GREEN,0));
  resultOK.set_font(bold_font);

  CellFormat resultError(mngr);
  resultError.set_color1(COLOR1_PAT_SOLID);
  resultError.set_color2(MAKE_COLOR2(EGA_RED,0));
  resultError.set_font(bold_font);

  // Print out the rows one by one
  for (int y = 0; y < m_pGridCtrl->GetRowCount(); ++y)
  {
    for (int x = 0; x < m_pGridCtrl->GetColumnCount(); ++x)
    {
      BasicExcelCell* cell = sheet->Cell(y,x);
      MCGridCellBase* gridcell = m_pGridCtrl->GetCell(y,x);
      if(gridcell)
      {
        SQLVariant var;
        CString text = gridcell->GetText();
        int sqlType  = gridcell->GetSQLType();
        int image    = gridcell->GetImage();

        // Numbering the records
        if(x == 0 && y > 0)
        {
          // Row Header is a number
          sqlType = SQL_C_LONG;
        }
        switch(sqlType)
        {
          case 0:             if(x == 1 && y > 0 && text.IsEmpty())
                              {
                                cell->SetInteger(image);
                                cell->SetFormat(image ? resultError : resultOK);
                                break;
                              }
          case SQL_C_CHAR:    // Fall through!
          case SQL_C_BINARY:  
          case SQL_C_GUID:    cell->Set(text);
                              break;
          case SQL_C_TINYINT:
          case SQL_C_STINYINT:
          case SQL_C_UTINYINT:
          case SQL_C_SHORT:
          case SQL_C_SSHORT:
          case SQL_C_USHORT:
          case SQL_C_LONG:
          case SQL_C_SLONG:     
          case SQL_C_ULONG:   if(!text.IsEmpty())
                              {
                                cell->SetInteger(_ttoi(text));
                              }
                              break;
          case SQL_C_FLOAT:
          case SQL_C_DOUBLE:
          case SQL_C_SBIGINT:
          case SQL_C_UBIGINT:
          case SQL_C_NUMERIC: if(!text.IsEmpty())
                              {
                                cell->SetDouble(_ttof(text));
                              }
                              break;

          case SQL_C_DATE:
          case SQL_C_TYPE_DATE: var.SetData(sqlType, text);
                                var.GetAsString(text);
                                cell->Set(text);
                                cell->SetFormat(dateFormat);
                                break;
          case SQL_C_TIME:
          case SQL_C_TYPE_TIME: var.SetData(sqlType, text);
                                var.GetAsString(text);
                                cell->Set(text);
                                cell->SetFormat(timeFormat);
                                break;
          case SQL_C_TIMESTAMP: 
          case SQL_C_TYPE_TIMESTAMP:  
                                var.SetData(sqlType,text);
                                var.GetAsString(text);
                                cell->Set(text);
                                cell->SetFormat(stampFormat);
                                break;
          // ALL INTERVAL TYPES HAVE NO REPRESENTATIVE TYPES IN MS-EXCEL
          // SO NO FORMATTING STRINGS YET. 
          // TO BE RESEARCHED!!
          default:              // var.SetData(sqlType, text);
                                // var.GetAsString(text);
                                cell->Set(text);
                                break;
        }
      }
    }
  }
  // Now setting the first row to bold font
  // Create cell formatter
  CellFormat formatter(mngr);
  formatter.set_font(bold_font);

  // Blue background
  CellFormat collformat(mngr);
  collformat.set_color1(COLOR1_PAT_SOLID);
  collformat.set_color2(MAKE_COLOR2(EGA_CYAN, 0));
  collformat.set_font(bold_font);

  // Format the first row of cells to bold-font
  for (int col = 0; col < m_pGridCtrl->GetColumnCount(); ++col)
  {
    BasicExcelCell* cell = sheet->Cell(0,col);
    cell->SetFormat(formatter);
    cell->SetFormat(collformat);
  }

  for(int row = 0;row < m_pGridCtrl->GetRowCount(); ++row)
  {
    BasicExcelCell* cell = sheet->Cell(row,0);
    cell->SetFormat(collformat);
  }

  // Now save it
  excel.SaveAs(filename);
  excel.Close();

  // Now rename to .xls
  _tcscpy_s(newname,MAX_FNAME_LEN,filename);
  TCHAR *xls = _tcsstr(newname, _T(".tmp"));
  if (xls)
  {
    _tcscpy_s(xls,5,_T(".xls"));
    _trename(filename, newname);
  }
  // Startup with Excel
  QueryToolApp* app = (QueryToolApp *)AfxGetApp();
  app->RunShellCommand(tempdir, _T("open"), newname, NULL);
}

void
CGridView::ExportToExcelSLK()
{
  TCHAR tempdir [MAX_FNAME_LEN + 1];
  TCHAR filename[MAX_FNAME_LEN + 1];
  TCHAR newname [MAX_FNAME_LEN + 1];
  CWaitCursor take_a_deep_sigh;

  if(!GetTempPath(MAX_FNAME_LEN,tempdir))
  {
    AfxMessageBox(_T("Cannot find a temporary directory for 'Excel-Export'"),MB_OK);
    return;
  }
  if(!GetTempFileName(tempdir,_T("ODBC_Excel"),0,filename))
  {
    AfxMessageBox(_T("Cannot find a temporary file for 'Excel-Export'"),MB_OK);
  }

  WinFile file(filename);
  if(!file.Open(winfile_write | open_trans_text,FAttributes::attrib_none,Encoding::UTF8))
  {
    AfxMessageBox(_T("Cannot open a temporary file for 'Excel-Export'"),MB_OK);
    return;
  }
  // Print the header wit formats
  CString header(excel_header);
  file.Write(header);

  // Print out the rows one by one
  for(int y = 0; y < m_pGridCtrl->GetRowCount(); ++y)
  {
    for(int x = 0; x < m_pGridCtrl->GetColumnCount(); ++x)
    {
//    CString text = m_pGridCtrl->GetItemText(y,x);
//    text.Replace(_T(";"),_T(";;"));
      if(x == 0)
      {
        file.Format(_T("C;Y%d;X%d;K\"%s\"\n"),y + 1,x + 1,m_pGridCtrl->GetItemText(y,x).GetString());
      }
      else
      {
        file.Format(_T("C;X%d;K\"%s\"\n"),x + 1,m_pGridCtrl->GetItemText(y,x).GetString());
      }
    }
  }
  // Print the ending flag
  CString footer(excel_footer);
  file.Write(footer);
  // finally close the directory
  file.Close();

  // Now rename to .slk
  _tcscpy_s(newname,MAX_FNAME_LEN,filename);
  TCHAR *slk = _tcsstr(newname,_T(".tmp")); 
  if(slk)
  {
    _tcscpy_s(slk,5,_T(".slk"));
    _trename(filename,newname);
  }
  // Startup with Excel
  QueryToolApp* app = (QueryToolApp*) AfxGetApp();
  app->RunShellCommand(tempdir,_T("open"),newname,NULL);
}

static TCHAR* html_header = 
  _T("<HTML>\n")
  _T("<BODY>\n")
  _T("<TABLE BORDER=\"1\" CELLPADDING=\"1\" CELLSPACING=\"0\">\n");

static TCHAR* html_footer = 
  _T("</TABLE>\n")
  _T("</BODY>\n")
  _T("</HTML>\n");

void
CGridView::ExportToHTML()
{
  TCHAR tempdir [MAX_FNAME_LEN + 1];
  TCHAR filename[MAX_FNAME_LEN + 1];
  TCHAR newname [MAX_FNAME_LEN + 1];
  CWaitCursor take_a_deep_sigh;

  if(!GetTempPath(MAX_FNAME_LEN,tempdir))
  {
    AfxMessageBox(_T("Cannot find a temporary directory for 'HTML-Export'"),MB_OK);
    return;
  }
  if(!GetTempFileName(tempdir,_T("ODBC_Excel"),0,filename))
  {
    AfxMessageBox(_T("Cannot find a temporary file for 'HTML-Export'"),MB_OK);
  }

  WinFile file(filename);
  if(!file.Open(winfile_write | open_trans_text,FAttributes::attrib_none,Encoding::UTF8))
  {
    AfxMessageBox(_T("Cannot open a temporary file for 'HTML-Export'"),MB_OK);
    return;
  }
  // Print the header wit formats
  CString header(html_header);
  file.Write(header);

  // HEADER
  file.Write(CString(_T("<TR>")));
  for(int x = 1; x < m_pGridCtrl->GetColumnCount(); ++x)
  {
    file.Format(_T("<TH>%s</TH>"),m_pGridCtrl->GetItemText(0,x).GetString());
  }
  file.Write(CString(_T("</TR>\n")));
  // BODY
  // Print out the rows one by one
  for(int y = 1; y < m_pGridCtrl->GetRowCount(); ++y)
  {
    // Print table header
    file.Write(CString(_T("<TR>")));
    // Print table data
    for(int x = 1; x < m_pGridCtrl->GetColumnCount(); ++x)
    {
      CString text = m_pGridCtrl->GetItemText(y,x);
      if(text.IsEmpty()) text = _T("&nbsp;");
      if(m_pGridCtrl->GetItemFormat(y,x) & DT_RIGHT)
      {
        file.Format(_T("<TD ALIGN=\"right\">%s</TD>"),text.GetString());
      }
      else
      {
        file.Format(_T("<TD>%s</TD>"),text.GetString());
      }
    }
    file.Write(CString(_T("</TR>\n")));
  }
  // Print the ending flag
  CString footer(html_footer);
  file.Write(footer);
  // finally close the directory
  file.Close();

  // Now rename to .html
  _tcscpy_s(newname,MAX_FNAME_LEN,filename);
  TCHAR *html = _tcsstr(newname,_T(".tmp")); 
  if(html)
  {
    _tcscpy_s(html,6,_T(".html"));
    _trename(filename,newname);
  }
  // Startup with HTML viewer
  QueryToolApp* app = (QueryToolApp*) AfxGetApp();
  app->RunShellCommand(tempdir,_T("open"),newname,NULL);
}

void
CGridView::ExportToTXT()
{
  TCHAR tempdir [MAX_FNAME_LEN + 1];
  TCHAR filename[MAX_FNAME_LEN + 1];
  TCHAR newname [MAX_FNAME_LEN + 1];
  CMap<int,int,int,int> colSize;
  CWaitCursor take_a_deep_sigh;

  if(!GetTempPath(MAX_FNAME_LEN,tempdir))
  {
    AfxMessageBox(_T("Cannot find a temporary directory for 'TXT-Export'"),MB_OK);
    return;
  }
  if(!GetTempFileName(tempdir,_T("SQL"),0,filename))
  {
    AfxMessageBox(_T("Cannot find a temporary file for 'TXT-Export'"),MB_OK);
  }
  WinFile file(filename);
  if(!file.Open(winfile_write | open_trans_text,FAttributes::attrib_none,Encoding::UTF8))
  {
    AfxMessageBox(_T("Cannot open a temporary file for 'TXT-Export'"),MB_OK);
    return;
  }
  // Calculate widths
  for(int x = 0; x < m_pGridCtrl->GetColumnCount(); ++x)
  {
    // All rows
    int w = 0;
    for(int y = 0; y < m_pGridCtrl->GetRowCount(); ++y)
    {
      CString content = m_pGridCtrl->GetItemText(y,x);
      if(content.GetLength() > w)
      {
        w = content.GetLength();
      }
    }
    colSize.SetAt(x,w);
  }
  // HEADER
  for(int x = 1; x < m_pGridCtrl->GetColumnCount(); ++x)
  {
    if(x > 1)
    {
      file.Write(CString(_T(" ")));
    }
    CString head = m_pGridCtrl->GetItemText(0,x);
    file.Write(head);
    int w = colSize[x];
    int extra = w - head.GetLength();
    while(extra-- > 0)
    {
      file.Write(CString(_T(" ")));
    }
  }
  file.Write(CString(_T("\n\n")));
  // BODY
  // Print out the rows one by one
  for(int y = 1; y < m_pGridCtrl->GetRowCount(); ++y)
  {
    // Print table data
    for(int x = 1; x < m_pGridCtrl->GetColumnCount(); ++x)
    {
      if(x > 1)
      {
        file.Write(CString(_T(" ")));
      }
      CString text = m_pGridCtrl->GetItemText(y,x);
      file.Write(text);

      int w = colSize[x];
      int extra = w - text.GetLength();
      while(extra-- > 0)
      {
        file.Write(CString(_T(" ")));
      }
    }
    file.Write(CString(_T("\n")));
  }
  // final close the file
  file.Close();

  // Now rename to .txt
  _tcscpy_s(newname,MAX_FNAME_LEN,filename);
  TCHAR *html = _tcsstr(newname,_T(".tmp")); 
  if(html)
  {
    _tcscpy_s(html,5,_T(".txt"));
    _trename(filename,newname);
  }
  // Startup with TXT viewer
  QueryToolApp* app = (QueryToolApp*) AfxGetApp();
  app->RunShellCommand(tempdir,_T("open"),newname,NULL);

  // Cleanup
  colSize.RemoveAll();
}

// EDIT DATA IN THE GRID

void
CGridView::OnBeginEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
  MG_NM_GRIDVIEW *cel = (MG_NM_GRIDVIEW*)pNMHDR;
  int col = cel->iColumn;
  int row = cel->iRow;
  if(m_editorView->CanEdit())
  {
    // Place marker.
  }
  else
  {
    AfxMessageBox(_T("Cannot edit this query, not one-table-query or no primary key found"));
  }
  m_saveEdit = GetItemText(row,col);
}

void
CGridView::OnEndInPlaceEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
  MG_NM_GRIDVIEW *cel = (MG_NM_GRIDVIEW*)pNMHDR;
  int col = cel->iColumn;
  int row = cel->iRow;

  CString text = GetItemText(row,col);
  if(!m_editorView->UpdateTable(row,col,text))
  {
    SetItemText(row,col,m_saveEdit);
    m_saveEdit = _T("");
  }
}


