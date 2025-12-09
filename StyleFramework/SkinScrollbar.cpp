////////////////////////////////////////////////////////////////////////
//
// File: SkinScrollbar.cpp
// Function: Add colored vertical/horizontal scroll bars to a control
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
#include "SkinScrollBar.h"
#include "StyleColors.h"
#include "SkinScrollWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TIMERID_NOTIFY	100
#define TIMERID_DELAY	  200
#define TIME_DELAY		   50
#define TIME_INTER		  100

/////////////////////////////////////////////////////////////////////////////
//
// SkinScrollBar: One of the two skinned scrollbars of a control
//                Arrow-heads and thumb-track are in bitmaps of 3 by 9 
//
SkinScrollBar::SkinScrollBar()
{
  memset(&m_si,0,sizeof(SCROLLINFO));
  m_si.nTrackPos = -1;
  m_uClicked     = (UINT) -1;
  m_bNotify      = FALSE;
  m_uHtPrev      = (UINT) -1;
  m_bPause       = FALSE;
  m_bTrace       = FALSE;
  m_bDrag        = FALSE;
}

SkinScrollBar::~SkinScrollBar()
{
}

BEGIN_MESSAGE_MAP(SkinScrollBar, CScrollBar)
  ON_WM_SIZE()
  ON_WM_PAINT()
  ON_WM_LBUTTONDOWN()
  ON_WM_MOUSEMOVE()
  ON_WM_LBUTTONUP()
  ON_WM_TIMER()
  ON_WM_SETFOCUS()
  ON_MESSAGE(WM_MOUSELEAVE,             OnMouseLeave)
  ON_MESSAGE(WM_DPICHANGED_BEFOREPARENT,OnDpiChangedBefore)
END_MESSAGE_MAP()

BOOL SkinScrollBar::IsVertical()
{
  DWORD dwStyle = GetStyle();
  return dwStyle & SBS_VERT;
}

/////////////////////////////////////////////////////////////////////////////
//
// SkinScrollBar message handlers
//
// Translate a hit-point to the SB* codes of a scrollbar
//
UINT SkinScrollBar::HitTest(CPoint pt)
{
  int nTestPos = pt.y;
  if(!IsVertical())
  {
    nTestPos = pt.x;
  }
  if(nTestPos < 0)
  {
    return (UINT) -1;
  }
  SCROLLINFO si = m_si;
  int nInternalHeight = m_nHeight - 2 * m_nWidth;
  if(nInternalHeight < 0)
  {
    nInternalHeight = 0;
  }
  int	nSliderHeight = si.nPage * nInternalHeight / (si.nMax - si.nMin + 1);
  if(nSliderHeight < THUMB_MINSIZE)
  {
    nSliderHeight = THUMB_MINSIZE;
  }
  if(nInternalHeight < THUMB_MINSIZE)
  {
    nSliderHeight = 0;
  }
  int nEmptyHeight = nInternalHeight - nSliderHeight;

  int nArrowHeight = (nInternalHeight == 0) ? (m_nHeight / 2) : m_nWidth;
  int nBottom = 0;
  int nSegLen = nArrowHeight;
  nBottom += nSegLen;
  UINT uHit = SB_LINEUP;
  if(nTestPos < nBottom)
  {
    return uHit;
  }
  if(si.nTrackPos == -1)
  {
    si.nTrackPos = si.nPos;
  }
  uHit = SB_PAGEUP;
  if((si.nMax - si.nMin - si.nPage) == 0)
  {
    nSegLen = nEmptyHeight / 2;
  }
  else
  {
    nSegLen = nEmptyHeight * (si.nTrackPos - si.nMin) / (si.nMax - si.nMin - si.nPage);
  }


  nBottom += nSegLen;
  if(nTestPos < nBottom)
  {
    return uHit;
  }
  nSegLen  = nSliderHeight;
  nBottom += nSegLen;

  if (nBottom > m_nHeight - nArrowHeight)
  {
    nBottom = m_nHeight - nArrowHeight;
  }

  uHit = SB_THUMBTRACK;
  if(nTestPos < nBottom)
  {
    return uHit;
  }
  nBottom = m_nHeight-nArrowHeight;
  uHit = SB_PAGEDOWN;
  if(nTestPos < nBottom)
  {
    return uHit;
  }
  uHit = SB_LINEDOWN;
  return uHit;
}

