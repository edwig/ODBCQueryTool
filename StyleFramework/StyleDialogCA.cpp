////////////////////////////////////////////////////////////////////////
//
// File:     StyleDialogCA.h
// Function: General Dialog box for styling windows
//           CA stands for "Client Area"
//
// Dialog with special styled header and behavior
// Paints the frame INSIDE of the client area (defined in the resources)
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace ThemeColor;

IMPLEMENT_DYNAMIC(StyleDialogCA,CDialogEx);

StyleDialogCA::StyleDialogCA(UINT nIDTemplate,CWnd* p_parent, bool caption /*=true*/)
              :CDialogEx(nIDTemplate, p_parent)
              ,m_caption(caption)
{
}

StyleDialogCA::~StyleDialogCA() 
{
}

BEGIN_MESSAGE_MAP(StyleDialogCA, CDialogEx)
  ON_WM_CREATE()
  ON_WM_ERASEBKGND()
  ON_WM_MOUSEMOVE()
  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONUP()
  ON_WM_MOUSEMOVE()
  ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
  ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
  ON_MESSAGE(WM_GRAYSCREEN, OnGrayScreen)
  ON_WM_CTLCOLOR()
  ON_WM_SIZE()
END_MESSAGE_MAP()

BOOL
StyleDialogCA::PreCreateWindow(CREATESTRUCT& cs)
{
  cs.dwExStyle |= WS_EX_TOPMOST;
  return CDialogEx::PreCreateWindow(cs);
}

int
StyleDialogCA::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  m_grayScreen.CreateEx(0, AfxRegisterWndClass(0),_T(""),WS_POPUP,CRect(0,0,0,0),this,0);
  return 0;
}

void
StyleDialogCA::OnDestroy()
{
  m_grayScreen.DestroyWindow();
}

