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

#pragma once
#include "ObjectTree.h"
#include <StyleComboBox.h>
#include <afxtoolbar.h>
#include <afxdockablepane.h>

class CObjectToolBar : public CMFCToolBar
{
  virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
  {
    CMFCToolBar::OnUpdateCmdUI((CFrameWnd*)GetOwner(), bDisableIfNoHndler);
  }

  virtual BOOL AllowShowOnList() const { return FALSE; }
};

//////////////////////////////////////////////////////////////////////////
//
//  Dockable View
//
//////////////////////////////////////////////////////////////////////////

class ObjectView : public CDockablePane
{
public:
  ObjectView() noexcept;
  virtual ~ObjectView();

  void ClearObjectTree();
  void AdjustLayout();
  void FindTable(CString& table);

protected:
  StyleComboBox   m_tableCBox;
  ObjectTree      m_wndObjectTree;
  CStringArray    m_tables;

  void FillObjectView();
  void DisplayTables(BOOL force = FALSE, BOOL curOnly = FALSE);
  void AddToTables(CString table);
  void OnTable_Enter();

  // Overrides
public:
  virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
  afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
  afx_msg void OnPaint();
  afx_msg void OnSetFocus(CWnd* pOldWnd);
  afx_msg void OnTable_SetFocus();
  afx_msg void OnTable_SelChange();
  afx_msg void OnTableTree_RClick(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnTableFirst();
  afx_msg void OnTableDDL();


  DECLARE_MESSAGE_MAP()
};

