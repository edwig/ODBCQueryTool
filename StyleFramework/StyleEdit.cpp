 ////////////////////////////////////////////////////////////////////////
//
// File: StyleEdit.cpp
// Function: Styling frame for a CEdit control
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
#include "StyleEdit.h"
#include "StyleColors.h"
#include "StyleFonts.h"
#include "StyleCalendar.h"
#include "StyleMessageBox.h"
#include "StyleComboBox.h"
#include "SkinScrollWnd.h"
#include "StyleSpinButtonCtrl.h"
#include "StyleTexts.h"
#include <winuser.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Standard MS-Windows ComboBox focus dots
bool g_StyleFxComboBoxDots = true;

IMPLEMENT_DYNAMIC(StyleEdit,CEdit)

BEGIN_MESSAGE_MAP(StyleEdit,CEdit)
  ON_WM_MOUSEMOVE()
  ON_WM_CHAR()
  ON_WM_MOVE()
  ON_WM_SIZE()
  ON_WM_PAINT()
  ON_WM_SHOWWINDOW()
  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONUP()
  ON_MESSAGE(WM_MOUSEHOVER,           OnMouseHover)
  ON_MESSAGE(WM_MOUSELEAVE,           OnMouseLeave)
  ON_MESSAGE(WM_LBUTTONDBLCLK,        OnDoubleClick)
  ON_CONTROL_REFLECT_EX(EN_KILLFOCUS, OnKillFocus)
  ON_CONTROL_REFLECT_EX(EN_SETFOCUS,  OnSetfocus)
  ON_WM_CTLCOLOR_REFLECT()

// BEWARE: Not activated
// ON_WM_WINDOWPOSCHANGED()

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSG_TextEdit

StyleEdit::StyleEdit() 
{
  // White for now
  // SetBkColor(RGB(255,255,255));
  // SetBackgroundColorEmpty(RGB(255, 255, 255));
}

StyleEdit::~StyleEdit()
{
  if(m_font)
  {
    delete m_font;
    m_font = nullptr;
  }
  // Strange but true: Only way to destroy this window
  OnNcDestroy();

  // Beware: you should use DDX_StyleEdit instead of DDX_Control or DDX_Text
  ASSERT(m_initCorrectly == true);
}

void
StyleEdit::PreSubclassWindow()
{
  if(m_directInit)
  {
    InitSkin();
  }
}

void
StyleEdit::InitSkin()
{
  // Test if init already done
  if(GetWindowLongPtr(GetSafeHwnd(),GWLP_USERDATA) != NULL)
  {
    return;
  }

  // Set read-only color
  if(!m_combo && GetStyle() & ES_READONLY)
  {
    SetBkColor(ClrFrameBkGnd);
  }

  DWORD style = GetStyle();
  DWORD exsty = GetExStyle();

  // Has a border?
  bool border = (style & WS_BORDER) || (exsty & WS_EX_CLIENTEDGE);

  // Getting the border size (if not set by SetBorderSize)
  if(m_borderSize < 0)
  {
    if ((style & WS_HSCROLL) || (style & WS_VSCROLL) || (style & ES_MULTILINE))
    {
      m_borderSize = STYLE_TEXTEDIT_BORDER;
    }
    else
    {
      m_borderSize = STYLE_SINGLELN_BORDER;
    }
  }
  SetFont(&STYLEFONTS.DialogTextFont);
  SkinWndScroll(this,border ? m_borderSize : 0);
  m_initCorrectly = true;
}

void
StyleEdit::ResetSkin()
{
  UnskinWndScroll(this);
}

// Set your own bordersize (not for multiline edit controls!!)
void 
StyleEdit::SetBorderSize(int p_border)
{
  if (p_border >= 0 && p_border <= STYLE_SINGLELN_BORDER)
  {
    m_borderSize = p_border;
  }
}

void
StyleEdit::SetDirectInit(bool p_init)
{
  m_directInit = p_init;
}

void
StyleEdit::SetMandatory(bool p_mand)
{
  m_mandatory = p_mand;
}

void
StyleEdit::SetMutable(bool p_mutable)
{
  m_mutable = p_mutable;
}

void
StyleEdit::SetNumeric(bool p_numeric /*= true*/)
{
  m_numeric = p_numeric;
}

void
StyleEdit::SetErrorState(bool p_error,LPCTSTR p_tip /*=""*/)
{
  m_error = p_error;
  SetBorderColor(p_error ? ClrEditFrameError : 0);
  if(p_error && p_tip)
  {
    StyleMessageBox(this,p_tip,GetStyleText(TXT_ERROR),MB_OK|MB_ICONERROR);
  }
  // Invalidate, so we draw/erase the exclamation mark
  Invalidate();

  if(m_buddy)
  {
    m_buddy->SetErrorState(p_error);
  }
  SkinScrollWnd* skin = GetSkin();
  if(skin)
  {
    skin->OnNcPaint();
    DrawEditFrame();
  }
}

void
StyleEdit::SetPasswordEyeColor(COLORREF p_colorPasswordEye)
{
  if(m_colorPasswordEye != p_colorPasswordEye)
  {
    m_colorPasswordEye = p_colorPasswordEye;
    DrawPasswordEye();
  }
}

