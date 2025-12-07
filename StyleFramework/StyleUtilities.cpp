////////////////////////////////////////////////////////////////////////
//
// File: StyleUtilities.cpp
// Function: String utilities for the StyleMessageBox
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
#include "StyleUtilities.h"
#include <afxglobals.h>
#include <shellscalingapi.h>
#include <VersionHelpers.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma comment(lib,"shcore.lib")

CString EverythingBefore(CString p_string,CString p_tag)
{
  int pos = p_string.Find(p_tag);
  if(pos >= 0)
  {
    return p_string.Left(pos);
  }
  return "";
}

CString EverythingAfter(CString p_string,CString p_tag)
{
  int pos = p_string.Find(p_tag);
  if(pos >= 0)
  {
    return p_string.Mid(pos + 1);
  }
  return "";
}

int FindNoCase(CString p_string,CString p_substring)
{
  p_string.MakeLower();
  p_substring.MakeLower();

  return p_string.Find(p_substring);
}

bool Contains(CString p_string,CString p_tag)
{
  return p_string.Find(p_tag) >= 0;
}

CString IntegerToString(int p_number)
{
  CString number;
  number.Format(_T("%d"),p_number);
  return number;
}

CString NumToString(int p_number)
{
  CString number;
  number.Format(_T("%d"),p_number);
  return number;
}

void    
Split(CString p_string,CString p_delimiter,std::vector<CString>& p_array)
{
  // p_array.empty();

  int pos = p_string.Find(p_delimiter);
  while(pos >= 0)
  {
    CString string = p_string.Left(pos);
    p_string = p_string.Mid(pos + 1);

    p_array.push_back(string);

    pos = p_string.Find(p_delimiter);
  }

  if(p_string.GetLength())
  {
    p_array.push_back(p_string);
  }
}

void
StyleGetWorkArea(CWnd* p_wnd,CRect& p_rect,int x, int y)
{
  CRect rectWindow;
  p_wnd->GetWindowRect(&rectWindow);

  // Try to get the work-area from the multi-monitor setup
  MONITORINFO mi;
  mi.cbSize = sizeof(MONITORINFO);
  CPoint point;
  if (x || y)
  {
    point.x = x;
    point.y = y;
  }
  else point = rectWindow.CenterPoint();
  if(GetMonitorInfo(MonitorFromPoint(point,MONITOR_DEFAULTTONEAREST), &mi))
  {
    p_rect = mi.rcWork;
  }
  else
  {
    // Fall back on system parameters (pre-Vista)
    ::SystemParametersInfo(SPI_GETWORKAREA, 0, &p_rect, 0);
  }

  // See to it that we can pull out the hidden taskbar
  int nShellAutohideBars = GetGlobalData()->GetShellAutohideBars();
  if (nShellAutohideBars & AFX_AUTOHIDE_BOTTOM)
  {
    p_rect.bottom -= 2;
  }
  else if(nShellAutohideBars & AFX_AUTOHIDE_TOP)
  {
    p_rect.top += 2;
  }
  else if(nShellAutohideBars & AFX_AUTOHIDE_RIGHT)
  {
    p_rect.right -= 2;
  }
  else if(nShellAutohideBars & AFX_AUTOHIDE_LEFT)
  {
    p_rect.left += 2;
  }
}

void    
RepositionFrameWnd(CWnd* p_wnd,bool p_isFrame /*=true*/)
{
  CRect rect(0,0,0,0);
  StyleGetWorkArea(p_wnd,rect);

  if(p_isFrame)
  {
    int dpi = ::GetDpiForWindow(p_wnd->GetSafeHwnd());
    int extra = 2 * GetSystemMetricsForDpi(SM_CYFRAME,dpi);
    rect.InflateRect(extra,extra);
  }

  p_wnd->MoveWindow(rect,FALSE);
}

void
MinimalFrameWnd(CWnd* p_wnd)
{
  // Getting our working area
  CRect rect(0,0,0,0);
  StyleGetWorkArea(p_wnd,rect);

  // Getting the current window area
  CRect window(0,0,0,0);
  p_wnd->GetWindowRect(&window);

  // Calculate the minimal size of our window
  bool doResize = false;
  int minx = rect.Width()  / 5;
  int miny = rect.Height() / 5;

  // See if it is too narrow
  if(window.Width() < minx)
  {
    window.right = window.left + minx;
    doResize = true;
  }

  // See if it is too low
  if(window.Height() < miny)
  {
    window.bottom = window.top + miny;
    doResize = true;
  }

  // If so found, resize our window
  if(doResize)
  {
    p_wnd->MoveWindow(window, FALSE);
  }
}

CString StyleGetStringFromClipboard(HWND p_wnd /*=NULL*/)
{
#ifdef UNICODE
  UINT format = CF_UNICODETEXT;
#else
  UINT format = CF_TEXT;
#endif

  CString string;
  if(OpenClipboard(p_wnd))
  {
    HANDLE glob = GetClipboardData(format);
    if(glob)
    {
      LPCTSTR text = (LPCTSTR) GlobalLock(glob);
      string = text;
      GlobalUnlock(glob);

    }
    CloseClipboard();
  }
  return string;
}

