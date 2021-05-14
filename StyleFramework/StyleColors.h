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

// Default gray color fro the styling framework
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
#define PurpleHeader      RGB(0x81, 0x56, 0XED)

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

#define Assistant4_50 RGB(0xD8, 0xE2, 0xE9)

#define Accent0       RGB(0x00, 0x5E, 0xA8)
#define Accent1       RGB(0x5E, 0xC5, 0xED)
#define Accent2       RGB(0xEE, 0x7F, 0x00)
#define Accent3       RGB(0xE3, 0x00, 0x45)
#define Accent4       RGB(0xFF, 0xED, 0x00)
#define Accent5       RGB(0x80, 0x19, 0x7F)
#define Accent6       RGB(0x00, 0x81, 0x85)

#define Accent0_50    RGB(0x7F, 0xAE, 0xD3)
#define Accent1_50    RGB(0xAE, 0xE2, 0xF6)
#define Accent2_50    RGB(0xF6, 0xB7, 0x7F)
#define Accent3_50    RGB(0xF1, 0x7F, 0xA2)
#define Accent4_50    RGB(0xFF, 0xF6, 0x7F)
#define Accent5_50    RGB(0xC0, 0x8C, 0xBF)
#define Accent6_50    RGB(0x7F, 0xC0, 0xC2)

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

#define VkTextInActive       RGB(0xa0, 0xa3, 0xa2)
#define UsersBackground      RGB(0xED, 0xF3, 0xF6)

namespace ThemeColor 
{
  typedef enum _themes
  {
    ThemeLime,          // Theme in color Lime
    ThemeSkyblue,       // Theme in color Skyblue
    ThemePurple,        // Theme in color Purple
    ThemeModerateGray,  // Theme in color ModerateGray
    ThemePureGray,      // Theme in color PureGray
    ThemeBlackWhite     // Theme in color BlackWhite
  }
  Themes;

  // Message that styles have changed
  extern UINT g_msg_changed;

  const  unsigned NO_OF_THEMES = 6;
  extern char*    theme_names[];
  extern COLORREF _Color1;
  extern COLORREF _Color2;
  extern COLORREF _Color3;
  extern COLORREF _Color4;
  extern Themes   _theme;
  extern Themes   GetTheme();
  extern void     SetTheme(Themes pTheme);
  extern void     SetColors(COLORREF pColor1, COLORREF pColor2, COLORREF pColor3, COLORREF pColor4);
  static COLORREF NoWhite(COLORREF color) { return color == Assistant0 ? Assistant7 : color; }
  extern HBITMAP  GetScrollbarBitmap();
}

// Where we store the styles in the registry of MS-Windows
#define STYLECOLORS_KEY   "HKCU\\Software\\StyleFramework\\StyleThemes"
#define STYLECOLORS_THEME "Theme"

// Window
#define ClrwindowTransparent      RGB(0xFF, 0x00, 0xFF)
#define ClrWindowHeader           ThemeColor::_Color1 
#define ClrWindowHeaderText       ClrWindowHeader == Assistant0 ? Assistant10 : Assistant0
#define ClrWindowHeaderIcon       ThemeColor::_Color2 
#define ClrWindowHeaderIconActive ThemeColor::_Color3 
#define ClrWindowFrame            RGB(0x96, 0x96, 0x96) // 20% graying 
#define ClrWindowFrameError       Accent3               // Red for errors
#define ClrWindowFrameTextError   Assistant0
#define ClrWindowTopBarIconText   Assistant8

#define ClrWindowMessageError     ClrEditBkgnd
#define ClrWindowMessageTextError ClrLabelTextNormal

// CCalendarCtrl
#define ClrHighlightBorder        Accent3
#define ClrCalDayBkgHover         ClrWindowFrame
#define ClrCalDayTextHover        Assistant2
#define ClrCalDayBorder           Assistant5

// Regular background
#define ClrCanvas                 Assistant1
#define ClrFrameBkGnd             UsersBackground

// Frame
#define ClrFrameFrame             Assistant7
#define ClrFrameText              ThemeColor::_Color4