BOOL
StyleDialogCA::OnInitDialog()
{ 
  ModifyStyleEx(0, WS_EX_LAYERED);
  SetLayeredWindowAttributes(ColorWindowTransparent,0,LWA_COLORKEY);

  CRect window;
  GetWindowRect(window);
  SetWindowPos(nullptr, 0, 0, window.Width() + WINDOWSHADOWBORDER, window.Height() + WINDOWSHADOWBORDER, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

  m_defaultBrush.DeleteObject();
  m_defaultBrush.CreateSolidBrush(ThemeColor::GetColor(Colors::ColorWindowFrame));

  return CDialogEx::OnInitDialog();
}

INT_PTR StyleDialogCA::DoModal()
{
  AutoStyleGrayScreen grayscreen;
  return CDialogEx::DoModal();
}

void StyleDialogCA::ShowCloseButton(bool p_show)
{
  if (m_close != p_show)
  {
    m_close = p_show;
    InvalidateRect(m_closeRect);
  }
}

BOOL 
StyleDialogCA::OnEraseBkgnd(CDC* pDC)
{
  // Shadow frame
  pDC->FillSolidRect(CRect(m_windowRect.right - WINDOWSHADOWBORDER, m_windowRect.top, m_windowRect.right, m_windowRect.top + 2 * WINDOWSHADOWBORDER), ColorWindowTransparent);
  pDC->FillSolidRect(CRect(m_windowRect.right - WINDOWSHADOWBORDER, m_windowRect.top + 2 * WINDOWSHADOWBORDER, m_windowRect.right, m_windowRect.bottom), ColorWindowGrayFrame);
        ValidateRect(CRect(m_windowRect.right - WINDOWSHADOWBORDER, m_windowRect.top, m_windowRect.right, m_windowRect.bottom));
  pDC->FillSolidRect(CRect(m_windowRect.left, m_windowRect.bottom - WINDOWSHADOWBORDER, m_windowRect.left * 2 * WINDOWSHADOWBORDER, m_windowRect.bottom), ColorWindowTransparent);
  pDC->FillSolidRect(CRect(m_windowRect.left + 2 * WINDOWSHADOWBORDER, m_windowRect.bottom - WINDOWSHADOWBORDER, m_windowRect.right - WINDOWSHADOWBORDER, m_windowRect.bottom), ColorWindowGrayFrame);
        ValidateRect(CRect(m_windowRect.left, m_windowRect.bottom - WINDOWSHADOWBORDER, m_windowRect.right - WINDOWSHADOWBORDER, m_windowRect.bottom));

  if (m_caption)
  {
    // caption bar
    CRect caption(m_clientRect.left, m_clientRect.top, m_clientRect.right, m_clientRect.top + WINDOWCAPTIONHEIGHT);
    pDC->FillSolidRect(caption, m_error ? ColorWindowFrameError : ColorWindowHeader);
    ValidateRect(caption);

    // title
    pDC->SelectObject(&STYLEFONTS.DialogTextFontBold);
    pDC->SetTextColor(m_error ? ColorWindowFrameTextError : ColorWindowHeaderText);
    caption.right -= WINDOWCAPTIONHEIGHT;
    caption.DeflateRect(5, 0);
    CString titel;
    GetWindowText(titel);
    pDC->DrawText(titel, caption, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
    
    if (m_close)
    {
      // close button
      if (m_over)
      {
        CRect button(m_closeRect);
        button.DeflateRect(6, 6);
        ++button.right;
        ++button.bottom;
        pDC->FillSolidRect(button, m_over && m_down ? ThemeColor::GetColor(Colors::ColorControlPressed) 
                                                    : ThemeColor::GetColor(Colors::ColorControlHover));
        CBrush brush(m_over && m_down ? ThemeColor::GetColor(Colors::ColorControlFramePressed) 
                                      : ThemeColor::GetColor(Colors::ColorControlFrameHover));
        pDC->FrameRect(button, &brush);
      }

      CPen pen;
      pen.CreatePen(PS_SOLID, 1, m_over ? m_over && m_down ? ThemeColor::GetColor(Colors::ColorControlTextPressed) 
                                                           : ThemeColor::GetColor(Colors::ColorControlTextHover) 
                                                 : m_error ? ColorWindowFrameTextError 
                                                           : ThemeColor::GetColor(Colors::ColorEditText));
      HGDIOBJ orgpen = pDC->SelectObject(pen);

      CPoint center(m_closeRect.right - WINDOWCAPTIONHEIGHT / 2, m_closeRect.top + WINDOWCAPTIONHEIGHT / 2);
      pDC->MoveTo(center.x - WS(3), center.y - WS(3));
      pDC->LineTo(center.x + WS(4), center.y + WS(4));
      pDC->MoveTo(center.x + WS(3), center.y - WS(3));
      pDC->LineTo(center.x - WS(4), center.y + WS(4));
      pDC->SelectObject(orgpen);
    }
  }
  else
  {
    ValidateRect(CRect(m_clientRect.left, m_clientRect.top, m_clientRect.right, m_clientRect.top + 1));
  }
  // window frame
  CBrush brush(m_error ? ColorWindowFrameError : ThemeColor::GetColor(Colors::AccentColor1));
  pDC->FrameRect(m_clientRect, &brush);

  ValidateRect(CRect(m_clientRect.left, m_clientRect.top, m_clientRect.left+1, m_clientRect.bottom));
  ValidateRect(CRect(m_clientRect.right-1, m_clientRect.top, m_clientRect.right, m_clientRect.bottom));
  ValidateRect(CRect(m_clientRect.left, m_clientRect.bottom-1, m_clientRect.right, m_clientRect.bottom));

  // real client area
  pDC->FillSolidRect(m_properClientRect,ThemeColor::GetColor(Colors::ColorWindowFrame));

  return TRUE;
}

void 
StyleDialogCA::OnMouseMove(UINT nFlags, CPoint point)
{
  CDialogEx::OnMouseMove(nFlags, point);
  if (m_close)
  {
    bool over = m_closeRect.PtInRect(point) != 0;
    if (over != m_over)
    {
      m_over = over;
      InvalidateRect(m_closeRect);
    }
  }

  TRACKMOUSEEVENT mouseEvent;

  mouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
  mouseEvent.dwFlags = TME_HOVER | TME_LEAVE;
  mouseEvent.dwHoverTime = 1;
  mouseEvent.hwndTrack = m_hWnd;

  _TrackMouseEvent(&mouseEvent);
}

void
StyleDialogCA::OnLButtonDown(UINT nFlags, CPoint point)
{
  CDialogEx::OnLButtonDown(nFlags, point);

  if (!m_down)
  {
    if (m_close && m_closeRect.PtInRect(point))
    {
      InvalidateRect(m_closeRect);
      m_down = true;
    }
  }

  if (m_dragRect.PtInRect(point))
  {
    CPoint lastpoint;
    GetCursorPos(&lastpoint);

    while (LBUTTONDOWN)
    {
      CPoint cursor;
      GetCursorPos(&cursor);
      if (cursor != lastpoint)
      {
        SetWindowPos(nullptr, cursor.x - point.x, cursor.y - point.y, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);
        lastpoint = cursor;
      }
    }
  }
}

void
StyleDialogCA::OnLButtonUp(UINT nFlags, CPoint point)
{
  CDialogEx::OnLButtonUp(nFlags, point);

  if (m_down)
  {
    InvalidateRect(m_closeRect);

    if (m_over)
    {
      EndDialog(IDCANCEL);
    }
  }

  m_down = false;
}

LRESULT
StyleDialogCA::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
  if (m_close)
  {
    bool over = m_closeRect.PtInRect((CPoint)lParam) != 0;
    if (over != m_over)
    {
      m_over = over;
      InvalidateRect(m_closeRect);
    }
  }
  return true;
}

LRESULT
StyleDialogCA::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
  if (m_over)
  {
    InvalidateRect(m_closeRect);
  }

  m_over = false;
  return true;
}

