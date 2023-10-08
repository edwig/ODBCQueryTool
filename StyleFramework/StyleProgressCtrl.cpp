////////////////////////////////////////////////////////////////////////
//
// File: StyleProgressCtrl.cpp
// Function: Styled progress control
//
//   _____ _         _ _             ______                                           _    
//  / ____| |       | (_)           |  ____|                                         | |   
// | (___ | |_ _   _| |_ _ __   __ _| |__ _ __ __ _ _ __ ___   _____      _____  _ __| | __
//  \___ \| __| | | | | | '_ \ / _` |  __| '__/ _` | '_ ` _ \ / _ \ \ /\ / / _ \| '__| |/ /
//  ____) | |_| |_| | | | | | | (_| | |  | | | (_| | | | | | |  __/\ V  V / (_) | |  |   < 
// |_____/ \__|\__, |_|_|_| |_|\__, |_|  |_|  \__,_|_| |_| |_|\___| \_/\_/ \___/|_|  |_|\_\
//              __/ |           __/ |                                                      
//             |___/           |___/                                                       
//
//
// Author: ir. W.E. Huisman
// For license: See the file "LICENSE.txt" in the root folder
//
#include "stdafx.h"
#include "StyleProgressCtrl.h"
#include "StyleFonts.h"

using namespace ThemeColor;

IMPLEMENT_DYNAMIC(StyleProgressCtrl,CWnd);

BEGIN_MESSAGE_MAP(StyleProgressCtrl,CWnd)
  ON_WM_TIMER()
  ON_WM_PAINT()
  ON_WM_NCPAINT()
END_MESSAGE_MAP()

StyleProgressCtrl::StyleProgressCtrl()
{
}

StyleProgressCtrl::~StyleProgressCtrl()
{
  DestroyWindow();
}

void
StyleProgressCtrl::PreSubclassWindow()
{
  ScaleControl(this);
}

// Generic creator
BOOL 
StyleProgressCtrl::Create(_In_ DWORD dwStyle, _In_ const RECT& rect, _In_ CWnd* pParentWnd, _In_ UINT nID)
{
  CWnd* pWnd = this;
  return pWnd->Create(_T(""),nullptr,dwStyle,rect,pParentWnd,nID);
}

// Extended creator
BOOL 
StyleProgressCtrl::CreateEx(_In_ DWORD dwExStyle,_In_ DWORD dwStyle,_In_ const RECT& rect,_In_ CWnd* pParentWnd,_In_ UINT nID)
{
  BOOL bRet = Create(dwStyle, rect, pParentWnd, nID);
  if (bRet && dwExStyle != 0)
  {
    bRet = ModifyStyleEx(0, dwExStyle);
  }
  return bRet;
}

// Sets range of values for the control. (16-bit limit)
void 
StyleProgressCtrl::SetRange(_In_ short nLower, _In_ short nUpper)
{
  m_lower = (int)nLower;
  m_upper = (int)nUpper;
  OnDrawProgress();
}

// Sets range of values for the control. (32-bit limit)
void 
StyleProgressCtrl::SetRange32(_In_ int nLower, _In_ int nUpper)
{
  m_lower = nLower;
  m_upper = nUpper;
  OnDrawProgress();
}

// Retrieves range of values for the control. (32-bit limit)
void 
StyleProgressCtrl::GetRange(_Out_ int& nLower, _Out_ int& nUpper) const
{
  nLower = m_lower;
  nUpper = m_upper;
}

// Sets the step by which increments happen with a call to StepIt().
int 
StyleProgressCtrl::SetStep(_In_ int nStep)
{
  // Step can be negative, but cannot be zero.
  if (nStep != 0)
  {
    m_step = nStep;
  }
  return m_step;
}

// Retrieves the step increment for the progress bar control.
int 
StyleProgressCtrl::GetStep() const
{
  return m_step;
}

// Gets the current position within the set range of the control.
int 
StyleProgressCtrl::GetPos() const
{
  return m_position;
}

