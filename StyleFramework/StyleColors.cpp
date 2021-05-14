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
COLORREF ThemeColor::_Color1 = Skyblue1;
COLORREF ThemeColor::_Color2 = Skyblue2;
COLORREF ThemeColor::_Color3 = Skyblue3;
COLORREF ThemeColor::_Color4 = SkyblueHeader;
Themes   ThemeColor::_theme  = ThemeColor::Themes::ThemeSkyblue;

char* ThemeColor::theme_names[NO_OF_THEMES] =
{
   "Lime"
  ,"Skyblue"
  ,"Purple"
  ,"Moderate gray"
  ,"Pure gray"
  ,"Black and White"
};

void ThemeColor::SetTheme(Themes pTheme)
{
  switch (pTheme)
  {
    case ThemeLime:         ThemeColor::SetColors(Lime1,        Lime2,        Lime3,        LimeHeader);        break;
    case ThemeSkyblue:      ThemeColor::SetColors(Skyblue1,     Skyblue2,     Skyblue3,     SkyblueHeader);     break;
    case ThemePurple:       ThemeColor::SetColors(Purple1,      Purple2,      Purple3,      PurpleHeader);      break;
    case ThemeModerateGray: ThemeColor::SetColors(ModerateGray1,ModerateGray2,ModerateGray3,ModerateGrayHeader);break;
    case ThemePureGray:     ThemeColor::SetColors(PureGray1,    PureGray2,    PureGray3,    PureGrayHeader);    break;
    case ThemeBlackWhite:   ThemeColor::SetColors(BlackWhite1,  BlackWhite2,  BlackWhite3,  BlackWhiteHeader);  break;
    default:                return;
  }
  _theme = pTheme;
}


void ThemeColor::SetColors(COLORREF pColor1, COLORREF pColor2, COLORREF pColor3, COLORREF pColor4)
{
  _Color1 = pColor1;
  _Color2 = pColor2;
  _Color3 = pColor3;
  _Color4 = pColor4;
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
  return g_scrollbars[_theme];
}
