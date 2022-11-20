////////////////////////////////////////////////////////////////////////
//
// File: StyleMessageBox.cpp
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
#include "stdafx.h"
#include <map>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int    OFFSET               =   8;    // Spaces between controls and texts
const int    EXTRA_LINESPACING    =  14;    // Extra space for "Do not show again" check box 
const int    ID_OFFSET            =  10;    // Keep away from IDOK / IDCANCEL
const double ButtonWidthFactor    =   8;    // With of a button in "W"-letters
const int    FOREGROUND_TIMER     = 100;    // Timer (higher then number of labels)
const int    FOREGROUND_INTERVAL  = 1000;   // Timer 1000ms back in the foreground

static void SuppressMessage(CString p_message);
static bool IsSuppressableMessage(CString p_message);

// OUTSIDE WORLD INTERFACE

INT_PTR 
StyleMessageBox(CWnd*  p_parent
               ,LPCSTR p_message
               ,LPCSTR p_title
               ,long   p_styles         /*= MB_OK*/
               ,bool*  p_doNotShowAgain /*= nullptr*/)
{
  MessageDialog dlg(p_parent,p_title,p_message,p_styles,p_doNotShowAgain);
  return dlg.GetResultID(dlg.DoModal());
}

INT_PTR 
StyleMessageBox(CWnd*  p_parent
               ,LPCSTR p_message
               ,LPCSTR p_title
               ,LPCSTR p_labels         /*= "ok"*/
               ,bool*  p_doNotShowAgain /*= nullptr*/
               ,bool   p_foreground     /*= false*/)
{
  MessageDialog dlg(p_parent,p_title,p_message,p_labels,p_doNotShowAgain);
  if(p_foreground)
  {
    dlg.SetForeground();
  }
  return dlg.GetResultID(dlg.DoModal());
}

//////////////////////////////////////////////////////////////////////////

