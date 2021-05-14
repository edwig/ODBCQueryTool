// GridCellCombo.cpp : implementation file
//
// MFC Grid Control - Main grid cell class
//
// Provides the implementation for a combobox cell type of the
// grid control.
//
// Written by Chris Maunder <chris@codeproject.com>
// Copyright (c) 1998-2005. All Rights Reserved.
//
// Parts of the code contained in this file are based on the original
// CInPlaceList from http://www.codeguru.com/listview
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name and all copyright 
// notices remains intact. 
//
// An email letting me know how you are using it would be nice as well. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// For use with CGridCtrl v2.22+
//
// History:
// 6 Aug 1998 - Added CComboEdit to subclass the edit control - code 
//              provided by Roelf Werkman <rdw@inn.nl>. Added nID to 
//              the constructor param list.
// 29 Nov 1998 - bug fix in onkeydown (Markus Irtenkauf)
// 13 Mar 2004 - GetCellExtent fixed by Yogurt
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GridCell.h"
#include "GridCtrl.h"
#include "GridCellCombo.h"
#include "..\SkinScrollWnd.h"
#include "..\StyleColors.h"
#include "..\StyleMacros.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CComboEdit

CComboEdit::CComboEdit()
{
}

CComboEdit::~CComboEdit()
{
}

// Stoopid win95 accelerator key problem workaround - Matt Weagle.
BOOL CComboEdit::PreTranslateMessage(MSG* pMsg) 
{
	// Make sure that the keystrokes continue to the appropriate handlers
	if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_KEYUP)
	{
		::TranslateMessage(pMsg);
		::DispatchMessage(pMsg);
		return TRUE;
	}	

	// Catch the Alt key so we don't choke if focus is going to an owner drawn button
  if(pMsg->message == WM_SYSCHAR)
  {
    return TRUE;
  }
	return CEdit::PreTranslateMessage(pMsg);
}

BEGIN_MESSAGE_MAP(CComboEdit, CEdit)
	ON_WM_KILLFOCUS()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComboEdit message handlers

void CComboEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	
  CInPlaceList* pOwner = dynamic_cast<CInPlaceList*>(GetOwner());  // This MUST be a CInPlaceList
  if(pOwner)
  {
    pOwner->EndEdit();	
  }
}

void CComboEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if((nChar == VK_PRIOR || nChar == VK_NEXT ||
		  nChar == VK_DOWN  || nChar == VK_UP   ||
		  nChar == VK_RIGHT || nChar == VK_LEFT) &&
		(GetKeyState(VK_CONTROL) < 0 && GetDlgCtrlID() == IDC_COMBOEDIT))
    {
      CWnd* pOwner = GetOwner();
      if(pOwner)
      {
        pOwner->SendMessage(WM_KEYDOWN,nChar,nRepCnt + (((DWORD)nFlags) << 16));
      }
      return;
    }

	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CComboEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_ESCAPE) 
	{
    CWnd* pOwner = GetOwner();
    if(pOwner)
    {
      pOwner->SendMessage(WM_KEYUP,nChar,nRepCnt + (((DWORD)nFlags) << 16));
    }
    return;
  }

	if (nChar == VK_TAB || nChar == VK_RETURN || nChar == VK_ESCAPE)
  {
    CWnd* pOwner = GetOwner();
    if(pOwner)
    {
      pOwner->SendMessage(WM_KEYUP,nChar,nRepCnt + (((DWORD)nFlags) << 16));
    }
    return;
  }

	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}

/////////////////////////////////////////////////////////////////////////////
// CInPlaceList

