////////////////////////////////////////////////////////////////////////
//
// File: StyleFrameWnd.cpp
// Function: Main Window styling frame for MDI and SDI applications
//           Has a singular title bar with a system menu
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
#include "RegistryManager.h"
#include "resource.h"
#include <afxglobalutils.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace ThemeColor;

IMPLEMENT_DYNAMIC(StyleFrameWnd,CFrameWndEx)

StyleFrameWnd::StyleFrameWnd()
{
  m_dwMenuBarState = AFX_MBS_VISIBLE;

  m_menuTooltip.Create(this, TTS_ALWAYSTIP);
  LoadStyleTheme();
}

BEGIN_MESSAGE_MAP(StyleFrameWnd, CFrameWndEx)
  ON_WM_SIZE()
  ON_WM_NCCALCSIZE()
  ON_WM_ERASEBKGND()
  ON_WM_NCPAINT()
  ON_WM_NCHITTEST()
  ON_WM_NCLBUTTONDOWN()
  ON_WM_NCLBUTTONUP()
  ON_WM_NCMOUSEMOVE()
  ON_WM_NCLBUTTONDBLCLK()
  ON_WM_SETTINGCHANGE()
  ON_WM_NCACTIVATE()
  ON_MESSAGE(WM_GRAYSCREEN,OnGrayScreen)
  ON_MESSAGE(WM_NCMOUSELEAVE,OnNcMouseLeave)
  ON_REGISTERED_MESSAGE(ThemeColor::g_msg_changed,OnStyleChanged)
END_MESSAGE_MAP()

// Remove the title bar from the main window frame
BOOL
StyleFrameWnd::PreCreateWindow(CREATESTRUCT& cs)
{
  cs.style = WS_CAPTION | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
             WS_SIZEBOX | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_OVERLAPPED;
  cs.dwExStyle |= WS_EX_WINDOWEDGE | WS_EX_APPWINDOW;

  if (!CFrameWndEx::PreCreateWindow(cs))
  {
    return FALSE;
  }
  return TRUE;
}

int
StyleFrameWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  int msg_changed = RegisterWindowMessage(STYLEFXCHANGED);
  ThemeColor::g_msg_changed = msg_changed;

  m_grayScreen.CreateEx(0,AfxRegisterWndClass(0),_T(""),WS_POPUP,CRect(0,0,0,0),this,0);

  return CFrameWndEx::OnCreate(lpCreateStruct);
}

BOOL StyleFrameWnd::PreTranslateMessage(MSG* message)
{
  m_menuTooltip.RelayEvent(message);
  return CFrameWndEx::PreTranslateMessage(message);
}

// Process the menu items.
// For an example see: CTestFrameApp::PumpMessage() 
// In the StyleFrameWork project
//
BOOL StyleFrameWnd::PreTranslateMenu(MSG* p_msg)
{
  if(p_msg->message == WM_KEYDOWN)
  {
    if(m_menuFocus)
    {
      switch(p_msg->wParam)
      {
#ifdef _DEBUG
        case VK_F11:    // Visual Studio F12 is DebugBreak-UI !
#else
        case VK_F12:    // Fall through
#endif
        case VK_ESCAPE: m_menuFocus = false;
                        m_curhit = -1;
                        break;
        case VK_RETURN: // Fall through
        case VK_SPACE:  // Fall through
        case VK_DOWN:   PerformMenu();
                        // Fall through
        default:        m_menuFocus = false;
                        m_curhit = -1;
                        break;
      }
      OnNcPaint();
      return TRUE;
    }
#ifdef _DEBUG
    else if(p_msg->wParam == VK_F11)
#else
    else if(p_msg->wParam == VK_F12)
#endif
    {
      m_menuFocus = true;
      m_curhit = HTMENU;
      OnNcPaint();
      return TRUE;
    }
  }
  return FALSE;
}

BOOL
StyleFrameWnd::SetSysMenu(UINT p_menuResource)
{
  m_sysmenu = p_menuResource;
  if (m_menu.GetSafeHmenu())
  {
    m_menu.DestroyMenu();
  }
  return m_menu.LoadMenu(m_sysmenu);
}