MessageDialog::MessageDialog(CWnd*   p_parent
                            ,LPCSTR  p_title
                            ,LPCSTR  p_message
                            ,LPCSTR  p_labels
                            ,bool*   p_doNotShowAgain /*= NULL*/)
              :StyleDialogCA(IDD_BOODSCHAP, p_parent)
              ,m_title(p_title)
              ,m_message(p_message)
              ,m_styles(MB_STRINGS)
              ,m_default(0)
              ,m_image(nullptr)
              ,m_icon(NULL)
              ,m_def_done(false)
              ,m_foreground(false)
              ,m_suppress(NULL)
              ,m_suppressable(false)
              ,m_notAgain(false)
              ,m_font(NULL)
              ,m_notAgainPtr(p_doNotShowAgain)
{
  InitButtons();

  CString rest(p_labels);
  // See if we must be forced into the foreground
  if(rest.GetLength() >= 2 && rest.Left(2) == "! ")
  {
    m_foreground = true;
    rest = rest.Mid(2);
  }

  // Walk the labels string
  int spatiePos = 0;
  for (int i = 0; i < MAX_LABELS; ++i)
  {
    spatiePos = rest.Find(' ');
    if(spatiePos > 0)
    {
      m_originalLabel[i] = EverythingBefore(rest," "); 
      rest = EverythingAfter(rest," "); 
    }
    else
    {
      m_originalLabel[i] = rest;
      rest.Empty();
    }

    // find timer information
    const CString tag = "_#timer";
    int pos = FindNoCase(m_originalLabel[i],tag);
    if (pos >= 0)
    {
      int begin = m_originalLabel[i].Find("[", pos + tag.GetLength());
      if (begin < 0)
      {
        begin = m_originalLabel[i].Find("(", pos + tag.GetLength());
        m_labelcountdown[i] = begin >= 0;
      }
      if (begin >= 0)
      {
        int end = m_originalLabel[i].Find(m_labelcountdown[i] ? ")" : "]", begin + 1);
        if (end >= 0)
        {
          m_labeltimers[i] = atoi(m_originalLabel[i].Mid(pos+tag.GetLength()+1, end - begin- 1));
          m_originalLabel[i].Delete(pos, end-pos+1);
        }
      }
    }
  }

  // Labels without timer information
  CString labels;
  for (int i = 0; i < MAX_LABELS; ++i)
  {
    if (m_originalLabel[i].IsEmpty())
    {
      break;
    }
    if (i > 0)
    {
      labels += " ";
    }
    labels += m_originalLabel[i];
  }

  for (int i = 0; i < MAX_LABELS; ++i)
  {
    if (m_originalLabel[i].IsEmpty())
    {
      break;
    }

    m_originalLabel[i].Replace("_"," ");
    rest.TrimLeft();
    if (m_originalLabel[i].Find('$') > 0)
    {
      m_originalLabel[i] = EverythingBefore(m_originalLabel[i],"$");
    }
    char exception = m_originalLabel[i].GetAt(0);
    if(exception == '!' || exception == '#' ||
       exception == '.' || exception == '?' ||
       exception == '*' )
    {
      m_originalLabel[i] = m_originalLabel[i].Mid(1);
    }
    int posAmpersand = m_originalLabel[i].Find("&");
    while (posAmpersand >= 0)
    {
      m_originalLabel[i].Delete(posAmpersand);
      posAmpersand = m_originalLabel[i].Find("&");
    }
    if (m_originalLabel[i].GetAt(0) == '@')
    {
      m_originalLabel[i] = m_originalLabel[i].Mid(1);
    }
  }

  // Find signal icon on the first label
  int signal = 0;
  if (labels.GetAt(0) == '!') 
  {
    signal  = MB_ICONEXCLAMATION;
    m_image = IDI_EXCLAMATION;
    m_icon  = LoadIcon(nullptr,m_image);
  }
  else if (labels.GetAt(0) == '?') 
  {
    signal  = MB_ICONQUESTION;
    m_image = IDI_QUESTION;
    m_icon  = LoadIcon(nullptr,m_image);
  }
  else if (labels.GetAt(0) == '.') 
  {
    m_error = true;
    signal  = MB_ICONHAND;
    m_image = IDI_HAND;
    m_icon  = LoadIcon(nullptr,m_image);
  }
  else if (labels.GetAt(0) == '#') 
  {
    signal = MB_ICONASTERISK;
    m_suppressable = true;
    m_image = IDI_ASTERISK;
    m_icon  = LoadIcon(nullptr,m_image);
  }
  else if(labels.GetAt(0) == '*')
  {
    signal = MB_ICONERROR;
    m_suppressable = true;
    m_image = IDI_SHIELD;
    m_icon  = LoadIcon(nullptr,m_image);
  }
  else
  {
    m_suppressable = true;
  }

  if (m_image)
  {
    // Remove the '!?.#' chars from the label
    labels = labels.Mid(1);
    // Give the correct beep
    MessageBeep(signal);
  }

  // Optimize the labels for styles
  if(labels == GetStyleText(TXT_S_OK_CANCEL))     // "ok cancel"
  {
    labels = GetStyleText(TXT_OK_CANCEL);         // "ok cancel$can"
  }
  if(labels == GetStyleText(TXT_S_ABORT_RETRY))   // "abort retry ignore"
  {
    labels = GetStyleText(TXT_ABORT_RETRY);       // "abort$can retry$ok ignore$rem"
  }
  if(labels == GetStyleText(TXT_S_YES_NO))        // "yes no"
  {
    labels = GetStyleText(TXT_S_YES_NO);          // "yes$ok no$can"
  }
  if(labels == GetStyleText(TXT_S_YES_NO_CANCEL)) // "yes no cancel"
  {
    labels = GetStyleText(TXT_YES_NO_CANCEL);     // "yes$ok no$can cancel$rem"
  }
  if(labels == GetStyleText(TXT_S_RETRY_CANCEL))  // "retry cancel"
  {
    labels = GetStyleText(TXT_RETRY_CANCEL);      // "retry$ok cancel$can";
  }
  SplitLabelTextAndStyles(labels);
}

