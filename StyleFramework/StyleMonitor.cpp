////////////////////////////////////////////////////////////////////////
//
// File: StyleMonitor.cpp
// Function: Connected monitors to the system
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
#include "StyleMonitor.h"
#include <shellscalingapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

StyleMonitor::StyleMonitor(HMONITOR p_handle
                          ,CString  p_name
                          ,RECT     p_rect
                          ,bool     p_primary)
             :m_monitor(p_handle)
             ,m_name(p_name)
             ,m_rect(p_rect)
             ,m_primary(p_primary)
{
  if(GetDpiMonitor(m_monitor,m_dpi_x,m_dpi_y) == false)
  {
    m_dpi_x = USER_DEFAULT_SCREEN_DPI;
    m_dpi_y = USER_DEFAULT_SCREEN_DPI;
  }
}

void
StyleMonitor::InitMonitor()
{
  m_fonts.Init(m_dpi_y);
}

const HMONITOR
StyleMonitor::GetMonitor() const
{
  return m_monitor;
}
  
const CString
StyleMonitor::GetName() const
{
  return m_name;
}

const CString
StyleMonitor::GetDeviceName() const
{
  return m_deviceName;
}

const CRect
StyleMonitor::GetRect() const
{
  return m_rect;
}
  
const bool
StyleMonitor::GetIsPrimary() const
{
  return m_primary;
}

const StyleFonts& 
StyleMonitor::GetFonts() const
{
  return m_fonts;
}

const int
StyleMonitor::GetHeight() const
{
  return m_rect.Height();
}

const int
StyleMonitor::GetWidth() const
{
  return m_rect.Width();
}

void
StyleMonitor::GetDPI(int& p_dpi_x,int& p_dpi_y) const
{
  p_dpi_x = m_dpi_x;
  p_dpi_y = m_dpi_y;
}

const bool
StyleMonitor::GetIsMarked() const
{
  return m_mark;
}

int
StyleMonitor::GetSystemMetrics(int p_index) const
{
  if(m_dpi_x)
  {
    return GetSystemMetricsForDpi(p_index,static_cast<UINT>(m_dpi_x));
  }
  else
  {
    return ::GetSystemMetrics(p_index);
  }
}

void
StyleMonitor::SetAsMarked(bool p_mark /* = true */)
{
  m_mark = p_mark;
}

void
StyleMonitor::SetDeviceName(CString p_deviceName)
{
  m_deviceName = p_deviceName;
}

//////////////////////////////////////////////////////////////////////////
//
// StyleMonitors: Handling of multiple StyleMonitor objects
//
//////////////////////////////////////////////////////////////////////////

StyleMonitors::~StyleMonitors()
{
  for(auto& monitor : m_monitors)
  {
    delete monitor;
  }
}

const MonitorMap& 
StyleMonitors::GetMonitors() const
{
  return m_monitors;
}

const StyleMonitor*
StyleMonitors::GetPrimaryMonitor() const
{
  return m_primaryMonitor;
}

bool
StyleMonitors::DiscoverAllMonitors()
{
  bool result = EnumDisplayMonitors(nullptr         // All monitors
                                   ,nullptr         // No clipping
                                   ,[](HMONITOR hMonitor,HDC hdcMonitor,LPRECT lprcMonitor,LPARAM dwData) -> BOOL
                                    {
                                      CString monitorName;
                                      bool primaryMonitor(false);
                                      MONITORINFOEX mi;
                                      mi.cbSize = sizeof(MONITORINFOEX);
                                      if(GetMonitorInfo(hMonitor,&mi))
                                      {
                                        monitorName    = mi.szDevice;
                                        primaryMonitor = (mi.dwFlags & MONITORINFOF_PRIMARY) != 0;
                                      }

                                      // Here we store the monitor handles and info
                                      StyleMonitors* monitors = reinterpret_cast<StyleMonitors*>(dwData);
                                      StyleMonitor*  monitor  = new StyleMonitor(hMonitor,monitorName,*lprcMonitor,primaryMonitor);
                                      if(monitors->TryAddMonitor(monitor) == false)
                                      {
                                        // Already present, delete the created one
                                        delete monitor;
                                      }
                                      // For now, we just continue enumeration
                                      return TRUE;
                                    }
                                   ,(LPARAM)this);  // No data

  // Find the logical device names for the monitors
  DISPLAY_DEVICE dd;
  dd.cb = sizeof(DISPLAY_DEVICE);
  for(int i = 0; EnumDisplayDevices(nullptr,i,&dd,0); ++i)
  {
    // Find the monitor with this device name
    for(auto& monitor : m_monitors)
    {
      if(monitor->GetName().CompareNoCase(dd.DeviceName) == 0)
      {
        monitor->SetDeviceName(dd.DeviceString);
        break;
      }
    }
  }

  return result;
}

