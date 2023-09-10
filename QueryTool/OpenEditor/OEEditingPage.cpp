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

/*
29.06.2003 improvement, "Restrict cursor" has been replaced with "Cursor beyond EOL" and "Cursor beyond EOF"
*/

#include "pch.h"
#include <COMMON/ExceptionHelper.h>
#include "OEEditingPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using OpenEditor::GlobalSettings;

/////////////////////////////////////////////////////////////////////////////
// COEEditingPage property page

COEEditingPage::COEEditingPage(SettingsManager& manager,CWnd* p_parent)
               :StyleDialog(COEEditingPage::IDD,p_parent)
               ,m_manager(manager)
{
  const GlobalSettings& settings = m_manager.GetGlobalSettings();

  m_DefFileExtension = settings.GetDefFileExtension();
  m_UndoMemLimit     = settings.GetUndoMemLimit() / 1024;
  m_MaxUndoCount     = settings.GetUndoLimit();
  m_UndoAfterSaving  = settings.GetUndoAfterSaving() ? TRUE : FALSE;
  m_TruncateSpaces   = settings.GetTruncateSpaces()  ? TRUE : FALSE;
	m_CursorBeyondEOL  = settings.GetCursorBeyondEOL() ? TRUE : FALSE;
	m_CursorBeyondEOF  = settings.GetCursorBeyondEOF() ? TRUE : FALSE;
}

COEEditingPage::~COEEditingPage()
{
}

void COEEditingPage::DoDataExchange(CDataExchange* pDX)
{
  StyleDialog::DoDataExchange(pDX);

  DDX_Control(pDX,IDC_OE_DEF_EXT,            m_editDefExt,   m_DefFileExtension);
  DDX_Control(pDX,IDC_OE_MAX_UNDO_MEM,       m_editUndoLimit,m_UndoMemLimit);
  DDX_Control(pDX,IDC_OE_MAX_UNDO_COUNT,     m_editUndoCount,m_MaxUndoCount);
  DDX_Control(pDX,IDC_OE_MAX_UNDO_MEM_SPIN,  m_spinLimit,   &m_editUndoLimit);
  DDX_Control(pDX,IDC_OE_MAX_UNDO_COUNT_SPIN,m_spinCount,   &m_editUndoCount);
  DDX_Control(pDX,IDC_OE_TRUNCATE_SPACES,    m_checkTruncSpaces);
  DDX_Control(pDX,IDC_OE_UNDO_AFTER_SAVING,  m_checkUndoAfter);
  DDX_Control(pDX,IDC_OE_CURSOR_BEYOND_EOL,  m_checkCursorBEOL);
  DDX_Control(pDX,IDC_OE_CURSOR_BEYOND_EOF,  m_checkCursorBEOF);

  DDX_Check(pDX, IDC_OE_TRUNCATE_SPACES,     m_TruncateSpaces);
  DDX_Check(pDX, IDC_OE_UNDO_AFTER_SAVING,   m_UndoAfterSaving);
  DDX_Check(pDX, IDC_OE_CURSOR_BEYOND_EOL,   m_CursorBeyondEOL);
  DDX_Check(pDX, IDC_OE_CURSOR_BEYOND_EOF,   m_CursorBeyondEOF);

  DDV_MaxChars (pDX, m_DefFileExtension, 16);
  DDV_MinMaxInt(pDX, m_MaxUndoCount, 0, 1000000);
  DDV_MinMaxInt(pDX, m_UndoMemLimit, 0, 1000000);

  if (!pDX->m_bSaveAndValidate)
  {
    m_checkCursorBEOF.EnableWindow(m_CursorBeyondEOL);
  }
  m_spinLimit.SetRange32(0, 1000000);
  m_spinCount.SetRange32(0, 1000000);
}

BEGIN_MESSAGE_MAP(COEEditingPage,StyleDialog)
  ON_BN_CLICKED(IDC_OE_CURSOR_BEYOND_EOL, OnBnClicked_CursorBeyondEol)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COEEditingPage message handlers

BOOL 
COEEditingPage::OnApply()
{
  try
  {
    GlobalSettings& settings = m_manager.GetGlobalSettings();
    settings.SetDefFileExtension((LPCSTR)m_DefFileExtension,      false /*notify*/);
    settings.SetUndoMemLimit   (m_UndoMemLimit * 1024,            false /*notify*/);
    settings.SetUndoLimit      (m_MaxUndoCount,                   false /*notify*/);
    settings.SetUndoAfterSaving(m_UndoAfterSaving ? true : false, false /*notify*/);
    settings.SetTruncateSpaces (m_TruncateSpaces  ? true : false, false /*notify*/);
    settings.SetCursorBeyondEOL(m_CursorBeyondEOL ? true : false, false /*notify*/);
    settings.SetCursorBeyondEOF(m_CursorBeyondEOL
                                && m_CursorBeyondEOF ? true : false, false /*notify*/);
  }
  _OE_DEFAULT_HANDLER_;

	return TRUE;
}

void 
COEEditingPage::OnBnClicked_CursorBeyondEol()
{
  BOOL check = m_checkCursorBEOL.GetCheck();
  m_checkCursorBEOF.EnableWindow(check);
}
