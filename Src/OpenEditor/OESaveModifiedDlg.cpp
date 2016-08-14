#include "stdafx.h"
#include "resource.h"
#include "OpenEditor\OESaveModifiedDlg.h"


// CSaveModifiedDlg dialog

COESaveModifiedDlg::COESaveModifiedDlg(DocSavingList& list)
	: CDialog(COESaveModifiedDlg::IDD),
    m_list(list)
{
}

BEGIN_MESSAGE_MAP(COESaveModifiedDlg, CDialog)
    ON_BN_CLICKED(IDNO, OnNO)
END_MESSAGE_MAP()

BOOL COESaveModifiedDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    CString title;
    title.LoadString(IDR_MAINFRAME);
    SetWindowText(title);

    HWND hList = ::GetDlgItem(m_hWnd, IDC_OESA_LIST);
    
    _ASSERTE(hList);

    if (hList)
    {
        int maxWidth = 0;
        CClientDC dc(this);
        CFont* pOrgFont = dc.SelectObject(GetFont());

        for (DocSavingListConst_Iterator it = m_list.begin(); it != m_list.end(); ++it)
        {
            title = it->first->GetPathName();
            if (title.IsEmpty()) title = it->first->GetTitle();
            int index = (int)::SendMessage(hList, LB_ADDSTRING, NULL, (LPARAM)(LPCSTR)title);
            ::SendMessage(hList, LB_SETSEL, it->second ? TRUE : FALSE, index);

            CSize size = dc.GetTextExtent(title);
            if (maxWidth < size.cx) maxWidth = size.cx;
        }

        ::SendMessage(hList, LB_SETHORIZONTALEXTENT, maxWidth + 10, 0);
        dc.SelectObject(pOrgFont);
   }

    return TRUE;
}

void COESaveModifiedDlg::OnOK()
{
    HWND hList = ::GetDlgItem(m_hWnd, IDC_OESA_LIST);
    
    DocSavingList_Iterator it = m_list.begin();

    for (int index = 0; it != m_list.end(); ++it, ++index)
    {
        it->second = ::SendMessage(hList, LB_GETSEL, index, 0L) ? true : false;
    }

    CDialog::OnOK();
}


void COESaveModifiedDlg::OnNO()
{
    EndDialog(IDNO);
}
