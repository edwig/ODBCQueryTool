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
#include "pch.h"
#include "QueryTool.h"
#include "ConnectionDlg.h"
#include "Common/FileSelectDialog.h"
#include "Common/AppGlobal.h"
#include "Version.h"
#include <SQLInfoDB.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const bool Controls2Data = true;   // UpdateData
const bool Data2Controls = false;

ConnectionDlg::ConnectionDlg(CWnd* pParent /*=NULL*/)
	            :StyleDialog(ConnectionDlg::IDD, pParent)
              ,m_database(NULL)
              ,m_connDead(false)
              ,m_readOnly(false)
              ,m_readOnlyApply(false)
              ,m_totalApply(false)
              ,m_tracing(false)
              ,m_tracingApply(false)
              ,m_traceFileApply(false)
              ,m_fileDSNApply(false)
              ,m_loginTimeout(DEFAULT_LOGIN_TIMEOUT)
              ,m_loginTimeoutApply(false)
              ,m_connTimeout(0)
              ,m_connTimeoutApply(false)
              ,m_fileDSNSaveApply(false)
              ,m_packetSize(4096)
              ,m_packetSizeApply(false)
              ,m_metadataID(0)
              ,m_metadataIDApply(false)
              ,m_odbcCursors(SQL_CUR_USE_DRIVER)
              ,m_odbcCursorsApply(false)
              ,m_txnLevel(SQL_TXN_READ_COMMITTED)
              ,m_txnLevelApply(false)
              ,m_transLibApply(false)
              ,m_transOption(0)
              ,m_transOptionApply(false)
              ,m_quietMode(0)
              ,m_autoIPD(false)
              ,m_autoCommit(true)
              ,m_autoCommitApply(false)
              ,m_myConnect(false)
              ,m_optUser(false)
              ,m_optPassword(false)
              ,m_safty(false)
              ,m_moment(0)
{
}

ConnectionDlg::~ConnectionDlg()
{
}

void
ConnectionDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);

  DDX_Control(pDX,IDC_CON_DATASOURCE,   m_comboDatasource);
  DDX_Control(pDX,IDC_CON_USER,         m_editUser,     m_user);
  DDX_Control(pDX,IDC_CON_PASSWORD,     m_editPassword, m_password);
  DDX_Control(pDX,IDC_CON_SAFTY,        m_comboSafty);
  DDX_Control(pDX,IDC_CON_STRING,       m_editConnString,m_connString);
  DDX_Control(pDX,IDC_CON_FILE_DSN,     m_editFileDsn,   m_fileDSN);
  DDX_Control(pDX,IDC_BTN_FILEDSN,      m_buttonFileDsn);
  DDX_Control(pDX,IDC_CON_SAVE_DSN,     m_editFileDsnSave,m_fileDSNSave);
  DDX_Control(pDX,IDC_BTN_SAVEDSN,      m_buttonFileDsnSave);
  DDX_Control(pDX,IDC_MY_CONNECT,       m_checkMyConnect);
  DDX_Control(pDX,IDC_OPT_USER,         m_checkOptUser);
  DDX_Control(pDX,IDC_OPT_PWD,          m_checkPassword);
  DDX_Control(pDX,IDC_TRACE_CONN,       m_checkTraceConn);
  DDX_Control(pDX,IDC_CON_CATALOG,      m_editCatalog,m_catalog);
  DDX_Control(pDX,IDC_CON_TRACEFILE,    m_editTracefile,m_traceFile);
  DDX_Control(pDX,IDC_BTN_LOGFILE,      m_buttonLogfile);
  DDX_Control(pDX,IDC_CON_TRANSLIB,     m_editTransLib,m_transLib);
  DDX_Control(pDX,IDC_BTN_TRANSLIB,     m_buttonTranslib);
  DDX_Control(pDX,IDC_CON_LOGINTIMEOUT, m_editLoginTimeout,m_strLoginTimeout);
  DDX_Control(pDX,IDC_CON_CONNTIMEOUT,  m_editConnTimeout, m_strConnTimeout);
  DDX_Control(pDX,IDC_CON_CURSORLIB,    m_comboCursor);
  DDX_Control(pDX,IDC_CON_TRANSOPTION,  m_editTransOption,m_strTransOption);
  DDX_Control(pDX,IDC_CON_ISOLATION,    m_comboIsolation);
  DDX_Control(pDX,IDC_CON_PACKETSIZE,   m_editPacketSize, m_strPacketSize);
  DDX_Control(pDX,IDC_CON_READONLY,     m_checkReadonly);
  DDX_Control(pDX,IDC_CON_DEAD,         m_checkDeadConnection);
  DDX_Control(pDX,IDC_CON_METADATAID,   m_checkMetadata);
  DDX_Control(pDX,IDC_CON_QUIET,        m_checkQuiet);
  DDX_Control(pDX,IDC_CON_AUTOIPD,      m_checkAutoIPD);
  DDX_Control(pDX,IDC_CON_AUTOCOMMIT,   m_checkAutoCommit);

  DDX_Control(pDX,IDC_CON_HELPEX,       m_buttonHelp);
  DDX_Control(pDX,IDC_APPLY,            m_buttonApply);
  DDX_Control(pDX,IDOK,                 m_buttonOK);
  DDX_Control(pDX,IDCANCEL,             m_buttonCancel);

  if((bool)pDX->m_bSaveAndValidate == Controls2Data)
  {
    SetControls2Data();
  }
  else
  {
    SetData2Controls();
  }
  // Set the apply button
  GetDlgItem(IDC_APPLY)->EnableWindow(m_totalApply && m_database);
}

