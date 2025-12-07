////////////////////////////////////////////////////////////////////////
//
// File: StyleToast.h
// Function: Styling frame for a TOAST control
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
#include "StyleStaticToast.h"
#include "resource.h"
#include <vector>

#define EV_TOAST     99  // Timer ID for total toast
#define EV_STEPPER   98  // Timer ID for the stepper
#define TOAST_STEPS 100  // Number of steps in the stepper

#define TOAST_SPACE   2  // 2 pixels from the side/top/bottom
#define TOAST_STACK   4  // Maximum toasts to stack on the screen
#define TOAST_LINE   12  // 12 pixels for the stepper line

#define TOAST_MAX_PARALLEL   20
#define TOAST_MINIMUM_TIME 3000
#define TOAST_DEFAULT_TIME 5000

// Styles of the toast message
#define STYLE_TOAST_MESSAGE   1
#define STYLE_TOAST_WARNING   2
#define STYLE_TOAST_ERROR     3

// Position for the toast message
#define STYLE_POS_TOPLEFT     0x11
#define STYLE_POS_TOPMIDDLE   0x12
#define STYLE_POS_TOPRIGHT    0x13
#define STYLE_POS_MIDLEFT     0x21
#define STYLE_POS_MIDMIDDLE   0x22
#define STYLE_POS_MIDRIGHT    0x23
#define STYLE_POS_BOTLEFT     0x31
#define STYLE_POS_BOTMIDDLE   0x32
#define STYLE_POS_BOTRIGHT    0x33

class StyleToast;

void DestroyToast(StyleToast* p_toast);
StyleToast* CreateToast(int      p_style
                       ,int      p_position
                       ,CString  p_text1
                       ,CString  p_text2   = _T("")
                       ,CString  p_text3   = _T("")
                       ,unsigned p_timeout = TOAST_DEFAULT_TIME
                       ,bool*    p_success = nullptr);

class Toasts
{
public:
  Toasts();
 ~Toasts();
  std::vector<StyleToast*> m_toasts;
  CRITICAL_SECTION m_lock;
};

class StyleToast : public CDialog
{
public:
  StyleToast(int      p_style
            ,int      p_position
            ,CString  p_text1
            ,CString  p_text2   = _T("")
            ,CString  p_text3   = _T("")
            ,unsigned p_timeout = TOAST_DEFAULT_TIME);
  virtual ~StyleToast();

  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;

  afx_msg int     OnCreate(LPCREATESTRUCT p_create);
  afx_msg void    OnSize(UINT nType, int x, int y);
  afx_msg void    OnClicked();
  afx_msg void    OnPaint();
  afx_msg void    OnTimer(UINT_PTR nIDEvent);
  afx_msg HBRUSH  OnCtlColor(CDC* pDC,CWnd* pWnd,UINT nCtlColor);
  afx_msg void    PumpMessage();

protected:
  void SetHalfBackgroundColor();
  void PaintStepper(CDC* p_dc,CRect p_stepper,DWORD p_color);

  DECLARE_MESSAGE_MAP()

private:
  int         m_style;
  int         m_position;
  CString     m_text1;
  CString     m_text2;
  CString     m_text3;
  unsigned    m_timeout;
  unsigned    m_steps;
  DWORD       m_background;
  DWORD       m_foreground;
  DWORD       m_dimmedback;

  UINT_PTR    m_killTimer;
  UINT_PTR    m_stepTimer;

  StyleStaticToast m_showText;
};