/////////////////////////////////////////////////////////////////////////////
// CSG_TextEdit message handlers
void 
StyleEdit::SetTextColor(COLORREF p_colorText)
{
  if(m_colorText != p_colorText)
  {
    m_colorText = p_colorText;
    // Draw text control again
    Invalidate();
  }
}

void
StyleEdit::SetTextColorEmpty(COLORREF p_colorText)
{
  m_colorTextEmpty = p_colorText;
}

void
StyleEdit::CreateBackgroundBrush(DWORD p_color)
{
  if(m_bkBrush.m_hObject)
  {
    m_bkBrush.DeleteObject();
  }
  m_bkBrush.CreateSolidBrush(p_color);
}

void
StyleEdit::CreateBackgroundEmptyBrush(DWORD p_color)
{
  if(m_bkEmptyBrush.m_hObject)
  {
    m_bkEmptyBrush.DeleteObject();
  }
  m_bkEmptyBrush.CreateSolidBrush(p_color);
}

void 
StyleEdit::SetBkColor(COLORREF p_colorBackground)
{
  if(m_colorBackground != p_colorBackground)
  {
    m_colorBackground = p_colorBackground;
    CreateBackgroundBrush(m_colorBackground);
    // Draw text control again if we are already in business
    if(GetSafeHwnd())
    {
      Invalidate();
    }
  }
}

void
StyleEdit::SetBackgroundColorEmpty(COLORREF p_colorEmptyBackground)
{
  if(m_colorBackgroundEmpty != p_colorEmptyBackground)
  {
    m_colorBackgroundEmpty = p_colorEmptyBackground;
    CreateBackgroundEmptyBrush(p_colorEmptyBackground);
    Invalidate();
  }
}

void 
StyleEdit::SetBorderColor(COLORREF p_color)
{
  if(m_colorBorder != p_color)
  {
    m_colorBorder = p_color;
    DrawEditFrame();
  }
}

void 
StyleEdit::SetBorderColorDisabled(COLORREF p_color)
{
  if(m_colorBorderDisabled != p_color)
  {
    m_colorBorderDisabled = p_color;
    DrawEditFrame();
  }
}

void 
StyleEdit::SetBorderColorFocus(COLORREF p_color)
{
  if(m_colorBorderFocus != p_color)
  {
    m_colorBorderFocus = p_color;
    DrawEditFrame();
  }
}

void
StyleEdit::SetEmpty(bool p_empty,CString p_text)
{
  if(p_empty && !p_text.IsEmpty())
  {
    m_emptyText = p_text;
  }
  m_empty = p_empty;
  CEdit::SetWindowText(p_text);
  SetPassword(m_password);
}

bool
StyleEdit::GetReadOnly()
{
  DWORD m_style = GetStyle();
  return ((m_style & ES_READONLY) || (IsWindowEnabled() == FALSE));
}

bool
StyleEdit::GetHoverOver()
{
  return (bool)m_over;
}

CString 
StyleEdit::GetWindowString()
{
  // Beware: call *our* GetWindowString
  // and not the one from CEdit
  CString text;
  GetWindowText(text);
  return text;
}

int
StyleEdit::GetWindowNumber()
{
  // Beware: call *our* GetWindowString
  // and not the one from CEdit
  CString text;
  GetWindowText(text);
  return atoi(text);
}

int
StyleEdit::GetWindowText(LPTSTR lpszStringBuf, int p_maxCount) const
{
  CString string;
  StyleEdit::GetWindowText(string);
  strncpy_s(lpszStringBuf,p_maxCount,string.GetString(),p_maxCount);

  if(string.GetLength() > p_maxCount)
  {
    return p_maxCount;
  }
  return string.GetLength();
}

void
StyleEdit::GetWindowText(CString& p_string) const
{
  CEdit::GetWindowText(p_string);
  if(m_empty && !m_emptyText.IsEmpty())
  {
    if(p_string.CompareNoCase(m_emptyText) == 0)
    {
      p_string.Empty();
    }
  }
}

int
StyleEdit::GetWindowTextLength() const
{
  CString string;
  CEdit::GetWindowText(string);
  if (m_empty && !m_emptyText.IsEmpty())
  {
    if (string.CompareNoCase(m_emptyText) == 0)
    {
      string.Empty();
    }
  }
  return string.GetLength();
}

void 
StyleEdit::SetWindowNumber(int p_number)
{
  CString number;
  number.Format("%d",p_number);
  CEdit::SetWindowText(number);
  m_empty = false;
  Invalidate();
}

void 
StyleEdit::SetWindowText(LPTSTR lpszStringBuf)
{
  CEdit::SetWindowText(lpszStringBuf);
  if(!m_emptyText.IsEmpty())
  {
    m_empty = strlen(lpszStringBuf) == 0;
    if (m_empty)
    {
      CEdit::SetWindowText(m_emptyText);
    }
  }
  Invalidate();
}

void
StyleEdit::SetWindowText(CString p_string)
{
  CEdit::SetWindowText(p_string);
  if(!m_emptyText.IsEmpty())
  {
    m_empty = p_string.IsEmpty();
    if (m_empty)
    {
      CEdit::SetWindowText(m_emptyText);
    }
  }
  DrawFrame();
}

