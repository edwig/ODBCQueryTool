////////////////////////////////////////////////////////////////////////
//
// File: SkinScrollWnd.h
// Function: Extra control window between control and the scrollbar
//
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
#include "SkinScrollBar.h"

#define TIMER_UPDATE              100               // Update the scrollbars timer ID
#define FRAME_FIRST_SCROLLBARS    100               // Update scrollbars after this amount of time (milliseconds)
#define FRAME_DEFAULT_COLOR       RGB(0x7F,0x7F,0x7F)
#define FRAME_MINIMAL_BORDER      1

// Skin frame for controls
//
class SkinScrollWnd : public CWnd
{
  DECLARE_DYNAMIC(SkinScrollWnd);

  // Construction
public:
  SkinScrollWnd();
  virtual ~SkinScrollWnd();
  BOOL     SkinWindow(CWnd* pWnd,int p_borderSize = 0,int p_clientBias = 0);
  CWnd*    GetControl();
  void     DrawFrame();
  void     DrawFrame(COLORREF p_color,int p_size = 1);
  void     SetScrollbarBias(int p_bias);
  void     SetBorderColor(COLORREF p_color);
  void     SetPollScrollbars(BOOL p_poll);
  void     SetPaintPassthrough(BOOL p_paint);
  void     SetSkinScrollWidth(HWND p_hwnd,HMONITOR p_monitor = nullptr);
  void     CalculateControlSize(CRect& p_rect);
  void     RepositionControlWindow(CWnd* p_wnd = nullptr);

  // Mouse capture
  void     SetMouseCapture(BOOL p_capture = TRUE,DWORD p_flags = TME_HOVER | TME_LEAVE,DWORD p_hoverTime = HOVER_DEFAULT);
  void     SkinSetCapture();
  void     SkinReleaseCapture();
  void     SkinSetMouseTracking();
  void     SkinCancelMouseTracking();

  // Attributes
  SkinScrollBar	m_sbHorz;       // Horizontal scrollbar
  SkinScrollBar m_sbVert;       // Vertical scroll bar
  CWnd          m_wndLimit;     // Limiter for the control
  BOOL          m_busy;         // Skin is busy, do not call again
  BOOL          m_moveClient;   // Client is moving within the skin
  BOOL          m_doTimer;      // Keep polling the scroll positions
  BOOL          m_doCapture;    // Skin must capture the mouse for the control
  BOOL          m_didCapture;   // Skin has an active mouse capture
  bool          m_didTracking;  // Skin is tracking the mouse
  DWORD         m_captureFlags; // Mouse move capture flags (TME_HOVER / TM_LEAVE)
  DWORD         m_hoverTime;    // hover time in milliseconds
  BOOL          m_doPaint;      // Paint pass-through
  COLORREF      m_lastColor;    // Last set color of the outer border
  COLORREF      m_borderColor;  // Color of the inner border (if any)
  int           m_nScrollWidth; // Width of a vertical scroll bar
  int           m_nAngleType;   // Angle type in the bitmap (left or right)
  int           m_borderSize;   // Border between frame and control
  int           m_scrollBias;   // Bias for the scrollbars
  int           m_clientBias;   // Bias for the client area (e.g. tree controls, combobox list controls)
  WNDPROC       m_funOldProc;   // Original WindowsProc before sub-classing
  CRect         m_dpiRect;      // DPI Awareness

  // Generated message map functions
  afx_msg LRESULT OnLimiterMove(WPARAM wParam, LPARAM lParam);
  afx_msg void    OnSize(UINT nType, int cx, int cy);
  afx_msg void    OnMove(int x,int y);
  afx_msg void    OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg void    OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg void    OnMouseMove(UINT nFlags,CPoint point);
  afx_msg LRESULT OnMouseHover(WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnMouseLeave(WPARAM wParam,LPARAM lParam);
  afx_msg void    OnPaint();
  afx_msg void    OnNcPaint();
  afx_msg void    OnSetFocus(CWnd* p_old);
  afx_msg void    OnTimer(UINT_PTR nIDEvent);
  afx_msg void    OnCaptureChanged(CWnd* pWnd);
  afx_msg LRESULT OnLButtonDown  (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnLButtonUp    (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnLButtonDblClk(WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnRButtonDown  (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnRButtonUp    (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnRButtonDblClk(WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnMButtonDown  (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnMButtonUp    (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnMButtonDblClk(WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnXButtonDown  (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnXButtonUp    (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnXButtonDblClk(WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnGetDpiScaledSize(WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnDpiChangedBefore(WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnDpiChangedAfter (WPARAM wParam,LPARAM lParam);

protected:
  DECLARE_MESSAGE_MAP();

  void RecalcSkinSize(CWnd* p_control);
};

extern "C" BOOL            UnskinWndScroll(CWnd* p_wnd);
extern "C" SkinScrollWnd*  SkinWndScroll  (CWnd* p_wnd,int p_borderSize = 0,int p_clientBias = 0);
