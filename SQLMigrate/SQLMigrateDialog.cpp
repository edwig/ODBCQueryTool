////////////////////////////////////////////////////////////////////////
//
// File: SQLMigrateDialog.cpp
//
// Copyright (c) 1998-2022 ir. W.E. Huisman
// All rights reserved
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
// Version number: See SQLComponents.h
//
#include "stdafx.h"
#include "AboutDlg.h"
#include "SQLMigrate.h"
#include "SQLMigrateApp.h"
#include "SQLMigrateDialog.h"
#include "MapDialog.h"
#include "AboutDlg.h"
#include <direct.h>
#include <shobjidl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Windows-shell taskbar list object for progress
const IID my_IID_ITaskbarList3 = {0xea1afb91, 0x9e28, 0x4b86, { 0x90, 0xe9, 0x9e, 0x9f, 0x8a, 0x5e, 0xef, 0xaf }};
const IID my_CLSID_TaskbarList = {0x56fdf344, 0xfd6d, 0x11d0, { 0x95, 0x8a, 0x00, 0x60, 0x97, 0xc9, 0xa0, 0x90 }};
static ITaskbarList3* ptbl = NULL;

SQLMigrateDialog::SQLMigrateDialog(CWnd* pParent)
                 :StyleDialog(SQLMigrateDialog::IDD, pParent)
                 ,m_font(nullptr)
                 ,m_start(0L)
{
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

  m_toLogfile       = false;
  m_allTables       = false;
  m_exportRunning   = false;
  m_commandLineMode = false;
  m_exportResult    = false;
  m_do_truncate     = false;
  m_do_deletes      = false;
  m_do_primarys     = false;
  m_do_indices      = false;
  m_do_sequences    = false;
  m_do_triggers     = false;
  m_do_access       = false;
  m_do_data         = false;
  m_do_foreigns     = false;
  m_do_tables       = false;
  m_do_views        = false;
  m_directMigration = MigrateType::DataPump;
}

SQLMigrateDialog::~SQLMigrateDialog()
{
  if(m_font)
  {
    delete m_font;
  }
}