void 
StyleEdit::InsertAtCurPos(const char* p_string,int p_offset)
{
  CString text;
  GetWindowText(text);

  if (m_startChar != m_endChar)
  {
    text.Delete(m_startChar,m_endChar - m_startChar);
  }
  text.Insert(m_startChar,p_string);
  SetWindowText(text);

  int pos = m_startChar + ((p_offset != -1) ? p_offset : (int)strlen(p_string));

  SetWindowText(text);
  SetFocus();
  SetSel(pos, pos);
}

// Returns 0 or 1 if all checks out OK
// Allows to do a count on the correct fields
int
StyleEdit::CheckEditOK()
{
  CString text;
  GetWindowText(text);

  // See if field is in general error state
  if(m_error)
  {
    return 0;
  }
  // Check that we have a text on a mandatory field
  if((m_mandatory || m_password) && text.IsEmpty())
  {
    m_error = true;
    Invalidate();
    return 0;
  }
  if(m_checkDate && !text.IsEmpty())
  {
    // Check for a valid date
    COleDateTime date;
    if(date.ParseDateTime(text,VAR_DATEVALUEONLY) == false)
    {
      m_error = true;
      Invalidate();
      return 0;
    }
  }
  // Seems to be OK
  return 1;
}

BOOL 
StyleEdit::EnableWindow(BOOL p_enable)
{
  BOOL result = CWnd::EnableWindow(p_enable);
  bool enable = p_enable && !(GetStyle() & ES_READONLY);
  COLORREF back = enable ? RGB(255, 255, 255) : ClrEditDisabledBack;
  SetBkColor(back);
  SkinScrollWnd* skin = GetSkin();
  if(skin)
  {
    skin->SetBorderColor(back);
    skin->OnNcPaint();
  }
  if(m_buddy)
  {
    m_buddy->EnableWindow(p_enable);
  }
  DrawEditFrame();
  Invalidate();
  return result;
}

void 
StyleEdit::SetSpinBuddy(StyleSpinButtonCtrl* p_buddy)
{
  m_buddy = p_buddy;
}

void
StyleEdit::OnChar(UINT p_char,UINT p_repetitions,UINT p_flags)
{
  static CString allowed("01234567890.,eE+- ");

  if(((GetStyle() & ES_READONLY) == 0) && IsWindowEnabled() && m_numeric)
  {
    BOOL ctrl = GetAsyncKeyState(VK_CONTROL);

    if(!ctrl && (allowed.Find((char)p_char) < 0) && 
      (p_char != VK_TAB) && (p_char != VK_DELETE) && (p_char != VK_BACK))
    {
      CString error = GetStyleText(TXT_ERROR);
      CString message = GetStyleText(TXT_NUM_ONLY);  // You can only enter numbers in a numeric text field
      StyleMessageBox(this,message,error,MB_OK|MB_ICONERROR);
      return;
    }
  }

  CString text;
  if(p_char && m_empty)
  {
    if(m_password)
    {
      // Hide empty text and start a password
      CEdit::SetWindowText("");
      SendMessage(EM_SETPASSWORDCHAR,(WPARAM)EDIT_PASSWORD_CHAR,0);
    }
    else if(!m_emptyText.IsEmpty())
    {
      CEdit::GetWindowText(text);
      if(text.Compare(m_emptyText) == 0)
      {
        CEdit::SetWindowText("");
      }
    }
    m_empty = false;
    DrawEditFrame();
  }

  // DEFAULT ACTION WITH THIS CHAR
  CEdit::OnChar(p_char,p_repetitions,p_flags);

  // See if text is cleared
  CEdit::GetWindowText(text);
  if(text.IsEmpty() && !m_emptyText.IsEmpty())
  {
    m_empty = true;
    if(m_password)
    {
      SendMessage(EM_SETPASSWORDCHAR,0,0);
    }
    CEdit::SetWindowText(m_emptyText);
  }
  // Try to draw the eye
  DrawPasswordEye();
  Invalidate();
}

void
StyleEdit::SetPassword(bool p_password /* = true */)
{
  m_password = p_password;
  ResetFont();

  CString text;
  CEdit::GetWindowText(text);

  SendMessage(EM_SETPASSWORDCHAR,0,0);
  if(m_password && (m_emptyText.IsEmpty() || (!text.IsEmpty() && text.Compare(m_emptyText))))
  {
    SendMessage(EM_SETPASSWORDCHAR,(WPARAM)EDIT_PASSWORD_CHAR,0);
  }
  Invalidate();
}

void
StyleEdit::OnPaint()
{
  CEdit::OnPaint();
  DrawEditFrame();
  DrawFrame();
  DrawPasswordEye();
}

void
StyleEdit::OnComboBoxPaint()
{
  if(m_combo)
  {
    DrawEditFrame();
    DrawFrame();
  }
}

