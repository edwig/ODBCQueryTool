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
#include <StyleMessageBox.h>
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

#define CHECK_FILE_OPERATION(r) if(!(r)) throw StdException(_T("File_exeption::check_last_error()"));

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
    m_extension   = _T("SQL");
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
    // m_mmfile.Close();
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
// COEDocument other methods

void COEDocument::SetPathName (LPCTSTR lpszPathName, BOOL bAddToMRU)
{
	CDocument::SetPathName(lpszPathName, bAddToMRU);

  CString extension;
  LPCTSTR ext = _tcsrchr(lpszPathName, _T('.'));
    
  if(ext)
  {
    extension = ++ext;
  }
  if (!m_extensionInitialized || extension != m_extension)
  {
    m_extension = extension;
    if (!m_extensionInitialized
      || AfxMessageBox(_T("Filename extension changed. Auto-setup for this extension?"), MB_YESNO | MB_ICONQUESTION) == IDYES)
    {
      m_settings.SetClassSetting(&m_settingsManager.FindByExt(m_extension.GetString(), true));
    }
    m_extensionInitialized = true;
  }
}

void
COEDocument::SetTitle()
{
  CString title = (m_orgModified ? m_orgTitle + _T(" *") : m_orgTitle);
  theApp.SetTitle(title);
}

void 
COEDocument::SetTitle (LPCTSTR lpszTitle)
{
  m_orgTitle = lpszTitle;
  CString title = (m_orgModified ? m_orgTitle + _T(" *") : m_orgTitle);
  CDocument::SetTitle(title);
  theApp.SetTitle(title);
}

