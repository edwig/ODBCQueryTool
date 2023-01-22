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
#pragma once

// Setting the registry key and profile before starting
int SetRegistryProfile(CString p_key,CString p_program,CString p_version);

// Setting a global environment variable (extends SetEnvironmentVariable)
bool SetGlobalEnvironmentVariable(CString p_variable,CString p_value);

//////////////////////////////////////////////////////////////////////////
//
//  RegistryManager is a stand alone registry management class. 
//  You can use it to read and write values from your system's registry
//  Reading and writing to an *.INI file is an option. 
//  Use RegistryManager to read and write string, integer, and binary data 
//  to and from the registry.
//
//////////////////////////////////////////////////////////////////////////

class RegistryManager
{
private:
	class AutoRegistryKey;

public:
	RegistryManager(HKEY hKeyBase = HKEY_CURRENT_USER);
	virtual ~RegistryManager();

  void SetRegistryKey(LPCTSTR lpszRegistryKey, LPCTSTR lpszProfileName);

	static void     SetINIFileName(LPCTSTR strINIFileName);
	static CString& GetINIFileName();

  // SETTERS
	BOOL    SetRegistryInteger(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue);
	BOOL    SetRegistryBinary (LPCTSTR lpszSection, LPCTSTR lpszEntry, LPBYTE pData, UINT nBytes);
	BOOL    SetRegistryString (LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue);
	BOOL    SetRegistryPoint  (LPCTSTR lpszSection, LPCTSTR lpszEntry, CPoint* pValue);
	BOOL    SetRegistryRect   (LPCTSTR lpszSection, LPCTSTR lpszEntry, CRect* pValue);
	BOOL    SetRegistrySize   (LPCTSTR lpszSection, LPCTSTR lpszEntry, CSize* pValue);
	BOOL    SetRegistryDouble (LPCTSTR lpszSection, LPCTSTR lpszEntry, double* pValue);
	BOOL    SetRegistryDWORD  (LPCTSTR lpszSection, LPCTSTR lpszEntry, DWORD* pValue);
	BOOL    SetRegistryColor  (LPCTSTR lpszSection, LPCTSTR lpszEntry, COLORREF* pValue);

  // GETTERS
  UINT    GetRegistryInteger(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault);
  BOOL    GetRegistryBinary (LPCTSTR lpszSection, LPCTSTR lpszEntry, BYTE** ppData, UINT* pBytes);
	CString GetRegistryString (LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault);
	BOOL    GetRegistryPoint  (LPCTSTR lpszSection, LPCTSTR lpszEntry, CPoint* ptResult);
	BOOL    GetRegistryRect   (LPCTSTR lpszSection, LPCTSTR lpszEntry, CRect* rcResult);
	BOOL    GetRegistrySize   (LPCTSTR lpszSection, LPCTSTR lpszEntry, CSize* rcResult);
	BOOL    GetRegistryDouble (LPCTSTR lpszSection, LPCTSTR lpszEntry, double* rcResult);
	BOOL    GetRegistryDWORD  (LPCTSTR lpszSection, LPCTSTR lpszEntry, DWORD* rcResult);
	BOOL    GetRegistryColor  (LPCTSTR lpszSection, LPCTSTR lpszEntry, COLORREF* rcResult);

  int     EnumRegistryValues(LPCTSTR lpszSection, CMap<CString, LPCTSTR, DWORD, DWORD&>*  mapItems, CStringArray* arrayNames);
  int     EnumRegistryKeys  (LPCTSTR lpszSection, CStringArray & arrayKeys);

	bool    DeleteRegistryKey  (LPCTSTR lpszSection, LPCTSTR lpszKey);
	bool    DeleteRegistryValue(LPCTSTR lpszSection, LPCTSTR lpszKey);

	LONG    GetErrorCode()    const;
	CString GetErrorMessage() const;

protected:
	virtual HKEY GetAppRegistryKey(REGSAM samDesired = KEY_WRITE | KEY_READ);
	virtual HKEY GetSectionKey(LPCTSTR lpszSection, REGSAM samDesired = KEY_WRITE | KEY_READ);

private:
	BOOL    GetProfileInfo();
	LONG    RecurseDeleteKey(HKEY hKey, LPCTSTR lpszKey);
	HKEY    GetFullPathKey(LPCTSTR lpszSection) const;
	HKEY    RecurseOpenKey(LPCTSTR lpszSection, REGSAM samDesired);

	LONG    m_lResult;

protected:

  HKEY    m_keyBase;            // Handle to the registry key to use. The default is HKEY_CURRENT_USER.
  CString m_registryKey;        // Used to determine the full registry key for storing application profile settings.
  CString m_profileName;        // The application's .INI filename.

  static CString m_iniFileName; // Name of an .INI file for registry settings.
};

//////////////////////////////////////////////////////////////////////

inline void 
RegistryManager::SetINIFileName(LPCTSTR strINIFileName) 
{
	m_iniFileName = strINIFileName;
}

inline 
CString& RegistryManager::GetINIFileName() 
{
	return m_iniFileName;
}

inline
LONG RegistryManager::GetErrorCode() const 
{
	return m_lResult;
}

/////////////////////////////////////////////////////////////////////////////
// CHKey - helper automatically closes open HKEY

class RegistryManager::AutoRegistryKey
{
public:
  AutoRegistryKey(HKEY hKey = nullptr) : m_hKey(hKey)
  {
  }

  ~AutoRegistryKey()
  {
    if (m_hKey != nullptr)
    {
      ::RegCloseKey(m_hKey);
    }
  }

  HKEY& operator=(HKEY hKey)
  {
    if (m_hKey != nullptr)
    {
      ::RegCloseKey(m_hKey);
    }
    m_hKey = hKey;
    return m_hKey;
  }

  BOOL operator==(HKEY hKey)
  {
    return m_hKey == hKey;
  }

  operator HKEY() const
  {
    return m_hKey;
  }

  operator PHKEY()
  {
    return &m_hKey;
  }

private:
  HKEY m_hKey;
};

