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

#ifndef __WorkbookMDIFrame_H__
#define __WorkbookMDIFrame_H__
#pragma once

#include "OpenEditor/OEStorage.h"
#include "OpenEditor/OESettings.h"
#include "OpenEditor/OETemplateStreams.h"
#include "OpenEditor/OESettingsStreams.h"
#include "FilePanelWnd.h"
#include "WorkbookBar.h"
#include "GridControl/GridCtrl.h"
#include "Common/PropertySheetMem.h"

class CWorkbookControlBar : public baseCMyBar
{
    afx_msg void OnContextMenu (CWnd*, CPoint);
    afx_msg void OnCbar_Docking();
    afx_msg void OnCbar_Hide();
	DECLARE_MESSAGE_MAP()
};

class CWorkbookMDIFrame : public CMDIFrameWnd
{
	DECLARE_DYNCREATE(CWorkbookMDIFrame)
protected:
	CWorkbookMDIFrame();
	virtual ~CWorkbookMDIFrame();

protected:
  int IDC_MF_WORKBOOK_BAR;
  int IDC_MF_FILEPANEL_BAR;
  int IDC_MF_FILEPANEL;
  int IDC_MF_FINDPANEL_BAR;
  int IDC_MF_FINDPANEL;
  const char* m_cszProfileName;
  const char* m_cszMainFrame;
  const char* m_cszWP;
  
  HWND m_hActiveChild, m_hLastChild, m_hSkipChild;
  BOOL m_bMDINextSeq;

  BOOL                m_bSaveMainWinPosition;
  BOOL                m_bShowed;
  CString             m_strProfileName;
  CWorkbookBar        m_wndWorkbookBar;
  CWorkbookControlBar	m_wndFilePanelBar;
  CFilePanelWnd	      m_wndFilePanel;

  CWorkbookControlBar m_wndFindPanelBar;
  CGridCtrl           m_wndFindPanel;

	CFont	              m_font; // default gui font for bar controls
  CString             m_defaultFileExtension;
  
  int DoCreate (BOOL loadBarState = TRUE);

  BOOL VerifyBarState ();
  BOOL LoadBarState   ();
//  void DockControlBarLeftOf (CWorkbookControlBar* Bar, CWorkbookControlBar* LeftOf);
  void DockControlBarLeftOf (CControlBar* Bar, CControlBar* LeftOf);
  friend class CMDIChildFrame;
  void OnCreateChild   (CMDIChildWnd*);
  void OnDestroyChild  (CMDIChildWnd*);
  void OnActivateChild (CMDIChildWnd*);
  void OnRenameChild   (CMDIChildWnd*, LPCTSTR);
  int  GetImageByDocument (const CDocument*);
  void InitGridEmpty();
  void LoadSettingsManager();
  void SaveSettingsManager();

  friend class CFilePanelWnd;
  void ActivateChild (CMDIChildWnd*);

  afx_msg void OnChangeWorkbookTab (NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnSysCommand (UINT nID, LPARAM lParam);

protected:
	DECLARE_MESSAGE_MAP()
public:
  void    SetDefaultFileExtension (const char* ext)  { m_defaultFileExtension = ext; }
  void    FindTable(CString& table);
  CString FindNativeSQL(CString& command);
  void    ReportCapabilities(CString& filename);

  afx_msg void OnClose();
  afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnViewWorkbook ();
	afx_msg void OnUpdateViewWorkbook (CCmdUI* pCmdUI);
  afx_msg void OnViewFilePanel();
  afx_msg void OnUpdateViewFilePanel(CCmdUI *pCmdUI);
  afx_msg void OnViewFindPanel();
  afx_msg void OnUpdateViewFindPanel(CCmdUI *pCmdUI);
  afx_msg void OnViewFilePanelSync();
  afx_msg void OnUpdateViewFilePanelSync(CCmdUI *pCmdUI);
	afx_msg BOOL OnToolTipText(UINT nID, NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnFpwRefreshOdbc();
  afx_msg void OnFindFiles();
  virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnLastWindow ();
  afx_msg void OnFindFilesClicked(NMHDR * pNotifyStruct,LRESULT * result);
  virtual BOOL PreTranslateMessage(MSG* pMsg);

  static OpenEditor::SettingsManager m_settingsManager;
};

#endif//__WorkbookMDIFrame_H__
