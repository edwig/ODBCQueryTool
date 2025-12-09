////////////////////////////////////////////////////////////////////////
//
// File: StyleDialog.cpp
// Function: General Dialog box for styling windows
//
// Dialog with special styled header and behavior
// Paints the frame OUTSIDE the client area (defined in the resources)
//
//   _____ _         _ _             ______                                           _    
//  / ____| |       | (_)           |  ____|                                         | |   
// | (___ | |_ _   _| |_ _ __   __ _| |__ _ __ __ _ _ __ ___   _____      _____  _ __| | __
//  \___ \| __| | | | | | '_ \ / _` |  __| '__/ _` | '_ ` _ \ / _ \ \ /\ / / _ \| '__| |/ /
//  ____) | |_| |_| | | | | | | (_| | |  | | | (_| | | | | | |  __/\ V  V / (_) | |  |   < 
// |_____/ \__|\__, |_|_|_| |_|\__, |_|  |_|  \__,_|_| |_| |_|\___| \_/\_/ \___/|_|  |_|\_\
//              __/ |           __/ |                                                      
//             |___/           |___/                                                       
//
//
// Author: ir. W.E. Huisman
// For license: See the file "LICENSE.txt" in the root folder
//
#include "stdafx.h"
#include "RegistryManager.h"
#include "StyleUtilities.h"
#include <ShellScalingApi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace ThemeColor;

static UINT auIDStatusBar[] =
{
  ID_SEPARATOR
};

IMPLEMENT_DYNAMIC(StyleDialog,CDialogEx);

StyleDialog::StyleDialog(UINT  p_IDTemplate
                        ,CWnd* p_parentWnd
                        ,bool  p_caption
                        ,bool  p_sysmenu
                        ,bool  p_status) 
            :CDialogEx(p_IDTemplate, p_parentWnd)
            ,m_caption(p_caption)
            ,m_mnuButton(p_sysmenu)
            ,m_hasStatus(p_status)
{
  int msg_changed = RegisterWindowMessage(STYLEFXCHANGED);
  g_msg_changed   = msg_changed;
  if(p_sysmenu)
  {
    LoadStyleTheme();
  }
  // Load the default icon
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
  // Set default background brush
  m_defaultBrush.DeleteObject();
  m_defaultBrush.CreateSolidBrush(ThemeColor::GetColor(Colors::ColorWindowFrame));

  // See if we must preserve our position
  if(p_parentWnd == nullptr)
  {
    m_saveMonitor = true;
  }
}

StyleDialog::~StyleDialog()
{
}

BEGIN_MESSAGE_MAP(StyleDialog,CDialogEx)
  ON_WM_CREATE()
  ON_WM_DESTROY()
  ON_WM_ERASEBKGND()
  ON_WM_CTLCOLOR()
  ON_WM_NCMOUSEMOVE()
  ON_WM_NCLBUTTONDOWN()
  ON_WM_NCRBUTTONUP()
  ON_WM_NCLBUTTONDBLCLK()
  ON_WM_NCMOUSELEAVE()
  ON_WM_SIZE()
  ON_WM_PAINT()
  ON_WM_NCCALCSIZE()
  ON_WM_NCPAINT()
  ON_WM_NCHITTEST()
  ON_WM_SYSCOMMAND()
  ON_WM_ACTIVATE()
  ON_WM_ACTIVATEAPP()
  ON_WM_SETTINGCHANGE()
  ON_WM_QUERYDRAGICON()
  ON_WM_DROPFILES()
  ON_REGISTERED_MESSAGE(g_msg_changed,OnStyleChanged)
  ON_NOTIFY_EX(TTN_NEEDTEXT,0,        OnToolTipNotify)
  ON_MESSAGE(WM_CTLCOLORSTATIC,       OnCtlColorStatic)
  ON_MESSAGE(WM_CTLCOLORLISTBOX,      OnCtlColorListBox)
  ON_MESSAGE(WM_GETDPISCALEDSIZE,     OnGetDpiScaledSize)
  ON_MESSAGE(WM_DPICHANGED,           OnDpiChanged)
  ON_MESSAGE(WM_DISPLAYCHANGE,        OnDisplayChange)
END_MESSAGE_MAP()

BOOL
StyleDialog::OnInitDialog()
{
  // Needed for the shadow border
  ModifyStyleEx(0, WS_EX_LAYERED);
  SetLayeredWindowAttributes(ColorWindowTransparent, 0, LWA_COLORKEY);

  // Make room for the shadow border
  int border = WINDOWSHADOWBORDER(m_hWnd);
  CRect window;
  GetWindowRect(window);
  SetWindowPos(nullptr
              ,0,0
              ,window.Width()  + border
              ,window.Height() + border
              ,SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);


  // Standard OnInitDialog
  CDialogEx::OnInitDialog();

  // Create status bar
  if(m_hasStatus)
  {
    InitStatusBar();
  }
  return InitFirstFocus();
}

// In order to be working correctly:
// call "SetCanResize()" prior to "PostInitDialog()"
// in all your applications
void
StyleDialog::PostInitDialog()
{
  if(m_saveMonitor)
  {
    StyleRestoreWindowPosition(this);
  }
}

int
StyleDialog::OnCreate(LPCREATESTRUCT p_create)
{
  p_create->dwExStyle |= WS_EX_CONTROLPARENT;

  int res = CDialogEx::OnCreate(p_create);

  // Getting the DPI
  GetDpi(GetSafeHwnd(),m_dpi_x,m_dpi_y);
  return res;
}

void
StyleDialog::OnDestroy()
{
  // Save window position on the monitor
  if(m_saveMonitor)
  {
    StyleSaveWindowPosition(this);
  }
  CDialogEx::OnDestroy();
}

void
StyleDialog::InitStatusBar()
{
  if(m_statusBar.Create(this,WS_CHILD|WS_VISIBLE|CBRS_BOTTOM|CBRS_GRIPPER|SBARS_SIZEGRIP))
  {
    CFont* font = GetSFXFont(GetSafeHwnd(),StyleFontType::DialogFont);
    m_statusBar.SetFont(font);
    m_statusBar.SetIndicators(auIDStatusBar,sizeof(auIDStatusBar) / sizeof(UINT));
    m_statusBar.SetWindowText(_T(""));
    m_statusBar.SetPaneStyle(0, SBPS_STRETCH|SBPS_NOBORDERS);
    RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0);
  }
  else
  {
    AfxMessageBox(_T("Error: Cannot create a dialog statusbar"));
  }
}

// NOW MISSING FROM AFX in HandleInitDialog()
BOOL
StyleDialog::InitFirstFocus()
{
  BOOL bResult = TRUE;
  CWnd* pWndNext = GetNextDlgTabItem(nullptr);
  if(pWndNext != NULL)
  {
    // Activate first control in the dialog
    CString text;
    pWndNext->GetWindowText(text);
    if(text.Compare(_T("CXSF")) == 0)
    {
      SkinScrollWnd* skin = dynamic_cast<SkinScrollWnd*>(pWndNext);
      if (skin)
      {
        CWnd* control = skin->m_wndLimit.GetWindow(GW_CHILD);
        if (control)
        {
          pWndNext = control;
        }
      }
    }
    pWndNext->SetFocus();
    bResult = FALSE;
  }
  return bResult;
}