// Label
#define ClrLabelTextNormal        Assistant10
#define ClrLabelTextDisabled      Assistant7
#define ClrLabelBkGnd             Assistant2

// EditControl
#define ClrEditBkgnd              Assistant0
#define ClrEditDisabledBack       UsersBackground
#define ClrEditText               InputTextActive
#define ClrEditDisabledFore       InputTextInactive
#define ClrEditRequiredFore       Assistant8
#define ClrEditWatermarkFore      Assistant7
#define ClrEditHyperlink          Accent0 
#define ClrEditFrameNormal        Assistant6
#define ClrEditFrameError         Accent3
#define ClrEditFrameHover         Assistant8
#define ClrEditFrameVerplicht     ThemeColor::NoWhite(ThemeColor::_Color1)

// Buttoncontrol
#define ClrControlFrameNormal     Assistant7
#define ClrControlFrameHover      Assistant7
#define ClrControlFramePressed    Assistant7
#define ClrControlFrameDisabled   Assistant6
#define ClrControlNormal          ClrFrameBkGnd
#define ClrControlHover           Assistant5
#define ClrControlPressed         Assistant6
#define ClrControlDisabled        Assistant2
#define ClrControlTextNormal      ThemeColor::_Color3
#define ClrControlTextHover       Assistant9
#define ClrControlTextPressed     Assistant9
#define ClrControlTextDisabled    Assistant7
#define ClrControlDefault         ThemeColor::_Color1

// Checkbox
#define ClrCheckBoxDefaultBkGrnd  UsersBackground
#define ClrCheckboxFrameNormal    Assistant6
#define ClrCheckboxFrameFocus     Assistant8
#define ClrCheckboxFrameHover     ClrCheckboxFrameFocus
#define ClrCheckboxNormal         Assistant0
#define ClrCheckboxFramePressed   ClrCheckboxNormal
#define ClrCheckboxHover          ClrCheckboxNormal
#define ClrCheckboxFocus          ClrCheckboxNormal
#define ClrCheckboxPressed        ClrCheckboxNormal
#define ClrCheckboxTextNormal     ThemeColor::NoWhite(ThemeColor::_Color1)
#define ClrCheckboxTextHover      ThemeColor::NoWhite(ThemeColor::_Color1)
#define ClrCheckboxTextFocus      ThemeColor::NoWhite(ThemeColor::_Color1)
#define ClrCheckboxTextPressed    Assistant1

// Edit 
#define ClrControlDisabledFore    ClrEditText
#define ClrControlDisabledBack    ClrEditDisabledBack

// Scrollbar
#define ClrScrollbarNormal        ThemeColor::NoWhite(ThemeColor::_Color2)
#define ClrScrollbarHover         ThemeColor::NoWhite(ThemeColor::_Color1)
#define ClrScrollbarPressed       ThemeColor::_Color3

// TabControl
#define ClrTabFrame               Assistant5
#define ClrTabBkGndActive         ClrFrameBkGnd
#define ClrTabTextActive          TabTextActive
#define ClrTabBkGndHover          Assistant4
#define ClrTabTextHover           Assistant9
#define ClrTabBkGndSelected       ThemeColor::NoWhite(ThemeColor::_Color1)
#define ClrTabTextSelected        TabTextSelected
#define ClrTabBkGndInactive       ClrControlDisabledBack
#define ClrTabTextInactive        TabTextInactive

// TitleBar
#define ClrWndTitleBarText        Assistant9

// GridControl
#define ClrGridFrameNormal        Assistant5
#define ClrGridBkGndNormal        ClrFrameBkGnd
#define ClrGridTextHeader         ClrLabelTextNormal
#define ClrGridBkGndHeader        Assistant4
#define ClrGridCurSel             Assistant9
#define ClrGridBkGndLine          Assistant3
#define ClrGridCellText           CellTextActive
#define ClrGridMessageBkGnd       Assistant4
#define ClrGridMessageText        RGB(255,0,0)
#define ClrGridCelDisabledBack    Assistant1
#define ClrGridBkGndSelected      RGB(0xD2, 0xE1, 0xEC)
#define ClrGridSelectedText       RGB(0x00, 0x00, 0x00)
#define ClrGridCellCadre          Assistant6