void
StyleFrameWnd::SetTheme(ThemeColor::Themes p_theme)
{
  // Write new theme to the registry
  RegistryManager manager;
  manager.SetRegistryInteger(STYLECOLORS_KEY, STYLECOLORS_THEME, (int)p_theme);

  // Broadcast the change to all top level windows
  DWORD components = BSM_APPLICATIONS;
  BroadcastSystemMessage(BSF_POSTMESSAGE, &components,ThemeColor::g_msg_changed,0,0);

  // Change our colors
  ThemeColor::SetTheme(p_theme);

  // Now repaint ourselves and all of our children
  if(GetSafeHwnd())
  {
    RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ALLCHILDREN|RDW_FRAME);
  }
}

// Another program has changed our styling
LRESULT
StyleFrameWnd::OnStyleChanged(WPARAM /*wParam*/,LPARAM /*lParam*/)
{
  LoadStyleTheme();
  return 0; // AOK
}

void
StyleFrameWnd::LoadStyleTheme()
{
  RegistryManager manager;
  int th = manager.GetRegistryInteger(STYLECOLORS_KEY,STYLECOLORS_THEME,(int)ThemeColor::Themes::ThemeSkyblue);
  ThemeColor::Themes theme = (ThemeColor::Themes)th;
  ThemeColor::SetTheme(theme);

  // Now repaint ourselves and all of our children
  if(GetSafeHwnd())
  {
    RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ALLCHILDREN|RDW_FRAME);
  }
}

void
StyleFrameWnd::OnStyleLime()
{
  SetTheme(ThemeColor::Themes::ThemeLime);
}

void
StyleFrameWnd::OnStyleSkyblue()
{
  SetTheme(ThemeColor::Themes::ThemeSkyblue);
}

void
StyleFrameWnd::OnStylePurple()
{
  SetTheme(ThemeColor::Themes::ThemePurple);
}

void
StyleFrameWnd::OnStyleMustard()
{
  SetTheme(ThemeColor::Themes::ThemeMustard);
}

void
StyleFrameWnd::OnStyleModerateGray()
{
  SetTheme(ThemeColor::Themes::ThemeModerateGray);
}

void
StyleFrameWnd::OnStylePureGray()
{
  SetTheme(ThemeColor::Themes::ThemePureGray);
}

void
StyleFrameWnd::OnStyleBlackWhite()
{
  SetTheme(ThemeColor::Themes::ThemeBlackWhite);
}

void
StyleFrameWnd::OnStyleDark()
{
  SetTheme(ThemeColor::Themes::ThemeDark);
}

void
StyleFrameWnd::PerformMenu()
{
  if (m_menu.GetSafeHmenu())
  {
    CRect rect;
    GetClientRect(rect);
    ClientToScreen(rect);

    CMenu* popup = m_menu.GetSubMenu(0);

    int flags = TPM_LEFTALIGN | TPM_RETURNCMD;
    int num = TrackPopupMenu(popup->GetSafeHmenu(), flags, rect.left, rect.top, 0, GetSafeHwnd(), nullptr);
    if (num > 0)
    {
      PostMessage(WM_COMMAND, num, 0);
    }
  }
}

// Override erasing the background
afx_msg BOOL
StyleFrameWnd::OnEraseBkgnd(CDC* pdc)
{
  // No background erase
  return TRUE;
}

