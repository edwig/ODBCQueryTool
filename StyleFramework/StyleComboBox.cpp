////////////////////////////////////////////////////////////////////////
//
// File: StyleComboBox.cpp
// Function: ComboBox with colored scrollbars in our style colors
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
#include "stdafx.h"
#include "StyleComboBox.h"
#include "StyleUtilities.h"
#include "Grid\GridCellCombo.h"
#include <algorithm>
#include <atlconv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace ThemeColor;

IMPLEMENT_DYNAMIC(StyleComboBox,CEdit)

StyleComboBox::StyleComboBox()
{
}

StyleComboBox::~StyleComboBox()
{
  // Prevent a OnCloseup
  m_buttonDown = false;

  if(m_itemControl)
  {
    m_itemControl->ResetSkin();
    delete m_itemControl;
    m_itemControl = nullptr;
  }
  if(m_listControl)
  {
    m_listControl->ResetSkin();
    delete m_listControl;
    m_listControl = nullptr;
  }
}

BEGIN_MESSAGE_MAP(StyleComboBox,CEdit)
  // General messages
  ON_WM_SIZE()
  ON_WM_PAINT()
  ON_WM_CHAR()
  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONDBLCLK()
  ON_WM_SETFOCUS()
  ON_WM_MOUSEMOVE()
  ON_WM_MOUSELEAVE()
  ON_WM_ERASEBKGND()
  ON_WM_SETTEXT()
  ON_WM_GETTEXT()
  ON_WM_GETTEXTLENGTH()
  // ComboBox messages
  ON_MESSAGE(CB_ADDSTRING,            OnAddString)
  ON_MESSAGE(CB_DELETESTRING,         OnDeleteString)
  ON_MESSAGE(CB_FINDSTRING,           OnFindString)
  ON_MESSAGE(CB_FINDSTRINGEXACT,      OnFindStringExact)
  ON_MESSAGE(CB_GETCOMBOBOXINFO,      OnGetComboBoxInfo)
  ON_MESSAGE(CB_GETCOUNT,             OnGetCount)
  ON_MESSAGE(CB_GETCUEBANNER,         OnGetCueBanner)
  ON_MESSAGE(CB_GETCURSEL,            OnGetCurSel)
  ON_MESSAGE(CB_GETDROPPEDCONTROLRECT,OnGetDroppedControlRect)
  ON_MESSAGE(CB_GETDROPPEDSTATE,      OnGetDroppedState)
  ON_MESSAGE(CB_GETDROPPEDWIDTH,      OnGetDroppedWidth)
  ON_MESSAGE(CB_GETEDITSEL,           OnGetEditSel)
  ON_MESSAGE(CB_GETEXTENDEDUI,        OnGetExtendedUI)
  ON_MESSAGE(CB_GETHORIZONTALEXTENT,  OnGetHorizontalExtent)
  ON_MESSAGE(CB_GETITEMDATA,          OnGetItemData)
  ON_MESSAGE(CB_GETITEMHEIGHT,        OnGetItemHeight)
  ON_MESSAGE(CB_GETLBTEXT,            OnGetLBText)
  ON_MESSAGE(CB_GETLBTEXTLEN,         OnGetLBTextLen)
  ON_MESSAGE(CB_GETLOCALE,            OnGetLocale)
  ON_MESSAGE(CB_GETMINVISIBLE,        OnGetMinVisible)
  ON_MESSAGE(CB_GETTOPINDEX,          OnGetTopIndex)
  ON_MESSAGE(CB_INITSTORAGE,          OnInitStorage)
  ON_MESSAGE(CB_INSERTSTRING,         OnInsertString)
  ON_MESSAGE(CB_LIMITTEXT,            OnLimitText)
  ON_MESSAGE(CB_RESETCONTENT,         OnResetContent)
  ON_MESSAGE(CB_SELECTSTRING,         OnSelectString)
  ON_MESSAGE(CB_SETCUEBANNER,         OnSetCueBanner)
  ON_MESSAGE(CB_SETCURSEL,            OnSetCurSel)
  ON_MESSAGE(CB_SETDROPPEDWIDTH,      OnSetDroppedWidth)
  ON_MESSAGE(CB_SETEDITSEL,           OnSetEditSel)
  ON_MESSAGE(CB_SETEXTENDEDUI,        OnSetExtendedUI)
  ON_MESSAGE(CB_SETHORIZONTALEXTENT,  OnSetHorizontalExtent)
  ON_MESSAGE(CB_SETITEMDATA,          OnSetItemData)
  ON_MESSAGE(CB_SETITEMHEIGHT,        OnSetItemHeight)
  ON_MESSAGE(CB_SETLOCALE,            OnSetLocale)
  ON_MESSAGE(CB_SETMINVISIBLE,        OnSetMinVisible)
  ON_MESSAGE(CB_SETTOPINDEX,          OnSetTopIndex)
  ON_MESSAGE(CB_SHOWDROPDOWN,         OnShowDropDown)
  ON_MESSAGE(WM_GETTEXT,              OnGetText)
  ON_MESSAGE(WM_GETTEXTLENGTH,        OnGetTextLength)
  /* NOTIFICATIONS TO THE PARENT 
  CBN_ERRSPACE      // NEVER SENT
  WM_COMPAREITEM    // WE NEVER ASK THE OWNER
  WM_DRAWITEM       // WE NEVER ASK THE OWNER
  WM_MEASUREITEM    // WE NEVER ASK THE OWNER
  // END OF THE NOTIFICATIONS */
END_MESSAGE_MAP()

void 
StyleComboBox::PreSubclassWindow()
{
  SetFont(&STYLEFONTS.DialogTextFont);
  ScaleControl(this);

  // Remove default box and list
  // Getting the default combobox implementation
  COMBOBOXINFO info;
  memset(&info,0,sizeof(COMBOBOXINFO));
  info.cbSize  = sizeof(COMBOBOXINFO);
  ::GetComboBoxInfo(m_hWnd,&info);

  // Remove the extra parts 
  if(info.hwndItem && info.hwndItem != m_hWnd)
  {
    ::DestroyWindow(info.hwndItem);
  }
  if(info.hwndList)
  {
    ::DestroyWindow(info.hwndList);
  }

  // Owner draw combo boxes get other dimensions, so correct these
  // Comboboxes are slightly lower than edit boxes, but when we draw them
  // as CEdit with a StyleFrame, the tend to be too low!
  CRect rect;
  ::GetWindowRect(info.hwndCombo,&rect);
  CWnd* parent = GetParent();
  if(parent)
  {
    parent->ScreenToClient(rect);
  }
  int height = (rect.Height() * 13) / 11;

  // When size factors are set, apply these as well.
  if(GetSFXSizeFactor() != 100)
  {
    height = (height * GetSFXSizeFactor()) / 100;
  }
  ::MoveWindow(info.hwndCombo,rect.left,rect.top,rect.Width(),height,TRUE);
}

void
StyleComboBox::InitSkin()
{
  // Check if already initialized
  if(m_itemControl)
  {
    return;
  }
  // Create our own sub-controls!
  CreateEditControl();
  CreateListControl();
}

void
StyleComboBox::CreateEditControl()
{
  // Getting the rect for the edit box
  CRect rect;
  GetWindowRect(&rect);
  rect.OffsetRect(-rect.left,-rect.top);

  // Record for the width of the list (skinned and all)
  m_minimalDropWidth = rect.Width();

  // Space for the combobox button on the right
  rect.right -= rect.Height() - 1;

  DWORD cbstyle = GetStyle();
  DWORD style   = WS_CHILD | WS_VISIBLE;
  DWORD styleEx = 0; // WS_EX_CLIENTEDGE;

  style |= (cbstyle & CBS_DROPDOWNLIST) == CBS_DROPDOWNLIST ? ES_READONLY : 0;
  style |= (cbstyle & CBS_AUTOHSCROLL)  ? ES_AUTOHSCROLL : 0;
  style |= (cbstyle & CBS_UPPERCASE)    ? ES_UPPERCASE   : 0;
  style |= (cbstyle & CBS_LOWERCASE)    ? ES_LOWERCASE   : 0;

  // Set default type buffer for
  m_typebuffer = ((cbstyle & CBS_DROPDOWNLIST) == CBS_DROPDOWNLIST);

  // Create the edit control
  m_itemControl = new SCBTextEdit();
  m_itemControl->SetDirectInit(false);
  m_itemControl->CreateEx(styleEx,_T("EDIT"),_T(""),style,rect,this,0);
  m_itemControl->ModifyStyle(0,WS_BORDER);
  m_itemControl->SetBorderSize(3);
  m_itemControl->SetComboBox(this);
  m_itemControl->InitSkin();
  m_itemControl->SetInitCorrectly();
  m_itemControl->SetAutoComplete();

  // Trigger the NCCALCSIZE procedure
  m_itemControl->SetWindowPos(&CWnd::wndTop,rect.left,rect.top,rect.Width(),rect.Height(),SWP_FRAMECHANGED);
}

