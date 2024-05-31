////////////////////////////////////////////////////////////////////////
//
// File: AutoFocus.h
// Function: Draw gray edge around a screen
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

// Auto object to be put on the stack
// e.q. while running a modal dialog
// Use: Restore the keyboard focus after closing the dialog

class AutoFocus
{
public:
  AutoFocus()
  {
    m_hwnd = GetFocus();
  }

  ~AutoFocus()
  {
    if(m_hwnd)
    {
      SetFocus(m_hwnd);
    }
  }
private:
  HWND m_hwnd;
};