LRESULT
StyleFrameWnd::OnGrayScreen(WPARAM wParam, LPARAM lParam)
{
  LRESULT result = 1;

  if(wParam)
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
StyleFrameWnd::OnNcMouseMove(UINT nFlags, CPoint point)
{
  CFrameWndEx::OnNcMouseMove(nFlags, point);
  

  if (m_curhit != (LRESULT)nFlags)
  {
    UINT prev = (UINT) m_curhit;
    m_curhit = nFlags;
    ReDrawButton(prev);
    ReDrawButton(m_curhit);
  }

  TRACKMOUSEEVENT mouseEvent;

  mouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
  mouseEvent.dwFlags = TME_LEAVE | TME_NONCLIENT;
  mouseEvent.hwndTrack = m_hWnd;
  mouseEvent.dwHoverTime = 500;

  _TrackMouseEvent(&mouseEvent);
}

void
StyleFrameWnd::OnNcLButtonDblClk(UINT nFlags, CPoint point)
{
  if (nFlags == HTCAPTION && (GetStyle() & WS_MAXIMIZE) == 0)
  {
    ShowWindow(SW_MAXIMIZE);
  }
  else
  {
    CFrameWndEx::OnNcLButtonDblClk(nFlags, point);
  }
}

void
StyleFrameWnd::OnNcLButtonDown(UINT nFlags, CPoint point)
{
  switch (nFlags) 
  {
    case HTCLOSE:
    case HTMINBUTTON:
    case HTMAXBUTTON:
    {
      m_curhit = nFlags;
      HRESULT lasthit = HTNOWHERE;
      CPoint lastpoint(0, 0);
      m_down = false;

      while (LBUTTONDOWN)
      {
        CPoint cursor;
        GetCursorPos(&cursor);
        if (cursor != lastpoint)
        {
          lastpoint = cursor;
          HRESULT hit = (HRESULT)OnNcHitTest(cursor);
          if (hit != lasthit)
          {
            lasthit = hit;
            m_down = (lasthit == m_curhit);

            // Redraw the button in the right state
            LRESULT curr = m_curhit;
            m_curhit = hit;
            ReDrawButton(nFlags);
            m_curhit = curr;
          }
        }
      }
      m_down = false;
      m_curhit = OnNcHitTest(lastpoint);
      ReDrawButton(nFlags);

      if (m_curhit == (LRESULT)nFlags)
      {
        switch (m_curhit) 
        {
          case HTMENU:        PerformMenu();
                              break;
          case HTCLOSE:       PostMessage(WM_CLOSE,0,0);
                              break;
          case HTMINBUTTON:   ShowWindow(SW_MINIMIZE);
                              break;
          case HTMAXBUTTON:   if ((GetStyle() & WS_MAXIMIZE) != 0)
                              {
                                ShowWindow(SW_SHOWNORMAL);
                              }
                              else
                              {
                                ShowWindow(SW_MAXIMIZE);
                              }
                              break;
        }
      }
    }
    break;
    default:      CFrameWndEx::OnNcLButtonDown(nFlags, point);
                  break;
    case HTMENU:  PerformMenu();
                  break;
    case HTCAPTION:
                  if(GetStyle() & WS_MAXIMIZE)
                  {
                    return;
                  }
                  SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW | SWP_NOSENDCHANGING | SWP_DRAWFRAME);

                  CPoint lastpoint(point);
                  CRect window;
                  GetWindowRect(window);
                  point.x -= window.left;
                  point.y -= window.top;

                  while (LBUTTONDOWN)
                  {
                    CPoint cursor;
                    GetCursorPos(&cursor);
                    if(cursor != lastpoint)
                    {
                      SetWindowPos(NULL,cursor.x - point.x,cursor.y - point.y,0,0,SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOSIZE);
                      lastpoint = cursor;
                    }
                  }
                  break;
  }
}

void
StyleFrameWnd::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
  m_down = false;
  ReDrawButton(m_curhit);
  m_curhit = nHitTest;

  CFrameWndEx::OnNcLButtonUp(nHitTest, point);
}

LRESULT
StyleFrameWnd::OnNcMouseLeave(WPARAM wParam, LPARAM lParam)
{
  if (m_curhit != HTNOWHERE)
  {
    UINT prev = (UINT)m_curhit;
    m_curhit  = HTNOWHERE;
    m_down    = false;
    ReDrawButton(prev);
  }
  return true;
}

void 
StyleFrameWnd::SetWindowText(LPCTSTR lpstString)
{
  CFrameWndEx::SetWindowText(lpstString);
  RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_FRAME|RDW_ALLCHILDREN);
}

void
StyleFrameWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
  if((uFlags == SPI_SETWORKAREA) && ((GetStyle() & WS_MAXIMIZE) == WS_MAXIMIZE))
  {
    ShowWindow(SW_RESTORE);
    ShowWindow(SW_MAXIMIZE);
  }
}

