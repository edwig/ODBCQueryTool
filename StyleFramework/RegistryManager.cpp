//////////////////////////////////////////////////////////////////////////
//
// File: RegistryManager.h
// Function: Handling the registry
//
//   _____ _         _ _             ______                                           _    
//  / ____| |       | (_)           |  ____|                                         | |   
// | (___ | |_ _   _| |_ _ __   __ _| |__ _ __ __ _ _ __ ___   _____      _____  _ __| | __
//  \___ \| __| | | | | | '_ \ / _` |  __| '__/ _` | '_ ` _ \ / _ \ \ /\ / / _ \| '__| |/ /
//  ____) | |_| |_| | | | | | | (_| | |  | | | (_| | | | | | |  __/\ V  V / (_) | |  |   < 
// |_____/ \__|\__, |_|_|_| |_|\__, |_|  |_|  \__,_|_| |_| |_|\___| \_/\_/ \___/|_|  |_|\_\
//              __/ |           __/ |                                                      
//             |___/           |___/                                                       
//
//
// Author: ir. W.E. Huisman
// For license: See the file "LICENSE.txt" in the root folder
//
#include "stdafx.h"
#include "RegistryManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RegistryManager

CString RegistryManager::m_iniFileName = "";

RegistryManager::RegistryManager(HKEY hKeyBase/*= HKEY_CURRENT_USER*/)
                :m_keyBase(0)
                ,m_lResult(0)
{
  if(hKeyBase == HKEY_CURRENT_USER || hKeyBase == HKEY_LOCAL_MACHINE)
  {
    m_keyBase = hKeyBase;
    m_lResult = ERROR_SUCCESS;
  }
}

RegistryManager::~RegistryManager()
{
}

void 
RegistryManager::SetRegistryKey(LPCTSTR lpszRegistryKey, LPCTSTR lpszProfileName)
{
	m_registryKey = lpszRegistryKey;
	m_profileName = lpszProfileName;
}