void
StyleEdit::DrawFrame()
{
  CDC* dc = GetDC();
  int  penstyle = PS_SOLID;
  bool readonly = (GetStyle() & ES_READONLY) || !IsWindowEnabled();
  COLORREF back = readonly ? UsersBackground : Assistant0;

  CRect rcItem;
  GetClientRect(&rcItem);

  // Getting the color
  DWORD color = back;
  if(m_combo)
  {
    if(m_colorBackground != FRAME_DEFAULT_COLOR)
    {
      color = m_colorBackground;
    }
    else
    {
      color = Assistant0;
      if(readonly)
      {
        color = UsersBackground;
        if (m_combo->GetDroppedState())
        {
          color = ComboBoxDropped;
        }
        else if (m_over)
        {
          color = ComboBoxActive;
        }
      }
    }
    if((m_focusDots || g_StyleFxComboBoxDots) &&
       (m_focus     || m_combo->GetDroppedState()))
    {
      dc->SetBkColor(color);
      penstyle = PS_DOT;                // Dotted line
      color    = ThemeColor::_Color1;   // In the style color
    }
  }

  if(dc)
  {
    CPen pen;
    pen.CreatePen(penstyle,1,color);
    HGDIOBJ orgpen = dc->SelectObject(pen);

    // Paint the frame
    dc->MoveTo(rcItem.left,      rcItem.top);
    dc->LineTo(rcItem.right - 1, rcItem.top);
    dc->LineTo(rcItem.right - 1, rcItem.bottom - 1);
    dc->LineTo(rcItem.left,      rcItem.bottom - 1);
    dc->LineTo(rcItem.left,      rcItem.top);

    dc->SelectObject(orgpen);
    dc->SetBkColor(back);
    ReleaseDC(dc);
  }
}

void    
StyleEdit::GetDrawFrameColor(COLORREF& p_color,int& p_bordersize,bool& p_readonly)
{
  p_color = m_colorBorder ? m_colorBorder : ThemeColor::_Color2;
  p_bordersize = 1;
  p_readonly   = !IsWindowEnabled() || (GetStyle() & ES_READONLY);

  if (m_error)
  {
    p_color = ClrEditFrameError;
    p_bordersize++;
  }
  else if ((!m_combo && p_readonly) || (m_combo && !m_combo->IsWindowEnabled()))
  {
    p_color = ThemeColor::_Color2;
  }
  else if (m_over || m_focus)
  {
    p_color = m_colorBorderFocus ? m_colorBorderFocus : ThemeColor::_Color1;
  }
}

void 
StyleEdit::DrawEditFrame()
{
  COLORREF color = FRAME_DEFAULT_COLOR;
  int bordersize = 1;
  bool  readonly = false;
  GetDrawFrameColor(color,bordersize,readonly);

  SkinScrollWnd* skin = (SkinScrollWnd*)GetWindowLongPtr(m_hWnd,GWLP_USERDATA);
  if (skin)
  {
    DWORD skinborder = readonly ? ClrFrameBkGnd : RGB(0xFF, 0xFF, 0xFF);
    if(m_combo && readonly)
    {
      if(m_combo->GetDroppedState())
      {
        skinborder = ComboBoxDropped;
      }
      else if(m_over)
      {
        skinborder = ComboBoxActive;
      }
      else
      {
        skinborder = ClrFrameBkGnd;
      }
    }
    else
    {
      if(m_colorBackground != FRAME_DEFAULT_COLOR)
      {
        skinborder = m_colorBackground;
      }
    }
    skin->SetBorderColor(skinborder);
    skin->DrawFrame(color,bordersize);
  }
  else
  {
    CEdit::OnNcPaint();
  }
  DrawPasswordEye();
  DrawErrorExclamation();
}

void
StyleEdit::DrawPasswordEye()
{
  // Only draw a password eye if we are a password field
  if(!m_password)
  {
    return;
  }

  // Getting the rectangle of the input field
  CRect rect;
  CDC* dc = this->GetDC();
  this->GetClientRect(&rect);

  // Bounding rectangle of the eye
  int top    = rect.Height() / 8;
  int left   = rect.right - rect.Height() + top;
  int bottom = rect.Height() - top;
  int right  = rect.right    - top;

  // Start/end of the eyebrow
  int startx = right;
  int starty = (bottom - top) / 2;
  int endx   = left;
  int endy   = starty;

  // Getting the color for the eye
  COLORREF color = (m_colorPasswordEye == FRAME_DEFAULT_COLOR) ? ThemeColor::_Color1 : m_colorPasswordEye;

  // Take a colored pen
  CPen pen;
  pen.CreatePen(PS_SOLID,EDIT_EYE_WEIGHT,color);
  CPen* oldpen = dc->SelectObject(&pen);

  // Draw the eyebrow
  dc->Arc(left,top,right,bottom,startx,starty,endx,endy);

  // Size of the eye circle
  int inner = EDIT_INNER_EYE;
  left   += inner;
  right  -= inner;
  top    += inner;
  bottom -= inner;

  // Draw Circle
  dc->Ellipse(left,top,right,bottom);

  // Done, restore pen and DC
  dc->SelectObject(oldpen);
  ReleaseDC(dc);
}


void
StyleEdit::DrawErrorExclamation()
{
  // Only draw it in case of an error situation
  if(!m_error)
  {
    return;
  }
  // Getting the rectangle of the input field
  CRect rect;
  CDC* dc = this->GetDC();
  if(dc)
  {
    this->GetClientRect(&rect);

    // Bounding rectangle of the eye
    rect.left = rect.right - 4;
    rect.DeflateRect(0,1,0,2);

    CString text("!");
    LOGBRUSH logbrush;
    COLORREF background = Assistant0;
    if(m_bkBrush.m_hObject)
    {
      m_bkBrush.GetLogBrush(&logbrush);
      background = logbrush.lbColor;
    }
    COLORREF oldbk = dc->SetBkColor(background);
    COLORREF oldtx = dc->SetTextColor(ClrEditFrameError);
    dc->DrawText(text, &rect, DT_CENTER|DT_VCENTER);
    dc->SetTextColor(oldtx);
    dc->SetBkColor(oldbk);
    ReleaseDC(dc);
  }
}

