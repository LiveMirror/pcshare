// ResourceManager.h: interface for the CResourceManager class.
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

#if !defined(AFX_RESOURCEMANAGER_H__53048258_89DC_4C5C_A143_3634E93B8883__INCLUDED_)
#define AFX_RESOURCEMANAGER_H__53048258_89DC_4C5C_A143_3634E93B8883__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CResourceInfo: public CObject
{
	DECLARE_SERIAL(CResourceInfo)
public:

	CResourceInfo(UINT nID = 0, BOOL bStandardResource = FALSE)
	{
		m_nID = nID;
		m_bStandardResource = bStandardResource;
	}

	virtual void Serialize(CArchive& ar);
	void DoPropExchange(CXTPPropExchange* pPX);

	UINT m_nID;
	BOOL m_bStandardResource;
	CString m_strCaption;
	BOOL m_bInUse;
};

typedef CMapStringToOb CMapResources;

class CResourceManager : public CObject
{
public:
	CResourceManager();
	virtual ~CResourceManager();

	UINT GetNextID();

	CMapResources* GetResources() {
		return &m_mapResources;
	}
	UINT GetStringID(CString str);
	CString GetStringID(UINT nID);

	CResourceInfo* Find(UINT nID);

	void Serialize(CArchive& ar);
	void DoPropExchange(CXTPPropExchange* pPX);
	void FreeAll();


public:
	UINT m_nNextID;


	CMapResources m_mapResources;

};


#endif // !defined(AFX_RESOURCEMANAGER_H__53048258_89DC_4C5C_A143_3634E93B8883__INCLUDED_)
