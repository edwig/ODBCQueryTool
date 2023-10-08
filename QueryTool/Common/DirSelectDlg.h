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

// 04/02/2002 it has been reimplemented with Shell API.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef _DIRSELECTDLG_H_
#define _DIRSELECTDLG_H_


namespace Common
{
#if !(_MFC_VER >= 0x0600)

	class CDirSelectDlg : public CFileDialog
	{
	private:
		static DWORD c_dwFlags;
		static const TCHAR* c_pcszFilter;

	public:
		CDirSelectDlg(LPCTSTR lpcszCaption, CWnd* pWndParent = NULL, LPCTSTR lpcszInitialDir = NULL );
		void GetPath(CString& strPath) const;

	protected:
		//{{AFX_MSG(CDirSelectDlg)
		virtual BOOL OnInitDialog();
		//}}AFX_MSG
	//	DECLARE_MESSAGE_MAP()
	};

#else

	class CDirSelectDlg
	{
	public:
		CDirSelectDlg (LPCTSTR lpcszCaption, CWnd* pWndParent = NULL, LPCTSTR lpcszInitialDir = NULL );
		void GetPath (CString& strPath) const { strPath = m_folder; }
		LPCTSTR GetPath () const               { return m_folder.GetString(); }
		int DoModal ();

	private:
		CWnd* m_pWndParent;
		CString m_folder, m_caption;
		static CString g_initialFolder;
		static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData);
	};

#endif

};//namespace Common

//{{AFX_INSERT_LOCATION}}

#endif //_DIRSELECTDLG_H_
