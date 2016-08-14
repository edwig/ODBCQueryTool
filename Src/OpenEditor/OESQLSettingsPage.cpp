/* 
    Copyright (C) 2005 Edwig Huisman

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
#include <COMMON/ExceptionHelper.h>
#include <Common/DirSelectDlg.h>
#include "OpenEditor/OESQLSettingsPage.h"

COESQLSettingsPage::COESQLSettingsPage (SettingsManager& manager)
	                 :CPropertyPage(COESQLSettingsPage::IDD)
                   ,m_manager(manager)
{
    const OpenEditor::GlobalSettings& settings = m_manager.GetGlobalSettings();

    m_prefetchLines   = settings.GetSQLPrefetchLines();
    m_queryTerminator = settings.GetSQLQueryTerminator().c_str();
    m_queryFont       = settings.GetSQLQueryFont().c_str();
}

COESQLSettingsPage::~COESQLSettingsPage()
{
}

void COESQLSettingsPage::DoDataExchange(CDataExchange* pDX)
{
  DDX_Text(pDX,IDC_OE_SQLPREFETCHLINES,m_prefetchLines);
  DDX_Text(pDX,IDC_OE_SQLTERMINATOR,   m_queryTerminator);
  DDX_Text(pDX,IDC_FONT,               m_queryFont);
}


BEGIN_MESSAGE_MAP(COESQLSettingsPage, CPropertyPage)
//    ON_BN_CLICKED(IDC_OE_BACKUP,         OnUpdateData)
//    ON_BN_CLICKED(IDC_OE_BACKUP_TO_FILE, OnUpdateData)
//    ON_BN_CLICKED(IDC_OE_BACKUP_TO_DIR,  OnUpdateData)
//    ON_BN_CLICKED(IDC_OE_BACKUP_DIR_CHANGE, OnBnClicked_FileBackupDirChange)
ON_EN_CHANGE(IDC_FONT,      &COESQLSettingsPage::OnEnChangeFont)
ON_BN_CLICKED(IDC_BUT_FONT, &COESQLSettingsPage::OnBnClickedButFont)
END_MESSAGE_MAP()


// COESQLSettingsPage message handlers

BOOL COESQLSettingsPage::OnApply()
{
    try
    {
        OpenEditor::GlobalSettings& settings = m_manager.GetGlobalSettings();
        
        settings.SetSQLPrefetchLines(m_prefetchLines);
        std::string term = m_queryTerminator;
        settings.SetSQLQueryTerminator(term);
        std::string font = m_queryFont;
        settings.SetSQLQueryFont(font);
    }
    _OE_DEFAULT_HANDLER_;

	return TRUE;
}

void COESQLSettingsPage::OnUpdateData()
{
    UpdateData();
}


void 
COESQLSettingsPage::OnEnChangeFont()
{
  UpdateData();
}

void 
COESQLSettingsPage::OnBnClickedButFont()
{
  LOGFONT    lFont;
  CHOOSEFONT cFont;
  ZeroMemory(&lFont,sizeof(LOGFONT));
  ZeroMemory(&cFont,sizeof(CHOOSEFONT));

  // Split current font registry in name and size
  CString fontName;
  int     fontSize;
  int pos = m_queryFont.Find(';');
  if(pos)
  {
    fontName = m_queryFont.Left(pos);
    fontSize = atoi(m_queryFont.Mid(pos+1));
  }
  if(fontName.IsEmpty())
  {
    fontName = "Tahoma";
  }
  if(fontSize < 8 ) fontSize = 8;
  if(fontSize > 24) fontSize = 24;

  // Vul logic font op basis van huidig font
  lFont.lfHeight         = -MulDiv(fontSize, GetDeviceCaps(GetDC()->m_hDC, LOGPIXELSY),72);
  lFont.lfWidth          = 0;
  lFont.lfEscapement     = 0;
  lFont.lfOrientation    = 0;
  lFont.lfWeight         = FW_NORMAL;
  lFont.lfItalic         = FALSE;
  lFont.lfUnderline      = FALSE;
  lFont.lfStrikeOut      = FALSE;
  lFont.lfOutPrecision   = OUT_DEFAULT_PRECIS;
  lFont.lfClipPrecision  = CLIP_DEFAULT_PRECIS;
  lFont.lfQuality        = DRAFT_QUALITY;
  lFont.lfPitchAndFamily = FF_DONTCARE;  // FF_MODERN
  strcpy_s(lFont.lfFaceName,LF_FACESIZE,fontName);

  // Vul choosefont
  cFont.iPointSize  = fontSize;
  cFont.nSizeMin    = 8;
  cFont.nSizeMax    = 24;
  cFont.lStructSize = sizeof(CHOOSEFONT);
  cFont.hwndOwner   = GetSafeHwnd();
  cFont.lpLogFont   = &lFont;
  cFont.nFontType   = SCREEN_FONTTYPE|PRINTER_FONTTYPE|REGULAR_FONTTYPE;
  cFont.Flags       = CF_BOTH|CF_INITTOLOGFONTSTRUCT|CF_LIMITSIZE; // CF_FIXEDPITCHONLY

  // Kies nu een ander font
  if(!ChooseFont(&cFont))
  {
    // Geen ander font gekozen
    return;
  }
  // Controleer op overbodige keuze
  if(lFont.lfWeight    != FW_NORMAL ||
     lFont.lfItalic    != FALSE     ||
     lFont.lfUnderline != FALSE     ||
     lFont.lfStrikeOut != FALSE     )
  {
    MessageBox("You have chosen another font style than 'normal' (italic, bold etc).\n"
               "Regrettably: ODBCQueryTool ignores your choise and will do nothing with it."
              ,"Warning"
              ,MB_OK|MB_ICONINFORMATION);
  }
  // Registreer keuze
  bool changed = false;
  if(strlen(lFont.lfFaceName) > 2)
  {
    changed  = true;
    fontName = lFont.lfFaceName;
  }
  if(cFont.iPointSize >= 80 && cFont.iPointSize <= 240)
  {
    changed  = true;
    fontSize = cFont.iPointSize / 10;
  }
  if(changed)
  {
    // Show changes
    m_queryFont.Format("%s;%d",fontName,fontSize);
    UpdateData(FALSE);
  }
}
