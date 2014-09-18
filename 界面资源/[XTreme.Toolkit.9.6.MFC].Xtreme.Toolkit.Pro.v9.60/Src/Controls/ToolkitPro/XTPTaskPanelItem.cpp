// XTPTaskPanelItem.cpp : implementation of the CXTPTaskPanelItem class.
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
#include "XTPImageManager.h"
#include "XTPVC80Helpers.h"
#include "XTPPropExchange.h"

#include "XTPTaskPanelItem.h"
#include "XTPTaskPanel.h"
#include "XTPTaskPanelItems.h"
#include "XTPTaskPanelPaintManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPTaskPanelItem

IMPLEMENT_DYNCREATE(CXTPTaskPanelItem, CCmdTarget)

CXTPTaskPanelItem::CXTPTaskPanelItem()
{
	m_pPanel = NULL;
	m_pItems = NULL;

	m_nIconIndex = -1;
	m_nID = 0;

	m_bEnabled = TRUE;
	m_typeItem = xtpTaskItemTypeLink;

	m_bAllowDrop = TRUE;
	m_nAllowDrag = xtpTaskItemAllowDragDefault;

	m_rcText.SetRectEmpty();
	m_nTextFormat = 0;


	EnableAutomation();

}

CXTPTaskPanelItem::~CXTPTaskPanelItem()
{
}

BOOL CXTPTaskPanelItem::IsItemHot()
{
	return m_bEnabled && m_pPanel && m_pPanel->GetHotItem() == this;
}

BOOL CXTPTaskPanelItem::IsItemPressed()
{
	return m_pPanel && m_pPanel->m_pItemPressed == this;
}

BOOL CXTPTaskPanelItem::IsItemDragging()
{
	return m_pPanel && m_pPanel->m_pItemDragging == this;
}

BOOL CXTPTaskPanelItem::IsItemDragOver()
{
	return m_pPanel && m_pPanel->m_pItemDragOver == this;
}

BOOL CXTPTaskPanelItem::IsItemFocused()
{
	return m_pPanel && m_bEnabled && m_pPanel->GetFocusedItem() == this;
}

BOOL CXTPTaskPanelItem::IsAcceptFocus()
{
	return (GetType() == xtpTaskItemTypeGroup || GetType() == xtpTaskItemTypeLink) && m_bEnabled;
}

void  CXTPTaskPanelItem::SetEnabled(BOOL bEnabled)
{
	if (m_bEnabled != bEnabled)
	{
		m_bEnabled = bEnabled;
		RedrawPanel();
	}
}
BOOL CXTPTaskPanelItem::GetEnabled()
{
	return m_bEnabled;
}
UINT CXTPTaskPanelItem::GetID()
{
	return m_nID;
}

XTPTaskPanelItemType CXTPTaskPanelItem::GetType()
{
	return m_typeItem;
}

int CXTPTaskPanelItem::GetIconIndex()
{
	return m_nIconIndex;
}



void CXTPTaskPanelItem::SetID(UINT nID)
{
	m_nID = nID;

	CString strPrompt;
	if (strPrompt.LoadString(nID))
	{
		if (strPrompt.Find('\n') != -1)
		{
			AfxExtractSubString(m_strTooltip, strPrompt, 1);
			AfxExtractSubString(m_strCaption, strPrompt, 0);
		}
		else
		{
			m_strCaption = strPrompt;
		}
	}
}

void CXTPTaskPanelItem::SetCaption(LPCTSTR lpstrCaption)
{
	if (m_strCaption != lpstrCaption)
	{
		m_strCaption = lpstrCaption;
		RepositionPanel();
	}
}
void CXTPTaskPanelItem::SetTooltip(LPCTSTR lpstrTooltip)
{
	m_strTooltip = lpstrTooltip;
}
void CXTPTaskPanelItem::SetType(XTPTaskPanelItemType typeItem)
{
	m_typeItem = typeItem;
	RepositionPanel();
}
void CXTPTaskPanelItem::SetIconIndex(int nIndex)
{
	if (m_nIconIndex != nIndex)
	{
		m_nIconIndex = nIndex;
		RedrawPanel();
	}
}

CXTPTaskPanelPaintManager* CXTPTaskPanelItem::GetPaintManager()
{
	ASSERT(m_pPanel);
	return m_pPanel->GetPaintManager();
}