HBRUSH StyleDialogCA::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
  switch (nCtlColor)
  {
    case CTLCOLOR_STATIC: if(m_error)
                          {
                            pDC->SetTextColor(ThemeColor::GetColor(Colors::ColorEditText));
                            pDC->SetBkColor  (ThemeColor::GetColor(Colors::ColorWindowFrame));
                          }
                          else
                          {
                            pDC->SetTextColor(ThemeColor::GetColor(Colors::ColorEditText));
                            pDC->SetBkColor  (ThemeColor::GetColor(Colors::ColorWindowFrame));
                          }
                          return m_defaultBrush;
    case CTLCOLOR_BTN:    if(m_error)
                          {
                            pDC->SetTextColor(ThemeColor::GetColor(Colors::ColorEditText));
                            pDC->SetBkColor  (ThemeColor::GetColor(Colors::ColorWindowFrame));
                          }
                          else
                          {
                            pDC->SetTextColor(ThemeColor::GetColor(Colors::ColorEditText));
                            pDC->SetBkColor  (ThemeColor::GetColor(Colors::ColorWindowFrame));
                          }
                          return m_defaultBrush;
  }
  return CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
}

void 
StyleDialogCA::SetWindowText(LPCTSTR lpstString)
{
  CDialogEx::SetWindowText(lpstString);
  InvalidateRect(m_dragRect);
}

void 
StyleDialogCA::OnSize(UINT nType, int cx, int cy)
{
  CDialogEx::OnSize(nType, cx, cy);
  
  m_windowRect.SetRect(0, 0, cx, cy);

  m_clientRect.SetRect(0, 0, cx - WINDOWSHADOWBORDER, cy - WINDOWSHADOWBORDER);
  if (m_caption)
  {
    m_closeRect.SetRect(cx - WINDOWSHADOWBORDER- WINDOWCAPTIONHEIGHT,0,cx - WINDOWSHADOWBORDER,                      WINDOWCAPTIONHEIGHT);
    m_dragRect .SetRect(0,                                           0,cx - WINDOWSHADOWBORDER - WINDOWCAPTIONHEIGHT,WINDOWCAPTIONHEIGHT);
    m_properClientRect.SetRect(1, WINDOWCAPTIONHEIGHT, cx - WINDOWSHADOWBORDER-1, cy - WINDOWSHADOWBORDER - 1);
  }
  else
  {
    m_closeRect.SetRect(0,0,0,0);
    m_dragRect .SetRect(0,0,0,0);
    m_properClientRect.SetRect(1, 1, cx - WINDOWSHADOWBORDER - 1, cy - WINDOWSHADOWBORDER - 1);
  }
}

void StyleDialogCA::GetClientRect(LPRECT lpRect)
{
  *lpRect = m_properClientRect;
}

