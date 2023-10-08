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

#pragma once
#pragma warning ( disable : 4786 )
#include <list>
#include <string>

namespace Common
{
    void AppRestoreHistory(CComboBox&     wndList,LPCTSTR szSection,LPCTSTR szEntry,int nSize);
    void AppRestoreHistory(StyleComboBox& wndList,LPCTSTR szSection,LPCTSTR szEntry,int nSize);
    void AppSaveHistory   (CComboBox&     wndList,LPCTSTR szSection,LPCTSTR szEntry,int nSize);
    void AppSaveHistory   (StyleComboBox& wndList,LPCTSTR szSection,LPCTSTR szEntry,int nSize);

    typedef CString              String;
    typedef std::list<String>    StringList;
    typedef StringList::iterator StringListIt;

    void AppWalkDir (LPCTSTR szPath, LPCTSTR szFileMask,
                     StringList& listFiles, BOOL bSortFiles = TRUE, StringList* pListSubdir = 0);
    BOOL AppDeleteFiles (LPCTSTR szPath, LPCTSTR szFileMask, BOOL bAndSubdir);
    BOOL AppDeleteDirectory (LPCTSTR szPath);
    void AppGetPath (CString& path);
    void AppGetFullPathName (const String& path, String& fullPath);
    void FindApplicDirectory(CString& p_directory);

    bool AppGetFileAttrs (LPCTSTR szPath
                         ,DWORD*      attrs
                         ,__int64*    fileSize      = 0
                         ,__int64*    creationTime  = 0
                         ,__int64*    lastWriteTime = 0);

}
//namespace Common
