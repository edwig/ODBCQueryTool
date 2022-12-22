////////////////////////////////////////////////////////////////////////
//
// File: MainFrame.cpp
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
#include "QueryTool.h"
#include "MainFrm.h"
#include "OEDocument.h"
#include "OEView.h"
#include "OEStreams.h"
#include "OESettingsStreams.h"
#include "Common\AppUtilities.h"
#include "Common\AppGlobal.h"
#include "OEFindFiles.h"
#include "ChildFrm.h"
#include "DonateDlg.h"
#include <GetExePath.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

LPCSTR CMainFrame::m_cszClassName = "ODBCQueryTool.OpenEditor";
SettingsManager CMainFrame::m_settingsManager;

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, StyleMDIFrameWnd)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, StyleMDIFrameWnd)
  ON_WM_CREATE()
  ON_WM_TIMER()
  ON_WM_COPYDATA()
  ON_WM_ACTIVATEAPP()
  ON_WM_SETTINGCHANGE()

  ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
  ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
  ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
  ON_REGISTERED_MESSAGE(AFX_WM_RESETTOOLBAR, OnToolbarReset)
  ON_COMMAND_RANGE          (ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
  ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)

  ON_COMMAND(ID_FILE_FIND_IN_FILE,            OnFindFiles)
  ON_COMMAND(ID_VIEW_FIND_PANEL,              OnViewFindPanel)
  ON_COMMAND(ID_VIEW_CONNECTBAR,              OnConnectBar)
  ON_COMMAND(ID_VIEW_DATABASEBAR,             OnDatabaseBar)
  ON_COMMAND(ID_TOOLS_SQLMIGRATION,           OnSQLMigration)
  ON_COMMAND(ID_HELP_BUG,                     OnSupportBug)
  ON_COMMAND(ID_HELP_FEATURE,                 OnSupportFeature)
  ON_COMMAND(ID_HELP_SFNET,                   OnWebSourceforge)
  ON_COMMAND(ID_HELP_GITHUB,                  OnWebGithub)
	ON_COMMAND(ID_DONATION,                     OnDonation)
  ON_COMMAND(ID_FILE_SYNC_LOCATION,           OnViewFilePanelSync)
  ON_COMMAND(ID_THEMA_LIME,                   OnStyleLime)
  ON_COMMAND(ID_THEMA_SKYBLUE,                OnStyleSkyblue)
  ON_COMMAND(ID_THEMA_PURPLE,                 OnStylePurple)
  ON_COMMAND(ID_THEMA_MODERATE,               OnStyleModerateGray)
  ON_COMMAND(ID_THEMA_PUREGRAY,               OnStylePureGray)
  ON_COMMAND(ID_THEMA_BLACKWHITE,             OnStyleBlackWhite)

  ON_UPDATE_COMMAND_UI(ID_FILE_SYNC_LOCATION, OnUpdateViewFilePanelSync)
  ON_UPDATE_COMMAND_UI(ID_ODBC_CONNECT,       OnUpdateConnect)
  ON_UPDATE_COMMAND_UI(ID_ODBC_DISCONNECT,    OnUpdateDisconnect)
  ON_UPDATE_COMMAND_UI(ID_ODBC_BEGIN,         OnUpdateBegin)
  ON_UPDATE_COMMAND_UI(ID_ODBC_COMMIT,        OnUpdateCommit)
  ON_UPDATE_COMMAND_UI(ID_ODBC_ROLLBACK,      OnUpdateRollback)
  ON_UPDATE_COMMAND_UI(ID_SESSION_ODBCREPORT, OnUpdateODBCReport)
  ON_UPDATE_COMMAND_UI(ID_VIEW_CONNECTBAR,    OnUpdateConnectBarVisible)
  ON_UPDATE_COMMAND_UI(ID_VIEW_DATABASEBAR,   OnUpdateDatabaseBarVisible)
  ON_UPDATE_COMMAND_UI(ID_SESSIONSTATUS,      OnUpdateSessionStatus)
  ON_UPDATE_COMMAND_UI(ID_VIEW_FIND_PANEL,    OnUpdateViewFindPanel)
END_MESSAGE_MAP()

