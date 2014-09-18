// XTThemeManager.cpp: implementation of the CXTThemeManager class.
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

#include "stdafx.h"
#include "XTThemeManager.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CProcessLocal<CXTThemeManager> CXTThemeManager::m_sThemeManager;


//////////////////////////////////////////////////////////////////////////
// CXTThemeManagerStyle

CXTThemeManagerStyle::CXTThemeManagerStyle()
{
	m_pFactory = 0;
}

CXTThemeManagerStyle::~CXTThemeManagerStyle()
{

}

void CXTThemeManagerStyle::RefreshMetrics()
{

}

BOOL CXTThemeManagerStyle::IsOfficeTheme() {
	return m_enumTheme != xtThemeDefault;
}


//////////////////////////////////////////////////////////////////////////
// CXTThemeManagerStyleFactory

IMPLEMENT_DYNCREATE(CXTThemeManagerStyleFactory, CObject);

CXTThemeManagerStyleFactory::CXTThemeManagerStyleFactory()
{
	m_pTheme = 0;

	m_pNextFactory = NULL;
	XTThemeManager()->m_factoryList.AddHead(this);

	m_hostList.Construct(offsetof(CXTThemeManagerStyleHost, m_pNextHost));

}

CXTThemeManagerStyleFactory::~CXTThemeManagerStyleFactory()
{
	if (m_pTheme)
	{
		delete m_pTheme;
	}

	XTThemeManager()->m_factoryList.Remove(this);

	m_hostList.RemoveAll();
}

CXTThemeManagerStyle* CXTThemeManagerStyleFactory::CreateTheme(XTThemeStyle /*theme*/)
{
	ASSERT(FALSE);
	return NULL;
}

void CXTThemeManagerStyleFactory::SetTheme(CXTThemeManagerStyle* pTheme)
{
	ASSERT(pTheme);

	if (m_pTheme)
	{
		delete m_pTheme;
	}

	m_pTheme = pTheme;

	m_pTheme->m_pFactory = this;
	m_pTheme->RefreshMetrics();

	for (CXTThemeManagerStyleHost* pHost = m_hostList.GetHead();
			pHost != NULL; pHost = pHost->m_pNextHost)
	{
		pHost->OnThemeChanged();
	}
}

void CXTThemeManagerStyleFactory::SetTheme(XTThemeStyle theme)
{
	SetTheme(CreateTheme(theme));
}

CXTThemeManagerStyle* CXTThemeManagerStyleFactory::GetSafeTheme()
{
	if (m_pTheme == 0)
	{
		m_pTheme = CreateTheme(XTThemeManager()->m_enumTheme);

		m_pTheme->m_pFactory = this;
		m_pTheme->RefreshMetrics();
	}
	return m_pTheme;
}

void CXTThemeManagerStyleFactory::RefreshMetrics()
{
	if (m_pTheme)
	{
		m_pTheme->RefreshMetrics();
	}
}

//////////////////////////////////////////////////////////////////////////
// CXTThemeManagerStyleHost

CXTThemeManagerStyleHost::CXTThemeManagerStyleHost(CRuntimeClass* pThemeFactoryClass)
{
	m_bAutoDeleteCustomFactory = FALSE;

	ASSERT(pThemeFactoryClass);
	ASSERT(pThemeFactoryClass->IsDerivedFrom(RUNTIME_CLASS(CXTThemeManagerStyleFactory)));

	m_pCustomFactory = NULL;
	m_pDefaultFactory = XTThemeManager()->GetDefaultThemeFactory(pThemeFactoryClass);

	m_pThemeFactoryClass = pThemeFactoryClass;

	m_pNextHost = NULL;
	GetSafeThemeFactory()->m_hostList.AddHead(this);
}

CXTThemeManagerStyleHost::~CXTThemeManagerStyleHost()
{
	GetSafeThemeFactory()->m_hostList.Remove(this);

	if (m_bAutoDeleteCustomFactory && m_pCustomFactory)
	{
		delete m_pCustomFactory;
	}
}

CXTThemeManagerStyleFactory* CXTThemeManagerStyleHost::CreateThemeFactory()
{
	return (CXTThemeManagerStyleFactory*)m_pThemeFactoryClass->CreateObject();
}

CXTThemeManagerStyleFactory* CXTThemeManagerStyleHost::GetSafeThemeFactory() const
{
	if (m_pCustomFactory != NULL)
		return m_pCustomFactory;

	return m_pDefaultFactory;
}