void 
SQLMigrateDialog::DoDataExchange(CDataExchange* pDX)
{
  StyleDialog::DoDataExchange(pDX);

  DDX_Control (pDX, IDC_DIRECTORY,       m_editDirectory,     m_directory);
  DDX_Control (pDX, IDC_BUTT_DIR,        m_buttonDirectory);
  // Source database
  DDX_CBString(pDX, IDC_SOURCE,          m_comboSourceDSN,    m_sourceDSN);
  DDX_Control (pDX, IDC_SOURCE_USER,     m_editSourceUser,    m_sourceUser);
  DDX_Control (pDX, IDC_SOURCE_PASSWORD, m_editSourcePassword,m_sourcePassword);
  DDX_Control (pDX, IDC_SOURCE_TYPE,     m_editSourceType,    m_sourceType);
  // Target database
  DDX_CBString(pDX, IDC_TARGET,          m_comboTargetDSN,    m_targetDSN);
  DDX_Control (pDX, IDC_TARGET_USER,     m_editTargetUser,    m_targetUser);
  DDX_Control (pDX, IDC_TARGET_PASSWORD, m_editTargetPassword,m_targetPassword);
  DDX_Control (pDX, IDC_TARGET_TYPE,     m_editTargetType,    m_targetType);
  // Schemata
  DDX_Control (pDX, IDC_SOURCE_SCHEMA,   m_editSourcSchema,   m_sourceSchema);
  DDX_Control (pDX, IDC_TARGET_SCHEMA,   m_editTargetSchema,  m_targetSchema);
  // Migration
  DDX_CBIndex (pDX, IDC_DIRECT_MIGRATION,m_comboDirectMigration,(int&)m_directMigration);
  DDX_Check   (pDX, IDC_ALLTABLES,       m_allTables);
  DDX_Check   (pDX, IDC_TOLOGFILE,       m_toLogfile);
  // Convert to file
  DDX_Control (pDX, IDC_DROP_FILE,       m_editDropscript,    m_dropscript);
  DDX_Control (pDX, IDC_CREATE_FILE,     m_editCreatescript,  m_createscript);
  DDX_Control (pDX, IDC_LOGLINES,        m_editLogPerRow,     m_logPerRow);
  // If only one table migration
  DDX_Control (pDX, IDC_TABLE,           m_editTable,         m_table);
  DDX_Control (pDX, IDC_TABLESPACE,      m_editTablespace,    m_tablespace);
  DDX_Control (pDX, IDC_WHERE,           m_editWhere,         m_where);
  // Options
  DDX_Control (pDX, IDC_TOLOGFILE,       m_checkToLogfile);
  DDX_Control (pDX, IDC_ALLTABLES,       m_checkAllTables);
  // Progress and status
  DDX_Control (pDX, IDC_STATUS,          m_editStatus,        m_status);
  DDX_Control (pDX, IDC_ESTIMATED,       m_editEstimated,     m_estimated);
  DDX_Control (pDX, IDC_PROGRESS1,       m_table_gauge);
  DDX_Control (pDX, IDC_PROGRESS2,       m_tables_gauge);
  // The results
  DDX_Control (pDX, IDC_LOG,             m_editLog,           m_log);
  // Options
  DDX_Control (pDX, IDC_DO_TABLES,       m_check_do_tables);
  DDX_Control (pDX, IDC_DO_VIEWS,        m_check_do_views);
  DDX_Control (pDX, IDC_DO_DATA,         m_check_do_data);
  DDX_Control (pDX, IDC_DO_TRUNCATE,     m_check_do_truncate);
  DDX_Control (pDX, IDC_DO_DELETES,      m_check_do_deletes);
  DDX_Control (pDX, IDC_DO_PRIMARYS,     m_check_do_primarys);
  DDX_Control (pDX, IDC_DO_INDICES,      m_check_do_indices);
  DDX_Control (pDX, IDC_DO_FOREIGN,      m_check_do_foreigns);
  DDX_Control (pDX, IDC_DO_SEQUENCES,    m_check_do_sequences);
  DDX_Control (pDX, IDC_DO_TRIGGERS,     m_check_do_triggers);
  DDX_Control (pDX, IDC_DO_ACCESS,       m_check_do_access);

  DDX_Check   (pDX, IDC_DO_TABLES,       m_do_tables);
  DDX_Check   (pDX, IDC_DO_DATA,         m_do_data);
  DDX_Check   (pDX, IDC_DO_VIEWS,        m_do_views);
  DDX_Check   (pDX, IDC_DO_TRUNCATE,     m_do_truncate);
  DDX_Check   (pDX, IDC_DO_DELETES,      m_do_deletes);
  DDX_Check   (pDX, IDC_DO_PRIMARYS,     m_do_primarys);
  DDX_Check   (pDX, IDC_DO_INDICES,      m_do_indices);
  DDX_Check   (pDX, IDC_DO_FOREIGN,      m_do_foreigns);
  DDX_Check   (pDX, IDC_DO_SEQUENCES,    m_do_sequences);
  DDX_Check   (pDX, IDC_DO_TRIGGERS,     m_do_triggers);
  DDX_Check   (pDX, IDC_DO_ACCESS,       m_do_access);

  DDX_Control (pDX, IDC_EXPORT,          m_buttonExport);
  DDX_Control (pDX, IDC_CLOSE,           m_buttonClose);
}

BEGIN_MESSAGE_MAP(SQLMigrateDialog, StyleDialog)
  ON_WM_SYSCOMMAND()
  ON_WM_PAINT()
  ON_WM_QUERYDRAGICON()
  ON_BN_CLICKED   (IDC_BUTT_DIR,        OnDirectory)
  ON_EN_KILLFOCUS (IDC_SOURCE_USER,     OnEnKillfocusSourceUser)
  ON_EN_KILLFOCUS (IDC_TARGET_USER,     OnEnKillfocusTargetUser)
  ON_CBN_CLOSEUP  (IDC_DIRECT_MIGRATION,OnDirectMigration)
  ON_BN_CLICKED   (IDC_ALLTABLES,       OnAllTables)
  ON_BN_CLICKED   (IDC_TOLOGFILE,       OnToLogfile)
  ON_BN_CLICKED   (IDC_EXPORT,          OnMigrate) 
  ON_BN_CLICKED   (IDC_CLOSE,           OnClosingButton)
  ON_COMMAND      (ID_APP_ABOUT,        OnAboutBox)
  ON_COMMAND      (ID_THEMA_LIME,       OnStyleLime)
  ON_COMMAND      (ID_THEMA_SKYBLUE,    OnStyleSkyblue)
  ON_COMMAND      (ID_THEMA_PURPLE,     OnStylePurple)
  ON_COMMAND      (ID_THEMA_MODERATE,   OnStyleModerateGray)
  ON_COMMAND      (ID_THEMA_PUREGRAY,   OnStylePureGray)
  ON_COMMAND      (ID_THEMA_BLACKWHITE, OnStyleBlackWhite)
END_MESSAGE_MAP()

