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
                   ,m_prefetch(0)
                   ,m_lenOption(0)
                   ,m_odbcMetaSQL(false)
                   ,m_charsetTranslation(0)
{
}

COESQLSettingsPage::~COESQLSettingsPage()
{
}

void 
COESQLSettingsPage::DoDataExchange(CDataExchange* pDX)
{
  StyleDialog::DoDataExchange(pDX);

  DDX_Control(pDX,IDC_OE_SQLPREFETCHLINES,m_editPrefetch,    m_prefetch);
  DDX_Control(pDX,IDC_OE_SQLTERMINATOR,   m_editTerminator,  m_terminator);
  DDX_Control(pDX,IDC_FONT,               m_editFont,        m_font);
  DDX_Control(pDX,IDC_BUT_FONT,           m_buttonFont);
  DDX_Control(pDX,IDC_META,               m_buttonODBC);
  DDX_CBIndex(pDX,IDC_SQL_LEN,            m_comboSqlLen,     m_lenOption);
  DDX_CBIndex(pDX,IDC_TRANSLATION,        m_comboTranslation,m_charsetTranslation);
  DDX_Control(pDX,IDC_CHARACTERSET,       m_comboCharset);

  m_comboCharset.EnableWindow(m_charsetTranslation == 2);
}

BEGIN_MESSAGE_MAP(COESQLSettingsPage,StyleDialog)
  ON_EN_CHANGE(IDC_OE_SQLPREFETCHLINES,&COESQLSettingsPage::OnEnChangePrefetch)
  ON_EN_CHANGE(IDC_OE_SQLTERMINATOR,   &COESQLSettingsPage::OnEnChangeTerminator)
  ON_EN_CHANGE(IDC_FONT,               &COESQLSettingsPage::OnEnChangeFont)
  ON_BN_CLICKED(IDC_BUT_FONT,          &COESQLSettingsPage::OnBnClickedButFont)
  ON_BN_CLICKED(IDC_META,              &COESQLSettingsPage::OnBnClickedPreferODBC)
  ON_CBN_SELCHANGE(IDC_SQL_LEN,        &COESQLSettingsPage::OnCbnSelchangeSqlLen)
  ON_CBN_SELCHANGE(IDC_TRANSLATION,    &COESQLSettingsPage::OnCbnSelchangeTranslation)
  ON_CBN_SELCHANGE(IDC_CHARACTERSET,   &COESQLSettingsPage::OnCbnSelchangeCharset)
END_MESSAGE_MAP()

BOOL
COESQLSettingsPage::OnInitDialog()
{
  StyleDialog::OnInitDialog();

  ReadSQLSettings();
  InitCombos();

  m_buttonODBC.SetCheck(m_odbcMetaSQL);

  UpdateData(FALSE);
  return TRUE;
}

