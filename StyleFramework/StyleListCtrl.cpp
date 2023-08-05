////////////////////////////////////////////////////////////////////////
//
// File: StyleListCtrl.cpp
// Function: Styling frame for CMFCListCtrl object
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
#include "StyleListCtrl.h"
#include "StyleFonts.h"
#include "StyleColors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace ThemeColor;

//////////////////////////////////////////////////////////////////////////
//
// StyleHeaderCtrl
//
IMPLEMENT_DYNAMIC(StyleHeaderCtrl,CMFCHeaderCtrl);

StyleHeaderCtrl::StyleHeaderCtrl()
{
}

StyleHeaderCtrl::~StyleHeaderCtrl()
{
}

void 
StyleHeaderCtrl::OnDrawItem(CDC* pDC,int iItem,CRect rect,BOOL bIsPressed,BOOL bIsHighlighted)
{
  int bgColor = ThemeColor::GetColor(Colors::ColorButtonBackground);
  int txColor = ThemeColor::GetColor(Colors::ColorButtonText);
  if(bIsHighlighted)
  {
    bgColor = ThemeColor::GetColor(Colors::ColorControlHover);
    txColor = ThemeColor::GetColor(Colors::ColorControlTextHover);
  }
  else if(bIsPressed)
  {
    bgColor = ThemeColor::GetColor(Colors::ColorControlPressed);
    txColor = RGB(0,0,0);
  }

  pDC->FillSolidRect(rect,bgColor);
  // CMFCHeaderCtrl::OnDrawItem(pDC,iItem,rect,bIsPressed,bIsHighlighted);

  ASSERT_VALID(this);
  ASSERT_VALID(pDC);

  const int nTextMargin = 5;

  if(iItem < 0)
  {
    return;
  }

  int nSortVal = 0;
  if(m_mapColumnsStatus.Lookup(iItem,nSortVal) && nSortVal != 0)
  {
    // Draw sort arrow:
    CRect rectArrow = rect;
    rectArrow.DeflateRect(5,5);
    rectArrow.left = rectArrow.right - rectArrow.Height();

    if(bIsPressed)
    {
      rectArrow.right++;
      rectArrow.bottom++;
    }

    rect.right = rectArrow.left - 1;

    int dy2 = (int) (.134 * rectArrow.Width());
    rectArrow.DeflateRect(0,dy2);

    m_bAscending = nSortVal > 0;
    OnDrawSortArrow(pDC,rectArrow);
  }

  HD_ITEM hdItem;
  memset(&hdItem,0,sizeof(hdItem));
  hdItem.mask = HDI_FORMAT | HDI_BITMAP | HDI_TEXT | HDI_IMAGE;

  TCHAR szText[256];
  hdItem.pszText = szText;
  hdItem.cchTextMax = 255;

  if(!GetItem(iItem,&hdItem))
  {
    return;
  }

  // Draw bitmap and image:
  if((hdItem.fmt & HDF_IMAGE) && hdItem.iImage >= 0)
  {
    // The column has a image from imagelist:
    CImageList* pImageList = GetImageList();
    if(pImageList != NULL)
    {
      int cx = 0;
      int cy = 0;

      VERIFY(::ImageList_GetIconSize(*pImageList,&cx,&cy));

      CPoint pt = rect.TopLeft();
      pt.x++;
      pt.y = (rect.top + rect.bottom - cy) / 2;

      VERIFY(pImageList->Draw(pDC,hdItem.iImage,pt,ILD_NORMAL));

      rect.left += cx;
    }
  }

  if((hdItem.fmt & (HDF_BITMAP | HDF_BITMAP_ON_RIGHT)) && hdItem.hbm != NULL)
  {
    CBitmap* pBmp = CBitmap::FromHandle(hdItem.hbm);
    ASSERT_VALID(pBmp);

    BITMAP bmp;
    pBmp->GetBitmap(&bmp);

    CRect rectBitmap = rect;
    if(hdItem.fmt & HDF_BITMAP_ON_RIGHT)
    {
      rectBitmap.right--;
      rect.right = rectBitmap.left = rectBitmap.right - bmp.bmWidth;
    }
    else
    {
      rectBitmap.left++;
      rect.left = rectBitmap.right = rectBitmap.left + bmp.bmWidth;
    }

    rectBitmap.top += max(0,(rectBitmap.Height() - bmp.bmHeight) / 2);
    rectBitmap.bottom = rectBitmap.top + bmp.bmHeight;

    pDC->DrawState(rectBitmap.TopLeft(),rectBitmap.Size(),pBmp,DSS_NORMAL);
  }

  // Draw text:
  if((hdItem.fmt & HDF_STRING) && hdItem.pszText != NULL)
  {
    CRect rectLabel = rect;
    rectLabel.DeflateRect(nTextMargin,0);

    CString strLabel = hdItem.pszText;

    UINT uiTextFlags = DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX;
    if(hdItem.fmt & HDF_CENTER)
    {
      uiTextFlags |= DT_CENTER;
    }
    else if(hdItem.fmt & HDF_RIGHT)
    {
      uiTextFlags |= DT_RIGHT;
    }

    pDC->SetTextColor(txColor);
    pDC->DrawText(strLabel,rectLabel,uiTextFlags);
  }
}

void 
StyleHeaderCtrl::OnFillBackground(CDC* pDC)
{
  CRect client;
  GetClientRect(client);
  int bkcolor = RGB(0,0,0); // ThemeColor::GetColor(Colors::ColorButtonBackground);
  pDC->SetBkColor(bkcolor);
  pDC->FillSolidRect(client,bkcolor);
}

//////////////////////////////////////////////////////////////////////////
//
// StyleListCtrl
//
IMPLEMENT_DYNAMIC(StyleListCtrl,CMFCListCtrl);

