///////////////////////////////////////////////////////////////////////////
//
// GridCellDateTime.cpp: implementation of the CGridCellDateTime class.
//
// Provides the implementation for a datetime picker cell type of the
// grid control.
//
// Written by Podsypalnikov Eugen 15 Mar 2001
// Modified:
//    31 May 2001  Fixed m_cTime bug (Chris Maunder)
//
// For use with CGridCtrl v2.22+
//
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GridCtrl.h"
#include "GridCell.h"
#include "GridCellDateTime.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CGridCellDateTime

IMPLEMENT_DYNCREATE(CGridCellDateTime, CGridCell)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGridCellDateTime::CGridCellDateTime() : CGridCell()
{
	m_dwStyle = 0;
	m_cTime   = CTime::GetCurrentTime();
}

CGridCellDateTime::CGridCellDateTime(DWORD dwStyle) : CGridCell()
{
	Init(dwStyle);
}

CGridCellDateTime::~CGridCellDateTime()
{
}

CSize CGridCellDateTime::GetCellExtent(CDC* pDC)
{    
  CSize sizeScroll (GetSystemMetrics(SM_CXVSCROLL), GetSystemMetrics(SM_CYHSCROLL));	
  CSize sizeCell (CGridCell::GetCellExtent(pDC));	
  sizeCell.cx += sizeScroll.cx;	
  sizeCell.cy = max(sizeCell.cy,sizeScroll.cy);	
  return sizeCell;
}

BOOL CGridCellDateTime::Edit(int nRow, int nCol, CRect rect, CPoint /* point */, 
							 UINT nID, UINT nChar)
{
	m_bEditing = TRUE;

	// CInPlaceDateTime auto-deletes itself
	m_pEditWnd = new CInPlaceDateTime(GetGrid(), rect,
										m_dwStyle|DTS_UPDOWN, nID, nRow, nCol, 
										GetTextClr(), GetBackClr(), GetTime(), nChar);
	return TRUE;
}

CWnd* CGridCellDateTime::GetEditWnd() const
{
	return m_pEditWnd;
}

void CGridCellDateTime::EndEdit()
{
	if (m_pEditWnd) (dynamic_cast<CInPlaceDateTime*>(m_pEditWnd))->EndEdit();
}

void CGridCellDateTime::Init(DWORD dwStyle)
{
	m_dwStyle = dwStyle;

	SetTime(CTime::GetCurrentTime());

	SetFormat(DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX
#ifndef _WIN32_WCE
		|DT_END_ELLIPSIS
#endif
		);
}

// Should be changed to use locale settings
void CGridCellDateTime::SetTime(CTime time)
{
	m_cTime = time;

	if (DTS_TIMEFORMAT == m_dwStyle) 
	{
//	SetText(m_cTime.Format(_T("%H:%M:%S")));
    SetText(m_cTime.Format(_T("%X")));  // Time for the current locale
	}
	else if (DTS_SHORTDATEFORMAT == m_dwStyle) 
	{
//	SetText(m_cTime.Format(("%d/%m/%Y")));
		SetText(m_cTime.Format(("%x")));   // Date for the current locale
	}
}

/////////////////////////////////////////////////////////////////////////////
// CInPlaceDateTime

CInPlaceDateTime::CInPlaceDateTime(CWnd* pParent, CRect& rect, DWORD dwStyle, UINT nID,
								   int nRow, int nColumn, 
								   COLORREF crFore, COLORREF crBack,
								   CTime* pcTime,
								   UINT nFirstChar)
{
	m_crForeClr     = crFore;
	m_crBackClr     = crBack;
	m_nRow          = nRow;
	m_nCol          = nColumn;
	m_nLastChar     = 0; 
	m_bExitOnArrows = FALSE;
	m_pcTime        = pcTime;

	DWORD dwStl = WS_BORDER|WS_VISIBLE|WS_CHILD|dwStyle;

	if (!Create(dwStl, rect, pParent, nID)) 
	{
		return;
	}

	SetTime(m_pcTime);

	SetFont(pParent->GetFont());
	SetFocus();

	switch (nFirstChar) 
	{
		case VK_LBUTTON: 
		case VK_RETURN: return;
		case VK_BACK:   break;
		case VK_DOWN: 
		case VK_UP:   
		case VK_RIGHT:
		case VK_LEFT:  
		case VK_NEXT:  
		case VK_PRIOR: 
		case VK_HOME:  
		case VK_END:    return;
		default:        break;
	}
	SendMessage(WM_CHAR, nFirstChar);
}

CInPlaceDateTime::~CInPlaceDateTime()
{
}

void CInPlaceDateTime::EndEdit()
{
	CString str;
	if (::IsWindow(m_hWnd)) 
	{
		GetWindowText(str);
		GetTime(*m_pcTime);
	}

	// Send Notification to parent
	GV_DISPINFO dispinfo;

	dispinfo.hdr.hwndFrom = GetSafeHwnd();
	dispinfo.hdr.idFrom   = GetDlgCtrlID();
	dispinfo.hdr.code     = GVN_ENDLABELEDIT;

	dispinfo.item.mask    = LVIF_TEXT|LVIF_PARAM;
	dispinfo.item.row     = m_nRow;
	dispinfo.item.col     = m_nCol;
	dispinfo.item.strText = str;
	dispinfo.item.lParam  = (LPARAM) m_nLastChar; 

	CWnd* pOwner = GetOwner();
	if (IsWindow(pOwner->GetSafeHwnd())) 
	{
		pOwner->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&dispinfo);
	}

	// Close this window (PostNcDestroy will delete this)
	if (::IsWindow(m_hWnd)) 
	{
		PostMessage(WM_CLOSE, 0, 0);
	}
}

void CInPlaceDateTime::PostNcDestroy() 
{
	CDateTimeCtrl::PostNcDestroy();
	delete this;
}

BEGIN_MESSAGE_MAP(CInPlaceDateTime, CDateTimeCtrl)
	ON_WM_KILLFOCUS()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_GETDLGCODE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInPlaceDateTime message handlers

void CInPlaceDateTime::OnKillFocus(CWnd* pNewWnd) 
{
	CDateTimeCtrl::OnKillFocus(pNewWnd);

	if(GetSafeHwnd() == pNewWnd->GetSafeHwnd()) 
	{
		return;
	}
	EndEdit();
}

UINT CInPlaceDateTime::OnGetDlgCode() 
{
	return DLGC_WANTALLKEYS;
}

void CInPlaceDateTime::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if((nChar == VK_PRIOR || nChar == VK_NEXT ||
			nChar == VK_DOWN  || nChar == VK_UP   ||
			nChar == VK_RIGHT || nChar == VK_LEFT) &&
			(m_bExitOnArrows  || GetKeyState(VK_CONTROL) < 0))
	{
		m_nLastChar = nChar;
		GetParent()->SetFocus();
		return;
	}
	CDateTimeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CInPlaceDateTime::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar == VK_TAB || nChar == VK_RETURN || nChar == VK_ESCAPE)
	{
		m_nLastChar = nChar;
		GetParent()->SetFocus();    // This will destroy this window
		return;
	}
	CDateTimeCtrl::OnKeyUp(nChar, nRepCnt, nFlags);
}
