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

/*
    23.03.2003 improvement, save files dialog appears when the program is closing
    31.03.2003 bug fix, the programm fails on attempt to open multiple file selection in Open dialog
    31.03.2003 bug fix, "Save all" command is not silent since "Save all on exit" dialog was created
    26.05.2003 bug fix, no file name on "Save As" or "Save" for a new file
*/

#include "pch.h"
#include "COMMON/DocManagerExt.h"
#include "OESaveModifiedDlg.h"
#include "OEDocument.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

    using namespace OpenEditor;

static 
void convert_extensions (const string& extensions, string& ofn_extensions)
{
    string buffer;
    string::const_iterator it = extensions.begin();
    for (; it != extensions.end(); it++)
    {
        if (isspace(*it) || *it == ',' && !buffer.empty())
        {
            if (!ofn_extensions.empty()) 
                ofn_extensions += ';';
            ofn_extensions += "*." + buffer;
            buffer.erase();
        }
        else
            buffer += *it;
    }
    if (!buffer.empty())
    {
        if (!ofn_extensions.empty()) 
            ofn_extensions += ';';
        ofn_extensions += "*." + buffer;
        buffer.erase();
    }
}

static 
void make_filter (CString& filter, OPENFILENAME& ofn)
{
    const SettingsManager& settingMgrl = COEDocument::GetSettingsManager();
    /*{
        const string& extensions = settingMgrl.GetDefaults().GetExtensions();

        string buffer;
        string::const_iterator it = extensions.begin();
        for (; it != extensions.end(); it++)
        {
            if (isspace(*it) || *it == ',' && !buffer.empty())
                break;
            else
                buffer += *it;
        }

        *pstrDefaultExt = buffer.c_str();
	    //ofn.lpstrDefExt = (LPTSTR)(LPCTSTR)(*pstrDefaultExt);
    }*/

    for (int i(0), count(settingMgrl.GetClassCount()); i < count; i++)
    {
        const ClassSettings& classSettings = settingMgrl.GetClassByPos(i);
        const string& extensions = classSettings.GetExtensions();
        const string& name = classSettings.GetName();

        string ofn_extensions;
        convert_extensions(extensions, ofn_extensions);

        filter += name.c_str();
        filter += " Files (";
        filter += ofn_extensions.c_str();
        filter += ')';
        filter += '\0';
        filter += ofn_extensions.c_str();
        filter += '\0';

	    ofn.nMaxCustFilter++;
    }
    
}

IMPLEMENT_DYNAMIC(CDocumentExt, CDocument)

CString CDocManagerExt::m_fileOpenPath;

IMPLEMENT_DYNAMIC(CDocManagerExt, CDocManager)

CDocManagerExt::CDocManagerExt ()
{
    m_nFilterIndex = 0;
}

BOOL CDocManagerExt::DoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* /*pTemplate*/)
{
  UINT flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER | OFN_ENABLESIZING;
  if(bOpenFileDialog)
  {
    flags |= OFN_ALLOWMULTISELECT;
  }
	CFileDialog dlgFile(bOpenFileDialog, NULL, NULL,flags,NULL, NULL, 0);

	CString title;
	VERIFY(title.LoadString(nIDSTitle));

	dlgFile.m_ofn.Flags |= lFlags;

	CString strFilter;
	// append the "*.*" all files filter
	strFilter += "All Files (*.*)";
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.*");
	strFilter += (TCHAR)'\0';   // last string
    
    make_filter(strFilter, dlgFile.m_ofn);
    strFilter += '\0'; // close

	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
    dlgFile.m_ofn.nFilterIndex = m_nFilterIndex;

// 31.03.2003 bug fix, the programm fails on attempt to open multiple file selection in Open dialog
#if _MAX_PATH*256 < 64*1024-1
#define PATH_BUFFER _MAX_PATH*256
#else
#define PATH_BUFFER 64*1024-1
#endif
    dlgFile.m_ofn.nMaxFile = PATH_BUFFER;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(dlgFile.m_ofn.nMaxFile);
    //dlgFile.m_ofn.lpstrFile[0] = 0; // 26.05.2003 bug fix, no file name on "Save As" or "Save" for a new file
	
    // overwrite initial dir
    dlgFile.m_ofn.lpstrInitialDir = m_fileOpenPath.IsEmpty() ? NULL : (LPCSTR)m_fileOpenPath;

    INT_PTR nResult = dlgFile.DoModal();
    
    m_nFilterIndex = dlgFile.m_ofn.nFilterIndex;
	fileName.ReleaseBuffer();

	return nResult == IDOK;
}

