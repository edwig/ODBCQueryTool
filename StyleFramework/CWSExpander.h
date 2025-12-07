////////////////////////////////////////////////////////////////////////
//
// File: CWSExpander.h
// Function: Expanding the drawing of an icon
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
#include "StyleMacros.h"

#define COLORREF_NULL (COLORREF)-1

class CWSExpander
{
public:
  CWSExpander(HWND p_hwnd,CDC* pDC,CRect rect,bool pClipRgn = true) 
              :m_hwnd(p_hwnd)
              ,m_pDC(pDC)
              ,m_rect(rect)
              ,m_ClipRgn(pClipRgn)
  {
  }

  void DrawIcon(HICON hicon,COLORREF pBackgroundColor,COLORREF color = COLORREF_NULL)
  {
    CBrush br(color == COLORREF_NULL ? RGB(0,0,0) : color);
    int iconsize = WS(m_hwnd,16);

    if(iconsize == 16)
    {
      m_pDC->DrawState(CPoint(m_rect.left - (16 - m_rect.Width()) / 2,m_rect.top - (16 - m_rect.Height()) / 2),CSize(16,16),hicon,DST_ICON | DSS_MONO,&br);
    }
    else
    {
      CDC dc;
      dc.CreateCompatibleDC(m_pDC);
      CBitmap bitmap;
      bitmap.CreateCompatibleBitmap(m_pDC,16,16);
      dc.SelectObject(bitmap);
      dc.FillSolidRect(0,0,16,16,pBackgroundColor);
      dc.DrawState(0,0,hicon,DST_ICON | DSS_MONO,&br);
      int save = m_pDC->SaveDC();

      if(m_ClipRgn)
      {
        CRgn clip;
        clip.CreateRectRgn(m_rect.left,m_rect.top,m_rect.right,m_rect.bottom);
        m_pDC->SelectClipRgn(&clip);
      }
      m_pDC->SetStretchBltMode(HALFTONE);
      m_pDC->StretchBlt(m_rect.left - (iconsize - m_rect.Width()) / 2,m_rect.top - (iconsize - m_rect.Height()) / 2,iconsize,iconsize,&dc,0,0,16,16,SRCCOPY);
      m_pDC->RestoreDC(save);
    }
  }

private:
  HWND  m_hwnd;
  CDC*  m_pDC;
  CRect m_rect;
  bool  m_ClipRgn;
};
