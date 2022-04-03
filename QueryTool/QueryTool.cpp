////////////////////////////////////////////////////////////////////////
//
// File: QueryTool.cpp
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
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "QueryTool.h"
#include "MainFrm.h"
#include "Common\AppGlobal.h"
#include "AboutDlg.h"
#include "ChildFrm.h"
#include "QueryToolDoc.h"
#include "QueryToolView.h"
#include "Version.h"
#include <RegistryManager.h>
#include <DDLCreateTable.h>
#include "OEDocument.h"
#include "Common\GUICommandDictionary.h"
#include "ConnectionDlg.h"
#include "MultConnectionsDlg.h"

using namespace Common;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// The one and only QueryToolApp object
QueryToolApp theApp;

// QueryToolApp
BEGIN_MESSAGE_MAP(QueryToolApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT,        &QueryToolApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW,         &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN,        &CWinAppEx::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)

  ON_COMMAND(ID_EDIT_PERMANENT_SETTINGS,  OnEditPermanetSettings)
  ON_THREAD_MESSAGE(CFileWatch::m_msgFileWatchNotify, OnFileWatchNotify)
  ON_COMMAND(ID_FILE_CLOSE_ALL,           OnFileCloseAll)
  ON_COMMAND(ID_FILE_SAVE_ALL,            OnFileSaveAll)
  ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_ALL,  OnUpdate_FileSaveAll)
  ON_COMMAND_RANGE(ID_APP_DBLKEYACCEL_FIRST, ID_APP_DBLKEYACCEL_LAST, OnDblKeyAccel)
  ON_COMMAND(ID_HELP_INDEX,               OnCHMHelp)
  ON_COMMAND(ID_ODBC_CONNECT,             OnConnect)
  ON_COMMAND(ID_ODBC_DISCONNECT,          OnDisconnect)
  ON_COMMAND(ID_ODBC_BEGIN,               OnODBCBegin)
  ON_COMMAND(ID_ODBC_COMMIT,              OnODBCCommit)
  ON_COMMAND(ID_ODBC_ROLLBACK,            OnODBCRollback)
  ON_COMMAND(ID_SESSION_ODBCREPORT,       ReportCapabilities)
  ON_COMMAND(ID_SESSIONSTATUS,            OnSessionStatus)

  ON_UPDATE_COMMAND_UI(ID_INDICATOR_POS,        OnUpdateEditIndicators)
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_SCROLL_POS, OnUpdateEditIndicators)
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_FILE_TYPE,  OnUpdateEditIndicators)
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_OVR,        OnUpdateEditIndicators)
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_FILE_TYPE,  OnUpdateEditIndicators)
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_BLOCK_TYPE, OnUpdateEditIndicators)
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_FILE_LINES, OnUpdateEditIndicators)
END_MESSAGE_MAP()


// QueryToolApp construction

