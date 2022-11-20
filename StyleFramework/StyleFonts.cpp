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

// The one and only STYLEFONTS object
StyleFonts STYLEFONTS;

StyleFonts::StyleFonts()
{
  ReCreateFonts();
}

LOGFONT
StyleFonts::MakeLOGFONTFromString(CString fontstring)
{
  LOGFONT lf;
  memset(&lf, 0, sizeof(lf));

  // Fill with default "Verdana;12;100"
  strcpy_s(lf.lfFaceName, 32, StyleFontName);
  lf.lfWeight = FW_NORMAL;
  lf.lfHeight = -MulDiv(12,96,StyleFonts::logpixelsy());

  std::vector<CString> fontParts;
  Split(fontstring,";",fontParts);

  // Set values from the fontstring
  if(fontParts.size() >= 1)
  {
    strcpy_s(lf.lfFaceName,32,fontParts[0]);
    if(fontParts.size() >= 2)
    {
      lf.lfHeight = -MulDiv(atoi(fontParts[1]),logpixelsy(),80);
      if(fontParts.size() == 3)
      {
        lf.lfWeight = atoi(fontParts[2]);
      }
    }
  }
  return lf;
}

int StyleFonts::logpixelsy()
{
  CWindowDC dc(0);
  return dc.GetDeviceCaps(LOGPIXELSY);
}

bool
StyleFonts::SetFactor(int p_factor)
{
  if(p_factor > 50 && p_factor <= 1000)
  {
    m_factor = p_factor;
    ReCreateFonts();
    return true;
  }
  return false;
}

int
StyleFonts::GetFactor()
{
  return m_factor;
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

  LOGFONT lf = MakeLOGFONTFromString(StyleFontName + CString(";") + IntegerToString((STANDARDFONTSIZE * m_factor)/100) + CString(";") + IntegerToString(FW_NORMAL));
  DialogTextFont.CreateFontIndirect(&lf);

  lf = MakeLOGFONTFromString(StyleFontName + CString(";") + IntegerToString((STANDARDFONTSIZE* m_factor)/100) + CString(";") + IntegerToString(FW_HEAVY));
  DialogTextFontBold.CreateFontIndirect(&lf);

  lf = MakeLOGFONTFromString(StyleFontName + CString(";") + IntegerToString((ERRORFONTSIZE * m_factor)/100) + CString(";") + IntegerToString(FW_BOLD));
  ErrorTextFont.CreateFontIndirect(&lf);

  lf = MakeLOGFONTFromString(StyleFontName + CString(";") + IntegerToString((CAPTIONTEXTSIZE * m_factor)/100) + CString(";") + IntegerToString(FW_BOLD));
  CaptionTextFont.CreateFontIndirect(&lf);

  lf = MakeLOGFONTFromString(EditFontName + CString(";") + IntegerToString((STANDARDFONTSIZE * m_factor)/100) + CString(";") + IntegerToString(FW_BOLD));
  EditorTextFont.CreateFontIndirect(&lf);
}
