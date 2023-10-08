/* 
    Copyright (C) 2004-2023 Edwig Huisman

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
#include "AboutDlg.h"
#include "Version.h"

// CAboutDlg dialog

IMPLEMENT_DYNAMIC(CAboutDlg, StyleDialog)
CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
          :StyleDialog(CAboutDlg::IDD, pParent)
{
  m_license = _T("ODBC Query Tool ") ODBCQUERYTOOL_VERSION _T(" Build: ") ODBCQUERYTOOL_BUILD
              _T("\r\n") ODBCQUERYTOOL_COPYRIGHT
              _T("\r\n")
              _T("\r\nThis program is free software; you can redistribute it") 
              _T("\r\n")
              _T("\r\nThis program is distributed in the hope that it will be useful,")
              _T(" but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY")
              _T(" or FITNESS FOR A PARTICULAR PURPOSE. See the license file for more details.")
              _T("\r\n")
              _T("\r\nCredits go to the following persons:")
              _T("\r\n")
              _T("\r\nFor the OpenEditor: Aleksey Kochetov")
              _T("\r\nFor his superb data grid: Chris Maunder")
              _T("\r\nFor the ODBC ideas: B.P. Nebbeling")
              _T("\r\nFor all the testing: Vincent Heuveling")
              _T("\r\nFor editable query grids: Marco Brattinga")
              _T("\r\nFor the binary coded decimal: Henrik Vestermark")
              _T("\r\nFor NUMERIC corrections: Ibo Boekestijn")
              _T("\r\nFor GUID corrections: Dann Corbit")
              _T("\r\nFor documentation ideas: Chandra Sekhar")
              _T("\r\nFor the table ideas: Dennis Walkewitz");
}

CAboutDlg::~CAboutDlg()
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
  StyleDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_OEA_LICENSE,m_editLicense,m_license);
  DDX_Control(pDX,IDOK,           m_buttonOK);
}

BEGIN_MESSAGE_MAP(CAboutDlg, StyleDialog)
	ON_STN_CLICKED(IDC_OEA_LOGO, OnStnClickedOeaLogo)
END_MESSAGE_MAP()

BOOL
CAboutDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();
  SetWindowText(_T("About Open ODBC QueryTool"));

  return TRUE;
}

// CAboutDlg message handlers

void CAboutDlg::OnStnClickedOeaLogo()
{
	// TODO: Add your control notification handler code here
  AfxMessageBox(_T("OK, This is the logo"));
}
