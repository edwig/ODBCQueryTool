////////////////////////////////////////////////////////////////////////
//
// File: StyleToast.cpp
// Function: Styling frame for a TOAST control
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
#include "StyleToast.h"
#include "StyleColors.h"
#include "StyleUtilities.h"
#include "StyleFonts.h"

// Use automatic critical section
class AutoLock
{
public:
  explicit AutoLock(CRITICAL_SECTION* section): m_section(section)
  {
    EnterCriticalSection(m_section);
  }
  ~AutoLock()
  {
    LeaveCriticalSection(m_section);
  }
  void Unlock()
  {
    LeaveCriticalSection(m_section);
  };
  void Relock()
  {
    EnterCriticalSection(m_section);
  };
private:
  CRITICAL_SECTION* m_section;
};



using namespace ThemeColor;

StyleToast::StyleToast(int      p_style
                      ,int      p_position
                      ,CString  p_text1
                      ,CString  p_text2
                      ,CString  p_text3
                      ,unsigned p_timeout)
           :CDialog(IDD_TOAST)
           ,m_style(p_style)
           ,m_position(p_position)
           ,m_text1(p_text1)
           ,m_text2(p_text2)
           ,m_text3(p_text3)
           ,m_timeout(p_timeout)
           ,m_steps(0)
           ,m_background(RGB(255,255,255))
           ,m_foreground(RGB(0,0,0))
{
}

StyleToast::~StyleToast()
{
  DestroyWindow();
}

void 
StyleToast::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX,IDC_TOAST,m_showText);
}

BEGIN_MESSAGE_MAP(StyleToast,CDialog)
  ON_WM_CREATE()
  ON_WM_TIMER()
  ON_WM_CTLCOLOR()
  ON_WM_PAINT()
  ON_STN_CLICKED(IDC_TOAST,OnClicked)
END_MESSAGE_MAP()

BOOL
StyleToast::OnInitDialog()
{
  CDialog::OnInitDialog();
  
  m_background = RGB(255,255,255);
  m_foreground = RGB(  0,  0,  0);
  switch(m_style)
  {
    case STYLE_TOAST_MESSAGE: m_background = ThemeColor::GetColor(Colors::AccentColor1);
                              m_foreground = RGB(  0,  0,  0);
                              break;
    case STYLE_TOAST_WARNING: m_background = RGB(236,183,  0);
                              m_foreground = RGB(  0,  0,  0);
                              break;
    case STYLE_TOAST_ERROR:   m_background = RGB(220,  0,  0);
                              m_foreground = RGB(255,255,  0);
                              break;
  }

  // Special case for black-on-black
  if(m_background == 0)
  {
    m_foreground = RGB(255,255,255);
    m_dimmedback = RGB(127,127,127);
  }
  else
  {
    SetHalfBackgroundColor();
  }
  m_showText.SetBkColor(m_background);
  m_showText.SetTextColor(m_foreground);
  m_showText.SetWindowText(m_text1);
  m_showText.SetExtraText1(m_text2);
  m_showText.SetExtraText2(m_text3);
  
  if(m_timeout > 0)
  {
    SetTimer(EV_TOAST,  m_timeout,nullptr);
    SetTimer(EV_STEPPER,m_timeout / TOAST_STEPS,nullptr);
    m_steps = TOAST_STEPS;
  }
  UpdateData(FALSE);
  PumpMessage();

  return TRUE;
}

void
StyleToast::SetHalfBackgroundColor()
{
  int halfred  =  (m_background & 0xFF) * 4 / 5;
  int halfgre  = ((m_background & 0xFFFF) >> 8) * 4 / 5;
  int halfblu  =  (m_background >> 16) * 4 / 5;
  m_dimmedback = RGB(halfred,halfgre,halfblu);
}

int
StyleToast::OnCreate(LPCREATESTRUCT p_create)
{
  int res = CDialog::OnCreate(p_create);

  CRect rect;
  GetWindowRect(&rect);
  SFXResizeByFactor(rect);
  MoveWindow(&rect);

  return res;
}