void CXTPTaskPanelItem::RedrawPanel()
{
	if (m_pPanel && m_pPanel->GetSafeHwnd() && (m_pPanel->m_nLockRedraw == 0))
		m_pPanel->Invalidate(FALSE);
}

void CXTPTaskPanelItem::RepositionPanel()
{
	if (m_pPanel) m_pPanel->Reposition();
}

CXTPImageManagerIcon* CXTPTaskPanelItem::GetImage(int nWidth)
{
	return m_pPanel->GetImageManager()->GetImage(GetIconIndex(), nWidth);
}

void CXTPTaskPanelItem::DoPropExchange(CXTPPropExchange* pPX)
{
	if (pPX->IsLoading())
		m_pPanel = (CXTPTaskPanel*)pPX->m_dwData;

	PX_String(pPX, _T("Caption"), m_strCaption, _T(""));
	PX_String(pPX, _T("Tooltip"), m_strTooltip, _T(""));
	PX_Long(pPX, _T("Id"), (long&)m_nID, 0);
	PX_Enum(pPX, _T("Type"), m_typeItem, xtpTaskItemTypeLink);
	PX_Int(pPX, _T("IconIndex"), m_nIconIndex, -1);
	PX_Bool(pPX, _T("Enabled"), m_bEnabled, TRUE);
	PX_Long(pPX, _T("AllowDrag"), m_nAllowDrag, xtpTaskItemAllowDragDefault);
	PX_Bool(pPX, _T("AllowDrop"), m_bAllowDrop, TRUE);

}

HGLOBAL CXTPTaskPanelItem::CacheGlobalData()
{
	try
	{
		CSharedFile sharedFile;
		CArchive ar (&sharedFile, CArchive::store);

		CRuntimeClass* pClass = GetRuntimeClass ();
		ASSERT (pClass != NULL);

		ar.WriteClass (pClass);

		CXTPPropExchangeArchive px(ar);
		DoPropExchange(&px);

		ar.Close();

		return sharedFile.Detach();
	}
	catch (COleException* pEx)
	{
		TRACE(_T("CXTPTaskPanelItem::PrepareDrag. OLE exception: %x\r\n"),
			pEx->m_sc);
		pEx->Delete ();
	}
	catch (CArchiveException* pEx)
	{
		TRACE(_T("CXTPTaskPanelItem::PrepareDrag. Archive exception\r\n"));
		pEx->Delete ();
	}

	return NULL;
}

BOOL CXTPTaskPanelItem::PrepareDrag (COleDataSource& srcItem, BOOL bCacheTextData)
{
	HGLOBAL hGlobal = CacheGlobalData();

	if (!hGlobal)
		return FALSE;

	srcItem.CacheGlobalData (CXTPTaskPanel::GetClipboardFormat(), hGlobal);

#ifndef  _XTP_EXCLUDE_XML
	if (!bCacheTextData)
		return TRUE;

	CXTPPropExchangeXMLNode px(FALSE, 0, _T("Item"));
	if (!px.OnBeforeExchange())
		return TRUE; 

	DoPropExchange(&px);
	CString strText = px.GetXML();

	HGLOBAL hGlobalBuff = ::GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE, strText.GetLength() + 1);

	CHAR* szBuffer = (CHAR*)::GlobalLock(hGlobalBuff);
	WCSTOMBS_S(szBuffer, strText, strText.GetLength() + 1);
	::GlobalUnlock(hGlobalBuff);

	srcItem.CacheGlobalData (CF_TEXT, hGlobalBuff);
#endif


	return TRUE;
}

CXTPTaskPanelItem* CXTPTaskPanelItem::CreateFromOleData(COleDataObject* pDataObject)
{
	ASSERT (pDataObject != NULL);

	CXTPTaskPanelItem* pItem = NULL;

	try
	{
		CFile* pFile = pDataObject->GetFileData(CXTPTaskPanel::GetClipboardFormat());
		if (!pFile)
			return NULL;

		CArchive ar (pFile, CArchive::load);

		CRuntimeClass* pClass = ar.ReadClass ();

		if (pClass != NULL)
		{
			pItem = (CXTPTaskPanelItem*) pClass->CreateObject();

			if (pItem != NULL)
			{
				CXTPPropExchangeArchive px(ar);
				pItem->DoPropExchange(&px);
			}
		}

		ar.Close ();
		delete pFile;

		return pItem;
	}
	catch (COleException* pEx)
	{
		TRACE(_T("CXTPTaskPanelItem::CreateFromOleData. OLE exception: %x\r\n"),
			pEx->m_sc);
		pEx->Delete ();
	}
	catch (CArchiveException* pEx)
	{
		TRACE(_T("CXTPTaskPanelItem::CreateFromOleData. Archive exception\r\n"));
		pEx->Delete ();
	}
	catch (CNotSupportedException *pEx)
	{
		TRACE(_T("CXTPTaskPanelItem::CreateFromOleData. \"Not Supported\" exception\r\n"));
		pEx->Delete ();
	}

	if (pItem != NULL)
	{
		delete pItem;
	}

	return NULL;
}