CInPlaceList::CInPlaceList(CWnd*          pParent
                          ,CRect&         rect
                          ,DWORD          dwStyle
                          ,UINT           nID
                          ,int            nRow
                          ,int            nColumn
                          ,COLORREF       crFore
                          ,COLORREF       crBack
                          ,CStringArray&  Items
                          ,CString        sInitText
                          ,UINT           nFirstChar)
{
  m_crForeClr = crFore;
  m_crBackClr = crBack;
	m_nNumLines = 10;
	m_sInitText = sInitText;
 	m_nRow		  = nRow;
 	m_nCol      = nColumn;
 	m_nLastChar = 0; 
  m_hWndList  = nullptr;
	m_bExitOnArrows = FALSE; //(nFirstChar != VK_LBUTTON);	// If mouse click brought us here,

	// Create the combobox
 	DWORD dwComboStyle = WS_BORDER|WS_CHILD|WS_VISIBLE|WS_VSCROLL| CBS_AUTOHSCROLL | CBS_NOINTEGRALHEIGHT | dwStyle;
	int nHeight = rect.Height();
	rect.bottom = rect.bottom + m_nNumLines*nHeight + ::GetSystemMetrics(SM_CYHSCROLL);
  if(!Create(dwComboStyle,rect,pParent,nID))
  {
    return;
  }

  // Getting the default combobox implementation
  COMBOBOXINFO info;
  memset(&info,0,sizeof(COMBOBOXINFO));
  info.cbSize = sizeof(COMBOBOXINFO);
  ::GetComboBoxInfo(m_hWnd,&info);

  // Skin the dropdown list
  if(info.hwndList)
  {
    m_hWndList = info.hwndList;
    CWnd* list = CWnd::FromHandle(info.hwndList);
    SkinScrollWnd* skin = SkinWndScroll(list,1);
    skin->SetScrollbarBias(0);
  }

	// Add the strings
  for(int i = 0; i < Items.GetSize(); i++)
  {
    AddString(Items[i]);
  }

	SetFont(pParent->GetFont());
	SetItemHeight(-1, nHeight);

  int nMaxLength = GetCorrectDropWidth();

	SetDroppedWidth(nMaxLength);
	SetHorizontalExtent(0); // no horizontal scrolling

	// Set the initial text to m_sInitText
  if(::IsWindow(m_hWnd) && SelectString(-1,m_sInitText) == CB_ERR)
  {
    // No text selected, so restore what was there before
    SetWindowText(m_sInitText);		
  }
  // ShowDropDown();
  OnDropdown();
  OnDropdown();

  // Subclass the combobox edit control if style includes CBS_DROPDOWN
  if((dwStyle & CBS_DROPDOWNLIST) != CBS_DROPDOWNLIST)
  {
    m_edit.SubclassDlgItem(IDC_COMBOEDIT, this);
    PositionEditWindow();
 	  SetFocus();
    switch (nFirstChar)
    {
        case VK_LBUTTON: 
        case VK_RETURN:   m_edit.SetSel((int)_tcslen(m_sInitText), -1); return;
        case VK_BACK:     m_edit.SetSel((int)_tcslen(m_sInitText), -1); break;
        case VK_DOWN: 
        case VK_UP:   
        case VK_RIGHT:
        case VK_LEFT:  
        case VK_NEXT:  
        case VK_PRIOR: 
        case VK_HOME:  
        case VK_END:      m_edit.SetSel(0,-1); return;
        default:          m_edit.SetSel(0,-1);
    }
    SendMessage(WM_CHAR, nFirstChar);
  }
  else
  {
    SetFocus();
  }
}

CInPlaceList::~CInPlaceList()
{
}

void CInPlaceList::OnDropdown()
{
  // TODO: Add control notification handler code here
  if(!m_hWndList) return;
  CWnd* pFrame = CWnd::FromHandle(::GetParent(::GetParent(m_hWndList)));
  CRect rc;
  GetWindowRect(&rc);
  CRect rc2;
  GetDroppedControlRect(&rc2);
  int height = 0;
  int lineHeight = GetItemHeight(0);
  if(lineHeight * GetCount() < rc2.Height() - 2)
  {
    height = lineHeight * GetCount() + 2;
  }
  else
  {
    height = rc2.Height();
  }
  pFrame->SetWindowPos(&wndTopMost,rc.left,rc.bottom - 1,rc2.Width(),height,0);
  pFrame->ShowWindow(SW_SHOW);
  ::ShowWindow(m_hWndList,SW_SHOW);
}