static UINT indicators[] =
{
    ID_SEPARATOR,           // status line indicator
    ID_INDICATOR_FILE_TYPE,
    ID_INDICATOR_BLOCK_TYPE,
    ID_INDICATOR_CUR_CHAR,
    ID_INDICATOR_FILE_LINES,
    ID_INDICATOR_POS,
    ID_INDICATOR_SCROLL_POS,
    ID_INDICATOR_OVR,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame() noexcept
           :m_conBarVisible(true)
           ,m_dbBarVisible(true)
{
	// TODO: add member initialization code here
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (StyleMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	DWORD ctrlStyle = TBSTYLE_FLAT | CBRS_TOOLTIPS | CBRS_SIZE_DYNAMIC;
	DWORD dwStyle   = AFX_DEFAULT_TOOLBAR_STYLE;
  CSize imageSize (TOOLBAR_ICON_X,  TOOLBAR_ICON_Y);
  CSize buttonSize(TOOLBAR_BUTTON_X,TOOLBAR_BUTTON_Y);
	CMFCToolBarInfo wtbi;
	CMFCToolBarInfo dtbi;
	CMFCToolBarInfo ctbi;
	const CRect rect(1,1,1,1);

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // other styles available...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;					// set to FALSE to place close button at right of tab area
	mdiTabParams.m_bTabIcons     = FALSE;   // set to TRUE to enable document icons on MDI tabs
	mdiTabParams.m_bAutoColor    = TRUE;    // set to FALSE to disable auto-coloring of MDI tabs
	mdiTabParams.m_bDocumentMenu = TRUE;		// enable the document menu at the right edge of the tab area
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);
	// prevent the menu bar from taking the focus on activation
  CMFCPopupMenu::SetForceMenuFocus(FALSE);
	CMFCMenuBar::SetShowAllCommands(TRUE);


	if (!m_wndToolBar.CreateEx(this,ctrlStyle,dwStyle,rect,IDR_OPENEDITOR) ||
		  !m_wndToolBar.LoadToolBarEx(IDR_OPENEDITOR,wtbi,TRUE)) 
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
  m_wndToolBar.SetMenuSizes(buttonSize, imageSize);
  m_wndToolBar.SetHeight(TOOLBAR_HEIGHT);
	m_wndToolBar.SetWindowText("Standard");
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, "Customize");


  // Database toolbar
  if (!m_dbToolBar.CreateEx(this, ctrlStyle, dwStyle, rect, IDR_DATABASE) ||
		  !m_dbToolBar.LoadToolBarEx(IDR_DATABASE,dtbi,TRUE))
  {
    TRACE0("Failed to create database toolbar\n");
    return -1;      // fail to create
  }
  m_dbToolBar.SetMenuSizes(buttonSize, imageSize);
  m_dbToolBar.SetHeight(TOOLBAR_HEIGHT);
	m_dbToolBar.SetWindowTextA("Database");


	// Connection toolbar
  if(!m_conToolBar.CreateEx(this, ctrlStyle,dwStyle,rect,IDR_CONNECTBAR) ||
     !m_conToolBar.LoadToolBarEx(IDR_CONNECTBAR,ctbi,TRUE))
	{
    TRACE0("Failed to create database toolbar\n");
		return -1;      // fail to create
  }
	m_conToolBar.SetMenuSizes(buttonSize, imageSize);
	m_conToolBar.SetHeight(TOOLBAR_HEIGHT);
	m_conToolBar.SetWindowText("Connection");


	// Allow user-defined toolbars operations:
	InitUserToolbars(nullptr, uiFirstUserToolBarId, uiLastUserToolBarId);

	// Create the status bar
	SetGrayWindowStatusBarHeight(ODBC_STATUSBAR_HEIGHT);
	if(!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
  // Connect status bar
  Common::SetStatusHwnd(m_wndStatusBar.m_hWnd);

	// TODO: Delete these five lines if you don't want the toolbar and menubar to be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_TOP);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_dbToolBar .EnableDocking(CBRS_ALIGN_ANY);
	m_conToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
  DockPane(&m_wndMenuBar);
	DockPane(&m_dbToolBar);
	DockPaneLeftOf(&m_conToolBar, &m_dbToolBar);
	DockPaneLeftOf(&m_wndToolBar, &m_conToolBar);

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// Load menu item image (not placed on any standard toolbars):
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// create docking windows
	if (!CreateDockingWindows())
	{
		TRACE0("Failed to create docking windows\n");
		return -1;
	}

	m_wndObjectView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndInfoView  .EnableDocking(CBRS_ALIGN_ANY);
	m_wndDirView   .EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndObjectView);
	CDockablePane* pTabbedBar = nullptr;
	CDockablePane* pTab2Bar = nullptr;
	m_wndInfoView.AttachToTabWnd(&m_wndObjectView,DM_SHOW,TRUE,&pTabbedBar);
	m_wndDirView .AttachToTabWnd(&m_wndObjectView,DM_SHOW,TRUE,&pTab2Bar);
	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput);
	ShowPane(&m_wndOutput,FALSE,FALSE,TRUE);

	// set the visual manager and style based on persisted value
	OnApplicationLook(theApp.m_nAppLook);

	// Enable enhanced windows management dialog
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// Enable toolbar and docking window menu replacement
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, "Customize", ID_VIEW_TOOLBAR);

	// enable quick (Alt+drag) toolbar customization
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == nullptr)
	{
		// load user-defined toolbar images
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

  // Use as system menu
  SetSysMenu(IDR_SYSMENU);

	// enable menu personalization (most-recently used commands)
	// TODO: define your own basic commands, ensuring that each pulldown menu has at least one basic command.
	CList<UINT, UINT> lstBasicCommands;

	// File menu
	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_RELOAD);
	lstBasicCommands.AddTail(ID_FILE_CLOSE);
	lstBasicCommands.AddTail(ID_FILE_CLOSE_ALL);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_SAVE_AS);
	lstBasicCommands.AddTail(ID_FILE_SAVE_ALL);
	lstBasicCommands.AddTail(ID_FILE_SYNC_LOCATION);
	lstBasicCommands.AddTail(ID_EDIT_PRINT_PAGE_SETUP);
	lstBasicCommands.AddTail(ID_FILE_PRINT_SETUP);
	lstBasicCommands.AddTail(ID_FILE_PRINT_PREVIEW);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	// Edit menu
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_EDIT_REDO);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_COPY);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_CUT_N_APPEND);
	lstBasicCommands.AddTail(ID_EDIT_CUT_BOOKMARKED);
	lstBasicCommands.AddTail(ID_EDIT_COPY_N_APPEND);
	lstBasicCommands.AddTail(ID_EDIT_COPY_BOOKMARKED);
	lstBasicCommands.AddTail(ID_EDIT_DELETE);
	lstBasicCommands.AddTail(ID_EDIT_DELETE_WORD_TO_RIGHT);
	lstBasicCommands.AddTail(ID_EDIT_DELETE_WORD_TO_LEFT);
	lstBasicCommands.AddTail(ID_EDIT_DELETE_LINE);
	lstBasicCommands.AddTail(ID_EDIT_DELETE_BOOKMARKED);
	lstBasicCommands.AddTail(ID_EDIT_SELECT_WORD);
	lstBasicCommands.AddTail(ID_EDIT_SELECT_LINE);
	lstBasicCommands.AddTail(ID_EDIT_SELECT_ALL);
	lstBasicCommands.AddTail(ID_EDIT_SCROLL_UP);
	lstBasicCommands.AddTail(ID_EDIT_SCROLL_CENTER);
	lstBasicCommands.AddTail(ID_EDIT_SCROLL_DOWN);
	// View Menu
	lstBasicCommands.AddTail(ID_VIEW_CONNECTBAR);
	lstBasicCommands.AddTail(ID_VIEW_DATABASEBAR);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_WINDOWS_7);
	lstBasicCommands.AddTail(ID_VIEW_FILE_PANEL);
	// Search menu
	lstBasicCommands.AddTail(ID_EDIT_FIND);
	lstBasicCommands.AddTail(ID_EDIT_REPLACE);
	lstBasicCommands.AddTail(ID_EDIT_FIND_NEXT);
	lstBasicCommands.AddTail(ID_EDIT_FIND_PREVIOUS);
	lstBasicCommands.AddTail(ID_EDIT_GOTO);
	lstBasicCommands.AddTail(ID_EDIT_NEXT);
	lstBasicCommands.AddTail(ID_EDIT_PREVIOUS);
	lstBasicCommands.AddTail(ID_EDIT_BKM_TOGGLE);
	lstBasicCommands.AddTail(ID_EDIT_BKM_NEXT);
	lstBasicCommands.AddTail(ID_EDIT_BKM_PREV);
	lstBasicCommands.AddTail(ID_EDIT_BKM_REMOVE_ALL);
	lstBasicCommands.AddTail(ID_EDIT_FIND_MATCH);
	lstBasicCommands.AddTail(ID_EDIT_FIND_MATCH_N_SELECT);
	lstBasicCommands.AddTail(ID_FILE_FIND_IN_FILE);
	// Format menu
  lstBasicCommands.AddTail(ID_EDIT_SORT);
  lstBasicCommands.AddTail(ID_EDIT_INDENT);
  lstBasicCommands.AddTail(ID_EDIT_UNDENT);
  lstBasicCommands.AddTail(ID_EDIT_UNTABIFY);
  lstBasicCommands.AddTail(ID_EDIT_TABIFY);
  lstBasicCommands.AddTail(ID_EDIT_TABIFY_LEADING);
  lstBasicCommands.AddTail(ID_EDIT_NORMALIZE_TEXT);
  lstBasicCommands.AddTail(ID_EDIT_LOWER);
  lstBasicCommands.AddTail(ID_EDIT_UPPER);
  lstBasicCommands.AddTail(ID_EDIT_CAPITALIZE);
  lstBasicCommands.AddTail(ID_EDIT_INVERT_CASE);
  lstBasicCommands.AddTail(ID_EDIT_COMMENT);
  lstBasicCommands.AddTail(ID_EDIT_UNCOMMENT);
  lstBasicCommands.AddTail(ID_EDIT_EXPAND_TEMPLATE);
  lstBasicCommands.AddTail(ID_EDIT_DATETIME_STAMP);
	// Text menu
  lstBasicCommands.AddTail(ID_EDIT_STREAM_SEL);
  lstBasicCommands.AddTail(ID_EDIT_COLUMN_SEL);
  lstBasicCommands.AddTail(ID_EDIT_TOGGLES_SEL);
  lstBasicCommands.AddTail(ID_EDIT_VIEW_WHITE_SPACE);
  lstBasicCommands.AddTail(ID_EDIT_VIEW_LINE_NUMBERS);
  lstBasicCommands.AddTail(ID_EDIT_FILE_SETTINGS);
  lstBasicCommands.AddTail(ID_EDIT_PERMANENT_SETTINGS);
	// Connect menu
  lstBasicCommands.AddTail(ID_ODBC_CONNECT);
  lstBasicCommands.AddTail(ID_ODBC_DISCONNECT);
  lstBasicCommands.AddTail(ID_ODBC_BEGIN);
  lstBasicCommands.AddTail(ID_ODBC_COMMIT);
  lstBasicCommands.AddTail(ID_ODBC_ROLLBACK);
  lstBasicCommands.AddTail(ID_SESSION_ODBCREPORT);
  lstBasicCommands.AddTail(ID_SESSIONSTATUS);
	// Script menu
  lstBasicCommands.AddTail(ID_SCRIPT_EXECUTE);
  lstBasicCommands.AddTail(ID_SCRIPT_CURRENT);
  lstBasicCommands.AddTail(ID_SCRIPT_EXECUTESTEP);
  lstBasicCommands.AddTail(ID_SCRIPT_NEXTERROR);
  lstBasicCommands.AddTail(ID_SCRIPT_PREVERROR);
  lstBasicCommands.AddTail(ID_SCRIPT_NATIVE);
  lstBasicCommands.AddTail(ID_SCRIPT_FINDTABLE);
  lstBasicCommands.AddTail(ID_SCRIPT_TABLEDDL);
  lstBasicCommands.AddTail(ID_SCRIPT_VARIABLES);
  // Tools
  lstBasicCommands.AddTail(ID_TOOLS_SQLMIGRATION);

	// Popup-menu
	lstBasicCommands.AddTail(ID_SORTING_SORTALPHABETIC);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYTYPE);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYACCESS);
	lstBasicCommands.AddTail(ID_SORTING_GROUPBYTYPE);
	// Help menu
  lstBasicCommands.AddTail(ID_HELP_INDEX);
  lstBasicCommands.AddTail(ID_HELP_BUG);
  lstBasicCommands.AddTail(ID_HELP_FEATURE);
  lstBasicCommands.AddTail(ID_HELP_SFNET);
  lstBasicCommands.AddTail(ID_HELP_GITHUB);
  lstBasicCommands.AddTail(ID_DONATION);
  lstBasicCommands.AddTail(ID_APP_ABOUT);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);

	// Switch the order of document name and application name on the window title bar. This
	// improves the usability of the taskbar because the document name is visible with the thumbnail.
	ModifyStyle(0,FWS_PREFIXTITLE);

  // Try to do a database ping every 5 minutes
  SetTimer(1,(5 * 60 * CLOCKS_PER_SEC),NULL);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !StyleMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	// Create ODBC info view
	CString strInfoName;
	bNameValid = strInfoName.LoadString(IDS_ODBC_VIEW);
	ASSERT(bNameValid);

	if (!m_wndInfoView.Create(strInfoName, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_INFOVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create ODBC Info View window\n");
		return FALSE; // failed to create
	}

	// Create object view
	CString strFileView;
	bNameValid = strFileView.LoadString(IDS_OBJECT_VIEW);
	ASSERT(bNameValid);
	if (!m_wndObjectView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_TABLEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create database object window\n");
		return FALSE; // failed to create
	}

	// Create directory view
	CString strDirView;
	bNameValid = strDirView.LoadString(IDS_DIRECTORY_VIEW);
	ASSERT(bNameValid);
  if (!m_wndDirView.Create(strDirView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
  {
    TRACE0("Failed to create Explorer window\n");
    return FALSE; // failed to create
  }

	// Create output window
	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Output window\n");
		return FALSE; // failed to create
	}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);

  CString drivePath = AfxGetApp()->GetProfileString("FileManager", "CurrentDrive");
  if (drivePath.IsEmpty())
  {
    std::string path;
    Common::AppGetPath(path);
    PathBuildRoot(drivePath.GetBuffer(MAX_PATH), PathGetDriveNumber(path.c_str()));
    drivePath.ReleaseBuffer();
  }
  m_wndDirView.SetCurDrivePath(drivePath);
	m_wndDirView.DisplayDrivers(TRUE, TRUE);
	m_wndDirView.SelectDrive(drivePath, TRUE);

	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hObjectViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndObjectView.SetIcon(hObjectViewIcon, FALSE);

	HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndInfoView.SetIcon(hClassViewIcon, FALSE);

  HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
  m_wndDirView.SetIcon(hFileViewIcon, FALSE);

	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

	UpdateMDITabbedBarsIcons();
}


