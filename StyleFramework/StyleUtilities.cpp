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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
    int extra = 2 * GetSystemMetrics(SM_CYFRAME);
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

int GetSFXSizeFactor();
void SFXResizeByFactor(CRect& p_rect);

// Scale a control to the SFXSizeFactor
void
ScaleControl(CWnd* p_wnd)
{
  // See if we must do scaling
  if(GetSFXSizeFactor() == 100)
  {
    return;
  }

  CRect rect;
  p_wnd->GetWindowRect(rect);

  CWnd* parent = p_wnd->GetParent();
  if(parent)
  {
    parent->ScreenToClient(rect);
  }
  SFXResizeByFactor(rect);
  // p_wnd->MoveWindow(rect);
  SetWindowPos(p_wnd->GetSafeHwnd(),p_wnd->GetSafeHwnd()
              ,rect.left,rect.top,rect.Width(),rect.Height()
              ,SWP_NOZORDER|SWP_NOACTIVATE);
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