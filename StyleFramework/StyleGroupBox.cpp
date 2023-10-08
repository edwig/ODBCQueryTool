////////////////////////////////////////////////////////////////////////
//
// File: StyleGroupBox.cpp
// Function: Styling variant of the CButton with BTN_GROUPBOX style
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
#include "StyleGroupBox.h"
#include "StyleFonts.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace ThemeColor;

IMPLEMENT_DYNAMIC(StyleGroupBox,CWnd);

StyleGroupBox::StyleGroupBox()
{
}

StyleGroupBox::~StyleGroupBox()
{
}

BEGIN_MESSAGE_MAP(StyleGroupBox,CWnd)
  ON_WM_PAINT()
END_MESSAGE_MAP()

void
StyleGroupBox::PreSubclassWindow()
{
  ScaleControl(this);
}

static HRGN SetControlClipping(HDC hdc, const RECT* rect)
{
  RECT rc = *rect;
  HRGN hrgn = CreateRectRgn(0, 0, 0, 0);

  if(GetClipRgn(hdc,hrgn) != 1)
  {
    DeleteObject(hrgn);
    hrgn = 0;
  }
  DPtoLP(hdc,(POINT*)&rc,2);
  if(GetLayout(hdc) & LAYOUT_RTL)  /* compensate for the shifting done by IntersectClipRect */
  {
    rc.left++;
    rc.right++;
  }
  IntersectClipRect(hdc, rc.left, rc.top, rc.right, rc.bottom);
  return hrgn;
}

void
StyleGroupBox::OnPaint()
{
  PAINTSTRUCT ps;
 
  CDC* dc = BeginPaint(&ps);
  Internal_Paint(dc);
  EndPaint(&ps);
}

void
StyleGroupBox::Internal_Paint(CDC* p_dc)
{
  // Text of the label
  CString text;
  GetWindowText(text);
  HFONT oldfont = (HFONT)p_dc->SelectObject(&STYLEFONTS.DialogTextFont);

  /* GroupBox acts like static control, so it sends CTLCOLORSTATIC */
  CWnd* parent = GetParent();

  HBRUSH hbr = (HBRUSH)parent->SendMessage(WM_CTLCOLORSTATIC,(WPARAM)p_dc->GetSafeHdc(),(LPARAM)GetSafeHwnd());
  if (!hbr)
  {
    /* did the app forget to call DefWindowProc ? */
    hbr = (HBRUSH)::DefWindowProcW(parent->GetSafeHwnd(),WM_CTLCOLORSTATIC,(WPARAM)p_dc->GetSafeHdc(),(LPARAM)GetSafeHwnd());
  }
  // Getting the rectangles
  CRect labelrect;
  GetClientRect(&labelrect);
  CRect rcFrame(labelrect);

  HRGN hrgn = SetControlClipping(p_dc->GetSafeHdc(),&labelrect);

  TEXTMETRIC tm;
  p_dc->GetTextMetrics(&tm);
  rcFrame.top += (tm.tmHeight / 2) + 2;
  CPen pen(PS_SOLID,1,ThemeColor::GetColor(Colors::ColorGroupLine));
  HPEN oldpen = (HPEN) p_dc->SelectObject(pen);
  p_dc->MoveTo(rcFrame.left,   rcFrame.top);
  p_dc->LineTo(rcFrame.right-1,rcFrame.top);
  p_dc->LineTo(rcFrame.right-1,rcFrame.bottom-1);
  p_dc->LineTo(rcFrame.left,   rcFrame.bottom-1);
  p_dc->LineTo(rcFrame.left,   rcFrame.top);
  p_dc->SelectObject(oldpen);

  InflateRect(&labelrect, -7,1);
  labelrect.top += 3;
  labelrect.bottom = labelrect.top + 2 + tm.tmHeight;
  CRect textrect(labelrect);
  {
     /* There is 1-pixel margin at the left, right, and bottom */
    labelrect.left -= 2;
    labelrect.bottom++;
    CPoint extent = p_dc->GetTextExtent(text);
    labelrect.right = labelrect.left + extent.x + 5;

    FillRect(p_dc->GetSafeHdc(),&labelrect,hbr);

    UINT flags = IsWindowEnabled() ? DSS_NORMAL : DSS_DISABLED;
    p_dc->DrawText(text,&textrect,flags);
  }
  if(hrgn)
  {
    SelectClipRgn(p_dc->GetSafeHdc(), hrgn);
    DeleteObject(hrgn);
  }
  p_dc->SelectObject(oldfont);
}
