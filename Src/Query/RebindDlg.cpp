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
#include "RebindDlg.h"
#include "OpenEditorApp.h"
#include "SQLVariant.h"

DataTypes allTypes[] = 
{
   { "<NO TYPE>",                  0                               }
  ,{ "CHAR",                       SQL_C_CHAR                      } 
  ,{ "SHORT",                      SQL_C_SHORT                     }
  ,{ "SIGNED SHORT",               SQL_C_SSHORT                    }
  ,{ "UNSIGNED SHORT",             SQL_C_USHORT                    }
  ,{ "LONG",                       SQL_C_LONG                      }
  ,{ "SIGNED LONG",                SQL_C_SLONG                     }
  ,{ "UNSIGNED LONG",              SQL_C_ULONG                     }
  ,{ "REAL",                       SQL_C_FLOAT                     }
  ,{ "FLOAT",                      SQL_C_FLOAT                     }
  ,{ "DOUBLE",                     SQL_C_DOUBLE                    }
  ,{ "BIT",                        SQL_C_BIT                       }
  ,{ "TINYINT",                    SQL_C_TINYINT                   }
  ,{ "SIGNED TINYINT",             SQL_C_STINYINT                  }
  ,{ "UNSIGNED TINYINT",           SQL_C_UTINYINT                  }
  ,{ "SIGNED BIGINT",              SQL_C_SBIGINT                   }
  ,{ "UNSIGNED BIGINT",            SQL_C_UBIGINT                   }
  ,{ "DECIMAL",                    SQL_C_NUMERIC                   }
  ,{ "NUMERIC",                    SQL_C_NUMERIC                   }
  ,{ "GUID",                       SQL_C_GUID                      }
  ,{ "BINARY",                     SQL_C_BINARY                    }
  ,{ "DATE",                       SQL_C_DATE                      }
  ,{ "TIME",                       SQL_C_TIME                      }
  ,{ "TIMESTAMP",                  SQL_C_TIMESTAMP                 }
  ,{ "TYPE DATE",                  SQL_C_TYPE_DATE                 }
  ,{ "TYPE TIME",                  SQL_C_TYPE_TIME                 }
  ,{ "TYPE TIMESTAMP",             SQL_C_TYPE_TIMESTAMP            }
  ,{ "INTERVAL YEAR",              SQL_C_INTERVAL_YEAR             }
  ,{ "INTERVAL MONTH",             SQL_C_INTERVAL_MONTH            }
  ,{ "INTERVAL DAY",               SQL_C_INTERVAL_DAY              }
  ,{ "INTERVAL HOUR",              SQL_C_INTERVAL_HOUR             }
  ,{ "INTERVAL MINUTE",            SQL_C_INTERVAL_MINUTE           }
  ,{ "INTERVAL SECOND",            SQL_C_INTERVAL_SECOND           }
  ,{ "INTERVAL YEAR TO MONTH",     SQL_C_INTERVAL_YEAR_TO_MONTH    }
  ,{ "INTERVAL DAY TO HOUR",       SQL_C_INTERVAL_DAY_TO_HOUR      }
  ,{ "INTERVAL DAY TO MINUTE",     SQL_C_INTERVAL_DAY_TO_MINUTE    }
  ,{ "INTERVAL DAY TO SECOND",     SQL_C_INTERVAL_DAY_TO_SECOND    }
  ,{ "INTERVAL HOUR TO MINUTE",    SQL_C_INTERVAL_HOUR_TO_MINUTE   }
  ,{ "INTERVAL HOUR TO SECOND",    SQL_C_INTERVAL_HOUR_TO_SECOND   }
  ,{ "INTERVAL MINUTE TO SECOND",  SQL_C_INTERVAL_MINUTE_TO_SECOND }
  ,{ NULL,                         0                               }
};

// Names must appear in this order to work properly!!
DataTypes allParams[] = 
{
  { "<UNKNOWN>", SQL_PARAM_TYPE_UNKNOWN }    // 0
 ,{ "INPUT",     SQL_PARAM_INPUT        }    // 1
 ,{ "INOUT",     SQL_PARAM_INPUT_OUTPUT }    // 2
 ,{ "COLUMN",    SQL_RESULT_COL         }    // 3
 ,{ "OUTPUT",    SQL_PARAM_OUTPUT       }    // 4
 ,{ "RETURN",    SQL_RETURN_VALUE       }    // 5
 ,{ NULL,        0                      }
};

// RebindDlg dialog

IMPLEMENT_DYNAMIC(RebindDlg, CDialog)

RebindDlg::RebindDlg(CWnd* pParent /*=NULL*/)
          :CDialog(RebindDlg::IDD, pParent)
{
}

RebindDlg::~RebindDlg()
{
}

void RebindDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_DATATYPE, m_typeCombo);
  DDX_Control(pDX,IDC_REBIND,   m_rebindCombo);
}


BEGIN_MESSAGE_MAP(RebindDlg, CDialog)
  ON_CBN_SELCHANGE(IDC_DATATYPE,&RebindDlg::OnCbnSelchangeDatatype)
  ON_CBN_SELCHANGE(IDC_REBIND,  &RebindDlg::OnCbnSelchangeRebind)
  ON_BN_CLICKED   (IDC_RESET,   &RebindDlg::OnBnClickedReset)
  ON_BN_CLICKED   (IDC_ADD,     &RebindDlg::OnBnClickedAdd)
  ON_BN_CLICKED   (IDOK,        &RebindDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL
RebindDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  DataTypes* types = allTypes;
  while(types->name)
  {
    m_typeCombo.AddString(types->name);
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
    m_type = var.FindDatatype((char*)type.GetString());
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
    m_rebind = var.FindDatatype((char*)type.GetString());
  }
}

void 
RebindDlg::OnBnClickedReset()
{
  if(AfxMessageBox("Clear the rebind mappings?",MB_YESNO|MB_DEFBUTTON2) == IDYES)
  {
    RebindMap* rebinds = theApp.GetRebinds();
    rebinds->clear();
    AfxMessageBox("Cleared all rebindings of datatypes!",MB_OK|MB_ICONINFORMATION);
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
    CString typeName   = var.FindDatatype(m_type);
    CString rebindName = var.FindDatatype(m_rebind);
    CString msg;
    msg.Format("Rebind datatype [%s] to [%s]",typeName,rebindName);
    AfxMessageBox(msg,MB_OK);
  }
}

void 
RebindDlg::OnBnClickedOk()
{
  OnOK();
}
