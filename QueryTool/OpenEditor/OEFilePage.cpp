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
#include <Common/DirSelectDlg.h>
#include "OpenEditor/OEFilePage.h"


COEFilePage::COEFilePage (SettingsManager& manager)
	: CPropertyPage(COEFilePage::IDD)
    , m_manager(manager)
{
    const OpenEditor::GlobalSettings& settings = m_manager.GetGlobalSettings();
    m_Locking            = settings.GetFileLocking() ? TRUE : FALSE;
    m_MemMapForBig       = settings.GetFileMemMapForBig() ? TRUE : FALSE;
    m_MemMapThreshold    = settings.GetFileMemMapThreshold();
    m_SaveOnSwitch       = settings.GetFileSaveOnSwith() ? TRUE : FALSE;
    m_DetectFileChanges  = settings.GetFileDetectChanges() ? TRUE : FALSE;
    m_ReloadChangedFiles = settings.GetFileReloadChanges() ? TRUE : FALSE;
    m_AutoscrollAfterReload = settings.GetFileAutoscrollAfterReload() ? TRUE : FALSE;
    m_OverwriteReadonly  = settings.GetFileOverwriteReadonly() ? TRUE : FALSE;
}

COEFilePage::~COEFilePage()
{
}

void COEFilePage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Check(pDX, IDC_OE_FILE_LOCKING, m_Locking);
    DDX_Check(pDX, IDC_OE_FILE_MEMMAP_FOR_BIG, m_MemMapForBig);
    DDX_Check(pDX, IDC_OE_FILE_SAVE_ON_SWITH, m_SaveOnSwitch);
    DDX_Check(pDX, IDC_OE_FILE_RELOAD_CHANGED, m_ReloadChangedFiles);
    DDX_Check(pDX, IDC_OE_FILE_OVERWRITE_READONLY, m_OverwriteReadonly);
    DDX_Text(pDX, IDC_OE_FILE_MEMMAP_THRESHOLD, m_MemMapThreshold);
    DDX_Check(pDX, IDC_OE_FILE_DETECT_CHANGES, m_DetectFileChanges);
    DDV_MinMaxUInt(pDX, m_MemMapThreshold, 1, 999999999);
    DDX_Check(pDX, IDC_OE_FILE_AUTOSCROLL_AFTER_RELOAD, m_AutoscrollAfterReload);

    GetDlgItem(IDC_OE_FILE_DETECT_CHANGES)->EnableWindow(!m_Locking);
    GetDlgItem(IDC_OE_FILE_RELOAD_CHANGED)->EnableWindow(!m_Locking && m_DetectFileChanges);
    GetDlgItem(IDC_OE_FILE_AUTOSCROLL_AFTER_RELOAD)->EnableWindow(!m_Locking && m_DetectFileChanges && m_ReloadChangedFiles);
    GetDlgItem(IDC_OE_FILE_MEMMAP_THRESHOLD)->EnableWindow(m_MemMapForBig);
}


BEGIN_MESSAGE_MAP(COEFilePage, CPropertyPage)
    ON_BN_CLICKED(IDC_OE_FILE_DETECT_CHANGES, OnUpdateData)
    ON_BN_CLICKED(IDC_OE_FILE_DETECT_CHANGES, OnUpdateData)
    ON_BN_CLICKED(IDC_OE_FILE_LOCKING,        OnUpdateData)
    ON_BN_CLICKED(IDC_OE_FILE_RELOAD_CHANGED, OnUpdateData)
    ON_BN_CLICKED(IDC_OE_FILE_MEMMAP_FOR_BIG, OnUpdateData)
END_MESSAGE_MAP()


// COEFilePage message handlers

BOOL COEFilePage::OnApply()
{
    try
    {
        OpenEditor::GlobalSettings& settings = m_manager.GetGlobalSettings();
        settings.SetFileLocking(m_Locking ? true : false, false /*notify*/);
        settings.SetFileMemMapForBig(m_MemMapForBig ? true : false, false /*notify*/);
        settings.SetFileMemMapThreshold(m_MemMapThreshold, false /*notify*/);
        settings.SetFileSaveOnSwith(m_SaveOnSwitch ? true : false, false /*notify*/);
        settings.SetFileDetectChanges(m_DetectFileChanges ? true : false, false /*notify*/);
        settings.SetFileReloadChanges(m_ReloadChangedFiles ? true : false, false /*notify*/);
        settings.SetFileAutoscrollAfterReload(m_AutoscrollAfterReload ? true : false, false /*notify*/);
        settings.SetFileOverwriteReadonly(m_OverwriteReadonly ? true : false, false /*notify*/);
    }
    _OE_DEFAULT_HANDLER_;

	return TRUE;
}

void COEFilePage::OnUpdateData()
{
    UpdateData();
}
