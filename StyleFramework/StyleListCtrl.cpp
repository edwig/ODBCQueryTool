////////////////////////////////////////////////////////////////////////
//
// File: StyleListCtrl.cpp
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
#include "stdafx.h"
#include "StyleListCtrl.h"
#include "StyleFonts.h"
#include "StyleColors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// StyleListCtrl

StyleListCtrl::StyleListCtrl()
{
}

StyleListCtrl::~StyleListCtrl()
{
  ResetSkin();
  CWnd::OnNcDestroy();
}

BEGIN_MESSAGE_MAP(StyleListCtrl,CListCtrl)
  ON_WM_CREATE()
  ON_WM_PAINT()
  ON_WM_SIZE()
  ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

void
StyleListCtrl::PreSubclassWindow()
{
  if(m_directInit)
  {
    InitSkin();
  }
}

void
StyleListCtrl::SetDirectInit(bool p_init)
{
  m_directInit = p_init;
}

/////////////////////////////////////////////////////////////////////////////
// StyleListCtrl message handlers

void
StyleListCtrl::InitSkin(int p_borderSize /*=1*/)
{
  SetFont(&STYLEFONTS.DialogTextFont);
  SkinScrollWnd* skin = SkinWndScroll(this,p_borderSize);
  skin->SetScrollbarBias(0);
}

void
StyleListCtrl::ResetSkin()
{
  UnskinWndScroll(this);
}

SkinScrollWnd* 
StyleListCtrl::GetSkin()
{
  return (SkinScrollWnd*)GetWindowLongPtr(GetSafeHwnd(), GWLP_USERDATA);
}

LRESULT 
StyleListCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(message == WM_VSCROLL || message == WM_HSCROLL)
	{
		WORD sbCode=LOWORD(wParam);
		if(sbCode == SB_THUMBTRACK || sbCode == SB_THUMBPOSITION)
		{
      CRect rcClient;
      SCROLLINFO siv = { 0 };
			siv.cbSize = sizeof(SCROLLINFO);
			siv.fMask  = SIF_ALL;
			SCROLLINFO sih = siv;
			int nPos = HIWORD(wParam);

      GetClientRect(&rcClient);
			GetScrollInfo(SB_VERT,&siv);
			GetScrollInfo(SB_HORZ,&sih);
			
      SIZE sizeAll;
      if (sih.nPage == 0)
      {
        sizeAll.cx = rcClient.right;
      }
      else
      {
        sizeAll.cx = rcClient.right*(sih.nMax + 1) / sih.nPage;
      }
      if (siv.nPage == 0)
      {
        sizeAll.cy = rcClient.bottom;
      }
      else
      {
        sizeAll.cy = rcClient.bottom*(siv.nMax + 1) / siv.nPage;
      }

			SIZE size = { 0,0 };
			if(WM_VSCROLL == message)
			{
				size.cx=sizeAll.cx*sih.nPos/(sih.nMax+1);
				size.cy=sizeAll.cy*(nPos-siv.nPos)/(siv.nMax+1);
			}
      else
			{
				size.cx=sizeAll.cx*(nPos-sih.nPos)/(sih.nMax+1);
				size.cy=sizeAll.cy*siv.nPos/(siv.nMax+1);
			}
			Scroll(size);
			return 1;
		}
	}
	return CListCtrl::WindowProc(message, wParam, lParam);
}

void
StyleListCtrl::DrawFrame()
{
  COLORREF color = (this == GetFocus()) ? ThemeColor::_Color1 : ThemeColor::_Color2;
  SkinScrollWnd* skin = (SkinScrollWnd*)GetWindowLongPtr(m_hWnd,GWLP_USERDATA);
  if(skin)
  {
    skin->DrawFrame(color);
  }
}

void
StyleListCtrl::OnPaint()
{
  if(!m_inPaint)
  {
    m_inPaint = true;
    CListCtrl::OnPaint();
    DrawFrame();
    m_inPaint = false;
  }
}

void 
StyleListCtrl::OnSize(UINT nType,int cx,int cy)
{
  CListCtrl::OnSize(nType,cx,cy);
  DrawFrame();
}

// Propagate the ShowWindow command
void
StyleListCtrl::OnShowWindow(BOOL bShow, UINT nStatus)
{
  SkinScrollWnd* skin = GetSkin();
  if (skin && nStatus == 0)
  {
    // nStatus == 0 means: Message sent is caused by an explict "ShowWindow" call
    skin->ShowWindow(bShow ? SW_SHOW : SW_HIDE);
  }
  else
  {
    CListCtrl::OnShowWindow(bShow, nStatus);
  }
}