void StyleFrameWnd::OnSize(UINT nType, int cx, int cy)
{
  CFrameWndEx::OnSize(nType, cx, cy);
  
  if (nType != SIZE_MINIMIZED)
  {
    // Leave the size-box, otherwise a maximized window will not react on 
    // arrow keys. And a window without a border cannot be resized
    switch (nType) 
    {
      case SIZE_MAXIMIZED:  if((GetStyle() & WS_SIZEBOX) != 0 && !m_dtsize)
                            {
                                m_dtsize = true;
                                RepositionFrameWnd(this);
                            }
                            break;
      case SIZE_RESTORED:   if ((GetStyle() & WS_SIZEBOX) == 0)
                            {
                                ModifyStyle(0, WS_SIZEBOX);
                            }
                            if(m_dtsize)
                            {
                              MinimalFrameWnd(this);
                              m_dtsize = false;
                            }
                            break;
      default:              m_dtsize = false;
                            break;
    }

    GetWindowRect(m_windowRectLocal);
    m_windowRectLocal.OffsetRect(-m_windowRectLocal.left, -m_windowRectLocal.top);

    int border = 0;
    if ((GetStyle() & WS_MAXIMIZE) != 0)
    {
      // Bij een volledig scherm gebruikt het OS deze marge buiten beeld!!
      CSize marge = afxGlobalUtils.GetSystemBorders(GetStyle());

      m_windowRectLocal.left   += marge.cx;
      m_windowRectLocal.top    += marge.cy;
      m_windowRectLocal.right  -= marge.cx;
      m_windowRectLocal.bottom -= marge.cy;
    }
    else
    {
      border = MARGIN;
    }

    m_closeRect.SetRect(m_windowRectLocal.right - border -     WINCAPTIONHEIGHT, m_windowRectLocal.top, m_windowRectLocal.right - border,                        m_windowRectLocal.top + WINCAPTIONHEIGHT);
    m_maxRect  .SetRect(m_windowRectLocal.right - border - 2 * WINCAPTIONHEIGHT, m_windowRectLocal.top, m_windowRectLocal.right - border -     WINCAPTIONHEIGHT, m_windowRectLocal.top + WINCAPTIONHEIGHT);
    m_minRect  .SetRect(m_windowRectLocal.right - border - 3 * WINCAPTIONHEIGHT, m_windowRectLocal.top, m_windowRectLocal.right - border - 2 * WINCAPTIONHEIGHT, m_windowRectLocal.top + WINCAPTIONHEIGHT);
    m_sysRect  .SetRect(m_windowRectLocal.left  + border,                        m_windowRectLocal.top, m_windowRectLocal.left  + border +     WINCAPTIONHEIGHT, m_windowRectLocal.top + WINCAPTIONHEIGHT);

    m_dragRect   .SetRect(m_windowRectLocal.left, m_windowRectLocal.top,m_windowRectLocal.right - border - 3 * WINCAPTIONHEIGHT, m_windowRectLocal.top + WINCAPTIONHEIGHT);
    m_captionRect.SetRect(m_sysRect.right,m_dragRect.top, m_minRect.left, m_dragRect.bottom);

    RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_FRAME|RDW_ALLCHILDREN);

    m_menuTooltip.Activate(FALSE);
  }
  else
  {
    m_dtsize = false;
  }
}

void 
StyleFrameWnd::OnNcCalcSize(BOOL calcValidRects,NCCALCSIZE_PARAMS* p_params)
{
  int marge = MARGIN;
  p_params->rgrc[0].top += WINCAPTIONHEIGHT;

  if(GetStyle() & WS_MAXIMIZE)
  {
    // Bij een volledig scherm gebruikt het OS deze marge buiten beeld!!
    CSize marge = afxGlobalUtils.GetSystemBorders(GetStyle());

    p_params->rgrc[0].left   += marge.cx;
    p_params->rgrc[0].top    += marge.cy;
    p_params->rgrc[0].right  -= marge.cx;
    p_params->rgrc[0].bottom -= marge.cy;
  }
  else
  {
    // Marge is nodig voor twee zaken:
    // 1) Om de linker/rechter/onder muis-uitrek actie te activeren
    // 2) Om ruimte te maken voor het kader rondom het venster
    p_params->rgrc[0].left   += marge;
    p_params->rgrc[0].right  -= marge;
    p_params->rgrc[0].bottom -= marge;
  }
  // Use same rectangle for displacement (so hide it)
  p_params->rgrc[2] = p_params->rgrc[0];
}

