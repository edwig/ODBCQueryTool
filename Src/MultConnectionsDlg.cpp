/*
    Copyright (C) 2004-2009 Edwig Huisman

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
#include "MultConnectionsDlg.h"
#include "Query\codeer.h"
#include <KnownFolders.h>
#include <direct.h>
#include <wincrypt.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// MultConnectionsDlg dialog


MultConnectionsDlg::MultConnectionsDlg(CWnd* pParent /*=NULL*/)
                   :CDialog(MultConnectionsDlg::IDD, pParent)
                   ,m_UserEdit("")
                   ,m_UserPassword("")
                   ,m_DataSource("")
                   ,m_Safty(false)
                   ,m_nextSection(1)
                   ,m_vista(false)
{
  FindApplicDirectory();
  CreateINIDirectory();
  m_ODBC_iniFile = GetLocalDirectory() + "ODBC_querytool.ini";
}

MultConnectionsDlg::~MultConnectionsDlg()
{
  m_settings.clear();
}

void MultConnectionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_CON_USER,       m_boxUserEdit);
  DDX_Control(pDX,IDC_CON_PASSWORD,   m_boxUserPassword);
  DDX_Control(pDX,IDC_CON_DATASOURCES,m_boxDataSource);
  DDX_Control(pDX,IDC_CON_SAFTY,      m_boxSafty);

  CString data;
  if(pDX->m_bSaveAndValidate == (BOOL)Controls2Data)
  {
    DDX_Text(pDX,IDC_CON_USER,         data);     m_UserEdit     = data;
    DDX_Text(pDX,IDC_CON_PASSWORD,     data);     m_UserPassword = data;
    DDX_Text(pDX,IDC_CON_DATASOURCES,  data);     m_DataSource   = data;
    DDX_Text(pDX,IDC_CON_SAFTY,        data); 
//  m_Safty = (data == "Read only (production)") ? true : false;
    m_Safty = m_boxSafty.GetCurSel();
  }
  else // Data2Controls
  {
    data = m_UserEdit;      DDX_Text (pDX,IDC_CON_USER,         data);
    data = m_UserPassword;  DDX_Text (pDX,IDC_CON_PASSWORD,     data);
    data = m_DataSource;    DDX_Text (pDX,IDC_CON_DATASOURCES,  data);
//  data =  m_Safty ? "Read only (production)" : "None (development)";
//  DDX_Text (pDX,IDC_CON_SAFTY,data);
    m_boxSafty.SetCurSel(m_Safty ? 1 : 0);
  }
}

BEGIN_MESSAGE_MAP(MultConnectionsDlg, CDialog)
    ON_NOTIFY    (NM_DBLCLK, IDC_CON_LIST, OnDblclk_list)
    ON_NOTIFY    (NM_CLICK,  IDC_CON_LIST, OnClick_list)
    ON_BN_CLICKED(IDC_CON_TEST,            OnBnClicked_test)
    ON_BN_CLICKED(IDC_CON_DELETE,          OnBnClicked_delete)
    ON_BN_CLICKED(IDC_CON_DETAILS,         OnDetails)
    ON_CBN_SELCHANGE(IDC_CON_SAFTY, &MultConnectionsDlg::OnCbnSelchangeConSafty)
END_MESSAGE_MAP()

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
  CCellID cell = m_list.GetFocusCell();
  int index = cell.row;
  if(index >= 1)
  {
    CString source = m_list.GetItemText(index,0);
    CString user   = m_list.GetItemText(index,1);
    ConSetIter set = FindSetting(user,source);
  
    if(set != m_settings.end())
    {
      // Found our set of Settings
      m_UserEdit     = set->GetUser();
      m_UserPassword = set->GetPassword();
      m_DataSource   = set->GetDatasource();
      m_Safty        = set->GetSafty();
      UpdateData(Data2Controls);
    }
  }
}

