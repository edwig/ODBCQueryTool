////////////////////////////////////////////////////////////////////////
//
// File: LinePrinter.cpp
// Function: Printing a bunch of lines to a printer
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
#include "LinePrinter.h"
#include "StyleMessageBox.h"
#include "StyleDIB.h"
#include "StyleTexts.h"
#include <afxdlgs.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CTOR
LinePrinter::LinePrinter()
            :m_dc(nullptr)
            ,m_saveDc(0)
            ,m_lineWrapping(false)
            ,m_charHeight(0)
            ,m_charWidth(0)
            ,m_width(0)
            ,m_height(0)
            ,m_marginLeft(0)
            ,m_marginRight(0)
            ,m_marginTop(0)
            ,m_marginBottom(0)
            ,m_foreground(0)
            ,m_background(RGB(255,255,255))
            ,m_printLine(0)
            ,m_printPos(0)
            ,m_pageNumber(1)
            ,m_header(LPHF_NONE)
            ,m_footer(LPHF_NONE)
            ,m_inHeaderFooter(false)
            ,m_headerLineWidth(LISTING_LINE)
{
  m_headerPageText = GetStyleText(TXT_PRINT_PAGE);  // "Page"
  m_headerDateText = GetStyleText(TXT_PRINT_DATE);  // "Date"
  m_footerEnding   = GetStyleText(TXT_END_LISTING); // "*** End of listing ***"
}

// Wrap long lines to the next line (possibly even next page)
// But only if we have not yet started printing
// Mind you: You must select a mono-spaced font!!
bool
LinePrinter::SetLineWrapping(bool p_wrapping)
{
  if(m_dc == nullptr)
  {
    m_lineWrapping = p_wrapping;
    return true;
  }
  return false;
}

void 
LinePrinter::SetHeaderFooterLineWdith(int p_width)
{
  if(p_width >= 0 && p_width < LISTING_LINE_MAX)
  {
    m_headerLineWidth = p_width;
  }
}

