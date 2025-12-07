////////////////////////////////////////////////////////////////////////
//
// File: StyleMessageBox.h
// Function: Our message boxes
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
#include "StyleButton.h"
#include "StyleEdit.h"
#include "StyleCheckbox.h"
#include "resource.h"

// Windows message to press the "Do not show again" checkbox
#define ID_SUPPRESS (WM_USER + 1959)
// Message box buttons defined by strings
#define MB_STRINGS 0x0008L
// Maximum number of buttons and control labels
#define MAX_LABELS   5

// OUTSIDE WORLD INTERFACE

INT_PTR StyleMessageBox(CWnd* p_parent,LPCTSTR p_message,LPCTSTR p_title,long    p_styles = MB_OK,bool* p_doNotShowAgain = nullptr,bool p_foreground = false);
INT_PTR StyleMessageBox(CWnd* p_parent,LPCTSTR p_message,LPCTSTR p_title,LPCTSTR p_labels       , bool* p_doNotShowAgain = nullptr,bool p_foreground = false);


class MessageDialog : public StyleDialog
{
public:
  // Create by means of text label styles
  MessageDialog(CWnd*    p_parent
               ,LPCTSTR  p_title
               ,LPCTSTR  p_message
               ,LPCTSTR  p_labels
               ,bool*    p_doNotShowAgain = nullptr
               ,bool     p_foreground     = false);
  // Create by means of standard MB_* styles
  MessageDialog(CWnd*    p_parent
               ,LPCTSTR  p_title
               ,LPCTSTR  p_message
               ,int      p_styles
               ,bool*    p_doNotShowAgain = nullptr
               ,bool     p_foreground     = false);
  // Destructor
  ~MessageDialog();

  virtual void DoDataExchange(CDataExchange* pDX) override;

  // The main modal-loop
  virtual INT_PTR  DoModal() override;
  
  // Gives the result as a string ("ok","yes","no" etc)
  CString GetResult(int p_id);
  CString GetOriginalResult(int p_id);
  // Gives the result as an ID (IDOK, IDYES etc)
	INT_PTR GetResultID(INT_PTR p_id);
  // Standard positive answer
  CString GetStandardPositive();
  int     GetStandardPositiveID();
  // Standard negative answer
  CString GetStandardNegative();
  int     GetStandardNegativeID();
  // CHeck if this is the default button ID
  bool    GetDefault(UINT ID);
  // We push this button
  void    PushThisButton(UINT p_id,CPoint point);
  // Always show in the foreground
  void    SetForeground() { m_foreground = true; }

  enum { IDD = IDD_BOODSCHAP };
  
protected:
  DECLARE_MESSAGE_MAP();

  // Repaint the dialog
  afx_msg void OnPaint();
  // Clicked the 'Do not show again' checkbox
  afx_msg void OnDoNotShowAgain();
  // Default ESC-key cancel
  afx_msg void OnCancel();

private:
  // METHODEN

  // Initialization of the dialog
  virtual BOOL    OnInitDialog() override;
  // Override of the pre-translate of the MS-Windows message
  virtual BOOL    PreTranslateMessage(MSG* pMsg) override;

  // To do when a timer goes off
  void OnTimer(UINT_PTR nIDEvent);
  // Reset the buttons
  void ResetButtons();
  // Suppress this message, always ID_OK
  void SuppressFromNowOn();
  // Flash our application and message box
  void FlashMessageBox();
  // Go to the Next/Previous control on the message box
  void GotoControl(int p_direction);

  // Initialisation after CTOR
  void    InitButtons();
  CString InitCorrectLabels(CString p_labels);
  CString InitCorrectLabels(int p_styles);
  void    InitStyle(int p_styles);
  void    InitStyle(CString& p_labels);
  CString InitOriginalLabels(CString p_labels);
  void    InitCleanOriginalLabels();
  void    InitDefaultButton(int p_styles);
  void    InitForeground(int p_styles);
  CString InitForeground(CString p_labels);
  void    InitSplitLabelTextAndStyles(CString& p_labels);

  // Creating the MessageDialog
  void CreateTextArea(CRect& p_textRect);
  void CreateMakeTextArea(CRect& p_textRect);
  void CreateAdjustLongTitle(CRect& p_textRect);
  void CreateButtons(CRect& p_textRect);
  void CreateMakeTheButtons(CRect& p_textRect
                           ,CSize  p_tsize
                           ,int&   p_buttonTop
                           ,int&   p_buttonWidth
                           ,int&   p_buttonHeight
                           ,int&   p_buttonBegin
                           ,int&   p_totalWidth);
  void CreateDoNotRepeat(CRect& p_window,CSize p_tsize,int p_buttonBegin,int p_buttonTop,int p_buttonHeight);
  void CreateSetForeground();
  BOOL InitFocus();

  // DATA
  CFont*        m_font;                       // Font we use for the message
  StyleEdit     m_edit;                       // Edit box with the message
  CString       m_title;                      // Title of the dialog
  CString       m_message;                    // The message we want to show
  int           m_styles;                     // Signal styles
  int           m_default;                    // This button is the default button
  bool          m_def_done;                   // Default focus done
  bool          m_foreground;                 // Forcibly bring to the foreground
  LPCTSTR       m_image;                      // Signal this bitmap (as resource name)
  HICON         m_icon;                       // Signal icon image
  StyleButton*  m_button[MAX_LABELS];         // Max = 5 Buttons
  CString       m_label[MAX_LABELS];          // Max = 5 labels
  CString       m_originalLabel[MAX_LABELS];  // Max = 5 original labels
  int           m_labeltimers[MAX_LABELS];    // Max = 5 label timers
  int           m_labelcountdown[MAX_LABELS]; // Max = 5 label countdowns (seconds)
  CString       m_style[MAX_LABELS];          // Max = 5 styles
  int           m_width[MAX_LABELS];          // Max = 5 button widths

  // For the message register
  bool          m_suppressable;               // Message can be suppressed
  StyleCheckbox* m_suppress;                  // The checkbox for suppression
  CRect         m_line;                       // Line above the suppression
  bool          m_notAgain;                   // Do not show again
  bool*         m_notAgainPtr;                // Where we put the result of the suppression
};