LRESULT 
SkinScrollBar::OnDpiChangedBefore(WPARAM wParam,LPARAM lParam)
{
  if(lParam)
  {
    m_nWidth = ThemeColor::GetSkinScrollWidth(GetSafeHwnd(),(HMONITOR)lParam);
  }
  return 0;
}

// Adjusting the width of the scrollbar
// taking the size from the bitmap
void 
SkinScrollBar::SetBitmapSize(HMONITOR p_monitor /*= nullptr*/)
{
  // Set the width from the bitmap size
  m_nWidth = ThemeColor::GetSkinScrollWidth(GetSafeHwnd(),p_monitor);

  ASSERT(m_hWnd);
  CRect rc;
  GetWindowRect(&rc);
  GetParent()->ScreenToClient(&rc);
  if(IsVertical())
  {
    rc.right = rc.left + m_nWidth;
  }
  else
  {
    rc.bottom = rc.top + m_nWidth;
  }
  MoveWindow(&rc);
}

// Recording the length of the scrollbar after a resize
void 
SkinScrollBar::OnSize(UINT nType, int cx, int cy) 
{
  CScrollBar::OnSize(nType, cx, cy);
  
  m_nHeight = IsVertical() ? cy : cx;
}

// Painting the complete scrollbar
// Fully owner-drawn control in the theme color
void 
SkinScrollBar::OnPaint() 
{
  CPaintDC dc(this); // device context for painting
  HBITMAP hBitmap = ThemeColor::GetScrollbarBitmap(GetSafeHwnd());
  CDC memdc;
  memdc.CreateCompatibleDC(&dc);
  HGDIOBJ hOldBmp=::SelectObject(memdc,hBitmap);

  RECT rcSour={0,0,m_nWidth,m_nWidth};
  if(!IsVertical())
  {
    OffsetRect(&rcSour,m_nWidth * 4,0);
  }
  RECT rcDest;
  rcDest=GetRect(SB_LINEUP);
  if((rcDest.right  - rcDest.left != rcSour.right  - rcSour.left) || 
     (rcDest.bottom - rcDest.top  != rcSour.bottom - rcSour.top))
  {
    dc.StretchBlt(rcDest.left,rcDest.top,rcDest.right - rcDest.left,rcDest.bottom - rcDest.top,
                  &memdc,
                  rcSour.left,rcSour.top,rcSour.right - rcSour.left,rcSour.bottom - rcSour.top,
                  SRCCOPY);
  }
  else
  {
    dc.BitBlt(rcDest.left,rcDest.top,m_nWidth,m_nWidth,&memdc,rcSour.left,rcSour.top,SRCCOPY);
  }
  rcDest=GetRect(SB_LINEDOWN);
  OffsetRect(&rcSour,m_nWidth,0);
  if((rcDest.right  - rcDest.left != rcSour.right  - rcSour.left) || 
     (rcDest.bottom - rcDest.top  != rcSour.bottom - rcSour.top))
  {
    dc.StretchBlt(rcDest.left,rcDest.top,rcDest.right - rcDest.left,rcDest.bottom - rcDest.top,
                  &memdc,
                  rcSour.left,rcSour.top,rcSour.right - rcSour.left,rcSour.bottom - rcSour.top,
                  SRCCOPY);
  }
  else
  {
    dc.BitBlt(rcDest.left,rcDest.top,m_nWidth,m_nWidth,&memdc,rcSour.left,rcSour.top,SRCCOPY);
  }
  rcDest=GetRect(SB_THUMBTRACK);
  OffsetRect(&rcSour,m_nWidth,0);
  DrawThumb(&dc,&rcDest,&memdc,&rcSour);
  OffsetRect(&rcSour,m_nWidth,0);
  rcDest=GetRect(SB_PAGEUP);
  TileBlt(&dc,&rcDest,&memdc,&rcSour);
  rcDest=GetRect(SB_PAGEDOWN);
  TileBlt(&dc,&rcDest,&memdc,&rcSour);

  ::SelectObject(memdc,hOldBmp);
}