// Open a new printer document for printing in the printer spooler
bool 
LinePrinter::Document(CString p_printerName
                     ,CString p_documentName
                     ,CString p_fontName
                     ,int     p_fontSize
                     ,bool    p_portret
                     ,int     p_numberCopies
                     ,int     p_topMargin
                     ,int     p_leftMargin
                     ,int     p_bottomMargin
                     ,int     p_rightMargin)
{
  // Keep the title
  m_title = p_documentName;

  // If already printer context: close it, we will make a new one
  if(m_dc)
  {
    if(m_dc->IsPrinting())
    {
      m_dc->AbortDoc();
    }
    m_dc->DeleteDC();
  }
  m_dc = new CDC();

  // All sanity checks (hard-coded!)
  if(p_numberCopies <   1) p_numberCopies  =   1;
  if(p_numberCopies > 500) p_numberCopies  = 500;
  if(p_fontSize     <   4) p_fontSize      =   4;
  if(p_fontSize     >  72) p_fontSize      =  72;
  if(p_topMargin    <   0) p_topMargin     =   0;
  if(p_topMargin    >  50) p_topMargin     =  50;
  if(p_leftMargin   <   0) p_leftMargin    =   0;
  if(p_leftMargin   > 100) p_leftMargin    = 100;
  if(p_bottomMargin <   0) p_bottomMargin  =   0;
  if(p_bottomMargin >  50) p_bottomMargin  =  50;
  if(p_rightMargin  <   0) p_rightMargin   =   0;
  if(p_rightMargin  > 100) p_rightMargin   = 100;

  m_marginTop    = p_topMargin;
  m_marginLeft   = p_leftMargin;
  m_marginBottom = p_bottomMargin;
  m_marginRight  = p_rightMargin;

  // Prepare the printer dialog
  CPrintDialog pd(true);

  if (!pd.GetDefaults())
  {
    return false;
  }
  pd.m_pd.Flags &= ~PD_RETURNDEFAULT;

  DEVMODE *pDevmode       = pd.GetDevMode();
  pDevmode->dmOrientation = (short)(p_portret ? DMORIENT_PORTRAIT : DMORIENT_LANDSCAPE);
  pDevmode->dmFields      |= DM_ORIENTATION;
  GlobalUnlock(pDevmode);

  pd.m_pd.Flags    |= PD_RETURNDC;
  pd.m_pd.nMinPage  = 1;
  pd.m_pd.nMaxPage  = 32000;
	pd.m_pd.nFromPage = 1;
	pd.m_pd.nToPage   = 32000;
  pd.m_pd.nCopies   = (WORD) p_numberCopies;
  pd.m_pd.lpPrintTemplateName = (LPCSTR) p_printerName.GetString();

  // Get the page orientation of the printer from the user
  if (pd.DoModal() != IDOK)
  {
    return false;
  }
  m_dc->Attach(pd.GetPrinterDC());
  m_saveDc = m_dc->SaveDC();
	m_dc->m_bPrinting = TRUE;

  m_dc->SetBkMode(TRANSPARENT);
  
  // Getting the device context en create the 'normal ' font
  m_dc->SelectStockObject(SYSTEM_FONT);
  if (m_font.m_hObject == NULL)
  {
    m_font.CreateFont(-MulDiv(p_fontSize, m_dc->GetDeviceCaps(LOGPIXELSY), 72),
                      0,0,0,FW_NORMAL,0,0,0,
                      ANSI_CHARSET,
                      OUT_DEFAULT_PRECIS,
                      CLIP_DEFAULT_PRECIS,
                      DEFAULT_QUALITY,
                      DEFAULT_PITCH | FF_MODERN,
                      (LPCSTR)p_fontName);
  }

  // Calculate the paper size. Font extensions can only be calculated after the
  // font has been set to the printer device context
  m_dc->SelectObject(m_font);
  CSize size   = m_dc->GetTextExtent("W");
  m_charHeight = size.cy;
  m_charWidth  = size.cx;
	m_paper.SetRect(0,0,m_dc->GetDeviceCaps(HORZRES),m_dc->GetDeviceCaps(VERTRES));
  m_paper.left   += m_charWidth  * m_marginLeft;
  m_paper.bottom -= m_charHeight * m_marginBottom;
  m_paper.top    += m_charHeight * m_marginTop;
  m_paper.right  -= m_charWidth; // Minimal margin for laser printers

  // Re-calculate the height and width from the paper in the print dialog
  // as given in by the end-user
  m_height  = (m_paper.bottom - m_paper.top)  / (m_charHeight + 2);
  m_width   = (m_paper.right  - m_paper.left) /  m_charWidth;
  if(m_height < LISTING_MIN_LINES)
  {
    // Not enough space. Must have at least 10 lines to print on a page
    StyleMessageBox(nullptr,GetStyleText(TXT_NO_SPACE),GetStyleText(TXT_ERROR),MB_OK|MB_ICONERROR);
    EndDocument();
    return false;
  }
  // Create the document-info to send the document as a logical unit to the printer spooler system
  DOCINFO info;
  info.cbSize       = sizeof(DOCINFO);
  info.lpszDocName  = (LPCSTR) p_documentName.GetString();
  info.lpszOutput   = NULL;
  info.lpszDatatype = NULL;
  // This document is a 'per line' type. Optimalisation on some versions of the OS
  info.fwType       = DI_APPBANDING;  // Currently only Windows95, ignored under Windows-NT type OS

  // Standard colors
  m_dc->SetTextColor(m_foreground = 0);               // black
  m_dc->SetBkColor(m_background = RGB(255,255,255));  // white
  // Start a printer document in the printer spooler
  m_dc->StartDoc(&info);
  m_dc->StartPage();

  // Initialise the counters
  m_printLine   = 0;
  m_printPos    = 0;
  m_pageNumber  = 1;
  return true;
}