// Try to add a monitor, returns false if already present
// Works on the fact that the HMONITOR is unique
bool 
StyleMonitors::TryAddMonitor(StyleMonitor* p_monitor)
{
  // New primary monitor?
  if(p_monitor->GetIsPrimary())
  {
    m_primaryMonitor = p_monitor;
  }
  // Check if already there
  for(auto& monitor : m_monitors)
  {
    if(monitor->GetMonitor() == p_monitor->GetMonitor())
    {
      // Same size monitor?
      if(p_monitor->GetWidth()  == monitor->GetWidth() &&
         p_monitor->GetHeight() == monitor->GetHeight())
      {
        int newx = 0;
        int newy = 0;
        int thisx = 0;
        int thisy = 0;
          monitor->GetDPI(thisx,thisy);
        p_monitor->GetDPI(newx, newy);

        // Same DPI monitor?
        if(newx == thisx ||  newy == thisy)
        {
          // Different DPI, update the existing monitor DPI info
          monitor->SetAsMarked();
          return false;
        }
      }
    }
  }
  // Init the fonts
  p_monitor->InitMonitor();

  // Remember our monitor
  p_monitor->SetAsMarked();
  m_monitors.push_back(p_monitor);
  return true;
}

const StyleMonitor* 
StyleMonitors::GetMonitor(HMONITOR p_monitor) const
{
  // Check if already there
  for(auto& monitor : m_monitors)
  {
    if(monitor->GetMonitor() == p_monitor)
    {
      // Already present
      return monitor;
    }
  }
  return nullptr;
}

const StyleMonitor*
StyleMonitors::GetMonitor(int p_dpi_x,int p_dpi_y) const
{
  // Check if already there
  for(auto& monitor : m_monitors)
  {
    int dpi_x;
    int dpi_y;
    monitor->GetDPI(dpi_x,dpi_y);
    if(dpi_x == p_dpi_x &&
       dpi_y == p_dpi_y)
    {
      // Should be a compatible monitor
      return monitor;
    }
  }
  return nullptr;
}

const StyleMonitor* 
StyleMonitors::GetMonitor(CString m_name) const
{
  for(auto& monitor : m_monitors)
  {
    if(monitor->GetName().CompareNoCase(m_name) == 0)
    {
      return monitor;
    }
  }
  return nullptr;
}

// Remove the mark on all monitors before discovery
void
StyleMonitors::UnMarkAllMonitors()
{
  for(auto& monitor : m_monitors)
  {
    monitor->SetAsMarked(false);
  }
}

// See if any monitors were removed since last discovery
void
StyleMonitors::CleanupRemovedMonitors()
{
  MonitorMap::iterator it = m_monitors.begin();
  while(it != m_monitors.end())
  {
    if((*it)->GetIsMarked() == false)
    {
      delete *it;
      it = m_monitors.erase(it);
    }
    else
    {
      ++it;
    }
  }
}

// Do the whole bunch again. See if we have new monitors or removed ones
void
StyleMonitors::ReDiscoverMonitors()
{
  UnMarkAllMonitors();
  DiscoverAllMonitors();
  CleanupRemovedMonitors();
}