// Getting the rect from the bitmap
// This is where we copy our image from
RECT 
SkinScrollBar::GetImageRect(UINT uSBCode,int nState)
{
  int nIndex=0;
  switch(uSBCode)
  {
    case SB_LINEUP:     nIndex = 0; break;
    case SB_PAGEUP:     nIndex = 3; break;
    case SB_THUMBTRACK: nIndex = 2; break;
    case SB_PAGEDOWN:   nIndex = 3; break;
    case SB_LINEDOWN:   nIndex = 1; break;
  }
  if(!IsVertical())
  {
    nIndex += 4;
  }
  RECT rcRet= { m_nWidth * nIndex
               ,m_nWidth * nState
               ,m_nWidth * (nIndex+1)
               ,m_nWidth * (nState+1) };
  return rcRet;
}

// Getting the rectangle an base of the SB_* code
// This will be the destination of the bitmap we will be drawing
RECT 
SkinScrollBar::GetRect(UINT uSBCode)
{
  SCROLLINFO si = m_si;
  if (si.nTrackPos == -1)
  {
    si.nTrackPos = si.nPos;
  }
  int nInternalHeight = m_nHeight - 2 * m_nWidth;
  if (nInternalHeight < 0)
  {
    nInternalHeight = 0;
  }
  int	nSliderHeight = si.nPage * nInternalHeight / (si.nMax - si.nMin + 1);
  if (nSliderHeight < THUMB_MINSIZE)
  {
    nSliderHeight = THUMB_MINSIZE;
  }
  if (nInternalHeight < THUMB_MINSIZE)
  {
    nSliderHeight = 0;
  }
  if ((si.nMax - si.nMin - si.nPage + 1) == 0)
  {
    nSliderHeight = 0;
  }
  int nEmptyHeight = nInternalHeight - nSliderHeight;
  int nArrowHeight = m_nWidth;
  if (nInternalHeight == 0)
  {
    nArrowHeight = m_nHeight / 2;
  }
  RECT rcRet={0,0,m_nWidth,nArrowHeight};
  if (uSBCode == SB_LINEUP)
  {
    goto end;
  }
  rcRet.top = rcRet.bottom;
  if((si.nMax - si.nMin - si.nPage) == 0)
  {
    rcRet.bottom += nEmptyHeight / 2;
  }
  else
  {
    rcRet.bottom += nEmptyHeight * (si.nTrackPos - si.nMin) / (si.nMax - si.nMin - si.nPage);
  }


  if (uSBCode == SB_PAGEUP)
  {
    goto end;
  }
  rcRet.top      = rcRet.bottom;
  rcRet.bottom  += nSliderHeight;

  if (rcRet.bottom > m_nHeight - nArrowHeight)
  {
    int size = rcRet.bottom - rcRet.top;
    rcRet.bottom = m_nHeight - nArrowHeight;
    rcRet.top    = rcRet.bottom - size;
  }

  if (uSBCode == SB_THUMBTRACK)
  {
    goto end;
  }
  rcRet.top    = rcRet.bottom;
  rcRet.bottom = m_nHeight - nArrowHeight;
  if (uSBCode == SB_PAGEDOWN)
  {
    goto end;
  }
  rcRet.top    = rcRet.bottom;
  rcRet.bottom = m_nHeight;
  if (uSBCode == SB_LINEDOWN)
  {
    goto end;
  }

end:
  if(!IsVertical())
  {
    // Convert positions to horizontal bar sizes
    int t = rcRet.left;
    rcRet.left = rcRet.top;
    rcRet.top  = t;
    t = rcRet.right;
    rcRet.right  = rcRet.bottom;
    rcRet.bottom = t;
  }
  return rcRet;
}

