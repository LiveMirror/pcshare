// XTPPropertyGridItemExt.cpp
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

#include "StdAfx.h"
#include <math.h>
#include "XTPVC80Helpers.h"
#include "XTPVC50Helpers.h"

#include "XTPPropertyGridInplaceEdit.h"
#include "XTPPropertyGridInplaceButton.h"
#include "XTPPropertyGridInplaceList.h"
#include "XTPPropertyGridItem.h"
#include "XTPPropertyGridItemExt.h"
#include "XTPPropertyGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CXTPPropertyGridInplaceMonthCal, CWnd)
	ON_NOTIFY_REFLECT(MCN_SELECT, OnSelect)
END_MESSAGE_MAP()

void CXTPPropertyGridInplaceMonthCal::OnSelect(NMHDR*, LRESULT*)
{
	OnAccept();
}
void CXTPPropertyGridInplaceMonthCal::OnAccept()
{
	SYSTEMTIME sysTime;
	::SendMessage(m_hWnd, MCM_GETCURSEL, 0, (LPARAM) &sysTime);

	sysTime.wHour = sysTime.wMinute = sysTime.wSecond = sysTime.wMilliseconds = 0;

	COleDateTime dtSelected(sysTime);

	CString str = dtSelected.Format(m_pItem->m_strFormat);

	CXTPPropertyGridItemDate* pItem = m_pItem;
	DestroyWindow();

	pItem->OnValueChanged(str);
}

void CXTPPropertyGridInplaceMonthCal::PostNcDestroy()
{
	delete this;
}

BOOL CXTPPropertyGridInplaceMonthCal::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if( message == WM_KILLFOCUS)
	{
		CWnd* pWnd = CWnd::FromHandle((HWND)wParam);
		if (pWnd && IsChild(pWnd))
		{
			return CWnd::OnWndMsg(message, wParam, lParam, pResult);
		}

		DestroyWindow();
		return TRUE;
	}
	if( message == WM_KEYDOWN && wParam == VK_ESCAPE )
	{
		DestroyWindow();
		return TRUE;
	}
	if( message == WM_KEYDOWN && wParam == VK_RETURN )
	{
		OnAccept();
		return TRUE;
	}

	return CWnd::OnWndMsg(message, wParam, lParam, pResult);
}

CXTPPropertyGridItemDate::CXTPPropertyGridItemDate(CString strCaption, COleDateTime& oleDate)
	: CXTPPropertyGridItem(strCaption)
{
	Init(oleDate);
}

CXTPPropertyGridItemDate::CXTPPropertyGridItemDate(UINT nID, COleDateTime& oleDate)
	: CXTPPropertyGridItem(nID)
{
	Init(oleDate);
}

void CXTPPropertyGridItemDate::Init(COleDateTime& oleDate)
{
	m_nFlags = xtpGridItemHasComboButton|xtpGridItemHasEdit;
	m_strFormat = _T("%d/%m/%Y");
	m_strValue = _T("00/00/0000");
	SetMask(_T("00/00/0000"), _T("__/__/____"));

	SetDate(oleDate);

	EnableAutomation();
}

BOOL CXTPPropertyGridItemDate::ParseDateTime(COleDateTime& dt, CString strValue)
{
	SYSTEMTIME sysTime;
	ZeroMemory(&sysTime, sizeof(SYSTEMTIME));

	WORD* ptrDate[3] = {0, 0, 0};
	int nResult[3] = {0, 0, 0};

	CString strFormat(m_strFormat);
	strFormat.MakeLower();

	int nIndex = -1, i = 0;

	for (i = 0; i < 3; i ++)
	{
		nIndex = FIND_S(strFormat, _T('%'), nIndex + 1);

		if (nIndex == -1 || nIndex == strFormat.GetLength() - 1)
			return FALSE;

		switch (strFormat[nIndex + 1])
		{
			case _T('d'): ptrDate[i] = &sysTime.wDay; break;
			case _T('y'): ptrDate[i] = &sysTime.wYear; break;
			case _T('m'): ptrDate[i] = &sysTime.wMonth; break;
			default: return FALSE;
		}

		strFormat.SetAt(nIndex + 1, _T('d'));
	}

	if (SCANF_S(strValue, strFormat, &nResult[0], &nResult[1], &nResult[2]) != 3)
		return FALSE;

	for (i = 0; i < 3; i++)
	{
		if (!ptrDate[i])
			return FALSE;

		*ptrDate[i] = (WORD)nResult[i];
	}

	dt = sysTime;
	return dt.GetStatus() == COleDateTime::valid;
}


