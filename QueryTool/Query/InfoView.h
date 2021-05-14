#pragma once
#include "InfoTree.h"

class CInfoToolBar : public CMFCToolBar
{
  virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
  {
    CMFCToolBar::OnUpdateCmdUI((CFrameWnd*)GetOwner(), bDisableIfNoHndler);
  }

  virtual BOOL AllowShowOnList() const { return FALSE; }
};

class InfoView : public CDockablePane
{
public:
  InfoView() noexcept;
  virtual ~InfoView();

  void FillInfoView();
  void AdjustLayout();
  void OnChangeVisualStyle();
  void ReportCapabilities(CString& filename);

protected:
  //CInfoToolBar  m_wndToolBar;
  InfoTree      m_wndInfoTree;
  CImageList    m_ClassViewImages;
  UINT          m_nCurrSort;
  bool          m_isOdbcInitialized;


  // Overrides
public:
  virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
  afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
  afx_msg void OnPaint();
  afx_msg void OnSetFocus(CWnd* pOldWnd);
  afx_msg void OnSort(UINT id);
  afx_msg void OnUpdateSort(CCmdUI* pCmdUI);

  DECLARE_MESSAGE_MAP()

};

