////////////////////////////////////////////////////////////////////////
//
// File: StyleEdit.h
// Function: Styling frame for a CEdit control
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
#include "StyleColors.h"
#include "StyleMacros.h"

/////////////////////////////////////////////////////////////////////////////
// CSG_TextEdit window

// Password eye: Number of DLU's the inner eye is within the eyebrow
#define   EDIT_INNER_EYE  WS(5)
#define   EDIT_EYE_WEIGHT WS(2)

// The password character
#define   EDIT_PASSWORD_CHAR '•'

// Default font name
#define   STYLE_DEFAULT_FONT     "Verdana"
#define   STYLE_DEFAULT_FONTSIZE 100 // 10.punts font
#define   STYLE_TEXTEDIT_BORDER  2   // Standard multiline text box
#define   STYLE_SINGLELN_BORDER  5   // Single-line MFC Wizard 

// Global functions to set your combobox style
void  SetStyleFxComboBoxDots(bool p_dots = true);
bool  GetStyleFxComboBoxDots();

class SkinScrollWnd;
class StyleComboBox;
class StyleSpinButtonCtrl;

class StyleEdit : public CEdit
{
  DECLARE_DYNAMIC(StyleEdit);
public:
  // CONSTRUCTION
  StyleEdit();
  virtual ~StyleEdit();

  virtual void PreSubclassWindow() override;
// 

  // OPERATIONS
  void InitSkin();
  void ResetSkin();
  BOOL EnableWindow(BOOL p_enable = TRUE);
  void DrawEditFrame();
  void DrawFrame();
  void SetWindowNumber(int p_number);
  void SetWindowText(LPTSTR lpszStringBuf);
  void SetWindowText(CString p_string1);
  void InsertAtCurPos(const char* p_text,int p_offset);
  int  CheckEditOK();
  void SetSpinBuddy(StyleSpinButtonCtrl* p_buddy);

  // SETTERS

  // Set your own border size (not for multiline edit controls!!)
  void SetBorderSize(int p_border);
  void SetDirectInit(bool p_init);
  void SetInitCorrectly() { m_initCorrectly = true; }
  void SetFocusDots(bool p_dots);
  // Set type of the text edit
  void SetMutable(bool p_mutable = true);
  void SetMandatory(bool p_mandatory = true);
  void SetErrorState(bool p_error,LPCTSTR p_tooltip = NULL);
  void SetNumeric(bool p_numeric = true);
  void SetPassword(bool p_password = true);
  void SetCalendar(bool p_calendar = true);
  void SetCheckDate(bool p_check = true);
  void SetComboBox(StyleComboBox* p_combo);
  void SetEmpty(bool p_empty, CString p_text = "");

  void SetTextColor           (COLORREF p_colorText);
  void SetBkColor             (COLORREF p_colorBackground);
  void SetBorderColor         (COLORREF p_color);
  void SetBorderColorDisabled (COLORREF p_color);
  void SetBorderColorFocus    (COLORREF p_color);
  void SetPasswordEyeColor    (COLORREF p_color);
  void SetTextColorEmpty      (COLORREF p_color);
  void SetBackgroundColorEmpty(COLORREF p_color);

  void SetFontSize(int p_size);
  void SetFontStyle(bool p_bold,bool p_italic = false,bool p_underLine = false);
  void SetFontName(CString p_fontName,int p_fontSize = 100,BYTE p_language = DEFAULT_CHARSET);
  void SetTooltip(CString p_tooltip);
  BOOL SetWindowPos(const CWnd* pWndInsertAfter,int x,int y,int cx,int cy,UINT nFlags);

