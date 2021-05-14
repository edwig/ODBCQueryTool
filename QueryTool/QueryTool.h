
// QueryTool.h : main header file for the QueryTool application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include <SQLDatabase.h>
#include <SQLTransaction.h>
#include <SQLQuery.h>

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

  void          DoFileSaveAll(bool silent, bool skipNew);
  BOOL          AllowThisInstance();
  bool          OpenDatabaseConnectie();
  void          UpdateDatabaseConnector();
  SQLDatabase&  GetDatabase();
  bool          DatabaseIsOpen();
  bool          DatabaseHasTransaction();
  CString       GetDatasource();
  void          RunShellCommand(const char* directory,const char* command,const char* filename,const char* parameters);
  VarMap&       GetVariables();
  RebindMap*    GetRebinds();
  void          SelectFirst100(CString& p_table);
  void          FindTable(CString& table);
  void          TableDDL(CString& p_table);
  CString       FindNativeSQL(CString& command);
  void          ReportCapabilities();
  void          SetTitle(CString p_document);

  // General message box
  int           DoMessageBox(LPCTSTR lpszPrompt, UINT nType, UINT nIDPrompt);

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
  bool    ParseConnectString();

  mutable SQLDatabase     m_database;
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
