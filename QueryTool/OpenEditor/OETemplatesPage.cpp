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
#include <COMMON/ExceptionHelper.h>
#include "OETemplates.h"
#include "OETemplatesPage.h"
#include "OETemplatesDlg.h"
#include "OELanguageManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;
using namespace OpenEditor;

static struct 
{
    const char* title;
    int width;
    int format;
}
g_columns[] =
{
    { "Name",   170, LVCFMT_LEFT  },
    { "Key",    125, LVCFMT_LEFT  },
    { "MinLen",  60, LVCFMT_RIGHT },
};

/////////////////////////////////////////////////////////////////////////////
// COETemplatesPage property page

COETemplatesPage::COETemplatesPage(SettingsManager& manager,CWnd* p_parent)
                 :StyleDialog(COETemplatesPage::IDD,p_parent)
                 ,m_manager(manager)
{
}

COETemplatesPage::~COETemplatesPage()
{
}

int 
COETemplatesPage::getCurrentSelection () const
{
  POSITION pos = m_templateList.GetFirstSelectedItemPosition();
  if (pos)
  {
    return m_templateList.GetNextSelectedItem(pos);
  }
  throw std::logic_error("No current selection");
}

void 
COETemplatesPage::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);

  DDX_CBString(pDX,IDC_OET_CATEGORY,            m_comboTemplate,m_templateName);
  DDX_Control (pDX,IDC_OET_LIST_IF_ALTERNATIVE, m_alternative);
	DDX_Control (pDX,IDC_OET_TEMPL_LIST,          m_templateList);
}

BEGIN_MESSAGE_MAP(COETemplatesPage, StyleDialog)
  ON_NOTIFY(LVN_GETDISPINFO, IDC_OET_TEMPL_LIST, OnLvnGetdispinfo_TemplList)
  ON_BN_CLICKED(IDC_OET_ADD_TEMPL,            OnBnClicked_AddTempl)
  ON_BN_CLICKED(IDC_OET_EDIT_TEMPL,           OnBnClicked_EditTempl)
  ON_BN_CLICKED(IDC_OET_DEL_TEMPL,            OnBnClicked_DelTempl)
  ON_BN_CLICKED(IDC_OET_MOVE_DOWN_TEMPL,      OnBnClicked_MoveDownTempl)
  ON_BN_CLICKED(IDC_OET_MOVE_UP_TEMPL,        OnBnClicked_MoveUpTempl)
  ON_NOTIFY(NM_DBLCLK, IDC_OET_TEMPL_LIST,    OnNMDblclk_TemplList)
  ON_CBN_SELCHANGE(IDC_OET_CATEGORY,          OnCbnSelchange_Category)
  ON_BN_CLICKED(IDC_OET_LIST_IF_ALTERNATIVE,  OnBnClicked_ListIfAlternative)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COETemplatesPage message handlers

BOOL 
COETemplatesPage::OnInitDialog() 
{
	StyleDialog::OnInitDialog();

  try 
  {
    // All template categories
    m_comboTemplate.ResetContent();
        
    const OpenEditor::TemplateCollection& templColl = m_manager.GetTemplateCollection();
    OpenEditor::TemplateCollection::ConstIterator 
        it = templColl.Begin(),
        end = templColl.End();

    for(; it != end; it++)
    {
      m_comboTemplate.AddString(it->first);
    }
    m_comboTemplate.SetCurSel(0);
    OnCbnSelchange_Category();

    m_templateList.SetExtendedStyle(LVS_EX_FULLROWSELECT);

    LV_COLUMN lvcolumn;
    lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;

    for (int i = 0; i < sizeof g_columns/sizeof g_columns[0]; i++) 
    {
        lvcolumn.pszText  = (LPSTR)(LPCSTR)g_columns[i].title;
        lvcolumn.iSubItem = i;
        lvcolumn.fmt      = g_columns[i].format;
        lvcolumn.cx       = g_columns[i].width;
        m_templateList.InsertColumn(i, &lvcolumn);
    }
  }
  _OE_DEFAULT_HANDLER_;
	
	return TRUE;
}


void COETemplatesPage::OnLvnGetdispinfo_TemplList(NMHDR *pNMHDR, LRESULT *pResult)
{
  NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);

  try
  {
    static char buffer[40];

    const Template::Entry& entry 
        = m_currTemplate->GetEntry(pDispInfo->item.iItem);

    switch (pDispInfo->item.iSubItem)
    {
      case 0: pDispInfo->item.pszText = (LPSTR)(LPCSTR)entry.name;    break;
      case 1: pDispInfo->item.pszText = (LPSTR)(LPCSTR)entry.keyword; break;
      case 2: _itoa_s(entry.minLength, buffer, 40, 10);
              pDispInfo->item.pszText = (LPSTR)(LPCSTR)buffer; break;
    }
  }
  _OE_DEFAULT_HANDLER_;

  *pResult = 0;
}

