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

// 14/08/2002 bug fix, empty string cannot be saved in in combo box history

#include "pch.h"
#include "COMMON\AppUtilities.h"
#include <StyleComboBox.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace Common
{

void AppRestoreHistory (CComboBox& wndList, LPCTSTR szSection, LPCTSTR szEntry, int nSize)
{
    wndList.ResetContent();

    for (int i(0); i < nSize; i++)
    {
        CString strEntry, strBuffer;
        strEntry.Format(_T("%s_%d"), (LPCTSTR)szEntry, i);
        strBuffer = AfxGetApp()->GetProfileString(szSection, strEntry);

        if (!i)
            wndList.SetWindowText(strBuffer);

        if (!strBuffer.IsEmpty())
            wndList.AddString(strBuffer);
        else 
            if (i) break; // only the first string can be null
    }
}

void AppRestoreHistory(StyleComboBox& wndList,LPCTSTR szSection,LPCTSTR szEntry,int nSize)
{
  wndList.ResetContent();

  for(int i(0); i < nSize; i++)
  {
    CString strEntry,strBuffer;
    strEntry.Format(_T("%s_%d"),(LPCTSTR) szEntry,i);
    strBuffer = AfxGetApp()->GetProfileString(szSection,strEntry);

    if(!i)
      wndList.SetWindowText(strBuffer);

    if(!strBuffer.IsEmpty())
      wndList.AddString(strBuffer);
    else
      if(i) break; // only the first string can be null
  }
}

void AppSaveHistory (CComboBox& wndList, LPCTSTR szSection, LPCTSTR szEntry, int nSize)
{
  int i(0);
  CString strText,strEntry;
  wndList.GetWindowText(strText);

  strEntry.Format(_T("%s_%d"),(LPCTSTR) szEntry,i);
  AfxGetApp()->WriteProfileString(szSection,strEntry,strText);
  i++;

  int nCount = wndList.GetCount();
  for(int j(0); i < nSize && j < nCount; i++,j++)
  {
    CString strBuffer;

    while(j < nCount)
    {
      wndList.GetLBText(j,strBuffer);

      if(strBuffer == strText)
      {
        j++;
      }
      else
      {
        break;
      }
    }

    if(strBuffer.IsEmpty() || strBuffer == strText)
    {
      break;
    }
    strEntry.Format(_T("%s_%d"),(LPCTSTR) szEntry,i);
    AfxGetApp()->WriteProfileString(szSection,strEntry,strBuffer);
  }
}

void AppSaveHistory(StyleComboBox& wndList,LPCTSTR szSection,LPCTSTR szEntry,int nSize)
{
  int i(0);
  CString strText,strEntry;
  wndList.GetWindowText(strText);

  strEntry.Format(_T("%s_%d"),(LPCTSTR) szEntry,i);
  AfxGetApp()->WriteProfileString(szSection,strEntry,strText);
  i++;

  int nCount = wndList.GetCount();
  for(int j(0); i < nSize && j < nCount; i++,j++)
  {
    CString strBuffer;

    while(j < nCount)
    {
      wndList.GetLBText(j,strBuffer);

      if(strBuffer == strText)
      {
        j++;
      }
      else
      {
        break;
      }
    }

    if(strBuffer.IsEmpty() || strBuffer == strText)
    {
      break;
    }
    strEntry.Format(_T("%s_%d"),(LPCTSTR) szEntry,i);
    AfxGetApp()->WriteProfileString(szSection,strEntry,strBuffer);
  }
}

void AppWalkDir (LPCTSTR szPath
                ,LPCTSTR szFileMask
                ,StringList& listFiles
                ,BOOL    bSortFiles
                ,StringList* pListSubdir)
{
    HANDLE hFile = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATA wfdFile;
    memset(&wfdFile, 0, sizeof wfdFile);

    CString strBuffer;
    CString strPath(szPath);
    if(strPath.GetLength() > 0 && strPath[strPath.GetLength() - 1] != '\\')
    {
      strPath += _T("\\");
    }
    StringList listLocal;

    strBuffer = strPath;
    strBuffer += szFileMask;
    hFile = FindFirstFile(strBuffer.GetString(), &wfdFile);

    if(hFile != INVALID_HANDLE_VALUE)
    {
        do
        {
            if(wfdFile.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
                listLocal.push_back(wfdFile.cFileName);
        }
        while(FindNextFile(hFile, &wfdFile));

        if(hFile != INVALID_HANDLE_VALUE)
            FindClose(hFile);
    }

    if (bSortFiles)
        listLocal.sort();

    StringListIt it;
    for(it = listLocal.begin(); it != listLocal.end(); it++)
    {
        strBuffer = strPath;
        strBuffer.Append(*it);
        listFiles.push_back(strBuffer);
    }

    memset(&wfdFile, 0, sizeof wfdFile);
    strBuffer = strPath;
    strBuffer += _T("*.*");

    listLocal.clear();
    hFile = FindFirstFile(strBuffer.GetString(), &wfdFile);

    if(hFile != INVALID_HANDLE_VALUE)
    {
        do
        {
            if(wfdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY
            && _tcscmp(_T("."),  wfdFile.cFileName)
            && _tcscmp(_T(".."), wfdFile.cFileName))
                listLocal.push_back(wfdFile.cFileName);
        }
        while(FindNextFile(hFile, &wfdFile));

        if(hFile != INVALID_HANDLE_VALUE)
        FindClose(hFile);
    }

    if (bSortFiles)
        listLocal.sort();

    for(it = listLocal.begin(); it != listLocal.end(); it++)
    {
        strBuffer = strPath;
        strBuffer.Append(*it);

        AppWalkDir(strBuffer.GetString(), szFileMask, listFiles, bSortFiles, pListSubdir);

        if (pListSubdir)
            pListSubdir->push_back(strBuffer);
    }

    listLocal.clear();
}

BOOL AppDeleteFiles (LPCTSTR szPath, LPCTSTR szFileMask, BOOL bAndSubdir)
{
    BOOL bRetVal = TRUE;
    StringList listFiles, listSubdir;

    AppWalkDir(szPath, szFileMask, listFiles, FALSE, &listSubdir);

    StringListIt it;
    for(it = listFiles.begin(); it != listFiles.end(); it++)
        if (DeleteFile((*it).GetString()) == FALSE)
            bRetVal = FALSE;

    if (bAndSubdir)
        for(it = listSubdir.begin(); it != listSubdir.end(); it++)
            if (RemoveDirectory((*it).GetString()) == FALSE)
                bRetVal = FALSE;

    return bRetVal;
}


BOOL AppDeleteDirectory (LPCTSTR szPath)
{
    AppDeleteFiles(szPath, _T("*.*"), TRUE);
    return RemoveDirectory(szPath);
}

bool AppGetFileAttrs (LPCTSTR  szPath
                     ,DWORD*   attrs
                     ,__int64* fileSize
                     ,__int64* creationTime
                     ,__int64* lastWriteTime)
{
  HANDLE hFile = ::CreateFile(szPath, GENERIC_READ, FILE_SHARE_WRITE|FILE_SHARE_READ, 
                              NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		BY_HANDLE_FILE_INFORMATION info;

        // it can be replaced with GetFileAttributesEx, but it'll not be compatible with win95
		if (GetFileInformationByHandle(hFile, &info))
		{
            if (attrs)
                *attrs = info.dwFileAttributes;
            if (fileSize)
                *fileSize = (__int64(info.nFileSizeHigh) << 32) 
                    + info.nFileSizeLow;
            if (creationTime)
			    *creationTime = (__int64(info.ftCreationTime.dwHighDateTime) << 32) 
                    + info.ftCreationTime.dwLowDateTime;
            if (lastWriteTime)
			    *lastWriteTime = (__int64(info.ftLastWriteTime.dwHighDateTime) << 32) 
                    + info.ftLastWriteTime.dwLowDateTime;
            ::CloseHandle(hFile);
			return true;
		}
        ::CloseHandle(hFile);
	}

    TRACE("Common::AppGetFileAttrs failed for %s", szPath);
	return false;
}

void AppGetPath (CString& path)
{
    TCHAR szBuff[_MAX_PATH + 1];
    VERIFY(::GetModuleFileName(AfxGetApp()->m_hInstance, szBuff, _MAX_PATH));
    szBuff[_MAX_PATH] = 0;
    path = szBuff;
    int pathLen = path.ReverseFind('\\');
    if(pathLen >= 0)
    {
      path.Truncate(pathLen);
    }
}

void AppGetFullPathName (const String& path, String& fullPath)
{
	LPTSTR filename;
  int length = GetFullPathName(path.GetString(), 0, 0, &filename);
  LPTSTR buffer = new TCHAR[length + 1];
  GetFullPathName(path.GetString(), length + 1, buffer, &filename);
  fullPath = buffer;
  delete [] buffer;
}

void
FindApplicDirectory(CString& p_directory)
{
  IMalloc*      pShellMalloc = NULL;    // A pointer to the shell's IMalloc interface
  IShellFolder* psfParent;              // A pointer to the parent folder object's IShellFolder interface.
  LPITEMIDLIST  pidlItem = NULL;        // The item's PIDL.
  LPITEMIDLIST  pidlRelative = NULL;    // The item's PIDL relative to the parent folder.
  TCHAR         szPath[MAX_PATH] = _T("");  // The path for Favorites.
  STRRET        str;                    // The structure for strings returned from IShellFolder.

  HRESULT hres = SHGetMalloc(&pShellMalloc);
  if(FAILED(hres))
  {
    return;
  }
  hres = SHGetSpecialFolderLocation(NULL,CSIDL_APPDATA,&pidlItem);
  if(SUCCEEDED(hres))
  {
    hres = SHBindToParent(pidlItem,
                          IID_IShellFolder,
                          (void**) &psfParent,
                          (LPCITEMIDLIST*) &pidlRelative);
    if(SUCCEEDED(hres))
    {
      // Retrieve the path
      memset(&str,0,sizeof(str));
      hres = psfParent->GetDisplayNameOf(pidlRelative,SHGDN_NORMAL | SHGDN_FORPARSING,&str);
      if(SUCCEEDED(hres))
      {
        StrRetToBuf(&str,pidlItem,szPath,ARRAYSIZE(szPath));
      }
      psfParent->Release();
    }
  }
  // Clean up allocated memory
  if(pidlItem)
  {
    pShellMalloc->Free(pidlItem);
  }
  pShellMalloc->Release();
  // Result
  p_directory = szPath;
}


}//namespace Common