void CInPlaceList::EndEdit()
{
  CString str;
  if (::IsWindow(m_hWnd))
  {
    GetWindowText(str);
  }
  // Send Notification to parent
  GV_DISPINFO dispinfo;
  // display info header
  dispinfo.hdr.hwndFrom = GetSafeHwnd();
  dispinfo.hdr.idFrom   = GetDlgCtrlID();
  dispinfo.hdr.code     = GVN_ENDLABELEDIT;
  // display info item
  dispinfo.item.mask    = LVIF_TEXT|LVIF_PARAM;
  dispinfo.item.row     = m_nRow;
  dispinfo.item.col     = m_nCol;
  dispinfo.item.strText = str;
  dispinfo.item.lParam  = (LPARAM) m_nLastChar; 
 
  CWnd* pOwner = GetOwner();
  if(IsWindow(pOwner->GetSafeHwnd()))
  {
    pOwner->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&dispinfo);
  }
  // Close this window (PostNcDestroy will delete this)
  if(::IsWindow(m_hWnd))
  {
    PostMessage(WM_CLOSE, 0, 0);
  }
}

int CInPlaceList::GetCorrectDropWidth()
{
  const int nMaxWidth = 200;  // don't let the box be bigger than this

  // Reset the dropped width
  int nNumEntries = GetCount();
  int nWidth = 0;
  CString str;

  CClientDC dc(this);
  int nSave = dc.SaveDC();
  dc.SelectObject(GetFont());

  int nScrollWidth = ::GetSystemMetrics(SM_CXVSCROLL);
  for (int i = 0; i < nNumEntries; i++)
  {
    GetLBText(i, str);
    int nLength = dc.GetTextExtent(str).cx + nScrollWidth;
    nWidth = max(nWidth, nLength);
  }
    
  // Add margin space to the calculations
  nWidth += dc.GetTextExtent(_T("0")).cx;

  dc.RestoreDC(nSave);

  nWidth = min(nWidth, nMaxWidth);

  return nWidth;
}

void
CInPlaceList::PositionEditWindow()
{
  CRect rect;
  GetClientRect(rect);
  int height = rect.Height();
  int width  = rect.Width() - height - WS(4);

  CRect editRect;
  m_edit.GetWindowRect(editRect);
  editRect.right = editRect.left + width;
  m_edit.SetWindowPos(0,editRect.left,editRect.top,editRect.Width(),editRect.Height(),SWP_NOMOVE|SWP_NOZORDER);
}

/*
// Fix by Ray (raybie@Exabyte.COM)
void CInPlaceList::OnSelendOK() 
{
    int iIndex = GetCurSel(); 
    if( iIndex != CB_ERR) 
    { 
        CString strLbText; 
        GetLBText( iIndex, strLbText); 
 
        if (!((GetStyle() & CBS_DROPDOWNLIST) == CBS_DROPDOWNLIST)) 
           m_edit.SetWindowText( strLbText); 
    } 
 
    GetParent()->SetFocus(); 	
}
*/

void CInPlaceList::PostNcDestroy() 
{
	CComboBox::PostNcDestroy();

	delete this;
}

BEGIN_MESSAGE_MAP(CInPlaceList,CComboBox)
  ON_WM_KILLFOCUS()
  ON_WM_KEYDOWN()
  ON_WM_KEYUP()
  ON_WM_PAINT()
  ON_CONTROL_REFLECT(CBN_CLOSEUP, OnCbnCloseup)
  ON_CONTROL_REFLECT(CBN_DROPDOWN,OnCbnDropdown)
	ON_WM_GETDLGCODE()
	ON_WM_CTLCOLOR_REFLECT()
  ///ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInPlaceList message handlers

UINT CInPlaceList::OnGetDlgCode() 
{
  return DLGC_WANTALLKEYS;
}

