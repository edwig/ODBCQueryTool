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
#include <COMMON/ExceptionHelper.h>
#include "OEPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COEPropPage property page

COEPropPage::COEPropPage(const SettingsManager& manager, InstanceSettings& settings,CWnd* p_parent)  
            :StyleDialog(COEPropPage::IDD,p_parent)
            ,m_manager(manager)
            ,m_settings(settings)
{
  m_initialized = false;
}

COEPropPage::~COEPropPage()
{
}

void 
COEPropPage::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);

  DDX_Control(pDX, IDC_OEC_LANGUAGE,            m_comboLanguage);
  DDX_Control(pDX, IDC_OEC_FILE_TYPE,           m_comboFileType);
  DDX_Control(pDX, IDC_OEC_TAB_SIZE,            m_editTabSize,    m_tabSize);
  DDX_Control(pDX, IDC_OEC_INDENT_SIZE,         m_editIndentSize, m_indentSize);
  DDX_Control(pDX, IDC_OEC_TAB_SIZE_SPIN,       m_spinTabSize,   &m_editTabSize);
  DDX_Control(pDX, IDC_OEC_INDENT_SIZE_SPIN,    m_spinIndentSize,&m_editIndentSize);
  DDX_Control(pDX, IDC_OEC_INSERT_SPACES,       m_radioInsSpaces);
  DDX_Control(pDX, IDC_OEC_KEEP_TABS,           m_radioKeepTabs);
  DDX_Control(pDX, IDC_OEC_INDENT_TYPE_NONE,    m_radioIndNone);
  DDX_Control(pDX, IDC_OEC_INDENT_TYPE_DEFAULT, m_radioIndDefault);
  DDX_Control(pDX, IDC_OEC_INDENT_TYPE_SMART,   m_radioIndSmart);

  DDV_StyleMinMaxInt(pDX, IDC_OEC_TAB_SIZE,     m_tabSize,   0,32);
  DDV_StyleMinMaxInt(pDX, IDC_OEC_INDENT_SIZE,  m_indentSize,0,32);
}


BEGIN_MESSAGE_MAP(COEPropPage, StyleDialog)
	ON_CBN_SELCHANGE(IDC_OEC_LANGUAGE,            OnCbnSelChangeLanguage)
  ON_CBN_SELCHANGE(IDC_OEC_FILE_TYPE,           OnCbnSelChangeFileType)
	ON_EN_CHANGE    (IDC_OEC_TAB_SIZE,            OnEnChangeTabSize)
  ON_EN_CHANGE    (IDC_OEC_INDENT_SIZE,         OnEnChangeIndentSize)
  ON_BN_CLICKED   (IDC_OEC_INSERT_SPACES,       OnBnClickedInsertSpaces)
  ON_BN_CLICKED   (IDC_OEC_KEEP_TABS,           OnBnClickedKeepTabs)
	ON_BN_CLICKED   (IDC_OEC_INDENT_TYPE_DEFAULT, OnBnClickedIndDefault)
	ON_BN_CLICKED   (IDC_OEC_INDENT_TYPE_NONE,    OnBnClickedIndNone)
	ON_BN_CLICKED   (IDC_OEC_INDENT_TYPE_SMART,   OnBnClickedIndSmart)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COEPropPage message handlers

LRESULT 
COEPropPage::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
  try 
  {
    return StyleDialog::WindowProc(message, wParam, lParam);
  }
  _OE_DEFAULT_HANDLER_;

  return 0;
}

BOOL 
COEPropPage::OnInitDialog() 
{
	StyleDialog::OnInitDialog();

  int count = m_manager.GetClassCount();
  for (int i(0); i < count; i++)
  {
      const CString& name = m_manager.GetClassByPos(i).GetName();

      m_comboLanguage.AddString(name);
      if (name == m_settings.GetName())
      {
        m_comboLanguage.SetCurSel(i);
      }
  }

  m_comboFileType.AddString(_T("Default"));
  m_comboFileType.AddString(_T("Windows (LF/CR)"));
  m_comboFileType.AddString(_T("Unix (LF)"));
  m_comboFileType.AddString(_T("Mac OS (CR/LF)"));

  // Set spin controls and buddies.
  m_spinTabSize   .SetRange(0, 32);
  m_spinIndentSize.SetRange(0, 32);

  ChangeLanguage();
  return TRUE;
}

void 
COEPropPage::ChangeLanguage()
{
  m_initialized = false;

  switch(m_settings.GetTabExpand())
  {
    case 0: m_radioKeepTabs .SetCheck(1); break;
    case 1: m_radioInsSpaces.SetCheck(1); break;
  }
  switch(m_settings.GetIndentType())
  {
    case 0: m_radioIndNone   .SetCheck(1); break;
    case 1: m_radioIndDefault.SetCheck(1); break;
    case 2: m_radioIndSmart  .SetCheck(1); break;
  }
  m_comboFileType.SetCurSel(m_settings.GetFileSaveAs() + 1);
  m_indentSize = m_settings.GetIndentSpacing();
  m_tabSize    = m_settings.GetTabSpacing();

  m_initialized = true;
  UpdateData(FALSE);
}

void 
COEPropPage::OnCbnSelChangeLanguage() 
{
  int ind = m_comboLanguage.GetCurSel();
  if (ind >= 0)
  {
    m_settings.SetClassSetting(&m_manager.GetClassByPos(ind));
    ChangeLanguage();
  }
}

void
COEPropPage::OnCbnSelChangeFileType()
{
  int ind = m_comboFileType.GetCurSel();
  if (ind >= 0)
  {
    m_settings.SetFileSaveAs(ind - 1);
  }
}

void
COEPropPage::OnEnChangeTabSize()
{
  if(m_initialized)
  {
    UpdateData();
    m_settings.SetTabSpacing(m_tabSize);
  }
}

void
COEPropPage::OnEnChangeIndentSize()
{
  if(m_initialized)
  {
    UpdateData();
    m_settings.SetIndentSpacing(m_indentSize);
  }
}

void 
COEPropPage::OnBnClickedInsertSpaces()
{
  bool ins = m_radioInsSpaces.GetCheck() > 0;
  m_settings.SetTabExpand(ins, false);
}

void 
COEPropPage::OnBnClickedKeepTabs()
{
  int keep = m_radioKeepTabs.GetCheck() > 0;
  m_settings.SetTabExpand(!keep,false);
}

void 
COEPropPage::OnBnClickedIndNone()
{
  if(m_radioIndNone.GetCheck())
  {
    m_settings.SetIndentType(0,false);
  }
}

void 
COEPropPage::OnBnClickedIndDefault()
{
  if(m_radioIndDefault.GetCheck())
  {
    m_settings.SetIndentType(1,false);
  }
}

void 
COEPropPage::OnBnClickedIndSmart()
{
  if (m_radioIndSmart.GetCheck())
  {
    m_settings.SetIndentType(2,false);
  }
}
