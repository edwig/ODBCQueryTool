''/*
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
#include "MultConnectionsDlg.h"
#include "ConnStringDlg.h"
#include "Query\codeer.h"
#include "COmmon\AppGlobal.h"
#include <KnownFolders.h>
#include <direct.h>
#include <wincrypt.h>
#include <SQLDatabase.h>
#include <StyleFrameWork.h>
#include <AppUtilities.h>
#include <time.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MultConnectionsDlg dialog


MultConnectionsDlg::MultConnectionsDlg(CWnd* pParent /*=NULL*/)
                   :StyleDialog(IDD_CONNECTION, pParent)
                   ,m_UserEdit("")
                   ,m_UserPassword("")
                   ,m_DataSource("")
                   ,m_Safty(false)
                   ,m_nextSection(1)
                   ,m_useConnString(false)
                   ,m_optionalUser(false)
                   ,m_optionalPassword(false)
{
  Common::FindApplicDirectory(m_applicDirectory);
  CreateINIDirectory();
  m_ODBC_iniFile = GetLocalDirectory() + "ODBC_querytool.ini";
}

MultConnectionsDlg::~MultConnectionsDlg()
{
  m_settings.clear();
}

void MultConnectionsDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);

  DDX_Control (pDX,IDC_CON_LIST,       m_list);
  DDX_Control (pDX,IDC_CON_USER,       m_boxUserEdit,    m_UserEdit);
  DDX_Control (pDX,IDC_CON_PASSWORD,   m_boxUserPassword,m_UserPassword);
  DDX_CBString(pDX,IDC_CON_DATASOURCES,m_comboDataSource,m_DataSource);
  DDX_Control (pDX,IDC_CON_SAFTY,      m_comboSafty);
  DDX_Control (pDX,IDC_CON_STRING,     m_buttonConString);
  DDX_Control (pDX,IDC_CON_TEST,       m_buttonTest);
  DDX_Control (pDX,IDC_CON_DELETE,     m_buttonDelete);
  DDX_Control (pDX,IDC_CON_DETAILS,    m_buttonDetails);
  DDX_Control (pDX,IDC_CON_HELP,       m_buttonHelp);
  DDX_Control (pDX,IDOK,               m_buttonOK);
  DDX_Control (pDX,IDCANCEL,           m_buttonCancel);

  CString data;
  if(pDX->m_bSaveAndValidate == (BOOL)Controls2Data)
  {
    m_Safty = m_comboSafty.GetCurSel();
  }
  else // Data2Controls
  {
    m_comboSafty.SetCurSel(m_Safty ? 1 : 0);
    int ind = m_comboDataSource.FindStringExact(0,m_DataSource);
    if (ind >= 0)
    {
      m_comboDataSource.SetCurSel(ind);
    }
  }
  m_buttonConString.EnableWindow(!m_connString.IsEmpty());
}

BEGIN_MESSAGE_MAP(MultConnectionsDlg, StyleDialog)
    ON_NOTIFY    (NM_DBLCLK, IDC_CON_LIST, OnDblclk_list)
    ON_NOTIFY    (NM_CLICK,  IDC_CON_LIST, OnClick_list)
    ON_BN_CLICKED(IDC_CON_STRING,          OnBnClickedConString)
    ON_BN_CLICKED(IDC_CON_TEST,            OnBnClicked_test)
    ON_BN_CLICKED(IDC_CON_DELETE,          OnBnClicked_delete)
    ON_BN_CLICKED(IDC_CON_DETAILS,         OnDetails)
    ON_CBN_SELCHANGE(IDC_CON_SAFTY, &MultConnectionsDlg::OnCbnSelchangeConSafty)
END_MESSAGE_MAP()

BOOL 
MultConnectionsDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();
  SetWindowText("ODBC Connections");

  // Set combo box
  m_comboSafty.AddString("None (development)");
  m_comboSafty.AddString("Read only (production)");

  // Set password field
  m_boxUserPassword.SetPassword(true);
  m_boxUserPassword.SetEmpty(true,"Password");

  // Fill the connection dialog
  InitGridEmpty();

  // Read in all settings
  ReadSettings();
  GetDatasourcesFromODBCManager();
  FillGridWithSettings();

  UpdateData(Data2Controls);

  // Leave the focus for the first input
  if(m_UserPassword.IsEmpty())
  {
    m_boxUserPassword.SetFocus();
  }
  else
  {
    m_boxUserEdit.SetFocus();
  }
  // Set the font for the list
  SetListFont();
  SetCanResize();

  return FALSE;
}