MessageDialog::MessageDialog(CWnd*  p_parent
                            ,LPCSTR p_title
                            ,LPCSTR p_message
                            ,int    p_styles
                            ,bool*  p_doNotShowAgain /*= NULL*/)
              :StyleDialogCA(IDD_BOODSCHAP,p_parent)
              ,m_title(p_title)
              ,m_message(p_message)
              ,m_default(0)
              ,m_image(nullptr)
              ,m_icon(NULL)
              ,m_def_done(false)
              ,m_foreground(false)
              ,m_styles(p_styles)
              ,m_suppress(NULL)
              ,m_suppressable(false)
              ,m_notAgain(false)
              ,m_font(NULL)
              ,m_notAgainPtr(p_doNotShowAgain)
{
  InitButtons();

  CString labels;
  if ((p_styles & MB_TYPEMASK) == MB_OK)
  {
    labels = GetStyleText(TXT_OK); // "ok"
  }
  if ((p_styles & MB_TYPEMASK) == MB_OKCANCEL)
  {
    labels = GetStyleText(TXT_OK_CANCEL); // "ok cancel$can"
  }
  if ((p_styles & MB_TYPEMASK) == MB_ABORTRETRYIGNORE)
  {
    labels = GetStyleText(TXT_ABORT_RETRY); // "abort$can retry$ok ignore$rem"
  }
  if ((p_styles & MB_TYPEMASK) == MB_YESNO)
  {
    labels = GetStyleText(TXT_YES_NO); // "yes$ok no$can"
  }
  if ((p_styles & MB_TYPEMASK) == MB_YESNOCANCEL)
  {
    labels = GetStyleText(TXT_YES_NO_CANCEL); // "yes$ok no$can cancel$rem"
  }
  if ((p_styles & MB_TYPEMASK) == MB_RETRYCANCEL)
  {
    labels = GetStyleText(TXT_RETRY_CANCEL); // "retry$ok cancel$can"
  }

  int style = p_styles & MB_ICONMASK;
  if (style)
  {
    // (MB_ICONHAND | MB_ICONQUESTION | MB_ICONEXCLAMATION | MB_ICONASTERISK)
    if(style == MB_ICONASTERISK)     m_image = IDI_ASTERISK;
    if(style == MB_ICONEXCLAMATION)  m_image = IDI_EXCLAMATION;
    if(style == MB_ICONQUESTION)     m_image = IDI_QUESTION;
    if(style == MB_USERICON)         m_image = IDI_SHIELD;
    if(style == MB_ICONHAND)
    {
      m_error = true;
      m_image = IDI_HAND;
    }
    if(m_image)
    {
      m_icon = LoadIcon(nullptr, m_image);
    }
		m_suppressable = style == MB_ICONASTERISK;
    // Give correct beep by style
    MessageBeep(style);
  }
  else
  {
    m_suppressable = true;
  }

  // Set different default button?
  if (p_styles & MB_DEFMASK)
  {
    if ((p_styles & MB_DEFMASK) == MB_DEFBUTTON1) m_default = ID_OFFSET + 0;
    if ((p_styles & MB_DEFMASK) == MB_DEFBUTTON2) m_default = ID_OFFSET + 1;
    if ((p_styles & MB_DEFMASK) == MB_DEFBUTTON3) m_default = ID_OFFSET + 2;
  }

  if (p_styles & MB_SETFOREGROUND)
  {
    m_foreground = true;
  }

  SplitLabelTextAndStyles(labels);
}

// Destructor
MessageDialog::~MessageDialog()
{
  SuppressFromNowOn();
  ResetButtons();
}

BEGIN_MESSAGE_MAP(MessageDialog,StyleDialogCA)
  ON_WM_PAINT()
  ON_BN_CLICKED(ID_SUPPRESS,OnDoNotShowAgain)
  ON_WM_TIMER()
END_MESSAGE_MAP()

void MessageDialog::DoDataExchange(CDataExchange* pDX)
{
  StyleDialogCA::DoDataExchange(pDX);

  DDX_Control(pDX,IDC_BOODSCHAP,m_edit);
}

// Init buttons
void
MessageDialog::InitButtons()
{
  for (int i = 0; i < MAX_LABELS; ++i)
  {
    m_button[i]         = nullptr;
    m_originalLabel[i]  = CString("");
    m_labeltimers[i]    = 0;
    m_labelcountdown[i] = false;
  }
}

// Reset the button controls
void
MessageDialog::ResetButtons()
{
  for (int i = 0; i < MAX_LABELS; ++i)
  {
    delete m_button[i];
    m_button[i] = nullptr;
  }
  if (m_suppress)
  {
    delete m_suppress;
    m_suppress = nullptr;
  }
}

BOOL
MessageDialog::PreTranslateMessage(MSG* pMsg)
{
  switch (pMsg->message)
  {
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
      if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_SPACE)
      {
        if(StyleButton* button = dynamic_cast<StyleButton*>(GetFocus()))
        { 
          EndDialog(button->GetDlgCtrlID());
        }
        return TRUE;
      }
      if(pMsg->wParam == VK_LEFT)
      {
        if(StyleButton* button = dynamic_cast<StyleButton*>(GetFocus()))
        {
          GotoControl(-1);
          return TRUE;
        }
      }
      if(pMsg->wParam == VK_RIGHT)
      {
        if(StyleButton* button = dynamic_cast<StyleButton*>(GetFocus()))
        {
          GotoControl(1);
          return TRUE;
        }
      }
      for (int i = 0; i < MAX_LABELS; ++i)
      {
        int ch = tolower((int)pMsg->wParam);

        // Find the label that begins with this character
        if(!m_label[i].IsEmpty() && (m_button[i]->GetStyle() & WS_DISABLED) == 0)
        {
          if(tolower(m_label[i].GetAt(0)) == ch)
          {
            EndDialog(i + ID_OFFSET);
            return TRUE;
          }
        }
      }
      break;
  }
  return StyleDialogCA::PreTranslateMessage(pMsg);
}

