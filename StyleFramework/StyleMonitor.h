////////////////////////////////////////////////////////////////////////
//
// File: StyleMonitor.h
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
#pragma once
#include <vector>
#include "StyleFonts.h"

class StyleMonitor
{
public:
  StyleMonitor(HMONITOR p_handle
              ,CString  p_name
              ,RECT     p_rect
              ,bool     p_primary);
 ~StyleMonitor() = default;

  // Initialize the monitor (fonts!)
  void InitMonitor();

  // GETTERS
  const HMONITOR    GetMonitor()   const;
  const CString     GetName()      const; 
  const CString     GetDeviceName()const;
  const CRect       GetRect()      const;
  const bool        GetIsPrimary() const;
  const bool        GetIsMarked()  const;
  const StyleFonts& GetFonts()     const;
  const int         GetHeight()    const;
  const int         GetWidth()     const;
  void              GetDPI(int& p_dpi_x,int& p_dpi_y) const;
  int               GetSystemMetrics(int p_index) const;

  void SetAsMarked(bool p_mark = true);
  void SetDeviceName(CString p_deviceName);


private:
  HMONITOR   m_monitor;             // Monitor handle for the OS
  CString    m_name;                // Internal name of the monitor
  CString    m_deviceName;          // Device name of the monitor
  CRect      m_rect;                // Usable portion of the virtual desktop
  bool       m_primary;             // Is the primary monitor in the system?
  bool       m_mark { false };      // Mark status for rescanning all monitors

  StyleFonts m_fonts;               // All style fonts usable on the monitor
  int        m_dpi_x { USER_DEFAULT_SCREEN_DPI };  // Current horizontal DPI setting
  int        m_dpi_y { USER_DEFAULT_SCREEN_DPI };  // Current vertical   DPI setting
};

using MonitorMap = std::vector<StyleMonitor*>;

//////////////////////////////////////////////////////////////////////////
//
// StyleMonitors: Handling of multiple StyleMonitor objects
//
////////////////////////////////////////////////////////////////////////

class StyleMonitors
{
public:
  StyleMonitors() = default;
 ~StyleMonitors();
  
  bool DiscoverAllMonitors();
  void ReDiscoverMonitors();
  bool TryAddMonitor(StyleMonitor* p_monitor);

  // GETTERS
  const MonitorMap&   GetMonitors() const;
  const StyleMonitor* GetPrimaryMonitor() const;
  const StyleMonitor* GetMonitor(HMONITOR p_monitor) const;
  const StyleMonitor* GetMonitor(int p_dpi_x,int p_dpi_y) const;
  const StyleMonitor* GetMonitor(CString m_name) const;

private:
  void UnMarkAllMonitors();
  void CleanupRemovedMonitors();

  MonitorMap    m_monitors;
  StyleMonitor* m_primaryMonitor { nullptr };
};