// Find a setting by name
ConSetIter
MultConnectionsDlg::FindSetting(CString user
                           ,CString datasource)
{
  ConSetIter iter;

  for(iter  = m_settings.begin();
      iter != m_settings.end();
      ++iter)
  {
    if((iter->GetUser() == user) && (iter->GetDatasource() == datasource))
    {
      return iter;
    }
  }
  return m_settings.end();
}

void
MultConnectionsDlg::InsertItem(int p_row,int p_col,CString& p_text,UINT p_format /* = 0 */)
{
  GV_ITEM item;
  item.mask    = GVIF_TEXT | GVIF_FORMAT;
  item.row     = p_row;
  item.col     = p_col;
  item.strText = p_text;
  item.nFormat = p_format ? p_format : DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX;
  m_list.SetItem(&item);
}

int 
MultConnectionsDlg::InsertListline (char *p_datasource
                                   ,CString& p_user
                                   ,CString& p_total
                                   ,CString& p_lasttime)
{
  int row = m_list.GetRowCount();
  m_list.InsertRow(p_datasource);
  InsertItem(row,1,p_user);
  InsertItem(row,2,p_total);
  InsertItem(row,3,p_lasttime);
  return 0;
}

void 
MultConnectionsDlg::InitGridEmpty(int p_type,bool nofirst /* = false */)
{
  if(!m_list)
  {
    return;
  }
	// Set standard
	m_list.SetEditable(FALSE);
	m_list.EnableDragAndDrop(TRUE);
	m_list.SetHeaderSort(TRUE);

  // Delete previous results
  while(m_list.GetRowCount() > 1)
  {
    m_list.DeleteRow(m_list.GetRowCount() - 1);
  }
  while(m_list.GetColumnCount() > 1)
  {
    m_list.DeleteColumn(m_list.GetColumnCount() - 1);
  }

  // Set empty grid
  try
  {
    m_list.SetRowCount(1); //nofirst ? 1 : 2);
    m_list.SetColumnCount(4);
		m_list.SetFixedRowCount(1);
  }
  catch(CMemoryException* e)
  {
    e->ReportError();
    e->Delete();
    return;
  }
  
  GV_ITEM item;
  item.mask = GVIF_TEXT | GVIF_FORMAT;
  item.row = 0;
  item.col = 0;
	item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX;

  item.strText = "Datasource";
  m_list.SetItem(&item);
  m_list.SetColumnWidth(0,200);

  item.col     = 1;
  item.strText = "User";
  m_list.SetItem(&item);
  m_list.SetColumnWidth(0,200);

  item.col     = 3;
  item.strText = "Last time";
  m_list.SetItem(&item);
  m_list.SetColumnWidth(0,200);

  item.col     = 2;
  item.strText = "Total";
  item.nFormat |= DT_SORT_NUMERIC;
  m_list.SetItem(&item);
  m_list.SetColumnWidth(0,120);

  // Sync with the outside world
	m_list.AutoSize();
  m_list.AutoSizeColumns(GVS_BOTH);
  m_list.ExpandLastColumn();
}

BOOL 
MultConnectionsDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  // Fill the connection dialog
  if(!m_list.Create(CRect(7,7,420,260)
                   ,this
                   ,IDC_CON_LIST
                   ,WS_CHILD | WS_TABSTOP | WS_VISIBLE))
  {
    TRACE0("Cannot create ODBC list");
  }
  InitGridEmpty(4);
  m_list.ShowWindow(SW_SHOW);
  m_list.SetSingleRowSelection(true);

  // Read in all settings
  CString mUser;
  CString mDatasource;
  CString mPassword;
  bool    mSafty   = 0;
  int     mSave    = 0;
  int     maxTimes = 0;

  ReadSettings();
  GetDatasourcesFromODBCManager();
  ConSetIter iter;
  for(iter  = m_settings.begin();
      iter != m_settings.end();
      ++iter)
  {
    int times = atol(iter->GetTimesUsed());
    if(times > maxTimes)
    {
      maxTimes    = times;
      mUser       = iter->GetUser();
      mDatasource = iter->GetDatasource();
      mPassword   = iter->GetPassword();
      mSafty      = iter->GetSafty();
    }
    char datasource[50];

    sprintf(datasource,iter->GetDatasource());
    // Insert into the ListCtrl
    InsertListline(datasource
                  ,iter->GetUser()
                  ,iter->GetTimesUsed()
                  ,iter->GetLastUsage());
  }
  m_list.AutoSizeColumns(GVS_BOTH);
  m_list.ExpandLastColumn();
  // Set the boxes with the maxvalue
  m_UserEdit     = mUser;
  m_UserPassword = mPassword;
  m_DataSource   = mDatasource;
  m_Safty        = mSafty;
  UpdateData(Data2Controls);

  // Leave the focus for the first input
  if(mPassword == "")
  {
    m_boxUserPassword.SetFocus();
  }
  else
  {
    m_boxUserEdit.SetFocus();
  }
  // Set the font for the list
  SetListFont();

  return true;
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
  struct tm* now = localtime(&ltime);
  CString moment;
  moment.Format("%4d-%02d-%02d %d:%02d:%02d"
               ,now->tm_year + 1900
               ,now->tm_mon  + 1
               ,now->tm_mday
               ,now->tm_hour
               ,now->tm_min
               ,now->tm_sec);
  return moment;
}

