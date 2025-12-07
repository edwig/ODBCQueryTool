////////////////////////////////////////////////////////////////////////
//
// File: StyleStepper.h
// Function: Stepper like wizard to guide the user through a mutation
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
#include <vector>

#define STEPPER_TOP       160   // Pages begin at this y coordinate
#define STEPPER_MARGIN     30   // Margin around the steps
#define STEPPER_LINEWIDTH  10   // Line between steps
#define STEPPER_BOTTOM     60   // Space for the buttons
#define STEPPER_SPLIT       2   // Space between step and line

class StyleTab;

struct StepperTab
{
  CString   m_title;
  StyleTab* m_page;
  UINT      m_resource;
};

using StepperPages = std::vector<StepperTab>;

class StyleStepper : public StyleDialog
{
  DECLARE_DYNAMIC(StyleStepper);

public:
  StyleStepper(CWnd*   p_parentWnd  = nullptr
              ,UINT    p_IDTemplate = IDD_STEPPER
              ,CString p_caption    = _T("")
              ,bool    p_sysmenu    = false
              ,bool    p_status     = false);
  virtual ~StyleStepper();

  // Add at least 1 page to the stepper
  virtual void AddPage(CString p_title,StyleTab* p_page,UINT p_resource);

  // SETTERS
  virtual void SetStepperData(void* p_data)         { m_data      = p_data; }
  virtual void SetButtonTextPrior(CString p_text)   { m_textPrior = p_text; }
  virtual void SetButtonTextNext (CString p_text)   { m_textNext  = p_text; }
  virtual void SetButtonTextReady(CString p_text)   { m_textReady = p_text; }
  virtual void SetCaption(CString p_caption);
  virtual void SetDefaultButton(bool p_prior = false,bool p_next = true);
  // GETTERS
  void*   GetStepperData()      { return m_data;      }
  int     GetPagesCount()       { return (int) m_pages.size(); }
  CString GetButtonTextPrior()  { return m_textPrior; }
  CString GetButtonTextNext()   { return m_textNext;  }
  CString GetButtonTextReady()  { return m_textReady; }
  CString GetCaption()          { return m_caption;   }
  // Steppers do not close with ENTER
  virtual bool OnClosing();

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;
  virtual void SetupDynamicLayout() override;

  virtual void InitPages();
  virtual void InitFonts();
  virtual void DisplayPage();
  
  virtual void TryPageForward();
  virtual void TryComplete();
  virtual void Draw();

  // Internal for OnSize
  void ResizePages(int cx,int cy);
  void ClearStepperArea();
  void AdjustSteperSize();
  void MoveButton(StyleButton& p_button,int x,int y);
  void AdjustDefaultButtons();

private:
  CString      m_caption;
  CString      m_textPrior;
  CString      m_textNext;
  CString      m_textReady;
  StyleButton  m_buttonPrior;
  StyleButton  m_buttonNext;
  StepperPages m_pages;
  CFont        m_stepFont;
  CFont        m_titleFont;
  unsigned     m_activePage;
  void*        m_data;
  bool         m_priorIsDefault { false };
  bool         m_nextIsDefault  { true  };

  DECLARE_MESSAGE_MAP();

  afx_msg void    OnPaint();
  afx_msg void    OnBnClickedPrior();
  afx_msg void    OnBnClickedNext();
  afx_msg void    OnSize(UINT p_type,int cx,int cy);
  afx_msg LRESULT OnDpiChanged(WPARAM wParam,LPARAM lParam);
  afx_msg void    OnCancel();
};

