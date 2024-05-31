////////////////////////////////////////////////////////////////////////
//
// File: StyleColors.cpp
// Function: Color definitions in the styleguide colors
//
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
#include "resource.h"
#include "StyleColors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace ThemeColor;

// Message that styles have changed
UINT ThemeColor::g_msg_changed = 0;

// Put default theme to Loyalty
Themes   ThemeColor::_theme   = ThemeColor::Themes::ThemeSkyblue;

const int ThemeColor::theme_colors[NO_OF_COLORS][NO_OF_THEMES] =
{
 /* LIME            SKYBLUE         PURPLE          MUSTARD            MODERATE-GRAY      PURE-GRAY       BLACK-WHITE,     DARK-THEME  */
  { Lime1,          Skyblue1,       Purple1,        Mustard1,          ModerateGray1,     PureGray1,      BlackWhite1,     Dark1               }  // AccentColor1
 ,{ Lime2,          Skyblue2,       Purple2,        Mustard2,          ModerateGray2,     PureGray2,      BlackWhite2,     Dark2               }  // AccentColor2
 ,{ Lime3,          Skyblue3,       Purple3,        Mustard3,          ModerateGray3,     PureGray3,      BlackWhite3,     Dark3               }  // AccentColor3
 ,{ LimeHeader,     SkyblueHeader,  PurpleHeader,   MustardHeader,     ModerateGrayHeader,PureGrayHeader, BlackWhiteHeader,DarkHeader          }  // AccentColor4
 ,{ UsersBackground,UsersBackground,UsersBackground,UsersBackground,   UsersBackground,   UsersBackground,UsersBackground, DarkBackground      }  // ColorWindowFrame
 ,{ Assistant0,     Assistant0,     Assistant0,     Assistant0,        Assistant0,        Assistant0,     Assistant0,      DarkCtrlBackground  }  // ColorCtrlBackground
 ,{ InputTextActive,InputTextActive,InputTextActive,InputTextActive,   InputTextActive,   InputTextActive,InputTextActive, ClrEditDark         }  // ColorEditText
 ,{ Assistant0,     Assistant0,     Assistant0,     Assistant0,        Assistant0,        Assistant0,     Assistant0,      Assistant10         }  // ColorEditBkgnd
 ,{ ClrEditText,    ClrEditText,    ClrEditText,    ClrEditText,       ClrEditText,       ClrEditText,    ClrEditText,     ClrEditDarkHover    }  // ColorEditHover
 ,{ ClrEditText,    ClrEditText,    ClrEditText,    ClrEditText,       ClrEditText,       ClrEditText,    ClrEditText,     ClrEditDark         }  // ColorLabelText
 ,{ ComboBoxActive, ComboBoxActive, ComboBoxActive, ComboBoxActive,    ComboBoxActive,    ComboBoxActive, ComboBoxActive,  ComboBoxDActive     }  // ColorComboActive
 ,{ ComboBoxDropped,ComboBoxDropped,ComboBoxDropped,ComboBoxDropped,   ComboBoxDropped,   ComboBoxDropped,ComboBoxDropped, ComboBoxDDropped    }  // ColorComboDropped
 ,{ GroupBoxLine,   GroupBoxLine,   GroupBoxLine,   GroupBoxLine,      GroupBoxLine,      GroupBoxLine,   GroupBoxLine,    GroupBoxLineDark    }  // ColorGroupLine
 ,{ Lime1,          Skyblue1,       Purple1,        Mustard1,          ModerateGray1,     PureGray1,      BlackWhite1,     GroupBoxLine        }  // ColorButtonCheck
 ,{ UsersBackground,UsersBackground,UsersBackground,UsersBackground,   UsersBackground,   UsersBackground,UsersBackground, DarkCtrlBackground  }  // ColorButtonBackground
 ,{ Lime3,          Skyblue3,       Purple3,        Mustard3,          ModerateGray3,     PureGray3,      BlackWhite3,     DarkButtonText      }  // ColorButtonText
 ,{ Assistant5,     Assistant5,     Assistant5,     Assistant5,        Assistant5,        Assistant5,     Assistant5,      Assistant5          }  // ColorControlHover
 ,{ Assistant6,     Assistant6,     Assistant6,     Assistant6,        Assistant6,        Assistant6,     Assistant6,      Assistant6          }  // ColorControlPressed
 ,{ Assistant7,     Assistant7,     Assistant7,     Assistant7,        Assistant7,        Assistant7,     Assistant7,      Assistant7          }  // ColorControlFramePressed
 ,{ Assistant8,     Assistant8,     Assistant8,     Assistant8,        Assistant8,        Assistant8,     Assistant8,      Assistant8          }  // ColorControlFrameHover
 ,{ Assistant9,     Assistant9,     Assistant9,     Assistant9,        Assistant9,        Assistant9,     Assistant9,      Assistant9          }  // ColorControlTextPressed
 ,{ Assistant9,     Assistant9,     Assistant9,     Assistant9,        Assistant9,        Assistant9,     Assistant9,      Assistant9          }  // ColorControlTextHover

 ,{ Assistant6,     Assistant6,     Assistant6,     Assistant6,        Assistant6,        Assistant6,     Assistant6,      Assistant6          }  // ColorControlFrameDisabled
 ,{ Assistant2,     Assistant2,     Assistant2,     Assistant2,        Assistant2,        Assistant2,     Assistant2,      Assistant2          }  // ColorControlDisabled
 ,{ Assistant7,     Assistant7,     Assistant7,     Assistant7,        Assistant7,        Assistant7,     Assistant7,      Assistant7          }  // ColorControlTextDisabled

 ,{ TabTextActive,  TabTextActive,  TabTextActive,  TabTextActive,     TabTextActive,     TabTextActive,  TabTextActive,   TabTextActive       }  // ColorTabTextActive
 ,{ TabTextInactive,TabTextInactive,TabTextInactive,TabTextInactive,   TabTextInactive,   TabTextInactive,TabTextInactive, TabTextInactive     }  // ColorTabTextInactive
 ,{ Assistant5,     Assistant5,     Assistant5,     Assistant5,        Assistant5,        Assistant5,     Assistant5,      Assistant5          }  // ColorTabFrame
 ,{ UsersBackground,UsersBackground,UsersBackground,UsersBackground,   UsersBackground,   UsersBackground,UsersBackground, DarkBackground      }  // ColorTabBkGndActive
 ,{ UsersBackground,UsersBackground,UsersBackground,UsersBackground,   UsersBackground,   UsersBackground,UsersBackground, DarkBackground      }  // ColorTabBkGndInactive
 ,{ TabTextSelected,TabTextSelected,TabTextSelected,TabTextSelected,   TabTextSelected,   TabTextSelected,TabTextSelected, TabTextSelected     }  // ColorTabTextSelected
 ,{ Assistant4,     Assistant4,     Assistant4,     Assistant4,        Assistant4,        Assistant4,     Assistant4,      Assistant4          }  // ColorTabBkGndHover
 ,{ Assistant9,     Assistant9,     Assistant9,     Assistant9,        Assistant9,        Assistant9,     Assistant9,      Assistant9          }  // ColorTabTextHover
};

