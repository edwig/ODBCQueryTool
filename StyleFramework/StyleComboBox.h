////////////////////////////////////////////////////////////////////////
//
// File: StyleComboBox.h
// Function: ComboBox with colored scrollbars in our style colors
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
#include "StyleListBox.h"
#include "StyleEdit.h"
#include <vector>
#include <map>

#define COMBO_DEFAULT_MINVISIBLE   20  // Number of items
#define COMBO_DEFAULT_DROPWIDTH   100  // Number of pixels
#define COMBO_DEFAULT_RESERVED   1000  // Number of items allocated
#define COMBO_KEYBOARD_CACHE     2000  // 2000 ms keyboard cache

class SCBTextEdit;
class SCBListBox;

// Storage for Set/Get ItemData
using ItemStore = std::map<int,DWORD_PTR>;

class StyleComboBox : public CComboBox
{
  DECLARE_DYNAMIC(StyleComboBox)

public:
  StyleComboBox();
  virtual ~StyleComboBox() override;

  void  InitSkin();

  void  SetAutoComplete(bool p_auto = true);
  bool  GetAutoComplete();
  void  SetMultiSelect(bool p_multi);
  bool  GetMultiSelect();
  bool  GetTypeBuffer();
  void  SetTypeBuffer(bool p_buffer = true);
  void  SetMultiSelection(int p_index,bool p_select);
  bool  GetMultiSelection(int p_index);
  void  SetErrorState(bool p_error,bool p_propagate = true);
  bool  GetErrorState();
  void  SetFocusDots(bool p_dots);
  bool  GetFocusDots();
  void  InsertAtCurPos(const char* p_text, int p_offset);

  // Standard CWnd methods
  void  GetWindowText(CString& p_text);
  void  SetWindowText(CString  p_text);
  BOOL  EnableWindow(BOOL p_enable = TRUE);
  BOOL  IsWindowEnabled();

  // Implementation of 'normal' ComboBox methods
  int   GetCount() const;
  int   GetCurSel() const;
  int   SetCurSel(int nSelect);
  LCID  GetLocale() const;
  LCID  SetLocale(LCID nNewLocale);
  int   GetTopIndex() const;
  int   SetTopIndex(int nIndex);
  int   InitStorage(int nItems,UINT nBytes);
  void  SetHorizontalExtent(UINT nExtent);
  UINT  GetHorizontalExtent() const;
  int   SetDroppedWidth(UINT nWidth);
  int   GetDroppedWidth() const;
  int   SetMaxDroppedWidth(UINT p_percentage);

  BOOL  GetComboBoxInfo(PCOMBOBOXINFO pcbi) const;
  BOOL  SetMinVisibleItems(int iMinVisible);
  int   GetMinVisible() const;

  DWORD GetEditSel() const;
  BOOL  LimitText(int nMaxChars);
  BOOL  SetEditSel(int nStartChar,int nEndChar);

  BOOL  SetCueBanner(LPCSTR lpszText,BOOL fDrawIfFocused = FALSE);
  BOOL  GetCueBanner(LPSTR  lpszText,int cchText) const;

  // for list item
  DWORD_PTR GetItemData(int nIndex) const;
  int       SetItemData(int nIndex,DWORD_PTR dwItemData);
  void*     GetItemDataPtr(int nIndex) const;
  int       SetItemDataPtr(int nIndex,void* pData);
  int       GetLBText(int nIndex,LPTSTR lpszText) const;
  void      GetLBText(int nIndex,CString& rString) const;
  int       GetLBTextLen(int nIndex) const;
  void      SelectAll(bool p_check);
  bool      GetCheck(int p_index);
  void      SetCheck(int p_index,bool p_check);

  int   SetItemHeight(int nIndex,UINT cyItemHeight);
  int   GetItemHeight(int nIndex) const;
  int   FindStringExact(int nIndexStart,LPCTSTR lpszFind) const;
  int   SetExtendedUI(BOOL bExtended = TRUE);
  BOOL  GetExtendedUI() const;
  BOOL  GetDroppedControlRect(LPRECT lprect) const;
  BOOL  GetDroppedState() const;
  bool  GetHasFocus() const;

  // for drop-down combo boxes
  void  ShowDropDown(BOOL bShowIt = TRUE);

  // manipulating listbox items
  int   AddString(LPCTSTR lpszString);
  int   DeleteString(UINT nIndex);
  int   InsertString(int nIndex,LPCTSTR lpszString);
  void  ResetContent();
  int   FindString(int nStartAfter,LPCTSTR lpszString) const;
  int   SelectString(int nStartAfter,LPCTSTR lpszString);

