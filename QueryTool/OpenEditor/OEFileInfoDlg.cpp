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
#include "resource.h"
#include "OEFileInfoDlg.h"
#include "OEDocument.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static void filetime_to_string (FILETIME& filetime, CString& str)
{
  SYSTEMTIME systemTime;
  const int buffLen = 40;
  char* buff = str.GetBuffer(buffLen);

  FileTimeToLocalFileTime(&filetime, &filetime);
  FileTimeToSystemTime(&filetime, &systemTime);

  int dateLen = GetDateFormat(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &systemTime, NULL, buff, buffLen);
  buff[dateLen - 1] = ' ';
  int timeLen = GetTimeFormat(LOCALE_USER_DEFAULT, TIME_NOSECONDS, &systemTime, NULL, buff + dateLen, buffLen - dateLen);

  str.ReleaseBuffer(dateLen + timeLen);
}

/////////////////////////////////////////////////////////////////////////////
// COEFileInfoDlg dialog


COEFileInfoDlg::COEFileInfoDlg(COEDocument* p_document,CWnd* p_parent)
	             :m_document(p_document)
               ,StyleDialog(COEFileInfoDlg::IDD,p_parent)
{
}


void COEFileInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	StyleDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_OEP_CREATED,					m_Created);
	DDX_Text(pDX, IDC_OEP_LAST_MODIFIED,		m_LastModified);
	DDX_Text(pDX, IDC_OEP_DRIVE_USAGE,			m_DriveUsage);
	DDX_Text(pDX, IDC_OEP_MEMORY_USAGE,			m_MemoryUsage);
	DDX_Text(pDX, IDC_OEP_MEMORY_ALLOCATED, m_MemoryAllocated);
	DDX_Text(pDX, IDC_OEP_LINES,						m_Lines);
	DDX_Text(pDX, IDC_OEP_UNDO_MEMORY,			m_UndoMemoryUsage);
	DDX_Text(pDX, IDC_OEP_PATH,							m_Path);
}

/*
BEGIN_MESSAGE_MAP(COEFileInfoDlg, StyleDialog)
	//{{AFX_MSG_MAP(COEFileInfoDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
*/

BOOL
COEFileInfoDlg::OnInitDialog()
{
	StyleDialog::OnInitDialog();

  unsigned usage, allocated, undo;
  OpenEditor::Storage& storage = m_document->GetStorage();
  storage.GetMemoryUsage(usage, allocated, undo);

  m_Lines.Format("%d lines", storage.GetLineCount());
  m_MemoryUsage.Format("%d bytes", usage);
  m_MemoryAllocated.Format("%d bytes", allocated);
  m_UndoMemoryUsage.Format("%d bytes", undo);
  m_Path = m_document->GetPathName();

  if (!m_Path.IsEmpty())
  {
    HANDLE hFile = CreateFile(m_Path,                 // file name
                              0,                      // access mode
                              FILE_SHARE_READ,        // share mode
                              NULL,                   // SD
                              OPEN_EXISTING,          // how to create
                              FILE_ATTRIBUTE_NORMAL,  // file attributes
                              NULL                    // handle to template file
                              );

    if (hFile != INVALID_HANDLE_VALUE)
    {
      BY_HANDLE_FILE_INFORMATION fileInformation;

      if (GetFileInformationByHandle(hFile, &fileInformation))
      {
        if (!fileInformation.nFileSizeHigh)
        {
          m_DriveUsage.Format("%u bytes", fileInformation.nFileSizeLow);
        }
        filetime_to_string(fileInformation.ftCreationTime, m_Created);
        filetime_to_string(fileInformation.ftLastWriteTime,m_LastModified);
      }

      CloseHandle(hFile);
    }
  }
  UpdateData(FALSE);

  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// COEFileInfoDlg message handlers
