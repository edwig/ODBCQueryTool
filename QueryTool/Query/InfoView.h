////////////////////////////////////////////////////////////////////////
//
// File: InfoView.h
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

