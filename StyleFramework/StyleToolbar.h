////////////////////////////////////////////////////////////////////////
//
// File: StyleToolBar.h
// Function: Styled progress control
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

// Override on CMFCToolBar. Needed for the LButtonUp on a toolbar button
// Will otherwise 'eat' our Exception handling stack and frames 
// Leaving the application to crash, instead of falling back on the SEH Exception handling

class StyleToolBar : public CMFCToolBar
{
  DECLARE_SERIAL(StyleToolBar)

public:

protected:
  afx_msg void OnLButtonUp(UINT nFlags,CPoint point);

  DECLARE_MESSAGE_MAP()
};
