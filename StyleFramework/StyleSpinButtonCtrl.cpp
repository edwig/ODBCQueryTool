////////////////////////////////////////////////////////////////////////
//
// File: StyleSpinButton.cpp
// Function: Styled spin button control (Buddy is the StyleEdit)
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
#include "StyleSpinButtonCtrl.h"
#include "StyleMessageBox.h"
#include "StyleEdit.h"
#include "StyleTexts.h"

using namespace ThemeColor;

IMPLEMENT_DYNAMIC(StyleSpinButtonCtrl,CSpinButtonCtrl);

BEGIN_MESSAGE_MAP(StyleSpinButtonCtrl,CSpinButtonCtrl)
  ON_WM_PAINT()
  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

StyleSpinButtonCtrl::StyleSpinButtonCtrl()
{
}

StyleSpinButtonCtrl::~StyleSpinButtonCtrl()
{
}

void
StyleSpinButtonCtrl::PreSubclassWindow()
{
  ScaleControl(this);
}

// Sets the up-down control's buddy window.
CWnd* 
StyleSpinButtonCtrl::SetBuddy(StyleEdit* p_buddy)
{
  CWnd* previous = nullptr;
  CWnd* buddy = GetBuddy();
  if(!buddy)
  {
    previous = CSpinButtonCtrl::SetBuddy((CWnd*)p_buddy->GetSkin());
  }
  if(p_buddy)
  {
    p_buddy->SetSpinBuddy(this);
    m_buddy = p_buddy;
  }
  return previous;
}

void
StyleSpinButtonCtrl::SetErrorState(bool p_error /*=true*/)
{
  if(m_error != p_error)
  {
    m_error = p_error;
    Invalidate();
  }
}

bool
StyleSpinButtonCtrl::GetErrroState()
{
  return m_error;
}

void 
StyleSpinButtonCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
  CRect crect;
  GetClientRect(crect);
  if (crect.top + crect.Height() / 2 > point.y)
  {
    m_buttonUP = true;
  }
  else
  {
    m_buttonDN = true;
  }
  CSpinButtonCtrl::OnLButtonDown(nFlags, point);
}

void 
StyleSpinButtonCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
  m_buttonUP = false;
  m_buttonDN = false;
  CSpinButtonCtrl::OnLButtonUp(nFlags,point);
}

void 
StyleSpinButtonCtrl::DrawTopButton(CDC* dc,CRect& rect, int p_offset /*= 0*/)
{
  int height = rect.Height() / 2;
  int middle = rect.Width() / 2;
  int half   = height / 2;

  CBrush brush(m_buttonUP ? SpinButtonActive : ThemeColor::GetColor(Colors::ColorCtrlBackground)); // Assistant0);
  HGDIOBJ oldbrush = dc->SelectObject(brush);
  CRect back(rect.left + 2, rect.top + 3, rect.Width() - 3, height);
  dc->FillRect(&back,&brush);

  // Paint the button
  dc->MoveTo(rect.left  + 1, rect.top + 2);
  dc->LineTo(rect.right - 3, rect.top + 2);
  dc->LineTo(rect.right - 3, rect.top + height - 1);
  dc->LineTo(rect.left  + 1, rect.top + height - 1);
  dc->LineTo(rect.left  + 1, rect.top + 2);

  // Paint arrows
  dc->MoveTo(rect.left + middle - 1, rect.top + half - 1);
  dc->LineTo(rect.left + middle + 0, rect.top + half - 1);
  dc->MoveTo(rect.left + middle - 2, rect.top + half + 0);
  dc->LineTo(rect.left + middle + 1, rect.top + half + 0);
  dc->MoveTo(rect.left + middle - 3, rect.top + half + 1);
  dc->LineTo(rect.left + middle + 2, rect.top + half + 1);
  dc->MoveTo(rect.left + middle - 4, rect.top + half + 2);
  dc->LineTo(rect.left + middle + 3, rect.top + half + 2);

  dc->SelectObject(oldbrush);
}