// Window procedure with our own error handling
LRESULT 
MessageDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
  try
  {
    return StyleDialogCA::WindowProc(message, wParam, lParam);
  }
  catch (CString& er)
  {
    AfxMessageBox(er,MB_OK | MB_ICONERROR);
    return 0;
  }
}

// Go to the Next/Previous control on the message box
void 
MessageDialog::GotoControl(int p_direction)
{
  // Are we on the text of the message dialog
  bool onText = (&m_edit == reinterpret_cast<CEdit*>(GetFocus()));
  int focusButton = 0;

  // Count the number of buttons
  int maxButton = 0;
  for(int ind = 0; ind < MAX_LABELS; ++ind)
  {
    if(!m_label[ind].IsEmpty())
    {
      ++maxButton;
    }
    else break;
  }

  if(onText)
  {
    if(p_direction > 0)
    {
      // We are on the text and go to the first button
      focusButton = 0;
    }
    else
    {
      focusButton = maxButton - 1;
    }
  }
  else
  {
    for(int ind = 0; ind < maxButton; ++ind)
    {
      StyleButton* button = reinterpret_cast<StyleButton*>(GetFocus());
      if(m_button[ind] == button)
      {
        if(button->GetStyle() & BS_DEFPUSHBUTTON)
        {
          button->ModifyStyle(BS_DEFPUSHBUTTON,0);
        }
        focusButton = ind + p_direction;
        break;
      }
    }
  }

  if(focusButton < 0 || focusButton == maxButton)
  {
    m_edit.SetFocus();
  }
  else
  {
    m_button[focusButton]->SetFocus();
  }
}

// Split label text in separate labels
// ok -> ok
// "yes no" -> "yes", "no"
// Get the button type from the label
// "buttontext$ab" -> label = "buttontext" style = "ab"
//
void
MessageDialog::SplitLabelTextAndStyles(CString& p_labels)
{
  // Reset everything
  for (int i = 0; i < MAX_LABELS; ++i)
  {
    m_label[i] = CString("");
    m_style[i] = CString("");
    m_width[i] = 0;
  }

  // Walk the labels string
  // Getting label and style
  CString rest = p_labels;
  int spatiePos = 0;
  for (int i = 0; i < MAX_LABELS; ++i)
  {
    spatiePos = rest.Find(' ');
    if(spatiePos > 0)
    {
      m_label[i] = EverythingBefore(rest," ");
      rest       = EverythingAfter(rest," ");
    }
    else
    {
      m_label[i] = rest;
    }
    m_label[i].Replace("_"," ");
    rest.TrimLeft();
    if (m_label[i].Find('$') > 0)
    {
      m_style[i] = EverythingAfter (m_label[i],"$");
      m_label[i] = EverythingBefore(m_label[i],"$");
    }
    if (spatiePos < 0)
    {
      break;
    }
  }

  // Find the default button. Only the first will be found
  for (int i = 0; i < MAX_LABELS; ++i)
  {
    if (!m_label[i].IsEmpty())
    {
      if (m_label[i].GetAt(0) == '@')
      {
        m_default  = i + ID_OFFSET;
        m_label[i] = m_label[i].Mid(1);
        break;
      }
    }
  }

  // If no default, get a default (first button)
  if (m_default == 0)
  {
    m_default = ID_OFFSET;
  }

  // Set first character in uppercase (exception = OK)
  for (int i = 0; i < MAX_LABELS; ++i)
  {
    if (!m_label[i].IsEmpty())
    {
      if (m_label[i] == "ok")
      {
        m_label[i] = "OK";
      }
      else
      {
        // Skip all non-alpha characters
        int index = 0;
        for (index = 0; (index < m_label[i].GetLength()) && (!isalpha(m_label[i].GetAt(index))); index++)
        {
          // do nothing
        }
        if (index < m_label[i].GetLength()) // Something left?
        {
          // Set first character of the label to uppercase
          m_label[i].SetAt(index, (char) toupper(m_label[i].GetAt(index)));
        }
      }
    }
  }

  // Check if the message is suppressible
  if (m_label[0].CompareNoCase("ok") != 0 || !m_label[1].IsEmpty())
  {
    m_suppressable = false;
  }
}

