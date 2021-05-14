// HyperLink.cpp : implementation file
//
// HyperLink static control. Will open the default browser with the given URL
// when the user clicks on the link.
//
// Copyright (C) 1997 - 1999 Chris Maunder
// All rights reserved. May not be sold for profit.
//
// Thanks to Pål K. Tønder for auto-size and window caption changes.
//
// "GotoURL" function by Stuart Patterson
// As seen in the August, 1997 Windows Developer's Journal.
// Copyright 1997 by Miller Freeman, Inc. All rights reserved.
// Modified by Chris Maunder to use TCHARs instead of chars.
//
// 2/29/00 -- P. Shaffer standard font modification
// 20/2/21 -- E. Huisman Default hand cursor

#include "stdafx.h"
#include "StyleHyperLink.h"
#include "StyleMessageBox.h"
#include "RegistryManager.h"
#include "atlconv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TOOLTIP_ID 1

/////////////////////////////////////////////////////////////////////////////
// StyleHyperLink

StyleHyperLink::StyleHyperLink()
{
    m_hLinkCursor       = NULL;                 // No cursor as yet
    m_crLinkColour      = RGB(  0,   0, 238);   // Blue
    m_crVisitedColour   = RGB( 85,  26, 139);   // Purple
    m_crHoverColour     = RGB(255,   0,   0);   // Red
    m_bOverControl      = FALSE;                // Cursor not yet over control
    m_bVisited          = FALSE;                // Hasn't been visited yet.
    m_nUnderline        = ulHover;              // Underline the link?
    m_bAdjustToFit      = TRUE;                 // Resize the window to fit the text?
    m_strURL.Empty();
    m_nTimerID          = 100;
}

StyleHyperLink::~StyleHyperLink()
{
  m_UnderlineFont.DeleteObject();
}

BEGIN_MESSAGE_MAP(StyleHyperLink, CStatic)
  ON_WM_SETCURSOR()
  ON_WM_MOUSEMOVE()
  ON_WM_TIMER()
  ON_CONTROL_REFLECT(STN_CLICKED,OnClicked)
  ON_WM_ERASEBKGND()
  ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// StyleHyperLink overrides

BOOL StyleHyperLink::DestroyWindow() 
{
  KillTimer(m_nTimerID);
	
	return CStatic::DestroyWindow();
}

BOOL StyleHyperLink::PreTranslateMessage(MSG* pMsg) 
{
  m_ToolTip.RelayEvent(pMsg);
  return CStatic::PreTranslateMessage(pMsg);
}

void StyleHyperLink::PreSubclassWindow() 
{
  // We want to get mouse clicks via STN_CLICKED
  DWORD dwStyle = GetStyle();
  ::SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle | SS_NOTIFY);
    
  // Set the URL as the window text
  if (m_strURL.IsEmpty())
  {
    GetWindowText(m_strURL);
  }

  // Check that the window text isn't empty. If it is, set it as the URL.
  CString strWndText;
  GetWindowText(strWndText);
  if (strWndText.IsEmpty()) 
  {
    ASSERT(!m_strURL.IsEmpty());    // Window and URL both NULL. DUH!
    SetWindowText(m_strURL);
  }

	CFont* pFont = GetFont();
	if(!pFont)
	{
		HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
    if (hFont == NULL)
    {
      hFont = (HFONT)GetStockObject(ANSI_VAR_FONT);
    }
    if (hFont)
    {
      pFont = CFont::FromHandle(hFont);
    }
	}
	ASSERT(pFont->GetSafeHandle());

  // Create the underline font
  LOGFONT lf;
  pFont->GetLogFont(&lf);
	m_StdFont.CreateFontIndirect(&lf);
  lf.lfUnderline = (BYTE) TRUE;
  m_UnderlineFont.CreateFontIndirect(&lf);

  PositionWindow();        // Adjust size of window to fit URL if necessary
  SetDefaultCursor();      // Try and load up a "hand" cursor
  SetUnderline();

  // Create the tooltip
  CRect rect; 
  GetClientRect(rect);
  m_ToolTip.Create(this);
  m_ToolTip.AddTool(this, m_strURL, rect, TOOLTIP_ID);

  CStatic::PreSubclassWindow();
}