void CXTPPropertyGridItemDate::SetValue(CString strValue)
{
	COleDateTime dt;
	TRY
	{
		if (ParseDateTime(dt, strValue))
		{
			SetDate(dt);
		}
		else if (dt.ParseDateTime(strValue, VAR_DATEVALUEONLY, LANG_RUSSIAN))
		{
			SetDate(dt);
		}
	}
	CATCH(COleException, e)
	{

	}
	END_CATCH
}


void CXTPPropertyGridItemDate::SetDateFormat(CString strFormat)
{
	m_strFormat = strFormat;

	CString strValue = m_oleDate.Format(m_strFormat);
	CXTPPropertyGridItem::SetValue(strValue);
}

void CXTPPropertyGridItemDate::SetDate(const COleDateTime& oleDate)
{
	ASSERT(oleDate.GetStatus() == COleDateTime::valid);
	m_oleDate = oleDate;

	CString strValue = oleDate.Format(m_strFormat);

	CXTPPropertyGridItem::SetValue(strValue);
}


#if _MSC_VER < 1200 // MFC 5.0

#define MIN_DATE                (-657434L)  // about year 100
#define MAX_DATE                2958465L    // about year 9999
#define HALF_SECOND  (1.0/172800.0)

static int nMonthDays[13] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};

static BOOL XTPPropetryGridTmFromOleDate(DATE dtSrc, struct tm& tmDest)
{
	// The legal range does not actually span year 0 to 9999.
	if (dtSrc > MAX_DATE || dtSrc < MIN_DATE) // about year 100 to about 9999
		return FALSE;

	long nDays;             // Number of days since Dec. 30, 1899
	long nDaysAbsolute;     // Number of days since 1/1/0
	long nSecsInDay;        // Time in seconds since midnight
	long nMinutesInDay;     // Minutes in day
	long n400Years;         // Number of 400 year increments since 1/1/0
	long n400Century;       // Century within 400 year block (0,1,2 or 3)
	long n4Years;           // Number of 4 year increments since 1/1/0
	long n4Day;             // Day within 4 year block

	//  (0 is 1/1/yr1, 1460 is 12/31/yr4)
	long n4Yr;              // Year within 4 year block (0,1,2 or 3)
	BOOL bLeap4 = TRUE;     // TRUE if 4 year block includes leap year

	double dblDate = dtSrc; // temporary serial date

	// If a valid date, then this conversion should not overflow
	nDays = (long)dblDate;

	// Round to the second
	dblDate += ((dtSrc > 0.0) ? HALF_SECOND : -HALF_SECOND);

	nDaysAbsolute = (long)dblDate + 693959L; // Add days from 1/1/0 to 12/30/1899

	dblDate = fabs(dblDate);
	nSecsInDay = (long)((dblDate - floor(dblDate)) * 86400.);

	// Calculate the day of week (sun=1, mon=2...)
	//   -1 because 1/1/0 is Sat.  +1 because we want 1-based
	tmDest.tm_wday = (int)((nDaysAbsolute - 1) % 7L) + 1;

	// Leap years every 4 yrs except centuries not multiples of 400.
	n400Years = (long)(nDaysAbsolute / 146097L);

	// Set nDaysAbsolute to day within 400-year block
	nDaysAbsolute %= 146097L;

	// -1 because first century has extra day
	n400Century = (long)((nDaysAbsolute - 1) / 36524L);

	// Non-leap century
	if (n400Century != 0)
	{
		// Set nDaysAbsolute to day within century
		nDaysAbsolute = (nDaysAbsolute - 1) % 36524L;

		// +1 because 1st 4 year increment has 1460 days
		n4Years = (long)((nDaysAbsolute + 1) / 1461L);

		if (n4Years != 0)
			n4Day = (long)((nDaysAbsolute + 1) % 1461L);
		else
		{
			bLeap4 = FALSE;
			n4Day = (long)nDaysAbsolute;
		}
	}
	else
	{
		// Leap century - not special case!
		n4Years = (long)(nDaysAbsolute / 1461L);
		n4Day = (long)(nDaysAbsolute % 1461L);
	}

	if (bLeap4)
	{
		// -1 because first year has 366 days
		n4Yr = (n4Day - 1) / 365;

		if (n4Yr != 0)
			n4Day = (n4Day - 1) % 365;
	}
	else
	{
		n4Yr = n4Day / 365;
		n4Day %= 365;
	}

	// n4Day is now 0-based day of year. Save 1-based day of year, year number
	tmDest.tm_yday = (int)n4Day + 1;
	tmDest.tm_year = n400Years * 400 + n400Century * 100 + n4Years * 4 + n4Yr;

	// Handle leap year: before, on, and after Feb. 29.
	if (n4Yr == 0 && bLeap4)
	{
		// Leap Year
		if (n4Day == 59)
		{
			/* Feb. 29 */
			tmDest.tm_mon = 2;
			tmDest.tm_mday = 29;
			goto DoTime;
		}

		// Pretend it's not a leap year for month/day comp.
		if (n4Day >= 60)
			--n4Day;
	}

	// Make n4DaY a 1-based day of non-leap year and compute
	//  month/day for everything but Feb. 29.
	++n4Day;

	// Month number always >= n/32, so save some loop time */
	for (tmDest.tm_mon = (n4Day >> 5) + 1;
	n4Day > nMonthDays[tmDest.tm_mon]; tmDest.tm_mon++);

	tmDest.tm_mday = (int)(n4Day - nMonthDays[tmDest.tm_mon-1]);

DoTime:
	if (nSecsInDay == 0)
		tmDest.tm_hour = tmDest.tm_min = tmDest.tm_sec = 0;
	else
	{
		tmDest.tm_sec = (int)nSecsInDay % 60L;
		nMinutesInDay = nSecsInDay / 60L;
		tmDest.tm_min = (int)nMinutesInDay % 60;
		tmDest.tm_hour = (int)nMinutesInDay / 60;
	}

	return TRUE;
}

