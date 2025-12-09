/////////////////////////////////////////////////////////////////////////////
// GridCellCombo.h : header file
//
// MFC Grid Control - Grid combo cell class header file
//
// Written by Chris Maunder <chris@codeproject.com>
// Copyright (c) 1998-2005. All Rights Reserved.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name and all copyright 
// notices remains intact. 
//
// An email letting me know how you are using it would be nice as well. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// For use with CGridCtrl v2.10
//
//////////////////////////////////////////////////////////////////////
#pragma once
#include "GridCell.h"

class CGridCellCombo : public CGridCell
{
  friend class CGridCtrl;
  DECLARE_DYNCREATE(CGridCellCombo)

public:
  CGridCellCombo();

// editing cells
public:
  virtual BOOL  Edit(int nRow, int nCol, CRect rect, CPoint point, UINT nID, UINT nChar) override;
  virtual void  EndEdit() override;
  virtual CSize GetCellExtent(CDC* pDC,HWND p_hwnd) override;
  virtual CWnd* GetEditWnd() const override;

// CGridCellCombo specific calls
public:
  void  SetOptions(const CStringArray& ar);
  void  SetStyle(DWORD dwStyle);
  DWORD GetStyle()                { return m_dwStyle;     }
  void  SetLastChar(UINT p_last)  { m_nLastChar = p_last; }

protected:
  virtual BOOL Draw(HWND p_hwnd,CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd = TRUE) override;
          void DrawComboButton(HWND p_hwnd,CDC* pDC,CRect p_rect);
          bool CreateNewComboBox(int p_row,int p_col,CRect p_rect,UINT p_id,UINT p_char);

  CStringArray m_Strings;
  DWORD        m_dwStyle   { 0 };
  UINT         m_nLastChar { 0 };
  int          m_row       { 0 };
  int          m_col       { 0 };
};

/////////////////////////////////////////////////////////////////////////////
// CComboEdit window

#define IDC_COMBOEDIT 1001

// class CComboEdit : public CEdit
// {
// // Construction
// public:
// 	CComboEdit();
//   virtual ~CComboEdit();
// 	virtual BOOL PreTranslateMessage(MSG* pMsg);
// 
// 	// Generated message map functions
// protected:
// 	void DrawComboButton();
// 
// 	afx_msg void OnKillFocus(CWnd* pNewWnd);
// 	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
// 	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
// 
// 	DECLARE_MESSAGE_MAP()
// };
// 
// /////////////////////////////////////////////////////////////////////////////
// // CInPlaceList window
// 
// class CInPlaceList : public CComboBox
// {
//   friend class CComboEdit;
// 
// // Construction
// public:
//   CInPlaceList(CWnd*          pParent,      // parent
//                CRect&         rect,         // dimensions & location
//                DWORD          dwStyle,      // window/combobox style
//                UINT           nID,          // control ID
//                int            nRow,
//                int            nColumn,      // row and column
//                COLORREF       crFore,
//                COLORREF       crBack,       // Foreground, background color
//                CStringArray&  Items,        // Items in list
//                CString        sInitText,    // initial selection
//                UINT           nFirstChar);  // first character to pass to control
//   virtual ~CInPlaceList();
//   void EndEdit(bool p_force = false);
// 
//   CComboEdit m_edit;  // subclassed edit control
// 
// // Operations
// protected:
//   virtual void PostNcDestroy();
//   void DrawComboButton();
//   void PositionEditWindow();
//   int  GetCorrectDropWidth();
//   void OnDropdown();
// 
// // Generated message map functions
// protected:
//   afx_msg void OnPaint();
//   afx_msg void OnKillFocus(CWnd* pNewWnd);
//   afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
//   afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
//   afx_msg void OnCbnCloseup();
//   afx_msg void OnCbnDropdown();
//   afx_msg void OnListChoise();
//   afx_msg void OnEndEdit();
//   afx_msg UINT OnGetDlgCode();
//   afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
// 
// 	DECLARE_MESSAGE_MAP()
// 
// private:
//   int        m_nNumLines;
//   CString    m_sInitText;
//   int        m_nRow;
//   int        m_nCol;
//   UINT       m_nLastChar; 
//   BOOL       m_bExitOnArrows; 
//   COLORREF   m_crForeClr;
//   COLORREF   m_crBackClr;
//   HWND       m_hWndList;
// };