void 
StyleSpinButtonCtrl::DrawBotButton(CDC* dc,CRect& rect, int p_offset /*= 0*/)
{
  int height = rect.Height() / 2;
  int middle = rect.Width() / 2;
  int half   = height / 2;

  CBrush brush(m_buttonDN ? SpinButtonActive : ThemeColor::GetColor(Colors::ColorCtrlBackground));
  HGDIOBJ oldbrush = dc->SelectObject(brush);
  CRect back(rect.left + 2, rect.top + height + 1, rect.Width() - 3, rect.bottom - 3);
  dc->FillRect(&back, &brush);

  // Paint the buttons
  dc->MoveTo(rect.left  + 1, rect.top + height);
  dc->LineTo(rect.right - 3, rect.top + height);
  dc->LineTo(rect.right - 3, rect.bottom - 3);
  dc->LineTo(rect.left  + 1, rect.bottom - 3);
  dc->LineTo(rect.left  + 1, rect.top + height);

  dc->MoveTo(rect.left + middle - 4, height + half - 3);
  dc->LineTo(rect.left + middle + 3, height + half - 3);
  dc->MoveTo(rect.left + middle - 3, height + half - 2);
  dc->LineTo(rect.left + middle + 2, height + half - 2);
  dc->MoveTo(rect.left + middle - 2, height + half - 1);
  dc->LineTo(rect.left + middle + 1, height + half - 1);
  dc->MoveTo(rect.left + middle - 1, height + half + 0);
  dc->LineTo(rect.left + middle + 0, height + half + 0);

  dc->SelectObject(oldbrush);
}

void
StyleSpinButtonCtrl::OnPaint()
{
  PAINTSTRUCT ps;
  BeginPaint(&ps);

  CRect rect;
  GetClientRect(&rect);
  CDC* dc = GetDC();

  COLORREF color = IsWindowEnabled() ? ThemeColor::GetColor(Colors::AccentColor1) : ThemeColor::GetColor(Colors::AccentColor2);
  if(m_error)
  {
    color = ColorEditFrameError;
  }

  CPen pen;
  pen.CreatePen(PS_SOLID,1,color);
  HGDIOBJ orgpen = dc->SelectObject(pen);

  // Paint the frame
  dc->MoveTo(rect.left  + 1, rect.top);
  dc->LineTo(rect.right - 1, rect.top);
  dc->LineTo(rect.right - 1, rect.bottom - 1);
  dc->LineTo(rect.left  + 1, rect.bottom - 1);
  dc->LineTo(rect.left  + 1, rect.top);

  // Draw the buttons
  DrawTopButton(dc,rect);
  DrawBotButton(dc,rect);

  dc->SelectObject(orgpen);
  ReleaseDC(dc);

  EndPaint(&ps);
}

//////////////////////////////////////////////////////////////////////////
//
// SUPPORT FOR DynamicDataEXchange in Dialogs
//
//////////////////////////////////////////////////////////////////////////

void AFXAPI DDX_Control(CDataExchange* pDX,int nIDC,StyleSpinButtonCtrl& p_control,StyleEdit* p_buddy /*=nullptr*/)
{
  CSpinButtonCtrl& control = reinterpret_cast<CSpinButtonCtrl&>(p_control);
  DDX_Control(pDX,nIDC,control);
  if(p_buddy)
  {
    p_control.SetBuddy(p_buddy);
  }
  else
  {
    StyleMessageBox(nullptr,_T("Provide for a buddy in the of the spin-button in DDX_Control!"),_T("ERROR"),MB_OK|MB_ICONERROR);
  }
}

void WINAPI
DDV_StyleMinMaxInt(CDataExchange* pDX,int nIDC,CString& p_value,int p_minimum,int p_maximum)
{
  int value = _ttoi(p_value.GetString());
  DDV_StyleMinMaxInt(pDX,nIDC,value,p_minimum,p_maximum);
}

void WINAPI 
DDV_StyleMinMaxInt(CDataExchange* pDX,int p_IDC,int p_value,int p_minimum,int p_maximum)
{
  if(p_value < p_minimum || p_maximum < p_value)
  {
    // Enter a value between %d and %d (inclusive)
    CString error;
    CString format = GetStyleText(TXT_VALUE_BETWEEN);
    error.Format(format,p_minimum,p_maximum);
    StyleMessageBox(pDX->m_pDlgWnd,error,GetStyleText(TXT_ERROR),MB_OK|MB_ICONEXCLAMATION);
    pDX->m_idLastControl = p_IDC;
    pDX->Fail();
  }
}