void 
StyleComboBox::CreateListControl()
{
  CRect rect(0,0,0,0);
  DWORD cbstyle = GetStyle();
  DWORD style   = LBS_OWNERDRAWFIXED | LBS_NOINTEGRALHEIGHT | WS_VSCROLL | WS_POPUP | LBS_NOTIFY | LBS_HASSTRINGS;
  DWORD styleEx = WS_EX_TOOLWINDOW   | WS_EX_TOPMOST;

  if(cbstyle & CBS_OWNERDRAWVARIABLE)
  {
    // BEWARE: We do NOT support owner draw style controls from here!
    ASSERT(FALSE);
  }

  // Propagate the combo box styles to the list control
  style |= cbstyle & CBS_SORT           ? LBS_SORT        : 0;
  style |= cbstyle & CBS_AUTOHSCROLL    ? WS_HSCROLL      : 0;
  style |= cbstyle & CBS_OWNERDRAWFIXED ? LBS_EXTENDEDSEL : 0;

  m_listControl = new SCBListBox();
  m_listControl->SetStyleComboBox(this);
  m_listControl->SetDirectInit(false);
  if(((style & LBS_EXTENDEDSEL) > 0) || m_multiselect)
  {
    style |= LBS_EXTENDEDSEL;
    m_listControl->SetMultiSelect(m_multiselect = true);
  }
  m_listControl->CreateEx(styleEx,_T("LISTBOX"),_T(""),style,rect,CWnd::FromHandle(::GetDesktopWindow()),0);
  m_listControl->InitSkin();
  if(m_listControl->GetSkin())
  {
    m_listControl->GetSkin()->SetScrollbarBias(0);
    m_listControl->GetSkin()->SetMouseCapture(TRUE,TME_HOVER);
    m_listControl->GetSkin()->SkinSetMouseTracking();
  }
}

// Needs to be called when created dynamic and not through a resource *.rc file
// Call directly after InitSkin()
void
StyleComboBox::ReadjustDynamic()
{
  for(CWnd* wnd = GetWindow(GW_CHILD); wnd != NULL; wnd = wnd->GetNextWindow(GW_HWNDNEXT))
  {
    if(wnd != m_itemControl)
    {
      wnd->DestroyWindow();
      break;
    }
  }
}

void
StyleComboBox::SetAutoComplete(bool p_auto /*= true*/)
{
  if(m_itemControl)
  {
    m_itemControl->SetAutoComplete(p_auto);
  }
}

bool
StyleComboBox::GetAutoComplete()
{
  if(m_itemControl)
  {
    return m_itemControl->GetAutoComplete();
  }
  return false;
}

void
StyleComboBox::SetMultiSelect(bool p_multi)
{
  m_listControl->SetMultiSelect(m_multiselect = p_multi);
}

bool
StyleComboBox::GetMultiSelect()
{
  return m_listControl->GetMultiSelect();
}

void  
StyleComboBox::SetTypeBuffer(bool p_buffer /*= true*/)
{
  m_typebuffer = p_buffer;
}

bool
StyleComboBox::GetTypeBuffer()
{
  return m_typebuffer;
}

void
StyleComboBox::SetMultiSelection(int p_index, bool p_select)
{
  m_listControl->SetMultiSelection(p_index,p_select);
}

bool
StyleComboBox::GetMultiSelection(int p_index)
{
  return m_listControl->GetMultiSelection(p_index);
}

void
StyleComboBox::SetErrorState(bool p_error,bool p_propagate)
{
  if(m_error != p_error)
  {
    m_error = p_error;
    if(p_propagate)
    {
      m_itemControl->SetErrorState(p_error);
    }
  }
}

bool
StyleComboBox::GetErrorState()
{
  return m_error;
}

void  
StyleComboBox::SetFocusDots(bool p_dots)
{
  m_itemControl->SetFocusDots(p_dots);
}

bool
StyleComboBox::GetFocusDots()
{
  return m_itemControl->GetFocusDots();
}

void  
StyleComboBox::SelectAll(bool p_check)
{
  m_listControl->SelectAll(p_check);
}

bool
StyleComboBox::GetCheck(int p_index)
{
  return m_listControl->GetMultiSelection(p_index);
}

void
StyleComboBox::SetCheck(int p_index,bool p_check)
{
  m_listControl->SetMultiSelection(p_index,p_check);
  SetEditSelection();
}

void  
StyleComboBox::InsertAtCurPos(LPCTSTR p_text,int p_offset)
{
  m_itemControl->InsertAtCurPos(p_text,p_offset);
}

void 
StyleComboBox::PreShowComboList()
{
  // Provide your own override if necessary
}

void
StyleComboBox::ShowComboList()
{
  int width  = FindOptimalWidth();
  int height = FindOptimalHeight();

  PositionDropList(width,height);

  // Record current selection and start browsing
  FindSelectionFromEdit();

  // Show listbox and start mouse capture/tracking
  SetCursor(LoadCursor(NULL,IDC_ARROW));
}

void 
StyleComboBox::PostShowComboList()
{
  // Provide your own override if necessary
  m_listControl->SetFocus();
}

void StyleComboBox::PreHideComboList() 
{
  // Provide your own override if necessary
}

void
StyleComboBox::HideComboList()
{
  if(m_itemControl && m_listControl)
  {
    SkinScrollWnd* skin = m_listControl->GetSkin();
    skin->ShowWindow(SW_HIDE);
    m_itemControl->SetFocus();
  }
}

void 
StyleComboBox::PostHideComboList() 
{
  // Provide your own override if necessary
}

int
StyleComboBox::FindOptimalWidth()
{
  // Finding the standard width
  CRect rect;
  GetWindowRect(&rect);
  int  width    = rect.Width();
  int  orgMax   = width * m_maxWidthPercentage / 100;
  bool expanded = false;

  CDC* dc = m_listControl->GetDC();
  int count = m_listControl->GetCount();
  for (int index = 0; index < count; ++index)
  {
    CString itemtext;
    GetLBText(index, itemtext);
    CSize size = dc->GetTextExtent(itemtext);
    if (size.cx > width)
    {
      width = size.cx;
      expanded = true;
    }
  }
  ReleaseDC(dc);

  // Calculate the max expansion of the drop down list
  if (expanded)
  {
    if(width > orgMax)
    {
      width = orgMax;
    }
    else
    {
      // Various fonts need a 10 percent wider space
      // Why? We cannot figure that out, but hey! it works!
      width = (width * 110) / 100;
    }
  }
  return width;
}

int
StyleComboBox::FindOptimalHeight()
{
  CRect rc2;
  this->GetDroppedControlRect(&rc2);
  int height = rc2.Height();
  int lineHeight = GetItemHeight(0);

  if(lineHeight * GetCount() < rc2.Height())
  {
    height = lineHeight * GetCount() + 2;
  }
  else
  {
    int visible = height / lineHeight;
    height = lineHeight * visible + 2;
  }
  return height;
}

void 
StyleComboBox::PositionDropList(int p_width,int p_height)
{
  CRect rc;
  GetWindowRect(&rc);

  SkinScrollWnd* skin = m_listControl->GetSkin();

  // Find workarea and see if we must open below or above the edit field
  CRect wa;
  StyleGetWorkArea(this, wa);
  if((rc.bottom - 1 + p_height > wa.bottom) && (rc.top > wa.top + wa.Height() / 2))
  {
    // List above the edit box
    skin->SetWindowPos(0,rc.left,rc.top - p_height + 1,p_width,p_height,SWP_NOREDRAW|SWP_NOACTIVATE);
  }
  else
  {
    // List fits in space below the edit box
    skin->SetWindowPos(0,rc.left,rc.bottom - 1,p_width,p_height,SWP_NOREDRAW | SWP_NOACTIVATE);
  }
  m_listControl->ModifyStyle(0,WS_VISIBLE);
  skin->SkinSetCapture();
  skin->SkinSetMouseTracking();
  skin->ShowWindow(SW_SHOW);
}

// Position the selected rows in the list
// based on the text in the edit box
void 
StyleComboBox::FindSelectionFromEdit()
{
  CString text;
  m_itemControl->GetWindowText(text);

  if(m_listControl->GetMultiSelect())
  {
    m_listControl->SetCurrentMultiSelect(text);
  }
  else
  {
    // Single selection list box
    int index = m_listControl->FindStringExact(0,text);
    if(index >= 0)
    {
      m_listControl->SetCurSel(index);
    }
    m_listControl->SaveCurSel();
  }
}

// Reset the contents of the edit box
// after we are done with the list
// and send the appropriate messages
void 
StyleComboBox::SetEditSelection()
{
  CString text;

  if(m_listControl->GetMultiSelect())
  {
    // Semi-colon separated list of selections
    text = m_listControl->GetCurrentMultiSelect();
  }
  else
  {
    int item = m_listControl->GetCurSel();
    if (item >= 0)
    {
      int len = m_listControl->GetTextLen(item);
      text.GetBufferSetLength(len + 1);
      m_listControl->GetText(item,(LPTSTR)text.GetString());
      text.ReleaseBuffer();
    }
  }
  // Tell it to the owner
  CWnd* owner = GetOwner();
  if(owner)
  {
    ::SendMessage(owner->GetSafeHwnd(),WM_COMMAND,MAKELONG(GetDlgCtrlID(),CBN_EDITUPDATE),(LPARAM)m_hWnd);
  }

  // HERE COMES THE UPDATE
  m_itemControl->SetWindowText(text);

  // Tell it to the owner
  if(owner)
  {
    ::SendMessage(owner->GetSafeHwnd(),WM_COMMAND,MAKELONG(GetDlgCtrlID(),CBN_EDITCHANGE),(LPARAM)m_hWnd);
  }
}

//////////////////////////////////////////////////////////////////////////
//
// HANDELING ALL COMBOBOX MESSAGES
// CALLING THE MFC COUNTERPARTS BELOW!
//
//////////////////////////////////////////////////////////////////////////

LRESULT
StyleComboBox::OnAddString(WPARAM wParam,LPARAM lParam)
{
  return AddString((LPCTSTR)lParam);
}

