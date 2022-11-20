////////////////////////////////////////////////////////////////////////
//
// File: StyleMDIFrameWnd.h
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
#pragma once
#include <afxmdiframewndex.h>

class StyleMDIFrameWnd : public CMDIFrameWndEx
{
  DECLARE_DYNAMIC(StyleMDIFrameWnd)

  DECLARE_MESSAGE_MAP();

public:
  StyleMDIFrameWnd();

  virtual BOOL PreCreateWindow(CREATESTRUCT& cs) override;
  virtual BOOL PreTranslateMessage(MSG* message) override;
  virtual BOOL PreTranslateMenu(MSG* p_msg);

  void LoadStyleTheme();
  void SetTheme(ThemeColor::Themes p_theme);
  BOOL SetSysMenu(UINT p_menuResource);

protected:
  // Sub-procedures
  void  ReDrawButton(LRESULT type);
  void  DrawButton(CDC* pDC, CRect rect, LRESULT type);
  void  SetWindowText(LPCTSTR lpstString);
  // Perform a sub-menu on the bar
  void  PerformMenu();

  // AFX Messages
  afx_msg int     OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg LRESULT OnGrayScreen(WPARAM wParam, LPARAM lParam);
  afx_msg void    OnNcLButtonDblClk(UINT nFlags, CPoint point);
  afx_msg void    OnNcMouseMove(UINT nFlags, CPoint point);
  afx_msg void    OnNcLButtonDown(UINT nFlags, CPoint point);
  afx_msg void    OnNcLButtonUp(UINT nHitTest, CPoint point);
  afx_msg LRESULT OnNcMouseLeave(WPARAM wParam, LPARAM lParam);
  afx_msg void    OnSize(UINT nType, int cx, int cy);
  afx_msg void    OnNcCalcSize(BOOL calcValidRects, NCCALCSIZE_PARAMS *params);
  afx_msg void    OnNcPaint();
  afx_msg LRESULT OnNcHitTest(CPoint point);
  afx_msg BOOL    OnEraseBkgnd(CDC* pdc);
  afx_msg LRESULT OnStyleChanged(WPARAM wParam,LPARAM lParam);
  afx_msg void    OnSettingChange(UINT uFlags,LPCTSTR lpszSection);
  afx_msg BOOL    OnNcActivate(BOOL bActive);

  // Standard handlers for a command menu
  afx_msg void    OnStyleLime();
  afx_msg void    OnStyleSkyblue();
  afx_msg void    OnStylePurple();
  afx_msg void    OnStyleModerateGray();
  afx_msg void    OnStylePureGray();
  afx_msg void    OnStyleBlackWhite();
  

  // Move hovers and is currently down
  LRESULT   m_curhit    { HTNOWHERE };
  UINT      m_sysmenu   { NULL      };
  bool      m_down      { false     };
  bool      m_dtsize    { false     };
  bool      m_menuFocus { false     };
  CMenu     m_menu;                           // System menu
  // Rectangles
  CRect     m_windowRectLocal { 0,0,0,0 };    // Local window rectangle
  CRect     m_sysRect         { 0,0,0,0 };    // System   button rectangle
  CRect     m_minRect         { 0,0,0,0 };    // Minimize button rectangle
  CRect     m_maxRect         { 0,0,0,0 };    // Maximize button rectangle
  CRect     m_closeRect       { 0,0,0,0 };    // Close button rectangle
  CRect     m_dragRect        { 0,0,0,0 };    // Rectangle for dragging
  CRect     m_captionRect     { 0,0,0,0 };    // Rectangle of the caption
  CRect     m_menuRect        { 0,0,0,0 };    // Rectangle for a menu
  // Tooltips 
  CToolTipCtrl m_menuTooltip;
  GrayWindow   m_grayScreen;
};