BOOL CXTPPropertyGridItemDate::GetAsSystemTime(SYSTEMTIME& sysTime)
{
	BOOL bRetVal = FALSE;
	if (m_oleDate.GetStatus() == COleDateTime::valid)
	{
		struct tm tmTemp;
		if (XTPPropetryGridTmFromOleDate(m_oleDate, tmTemp))
		{
			sysTime.wYear = (WORD) tmTemp.tm_year;
			sysTime.wMonth = (WORD) tmTemp.tm_mon;
			sysTime.wDayOfWeek = (WORD) tmTemp.tm_wday;
			sysTime.wDay = (WORD) tmTemp.tm_mday;
			sysTime.wMinute = sysTime.wMilliseconds = sysTime.wSecond = sysTime.wHour = 0;

			bRetVal = TRUE;
		}
	}

	return bRetVal;
}
#else

BOOL CXTPPropertyGridItemDate::GetAsSystemTime(SYSTEMTIME& sysTime)
{
	BOOL bRetVal = FALSE;
	if (m_oleDate.GetStatus() == COleDateTime::valid)
	{
		bRetVal = m_oleDate.GetAsSystemTime(sysTime);
		sysTime.wMinute = sysTime.wMilliseconds = sysTime.wSecond = sysTime.wHour = 0;
	}

	return bRetVal;
}
#endif

void CXTPPropertyGridItemDate::OnInplaceButtonDown()
{
	CXTPPropertyGridInplaceMonthCal*pManthCtrl = new CXTPPropertyGridInplaceMonthCal(this);

	CRect rcItem= GetItemRect();
	m_pGrid->ClientToScreen(&rcItem);

#if _MSC_VER < 1200 // MFC 5.0
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(icex);
	icex.dwICC = ICC_DATE_CLASSES;
	VERIFY(InitCommonControlsEx(&icex));
#else
	VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTL_DATE_REG));
