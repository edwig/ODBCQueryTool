////////////////////////////////////////////////////////////////////////
//
// File: StyleColors.h
// Function: Color definitions in the styling framework colors
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

// Default gray color from the styling framework
#define FRAME_DEFAULT_COLOR       RGB(0x7F,0x7F,0x7F)

#define Lime1             RGB(0x75, 0xC0, 0x1F)   // Lime color
#define Lime2             RGB(0xD6, 0xFA, 0xAC)
#define Lime3             RGB(0x00, 0x96, 0x3F)
#define LimeHeader        RGB(0x00, 0x98, 0x84)

#define Skyblue1          RGB(0x19, 0xA6, 0xFE)   // Skyblue color
#define Skyblue2          RGB(0xB7, 0xD9, 0xFF)
#define Skyblue3          RGB(0x00, 0x5B, 0xAA)
#define SkyblueHeader     RGB(0x34, 0x9B, 0xDB)

#define Purple1           RGB(0x87, 0x5A, 0xF7)   // Purple color
#define Purple2           RGB(0xC2, 0xA9, 0xFF)
#define Purple3           RGB(0x63, 0x33, 0xC0)
#define PurpleHeader      RGB(0x81, 0x56, 0xED)

#define Mustard1          RGB(0xFD, 0xCA, 0x1C)   // Mustard color
#define Mustard2          RGB(0xC6, 0x9E, 0x2A)
#define Mustard3          RGB(0xA1, 0x80, 0x22)
#define MustardHeader     RGB(0xF0, 0x90, 0x30)

#define ModerateGray1          RGB(0x86, 0xA9, 0xBE)   // ModerateGray color
#define ModerateGray2          RGB(0xC2, 0xE8, 0xF5)
#define ModerateGray3          RGB(0x55, 0x78, 0x95)
#define ModerateGrayHeader     RGB(0x6B, 0x8D, 0xA1)

#define PureGray1         RGB(0xC8, 0xDE, 0xF3)   // PureGray color
#define PureGray2         RGB(0xFF, 0xFF, 0xFF)
#define PureGray3         RGB(0x6C, 0x9D, 0xCB)
#define PureGrayHeader    RGB(0x67, 0x89, 0xa9)

#define BlackWhite1       RGB(0x00, 0x00, 0x00)  // BlackWhite color
#define BlackWhite2       RGB(0x44, 0x58, 0x73)
#define BlackWhite3       RGB(0x00, 0x00, 0x00)
#define BlackWhiteHeader  RGB(0x44, 0x58, 0x73)

#define Dark1             RGB(0x3C, 0x3C, 0x3C) //  RGB(0xBE, 0xBE, 0xBE)
#define Dark2             RGB(0x2C, 0x2C, 0x2C) // 0x99, 0x99, 0x99)
#define Dark3             RGB(0x1D, 0x1D, 0x1D) // (0x4D, 0x4D, 0x4D)
#define DarkHeader        RGB(0x1F, 0x1F, 0x1F)
#define DarkButtonText    RGB(0xDD, 0xDD, 0xDD)

#define Assistant0    RGB(0xFF, 0xFF, 0xFF)
#define Assistant1    RGB(0xFA, 0xFB, 0xFC)
#define Assistant2    RGB(0xF5, 0xF8, 0xFB)
#define Assistant3    RGB(0xEE, 0xF3, 0xF7)
#define Assistant4    RGB(0xE2, 0xEC, 0xF3)
#define Assistant5    RGB(0xCF, 0xDC, 0xE6)
#define Assistant6    RGB(0xB8, 0xC5, 0xD1)
#define Assistant7    RGB(0x9D, 0xAB, 0xBA)
#define Assistant8    RGB(0x7C, 0x8C, 0xA1)
#define Assistant9    RGB(0x44, 0x58, 0x73)
#define Assistant10   RGB(0x00, 0x00, 0x00)

#define Accent0       RGB(0x00, 0x5E, 0xA8)
#define Accent1       RGB(0x5E, 0xC5, 0xED)
#define Accent2       RGB(0xEE, 0x7F, 0x00)
#define Accent3       RGB(0xE3, 0x00, 0x45)
#define Accent4       RGB(0xFF, 0xED, 0x00)
#define Accent5       RGB(0x80, 0x19, 0x7F)
#define Accent6       RGB(0x00, 0x81, 0x85)

#define Labeltext            RGB(0x65, 0x7C, 0x9B)
#define InputTextActive      RGB(0x00, 0x00, 0x00)
#define InputTextInactive    RGB(0x00, 0x00, 0x00)
#define ButtonText           RGB(0x49, 0x52, 0x76)
#define TabTextActive        RGB(0x49, 0x52, 0x76)
#define TabTextInactive      RGB(0xc9, 0xd3, 0xe2)
#define TabTextSelected      RGB(0xFF, 0xFF, 0xFF)
#define DialogText           RGB(0xFF, 0xFF, 0xFF)
#define CellTextActive       RGB(0x49, 0x52, 0x76)
#define CellTextInactive     RGB(0x49, 0x52, 0x76)
#define ComboBoxActive       RGB(0xE5, 0xE8, 0xFF) 
#define ComboBoxDropped      RGB(0xC5, 0xC8, 0xDF)
#define SpinButtonActive     RGB(0xC5, 0xC8, 0xDF)
#define ComboBoxDActive      RGB(0x72, 0x74, 0x7F) 
#define ComboBoxDDropped     RGB(0x62, 0x64, 0x6F)

