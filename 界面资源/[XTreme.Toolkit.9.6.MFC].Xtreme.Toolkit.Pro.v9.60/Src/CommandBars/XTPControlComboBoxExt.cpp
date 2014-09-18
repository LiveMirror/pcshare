// XTPControlComboBoxExt.cpp : implementation of the CXTPControlComboBoxExt class.
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
#include "XTPVC80Helpers.h"
#include "XTPResourceManager.h"
#include "XTPPropExchange.h"

#include "XTPCommandBarsDefines.h"
#include "XTPCommandBarsResource.h"
#include "XTPMouseManager.h"
#include "XTPControlComboBox.h"
#include "XTPControlComboBoxExt.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define XTP_FONTCOMBOSTYLE_DEFAULTCHARSETONLY 1


// reserve lobyte for charset
#define PRINTER_FONT 0x0100
#define TT_FONT 0x0200
#define DEVICE_FONT 0x0400

#define MAX_POINT_SIZE 10

static int nFontSizes[] =
	{8, 9, 10, 11, 12, 14, 16, 18, 20, 22, 24, 26, 28, 36, 48, 72};

static void OnCharFormatChanged(CHARFORMAT& cf, CXTPControl* pControl)
{
	NMXTPCHARHDR fnm;
	fnm.hdr.code = XTP_FN_SETFORMAT;
	fnm.hdr.idFrom = pControl->GetID();
	fnm.hdr.hwndFrom = 0;
	fnm.pControl = pControl;
	fnm.cf = cf;

	pControl->GetParent()->GetSite()->SendMessage(WM_XTP_COMMAND, pControl->GetID(), (LPARAM)&fnm);
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_XTP_CONTROL(CXTPControlSizeComboBox, CXTPControlComboBox)

CXTPControlSizeComboBox::CXTPControlSizeComboBox()
{
	SetWidth(50);
}

CXTPControlSizeComboBox::~CXTPControlSizeComboBox()
{

}

BOOL CXTPControlSizeComboBox::OnSetPopup(BOOL bPopup)
{
	if (bPopup && m_pCommandBar)
	{

		CString str = GetEditText();

		NMXTPCHARHDR fnm;
		fnm.hdr.code = XTP_FN_GETFORMAT;
		fnm.hdr.idFrom = m_nId;
		fnm.hdr.hwndFrom = 0;
		fnm.pControl = this;

		m_pParent->GetSite()->SendMessage(WM_XTP_COMMAND, m_nId, (LPARAM)&fnm);

		if ((fnm.cf.dwMask & (CFM_FACE|CFM_CHARSET)) == (CFM_FACE|CFM_CHARSET))
		{
			CString strFaceName = fnm.cf.szFaceName;
			CWindowDC dc(CWnd::GetDesktopWindow());
			EnumFontSizes(dc, strFaceName);
		}

		int nIdx = FindStringExact(-1, str);
		if (nIdx != CB_ERR) SetCurSel(nIdx);

	}
	return CXTPControlComboBox::OnSetPopup(bPopup);
}

void CXTPControlSizeComboBox::OnExecute()
{
	int nSize = GetTwipSize();
	if (nSize == -2)
	{
		XTPResourceManager()->ShowMessageBox(XTP_IDS_INVALID_NUMBER, MB_OK|MB_ICONINFORMATION);
	}
	else if ((nSize >= 0 && nSize < 20) || nSize > 32760)
	{
		XTPResourceManager()->ShowMessageBox(XTP_IDS_INVALID_FONTSIZE, MB_OK|MB_ICONINFORMATION);
	}
	else if (nSize > 0)
	{
		CHARFORMAT cf;
		cf.cbSize = sizeof(CHARFORMAT);
		cf.dwMask = CFM_SIZE;
		cf.yHeight = nSize;
		OnCharFormatChanged(cf, this);
	}

	CXTPControlComboBox::OnExecute();
}

void CXTPControlSizeComboBox::EnumFontSizes(CDC& dc, LPCTSTR pFontName)
{
	ResetContent();
	if (pFontName == NULL)
		return;
	if (pFontName[0] == NULL)
		return;

	ASSERT(dc.m_hDC != NULL);
	m_nLogVert = dc.GetDeviceCaps(LOGPIXELSY);

	::EnumFontFamilies(dc.m_hDC, pFontName,
		(FONTENUMPROC) EnumSizeCallBack, (LPARAM) this);
}

BOOL CALLBACK AFX_EXPORT CXTPControlSizeComboBox::EnumSizeCallBack(LOGFONT FAR* /*lplf*/,
		LPNEWTEXTMETRIC lpntm, int FontType, LPVOID lpv)
{
	CXTPControlSizeComboBox* pThis = (CXTPControlSizeComboBox*)lpv;
	ASSERT(pThis != NULL);
	TCHAR buf[MAX_POINT_SIZE];
	if (
		(FontType & TRUETYPE_FONTTYPE) ||
		!( (FontType & TRUETYPE_FONTTYPE) || (FontType & RASTER_FONTTYPE) )
		) // if truetype or vector font
	{
		// this occurs when there is a truetype and nontruetype version of a font
		if (pThis->GetCount() != 0)
			pThis->ResetContent();

		for (int i = 0; i < 16; i++)
		{
			wsprintf(buf, _T("%d"), nFontSizes[i]);
			pThis->AddString(buf);
		}
		return FALSE; // don't call me again
	}
	// calc character height in pixels
	pThis->InsertSize(MulDiv(lpntm->tmHeight-lpntm->tmInternalLeading,
		1440, pThis->m_nLogVert));
	return TRUE; // call me again
}

void CXTPControlSizeComboBox::InsertSize(int nSize)
{
	ASSERT(nSize > 0);
	DWORD dwSize = (DWORD)nSize;
	TCHAR buf[MAX_POINT_SIZE];
	TwipsToPointString(buf, nSize);
	if (FindStringExact(-1, buf) == CB_ERR)
	{
		int nIndex = -1;
		int nPos = 0;
		DWORD dw;
		while ((dw = (DWORD)GetItemData(nPos)) != (DWORD)CB_ERR)
		{
			if (dw > dwSize)
			{
				nIndex = nPos;
				break;
			}
			nPos++;
		}
		nIndex = InsertString(nIndex, buf);
		ASSERT(nIndex != CB_ERR);
		if (nIndex != CB_ERR)
			SetItemData(nIndex, dwSize);
	}
}

void CXTPControlSizeComboBox::TwipsToPointString(LPTSTR lpszBuf, int nTwips)
{
	ASSERT(lpszBuf != NULL);
	lpszBuf[0] = NULL;
	if (nTwips >= 0)
	{
		// round to nearest half point
		nTwips = (nTwips+5)/10;
		if ((nTwips%2) == 0)
			SPRINTF_S(lpszBuf, MAX_POINT_SIZE, _T("%ld"), nTwips/2);
		else
			SPRINTF_S(lpszBuf, MAX_POINT_SIZE, _T("%.1f"), (float)nTwips/2.F);
	}
}


int CXTPControlSizeComboBox::GetTwipSize()
{
	// return values
	// -2 -- error
	// -1 -- edit box empty
	// >=0 -- font size in twips
	CString str = GetEditText();
	LPCTSTR lpszText = str;

	while (*lpszText == ' ' || *lpszText == '\t')
		lpszText++;

	if (lpszText[0] == NULL)
		return -1; // no text in control

	double d = _tcstod(lpszText, (LPTSTR*)&lpszText);
	while (*lpszText == ' ' || *lpszText == '\t')
		lpszText++;

	if (*lpszText != NULL)
		return -2;   // not terminated properly

	return (d<0.) ? 0 : (int)(d*20.);
}

void CXTPControlSizeComboBox::SetTwipSize(int nTwips)
{
	if (XTPMouseManager()->IsTrackedLock(NULL))
		return;

	if (nTwips != GetTwipSize())
	{
		TCHAR buf[MAX_POINT_SIZE];
		TwipsToPointString(buf, nTwips);
		SetEditText(buf, TRUE);
	}
	m_nTwipsLast = nTwips;
}

//////////////////////////////////////////////////////////////////////
// CXTPControlFontComboBox Class
//////////////////////////////////////////////////////////////////////



class CXTPControlFontComboBoxList::CFontDesc
{
public:
	CFontDesc(LPCTSTR lpszName, LPCTSTR lpszScript, BYTE nCharSet,
		BYTE nPitchAndFamily, DWORD dwFlags);
	CString m_strName;
	CString m_strScript;
	BYTE m_nCharSet;
	BYTE m_nPitchAndFamily;
	DWORD m_dwFlags;
};

CXTPControlFontComboBoxList::CFontDesc::CFontDesc(LPCTSTR lpszName, LPCTSTR lpszScript, BYTE nCharSet,
	BYTE nPitchAndFamily, DWORD dwFlags)
{
	m_strName = lpszName;
	m_strScript = lpszScript;
	m_nCharSet = nCharSet;
	m_nPitchAndFamily = nPitchAndFamily;
	m_dwFlags = dwFlags;
}

class CXTPControlFontComboBoxList::CFontDescHolder
{
public:
	static BOOL CALLBACK AFX_EXPORT EnumFamScreenCallBack(
		ENUMLOGFONT* pelf, NEWTEXTMETRICEX* /*lpntm*/, int FontType,
		LPVOID pThis);

	static BOOL CALLBACK AFX_EXPORT EnumFamScreenCallBackEx(
		ENUMLOGFONTEX* pelf, NEWTEXTMETRICEX* /*lpntm*/, int FontType,
		LPVOID pThis);

	void AddFont(ENUMLOGFONT* pelf, DWORD dwType, LPCTSTR lpszScript = NULL);

	void EnumFontFamilies();

	~CFontDescHolder()
	{
		for (int i = 0; i < m_arrayFontDesc.GetSize(); i++)
			delete m_arrayFontDesc[i];

	}


	CArray<CFontDesc*, CFontDesc*> m_arrayFontDesc;
};


void CXTPControlFontComboBoxList::CFontDescHolder::EnumFontFamilies()
{
	if (m_arrayFontDesc.GetSize() == 0)
	{
		LOGFONT lf;
		memset(&lf, 0, sizeof(LOGFONT));
		lf.lfCharSet = DEFAULT_CHARSET;

		DWORD dwVersion = ::GetVersion();
		BOOL bWin4 = (BYTE)dwVersion >= 4;
		CWindowDC dc(CWnd::GetDesktopWindow());
		if (bWin4)
		{
			::EnumFontFamiliesEx(dc, &lf,
				(FONTENUMPROC) EnumFamScreenCallBackEx, (LPARAM) this, NULL);
		}
		else
		{
			::EnumFontFamilies(dc, NULL,
				(FONTENUMPROC) EnumFamScreenCallBack, (LPARAM) this);
		}
	}
}
void CXTPControlFontComboBoxList::CFontDescHolder::AddFont(ENUMLOGFONT* pelf, DWORD dwType, LPCTSTR lpszScript)
{
	LOGFONT& lf = pelf->elfLogFont;
	if (lf.lfCharSet == MAC_CHARSET) // don't put in MAC fonts, commdlg doesn't either
		return;
	// Don't display vertical font for FE platform
	if ((GetSystemMetrics(SM_DBCSENABLED)) && (lf.lfFaceName[0] == '@'))
		return;

	for (int i = 0; i < m_arrayFontDesc.GetSize(); i++)
	{
		CFontDesc* pDesc = (CFontDesc*)m_arrayFontDesc[i];
		if (pDesc->m_strName == lf.lfFaceName)
			return;
	}

	// don't put in non-printer raster fonts
	CFontDesc* pDesc = new CFontDesc(lf.lfFaceName, lpszScript,
		lf.lfCharSet, lf.lfPitchAndFamily, dwType);

	int nIndex = 0;
	for (nIndex = 0; nIndex < m_arrayFontDesc.GetSize(); nIndex++)
	{
		if (pDesc->m_strName < ((CFontDesc*)m_arrayFontDesc[nIndex])->m_strName)
			break;
	}
	m_arrayFontDesc.InsertAt(nIndex, pDesc);
}


BOOL CALLBACK AFX_EXPORT CXTPControlFontComboBoxList::CFontDescHolder::EnumFamScreenCallBack(ENUMLOGFONT* pelf,
	NEWTEXTMETRICEX* /*lpntm*/, int FontType, LPVOID pThis)
{
	// don't put in non-printer raster fonts
	if (FontType & RASTER_FONTTYPE)
		return 1;
	DWORD dwData = (FontType & TRUETYPE_FONTTYPE) ? TT_FONT : 0;
	((CXTPControlFontComboBoxList::CFontDescHolder *)pThis)->AddFont(pelf, dwData);
	return 1;
}


BOOL CALLBACK AFX_EXPORT CXTPControlFontComboBoxList::CFontDescHolder::EnumFamScreenCallBackEx(ENUMLOGFONTEX* pelf,
	NEWTEXTMETRICEX* /*lpntm*/, int FontType, LPVOID pThis)
{
	// don't put in non-printer raster fonts
	if (FontType & RASTER_FONTTYPE)
		return 1;
	DWORD dwData = (FontType & TRUETYPE_FONTTYPE) ? TT_FONT : 0;
	((CXTPControlFontComboBoxList::CFontDescHolder *)pThis)->AddFont((ENUMLOGFONT*)pelf, dwData, CString((TCHAR*)pelf->elfScript));
	return 1;
}

void CXTPControlFontComboBoxList::CreateListBox()
{
	CreateEx(WS_EX_STATICEDGE|WS_EX_TOOLWINDOW, _T("LISTBOX"), _T(""),
		WS_POPUP|WS_VSCROLL|WS_BORDER|WS_CLIPCHILDREN|LBS_SORT, CRect(0,0,0,0),  0,  0);
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_XTP_CONTROL(CXTPControlFontComboBox, CXTPControlComboBox)

CXTPControlFontComboBox::CXTPControlFontComboBox(DWORD dwStyleListBox)
{
	if (m_pCommandBar)
	{
		m_pCommandBar->InternalRelease();
	}

	m_pCommandBar = new CXTPControlFontComboBoxList();
	((CXTPControlFontComboBoxList*)m_pCommandBar)->CreateListBox();

	((CXTPControlFontComboBoxList*)m_pCommandBar)->EnumFontFamiliesEx(dwStyleListBox);


	SetWidth(150);
	SetDropDownWidth(250);
}

CXTPControlFontComboBox::~CXTPControlFontComboBox()
{
}


void CXTPControlFontComboBox::OnExecute()
{
	USES_CONVERSION;

	CHARFORMAT cf;
	cf.cbSize = sizeof(CHARFORMAT);
	cf.szFaceName[0] = NULL;

	// this will retrieve the font entered in the edit control
	// it tries to match the font to something already present in the combo box
	// this effectively ignores case of a font the user enters
	// if a user enters arial, this will cause it to become Arial
	CString str;
	GetEditCtrl()->GetWindowText(str);

	SetEditText(str);                    // selects "Arial"
	GetEditCtrl()->GetWindowText(str);  // returns "Arial"

	// if font name box is not empty
	if (str[0] != NULL)
	{
		cf.dwMask = CFM_FACE | CFM_CHARSET;
		int nIndex = FindStringExact(-1, str);
		if (nIndex != CB_ERR)
		{
			CXTPControlFontComboBoxList::CFontDesc* pDesc = (CXTPControlFontComboBoxList::CFontDesc*)GetItemData(nIndex);
			ASSERT(pDesc != NULL);
			ASSERT(pDesc->m_strName.GetLength() < LF_FACESIZE);
		#if (_RICHEDIT_VER >= 0x0200)
			lstrcpyn(cf.szFaceName, pDesc->m_strName, LF_FACESIZE);
		#else
			lstrcpynA(cf.szFaceName,
				T2A((LPTSTR) (LPCTSTR) pDesc->m_strName), LF_FACESIZE);
		#endif
			cf.bCharSet = pDesc->m_nCharSet;
			cf.bPitchAndFamily = pDesc->m_nPitchAndFamily;
		}
		else // unknown font
		{
			ASSERT(str.GetLength() < LF_FACESIZE);

		#if (_RICHEDIT_VER >= 0x0200)
			lstrcpyn(cf.szFaceName, str, LF_FACESIZE);
		#else
			lstrcpynA(cf.szFaceName,
				T2A((LPTSTR) (LPCTSTR) str), LF_FACESIZE);
		#endif
			cf.bCharSet = DEFAULT_CHARSET;
			cf.bPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
		}
		OnCharFormatChanged(cf, this);
	}

	CXTPControlComboBox::OnExecute();
}



void CXTPControlFontComboBox::MatchFont(LPCTSTR lpszName, BYTE nCharSet)
{
	int nFirstIndex = FindString(-1, lpszName);
	if (nFirstIndex != CB_ERR)
	{
		int nIndex = nFirstIndex;
		do
		{
			CXTPControlFontComboBoxList::CFontDesc* pDesc = (CXTPControlFontComboBoxList::CFontDesc*)GetItemData(nIndex);
			ASSERT(pDesc != NULL);
			// check the actual font name to avoid matching Courier western
			// to Courier New western
			if ((nCharSet == DEFAULT_CHARSET || pDesc->m_nCharSet == nCharSet) &&
				lstrcmp(lpszName, pDesc->m_strName)==0)
			{
				//got a match
				if (GetCurSel() != nIndex)
					SetCurSel(nIndex);

				SetEditText(lpszName);
				return;
			}
			nIndex = FindString(nIndex, lpszName);
		} while (nIndex != nFirstIndex);
		// loop until found or back to first item again
	}
	//enter font name
	SetEditText(lpszName, TRUE);
}


void CXTPControlFontComboBox::SetCharFormat(CHARFORMAT& cf)
{
	USES_CONVERSION;

	if (XTPMouseManager()->IsTrackedLock(NULL))
		return;

	// the selection must be same font and charset to display correctly
	if ((cf.dwMask & (CFM_FACE|CFM_CHARSET)) == (CFM_FACE|CFM_CHARSET))
		MatchFont(CString(cf.szFaceName), cf.bCharSet);
	else
		SetEditText(_T(""));
}



void CXTPControlFontComboBoxList::EnumFontFamiliesEx(BOOL dwStyleListBox)
{
	m_dwStyleListBox = dwStyleListBox;

	CMapStringToPtr map;

	((CListBox*)this)->ResetContent();

	static CXTPControlFontComboBoxList::CFontDescHolder s_fontHolder;
	s_fontHolder.EnumFontFamilies();

	// now walk through the fonts and remove (charset) from fonts with only one
	CArray<CFontDesc*, CFontDesc*>& arrFontDesc = s_fontHolder.m_arrayFontDesc;

	int nCount = (int)arrFontDesc.GetSize();
	int i;
	// walk through fonts adding names to string map
	// first time add value 0, after that add value 1
	for (i = 0; i < nCount; i++)
	{
		CFontDesc* pDesc = arrFontDesc[i];
		void* pv = NULL;
		if (map.Lookup(pDesc->m_strName, pv)) // found it
		{
			if (pv == NULL) // only one entry so far
			{
				map.RemoveKey(pDesc->m_strName);
				map.SetAt(pDesc->m_strName, (void*)1);
			}
		}
		else // not found
			map.SetAt(pDesc->m_strName, (void*)0);
	}

	for (i = 0; i < nCount; i++)
	{
		CFontDesc* pDesc = arrFontDesc[i];
		CString str = pDesc->m_strName;
		void* pv = NULL;
		VERIFY(map.Lookup(str, pv));
		if (pv != NULL && !pDesc->m_strScript.IsEmpty())
		{
			str += " (";
			str += pDesc->m_strScript;
			str += ")";
		}

		int nIndex = ((CListBox*)this)->AddString(str);
		ASSERT(nIndex >=0);
		if (nIndex >=0) //no error
			((CListBox*)this)->SetItemDataPtr(nIndex, pDesc);
	}}


IMPLEMENT_XTP_COMMANDBAR(CXTPControlFontComboBoxList, CXTPControlComboBoxList)

void CXTPControlFontComboBoxList::Copy(CXTPCommandBar* pCommandBar, BOOL bRecursive)
{
	CXTPPopupBar::Copy(pCommandBar, bRecursive);

	ASSERT(pCommandBar->IsKindOf(RUNTIME_CLASS(CXTPControlFontComboBoxList)));
	m_dwStyleListBox = ((CXTPControlFontComboBoxList*)pCommandBar)->m_dwStyleListBox;

	if (!m_hWnd)
		CreateListBox();

	EnumFontFamiliesEx(m_dwStyleListBox);

}

void CXTPControlFontComboBoxList::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPPopupBar::DoPropExchange(pPX);

	PX_DWord(pPX, _T("StyleListBox"), m_dwStyleListBox, XTP_FONTCOMBOSTYLE_DEFAULTCHARSETONLY);

	if (pPX->IsLoading())
	{
		if (!m_hWnd)
			CreateListBox();

		EnumFontFamiliesEx(m_dwStyleListBox);
	}
}
