////////////////////////////////////////////////////////////////////////
//
// File: StyleLIstCtrl.h
// Function: Styling frame for CListCtrl object
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

class StyleListCtrl : public CMFCListCtrl
{
// Construction
public:
	StyleListCtrl();
  virtual ~StyleListCtrl();

  void InitSkin(int p_borderSize = 1);
  void ResetSkin();
  void DrawFrame();
  SkinScrollWnd* GetSkin();
  void SetDirectInit(bool p_init);

protected:
  bool    m_inPaint    { false };
  bool    m_directInit { true  };

  virtual LRESULT WindowProc(UINT message,WPARAM wParam,LPARAM lParam) override;
  virtual void    PreSubclassWindow() override;
          void    CheckColors();

  afx_msg void OnPaint();
  afx_msg void OnSize(UINT nType,int cx,int cy);
  afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
  DECLARE_MESSAGE_MAP()
};

