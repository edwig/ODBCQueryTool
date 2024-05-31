////////////////////////////////////////////////////////////////////////
//
// File: StyleFrameWndEx.cpp
// Function: Main Window styling frame for MDI and SDI applications
//           Has a title bar with six distinct menu buttons
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

IMPLEMENT_DYNAMIC(StyleFrameWndEx,CFrameWndEx)

StyleFrameWndEx::StyleFrameWndEx()
{
  m_dwMenuBarState = AFX_MBS_HIDDEN;
  m_menuTooltip.Create(this, TTS_ALWAYSTIP);
  memset(&m_menuicon,0,sizeof(UINT));

  LoadStyleTheme();
}

BEGIN_MESSAGE_MAP(StyleFrameWndEx, CFrameWndEx)
  ON_WM_SIZE()
  ON_WM_NCCALCSIZE()
  ON_WM_ERASEBKGND()
  ON_WM_NCPAINT()
  ON_WM_NCHITTEST()
  ON_WM_NCMOUSEMOVE()
  ON_WM_NCMOUSELEAVE()
  ON_WM_NCLBUTTONDOWN()
  ON_WM_NCLBUTTONUP()
  ON_WM_NCLBUTTONDBLCLK()
  ON_WM_SETTINGCHANGE()
  ON_WM_NCACTIVATE()
  ON_MESSAGE(WM_GRAYSCREEN,OnGrayScreen)
  ON_REGISTERED_MESSAGE(g_msg_changed,OnStyleChanged)
END_MESSAGE_MAP()

// Remove the title bar from the main window frame
BOOL
StyleFrameWndEx::PreCreateWindow(CREATESTRUCT& cs)
{
  cs.style = WS_CAPTION | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | 
             WS_SIZEBOX | WS_MINIMIZEBOX  | WS_MAXIMIZEBOX  | WS_OVERLAPPED;
  cs.dwExStyle |= WS_EX_WINDOWEDGE | WS_EX_APPWINDOW;

  if (!CFrameWndEx::PreCreateWindow(cs))
  {
    return FALSE;
  }
  return TRUE;
}

int
StyleFrameWndEx::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  int msg_changed = RegisterWindowMessage(STYLEFXCHANGED);
  g_msg_changed   = msg_changed;

  m_grayScreen.CreateEx(0,AfxRegisterWndClass(0),_T(""),WS_POPUP,CRect(0,0,0,0),this,0);

  return CFrameWndEx::OnCreate(lpCreateStruct);
}

BOOL StyleFrameWndEx::PreTranslateMessage(MSG* p_msg)
{
  m_menuTooltip.RelayEvent(p_msg);
  return CFrameWndEx::PreTranslateMessage(p_msg);
}

// Process the menu items.
// For an example see: CTestFrameApp::PumpMessage() 
// In the StyleFrameWork project
//
BOOL StyleFrameWndEx::PreTranslateMenu(MSG* p_msg)
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
                        m_selectedMenu = -1;
                        break;
        case VK_LEFT:   if(--m_selectedMenu < 0)
                        {
                          m_selectedMenu = MENUCOUNT - 1;
                        }
                        break;
        case VK_RIGHT:  if(++m_selectedMenu == MENUCOUNT)
                        {
                          m_selectedMenu = 0;
                        }
                        break;
        case VK_RETURN: // Fall through
        case VK_SPACE:  // Fall through
        case VK_DOWN:   { CRect window;
                          GetWindowRect(window);
                          CPoint point = m_iconRects[m_selectedMenu].CenterPoint();
                          point.Offset(window.TopLeft());
                          MenuFromPoint(point);
                          m_selectedMenu = -1;
                          m_menuFocus = false;
                        }
                        OnNcPaint();
                        // Return/space are handled. Use Down to set first menu item!
                        return (p_msg->wParam != VK_DOWN);
        default:        m_menuFocus = false;
                        m_selectedMenu = -1;
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
      m_selectedMenu = 0;
      OnNcPaint();
      return TRUE;
    }
  }
  return FALSE;
}

void
StyleFrameWndEx::SetTheme(ThemeColor::Themes p_theme)
{
  // Write new theme to the registry
  RegistryManager manager;
  manager.SetRegistryInteger(STYLECOLORS_KEY, STYLECOLORS_THEME, (int)p_theme);

  // Broadcast the change to all top level windows
  DWORD components = BSM_APPLICATIONS;
  BroadcastSystemMessage(BSF_POSTMESSAGE, &components,g_msg_changed,0,0);

  // Change our colors
  ThemeColor::SetTheme(p_theme);

  // Now repaint ourselves and all of our children
  if(GetSafeHwnd())
  {
    RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_FRAME|RDW_ALLCHILDREN);
  }
}

