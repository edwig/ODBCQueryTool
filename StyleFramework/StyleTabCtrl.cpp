////////////////////////////////////////////////////////////////////////
//
// File: StyleTabCtrl.cpp
// Function: Styling frame for a CTabCtrl
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
#include "StyleTabCtrl.h"
#include "StyleFonts.h"
#include "StyleColors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(StyleTabCtrl,CTabCtrl)

StyleTabCtrl::StyleTabCtrl()
{
  m_selColor   = ClrTabTextActive;
  m_unselColor = ClrTabTextInactive;
  m_font       = &STYLEFONTS.DialogTextFontBold;
  m_hover      = -1;
  m_notch      = false;
  m_offset     = 0;

  m_brush.CreateSolidBrush(UsersBackground);
}

StyleTabCtrl::~StyleTabCtrl()
{
}

BEGIN_MESSAGE_MAP(StyleTabCtrl, CTabCtrl)
  ON_WM_PAINT()
  ON_WM_CREATE()
  ON_WM_SIZE()
  ON_WM_ERASEBKGND()
  ON_WM_CTLCOLOR()
  ON_MESSAGE(WM_CTLCOLORSTATIC,   OnCtlColorStatic)
  ON_NOTIFY_REFLECT(TCN_SELCHANGE,OnTcnSelchangeTabs)
END_MESSAGE_MAP()

int 
StyleTabCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CTabCtrl::OnCreate(lpCreateStruct) == -1)
  {
    return -1;
  }
  ModifyStyle(0, TCS_OWNERDRAWFIXED);
  return 0;
}

void 
StyleTabCtrl::PreSubclassWindow()
{
  CTabCtrl::PreSubclassWindow();
  ModifyStyle(0, TCS_OWNERDRAWFIXED);
  SetFont(&STYLEFONTS.DialogTextFontBold);
}

HBRUSH
StyleTabCtrl::OnCtlColor(CDC* /*pDC*/,CWnd* /*pWnd*/,UINT /*nCtlColor*/)
{
  return (HBRUSH)m_brush;
}

LPARAM
StyleTabCtrl::OnCtlColorStatic(WPARAM wParam,LPARAM /*lParam*/)
{
  HDC hdc = (HDC)wParam;
  SetTextColor(hdc,InputTextActive);
  SetBkColor(hdc,UsersBackground);
  return (LPARAM)(HBRUSH)m_brush;
}

BOOL
StyleTabCtrl::OnEraseBkgnd(CDC* pDC)
{
  CRect rect;
  GetClientRect(&rect);
  pDC->FillSolidRect(&rect,UsersBackground);
  return TRUE;
}

// Insert a CWnd on a tab, so that the OnSize may work
LONG
StyleTabCtrl::InsertItem(int p_item,CWnd* p_wnd,CString p_text)
{
  // See to it that the tab control knows the height of the items
  CSize size(0,tabHeaderHeight);
  SetItemSize(size);

  return CTabCtrl::InsertItem(TCIF_TEXT|TCIF_PARAM
                             ,p_item                // Number
                             ,p_text.GetString()    // Text on the tab
                             ,0                     // image
                             ,(LPARAM) p_wnd        // Param
                             ,0                     // State
                             ,0);                   // State mask
}

// Call Init after all tabs are inserted
void
StyleTabCtrl::Init()
{
  CRect rect;
  GetWindowRect(&rect);
  OnSize(0, rect.Width(), rect.Height());

  // Select the first tab
  SetTabActive(0, true);
  SelectTab(0);
}

void 
StyleTabCtrl::SetColours(COLORREF selColour, COLORREF unselColour)
{
  m_selColor   = selColour;
  m_unselColor = unselColour;
  Invalidate();
}

// Setting the image list on a BITMAP resource IDB_* reference
void 
StyleTabCtrl::SetImages(UINT p_bitmap)
{
  CBitmap bitmap;
  bitmap.LoadBitmap(p_bitmap);
  m_images.Create(16, 16, ILC_COLOR24 | ILC_MASK, 16, 1);
  m_images.Add(&bitmap, RGB(255, 0, 255));
  CTabCtrl::SetImageList(&m_images);
}

// Setting a tab from the BITMAP IDB_* reference
void
StyleTabCtrl::SetTabImage(int p_tab, int p_image)
{
  TCITEM item;
  item.mask = TCIF_IMAGE;
  item.iImage = p_image;
  SetItem(p_tab, &item);
}

void 
StyleTabCtrl::SetUseTabNotch(int p_notch)
{
  m_notch = p_notch;
  Invalidate();
}

void 
StyleTabCtrl::AdjustRect(BOOL bLarger, LPRECT lpRect)
{
  CRect client;
  GetClientRect(client);
  lpRect->top    = client.top;
  lpRect->left   = client.left;
  lpRect->bottom = client.bottom;
  lpRect->right  = client.right;

  if (bLarger)
  {
    lpRect->bottom = client.top + tabHeaderHeight;
  }
  else
  {
    lpRect->top = client.top + tabHeaderHeight;
  }
}