BEGIN_MESSAGE_MAP(ConnectionDlg, StyleDialog)
    ON_CBN_CLOSEUP (IDC_CON_DATASOURCE,   OnDatasource)
    ON_EN_KILLFOCUS(IDC_CON_USER,         OnFieldUpdate)
    ON_EN_KILLFOCUS(IDC_CON_PASSWORD,     OnFieldUpdate)
    ON_EN_KILLFOCUS(IDC_CON_STRING,       OnFieldUpdate)

    ON_BN_CLICKED  (IDC_OPT_USER,         OnOptUser)
    ON_BN_CLICKED  (IDC_OPT_PWD,          OnOptPassword)
    ON_BN_CLICKED  (IDC_MY_CONNECT,       OnMyConnect)
    ON_BN_CLICKED  (IDC_CON_DEAD,         OnDead)
    ON_BN_CLICKED  (IDC_CON_QUIET,        OnQuiet)
    ON_BN_CLICKED  (IDC_CON_AUTOIPD,      OnAutoIPD)
    ON_BN_CLICKED  (IDC_CON_AUTOCOMMIT,   OnFieldUpdate)
    ON_BN_CLICKED  (IDC_CON_READONLY,     OnFieldUpdate)
    ON_BN_CLICKED  (IDC_CON_METADATAID,   OnFieldUpdate)
    ON_BN_CLICKED  (IDC_APPLY,            OnApplyAfter)
    ON_BN_CLICKED  (IDC_TRACE_CONN,       OnTracing)
    ON_EN_KILLFOCUS(IDC_CON_TRACEFILE,    OnFieldUpdate)
    ON_BN_CLICKED  (IDC_BTN_LOGFILE,      OnButtonTracefile)
    ON_BN_CLICKED  (IDC_BTN_FILEDSN,      OnButtonDSNFile)
    ON_BN_CLICKED  (IDC_BTN_SAVEDSN,      OnButtonDSNSave)
    ON_BN_CLICKED  (IDC_BTN_TRANSLIB,     OnButtonTranslib)
    ON_EN_KILLFOCUS(IDC_CON_FILE_DSN,     OnFieldUpdate)
    ON_EN_KILLFOCUS(IDC_CON_SAVE_DSN,     OnFieldUpdate)
    ON_EN_KILLFOCUS(IDC_CON_LOGINTIMEOUT, OnFieldUpdate)
    ON_EN_KILLFOCUS(IDC_CON_CONNTIMEOUT,  OnFieldUpdate)
    ON_EN_KILLFOCUS(IDC_CON_PACKETSIZE,   OnFieldUpdate)
    ON_CBN_CLOSEUP (IDC_CON_CURSORLIB,    OnFieldUpdate)
    ON_CBN_CLOSEUP (IDC_CON_ISOLATION,    OnFieldUpdate)
    ON_EN_KILLFOCUS(IDC_CON_TRANSLIB,     OnFieldUpdate)
    ON_EN_KILLFOCUS(IDC_CON_TRANSOPTION,  OnFieldUpdate)
END_MESSAGE_MAP()

