////////////////////////////////////////////////////////////////////////
//
// File: StyleListBox.cpp
// Function: Styling frame for CListBox object
//           Also does a recalculate of the widest string
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
#include "StyleListBox.h"
#include "StyleFonts.h"
#include "StyleColors.h"
#include "LinePrinter.h"
#include "StyleTexts.h"
#include "StyleMessageBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace ThemeColor;

/////////////////////////////////////////////////////////////////////////////
// StyleListBox

IMPLEMENT_DYNAMIC(StyleListBox,CListBox)

StyleListBox::StyleListBox()
             :m_skin(nullptr)
{
}

StyleListBox::~StyleListBox()
{
  RemoveLineInfo();
  ResetSkin();
  OnNcDestroy();
}

BEGIN_MESSAGE_MAP(StyleListBox, CListBox)
  ON_WM_PAINT()
  ON_WM_SHOWWINDOW()
  ON_WM_DESTROY()
  ON_WM_ERASEBKGND()
  ON_WM_HSCROLL()
END_MESSAGE_MAP()

void 
StyleListBox::PreSubclassWindow()
{
  // Check for unsupported styles in StyleFramework
  int style = GetStyle();
  ASSERT((style & LBS_OWNERDRAWVARIABLE) == 0);
  ASSERT((style & LBS_MULTICOLUMN)       == 0);

  ScaleControl(this);

  if(m_directInit)
  {
    InitSkin();
  }
}

//////////////////////////////////////////////////////////////////////////
// 
// Skinning functions
//
void
StyleListBox::InitSkin(int p_borderSize /*=1*/,int p_clientBias /*=0*/)
{
  if(m_skin == nullptr)
  {
    SetFont(&STYLEFONTS.DialogTextFont);
    int height = LISTBOX_ITEMHEIGTH;
    SetItemHeight(0,(height * GetSFXSizeFactor()) / 100);

    m_skin = SkinWndScroll(this,p_borderSize,p_clientBias);
  }
}

void
StyleListBox::ResetSkin()
{
  UnskinWndScroll(this);
}

SkinScrollWnd*
StyleListBox::GetSkin()
{
  return m_skin;
}

void
StyleListBox::SetDirectInit(bool p_init)
{
  m_directInit = p_init;
}

void
StyleListBox::DrawFrame()
{
  SkinScrollWnd* skin = GetSkin();
  if(skin)
  {
    COLORREF color = (this == GetFocus()) ? ThemeColor::GetColor(Colors::AccentColor1) : ThemeColor::GetColor(Colors::AccentColor2);
    skin->DrawFrame(color);
  }
}

BOOL
StyleListBox::OnEraseBkgnd(CDC* pDC)
{
  CRect client;
  GetClientRect(client);
  pDC->FillSolidRect(client,ThemeColor::GetColor(Colors::ColorCtrlBackground));
  return TRUE;
}

void
StyleListBox::OnPaint()
{
  if(!m_inPaint)
  {
    m_inPaint = true;
    PAINTSTRUCT ps;
    CDC* cdc = BeginPaint(&ps);
    OnEraseBkgnd(cdc);
    Internal_Paint(cdc);
    EndPaint(&ps);
    DrawFrame();
    m_inPaint = false;
  }
}

void
StyleListBox::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
  CListBox::OnHScroll(nSBCode,nPos,pScrollBar);
  Invalidate();
}

// Propagate the ShowWindow command
void
StyleListBox::OnShowWindow(BOOL bShow, UINT nStatus)
{
  SkinScrollWnd* skin = GetSkin();
  if(skin && nStatus == 0)
  {
    // nStatus == 0 means: Message sent is caused by an explict "ShowWindow" call
    skin->ShowWindow(bShow ? SW_SHOW : SW_HIDE);
  }
  else
  {
    CListBox::OnShowWindow(bShow, nStatus);
  }
}