/////////////////////////////////////////////////////////////////////////////
// StyleHyperLink message handlers

void StyleHyperLink::OnClicked()
{
  m_bOverControl = FALSE;
	int result = (INT64)GotoURL(m_strURL, SW_SHOW) & 0xFFFF;
	m_bVisited = (result > HINSTANCE_ERROR);
  if (!m_bVisited)
  {
    // Unable to follow link
    MessageBeep(MB_ICONEXCLAMATION);
    ReportError(result);
  }
  else
  {
    // Repaint to show visited color
    SetVisited();
  }
}

HBRUSH 
StyleHyperLink::CtlColor(CDC* pDC,UINT nCtlColor) 
{
  ASSERT(nCtlColor == CTLCOLOR_STATIC);

  if (m_bOverControl)
  {
    pDC->SetTextColor(m_crHoverColour);
  }
  else if (m_bVisited)
  {
    pDC->SetTextColor(m_crVisitedColour);
  }
  else
  {
    pDC->SetTextColor(m_crLinkColour);
  }
  // transparent text.
  pDC->SetBkMode(TRANSPARENT);
  return (HBRUSH)GetStockObject(NULL_BRUSH);
}

void StyleHyperLink::OnMouseMove(UINT nFlags, CPoint point) 
{
  if (!m_bOverControl)        // Cursor has just moved over control
  {
    m_bOverControl = TRUE;
    if (m_nUnderline == ulHover)
    {
      SetFont(&m_UnderlineFont);
    }
    Invalidate();
    SetTimer(m_nTimerID, 100, NULL);
  }
  CStatic::OnMouseMove(nFlags, point);
}

void StyleHyperLink::OnTimer(UINT_PTR nIDEvent) 
{
  CPoint p(GetMessagePos());
  ScreenToClient(&p);

  CRect rect;
  GetClientRect(rect);
  if (!rect.PtInRect(p))
  {
    m_bOverControl = FALSE;
    KillTimer(m_nTimerID);

    if (m_nUnderline != ulAlways)
    {
      SetFont(&m_StdFont);
    }
    rect.bottom+=10;
    InvalidateRect(rect);
  }
   
	CStatic::OnTimer(nIDEvent);
}

BOOL StyleHyperLink::OnSetCursor(CWnd* /*pWnd*/, UINT /*nHitTest*/, UINT /*message*/) 
{
  if (m_hLinkCursor)
  {
    ::SetCursor(m_hLinkCursor);
    return TRUE;
  }
  return FALSE;
}

