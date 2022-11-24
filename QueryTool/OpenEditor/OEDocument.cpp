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

#include "pch.h"
#include "Shlwapi.h"
#include "COMMON/AppUtilities.h"
#include "COMMON/AppGlobal.h"
#include <COMMON/ExceptionHelper.h>
#include "COMMON\StrHelpers.h"
#include "COMMON/VisualAttributesPage.h"
#include "COMMON/PropertySheetMem.h"
#include "OEDocument.h"
#include "OEView.h"
#include "OEPrintPageSetup.h"
#include "OEEditingPage.h"
#include "OEBlockPage.h"
#include "OEClassPage.h"
#include "OESettings.h"
#include "OESettingsStreams.h"
#include <OELanguageStreams.h>
#include "OETemplateStreams.h"
#include "OESettingsDlg.h"

#include "OEGeneralPage.h"
#include "OEFilePage.h"
#include "OEBackupPage.h"
#include "OETemplatesPage.h"
#include "OEDocument.h"
#include "OEDocumentDlg.h"
#include <OESQLSettingsPage.h>
#include "Query/QueryPanelWnd.h"
#include <io.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COEDocument

using namespace Common;
using namespace OpenEditor;

#define CHECK_FILE_OPERATION(r) if(!(r)) file_exeption::check_last_error();

CString         COEDocument::m_settingsDir;
SettingsManager COEDocument::m_settingsManager;
Searcher        COEDocument::m_searcher(true);
bool            COEDocument::m_saveModified_silent = false;
bool            COEDocument::m_saveModified_skipNew = false;
bool            COEDocument::m_enableOpenUnexisting = false;

IMPLEMENT_DYNCREATE(COEDocument, CDocumentExt)

BEGIN_MESSAGE_MAP(COEDocument, CDocument)
  ON_COMMAND(ID_FILE_OPEN,              OnFileOpen)
  ON_COMMAND(ID_FILE_RELOAD,            OnFileReload)
  ON_COMMAND(ID_EDIT_PRINT_PAGE_SETUP,  OnEditPrintPageSetup)
	ON_COMMAND(ID_EDIT_FILE_SETTINGS,     OnEditFileSettings)
  ON_COMMAND(ID_EDIT_VIEW_LINE_NUMBERS, OnEditViewLineNumbers)
  ON_COMMAND(ID_EDIT_VIEW_WHITE_SPACE,  OnEditViewWhiteSpace)

  ON_UPDATE_COMMAND_UI(ID_FILE_RELOAD,            OnUpdate_FileReload)
  ON_UPDATE_COMMAND_UI(ID_FILE_SYNC_LOCATION,     OnUpdate_FileReload)
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_FILE_TYPE,    OnUpdate_FileType)
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_FILE_LINES,   OnUpdate_FileLines)
  ON_UPDATE_COMMAND_UI(ID_EDIT_VIEW_LINE_NUMBERS, OnUpdate_EditViewLineNumbers)
	ON_UPDATE_COMMAND_UI(ID_EDIT_VIEW_WHITE_SPACE,  OnUpdate_EditViewWhiteSpace)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COEDocument construction/destruction

COEDocument::COEDocument()
{
  try
  {
    m_vmdata = NULL;
    m_orgModified = false;
    m_newOrSaveAs = false;
    m_extension   = "SQL";
    m_extensionInitialized = false;
    m_settings.SetClassSetting(&m_settingsManager.GetDefaults());
    m_storage.SetSettings(&m_settings);
    m_storage.SetSearcher(&m_searcher);
  }
  _OE_DEFAULT_HANDLER_;
}


COEDocument::~COEDocument()
{
  CWaitCursor wait;

  try 
  {
    if(m_vmdata)
    {
      VirtualFree(m_vmdata,0,MEM_RELEASE);
    }
    m_storage.Clear();
    m_mmfile.Close();
  }
  catch (...) { DEFAULT_HANDLER_ALL; }

  // for safe delete
  //m_storage.SetSettings(&m_settingsManager.GetDefaults());
}


BOOL COEDocument::IsModified()
{
    return m_storage.IsModified() ? TRUE : FALSE;
}


