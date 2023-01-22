////////////////////////////////////////////////////////////////////////
//
// File: StyleDialogCA.cpp
// Function: General Dialog box for styling windows
//
// Dialog with special styled header and behavior
// Paints the frame INSIDE of the client area (defined in the resources)
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
#include <afxdialogex.h>

class StyleDialogCA: public CDialogEx
{
#define CONTROLCOUNT 10
  DECLARE_MESSAGE_MAP();

public:
  enum BUTTONSTATE
  {
    BS_NORMAL,
    BS_HOVER,
    BS_PRESSED,
    BS_DISABLED
  };

  DECLARE_DYNAMIC(StyleDialogCA)

  StyleDialogCA(UINT nIDTemplate,CWnd* p_parent = nullptr,bool p_caption = true);
  virtual ~StyleDialogCA();
  virtual INT_PTR DoModal()      override;
  virtual BOOL    OnInitDialog() override;

  void    ShowCloseButton(bool p_show = true);
  void    GetClientRect(LPRECT lpRect);
  void    ReDrawFrame();
  void    SetWindowText(LPCTSTR lpstString);

  // Paint window button from hooked framework
  static void Button(CDC* pDC, CRect rect, LRESULT type, BUTTONSTATE state = BS_NORMAL, bool max = true);

protected:
  virtual BOOL    PreCreateWindow(CREATESTRUCT& cs) override;

  afx_msg int     OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void    OnDestroy();
  afx_msg BOOL    OnEraseBkgnd(CDC* pDC);
  afx_msg void    OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void    OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void    OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnGrayScreen(WPARAM wParam, LPARAM lParam);
  afx_msg HBRUSH  OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
  afx_msg void    OnSize(UINT nType, int cx, int cy);

  bool      m_error     { false };          // Error state
  bool      m_over      { false };          // mouse hovers over m_closeRect
  bool      m_down      { false };          // mouse pressed in m_closeRect
  bool      m_caption   { true  };          // Has a caption
  bool      m_close     { true  };          // Closing button
  CRect     m_windowRect{ 0,0,0,0 };        // Total window rectangle
  CRect     m_clientRect{ 0,0,0,0 };        // client rectangle
  CRect     m_properClientRect{ 0,0,0,0 };  // real client rect
  CRect     m_closeRect { 0,0,0,0 };        // close button rect
  CRect     m_dragRect  { 0,0,0,0 };        // dragging area of the caption

protected:
  GrayWindow m_grayScreen;
  CBrush     m_defaultBrush;
};
