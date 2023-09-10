//////////////////////////////////////////////////////////////////////
// FileWatch.h: interface for the CFileWatch class.
// These classes are based on Herbert Griebel code (herbertgriebel@yahoo.com)
// His code has been changed but the idea hasn't
//////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __FILEWATCH_H__
#define __FILEWATCH_H__

#include <afxmt.h>
#include <map>
#include <set>
#include <string>

using std::map;
using std::set;

/////////////////////////////////////////////////////////////////////////////
// CFileWatch
//      assumption: clients are been created only in main thread

class CFileWatchClient
{
public:
  CFileWatchClient ();
  virtual ~CFileWatchClient ();

protected:
	void StartWatching (LPCSTR = NULL);
	void StopWatching ();
	void UpdateWatchInfo ();

private:
  friend class CFileWatch;
  virtual void OnWatchedFileChanged () = 0;
    
  CString	m_fileName;
	__int64	m_lastWriteTime, m_nFileSize;
  bool m_modified;
  bool m_suspendWatch;
};

class CFileWatch
{
public:
  static void NotifyClients ();
  static void SuspendThread ();
  static void ResumeThread  ();
	static UINT m_msgFileWatchNotify;

private:
  friend CFileWatchClient;
	static void StartThread ();
	static UINT Watch (LPVOID);
public:
	static void Stop ();

private:
	static void AddFileToWatch (CFileWatchClient&);
	static void RemoveFileToWatch (CFileWatchClient&);

  static set<CFileWatchClient*> m_clients;
  static map<CString, HANDLE> m_folders;

	static CCriticalSection	m_csDataLock;
	static CEvent			m_EventUpdate;
	static CEvent			m_EventCheck;
	static bool				m_bWatchClosed;
	static bool				m_bStopWatch;
	static CWinThread		*m_pThread;
};


#endif//__FILEWATCH_H__
