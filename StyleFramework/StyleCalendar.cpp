////////////////////////////////////////////////////////////////////////
//
// File: StyleCalendar.cpp
// Function: Calendar pop-up in styling colors
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
#include "StdAfx.h"
#include "StyleCalendar.h"
#include "StyleUtilities.h"
#include "StyleTexts.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

StyleCalendar::StyleCalendar(CWnd* p_parent,CString p_select,int top,int left)
              :StyleDialog(IDD_CALENDAR,p_parent)
              ,m_select(p_select)
              ,m_font(NULL)
              ,m_top(top)
              ,m_left(left)
{
}

StyleCalendar::~StyleCalendar()
{
}

void 
StyleCalendar::DoDataExchange(CDataExchange* pDX)
{
  StyleDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_MONTH,m_control);
}

BEGIN_MESSAGE_MAP(StyleCalendar,StyleDialog)
  ON_NOTIFY(MCN_SELECT,IDC_MONTH,OnDubbel)
END_MESSAGE_MAP()

BOOL
StyleCalendar::OnInitDialog()
{
  StyleDialog::OnInitDialog();
  // Select a date
  SetWindowText(GetStyleText(TXT_CALENDAR_TITLE));

  if(m_font)
  {
    m_control.SetFont(m_font);
  }
  m_control.SetColor(MCSC_MONTHBK,ThemeColor::_Color1);
  m_control.SetColor(MCSC_TEXT,   ThemeColor::_Color1);

  // Find the size and position of the window
  CRect rect;
  GetWindowRect(&rect);
  CRect newpos(m_left - CSG_CALENDAR_LEFT_SHIFT
              ,m_top  + CSG_CALENDAR_DOWN_SHIFT
              ,m_left + (rect.right  - rect.left)
              ,m_top  + (rect.bottom - rect.top));

  // Reposition the calendar control
  EdgeDetection(newpos);
  MoveWindow(newpos,TRUE);

  // Select current selection
  if(!m_select.IsEmpty())
  {
    COleDateTime dat;
    try
    {
      dat.ParseDateTime(m_select,VAR_DATEVALUEONLY);
      m_control.SetCurSel(dat);
      m_select.Empty();
    }
    catch (...)
    {
      m_select.Empty();
    }
  }
  // Set focus on first control
  return TRUE;
}

void 
StyleCalendar::RegisterFont(CFont* p_font)
{
  m_font = p_font;
}

void
StyleCalendar::OnDubbel(NMHDR* /*nMHdr*/,LRESULT *lResult)
{
  COleDateTime sel;
  m_control.GetCurSel(sel);
  if(sel.m_status == COleDateTime::valid)
  {
    m_select.Format("%2.2d-%2.2d-%4.4d",sel.GetDay(),sel.GetMonth(),sel.GetYear());
  }
  OnOK();
  *lResult = 0;
}

// Do not fall of the screen
void 
StyleCalendar::EdgeDetection(CRect& p_rect)
{
  CRect work;
  StyleGetWorkArea(this,work,p_rect.left,p_rect.bottom);

  // Move to the left?
  if(p_rect.right > work.right)
  {
    int left = p_rect.Width();
    p_rect.left  -= left;
    p_rect.right -= left;
  }
  // Move up?
  if(p_rect.bottom > work.bottom)
  {
    int up = p_rect.Height();
    p_rect.top    -= up;
    p_rect.bottom -= up;
  }
}