BOOL 
RegistryManager::GetProfileInfo()
{
	if (m_registryKey.IsEmpty() || m_profileName.IsEmpty())
	{
		CWinApp* pWinApp = AfxGetApp();
		if (pWinApp != nullptr)
		{
			m_registryKey = pWinApp->m_pszRegistryKey;
			m_profileName = pWinApp->m_pszProfileName;
		}

		// If this fails, you need to call SetRegistryKey(...); in your
		// applications CWinApp::InitInstance() override, or you have called
		// this method before you call SetRegistryKey..  Calling SetRegistryKey
		// will initialize m_pszProfileName and m_pszRegistryKey for CWinApp.

		// NOTE: If you want to write to an INI file instead of the system
		// registry, call RegistryManager::SetINIFileName(...) prior to
		// making this call.

		if (m_registryKey.IsEmpty() || m_profileName.IsEmpty())
		{
			return FALSE;
		}
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

HKEY 
RegistryManager::GetAppRegistryKey(REGSAM samDesired)
{
  if (!GetProfileInfo())
  {
    return 0;
  }

	HKEY hAppKey = nullptr;
	AutoRegistryKey hSoftKey;
	AutoRegistryKey hCompanyKey;

	m_lResult = ::RegOpenKeyEx(m_keyBase,_T("Software"),0,samDesired,hSoftKey);

  if (m_lResult != ERROR_SUCCESS)
  {
    return nullptr;
  }

	DWORD dw = 0;
	m_lResult = ::RegCreateKeyEx(hSoftKey
                              ,m_registryKey
                              ,0
                              ,REG_NONE
                              ,REG_OPTION_NON_VOLATILE
                              ,samDesired
                              ,nullptr
                              ,hCompanyKey
                              ,&dw);

  if (m_lResult != ERROR_SUCCESS)
  {
    return nullptr;
  }
	m_lResult = ::RegCreateKeyEx(hCompanyKey
                              ,m_profileName
                              ,0
                              ,REG_NONE
                              ,REG_OPTION_NON_VOLATILE
                              ,samDesired
                              ,nullptr
                              ,&hAppKey
                              ,&dw);

  if(m_lResult != ERROR_SUCCESS)
  {
    return nullptr;
  }
	return hAppKey;
}

HKEY 
RegistryManager::GetFullPathKey(LPCTSTR lpszSection) const
{
  if (_tcsncmp(lpszSection, _T("HKCU"), 4) == 0)
  {
    return HKEY_CURRENT_USER;
  }
  if (_tcsncmp(lpszSection, _T("HKLM"), 4) == 0)
  {
    return HKEY_LOCAL_MACHINE;
  }
	return 0;
}

HKEY 
RegistryManager::RecurseOpenKey(LPCTSTR lpszSection, REGSAM samDesired)
{
	HKEY hKeyBase = GetFullPathKey(lpszSection);
  if (hKeyBase == 0)
  {
    return 0;
  }
	CString strRegistryKey = lpszSection + 5;

	AutoRegistryKey hParentKey = nullptr;
	DWORD dw = 0;

	int nIndex = strRegistryKey.Find('\\');
	while (nIndex != -1)
	{
		CString strKey = strRegistryKey.Left(nIndex);
		strRegistryKey = strRegistryKey.Mid(nIndex + 1);
		HKEY hKey = 0;

		m_lResult = ::RegCreateKeyEx(hParentKey == nullptr ? hKeyBase : (HKEY)hParentKey
                                ,strKey
                                ,0
                                ,REG_NONE
                                ,REG_OPTION_NON_VOLATILE
                                ,samDesired
                                ,nullptr
                                ,&hKey
                                ,&dw);

    if (m_lResult != ERROR_SUCCESS)
    {
      return nullptr;
    }
		hParentKey = hKey;
		nIndex = strRegistryKey.Find('\\');
	}

	HKEY hAppKey = nullptr;
	m_lResult = ::RegCreateKeyEx(hParentKey
                              ,strRegistryKey
                              ,0
                              ,REG_NONE
                              ,REG_OPTION_NON_VOLATILE
                              ,samDesired
                              ,nullptr
                              ,&hAppKey
                              ,&dw);

  if (m_lResult != ERROR_SUCCESS)
  {
    return nullptr;
  }
	return hAppKey;
}

HKEY 
RegistryManager::GetSectionKey(LPCTSTR lpszSection, REGSAM samDesired)
{
	if(GetFullPathKey(lpszSection))
	{
		return RecurseOpenKey(lpszSection, samDesired);
	}

  if (!GetProfileInfo())
  {
    return 0;
  }
	ASSERT(lpszSection != nullptr);
  if (!lpszSection)
  {
    return 0;
  }
	HKEY hSectionKey = nullptr;
	AutoRegistryKey hAppKey(GetAppRegistryKey(samDesired));

  if (hAppKey == nullptr)
  {
    return nullptr;
  }
	DWORD dw = 0;
	m_lResult = ::RegCreateKeyEx(hAppKey
                              ,lpszSection
                              ,0
                              ,REG_NONE
                              ,REG_OPTION_NON_VOLATILE
                              ,samDesired
                              ,nullptr
                              ,&hSectionKey
                              ,&dw);

  if (m_lResult != ERROR_SUCCESS)
  {
    return nullptr;
  }
	return hSectionKey;
}

BOOL 
RegistryManager::SetRegistryInteger(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue)
{
  if(lpszSection == nullptr || lpszEntry == nullptr)
  {
    return FALSE;
  }

	if(m_iniFileName.IsEmpty())
	{
		AutoRegistryKey hSecKey(GetSectionKey(lpszSection));
    if (hSecKey == nullptr)
    {
      return FALSE;
    }
		m_lResult = ::RegSetValueEx(hSecKey
                               ,lpszEntry
                               ,NULL
                               ,REG_DWORD
                               ,(LPBYTE)&nValue
                               ,sizeof(nValue));

    if (m_lResult != ERROR_SUCCESS)
    {
      return FALSE;
    }
		return TRUE;
	}
	else
	{
		TCHAR szT[16];
		wsprintf(szT, _T("%d"), nValue);
		return ::WritePrivateProfileString(lpszSection,lpszEntry,szT,m_iniFileName);
	}
}

BOOL 
RegistryManager::SetRegistryBinary(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPBYTE pData, UINT nBytes)
{
  if(lpszSection == nullptr || lpszEntry == nullptr)
  {
    return FALSE;
  }

	if (m_iniFileName.IsEmpty())
	{
		AutoRegistryKey hSecKey(GetSectionKey(lpszSection));
    if (hSecKey == nullptr)
    {
      return FALSE;
    }
		m_lResult = ::RegSetValueEx(hSecKey
                               ,lpszEntry
                               ,NULL
                               ,REG_BINARY
                               ,pData
                               ,nBytes);

    if (m_lResult != ERROR_SUCCESS)
    {
      return FALSE;
    }
		return TRUE;
	}

	// convert to string and write out
	LPTSTR lpsz = new TCHAR[nBytes * 2 + 1];
	UINT i;
	for (i = 0; i < nBytes; i++)
	{
		lpsz[i * 2    ] = (TCHAR)( (pData[i]       & 0x0F) + 'A'); // low nibble
		lpsz[i * 2 + 1] = (TCHAR)(((pData[i] >> 4) & 0x0F) + 'A'); // high nibble
	}
	lpsz[i * 2] = 0;

	BOOL bResult = SetRegistryString(lpszSection, lpszEntry, lpsz);
	delete[] lpsz;
	return bResult;
}

BOOL 
RegistryManager::SetRegistryString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue)
{
  if(lpszSection == nullptr || lpszEntry == nullptr)
  {
    return FALSE;
  }

	if (m_iniFileName.IsEmpty())
	{
		if (lpszEntry == nullptr) //delete whole section
		{
			AutoRegistryKey hAppKey(GetAppRegistryKey(FALSE));
      if (hAppKey == nullptr)
      {
        return FALSE;
      }
			m_lResult = ::RegDeleteKey(hAppKey, lpszSection);

      if (m_lResult != ERROR_SUCCESS)
      {
        return FALSE;
      }
		}
		else if (lpszValue == nullptr)
		{
			AutoRegistryKey hSecKey(GetSectionKey(lpszSection));
      if (hSecKey == nullptr)
      {
        return FALSE;
      }
			// necessary to cast away const below
			m_lResult = ::RegDeleteValue(hSecKey, (LPTSTR)lpszEntry);

      if (m_lResult != ERROR_SUCCESS)
      {
        return FALSE;
      }
		}
		else
		{
			AutoRegistryKey hSecKey(GetSectionKey(lpszSection));
      if (hSecKey == nullptr)
      {
        return FALSE;
      }
			m_lResult = ::RegSetValueEx(hSecKey
                                 ,lpszEntry
                                 ,NULL
                                 ,REG_SZ
                                 ,(LPBYTE)lpszValue
                                 ,(lstrlen(lpszValue)+1)*sizeof(TCHAR));

      if (m_lResult != ERROR_SUCCESS)
      {
        return FALSE;
      }
		}
		return TRUE;
	}
	else
	{
		return ::WritePrivateProfileString(lpszSection,lpszEntry,lpszValue,m_iniFileName);
	}
}

UINT 
RegistryManager::GetRegistryInteger(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault)
{
  if(lpszSection == nullptr || lpszEntry == nullptr)
  {
    return FALSE;
  }
	if(m_iniFileName.IsEmpty()) // use registry
	{
		AutoRegistryKey hSecKey(GetSectionKey(lpszSection, KEY_READ));
    if(hSecKey == nullptr)
    {
      return nDefault;
    }
		DWORD dwValue;
		DWORD dwType;
		DWORD dwCount = sizeof(DWORD);

		m_lResult = ::RegQueryValueEx(hSecKey
                                 ,(LPTSTR)lpszEntry
                                 ,nullptr
                                 ,&dwType
                                 ,(LPBYTE)&dwValue
                                 ,&dwCount);

    if(m_lResult != ERROR_SUCCESS)
    {
      return nDefault;
    }
		ASSERT(dwType  == REG_DWORD);
		ASSERT(dwCount == sizeof(dwValue));

		return (UINT)dwValue;
	}
	else
	{
		return ::GetPrivateProfileInt(lpszSection,lpszEntry,nDefault,m_iniFileName);
	}
}

BOOL 
RegistryManager::GetRegistryBinary(LPCTSTR lpszSection, LPCTSTR lpszEntry, BYTE** ppData, UINT* pBytes)
{
  if(lpszSection == nullptr || lpszEntry == nullptr)
  {
    return FALSE;
  }

  if(ppData == nullptr || pBytes == nullptr)
  {
    return FALSE;
  }

  *ppData = nullptr;
	*pBytes = 0;

	if (m_iniFileName.IsEmpty())
	{
		AutoRegistryKey hSecKey(GetSectionKey(lpszSection, KEY_READ));
    if (hSecKey == nullptr)
    {
      return FALSE;
    }
		DWORD dwType, dwCount;
		m_lResult = ::RegQueryValueEx(hSecKey
                                 ,(LPTSTR)lpszEntry
                                 ,nullptr
                                 ,&dwType
                                 ,nullptr
                                 ,&dwCount);

    if (m_lResult != ERROR_SUCCESS)
    {
      return FALSE;
    }
		*pBytes = dwCount;
		*ppData = new BYTE[*pBytes];
		ASSERT(dwType == REG_BINARY);

		m_lResult = ::RegQueryValueEx(hSecKey
                                 ,(LPTSTR)lpszEntry
                                 ,nullptr
                                 ,&dwType
                                 ,*ppData
                                 ,&dwCount);

		if (m_lResult != ERROR_SUCCESS)
		{
      if(*ppData)
      {
        delete [] *ppData;
        *ppData = nullptr;
      }

			return FALSE;
		}

		ASSERT(dwType == REG_BINARY);
		return TRUE;
	}
	else
	{
		CString str = GetRegistryString(lpszSection, lpszEntry, nullptr);
    if (str.IsEmpty())
    {
      return FALSE;
    }
		ASSERT(str.GetLength()%2 == 0);
		int nLen = str.GetLength();
		*pBytes  = nLen/2;
		*ppData  = new BYTE[*pBytes];

    for (int i = 0; i < nLen; i += 2)
		{
			(*ppData)[i/2] = (BYTE) (((str[i + 1] - 'A') << 4) + (str[i] - 'A'));
		}
		return TRUE;
	}
}

CString 
RegistryManager::GetRegistryString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault)
{
  if(lpszSection == nullptr || lpszEntry == nullptr)
  {
    return "";
  }

  if (m_iniFileName.IsEmpty())
	{
		AutoRegistryKey hSecKey(GetSectionKey(lpszSection, KEY_READ));
    if (hSecKey == nullptr)
    {
      return lpszDefault;
    }
		CString strValue;
		DWORD dwType, dwCount;

		m_lResult = ::RegQueryValueEx(hSecKey
                                 ,(LPTSTR)lpszEntry
                                 ,nullptr
                                 ,&dwType
                                 ,nullptr
                                 ,&dwCount);

    if (m_lResult != ERROR_SUCCESS)
    {
      return lpszDefault;
    }
		ASSERT(dwType == REG_SZ);
		m_lResult = ::RegQueryValueEx(hSecKey
                                 ,(LPTSTR)lpszEntry
                                 ,nullptr
                                 ,&dwType
                                 ,(LPBYTE)strValue.GetBuffer(dwCount/sizeof(TCHAR))
                                 ,&dwCount);
		strValue.ReleaseBuffer();

    if (m_lResult != ERROR_SUCCESS)
    {
      return lpszDefault;
    }
		ASSERT(dwType == REG_SZ);
		return strValue;
	}
	else
	{
		TCHAR chNil = '\0';
		if(lpszDefault == nullptr)
		{
			lpszDefault = &chNil;    // don't pass in nullptr
		}

		TCHAR szT[4096];
    ::GetPrivateProfileString(lpszSection,lpszEntry,lpszDefault,szT,_countof(szT),m_iniFileName);
		return szT;
	}
}

