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

#ifndef __DOCMANAGEREXT_H__
#define __DOCMANAGEREXT_H__
#pragma once

/*
  CDocManagerExt supports multiple extensions for one document type:
  * extended syntax - ... SQL Files|SQL Files\n*.SQL|*.PLS;*.PLB ...;
  * backward compatibility according MFC documentation (... .SQL ...);

  The following methods nave not been implemented yet:
    void RegisterShellFileTypes (BOOL bCompat);
    void UnregisterShellFileTypes ();
*/

class CDocumentExt : public CDocument
{
    DECLARE_DYNAMIC(CDocumentExt);
public:
    virtual BOOL OnIdle (LONG) = 0;
};

class CDocManagerExt : public CDocManager  
{
public:
    DECLARE_DYNAMIC(CDocManagerExt);
    CDocManagerExt ();
	// helper for standard commdlg dialogs
	virtual BOOL DoPromptFileName (CString& fileName, UINT nIDSTitle, DWORD lFlags, 
                                   BOOL bOpenFileDialog, CDocTemplate* pTemplate);
    virtual void OnFileOpen ();

    virtual BOOL SaveAndCloseAllDocuments ();

    static void SetFileOpenPath (const CString path) { m_fileOpenPath = path; }
private:
    DWORD m_nFilterIndex; 
    static CString m_fileOpenPath;
};

class CMultiDocTemplateExt : public CMultiDocTemplate
{
public:
    DECLARE_DYNAMIC(CMultiDocTemplateExt);
    CMultiDocTemplateExt (UINT nIDResource, CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass)
        : CMultiDocTemplate (nIDResource, pDocClass, pFrameClass, pViewClass) {}

    virtual BOOL SaveAllModified();
    virtual BOOL SaveAndCloseAllDocuments ();
    
    BOOL OnIdle (LONG);
};


#endif//__DOCMANAGEREXT_H__
