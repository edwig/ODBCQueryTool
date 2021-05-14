////////////////////////////////////////////////////////////////////////
//
// File: StyleFormView.cpp
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
#include "StyleFormView.h"
#include "StyleFonts.h"

IMPLEMENT_DYNAMIC(StyleFormView,CFormView)

BEGIN_MESSAGE_MAP(StyleFormView,CFormView)
  ON_WM_PAINT()
  ON_WM_SIZE()
  ON_WM_WINDOWPOSCHANGED()
END_MESSAGE_MAP()

StyleFormView::StyleFormView(LPCTSTR lpszTemplateName)
              :CFormView(lpszTemplateName)
{
}

StyleFormView::StyleFormView(UINT nIDtemplate)
              :CFormView(nIDtemplate)
{
}

StyleFormView::~StyleFormView()
{
}

void
StyleFormView::Init(int p_borderSize /*=0*/)
{
  SetFont(&STYLEFONTS.DialogTextFont);
  SkinScrollWnd* skin = SkinWndScroll(this, p_borderSize);
  skin->SetScrollbarBias(1);
}

SkinScrollWnd*
StyleFormView::GetSkin()
{
  return (SkinScrollWnd*)GetWindowLongPtr(GetSafeHwnd(), GWLP_USERDATA);
}

void
StyleFormView::OnPaint()
{
  CFormView::OnPaint();
}

void
StyleFormView::OnSize(UINT nType, int cx, int cy)
{
  CFormView::OnSize(nType,cx,cy);

  SkinScrollWnd* skin = GetSkin();
  if (skin && skin->m_moveClient == FALSE)
  {
    // Must we resize the skin frame?
    CRect client;
    skin->CalculateControlSize(client);
    CRect frame;
    skin->GetWindowRect(&frame);
    ::OffsetRect(&frame, -frame.left, -frame.top);

    // Resize if we must. 
    if (client.Width() != cx || client.Height() != cy)
    {
      frame.right  = frame.left + cx;
      frame.bottom = frame.top  + cy;

      skin->MoveWindow(frame.left, frame.top, frame.Width(), frame.Height());
      skin->RepositionControlWindow();
    }
  }
}

void
StyleFormView::OnWindowPosChanged(WINDOWPOS* pos)
{
  CFormView::OnWindowPosChanged(pos);

  SkinScrollWnd* skin = GetSkin();
  if(skin && skin->m_moveClient == FALSE)
  {
    bool mustMove = false;
    bool canMove  = (pos->flags & SWP_NOMOVE) == 0;
    bool canSize  = (pos->flags & SWP_NOSIZE) == 0;

    if(canMove || canSize)
    {
      // Must we resize the skin frame?
      CRect client;
      skin->CalculateControlSize(client);
      CRect frame;
      skin->GetWindowRect(&frame);
      ::OffsetRect(&frame,-frame.left,-frame.top);

      if(canMove && canSize)
      {
        // Full resize of the window. All positions different from the message
        if(pos->x  != client.left    || pos->y  != client.top ||
           pos->cx != client.Width() || pos->cy != client.Height())
        {
          mustMove     = true;
          frame.left   = pos->x;
          frame.top    = pos->y;
          frame.right  = pos->x + pos->cx;
          frame.bottom = pos->y + pos->cy;
        }
      }
      else if (canMove || pos->x || pos->y)
      {
        // Move if we must. But do NOT rely on the cx and cy of the message
        if(client.left != pos->x && client.top != pos->y)
        {
          mustMove     = true;
          frame.right  = pos->x + frame.Width();
          frame.bottom = pos->y + frame.Height();
          frame.left   = pos->x;
          frame.top    = pos->y;
        }
      }
      else if (canSize || pos->cx || pos->cy)
      {
        // Resize if we must. But do NOT rely on the x and y of the message
        if(client.Width() != pos->cx || client.Height() != pos->cy)
        {
          mustMove     = true;
          frame.right  = frame.left + pos->cx;
          frame.bottom = frame.top  + pos->cy;
        }
      }
      // Now resize the skinning frame if we must
      if(mustMove)
      {
        skin->MoveWindow(frame.left,frame.top,frame.Width(),frame.Height());
        skin->RepositionControlWindow();
      }
    }
  }
}
