////////////////////////////////////////////////////////////////////////
//
// File: StyleCalendar.h
// Function: Calandar popup in styling colors
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
#include "StyleDialog.h"
#include "resource.h"
#include <afxdtctl.h>

#define CSG_CALENDAR_LEFT_SHIFT   1
#define CSG_CALENDAR_DOWN_SHIFT   3

class StyleCalendar : public StyleDialog
{
public:
  StyleCalendar(CWnd* p_parent,CString p_select,int top,int left);
 ~StyleCalendar();

  void    RegisterFont(CFont* p_font);
  void    DoDataExchange(CDataExchange* pDX);
  BOOL    OnInitDialog();
  CString GetSelection();

  enum { IDD = IDD_CALENDAR };
  DECLARE_MESSAGE_MAP()
  afx_msg void OnDubbel(NMHDR *nMHdr,LRESULT *lResult);

  //afx_msg void 
private:
  // Do not fall of the screen
  void EdgeDetection(CRect& p_rect);

  CMonthCalCtrl m_control;
  CFont*        m_font;
  CString       m_select;
  int           m_top;
  int           m_left;
};

inline CString 
StyleCalendar::GetSelection()
{
  return m_select;
}