int ThemeColor::GetColor(Colors p_color)
{
  return ThemeColor::theme_colors[(int)p_color][(int)ThemeColor::_theme];
}

LPCTSTR ThemeColor::theme_names[NO_OF_THEMES] =
{
   _T("Lime")
  ,_T("Skyblue")
  ,_T("Purple")
  ,_T("Mustard")
  ,_T("Moderate gray")
  ,_T("Pure gray")
  ,_T("Black and White")
  ,_T("Dark theme")
};

void ThemeColor::SetTheme(Themes pTheme)
{
  _theme = pTheme;
}

Themes ThemeColor::GetTheme()
{
  return _theme;
}

static HBITMAP g_scrollbars[NO_OF_THEMES] = { NULL };

HBITMAP  
ThemeColor::GetScrollbarBitmap()
{
  if(g_scrollbars[0] == nullptr)
  {
    for (int index = 0; index < NO_OF_THEMES; ++index)
    {
      CBitmap bmp;
      bmp.LoadBitmap(IDB_SCROLLBAR1 + index);
      BITMAP bm;
      bmp.GetBitmap(&bm);
      g_scrollbars[index] = (HBITMAP)bmp.Detach();
    }
  }
  return g_scrollbars[(int)ThemeColor::_theme];
}

COLORREF
ThemeColor::HalfTone(COLORREF color,double p_factor /*=0.5*/)
{
  int red   =  color >> 16;
  int green = (color & 0xFF00) >> 8;
  int blue  = (color & 0x00FF);

  return RGB(red * p_factor,green * p_factor,blue * p_factor);
}

COLORREF 
ThemeColor::NoWhite(COLORREF color) 
{ 
  if(_theme == Themes::ThemeDark)
  {
    return RGB(0xBF,0xBF,0xBF);
  }
  return color == Assistant0 ? Assistant7 : color; 
}