int 
RegistryManager::EnumRegistryValues(LPCTSTR lpszSection, CMap<CString, LPCTSTR, DWORD, DWORD&>* mapItems,CStringArray * arrayNames)
{
  if(lpszSection == nullptr)
  {
    return 0;
  }

	AutoRegistryKey hKey(GetSectionKey(lpszSection, KEY_READ));
  if (hKey == nullptr)
  {
    return 0;
  }
	int index = 0;

	TCHAR szValue[512];
	DWORD dwLen = 512;
	DWORD dwType;

	while(::RegEnumValue(hKey
                      ,index++
                      ,szValue
                      ,&dwLen
                      ,nullptr
                      ,&dwType
                      ,nullptr
                      ,nullptr) == ERROR_SUCCESS)
	{
    if (mapItems)
    {
      mapItems->SetAt(szValue, dwType);
    }
    if (arrayNames)
    {
      arrayNames->Add(szValue);
    }
		dwLen = 512;
	}

	return --index;
}

int 
RegistryManager::EnumRegistryKeys(LPCTSTR lpszSection, CStringArray & arrayKeys)
{
  if(lpszSection == nullptr)
  {
    return 0;
  }

	AutoRegistryKey hKey(GetSectionKey(lpszSection, KEY_READ));
  if (hKey == nullptr)
  {
    return 0;
  }
	int index = 0;

	TCHAR szValue[512];
	DWORD dwLen = 512;

	while(::RegEnumKeyEx(hKey
                      ,index++
                      ,szValue
                      ,&dwLen
                      ,nullptr
                      ,nullptr
                      ,nullptr
                      ,nullptr) == ERROR_SUCCESS)
	{
		arrayKeys.Add(szValue);
		dwLen = 512;
	}

	return --index;
}

