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

#include "stdafx.h"
#include <Shlobj.h>
#include <Common/DirSelectDlg.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace Common 
{

#if !(_MFC_VER >= 0x0600)

DWORD CDirSelectDlg::c_dwFlags = OFN_SHOWHELP | OFN_HIDEREADONLY | OFN_ENABLESIZING | WS_SIZEBOX
                                 OFN_OVERWRITEPROMPT | OFN_ENABLETEMPLATE;
const char* CDirSelectDlg::c_pcszFilter = "(none)|NONE.$$$||";


/////////////////////////////////////////////////////////////////////////////
// CDirSelectDlg

CDirSelectDlg::CDirSelectDlg(LPCTSTR lpcszCaption, CWnd* pWndParent, LPCTSTR lpcszInitialDir) :
    CFileDialog(FALSE, NULL, NULL, c_dwFlags, c_pcszFilter, pWndParent)
{
    m_ofn.lpstrTitle      = lpcszCaption;
    m_ofn.lpstrInitialDir = lpcszInitialDir;
    m_ofn.hInstance = AfxFindResourceHandle(MAKEINTRESOURCE(IDD_DFP_DIRSELECT), RT_DIALOG);
    m_ofn.lpTemplateName  = MAKEINTRESOURCE(IDD_DFP_DIRSELECT);
    m_ofn.Flags &= ~OFN_EXPLORER;
}

void CDirSelectDlg::GetPath(CString& strPath) const
{
    CString strTemp(m_ofn.lpstrFile);
    strPath = strTemp.Left(m_ofn.nFileOffset);
}

/*
BEGIN_MESSAGE_MAP(CDirSelectDlg, CFileDialog)
    //{{AFX_MSG_MAP(CDirSelectDlg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()
*/

BOOL CDirSelectDlg::OnInitDialog()
{
    CFileDialog::OnInitDialog();

    GetDlgItem(IDC_DFP_STATIC_FILE_NAME)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_DFP_FILE_NAME)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_DFP_FILE_LIST)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_DFP_STATIC_TYPE)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_DFP_TYPE_LIST)->ShowWindow(SW_HIDE);

    // If the file name field is empty, or has a wildcard in it when the 
    // user clicks OK, the dialog will not close.  Thus, we need to put
    // some dummy text in there.
    
    SetDlgItemText(IDC_DFP_FILE_NAME, "none");
    GetDlgItem(IDC_DFP_DIR_LIST)->SetFocus();
    return FALSE;
}

#else

    CString CDirSelectDlg::g_initialFolder;
    int CALLBACK CDirSelectDlg::BrowseCallbackProc (HWND hwnd, UINT uMsg, LPARAM lp, LPARAM /*pData*/) 
    {
        TCHAR szDir[MAX_PATH];   
        switch(uMsg) 
        {
        case BFFM_INITIALIZED: 
            // WParam is TRUE since you are passing a path.
            // It would be FALSE if you were passing a pidl.
            SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)(LPCSTR)g_initialFolder);
            break;   
        case BFFM_SELCHANGED: 
            // Set the status window to the currently selected path.
            if (SHGetPathFromIDList((LPITEMIDLIST)lp ,szDir))
                SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0, (LPARAM)szDir);
            break;
        }
        return 0;
    }

    CDirSelectDlg::CDirSelectDlg (LPCTSTR lpcszCaption, CWnd* pWndParent, LPCTSTR lpcszInitialDir)
        : m_folder(lpcszInitialDir), m_caption(lpcszCaption), m_pWndParent(pWndParent)
    {
    }

    int CDirSelectDlg::DoModal ()
    {
        int retVal = IDCANCEL;
        LPMALLOC lpMalloc = 0;
        if (::SHGetMalloc(&lpMalloc) == NOERROR)
        {
            BROWSEINFO info;
            memset(&info, 0, sizeof(info));
            info.hwndOwner  = m_pWndParent->GetSafeHwnd();
            info.lpfn       = BrowseCallbackProc;

#if _MFC_VER > 0x0600
            info.ulFlags    = BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS;
#else
            info.ulFlags    = BIF_RETURNONLYFSDIRS;
#endif

            info.lpszTitle  = m_caption; 
            g_initialFolder = m_folder;
            info.pszDisplayName = m_folder.GetBuffer(_MAX_PATH); 

            LPCITEMIDLIST lplist = ::SHBrowseForFolder(&info);
            
            if (lplist && ::SHGetPathFromIDList(lplist, info.pszDisplayName))
            {
                if (info.pszDisplayName[0])
                    retVal = IDOK;
                else
                    retVal = IDCANCEL;
            }
            else
                retVal = IDCANCEL;

            lpMalloc->Free((void*)lplist);
            lpMalloc->Release();
            m_folder.ReleaseBuffer();
        }
        else
        {
            MessageBeep((UINT)-1);
            WideMessageBox(NULL,"Cannot open \"Browse For Folder\" dialog.", "ERROR", MB_OK|MB_ICONSTOP);
        }

        return retVal;
    }

#endif

};//namespace Common