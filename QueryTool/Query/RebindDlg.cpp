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
#include "RebindDlg.h"
#include "QueryTool.h"
#include <SQLVariant.h>
#include <SQLDataType.h>

DataTypes allTypes[] = 
{
   { _T("<NO TYPE>"),                  0                               }
  ,{ _T("CHAR"),                       SQL_C_CHAR                      } 
  ,{ _T("WCHAR"),                      SQL_C_WCHAR                     } 
  ,{ _T("SHORT"),                      SQL_C_SHORT                     }
  ,{ _T("SIGNED SHORT"),               SQL_C_SSHORT                    }
  ,{ _T("UNSIGNED SHORT"),             SQL_C_USHORT                    }
  ,{ _T("LONG"),                       SQL_C_LONG                      }
  ,{ _T("SIGNED LONG"),                SQL_C_SLONG                     }
  ,{ _T("UNSIGNED LONG"),              SQL_C_ULONG                     }
  ,{ _T("REAL"),                       SQL_C_FLOAT                     }
  ,{ _T("FLOAT"),                      SQL_C_FLOAT                     }
  ,{ _T("DOUBLE"),                     SQL_C_DOUBLE                    }
  ,{ _T("BIT"),                        SQL_C_BIT                       }
  ,{ _T("TINYINT"),                    SQL_C_TINYINT                   }
  ,{ _T("SIGNED TINYINT"),             SQL_C_STINYINT                  }
  ,{ _T("UNSIGNED TINYINT"),           SQL_C_UTINYINT                  }
  ,{ _T("SIGNED BIGINT"),              SQL_C_SBIGINT                   }
  ,{ _T("UNSIGNED BIGINT"),            SQL_C_UBIGINT                   }
  ,{ _T("DECIMAL"),                    SQL_C_NUMERIC                   }
  ,{ _T("NUMERIC"),                    SQL_C_NUMERIC                   }
  ,{ _T("GUID"),                       SQL_C_GUID                      }
  ,{ _T("BINARY"),                     SQL_C_BINARY                    }
  ,{ _T("DATE"),                       SQL_C_DATE                      }
  ,{ _T("TIME"),                       SQL_C_TIME                      }
  ,{ _T("TIMESTAMP"),                  SQL_C_TIMESTAMP                 }
  ,{ _T("TYPE DATE"),                  SQL_C_TYPE_DATE                 }
  ,{ _T("TYPE TIME"),                  SQL_C_TYPE_TIME                 }
  ,{ _T("TYPE TIMESTAMP"),             SQL_C_TYPE_TIMESTAMP            }
  ,{ _T("INTERVAL YEAR"),              SQL_C_INTERVAL_YEAR             }
  ,{ _T("INTERVAL MONTH"),             SQL_C_INTERVAL_MONTH            }
  ,{ _T("INTERVAL DAY"),               SQL_C_INTERVAL_DAY              }
  ,{ _T("INTERVAL HOUR"),              SQL_C_INTERVAL_HOUR             }
  ,{ _T("INTERVAL MINUTE"),            SQL_C_INTERVAL_MINUTE           }
  ,{ _T("INTERVAL SECOND"),            SQL_C_INTERVAL_SECOND           }
  ,{ _T("INTERVAL YEAR TO MONTH"),     SQL_C_INTERVAL_YEAR_TO_MONTH    }
  ,{ _T("INTERVAL DAY TO HOUR"),       SQL_C_INTERVAL_DAY_TO_HOUR      }
  ,{ _T("INTERVAL DAY TO MINUTE"),     SQL_C_INTERVAL_DAY_TO_MINUTE    }
  ,{ _T("INTERVAL DAY TO SECOND"),     SQL_C_INTERVAL_DAY_TO_SECOND    }
  ,{ _T("INTERVAL HOUR TO MINUTE"),    SQL_C_INTERVAL_HOUR_TO_MINUTE   }
  ,{ _T("INTERVAL HOUR TO SECOND"),    SQL_C_INTERVAL_HOUR_TO_SECOND   }
  ,{ _T("INTERVAL MINUTE TO SECOND"),  SQL_C_INTERVAL_MINUTE_TO_SECOND }
  ,{ NULL,                         0                               }
};

