// GridCellCombo.cpp : implementation file
//
// MFC Grid Control - Main grid cell class
//
// Provides the implementation for a combobox cell type of the grid control.
// For use with CGridCtrl v2.22+
//
// History:
//  6 Aug 1998 - Added CComboEdit to subclass the edit control
// 24 Nov 2023 - Complete rewrite with use of StyleComboBox by W.E. Huisman
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GridCell.h"
#include "GridCtrl.h"
#include "GridCellCombo.h"
#include "..\SkinScrollWnd.h"
#include "..\StyleColors.h"
#include "..\StyleMacros.h"
#include "..\StyleComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace ThemeColor;

#define IDC_ENDEDIT (WM_USER + 1)

/////////////////////////////////////////////////////////////////////////////
// CGridCellCombo 
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CGridCellCombo, CGridCell)

CGridCellCombo::CGridCellCombo() : CGridCell()
{
  // Minimal combo style in case the programmer does not chooses one.
  SetStyle(CBS_DROPDOWN);  
}

// Only these styles are allowed
// CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE, CBS_SORT
void  
CGridCellCombo::SetStyle(DWORD dwStyle)
{
  if(dwStyle == 0)
  {
    m_dwStyle = CBS_DROPDOWN;
    return;
  }
  // Check dropdown style
  if((dwStyle & CBS_DROPDOWNLIST) == CBS_DROPDOWNLIST)
  {
    m_dwStyle |= CBS_DROPDOWNLIST;
  }
  else if((dwStyle & CBS_DROPDOWN) == CBS_DROPDOWN)
  {
    m_dwStyle |=  CBS_DROPDOWN;
  }
  else if((dwStyle & CBS_SIMPLE) == CBS_SIMPLE)
  {
    m_dwStyle |=  CBS_SIMPLE;
  }
  // Check sort style
  if((dwStyle & CBS_SORT) == CBS_SORT)
  {
    m_dwStyle |= CBS_SORT;
  }
}

// Create a control to do the editing
BOOL
CGridCellCombo::Edit(int nRow, int nCol, CRect rect, CPoint /* point */, UINT nID, UINT nChar)
{
  m_bEditing = TRUE;
  // Remember our cell
  m_row = nRow;
  m_col = nCol;

  if(m_pEditWnd == nullptr)
  {
    if(CreateNewComboBox(nRow,nCol,rect,nID,nChar))
    {
      return TRUE;
    }
  }
  // Now send the last edit character
  // But only begin at alfanumerics and do not send cursor keys
  if(nChar >= '0')
  {
    m_pEditWnd->PostMessage(WM_KEYDOWN,nChar,0);
    m_pEditWnd->PostMessage(WM_CHAR,   nChar,0);
    m_pEditWnd->PostMessage(WM_KEYUP,  nChar,0);
  }
  return TRUE;
}

bool
CGridCellCombo::CreateNewComboBox(int p_row,int p_col,CRect p_rect,UINT p_id,UINT p_char)
{
  // Use our StyleComboBox
  m_pEditWnd = new StyleComboBox();
  StyleComboBox* combo = dynamic_cast<StyleComboBox*>(m_pEditWnd);
  combo->SetGridCell(this);

  // Make sure we can see it and create it
  m_dwStyle |= WS_CHILD | WS_VISIBLE;
  combo->Create(m_dwStyle,p_rect,GetGrid(),p_id);
  combo->InitSkin();

  // Add all strings that we intend to use
  for(int ind = 0;ind < m_Strings.GetCount();++ind)
  {
    combo->AddString(m_Strings.GetAt(ind));
  }

  // Tell the grid that we are in business for this cell
  GetGrid()->RegisterEditCell(p_row,p_col);

  // Propagate the current contents
  CString initText = GetText();
  combo->SetWindowText(initText);

  // First action of the list
  if((m_dwStyle & CBS_DROPDOWNLIST) == CBS_DROPDOWNLIST)
  {
    combo->OnDropdown();
  }
  else
  {
    // CBS_SIMPLE or CBS_DROPDOWN can edit the current contents
    combo->SubclassDlgItem(IDC_COMBOEDIT,GetGrid());
    SCBTextEdit* edit = combo->GetEditControl();
    switch (p_char)
    {
        case VK_LBUTTON: 
        case VK_RETURN:   edit->SetSel((int)_tcslen(initText), -1); 
                          return true;
        case VK_BACK:     edit->SetSel((int)_tcslen(initText), -1);
                          break;
        case VK_DOWN: 
        case VK_UP:   
        case VK_RIGHT:
        case VK_LEFT:  
        case VK_NEXT:  
        case VK_PRIOR: 
        case VK_HOME:  
        case VK_END:      edit->SetSel(0,-1); 
                          return true;
        default:          edit->SetSel(0,-1);
                          break;
    }
  }
  // Not yet ready
  return false;
}

CWnd*
CGridCellCombo::GetEditWnd() const
{
  if(m_pEditWnd && (m_pEditWnd->GetStyle() & CBS_DROPDOWNLIST) != CBS_DROPDOWNLIST)
  {
    StyleComboBox* box = dynamic_cast<StyleComboBox*>(m_pEditWnd);
    if(box)
    {
      return reinterpret_cast<CWnd*>(box->GetEditControl());
    }
  }
	return NULL;
}

CSize
CGridCellCombo::GetCellExtent(CDC* pDC,HWND p_hwnd)
{    
  int dpi = ::GetDpiForWindow(p_hwnd);
  CSize sizeScroll(GetSystemMetricsForDpi(SM_CXVSCROLL,dpi)
                  ,GetSystemMetricsForDpi(SM_CYHSCROLL,dpi));    
  CSize sizeCell(CGridCell::GetCellExtent(pDC,p_hwnd));
  sizeCell.cx += sizeScroll.cx;    
  sizeCell.cy = max(sizeCell.cy,sizeScroll.cy);    
  return sizeCell;
}