/*
  overwritten to support OFN_ALLOWMULTISELECT
*/
#pragma warning (push)
#pragma warning (disable : 4706)
void CDocManagerExt::OnFileOpen ()
{
	// prompt the user (with all document templates)
	CString newName;
	
    if (!DoPromptFileName(newName, AFX_IDS_OPENFILE, 
            OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, TRUE, NULL))
		return; // open cancelled

    const char* pszBuffer = newName.LockBuffer();
    const char* pszName = strchr(pszBuffer, 0);
  
    _ASSERTE(pszName);

    if (*(++pszName) == 0)
	    AfxGetApp()->OpenDocumentFile(newName);
    else 
    {
        do 
        {
            CString strFileName(pszBuffer);
   	        AfxGetApp()->OpenDocumentFile(strFileName + '\\' + pszName);
        } 
        while ((pszName = strchr(pszName, 0)) && (*(++pszName) != 0));
    }
}
#pragma warning (pop)

BOOL CDocManagerExt::SaveAndCloseAllDocuments ()
{
    BOOL retVal = TRUE;
	POSITION pos = m_templateList.GetHeadPosition();
	while (pos != NULL)
	{
		CDocTemplate* pTemplate = (CDocTemplate*)m_templateList.GetNext(pos);
		
        ASSERT_KINDOF(CDocTemplate, pTemplate);
		
        if (pTemplate->IsKindOf(RUNTIME_CLASS(CMultiDocTemplateExt)))
        {
            if (!((CMultiDocTemplateExt*)(pTemplate))->SaveAndCloseAllDocuments())
                retVal = FALSE;
        }
        else
        {
            if (pTemplate->SaveAllModified())
		        pTemplate->CloseAllDocuments(FALSE);
            else
                retVal = FALSE;
        }
	}
	return retVal;
}


IMPLEMENT_DYNAMIC(CMultiDocTemplateExt, CMultiDocTemplate)

BOOL CMultiDocTemplateExt::SaveAllModified()
{
    // 31.03.2003 bug fix, "Save all" command is not silent since "Save all on exit" dialog was created
    if (!COEDocument::GetSaveModifiedSilent())
    {
        BOOL failed = FALSE;

        DocSavingList saveList;

	    POSITION pos = GetFirstDocPosition();
    	
        while (pos != NULL)
	    {
		    CDocument* pDoc = GetNextDoc(pos);
    		
            if (pDoc->IsModified())
                saveList.push_back(std::pair<CDocument*, bool>(pDoc, true));
	    }

        if (saveList.size())
        {
            switch (COESaveModifiedDlg(saveList).DoModal())
            {
            case IDOK:
			    {
				    for (DocSavingListConst_Iterator it = saveList.begin(); it != saveList.end(); ++it)
					    if (it->second) 
						    failed |= !it->first->DoFileSave();
			    }
                break;
            case IDCANCEL:
                failed = TRUE;
            }
        }

        return !failed;
    }
    return CMultiDocTemplate::SaveAllModified();
}

BOOL CMultiDocTemplateExt::SaveAndCloseAllDocuments ()
{
    BOOL retVal = SaveAllModified();

    if (retVal)
    {
	    POSITION pos = GetFirstDocPosition();

	    while (pos != NULL)
		    GetNextDoc(pos)->OnCloseDocument();
    }

	return retVal;
}

BOOL CMultiDocTemplateExt::OnIdle (LONG lCount)
{
    BOOL retVal = FALSE;

	POSITION pos = GetFirstDocPosition();

	while (pos != NULL)
	{
		CDocumentExt* pDoc = (CDocumentExt*)GetNextDoc(pos);
		ASSERT_VALID(pDoc);
		ASSERT_KINDOF(CDocumentExt, pDoc);
		retVal |= pDoc->OnIdle(lCount);
	}

    return retVal;
}
