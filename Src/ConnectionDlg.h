/*
    Copyright (C) 2004 Edwig Huisman

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

// Timeout and net wait defaults
#define DEFAULT_LOGIN_TIMEOUT 15    // seconds to before fail on connect
#define DEFAULT_QUERY_TIMEOUT 15    // seconds to before fail waiting for results

class ConnectionDlg : public CDialog
{
// Construction
public:
	 ConnectionDlg(CWnd* pParent = NULL);   // standard constructor
  ~ConnectionDlg();      // Standard destructor

  // Dialog Data
	enum { IDD = IDD_CONNECT };

  void SetLogin(bool    moment
               ,CString user
               ,CString password
               ,CString datasource
               ,bool    safty);
  void    SetDataConnector(SQLDatabase** database);
  CString GetUser()       { return m_user;       };
  CString GetPassword()   { return m_password;   };
  CString GetDataSource() { return m_datasource; };
  bool    GetSafty()      { return m_safty;      };

  // Overrides
	// ClassWizard generated virtual function override
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  virtual void OnSimple();
  virtual void OnDead();
  virtual void OnQuiet();
  virtual void OnAutoIPD();
  virtual void OnFieldUpdate();
  virtual void OnApplyBefore();
  virtual void OnApplyAfter();
  virtual void OnTracing();
  virtual void OnButtonTracefile();
  virtual void OnButtonDSNFile();
  virtual void OnButtonDSNSave();
  virtual void OnButtonTranslib();

  // Implementation
protected:

  // Make a connection
  bool Connect();
  // Set controls read-only read-write
  void SetDialogControls();
  void GetConnectionAttributes();
  // Get the directory for DSN files from the registry
  CString GetDSNDirectory();

  // Generated message map functions
	//{{AFX_MSG(MultConnectionsDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
  int     m_moment;     // Pre-logon / during connection

  CString   m_user;
  CString   m_password;
  CString   m_datasource;
  bool      m_safty;
  CString   m_catalog;          // Static attribute
  CComboBox m_boxSafty;

  // Connection status
  bool    m_readOnly;         // Read-only status
  bool    m_readOnlyApply;    // Must re-apply it
  bool    m_connDead;         // Connection is dead (not updatable)
  bool    m_tracing;          // Do tracing
  bool    m_tracingApply;     // Must reapply it
  CString m_traceFile;        // File to trace to
  bool    m_traceFileApply;   // Reapply tracing file
  CString m_fileDSN;          // Login file DSN
  bool    m_fileDSNApply;
  CString m_fileDSNSave;      // Saved DSN file after success
  bool    m_fileDSNSaveApply; // Reapply
  int     m_loginTimeout;     // Timout at login (default = 15)
  bool    m_loginTimeoutApply;// Reapply
  int     m_connTimeout;      // Timeout at connection level (default = 0)
  bool    m_connTimeoutApply; // Reapply
  int     m_packetSize;       // Network packet size
  bool    m_packetSizeApply;  // Reapply
  bool    m_metadataID;       // METADATA-ID
  bool    m_metadataIDApply;  // Reapply
  int     m_odbcCursors;      // ODBC cursor library
  int     m_odbcCursorsApply; // Reapply
  int     m_txnLevel;         // Transaction isolation level
  bool    m_txnLevelApply;    // Reapply
  CString m_transLib;         // Translation library
  bool    m_transLibApply;    // Reapply
  int     m_transOption;      // Option at start of translation library
  bool    m_transOptionApply; // Reapply
  int     m_quietMode;        // WIndow handle
  bool    m_autoIPD;          // Auto IPD population
  int     m_autoCommit;
  bool    m_autoCommitApply;

  // Total of all Applies
  bool    m_totalApply;

  // Connect 
  SQLDatabase** m_database;
};
