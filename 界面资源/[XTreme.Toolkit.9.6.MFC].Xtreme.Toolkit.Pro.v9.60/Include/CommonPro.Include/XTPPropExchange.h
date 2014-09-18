//////////////////////////////////////////////////////////////////////
// XTPPropExchange.h: interface for the CXTPPropExchange class.
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
#if !defined(__XTPPROPEXCHANGE_H__)
#define __XTPPROPEXCHANGE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XTPXMLHelpers.h"
#include "XTPPropExchange.inc"

#pragma warning(disable:4510)
#pragma warning(disable:4610)


class CXTPPropExchange;


//===========================================================================
// Summary:
//     CXTPPropExchangeEnumerator is standalone class. It used by CXTPPropExchange to enumerate child sections.
//
// Example:
// <code>
// extern CXTPPropExchange* pPX;
//
// CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("Icon")));
// POSITION posEnum = pEnumerator->GetPosition();
//
// while (posEnum)
// {
//     CXTPPropExchangeSection secIcon(pEnumerator->GetNext(posEnum));
// }
// </code>
// See Also: CXTPPropExchange, CXTPPropExchange::GetEnumerator
//===========================================================================
class _XTP_EXT_CLASS CXTPPropExchangeEnumerator
{
public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropExchangeEnumerator object.
	//-------------------------------------------------------------------------
	CXTPPropExchangeEnumerator(CXTPPropExchange* pContainer, LPCTSTR lpszSection);
	
	//-------------------------------------------------------------------------
	//     Destroys a CXTPPropExchangeEnumerator object, handles cleanup and de-
	//     allocation.
	//-------------------------------------------------------------------------
	virtual ~CXTPPropExchangeEnumerator();

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to init enumerable section
	// Parameters:
	//     dwCount - Count of items to be stored.
	// Returns:
	//     A POSITION value that can be used for iteration; NULL if the section is empty.
	//-------------------------------------------------------------------------
	virtual POSITION GetPosition(DWORD dwCount = 0, BOOL bCompactMode = TRUE);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to get next enumerable section to load.
	// Parameters:
	//     pos - A reference to a POSITION value returned by a previous GetNext, GetPosition, or other member function call.
	//-------------------------------------------------------------------------
	virtual CXTPPropExchange* GetNext(POSITION& pos);

protected:
	CXTPPropExchange*  m_pContainer;    // Parent section of enumerator
	CString m_strSectionName;           // Enumerator section name
	DWORD m_nCount;                     // Count of items to load.
};

