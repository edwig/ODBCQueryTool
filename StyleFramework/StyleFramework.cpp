////////////////////////////////////////////////////////////////////////
//
// File: StyleFrameWork.cpp
// Function: General header of the StylingFramework
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

// The one-and-only global styling object
StylingFramework g_styling;

StylingFramework::StylingFramework()
{
  if(m_instance || &g_styling != this)
  {
    throw CString(_T("SFX can only be initialized once!"));
  }
  m_instance = this;
  SFXCalculateDPI();
}

void
StylingFramework::SFXCalculateDPI()
{
  HDC screen = GetDC(0);
  int dpiX = GetDeviceCaps(screen,LOGPIXELSX);
  int dpiY = GetDeviceCaps(screen,LOGPIXELSY);

  m_factor_x = MulDiv(100,dpiX,96);
  m_factor_y = MulDiv(100,dpiY,96);

  STYLEFONTS.SetFactor(m_factor_y);

  ReleaseDC(0,screen);
}

bool
StylingFramework::SetSizeFactorX(int p_factor)
{
  if(p_factor > 50 && p_factor < 1000)
  {
    m_factor_x = p_factor;
    return true;
  }
  return false;
}

bool
StylingFramework::SetSizeFactorY(int p_factor)
{
  if(p_factor > 50 && p_factor < 1000)
  {
    m_factor_y = p_factor;
    return STYLEFONTS.SetFactor(p_factor);
  }
  return false;
}

int
StylingFramework::GetSizeFactorX()
{
  return m_factor_x;
}

int
StylingFramework::GetSizeFactorY()
{
  return m_factor_y;
}

// Setting the size factor
// To be called in your main program or InitInstance **BEFORE** you create any dialog or window
bool 
SetSFXSizeFactor(int p_factorX,int p_factorY)
{
  if(g_styling.SetSizeFactorX(p_factorY) && g_styling.SetSizeFactorY(p_factorY))
  {
    return true;
  }
  return false;
}

int GetSFXSizeFactor()
{
  return g_styling.GetSizeFactorY();
}

void
SFXResizeByFactor(CRect& p_rect)
{
  p_rect.top    = (p_rect.top    * g_styling.GetSizeFactorY()) / 100;
  p_rect.bottom = (p_rect.bottom * g_styling.GetSizeFactorY()) / 100;
  p_rect.left   = (p_rect.left   * g_styling.GetSizeFactorX()) / 100;
  p_rect.right  = (p_rect.right  * g_styling.GetSizeFactorX()) / 100;
}

void 
SFXResizeByFactor(int& p_x,int& p_y,int& p_w,int& p_h)
{
  p_x = (p_x * g_styling.GetSizeFactorX()) / 100;
  p_y = (p_y * g_styling.GetSizeFactorY()) / 100;
  p_w = (p_w * g_styling.GetSizeFactorX()) / 100;
  p_h = (p_h * g_styling.GetSizeFactorY()) / 100;
}
