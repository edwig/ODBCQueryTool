//////////////////////////////////////////////////////////////////////
// GridDropTarget.h : header file
//
// MFC Grid Control - Drag/Drop target implementation
//
// Written by Chris Maunder <cmaunder@mail.com>
// Copyright (c) 1998-2002. All Rights Reserved.
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
// For use with MCGridCtrl v2.10+
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <afxole.h>

class MCGridCtrl;

/////////////////////////////////////////////////////////////////////////////
// CGridDropTarget command target

class MCGridDropTarget : public COleDropTarget
{
public:
    MCGridDropTarget();
    virtual ~MCGridDropTarget();

// Attributes
public:
    MCGridCtrl* m_pGridCtrl;
    BOOL        m_bRegistered;

// Operations
public:
    BOOL Register(MCGridCtrl *pGridCtrl);
    virtual void Revoke();

    BOOL        OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
    DROPEFFECT  OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
    void        OnDragLeave(CWnd* pWnd);
    DROPEFFECT  OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
    DROPEFFECT  OnDragScroll(CWnd* pWnd, DWORD dwKeyState, CPoint point);

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CGridDropTarget)
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CGridDropTarget)
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};