//===========================================================================
// Summary:
//     CXTPPropExchange is standalone pure class.
//     The CXTPPropExchange class allows you to save a complex network of objects in a permanent binary or text form (usually disk storage) that persists after those objects are deleted. Later you can load the objects from persistent storage, reconstituting them in memory. This process of making data persistent is called “serialization.”
//
//     You can use one of derived classes:
//       * CXTPPropExchangeArchive to serialize to archive,
//       * CXTPPropExchangeRegistry to serialize to registry key,
//       * CXTPPropExchangeXMLNode to serialize to XML node.
// Example:
// <code>
// CXTPPropExchangeXMLNode px(TRUE, 0, _T("Settings"));
// if (px.LoadFromFile(m_strIniFileName))
// {
//     m_wndTaskPanel.GetGroups()->DoPropExchange(&px);
// }
// </code>
//===========================================================================
class _XTP_EXT_CLASS CXTPPropExchange
{
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropExchange object.
	//-------------------------------------------------------------------------
	CXTPPropExchange();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropExchange object, handles cleanup and de-
	//     allocation.
	//-------------------------------------------------------------------------
	virtual ~CXTPPropExchange();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method determines whether properties are being loaded to
	//     the class or stored from it.
	// Returns:
	//     Nonzero if properties are being loaded; otherwise, it is zero.
	//-----------------------------------------------------------------------
	BOOL IsLoading();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method determines whether properties are being loaded to
	//     the class or stored from it.
	// Returns:
	//     Nonzero if the properties are currently being used for storing; otherwise 0.
	//-----------------------------------------------------------------------
	BOOL IsStoring();


	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to use default values specified in PX_* functions while load process.
	// Parameters:
	//     bUseDefaultOnLoad - TRUE to use default value; FALSE to ignore it.
	// See Also: PX_String, PX_Int, PX_Bool
	//-----------------------------------------------------------------------
	void UseDefaultOnLoad(BOOL bUseDefaultOnLoad = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if CXTPPropExchange is used as root section
	// Returns:
	//     TRUE if CXTPPropExchange object it root section.
	// Remarks:
	//     Used only in CXTPPropExchangeXMLNode and CXTPPropExchangeRegistry classes
	// See Also: GetSection
	//-----------------------------------------------------------------------
	BOOL IsRootSection();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function from the DoPropExchange function to determine the version of the object that is currently being de-serialized
	// Returns:
	//     During de-serialization, the version of the object being read.
	//-----------------------------------------------------------------------
	UINT GetSchema();

	//-----------------------------------------------------------------------
	// Summary:
	//     Exchanges a property between a storage and the class.
	// Parameters:
	//     pszPropName - The name of the property being exchanged.
	//     vtProp      - A symbol specifying the type of the property being
	//                   exchanged.
	//     pvProp      - A pointer to the property's value.
	//     pvDefault   - Pointer to a default value for the property.
	// Remarks:
	//    Possible values of properties are:
	//    <TABLE>
	//         Symbol         Property Type
	//         -------------  -------------
	//         VT_I2          short
	//         VT_I4          long
	//         VT_BOOL        BOOL
	//         VT_BSTR        CString
	//         VT_CY          CY
	//         VT_R4          float
	//         VT_R8          double
	//    </TABLE>
	// Returns:
	//     Nonzero if the exchange was successful; 0 if unsuccessful.
	//-----------------------------------------------------------------------
	virtual BOOL ExchangeProp(LPCTSTR pszPropName, VARTYPE vtProp,
								void* pvProp, const void* pvDefault = NULL) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method serializes a property that stores binary large object (BLOB) data.
	// Parameters:
	//     pszPropName - The name of the property being exchanged.
	//     pData       - A pointer to a user-supplied buffer that contains the data to be serialized
	//     nBytes      - An integer that specifies the number of bytes to be read or written.
	// Returns:
	//     Nonzero if the exchange was successful; 0 if unsuccessful.
	//-----------------------------------------------------------------------
	virtual BOOL ExchangeBlobProp(LPCTSTR pszPropName, BYTE*& pData, DWORD& nBytes);

	//-----------------------------------------------------------------------
	// Summary:
	//     Exchanges a CRuntimeClass between a storage and the class.
	// Parameters:
	//     pszPropName   - The name of the property being exchanged.
	//     pClass        - CRuntimeClass class pointer.
	//     pDefaultClass - Pointer to a default runtime class for the property.
	// Returns:
	//     Nonzero if the exchange was successful; 0 if unsuccessful.
	//-----------------------------------------------------------------------
	virtual BOOL ExchangeRuntimeClass(LPCTSTR pszPropName, CRuntimeClass*& pClass, CRuntimeClass* pDefaultClass);

	//-----------------------------------------------------------------------
	// Summary:
	//     Exchanges a class instance.
	// Parameters:
	//     pObject        - Object being exchanged.
	//     pObjectRuntime - Default RUntime class of object
	// Returns:
	//     Nonzero if the exchange was successful; 0 if unsuccessful.
	//-----------------------------------------------------------------------
	virtual BOOL ExchangeObjectInstance(CObject*& pObject, CRuntimeClass* pObjectRuntime);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method create new Archive to serialize.
	// Parameters:
	//     pszPropName - The name of the property being exchanged.
	// Returns:
	//     CArchive pointer. You must call ReleaseArchive to free memory.
	// See Also: ReleaseArchive
	//-----------------------------------------------------------------------
	virtual CArchive* GetArchive(LPCTSTR pszPropName);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method releases CArchive pointer.
	// Parameters:
	//     pszPropName - The name of the property being exchanged.
	//     pArchive    - Archive pointer was previously returned by GetArchive member.
	// See Also: GetArchive
	//-----------------------------------------------------------------------
	virtual void ReleaseArchive(LPCTSTR pszPropName, CArchive* pArchive);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to serialize or initialize the state of a class’s version information
	//-------------------------------------------------------------------------
	void ExchangeSchema();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method creates new section for text base storages (CXTPPropExchangeXMLNode and CXTPPropExchangeRegistry)
	// Parameters:
	//     lpszSection - The name of the section.
	// Returns:
	//     CXTPPropExchange pointer to new section. You must delete this pointer or use CXTPPropExchangeSection to manage it.
	// Example:
	// <code>
	// CXTPPropExchangeSection pxTaskPanel(px.GetSection(_T("TaskPanel")));
	// m_wndTaskPanel.GetGroups()->DoPropExchange(&pxTaskPanel);
	// </code>
	//-----------------------------------------------------------------------
	virtual CXTPPropExchange* GetSection(LPCTSTR lpszSection) = 0;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to free section information before storing
	//-------------------------------------------------------------------------
	virtual void EmptySection();

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this member function to Write DWORD number to storage
	// Parameters:
	//     dwCount - DWORD number
	// See Also: ReadCount
	//-----------------------------------------------------------------------
	virtual void WriteCount (DWORD dwCount);

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this member function to read DWORD number from storage
	// Returns:
	//     DWORD number
	// See Also: WriteCount
	//-----------------------------------------------------------------------
	virtual DWORD ReadCount ();

	//-----------------------------------------------------------------------
	// Summary:
	//     Writes a specified number of bytes.
	// Parameters:
	//     pszPropName - The name of the property being exchanged.
	//     lpBuf       - A pointer to a user-supplied buffer that contains the data to be written.
	//     nCount      - An integer that specifies the number of bytes to be written.
	//-----------------------------------------------------------------------
	virtual void Write (LPCTSTR pszPropName, const void* lpBuf, UINT nCount);

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads a specified number of bytes.
	// Parameters:
	//     pszPropName - The name of the property being exchanged.
	//     lpBuf       - A pointer to a user-supplied buffer that is to receive the data
	//     nCount      - An unsigned integer specifying the number of bytes to be read from the archive.
	// Returns:
	//     An unsigned integer containing the number of bytes actually read.
	//-----------------------------------------------------------------------
	virtual UINT Read (LPCTSTR pszPropName, void* lpBuf, UINT nCount);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to determine if storage supported default values.
	// Returns:
	//     TRUE if storage allows default values
	//-----------------------------------------------------------------------
	virtual BOOL IsAllowDefaultValues();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to determine if storage allows exchange plain blob values.
	// Returns:
	//     TRUE if storage allows exchange plain blob values.
	//-----------------------------------------------------------------------
	virtual BOOL IsAllowBlobValues();


	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called before exchange properties.
	// Returns:
	//     TRUE if storage ready to exchange properties.
	//-----------------------------------------------------------------------
	virtual BOOL OnBeforeExchange();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to enumerate child sections.
	// Parameters:
	//     lpszSection - child section name.
	// Returns:
	//     Returns an enumerator for child sections.
	// Example:
	// <code>
	// extern CXTPPropExchange* pPX;
	//
	// CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("Icon")));
	// POSITION posEnum = pEnumerator->GetPosition();
	//
	// while (posEnum)
	// {
	//     CXTPPropExchangeSection secIcon(pEnumerator->GetNext(posEnum));
	// }
	// </code>
	// See Also: CXTPPropExchangeEnumerator
	//-----------------------------------------------------------------------
	virtual CXTPPropExchangeEnumerator* GetEnumerator(LPCTSTR lpszSection);


protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This methods is called to copy internal data from root section.
	// Parameters:
	//     pRootSection - Parent section
	//-----------------------------------------------------------------------
	void InitSection(CXTPPropExchange* pRootSection);

protected:
//{{AFX_CODEJOCK_PRIVATE
	DWORD GetSizeOfVarType(VARTYPE vt);
	BOOL IsSamePropValue(VARTYPE vtProp, const void* pv1, const void* pv2);
	BOOL CopyPropValue(VARTYPE vtProp, void* pvDest, const void * pvSrc);

	BOOL ExchangePropStringHelper(CString& strValue, VARTYPE vtProp, void* pvProp, const void* pvDefault);
	void PreformatString(CString& strValue);
//}}AFX_CODEJOCK_PRIVATE

private:
	CRuntimeClass* FindRuntimeClass(LPCSTR szClassName);

public:
	DWORD_PTR m_dwData;         // User defined 32-bit value.

protected:
	BOOL m_bLoading;            // TRUE if properties are being loaded to the class.
	UINT m_nSchema;             // Schema of object.
	BOOL m_bChildSection;       // TRUE if section is child
	BOOL m_bEmptySection;       // TRUE if EmptySection method was called.
	BOOL m_bInitialized;        // TRUE if section was initialized.
	BOOL m_bUseDefaultOnLoad;   // TRUE if default values must be used in load process.

};


//===========================================================================
// Summary:
//     CXTPPropExchangeArchive is CXTPPropExchange derived class.
//     The CXTPPropExchangeArchive class allows you to save a complex network of objects in a permanent binary form (usually disk storage) that persists after those objects are deleted. Later you can load the objects from persistent storage, reconstituting them in memory. This process of making data persistent is called “serialization.”
// Example:
// <code>
// CFile myFile("myfile", CFile::modeCreate | CFile::modeWrite);
//
// // Create a storing archive.
// CArchive arStore(&myFile, CArchive::store);
// CXTPPropExchangeArchive px(arStore);
// PX_String(&px, _T("Caption"), m_strCaption);
// </code>
//===========================================================================
class _XTP_EXT_CLASS CXTPPropExchangeArchive : public CXTPPropExchange
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropExchangeArchive object.
	// Parameters:
	//     ar - A reference to the CArchive object that is the ultimate source or destination of the persistent data.
	//-----------------------------------------------------------------------
	CXTPPropExchangeArchive(CArchive& ar);