LRESULT 
StyleComboBox::OnDeleteString(WPARAM wParam,LPARAM lParam)
{
  return DeleteString((UINT)wParam);
}

LRESULT 
StyleComboBox::OnFindString(WPARAM wParam,LPARAM lParam)
{
  return FindString((int)wParam,(LPCTSTR)lParam);
}

LRESULT 
StyleComboBox::OnFindStringExact(WPARAM wParam,LPARAM lParam)
{
  return FindStringExact((int)wParam,(LPCTSTR)lParam);
}

LRESULT 
StyleComboBox::OnGetComboBoxInfo(WPARAM wParam,LPARAM lParam)
{
  return GetComboBoxInfo((PCOMBOBOXINFO)lParam);
}

LRESULT 
StyleComboBox::OnGetCount(WPARAM wParam,LPARAM lParam)
{
  if(wParam || lParam)
  {
    return CB_ERR;
  }
  return GetCount();
}

LRESULT 
StyleComboBox::OnGetCueBanner(WPARAM wParam,LPARAM lParam)
{
  return GetCueBanner((LPTSTR)wParam,(int)lParam);
}

LRESULT 
StyleComboBox::OnGetCurSel(WPARAM wParam,LPARAM lParam)
{
  if(wParam || lParam)
  {
    return CB_ERR;
  }
  return GetCurSel();
}

LRESULT 
StyleComboBox::OnGetDroppedControlRect(WPARAM wParam,LPARAM lParam)
{
  return GetDroppedControlRect((LPRECT)lParam);
}

LRESULT 
StyleComboBox::OnGetDroppedState(WPARAM wParam,LPARAM lParam)
{
  if(wParam || lParam)
  {
    return CB_ERR;
  }
  return GetDroppedState();
}

LRESULT 
StyleComboBox::OnGetDroppedWidth(WPARAM wParam,LPARAM lParam)
{
  if(wParam || lParam)
  {
    return CB_ERR;
  }
  return GetDroppedWidth();
}

LRESULT 
StyleComboBox::OnGetEditSel(WPARAM wParam, LPARAM lParam)
{
  DWORD pos = GetEditSel();
  if (wParam)
  {
    *((DWORD*)wParam) = pos & 0x0FF;
  }
  if (lParam)
  {
    *((DWORD*)lParam) = (pos & 0xFF00) >> 16;
  }
  return pos;
}

LRESULT 
StyleComboBox::OnGetExtendedUI(WPARAM wParam, LPARAM lParam)
{
  if(wParam || lParam)
  {
    return FALSE;
  }
  return GetExtendedUI();
}

LRESULT 
StyleComboBox::OnGetHorizontalExtent(WPARAM wParam, LPARAM lParam)
{
  if(wParam || lParam)
  {
    return FALSE;
  }
  return GetHorizontalExtent();
}

LRESULT 
StyleComboBox::OnGetItemData(WPARAM wParam, LPARAM lParam)
{
  return GetItemData((int)wParam);
}

LRESULT 
StyleComboBox::OnGetItemHeight(WPARAM wParam, LPARAM lParam)
{
  int height = GetItemHeight((int)wParam);
  return (height * GetSFXSizeFactor()) / 100;
}

LRESULT 
StyleComboBox::OnGetLBText(WPARAM wParam, LPARAM lParam)
{
  return GetLBText((int)wParam, (LPTSTR)lParam);
}

LRESULT 
StyleComboBox::OnGetLBTextLen(WPARAM wParam, LPARAM lParam)
{
  return GetLBTextLen((int)wParam);
}

LRESULT 
StyleComboBox::OnGetLocale(WPARAM wParam, LPARAM lParam)
{
  return GetLocale();
}

LRESULT 
StyleComboBox::OnGetMinVisible(WPARAM wParam, LPARAM lParam)
{
  return GetMinVisible();
}

LRESULT 
StyleComboBox::OnGetTopIndex(WPARAM wParam, LPARAM lParam)
{
  if(wParam || lParam)
  {
    return FALSE;
  }
  return GetTopIndex();
}

LRESULT 
StyleComboBox::OnInitStorage(WPARAM wParam, LPARAM lParam)
{
  return InitStorage((int)wParam,(UINT)lParam);
}

LRESULT 
StyleComboBox::OnInsertString(WPARAM wParam, LPARAM lParam)
{
  return InsertString((int)wParam,(LPCTSTR)lParam);
}

LRESULT 
StyleComboBox::OnLimitText(WPARAM wParam, LPARAM lParam)
{
  return LimitText((int)wParam);
}

LRESULT 
StyleComboBox::OnResetContent(WPARAM wParam, LPARAM lParam)
{
  if (wParam || lParam)
  {
    return FALSE;
  }
  ResetContent();
  return CB_OKAY;
}

LRESULT 
StyleComboBox::OnSelectString(WPARAM wParam, LPARAM lParam)
{
  return SelectString((int)wParam,(LPCTSTR)lParam);
}

LRESULT 
StyleComboBox::OnSetCueBanner(WPARAM wParam, LPARAM lParam)
{
  if(wParam)
  {
    return FALSE;
  }
  return SetCueBanner((LPCTSTR)wParam);
}

LRESULT 
StyleComboBox::OnSetCurSel(WPARAM wParam, LPARAM lParam)
{
  return SetCurSel((int)wParam);
}

LRESULT 
StyleComboBox::OnSetDroppedWidth(WPARAM wParam, LPARAM lParam)
{
  return SetDroppedWidth((int)wParam);
}

LRESULT 
StyleComboBox::OnSetEditSel(WPARAM wParam, LPARAM lParam)
{
  int start = LOWORD(lParam);
  int end   = HIWORD(lParam);
  return SetEditSel(start,end);
}

LRESULT 
StyleComboBox::OnSetExtendedUI(WPARAM wParam, LPARAM lParam)
{
  return SetExtendedUI((BOOL)wParam);
}

LRESULT 
StyleComboBox::OnSetHorizontalExtent(WPARAM wParam, LPARAM lParam)
{
  SetHorizontalExtent((UINT)wParam);
  return TRUE;
}

LRESULT 
StyleComboBox::OnSetItemData(WPARAM wParam, LPARAM lParam)
{
  return SetItemData((int)wParam,(DWORD_PTR)lParam);
}

LRESULT 
StyleComboBox::OnSetItemHeight(WPARAM wParam, LPARAM lParam)
{
  return SetItemHeight((int)wParam,(UINT)lParam);
}

LRESULT 
StyleComboBox::OnSetLocale(WPARAM wParam, LPARAM lParam)
{
  return SetLocale((LCID)wParam);
}

LRESULT 
StyleComboBox::OnSetMinVisible(WPARAM wParam, LPARAM lParam)
{
  if(lParam)
  {
    return FALSE;
  }
  return SetMinVisibleItems((int)wParam);
}

LRESULT 
StyleComboBox::OnSetTopIndex(WPARAM wParam, LPARAM lParam)
{
  return SetTopIndex((int)wParam);
}

LRESULT 
StyleComboBox::OnShowDropDown(WPARAM wParam, LPARAM lParam)
{
  ShowDropDown((BOOL)wParam);
  return TRUE;
}

// By adding this message handler, we may use CWnd::GetText()
LRESULT 
StyleComboBox::OnGetText(WPARAM wParam,LPARAM lParam)
{
  CString text;
  m_itemControl->GetWindowText(text);
  lstrcpyn((LPTSTR)lParam,text,(INT)wParam);
  return text.GetLength();
}

// By adding this message handler, we may use CWnd::GetTextLength()
LRESULT 
StyleComboBox::OnGetTextLength(WPARAM wParam,LPARAM lParam)
{
  CString text;
  m_itemControl->GetWindowText(text);
  return text.GetLength();
}

void
StyleComboBox::OnChar(UINT nChar,UINT nRepCnt,UINT nFlags)
{
  if((nChar == VK_TAB) && m_itemControl)
  {
    m_itemControl->SendMessage(WM_CHAR,VK_TAB,nFlags);
  }
  else
  {
    if(m_buttonDown)
    {
      if(m_listControl)
      {
        m_listControl->SetFocus();
        m_listControl->PostMessage(WM_KEYDOWN,nChar,nFlags);
        m_listControl->PostMessage(WM_CHAR,   nChar,nFlags);
        m_listControl->PostMessage(WM_KEYUP,  nChar,nFlags);
        return;
      }
    }
    else
    {
      if(m_itemControl)
      {
        m_itemControl->SetFocus();
        m_itemControl->OnChar(nChar,nRepCnt,nFlags);
        return;
      }
    }
  }
  CEdit::OnChar(nChar,nRepCnt,nFlags);
}

//////////////////////////////////////////////////////////////////////////
//
// COMBOBOX CONTROL NOTIFICATIONS TO THE PARENT
//
//////////////////////////////////////////////////////////////////////////

void
StyleComboBox::OnCloseup(bool p_force /*= false*/)
{
  if(m_buttonDown || p_force)
  {
    // Set as false direct, HideComboList might trigger OnActivate...
    m_buttonDown = false;

    // Hide the implementation
    PreHideComboList();
    HideComboList();
    PostHideComboList();

    // Tell it to the owner
    CWnd* owner = GetOwner();
    if(owner)
    {
      // Take actions for closeup in the owner dialog
      ::SendMessage(owner->GetSafeHwnd(),WM_COMMAND,MAKELONG(GetDlgCtrlID(),CBN_CLOSEUP),(LPARAM)m_hWnd);
    }
    Invalidate();
    m_itemControl->Invalidate();
  }
}

