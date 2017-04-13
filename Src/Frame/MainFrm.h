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

#ifndef __MAINFRM_H__
#define __MAINFRM_H__
#pragma once

#include "COMMON/WorkbookMDIFrame.h"
#include "ConnectionBar.h"

class CMDIMainFrame : public CWorkbookMDIFrame
{
	DECLARE_DYNAMIC(CMDIMainFrame)
public:
  using CWorkbookMDIFrame::m_bSaveMainWinPosition;
  static LPCSTR m_cszClassName;
  static UINT m_nOpenFileMsg;

	CMDIMainFrame();
	virtual ~CMDIMainFrame();

protected:
	CStatusBar    m_wndStatusBar;
	CToolBar      m_wndToolBar;
  CToolBar      m_dbToolBar;
  ConnectionBar m_conToolBar;

public:
	DECLARE_MESSAGE_MAP()
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
  afx_msg void OnConnectBar();
  afx_msg void OnDatabaseBar();
  afx_msg void OnSupportBug();
  afx_msg void OnSupportFeature();
  afx_msg void OnWebSourceforge();
  afx_msg void OnWebGithub();
  afx_msg void OnUpdateConnect(CCmdUI* pCmdUI);
  afx_msg void OnUpdateDisconnect(CCmdUI* pCmdUI);
  afx_msg void OnUpdateConnectBarVisible(CCmdUI* pCmdUI);
  afx_msg void OnUpdateDatabaseBarVisible(CCmdUI* pCmdUI);
  afx_msg void OnUpdateBegin(CCmdUI* pCmdUI);
  afx_msg void OnUpdateCommit(CCmdUI* pCmdUI);
  afx_msg void OnUpdateRollback(CCmdUI* pCmdUI);
  afx_msg void OnUpdateODBCReport(CCmdUI* pCmdUI);
  afx_msg void OnUpdateSessionStatus(CCmdUI* pCmdUI);

  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  void SendSupportEmail(CString p_subject,CString p_body);

public:
  void SetConnectionText(CString& text);

//#if _MFC_VER > 0x0600
    afx_msg void OnActivateApp(BOOL bActive, DWORD dwThreadID);
//#else
//    afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
//#endif

private:
  bool m_conBarVisible;
  bool m_dbBarVisible;
};

#endif//__MAINFRM_H__
