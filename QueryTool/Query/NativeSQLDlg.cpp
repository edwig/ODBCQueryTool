/* 
    Copyright (C) 2004 - 2017 Edwig Huisman

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
#include "NativeSQLDlg.h"

// CNativeSQLDlg dialog
IMPLEMENT_DYNAMIC(CNativeSQLDlg, StyleDialog)

CNativeSQLDlg::CNativeSQLDlg(CWnd*   p_parent
                            ,CString p_native
                            ,CString p_title)
	            :StyleDialog(CNativeSQLDlg::IDD, p_parent)
              ,m_native(p_native)
              ,m_title(p_title)
{
}

CNativeSQLDlg::~CNativeSQLDlg()
{
  if(m_font.m_hObject)
  {
    ::DeleteObject(m_font);
  }
}

void
CNativeSQLDlg::SetMonoFont()
{
  if(m_font.m_hObject != NULL)
  {
    return;
  }
  CString name = "Courier New";
  CDC* dc = GetDC();
  m_font.CreateFont(-MulDiv(10, dc->GetDeviceCaps(LOGPIXELSY), 72),
                    0,0,0,
                    FW_NORMAL, // Boldness
                    false,     // Italic
                    0,0,
                    ANSI_CHARSET,
                    OUT_DEFAULT_PRECIS,
                    CLIP_DEFAULT_PRECIS,
                    DEFAULT_QUALITY,
                    DEFAULT_PITCH | FF_MODERN,
                    (LPCTSTR)name.GetString());
  CWnd* box = GetDlgItem(IDC_EDIT_NATIVE_SQL);
  if(box)
  {
    box->SetFont(&m_font,true);
  }
  ReleaseDC(dc);
}

void CNativeSQLDlg::DoDataExchange(CDataExchange* pDX)
{
  StyleDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_EDIT_NATIVE_SQL,m_editNative,m_native);
  DDX_Control(pDX,IDOK,m_buttonOK);
}

BOOL 
CNativeSQLDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();
  SetWindowText("Native SQL");

  SetMonoFont();
  if(!m_title.IsEmpty())
  {
    SetWindowText(m_title);
  }
  ShowMinMaxButton(false,true);
  SetCanResize();

  return TRUE;
}

void
CNativeSQLDlg::SetupDynamicLayout()
{
  StyleDialog::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif

  CWnd* edit = (CWnd*) m_editNative.GetSkin();
  manager.AddItem(edit->GetSafeHwnd(),manager.MoveNone(), manager.SizeHorizontalAndVertical(100, 100));
  manager.AddItem(m_buttonOK,         manager.MoveHorizontalAndVertical(100, 100), manager.SizeNone());

  manager.Adjust();
}