LONG 
RegistryManager::RecurseDeleteKey(HKEY hKey, LPCTSTR lpszKey)
{
	AutoRegistryKey hSubKey;
	LONG lRes = ::RegOpenKeyEx(hKey, lpszKey, 0, KEY_READ | KEY_WRITE, hSubKey);
  if (lRes != ERROR_SUCCESS)
  {
    return lRes;
  }

	FILETIME time;
	DWORD dwSize = 256;
	TCHAR szBuffer[256];

	while(::RegEnumKeyEx(hSubKey
                      ,0
                      ,szBuffer
                      ,&dwSize
                      ,nullptr
                      ,nullptr
                      ,nullptr
                      ,&time) == ERROR_SUCCESS)
	{
		lRes = RecurseDeleteKey(hSubKey, szBuffer);
    if (lRes != ERROR_SUCCESS)
    {
      return lRes;
    }
		dwSize = 256;
	}

	m_lResult = ::RegDeleteKey(hKey, lpszKey);
	return m_lResult;
}

bool 
RegistryManager::DeleteRegistryKey(LPCTSTR lpszSection, LPCTSTR lpszKey)
{
	AutoRegistryKey hSectionKey(GetSectionKey(lpszSection));
  if (hSectionKey == nullptr)
  {
    return false;
  }
	return (RecurseDeleteKey(hSectionKey, lpszKey) == ERROR_SUCCESS);
}

