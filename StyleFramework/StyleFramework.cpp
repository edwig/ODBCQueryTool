////////////////////////////////////////////////////////////////////////
//
// File: StyleFrameWork.cpp
// Function: General header of the StylingFramework
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

// The one-and-only global styling object
StylingFramework g_styling;

StylingFramework::StylingFramework()
{
  if(m_instance || &g_styling != this)
  {
    throw CString(_T("SFX can only be initialized once!"));
  }
  m_instance = this;

  // Support for StyleRichEdit
  LoadLibrary(_T("Riched32.dll"));

  // Getting all monitors and fonts
  m_monitors.DiscoverAllMonitors();
}

// Getting all monitors and fonts
void
StylingFramework::RefreshMonitors()
{
  m_monitors.ReDiscoverMonitors();
}

const StyleMonitor* 
StylingFramework::GetMonitor(HWND p_hwnd) const
{
  HMONITOR hMonitor = ::MonitorFromWindow(p_hwnd,MONITOR_DEFAULTTONEAREST);
  if(hMonitor)
  {
    return GetMonitor(hMonitor);
  }
  return nullptr;
}

const StyleMonitor*
StylingFramework::GetMonitor(HMONITOR p_monitor) const
{
  return m_monitors.GetMonitor(p_monitor);
}

const StyleMonitor*
StylingFramework::GetMonitor(int p_dpi_x,int p_dpi_y) const
{
  return m_monitors.GetMonitor(p_dpi_x,p_dpi_y);
}

const StyleMonitor* 
StylingFramework::GetMonitor(CString p_name) const
{
  return m_monitors.GetMonitor(p_name);
}

const StyleMonitor* 
StylingFramework::GetPrimaryMonitor() const
{
  return m_monitors.GetPrimaryMonitor();
}

int GetSFXSizeFactor(HWND p_hwnd)
{
  const StyleMonitor* monitor = g_styling.GetMonitor(p_hwnd);
  if(!monitor)
  {
    return 100;
  }
  int dpi_x;
  int dpi_y;
  monitor->GetDPI(dpi_x,dpi_y);
  return (dpi_y * 100) / USER_DEFAULT_SCREEN_DPI;
}

int GetSFXSizeFactor(HMONITOR p_monitor)
{
  const StyleMonitor* monitor = g_styling.GetMonitor(p_monitor);
  if(!monitor)
  {
    return 100;
  }
  int dpi_x;
  int dpi_y;
  monitor->GetDPI(dpi_x,dpi_y);
  return (dpi_y * 100) / USER_DEFAULT_SCREEN_DPI;
}

// Global function to get the correct font for a window
// based on its monitor
CFont*
GetSFXFont(HWND p_hwnd,StyleFontType p_type)
{
  const StyleMonitor* monitor = g_styling.GetMonitor(p_hwnd);
  if(monitor)
  {
    StyleFonts& fonts = const_cast<StyleFonts&>(monitor->GetFonts());
    return fonts.GetFont(p_type);
  }
  return nullptr;
}

CFont* 
GetSFXFont(HMONITOR hm,StyleFontType p_type)
{
  const StyleMonitor* monitor = g_styling.GetMonitor(hm);
  if(monitor)
  {
    StyleFonts& fonts = const_cast<StyleFonts&>(monitor->GetFonts());
    return fonts.GetFont(p_type);
  }
  return nullptr;
}
