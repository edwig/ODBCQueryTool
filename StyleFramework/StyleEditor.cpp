////////////////////////////////////////////////////////////////////////
//
// $Source: BRIEFInterface/StyleEditor.cpp $
// $Author: ehuisman $
// 
// Copyright (c) 1995 - 2020 Hercules Social Housing B.V.
// Alle rechten voorbehouden
//
#include "stdafx.h"
#include "StyleEditor.h"
#include "SkinScrollWnd.h"
#include "StyleFonts.h"
#include "StyleColors.h"
#include "MSOfficeSettings.h"
#include "Editor.h"
#include "WordEditor_Commands.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC( StyleEditor, CWnd )

// ClassWizard generated message map functions
BEGIN_MESSAGE_MAP( StyleEditor, CWnd )
END_MESSAGE_MAP()

// Construction/Destruction
StyleEditor::StyleEditor()
{
}

StyleEditor::~StyleEditor()
{
  // Forced close of edit window
  WordEditor_Close(m_hWnd, FALSE);
}

// Lees input als een HTML bestand
void
StyleEditor::ReadHTML(BOOL p_html)
{
  // Maak het mogelijk om bij de input file te scannen op HTML input, zodat we een HTML file
  // kunnen omzetten naar PDF
  WordEditor_SetFlags4(GetSafeHwnd(), p_html, TFLAG4_HTML_INPUT);
}

// Implementation
BOOL 
StyleEditor::Create(DWORD dwStyle,const RECT& rect,CWnd* pParentWnd,UINT nID)
{
  // Let op: Vensternaam leeglaten!
  return CWnd::Create(WORDEDITOR_CLASS, "", dwStyle, rect, pParentWnd, nID);
}

void
StyleEditor::Init()
{
  ModifyStyle(WS_BORDER,0);
  ModifyStyleEx(WS_EX_CLIENTEDGE,0);

  SetFont(&STYLEFONTS.DialogTextFont);
  SkinScrollWnd* skin = SkinWndScroll(this);
  skin->SetScrollbarBias(0);
}

SkinScrollWnd*
StyleEditor::GetSkin()
{
  return (SkinScrollWnd*)GetWindowLongPtr(GetSafeHwnd(),GWLP_USERDATA);
}

void
StyleEditor::DrawFrame()
{
  COLORREF color = ThemeColor::_Color1;
  SkinScrollWnd* skin = (SkinScrollWnd*)GetWindowLongPtr(m_hWnd,GWLP_USERDATA);
  if(skin)
  {
    skin->DrawFrame(color);
  }
}

void
StyleEditor::ReadFile(CString& p_file)
{
  // Eerst leeg maken
  WordEditor_SetReadOnly(m_hWnd, FALSE); //ID_NEW requests non read-only session
  WordEditor_SetModify(m_hWnd, FALSE);
  SendMessage(WM_COMMAND,ID_NEW, 0L);
  WordEditor_SetReadOnly(m_hWnd, TRUE); //ID_NEW requests non read-only session

  // Nieuwe bestand inlezen
  if (!p_file.IsEmpty())
  {
    WordEditor_ReadFile(m_hWnd, (LPBYTE)(const char*)p_file);
  }
}
