// HyperLink.h : header file
//
// HyperLink static control. Will open the default browser with the given URL
// when the user clicks on the link.
//
// Copyright Chris Maunder, 1997-1999 (cmaunder@mail.com)
// Feel free to use and distribute. May not be sold for profit. 
//
// 2/29/00 -- P. Shaffer standard font mod.
// 20/2/21 -- E. Huisman Default hand cursor
//
#pragma once

class StyleHyperLink : public CStatic
{
public:
  StyleHyperLink();
  virtual ~StyleHyperLink();

  enum UnderLineOptions 
  { 
    ulHover  = -1
   ,ulNone   = 0
   ,ulAlways = 1
  };

// Attributes
public:
	void      SetTipText(LPCTSTR p_tipText);
  void      SetURL(CString strURL);
  CString   GetURL() const;
  void      SetColours(COLORREF crLinkColour
                      ,COLORREF crVisitedColour
                      ,COLORREF crHoverColour = -1);
  COLORREF  GetLinkColour() const;
  COLORREF  GetVisitedColour() const;
  COLORREF  GetHoverColour() const;

  void      SetVisited(BOOL bVisited = TRUE);
  BOOL      GetVisited() const;

  void      SetLinkCursor(HCURSOR hCursor);
  HCURSOR   GetLinkCursor() const;

  void      SetUnderline(int nUnderline = ulHover);
  int       GetUnderline() const;

  void      SetAutoSize(BOOL bAutoSize = TRUE);
  BOOL      GetAutoSize() const;

public:
  virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
protected:
  virtual void PreSubclassWindow();

  // Implementation
protected:
  void      SetFontsAndSize();
  HINSTANCE GotoURL(LPCTSTR url, int showcmd);
  void      ReportError(int nError);
  LONG      GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata);
  void      PositionWindow();
  void      SetDefaultCursor();

// Protected attributes
protected:
  COLORREF  m_crLinkColour;                        // Hyperlink colors
  COLORREF  m_crVisitedColour;                     // Hyperlink has been visited
  COLORREF  m_crHoverColour;                       // Hover color
  BOOL      m_bOverControl;                        // cursor over control?
  BOOL      m_bVisited;                            // Has it been visited?
  int       m_nUnderline;                          // underline hyperlink?
  BOOL      m_bAdjustToFit;                        // Adjust window size to fit text?
  CString   m_strURL;                              // hyperlink URL
  CFont     m_UnderlineFont;                       // Font for underline display
  CFont     m_StdFont;                             // Standard font
  HCURSOR   m_hLinkCursor;                         // Cursor for hyperlink
  CToolTipCtrl m_ToolTip;                          // The tooltip
  UINT      m_nTimerID;
  CBrush	  m_bkBrush;

  // Generated message map functions
public:
  afx_msg HBRUSH  CtlColor(CDC* pDC, UINT nCtlColor);
protected:
  afx_msg BOOL    OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
  afx_msg void    OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void    OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL    OnEraseBkgnd(CDC* pDC);
  afx_msg void    OnClicked();
  afx_msg LRESULT OnDpiChanged(WPARAM wParam,LPARAM lParam);

  DECLARE_MESSAGE_MAP()
};
