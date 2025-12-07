////////////////////////////////////////////////////////////////////////
//
// File: StyleRichEdit.h
// Function: Styled RTF Rich Edit control
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
#include <richedit.h>
#include <afxcmn.h>
#include <map>

using RTFFonts  = std::map<int,CString>;
using RTFSizes  = std::map<int,int>;
using RTFColors = std::map<int,int>;

// Override the CRichtEditCtrl so that we 
// can paint our own (modern-look) border around it
//
class StyleRichEdit : public CRichEditCtrl
{
  DECLARE_DYNAMIC(StyleRichEdit)

public:
  StyleRichEdit();

  // Getting the RTF text out of the control
  CString GetRTFText();
  // Setting RTF text in the control
  void    SetRTFText(CString p_text);
  // Setting text in 3 fonts, colors and sizes
  CString FormatRTFText(CString p_text);
  
  // SETTERS  
  void    SetBorderColor(int p_color);
  void    SetBorderSize(int p_size);
  void    SetFontname(int p_num,CString p_name);
  void    SetFontsize(int p_num,int p_size);
  void    SetColor(int p_num,int p_color);
  // GETTERS
  CString GetFontname(int p_num);
  int     GetFontsize(int p_num);
  int     GetColor(int p_num);

  DECLARE_MESSAGE_MAP()
protected:
  CString CreateRTFHeader();
  void    PrepareRTFText(CString& p_text);
  void    Redisplay();

  // Paint our own border
  afx_msg void    OnPaint();
  afx_msg void    OnSetFont(CFont* pFont,BOOL bRedraw);
  afx_msg void    OnHScroll(UINT nSBCode,UINT nPos,CScrollBar* pScrollBar);
  afx_msg void    OnVScroll(UINT nSBCode,UINT nPos,CScrollBar* pScrollBar);
  afx_msg LRESULT OnDpiChangedAfter(WPARAM wParam, LPARAM lParam);

private:
  // Border
  int       m_borderColor { -1 };   // Gray border
  int       m_borderSize  {  1 };   // Size of one line
  // Contents
  CString   m_unformatted;          // Original unformatted text
  // How to format the text
  int       m_factor { 0 };         // Scaling factor for sizes
  RTFFonts  m_fonts;                // All fontnames
  RTFSizes  m_sizes;                // All font sizes
  RTFColors m_colors;               // All color codes + BLACK
};

