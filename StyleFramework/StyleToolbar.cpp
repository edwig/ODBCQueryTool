////////////////////////////////////////////////////////////////////////
//
// File: StyleToolBar.cpp
// Function: Styled progress control
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
#include "StyleToolbar.h"
#include "afxtoolbarmenubuttonsbutton.h"

#define AFX_BUTTON_MIN_WIDTH 5
#define AFX_STRETCH_DELTA    6

IMPLEMENT_SERIAL(StyleToolBar,CMFCToolBar,VERSIONABLE_SCHEMA | 1)

BEGIN_MESSAGE_MAP(StyleToolBar,CMFCToolBar)
  ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


void 
StyleToolBar::OnLButtonUp(UINT nFlags,CPoint point)
{
  if(IsCustomizeMode() && !m_bLocked)
  {
    if(m_bStretchButton)
    {
      ASSERT_VALID(m_pDragButton);

      CRect rect = m_pDragButton->Rect();
      rect.right = point.x;

      if(rect.Width() >= AFX_BUTTON_MIN_WIDTH && abs(m_pDragButton->Rect().right - point.x) > AFX_STRETCH_DELTA)
      {
        m_pDragButton->OnSize(rect.Width());
        AdjustLayout();
      }

      m_rectTrack.SetRectEmpty();

      m_pDragButton = NULL;
      m_bStretchButton = FALSE;

      RedrawWindow(NULL,NULL,RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);

      ::ReleaseCapture();

      if(m_pWndLastCapture != NULL)
      {
        m_pWndLastCapture->SetCapture();
        m_pWndLastCapture = NULL;
      }

      AdjustSizeImmediate();
    }
    CMFCBaseToolBar::OnLButtonUp(nFlags,point);
    return;
  }

  if(m_bDragMode)
  {
    CMFCBaseToolBar::OnLButtonUp(nFlags,point);
    return;
  }

  if(m_iButtonCapture == -1)
  {
    if(HitTest(point) == -1)
    {
      CMFCBaseToolBar::OnLButtonUp(nFlags,point);

      m_ptLastMouse = CPoint(-1,-1);
      OnMouseMove(0,point);
    }

    return;     // not captured
  }

  ::ReleaseCapture();
  if(m_pWndLastCapture != NULL)
  {
    m_pWndLastCapture->SetCapture();
    m_pWndLastCapture = NULL;
  }

  m_iHighlighted = HitTest(point);

  CMFCToolBarButton* pButton = GetButton(m_iButtonCapture);
  if(pButton == NULL)
  {
    ASSERT(FALSE);
    return;
  }

  const BOOL bIsSystemMenuButton = pButton->IsKindOf(RUNTIME_CLASS(CMFCToolBarMenuButtonsButton));

  ASSERT(!(pButton->m_nStyle & TBBS_SEPARATOR));
  UINT nIDCmd = 0;

  UINT nNewStyle = (pButton->m_nStyle & ~TBBS_PRESSED);
  if(m_iButtonCapture == m_iHighlighted)
  {
    // we did not lose the capture
    if(HitTest(point) == m_iButtonCapture)
    {
      // give button a chance to update
      UpdateButton(m_iButtonCapture);

      // then check for disabled state
      if(!(pButton->m_nStyle & TBBS_DISABLED))
      {
        // pressed, will send command notification
        nIDCmd = pButton->m_nID;

        if(pButton->m_nStyle & TBBS_CHECKBOX)
        {
          // auto check: three state => down
          if(nNewStyle & TBBS_INDETERMINATE)
          {
            nNewStyle &= ~TBBS_INDETERMINATE;
          }
          nNewStyle ^= TBBS_CHECKED;
        }
      }
    }
  }

  if(m_hookMouseHelp == NULL)
  {
    GetOwner()->PostMessage(WM_SETMESSAGESTRING,AFX_IDS_IDLEMESSAGE);
  }

  int iButtonCapture = m_iButtonCapture;
  m_iButtonCapture = -1;
  m_iHighlighted = -1;

  HWND hwndSaved = m_hWnd;
  RestoreFocus();

  if(HitTest(point) == iButtonCapture && !OnSendCommand(pButton) && nIDCmd != 0 && nIDCmd != (UINT)-1)
  {
    InvalidateButton(iButtonCapture);
    UpdateWindow(); // immediate feedback

    AddCommandUsage(nIDCmd);

    if(!pButton->OnClickUp() && (afxUserToolsManager == NULL || !afxUserToolsManager->InvokeTool(nIDCmd)))
    {
      // BUG in MFC 14.28. Changed this from "SendMessage" to "PostMessage"
      GetOwner()->PostMessage(WM_COMMAND,nIDCmd);    // send command
    }
  }
  else
  {
    if(::IsWindow(hwndSaved) && !::IsIconic(hwndSaved) && ::IsZoomed(hwndSaved))
    {
      pButton->OnClickUp();
    }
  }

  if(::IsWindow(hwndSaved) &&                   // "This" may be destroyed now!
     iButtonCapture < m_Buttons.GetCount())     // Button may disappear now!
  {
    if(bIsSystemMenuButton)
    {
      CMFCToolBarButton* pButtonCapt = GetButton(iButtonCapture);
      if(pButtonCapt != NULL)
      {
        ASSERT_VALID(pButtonCapt);
        pButtonCapt->m_nStyle &= ~TBBS_PRESSED;
      }
    }
    else
    {
      SetButtonStyle(iButtonCapture,nNewStyle);
    }

    UpdateButton(iButtonCapture);
    InvalidateButton(iButtonCapture);
    UpdateWindow(); // immediate feedback

    m_ptLastMouse = CPoint(-1,-1);
    OnMouseMove(0,point);
  }
}
