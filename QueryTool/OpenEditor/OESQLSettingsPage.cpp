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
  m_comboSqlLen.AddString(_T("Statement NTS (Null Terminated String"));
  m_comboSqlLen.AddString(_T("Statement exact string length"));
  m_comboSqlLen.AddString(_T("Statement string-length + 1 for NTS (Default)"));

  m_comboTranslation.AddString(_T("No action taken (default)"));
  m_comboTranslation.AddString(_T("Use database default charset"));
  m_comboTranslation.AddString(_T("Use charset translation"));

  if(m_lenOption < 1 || m_lenOption > 3)
  {
    m_lenOption = 3;
  }

  m_comboSqlLen.SetCurSel(m_lenOption - 1);
  m_comboTranslation.SetCurSel(m_charsetTranslation);

  // CHARSET
  m_comboCharset.SetMaxDroppedWidth(200);

  m_comboCharset.AddString(_T(""));
  m_comboCharset.AddString(_T("utf-8")              _T(" - Unicode (UTF-8)"));
  m_comboCharset.AddString(_T("utf-16")             _T(" - Unicode UTF-16, little endian byte order (BMP of ISO 10646);"));
  m_comboCharset.AddString(_T("unicodeFFFE")        _T(" - Unicode UTF-16, big endian byte order"));
  m_comboCharset.AddString(_T("utf-32")             _T(" - Unicode UTF-32, little endian byte order"));
  m_comboCharset.AddString(_T("utf-32BE")           _T(" - Unicode UTF-32, big endian byte order"));
  m_comboCharset.AddString(_T("utf-7")              _T(" - Unicode (UTF-7)"));
  m_comboCharset.AddString(_T("windows-1250")       _T(" - ANSI Central European; Central European (Windows) "));
  m_comboCharset.AddString(_T("windows-1251")       _T(" - ANSI Cyrillic; Cyrillic (Windows)"));
  m_comboCharset.AddString(_T("windows-1252")       _T(" - ANSI Latin 1; Western European (Windows)"));
  m_comboCharset.AddString(_T("windows-1253")       _T(" - ANSI Greek; Greek (Windows)"));
  m_comboCharset.AddString(_T("windows-1254")       _T(" - ANSI Turkish; Turkish (Windows)"));
  m_comboCharset.AddString(_T("windows-1255")       _T(" - ANSI Hebrew; Hebrew (Windows)"));
  m_comboCharset.AddString(_T("windows-1256")       _T(" - ANSI Arabic; Arabic (Windows)"));
  m_comboCharset.AddString(_T("windows-1257")       _T(" - ANSI Baltic; Baltic (Windows)"));
  m_comboCharset.AddString(_T("windows-1258")       _T(" - ANSI/OEM Vietnamese; Vietnamese (Windows)"));
  m_comboCharset.AddString(_T("iso-8859-1")         _T(" - ISO 8859-1 Latin 1; Western European (ISO)"));
  m_comboCharset.AddString(_T("iso-8859-2")         _T(" - ISO 8859-2 Central European; Central European (ISO)"));
  m_comboCharset.AddString(_T("iso-8859-3")         _T(" - ISO 8859-3 Latin 3 "));
  m_comboCharset.AddString(_T("iso-8859-4")         _T(" - ISO 8859-4 Baltic"));
  m_comboCharset.AddString(_T("iso-8859-5")         _T(" - ISO 8859-5 Cyrillic"));
  m_comboCharset.AddString(_T("iso-8859-6")         _T(" - ISO 8859-6 Arabic"));
  m_comboCharset.AddString(_T("iso-8859-7")         _T(" - ISO 8859-7 Greek"));
  m_comboCharset.AddString(_T("iso-8859-8")         _T(" - ISO 8859-8 Hebrew; Hebrew (ISO-Visual)"));
  m_comboCharset.AddString(_T("iso-8859-9")         _T(" - ISO 8859-9 Turkish"));
  m_comboCharset.AddString(_T("iso-8859-13")        _T(" - ISO 8859-13 Estonian"));
  m_comboCharset.AddString(_T("iso-8859-15")        _T(" - ISO 8859-15 Latin 9"));
  m_comboCharset.AddString(_T("IBM037")             _T(" - IBM EBCDIC US-Canada"));
  m_comboCharset.AddString(_T("IBM437")             _T(" - OEM United States"));
  m_comboCharset.AddString(_T("IBM500")             _T(" - IBM EBCDIC International"));
  m_comboCharset.AddString(_T("IBM1026")            _T(" - IBM EBCDIC Turkish (Latin 5)"));
  m_comboCharset.AddString(_T("IBM01047")           _T(" - IBM EBCDIC Latin 1/Open System"));
  m_comboCharset.AddString(_T("IBM01140")           _T(" - IBM EBCDIC US-Canada (037 + Euro symbol); IBM EBCDIC (US-Canada-Euro)"));
  m_comboCharset.AddString(_T("IBM01141")           _T(" - IBM EBCDIC Germany (20273 + Euro symbol); IBM EBCDIC (Germany-Euro)"));
  m_comboCharset.AddString(_T("IBM01142")           _T(" - IBM EBCDIC Denmark-Norway (20277 + Euro symbol); IBM EBCDIC (Denmark-Norway-Euro)"));
  m_comboCharset.AddString(_T("IBM01143")           _T(" - IBM EBCDIC Finland-Sweden (20278 + Euro symbol); IBM EBCDIC (Finland-Sweden-Euro)"));
  m_comboCharset.AddString(_T("IBM01144")           _T(" - IBM EBCDIC Italy (20280 + Euro symbol); IBM EBCDIC (Italy-Euro)"));
  m_comboCharset.AddString(_T("IBM01145")           _T(" - IBM EBCDIC Latin America-Spain (20284 + Euro symbol); IBM EBCDIC (Spain-Euro)"));
  m_comboCharset.AddString(_T("IBM01146")           _T(" - IBM EBCDIC United Kingdom (20285 + Euro symbol); IBM EBCDIC (UK-Euro)"));
  m_comboCharset.AddString(_T("IBM01147")           _T(" - IBM EBCDIC France (20297 + Euro symbol); IBM EBCDIC (France-Euro)"));
  m_comboCharset.AddString(_T("IBM01148")           _T(" - IBM EBCDIC International (500 + Euro symbol); IBM EBCDIC (International-Euro)"));
  m_comboCharset.AddString(_T("IBM01149")           _T(" - IBM EBCDIC Icelandic (20871 + Euro symbol); IBM EBCDIC (Icelandic-Euro)"));
  m_comboCharset.AddString(_T("us-ascii")           _T(" - US-ASCII (7-bit)"));
  m_comboCharset.AddString(_T("DOS-720")            _T(" - Arabic(Transparent ASMO); Arabic (DOS)"));
  m_comboCharset.AddString(_T("ibm737")             _T(" - OEM Greek (Formerly 437G); Greek (DOS)"));
  m_comboCharset.AddString(_T("ibm775")             _T(" - OEM Baltic; Baltic (DOS)"));
  m_comboCharset.AddString(_T("ibm850")             _T(" - OEM Multilingual Latin 1; Western European (DOS)"));
  m_comboCharset.AddString(_T("imb852")             _T(" - OEM Latin 2; Central European (DOS)"));
  m_comboCharset.AddString(_T("ibm855")             _T(" - OEM Cyrillic (primarily Russian)"));
  m_comboCharset.AddString(_T("ibm857")             _T(" - OEM Turkish; Turkish (DOS)"));
  m_comboCharset.AddString(_T("IBM00858")           _T(" - OEM Multilingual Latin 1 + Euro symbol"));
  m_comboCharset.AddString(_T("IBM860")             _T(" - OEM Portuguese; Portuguese (DOS)"));
  m_comboCharset.AddString(_T("ibm861")             _T(" - OEM Icelandic; Icelandic (DOS)"));
  m_comboCharset.AddString(_T("DOS-862")            _T(" - OEM Hebrew; Hebrew (DOS)"));
  m_comboCharset.AddString(_T("IBM863")             _T(" - OEM French Canadian; French Canadian (DOS)"));
  m_comboCharset.AddString(_T("IBM864")             _T(" - OEM Arabic; Arabic (864)"));
  m_comboCharset.AddString(_T("IBM865")             _T(" - OEM Nordic; Nordic (DOS)"));
  m_comboCharset.AddString(_T("cp866")              _T(" - OEM Russian; Cyrillic (DOS)"));
  m_comboCharset.AddString(_T("ibm869")             _T(" - OEM Modern Greek; Greek, Modern (DOS)"));
  m_comboCharset.AddString(_T("IBM870")             _T(" - IBM EBCDIC Multilingual/ROECE (Latin 2); IBM EBCDIC Multilingual Latin 2"));
  m_comboCharset.AddString(_T("windows-874")        _T(" - ANSI/OEM Thai (same as 28605, ISO 8859-15); Thai (Windows)"));
  m_comboCharset.AddString(_T("cp875")              _T(" - IBM EBCDIC Greek Modern"));
  m_comboCharset.AddString(_T("shift_jis")          _T(" - ANSI/OEM Japanese; Japanese (Shift-JIS)"));
  m_comboCharset.AddString(_T("gb2312")             _T(" - ANSI/OEM Simplified Chinese (PRC, Singapore); Chinese Simplified (GB2312)"));
  m_comboCharset.AddString(_T("ks_c_5601-1987")     _T(" - ANSI/OEM Korean (Unified Hangul Code)"));
  m_comboCharset.AddString(_T("big5")               _T(" - ANSI/OEM Traditional Chinese (Taiwan; Hong Kong SAR, PRC); Chinese Traditional (Big5)"));
  m_comboCharset.AddString(_T("Johab")              _T(" - Korean (Johab)"));
  m_comboCharset.AddString(_T("macintosh")          _T(" - MAC Roman; Western European (Mac)"));
  m_comboCharset.AddString(_T("x-mac-japanese")     _T(" - Japanese (Mac)"));
  m_comboCharset.AddString(_T("x-mac-chinesetrad")  _T(" - MAC Traditional Chinese (Big5); Chinese Traditional (Mac)"));
  m_comboCharset.AddString(_T("x-mac-korean")       _T(" - Korean (Mac)"));
  m_comboCharset.AddString(_T("x-mac-arabic")       _T(" - Arabic (Mac)"));
  m_comboCharset.AddString(_T("x-mac-hebrew")       _T(" - Hebrew (Mac)"));
  m_comboCharset.AddString(_T("x-mac-greek")        _T(" - Greek (Mac)"));
  m_comboCharset.AddString(_T("x-mac-cyrillic")     _T(" - Cyrillic (Mac)"));
  m_comboCharset.AddString(_T("x-mac-chinesesimp")  _T(" - MAC Simplified Chinese (GB 2312); Chinese Simplified (Mac)"));
  m_comboCharset.AddString(_T("x-mac-romanian")     _T(" - Romanian (Mac)"));
  m_comboCharset.AddString(_T("x-mac-ukranian")     _T(" - Ukranian (Mac)"));
  m_comboCharset.AddString(_T("x-mac-thai")         _T(" - Thai (Mac)"));
  m_comboCharset.AddString(_T("x-mac-ce")           _T(" - MAC Latin 2; Central European (Mac)"));
  m_comboCharset.AddString(_T("x-mac-icelandic")    _T(" - Icelandic (Mac)"));
  m_comboCharset.AddString(_T("x-mac-turkish")      _T(" - Turkish (Mac)"));
  m_comboCharset.AddString(_T("x-mac-croatian")     _T(" - Croatian (Mac)"));
  m_comboCharset.AddString(_T("x-Chinese_CNS")      _T(" - CNS Taiwan; Chinese Traditional (CNS) "));
  m_comboCharset.AddString(_T("x-cp20001")          _T(" - TCA Taiwan"));
  m_comboCharset.AddString(_T("x_Chinese-Eten")     _T(" - Eten Taiwan; Chinese Traditional (Eten)"));
  m_comboCharset.AddString(_T("x-cp20003")          _T(" - IBM5550 Taiwan"));
  m_comboCharset.AddString(_T("x-cp20004")          _T(" - TeleText Taiwan"));
  m_comboCharset.AddString(_T("x-cp20005")          _T(" - Want Taiwan"));
  m_comboCharset.AddString(_T("x-IA5")              _T(" - IA5 (IRV International Alphabet No. 5, 7-bit); Western European (IA5)"));
  m_comboCharset.AddString(_T("x-IA5-German")       _T(" - IA5 German (7-bit)"));
  m_comboCharset.AddString(_T("x-IA5-Swedish")      _T(" - IA5 Swedish (7-bit)"));
  m_comboCharset.AddString(_T("x-IA5-Norwegian")    _T(" - IA5 Norwegian (7-bit)"));
  m_comboCharset.AddString(_T("x-cp20261")          _T(" - T.61"));
  m_comboCharset.AddString(_T("x-cp20269")          _T(" - ISO 6937 Non-Spacing Accent"));
  m_comboCharset.AddString(_T("IBM273")             _T(" - IBM EBCDIC Germany"));
  m_comboCharset.AddString(_T("IBM277")             _T(" - IBM EBCDIC Denmark-Norway"));
  m_comboCharset.AddString(_T("IBM278")             _T(" - IBM EBCDIC Finland-Sweden"));
  m_comboCharset.AddString(_T("IBM280")             _T(" - IBM EBCDIC Italy"));
  m_comboCharset.AddString(_T("IBM284")             _T(" - IBM EBCDIC Latin America-Spain"));
  m_comboCharset.AddString(_T("IBM285")             _T(" - IBM EBCDIC United Kingdom"));
  m_comboCharset.AddString(_T("IBM290")             _T(" - IBM EBCDIC Japanese Katakana Extended"));
  m_comboCharset.AddString(_T("IBM297")             _T(" - IBM EBCDIC France"));
  m_comboCharset.AddString(_T("IBM420")             _T(" - IBM EBCDIC Arabic"));
  m_comboCharset.AddString(_T("IBM423")             _T(" - IBM EBCDIC Greek"));
  m_comboCharset.AddString(_T("IBM424")             _T(" - IBM EBCDIC Hebrew"));
  m_comboCharset.AddString(_T("x-EBCDIC-KoreanExtended") _T(" - IBM EBCDIC Korean Extended"));
  m_comboCharset.AddString(_T("IBM-Thai")           _T(" - IBM EBCDIC Thai"));
  m_comboCharset.AddString(_T("koi8-r")             _T(" - Russian (KOI8-R); Cyrillic (KOI8-R)"));
  m_comboCharset.AddString(_T("IBM871")             _T(" - IBM EBCDIC Icelandic"));
  m_comboCharset.AddString(_T("IBM880")             _T(" - IBM EBCDIC Cyrillic Russian"));
  m_comboCharset.AddString(_T("IMB905")             _T(" - IBM EBCDIC Turkish"));
  m_comboCharset.AddString(_T("IBM00924")           _T(" - IBM EBCDIC Latin 1/Open System (1047 + Euro symbol)"));
  m_comboCharset.AddString(_T("EUC-JP")             _T(" - Japanese (JIS 0208-1990 and 0121-1990)"));
  m_comboCharset.AddString(_T("x-cp20936")          _T(" - Simplified Chinese (GB2312); Chinese Simplified (GB2312-80)"));
  m_comboCharset.AddString(_T("x-cp20949")          _T(" - Korean Wansung"));
  m_comboCharset.AddString(_T("cp1025")             _T(" - IBM EBCDIC Cyrillic Serbian-Bulgarian"));
  m_comboCharset.AddString(_T("koi8-u")             _T(" - Ukrainian (KOI8-U); Cyrillic (KOI8-U)"));
  m_comboCharset.AddString(_T("x-Europa")           _T(" - Europa 3"));
  m_comboCharset.AddString(_T("iso-8859-8-i")       _T(" - ISO 8859-8 Hebrew; Hebrew (ISO-Logical)"));
  m_comboCharset.AddString(_T("iso-2022")           _T(" - ISO 2022 Japanese with no halfwidth Katakana; Japanese (JIS)"));
  m_comboCharset.AddString(_T("csISO20220JP")       _T(" - ISO 2022 Japanese with halfwidth Katakana; Japanese (JIS-Allow 1 byte Kana)"));
  m_comboCharset.AddString(_T("iso-2022-jp")        _T(" - ISO 2022 Japanese JIS X 0201-1989; Japanese (JIS-Allow 1 byte Kana - SO/SI)"));
  m_comboCharset.AddString(_T("iso-2022-kr")        _T(" - ISO 2022 Korean"));
  m_comboCharset.AddString(_T("x-cp50227")          _T(" - ISO 2022 Simplified Chinese; Chinese Simplified (ISO 2022)"));
  m_comboCharset.AddString(_T("euc-jp")             _T(" - EUC Japanese"));
  m_comboCharset.AddString(_T("EUC-CN")             _T(" - EUC Simplified Chinese; Chinese Simplified (EUC)"));
  m_comboCharset.AddString(_T("euc-kr")             _T(" - EUC Korean"));
  m_comboCharset.AddString(_T("hz-gb-2312")         _T(" - HZ-GB2312 Simplified Chinese; Chinese Simplified (HZ)"));
  m_comboCharset.AddString(_T("GB18030")            _T(" - GB18030 Simplified Chinese (4 byte); Chinese Simplified (GB18030)"));
  m_comboCharset.AddString(_T("x-iscii-de")         _T(" - ISCII Devanagari"));
  m_comboCharset.AddString(_T("x-iscii-be")         _T(" - ISCII Bengali"));
  m_comboCharset.AddString(_T("x-iscii-ta")         _T(" - ISCII Tamil"));
  m_comboCharset.AddString(_T("x-iscii-te")         _T(" - ISCII Telugu"));
  m_comboCharset.AddString(_T("x-iscii-as")         _T(" - ISCII Assamese"));
  m_comboCharset.AddString(_T("x-iscii-or")         _T(" - ISCII Oriya"));
  m_comboCharset.AddString(_T("x-iscii-ka")         _T(" - ISCII Kannada"));
  m_comboCharset.AddString(_T("x-iscii-ma")         _T(" - ISCII Malayalam"));
  m_comboCharset.AddString(_T("x-iscii-gu")         _T(" - ISCII Gujarati"));
  m_comboCharset.AddString(_T("x-iscii-pa")         _T(" - ISCII Punjabi"));

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
    m_terminator         = settings.GetSQLQueryTerminator();
    m_font               = settings.GetSQLQueryFont();
    m_odbcMetaSQL        = settings.GetPreferODBCMetaSQL();
    m_lenOption          = settings.GetSQLLengthOption();
    m_charsetTranslation = settings.GetSQLCharsetTranslation();
    m_charset            = settings.GetSQLCharsetUsed();
  }
  _OE_DEFAULT_HANDLER_;
}