CXTThemeManagerStyle* CXTThemeManagerStyleHost::GetSafeTheme() const
{
	CXTThemeManagerStyleFactory* pFactory = GetSafeThemeFactory();
	ASSERT(pFactory);
	return pFactory->GetSafeTheme();
}

void CXTThemeManagerStyleHost::SetTheme (XTThemeStyle theme)
{
	SetTheme(m_pDefaultFactory->CreateTheme(theme));
}

void CXTThemeManagerStyleHost::SetTheme (CXTThemeManagerStyle* pTheme)
{
	if (m_pCustomFactory == NULL && pTheme == NULL)
		return;

	GetSafeThemeFactory()->m_hostList.Remove(this);

	if (m_pCustomFactory && m_bAutoDeleteCustomFactory)
	{
		delete m_pCustomFactory;
	}
	if (pTheme != NULL)
	{
		m_pCustomFactory = CreateThemeFactory();
		m_pCustomFactory->SetTheme(pTheme);
		m_bAutoDeleteCustomFactory = TRUE;
	}
	else
	{
		m_pCustomFactory = NULL;
		m_bAutoDeleteCustomFactory = FALSE;
	}

	OnThemeChanged();
	GetSafeThemeFactory()->m_hostList.AddHead(this);
}

void CXTThemeManagerStyleHost::AtachThemeFactory(CXTThemeManagerStyleFactory* pFactory)
{
	GetSafeThemeFactory()->m_hostList.Remove(this);

	if (m_pCustomFactory && m_bAutoDeleteCustomFactory)
	{
		delete m_pCustomFactory;
	}
	m_pCustomFactory = pFactory;
	m_bAutoDeleteCustomFactory = FALSE;

	OnThemeChanged();
	GetSafeThemeFactory()->m_hostList.AddHead(this);
}

void CXTThemeManagerStyleHost::OnThemeChanged()
{
	// Can be overridden by subclass
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTThemeManager::CXTThemeManager()
{
	m_factoryList.Construct(offsetof(CXTThemeManagerStyleFactory, m_pNextFactory));
	m_enumTheme = xtThemeDefault;
}

CXTThemeManager::~CXTThemeManager()
{
	POSITION pos = m_mapDefaultFactories.GetStartPosition();
	void* pThemeFactoryClass;
	CXTThemeManagerStyleFactory* pFactory = 0;
	while(pos)
	{
		m_mapDefaultFactories.GetNextAssoc(pos, pThemeFactoryClass, (void *&)pFactory);
		delete pFactory;
	}

	ASSERT(m_factoryList.m_pHead == 0);
	m_factoryList.RemoveAll();
}

CXTThemeManagerStyleFactory* CXTThemeManager::GetDefaultThemeFactory(CRuntimeClass* pThemeFactoryClass)
{
	ASSERT(pThemeFactoryClass);
	ASSERT(pThemeFactoryClass->IsDerivedFrom(RUNTIME_CLASS(CXTThemeManagerStyleFactory)));

	CXTThemeManagerStyleFactory* pFactory = 0;

	if (!m_mapDefaultFactories.Lookup(pThemeFactoryClass, (void *&)pFactory))
	{
		pFactory = (CXTThemeManagerStyleFactory*)pThemeFactoryClass->CreateObject();
		m_mapDefaultFactories.SetAt(pThemeFactoryClass, pFactory);
	}

	return pFactory;
}

void CXTThemeManager::SetTheme(XTThemeStyle eTheme)
{
	if (m_enumTheme == eTheme)
		return;

	m_enumTheme = eTheme;

	POSITION pos = m_mapDefaultFactories.GetStartPosition();
	void* pThemeFactoryClass;
	CXTThemeManagerStyleFactory* pFactory = 0;
	while(pos)
	{
		m_mapDefaultFactories.GetNextAssoc(pos, pThemeFactoryClass, (void *&)pFactory);

		pFactory->SetTheme(eTheme);
	}
}

void CXTThemeManager::RefreshMetrics()
{
	for (CXTThemeManagerStyleFactory* pFactory = m_factoryList.GetHead();
			pFactory != NULL; pFactory = pFactory->m_pNextFactory)
	{
		pFactory->RefreshMetrics();
	}
}

_XT_EXT_CLASS CXTThemeManager* AFX_CDECL XTThemeManager()
{
	return CXTThemeManager::m_sThemeManager.GetData();
}
