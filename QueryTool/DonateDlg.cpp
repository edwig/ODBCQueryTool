////////////////////////////////////////////////////////////////////////
//
// File: DonateDlg.cpp
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of 
// this software and associated documentation files (the "Software"), 
// to deal in the Software without restriction, including without limitation the rights 
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies 
// or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION 
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
#include "pch.h"
#include "QueryTool.h"
#include "DonateDlg.h"
#include "afxdialogex.h"

// DonateDlg dialog

IMPLEMENT_DYNAMIC(DonateDlg, StyleDialog)

DonateDlg::DonateDlg(CWnd* pParent /*=nullptr*/)
          :StyleDialog(IDD_DONATE, pParent)
{
}

DonateDlg::~DonateDlg()
{
}

void 
DonateDlg::DoDataExchange(CDataExchange* pDX)
{
  StyleDialog::DoDataExchange(pDX);

  DDX_Control(pDX,IDC_TEXT,    m_editText,m_text);
  DDX_Control(pDX,IDC_BUTT_10, m_button10);
  DDX_Control(pDX,IDC_BUTT_25, m_button25);
  DDX_Control(pDX,IDC_BUTT_ANY,m_buttonAny);
}

BEGIN_MESSAGE_MAP(DonateDlg, StyleDialog)
  ON_BN_CLICKED(IDC_BUTT_10, OnBtnClicked10)
  ON_BN_CLICKED(IDC_BUTT_25, OnBtnClicked25)
  ON_BN_CLICKED(IDC_BUTT_ANY,OnBtnClickedAny)
END_MESSAGE_MAP()

BOOL
DonateDlg::OnInitDialog()
{
  StyleDialog::OnInitDialog();
  SetWindowText("Consider a donation");

  m_text = "Do you like this tool and would you see it continue developing?\r\n"
           "Then please consider making a donation...";

  UpdateData(FALSE);
  m_buttonAny.SetFocus();
  return FALSE;
}

CString 
DonateDlg::CreateDonationLink(int p_amount)
{
  CString donation("https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=edwig.huisman@hetnet.nl&item_name=Friends+of+Open+ODBCQuerytool&item_number=Supporting+open+source+development");
  if(p_amount)
  {
    donation.AppendFormat("&amount=%d%%2e00",p_amount);
  }
  donation += "&currency_code=EUR";
  return donation;
}

void
DonateDlg::StartDonation(CString p_link)
{
  ::ShellExecuteA(nullptr,"open",p_link.GetString(),nullptr,nullptr,SW_SHOW);
  OnOK();
}

// DonateDlg message handlers

void
DonateDlg::OnBtnClicked10()
{
  CString donation = CreateDonationLink(10);
  StartDonation(donation);
}

void
DonateDlg::OnBtnClicked25()
{
  CString donation = CreateDonationLink(25);
  StartDonation(donation);
}

void
DonateDlg::OnBtnClickedAny()
{
  CString donation = CreateDonationLink(0);
  StartDonation(donation);
}