void 
StyleEdit::OnMouseMove(UINT nFlags, CPoint point) 
{
	TRACKMOUSEEVENT mouseEvent;
	
	mouseEvent.cbSize      = sizeof(TRACKMOUSEEVENT);
	mouseEvent.dwFlags     = TME_HOVER | TME_LEAVE;
  mouseEvent.dwHoverTime = HOVER_DEFAULT;
	mouseEvent.hwndTrack   = m_hWnd;

	_TrackMouseEvent(&mouseEvent);

  OnMouseHover(0,0);

	CEdit::OnMouseMove(nFlags, point);
}

void
StyleEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
  CRect rcItem;
  this->GetClientRect(&rcItem);

  // Only if the mouse is above the password eye do we reveal the password
  if(m_password)
  {
    // See if we hit the password eye
    if(point.y <= rcItem.bottom && point.x >= (rcItem.right - rcItem.Height()))
    {
      // Show the password while the mouse button is down
      SendMessage(EM_SETPASSWORDCHAR,0,0);
    }
    else
    {
      CString text;
      CEdit::GetWindowText(text);
      if(!m_emptyText.IsEmpty() && text.CompareNoCase(m_emptyText) == 0)
      {
        CEdit::SetWindowText("");
        SendMessage(EM_SETPASSWORDCHAR,0,0);
      }
    }
  }

  CEdit::OnLButtonDown(nFlags,point);
  DrawPasswordEye();
}

void
StyleEdit::OnLButtonUp(UINT nFlags,CPoint point)
{
  CRect rcItem;
  this->GetClientRect(&rcItem);

  if(m_password)
  {
    CString text;
    CEdit::GetWindowText(text);
    if(text.CompareNoCase(m_emptyText))
    {
      // Re-hide the password
      SendMessage(EM_SETPASSWORDCHAR,(WPARAM)EDIT_PASSWORD_CHAR,0);
      Invalidate();
    }
  }
  CEdit::OnLButtonUp(nFlags, point);
  DrawPasswordEye();
}

BOOL 
StyleEdit::OnKillFocus() 
{
  GetSel(m_startChar,m_endChar);
  CString text;

  m_focus = FALSE;
  if(m_mandatory)
  {
    CString tip;
    CEdit::GetWindowText(text);
    if(text.IsEmpty())
    {
      // "Mandatory field **must** be filled in!"
      tip = GetStyleText(TXT_MANDATORY_FLD);
    }
    SetErrorState(text.IsEmpty(),tip);
  }
  if(m_checkDate)
  {
    GetWindowText(text);
    if(!text.IsEmpty())
    {
      COleDateTime date;
      bool result = date.ParseDateTime(text,VAR_DATEVALUEONLY);
      SetErrorState(!result);
    }
  }

  // Reset the empty text state
  CEdit::GetWindowText(text);
  if(text.IsEmpty() && !m_emptyText.IsEmpty())
  {
    m_empty = true;
    if(m_password)
    {
      SendMessage(EM_SETPASSWORDCHAR,0,0);
    }
    CEdit::SetWindowText(m_emptyText);
    Invalidate();
  }
  if(m_combo)
  {
    m_combo->Invalidate();
  }
  DrawEditFrame();
  DrawFrame();
  // Let the parent process the message
  return FALSE;
}

BOOL 
StyleEdit::OnSetfocus() 
{
  m_focus = TRUE;
  if(m_empty)
  {
    CEdit::SetWindowText("");
    SetSel(0,0,TRUE);
    Invalidate();
  }
  if(m_combo)
  {
    m_combo->Invalidate();
  }
  DrawEditFrame();
  DrawFrame();

  // Let the parent process the message!
  return FALSE;
}

LRESULT 
StyleEdit::OnMouseHover(WPARAM wParam,LPARAM lParam)
{
	m_over = TRUE;
  if(m_combo)
  {
    if(GetReadOnly())
    {
      SetCursor(LoadCursor(NULL, IDC_ARROW));
    }
    m_combo->Invalidate();
    Invalidate();
  }
	DrawEditFrame();
	return 0;
}

LRESULT 
StyleEdit::OnMouseLeave(WPARAM wParam,LPARAM lParam)
{
	m_over = FALSE;
  if(m_combo)
  {
    m_combo->Invalidate();
    Invalidate();
  }
  DrawEditFrame();
  return 0;
}