// Close the document for printer commands
bool 
LinePrinter::EndDocument()
{
  // Close document in the printer spooler
  if(m_dc)
  {
    // Closing footer
    if(GetFooterLines())
    {
      PrintFooter(true);
    }

    // Close the document on the printer spooler system
    if(m_dc->m_hDC)
    {
      m_dc->EndPage();
      m_dc->EndDoc();
    }

    // Deallocate DC's and objects
    m_dc->RestoreDC(m_saveDc);
    m_dc->Detach();
    m_dc->DeleteDC();
    delete m_dc;
    m_dc = nullptr;

    // Release last font
    if (m_font.m_hObject != nullptr)
    {
      ::DeleteObject(m_font);
    }
    return true;
  }
  return false;
}

// PRINT( <line>,<position>,"TEXT FORMAT", param1 [,param2 [, ...]])
// PRINT( <line>,"TEXT FORMAT", param1 [,param2 [, ...]])
// PRINT( "TEXT FORMAT", param1 [, param2 [, ... ]])
// PRINT( "TEXT ONLY")
//
// Print text on a line...
//
bool
LinePrinter::Print(CString p_format)
{
  Arguments args;
  return Print(0,0,p_format,&args);
}

bool
LinePrinter::Print(CString p_format,Arguments* p_args)
{
  return Print(0,0,p_format,p_args);
}

bool
LinePrinter::Print(int        p_lineNumber
                  ,CString    p_format
                  ,Arguments* p_args)
{
  return Print(p_lineNumber,0,p_format,p_args);
}

bool 
LinePrinter::Print(int        p_lineNumber
                  ,int        p_linePosition
                  ,CString    p_format
                  ,Arguments* p_args)
{
  if(!m_dc)
  {
    return false;
  }
  CString printString = Format(p_format,p_args);
  CRect rect;
  rect.CopyRect(m_paper);

  // Different print line given?
  if((p_lineNumber >= 1) && (p_lineNumber <= m_height))
  {
    m_printLine = p_lineNumber - 1;
  }

  // Different print position given?
  if((p_linePosition >= 1) && (p_linePosition <= m_width))
  {
    m_printPos = (p_linePosition - 1) * m_charWidth;
  }

  if((p_lineNumber > 0) && (p_linePosition == -1))
  {
    // Just a line number. So use default print position (left side!)
    m_printPos = 0;
  }

  // See if we must do an overflow
  CString overflowString;
  if(m_lineWrapping)
  {
    int left = rect.left + m_printPos;
    int spaceForChars  = (rect.right - left) / m_charWidth;
        spaceForChars -= (m_marginRight > 1 ? m_marginRight - 2 : 0);
    if(spaceForChars < printString.GetLength())
    {
      overflowString = printString.Mid (spaceForChars);
      printString    = printString.Left(spaceForChars);
    }
  }

  // See if we must print a footer on the bottom
  if(!m_inHeaderFooter && GetFooterLines())
  {
    int spacing = rect.top + ((GetFooterLines() + m_printLine) * (m_charHeight + 2));
    if(spacing > rect.bottom)
    {
      PrintFooter();
    }
  }

  // See if we must do a header on the top of the paper
  // Even last footer could have advanced us to the next page
  if (!m_inHeaderFooter && m_printLine == 0 && m_printPos == 0 && GetHeaderLines())
  {
    PrintHeader();
  }

  // The place where we will be printing (line banding)
  rect.top    += m_printLine * (m_charHeight + 2);
  rect.bottom  = rect.top    + m_charHeight;
  rect.left   += m_printPos;

  // Here comes the text line
  m_dc->DrawText(printString,rect,DT_NOPREFIX | DT_SINGLELINE);

  // Shift the EXACT size to the right
  // Can be different than: aantal_chars * m_tekenBreedte
  CSize size = m_dc->GetOutputTextExtent(printString);
  m_printPos += size.cx;

  // Any overflow left? Call ourselves recursively
  if(overflowString.GetLength())
  {
    NextLine();
    Print(overflowString);
  }
  return true;
}

void
LinePrinter::PrintHeader()
{
  m_inHeaderFooter = true;
  PrintHeaderFooter(m_header);
  PrintHeaderFooterLine();
  m_inHeaderFooter = false;
}