void
COESQLSettingsPage::InitCombos()
{
  m_comboSqlLen.AddString("Statement NTS (Null Terminated String");
  m_comboSqlLen.AddString("Statement exact string length");
  m_comboSqlLen.AddString("Statement string-length + 1 for NTS (Default)");

  m_comboTranslation.AddString("No action taken (default)");
  m_comboTranslation.AddString("Use database default charset");
  m_comboTranslation.AddString("Use charset translation");

  if(m_lenOption < 1 || m_lenOption > 3)
  {
    m_lenOption = 3;
  }

  m_comboSqlLen.SetCurSel(m_lenOption - 1);
  m_comboTranslation.SetCurSel(m_charsetTranslation);

  // CHARSET
  m_comboCharset.SetMaxDroppedWidth(200);

  m_comboCharset.AddString("");
  m_comboCharset.AddString("utf-8"              " - Unicode (UTF-8)");
  m_comboCharset.AddString("utf-16"             " - Unicode UTF-16, little endian byte order (BMP of ISO 10646);");
  m_comboCharset.AddString("unicodeFFFE"        " - Unicode UTF-16, big endian byte order");
  m_comboCharset.AddString("utf-32"             " - Unicode UTF-32, little endian byte order");
  m_comboCharset.AddString("utf-32BE"           " - Unicode UTF-32, big endian byte order");
  m_comboCharset.AddString("utf-7"              " - Unicode (UTF-7)");
  m_comboCharset.AddString("windows-1250"       " - ANSI Central European; Central European (Windows) ");
  m_comboCharset.AddString("windows-1251"       " - ANSI Cyrillic; Cyrillic (Windows)");
  m_comboCharset.AddString("windows-1252"       " - ANSI Latin 1; Western European (Windows)");
  m_comboCharset.AddString("windows-1253"       " - ANSI Greek; Greek (Windows)");
  m_comboCharset.AddString("windows-1254"       " - ANSI Turkish; Turkish (Windows)");
  m_comboCharset.AddString("windows-1255"       " - ANSI Hebrew; Hebrew (Windows)");
  m_comboCharset.AddString("windows-1256"       " - ANSI Arabic; Arabic (Windows)");
  m_comboCharset.AddString("windows-1257"       " - ANSI Baltic; Baltic (Windows)");
  m_comboCharset.AddString("windows-1258"       " - ANSI/OEM Vietnamese; Vietnamese (Windows)");
  m_comboCharset.AddString("iso-8859-1"         " - ISO 8859-1 Latin 1; Western European (ISO)");
  m_comboCharset.AddString("iso-8859-2"         " - ISO 8859-2 Central European; Central European (ISO)");
  m_comboCharset.AddString("iso-8859-3"         " - ISO 8859-3 Latin 3 ");
  m_comboCharset.AddString("iso-8859-4"         " - ISO 8859-4 Baltic");
  m_comboCharset.AddString("iso-8859-5"         " - ISO 8859-5 Cyrillic");
  m_comboCharset.AddString("iso-8859-6"         " - ISO 8859-6 Arabic");
  m_comboCharset.AddString("iso-8859-7"         " - ISO 8859-7 Greek");
  m_comboCharset.AddString("iso-8859-8"         " - ISO 8859-8 Hebrew; Hebrew (ISO-Visual)");
  m_comboCharset.AddString("iso-8859-9"         " - ISO 8859-9 Turkish");
  m_comboCharset.AddString("iso-8859-13"        " - ISO 8859-13 Estonian");
  m_comboCharset.AddString("iso-8859-15"        " - ISO 8859-15 Latin 9");
  m_comboCharset.AddString("IBM037"             " - IBM EBCDIC US-Canada");
  m_comboCharset.AddString("IBM437"             " - OEM United States");
  m_comboCharset.AddString("IBM500"             " - IBM EBCDIC International");
  m_comboCharset.AddString("IBM1026"            " - IBM EBCDIC Turkish (Latin 5)");
  m_comboCharset.AddString("IBM01047"           " - IBM EBCDIC Latin 1/Open System");
  m_comboCharset.AddString("IBM01140"           " - IBM EBCDIC US-Canada (037 + Euro symbol); IBM EBCDIC (US-Canada-Euro)");
  m_comboCharset.AddString("IBM01141"           " - IBM EBCDIC Germany (20273 + Euro symbol); IBM EBCDIC (Germany-Euro)");
  m_comboCharset.AddString("IBM01142"           " - IBM EBCDIC Denmark-Norway (20277 + Euro symbol); IBM EBCDIC (Denmark-Norway-Euro)");
  m_comboCharset.AddString("IBM01143"           " - IBM EBCDIC Finland-Sweden (20278 + Euro symbol); IBM EBCDIC (Finland-Sweden-Euro)");
  m_comboCharset.AddString("IBM01144"           " - IBM EBCDIC Italy (20280 + Euro symbol); IBM EBCDIC (Italy-Euro)");
  m_comboCharset.AddString("IBM01145"           " - IBM EBCDIC Latin America-Spain (20284 + Euro symbol); IBM EBCDIC (Spain-Euro)");
  m_comboCharset.AddString("IBM01146"           " - IBM EBCDIC United Kingdom (20285 + Euro symbol); IBM EBCDIC (UK-Euro)");
  m_comboCharset.AddString("IBM01147"           " - IBM EBCDIC France (20297 + Euro symbol); IBM EBCDIC (France-Euro)");
  m_comboCharset.AddString("IBM01148"           " - IBM EBCDIC International (500 + Euro symbol); IBM EBCDIC (International-Euro)");
  m_comboCharset.AddString("IBM01149"           " - IBM EBCDIC Icelandic (20871 + Euro symbol); IBM EBCDIC (Icelandic-Euro)");
  m_comboCharset.AddString("us-ascii"           " - US-ASCII (7-bit)");
  m_comboCharset.AddString("DOS-720"            " - Arabic(Transparent ASMO); Arabic (DOS)");
  m_comboCharset.AddString("ibm737"             " - OEM Greek (Formerly 437G); Greek (DOS)");
  m_comboCharset.AddString("ibm775"             " - OEM Baltic; Baltic (DOS)");
  m_comboCharset.AddString("ibm850"             " - OEM Multilingual Latin 1; Western European (DOS)");
  m_comboCharset.AddString("imb852"             " - OEM Latin 2; Central European (DOS)");
  m_comboCharset.AddString("ibm855"             " - OEM Cyrillic (primarily Russian)");
  m_comboCharset.AddString("ibm857"             " - OEM Turkish; Turkish (DOS)");
  m_comboCharset.AddString("IBM00858"           " - OEM Multilingual Latin 1 + Euro symbol");
  m_comboCharset.AddString("IBM860"             " - OEM Portuguese; Portuguese (DOS)");
  m_comboCharset.AddString("ibm861"             " - OEM Icelandic; Icelandic (DOS)");
  m_comboCharset.AddString("DOS-862"            " - OEM Hebrew; Hebrew (DOS)");
  m_comboCharset.AddString("IBM863"             " - OEM French Canadian; French Canadian (DOS)");
  m_comboCharset.AddString("IBM864"             " - OEM Arabic; Arabic (864)");
  m_comboCharset.AddString("IBM865"             " - OEM Nordic; Nordic (DOS)");
  m_comboCharset.AddString("cp866"              " - OEM Russian; Cyrillic (DOS)");
  m_comboCharset.AddString("ibm869"             " - OEM Modern Greek; Greek, Modern (DOS)");
  m_comboCharset.AddString("IBM870"             " - IBM EBCDIC Multilingual/ROECE (Latin 2); IBM EBCDIC Multilingual Latin 2");
  m_comboCharset.AddString("windows-874"        " - ANSI/OEM Thai (same as 28605, ISO 8859-15); Thai (Windows)");
  m_comboCharset.AddString("cp875"              " - IBM EBCDIC Greek Modern");
  m_comboCharset.AddString("shift_jis"          " - ANSI/OEM Japanese; Japanese (Shift-JIS)");
  m_comboCharset.AddString("gb2312"             " - ANSI/OEM Simplified Chinese (PRC, Singapore); Chinese Simplified (GB2312)");
  m_comboCharset.AddString("ks_c_5601-1987"     " - ANSI/OEM Korean (Unified Hangul Code)");
  m_comboCharset.AddString("big5"               " - ANSI/OEM Traditional Chinese (Taiwan; Hong Kong SAR, PRC); Chinese Traditional (Big5)");
  m_comboCharset.AddString("Johab"              " - Korean (Johab)");
  m_comboCharset.AddString("macintosh"          " - MAC Roman; Western European (Mac)");
  m_comboCharset.AddString("x-mac-japanese"     " - Japanese (Mac)");
  m_comboCharset.AddString("x-mac-chinesetrad"  " - MAC Traditional Chinese (Big5); Chinese Traditional (Mac)");
  m_comboCharset.AddString("x-mac-korean"       " - Korean (Mac)");
  m_comboCharset.AddString("x-mac-arabic"       " - Arabic (Mac)");
  m_comboCharset.AddString("x-mac-hebrew"       " - Hebrew (Mac)");
  m_comboCharset.AddString("x-mac-greek"        " - Greek (Mac)");
  m_comboCharset.AddString("x-mac-cyrillic"     " - Cyrillic (Mac)");
  m_comboCharset.AddString("x-mac-chinesesimp"  " - MAC Simplified Chinese (GB 2312); Chinese Simplified (Mac)");
  m_comboCharset.AddString("x-mac-romanian"     " - Romanian (Mac)");
  m_comboCharset.AddString("x-mac-ukranian"     " - Ukranian (Mac)");
  m_comboCharset.AddString("x-mac-thai"         " - Thai (Mac)");
  m_comboCharset.AddString("x-mac-ce"           " - MAC Latin 2; Central European (Mac)");
  m_comboCharset.AddString("x-mac-icelandic"    " - Icelandic (Mac)");
  m_comboCharset.AddString("x-mac-turkish"      " - Turkish (Mac)");
  m_comboCharset.AddString("x-mac-croatian"     " - Croatian (Mac)");
  m_comboCharset.AddString("x-Chinese_CNS"      " - CNS Taiwan; Chinese Traditional (CNS) ");
  m_comboCharset.AddString("x-cp20001"          " - TCA Taiwan");
  m_comboCharset.AddString("x_Chinese-Eten"     " - Eten Taiwan; Chinese Traditional (Eten)");
  m_comboCharset.AddString("x-cp20003"          " - IBM5550 Taiwan");
  m_comboCharset.AddString("x-cp20004"          " - TeleText Taiwan");
  m_comboCharset.AddString("x-cp20005"          " - Want Taiwan");
  m_comboCharset.AddString("x-IA5"              " - IA5 (IRV International Alphabet No. 5, 7-bit); Western European (IA5)");
  m_comboCharset.AddString("x-IA5-German"       " - IA5 German (7-bit)");
  m_comboCharset.AddString("x-IA5-Swedish"      " - IA5 Swedish (7-bit)");
  m_comboCharset.AddString("x-IA5-Norwegian"    " - IA5 Norwegian (7-bit)");
  m_comboCharset.AddString("x-cp20261"          " - T.61");
  m_comboCharset.AddString("x-cp20269"          " - ISO 6937 Non-Spacing Accent");
  m_comboCharset.AddString("IBM273"             " - IBM EBCDIC Germany");
  m_comboCharset.AddString("IBM277"             " - IBM EBCDIC Denmark-Norway");
  m_comboCharset.AddString("IBM278"             " - IBM EBCDIC Finland-Sweden");
  m_comboCharset.AddString("IBM280"             " - IBM EBCDIC Italy");
  m_comboCharset.AddString("IBM284"             " - IBM EBCDIC Latin America-Spain");
  m_comboCharset.AddString("IBM285"             " - IBM EBCDIC United Kingdom");
  m_comboCharset.AddString("IBM290"             " - IBM EBCDIC Japanese Katakana Extended");
  m_comboCharset.AddString("IBM297"             " - IBM EBCDIC France");
  m_comboCharset.AddString("IBM420"             " - IBM EBCDIC Arabic");
  m_comboCharset.AddString("IBM423"             " - IBM EBCDIC Greek");
  m_comboCharset.AddString("IBM424"             " - IBM EBCDIC Hebrew");
  m_comboCharset.AddString("x-EBCDIC-KoreanExtended" " - IBM EBCDIC Korean Extended");
  m_comboCharset.AddString("IBM-Thai"           " - IBM EBCDIC Thai");
  m_comboCharset.AddString("koi8-r"             " - Russian (KOI8-R); Cyrillic (KOI8-R)");
  m_comboCharset.AddString("IBM871"             " - IBM EBCDIC Icelandic");
  m_comboCharset.AddString("IBM880"             " - IBM EBCDIC Cyrillic Russian");
  m_comboCharset.AddString("IMB905"             " - IBM EBCDIC Turkish");
  m_comboCharset.AddString("IBM00924"           " - IBM EBCDIC Latin 1/Open System (1047 + Euro symbol)");
  m_comboCharset.AddString("EUC-JP"             " - Japanese (JIS 0208-1990 and 0121-1990)");
  m_comboCharset.AddString("x-cp20936"          " - Simplified Chinese (GB2312); Chinese Simplified (GB2312-80)");
  m_comboCharset.AddString("x-cp20949"          " - Korean Wansung");
  m_comboCharset.AddString("cp1025"             " - IBM EBCDIC Cyrillic Serbian-Bulgarian");
  m_comboCharset.AddString("koi8-u"             " - Ukrainian (KOI8-U); Cyrillic (KOI8-U)");
  m_comboCharset.AddString("x-Europa"           " - Europa 3");
  m_comboCharset.AddString("iso-8859-8-i"       " - ISO 8859-8 Hebrew; Hebrew (ISO-Logical)");
  m_comboCharset.AddString("iso-2022"           " - ISO 2022 Japanese with no halfwidth Katakana; Japanese (JIS)");
  m_comboCharset.AddString("csISO20220JP"       " - ISO 2022 Japanese with halfwidth Katakana; Japanese (JIS-Allow 1 byte Kana)");
  m_comboCharset.AddString("iso-2022-jp"        " - ISO 2022 Japanese JIS X 0201-1989; Japanese (JIS-Allow 1 byte Kana - SO/SI)");
  m_comboCharset.AddString("iso-2022-kr"        " - ISO 2022 Korean");
  m_comboCharset.AddString("x-cp50227"          " - ISO 2022 Simplified Chinese; Chinese Simplified (ISO 2022)");
  m_comboCharset.AddString("euc-jp"             " - EUC Japanese");
  m_comboCharset.AddString("EUC-CN"             " - EUC Simplified Chinese; Chinese Simplified (EUC)");
  m_comboCharset.AddString("euc-kr"             " - EUC Korean");
  m_comboCharset.AddString("hz-gb-2312"         " - HZ-GB2312 Simplified Chinese; Chinese Simplified (HZ)");
  m_comboCharset.AddString("GB18030"            " - GB18030 Simplified Chinese (4 byte); Chinese Simplified (GB18030)");
  m_comboCharset.AddString("x-iscii-de"         " - ISCII Devanagari");
  m_comboCharset.AddString("x-iscii-be"         " - ISCII Bengali");
  m_comboCharset.AddString("x-iscii-ta"         " - ISCII Tamil");
  m_comboCharset.AddString("x-iscii-te"         " - ISCII Telugu");
  m_comboCharset.AddString("x-iscii-as"         " - ISCII Assamese");
  m_comboCharset.AddString("x-iscii-or"         " - ISCII Oriya");
  m_comboCharset.AddString("x-iscii-ka"         " - ISCII Kannada");
  m_comboCharset.AddString("x-iscii-ma"         " - ISCII Malayalam");
  m_comboCharset.AddString("x-iscii-gu"         " - ISCII Gujarati");
  m_comboCharset.AddString("x-iscii-pa"         " - ISCII Punjabi");

  int ind = m_comboCharset.FindString(-1,m_charset);
  if(ind >= 0)
  {
    m_comboCharset.SetCurSel(ind);
  }
}

