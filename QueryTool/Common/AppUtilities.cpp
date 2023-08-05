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

void AppRestoreHistory (CComboBox& wndList, const char* szSection, const char* szEntry, int nSize)
{
    wndList.ResetContent();

    for (int i(0); i < nSize; i++)
    {
        CString strEntry, strBuffer;
        strEntry.Format("%s_%d", (const char*)szEntry, i);
        strBuffer = AfxGetApp()->GetProfileString(szSection, strEntry);

        if (!i)
            wndList.SetWindowText(strBuffer);

        if (!strBuffer.IsEmpty())
            wndList.AddString(strBuffer);
        else 
            if (i) break; // only the first string can be null
    }
}

void AppRestoreHistory(StyleComboBox& wndList,const char* szSection,const char* szEntry,int nSize)
{
  AppRestoreHistory(reinterpret_cast<CComboBox&>(wndList),szSection,szEntry,nSize);
}

void AppSaveHistory (CComboBox& wndList, const char* szSection, const char* szEntry, int nSize)
{
    int i(0);
    CString strText, strEntry;
    wndList.GetWindowText(strText);

    strEntry.Format("%s_%d", (const char*)szEntry, i);
    AfxGetApp()->WriteProfileString(szSection, strEntry, strText);
    i++;

    int nCount = wndList.GetCount();
    for (int j(0); i < nSize && j < nCount; i++, j++)
    {
        CString strBuffer;

        while (j < nCount)
        {
            wndList.GetLBText(j, strBuffer);

            if (strBuffer == strText) j++;
            else break;
        }

        if (strBuffer.IsEmpty()
        || strBuffer == strText) break;

        strEntry.Format("%s_%d", (const char*)szEntry, i);
        AfxGetApp()->WriteProfileString(szSection, strEntry, strBuffer);
    }
}

void AppSaveHistory(StyleComboBox& wndList,const char* szSection,const char* szEntry,int nSize)
{
  AppSaveHistory(reinterpret_cast<CComboBox&>(wndList),szSection,szEntry,nSize);
}

void AppWalkDir (const char* szPath, const char* szFileMask,
                 StringList& listFiles, BOOL bSortFiles, StringList* pListSubdir)
{
    HANDLE hFile = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATA wfdFile;
    memset(&wfdFile, 0, sizeof wfdFile);

    String strBuffer;
    String strPath(szPath);
    if (strPath.length() > 0 && strPath[strPath.length()-1] != '\\')
        strPath += "\\";

    StringList listLocal;

    strBuffer = strPath;
    strBuffer += szFileMask;
    hFile = FindFirstFile(strBuffer.c_str(), &wfdFile);

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
        strBuffer.append(*it);
        listFiles.push_back(strBuffer);
    }

    memset(&wfdFile, 0, sizeof wfdFile);
    strBuffer = strPath;
    strBuffer += "*.*";

    listLocal.clear();
    hFile = FindFirstFile(strBuffer.c_str(), &wfdFile);

    if(hFile != INVALID_HANDLE_VALUE)
    {
        do
        {
            if(wfdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY
            && strcmp(".", wfdFile.cFileName)
            && strcmp("..", wfdFile.cFileName))
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
        strBuffer.append(*it);

        AppWalkDir(strBuffer.c_str(), szFileMask, listFiles, bSortFiles, pListSubdir);

        if (pListSubdir)
            pListSubdir->push_back(strBuffer);
    }

    listLocal.clear();
}

BOOL AppDeleteFiles (const char* szPath, const char* szFileMask, BOOL bAndSubdir)
{
    BOOL bRetVal = TRUE;
    StringList listFiles, listSubdir;

    AppWalkDir(szPath, szFileMask, listFiles, FALSE, &listSubdir);

    StringListIt it;
    for(it = listFiles.begin(); it != listFiles.end(); it++)
        if (DeleteFile((*it).c_str()) == FALSE)
            bRetVal = FALSE;

    if (bAndSubdir)
        for(it = listSubdir.begin(); it != listSubdir.end(); it++)
            if (RemoveDirectory((*it).c_str()) == FALSE)
                bRetVal = FALSE;

    return bRetVal;
}


BOOL AppDeleteDirectory (const char* szPath)
{
    AppDeleteFiles(szPath, "*.*", TRUE);
    return RemoveDirectory(szPath);
}

bool AppGetFileAttrs (
        const char* szPath, DWORD* attrs, __int64* fileSize,
        __int64* creationTime, __int64* lastWriteTime
    )
{
    HANDLE hFile = ::CreateFile(
            szPath, GENERIC_READ, FILE_SHARE_WRITE|FILE_SHARE_READ, 
            NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
        );

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

void AppGetPath (std::string& path)
{
    char szBuff[_MAX_PATH + 1];
    VERIFY(::GetModuleFileName(AfxGetApp()->m_hInstance, szBuff, _MAX_PATH));
    szBuff[_MAX_PATH] = 0;
    path = szBuff;
    String::size_type pathLen = path.find_last_of('\\');
    path.resize((pathLen != String::npos) ? pathLen : 0);
}

void AppGetFullPathName (const String& path, String& fullPath)
{
	char *filename;
  int length = GetFullPathName(path.c_str(), 0, 0, &filename);
  char *buffer = new char[length + 1];
  GetFullPathName(path.c_str(), length + 1, buffer, &filename);
  fullPath = buffer;
  delete buffer;
}

void
FindApplicDirectory(CString& p_directory)
{
  IMalloc*      pShellMalloc = NULL;    // A pointer to the shell's IMalloc interface
  IShellFolder* psfParent;              // A pointer to the parent folder object's IShellFolder interface.
  LPITEMIDLIST  pidlItem = NULL;        // The item's PIDL.
  LPITEMIDLIST  pidlRelative = NULL;    // The item's PIDL relative to the parent folder.
  CHAR          szPath[MAX_PATH] = "";  // The path for Favorites.
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
