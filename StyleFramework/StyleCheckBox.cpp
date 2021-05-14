////////////////////////////////////////////////////////////////////////
//
// File: StyleCheckbox.cpp
// Function: StyleCheckbox and radio buttons in OUR colors
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
#include "StyleCheckbox.h"
#include "StyleColors.h"
#include "StyleMacros.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(StyleCheckbox, CButton)
  ON_WM_PAINT()
  ON_WM_KEYDOWN()
  ON_WM_LBUTTONDOWN()
  ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

StyleCheckbox::StyleCheckbox(bool p_mutable,bool p_radio,CWnd* p_field) 
              :m_mutable(p_mutable)
              ,m_radioButton(p_radio)
              ,m_field(p_field)
              ,m_error(false)
{
}

StyleCheckbox::~StyleCheckbox()
{
  DestroyWindow();
}

void
StyleCheckbox::SetErrorState(bool p_error /*= true*/)
{
  if(m_error != p_error)
  {
    m_error = p_error;
    Invalidate();
  }
}

bool
StyleCheckbox::GetErrorState()
{
  return m_error;
}

void 
StyleCheckbox::Draw(CWnd* p_wnd
                   ,CDC*  p_dc
                   ,CRect p_rect
                   ,DWORD p_style
                   ,UINT  p_state
                   ,bool  p_hover
                   ,bool  p_inError
                   ,bool  p_patternFound
                   ,COLORREF p_background
                   ,COLORREF p_outline)
{
  int save = p_dc->SaveDC();

  // Betting the colors
  COLORREF outline;
  COLORREF filling;
  COLORREF textcolor;

  if ((p_style & WS_DISABLED) != 0)
  {
    filling   = ClrControlDisabledBack;
    textcolor = ClrControlDisabledFore;
    outline   = ThemeColor::_Color2;
  }
  else
  {
    if ((p_state & BST_PUSHED) != 0)
    {
      filling   = ClrCheckboxPressed;
      textcolor = ClrCheckboxTextPressed;
      outline   = ThemeColor::_Color1;
    }
    else if ((p_state & BST_FOCUS) != 0)
    {
      filling   = ClrCheckboxFocus;
      textcolor = ClrCheckboxTextFocus;
      outline   = ThemeColor::_Color1;
    }
    else if (p_hover)
    {
      filling   = ClrCheckboxHover;
      textcolor = ClrCheckboxTextHover;
      outline   = ThemeColor::_Color1;
    }
    else
    {
      filling   = ClrCheckboxNormal;
      textcolor = ClrCheckboxTextNormal;
      outline   = ThemeColor::_Color2;
    }
  }

  if(p_outline)
  {
    outline = p_outline;
  }
  if(p_background)
  {
    filling = p_background;
  }
  if(p_inError)
  {
    outline = ClrEditFrameError;
  }

  if ((p_style & BS_TYPEMASK) == BS_RADIOBUTTON || 
      (p_style & BS_TYPEMASK) == BS_AUTORADIOBUTTON)
  {
    // Scale by a factor of 2 to enlarge, so that StretchBlt at the shrinking will do an anti-alias
    // so that the circle of the radio button will not look strange
#define FACTOR 2
    //create DC
    CDC dc;
    dc.CreateCompatibleDC(p_dc);
    // create bitmap
    CBitmap bitmap;
    CSize MySize(p_rect.Height() * FACTOR, p_rect.Height() * FACTOR);
    bitmap.CreateCompatibleBitmap(p_dc, MySize.cx, MySize.cy);
    dc.SelectObject(bitmap);
    dc.FillSolidRect(0, 0, MySize.cx, MySize.cy, ClrFrameBkGnd);

    // Draw the edge
    CPen pen;
    pen.CreatePen(PS_SOLID, p_inError ? FACTOR*2 : FACTOR, outline);
    dc.SelectObject(pen);

    CBrush br;
    br.CreateSolidBrush(filling);
    dc.SelectObject(br);

    dc.Ellipse(CRect(CPoint(0,0), MySize));

    // DrawSelection
    if ((p_state & BST_CHECKED) != 0)
    {
      CRect r(CPoint(0, 0), MySize);

      r.DeflateRect(3 * FACTOR, 3 * FACTOR);
      pen.DeleteObject();
      pen.CreatePen(PS_SOLID, 1, textcolor);
      dc.SelectObject(pen);

      br.DeleteObject();
      br.CreateSolidBrush(textcolor);
      dc.SelectObject(br);

      dc.Ellipse(r);
    }

    p_dc->SetStretchBltMode(HALFTONE);
    p_dc->StretchBlt(p_rect.left, p_rect.top, p_rect.Height(), p_rect.Height(), &dc, 0, 0, MySize.cx,MySize.cy, SRCCOPY);
  }
  else
  {
    // Style = BS_CHECKBOX
    // Draw Edge
    CBrush br;
    br.CreateSolidBrush(outline);
    CRect mark(p_rect);
    mark.right = mark.left + (mark.bottom - mark.top);


    p_dc->FrameRect(mark, &br);
    mark.DeflateRect(1, 1);
    if (p_inError)
    {
      p_dc->FrameRect(mark, &br);
      mark.DeflateRect(1, 1);
    }

    if (!p_patternFound)
    {
      // Paint the background, if we have a filling pattern
      p_dc->FillSolidRect(mark, filling);
    }

    CPen pen;
    pen.CreatePen(PS_SOLID,2,textcolor);
    p_dc->SelectObject(pen);

    if ((p_state & BST_CHECKED) != 0)
    {
      p_dc->MoveTo(mark.CenterPoint().x - WS(4), mark.CenterPoint().y - WS(1));
      p_dc->LineTo(mark.CenterPoint().x - WS(1), mark.CenterPoint().y + WS(3));
      p_dc->LineTo(mark.CenterPoint().x + WS(4), mark.CenterPoint().y - WS(4));
    }
    else if ((p_state & BST_INDETERMINATE) != 0)
    {
      mark.DeflateRect(3, 3);
      p_dc->FillSolidRect(mark, textcolor);
    }
  }

  // Paint the text
  if(p_wnd)
  {
    CString text;
    p_wnd->GetWindowText(text);
    if(!text.IsEmpty())
    {
      CPen pen;
      pen.CreatePen(PS_SOLID,1,textcolor);
      p_dc->SelectObject(pen);
      p_dc->SetBkColor(ClrCheckBoxDefaultBkGrnd);
      p_dc->SelectObject(STYLEFONTS.DialogTextFont);
      p_rect.left += WS(28);
      p_dc->DrawText(text,&p_rect,DT_VCENTER | DT_SINGLELINE);
    }
  }
  // Ready with DC
  p_dc->RestoreDC(save);
}

