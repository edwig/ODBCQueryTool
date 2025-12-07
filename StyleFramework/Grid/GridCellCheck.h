/////////////////////////////////////////////////////////////////////////////
// GridCellCheck.h : header file
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
// For use with CGridCtrl v2.22+
//
//////////////////////////////////////////////////////////////////////
#pragma once
#include "GridCell.h"

class CGridCellCheck : public CGridCell
{
  friend class CGridCtrl;
  DECLARE_DYNCREATE(CGridCellCheck)

public:
  CGridCellCheck();

	BOOL SetCheck(BOOL bChecked = TRUE);
	BOOL GetCheck();
  void SetCenter(bool p_center = true);

  // Operations
	virtual CSize GetCellExtent(CDC* pDC,HWND p_hwnd);
  virtual void OnClick( CPoint PointCellRelative);
  virtual BOOL GetTextRect( LPRECT pRect,HWND p_hwnd);

protected:
  virtual BOOL Draw(HWND p_hwnd,CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd = TRUE);
  CRect GetCheckPlacement(HWND p_hwnd);
  // Data
  BOOL  m_bChecked;
  CRect m_Rect;
  bool  m_center { false };
};

