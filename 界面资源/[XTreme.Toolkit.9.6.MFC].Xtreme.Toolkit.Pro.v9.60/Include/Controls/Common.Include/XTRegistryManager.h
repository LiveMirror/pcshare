// XTRegistryManager.h : header file
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// ©1998-2005 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

//{{AFX_CODEJOCK_PRIVATE
#ifndef __XTREGISTRYMANAGER_H__
#define __XTREGISTRYMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     CXTRegistryManager is a stand alone registry management class. You
//     can use it to read and write values from your system's registry or an
//     .INI file. Use CXTRegistryManager to read and write string, integer,
//     and binary data to and from the registry. You can also specify an .INI
//     file name, and have these values stored there instead.
//===========================================================================
class _XT_EXT_CLASS CXTRegistryManager
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTRegistryManager object
	// Parameters:
	//     hKeyBase - Current key to be used in the registry.
	//-----------------------------------------------------------------------
	CXTRegistryManager(HKEY hKeyBase = HKEY_CURRENT_USER);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTRegistryManager object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTRegistryManager();

protected:

	HKEY     m_hKeyBase;        // Handle to the registry key to use. The default is HKEY_CURRENT_USER.
	LPCTSTR  m_pszRegistryKey;  // Used to determine the full registry key for storing application profile settings.
	LPCTSTR  m_pszProfileName;  // The application’s .INI filename.

	static CString m_strINIFileName;  // Name of an .INI file for registry settings.

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the .INI file name for the registry
	//     manager to use instead of the system registry.
	// Parameters:
	//     strINIFileName - Full path to the .INI file.
	//-----------------------------------------------------------------------
	static void SetINIFileName(CString strINIFileName);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will return the file name of the .INI file
	//     used by the registry manager.
	// Returns:
	//     A reference to the CString object that contains the INI file path.
	//-----------------------------------------------------------------------
	static CString& GetINIFileName();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to write the specified INT value into
	//     the specified section of the application’s registry or .INI file.
	// Parameters:
	//     lpszSection - Points to a null-terminated string that specifies the section
	//                   containing the entry. If the section does not exist, it is created.
	//                   The name of the section is case independent. The string may be any
	//                   combination of uppercase and lowercase letters.
	//     lpszEntry   - Points to a null-terminated string that contains the entry into
	//                   which the value is to be written. If the entry does not exist in
	//                   the specified section, it is created.
	//     nValue      - Contains the value to be written.
	// Returns:
	//     Nonzero if successful, otherwise returns zero.
	//-----------------------------------------------------------------------
	BOOL WriteProfileInt(LPCTSTR lpszSection,LPCTSTR lpszEntry,int nValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to write the specified binary value into
	//     the specified section of the application’s registry or .INI file.
	// Parameters:
	//     lpszSection - Points to a null-terminated string that specifies the section
	//                   containing the entry. If the section does not exist, it is created.
	//                   The name of the section is case independent. The string may be any
	//                   combination of uppercase and lowercase letters.
	//     lpszEntry   - Points to a null-terminated string that contains the entry into
	//                   which the value is to be written. If the entry does not exist in
	//                   the specified section, it is created.
	//     pData       - Contains the value to be written.
	//     nBytes      - Contains the size of the value to be written.
	// Returns:
	//     Nonzero if successful, otherwise returns zero.
	//-----------------------------------------------------------------------
	BOOL WriteProfileBinary(LPCTSTR lpszSection,LPCTSTR lpszEntry,LPBYTE pData,UINT nBytes);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to write the specified string value into
	//     the specified section of the application’s registry or .INI file.
	// Parameters:
	//     lpszSection - Points to a null-terminated string that specifies the section
	//                   containing the entry. If the section does not exist, it is created.
	//                   The name of the section is case independent. The string may be any
	//                   combination of uppercase and lowercase letters.
	//     lpszEntry   - Points to a null-terminated string that contains the entry into
	//                   which the value is to be written. If the entry does not exist in
	//                   the specified section, it is created.
	//     lpszValue   - Contains the value to be written.
	// Returns:
	//     Nonzero if successful, otherwise returns zero.
	//-----------------------------------------------------------------------
	BOOL WriteProfileString(LPCTSTR lpszSection,LPCTSTR lpszEntry,LPCTSTR lpszValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to write the specified CPoint value into
	//     the specified section of the application’s registry or .INI file.
	// Parameters:
	//     lpszSection - Points to a null-terminated string that specifies the section
	//                   containing the entry. If the section does not exist, it is created.
	//                   The name of the section is case independent. The string may be any
	//                   combination of uppercase and lowercase letters.
	//     lpszEntry   - Points to a null-terminated string that contains the entry into
	//                   which the value is to be written. If the entry does not exist in
	//                   the specified section, it is created.
	//     pValue      - Contains the value to be written.
	// Returns:
	//     Nonzero if successful, otherwise returns zero.
	//-----------------------------------------------------------------------
	BOOL WriteProfilePoint(LPCTSTR lpszSection,LPCTSTR lpszEntry,CPoint * pValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to write the specified CRect value into
	//     the specified section of the application’s registry or .INI file.
	// Parameters:
	//     lpszSection - Points to a null-terminated string that specifies the section
	//                   containing the entry. If the section does not exist, it is created.
	//                   The name of the section is case independent. The string may be any
	//                   combination of uppercase and lowercase letters.
	//     lpszEntry   - Points to a null-terminated string that contains the entry into
	//                   which the value is to be written. If the entry does not exist in
	//                   the specified section, it is created.
	//     pValue      - Contains the value to be written.
	// Returns:
	//     Nonzero if successful, otherwise returns zero.
	//-----------------------------------------------------------------------
	BOOL WriteProfileRect(LPCTSTR lpszSection,LPCTSTR lpszEntry,CRect * pValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to write the specified CSize value into
	//     the specified section of the application’s registry or .INI file.
	// Parameters:
	//     lpszSection - Points to a null-terminated string that specifies the section
	//                   containing the entry. If the section does not exist, it is created.
	//                   The name of the section is case independent. The string may be any
	//                   combination of uppercase and lowercase letters.
	//     lpszEntry   - Points to a null-terminated string that contains the entry into
	//                   which the value is to be written. If the entry does not exist in
	//                   the specified section, it is created.
	//     pValue      - Contains the value to be written.
	// Returns:
	//     Nonzero if successful, otherwise returns zero.
	//-----------------------------------------------------------------------
	BOOL WriteProfileSize(LPCTSTR lpszSection,LPCTSTR lpszEntry,CSize * pValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to write the specified double/float value
	//     into the specified section of the application’s registry or .INI file.
	// Parameters:
	//     lpszSection - Points to a null-terminated string that specifies the section
	//                   containing the entry. If the section does not exist, it is created.
	//                   The name of the section is case independent. The string may be any
	//                   combination of uppercase and lowercase letters.
	//     lpszEntry   - Points to a null-terminated string that contains the entry into
	//                   which the value is to be written. If the entry does not exist in
	//                   the specified section, it is created.
	//     pValue      - Contains the value to be written.
	// Returns:
	//     Nonzero if successful, otherwise returns zero.
	//-----------------------------------------------------------------------
	BOOL WriteProfileDouble(LPCTSTR lpszSection,LPCTSTR ,double * pValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to write the specified DWORD value into
	//     the specified section of the application’s registry or .INI file.
	// Parameters:
	//     lpszSection - Points to a null-terminated string that specifies the section
	//                   containing the entry. If the section does not exist, it is created.
	//                   The name of the section is case independent. The string may be any
	//                   combination of uppercase and lowercase letters.
	//     lpszEntry   - Points to a null-terminated string that contains the entry into
	//                   which the value is to be written. If the entry does not exist in
	//                   the specified section, it is created.
	//     pValue      - Contains the value to be written.
	//
	//          Note that as COLORREF is a typedef for a DWORD, WriteProfileColor calls
	//          this function.
	// Returns:
	//     Nonzero if successful, otherwise returns zero.
	//-----------------------------------------------------------------------
	BOOL WriteProfileDword(LPCTSTR lpszSection,LPCTSTR lpszEntry,DWORD * pValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to write the specified COLORREF value
	//     into the specified section of the application’s registry or .INI file.
	// Parameters:
	//     lpszSection - Points to a null-terminated string that specifies the section
	//                   containing the entry. If the section does not exist, it is created.
	//                   The name of the section is case independent. The string may be any
	//                   combination of uppercase and lowercase letters.
	//     lpszEntry   - Points to a null-terminated string that contains the entry into
	//                   which the value is to be written. If the entry does not exist in
	//                   the specified section, it is created.
	//     pValue      - Contains the value to be written.
	//
	//          Note that because COLORREF is a typedef for a DWORD, this function only
	//          calls WriteProfileDword.
	// Returns:
	//     Nonzero if successful, otherwise returns zero.
	//-----------------------------------------------------------------------
	BOOL WriteProfileColor(LPCTSTR lpszSection,LPCTSTR lpszEntry,COLORREF * pValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the value of an integer from
	//     an entry within a specified section of the application’s registry or
	//     .INI file.
	// Parameters:
	//     lpszSection - Points to a null-terminated string that specifies the section
	//                   containing the entry.
	//     lpszEntry   - Points to a null-terminated string that contains the entry whose
	//                   value is to be retrieved.
	//     nDefault    - Specifies the default value to return if the framework cannot
	//                   find the entry. This value can be an unsigned value in the range 0
	//                   through 65,535 or a signed value in the range –32,768 through 32,767.
	//
	//          This member function supports hexadecimal notation for the value
	//          in the .INI file. When you retrieve a signed integer, you should cast
	//          the value into an int.
	// Returns:
	//     The integer value of the string that follows the
	//     specified entry if the function is successful. The return value is
	//     the value of the 'nDefault' parameter if the function does not find the
	//     entry. The return value is zero if the value that corresponds to the
	//     specified entry is not an integer.
	//-----------------------------------------------------------------------
	UINT GetProfileInt(LPCTSTR lpszSection,LPCTSTR lpszEntry,int nDefault);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the binary value from an entry
	//     within a specified section of the application’s registry or .INI file.
	// Parameters:
	//     lpszSection - Points to a null-terminated string that specifies the section
	//                   containing the entry.
	//     lpszEntry   - Points to a null-terminated string that contains the entry whose
	//                   value is to be retrieved.
	//     pBytes      - Address of a UINT to receive the size of 'ppData'.
	//     ppData      - Address of a LPBYTE data member to receive the value. It is the
	//     caller's responsibility to free the memory allocated for 'ppData'.
	// Example: An example of this would be:
	//
	//          <code>
	//          void CXTDockWindow::LoadBarSize(CString strSection)
	//          {
	//              XT_MANAGE_STATE;  // Switch to toolkit resources/module state
	//
	//              // restore the previous bar style.
	//              if(::IsWindow(m_hWnd))
	//              {
	//                  UINT nSize = 0;
	//                  LPBYTE pbtData = 0;
	//                  CString string;
	//                  string.LoadString(XT_IDS_REG_BARPOS);
	//
	//                  CXTRegistryManager regManager;
	//                  if (regManager.GetProfileBinary( strSection,
	//                      string, &pbtData, &nSize))
	//                  {
	//                      XT_BARPLACEMENT* pBP =
	//                          reinterpret_cast<XT_BARPLACEMENT*>(pbtData);
	//
	//                      SetBarPlacement(pBP);
	//                      delete [] pbtData;
	//                  }
	//              }
	//          }</code>
	// Returns:
	//     Nonzero if successful, otherwise returns zero.
	//-----------------------------------------------------------------------
	BOOL GetProfileBinary(LPCTSTR lpszSection,LPCTSTR lpszEntry,BYTE** ppData,UINT* pBytes);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the string associated with
	//     an entry within the specified section in the application’s registry
	//     or .INI file. The return value is the string from the application’s
	//     .INI file or 'lpszDefault' if the string cannot be found. The maximum
	//     string length supported by the framework is _MAX_PATH. If 'lpszDefault'
	//     is NULL, the return value is an empty string.
	// Parameters:
	//     lpszSection - Points to a null-terminated string that specifies the section
	//                   containing the entry.
	//     lpszEntry   - Points to a null-terminated string that contains the entry whose
	//                   value is to be retrieved.
	//     lpszDefault - Points to the default string value for the given entry if the
	//                   entry cannot be found in the initialization file.
	// Returns:
	//     The string associated with an entry within the specified section in the
	//     application’s registry or .INI file. The return value is the string from the
	//     application’s .INI file or 'lpszDefault' if the string cannot be found.
	//-----------------------------------------------------------------------
	CString GetProfileString(LPCTSTR lpszSection,LPCTSTR lpszEntry,LPCTSTR lpszDefault);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the value of a CPoint from
	//     an entry within a specified section of the application’s registry or
	//     .INI file. Retrieves the CPoint value that follows the specified entry
	//     if the function is successful.
	// Parameters:
	//     lpszSection - Points to a null-terminated string that specifies the section
	//                   containing the entry.
	//     lpszEntry   - Points to a null-terminated string that contains the entry whose
	//                   value is to be retrieved.
	//     ptResult    - Points to the variable to be filled from the registry.
	// Returns:
	//     TRUE if the function succeeds, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL GetProfilePoint(LPCTSTR lpszSection,LPCTSTR lpszEntry,CPoint * ptResult);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the value of a CRect from
	//     an entry within a specified section of the application’s registry or
	//     .INI file. Retrieves the CRect value that follows the specified entry
	//     if the function is successful.
	// Parameters:
	//     lpszSection - Points to a null-terminated string that specifies the section
	//                   containing the entry.
	//     lpszEntry   - Points to a null-terminated string that contains the entry whose
	//                   value is to be retrieved.
	//     rcResult    - Points to the variable to be filled from the registry.
	// Returns:
	//     TRUE if the function succeeds, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL GetProfileRect(LPCTSTR lpszSection,LPCTSTR lpszEntry,CRect * rcResult);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the value of a CSize from
	//     an entry within a specified section of the application’s registry or
	//     .INI file. Retrieves the CSize value that follows the specified entry
	//     if the function is successful.
	// Parameters:
	//     lpszSection - Points to a null-terminated string that specifies the section
	//                   containing the entry.
	//     lpszEntry   - Points to a null-terminated string that contains the entry whose
	//                   value is to be retrieved.
	//     rcResult    - Points to the variable to be filled from the registry.
	// Returns:
	//     TRUE if the function succeeds, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL GetProfileSize(LPCTSTR lpszSection,LPCTSTR lpszEntry,CSize * rcResult);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the value of a double from
	//     an entry within a specified section of the application’s registry or
	//     .INI file. Retrieves the value that follows the specified entry if the
	//     function is successful.
	// Parameters:
	//     lpszSection - Points to a null-terminated string that specifies the section
	//                   containing the entry.
	//     lpszEntry   - Points to a null-terminated string that contains the entry whose
	//                   value is to be retrieved.
	//     rcResult    - Points to the variable to be filled from the registry.
	// Returns:
	//     TRUE if the function succeeds, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL GetProfileDouble(LPCTSTR lpszSection,LPCTSTR lpszEntry,double * rcResult);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the value of a DWORD from
	//     an entry within a specified section of the application’s registry or
	//     .INI file. Retrieves the value that follows the specified entry if the
	//     function is successful.
	// Parameters:
	//     lpszSection - Points to a null-terminated string that specifies the section
	//                   containing the entry.
	//     lpszEntry   - Points to a null-terminated string that contains the entry whose
	//                   value is to be retrieved.
	//     rcResult    - Points to the variable to be filled from the registry.
	//
	//          Note that because COLORREF is a typedef for a DWORD, GetProfileColor calls
	//          this function.
	// Returns:
	//     TRUE if the function succeeds, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL GetProfileDword(LPCTSTR lpszSection,LPCTSTR lpszEntry,DWORD * rcResult);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the value of a COLORREF from
	//     an entry within a specified section of the application’s registry or
	//     .INI file. Retrieves the value that follows the specified entry if the
	//     function is successful.
	// Parameters:
	//     lpszSection - Points to a null-terminated string that specifies the section
	//                   containing the entry.
	//     lpszEntry   - Points to a null-terminated string that contains the entry whose
	//                   value is to be retrieved.
	//     rcResult    - Points to the variable to be filled from the registry.
	//
	//          Note that because COLORREF is a typedef for a DWORD, this function only
	//          calls GetProfileDword.
	// Returns:
	//     TRUE if the function succeeds, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL GetProfileColor(LPCTSTR lpszSection,LPCTSTR lpszEntry,COLORREF * rcResult);

	// -----------------------------------------------------------------------------
	// Summary:
	//     Call this function to fill a CMap (CString to DWORD) with the
	//     names and value types under a specific registry section.
	// Parameters:
	//     lpszSection -  Points to a null\-terminated string that specifies the
	//                    section containing the entry.
	//     mapItems -     A reference to a CMap, which will contain the items in the
	//                    registry under this section. Can be NULL. Does not include
	//                    subsection names.
	//     arrayNames -   A reference to a CStringArray which, if not NULL, will
	//                    contain an array of names in the order they were read from
	//                    the registry.
	// Remarks:
	//     The function also takes a pointer to a CStringArray because CMap
	//     is a sorted container; therefore, it is not possible to get the
	//     items in the order they appeared from it.
	//
	//     Pass NULL for the CStringArray* if you don't want to use it, and
	//     NULL into the CMap if you only require a list of the item names.
	//
	//     The DWORD values returned are as follows:
	//
	//     * <b>REG_BINARY</b> Binary data in any form.
	//     * <b>REG_DWORD</b> A 32-bit number.
	//     * <b>REG_DWORD_LITTLE_ENDIAN</b> A 32-bit number in
	//           little-endian format. This is equivalent to REG_DWORD. In
	//           little-endian format, a multi-byte value is stored in memory from
	//           the lowest byte (the "little end") to the highest byte. For
	//           \example, the value 0x12345678 is stored as (0x78 0x56 0x34 0x12)
	//           in little-endian format. Windows NT/Windows 2000, Windows 95, and
	//           Windows 98 are designed to run on little-endian computer
	//           architectures. A user may connect to computers that have
	//           big-endian architectures, such as some UNIX systems.
	//     * <b>REG_DWORD_BIG_ENDIAN</b> A 32-bit number in big-endian
	//           format. In big-endian format, a multi-byte value is stored in
	//           memory from the highest byte (the "big end") to the lowest byte.
	//           For example, the value 0x12345678 is stored as (0x12 0x34 0x56
	//           0x78) in big-endian format.
	//     * <b>REG_EXPAND_SZ</b> A null-terminated string that contains
	//           unexpanded references to environment variables (for example,
	//           "%PATH%"). It will be a Unicode or ANSI string depending on
	//           whether you use the Unicode or ANSI functions. To expand the
	//           environment variable references, use the ExpandEnvironmentStrings
	//           function.
	//     * <b>REG_LINK</b> A Unicode symbolic link. Used internally.
	//           Applications should not use this type.
	//     * <b>REG_MULTI_SZ</b> An array of null-terminated strings,
	//           terminated by two null characters.
	//     * <b>REG_NONE</b> No defined value type.
	//     * <b>REG_QWORD</b> A 64-bit number.
	//     * <b>REG_QWORD_LITTLE_ENDIAN</b> A 64-bit number in
	//           little-endian format. This is equivalent to REG_QWORD.
	//     * <b>REG_RESOURCE_LIST</b> A device-driver resource list.
	//     * <b>REG_SZ</b> A null-terminated string. It will be a Unicode
	//           \or ANSI string, depending on whether or not you use the Unicode
	//           \or ANSI functions.
	// Returns:
	//     Zero if the section is empty; otherwise it returns the number of
	//     values present.
	// -----------------------------------------------------------------------------
	int EnumValues(LPCTSTR lpszSection,CMap<CString, LPCTSTR, DWORD, DWORD&> * mapItems,CStringArray * arrayNames);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to fill a CStringArray with the names of keys below
	//     the key specified. The return value indicates the number of keys found
	//     by the function, which can be used to check if any were found.
	//     CStringArray::GetSize will give the same result if needed, assuming
	//     the array is zeroed when it goes in. The function does not zero it,
	//     leaving the possibility of getting a list of subkeys of more than one
	//     key for whatever reason.
	// Parameters:
	//     lpszSection - Points to a null-terminated string that specifies the section
	//                   containing the entry.
	//     arrayKeys   - A reference to a CStringArray, which will contain the keys in the
	//                   registry under the above key.
	//-----------------------------------------------------------------------
	int EnumKeys(LPCTSTR lpszSection,CStringArray & arrayKeys);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to delete a registry key and all of its contents.
	// Parameters:
	//     lpszSection - Points to a null-terminated string that specifies the section
	//                   containing the entry.
	//     lpszKey     - Points to a null-terminated string that specifies the subkey to delete.
	// Returns:
	//     true if successful, otherwise returns false.
	//-----------------------------------------------------------------------
	bool DeleteKey(LPCTSTR lpszSection,LPCTSTR lpszKey);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to delete a registry value.
	// Parameters:
	//     lpszSection - Points to a null-terminated string that specifies the section
	//                   containing the entry.
	//     lpszKey     - Points to a null-terminated string that specifies the value to delete.
	// Returns:
	//     true if successful, otherwise returns false.
	//-----------------------------------------------------------------------
	bool DeleteValue(LPCTSTR lpszSection,LPCTSTR lpszKey);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Called to get a handle to the CWinApp's registry key.
	// Returns:
	//     An HKEY handle to the applications registry key.
	//-----------------------------------------------------------------------
	virtual HKEY GetAppRegistryKey();

	//-----------------------------------------------------------------------
	// Summary:
	//     Called to get a handle to the specified section from the applications registry key.
	// Parameters:
	//     lpszSection - Section to get the key for.
	// Returns:
	//     An HKEY handle to the specified section from the applications registry key.
	//-----------------------------------------------------------------------
	virtual HKEY GetSectionKey(LPCTSTR lpszSection);

private:
	BOOL GetProfileInfo();
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTRegistryManager::SetINIFileName(CString strINIFileName) {
	m_strINIFileName = strINIFileName;
}
AFX_INLINE CString& CXTRegistryManager::GetINIFileName() {
	return m_strINIFileName;
}

#endif // __XTREGISTRYMANAGER_H__