QueryToolApp::QueryToolApp() noexcept
{
	m_bHiColorIcons = TRUE;
	m_nAppLook      = 0;
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("EDO.QueryTool.App.Version"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
  m_dblKeyAccelInx      = -1;
  m_hMutex              = NULL;
  m_accelTable          = NULL;
  m_pDocManager         = new CDocManagerExt;
  m_isClosing           = false;
}

QueryToolApp::~QueryToolApp()
{
  if (m_transaction)
  {
    // Rollback our last transaction
    m_transaction->Rollback();
    delete m_transaction;
    m_transaction = NULL;
  }

  // Close our database
  m_database.IsOpen();
  {
    m_database.Close();
  }

  VarMap::iterator it = m_variables.begin();
  while (it != m_variables.end())
  {
    SQLVariant* var = it->second;
    it->second = NULL;
    delete var;
    ++it;
  }
  m_variables.clear();
  m_rebinds.clear();
}

void QueryToolApp::InitGUICommand()
{
#if (ID_APP_DBLKEYACCEL_FIRST != 0xDF00 || ID_APP_DBLKEYACCEL_LAST != 0xDFFF)
#pragma error("Check ID_APP_DBLKEYACCEL_FIRST or ID_APP_DBLKEYACCEL_LAST definition")
#endif

  if(!GUICommandDictionary::m_firstDblKeyAccelCommandId)
  {
    GUICommandDictionary::m_firstDblKeyAccelCommandId = ID_APP_DBLKEYACCEL_FIRST;

    //File
    GUICommandDictionary::InsertCommand("File.New",                        ID_FILE_NEW);
    GUICommandDictionary::InsertCommand("File.Open",                       ID_FILE_OPEN);
    GUICommandDictionary::InsertCommand("File.Reload",                     ID_FILE_RELOAD);
    GUICommandDictionary::InsertCommand("File.Close",                      ID_FILE_CLOSE);
    GUICommandDictionary::InsertCommand("File.CloseAll",                   ID_FILE_CLOSE_ALL);
    GUICommandDictionary::InsertCommand("File.Save",                       ID_FILE_SAVE);
    GUICommandDictionary::InsertCommand("File.SaveAs",                     ID_FILE_SAVE_AS);
    GUICommandDictionary::InsertCommand("File.SaveAll",                    ID_FILE_SAVE_ALL);
    GUICommandDictionary::InsertCommand("File.ShowFileLocation",           ID_FILE_SYNC_LOCATION);
    GUICommandDictionary::InsertCommand("File.PageSetup",                  ID_EDIT_PRINT_PAGE_SETUP);
    GUICommandDictionary::InsertCommand("File.PrintSetup",                 ID_FILE_PRINT_SETUP);
    GUICommandDictionary::InsertCommand("File.PrintPreview",               ID_FILE_PRINT_PREVIEW);
    GUICommandDictionary::InsertCommand("File.Print",                      ID_FILE_PRINT);
    GUICommandDictionary::InsertCommand("File.Exit",                       ID_APP_EXIT);
    //Edit
    GUICommandDictionary::InsertCommand("Edit.Undo",                       ID_EDIT_UNDO);
    GUICommandDictionary::InsertCommand("Edit.Undo",                       ID_EDIT_UNDO);
    GUICommandDictionary::InsertCommand("Edit.Redo",                       ID_EDIT_REDO);
    GUICommandDictionary::InsertCommand("Edit.Cut",                        ID_EDIT_CUT);
    GUICommandDictionary::InsertCommand("Edit.Cut",                        ID_EDIT_CUT);
    GUICommandDictionary::InsertCommand("Edit.Copy",                       ID_EDIT_COPY);
    GUICommandDictionary::InsertCommand("Edit.Copy",                       ID_EDIT_COPY);
    GUICommandDictionary::InsertCommand("Edit.Paste",                      ID_EDIT_PASTE);
    GUICommandDictionary::InsertCommand("Edit.Paste",                      ID_EDIT_PASTE);
    GUICommandDictionary::InsertCommand("Edit.CutAppend",                  ID_EDIT_CUT_N_APPEND);
    GUICommandDictionary::InsertCommand("Edit.CutAppend",                  ID_EDIT_CUT_N_APPEND);
    GUICommandDictionary::InsertCommand("Edit.CutBookmarkedLines",         ID_EDIT_CUT_BOOKMARKED);
    GUICommandDictionary::InsertCommand("Edit.CopyAppend",                 ID_EDIT_COPY_N_APPEND);
    GUICommandDictionary::InsertCommand("Edit.CopyAppend",                 ID_EDIT_COPY_N_APPEND);
    GUICommandDictionary::InsertCommand("Edit.CopyBookmarkedLines",        ID_EDIT_COPY_BOOKMARKED);
    GUICommandDictionary::InsertCommand("Edit.Delete",                     ID_EDIT_DELETE);
    GUICommandDictionary::InsertCommand("Edit.DeletetoEndOfWord",          ID_EDIT_DELETE_WORD_TO_RIGHT);
    GUICommandDictionary::InsertCommand("Edit.DeleteWordBack",             ID_EDIT_DELETE_WORD_TO_LEFT);
    GUICommandDictionary::InsertCommand("Edit.DeleteLine",                 ID_EDIT_DELETE_LINE);
    GUICommandDictionary::InsertCommand("Edit.DeleteBookmarkedLines",      ID_EDIT_DELETE_BOOKMARKED);
    GUICommandDictionary::InsertCommand("Edit.SelectWord",                 ID_EDIT_SELECT_WORD);
    GUICommandDictionary::InsertCommand("Edit.SelectLine",                 ID_EDIT_SELECT_LINE);
    GUICommandDictionary::InsertCommand("Edit.SelectAll",                  ID_EDIT_SELECT_ALL);
    GUICommandDictionary::InsertCommand("Edit.ScrollUp",                   ID_EDIT_SCROLL_UP);
    GUICommandDictionary::InsertCommand("Edit.ScrollToCenter",             ID_EDIT_SCROLL_CENTER);
    GUICommandDictionary::InsertCommand("Edit.ScrollToCenter",             ID_EDIT_SCROLL_CENTER);
    GUICommandDictionary::InsertCommand("Edit.ScrollDown",                 ID_EDIT_SCROLL_DOWN);
    //Search
    GUICommandDictionary::InsertCommand("Edit.Find",                       ID_EDIT_FIND);
    GUICommandDictionary::InsertCommand("Edit.Replace",                    ID_EDIT_REPLACE);
    GUICommandDictionary::InsertCommand("Edit.FindNext",                   ID_EDIT_FIND_NEXT);
    GUICommandDictionary::InsertCommand("Edit.FindPrevious",               ID_EDIT_FIND_PREVIOUS);
    GUICommandDictionary::InsertCommand("Edit.GoTo",                       ID_EDIT_GOTO);
    GUICommandDictionary::InsertCommand("Edit.JumpNext",                   ID_EDIT_NEXT);
    GUICommandDictionary::InsertCommand("Edit.JumpPrevious",               ID_EDIT_PREVIOUS);
    GUICommandDictionary::InsertCommand("Edit.ToggleBookmark",             ID_EDIT_BKM_TOGGLE);
    GUICommandDictionary::InsertCommand("Edit.NextBookmark",               ID_EDIT_BKM_NEXT);
    GUICommandDictionary::InsertCommand("Edit.PreviousBookmark",           ID_EDIT_BKM_PREV);
    GUICommandDictionary::InsertCommand("Edit.RemoveallBookmarks",         ID_EDIT_BKM_REMOVE_ALL);
    //SetRandomBookmark
    GUICommandDictionary::InsertCommand("Edit.SetRandomBookmark.0",        ID_EDIT_BKM_SET_0);
    GUICommandDictionary::InsertCommand("Edit.SetRandomBookmark.1",        ID_EDIT_BKM_SET_1);
    GUICommandDictionary::InsertCommand("Edit.SetRandomBookmark.2",        ID_EDIT_BKM_SET_2);
    GUICommandDictionary::InsertCommand("Edit.SetRandomBookmark.3",        ID_EDIT_BKM_SET_3);
    GUICommandDictionary::InsertCommand("Edit.SetRandomBookmark.4",        ID_EDIT_BKM_SET_4);
    GUICommandDictionary::InsertCommand("Edit.SetRandomBookmark.5",        ID_EDIT_BKM_SET_5);
    GUICommandDictionary::InsertCommand("Edit.SetRandomBookmark.6",        ID_EDIT_BKM_SET_6);
    GUICommandDictionary::InsertCommand("Edit.SetRandomBookmark.7",        ID_EDIT_BKM_SET_7);
    GUICommandDictionary::InsertCommand("Edit.SetRandomBookmark.8",        ID_EDIT_BKM_SET_8);
    GUICommandDictionary::InsertCommand("Edit.SetRandomBookmark.9",        ID_EDIT_BKM_SET_9);
    //GetRandomBookmark
    GUICommandDictionary::InsertCommand("Edit.GetRandomBookmark.0",        ID_EDIT_BKM_GET_0);
    GUICommandDictionary::InsertCommand("Edit.GetRandomBookmark.1",        ID_EDIT_BKM_GET_1);
    GUICommandDictionary::InsertCommand("Edit.GetRandomBookmark.2",        ID_EDIT_BKM_GET_2);
    GUICommandDictionary::InsertCommand("Edit.GetRandomBookmark.3",        ID_EDIT_BKM_GET_3);
    GUICommandDictionary::InsertCommand("Edit.GetRandomBookmark.4",        ID_EDIT_BKM_GET_4);
    GUICommandDictionary::InsertCommand("Edit.GetRandomBookmark.5",        ID_EDIT_BKM_GET_5);
    GUICommandDictionary::InsertCommand("Edit.GetRandomBookmark.6",        ID_EDIT_BKM_GET_6);
    GUICommandDictionary::InsertCommand("Edit.GetRandomBookmark.7",        ID_EDIT_BKM_GET_7);
    GUICommandDictionary::InsertCommand("Edit.GetRandomBookmark.8",        ID_EDIT_BKM_GET_8);
    GUICommandDictionary::InsertCommand("Edit.GetRandomBookmark.9",        ID_EDIT_BKM_GET_9);
    GUICommandDictionary::InsertCommand("Edit.FindMatch",                  ID_EDIT_FIND_MATCH);
    GUICommandDictionary::InsertCommand("Edit.FindMatchSelect",            ID_EDIT_FIND_MATCH_N_SELECT);
    GUICommandDictionary::InsertCommand("Edit.FindInFile",                 ID_FILE_FIND_IN_FILE);
    //Format
    GUICommandDictionary::InsertCommand("Edit.Sort",                       ID_EDIT_SORT);
    GUICommandDictionary::InsertCommand("Edit.IndentSelection",            ID_EDIT_INDENT);
    GUICommandDictionary::InsertCommand("Edit.UndentSelection",            ID_EDIT_UNDENT);
    GUICommandDictionary::InsertCommand("Edit.UntabifySelection",          ID_EDIT_UNTABIFY);
    GUICommandDictionary::InsertCommand("Edit.TabifySelectionAll",         ID_EDIT_TABIFY);
    GUICommandDictionary::InsertCommand("Edit.TabifySelectionLeading",     ID_EDIT_TABIFY_LEADING);
    GUICommandDictionary::InsertCommand("Edit.NormalizeKeyword",           ID_EDIT_NORMALIZE_TEXT);
    GUICommandDictionary::InsertCommand("Edit.Lowercase",                  ID_EDIT_LOWER);
    GUICommandDictionary::InsertCommand("Edit.Uppercase",                  ID_EDIT_UPPER);
    GUICommandDictionary::InsertCommand("Edit.Capitalize",                 ID_EDIT_CAPITALIZE);
    GUICommandDictionary::InsertCommand("Edit.InvertCase",                 ID_EDIT_INVERT_CASE);
    GUICommandDictionary::InsertCommand("Edit.CommentLine",                ID_EDIT_COMMENT);
    GUICommandDictionary::InsertCommand("Edit.UncommentLine",              ID_EDIT_UNCOMMENT);
    GUICommandDictionary::InsertCommand("Edit.ExpandTemplate",             ID_EDIT_EXPAND_TEMPLATE);
    GUICommandDictionary::InsertCommand("Edit.DatetimeStamp",              ID_EDIT_DATETIME_STAMP);
    //Text
    GUICommandDictionary::InsertCommand("Edit.StreamSelection",            ID_EDIT_STREAM_SEL);
    GUICommandDictionary::InsertCommand("Edit.ColumnSelection",            ID_EDIT_COLUMN_SEL);
    GUICommandDictionary::InsertCommand("Edit.ToggleSelectionMode",        ID_EDIT_TOGGLES_SEL);
    GUICommandDictionary::InsertCommand("Edit.VisibleSpaces",              ID_EDIT_VIEW_WHITE_SPACE);
    GUICommandDictionary::InsertCommand("Edit.LineNumbers",                ID_EDIT_VIEW_LINE_NUMBERS);
    GUICommandDictionary::InsertCommand("Edit.FileSettings",               ID_EDIT_FILE_SETTINGS);
    GUICommandDictionary::InsertCommand("Edit.PermanentSettings",          ID_EDIT_PERMANENT_SETTINGS);
    //Session
    GUICommandDictionary::InsertCommand("Session.Connect",                 ID_ODBC_CONNECT);
    GUICommandDictionary::InsertCommand("Session.Disconnect",              ID_ODBC_DISCONNECT);
    GUICommandDictionary::InsertCommand("Session.Begin",                   ID_ODBC_BEGIN);
    GUICommandDictionary::InsertCommand("Session.Commit",                  ID_ODBC_COMMIT);
    GUICommandDictionary::InsertCommand("Session.Rollback",                ID_ODBC_ROLLBACK);
    GUICommandDictionary::InsertCommand("Session.ODBCReport",              ID_SESSION_ODBCREPORT);
    GUICommandDictionary::InsertCommand("Session.SessionStatus",           ID_SESSIONSTATUS);
    //Script
    GUICommandDictionary::InsertCommand("Script.Native",                   ID_SCRIPT_NATIVE);
    GUICommandDictionary::InsertCommand("Script.Execute",                  ID_SCRIPT_EXECUTE);
    GUICommandDictionary::InsertCommand("Script.ExecuteCurrent",           ID_SCRIPT_CURRENT);
    GUICommandDictionary::InsertCommand("Script.ExecuteAndStep",           ID_SCRIPT_EXECUTESTEP);
    GUICommandDictionary::InsertCommand("Script.FindTable",                ID_SCRIPT_FINDTABLE);
    GUICommandDictionary::InsertCommand("Script.TableDDL",                 ID_SCRIPT_TABLEDDL);
    GUICommandDictionary::InsertCommand("Script.Variables",                ID_SCRIPT_VARIABLES);
    GUICommandDictionary::InsertCommand("Script.NextError",                ID_SCRIPT_NEXTERROR);
    GUICommandDictionary::InsertCommand("Script.PreviousError",            ID_SCRIPT_PREVERROR);
    //View
    GUICommandDictionary::InsertCommand("View.Toolbar",                    ID_VIEW_TOOLBAR);
    GUICommandDictionary::InsertCommand("View.StatusBar",                  ID_VIEW_STATUS_BAR);
    GUICommandDictionary::InsertCommand("View.FilePanel",                  ID_VIEW_FILE_PANEL);
    GUICommandDictionary::InsertCommand("View.Workbook",                   ID_VIEW_WORKBOOK);
    GUICommandDictionary::InsertCommand("View.NextPane",                   ID_NEXT_PANE);
    GUICommandDictionary::InsertCommand("View.PrevPane",                   ID_PREV_PANE);
    //Grid
    GUICommandDictionary::InsertCommand("Grid.ShowGrid",                   ID_QUERY_GRID);
    GUICommandDictionary::InsertCommand("Grid.ShowStatistics",             ID_QUERY_STATISTICS);
    GUICommandDictionary::InsertCommand("Grid.ShowOutput",                 ID_QUERY_OUTPUT);
    GUICommandDictionary::InsertCommand("Grid.ShowHistory",                ID_QUERY_HISTORY);
    GUICommandDictionary::InsertCommand("Grid.SizeHeader",                 ID_QUERY_SIZEHEADER);
    GUICommandDictionary::InsertCommand("Grid.SizeData",                   ID_QUERY_SIZEDATA);
    GUICommandDictionary::InsertCommand("Grid.SizeBoth",                   ID_QUERY_SIZEBOTH);

    GUICommandDictionary::InsertCommand("Grid.Begin",                      ID_QUERY_BEGIN);
    GUICommandDictionary::InsertCommand("Grid.PrevPage",                   ID_QUERY_PREVPAGE);
    GUICommandDictionary::InsertCommand("Grid.Prev",                       ID_QUERY_PREV);
    GUICommandDictionary::InsertCommand("Grid.Next",                       ID_QUERY_NEXT);
    GUICommandDictionary::InsertCommand("Grid.NextPage",                   ID_QUERY_NEXTPAGE);
    GUICommandDictionary::InsertCommand("Grid.End",                        ID_QUERY_END);
    GUICommandDictionary::InsertCommand("Grid.ExportHTML",                 ID_QUERY_HTML);
    GUICommandDictionary::InsertCommand("Grid.ExportExcel",                ID_QUERY_EXCEL);   
    GUICommandDictionary::InsertCommand("Grid.ExportTxt",                  ID_QUERY_TXT);
    //Window
    GUICommandDictionary::InsertCommand("Window.NewWindow",                ID_WINDOW_NEW);
    GUICommandDictionary::InsertCommand("Window.Cascade",                  ID_WINDOW_CASCADE);
    GUICommandDictionary::InsertCommand("Window.TileHorizontally",         ID_WINDOW_TILE_HORZ);
    GUICommandDictionary::InsertCommand("Window.TileVertically",           ID_WINDOW_TILE_VERT);
    GUICommandDictionary::InsertCommand("Window.ArrangeIcons",             ID_WINDOW_ARRANGE);
    GUICommandDictionary::InsertCommand("Window.LastWindow",               ID_WINDOW_LAST);
    //Help
    GUICommandDictionary::InsertCommand("Help.Index",                      ID_HELP_INDEX);
    GUICommandDictionary::InsertCommand("Help.AboutOpenEditor",            ID_APP_ABOUT);
  }
}

void 
QueryToolApp::UpdateAccelAndMenu()
{
  InitGUICommand();

  string buffer;
  Common::AppGetPath(buffer);
  if(GUICommandDictionary::BuildAcceleratorTable(
    (buffer + "\\data\\" + COEDocument::GetSettingsManager().GetGlobalSettings().GetKeymapLayout() + ".keymap").c_str(),
    m_accelTable))
  {
    POSITION pos = m_pDocManager->GetFirstDocTemplatePosition();
    while (pos != NULL)
    {
      CDocTemplate* pTemplate = m_pDocManager->GetNextDocTemplate(pos);
      if (pTemplate && pTemplate->IsKindOf(RUNTIME_CLASS(CMultiDocTemplate)))
      {
        ((CMultiDocTemplate*)pTemplate)->m_hAccelTable = m_accelTable;
        GUICommandDictionary::AddAccelDescriptionToMenu(((CMultiDocTemplate*)pTemplate)->m_hMenuShared);
      }
    }
    if (m_pMainWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd)))
    {
      ((CFrameWnd*)m_pMainWnd)->m_hAccelTable = m_accelTable;
      GUICommandDictionary::AddAccelDescriptionToMenu(((CFrameWnd*)m_pMainWnd)->m_hMenuDefault);
    }
  }
}

