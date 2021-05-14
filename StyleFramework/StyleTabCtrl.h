////////////////////////////////////////////////////////////////////////
//
// File: StyleTabCtrl.h
// Function: Styling frame for a CTabCtrl
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
#include <vector>

#define tabHeaderHeight 30
#define tabHeaderNotch  5

typedef std::vector<CRect> TabMap;

class StyleTabCtrl : public CTabCtrl
{
  DECLARE_DYNAMIC(StyleTabCtrl)

public:
  StyleTabCtrl();
  virtual ~StyleTabCtrl();

  // Insert a tab item along with a window on that tab
  LONG InsertItem(int p_item,CWnd* p_wnd,CString p_text);
  // Call Init after all tabs are inserted
  void Init();

  // SETTERS
  void SetColours(COLORREF selColour, COLORREF unselColour);
  void SetImages(UINT p_bitmap);
  void SetUseTabNotch(int p_notch);
  void SetTabImage  (int p_tab, int p_image);
  void SetErrorState(int p_item,int p_errors);
  void SetTabActive (int p_item,bool p_active);

  void SelectTab(int p_tab);

  // GETTERS
  int  GetErrorState (int p_item);
  bool GetTabActive(int p_item);

  void AdjustRect(BOOL bLarger,LPRECT lpRect);

  DECLARE_MESSAGE_MAP()

  afx_msg void   OnTcnSelchangeTabs(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg int    OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void   OnSize(UINT nType,int cx,int cy);
  afx_msg void   OnPaint();
  afx_msg BOOL   OnEraseBkgnd(CDC* pDC);
  afx_msg HBRUSH OnCtlColor(CDC* pDC,CWnd* pWnd,UINT nCtlColor);
  afx_msg LPARAM OnCtlColorStatic(WPARAM wParam,LPARAM lParam);

protected:
  virtual void PreSubclassWindow();
          void PaintError(CDC* pDC, CRect rect);

private:
  COLORREF   m_selColor;
  COLORREF   m_unselColor;
  CFont*     m_font;
  CImageList m_images;
  CBrush     m_brush;
  int        m_hover;
  bool       m_notch;
  int        m_offset;
  std::vector<int>  m_errors;
  std::vector<bool> m_active;
};


