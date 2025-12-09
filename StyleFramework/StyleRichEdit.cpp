////////////////////////////////////////////////////////////////////////
//
// File: StyleRichEdit.cpp
// Function: Styled RTF Rich Edit control
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
#include "StyleRichEdit.h"
#include "StyleColors.h"
#include "StyleFonts.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define STYLE_RICHEDIT_MAX_FONTS   10                     // Maximum number of different fonts
#define STYLE_RICHEDIT_DEF_SIZE    (2 * DIALOOGFONTSIZE)  // RTF is in half-pica points, so multiply by 2

using namespace ThemeColor;

IMPLEMENT_DYNAMIC(StyleRichEdit,CRichEditCtrl)

StyleRichEdit::StyleRichEdit()
{
}

BEGIN_MESSAGE_MAP(StyleRichEdit,CRichEditCtrl)
  ON_WM_PAINT()
  ON_WM_HSCROLL()
  ON_WM_VSCROLL()
  ON_WM_SETFONT()
  ON_MESSAGE(WM_DPICHANGED_AFTERPARENT,OnDpiChangedAfter)
END_MESSAGE_MAP()

void
StyleRichEdit::SetBorderColor(int p_color)
{
  if(m_borderColor != p_color)
  {
    m_borderColor = p_color;
    Invalidate();
  }
}

void
StyleRichEdit::SetBorderSize(int p_size)
{
  if(m_borderSize != p_size)
  {
    m_borderSize = p_size;
    Invalidate();
  }
}

// wParam = new DPI, lParam = HMONITOR
LRESULT 
StyleRichEdit::OnDpiChangedAfter(WPARAM wParam, LPARAM lParam)
{
  HMONITOR monitor = reinterpret_cast<HMONITOR>(lParam);
  if(monitor)
  {
    m_factor = GetSFXSizeFactor(monitor);
    Redisplay();
    Invalidate();
  }
  return 0;
}

void
StyleRichEdit::OnSetFont(CFont* pFont,BOOL bRedraw)
{
  // Ignore the WM_SETFONT from the DPI mechanism
}

void StyleRichEdit::OnPaint()
{
  CRichEditCtrl::OnPaint();

  CDC* dc = GetDC();
  CRect rect;
  GetClientRect(&rect);

  int borderColor(m_borderColor);
  if(borderColor < 0)
  {
    borderColor = ThemeColor::GetColor(Colors::AccentColor1);
  }

  for(int index = 0; index < m_borderSize; ++index)
  {
    dc->Draw3dRect(rect,borderColor,borderColor);
    rect.DeflateRect(1,1);
  }
  ReleaseDC(dc);
}

DWORD __stdcall 
EditStreamInCallback(DWORD_PTR dwCookie,LPBYTE pbBuff,LONG cb,LONG *pcb)
{
  CString *psBuffer = (CString *)dwCookie;

  if(cb > psBuffer->GetLength())
  {
    cb = psBuffer->GetLength();
  }
  for(int i=0;i<cb;++i)
  {
    *(pbBuff+i) = (BYTE) psBuffer->GetAt(i);
  }
  *pcb = cb;
  *psBuffer = psBuffer->Mid(cb);
  return 0;
}

DWORD __stdcall 
EditStreamOutCallback(DWORD_PTR dwCookie,LPBYTE pbBuff,LONG cb,LONG *pcb)
{
  CString thisWrite;
  thisWrite.GetBufferSetLength(cb);
  CString *psBuffer = (CString *)dwCookie;

  // Getting all RTF chars
  for (int i=0;i<cb;++i) 
  {
    thisWrite.SetAt(i,*(pbBuff+i));
  }

  // Place in buffer
  *psBuffer += thisWrite;

  // Free temporary
  *pcb = thisWrite.GetLength();
  thisWrite.ReleaseBuffer();
  return 0;
}

// Getting the RTF text out of the control
CString 
StyleRichEdit::GetRTFText()
{
  CString readText;
  EDITSTREAM es;

  es.dwCookie    = (DWORD_PTR)&readText;    // Pass a pointer to the CString to the callback function 
  es.pfnCallback = EditStreamOutCallback;   // Specify the pointer to the callback function.
  es.dwError     = 0;

  // Perform the streaming
  StreamOut(SF_RTF,es); 

  return readText;
}

// Setting RTF text in the control
// Make sure p_text begins with "{\rtf1....}" !!
void
StyleRichEdit::SetRTFText(CString p_text)
{
  if((GetTextMode() & TM_RICHTEXT) == 0)
  {
    SetTextMode(TM_RICHTEXT | TM_SINGLECODEPAGE);
  }
  EDITSTREAM es;
  es.dwCookie    = (DWORD_PTR)&p_text;    // Pass a pointer to the CString to the callback function
  es.pfnCallback = EditStreamInCallback;  // Specify the pointer to the callback function
  es.dwError     = 0;

  // Perform the streaming
  StreamIn(SF_RTF,es); 
}