LRESULT 
CMainFrame::OnToolbarReset(WPARAM wp, LPARAM)
{
  UINT uiToolBarId = (UINT)wp;

  switch (uiToolBarId)
  {
		case IDR_MAINFRAME:

			break;

		case IDR_CONNECTBAR:
			// Replace button for edit box to display current connection
			CMFCToolBarEditBoxButton  editBox(ID_ODBC_DATABASE, GetCmdMgr()->GetCmdImage(ID_ODBC_DATABASE,FALSE),ES_READONLY);
			m_connect = m_conToolBar.ReplaceButton(ID_ODBC_DATABASE,editBox);
			break;
	}

  return 0;
}



// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	StyleMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	StyleMDIFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* scan menus */);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = StyleMDIFrameWnd::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	//m_wndOutput.UpdateFonts();
	RedrawWindow(nullptr, nullptr, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}


BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext)
{
	if (!StyleMDIFrameWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// enable customization button for all user toolbars
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != nullptr)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}

void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	StyleMDIFrameWnd::OnSettingChange(uFlags, lpszSection);
	// m_wndOutput.UpdateFonts();
}

void
CMainFrame::FindTable(CString& table)
{
  m_wndObjectView.FindTable(table);
}

void
CMainFrame::ReportCapabilities(CString& filename)
{
	m_wndInfoView.ReportCapabilities(filename);
}

