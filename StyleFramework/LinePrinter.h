////////////////////////////////////////////////////////////////////////
//
// File: LinePrinter.h
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
#pragma once
#include <vector>

typedef enum _lphf
{
  LPHF_NONE           =     0
 ,LPHF_LEFT_TITLE     = 0x001     // Title     on left  side of header/footer
 ,LPHF_LEFT_DATETIME  = 0x002     // Date/time on left  side of header/footer
 ,LPHF_LEFT_PAGENO    = 0x004     // Page: <n> on left  side of header/footer
 ,LPHF_RIGHT_TITLE    = 0x010     // Title     on right side of header/footer
 ,LPHF_RIGHT_DATETIME = 0x020     // Date/time on right side of header/footer
 ,LPHF_RIGHT_PAGENO   = 0x040     // Page: <n> on right side of header/footer
 ,LPHF_ENDING         = 0x100     // *** End of listing ***
}
HeaderFooter;

#define LISTING_LINE     4                        // Thickness of the header/footer line
#define LISTING_LINE_MAX 100                      // Header line cannot be thicker than this
#define LISTING_MIN_LINES 10                      // Minimum of lines to print

using Arguments = std::vector<CString>;

class LinePrinter
{
public:
  // CTOR
  LinePrinter::LinePrinter();
  // DTOR
  LinePrinter::~LinePrinter() = default;
  
  // Open a new printer document for printing in the printer spooler
  bool Document(CString p_printerName
               ,CString p_documentName
               ,CString p_fontName     = "Courier new"
               ,int     p_fontSize     = 9
               ,bool    p_portrait     = true
               ,int     p_numberCopies = 1
               ,int     p_marginTop    = 2
               ,int     p_marginLeft   = 5
               ,int     p_marginBottom = 2
               ,int     p_marginRight  = 5);

  // Close the document for printer commands
  bool EndDocument();

  // Print a text on the current line after last text
  bool Print(CString p_format);

  // Print a formatted text with arguments after last text
  bool Print(CString p_format, Arguments* p_args);

  // Print text on a different line
  bool Print(int        p_lineNumber
            ,CString    p_format
            ,Arguments* p_args);

  // Print text on a different line on a specified position
  bool Print(int        p_lineNumber
            ,int        p_linePosition
            ,CString    p_format
            ,Arguments* p_hArgs);

  // Advance printing position so that this text will print right-margin aligned
  bool RightAdjust(CString p_text);

  // Printing a bitmap, returns the effective height in lines
  int  PrintBitmap(int regel,int position,CString p_bitmap,int p_width);

  // Go to the next page for this document
  bool Page();

  // Go to the next line on the printer page
  bool NextLine();

  // SETTERS

  // Wrap long lines to the next line (possibly even next page). USE MONO-SPACED FONTS!
  bool SetLineWrapping(bool p_wrapping);
  // Set header type for listing
  void SetHeader(HeaderFooter p_header);
  // Set footer type for listing
  void SetFooter(HeaderFooter p_footer);
  // Choose another font for the next printer command
  bool SetFont(CString p_name,int p_size = 10,CString p_attrib = "",int p_foregorund = 0,int p_background = RGB(255,255,255));
  // Set header/footer text specials
  void SetHeaderFooterTextPage(CString p_page);
  void SetHeaderFooterTextDate(CString p_date);
  void SetHeaderFooterTextEnding(CString p_ending);
  void SetHeaderFooterLineWdith(int p_width);

  // GETTERS

  bool GetLineWrapping()     { return m_lineWrapping;  };
  int  GetCharacterHeight()  { return m_charHeight;    };
  int  GetCharacterWidth()   { return m_charWidth;     };
  int  GetPaperWidth()       { return m_width;         };
  int  GetPaperLength()      { return m_height;        };
  int  GetMarginLeft()       { return m_marginLeft;    };
  int  GetMarginRight()      { return m_marginRight;   };
  int  GetMarginTop()        { return m_marginTop;     };
  int  GetMarginBottom()     { return m_marginBottom;  };
  int  GetForgroundColor()   { return m_foreground;    };
  int  GetBackgroundColor()  { return m_background;    };
  
  int  GetCurrentLine()      { return m_printLine;     };
  int  GetCurrentPosition()  { return m_printPos;      };
  int  GetPageNumber()       { return m_pageNumber;    };

private:
  // Private Methods
  CString Format(CString& p_string,Arguments* hArgs);
  double  Round(double p_value, int p_positions);
  int     GetHeaderLines();
  int     GetFooterLines();
  void    PrintHeader();
  void    PrintFooter(bool p_last = false);
  void    PrintHeaderFooter(HeaderFooter p_line);
  void    PrintHeaderFooterLine();
  bool    AdvanceToFooter();

  // Private data
  CDC*    m_dc;              // Device context of the printer
  CFont   m_font;            // FOnt we are printing in
  CRect   m_paper;           // Printable area on the paper (without margins)
  CString m_title;           // Document title (header & spooler document)
  int     m_saveDc;          // Last saved DC
  bool    m_lineWrapping;    // Wrap around long lines to the next line
  int     m_charHeight;      // Height of a char in pixels
  int     m_charWidth;       // Width  of a char in pixels
  int     m_width;           // Width  of the paper in char
  int     m_height;          // Height of the paper in lines
  int     m_marginLeft;      // Margin left   in char
  int     m_marginRight;     // Margin right  in char
  int     m_marginTop;       // Margin top    in char
  int     m_marginBottom;    // Margin bottom in char
  int     m_foreground;      // Foreground color (default black = 0)
  int     m_background;      // Background color (default white = RGB(255,255,255))

  int     m_printLine;       // Current printing line
  int     m_printPos;        // Current line position
  int     m_pageNumber;      // Current page number

  HeaderFooter m_header;     // Header definition
  HeaderFooter m_footer;     // Footer definition
  bool    m_inHeaderFooter;  // Currently printing header/footer
  int     m_headerLineWidth; // Header/footer lines in pixels
  CString m_headerPageText;  // The "Page: " text in the header/footer
  CString m_headerDateText;  // The "Date: " text in the header/footer
  CString m_footerEnding;    // The "*** End of listing ***" text 
};

inline void 
LinePrinter::SetHeader(HeaderFooter p_header)
{
  m_header = (HeaderFooter)((int)m_header | (int)p_header);
}

inline void 
LinePrinter::SetFooter(HeaderFooter p_footer)
{
  m_footer = (HeaderFooter)((int)m_footer | (int)p_footer);
}

inline int
LinePrinter::GetHeaderLines()
{
  return m_header == LPHF_NONE ? 0 : 2;
}

inline int
LinePrinter::GetFooterLines()
{
  return m_footer == LPHF_NONE ? 0 : 2;
}

inline void
LinePrinter::SetHeaderFooterTextPage(CString p_page)
{
  m_headerPageText = p_page;
}

inline void
LinePrinter::SetHeaderFooterTextDate(CString p_date)
{
  m_headerDateText = p_date;
}

inline void
LinePrinter::SetHeaderFooterTextEnding(CString p_ending)
{
  m_footerEnding = p_ending;
}

