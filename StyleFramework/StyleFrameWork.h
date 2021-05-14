////////////////////////////////////////////////////////////////////////
//
// File: StyleFrameWork.h
// Function: General header of the StylingFramework
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

#define SFX_VERSION "1.0.1"       // Current SFX version number

// General headers
#include "StyleColors.h"
#include "StyleFonts.h"
#include "StyleMacros.h"
#include "StyleTexts.h"

// Skinning & scrollbars
#include "SkinScrollWnd.h"

// Dialogs & views
#include "StyleDialog.h"          // CDialog          -> StyleDialog
#include "StyleFormView.h"        // CFormView        -> StyleFormView
#include "StyleFrameWnd.h"        // CFrameWnd        -> StyleFrameWnd
#include "StyleFrameWndEx.h"      // CFrameWndEx      -> StyleFrameWndEx
#include "StyleMessageBox.h"      // MessageBox       -> StyleMessageBox
#include "StyleMDIFrameWnd.h"     // CMDIFrameWndEx   -> StyleMDIFrameWnd
#include "StyleToast.h"           // ---              -> StyleToast

// Controls
#include "StyleButton.h"          // CButton          -> StyleButton
#include "StyleCalendar.h"        // CCalendarCtrl    -> StyleCalendar
#include "StyleCheckbox.h"        // CButton          -> StyleCheckbox / StyleRadioButton
#include "StyleComboBox.h"        // CComboBox        -> StyleComboBox
#include "StyleEdit.h"            // CEdit            -> StyleEdit
#include "StyleGridCtrl.h"        // CGridCtrl        -> StyleGridCtrl
#include "StyleHyperlink.h"       // CLinkCtrl        -> StyleHyperlink
#include "StyleListBox.h"         // CListBox         -> StyleListBox
#include "StyleListCtrl.h"        // CListCtrl        -> StyleListCtrl
#include "StyleProgressCtrl.h"    // CProgressCtrl    -> StyleProgressCtrl
#include "StyleStatic.h"          // CStatic          -> StyleStatic
#include "StyleSpinButtonCtrl.h"  // CSpinButtonCtrl  -> StyleSpinButtonCtrl
#include "StyleTabCtrl.h"         // CTabCtrl         -> StyleTabCtrl
#include "StyleTreeCtrl.h"        // CTreeCtrl        -> StyleTreeCtrl

// Selecting the right library to link with automatically
// So we do not need to worry about which library to use in the linker settings
#if defined _M_IX86
#define SFX_PLATFORM "x86"
#else
#define SFX_PLATFORM "x64"
#endif

#if defined _DEBUG
#define SFX_CONFIGURATION "D"
#else
#define SFX_CONFIGURATION "R"
#endif 

#ifndef SQL_COMPONENTS_NOAUTOLINK
#pragma comment(lib,"StyleFramework_"                        SFX_PLATFORM SFX_CONFIGURATION ".lib")
#pragma message("Automatically linking with StyleFramework_" SFX_PLATFORM SFX_CONFIGURATION ".lib")
#endif 
