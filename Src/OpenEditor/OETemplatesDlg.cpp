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

#include "stdafx.h"
#include "resource.h"
#include "OpenEditor/OETemplatesDlg.h"

//using OpenEditor::Template::Entry;
using Common::VisualAttribute;

// COETemplatesDlg dialog

COETemplatesDlg::COETemplatesDlg (CWnd* pParent
                                 ,Mode mode
                                 ,OpenEditor::Template::Entry& entry
                                 ,const VisualAttribute& textAttr)
	: CDialog(COETemplatesDlg::IDD, pParent),
    m_mode(mode),
    m_entry(entry),
    m_textAttr(textAttr),
    m_cursorPosMarker('^')
{
    m_name         = m_entry.name.c_str();
    m_keyword      = m_entry.keyword.c_str();
    m_minKeyLength = m_entry.minLength;
    m_text         = m_entry.text.c_str();

    const char* ptr, *buff;
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

void COETemplatesDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_OETF_NAME, m_name);
    DDX_Text(pDX, IDC_OETF_KEYWORD, m_keyword);
    DDX_Text(pDX, IDC_OETF_TEXT, m_text);

    DDX_Text(pDX, IDC_OETF_MIN_LENGTH, m_minKeyLength);
    DDV_MinMaxInt(pDX, m_minKeyLength, 0, 999);
    SendDlgItemMessage(IDC_OETF_MIN_LENGTH_SPIN, UDM_SETRANGE32, 0, 999);
}


BEGIN_MESSAGE_MAP(COETemplatesDlg, CDialog)
    ON_BN_CLICKED(IDOK, OnOk)
END_MESSAGE_MAP()


// COETemplatesDlg message handlers

BOOL COETemplatesDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    LOGFONT logfont;
    memset(&logfont, 0, sizeof(logfont));

    logfont.lfHeight         = -10 * m_textAttr.m_FontSize;
    logfont.lfCharSet        = DEFAULT_CHARSET;
    logfont.lfOutPrecision   = CLIP_DEFAULT_PRECIS;
    logfont.lfClipPrecision  = CLIP_DEFAULT_PRECIS;
    logfont.lfQuality        = DEFAULT_QUALITY;
    logfont.lfPitchAndFamily = FIXED_PITCH;
    strncpy(logfont.lfFaceName, m_textAttr.m_FontName.c_str(), LF_FACESIZE-1);


    CWnd* pWndText = GetDlgItem(IDC_OETF_TEXT);
    CClientDC dc(pWndText);
    m_textFont.CreatePointFontIndirect(&logfont, &dc);
    pWndText->SetFont(&m_textFont);

    switch (m_mode)
    {
    case emInsert:
        SetWindowText("Insert Template");
        break;
    case emEdit:
        SetWindowText("Edit Template");
        break;
    case emDelete:
        SetWindowText("Delete Template");
        ::EnableWindow(::GetDlgItem(*this, IDC_OETF_NAME), FALSE);
        ::EnableWindow(::GetDlgItem(*this, IDC_OETF_KEYWORD), FALSE);
        ::EnableWindow(::GetDlgItem(*this, IDC_OETF_MIN_LENGTH), FALSE);
        ::EnableWindow(::GetDlgItem(*this, IDC_OETF_TEXT), FALSE);
        break;
    }

    return TRUE;
}

void COETemplatesDlg::OnOk()
{
    OnOK();

    m_entry.name = m_name;
    m_entry.keyword = m_keyword;
    m_entry.minLength = m_minKeyLength;
    
    m_entry.curPos = m_entry.curLine = 0;
    const char* buff = m_text.LockBuffer();
    const char* pos = strchr(buff, m_cursorPosMarker);
    
    for (const char* ptr = buff; pos && ptr < pos; ptr++, m_entry.curPos++)
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
        m_text.Delete((int)(pos - buff), 1);

    m_entry.text = m_text;
}