void
MultConnectionsDlg::SetupDynamicLayout()
{
  StyleDialog::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif

  manager.AddItem(IDC_CON_LIST,     manager.MoveNone(),                         manager.SizeHorizontalAndVertical(100,100));
  manager.AddItem(IDC_STATIC1,      manager.MoveHorizontal(100),                manager.SizeNone());
  manager.AddItem(IDC_STATIC2,      manager.MoveHorizontal(100),                manager.SizeNone());
  manager.AddItem(IDC_STATIC3,      manager.MoveHorizontal(100),                manager.SizeNone());
  manager.AddItem(IDC_STATIC4,      manager.MoveHorizontal(100),                manager.SizeNone());
  manager.AddItem(IDC_CON_DATASOURCES,  manager.MoveHorizontal(100),                manager.SizeNone());
  manager.AddItem(IDC_CON_PASSWORD, manager.MoveHorizontal(100),                manager.SizeNone());
  manager.AddItem(IDC_CON_USER,     manager.MoveHorizontal(100),                manager.SizeNone());
  manager.AddItem(IDC_CON_SAFTY,    manager.MoveHorizontal(100),                manager.SizeNone());
  manager.AddItem(IDC_CON_STRING,   manager.MoveHorizontal(100),                manager.SizeNone());
  manager.AddItem(m_buttonTest,     manager.MoveHorizontalAndVertical(100,100), manager.SizeNone());
  manager.AddItem(m_buttonDelete,   manager.MoveHorizontalAndVertical(100,100), manager.SizeNone());
  manager.AddItem(m_buttonDetails,  manager.MoveHorizontalAndVertical(100,100), manager.SizeNone());
  manager.AddItem(m_buttonHelp,     manager.MoveHorizontalAndVertical(100,100), manager.SizeNone());
  manager.AddItem(m_buttonOK,       manager.MoveHorizontalAndVertical(100,100), manager.SizeNone());
  manager.AddItem(m_buttonCancel,   manager.MoveHorizontalAndVertical(100,100), manager.SizeNone());

  manager.Adjust();
}

/////////////////////////////////////////////////////////////////////////////
// MultConnectionsDlg message handlers

void 
MultConnectionsDlg::OnDblclk_list(NMHDR* hdr, LRESULT *pResult)
{
  OnClick_list(hdr,pResult);
  OnOK();
}

void
MultConnectionsDlg::OnClick_list(NMHDR* hdr, LRESULT *pResult)
{
  MCCellID cell = m_list.GetFocusCell();
  int index = cell.row;
  if(index >= 1)
  {
    CString source   = m_list.GetItemText(index,0);
    CString user     = m_list.GetItemText(index,1);
    ODBCSetting* set = FindSetting(user,source);
  
    if(set)
    {
      // Found our set of Settings
      m_UserEdit         = set->GetUser();
      m_UserPassword     = set->GetPassword();
      m_DataSource       = set->GetDatasource();
      m_Safty            = set->GetSafty();
      m_connString       = set->GetConnString();
      m_useConnString    = set->GetUseConnection();
      m_optionalUser     = set->GetOptionalUser();
      m_optionalPassword = set->GetOptionalPassword();

      int ind = m_comboDataSource.FindStringExact(0,m_DataSource);
      m_comboDataSource.SetCurSel(ind);

      UpdateData(Data2Controls);
    }
  }
}

// Find a setting by name
ODBCSetting*
MultConnectionsDlg::FindSetting(CString user,CString datasource)
{
  for(auto& setting : m_settings)
  {
    if((setting.GetUser() == user) && (setting.GetDatasource() == datasource))
    {
      return &setting;
    }
  }
  return nullptr;
}