// This version of GetNextDlgTabItem knows about SkinScrollWnd skins
// and uses the top skin as the sibling windows in the tab order
CWnd* 
StyleDialog::GetNextDlgTabItem(CWnd* p_control, BOOL p_previous /*= FALSE*/) const
{
  CWnd* found = nullptr;
  CWnd* next  = nullptr;
  if (p_control)
  {
    next = p_previous ? p_control->GetWindow(GW_HWNDPREV) : p_control->GetWindow(GW_HWNDNEXT);
  }
  else
  {
    // If no starting point given, use first control in the dialog
    p_control = GetWindow(GW_CHILD);
    next = p_control;
    // Find a skin if possible
    SkinScrollWnd* skin = dynamic_cast<SkinScrollWnd*>((CWnd*)GetWindowLongPtr(GetSafeHwnd(), GWLP_USERDATA));
    if(skin)
    {
      next = skin;
    }
  }

  // Finding the next tabstop
  while(p_control && next && (p_control != next))
  {
    if(next->GetExStyle() & WS_EX_CONTROLPARENT)
    {
      CWnd* deeper = next->GetNextDlgGroupItem(nullptr,p_previous);
      if(deeper)
      {
        found = deeper;
        break;
      }
    }
    if(next->GetStyle() & WS_TABSTOP)
    {
      found = next;
      break;
    }
    next = p_previous ? p_control->GetWindow(GW_HWNDPREV) : p_control->GetWindow(GW_HWNDNEXT);
  }

  // If found & a skin, use the control in the skin as the next found window
  if(found)
  {
    SkinScrollWnd* skin = dynamic_cast<SkinScrollWnd*>(found);
    if(skin)
    {
      CWnd* control = skin->m_wndLimit.GetWindow(GW_CHILD);
      if(control)
      {
        found = control;
      }
    }
  }
  return found;
}

INT_PTR 
StyleDialog::DoModal()
{
  AutoStyleGrayScreen grayscreen;
  return CDialogEx::DoModal();
}

INT_PTR 
StyleDialog::DoModal(bool p_showGrayScreen)
{
  if(p_showGrayScreen)
  {
    AutoStyleGrayScreen grayscreen;
    return CDialogEx::DoModal();
  }
  return CDialogEx::DoModal();
}

BOOL 
StyleDialog::PreTranslateMessage(MSG* p_msg)
{
  if(PreTranslateMenu(p_msg))
  {
    return TRUE;
  }

  for(auto& wnd : m_controlPlanes)
  {
    if(wnd->GetSafeHwnd() && wnd->IsDialogMessage(p_msg))
    {
      return TRUE;
    }
  }
  return CDialogEx::PreTranslateMessage(p_msg);
}

// Process the menu items.
//
BOOL StyleDialog::PreTranslateMenu(MSG* p_msg)
{
  if(p_msg->message == WM_KEYDOWN)
  {
    if(m_menuFocus)
    {
      switch(p_msg->wParam)
      {
#ifdef _DEBUG
        case VK_F11:    // Visual Studio F12 is DebugBreak-UI !
#else
        case VK_F12:    // Fall through
#endif
        case VK_ESCAPE: m_menuFocus = false;
                        m_curhit = -1;
                        break;
        case VK_RETURN: // Fall through
        case VK_SPACE:  // Fall through
        case VK_DOWN:   PerformMenu();
                        // Fall through
        default:        m_menuFocus = false;
                        m_curhit = -1;
                        break;
      }
      OnNcPaint();
      return TRUE;
    }
#ifdef _DEBUG
    else if(p_msg->wParam == VK_F11)
#else
    else if(p_msg->wParam == VK_F12)
#endif
    {
      m_menuFocus = true;
      m_curhit = HTMENU;
      OnNcPaint();
      return TRUE;
    }
  }
  return FALSE;
}

void    
StyleDialog::ShowSysMenu(bool p_sysmenu) 
{ 
  m_mnuButton = p_sysmenu;
  LoadStyleTheme();
  PositionButtons();
  ReDrawFrame();
};

void
StyleDialog::ShowMinMaxButton(bool p_minButton,bool p_maxButton)
{
  m_minButton = p_minButton;
  m_maxButton = p_maxButton;
  PositionButtons();
  ReDrawFrame();
}

void
StyleDialog::SetCanResize(bool p_resize)
{
  m_canResize = p_resize;

  if(p_resize && IsDynamicLayoutEnabled())
  {
    // Do not bother: it's already 'ON'
    return;
  }
  if(m_originalSize.Width() == 0 && m_originalSize.Height() == 0)
  {
    GetWindowRect(m_originalSize);
  }
  if(p_resize)
  {
    SetupDynamicLayout();
  }
  else
  {
    EnableDynamicLayout(FALSE);
  }
}

void
StyleDialog::SetSaveMonitor(bool p_save /*= true*/)
{
  m_saveMonitor = p_save;
}

void 
StyleDialog::ShowCloseButton(bool p_show)
{
  if(m_closeButton != p_show)
  {
    m_closeButton = p_show;
  }
  PositionButtons();
  ReDrawFrame();
}

void
StyleDialog::ShowGripper(bool p_gripper /*= true*/)
{
  m_hasGripper = p_gripper;
  if(p_gripper)
  {
    SetCanResize(true);
  }
}

void
StyleDialog::SetupDynamicLayout()
{
  auto manager = GetDynamicLayout();
  if (manager == nullptr)
  {
    EnableDynamicLayout();
    manager = GetDynamicLayout();
  }
  if(manager != nullptr)
  {
    manager->Create(this);
    LoadDynamicLayoutResource(m_lpszTemplateName);
  }
}

BOOL
StyleDialog::SetSysMenu(UINT p_menuResource)
{
  ShowSysMenu();
  m_sysmenu = p_menuResource;
  if(m_menu.GetSafeHmenu())
  {
    m_menu.DestroyMenu();
  }
  return m_menu.LoadMenu(m_sysmenu);
}

// Mostly generated by the MFC Wizard of Visual Studio
// Here replaced by one single method
// p_command  is normally IDM_ABOUTBOX
// p_mentitle is normally IDS_ABOUTBOX
//
// To be called as: SetAboutBoxAndIcon(IDM_ABOUTBOX,IDS_ABOUTBOX)
// from within the "OnInitDialog()"
void
StyleDialog::SetAboutBoxAndIcon(int p_command,int p_menutitle)
{
  // IDM_ABOUTBOX must be in the system command range.
  ASSERT((IDM_ABOUTBOX & 0xFFF0) == p_command);
  ASSERT (IDM_ABOUTBOX < 0xF000);

  CMenu* pSysMenu = GetSystemMenu(FALSE);
  if (pSysMenu != nullptr)
  {
    BOOL bNameValid;
    CString strAboutMenu;
    bNameValid = strAboutMenu.LoadString(p_menutitle);
    ASSERT(bNameValid);
    if (!strAboutMenu.IsEmpty())
    {
      pSysMenu->AppendMenu(MF_SEPARATOR);
      pSysMenu->AppendMenu(MF_STRING,p_command,strAboutMenu);
    }
  }
  // Set the icon for this dialog.
  SetIcon(m_hIcon, TRUE);   // Set big icon
  SetIcon(m_hIcon, FALSE);  // Set small icon
}