// QueryToolApp initialization

BOOL QueryToolApp::InitInstance()
{
#if 0
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF
    | _CRTDBG_DELAY_FREE_MEM_DF
    | _CRTDBG_CHECK_ALWAYS_DF
    | _CRTDBG_CHECK_CRT_DF
    | _CRTDBG_LEAK_CHECK_DF);
#endif

  CMainFrame* pMainFrame = nullptr;
  try
  {
    // Load editor settings
    COEDocument::LoadSettingsManager();
    setlocale(LC_ALL, COEDocument::GetSettingsManager().GetGlobalSettings().GetLocale().c_str());

    if (!AllowThisInstance()) // must be after COEDocument::LoadSettingsManager();
    {
      return FALSE;
    }
    AfxEnableControlContainer();

    if (!AfxOleInit())
    {
      AfxMessageBox("OLE Initialization failed!");
      return FALSE;
    }
    InitSQLComponents();

    SetRegistryKey(PRODUCT_REGISTRY);
    if(SetRegistryProfile(PRODUCT_REGISTRY,PROGRAM_NAME,PROFILE_VERSION))
    {
      StyleMessageBox(nullptr,"Version change: Your profile is re-initialized","Warning",MB_OK|MB_ICONINFORMATION);
    }
    LoadStdProfileSettings(10);  // Load standard INI file options (including MRU)
    ParseODBCCommandLine();
  
    // InitCommonControlsEx() is required on Windows XP if an application
	  // manifest specifies use of ComCtl32.dll version 6 or later to enable
	  // visual styles.  Otherwise, any window creation will fail.
	  INITCOMMONCONTROLSEX InitCtrls;
	  InitCtrls.dwSize = sizeof(InitCtrls);
	  // Set this to include all the common control classes you want to use
	  // in your application.
	  InitCtrls.dwICC = ICC_WIN95_CLASSES;
	  InitCommonControlsEx(&InitCtrls);

  	CWinAppEx::InitInstance();

	  EnableTaskbarInteraction();
	  InitContextMenuManager();
	  InitKeyboardManager();
	  InitTooltipManager();

    CMFCToolTipInfo ttParams;
	  ttParams.m_bVislManagerTheme = TRUE;
	  theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	  // Register the application's document templates.  Document templates
	  //  serve as the connection between documents, frame windows and views
	  CMultiDocTemplate* pDocTemplate;
	  pDocTemplate = new CMultiDocTemplateExt(IDR_OPENEDITOR,
	                                          RUNTIME_CLASS(COEDocument),
	                                          RUNTIME_CLASS(CMDIChildFrame),
	                                          RUNTIME_CLASS(CQueryToolView));
    if(!pDocTemplate)
    {
      return FALSE;
    }
  	AddDocTemplate(pDocTemplate);

    
    // create main MDI Frame window
	  pMainFrame = new CMainFrame;
    if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	  {
		  delete pMainFrame;
		  return FALSE;
	  }
	  m_pMainWnd = pMainFrame;

	  // call DragAcceptFiles only if there's a suffix
	  //  In an MDI app, this should occur immediately after setting m_pMainWnd
	  // Enable drag/drop open
	  m_pMainWnd->DragAcceptFiles();

    // Enable DDE Execute open
    EnableShellOpen();
    UpdateAccelAndMenu();
    RegisterShellFileTypes(TRUE);

    // Parse command line for standard shell commands, DDE, file open
	  CCommandLineInfo cmdInfo;
	  ParseCommandLine(cmdInfo);

    if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew && 
      !COEDocument::GetSettingsManager().GetGlobalSettings().GetNewDocOnStartup())
    {
      cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
    }
    COEDocument::EnableOpenUnexisting enable;

	  // Dispatch commands specified on the command line.  Will return FALSE if
	  // app was launched with /RegServer, /Register, /Unregserver or /Unregister.
    if (!ProcessShellCommand(cmdInfo))
    {
      return FALSE;
    }
	  // The main window has been initialized, so show and update it
	  pMainFrame->ShowWindow(m_nCmdShow);
	  pMainFrame->UpdateWindow();

    pMainFrame->SetConnectionText((CString)"No connection");
    if (m_connectString.IsEmpty())
    {
      // Go find a connection after we start
      pMainFrame->PostMessage(WM_COMMAND,ID_ODBC_CONNECT,0);
      if(AppStarted25Times())
      {
        pMainFrame->PostMessage(WM_COMMAND,ID_DONATION,0);
      }
    }
    else if(ParseConnectString())
    {
      bool open = OpenDatabaseConnectie();
      UpdateDatabaseConnector();
      if (open)
      {
        // Execute the script as soon as we can do this
        PostMessage(pMainFrame->GetSafeHwnd(),WM_COMMAND,ID_SCRIPT_EXECUTE,0);
      }
    }
  }
  catch (CException* x)     { DEFAULT_HANDLER(x);  return FALSE; }
  catch (std::exception& x) { DEFAULT_HANDLER(x);  return FALSE; }
  catch (StdException& x)   { DEFAULT_HANDLER(x);  return FALSE; }
  catch (CString& x)        { DEFAULT_HANDLER(x);  return FALSE; }
  catch (...)               { DEFAULT_HANDLER_ALL; return FALSE; }
  
  if(m_hMutex)
  {
    ReleaseMutex(m_hMutex);
  }
  return TRUE;
}

int QueryToolApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	return CWinAppEx::ExitInstance();
}

// Simple way to get an argument from the command line
// ODBCQueryTool [/CONNECT:<connectstring>] <file.sql>
//
BOOL
QueryToolApp::ParseODBCCommandLine()
{
  m_connectString = "";
  for (int i = 1; i < __argc; i++)
  {
    LPCSTR lpszParam = __argv[i];

    if (lpszParam[0] == '-' || lpszParam[0] == '/')
    {
      if (_strnicmp(&lpszParam[1], "CONNECT:", 8) == 0)
      {
        m_connectString = &lpszParam[9];
      }
    }
  }
  return TRUE;
}

BOOL QueryToolApp::AllowThisInstance ()
{
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
  if ((cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew || 
       cmdInfo.m_nShellCommand == CCommandLineInfo::FileOpen)
  && !COEDocument::GetSettingsManager().GetGlobalSettings().GetAllowMultipleInstances())
  {
    const char* cszMutex = "EDO.ODBCQueryTool";
    const char* cszError = "Cannot connect to another program instance.";

    m_hMutex = CreateMutex(NULL, FALSE, cszMutex);
    if (m_hMutex == NULL)
    {
        AfxMessageBox(CString(cszError) + " CreateMutex error.", MB_OK|MB_ICONHAND);
        return TRUE;
    }

    DWORD dwWaitResult = WaitForSingleObject(m_hMutex, 3000L);
    if (dwWaitResult == WAIT_TIMEOUT)
    {
      AfxMessageBox(CString(cszError) + " WaitForMutex timeout.", MB_OK|MB_ICONHAND);
      return TRUE;
    }

    HWND hAnother = FindWindow(CMainFrame::m_cszClassName, NULL);
    if (hAnother)
    {
      CString buffer;
      if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileOpen)
      {
	      char *filename;
        int length = GetFullPathName(cmdInfo.m_strFileName, 0, 0, &filename);
        char *buff = buffer.GetBuffer(length + 1);
        GetFullPathName(cmdInfo.m_strFileName, length + 1, buff, &filename);
        buffer.ReleaseBuffer();
      }

      COPYDATASTRUCT cpdata;
      cpdata.dwData = 1;
      cpdata.cbData = buffer.GetLength();
      cpdata.lpData = (cpdata.cbData) ? (LPVOID)(LPCSTR)buffer : NULL;

      DWORD_PTR dwResult = TRUE;
      if (SendMessageTimeout( hAnother,                       // handle to window
                              WM_COPYDATA,                    // message
                              0,                              // first message parameter
                              (LPARAM)&cpdata,                // second message parameter
                              SMTO_ABORTIFHUNG|SMTO_BLOCK,    // send options
                              1500,                           // time-out duration
                              &dwResult                       // return value for synchronous call
                              )
      && dwResult == TRUE)
      {
        ::SetForegroundWindow(hAnother);
        if (::IsIconic(hAnother)) OpenIcon(hAnother);
        return FALSE;
      }
    }
  }
  return TRUE;
}