bool
MultConnectionsDlg::RemoveSetting(ODBCSetting* p_setting)
{
  for(ODBCSettings::iterator it = m_settings.begin();it != m_settings.end();++it)
  {
    if(&(*it) == p_setting)
    {
      m_settings.erase(it);
      return true;
    }
  }
  return false;
}

void
MultConnectionsDlg::FillGridWithSettings()
{
  CString mUser;
  CString mDatasource;
  CString mPassword;
  CString mConnString;
  bool    mSafty   = 0;
  int     mSave    = 0;
  int     maxTimes = 0;

  for(auto& setting : m_settings)
  {
    int times = atol(setting.GetTimesUsed());
    if(times > maxTimes)
    {
      maxTimes    = times;
      mUser       = setting.GetUser();
      mDatasource = setting.GetDatasource();
      mPassword   = setting.GetPassword();
      mSafty      = setting.GetSafty();
      mConnString = setting.GetConnString();
    }
    // Insert into the ListCtrl
    InsertListline(setting.GetDatasource()
                  ,setting.GetUser()
                  ,setting.GetTimesUsed()
                  ,setting.GetConnString().IsEmpty() ? "" : "X"
                  ,setting.GetLastUsage());
  }
  // Set the boxes with the max value
  m_UserEdit     = mUser;
  m_UserPassword = mPassword;
  m_DataSource   = mDatasource;
  m_Safty        = mSafty;
  m_connString   = mConnString;
}

void
MultConnectionsDlg::InsertItem(int p_row,int p_col,CString& p_text,UINT p_format /* = 0 */)
{
  MC_ITEM item;
  item.mask    = GVIF_TEXT | GVIF_FORMAT;
  item.row     = p_row;
  item.col     = p_col;
  item.strText = p_text;
  item.nFormat = p_format ? p_format : DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX;
  m_list.SetItem(&item);
}

int 
MultConnectionsDlg::InsertListline (CString& p_datasource
                                   ,CString& p_user
                                   ,CString& p_total
                                   ,CString  p_hasConnString
                                   ,CString& p_lasttime)
{
  int row = m_list.GetRowCount();
  m_list.InsertRow(p_datasource);
  InsertItem(row,1,p_user);
  InsertItem(row,2,p_total);
  InsertItem(row,3,p_hasConnString);
  InsertItem(row,4,p_lasttime);
  return 0;
}

void 
MultConnectionsDlg::InitGridEmpty()
{
  if(!m_list)
  {
    return;
  }
	// Set standard
	m_list.SetEditable(FALSE);
	m_list.EnableDragAndDrop(FALSE);
	m_list.SetHeaderSort(TRUE);
  m_list.DeleteNonFixedRows();

  // Set empty grid
  try
  {
    m_list.SetRowCount(1);
    m_list.SetColumnCount(5);
		m_list.SetFixedRowCount(1);
  }
  catch(CMemoryException* e)
  {
    e->ReportError();
    e->Delete();
    return;
  }
  
  MC_ITEM item;
  item.mask = GVIF_TEXT | GVIF_FORMAT;
  item.row = 0;
  item.col = 0;
	item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX;

  item.strText = "Datasource";
  m_list.SetItem(&item);
  m_list.SetColumnWidth(0,120);

  item.col     = 1;
  item.strText = "User";
  m_list.SetItem(&item);
  m_list.SetColumnWidth(1,120);

  item.col     = 2;
  item.strText = "Total";
  item.nFormat |= DT_SORT_NUMERIC;
  m_list.SetItem(&item);
  m_list.SetColumnWidth(2,50);

  item.col     = 3;
  item.strText = "STR";
  m_list.SetItem(&item);
  m_list.SetColumnWidth(3,40);

  item.col     = 4;
  item.strText = "Last time";
  m_list.SetItem(&item);
  m_list.SetColumnWidth(4,150);

  // Sync with the outside world
 	//m_list.AutoSize(GVS_HEADER);
  //m_list.AutoSizeColumns(GVS_HEADER);
  m_list.ExpandLastColumn();

  m_list.ShowWindow(SW_SHOW);
  m_list.SetSingleRowSelection(true);
}