void
StyleCheckbox::OnPaint()
{
  if(GetUpdateRect(NULL, FALSE))
  {
    CPaintDC dc(this);

    CRect rect;
    GetClientRect(rect);

    // Remove underlying MFC implementation
    dc.FillSolidRect(rect,ClrCheckBoxDefaultBkGrnd);

    DWORD style = GetStyle();
    UINT  state = GetState();
    bool  hover = ((state & BST_HOT) != 0); 

    bool enabled = m_field ? m_mutable && IsWindowEnabled() : true;
    if (!enabled)
    {
      style |= WS_DISABLED;
    }

    COLORREF outline    = NULL;
    COLORREF background = NULL;
    Draw(this,&dc, rect, style, state, hover, m_error, false, background, outline);
  }
}

BOOL
StyleCheckbox::OnEraseBkgnd(CDC* pDC)
{
  DWORD style = GetStyle() & BS_TYPEMASK;
  if(style == BS_CHECKBOX     || 
     style == BS_RADIOBUTTON  ||
     style == BS_AUTOCHECKBOX ||
     style == BS_AUTORADIOBUTTON)
  {
    CRect rcChild;
    GetClientRect(rcChild);
    pDC->FillSolidRect(0, 0, rcChild.Width(), rcChild.Height(),ClrCheckBoxDefaultBkGrnd);
  }
  
  return TRUE;
}

void
StyleCheckbox::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  if(m_mutable)
  {
    CButton::OnKeyDown(nChar,nRepCnt,nFlags);
    if(m_radioButton)
    {
      if(nChar == VK_SPACE)
      {
        Click();
      }
    }
  }
}

void 
StyleCheckbox::OnLButtonDown( UINT nFlags, CPoint point )
{
  if(m_mutable)
  {
    CButton::OnLButtonDown(nFlags,point);
    if(m_radioButton)
    {
      Click();
      return;
    }
  }
  CButton::OnLButtonDown(nFlags, point);
}

void StyleCheckbox::Click()
{
  if(m_mutable && !m_radioButton)
  {
    SetCheck((GetCheck() + 1) % (((GetStyle() & BS_TYPEMASK) == BS_AUTO3STATE) ? 3 : 2));
  }
}

BOOL
StyleCheckbox::EnableWindow(BOOL p_enable)
{
  BOOL result = CWnd::EnableWindow(p_enable);
  Invalidate();
  return result;
}
