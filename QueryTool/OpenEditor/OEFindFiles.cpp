/* 
    Copyright (C) 2004 - 2017 Edwig Huisman

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
#include <COMMON/ExceptionHelper.h>
#include <Common/DirSelectDlg.h>
#include "OEFindFiles.h"
#include <Common/regexp.h>

const bool Controls2Data = true;   // UpdateData
const bool Data2Controls = false;


COEFindFiles::COEFindFiles (SettingsManager& manager,CWnd* p_parent)
	           :StyleDialog(COEFindFiles::IDD,p_parent)
             ,m_manager(manager)
{
  const OpenEditor::GlobalSettings& settings = m_manager.GetGlobalSettings();
  m_whole_word = settings.GetFileWholeWord() ? true : false;
  m_match_case = settings.GetFileMatchCase() ? true : false;
  m_use_regex  = settings.GetFileUseRegEx()  ? true : false;
  m_subfolders = settings.GetFileSubfolders()? true : false;
  m_savebefore = settings.GetFileSaveBefore()? true : false;
  m_collapse   = settings.GetFileCollapse()  ? true : false;
  m_whattofind = settings.GetFileWhatToFind().c_str();
  m_filetypes  = settings.GetFileFileTypes().c_str();
  m_directories= settings.GetFileStartDirectory().c_str();
}

COEFindFiles::~COEFindFiles()
{
}

void COEFindFiles::DoDataExchange(CDataExchange* pDX)
{
  StyleDialog::DoDataExchange(pDX);

  DDX_Control(pDX,IDC_FIND_FINDWHAT, m_editWhatToFind,  m_whattofind);
  DDX_Control(pDX,IDC_FILE_TYPES,    m_editFileTypes,   m_filetypes);
  DDX_Control(pDX,IDC_FILE_DIRECTORY,m_editDirectories, m_directories);

  DDX_Control(pDX,IDC_WHOLE_WORD,         m_checkWholeWord);
  DDX_Control(pDX,IDC_MATCH_CASE,         m_checkMatchCase);
  DDX_Control(pDX,IDC_REGULAR_EXPRESSIONS,m_checkUseRegex);
  DDX_Control(pDX,IDC_SUBFOLDERS,         m_checkSubfolders);
  DDX_Control(pDX,IDC_SAVEBEFORE,         m_checkSaveBefore);
  DDX_Control(pDX,IDC_COLLAPSELIST,       m_checkCollapse);

  DDX_Control(pDX, IDC_BROWSEDIR, m_buttonBrowse);
  DDX_Control(pDX, IDOK,          m_buttonBrowse);
  DDX_Control(pDX, IDCANCEL,      m_buttonCancel);

  DDX_Check(pDX, IDC_WHOLE_WORD,          m_whole_word); 
  DDX_Check(pDX, IDC_MATCH_CASE,          m_match_case);
  DDX_Check(pDX, IDC_REGULAR_EXPRESSIONS, m_use_regex);
  DDX_Check(pDX, IDC_SUBFOLDERS,          m_subfolders);
  DDX_Check(pDX, IDC_SAVEBEFORE,          m_savebefore);
  DDX_Check(pDX, IDC_COLLAPSELIST,        m_collapse);

}

BEGIN_MESSAGE_MAP(COEFindFiles, StyleDialog)
  ON_BN_CLICKED(IDC_BROWSEDIR,OnBrowseDirectories)
END_MESSAGE_MAP()

BOOL
COEFindFiles::OnInitDialog()
{
  StyleDialog::OnInitDialog();
  SetWindowText("Find in Files");

  SetCanResize();
  return TRUE;
}

void
COEFindFiles::SetupDynamicLayout()
{
  StyleDialog::SetupDynamicLayout();

  CMFCDynamicLayout& manager = *GetDynamicLayout();
#ifdef _DEBUG
  manager.AssertValid();
#endif

  manager.AddItem(IDC_FIND_FINDWHAT, manager.MoveNone(), manager.SizeHorizontal(100));
  manager.AddItem(IDC_FILE_TYPES,    manager.MoveNone(), manager.SizeHorizontal(100));
  manager.AddItem(IDC_FILE_DIRECTORY,manager.MoveNone(), manager.SizeHorizontal(100));
  manager.AddItem(IDC_BROWSEDIR,     manager.MoveHorizontal(100), manager.SizeNone());
  manager.AddItem(IDOK,              manager.MoveHorizontal(100), manager.SizeNone());
  manager.AddItem(IDCANCEL,          manager.MoveHorizontal(100), manager.SizeNone());
}

void COEFindFiles::OnUpdateData()
{
  UpdateData();
}

void COEFindFiles::OnOK()
{
  OnApply();
  StyleDialog::OnOK();
}

BOOL COEFindFiles::OnApply()
{
  UpdateData(Controls2Data);
  try
  {
      OpenEditor::GlobalSettings& settings = m_manager.GetGlobalSettings();
      settings.SetFileWholeWord (m_whole_word ? true : false, false /*notify*/);
      settings.SetFileMatchCase (m_match_case ? true : false, false /*notify*/);
      settings.SetFileUseRegEx  (m_use_regex  ? true : false, false /*notify*/);
      settings.SetFileSubfolders(m_subfolders ? true : false, false /*notify*/);
      settings.SetFileSaveBefore(m_savebefore ? true : false, false /*notify*/);
      settings.SetFileCollapse  (m_collapse   ? true : false, false /*notify*/);
      std::string str;
      str = m_whattofind;  settings.SetFileWhatToFind(str,false);
      str = m_filetypes;   settings.SetFileFileTypes (str, false);
      str = m_directories; settings.SetFileStartDirectory(str,false);
      settings.NotifySettingsChanged();
  }
  _OE_DEFAULT_HANDLER_;

	return TRUE;
}