// Copy a part of the bitmap to the destination,
// possibly stretching the image (as thumb-track can be longer than the source image)
void 
SkinScrollBar::TileBlt(CDC *pDestDC, RECT *pDestRect, CDC *pSourDC, RECT *pSourRect)
{
  int nSoureHeight = pSourRect->bottom-pSourRect->top;
  int nSourceWidth = pSourRect->right-pSourRect->left;

  int y=pDestRect->top;
  
  while(y<pDestRect->bottom)
  {
    int nHeight = nSoureHeight;
    if (y + nHeight > pDestRect->bottom)
    {
      nHeight = pDestRect->bottom - y;
    }
    int x=pDestRect->left;
    while(x<pDestRect->right)
    {
      int nWid = nSourceWidth;
      if (x + nWid > pDestRect->right)
      {
        nWid = pDestRect->right - x;
      }
      pDestDC->BitBlt(x,y,nWid,nHeight,pSourDC,pSourRect->left,pSourRect->top,SRCCOPY);
      x+=nWid;
    }
    y += nHeight;
  }
}

// Drawing the thumb-track
void 
SkinScrollBar::DrawThumb(CDC *pDestDC, RECT *pDestRect, CDC *pSourDC, RECT *pSourRect)
{
  if(IsRectEmpty(pDestRect)) return;
  RECT rcDestin = *pDestRect;
  RECT rcSource = *pSourRect;
  if(IsVertical())
  {
    ASSERT(pDestRect->bottom-pDestRect->top>=THUMB_MINSIZE);
    pDestDC->BitBlt(pDestRect->left,pDestRect->top,m_nWidth,THUMB_BORDER,pSourDC,pSourRect->left,pSourRect->top,SRCCOPY);
    pDestDC->BitBlt(pDestRect->left,pDestRect->bottom-THUMB_BORDER,m_nWidth,THUMB_BORDER,pSourDC,pSourRect->left,pSourRect->bottom-THUMB_BORDER,SRCCOPY);
    rcDestin.top    += THUMB_BORDER;
    rcDestin.bottom -= THUMB_BORDER;
    rcSource.top    += THUMB_BORDER;
    rcSource.bottom -= THUMB_BORDER;
    TileBlt(pDestDC,&rcDestin,pSourDC,&rcSource);
  }
  else
  {
    ASSERT(pDestRect->right-pDestRect->left>=THUMB_MINSIZE);
    pDestDC->BitBlt(pDestRect->left,pDestRect->top,THUMB_BORDER,m_nWidth,pSourDC,pSourRect->left,pSourRect->top,SRCCOPY);
    pDestDC->BitBlt(pDestRect->right-THUMB_BORDER,pDestRect->top,THUMB_BORDER,m_nWidth,pSourDC,pSourRect->right-THUMB_BORDER,pSourRect->top,SRCCOPY);
    rcDestin.left  += THUMB_BORDER;
    rcDestin.right -= THUMB_BORDER;
    rcSource.left  += THUMB_BORDER;
    rcSource.right -= THUMB_BORDER;
    TileBlt(pDestDC,&rcDestin,pSourDC,&rcSource);
  }
}

