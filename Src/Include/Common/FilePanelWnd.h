/* 
    Copyright (C) 2002 Aleksey Kochetov
    Copyright (C) 2006 - 2017 Edwig Huisman

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
#include "Common/FakeRClick.h"
#include "Frame/DirTreeCtrl.h"
#include "Frame/ObjectTree.h"
#include "Frame/InfoTree.h"

class CFilePanelWnd : public CTabCtrl
{
	DECLARE_DYNAMIC(CFilePanelWnd)

public:
	CFilePanelWnd ();
	virtual ~CFilePanelWnd ();

    CListCtrl&  GetOpenFilesListCtrl () { return m_openFilesList; }
    CImageList& GetSysImageList() { return m_explorerTree.GetSysImageList(); }

    void   OpenFiles_Append (LVITEM&);
    void   OpenFiles_UpdateByParam (LPARAM param, LVITEM&);
    void   OpenFiles_RemoveByParam (LPARAM param);
    void   OpenFiles_ActivateByParam (LPARAM param);
    LPARAM OpenFiles_GetCurSelParam ();

    const CString& GetCurDrivePath () const;
    void    SetCurDrivePath (const CString&);
    BOOL    SetCurPath (const CString&);
    void    FindTable(CString& table);
    CString FindNativeSQL(CString& command);
    void    ReportCapabilities(CString& filename);

protected:
    int OpenFiles_FindByParam (LPARAM param);

protected:
    BOOL         m_isExplorerInitialized;
    BOOL         m_isDrivesInitialized;
    bool         m_isOdbcInitialized;
    CString      m_curDrivePath;

    CImageList   m_explorerStateImageList;
    CListCtrl    m_openFilesList;
    CDirTreeCtrl m_explorerTree;
    InfoTree     m_odbcTree;
    ObjectTree   m_tableTree;
    CComboBoxEx  m_drivesCBox;
    CComboBox    m_filterCBox;
    CComboBox    m_tableCBox;
    CFakeRClick  m_drivesRClick;
    CFakeRClick  m_filterRClick;
    CStringArray m_driverPaths;
    CStringArray m_filters;
    CStringArray m_tables;

    void SelectDrive    (const CString&,     BOOL force   = FALSE);
    void DisplayFilters (BOOL force = FALSE, BOOL curOnly = FALSE);
    void DisplayDrivers (BOOL force = FALSE, BOOL curOnly = FALSE);
    void DisplayTables  (BOOL force = FALSE, BOOL curOnly = FALSE);
    void AddToFilters   (CString& filter);
    void AddToTables    (CString& table);
    void ChangeTab (int);
    void ActivateOpenFile ();

protected:
	  DECLARE_MESSAGE_MAP()

    BOOL PreTranslateMessage(MSG* pMsg);
    int  m_tab { 0 };
public:
    afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnDestroy();
    afx_msg void OnSize (UINT nType, int cx, int cy);
    afx_msg void OnTab_SelChange(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnDrive_SetFocus();
    afx_msg void OnDrive_SelChange();
    afx_msg void OnFilter_SetFocus();
    afx_msg void OnFilter_SelChange();
    afx_msg void OnTable_SetFocus();
    afx_msg void OnTable_SelChange();
    afx_msg void OnTable_Enter();
    afx_msg void OnOpenFiles_Click      (NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnOpenFiles_KeyDown    (NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnOpenFiles_RClick     (NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnExplorerTree_DblClick(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnExplorerTree_RClick  (NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnDrivers_RClick       (NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnFpwOpen();
    afx_msg void OnFpwRefresh();
    afx_msg void OnFpwSetWorDir();
    afx_msg void OnFpwRefreshDrivers();
    afx_msg void OnFpwRefreshOdbc();
    afx_msg BOOL OnTooltipNotify(UINT wnd,NMHDR* tooltip,LRESULT* res);
};

inline
const CString& CFilePanelWnd::GetCurDrivePath () const
{ 
  return m_curDrivePath; 
}

inline
void CFilePanelWnd::SetCurDrivePath (const CString& curDrivePath)
{ 
  SelectDrive(curDrivePath); 
}
