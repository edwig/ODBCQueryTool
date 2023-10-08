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

#pragma once
#include "resource.h"
#include "OETemplates.h"
#include "OETemplates.h"
#include "COMMON\VisualAttributes.h"

class COETemplatesDlg : public StyleDialog
{
public:
    enum Mode { emInsert, emEdit, emDelete };
public:
  COETemplatesDlg(CWnd* pParent, Mode mode, OpenEditor::Template::Entry&, const Common::VisualAttribute& textAttr);
	virtual ~COETemplatesDlg ();

	enum { IDD = IDD_OE_TEMPLATE_FORM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual BOOL OnInitDialog() override;
  afx_msg void OnOk();

	DECLARE_MESSAGE_MAP()

protected:
    Mode m_mode;
    OpenEditor::Template::Entry& m_entry;
    Common::VisualAttribute m_textAttr;
    CFont m_textFont;
    const TCHAR m_cursorPosMarker;

    CString m_name;
    CString m_keyword;
    int     m_minKeyLength;
    CString m_text;

    StyleEdit   m_editName;
    StyleEdit   m_editKeyword;
    StyleEdit   m_editMinLength;
    StyleEdit   m_editText;
    StyleSpinButtonCtrl m_spinLength;
    StyleButton         m_buttonOK;
    StyleButton         m_buttonCancel;
};
