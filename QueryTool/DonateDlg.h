////////////////////////////////////////////////////////////////////////
//
// File: DonateDlg.h
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
#pragma once
#include <StyleButton.h>
#include <StyleDialog.h>

// DonateDlg dialog

class DonateDlg : public StyleDialog
{
	DECLARE_DYNAMIC(DonateDlg)

public:
	DonateDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~DonateDlg();

// Dialog Data
	enum { IDD = IDD_DONATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual BOOL OnInitDialog() override;

	CString CreateDonationLink(int p_amount);
	void    StartDonation(CString p_link);

	CString     m_text;
	StyleEdit   m_editText;
	StyleButton m_button10;
	StyleButton m_button25;
	StyleButton m_buttonAny;

	DECLARE_MESSAGE_MAP()

	afx_msg void OnBtnClicked10();
	afx_msg void OnBtnClicked25();
	afx_msg void OnBtnClickedAny();
};
