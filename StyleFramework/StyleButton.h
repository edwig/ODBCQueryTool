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
// Pressed buttons are only 80% of the background color
#define BUTTON_DIMMING 80

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
  void      SetMandatory(bool pValue)           { m_mandatory     = pValue;       }
  void      SetInError(bool pValue)             { m_error         = pValue;       }
  void      SetDefaultButton(bool pDefaultknop) { m_defaultButton = pDefaultknop; }
  void      SetBold(bool p_bold)                { m_bold          = p_bold;       }
  void      SetFontSize(int p_size);
  void      SetFontName(CString p_fontName,int p_fontSize);
  void      SetFontStyle(bool p_bold,bool p_italic,bool p_underLine);
  void      SetFontColor(int p_color);
  void      SetTextColor(int p_color);
  void      SetBkColor(int p_color);

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
  void    ResetFont();
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

  CBitmap   m_bitmap;
  CString   m_fontName;
  HICON     m_icon          { nullptr };
  CFont*    m_font          { nullptr };
  bool      m_error         { false   };
  bool      m_over          { false   };
  bool      m_mandatory     { false   };
  bool      m_defaultButton { false   };
  int       m_style         { 0       };
  int       m_fontSize      { 100     };
  bool      m_bold          { false   };
  bool      m_italic        { false   };
  bool      m_underLine     { false   };
  int       m_fontcolor     { -1      };
  COLORREF  m_colorText     { 0       };
  COLORREF  m_colorBackgnd  { 0       };
};

inline CBitmap& 
StyleButton::GetBitmap()
{
  return m_bitmap;
}
