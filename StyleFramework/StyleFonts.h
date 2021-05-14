////////////////////////////////////////////////////////////////////////
//
// File: StyleFonts.h
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
#pragma once
#include "StyleUtilities.h"

// Font names and sizes for the controls

#define StyleFontName "Verdana"       // Standard font for dialogs
#define EditFontName  "Courier New"   // Standard edit font

#define SMALLFONTSIZE      MulDiv( 8, 96, StyleFonts::logpixelsy()) //  8
#define DIALOOGFONTSIZE    MulDiv(10, 96, StyleFonts::logpixelsy()) // 10
#define STANDAARDFONTSIZE  MulDiv(10, 96, StyleFonts::logpixelsy()) // 10
#define ERRORFONTSIZE      MulDiv(12, 96, StyleFonts::logpixelsy()) // 12
#define CAPTIONTEXTSIZE    MulDiv(18, 96, StyleFonts::logpixelsy()) // 18

#define CaptionFontString       StyleFontName + CString(";") + IntegerToString(CAPTIONTEXTSIZE)     + CString(";") + IntegerToString(FW_BOLD)
#define DialogFontString        StyleFontName + CString(";") + IntegerToString(STANDAARDFONTSIZE)   + CString(";") + IntegerToString(FW_NORMAL)
#define DialogFontBoldString    StyleFontName + CString(";") + IntegerToString(STANDAARDFONTSIZE)   + CString(";") + IntegerToString(FW_HEAVY)
#define EditorFontString        EditFontName  + CString(";") + IntegerToString(STANDAARDFONTSIZE)   + CString(";") + IntegerToString(FW_NORMAL)
#define ErrorFontString         StyleFontName + CString(";") + IntegerToString(ERRORFONTSIZE)       + CString(";") + IntegerToString(FW_BOLD) 
#define ListFontString          StyleFontName + CString(";") + IntegerToString(SMALLFONTSIZE)       + CString(";") + IntegerToString(FW_NORMAL)

class StyleFonts
{
public:
  StyleFonts();
  static int logpixelsy();
  static LOGFONT MakeLOGFONTFromString(CString fontstring);

  CFont CaptionTextFont;
  CFont DialogTextFont;
  CFont DialogTextFontBold;
  CFont ErrorTextFont;
  CFont ListTextFont;
};

static StyleFonts STYLEFONTS;
