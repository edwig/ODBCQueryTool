////////////////////////////////////////////////////////////////////////
//
// File: QueryReWriterDlg.cpp
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
#include "pch.h"
#include "framework.h"
#include "QueryReWriter.h"
#include "QueryReWriterDlg.h"
#include "QueryDlg.h"
#include "ConfigDlg.h"
#include <RegistryManager.h>
#include <afxdialogex.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// AboutDlg dialog used for App About

class AboutDlg : public StyleDialog
{
public:
	AboutDlg(CWnd* p_parent);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;

// Implementation
protected:
  DECLARE_MESSAGE_MAP()

  CString m_version;
  CString m_copyright;

  StyleEdit   m_editVersion;
  StyleEdit   m_editCopyright;
  StyleButton m_buttonOk;
};

AboutDlg::AboutDlg(CWnd* p_parent)
          :StyleDialog(IDD_ABOUTBOX,p_parent)
{
}

void AboutDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_VERSION,  m_editVersion,  m_version);
  DDX_Control(pDX,IDC_COPYRIGHT,m_editCopyright,m_copyright);
  DDX_Control(pDX,IDOK,         m_buttonOk);
}

BEGIN_MESSAGE_MAP(AboutDlg, StyleDialog)
END_MESSAGE_MAP()

BOOL
AboutDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();
  SetWindowText(_T("About Query ReWriter"));

  m_buttonOk.SetStyle(_T("ok"));
  m_version   = _T("Query ReWriter. Version: ")  SQL_COMPONENTS_VERSION;
  m_copyright = SQL_COMPONENTS_COPYRIGHT;
  UpdateData(FALSE);

  m_buttonOk.SetFocus();
  return FALSE;
}

// QueryReWriterDlg dialog

QueryReWriterDlg::QueryReWriterDlg(CWnd* pParent /*=nullptr*/)
	               :StyleDialog(IDD_QUERYREWRITER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

  m_page1 = new QueryDlg(this);
  m_page2 = new ConfigDlg(this);
}

QueryReWriterDlg::~QueryReWriterDlg()
{
  delete m_page1;
  delete m_page2;
}


void 
QueryReWriterDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);

  DDX_Control(pDX,IDC_CONFIG,m_editConfig,m_config);
  DDX_Control(pDX,IDC_TAB,   m_tabs);
}

BEGIN_MESSAGE_MAP(QueryReWriterDlg, StyleDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
  ON_EN_CHANGE(IDC_CONFIG,              OnEnChangeConfig)

  ON_COMMAND  (ID_MENU_THEMELIME,       OnStyleLime)
  ON_COMMAND  (ID_MENU_THEMESKYBLUE,    OnStyleSkyblue)
  ON_COMMAND  (ID_MENU_THEMEPURPLE,     OnStylePurple)
  ON_COMMAND  (ID_MENU_THEMEMUSTARD,    OnStyleMustard)
  ON_COMMAND  (ID_MENU_THEMEMODERATE,   OnStyleModerateGray)
  ON_COMMAND  (ID_MENU_THEMEPURE,       OnStylePureGray)
  ON_COMMAND  (ID_MENU_THEMEBLACK,      OnStyleBlackWhite)
  ON_COMMAND  (ID_MENU_THEMEDARK,       OnStyleDark)
  ON_COMMAND  (ID_MENU_ABOUT,           OnAbout)
  ON_COMMAND  (ID_MENU_EXIT,            OnExit)

END_MESSAGE_MAP()

// QueryReWriterDlg message handlers

BOOL 
QueryReWriterDlg::OnInitDialog()
{
	StyleDialog::OnInitDialog();
  SetWindowText(_T("Query ReWriter"));
  ShowMinMaxButton();
  SetSysMenu(IDR_MENU);
  SetAboutBoxAndIcon(IDM_ABOUTBOX,IDS_ABOUTBOX);

	// Add extra initialization here
  InitTabs();
  InitConfig();
  SetCanResize();

  UpdateData(FALSE);
  m_tabs.SetFocus();
	return FALSE;  // return TRUE  unless you set the focus to a control
}

void
QueryReWriterDlg::SetupDynamicLayout()
{
  StyleDialog::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif
  manager.AddItem(IDC_CONFIG,CMFCDynamicLayout::MoveNone(),CMFCDynamicLayout::SizeHorizontal(100));
  manager.AddItem(IDC_TAB,   CMFCDynamicLayout::MoveNone(),CMFCDynamicLayout::SizeHorizontalAndVertical(100,100));
}

void
QueryReWriterDlg::InitTabs()
{
  m_page1->Create(IDD_QUERIES,&m_tabs);
  m_page2->Create(IDD_CONFIG, &m_tabs);

  m_tabs.InsertItem(0,m_page1,_T("Query"));
  m_tabs.InsertItem(1,m_page2,_T("Configuration"));

  m_tabs.Init();
}

void
QueryReWriterDlg::InitConfig()
{
  RegistryManager manager;
  CString config(_T("HKCU\\Software\\EDO\\QueryRewriter"));
  CString file = manager.GetRegistryString(config,_T("configfile"),_T(""));
  if(!file.IsEmpty())
  {
    m_page2->SetConfigFile(file);
    if(!m_page2->ReadConfig())
    {
      manager.DeleteRegistryValue(config,_T("configfile"));
    }
  }
}

void 
QueryReWriterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		AboutDlg dlgAbout(this);
		dlgAbout.DoModal();
    return;
	}
  else if(nID == SC_CLOSE)
  {
    m_page2->WriteConfig();
  }
  StyleDialog::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void 
QueryReWriterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR 
QueryReWriterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void 
QueryReWriterDlg::SetConfig(CString p_config)
{
  m_config = p_config;
  UpdateData(FALSE);
}

void 
QueryReWriterDlg::OnEnChangeConfig()
{
  UpdateData();
}

void 
QueryReWriterDlg::OnAbout()
{
  AboutDlg dlg(this);
  dlg.DoModal();
}

void 
QueryReWriterDlg::OnExit()
{
  PostMessage(WM_SYSCOMMAND,SC_CLOSE,0);
}
