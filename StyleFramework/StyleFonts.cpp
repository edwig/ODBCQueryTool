////////////////////////////////////////////////////////////////////////
//
// File: StyleFonts.cpp
// Function: Font related info and definitions
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
#include "StyleFonts.h"
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

StyleFonts::StyleFonts()
{
}

void
StyleFonts::Init(int p_dpi_y)
{
  m_dpi    = p_dpi_y;
  m_factor = (p_dpi_y * 100) / USER_DEFAULT_SCREEN_DPI;
  ReCreateFonts();
}

int
StyleFonts::GetFactor()
{
  return m_factor;
}

// Logical font structure from a string "FontName;Size;Weight"
LOGFONT
StyleFonts::MakeLOGFONTFromString(CString fontstring)
{
  LOGFONT lf;
  memset(&lf, 0, sizeof(lf));

  // Fill with default "Verdana;12;100"
  _tcscpy_s(lf.lfFaceName,LF_FACESIZE,StyleFontName);
  lf.lfWeight = FW_NORMAL;
  // lf.lfHeight = -MulDiv(12,96,m_dpi);
  lf.lfHeight = -MulDiv(STANDARDFONTSIZE,m_dpi,72);

  std::vector<CString> fontParts;
  Split(fontstring,_T(";"),fontParts);

  // Set values from the fontstring
  if(fontParts.size() >= 1)
  {
    _tcscpy_s(lf.lfFaceName,LF_FACESIZE,fontParts[0]);
    if(fontParts.size() >= 2)
    {
      lf.lfHeight = -MulDiv(_ttoi(fontParts[1]),m_dpi,72);
      if(fontParts.size() == 3)
      {
        lf.lfWeight = _ttoi(fontParts[2]);
      }
    }
  }
  return lf;
}

CFont* 
StyleFonts::GetFont(StyleFontType p_type)
{
  switch(p_type)
  {
    case StyleFontType::CaptionFont:    return &CaptionTextFont;
    case StyleFontType::DialogFont:     return &DialogTextFont;
    case StyleFontType::DialogFontBold: return &DialogTextFontBold;
    case StyleFontType::EditorFont:     return &EditorTextFont;
    case StyleFontType::ErrorFont:      return &ErrorTextFont;
    default:                            return &DialogTextFont;
  }
}

//////////////////////////////////////////////////////////////////////////
//
// PRIVATE
//
//////////////////////////////////////////////////////////////////////////

void
StyleFonts::ReCreateFonts()
{
  DialogTextFont    .DeleteObject();
  DialogTextFontBold.DeleteObject();
  EditorTextFont    .DeleteObject();
  ErrorTextFont     .DeleteObject();
  CaptionTextFont   .DeleteObject();

  LOGFONT lf = MakeLOGFONTFromString(StyleFontName + CString(_T(";")) + IntegerToString(STANDARDFONTSIZE) + CString(_T(";")) + IntegerToString(FW_NORMAL));
  DialogTextFont.CreateFontIndirect(&lf);

  lf = MakeLOGFONTFromString(StyleFontName + CString(_T(";")) + IntegerToString(STANDARDFONTSIZE) + CString(_T(";")) + IntegerToString(FW_HEAVY));
  DialogTextFontBold.CreateFontIndirect(&lf);

  lf = MakeLOGFONTFromString(StyleFontName + CString(_T(";")) + IntegerToString(ERRORFONTSIZE) + CString(_T(";")) + IntegerToString(FW_BOLD));
  ErrorTextFont.CreateFontIndirect(&lf);

  lf = MakeLOGFONTFromString(StyleFontName + CString(_T(";")) + IntegerToString(CAPTIONTEXTSIZE) + CString(_T(";")) + IntegerToString(FW_BOLD));
  CaptionTextFont.CreateFontIndirect(&lf);

  lf = MakeLOGFONTFromString(EditFontName + CString(_T(";")) + IntegerToString(STANDARDFONTSIZE) + CString(_T(";")) + IntegerToString(FW_BOLD));
  EditorTextFont.CreateFontIndirect(&lf);
}