// Setting text in 3 fonts, colors and sizes
CString
StyleRichEdit::FormatRTFText(CString p_text)
{
  if(!m_factor)
  {
    m_factor = GetSFXSizeFactor(GetSafeHwnd());
  }

  // Keep the unformatted text
  m_unformatted = p_text;

  CString rtfHeader = CreateRTFHeader();
  PrepareRTFText(p_text);
  CString fullRTF = rtfHeader + p_text + _T("}");

  SetRTFText(fullRTF);
  return fullRTF;
}

void
StyleRichEdit::Redisplay()
{
  CString rtfHeader = CreateRTFHeader();
  CString prep(m_unformatted);
  PrepareRTFText(prep);
  SetRTFText(rtfHeader + prep + _T("}"));
}

void 
StyleRichEdit::OnHScroll(UINT nSBCode,UINT nPos,CScrollBar* pScrollBar)
{
  CRichEditCtrl::OnHScroll(nSBCode,nPos,pScrollBar);
  Invalidate();
}

void 
StyleRichEdit::OnVScroll(UINT nSBCode,UINT nPos,CScrollBar* pScrollBar)
{
  CRichEditCtrl::OnVScroll(nSBCode,nPos,pScrollBar);
  Invalidate();
}

CString 
StyleRichEdit::CreateRTFHeader()
{
  CString header;
  header += _T("{\\rtf1\\ansi\\ansicpg1252\\deflang1043\n");

  // Add the font table
  header += _T("{\\fonttbl{\\f0 \\fswiss \\fcharset0 Verdana; }");
  for(auto& font : m_fonts)
  {
    CString fontname = font.second;
    if(!fontname.IsEmpty())
    {
      header.AppendFormat(_T("{\\f%d \\fcharset0 %s; }"),font.first,fontname.GetString());
    }
  }
  header += _T("}\n"); // End font table

  // Add the color table
  header += _T("{\\colortbl; ");
  for(auto& color : m_colors)
  {
    if(color.second >= 0)
    {
      header.AppendFormat(_T("\\red%d\\green%d\\blue%d;"),GetRValue(color.second),GetGValue(color.second),GetBValue(color.second));
    }
  }
  header += _T("\\red0\\green0\\blue0;");   // Last color: BLACK
  header += _T("}\n");                      // End color table

  // Set default font size
  int fontsize = (m_factor * STYLE_RICHEDIT_DEF_SIZE) / 100;

  header.AppendFormat(_T("\\viewkind4\\uc1\\pard\\f0\\fs%d\n"),fontsize);
  header.AppendFormat(_T("\\plain\\f0\\fs%d\n"),fontsize);

  return header;
}

void
StyleRichEdit::PrepareRTFText(CString& p_text)
{
  p_text.Replace(_T("\n"), _T("\\par\n"));
  p_text.Replace(_T("\t"), _T("\\tab "));

  for(auto& fs : m_fonts)
  {
    CString tofind;
    tofind.Format(_T("\\f%d"),fs.first);
    if(p_text.Find(tofind) >= 0)
    {
      CString font;
      int size = GetFontsize(fs.first);
      font.Format(_T("\\f%d\\fs%d"),fs.first,(m_factor * size) / 100);
      p_text.Replace(tofind,font);
    }
  }
}

void
StyleRichEdit::SetFontname(int p_num,CString p_name)
{
  if(p_num > 0 && p_num < STYLE_RICHEDIT_MAX_FONTS)
  {
    if((int)m_fonts.size() != p_num - 1)
    {
      StyleMessageBox(this,_T("Font names must be set in sequence starting from 1"),_T("Error"), MB_OK | MB_ICONERROR);
      return;
    }
    m_fonts[p_num] = p_name;
  }
}

void
StyleRichEdit::SetFontsize(int p_num,int p_size)
{
  if(p_num >= 0 && p_num < STYLE_RICHEDIT_MAX_FONTS)
  {
    if((int)m_sizes.size() != p_num - 1)
    {
      StyleMessageBox(this,_T("Font sizes must be set in sequence starting from 1"),_T("Error"), MB_OK | MB_ICONERROR);
      return;
    }
    m_sizes[p_num] = p_size;
  }
}

void
StyleRichEdit::SetColor(int p_num,int p_color)
{
  if(p_num >= 0 && p_num < STYLE_RICHEDIT_MAX_FONTS)
  {
    if((int)m_colors.size() != p_num - 1)
    {
      StyleMessageBox(this,_T("Font colors must be set in sequence starting from 1"),_T("Error"), MB_OK | MB_ICONERROR);
      return;
    }
    m_colors[p_num] = p_color;
  }
}

CString
StyleRichEdit::GetFontname(int p_num)
{
  RTFFonts::iterator it = m_fonts.find(p_num);
  if(it != m_fonts.end())
  {
    return it->second;
  }
  return StyleFontName;
}

int
StyleRichEdit::GetFontsize(int p_num)
{
  RTFSizes::iterator it = m_sizes.find(p_num);
  if(it != m_sizes.end())
  {
    return it->second;
  }
  return 20;  // 20 half-pica points = 10 point font
}

int
StyleRichEdit::GetColor(int p_num)
{
  RTFColors::iterator it = m_colors.find(p_num);
  if(it != m_colors.end())
  {
    return it->second;
  }
  return 0; // BLACK
}