//////////////////////////////////////////////////////////////////////////
//
// LIST BOX PUBLIC FUNCTIONS
//
//////////////////////////////////////////////////////////////////////////

int 
StyleListBox::AddString(LPCTSTR p_string)
{
  // Add string anyhow, so we can honour the LBS_SORT settings
  int result = CListBox::AddString(p_string);
  if(result != LB_ERR)
  {
    if(GetStyle() & LBS_OWNERDRAWFIXED)
    {
      ListBoxColorLine* line = new ListBoxColorLine();
      line->m_foreground = FRAME_DEFAULT_COLOR;
      line->m_background = FRAME_DEFAULT_COLOR;
      line->m_text       = p_string;

      SetItemPointer(result,line);
    }
    UpdateWidth(p_string);
    AdjustScroll();
  }
  return result;
}

int 
StyleListBox::InsertString(int p_index,LPCTSTR p_string,COLORREF p_foreground,COLORREF p_background)
{
  // Add string anyhow, so we can honour the LBS_SORT settings
  int result = CListBox::InsertString(p_index,p_string);
  if (result != LB_ERR)
  {
    if (GetStyle() & LBS_OWNERDRAWFIXED)
    {
      ListBoxColorLine* line = new ListBoxColorLine();
      line->m_foreground = p_foreground;
      line->m_background = p_background;
      line->m_text       = p_string;

      SetItemPointer(result,line);
    }
    UpdateWidth(p_string);
    AdjustScroll();
  }
  return result;
}

int 
StyleListBox::AppendString(LPCTSTR p_string,COLORREF p_foreground,COLORREF p_background)
{
  // Add string anyhow, so we can honor the LBS_SORT settings
  int result = CListBox::InsertString(-1,p_string);
  if (result != LB_ERR)
  {
    if(GetStyle() & LBS_OWNERDRAWFIXED)
    {
      ListBoxColorLine* line = new ListBoxColorLine();
      line->m_foreground = p_foreground;
      line->m_background = p_background;
      line->m_text       = p_string;

      SetItemPointer(result,line);
    }
    UpdateWidth(p_string);
    AdjustScroll();
}
  return result;
}

void
StyleListBox::SetItemPointer(int p_index,void* p_data)
{
  ListBoxColorLine* line = reinterpret_cast<ListBoxColorLine*>(GetItemDataPtr(p_index));
  if(line && (line != (ListBoxColorLine*) LB_ERR) && line->m_magic == LIST_MAGIC)
  {
    try
    {
      delete line;
    }
    catch(...) {}
  }
  SetItemDataPtr(p_index,p_data);
}

void 
StyleListBox::MeasureItem(LPMEASUREITEMSTRUCT p_measureItemStruct)
{
  // TODO: Add your code to determine the size of specified item
  ASSERT(p_measureItemStruct->CtlType == ODT_LISTBOX);

  ListBoxColorLine* listBox = reinterpret_cast<ListBoxColorLine*>(GetItemDataPtr(p_measureItemStruct->itemID));
  if(listBox && (listBox != (ListBoxColorLine*)LB_ERR) && listBox->m_magic == LIST_MAGIC)
  {
    CString strText = listBox->m_text;
    if (strText.GetLength())
    {
      CRect rect;
      GetItemRect(p_measureItemStruct->itemID, &rect);

      CDC* pDC = GetDC();
      p_measureItemStruct->itemHeight = pDC->DrawText(strText, -1, rect, DT_WORDBREAK | DT_CALCRECT);
      ReleaseDC(pDC);
    }
  }
}