void
LinePrinter::PrintFooter(bool p_last /*= false*/)
{
  m_inHeaderFooter = true;
  if(p_last)
  {
    if(m_footer & LPHF_ENDING)
    {
      NextLine();
      Print(m_footerEnding);
    }
    AdvanceToFooter();
  }
  PrintHeaderFooterLine();
  PrintHeaderFooter(m_footer);
  m_inHeaderFooter = false;
}

void
LinePrinter::PrintHeaderFooter(HeaderFooter p_line)
{
  CString printLeft;
  CString printRight;

  // Title left or right
  if(p_line & LPHF_LEFT_TITLE)
  {
    printLeft = m_title;
  }
  if (p_line & LPHF_RIGHT_TITLE)
  {
    printRight = m_title;
  }

  // Page number left or right
  if(p_line & LPHF_LEFT_PAGENO || p_line & LPHF_RIGHT_PAGENO)
  {
    CString date;
    date.Format("%s: %d",m_headerPageText,m_pageNumber);
    if (p_line & LPHF_LEFT_PAGENO)
    {
      printLeft = date;
    }
    else
    {
      printRight = date;
    }
  }

  // Current timestamp left or right
  if(p_line & LPHF_LEFT_DATETIME || p_line & LPHF_RIGHT_DATETIME)
  {
    CString    time;
    __time64_t datetime;
    _time64(&datetime);
    struct tm now;
    _localtime64_s(&now,&datetime);
    time.Format("%s: %02.2d-%02.2d-%4.4d %02.2d:%02.2d:%02.2d"
               ,m_headerDateText
               ,now.tm_mday,now.tm_mon,now.tm_year + 1900
               ,now.tm_hour,now.tm_min,now.tm_sec);
    if (p_line & LPHF_LEFT_DATETIME)
    {
      printLeft = time;
    }
    else
    {
      printRight = time;
    }
  }
  if(printLeft.GetLength())
  {
    // Printpos = 1, so we are sure to start left
    Arguments args;
    Print(0,1,printLeft,&args);
  }
  if(printRight.GetLength())
  {
    RightAdjust(printRight);
    Print(printRight);
  }
  NextLine();
}

// Returns true if we do go downward on the page
bool
LinePrinter::AdvanceToFooter()
{
  int newline = m_height - GetFooterLines();
  bool result = m_printLine < newline;
  m_printLine = newline;

  return result;
}

void
LinePrinter::PrintHeaderFooterLine()
{
  CRect rect;
  rect.CopyRect(m_paper);

  // Where we draw our line
  rect.top   += (m_printLine  * (m_charHeight + 2)) + (m_charHeight / 2);
  rect.right -= (m_marginRight - 1) * m_charWidth;

  // Create pen and draw the line
  CPen pen(PS_SOLID,m_headerLineWidth,m_foreground);
  HGDIOBJ oldpen = m_dc->SelectObject(pen);
  m_dc->MoveTo(rect.left, rect.top);
  m_dc->LineTo(rect.right,rect.top);
  m_dc->SelectObject(oldpen);

  NextLine();
}

// Advance printing position so that this text will print right-margin aligned
// Returns true  if we can safely print on this position
// Returns false if we might overwrite already printed context
bool 
LinePrinter::RightAdjust(CString p_text)
{
  CRect rect;
  rect.CopyRect(m_paper);

  // Getting the right margin of the printable region
  int margin = rect.right - ((m_marginRight - 1) * m_charWidth);
  // Getting the size of the text to print
  CSize size = m_dc->GetOutputTextExtent(p_text);
  // This will be the new printing position
  int newpos = margin - size.cx - rect.left;

  // If we will ADVANCE the position, result is TRUE
  bool result = m_printPos < newpos;
  
  // New printing position
  m_printPos = newpos;
  return result;
}