BOOL StyleHyperLink::OnEraseBkgnd(CDC* pDC) 
{
  CRect rect;
  GetClientRect(rect);
  pDC->FillSolidRect(rect,::GetSysColor(COLOR_3DFACE));

  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// StyleHyperLink operations

void StyleHyperLink::SetURL(CString strURL)
{
  m_strURL = strURL;

  if (::IsWindow(GetSafeHwnd())) 
  {
    PositionWindow();
    m_ToolTip.UpdateTipText(strURL, this, TOOLTIP_ID);
  }
}

CString StyleHyperLink::GetURL() const
{ 
  return m_strURL;   
}

void StyleHyperLink::SetColours(COLORREF crLinkColour, COLORREF crVisitedColour,
                            COLORREF crHoverColour /* = -1 */) 
{ 
  m_crLinkColour    = crLinkColour; 
  m_crVisitedColour = crVisitedColour;

  if (crHoverColour == -1)
  {
    m_crHoverColour = ::GetSysColor(COLOR_HIGHLIGHT);
  }
  else
  {
    m_crHoverColour = crHoverColour;
  }
  if (::IsWindow(m_hWnd))
  {
    Invalidate();
  }
}

COLORREF StyleHyperLink::GetLinkColour() const
{ 
  return m_crLinkColour; 
}

COLORREF StyleHyperLink::GetVisitedColour() const
{
  return m_crVisitedColour; 
}

COLORREF StyleHyperLink::GetHoverColour() const
{
  return m_crHoverColour;
}

void StyleHyperLink::SetVisited(BOOL bVisited /* = TRUE */) 
{ 
  m_bVisited = bVisited; 

  if(::IsWindow(GetSafeHwnd()))
  {
    Invalidate();
  }
}

BOOL StyleHyperLink::GetVisited() const
{ 
  return m_bVisited; 
}

void StyleHyperLink::SetLinkCursor(HCURSOR hCursor)
{ 
  m_hLinkCursor = hCursor;
  if (m_hLinkCursor == NULL)
  {
    SetDefaultCursor();
  }
}

HCURSOR StyleHyperLink::GetLinkCursor() const
{
  return m_hLinkCursor;
}

void StyleHyperLink::SetUnderline(int nUnderline /*=ulHover*/)
{
  if (m_nUnderline == nUnderline)
  {
    return;
  }
  if (::IsWindow(GetSafeHwnd()))
  {
    if (nUnderline == ulAlways)
    {
      SetFont(&m_UnderlineFont);
    }
    else
    {
      SetFont(&m_StdFont);
    }
    Invalidate(); 
  }

  m_nUnderline = nUnderline;
}

int StyleHyperLink::GetUnderline() const
{ 
  return m_nUnderline; 
}

void StyleHyperLink::SetAutoSize(BOOL bAutoSize /* = TRUE */)
{
  m_bAdjustToFit = bAutoSize;

  if (::IsWindow(GetSafeHwnd()))
  {
    PositionWindow();
  }
}

BOOL StyleHyperLink::GetAutoSize() const
{ 
  return m_bAdjustToFit; 
}

// Move and resize the window so that the window is the same size
// as the hyperlink text. This stops the hyperlink cursor being active
// when it is not directly over the text. If the text is left justified
// then the window is merely shrunk, but if it is centered or right
// justified then the window will have to be moved as well.
//
// Suggested by Pål K. Tønder 

void StyleHyperLink::PositionWindow()
{
  if(!::IsWindow(GetSafeHwnd()) || !m_bAdjustToFit)
  {
    return;
  }

  // Get the current window position
  CRect WndRect, ClientRect;
  GetWindowRect(WndRect);
  GetClientRect(ClientRect);

  ClientToScreen(ClientRect);

  CWnd* pParent = GetParent();
  if (pParent)
  {
    pParent->ScreenToClient(WndRect);
    pParent->ScreenToClient(ClientRect);
  }

  // Get the size of the window text
  CString strWndText;
  GetWindowText(strWndText);

  CDC* pDC = GetDC();
  CFont* pOldFont = pDC->SelectObject(&m_UnderlineFont);
  CSize Extent = pDC->GetTextExtent(strWndText);
  pDC->SelectObject(pOldFont);
  ReleaseDC(pDC);

  // Adjust for window borders
  Extent.cx += WndRect.Width() - ClientRect.Width(); 
  Extent.cy += WndRect.Height() - ClientRect.Height(); 

  // Get the text justification via the window style
  DWORD dwStyle = GetStyle();

  // Recalc the window size and position based on the text justification
  if (dwStyle & SS_CENTERIMAGE)
  {
    WndRect.DeflateRect(0, (WndRect.Height() - Extent.cy) / 2);
  }
  else
  {
    WndRect.bottom = WndRect.top + Extent.cy;
  }
  if (dwStyle & SS_CENTER)
  {
    WndRect.DeflateRect((WndRect.Width() - Extent.cx) / 2, 0);
  }
  else if (dwStyle & SS_RIGHT)
  {
    WndRect.left = WndRect.right - Extent.cx;
  }
  else // SS_LEFT = 0, so we can't test for it explicitly 
  {
    WndRect.right = WndRect.left + Extent.cx;
  }
  // Move the window
  SetWindowPos(NULL, WndRect.left, WndRect.top, WndRect.Width(), WndRect.Height(), SWP_NOZORDER);
}

/////////////////////////////////////////////////////////////////////////////
// StyleHyperLink implementation

// The following appeared in Paul DiLascia's Jan 1998 MSJ articles.
// It loads a "hand" cursor from the winhlp32.exe module
void StyleHyperLink::SetDefaultCursor()
{
  // No cursor handle - load our own
  if(m_hLinkCursor == nullptr)
  {
    m_hLinkCursor = CopyCursor(LoadCursor(nullptr,IDC_HAND));
  }
}

LONG StyleHyperLink::GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata)
{
  HKEY hkey;
  LONG retval = RegOpenKeyEx(key, subkey, 0, KEY_QUERY_VALUE, &hkey);

  if (retval == ERROR_SUCCESS) 
  {
    long datasize = MAX_PATH;
    TCHAR data[MAX_PATH];
    RegQueryValue(hkey, NULL, data, &datasize);
    lstrcpy(retdata,data);
    RegCloseKey(hkey);
  }

  return retval;
}