BOOL COEDocument::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// COEDocument othes methods

void COEDocument::SetPathName (LPCTSTR lpszPathName, BOOL bAddToMRU)
{
	CDocument::SetPathName(lpszPathName, bAddToMRU);

  std::string extension;
  const char* ext = strrchr(lpszPathName, '.');
    
  if(ext)
  {
    extension = ++ext;
  }
  if (!m_extensionInitialized || extension != m_extension)
  {
    m_extension = extension;
    if (!m_extensionInitialized
      || AfxMessageBox("Filename extension changed. Auto-setup for this extension?", MB_YESNO | MB_ICONQUESTION) == IDYES)
    {
      m_settings.SetClassSetting(&m_settingsManager.FindByExt(m_extension.c_str(), true));
    }
    m_extensionInitialized = true;
  }
}

void
COEDocument::SetTitle()
{
  CString title = (m_orgModified ? m_orgTitle + " *" : m_orgTitle).c_str();
  theApp.SetTitle(title);
}

void 
COEDocument::SetTitle (LPCTSTR lpszTitle)
{
  m_orgTitle = lpszTitle;
  CString title = (m_orgModified ? m_orgTitle + " *" : m_orgTitle).c_str();
  CDocument::SetTitle(title);
  theApp.SetTitle(title);
}

void 
COEDocument::OnIdle ()
{
  if (m_orgModified != m_storage.IsModified())
  {
    m_orgModified = m_storage.IsModified();
    CString title = (m_orgModified ? m_orgTitle + " *" : m_orgTitle).c_str();
    CDocument::SetTitle(title);
    theApp.SetTitle(title);
  }
}

BOOL 
COEDocument::OnIdle (LONG)
{
  return m_storage.OnIdle() ? TRUE : FALSE;
}

BOOL 
COEDocument::DoFileSave ()
{
  DWORD dwAttrib = m_strPathName.IsEmpty() ? INVALID_FILE_ATTRIBUTES : GetFileAttributes(m_strPathName);
    
  if (dwAttrib != INVALID_FILE_ATTRIBUTES && dwAttrib & FILE_ATTRIBUTE_READONLY)
  {
    if (m_settings.GetFileOverwriteReadonly())
    {
      SetFileAttributes(m_strPathName, dwAttrib & (~FILE_ATTRIBUTE_READONLY));
    }
    else
    {
      switch (StyleMessageBox(AfxGetMainWnd()
              ,GetPathName() 
              + "\n\nThis file cannot be saved because it is write-protected.\t"
                "\nYou can either save the file in a different location [Yes],"
                "\nor Program can attempt to remove write-protection"
                "\nand overwrite the file in its current location [No].",
                "Save of Read-Only File", MB_YESNOCANCEL|MB_ICONEXCLAMATION)
          )
      {
        case IDCANCEL:  // cancel saving
                        return FALSE;
        case IDNO:      // remove write-protection here
                        SetFileAttributes(m_strPathName, dwAttrib & (~FILE_ATTRIBUTE_READONLY));
        case IDYES:     // try "Save As"
                        break;
      }
    }
  }

  if (CDocument::DoFileSave())
  {
    if (!m_settings.GetUndoAfterSaving())
    {
      m_storage.ClearUndo();
    }
    return TRUE;
  }
  return FALSE;
}

BOOL 
COEDocument::DoSave (LPCTSTR lpszPathName, BOOL bReplace)
{
  try
  {
    if (!lpszPathName) m_newOrSaveAs = true;
    BOOL retVal = doSave(lpszPathName, bReplace);
    m_newOrSaveAs = false;
    return retVal;
  }
  catch (...)
  {
    m_newOrSaveAs = false;
    throw;
  }
}

