#pragma once
#include "OEDocument.h"
#include "OEFileInfoDlg.h"
#include "OEPropPage.h"


class COEDocumentDlg : public StyleDialog
{
public:
  COEDocumentDlg(COEDocument* p_document,InstanceSettings& p_settings,CWnd* p_parent);
  virtual ~COEDocumentDlg();

private:
  virtual BOOL OnInitDialog() override;
  virtual void DoDataExchange(CDataExchange* pDX) override;
  virtual void SetupDynamicLayout() override;
  
  void InitTabs();

  enum { IDD = IDD_OE_DOCUMENT };

protected:
  DECLARE_MESSAGE_MAP()

  afx_msg void OnOK();

private:
  COEDocument*      m_document;
  InstanceSettings& m_settings;
  StyleTabCtrl      m_tabs;
  COEPropPage*      m_page1 { nullptr };
  COEFileInfoDlg*   m_page2 { nullptr };
  StyleButton       m_buttonOK;
  StyleButton       m_buttonCancel;
};
