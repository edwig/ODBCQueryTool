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
#include "stdafx.h"
#include "OpenEditorApp.h"
#include "ConnectionDlg.h"
#include "Common/FileSelectDialog.h"
#include "Database\SQLDatabase.h"
#include "Database\SQLInfoDB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const bool Controls2Data = true;   // UpdateData
const bool Data2Controls = false;

ConnectionDlg::ConnectionDlg(CWnd* pParent /*=NULL*/)
	            :CDialog(ConnectionDlg::IDD, pParent)
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
{
}

ConnectionDlg::~ConnectionDlg()
{
}

void ConnectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

  CString sitem;
  int number;
  if(pDX->m_bSaveAndValidate == (BOOL)Controls2Data)
  {
    DDX_Text   (pDX,IDC_CON_USER,sitem);       m_user       = sitem;
    DDX_Text   (pDX,IDC_CON_PASSWORD,sitem);   m_password   = sitem;
    DDX_Text   (pDX,IDC_CON_DATASOURCE,sitem); m_datasource = sitem;
    DDX_Control(pDX,IDC_CON_SAFTY,m_boxSafty); 
    m_safty = m_boxSafty.GetCurSel() > 0;
//  m_safty = (sitem == "Read only (production)") ? true : false;
    // Read-only
    DDX_Check(pDX,IDC_CON_READONLY,number);
    if((number != 0) != m_readOnly)
    {
      m_totalApply    = true;
      m_readOnlyApply = true;
      m_readOnly      = (number != 0);
    }
    DDX_Check(pDX,IDC_CON_METADATAID,number);
    if((number!=0) != m_metadataID)
    {
      m_totalApply      = true;
      m_metadataIDApply = true;
      m_metadataID      = (number != 0);
    }
    DDX_Check(pDX,IDC_TRACE_CONN,number);
    if((number!=0) != m_tracing)
    {
      m_totalApply   = true;
      m_tracingApply = true;
      m_tracing      = (number != 0);
    }
    DDX_Text(pDX,IDC_CON_TRACEFILE,sitem);
    if(sitem != m_traceFile)
    {
      m_totalApply     = true;
      m_traceFileApply = true;
      m_traceFile      = sitem;
    }
    DDX_Text(pDX,IDC_CON_FILE_DSN,sitem);
    if(sitem != m_fileDSN)
    {
      m_totalApply   = true;
      m_fileDSNApply = true;
      m_fileDSN      = sitem;
    }
    DDX_Text(pDX,IDC_CON_SAVE_DSN,sitem);
    if(sitem != m_fileDSNSave)
    {
      m_totalApply       = true;
      m_fileDSNSaveApply = true;
      m_fileDSNSave      = sitem;
    }
    DDX_Text(pDX,IDC_CON_LOGINTIMEOUT,sitem);
    number = atoi(sitem); 
    if(number >= 0 && number != m_loginTimeout)
    {
      m_totalApply        = true;
      m_loginTimeout      = number;
      m_loginTimeoutApply = true;
    }
    DDX_Text(pDX,IDC_CON_CONNTIMEOUT,sitem);
    number = atoi(sitem);
    if(number >= 0 && number != m_connTimeout)
    {
      m_totalApply       = true;
      m_connTimeout      = number;
      m_connTimeoutApply = true;
    }
    DDX_Text(pDX,IDC_CON_PACKETSIZE,sitem);
    number = atoi(sitem);
    if(number >= 0 && number != m_packetSize)
    {
      m_totalApply      = true;
      m_packetSizeApply = true;
      m_packetSize      = number;
    }
    // SQL_CUR_USE_IF_NEEDED = 0 -> First string
    // SQL_CUR_USE_ODBC      = 1 -> Second string
    // SQL_CUR_USE_DRIVER    = 2 -> Third String
    DDX_CBIndex(pDX,IDC_CON_CURSORLIB,number);
    if(number >= 0 && number <= 2 && number != m_odbcCursors)
    {
      m_totalApply       = true;
      m_odbcCursorsApply = true;
      m_odbcCursors      = number;
    }
    // SQL_TXN_READ_UNCOMMITTED  = 1
    // SQL_TXN_READ_COMMITTED    = 2
    // SQL_TXN_REPEATABLE_READ   = 4
    // SQL_TXN_SERIALIZABLE      = 8
    DDX_CBIndex(pDX,IDC_CON_ISOLATION,number);
    switch(number)
    {
      case 0: number = SQL_TXN_READ_UNCOMMITTED; break;
      case 1: number = SQL_TXN_READ_COMMITTED;   break;
      case 2: number = SQL_TXN_REPEATABLE_READ;  break;
      case 3: number = SQL_TXN_SERIALIZABLE;     break;
    }
    if(number >= 0 && number != m_txnLevel)
    {
      m_totalApply    = true;
      m_txnLevelApply = true;
      m_txnLevel      = number;
    }
    DDX_Text(pDX,IDC_CON_TRANSLIB,sitem); 
    if(sitem != m_transLib)
    {
      m_totalApply    = true;
      m_transLib      = sitem;
      m_transLibApply = true;
    }
    DDX_Text(pDX,IDC_CON_TRANSOPTION,sitem);
    number = atoi(sitem);
    if(number != m_transOption)
    {
      m_totalApply       = true;
      m_transOptionApply = true;
      m_transOption      = number;
    }
    DDX_Check(pDX,IDC_CON_AUTOCOMMIT,number);
    if(number != m_autoCommit)
    {
      m_totalApply      = true;
      m_autoCommitApply = true;
      m_autoCommit      = number;
    }
  }
  else // Data2Controls
  {
    DDX_Text(pDX,IDC_CON_USER,      m_user);
    DDX_Text(pDX,IDC_CON_PASSWORD,  m_password);
    DDX_Text(pDX,IDC_CON_DATASOURCE,m_datasource);
    DDX_Control(pDX,IDC_CON_SAFTY,  m_boxSafty);
//  sitem =  m_safty ? "Read only (production)" : "None (development)";
//  DDX_Text (pDX,IDC_CON_SAFTY,sitem);
    m_boxSafty.SetCurSel(m_safty ? 1 : 0);

    number = m_readOnly;
    DDX_Check(pDX,IDC_CON_READONLY,number);
    number = m_connDead;
    DDX_Check(pDX,IDC_CON_DEAD,number);
    number = m_tracing;
    DDX_Check(pDX,IDC_TRACE_CONN,  number);
    DDX_Text(pDX,IDC_CON_TRACEFILE,m_traceFile);
    DDX_Text(pDX,IDC_CON_CATALOG,  m_catalog);
    DDX_Text(pDX,IDC_CON_FILE_DSN, m_fileDSN);
    DDX_Text(pDX,IDC_CON_SAVE_DSN, m_fileDSNSave);
    sitem.Format("%d",m_loginTimeout);
    DDX_Text(pDX,IDC_CON_LOGINTIMEOUT,sitem);
    sitem.Format("%d",m_connTimeout);
    DDX_Text(pDX,IDC_CON_CONNTIMEOUT, sitem);
    sitem.Format("%d",m_packetSize);
    DDX_Text(pDX,IDC_CON_PACKETSIZE,  sitem);
    number = m_metadataID;
    DDX_Check(pDX,IDC_CON_METADATAID,number);
    number = (m_quietMode > 0);
    DDX_Check(pDX,IDC_CON_QUIET,number);
    number = m_autoIPD;
    DDX_Check(pDX,IDC_CON_AUTOIPD,number);

    // SQL_CUR_USE_IF_NEEDED = 0 -> First string
    // SQL_CUR_USE_ODBC      = 1 -> Second string
    // SQL_CUR_USE_DRIVER    = 2 -> Third String
    DDX_CBIndex(pDX,IDC_CON_CURSORLIB,m_odbcCursors);

    // SQL_TXN_READ_UNCOMMITTED  = 1
    // SQL_TXN_READ_COMMITTED    = 2
    // SQL_TXN_REPEATABLE_READ   = 4
    // SQL_TXN_SERIALIZABLE      = 8
    switch(m_txnLevel)
    {
      case SQL_TXN_READ_UNCOMMITTED: number = 0; break;
      case SQL_TXN_READ_COMMITTED:   number = 1; break;
      case SQL_TXN_REPEATABLE_READ:  number = 2; break;
      case SQL_TXN_SERIALIZABLE:     number = 3; break;
    }
    DDX_CBIndex(pDX,IDC_CON_ISOLATION,number);

    DDX_Text(pDX,IDC_CON_TRANSLIB,m_transLib);
    sitem.Format("%d",m_transOption);
    DDX_Text(pDX,IDC_CON_TRANSOPTION,sitem);
    number = m_autoCommit != 0;
    DDX_Check(pDX,IDC_CON_AUTOCOMMIT,number);
  }
  // Set the apply button
  GetDlgItem(IDC_APPLY)->EnableWindow(m_totalApply && (*m_database));
}

