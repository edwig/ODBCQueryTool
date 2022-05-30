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

class COEBlockPage : public StyleDialog
{
public:
	COEBlockPage (SettingsManager& manager,CWnd* p_parent);
  virtual ~COEBlockPage();

	enum { IDD = IDD_OE_BLOCKS };

	BOOL	m_BlockKeepMarking;
	BOOL	m_BlockKeepMarkingAfterDragAndDrop;
	BOOL	m_BlockDelAndBSDelete;
	BOOL	m_BlockTypingOverwrite;
	BOOL	m_BlockTabIndent;

	int		m_ColBlockDeleteSpaceAfterMove;
	int		m_ColBlockCursorToStartAfterPaste;

	SettingsManager& m_manager;

  StyleCheckbox   m_checkKeepMarking;
  StyleCheckbox   m_checkKeemAfterDD;
  StyleCheckbox   m_checkDelAndBS;
  StyleCheckbox   m_checkOverwrite;
  StyleCheckbox   m_checkTabIndent;

  StyleCheckbox   m_radioDeleteAfterMove;
  StyleCheckbox   m_radioLeaveAfterMove;
  StyleCheckbox   m_radioToStartAfterPaste;
  StyleCheckbox   m_radioToEndAfterPaste;

public:
	virtual BOOL OnApply();
protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;

protected:
};