// Translate button ID of the result
// IDCANCEL = ESC key
// ID       = ID of the control
CString
MessageDialog::GetResult(int p_id)
{
  CString result;
  if ((p_id >= ID_OFFSET) && (p_id < (ID_OFFSET + MAX_LABELS)))
  {
    int label = p_id - ID_OFFSET;
    if (label >= 0 && label < MAX_LABELS)
    {
      result = m_label[label];
    }
  }
  else
  {
    if (p_id == IDCANCEL)
    {
      // Pressed the ESC / CANCEL button
      result = "";
    }
  }
  result.MakeLower();
  return result;
}

CString
MessageDialog::GetOriginalResult(int p_id)
{
  CString result;
  if ((p_id >= ID_OFFSET) && (p_id < (ID_OFFSET + MAX_LABELS)))
  {
    int label = p_id - ID_OFFSET;
    if (label >= 0 && label < MAX_LABELS)
    {
      result = m_originalLabel[label];
    }
  }
  else
  {
    if (p_id == IDCANCEL)
    {
      // Pressed the cancel button
      result = "";
    }
  }
  result.MakeLower();
  return result;
}

// Return the result as a standard MessageBox ID
INT_PTR
MessageDialog::GetResultID(INT_PTR p_id)
{
  switch (m_styles & 0x0f)
  {
    case MB_OK:               if (p_id == (ID_OFFSET + 0))  return IDOK;
                              break;
    case MB_YESNO:            if (p_id == (ID_OFFSET + 0))  return IDYES;
                              if (p_id == (ID_OFFSET + 1))  return IDNO;
                              break;
    case MB_RETRYCANCEL:      if (p_id == (ID_OFFSET + 0))  return IDRETRY;
                              if (p_id == (ID_OFFSET + 1))  return IDCANCEL;
                              break;
    case MB_OKCANCEL:         if (p_id == (ID_OFFSET + 0))  return IDOK;
                              if (p_id == (ID_OFFSET + 1))  return IDCANCEL;
                              break;
    case MB_YESNOCANCEL:      if (p_id == (ID_OFFSET + 0))  return IDYES;
                              if (p_id == (ID_OFFSET + 1))  return IDNO;
                              if (p_id == (ID_OFFSET + 2))  return IDCANCEL;
                              break;
    case MB_ABORTRETRYIGNORE: if (p_id == (ID_OFFSET + 0))  return IDABORT;
                              if (p_id == (ID_OFFSET + 1))  return IDRETRY;
                              if (p_id == (ID_OFFSET + 2))  return IDIGNORE;
                              break;
    case MB_STRINGS:          return (p_id - ID_OFFSET);
                              break;
  }
  if (p_id == IDCANCEL)
  {
    return p_id;
  }
  // Unknown result
  return 0;
}

CString
MessageDialog::GetStandardPositive()
{
  for (int i = 0; i < MAX_LABELS; ++i)
  {
    if (!m_style[i].IsEmpty())
    {
      if (m_style[i].CompareNoCase("ok") == 0)
      {
        return m_label[i];
      }
    }
  }
  return "";
}

CString
MessageDialog::GetStandardNegative()
{
  for (int i = 0; i < MAX_LABELS; ++i)
  {
    if (!m_style[i].IsEmpty())
    {
      if (m_style[i].CompareNoCase("an") == 0)
      {
        return m_label[i];
      }
    }
  }
  return "";
}

int
MessageDialog::GetStandardPositiveID()
{
  for (int i = 0; i < MAX_LABELS; ++i)
  {
    if (!m_style[i].IsEmpty())
    {
      if (m_style[i].CompareNoCase("ok") == 0)
      {
        return i + ID_OFFSET;
      }
    }
  }
  return 0;
}

int
MessageDialog::GetStandardNegativeID()
{
  for (int i = 0; i < MAX_LABELS; ++i)
  {
    if (!m_style[i].IsEmpty())
    {
      if (m_style[i].CompareNoCase("an") == 0)
      {
        return i + ID_OFFSET;
      }
    }
  }
  return 0;
}

//////////////////////////////////////////////////////////////////////////
//
// PRIVATE METHODS
//
//////////////////////////////////////////////////////////////////////////

