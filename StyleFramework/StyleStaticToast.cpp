////////////////////////////////////////////////////////////////////////
//
// File: StyleStaticToast.cpp
// Function: Styling frame for a static control
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
#include "StyleStaticToast.h"
#include "StyleFonts.h"
#include "StyleColors.h"
#include "StyleMacros.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// StyleStaticToast

StyleStaticToast::StyleStaticToast()
{
  m_leftOffset      = 2 * STANDARDFONTSIZE;
  m_colorBackground = ::GetSysColor(COLOR_3DFACE); // Initializing the Background Color to the system face color.
  m_colorText       = STATIC_BLACK; // Initializing the text to Black
  m_brushBackground.CreateSolidBrush(m_colorBackground); // Create the Brush Color for the Background.
}

StyleStaticToast::~StyleStaticToast()
{
}

BEGIN_MESSAGE_MAP(StyleStaticToast, CStatic)
  ON_WM_PAINT()
  ON_WM_CTLCOLOR_REFLECT()
  ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

void
StyleStaticToast::PreSubclassWindow()
{
  ScaleControl(this);
}

/////////////////////////////////////////////////////////////////////////////
// StyleStaticToast message handlers

HBRUSH StyleStaticToast::CtlColor(CDC* pDC, UINT nCtlColor) 
{
  UNUSED_ALWAYS(nCtlColor);
  HBRUSH hbr;
  hbr = (HBRUSH)m_brushBackground;		// Passing a Handle to the Brush
  pDC->SetBkColor(m_colorBackground); // Setting the Color of the Text Background to the one passed by the Dialog
  pDC->SetTextColor(m_colorText);			// Setting the Text Color to the one Passed by the Dialog

	return hbr;
}

void 
StyleStaticToast::SetLeftOffset(int p_offset)
{
  m_leftOffset = WS(p_offset);
}

void 
StyleStaticToast::SetExtraText1(CString p_text)
{
  m_text1 = p_text;
}

void 
StyleStaticToast::SetExtraText2(CString p_text)
{
  m_text2 = p_text;
}

void 
StyleStaticToast::SetBkColor(COLORREF crColor)
{
  m_colorBackground = crColor;                  // Passing the value passed by the dialog to the member variable for Background Color
  m_brushBackground.DeleteObject();             // Deleting any Previous Brush Colors if any existed.
  m_brushBackground.CreateSolidBrush(crColor);  // Creating the Brush Color For the Static Text Background
  RedrawWindow();
}

void 
StyleStaticToast::SetTextColor(COLORREF crColor)
{
  m_colorText = crColor; // Passing the value passed by the dialog to the member variable for Text Color
  RedrawWindow();
}

int
StyleStaticToast::GetPosition(int p_height)
{
  int lines = 1;
  if(m_text1.GetLength()) ++lines;
  if(m_text2.GetLength()) ++lines;
  int space = p_height / (lines + 1);

  // Middle minus half a textline
  return space - (WS(20) / 2);
}

void
StyleStaticToast::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd* owner = GetOwner();
	if(owner)
	{
    owner->OnCmdMsg(GetDlgCtrlID(),STN_CLICKED,NULL,NULL);
	}
}

void
StyleStaticToast::OnPaint()
{
	// Do the default processing
	CStatic::Default();

	// Get the DC and the rectangle
	CDC* pDC = GetDC();
	CRect rect;
	GetClientRect(&rect);

	// Get the text1
	CString text;
	GetWindowText(text);

	// Paint the background
	pDC->FillSolidRect(&rect, m_colorBackground);

	// Paint the text
	int fontheight = 2 * STANDARDFONTSIZE;
	CFont* org = pDC->SelectObject(&STYLEFONTS.DialogTextFontBold);
	pDC->SetTextColor(m_colorText);
	rect.left += m_leftOffset;
	rect.top  += GetPosition(rect.Height());
	rect.bottom = rect.top + fontheight;

	pDC->DrawText(text,&rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);

	if(m_text1.GetLength())
	{
		pDC->SelectObject(&STYLEFONTS.DialogTextFont);
		rect.top    += fontheight;
		rect.bottom += fontheight;
    pDC->DrawText(m_text1,&rect,DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);

		if(m_text2.GetLength())
		{
			rect.top    += fontheight;
			rect.bottom += fontheight;
			pDC->DrawText(m_text2,&rect,DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
		}
	}

	pDC->SelectObject(org);
	ReleaseDC(pDC);
}
