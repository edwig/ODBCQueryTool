////////////////////////////////////////////////////////////////////////
//
// File: StyleTab.h
// Function: Styling frame for a tab in a StyleTabCtrl
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

class StyleTab: public CDialog
{
  DECLARE_DYNAMIC(StyleTab)

public:
  StyleTab(UINT p_IDTemplate,CWnd* p_parentWnd = NULL);
  virtual ~StyleTab();

  void SetCanResize(bool p_resize = true);
  virtual void OnDroppedFile(UINT p_id,UINT p_index,LPCTSTR p_fileName);

  // Stepper and other logic
  virtual bool InitStyleTab (void* p_data);
  virtual bool CheckStyleTab(void* p_data);
  virtual bool SaveStyleTab (void* p_data);

protected:
  virtual BOOL PreTranslateMessage(MSG* p_msg) override;
  virtual BOOL OnInitDialog() override;
  virtual void SetupDynamicLayout();

  void RegisterTooltip(int p_ID,            LPCTSTR p_text);
  void RegisterTooltip(CWnd& p_wnd,         LPCTSTR p_text);
  void RegisterTooltip(StyleComboBox& p_wnd,LPCTSTR p_text);

private:
  bool      m_error     { false  };
  bool      m_canResize { false };
  CRect     m_originalSize;
  ToolTips  m_tooltips;
  CBrush    m_defaultBrush;
  
  DECLARE_MESSAGE_MAP();

  afx_msg INT_PTR OnToolHitTest(CPoint point,TOOLINFO* pTI) const;
  afx_msg BOOL    OnToolTipNotify(UINT id,NMHDR* pNMHDR,LRESULT* pResult);
  afx_msg BOOL    OnEraseBkgnd(CDC* pDC);
  afx_msg LPARAM  OnCtlColorStatic (WPARAM wParam,LPARAM lParam);
  afx_msg LPARAM  OnCtlColorListBox(WPARAM wParam,LPARAM lParam);
  afx_msg HBRUSH  OnCtlColor(CDC* pDC,CWnd* pWnd,UINT nCtlColor);
  afx_msg LRESULT OnStyleChanged(WPARAM,LPARAM);
  afx_msg void    OnCancel();
  afx_msg void    OnOK();
};