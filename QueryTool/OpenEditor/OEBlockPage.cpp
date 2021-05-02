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
    23.03.2003 improvement, added a new editor option - keep selection after drag and drop operation
*/

#include "stdafx.h"
#include "resource.h"
#include <COMMON/ExceptionHelper.h>
#include "OpenEditor/OEBlockPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

    using OpenEditor::GlobalSettings;

COEBlockPage::COEBlockPage (SettingsManager& manager) 
: CPropertyPage(COEBlockPage::IDD),
m_manager(manager)
{
    /*
	//{{AFX_DATA_INIT(COEBlockPage)
	m_BlockKeepMarking                = FALSE;
	m_BlockDelAndBSDelete             = FALSE;
	m_BlockTypingOverwrite            = FALSE;
	m_BlockTabIndent                  = FALSE;
	m_ColBlockDeleteSpaceAfterMove    = -1;
	m_ColBlockCursorToStartAfterPaste = -1;
	//}}AFX_DATA_INIT
    */
    const GlobalSettings& settings = m_manager.GetGlobalSettings();
	m_BlockKeepMarking                = settings.GetBlockKeepMarking()               ;
	m_BlockDelAndBSDelete             = settings.GetBlockDelAndBSDelete()            ;
	m_BlockTypingOverwrite            = settings.GetBlockTypingOverwrite()           ;
	m_BlockTabIndent                  = settings.GetBlockTabIndent()                 ;
    m_ColBlockDeleteSpaceAfterMove    = settings.GetColBlockDeleteSpaceAfterMove() ? 0 : 1;
	m_ColBlockCursorToStartAfterPaste = settings.GetColBlockCursorToStartAfterPaste();
    m_BlockKeepMarkingAfterDragAndDrop = settings.GetBlockKeepMarkingAfterDragAndDrop();
}

COEBlockPage::~COEBlockPage()
{
}

void COEBlockPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COEBlockPage)
	DDX_Check(pDX, IDC_OEB_KEEP_BLOCK_MARKING,       m_BlockKeepMarking               );
	DDX_Check(pDX, IDC_OEB_DEL_A_BS_DELETE_BLOCK,    m_BlockDelAndBSDelete            );
	DDX_Check(pDX, IDC_OEB_TYPING_OVERWRITE_BLOCK,   m_BlockTypingOverwrite           );
	DDX_Check(pDX, IDC_OEB_TAB_INDENT_BLOCK,         m_BlockTabIndent                 );
	DDX_Radio(pDX, IDC_OEB_COL_BLK_DELETE_SPACE,     m_ColBlockDeleteSpaceAfterMove   );
	DDX_Radio(pDX, IDC_OEB_COL_BLK_LEAVE_CUR_AT_BEG, m_ColBlockCursorToStartAfterPaste);
	//}}AFX_DATA_MAP
    DDX_Check(pDX, IDC_OEB_KEEP_BLOCK_MARKING_AFTER_DROP, m_BlockKeepMarkingAfterDragAndDrop);
}

/*
BEGIN_MESSAGE_MAP(COEBlockPage, CPropertyPage)
	//{{AFX_MSG_MAP(COEBlockPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
*/
/////////////////////////////////////////////////////////////////////////////
// COEBlockPage message handlers

BOOL COEBlockPage::OnApply() 
{
    try
    {
        GlobalSettings& settings = m_manager.GetGlobalSettings();

        settings.SetBlockKeepMarking                    (m_BlockKeepMarking ? true : false, false /*notify*/);
        settings.SetBlockDelAndBSDelete                 (m_BlockDelAndBSDelete ? true : false, false /*notify*/);
        settings.SetBlockTypingOverwrite                (m_BlockTypingOverwrite ? true : false, false /*notify*/);
        settings.SetBlockTabIndent                      (m_BlockTabIndent ? true : false, false /*notify*/);
        settings.SetColBlockDeleteSpaceAfterMove        (m_ColBlockDeleteSpaceAfterMove ? false : true, false /*notify*/);
        settings.SetColBlockCursorToStartAfterPaste     (m_ColBlockCursorToStartAfterPaste ? true : false, false /*notify*/);
        settings.SetBlockKeepMarkingAfterDragAndDrop    (m_BlockKeepMarkingAfterDragAndDrop ? true : false, false /*notify*/);
    }
    _OE_DEFAULT_HANDLER_;

	return TRUE;
}
