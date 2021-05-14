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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// StyleListBox

IMPLEMENT_DYNAMIC(StyleListBox,CListBox)

StyleListBox::StyleListBox()
             :m_skin(nullptr)
{
}

StyleListBox::~StyleListBox()
{
  ResetSkin();
  OnNcDestroy();
}

BEGIN_MESSAGE_MAP(StyleListBox,CListBox)
  ON_WM_PAINT()
  ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

void 
StyleListBox::PreSubclassWindow()
{
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
    COLORREF color = (this == GetFocus()) ? ThemeColor::_Color1 : ThemeColor::_Color2;
    skin->DrawFrame(color);
  }
}

void
StyleListBox::OnPaint()
{
  if(!m_inPaint)
  {
    m_inPaint = true;
    CListBox::OnPaint();
    DrawFrame();
    m_inPaint = false;
  }
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
  int result = CListBox::AddString(p_string);
  if(result < 0)
  {
    return result;
  }
  UpdateWidth(p_string);
  AdjustScroll();
  return result;
}

int 
StyleListBox::InsertString(int p_index,LPCTSTR p_string)
{
  int result = CListBox::InsertString(p_index,p_string);
  if(result < 0)
  {
    return result;
  }
  UpdateWidth(p_string);
  AdjustScroll();
  return result;
}

void StyleListBox::ResetContent()
{
  CListBox::ResetContent();
  CListBox::SetHorizontalExtent(0);
  AdjustScroll();
  m_width = 0;
}

// Deleting a string implies recalculating the width
// of all other strings in the control to shrink the
// maximum extend of the horizontal scrollbar
int 
StyleListBox::DeleteString(int p_number)
{
  int result = CListBox::DeleteString(p_number);
  if(result < 0)
  {
    return result;
  }
  CClientDC dc(this);

  CFont* f = CListBox::GetFont();
  dc.SelectObject(f);

  m_width = 0;
  for(int i = 0; i < CListBox::GetCount(); i++)
  {
    /* scan strings */
    CString s;
    CListBox::GetText(i,s);
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
  return result;
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
      result += "\r\n";
    }
    CString text;
    CListBox::GetText(lines[index],text);
    RemoveLineNumber(text);
    result += text;
  }

  // Put the text in a global GMEM_MOVABLE memory handle
  size_t    size = (size_t)result.GetLength() + 1;
  HGLOBAL memory = GlobalAlloc(GHND,size);
  if(memory)
  {
    void* data = GlobalLock(memory);
    if(data)
    {
      strncpy_s((char*)data,size,result.GetString(),size);
    }
    GlobalUnlock(memory);

    // Set the text on the clipboard
    // and transfer ownership of the memory segment
    OpenClipboard();
    EmptyClipboard();
    SetClipboardData(CF_TEXT,memory);
    CloseClipboard();
  }
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

  // Set the text on the clipboard
  // and transfer ownership of the memory segment
  OpenClipboard();
  HANDLE memory = GetClipboardData(CF_TEXT);
  if(memory)
  {
    // Getting clipboard text as a string
    CString text;
    void* data = GlobalLock(memory);
    if(data)
    {
      text = (const char*)data;
    }
    // Insert into the listbox
    while(text.GetLength())
    {
      int pos = text.FindOneOf("\r\n");
      if (pos > 0)
      {
        CString part = text.Left(pos);
        CListBox::InsertString(insert++, part);
        text = text.Mid(pos + 1);
        if (text.Left(1) == "\n")
        {
          text = text.Mid(1);
        }
      }
      else
      {
        // One/Last line insert
        CListBox::InsertString(insert, text);
        text.Empty();
      }
    }

    // Closing the clipboard
    GlobalUnlock(memory);
  }
  CloseClipboard();
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
      CListBox::GetText(index, line);
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
      CListBox::GetText(lines[index],line);
      print.Print(line);
      print.NextLine();
    }
    print.EndDocument();
    result = true;
  }
  delete[] lines;

  return result;
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

  CFont* font    = CListBox::GetFont();
  CFont* oldfont = dc.SelectObject(font);

  // Guard against really long strings like SOAP messages
  int len = 0;
  const char* pos = strchr(p_string,'\n');
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
    skin->PostMessage(WM_TIMER,1,0);
  }
}

void    
StyleListBox::RemoveLineNumber(CString& p_text)
{
  int pos = p_text.Find(": ");
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