// If Dialog OK, Save our settings
void
MultConnectionsDlg::OnOK()
{
  UpdateData(Controls2Data);

  if(m_UserEdit == "" || m_UserPassword == "" || m_DataSource == "")
  {
    AfxMessageBox("For an ODBC login the values of (user,password,datasource) must be set");
    return;
  }
  CString timesUsed("1");
  CString lastUsage = Current();

#ifdef OQT_SAVE_PASSWORD
  CString password = m_UserPassword;
#else
  CString password;
#endif
  CString safty        = m_Safty ? "true" : "false";
  ConSetIter set = FindSetting(m_UserEdit,m_DataSource);
  if(set != m_settings.end())
  {
    // Set times used one (1) higher
    int times = atol(set->GetTimesUsed()) ;
    timesUsed.Format("%ld",times + 1);

    set->SetTimesUsed(timesUsed);
    set->SetLastUsage(lastUsage);
    set->SetPassword(password);
    set->SetSafty(m_Safty);
  }
  else
  {
    CConnectionDlgSetting setting(m_nextSection++
                                 ,m_UserEdit
                                 ,password
                                 ,m_DataSource
                                 ,lastUsage
                                 ,timesUsed
                                 ,safty);
    m_settings.push_back(setting);
  }
  SaveSettings();

  m_boxUserEdit.SetFocus();
  CDialog::OnOK();
}

void
MultConnectionsDlg::OnDetails()
{
  UpdateData(Controls2Data);
  EndDialog(IDC_CON_DETAILS);
}