// QueryToolApp message handlers


// App command to run the dialog
void QueryToolApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// QueryToolApp customization load/save methods

void QueryToolApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void QueryToolApp::LoadCustomState()
{
}

void QueryToolApp::SaveCustomState()
{
}

bool    
QueryToolApp::AppStarted25Times()
{
  bool result = false;
  CString key("HKCU\\Software\\");
  key += PRODUCT_REGISTRY;
  CString amount("TimesStarted");

  RegistryManager manager;
  int times = manager.GetRegistryInteger(key,amount,0);
  if((++times % 25) == 0)
  {
    result = true;
  }
  manager.SetRegistryInteger(key,amount,times);
  return result;
}


//////////////////////////////////////////////////////////////////////////
// 
// QueryToolApp message handlers
//
//////////////////////////////////////////////////////////////////////////

void 
QueryToolApp::OnEditPermanetSettings()
{
  try
  {
    COEDocument::ShowSettingsDialog();
    setlocale(LC_ALL, COEDocument::GetSettingsManager().GetGlobalSettings().GetLocale().c_str());
    UpdateAccelAndMenu();
    RefreshODBCPanels();
  }
  _OE_DEFAULT_HANDLER_;
}

void 
QueryToolApp::OnUpdateEditIndicators(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(FALSE);
}

