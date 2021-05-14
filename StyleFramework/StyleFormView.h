////////////////////////////////////////////////////////////////////////
//
// File: StyleFormView.h
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

class StyleFormView : public CFormView
{
  DECLARE_DYNAMIC(StyleFormView)

public:
  StyleFormView(LPCTSTR lpszTemplateName);
  StyleFormView(UINT nIDTemplate);
  virtual ~StyleFormView();

  void    Init(int p_borderSize = 0);
  SkinScrollWnd* GetSkin();

  afx_msg void OnPaint();
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnWindowPosChanged(WINDOWPOS* pos);

  DECLARE_MESSAGE_MAP()
};
