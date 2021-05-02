/*
Copyright (C) 2004 - 2017 Edwig Huisman

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#include "stdafx.h"
#include "WideMessageBox.h"

#ifndef MB_CANCELTRYCONTINUE
#define MB_CANCELTRYCONTINUE 0x006L
#endif

// Measure the extents of a string of text in a specified font
// by drawing it on an off-screen DC and by doing so,
// getting the outer measures in a CRect.
static BOOL 
_WMB_CalcTextSize(const CString& strText, CSize& sizeText, CFont* font)
{
  BOOL result = FALSE;

  if (strText.IsEmpty() || font->m_hObject == NULL)
  {
    sizeText = CSize(0,0);
    return FALSE;
  }
  CWindowDC dc(NULL);
  CFont* pFontPrev = dc.SelectObject(font);

  CRect rectText = CRect(CPoint(0,0),sizeText);
  if (dc.DrawText(strText, &rectText,DT_CALCRECT|DT_LEFT|DT_NOPREFIX|DT_TOP|DT_WORDBREAK) != 0)
  {
    sizeText.cx = rectText.right;
    sizeText.cy = rectText.bottom;
    dc.SelectObject(pFontPrev);
    result = TRUE;
  }
  else
  {
    sizeText = CSize(0,0);
  }
  // Reset the font
  dc.SelectObject(pFontPrev);
  return result;
}

// Convert pixels to DLU's
// See the MSDN documentation at "GetDialogBaseUnits"
static CPoint 
_WMB_Pix2Dlu(int pixX, int pixY)
{
  CPoint baseXY(::GetDialogBaseUnits());
  CPoint dluXY;
  dluXY.x = ::MulDiv(pixX, 4, baseXY.x);
  dluXY.y = ::MulDiv(pixY, 8, baseXY.y);
  return dluXY;
}

static bool loaded = false;
typedef HRESULT (WINAPI* TDIF)(const TASKDIALOGCONFIG*,int*,int*,BOOL*);
TDIF TaskDialogIndirectFunc = NULL;

static bool
LoadCommonControls()
{
  if(loaded)
  {
    return true;
  }
  HMODULE mod = ::LoadLibrary("comctl32.dll");
  if(mod)
  {
    TaskDialogIndirectFunc = (TDIF) GetProcAddress(mod,"TaskDialogIndirect");
  }
  if(TaskDialogIndirectFunc)
  {
    return (loaded = true);
  }
  return false;
}

// WideMessageBox: The replacement of ::MessageBox on MS-Vista
// Does auto-sizing in width on basis of the p_message text
int 
WideMessageBox(HWND        p_hwnd
              ,const char* p_message
              ,const char* p_title
              ,int         p_buttons /* = MB_OK */)
{
  if(LoadCommonControls() == false)
  {
    // Still no TaskDialog function available
    return ::MessageBox(p_hwnd,p_message,p_title,p_buttons);
  }
  USES_CONVERSION; // For title and message
  TASKDIALOGCONFIG config;
  int pressed = 0;

  // Prepare taskdialog's config structure
  memset(&config,0,sizeof(TASKDIALOGCONFIG));
  config.cbSize  = sizeof(TASKDIALOGCONFIG);

  // Parent message box to parent window
  config.hwndParent = p_hwnd;

  // Title of the box
  CComBSTR title = CT2CW(p_title);
  config.pszWindowTitle = title;

  // Determine which buttons to use
  switch(p_buttons & 0x0f)
  {
    case MB_OK:                 config.dwCommonButtons = TDCBF_OK_BUTTON;
                                break;
    case MB_YESNO:              config.dwCommonButtons = TDCBF_YES_BUTTON | TDCBF_NO_BUTTON;
                                break;
    case MB_YESNOCANCEL:        config.dwCommonButtons = TDCBF_YES_BUTTON | TDCBF_NO_BUTTON | TDCBF_CANCEL_BUTTON;
                                break;
    case MB_RETRYCANCEL:        config.dwCommonButtons = TDCBF_RETRY_BUTTON | TDCBF_CANCEL_BUTTON;
                                break;
    case MB_ABORTRETRYIGNORE:   // Fall through
    case MB_CANCELTRYCONTINUE:  // New Microsoft policy: Not a good user dialog exchange
                                // Don't do this. We don't support it here.
                                return ::MessageBox(p_hwnd,p_message,p_title,p_buttons);
  }
  // Determine which main icon to use
  switch(p_buttons & 0x0f0)
  {
    //   MB_ICONHAND:
    case MB_ICONERROR:        config.pszMainIcon = TD_ERROR_ICON;
                              break;
    //   MB_ICONEXCLAMATION
    case MB_ICONWARNING:      config.pszMainIcon = TD_WARNING_ICON;
                              break;
    //   MB_ICONASTERISK:
    case MB_ICONINFORMATION:  config.pszMainIcon = TD_INFORMATION_ICON;
                              break;
    case MB_ICONQUESTION:     // New Microsoft policy. Question icons are for help
                              config.pszMainIcon = TD_INFORMATION_ICON;
                              break;
    // MAKEINTRESOURCEW(value) are listed here
    // -4 TD_SHIELD_ICON    
    // -5 Shield icon       + blue band
    // -6 Update icon       + yellow band  
    // -7 Protection shield + red band
    // -8 Green shield      + green band
    // -9 Admin shield      + brown band
  }
  // Getting a standard Vista message font
  NONCLIENTMETRICS ncm;
  ncm.cbSize = sizeof(NONCLIENTMETRICS);
  ::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &ncm, 0);
  CFont font;
  font.DeleteObject();
  CWindowDC dc(NULL);
  if(_tcscmp(ncm.lfMenuFont.lfFaceName,_T("Segoe UI")) == 0)
  {
    ncm.lfMenuFont.lfQuality = 5;
  }
  ncm.lfMenuFont.lfWeight = FW_NORMAL;
  ncm.lfMenuFont.lfItalic = 0;
  font.CreateFontIndirect(&ncm.lfMenuFont);

  // Size of the text to be displayed
  CString text(p_message);
  CSize sizeText(4000,2000);
  if(_WMB_CalcTextSize(text,sizeText,&font))
  {
    // Left / Right margin in the dialog
    sizeText.cx += 20; 
    if(config.pszMainIcon)
    {
      // Main icon takes this amount of space
      sizeText.cx += 48;
    }
    // POLICY: WideMessageBox is at maximum 90% of the screen's width
    int maxWidth = GetSystemMetrics(SM_CXSCREEN) * 90 / 100;
    if(sizeText.cx > maxWidth)
    {
      sizeText.cx = maxWidth;
    }
    // Convert pixels to dialog-units (DLU's)
    CPoint sizeDLU = _WMB_Pix2Dlu(sizeText.cx,sizeText.cy);
    config.cxWidth = sizeDLU.x;
  }
  else
  {
    TRACE("WideMessageBox: Cannot get an off-screen measure of the text: %s",text);
  }
  // Support and correct all kind of mistakes from programmers
  text.Replace("\r\n","\n");
  text.Replace("\n","\r\n");
  CComBSTR mess  = CT2CW(text);
  config.pszContent = mess;

  // Do the task dialog box as a replacement of MessageBox
  HRESULT result = (*TaskDialogIndirectFunc)(&config    // How to config it
                                     ,&pressed   // Result of the box
                                     ,NULL       // No radio buttons used
                                     ,NULL);     // No check box result
  switch(result)
  {
    case S_OK:           break;
    case E_OUTOFMEMORY:  WideMessageBox(p_hwnd,"Out of memory","Error",MB_OK|MB_ICONERROR);
                         pressed = 0;
                         break;
    case E_INVALIDARG:   WideMessageBox(p_hwnd,"Wrong argument to TaskDialog","Error",MB_OK|MB_ICONERROR);
                         pressed = 0;
                         break;
    case E_FAIL:         WideMessageBox(p_hwnd,"Failure in TaskDialog","Error",MB_OK|MB_ICONERROR);
                         pressed = 0;
                         break;
    default:             WideMessageBox(p_hwnd,"Unknown error in TaskDialog","Error",MB_OK|MB_ICONERROR);
                         pressed = 0;
                         break;
  }
  return pressed;
}
