////////////////////////////////////////////////////////////////////////
//
// File: StyleGridCtrl.h
// Function: Styling frame for the Chris Maunder CGridCtrl grid.
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

#include "GridCtrl.h"
#include "SkinScrollWnd.h"

class StyleGridCtrl : public CGridCtrl
{
public:
  StyleGridCtrl();
  virtual ~StyleGridCtrl();

  void InitSkin();
  void ResetSkin();
  SkinScrollWnd* GetSkin();

protected:
  bool    m_inPaint { false };
  void    DrawFrame();
  void    CheckColors();

  virtual void    PreSubclassWindow() override;
  virtual LRESULT WindowProc(UINT message,WPARAM wParam,LPARAM lParam) override;

  DECLARE_MESSAGE_MAP()

  afx_msg void OnPaint();
  afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
private:
};

void WINAPI DDX_Control(CDataExchange* pDX,int nIDC,StyleGridCtrl& p_editControl);