void
StyleComboBox::OnDropdown()
{
  if(m_buttonDown)
  {
    return;
  }

  // Prepare the list control (to eat the lbuttonup)
  m_listControl->SetActive();

  // Tell it to the owner
  CWnd* owner = GetOwner();
  if(owner)
  {
    ::SendMessage(owner->GetSafeHwnd(),WM_COMMAND,MAKELONG(GetDlgCtrlID(),CBN_DROPDOWN),(LPARAM)m_hWnd);
  }

  // Show the list
  PreShowComboList();
  ShowComboList();
  PostShowComboList();

  m_buttonDown = true;
}

void
StyleComboBox::OnSelEndOK(bool p_force /* = false*/)
{
  if(m_buttonDown || p_force)
  {
    if(!p_force)
    {
      // Set selection in the edit box
      SetEditSelection();
    }
    // Tell it to the owner
    CWnd* owner = GetOwner();
    if(owner)
    {
      ::SendMessage(owner->GetSafeHwnd(),WM_COMMAND,MAKELONG(GetDlgCtrlID(),CBN_SELENDOK),(LPARAM)m_hWnd);
    }
  }
}

void
StyleComboBox::OnSelEndCancel()
{
  if(m_buttonDown)
  {
    if((GetStyle() & CBS_DROPDOWNLIST) == CBS_DROPDOWNLIST)
    {
      // Set selection in the edit box
      SetEditSelection();
    }
    // Go cancel the selection list / closeup will follow
    CWnd* owner = GetOwner();
    if(owner)
    {
      ::SendMessage(owner->GetSafeHwnd(),WM_COMMAND,MAKELONG(GetDlgCtrlID(),CBN_SELENDCANCEL),(LPARAM)m_hWnd);
    }
  }
}

void
StyleComboBox::OnLButtonDown(UINT nFlags,CPoint point)
{
  if(nFlags & MK_LBUTTON)
  {
    // No Control keys are down
    CRect rect;
    GetClientRect(&rect);
    if(rect.PtInRect(point))
    {
      if(m_buttonDown)
      {
        OnCloseup();
      }
      else
      {
        OnDropdown();
      }
    }
  }
}

void    
StyleComboBox::OnLButtonDblClk(UINT nFlags, CPoint point)
{
  OnLButtonUp(nFlags,point);
  m_listControl->DrawFrame();
}

void
StyleComboBox::OnSetFocus(CWnd* pOldWnd)
{
  m_focus = true;
  if((m_itemControl && pOldWnd != m_itemControl) &&
     (m_listControl && pOldWnd != m_listControl) &&
     GetSafeHwnd())
  {
    CWnd* owner = GetOwner();
    if(owner)
    {
      ::SendMessage(owner->GetSafeHwnd(),WM_COMMAND,MAKELONG(GetDlgCtrlID(),CBN_SETFOCUS),(LPARAM)m_hWnd);
    }
  }
  if(m_listControl && m_listControl->IsWindowVisible())
  {
    m_listControl->SetFocus();
  }
  else if(m_itemControl && m_itemControl->GetSafeHwnd())
  {
    m_itemControl->SetFocus();
  }
}

void
StyleComboBox::OnKillFocus()
{
  CWnd* owner = GetOwner();
  if(owner)
  {
    ::SendMessage(owner->GetSafeHwnd(),WM_COMMAND,MAKELONG(GetDlgCtrlID(),CBN_KILLFOCUS),(LPARAM)m_hWnd);
  }
  m_focus = false;
}

// Pass on a LBN_SELCHANGE as a CBN_SELCHANGE
void
StyleComboBox::OnLbnSelChange()
{
  CWnd* owner = GetOwner();
  if(owner)
  {
    ::SendMessage(owner->GetSafeHwnd(),WM_COMMAND,MAKELONG(GetDlgCtrlID(),CBN_SELCHANGE),(LPARAM)m_hWnd);
  }
}

void
StyleComboBox::OnEditChange()
{
  CWnd* owner = GetOwner();
  if(owner)
  {
    ::SendMessage(owner->GetSafeHwnd(),WM_COMMAND,MAKELONG(GetDlgCtrlID(),CBN_EDITCHANGE),(LPARAM)m_hWnd);
  }
}

void
StyleComboBox::OnEditUpdate()
{
  if((GetStyle() & CBS_DROPDOWNLIST) == CBS_DROPDOWNLIST)
  {
    // Set selection in the edit box
    SetEditSelection();
  }
  CWnd* owner = GetOwner();
  if(owner)
  {
    ::SendMessage(owner->GetSafeHwnd(),WM_COMMAND,MAKELONG(GetDlgCtrlID(),CBN_EDITUPDATE),(LPARAM)m_hWnd);
  }
}

// Pass on a LBN_DBLCLK as s CBN_DBLCLK
void
StyleComboBox::OnLbnDblClk()
{
  CWnd* owner = GetOwner();
  if(owner)
  {
    ::SendMessage(owner->GetSafeHwnd(),WM_COMMAND,MAKELONG(GetDlgCtrlID(),CBN_DBLCLK),(LPARAM)m_hWnd);
  }
}

void    
StyleComboBox::OnMouseMove(UINT nFlags, CPoint point)
{
  CRect rect;
  GetClientRect(&rect);
  rect.left = rect.right - rect.Height();
  if(rect.PtInRect(point))
  {
    SetCursor(LoadCursor(NULL, IDC_ARROW));

  	TRACKMOUSEEVENT mouseEvent;
	  mouseEvent.cbSize      = sizeof(TRACKMOUSEEVENT);
	  mouseEvent.dwFlags     = TME_LEAVE;
	  mouseEvent.dwHoverTime = HOVER_DEFAULT;
	  mouseEvent.hwndTrack   = m_hWnd;
	  _TrackMouseEvent(&mouseEvent);

    m_itemControl->OnMouseHover(0,0);
  }
  CEdit::OnMouseMove(nFlags,point);
}

void
StyleComboBox::OnMouseLeave()
{
  m_itemControl->OnMouseLeave(0,0);
}

void
StyleComboBox::OnSize(UINT nType, int cx, int cy)
{
  CEdit::OnSize(nType,cx,cy);

  if(m_itemControl)
  {
    // Getting the rect for the edit box
    CRect rect;
    GetWindowRect(&rect);
    rect.OffsetRect(-rect.left, -rect.top);

    // Record for the width of the list (skinned and all)
    m_minimalDropWidth = rect.Width();

    // Space for the combobox button on the right
    rect.right -= rect.Height() - 1;

    SkinScrollWnd* frame = m_itemControl->GetSkin();
    if(frame)
    {
      frame->MoveWindow(&rect);
    }
    else
    {
      m_itemControl->MoveWindow(&rect);
    }
  }
}

