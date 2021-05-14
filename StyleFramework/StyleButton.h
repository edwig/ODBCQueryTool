////////////////////////////////////////////////////////////////////////
//
// File: StyleButton.h
// Function: Buttons and IMG button to quickly set an icon resource
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

// WM_IBS_SetStyle message
#define WM_NORMALBUTTON (WM_USER + 13)

class StyleEdit;

class StyleButton : public CButton
{
  DECLARE_DYNAMIC(StyleButton)

public:
  StyleButton();
  StyleButton(CString p_type,bool pInError = false);
  virtual  ~StyleButton();
  virtual   BOOL DestroyWindow() override;
  BOOL      EnableWindow(BOOL p_enable = TRUE);

  // SETTERS
  void      SetStyle(CString p_style);
  void      SetImage(UINT p_bitmap);
  void      SetIconImage(UINT p_icon);
  void      SetMandatory(bool pValue) { m_mandatory = pValue; }
  void      SetInError(bool pValue) { m_error = pValue; }
  void      SetDefaultButton(bool pDefaultknop) { m_defaultButton = pDefaultknop; }

  // GETTERS
  CBitmap&  GetBitmap();
  bool      GetMandatory() { return m_mandatory; }

  // CALENDAR
  void      StartCalendar(int p_control);
  void      StartCalendar(StyleEdit* p_edit);

  DECLARE_MESSAGE_MAP()

  afx_msg int      OnCreate(LPCREATESTRUCT cs);
  afx_msg LRESULT  OnNormalButton(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT  OnSetStyle(WPARAM wParam, LPARAM lParam);
  afx_msg void     OnMouseMove(UINT nFlags, CPoint point);
  afx_msg LRESULT  OnMouseHover(WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT  OnMouseLeave(WPARAM wParam,LPARAM lParam);
  afx_msg void     OnDestroy();
  afx_msg void     OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void     OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void     OnSize(UINT nType, int cx, int cy);
  afx_msg BOOL     OnEraseBkgnd(CDC* pDC);
  afx_msg void     OnPaint();

protected:
  virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam) override;
  void    TranslateStyle(CString p_style);
  void    Draw(CDC*    pDC
              ,CRect   rect
              ,bool    over
              ,UINT    state
              ,DWORD   style
              ,HICON   hicon
              ,CRect   bmpRect
              ,CString txt
              ,bool    pInError      = false
              ,bool    dialog        = false
              ,bool    mandatory     = false
              ,bool    p_themeColor  = false
              ,bool    p_default     = false);

  HICON   m_icon { nullptr };
  bool    m_error;
  bool    m_over;
  bool    m_mandatory;
  bool    m_defaultButton;
  int     m_style { 1 };
  CBitmap m_bitmap;
};

inline CBitmap& 
StyleButton::GetBitmap()
{
  return m_bitmap;
}
