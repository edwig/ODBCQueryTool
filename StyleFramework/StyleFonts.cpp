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

LOGFONT
StyleFonts::MakeLOGFONTFromString(CString fontstring)
{
  LOGFONT lf;
  memset(&lf, 0, sizeof(lf));

  strcpy_s(lf.lfFaceName, 32, StyleFontName);
  lf.lfWeight = FW_NORMAL;
  lf.lfHeight = -MulDiv(12, 96, StyleFonts::logpixelsy());

  std::vector<CString> fontParts;
  Split(fontstring,";",fontParts);

  if (fontParts.size() == 3)
  {
    strcpy_s(lf.lfFaceName, 32, fontParts[0]);
    lf.lfHeight = -MulDiv(atoi(fontParts[1]), logpixelsy(), 72);
    lf.lfWeight = atoi(fontParts[2]);
  }

  return lf;
}

StyleFonts::StyleFonts()
{
  LOGFONT lf = MakeLOGFONTFromString(DialogFontString);
  DialogTextFont.CreateFontIndirect(&lf);

  lf = MakeLOGFONTFromString(DialogFontBoldString);
  DialogTextFontBold.CreateFontIndirect(&lf);

  lf = MakeLOGFONTFromString(ErrorFontString);
  ErrorTextFont.CreateFontIndirect(&lf);

  lf = MakeLOGFONTFromString(CaptionFontString);
  CaptionTextFont.CreateFontIndirect(&lf);

  lf = MakeLOGFONTFromString(ListFontString);
  ListTextFont.CreateFontIndirect(&lf);
}

int StyleFonts::logpixelsy()
{
  CWindowDC dc(0);
  return dc.GetDeviceCaps(LOGPIXELSY);
}