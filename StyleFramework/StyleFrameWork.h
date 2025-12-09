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

#define SFX_VERSION "1.2.1"       // Current SFX version number

// General headers
#include "AutoFocus.h"
#include "StyleColors.h"
#include "StyleFonts.h"
#include "StyleMacros.h"
#include "StyleTexts.h"
#include "StyleMonitor.h"

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
#include "StyleStepper.h"         // ---              -> StyleStepper

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
#include "StyleRichEdit.h"        // CRichEditCtrl    -> StyleRichEdit
#include "StyleStatic.h"          // CStatic          -> StyleStatic
#include "StyleStaticToast.h"     // ----             -> For StyleToast
#include "StyleSpinButtonCtrl.h"  // CSpinButtonCtrl  -> StyleSpinButtonCtrl
#include "StyleTabCtrl.h"         // CTabCtrl         -> StyleTabCtrl
#include "StyleTab.h"             // ----             -> StyleTab (simple StyleDialog)
#include "StyleToolBar.h"         // CMFCToolBar      -> StyleToolBar
#include "StyleTreeCtrl.h"        // CTreeCtrl        -> StyleTreeCtrl

//////////////////////////////////////////////////////////////////////////
//
// The one-and-only global style framework object
// Used to get font and monitor information and scaling factors
//
class StylingFramework
{
public:
  StylingFramework();

  // GETTERS
  const StyleMonitor* GetMonitor(HWND p_hwnd) const;
  const StyleMonitor* GetMonitor(HMONITOR p_monitor) const;
  const StyleMonitor* GetMonitor(int p_dpi_x,int p_dpi_y) const;
  const StyleMonitor* GetMonitor(CString p_name) const;
  const StyleMonitor* GetPrimaryMonitor() const;

  void  RefreshMonitors();

private:
  StylingFramework* m_instance { nullptr };
  StyleMonitors     m_monitors;
};

// General resize a windows rectangle
int    GetSFXSizeFactor(HWND p_hwnd);
int    GetSFXSizeFactor(HMONITOR p_monitor);
CFont* GetSFXFont(HWND p_hwnd,StyleFontType p_type);
CFont* GetSFXFont(HMONITOR hm,StyleFontType p_type);

// The one-and-only global
extern StylingFramework g_styling;

// Selecting the right library to link with automatically
// So we do not need to worry about which library to use in the linker settings

#ifdef UNICODE
#if defined _M_IX86
#define SFX_PLATFORM "Ux86"
#else
#define SFX_PLATFORM "Ux64"
#endif
#else
#if defined _M_IX86
#define SFX_PLATFORM "x86"
#else
#define SFX_PLATFORM "x64"
#endif
#endif // UNICODE

#if defined _DEBUG
#define SFX_CONFIGURATION "D"
#else
#define SFX_CONFIGURATION "R"
#endif 

#ifndef STYLEFRAMEWORK_NOAUTOLINK
#pragma comment(lib,"StyleFramework_"                        SFX_PLATFORM SFX_CONFIGURATION ".lib")
#pragma message("Automatically linking with StyleFramework_" SFX_PLATFORM SFX_CONFIGURATION ".lib")
#else
#pragma message("Now building library 'StyleFramework_" SFX_PLATFORM SFX_CONFIGURATION ".lib'")
#endif 