void 
StyleListBox::DrawItem(LPDRAWITEMSTRUCT p_drawItemStruct)
{
  // TODO: Add your code to draw the specified item
  ASSERT(p_drawItemStruct->CtlType == ODT_LISTBOX);

  // Getting a DC
  CDC dc;
  dc.Attach(p_drawItemStruct->hDC);

  // Save these value to restore them when done drawing.
  COLORREF crOldTextColor = dc.GetTextColor();
  COLORREF crOldBkColor   = dc.GetBkColor();

  COLORREF foreground(FRAME_DEFAULT_COLOR);
  COLORREF background(FRAME_DEFAULT_COLOR);
  // Getting our foreground/background colors
  ListBoxColorLine* line = reinterpret_cast<ListBoxColorLine*>(p_drawItemStruct->itemData);
  if(!line || (line == (ListBoxColorLine*)LB_ERR) || line->m_magic != LIST_MAGIC)
  {
    return;
  }
  foreground = line->m_foreground;
  background = line->m_background;
  if(foreground == FRAME_DEFAULT_COLOR)
  {
    foreground = ThemeColor::GetColor(Colors::ColorEditText);
  }
  if(background == FRAME_DEFAULT_COLOR)
  {
    background = ThemeColor::GetColor(Colors::ColorCtrlBackground);
  }

  // If this item is selected, set the background color 
  // and the text color to appropriate values. Also, erase
  // rect by filling it with the background color.
  if((p_drawItemStruct->itemAction | ODA_SELECT) &&
     (p_drawItemStruct->itemState  & ODS_SELECTED))
  {
    int accent = ThemeColor::GetColor(Colors::AccentColor1);
    dc.SetTextColor(background);
    dc.SetBkColor  (accent);
    dc.FillSolidRect(&p_drawItemStruct->rcItem,accent);
  }
  else
  {
    dc.SetTextColor(foreground);
    dc.SetBkColor  (background);
    dc.FillSolidRect(&p_drawItemStruct->rcItem,background);
  }
  if((p_drawItemStruct->itemAction | ODA_FOCUS) &&
     (p_drawItemStruct->itemState  & ODS_FOCUS))
  {
    dc.DrawFocusRect(&p_drawItemStruct->rcItem);
  }

  // Draw the text.
  p_drawItemStruct->rcItem.left   += 5;
  p_drawItemStruct->rcItem.top    += 1;
  p_drawItemStruct->rcItem.bottom -= 1;

  dc.DrawText(line->m_text.GetString(),line->m_text.GetLength(),&p_drawItemStruct->rcItem,DT_NOCLIP | DT_NOPREFIX | DT_VCENTER);

  // Reset the background color and the text color back to their
  // original values.
  dc.SetTextColor(crOldTextColor);
  dc.SetBkColor  (crOldBkColor);

  dc.Detach();
}

int 
StyleListBox::CompareItem(LPCOMPAREITEMSTRUCT p_compare)
{
  if(p_compare->itemID1 >= 0 && p_compare->itemID1 < (UINT)GetCount() &&
     p_compare->itemID2 >= 0 && p_compare->itemID2 < (UINT)GetCount())
  {
    ListBoxColorLine* listBox1 = reinterpret_cast<ListBoxColorLine*>(GetItemDataPtr(p_compare->itemID1));
    ListBoxColorLine* listBox2 = reinterpret_cast<ListBoxColorLine*>(GetItemDataPtr(p_compare->itemID2));
    if(listBox1->m_magic == LIST_MAGIC && listBox2->m_magic == LIST_MAGIC)
    {
      return listBox1->m_text.Compare(listBox2->m_text);
    }
  }
  return 0;
}

void 
StyleListBox::DeleteItem(LPDELETEITEMSTRUCT p_deleteItemStruct)
{
  ASSERT(p_deleteItemStruct->CtlType == ODT_LISTBOX);
  ListBoxColorLine* line = reinterpret_cast<ListBoxColorLine*>(p_deleteItemStruct->itemData);
  if(line && (line != (ListBoxColorLine*) LB_ERR) && line->m_magic == LIST_MAGIC)
  {
    try
    {
      delete line;
      SetItemDataPtr(p_deleteItemStruct->itemID,nullptr);
    }
    catch(...){}
  }
  CListBox::DeleteItem(p_deleteItemStruct);
}