void CInPlaceList::OnCbnCloseup()
{
  // TODO: Add control notification handler code here
  if(!m_hWndList) return;
  ::ShowWindow(::GetParent(::GetParent(m_hWndList)),SW_HIDE);
}

void CInPlaceList::OnCbnDropdown() 
{
  SetDroppedWidth(GetCorrectDropWidth());
}

void CInPlaceList::OnKillFocus(CWnd* pNewWnd) 
{
	CComboBox::OnKillFocus(pNewWnd);

  if(GetSafeHwnd() == pNewWnd->GetSafeHwnd())
  {
    return;
  }
  // Only end editing on change of focus if we're using the CBS_DROPDOWNLIST style
  if((GetStyle() & CBS_DROPDOWNLIST) == CBS_DROPDOWNLIST)
  {
    EndEdit();
  }
}

// If an arrow key (or associated) is pressed, then exit if
//  a) The Ctrl key was down, or
//  b) m_bExitOnArrows == TRUE
void CInPlaceList::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if((nChar == VK_PRIOR || nChar == VK_NEXT ||
		  nChar == VK_DOWN  || nChar == VK_UP   ||
		  nChar == VK_RIGHT || nChar == VK_LEFT) &&
		 (m_bExitOnArrows   || GetKeyState(VK_CONTROL) < 0))
	{
		m_nLastChar = nChar;
		GetParent()->SetFocus();
		return;
	}

	CComboBox::OnKeyDown(nChar, nRepCnt, nFlags);
}

// Need to keep a lookout for Tabs, Esc and Returns.
void CInPlaceList::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
  if(nChar == VK_ESCAPE)
  {
    SetWindowText(m_sInitText);	// restore previous text
  }
	if(nChar == VK_TAB || nChar == VK_RETURN || nChar == VK_ESCAPE)
	{
		m_nLastChar = nChar;
		GetParent()->SetFocus();	// This will destroy this window
		return;
	}
	CComboBox::OnKeyUp(nChar, nRepCnt, nFlags);
}

HBRUSH CInPlaceList::CtlColor(CDC* pDC,UINT nCtlColor) 
{
  /*
  static CBrush brush(m_crBackClr);
  pDC->SetTextColor(m_crForeClr);
  pDC->SetBkMode(TRANSPARENT);
  return (HBRUSH) brush.GetSafeHandle();
  */
	
	// Return a non-NULL brush if the parent's handler should not be called
	return NULL;
}

void CInPlaceList::OnPaint()
{
  CComboBox::OnPaint();
  DrawComboButton();
}

void CInPlaceList::DrawComboButton()
{
  CRect rect;
  GetClientRect(rect);
  CDC* pDC = GetDC();
  rect.left = rect.right - rect.Height() - WS(1);

  // Find the frame color
  COLORREF color = ThemeColor::_Color1;

  // Create pen
  CPen pen;
  pen.CreatePen(PS_SOLID,1,color);
  HGDIOBJ orgpen = pDC->SelectObject(pen);

  // Paint the button
  int size = rect.Height();
  CRect but(rect);
  DWORD  background = ComboBoxDropped;

  pDC->FillSolidRect(but,background);
  but.CenterPoint();

  POINT points[3];
  points[0].x = but.CenterPoint().x - WS(4);
  points[0].y = but.CenterPoint().y + WS(4);
  points[1].x = but.CenterPoint().x + 1;
  points[1].y = but.CenterPoint().y - 1;
  points[2].x = but.CenterPoint().x + WS(4) + 2;
  points[2].y = but.CenterPoint().y + WS(4);

  CBrush brush(background);
  HGDIOBJ orgbrush = pDC->SelectObject(&brush);
  pDC->Polygon(points,3);

  // Paint the frame
  pDC->MoveTo(rect.left,        rect.top);
  pDC->LineTo(rect.right - 1,   rect.top);
  pDC->LineTo(rect.right - 1,   rect.bottom - 1);
  pDC->LineTo(rect.left,        rect.bottom - 1);
  pDC->LineTo(rect.left,        rect.top);

  // Restore DC
  pDC->SelectObject(orgpen);
  pDC->SelectObject(orgbrush);
  ReleaseDC(pDC);
}

