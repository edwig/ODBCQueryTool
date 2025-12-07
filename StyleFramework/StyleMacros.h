////////////////////////////////////////////////////////////////////////
//
// File: StyleMacros.h
// Function: Styling macros for font scaling and system metrics
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
#include <assert.h>
#include "StyleFonts.h"

// Macro to scale FONT and other sizes related to MS-Windows scaling
#ifndef WS
#define WS(h,val)              ((val * GetSFXSizeFactor(h)) / 100)
#endif

#define MARGIN(h)              ((  1 * GetSFXSizeFactor(h)) / 100)
#define PANELHEADERHEIGHT(h)   ((  0 * GetSFXSizeFactor(h)) / 100)
#define CAPTIONHEIGHT(h)       (( 46 * GetSFXSizeFactor(h)) / 100)
#define WINCAPTIONHEIGHT(h)    (( 30 * GetSFXSizeFactor(h)) / 100)
#define HALF_ICONSIZE(h)       (( 16 * GetSFXSizeFactor(h)) / 100)
#define SIZEMARGIN(h)          ((  4 * GetSFXSizeFactor(h)) / 100)
#define MENUITEMWIDTH_MIN(h)   (( 48 * GetSFXSizeFactor(h)) / 100)
#define MENUITEMWIDTH(h)       (( 60 * GetSFXSizeFactor(h)) / 100)
#define MENUITEMWIDTH_MAX(h)   ((120 * GetSFXSizeFactor(h)) / 100)
#define WINDOWSHADOWBORDER(h)  ((  2 * GetSFXSizeFactor(h)) / 100)
#define WINDOWCAPTIONHEIGHT(h) (( 30 * GetSFXSizeFactor(h)) / 100)

#define VK_LOGICAL_LBUTTON  GetSystemMetrics(SM_SWAPBUTTON) ? VK_RBUTTON : VK_LBUTTON
#define VK_LOGICAL_RBUTTON  GetSystemMetrics(SM_SWAPBUTTON) ? VK_LBUTTON : VK_RBUTTON

#define LBUTTONDOWN        (GetAsyncKeyState(VK_LOGICAL_LBUTTON) & KF_UP) != 0
#define RBUTTONDOWN        (GetAsyncKeyState(VK_LOGICAL_RBUTTON) & KF_UP) != 0

// Registered message for style changes
#define STYLEFXCHANGED     _T("STYLEFXCHANGED")