void
StyleDialog::SetTheme(ThemeColor::Themes p_theme)
{
  // Write new theme to the registry
  RegistryManager manager;
  manager.SetRegistryInteger(STYLECOLORS_KEY,STYLECOLORS_THEME,(int)p_theme);

  // Broadcast the change to all top level windows
  DWORD components = BSM_APPLICATIONS;
  BroadcastSystemMessage(BSF_POSTMESSAGE,&components,g_msg_changed,0,(int)p_theme);

  // Change our colors
  ThemeColor::SetTheme(p_theme);

  // Now repaint ourselves and all of our children
  if(GetSafeHwnd())
  {
    ReDrawFrame();
    ReDrawDialog();
  }
}

// Called only after the derived dialog calls "EnableTooltips()"
// This version knows about the CSFX framework
//
INT_PTR
StyleDialog::OnToolHitTest(CPoint point,TOOLINFO* pTI) const
{
  INT_PTR result = CWnd::OnToolHitTest(point,pTI);
  if(pTI->uFlags & TTF_IDISHWND)
  {
    CWnd* skin = CWnd::FromHandle((HWND)pTI->uId);
    StyleComboBox* combo = dynamic_cast<StyleComboBox*>(skin);
    if(combo)
    {
      CRect comboRect;
      combo->GetWindowRect(&comboRect);
      CRect dialogRect;
      GetWindowRect(&dialogRect);
      comboRect.OffsetRect(-dialogRect.left,- (dialogRect.top + WINDOWCAPTIONHEIGHT(m_hWnd)));
      comboRect.right -= comboRect.Height();
      if(comboRect.PtInRect(point))
      {
        pTI->uId      = (UINT_PTR)combo->GetEditControl()->GetSafeHwnd();
        pTI->lpszText = LPSTR_TEXTCALLBACK;
      }
    }
    else if(skin)
    {
      CString check;
      skin->GetWindowText(check);
      if(check.Compare(_T("CXSF")) == 0)
      {
        CWnd* control = ((SkinScrollWnd*)skin)->GetControl();
        if(control)
        {
          pTI->uId      = (UINT_PTR)control->GetSafeHwnd();
          pTI->lpszText = LPSTR_TEXTCALLBACK;
        }
      }
    }
  }
  return result;
}

void
StyleDialog::RegisterTooltip(int p_ID,LPCTSTR p_text)
{
  CWnd* wnd = GetDlgItem(p_ID);
  if(wnd && wnd->GetSafeHwnd())
  {
    m_tooltips[wnd->GetSafeHwnd()] = p_text;
  }
}

void
StyleDialog::RegisterTooltip(CWnd& p_wnd,LPCTSTR p_text)
{
  if(p_wnd.GetSafeHwnd())
  {
    m_tooltips[p_wnd.GetSafeHwnd()] = p_text;
  }
}

void
StyleDialog::RegisterTooltip(StyleComboBox& p_wnd,LPCTSTR p_text)
{
  if(p_wnd.GetSafeHwnd() && p_wnd.GetEditControl()->GetSafeHwnd())
  {
    m_tooltips[p_wnd.GetSafeHwnd()] = p_text;
    m_tooltips[p_wnd.GetEditControl()->GetSafeHwnd()] = p_text;
  }
}

bool
StyleDialog::AddControlPlane(CWnd* p_dialog)
{
  if(GetExStyle() & WS_EX_CONTROLPARENT)
  {
    m_controlPlanes.push_back(p_dialog);
    return true;
  }
  return false;
}

BOOL
StyleDialog::OnToolTipNotify(UINT id,NMHDR* pNMHDR,LRESULT* pResult)
{
  TOOLTIPTEXT* pTTT = (TOOLTIPTEXT*)pNMHDR;

  if(pTTT->uFlags & TTF_IDISHWND)
  {
    ToolTips::iterator tip = m_tooltips.find((HWND)pNMHDR->idFrom);
    if(tip != m_tooltips.end())
    {
      pTTT->lpszText = (LPTSTR) tip->second;
    }
  }
  // Return value of TTN_NEEDTEXT is not used.
  return TRUE;
}

// Another program has changed our styling
LRESULT
StyleDialog::OnStyleChanged(WPARAM,LPARAM)
{
  LoadStyleTheme();
  return 0; // AOK
}

void
StyleDialog::LoadStyleTheme()
{
  RegistryManager manager;
  int th = manager.GetRegistryInteger(STYLECOLORS_KEY,STYLECOLORS_THEME,(int)ThemeColor::Themes::ThemeSkyblue);
  ThemeColor::Themes theme = (ThemeColor::Themes)th;
  ThemeColor::SetTheme(theme);

  SendMessageToAllChildWindows(g_msg_changed,0,0);

  // Needed for coloring backgrounds of the controls
  m_defaultBrush.DeleteObject();
  m_defaultBrush.CreateSolidBrush(ThemeColor::GetColor(Colors::ColorWindowFrame));

  // Now repaint ourselves and all of our children
  if(GetSafeHwnd())
  {
    Invalidate();
    ReDrawFrame();
    ReDrawDialog();
    RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_FRAME|RDW_ALLCHILDREN);
  }
}

void 
StyleDialog::SendMessageToAllChildWindows(UINT MessageId,WPARAM wParam,LPARAM lParam)
{
  SMessage sMessage;
  sMessage.MessageId = MessageId;
  sMessage.wParam    = wParam;
  sMessage.lParam    = lParam;

  if(GetSafeHwnd())
  {
    ::EnumChildWindows(m_hWnd,
                       [](HWND p_wnd,LPARAM lParam) -> BOOL
                       {
                          PSMessage psMessage = (PSMessage)lParam;
                          ::SendMessage(p_wnd,psMessage->MessageId,psMessage->wParam,psMessage->lParam);
                         return TRUE;
                       },
                       (LPARAM)&sMessage);
  }
}

// After setting of a theme,
// we invalidate all themed controls
void
StyleDialog::ReDrawDialog()
{
  CWnd* child = GetWindow(GW_CHILD);
  while(child)
  {
    SkinScrollWnd* skin = DYNAMIC_DOWNCAST(SkinScrollWnd, child);
    if(skin)
    {
      CWnd* control = skin->m_wndLimit.GetWindow(GW_CHILD);
      if(control)
      {
        control->Invalidate();
      }
      skin->m_sbHorz.Invalidate();
      skin->m_sbVert.Invalidate();
    }
    else
    {
      child->Invalidate();
    }
    child = child->GetNextWindow(GW_HWNDNEXT);
  }
}

void
StyleDialog::OnStyleLime()
{
  SetTheme(ThemeColor::Themes::ThemeLime);
}

void
StyleDialog::OnStyleSkyblue()
{
  SetTheme(ThemeColor::Themes::ThemeSkyblue);
}

void
StyleDialog::OnStylePurple()
{
  SetTheme(ThemeColor::Themes::ThemePurple);
}

void
StyleDialog::OnStyleMustard()
{
  SetTheme(ThemeColor::Themes::ThemeMustard);
}

void
StyleDialog::OnStyleModerateGray()
{
  SetTheme(ThemeColor::Themes::ThemeModerateGray);
}

void
StyleDialog::OnStylePureGray()
{
  SetTheme(ThemeColor::Themes::ThemePureGray);
}

void
StyleDialog::OnStyleBlackWhite()
{
  SetTheme(ThemeColor::Themes::ThemeBlackWhite);
}