  // GETTERS
  bool    GetErrorState();
  bool    GetIsEmpty();
  bool    GetMutable();
  bool    GetIsCalendar();
  bool    GetCheckDate();
  bool    GetIsPassword();
  bool    GetIsComboBox();
  bool    GetReadOnly();
  bool    GetHoverOver();
  CString GetTooltip();
  CString GetEmptyText();
  CString GetWindowString();
  int     GetWindowNumber();
  // Overrides on CEdit/CWnd
  int     GetWindowText(LPTSTR lpszStringBuf,int p_maxCount) const;
  void    GetWindowText(CString& p_string) const;
  int     GetWindowTextLength() const;
  // Skin getters
  SkinScrollWnd* GetSkin();
  void    OnComboBoxPaint();
  bool    GetFocusDots();
  void    GetDrawFrameColor(COLORREF& p_color,int& p_bordersize,bool& p_readonly);

protected:
  afx_msg void    OnMove(int x, int y);
  afx_msg void    OnSize(UINT nType, int cx, int cy);
  afx_msg void    OnShowWindow(BOOL bShow, UINT nStatus);
  afx_msg BOOL    OnKillFocus();
  afx_msg BOOL    OnSetfocus();
  afx_msg void    OnLButtonDown(UINT   nFlags,CPoint point);
  afx_msg void    OnLButtonUp  (UINT   nFlags,CPoint point);
  afx_msg LRESULT OnDoubleClick(WPARAM wParam,LPARAM lParam);
  afx_msg void    OnPaint();
  afx_msg HBRUSH  CtlColor(CDC* pDC, UINT nCtlColor);
  afx_msg void    OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void    OnWindowPosChanged(WINDOWPOS* lpwndpos);
public:
  afx_msg void    OnMouseMove (UINT   nFlags,CPoint point);
  afx_msg LRESULT OnMouseHover(WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnMouseLeave(WPARAM wParam,LPARAM lParam);
  
	DECLARE_MESSAGE_MAP()

protected:
  StyleComboBox*        m_combo { nullptr };
  StyleSpinButtonCtrl*  m_buddy { nullptr };
  BOOL                  m_over  { FALSE   };
  BOOL                  m_focus { FALSE   };

private:
  void     ResetFont();
  void     CreateBackgroundBrush(DWORD p_color);
  void     CreateBackgroundEmptyBrush(DWORD p_color);
  void     DrawPasswordEye();
  void     DrawErrorExclamation();

  CString  m_tooltip;     // Tooltip on the contents
  CString  m_emptyText;   // Background text in case the field is empty: hint what to fill in
  int      m_startChar;
  int      m_endChar;

  // Flags
  bool     m_initCorrectly{ false };      // Correctly initialized
  bool     m_directInit   { true  };      // Directly initialized in PreSubclassWindow
  bool     m_mandatory    { false };      // Mandatory edit field. Must be filled
  bool     m_error        { false };      // Field currently in error state (red-border)
  bool     m_numeric      { false };      // Contents is a NUMBER
  bool     m_password     { false };      // Contents is a password (show eye)
  bool     m_calendar     { false };      // Contents is a date (use calendar)
  bool     m_checkDate    { false };      // Check valid date in a calendar field
  bool     m_empty        { false };      // Currently empty, show emptyText
  bool     m_mutable      { true  };      // Logically mutable (for a combobox)
  bool     m_focusDots    { false };      // Focus dots in a combobox
  // Font
  int      m_fontSize     { STYLE_DEFAULT_FONTSIZE };   // Font size (pica's times 10)
  bool     m_italic       { false };                    // Use italic font
  bool     m_bold         { false };                    // Use bold font
  bool     m_underLine    { false };                    // Use underlined font
  CString  m_fontName     { STYLE_DEFAULT_FONT };       // Name of font to use
  CFont*   m_font         { nullptr };                  // Pointer to font structure
  BYTE     m_language     { DEFAULT_CHARSET };          // Default language
  // Colors
  COLORREF m_colorText            { FRAME_DEFAULT_COLOR };
  COLORREF m_colorBackground      { FRAME_DEFAULT_COLOR };
  COLORREF m_colorTextEmpty       { RGB(0x8F,0x8F,0x8F) };  // Light gray
  COLORREF m_colorBackgroundEmpty { RGB(0xFF,0xFF,0xFF) };
  COLORREF m_colorPasswordEye     { FRAME_DEFAULT_COLOR };
  CBrush   m_bkBrush;
  CBrush   m_bkEmptyBrush;

  int      m_borderSize           { -1 };  // Different border size
  COLORREF m_colorBorder          {  0 };  // Normal border color outside
  COLORREF m_colorBorderDisabled  {  0 };  // Disabled border color outside
  COLORREF m_colorBorderFocus     {  0 };  // Border color outside if mouse-over or focus 
};

inline bool 
StyleEdit::GetErrorState()
{
  return m_error;
}

inline void 
StyleEdit::SetTooltip(CString p_tooltip)
{
  m_tooltip = p_tooltip;
}

inline CString
StyleEdit::GetTooltip()
{
  return m_tooltip;
}

inline bool
StyleEdit::GetIsEmpty()
{
  return m_empty;
}

inline bool
StyleEdit::GetMutable()
{
  return m_mutable;
}

inline CString
StyleEdit::GetEmptyText()
{
  return m_emptyText;
}

inline void 
StyleEdit::SetCalendar(bool p_calendar /*=true*/)
{
  m_calendar = p_calendar;
}

inline void 
StyleEdit::SetCheckDate(bool p_check /*=true*/)
{
  m_checkDate = p_check;
}

inline bool
StyleEdit::GetIsCalendar()
{
  return m_calendar;
}

inline bool
StyleEdit::GetCheckDate()
{
  m_checkDate;
}

inline bool
StyleEdit::GetIsPassword()
{
  return m_password;
}

inline void 
StyleEdit::SetComboBox(StyleComboBox* p_combo)
{
  m_combo = p_combo;
}

inline bool
StyleEdit::GetIsComboBox()
{
  return m_combo != nullptr;
}

// SUPPORT FOR DynamicDataEXchange in Dialogs

void WINAPI DDX_Control(CDataExchange* pDX,int nIDC,StyleEdit& p_editControl);
void WINAPI DDX_Control(CDataExchange* pDX,int nIDC,StyleEdit& p_editControl,CString& p_text);
void WINAPI DDX_Control(CDataExchange* pDX,int nIDC,StyleEdit& p_editControl,int& p_number);
void WINAPI DDX_Control(CDataExchange* pDX,int nIDC,StyleEdit& p_editControl,double& p_number);

// SUPPORT FOR DynamicDataValidation in Dialogs

void AFXAPI DDV_MinMaxShort   (CDataExchange* pDX,StyleEdit& p_control,short    value,short    minVal,short    maxVal);
void AFXAPI DDV_MinMaxInt     (CDataExchange* pDX,StyleEdit& p_control,int      value,int      minVal,int      maxVal);
void AFXAPI DDV_MinMaxLong    (CDataExchange* pDX,StyleEdit& p_control,long     value,long     minVal,long     maxVal);
void AFXAPI DDV_MinMaxUint    (CDataExchange* pDX,StyleEdit& p_control,UINT     value,UINT     minVal,UINT     maxVal);
void AFXAPI DDV_MinMaxLongLong(CDataExchange* pDX,StyleEdit& p_control,LONGLONG value,LONGLONG minVal,LONGLONG maxVal);
void AFXAPI DDV_MinMaxDouble  (CDataExchange* pDX,StyleEdit& p_control,double   value,double   minVal,double   maxVal);