void
COESQLSettingsPage::SaveSQLSettings()
{
  try
  {
    OpenEditor::GlobalSettings& settings = m_manager.GetGlobalSettings();

    CString term = m_terminator;
    CString font = m_font;

    settings.SetSQLPrefetchLines(m_prefetch);
    settings.SetSQLQueryTerminator(term);
    settings.SetSQLQueryFont(font);
    settings.SetPreferODBCMetaSQL(m_odbcMetaSQL);
    settings.SetSQLLengthOption(m_lenOption + 1);
    settings.SetSQLCharsetTranslation(m_charsetTranslation);
    settings.SetSQLCharsetUsed(m_charset.GetString());
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
    fontSize = _ttoi(m_font.Mid(pos+1));
  }
  if(fontName.IsEmpty())
  {
    fontName = _T("Tahoma");
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
  _tcscpy_s(lFont.lfFaceName,LF_FACESIZE,fontName);

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
    MessageBox(_T("You have chosen another font style than 'normal' (italic, bold etc).\n")
               _T("Regrettably: ODBCQueryTool ignores your choise and will do nothing with it.")
              ,_T("Warning")
              ,MB_OK|MB_ICONINFORMATION);
  }
  // Register the choice
  bool changed = false;
  if(_tcslen(lFont.lfFaceName) > 2)
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
    m_font.Format(_T("%s;%d"),fontName.GetString(),fontSize);
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
    int pos = charset.Find(_T(" - "));
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