#define UsersBackground      RGB(0xED, 0xF3, 0xF6)
#define DarkBackground       RGB(0x1F, 0x1F, 0x1F)
#define DarkCtrlBackground   RGB(0x2F, 0x2F, 0x2F)
#define GroupBoxLine         RGB(0xDC, 0xDC, 0xDC)
#define GroupBoxLineDark     RGB(0x50, 0x50, 0x50)

namespace ThemeColor 
{
  enum class Themes
  {
    ThemeLime,          // Theme in color Lime
    ThemeSkyblue,       // Theme in color Skyblue
    ThemePurple,        // Theme in color Purple
    ThemeMustard,       // Theme in color Mustard
    ThemeModerateGray,  // Theme in color ModerateGray
    ThemePureGray,      // Theme in color PureGray
    ThemeBlackWhite,    // Theme in color BlackWhite
    ThemeDark,          // Theme in color DarkContrast
    ThemeMax
  };

  enum class Colors
  {
    AccentColor1
   ,AccentColor2
   ,AccentColor3
   ,AccentColor4
   ,ColorWindowFrame
   ,ColorCtrlBackground
   ,ColorEditText
   ,ColorEditBkgnd
   ,ColorEditHover
   ,ColorLabelText
   ,ColorComboActive
   ,ColorComboDropped
   ,ColorGroupLine
   ,ColorButtonCheck
   ,ColorButtonBackground
   ,ColorButtonText
   ,ColorControlHover
   ,ColorControlPressed
   ,ColorControlFramePressed
   ,ColorControlFrameHover
   ,ColorControlTextPressed
   ,ColorControlTextHover
   ,ColorControlFrameDisabled
   ,ColorControlDisabled
   ,ColorControlTextDisabled
   ,ColorTabTextActive
   ,ColorTabTextInactive
   ,ColorTabFrame
   ,ColorTabBkGndActive
   ,ColorTabBkGndInactive
   ,ColorTabTextSelected
   ,ColorTabBkGndHover
   ,ColorTabTextHover
   ,ColorMax
  };

  // Message that styles have changed
  extern UINT g_msg_changed;

  constexpr unsigned NO_OF_THEMES  = (int)Themes::ThemeMax;
  constexpr unsigned NO_OF_COLORS  = (int)Colors::ColorMax;
  constexpr unsigned NO_OF_FACTORS = 4; // 100%, 125%, 150%, 200%
  extern LPCTSTR     theme_names [NO_OF_THEMES];
  extern const int   theme_colors[NO_OF_COLORS][NO_OF_THEMES];
  extern Themes     _theme;

  extern Themes   GetTheme();
  extern void     SetTheme(Themes pTheme);
  extern int      GetColor(Colors p_color);
  extern COLORREF NoWhite(COLORREF color);
  extern int      GetSkinScrollWidth    (HWND p_hwnd,HMONITOR p_monitor = nullptr);
  extern int      GetBitmapScalingFactor(HWND p_hwnd,HMONITOR p_monitor = nullptr);
  extern HBITMAP  GetScrollbarBitmap    (HWND p_hwnd,HMONITOR p_monitor = nullptr);
  extern COLORREF HalfTone(COLORREF color,double p_factor = 0.5);
}

// Where we store the styles in the registry of MS-Windows
#define STYLECOLORS_KEY   _T("HKCU\\Software\\StyleFramework\\StyleThemes")
#define STYLECOLORS_THEME _T("Theme")

// Window
#define ColorWindowTransparent      RGB(0xFF, 0x00, 0xFF)
#define ColorWindowHeader           ThemeColor::GetColor(Colors::AccentColor1) 
#define ColorWindowHeaderText       ColorWindowHeader == Assistant0 ? Assistant10 : Assistant0
#define ColorWindowHeaderIcon       ThemeColor::GetColor(Colors::AccentColor2) 
#define ColorWindowHeaderIconActive ThemeColor::GetColor(Colors::AccentColor3) 
#define ColorWindowGrayFrame        RGB(0x96, 0x96, 0x96) // 20% graying 
#define ColorWindowFrameError       RGB(0xE3, 0x00, 0x45) // Red for errors
#define ColorWindowFrameTextError   Assistant0
#define ColorWindowTopBarIconText   Assistant8
#define ColorEditFrameError         RGB(0xE3, 0x00, 0x45)
#define ClrWindowMessageError       ClrEditBkgnd
#define ColorWindowMessageTextError RGB(0xFF, 0x00, 0x00)

#define ClrEditText                 InputTextActive
#define ClrEditBkgnd                Assistant0
#define ClrEditDark                 RGB(0xFF,0xFF,0xFF)
#define ClrEditDarkHover            RGB(0x00,0x00,0xFF)

