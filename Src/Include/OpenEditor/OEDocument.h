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

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __OEDocument_h__
#define __OEDocument_h__

#include "OpenEditor/OEStorage.h"
#include "OpenEditor/OESettings.h"
#include <COMMON/MemoryMappedFile.h>
#include "Frame/FileWatch.h"
#include "COMMON/DocManagerExt.h"


  class COEditorView;

class COEDocument : public CDocumentExt, CFileWatchClient
{
    // for a modified sign '*' in a title bar
    std::string m_orgTitle, m_extension;
    bool m_orgModified, m_extensionInitialized;
    static bool m_saveModified_silent;
    static bool m_saveModified_skipNew;
    static bool m_enableOpenUnexisting;

    // for backup processing only
    bool m_newOrSaveAs;
    std::string m_lastBackupPath;
    void backupFile (LPCTSTR lpszPathName);

    // settings container
    OpenEditor::InstanceSettings m_settings;
    // the main object
    OpenEditor::Storage m_storage;

    // memory mapped file support
    Common::MemoryMappedFile m_mmfile;
    // allocated memory when file locking is disabled
    LPVOID m_vmdata;
    void loadFile (const char* path, bool reload = false, bool external = false);

    static OpenEditor::SettingsManager m_settingsManager;
    static OpenEditor::Searcher m_searcher;

    virtual void OnWatchedFileChanged ();

protected:
    using CDocumentExt::m_strPathName;
    virtual void GetNewPathName (CString& newName) const { newName = m_strPathName; }

    void SetClassSetting (const char* name) { if (name) m_settings.SetClassSetting(&m_settingsManager.FindByName(name)); }
    void SetText (LPVOID, unsigned long);
    LPCVOID GetVMData () const;

	COEDocument();
	virtual ~COEDocument();
	DECLARE_DYNCREATE(COEDocument)

public:
    void SetText (const char*, unsigned long);

    static bool GetSaveModifiedSilent () { return m_saveModified_silent; }
    static void SetSaveModifiedSilent (bool silent) { m_saveModified_silent = silent; }
    static bool GetSaveModifiedSkipNew () { return m_saveModified_skipNew; }
    static void SetSaveModifiedSkipNew (bool skip) { m_saveModified_skipNew = skip; }

    static bool GetEnableOpenUnexisting () { return m_enableOpenUnexisting; }
    static void SetEnableOpenUnexisting (bool enable) { m_enableOpenUnexisting = enable; }
    struct EnableOpenUnexisting
    {
        EnableOpenUnexisting ()  { SetEnableOpenUnexisting(true); }
        ~EnableOpenUnexisting () { SetEnableOpenUnexisting(false); }
    };

    static void LoadSettingsManager ();
    static void SaveSettingsManager ();
    static void ShowSettingsDialog ();
    static const OpenEditor::SettingsManager& GetSettingsManager ();

    OpenEditor::Storage& GetStorage ()                  { return m_storage; }
    const OpenEditor::InstanceSettings& GetSettings ()  { return m_settings; }

	//{{AFX_VIRTUAL(COEDocument)
	public:
	  virtual BOOL IsModified();
	  virtual BOOL OnNewDocument();
	  virtual void SetTitle(LPCTSTR lpszTitle);
	  virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	  virtual void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE);
	  virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
    virtual BOOL SaveModified ();
    void SetModifiedFlag (BOOL bModified = TRUE);
	//}}AFX_VIRTUAL
    virtual BOOL DoFileSave();
	  virtual BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace = TRUE);
    virtual void OnIdle();
    virtual BOOL OnIdle(LONG);

protected:
    BOOL doSave (LPCTSTR lpszPathName, BOOL bReplace);
	//{{AFX_MSG(COEDocument)
    afx_msg void OnEditPrintPageSetup();
	afx_msg void OnEditFileSettings();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnFileReload();
    afx_msg void OnUpdate_FileReload(CCmdUI *pCmdUI);
    afx_msg void OnUpdate_FileType  (CCmdUI* pCmdUI);
    afx_msg void OnUpdate_FileLines (CCmdUI* pCmdUI);

    afx_msg void OnEditViewLineNumbers ();
    afx_msg void OnUpdate_EditViewLineNumbers(CCmdUI *pCmdUI);
  	afx_msg void OnEditViewWhiteSpace ();
	  afx_msg void OnUpdate_EditViewWhiteSpace (CCmdUI* pCmdUI);

    afx_msg void OnFileOpen ();
    //afx_msg void OnChangedViewList();
};

inline
    LPCVOID COEDocument::GetVMData () const { return m_vmdata; }

inline
    const OpenEditor::SettingsManager& 
        COEDocument::GetSettingsManager () { return m_settingsManager; }

//{{AFX_INSERT_LOCATION}}

#endif//__OEDocument_h__
