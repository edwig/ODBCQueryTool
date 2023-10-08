/////////////////////////////////////////////////////////////////////////////
//
//	Copyright(c) Armen Hakobyan 2001
//	mailto:armenh@cit.am
//	http://www.cit.am
//
//	CWinException class is a wrapper for Win32 API GetLasError.
// 
//---------------------------------------------------------------------------
//	VERSION HISTORY
//---------------------------------------------------------------------------
//	1.0		Nov 12 2001		- First release of CLastError class
//	1.1		Nov 14 2001		- Operators and Clear method added
//	1.2		Nov 25 2001		- Totally rewritten and derived from CException 
//							  MFC class to throw :) and became CWinException
//---------------------------------------------------------------------------
//	NOTES
//---------------------------------------------------------------------------
//	This software is provided "AS IS" without express or implied warranty. 
//	Use it at your own risk! The author accepts no liability for any 
//	damage/loss of business that this product may cause.
//---------------------------------------------------------------------------
//	Feel free to use this class anyway you want but with one condition - 
//	please report me all bugs or fixes and mail me all improvements.
/////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "WinException.h"
#include <lmerr.h>

#ifdef _DEBUG
	#undef THIS_FILE
	static char THIS_FILE[]=__FILE__;
	#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
//
IMPLEMENT_DYNAMIC(CWinException, CException) 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction

CWinException::CWinException()
{
	GetLastError();
}

CWinException::CWinException(DWORD dwErrCode)
{
	SetLastError(dwErrCode);
}

CWinException::~CWinException()
{
}

//////////////////////////////////////////////////////////////////////
// Get Last Error String

BOOL CWinException::GetErrorMessage(LPTSTR lpszError,UINT nMaxError,PUINT pnHelpContext /*= NULL*/)
{
	pnHelpContext = NULL;
  if(nMaxError)
  {
    return lstrcpyn(lpszError,m_szLastError.GetString(),nMaxError) != nullptr;
  }
  else
  {
    return lstrcpy(lpszError,m_szLastError.GetString()) != nullptr;
  }
}

//////////////////////////////////////////////////////////////////////
// Report Last Errror

int CWinException::ReportError(UINT nType /*= MB_OK*/, 
							   UINT nMessageID /*= 0*/)
{
	CString szMessage;
	szMessage.Format(_T("System error \'%lu\':\n"), m_dwLastError);
	
	if(m_szLastError.IsEmpty())
	{
		if(nMessageID != 0)
		{		
			CString szRes;
			szRes.LoadString(nMessageID);
			if(!szRes.IsEmpty())
				szMessage += _T("\n") + szRes;
		}
	}
	else
		szMessage += _T("\n") + m_szLastError;
	
	if(nType == MB_OK)
		nType |= MB_APPLMODAL | MB_ICONSTOP;
	
	return AfxMessageBox(szMessage, nType);	
}

//////////////////////////////////////////////////////////////////////
// Set/Get Last Error Codes

DWORD CWinException::GetLastError(void) 
{ 
	m_dwLastError = ::GetLastError();	
	ToString(m_dwLastError, m_szLastError);
	return m_dwLastError;
}

BOOL CWinException::SetLastError(DWORD dwErrCode) 
{
	::SetLastError(dwErrCode);
	m_dwLastError = dwErrCode;
	return ToString(m_dwLastError, m_szLastError);
}

//////////////////////////////////////////////////////////////////////
// Convert error code to string

BOOL CWinException::ToString(DWORD dwErrCode, CString& szError) /*static*/
{	

	HMODULE 	hModule 	= NULL; 
	LPTSTR		lpszMessage	= NULL;
	DWORD		cbBuffer	= NULL;
	BOOL		bRet		= FALSE;

	szError.Empty();

	if ((dwErrCode >= NERR_BASE) && (dwErrCode <= MAX_NERR /*NERR_BASE+899*/))
	{
		hModule = ::LoadLibraryEx( _T("netmsg.dll"), NULL, LOAD_LIBRARY_AS_DATAFILE);
	}
	
	cbBuffer = ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
							   FORMAT_MESSAGE_IGNORE_INSERTS | 
							   FORMAT_MESSAGE_FROM_SYSTEM | // always consider system table
							   ((hModule != NULL) ? FORMAT_MESSAGE_FROM_HMODULE : 0), 
							   hModule, // module to get message from (NULL == system)
							   dwErrCode,
							   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // default language
							   (LPTSTR) &lpszMessage, 0, NULL );
	
	//ERROR_RESOURCE_LANG_NOT_FOUND

    if (cbBuffer)
	{
		szError = (LPCTSTR)lpszMessage;
		bRet = TRUE;
	}

	::LocalFree(lpszMessage);
	if (hModule) ::FreeLibrary(hModule);

    if (szError.IsEmpty())
		szError.Format(_T("Unknown system error - %lu."), dwErrCode);

	return bRet;
}