BOOL 
ConnectionDlg::OnInitDialog()
{
   StyleDialog::OnInitDialog();
   SetWindowText(_T("ODBC Connection"));

   m_comboSafty.AddString(_T("None (development)"));
   m_comboSafty.AddString(_T("Read only (production)"));

   m_comboCursor.AddString(_T("1: Use if needed"));
   m_comboCursor.AddString(_T("2: Use ODBC cursors"));
   m_comboCursor.AddString(_T("3: Use RDBMS"));

   m_comboIsolation.AddString(_T("1: Read uncommitted"));
   m_comboIsolation.AddString(_T("2: Read committed"));
   m_comboIsolation.AddString(_T("3: Repeatable read"));
   m_comboIsolation.AddString(_T("4: Serializable"));

   if(m_moment == 0)
   {
     m_editPassword.SetPassword(true);
     m_editPassword.SetEmpty(true,_T("Password"));
   }

   FillDatasource();

   SetDialogControls();
   UpdateData(FALSE);
   return TRUE;
}

void
ConnectionDlg::SetControls2Data()
{
  CString sitem;
  int number;

  int ind = m_comboDatasource.GetCurSel();
  if (ind >= 0)
  {
    m_comboDatasource.GetLBText(ind, m_datasource);
  }
  m_safty = m_comboSafty.GetCurSel() > 0;
  if(!m_fileDSN.IsEmpty())
  {
    m_totalApply = true;
    m_fileDSNApply = true;
  }
  if(m_tracing != m_checkTraceConn.GetCheck() > 0)
  {
    m_totalApply   = true;
    m_tracingApply = true;
    m_tracing      = m_checkTraceConn.GetCheck();
  }
  if(!m_traceFile.IsEmpty())
  {
    m_totalApply     = true;
    m_traceFileApply = true;
  }
  if(!m_transLib.IsEmpty())
  {
    m_totalApply    = true;
    m_transLibApply = true;
  }
  number = _ttoi(m_strLoginTimeout); 
  if(number >= 0 && number != m_loginTimeout)
  {
    m_totalApply        = true;
    m_loginTimeoutApply = true;
    m_loginTimeout      = number;
  }
  number = _ttoi(m_strConnTimeout);
  if(number >= 0 && number != m_connTimeout)
  {
    m_totalApply       = true;
    m_connTimeoutApply = true;
    m_connTimeout      = number;
  }
  number = _ttoi(m_strTransOption);
  if(number != m_transOption)
  {
    m_totalApply       = true;
    m_transOptionApply = true;
    m_transOption      = number;
  }
  // SQL_TXN_READ_UNCOMMITTED  = 1
  // SQL_TXN_READ_COMMITTED    = 2
  // SQL_TXN_REPEATABLE_READ   = 4
  // SQL_TXN_SERIALIZABLE      = 8
  number = m_comboIsolation.GetCurSel();
  switch(number)
  {
    case 0: number = SQL_TXN_READ_UNCOMMITTED; break;
    case 1: number = SQL_TXN_READ_COMMITTED;   break;
    case 2: number = SQL_TXN_REPEATABLE_READ;  break;
    case 3: number = SQL_TXN_SERIALIZABLE;     break;
  }
  // SQL_CUR_USE_IF_NEEDED = 0 -> First string
  // SQL_CUR_USE_ODBC      = 1 -> Second string
  // SQL_CUR_USE_DRIVER    = 2 -> Third String
  number = m_comboCursor.GetCurSel();
  if(number >= 0 && number <= 2 && number != m_odbcCursors)
  {
    m_totalApply       = true;
    m_odbcCursorsApply = true;
    m_odbcCursors      = number;
  }

  number = _ttoi(m_strPacketSize);
  if(number >= 0 && number != m_packetSize)
  {
    m_totalApply      = true;
    m_packetSizeApply = true;
    m_packetSize      = number;
  }

  // Checks
  number = m_checkReadonly.GetCheck() > 0;
  if((number != 0) != m_readOnly)
  {
    m_totalApply    = true;
    m_readOnlyApply = true;
    m_readOnly      = (number != 0);
  }
  number = m_checkMetadata.GetCheck() > 0;
  if((number!=0) != m_metadataID)
  {
    m_totalApply      = true;
    m_metadataIDApply = true;
    m_metadataID      = (number != 0);
  }
  number = m_checkAutoCommit.GetCheck() > 0;
  if (number != m_autoCommit)
  {
    m_totalApply = true;
    m_autoCommitApply = true;
    m_autoCommit = number;
  }
  number = m_comboIsolation.GetCurSel();
  if(number >= 0 && number != m_txnLevel)
  {
    m_totalApply    = true;
    m_txnLevelApply = true;
    m_txnLevel      = number;
  }

  m_myConnect   = m_checkMyConnect.GetCheck() > 0;
  m_optUser     = m_checkOptUser  .GetCheck() > 0;
  m_optPassword = m_checkPassword .GetCheck() > 0;
}