void 
COEDocument::loadFile (const char* path, bool reload, bool external)
{
    CWaitCursor wait;
    LPVOID vmdata = 0;
    bool locking = m_settings.GetFileLocking();
    try
    {
        unsigned long length = 0;
        MemoryMappedFile mmfile;

        if (!locking && m_settings.GetFileMemMapForBig())
        {
            DWORD attrs;
            __int64 fileSize;
            if (Common::AppGetFileAttrs(path, &attrs, &fileSize) 
            && fileSize > __int64(m_settings.GetFileMemMapThreshold()) * (1024 * 1024))
            {
                locking = true;
            }
        }

        mmfile.Open(path, locking ? emmoRead|emmoWrite|emmoShareRead : emmoRead|emmoShareRead|emmoShareWrite, 0);
        length = mmfile.GetDataLength();

        if (locking) // it's equal to using of mm
        {
            MemoryMappedFile::Swap(mmfile, m_mmfile);
            m_storage.SetText((const char*)m_mmfile.GetData(), length, true, reload, external);
            m_storage.SetSavedUndoPos();
        }
        else
        {
            if (length > 0)
            {
                vmdata = VirtualAlloc(NULL, length, MEM_COMMIT, PAGE_READWRITE);
                memcpy(vmdata, mmfile.GetData(), length);
                m_storage.SetText((const char*)vmdata, length, true, reload, external);
            }
            else
            {
                m_storage.SetText("", length, true, reload, external);
            }

            m_storage.SetSavedUndoPos();
            if (m_vmdata) VirtualFree(m_vmdata, 0, MEM_RELEASE);
            m_vmdata = vmdata;
            vmdata = 0;
        }
    }
    catch (file_exeption)
    {
      throw;
    }
    catch (const std::exception& x)   
    { 
        try { if (vmdata) VirtualFree(m_vmdata, 0, MEM_RELEASE);} catch (...) {}
        try { m_mmfile.Close(); } catch (...) {}
        throw std::exception((std::string(locking ? "Cannot open file and lock it.\n" : "Cannot open file.\n") + x.what()).c_str());
    }
    catch (...) 
    { 
        try { if (vmdata) VirtualFree(m_vmdata, 0, MEM_RELEASE);} catch (...) {}
        try { m_mmfile.Close(); } catch (...) {}
        throw;
    }
}


BOOL COEDocument::OnOpenDocument (LPCTSTR lpszPathName) 
{
    try 
    {
        _ASSERTE(!m_mmfile.IsOpen() && !m_vmdata);

        DWORD dummy;
        if (m_enableOpenUnexisting 
        && !AppGetFileAttrs(lpszPathName, &dummy))
        {
            if (COEDocument::OnNewDocument())
            {
	            SetPathName(lpszPathName);
                return TRUE;
            }
            return FALSE;
        }
        loadFile(lpszPathName);
    }
    catch (file_exeption& f)
    {
      CString message;
      message.Format("Cannot open document file '%s'\n%s",lpszPathName,f.what());
      AfxMessageBox(message,MB_OK|MB_ICONERROR);
      return FALSE;
    }
    catch (const std::exception& x)   
    { 
        DEFAULT_HANDLER(x); 
        return FALSE;
    }
    catch (StdException& x)
    {
      DEFAULT_HANDLER(x);
      return false;
    }
    catch (...)   
    { 
        DEFAULT_HANDLER_ALL; 
        return FALSE;
    }

    CFileWatchClient::StartWatching(lpszPathName);

	return TRUE;
}