void
StyleDialog::OnStyleDark()
{
  SetTheme(ThemeColor::Themes::ThemeDark);
}

BOOL
StyleDialog::OnEraseBkgnd(CDC* pDC)
{
  CRect client;
  GetClientRect(client);
  pDC->FillSolidRect(client,ThemeColor::GetColor(Colors::ColorWindowFrame));
  return TRUE;
}

LPARAM
StyleDialog::OnCtlColorStatic(WPARAM wParam,LPARAM /*lParam*/)
{
  HDC hdc = (HDC)wParam;
  SetTextColor(hdc,ThemeColor::GetColor(Colors::ColorEditText));
  SetBkColor  (hdc,ThemeColor::GetColor(Colors::ColorWindowFrame));
  return (LPARAM)(HBRUSH)m_defaultBrush;
}

LPARAM
StyleDialog::OnCtlColorListBox(WPARAM wParam, LPARAM lParam)
{
  HDC hdc = (HDC)wParam;
  SetTextColor(hdc, ThemeColor::GetColor(Colors::ColorEditText));
  SetBkColor  (hdc, ThemeColor::GetColor(Colors::ColorCtrlBackground));
  return (LPARAM)(HBRUSH)m_defaultBrush;
}

void
StyleDialog::OnActivate(UINT nState,CWnd* pWndOther,BOOL bMinimized)
{
  if(m_canActivate)
  {
    if(nState == WA_ACTIVE || nState == WA_CLICKACTIVE)
    {
      CDialogEx::OnActivate(nState,pWndOther,bMinimized);
      ReDrawFrame();
    }
  }
}

void
StyleDialog::OnActivateApp(BOOL bActive,DWORD dwThreadID)
{
  CDialogEx::OnActivateApp(bActive,dwThreadID);
  if(bActive)
  {
    ReDrawFrame();
  }
}

void 
StyleDialog::OnNcMouseMove(UINT nFlags, CPoint point)
{
  if ((GetStyle() & WS_POPUP) == 0)
  {
    CDialogEx::OnNcMouseMove(nFlags, point);
  }
  else
  {
    CDialogEx::OnNcMouseMove(nFlags,point);

    if(m_curhit != (LRESULT)nFlags)
    {
      UINT prev = (UINT)m_curhit;
      m_curhit = nFlags;
      ReDrawButton(prev);
      ReDrawButton(m_curhit);
    }

    if(!m_trackMouse && OnNcHitTest(point) != HTNOWHERE)
    {
      TRACKMOUSEEVENT mouseEvent;
      mouseEvent.cbSize      = sizeof(TRACKMOUSEEVENT);
      mouseEvent.dwFlags     = TME_NONCLIENT | TME_LEAVE;
      mouseEvent.hwndTrack   = m_hWnd;
      mouseEvent.dwHoverTime = HOVER_DEFAULT;
      _TrackMouseEvent(&mouseEvent);
      m_trackMouse = true;
    }
  }
}

void
StyleDialog::OnNcMouseLeave()
{
  if(m_curhit != HTNOWHERE)
  {
    UINT prev = (UINT)m_curhit;
    m_curhit = HTNOWHERE;
    m_down = false;
    ReDrawButton(prev);
  }
  if(m_trackMouse)
  {
    TRACKMOUSEEVENT mouseEvent;
    mouseEvent.cbSize      = sizeof(TRACKMOUSEEVENT);
    mouseEvent.dwFlags     = TME_NONCLIENT | TME_LEAVE | TME_CANCEL;
    mouseEvent.hwndTrack   = m_hWnd;
    mouseEvent.dwHoverTime = 0;
    _TrackMouseEvent(&mouseEvent);
    m_trackMouse = false;
  }
}

void
StyleDialog::OnNcRButtonUp(UINT nFlags,CPoint point)
{
  if(!m_mnuButton)
  {
    CDialogEx::OnNcRButtonUp(nFlags,point);
    return;
  }
  // Implement our system menu
  CMenu* menu = GetSystemMenu(FALSE);
  int flags = TPM_LEFTALIGN | TPM_RETURNCMD;
  int x = point.x;
  int y = point.y;
  int num = TrackPopupMenu(menu->GetSafeHmenu(),flags,x,y,0,GetSafeHwnd(),nullptr);
  if(num > 0)
  {
    PostMessage(WM_SYSCOMMAND,num,0);
  }
}

void
StyleDialog::PerformMenu()
{
  if(m_menu.GetSafeHmenu())
  {
    CRect rect;
    GetClientRect(rect);
    ClientToScreen(rect);

    CMenu* popup = m_menu.GetSubMenu(0);

    int flags = TPM_LEFTALIGN | TPM_RETURNCMD;
    int num = TrackPopupMenu(popup->GetSafeHmenu(),flags,rect.left,rect.top,0,GetSafeHwnd(),nullptr);
    if(num > 0)
    {
      PostMessage(WM_COMMAND,num,0);
    }
  }
}

void
StyleDialog::OnNcLButtonDown(UINT nFlags, CPoint point)
{
  if((GetStyle() & WS_POPUP) == 0)
  {
    CDialogEx::OnNcLButtonDown(nFlags, point);
    return;
  }
  int oldDpi = m_dpi_x;

  // Remove gripper. We could resize the dialog
  EraseGripper();

  switch (nFlags) 
  {
    case HTCLOSE:
    case HTMENU:
    case HTMINBUTTON:
    case HTMAXBUTTON:
    {
      m_curhit = nFlags;
      HRESULT lasthit = HTNOWHERE;
      CPoint lastpoint(0, 0);
      m_down = false;

      while (LBUTTONDOWN)
      {
        CPoint cursor;
        GetCursorPos(&cursor);
        if (cursor != lastpoint)
        {
          lastpoint = cursor;
          HRESULT hit = (HRESULT) OnNcHitTest(cursor);
          if (hit != lasthit)
          {
            lasthit = hit;
            m_down = (lasthit == m_curhit);

            // Redraw the button in the right state
            LRESULT curr = m_curhit;
            m_curhit = hit;
            ReDrawButton(nFlags);
            m_curhit = curr;
          }
        }
      }

      m_down = false;
      m_curhit = OnNcHitTest(lastpoint);
      ReDrawButton(nFlags);

      if(m_curhit == (LRESULT)nFlags)
      {
        switch(m_curhit) 
        {
          case HTMENU:      PerformMenu();
                            break;
          case HTCLOSE:     PostMessage(WM_SYSCOMMAND,SC_CLOSE,0);
                            break;
          case HTMINBUTTON: ShowWindow(SW_MINIMIZE);
                            break;
          case HTMAXBUTTON: if((GetStyle() & WS_MAXIMIZE) != 0)
                            {
                              ShowWindow(SW_SHOWNORMAL);
                            }
                            else
                            {
                              ShowWindow(SW_MAXIMIZE);
                            }
                            RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_FRAME);
                            break;
        }
        ReDrawFrame();
      }
    }
    break;

    case HTCAPTION:
    {
      if(GetStyle() & WS_MAXIMIZE)
      {
        goto ready;
      }

      SetWindowPos(&CWnd::wndTop,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW|SWP_NOSENDCHANGING|SWP_DRAWFRAME);

      CPoint lastpoint(point);
      CPoint moved(point);
      CRect window;
      GetWindowRect(window);
      moved.x -= window.left;
      moved.y -= window.top;

      while (LBUTTONDOWN)
      {
        CPoint cursor;
        GetCursorPos(&cursor);
        if (cursor != lastpoint)
        {
          SetWindowPos(NULL, cursor.x - moved.x, cursor.y - moved.y, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);
          lastpoint = cursor;
        }
      }
      goto ready;
    }
    break;

    case HTLEFT:
    case HTRIGHT:
    case HTTOP:
    case HTTOPLEFT:
    case HTTOPRIGHT:
    case HTBOTTOM:
    case HTBOTTOMLEFT:
    case HTBOTTOMRIGHT:
    {
      if(!m_canResize || (GetStyle() & WS_MAXIMIZE))
      {
        goto ready;
      }
      CPoint lastpoint(point);
      CRect window;
      GetWindowRect(window);

      while (LBUTTONDOWN)
      {
        CPoint cursor;
        GetCursorPos(&cursor);
        if (cursor != lastpoint)
        {
          switch (nFlags) 
          {
            case HTLEFT:        window.left   += cursor.x - lastpoint.x;
                                break;
            case HTRIGHT:       window.right  += cursor.x - lastpoint.x;
                                break;
            case HTTOP:         window.top    += cursor.y - lastpoint.y;
                                break;
            case HTTOPLEFT:     window.top    += cursor.y - lastpoint.y;
                                window.left   += cursor.x - lastpoint.x;
                                break;
            case HTTOPRIGHT:    window.top    += cursor.y - lastpoint.y;
                                window.right  += cursor.x - lastpoint.x;
                                break;
            case HTBOTTOM:      window.bottom += cursor.y - lastpoint.y;
                                break;
            case HTBOTTOMLEFT:  window.bottom += cursor.y - lastpoint.y;
                                window.left   += cursor.x - lastpoint.x;
                                break;
            case HTBOTTOMRIGHT: window.bottom += cursor.y - lastpoint.y;
                                window.right  += cursor.x - lastpoint.x;
                                break;
          }
          if(window.Width()  >= m_originalSize.Width() &&
             window.Height() >= m_originalSize.Height())
          {
            SetWindowPos(nullptr, window.left, window.top, window.Width(), window.Height(), SWP_NOZORDER | SWP_DRAWFRAME);
          }
          lastpoint = cursor;
        }
      }
      break;
    }
  }
