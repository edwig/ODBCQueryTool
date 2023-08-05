////////////////////////////////////////////////////////////////////////
//
// File: StyleTab.cpp
// Function: Styling frame for a tab in a StyleTabCtrl
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
#include "StyleTab.h"
#include "RegistryManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace ThemeColor;

IMPLEMENT_DYNAMIC(StyleTab,CDialog);

BEGIN_MESSAGE_MAP(StyleTab,CDialog)
  ON_WM_ERASEBKGND()
  ON_WM_CTLCOLOR()
  ON_REGISTERED_MESSAGE(g_msg_changed,OnStyleChanged)
  ON_NOTIFY_EX(TTN_NEEDTEXT,0,  OnToolTipNotify)
  ON_MESSAGE(WM_CTLCOLORSTATIC, OnCtlColorStatic)
  ON_MESSAGE(WM_CTLCOLORLISTBOX,OnCtlColorListBox)
END_MESSAGE_MAP()

StyleTab::StyleTab(UINT  p_IDTemplate,CWnd* p_parentWnd)
         :CDialog(p_IDTemplate,p_parentWnd)
{
  m_defaultBrush.DeleteObject();
  m_defaultBrush.CreateSolidBrush(ThemeColor::GetColor(Colors::ColorWindowFrame));
}

StyleTab::~StyleTab()
{
}

BOOL
StyleTab::OnInitDialog()
{
  CDialog::OnInitDialog();
  OnStyleChanged(0,0);
  return TRUE;
}

void
StyleTab::SetCanResize(bool p_resize)
{
  m_canResize = p_resize;

  if(p_resize && IsDynamicLayoutEnabled())
  {
    // Do not bother: it's already 'ON'
    return;
  }
  GetWindowRect(m_originalSize);

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
StyleTab::SetupDynamicLayout()
{
  auto manager = GetDynamicLayout();
  if(manager == nullptr)
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

// Another program has changed our styling
LRESULT
StyleTab::OnStyleChanged(WPARAM,LPARAM)
{
  RegistryManager manager;
  int th = manager.GetRegistryInteger(STYLECOLORS_KEY,STYLECOLORS_THEME,(int) ThemeColor::Themes::ThemeSkyblue);
  ThemeColor::Themes theme = (ThemeColor::Themes) th;
  ThemeColor::SetTheme(theme);

  // Needed for coloring backgrounds of the controls
  m_defaultBrush.DeleteObject();
  m_defaultBrush.CreateSolidBrush(ThemeColor::GetColor(Colors::ColorWindowFrame));

  // Now repaint ourselves and all of our children
  if(GetSafeHwnd())
  {
    Invalidate();
    RedrawWindow(NULL,NULL,RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
  }
  return 0; // AOK
}

// Catch the ESC key, so the tab will always stay visible
BOOL 
StyleTab::PreTranslateMessage(MSG* p_msg)
{
  if(p_msg->message == WM_KEYDOWN)
  {
    if(p_msg->wParam == VK_ESCAPE)
    {
      return TRUE;
    }
  }
  // Message not (yet) handled
  return FALSE;
}

// Called only after the derived dialog calls "EnableTooltips()"
// This version knows about the CSFX framework
//
INT_PTR
StyleTab::OnToolHitTest(CPoint point,TOOLINFO* pTI) const
{
  INT_PTR result = CWnd::OnToolHitTest(point,pTI);
  if(pTI->uFlags & TTF_IDISHWND)
  {
    CWnd* skin = CWnd::FromHandle((HWND) pTI->uId);
    StyleComboBox* combo = dynamic_cast<StyleComboBox*>(skin);
    if(combo)
    {
      CRect comboRect;
      combo->GetWindowRect(&comboRect);
      CRect dialogRect;
      GetWindowRect(&dialogRect);
      comboRect.OffsetRect(-dialogRect.left,-(dialogRect.top + WINDOWCAPTIONHEIGHT));
      comboRect.right -= comboRect.Height();
      if(comboRect.PtInRect(point))
      {
        pTI->uId = (UINT_PTR) combo->GetEditControl()->GetSafeHwnd();
        pTI->lpszText = LPSTR_TEXTCALLBACK;
      }
    }
    else if(skin)
    {
      CString check;
      skin->GetWindowText(check);
      if(check.Compare("CXSF") == 0)
      {
        CWnd* control = ((SkinScrollWnd*) skin)->GetControl();
        if(control)
        {
          pTI->uId = (UINT_PTR) control->GetSafeHwnd();
          pTI->lpszText = LPSTR_TEXTCALLBACK;
        }
      }
    }
  }
  return result;
}

void
StyleTab::RegisterTooltip(int p_ID,const char* p_text)
{
  CWnd* wnd = GetDlgItem(p_ID);
  if(wnd && wnd->GetSafeHwnd())
  {
    m_tooltips[wnd->GetSafeHwnd()] = p_text;
  }
}

void
StyleTab::RegisterTooltip(CWnd& p_wnd,const char* p_text)
{
  if(p_wnd.GetSafeHwnd())
  {
    m_tooltips[p_wnd.GetSafeHwnd()] = p_text;
  }
}

void
StyleTab::RegisterTooltip(StyleComboBox& p_wnd,const char* p_text)
{
  if(p_wnd.GetSafeHwnd() && p_wnd.GetEditControl()->GetSafeHwnd())
  {
    m_tooltips[p_wnd.GetSafeHwnd()] = p_text;
    m_tooltips[p_wnd.GetEditControl()->GetSafeHwnd()] = p_text;
  }
}

BOOL
StyleTab::OnToolTipNotify(UINT id,NMHDR * pNMHDR,LRESULT * pResult)
{
  TOOLTIPTEXT* pTTT = (TOOLTIPTEXT*) pNMHDR;

  if(pTTT->uFlags & TTF_IDISHWND)
  {
    ToolTips::iterator tip = m_tooltips.find((HWND) pNMHDR->idFrom);
    if(tip != m_tooltips.end())
    {
      pTTT->lpszText = (LPSTR) tip->second;
    }
  }
  // Return value of TTN_NEEDTEXT is not used.
  return TRUE;
}

BOOL
StyleTab::OnEraseBkgnd(CDC* pDC)
{
  CRect client;
  GetClientRect(client);
  pDC->FillSolidRect(client,ThemeColor::GetColor(Colors::ColorWindowFrame));
  return TRUE;
}

LPARAM
StyleTab::OnCtlColorStatic(WPARAM wParam,LPARAM /*lParam*/)
{
  HDC hdc = (HDC) wParam;
  SetTextColor(hdc,ThemeColor::GetColor(Colors::ColorEditText));
  SetBkColor  (hdc,ThemeColor::GetColor(Colors::ColorWindowFrame));
  return (LPARAM) (HBRUSH) m_defaultBrush;
}

LPARAM
StyleTab::OnCtlColorListBox(WPARAM wParam,LPARAM lParam)
{
  HDC hdc = (HDC) wParam;
  SetTextColor(hdc,ThemeColor::GetColor(Colors::ColorEditText));
  SetBkColor  (hdc,ThemeColor::GetColor(Colors::ColorCtrlBackground));
  return (LPARAM) (HBRUSH) m_defaultBrush;
}

HBRUSH 
StyleTab::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
  return CDialog::OnCtlColor(pDC,pWnd,nCtlColor);
}

void
StyleTab::OnCancel()
{
  return;
}

void
StyleTab::OnOK()
{
  return;
}