bool 
RegistryManager::DeleteRegistryValue(LPCTSTR lpszSection, LPCTSTR lpszValue)
{
	AutoRegistryKey hSecKey(GetSectionKey(lpszSection, KEY_ALL_ACCESS));
  if (hSecKey == nullptr)
  {
    return false;
  }
	m_lResult = ::RegDeleteValue(hSecKey, (LPTSTR)lpszValue);
	return (m_lResult == ERROR_SUCCESS);
}

BOOL 
RegistryManager::SetRegistryPoint(LPCTSTR lpszSection, LPCTSTR lpszEntry, CPoint* pValue)
{
  if(lpszSection == nullptr)
  {
    return FALSE;
  }

  if(m_iniFileName.IsEmpty())
	{
		AutoRegistryKey hSecKey(GetSectionKey(lpszSection));
    if (hSecKey == nullptr)
    {
      return FALSE;
    }
		m_lResult = ::RegSetValueEx(hSecKey
                               ,lpszEntry
                               ,NULL
                               ,REG_BINARY
                               ,(LPBYTE)pValue
                               ,sizeof(CPoint));

    if (m_lResult != ERROR_SUCCESS)
    {
      return FALSE;
    }
		return TRUE;
	}
	CString str;
	str.Format(_T("%i,%i"), pValue->x, pValue->y);
	return SetRegistryString(lpszSection, lpszEntry, str);
}

BOOL 
RegistryManager::GetRegistryPoint(LPCTSTR lpszSection, LPCTSTR lpszEntry, CPoint* ptResult)
{
  if (lpszSection == nullptr || lpszEntry == nullptr)
  {
    return FALSE;
  }

	if (m_iniFileName.IsEmpty())
	{
		AutoRegistryKey hSecKey(GetSectionKey(lpszSection, KEY_READ));
    if (hSecKey == nullptr)
    {
      return FALSE;
    }
		DWORD dwType, dwCount;
		m_lResult = ::RegQueryValueEx(hSecKey
                                 ,(LPTSTR)lpszEntry
                                 ,nullptr
                                 ,&dwType
                                 ,nullptr
                                 ,&dwCount);

    if (m_lResult != ERROR_SUCCESS)
    {
      return FALSE;
    }
		ASSERT(dwType == REG_BINARY);
		m_lResult = ::RegQueryValueEx(hSecKey
                                 ,(LPTSTR)lpszEntry
                                 ,nullptr
                                 ,&dwType
                                 ,(LPBYTE)ptResult
                                 ,&dwCount);

    if (m_lResult != ERROR_SUCCESS)
    {
      return FALSE;
    }
		ASSERT(dwType == REG_BINARY);
		return TRUE;
	}
	else
	{
		CString str = GetRegistryString(lpszSection, lpszEntry, nullptr);
    if (str.IsEmpty())
    {
      return FALSE;
    }
		_tscanf_s(str, _T("%ld,%ld"), &ptResult->x, &ptResult->y);
		return TRUE;
	}
}