// Dragging the thumb-track
// Or just scrolling the contents
void 
SkinScrollBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
  SetCapture();
  UINT uHit = HitTest(point);
  if(uHit == SB_THUMBTRACK)
  {
    m_bDrag  = TRUE;
    m_ptDrag = point;
    m_si.nTrackPos = m_si.nPos;
    m_nDragPos     = m_si.nPos;
  }
  else
  {
    m_uClicked = uHit;
    GetParent()->SendMessage(IsVertical()?WM_VSCROLL:WM_HSCROLL,MAKELONG(m_uClicked,0),(LPARAM)m_hWnd);
    SetTimer(TIMERID_DELAY,TIME_DELAY,nullptr);
    m_bPause = FALSE;
    if (uHit == SB_LINEUP || uHit == SB_LINEDOWN)
    {
      DrawArrow(uHit, 2);
    }
  }
}

// What happens on a mouse move for a registered thumb-track movement
void 
SkinScrollBar::OnMouseMove(UINT nFlags, CPoint point) 
{
  if(!m_bTrace && nFlags!=-1)
  {
    m_bTrace        = TRUE;
    TRACKMOUSEEVENT tme;
    tme.cbSize      = sizeof(tme);
    tme.hwndTrack   = m_hWnd;
    tme.dwFlags     = TME_LEAVE;
    tme.dwHoverTime = 1;
    m_bTrace        = _TrackMouseEvent(&tme);
  }

  HBITMAP hBitmap = ThemeColor::GetScrollbarBitmap(GetSafeHwnd());

  if(m_bDrag)
  {
    int nInternalHeight = m_nHeight - 2 * m_nWidth;
    int	nSliderHeight   = m_si.nPage * nInternalHeight / (m_si.nMax - m_si.nMin + 1);
    if (nSliderHeight < THUMB_MINSIZE)
    {
      nSliderHeight = THUMB_MINSIZE;
    }
    if (nInternalHeight < THUMB_MINSIZE)
    {
      nSliderHeight = 0;
    }
    int nEmptyHei = nInternalHeight - nSliderHeight;
    int nDragLen  = IsVertical() ? point.y - m_ptDrag.y : point.x - m_ptDrag.x;
    int nSlide    = (nEmptyHei == 0) ? 0 : (nDragLen * (int)(m_si.nMax /*- m_si.nMin*/ - m_si.nPage + 1) / nEmptyHei);
    int nNewTrackPos = m_nDragPos+nSlide;
    if(nNewTrackPos < m_si.nMin)
    {
      nNewTrackPos = m_si.nMin;
    }
    else if(nNewTrackPos > (int)(m_si.nMax /*- m_si.nMin */ - m_si.nPage + 1))
    {
      nNewTrackPos = m_si.nMax /*- m_si.nMin */- m_si.nPage + 1;
    }
    if(nNewTrackPos != m_si.nTrackPos)
    {
      CDC *pDC=GetDC();
      CDC memdc;
      memdc.CreateCompatibleDC(pDC);
      HGDIOBJ hOldBmp  = SelectObject(memdc,hBitmap);
      RECT    rcThumb1 = GetRect(SB_THUMBTRACK);
      m_si.nTrackPos   = nNewTrackPos;
      RECT    rcThumb2 = GetRect(SB_THUMBTRACK);

      RECT rcSourSlide = GetImageRect(SB_PAGEUP,0);
      RECT rcSourThumb = GetImageRect(SB_THUMBTRACK,2);
      RECT rcOld;
      if(IsVertical())
      {
        rcOld.left  = 0;
        rcOld.right = m_nWidth;
        if(rcThumb2.bottom > rcThumb1.bottom)
        {
          rcOld.top    = rcThumb1.top;
          rcOld.bottom = rcThumb2.top;
        }
        else
        {
          rcOld.top    = rcThumb2.bottom;
          rcOld.bottom = rcThumb1.bottom;
        }
      }else
      {
        rcOld.top    = 0;
        rcOld.bottom = m_nWidth;
        if(rcThumb2.right > rcThumb1.right)
        {
          rcOld.left  = rcThumb1.left;
          rcOld.right = rcThumb2.left;
        }
        else
        {
          rcOld.left  = rcThumb2.right;
          rcOld.right = rcThumb1.right;
        }
      }
      TileBlt(pDC,&rcOld,&memdc,&rcSourSlide);
      DrawThumb(pDC,&rcThumb2,&memdc,&rcSourThumb);
      SelectObject(memdc,hOldBmp);
      ReleaseDC(pDC);
      
      GetParent()->SendMessage(IsVertical()?WM_VSCROLL:WM_HSCROLL,MAKELONG(SB_THUMBTRACK,m_si.nTrackPos),(LPARAM)m_hWnd);
    }
  }
  else if(m_uClicked != -1)
  {
    CRect rc=GetRect(m_uClicked);
    m_bPause=!rc.PtInRect(point);
    if(m_uClicked == SB_LINEUP || m_uClicked == SB_LINEDOWN)
    {
      DrawArrow(m_uClicked,m_bPause ? 0 : 2);
    }
  }
  else
  {
    UINT uHit = HitTest(point);
    if(uHit != m_uHtPrev)
    {
      if(m_uHtPrev != -1)
      {
        if(m_uHtPrev == SB_THUMBTRACK)
        {
          CDC *pDC = GetDC();
          CDC memdc;
          memdc.CreateCompatibleDC(pDC);
          HGDIOBJ hOldBmp = SelectObject(memdc,hBitmap);
          RECT rcDest = GetRect(SB_THUMBTRACK);
          RECT rcSour = GetImageRect(SB_THUMBTRACK,0);
          DrawThumb(pDC,&rcDest,&memdc,&rcSour);
          ReleaseDC(pDC);
          SelectObject(memdc,hOldBmp);
        }
        else if(m_uHtPrev == SB_LINEUP || m_uHtPrev == SB_LINEDOWN)
        {
          DrawArrow(m_uHtPrev,0);
        }
      }
      if(uHit != -1)
      {
        if(uHit == SB_THUMBTRACK)
        {
          CDC *pDC = GetDC();
          CDC memdc;
          memdc.CreateCompatibleDC(pDC);
          HGDIOBJ hOldBmp = SelectObject(memdc,hBitmap);
          RECT rcDest = GetRect(SB_THUMBTRACK);
          RECT rcSour = GetImageRect(SB_THUMBTRACK,1);
          DrawThumb(pDC,&rcDest,&memdc,&rcSour);
          ReleaseDC(pDC);
          SelectObject(memdc,hOldBmp);
        }
        else if(uHit == SB_LINEUP || uHit == SB_LINEDOWN)
        {
          DrawArrow(uHit,1);
        }
      }
      m_uHtPrev=uHit;
    }
  }
}