ready:
  // In case we changed DPI while moving/resizing redraw the frame
  if(m_dpi_x != oldDpi)
  {
    OnNcPaint();
  }
}

void
StyleDialog::OnNcLButtonDblClk(UINT nFlags, CPoint point)
{
  if(nFlags == HTCAPTION)
  {
    if(m_maxButton)
    {
      ShowWindow((GetStyle() & WS_MAXIMIZE) ? SW_SHOWNORMAL : SW_MAXIMIZE);
    }
    ReDrawFrame();
  }
  else
  {
    CDialogEx::OnNcLButtonDblClk(nFlags,point);
  }
}

bool
StyleDialog::OnClosing()
{
  return true;
}

void
StyleDialog::OnSysCommand(UINT nID, LPARAM lParam)
{
  if((nID & 0xFFF0) == SC_CLOSE)
  {
    if(!OnClosing())
    {
      return;
    }
  }
  CWnd::OnSysCommand(nID, lParam);
  if((nID & 0xFFF0) == SC_RESTORE)
  {
    RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_FRAME);
  }
}

HBRUSH 
StyleDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
  switch (nCtlColor)
  {
    case CTLCOLOR_LISTBOX:// Fall through
    case CTLCOLOR_EDIT:   pDC->SetTextColor(ThemeColor::GetColor(Colors::ColorEditText));
                          pDC->SetBkColor  (ThemeColor::GetColor(Colors::ColorCtrlBackground));
                          break;
    case CTLCOLOR_STATIC: if(dynamic_cast<StyleHyperLink*>(pWnd))
                          {
                            return (dynamic_cast<StyleHyperLink*>(pWnd))->CtlColor(pDC,nCtlColor);
                          }
                          if(m_error)
                          {
                            pDC->SetTextColor(ColorWindowMessageTextError);
                            pDC->SetBkColor(ThemeColor::GetColor(Colors::ColorWindowFrame));
                          }
                          else
                          {
                            pDC->SetTextColor(ThemeColor::GetColor(Colors::ColorLabelText));
                            pDC->SetBkColor  (ThemeColor::GetColor(Colors::ColorWindowFrame));
                          }
                          break;
    case CTLCOLOR_BTN:    if(m_error)
                          {
                            pDC->SetTextColor(ColorWindowMessageTextError);
                            pDC->SetBkColor(ThemeColor::GetColor(Colors::ColorWindowFrame));
                          }
                          else
                          {
                            pDC->SetTextColor(ThemeColor::GetColor(Colors::ColorLabelText));
                            pDC->SetBkColor  (ThemeColor::GetColor(Colors::ColorWindowFrame));
                          }
                          break;
  }
  return CDialogEx::OnCtlColor(pDC,pWnd,nCtlColor);
}

void 
StyleDialog::SetWindowText(LPCTSTR lpstString)
{
  CDialogEx::SetWindowText(lpstString);
  ReDrawFrame();
}

void
StyleDialog::OnSettingChange(UINT uFlags,LPCTSTR lpszSection)
{
  if((uFlags == SPI_SETWORKAREA) && ((GetStyle() & WS_MAXIMIZE) == WS_MAXIMIZE))
  {
    ShowWindow(SW_RESTORE);
    ShowWindow(SW_MAXIMIZE);
  }
}

// Get the DPI scaled size for all child windows
// Called before resizing the dialog itself and the DPI change
LRESULT
StyleDialog::OnGetDpiScaledSize(WPARAM wParam,LPARAM lParam)
{
  SendMessageToAllChildWindows(WM_GETDPISCALEDSIZE,wParam,lParam);
  // Do the default resizing
  return 0;
}

static int   g_dpi_x = USER_DEFAULT_SCREEN_DPI;
static int   g_dpi_y = USER_DEFAULT_SCREEN_DPI;
static CWnd* g_resize_wnd(nullptr);