	//-----------------------------------------------------------------------
	// Summary:
	//     Exchanges a property between a storage medium and the control.
	// Parameters:
	//     pszPropName - The name of the property being exchanged.
	//     vtProp      - A symbol specifying the type of the property being
	//                   exchanged.
	//     pvProp      - A pointer to the property's value.
	//     pvDefault   - Pointer to a default value for the property.
	// Remarks:
	//    Possible values of properties are:
	//    <TABLE>
	//         Symbol         Property Type
	//         -------------  -------------
	//         VT_I2          short
	//         VT_I4          long
	//         VT_BOOL        BOOL
	//         VT_BSTR        CString
	//         VT_CY          CY
	//         VT_R4          float
	//         VT_R8          double
	//    </TABLE>
	// Returns:
	//     Nonzero if the exchange was successful; 0 if unsuccessful.
	//-----------------------------------------------------------------------
	virtual BOOL ExchangeProp(LPCTSTR pszPropName, VARTYPE vtProp,
				void* pvProp, const void* pvDefault = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method serializes a property that stores binary large object (BLOB) data.
	// Parameters:
	//     pszPropName - The name of the property being exchanged.
	//     pData       - A pointer to a user-supplied buffer that contains the data to be serialized
	//     nBytes      - An integer that specifies the number of bytes to be read or written.
	// Returns:
	//     Nonzero if the exchange was successful; 0 if unsuccessful.
	//-----------------------------------------------------------------------
	virtual BOOL ExchangeBlobProp(LPCTSTR pszPropName, BYTE*& pData, DWORD& nBytes);

	//-----------------------------------------------------------------------
	// Summary:
	//     Exchanges a CRuntimeClass between a storage and the class.
	// Parameters:
	//     pszPropName   - The name of the property being exchanged.
	//     pClass        - CRuntimeClass class pointer.
	//     pDefaultClass - Pointer to a default runtime class for the property.
	// Returns:
	//     Nonzero if the exchange was successful; 0 if unsuccessful.
	//-----------------------------------------------------------------------
	virtual BOOL ExchangeRuntimeClass(LPCTSTR pszPropName, CRuntimeClass*& pClass, CRuntimeClass* pDefaultClass);

	//-----------------------------------------------------------------------
	// Summary:
	//     Exchanges a class instance.
	// Parameters:
	//     pObject        - Object being exchanged.
	//     pObjectRuntime - Default RUntime class of object
	// Returns:
	//     Nonzero if the exchange was successful; 0 if unsuccessful.
	//-----------------------------------------------------------------------
	virtual BOOL ExchangeObjectInstance(CObject*& pObject, CRuntimeClass* pObjectRuntime);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method create new Archive to serialize.
	// Parameters:
	//     pszPropName - The name of the property being exchanged.
	// Returns:
	//     CArchive pointer. You must call ReleaseArchive to free memory.
	// See Also: ReleaseArchive
	//-----------------------------------------------------------------------
	virtual CArchive* GetArchive(LPCTSTR pszPropName);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method releases CArchive pointer.
	// Parameters:
	//     pszPropName - The name of the property being exchanged.
	//     pArchive    - Archive pointer was previously returned by GetArchive member.
	// See Also: GetArchive
	//-----------------------------------------------------------------------
	virtual void ReleaseArchive(LPCTSTR pszPropName, CArchive* pArchive);

	//-----------------------------------------------------------------------
	// Summary:
	//     Writes a specified number of bytes.
	// Parameters:
	//     pszPropName - The name of the property being exchanged.
	//     lpBuf       - A pointer to a user-supplied buffer that contains the data to be written.
	//     nCount      - An integer that specifies the number of bytes to be written.
	//-----------------------------------------------------------------------
	virtual void Write (LPCTSTR pszPropName, const void* lpBuf, UINT nCount);

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads a specified number of bytes.
	// Parameters:
	//     pszPropName - The name of the property being exchanged.
	//     lpBuf       - A pointer to a user-supplied buffer that is to receive the data
	//     nCount      - An unsigned integer specifying the number of bytes to be read from the archive.
	// Returns:
	//     An unsigned integer containing the number of bytes actually read.
	//-----------------------------------------------------------------------
	virtual UINT Read (LPCTSTR pszPropName, void* lpBuf, UINT nCount);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to determine if storage supported default values.
	// Returns:
	//     TRUE if storage allows default values
	//-----------------------------------------------------------------------
	virtual BOOL IsAllowDefaultValues();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to determine if storage allows exchange plain blob values.
	// Returns:
	//     TRUE if storage allows exchange plain blob values.
	//-----------------------------------------------------------------------
	virtual BOOL IsAllowBlobValues();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method creates new section for text base storages (CXTPPropExchangeXMLNode and CXTPPropExchangeRegistry)
	// Parameters:
	//     lpszSection - The name of the section.
	// Returns:
	//     CXTPPropExchange pointer to new section. You must delete this pointer or use CXTPPropExchangeSection to manage it.
	// Example:
	// <code>
	// CXTPPropExchangeSection pxTaskPanel(px.GetSection(_T("TaskPanel")));
	// m_wndTaskPanel.GetGroups()->DoPropExchange(&pxTaskPanel);
	// </code>
	//-----------------------------------------------------------------------
	CXTPPropExchange* GetSection(LPCTSTR lpszSection);

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this member function to write DWORD number to storage
	// Parameters:
	//     dwCount - DWORD number
	// See Also: ReadCount
	//-----------------------------------------------------------------------
	virtual void WriteCount(DWORD dwCount);

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this member function to read DWORD number from storage
	// Returns:
	//     DWORD number
	// See Also: WriteCount
	//-----------------------------------------------------------------------
	virtual DWORD ReadCount();

protected:

//{{AFX_CODEJOCK_PRIVATE
	class CSelfArchive : public CArchive
	{
		friend class CXTPPropExchangeArchive;
	};
//}}AFX_CODEJOCK_PRIVATE


	CSelfArchive& m_ar;         // Contains the underlying CArchive for this CXTPPropExchangeArchive object.
};


//===========================================================================
// Summary:
//     CXTPPropExchangeRegistry is CXTPPropExchange derived class.
//     The CXTPPropExchangeRegistry class allows you to save a complex network of objects in a system registry that persists after those objects are deleted. Later you can load the objects from persistent storage, reconstituting them in memory. This process of making data persistent is called “serialization.”
// Example:
// <code>
// HKEY hKey = AfxGetApp()->GetAppRegistryKey();
// CXTPPropExchangeRegistry px(FALSE, hKey, szSection);
// DoPropExchange(&px);
// RegCloseKey(hKey);
// </code>
//===========================================================================
class _XTP_EXT_CLASS CXTPPropExchangeRegistry : public CXTPPropExchange
{

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs CXTPPropExchangeRegistry a object.
	// Parameters:
	//     bLoading    - A flag that specifies whether objects will be loaded from or stored.
	//     hParentKey  - Root HKEY
	//     lpszSection - Points to a null-terminated string that specifies the section containing the entry.
	//-----------------------------------------------------------------------
	CXTPPropExchangeRegistry(BOOL bLoading, HKEY hParentKey, LPCTSTR lpszSection);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropExchangeRegistry object, handles cleanup and de-
	//     allocation.
	//-------------------------------------------------------------------------
	~CXTPPropExchangeRegistry();