void
CMainFrame::OnRefreshOdbc()
{
	m_wndInfoView.FillInfoView();
	m_wndObjectView.ClearObjectTree();
}

void
CMainFrame::SetConnectionText(CString& text)
{
  if (!m_conToolBar.IsVisible())
  {
    // Then show it immediately
    ShowPane(&m_conToolBar,TRUE,FALSE,TRUE);
  }
  // Set the connection text
	int index = m_conToolBar.CommandToIndex(ID_ODBC_DATABASE);
	CMFCToolBarEditBoxButton* edit = (CMFCToolBarEditBoxButton*)m_conToolBar.GetButton(index);
 	if (edit && edit->GetHwnd())
	{
		edit->SetContents(text);
	}
}

BOOL CMainFrame::OnCopyData(CWnd*, COPYDATASTRUCT* pCopyDataStruct)
{
  if (pCopyDataStruct->dwData == 1)
  {
    if (pCopyDataStruct->cbData)
    {
      if (IsIconic()) OpenIcon();
      COEDocument::EnableOpenUnexisting enable;
      AfxGetApp()->OpenDocumentFile(CString((LPCSTR)pCopyDataStruct->lpData, pCopyDataStruct->cbData));
    }
    return TRUE;
  }
  return FALSE;
}

void 
CMainFrame::OnActivateApp(BOOL bActive, DWORD dwThreadID)
{
  StyleMDIFrameWnd::OnActivateApp(bActive, dwThreadID);

	theApp.OnActivateApp(bActive);
}

