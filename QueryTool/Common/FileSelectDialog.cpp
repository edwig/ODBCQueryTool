////////////////////////////////////////////////////////////////////////
//
// File:   FileSelectDialog.cpp
// Author: Edwig Huisman
//
#include "pch.h"
#include "FileSelectDialog.h"
#include <afx.h>
#include <dlgs.h>

FileSelectDialog::FileSelectDialog(bool    p_open        // true = open, false = SaveAs
                                  ,CString p_titel       // Title of the dialog
                                  ,CString p_defext      // Default extension
                                  ,CString p_bestand     // Default first file
                                  ,int     p_vlaggen     // Standard flags
                                  ,CString p_filter)     // Filter for extensions
                                  :m_open(p_open)
{
  // Register the CWD (Current Working Directory) 
  GetCurrentDirectory(MAX_PATH, m_origineleDir);
  _tcsncpy_s(m_filter, 1024,     p_filter.GetString(), 1024);
  _tcsncpy_s(m_bestand,MAX_PATH, p_bestand.GetString(),MAX_PATH);
  _tcsncpy_s(m_defext, 100,      p_defext.GetString(), 100);
  _tcsncpy_s(m_titel,  100,      p_titel.GetString(),  100);
  FilterString(m_filter);

  // Fill in the open-filename structure
  p_vlaggen |= OFN_ENABLESIZING | OFN_LONGNAMES | OFN_NOCHANGEDIR |  OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
  p_vlaggen &= ~(OFN_NODEREFERENCELINKS | OFN_NOLONGNAMES | OFN_NOTESTFILECREATE);

  m_ofn.lStructSize       = sizeof(OPENFILENAME);
  m_ofn.hwndOwner         = AfxGetApp()->GetMainWnd()->GetSafeHwnd();
  m_ofn.hInstance         = (HINSTANCE) GetWindowLongPtr(m_ofn.hwndOwner,GWLP_HINSTANCE);
  m_ofn.lpstrFile         = (LPTSTR) m_bestand;
  m_ofn.lpstrDefExt       = (LPTSTR) m_defext;
  m_ofn.lpstrTitle        = (LPTSTR) m_titel;
  m_ofn.lpstrFilter       = (LPTSTR) m_filter;
  m_ofn.Flags             = p_vlaggen;
  m_ofn.nFilterIndex      = 1;    // Use lpstrFilter
  m_ofn.nMaxFile          = MAX_PATH;
  m_ofn.lpstrCustomFilter = NULL; //(LPSTR) buf_filter;
  m_ofn.nMaxCustFilter    = 0;
  m_ofn.lpstrFileTitle    = NULL;
  m_ofn.nMaxFileTitle     = 0;
  m_ofn.lpstrInitialDir   = NULL;
  m_ofn.nFileOffset       = 0;
  m_ofn.lCustData         = NULL;
  m_ofn.lpfnHook          = NULL;
  m_ofn.lpTemplateName    = NULL;
}

FileSelectDialog::~FileSelectDialog()
{
  // Go back to the original directory
  SetCurrentDirectory(m_origineleDir);
}

int 
FileSelectDialog::DoModal()
{
  try
  {
    if(m_open)
    {
      return GetOpenFileName(&m_ofn);
    }
    else
    {
      return GetSaveFileName(&m_ofn);
    }
  }
  catch(...)
  {
    StyleMessageBox(nullptr,_T("Cannot create a file dialog"),_T("Error"),MB_OK | MB_TASKMODAL);
  }
  return IDCANCEL;
}

CString 
FileSelectDialog::GetChosenFile()
{
  return (CString) m_ofn.lpstrFile;
}

void
FileSelectDialog::FilterString(LPTSTR filter)
{
  LPTSTR pnt = filter;
  while(*pnt)
  {
    if(*pnt == '|')
    {
      *pnt = 0;
    }
    ++pnt;
  }
  *pnt++ = 0;
  *pnt++ = 0;
}