void
MultConnectionsDlg::SetListFont()
{
  // Get QueryFont
  const OpenEditor::SettingsManager& manager  = COEDocument::GetSettingsManager();
  const OpenEditor::GlobalSettings&  settings = manager.GetGlobalSettings();
  CString queryFont = settings.GetSQLQueryFont().c_str();

  // Split current font registry in name and size
  CString fontName;
  int     fontSize = 0;
  int pos = queryFont.Find(';');
  if(pos)
  {
    fontName = queryFont.Left(pos);
    fontSize = atoi(queryFont.Mid(pos+1));
  }
  if(fontName.IsEmpty())
  {
    fontName = "Tahoma";
  }
  if(fontSize < 8 ) fontSize = 8;
  if(fontSize > 24) fontSize = 24;

  // Set the font
  CFont font;
  font.CreatePointFont(fontSize * 10,fontName);
  m_list.SetFont(&font,TRUE);
}

CString
MultConnectionsDlg::Current()
{
  _tzset();
  time_t ltime;
  time(&ltime);
  struct tm now;
  struct tm* ptnow = &now;
  localtime_s(ptnow,&ltime);
  CString moment;
  moment.Format("%4d-%02d-%02d %d:%02d:%02d"
               ,now.tm_year + 1900
               ,now.tm_mon  + 1
               ,now.tm_mday
               ,now.tm_hour
               ,now.tm_min
               ,now.tm_sec);
  return moment;
}

// If Dialog OK, Save our settings
void
MultConnectionsDlg::OnOK()
{
  UpdateData(Controls2Data);

  if(m_DataSource.IsEmpty() && m_connString.IsEmpty())
  {
    AfxMessageBox("For an ODBC login we need at least a datasource or a connection string",MB_OK | MB_ICONERROR);
    return;
  }
  m_boxUserEdit.SetFocus();


  SaveSettings(m_DataSource
              ,m_UserEdit
              ,m_UserPassword
              ,m_Safty
              ,m_connString
              ,m_useConnString
              ,m_optionalUser
              ,m_optionalPassword);

  StyleDialog::OnOK();
}