void CXTPTaskPanelItem::CopyToClipboard()
{
	if (!m_pPanel->OpenClipboard())
		return;

	if (!::EmptyClipboard())
	{
		::CloseClipboard ();
		return;
	}

	HGLOBAL hGlobal = CacheGlobalData();

	if (hGlobal)
	{
		SetClipboardData(CXTPTaskPanel::GetClipboardFormat(), hGlobal);
	}

	::CloseClipboard ();
}

CXTPTaskPanelItem* CXTPTaskPanelItem::PasteFromClipboard()
{
	COleDataObject data;

	if (!data.AttachClipboard ())
	{
		return 0;
	}

	if (!data.IsDataAvailable (CXTPTaskPanel::GetClipboardFormat()))
		return 0;

	return CXTPTaskPanelItem::CreateFromOleData(&data);
}

void CXTPTaskPanelItem::Remove()
{
	m_pItems->Remove(this);
}

int CXTPTaskPanelItem::GetIndex()
{
	return m_pItems->GetIndex(this);
}

BOOL CXTPTaskPanelItem::IsAllowDrop()
{
	return m_bAllowDrop && ((CXTPTaskPanelItem*)GetItemGroup())->m_bAllowDrop &&
		m_pPanel && m_pPanel->m_bAllowDrop;
}
void CXTPTaskPanelItem::AllowDrop(BOOL bAllowDrop)
{
	m_bAllowDrop = bAllowDrop;
}

long CXTPTaskPanelItem::IsAllowDrag()
{
	if (m_nAllowDrag != xtpTaskItemAllowDragDefault)
		return m_nAllowDrag;

	CXTPTaskPanelItem* pGroup = (CXTPTaskPanelItem*)GetItemGroup();
	if (pGroup && pGroup->m_nAllowDrag != xtpTaskItemAllowDragDefault)
		return pGroup->m_nAllowDrag;

	return !m_pPanel || (m_pPanel->m_nAllowDrag == xtpTaskItemAllowDragDefault)? xtpTaskItemAllowDrag:
		m_pPanel->m_nAllowDrag;
}

void CXTPTaskPanelItem::AllowDrag(long nAllowDrag /* = xtpTaskItemAllowDragDefault */)
{
	m_nAllowDrag = nAllowDrag;
}

CFont* CXTPTaskPanelItem::GetItemFont()
{
	return m_pPanel->GetPaintManager()->GetItemFont(this);
}

void CXTPTaskPanelItem::SetTextRect(CDC* pDC, CRect rc, UINT nFormat)
{
	CRect rcText(rc.left, 0, rc.right, 0);

	CString strCaption = GetCaption();
	pDC->DrawText(strCaption.IsEmpty()? _T(" "): strCaption, rcText, (nFormat|DT_CALCRECT) & (~(DT_END_ELLIPSIS|DT_VCENTER)));

	int nDelta = rc.Height() - rcText.Height();
	int nOffset = nFormat & DT_VCENTER? nDelta/2: 0;

	rc.DeflateRect(0, nOffset, 0, nDelta - nOffset);

	m_nTextFormat = nFormat;
	m_rcText = rc;
}

CRect CXTPTaskPanelItem::GetTextRect(UINT* pFormat)
{
	if (pFormat)
		*pFormat = m_nTextFormat;

	return m_rcText;
}

CRect CXTPTaskPanelItem::GetHitTestRect()
{
	return CRect(0, 0, 0, 0);
}

BOOL CXTPTaskPanelItem::IsGroup()
{
	return GetType() == xtpTaskItemTypeGroup;
}

CXTPTaskPanelGroup* CXTPTaskPanelItem::GetItemGroup()
{
	return (CXTPTaskPanelGroup*)this;
}