void
StyleToast::OnSize(UINT nType, int x, int y)
{
  y -= m_timeout ? TOAST_LINE : 0;
  m_showText.MoveWindow(0,0,x,y);
}

void
StyleToast::OnTimer(UINT_PTR nIDEvent)
{
  if(nIDEvent == EV_TOAST)
  {
    DestroyToast(this);
  }
  if(nIDEvent == EV_STEPPER)
  {
    --m_steps;
    Invalidate();
  }
}

HBRUSH
StyleToast::OnCtlColor(CDC* pDC,CWnd* pWnd,UINT nCtlColor)
{
  if(nCtlColor == CTLCOLOR_DLG)
  {
    pDC->SetBkColor(m_background);
  }
  return CDialog::OnCtlColor(pDC,pWnd,nCtlColor);
}

void
StyleToast::OnClicked()
{
  DestroyToast(this);
}

void
StyleToast::OnPaint()
{
  CDialog::OnPaint();

  CRect rect;
  GetClientRect(rect);
  CDC* dc = GetDC();
  CRect stepper(rect);
  stepper.top    = rect.bottom - TOAST_LINE;
  stepper.right  = rect.left + (rect.Width() * m_steps / TOAST_STEPS);

  // Paint left side of the stepper
  PaintStepper(dc,stepper,m_dimmedback);

  // Draw the right side
  stepper.left  = stepper.right;
  stepper.right = rect.right;
  PaintStepper(dc,stepper,m_background);

  ReleaseDC(dc);
}

void
StyleToast::PaintStepper(CDC* p_dc,CRect p_stepper,DWORD p_color)
{
  // Select brush and pen
  CBrush  brush(p_color);
  CBrush* oldBrush = p_dc->SelectObject(&brush);
  CPen    pen;
  pen.CreatePen(PS_SOLID,1,p_color);
  CPen* oldPen = p_dc->SelectObject(&pen);

  // Draw the stepper part
  p_dc->Rectangle(p_stepper);

  /// Reselect original objects
  p_dc->SelectObject(oldBrush);
  p_dc->SelectObject(oldPen);
}

