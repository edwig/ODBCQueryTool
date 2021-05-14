#include "pch.h"
#include "OEDocumentDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace OpenEditor;


COEDocumentDlg::COEDocumentDlg(COEDocument*       p_document
                              ,InstanceSettings&  p_settings
                              ,CWnd*              p_parent)
               :m_document(p_document)
               ,m_settings(p_settings)
               ,StyleDialog(COEDocumentDlg::IDD, p_parent)
{
}

COEDocumentDlg::~COEDocumentDlg()
{
  delete m_page1;
  delete m_page2;
}

void 
COEDocumentDlg::DoDataExchange(CDataExchange* pDX)
{
  StyleDialog::DoDataExchange(pDX);

  DDX_Control(pDX, IDC_TAB,  m_tabs);
  DDX_Control(pDX, IDOK,     m_buttonOK);
  DDX_Control(pDX, IDCANCEL, m_buttonCancel);
}

BEGIN_MESSAGE_MAP(COEDocumentDlg,StyleDialog)
  ON_BN_CLICKED(IDCANCEL,OnCancel)
END_MESSAGE_MAP()

BOOL
COEDocumentDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();
  SetWindowText("Document info");
  ShowMinMaxButton(false,true);

  InitTabs();

  SetCanResize();
  return InitFirstFocus();
}

void
COEDocumentDlg::SetupDynamicLayout()
{
  StyleDialog::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif

  manager.AddItem(IDC_TAB, manager.MoveNone(),manager.SizeHorizontalAndVertical(100,100));
  manager.AddItem(IDCANCEL,manager.MoveHorizontalAndVertical(100,100),manager.SizeNone());
}

void
COEDocumentDlg::InitTabs()
{
  m_page1 = new COEPropPage(m_document->GetSettingsManager(),m_settings,this);
  m_page2 = new COEFileInfoDlg(m_document,this);

  m_page1->Create(IDD_OE_FILE_SETTINGS, &m_tabs);
  m_page2->Create(IDD_OE_FILE_INFO,     &m_tabs);

  m_tabs.InsertItem(0,m_page1,"Properties");
  m_tabs.InsertItem(1,m_page2,"File");

  m_tabs.Init();
}

void
COEDocumentDlg::OnOK()
{
  StyleDialog::OnOK();
}