void
ConnectionDlg::SetData2Controls()
{
  CString sitem;
  int number;
  int ind = m_comboDatasource.FindStringExact(0, m_datasource);
  if (ind < 0)
  {
    ind = m_comboDatasource.AddString(m_datasource);
  }
  m_comboDatasource.SetCurSel(ind);
  m_comboSafty.SetCurSel(m_safty ? 1 : 0);
  m_checkTraceConn.SetCheck(m_tracing);

  m_strLoginTimeout.Format(_T("%d"), m_loginTimeout);
  m_editLoginTimeout.SetWindowText(m_strLoginTimeout);

  m_strConnTimeout.Format(_T("%d"),m_connTimeout);
  m_editConnTimeout.SetWindowText(m_strConnTimeout);

  m_strTransOption.Format(_T("%d"), m_transOption);
  m_editTransOption.SetWindowText(m_strTransOption);

  m_strPacketSize.Format(_T("%d"), m_packetSize);
  m_editPacketSize.SetWindowText(m_strPacketSize);

  // SQL_TXN_READ_UNCOMMITTED  = 1
  // SQL_TXN_READ_COMMITTED    = 2
  // SQL_TXN_REPEATABLE_READ   = 4
  // SQL_TXN_SERIALIZABLE      = 8
  number = 1;
  switch(m_txnLevel)
  {
    case SQL_TXN_READ_UNCOMMITTED: number = 0; break;
    case SQL_TXN_READ_COMMITTED:   number = 1; break;
    case SQL_TXN_REPEATABLE_READ:  number = 2; break;
    case SQL_TXN_SERIALIZABLE:     number = 3; break;
  }
  m_comboIsolation.SetCurSel(number);

  // SQL_CUR_USE_IF_NEEDED = 0 -> First string
  // SQL_CUR_USE_ODBC      = 1 -> Second string
  // SQL_CUR_USE_DRIVER    = 2 -> Third String
  m_comboCursor.SetCurSel(m_odbcCursors);

  if(m_myConnect)
  {
    m_optUser     = true;
    m_optPassword = true;
  }

  // Checks
  m_checkReadonly      .SetCheck(m_readOnly);
  m_checkDeadConnection.SetCheck(m_connDead);
  m_checkMetadata      .SetCheck(m_metadataID);
  m_checkQuiet         .SetCheck(m_quietMode);
  m_checkAutoIPD       .SetCheck(m_autoIPD);
  m_checkAutoCommit    .SetCheck(m_autoCommit);
  m_checkMyConnect     .SetCheck(m_myConnect);
  m_checkOptUser       .SetCheck(m_optUser);
  m_checkPassword      .SetCheck(m_optPassword);
}

void
ConnectionDlg::FillDatasource()
{
  HENV ODBCHandle;
  ::SQLAllocEnv(&ODBCHandle);

  if(ODBCHandle)
  {
    short dsnLen,desLen;
    _TUCHAR DataSourceBuffer[SQL_MAX_DSN_LENGTH + 1];
    _TUCHAR Description[256];
    SQLRETURN res;

    // Initial search
    res = ::SQLDataSources(ODBCHandle
                          ,SQL_FETCH_FIRST
                          ,DataSourceBuffer
                          ,SQL_MAX_DSN_LENGTH
                          ,&dsnLen
                          ,Description
                          ,255
                          ,&desLen);
    while(res == SQL_SUCCESS)
    {
      m_comboDatasource.AddString((const TCHAR*) DataSourceBuffer);
      // For all next datasources
      res = ::SQLDataSources(ODBCHandle
                            ,SQL_FETCH_NEXT
                            ,DataSourceBuffer
                            ,SQL_MAX_DSN_LENGTH
                            ,&dsnLen
                            ,Description
                            ,255
                            ,&desLen);
    }
  }
  ::SQLFreeEnv(ODBCHandle);
}

