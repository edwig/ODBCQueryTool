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

/////////////////////////////////////////////////////////////////////////////
//
// USAGE :
//---------------------------------------------------------------------------
// void CSomeClass::SomeFnThatThrows() throw()
// {
//		If(!Win32Fn)
//			throw &CWinException();
// }
//
// void CSomeClass::SomeFnThatUsesThrowingFn() 
// {
//		try
//		{
//			SomeFnThatThrows();
//		}
//		catch(CWinException* e)
//		{
//			e->ReportError();
//		}
// }
//
//---------------------------------------------------------------------------
// or just :
//---------------------------------------------------------------------------
// 
// CWinException e;
// e.ReportError();
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __WINEXCEPTION_H__
#define __WINEXCEPTION_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWinException : public CException   
{
public:
	CWinException();
	CWinException(DWORD dwErrCode);
	virtual ~CWinException();
	DECLARE_DYNAMIC(CWinException)

public:
	operator LPCTSTR() const	{return m_szLastError;}
	operator DWORD() const 		{return m_dwLastError;}
	/*
	operator ==(DWORD dwErrCode){return (m_dwLastError == dwErrCode);}
	operator !=(DWORD dwErrCode){return (m_dwLastError != dwErrCode);}
	operator > (DWORD dwErrCode){return (m_dwLastError >  dwErrCode);}
	operator < (DWORD dwErrCode){return (m_dwLastError <  dwErrCode);}
	operator >=(DWORD dwErrCode){return (m_dwLastError >= dwErrCode);}
	operator <=(DWORD dwErrCode){return (m_dwLastError <= dwErrCode);}
	*/

public:
	CString m_szLastError;
	DWORD	m_dwLastError;

public:
	BOOL			SetLastError(DWORD dwErrorCode);	
	DWORD			GetLastError(void);
	static BOOL		ToString(DWORD dwErrorCode, CString& szError);
	
	// Inlines
	inline BOOL		GetLastErrorString(CString& szLastError)
					{return ToString(m_dwLastError, szLastError);}
	inline void		ClearLastError(void){SetLastError(ERROR_SUCCESS);}
	
public:
	
	// Virtuals
	virtual BOOL	GetErrorMessage( LPTSTR lpszError, UINT nMaxError, 
					PUINT pnHelpContext = NULL );
	virtual int		ReportError( UINT nType = MB_OK, UINT nMessageID = 0 );	
};

//////////////////////////////////////////////////////////////////////
#endif //__WINEXCEPTION_H__
