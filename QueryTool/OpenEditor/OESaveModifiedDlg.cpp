#include "pch.h"
#include "resource.h"
#include "OESaveModifiedDlg.h"

// CSaveModifiedDlg dialog

COESaveModifiedDlg::COESaveModifiedDlg(DocSavingList& list)
	                 :StyleDialog(COESaveModifiedDlg::IDD)
                   ,m_list(list)
{
}

void COESaveModifiedDlg::DoDataExchange(CDataExchange* pDX)
{
  StyleDialog::DoDataExchange(pDX);

  DDX_Control(pDX,IDC_OESA_LIST,m_listBox);
  DDX_Control(pDX,IDOK,         m_buttonYes);
  DDX_Control(pDX,IDNO,         m_buttonNo);
  DDX_Control(pDX,IDCANCEL,     m_buttonCancel);

}


BEGIN_MESSAGE_MAP(COESaveModifiedDlg, StyleDialog)
    ON_BN_CLICKED(IDNO, OnNO)
END_MESSAGE_MAP()

BOOL COESaveModifiedDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();

  CString title;
  title.LoadString(IDR_MAINFRAME);
  SetWindowText(title);

  for (auto& item : m_list)
  {
    title = item.first->GetTitle();
    m_listBox.AddString(title);
  }
  m_listBox.Invalidate();

  UpdateData(FALSE);
  return TRUE;
}

void 
COESaveModifiedDlg::OnOK()
{
  CDialog::OnOK();
}

void 
COESaveModifiedDlg::OnNO()
{
  EndDialog(IDNO);
}
