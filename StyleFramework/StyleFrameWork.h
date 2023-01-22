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

class StylingFramework
{
public:
  StylingFramework();

  // Set sizing in percentage (96 DPI = 100 %)
  bool SetSizeFactorX(int p_factor);
  bool SetSizeFactorY(int p_factor);
  // Getting the sizing factor
  int  GetSizeFactorX();
  int  GetSizeFactorY();

private:
  // Pre-calculate the DPI scaling factor
  void SFXCalculateDPI();

  StylingFramework* m_instance { nullptr };
  int m_factor_x { 100 };
  int m_factor_y { 100 };
};


// Setting the size factor
// To be called in your main program or InitInstance **BEFORE** you create any dialog or window
bool SetSFXSizeFactor(int p_factorX,int p_factorY);
int  GetSFXSizeFactor();

// General resize a windows rectangle
void SFXResizeByFactor(CRect& p_rect);
void SFXResizeByFactor(int& p_x,int& p_y,int& p_w,int& p_h);

// General headers
#include "StyleColors.h"
#include "StyleFonts.h"
#include "StyleMacros.h"
#include "StyleTexts.h"

// Skinning & scrollbars
#include "SkinScrollWnd.h"

// Header                         MFC Original        Replacement functionality 
// ------------------------------ ------------------- -------------------------

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
#include "StyleGroupBox.h"        // CButton          -> StyleGroupBox
#include "StyleHyperlink.h"       // CLinkCtrl        -> StyleHyperlink
#include "StyleListBox.h"         // CListBox         -> StyleListBox
#include "StyleListCtrl.h"        // CListCtrl        -> StyleListCtrl
#include "StyleProgressCtrl.h"    // CProgressCtrl    -> StyleProgressCtrl
#include "StyleStatic.h"          // CStatic          -> StyleStatic
#include "StyleStaticToast.h"     // ----             -> For StyleToast
#include "StyleSpinButtonCtrl.h"  // CSpinButtonCtrl  -> StyleSpinButtonCtrl
#include "StyleTabCtrl.h"         // CTabCtrl         -> StyleTabCtrl
#include "StyleTab.h"             // ----             -> StyleTab (simple StyleDialog)
#include "StyleToolBar.h"         // CMFCToolBar      -> StyleToolBar
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

#ifndef STYLEFRAMEWORK_NOAUTOLINK
#pragma comment(lib,"StyleFramework_"                        SFX_PLATFORM SFX_CONFIGURATION ".lib")
#pragma message("Automatically linking with StyleFramework_" SFX_PLATFORM SFX_CONFIGURATION ".lib")
#endif 