void
StyleTabCtrl::OnSize(UINT nType,int cx,int cy)
{
  CTabCtrl::OnSize(nType,cx,cy);

  CRect rect;
  GetClientRect(&rect);

  rect.top    += 4 + tabHeaderHeight;
  rect.left   += 1;
  rect.right  -= 1;
  rect.bottom -= 1;

  if (m_notch)
  {
    rect.top += tabHeaderNotch;
  }

  for(int index = 0;index < GetItemCount();++index)
  {
    TCITEM item;
    memset(&item,0,sizeof(TCITEM));
    item.mask = TCIF_PARAM;
    GetItem(index,&item);

    CWnd* wnd = reinterpret_cast<CWnd*>(item.lParam);
    if(wnd)
    {
      wnd->MoveWindow(rect,true);
    }
  }
}

void
StyleTabCtrl::OnPaint()
{
  if (GetUpdateRect(NULL))
  {
    CPaintDC paint(this);
    CDC* dc = GetDC();
    dc->SelectObject(&STYLEFONTS.DialogTextFont);

    CRect client;
    GetClientRect(client);
    //client.bottom = client.top + tabHeaderHeight;
    const CRect itemrect(client.left, client.top, client.right, client.top + tabHeaderHeight);
    CRect rect;

    // Draw items    
    // If the last item contained errors, Draw left line with "ClrWindowFrameError".
    bool lastTabHasErrors = false;
    // int teller = 0;
    int count  = GetItemCount();
    int cursel = TabCtrl_GetCurSel(GetSafeHwnd());

    for (int ind = 0;ind < count; ++ind)
    {
      // Getting the icon image an the tab text
      char buffer[MAX_PATH + 1];
      TCITEM item;
      item.mask       = TCIF_TEXT | TCIF_IMAGE;
      item.pszText    = buffer;
      item.cchTextMax = MAX_PATH;
      item.iImage     = -1;
      GetItem(ind,&item);

      // Getting the rectangle for the tab item
      // Inflate them, so the collide
      GetItemRect(ind, rect);
      rect.InflateRect(2, 2);

      // Getting the active state
      int active = GetTabActive(ind);

      --rect.right;
      // Normal color and error color
      CPen normalPen(PS_SOLID, 1, ClrTabFrame);
      CPen errorPen (PS_SOLID, 1, ClrWindowFrameError);
      dc->SelectObject(GetErrorState(ind) ? errorPen : normalPen);

      if (dc->RectVisible(rect))
      {
        COLORREF clrbkgnd;
        COLORREF clrtext;
        // bool close = false;

        if (!active)
        {
          clrbkgnd = ClrTabBkGndInactive;
          clrtext  = ClrTabTextInactive;
        }
        else if (ind == cursel)
        {
          clrbkgnd = ClrTabBkGndSelected;
          clrtext  = ClrTabTextSelected;
          // close = it->m_closable;
        }
        else if (ind == m_hover)
        {
          clrbkgnd = ClrTabBkGndHover;
          clrtext  = ClrTabTextHover;
        }
        else
        {
          clrbkgnd = ClrTabBkGndActive;
          clrtext  = ClrTabTextActive;
        }

        CBrush brush(clrbkgnd);
        HGDIOBJ org = dc->SelectObject(&brush);

        POINT points[7];
        points[0].x = rect.left;
        points[0].y = rect.bottom - 1 - (m_notch ? tabHeaderNotch : 0);
        points[1].x = rect.left;
        points[1].y = rect.top;
        points[2].x = rect.right - 1;
        points[2].y = rect.top;
        points[3].x = rect.right - 1;
        points[3].y = rect.bottom - 1 - (m_notch ? tabHeaderNotch : 0);

        if (ind == cursel)
        {
          // Optionally paint the notch (arrow down under the tab)
          points[4].x = rect.left + rect.Width() / 2 + 5;
          points[4].y = rect.bottom - 1 - (m_notch ? tabHeaderNotch : 0);
          points[5].x = rect.left + rect.Width() / 2;
          points[5].y = rect.bottom - 1;
          points[6].x = rect.left + rect.Width() / 2 - 5;
          points[6].y = rect.bottom - 1 - (m_notch ? tabHeaderNotch : 0);

          dc->Polygon(points, 7);

          if (lastTabHasErrors)
          {
            dc->SelectObject(errorPen);
            dc->Polygon(points, 2);
          }
        }
        else
        {
          dc->Polygon(points, 4);

          if (lastTabHasErrors)
          {
            dc->SelectObject(errorPen);
            dc->Polygon(points, 2);
          }

          if (m_notch)
          {
            points[0].y += tabHeaderNotch;
            points[3].y += tabHeaderNotch;
            points[1].y = points[0].y - tabHeaderNotch + 1;
            points[2].y = points[3].y - tabHeaderNotch + 1;
            CBrush blank(ClrTabBkGndActive);
            CPen   nopen(PS_SOLID,1,ClrTabBkGndActive);
            dc->SelectObject(blank);
            dc->SelectObject(nopen);
            dc->Polygon(points, 4);
          }

        }
        dc->SelectObject(org);

        rect.bottom -= (m_notch ? tabHeaderNotch : 0);
        dc->SetBkColor(clrbkgnd);
        dc->SetTextColor(clrtext);

        if (GetErrorState(ind))
        {
          CRect r = rect;
          r.right  -= 5;
          r.left    = r.right - 4;
          r.top    += 8 - (m_notch ? tabHeaderNotch / 2 : 0);
          r.bottom += 8 - (m_notch ? tabHeaderNotch / 2 : 0);
          PaintError(dc, r);
        }

        // Draw the icon on the tab
        if (item.iImage >= 0)
        {
          CImageList* list = GetImageList();
          HICON icon = list->ExtractIcon(item.iImage);

          CRect bmpRect(rect);
          bmpRect.left  += 4;
          bmpRect.top   += (bmpRect.Height() - 16) / 2;
          bmpRect.right  = bmpRect.left + 16;
          bmpRect.bottom = bmpRect.top  + 16;
          DrawIconEx(dc->m_hDC, bmpRect.left, bmpRect.top, icon, 16, 16, 0, NULL,active ?  DI_NORMAL : DI_MASK);
          rect.left += 24;
        }

        // Draw the text on the tab
        if (GetErrorState(ind))
        {
          CRect text(rect);
          text.right -= 10;
          dc->DrawText(item.pszText, text, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        }
        else
        {
          dc->DrawText(item.pszText, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        }
      }
      lastTabHasErrors = GetErrorState(ind);
    }
    CPen normalPen(PS_SOLID, 1, ClrTabFrame);

    if (!lastTabHasErrors)
    {
      dc->SelectObject(normalPen);
    }

    // Line on the right side (bottom to top)
    dc->SelectObject(normalPen);
    int begin = m_notch ? client.left : rect.right;
    dc->MoveTo(begin,           itemrect.bottom + 3);
    dc->LineTo(client.right - 1,itemrect.bottom + 3);

    // Get the working space
    dc->MoveTo(rect.right,      client.top);
    dc->LineTo(client.right -1, client.top);
    dc->LineTo(client.right -1, client.bottom - 1);
    dc->LineTo(client.left,     client.bottom - 1);
    dc->LineTo(client.left,     client.top);

    ReleaseDC(dc);
  }
}

void 
StyleTabCtrl::PaintError(CDC* pDC, CRect rect)
{
  HGDIOBJ  oldfont  = pDC->SelectObject(STYLEFONTS.ErrorTextFont);
  COLORREF oldcolor = pDC->SetTextColor(ClrWindowFrameError);
  int      oldmode  = pDC->SetBkMode(TRANSPARENT);

  pDC->DrawText("!", &rect, DT_RIGHT | DT_SINGLELINE);

  pDC->SetTextColor(oldcolor);
  pDC->SelectObject(oldfont);
  pDC->SetBkMode(oldmode);
}

// If no errors are registered, the errors number is ZERO!
int
StyleTabCtrl::GetErrorState(int p_item)
{
  if(p_item >= 0 && p_item < (int)m_errors.size())
  {
    return m_errors[p_item];
  }
  return 0;
}

// If no active state is registered, the tab is ACTIVE!
bool
StyleTabCtrl::GetTabActive(int p_item)
{
  if(p_item >= 0 && p_item < (int)m_active.size())
  {
    return m_active[p_item];
  }
  return 1;
}

// Add to the errors map if no errors still found
void
StyleTabCtrl::SetErrorState(int p_item,int p_errors)
{
  while ((int)m_errors.size() <= p_item)
  {
    m_errors.push_back(0);
  }
  m_errors[p_item] = p_errors;
  Invalidate();
}

// Add to the active state map, if no states found
void 
StyleTabCtrl::SetTabActive(int p_item,bool p_active)
{
  while((int)m_active.size() <= p_item)
  {
    // Default active tab
    m_active.push_back(true);
  }
  m_active[p_item] = p_active;
  Invalidate();
}

void 
StyleTabCtrl::SelectTab(int p_tab)
{
  for (int index = 0; index < GetItemCount(); ++index)
  {
    TCITEM item;
    memset(&item, 0, sizeof(TCITEM));
    item.mask = TCIF_PARAM;
    GetItem(index, &item);

    CWnd* wnd = reinterpret_cast<CWnd*>(item.lParam);
    if (wnd)
    {
      wnd->ShowWindow(p_tab == index ? SW_SHOW : SW_HIDE);
    }
  }
}

void
StyleTabCtrl::OnTcnSelchangeTabs(NMHDR* pNMHDR, LRESULT* pResult)
{
  int num = TabCtrl_GetCurSel(pNMHDR->hwndFrom);

  // Make this tab visible
  SelectTab(num);
  *pResult = 0;
}