////////////////////////////////////////////////////////////////////////
//
// File: QueryTool.h
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of 
// this software and associated documentation files (the "Software"), 
// to deal in the Software without restriction, including without limitation the rights 
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies 
// or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION 
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include <SQLDatabase.h>
#include <SQLTransaction.h>
#include <SQLQuery.h>

using namespace SQLComponents;

// QueryToolApp:
// See QueryTool.cpp for the implementation of this class
//

class QueryToolApp : public CWinAppEx
{
public:
	QueryToolApp() noexcept;
  virtual ~QueryToolApp();

// Overrides
public:
	virtual BOOL  InitInstance() override;
	virtual int   ExitInstance() override;
  virtual BOOL  SaveAllModified() override;
  virtual int   DoMessageBox(LPCTSTR lpszPrompt,UINT nType,UINT nIDPrompt) override;

  void          DoFileSaveAll(bool silent, bool skipNew);
  BOOL          AllowThisInstance();
  bool          OpenDatabaseConnectie();
  void          UpdateDatabaseConnector();
  SQLDatabase&  GetDatabase();
  bool          DatabaseIsOpen();
  bool          DatabaseHasTransaction();
  CString       GetDatasource();
  void          RunShellCommand(LPCTSTR directory,LPCTSTR command,LPCTSTR filename,LPCTSTR parameters);
  VarMap&       GetVariables();
  RebindMap*    GetRebinds();
  void          SelectFirst100(CString& p_table);
  void          FindTable(CString& table);
  void          TableDDL(CString& p_table);
  CString       FindNativeSQL(CString& command);
  void          ReportCapabilities();
  void          SetTitle(CString p_document);

  // Implementation
	UINT    m_nAppLook;
	BOOL    m_bHiColorIcons;
  int     m_dblKeyAccelInx;
  HACCEL  m_accelTable;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

  afx_msg void OnAppAbout();
  afx_msg void OnEditPermanetSettings();
  afx_msg void OnCHMHelp();
  afx_msg void OnConnect();
  afx_msg void OnDisconnect();
  afx_msg void OnODBCBegin();
  afx_msg void OnODBCCommit();
  afx_msg void OnODBCRollback();
  afx_msg void OnUpdateEditIndicators(CCmdUI* pCmdUI);
  afx_msg void OnFileWatchNotify(WPARAM, LPARAM);
  afx_msg void OnFileCloseAll();
  afx_msg void OnFileSaveAll();
  afx_msg void OnUpdate_FileSaveAll(CCmdUI* pCmdUI);
  afx_msg void OnActivateApp(BOOL bActive);
  afx_msg void OnDblKeyAccel(UINT nID);
  afx_msg void OnSessionStatus();

	DECLARE_MESSAGE_MAP()

private:
  virtual BOOL PreTranslateMessage(MSG* pMsg) override;
  virtual BOOL OnIdle(LONG lCount);

  BOOL    ParseODBCCommandLine();
  void    InitGUICommand();
  void    UpdateAccelAndMenu();
  void    RefreshODBCPanels();
  CString GetExeDirectory(void);
  bool    AppStarted25Times();

  mutable SQLDatabase     m_database;
  mutable SQLTransaction* m_transaction;
  mutable CString         m_user;
  mutable CString         m_password;
  mutable CString         m_datasource;
  mutable CString         m_connString;
  mutable bool            m_useConnString;
  mutable bool            m_optionalUser;
  mutable bool            m_optionalPassword;
  mutable bool            m_savePassword;
  mutable bool            m_safty;
  mutable bool            m_isClosing;
  mutable VarMap          m_variables;
  mutable RebindMap       m_rebinds;

  HANDLE m_hMutex;
};

inline bool
QueryToolApp::DatabaseIsOpen()
{
  return m_database.IsOpen();
}

inline CString
QueryToolApp::GetDatasource()
{
  return m_datasource;
}

inline VarMap&
QueryToolApp::GetVariables()
{
  return m_variables;
};

inline RebindMap*
QueryToolApp::GetRebinds()
{
  return &m_rebinds;
}

extern QueryToolApp theApp;
