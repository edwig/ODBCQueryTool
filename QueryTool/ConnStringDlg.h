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

#pragma once
#include <StyleDialog.h>
#include "resource.h"

class ConnStringDlg: public StyleDialog
{
public:
  ConnStringDlg(CWnd* p_parent,CString p_connString);
  virtual ~ConnStringDlg();

  CString GetConnectionString() { return m_connString; }

  // Dialog Data
  enum { IDD = IDD_CON_STRING };

private:
  CString     m_connString;
  StyleEdit   m_editConnString;
  StyleButton m_buttonOK;
  StyleButton m_buttonCancel;

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual void SetupDynamicLayout() override;
  virtual BOOL OnInitDialog() override;

  DECLARE_MESSAGE_MAP()

  afx_msg void OnKillfocusConnString();
  afx_msg void OnOK();
  afx_msg void OnCancel();
};