// Sets the current position within the set range of the control.
int StyleProgressCtrl::SetPos(_In_ int nPos)
{
  if (m_lower <= nPos && nPos <= m_upper)
  {
    m_position = nPos;
  }
  OnDrawProgress();
  return m_position;
}

// Displaces the current position within the set range of the
// control by the passed value.
int 
StyleProgressCtrl::OffsetPos(_In_ int nPos)
{
  m_position += nPos;
  BoundsCheck();
  OnDrawProgress();

  return m_position;
}

// Sets the control's background color.
COLORREF 
StyleProgressCtrl::SetBkColor(_In_ COLORREF clrNew)
{
  COLORREF old = m_background;
  m_background = clrNew;
  OnDrawProgress();
  return old;
}

// Sets the color of the progress indicator bar in the progress bar control.
COLORREF 
StyleProgressCtrl::SetBarColor(_In_ COLORREF clrBar)
{
  COLORREF old = m_barcolor;
  m_barcolor = clrBar;
  OnDrawProgress();
  return old;
}

// Retrieves the background color of the progress bar control.
COLORREF 
StyleProgressCtrl::GetBkColor() const
{
  if(m_background == NO_COLOR)
  {
    return ThemeColor::GetColor(Colors::ColorCtrlBackground);
  }
  // Explicitly set user color
  return m_background;
}

// Retrieves the color of the progress bar control.
COLORREF 
StyleProgressCtrl::GetBarColor() const
{
  if(m_state == PBST_ERROR)
  {
    return RGB(0xFF,0,0); // Color RED
  }
  if(m_state == PBST_PAUSED)
  {
    return ThemeColor::GetColor(Colors::AccentColor3); // Lighter theme color
  }
  if(m_barcolor == NO_COLOR)
  {
    return ThemeColor::GetColor(Colors::AccentColor1); // Our theme
  }
  return m_barcolor; // Explicitly set user color
}

// Sets the state of the progress bar.
int 
StyleProgressCtrl::SetState(_In_ int iState)
{
  int old = m_state;
  if (iState == PBST_NORMAL || iState == PBST_PAUSED || iState == PBST_ERROR)
  {
    m_state = iState;
  }
  OnDrawProgress();
  return old;
}

// Retrieves the state of the progress bar.
int 
StyleProgressCtrl::GetState() const
{
  return m_state;
}

// Retrieves whether we should show the progress
bool 
StyleProgressCtrl::GetShowPercentage() const
{
  return m_showperc;
}

// Sets the showing of the progress in percentage
bool
StyleProgressCtrl::SetShowPercentage(bool p_show)
{
  return m_showperc = p_show;
}

// Sets the progress bar control to marquee mode.
BOOL 
StyleProgressCtrl::SetMarquee(_In_ BOOL fMarqueeMode, _In_ int nInterval)
{
  BOOL old = (m_state == PBST_MARQUEE);

  if(fMarqueeMode)
  {
    m_lower = 0;
    m_upper = 100;
    m_state = PBST_MARQUEE;
    SetTimer(1,nInterval,nullptr);
    m_position = 0;
  }
  else
  {
    m_state = PBST_NORMAL;
    KillTimer(1);
  }
  OnDrawProgress();
  return old;
}

// Steps the control by the value set with SetStep().
int 
StyleProgressCtrl::StepIt()
{
  int old = m_position;
  if (m_position < m_upper)
  {
    if(m_state == PBST_NORMAL)
    {
      m_position += m_step;
      BoundsCheck();
    }
    OnDrawProgress();
  }
  return old;
}

//////////////////////////////////////////////////////////////////////////
//
// Message handlers
//
//////////////////////////////////////////////////////////////////////////

void 
StyleProgressCtrl::OnTimer(UINT_PTR pID)
{
  if(pID == TIMER_MARQUEE)
  {
    // Do the marquee, max 125%
    if(m_position++ > (m_upper * (100 + (100 / MARQUEE_PART)) / 100))
    {
      m_position = 0;
    }
    OnDrawProgress();
  }
}

void
StyleProgressCtrl::OnPaint()
{
  // Still do the CWnd::Default to handle the message
  Default();
  // Draw the control
  OnDrawProgress();
}

