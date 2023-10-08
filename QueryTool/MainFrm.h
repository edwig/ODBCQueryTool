////////////////////////////////////////////////////////////////////////
//
// File: MainFrame.h
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
#include "ObjectView.h"
#include "InfoView.h"
#include "DirTreeView.h"
#include "OutputWnd.h"
#include "ConnectionBar.h"
#include "OESettings.h"

#define TOOLBAR_ICON_X   19
#define TOOLBAR_ICON_Y   19
#define TOOLBAR_BUTTON_X 28
#define TOOLBAR_BUTTON_Y 28
#define TOOLBAR_HEIGHT   35

#define ODBC_STATUSBAR_HEIGHT      20 

class CMainFrame : public StyleMDIFrameWnd //CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame() noexcept;
  virtual ~CMainFrame();

// Attributes
public:
  static LPCTSTR m_cszClassName;
  static UINT    m_nOpenFileMsg;

  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = nullptr, CCreateContext* pContext = nullptr);
  void         SetConnectionText(CString& text);
  void         ReportCapabilities(CString& filename);
  void         FindTable(CString& table);
  void         OnRefreshOdbc();
  void         SendSupportEmail(CString p_subject,CString p_body);

#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCToolBar				m_dbToolBar;
	ConnectionBar			m_conToolBar;

	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	ObjectView        m_wndObjectView;
	InfoView  				m_wndInfoView;
  DirTreeView       m_wndDirView;
	COutputWnd        m_wndOutput;

	int m_connect{ 0 };

// Generated message map functions
protected:
  LRESULT DefWindowProc(UINT message,WPARAM wParam,LPARAM lParam) override;


	afx_msg int			OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void    OnTimer(UINT_PTR nIDEvent);
	afx_msg void		OnWindowManager();
	afx_msg void		OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void		OnApplicationLook(UINT id);
	afx_msg void		OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void		OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg LRESULT OnToolbarReset(WPARAM wp, LPARAM);
	afx_msg BOOL		OnCopyData(CWnd*, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void		OnActivateApp(BOOL bActive, DWORD dwThreadID);
  afx_msg void    OnConnectBar();
  afx_msg void    OnDatabaseBar();
  afx_msg void    OnSQLMigration();
  afx_msg void    OnSQLReWriter();
  afx_msg void    OnSupportBug();
  afx_msg void    OnSupportFeature();
  afx_msg void    OnWebSourceforge();
  afx_msg void    OnWebGithub();
  afx_msg void    OnDonation();
  afx_msg void    OnFindFiles();
  afx_msg void    OnViewFindPanel();
  afx_msg void    OnViewFilePanelSync();
  afx_msg void    OnUpdateConnect(CCmdUI* pCmdUI);
  afx_msg void    OnUpdateDisconnect(CCmdUI* pCmdUI);
  afx_msg void    OnUpdateConnectBarVisible(CCmdUI* pCmdUI);
  afx_msg void    OnUpdateDatabaseBarVisible(CCmdUI* pCmdUI);
  afx_msg void    OnUpdateBegin(CCmdUI* pCmdUI);
  afx_msg void    OnUpdateCommit(CCmdUI* pCmdUI);
  afx_msg void    OnUpdateRollback(CCmdUI* pCmdUI);
  afx_msg void    OnUpdateODBCReport(CCmdUI* pCmdUI);
  afx_msg void    OnUpdateSessionStatus(CCmdUI* pCmdUI);
  afx_msg void    OnUpdateViewFindPanel(CCmdUI* pCmdUI);
  afx_msg void    OnUpdateViewFilePanelSync(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);

private:
  static OpenEditor::SettingsManager m_settingsManager;
  void LoadSettingsManager();
  void SaveSettingsManager();

  bool m_conBarVisible;
  bool m_dbBarVisible;
};