// Make a connection to the database
bool
ConnectionDlg::Connect()
{
  bool didError = false;
  CString status;
  TCHAR buffer[1024];
  CWaitCursor take_a_deep_sigh;

  CString connectStr;

  if(m_fileDSN.IsEmpty())
  {
    connectStr  = _T("DSN=");
    connectStr += m_datasource;
    status.Format(_T("Trying to connect to: %s as %s"),m_datasource,m_user);
  }
  else
  {
    connectStr  = _T("FILEDSN=");
    connectStr += m_fileDSN;
    status.Format(_T("Trying to connect with FILEDSN=%s as %s"),m_fileDSN,m_user);
  }
  connectStr += _T(";UID=");
  connectStr += m_user;
  connectStr += _T(";PWD=");
  connectStr += m_password;
  // Eventually a save file
  if(!m_fileDSNSave.IsEmpty())
  {
    connectStr += _T(";SAVEFILE=");
    connectStr += m_fileDSNSave;
  }

  try
  {
    Common::SetStatusText(status, TRUE);
    if(m_database->IsOpen())
    {
      m_database->Close();
    }
    OnApplyBefore();
    m_database->Open(connectStr,m_safty);
  }
  catch(CString& error)
  {
    _tcsncpy_s(buffer,error.GetString(),1024);
    didError = true;
  }
  catch (CException* e)
  {
    e->ReportError();
    e->Delete();
    e->GetErrorMessage(buffer,1024);
    didError = true;
  }
  catch (...)
  {
    _tcscpy_s(buffer,1024,_T("No connection with the database"));
    didError = true;
  }
  if(didError)
  {
    TCHAR buffer2[2048];
    _tcscpy_s(buffer2,2048,buffer);

    if(m_password != _T(""))
    {
      m_password = _T("********");
    }
    _tcscat_s(buffer2,2048,_T("\r\nDatasource = \""));
    _tcscat_s(buffer2,2048,m_datasource);
    _tcscat_s(buffer2,2048,_T("\" User = \""));
    _tcscat_s(buffer2,2048,m_user);
    _tcscat_s(buffer2,2048,_T("\" Password = \""));
    _tcscat_s(buffer2,2048,m_password);
    AfxMessageBox(buffer2);
    Common::SetStatusText(_T(""), TRUE);
  }
  else
  {
    // Apply connection attributes after connect
    OnApplyAfter();
  }
  return !didError;
}

void
ConnectionDlg::OnDatasource()
{
  if(m_comboDatasource.IsWindowEnabled())
  {
    int ind = m_comboDatasource.GetCurSel();
    if(ind >= 0)
    {
      m_comboDatasource.GetLBText(ind,m_datasource);
    }
    UpdateData(Data2Controls);
  }
  else
  {
    int ind = m_comboDatasource.FindStringExact(0,m_datasource);
    if(ind < 0)
    {
      ind = m_comboDatasource.AddString(m_datasource);
    }
    m_comboDatasource.SetCurSel(ind);
  }
}

void
ConnectionDlg::OnMyConnect()
{
  m_myConnect = m_checkMyConnect.GetCheck() > 0;
  UpdateData(Data2Controls);
}

void 
ConnectionDlg::OnOptUser()
{
  m_optUser = m_checkOptUser.GetCheck() > 0;
  UpdateData(Data2Controls);
}

void 
ConnectionDlg::OnOptPassword()
{
  m_optPassword = m_checkPassword.GetCheck() > 0;
  UpdateData(Data2Controls);
}

void
ConnectionDlg::OnFieldUpdate()
{
  UpdateData(Controls2Data);
}

void
ConnectionDlg::OnDead()
{
  // Nothing to do to be dead
  if(m_database->IsOpen())
  {
    m_connDead = m_database->GetSQLInfoDB()->GetConnectionDead();
  }
  else
  {
    m_connDead = false;
  }
  UpdateData(Data2Controls);
}

void
ConnectionDlg::OnQuiet()
{
  if(m_database->IsOpen())
  {
    m_quietMode = m_database->GetSQLInfoDB()->GetAttributeQuiet();
  }
  else
  {
    m_quietMode = false;
  }
  UpdateData(Data2Controls);
}

void
ConnectionDlg::OnAutoIPD()
{
  if(m_database->IsOpen())
  {
    if(m_database->GetSQLInfoDB()->GetAttributeAutoIPD() == -1)
    {
      m_autoIPD = false;
    }
  }
  else
  {
    m_autoIPD = false;
  }
  UpdateData(Data2Controls);
}

void
ConnectionDlg::OnCancel()
{
  EndDialog(IDC_CON_SIMPLE);
}

void
ConnectionDlg::OnTracing()
{
  UpdateData(Controls2Data);
}

void
ConnectionDlg::OnButtonTracefile()
{
  FileSelectDialog diag(true
                       ,_T("File for ODBC trace logging")
                       ,_T("log")
                       ,_T("C:\\*.*")
                       ,0
                       ,_T("Logfiles|*.log|Text files|*.txt|All files|*.*"));
  if(diag.DoModal() == IDOK)
  {
    if(m_traceFile.CompareNoCase(diag.GetChosenFile()))
    {
      // Different trace file chosen
      m_traceFile      = diag.GetChosenFile();
      m_traceFileApply = true;
      UpdateData(Data2Controls);
    }
  }
}

