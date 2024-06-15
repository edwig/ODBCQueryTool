////////////////////////////////////////////////////////////////////////
//
// File: StyleTexts.h
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

enum class StyleLanguage
{
  SL_DEFAULT
 ,SL_ENGLISH
 ,SL_DUTCH
 ,SL_GERMAN
 ,SL_FRENCH
 ,SL_LAST
};

// Sets the currently used (default) language for the style framework
void  SetStyleFxLanguage(StyleLanguage p_language = StyleLanguage::SL_ENGLISH);
// And retrieving the currently set language
StyleLanguage GetStyleFxLanguage();

// Adding and retrieving a text
bool    AddStyleText(int p_id,CString p_text,StyleLanguage p_language = StyleLanguage::SL_DEFAULT);
CString GetStyleText(int p_id,               StyleLanguage p_language = StyleLanguage::SL_DEFAULT);


//////////////////////////////////////////////////////////////////////////
//
// MACRO'S FOR TEXTS
//
//////////////////////////////////////////////////////////////////////////

#define TXT_OK                1
#define TXT_OK_CANCEL         2
#define TXT_ABORT_RETRY       3
#define TXT_YES_NO            4
#define TXT_YES_NO_CANCEL     5
#define TXT_RETRY_CANCEL      6
#define TXT_NO_REPEAT         7
#define TXT_S_OK_CANCEL       8
#define TXT_S_ABORT_RETRY     9
#define TXT_S_YES_NO         10
#define TXT_S_YES_NO_CANCEL  11
#define TXT_S_RETRY_CANCEL   12
#define TXT_ERROR            13
#define TXT_VALUE_BETWEEN    14
#define TXT_PRO_LIST         15
#define TXT_SEL_LIST         16
#define TXT_NUM_ONLY         17
#define TXT_MANDATORY_FLD    18
#define TXT_NO_BITMAP        19
#define TXT_FILE_NOT_OPEN    20
#define TXT_FILE_NOT_CLOSED  21
#define TXT_CALENDAR_TITLE   22
#define TXT_NO_SPACE         23
#define TXT_X_ARGUMENTS      24
#define TXT_U_ARGUMENTS      25
#define TXT_NO_BMP_FILE      26
#define TXT_END_LISTING      27
#define TXT_PRINT_PAGE       28
#define TXT_PRINT_DATE       29
#define TXT_BOOLEAN_TRUE     30
#define TXT_BOOLEAN_FALSE    31
#define TXT_WIZARD           32
#define TXT_CANCEL_WIZARD    33