BOOL 
RegistryManager::SetRegistryRect(LPCTSTR lpszSection, LPCTSTR lpszEntry, CRect* pValue)
{
  if(lpszSection == nullptr || lpszEntry == nullptr)
  {
    return FALSE;
  }

  if (m_iniFileName.IsEmpty())
	{
		AutoRegistryKey hSecKey(GetSectionKey(lpszSection));
    if (hSecKey == nullptr)
    {
      return FALSE;
    }
		m_lResult = ::RegSetValueEx(hSecKey
                               ,lpszEntry
                               ,NULL
                               ,REG_BINARY
                               ,(LPBYTE)pValue
                               ,sizeof(CRect));

    if (m_lResult != ERROR_SUCCESS)
    {
      return FALSE;
    }
		return TRUE;
	}

	CString str;
	str.Format(_T("%i,%i,%i,%i"), pValue->left, pValue->top, pValue->right, pValue->bottom);

	BOOL bResult = SetRegistryString(lpszSection, lpszEntry, str);

	return bResult;
}

BOOL 
RegistryManager::GetRegistryRect(LPCTSTR lpszSection, LPCTSTR lpszEntry, CRect* rcResult)
{
  if(lpszSection == nullptr || lpszEntry == nullptr)
  {
    return FALSE;
  }

	if (m_iniFileName.IsEmpty())
	{
		AutoRegistryKey hSecKey(GetSectionKey(lpszSection, KEY_READ));
    if (hSecKey == nullptr)
    {
      return FALSE;
    }
		DWORD dwType, dwCount;
		m_lResult = ::RegQueryValueEx(hSecKey
                                 ,(LPTSTR)lpszEntry
                                 ,nullptr
                                 ,&dwType
                                 ,nullptr
                                 ,&dwCount);

    if (m_lResult != ERROR_SUCCESS)
    {
      return FALSE;
    }
		ASSERT(dwType == REG_BINARY);
		m_lResult = ::RegQueryValueEx(hSecKey
                                 ,(LPTSTR)lpszEntry
                                 ,nullptr
                                 ,&dwType
                                 ,(LPBYTE)rcResult
                                 ,&dwCount);

    if (m_lResult != ERROR_SUCCESS)
    {
      return FALSE;
    }
	}
	else
	{
		// convert to string and write out
		CString str = GetRegistryString(lpszSection, lpszEntry, nullptr);
    if (str.IsEmpty())
    {
      return FALSE;
    }
		_tscanf_s(str, _T("%ld,%ld,%ld,%ld"), &rcResult->left, &rcResult->top, &rcResult->right, &rcResult->bottom);
	}
  return TRUE;
}

BOOL 
RegistryManager::SetRegistrySize(LPCTSTR lpszSection, LPCTSTR lpszEntry, CSize* pValue)
{
  if(lpszSection == nullptr || lpszEntry == nullptr)
  {
    return FALSE;
  }

  if(m_iniFileName.IsEmpty())
	{
		AutoRegistryKey hSecKey(GetSectionKey(lpszSection));
    if (hSecKey == nullptr)
    {
      return FALSE;
    }
		m_lResult = ::RegSetValueEx(hSecKey
                               ,lpszEntry
                               ,NULL
                               ,REG_BINARY
                               ,(LPBYTE)pValue
                               ,sizeof(CSize));

    if (m_lResult != ERROR_SUCCESS)
    {
      return FALSE;
    }
		return TRUE;
	}

	CString str;
	str.Format(_T("%i,%i"), pValue->cx, pValue->cy);

	return SetRegistryString(lpszSection, lpszEntry, str);
}

BOOL 
RegistryManager::GetRegistrySize(LPCTSTR lpszSection, LPCTSTR lpszEntry, CSize* szResult)
{
  if(lpszSection == nullptr || lpszEntry == nullptr)
  {
    return FALSE;
  }

	if (m_iniFileName.IsEmpty())
	{
		AutoRegistryKey hSecKey(GetSectionKey(lpszSection, KEY_READ));
    if (hSecKey == nullptr)
    {
      return FALSE;
    }
		DWORD dwType, dwCount;
		m_lResult = ::RegQueryValueEx(hSecKey
                                 ,(LPTSTR)lpszEntry
                                 ,nullptr
                                 ,&dwType
                                 ,nullptr
                                 ,&dwCount);

    if (m_lResult != ERROR_SUCCESS)
    {
      return FALSE;
    }
		ASSERT(dwType == REG_BINARY);
		m_lResult = ::RegQueryValueEx(hSecKey
                                 ,(LPTSTR)lpszEntry
                                 ,nullptr
                                 ,&dwType
                                 ,(LPBYTE)szResult
                                 ,&dwCount);

    if (m_lResult != ERROR_SUCCESS)
    {
      return FALSE;
    }
		ASSERT(dwType == REG_BINARY);
	}
	else
	{
		// convert to string and write out
		CString str = GetRegistryString(lpszSection, lpszEntry, nullptr);
    if (str.IsEmpty())
    {
      return FALSE;
    }
		_tscanf_s(str, _T("%i,%i"), &szResult->cx, &szResult->cy);
	}
  return TRUE;
}