void
CMainFrame::OnConnectBar()
{
  m_conBarVisible = !m_conBarVisible;
  ShowPane(&m_conToolBar,m_conBarVisible,FALSE,TRUE);
}

void
CMainFrame::OnDatabaseBar()
{
  m_dbBarVisible = !m_dbBarVisible;
  ShowPane(&m_dbToolBar,m_dbBarVisible,FALSE,TRUE);
}

void
CMainFrame::OnSupportBug()
{
  SendSupportEmail("Bug report for Open-ODBCQueryTool", "The following functionality is broken...");
}

void
CMainFrame::OnSupportFeature()
{
  SendSupportEmail("Feature request for Open-ODBCQueryTool", "I suggest that you....");
}

void
CMainFrame::SendSupportEmail(CString p_subject, CString p_body)
{
  CString command("open");
  CString parameters("mailto:edwig.huisman@hetnet.nl");

  parameters.AppendFormat("?Subject=%s", p_subject);
  parameters.AppendFormat("&Body=%s",    p_body);

  CWaitCursor aSigh;
  ShellExecute(GetSafeHwnd(), command, parameters, NULL, NULL, SW_SHOWNORMAL);
}

void
CMainFrame::OnWebSourceforge()
{
  CString source("https://sourceforge.net/projects/odbcquerytool/");
  ShellExecute(NULL, "open", source, NULL, NULL, SW_SHOWMAXIMIZED);
}

