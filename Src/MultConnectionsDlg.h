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
// ConnectionDlg.h : header file
//
#include "Query/GridView.h"
#include "resource.h"
#include <sql.h>
#include <sqlext.h>
#include <vector>

#define MAX_ODBCSETTINGS  50

const bool Controls2Data = true;   // UpdateData
const bool Data2Controls = false;

/////////////////////////////////////////////////////////////////////////////
// MultConnectionsDlg dialog

class CConnectionDlgSetting
{
public:
   CConnectionDlgSetting(int    section
                        ,CString user
                        ,CString password
                        ,CString datasource
                        ,CString lastUsage
                        ,CString timesUsed
                        ,bool    safty);
  ~CConnectionDlgSetting();
  // Getters
  int     GetSection()      { return m_section;     }
  CString GetUser()         { return m_user;        }
  CString GetPassword()     { return m_password;    }
  CString GetDatasource()   { return m_datasource;  }
  CString GetLastUsage()    { return m_lastUsage;   }
  CString GetTimesUsed()    { return m_timesUsed;   }
  bool    GetSafty()        { return m_safty;       }
  // Setters
  void   SetSection(int section)    { m_section   = section; }
  void   SetPassword (CString word)  { m_password  = word;  }
  void   SetLastUsage(CString usage) { m_lastUsage = usage; }
  void   SetTimesUsed(CString used)  { m_timesUsed = used;  }
  void   SetSafty(bool safty)       { m_safty     = safty; }

private:
  int     m_section;
  bool    m_safty;
  CString m_user;
  CString m_password;
  CString m_datasource;
  CString m_lastUsage;
  CString m_timesUsed;
};

typedef std::vector<CConnectionDlgSetting>::iterator ConSetIter;

class MultConnectionsDlg : public CDialog
{
// Construction
public:
	MultConnectionsDlg(CWnd* pParent = NULL);   // standard constructor
  ~MultConnectionsDlg();                      // Standard destructor

  // Getters
  CString GetUser()         { return m_UserEdit;    }
  CString GetPassword()     { return m_UserPassword;}
  CString GetDataSource()   { return m_DataSource;  }
  bool    GetSafty()        { return m_Safty;       }
  
  // Setters
  void  SetUser(CString p_user)             { m_UserEdit      = p_user;       }
  void  SetPassword(CString p_password)     { m_UserPassword  = p_password;   } 
  void  SetDataSource(CString p_datasource) { m_DataSource    = p_datasource; }
  void  SetSafty(bool p_safty)              { m_Safty         = p_safty;      }

	enum { IDD = IDD_CONNECTION };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual BOOL OnInitDialog();
  virtual void OnClick_list(NMHDR* hdr,  LRESULT *pResult);
  virtual void OnDblclk_list(NMHDR* hdr, LRESULT *pResult);
  virtual void OnBnClicked_test();
  virtual void OnBnClicked_delete();
  virtual void OnOK();
  virtual void OnDetails();
	DECLARE_MESSAGE_MAP()
private:
  int InsertListline (char *p_datasource
                     ,CString& p_user
                     ,CString& p_total
                     ,CString& p_lasttime);
  void        InsertItem(int p_row,int p_col,CString& p_text,UINT p_format = 0);
  void        InitGridEmpty(int p_type,bool nofirst = false);
  bool        SaveSettings();
  bool        ReadSettings();
  ConSetIter  FindSetting(CString user,CString datasource);
  bool        GetDatasourcesFromODBCManager();
  CString     GetLocalDirectory();
  void        FindApplicDirectory();
  void        CreateINIDirectory();
  void        SetListFont();
  CString     Current();  
  
  CGridCtrl   m_list;
  CEdit       m_boxUserEdit;
  CEdit       m_boxUserPassword;
  CComboBox   m_boxDataSource;
  CComboBox   m_boxSafty;

  CString     m_UserEdit;
  CString     m_UserPassword;
  CString     m_DataSource;
  bool        m_Safty;
  int         m_nextSection;
  
  bool        m_vista; // MS-Vista and above
  CString     m_applicDirectory;
  CString     m_ODBC_iniFile;

  std::vector<CConnectionDlgSetting> m_settings;
public:
  afx_msg void OnCbnSelchangeConSafty();
};