// Choose another font for the next printer command
bool 
LinePrinter::SetFont(CString p_fontName
                    ,int     p_fontSize     /* = 10               6-72  */
                    ,CString p_attributes   /* = ""               BUI   */
                    ,int     p_foreground   /* = RGB(0,0,0)       BLACK */
                    ,int     p_background)  /* = RGB(255,255,255) WHITE */
{
  if(m_dc)
  {
    int  weight    = FW_NORMAL;
    bool italic    = false;
    bool underline = false;
    p_attributes.MakeUpper();
    if(p_attributes.Find("B",0) >= 0)   weight    = FW_BOLD; // BOLD
    if(p_attributes.Find("V",0) >= 0)   weight    = FW_BOLD; // VET
    if(p_attributes.Find("U",0) >= 0)   underline = true;    // UNDERLINE
    if(p_attributes.Find("O",0) >= 0)   underline = true;    // ONDERSTREEPT
    if(p_attributes.Find("I",0) >= 0)   italic    = true;    // ITALIC
    if(p_attributes.Find("C",0) >= 0)   italic    = true;    // CURSIEF

    if(p_fontSize <= 4)
    {
       p_fontSize = 4; // Minimal font size
    }
    if(p_foreground < 0 || p_foreground > RGB(255,255,255))
    {
      p_foreground = 0; // black
    }
    if(p_background < 0 || p_background > RGB(255,255,255))
    {
      p_background = RGB(255,255,255); // White
    }
    m_font.DeleteObject();
    //::DeleteObject(m_font);
    m_font.CreateFont(-MulDiv(p_fontSize, m_dc->GetDeviceCaps(LOGPIXELSY), 72) // hoogte
                     ,0  // width
                     ,0  // escapement
                     ,0  // orientation
                     ,weight
                     ,italic
                     ,underline
                     ,0   // strikeout
                     ,ANSI_CHARSET
                     ,OUT_DEFAULT_PRECIS
                     ,CLIP_DEFAULT_PRECIS
                     ,DEFAULT_QUALITY
                     ,DEFAULT_PITCH  // | FF_MODERN
                     ,p_fontName);

    // Reselect the font object
    m_dc->SelectObject(m_font);
    // Use colors
    m_foreground = p_foreground;
    m_background = p_background;

    // Set the colors to the DC
    m_dc->SetTextColor(m_foreground);
    m_dc->SetBkColor(m_background);
    // Do not print the white background!!
    m_dc->SetBkMode((m_background == RGB(255,255,255)) ? TRANSPARENT : OPAQUE);
  }
  return false;
}

// Go to the next page for this document
bool 
LinePrinter::Page()
{
  if(m_dc)
  {
    // New page for the document to the printer spooler
    m_dc->EndPage();
    m_dc->StartPage();
    m_printLine = 0;
    m_printPos  = 0;
    m_pageNumber++;
    return true;
  }
  return false;
}

// Go to the next line on the printer page
bool 
LinePrinter::NextLine()
{
  if(m_dc)
  {
    // To the next line 
    m_printPos = 0;   // Carriage return
    ++m_printLine;    // Line feed
    if(m_printLine > m_height)
    {
      return Page();
    }
    return true;
  }
  return false;
}

//////////////////////////////////////////////////////////////////////////////////
//
// Special format
//
//////////////////////////////////////////////////////////////////////////////////