BOOL
StyleComboBox::OnEraseBkgnd(CDC* pdc)
{
  // No background erase
  return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//
// MFC FUNCTIONS FOR THE MESSAGES
// ALSO CALLED BY THE OnMessage METHODS ABOVE
//
//////////////////////////////////////////////////////////////////////////

int   
StyleComboBox::AddString(LPCTSTR lpszString)
{
  int index = -1;
  if(m_listControl)
  {
    index = m_listControl->AddString(lpszString);
    int height = GetItemHeight(-1);
    if(height != CB_ERR)
    {
      m_listControl->SetItemHeight(index,height);
    }
  }
  return index;
}

int
StyleComboBox::DeleteString(UINT nIndex)
{
  if(m_listControl)
  {
    return m_listControl->DeleteString(nIndex);
  }
  return -1;
}

int
StyleComboBox::FindString(int nStartAfter,LPCTSTR lpszString) const
{
  if(nStartAfter < -1)
  {
    nStartAfter = -1;
  }
  int len = (int) _tcslen(lpszString);
  CString text;
  for(int ind = 0;ind < m_listControl->GetCount();++ind)
  {
    m_listControl->GetText(ind,text);
    if(text.Left(len).CompareNoCase(lpszString) == 0)
    {
      return ind;
    }
  }
  return -1;
}

int   
StyleComboBox::FindStringExact(int nIndexStart,LPCTSTR lpszFind) const
{
  CString text;
  for(int ind = 0;ind < m_listControl->GetCount(); ++ind)
  {
    m_listControl->GetText(ind,text);
    if(text.CompareNoCase(lpszFind) == 0)
    {
      return ind;
    }
  }
  return -1;
}

BOOL  
StyleComboBox::GetComboBoxInfo(PCOMBOBOXINFO pcbi) const
{
  if(pcbi->cbSize != sizeof(PCOMBOBOXINFO))
  {
    return FALSE;
  }
  pcbi->hwndCombo   = GetSafeHwnd();
  pcbi->hwndList    = m_listControl ? m_listControl->GetSafeHwnd() : nullptr;
  pcbi->hwndItem    = m_itemControl ? m_itemControl->GetSafeHwnd() : nullptr;
  pcbi->stateButton = m_buttonDown ? STATE_SYSTEM_PRESSED : 0;

  pcbi->rcItem   = CRect(0,0,0,0);
  pcbi->rcButton = CRect(0,0,0,0);
  if(m_itemControl)
  {
    m_itemControl->GetWindowRect(&pcbi->rcItem);
    pcbi->rcButton = pcbi->rcItem;
    pcbi->rcButton.left = pcbi->rcItem.right - (pcbi->rcItem.bottom - pcbi->rcItem.top);
  }
  return TRUE;
}

int
StyleComboBox::GetCount() const
{
  return m_listControl->GetCount();
}

BOOL 
StyleComboBox::GetCueBanner(LPTSTR lpszText,int cchText) const
{
  CString text = m_itemControl->GetEmptyText();
  if(text.GetLength() <= cchText)
  {
    _tcscpy_s(lpszText,cchText,text.GetString());
    return TRUE;
  }
  return FALSE;
}

int
StyleComboBox::GetCurSel() const
{
  return m_listControl->GetCurSel();
}

BOOL
StyleComboBox::GetDroppedControlRect(LPRECT lprect) const
{
  int itemHeight = m_listControl->GetItemHeight(0);
  int height = m_minimum * itemHeight;
  GetWindowRect(lprect);
  lprect->top     = lprect->bottom;
  lprect->bottom  = lprect->top + height;
  return TRUE;
}

BOOL  
StyleComboBox::GetDroppedState() const
{
  return (BOOL)m_buttonDown;
}

bool  
StyleComboBox::GetHasFocus() const
{
  return m_focus || m_itemControl->GetHasFocus();
}

int
StyleComboBox::GetDroppedWidth() const
{
  return m_minimalDropWidth;
}

DWORD 
StyleComboBox::GetEditSel() const
{
  return m_itemControl->GetSel();
}

BOOL  
StyleComboBox::GetExtendedUI() const
{
  return m_extendedUI;
}

UINT  
StyleComboBox::GetHorizontalExtent() const
{
  return m_listControl->GetHorizontalExtent();
}

DWORD_PTR 
StyleComboBox::GetItemData(int nIndex) const
{
  ItemStore::const_iterator it = m_items.find(nIndex);
  if(it != m_items.end())
  {
    return it->second;
  }
  return (DWORD_PTR)CB_ERR;
}

void*
StyleComboBox::GetItemDataPtr(int nIndex) const
{
  if(m_listControl)
  {
    return m_listControl->GetItemDataPtr(nIndex);
  }
  return nullptr;
}

int
StyleComboBox::GetItemHeight(int nIndex) const
{
  if(nIndex == -1)
  {
    // Calculate the itemheight within the skin!
    CRect rect;
    GetWindowRect(rect);
    return rect.Height() - (2 * STYLE_TEXTEDIT_BORDER);
  }
  if(m_listControl)
  {
    return m_listControl->GetItemHeight(nIndex);
  }
  return CB_ERR;
}

int
StyleComboBox::GetLBText(int nIndex,LPTSTR lpszText) const
{
  if(m_listControl)
  {
    return m_listControl->GetText(nIndex,lpszText);
  }
  return 0;
}

void
StyleComboBox::GetLBText(int nIndex, CString& rString) const
{
  int len = m_listControl->GetTextLen(nIndex);
  if(len > 0)
  {
    rString.GetBufferSetLength(len + 1);
    m_listControl->GetText(nIndex,(LPTSTR)rString.GetString());
    rString.ReleaseBuffer();
  }
}

int
StyleComboBox::GetLBTextLen(int nIndex) const
{
  return m_listControl->GetTextLen(nIndex);
}

LCID  
StyleComboBox::GetLocale() const
{
  return m_listControl->GetLocale();
}

int
StyleComboBox::GetMinVisible() const
{
  return m_minimum;
}

int
StyleComboBox::GetTopIndex() const
{
  return m_listControl->GetTopIndex();
}

int
StyleComboBox::InitStorage(int nItems, UINT nBytes)
{
  return m_listControl->InitStorage(nItems,nBytes);
}

int
StyleComboBox::InsertString(int nIndex, LPCTSTR lpszString)
{
  return m_listControl->InsertString(nIndex,lpszString);
}

BOOL  
StyleComboBox::LimitText(int nMaxChars)
{
  m_itemControl->LimitText(nMaxChars);
  return TRUE;
}

void  
StyleComboBox::ResetContent()
{
  m_itemControl->SetWindowText("");
  m_listControl->ResetContent();
  m_items.clear();
}

int
StyleComboBox::SelectString(int nStartAfter, LPCTSTR lpszString)
{
  return m_listControl->SelectString(nStartAfter,lpszString);
}

BOOL
StyleComboBox::SetCueBanner(LPCTSTR lpszText,BOOL /* fDrawIfFocused = FALSE*/)
{
  CString text(lpszText);
  m_itemControl->SetEmpty(true,text);
  return TRUE;
}

int
StyleComboBox::SetCurSel(int nSelect)
{
  if(nSelect < 0 || nSelect >= m_listControl->GetCount())
  {
    // Clear the combo box
    m_itemControl->SetWindowText("");
    m_listControl->SetCurSel(-1);
    return CB_ERR;
  }
  CString text;
  GetLBText(nSelect,text);
  m_itemControl->SetWindowText(text);
  return m_listControl->SetCurSel(nSelect);
}

int
StyleComboBox::SetDroppedWidth(UINT nWidth)
{
  CRect rect;
  GetWindowRect(&rect);

  if(nWidth >= 0 && nWidth <= (UINT)rect.Width())
  {
    m_minimalDropWidth = nWidth;
    return nWidth;
  }
  return CB_ERR;
}

int
StyleComboBox::SetMaxDroppedWidth(UINT p_percentage)
{
  if(p_percentage >= 100 && p_percentage <= 1000)
  {
    m_maxWidthPercentage = p_percentage;
    return CB_OKAY;
  }
  return CB_ERR;
}

BOOL
StyleComboBox::SetEditSel(int nStartChar, int nEndChar)
{
  if(GetStyle() & CBS_DROPDOWNLIST)
  {
    return CB_ERR;
  }
  m_itemControl->SetSel(nStartChar, nEndChar);
  return TRUE;
}

int
StyleComboBox::SetExtendedUI(BOOL bExtended /*= TRUE*/)
{
  m_extendedUI = bExtended;
  return CB_OKAY;
}

void
StyleComboBox::SetHorizontalExtent(UINT nExtent)
{
  m_listControl->SetHorizontalExtent(nExtent);
}

int
StyleComboBox::SetItemData(int nIndex, DWORD_PTR dwItemData)
{
  try
  {
    ItemStore::iterator it = m_items.find(nIndex);
    if(it == m_items.end())
    {
      m_items.insert(std::make_pair(nIndex,dwItemData));
    }
    else
    {
      m_items[nIndex] = dwItemData;
    }
  }
  catch(std::exception& ex)
  {
    UNREFERENCED_PARAMETER(ex);
    return CB_ERR;
  }
  return 0;
}

int
StyleComboBox::SetItemDataPtr(int nIndex, void* pData)
{
  return m_listControl->SetItemDataPtr(nIndex, pData);
}

int
StyleComboBox::SetItemHeight(int nIndex,UINT cyItemHeight)
{
  if (m_listControl)
  {
    return m_listControl->SetItemHeight(nIndex, cyItemHeight);
  }
  return CB_ERR;
}

LCID
StyleComboBox::SetLocale(LCID nNewLocale)
{
  return m_listControl->SetLocale(nNewLocale);
}

BOOL
StyleComboBox::SetMinVisibleItems(int iMinVisible)
{
  m_minimum = iMinVisible;
  return TRUE;
}

int
StyleComboBox::SetTopIndex(int nIndex)
{
  return m_listControl->SetTopIndex(nIndex);
}

void
StyleComboBox::ShowDropDown(BOOL bShowIt /*= TRUE*/)
{
  if(bShowIt)
  {
    OnDropdown();
  }
  else
  {
    OnCloseup();
  }
}

// Clipboard operations

void
StyleComboBox::Clear()
{
  m_itemControl->Clear();
}

void
StyleComboBox::Copy()
{
  m_itemControl->Copy();
}

void
StyleComboBox::Cut()
{
  m_itemControl->Cut();
}

void
StyleComboBox::Paste()
{
  m_itemControl->Paste();
}

void  
StyleComboBox::GetWindowText(CString& p_text)
{
  m_itemControl->GetWindowText(p_text);
}

void  
StyleComboBox::SetWindowText(CString  p_text)
{
  m_itemControl->SetWindowText(p_text);
}

int
StyleComboBox::OnSetText(LPCTSTR lpszText)
{
  m_itemControl->SetWindowText(lpszText);
  return 1;
}

int     
StyleComboBox::OnGetText(int nMaxChars,LPTSTR lpszText)
{
  CString text;
  m_itemControl->GetWindowText(text);
  int length = text.GetLength();
  if(length > nMaxChars)
  {
    length = nMaxChars;
  }
  _tcscpy_s(lpszText,nMaxChars,text.GetString());
  return length;
}

UINT
StyleComboBox::OnGetTextLength()
{
  CString text;
  m_itemControl->GetWindowText(text);
  return text.GetLength();
}

BOOL  
StyleComboBox::EnableWindow(BOOL p_enable /*= TRUE*/)
{
  BOOL result = FALSE;
  result |= m_itemControl->EnableWindow(p_enable);
  result |= m_listControl->EnableWindow(p_enable);
  Invalidate();
  return result;
}

BOOL
StyleComboBox::IsWindowEnabled()
{
  return m_itemControl->IsWindowEnabled();
}

void
StyleComboBox::SetEmpty(bool p_empty,CString p_text /*= ""*/)
{
  if(m_itemControl)
  {
    m_itemControl->SetEmpty(p_empty,p_text);
  }
}

void
StyleComboBox::SetTextColorEmpty(COLORREF p_color)
{
  if(m_itemControl)
  {
    m_itemControl->SetTextColorEmpty(p_color);
  }
}

void
StyleComboBox::SetBackgroundColorEmpty(COLORREF p_color)
{
  if(m_itemControl)
  {
    m_itemControl->SetBackgroundColorEmpty(p_color);
  }
}

bool
StyleComboBox::GetIsEmpty()
{
  if(m_itemControl)
  {
    m_itemControl->GetIsEmpty();
  }
  return false;
}

CString
StyleComboBox::GetEmptyText()
{
  if(m_itemControl)
  {
    return m_itemControl->GetEmptyText();
  }
  return "";
}

void
StyleComboBox::SetFontSize(int p_size)
{
  if(m_itemControl && m_listControl)
  {
    m_itemControl->SetFontSize(p_size);
    m_listControl->SetFontSize(p_size);
  }
}

void
StyleComboBox::SetFontStyle(bool p_bold,bool p_italic /*= false*/,bool p_underLine /*= false*/)
{
  if(m_itemControl && m_listControl)
  {
    m_itemControl->SetFontStyle(p_bold,p_italic,p_underLine);
    m_listControl->SetFontStyle(p_bold,p_italic,p_underLine);
  }
}

void
StyleComboBox::SetFontName(CString p_fontName,int p_fontSize /*= 100*/,BYTE p_language /*= DEFAULT_CHARSET*/)
{
  if(m_itemControl && m_listControl)
  {
    m_itemControl->SetFontName(p_fontName,p_fontSize,p_language);
    m_listControl->SetFontName(p_fontName,p_fontSize,p_language);
  }
}

//////////////////////////////////////////////////////////////////////////
//
// PAINT CODE
//
//////////////////////////////////////////////////////////////////////////

void
StyleComboBox::OnPaint()
{
  PAINTSTRUCT paint;
  CDC* dc = BeginPaint(&paint);
  if(dc && m_itemControl)
  {
    m_itemControl->OnComboBoxPaint();

    CRect rcItem;
    this->GetClientRect(&rcItem);

    CRect editItem(rcItem);
    if(m_itemControl->GetSkin())
    {
      m_itemControl->GetSkin()->GetClientRect(&editItem);
    }
    int size = editItem.Height();
    CRect but(rcItem.right - size,rcItem.top,rcItem.right,rcItem.top + size);

    // Draw the arrow
    COLORREF color = DrawComboButton(dc,but);

    CPen framepen;
    COLORREF framecolor = FRAME_DEFAULT_COLOR;
    int bordersize = 1;
    bool  readonly = false;
    m_itemControl->GetDrawFrameColor(framecolor,bordersize,readonly);

    framepen.CreatePen(PS_SOLID,bordersize,framecolor);
    HGDIOBJ orgpen = dc->SelectObject(framepen);
    CBrush  brush(color);
    HGDIOBJ orgbrush = dc->SelectObject(&brush);

    // Paint the frame
    dc->MoveTo(rcItem.left  + 1,    rcItem.top + bordersize - 1);
    dc->LineTo(rcItem.right - 1,    rcItem.top + bordersize - 1);
    dc->LineTo(rcItem.right - 1,    rcItem.bottom           - 1);
    dc->LineTo(rcItem.left  + 1,    rcItem.bottom           - 1);
    dc->LineTo(rcItem.left  + 1,    rcItem.top + bordersize - 1);
    dc->MoveTo(rcItem.right - size, rcItem.top + bordersize - 1);
    dc->LineTo(rcItem.right - size, rcItem.bottom           - 1);

    dc->SelectObject(orgpen);
    dc->SelectObject(orgbrush);
  }
  EndPaint(&paint);
}

COLORREF
StyleComboBox::DrawComboButton(CDC* p_dc,CRect& p_but)
{
  // Find the arrow color
  COLORREF color = ThemeColor::GetColor(Colors::AccentColor1);
  if(m_arrowColor)
  {
    color = m_arrowColor;
  }
  else if(!this->IsWindowEnabled())
  {
    color = ThemeColor::GetColor(Colors::AccentColor2);
  }

  // Create pen
  CPen pen;
  pen.CreatePen(PS_SOLID,1,color);
  HGDIOBJ orgpen = p_dc->SelectObject(pen);

  // Paint the button
  DWORD background = ThemeColor::GetColor(Colors::ColorCtrlBackground);
  if(m_buttonDown)
  {
    background = ThemeColor::GetColor(Colors::ColorComboDropped);
  }
  else if(m_itemControl->GetHoverOver())
  {
    background = ThemeColor::GetColor(Colors::ColorComboActive);
  }
  p_dc->FillSolidRect(p_but,background);
  p_but.CenterPoint();

  POINT points[3];
  if(m_buttonDown)
  {
    points[0].x = p_but.CenterPoint().x - WS(4);
    points[0].y = p_but.CenterPoint().y + WS(4);
    points[1].x = p_but.CenterPoint().x + 1;
    points[1].y = p_but.CenterPoint().y - 1;
    points[2].x = p_but.CenterPoint().x + WS(4) + 2;
    points[2].y = p_but.CenterPoint().y + WS(4);
  }
  else
  {
    points[0].x = p_but.CenterPoint().x - WS(4);
    points[0].y = p_but.CenterPoint().y - 1;
    points[1].x = p_but.CenterPoint().x + 1;
    points[1].y = p_but.CenterPoint().y + WS(4);
    points[2].x = p_but.CenterPoint().x + WS(4) + 2;
    points[2].y = p_but.CenterPoint().y - 1;
  }

  CBrush brush(color);
  HGDIOBJ orgbrush = p_dc->SelectObject(&brush);
  p_dc->Polygon(points,3);

  p_dc->SelectObject(orgpen);
  p_dc->SelectObject(orgbrush);

  return color;
}

//////////////////////////////////////////////////////////////////////////
//
// THE EDIT CONTROL OF THE COMBO BOX
//
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(SCBTextEdit,StyleEdit)

BEGIN_MESSAGE_MAP(SCBTextEdit,StyleEdit)
  ON_WM_CHAR()
  ON_WM_SETFOCUS()
  ON_WM_KILLFOCUS()
  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONUP()
  ON_WM_KEYDOWN()
  ON_WM_KEYUP()
  ON_WM_PASTE()
  // BEWARE: Do not activate
  // ON_WM_WINDOWPOSCHANGED()
END_MESSAGE_MAP()

SCBTextEdit::SCBTextEdit()
            :m_autoComplete(false)
            ,m_doCompletion(false)
            ,m_complete(false)
            ,m_keyboardTime(0)
{
}

SCBTextEdit::~SCBTextEdit()
{
}

void 
SCBTextEdit::SetAutoComplete(bool p_auto /*= true*/)
{
  m_autoComplete = p_auto;
}

bool 
SCBTextEdit::GetAutoComplete()
{
  return m_autoComplete;
}

bool
SCBTextEdit::GetHasFocus() const
{
  return m_focus;
}

void
SCBTextEdit::CheckAutoCompletion()
{
  m_doCompletion = false;
  if(m_autoComplete)
  {
    CString text;
    GetWindowText(text);
    int first = -1;
    int last  = -1;
    GetSel(first,last);
    if(first > 0)
    {
      text = text.Left(first);
    }
    if(text.GetLength() == 0 || text.Compare(m_origText) == 0 || 
      (GetStyle() & ES_READONLY))
    {
      m_doCompletion = true;
    }
  }
}

void
SCBTextEdit::OnChar(UINT nChar,UINT nRepCnt,UINT nFlags)
{
  int first = -1;
  int last  = -1;
  int type  = (GetStyle() & ES_READONLY) ? 0 : 1;
  CString text;
  CString after;

  // Send CBN_EDITUPDATE: A text field is about to be changed
  m_combo->OnEditUpdate();

  // Check auto completion
  if(m_doCompletion)
  {
    GetWindowText(text);
    GetSel(first,last);
    if(type)
    {
      m_origText = text.Left(first);
    }
    if(nRepCnt == 1 && (' ' <= nChar && nChar <= 0x7F))
    {
      if(type || (m_combo->GetTypeBuffer() && GetTickCount() < (m_keyboardTime + COMBO_KEYBOARD_CACHE)))
      {
        m_origText += (char)nChar;
      }
      else
      {
        m_origText = (char)nChar;
      }
    }
    else
    {
      if(GetTickCount() >= (m_keyboardTime + COMBO_KEYBOARD_CACHE))
      {
        m_origText = (char)nChar;
      }
    }
  }

  m_keyboardTime = GetTickCount();

  // Do the original change and capture an eventual change
  CEdit::OnChar(nChar, nRepCnt, nFlags);
  GetWindowText(after);
  
  if(m_doCompletion && type && m_origText.Compare(after) == 0)
  {
    m_complete = true;
  }

  if(m_doCompletion && (nChar != VK_BACK && nChar != VK_DELETE)) 
  {
    int base = m_combo->GetCurSel();
    int find = 0;
    CString searchtext(m_origText);
    if(!type)
    {
      searchtext = (char)nChar;
    }
    if(m_origText.GetLength() > 1)
    {
      find = m_combo->FindString(0,searchtext);
    }
    else
    {
       find = m_combo->FindString(base - type, searchtext);
    }
    if(find == -1 && base > 0)
    {
      // Search from the beginning, in case we searched past the end
      find = m_combo->FindString(-1,searchtext);
    }
    if(find >= 0)
    {
      CString total;
      m_combo->GetLBText(find, total);
      m_combo->SetCurSel(find);
      SetWindowText(total);
      if(type)
      {
        SetSel(m_origText.GetLength(),total.GetLength(),TRUE);
      }
      else
      {
        SetSel(0,0);
      }
    }
  }

  // See if something has changed
  if(text.Compare(after))
  {
    // Send CBN_EDITCHANGE: A textfield HAS changed
    m_combo->OnEditChange();
  }
  DrawFrame();
}

void
SCBTextEdit::OnKeyDown(UINT nChar,UINT nRepCnt,UINT nFlags)
{
  StyleEdit::OnKeyDown(nChar,nRepCnt,nFlags);
  DrawFrame();
}

void
SCBTextEdit::OnKeyUp(UINT nChar,UINT nRepCnt,UINT nFlags)
{
  StyleEdit::OnKeyUp(nChar,nRepCnt,nFlags);
  DrawFrame();
}

void
SCBTextEdit::OnLButtonUp(UINT nFlags, CPoint point)
{
  StyleEdit::OnLButtonUp(nFlags,point);
  DrawFrame();
}

void
SCBTextEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
  if(m_combo && GetReadOnly())
  {
    m_combo->OnLButtonDown(nFlags,point);
  }
  else
  {
    CEdit::OnLButtonDown(nFlags,point);
  }
  DrawFrame();
}

