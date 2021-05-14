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
#include "resource.h"
#include "COMMON/StrHelpers.h"
#include <COMMON/ExceptionHelper.h>
#include "OEClassPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COEClassPage property page

COEClassPage::COEClassPage(SettingsManager& manager,CWnd* p_parent) 
             :StyleDialog(COEClassPage::IDD,p_parent)
             ,m_manager(manager)
             ,m_dataInitialized(false)
{
}

COEClassPage::~COEClassPage()
{
}

// Check if resources still OK!
#if (!(IDC_OEC_INDENT_TYPE_NONE < IDC_OEC_INDENT_TYPE_DEFAULT && IDC_OEC_INDENT_TYPE_DEFAULT < IDC_OEC_INDENT_TYPE_SMART))
#error("check resource indentifiers defenition: IDC_OEC_INDENT_TYPE_NONE, IDC_OEC_INDENT_TYPE_DEFAULT, IDC_OEC_INDENT_TYPE_SMART")
#endif
#if (!(IDC_OEC_INSERT_SPACES < IDC_OEC_KEEP_TABS))
#error("check resource indentifiers defenition: IDC_OEC_INSERT_SPACES, IDC_OEC_KEEP_TABS")
#endif

void 
COEClassPage::DoDataExchange(CDataExchange* pDX)
{
  StyleDialog::DoDataExchange(pDX);

  DDX_CBString(pDX, IDC_OEC_CLASS,              m_comboClass,         m_class);
  DDX_Control (pDX, IDC_OEC_FILE_EXTENSIONS,    m_editFileExtensions, m_fileExtensions);
  DDX_CBString(pDX, IDC_OEC_FILE_TYPE,          m_comboFileType,      m_fileType);
  DDX_Control (pDX, IDC_OEC_DELIMITERS,         m_editDelimiters,     m_delimiters);
  DDX_Control (pDX, IDC_OEC_TAB_SIZE,           m_editTabSize,        m_tabSize);
  DDX_Control (pDX, IDC_OEC_INDENT_SIZE,        m_editIndentSize,     m_indentSize);
  DDX_Control (pDX, IDC_OEC_TAB_SIZE_SPIN,      m_spinTabSize,       &m_editTabSize);
  DDX_Control (pDX, IDC_OEC_INDENT_SIZE_SPIN,   m_spinIndentSize,    &m_editIndentSize);
  DDX_Control (pDX, IDC_OEC_INSERT_SPACES,      m_checkInsertSpaces);
  DDX_Control (pDX, IDC_OEC_KEEP_TABS,          m_checkKeepTabs);
  DDX_Control (pDX, IDC_OEC_INDENT_TYPE_NONE,   m_checkIndentNone);
  DDX_Control (pDX, IDC_OEC_INDENT_TYPE_DEFAULT,m_checkIndentDefault);
  DDX_Control (pDX, IDC_OEC_INDENT_TYPE_SMART,  m_checkIndentSmart);
  DDX_Control (pDX, IDC_OEC_LINE_NUMBERS,       m_checkLineNumbers);
  DDX_Control (pDX, IDC_OEC_VISIBLE_SPACES,     m_checkVisibleSpaces);
  DDX_Control (pDX, IDC_OEC_NORMALIZE_KEYWORDS, m_checkNormalizeKW);

  if (pDX->m_bSaveAndValidate)
  {
    m_insertSpaces = IsDlgButtonChecked(IDC_OEC_INSERT_SPACES);

         if (IsDlgButtonChecked(IDC_OEC_INDENT_TYPE_SMART))   m_indent = 2;
    else if (IsDlgButtonChecked(IDC_OEC_INDENT_TYPE_DEFAULT)) m_indent = 1;
    else m_indent = 0;
  }
  else
  {
    CheckRadioButton(IDC_OEC_INSERT_SPACES, 
                     IDC_OEC_INSERT_SPACES + 1, 
                     IDC_OEC_INSERT_SPACES + (m_insertSpaces ? 0 : 1));
    CheckRadioButton(IDC_OEC_INDENT_TYPE_NONE, 
                     IDC_OEC_INDENT_TYPE_NONE + 2, 
                     IDC_OEC_INDENT_TYPE_NONE + m_indent);
  }
}