BEGIN_MESSAGE_MAP(ConnectionDlg, CDialog)
    ON_BN_CLICKED  (IDC_CON_SIMPLE,       OnSimple)
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
   BOOL res = CDialog::OnInitDialog();
   SetDialogControls();
   return res;
}

// Make a connection to the database
bool
ConnectionDlg::Connect()
{
  bool didError = false;
  CString status;
  char buffer[1024];
  CWaitCursor take_a_deep_sigh;

  CString connectStr;

  if(m_fileDSN.IsEmpty())
  {
    connectStr  = "DSN=";
    connectStr += m_datasource;
    status.Format("Trying to connect to: %s as %s",m_datasource,m_user);
  }
  else
  {
    connectStr  = "FILEDSN=";
    connectStr += m_fileDSN;
    status.Format("Trying to connect with DSN=%s as %s",m_fileDSN,m_user);
  }
  connectStr += ";UID=";
  connectStr += m_user;
  connectStr += ";PWD=";
  connectStr += m_password;
  // Eventually a savefile
  if(!m_fileDSNSave.IsEmpty())
  {
    connectStr += ";SAVEFILE=";
    connectStr += m_fileDSNSave;
  }

  try
  {
    Common::SetStatusText(status, TRUE);
    if(*m_database)
    {
      delete (*m_database);
    }
    *m_database = new SQLDatabase();
    OnApplyBefore();
    (*m_database)->Open(connectStr,m_safty);
  }
  catch(CString& error)
  {
    strncpy(buffer,error.GetString(),1024);
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
    strcpy(buffer,"No connection with the database");
    didError = true;
  }
  if(didError)
  {
    char buffer2[2048];
    strcpy(buffer2,buffer);

    if(m_password != "")
    {
      m_password = "********";
    }
    strcat(buffer2,"\r\nDatasource = \"");
    strcat(buffer2,m_datasource);
    strcat(buffer2,"\" User = \"");
    strcat(buffer2,m_user);
    strcat(buffer2,"\" Password = \"");
    strcat(buffer2,m_password);
    AfxMessageBox(buffer2);
    Common::SetStatusText("", TRUE);
  }
  else
  {
    // Apply connection attributes after connect
    OnApplyAfter();
  }
  return !didError;
}