// Avoid flicker of the titlebar on activate
BOOL
StyleFrameWnd::OnNcActivate(BOOL bActive)
{
  if (bActive)
  {
    OnNcPaint();
  }
  return TRUE;
}

void 
StyleFrameWnd::OnNcPaint()
{
  CWindowDC dc(this);

  // MARGE
  
  // BORDER around the window frame if not maximized
  if ((GetStyle() & WS_MAXIMIZE) == 0)
  {
    CRect r;
    COLORREF bkgnd = ThemeColor::GetColor(Colors::AccentColor1);
    int width = MARGIN;

    r.SetRect(m_windowRectLocal.left,         m_windowRectLocal.top,           m_windowRectLocal.right,       m_windowRectLocal.top + width);
    dc.FillSolidRect(r, bkgnd);
    r.SetRect(m_windowRectLocal.right - width,m_windowRectLocal.top,           m_windowRectLocal.right,       m_windowRectLocal.bottom);
    dc.FillSolidRect(r, bkgnd);
    r.SetRect(m_windowRectLocal.left,         m_windowRectLocal.bottom - width,m_windowRectLocal.right,       m_windowRectLocal.bottom);
    dc.FillSolidRect(r, bkgnd);
    r.SetRect(m_windowRectLocal.left,         m_windowRectLocal.top,           m_windowRectLocal.left + width,m_windowRectLocal.bottom);
    dc.FillSolidRect(r, bkgnd);
  }
  
  // caption bar
  dc.FillSolidRect(m_dragRect,ThemeColor::GetColor(Colors::AccentColor1));

  // title
  CRect titleRect(m_captionRect);
  titleRect.left += WINCAPTIONHEIGHT / 3;
  CFont* orgfont = dc.SelectObject(&STYLEFONTS.CaptionTextFont);
  dc.SetTextColor(ColorWindowHeaderText);
  CString titel;
  GetWindowText(titel);
  dc.DrawText(titel,titleRect,DT_SINGLELINE|DT_LEFT|DT_VCENTER|DT_EXPANDTABS|DT_NOPREFIX);
  dc.SelectObject(orgfont);

  // System menu
  DrawButton(&dc, m_sysRect,   HTMENU);
  // minimize button
  DrawButton(&dc, m_minRect,   HTMINBUTTON);
  // maximize button
  DrawButton(&dc, m_maxRect,   HTMAXBUTTON);
  // close button
  DrawButton(&dc, m_closeRect, HTCLOSE);
}

LRESULT StyleFrameWnd::OnNcHitTest(CPoint point)
{
  CRect window;
  GetWindowRect(window);
  point.Offset(-window.TopLeft());

  if(m_closeRect.PtInRect(point))
  {
    return HTCLOSE;
  }
  if(m_minRect.PtInRect(point))
  {
    return HTMINBUTTON;
  }
  if(m_maxRect.PtInRect(point))
  {
    return HTMAXBUTTON;
  }
  if(m_sysRect.PtInRect(point))
  {
    return HTMENU;
  }

  // BORDERS

  if (!(GetStyle() & WS_MAXIMIZE))
  {
    window.OffsetRect(-window.left, -window.top);
    if (point.x <= window.left + SIZEMARGIN)
    {
      if (point.y >= window.bottom - SIZEMARGIN)
      {
        return HTBOTTOMLEFT;
      }
      if (point.y <= window.top + SIZEMARGIN)
      {
        return HTTOPLEFT;
      }
      return HTLEFT;
    }
    if (point.x >= window.right - SIZEMARGIN)
    {
      if (point.y >= window.bottom - SIZEMARGIN)
      {
        return HTBOTTOMRIGHT;
      }
      if (point.y <= window.top + SIZEMARGIN)
      {
        return HTTOPRIGHT;
      }
      return HTRIGHT;
    }
    if (point.y >= window.bottom - SIZEMARGIN)
    {
      return HTBOTTOM;
    }
    if (point.y <= window.top + SIZEMARGIN)
    {
      return HTTOP;
    }
  }
  if (m_captionRect.PtInRect(point))
  {
    return HTCAPTION;
  }

  CRect client;
  GetClientRect(&client);
  if (PtInRect(client, point))
  {
    return HTCLIENT;
  }
  return HTNOWHERE;
}