StyleListCtrl::StyleListCtrl()
{
}

StyleListCtrl::~StyleListCtrl()
{
  ResetSkin();
  CWnd::OnNcDestroy();
}

BEGIN_MESSAGE_MAP(StyleListCtrl,CMFCListCtrl)
  ON_WM_CREATE()
  ON_WM_PAINT()
  ON_WM_SIZE()
  ON_WM_ERASEBKGND()
  ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

void
StyleListCtrl::PreSubclassWindow()
{
  CMFCListCtrl::PreSubclassWindow();
  ScaleControl(this);
  if(m_directInit)
  {
    InitSkin();
  }
}

CMFCHeaderCtrl& 
StyleListCtrl::GetHeaderCtrl() 
{ 
  return m_styleHeader; 
}

void
StyleListCtrl::SetDirectInit(bool p_init)
{
  m_directInit = p_init;
}

// Default string comparison sort
int 
StyleListCtrl::OnCompareItems(LPARAM lParam1,LPARAM lParam2,int iColumn)
{
  int lower = (int)(lParam1 < lParam2 ? lParam1 : lParam2);
  int upper = (int)(lParam1 < lParam2 ? lParam2 : lParam1);
  CString first  = GetItemText(lower,iColumn);
  CString second = GetItemText(upper,iColumn);

  return first.CompareNoCase(second);
}

COLORREF 
StyleListCtrl::OnGetCellTextColor(int p_row,int p_colum)
{
  return ThemeColor::GetColor(Colors::ColorEditText);
}

COLORREF 
StyleListCtrl::OnGetCellBkColor(int p_row,int p_colum)
{
  return ThemeColor::GetColor(Colors::ColorEditBkgnd);
}

/////////////////////////////////////////////////////////////////////////////
// StyleListCtrl message handlers

void
StyleListCtrl::InitSkin(int p_borderSize /*=1*/)
{
  SetFont(&STYLEFONTS.DialogTextFont);
  SkinScrollWnd* skin = SkinWndScroll(this,p_borderSize);
  skin->SetScrollbarBias(0);
}

void
StyleListCtrl::ResetSkin()
{
  UnskinWndScroll(this);
}

SkinScrollWnd* 
StyleListCtrl::GetSkin()
{
  return (SkinScrollWnd*)GetWindowLongPtr(GetSafeHwnd(), GWLP_USERDATA);
}

LRESULT 
StyleListCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(message == WM_VSCROLL || message == WM_HSCROLL)
	{
		WORD sbCode=LOWORD(wParam);
		if(sbCode == SB_THUMBTRACK || sbCode == SB_THUMBPOSITION)
		{
      CRect rcClient;
      SCROLLINFO siv = { 0 };
			siv.cbSize = sizeof(SCROLLINFO);
			siv.fMask  = SIF_ALL;
			SCROLLINFO sih = siv;
			int nPos = HIWORD(wParam);

      GetClientRect(&rcClient);
			GetScrollInfo(SB_VERT,&siv);
			GetScrollInfo(SB_HORZ,&sih);
			
      SIZE sizeAll;
      if (sih.nPage == 0)
      {
        sizeAll.cx = rcClient.right;
      }
      else
      {
        sizeAll.cx = rcClient.right*(sih.nMax + 1) / sih.nPage;
      }
      if (siv.nPage == 0)
      {
        sizeAll.cy = rcClient.bottom;
      }
      else
      {
        sizeAll.cy = rcClient.bottom*(siv.nMax + 1) / siv.nPage;
      }

			SIZE size = { 0,0 };
			if(WM_VSCROLL == message)
			{
				size.cx=sizeAll.cx*sih.nPos/(sih.nMax+1);
				size.cy=sizeAll.cy*(nPos-siv.nPos)/(siv.nMax+1);
			}
      else
			{
				size.cx=sizeAll.cx*(nPos-sih.nPos)/(sih.nMax+1);
				size.cy=sizeAll.cy*siv.nPos/(siv.nMax+1);
			}
			Scroll(size);
			return 1;
		}
	}
	return CMFCListCtrl::WindowProc(message, wParam, lParam);
}

void
StyleListCtrl::DrawFrame()
{
  COLORREF color = (this == GetFocus()) ? ThemeColor::GetColor(Colors::AccentColor1) : ThemeColor::GetColor(Colors::AccentColor2);
  SkinScrollWnd* skin = (SkinScrollWnd*)GetWindowLongPtr(m_hWnd,GWLP_USERDATA);
  if(skin)
  {
    skin->DrawFrame(color);
  }
}

BOOL
StyleListCtrl::OnEraseBkgnd(CDC* pDC)
{
  CRect client;
  GetClientRect(client);
  pDC->FillSolidRect(client,ThemeColor::GetColor(Colors::ColorCtrlBackground));
  return TRUE;
}

void
StyleListCtrl::OnPaint()
{
  if(!m_inPaint)
  {
    m_inPaint = true;
    CMFCListCtrl::OnPaint();
    DrawFrame();
    m_inPaint = false;
  }
}

void 
StyleListCtrl::OnSize(UINT nType,int cx,int cy)
{
  CMFCListCtrl::OnSize(nType,cx,cy);
  DrawFrame();
}

// Propagate the ShowWindow command
void
StyleListCtrl::OnShowWindow(BOOL bShow, UINT nStatus)
{
  SkinScrollWnd* skin = GetSkin();
  if (skin && nStatus == 0)
  {
    // nStatus == 0 means: Message sent is caused by an explict "ShowWindow" call
    skin->ShowWindow(bShow ? SW_SHOW : SW_HIDE);
  }
  else
  {
    CMFCListCtrl::OnShowWindow(bShow, nStatus);
  }
}