void 
COETemplatesPage::OnBnClicked_AddTempl()
{
  Template::Entry entry;
  if (COETemplatesDlg(this, COETemplatesDlg::emInsert, entry, m_textAttr).DoModal() == IDOK)
  {
    LV_ITEM lvitem;
    memset(&lvitem, 0, sizeof lvitem);
    lvitem.iItem = !m_currTemplate->GetCount() ? 0 : getCurrentSelection() + 1;
    m_currTemplate->InsertEntry(lvitem.iItem, entry);
    insertTemplListline(lvitem);
    m_templateList.SetItemState(lvitem.iItem, LVIS_SELECTED, LVIS_SELECTED);
  }
}

void 
COETemplatesPage::OnBnClicked_EditTempl()
{
  int index = getCurrentSelection();
  Template::Entry entry = m_currTemplate->GetEntry(index);
  if (COETemplatesDlg(this, COETemplatesDlg::emEdit, entry, m_textAttr).DoModal() == IDOK)
  {
    m_currTemplate->UpdateEntry(index, entry);
    m_templateList.Update(index);
  }
}

void 
COETemplatesPage::OnBnClicked_DelTempl()
{
  int index = getCurrentSelection();
  Template::Entry entry = m_currTemplate->GetEntry(index);
  if (COETemplatesDlg(this, COETemplatesDlg::emDelete, entry, m_textAttr).DoModal() == IDOK)
  {
    m_currTemplate->DeleteEntry(index);
    m_templateList.DeleteItem(index);
    if (index < m_currTemplate->GetCount() || --index >= 0)
    {
      m_templateList.SetItemState(index, LVIS_SELECTED, LVIS_SELECTED);
    }
  }
}

void 
COETemplatesPage::OnBnClicked_MoveDownTempl()
{
  int index = getCurrentSelection();
  if (m_currTemplate->MoveDown(index))
  {
    m_templateList.SetItemState(index + 1, LVIS_SELECTED, LVIS_SELECTED);
  }
  else
  {
    MessageBeep((UINT)-1);
  }
  m_templateList.Invalidate();
}

void 
COETemplatesPage::OnBnClicked_MoveUpTempl()
{
  int index = getCurrentSelection();
  if (m_currTemplate->MoveUp(index))
  {
    m_templateList.SetItemState(index - 1, LVIS_SELECTED, LVIS_SELECTED);
  }
  else
  {
    MessageBeep((UINT)-1);
  }
}

void 
COETemplatesPage::OnNMDblclk_TemplList (NMHDR*, LRESULT *pResult)
{
  OnBnClicked_EditTempl();
  *pResult = 0;
}

void 
COETemplatesPage::OnCbnSelchange_Category()
{
  try 
  {
    UpdateData();
    m_templateList.DeleteAllItems();
    if(!m_templateName.IsEmpty())
    {
      m_currTemplate = m_manager.GetTemplateCollection().Find(m_templateName);

      LV_ITEM lvitem;
      memset(&lvitem, 0, sizeof lvitem);
      int ncount = m_currTemplate->GetCount();
      for (; lvitem.iItem < ncount; lvitem.iItem++)
      {
        insertTemplListline(lvitem);
      }
      if (ncount) 
      {
        m_templateList.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
      }
      m_textAttr = m_manager.FindByName(m_templateName).GetVisualAttributesSet().FindByName("Text");

      m_alternative.SetCheck(m_currTemplate->GetAlwaysListIfAlternative());
    }
  }
  _OE_DEFAULT_HANDLER_;
}

void 
COETemplatesPage::insertTemplListline (LV_ITEM& lvitem)
{
  lvitem.iSubItem = 0;
  lvitem.mask     = LVIF_TEXT;
  lvitem.pszText  = LPSTR_TEXTCALLBACK;
  lvitem.iItem    = m_templateList.InsertItem(&lvitem);

  lvitem.mask = LVIF_TEXT;
  for (lvitem.iSubItem = 1; lvitem.iSubItem < sizeof g_columns / sizeof g_columns[0]; lvitem.iSubItem++)
  {
    m_templateList.SetItem(&lvitem);
  }
}

LRESULT COETemplatesPage::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
  try
  {
    return StyleDialog::WindowProc(message, wParam, lParam);
  }
  _OE_DEFAULT_HANDLER_;

  return 0;
}

void 
COETemplatesPage::OnBnClicked_ListIfAlternative()
{
    m_currTemplate->SetAlwaysListIfAlternative(m_alternative.GetCheck() ? true : false);
}