void
MessageDialog::OnTimer(UINT_PTR nIDEvent)
{
  // Once in a while we go back to the foreground
  // Specially if multiple programs want to be there !!
  if(nIDEvent == FOREGROUND_TIMER)
  {
    SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
    return;
  }

  // Do the label timers
  if (m_labelcountdown[nIDEvent])
  {
    CString sButtonTekst = m_label[nIDEvent];
    if (!Contains(sButtonTekst,"&"))
    {
      sButtonTekst = "&" + sButtonTekst;
    }
    else
    {
      int posAmpersand = m_label[nIDEvent].Find("&");
      while (posAmpersand >= 0)
      {
        m_label[nIDEvent].Delete(posAmpersand);
        posAmpersand = m_label[nIDEvent].Find("&");
      }
    }

    if (--m_labeltimers[nIDEvent] > 0)
    {
      sButtonTekst.AppendFormat("(%d)",m_labeltimers[nIDEvent]);
      SetTimer(nIDEvent, 1000, NULL);
      m_button[nIDEvent]->SetWindowText(sButtonTekst);
      return;
    }
    m_button[nIDEvent]->SetWindowText(sButtonTekst);
  }

  m_button[nIDEvent]->EnableWindow();
  if (m_button[nIDEvent] == GetDlgItem(m_default))
  {
    m_button[nIDEvent]->SetFocus();
  }
}