// Another program has changed our styling
LRESULT
StyleFrameWndEx::OnStyleChanged(WPARAM /*wParam*/,LPARAM /*lParam*/)
{
  LoadStyleTheme();
  return 0; // AOK
}

void
StyleFrameWndEx::LoadStyleTheme()
{
  RegistryManager manager;
  int th = manager.GetRegistryInteger(STYLECOLORS_KEY,STYLECOLORS_THEME,(int)ThemeColor::Themes::ThemeSkyblue);
  ThemeColor::Themes theme = (ThemeColor::Themes)th;
  ThemeColor::SetTheme(theme);

  // Now repaint ourselves and all of our children
  if(GetSafeHwnd())
  {
    RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_FRAME|RDW_ALLCHILDREN);
  }
}

void
StyleFrameWndEx::OnStyleLime()
{
  SetTheme(ThemeColor::Themes::ThemeLime);
}

void
StyleFrameWndEx::OnStyleSkyblue()
{
  SetTheme(ThemeColor::Themes::ThemeSkyblue);
}

void
StyleFrameWndEx::OnStylePurple()
{
  SetTheme(ThemeColor::Themes::ThemePurple);
}

void
StyleFrameWndEx::OnStyleMustard()
{
  SetTheme(ThemeColor::Themes::ThemeMustard);
}

void
StyleFrameWndEx::OnStyleModerateGray()
{
  SetTheme(ThemeColor::Themes::ThemeModerateGray);
}

void
StyleFrameWndEx::OnStylePureGray()
{
  SetTheme(ThemeColor::Themes::ThemePureGray);
}

void
StyleFrameWndEx::OnStyleBlackWhite()
{
  SetTheme(ThemeColor::Themes::ThemeBlackWhite);
}

void
StyleFrameWndEx::OnStyleDark()
{
  SetTheme(ThemeColor::Themes::ThemeDark);
}

// Setting the resource and tooltip for a menu entry
void 
StyleFrameWndEx::SetMenuResource(int p_menu,UINT p_resource,CString p_tooltip /*=""*/)
{
  if (p_menu >= 0 && p_menu < MENUCOUNT)
  {
    m_menuicon[p_menu] = p_resource;
    m_tooltips[p_menu] = p_tooltip;
  }
}

// Setting the number of pixels for a menu item
void 
StyleFrameWndEx::SetMenuItemWidth(int p_width)
{
  if(p_width >= MENUITEMWIDTH_MIN && p_width <= MENUITEMWIDTH_MAX)
  {
    m_menuItemWidth = p_width;
  }
  RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_FRAME|RDW_ALLCHILDREN);
}

// Override erasing the background
afx_msg BOOL
StyleFrameWndEx::OnEraseBkgnd(CDC* pdc)
{
  // No background erase
  return TRUE;
}

LRESULT
StyleFrameWndEx::OnGrayScreen(WPARAM wParam, LPARAM lParam)
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
StyleFrameWndEx::OnNcMouseMove(UINT nFlags, CPoint point)
{
  CFrameWndEx::OnNcMouseMove(nFlags, point);
  
  if (m_curhit == HTMENU || nFlags == HTMENU)
  {
    CRect window;
    GetWindowRect(window);
    point.Offset(-window.TopLeft());

    int curmenu = m_selectedMenu;
    m_selectedMenu = MenuHitTest(point);

    if (m_selectedMenu != curmenu)
    {
      ReDrawIcon(curmenu);
      ReDrawIcon(m_selectedMenu);
    }
  }

  if (m_curhit != (LRESULT)nFlags)
  {
    UINT prev = (UINT) m_curhit;
    m_curhit = nFlags;
    ReDrawButton(prev);
    ReDrawButton(m_curhit);
  }

  TRACKMOUSEEVENT mouseEvent;

  mouseEvent.cbSize      = sizeof(TRACKMOUSEEVENT);
  mouseEvent.dwFlags     = TME_LEAVE | TME_NONCLIENT;
  mouseEvent.hwndTrack   = m_hWnd;
  mouseEvent.dwHoverTime = 200;

  _TrackMouseEvent(&mouseEvent);
}