void
StyleListBox::ResetContent()
{
  if(GetStyle() & LBS_OWNERDRAWFIXED)
  {
    RemoveLineInfo();
  }
  // Empty the control
  CListBox::ResetContent();
  CListBox::SetHorizontalExtent(0);
  AdjustScroll();
  m_width = 0;
}

void
StyleListBox::OnDestroy()
{
  if(GetStyle() & LBS_OWNERDRAWFIXED)
  {
    RemoveLineInfo();
  }
  CListBox::OnDestroy();
}

// Deleting a string implies recalculating the width
// of all other strings in the control to shrink the
// maximum extend of the horizontal scrollbar
int 
StyleListBox::DeleteString(int p_number)
{
  if(GetStyle() & LBS_OWNERDRAWFIXED)
  {
    // Remove color info
    ListBoxColorLine* line = reinterpret_cast<ListBoxColorLine*>(GetItemDataPtr(p_number));
    if(line && (line != (ListBoxColorLine*) LB_ERR) && line->m_magic == LIST_MAGIC)
    {
      try
      {
        delete line;
        SetItemDataPtr(p_number,nullptr);
      }
      catch(...){}
    }
  }

  // Do the delete
  int result = CListBox::DeleteString(p_number);
  if(result < 0)
  {
    return result;
  }
  AdjustHorizontalExtent();
  return result;
}

void
StyleListBox::AdjustHorizontalExtent()
{
  CClientDC dc(this);

  CFont* f = GetFont();
  CFont* o = dc.SelectObject(f);

  m_width = 0;
  for(int i = 0; i < CListBox::GetCount(); i++)
  {
    /* scan strings */
    CString s;
    GetText(i,s);
    CSize sz = dc.GetTextExtent(s);
    sz.cx += 3 * ::GetSystemMetrics(SM_CXBORDER);
    if(sz.cx > m_width)
    {
      m_width = sz.cx;
    }
    /* scan strings */
  }
  CListBox::SetHorizontalExtent(m_width);
  AdjustScroll();
  dc.SelectObject(o);
}

void
StyleListBox::MoveWindow(int x,int y,int nWidth,int nHeight,BOOL bRepaint /* = TRUE */)
{
  SkinScrollWnd* skin = GetSkin();
  if(skin)
  {
    skin->MoveWindow(x,y,nWidth,nHeight,bRepaint);
    skin->OnSize(SIZE_RESTORED,nWidth - x,nHeight - y);
    return;
  }
  CListBox::MoveWindow(x,y,nWidth,nHeight,bRepaint);
}

// Edit functions
void 
StyleListBox::SelectAll(BOOL p_select /*=TRUE*/)
{
  int begin = 0;
  int einde = CListBox::GetCount();

  CListBox::SelItemRange(p_select,begin,einde - 1);
}

void 
StyleListBox::Copy()
{
  // Get selection from the listbox
  int count = CListBox::GetSelCount();
  if(count <= 0)
  {
    return;
  }
  int* lines = new int[count];
  CListBox::GetSelItems(count,lines);

  // Getting the text from the listbox
  CString result;
  for(int index = 0; index < count; ++index)
  {
    if(result.GetLength())
    {
      result += _T("\r\n");
    }
    CString text;
    GetText(lines[index],text);
    RemoveLineNumber(text);
    result += text;
  }

  // Put to clipboard (Unicode aware!)
  StylePutStringToClipboard(result,GetSafeHwnd());

  // Free lines array
  delete[] lines;
}

void 
StyleListBox::Cut()
{
  int count = CListBox::GetSelCount();
  if(count <= 0)
  {
    return;
  }
  int* lines = new int[count];
  GetSelItems(count,lines);

  // Remove in reverse order, otherwise we do 'striping'
  for(int index = count - 1 ;index >= 0; --index)
  {
    CListBox::DeleteString(lines[index]);
  }
  delete[] lines;
}