bool StylePutStringToClipboard(CString p_string,HWND p_wnd /*=NULL*/,bool p_append /*=false*/)
{
  bool result = false;
#ifdef UNICODE
  UINT format = CF_UNICODETEXT;
#else
  UINT format = CF_TEXT;
#endif

  if(!p_append)
  {
    OpenClipboard(p_wnd);
    EmptyClipboard();
    CloseClipboard();
  }

  if(OpenClipboard(p_wnd))
  {
    // Put the text in a global GMEM_MOVABLE memory handle
    size_t size = ((size_t) p_string.GetLength() + 1) * sizeof(TCHAR);
    HGLOBAL memory = GlobalAlloc(GHND,size);
    if(memory)
    {
      void* data = GlobalLock(memory);
      if(data)
      {
        _tcsncpy_s((LPTSTR) data,size,(LPCTSTR) p_string.GetString(),size);
      }
      else
      {
        GlobalFree(memory);
        return false;
      }

      // Set the text on the clipboard
      // and transfer ownership of the memory segment
      SetClipboardData(format,memory);
      GlobalUnlock(memory);
      result = true;
    }
    CloseClipboard();
  }
  return result;
}

// Getting the DPI for the DPI_AWARE_PER_MONITOR_V2
bool GetDpi(HWND hWnd,int& p_dpi_x,int& p_dpi_y)
{
   bool v81 = IsWindows8Point1OrGreater();
   bool v10 = IsWindows10OrGreater();
   if (v81 || v10)
   {
      HMONITOR hMonitor = ::MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
      return GetDpiMonitor(hMonitor,p_dpi_x,p_dpi_y);
   }
   else
   {
      HDC hDC  = ::GetDC(hWnd);
      INT xdpi = ::GetDeviceCaps(hDC, LOGPIXELSX);
      INT ydpi = ::GetDeviceCaps(hDC, LOGPIXELSY);
      ::ReleaseDC(NULL, hDC);
      p_dpi_x = static_cast<int>(xdpi);
      p_dpi_y = static_cast<int>(ydpi);
      return true;
   }
}

bool GetDpiMonitor(HMONITOR hMonitor,int& p_dpi_x,int& p_dpi_y)
{
  UINT xdpi, ydpi;
  LRESULT success = ::GetDpiForMonitor(hMonitor,MDT_EFFECTIVE_DPI,&xdpi,&ydpi);
  if(success == S_OK)
  {
    p_dpi_x = static_cast<int>(xdpi);
    p_dpi_y = static_cast<int>(ydpi);
    return true;
  }
  p_dpi_x = USER_DEFAULT_SCREEN_DPI;
  p_dpi_y = USER_DEFAULT_SCREEN_DPI;
  return false;
}

// Saving and restoring the window on a multi-monitor setup
void StyleSaveWindowPosition(CWnd* p_wnd)
{
  CWinApp* app = AfxGetApp();
  const StyleMonitor* monitor = g_styling.GetMonitor(p_wnd->GetSafeHwnd());
  if(!monitor)
  {
    return;
  }
  // Save the name of the monitor
  const CString name = monitor->GetName();
  app->WriteProfileString(_T("Monitor"),_T("Name"),  name);
  app->WriteProfileInt   (_T("Monitor"),_T("Width"), monitor->GetWidth());
  app->WriteProfileInt   (_T("Monitor"),_T("Height"),monitor->GetHeight());

  // Save the window position relative to the monitor
  CRect monRect = monitor->GetRect();
  CRect winRect;
  p_wnd->GetWindowRect(&winRect);

  int left = winRect.left - monRect.left;
  int top  = winRect.top  - monRect.top;
  app->WriteProfileInt(_T("Window"),_T("Left"),  left);
  app->WriteProfileInt(_T("Window"),_T("Top"),   top);
  app->WriteProfileInt(_T("Window"),_T("Width"), winRect.Width());
  app->WriteProfileInt(_T("Window"),_T("Height"),winRect.Height());
}

// We should at least be able to see a minimum amount of pixels
// so we will not be obscured by a task bar in a different position
#define MINIMUM_VISIBLE 80

void
StyleRestoreWindowPosition(CWnd* p_wnd)
{
  CWinApp* app = AfxGetApp();
  CString name = app->GetProfileString(_T("Monitor"),_T("Name"),_T(""));
  if(name.IsEmpty())
  {
    return;
  }
  int width  = app->GetProfileInt(_T("Monitor"),_T("Width"), -1);
  int height = app->GetProfileInt(_T("Monitor"),_T("Height"),-1);

  const StyleMonitor* monitor = g_styling.GetMonitor(name);
  if(!monitor)
  {
    return;
  }
  // Getting the saved position
  int top  = app->GetProfileInt(_T("Window"),_T("Top"), -1);
  int left = app->GetProfileInt(_T("Window"),_T("Left"),-1);
  if(top == -1 && left == -1)
  {
    return;
  }

  // See if we would still be visible on that monitor after the move!
  if(!((left < (monitor->GetWidth()  - MINIMUM_VISIBLE)) &&
       (top  < (monitor->GetHeight() - MINIMUM_VISIBLE))))
  {
    return;
  }

  // Getting the relative monitor position
  CRect rect = monitor->GetRect();
  rect.left += left;
  rect.top  += top;

  // Move our window to this position
  CRect winrect;
  p_wnd->GetWindowRect(&winrect);
  ::SetWindowPos(p_wnd->GetSafeHwnd(),nullptr
                ,rect.left,rect.top,winrect.Width(),winrect.Height()
                ,SWP_NOZORDER | SWP_NOACTIVATE);

  // See if monitor not changed and we can resize the application
  if((width != monitor->GetWidth()) || (height != monitor->GetHeight()))
  {
    return;
  }

  // Getting the stored window size
  int winWidth  = app->GetProfileInt(_T("Window"),_T("Width"), -1);
  int winHeight = app->GetProfileInt(_T("Window"),_T("Height"),-1);

  // Resize to original size
  p_wnd->MoveWindow(rect.left,rect.top,winWidth,winHeight);
}
