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
    void AppRestoreHistory(CComboBox&     wndList,const char* szSection,const char* szEntry,int nSize);
    void AppRestoreHistory(StyleComboBox& wndList,const char* szSection,const char* szEntry,int nSize);
    void AppSaveHistory   (CComboBox&     wndList,const char* szSection,const char* szEntry,int nSize);
    void AppSaveHistory   (StyleComboBox& wndList,const char* szSection,const char* szEntry,int nSize);

    typedef std::string          String;
    typedef std::list<String>    StringList;
    typedef StringList::iterator StringListIt;

    void AppWalkDir (const char* szPath, const char* szFileMask,
                     StringList& listFiles, BOOL bSortFiles = TRUE, StringList* pListSubdir = 0);
    BOOL AppDeleteFiles (const char* szPath, const char* szFileMask, BOOL bAndSubdir);
    BOOL AppDeleteDirectory (const char* szPath);
    void AppGetPath (std::string& path);
    void AppGetFullPathName (const String& path, String& fullPath);
    void FindApplicDirectory(CString& p_directory);

    bool AppGetFileAttrs (const char* szPath
                         ,DWORD*      attrs
                         ,__int64*    fileSize      = 0
                         ,__int64*    creationTime  = 0
                         ,__int64*    lastWriteTime = 0);

}
//namespace Common