void 
StyleListBox::Paste()
{
  // Find our insertion point
  int insert = GetAnchorIndex();
  if(insert < 0)
  {
    insert = GetCount();
  }

  // Get text from the clipboard (Unicode aware!)
  CString text = StyleGetStringFromClipboard(GetSafeHwnd());

  // Insert into the listbox
  while(text.GetLength())
  {
    int pos = text.FindOneOf(_T("\r\n"));
    if (pos > 0)
    {
      CString part = text.Left(pos);
      CListBox::InsertString(insert++, part);
      text = text.Mid(pos + 1);
      if(text.Left(1) == _T("\n"))
      {
        text = text.Mid(1);
      }
    }
    else
    {
      // One/Last line insert
      CListBox::InsertString(insert,text);
      text.Empty();
    }
  }
}

// Print the complete list
bool
StyleListBox::Print(CString p_documentName
                   ,CString p_printer    /*= ""*/
                   ,CString p_pageName   /*= ""*/
                   ,CString p_dateName   /*= ""*/
                   ,CString p_endListing /*= ""*/)
{
  bool result = false;

  // Document name cannot be empty for the printer spooler system
  if(p_documentName.IsEmpty())
  {
    p_documentName = GetStyleText(TXT_PRO_LIST); // "List from program"
  }

  LinePrinter print;
  print.SetLineWrapping(true);
  print.SetHeader(LPHF_LEFT_TITLE);
  print.SetHeader(LPHF_RIGHT_DATETIME);
  print.SetFooter(LPHF_RIGHT_PAGENO);
  print.SetFooter(LPHF_ENDING);

  if(!p_pageName.IsEmpty())   print.SetHeaderFooterTextPage(p_pageName);
  if(!p_dateName.IsEmpty())   print.SetHeaderFooterTextDate(p_dateName);
  if(!p_endListing.IsEmpty()) print.SetHeaderFooterTextEnding(p_endListing);

  if(print.Document(p_printer,p_documentName))
  {
    for(int index = 0; index < GetCount(); ++index)
    {
      CString line;
      GetText(index, line);
      print.Print(line);
      print.NextLine();
    }
    print.EndDocument();
    result = true;
  }
  return result;
}

// Print the selected lines
bool
StyleListBox::PrintSelection(CString p_documentName
                            ,CString p_printer    /*= ""*/
                            ,CString p_pageName   /*= ""*/
                            ,CString p_dateName   /*= ""*/
                            ,CString p_endListing /*= ""*/)
{
  bool result = false;

  // Document name cannot be empty for the printer spooler system
  if(p_documentName.IsEmpty())
  {
    p_documentName = GetStyleText(TXT_SEL_LIST); // "Selection from list"
  }

  // Get selection from the listbox
  int count = CListBox::GetSelCount();
  if (count <= 0)
  {
    return result;
  }
  int* lines = new int[count];
  CListBox::GetSelItems(count,lines);

  // Print the selection
  LinePrinter print;
  print.SetLineWrapping(true);
  if (print.Document(p_printer, p_documentName))
  {
    print.SetLineWrapping(true);
    print.SetHeader(LPHF_LEFT_TITLE);
    print.SetHeader(LPHF_RIGHT_DATETIME);
    print.SetFooter(LPHF_RIGHT_PAGENO);
    print.SetFooter(LPHF_ENDING);

    if(!p_pageName.IsEmpty())   print.SetHeaderFooterTextPage(p_pageName);
    if(!p_dateName.IsEmpty())   print.SetHeaderFooterTextDate(p_dateName);
    if(!p_endListing.IsEmpty()) print.SetHeaderFooterTextEnding(p_endListing);

    for(int index = 0; index < count; ++index)
    {
      CString line;
      GetText(lines[index],line);
      print.Print(line);
      print.NextLine();
    }
    print.EndDocument();
    result = true;
  }
  delete[] lines;

  return result;
}

