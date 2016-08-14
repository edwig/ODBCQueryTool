/* 
    Copyright (C) 2002 Aleksey Kochetov

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
*/

#include "stdafx.h"
#include <string>
#include <strstream>
#include <COMMON/ExceptionHelper.h>
#include "OpenEditor/OEGeneralPage.h"

    const char* COEGeneralPage::m_keymapLayoutList = "ConText;Custom;Default;EditPlus2;TextPad;UltraEdit;";

COEGeneralPage::COEGeneralPage (SettingsManager& manager)
	: CPropertyPage(COEGeneralPage::IDD)
    , m_manager(manager)
    /*, m_AllowMultipleInstances(FALSE)
    , m_NewDocOnStartup(FALSE)
    , m_MaximizeFirstDocument(FALSE)
    , m_WorkDirFollowsDoc(FALSE)
    , m_SaveCursPosAndBookmarks(FALSE)
    , m_SaveMainWindowPosition(FALSE)
    , m_keymapLayout(_T(""))
    , m_locale(_T(""))*/
{
    const OpenEditor::GlobalSettings& settings = m_manager.GetGlobalSettings();
    m_AllowMultipleInstances  = settings.GetAllowMultipleInstances() ? TRUE : FALSE;
    m_NewDocOnStartup         = settings.GetNewDocOnStartup()        ? TRUE : FALSE;
    m_MaximizeFirstDocument   = settings.GetMaximizeFirstDocument()  ? TRUE : FALSE;
    m_WorkDirFollowsDoc       = settings.GetWorkDirFollowsDocument() ? TRUE : FALSE;
    m_SaveCursPosAndBookmarks = settings.GetSaveCurPosAndBookmarks() ? TRUE : FALSE;
    m_SaveMainWindowPosition  = settings.GetSaveMainWinPosition()    ? TRUE : FALSE;
    m_keymapLayout            = settings.GetKeymapLayout().c_str();
    m_locale                  = settings.GetLocale().c_str();
}

COEGeneralPage::~COEGeneralPage()
{
}

void COEGeneralPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Check(pDX, IDC_OE_GEN_MULTIPLE_INST, m_AllowMultipleInstances);
    DDX_Check(pDX, IDC_OE_GEN_NEW_DOC_ON_STARTUP, m_NewDocOnStartup);
    DDX_Check(pDX, IDC_OE_GEN_MAXIMIZE_FIRST_DOC, m_MaximizeFirstDocument);
    DDX_Check(pDX, IDC_OE_GEN_WORK_DIR_FOLLOWS_DOC, m_WorkDirFollowsDoc);
    DDX_Check(pDX, IDC_OE_GEN_SAVE_CURS_N_BOOKMARKS, m_SaveCursPosAndBookmarks);
    DDX_Check(pDX, IDC_OE_GEN_SAVE_MAIN_WIN_POS, m_SaveMainWindowPosition);
    
    if (!pDX->m_bSaveAndValidate)
    {
        HWND hList = ::GetDlgItem(m_hWnd, IDC_OE_GEN_KEYMAP_LAYOUT);

        if (!::SendMessage(hList, CB_GETCOUNT, 0, 0))
        {
            std::istrstream in(m_keymapLayoutList);
            std::string line;
            while (std::getline(in, line, ';'))
                ::SendMessage(hList, CB_ADDSTRING, 0, (LPARAM)line.c_str());
        }
    }
    
    DDX_CBString(pDX, IDC_OE_GEN_KEYMAP_LAYOUT, m_keymapLayout);
    DDX_CBString(pDX, IDC_OE_GEN_LOCALE, m_locale);
}


//BEGIN_MESSAGE_MAP(COEGeneralPage, CPropertyPage)
//END_MESSAGE_MAP()


// COEGeneralPage message handlers

BOOL COEGeneralPage::OnApply()
{
    try
    {
        OpenEditor::GlobalSettings& settings = m_manager.GetGlobalSettings();
        settings.SetAllowMultipleInstances(m_AllowMultipleInstances  ? true : false,  false /*notify*/);
        settings.SetNewDocOnStartup(m_NewDocOnStartup                ? true : false,  false /*notify*/);
        settings.SetMaximizeFirstDocument(m_MaximizeFirstDocument    ? true : false,  false /*notify*/);
        settings.SetWorkDirFollowsDocument(m_WorkDirFollowsDoc       ? true : false,  false /*notify*/);
        settings.SetSaveCurPosAndBookmarks(m_SaveCursPosAndBookmarks ? true : false,  false /*notify*/);
        settings.SetSaveMainWinPosition(m_SaveMainWindowPosition     ? true : false,  false /*notify*/);
        settings.SetKeymapLayout((LPCSTR)m_keymapLayout);
        settings.SetLocale((LPCSTR)m_locale);
    }
    _OE_DEFAULT_HANDLER_;

	return TRUE;
}
