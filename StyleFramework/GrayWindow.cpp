////////////////////////////////////////////////////////////////////////
//
// File: Grayscreen.h
// Function: Draw gray edge around a screen
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
#include "GrayWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static int g_statusBarHeight = STATUSBAR_HEIGHT;

int
SetGrayWindowStatusBarHeight(int p_height)
{
  int previous = g_statusBarHeight;
  if(p_height >= 0 && p_height < STATUSBAR_HEIGHT_MAX)
  {
    g_statusBarHeight = p_height;
  }
  return previous;
}

BOOL CALLBACK GrayWindow::MonitorEnumProc(__in  HMONITOR  hMonitor,
                                          __in  HDC       hdcMonitor,
                                          __in  LPRECT    lprcMonitor,
                                          __in  LPARAM    dwData)
{
  MonitorEnumProcStruct* _struct = (MonitorEnumProcStruct*)dwData;
  CRect _rect(*lprcMonitor);

  MONITORINFO mi;
  mi.cbSize = sizeof(MONITORINFO);
  GetMonitorInfo(hMonitor,&mi);

  BLENDFUNCTION function;
  function.BlendOp = AC_SRC_OVER;
  function.BlendFlags = 0;
  function.SourceConstantAlpha = 175;
  function.AlphaFormat = 0;
  CWindowDC wdc(GetDesktopWindow());
  _struct->graywnd->m_dc.AlphaBlend(_rect.left - _struct->window.left
                                   ,_rect.top  - _struct->window.top
                                   ,_rect.Width()
                                   ,_rect.Height()
                                   ,&wdc,_rect.left
                                   ,_rect.top,_rect.Width()
                                   ,_rect.Height()
                                   ,function);

  return TRUE;
}

bool GrayWindow::Show()
{
  if(m_dc.m_hDC != NULL)
  {
    return false;
  }
  CRect window;
  AfxGetMainWnd()->GetWindowRect(window);
  CWindowDC pDC(this);
  m_dc.CreateCompatibleDC(&pDC);
  m_bitmap.DeleteObject();
  m_bitmap.CreateCompatibleBitmap(&pDC,window.Width(),window.Height() - g_statusBarHeight);
  m_dc.SelectObject(&m_bitmap);
  m_dc.FillSolidRect(0,0,window.Width(),window.Height() - g_statusBarHeight,RGB(0,0,0));

  MonitorEnumProcStruct _struct;
  _struct.window = window;
  _struct.graywnd = this;

  EnumDisplayMonitors(NULL,window,GrayWindow::MonitorEnumProc,(LPARAM)&_struct);

  SetWindowPos(0,window.left,window.top,window.Width(),window.Height() - g_statusBarHeight,SWP_NOZORDER | SWP_SHOWWINDOW);
  return true;
}

void GrayWindow::Hide()
{
  ShowWindow(SW_HIDE);
  m_dc.DeleteDC();
  m_bitmap.DeleteObject();
}

BEGIN_MESSAGE_MAP(GrayWindow,CWnd)
  ON_WM_ERASEBKGND()
  ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL GrayWindow::OnEraseBkgnd(CDC* pDC)
{
  return TRUE;
}

void 
GrayWindow::OnPaint()
{
  if(GetUpdateRect(NULL,FALSE))
  {
    CPaintDC dc(this);
    CRect client;
    GetClientRect(client);
    dc.BitBlt(0,0,client.Width(),client.Height(),&m_dc,0,0,SRCCOPY);
  }
}

// Main window must implement the WM_GRAYSCREEN message !!

AutoStyleGrayScreen::AutoStyleGrayScreen()
{
  HWND wnd = AfxGetMainWnd()->GetSafeHwnd();
  if (wnd)
  {
    m_hide = ::SendMessage(wnd,WM_GRAYSCREEN,1,0);
  }
}

AutoStyleGrayScreen::~AutoStyleGrayScreen()
{
  if(m_hide)
  {
    HWND wnd = AfxGetMainWnd()->GetSafeHwnd();
    if (wnd)
    {
      SendMessage(wnd,WM_GRAYSCREEN,0,0);
      m_hide = false;
    }
  }
}
