////////////////////////////////////////////////////////////////////////
//
// File: SkinScrollWnd.cpp
// Function: Extra control window between control and the scrollbar
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
//////////////////////////////////////////////////////////////////////////
//
// SkinScrollWnd.cpp : implementation file
// Freely based on the following codeproject article:
// https://www.codeproject.com/Articles/14724/Replace-a-Window-s-Internal-Scrollbar-with-a-custo
// But with many changes and enhancements.
//
//////////////////////////////////////////////////////////////////////	

#include "stdafx.h"
#include "SkinScrollWnd.h"
#include "StyleColors.h"
#include "StyleEdit.h"
#include "StyleComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace ThemeColor;

#define TIMER_UPDATE      100               // Update the scrollbars after this amount of milliseconds
#define SF_LIMITERMOVE	  (WM_USER+100)     // User message to move the scrollbars, limiter and control

IMPLEMENT_DYNAMIC(SkinScrollWnd,CWnd);

extern UINT GetMouseScrollLines();

static LRESULT CALLBACK
HookWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
  SkinScrollWnd* skin = (SkinScrollWnd*)GetWindowLongPtr(hwnd,GWLP_USERDATA);
  if(skin == nullptr)
  {
    return 0;
  }
  if(msg == WM_DESTROY)
  {
    // Undo the hooking of the skin framework
    WNDPROC procOld = skin->m_funOldProc;
    UnskinWndScroll(CWnd::FromHandle(hwnd));
    return ::CallWindowProc(procOld,hwnd,msg,wp,lp);
  }
  if (msg == WM_MOUSEWHEEL)
  {
    short delta = (short)HIWORD(wp);
    static int MouseScrollLines = GetMouseScrollLines();
    int nRowsScrolled = MouseScrollLines * delta / 120;

    if (nRowsScrolled > 0)
    {
      for (int i = 0; i < nRowsScrolled; i++)
      {
        skin->PostMessage(WM_VSCROLL, SB_LINEUP, (LPARAM)skin->m_sbVert.m_hWnd);
      }
    }
    else
    {
      for (int i = 0; i > nRowsScrolled; i--)
      {
        skin->PostMessage(WM_VSCROLL, SB_LINEDOWN, (LPARAM)skin->m_sbVert.m_hWnd);
      }
    }
    return 0;
  }
  // The original WindowsProc of the control
  LRESULT lr = ::CallWindowProc(skin->m_funOldProc,hwnd,msg,wp,lp);

  if(skin->m_busy)
  {
    return lr;
  }

  if(msg == WM_ERASEBKGND)
  {
    // To do after the erasure of the background of the control
    // Re-do the scroll information
    SCROLLINFO si;
    DWORD dwStyle = ::GetWindowLong(hwnd,GWL_STYLE);
    if(dwStyle & WS_VSCROLL)
    {
      memset(&si,0,sizeof(si));
      si.cbSize = sizeof(si);
      si.fMask  = SIF_ALL;
      ::GetScrollInfo(hwnd,SB_VERT,&si);
      if(si.nMax  != skin->m_sbVert.m_si.nMax  ||
         si.nMin  != skin->m_sbVert.m_si.nMin  ||
         si.nPos  != skin->m_sbVert.m_si.nPos  ||
         si.nPage != skin->m_sbVert.m_si.nPage)
      {
        skin->m_sbVert.SetScrollInfo(&si,!skin->m_busy);
        skin->m_sbVert.EnableWindow((UINT)si.nMax >= si.nPage);
      }
    }
    if(dwStyle & WS_HSCROLL)
    {
      memset(&si,0,sizeof(si));
      si.cbSize = sizeof(si);
      si.fMask  = SIF_ALL;
      ::GetScrollInfo(hwnd,SB_HORZ,&si);
      if(si.nMax  != skin->m_sbHorz.m_si.nMax  ||
         si.nMin  != skin->m_sbHorz.m_si.nMin  ||
         si.nPos  != skin->m_sbHorz.m_si.nPos  ||
         si.nPage != skin->m_sbHorz.m_si.nPage)
      {
        skin->m_sbHorz.SetScrollInfo(&si,!skin->m_busy);
        skin->m_sbHorz.EnableWindow((UINT)si.nMax >= si.nPage);
      }
    }

    // Draw a frame around it
    skin->DrawFrame();
  }
  else if(msg == WM_NCCALCSIZE && wp)
  {
    // To do after the calculation of the Non-Client area
    LPNCCALCSIZE_PARAMS pNcCalcSizeParam = (LPNCCALCSIZE_PARAMS)lp;
    DWORD dwStyle     = ::GetWindowLong(hwnd,GWL_STYLE);
    DWORD dwExStyle   = ::GetWindowLong(hwnd,GWL_EXSTYLE);
    bool  bLeftScroll = (dwExStyle & WS_EX_LEFTSCROLLBAR) != 0;
    int nWid=::GetSystemMetrics(SM_CXVSCROLL);
    if(dwStyle & WS_VSCROLL) 
    {
      if (bLeftScroll)
      {
        pNcCalcSizeParam->rgrc[0].left -= nWid - skin->m_nScrollWid;
      }
      else
      {
        pNcCalcSizeParam->rgrc[0].right += nWid - skin->m_nScrollWid;
      }
    }
    if(dwStyle & WS_HSCROLL)
    {
      pNcCalcSizeParam->rgrc[0].bottom += nWid - skin->m_nScrollWid;
    }
    RECT rcVert,rcHorz;
      
    nWid = skin->m_nScrollWid;
    int border = skin->m_borderSize;
    int bias   = skin->m_scrollBias;
    if(bLeftScroll)
    {
      int nLeft     = pNcCalcSizeParam->rgrc[0].left;
      int nBottom   = pNcCalcSizeParam->rgrc[0].bottom;
      rcVert.right  = nLeft - bias;
      rcVert.left   = nLeft - nWid - bias;
      rcVert.top    = border;
      rcVert.bottom = nBottom - border + 1 - bias;
      rcHorz.left   = nLeft + border;
      rcHorz.right  = pNcCalcSizeParam->rgrc[0].right - border;
      rcHorz.top    = nBottom + bias;
      rcHorz.bottom = nBottom + nWid + bias;
    }
    else
    {
      int nRight    = pNcCalcSizeParam->rgrc[0].right;
      int nBottom   = pNcCalcSizeParam->rgrc[0].bottom;
      rcVert.left   = nRight + bias;
      rcVert.right  = nRight + nWid + bias;
      rcVert.top    = border;
      rcVert.bottom = nBottom - border + 1 - bias;
      rcHorz.left   = border;
      rcHorz.right  = nRight  - border + 1;
      rcHorz.top    = nBottom + bias;
      rcHorz.bottom = nBottom + nWid + bias;
    }
    if(dwStyle & WS_VSCROLL && dwStyle & WS_HSCROLL)
    {
      skin->m_nAngleType = bLeftScroll ? 1 : 2;
    }
    else
    {
      skin->m_nAngleType = 0;
    }
    if(dwStyle & WS_VSCROLL)
    {
      skin->m_sbVert.MoveWindow(&rcVert);
      skin->m_sbVert.ShowWindow(SW_SHOW);
    }
    else
    {
      skin->m_sbVert.ShowWindow(SW_HIDE);
    }
    if(dwStyle & WS_HSCROLL)
    {
      skin->m_sbHorz.MoveWindow(&rcHorz);
      skin->m_sbHorz.ShowWindow(SW_SHOW);
    }
    else
    {
      skin->m_sbHorz.ShowWindow(SW_HIDE);
    }
    skin->PostMessage(SF_LIMITERMOVE,dwStyle & (WS_VSCROLL | WS_HSCROLL),bLeftScroll);
    // First setting of scrollbars in this amount of delay time
    skin->SetTimer(TIMER_UPDATE,FRAME_FIRST_SCROLLBARS,nullptr);
  }
  return lr;
}

