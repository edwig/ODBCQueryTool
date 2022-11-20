////////////////////////////////////////////////////////////////////////
//
// File: StyleSpinButton.h
// Function: Styled progress control
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
#include "StyleColors.h"

class StyleEdit;

class StyleSpinButtonCtrl : public CSpinButtonCtrl
{
  DECLARE_DYNAMIC(StyleSpinButtonCtrl)

public:
  StyleSpinButtonCtrl();
  virtual ~StyleSpinButtonCtrl();

  // Sets the up-down control's buddy window.
  CWnd*   SetBuddy(StyleEdit* p_buddy);
  // Set error state
  void    SetErrorState(bool p_error = true);
  bool    GetErrroState();

protected:
  DECLARE_MESSAGE_MAP()

  afx_msg void OnPaint();
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnLButtonUp  (UINT nFlags, CPoint point);

private:
  virtual void PreSubclassWindow() override;

  void DrawTopButton(CDC* p_dc,CRect& rect,int p_offset = 0);
  void DrawBotButton(CDC* p_dc,CRect& rect,int p_offset = 0);

  StyleEdit*  m_buddy  { nullptr };
  bool        m_buttonUP { false };
  bool        m_buttonDN { false };
  bool        m_error    { false };
};

// SUPPORT FOR DynamicDataEXchange in Dialogs

void AFXAPI DDX_Control       (CDataExchange* pDX,int nIDC, StyleSpinButtonCtrl& p_control,StyleEdit* p_buddy = nullptr);
void WINAPI DDV_StyleMinMaxInt(CDataExchange* pDX,int p_IDC,CString& p_value,int p_minimum,int p_maximum);
void WINAPI DDV_StyleMinMaxInt(CDataExchange* pDX,int p_IDC,int      p_value,int p_minimum,int p_maximum);