void
COESQLSettingsPage::ReadSQLSettings()
{
  try
  {
    const OpenEditor::GlobalSettings& settings = m_manager.GetGlobalSettings();

    m_prefetch           = settings.GetSQLPrefetchLines();
    m_terminator         = settings.GetSQLQueryTerminator().c_str();
    m_font               = settings.GetSQLQueryFont().c_str();
    m_odbcMetaSQL        = settings.GetPreferODBCMetaSQL();
    m_lenOption          = settings.GetSQLLengthOption();
    m_charsetTranslation = settings.GetSQLCharsetTranslation();
    m_charset            = settings.GetSQLCharsetUsed().c_str();
  }
  _OE_DEFAULT_HANDLER_;
}

void
COESQLSettingsPage::SaveSQLSettings()
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
    settings.SetSQLCharsetTranslation(m_charsetTranslation);
    settings.SetSQLCharsetUsed(std::string(m_charset.GetString()));
  }
  _OE_DEFAULT_HANDLER_;
}


// COESQLSettingsPage message handlers

BOOL COESQLSettingsPage::OnApply()
{
  SaveSQLSettings();
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
COESQLSettingsPage::OnCbnSelchangeTranslation()
{
  UpdateData();
  if(m_charsetTranslation < 2)
  {
    m_charset.Empty();
    UpdateData(FALSE);
  }
}

void 
COESQLSettingsPage::OnCbnSelchangeCharset()
{
  int ind = m_comboCharset.GetCurSel();
  if(ind >= 0)
  {
    CString charset;
    m_comboCharset.GetLBText(ind,charset);
    int pos = charset.Find(" - ");
    if(pos > 0)
    {
      m_charset = charset.Left(pos);
    }
  }
}

void
COESQLSettingsPage::OnBnClickedPreferODBC()
{
  m_odbcMetaSQL = m_buttonODBC.GetCheck() > 0;
}