LRESULT
StyleEdit::OnDoubleClick(WPARAM wParam, LPARAM lParam)
{
  // Do we have the calendar style?
  if(m_calendar == false)
  {
    return 0;
  }
  // May we edit the field?
  if((GetStyle() & ES_READONLY) || IsWindowEnabled() == false)
  {
    return 0;
  }
  // Use the position of the current control
  // Or use the position of the given control
  int bottom = 0;
  int left   = 0;
  int border = STYLE_TEXTEDIT_BORDER;
  if(wParam == 1)
  {
    RECT rect;
    GetWindowRect(&rect);

    left   = rect.left   - border - 2;
    bottom = rect.bottom + border;
  }
  else
  {
    left   = (int)wParam + border - 1;
    bottom = (int)lParam - border - 1;
  }

  // Start the control
  CString text;
  GetWindowText(text);

  StyleCalendar cal(this,text,bottom,left);
  cal.RegisterFont(m_font);

  if(cal.DoModal() == IDOK)
  {
    // Retrieve the selected date
    CString date = cal.GetSelection();
    if(m_empty && text.Compare(m_emptyText))
    {
      SetEmpty(false);
    }
    CEdit::SetWindowText(date);
    SetErrorState(false,"");
    Invalidate();
    // Set-off the content checks by sending a KILLFOCUS notify
    CWnd* dialog = GetSkin()->GetParent();
    dialog->PostMessage(WM_COMMAND,MAKELONG(GetDlgCtrlID(),EN_KILLFOCUS),(LPARAM)GetSafeHwnd());
  }
  return 0;
}

HBRUSH 
StyleEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
  // Find readonly status
  bool readonly = false;
  if (!this->IsWindowEnabled() || (GetStyle() & ES_READONLY))
  {
    readonly = true;
  }

  // Find background color
  DWORD background = ClrEditBkgnd;
  if(readonly)
  {
    background = ClrFrameBkGnd;
  }
  if(m_combo && readonly)
  {
    if(m_combo->GetDroppedState())
    {
      background = ComboBoxDropped;
    }
    else if(m_over)
    {
      background = ComboBoxActive;
    }
    else
    {
      background = UsersBackground;
    }
  }
  else
  {
    if (m_colorBackground != FRAME_DEFAULT_COLOR)
    {
      background = m_colorBackground;
    }
  }

  // Find foreground text color
  DWORD foreground = ClrEditText;
  if(m_colorText != FRAME_DEFAULT_COLOR)
  {
    foreground = m_colorText;
  }

  // Change attributes of the DC here
  if(m_empty && !readonly)
  {
    pDC->SetTextColor(m_colorTextEmpty);
    pDC->SetBkColor(m_colorBackgroundEmpty);
    CreateBackgroundEmptyBrush(m_colorBackgroundEmpty);
    return m_bkEmptyBrush;
  }
	pDC->SetTextColor(foreground);
	pDC->SetBkColor  (background);
  CreateBackgroundBrush(background);

  // Return a non-NULL brush if the parent's handler should not be called
	return m_bkBrush;
}

void 
StyleEdit::ResetFont()
{
  LOGFONT  lgFont;

  lgFont.lfCharSet        = m_language;
  lgFont.lfClipPrecision  = 0;
  lgFont.lfEscapement     = 0;
  strcpy_s(lgFont.lfFaceName,LF_FACESIZE,m_fontName);
  lgFont.lfHeight         = m_fontSize;
  lgFont.lfItalic         = m_italic;
  lgFont.lfOrientation    = 0;
  lgFont.lfOutPrecision   = 0;
  lgFont.lfPitchAndFamily = 2;
  lgFont.lfQuality        = 0;
  lgFont.lfStrikeOut      = 0;
  lgFont.lfUnderline      = m_underLine;
  lgFont.lfWidth          = 0;
  lgFont.lfWeight         = m_bold ? FW_BOLD : FW_MEDIUM;

  // Create new font or remove old object from it
  if(m_font)
  {
    if(m_font->m_hObject)
    {
      m_font->DeleteObject();
    }
  }
  else
  {
    m_font = new CFont();
  }
  // Create new font and set it to this control
  m_font->CreatePointFontIndirect(&lgFont);
  SetFont(m_font);
}

void 
StyleEdit::SetFontSize(int p_size)
{
  m_fontSize = p_size;
  ResetFont();
}

void 
StyleEdit::SetFontStyle(bool p_bold,bool p_italic,bool p_underLine)
{
  m_bold       = p_bold;
  m_italic     = p_italic;
  m_underLine  = p_underLine;
  ResetFont();
}

void 
StyleEdit::SetFontName(CString p_fontName,int p_fontSize,BYTE p_language)
{
  m_fontName = p_fontName;
  m_fontSize = p_fontSize;
  m_language = p_language;
  ResetFont();
}

void
StyleEdit::SetFocusDots(bool p_dots)
{
  m_focusDots = p_dots;
}

bool
StyleEdit::GetFocusDots()
{
  return m_focusDots;
}

BOOL
StyleEdit::SetWindowPos(const CWnd* pWndInsertAfter,int x,int y,int cx,int cy,UINT nFlags)
{
  SkinScrollWnd* skin = (SkinScrollWnd*)GetWindowLongPtr(GetSafeHwnd(),GWLP_USERDATA);
  if(skin)
  {
    return skin->SetWindowPos(pWndInsertAfter,x,y,cx,cy,nFlags);
  }
  else
  {
    return CEdit::SetWindowPos(pWndInsertAfter,x,y,cx,cy,nFlags);
  }
}

SkinScrollWnd*
StyleEdit::GetSkin()
{
  return (SkinScrollWnd*)GetWindowLongPtr(GetSafeHwnd(),GWLP_USERDATA);
}

// Propagate the ShowWindow command
void
StyleEdit::OnShowWindow(BOOL bShow, UINT nStatus)
{
  SkinScrollWnd* skin = GetSkin();
  if (skin && nStatus == 0)
  {
    skin->ShowWindow(bShow ? SW_SHOW : SW_HIDE);
  }
  else
  {
    CEdit::OnShowWindow(bShow, nStatus);
  }
}

