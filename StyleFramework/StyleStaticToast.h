////////////////////////////////////////////////////////////////////////
//
// File: StyleStaticToast.h
// Function: Styling frame for a CTabCtrl
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
#pragma once

#define STATIC_BLACK  RGB(0,0,0)
#define STATIC_WHITE  RGB(255,255,255)

class StyleStaticToast : public CStatic
{
// Construction
public:
  StyleStaticToast();
  virtual ~StyleStaticToast();

	void SetLeftOffset(int p_offset);
	void SetExtraText1(CString p_text);
	void SetExtraText2(CString p_text);

	void SetTextColor(COLORREF crColor);	// This Function is to set the Color for the Text.
	void SetBkColor  (COLORREF crColor);	// This Function is to set the BackGround Color for the Text.

	// Generated message map functions
protected:
	int      GetPosition(int p_height);

	CBrush   m_brushBackground;   // Holds Brush Color for the Static Text
	COLORREF m_colorBackground;   // Holds the Background Color for the Text
	COLORREF m_colorText;					// Holds the Color for the Text
	CString  m_text1;
	CString  m_text2;
	int      m_leftOffset;

	afx_msg void   OnPaint();
	afx_msg void   OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
};