// What happens if we let go of the left button on a thumb-track
void 
SkinScrollBar::OnLButtonUp(UINT nFlags, CPoint point) 
{
  // Reset the capture if necessary
  ReleaseCapture();
  // Recapture by the skin, so we can close a popup window (ComboBox)
  SkinScrollWnd* parent = reinterpret_cast<SkinScrollWnd*>(GetParent());
  if(parent->m_doCapture)
  {
    parent->SkinSetCapture();
  }

  if(m_bDrag)
  {
    HBITMAP hBitmap = ThemeColor::GetScrollbarBitmap(GetSafeHwnd());
    m_bDrag = FALSE;
    GetParent()->SendMessage(IsVertical()?WM_VSCROLL:WM_HSCROLL,MAKELONG(SB_THUMBPOSITION,m_si.nTrackPos),(LPARAM)m_hWnd);
    CDC *pDC = GetDC();
    CDC memdc;
    memdc.CreateCompatibleDC(pDC);
    HGDIOBJ hOldBmp = SelectObject(memdc,hBitmap);
    if(m_si.nTrackPos != m_si.nPos)
    {
      RECT rcThumb = GetRect(SB_THUMBTRACK);
      RECT rcSour  = {m_nWidth*3,0,m_nWidth*4,m_nWidth};
      if(!IsVertical())
      {
        OffsetRect(&rcSour, m_nWidth * 4, 0);
      }
      TileBlt(pDC,&rcThumb,&memdc,&rcSour);
    }
    m_si.nTrackPos=-1;

    RECT rcThumb = GetRect(SB_THUMBTRACK);
    RECT rcSour  = {m_nWidth*2,0,m_nWidth*3,m_nWidth};
    if (PtInRect(&rcThumb, point))
    {
      OffsetRect(&rcSour, 0, m_nWidth);
    }
    if (!IsVertical())
    {
      OffsetRect(&rcSour, m_nWidth * 4, 0);
    }
    DrawThumb(pDC,&rcThumb,&memdc,&rcSour);
    SelectObject(memdc,hOldBmp);
    ReleaseDC(pDC);
  }
  else if(m_uClicked != -1)
  {
    if(m_bNotify)
    {
      KillTimer(TIMERID_NOTIFY);
      m_bNotify = FALSE;
    }
    else
    {
      KillTimer(TIMERID_DELAY);
    }
    if (m_uClicked == SB_LINEUP || m_uClicked == SB_LINEDOWN)
    {
      DrawArrow(m_uClicked, 0);
    }
    m_uClicked = (UINT) -1;
  }
}