void
StyleEdit::OnMove(int x, int y)
{
  CEdit::OnMove(x, y);

  SkinScrollWnd* skin = GetSkin();
  if (skin && skin->m_moveClient == FALSE)
  {
    // Must we move the skin frame?
    CRect client;
    skin->CalculateControlSize(client);
    CRect frame;
    skin->GetWindowRect(&frame);
    ::OffsetRect(&frame, -frame.left, -frame.top);

    // Move if we must.
    if (client.left != x && client.top != y)
    {
      frame.right  = x + frame.Width();
      frame.bottom = y + frame.Height();
      frame.left   = x;
      frame.top    = y;

      skin->MoveWindow(frame.left, frame.top, frame.Width(), frame.Height());
      skin->RepositionControlWindow();
    }
  }
}

void
StyleEdit::OnSize(UINT nType, int cx, int cy)
{
  CEdit::OnSize(nType,cx,cy);

  SkinScrollWnd* skin = GetSkin();
  if (skin && skin->m_moveClient == FALSE)
  {
    // Must we resize the skin frame?
    CRect client;
    skin->CalculateControlSize(client);
    CRect frame;
    skin->GetWindowRect(&frame);
    ::OffsetRect(&frame, -frame.left, -frame.top);

    // Resize if we must. 
    if (client.Width() != cx || client.Height() != cy)
    {
      frame.right  = frame.left + cx;
      frame.bottom = frame.top  + cy;

      skin->MoveWindow(frame.left, frame.top, frame.Width(), frame.Height());
      skin->RepositionControlWindow();
    }
  }
}