bool
MultConnectionsDlg::SaveSettings(CString p_datasource
                                ,CString p_user
                                ,CString p_password
                                ,bool    p_safty
                                ,CString p_connString
                                ,bool    p_useConnString
                                ,bool    p_optionalUser
                                ,bool    p_optionalPassword)
{
  CString timesUsed("1");
  CString lastUsage = Current();

#ifdef OQT_SAVE_PASSWORD
  CString password = p_password;
#else
  CString password;
#endif
  CString safty    = p_safty ? "true" : "false";
  ODBCSetting* set = FindSetting(p_user,p_datasource);
  if(set)
  {
    // Set times used one (1) higher
    int times = atol(set->GetTimesUsed()) ;
    timesUsed.Format("%ld",times + 1);

    set->SetTimesUsed(timesUsed);
    set->SetLastUsage(lastUsage);
    set->SetPassword(password);
    set->SetSafty(p_safty);
    set->SetConnString(p_connString);
    set->SetUseConnection(p_useConnString);
    set->SetOptionalUser(p_optionalUser);
    set->SetOptionalPassword(p_optionalPassword);
  }
  else
  {
    // Newly defined combination.
    // Test for user/password 
    if((p_user.IsEmpty() || p_password.IsEmpty()) && p_connString.IsEmpty())
    {
      if(StyleMessageBox(this
                       ,"For this datasource the user/password combination is not completely filled in. Proceed anyway?"
                       ,"Open ODBCQuerytool"
                       ,MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
      {
        return false;
      }
    }

    ODBCSetting setting(m_nextSection++
                       ,p_user
                       ,password
                       ,p_datasource
                       ,lastUsage
                       ,timesUsed
                       ,safty
                       ,p_connString
                       ,p_useConnString
                       ,p_optionalUser
                       ,p_optionalPassword);
    m_settings.push_back(setting);
  }
  return WriteSettings();
}

void
MultConnectionsDlg::OnDetails()
{
  UpdateData(Controls2Data);
  EndDialog(IDC_CON_DETAILS);
}

bool 
MultConnectionsDlg::WriteSettings()
{
  int  ind = 1;
  CString number;
  for(auto& setting : m_settings)
  {
    int section = setting.GetSection();
    if(section == 0)
    {
      section = m_nextSection++;
    }
    number.Format("%d",section);
    CCodeer cod;
    CString codedPassword = cod.CodeerString(setting.GetPassword(),setting.GetUser());
    char buffer[1024];
    DWORD size = 1024;
    CryptBinaryToString((BYTE*)codedPassword.GetString(),codedPassword.GetLength(),CRYPT_STRING_BASE64|CRYPT_STRING_NOCRLF,buffer,&size);

    CString safty = setting.GetSafty() ? "true" : "false";
    ::WritePrivateProfileString(number,"Datasource",    setting.GetDatasource(),   m_ODBC_iniFile);
    ::WritePrivateProfileString(number,"User",          setting.GetUser(),         m_ODBC_iniFile);
#ifdef OQT_SAVE_PASSWORD
    ::WritePrivateProfileString(number,"Password",      buffer,                    m_ODBC_iniFile);
#endif
    ::WritePrivateProfileString(number,"LastUsage",     setting.GetLastUsage(),    m_ODBC_iniFile);
    ::WritePrivateProfileString(number,"TimesUsed",     setting.GetTimesUsed(),    m_ODBC_iniFile);
    ::WritePrivateProfileString(number,"Safty",         safty,                     m_ODBC_iniFile);
    ::WritePrivateProfileString(number,"ConnString",    setting.GetConnString(),   m_ODBC_iniFile);
    ::WritePrivateProfileString(number,"UseConnString", setting.GetUseConnection()    ? "true" : "false",m_ODBC_iniFile);
    ::WritePrivateProfileString(number,"OptionalUser",  setting.GetOptionalUser()     ? "true" : "false",m_ODBC_iniFile);
    ::WritePrivateProfileString(number,"OptionalPasswd",setting.GetOptionalPassword() ? "true" : "false",m_ODBC_iniFile);
  }
  if(ind > MAX_ODBCSETTINGS)
  {
    AfxMessageBox("Max number of ODBC settings reached (50).\r\nLast setting will not be read back in next connection");
  }
  return true;
}

bool 
MultConnectionsDlg::ReadSettings()
{
  int ind = 1;
  CString number;
  char buffer[1025];
  CString datasource;
  CString user;
  CString password;
  CString lastUsage;
  CString timesUsed;
  CString safty;
  CString connString;
  bool    useConnString;
  bool    optionalUser;
  bool    optionalPasswd;
  
  for(ind = 1; ind < MAX_ODBCSETTINGS; ++ind)
  {
    number.Format("%d",ind);
    ::GetPrivateProfileString(number,"Datasource",  "",buffer,1024,m_ODBC_iniFile); datasource   = buffer;
    ::GetPrivateProfileString(number,"User",        "",buffer,1024,m_ODBC_iniFile); user         = buffer;
#ifdef OQT_SAVE_PASSWORD
    ::GetPrivateProfileString(number,"Password",    "",buffer,1024,m_ODBC_iniFile); password     = buffer;
#endif
    ::GetPrivateProfileString(number,"LastUsage",   "",buffer,1024,m_ODBC_iniFile); lastUsage    = buffer;
    ::GetPrivateProfileString(number,"TimesUsed",   "",buffer,1024,m_ODBC_iniFile); timesUsed    = buffer;
    ::GetPrivateProfileString(number,"Safty",       "",buffer,1024,m_ODBC_iniFile); safty        = buffer;
    ::GetPrivateProfileString(number,"ConnString",  "",buffer,1024,m_ODBC_iniFile); connString   = buffer;
    ::GetPrivateProfileString(number,"UseConnString", "false",buffer,1024,m_ODBC_iniFile); useConnString  = _stricmp(buffer,"true") == 0;
    ::GetPrivateProfileString(number,"OptionalUser",  "false",buffer,1024,m_ODBC_iniFile); optionalUser   = _stricmp(buffer,"true") == 0;
    ::GetPrivateProfileString(number,"OptionalPasswd","false",buffer,1024,m_ODBC_iniFile); optionalPasswd = _stricmp(buffer,"true") == 0;

    if(!datasource.IsEmpty())
    {
      CString decodedPassword;

      if(!password.IsEmpty())
      {
        char  buffer[1024];
        DWORD size = 1024;
        DWORD skip = 0;
        DWORD flags = 0;
        if(CryptStringToBinary(password,password.GetLength(),CRYPT_STRING_BASE64,(BYTE*)buffer,&size,&skip,&flags))
        {
          if(flags == CRYPT_STRING_BASE64 && size < 1024)
          {
            buffer[size] = 0;
          }
          CCodeer cod;
          decodedPassword = cod.DeCodeerString(buffer,user);
        }
      }

      if(ind >= m_nextSection)
      {
        m_nextSection = ind + 1;
      }
      ODBCSetting setting(ind
                         ,user
                         ,decodedPassword
                         ,datasource
                         ,lastUsage
                         ,timesUsed
                         ,(safty == "true")
                         ,connString
                         ,useConnString
                         ,optionalUser
                         ,optionalPasswd);
      m_settings.push_back(setting);
    }
  }
  return true;
}

bool 
MultConnectionsDlg::GetDatasourcesFromODBCManager()
{
  HENV ODBCHandle;
  ::SQLAllocEnv(&ODBCHandle);

  if(ODBCHandle)
  {
    short dsnLen,desLen;
    unsigned char DataSourceBuffer[SQL_MAX_DSN_LENGTH+1];
    unsigned char Description[256];
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
      m_comboDataSource.AddString((const char *)DataSourceBuffer);
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
    ::SQLFreeEnv(ODBCHandle);
    return true;
  }
  ::SQLFreeEnv(ODBCHandle);
  return false;
}

void 
MultConnectionsDlg::OnBnClicked_test()
{
  UpdateData(Controls2Data);

  MCCellID cell = m_list.GetFocusCell();
  int index = cell.row;
  if(index > 0)
  {
    CString source   = m_list.GetItemText(index,0);
    CString user     = m_list.GetItemText(index,1);
    ODBCSetting* set = FindSetting(user,source);
  
    if(set)
    {
        SQLDatabase* database = NULL;
        CString      password = set->GetPassword();
        CString      connectStr;

        // Check for password
        password = password.GetLength() ? password : m_UserPassword;

        if(password.IsEmpty())
        {
          CString mesg("No password saved in the settings and no password given on the dialog.\n"
                       "Are you sure you want to continue to test the datasource?");
          if(AfxMessageBox(mesg,MB_YESNO|MB_ICONEXCLAMATION|MB_DEFBUTTON2) == IDNO)
          {
            return;
          }
        }

        // Construct the connection string
        connectStr += "DSN=";
        connectStr += set->GetDatasource();
        connectStr += ";UID=";
        connectStr += set->GetUser();
        connectStr += ";PWD=";
        connectStr += password;

        // Setting the status for a long operation
        CString status;
        status.Format("Trying to connect to: %s",set->GetDatasource().GetString());

        try
        {
          CWaitCursor hold_our_breath;
          Common::SetStatusText(status, TRUE);
          database = new SQLDatabase();
          database->Open(connectStr);
        }
        catch(...)
        {
          status.Format("Cannot make connection with: %s as: %s"
                        ,set->GetDatasource()
                        ,set->GetUser());
          AfxMessageBox(status);
        }
        if(database && database->IsOpen())
        {
          status.Format("Successful connection to: %s as: %s"
                        ,set->GetDatasource()
                        ,set->GetUser());
          AfxMessageBox(status);
          database->Close();
        }
        if(database)
        {
          delete database;
          database = NULL;
        }
        Common::SetStatusText("",TRUE);
    }
    else
    {
      AfxMessageBox("No previous connections (yet)!");
    }
  }
  else
  {
    AfxMessageBox("Select an ODBC connection in the list first!");
  }
}

void 
MultConnectionsDlg::OnBnClicked_delete()
{
  MCCellID cell = m_list.GetFocusCell();
  int row = cell.row;
  if(row > 0)
  {
    CString source   = m_list.GetItemText(row,0);
    CString user     = m_list.GetItemText(row,1);
    ODBCSetting* set = FindSetting(user,source);
    if(set)
    {
      CString question;
      question.Format("Do you want to delete the datasource '%s' for user '%s'?",source,user);
      if(StyleMessageBox(this,question,"ODBCQueryTool",MB_YESNO|MB_TASKMODAL|MB_DEFBUTTON2) != IDYES)
      {
        // Then don't do it
        return;
      }
      // Remove from the ini file
      CString num;
      int section = set->GetSection();
      num.Format("%d",section);
      ::WritePrivateProfileString(num,"Datasource",    "",m_ODBC_iniFile);
      ::WritePrivateProfileString(num,"User",          "",m_ODBC_iniFile);
      ::WritePrivateProfileString(num,"LastUsage",     "",m_ODBC_iniFile);
      ::WritePrivateProfileString(num,"TimesSaved",    "",m_ODBC_iniFile);
      ::WritePrivateProfileString(num,"SavePassword",  "",m_ODBC_iniFile);
      ::WritePrivateProfileString(num,"Safty",         "",m_ODBC_iniFile);
      ::WritePrivateProfileString(num,"ConnString",    "",m_ODBC_iniFile);
      ::WritePrivateProfileString(num,"UseConnString", "",m_ODBC_iniFile);
      ::WritePrivateProfileString(num,"OptionalUser",  "",m_ODBC_iniFile);
      ::WritePrivateProfileString(num,"OptionalPasswd","",m_ODBC_iniFile);

      // Remove from the settings registration
      RemoveSetting(set);
    }
    m_list.DeleteRow(row);
    // Sync with the outside world
	  m_list.AutoSize();
    m_list.AutoSizeColumns(GVS_BOTH);
    m_list.ExpandLastColumn();
    
    m_list.SetFocusCell(cell);
  }
}

void 
MultConnectionsDlg::OnBnClickedConString()
{
  ConnStringDlg dlg(this,m_connString);
  if(dlg.DoModal() == IDOK)
  {
    m_connString = dlg.GetConnectionString();
  }
}

void MultConnectionsDlg::OnCbnSelchangeConSafty()
{
}

CString
MultConnectionsDlg::GetLocalDirectory()
{
  return m_applicDirectory + "\\EDO\\OpenODBCQuerytool\\";
}

void
MultConnectionsDlg::CreateINIDirectory()
{
  if(!m_applicDirectory.IsEmpty())
  {
    CString directory = m_applicDirectory + "\\EDO";
    if(_mkdir(directory))
    {
      int er = GetLastError();
      if(er != EEXIST && er != ERROR_ALREADY_EXISTS)
      {
        StyleMessageBox(this,"Cannot create directory: " + directory,"Error",MB_OK|MB_ICONERROR);
      }
    }
    directory += "\\OpenODBCQuerytool";
    if(_mkdir(directory))
    {
      int er = GetLastError();
      if(er != EEXIST && er != ERROR_ALREADY_EXISTS)
      {
        StyleMessageBox(this,"Cannot create directory: " + directory,"Error",MB_OK|MB_ICONERROR);
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
// ConnectionDlgSettings
//
//////////////////////////////////////////////////////////////////////////

ODBCSetting::ODBCSetting(int     p_section
                        ,CString p_user
                        ,CString p_password
                        ,CString p_datasource
                        ,CString p_lastUsage
                        ,CString p_timesUsed
                        ,bool    p_safty
                        ,CString p_connString
                        ,bool    p_useConnString
                        ,bool    p_optUser
                        ,bool    p_optPassword)
            :m_section(p_section)
            ,m_user(p_user)
            ,m_password(p_password)
            ,m_datasource(p_datasource)
            ,m_lastUsage(p_lastUsage)
            ,m_timesUsed(p_timesUsed)
            ,m_safty(p_safty)
            ,m_connString(p_connString)
            ,m_useConnectionString(p_useConnString)
            ,m_optionalUser(p_optUser)
            ,m_optionalPassword(p_optPassword)
{
}

ODBCSetting::~ODBCSetting()
{
}

