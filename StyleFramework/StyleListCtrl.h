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
#include <afxlistctrl.h>

//////////////////////////////////////////////////////////////////////////
//
// StyleHeaderCtrl
//
class StyleHeaderCtrl: public CMFCHeaderCtrl
{
  DECLARE_DYNAMIC(StyleHeaderCtrl)
public:
  StyleHeaderCtrl();
  virtual ~StyleHeaderCtrl();

  virtual void OnDrawItem(CDC* pDC,int iItem,CRect rect,BOOL bIsPressed,BOOL bIsHighlighted) override;
  virtual void OnFillBackground(CDC* pDC) override;
};

//////////////////////////////////////////////////////////////////////////
//
// StyleListCtrl
//
class StyleListCtrl : public CMFCListCtrl
{
  DECLARE_DYNAMIC(StyleListCtrl)

  // Construction
public:
	StyleListCtrl();
  virtual ~StyleListCtrl();

  void InitSkin(int p_borderSize = 1);
  void ResetSkin();
  void DrawFrame();
  SkinScrollWnd* GetSkin();
  void SetDirectInit(bool p_init);

  // Override if you want anything other than string comparisons!
  // Beware: You must use the "SetItemData(item,<something>)" to trigger this function
  virtual int OnCompareItems(LPARAM lParam1,LPARAM lParam2,int iColumn) override;

  virtual CMFCHeaderCtrl& GetHeaderCtrl() override;
  virtual COLORREF        OnGetCellTextColor(int p_row,int p_colum) override;
  virtual COLORREF        OnGetCellBkColor  (int p_row,int p_colum) override;

protected:
  StyleHeaderCtrl m_styleHeader;

  bool    m_inPaint    { false };
  bool    m_directInit { true  };

  virtual LRESULT WindowProc(UINT message,WPARAM wParam,LPARAM lParam) override;
  virtual void    PreSubclassWindow() override;

  afx_msg void OnPaint();
  afx_msg void OnSize(UINT nType,int cx,int cy);
  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
  DECLARE_MESSAGE_MAP()
};