BOOL COEDocument::OnSaveDocument (LPCTSTR lpszPathName)
{
  bool storage_invalidated = false;
	char tmpname[_MAX_PATH];

  try 
  {
      backupFile(lpszPathName);       // create backup file
      m_storage.TruncateSpaces();     // truncate spaces before size allocation
      unsigned long length = m_storage.GetTextLength(); 
        
      // create mm file and copy data
      MemoryMappedFile outfile;

      // if we using mm file then we have to use an intermediate file for file saving
      if (m_mmfile.IsOpen())
      {
	      char *name, path[_MAX_PATH];
	      // get the directory for the intermediate file
        CHECK_FILE_OPERATION(::GetFullPathName(lpszPathName, sizeof(path), path, &name));
	      *name = 0; // cut file name
	      // get a temporary name for the intermediate file
        CHECK_FILE_OPERATION(::GetTempFileName(path, "ODBC~", 0, tmpname));
        // open output mm file
        outfile.Open(tmpname, emmoCreateAlways|emmoWrite|emmoShareRead, length);
      }
      else
      {
          // suspend file watching
          CFileWatchClient::StopWatching();
          // open output mm file
          outfile.Open(lpszPathName, emmoCreateAlways|emmoWrite|emmoShareRead, length);
      }

      // copy text to output mm file
      m_storage.GetText((char*)outfile.GetData(), length);
      outfile.Close();

      if (m_mmfile.IsOpen())
      {
          storage_invalidated = true;
          // suspend file watching
          CFileWatchClient::StopWatching();
          m_mmfile.Close();
          // delete the original file we have to ignore failure for SaveAs operation
          ::DeleteFile(lpszPathName);
          // rename tmp file to the original
          CHECK_FILE_OPERATION(!::rename(tmpname, lpszPathName));
      }

      // open a new copy and reload a file
      loadFile(lpszPathName, true/*reload*/);
        
      // resume file watching
      CFileWatchClient::StartWatching(lpszPathName);
  }
  catch (const std::exception& x)   
  { 
      if (storage_invalidated) m_storage.Clear();
      AfxMessageBox((std::string("Cannot save file.\n") + x.what()).c_str(), MB_OK|MB_ICONSTOP);
      return FALSE;
  }
  catch (...) 
  { 
      if (storage_invalidated) m_storage.Clear();
      AfxMessageBox("Cannot save file. Unknown error.\n", MB_OK|MB_ICONSTOP);
      return FALSE;
  }

  return TRUE;
}

BOOL COEDocument::doSave (LPCTSTR lpszPathName, BOOL bReplace)
	// Save the document data to a file
	// lpszPathName = path name where to save document file
	// if lpszPathName is NULL then the user will be prompted (SaveAs)
	// note: lpszPathName can be different than 'm_strPathName'
	// if 'bReplace' is TRUE will change file name if successful (SaveAs)
	// if 'bReplace' is FALSE will not change path name (SaveCopyAs)
{
	CString newName = lpszPathName;
	
  if (newName.IsEmpty())
	{
    // 06.18.2003 bug fix, SaveFile dialog occurs for sql files which are loaded from db 
    //            if "save files when switching takss" is activated.
		//newName = m_strPathName;
    GetNewPathName(newName);

		if (bReplace && newName.IsEmpty())
		{
			newName = m_strTitle;
			// check for dubious filename
			int iBad = newName.FindOneOf(_T(":/\\* "));
			if (iBad != -1)
      {
        newName.GetBuffer(iBad);
				newName.ReleaseBuffer(iBad);
      }
			// append the default suffix if there is one
			int iStart = 0;
      CString strExt = m_settings.GetExtensions().c_str();
#if _MFC_VER > 0x0600
			newName += '.' + strExt.Tokenize(_T(" ;,"), iStart);
#else
      int iEnd = strExt.FindOneOf(_T(" ;,"));
			newName += '.' + ((iEnd != -1) ? strExt.Left(iEnd) : strExt);
#endif
		}
		if (!AfxGetApp()->DoPromptFileName(newName
                                      ,bReplace ? AFX_IDS_SAVEFILE : AFX_IDS_SAVEFILECOPY
                                      ,OFN_HIDEREADONLY | OFN_PATHMUSTEXIST 
                                      ,FALSE
                                      ,NULL/*pTemplate*/))
    {
			return FALSE; // don't even attempt to save
    }
	}
	CWaitCursor wait;

	if (!OnSaveDocument(newName))
	{
		if (lpszPathName == NULL)
		{
			// be sure to delete the file
			try 
      {
				CFile::Remove(newName);
      }  
      catch (...) 
      {
				TRACE0("Warning: failed to delete file after failed SaveAs.\n");
        DEFAULT_HANDLER_ALL; 
			}
		}
		return FALSE;
	}
	// reset the title and change the document name
	if (bReplace)
  {
    SetPathName(newName);
  }
	return TRUE;        // success
}

