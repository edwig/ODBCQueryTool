////////////////////////////////////////////////////////////////////////
//
// File: StyleListBox.h
// Function: Styling frame for CListBox object
//           Also does a recalculate of the widest string
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
#include "SkinScrollWnd.h"

class StyleListBox : public CListBox
{
  DECLARE_DYNAMIC(StyleListBox)

public:
  StyleListBox();
  virtual ~StyleListBox();
  
  // Styling functions
  void InitSkin(int p_borderSize = 1,int p_clientBias = 0);
  void ResetSkin();
  void SetDirectInit(bool p_init);

  void DrawFrame();
  SkinScrollWnd* GetSkin();

  // Overrides of CListBox functions
  int  AddString(LPCTSTR p_string) ;
  int  InsertString(int p_index,LPCTSTR p_string);
  void ResetContent();
  int  DeleteString(int p_number);
  void MoveWindow(int x,int y,int nWidth,int nHeight,BOOL bRepaint = TRUE);

  // Edit functions
  void SelectAll(BOOL p_select = TRUE);
  void Copy();
  void Cut();
  void Paste();
  bool Print         (CString p_documentName,CString p_printer = "",CString p_pageName = "",CString p_dateName = "",CString p_endlisting = "");
  bool PrintSelection(CString p_documentName,CString p_printer = "",CString p_pageName = "",CString p_dateName = "",CString p_endlisting = "");

  DECLARE_MESSAGE_MAP()

protected:
  virtual void PreSubclassWindow() override;

  void    UpdateWidth(LPCTSTR p_string);
  void    AdjustScroll();
  void    RemoveLineNumber(CString& p_text);

  afx_msg void OnPaint();
  afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

  SkinScrollWnd* m_skin;
  int  m_width      { 0     };
  bool m_inPaint    { false };
  bool m_directInit { true  };
};
