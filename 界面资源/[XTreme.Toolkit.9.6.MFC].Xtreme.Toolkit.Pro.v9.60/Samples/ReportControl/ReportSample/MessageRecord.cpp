// MessageRecord.cpp: implementation of the CMessageRecord class.
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
#include "ReportSample.h"
#include "MessageRecord.h"
#include "ReportSampleView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// CMessageRecordItemPrice

CMessageRecordItemPrice::CMessageRecordItemPrice(double dValue)
: CXTPReportRecordItemNumber(dValue, _T("$ %.2f"))
{

}
CString CMessageRecordItemPrice::GetGroupCaption(CXTPReportColumn* /*pColumn*/)
{
	if (m_dValue > 20)
		return _T("Record Price > 20");

	if (m_dValue > 5)
		return _T("Record Price 5 - 20");

	return _T("Record Price < 5");
}

int CMessageRecordItemPrice::CompareGroupCaption(CXTPReportColumn* pColumn, CXTPReportRecordItem* pItem)
{
	return GetGroupCaption(pColumn).Compare(pItem->GetGroupCaption(pColumn));
}

//////////////////////////////////////////////////////////////////////////
// CMessageRecordItemCheck

CMessageRecordItemCheck::CMessageRecordItemCheck(BOOL bCheck)
{
	HasCheckbox(TRUE);
	SetChecked(bCheck);
}

int CMessageRecordItemCheck::GetGroupCaptionID(CXTPReportColumn* /*pColumn*/)
{
	return IsChecked()? IDS_GROUP_CHECKED_TRUE: IDS_GROUP_CHECKED_FALSE;
}

int CMessageRecordItemCheck::Compare(CXTPReportColumn* /*pColumn*/, CXTPReportRecordItem* pItem)
{
	return int(IsChecked()) - int(pItem->IsChecked());

}


//////////////////////////////////////////////////////////////////////////
// CMessageRecordItemImportance

CMessageRecordItemImportance::CMessageRecordItemImportance(MESSAGE_IMPORTANCE eImportance)
	: m_eImportance(eImportance)
{
	SetIconIndex(eImportance == msgImportanceHigh ? 6 :
								 eImportance == msgImportanceLow ? 9: -1);

	SetGroupPriority(
		m_eImportance == msgImportanceHigh? IDS_GROUP_IMPOPRTANCE_HIGH:
		m_eImportance == msgImportanceLow?  IDS_GROUP_IMPOPRTANCE_LOW: IDS_GROUP_IMPOPRTANCE_NORMAL);

	SetSortPriority(GetGroupPriority());
}


//////////////////////////////////////////////////////////////////////////
// CMessageRecordItemAttachment

CMessageRecordItemAttachment::CMessageRecordItemAttachment(BOOL bHasAttachment)
	: m_bHasAttachment(bHasAttachment)
{
	SetIconIndex(bHasAttachment ? 8 : -1);
	SetGroupPriority(m_bHasAttachment? IDS_GROUP_ATTACHMENTS_TRUE: IDS_GROUP_ATTACHMENTS_FALSE);
	SetSortPriority(GetGroupPriority());
}

//////////////////////////////////////////////////////////////////////////
// CMessageRecordItemIcon

CMessageRecordItemIcon::CMessageRecordItemIcon(BOOL bRead)
	: m_bRead(bRead)
{
	UpdateReadIcon();
}

void CMessageRecordItemIcon::UpdateReadIcon()
{
	SetIconIndex(m_bRead ? 4 : 3);

}

int CMessageRecordItemIcon::Compare(CXTPReportColumn* /*pColumn*/, CXTPReportRecordItem* pItem)
{
	return int(m_bRead) - int(((CMessageRecordItemIcon*)pItem)->m_bRead);
}


CString CMessageRecordItemIcon::GetGroupCaption(CXTPReportColumn* /*pColumn*/)
{
	if (m_bRead)
		return _T("Message status: Read");
	else
		return _T("Message status: Unread");
}

int CMessageRecordItemIcon::CompareGroupCaption(CXTPReportColumn* pColumn, CXTPReportRecordItem* pItem)
{
	return GetGroupCaption(pColumn).Compare(pItem->GetGroupCaption(pColumn));
}



//////////////////////////////////////////////////////////////////////////
// CMessageRecordItemSent

CMessageRecordItemDate::CMessageRecordItemDate(COleDateTime odtValue)
	: CXTPReportRecordItemDateTime(odtValue)
{
}