BEGIN_MESSAGE_MAP(COEClassPage, StyleDialog)
  ON_CBN_SELCHANGE(IDC_OEC_CLASS,           OnSelChangeLanguage)
  ON_EN_CHANGE(IDC_OEC_FILE_EXTENSIONS,     OnChangeData)
  ON_CBN_SELCHANGE(IDC_OEC_FILE_TYPE,       OnChangeData)
  ON_EN_CHANGE(IDC_OEC_DELIMITERS,          OnChangeData)
  ON_EN_CHANGE(IDC_OEC_TAB_SIZE,            OnChangeData)
  ON_EN_CHANGE(IDC_OEC_INDENT_SIZE,         OnChangeData)
  ON_BN_CLICKED(IDC_OEC_INSERT_SPACES,      OnChangeData)
  ON_BN_CLICKED(IDC_OEC_KEEP_TABS,          OnChangeData)
  ON_BN_CLICKED(IDC_OEC_INDENT_TYPE_NONE,   OnChangeData)
  ON_BN_CLICKED(IDC_OEC_INDENT_TYPE_DEFAULT,OnChangeData)
  ON_BN_CLICKED(IDC_OEC_INDENT_TYPE_SMART,  OnChangeData)
  ON_BN_CLICKED(IDC_OEC_LINE_NUMBERS,       OnChangeData)
  ON_BN_CLICKED(IDC_OEC_VISIBLE_SPACES,     OnChangeData)
  ON_BN_CLICKED(IDC_OEC_NORMALIZE_KEYWORDS, OnChangeData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COEClassPage message handlers

LRESULT 
COEClassPage::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
  try 
  {
    return StyleDialog::WindowProc(message, wParam, lParam);
  }
  _OE_DEFAULT_HANDLER_;

  return 0;
}

BOOL 
COEClassPage::OnInitDialog() 
{
	StyleDialog::OnInitDialog();

  // Fill combo box
  m_comboClass.ResetContent();
  int count = m_manager.GetClassCount();
  for (int i(0); i < count; i++)
  {
    m_comboClass.AddString(m_manager.GetClassByPos(i).GetName().c_str());
  }
    
  // Fill file types
  m_comboFileType.ResetContent();
  m_comboFileType.AddString("Windows (LF/CR)");
  m_comboFileType.AddString("Unix (LF)");
  m_comboFileType.AddString("Mac OS (CR/LF)");
	
  m_comboClass.SetCurSel(0);
  OnSelChangeLanguage();
    
  m_spinTabSize   .SetRange32(0,32);
  m_spinIndentSize.SetRange32(0,32);

	return TRUE;
}

#pragma warning (disable: 4996)

void COEClassPage::OnSelChangeLanguage() 
{
  m_dataInitialized = false;

  int category = m_comboClass.GetCurSel();
  ClassSettings& settings = m_manager.GetClassByPos(category);
  m_comboClass.GetLBText(category,m_class);

  std::string delim;
  Common::to_printable_str(settings.GetDelimiters().c_str(), delim);
  m_delimiters = delim.c_str();

  int ind = m_comboFileType.SetCurSel(settings.GetFileCreateAs());
  m_comboFileType.GetLBText(ind, m_fileType);

  m_fileExtensions  = settings.GetExtensions().c_str();
  m_tabSize         = settings.GetTabSpacing();
  m_indentSize      = settings.GetIndentSpacing();
  m_insertSpaces    = settings.GetTabExpand();
  m_indent          = settings.GetIndentType();

  m_checkLineNumbers  .SetCheck(settings.GetLineNumbers());
  m_checkVisibleSpaces.SetCheck(settings.GetVisibleSpaces());
  m_checkNormalizeKW  .SetCheck(settings.GetNormalizeKeywords());

  m_dataInitialized = true;
  UpdateData(FALSE);
}

void 
COEClassPage::OnChangeData() 
{
  if(m_dataInitialized)
  {
    UpdateData();

    ClassSettings& settings = m_manager.GetClassByPos(m_comboClass.GetCurSel());

    settings.SetExtensions((std::string)m_fileExtensions, false);

    std::string delim;
    Common::to_unprintable_str (m_delimiters,delim);
	  settings.SetDelimiters(delim, false);
    settings.SetFileCreateAs  (m_comboFileType.GetCurSel(), false);
    settings.SetIndentSpacing (m_indentSize,    false);
    settings.SetTabSpacing    (m_tabSize,       false);
    settings.SetTabExpand     (m_insertSpaces,  false);
    settings.SetIndentType    (m_indent,        false);
    // Checks
    settings.SetLineNumbers      (m_checkLineNumbers  .GetCheck() ? true : false, false);
    settings.SetVisibleSpaces    (m_checkVisibleSpaces.GetCheck() ? true : false, false);
    settings.SetNormalizeKeywords(m_checkNormalizeKW  .GetCheck() ? true : false, false);
  }
}