LRESULT
StyleDialogCA::OnGrayScreen(WPARAM wParam, LPARAM lParam)
{
  LRESULT result = 1;

  if (wParam)
  {
    result = m_grayScreen.Show();
  }
  else
  {
    m_grayScreen.Hide();
  }
  // Return whether we succeeded
  return result;
}

void
StyleDialogCA::ReDrawFrame()
{
  // Does nothing
}

// Paint a system button on the NonClient area
void 
StyleDialogCA::Button(CDC* pDC, CRect rect, LRESULT type,BUTTONSTATE state, bool max)
{
  CPen pen;

  switch (state) 
  {
    case BS_NORMAL:
    case BS_DISABLED: pDC->FillSolidRect(rect, ThemeColor::GetColor(Colors::AccentColor1));
                      pen.CreatePen(PS_SOLID,1,ThemeColor::GetColor(Colors::AccentColor2));
                      break;
    case BS_HOVER:    pDC->FillSolidRect(rect, ThemeColor::GetColor(Colors::ColorControlHover));
                      pen.CreatePen(PS_SOLID,1,ThemeColor::GetColor(Colors::ColorControlTextHover));
                      break;
    case BS_PRESSED:  pDC->FillSolidRect(rect, ThemeColor::GetColor(Colors::ColorControlPressed));
                      pen.CreatePen(PS_SOLID,1,ThemeColor::GetColor(Colors::ColorControlTextPressed));
                      break;
    default:          assert(false);
                      return;
  }
  HGDIOBJ orgpen = pDC->SelectObject(pen);

  switch (type) 
  {
    case HTCLOSE:
                      pDC->MoveTo(rect.CenterPoint().x - WS(5), rect.CenterPoint().y - WS(5));
                      pDC->LineTo(rect.CenterPoint().x + WS(6), rect.CenterPoint().y + WS(6));
                      pDC->MoveTo(rect.CenterPoint().x + WS(5), rect.CenterPoint().y - WS(5));
                      pDC->LineTo(rect.CenterPoint().x - WS(6), rect.CenterPoint().y + WS(6));
                      break;

    case HTMINBUTTON:
                      pDC->MoveTo(rect.CenterPoint().x - WS(6), rect.CenterPoint().y + WS(6));
                      pDC->LineTo(rect.CenterPoint().x + WS(6), rect.CenterPoint().y + WS(6));
                      break;

    case HTMAXBUTTON:
                      if(max)
                      {
                        // window before
                        pDC->MoveTo(rect.CenterPoint().x - WS(6), rect.CenterPoint().y - WS(3));
                        pDC->LineTo(rect.CenterPoint().x + WS(4), rect.CenterPoint().y - WS(3));
                        pDC->LineTo(rect.CenterPoint().x + WS(4), rect.CenterPoint().y + WS(7));
                        pDC->LineTo(rect.CenterPoint().x - WS(6), rect.CenterPoint().y + WS(7));
                        pDC->LineTo(rect.CenterPoint().x - WS(6), rect.CenterPoint().y - WS(3));

                        // window behind
                        pDC->MoveTo(rect.CenterPoint().x - WS(3), rect.CenterPoint().y - WS(4));
                        pDC->LineTo(rect.CenterPoint().x - WS(3), rect.CenterPoint().y - WS(6));
                        pDC->LineTo(rect.CenterPoint().x + WS(7), rect.CenterPoint().y - WS(6));
                        pDC->LineTo(rect.CenterPoint().x + WS(7), rect.CenterPoint().y + WS(4));
                        pDC->LineTo(rect.CenterPoint().x + WS(4), rect.CenterPoint().y + WS(4));
                      }
                      else
                      {
                        // One window to maximize
                        pDC->MoveTo(rect.CenterPoint().x - WS(5), rect.CenterPoint().y - WS(5));
                        pDC->LineTo(rect.CenterPoint().x + WS(6), rect.CenterPoint().y - WS(5));
                        pDC->LineTo(rect.CenterPoint().x + WS(6), rect.CenterPoint().y + WS(6));
                        pDC->LineTo(rect.CenterPoint().x - WS(5), rect.CenterPoint().y + WS(6));
                        pDC->LineTo(rect.CenterPoint().x - WS(5), rect.CenterPoint().y - WS(5));
                      }
                      break;
  }
  pDC->SelectObject(orgpen);
}
