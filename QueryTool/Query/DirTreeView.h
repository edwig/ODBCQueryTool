////////////////////////////////////////////////////////////////////////
//
// File: DirTreeView.h
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
#pragma once
#include "DirTreeCtrl.h"

//////////////////////////////////////////////////////////////////////////
//
// VIEW
//
//////////////////////////////////////////////////////////////////////////

class CDirToolBar : public CMFCToolBar
{
  virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
  {
    CMFCToolBar::OnUpdateCmdUI((CFrameWnd*)GetOwner(), bDisableIfNoHndler);
  }

  virtual BOOL AllowShowOnList() const { return FALSE; }
};

class DirTreeView : public CDockablePane
{
public:
  DirTreeView() noexcept;
  virtual ~DirTreeView();

  void AdjustLayout();
  void OnChangeVisualStyle();
  void DisplayDrivers(BOOL force = FALSE, BOOL curOnly = FALSE);
  void DisplayFilters(BOOL force = FALSE, BOOL curOnly = FALSE);
  void SelectDrive(const CString& path, BOOL force = FALSE);
  BOOL SetCurPath(const CString&);
  void SetCurDrivePath(const CString& curDrivePath);
  const CString& GetCurDrivePath() const;
  void AddToFilters(CString filter);

protected:
//CDirToolBar   m_wndToolBar;
  CComboBoxEx   m_drivesCBox;
  CComboBox     m_filterCBox;
  CDirTreeCtrl  m_wndDirTree;
  CImageList    m_ClassViewImages;
  UINT          m_nCurrSort;
  bool          m_isExplorerInitialized;
  bool          m_isDrivesInitialized;
  CString       m_curDrivePath;
  CStringArray  m_driverPaths;
  CStringArray  m_filters;
  CImageList    m_dirTreeStateImageList;
  // Overrides
public:
  virtual BOOL PreTranslateMessage(MSG* pMsg);
  void    OnActivate();

protected:

  afx_msg int     OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void    OnSize(UINT nType, int cx, int cy);
  afx_msg void    OnContextMenu(CWnd* pWnd, CPoint point);
  afx_msg void    OnPaint();
  afx_msg void    OnSetFocus(CWnd* pOldWnd);
  afx_msg void    OnDrive_SetFocus();
  afx_msg void    OnDrive_SelChange();
  afx_msg void    OnFpwOpen();
  afx_msg void    OnFpwRefresh();
  afx_msg void    OnFpwSetWorDir();
  afx_msg void    OnFpwRefreshDrivers();
  afx_msg void    OnExplorerTree_DblClick(NMHDR*, LRESULT* pResult);
  afx_msg void    OnExplorerTree_RClick(NMHDR*, LRESULT* pResult);
  afx_msg void    OnDrivers_RClick(NMHDR*, LRESULT* pResult);
  afx_msg BOOL    OnTooltipNotify(UINT wnd, NMHDR* tooltip, LRESULT* res);
  afx_msg void    OnFilter_SelChange();
  afx_msg void    OnFilter_SetFocus();

  DECLARE_MESSAGE_MAP()
};