#endif // _MSC_VER < 1200


	CRect rect(0, 0, 0, 0);
	pManthCtrl->CreateEx(WS_EX_TOPMOST, MONTHCAL_CLASS, NULL, WS_BORDER|WS_POPUP|WS_VISIBLE, rect, m_pGrid, 0);

	SYSTEMTIME sysTime;
	if (GetAsSystemTime(sysTime))
	{
		pManthCtrl->SendMessage(MCM_SETCURSEL, 0, (LPARAM) &sysTime);
	}
	
	if (pManthCtrl->GetMinReqRect(rect))
	{
		rect.SetRect(rcItem.right - rect.Width(), rcItem.bottom, rcItem.right, rcItem.bottom + rect.Height());

		CRect rcWork = CXTPDrawHelpers::GetWorkArea(rcItem);
		if (rect.bottom > rcWork.bottom && rect.top > rcWork.CenterPoint().y)
		{
			rect.OffsetRect(0, - rect.Height() - rcItem.Height() - 1);
		}
		if (rect.left < rcWork.left) rect.OffsetRect(rcWork.left - rect.left, 0);
		if (rect.right > rcWork.right) rect.OffsetRect(rcWork.right - rect.right, 0);

		pManthCtrl->SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
	}


	pManthCtrl->SetOwner(m_pGrid);
	pManthCtrl->SetFocus();
}

long CXTPPropertyGridItemDate::GetDay()
{
	return m_oleDate.GetDay();
}

void CXTPPropertyGridItemDate::SetDay(long nDay)
{
	COleDateTime oleDate(GetYear(), GetMonth(), nDay, 0, 0, 0);
	SetDate(oleDate);
}

long CXTPPropertyGridItemDate::GetMonth()
{
	return m_oleDate.GetMonth();
}
void CXTPPropertyGridItemDate::SetMonth(long nMonth)
{
	COleDateTime oleDate(GetYear(), nMonth, GetDay(), 0, 0, 0);
	SetDate(oleDate);
}

long CXTPPropertyGridItemDate::GetYear()
{
	return m_oleDate.GetYear();
}

void CXTPPropertyGridItemDate::SetYear(long nYear)
{
	COleDateTime oleDate(nYear, GetMonth(), GetDay(), 0, 0, 0);
	SetDate(oleDate);
}



#ifdef __AFXCTL_H__

//////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemPicture

CXTPPropertyGridItemPicture::CXTPPropertyGridItemPicture(CString strCaption)
	: CXTPPropertyGridItem(strCaption)
{
	m_nFlags = xtpGridItemHasExpandButton;
}

BOOL CXTPPropertyGridItemPicture::OnDrawItemValue(CDC& dc, CRect rcValue)
{
	COLORREF clr = dc.GetTextColor();
	CRect rcSample(rcValue.left - 2, rcValue.top + 1, rcValue.left + 18, rcValue.bottom - 1);

	m_olePicture.Render(&dc, rcSample, rcSample);
	dc.Draw3dRect(rcSample, clr, clr);

	CRect rcText(rcValue);
	rcText.left += 25;

	short type = m_olePicture.GetType();
	CString str = type == PICTYPE_ICON? _T("(Icon)"):
		type == PICTYPE_BITMAP? _T("(Bitmap)") : _T("None");

	dc.DrawText(str, rcText, DT_SINGLELINE|DT_VCENTER);

	return TRUE;
}

void CXTPPropertyGridItemPicture::OnInplaceButtonDown()
{
	CString strFilter = _T("All Picture Types|*.bmp;*.cur;*.dib;*.emf;*.ico;*.wmf;*.gif;*.jpg|Bitmaps (*.bmp;*.dib;*.gif;*.jpg)|*.bmp;*.dib;*.gif;*.jpg|Icons/Cursors (*.ico;*.cur)|*.ico;*.cur|Metafiles (*.wmf;*.emf)|*.wmf;*.emf|All files (*.*)|*.*||");

	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, strFilter);

	if (dlg.DoModal() == IDOK)
	{
		SetPicturePath(dlg.GetPathName());
	}
}

void CXTPPropertyGridItemPicture::SetPicturePath(LPCTSTR lpszPath)
{
	USES_CONVERSION;

	m_strPicturePath = lpszPath;
	LPPICTUREDISP pPict = NULL;
	
	if (OleLoadPicturePath(T2OLE((LPTSTR)(LPCTSTR)m_strPicturePath), NULL, 0, 0, IID_IPictureDisp, (LPVOID*)&pPict) == S_OK )
	{
		m_olePicture.SetPictureDispatch(pPict);
		pPict->Release();
		
		OnValueChanged(_T(""));
		((CWnd*)m_pGrid)->Invalidate(FALSE);
	}
}



#endif