BOOL 
RegistryManager::SetRegistryDouble(LPCTSTR lpszSection, LPCTSTR lpszEntry, double* pValue)
{
  if(lpszSection == nullptr || lpszEntry == nullptr)
  {
    return FALSE;
  }

  if (m_iniFileName.IsEmpty())
	{
		AutoRegistryKey hSecKey(GetSectionKey(lpszSection));
    if (hSecKey == nullptr)
    {
      return FALSE;
    }
		m_lResult = ::RegSetValueEx(hSecKey
                               ,lpszEntry
                               ,NULL
                               ,REG_BINARY
                               ,(LPBYTE)pValue
                               ,sizeof(*pValue));

    if (m_lResult != ERROR_SUCCESS)
    {
      return FALSE;
    }
		return TRUE;
	}

	LPBYTE pData = (LPBYTE) pValue;
	UINT nBytes = sizeof(double);
	LPTSTR lpsz = new TCHAR[nBytes * 2 + 1];
	UINT i;
	for (i = 0; i < nBytes; i++)
	{
		lpsz[i * 2    ] = (TCHAR)( (pData[i]       & 0x0F) + 'A'); // low nibble
		lpsz[i * 2 + 1] = (TCHAR)(((pData[i] >> 4) & 0x0F) + 'A'); // high nibble
	}
	lpsz[i * 2] = 0;

	ASSERT(m_iniFileName.IsEmpty() == FALSE);

	BOOL bResult = SetRegistryString(lpszSection, lpszEntry, lpsz);
	delete[] lpsz;
	return bResult;
}

BOOL 
RegistryManager::GetRegistryDouble(LPCTSTR lpszSection, LPCTSTR lpszEntry, double* dResult)
{
  if(lpszSection == nullptr || lpszEntry == nullptr)
  {
    return FALSE;
  }

	if (m_iniFileName.IsEmpty())
	{

		AutoRegistryKey hSecKey(GetSectionKey(lpszSection, KEY_READ));
    if (hSecKey == nullptr)
    {
      return FALSE;
    }
		DWORD dwType, dwCount;
		m_lResult = ::RegQueryValueEx(hSecKey
                                 ,(LPTSTR)lpszEntry
                                 ,nullptr
                                 ,&dwType
                                 ,nullptr
                                 ,&dwCount);

    if (m_lResult != ERROR_SUCCESS)
    {
      return FALSE;
    }
		ASSERT(dwType == REG_BINARY);
		m_lResult = ::RegQueryValueEx(hSecKey
                                 ,(LPTSTR)lpszEntry
                                 ,nullptr
                                 ,&dwType
                                 ,(LPBYTE)dResult
                                 ,&dwCount);

    if (m_lResult != ERROR_SUCCESS)
    {
      return FALSE;
    }
		ASSERT(dwType == REG_BINARY);
		return TRUE;
	}
	else
	{
		CString str = GetRegistryString(lpszSection, lpszEntry, nullptr);
    if (str.IsEmpty())
    {
      return FALSE;
    }
		ASSERT(str.GetLength()%2 == 0);
		int nLen = str.GetLength();

		LPBYTE pData = (LPBYTE) dResult;

		int i;
		for (i = 0; i < nLen; i += 2)
		{
			(pData)[i/2] = (BYTE)	(((str[i + 1] - 'A') << 4) + (str[i] - 'A'));
		}

		return TRUE;
	}
}

BOOL
RegistryManager::SetRegistryDWORD(LPCTSTR lpszSection, LPCTSTR lpszEntry, DWORD* pValue)
{
  if(lpszSection == nullptr || lpszEntry == nullptr)
  {
    return FALSE;
  }
	if (m_iniFileName.IsEmpty())
	{
		AutoRegistryKey hSecKey(GetSectionKey(lpszSection));
    if (hSecKey == nullptr)
    {
      return FALSE;
    }
		m_lResult = ::RegSetValueEx(hSecKey
                               ,lpszEntry
                               ,NULL
                               ,REG_DWORD
                               ,(LPBYTE)pValue
                               ,sizeof(*pValue));

    if (m_lResult != ERROR_SUCCESS)
    {
      return FALSE;
    }
		return TRUE;
	}


	return SetRegistryInteger(lpszSection, lpszEntry, int(*pValue));
}

