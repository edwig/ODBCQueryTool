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

#include "pch.h"
#include "OEGeneralPage.h"
#include <COMMON/ExceptionHelper.h>
#include <StringUtilities.h>

LPCTSTR COEGeneralPage::m_keymapLayoutList = _T("ConText;Custom;Default;EditPlus2;TextPad;UltraEdit;");

COEGeneralPage::COEGeneralPage (SettingsManager& manager,CWnd* p_parent)
               :StyleDialog(COEGeneralPage::IDD,p_parent)
               ,m_manager(manager)
{
    const OpenEditor::GlobalSettings& settings = m_manager.GetGlobalSettings();
    m_AllowMultipleInstances  = settings.GetAllowMultipleInstances() ? TRUE : FALSE;
    m_NewDocOnStartup         = settings.GetNewDocOnStartup()        ? TRUE : FALSE;
    m_MaximizeFirstDocument   = settings.GetMaximizeFirstDocument()  ? TRUE : FALSE;
    m_WorkDirFollowsDoc       = settings.GetWorkDirFollowsDocument() ? TRUE : FALSE;
    m_SaveCursPosAndBookmarks = settings.GetSaveCurPosAndBookmarks() ? TRUE : FALSE;
    m_SaveMainWindowPosition  = settings.GetSaveMainWinPosition()    ? TRUE : FALSE;
    m_keymapLayout            = settings.GetKeymapLayout();
    m_locale                  = settings.GetLocale();
}

COEGeneralPage::~COEGeneralPage()
{
}

void COEGeneralPage::DoDataExchange(CDataExchange* pDX)
{
    StyleDialog::DoDataExchange(pDX);
    DDX_Control(pDX,IDC_OE_GEN_MULTIPLE_INST,        m_checkAllowMultiple);
    DDX_Control(pDX,IDC_OE_GEN_NEW_DOC_ON_STARTUP,   m_checkNewDocOnStartup);
    DDX_Control(pDX,IDC_OE_GEN_MAXIMIZE_FIRST_DOC,   m_checkMaximizeFirstDoc);
    DDX_Control(pDX,IDC_OE_GEN_WORK_DIR_FOLLOWS_DOC, m_checkWorkDirFollowsDoc);
    DDX_Control(pDX,IDC_OE_GEN_SAVE_CURS_N_BOOKMARKS,m_checkSaveCursPosBookmarks);
    DDX_Control(pDX,IDC_OE_GEN_SAVE_MAIN_WIN_POS,    m_checkSaveMainWindowPos);

    DDX_Check(pDX, IDC_OE_GEN_MULTIPLE_INST,        m_AllowMultipleInstances);
    DDX_Check(pDX, IDC_OE_GEN_NEW_DOC_ON_STARTUP,   m_NewDocOnStartup);
    DDX_Check(pDX, IDC_OE_GEN_MAXIMIZE_FIRST_DOC,   m_MaximizeFirstDocument);
    DDX_Check(pDX, IDC_OE_GEN_WORK_DIR_FOLLOWS_DOC, m_WorkDirFollowsDoc);
    DDX_Check(pDX, IDC_OE_GEN_SAVE_CURS_N_BOOKMARKS,m_SaveCursPosAndBookmarks);
    DDX_Check(pDX, IDC_OE_GEN_SAVE_MAIN_WIN_POS,    m_SaveMainWindowPosition);
    
    DDX_CBString(pDX, IDC_OE_GEN_KEYMAP_LAYOUT,     m_comboKeymap,m_keymapLayout);
    DDX_CBString(pDX, IDC_OE_GEN_LOCALE,            m_comboLocale,m_locale);

    if (!pDX->m_bSaveAndValidate)
    {
      if(m_comboKeymap.GetCount() == 0)
      {
        CString in(m_keymapLayoutList);
        std::vector<CString> list;
        SplitString(in,list,';');

        for(auto& str : list)
        {
          m_comboKeymap.AddString(str);
        }
      }
    }
}


//BEGIN_MESSAGE_MAP(COEGeneralPage, CPropertyPage)
//END_MESSAGE_MAP()

// COEGeneralPage message handlers

BOOL 
COEGeneralPage::OnApply()
{
  try
  {
    OpenEditor::GlobalSettings& settings = m_manager.GetGlobalSettings();
    settings.SetAllowMultipleInstances(m_AllowMultipleInstances  ? true : false,  false /*notify*/);
    settings.SetNewDocOnStartup       (m_NewDocOnStartup         ? true : false,  false /*notify*/);
    settings.SetMaximizeFirstDocument (m_MaximizeFirstDocument   ? true : false,  false /*notify*/);
    settings.SetWorkDirFollowsDocument(m_WorkDirFollowsDoc       ? true : false,  false /*notify*/);
    settings.SetSaveCurPosAndBookmarks(m_SaveCursPosAndBookmarks ? true : false,  false /*notify*/);
    settings.SetSaveMainWinPosition   (m_SaveMainWindowPosition  ? true : false,  false /*notify*/);
    settings.SetKeymapLayout          ((LPCTSTR)m_keymapLayout);
    settings.SetLocale((LPCTSTR)m_locale);
  }
  _OE_DEFAULT_HANDLER_;

	return TRUE;
}