void COEDocument::backupFile (LPCTSTR lpszPathName)
{
  EBackupMode backupMode = (EBackupMode)m_settings.GetFileBackup();

  if (!m_newOrSaveAs &&  backupMode != ebmNone)
  {
    int length = (int)strlen(lpszPathName);

    if (lpszPathName && length)
    {
      std::string backupPath;

      switch (backupMode)
      {
        case ebmCurrentDirectory:
          {
              std::string path, name, ext;
              
              CString buff(lpszPathName);
              ::PathRemoveFileSpec(buff.LockBuffer()); buff.UnlockBuffer();
              path = buff;

              buff = ::PathFindExtension(lpszPathName);
              ext = (!buff.IsEmpty() && buff[0] == '.') ? (LPCSTR)buff + 1 : buff;

              buff = ::PathFindFileName(lpszPathName);
              ::PathRemoveExtension(buff.LockBuffer()); buff.UnlockBuffer();
              name = buff;

              Common::Substitutor substr;
              substr.AddPair("<NAME>", name);
              substr.AddPair("<EXT>",  ext);
              substr << m_settings.GetFileBackupName().c_str();

              backupPath = path + '\\' + substr.GetResult();
          }
          break; // 09.03.2003 bug fix, backup file problem if a destination is the current folder
        case ebmBackupDirectory:
          {
            std::string path, name;
            
            path = m_settings.GetFileBackupDirectory();

            if (path.rbegin() != path.rend() && *path.rbegin() != '\\')
                path += '\\';

            name = ::PathFindFileName(lpszPathName);

            backupPath = path + name;
          }
          break;
      }

        if (m_lastBackupPath != backupPath              // only for the first attemption
        && _stricmp(GetPathName(), backupPath.c_str()))  // avoiding copy the file into itself
        {
            _CHECK_AND_THROW_(
                CopyFile(lpszPathName, backupPath.c_str(), FALSE) != 0,
                "Cannot create backup file!"
                );
        m_lastBackupPath = backupPath;
      }
    }
  }
}

BOOL COEDocument::SaveModified ()
{
  if (m_saveModified_skipNew && m_strPathName.IsEmpty())
    return TRUE;
  else if (m_saveModified_silent && !m_strPathName.IsEmpty())
    return DoFileSave() ? TRUE : FALSE;
  else
    return CDocument::SaveModified();
}

void COEDocument::SetModifiedFlag (BOOL bModified)
{
  if (!bModified)
    m_storage.SetSavedUndoPos(); // 14.04.2003 bug fix, SetModifiedFlag clears an undo history
  else
    _ASSERTE(0);
}

void COEDocument::OnEditPrintPageSetup()
{
    SettingsManager mgr(m_settingsManager);

    if (COEPrintPageSetup(mgr).DoModal() == IDOK)
    {
        m_settingsManager = mgr;
        SaveSettingsManager();
    }
}

void COEDocument::SetText (LPVOID vmdata, unsigned long length)
{
    m_storage.Clear();
    m_mmfile.Close();
    if (m_vmdata) VirtualFree(m_vmdata, 0, MEM_RELEASE);

    m_vmdata = vmdata;
    m_storage.SetText((const char*)m_vmdata, length, true/*use_buffer*/, false/*reload*/, false/*external*/);
}

void COEDocument::SetText (const char* text, unsigned long length)
{
    LPVOID vmdata = VirtualAlloc(NULL, length, MEM_COMMIT, PAGE_READWRITE);
    try
    {
        memcpy(vmdata, text, length);
        SetText(vmdata, length);
    }
    catch (...)
    {
        if (vmdata != m_vmdata)
            VirtualFree(vmdata, 0, MEM_RELEASE);
    }
}