void 
QueryToolApp::OnFileWatchNotify(WPARAM, LPARAM)
{
  TRACE("COpenEditorApp::OnFileWatchNotify\n");
  CFileWatch::NotifyClients();
}

void 
QueryToolApp::OnFileCloseAll()
{
  if (m_pDocManager != NULL && m_pDocManager->IsKindOf(RUNTIME_CLASS(CDocManagerExt)))
  {
    ((CDocManagerExt*)m_pDocManager)->SaveAndCloseAllDocuments();
  }
}

void 
QueryToolApp::DoFileSaveAll(bool silent, bool skipNew)
{
  bool _silent  = COEDocument::GetSaveModifiedSilent();
  bool _skipNew = COEDocument::GetSaveModifiedSkipNew();
  try
  {
    COEDocument::SetSaveModifiedSilent(silent);
    COEDocument::SetSaveModifiedSkipNew(skipNew);
    SaveAllModified();
    COEDocument::SetSaveModifiedSilent(_silent);
    COEDocument::SetSaveModifiedSkipNew(_skipNew);
  }
  catch (...)
  {
    COEDocument::SetSaveModifiedSilent(_silent);
    COEDocument::SetSaveModifiedSkipNew(_skipNew);
    throw;
  }
}

void 
QueryToolApp::OnFileSaveAll()
{
  DoFileSaveAll(/*silent*/true, /*skipNew*/false);
}

void
QueryToolApp::OnActivateApp(BOOL bActive)
{
  if (!bActive && COEDocument::GetSettingsManager().GetGlobalSettings().GetFileSaveOnSwith())
  {
    DoFileSaveAll(/*silent*/true, /*skipNew*/true);
  }
  if (bActive)
  {
    CFileWatch::ResumeThread();
  }
  else
  {
    CFileWatch::SuspendThread();
  }
}

void
QueryToolApp::OnUpdate_FileSaveAll(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(m_pDocManager != NULL && m_pDocManager->GetOpenDocumentCount());
}

void QueryToolApp::OnDblKeyAccel(UINT nID)
{
  string desc;
  m_dblKeyAccelInx = nID - ID_APP_DBLKEYACCEL_FIRST;
  if (GUICommandDictionary::GetDblKeyDescription(m_dblKeyAccelInx, desc))
  {
    desc += " was pressed. Waiting for second key of chord...";
    Common::SetStatusText(desc.c_str(), TRUE);
  }
}

void
QueryToolApp::OnSessionStatus()
{
  ConnectionDlg connect;
  connect.SetLogin(true,m_user,m_password,m_datasource,m_safty);
  connect.SetDataConnector(&m_database);
  connect.DoModal();
}

void
QueryToolApp::UpdateDatabaseConnector()
{
  CMainFrame* frame = (CMainFrame*)m_pMainWnd;
  if(DatabaseIsOpen())
  {
    // OK, It's connected
    CString conText;
    conText.Format("%s@%s", m_user, m_datasource);
    if (m_safty)
    {
      conText += " (safe)";
    }
    frame->SetConnectionText(conText);
  }
  Common::SetStatusText("", TRUE);
  // Refresh the ODBC Pane
  // TAKES TOO MUCH TIME
  // frame->OnFpwRefreshOdbc();
}

bool
QueryToolApp::DatabaseHasTransaction()
{
  return m_database.GetTransaction() != nullptr;
}

// Make a connection with a database
void 
QueryToolApp::OnConnect()
{
  if(m_database.IsOpen())
  {
    AfxMessageBox("Already connected to an ODBC datasource!");
    return;
  }
  INT_PTR result = 0;
  while(result != IDCANCEL)
  {
    // Simple connection
    MultConnectionsDlg connection;
    result = connection.DoModal();

    if(result == IDOK)
    {
      m_user          = connection.GetUser();
      m_password      = connection.GetPassword();
      m_datasource    = connection.GetDataSource();
      m_safty         = connection.GetSafty();

      OpenDatabaseConnectie();
      UpdateDatabaseConnector();
      // Refresh the ODBC info
      RefreshODBCPanels();
      return;
    }
    if(result == IDC_CON_DETAILS)
    {
      ConnectionDlg connect;
      connect.SetLogin(false  // Pre-logon
                      ,connection.GetUser()
                      ,connection.GetPassword()
                      ,connection.GetDataSource()
                      ,connection.GetSafty());
      if(connect.SetDataConnector(&m_database))
      {
        result = connect.DoModal();
        if(result == IDOK)
        {
          m_user = connect.GetUser();
          m_password = connect.GetPassword();
          m_datasource = connect.GetDataSource();
          m_safty = connect.GetSafty();
          UpdateDatabaseConnector();
          return;
        }
      }
      else
      {
        AfxMessageBox("Cannot open database. Provide correct user/password details",MB_OK | MB_ICONERROR);
      }
    }
  }
}