// Called by the Init() of the control
extern "C" SkinScrollWnd* 
SkinWndScroll(CWnd* p_wnd,int p_bordersize /*=0*/,int p_clientBias /*=0*/)
{
  // Keep the original owner
  CWnd* owner = p_wnd->GetOwner();
  // Create and apply the skin
  SkinScrollWnd* frame = new SkinScrollWnd();
  frame->SkinWindow(p_wnd,p_bordersize,p_clientBias);
  // Re-Apply the original owner
  p_wnd->SetOwner(owner);

  if(owner != nullptr)
  {
    DWORD dwExStyle = owner->GetExStyle();
    if((dwExStyle & WS_EX_CONTROLPARENT) != 0)
    {
      frame->ModifyStyleEx(0,WS_EX_CONTROLPARENT);
      frame->m_wndLimit.ModifyStyleEx(0,WS_EX_CONTROLPARENT);
    }
  }
  return frame;
}

// Called by the Destroy or the DTOR of the control
extern "C" BOOL 
UnskinWndScroll(CWnd* p_wnd)
{
  SkinScrollWnd* frame = (SkinScrollWnd *)GetWindowLongPtr(p_wnd->m_hWnd,GWLP_USERDATA);
  if(frame) 
  {
    RECT rc;
    CWnd *pParent = frame->GetParent();
    frame->GetWindowRect(&rc);
    pParent->ScreenToClient(&rc);
    SetWindowLongPtr(p_wnd->m_hWnd,GWLP_WNDPROC,(LONG_PTR)frame->m_funOldProc);
    SetWindowLongPtr(p_wnd->m_hWnd,GWLP_USERDATA,0);
    p_wnd->SetParent(pParent);
    p_wnd->MoveWindow(&rc);
    frame->DestroyWindow();
    delete frame;
  }
  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CSkinScrollWnd

SkinScrollWnd::SkinScrollWnd()
{
  m_funOldProc   = nullptr;
  m_busy         = FALSE;
  m_moveClient   = FALSE;
  m_doTimer      = FALSE;
  m_doPaint      = FALSE;
  m_doCapture    = FALSE;
  m_didCapture   = FALSE;
  m_nScrollWid   = 16;
  m_nAngleType   = 0;
  m_lastColor    = RGB(0,0,0);
  m_borderColor  = RGB(0xFF,0xFF,0xFF);
  m_borderSize   = 0;
  m_scrollBias   = -1;
  m_clientBias   = 0;
  m_captureFlags = 0;
  m_hoverTime    = 0;
}

SkinScrollWnd::~SkinScrollWnd()
{
  // Nothing to do yet
}

BEGIN_MESSAGE_MAP(SkinScrollWnd, CWnd)
  ON_WM_SIZE()
  ON_WM_MOVE()
  ON_WM_VSCROLL()
  ON_WM_HSCROLL()
  ON_WM_PAINT()
  ON_WM_NCPAINT()
  ON_WM_TIMER()
  ON_WM_SETFOCUS()
  ON_WM_MOUSEMOVE()
  ON_WM_CAPTURECHANGED()
  ON_MESSAGE(SF_LIMITERMOVE,  OnLimiterMove)
  // MOUSE BUTTON MESSAGES
  ON_MESSAGE(WM_MOUSEHOVER,   OnMouseHover)
  ON_MESSAGE(WM_MOUSELEAVE,   OnMouseLeave)
  ON_MESSAGE(WM_LBUTTONDOWN,  OnLButtonDown)
  ON_MESSAGE(WM_LBUTTONUP,    OnLButtonUp)
  ON_MESSAGE(WM_LBUTTONDBLCLK,OnLButtonDblClk)
  ON_MESSAGE(WM_RBUTTONDOWN,  OnRButtonDown)
  ON_MESSAGE(WM_RBUTTONUP,    OnRButtonUp)
  ON_MESSAGE(WM_RBUTTONDBLCLK,OnRButtonDblClk)
  ON_MESSAGE(WM_MBUTTONDOWN,  OnMButtonDown)
  ON_MESSAGE(WM_MBUTTONUP,    OnMButtonUp)
  ON_MESSAGE(WM_MBUTTONDBLCLK,OnMButtonDblClk)
  ON_MESSAGE(WM_XBUTTONDOWN,  OnXButtonDown)
  ON_MESSAGE(WM_XBUTTONUP,    OnXButtonUp)
  ON_MESSAGE(WM_XBUTTONDBLCLK,OnXButtonDblClk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinScrollWnd message handlers
//
BOOL
SkinScrollWnd::SkinWindow(CWnd* p_wnd,int p_borderSize /*=0*/,int p_clientBias /*=0*/)
{
  ASSERT(m_hWnd == nullptr);
  HBITMAP hBmpScroll = ThemeColor::GetScrollbarBitmap();
  BITMAP bm;
  GetObject(hBmpScroll,sizeof(bm),&bm);
  m_nScrollWid = bm.bmWidth / 9;
  m_clientBias = p_clientBias;

  // Calculate the frame and new control rectangles
  CWnd* parent = p_wnd->GetParent();
  if(parent == nullptr)
  {
    // Must be the desktop window
    parent = GetDesktopWindow();
  }
  CRect rcFrm;
  p_wnd->GetWindowRect(&rcFrm);
  parent->ScreenToClient(&rcFrm);

  // Propagate the ID and styles of the control window
  UINT  uID          = p_wnd->GetDlgCtrlID();
  DWORD dwStyle      = p_wnd->GetStyle();
  DWORD dwExStyle    = p_wnd->GetExStyle();
  DWORD dwFrmStyle   = WS_CHILD|SS_NOTIFY;
  DWORD dwFrmStyleEx = 0;
  if(dwExStyle & WS_EX_TOOLWINDOW)
  {
    dwFrmStyleEx |= WS_EX_TOOLWINDOW;
    dwFrmStyleEx |= WS_EX_TOPMOST;
  }
  if(dwStyle & WS_VISIBLE)
  {
    dwFrmStyle |= WS_VISIBLE;
  }
  // Taking care of the borders (WS_BORDER / WS_EX_CLIENTEDGE)
  if(dwStyle & WS_BORDER)
  {
    p_wnd->ModifyStyle(WS_BORDER,0);
    m_borderSize = FRAME_MINIMAL_BORDER;
  }
  else if(dwExStyle & WS_EX_CLIENTEDGE)
  {
    p_wnd->ModifyStyleEx(WS_EX_CLIENTEDGE,0);
    m_borderSize = FRAME_MINIMAL_BORDER;
  }
  // Want a bigger border in this control?
  if(p_borderSize > m_borderSize)
  {
    m_borderSize = p_borderSize;
  }

  // Getting our tabstop order before creating new sub windows to the parent
  CWnd* next = nullptr;
  CWnd* prev = nullptr;
  if (dwStyle & WS_TABSTOP)
  {
    next = p_wnd->GetNextWindow(GW_HWNDNEXT);
    prev = p_wnd->GetNextWindow(GW_HWNDPREV);
  }

  // In case we have NO border and no bias, the limiter must align with the control
  CRect original;
  p_wnd->GetWindowRect(&original);
  ::OffsetRect(&original,-original.left,-original.top);

  // Create the skinning windows (Frame, limiter and scrollbars)
  this->CreateEx(dwFrmStyleEx,AfxRegisterWndClass(NULL),_T("CXSF"),dwFrmStyle,rcFrm,parent,uID);
  m_wndLimit.Create(NULL,_T("LIMIT"),WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,original,this,200);
  m_sbHorz.Create(WS_CHILD|SBS_HORZ,CRect(0,0,0,0),this,100);
  m_sbVert.Create(WS_CHILD|SBS_VERT,CRect(0,0,0,0),this,101);
  m_sbHorz.SetBitmapSize();
  m_sbVert.SetBitmapSize();

  // Connect the window with the limiter and set the hooking procedure
  p_wnd->SetParent(&m_wndLimit);
  SetWindowLongPtr(p_wnd->m_hWnd,GWLP_USERDATA,(LONG_PTR)this);
  m_funOldProc=(WNDPROC)SetWindowLongPtr(p_wnd->m_hWnd,GWLP_WNDPROC,(LONG_PTR)HookWndProc);

  // Restore the TAB order
  if(next || prev)
  {
    // Propagate tab stop to this window
    p_wnd->ModifyStyle(WS_TABSTOP,0);

    // Take the original control out of the tab order
    ModifyStyle(0,WS_TABSTOP);
    p_wnd->SetWindowPos(nullptr, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

    // Skin window must be re-connected in this order
    // Previous, even if nullptr
    // next window connected to this skin
    SetWindowPos(prev,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
    if(next)
    {
      next->SetWindowPos(this,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
    }
  }

  // Calculate the NEW window size for the control
  // This will be the new client area of the skinned control
  RepositionControlWindow(p_wnd);

  // First setting of scrollbars in this amount of delay time
  // SetTimer(TIMER_UPDATE,FRAME_FIRST_SCROLLBARS,nullptr);
  return TRUE;
}

// This is the original control that has been skinned!
CWnd* 
SkinScrollWnd::GetControl()
{
  return m_wndLimit.GetWindow(GW_CHILD);
}

// Calculate the position of the control within the skin frame
// To be done initially and after a move on the control itself
void
SkinScrollWnd::RepositionControlWindow(CWnd* p_wnd /*=nullptr*/)
{
  if(p_wnd == nullptr)
  {
    p_wnd = m_wndLimit.GetWindow(GW_CHILD);
  }
  // Calculate the window size for the control
  CRect control;
  CalculateControlSize(control);
  if(m_borderSize == 0)
  {
  	RecalcSkinSize(p_wnd);
  }
  m_moveClient = TRUE;
  p_wnd->MoveWindow(&control);
  m_moveClient = FALSE;
}

// In case we have no border or if we resize no WM_NCCALCSIZE
// will be triggered as no non-client area get's painted
// So we fake it here
void 
SkinScrollWnd::RecalcSkinSize(CWnd* p_control)
{
  CRect client;
  p_control->GetClientRect(&client);
  NCCALCSIZE_PARAMS nccalc;
  memset(&nccalc, 0, sizeof(NCCALCSIZE_PARAMS));
  nccalc.rgrc[0] = client;
  p_control->SendMessage(WM_NCCALCSIZE,1,(LPARAM)&nccalc);
}

void
SkinScrollWnd::SetScrollbarBias(int p_bias)
{
  m_scrollBias = p_bias;
}

void
SkinScrollWnd::SetBorderColor(COLORREF p_color)
{
  m_borderColor = p_color;
}

void
SkinScrollWnd::SetPollScrollbars(BOOL p_poll)
{
  m_doTimer = p_poll;
}

void
SkinScrollWnd::SetPaintPassthrough(BOOL p_paint)
{
  m_doPaint = p_paint;
}

void
SkinScrollWnd::SetMouseCapture(BOOL p_capture /*= TRUE*/,DWORD p_flags /*= TME_HOVER | TME_LEAVE*/,DWORD p_hoverTime /*= HOVER_DEFAULT*/)
{
  m_doCapture    = p_capture;
  m_captureFlags = p_flags;
  m_hoverTime    = p_hoverTime;
}

// Calculate the size of the encapsulated control
void
SkinScrollWnd::CalculateControlSize(CRect& p_rect)
{
  // Getting the frame window
  GetWindowRect(&p_rect);
  ::OffsetRect(&p_rect,-p_rect.left,-p_rect.top);
  // Take border into account
  if (m_borderSize)
  {
    p_rect.DeflateRect(m_borderSize, m_borderSize);
  }
  // Some clients have a bias
  if(m_clientBias)
  {
    if(m_sbVert.IsWindowVisible())
    {
      p_rect.right += m_clientBias;
    }
    if (m_sbHorz.IsWindowVisible())
    {
      p_rect.bottom += m_clientBias;
    }
  }
}

// If the skin frame gets resized, we must also resize the control
// within the frame, and possibly recalc the scrollbar sizes
//
// Set "m_moveClient" to give the control a way to detect recursive resizes
//
void 
SkinScrollWnd::OnSize(UINT nType, int cx, int cy) 
{
  CWnd::OnSize(nType, cx, cy);
  if(cx == 0 || cy == 0)
  {
    return;
  }
  m_moveClient = TRUE;
  if(m_wndLimit.m_hWnd)
  {
    CWnd *pWnd = m_wndLimit.GetWindow(GW_CHILD);
    CRect rect;
    CalculateControlSize(rect);
    pWnd->MoveWindow(&rect);

    if(m_borderSize == 0)
    {
      RecalcSkinSize(pWnd);
    }
    SetTimer(TIMER_UPDATE,FRAME_FIRST_SCROLLBARS,nullptr);
  }
  m_moveClient = FALSE;
}

// Moving just needs to invalidate the control within the skin
// For repaint errors: trigger the drawing of the scrollbars
void 
SkinScrollWnd::OnMove(int x,int y)
{
  CWnd::OnMove(x,y);
  if(m_wndLimit.m_hWnd)
  {
    CWnd* pWnd = m_wndLimit.GetWindow(GW_CHILD);
    pWnd->Invalidate();
    SetTimer(TIMER_UPDATE,FRAME_FIRST_SCROLLBARS,nullptr);
  }
}

// Capture the scroll position on a vertical scroll action
void 
SkinScrollWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
  CWnd *pWnd = m_wndLimit.GetWindow(GW_CHILD);
  
  m_busy = TRUE;
  pWnd->SendMessage(WM_VSCROLL,MAKELONG(nSBCode,nPos),0);
  m_busy = FALSE;

  if(nSBCode == SB_THUMBTRACK)
  {
    return;
  }
  SCROLLINFO si = { 0 };
  si.cbSize = sizeof(si);
  si.fMask  = SIF_ALL;
  pWnd->GetScrollInfo(SB_VERT,&si);
  pScrollBar->SetScrollInfo(&si,TRUE);
  DrawFrame();
}

// Capture the scroll position on a horizontal scroll action
void 
SkinScrollWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
  CWnd *pWnd=m_wndLimit.GetWindow(GW_CHILD);

  m_busy = TRUE;
  pWnd->SendMessage(WM_HSCROLL,MAKELONG(nSBCode,nPos),0);
  m_busy = FALSE;

  if (nSBCode == SB_THUMBTRACK)
  {
    return;
  }
  SCROLLINFO si = { 0 };
  si.cbSize = sizeof(si);
  si.fMask  = SIF_ALL;
  pWnd->GetScrollInfo(SB_HORZ,&si);
  pScrollBar->SetScrollInfo(&si,TRUE);
  DrawFrame();
}

// If we have border sizes larger than 1, paint the inside
// of the border in the background color. 
// e.g. to erase the error status color of a control
void 
SkinScrollWnd::OnNcPaint()
{
  CWnd::OnNcPaint();

  // Optimize: Only for border-sizes > 1
  if(m_borderSize <= FRAME_MINIMAL_BORDER)
  {
    return;
  }

  // Erase old border
  CRect rc;
  CDC* dc = this->GetDC();
  this->GetWindowRect(&rc);
  OffsetRect(&rc,-rc.left,-rc.top);

  bool readonly = false;
  DWORD color   = m_borderColor;
  CWnd* control = m_wndLimit.GetSafeHwnd() ? m_wndLimit.GetWindow(GW_CHILD) : nullptr;
  if(control)
  {
    readonly = (control->GetStyle() & ES_READONLY) > 0 || !control->IsWindowEnabled();
    if(readonly)
    {
      color = ThemeColor::GetColor(Colors::ColorWindowFrame);
    }
  }

  StyleEdit* edit = reinterpret_cast<StyleEdit*>(control);
  if(edit && edit->GetIsComboBox())
  {
    color = readonly ? m_borderColor : ThemeColor::GetColor(Colors::ColorCtrlBackground); // Assistant0
  }

  if(dc)
  {
    CPen pen;
    pen.CreatePen(PS_SOLID,1,color);
    HGDIOBJ orgpen = dc->SelectObject(pen);

    for(int size = FRAME_MINIMAL_BORDER;size < m_borderSize; ++size)
    {
      // One smaller
      InflateRect(&rc, -1, -1);
      // Draw frame without changing the contents of the rectangle
      dc->MoveTo(rc.left,      rc.top);
      dc->LineTo(rc.right - 1, rc.top);
      dc->LineTo(rc.right - 1, rc.bottom - 1);
      dc->LineTo(rc.left,      rc.bottom - 1);
      dc->LineTo(rc.left,      rc.top);
    }
    dc->SelectObject(orgpen);
  }
  ReleaseDC(dc);
}

// If we paint the control, there is not much to do. But we do:
// 1) Paint the angle if we have two scroll bars (left or right)
//    Or tell the control to paint that bit
// 2) Call the DrawFrame. Draws frame around control OR 
//    Let the control draw it's own frame
// The client area never gets painted: It contains the control!!
void 
SkinScrollWnd::OnPaint() 
{
  CPaintDC dc(this); // device context for painting
  CDC memdc;
  memdc.CreateCompatibleDC(&dc);
  HGDIOBJ hOldBmp = ::SelectObject(memdc,ThemeColor::GetScrollbarBitmap());
  RECT rc;
  GetClientRect(&rc);
  if(m_nAngleType == 0 || m_doPaint)
  {
    CWnd* child = m_wndLimit.GetWindow(GW_CHILD);
    if (child)
    {
      child->SendMessage(WM_PAINT,0,0);
    }
  }
  else if(m_nAngleType == 1)
  {
    dc.BitBlt(rc.left   + m_borderSize
             ,rc.bottom - m_borderSize - m_nScrollWid
             ,m_nScrollWid
             ,m_nScrollWid
             ,&memdc
             ,m_nScrollWid * 8
             ,m_nScrollWid * 1
             ,SRCCOPY);
  }
  else if(m_nAngleType == 2)
  {
    dc.BitBlt(rc.right  - m_nScrollWid - m_borderSize
             ,rc.bottom - m_nScrollWid - m_borderSize
             ,m_nScrollWid
             ,m_nScrollWid
             ,&memdc
             ,m_nScrollWid * 8
             ,m_nScrollWid * 0
             ,SRCCOPY);
  }
  ::SelectObject(memdc,hOldBmp);

  DrawFrame();
}

// Once-in-a-while we set the scrollbars and draw the frame
// Essentially after creating or resizing the frame
void 
SkinScrollWnd::OnTimer(UINT_PTR nIDEvent) 
{
  CWnd *pWnd = m_wndLimit.GetWindow(GW_CHILD);
  // Too early in the construction?
  if (pWnd == nullptr)
  {
    return;
  }
  SCROLLINFO si1 = { 0 };
  si1.cbSize = sizeof(SCROLLINFO);
  si1.fMask  = SIF_ALL;
  SCROLLINFO si2 = { 0 };
  if(GetWindowLong(m_sbVert.m_hWnd,GWL_STYLE) & WS_VISIBLE && !m_sbVert.m_bDrag)
  {
    pWnd->GetScrollInfo(SB_VERT,&si1);
    m_sbVert.GetScrollInfo(&si2,SIF_ALL);
    if (si1.nMax != si2.nMax || si1.nMin != si2.nMin || si1.nPos != si2.nPos || si1.nPage != si2.nPage)
    {
      m_sbVert.SetScrollInfo(&si1);
    }
  }
  if (GetWindowLong(m_sbHorz.m_hWnd, GWL_STYLE) & WS_VISIBLE && !m_sbHorz.m_bDrag)
  {
    pWnd->GetScrollInfo(SB_HORZ, &si1);
    m_sbHorz.GetScrollInfo(&si2, SIF_ALL);
    if (si1.nMax != si2.nMax || si1.nMin != si2.nMin || si1.nPos != si2.nPos || si1.nPage != si2.nPage)
    {
      m_sbHorz.SetScrollInfo(&si1);
    }
  }
  if((nIDEvent == TIMER_UPDATE) && (m_doTimer == FALSE))
  {
    KillTimer(nIDEvent);
  }
  DrawFrame();
}

// Resize and move the limiter around the control
// so that the original scrollbars are hidden !!!
LRESULT 
SkinScrollWnd::OnLimiterMove(WPARAM wParam, LPARAM lParam)
{
  m_busy = TRUE;
  bool bVScroll    = ((wParam & WS_VSCROLL) != 0);
  bool bHScroll    = ((wParam & WS_HSCROLL) != 0);
  bool bLeftScroll = (bool) lParam;
  RECT rcClient;
  GetClientRect(&rcClient);
  RECT rcLimit,rcWnd;
  rcWnd   = rcClient;
  rcLimit = rcClient;

  if(bHScroll)
  {
    rcLimit.bottom -= m_nScrollWid + m_borderSize;
  }
  else
  {
    rcLimit.bottom -= m_borderSize;
  }
  if(bVScroll)
  {
    if(bLeftScroll)
    {
      rcLimit.left += m_nScrollWid + m_borderSize;
      OffsetRect(&rcWnd,-m_nScrollWid-m_borderSize,0);
    }
    else
    {
      rcLimit.right -= m_nScrollWid + m_borderSize;
    }
  }

  m_wndLimit.MoveWindow(&rcLimit);
  CWnd* child = m_wndLimit.GetWindow(GW_CHILD);
  if(child)
  {
    child->SetWindowPos(nullptr,rcWnd.left + m_borderSize,m_borderSize,0,0,SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE);
  }
  DrawFrame();
  m_busy = FALSE;
  return 0;
}

// Draw a one-line frame around the control
// But some controls do it themselves.
void
SkinScrollWnd::DrawFrame()
{
  // See if we must draw a border
  if (m_borderSize == 0)
  {
    return;
  }

  COLORREF color = ThemeColor::GetColor(Colors::AccentColor2);
  if (m_wndLimit.m_hWnd)
  {
    CWnd* control = m_wndLimit.GetWindow(GW_CHILD);
    if (control)
    {
      StyleEdit* edit = DYNAMIC_DOWNCAST(StyleEdit,control);
      if(edit)
      {
        edit->DrawEditFrame();
        return;
      }
      SCBTextEdit* cbEdit = DYNAMIC_DOWNCAST(SCBTextEdit,control);
      if(cbEdit)
      {
        cbEdit->DrawEditFrame();
        return;
      }
    }
    if(control == GetFocus())
    {
      color = ThemeColor::GetColor(Colors::AccentColor1);
    }
  }
  DrawFrame(color);
}

// Draw a frame around the control
// Possibly more than 1 line, depending on the control
void
SkinScrollWnd::DrawFrame(COLORREF p_color,int p_size /*=1*/)
{
  // See if we must draw a border
  if(m_borderSize == 0)
  {
    return;
  }

  CRect rcItem;
  CDC* dc = this->GetDC();
  this->GetClientRect(&rcItem);

  if(p_color == FRAME_DEFAULT_COLOR)
  {
    p_color = m_lastColor;
  }
  if(dc)
  {
    CPen pen;
    pen.CreatePen(PS_SOLID,1,p_color);
    HGDIOBJ orgpen = dc->SelectObject(pen);

    while(p_size--)
    {
      dc->MoveTo(rcItem.left,     rcItem.top);
      dc->LineTo(rcItem.right - 1,rcItem.top);
      dc->LineTo(rcItem.right - 1,rcItem.bottom - 1);
      dc->LineTo(rcItem.left,     rcItem.bottom - 1);
      dc->LineTo(rcItem.left,     rcItem.top);
      if (p_size)
      {
        if(!::InflateRect(rcItem, -1, -1))
        {
          break;
        }
      }
    }
    dc->SelectObject(orgpen);
  }
  m_lastColor = p_color;
  ReleaseDC(dc);
}

//////////////////////////////////////////////////////////////////////////
//
// PROPAGATION of messages to the original windows
//
//////////////////////////////////////////////////////////////////////////

void
SkinScrollWnd::OnSetFocus(CWnd* p_old)
{
  CWnd* child = m_wndLimit.GetWindow(GW_CHILD);
  if(child)
  {
    child->SetFocus();
  }
}

void
SkinScrollWnd::SkinSetCapture()
{
  if(m_doCapture && !m_didCapture)
  {
    SetCapture();
    m_didCapture = TRUE;
  }
}

void
SkinScrollWnd::SkinReleaseCapture()
{
  if(m_didCapture)
  {
    m_didCapture = FALSE;
    ReleaseCapture();
  }
}

// If the control did it's own capture and it 
// releases it again (pwnd == nullpptr) then
// in case we capture, we now re-capture the mouse
void    
SkinScrollWnd::OnCaptureChanged(CWnd* pWnd)
{
  if(m_didCapture && pWnd == nullptr)
  {
    SetCapture();
  }
  else m_didCapture = FALSE;
}

void
SkinScrollWnd::SkinSetMouseTracking()
{
  if(m_captureFlags && m_hoverTime)
  {
    TRACKMOUSEEVENT mouseEvent;
    mouseEvent.cbSize      = sizeof(TRACKMOUSEEVENT);
    mouseEvent.dwFlags     = m_captureFlags;
    mouseEvent.dwHoverTime = m_hoverTime;
    mouseEvent.hwndTrack   = m_hWnd;
    _TrackMouseEvent(&mouseEvent);
  }
}

void
SkinScrollWnd::SkinCancelMouseTracking()
{
  if(m_captureFlags && m_hoverTime)
  {
    TRACKMOUSEEVENT mouseEvent;
    mouseEvent.cbSize      = sizeof(TRACKMOUSEEVENT);
    mouseEvent.dwFlags     = m_captureFlags | TME_CANCEL;
    mouseEvent.dwHoverTime = 0;
    mouseEvent.hwndTrack   = m_hWnd;
    _TrackMouseEvent(&mouseEvent);
  }
}

void
SkinScrollWnd::OnMouseMove(UINT nFlags,CPoint point)
{
  // Try capture
  SkinSetCapture();
  // Try tracking
  SkinSetMouseTracking();

  CWnd* child = m_wndLimit.GetWindow(GW_CHILD);
  if(child)
  {
    LPARAM param = MAKELONG(point.x,point.y);
    child->SendMessage(WM_MOUSEMOVE,(WPARAM)nFlags,param);
  }
}

LRESULT 
SkinScrollWnd::OnMouseHover(WPARAM wParam,LPARAM lParam)
{
  CWnd* child = m_wndLimit.GetWindow(GW_CHILD);
  if(child)
  {
    // Return 0 if processed
    return child->SendMessage(WM_MOUSEHOVER,wParam,lParam);
  }
  return 1;
}

LRESULT 
SkinScrollWnd::OnMouseLeave(WPARAM wParam,LPARAM lParam)
{
  CWnd* child = m_wndLimit.GetWindow(GW_CHILD);
  if(child)
  {
    // Return 0 if processed
    return child->SendMessage(WM_MOUSELEAVE,wParam,lParam);
  }
  return 1;
}

LRESULT
SkinScrollWnd::OnLButtonDown(WPARAM wParam,LPARAM lParam)
{
  CWnd* child = m_wndLimit.GetWindow(GW_CHILD);
  if(child)
  {
    // Return 0 if processed
    return child->SendMessage(WM_LBUTTONDOWN,wParam,lParam);
  }
  return 1;
}

LRESULT
SkinScrollWnd::OnLButtonUp(WPARAM wParam,LPARAM lParam)
{
  CWnd* child = m_wndLimit.GetWindow(GW_CHILD);
  if(child)
  {
    // Return 0 if processed
    return child->SendMessage(WM_LBUTTONUP,wParam,lParam);
  }
  return 1;
}

LRESULT
SkinScrollWnd::OnLButtonDblClk(WPARAM wParam,LPARAM lParam)
{
  CWnd* child = m_wndLimit.GetWindow(GW_CHILD);
  if(child)
  {
    // Return 0 if processed
    return child->SendMessage(WM_LBUTTONDBLCLK,wParam,lParam);
  }
  return 1;
}

LRESULT
SkinScrollWnd::OnRButtonDown(WPARAM wParam,LPARAM lParam)
{
  CWnd* child = m_wndLimit.GetWindow(GW_CHILD);
  if(child)
  {
    // Return 0 if processed
    return child->SendMessage(WM_RBUTTONDOWN,wParam,lParam);
  }
  return 1;
}

LRESULT
SkinScrollWnd::OnRButtonUp(WPARAM wParam,LPARAM lParam)
{
  CWnd* child = m_wndLimit.GetWindow(GW_CHILD);
  if(child)
  {
    // Return 0 if processed
    return child->SendMessage(WM_RBUTTONUP,wParam,lParam);
  }
  return 1;
}

LRESULT
SkinScrollWnd::OnRButtonDblClk(WPARAM wParam,LPARAM lParam)
{
  CWnd* child = m_wndLimit.GetWindow(GW_CHILD);
  if(child)
  {
    // Return 0 if processed
    return child->SendMessage(WM_RBUTTONDBLCLK,wParam,lParam);
  }
  return 1;
}

LRESULT
SkinScrollWnd::OnMButtonDown(WPARAM wParam,LPARAM lParam)
{
  CWnd* child = m_wndLimit.GetWindow(GW_CHILD);
  if(child)
  {
    // Return 0 if processed
    return child->SendMessage(WM_MBUTTONDOWN,wParam,lParam);
  }
  return 1;
}

LRESULT
SkinScrollWnd::OnMButtonUp(WPARAM wParam,LPARAM lParam)
{
  CWnd* child = m_wndLimit.GetWindow(GW_CHILD);
  if(child)
  {
    // Return 0 if processed
    return child->SendMessage(WM_MBUTTONUP,wParam,lParam);
  }
  return 1;
}

LRESULT
SkinScrollWnd::OnMButtonDblClk(WPARAM wParam,LPARAM lParam)
{
  CWnd* child = m_wndLimit.GetWindow(GW_CHILD);
  if(child)
  {
    // Return 0 if processed
    return child->SendMessage(WM_MBUTTONDBLCLK,wParam,lParam);
  }
  return 1;
}

LRESULT
SkinScrollWnd::OnXButtonDown(WPARAM wParam,LPARAM lParam)
{
  CWnd* child = m_wndLimit.GetWindow(GW_CHILD);
  if(child)
  {
    // Return TRUE if processed
    return child->SendMessage(WM_XBUTTONDOWN,wParam,lParam);
  }
  return FALSE;
}

LRESULT
SkinScrollWnd::OnXButtonUp(WPARAM wParam,LPARAM lParam)
{
  CWnd* child = m_wndLimit.GetWindow(GW_CHILD);
  if(child)
  {
    // Return TRUE if processed
    return child->SendMessage(WM_XBUTTONUP,wParam,lParam);
  }
  return FALSE;
}

LRESULT
SkinScrollWnd::OnXButtonDblClk(WPARAM wParam,LPARAM lParam)
{
  CWnd* child = m_wndLimit.GetWindow(GW_CHILD);
  if(child)
  {
    // Return TRUE if processed
    return child->SendMessage(WM_XBUTTONDBLCLK,wParam,lParam);
  }
  return FALSE;
}
