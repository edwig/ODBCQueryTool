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
#pragma once
#define WM_GRAYSCREEN (WM_USER + 666)

#define STATUSBAR_HEIGHT      30    // Mostly 30 pixels
#define STATUSBAR_HEIGHT_MAX 120    // Max acceptable status bar height

int SetGrayWindowStatusBarHeight(int p_height);

class GrayWindow : public CWnd
{
public:
  bool Show();
  void Hide();

  DECLARE_MESSAGE_MAP();
  BOOL OnEraseBkgnd(CDC* pDC);
  void OnPaint();

protected:
  struct MonitorEnumProcStruct
  {
    CRect       window;
    GrayWindow* graywnd;
  };

  static BOOL CALLBACK MonitorEnumProc(__in  HMONITOR hMonitor,
                                       __in  HDC      hdcMonitor,
                                       __in  LPRECT   lprcMonitor,
                                       __in  LPARAM   dwData);

  CDC     m_dc;
  CBitmap m_bitmap;
};

class AutoStyleGrayScreen
{
public:
  AutoStyleGrayScreen();
 ~AutoStyleGrayScreen();
private:
  bool m_hide { false };
};