void
StyleFrameWndEx::OnNcLButtonDblClk(UINT nFlags, CPoint point)
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
StyleFrameWndEx::OnNcLButtonDown(UINT nFlags, CPoint point)
{
  switch (nFlags) 
  {
    case HTCLOSE:
    case HTMINBUTTON:
    case HTMAXBUTTON:
    {
      m_curhit = nFlags;
      LRESULT lasthit = m_curhit;  // A: Where we are
      CPoint  lastpoint(0, 0);

      m_down = true; // B: Left button is DOWN
      while (LBUTTONDOWN)
      {
        CPoint cursor;
        GetCursorPos(&cursor);
        if (cursor != lastpoint)
        {
          lastpoint = cursor;  // C: Moved from the end of scope
          HRESULT hit = (HRESULT)OnNcHitTest(cursor);
          if (hit != lasthit)
          {
            lasthit = hit;
            m_down  = (lasthit == m_curhit);

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
          case HTMENU:        MenuFromPoint(point);
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
    case HTMENU:  MenuFromPoint(point);
                  break;
    case HTCAPTION:
                  if(GetStyle() & WS_MAXIMIZE)
                  {
                    return;
                  }
                  SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW|SWP_NOSENDCHANGING|SWP_DRAWFRAME);

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
StyleFrameWndEx::OnNcLButtonUp(UINT nHitTest,CPoint point)
{
  m_down = false;
  ReDrawButton(m_curhit);
  m_curhit = nHitTest;

  CFrameWndEx::OnNcLButtonUp(nHitTest,point);
}

void
StyleFrameWndEx::OnNcMouseLeave()
{
  if(m_curhit != HTNOWHERE)
  {
    if (m_curhit == HTMENU)
    {
      int menu = m_selectedMenu;
      m_selectedMenu = -1;
      ReDrawIcon(menu);
    }
    UINT prev = (UINT)m_curhit;
    m_curhit = HTNOWHERE;
    m_down = false;

    ReDrawButton(prev);
  }
}

void 
StyleFrameWndEx::SetWindowText(LPCTSTR lpstString)
{
  CFrameWndEx::SetWindowText(lpstString);
  RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_FRAME|RDW_ALLCHILDREN);
}

void
StyleFrameWndEx::OnSettingChange(UINT uFlags,LPCTSTR lpszSection)
{
  if((uFlags == SPI_SETWORKAREA) && ((GetStyle() & WS_MAXIMIZE) == WS_MAXIMIZE))
  {
    ShowWindow(SW_RESTORE);
    ShowWindow(SW_MAXIMIZE);
  }
}

void StyleFrameWndEx::OnSize(UINT nType, int cx, int cy)
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
      // On a full screen the OS uses this margin outside of view
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

    m_closeRect.SetRect(m_windowRectLocal.right - border -     CAPTIONHEIGHT, m_windowRectLocal.top, m_windowRectLocal.right - border,                     m_windowRectLocal.top + CAPTIONHEIGHT);
    m_maxRect  .SetRect(m_windowRectLocal.right - border - 2 * CAPTIONHEIGHT, m_windowRectLocal.top, m_windowRectLocal.right - border -     CAPTIONHEIGHT, m_windowRectLocal.top + CAPTIONHEIGHT);
    m_minRect  .SetRect(m_windowRectLocal.right - border - 3 * CAPTIONHEIGHT, m_windowRectLocal.top, m_windowRectLocal.right - border - 2 * CAPTIONHEIGHT, m_windowRectLocal.top + CAPTIONHEIGHT);

    m_dragRect.SetRect(m_windowRectLocal.left, m_windowRectLocal.top,m_windowRectLocal.right - border - 3 * CAPTIONHEIGHT, m_windowRectLocal.top + CAPTIONHEIGHT);
    m_menuRect.SetRect(m_windowRectLocal.left, m_windowRectLocal.top,m_windowRectLocal.right - border,                     m_windowRectLocal.top + CAPTIONHEIGHT);

    m_menuRects[0].SetRect(m_menuRect.left,                        m_menuRect.top, m_menuRect.left  +     m_menuItemWidth, m_menuRect.bottom);
    m_menuRects[1].SetRect(m_menuRect.left  +     m_menuItemWidth, m_menuRect.top, m_menuRect.left  + 2 * m_menuItemWidth, m_menuRect.bottom);
    m_menuRects[2].SetRect(m_menuRect.left  + 2 * m_menuItemWidth, m_menuRect.top, m_menuRect.left  + 3 * m_menuItemWidth, m_menuRect.bottom);
    m_menuRects[3].SetRect(m_dragRect.right - 3 * m_menuItemWidth, m_menuRect.top, m_dragRect.right - 2 * m_menuItemWidth, m_menuRect.bottom);
    m_menuRects[4].SetRect(m_dragRect.right - 2 * m_menuItemWidth, m_menuRect.top, m_dragRect.right -     m_menuItemWidth, m_menuRect.bottom);
    m_menuRects[5].SetRect(m_dragRect.right -     m_menuItemWidth, m_menuRect.top, m_dragRect.right,                       m_menuRect.bottom);

    m_iconRects[0].SetRect(m_menuRects[0].left + (m_menuItemWidth - CAPTIONHEIGHT) / 2, m_dragRect.top, m_menuRects[0].left + (m_menuItemWidth - CAPTIONHEIGHT) / 2 + CAPTIONHEIGHT, m_dragRect.bottom);
    m_iconRects[1].SetRect(m_menuRects[1].left + (m_menuItemWidth - CAPTIONHEIGHT) / 2, m_dragRect.top, m_menuRects[1].left + (m_menuItemWidth - CAPTIONHEIGHT) / 2 + CAPTIONHEIGHT, m_dragRect.bottom);
    m_iconRects[2].SetRect(m_menuRects[2].left + (m_menuItemWidth - CAPTIONHEIGHT) / 2, m_dragRect.top, m_menuRects[2].left + (m_menuItemWidth - CAPTIONHEIGHT) / 2 + CAPTIONHEIGHT, m_dragRect.bottom);
    m_iconRects[3].SetRect(m_menuRects[3].left + (m_menuItemWidth - CAPTIONHEIGHT) / 2, m_dragRect.top, m_menuRects[3].left + (m_menuItemWidth - CAPTIONHEIGHT) / 2 + CAPTIONHEIGHT, m_dragRect.bottom);
    m_iconRects[4].SetRect(m_menuRects[4].left + (m_menuItemWidth - CAPTIONHEIGHT) / 2, m_dragRect.top, m_menuRects[4].left + (m_menuItemWidth - CAPTIONHEIGHT) / 2 + CAPTIONHEIGHT, m_dragRect.bottom);
    m_iconRects[5].SetRect(m_menuRects[5].left + (m_menuItemWidth - CAPTIONHEIGHT) / 2, m_dragRect.top, m_menuRects[5].left + (m_menuItemWidth - CAPTIONHEIGHT) / 2 + CAPTIONHEIGHT, m_dragRect.bottom);

    m_captionRect.SetRect(m_iconRects[2].right, m_dragRect.top, m_iconRects[3].left, m_dragRect.bottom);

    RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_FRAME|RDW_ALLCHILDREN);

    m_menuTooltip.Activate(FALSE);
    for (int menu = 1;menu <= MENUCOUNT; ++menu)
    {
      m_menuTooltip.DelTool(this,menu);
    }

    CRect rect;
    CRect client;
    GetClientRect(client);
    CSize offset(-m_menuRect.left, client.Height() - m_windowRectLocal.Height());

    for(int ind = 0;ind < MENUCOUNT;++ind)
    {
      rect = m_iconRects[ind];
      rect.OffsetRect(offset);
      m_menuTooltip.AddTool(this,m_tooltips[ind],rect,ind + 1);
    }
    m_menuTooltip.Activate(TRUE);
  }
  else
  {
    m_dtsize = false;
  }
}

// Perform a sub-menu on the bar
void
StyleFrameWndEx::MenuFromPoint(CPoint p_point)
{
  CRect window;
  GetWindowRect(window);
  CPoint localpoint(p_point);
  localpoint.Offset(-window.TopLeft());

  for (int i = 0; i < MENUCOUNT; ++i)
  {
    if(m_iconRects[i].PtInRect(localpoint))
    {
      CMenu menu;
      menu.LoadMenu(IDR_MAINFRAME);
      CMenu* sub = menu.GetSubMenu(i);

      CMFCPopupMenu* popup = new CMFCPopupMenu();
      if(!popup->Create(this,p_point.x,p_point.y,sub->m_hMenu,FALSE,FALSE))
      {
        return;
      }
      m_selectedMenu = i;
      CFrameWndEx* frame = (CFrameWndEx*)AfxGetMainWnd();
      popup->RecalcLayout();
      CMFCPopupMenu::ActivatePopupMenu(frame,popup);
      return;
    }
  }
}

void 
StyleFrameWndEx::OnNcCalcSize(BOOL calcValidRects,NCCALCSIZE_PARAMS* p_params)
{
  if(GetStyle() & WS_MAXIMIZE)
  {
    CRect area;
    StyleGetWorkArea(this,area);
    area.top += CAPTIONHEIGHT;
    p_params->rgrc[0] = area;
  }
  else
  {
    // The baseMargin is needed for two things:
    // 1) To activate the left/right/bottom mouse pulling action
    // 2) To provide space for a painted border around the window
    p_params->rgrc[0].top    += CAPTIONHEIGHT;
    p_params->rgrc[0].left   += MARGIN;
    p_params->rgrc[0].right  -= MARGIN;
    p_params->rgrc[0].bottom -= MARGIN;
  }
  // Use same rectangle for displacement (so hide it)
  if(calcValidRects)
  {
    p_params->rgrc[2] = p_params->rgrc[0];
  }
}

// Avoid flicker of the titlebar on activate
BOOL
StyleFrameWndEx::OnNcActivate(BOOL bActive)
{
  if(bActive)
  {
    OnNcPaint();
  }
  return TRUE;
}

void 
StyleFrameWndEx::OnNcPaint()
{
  CWindowDC dc(this);

  // BORDER around the window frame if not maximized
  if((GetStyle() & WS_MAXIMIZE) == 0)
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
  
  // CAPTION BAR
  dc.FillSolidRect(m_dragRect,ThemeColor::GetColor(Colors::AccentColor1));

  // TITLE
  CFont* orgfont = dc.SelectObject(&STYLEFONTS.CaptionTextFont);
  dc.SetTextColor(ColorWindowHeaderText);
  CString titel;
  GetWindowText(titel);
  dc.DrawText(titel, m_captionRect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
  dc.SelectObject(orgfont);

  // Windows Buttons
  // minimize button
  DrawButton(&dc, m_minRect, HTMINBUTTON);
  // maximize button
  DrawButton(&dc, m_maxRect, HTMAXBUTTON);
  // close button
  DrawButton(&dc, m_closeRect, HTCLOSE);

  // Draw menu items
  for(int ind = 0; ind < MENUCOUNT; ++ind)
  {
    DrawIcon(&dc, ind);
  }
}

void 
StyleFrameWndEx::ReDrawIcon(int index)
{
  if (index >= 0)
  {
    CWindowDC dc(this);
    DrawIcon(&dc, index);
  }
}

// Draw the menu icons on the toolbar
void 
StyleFrameWndEx::DrawIcon(CDC* pDC, int index)
{
  // Fill icon rectangle
  COLORREF frame = ThemeColor::GetColor(Colors::ColorWindowFrame);
  pDC->FillSolidRect(m_iconRects[index], index == m_selectedMenu ? ColorWindowHeader == frame ? Assistant5 : frame : ColorWindowHeader);

  // Get position of the icon
  CRect rect = m_iconRects[index];
  rect.SetRect(rect.CenterPoint().x - HALF_ICONSIZE
              ,rect.CenterPoint().y - HALF_ICONSIZE
              ,rect.CenterPoint().x + HALF_ICONSIZE
              ,rect.CenterPoint().y + HALF_ICONSIZE);

  // Menu index in the right domain
  if (index < 0 || index >= MENUCOUNT)
  {
    return;
  }

  // Load bitmap in compatible memory DC
  CDC memdc;
  CBitmap bitmap;
  bitmap.LoadBitmap(m_menuicon[index]);
  memdc.CreateCompatibleDC(pDC);
  CBitmap* prev = memdc.SelectObject(&bitmap);

  // Paint the icon
  int stretch = (index == m_selectedMenu) ? BLACKONWHITE : WHITEONBLACK;
  int paint   = (index == m_selectedMenu) ? NOTSRCCOPY   : SRCPAINT;

  int mode = pDC->SetStretchBltMode(stretch);
  SetBrushOrgEx(pDC->m_hDC, 0, 0, NULL);
  pDC->BitBlt(rect.left,rect.top,rect.right,rect.bottom,&memdc,0,0, paint);
  pDC->SetStretchBltMode(mode);
  // Restore old
  memdc.SelectObject(prev);
}

LRESULT StyleFrameWndEx::OnNcHitTest(CPoint point)
{
  CRect window;
  GetWindowRect(window);
  point.Offset(-window.TopLeft());

  if (m_closeRect.PtInRect(point))
  {
    return HTCLOSE;
  }
  if (m_minRect.PtInRect(point))
  {
    return HTMINBUTTON;
  }
  if (m_maxRect.PtInRect(point))
  {
    return HTMAXBUTTON;
  }
  if(MenuHitTest(point) >= 0)
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
    if (point.x >= window.right - 2 * SIZEMARGIN)
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
  if(PtInRect(client, point))
  {
    return HTCLIENT;
  }
  return HTNOWHERE;
}

// See if the mouse is on a menu
int   
StyleFrameWndEx::MenuHitTest(CPoint p_point)
{
  for(int index = 0; index < MENUCOUNT; ++index)
  {
    if(m_iconRects[index].PtInRect(p_point))
    {
      return index;
    }
  }
  return -1;
}

void StyleFrameWndEx::ReDrawButton(LRESULT type)
{
  CWindowDC dc(this);

  switch (type) 
  {
    case HTCLOSE:     DrawButton(&dc, m_closeRect,type); break;
    case HTMINBUTTON: DrawButton(&dc, m_minRect,  type); break;
    case HTMAXBUTTON: DrawButton(&dc, m_maxRect,  type); break;
   }
}

void StyleFrameWndEx::DrawButton(CDC* pDC, CRect rect, LRESULT type)
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

  switch (type) 
  {
    case HTCLOSE:     pDC->MoveTo(rect.CenterPoint().x - WS(6), rect.CenterPoint().y - WS(6));
                      pDC->LineTo(rect.CenterPoint().x + WS(7), rect.CenterPoint().y + WS(7));
                      pDC->MoveTo(rect.CenterPoint().x + WS(6), rect.CenterPoint().y - WS(6));
                      pDC->LineTo(rect.CenterPoint().x - WS(7), rect.CenterPoint().y + WS(7));
                      break;

    case HTMINBUTTON: pDC->MoveTo(rect.CenterPoint().x - WS(7), rect.CenterPoint().y + WS(7));
                      pDC->LineTo(rect.CenterPoint().x + WS(7), rect.CenterPoint().y + WS(7));
                      break;

    case HTMAXBUTTON: if ((GetStyle() & WS_MAXIMIZE) != 0)
                      {
                        // 'front' window
                        pDC->MoveTo(rect.CenterPoint().x - WS(7), rect.CenterPoint().y - WS(3));
                        pDC->LineTo(rect.CenterPoint().x + WS(4), rect.CenterPoint().y - WS(3));
                        pDC->LineTo(rect.CenterPoint().x + WS(4), rect.CenterPoint().y + WS(8));
                        pDC->LineTo(rect.CenterPoint().x - WS(7), rect.CenterPoint().y + WS(8));
                        pDC->LineTo(rect.CenterPoint().x - WS(7), rect.CenterPoint().y - WS(3));
      
                        //'back' window
                        pDC->MoveTo(rect.CenterPoint().x - WS(3), rect.CenterPoint().y - WS(4));
                        pDC->LineTo(rect.CenterPoint().x - WS(3), rect.CenterPoint().y - WS(7));
                        pDC->LineTo(rect.CenterPoint().x + WS(8), rect.CenterPoint().y - WS(7));
                        pDC->LineTo(rect.CenterPoint().x + WS(8), rect.CenterPoint().y + WS(4));
                        pDC->LineTo(rect.CenterPoint().x + WS(4), rect.CenterPoint().y + WS(4));
                      }
                      else
                      {
                        pDC->MoveTo(rect.CenterPoint().x - WS(6), rect.CenterPoint().y - WS(6));
                        pDC->LineTo(rect.CenterPoint().x + WS(7), rect.CenterPoint().y - WS(6));
                        pDC->LineTo(rect.CenterPoint().x + WS(7), rect.CenterPoint().y + WS(7));
                        pDC->LineTo(rect.CenterPoint().x - WS(6), rect.CenterPoint().y + WS(7));
                        pDC->LineTo(rect.CenterPoint().x - WS(6), rect.CenterPoint().y - WS(6));
                      }
                      break;
  }
  pDC->SelectObject(orgpen);
}