// Calculate whole message box and start it
BOOL
MessageDialog::OnInitDialog()
{
  StyleDialogCA::OnInitDialog();

  ShowCloseButton(false);

  // MB_ICONERROR message?
  m_font = (m_image == IDI_SHIELD) ? &STYLEFONTS.ErrorTextFont : &STYLEFONTS.DialogTextFont;
  SetWindowText(m_title); 

  // Grab control and put font on it
  CEdit* edit = (CEdit*)GetDlgItem(IDC_BOODSCHAP);
  edit->SetFont(m_font);
  
  // GetDC will **NOT** get you a dc with the right font
  // So we select the correct font here to do the calculations
  CClientDC dc(edit);
  dc.SelectObject(m_font);

  // Getting the text
  CString text = m_message;

  // Replace all occurrences of \n for \r\n
  text.Replace("\r\n", "\n");
  text.Replace("\n", "\r\n");

  // Getting width and height
  CRect tekstRect = CRect(0, 0, 0, 0);
  tekstRect.right = GetSystemMetrics(SM_CXSCREEN) * 90 / 100;
  dc.DrawText(text,&tekstRect,DT_CALCRECT | DT_LEFT | DT_NOPREFIX | DT_WORDBREAK | DT_EDITCONTROL);

  // The internal margins of the edit control need to be added to the text part
  // so the edit control will become big enough to show margins itself
  CRect margins;
  edit->GetRect(margins);
  tekstRect.OffsetRect(0, WINDOWCAPTIONHEIGHT);
  tekstRect.right  += margins.left * 2;
  tekstRect.bottom += margins.top * 2;

  // If it gets to high, constraint it and use an extra scroll bar
  int maxHeight = (GetSystemMetrics(SM_CYSCREEN) * 80) / 100;
  if (tekstRect.bottom > maxHeight)
  {
    edit->ModifyStyle(0, WS_VSCROLL);
    tekstRect.bottom = maxHeight;
    tekstRect.right += GetSystemMetrics(SM_CXHTHUMB);
  }

  // Add fixed offsets 
  tekstRect.OffsetRect(OFFSET,OFFSET);

  // Information pictogram
  if (m_image)
  {
    // Extra space at the left side of 32 pixels for the icon
    tekstRect.OffsetRect(32 + OFFSET,0);
    if (tekstRect.bottom - WINDOWCAPTIONHEIGHT < 32)
    {
      // Still to do: vertically centering
      tekstRect.bottom = WINDOWCAPTIONHEIGHT + 32 + OFFSET;
    }
  }

  // Place the edit control and the text
  edit->MoveWindow(tekstRect);
  edit->SetWindowText(text);

  // See to it that a very long title will be visible
  NONCLIENTMETRICS ncm;
  ncm.cbSize = sizeof(ncm);
  if (SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(ncm), &ncm, 0))
  {
    CFont fnt;
    fnt.CreateFontIndirect(&ncm.lfCaptionFont);
    HGDIOBJ obj = dc.SelectObject(fnt);
    CSize size = dc.GetTextExtent(CString(m_title));
    dc.SelectObject(obj);
    DeleteObject(fnt);
    if (size.cx > (GetSystemMetrics(SM_CXSCREEN) * 90) / 100)
    {
      size.cx = (GetSystemMetrics(SM_CXSCREEN) * 90) / 100;
    }
    if (size.cx > tekstRect.Width())
    {
      tekstRect.right = tekstRect.left + size.cx;
    }
  }

  // Making the buttons
  // For the width we use the size of a "W"
  // and for the height the 'real' font-height
  CSize tsize = dc.GetTextExtent("W");

  int buttonTop    = tekstRect.bottom + OFFSET;
  int buttonWidth  = (int) (ButtonWidthFactor * tsize.cx) + (3 * OFFSET);
  int buttonHeight = tsize.cy + (2 * GetSystemMetrics(SM_CYFIXEDFRAME));
  int totalWidth   = OFFSET;

  // Consider the widths
  int layout = 0; 
  if (!layout)
  {
    buttonWidth -= (3 * OFFSET);
  }
  // Calculate the width of all buttons together
  for (int i = 0; i < MAX_LABELS; ++i)
  {
    if (!m_label[i].IsEmpty())
    {
      if (totalWidth > OFFSET) 
      {
        totalWidth += OFFSET;
      }
      m_width[i] = buttonWidth;
      {
        int breedte = (((m_label[i].GetLength() * tsize.cx) * 2) / 3) + (3 * OFFSET);
        if (breedte > buttonWidth)
        {
          m_width[i] = breedte;
        }
      }
      totalWidth += m_width[i];
    }
  }
  // Calculate the beginning of the buttons
  // If the text is wider than the buttons, use the text width for the button
  int buttonBegin = OFFSET;
  if (tekstRect.right > totalWidth)
  {
    buttonBegin += tekstRect.right - totalWidth;
    totalWidth   = tekstRect.right;
  }
  // Create the buttons
  long buttonStyle = BS_PUSHBUTTON | WS_TABSTOP | BS_NOTIFY | WS_CHILD | WS_VISIBLE | BS_ICON;
  for (int i = 0; i < MAX_LABELS; ++i)
  {
    CString sButtonTekst = m_label[i];
    if (!sButtonTekst.IsEmpty())
    {
      DWORD style = buttonStyle;
      int ID = i + ID_OFFSET;
      if (i == 0)
      {
        style |= WS_GROUP;
      }
      if (m_default - ID_OFFSET == i)
      {
        style |= BS_DEFPUSHBUTTON;
				SetDefID(ID);
      }
      // Set an accelerator on the first char of the label, in case of no accelerator
      if (!Contains(sButtonTekst,"&"))
      {
        sButtonTekst = "&" + sButtonTekst;
      }
      else
      {
        // Remove all cases of a '&' from the original string
        int posAmpersand = m_label[i].Find("&");
        while (posAmpersand >= 0)
        {
          m_label[i].Delete(posAmpersand);
          posAmpersand = m_label[i].Find("&");
        }
      }
      
      if (m_labeltimers[i] > 0)
      {
        style |= WS_DISABLED;
        SetTimer(i, m_labelcountdown[i] ? 1000 : m_labeltimers[i] * 1000, NULL);
        if (m_labelcountdown[i])
        {
          sButtonTekst.AppendFormat("(%d)",m_labeltimers[i]);
        }
      }

      buttonWidth = m_width[i];
      CRect rect(buttonBegin, buttonTop, buttonBegin + buttonWidth, buttonTop + buttonHeight);
      m_button[i]  = new StyleButton(m_style[i], m_error);
      m_button[i]->Create(sButtonTekst, // Label text
                          style,        // MS-Windows window style
                          rect,         // Rectangle
                          this,         // My parent
                          ID            // CtrlID of this button
                          );
      m_button[i]->SetFont(m_font);
      // For the next button
      buttonBegin += buttonWidth + OFFSET;
    }
  }
  // Recalculate the window size
  CRect rect(0, 0, totalWidth + OFFSET + WINDOWSHADOWBORDER, buttonTop + OFFSET + buttonHeight + WINDOWSHADOWBORDER);
  if (m_suppressable || (m_notAgainPtr != NULL))
  {
    int hcb = (tsize.cy + 2*GetSystemMetrics(SM_CYFIXEDFRAME));
    rect.bottom += hcb;
    rect.bottom += OFFSET;

    // Make the check box
    int top = buttonTop + buttonHeight + 2 * OFFSET;
    m_line.left   = OFFSET;
    m_line.top    = top - OFFSET;
    m_line.right  = buttonBegin - OFFSET;
    m_line.bottom = m_line.top;

    CRect brect(OFFSET,top+hcb-24, OFFSET+ 16 + 160,top +hcb-6);
    m_suppress = new StyleCheckbox();
    // Beware: Text must be very small, so it will always fit into the message box and be readable
    m_suppress->Create( GetStyleText(TXT_NO_REPEAT),  // "Do not repeat"
                        WS_TABSTOP | BS_AUTOCHECKBOX | BS_NOTIFY | WS_CHILD | WS_VISIBLE,
                        brect,
                        this,
                        ID_SUPPRESS);
  }
  AdjustWindowRect(rect, GetStyle(), false);

  MoveWindow(rect);
  CenterWindow();

  // Set ourselves in the foreground
  if(m_foreground)
  {
    SetForegroundWindow();
    SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
    SetTimer(FOREGROUND_TIMER,FOREGROUND_INTERVAL,nullptr);
    FlashMessageBox();
  }
  // Set focus on the default button
  if (CWnd* defwin = GetDlgItem(m_default))
  {
    defwin->SetFocus();
    return FALSE;
  }
  return TRUE;
}