	//-----------------------------------------------------------------------
	// Summary:
	//     Exchanges a property between a storage medium and the control.
	// Parameters:
	//     pszPropName - The name of the property being exchanged.
	//     vtProp      - A symbol specifying the type of the property being
	//                   exchanged.
	//     pvProp      - A pointer to the property's value.
	//     pvDefault   - Pointer to a default value for the property.
	// Remarks:
	//    Possible values of properties are:
	//    <TABLE>
	//         Symbol         Property Type
	//         -------------  -------------
	//         VT_I2          short
	//         VT_I4          long
	//         VT_BOOL        BOOL
	//         VT_BSTR        CString
	//         VT_CY          CY
	//         VT_R4          float
	//         VT_R8          double
	//    </TABLE>
	// Returns:
	//     Nonzero if the exchange was successful; 0 if unsuccessful.
	//-----------------------------------------------------------------------
	virtual BOOL ExchangeProp(LPCTSTR pszPropName, VARTYPE vtProp,
				void* pvProp, const void* pvDefault = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method serializes a property that stores binary large object (BLOB) data.
	// Parameters:
	//     pszPropName - The name of the property being exchanged.
	//     pData       - A pointer to a user-supplied buffer that contains the data to be serialized
	//     nBytes      - An integer that specifies the number of bytes to be read or written.
	// Returns:
	//     Nonzero if the exchange was successful; 0 if unsuccessful.
	//-----------------------------------------------------------------------
	virtual BOOL ExchangeBlobProp(LPCTSTR pszPropName, BYTE*& pData, DWORD& nBytes);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method creates new section for text base storages (CXTPPropExchangeXMLNode and CXTPPropExchangeRegistry)
	// Parameters:
	//     lpszSection - The name of the section.
	// Returns:
	//     CXTPPropExchange pointer to new section. You must delete this pointer or use CXTPPropExchangeSection to manage it.
	// Example:
	// <code>
	// CXTPPropExchangeSection pxTaskPanel(px.GetSection(_T("TaskPanel")));
	// m_wndTaskPanel.GetGroups()->DoPropExchange(&pxTaskPanel);
	// </code>
	//-----------------------------------------------------------------------
	virtual CXTPPropExchange* GetSection(LPCTSTR lpszSection);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to free section information before storing
	//-------------------------------------------------------------------------
	virtual void EmptySection();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called before exchange properties.
	// Returns:
	//     TRUE if storage ready to exchange properties.
	//-----------------------------------------------------------------------
	virtual BOOL OnBeforeExchange();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to determine if storage allows exchange plain blob values.
	// Returns:
	//     TRUE if storage allows exchange plain blob values.
	//-----------------------------------------------------------------------
	virtual BOOL IsAllowBlobValues();


private:
	LONG DelRegTreeHelper(HKEY hParentKey, LPCTSTR strKeyName);

protected:
	HKEY m_hSectionKey;         // Current registry key
	HKEY m_hParentKey;          // Parent registry key
	CString m_strSectionName;   // Section name

};

#ifndef _XTP_EXCLUDE_XML


//===========================================================================
// Summary:
//     CXTPPropExchangeXMLNode is CXTPPropExchange derived class.
//     The CXTPPropExchangeXMLNode class allows you to save a complex network of objects in a XML file that persists after those objects are deleted. Later you can load the objects from persistent storage, reconstituting them in memory. This process of making data persistent is called “serialization.”
// Example:
// <code>
// CXTPPropExchangeXMLNode px(TRUE, 0, _T("Settings"));
// if (px.LoadFromFile(m_strIniFileName))
// {
//     m_wndTaskPanel.GetGroups()->DoPropExchange(&px);
// }
// </code>
//===========================================================================
class _XTP_EXT_CLASS CXTPPropExchangeXMLNode : public CXTPPropExchange
{
	class CXMLEnumerator;

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs CXTPPropExchangeXMLNode a object.
	// Parameters:
	//     bLoading      - A flag that specifies whether objects will be loaded from or stored.
	//     xmlParentNode - Parent node element. Can be NULL.
	//     lpszSection   - Points to a null-terminated string that specifies the section containing the entry.
	//-----------------------------------------------------------------------
	CXTPPropExchangeXMLNode(BOOL bLoading, CXTPDOMNodePtr xmlParentNode, LPCTSTR lpszSection);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Exchanges a property between a storage medium and the control.
	// Parameters:
	//     pszPropName - The name of the property being exchanged.
	//     vtProp      - A symbol specifying the type of the property being
	//                   exchanged.
	//     pvProp      - A pointer to the property's value.
	//     pvDefault   - Pointer to a default value for the property.
	// Remarks:
	//    Possible values of properties are:
	//    <TABLE>
	//         Symbol         Property Type
	//         -------------  -------------
	//         VT_I2          short
	//         VT_I4          long
	//         VT_BOOL        BOOL
	//         VT_BSTR        CString
	//         VT_CY          CY
	//         VT_R4          float
	//         VT_R8          double
	//    </TABLE>
	// Returns:
	//     Nonzero if the exchange was successful; 0 if unsuccessful.
	//-----------------------------------------------------------------------
	virtual BOOL ExchangeProp(LPCTSTR pszPropName, VARTYPE vtProp,
		void* pvProp, const void* pvDefault = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method creates new section for text base storages (CXTPPropExchangeXMLNode and CXTPPropExchangeRegistry)
	// Parameters:
	//     lpszSection - The name of the section.
	// Returns:
	//     CXTPPropExchange pointer to new section. You must delete this pointer or use CXTPPropExchangeSection to manage it.
	// Example:
	// <code>
	// CXTPPropExchangeSection pxTaskPanel(px.GetSection(_T("TaskPanel")));
	// m_wndTaskPanel.GetGroups()->DoPropExchange(&pxTaskPanel);
	// </code>
	//-----------------------------------------------------------------------
	virtual CXTPPropExchange* GetSection(LPCTSTR lpszSection);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to free section information before storing
	//-------------------------------------------------------------------------
	virtual void EmptySection();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to enumerate child sections.
	// Parameters:
	//     lpszSection - child section name.
	// Returns:
	//     Returns an enumerator for child sections.
	// Example:
	// <code>
	// extern CXTPPropExchange* pPX;
	//
	// CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("Icon")));
	// POSITION posEnum = pEnumerator->GetPosition();
	//
	// while (posEnum)
	// {
	//     CXTPPropExchangeSection secIcon(pEnumerator->GetNext(posEnum));
	// }
	// </code>
	// See Also: CXTPPropExchangeEnumerator
	//-----------------------------------------------------------------------
	virtual CXTPPropExchangeEnumerator* GetEnumerator(LPCTSTR lpszSection);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to check if property entry exists.
	// Parameters:
	//     pszPropName - The name of the property being checked.
	//-----------------------------------------------------------------------
	BOOL IsPropertyExists(LPCTSTR pszPropName);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to store data in compact mode (To use node attributes instead nodes)
	// Parameters:
	//     bCompactMode - TRUE to use compact mode (default)
	//-----------------------------------------------------------------------
	void SetCompactMode(BOOL bCompactMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     Loads the contents of an existing file into a XML.
	// Parameters:
	//     strFileName -   A CString value that contains the name of a file to be loaded into the XML
	// Returns:
	//     TRUE if successful; otherwise 0.
	//-----------------------------------------------------------------------
	BOOL LoadFromFile(CString strFileName);
	BOOL LoadFromString(BSTR lpszState); // <combine CXTPPropExchangeXMLNode::LoadFromFile@CString>

	//-----------------------------------------------------------------------
	// Summary:
	//     Loads the contents of string to XML.
	// Parameters:
	//     lpszState - A CString value that contains XML nodes
	// Returns:
	//     TRUE if successful; otherwise 0.
	//-----------------------------------------------------------------------
	BOOL LoadFromString(LPCTSTR lpszState);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the XML representation of the node and all its descendants.
	// Parameters:
	//     bFormatted - TRUE to format XML output
	// Returns:
	//     XML representation of this node and all its descendants.
	//-----------------------------------------------------------------------
	CString GetXML(BOOL bFormatted = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Saves an XML document to the specified location.
	// Parameters:
	//     strFileName - Destination file name.
	//     bFormatted  - TRUE to format XML output
	//-----------------------------------------------------------------------
	void SaveToFile(CString strFileName, BOOL bFormatted = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called before exchange properties.
	// Returns:
	//     TRUE if storage ready to exchange properties.
	//-----------------------------------------------------------------------
	virtual BOOL OnBeforeExchange();

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves profile node.
	// Parameters:
	//     strSection - Section name of profile node
	// Returns:
	//     Profile node.to be used
	//-----------------------------------------------------------------------
	CXTPDOMNodePtr GetProfileNode(CString strSection);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves entry node which the value is to be written.
	// Parameters:
	//     lpszEntry - Points to a null-terminated string that contains the entry into which the value is to be written.
	//     bCreate - TRUE to create if doesn't exist;
	// Returns:
	//     Entry node to be used
	//-----------------------------------------------------------------------
	CXTPDOMNodePtr GetEntryNode(LPCTSTR lpszEntry, BOOL bCreate);

private:
	BOOL CreateDocumentInstance();

protected:
	CXTPDOMDocumentPtr m_xmlDocPtr;         // XML Document pointer
	CXTPDOMNodePtr m_xmlParentNode;         // XML Parent node pointer
	CXTPDOMNodePtr m_xmlSectionNode;        // XML Section node pointer
	CString m_strSectionName;               // Section name
	BOOL m_bCompactMode;                    // TRUE to use compact mode.
	friend class CXMLEnumerator;
};


#endif

//===========================================================================
// Summary:
//     CXTPPropExchangeIniFile is CXTPPropExchange derived class.
//     The CXTPPropExchangeIniFile class allows you to save a complex network of objects in a ini file that persists after those objects are deleted. Later you can load the objects from persistent storage, reconstituting them in memory. This process of making data persistent is called “serialization.”
// Example:
// <code>
// CXTPPropExchangeIniFile px(TRUE, 0, _T("Settings"));
// if (px.LoadFromFile(m_strIniFileName))
// {
//     m_wndTaskPanel.GetGroups()->DoPropExchange(&px);
// }
// </code>
//===========================================================================
class _XTP_EXT_CLASS CXTPPropExchangeIniFile : public CXTPPropExchange
{

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs CXTPPropExchangeIniFile a object.
	// Parameters:
	//     bLoading           - A flag that specifies whether objects will be loaded from or stored.
	//     lpszParentSection  - Root scrtion. Can be NULL.
	//     lpszSection        - Points to a null-terminated string that specifies the section containing the entry.
	//-----------------------------------------------------------------------
	CXTPPropExchangeIniFile(BOOL bLoading, LPCTSTR lpszParentSection, LPCTSTR lpszSection);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropExchangeIniFile object, handles cleanup and de-
	//     allocation.
	//-------------------------------------------------------------------------
	~CXTPPropExchangeIniFile();

	//-----------------------------------------------------------------------
	// Summary:
	//     Exchanges a property between a storage medium and the control.
	// Parameters:
	//     pszPropName - The name of the property being exchanged.
	//     vtProp      - A symbol specifying the type of the property being
	//                   exchanged.
	//     pvProp      - A pointer to the property's value.
	//     pvDefault   - Pointer to a default value for the property.
	// Remarks:
	//    Possible values of properties are:
	//    <TABLE>
	//         Symbol         Property Type
	//         -------------  -------------
	//         VT_I2          short
	//         VT_I4          long
	//         VT_BOOL        BOOL
	//         VT_BSTR        CString
	//         VT_CY          CY
	//         VT_R4          float
	//         VT_R8          double
	//    </TABLE>
	// Returns:
	//     Nonzero if the exchange was successful; 0 if unsuccessful.
	//-----------------------------------------------------------------------
	virtual BOOL ExchangeProp(LPCTSTR pszPropName, VARTYPE vtProp,
				void* pvProp, const void* pvDefault = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method creates new section for text base storages (CXTPPropExchangeXMLNode and CXTPPropExchangeRegistry)
	// Parameters:
	//     lpszSection - The name of the section.
	// Returns:
	//     CXTPPropExchange pointer to new section. You must delete this pointer or use CXTPPropExchangeSection to manage it.
	// Example:
	// <code>
	// CXTPPropExchangeSection pxTaskPanel(px.GetSection(_T("TaskPanel")));
	// m_wndTaskPanel.GetGroups()->DoPropExchange(&pxTaskPanel);
	// </code>
	//-----------------------------------------------------------------------
	virtual CXTPPropExchange* GetSection(LPCTSTR lpszSection);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called before exchange properties.
	// Returns:
	//     TRUE if storage ready to exchange properties.
	//-----------------------------------------------------------------------
	virtual BOOL OnBeforeExchange();

	//-----------------------------------------------------------------------
	// Summary:
	//     Saves an ini file to the specified location.
	// Parameters:
	//     strFileName - Destination file name.
	//-----------------------------------------------------------------------
	void SaveToFile(CString strFileName);

	//-----------------------------------------------------------------------
	// Summary:
	//     Loads the contents of an existing ini file.
	// Parameters:
	//     strFileName -   A CString value that contains the name of a file to be loaded
	// Returns:
	//     TRUE if successful; otherwise 0.
	//-----------------------------------------------------------------------
	BOOL LoadFromFile(CString strFileName);

protected:
	CString m_strSectionName;   // Section name.
	CString m_strFileName;      // Temporary file name.
};



//===========================================================================
// Summary:
//     CXTPPropExchangeSection is standalone class. It is used to delete memory allocated by CXTPPropExchange::GetSection.
// Example:
// <code>
// CXTPPropExchangeSection pxTaskPanel(px.GetSection(_T("TaskPanel")));
// </code>
//===========================================================================
class _XTP_EXT_CLASS CXTPPropExchangeSection
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs CXTPPropExchangeSection a object.
	// Parameters:
	//     pPX -  pointer to the CXTPPropExchange object that is the ultimate source or destination of the persistent data.
	//-----------------------------------------------------------------------
	explicit CXTPPropExchangeSection(CXTPPropExchange* pPX)
		: m_pPX(pPX) {
	}

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropExchangeSection object, handles cleanup and de-
	//     allocation.
	//-------------------------------------------------------------------------
	~CXTPPropExchangeSection() {
		delete m_pPX;
	}

//{{AFX_CODEJOCK_PRIVATE
	CXTPPropExchange& operator*() const {
		return *m_pPX;
	}
	CXTPPropExchange *operator->() const {
		return m_pPX;
	}
	CXTPPropExchange *operator&() const {
		return m_pPX;
	}
//}}AFX_CODEJOCK_PRIVATE

private:
	CXTPPropExchange *m_pPX;
};

//{{AFX_CODEJOCK_PRIVATE
class CXTPPropExchangeEnumeratorPtr
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs CXTPPropExchangeEnumeratorPtr a object.
	// Parameters:
	//     pPX -  pointer to the CXTPPropExchangeEnumerator object that is the ultimate source or destination of the persistent data.
	//-----------------------------------------------------------------------
	explicit CXTPPropExchangeEnumeratorPtr(CXTPPropExchangeEnumerator* pPX)
		: m_pPX(pPX) {
	}

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropExchangeEnumeratorPtr object, handles cleanup and de-
	//     allocation.
	//-------------------------------------------------------------------------
	~CXTPPropExchangeEnumeratorPtr() {
		delete m_pPX;
	}

	CXTPPropExchangeEnumerator& operator*() const {
		return *m_pPX;
	}
	CXTPPropExchangeEnumerator *operator->() const {
		return m_pPX;
	}
	CXTPPropExchangeEnumerator *operator&() const {
		return m_pPX;
	}
private:
	CXTPPropExchangeEnumerator *m_pPX;
};
//}}AFX_CODEJOCK_PRIVATE



//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class’s DoPropExchange member function to serialize or initialize a property of type BYTE.
//     The property’s value is read from or written to the variable referenced by nValue, as appropriate. If nDefault is specified, it will be used as the property’s default value. This value is used if, for any reason, the serialization process fails.
// Parameters:
//     pPX - Pointer to the CXTPPropExchange object (typically passed as a parameter to DoPropExchange).
//     pszPropName - The name of the property being exchanged.
//     nValue - Reference to the variable where the property is stored (typically a member variable of your class).
//     nDefault - Default value for the property
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL PX_Byte(CXTPPropExchange* pPX, LPCTSTR pszPropName, BYTE& nValue, BYTE nDefault);

//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class’s DoPropExchange member function to serialize or initialize a property of type short.
//     The property’s value is read from or written to the variable referenced by sValue, as appropriate. If sDefault is specified, it will be used as the property’s default value. This value is used if, for any reason, the serialization process fails.
// Parameters:
//     pPX - Pointer to the CXTPPropExchange object (typically passed as a parameter to DoPropExchange).
//     pszPropName - The name of the property being exchanged.
//     sValue - Reference to the variable where the property is stored (typically a member variable of your class).
//     sDefault - Default value for the property
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL PX_Short(CXTPPropExchange* pPX, LPCTSTR pszPropName, short& sValue);
_XTP_EXT_CLASS BOOL AFX_CDECL PX_Short(CXTPPropExchange* pPX, LPCTSTR pszPropName, short& sValue, short sDefault);// <COMBINE PX_Short>

//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class’s DoPropExchange member function to serialize or initialize a property of type USHORT.
//     The property’s value is read from or written to the variable referenced by usValue, as appropriate. If usDefault is specified, it will be used as the property’s default value. This value is used if, for any reason, the serialization process fails.
// Parameters:
//     pPX - Pointer to the CXTPPropExchange object (typically passed as a parameter to DoPropExchange).
//     pszPropName - The name of the property being exchanged.
//     usValue - Reference to the variable where the property is stored (typically a member variable of your class).
//     usDefault - Default value for the property
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL PX_UShort(CXTPPropExchange* pPX, LPCTSTR pszPropName, USHORT& usValue);
_XTP_EXT_CLASS BOOL AFX_CDECL PX_UShort(CXTPPropExchange* pPX, LPCTSTR pszPropName, USHORT& usValue, USHORT usDefault); // <COMBINE PX_UShort>


//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class’s DoPropExchange member function to serialize or initialize a property of type long.
//     The property’s value is read from or written to the variable referenced by lValue, as appropriate. If lDefault is specified, it will be used as the property’s default value. This value is used if, for any reason, the serialization process fails.
// Parameters:
//     pPX - Pointer to the CXTPPropExchange object (typically passed as a parameter to DoPropExchange).
//     pszPropName - The name of the property being exchanged.
//     lValue - Reference to the variable where the property is stored (typically a member variable of your class).
//     lDefault - Default value for the property
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL PX_Long(CXTPPropExchange* pPX, LPCTSTR pszPropName, long& lValue);
_XTP_EXT_CLASS BOOL AFX_CDECL PX_Long(CXTPPropExchange* pPX, LPCTSTR pszPropName, long& lValue, long lDefault); // <COMBINE PX_Long>



//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class’s DoPropExchange member function to serialize or initialize a property of type DWORD.
//     The property’s value is read from or written to the variable referenced by dwValue, as appropriate. If dwDefault is specified, it will be used as the property’s default value. This value is used if, for any reason, the serialization process fails.
// Parameters:
//     pPX - Pointer to the CXTPPropExchange object (typically passed as a parameter to DoPropExchange).
//     pszPropName - The name of the property being exchanged.
//     dwValue - Reference to the variable where the property is stored (typically a member variable of your class).
//     dwDefault - Default value for the property
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL PX_DWord(CXTPPropExchange* pPX, LPCTSTR pszPropName, DWORD& dwValue, DWORD dwDefault);

//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class’s DoPropExchange member function to serialize or initialize a property of type int.
//     The property’s value is read from or written to the variable referenced by nValue, as appropriate. If nDefault is specified, it will be used as the property’s default value. This value is used if, for any reason, the serialization process fails.
// Parameters:
//     pPX - Pointer to the CXTPPropExchange object (typically passed as a parameter to DoPropExchange).
//     pszPropName - The name of the property being exchanged.
//     nValue - Reference to the variable where the property is stored (typically a member variable of your class).
//     nDefault - Default value for the property
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL PX_Int(CXTPPropExchange* pPX, LPCTSTR pszPropName, int& nValue);
_XTP_EXT_CLASS BOOL AFX_CDECL PX_Int(CXTPPropExchange* pPX, LPCTSTR pszPropName, int& nValue, int nDefault); // <COMBINE PX_Int>

//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class’s DoPropExchange member function to serialize or initialize a property of type ULONG.
//     The property’s value is read from or written to the variable referenced by ulValue, as appropriate. If ulDefault is specified, it will be used as the property’s default value. This value is used if, for any reason, the serialization process fails.
// Parameters:
//     pPX - Pointer to the CXTPPropExchange object (typically passed as a parameter to DoPropExchange).
//     pszPropName - The name of the property being exchanged.
//     ulValue - Reference to the variable where the property is stored (typically a member variable of your class).
//     ulDefault - Default value for the property
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL PX_ULong(CXTPPropExchange* pPX, LPCTSTR pszPropName, ULONG& ulValue);
_XTP_EXT_CLASS BOOL AFX_CDECL PX_ULong(CXTPPropExchange* pPX, LPCTSTR pszPropName, ULONG& ulValue, ULONG ulDefault); // <COMBINE PX_ULong>

//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class’s DoPropExchange member function to serialize or initialize a property of type BOOL.
//     The property’s value is read from or written to the variable referenced by bValue, as appropriate. If bDefault is specified, it will be used as the property’s default value. This value is used if, for any reason, the serialization process fails.
// Parameters:
//     pPX - Pointer to the CXTPPropExchange object (typically passed as a parameter to DoPropExchange).
//     pszPropName - The name of the property being exchanged.
//     bValue - Reference to the variable where the property is stored (typically a member variable of your class).
//     bDefault - Default value for the property
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL PX_Bool(CXTPPropExchange* pPX, LPCTSTR pszPropName, BOOL& bValue);
_XTP_EXT_CLASS BOOL AFX_CDECL PX_Bool(CXTPPropExchange* pPX, LPCTSTR pszPropName, BOOL& bValue, BOOL bDefault); // <COMBINE PX_Bool>

//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class’s DoPropExchange member function to serialize or initialize a character string property.
//     The property’s value is read from or written to the variable referenced by strValue, as appropriate. If lpszDefault is specified, it will be used as the property’s default value. This value is used if, for any reason, the serialization process fails.
// Parameters:
//     pPX - Pointer to the CXTPPropExchange object (typically passed as a parameter to DoPropExchange).
//     pszPropName - The name of the property being exchanged.
//     strValue - Reference to the variable where the property is stored (typically a member variable of your class).
//     lpszDefault - Default value for the property
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL PX_String(CXTPPropExchange* pPX, LPCTSTR pszPropName, CString& strValue);
_XTP_EXT_CLASS BOOL AFX_CDECL PX_String(CXTPPropExchange* pPX, LPCTSTR pszPropName, CString& strValue, LPCTSTR lpszDefault); // <COMBINE PX_String>

//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class’s DoPropExchange member function to serialize or initialize a property of type float.
//     The property’s value is read from or written to the variable referenced by floatValue, as appropriate. If floatDefault is specified, it will be used as the property’s default value. This value is used if, for any reason, the serialization process fails.
// Parameters:
//     pPX - Pointer to the CXTPPropExchange object (typically passed as a parameter to DoPropExchange).
//     pszPropName - The name of the property being exchanged.
//     floatValue - Reference to the variable where the property is stored (typically a member variable of your class).
//     floatDefault - Default value for the property
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL PX_Float(CXTPPropExchange* pPX, LPCTSTR pszPropName, float& floatValue);
_XTP_EXT_CLASS BOOL AFX_CDECL PX_Float(CXTPPropExchange* pPX, LPCTSTR pszPropName, float& floatValue, float floatDefault); // <COMBINE PX_Float>

//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class’s DoPropExchange member function to serialize or initialize a property of type double.
//     The property’s value is read from or written to the variable referenced by doubleValue, as appropriate. If doubleDefault is specified, it will be used as the property’s default value. This value is used if, for any reason, the serialization process fails.
// Parameters:
//     pPX - Pointer to the CXTPPropExchange object (typically passed as a parameter to DoPropExchange).
//     pszPropName - The name of the property being exchanged.
//     doubleValue - Reference to the variable where the property is stored (typically a member variable of your class).
//     doubleDefault - Default value for the property
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL PX_Double(CXTPPropExchange* pPX, LPCTSTR pszPropName, double& doubleValue);
_XTP_EXT_CLASS BOOL AFX_CDECL PX_Double(CXTPPropExchange* pPX, LPCTSTR pszPropName, double& doubleValue, double doubleDefault); // <COMBINE PX_Double>

//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class’s DoPropExchange member function to serialize or initialize a property of type SIZE.
//     The property’s value is read from or written to the variable referenced by szValue, as appropriate. If szDefault is specified, it will be used as the property’s default value. This value is used if, for any reason, the serialization process fails.
// Parameters:
//     pPX - Pointer to the CXTPPropExchange object (typically passed as a parameter to DoPropExchange).
//     pszPropName - The name of the property being exchanged.
//     szValue - Reference to the variable where the property is stored (typically a member variable of your class).
//     szDefault - Default value for the property
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL PX_Size(CXTPPropExchange* pPX, LPCTSTR pszPropName, SIZE& szValue, SIZE szDefault);

//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class’s DoPropExchange member function to serialize or initialize a property of type RECT.
//     The property’s value is read from or written to the variable referenced by rcValue, as appropriate. If rcDefault is specified, it will be used as the property’s default value. This value is used if, for any reason, the serialization process fails.
// Parameters:
//     pPX - Pointer to the CXTPPropExchange object (typically passed as a parameter to DoPropExchange).
//     pszPropName - The name of the property being exchanged.
//     rcValue - Reference to the variable where the property is stored (typically a member variable of your class).
//     rcDefault - Default value for the property
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL PX_Rect(CXTPPropExchange* pPX, LPCTSTR pszPropName, RECT& rcValue, RECT rcDefault);

//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class’s DoPropExchange member function to serialize or initialize a property of type POINT.
//     The property’s value is read from or written to the variable referenced by ptValue, as appropriate. If ptDefault is specified, it will be used as the property’s default value. This value is used if, for any reason, the serialization process fails.
// Parameters:
//     pPX - Pointer to the CXTPPropExchange object (typically passed as a parameter to DoPropExchange).
//     pszPropName - The name of the property being exchanged.
//     ptValue - Reference to the variable where the property is stored (typically a member variable of your class).
//     ptDefault - Default value for the property
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL PX_Point(CXTPPropExchange* pPX, LPCTSTR pszPropName, POINT& ptValue, POINT ptDefault);

//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class’s DoPropExchange member function to serialize or initialize a property of type CRuntimeClass.
//     The property’s value is read from or written to the variable referenced by pClass, as appropriate. If pDefaultClass is specified, it will be used as the property’s default value. This value is used if, for any reason, the serialization process fails.
// Parameters:
//     pPX - Pointer to the CXTPPropExchange object (typically passed as a parameter to DoPropExchange).
//     pszPropName - The name of the property being exchanged.
//     pClass - Reference to the variable where the property is stored (typically a member variable of your class).
//     pDefaultClass - Default value for the property
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL PX_RuntimeClass(CXTPPropExchange* pPX, LPCTSTR pszPropName, CRuntimeClass*& pClass, CRuntimeClass* pDefaultClass);

//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class’s DoPropExchange member function to serialize object.
// Parameters:
//     pPX - Pointer to the CXTPPropExchange object (typically passed as a parameter to DoPropExchange).
//     pszPropName - The name of the property being exchanged.
//     pObject - Object you need to serialize
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL PX_Serialize(CXTPPropExchange* pPX, LPCTSTR pszPropName, CObject* pObject);


//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class’s DoPropExchange member function to serialize BYTE array.
// Parameters:
//     pPX - Pointer to the CXTPPropExchange object (typically passed as a parameter to DoPropExchange).
//     pszPropName - The name of the property being exchanged.
//     pData - Byte array
//     nBytes - Size of Byte array.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS void AFX_CDECL PX_Blob(CXTPPropExchange* pPX, LPCTSTR pszPropName, BYTE*& pData, DWORD& nBytes);


//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class’s DoPropExchange member function to serialize or initialize a enum property.
//     The property’s value is read from or written to the variable referenced by nValue, as appropriate. If nDefault is specified, it will be used as the property’s default value. This value is used if, for any reason, the serialization process fails.
// Parameters:
//     pPX - Pointer to the CXTPPropExchange object (typically passed as a parameter to DoPropExchange).
//     pszPropName - The name of the property being exchanged.
//     nValue - Reference to the variable where the property is stored (typically a member variable of your class).
//     nDefault - Default value for the property
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
template <class TEnum>
AFX_INLINE BOOL AFX_CDECL PX_Enum(CXTPPropExchange* pPX, LPCTSTR pszPropName, TEnum& nValue, TEnum nDefault) {
	ASSERT(sizeof(sizeof(TEnum) == sizeof(int)));
	return PX_Int(pPX, pszPropName, (int&)nValue, (int)nDefault);
}


//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class’s DoPropExchange member function to serialize any class having DoPropExchange method
// Parameters:
//     pPX - Pointer to the CXTPPropExchange object (typically passed as a parameter to DoPropExchange).
//     pszPropName - The name of the property being exchanged.
//     pObject - Reference to the object.
//     pObjectRuntime - Default runtime class  of object
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
template <class TObject>
AFX_INLINE BOOL AFX_CDECL PX_Object(CXTPPropExchange* pPX, LPCTSTR pszPropName, TObject*& pObject, CRuntimeClass* pObjectRuntime)
{
	CXTPPropExchangeSection secObject(pPX->GetSection(pszPropName));
	if (!secObject->ExchangeObjectInstance((CObject*&)pObject, pObjectRuntime))
		return FALSE;
	pObject->DoPropExchange(&secObject);
	return TRUE;
}
template <class TObject>
AFX_INLINE BOOL AFX_CDECL PX_Object(CXTPPropExchange* pPX, TObject*& pObject, CRuntimeClass* pObjectRuntime)
{
	if (!pPX->ExchangeObjectInstance((CObject*&)pObject, pObjectRuntime))
		return FALSE;
	pObject->DoPropExchange(pPX);
	return TRUE;
}


//---------------------------------------------------------------------------

AFX_INLINE BOOL CXTPPropExchange::OnBeforeExchange() {
	m_bInitialized = TRUE;
	return TRUE;
}
AFX_INLINE void CXTPPropExchange::UseDefaultOnLoad(BOOL bUseDefaultOnLoad) {
	m_bUseDefaultOnLoad = bUseDefaultOnLoad;
}

AFX_INLINE BOOL CXTPPropExchange::IsLoading() {
	return m_bLoading;
}
AFX_INLINE BOOL CXTPPropExchange::IsStoring() {
	return !m_bLoading;
}
AFX_INLINE UINT CXTPPropExchange::GetSchema() {
	return m_nSchema;
}
AFX_INLINE BOOL CXTPPropExchange::IsRootSection() {
	return !m_bChildSection;
}
AFX_INLINE BOOL CXTPPropExchange::IsAllowDefaultValues() {
	return TRUE;
}
AFX_INLINE BOOL CXTPPropExchange::IsAllowBlobValues() {
	return FALSE;
}
AFX_INLINE BOOL CXTPPropExchangeArchive::IsAllowDefaultValues() {
	return FALSE;
}
AFX_INLINE BOOL CXTPPropExchangeArchive::IsAllowBlobValues() {
	return TRUE;
}

AFX_INLINE BOOL CXTPPropExchangeRegistry::IsAllowBlobValues() {
	return TRUE;
}

#endif // #if !defined(__XTPPROPEXCHANGE_H__)
