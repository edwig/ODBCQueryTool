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

#define StyleFontName _T("Verdana")       // Standard font for dialogs
#define EditFontName  _T("Courier New")   // Standard edit font

// By defining static values, the screen size scaling of MS-Windows 10 works correctly (again)
#define DIALOOGFONTSIZE    10
#define STANDARDFONTSIZE   10
#define ERRORFONTSIZE      12
#define CAPTIONTEXTSIZE    18

enum class StyleFontType
{
  CaptionFont,
  DialogFont,
  DialogFontBold, 
  EditorFont,
  ErrorFont
};

class StyleFonts
{
public:
  StyleFonts();

  void    Init(int p_dpi_y);
  int     GetFactor();
  CFont*  GetFont(StyleFontType p_type);
  LOGFONT MakeLOGFONTFromString(CString fontstring);

private:
  void    ReCreateFonts();

  // 100 % by 96 dots per inch (DPI)
  int   m_dpi    { USER_DEFAULT_SCREEN_DPI };
  int   m_factor { 100 };
  // All the fonts
  CFont CaptionTextFont;
  CFont DialogTextFont;
  CFont DialogTextFontBold;
  CFont EditorTextFont;
  CFont ErrorTextFont;
};
