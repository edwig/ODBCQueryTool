////////////////////////////////////////////////////////////////////////
//
// $Source: DocumentsLibrary/Style/StyleEditor.h $
// $Author: ehuisman $
// 
// Copyright (c) 1995 - 2020 Hercules Social Housing B.V.
// Alle rechten voorbehouden
//
#pragma once

class SkinScrollWnd;

/////////////////////////////////////////////////////////////////////////////
// StyleEditor Declaration

class StyleEditor : public CWnd
{
  DECLARE_DYNAMIC( StyleEditor )

public:
  // Construction/Destruction
  StyleEditor();
  virtual ~StyleEditor();
  
  void            Init();
  SkinScrollWnd*  GetSkin();
  void            DrawFrame();

  // Lees input als een HTML bestand
  void  ReadHTML(BOOL p_html);
  // Implementation
  BOOL	Create(DWORD dwstyle,const RECT& rect,CWnd* pParentWnd,UINT nID);
  // Read a file
  void  ReadFile(CString& p_file);

protected:
  DECLARE_MESSAGE_MAP()
};