void
ConnectionDlg::OnButtonDSNFile()
{
  CString directory = GetDSNDirectory();

  FileSelectDialog diag(true // For open
                       ,_T("DatasourceName file (DSN) for login")
                       ,_T("dsn")
                       ,directory
                       ,0
                       ,_T("ODBC Datasources|*.dsn|All files|*.*"));
  if(diag.DoModal() == IDOK)
  {
    if(m_fileDSN.CompareNoCase(diag.GetChosenFile()))
    {
      // Different DSN file chosen
      m_fileDSN      = diag.GetChosenFile();
      m_fileDSNApply = true;
      m_datasource   = _T("");
      UpdateData(Data2Controls);
      SetDialogControls();
    }
  }
}

void
ConnectionDlg::OnButtonDSNSave()
{
  CString directory = GetDSNDirectory();

  FileSelectDialog diag(true // For open
                       ,_T("DatasourceName file (DSN) for saving successful login")
                       ,_T("dsn")
                       ,directory
                       ,0
                       ,_T("ODBC Datasources|*.dsn|All files|*.*"));
  if(diag.DoModal() == IDOK)
  {
    if(m_fileDSNSave.CompareNoCase(diag.GetChosenFile()))
    {
      // Different DSN file chosen
      m_fileDSNSave      = diag.GetChosenFile();
      m_fileDSNSaveApply = true;
      UpdateData(Data2Controls);
    }
  }
}

void
ConnectionDlg::OnButtonTranslib()
{
  CString directory;
  FileSelectDialog diag(true // For open
                       ,_T("Translation library (*.DLL) for datasource")
                       ,_T("dll")
                       ,directory
                       ,0
                       ,_T("Libraries|*.dll|All files|*.*"));
  if(diag.DoModal() == IDOK)
  {
    if(m_transLib.CompareNoCase(diag.GetChosenFile()))
    {
      // Different DSN file chosen
      m_transLib      = diag.GetChosenFile();
      m_transLibApply = true;
      UpdateData(Data2Controls);
    }
  }
}

void 
ConnectionDlg::SetLogin(bool    p_moment
                       ,CString p_user
                       ,CString p_password
                       ,CString p_datasource
                       ,bool    p_safty
                       ,CString p_connString
                       ,bool    p_useConnString
                       ,bool    p_optionalUser
                       ,bool    p_optionalPassword)
{
  m_moment      = p_moment;
  m_user        = p_user;
  m_password    = p_password;
  m_datasource  = p_datasource;
  m_safty       = p_safty;
  m_connString  = p_connString;
  m_myConnect   = p_useConnString;
  m_optUser     = p_optionalUser;
  m_optPassword = p_optionalPassword;
}

bool
ConnectionDlg::SetDataConnector(SQLDatabase* database,bool p_open /*=true*/)
{
  m_database  = database;

  if(!m_database->IsOpen() && p_open)
  {
    CString options;
    m_database->Open(m_datasource,m_user,m_password,options,m_safty);
    if(!m_database->IsOpen())
    {
      return false;
    }
  }
  // In case we come from the toolbar in an opened database
  if(m_database->IsOpen())
  {
    GetConnectionAttributes();
  }
  return true;
}

// Set controls read-only read-write
void
ConnectionDlg::SetDialogControls()
{
  m_editUser            .EnableWindow(m_moment == 0);
  m_editPassword        .EnableWindow(m_moment == 0);
  m_comboDatasource     .EnableWindow(m_moment == 0 && m_fileDSN.IsEmpty());
  m_comboSafty          .EnableWindow(m_moment == 0 && m_fileDSN.IsEmpty());
  m_editConnString      .EnableWindow(m_moment == 0 && m_fileDSN.IsEmpty());
  m_editFileDsn         .EnableWindow(m_moment == 0);
  m_editFileDsnSave     .EnableWindow(m_moment == 0);
  m_checkTraceConn      .EnableWindow(true);
  m_editTracefile       .EnableWindow(true);
  m_editTransLib        .EnableWindow(true);
  m_editTransOption     .EnableWindow(true);
  m_editLoginTimeout    .EnableWindow(m_moment == 0);
  m_editConnTimeout     .EnableWindow(true);
  m_comboCursor         .EnableWindow(m_moment == 0);
  m_comboIsolation      .EnableWindow(true);
  m_editPacketSize      .EnableWindow(m_moment == 0);
  m_editCatalog         .ModifyStyle(0,ES_READONLY);
  m_checkReadonly       .EnableWindow(m_moment != 0);
  m_checkDeadConnection .EnableWindow(true);
  m_checkQuiet          .EnableWindow(true);
  m_checkMetadata       .EnableWindow(true);
  m_checkAutoIPD        .EnableWindow(true);
  m_checkAutoCommit     .EnableWindow(true);

  m_checkMyConnect      .EnableWindow(m_moment == 0);
  m_checkOptUser        .EnableWindow(m_moment == 0);
  m_checkPassword       .EnableWindow(m_moment == 0);
  // Buttons
  m_buttonApply         .EnableWindow(false);
  m_buttonFileDsn       .EnableWindow(m_moment == 0);
  m_buttonFileDsnSave   .EnableWindow(m_moment == 0);
  m_buttonTranslib      .EnableWindow(true);
}