  // Clipboard operations
  void  Clear();
  void  Copy();
  void  Cut();
  void  Paste();

  SCBTextEdit*  GetEditControl()                { return m_itemControl;  }
  SCBListBox*   GetListControl()                { return m_listControl;  }
  COLORREF      GetFrameColor()                 { return m_frameColor;   }
  void          SetFrameColor(COLORREF p_color) { m_frameColor = p_color;}
  COLORREF      GetArrowColor()                 { return m_arrowColor;   }
  void          SetArrowColor(COLORREF p_color) { m_arrowColor = p_color;}

  virtual void PreShowComboList();
  virtual void PostShowComboList();
  virtual void PreHideComboList();
  virtual void PostHideComboList();

protected:
  // Allow for other (derived) controls to be created
  virtual void CreateEditControl();
  virtual void CreateListControl();

  virtual void ShowComboList();
  virtual void HideComboList();

  virtual void SetEditSelection();
  virtual int  FindOptimalWidth();
  virtual int  FindOptimalHeight();
  virtual void PositionDropList(int p_width,int p_height);
  virtual void FindSelectionFromEdit();

  SCBTextEdit* m_itemControl        { nullptr };                    // The real edit control
  SCBListBox*  m_listControl        { nullptr };                    // The list control
  int          m_minimum            { COMBO_DEFAULT_MINVISIBLE };   // Minimum visible items in the list
  int          m_minimalDropWidth   { COMBO_DEFAULT_DROPWIDTH  };   // Minimal allowable dropped width
  int          m_maxWidthPercentage {   100 };                      // Max width between 100 and 1000 percent
  bool         m_buttonDown         { false };                      // Button is pressed, List is shown
  bool         m_extendedUI         { true  };                      // F4/Down key opens the list
  bool         m_multiselect        { false };                      // Multi-selection combo box list
  bool         m_typebuffer         { false };                      // type buffer for combo list
  bool         m_focus              { false };                      // Has the keyboard focus
  bool         m_error              { false };                      // Control is in error state
  ItemStore    m_items;                                             // Set/GetDataItem storage
  COLORREF     m_frameColor         { RGB(0,0,0) };                 // Color of the outer rim of the combobox
  COLORREF     m_arrowColor         { RGB(0,0,0) };                 // Color of the combobox arrow

  virtual void PreSubclassWindow() override;

  DECLARE_MESSAGE_MAP()

