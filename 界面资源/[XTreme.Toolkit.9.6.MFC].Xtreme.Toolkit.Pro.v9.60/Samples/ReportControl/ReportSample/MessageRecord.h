// MessageRecord.h: interface for the CMessageRecord class.
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

#if !defined(AFX_MESSAGERECORD_H__A08F955C_1EA1_40B4_A18F_D2B7857FB244__INCLUDED_)
#define AFX_MESSAGERECORD_H__A08F955C_1EA1_40B4_A18F_D2B7857FB244__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CReportSampleView;

class CMessageRecordItemPrice : public CXTPReportRecordItemNumber
{
public:
	CMessageRecordItemPrice(double dValue);

	virtual CString GetGroupCaption(CXTPReportColumn* pColumn);
	virtual int CompareGroupCaption(CXTPReportColumn* pColumn, CXTPReportRecordItem* pItem);
};

class CMessageRecordItemCheck : public CXTPReportRecordItem
{
public:
	CMessageRecordItemCheck(BOOL bCheck);

	virtual int GetGroupCaptionID(CXTPReportColumn* pColumn);
	virtual int Compare(CXTPReportColumn* pColumn, CXTPReportRecordItem* pItem);
};

typedef enum MESSAGE_IMPORTANCE
{
	msgImportanceNormal,
	msgImportanceHigh,
	msgImportanceLow
};

class CMessageRecordItemImportance : public CXTPReportRecordItem
{
public:
	CMessageRecordItemImportance(MESSAGE_IMPORTANCE eImportance = msgImportanceNormal);

protected:
	MESSAGE_IMPORTANCE m_eImportance;   // Message importance
};

class CMessageRecordItemAttachment : public CXTPReportRecordItem
{
public:
	CMessageRecordItemAttachment(BOOL bHasAttachment);

protected:
	BOOL m_bHasAttachment;
};

class CMessageRecordItemIcon : public CXTPReportRecordItem
{
public:
	CMessageRecordItemIcon(BOOL bRead);
	virtual CString GetGroupCaption(CXTPReportColumn* pColumn);
	virtual int CompareGroupCaption(CXTPReportColumn* pColumn, CXTPReportRecordItem* pItem);
	void UpdateReadIcon();

	int Compare(CXTPReportColumn* pColumn, CXTPReportRecordItem* pItem);

public:
	BOOL m_bRead;
};


class CMessageRecordItemDate : public CXTPReportRecordItemDateTime
{
public:
	CMessageRecordItemDate(COleDateTime odtValue);
	virtual int GetGroupCaptionID(CXTPReportColumn* pColumn);
};



class CMessageRecord : public CXTPReportRecord
{
	DECLARE_DYNAMIC(CMessageRecord)
public:
	CMessageRecord(
		MESSAGE_IMPORTANCE eImportance, BOOL bChecked, int  nAttachmentBitmap,
		CString strFromName, CString strSubject,
		COleDateTime odtSent, int nMessageSize, BOOL bRead,
		double dPrice, COleDateTime odtReceived, COleDateTime odtCreated,
		CString strConversation, CString strContact, CString strMessage,
		CString strCC, CString strCategories, CString strAutoforward,
		CString strDoNotAutoarch, CString strDueBy,
		CString strPreview
		);

	virtual ~CMessageRecord();

	BOOL SetRead();

	virtual void GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pItemMetrics);

	CMessageRecordItemIcon* m_pItemIcon;
	CMessageRecordItemDate* m_pItemReceived;
	CXTPReportRecordItem* m_pItemSize;

};

#endif // !defined(AFX_MESSAGERECORD_H__A08F955C_1EA1_40B4_A18F_D2B7857FB244__INCLUDED_)
