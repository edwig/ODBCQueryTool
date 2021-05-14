#include "pch.h"
#include "OESettingsDlg.h"
#include "OEDocument.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace OpenEditor;

COESettingsDlg::COESettingsDlg(SettingsManager&   p_settings
                              ,CWnd*              p_parent)
               :m_settings(p_settings)
               ,StyleDialog(COESettingsDlg::IDD, p_parent)
{
}

COESettingsDlg::~COESettingsDlg()
{
  delete m_page1;
  delete m_page2;
  delete m_page3;
  delete m_page4;
  delete m_page5;
  delete m_page6;
  delete m_page7;
  delete m_page8;
  delete m_page9;
}

void 
COESettingsDlg::DoDataExchange(CDataExchange* pDX)
{
  StyleDialog::DoDataExchange(pDX);

  DDX_Control(pDX, IDC_TAB,  m_tabs);
  DDX_Control(pDX, IDOK,     m_buttonOK);
  DDX_Control(pDX, IDCANCEL, m_buttonCancel);
}

BEGIN_MESSAGE_MAP(COESettingsDlg,StyleDialog)
  ON_BN_CLICKED(IDCANCEL,OnCancel)
  ON_BN_CLICKED(IDOK, &COESettingsDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL
COESettingsDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();
  SetWindowText("Document info");
  ShowMinMaxButton(false,true);

  InitTabs();

  SetCanResize();
  return InitFirstFocus();
}

void
COESettingsDlg::SetupDynamicLayout()
{
  StyleDialog::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif

  manager.AddItem(IDC_TAB, manager.MoveNone(),manager.SizeHorizontalAndVertical(100,100));
  manager.AddItem(IDOK,    manager.MoveHorizontalAndVertical(100,100),manager.SizeNone());
  manager.AddItem(IDCANCEL,manager.MoveHorizontalAndVertical(100,100),manager.SizeNone());
}

void
COESettingsDlg::InitTabs()
{
  m_page1 = new COEGeneralPage       (m_settings,&m_tabs);
  m_page2 = new COEFilePage          (m_settings,&m_tabs);
  m_page3 = new COEBackupPage        (m_settings,&m_tabs);
  m_page4 = new COEEditingPage       (m_settings,&m_tabs);
  m_page5 = new COEBlockPage         (m_settings,&m_tabs);
  m_page6 = new COEClassPage         (m_settings,&m_tabs);
  m_page7 = new COETemplatesPage     (m_settings,&m_tabs);
  m_page8 = new COESQLSettingsPage   (m_settings,&m_tabs);
  m_page9 = new CVisualAttributesPage(m_settings,&m_tabs);

  m_page1->Create(IDD_OE_GENERAL,         &m_tabs);
  m_page2->Create(IDD_OE_FILES,           &m_tabs);
  m_page3->Create(IDD_OE_BACKUP,          &m_tabs);
  m_page4->Create(IDD_OE_EDITING,         &m_tabs);
  m_page5->Create(IDD_OE_BLOCKS,          &m_tabs);
  m_page6->Create(IDD_OE_CLASSES,         &m_tabs);
  m_page7->Create(IDD_OE_TEMPLATES,       &m_tabs);
  m_page8->Create(IDD_OE_SQLSETTINGS,     &m_tabs);
  m_page9->Create(IDD_OE_PROP_FONT_COLORS,&m_tabs);

  m_tabs.InsertItem(0,m_page1,"General");
  m_tabs.InsertItem(1,m_page2,"Files");
  m_tabs.InsertItem(2,m_page3,"Backup");
  m_tabs.InsertItem(3,m_page4,"Editing");
  m_tabs.InsertItem(4,m_page5,"Selection");
  m_tabs.InsertItem(5,m_page6,"Categories");
  m_tabs.InsertItem(6,m_page7,"Templates");
  m_tabs.InsertItem(7,m_page8,"SQL");
  m_tabs.InsertItem(8,m_page9,"Fonts");

  m_tabs.Init();
}

void
COESettingsDlg::OnBnClickedOk()
{
  m_page1->OnApply();
  m_page2->OnApply();
  m_page3->OnApply();
  m_page4->OnApply();
  m_page5->OnApply();
  m_page8->OnApply();

  StyleDialog::OnOK();
}