int CMessageRecordItemDate::GetGroupCaptionID(CXTPReportColumn* /*pColumn*/)
{
	COleDateTime odtNow(COleDateTime::GetCurrentTime());

	if (m_odtValue.GetYear() < odtNow.GetYear())
		return IDS_GROUP_DATE_OLDER;

	if (m_odtValue.GetMonth() < odtNow.GetMonth())
		return IDS_GROUP_DATE_THISYEAR;

	if (m_odtValue.GetDay() < odtNow.GetDay())
		return IDS_GROUP_DATE_THISMONTH;

	if (m_odtValue.m_dt <= odtNow.m_dt)
		return IDS_GROUP_DATE_TODAY;

	return -1;

}



//////////////////////////////////////////////////////////////////////
// CMessageRecord class

IMPLEMENT_DYNAMIC(CMessageRecord, CXTPReportRecord)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMessageRecord::CMessageRecord(
					MESSAGE_IMPORTANCE eImportance, BOOL bChecked, int  nAttachmentBitmap,
					CString strFromName, CString strSubject,
					COleDateTime odtReceived, int nMessageSize, BOOL bRead,
					double dPrice, COleDateTime odtSent, COleDateTime odtCreated,
					CString strConversation, CString strContact, CString strMessage,
					CString strCC, CString strCategories, CString strAutoforward,
					CString strDoNotAutoarch, CString strDueBy,
					CString strPreview)
{
	AddItem(new CMessageRecordItemImportance(eImportance));
	m_pItemIcon = (CMessageRecordItemIcon*)AddItem(new CMessageRecordItemIcon(bRead));
	AddItem(new CMessageRecordItemAttachment(nAttachmentBitmap));
	AddItem(new CXTPReportRecordItemText(strFromName));
	AddItem(new CXTPReportRecordItemText(strSubject));
	m_pItemReceived = (CMessageRecordItemDate*)AddItem(new CMessageRecordItemDate(odtReceived));
	m_pItemSize = AddItem(new CXTPReportRecordItemNumber(nMessageSize));
	AddItem(new CMessageRecordItemCheck(bChecked));
	AddItem(new CMessageRecordItemPrice(dPrice));
	AddItem(new CMessageRecordItemDate(odtCreated));
	AddItem(new CMessageRecordItemDate(odtSent));
	AddItem(new CXTPReportRecordItemText(strConversation));
	AddItem(new CXTPReportRecordItemText(strContact));
	AddItem(new CXTPReportRecordItemText(strMessage));
	AddItem(new CXTPReportRecordItemText(strCC));
	AddItem(new CXTPReportRecordItemText(strCategories));
	AddItem(new CXTPReportRecordItemText(strAutoforward));
	AddItem(new CXTPReportRecordItemText(strDoNotAutoarch));
	AddItem(new CXTPReportRecordItemText(strDueBy));


	SetPreviewItem(new CXTPReportRecordItemPreview(strPreview));

}

CMessageRecord::~CMessageRecord()
{

}

BOOL CMessageRecord::SetRead()
{
	ASSERT(m_pItemIcon);
	if (m_pItemIcon->m_bRead)
		return FALSE;

	m_pItemIcon->m_bRead = TRUE;
	m_pItemIcon->UpdateReadIcon();

	return TRUE;
}

void CMessageRecord::GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pItemMetrics)
{
	if (!m_pItemIcon->m_bRead && !pDrawArgs->pItem->IsPreviewItem())
		pItemMetrics->pFont = &pDrawArgs->pControl->GetPaintManager()->m_fontBoldText;

	CReportSampleView* pView = DYNAMIC_DOWNCAST(CReportSampleView, pDrawArgs->pControl->GetParent());

	if (pView && pView->m_bAutomaticFormating)
	{
		if ((pDrawArgs->pRow->GetIndex() % 2) && !pDrawArgs->pItem->IsPreviewItem())
		{
			pItemMetrics->clrBackground = RGB(245, 245, 245);
		}
		if (m_pItemReceived->GetGroupCaptionID(NULL) <= IDS_GROUP_DATE_THISYEAR  && !pDrawArgs->pItem->IsPreviewItem())
		{
			static CFont m_fontStriked;

			if (!m_fontStriked.GetSafeHandle())
			{
				LOGFONT lf;
				pDrawArgs->pControl->GetPaintManager()->m_fontText.GetLogFont(&lf);
				lf.lfStrikeOut = TRUE;

				m_fontStriked.CreateFontIndirect(&lf);
			}

			pItemMetrics->pFont = &m_fontStriked;
		}
		if (pDrawArgs->pItem->GetCaption(pDrawArgs->pColumn).Find(_T("Undeliverable")) >= 0)
		{
			pItemMetrics->clrForeground = RGB(0xFF, 0, 0);

		}
	}
}