void 
SCBTextEdit::OnSetFocus(CWnd* p_prev)
{
  m_combo->OnSetFocus(p_prev);
  CEdit::OnSetFocus(p_prev);
  CheckAutoCompletion();
  DrawFrame();

  if((m_combo->GetStyle() & CBS_DROPDOWNLIST) == CBS_DROPDOWN)
  {
    SetSel(0,-1,TRUE);
  }
}

void
SCBTextEdit::OnKillFocus(CWnd* p_new)
{
  if(p_new == this || p_new == m_combo || 
     p_new == m_combo->GetListControl())
  {
    return;
  }

  if(m_doCompletion)
  {
    CString text;
    GetWindowText(text);
    if(m_complete || text.CompareNoCase(m_origText) && !GetReadOnly())
    {
      int index = m_combo->FindStringExact(-1,text);
      if(index >= 0)
      {
        m_combo->SetCurSel(index);
        m_combo->OnSelEndOK(true);
      }
      m_complete = false;
    }
  }
  CEdit::OnKillFocus(p_new);
  DrawFrame();

  // Now propagate to the combo box above
  m_combo->OnKillFocus();
}

BOOL
SCBTextEdit::PreTranslateMessage(MSG* p_msg)
{
  if(p_msg->message == WM_KEYDOWN)
  {
    UINT key   = (UINT) p_msg->wParam;
    bool cntrl = GetKeyState(VK_CONTROL) != 0;
    if(p_msg->wParam == VK_F4 || 
      (p_msg->wParam == VK_DOWN && !cntrl))
    {
      if(m_combo && m_combo->GetExtendedUI())
      {
        if(p_msg->wParam == VK_F4)
        {
          m_combo->SetMaxDroppedWidth(400);
        }
        m_combo->OnDropdown();
        DrawFrame();
        return TRUE;
      }
    }
    if(cntrl &&
       (key == VK_PRIOR || key == VK_NEXT  ||
        key == VK_DOWN  || key == VK_UP    ||
        key == VK_RIGHT || key == VK_LEFT) )
    {
      if(m_combo->GetGridCell())
      {
        CGridCellCombo* cell = m_combo->GetGridCell();
        CGridCtrl* grid = reinterpret_cast<CGridCtrl*>(m_combo->GetParent());
        cell->SetLastChar(key);
        cell->EndEdit();
        // This will end this control!
        grid->SetFocus();
      }
      return TRUE;
    }
  }
  if(p_msg->message == WM_KEYUP)
  {
    if(p_msg->wParam == VK_BACK || p_msg->wParam == VK_DELETE)
    {
      int start = -1;
      int last  = -1;
      GetWindowText(m_origText);
      GetSel(start,last);

      if(p_msg->wParam == VK_DELETE && (GetStyle() & ES_READONLY))
      {
        m_combo->SetCurSel(-1);
        m_origText.Empty();
      }
      else // if(p_msg->wParam == VK_BACK)
      {
        m_origText = m_origText.Left(start);
      }
      m_doCompletion = m_autoComplete;
    }
  }
  return StyleEdit::PreTranslateMessage(p_msg);
}

