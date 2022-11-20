////////////////////////////////////////////////////////////////////////
//
// File: StyleTreeCtrl.cpp
// Function: Styling frame for CTreeCtrl object
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
#include "StyleTreeCtrl.h"
#include "SkinScrollWnd.h"
#include "StyleFonts.h"
#include "StyleColors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace ThemeColor;

StyleTreeCtrl::StyleTreeCtrl()
{
}

StyleTreeCtrl::~StyleTreeCtrl()
{
  ResetSkin();
  OnNcDestroy();
}

BEGIN_MESSAGE_MAP(StyleTreeCtrl, CTreeCtrl)
  ON_WM_PAINT()
  ON_WM_SIZE()
  ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// StyleTreeCtrl message handlers

void
StyleTreeCtrl::PreSubclassWindow()
{
  ScaleControl(this);

  if(m_directInit)
  {
    InitSkin();
  }
}

void
StyleTreeCtrl::InitSkin()
{
  // Tree control must have a client edge
  // otherwise the original scrollbars will show through when resizing!
  ModifyStyleEx(0, WS_EX_CLIENTEDGE);
  SetFont(&STYLEFONTS.DialogTextFont);
  SkinScrollWnd* skin = SkinWndScroll(this,TREECONTROL_BORDER,TREECONTROL_CLIENT_BIAS);
  skin->SetScrollbarBias(TREECONTROL_SCROLL_BIAS);
}

void
StyleTreeCtrl::ResetSkin()
{
  UnskinWndScroll(this);
}

SkinScrollWnd*
StyleTreeCtrl::GetSkin()
{
  return (SkinScrollWnd*)GetWindowLongPtr(GetSafeHwnd(), GWLP_USERDATA);
}

void
StyleTreeCtrl::SetDirectInit(bool p_init)
{
  m_directInit = p_init;
}

void
StyleTreeCtrl::DrawFrame()
{
  COLORREF color = (this == GetFocus()) ? ThemeColor::GetColor(Colors::AccentColor1) 
                                        : ThemeColor::GetColor(Colors::AccentColor2);
  SkinScrollWnd* skin = (SkinScrollWnd*)GetWindowLongPtr(m_hWnd, GWLP_USERDATA);
  if (skin)
  {
    skin->DrawFrame(color);
  }
}

void
StyleTreeCtrl::CheckColors()
{
  HWND hwnd = GetSafeHwnd();
  int textColor = ThemeColor::GetColor(Colors::ColorEditText);
  int backColor = ThemeColor::GetColor(Colors::ColorCtrlBackground);
  if(TreeView_GetTextColor(hwnd) != textColor)
  {
    TreeView_SetTextColor(hwnd,textColor);
  }
  if(TreeView_GetBkColor(hwnd) != backColor)
  {
    TreeView_SetBkColor(hwnd,backColor);
  }
}

void
StyleTreeCtrl::OnPaint()
{
  if(!m_inPaint)
  {
    m_inPaint = true;
    CheckColors();
    CTreeCtrl::OnPaint();
    DrawFrame();
    m_inPaint = false;
  }
}

void
StyleTreeCtrl::OnSize(UINT nType, int cx, int cy)
{
  CTreeCtrl::OnSize(nType, cx, cy);
  DrawFrame();
}

// Propagate the ShowWindow command
void
StyleTreeCtrl::OnShowWindow(BOOL bShow, UINT nStatus)
{
  SkinScrollWnd* skin = GetSkin();
  if (skin && nStatus == 0)
  {
    // nStatus == 0 means: Message sent is caused by an explict "ShowWindow" call
    skin->ShowWindow(bShow ? SW_SHOW : SW_HIDE);
  }
  else
  {
    CTreeCtrl::OnShowWindow(bShow, nStatus);
  }
}

LRESULT
StyleTreeCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
  if (message == WM_VSCROLL || message == WM_HSCROLL)
  {
    WORD sbCode = LOWORD(wParam);
    if (sbCode == SB_THUMBTRACK || sbCode == SB_THUMBPOSITION)
    {
      CRect rcClient;
      SCROLLINFO siv = { 0 };
      siv.cbSize = sizeof(SCROLLINFO);
      siv.fMask = SIF_ALL;
      SCROLLINFO sih = siv;
      int nPos = HIWORD(wParam);

      GetClientRect(&rcClient);
      GetScrollInfo(SB_VERT, &siv);
      GetScrollInfo(SB_HORZ, &sih);

      // Rather clunky: but it gets the job done
      // But this is what CGridCtrl also does: translate to single scroll actions
      if (WM_VSCROLL == message)
      {
        return SendMessage(message, nPos > siv.nPos ? SB_LINEDOWN : SB_LINEUP, 0);
      }
      else
      {
        return SendMessage(message, nPos > sih.nPos ? SB_RIGHT : SB_LEFT);
      }
    }
  }
  return CTreeCtrl::WindowProc(message, wParam, lParam);
}