void COEDocument::LoadSettingsManager()
{
  std::string path;
  AppGetPath(path);

  if(m_settingsDir.IsEmpty())
  {
    CString dir;
    Common::FindApplicDirectory(dir);
    m_settingsDir = dir + "\\EDO\\OpenODBCQuerytool\\";
  }

  // Try to load from the users path
  std::string settingsdir(m_settingsDir.GetBuffer());
  std::string languages = settingsdir + "languages.dat";
  std::string templates = settingsdir + "templates.dat";
  std::string settings  = settingsdir + "settings.dat";

  // Load language support
  if(_access(languages.c_str(),0) == 0)
  {
    LanguagesCollectionReader(FileInStream(languages.c_str())) >> LanguagesCollection();
  }
  else
  {
    LanguagesCollectionReader(FileInStream((path + "\\data\\languages.dat").c_str())) >> LanguagesCollection();
  }
  // Load template settings
  if(_access(templates.c_str(),0) == 0)
  {
    TemplateCollectionReader(FileInStream(templates.c_str())) >> m_settingsManager;
  }
  else
  {
    TemplateCollectionReader(FileInStream((path + "\\data\\templates.dat").c_str())) >> m_settingsManager;
  }
  // Load editor settings
  if(_access(settings.c_str(),0) == 0)
  {
    SettingsManagerReader(FileInStream(settings.c_str())) >> m_settingsManager;
  }
  else
  {
    SettingsManagerReader(FileInStream((path + "\\data\\settings.dat").c_str())) >> m_settingsManager;
  }
}

static bool g_isBackupDone = false;

void COEDocument::SaveSettingsManager()
{
    std::string path;
    AppGetPath(path);

    if(m_settingsDir.IsEmpty())
    {
      CString dir;
      Common::FindApplicDirectory(dir);
      m_settingsDir = dir + "\\EDO\\OpenODBCQuerytool\\";
    }

    std::string settingsdir(m_settingsDir.GetBuffer());
    std::string orgtemplates = path + "\\data\\templates.dat";
    std::string orgsettings  = path + "\\data\\settings.dat";
    std::string owntemplates = settingsdir + "templates.dat";
    std::string ownsettings  = settingsdir + "settings.dat";
    std::string baktemplates = owntemplates + ".bak";
    std::string baksettings  = ownsettings  + ".bak";

    if(_access(owntemplates.c_str(),0) == 0 &&
       _access(ownsettings .c_str(),0) == 0)
    {
      // Own files already exist. Should we make a backup?
      if(!g_isBackupDone)
      {
        // old settings backup
        _CHECK_AND_THROW_(CopyFile(owntemplates.c_str(),baktemplates.c_str(), FALSE) != 0 && 
                          CopyFile(ownsettings .c_str(),baksettings .c_str(), FALSE) != 0
                         ,"Settings backup file creation error!");
        g_isBackupDone = true;
      }
    }
    else
    {
      // Our files do not exist. Create them
      CopyFile(orgtemplates.c_str(),owntemplates.c_str(),FALSE);
      CopyFile(orgsettings .c_str(),ownsettings .c_str(),FALSE);
    }
    // save new settings
    TemplateCollectionWriter(FileOutStream(owntemplates.c_str())) << m_settingsManager;
    SettingsManagerWriter   (FileOutStream(ownsettings .c_str())) << m_settingsManager;
}

void COEDocument::ShowSettingsDialog()
{
  SettingsManager mgr(m_settingsManager);
  COESettingsDlg dlg(mgr,theApp.m_pMainWnd);

  if (dlg.DoModal() == IDOK)
  {
    m_settingsManager = mgr;
    SaveSettingsManager();
  }
}

void 
COEDocument::OnEditFileSettings()
{
  InstanceSettings settings(m_settings);

  COEDocumentDlg dlg(this,settings,theApp.m_pMainWnd);
  if(dlg.DoModal() == IDOK)
  {
    m_settings = settings;
  }
}

