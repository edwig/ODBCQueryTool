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
#include <COMMON/ExceptionHelper.h>
#include "OpenEditor/OEPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COEPropPage property page

COEPropPage::COEPropPage(const SettingsManager& manager, InstanceSettings& settings)  
: CPropertyPage(COEPropPage::IDD),
m_manager(manager), m_settings(settings)
{
    m_dataInitialized = false;

	//{{AFX_DATA_INIT(COEPropPage)
	//}}AFX_DATA_INIT
}

COEPropPage::~COEPropPage()
{
}

void COEPropPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COEPropPage)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COEPropPage, CPropertyPage)
	//{{AFX_MSG_MAP(COEPropPage)
	ON_CBN_SELCHANGE(IDC_OEC_LANGUAGE, OnSelChangeLanguage)
	ON_EN_CHANGE(IDC_OEC_TAB_SIZE, OnChangeData)
	ON_CBN_SELCHANGE(IDC_OEC_FILE_TYPE, OnChangeData)
	ON_EN_CHANGE(IDC_OEC_INDENT_SIZE, OnChangeData)
	ON_BN_CLICKED(IDC_OEC_INDENT_TYPE_DEFAULT, OnChangeData)
	ON_BN_CLICKED(IDC_OEC_INDENT_TYPE_NONE, OnChangeData)
	ON_BN_CLICKED(IDC_OEC_INDENT_TYPE_SMART, OnChangeData)
    ON_BN_CLICKED(IDC_OEC_INSERT_SPACES, OnChangeData)
    ON_BN_CLICKED(IDC_OEC_KEEP_TABS, OnChangeData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COEPropPage message handlers

LRESULT COEPropPage::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
    try 
    {
        return CPropertyPage::WindowProc(message, wParam, lParam);
    }
    _OE_DEFAULT_HANDLER_;

    return 0;
}


BOOL COEPropPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

    SendDlgItemMessage(IDC_OEC_LANGUAGE, CB_RESETCONTENT);
    
    int count = m_manager.GetClassCount();
    for (int i(0); i < count; i++)
    {
        const std::string& name = m_manager.GetClassByPos(i).GetName();

        SendDlgItemMessage(IDC_OEC_LANGUAGE, CB_ADDSTRING, 0, (LPARAM)name.c_str());

        if (name == m_settings.GetName())
            SendDlgItemMessage(IDC_OEC_LANGUAGE, CB_SETCURSEL, i);
    }

    SendDlgItemMessage(IDC_OEC_FILE_TYPE, CB_RESETCONTENT);
    SendDlgItemMessage(IDC_OEC_FILE_TYPE, CB_ADDSTRING, 0, (LPARAM)"Default");
    SendDlgItemMessage(IDC_OEC_FILE_TYPE, CB_ADDSTRING, 0, (LPARAM)"Dos (LF/CR)");
    SendDlgItemMessage(IDC_OEC_FILE_TYPE, CB_ADDSTRING, 0, (LPARAM)"Unix (LF)");
    SendDlgItemMessage(IDC_OEC_FILE_TYPE, CB_ADDSTRING, 0, (LPARAM)"Mac (CR/LF)");

    doSelChangeLanguage();
    
    SendDlgItemMessage(IDC_OEC_INDENT_SIZE_SPIN, UDM_SETRANGE32, 0, 32);
    SendDlgItemMessage(IDC_OEC_TAB_SIZE_SPIN,    UDM_SETRANGE32, 0, 32);

	return TRUE;
}


void COEPropPage::doSelChangeLanguage ()
{
    m_dataInitialized = false;

    char buff[80];
    SetDlgItemText(IDC_OEC_INDENT_SIZE, itoa(m_settings.GetIndentSpacing(), buff, 10));
    SetDlgItemText(IDC_OEC_TAB_SIZE,    itoa(m_settings.GetTabSpacing(), buff, 10));   

    CheckRadioButton(IDC_OEC_INDENT_TYPE_NONE, 
                     IDC_OEC_INDENT_TYPE_NONE + 2, 
                     IDC_OEC_INDENT_TYPE_NONE + m_settings.GetIndentType());

    CheckRadioButton(IDC_OEC_INSERT_SPACES, 
                     IDC_OEC_INSERT_SPACES + 1, 
                     IDC_OEC_INSERT_SPACES + (m_settings.GetTabExpand() ? 0 : 1));

    SendDlgItemMessage(IDC_OEC_FILE_TYPE, CB_SETCURSEL, m_settings.GetFileSaveAs() + 1);

    m_dataInitialized = true;
}


void COEPropPage::OnSelChangeLanguage() 
{
    m_settings.SetClassSetting(&m_manager.GetClassByPos((int)SendDlgItemMessage(IDC_OEC_LANGUAGE, CB_GETCURSEL)));
    doSelChangeLanguage();
}



void COEPropPage::OnChangeData() 
{
    if (m_dataInitialized)
    {
        char buff[80];
        GetDlgItemText(IDC_OEC_INDENT_SIZE, buff, sizeof(buff));
	    m_settings.SetIndentSpacing(atoi(buff), false);
        GetDlgItemText(IDC_OEC_TAB_SIZE, buff, sizeof(buff));
	    m_settings.SetTabSpacing(atoi(buff), false);

        if (IsDlgButtonChecked(IDC_OEC_INDENT_TYPE_SMART))
	        m_settings.SetIndentType(2, false);
        else if (IsDlgButtonChecked(IDC_OEC_INDENT_TYPE_DEFAULT))
	        m_settings.SetIndentType(1, false);
        else
	        m_settings.SetIndentType(0, false);

        if (IsDlgButtonChecked(IDC_OEC_INSERT_SPACES))
	        m_settings.SetTabExpand(true, false);
        else
	        m_settings.SetTabExpand(false, false);

        m_settings.SetFileSaveAs((int)SendDlgItemMessage(IDC_OEC_FILE_TYPE, CB_GETCURSEL) - 1, false);
    }
}