CString
LinePrinter::Format(CString& p_string,Arguments* p_args)
{
  int maxLen   = p_string.GetLength();
  int position = 0;
  int argnum   = 0;
  CString answer;
  char    cc;

  for(position = 0; position < maxLen; ++position)
  {
    cc = p_string.GetAt(position);
    if(cc == '$')
    {
      // Found a format
      int  width     = 0;
      int  precision = 2;
      bool left      = false;
      bool right     = false;
      int  dpos;

      if(++position >= maxLen) return answer;
      cc = p_string.GetAt(position);
      if(cc == '-' || cc == '+')
      {
        if(cc == '-') left  = true;
        if(cc == '+') right = true;
        if(++position >= maxLen) return answer;
        cc = p_string.GetAt(position);
      }
      while(isdigit(cc))
      {
        width *= 10;
        width += (cc - '0');
        if(++position >= maxLen) return answer;
        cc = p_string.GetAt(position);
      }
      if(cc == '.')
      {
        if(++position >= maxLen) return answer;
        cc = p_string.GetAt(position);
        while(isdigit(cc))
        {
          precision *= 10;
          precision += (cc - '0');
          if(++position >= maxLen) return answer;
          cc = p_string.GetAt(position);
        }
      }
      if(argnum >= (int) p_args->size())
      {
        // "More escape characters($) in printer string than arguments!"
        CString error = GetStyleText(TXT_X_ARGUMENTS);
        StyleMessageBox(nullptr,error,GetStyleText(TXT_ERROR),MB_OK|MB_ICONERROR);
        throw error;
      }
      double  number;
      CString extra;
      switch(cc)
      {      
        case 'G': number = atof(p_args->at(argnum++));
                  number = Round(number,precision);
                  extra.Format("%g",number);
                  dpos  = extra.Find('.');
                  if(dpos >= 0)
                  {
                    if(extra.GetLength() > (dpos + precision))
                    {
                      extra = extra.Left(dpos + precision);
                    }
                  }
                  break;
        case 'B': if(atoi(p_args->at(argnum++)))
                  {
                    extra = GetStyleText(TXT_BOOLEAN_TRUE); // TRUE
                  }
                  else
                  {
                    extra = GetStyleText(TXT_BOOLEAN_FALSE); // FALSE
                  }
                  break;
        case 'S': // Fall through
        case 'I': // Fall through
        case 'D': // Fall through
        case 'T': // Fall through
        case 'M': extra = p_args->at(argnum++);
                  break;
        case 'P': extra.Format("%d",m_pageNumber);
                  break;
        case '$': extra += cc; // Process the $$
                  break;
        default:  // Really bad error
                  { CString error;
                    CString format = GetStyleText(TXT_U_ARGUMENTS);
                    error.Format(format,cc,p_string.GetString());
                    StyleMessageBox(nullptr,error,GetStyleText(TXT_ERROR),MB_OK | MB_ICONERROR);
                    throw error;
                  }
                  break;
      }
      while(left && extra.GetLength() < width)
      {
        extra = extra + " ";
      }
      while(right && extra.GetLength() < width)
      {
        extra = " " + extra;
      }
      // Add to the answer
      answer += extra;
      continue;
    }
    // No formatting or ready with formatting
    answer += cc;
  }
  return answer;
}

// Printing a bitmap, returns the effective height in lines
int
LinePrinter::PrintBitmap(int     p_lineNumber
                        ,int     p_linePosition
                        ,CString p_bitmap
                        ,int     p_width)
{
  StyleDIB afb;
  CRect rect;
  // Load a bitmap, if not loaded yet
  afb.Load((LPSTR)p_bitmap.GetString());
  if((afb.GetWidth() == 0) && (afb.GetHeight() == 0))
  {
    CString error = GetStyleText(TXT_NO_BMP_FILE) + p_bitmap;
    StyleMessageBox(nullptr,error,GetStyleText(TXT_ERROR),MB_OK|MB_ICONERROR);
    return 0;
  }
  // Get the rect of the loaded bitmap
  afb.GetRect(&rect);

  if((p_lineNumber >= 1) && (p_lineNumber <= m_height))
  {
    m_printLine = p_lineNumber - 1;
  }
  // Possibly on a different position on the line
  if((p_linePosition >= 1) && (p_linePosition <= m_width))
  {
    m_printPos = (p_linePosition - 1) * m_charWidth;
  }
  if((p_lineNumber > 0) && (p_linePosition == -1))
  {
    // No default position, reset to the beginning of the line
    m_printPos = 0;
  }

  int width;
  int height  = m_paper.top +  (m_printLine * (m_charHeight + 2));
  int pos     = m_paper.left +  m_printPos;
  if(p_width > 0)
  {
    // Calculate effective width
    width = p_width * m_charWidth;
  }
  else
  {
    // Use standard width of the bitmap itself
    width = rect.right - rect.left;
  }
  int size  = ((rect.bottom - rect.top) * width) / (rect.right - rect.left);
  int lines = ((size - 1) / (m_charHeight + 2)) + 1;

  afb.Draw(m_dc,pos,height,width,size);
  return lines;
}

// Round a number
double  
LinePrinter::Round(double p_value, int p_positions)
{
  if(p_positions <= 0)
  {
    return p_value;
  }
  double power = pow(10, p_positions);
  double value = round(p_value * power);
  return value / power;
}
