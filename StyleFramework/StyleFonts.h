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

// By defining static values, the screen size scaling of MS-Windows 10 works correctly (again)
#define DIALOOGFONTSIZE    10 // MulDiv(10, 96, StyleFonts::logpixelsy())
#define STANDARDFONTSIZE   10 // MulDiv(10, 96, StyleFonts::logpixelsy())
#define ERRORFONTSIZE      12 // MulDiv(12, 96, StyleFonts::logpixelsy())
#define CAPTIONTEXTSIZE    18 // MulDiv(18, 96, StyleFonts::logpixelsy())

#define LISTBOX_ITEMHEIGTH 16 // For StyleListBox

#define CaptionFontString       StyleFontName + CString(";") + IntegerToString(CAPTIONTEXTSIZE)    + CString(";") + IntegerToString(FW_BOLD)
#define DialogFontString        StyleFontName + CString(";") + IntegerToString(STANDARDFONTSIZE)   + CString(";") + IntegerToString(FW_NORMAL)
#define DialogFontBoldString    StyleFontName + CString(";") + IntegerToString(STANDARDFONTSIZE)   + CString(";") + IntegerToString(FW_HEAVY)
#define EditorFontString        EditFontName  + CString(";") + IntegerToString(STANDARDFONTSIZE)   + CString(";") + IntegerToString(FW_NORMAL)
#define ErrorFontString         StyleFontName + CString(";") + IntegerToString(ERRORFONTSIZE)      + CString(";") + IntegerToString(FW_BOLD) 

class StyleFonts
{
public:
  StyleFonts();
  bool SetFactor(int p_factor);
  int  GetFactor();

  static int logpixelsy();
  static LOGFONT MakeLOGFONTFromString(CString fontstring);

  CFont CaptionTextFont;
  CFont DialogTextFont;
  CFont DialogTextFontBold;
  CFont EditorTextFont;
  CFont ErrorTextFont;
private:
  void  ReCreateFonts();

  int   m_factor = 100;
};

extern StyleFonts STYLEFONTS;