void 
COEDocument::OnIdle ()
{
  if (m_orgModified != m_storage.IsModified())
  {
    m_orgModified = m_storage.IsModified();
    CString title = (m_orgModified ? m_orgTitle + _T(" *") : m_orgTitle);
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
              + _T("\n\nThis file cannot be saved because it is write-protected.\t")
                _T("\nYou can either save the file in a different location [Yes],")
                _T("\nor Program can attempt to remove write-protection")
                _T("\nand overwrite the file in its current location [No]."),
                _T("Save of Read-Only File"), MB_YESNOCANCEL|MB_ICONEXCLAMATION)
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
COEDocument::loadFile(LPCTSTR path,bool reload,bool external)
{
  CWaitCursor wait;
  bool locking = m_settings.GetFileLocking();

  CString total;
  WinFile file(path);
  if(file.Open(winfile_read | open_trans_text | (locking ? open_shared_read : open_shared_write)))
  {
    CString line;
    while(file.Read(line))
    {
      total += line;
    }
    file.Close();

    m_storage.SetText(total.GetString(),total.GetLength(),false,reload,external);
    m_storage.SetSavedUndoPos();
    return;
  }
  throw StdException(CString(locking ? _T("Cannot open file and lock it:\n") : _T("Cannot open file:\n")) + path);
}

// void 
// COEDocument::loadFile (LPCTSTR path, bool reload, bool external)
// {
//     CWaitCursor wait;
//     LPVOID vmdata = 0;
//     bool locking = m_settings.GetFileLocking();
//     try
//     {
//         unsigned long length = 0;
//         MemoryMappedFile mmfile;
// 
//         if (!locking && m_settings.GetFileMemMapForBig())
//         {
//             DWORD attrs;
//             __int64 fileSize;
//             if (Common::AppGetFileAttrs(path, &attrs, &fileSize) 
//             && fileSize > __int64(m_settings.GetFileMemMapThreshold()) * (1024 * 1024))
//             {
//                 locking = true;
//             }
//         }
// 
//         mmfile.Open(path, locking ? emmoRead|emmoWrite|emmoShareRead : emmoRead|emmoShareRead|emmoShareWrite, 0);
//         length = mmfile.GetDataLength();
// 
//         if (locking) // it's equal to using of mm
//         {
//             MemoryMappedFile::Swap(mmfile, m_mmfile);
//             m_storage.SetText((LPCTSTR)m_mmfile.GetData(), length, true, reload, external);
//             m_storage.SetSavedUndoPos();
//         }
//         else
//         {
//             if (length > 0)
//             {
//                 vmdata = VirtualAlloc(NULL, length, MEM_COMMIT, PAGE_READWRITE);
//                 memcpy(vmdata, mmfile.GetData(), length);
//                 m_storage.SetText((LPCTSTR)vmdata, length, true, reload, external);
//             }
//             else
//             {
//                 m_storage.SetText(_T(""), length, true, reload, external);
//             }
// 
//             m_storage.SetSavedUndoPos();
//             if (m_vmdata) VirtualFree(m_vmdata, 0, MEM_RELEASE);
//             m_vmdata = vmdata;
//             vmdata = 0;
//         }
//     }
//     catch (file_exeption)
//     {
//       throw;
//     }
//     catch (const std::exception& x)   
//     { 
//         try { if (vmdata) VirtualFree(m_vmdata, 0, MEM_RELEASE);} catch (...) {}
//         try { m_mmfile.Close(); } catch (...) {}
//         throw StdException(CString(locking ? _T("Cannot open file and lock it.\n") : _T("Cannot open file.\n")) + x.what());
//     }
//     catch (...) 
//     { 
//         try { if (vmdata) VirtualFree(m_vmdata, 0, MEM_RELEASE);} catch (...) {}
//         try { m_mmfile.Close(); } catch (...) {}
//         throw;
//     }
// }

BOOL COEDocument::OnOpenDocument (LPCTSTR lpszPathName) 
{
  try 
  {
    DWORD dummy;
    if (m_enableOpenUnexisting && !AppGetFileAttrs(lpszPathName, &dummy))
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
  catch (StdException& f)
  {
    CString message;
    message.Format(_T("Cannot open document file '%s'\n%s"),lpszPathName,f.GetErrorMessage().GetString());
    AfxMessageBox(message,MB_OK|MB_ICONERROR);
    return FALSE;
  }
  catch (const std::exception& x)   
  { 
      DEFAULT_HANDLER(x); 
      return FALSE;
  }
  catch (...)   
  { 
      DEFAULT_HANDLER_ALL; 
      return FALSE;
  }
  CFileWatchClient::StartWatching(lpszPathName);
	return TRUE;
}

BOOL COEDocument::OnSaveDocument(LPCTSTR lpszPathName)
{
  bool storage_invalidated = false;
  TCHAR tmpname[_MAX_PATH + 1];

  try
  {
    backupFile(lpszPathName);       // create backup file
    m_storage.TruncateSpaces();     // truncate spaces before size allocation

    // CREATE TEMPORARY FILE FOR SAVING
    // make it in the same directory as the original file
    LPTSTR name;
    TCHAR  path[_MAX_PATH + 1];
    // get the directory for the intermediate file
    CHECK_FILE_OPERATION(::GetFullPathName(lpszPathName,_MAX_PATH,path,&name));
    *name = 0; // cut file name
    // get a temporary name for the intermediate file
    CHECK_FILE_OPERATION(::GetTempFileName(path,_T("ODBC"),0,tmpname));

    // WRITE THE CONTENT TO THE TEMPORARY FILE
    WinFile file(tmpname);
    if(file.Open(winfile_write | open_trans_text,FAttributes::attrib_none,Encoding::UTF8))
    {
      LPCTSTR line = nullptr;
      int lines = m_storage.GetLineCount();
      for(int index = 0;index < lines; ++index)
      {
        m_storage.GetLine(index,line);
        CString str(line);
        str += _T("\n");
        if(!file.Write(str))
        {
          return FALSE;
        }
      }
      if(!file.Close())
      {
        return FALSE;
      }
    }
    else
    {
      return FALSE;
    }

    // REPLACE THE FILE WITH THE TEMPORARY ONE
    // suspend file watching
    CFileWatchClient::StopWatching();

    // DELETE ORIGINAL FILE AND REPLACE BY TEMP FILE
    storage_invalidated = true;
    // delete the original file we have to ignore failure for SaveAs operation
    ::DeleteFile(lpszPathName);
    // rename tmp file to the original
    CHECK_FILE_OPERATION(!::_trename(tmpname,lpszPathName));

    // resume file watching
    CFileWatchClient::StartWatching(lpszPathName);
  }
  catch(StdException& x)
  {
    if(storage_invalidated)
    {
      m_storage.Clear();
    }
    AfxMessageBox(CString(_T("Cannot save file.\n")) + x.GetErrorMessage(),MB_OK | MB_ICONSTOP);
    return FALSE;
  }
  catch(...)
  {
    if(storage_invalidated)
    {
      m_storage.Clear();
    }
    AfxMessageBox(_T("Cannot save file. Unknown error.\n"),MB_OK | MB_ICONSTOP);
    return FALSE;
  }
  SetModifiedFlag(FALSE);
  return TRUE;
}

// BOOL COEDocument::OnSaveDocument (LPCTSTR lpszPathName)
// {
//   bool storage_invalidated = false;
// 	TCHAR tmpname[_MAX_PATH];
// 
//   try 
//   {
//       backupFile(lpszPathName);       // create backup file
//       m_storage.TruncateSpaces();     // truncate spaces before size allocation
//       unsigned long length = m_storage.GetTextLength(); 
//         
//       // create mm file and copy data
//       MemoryMappedFile outfile;
// 
//       // if we using mm file then we have to use an intermediate file for file saving
//       if (m_mmfile.IsOpen())
//       {
//         LPTSTR name;
//         TCHAR  path[_MAX_PATH];
// 	      // get the directory for the intermediate file
//         CHECK_FILE_OPERATION(::GetFullPathName(lpszPathName, sizeof(path), path, &name));
// 	      *name = 0; // cut file name
// 	      // get a temporary name for the intermediate file
//         CHECK_FILE_OPERATION(::GetTempFileName(path, _T("ODBC~"), 0, tmpname));
//         // open output mm file
//         outfile.Open(tmpname, emmoCreateAlways|emmoWrite|emmoShareRead, length);
//       }
//       else
//       {
//           // suspend file watching
//           CFileWatchClient::StopWatching();
//           // open output mm file
//           outfile.Open(lpszPathName, emmoCreateAlways|emmoWrite|emmoShareRead, length);
//       }
// 
//       // copy text to output mm file
//       m_storage.GetText((LPTSTR)outfile.GetData(), length);
//       outfile.Close();
// 
//       if (m_mmfile.IsOpen())
//       {
//           storage_invalidated = true;
//           // suspend file watching
//           CFileWatchClient::StopWatching();
//           m_mmfile.Close();
//           // delete the original file we have to ignore failure for SaveAs operation
//           ::DeleteFile(lpszPathName);
//           // rename tmp file to the original
//           CHECK_FILE_OPERATION(!::_trename(tmpname, lpszPathName));
//       }
// 
//       // open a new copy and reload a file
//       loadFile(lpszPathName, true/*reload*/);
//         
//       // resume file watching
//       CFileWatchClient::StartWatching(lpszPathName);
//   }
//   catch (const std::exception& x)   
//   { 
//       if (storage_invalidated) m_storage.Clear();
// 
//       AfxMessageBox(CString(_T("Cannot save file.\n")) + x.what(), MB_OK|MB_ICONSTOP);
//       return FALSE;
//   }
//   catch (...) 
//   { 
//       if (storage_invalidated) m_storage.Clear();
//       AfxMessageBox(_T("Cannot save file. Unknown error.\n"), MB_OK|MB_ICONSTOP);
//       return FALSE;
//   }
// 
//   return TRUE;
// }

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
    // 06.18.2003 bug fix, SaveFile dialog occurs for SQL files which are loaded from db 
    //            if "save files when switching tasks" is activated.
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
      CString strExt = m_settings.GetExtensions().GetString();
			newName += '.' + strExt.Tokenize(_T(" ;,"), iStart);
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
    int length = (int)_tcslen(lpszPathName);

    if (lpszPathName && length)
    {
      CString backupPath;

      switch (backupMode)
      {
        case ebmCurrentDirectory:
          {
              CString path, name, ext;
              
              CString buff(lpszPathName);
              ::PathRemoveFileSpec(buff.LockBuffer()); buff.UnlockBuffer();
              path = buff;

              buff = ::PathFindExtension(lpszPathName);
              ext = (!buff.IsEmpty() && buff[0] == '.') ? (LPCTSTR)buff + 1 : buff;

              buff = ::PathFindFileName(lpszPathName);
              ::PathRemoveExtension(buff.LockBuffer()); buff.UnlockBuffer();
              name = buff;

              Common::Substitutor substr;
              substr.AddPair(_T("<NAME>"), name.GetString());
              substr.AddPair(_T("<EXT>"),  ext.GetString());
              substr << m_settings.GetFileBackupName().GetString();

              backupPath = path + '\\' + substr.GetResult();
          }
          break;
        case ebmBackupDirectory:
          {
            CString path, name;
            
            path = m_settings.GetFileBackupDirectory();

            if(path.Right(1) != '\\')
            {
              path += '\\';
            }
            name = ::PathFindFileName(lpszPathName);

            backupPath = path + name;
          }
          break;
      }

      if (m_lastBackupPath != backupPath                    // only for the first attempt
      && _tcsicmp(GetPathName(), backupPath.GetString()))   // avoiding copy the file into itself
      {
        _CHECK_AND_THROW_(CopyFile(lpszPathName, backupPath.GetString(), FALSE) != 0,_T("Cannot create backup file!"));
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
    // m_mmfile.Close();
    if (m_vmdata) VirtualFree(m_vmdata, 0, MEM_RELEASE);

    m_vmdata = vmdata;
    m_storage.SetText((LPCTSTR)m_vmdata, length, true/*use_buffer*/, false/*reload*/, false/*external*/);
}

void COEDocument::SetText (LPCTSTR text, unsigned long length)
{
    LPVOID vmdata = VirtualAlloc(NULL, length, MEM_COMMIT, PAGE_READWRITE);
    try
    {
      memcpy(vmdata, text, length * sizeof(TCHAR));
      SetText(vmdata, length);
    }
    catch (...)
    {
      if(vmdata != m_vmdata)
      {
        VirtualFree(vmdata,0,MEM_RELEASE);
      }
    }
}

void COEDocument::LoadSettingsManager()
{
  CString path;
  AppGetPath(path);

  if(m_settingsDir.IsEmpty())
  {
    CString dir;
    Common::FindApplicDirectory(dir);
    m_settingsDir = dir + _T("\\EDO\\OpenODBCQuerytool\\");
  }

  // Try to load from the users path
  CString settingsdir(m_settingsDir);
  CString languages = settingsdir + _T("languages.dat");
  CString templates = settingsdir + _T("templates.dat");
  CString settings  = settingsdir + _T("settings.dat");

  bool loaded(false);

  // Load language support
  if(_taccess(languages.GetString(),0) == 0)
  {
    try
    {
      LanguagesCollectionReader(FileInStream(languages.GetString())) >> LanguagesCollection();
      loaded = true;
    }
    catch(std::exception& /*ex*/)
    {
      // Incompatible settings from a previous version
      DeleteFile(templates.GetString());
      StyleMessageBox(NULL,CString(_T("Incompatible languages from a previous version removed from your roaming-profile\n")) + languages
                      ,_T("WARNING"),MB_OK | MB_ICONWARNING);
    }
  }
  if(!loaded)
  {
    LanguagesCollectionReader(FileInStream((path + _T("\\data\\languages.dat")).GetString())) >> LanguagesCollection();
  }
  // Load template settings
  loaded = false;
  if(_taccess(templates.GetString(),0) == 0)
  {
    try
    {
      TemplateCollectionReader(FileInStream(templates.GetString())) >> m_settingsManager;
      loaded = true;
    }
    catch(std::exception& /*ex*/)
    {
      // Incompatible settings from a previous version
      DeleteFile(templates.GetString());
      StyleMessageBox(NULL,CString(_T("Incompatible templates from a previous version removed from your roaming-profile\n")) + templates
                      ,_T("WARNING"),MB_OK | MB_ICONWARNING);
    }
  }
  if(!loaded)
  {
    TemplateCollectionReader(FileInStream((path + _T("\\data\\templates.dat")).GetString())) >> m_settingsManager;
  }
  // Load editor settings
  loaded = false;
  if(_taccess(settings.GetString(),0) == 0)
  {
    try
    {
      SettingsManagerReader(FileInStream(settings.GetString())) >> m_settingsManager;
      loaded = true;
    }
    catch(std::exception& /*ex*/)
    {
      // Incompatible settings from a previous version
      DeleteFile(settings.GetString());
      StyleMessageBox(NULL,CString(_T("Incompatible settings from a previous version removed from your roaming-profile\n")) + settings
                     ,_T("WARNING"),MB_OK|MB_ICONWARNING);
    }
  }
  if(!loaded)
  {
    SettingsManagerReader(FileInStream((path + _T("\\data\\settings.dat")).GetString())) >> m_settingsManager;
  }
}

static bool g_isBackupDone = false;

void COEDocument::SaveSettingsManager()
{
    CString path;
    AppGetPath(path);

    if(m_settingsDir.IsEmpty())
    {
      CString dir;
      Common::FindApplicDirectory(dir);
      m_settingsDir = dir + _T("\\EDO\\OpenODBCQuerytool\\");
    }

    CString settingsdir(m_settingsDir.GetBuffer());
    CString orgtemplates = path + _T("\\data\\templates.dat");
    CString orgsettings  = path + _T("\\data\\settings.dat");
    CString owntemplates = settingsdir + _T("templates.dat");
    CString ownsettings  = settingsdir + _T("settings.dat");
    CString baktemplates = owntemplates + _T(".bak");
    CString baksettings  = ownsettings  + _T(".bak");

    if(_taccess(owntemplates.GetString(),0) == 0 &&
       _taccess(ownsettings .GetString(),0) == 0)
    {
      // Own files already exist. Should we make a backup?
      if(!g_isBackupDone)
      {
        // old settings backup
        _CHECK_AND_THROW_(CopyFile(owntemplates.GetString(),baktemplates.GetString(), FALSE) != 0 && 
                          CopyFile(ownsettings .GetString(),baksettings .GetString(), FALSE) != 0
                         ,_T("Settings backup file creation error!"));
        g_isBackupDone = true;
      }
    }
    else
    {
      // Our files do not exist. Create them
      CopyFile(orgtemplates.GetString(),owntemplates.GetString(),FALSE);
      CopyFile(orgsettings .GetString(),ownsettings .GetString(),FALSE);
    }
    // save new settings
    TemplateCollectionWriter(FileOutStream(owntemplates.GetString())) << m_settingsManager;
    SettingsManagerWriter   (FileOutStream(ownsettings .GetString())) << m_settingsManager;
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
            prompt += _T("\n\nThis file has been modified outside of the editor.")
                         _T("\nDo you want to reload it");
            prompt += !IsModified() ? _T("?") : _T(" and lose the changes made in the editor?");

            if ((m_settings.GetFileReloadChanges() && !IsModified())
            || ::MessageBox(theApp.m_pMainWnd->GetSafeHwnd(),prompt,_T("Changed"),
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
                Common::SetStatusText(CString(_T("Reloading file \"")) + GetPathName() + _T("\" ..."));
                CFileWatchClient::UpdateWatchInfo();
                loadFile(GetPathName(), true/*reload*/, true/*external*/);
                Common::SetStatusText(CString(_T("File \"")) + GetPathName() + _T("\" reloaded."));

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
            CString prompt = _T("Do you want to reload \n\n") + GetPathName() 
                           + _T("\n\nand lose the changes made in the editor?");

            if (!IsModified() || AfxMessageBox(prompt, MB_YESNO|MB_ICONEXCLAMATION) == IDYES)
            {
                
                Common::SetStatusText(CString(_T("Reloading file \"")) + GetPathName() + _T("\" ..."));
                loadFile(GetPathName(), true/*reload*/, true/*external*/);
                Common::SetStatusText(CString(_T("File \"")) + GetPathName() + _T("\" reloaded."));
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
    TCHAR buff[80];
    _stprintf_s(buff,80,_T(" Lines: %d "), m_storage.GetLineCount());
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
    CDocManagerExt::SetFileOpenPath(_T(""));
}