void StyleFrameWnd::ReDrawButton(LRESULT type)
{
  CWindowDC dc(this);

  switch (type) 
  {
    case HTMENU:      DrawButton(&dc, m_sysRect,  type); break;
    case HTCLOSE:     DrawButton(&dc, m_closeRect,type); break;
    case HTMINBUTTON: DrawButton(&dc, m_minRect,  type); break;
    case HTMAXBUTTON: DrawButton(&dc, m_maxRect,  type); break;
  }
}

void StyleFrameWnd::DrawButton(CDC* pDC, CRect rect, LRESULT type)
{
  pDC->FillSolidRect(rect,ThemeColor::GetColor(Colors::AccentColor1));
  if (m_curhit == type)
  {
    pDC->FillSolidRect(rect, m_down ? ThemeColor::GetColor(Colors::ColorControlPressed) 
                                    : ThemeColor::GetColor(Colors::ColorControlHover));
  }
  CPen pen;
  pen.CreatePen(PS_SOLID, 1, m_curhit == type ? m_down ? ThemeColor::GetColor(Colors::ColorControlTextPressed) 
                                                       : ThemeColor::GetColor(Colors::ColorControlTextHover) 
                                                       : ColorWindowHeaderIcon);
  HGDIOBJ orgpen = pDC->SelectObject(pen);

  switch(type) 
  {
    case HTMENU:      pDC->MoveTo(rect.CenterPoint().x - WS(7),rect.CenterPoint().y - WS(5));
                      pDC->LineTo(rect.CenterPoint().x + WS(7),rect.CenterPoint().y - WS(5));
                      pDC->MoveTo(rect.CenterPoint().x - WS(7),rect.CenterPoint().y);
                      pDC->LineTo(rect.CenterPoint().x + WS(7),rect.CenterPoint().y);
                      pDC->MoveTo(rect.CenterPoint().x - WS(7),rect.CenterPoint().y + WS(5));
                      pDC->LineTo(rect.CenterPoint().x + WS(7),rect.CenterPoint().y + WS(5));
                      break;
    case HTCLOSE:     pDC->MoveTo(rect.CenterPoint().x - WS(5), rect.CenterPoint().y - WS(5));
                      pDC->LineTo(rect.CenterPoint().x + WS(6), rect.CenterPoint().y + WS(6));
                      pDC->MoveTo(rect.CenterPoint().x + WS(5), rect.CenterPoint().y - WS(5));
                      pDC->LineTo(rect.CenterPoint().x - WS(6), rect.CenterPoint().y + WS(6));
                      break;
    case HTMINBUTTON: pDC->MoveTo(rect.CenterPoint().x - WS(6), rect.CenterPoint().y + WS(6));
                      pDC->LineTo(rect.CenterPoint().x + WS(6), rect.CenterPoint().y + WS(6));
                      break;
    case HTMAXBUTTON: if ((GetStyle() & WS_MAXIMIZE) != 0)
                      {
                        // 'Front' window
                        pDC->MoveTo(rect.CenterPoint().x - WS(6), rect.CenterPoint().y - WS(3));
                        pDC->LineTo(rect.CenterPoint().x + WS(4), rect.CenterPoint().y - WS(3));
                        pDC->LineTo(rect.CenterPoint().x + WS(4), rect.CenterPoint().y + WS(7));
                        pDC->LineTo(rect.CenterPoint().x - WS(6), rect.CenterPoint().y + WS(7));
                        pDC->LineTo(rect.CenterPoint().x - WS(6), rect.CenterPoint().y - WS(3));

                        // 'Behind' window
                        pDC->MoveTo(rect.CenterPoint().x - WS(3), rect.CenterPoint().y - WS(4));
                        pDC->LineTo(rect.CenterPoint().x - WS(3), rect.CenterPoint().y - WS(6));
                        pDC->LineTo(rect.CenterPoint().x + WS(7), rect.CenterPoint().y - WS(6));
                        pDC->LineTo(rect.CenterPoint().x + WS(7), rect.CenterPoint().y + WS(4));
                        pDC->LineTo(rect.CenterPoint().x + WS(4), rect.CenterPoint().y + WS(4));
                      }
                      else
                      {
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