void
ConnectionDlg::GetConnectionAttributes()
{
  if(m_database)
  {
    m_readOnly     = m_database->GetReadOnly();
    m_loginTimeout = m_database->GetLoginTimeout();
    m_connDead     = m_database->GetSQLInfoDB()->GetConnectionDead();
    m_tracing      = m_database->GetSQLInfoDB()->GetAttributeTracing();
    m_traceFile    = m_database->GetSQLInfoDB()->GetAttributeTraceFile();
    m_catalog      = m_database->GetSQLInfoDB()->GetAttributeCatalog();
    m_fileDSN      = m_database->GetSQLInfoDB()->GetAttributeFileDSN();
    m_fileDSNSave  = m_database->GetSQLInfoDB()->GetAttributeFileDSNSave();
    m_connTimeout  = m_database->GetSQLInfoDB()->GetAttributeConnTimeout();
    m_packetSize   = m_database->GetSQLInfoDB()->GetAttributePacketSize();
    m_metadataID   = m_database->GetSQLInfoDB()->GetAttributeMetadataID();
    m_txnLevel     = m_database->GetSQLInfoDB()->GetAttributeTransLevel();
    m_transLib     = m_database->GetSQLInfoDB()->GetAttributeTranslib();
    m_transOption  = m_database->GetSQLInfoDB()->GetAttributeTransoption();
    m_quietMode    = m_database->GetSQLInfoDB()->GetAttributeQuiet();
    m_autoCommit   = m_database->GetTransaction() ? false : true;
    if(m_tracing && m_traceFile.IsEmpty())
    {
      m_traceFile = _T("<Not set: search for SQL.LOG>");
    }
  }
}

// Apply attributes before a connection is made
void
ConnectionDlg::OnApplyBefore()
{
  if(m_loginTimeoutApply && m_database->IsOpen())
  {
    m_database->SetLoginTimeout(m_loginTimeout);
    m_loginTimeoutApply = false;
  }
  if(m_readOnlyApply && m_database->IsOpen())
  {
    m_database->SetReadOnly(m_readOnly);
    m_readOnlyApply = false;
  }
  if(m_fileDSNApply && m_database->IsOpen())
  {
    m_database->GetSQLInfoDB()->SetAttributeFileDSN(m_fileDSN);
    m_fileDSNApply = false;
  }
  if(m_fileDSNSaveApply && m_database->IsOpen())
  {
    m_database->GetSQLInfoDB()->SetAttributeFileDSNSave(m_fileDSNSave);
    m_fileDSNSaveApply = false;
  }
  if(m_odbcCursorsApply && m_database->IsOpen())
  {
    m_database->GetSQLInfoDB()->SetAttributeOdbcCursors(m_odbcCursors);
    m_odbcCursorsApply = false;
  }
  if(m_packetSizeApply && m_database->IsOpen())
  {
    if(!m_database->GetSQLInfoDB()->SetAttributePacketSize(m_packetSize))
    {
      m_packetSize = 0;
    }
    m_packetSizeApply = false;
  }
}

