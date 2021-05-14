////////////////////////////////////////////////////////////////////////
//
// File: Checkbox.h
// Function: Checkbox and radio buttons in OUR colors
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

class StyleCheckbox : public CButton
{
public:
  // Construction
  //
  StyleCheckbox(bool p_mutable = true,bool p_radio = false,CWnd* veld = nullptr);
  virtual ~StyleCheckbox();

  void  SetMutable(bool p_mutable) { m_mutable = p_mutable; };
  void  SetErrorState(bool p_error = true);
  bool  GetErrorState();
  void  Click();

  // Handlers
  //
  BOOL  EnableWindow(BOOL p_enable = TRUE);

  afx_msg void  OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );
  afx_msg void  OnLButtonDown( UINT nFlags, CPoint point );
  afx_msg void  OnPaint();
  afx_msg BOOL  OnEraseBkgnd(CDC* pDC);

  static  void  Draw(CWnd* p_wnd,CDC* pDC,CRect rect,DWORD style,UINT state,bool hover,bool pInError = false,bool pPatternFound = false,COLORREF pBack = NULL,COLORREF pOutl = NULL);

  DECLARE_MESSAGE_MAP()

private:
  // Members
  CWnd* m_field;
  bool  m_radioButton;
  bool  m_mutable;
  bool  m_error;
};