void StyleHyperLink::ReportError(int nError)
{
  CString str;
  switch (nError) 
  {
      case 0:                       str = "The operating system is out\nof memory or resources."; break;
      case SE_ERR_PNF:              str = "The specified path was not found."; break;
      case SE_ERR_FNF:              str = "The specified file was not found."; break;
      case ERROR_BAD_FORMAT:        str = "The .EXE file is invalid\n(non-Win32 .EXE or error in .EXE image)."; break;
      case SE_ERR_ACCESSDENIED:     str = "The operating system denied\naccess to the specified file."; break;
      case SE_ERR_ASSOCINCOMPLETE:  str = "The filename association is\nincomplete or invalid."; break;
      case SE_ERR_DDEBUSY:          str = "The DDE transaction could not\nbe completed because other DDE transactions\nwere being processed."; break;
      case SE_ERR_DDEFAIL:          str = "The DDE transaction failed."; break;
      case SE_ERR_DDETIMEOUT:       str = "The DDE transaction could not\nbe completed because the request timed out."; break;
      case SE_ERR_DLLNOTFOUND:      str = "The specified dynamic-link library was not found."; break;
      case SE_ERR_NOASSOC:          str = "There is no application associated\nwith the given filename extension."; break;
      case SE_ERR_OOM:              str = "There was not enough memory to complete the operation."; break;
      case SE_ERR_SHARE:            str = "A sharing violation occurred. ";
      default:                      str.Format(_T("Unknown Error (%d) occurred."), nError); break;
  }
  str = "Unable to open hyperlink:\n\n" + str;
  StyleMessageBox(this,str,"ERROR",MB_ICONEXCLAMATION|MB_OK);
}

HINSTANCE StyleHyperLink::GotoURL(LPCTSTR url, int showcmd)
{
  TCHAR key[MAX_PATH + MAX_PATH];

  // First try ShellExecute()
  HINSTANCE result = ShellExecute(NULL, _T("open"), url, NULL,NULL, showcmd);

  // If it failed, get the .htm regkey and lookup the program
  if ((INT64)result <= HINSTANCE_ERROR) 
  {
    if(GetRegKey(HKEY_CLASSES_ROOT, _T(".htm"), key) == ERROR_SUCCESS) 
    {
      strcat_s(key,(2*MAX_PATH),"\\shell\\open\\command");
      if(GetRegKey(HKEY_CLASSES_ROOT,key,key) == ERROR_SUCCESS) 
      {
        TCHAR *pos;
        pos = strstr(key,"\"%1\"");
        if (pos == NULL) 
        {
          // No quotes found
          pos = strstr(key,"%1");      // Check for %1, without quotes 
          if (pos == NULL)             // No parameter at all...
          {
            pos = key + lstrlen(key) - 1;
          }
          else
          {
            // Remove the parameter
            *pos = '\0';
          }
        }
        else
        {
          // Remove the parameter
          *pos = '\0';
        }
        strcat_s(pos,(2*MAX_PATH)," ");
        strcat_s(pos,(2*MAX_PATH),url);

        USES_CONVERSION;
        result = (HINSTANCE)(INT64)WinExec(T2A(key),showcmd);
      }
    }
  }
  return result;
}