void 
SkinScrollBar::OnTimer(UINT_PTR nIDEvent) 
{
  // TODO: Add your message handler code here and/or call default
  if(nIDEvent == (UINT_PTR)TIMERID_DELAY)
  {
    m_bNotify = TRUE;
    nIDEvent  = TIMERID_NOTIFY;
    KillTimer(TIMERID_DELAY);
    SetTimer(TIMERID_NOTIFY,TIME_INTER,nullptr);
  }
  if(nIDEvent == (UINT_PTR)TIMERID_NOTIFY && !m_bPause)
  {
    if(m_uClicked == -1 || m_uClicked == SB_THUMBTRACK)
    {
      return;
    }
    
    switch(m_uClicked)
    {
      case SB_LINEUP:
                        if(m_si.nPos == m_si.nMin)
                        {
                          KillTimer(TIMERID_NOTIFY);
                          break;
                        }
                        GetParent()->SendMessage(IsVertical()?WM_VSCROLL:WM_HSCROLL,MAKELONG(SB_LINEUP,0),(LPARAM)m_hWnd);
                        break;
      case SB_LINEDOWN:
                        if(m_si.nPos == m_si.nMax)
                        {
                          KillTimer(TIMERID_NOTIFY);
                          break;
                        }
                        GetParent()->SendMessage(IsVertical()?WM_VSCROLL:WM_HSCROLL,MAKELONG(SB_LINEDOWN,0),(LPARAM)m_hWnd);
                        break;
      case SB_PAGEUP:
      case SB_PAGEDOWN:
                        {
                          CPoint pt;
                          GetCursorPos(&pt);
                          ScreenToClient(&pt);
                          CRect rc = GetRect(SB_THUMBTRACK);
                          if(rc.PtInRect(pt))
                          {
                            KillTimer(TIMERID_NOTIFY);
                            break;
                          }
                          GetParent()->SendMessage(IsVertical()?WM_VSCROLL:WM_HSCROLL,MAKELONG(m_uClicked,0),(LPARAM)m_hWnd);
                        }
                        break;
      default:			    ASSERT(FALSE);
                        break;
    }
  }
}