void
QueryToolApp::OnDisconnect()
{
  // Close a still open database
  if(m_database.IsOpen())
  {
    if(m_database.GetTransaction())
    {
      CString warning;
      warning.Format("There is still a running transaction to database: %s.\r\n"
                     "Loose your changes altogether and disconnect?",GetDatasource());
      if(AfxMessageBox(warning,MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
      {
        return;
      }
      OnODBCRollback();
    }
    m_database.Close();

    // OK, It's disconnected
    if(!m_isClosing)
    {
      CString conText = "No connection";
      ((CMainFrame*)m_pMainWnd)->SetConnectionText(conText);
    }
  }
  else
  {
    if(!m_isClosing)
    {
      AfxMessageBox("Not connected to an ODBC datasource!");
    }
  }
  // Refresh the ODBC info
  RefreshODBCPanels();
}

void
QueryToolApp::RefreshODBCPanels()
{
  // Refresh the ODBC info
  if (m_pMainWnd)
  {
    CMainFrame* frame = (CMainFrame*)m_pMainWnd;
    frame->OnRefreshOdbc();
  }
}

void
QueryToolApp::ReportCapabilities()
{
  if (m_pMainWnd)
  {
    CString filename = "C:\\ReportODBC.txt";

    if (!AfxGetApp()->DoPromptFileName(filename
                                      ,AFX_IDS_SAVEFILE
                                      ,OFN_HIDEREADONLY | OFN_PATHMUSTEXIST
                                      ,FALSE
                                      ,NULL/*pTemplate*/))
    {
      return;
    }
    CMainFrame* frame = (CMainFrame*)m_pMainWnd;
    return frame->ReportCapabilities(filename);
  }
}

void
QueryToolApp::OnODBCBegin()
{
  if (!m_database.IsOpen())
  {
    AfxMessageBox("BEGIN: There is no current connection to a database", MB_OK | MB_ICONEXCLAMATION);
    return;
  }
  m_transaction = new SQLTransaction(&m_database, "Transaction");
}

void
QueryToolApp::OnODBCCommit()
{
  if (!m_database.IsOpen())
  {
    AfxMessageBox("BEGIN: There is no current connection to a database", MB_OK | MB_ICONEXCLAMATION);
    return;
  }
  if (m_transaction)
  {
    m_transaction->Commit();
    delete m_transaction;
    m_transaction = NULL;
  }
}

void
QueryToolApp::OnODBCRollback()
{
  if (!m_database.IsOpen())
  {
    AfxMessageBox("BEGIN: There is no current connection to a database", MB_OK | MB_ICONEXCLAMATION);
    return;
  }
  if (m_transaction)
  {
    m_transaction->Rollback();
    delete m_transaction;
    m_transaction = NULL;
  }
}

CString
QueryToolApp::GetExeDirectory(void)
{
  CString sCommandLine = GetCommandLine();   
  CString sRetval;
  if(sCommandLine.Find('"')>=0)
  // stands between " 
  {
    int pos1 = sCommandLine.Find('"');
    int pos = sCommandLine.Find('"',pos1+1);
    if(pos >= 0)
    {
      sRetval = sCommandLine.Mid(1,pos-1);
    }
    else
    {
      sRetval = sCommandLine.Mid(1);
    }
  }
  else
  // Not between ", so take a space as the bordering of the words
  {
    int pos = sCommandLine.Find(' ');
    if(pos >= 0)
    {
      sRetval = sCommandLine.Left(pos);
    }
    else
    {
      sRetval = sCommandLine;
    }
  }
  // Strip
  int slash = sRetval.ReverseFind('\\');  
  if(slash==-1)
  {
    slash = sRetval.ReverseFind('/');
  }
  if(slash==-1)
  {
    slash = sRetval.ReverseFind(':');
  }
  if(slash==-1)
  {
    return "";
  }
  return sRetval.Left(slash);
}
void 
QueryToolApp::OnCHMHelp()
{
  CString directory = GetExeDirectory();
  directory.MakeUpper();
  RunShellCommand((char *)directory.GetString()
                 ,"open"
                 ,"ODBCQueryTool.chm"
                 ,"");
}

void
QueryToolApp::RunShellCommand(const char* directory
                             ,const char* command
                             ,const char* filename
                             ,const char* parameters)
{
  HWND hwnd = GetMainWnd()->GetSafeHwnd();
  HINSTANCE res = ShellExecute(hwnd         // Controlling window
                              ,command      // "open" o.i.d. DDE Command
                              ,filename     // do it on this file
                              ,parameters   // with this parameters
                              ,directory    // scene of the crime
                              ,SW_MAXIMIZE);// Full scale visibility
  if ((DWORD_PTR)res < 32)
	{
		char buffer[1025];
		char fout[100];
		DWORD error = GetLastError();
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |
									FORMAT_MESSAGE_IGNORE_INSERTS,
									NULL,
									error,
									MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
									(LPTSTR)&buffer,
									500,
									NULL
		);
		strcat_s(buffer,1024,"\r\nFile: ");
		strcat_s(buffer,1024,filename);
		sprintf_s(fout,100,"\r\nMS-Windows error: %d", error);
		strcat_s(buffer,1024,fout);
		// Display the string. 
		StyleMessageBox(m_pMainWnd,buffer,"ODBC QueryTool",MB_OK|MB_ICONINFORMATION);
	}
}

SQLDatabase&
QueryToolApp::GetDatabase()
{
  return m_database;
}

bool
QueryToolApp::ParseConnectString()
{
  int pos = 0;
  CString temp;
  m_datasource = "";
  m_user       = "";
  m_password   = "";
  
  // Datasource name
  pos = m_connectString.Find("DSN=");
  if(pos >= 0)
  {
    temp = m_connectString.Mid(pos + 4);
    pos  = temp.Find(';');
    if(pos > 0)
    {
      m_datasource = temp.Left(pos);
    }
    else
    {
      m_datasource = temp;
    }
  }
  // Username
  pos = m_connectString.Find("UID=");
  if(pos >= 0)
  {
    temp = m_connectString.Mid(pos + 4);
    pos  = temp.Find(';');
    if(pos > 0)
    {
      m_user = temp.Left(pos);
    }
    else
    {
      m_user = temp;
    }
  }
  // Password
  pos = m_connectString.Find("PWD=");
  if(pos >= 0)
  {
    temp = m_connectString.Mid(pos + 4);
    pos  = temp.Find(';');
    if(pos > 0)
    {
      m_password = temp.Left(pos);
    }
    else
    {
      m_password = temp;
    }
  }
  if(!m_datasource.IsEmpty() && !m_user.IsEmpty() && !m_password.IsEmpty())
  {
    return true;
  }
  return false;
}

// Creating and opening a database (m_database)
bool
QueryToolApp::OpenDatabaseConnectie()
{
  bool didError = false;
  CString connectStr;
  connectStr += "DSN=";
  connectStr += m_datasource;
  connectStr += ";UID=";
  connectStr += m_user;
  connectStr += ";PWD=";
  connectStr += m_password;
  char buffer[1024];
  CString status;
  status.Format("Trying to connect to: %s", m_datasource);

  // Reset the connection status on the DialogBar
  CMainFrame* frame = (CMainFrame*)m_pMainWnd;
  frame->SetConnectionText((CString)"No connection");

  try
  {
    Common::SetStatusText(status, TRUE);
    CWaitCursor take_a_deep_sigh;
    m_database.IsOpen();
    {
      m_database.Close();
    }
    m_database.Open(connectStr, m_safty);
  }
  catch (CString& error)
  {
    strncpy_s(buffer,1024,error.GetString(), 1024);
    didError = true;
  }
  catch (CException* e)
  {
    e->ReportError();
    e->Delete();
    e->GetErrorMessage(buffer, 1024);
    didError = true;
  }
  catch (...)
  {
    strcpy_s(buffer,1024,"No connection with the database");
    didError = true;
  }
  if (didError)
  {
    CString buffer2(buffer);

    if (m_password != "")
    {
      m_password = "********";
    }
    buffer2 += "\r\nDatasource = \"";
    buffer2 += m_datasource;
    buffer2 += "\" User = \"";
    buffer2 += m_user;
    buffer2 += "\" Password = \"";
    buffer2 += m_password;
    AfxMessageBox(buffer2);
    Common::SetStatusText("", TRUE);
    return false;
  }
  return true;
}

void
QueryToolApp::SelectFirst100(CString& p_table)
{
  if (m_pMainWnd)
  {
    CWaitCursor take_a_deep_sigh;

    // Get the temp directory
    CString directory;
    directory.GetEnvironmentVariable("TMP");
    directory.TrimRight('\\');
    // Get a filename for the table
    CString filename(directory);
    filename += "\\";
    filename += "Select_";
    filename += p_table;
    filename += ".sql";

    try
    {
      CString select("SELECT * FROM ");
      select += p_table;
      select += ";\n";

      FILE* fp = nullptr;
      fopen_s(&fp, filename, "w");
      if (fp)
      {
        fputs(select, fp);
        fclose(fp);
      }

      // Open as a document
      AfxGetApp()->OpenDocumentFile(filename);
      // Execute the script
      m_pMainWnd->PostMessage(WM_COMMAND, ID_SCRIPT_CURRENT, 0);

    }
    catch (CString& error)
    {
      DoMessageBox("Cannot create select script for table: " + p_table + "\n" + error, MB_OK | MB_ICONERROR, 0);
    }
  }
}

void
QueryToolApp::TableDDL(CString& p_table)
{
  if (m_pMainWnd)
  {
    CWaitCursor take_a_deep_sigh;

    // Get the temp directory
    CString directory;
    directory.GetEnvironmentVariable("TMP");
    directory.TrimRight('\\');
    // Get a filename for the table
    CString filename(directory);
    filename += "\\";
    filename += "CreateTable_";
    filename += p_table;
    filename += ".sql";

    try
    {
      DDLCreateTable create(m_database.GetSQLInfoDB());
      create.GetTableDDL(p_table);
      create.SaveDDL(filename);
      AfxGetApp()->OpenDocumentFile(filename);
    }
    catch (CString& error)
    {
      DoMessageBox("Cannot create DDL for table: " + p_table + "\n" + error, MB_OK | MB_ICONERROR, 0);
    }
  }
}

CString
QueryToolApp::FindNativeSQL(CString& command)
{
  if(DatabaseIsOpen())
  {
    CString sql(command);
    if(GetDatabase().ODBCNativeSQL(sql))
    {
      return sql;
    }
  }
  return command;
}

void
QueryToolApp::FindTable(CString& table)
{
  if(m_pMainWnd)
  {
    CMainFrame* frame = (CMainFrame*)m_pMainWnd;
    frame->FindTable(table);
  }
}

// Override 
BOOL
QueryToolApp::SaveAllModified()
{
  if (m_database.GetTransaction())
  {
    CString warning;
    warning.Format("There is still a running transaction to database: %s.\r\n"
                   "Loose your changes altogether and disconnect?", GetDatasource());
    if (AfxMessageBox(warning, MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
    {
      return FALSE;
    }
    // Rollback the transaction
    OnODBCRollback();
  }
  return CWinApp::SaveAllModified();
}

int
QueryToolApp::DoMessageBox(LPCTSTR lpszPrompt, UINT nType, UINT nIDPrompt)
{
  HWND hwnd = NULL;
  if (m_pMainWnd)
  {
    hwnd = m_pMainWnd->GetSafeHwnd();
  }
  CString title;
  title.LoadString(IDR_MAINFRAME);

  return (int) StyleMessageBox(m_pMainWnd,lpszPrompt,title,nType);
}

void
QueryToolApp::SetTitle(CString p_document)
{
  if(m_pMainWnd->GetSafeHwnd())
  {
    CString title;
    title.LoadString(IDR_MAINFRAME);
    if(!p_document.IsEmpty())
    {
      title += " - ";
      title += p_document;
    }
    m_pMainWnd->SetWindowText(title);
  }
}

BOOL 
QueryToolApp::PreTranslateMessage(MSG* pMsg)
{
  if (m_dblKeyAccelInx != -1)
  {
    Common::Command commandId;
    Common::VKey vkey = static_cast<Common::VKey>(pMsg->wParam);

    if (pMsg->message == WM_KEYDOWN && GUICommandDictionary::GetDblKeyAccelCmdId(m_dblKeyAccelInx, vkey, commandId))
    {
      pMsg->message = WM_NULL;
      m_dblKeyAccelInx = -1;
      Common::SetStatusText("", TRUE);
      if (m_pMainWnd)
      {
        m_pMainWnd->SendMessage(WM_COMMAND, commandId);
      }
      return FALSE;
    }
    else if ((pMsg->message == WM_KEYDOWN)
          || (pMsg->message == WM_COMMAND)
          || (pMsg->message == WM_SYSCOMMAND)
          || (pMsg->message > WM_MOUSEFIRST && pMsg->message <= WM_MOUSELAST))
    {
      string desc;
      GUICommandDictionary::GetDblKeyDescription(m_dblKeyAccelInx, desc, vkey);
      desc += "   is not an acceleration sequence.";
      Common::SetStatusText(desc.c_str(), TRUE);
      pMsg->message = WM_NULL;
      m_dblKeyAccelInx = -1;
      MessageBeep((UINT)-1);
      return FALSE;
    }
  }
  return CWinApp::PreTranslateMessage(pMsg);
}

BOOL 
QueryToolApp::OnIdle(LONG lCount)
{
  BOOL more = FALSE;

  try
  {
    more = CWinApp::OnIdle(lCount);
  }
  catch (...) { /*it's silent*/ }

  return more;
}

