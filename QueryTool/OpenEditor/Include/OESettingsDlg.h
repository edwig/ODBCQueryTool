#pragma once
#include "OESettings.h"
#include "OEDocument.h"
#include "OEGeneralPage.h"
#include "OEFilePage.h"
#include "OEBackupPage.h"
#include "OEEditingPage.h"
#include "OEBlockPage.h"
#include "OEClassPage.h"
#include "OETemplatesPage.h"
#include "OESQLSettingsPage.h"
#include "Common\VisualAttributesPage.h"

using OpenEditor::SettingsManager;

class COESettingsDlg : public StyleDialog
{
public:
  COESettingsDlg(SettingsManager& p_settings,CWnd* p_parent);
  virtual ~COESettingsDlg();

private:
  virtual BOOL OnInitDialog() override;
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual void SetupDynamicLayout() override;

  void InitTabs();

  enum { IDD = IDD_OE_SETTINGS };

protected:
  DECLARE_MESSAGE_MAP()

  afx_msg void OnBnClickedOk();

private:
  SettingsManager&        m_settings;
  StyleTabCtrl            m_tabs;
  COEGeneralPage*         m_page1 { nullptr };
  COEFilePage*            m_page2 { nullptr };
  COEBackupPage*          m_page3 { nullptr };
  COEEditingPage*         m_page4 { nullptr };
  COEBlockPage*           m_page5 { nullptr };
  COEClassPage*           m_page6 { nullptr };
  COETemplatesPage*       m_page7 { nullptr };
  COESQLSettingsPage*     m_page8 { nullptr };
  CVisualAttributesPage*  m_page9 { nullptr };
  StyleButton             m_buttonOK;
  StyleButton             m_buttonCancel;
};
