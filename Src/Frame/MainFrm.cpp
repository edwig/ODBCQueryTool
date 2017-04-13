/* 
    Copyright (C) 2002 Aleksey Kochetov
                  2004 Edwig Huisman (Database stuff)
                  2015 Edwig Huisman (Support stuff)

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
#include "COMMON/AppGlobal.h"
#include "MainFrm.h"
#include "OpenEditorApp.h"
#include "OpenEditor/OEDocument.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

    LPCSTR CMDIMainFrame::m_cszClassName = "ODBCQueryTool.OpenEditor";

/////////////////////////////////////////////////////////////////////////////
// CMDIMainFrame

IMPLEMENT_DYNAMIC(CMDIMainFrame, CWorkbookMDIFrame)

BEGIN_MESSAGE_MAP(CMDIMainFrame, CWorkbookMDIFrame)
    ON_WM_CREATE()
    ON_WM_COPYDATA()
    ON_WM_ACTIVATEAPP()
    ON_WM_KEYDOWN()
    ON_COMMAND(ID_VIEW_CONNECTBAR,           OnConnectBar)
    ON_COMMAND(ID_VIEW_DATABASEBAR,          OnDatabaseBar)
    ON_COMMAND(ID_HELP_BUG,                  OnSupportBug)
    ON_COMMAND(ID_HELP_FEATURE,              OnSupportFeature)
    ON_COMMAND(ID_HELP_SFNET,                OnWebSourceforge)
    ON_COMMAND(ID_HELP_GITHUB,               OnWebGithub)
    ON_UPDATE_COMMAND_UI(ID_ODBC_CONNECT,    OnUpdateConnect)
    ON_UPDATE_COMMAND_UI(ID_ODBC_DISCONNECT, OnUpdateDisconnect)
    ON_UPDATE_COMMAND_UI(ID_ODBC_BEGIN,      OnUpdateBegin)
    ON_UPDATE_COMMAND_UI(ID_ODBC_COMMIT,     OnUpdateCommit)
    ON_UPDATE_COMMAND_UI(ID_ODBC_ROLLBACK,   OnUpdateRollback)
    ON_UPDATE_COMMAND_UI(ID_SESSION_ODBCREPORT,OnUpdateODBCReport)
    ON_UPDATE_COMMAND_UI(ID_VIEW_CONNECTBAR, OnUpdateConnectBarVisible)
    ON_UPDATE_COMMAND_UI(ID_VIEW_DATABASEBAR,OnUpdateDatabaseBarVisible)
    ON_UPDATE_COMMAND_UI(ID_SESSIONSTATUS,   OnUpdateSessionStatus)
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

static UINT BASED_CODE CONIndicators[] =
{
  ID_ODBC_CONNECT
 ,ID_ODBC_DISCONNECT
 ,ID_SEPARATOR
 ,ID_SEPARATOR // Edit box connection
};

/////////////////////////////////////////////////////////////////////////////
// CMDIMainFrame construction/destruction

CMDIMainFrame::CMDIMainFrame ()
              :m_conBarVisible(true)
              ,m_dbBarVisible(true)
{
}

CMDIMainFrame::~CMDIMainFrame ()
{
}

int CMDIMainFrame::OnCreate (LPCREATESTRUCT lpCreateStruct)
{
    if (CWorkbookMDIFrame::OnCreate(lpCreateStruct) == -1)
    {
      return -1;
    }
    if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
            | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(4, 2, 4, 2)) 
    || !m_wndToolBar.LoadToolBar(IDR_OPENEDITOR))
    {
        TRACE0("CMDIMainFrame::OnCreate: Failed to create window toolbar\n");
        return -1;      // fail to create
    }
    if (!m_dbToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
            | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(4, 2, 4, 2)) 
    || !m_dbToolBar.LoadToolBar(IDR_DATABASE))
    {
        TRACE0("CMDIMainFrame::OnCreate: Failed to create database toolbar\n");
        return -1;      // fail to create
    }
    if(!m_conToolBar.CreateEx(this
                           ,TBSTYLE_FLAT , WS_CHILD   | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | 
                            CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect (4, 2, 4, 2))
    || !m_conToolBar.LoadBitmap(IDR_CONNECTBAR)
    || !m_conToolBar.SetButtons(CONIndicators,sizeof(CONIndicators)/sizeof(UINT)))
    {
        TRACE0("CMDIMainFrame::OnCreate: Failed to create connectbar\n");
        return -1;      // fail to create
    }
    else
    {
      CRect rect;
      SIZE  butSize;
      SIZE  iconSize;
      butSize.cx = 26; butSize.cy = 26;
      iconSize.cx = 19; iconSize.cy =19;
      m_conToolBar.SetSizes(butSize,iconSize);
      m_conToolBar.SetButtonInfo(3, IDC_CUR_CONNECTION, TBBS_SEPARATOR, 180);
      m_conToolBar.GetItemRect(3, &rect);
      rect.DeflateRect(2,2,2,2);
      if(!m_conToolBar.m_rectangle.Create(""
                                         ,SS_SUNKEN
                                         ,rect
                                         ,&m_conToolBar))
      {
        TRACE0("Cannot create connect toolbar rectangle");
      }
      rect.DeflateRect(2,4,2,2);

      if(!m_conToolBar.m_connectText.Create("No connection"
                                           ,SS_CENTER 
                                           ,rect
                                           ,&m_conToolBar
                                           ,IDC_CUR_CONNECTION))
      {
        TRACE0("CMIDMainFrame: Kan connection edit box maken");
        return -1;
      }
      //  Font maken
      LOGFONT logFont;
      memset(&logFont, 0, sizeof(logFont));
      logFont.lfHeight = -12;
      logFont.lfPitchAndFamily = VARIABLE_PITCH | FF_SWISS;
      lstrcpy(logFont.lfFaceName, "MS Sans Serif");
      if (!m_conToolBar.m_font.CreateFontIndirect(&logFont))
      {
        TRACE0("Kan font voor ombo voor historiebalk niet maken");
      }
      else
      {
        m_conToolBar.m_connectText.SetFont(&m_conToolBar.m_font);
      }
    }

    if (!m_wndStatusBar.Create(this) 
    || !m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT)))
    {
        TRACE0("CMDIMainFrame::OnCreate: Failed to create status bar\n");
        return -1;      // fail to create
    }
    m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
    m_dbToolBar. EnableDocking(CBRS_ALIGN_ANY);
    m_conToolBar.EnableDocking(CBRS_ALIGN_ANY);
    EnableDocking(CBRS_ALIGN_ANY);
    DockControlBar(&m_wndToolBar);
    DockControlBar(&m_conToolBar);
    DockControlBar(&m_dbToolBar);
    DockControlBarLeftOf(&m_wndToolBar,&m_conToolBar);
    DockControlBarLeftOf(&m_conToolBar,&m_dbToolBar);

    m_conToolBar.m_rectangle.  ShowWindow(SW_SHOW);
    m_conToolBar.m_connectText.ShowWindow(SW_SHOW);

    if (CWorkbookMDIFrame::DoCreate() == -1)
    {
        TRACE0("CMDIMainFrame::OnCreate: Failed to create CWorkbookMDIFrame\n");
        return -1;      // fail to create
    }
    Common::SetStatusHwnd(m_wndStatusBar.m_hWnd);

    return 0;
}


BOOL CMDIMainFrame::PreCreateWindow (CREATESTRUCT& cs)
{
    WNDCLASS wndClass;
    BOOL bRes = CWorkbookMDIFrame::PreCreateWindow(cs);
    HINSTANCE hInstance = AfxGetInstanceHandle();

    // see if the class already exists
    if (!::GetClassInfo(hInstance, m_cszClassName, &wndClass)) 
    {
        // get default stuff
        ::GetClassInfo(hInstance, cs.lpszClass, &wndClass);
        wndClass.lpszClassName = m_cszClassName;
        wndClass.hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
        // register a new class
        if (!AfxRegisterClass(&wndClass))
            AfxThrowResourceException();
    }
    cs.lpszClass = m_cszClassName;

    return bRes;
}

BOOL CMDIMainFrame::OnCopyData (CWnd*, COPYDATASTRUCT* pCopyDataStruct)
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

//#if _MFC_VER > 0x0600

void CMDIMainFrame::OnActivateApp(BOOL bActive, DWORD dwThreadID)
{
    CWorkbookMDIFrame::OnActivateApp(bActive, dwThreadID);
    
	if (AfxGetApp()->IsKindOf(RUNTIME_CLASS(COpenEditorApp)))
        ((COpenEditorApp*)AfxGetApp())->OnActivateApp(bActive);
}

//#else

//void CMDIMainFrame::OnActivateApp(BOOL bActive, HTASK hTask)
//{
//    CWorkbookMDIFrame::OnActivateApp(bActive, hTask);
    
//	if (AfxGetApp()->IsKindOf(RUNTIME_CLASS(COpenEditorApp)))
//        ((COpenEditorApp*)AfxGetApp())->OnActivateApp(bActive);
//}

void 
CMDIMainFrame::SetConnectionText(CString& text)
{
  if(!m_conToolBar.IsVisible())
  {
    // Then show it immediatly
    ShowControlBar(&m_conToolBar,TRUE,FALSE);
  }
  // Set the connection text
  m_conToolBar.m_connectText.SetWindowText(text);
}

void 
CMDIMainFrame::OnConnectBar()
{
  m_conBarVisible = !m_conBarVisible;
  ShowControlBar(&m_conToolBar,m_conBarVisible,TRUE);
}

void
CMDIMainFrame::OnDatabaseBar()
{
  m_dbBarVisible = !m_dbBarVisible;
  ShowControlBar(&m_dbToolBar,m_dbBarVisible,TRUE);
}

void
CMDIMainFrame::OnUpdateConnect(CCmdUI* pCmdUI)
{
  COpenEditorApp *app = dynamic_cast<COpenEditorApp *> (AfxGetApp());
  pCmdUI->Enable(!app->DatabaseIsOpen());
}

void
CMDIMainFrame::OnUpdateDisconnect(CCmdUI* pCmdUI)
{
  COpenEditorApp *app = dynamic_cast<COpenEditorApp *> (AfxGetApp());
  pCmdUI->Enable(app->DatabaseIsOpen());
}

void
CMDIMainFrame::OnUpdateConnectBarVisible(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(m_conBarVisible);
}

void
CMDIMainFrame::OnUpdateDatabaseBarVisible(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(m_dbBarVisible);
}

void 
CMDIMainFrame::OnUpdateBegin(CCmdUI* pCmdUI)
{
  COpenEditorApp *app = dynamic_cast<COpenEditorApp *> (AfxGetApp());
  pCmdUI->Enable(app->DatabaseIsOpen() && !app->DatabaseHasTransaction());
}

void 
CMDIMainFrame::OnUpdateCommit(CCmdUI* pCmdUI)
{
  COpenEditorApp *app = dynamic_cast<COpenEditorApp *> (AfxGetApp());
  pCmdUI->Enable(app->DatabaseHasTransaction());
}

void 
CMDIMainFrame::OnUpdateRollback(CCmdUI* pCmdUI)
{
  COpenEditorApp *app = dynamic_cast<COpenEditorApp *> (AfxGetApp());
  pCmdUI->Enable(app->DatabaseHasTransaction());
}

void
CMDIMainFrame::OnUpdateODBCReport(CCmdUI* pCmdUI)
{
  COpenEditorApp *app = dynamic_cast<COpenEditorApp *> (AfxGetApp());
  pCmdUI->Enable(app->DatabaseIsOpen());
}

void
CMDIMainFrame::OnUpdateSessionStatus(CCmdUI* pCmdUI)
{
  COpenEditorApp *app = dynamic_cast<COpenEditorApp *> (AfxGetApp());
  pCmdUI->Enable(app->DatabaseIsOpen());
}

void
CMDIMainFrame::OnSupportBug()
{
  SendSupportEmail("Bug report for Open-ODBCQueryTool","The following functionality is broken...");
}

void
CMDIMainFrame::OnSupportFeature()
{
  SendSupportEmail("Feature request for Open-ODBCQueryTool","I suggest that you....");
}

void
CMDIMainFrame::SendSupportEmail(CString p_subject,CString p_body)
{
  CString command("open");
  CString parameters("mailto:edwig.huisman@hetnet.nl");

  parameters.AppendFormat("?Subject=%s",p_subject);
  parameters.AppendFormat("&Body=%s",p_body);

  CWaitCursor aSigh;
  ShellExecute(GetSafeHwnd(),command,parameters,NULL,NULL,SW_SHOWNORMAL);
}

void
CMDIMainFrame::OnWebSourceforge()
{
  CString source("https://sourceforge.net/projects/odbcquerytool/");
  ShellExecute(NULL,"open",source,NULL,NULL,SW_SHOWMAXIMIZED);
}

void
CMDIMainFrame::OnWebGithub()
{
  CString github("https://github.com/edwig/ODBCQueryTool/");
  ShellExecute(NULL,"open", github,NULL,NULL,SW_SHOWMAXIMIZED);
}

//#endif
