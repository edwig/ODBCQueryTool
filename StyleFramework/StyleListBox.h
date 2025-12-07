////////////////////////////////////////////////////////////////////////
//
// File: StyleListBox.h
// Function: Styling frame for CListBox object
//           Also does a recalculate of the widest string
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
#include "SkinScrollWnd.h"
#include "StyleEdit.h"

#define LIST_MAGIC         0xDEBAC1   // Marker for ownerdraw lines
#define LISTBOX_ITEMHEIGTH 22         // Standard heigth of an item

/////////////////////////////////////////////////////////////////////////////
// CMulitLineListBox window
class ListBoxColorLine
{
public:
  int      m_magic;
  CString  m_text;
  COLORREF m_foreground;
  COLORREF m_background;
  
  ListBoxColorLine()
  {
    m_magic      = LIST_MAGIC;
    m_foreground = FRAME_DEFAULT_COLOR;
    m_background = FRAME_DEFAULT_COLOR;
  }
};

class StyleListBox : public CListBox
{
  DECLARE_DYNAMIC(StyleListBox)

public:
  StyleListBox();
  virtual ~StyleListBox();
  
  // Styling functions
  void InitSkin(int p_borderSize = 1,int p_clientBias = 0);
  void ResetSkin();
  void SetDirectInit(bool p_init);
  void SetFontSize(int p_size);
  void SetFontStyle(bool p_bold,bool p_italic = false,bool p_underLine = false);
  void SetFontName(CString p_fontName,int p_fontSize = 100,BYTE p_language = DEFAULT_CHARSET);

  void DrawFrame();
  SkinScrollWnd* GetSkin();

  // Overrides of CListBox functions
  int  AddString(LPCTSTR p_string);
  int  InsertString(int p_index,LPCTSTR p_string,COLORREF p_foreground = FRAME_DEFAULT_COLOR,COLORREF p_background = FRAME_DEFAULT_COLOR);
  int  AppendString(LPCTSTR p_text,COLORREF p_foreground = FRAME_DEFAULT_COLOR,COLORREF p_background = FRAME_DEFAULT_COLOR);
  void ResetContent();
  int  DeleteString(int p_number);
  void MoveWindow(int x,int y,int nWidth,int nHeight,BOOL bRepaint = TRUE);
  int  GetText(int nIndex,LPTSTR lpszBuffer) const;
  void GetText(int nIndex,CString& rString) const;
  int  GetTextLen(int nIndex) const;

  // Edit functions
  void SelectAll(BOOL p_select = TRUE);
  void Copy();
  void Cut();
  void Paste();
  bool Print         (CString p_documentName,CString p_printer = _T(""),CString p_pageName = _T(""),CString p_dateName = _T(""),CString p_endlisting = _T(""));
  bool PrintSelection(CString p_documentName,CString p_printer = _T(""),CString p_pageName = _T(""),CString p_dateName = _T(""),CString p_endlisting = _T(""));

  DECLARE_MESSAGE_MAP()

protected:
  virtual void PreSubclassWindow() override;
  virtual void MeasureItem(LPMEASUREITEMSTRUCT p_measureItemStruct) override;
  virtual void DrawItem   (LPDRAWITEMSTRUCT    p_drawItemStruct)    override;
  virtual int  CompareItem(LPCOMPAREITEMSTRUCT p_compareItemStruct) override;
  virtual void DeleteItem (LPDELETEITEMSTRUCT  p_deleteItemStruct)  override;

  void    UpdateWidth(LPCTSTR p_string);
  void    AdjustScroll();
  void    AdjustHorizontalExtent();
  void    RemoveLineNumber(CString& p_text);
  void    RemoveLineInfo();
  void    SetItemPointer(int p_index,void* p_data);
  void    ResetFont(HMONITOR p_monitor = nullptr);

  // Owner painting inside OnPaint
  void    Internal_Paint(CDC* p_cdc);
  void    Internal_PaintItem(CDC* p_cdc,const RECT* rect,INT index,UINT action,BOOL ignoreFocus);

  afx_msg void    OnPaint();
  afx_msg void    OnShowWindow(BOOL bShow, UINT nStatus);
  afx_msg void    OnDestroy();
  afx_msg BOOL    OnEraseBkgnd(CDC* pDC);
  afx_msg void    OnHScroll(UINT nSBCode,UINT nPos,CScrollBar* pScrollBar);
  afx_msg void    OnVScroll(UINT nSBCode,UINT nPos,CScrollBar* pScrollBar);

  afx_msg LRESULT OnDpiChanged(WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnItemHeight(WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnItemRect  (WPARAM wParam,LPARAM lParam);

  SkinScrollWnd* m_skin;
  int  m_width      { 0     };
  bool m_inPaint    { false };
  bool m_directInit { true  };
  // Font
  int      m_fontSize     { STYLE_DEFAULT_FONTSIZE };   // Font size (pica's times 10)
  bool     m_italic       { false };                    // Use italic font
  bool     m_bold         { false };                    // Use bold font
  bool     m_underLine    { false };                    // Use underlined font
  CString  m_fontName     { STYLE_DEFAULT_FONT };       // Name of font to use
  CFont    m_font;                                      // Pointer to font structure
  BYTE     m_language     { DEFAULT_CHARSET };          // Default language
};
