////////////////////////////////////////////////////////////////////////
//
// File: StyleGridCtrl.cpp
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
#include "stdafx.h"
#include "StyleGridCtrl.h"
#include "StyleFonts.h"
#include "Stylecolors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// StyleGridCtrl

StyleGridCtrl::StyleGridCtrl()
{
}

StyleGridCtrl::~StyleGridCtrl()
{
}

BEGIN_MESSAGE_MAP(StyleGridCtrl, CGridCtrl)
  ON_WM_PAINT()
  ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// message handlers

void
StyleGridCtrl::InitSkin()
{
  // Test if init already done
  if(GetWindowLongPtr(GetSafeHwnd(),GWLP_USERDATA) != NULL)
  {
    return;
  }
  // Skin our grid
  SetFont(&STYLEFONTS.DialogTextFont);
  SkinScrollWnd* skin = SkinWndScroll(this,1);
  skin->SetScrollbarBias(0);
}

void
StyleGridCtrl::ResetSkin()
{
  UnskinWndScroll(this);
}

LRESULT
StyleGridCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
  if(message == WM_VSCROLL || message == WM_HSCROLL)
  {
    WORD sbCode = LOWORD(wParam);
    if (sbCode == SB_THUMBTRACK || sbCode == SB_THUMBPOSITION)
    {
      SCROLLINFO siv = { 0 };
      siv.cbSize     = sizeof(SCROLLINFO);
      siv.fMask      = SIF_ALL;
      SCROLLINFO sih = siv;
      int nPos       = HIWORD(wParam);

      GetScrollInfo(SB_VERT, &siv);
      GetScrollInfo(SB_HORZ, &sih);

      // Rather clunky: but it gets the job done
      // But this is what CGridCtrl also does: translate to single scroll actions
      if (WM_VSCROLL == message)
      {
        return SendMessage(message,nPos > siv.nPos ? SB_LINEDOWN : SB_LINEUP,0);
      }
      else
      {
        return SendMessage(message,nPos > sih.nPos ? SB_RIGHT : SB_LEFT);
      }
    }
  }
  return CGridCtrl::WindowProc(message, wParam, lParam);
}

void
StyleGridCtrl::DrawFrame()
{
  COLORREF color = ThemeColor::_Color1;
  SkinScrollWnd* skin = (SkinScrollWnd*)GetWindowLongPtr(m_hWnd,GWLP_USERDATA);
  if(skin)
  {
    skin->DrawFrame(color);
  }
}

void
StyleGridCtrl::OnPaint()
{
  if(!m_inPaint)
  {
    m_inPaint = true;
    CGridCtrl::OnPaint();
    DrawFrame();
    m_inPaint = false;
  }
}

SkinScrollWnd*
StyleGridCtrl::GetSkin()
{
  return (SkinScrollWnd*)GetWindowLongPtr(GetSafeHwnd(), GWLP_USERDATA);
}

// Propagate the ShowWindow command
void
StyleGridCtrl::OnShowWindow(BOOL bShow, UINT nStatus)
{
  SkinScrollWnd* skin = GetSkin();
  if (skin && nStatus == 0)
  {
    // nStatus == 0 means: Message sent is caused by an explict "ShowWindow" call
    skin->ShowWindow(bShow ? SW_SHOW : SW_HIDE);
  }
  else
  {
    CGridCtrl::OnShowWindow(bShow, nStatus);
  }
}

//////////////////////////////////////////////////////////////////////////
//
// SUPPORT FOR DynamicDataEXchange in Dialogs
//
//////////////////////////////////////////////////////////////////////////

void WINAPI DDX_Control(CDataExchange* pDX,int nIDC,StyleGridCtrl& p_gridControl)
{
  CWnd& control = reinterpret_cast<CWnd&>(p_gridControl);
  DDX_Control(pDX,nIDC,control);
  p_gridControl.InitSkin();
}