BOOL 
SQLMigrateDialog::OnInitDialog()
{
  StyleDialog::OnInitDialog();
  SetWindowText(SQL_MIGRATE " " SQL_COMPONENTS_VERSION);
  ShowMinMaxButton();
  // Use as system menu
  SetSysMenu(IDR_SYSMENU);
  SetAboutBoxAndIcon(IDM_ABOUTBOX,IDS_ABOUTBOX);

  SetDirectory();
  SetComboBoxes();

  // Reset gauges
  SetTableGauge(0,100);
  SetTablesGauge(0,100);

  // Disable for now
  m_editStatus.EnableWindow(FALSE);
  m_editWhere.EnableWindow(FALSE);

  // Set font of the log window
  m_font = new CFont();
  m_font->CreatePointFont(10,"Courier");
  m_editLog.SetFont(m_font);
  m_editLog.SetLimitText(0);
  m_editLog.SetMutable(false);
  // Set the buttons
  m_buttonDirectory.SetStyle("dir");
  m_buttonExport.SetStyle("ok");
  m_buttonClose.SetStyle("can");

  // Set the defaults
  m_toLogfile           = 0;
  m_allTables           = 0;
  m_parameters.v_direct = MigrateType::DataPump;

  m_do_tables    = 1;
  m_do_data      = 1;
  m_do_truncate  = 1;
  m_do_deletes   = 1;

  if(!StartFromCommandLine())
  {
    return FALSE;
  }

  InitTaskbar();

  // Load standard profile
  FindProfile();
  LoadProfile();
  if(m_commandLineMode)
  {
    OnMigrate();
    Closing(m_exportResult);
  }
  SetCanResize();

  // return TRUE  unless you set the focus to a control
  m_editLog.SetFocus();
  return FALSE;
}

