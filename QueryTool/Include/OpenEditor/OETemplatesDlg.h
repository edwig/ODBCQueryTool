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

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __OETemplatesDlg_h__
#define __OETemplatesDlg_h__

#include "resource.h"
#include "OETemplates.h"
#include "OETemplates.h"
#include "COMMON\VisualAttributes.h"


class COETemplatesDlg : public CDialog
{
public:
    enum Mode { emInsert, emEdit, emDelete };
public:
    COETemplatesDlg (CWnd* pParent, Mode mode, OpenEditor::Template::Entry&, const Common::VisualAttribute& textAttr);
	virtual ~COETemplatesDlg ();

	enum { IDD = IDD_OE_TEMPLATE_FORM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    afx_msg void OnOk();

	DECLARE_MESSAGE_MAP()

protected:
    Mode m_mode;
    OpenEditor::Template::Entry& m_entry;
    Common::VisualAttribute m_textAttr;
    CFont m_textFont;
    const char m_cursorPosMarker;

    CString m_name;
    CString m_keyword;
    int m_minKeyLength;
    CString m_text;
};


#endif//__OETemplatesDlg_h__