int
StyleListBox::GetText(int p_index,LPTSTR p_buffer) const
{
  ASSERT(::IsWindow(m_hWnd));
  int length = 0;
  if(p_index >= 0 && p_index < GetCount())
  {
    if(GetStyle() & LBS_OWNERDRAWFIXED)
    {
      ListBoxColorLine* listBox = reinterpret_cast<ListBoxColorLine*>(GetItemDataPtr(p_index));
      if(listBox && listBox->m_magic == LIST_MAGIC)
      {
        length = listBox->m_text.GetLength() + 1;
        _tcscpy_s(p_buffer, length, listBox->m_text.GetString());
      }
    }
    else
    {
      return CListBox::GetText(p_index,p_buffer);
    }
  }
  return length;
}

void 
StyleListBox::GetText(int p_index,CString& p_string) const
{
  ASSERT(::IsWindow(m_hWnd));
  if (p_index >= 0 && p_index < GetCount())
  {
    if(GetStyle() & LBS_OWNERDRAWFIXED)
    {
      ListBoxColorLine* listBox = reinterpret_cast<ListBoxColorLine*>(GetItemDataPtr(p_index));
      if(listBox && listBox->m_magic == LIST_MAGIC)
      {
        p_string = listBox->m_text;
      }
    }
    else 
    {
      CListBox::GetText(p_index,p_string);
    }
  }
}

int 
StyleListBox::GetTextLen(int p_index) const
{
  ASSERT(::IsWindow(m_hWnd)); 
  if(p_index >= 0 && p_index < GetCount())
  {
    if(GetStyle() & LBS_OWNERDRAWFIXED)
    {
      ListBoxColorLine* listBox = reinterpret_cast<ListBoxColorLine*>(GetItemDataPtr(p_index));
      if(listBox && listBox->m_magic == LIST_MAGIC)
      {
        return listBox->m_text.GetLength();
      }
    }
    else 
    { 
      return CListBox::GetTextLen(p_index);
    }
  }
  return 0;
}

//////////////////////////////////////////////////////////////////////////
//
// PRIVATE
//
//////////////////////////////////////////////////////////////////////////

// Optimize the updates to the CListBox
void 
StyleListBox::UpdateWidth(LPCTSTR p_string)
{
  CClientDC dc(this);

  CFont* font    = GetFont();
  CFont* oldfont = dc.SelectObject(font);

  // Guard against really long strings like SOAP messages
  int len = 0;
  LPCTSTR pos = _tcschr(p_string,'\n');
  if(pos)
  {
    len = (int)(pos - p_string);
  }
  else
  {
    len = (int) _tcslen(p_string);
  }

  // Find our extent
  CSize sz = dc.GetTextExtent(p_string,len);
  sz.cx += 3 * ::GetSystemMetrics(SM_CXBORDER);
  if(sz.cx > m_width)
  {
    // Extend the max horizontal scroll bar width
    m_width = sz.cx;
    CListBox::SetHorizontalExtent(m_width);
  }
  dc.SelectObject(oldfont);
  ReleaseDC(&dc);
}

void
StyleListBox::AdjustScroll()
{
  SkinScrollWnd* skin = GetSkin();
  if(skin)
  {
    skin->SetTimer(TIMER_UPDATE,FRAME_FIRST_SCROLLBARS,nullptr);
  }
}

void    
StyleListBox::RemoveLineNumber(CString& p_text)
{
  int pos = p_text.Find(_T(": "));
  if(pos >= 0)
  {
    CString number = p_text.Left(pos);
    for (int ind = 0; ind < number.GetLength(); ++ind)
    {
      if (!isdigit(p_text[ind]))
      {
        return;
      }
    }
    p_text = p_text.Mid(pos + 2);
  }
}

