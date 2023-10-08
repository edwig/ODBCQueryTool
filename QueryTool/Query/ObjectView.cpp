/*
  Copyright (C) 2017 Edwig Huisman

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
#include "ObjectView.h"
#include "QueryTool.h"
#include "SQLInfoDB.h"
#include "Query\NativeSQLDlg.h"

//////////////////////////////////////////////////////////////////////////
//
//  Dockable View
//
//////////////////////////////////////////////////////////////////////////

class CObjectViewMenuButton : public CMFCToolBarMenuButton
{
  friend class CClassView;

  DECLARE_SERIAL(CObjectViewMenuButton)

public:
  CObjectViewMenuButton(HMENU hMenu = nullptr) noexcept : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
  {
  }

  virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE,
    BOOL bCustomizeMode = FALSE, BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE)
  {
    pImages = CMFCToolBar::GetImages();

    CAfxDrawState ds;
    pImages->PrepareDrawImage(ds);

    CMFCToolBarMenuButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);

    pImages->EndDrawImage(ds);
  }
};

IMPLEMENT_SERIAL(CObjectViewMenuButton, CMFCToolBarMenuButton, 1)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ObjectView::ObjectView() noexcept
{
}

ObjectView::~ObjectView()
{
}

BEGIN_MESSAGE_MAP(ObjectView, CDockablePane)
  ON_WM_CREATE()
  ON_WM_SIZE()
  ON_WM_PAINT()
  ON_WM_SETFOCUS()
  ON_WM_CONTEXTMENU()
  ON_CBN_SETFOCUS (ID_FPW_TABFILTER,OnTable_SetFocus)
  ON_CBN_SELCHANGE(ID_FPW_TABFILTER,OnTable_SelChange)
  ON_CBN_KILLFOCUS(ID_FPW_TABFILTER,OnTable_SelChange)
  ON_NOTIFY(NM_RCLICK,ID_FPW_TABLE, OnTableTree_RClick)
  ON_COMMAND      (ID_TABLE_FIRST,  OnTableFirst)
  ON_COMMAND      (ID_TABLE_DDL,    OnTableDDL)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassView message handlers

int ObjectView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CDockablePane::OnCreate(lpCreateStruct) == -1)
    return -1;

  CRect rectDummy;
  rectDummy.SetRectEmpty();

  // Create views:
  const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

  m_wndObjectTree.SetDirectInit(false);
  if (!m_wndObjectTree.Create(dwViewStyle, rectDummy, this, ID_FPW_TABLE))
  {
    TRACE0("Failed to create ODBC Info View\n");
    return -1;      // fail to create
  }
  m_wndObjectTree.InitSkin();
  m_wndObjectTree.CreateImageList();

  if(!m_tableCBox.Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|CBS_DROPDOWN,CRect(0, 0, 40, GetSystemMetrics(SM_CYFULLSCREEN) / 2),this,ID_FPW_TABFILTER))
  {
    TRACE0("Failed to create filter combo box\n");
   
    return -1;
  }
  m_tableCBox.InitSkin();
  m_tableCBox.ReadjustDynamic();
  // So we can empty the combo again
  m_tableCBox.AddString(_T(""));
  m_tableCBox.SetFocusDots(false);
  SetStyleFxComboBoxDots(false);

  EnableToolTips(TRUE);
  m_tableCBox.EnableToolTips(TRUE);

  // Fill in some static tree view data (dummy code, nothing magic here)
  FillObjectView();
  AdjustLayout();

  return 0;
}

void
ObjectView::OnSize(UINT nType, int cx, int cy)
{
  CDockablePane::OnSize(nType, cx, cy);
  AdjustLayout();
}

void ObjectView::FillObjectView()
{
}

void ObjectView::OnContextMenu(CWnd* pWnd, CPoint point)
{
  StyleTreeCtrl* pWndTree = (StyleTreeCtrl*)&m_wndObjectTree;
  ASSERT_VALID(pWndTree);

  if (pWnd != pWndTree)
  {
    CDockablePane::OnContextMenu(pWnd, point);
    return;
  }

  if (point != CPoint(-1, -1))
  {
    // Select clicked item:
    CPoint ptTree = point;
    pWndTree->ScreenToClient(&ptTree);

    UINT flags = 0;
    HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
    if (hTreeItem != nullptr)
    {
      pWndTree->SelectItem(hTreeItem);
    }
  }

  pWndTree->SetFocus();
  CMenu menu;
  menu.LoadMenu(IDR_POPUP_SORT);

  CMenu* pSumMenu = menu.GetSubMenu(0);

  if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
  {
    CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

    if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
      return;

    ((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
    UpdateDialogControls(this, FALSE);
  }
}

void 
ObjectView::ClearObjectTree()
{
  // Clear the tree
  m_wndObjectTree.SetFilter(_T(""));
  m_wndObjectTree.ClearTree();
}

#define COMBOBOX_SIZE_HEIGHT 24

void
ObjectView::AdjustLayout()
{
  if (GetSafeHwnd() == nullptr)
  {
    return;
  }

  UINT flags = SWP_NOACTIVATE | SWP_NOZORDER;
  CRect rectClient;
  GetClientRect(rectClient);

  m_tableCBox.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), COMBOBOX_SIZE_HEIGHT, flags);

  CRect comboRect;
  m_tableCBox.GetWindowRect(&comboRect);
  int size = comboRect.Height();
  m_wndObjectTree.GetSkin()->SetWindowPos(nullptr, rectClient.left, rectClient.top + size, rectClient.Width(), rectClient.Height() - size, SWP_NOACTIVATE | SWP_NOZORDER);
}

void
ObjectView::OnPaint()
{
  CPaintDC dc(this); // device context for painting

  CRect rectTree;
  m_wndObjectTree.GetWindowRect(rectTree);
  ScreenToClient(rectTree);

  rectTree.InflateRect(1, 1);
  dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void
ObjectView::OnSetFocus(CWnd* pOldWnd)
{
  CDockablePane::OnSetFocus(pOldWnd);

  m_wndObjectTree.SetFocus();
  DisplayTables();
}

void
ObjectView::FindTable(CString& table)
{
  // Add to listbox
  AddToTables(table);
  m_tableCBox.SelectString(-1, table);

  m_wndObjectTree.ClearTree();

  // Expand total table
  m_wndObjectTree.ExpandFirstTable(table);
}

void
ObjectView::OnTable_SetFocus()
{
  DisplayTables();
}

void
ObjectView::DisplayTables(BOOL force, BOOL curOnly)
{
  if (force)
  {
    if (!curOnly)
    {
      m_tableCBox.LockWindowUpdate();
    }
    m_tableCBox.SetRedraw(FALSE);
    m_tableCBox.ResetContent();

    for (int ind = 0; ind < m_tables.GetCount(); ++ind)
    {
      CString text = m_tables.GetAt(ind);
      text.GetBuffer();
      m_tableCBox.AddString(text);
      text.ReleaseBuffer();
    }
    m_tableCBox.SetRedraw(TRUE);
    if (!curOnly)
    {
      m_tableCBox.UnlockWindowUpdate();
    }
  }
}

void
ObjectView::OnTable_SelChange()
{
  // Get current selection
  CString table;
  m_tableCBox.GetWindowText(table);
  table.Trim();
  if (!table.IsEmpty())
  {
    AddToTables(table);
  }
  // Compare with filter, if changed
  if (m_wndObjectTree.GetFilter().CompareNoCase(table))
  {
    m_wndObjectTree.ClearTree();
    m_wndObjectTree.SetFilter(table);
  }
}

void 
ObjectView::AddToTables(CString table)
{
  for (int ind = 0; ind < m_tables.GetCount(); ++ind)
  {
    CString text = m_tables.GetAt(ind);
    if (!text.Find(table) && (text.GetLength() == table.GetLength()))
    {
      // OK, Already in the list
      return;
    }
  }
  m_tables.Add(table);
  m_tableCBox.AddString(table);
}

void
ObjectView::OnTableTree_RClick(NMHDR* pNMHDR, LRESULT* pResult)
{
  CPoint point;
  ::GetCursorPos(&point);

  m_tableCBox.GetErrorState();

  HTREEITEM hCurSel = m_wndObjectTree.GetNextItem(TVI_ROOT, TVGN_CARET);
  if (hCurSel)
  {
    if(m_wndObjectTree.OnTableOrView(hCurSel))
    {
      CMenu menu;
      VERIFY(menu.LoadMenu(IDR_OE_TABLES_POPUP));
      CMenu* pPopup = menu.GetSubMenu(0);
      ASSERT(pPopup != NULL);
      pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
    }
  }
  *pResult = 1;
}

void
ObjectView::OnTableFirst()
{
  HTREEITEM hCurSel = m_wndObjectTree.GetNextItem(TVI_ROOT, TVGN_CARET);
  if(hCurSel)
  {
    m_wndObjectTree.OnTableFirst(hCurSel);
  }
}

void
ObjectView::OnTableDDL()
{
  HTREEITEM hCurSel = m_wndObjectTree.GetNextItem(TVI_ROOT, TVGN_CARET);
  if(hCurSel)
  {
    m_wndObjectTree.OnTableDDL(hCurSel);
  }
}

void
ObjectView::OnTable_Enter()
{
  CString filter;
  m_tableCBox.GetWindowText(filter);
  filter.Trim();

  // Potentially add to list
  if (!filter.IsEmpty())
  {
    AddToTables(filter);
  }

  // Compare with filter, if changed
  if (m_wndObjectTree.GetFilter().CompareNoCase(filter))
  {
    m_wndObjectTree.ClearTree();
    m_wndObjectTree.SetFilter(filter);
  }
}

BOOL
ObjectView::PreTranslateMessage(MSG* pMsg)
{
  if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
  {
    OnTable_Enter();
    return TRUE;
  }
  return CDockablePane::PreTranslateMessage(pMsg);
}