void
StyleEdit::OnWindowPosChanged(WINDOWPOS* pos)
{
  CEdit::OnWindowPosChanged(pos);

  SkinScrollWnd* skin = GetSkin();
  if(skin && skin->m_moveClient == FALSE)
  {
    bool mustMove = false;
    bool canMove  = (pos->flags & SWP_NOMOVE) == 0;
    bool canSize  = (pos->flags & SWP_NOSIZE) == 0;

    if(canMove || canSize)
    {
      // Must we resize the skin frame?
      CRect client;
      skin->CalculateControlSize(client);
      CRect frame;
      skin->GetWindowRect(&frame);
      ::OffsetRect(&frame,-frame.left,-frame.top);

      if(canMove && canSize)
      {
        // Full resize of the window. All positions different from the message
        if(pos->x  != client.left    || pos->y  != client.top ||
           pos->cx != client.Width() || pos->cy != client.Height())
        {
          mustMove     = true;
          frame.left   = pos->x;
          frame.top    = pos->y;
          frame.right  = pos->x + pos->cx;
          frame.bottom = pos->y + pos->cy;
        }
      }
      else if (canMove || pos->x || pos->y)
      {
        // Move if we must. But do NOT rely on the cx and cy of the message
        if(client.left != pos->x && client.top != pos->y)
        {
          mustMove     = true;
          frame.right  = pos->x + frame.Width();
          frame.bottom = pos->y + frame.Height();
          frame.left   = pos->x;
          frame.top    = pos->y;
        }
      }
      else if (canSize || pos->cx || pos->cy)
      {
        // Resize if we must. But do NOT rely on the x and y of the message
        if(client.Width() != pos->cx || client.Height() != pos->cy)
        {
          mustMove     = true;
          frame.right  = frame.left + pos->cx;
          frame.bottom = frame.top  + pos->cy;
        }
      }
      // Now resize the skinning frame if we must
      if(mustMove)
      {
        skin->MoveWindow(frame.left,frame.top,frame.Width(),frame.Height());
        skin->RepositionControlWindow();
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
// SUPPORT FOR DynamicDataEXchange in Dialogs
//
//////////////////////////////////////////////////////////////////////////

void WINAPI DDX_Control(CDataExchange* pDX,int nIDC,StyleEdit& p_editControl)
{
  StyleMessageBox(nullptr,"Use one of the DDX_StyleEdit functions!","ERROR",MB_OK|MB_ICONERROR);
  // Keep on running on empty
  CString empty;
  DDX_Control(pDX,nIDC,p_editControl,empty);
}

void WINAPI DDX_Control(CDataExchange* pDX,int nIDC,StyleEdit& p_editControl,CString& p_text)
{
  CEdit& edit = reinterpret_cast<CEdit&>(p_editControl);
  DDX_Control(pDX,nIDC,edit);
  p_editControl.SetInitCorrectly();
  if(pDX->m_bSaveAndValidate)
  {
    p_editControl.GetWindowText(p_text);
  }
  else
  {
    p_editControl.SetWindowText(p_text);
    if(p_editControl.GetIsPassword())
    {
      p_editControl.SetPassword(true);
    }
  }
}

void WINAPI DDX_Control(CDataExchange* pDX,int nIDC,StyleEdit& p_editControl,int& p_number)
{
  CString text;
  CEdit& edit = reinterpret_cast<CEdit&>(p_editControl);
  DDX_Control(pDX,nIDC,edit);
  p_editControl.SetInitCorrectly();
  if(pDX->m_bSaveAndValidate)
  {
    p_editControl.GetWindowText(text);
    p_number = atoi(text);
  }
  else
  {
    text.Format("%d", p_number);
    p_editControl.SetWindowText(text);
    if(p_editControl.GetIsPassword())
    {
      p_editControl.SetPassword(true);
    }
  }
}

void WINAPI DDX_Control(CDataExchange* pDX,int nIDC,StyleEdit& p_editControl,double& p_number)
{
  CString text;
  CEdit& edit = reinterpret_cast<CEdit&>(p_editControl);
  DDX_Control(pDX,nIDC,edit);
  p_editControl.SetInitCorrectly();
  if(pDX->m_bSaveAndValidate)
  {
    p_editControl.GetWindowText(text);
    p_number = atof(text);
  }
  else
  {
    text.Format("%.2f", p_number);
    p_editControl.SetWindowText(text);
    if(p_editControl.GetIsPassword())
    {
      p_editControl.SetPassword(true);
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
// SUPPORT FOR Standard MS-Windows combobox styles
//
//////////////////////////////////////////////////////////////////////////


// Global functions to set your combobox style
void  
SetStyleFxComboBoxDots(bool p_dots /*= true*/)
{
  g_StyleFxComboBoxDots = p_dots;
}

bool  
GetStyleFxComboBoxDots()
{
  return g_StyleFxComboBoxDots;
}

//////////////////////////////////////////////////////////////////////////
//
// SUPPORT FOR DynamicDataEXchange in Dialogs
//
//////////////////////////////////////////////////////////////////////////

AFX_STATIC void AFXAPI StyleFailMinMaxWithFormat(CDataExchange* pDX,StyleEdit& p_control,
                                                 LONGLONG minVal,LONGLONG maxVal,LPCTSTR lpszFormat,UINT nIDPrompt)
                                                 // error string must have '%1' and '%2' strings for min and max values
                                                 // since minVal and maxVal are 64-bit, lpszFormat should be "%I64d" or "%I64u"
{
  ASSERT(lpszFormat != NULL);

  if(!pDX->m_bSaveAndValidate)
  {
    // don't stop now
    return;
  }

  const int MINMAX_BUFFER_SIZE = 64;
  TCHAR szMin[MINMAX_BUFFER_SIZE];
  TCHAR szMax[MINMAX_BUFFER_SIZE];

  ATL_CRT_ERRORCHECK_SPRINTF(_sntprintf_s(szMin,_countof(szMin),_countof(szMin) - 1,lpszFormat,minVal));
  ATL_CRT_ERRORCHECK_SPRINTF(_sntprintf_s(szMax,_countof(szMax),_countof(szMax) - 1,lpszFormat,maxVal));

  CString prompt;
  AfxFormatString2(prompt,nIDPrompt,szMin,szMax);
  StyleMessageBox(&p_control,prompt,"Error",MB_OK|MB_ICONEXCLAMATION);
  p_control.SetFocus();
}

void AFXAPI DDV_MinMaxShort(CDataExchange* pDX,StyleEdit& p_control,short value,short minVal,short maxVal)
{
  ASSERT(minVal <= maxVal);
  if(value < minVal || value > maxVal)
  {
    StyleFailMinMaxWithFormat(pDX,p_control,minVal,maxVal,"%I64d",AFX_IDP_PARSE_INT_RANGE);
  }
}

void AFXAPI DDV_MinMaxInt(CDataExchange* pDX,StyleEdit& p_control,int value,int minVal,int maxVal)
{
  ASSERT(minVal <= maxVal);
  if(value < minVal || value > maxVal)
  {
    StyleFailMinMaxWithFormat(pDX,p_control,minVal,maxVal,"%I64d",AFX_IDP_PARSE_INT_RANGE);
  }
}

void AFXAPI DDV_MinMaxLong(CDataExchange* pDX,StyleEdit& p_control,long value,long minVal,long maxVal)
{
  ASSERT(minVal <= maxVal);
  if(value < minVal || value > maxVal)
  {
    StyleFailMinMaxWithFormat(pDX,p_control,minVal,maxVal,"%I64d",AFX_IDP_PARSE_INT_RANGE);
  }
}

void AFXAPI DDV_MinMaxUint(CDataExchange* pDX,StyleEdit& p_control,UINT value,UINT minVal,UINT maxVal)
{
  ASSERT(minVal <= maxVal);
  if(value < minVal || value > maxVal)
  {
    StyleFailMinMaxWithFormat(pDX,p_control,minVal,maxVal,"%I64d",AFX_IDP_PARSE_INT_RANGE);
  }
}

void AFXAPI DDV_MinMaxLongLong(CDataExchange* pDX,StyleEdit& p_control,LONGLONG value,LONGLONG minVal,LONGLONG maxVal)
{
  ASSERT(minVal <= maxVal);
  if(value < minVal || value > maxVal)
  {
    StyleFailMinMaxWithFormat(pDX,p_control,minVal,maxVal,"%I64d",AFX_IDP_PARSE_INT_RANGE);
  }
}

void AFXAPI DDV_MinMaxDouble(CDataExchange* pDX,StyleEdit& p_control,double value,double minVal,double maxVal)
{
  ASSERT(minVal <= maxVal);
  if(value < minVal || value > maxVal)
  {
    StyleFailMinMaxWithFormat(pDX,p_control,(LONGLONG)minVal,(LONGLONG)maxVal,"%I64d",AFX_IDP_PARSE_INT_RANGE);
  }
}