void
CMainFrame::OnWebGithub()
{
  CString github("https://github.com/edwig/ODBCQueryTool/");
  ShellExecute(NULL, "open", github, NULL, NULL, SW_SHOWMAXIMIZED);
}

void
CMainFrame::OnSQLMigration()
{
  CString path = GetExePath();
  path += "SQLMigrate.exe";
  ShellExecute(NULL,"open",path,NULL,NULL,SW_SHOWNORMAL);
}

//////////////////////////////////////////////////////////////////////////


void
CMainFrame::OnUpdateConnect(CCmdUI* pCmdUI)
{
  QueryToolApp* app = dynamic_cast<QueryToolApp*> (AfxGetApp());
  pCmdUI->Enable(!app->DatabaseIsOpen());
}

void
CMainFrame::OnUpdateDisconnect(CCmdUI* pCmdUI)
{
  QueryToolApp* app = dynamic_cast<QueryToolApp*> (AfxGetApp());
  pCmdUI->Enable(app->DatabaseIsOpen());
}

void
CMainFrame::OnUpdateConnectBarVisible(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(m_conBarVisible);
}

void
CMainFrame::OnUpdateDatabaseBarVisible(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(m_dbBarVisible);
}

void
CMainFrame::OnUpdateBegin(CCmdUI* pCmdUI)
{
  QueryToolApp* app = dynamic_cast<QueryToolApp*> (AfxGetApp());
  pCmdUI->Enable(app->DatabaseIsOpen() && !app->DatabaseHasTransaction());
}

void
CMainFrame::OnUpdateCommit(CCmdUI* pCmdUI)
{
  QueryToolApp* app = dynamic_cast<QueryToolApp*> (AfxGetApp());
  pCmdUI->Enable(app->DatabaseHasTransaction());
}

