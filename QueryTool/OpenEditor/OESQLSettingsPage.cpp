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

#include "pch.h"
#include <COMMON/ExceptionHelper.h>
#include <Common/DirSelectDlg.h>
#include "OESQLSettingsPage.h"
#include "QueryTool.h"

COESQLSettingsPage::COESQLSettingsPage(SettingsManager& manager,CWnd* p_parent)
	                 :StyleDialog(COESQLSettingsPage::IDD,p_parent)
                   ,m_manager(manager)
{
  const OpenEditor::GlobalSettings& settings = m_manager.GetGlobalSettings();

  m_prefetch    = settings.GetSQLPrefetchLines();
  m_terminator  = settings.GetSQLQueryTerminator().c_str();
  m_font        = settings.GetSQLQueryFont().c_str();
  m_odbcMetaSQL = settings.GetPreferODBCMetaSQL();
  m_lenOption   = settings.GetSQLLengthOption();
}

COESQLSettingsPage::~COESQLSettingsPage()
{
}

void 
COESQLSettingsPage::DoDataExchange(CDataExchange* pDX)
{
  StyleDialog::DoDataExchange(pDX);

  DDX_Control(pDX,IDC_OE_SQLPREFETCHLINES,m_editPrefetch,  m_prefetch);
  DDX_Control(pDX,IDC_OE_SQLTERMINATOR,   m_editTerminator,m_terminator);
  DDX_Control(pDX,IDC_FONT,               m_editFont,      m_font);
  DDX_Control(pDX,IDC_BUT_FONT,           m_buttonFont);
  DDX_Control(pDX,IDC_META,               m_buttonODBC);
  DDX_CBIndex(pDX,IDC_SQL_LEN,            m_comboSqlLen,   m_lenOption);
}

BEGIN_MESSAGE_MAP(COESQLSettingsPage,StyleDialog)
  ON_EN_CHANGE(IDC_OE_SQLPREFETCHLINES,&COESQLSettingsPage::OnEnChangePrefetch)
  ON_EN_CHANGE(IDC_OE_SQLTERMINATOR,   &COESQLSettingsPage::OnEnChangeTerminator)
  ON_EN_CHANGE(IDC_FONT,               &COESQLSettingsPage::OnEnChangeFont)
  ON_BN_CLICKED(IDC_BUT_FONT,          &COESQLSettingsPage::OnBnClickedButFont)
  ON_BN_CLICKED(IDC_META,              &COESQLSettingsPage::OnBnClickedPreferODBC)
  ON_CBN_SELCHANGE(IDC_SQL_LEN,        &COESQLSettingsPage::OnCbnSelchangeSqlLen)
END_MESSAGE_MAP()

BOOL
COESQLSettingsPage::OnInitDialog()
{
  StyleDialog::OnInitDialog();

  m_comboSqlLen.AddString("Statement NTS (Null Terminated String");
  m_comboSqlLen.AddString("Statement exact string length");
  m_comboSqlLen.AddString("Statement string-length + 1 for NTS (Default)");

  if(m_lenOption < 1 || m_lenOption > 3)
  {
    m_lenOption = 3;
  }

  m_comboSqlLen.SetCurSel(m_lenOption - 1);

  m_buttonODBC.SetCheck(m_odbcMetaSQL);
  return TRUE;
}

// COESQLSettingsPage message handlers

BOOL COESQLSettingsPage::OnApply()
{
  try
  {
    OpenEditor::GlobalSettings& settings = m_manager.GetGlobalSettings();
        
    std::string term = m_terminator;
    std::string font = m_font;

    settings.SetSQLPrefetchLines(m_prefetch);
    settings.SetSQLQueryTerminator(term);
    settings.SetSQLQueryFont(font);
    settings.SetPreferODBCMetaSQL(m_odbcMetaSQL);
    settings.SetSQLLengthOption(m_lenOption + 1);
  }
  _OE_DEFAULT_HANDLER_;

	return TRUE;
}

void COESQLSettingsPage::OnUpdateData()
{
    UpdateData();
}

void
COESQLSettingsPage::OnEnChangePrefetch()
{
  UpdateData();
}

void
COESQLSettingsPage::OnEnChangeTerminator()
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
  int pos = m_font.Find(';');
  if(pos)
  {
    fontName = m_font.Left(pos);
    fontSize = atoi(m_font.Mid(pos+1));
  }
  if(fontName.IsEmpty())
  {
    fontName = "Tahoma";
  }
  if(fontSize < 8 ) fontSize = 8;
  if(fontSize > 24) fontSize = 24;

  // Fill in logic font on the basis of the current font
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

  // Fill in font to choose
  cFont.iPointSize  = fontSize;
  cFont.nSizeMin    = 8;
  cFont.nSizeMax    = 24;
  cFont.lStructSize = sizeof(CHOOSEFONT);
  cFont.hwndOwner   = GetSafeHwnd();
  cFont.lpLogFont   = &lFont;
  cFont.nFontType   = SCREEN_FONTTYPE|PRINTER_FONTTYPE|REGULAR_FONTTYPE;
  cFont.Flags       = CF_BOTH|CF_INITTOLOGFONTSTRUCT|CF_LIMITSIZE; // CF_FIXEDPITCHONLY

  // Let the user choose another font
  if(!ChooseFont(&cFont))
  {
    // No other font chosen
    return;
  }
  // Check on redundant choices
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
  // Register the choice
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
    m_font.Format("%s;%d",fontName,fontSize);
    UpdateData(FALSE);
  }
}

void 
COESQLSettingsPage::OnCbnSelchangeSqlLen()
{
  UpdateData();
}

void
COESQLSettingsPage::OnBnClickedPreferODBC()
{
  m_odbcMetaSQL = m_buttonODBC.GetCheck() > 0;
}
