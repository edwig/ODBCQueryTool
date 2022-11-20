////////////////////////////////////////////////////////////////////////
//
// File: StyleProgressCtrl.h
// Function: Styled progress control
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

// Implement marquee also for non-Unicode programs
#define PBST_MARQUEE 0x004
#define MARQUEE_PART     4

// Timers
#define TIMER_MARQUEE   0x01
#define TIMER_CREATED   0x02

#define NO_COLOR -1

class StyleProgressCtrl : public CWnd
{
  DECLARE_DYNAMIC(StyleProgressCtrl)

  // Constructors/Destructors
public:
  StyleProgressCtrl();
  virtual ~StyleProgressCtrl();

  // Generic creator
  virtual BOOL Create(_In_ DWORD dwStyle
                     ,_In_ const RECT& rect
                     ,_In_ CWnd* pParentWnd
                     ,_In_ UINT nID);

  // Generic creator allowing extended style bits
  virtual BOOL CreateEx(_In_ DWORD dwExStyle
                       ,_In_ DWORD dwStyle
                       ,_In_ const RECT& rect
                       ,_In_ CWnd* pParentWnd
                       ,_In_ UINT  nID);

  // ATTRIBUTES

  // Sets range of values for the control. (16-bit limit)
  void SetRange(_In_ short nLower, _In_ short nUpper);

  // Sets range of values for the control. (32-bit limit)
  void SetRange32(_In_ int nLower, _In_ int nUpper);

  // Retrieves range of values for the control. (32-bit limit)
  void GetRange(_Out_ int& nLower, _Out_ int& nUpper) const;

  // Gets the current position within the set range of the control.
  int GetPos() const;

  // Sets the current position within the set range of the control.
  int SetPos(_In_ int nPos);

  // Displaces the current position within the set range of the
  // control by the passed value.
  int OffsetPos(_In_ int nPos);

  // Sets the step by which increments happen with a call to StepIt().
  int SetStep(_In_ int nStep);

  // Sets the control's background color.
  COLORREF SetBkColor(_In_ COLORREF clrNew);

  // Sets the color of the progress indicator bar in the progress bar control.
  COLORREF SetBarColor(_In_ COLORREF clrBar);

  // Sets the progress bar control to marquee mode.
  BOOL SetMarquee(_In_ BOOL fMarqueeMode, _In_ int nInterval);

  // Retrieves the step increment for the progress bar control.
  int GetStep() const;

  // Retrieves the background color of the progress bar control.
  COLORREF GetBkColor() const;

  // Retrieves the color of the progress bar control.
  COLORREF GetBarColor() const;

  // Sets the state of the progress bar.
  int SetState(_In_ int iState);

  // Retrieves the state of the progress bar.
  int GetState() const;

  // Retrieves whether we should show the progress
  bool GetShowPercentage() const;

  // Sets the showing of the progress in percentage
  bool SetShowPercentage(bool p_show);

  // OPERATIONS:
  // THIS IS WHAT WE DO!

  // Steps the control by the value set with SetStep().
  int StepIt();

protected:
  DECLARE_MESSAGE_MAP()

  afx_msg void   OnTimer(UINT_PTR pID);
  afx_msg void   OnPaint();
  afx_msg void   OnNcPaint();

private:
  virtual void PreSubclassWindow() override;

  void BoundsCheck();
  void DrawFrame(CDC* p_dc);
  void DrawMarquee(CDC* p_dc,CRect& p_rect,bool p_vertical);
  void OnDrawProgress();

  int  m_lower    {   0 };          // Lower bound of the indicated range
  int  m_upper    { 100 };          // Upper bound of the indicated range
  int  m_step     {   1 };          // Step size per step of "StepIt"
  int  m_position {   0 };          // Current position between lower and upper
  int  m_state    { PBST_NORMAL };  // State we are in
  bool m_showperc { true };        // Show percentage of progress
  COLORREF m_background = (DWORD) NO_COLOR; // If no color, use ThemeColor::GetColor(Colors::WindowFrameBackground)
  COLORREF m_barcolor   = (DWORD) NO_COLOR; // If no color, use ThemeColor::GetColor(Colors::AccentColor1)
};

