////////////////////////////////////////////////////////////////////////
//
// File: StyleStatic.h
// Function: Extension of CStatic
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

#define STATIC_BLACK  RGB(0,0,0)
#define STATIC_WHITE  RGB(255,255,255)
#define NO_COLOR      -1

class StyleStatic: public CStatic
{
  // Construction
public:
  StyleStatic();
  virtual ~StyleStatic();
  virtual void PreSubclassWindow() override;

  void     SetLeftOffset(int p_offset);
  void     SetTextColor(COLORREF crColor);	// This Function is to set the Color for the Text.
  void     SetBkColor  (COLORREF crColor);	// This Function is to set the BackGround Color for the Text.
  COLORREF GetTextColor();
  COLORREF GetBkColor();

  // Generated message map functions
protected:

  COLORREF m_colorBackground = (DWORD) NO_COLOR;;   // Holds the Background Color for the Text
  COLORREF m_colorText       = (DWORD) NO_COLOR;;   // Holds the Color for the Text
  int      m_leftOffset { 0 };

  DECLARE_MESSAGE_MAP()
    
  afx_msg void OnPaint();
  afx_msg void OnLButtonDown(UINT nFlags,CPoint point);
};
#pragma once