// A change in DPI has happened
LRESULT
StyleDialog::OnDpiChanged(WPARAM wParam,LPARAM lParam)
{
  g_resize_wnd = this;

  // Save the old DPI
  g_dpi_x = m_dpi_x;
  g_dpi_y = m_dpi_y;
  // The new DPI
  m_dpi_x = HIWORD(wParam);
  m_dpi_y = LOWORD(wParam);

  // Check if anything has changed
  if(m_dpi_x == g_dpi_x && m_dpi_y == g_dpi_y)
  {
    // No change
    return 0;
  }

  // Remove the invalid layout manager
  auto manager = GetDynamicLayout();
  bool layoutEnabled(manager != nullptr);
  if(manager)
  {
    m_pDynamicLayout = nullptr;
    delete manager;
  }

  // Current monitor configuration
  g_styling.RefreshMonitors();

  // Set the new window size/position as suggested by the system
  CRect wrect;
  GetWindowRect(wrect);

  wrect.right  = wrect.left + ::MulDiv(wrect.Width(), m_dpi_x,g_dpi_x);
  wrect.bottom = wrect.top  + ::MulDiv(wrect.Height(),m_dpi_x,g_dpi_y);

  ::SetWindowPos(m_hWnd,
                 nullptr,
                 wrect.left,
                 wrect.top,
                 wrect.Width(),
                 wrect.Height(),
                 SWP_NOZORDER | SWP_NOACTIVATE);

  // Notify all child windows that we are about to change DPI
  NotifyMonitorToAllChilds(true);

  // Scale child windows using newDpi and oldDpi (the previous DPI)
  ::EnumChildWindows( m_hWnd,
                      [](HWND hWnd,LPARAM lParam) -> BOOL
                      {
                        const int dpi_x = HIWORD(lParam);
                        const int dpi_y = LOWORD(lParam);

                        // Getting child and parent
                        // Do not handle CXSF and limiter windows.
                        CWnd* child  = CWnd::FromHandle(hWnd);
                        CWnd* parent = child->GetParent();
                        if(parent != g_resize_wnd)
                        {
                          return TRUE;
                        }
                        // Getting the child window rectangle
                        CRect rcChild;
                        child->GetWindowRect(rcChild);

                        // Getting the parent window to calc displacement
                        CRect rcParent;
                        parent->GetWindowRect(rcParent);

                        // Creating the target rectangle
                        CRect rcTarget;
                        rcTarget.left   = rcChild.left   - rcParent.left;
                        rcTarget.top    = rcChild.top    - rcParent.top;
                        rcTarget.right  = rcChild.right  - rcParent.left;
                        rcTarget.bottom = rcChild.bottom - rcParent.top;

                        // Correct for StyleDialog
                        int caption = WINDOWCAPTIONHEIGHT(g_resize_wnd->GetSafeHwnd());
                        int sborder = WINDOWSHADOWBORDER (g_resize_wnd->GetSafeHwnd()) * 2;
                        rcTarget.top    -= caption;
                        rcTarget.bottom -= caption;
                        rcTarget.left   -= sborder;
                        rcTarget.right  -= sborder;

                        // Now scale the target rectangle
                        rcTarget.left   = ::MulDiv(rcTarget.left,  dpi_x,g_dpi_x);
                        rcTarget.top    = ::MulDiv(rcTarget.top,   dpi_y,g_dpi_y);
                        rcTarget.right  = ::MulDiv(rcTarget.right, dpi_x,g_dpi_x);
                        rcTarget.bottom = ::MulDiv(rcTarget.bottom,dpi_y,g_dpi_y);

                        child->MoveWindow(rcTarget);
                        return TRUE;
                      },
                      (LPARAM)wParam);

  // Notify all child windows after parent has changed DPI
  // So that we can handle different fonts etc.
  NotifyMonitorToAllChilds();

  // Move and resize the status bar (if any)
  if(::IsWindow(m_statusBar.GetSafeHwnd()))
  {
    RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0);
  }

  // Propagate the DPI change to all child windows
  SendMessageToAllChildWindows(WM_DPICHANGED,wParam,lParam);

  // Now redraw everything
  Invalidate(TRUE);
  OnNcPaint();

  // Create a new dynamic layout manager for the new DPI
  if(layoutEnabled)
  {
    // Scale the original size
    m_originalSize.right  = m_originalSize.left + ::MulDiv(m_originalSize.Width(), m_dpi_x,g_dpi_x);
    m_originalSize.bottom = m_originalSize.top  + ::MulDiv(m_originalSize.Height(),m_dpi_y,g_dpi_y);

    SetCanResize(true);
  }
  return 0;
}

// A change of monitors could have happened
// or a change in the display rate (width/height) could have been performed
LRESULT
StyleDialog::OnDisplayChange(WPARAM wParam,LPARAM lParam)
{
  // Current monitor configuration
  g_styling.RefreshMonitors();

  // Mainly to set new font sizes
  NotifyMonitorToAllChilds();

  // Move to original monitor if needed
  return 0;
}

// After a DPI change or a change in monitors or monitor settings
void
StyleDialog::NotifyMonitorToAllChilds(bool p_beforeParent /*= false*/)
{
  // Take the HMONITOR for the new monitor
  const  StyleMonitor* mon = g_styling.GetMonitor(GetSafeHwnd());
  if(!mon)
  {
    return;
  }
  HMONITOR newMonitor = mon->GetMonitor();

  if(p_beforeParent)
  {
    // Notify all child windows before parent has changed DPI
    // So that we can prepare for the change
    SendMessageToAllChildWindows(WM_DPICHANGED_BEFOREPARENT,m_dpi_x,(LPARAM)newMonitor);
  }
  else
  {
    // Notify all child windows after parent has changed DPI
    // So that we can handle different fonts etc.
    CFont* font = GetSFXFont(newMonitor,StyleFontType::DialogFont);
    if(font)
    {
      SendMessageToAllChildWindows(WM_SETFONT,(WPARAM)font->GetSafeHandle(),(LPARAM)TRUE);
    }
    // Let Style controls 'do-their-thing'
    SendMessageToAllChildWindows(WM_DPICHANGED_AFTERPARENT,m_dpi_x,(LPARAM)newMonitor);
  }
}

void 
StyleDialog::OnSize(UINT nType, int cx, int cy)
{
  if ((GetStyle() & WS_POPUP) == 0)
  {
    CDialogEx::OnSize(nType, cx, cy);
  }
  else
  {
    CDialogEx::OnSize(nType,cx,cy);

    if(nType == SIZE_MAXIMIZED && ((GetStyle() & WS_MAXIMIZE) != 0) && !m_dtsize)
    {
      m_dtsize = true;
      RepositionFrameWnd(this,false);
    }
    else
    {
      m_dtsize = false;
    }

    // Reposition buttons and statusbar (if any)
    PositionButtons();
    if(::IsWindow(m_statusBar.GetSafeHwnd()))
    {
      RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0);
    }
    OnNcPaint();
  }

  if(m_canResize)
  {
    CMFCDynamicLayout* manager = GetDynamicLayout();
    if (manager)
    {
      manager->Adjust();
    }
  }
}

void
StyleDialog::PositionButtons()
{
  CRect window;
  GetWindowRect(window);
  window.OffsetRect(-window.left, -window.top);

  int border  = WINDOWSHADOWBORDER(m_hWnd);
  int caption = WINDOWCAPTIONHEIGHT(m_hWnd);

  if(m_caption)
  {
    m_captionRect.SetRect(0, 0, window.Width() - border, caption);

    int index = 1;
    if(m_closeButton)
    {
      m_closeRect.SetRect(window.Width() - border - index * caption, 0, window.Width() - border, caption);
      ++index;
    }
    if(m_maxButton)
    {
      m_maxRect.SetRect(window.Width() - border - index * caption, 0, window.Width() - border - (index - 1) * caption, caption);
      ++index;
    }
    if(m_minButton)
    {
      m_minRect.SetRect(window.Width() - border - index * caption, 0, window.Width() - border - (index - 1) * caption, caption);
      ++index;
    }
    if(m_mnuButton)
    {
      m_sysRect.SetRect(0,0,caption,caption);
    }
  }
  else
  {
    m_closeRect  .SetRect(0, 0, 0, 0);
    m_captionRect.SetRect(0, 0, 0, 0);
    m_maxRect    .SetRect(0, 0, 0, 0);
    m_minRect    .SetRect(0, 0, 0, 0);
    m_sysRect    .SetRect(0, 0, 0, 0);
  }
}