// Cancel the editing.
void
CGridCellCombo::EndEdit()
{
  StyleComboBox* combo = dynamic_cast<StyleComboBox*>(m_pEditWnd);
  if(!combo)
  {
    return;
  }
  CString str;
  if(::IsWindow(combo->GetEditControl()->GetSafeHwnd()))
  {
    combo->GetWindowText(str);
    if(!str.IsEmpty())
    {
      if((combo->GetStyle() & CBS_DROPDOWNLIST) == CBS_DROPDOWNLIST)
      {
        // Check if we have a valid text
        if(combo->FindStringExact(-1,str) == CB_ERR)
        {
          ASSERT(FALSE);
        }
      }
    }
  }

  // Send Notification to parent
  GV_DISPINFO dispinfo;
  // display info header
  dispinfo.hdr.hwndFrom = combo->GetSafeHwnd();
  dispinfo.hdr.idFrom   = combo->GetDlgCtrlID();
  dispinfo.hdr.code     = GVN_ENDLABELEDIT;
  // display info item
  dispinfo.item.mask    = LVIF_TEXT|LVIF_PARAM;
  dispinfo.item.row     = m_row;
  dispinfo.item.col     = m_col;
  dispinfo.item.strText = str;
  dispinfo.item.lParam  = (LPARAM) m_nLastChar;
 
  CWnd* pOwner = GetGrid();
  if(IsWindow(pOwner->GetSafeHwnd()))
  {
    pOwner->SendMessage(WM_NOTIFY, combo->GetDlgCtrlID(),(LPARAM)&dispinfo);
  }

  // Dispose of the detailed edit control
  if(combo)
  {
    combo->DestroyWindow();
    delete combo;
    m_pEditWnd = nullptr;
  }
}

// Override draw so that when the cell is selected, a drop arrow is shown in the RHS.
BOOL
CGridCellCombo::Draw(HWND p_hwnd,CDC* pDC, int nRow, int nCol, CRect rect,  BOOL bEraseBkgnd /*=TRUE*/)
{
  // Cell selected?
  if(GetGrid()->IsCellEditable(nRow,nCol))
  {
    // Get the size of the scroll box
    CSize sizeScroll(rect.Height(),rect.Height());

    // enough room to draw?
    if(sizeScroll.cy < rect.Width())
    {
      // Draw control at RHS of cell
      CRect ScrollRect = rect;
      ScrollRect.left = rect.right - rect.Height();
      if(m_pEditWnd)
      {
        reinterpret_cast<StyleComboBox*>(m_pEditWnd)->DrawComboButton(pDC,ScrollRect);
      }
      else
      {
        DrawComboButton(p_hwnd,pDC,ScrollRect);
      }

      // Adjust the remaining space in the cell
      rect.right = ScrollRect.left;
    }
  }
  // Do the draw if we are a combo box control
  if(m_pEditWnd && m_pEditWnd->GetSafeHwnd())
  {
    reinterpret_cast<StyleComboBox*>(m_pEditWnd)->OnPaint();
    return TRUE;
  }

  CString strTempText = GetText();
  if(IsEditing())
  {
    SetText(_T(""));
  }
  // drop through and complete the cell drawing using the base class' method
  BOOL bResult = CGridCell::Draw(p_hwnd,pDC,nRow,nCol,rect,bEraseBkgnd);

  if(IsEditing())
  {
    SetText(strTempText);
  }
	return bResult;
}

void
CGridCellCombo::DrawComboButton(HWND p_hwnd,CDC* pDC,CRect p_rect)
{
  // Find the frame color
  COLORREF color = ThemeColor::GetColor(Colors::AccentColor1);

  // Create pen
  CPen pen;
  pen.CreatePen(PS_SOLID,1,color);
  HGDIOBJ orgpen = pDC->SelectObject(pen);

  // Paint the button
  int size = p_rect.Height();
  CRect but(p_rect);
  DWORD background = ClrEditBkgnd;

  pDC->FillSolidRect(but,background);
  but.CenterPoint();

  POINT points[3];
  int spacing = WS(p_hwnd,4);
  int extra   = WS(p_hwnd,2);
  points[0].x = but.CenterPoint().x - spacing;
  points[0].y = but.CenterPoint().y - 1;
  points[1].x = but.CenterPoint().x + 1;
  points[1].y = but.CenterPoint().y + spacing;
  points[2].x = but.CenterPoint().x + spacing + extra;
  points[2].y = but.CenterPoint().y - 1;

  CBrush brush(background);
  HGDIOBJ orgbrush = pDC->SelectObject(&brush);
  pDC->Polygon(points,3);

  // Paint the frame
  pDC->MoveTo(p_rect.left,        p_rect.top);
  pDC->LineTo(p_rect.right - 1,   p_rect.top);
  pDC->LineTo(p_rect.right - 1,   p_rect.bottom - 1);
  pDC->LineTo(p_rect.left,        p_rect.bottom - 1);
  pDC->LineTo(p_rect.left,        p_rect.top);

  // Restore DC
  pDC->SelectObject(orgpen);
  pDC->SelectObject(orgbrush);
}

// For setting the strings that will be displayed in the drop list
void
CGridCellCombo::SetOptions(const CStringArray& ar)
{ 
  m_Strings.RemoveAll();
  for(int i = 0; i < ar.GetSize(); i++)
  {
    m_Strings.Add(ar[i]);
  }
}