bool 
MultConnectionsDlg::SaveSettings()
{
  int  ind = 1;
  CString number;
  ConSetIter iter;
  for(iter  = m_settings.begin();
      iter != m_settings.end();
      ++iter)
  {
    int section = iter->GetSection();
    if(section == 0)
    {
      section = m_nextSection++;
    }
    number.Format("%d",section);
    CCodeer cod;
    CString codedPassword = cod.CodeerString(iter->GetPassword(),iter->GetUser());
    char buffer[1024];
    DWORD size = 1024;
    CryptBinaryToString((BYTE*)codedPassword.GetString(),codedPassword.GetLength(),CRYPT_STRING_BASE64|CRYPT_STRING_NOCRLF,buffer,&size);

    CString safty = iter->GetSafty() ? "true" : "false";
    ::WritePrivateProfileString(number,"Datasource",  iter->GetDatasource(),   m_ODBC_iniFile);
    ::WritePrivateProfileString(number,"User",        iter->GetUser(),         m_ODBC_iniFile);
#ifdef OQT_SAVE_PASSWORD
    ::WritePrivateProfileString(number,"Password",    buffer,                  m_ODBC_iniFile);
#endif
    ::WritePrivateProfileString(number,"LastUsage",   iter->GetLastUsage(),    m_ODBC_iniFile);
    ::WritePrivateProfileString(number,"TimesUsed",   iter->GetTimesUsed(),    m_ODBC_iniFile);
    ::WritePrivateProfileString(number,"Safty",       safty,                   m_ODBC_iniFile);
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
  char buffer[100];
  CString datasource;
  CString user;
  CString password;
  CString lastUsage;
  CString timesUsed;
  CString safty;
  
  for(ind = 1; ind < MAX_ODBCSETTINGS; ++ind)
  {
    number.Format("%d",ind);
    ::GetPrivateProfileString(number,"Datasource",  "",buffer,100,m_ODBC_iniFile); datasource   = buffer;
    ::GetPrivateProfileString(number,"User",        "",buffer,100,m_ODBC_iniFile); user         = buffer;
#ifdef OQT_SAVE_PASSWORD
    ::GetPrivateProfileString(number,"Password",    "",buffer,100,m_ODBC_iniFile); password     = buffer;
#endif
    ::GetPrivateProfileString(number,"LastUsage",   "",buffer,100,m_ODBC_iniFile); lastUsage    = buffer;
    ::GetPrivateProfileString(number,"TimesUsed",   "",buffer,100,m_ODBC_iniFile); timesUsed    = buffer;
    ::GetPrivateProfileString(number,"Safty",       "",buffer,100,m_ODBC_iniFile); safty        = buffer;

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
      CConnectionDlgSetting setting(ind
                                   ,user
                                   ,decodedPassword
                                   ,datasource
                                   ,lastUsage
                                   ,timesUsed
                                   ,(safty == "true"));
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
      m_boxDataSource.AddString((const char *)DataSourceBuffer);
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

  CCellID cell = m_list.GetFocusCell();
  int index = cell.row;
  if(index > 0)
  {
    CString source = m_list.GetItemText(index,0);
    CString user   = m_list.GetItemText(index,1);
    ConSetIter set = FindSetting(user,source);
  
    if(set != m_settings.end())
    {
        SQLDatabase* database = NULL;
        CString      password = set->GetPassword();
        CString      connectStr;

        // Check for password
        password = password.GetLength() ? password : m_UserPassword;

        if(password.IsEmpty())
        {
          CString mesg("No password saved in the settings and no password givven on the dialog.\n"
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
        status.Format("Trying to connect to: %s",set->GetDatasource());

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
  CCellID cell = m_list.GetFocusCell();
  int row = cell.row;
  if(row > 0)
  {
    CString source = m_list.GetItemText(row,0);
    CString user   = m_list.GetItemText(row,1);
    ConSetIter set = FindSetting(user,source);
    if(set != m_settings.end())
    {
      CString question;
      question.Format("Do you want to delete the datasource '%s' for user '%s'?",source,user);
      if(WideMessageBox(m_hWnd,question,"ODBCQueryTool",MB_YESNO|MB_TASKMODAL|MB_DEFBUTTON2) != IDYES)
      {
        // Then don't do it
        return;
      }
      // Remove from the ini file
      CString num;
      int section = set->GetSection();
      num.Format("%d",section);
      ::WritePrivateProfileString(num,"Datasource",  "",m_ODBC_iniFile);
      ::WritePrivateProfileString(num,"User",        "",m_ODBC_iniFile);
      ::WritePrivateProfileString(num,"LastUsage",   "",m_ODBC_iniFile);
      ::WritePrivateProfileString(num,"TimesSaved",  "",m_ODBC_iniFile);
      ::WritePrivateProfileString(num,"SavePassword","",m_ODBC_iniFile);
      ::WritePrivateProfileString(num,"Safty",       "",m_ODBC_iniFile);
      // Remove from the settings registration
      m_settings.erase(set);
    }
    m_list.DeleteRow(row);
    // Sync with the outside world
	  m_list.AutoSize();
    m_list.AutoSizeColumns(GVS_BOTH);
    m_list.ExpandLastColumn();
    
    m_list.SetFocusCell(cell);
  }
}

CString
MultConnectionsDlg::GetLocalDirectory()
{
  CString directory = m_applicDirectory;

  if(m_vista)
  {
    // MS-Windows version 6 (Vista/Windows7)
    // Might swap local APPDATA (roaming for local)
    // 1) Strip "\\Roaming"
    // 2) add new appdata directory += "\\Local";
  }
  directory += "\\EDO\\OpenODBCQuerytool\\";
  return directory;
}

// MS-Windows Vista and above (Windows-7) should now use SHGetKnownFolderPath
// to correctly get the systemwide special folder names.
// And it's a lot prettier and shorter code at that too!
// void
// MultConnectionsDlg::FindApplicDirectory()
// {
//   USES_CONVERSION;
//   PWCHAR szPath;
// 
//   HRESULT res = SHGetKnownFolderPath(FOLDERID_RoamingAppData 
//                                     ,KF_FLAG_NO_ALIAS   // no %ENV% strings in result
//                                     ,NULL               // No Impersonation
//                                     ,&szPath);
//   if(SUCCEEDED(res))
//   {
//     // This was our goal
//     m_applicDirectory = CW2T(szPath);
//     // Free result
//     CoTaskMemFree(szPath);
//   }
// }

void 
MultConnectionsDlg::FindApplicDirectory()
{
  IMalloc*      pShellMalloc = NULL;    // A pointer to the shell's IMalloc interface
  IShellFolder* psfParent;              // A pointer to the parent folder object's IShellFolder interface.
  LPITEMIDLIST  pidlItem     = NULL;    // The item's PIDL.
  LPITEMIDLIST  pidlRelative = NULL;    // The item's PIDL relative to the parent folder.
  CHAR szPath[MAX_PATH] = "";           // The path for Favorites.
  STRRET str;                           // The structure for strings returned from IShellFolder.

  HRESULT hres = SHGetMalloc(&pShellMalloc);
  if (FAILED(hres))
  {
    return;
  }
  hres = SHGetSpecialFolderLocation(NULL,CSIDL_APPDATA,&pidlItem);
  if (SUCCEEDED(hres))
  {
    hres = SHBindToParent(pidlItem,
                          IID_IShellFolder,
                          (void**)&psfParent,
                          (LPCITEMIDLIST*)&pidlRelative);
    if (SUCCEEDED(hres))
    {
      // Retrieve the path
      memset(&str, 0, sizeof(str));
      hres = psfParent->GetDisplayNameOf(pidlRelative, SHGDN_NORMAL | SHGDN_FORPARSING, &str);
      if (SUCCEEDED(hres))
      {
        StrRetToBuf(&str, pidlItem, szPath, ARRAYSIZE(szPath));
      }
      psfParent->Release();
    }
  }
  // Clean up allocated memory
  if (pidlItem)
  {
    pShellMalloc->Free(pidlItem);
  }
  pShellMalloc->Release();

  m_applicDirectory = szPath;
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
        WideMessageBox(GetSafeHwnd(),"Cannot create directory: " + directory,"Error",MB_OK|MB_ICONERROR);
      }
    }
    directory += "\\OpenODBCQuerytool";
    if(_mkdir(directory))
    {
      int er = GetLastError();
      if(er != EEXIST && er != ERROR_ALREADY_EXISTS)
      {
        WideMessageBox(GetSafeHwnd(),"Cannot create directory: " + directory,"Error",MB_OK|MB_ICONERROR);
      }
    }
  }
}

/////////////////////////////
//
// ConnectionDlgSettings
//
/////////////////////////////

CConnectionDlgSetting::CConnectionDlgSetting(int    section
                                            ,CString user
                                            ,CString password
                                            ,CString datasource
                                            ,CString lastUsage
                                            ,CString timesUsed
                                            ,bool    safty)
                      :m_section(section)
                      ,m_user(user)
                      ,m_password(password)
                      ,m_datasource(datasource)
                      ,m_lastUsage(lastUsage)
                      ,m_timesUsed(timesUsed)
                      ,m_safty(safty)
{
}

CConnectionDlgSetting::~CConnectionDlgSetting()
{
}

void MultConnectionsDlg::OnCbnSelchangeConSafty()
{
}
