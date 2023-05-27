/*
    Copyright (C) 2004-2023 Edwig Huisman

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
#include <StyleFramework.h>
#include <SQLDatabase.h>

using namespace SQLComponents;

// Timeout and net wait defaults
#define DEFAULT_LOGIN_TIMEOUT 15    // seconds to before fail on connect
#define DEFAULT_QUERY_TIMEOUT 15    // seconds to before fail waiting for results

class ConnectionDlg : public StyleDialog
{
  // Construction
public:
	 ConnectionDlg(CWnd* pParent = NULL);   // standard constructor
   virtual ~ConnectionDlg();      // Standard destructor

  // Dialog Data
	enum { IDD = IDD_CONNECT };

  void SetLogin(bool    p_moment
               ,CString p_user
               ,CString p_password
               ,CString p_datasource
               ,bool    p_safty
               ,CString p_connString
               ,bool    p_useConnString
               ,bool    p_optionalUser
               ,bool    p_optionalPassword);
  bool    SetDataConnector(SQLDatabase* database,bool p_open = true);

  CString GetUser()             { return m_user;        }
  CString GetPassword()         { return m_password;    }
  CString GetDataSource()       { return m_datasource;  }
  bool    GetSafty()            { return m_safty;       }
  CString GetConnString()       { return m_connString;  }
  bool    GetUseConnString()    { return m_myConnect;   }
  bool    GetOptionalUser()     { return m_optUser;     }
  bool    GetOptionalPassword() { return m_optPassword; }

  // Overrides
	// ClassWizard generated virtual function override
protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;

  afx_msg void OnOK();
  afx_msg void OnCancel();
  afx_msg void OnDead();
  afx_msg void OnQuiet();
  afx_msg void OnAutoIPD();
  afx_msg void OnDatasource();
  afx_msg void OnFieldUpdate();
  afx_msg void OnApplyBefore();
  afx_msg void OnApplyAfter();
  afx_msg void OnTracing();
  afx_msg void OnButtonTracefile();
  afx_msg void OnButtonDSNFile();
  afx_msg void OnButtonDSNSave();
  afx_msg void OnButtonTranslib();
  afx_msg void OnMyConnect();
  afx_msg void OnOptUser();
  afx_msg void OnOptPassword();

  // Implementation
protected:

  // To be done after a screen update
  void    SetControls2Data();
  void    SetData2Controls();
  // Make a connection
  bool    Connect();
  // Set controls read-only read-write
  void    SetDialogControls();
  void    GetConnectionAttributes();
  // Get the directory for DSN files from the registry
  CString GetDSNDirectory();
  void    FillDatasource();
  // Check the input for correctness
  bool    CheckInput();

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
  CString   m_connString;
  CString   m_catalog;          // Static attribute
  bool      m_myConnect;
  bool      m_optUser;
  bool      m_optPassword;

  StyleComboBox m_comboDatasource;
  StyleEdit     m_editUser;
  StyleEdit     m_editPassword;
  StyleComboBox m_comboSafty;
  StyleEdit     m_editConnString;
  StyleEdit     m_editFileDsn;
  StyleButton   m_buttonFileDsn;
  StyleEdit     m_editFileDsnSave;
  StyleButton   m_buttonFileDsnSave;
  StyleCheckbox m_checkMyConnect;
  StyleCheckbox m_checkOptUser;
  StyleCheckbox m_checkPassword;
  StyleCheckbox m_checkTraceConn;
  StyleEdit     m_editCatalog;
  StyleEdit     m_editTracefile;
  StyleButton   m_buttonLogfile;
  StyleEdit     m_editTransLib;
  StyleButton   m_buttonTranslib;
  StyleEdit     m_editLoginTimeout;
  StyleEdit     m_editConnTimeout;
  StyleComboBox m_comboCursor;
  StyleEdit     m_editTransOption;
  StyleComboBox m_comboIsolation;
  StyleEdit     m_editPacketSize;
  StyleCheckbox m_checkReadonly;
  StyleCheckbox m_checkDeadConnection;
  StyleCheckbox m_checkMetadata;
  StyleCheckbox m_checkQuiet;
  StyleCheckbox m_checkAutoIPD;
  StyleCheckbox m_checkAutoCommit;
  StyleButton   m_buttonSimple;
  StyleButton   m_buttonHelp;
  StyleButton   m_buttonApply;
  StyleButton   m_buttonOK;
  StyleButton   m_buttonCancel;

  // Connection status
  bool    m_readOnly;         // Read-only status
  bool    m_readOnlyApply;    // Must re-apply it
  bool    m_connDead;         // Connection is dead (not Updatable)
  bool    m_tracing;          // Do tracing
  bool    m_tracingApply;     // Must reapply it
  CString m_traceFile;        // File to trace to
  bool    m_traceFileApply;   // Reapply tracing file
  CString m_fileDSN;          // Login file DSN
  bool    m_fileDSNApply;
  CString m_fileDSNSave;      // Saved DSN file after success
  bool    m_fileDSNSaveApply; // Reapply
  CString m_strLoginTimeout;
  int     m_loginTimeout;     // Timeout at login (default = 15)
  bool    m_loginTimeoutApply;// Reapply
  CString m_strConnTimeout;
  int     m_connTimeout;      // Timeout at connection level (default = 0)
  bool    m_connTimeoutApply; // Reapply
  CString m_strPacketSize;
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
  CString m_strTransOption;
  int     m_transOption;      // Option at start of translation library
  bool    m_transOptionApply; // Reapply
  int     m_quietMode;        // WIndow handle
  bool    m_autoIPD;          // Auto IPD population
  int     m_autoCommit;
  bool    m_autoCommitApply;

  // Total of all Applies
  bool    m_totalApply;

  // Connect 
  SQLDatabase* m_database;
};
