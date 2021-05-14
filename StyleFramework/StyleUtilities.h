////////////////////////////////////////////////////////////////////////
//
// File: StyleUtilities.h
// Function: String utilities for the StyleMessageBox
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
#include <vector>

CString EverythingBefore(CString p_string,CString p_tag);
CString EverythingAfter (CString p_string,CString p_tag);

int     FindNoCase(CString p_string,CString p_substring);
bool    Contains  (CString p_string,CString p_tag);
CString IntegerToString(int p_number);
CString NumToString(int p_number);
void    Split(CString p_string,CString p_delimiter,std::vector<CString>& p_array);

void    StyleGetWorkArea  (CWnd* p_wnd,CRect& p_rect,int x = 0,int y = 0);
void    RepositionFrameWnd(CWnd* p_wnd,bool p_isFrame = true);
void    MinimalFrameWnd   (CWnd* p_wnd);