void
SCBTextEdit::OnPaste()
{
  CString text = StyleGetStringFromClipboard(GetSafeHwnd());

  if(!text.IsEmpty())
  {
    int index = m_combo->FindStringExact(-1,text);
    if(index >= 0)
    {
      m_combo->SetCurSel(index);
      m_combo->OnSelEndOK(true);
      m_combo->OnCloseup(true);
    }
    else m_combo->SetCurSel(-1);
  }
}

//////////////////////////////////////////////////////////////////////////
//
// THE LIST CONTROL OF THE COMBO BOX
//
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(SCBListBox, StyleListBox)

BEGIN_MESSAGE_MAP(SCBListBox,StyleListBox)
  ON_WM_MOUSEMOVE()
  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONUP()
  ON_WM_KEYDOWN()
  ON_WM_ACTIVATE()
  ON_WM_SETFOCUS()
  // BEWARE: Do not activate
  // ON_WM_WINDOWPOSCHANGED()
END_MESSAGE_MAP()

SCBListBox::SCBListBox()
           :m_combo(nullptr)
           ,m_active(false)
           ,m_multiSelect(false)
           ,m_current(-1)
           ,m_lastClick(0)
{
}

SCBListBox::~SCBListBox()
{
  if(GetSafeHwnd())
  {
    DestroyWindow();
  }
}

void
SCBListBox::PreSubclassWindow()
{
  SetFont(&STYLEFONTS.DialogTextFont);
}

bool
SCBListBox::GetMultiSelect()
{
  return m_multiSelect;
}

void
SCBListBox::SetMultiSelect(bool p_multi)
{
  m_multiSelect = p_multi;
}

void 
SCBListBox::SetStyleComboBox(StyleComboBox* p_combo)
{
  m_combo = p_combo;
}

int
SCBListBox::AddString(LPCTSTR p_string)
{
  int index = StyleListBox::AddString(p_string);
  if(index >= 0 && m_multiSelect)
  {
    m_selected.push_back(false);
  }
  return index;
}

int
SCBListBox::DeleteString(UINT nIndex)
{
  if(m_selected.size() > 0)
  {
    m_selected.pop_back();
  }
  return StyleListBox::DeleteString(nIndex);
}

int  
SCBListBox::InsertString(int nIndex,LPCTSTR lpszString)
{
  int index = StyleListBox::InsertString(nIndex,lpszString);
  if (index >= 0 && m_multiSelect)
  {
    m_selected.push_back(false);
  }
  return index;
}

void 
SCBListBox::SelectAll(BOOL p_select /*=TRUE*/)
{
  StyleListBox::SelectAll(p_select);
  for(int index = 0;index < (int)m_selected.size();++index)
  {
    m_selected[index] = p_select;
  }
}

void 
SCBListBox::ResetContent()
{
  StyleListBox::ResetContent();
  m_selected.clear();
}

void 
SCBListBox::SetMultiSelection(int p_index, bool p_select)
{
  if(p_index >= 0 && p_index < (int)m_selected.size())
  {
    m_selected[p_index] = p_select;
  }
}

bool 
SCBListBox::GetMultiSelection(int p_index)
{
  if(p_index >= 0 && p_index < (int)m_selected.size())
  {
    return m_selected[p_index];
  }
  return false;
}

CString 
SCBListBox::GetCurrentMultiSelect()
{
  CString text;
  for(int index = 0;index < (int)m_selected.size();++index)
  {
    if(m_selected[index])
    {
      CString item;
      GetText(index,item);
      if (!text.IsEmpty())
      {
        text += ";";
      }
      text += item;
    }
  }
  return text;
}

void
SCBListBox::SetCurrentMultiSelect(CString p_text)
{
  // Reset selected
  for(int index = 0;index < (int)m_selected.size();++index)
  {
    m_selected[index] = false;
  }
  // Use the string
  while(p_text.GetLength())
  {
    CString selected;
    int pos = p_text.Find(';');
    if (pos > 0)
    {
      selected = p_text.Left(pos);
      p_text = p_text.Mid(pos + 1);
    }
    else
    {
      selected = p_text;
      p_text.Empty();
    }
    int index = FindStringExact(0,selected);
    if (index >= 0)
    {
      m_selected[index] = true;
    }
  }
  // Reset 
  m_current = -1;
}

void 
SCBListBox::SaveCurSel()
{
  m_current = GetCurSel();
}

void 
SCBListBox::ResetCurSel()
{
  SetCurSel(m_current);
}

void 
SCBListBox::SetActive() 
{ 
  m_active = true; 
  if(m_multiSelect)
  {
    ShowMultiSelection();
  }
  m_mouseActivated = false;
  if(GetAsyncKeyState(GetSystemMetrics(SM_SWAPBUTTON)  ? VK_RBUTTON : VK_LBUTTON) & 0x8000)
  {
    m_mouseActivated = true;
  }
}

void
SCBListBox::ShowMultiSelection()
{
  int topindex = GetTopIndex();
  int current  = GetCurSel();
  SendMessage(WM_SETREDRAW,FALSE,0);

  int count = GetCount();
  for(int index = 0;index < count;++index)
  {
    if(index < (int)m_selected.size())
    {
      SetSel(index,m_selected[index]);
    }
  }
  SetTopIndex(topindex);
  SendMessage(WM_SETREDRAW,TRUE,0);
  SetCurSel(current);
  Invalidate();
}

void 
SCBListBox::SelectCurrentSelection()
{
  if(!IsWindowEnabled())
  {
    CancelCurrentSelection();
    return;
  }
  m_active = false;
  GetSkin()->SkinReleaseCapture();
  GetSkin()->SkinCancelMouseTracking();
  // Notify that selection has changed
  m_combo->OnSelEndOK();
  m_combo->OnCloseup();
  m_combo->OnLbnSelChange();
}