// Apply attributes after a connect is made
void
ConnectionDlg::OnApplyAfter()
{
  if(m_readOnlyApply)
  {
    if(!m_database->SetReadOnly(m_readOnly))
    {
      // Could not apply on the datasource
      m_readOnly = !m_readOnly;
    }
    m_readOnlyApply = false;
  }
  if(m_traceFileApply)
  {
    if(( m_tracing && !m_traceFile.IsEmpty()) ||
       (!m_tracing &&  m_traceFile.IsEmpty()) )
    {
      if(!m_database->GetSQLInfoDB()->SetAttributeTraceFile(m_traceFile))
      {
        m_traceFile = _T("");
      }
    }
    m_traceFileApply = false;
  }
  if(m_tracingApply)
  {
    if(m_tracing && m_traceFile.IsEmpty())
    {
      m_traceFile = _T("<Not set: search for SQL.LOG>");
    }
    if(!m_tracing && m_traceFile.GetAt(0) == '<')
    {
      m_traceFile = _T("");
    }
    if(!m_database->GetSQLInfoDB()->SetAttributeTracing(m_tracing))
    {
      m_tracing = !m_tracing;
    }
    m_tracingApply = false;
  }
  if(m_connTimeoutApply)
  {
    if(!m_database->GetSQLInfoDB()->SetAttributeConnTimeout(m_connTimeout))
    {
      m_connTimeout = 0;
    }
    m_connTimeoutApply = false;
  }
  if(m_metadataIDApply)
  {
    if(!m_database->GetSQLInfoDB()->SetAttributeMetadataID(m_metadataID))
    {
      m_metadataID = false;
    }
    m_metadataIDApply = false;
  }
  if(m_txnLevelApply)
  {
    m_database->GetSQLInfoDB()->SetAttributeTransLevel(m_txnLevel);
    m_txnLevelApply = false;
  }
  if(m_transLibApply)
  {
    if(!m_database->GetSQLInfoDB()->SetAttributeTranslib(m_transLib))
    {
      m_transLib = _T("");
    }
    m_transLibApply = false;
  }
  if(m_transOptionApply)
  {
    if(!m_database->GetSQLInfoDB()->SetAttributeTransoption(m_transOption))
    {
      m_transOption = 0;
    }
    m_transOptionApply = false;
  }
  if(m_autoCommitApply)
  {
    if(m_autoCommit)
    {
      theApp.OnODBCCommit();
    }
    else
    {
      theApp.OnODBCBegin();
    }
    m_autoCommitApply = false;
  }
  m_totalApply = false;
  UpdateData(Data2Controls);  
}

// Get the directory for DSN files from the registry
CString
ConnectionDlg::GetDSNDirectory()
{
  CString directory;
  HKEY where = HKEY_CURRENT_USER;
  HKEY hKey;
  for(int i = 0; i < 2; ++i)
  {
    if (RegOpenKeyEx(where,_T("Software\\ODBC\\ODBC.INI\\ODBC File DSN")
      ,0,KEY_QUERY_VALUE,&hKey) == ERROR_SUCCESS)
    {
      TCHAR szData[128];
      DWORD dwKeyDataType;
      DWORD dwDataBufSize = sizeof(szData);

      if (RegQueryValueEx(hKey
                        ,_T("DefaultDSNDir")
                        ,NULL
                        ,&dwKeyDataType
                        ,(LPBYTE)&szData
                        ,&dwDataBufSize) == ERROR_SUCCESS)
      {
        directory = szData;
      }
      RegCloseKey(hKey);
    }
    if(!directory.IsEmpty()) break;
    where = HKEY_LOCAL_MACHINE;
  }
  if(!directory.IsEmpty())
  {
    directory += _T("\\*.dsn");
  }
  return directory;
}

bool
ConnectionDlg::CheckInput()
{
  CString errors;

  if(m_datasource.IsEmpty())
  {
    errors = _T("You MUST supply a datasource name for the connection!");
  }
  if(m_myConnect && m_connString.IsEmpty())
  {
    if(!errors.IsEmpty()) errors += _T("\r\n");
    errors += _T("You MUST supply a connection string as you have selected so!");
  }
  if(!m_optUser && m_user.IsEmpty())
  {
    if(!errors.IsEmpty()) errors += _T("\r\n");
    errors += _T("You MUST supply a user name as you have NOT selected to leave one out!");
  }
  if(!m_optPassword && m_password.IsEmpty())
  {
    if(!errors.IsEmpty()) errors += _T("\r\n");
    errors += _T("You MUST supply a password as you have NOT selected to leave one out!");
  }
  if(!errors.IsEmpty())
  {
    StyleMessageBox(this,errors,PROGRAM_NAME,MB_OK|MB_ICONERROR);
    return false;
  }
  return true;
}

void
ConnectionDlg::OnOK()
{
  UpdateData(Controls2Data);
  if(m_moment)
  {
    // Dialog used on living connection
    if(m_totalApply && m_database)
    {
      OnApplyAfter();
    }
  }
  else if(!CheckInput())
  {
    return;
  }
  StyleDialog::OnOK();
}

