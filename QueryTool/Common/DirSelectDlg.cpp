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

#include "pch.h"
#include <Shlobj.h>
#include <Common/DirSelectDlg.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace Common 
{
    CString CDirSelectDlg::g_initialFolder;
    int CALLBACK CDirSelectDlg::BrowseCallbackProc (HWND hwnd, UINT uMsg, LPARAM lp, LPARAM /*pData*/) 
    {
        TCHAR szDir[MAX_PATH];   
        switch(uMsg) 
        {
        case BFFM_INITIALIZED: 
            // WParam is TRUE since you are passing a path.
            // It would be FALSE if you were passing a pidl.
            SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)(LPCTSTR)g_initialFolder);
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
            StyleMessageBox(NULL,_T("Cannot open \"Browse For Folder\" dialog."), _T("ERROR"), MB_OK|MB_ICONSTOP);
        }

        return retVal;
    }
};//namespace Common