// Flash our application and message box
void 
MessageDialog::FlashMessageBox()
{
  FLASHWINFO flash;
  flash.cbSize = sizeof(FLASHWINFO);
  flash.hwnd      = GetSafeHwnd();
  flash.dwFlags   = FLASHW_ALL;
  flash.uCount    = 6;
  flash.dwTimeout = 250;

  ::FlashWindowEx(&flash);
}

void
MessageDialog::OnPaint()
{
  if (GetUpdateRect(NULL, FALSE))
  {
    CPaintDC dc(this);

    // We must draw the signal icon
    if(m_image)
    {
      dc.DrawIcon(OFFSET,WINDOWCAPTIONHEIGHT + OFFSET,m_icon);
    }

    // Drawing a line above the checkbox
    // Checkbox will draw itself!
    if (m_suppressable || (m_notAgainPtr != NULL))
    {
      dc.DrawEdge(m_line, EDGE_ETCHED, BF_TOP);
    }
  }
}

void
MessageDialog::OnCancel()
{
  // Ignore the ESC key from a StyleMesageBox
}

// Override for a suppressible message
INT_PTR
MessageDialog::DoModal()
{
  if (m_suppressable)
  {
    if (IsSuppressableMessage(m_message))
    {
      return IDOK;
    }
  }

  // Request attention if we are not in the foreground
  CWnd *w = GetForegroundWindow();
  bool bFlash = true;
  while (w)
  {
    if (w == AfxGetMainWnd())
    {
      bFlash = false;
      break;
    }
    w = w->GetParent();
  }

  // Check parent
  if (!m_pParentWnd)
  {
    m_pParentWnd = AfxGetMainWnd();
  }
  if(m_pParentWnd && m_pParentWnd->GetSafeHwnd() && !m_pParentWnd->IsWindowVisible() && bFlash)
  {
    m_pParentWnd->FlashWindowEx(FLASHW_TRAY,3,0);
  }

  // Show dialog by parent's DoModal
  INT_PTR res = StyleDialogCA::DoModal();

  if (m_notAgainPtr != NULL)
  {
    *m_notAgainPtr = m_notAgain;
  }
  // Ready
  return res;
}

// We push this button
// Only a hit if the mouse is still above the button
void
MessageDialog::PushThisButton(UINT p_id,CPoint point)
{
  CWnd* wnd = GetDlgItem(p_id);
  if (wnd)
  {
    CRect rect;
    wnd->GetClientRect(rect);
    if (rect.PtInRect(point))
    {
      EndDialog(p_id);
    }
  }
}

// See if the button is the default button of the dialog
// if so: set the initial focus
bool
MessageDialog::GetDefault(UINT ID)
{
  if (!m_def_done)
  {
    // First moment after the starting of the dialog
    // and before the painting of the first button. Setting the focus
    m_def_done = true;
    if (m_default)
    {
      GotoDlgCtrl(m_button[m_default - ID_OFFSET]);
    }
  }
  if (ID == (UINT)m_default)
  {
    return true;
  }
  return false;
}

// Check box handler for suppression
void
MessageDialog::OnDoNotShowAgain()
{
  m_notAgain = !m_notAgain;
}

// If message suppressed, always IDOK
void
MessageDialog::SuppressFromNowOn()
{
  if (m_notAgain && (m_notAgainPtr == NULL))
  {
    SuppressMessage(m_message);
  }
}

//////////////////////////////////////////////////////////////////////////
//
// Suppress a message
//
//////////////////////////////////////////////////////////////////////////

using  SurpressionMap = std::map<CString,int>;
static SurpressionMap suppress;

static void SuppressMessage(CString p_message)
{
  SurpressionMap::iterator it = suppress.find(p_message);
  if(it == suppress.end())
  {
    suppress.insert(std::make_pair(p_message,1));
  }
  else
  {
    ++it->second;
  }
}

static bool IsSuppressableMessage(CString p_message)
{
  SurpressionMap::iterator it = suppress.find(p_message);
  return it != suppress.end();
}
