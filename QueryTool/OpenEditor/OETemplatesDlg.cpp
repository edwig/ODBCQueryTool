/* 
    Copyright (C) 2002 Aleksey Kochetov

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

#include "pch.h"
#include "resource.h"
#include "OETemplatesDlg.h"

//using OpenEditor::Template::Entry;
using Common::VisualAttribute;

// COETemplatesDlg dialog

COETemplatesDlg::COETemplatesDlg(CWnd* pParent
                                ,Mode mode
                                ,OpenEditor::Template::Entry& entry
                                ,const VisualAttribute& textAttr)
	              :StyleDialog(COETemplatesDlg::IDD, pParent)
                ,m_mode(mode)
                ,m_entry(entry)
                ,m_textAttr(textAttr)
                ,m_cursorPosMarker('^')
{
  m_name         = m_entry.name;
  m_keyword      = m_entry.keyword;
  m_minKeyLength = m_entry.minLength;
  m_text         = m_entry.text;

  LPCTSTR ptr;
  LPCTSTR buff;
  buff = ptr = m_text.LockBuffer();
    
  for (int line = m_entry.curLine; ptr && line >= 0; ptr++)
  {
    if (*ptr == '\r') line--;
    if (!line) break;
  }

  ptr += m_entry.curPos + (m_entry.curLine ? 2 : 0);
  m_text.UnlockBuffer();
  m_text.Insert((int)(ptr - buff), m_cursorPosMarker);
}

COETemplatesDlg::~COETemplatesDlg()
{
}

void 
COETemplatesDlg::DoDataExchange(CDataExchange* pDX)
{
  StyleDialog::DoDataExchange(pDX);

  DDX_Control(pDX, IDC_OETF_NAME,           m_editName,     m_name);
  DDX_Control(pDX, IDC_OETF_KEYWORD,        m_editKeyword,  m_keyword);
  DDX_Control(pDX, IDC_OETF_MIN_LENGTH,     m_editMinLength,m_minKeyLength);
  DDX_Control(pDX, IDC_OETF_TEXT,           m_editText,     m_text);
  DDX_Control(pDX, IDC_OETF_MIN_LENGTH_SPIN,m_spinLength,  &m_editMinLength);
  DDX_Control(pDX, IDOK,                    m_buttonOK);
  DDX_Control(pDX, IDCANCEL,                m_buttonCancel);

  DDV_MinMaxInt(pDX, m_minKeyLength, 0, 999);
}

BEGIN_MESSAGE_MAP(COETemplatesDlg,StyleDialog)
  ON_BN_CLICKED(IDOK, OnOk)
END_MESSAGE_MAP()

// COETemplatesDlg message handlers

BOOL 
COETemplatesDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();

  LOGFONT logfont;
  memset(&logfont, 0, sizeof(logfont));

  logfont.lfHeight         = -10 * m_textAttr.m_FontSize;
  logfont.lfCharSet        = DEFAULT_CHARSET;
  logfont.lfOutPrecision   = CLIP_DEFAULT_PRECIS;
  logfont.lfClipPrecision  = CLIP_DEFAULT_PRECIS;
  logfont.lfQuality        = DEFAULT_QUALITY;
  logfont.lfPitchAndFamily = FIXED_PITCH;
    
  _tcsncpy_s(logfont.lfFaceName, m_textAttr.m_FontName.GetString(), LF_FACESIZE-1);


  CClientDC dc(&m_editText);
  m_textFont.CreatePointFontIndirect(&logfont, &dc);
  m_editText.SetFont(&m_textFont);

  switch (m_mode)
  {
    case emInsert:  SetWindowText(_T("Insert Template"));
                    break;
    case emEdit:    SetWindowText(_T("Edit Template"));
                    break;
    case emDelete:  SetWindowText(_T("Delete Template"));
                    m_editName     .EnableWindow(FALSE);
                    m_editKeyword  .EnableWindow(FALSE);
                    m_editMinLength.EnableWindow(FALSE);
                    m_editText     .EnableWindow(FALSE);
                    break;
  }
  m_spinLength.SetRange32(0,999);
  return TRUE;
}

void 
COETemplatesDlg::OnOk()
{
  OnOK();

  m_entry.name      = m_name;
  m_entry.keyword   = m_keyword;
  m_entry.minLength = m_minKeyLength;
    
  m_entry.curPos   = m_entry.curLine = 0;
  LPCTSTR buff = m_text.LockBuffer();
  LPCTSTR pos  = _tcsrchr(buff, m_cursorPosMarker);
    
  for (LPCTSTR ptr = buff; pos && ptr < pos; ptr++, m_entry.curPos++)
  {
    if (*ptr == '\r')
    {
      m_entry.curLine++;
      m_entry.curPos = 0;
    }
  }
  if (m_entry.curLine) m_entry.curPos -=2;
  m_text.UnlockBuffer();
    
  if (pos)
  {
    m_text.Delete((int)(pos - buff), 1);
  }
  m_entry.text = m_text;
}
