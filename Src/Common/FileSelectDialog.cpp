////////////////////////////////////////////////////////////////////////
//
// Bestand:      $Archive: /Pronto/pronto_ibs/FileSelectDialog.cpp $
// Auteurs:      Edwig Huisman
//
// Copyright (c) 1998 - 2005 Centric IT Solutions
// Alle rechten voorbehouden
//
// Laatste wijziging: $JustDate: 29-08-05 $
// Gewijzigd door:    $Author: Bsamwel $
// Versienummer:      $Revision: 7 $
//
#include "StdAfx.h"
#include "FileSelectDialog.h"
#include <afx.h>
#include <dlgs.h>

FileSelectDialog::FileSelectDialog(bool    p_open        // true = open, false = SaveAs
                              ,CString p_titel       // Titel van de dialoog
                              ,CString p_defext      // Default extension
                              ,CString p_bestand     // Default first file
                              ,int     p_vlaggen     // Standaard vlaggen
                              ,CString p_filter)     // Filter voor extensies
                              :m_open(p_open)
{
  // Oorspronkelijke CWD (Current Working Directory) registreren
  GetCurrentDirectory(MAX_PATH, m_origineleDir);
  strncpy(m_filter, p_filter.GetString(), 1024);
  strncpy(m_bestand,p_bestand.GetString(),MAX_PATH);
  strncpy(m_defext, p_defext.GetString(), 100);
  strncpy(m_titel,  p_titel.GetString(),  100);
  FilterString(m_filter);

  // Vul de filename structuur
  p_vlaggen |= OFN_ENABLESIZING | OFN_LONGNAMES | OFN_NOCHANGEDIR |  OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
  p_vlaggen &= ~(OFN_NODEREFERENCELINKS | OFN_NOLONGNAMES | OFN_NOTESTFILECREATE);

  m_ofn.lStructSize       = sizeof(OPENFILENAME);
  m_ofn.hwndOwner         = AfxGetApp()->GetMainWnd()->GetSafeHwnd();
  m_ofn.hInstance         = (HINSTANCE) GetWindowLongPtr(m_ofn.hwndOwner,GWLP_HINSTANCE);
  m_ofn.lpstrFile         = (LPSTR) m_bestand;
  m_ofn.lpstrDefExt       = (LPSTR) m_defext;
  m_ofn.lpstrTitle        = (LPSTR) m_titel;
  m_ofn.lpstrFilter       = (LPSTR) m_filter;
  m_ofn.Flags             = p_vlaggen;
  m_ofn.nFilterIndex      = 1;    // Gebruik lpstrFilter
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
  // Ga terug naar de oorspronkelijke directory
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
    WideMessageBox(NULL,"Cannot create a file dialog","Error",MB_OK | MB_TASKMODAL);
  }
  return IDCANCEL;
}

CString 
FileSelectDialog::GetChosenFile()
{
  return (CString) m_ofn.lpstrFile;
}

void
FileSelectDialog::FilterString(char *filter)
{
  char *pnt = filter;
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