void 
StyleDialog::OnNcCalcSize(BOOL calcValidRects, NCCALCSIZE_PARAMS *params)
{
  if ((GetStyle() & WS_POPUP) == 0)
  {
    CDialogEx::OnNcCalcSize(calcValidRects, params);
  }
  else
  {
    CDialogEx::OnNcCalcSize(calcValidRects, params);

    if (m_caption)
    {
      int border  = WINDOWSHADOWBORDER(m_hWnd);
      int caption = WINDOWCAPTIONHEIGHT(m_hWnd);
      int margin  = SIZEMARGIN(m_hWnd);
      // Derived from CDialogEx
      params->rgrc[0].left   += margin - 1;
      params->rgrc[0].top    += caption;
      params->rgrc[0].bottom -= margin - 1 + border;
      params->rgrc[0].right  -= margin - 1 + border;
    }
  }
}

void 
StyleDialog::ReDrawFrame()
{
  OnNcPaint();
}

void 
StyleDialog::OnNcPaint()
{
  if ((GetStyle() & WS_POPUP) == 0)
  {
    CDialogEx::OnNcPaint();
  }
  else
  {
    CWindowDC dc(this);
    CRect window;
    GetWindowRect(window);
    window.OffsetRect(-window.left, -window.top);

    if(m_caption)
    {
      int border  = WINDOWSHADOWBORDER(m_hWnd);
      int caption = WINDOWCAPTIONHEIGHT(m_hWnd);
      int margin  = SIZEMARGIN(m_hWnd);

      // Shadow frame    
      dc.FillSolidRect(CRect(window.right - border,   window.top,             window.right,            window.top + 2 * border), ColorWindowTransparent);
      dc.FillSolidRect(CRect(window.right - border,   window.top + 2 * border,window.right,            window.bottom), ColorWindowGrayFrame);
      dc.FillSolidRect(CRect(window.left,             window.bottom -  border,window.left + 2 * border,window.bottom), ColorWindowTransparent);
      dc.FillSolidRect(CRect(window.left + 2 * border,window.bottom -  border,window.right    - border,window.bottom), ColorWindowGrayFrame);

      // caption bar
      CRect captionrc(m_captionRect);
      dc.FillSolidRect(captionrc, m_error ? ColorWindowFrameError : ThemeColor::GetColor(Colors::AccentColor1));

      // title
      dc.SetTextColor(m_error ? ColorWindowFrameTextError : ColorWindowHeaderText);
      captionrc.right -= caption;
      captionrc.right -= m_maxButton ? caption : 0;
      captionrc.right -= m_minButton ? caption : 0;
      captionrc.left  += m_mnuButton ? caption : 0;
      captionrc.DeflateRect(5, 0);
      CString titel;
      GetWindowText(titel);

      CFont* font = GetSFXFont(GetSafeHwnd(),StyleFontType::DialogFontBold);
      dc.SelectObject(font);
      dc.DrawText(titel, captionrc, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

      // System menu button
      if (m_mnuButton)
      {
        DrawButton(&dc, m_sysRect, HTMENU);
      }
      // minimize button
      if(m_minButton)
      {
        DrawButton(&dc, m_minRect, HTMINBUTTON);
      }
      // maximize button
      if(m_maxButton)
      {
        DrawButton(&dc, m_maxRect, HTMAXBUTTON);
      }
      // close button
      if(m_closeButton)
      {
        DrawButton(&dc, m_closeRect, HTCLOSE);
      }
      // window frame
      CPen pen;
      pen.CreatePen(PS_SOLID, 1, m_error ? ColorWindowFrameError : ColorWindowGrayFrame);
      HGDIOBJ orgpen = dc.SelectObject(pen);
      dc.MoveTo(window.right- border-1, window.top    + caption);
      dc.LineTo(window.right- border-1, window.bottom - border - 1);
      dc.LineTo(window.left, window.bottom - border  - 1);
      dc.LineTo(window.left, window.top    + caption - 1);
      dc.SelectObject(orgpen);

      CRect r;
      int back = ThemeColor::GetColor(Colors::ColorWindowFrame);

      r.SetRect(window.left + 1, window.top + caption, window.left + margin, window.bottom - border -1);
      dc.FillSolidRect(r, back);
      r.SetRect(window.left + margin, window.bottom - border - margin, window.right - border - 1, window.bottom - border - 1);
      dc.FillSolidRect(r, back);
      r.SetRect(window.right - border - margin, window.top + caption, window.right - border - 1, window.bottom - border - margin);
      dc.FillSolidRect(r, back);
    }
    else
    {
      // window frame
      CBrush brush(m_error ? ColorWindowFrameError : ColorWindowGrayFrame);
      dc.FrameRect(window, &brush);
    }
  }
}

LRESULT 
StyleDialog::OnNcHitTest(CPoint point)
{
  if((GetStyle() & WS_POPUP) == 0)
  {
    return CDialogEx::OnNcHitTest(point);
  }

  CRect window;
  GetWindowRect(window);
  point.Offset(-window.TopLeft());

  if(m_closeButton && m_closeRect.PtInRect(point))
  {
    return HTCLOSE;
  }
  if(m_minButton && m_minRect.PtInRect(point))
  {
    return HTMINBUTTON;
  }
  if(m_maxButton && m_maxRect.PtInRect(point))
  {
    return HTMAXBUTTON;
  }
  if(m_mnuButton && m_sysRect.PtInRect(point))
  {
    return HTMENU;
  }

  if(!(GetStyle() & WS_MAXIMIZE))
  {
    int margin = SIZEMARGIN(m_hWnd);

    window.OffsetRect(-window.left, -window.top);
    if(point.x <= window.left + margin)
    {
      if(point.y >= window.bottom - margin)
      {
        return HTBOTTOMLEFT;
      }
      if(point.y <= window.top + margin)
      {
        return HTTOPLEFT;
      }
      return HTLEFT;
    }
    if(point.x >= window.right - margin)
    {
      if(point.y >= window.bottom - margin)
      {
        return HTBOTTOMRIGHT;
      }
      if(point.y <= window.top + margin)
      {
        return HTTOPRIGHT;
      }
      return HTRIGHT;
    }
    if(point.y >= window.bottom - margin)
    {
      return HTBOTTOM;
    }
    if(point.y <= window.top + margin)
    {
      return HTTOP;
    }
  }

  if (m_caption && (m_captionRect.PtInRect(point)))
  {
    return HTCAPTION;
  }
  return HTNOWHERE;
}

void 
StyleDialog::ReDrawButton(LRESULT type)
{
  CWindowDC dc(this);
  switch (type) 
  {
    case HTMENU:      DrawButton(&dc,m_sysRect,type);
                      break;
    case HTCLOSE:     DrawButton(&dc, m_closeRect, type);
                      break;
    case HTMINBUTTON: DrawButton(&dc, m_minRect, type);
                      break;
    case HTMAXBUTTON: DrawButton(&dc, m_maxRect, type);
                      break;       
  }
}

void 
StyleDialog::DrawButton(CDC* pDC, CRect rect, LRESULT type)
{
  pDC->FillSolidRect(rect,ThemeColor::GetColor(Colors::AccentColor1));
  if (m_curhit == type)
  {
    pDC->FillSolidRect(rect, m_down ? ThemeColor::GetColor(Colors::ColorControlPressed) 
                                    : ThemeColor::GetColor(Colors::ColorControlHover));
  }
  CPen pen;
  pen.CreatePen(PS_SOLID, 1, m_curhit == type ? m_down ? ThemeColor::GetColor(Colors::ColorControlTextPressed) 
                                                       : ThemeColor::GetColor(Colors::ColorControlTextHover)
                                                       : ColorWindowHeaderIcon);
  HGDIOBJ orgpen = pDC->SelectObject(pen);
  int hpos3 = WS(GetSafeHwnd(),3);
  int hpos4 = WS(GetSafeHwnd(),4);
  int hpos5 = WS(GetSafeHwnd(),5);
  int hpos6 = WS(GetSafeHwnd(),6);
  int hpos7 = WS(GetSafeHwnd(),7);

  switch (type) 
  {
    case HTMENU:      pDC->MoveTo(rect.CenterPoint().x - hpos7,rect.CenterPoint().y - hpos5);
                      pDC->LineTo(rect.CenterPoint().x + hpos7,rect.CenterPoint().y - hpos5);
                      pDC->MoveTo(rect.CenterPoint().x - hpos7,rect.CenterPoint().y);
                      pDC->LineTo(rect.CenterPoint().x + hpos7,rect.CenterPoint().y);
                      pDC->MoveTo(rect.CenterPoint().x - hpos7,rect.CenterPoint().y + hpos5);
                      pDC->LineTo(rect.CenterPoint().x + hpos7,rect.CenterPoint().y + hpos5);
                      break;
    case HTCLOSE:     pDC->MoveTo(rect.CenterPoint().x - hpos5, rect.CenterPoint().y - hpos5);
                      pDC->LineTo(rect.CenterPoint().x + hpos6, rect.CenterPoint().y + hpos6);
                      pDC->MoveTo(rect.CenterPoint().x + hpos5, rect.CenterPoint().y - hpos5);
                      pDC->LineTo(rect.CenterPoint().x - hpos6, rect.CenterPoint().y + hpos6);
                      break;
    
    case HTMINBUTTON: pDC->MoveTo(rect.CenterPoint().x - hpos6, rect.CenterPoint().y + hpos6);
                      pDC->LineTo(rect.CenterPoint().x + hpos6, rect.CenterPoint().y + hpos6);
                      break;

    case HTMAXBUTTON: if ((GetStyle() & WS_MAXIMIZE) != 0)
                      {
                        // 'front' window
                        pDC->MoveTo(rect.CenterPoint().x - hpos5, rect.CenterPoint().y - hpos3);
                        pDC->LineTo(rect.CenterPoint().x + hpos4, rect.CenterPoint().y - hpos3);
                        pDC->LineTo(rect.CenterPoint().x + hpos4, rect.CenterPoint().y + hpos7);
                        pDC->LineTo(rect.CenterPoint().x - hpos5, rect.CenterPoint().y + hpos7);
                        pDC->LineTo(rect.CenterPoint().x - hpos5, rect.CenterPoint().y - hpos3);

                        // 'behind' window
                        pDC->MoveTo(rect.CenterPoint().x - hpos3, rect.CenterPoint().y - hpos4);
                        pDC->LineTo(rect.CenterPoint().x - hpos3, rect.CenterPoint().y - hpos5);
                        pDC->LineTo(rect.CenterPoint().x + hpos7, rect.CenterPoint().y - hpos5);
                        pDC->LineTo(rect.CenterPoint().x + hpos7, rect.CenterPoint().y + hpos4);
                        pDC->LineTo(rect.CenterPoint().x + hpos4, rect.CenterPoint().y + hpos4);
                      }
                      else
                      {
                        pDC->MoveTo(rect.CenterPoint().x - hpos5, rect.CenterPoint().y - hpos5);
                        pDC->LineTo(rect.CenterPoint().x + hpos6, rect.CenterPoint().y - hpos5);
                        pDC->LineTo(rect.CenterPoint().x + hpos6, rect.CenterPoint().y + hpos6);
                        pDC->LineTo(rect.CenterPoint().x - hpos5, rect.CenterPoint().y + hpos6);
                        pDC->LineTo(rect.CenterPoint().x - hpos5, rect.CenterPoint().y - hpos5);
                      }
                      break;
  }
  pDC->SelectObject(orgpen);
}

void
StyleDialog::OnPaint()
{
  if(IsIconic())
  {
    CPaintDC dc(this); // device context for painting

    SendMessage(WM_ICONERASEBKGND,reinterpret_cast<WPARAM>(dc.GetSafeHdc()),0);

    // Center icon in client rectangle
    int dpi = ::GetDpiForWindow(GetSafeHwnd());
    int cxIcon = GetSystemMetricsForDpi(SM_CXICON,dpi);
    int cyIcon = GetSystemMetricsForDpi(SM_CYICON,dpi);
    CRect rect;
    GetClientRect(&rect);
    int x = (rect.Width()  - cxIcon + 1) / 2;
    int y = (rect.Height() - cyIcon + 1) / 2;

    // Draw the icon
    dc.DrawIcon(x,y,m_hIcon);
    return;
  }

  CDialogEx::OnPaint();

  if(m_canResize && (m_hasGripper || (GetParent() == nullptr) && (GetStyle() & WS_MAXIMIZE) == 0))
  {
    CDC* pDC = GetDC();
    CRect rect;
    GetClientRect(rect);
    int dpi = ::GetDpiForWindow(GetSafeHwnd());
    rect.left = rect.right  - ::GetSystemMetricsForDpi(SM_CXHSCROLL,dpi);
    rect.top  = rect.bottom - ::GetSystemMetricsForDpi(SM_CYVSCROLL,dpi);
    pDC->DrawFrameControl(rect,DFC_SCROLL,DFCS_SCROLLSIZEGRIP);
    ReleaseDC(pDC);
  }
}

// Remove gripper before resizing the dialog
void
StyleDialog::EraseGripper()
{
  if(m_canResize && (m_hasGripper || (GetParent() == nullptr) && (GetStyle() & WS_MAXIMIZE) == 0))
  {
    CDC* pDC = GetDC();
    CRect rect;
    GetClientRect(rect);
    int dpi = ::GetDpiForWindow(GetSafeHwnd());
    rect.left = rect.right - ::GetSystemMetricsForDpi(SM_CXHSCROLL,dpi);
    rect.top = rect.bottom - ::GetSystemMetricsForDpi(SM_CYVSCROLL,dpi);
    pDC->FillRect(rect,&m_defaultBrush);
    ReleaseDC(pDC);
  }
}

// The system calls this function to obtain the cursor to display while the user drags the minimized window.
// Normally generated by the MFC Wizard
HCURSOR 
StyleDialog::OnQueryDragIcon()
{
  return static_cast<HCURSOR>(m_hIcon);
}

void
StyleDialog::OnDroppedFile(UINT p_id,UINT p_index,LPCTSTR p_fileName)
{
  StyleMessageBox(this,_T("ERROR: Implement your own OnDroppedFile for this Dialog"),_T("Error"),MB_OK | MB_ICONERROR);
}

// Try overridden OnClosing before canceling the dialog
void
StyleDialog::OnCancel()
{
  if(!OnClosing())
  {
    return;
  }
  CDialogEx::OnCancel();
}

// Try overridden OnClosing before closing the dialog.
void
StyleDialog::OnOK()
{
  if(!OnClosing())
  {
    return;
  }
  CDialogEx::OnOK();
}