void
StyleListBox::RemoveLineInfo()
{
  // See if we have lines left
  int nCount = 0;
  if(::IsWindow(GetSafeHwnd()))
  {
    nCount = GetCount();
  }
  else
  {
    // Nothing to do
    return;
  }
  // Remove our text and color content
  for(int index = 0;index < nCount;index++)
  {
    ListBoxColorLine* line = reinterpret_cast<ListBoxColorLine*>(GetItemDataPtr(index));
    if(line && (line != (ListBoxColorLine*) LB_ERR) && line->m_magic == LIST_MAGIC)
    {
      try
      {
        delete line;
        SetItemDataPtr(index,nullptr);
      }
      catch(...){}
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
// OWNERDRAW PAINTING CODE
//
//////////////////////////////////////////////////////////////////////////

static const char* DebugRect(const RECT* rect)
{
  static char buffer[51];
  sprintf_s(buffer,50,"top=%d,left=%d,right=%d,bottom=%d",rect->top,rect->left,rect->right,rect->bottom);
  return buffer;
}

void
StyleListBox::Internal_Paint(CDC* p_cdc)
{
  int    item_height = GetItemHeight(0);
  int    items = GetCount();
  int    style = GetStyle();
  CRect  clientrect;
  RECT   focusRect = { -1,-1,-1,-1 };

  // Special case! Do not paint
  if(style & LBS_NOREDRAW)
  {
    return;
  }

  // Getting the client rectangle
  GetClientRect(&clientrect);
  SCROLLINFO info;
  info.cbSize = sizeof(SCROLLINFO);
  GetScrollInfo(SB_HORZ,&info);
  if(info.nPos > 0)
  {
    int shift = info.nPos + ::GetSystemMetrics(SM_CXHSCROLL);
    SetWindowOrgEx(p_cdc->GetSafeHdc(),shift,0,nullptr);
    clientrect.right += shift;
  }
  CRect itemrect(clientrect);

  // Should get the brush from the parent normally
  // hbrush = (HBRUSH)SendMessageW(GetParent()->GetSafeHwnd(),WM_CTLCOLORLISTBOX,(WPARAM)p_cdc->GetSafeHdc(),(LPARAM)GetSafeHwnd());
  CBrush brush;
  brush.CreateSolidBrush(ThemeColor::GetColor(Colors::ColorCtrlBackground));
  HBRUSH oldBrush = (HBRUSH) p_cdc->SelectObject(brush);

  if(!IsWindowEnabled())
  {
    p_cdc->SetTextColor(GetSysColor(COLOR_GRAYTEXT));
  }

  // Paint all the items, regarding the selection 
  // Focus state will be painted after

  int top_item   = GetTopIndex();
  int focus_item = GetCaretIndex();
  
  HFONT oldFont = (HFONT) p_cdc->SelectObject(GetFont());

  for(int index = top_item; index < items; index++)
  {
    itemrect.bottom = itemrect.top + item_height;

    /* keep the focus rect, to paint the focus item after */
    if (index == focus_item)
    {
      focusRect = itemrect;
    }
    Internal_PaintItem(p_cdc,&itemrect,index,ODA_DRAWENTIRE,TRUE);
    itemrect.top = itemrect.bottom;

    if(itemrect.top >= clientrect.Height())
    {
      break;
    }
  }

  /* Paint the focus item now */
  if(focusRect.top != focusRect.bottom && focus_item >= 0)
  {
    Internal_PaintItem(p_cdc,&focusRect, focus_item, ODA_FOCUS, FALSE);
  }

  /* Clear the remainder of the client area */
  p_cdc->SelectObject(brush);
  if (itemrect.top < clientrect.Height())
  {
    itemrect.bottom = clientrect.bottom;
    p_cdc->ExtTextOut(0, 0, ETO_OPAQUE | ETO_CLIPPED,&itemrect, NULL, 0, NULL);
  }
  if(itemrect.right < clientrect.Width())
  {
    itemrect.left   = itemrect.right;
    itemrect.right  = clientrect.right;
    itemrect.top    = 0;
    itemrect.bottom = clientrect.bottom;
    p_cdc->ExtTextOut(0, 0, ETO_OPAQUE | ETO_CLIPPED,&itemrect, NULL, 0, NULL);
  }

  // Reset old values in DC
  if (oldFont)  p_cdc->SelectObject(oldFont);
  if (oldBrush) p_cdc->SelectObject(oldBrush);
}

void 
StyleListBox::Internal_PaintItem(CDC* p_cdc,const RECT* rect,INT index,UINT action,BOOL ignoreFocus)
{
  int  nb_items = GetCount();
  bool selected = GetSel(index);
  bool focused  = GetCaretIndex() == index;
  bool enabled  = IsWindowEnabled();

  ListBoxColorLine  line;
  ListBoxColorLine* line_ptr = &line;
  DRAWITEMSTRUCT    dis;

  if(GetStyle() & (LBS_OWNERDRAWFIXED | LBS_OWNERDRAWVARIABLE))
  {

    line_ptr = reinterpret_cast<ListBoxColorLine*>(GetItemDataPtr(index));
    if(!line_ptr || (line_ptr == (ListBoxColorLine*) LB_ERR) || line_ptr->m_magic != LIST_MAGIC)
    {
      TRACE("called with an out of bounds index %d (Total: %d) in owner draw, Not good.\n",index,GetCount());
      return;
    }
  }
  else
  {
    // Get the text from the control
    CListBox::GetText(index,line.m_text);
  }

  if (index >= nb_items)
  {
    if(action == ODA_FOCUS)
    {
      p_cdc->DrawFocusRect(rect);
    }
    else
    {
      TRACE("called with an out of bounds index %d(%d) in owner draw, Not good.\n", index, nb_items);
    }
    return;
  }
  dis.CtlType       = ODT_LISTBOX;
  dis.CtlID         = (UINT)GetWindowLongPtrW(GetSafeHwnd(), GWLP_ID);
  dis.hwndItem      = GetSafeHwnd();
  dis.itemAction    = action;
  dis.hDC           = p_cdc->GetSafeHdc();
  dis.itemID        = index;
  dis.itemState     = 0;
  if (selected)     dis.itemState |= ODS_SELECTED;
  if (focused)      dis.itemState |= ODS_FOCUS;
  if (!enabled)     dis.itemState |= ODS_DISABLED;
  dis.itemData      = (ULONG_PTR) line_ptr;
  dis.rcItem        = *rect;
  // TRACE("[%p]: drawitem %d (%s) action=%02x state=%02x rect=%s\n",GetSafeHwnd(),index,item_str, action, dis.itemState, DebugRect(rect));

  // This is the reason we are here!
  // Standard MS-Windows sends this to the parent of the control so CMenu can have a go at it
  // But the standard desktop does NOT reflect the MEASUREITEM/DRAWITEM messages
  // So we send it directly to ourselves!!!!!!
  // 
  SendMessage(WM_DRAWITEM,(WPARAM)dis.CtlID,(LPARAM)&dis);
}

void
StyleListBox::SetFontSize(int p_size)
{
  m_fontSize = p_size;
  ResetFont();
}

void
StyleListBox::SetFontStyle(bool p_bold,bool p_italic,bool p_underLine)
{
  m_bold      = p_bold;
  m_italic    = p_italic;
  m_underLine = p_underLine;
  ResetFont();
}

void
StyleListBox::SetFontName(CString p_fontName,int p_fontSize,BYTE p_language)
{
  m_fontName = p_fontName;
  m_fontSize = p_fontSize;
  m_language = p_language;
  ResetFont();
}

void
StyleListBox::ResetFont()
{
  LOGFONT  lgFont;

  lgFont.lfCharSet        = m_language;
  lgFont.lfClipPrecision  = 0;
  lgFont.lfEscapement     = 0;
  _tcscpy_s(lgFont.lfFaceName,LF_FACESIZE,m_fontName);
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