// Drawing the arrow at the end of the track
void 
SkinScrollBar::DrawArrow(UINT uArrow, int nState)
{
  ASSERT(uArrow == SB_LINEUP || uArrow == SB_LINEDOWN);
  HBITMAP hBitmap = ThemeColor::GetScrollbarBitmap(GetSafeHwnd());
  CDC *pDC = GetDC();
  CDC memdc ;
  memdc.CreateCompatibleDC(pDC);
  HGDIOBJ hOldBmp = ::SelectObject(memdc,hBitmap);
  
  RECT rcDest = GetRect(uArrow);
  RECT rcSour = GetImageRect(uArrow,nState);
  if((rcDest.right  - rcDest.left != rcSour.right  - rcSour.left) || 
     (rcDest.bottom - rcDest.top  != rcSour.bottom - rcSour.top))
  {
    pDC->StretchBlt(rcDest.left,rcDest.top,rcDest.right - rcDest.left,rcDest.bottom - rcDest.top,
                    &memdc,
                    rcSour.left,rcSour.top,rcSour.right - rcSour.left,rcSour.bottom - rcSour.top,
                    SRCCOPY);
  }
  else
  {
    pDC->BitBlt(rcDest.left,rcDest.top,m_nWidth,m_nWidth,&memdc,rcSour.left,rcSour.top,SRCCOPY);
  }
  ReleaseDC(pDC);
  ::SelectObject(memdc,hOldBmp);
}

// The mouse leaves the scrollbar
LRESULT 
SkinScrollBar::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
  m_bTrace = FALSE;
  OnMouseMove((UINT)-1,CPoint(-1,-1));
  return 0;
}

// Sub classed hooked window procedure
// Getting or setting the scroll position info
LRESULT 
SkinScrollBar::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
  if(message == WM_LBUTTONDBLCLK) return 1;

  if(message == SBM_SETSCROLLINFO)
  {
    BOOL bRedraw=(BOOL)wParam;
    LPSCROLLINFO lpScrollInfo=(LPSCROLLINFO)lParam;
    if(lpScrollInfo->fMask & SIF_PAGE) m_si.nPage = lpScrollInfo->nPage;
    if(lpScrollInfo->fMask & SIF_POS)  m_si.nPos  = lpScrollInfo->nPos;
    if(lpScrollInfo->fMask & SIF_RANGE)
    {
      m_si.nMin = lpScrollInfo->nMin;
      m_si.nMax = lpScrollInfo->nMax;
    }
    if(bRedraw)
    {
      HBITMAP hBitmap = ThemeColor::GetScrollbarBitmap(GetSafeHwnd());
      CDC *pDC = GetDC();
      CDC memdc;
      memdc.CreateCompatibleDC(pDC);
      HGDIOBJ hOldBmp = ::SelectObject(memdc,hBitmap);

      RECT rcSource = GetImageRect(SB_PAGEUP);
      RECT rcDestin = GetRect(SB_PAGEUP);
      TileBlt(pDC,&rcDestin,&memdc,&rcSource);
      rcDestin = GetRect(SB_THUMBTRACK);
      rcSource = GetImageRect(SB_THUMBTRACK);
      DrawThumb(pDC,&rcDestin,&memdc,&rcSource);
      rcDestin = GetRect(SB_PAGEDOWN);
      rcSource = GetImageRect(SB_PAGEDOWN);
      TileBlt(pDC,&rcDestin,&memdc,&rcSource);
      ::SelectObject(memdc,hOldBmp);
      ReleaseDC(pDC);
    }
    return TRUE;
  }
  if(message == SBM_GETSCROLLINFO)
  {
    LPSCROLLINFO lpScrollInfo = (LPSCROLLINFO)lParam;
    int nMask = lpScrollInfo->fMask;
    if(nMask & SIF_PAGE)      lpScrollInfo->nPage     = m_si.nPage;
    if(nMask & SIF_POS)       lpScrollInfo->nPos      = m_si.nPos;
    if(nMask & SIF_TRACKPOS)  lpScrollInfo->nTrackPos = m_si.nTrackPos;
    if(nMask & SIF_RANGE)
    {
      lpScrollInfo->nMin = m_si.nMin;
      lpScrollInfo->nMax = m_si.nMax;
    }
    return TRUE;
  }
  return CScrollBar::WindowProc(message, wParam, lParam);
}

void
SkinScrollBar::OnSetFocus(CWnd* p_wnd)
{
  CScrollBar::OnSetFocus(p_wnd);
}
