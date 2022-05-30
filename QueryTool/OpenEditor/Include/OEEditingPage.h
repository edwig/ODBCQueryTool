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

#pragma once
#include "resource.h"
#include "OESettings.h"

using OpenEditor::SettingsManager;

class COEEditingPage : public StyleDialog
{
public:
	COEEditingPage(SettingsManager& manager,CWnd* p_parent);
 ~COEEditingPage();

	enum { IDD = IDD_OE_EDITING };
 
  CString m_DefFileExtension;
  int 	  m_UndoMemLimit;
  int 	  m_MaxUndoCount;

  BOOL    m_UndoAfterSaving;
	BOOL	  m_CursorBeyondEOL;
	BOOL	  m_CursorBeyondEOF;
	BOOL	  m_TruncateSpaces;

  SettingsManager&    m_manager;
  StyleEdit           m_editDefExt;
  StyleEdit           m_editUndoLimit;
  StyleEdit           m_editUndoCount;
  StyleSpinButtonCtrl m_spinLimit;
  StyleSpinButtonCtrl m_spinCount;
  StyleCheckbox       m_checkUndoAfter;
  StyleCheckbox       m_checkCursorBEOL;
  StyleCheckbox       m_checkCursorBEOF;
  StyleCheckbox       m_checkTruncSpaces;

	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClicked_CursorBeyondEol();
};