// Names must appear in this order to work properly!!
DataTypes allParams[] = 
{
  { _T("<UNKNOWN>"), SQL_PARAM_TYPE_UNKNOWN }    // 0
 ,{ _T("INPUT"),     SQL_PARAM_INPUT        }    // 1
 ,{ _T("INOUT"),     SQL_PARAM_INPUT_OUTPUT }    // 2
 ,{ _T("COLUMN"),    SQL_RESULT_COL         }    // 3
 ,{ _T("OUTPUT"),    SQL_PARAM_OUTPUT       }    // 4
 ,{ _T("RETURN"),    SQL_RETURN_VALUE       }    // 5
 ,{ NULL,        0                      }
};

// RebindDlg dialog

IMPLEMENT_DYNAMIC(RebindDlg, StyleDialog)

RebindDlg::RebindDlg(CWnd* pParent /*=NULL*/)
          :StyleDialog(RebindDlg::IDD, pParent)
{
}

RebindDlg::~RebindDlg()
{
}

void RebindDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_DATATYPE, m_typeCombo);
  DDX_Control(pDX,IDC_REBIND,   m_rebindCombo);
  DDX_Control(pDX,IDC_RESET,    m_buttonReset);
  DDX_Control(pDX,IDC_ADD,      m_buttonAdd);
  DDX_Control(pDX,IDOK,         m_buttonOK);
  DDX_Control(pDX,IDCANCEL,     m_buttonCancel);
}

BEGIN_MESSAGE_MAP(RebindDlg, StyleDialog)
  ON_CBN_SELCHANGE(IDC_DATATYPE,&RebindDlg::OnCbnSelchangeDatatype)
  ON_CBN_SELCHANGE(IDC_REBIND,  &RebindDlg::OnCbnSelchangeRebind)
  ON_BN_CLICKED   (IDC_RESET,   &RebindDlg::OnBnClickedReset)
  ON_BN_CLICKED   (IDC_ADD,     &RebindDlg::OnBnClickedAdd)
  ON_BN_CLICKED   (IDOK,        &RebindDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL
RebindDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();
  SetWindowText(_T("Rebind datatype"));

  DataTypes* types = allTypes;
  while(types->name)
  {
    m_typeCombo  .AddString(types->name);
    m_rebindCombo.AddString(types->name);
    ++types;
  }
  return TRUE;
}

// RebindDlg message handlers

void 
RebindDlg::OnCbnSelchangeDatatype()
{
  int num = m_typeCombo.GetCurSel();
  if(num >= 0)
  {
    CString type;
    m_typeCombo.GetLBText(num,type);
    SQLVariant var;
    m_type = SQLDataType::FindDatatype((TCHAR*)type.GetString());
  }
}

void 
RebindDlg::OnCbnSelchangeRebind()
{
  int num = m_rebindCombo.GetCurSel();
  if(num >= 0)
  {
    CString type;
    m_rebindCombo.GetLBText(num,type);
    SQLVariant var;
    m_rebind = SQLDataType::FindDatatype((TCHAR*)type.GetString());
  }
}

void 
RebindDlg::OnBnClickedReset()
{
  if(AfxMessageBox(_T("Clear the rebind mappings?"),MB_YESNO|MB_DEFBUTTON2) == IDYES)
  {
    RebindMap* rebinds = theApp.GetRebinds();
    rebinds->clear();
    AfxMessageBox(_T("Cleared all rebindings of datatypes!"),MB_OK|MB_ICONINFORMATION);
  }
}

void 
RebindDlg::OnBnClickedAdd()
{
  if(m_type > 0 && m_rebind > 0)
  {
    RebindMap* rebinds = theApp.GetRebinds();
    rebinds->insert(std::make_pair(m_type,m_rebind));

    SQLVariant var;
    CString typeName   = SQLDataType::FindDatatype(m_type);
    CString rebindName = SQLDataType::FindDatatype(m_rebind);
    CString msg;
    msg.Format(_T("Rebind datatype [%s] to [%s]"),typeName.GetString(),rebindName.GetString());
    AfxMessageBox(msg,MB_OK);
  }
}

void 
RebindDlg::OnBnClickedOk()
{
  OnOK();
}