void
CMainFrame::OnUpdateRollback(CCmdUI* pCmdUI)
{
  QueryToolApp* app = dynamic_cast<QueryToolApp*> (AfxGetApp());
  pCmdUI->Enable(app->DatabaseHasTransaction());
}

void
CMainFrame::OnUpdateODBCReport(CCmdUI* pCmdUI)
{
  QueryToolApp* app = dynamic_cast<QueryToolApp*> (AfxGetApp());
  pCmdUI->Enable(app->DatabaseIsOpen());
}

void
CMainFrame::OnUpdateSessionStatus(CCmdUI* pCmdUI)
{
  QueryToolApp* app = dynamic_cast<QueryToolApp*> (AfxGetApp());
  pCmdUI->Enable(app->DatabaseIsOpen());
}

void
CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
  if(nIDEvent == 1)
  {
    QueryToolApp* app = dynamic_cast<QueryToolApp*> (AfxGetApp());
    app->GetDatabase().Ping();
  }
}

void
CMainFrame::LoadSettingsManager()
{
  std::string path;
  Common::AppGetPath(path);

  // Load editor settings
  OpenEditor::SettingsManagerReader(OpenEditor::FileInStream((path + "\\data\\settings.dat").c_str())) >> m_settingsManager;
}

void
CMainFrame::SaveSettingsManager()
{
  std::string path;
  Common::AppGetPath(path);

  // old settings backup
  _CHECK_AND_THROW_(CopyFile((path + "\\data\\settings.dat").c_str(),
										(path + "\\data\\settings.dat.old").c_str(), FALSE) != 0,
										"File search: settings error!"
  );

  OpenEditor::SettingsManagerWriter(OpenEditor::FileOutStream((path + "\\data\\settings.dat").c_str())) << m_settingsManager;
}

void
CMainFrame::OnFindFiles()
{
  LoadSettingsManager();

  COEFindFiles findFiles(m_settingsManager,this);

  if (findFiles.DoModal() == IDOK)
  {
    SaveSettingsManager();

    // Hide the find-in-files control for pre-emptive redrawing
		ShowPane(&m_wndOutput,FALSE,FALSE,TRUE);

		StyleGridCtrl* grid = m_wndOutput.GetEmptyFindGrid();
    if (findFiles.GoFind(grid))
    {
      // Something found
      ShowPane(&m_wndOutput,TRUE,FALSE,TRUE);
      DockPane(&m_wndOutput,AFX_IDW_DOCKBAR_BOTTOM);
      grid->ExpandLastColumn();
    }
  }
}

void 
CMainFrame::OnViewFindPanel()
{
	BOOL show = !m_wndOutput.IsVisible();
	ShowPane(&m_wndOutput,show,FALSE,TRUE);
}

void 
CMainFrame::OnUpdateViewFindPanel(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(m_wndOutput.IsVisible());
}

LRESULT 
CMainFrame::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
  if(WM_SETTEXT == message)
  {
    // if set text (called from UpdateFrameTitleForDocument)
    // do not call DefWindowProc
    return CWnd::DefWindowProc(message, wParam, lParam);
  }

  // otherwise call DefWindowProc that in turn calls DefFrameProc
  return StyleMDIFrameWnd::DefWindowProc(message, wParam, lParam);
}

void 
CMainFrame::OnViewFilePanelSync()
{
	CFrameWnd* frame = GetActiveFrame();
	if(frame)
	{
		COEditorView* view =  (COEditorView*) frame->GetActiveView();
		if(view)
		{
			COEDocument* document = view->GetDocument();
			if(document)
			{
				CString path = document->GetPathName();
				if (!path.IsEmpty() && m_wndDirView.SetCurPath(path))
				{
					if(!m_wndDirView.IsVisible())
					{
						ShowPane(&m_wndDirView, TRUE, FALSE, TRUE);
					}
					m_wndDirView.SetFocus();
					return; // Ok!
				}
			}
		}
	}
  MessageBeep((UINT)-1); // something fails
}

void 
CMainFrame::OnUpdateViewFilePanelSync(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(!m_wndDirView.IsVisible() ? TRUE : FALSE);
}

void
CMainFrame::OnDonation()
{
	DonateDlg dlg(this);
	dlg.DoModal();
}