/////////////////////////////////////////////////////////////////////////////
// CGridCellCombo 
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CGridCellCombo, CGridCell)

CGridCellCombo::CGridCellCombo() : CGridCell()
{
  SetStyle(CBS_DROPDOWN);  // CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE, CBS_SORT
}

// Create a control to do the editing
BOOL CGridCellCombo::Edit(int nRow, int nCol, CRect rect, CPoint /* point */, UINT nID, UINT nChar)
{
  m_bEditing = TRUE;

  // CInPlaceList auto-deletes itself
  m_pEditWnd = new CInPlaceList(GetGrid(), rect, GetStyle(), nID, nRow, nCol, 
                                GetTextClr(), GetBackClr(), m_Strings, GetText(), nChar);

  return TRUE;
}

CWnd* CGridCellCombo::GetEditWnd() const
{
  if(m_pEditWnd && (m_pEditWnd->GetStyle() & CBS_DROPDOWNLIST) != CBS_DROPDOWNLIST)
  {
    return &((dynamic_cast<CInPlaceList*>(m_pEditWnd))->m_edit);
  }
	return NULL;
}


CSize CGridCellCombo::GetCellExtent(CDC* pDC)
{    
  CSize sizeScroll (GetSystemMetrics(SM_CXVSCROLL), GetSystemMetrics(SM_CYHSCROLL));    
  CSize sizeCell (CGridCell::GetCellExtent(pDC));    
  sizeCell.cx += sizeScroll.cx;    
  sizeCell.cy = max(sizeCell.cy,sizeScroll.cy);    
  return sizeCell;
}

// Cancel the editing.
void CGridCellCombo::EndEdit()
{
  if(m_pEditWnd)
  {
    (dynamic_cast<CInPlaceList*>(m_pEditWnd))->EndEdit();
  }
}

// Override draw so that when the cell is selected, a drop arrow is shown in the RHS.
BOOL CGridCellCombo::Draw(CDC* pDC, int nRow, int nCol, CRect rect,  BOOL bEraseBkgnd /*=TRUE*/)
{
  // Cell selected?
  if (GetGrid()->IsCellEditable(nRow, nCol) && !IsEditing())
  {
    // Get the size of the scroll box
    CSize sizeScroll(rect.Height(),rect.Height());

    // enough room to draw?
    if(sizeScroll.cy < rect.Width())
    {
      // Draw control at RHS of cell
      CRect ScrollRect  = rect;
      ScrollRect.left   = rect.right - rect.Height();

      // Do the draw 
      // pDC->DrawFrameControl(ScrollRect, DFC_SCROLL, DFCS_SCROLLDOWN | DFCS_FLAT);
      DrawComboButton(pDC,ScrollRect);

      // Adjust the remaining space in the cell
      rect.right = ScrollRect.left;
    }
  }

  CString strTempText = GetText();
  if(IsEditing())
  {
    SetText(_T(""));
  }
  // drop through and complete the cell drawing using the base class' method
  BOOL bResult = CGridCell::Draw(pDC, nRow, nCol, rect,  bEraseBkgnd);

  if(IsEditing())
  {
    SetText(strTempText);
  }
	return bResult;
}

void  CGridCellCombo::DrawComboButton(CDC* pDC,CRect p_rect)
{
  // Find the frame color
  COLORREF color = ThemeColor::_Color1;

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
  points[0].x = but.CenterPoint().x - WS(4);
  points[0].y = but.CenterPoint().y - 1;
  points[1].x = but.CenterPoint().x + 1;
  points[1].y = but.CenterPoint().y + WS(4);
  points[2].x = but.CenterPoint().x + WS(4) + 2;
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
void CGridCellCombo::SetOptions(const CStringArray& ar)
{ 
  m_Strings.RemoveAll();
  for(int i = 0; i < ar.GetSize(); i++)
  {
    m_Strings.Add(ar[i]);
  }
}
