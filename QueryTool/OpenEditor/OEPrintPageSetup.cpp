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
#include "OEPrintPageSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define OEPP_GET_PROPERTY(N)         m_##N = settings.GetPrint##N();
#define OEPP_GET_STR_PROPERTY(N)     m_##N = settings.GetPrint##N().c_str();
#define OEPP_SET_PROPERTY(N)         settings.SetPrint##N(m_##N);
#define OEPP_SET_STR_PROPERTY(N)     settings.SetPrint##N((const char*)m_##N);

/////////////////////////////////////////////////////////////////////////////
// COEPrintPageSetup dialog

COEPrintPageSetup::COEPrintPageSetup(OpenEditor::SettingsManager& manager)
	                :StyleDialog(COEPrintPageSetup::IDD, NULL)
                  ,m_manager(manager)
{
  const OpenEditor::GlobalSettings& settings = m_manager.GetGlobalSettings();

  OEPP_GET_PROPERTY(BlackAndWhite);
  OEPP_GET_STR_PROPERTY(Header);
  OEPP_GET_STR_PROPERTY(Footer);

  OEPP_GET_PROPERTY(LeftMargin);
  OEPP_GET_PROPERTY(RightMargin);
  OEPP_GET_PROPERTY(TopMargin);
  OEPP_GET_PROPERTY(BottomMargin);
}


void COEPrintPageSetup::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);

  DDX_Check  (pDX,IDC_PPS_BLACK_AND_WHITE, m_BlackAndWhite);
  DDX_Control(pDX,IDC_PPS_HEADER,m_editHeader,m_Header);
  DDX_Control(pDX,IDC_PPS_FOOTER,m_editFooter,m_Footer);

  DDX_Control(pDX,IDC_PPS_LEFT,  m_editLeft,m_LeftMargin);
	DDV_MinMaxDouble(pDX,          m_LeftMargin,0.,9999.);

  DDX_Control(pDX,IDC_PPS_RIGHT, m_editRight,m_RightMargin);
	DDV_MinMaxDouble(pDX,          m_RightMargin, 0., 9999.);

  DDX_Control(pDX,IDC_PPS_TOP,   m_editTop,m_TopMargin);
	DDV_MinMaxDouble(pDX,          m_TopMargin, 0., 9999.);

  DDX_Control(pDX,IDC_PPS_BOTTOM,m_editBottom,m_BottomMargin);
	DDV_MinMaxDouble(pDX,          m_BottomMargin, 0., 9999.);

  DDX_Control(pDX, IDOK,         m_buttonOK);
  DDX_Control(pDX, IDCANCEL,     m_buttonCancel);

  if (pDX->m_bSaveAndValidate)
  {
    OpenEditor::GlobalSettings& settings = m_manager.GetGlobalSettings();

#pragma warning (disable : 4800)
    OEPP_SET_PROPERTY(BlackAndWhite);
    OEPP_SET_STR_PROPERTY(Header);
    OEPP_SET_STR_PROPERTY(Footer);

    OEPP_SET_PROPERTY(LeftMargin);
    OEPP_SET_PROPERTY(RightMargin);
    OEPP_SET_PROPERTY(TopMargin);
    OEPP_SET_PROPERTY(BottomMargin);
  }
}

BOOL
COEPrintPageSetup::OnInitDialog()
{
  StyleDialog::OnInitDialog();
  SetWindowText("Print page setup");
  return InitFirstFocus();
}

/*
BEGIN_MESSAGE_MAP(COEPrintPageSetup, CDialog)
	//{{AFX_MSG_MAP(COEPrintPageSetup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
*/
/////////////////////////////////////////////////////////////////////////////
// COEPrintPageSetup message handlers