void
StyleProgressCtrl::OnNcPaint()
{
  CDC* dc = GetDC();
  DrawFrame(dc);
  ReleaseDC(dc);
}

//////////////////////////////////////////////////////////////////////////
//
// PRIVATE
//
//////////////////////////////////////////////////////////////////////////

void
StyleProgressCtrl::BoundsCheck()
{
  if(m_position > m_upper)
  {
    m_position = m_upper;
  }
  if(m_position < m_lower)
  {
    m_position = m_lower;
  }
}

void
StyleProgressCtrl::DrawFrame(CDC* p_dc)
{
  CPen pen;
  pen.CreatePen(PS_SOLID,1,GetBarColor());
  HGDIOBJ orgpen = p_dc->SelectObject(pen);

  CRect rcItem;
  GetClientRect(&rcItem);

  p_dc->MoveTo(rcItem.left,     rcItem.top);
  p_dc->LineTo(rcItem.right - 1,rcItem.top);
  p_dc->LineTo(rcItem.right - 1,rcItem.bottom - 1);
  p_dc->LineTo(rcItem.left,     rcItem.bottom - 1);
  p_dc->LineTo(rcItem.left,     rcItem.top);
  p_dc->SelectObject(orgpen);
}

void
StyleProgressCtrl::DrawMarquee(CDC* p_dc,CRect& p_rect,bool p_vertical)
{
  if(p_vertical)
  {
    int newTop = p_rect.bottom - (m_position * p_rect.Height()) / (m_upper - m_lower);
    int newBot = newTop + (p_rect.Height() / MARQUEE_PART);
    if (newTop < p_rect.top)    newTop = p_rect.top;
    if (newBot > p_rect.bottom) newBot = p_rect.bottom;
    p_rect.top    = newTop;
    p_rect.bottom = newBot;
  }
  else
  {
    int newRight = (m_position * p_rect.Width()) / (m_upper - m_lower);
    int newLeft  = newRight - (p_rect.Width() / MARQUEE_PART);
    if(newRight > p_rect.right) newRight = p_rect.right;
    if(newLeft  < p_rect.left)  newLeft  = p_rect.left;
    p_rect.left  = newLeft;
    p_rect.right = newRight;

  }
  p_dc->FillSolidRect(p_rect,GetBarColor());
}

void
StyleProgressCtrl::OnDrawProgress()
{
  CDC* dc = GetDC();
  if (dc == nullptr)
  {
    return;
  }

  // Getting the client area
  CRect rcItem;
  GetClientRect(&rcItem);
  dc->FillSolidRect(rcItem,GetBkColor());

  bool vertical = GetStyle() & PBS_VERTICAL;

  DrawFrame(dc);

  // Here we draw our progress control
  rcItem.DeflateRect(2, 2);

  if (m_state == PBST_MARQUEE)
  {
    DrawMarquee(dc,rcItem,vertical);
    ReleaseDC(dc);
    return;
  }

  // This is our progress
  if(m_upper > m_lower)
  {
    CRect progress(rcItem);
    if(vertical)
    {
      int newTop = (m_position * rcItem.Height()) / (m_upper - m_lower);
      progress.top = rcItem.bottom - newTop;
    }
    else
    {
      int newRight = (m_position * rcItem.Width()) / (m_upper - m_lower);
      progress.right = rcItem.left + newRight;
    }
    dc->FillSolidRect(progress,GetBarColor());
  }
  else
  {
    // No progress possible (divide by zero would ensue)
  }

  // Show the percentage on horizontal progress bars
  if(m_showperc && !vertical)
  {
    CString showing;
    int percentage = m_upper ? ((m_position * 100) / m_upper) : 0;
    showing.Format(_T("%d %%"),percentage);
    int mode = dc->SetBkMode(TRANSPARENT);
    dc->SelectObject(&STYLEFONTS.DialogTextFont);
    dc->SetTextColor(ThemeColor::GetColor(Colors::ColorEditText));
    dc->DrawText(showing,&rcItem,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
    dc->SetBkMode(mode);
  }

  // Done with DC
  ReleaseDC(dc);
}