void
StyleToast::PumpMessage()
{
  // We just handle the paint messages, so we get visible
  // for larger and longer processes.
  // We could face an eternal loop, so we put a time constriction on it!
  MSG msg;
  UINT ticks = GetTickCount();
  while(GetTickCount() - ticks < 200 &&
        (PeekMessage(&msg,NULL,WM_PAINT,WM_PAINT,          PM_REMOVE) ||
         PeekMessage(&msg,NULL,WM_SYSCOMMAND,WM_SYSCOMMAND,PM_REMOVE)))
  {
    try
    {
      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);
    }
    catch(...)
    {
      // How now, brown cow?
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
// STATIC FUNCTIONS
//
//////////////////////////////////////////////////////////////////////////

Toasts g_toasts;

Toasts::Toasts()
{
  InitializeCriticalSection(&m_lock);
}

Toasts::~Toasts()
{
  // Delete in lock scope
  {
    AutoLock lock(&m_lock);
    for(auto& toast : m_toasts)
    {
      delete toast;
    }
    m_toasts.clear();
  }
  DeleteCriticalSection(&m_lock);
}

void DestroyToast(StyleToast* p_toast)
{
  AutoLock lock(&g_toasts.m_lock);

  std::vector<StyleToast*>::iterator it = g_toasts.m_toasts.begin();
  while(it != g_toasts.m_toasts.end())
  {
    if(*it == p_toast)
    {
      delete p_toast;
      g_toasts.m_toasts.erase(it);
      return;
    }
    ++it;
  }
  // Not found. Delete it anyway
  delete p_toast;
}

static void MoreThanOne(int h,int& y,bool up)
{
  AutoLock lock(&g_toasts.m_lock);

  int size = (int) g_toasts.m_toasts.size();
  int num  = (size - 1) % TOAST_STACK;

  if(up)
  {
    y -= (num * h) + (num * TOAST_SPACE);
  }
  else
  {
    y += (num * h) + (num * TOAST_SPACE);
  }
}

static bool CalculateHorizontalSize(CDC* p_dc,CString& p_text,int& p_size,int p_max)
{
  CSize size = p_dc->GetTextExtent(p_text);
  int total  = size.cx + 4 * STANDARDFONTSIZE;
  if(total > p_size)
  {
    p_size = total;
    if(p_size > p_max)
    {
      p_size = p_max;
    }
    return true;
  }
  return false;
}

// Create a new TOAST window and put in the queue
// To preserve the object
StyleToast* CreateToast(int      p_style
                       ,int      p_position
                       ,CString  p_text1
                       ,CString  p_text2    /* = ""   */
                       ,CString  p_text3    /* = ""   */
                       ,unsigned p_timeout  /* = 3000 */
                       ,bool*    p_success  /* = nullptr*/ )
{
  CWnd* focuswin = CWnd::FromHandle(GetFocus());
  
  StyleToast* toast = new StyleToast(p_style,p_position,p_text1,p_text2,p_text3,p_timeout);

  if(!toast->Create(MAKEINTRESOURCE(IDD_TOAST),CWnd::FromHandle(GetDesktopWindow())))
  {
    if(p_success)
    {
      *p_success = false;
    }
    delete toast;
    return nullptr;
  }

  // Add to the toasts
  {
    AutoLock lock(&g_toasts.m_lock);
    g_toasts.m_toasts.push_back(toast);
  }

  CRect rect; // Total workarea on the desktop
  CRect size; // Size of the Toast window
  StyleGetWorkArea(toast,rect);
  toast->GetWindowRect(size);
  int y = 0;
  int x = 0;
  bool up = false;

  // Adjust the size if text is longer than expected
  // Can only GROW the toast control to be wider!
  int cx  = size.Width();
  int cy  = size.Height();
  int max = rect.Width() / 2;

  CDC* dc = toast->GetDC();
  if(CalculateHorizontalSize(dc,p_text1,cx,max) ||
     CalculateHorizontalSize(dc,p_text2,cx,max) ||
     CalculateHorizontalSize(dc,p_text3,cx,max) ||
     p_timeout)
  {
    size.left    = size.right - cx;
    size.bottom += p_timeout ? TOAST_LINE : 0;
    toast->MoveWindow(size);
    toast->OnSize(SIZE_MAXIMIZED,size.Width(),size.Height());
  }
  toast->ReleaseDC(dc);
  
  // Top/middle/bottom
  switch(p_position & 0xF0)
  {
    case 0x10:  y = rect.top + TOAST_SPACE; 
                break;
    case 0x20:  y = rect.top + rect.Height() / 2 - size.Height() / 2;
                break;
    case 0x30:  y = rect.bottom - size.Height() - TOAST_SPACE;
                up = true;
                break;
    default:    y = 0;
                break;
  }

  // Left/Middle/Right
  switch(p_position & 0x0F)
  {
    case 1:   x = TOAST_SPACE;
              break;
    case 2:   x = rect.left + rect.Width() / 2 - size.Width() / 2;
              break;
    case 3:   x = rect.right - size.Width() - TOAST_SPACE;
              break;
    default:  x = 0;
              break;
  }

  // If more than one toast at once
  MoreThanOne(size.Height(),y,up);

  if(x == 0 || y == 0)
  {
    // Incorrect p_position
    ASSERT(FALSE);
  }
  toast->SetWindowPos(&CWnd::wndTopMost,x,y,0,0,SWP_NOSIZE | SWP_SHOWWINDOW);
  toast->SetForegroundWindow();
  toast->Invalidate();
  toast->PostMessage(WM_PAINT,0,0);
  toast->PumpMessage();

  if(focuswin)
  {
    focuswin->SetFocus();
  }

  // Tell of our success
  if(p_success)
  {
    *p_success = true;
  }

  // Only return the toast if we are prepared to destroy it later
  return p_timeout > 0 ? nullptr : toast;
}