BOOL 
RegistryManager::GetRegistryDWORD(LPCTSTR lpszSection, LPCTSTR lpszEntry, DWORD* dwResult)
{
  if(lpszSection == nullptr || lpszEntry == nullptr)
  {
    return FALSE;
  }

	if (m_iniFileName.IsEmpty())
	{
		AutoRegistryKey hSecKey(GetSectionKey(lpszSection, KEY_READ));
    if (hSecKey == nullptr)
    {
      return FALSE;
    }
		DWORD dwType, dwCount;
		m_lResult = ::RegQueryValueEx(hSecKey
                                 ,(LPTSTR)lpszEntry
                                 ,nullptr
                                 ,&dwType
                                 ,nullptr
                                 ,&dwCount);

    if (m_lResult != ERROR_SUCCESS)
    {
      return FALSE;
    }
		ASSERT(dwType == REG_DWORD);
		m_lResult = ::RegQueryValueEx(hSecKey
                                 ,(LPTSTR)lpszEntry
                                 ,nullptr
                                 ,&dwType
                                 ,(LPBYTE)dwResult
                                 ,&dwCount);

    if (m_lResult != ERROR_SUCCESS)
    {
      return FALSE;
    }
		ASSERT(dwType == REG_DWORD);
	}
	else
	{

		CString str = GetRegistryString(lpszSection, lpszEntry, nullptr);
    if (str.IsEmpty())
    {
      return FALSE;
    }
		*dwResult = (DWORD)GetRegistryInteger(lpszSection, lpszEntry, 0);
	}
  return TRUE;
}

BOOL 
RegistryManager::SetRegistryColor(LPCTSTR lpszSection, LPCTSTR lpszEntry, COLORREF* pValue)
{
	return SetRegistryDWORD(lpszSection, lpszEntry, pValue);
}

BOOL 
RegistryManager::GetRegistryColor(LPCTSTR lpszSection, LPCTSTR lpszEntry, COLORREF* rgbResult)
{
	return GetRegistryDWORD(lpszSection, lpszEntry, rgbResult);
}

CString 
RegistryManager::GetErrorMessage() const
{
	LPVOID lpMsgBuf = 0;

	::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		              FORMAT_MESSAGE_FROM_SYSTEM |
		              FORMAT_MESSAGE_IGNORE_INSERTS,
		              nullptr,
		              GetErrorCode(),
		              MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		              (LPTSTR) &lpMsgBuf,
		              0,
		              nullptr);

	CString csMessage((LPCTSTR)lpMsgBuf);

	// Free the buffer.
	::LocalFree(lpMsgBuf);

	return csMessage;
}

//////////////////////////////////////////////////////////////////////////
//
// Setting the registry profile version before program start
// Combines with the "Workspace" of CFrameWnd(ex) and CMDIFrameWnd(ex)
//
//////////////////////////////////////////////////////////////////////////

int SetRegistryProfile(CString p_key,CString p_program,CString p_version)
{
  RegistryManager manager;
  manager.SetRegistryKey(p_key,p_program);

  CString version = manager.GetRegistryString(_T("Settings"),_T("StyleVersion"),_T("1.0"));
  if(version.CompareNoCase(p_version))
  {
    // Not our version: delete the workspace of the program
    manager.DeleteRegistryKey(_T("Workspace"),_T(""));
    // Next time: use this version
    manager.SetRegistryString(_T("Settings"),_T("StyleVersion"),p_version);
    return 1;
  }
  return 0;
}

//////////////////////////////////////////////////////////////////////////
//
// Things that use a Registry Manager
//
//////////////////////////////////////////////////////////////////////////

// Setting a global environment variable (extends SetEnvironmentVariable)
bool
SetGlobalEnvironmentVariable(CString p_variable,CString p_value)
{
  bool success(false);
  RegistryManager manager(HKEY_LOCAL_MACHINE);
  CString key(_T("HKLM\\System\\CurrentControlSet\\Control\\Session Manager\\Environment"));

  // Possibly remove old value
  success = manager.DeleteRegistryKey(key,p_variable);
  if(!p_value.IsEmpty())
  {
    // Try to set new value
    success = manager.SetRegistryString(key,p_variable,p_value);
  }
  if(success)
  {
    // Broadcast the changing of the system variable
    SendMessageTimeout(HWND_BROADCAST,WM_SETTINGCHANGE,NULL,NULL,SMTO_NORMAL,1500,NULL);
  }
  return success;
}