void 
ConnectionDlg::OnOK()
{
  UpdateData(Controls2Data);
  if(m_moment)
  {
    // Dialog used on living connection
    if(m_totalApply && (*m_database))
    {
      OnApplyAfter();
    }
  }
  else
  {
    // Try to connect after start/logoff
    if(!Connect())
    {
      return;
    }
  }
  CDialog::OnOK();
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
  if(*m_database)
  {
    m_connDead = (*m_database)->GetSQLInfoDB()->GetConnectionDead();
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
  if(*m_database)
  {
    m_quietMode = (*m_database)->GetSQLInfoDB()->GetAttributeQuiet();
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
  if(*m_database)
  {
    if((*m_database)->GetSQLInfoDB()->GetAttributeAutoIPD() == -1)
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
ConnectionDlg::OnSimple()
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
                       ,"File for ODBC trace logging"
                       ,"log"
                       ,"C:\\*.*"
                       ,0
                       ,"Logfiles|*.log|Text files|*.txt|All files|*.*");
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
                       ,"DatasourceName file (DSN) for login"
                       ,"dsn"
                       ,directory
                       ,0
                       ,"ODBC Datasources|*.dsn|All files|*.*");
  if(diag.DoModal() == IDOK)
  {
    if(m_fileDSN.CompareNoCase(diag.GetChosenFile()))
    {
      // Different DSN file chosen
      m_fileDSN      = diag.GetChosenFile();
      m_fileDSNApply = true;
      m_datasource   = "";
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
                       ,"DatasourceName file (DSN) for saving successful login"
                       ,"dsn"
                       ,directory
                       ,0
                       ,"ODBC Datasources|*.dsn|All files|*.*");
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
                       ,"Translation library (*.DLL) for datasource"
                       ,"dll"
                       ,directory
                       ,0
                       ,"Libraries|*.dll|All files|*.*");
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
ConnectionDlg::SetLogin(bool    moment
                       ,CString user
                       ,CString password
                       ,CString datasource
                       ,bool    safty)
{
  m_moment     = moment;
  m_user       = user;
  m_password   = password;
  m_datasource = datasource;
  m_safty      = safty;
}

void
ConnectionDlg::SetDataConnector(SQLDatabase** database)
{
  m_database  = database;
  GetConnectionAttributes();
}

// Set controls read-only read-write
void
ConnectionDlg::SetDialogControls()
{
  CWnd* wind;
  wind = GetDlgItem(IDC_CON_USER);          wind->EnableWindow(m_moment == 0);
  wind = GetDlgItem(IDC_CON_PASSWORD);      wind->EnableWindow(m_moment == 0);
  wind = GetDlgItem(IDC_CON_DATASOURCE);    wind->EnableWindow(m_moment == 0 && m_fileDSN.IsEmpty());
  wind = GetDlgItem(IDC_CON_SAFTY);         wind->EnableWindow(m_moment == 0 && m_fileDSN.IsEmpty());
  wind = GetDlgItem(IDC_CON_DRIVER);        wind->EnableWindow(false);
  wind = GetDlgItem(IDC_CON_FILE_DSN);      wind->EnableWindow(m_moment == 0);
  wind = GetDlgItem(IDC_CON_SAVE_DSN);      wind->EnableWindow(m_moment == 0);
  wind = GetDlgItem(IDC_TRACE_CONN);        wind->EnableWindow(true);
  wind = GetDlgItem(IDC_CON_TRACEFILE);     wind->EnableWindow(true);
  wind = GetDlgItem(IDC_CON_TRANSLIB);      wind->EnableWindow(true);
  wind = GetDlgItem(IDC_CON_TRANSOPTION);   wind->EnableWindow(true);
  wind = GetDlgItem(IDC_CON_LOGINTIMEOUT);  wind->EnableWindow(m_moment == 0);
  wind = GetDlgItem(IDC_CON_CONNTIMEOUT);   wind->EnableWindow(true);
  wind = GetDlgItem(IDC_CON_CURSORLIB);     wind->EnableWindow(m_moment == 0);
  wind = GetDlgItem(IDC_CON_ISOLATION);     wind->EnableWindow(true);
  wind = GetDlgItem(IDC_CON_PACKETSIZE);    wind->EnableWindow(m_moment == 0);
  wind = GetDlgItem(IDC_CON_CATALOG);       wind->ModifyStyle(0,ES_READONLY);
  wind = GetDlgItem(IDC_CON_READONLY);      wind->EnableWindow(m_moment != 0);
  wind = GetDlgItem(IDC_CON_DEAD);          wind->EnableWindow(true);
  wind = GetDlgItem(IDC_CON_QUIET);         wind->EnableWindow(true);
  wind = GetDlgItem(IDC_CON_METADATAID);    wind->EnableWindow(true);
  wind = GetDlgItem(IDC_CON_AUTOIPD);       wind->EnableWindow(true);
  wind = GetDlgItem(IDC_CON_AUTOCOMMIT);    wind->EnableWindow(true);

  wind = GetDlgItem(IDC_OPT_USER);          wind->EnableWindow(m_moment == 0);
  wind = GetDlgItem(IDC_OPT_PWD);           wind->EnableWindow(m_moment == 0);
  // Buttons
  wind = GetDlgItem(IDC_CON_SIMPLE);        if(m_moment) wind->DestroyWindow();
  wind = GetDlgItem(IDC_BTN_RDBMS);         wind->EnableWindow(m_moment == 0);
  wind = GetDlgItem(IDC_APPLY);             wind->EnableWindow(false);
  // File buttons
  wind = GetDlgItem(IDC_BTN_FILEDSN);       wind->EnableWindow(m_moment == 0);
  wind = GetDlgItem(IDC_BTN_SAVEDSN);       wind->EnableWindow(m_moment == 0);
  wind = GetDlgItem(IDC_BTN_TRANSLIB);      wind->EnableWindow(true);
}

void
ConnectionDlg::GetConnectionAttributes()
{
  if(*m_database)
  {
    m_readOnly     = (*m_database)->GetReadOnly();
    m_loginTimeout = (*m_database)->GetLoginTimeout();
    m_connDead     = (*m_database)->GetSQLInfoDB()->GetConnectionDead();
    m_tracing      = (*m_database)->GetSQLInfoDB()->GetAttributeTracing();
    m_traceFile    = (*m_database)->GetSQLInfoDB()->GetAttributeTraceFile();
    m_catalog      = (*m_database)->GetSQLInfoDB()->GetAttributeCatalog();
    m_fileDSN      = (*m_database)->GetSQLInfoDB()->GetAttributeFileDSN();
    m_fileDSNSave  = (*m_database)->GetSQLInfoDB()->GetAttributeFileDSNSave();
    m_connTimeout  = (*m_database)->GetSQLInfoDB()->GetAttributeConnTimeout();
    m_packetSize   = (*m_database)->GetSQLInfoDB()->GetAttributePacketSize();
    m_metadataID   = (*m_database)->GetSQLInfoDB()->GetAttributeMetadataID();
    m_txnLevel     = (*m_database)->GetSQLInfoDB()->GetAttributeTransLevel();
    m_transLib     = (*m_database)->GetSQLInfoDB()->GetAttributeTranslib();
    m_transOption  = (*m_database)->GetSQLInfoDB()->GetAttributeTransoption();
    m_quietMode    = (*m_database)->GetSQLInfoDB()->GetAttributeQuiet();
    m_autoCommit   = (*m_database)->GetTransaction() ? false : true;
    if(m_tracing && m_traceFile.IsEmpty())
    {
      m_traceFile = "<Not set: search for SQL.LOG>";
    }
  }
}

// Apply attributes before a connection is made
void
ConnectionDlg::OnApplyBefore()
{
  if(m_loginTimeoutApply && (*m_database))
  {
    (*m_database)->SetLoginTimeout(m_loginTimeout);
    m_loginTimeoutApply = false;
  }
  if(m_readOnlyApply && (*m_database))
  {
    (*m_database)->SetReadOnly(m_readOnly);
    m_readOnlyApply = false;
  }
  if(m_fileDSNApply && (*m_database))
  {
    (*m_database)->GetSQLInfoDB()->SetAttributeFileDSN(m_fileDSN);
    m_fileDSNApply = false;
  }
  if(m_fileDSNSaveApply && (*m_database))
  {
    (*m_database)->GetSQLInfoDB()->SetAttributeFileDSNSave(m_fileDSNSave);
    m_fileDSNSaveApply = false;
  }
  if(m_odbcCursorsApply && (*m_database))
  {
    (*m_database)->GetSQLInfoDB()->SetAttributeOdbcCursors(m_odbcCursors);
    m_odbcCursorsApply = false;
  }
  if(m_packetSizeApply && (*m_database))
  {
    if(!(*m_database)->GetSQLInfoDB()->SetAttributePacketSize(m_packetSize))
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
    if(!(*m_database)->SetReadOnly(m_readOnly))
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
      if(!(*m_database)->GetSQLInfoDB()->SetAttributeTraceFile(m_traceFile))
      {
        m_traceFile = "";
      }
    }
    m_traceFileApply = false;
  }
  if(m_tracingApply)
  {
    if(m_tracing && m_traceFile.IsEmpty())
    {
      m_traceFile = "<Not set: search for SQL.LOG>";
    }
    if(!m_tracing && m_traceFile.GetAt(0) == '<')
    {
      m_traceFile = "";
    }
    if(!(*m_database)->GetSQLInfoDB()->SetAttributeTracing(m_tracing))
    {
      m_tracing = !m_tracing;
    }
    m_tracingApply = false;
  }
  if(m_connTimeoutApply)
  {
    if(!(*m_database)->GetSQLInfoDB()->SetAttributeConnTimeout(m_connTimeout))
    {
      m_connTimeout = 0;
    }
    m_connTimeoutApply = false;
  }
  if(m_metadataIDApply)
  {
    if(!(*m_database)->GetSQLInfoDB()->SetAttributeMetadataID(m_metadataID))
    {
      m_metadataID = false;
    }
    m_metadataIDApply = false;
  }
  if(m_txnLevelApply)
  {
    (*m_database)->GetSQLInfoDB()->SetAttributeTransLevel(m_txnLevel);
    m_txnLevelApply = false;
  }
  if(m_transLibApply)
  {
    if(!(*m_database)->GetSQLInfoDB()->SetAttributeTranslib(m_transLib))
    {
      m_transLib = "";
    }
    m_transLibApply = false;
  }
  if(m_transOptionApply)
  {
    if(!(*m_database)->GetSQLInfoDB()->SetAttributeTransoption(m_transOption))
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
    if (RegOpenKeyEx(where,"Software\\ODBC\\ODBC.INI\\ODBC File DSN"
      ,0,KEY_QUERY_VALUE,&hKey) == ERROR_SUCCESS)
    {
      TCHAR szData[128];
      DWORD dwKeyDataType;
      DWORD dwDataBufSize = sizeof(szData);

      if (RegQueryValueEx(hKey
        ,"DefaultDSNDir"
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
    directory += "\\*.dsn";
  }
  return directory;
}