  // COMBOBOX Messages
  afx_msg LRESULT OnAddString             (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnDeleteString          (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnFindString            (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnFindStringExact       (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnGetComboBoxInfo       (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnGetCount              (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnGetCueBanner          (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnGetCurSel             (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnGetDroppedControlRect (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnGetDroppedState       (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnGetDroppedWidth       (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnGetEditSel            (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnGetExtendedUI         (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnGetHorizontalExtent   (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnGetItemData           (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnGetItemHeight         (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnGetLBText             (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnGetLBTextLen          (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnGetLocale             (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnGetMinVisible         (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnGetTopIndex           (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnInitStorage           (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnInsertString          (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnLimitText             (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnResetContent          (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnSelectString          (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnSetCueBanner          (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnSetCurSel             (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnSetDroppedWidth       (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnSetEditSel            (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnSetExtendedUI         (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnSetHorizontalExtent   (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnSetItemData           (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnSetItemHeight         (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnSetLocale             (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnSetMinVisible         (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnSetTopIndex           (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnShowDropDown          (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnGetText               (WPARAM wParam,LPARAM lParam);
  afx_msg LRESULT OnGetTextLength         (WPARAM wParam,LPARAM lParam);

public:
  afx_msg void    OnCloseup(bool p_force = false);
  afx_msg void    OnDropdown();
  afx_msg void    OnSelEndOK(bool p_force = false);
  afx_msg void    OnSelEndCancel();
  afx_msg void    OnKillFocus();
  afx_msg void    OnLbnSelChange();
  afx_msg void    OnLbnDblClk();
  afx_msg void    OnEditChange();
  afx_msg void    OnEditUpdate();
  afx_msg void    OnMouseLeave();

  afx_msg int     OnSetText(LPCTSTR lpszText);
  afx_msg int     OnGetText(int nMaxChars,LPTSTR lpszText);
  afx_msg UINT    OnGetTextLength();
  afx_msg void    OnSetFocus(CWnd* pOldWnd);
  afx_msg void    OnLButtonDown  (UINT nFlags, CPoint point);
  afx_msg void    OnLButtonDblClk(UINT nFlags, CPoint point);
  afx_msg void    OnMouseMove    (UINT nFlags, CPoint point);
  afx_msg void    OnSize(UINT nType, int cx, int cy);
  afx_msg BOOL    OnEraseBkgnd(CDC* pdc);
  afx_msg void    OnPaint();
  afx_msg void    OnChar(UINT nChar,UINT nRepCnt,UINT nFlags);
};

// The S(tyle)C(ombo)B(ox)TextEdit control is the
// base edit control of the combo box
//
class SCBTextEdit : public StyleEdit
{
  DECLARE_DYNAMIC(SCBTextEdit);
public:
  SCBTextEdit();
  virtual ~SCBTextEdit();

  void SetAutoComplete(bool p_auto = true);
  bool GetAutoComplete();
  bool GetHasFocus() const;

  afx_msg void OnPaste();
  afx_msg void OnSetFocus(CWnd* p_prev);
  afx_msg void OnKillFocus(CWnd* p_new);
  afx_msg void OnLButtonUp  (UINT nFlags, CPoint point);
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnKeyDown(UINT nChar,UINT nRepCnt,UINT nFlags);
  afx_msg void OnKeyUp  (UINT nChar,UINT nRepCnt,UINT nFlags);
  afx_msg void OnChar   (UINT nChar,UINT nRepCnt,UINT nFlags);

  DECLARE_MESSAGE_MAP()

protected:
  virtual BOOL PreTranslateMessage(MSG* p_msg) override;
          void CheckAutoCompletion();

  CString     m_origText;       // Original text of the edit control
  bool        m_autoComplete;   // Use auto completion when typing
  bool        m_doCompletion;   // Auto completion still active
  bool        m_complete;       // Found a complete answer
  DWORD       m_keyboardTime;   // Last keyboard char received
};

typedef std::vector<bool> LBSelections;

// The S(tyle)C(ombo)B(ox)ListBox is the dropped down list
// of the styled combo box
//
class SCBListBox : public StyleListBox
{
  DECLARE_DYNAMIC(SCBListBox);
public:
  SCBListBox();
  virtual ~SCBListBox();

  int     AddString(LPCTSTR p_string);
  int     DeleteString(UINT nIndex);
  int     InsertString(int nIndex,LPCTSTR lpszString);
  void    SelectAll(BOOL p_select = TRUE);
  void    ResetContent();

  void    SetStyleComboBox(StyleComboBox* p_combo);
  void    SelectCurrent(CPoint point);
  bool    GetMultiSelect();
  void    SetMultiSelection(int p_index,bool p_select);
  bool    GetMultiSelection(int p_index);
  CString GetCurrentMultiSelect();
  void    SetCurrentMultiSelect(CString p_text);

  // Public, but only to be called from within StyleListBox
  void    SetActive();
  void    SaveCurSel();
  void    ResetCurSel();
  void    SetMultiSelect(bool p_multi);

  afx_msg void    OnLButtonDown(UINT nFlags,CPoint point);
  afx_msg void    OnLButtonUp  (UINT nFlags,CPoint point);
  afx_msg void    OnMouseMove  (UINT nFlags,CPoint point);
  afx_msg void    OnActivate(UINT nState,CWnd* pWndOther,BOOL pMinimized);
  afx_msg void    OnSetFocus(CWnd* pOldWnd);

  DECLARE_MESSAGE_MAP()

private:
  virtual BOOL PreTranslateMessage(MSG* pMsg) override;
  virtual void PreSubclassWindow() override;

  void HandleDoubleClick(CPoint point);
  void ShowMultiSelection();
  void SelectCurrentSelection();
  void CancelCurrentSelection();
  int  LastSelected();

  StyleComboBox*  m_combo;          // Our parent ComboBox
  bool            m_active;         // List now active and dropped-down
  bool            m_mouseActivated; // List was activated by a mouse press
  bool            m_multiSelect;    // It is a multi-select list
  int             m_current;        // Starting selection in a single-select list
  UINT            m_lastClick;      // Last LButtonDown time to simulate a double-click
  LBSelections    m_selected;       // Active selections in a multi-select list
  DWORD           m_keyboardTime;   // Last keyboard char received
  CString         m_searchText;     // Text searched in the combo list
};

// SUPPORT FOR DynamicDataEXchange in Dialogs
void AFXAPI DDX_Control (CDataExchange* pDX,int nIDC,StyleComboBox& p_control);
void WINAPI DDX_CBString(CDataExchange* pDX,int nIDC,StyleComboBox& p_control,CString& p_text);
void WINAPI DDX_CBIndex (CDataExchange* pDX,int nIDC,StyleComboBox& p_control,int& p_index);