void 
SCBListBox::CancelCurrentSelection()
{
  GetSkin()->SkinReleaseCapture();
  GetSkin()->SkinCancelMouseTracking();
  ResetCurSel();
  m_combo->OnSelEndCancel();
  m_combo->OnCloseup();
}

void
SCBListBox::OnActivate(UINT nState,CWnd* pWndOther,BOOL pMinimized)
{
  if(nState == WA_INACTIVE && pWndOther)
  {
    // Application loses the focus
    m_combo->OnCloseup();
  }
}

void
SCBListBox::HandleDoubleClick(CPoint point)
{
  // Select the current item
  int height = GetItemHeight(0);
  int index = point.y / height;
  index += GetTopIndex();
  m_selected[index] = true;

  // For feedback purposes we show the user what was double clicked
  ShowMultiSelection();
  SendMessage(WM_PAINT,0,0);
  Sleep(100);

  // This is what we want.
  SelectCurrentSelection();
}

void 
SCBListBox::OnSetFocus(CWnd* pOldWnd)
{
  m_combo->OnSetFocus(pOldWnd);
  if(pOldWnd != this && pOldWnd != m_combo &&
     pOldWnd != m_combo->GetEditControl())
  {
    if(!m_combo->GetDroppedState() && m_combo->GetEditControl())
    {
      m_combo->GetEditControl()->SetFocus();
      return;
    }
  }
  StyleListBox::OnSetFocus(pOldWnd);
}

void
SCBListBox::OnMouseMove(UINT nFlags,CPoint point)
{
  CRect rcClient;
  GetClientRect(&rcClient);

  if(PtInRect(rcClient,point) && m_combo && m_active)
  {
    INT topIndex   = GetTopIndex();
    INT itemHeight = GetItemHeight(0);

    // Compute which index to check/unchecked
    INT index = topIndex + point.y / itemHeight;

    CRect rcItem;
    GetItemRect(index,&rcItem);

    if(PtInRect(rcItem,point))
    {
      // Current item
      CRect rcCurrent;
      int current = GetCurSel();
      GetItemRect(current, &rcCurrent);

      // Change selection
      SetCurSel(index);
      // Paint optimizations:
      // Only redraw the changed items!
      InvalidateRect(rcCurrent);
      InvalidateRect(rcItem);
    }
  }
}

void
SCBListBox::SelectCurrent(CPoint point)
{
  CRect rcClient;
  GetClientRect(&rcClient);

  if(PtInRect(rcClient,point) && m_combo)
  {
    INT topIndex   = GetTopIndex();
    INT itemHeight = GetItemHeight(0);

    // Compute which index to check/unchecked
    INT index = topIndex + point.y / itemHeight;

    CRect rcItem;
    GetItemRect(index,&rcItem);

    if(PtInRect(rcItem,point))
    {
      // Change selection
      SetCurSel(index);
    }
  }
}

void
SCBListBox::OnLButtonUp(UINT nFlags, CPoint point)
{
  CRect frame;
  GetSkin()->GetWindowRect(&frame);

  CPoint scpoint(point);
  ClientToScreen(&scpoint);
  if (frame.PtInRect(scpoint))
  {
    CRect rect;
    GetClientRect(&rect);

    if (rect.PtInRect(point))
    {
      if(!m_multiSelect)
      {
        SelectCurrent(point);
        SelectCurrentSelection();
      }
      return;
    }
  }
  else
  {
    // EAT one (1) mouse button up!
    if(m_mouseActivated)
    {
      m_mouseActivated = false;
      return;
    }
    // Outside the frame
    CancelCurrentSelection();
    return;
  }
  StyleListBox::OnLButtonUp(nFlags, point);
}

void 
SCBListBox::OnLButtonDown(UINT nFlags,CPoint point)
{
  CRect frame;
  GetSkin()->GetWindowRect(&frame);

  CPoint scpoint(point);
  ClientToScreen(&scpoint);
  if(frame.PtInRect(scpoint))
  {
    CRect rect;
    GetClientRect(&rect);

    if(rect.PtInRect(point))
    {
      if(m_multiSelect)
      {
        int height = GetItemHeight(0);
        int index  = point.y / height;
        index += GetTopIndex();

        if((m_current == index) && (clock() - m_lastClick < ::GetDoubleClickTime()))
        {
          HandleDoubleClick(point);
          return;
        }
        m_current   = index;
        m_lastClick = clock();
        m_selected[index] = !m_selected[index];
        ShowMultiSelection();
      }
      return;
    }
  }
  else
  {
    // Outside the frame
    CancelCurrentSelection();
    return;
  }

  if(m_combo->GetDroppedState())
  {
    SkinScrollWnd* skin = GetSkin();
    skin->SkinReleaseCapture();
    skin->SkinCancelMouseTracking();

    LPARAM param = MAKELONG(point.x,point.y);
    skin->m_sbVert.SendMessage(WM_LBUTTONDOWN,(WPARAM)nFlags,param);
  }
}

int
SCBListBox::LastSelected()
{
  int last = -1;
  for(int index = 0;index < (int)m_selected.size();++index)
  {
    if(m_selected[index])
    {
      last = index;
    }
  }
  return last;
}

// Handle all special keys in the pre-translate
// as larger and complex programs will eat your keys!
BOOL 
SCBListBox::PreTranslateMessage(MSG* pMsg)
{
  if(pMsg->message == WM_KEYDOWN)
  {
    int nchar = (int) pMsg->wParam;
    if(nchar == VK_UP    || nchar == VK_DOWN ||
       nchar == VK_PRIOR || nchar == VK_NEXT)
    {
      int index = GetCaretIndex();
      int page  = m_combo->GetMinVisible();
      m_searchText.Empty();

      switch(nchar)
      {
        case VK_UP:    --index;       break;
        case VK_DOWN:  ++index;       break;
        case VK_PRIOR: index -= page; break;
        case VK_NEXT:  index += page; break;
      }
      if(index >= GetCount()) index = GetCount() - 1;
      if(index < 0)           index = 0;

      if(m_multiSelect)
      {
        ShowMultiSelection();
        SetCaretIndex(index);
      }
      else
      {
        SetCurSel(index);
        SetCaretIndex(index);
        Invalidate();
      }
      return TRUE;
    }
    if(nchar == VK_ESCAPE || nchar == VK_BACK)
    {
      m_searchText.Empty();
      m_combo->OnCloseup();
      DrawFrame();
      return TRUE;
    }
    if(nchar == VK_RETURN)
    {
      m_searchText.Empty();
      SelectCurrentSelection();
      return TRUE;
    }
    if(nchar == VK_SPACE || nchar == VK_TAB)
    {
      m_searchText.Empty();
      if(m_multiSelect)
      {
        int  index = GetCaretIndex();
        bool caret = index != LastSelected();
        m_selected[index] = !m_selected[index];
        ShowMultiSelection();
        if(caret)
        {
          SetCaretIndex(index);
        }
        return TRUE;
      }
      SelectCurrentSelection();
      if(nchar == VK_TAB)
      {
        m_combo->PostMessage(WM_KEYDOWN,VK_TAB,pMsg->lParam);
        m_combo->PostMessage(WM_CHAR,   VK_TAB,pMsg->lParam);
        m_combo->PostMessage(WM_KEYUP,  VK_TAB,pMsg->lParam);
      }
      return TRUE;
    }

    if (nchar <= 0x7F && !(nchar == VK_LEFT || nchar == VK_UP || nchar == VK_RIGHT || nchar == VK_DOWN))
    {
      if(m_combo->GetTypeBuffer() && GetTickCount() < (m_keyboardTime + COMBO_KEYBOARD_CACHE))
      {
        m_searchText += (char)nchar;
      }
      else
      {
        m_searchText = (char)nchar;
      }
      m_combo->SelectString(0, m_searchText);
      m_keyboardTime = GetTickCount();
      Invalidate();
      return true;
    }
  }
  return StyleListBox::PreTranslateMessage(pMsg);
}

//////////////////////////////////////////////////////////////////////////
//
// SUPPORT FOR DynamicDataEXchange in Dialogs
// MANDATORY: Use either of these to initialize the StyleComboBox !!
//
//////////////////////////////////////////////////////////////////////////

void AFXAPI DDX_Control(CDataExchange* pDX,int nIDC,StyleComboBox& p_control)
{
  CWnd& parent = reinterpret_cast<CWnd&>(p_control);
  DDX_Control(pDX,nIDC,parent);
  p_control.InitSkin();
}

void WINAPI DDX_CBString(CDataExchange* pDX,int nIDC,StyleComboBox& p_control,CString& p_text)
{
  CWnd& parent = reinterpret_cast<CWnd&>(p_control);
  DDX_Control(pDX,nIDC,parent);
  p_control.InitSkin();

  if(pDX->m_bSaveAndValidate)
  {
    p_control.GetWindowText(p_text);
  }
  else
  {
    p_control.SetWindowText(p_text);
  }
}

void WINAPI DDX_CBIndex(CDataExchange* pDX,int nIDC,StyleComboBox& p_control,int& p_index)
{
  CWnd& parent = reinterpret_cast<CWnd&>(p_control);
  DDX_Control(pDX,nIDC,parent);
  p_control.InitSkin();

  if(pDX->m_bSaveAndValidate)
  {
    p_index = p_control.GetCurSel();
  }
  else
  {
    p_control.SetCurSel(p_index);
  }
}
