////////////////////////////////////////////////////////////////////////
//
// File: StyleDialog.h
// Function: General Dialog box for styling windows
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
#include <map>

class StyleComboBox;
using ToolTips = std::map<HWND,const char*>;

class StyleDialog : public CDialog
{
  DECLARE_DYNAMIC(StyleDialog)

  DECLARE_MESSAGE_MAP();

public:
  enum BUTTONSTATE
  {
    BS_NORMAL,
    BS_HOVER,
    BS_PRESSED,
    BS_DISABLED
  };

  StyleDialog(UINT  p_IDTemplate
             ,CWnd* p_parentWnd = NULL
             ,bool  p_caption   = true
             ,bool  p_sysmenu   = false
             ,bool  p_status    = false);

  virtual INT_PTR DoModal() override;
  virtual INT_PTR DoModal(bool p_showGrayscreen);
  virtual BOOL    OnInitDialog() override;
  virtual BOOL    InitFirstFocus();
  virtual bool    OnClosing();
  virtual void    SetupDynamicLayout();

  void    ReDrawFrame();
  void    ShowSysMenu     (bool p_sysmenu     = true);
  void    ShowMinMaxButton(bool p_minButton   = true
                          ,bool p_maxButton   = true);
  void    ShowCloseButton (bool p_closeButton = true);
  void    SetCanResize    (bool p_resize      = true);
  void    SetWindowText(LPCTSTR lpstString);
  BOOL    SetSysMenu(UINT p_menuResource);
  void    SetAboutBoxAndIcon(int p_command,int p_menutitle);

  void    SetTheme(ThemeColor::Themes p_theme);
  void    LoadStyleTheme();
  CWnd*   GetNextDlgTabItem(CWnd* p_control, BOOL p_previous = FALSE) const;

  void    RegisterTooltip(int p_ID,            const char* p_text);
  void    RegisterTooltip(CWnd& p_wnd,         const char* p_text);
  void    RegisterTooltip(StyleComboBox& p_wnd,const char* p_text);

protected:
  virtual BOOL    PreTranslateMessage(MSG* p_msg) override;
  virtual INT_PTR OnToolHitTest(CPoint point,TOOLINFO* pTI) const override;

  BOOL    PreTranslateMenu(MSG* p_msg);
  void    ReDrawDialog();
  void    ReDrawButton(LRESULT type);
  void    DrawButton(CDC* pDC,CRect rect,LRESULT type);
  void    PositionButtons();
  void    Button(CDC* pDC, CRect rect, LRESULT type, BUTTONSTATE state = BS_NORMAL, bool max = true);
  void    PerformMenu();
  void    InitStatusBar();

  // Message handlers
  afx_msg int     OnCreate(LPCREATESTRUCT p_create);
  afx_msg BOOL    OnEraseBkgnd(CDC* pDC);
  afx_msg HBRUSH  OnCtlColor(CDC* pDC,CWnd* pWnd,UINT nCtlColor);
  afx_msg LPARAM  OnCtlColorStatic (WPARAM wParam,LPARAM lParam);
  afx_msg LPARAM  OnCtlColorListBox(WPARAM wParam,LPARAM lParam);
  afx_msg void    OnNcMouseMove(UINT nFlags, CPoint point);
  afx_msg void    OnNcLButtonDown(UINT nFlags, CPoint point);
  afx_msg void    OnNcRButtonUp(UINT nFlags, CPoint point);
  afx_msg void    OnNcLButtonDblClk(UINT nFlags, CPoint point);
  afx_msg void    OnNcMouseLeave();
  afx_msg void    OnSize(UINT nType, int cx, int cy);
  afx_msg void    OnNcCalcSize(BOOL calcValidRects, NCCALCSIZE_PARAMS *params);
  afx_msg void    OnNcPaint();
  afx_msg LRESULT OnNcHitTest(CPoint point);
  afx_msg void    OnSysCommand(UINT nID, LPARAM lParam);
  afx_msg void    OnActivate(UINT nState,CWnd* pWndOther,BOOL bMinimized);
  afx_msg void    OnActivateApp(BOOL bActive,DWORD dwThreadID);
  afx_msg void    OnCancel() override;
  afx_msg LRESULT OnStyleChanged(WPARAM wParam,LPARAM lParam);
  afx_msg void    OnSettingChange(UINT uFlags,LPCTSTR lpszSection);
  afx_msg BOOL    OnToolTipNotify(UINT id,NMHDR* pNMHDR,LRESULT* pResult);
  afx_msg HCURSOR OnQueryDragIcon();
  afx_msg void    OnPaint();
  afx_msg void    OnOK() override;

  // Standard handlers for a command menu
  afx_msg void    OnStyleLime();
  afx_msg void    OnStyleSkyblue();
  afx_msg void    OnStylePurple();
  afx_msg void    OnStyleModerateGray();
  afx_msg void    OnStylePureGray();
  afx_msg void    OnStyleBlackWhite();
  afx_msg void    OnStyleDark();
  
  bool      m_error       { false   };  // Dialog is in an error state?
  bool      m_down        { false   };  // mouse pressed in m_closeRect
  bool      m_dtsize      { false   };  // Down-to-size above task bar
  bool      m_menuFocus   { false   };  // System menu has the focus
  // Button rectangles
  CRect     m_sysRect     { 0,0,0,0 };  // System   button rectangle
  CRect     m_minRect     { 0,0,0,0 };  // Minimize button rectangle
  CRect     m_maxRect     { 0,0,0,0 };  // Maximize button rectangle
  CRect     m_closeRect   { 0,0,0,0 };  // Close    button rectangle
  // drag rectangle
  CRect     m_captionRect { 0,0,0,0 };
  // Original window size
  CRect     m_originalSize{ 0,0,0,0 };
  // caption bar 
  bool      m_caption     { true  };
  bool      m_closeButton { true  };
  bool      m_mnuButton   { false };
  bool      m_minButton   { false };
  bool      m_maxButton   { false };
  bool      m_canResize   { false };
  bool      m_hasStatus   { false };
  LRESULT   m_curhit      { HTNOWHERE };
  UINT      m_sysmenu     { NULL  };
  // Objects
  CMenu      m_menu;
  HICON      m_hIcon;
  CStatusBar m_statusBar;
  CBrush     m_defaultBrush;
  ToolTips   m_tooltips;
};
