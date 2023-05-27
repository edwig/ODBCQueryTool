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
// ConnectionDlg.h : header file
//
#include "Query/GridView.h"
#include "MGridCtrl.h"
#include "resource.h"
#include <sql.h>
#include <sqlext.h>
#include <vector>

#define MAX_ODBCSETTINGS  50

const bool Controls2Data = true;   // UpdateData
const bool Data2Controls = false;

/////////////////////////////////////////////////////////////////////////////
// MultConnectionsDlg dialog

class ODBCSetting
{
public:
   ODBCSetting(int     p_section
              ,CString p_user
              ,CString p_password
              ,CString p_datasource
              ,CString p_lastUsage
              ,CString p_timesUsed
              ,bool    p_safty
              ,CString p_connString
              ,bool    p_useConnString
              ,bool    p_optUser
              ,bool    p_optPassword);
  ~ODBCSetting();
  // Getters
  int     GetSection()          { return m_section;             }
  CString GetUser()             { return m_user;                }
  CString GetPassword()         { return m_password;            }
  CString GetDatasource()       { return m_datasource;          }
  CString GetLastUsage()        { return m_lastUsage;           }
  CString GetTimesUsed()        { return m_timesUsed;           }
  CString GetConnString()       { return m_connString;          }
  bool    GetSafty()            { return m_safty;               }
  bool    GetUseConnection()    { return m_useConnectionString; }
  bool    GetOptionalUser()     { return m_optionalUser;        }
  bool    GetOptionalPassword() { return m_optionalPassword;    }
  // Setters
  void   SetSection(int section)          { m_section             = section; }
  void   SetPassword (CString word)       { m_password            = word;    }
  void   SetLastUsage(CString usage)      { m_lastUsage           = usage;   }
  void   SetTimesUsed(CString used)       { m_timesUsed           = used;    }
  void   SetConnString(CString str)       { m_connString          = str;     }
  void   SetSafty(bool safty)             { m_safty               = safty;   }
  void   SetUseConnection(bool p_use)     { m_useConnectionString = p_use;   }
  void   SetOptionalUser(bool p_opt)      { m_optionalUser        = p_opt;   }
  void   SetOptionalPassword(bool p_opt)  { m_optionalPassword    = p_opt;   }

private:
  int     m_section;
  bool    m_safty;
  CString m_user;
  CString m_password;
  CString m_datasource;
  CString m_lastUsage;
  CString m_timesUsed;
  CString m_connString;

  bool    m_useConnectionString;
  bool    m_optionalUser;
  bool    m_optionalPassword;
};

using ODBCSettings = std::vector<ODBCSetting>;

class MultConnectionsDlg : public StyleDialog
{
// Construction
public:
	MultConnectionsDlg(CWnd* pParent = NULL);   // standard constructor
 ~MultConnectionsDlg();                       // Standard destructor

  bool SaveSettings(CString p_datasource
                   ,CString p_user
                   ,CString p_password
                   ,bool    p_safty
                   ,CString p_connString
                   ,bool    p_useConnString
                   ,bool    p_optionalUser
                   ,bool    p_optionalPassword);

  // Getters
  CString GetUser()             { return m_UserEdit;          }
  CString GetPassword()         { return m_UserPassword;      }
  CString GetDataSource()       { return m_DataSource;        }
  bool    GetSafty()            { return m_Safty;             }
  CString GetConnString()       { return m_connString;        }
  bool    GetUseConnString()    { return m_useConnString;     }
  bool    GetOptionalUser()     { return m_optionalUser;      }
  bool    GetOptionalPassword() { return m_optionalPassword;  }
  
  // Setters
  void  SetUser(CString p_user)             { m_UserEdit      = p_user;       }
  void  SetPassword(CString p_password)     { m_UserPassword  = p_password;   } 
  void  SetDataSource(CString p_datasource) { m_DataSource    = p_datasource; }
  void  SetSafty(bool p_safty)              { m_Safty         = p_safty;      }
  void  SetConnString(CString p_connString) { m_connString    = p_connString; }

	enum { IDD = IDD_CONNECTION };

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;

  DECLARE_MESSAGE_MAP()

private:
  virtual void SetupDynamicLayout() override;

  int InsertListline (CString& p_datasource
                     ,CString& p_user
                     ,CString& p_total
                     ,CString  p_hasConnString
                     ,CString& p_lasttime);
  void        InsertItem(int p_row,int p_col,CString& p_text,UINT p_format = 0);
  void        InitGridEmpty();
  bool        WriteSettings();
  bool        ReadSettings();
  void        FillGridWithSettings();
  ODBCSetting* FindSetting(CString user,CString datasource);
  bool        RemoveSetting(ODBCSetting* p_setting);
  bool        GetDatasourcesFromODBCManager();
  CString     GetLocalDirectory();
  void        CreateINIDirectory();
  void        SetListFont();
  CString     Current();  
  
  MCGridCtrl    m_list;
  StyleEdit     m_boxUserEdit;
  StyleEdit     m_boxUserPassword;
  StyleComboBox m_comboDataSource;
  StyleComboBox m_comboSafty;
  StyleButton   m_buttonConString;
  StyleButton   m_buttonTest;
  StyleButton   m_buttonDelete;
  StyleButton   m_buttonDetails;
  StyleButton   m_buttonHelp;
  StyleButton   m_buttonOK;
  StyleButton   m_buttonCancel;

  CString       m_UserEdit;
  CString       m_UserPassword;
  CString       m_DataSource;
  bool          m_Safty;
  CString       m_connString;
  bool          m_useConnString;
  bool          m_optionalUser;
  bool          m_optionalPassword;
  int           m_nextSection;
  CString       m_applicDirectory;
  CString       m_ODBC_iniFile;

  ODBCSettings  m_settings;
public:
  afx_msg void OnCbnSelchangeConSafty();
  afx_msg void OnClick_list(NMHDR* hdr,LRESULT* pResult);
  afx_msg void OnDblclk_list(NMHDR* hdr,LRESULT* pResult);
  afx_msg void OnBnClicked_test();
  afx_msg void OnBnClicked_delete();
  afx_msg void OnBnClickedConString();
  afx_msg void OnOK();
  afx_msg void OnDetails();
};

