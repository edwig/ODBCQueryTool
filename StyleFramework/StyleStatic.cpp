////////////////////////////////////////////////////////////////////////
//
// File: StyleStatic.cpp
// Function: Styling for the CStatic
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// StyleStatic

StyleStatic::StyleStatic()
{
}

StyleStatic::~StyleStatic()
{
}

BEGIN_MESSAGE_MAP(StyleStatic,CStatic)
  ON_WM_PAINT()
  ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

void
StyleStatic::PreSubclassWindow()
{
  ScaleControl(this);
}

/////////////////////////////////////////////////////////////////////////////
// StyleStatic message handlers

void
StyleStatic::SetLeftOffset(int p_offset)
{
  if(p_offset > 0)
  {
    m_leftOffset = WS(p_offset);
  }
}

void
StyleStatic::SetBkColor(COLORREF crColor)
{
  m_colorBackground = crColor;
  RedrawWindow();
}

void
StyleStatic::SetTextColor(COLORREF crColor)
{
  m_colorText = crColor;
  RedrawWindow();
}

COLORREF
StyleStatic::GetTextColor()
{
  return m_colorText;
}

COLORREF
StyleStatic::GetBkColor()
{
  return m_colorBackground;
}

void
StyleStatic::OnLButtonDown(UINT nFlags,CPoint point)
{
  CWnd* owner = GetOwner();
  if(owner)
  {
    owner->OnCmdMsg(GetDlgCtrlID(),STN_CLICKED,NULL,NULL);
  }
}

void
StyleStatic::OnPaint()
{
  // Do the default processing
  CStatic::Default();

  // Get the DC and the rectangle
  CDC* pDC = GetDC();
  CRect rect;
  GetClientRect(&rect);

  // Get the text1
  CString text;
  GetWindowText(text);

  // Paint the background
  DWORD background = m_colorBackground;
  if(background == (DWORD)NO_COLOR)
  {
    background = ThemeColor::GetColor(ThemeColor::Colors::ColorWindowFrame);
  }
  pDC->FillSolidRect(&rect,background);

  // Set text color
  DWORD textcolor = m_colorText;
  if(textcolor == (DWORD) NO_COLOR)
  {
    textcolor = ThemeColor::GetColor(ThemeColor::Colors::ColorEditText);
  }
  pDC->SetTextColor(textcolor);

  // Paint the text
  int fontheight = STANDARDFONTSIZE;
  CFont* org = pDC->SelectObject(&STYLEFONTS.DialogTextFont);
  
  // Use left offset
  rect.left += m_leftOffset;

  // Print the text
  pDC->DrawText(text,&rect,DT_LEFT | DT_VCENTER | DT_NOPREFIX | DT_WORDBREAK | DT_EXPANDTABS);

  // Restore originals
  pDC->SelectObject(org);
  ReleaseDC(pDC);
}