void 
COEDocument::OnWatchedFileChanged ()
{
    try
    {
        // ignore notification if it's a disabled feature
        if (m_settings.GetFileDetectChanges())
        {
            CString prompt = GetPathName();
            prompt += "\n\nThis file has been modified outside of the editor."
                      "\nDo you want to reload it";
            prompt += !IsModified() ? "?" : " and lose the changes made in the editor?";

            if ((m_settings.GetFileReloadChanges() && !IsModified())
            || AfxMessageBox(prompt, 
                !IsModified() ? MB_YESNO|MB_ICONQUESTION : MB_YESNO|MB_ICONEXCLAMATION) == IDYES)
            {
                vector<COEditorView*> viewsToScroll;
                if (m_settings.GetFileAutoscrollAfterReload())
                {
                    POSITION pos = GetFirstViewPosition();
                    while (pos != NULL) 
                    {
                        CView* pView = GetNextView(pos);
                        if (pView && (pView->IsKindOf(RUNTIME_CLASS(COEditorView))))
                        {
                            Position pos = ((COEditorView*)pView)->GetPosition();
                            if (pos.line >= m_storage.GetLineCount()-1)
                                viewsToScroll.push_back(((COEditorView*)pView));
                        }
                    }
                }
                Common::SetStatusText(CString("Reloading file \"") + GetPathName() + "\" ...");
                CFileWatchClient::UpdateWatchInfo();
                loadFile(GetPathName(), true/*reload*/, true/*external*/);
                Common::SetStatusText(CString("File \"") + GetPathName() + "\" reloaded.");

                if (viewsToScroll.size())
                {
                    
                    for (std::vector<COEditorView*>::iterator it = viewsToScroll.begin();
                        it != viewsToScroll.end(); ++it)
                        (*it)->MoveToBottom();
                }

            }
        }
    }
    _OE_DEFAULT_HANDLER_
}

void COEDocument::OnFileReload ()
{
    try
    {
        if (!GetPathName().IsEmpty())
        {
            CString prompt = "Do you want to reload \n\n" + GetPathName() 
                    + "\n\nand lose the changes made in the editor?";

            if (!IsModified() || AfxMessageBox(prompt, MB_YESNO|MB_ICONEXCLAMATION) == IDYES)
            {
                
                Common::SetStatusText(CString("Reloading file \"") + GetPathName() + "\" ...");
                loadFile(GetPathName(), true/*reload*/, true/*external*/);
                Common::SetStatusText(CString("File \"") + GetPathName() + "\" reloaded.");
            }
        }
    }
    _OE_DEFAULT_HANDLER_
}

void COEDocument::OnUpdate_FileReload (CCmdUI *pCmdUI)
{
    pCmdUI->Enable(!GetPathName().IsEmpty());
}

void COEDocument::OnUpdate_FileType (CCmdUI* pCmdUI)
{
    pCmdUI->SetText(m_storage.GetFileFormatName());
    pCmdUI->Enable(TRUE);
}

void COEDocument::OnUpdate_FileLines (CCmdUI* pCmdUI)
{
    char buff[80];
    sprintf_s(buff,80," Lines: %d ", m_storage.GetLineCount());
    pCmdUI->SetText(buff);
    pCmdUI->Enable(TRUE);
}

void COEDocument::OnEditViewWhiteSpace()
{
    m_settings.SetVisibleSpaces(!m_settings.GetVisibleSpaces());
}

void COEDocument::OnUpdate_EditViewWhiteSpace(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_settings.GetVisibleSpaces() ? TRUE : FALSE);
}

void COEDocument::OnEditViewLineNumbers ()
{
    m_settings.SetLineNumbers(!m_settings.GetLineNumbers());
}

void COEDocument::OnUpdate_EditViewLineNumbers (CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck(m_settings.GetLineNumbers() ? TRUE : FALSE);
}

void COEDocument::OnFileOpen ()
{
    CString path = GetPathName();
    if (m_settings.GetWorkDirFollowsDocument() && !path.IsEmpty())
    {
        PathRemoveFileSpec(path.LockBuffer());
        path.UnlockBuffer();
    }
    CDocManagerExt::SetFileOpenPath(path);
    AfxGetApp()->OnCmdMsg(ID_FILE_OPEN, 0, 0, 0);
    CDocManagerExt::SetFileOpenPath("");
}

/*
void COEDocument::OnChangedViewList()
{
   POSITION pos = GetFirstViewPosition();
   while (pos != NULL)
   {
      CView* pView = GetNextView(pos);
      CQueryPanelWnd* panel;
      panel = dynamic_cast<CQueryPanelWnd* > (pView);
      if(panel)
      {
        RemoveView(pView);
        break;
      }
   }     
}
*/