void
COEFindFiles::OnBrowseDirectories()
{
  UpdateData();
  Common::CDirSelectDlg dir("Directory to start searching",this,m_directories);
  if(dir.DoModal() == IDOK)
  {
    m_directories = dir.GetPath();
    UpdateData(Data2Controls);
  }
}

bool
COEFindFiles::GoFind(StyleGridCtrl* grid)
{
  if(m_savebefore)
  {
    if(! AfxGetApp()->SaveAllModified())
    {
      // Saving not successful. Stop the find action
      return false;
    }
  }
  m_directories.TrimRight('\\');
  int num = GoFindInFolder(m_directories,grid);

  CString all = "All files";
  int found = grid->GetRowCount() - 1;
  AppendRow(grid,all,num,  "Files searched");
  AppendRow(grid,all,found,"Matches found");
  grid->AutoSize();
  grid->ExpandLastColumn();
  return (num != 0);
}

int
COEFindFiles::GoFindInFolder(CString p_folder,StyleGridCtrl* grid)
{
  struct _finddata_t fileinfo;
  intptr_t fileHandle;
  CString patroon;
  int    aantal = 0;

  // Step 1: Find the files by pattern types
  patroon = p_folder + "\\" + m_filetypes;
  fileHandle = _findfirst(patroon,&fileinfo);
  if(fileHandle != -1)
  {
    // OK, found something
    do
    {
      if(!(fileinfo.attrib & _A_SUBDIR))
      {
        // Search in this file
        ++aantal;
        GoFindInFile(p_folder + "\\" + fileinfo.name,grid);
      }
    }
    while(_findnext(fileHandle,&fileinfo) == 0);
    _findclose(fileHandle);
  }
  // Step 2: Find the directories
  if(m_subfolders)
  {
    patroon = p_folder + "\\*.*";
    fileHandle = _findfirst(patroon,&fileinfo);
    if(fileHandle != -1)
    {
      do
      {
        if(fileinfo.attrib & _A_SUBDIR)
        {
          if(fileinfo.name[0] != '.')
          {
            aantal += GoFindInFolder(p_folder + "\\" + fileinfo.name,grid);
          }
        }
      }
      while(_findnext(fileHandle,&fileinfo) == 0);
      _findclose(fileHandle);
    }
  }
  return aantal;
}

void
COEFindFiles::GoFindInFile(CString fileName,StyleGridCtrl* grid)
{
  // m_whole_word, m_match_case, m_use_regex, m_collapse
  FILE*   file  = nullptr;
  bool    res   = false;
  int     len   = 0;
  CString findThis   = m_whattofind;
  long    lineNumber = 0;
  RegExp  rex;
  char    buffer[MAX_BUFSIZE+1];

  fopen_s(&file,fileName, "rt");

  if(!m_match_case)
  {
    findThis.MakeUpper();
  }
  if(m_use_regex)
  {
    if(!rex.SetExp(m_whattofind))
    {
      CString message = "Error in regular expression: " + m_whattofind;
      AfxMessageBox(message,MB_OK);
      return;
    }
    rex.SetUseCase(m_match_case);
  }
  if(file)
  {
    while(fgets(buffer,MAX_BUFSIZE,file) != NULL)
    {
      ++lineNumber;
      do
      {
        len = (int)strlen(buffer);
        if(buffer[len-1] == '\n') { buffer[len-1] = 0; continue; }
        if(buffer[len-1] == '\r') { buffer[len-1] = 0; continue; }
        break;
      }
      while(len > 0);
      res = false;
      if(!m_use_regex)
      {
        if(!m_match_case)
        {
          char* pnt = buffer;
          while(*pnt)
          {
            *pnt = toupper(*pnt);
            ++pnt;
          }
        }
        res = (strstr(buffer,findThis) != NULL);
      }
      else
      {
        res = rex.Match(buffer);
      }
      if(m_collapse && res)
      {
        // Collapsed list and result, so perform early dropout
        AppendRow(grid,fileName,lineNumber,buffer);
        fclose(file);
        return;
      }
      if(res)
      {
        AppendRow(grid,fileName,lineNumber,buffer);
      }
    }
    fclose(file);
  }
}

void
COEFindFiles::AppendRow(StyleGridCtrl* grid,CString& fileName,int lineNumber,char* buffer)
{
  char buf[20];
  _itoa_s(lineNumber,buf,20,10);

  GV_ITEM item;
  item.mask = GVIF_TEXT | GVIF_FORMAT;
	item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX;

  int row = grid->InsertRow(fileName);

  // Linenumber
  item.row = row;
  item.col = 1;
  item.strText = (CString) buf;
  grid->SetItem(&item);

  // Text
  item.col = 2;
  item.strText = buffer;
  grid->SetItem(&item);
}