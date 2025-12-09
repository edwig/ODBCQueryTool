////////////////////////////////////////////////////////////////////////
//
// File: SkinScrollbar.h
// Function: Add colored vertical/horizontal scroll bars to a control
//
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

#define THUMB_BORDER	  3
#define THUMB_MINSIZE	  (THUMB_BORDER*2)

class SkinScrollBar : public CScrollBar
{
// Construction
public:
  SkinScrollBar();
  virtual ~SkinScrollBar();

  void DrawArrow(UINT uArrow, int nState);
  void SetBitmapSize(HMONITOR p_monitor = nullptr);
  BOOL IsVertical();
  
  HBITMAP	        m_hBitmap;
  int             m_nWidth;
  int             m_nHeight;

  SCROLLINFO      m_si;
  BOOL            m_bDrag;
  CPoint          m_ptDrag;
  int             m_nDragPos;

  UINT            m_uClicked;
  BOOL            m_bNotify;
  UINT            m_uHtPrev;
  BOOL            m_bPause;
  BOOL            m_bTrace;

protected:
  virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam) override;
	
  void DrawThumb(CDC *pDestDC, RECT *pDestRect, CDC *pSourDC, RECT *pSourRect);
  void TileBlt(CDC *pDestDC,RECT *pDestRect,CDC *pSourDC,RECT *pSourRect);
  RECT GetRect(UINT uSBCode);
  RECT GetImageRect(UINT uSBCode,int nState=0);
  UINT HitTest(CPoint pt);

  // Message map functions
  afx_msg void    OnSize(UINT nType, int cx, int cy);
  afx_msg void    OnPaint();
  afx_msg void    OnSetFocus(CWnd* p_wnd);
  afx_msg void    OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void    OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void    OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void    OnTimer(UINT_PTR nIDEvent);
  afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
  afx_msg LRESULT OnDpiChangedBefore(WPARAM wParam,LPARAM lParam);

  DECLARE_MESSAGE_MAP()
};