bool
SQLMigrateDialog::StartFromCommandLine()
{
  LPWSTR* argv;
  int argc;
  argv = CommandLineToArgvW(GetCommandLineW(),&argc);
  m_iniFile = "SQLMigrate.ini";
  if(argc == 2)
  {
    m_commandLineMode = true;
    m_iniFile = argv[1];
    HANDLE file_handle;
    file_handle = CreateFile(m_iniFile,0,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
    if(file_handle == INVALID_HANDLE_VALUE)
    {
      m_toLogfile = true;
      Logging log;
      log.Open(false);
      log.WriteLog("Absolute or relative pathname to SQLMigrate.ini is NOT correct!");
      log.WriteLog("End of SQLMigrate.");
      log.Close();
      Closing(false);
      return false;
    }
    CloseHandle(file_handle);
  }
  GlobalFree(argv);

  if(argc > 2)
  {
    StyleMessageBox(this,"Wrong number of arguments on the command line",SQL_MIGRATE,MB_OK | MB_ICONERROR);
  }
  return true;
}

void
SQLMigrateDialog::SetupDynamicLayout()
{
  StyleDialog::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif

  manager.AddItem(IDC_GRP_STATUS,manager.MoveNone(),manager.SizeHorizontal(100));
  manager.AddItem(IDC_PROGRESS1, manager.MoveNone(),manager.SizeHorizontal(100));
  manager.AddItem(IDC_PROGRESS2, manager.MoveNone(),manager.SizeHorizontal(100));
  manager.AddItem(IDC_LOG,       manager.MoveNone(),manager.SizeHorizontalAndVertical(100,100));
  manager.AddItem(IDC_CLOSE,     manager.MoveHorizontalAndVertical(100,100),manager.SizeNone());
  manager.AddItem(IDC_EXPORT,    manager.MoveHorizontalAndVertical(100,100),manager.SizeNone());

  manager.Adjust();
}

void
SQLMigrateDialog::OnAboutBox()
{
  AboutDlg dlg;
  dlg.DoModal();
}

void
SQLMigrateDialog::InitTaskbar()
{
  HRESULT res = CoInitialize(NULL);
  if(res == S_OK)
  {
    HRESULT hr = CoCreateInstance(my_CLSID_TaskbarList,NULL,CLSCTX_ALL,my_IID_ITaskbarList3,(LPVOID*) &ptbl);
    if(hr != S_OK)
    {
      // Fail silently
      ptbl = nullptr;
    }
  }
}

void 
SQLMigrateDialog::SetDirectory()
{
  char* dir = m_directory.GetBufferSetLength(MAX_PATH);
  GetCurrentDirectory(MAX_PATH,dir);
  m_directory.ReleaseBuffer();
  UpdateData(FALSE);
}

void
SQLMigrateDialog::SetComboBoxes()
{
  // Getting datasources and adding to the combo boxes
  DSMap sources;
  SQLDatabase database;
  database.GetDataSources(sources);

  for(auto& datasource : sources)
  {
    m_comboSourceDSN.AddString(datasource.m_datasource);
    m_comboTargetDSN.AddString(datasource.m_datasource);
  }

  m_comboDirectMigration.AddString("Direct datapump migration");
  m_comboDirectMigration.AddString("SELECT/INSERT migration");
  m_comboDirectMigration.AddString("Write to scripts");
}

void 
SQLMigrateDialog::OnSysCommand(UINT nID, LPARAM lParam)
{
  if((nID & 0xFFF0) == IDM_ABOUTBOX)
  {
    AboutDlg dlgAbout;
    dlgAbout.DoModal();
    return;
  }
  if(nID == SC_CLOSE)
  {
    if(StyleMessageBox(this,"Do you want to close the program?",SQL_MIGRATE,MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION) == IDNO)
    {
      return;
    }
  }
  StyleDialog::OnSysCommand(nID,lParam);
}

void
SQLMigrateDialog::FindProfile()
{
  XString workingDir("");

  if(m_iniFile.Left(2).Right(1) == ":" || m_iniFile.Left(1) == "\\")
  {
    // absolute pathname
    LPCTSTR path = m_iniFile;
    char drive[_MAX_DRIVE];
    char dir  [_MAX_DIR];
    char fname[_MAX_FNAME];
    char ext  [_MAX_EXT];
    _splitpath_s(path,drive,_MAX_DRIVE,dir,_MAX_DIR,fname,_MAX_FNAME,ext,_MAX_EXT);
    workingDir = (XString) drive + (XString) dir;
    m_iniFile  = (XString) fname + (XString) ext;
  }
  else
  {
    // Find in the current working directory
    char curdir[MAX_PATH];
    _getcwd(curdir,MAX_PATH);
    workingDir = curdir;
    if(workingDir.Right(1) != '\\')
    {
      workingDir += "\\";
    }
  }
  m_profile = workingDir + m_iniFile;
}

void
SQLMigrateDialog::LoadProfile()
{
  char buffer[MAX_PATH + 1];
  XString log_loglines;

  // Source Database
  GetPrivateProfileString("SOURCE","database","<source-database>",buffer,MAX_PATH,m_profile); m_sourceDSN      = buffer;
  GetPrivateProfileString("SOURCE","user"    ,"user",             buffer,MAX_PATH,m_profile); m_sourceUser     = buffer;
  GetPrivateProfileString("SOURCE","password","password",         buffer,MAX_PATH,m_profile); m_sourcePassword = buffer;
  GetPrivateProfileString("SOURCE","schema",  "",                 buffer,MAX_PATH,m_profile); m_sourceSchema   = buffer;

  // Target Database
  GetPrivateProfileString("TARGET","database","<target-database>",buffer,MAX_PATH,m_profile); m_targetDSN      = buffer;
  GetPrivateProfileString("TARGET","user",    "user",             buffer,MAX_PATH,m_profile); m_targetUser     = buffer;
  GetPrivateProfileString("TARGET","password","password",         buffer,MAX_PATH,m_profile); m_targetPassword = buffer;
  GetPrivateProfileString("TARGET","schema",  "",                 buffer,MAX_PATH,m_profile); m_targetSchema   = buffer;

  // Type migration
  GetPrivateProfileString("TABLE","tablespace","",                buffer,MAX_PATH,m_profile); m_tablespace     = buffer;

  // File
  GetPrivateProfileString("FILE","create",   "script.sql",    buffer,MAX_PATH,m_profile); m_createscript = buffer;
  GetPrivateProfileString("FILE","drop",     "dropscript.sql",buffer,MAX_PATH,m_profile); m_dropscript   = buffer;
  m_directMigration = (MigrateType) GetPrivateProfileInt("FILE", "direct",1,m_profile);
  m_comboDirectMigration.SetCurSel((int)m_directMigration);

  // Tables
  GetPrivateProfileString("TABLE","table","",buffer,MAX_PATH,m_profile);  m_table = buffer;
  GetPrivateProfileString("TABLE","where","",buffer,MAX_PATH,m_profile);  m_where = buffer;
  m_allTables  = GetPrivateProfileInt("TABLE","all",    1,   m_profile);

  // Log and log lines
  m_toLogfile = GetPrivateProfileInt("LOG","logging",1,m_profile);
  int log_log = GetPrivateProfileInt("LOG","lines",1000,m_profile);
  if(log_log > 0)
  {
    log_loglines.Format("%d",log_log);
    m_logPerRow = log_loglines;
  }

  // Options
  m_do_tables    = GetPrivateProfileInt("OPTION","tables",   1,m_profile);
  m_do_data      = GetPrivateProfileInt("OPTION","data",     1,m_profile);
  m_do_views     = GetPrivateProfileInt("OPTION","views",    1,m_profile);
  m_do_truncate  = GetPrivateProfileInt("OPTION","truncate", 1,m_profile);
  m_do_deletes   = GetPrivateProfileInt("OPTION","deletes",  1,m_profile);
  m_do_primarys  = GetPrivateProfileInt("OPTION","primarys", 0,m_profile);
  m_do_indices   = GetPrivateProfileInt("OPTION","indices",  0,m_profile);
  m_do_foreigns  = GetPrivateProfileInt("OPTION","foreigns", 0,m_profile);
  m_do_sequences = GetPrivateProfileInt("OPTION","sequences",0,m_profile);
  m_do_triggers  = GetPrivateProfileInt("OPTION","triggers", 0,m_profile);
  m_do_access    = GetPrivateProfileInt("OPTION","access",   0,m_profile);


  // Check scripts
  if(m_dropscript.IsEmpty())
  {
    m_dropscript = "dropscript.sql";
  }
  if(m_createscript.IsEmpty())
  {
    m_createscript = "createscript.sql";
  }

  // Tell it the dialog
  UpdateData(FALSE);

  // Post processing in the interface
  OnToLogfile();
  OnDirectMigration();
  OnAllTables();
}

void
SQLMigrateDialog::SaveProfile()
{
  UpdateData();

  if(m_profile == "")
  {
    return;
  }

  // Source Database
  WritePrivateProfileString("SOURCE","database",m_sourceDSN,     m_profile);
  WritePrivateProfileString("SOURCE","user",    m_sourceUser,    m_profile);
  WritePrivateProfileString("SOURCE","password",m_sourcePassword,m_profile);
  WritePrivateProfileString("SOURCE","schema",  m_sourceSchema,  m_profile);

  // Target Database
  WritePrivateProfileString("TARGET","database", m_targetDSN,      m_profile);
  WritePrivateProfileString("TARGET","user",     m_targetUser,     m_profile);
  WritePrivateProfileString("TARGET","password", m_targetPassword, m_profile);
  WritePrivateProfileString("TARGET","schema",   m_targetSchema,   m_profile);

  // Type Migration
  WritePrivateProfileString("TABLE","tablespace",m_tablespace,     m_profile);

  // files
  XString directstring;
  directstring.Format("%d",m_directMigration);

  // Directory and files
  WritePrivateProfileString("FILE","create",   m_createscript, m_profile);
  WritePrivateProfileString("FILE","drop",     m_dropscript,   m_profile);
  WritePrivateProfileString("FILE","direct",   directstring,   m_profile);

  // Tables
  XString table_allestr = m_allTables ? "1" : "0";
  WritePrivateProfileString("TABLE","all",        table_allestr,    m_profile);
  WritePrivateProfileString("TABLE","table",      m_table,          m_profile);
  WritePrivateProfileString("TABLE","where",      m_where,         m_profile);

  // Log en log lines
  XString log_logstr = m_toLogfile ? "1" : "0";
  WritePrivateProfileString("LOG","logging",log_logstr  ,m_profile);
  WritePrivateProfileString("LOG","lines",  m_logPerRow ,m_profile);

  WritePrivateProfileString("OPTION","tables",   m_do_tables    ? "1" : "0",m_profile);
  WritePrivateProfileString("OPTION","data",     m_do_data      ? "1" : "0",m_profile);
  WritePrivateProfileString("OPTION","views",    m_do_views     ? "1" : "0",m_profile);
  WritePrivateProfileString("OPTION","truncate", m_do_truncate  ? "1" : "0",m_profile);
  WritePrivateProfileString("OPTION","deletes",  m_do_deletes   ? "1" : "0",m_profile);
  WritePrivateProfileString("OPTION","primarys", m_do_primarys  ? "1" : "0",m_profile);
  WritePrivateProfileString("OPTION","indices",  m_do_indices   ? "1" : "0",m_profile);
  WritePrivateProfileString("OPTION","foreigns", m_do_foreigns  ? "1" : "0",m_profile);
  WritePrivateProfileString("OPTION","sequences",m_do_sequences ? "1" : "0",m_profile);
  WritePrivateProfileString("OPTION","triggers", m_do_triggers  ? "1" : "0",m_profile);
  WritePrivateProfileString("OPTION","access",   m_do_access    ? "1" : "0",m_profile);
}

///////////////////////////////////////////////////////////////////////////////////////
//
//   MIGRATION
//
///////////////////////////////////////////////////////////////////////////////////////

void
SQLMigrateDialog::GetMigrationParameters()
{
  // Getting all the parameters from the dialog
  UpdateData();

  // Source
  m_parameters.v_source_dsn       = m_sourceDSN;
  m_parameters.v_source_user      = m_sourceUser;
  m_parameters.v_source_password  = m_sourcePassword;
  // Target
  m_parameters.v_target_dsn       = m_targetDSN;
  m_parameters.v_target_user      = m_targetUser;
  m_parameters.v_target_password  = m_targetPassword;
  // Schemata
  m_parameters.v_source_schema    = m_sourceSchema;
  m_parameters.v_target_schema    = m_targetSchema;

  m_parameters.v_directory        = m_directory;
  m_parameters.v_dropscript       = m_dropscript;
  m_parameters.v_createscript     = m_createscript;

  // Options
  m_parameters.v_table            = m_table; 
  m_parameters.v_tablespace       = m_tablespace;
  m_parameters.v_where            = m_where;
  m_parameters.v_logLines         = atoi(m_logPerRow);
  m_parameters.v_direct           = m_directMigration;
  // Options: Booleans
  m_parameters.v_do_tables        = m_do_tables;
  m_parameters.v_do_data          = m_do_data;
  m_parameters.v_do_views         = m_do_views;
  m_parameters.v_truncate         = m_do_truncate;
  m_parameters.v_deletes          = m_do_deletes;
  m_parameters.v_primarys         = m_do_primarys;
  m_parameters.v_indices          = m_do_indices;
  m_parameters.v_foreigns         = m_do_foreigns;
  m_parameters.v_sequences        = m_do_sequences;
  m_parameters.v_triggers         = m_do_triggers;
  m_parameters.v_access           = m_do_access;
  m_parameters.v_allObjects       = m_allTables;

  // Empty the log window
  m_editLog.SetWindowText(""); 
}

//   Perform the real migration now!
void
SQLMigrateDialog::PerformMigration()
{
  XString error;
  SQLMigrate migrate(m_parameters,m_logfile);

  // Needed for the estimated time on the dialog
  m_start = clock();

  try
  {
    // block all buttons
    m_exportRunning = true;
    m_exportResult  = migrate.Migrate();
  }
  catch(StdException& ex)
  {
    error = ex.GetErrorMessage();
  }
  catch(XString& ex)
  {
    error = ex;
  }
  catch(...)
  {
    error = "Migration is stopped with an error";
  }

  // NOW READY!
  m_exportRunning = false;

  // Show errors (if any)
  if(!error.IsEmpty())
  {
    m_logfile.WriteLog("");
    m_logfile.WriteLog("STOPPED WITH ERROR!");
    m_logfile.WriteLog(error);

    if(m_commandLineMode == false)
    {
      StyleMessageBox(this,error,SQL_MIGRATE,MB_OK | MB_ICONERROR);
    }
  }
}

// POST MIGRATION: CLEAN-UP
void
SQLMigrateDialog::PostMigration()
{
  // Reset the time
  m_estimated.Empty();
  m_start = 0L;

  // Reset the status box on the window
  SetTableGauge (0,100);
  SetTablesGauge(0,100);
  m_sourceType.Empty();
  m_targetType.Empty();
  SetDlgItemText(IDC_SOURCE_TYPE,"");
  SetDlgItemText(IDC_TARGET_TYPE,"");
  SetStatus("");
  m_editLog.LineScroll(m_editLog.GetLineCount());

  if(m_exportResult)
  {
    XString text("The migration is complete");
    if (m_commandLineMode)
    {
      m_logfile.WriteLog("");
      m_logfile.WriteLog(text);
    }
    else
    {
      StyleMessageBox(this,text,SQL_MIGRATE,MB_OK);
    }
  }
  // Closing the logfile now. we are done.
  m_logfile.Close();
}

//////////////////////////////////////////////////////////////////////////
//
// Message handlers
//
//////////////////////////////////////////////////////////////////////////

void SQLMigrateDialog::OnPaint() 
{
  if (IsIconic())
  {
    CPaintDC dc(this); // device context for painting

    SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

    // Center icon in client rectangle
    int cxIcon = GetSystemMetrics(SM_CXICON);
    int cyIcon = GetSystemMetrics(SM_CYICON);
    CRect rect;
    GetClientRect(&rect);
    int x = (rect.Width() - cxIcon + 1) / 2;
    int y = (rect.Height() - cyIcon + 1) / 2;

    // Draw the icon
    dc.DrawIcon(x, y, m_hIcon);
  }
  else
  {
    StyleDialog::OnPaint();
  }
}

HCURSOR 
SQLMigrateDialog::OnQueryDragIcon()
{
  return (HCURSOR) m_hIcon;
}

void
SQLMigrateDialog::OnClosingButton()
{
  OnClosing();
}

bool
SQLMigrateDialog::OnClosing() 
{
  if(m_exportRunning)
  {
    return false;
  }
  // Close program
  SaveProfile();
  Closing(true);

  return true;
}

void
SQLMigrateDialog::OnDirectory()
{
  MapDialog dlg;
  if(dlg.Browse(GetSafeHwnd(),"Find working directory",m_directory))
  {
    m_directory = dlg.GetPath();
    m_profile   = m_directory + "\\" + m_iniFile;
    if(StyleMessageBox(this,"Load profile from new working directory?",SQL_MIGRATE,MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION) == IDYES)
    {
      LoadProfile();
    }
    UpdateData(FALSE);
  }
}

// Preset schema with user
void 
SQLMigrateDialog::OnEnKillfocusSourceUser()
{
  UpdateData();
  m_sourceSchema = m_sourceUser;
  UpdateData(FALSE);
}

// Preset schema with user
void
SQLMigrateDialog::OnEnKillfocusTargetUser()
{
  UpdateData();
  m_targetSchema = m_targetUser;
  UpdateData(FALSE);
}

void
SQLMigrateDialog::Closing(bool p_success)
{
  if (m_commandLineMode)
  {
    ExitProcess(p_success ? EXIT_SUCCESS : EXIT_FAILURE);
  }
  else
  {
    CDialog::OnOK();
  }
}

void 
SQLMigrateDialog::OnDirectMigration() 
{
  UpdateData();

  if(m_directMigration == MigrateType::DataPump)
  {
    m_parameters.v_direct = MigrateType::DataPump;

    m_editDropscript  .EnableWindow(FALSE);
    m_editCreatescript.EnableWindow(FALSE);
    // enable target database
    m_comboTargetDSN.EnableWindow(TRUE);
  }
  else if(m_directMigration == MigrateType::SelectInsert)
  {
    m_parameters.v_direct = MigrateType::SelectInsert;
    m_editDropscript  .EnableWindow(FALSE);
    m_editCreatescript.EnableWindow(FALSE);
    // enable target database
    m_comboTargetDSN.EnableWindow(TRUE);
  } 
  else if (m_directMigration == MigrateType::SQLScripts)
  {
    m_parameters.v_direct = MigrateType::SQLScripts;
    m_editDropscript  .EnableWindow(TRUE);
    m_editCreatescript.EnableWindow(TRUE);
    // disable target database
    m_comboTargetDSN.EnableWindow(FALSE);
  }
}

void 
SQLMigrateDialog::OnAllTables() 
{
  UpdateData();
  m_allTables = m_checkAllTables.GetCheck();

  if (m_allTables == TRUE)
  {
    m_editTable .EnableWindow(FALSE);
    m_editWhere.EnableWindow(FALSE);
    m_parameters.v_table.Empty();
    m_parameters.v_where.Empty();
    m_table.Empty();
    m_where.Empty();
  }
  else
  {
    m_editTable .EnableWindow(TRUE);
    m_editWhere.EnableWindow(TRUE);
    m_parameters.v_table = m_table;
    m_parameters.v_where = m_where;
  }  
  UpdateData(FALSE);
}

void 
SQLMigrateDialog::OnToLogfile() 
{
  UpdateData();
  m_toLogfile = m_checkToLogfile.GetCheck();
  m_editLogPerRow.EnableWindow(m_toLogfile);
}

void
SQLMigrateDialog::SetTableGauge(int num,int maxnum)
{
  static ULONGLONG clock = 0;

  // Reduce flickering by repainting it only once a second
  if(GetTickCount64() < (clock + 1000L))
  {
    return;
  }
  clock = GetTickCount64();

  // The MFC documentation states that the range is in the 32 bit range
  // @#! This is NOT the case. The maximum is 32768
  if(maxnum > 32000)
  {
    int max = (100 * maxnum) / 32000;
    int pos = (100 * num)    / 32000;
    m_table_gauge.SetRange(0,(short)max);
    m_table_gauge.SetPos(pos);
  }
  else
  {
    m_table_gauge.SetRange(0,(short)maxnum);
    m_table_gauge.SetPos(num);
  }
}

void
SQLMigrateDialog::SetTablesGauge(int num,int maxnum)
{
  // Show total gauge on the dialog
  // In case this is bigger than 32768 tables we are in big trouble anyway
  // so do not bother with calculations on the maximum
  m_tables_gauge.SetRange(0,(short)maxnum);
  m_tables_gauge.SetPos(num);

  // Show total gauge on the taskbar
  if(ptbl)
  {
    ptbl->SetProgressValue(GetSafeHwnd(),num,maxnum);
  }
  // Calculate and show remaining time
  EstimateRemainingTime(num,maxnum);
}

void  
SQLMigrateDialog::SetSourceType(XString p_type)
{
  SetDlgItemText(IDC_SOURCE_TYPE,p_type);
}

void
SQLMigrateDialog::SetTargetType(XString p_type)
{
  SetDlgItemText(IDC_TARGET_TYPE,p_type);
}

void
SQLMigrateDialog::SetStatus(XString status)
{
  m_editStatus.EnableWindow(status != "");
  m_editStatus.SetWindowText(status); 
  m_status = status;
}

void 
SQLMigrateDialog::OnMigrate()
{
  GetMigrationParameters();
  PerformMigration();
  PostMigration();
}

BOOL
SQLMigrateDialog::AddLogLine(XString msg)
{
  m_editLog.SetRedraw(FALSE);

  int cLines = m_editLog.GetLineCount();
  int nLastChar = (cLines ? m_editLog.LineIndex(cLines-1) : 0) + m_editLog.LineLength(cLines);
  cLines = m_editLog.GetLineCount();
  while(m_editLog.LineIndex(cLines-1) > 28000)
  {
    // If the number of lines in the edit box becomes to large (30.000 lines)
    // we must clear a few lines at the beginning
    int nFirstLine  = m_editLog.LineIndex(0);
    int nSecondLine = m_editLog.LineIndex(10);
    m_editLog.SetSel(nFirstLine, nSecondLine, TRUE /*NoScroll*/);
    m_editLog.ReplaceSel("");
    cLines = m_editLog.GetLineCount();
  }
  nLastChar = (cLines ? m_editLog.LineIndex(cLines-1) : 0) + m_editLog.LineLength(cLines);
  m_editLog.SetSel(nLastChar, nLastChar, TRUE);

  m_editLog.SetRedraw(TRUE);

  msg.Replace("\n","\r\n");

  m_editLog.ReplaceSel(msg);
  m_editLog.ReplaceSel("\r\n");

  m_editLog.GetWindowText(m_log);
  m_editLog.LineScroll(m_editLog.GetLineCount());
  return TRUE;
}

void 
SQLMigrateDialog::EstimateRemainingTime(int p_num,int p_maxnum)
{
  // Check if we have something to do
  m_estimated.Empty();
  if(m_start && p_num > 0)
  {
    clock_t now = clock();

    long total  = ((now - m_start) / p_num) * p_maxnum;
    long remain = total - (now - m_start);

    int days = remain / (24 * 60 * 60 * CLOCKS_PER_SEC);
    remain  -=   days * (24 * 60 * 60 * CLOCKS_PER_SEC);
    int hour = remain /      (60 * 60 * CLOCKS_PER_SEC);
    remain  -=   hour *      (60 * 60 * CLOCKS_PER_SEC);
    int mins = remain /           (60 * CLOCKS_PER_SEC);
    remain  -=   mins *           (60 * CLOCKS_PER_SEC);
    int secs = remain /                 CLOCKS_PER_SEC;

    if(days)
    {
      m_estimated.Format("%d days ",days);
    }
    m_estimated.AppendFormat("%2.2d:%2.2d:%2.2d",hour,mins,secs);
  }
  m_editEstimated.SetWindowText(m_estimated);
}

void 
SQLMigrateDialog::HandleMessages()
{
  // Handle only the PAINT messages for larger processes
  // So we can see the application
  // Potentially we can have an endless loop here
  // so we limit the loop to a certain time frame
  MSG msg;
  ULONGLONG ticks = GetTickCount64();
  while((GetTickCount64() - ticks) < 200L && (PeekMessage(&msg,NULL,WM_MOVE,WM_USER,PM_REMOVE)))
  {
    try
    {
      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);
    }
    catch(StdException& er)
    {
      // How now brown cow?
      UNREFERENCED_PARAMETER(er);
    }
  }
}

void
SQLMigrateDialog::OnCancel()
{
  if(StyleMessageBox(this,"Do you want to close the program?",SQL_MIGRATE,MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION) == IDYES)
  {
    StyleDialog::OnCancel();
  }
}
