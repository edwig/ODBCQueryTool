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

// 28.06.2002 improvement, double key accelerator, for example "Ctrl+K,B"

#ifndef __OpenEditorApp_H__
#define __OpenEditorApp_H__
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"
#include <Common/AppGlobal.h>
#include "COMMON/DocManagerExt.h"
#include "SQLDatabase.h"
#include "SQLQuery.h"
#include "SQLVariant.h"
#include "SQLTransaction.h"
#include <map>

using namespace SQLComponents;

// UNCOMMENT THE NEXT LINE FOR DEBUG BUILDS SAVING THE PASSWORD
#define OQT_SAVE_PASSWORD

class COpenEditorApp : public CWinApp
{
	DECLARE_DYNCREATE(COpenEditorApp)

  int m_dblKeyAccelInx;
  HANDLE m_hMutex;
  HACCEL m_accelTable;
  CMultiDocTemplateExt* m_pEditorDocTemplate;

  void InitGUICommand ();
  void UpdateAccelAndMenu ();
  BOOL AllowThisInstance ();

  bool          OpenDatabaseConnectie();
  void          UpdateDatabaseConnector();
  SQLDatabase&  GetDatabase();
  bool          DatabaseIsOpen();
  bool          DatabaseHasTransaction();
  CString       GetDatasource();
  BOOL          SaveAllModified();
  void        RunShellCommand(char *directory
                             ,char *command
                             ,char *filename 
                             ,char *parameters);

public:
   COpenEditorApp();
  ~COpenEditorApp();

  void    DoFileSaveAll (bool silent, bool skipNew);
  void    OnActivateApp (BOOL bActive);
  void    FindTable     (CString& table);
  void    TableDDL      (CString& p_table);
  CString FindNativeSQL (CString& command);
  void    ReportCapabilities();
  void    OnSessionStatus();
  VarMap&     GetVariables();
  RebindMap*  GetRebinds();
  int     DoMessageBox(LPCTSTR lpszPrompt, UINT nType, UINT nIDPrompt);

	DECLARE_MESSAGE_MAP()
// Overrides
//{{AFX_VIRTUAL(COpenEditorApp)
	public:
	virtual BOOL InitInstance();
	virtual int  ExitInstance();
//  virtual BOOL OnCmdMsg(UINT nID,int nCode,void* pExtra,AFX_CMDHANDLERINFO* pHandlerInfo);
//}}AFX_VIRTUAL

	//{{AFX_MSG(COpenEditorApp)
	afx_msg void OnAppAbout();
	afx_msg void OnEditPermanetSettings();

	//}}AFX_MSG
	afx_msg void OnUpdateEditIndicators (CCmdUI* pCmdUI);
  afx_msg void OnFileWatchNotify (WPARAM, LPARAM);

  afx_msg void OnFileCloseAll();
  afx_msg void OnFileSaveAll();
  afx_msg void OnUpdate_FileSaveAll(CCmdUI *pCmdUI);
  afx_msg void OnDblKeyAccel (UINT nID);
  virtual BOOL PreTranslateMessage(MSG* pMsg);
  virtual BOOL OnIdle(LONG lCount);
  afx_msg void OnCHMHelp();
  afx_msg void OnConnect();
  afx_msg void OnDisconnect();
  afx_msg void OnODBCBegin();
  afx_msg void OnODBCCommit();
  afx_msg void OnODBCRollback();

private:
  CString GetExeDirectory(void);
  BOOL    ParseODBCCommandLine();
  bool    ParseConnectString();
  void    RefreshODBCPanels();

  mutable SQLDatabase*    m_database;
  mutable SQLTransaction* m_transaction;
  mutable CString         m_user;
  mutable CString         m_password;
  mutable CString         m_datasource;
  mutable bool            m_savePassword;
  mutable bool            m_safty;
  mutable bool            m_isClosing;
  mutable VarMap          m_variables;
  mutable RebindMap       m_rebinds;
  mutable CString         m_connectString;
};

extern COpenEditorApp theApp;

inline CString
COpenEditorApp::GetDatasource()
{
  return m_datasource;
}

inline VarMap& 
COpenEditorApp::GetVariables()
{ 
  return m_variables;
};

inline RebindMap*
COpenEditorApp::GetRebinds()
{
  return &m_rebinds;
}


//{{AFX_INSERT_LOCATION}}

#endif//__OpenEditorApp_H__
