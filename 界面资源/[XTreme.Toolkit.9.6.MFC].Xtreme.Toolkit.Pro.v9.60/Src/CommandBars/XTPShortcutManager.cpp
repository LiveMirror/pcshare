// XTPShortcutManager.cpp : implementation of the CXTPShortcutManager class.
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
#include "XTPPropExchange.h"

#include "XTPToolBar.h"
#include "XTPShortcutManager.h"
#include "XTPCommandBars.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CProcessLocal<CXTPShortcutManager> CXTPShortcutManager::m_managerInstance;

CXTPShortcutManager* AFX_CDECL XTPShortcutManager()
{
	return CXTPShortcutManager::m_managerInstance.GetData();
}

void CXTPShortcutManager::SetShortcutManager(CXTPShortcutManager* pShortcutManager)
{
	if (pShortcutManager == NULL)
		return;

	if (m_managerInstance.m_pObject != NULL)
		delete m_managerInstance.m_pObject;

	m_managerInstance.m_pObject = pShortcutManager;
}

CXTPShortcutManager::CXTPShortcutManager()
{

}

CString CXTPShortcutManager::Format(LPACCEL lpAccel, int* pPriority)
{
	ASSERT(pPriority);

	CString str;
	CKeyHelper helper (lpAccel);
	helper.Format (str);

	*pPriority = helper.Priority();

	return str;
}

BOOL CXTPShortcutManager::FindDefaultAccelerator (UINT nCmd, CString& strShortcut, CXTPCommandBars* pCommandBars)
{
	CFrameWnd* pWndMain = DYNAMIC_DOWNCAST (CFrameWnd, (pCommandBars? pCommandBars->GetSite(): AfxGetMainWnd ()));
	if (pWndMain == NULL)
		return FALSE;

	HACCEL hAccelTable = pWndMain->GetDefaultAccelerator ();

	if (!hAccelTable)
		return FALSE;

	ASSERT(hAccelTable);

	int nAccelSize = ::CopyAcceleratorTable (hAccelTable, NULL, 0);

	LPACCEL lpAccel = new ACCEL [nAccelSize];
	::CopyAcceleratorTable (hAccelTable, lpAccel, nAccelSize);

	BOOL bFound = FALSE;
	BOOL bEqual = FALSE;
	CString strFirst = _T("");
	int nFirstPriorety = 0;


	for (int i = 0; i < nAccelSize; i ++)
	{
		if (lpAccel [i].cmd == nCmd)
		{
			int nPriority = 0;
			CString str = Format(&lpAccel [i], &nPriority);

			if (str == strShortcut)
				bEqual = TRUE;

			if (strFirst.IsEmpty() || (nFirstPriorety < nPriority))
			{
				strFirst = str;
				nFirstPriorety = nPriority;
			}

			bFound = TRUE;
		}
	}
	delete[] lpAccel;

	if (!bFound)
		strShortcut = "";
	else if (!bEqual)
		strShortcut = strFirst;


	return bFound;
}

CXTPShortcutManager::CKeyHelper::CKeyHelper(LPACCEL lpAccel) :
	m_lpAccel (lpAccel)
{
}

CXTPShortcutManager::CKeyHelper::~CKeyHelper()
{
}

int CXTPShortcutManager::CKeyHelper::Priority()
{
	if (m_lpAccel == NULL)
		return 0;
	if (m_lpAccel->fVirt & FCONTROL)
		return 3;
	if (m_lpAccel->fVirt & FALT)
		return 2;
	if (m_lpAccel->fVirt & FSHIFT)
		return 1;
	return 4;
}




void CXTPShortcutManager::CKeyHelper::Format (CString& str) const
{
	str.Empty ();

	if (m_lpAccel == NULL)
	{
		ASSERT (FALSE);
		return;
	}

	if (m_lpAccel->key != VK_CANCEL)
	{
		if (m_lpAccel->fVirt & FCONTROL)
		{
			AddVirtKeyStr (str, VK_CONTROL);
		}

		if (m_lpAccel->fVirt & FSHIFT)
		{
			AddVirtKeyStr (str, VK_SHIFT);
		}

		if (m_lpAccel->fVirt & FALT)
		{
			AddVirtKeyStr (str, VK_MENU);
		}
	}

	if (m_lpAccel->key)
	{
		if (m_lpAccel->fVirt & FVIRTKEY)
		{
			AddVirtKeyStr (str, m_lpAccel->key, TRUE);
		}
		else if (m_lpAccel->key != 27)
		{
			str += (char) m_lpAccel->key;
		}
	}
	else if (str.GetLength() > 0)
	{
		str = str.Left(str.GetLength() - 1);
	}
}

void CXTPShortcutManager::CKeyHelper::AddVirtKeyStr (CString& str, UINT uiVirtKey, BOOL bLast) const
{
	const static struct
	{
		WORD    wKey;       // Virtual Key Code.
		LPCTSTR szKeyName;  // Display Name (i.e "CTRL").
	} virtKeys[] =
	{
		{ _T('0'),      _T("0") },
		{ _T('1'),      _T("1") },
		{ _T('2'),      _T("2") },
		{ _T('3'),      _T("3") },
		{ _T('4'),      _T("4") },
		{ _T('5'),      _T("5") },
		{ _T('6'),      _T("6") },
		{ _T('7'),      _T("7") },
		{ _T('8'),      _T("8") },
		{ _T('9'),      _T("9") },
		{ _T('A'),      _T("A") },
		{ _T('B'),      _T("B") },
		{ _T('C'),      _T("C") },
		{ _T('D'),      _T("D") },
		{ _T('E'),      _T("E") },
		{ _T('F'),      _T("F") },
		{ _T('G'),      _T("G") },
		{ _T('H'),      _T("H") },
		{ _T('I'),      _T("I") },
		{ _T('J'),      _T("J") },
		{ _T('K'),      _T("K") },
		{ _T('L'),      _T("L") },
		{ _T('M'),      _T("M") },
		{ _T('N'),      _T("N") },
		{ _T('O'),      _T("O") },
		{ _T('P'),      _T("P") },
		{ _T('Q'),      _T("Q") },
		{ _T('R'),      _T("R") },
		{ _T('S'),      _T("S") },
		{ _T('T'),      _T("T") },
		{ _T('U'),      _T("U") },
		{ _T('V'),      _T("V") },
		{ _T('W'),      _T("W") },
		{ _T('X'),      _T("X") },
		{ _T('Y'),      _T("Y") },
		{ _T('Z'),      _T("Z") },
		{ VK_LBUTTON,   _T("Left Button") },
		{ VK_RBUTTON,   _T("Right Button") },
		{ VK_CANCEL,    _T("Ctrl+Break") },
		{ VK_MBUTTON,   _T("Middle Button") },
		{ VK_BACK,      _T("Back") },
		{ VK_TAB,       _T("Tab") },
		{ VK_CLEAR,     _T("Clear") },
		{ VK_RETURN,    _T("Enter") },
		{ VK_SHIFT,     _T("Shift") },
		{ VK_CONTROL,   _T("Ctrl") },
		{ VK_MENU,      _T("Alt") },
		{ VK_PAUSE,     _T("Pause") },
		{ VK_CAPITAL,   _T("Caps Lock") },
		{ VK_ESCAPE,    _T("Esc") },
		{ VK_SPACE,     _T("Space") },
		{ VK_PRIOR,     _T("Page Up") },
		{ VK_NEXT,      _T("Page Down") },
		{ VK_END,       _T("End") },
		{ VK_HOME,      _T("Home") },
		{ VK_LEFT,      _T("Left Arrow") },
		{ VK_UP,        _T("Up Arrow") },
		{ VK_RIGHT,     _T("Right Arrow") },
		{ VK_DOWN,      _T("Down Arrow") },
		{ VK_SELECT,    _T("Select") },
		{ VK_PRINT,     _T("Print") },
		{ VK_EXECUTE,   _T("Execute") },
		{ VK_SNAPSHOT,  _T("Snapshot") },
		{ VK_INSERT,    _T("Ins") },
		{ VK_DELETE,    _T("Del") },
		{ VK_HELP,      _T("Help") },
		{ VK_LWIN ,     _T("Left Windows") },
		{ VK_RWIN,      _T("Right Windows") },
		{ VK_APPS,      _T("Application") },
		{ VK_MULTIPLY,  _T("Num *") },
		{ VK_ADD,       _T("Num +") },
		{ VK_SEPARATOR, _T("Separator") },
		{ VK_SUBTRACT,  _T("Num -") },
		{ VK_DECIMAL,   _T("Num .") },
		{ VK_DIVIDE,    _T("Num /") },
		{ VK_F1,        _T("F1") },
		{ VK_F2,        _T("F2") },
		{ VK_F3,        _T("F3") },
		{ VK_F4,        _T("F4") },
		{ VK_F5,        _T("F5") },
		{ VK_F6,        _T("F6") },
		{ VK_F7,        _T("F7") },
		{ VK_F8,        _T("F8") },
		{ VK_F9,        _T("F9") },
		{ VK_F10,       _T("F10") },
		{ VK_F11,       _T("F11") },
		{ VK_F12,       _T("F12") },
		{ VK_NUMPAD0,   _T("Num 0") },
		{ VK_NUMPAD1,   _T("Num 1") },
		{ VK_NUMPAD2,   _T("Num 2") },
		{ VK_NUMPAD3,   _T("Num 3") },
		{ VK_NUMPAD4,   _T("Num 4") },
		{ VK_NUMPAD5,   _T("Num 5") },
		{ VK_NUMPAD6,   _T("Num 6") },
		{ VK_NUMPAD7,   _T("Num 7") },
		{ VK_NUMPAD8,   _T("Num 8") },
		{ VK_NUMPAD9,   _T("Num 9") },
		{ VK_NUMLOCK,   _T("Num Lock") },
		{ VK_SCROLL,    _T("Scrl Lock") },
		{ VK_ATTN,      _T("Attn") },
		{ VK_CRSEL,     _T("Crsel") },
		{ VK_EXSEL,     _T("Exsel") },
		{ VK_EREOF,     _T("Ereof") },
		{ VK_PLAY,      _T("Play") },
		{ VK_ZOOM,      _T("Zoom") },
		{ VK_NONAME,    _T("No Name") },
		{ VK_PA1,       _T("Pa1") },
		{ VK_OEM_CLEAR, _T("Oem Clear") },
	};


	#define BUFFER_LEN 50
	TCHAR szBuffer [BUFFER_LEN + 1];
	ZeroMemory(szBuffer, BUFFER_LEN);

	if (uiVirtKey != VK_CANCEL)
	{
		UINT nScanCode = ::MapVirtualKeyEx (uiVirtKey, 0,
			::GetKeyboardLayout (0)) <<16 | 0x1;

		if (uiVirtKey >= VK_PRIOR && uiVirtKey <= VK_HELP)
		{
			nScanCode |= 0x01000000;
		}

		::GetKeyNameText (nScanCode, szBuffer, BUFFER_LEN);
	}

	CString strKey(szBuffer);
	if (strKey.IsEmpty())
	{
		for (int i = 0; i < _countof(virtKeys); i++)
		{
			if(uiVirtKey == virtKeys[i].wKey)
			{
				strKey = virtKeys[i].szKeyName;
			}
		}
	}
	else
	{
		strKey.MakeLower();

		for (int i = 0; i < strKey.GetLength(); i++)
		{
			TCHAR c = strKey[i];
			if (IsCharLower (c))
			{
				c = (TCHAR) toupper (c);
				strKey.SetAt (i, c);
				break;
			}
		}
	}

	if (!strKey.IsEmpty())
	{
		str += strKey;

		if (!bLast)
		{
			str += '+';
		}
	}
}


TCHAR CXTPShortcutManager::ToUpper(TCHAR vkTCHAR)
{

#if 1

	CString str(vkTCHAR);
	str.MakeUpper();
	return str.GetLength() > 0? str[0]: (TCHAR)0;

#else

	BYTE lpKeyState[256];
	::GetKeyboardState( lpKeyState );
	HKL hKeyboardLayout = ::GetKeyboardLayout(::AfxGetThread() -> m_nThreadID);
#if (defined _UNICODE)
	TCHAR szChar[2] = { _T('\0'), _T('\0') };
	::ToUnicodeEx(vkTCHAR, ::MapVirtualKey( vkTCHAR, 0 ), lpKeyState, szChar, 1, 1, hKeyboardLayout );
#else
	WORD nMapped = 0;
	::ToAsciiEx(vkTCHAR, ::MapVirtualKey( vkTCHAR, 0 ), lpKeyState, &nMapped, 1, hKeyboardLayout);
	TCHAR szChar[2] = { (TCHAR)nMapped, _T('\0') };
#endif
	::CharUpper( szChar );
	return szChar[0];

#endif
}


/////////////////////////////////////////////////////////////////////////////
// CKeyAssign

CXTPShortcutManager::CKeyAssign::CKeyAssign()
	: m_keyHelper (&m_hAccel)
{
	m_bKeyDefined = FALSE;
	ResetKey ();
}

CXTPShortcutManager::CKeyAssign::~CKeyAssign()
{
}

BOOL CXTPShortcutManager::CKeyAssign::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_LBUTTONDOWN ||
		pMsg->message == WM_MBUTTONDOWN ||
		pMsg->message == WM_RBUTTONDOWN)
	{
		SetFocus ();
		return TRUE;
	}

	BOOL bPressed = pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN;

	if (bPressed || pMsg->message == WM_KEYUP || pMsg->message == WM_SYSKEYUP)
	{
		if (bPressed && !((1 << 30) & pMsg->lParam))
		{
			ResetKey();
		}

		if (!m_bKeyDefined)
		{
			// read in the actual state because we were not tracking releases if there was a
			// definition
			SetAccelFlag (FSHIFT, ::GetKeyState(VK_SHIFT) & 0x8000);
			SetAccelFlag (FCONTROL, ::GetKeyState(VK_CONTROL) & 0x8000);
			SetAccelFlag (FALT, ::GetKeyState(VK_MENU) & 0x8000);

			if (pMsg->wParam == VK_SHIFT
					|| pMsg->wParam == VK_CONTROL
					|| pMsg->wParam == VK_MENU
					)
			{
				// all work is already done
			}
			else
			{
				m_hAccel.key = (WORD) pMsg->wParam;
				if (bPressed)
				{
					m_bKeyDefined = TRUE;
					SetAccelFlag (FVIRTKEY, TRUE);
				}
			}
		}

		if ((m_hAccel.fVirt & FCONTROL) == 0
				&& (m_hAccel.fVirt & FSHIFT) == 0
				&& (m_hAccel.fVirt & FALT) == 0
				&& (m_hAccel.fVirt & FVIRTKEY)
				&& m_hAccel.key == VK_ESCAPE
		)
		{
			ResetKey();
			return TRUE;
		}

		if (!((m_hAccel.fVirt & FCONTROL) == 0
				&& (m_hAccel.fVirt & FALT) == 0
				&& (m_hAccel.fVirt & FVIRTKEY)
				&& m_hAccel.key == VK_TAB)
		)
		{
			CString str;
			m_keyHelper.Format (str);

			SetWindowText (str);
			return TRUE;
		}

		ResetKey();
	}

	return CEdit::PreTranslateMessage(pMsg);
}

void CXTPShortcutManager::CKeyAssign::ResetKey ()
{
	memset (&m_hAccel, 0, sizeof (ACCEL));
	m_bKeyDefined = FALSE;

	if (m_hWnd != NULL)
	{
		SetWindowText (_T(""));
	}
}

void CXTPShortcutManager::CKeyAssign::SetAccelFlag (BYTE bFlag, BOOL bSet)
{
	if (bSet) m_hAccel.fVirt |= bFlag;
	else m_hAccel.fVirt &= ~bFlag;
}

BOOL CXTPShortcutManager::CKeyAssign::IsKeyDefined () const
{
	return m_bKeyDefined;
}

ACCEL const* CXTPShortcutManager::CKeyAssign::GetAccel () const
{
	return &m_hAccel;
}

void CXTPShortcutManager::CKeyAssign::SetAccel(ACCEL& hAccel)
{
	m_hAccel = hAccel;

	if (m_hWnd != NULL)
	{
		CString str;
		m_keyHelper.Format (str);

		SetWindowText (str);
	}
}

#define REG_ENTRY_DATA  _T("Accelerators")

void CXTPShortcutManager::UpdateAcellTable(CFrameWnd* pFrame, LPACCEL lpAccel, int nSize)
{
	HACCEL hAccelNew = ::CreateAcceleratorTable(lpAccel, nSize);

	HACCEL hAccelTable = pFrame->m_hAccelTable;
	pFrame->m_hAccelTable = hAccelNew;
	if (hAccelTable) ::DestroyAcceleratorTable (hAccelTable);

}



ACCEL * CXTPShortcutManager::GetShortcut(CFrameWnd* pFrame, int ID, ACCEL * pAccel)
{
	if (!pFrame)
		return NULL;

	ACCEL * result = NULL;

	HACCEL hAccelTable = pFrame->m_hAccelTable;

	int nAccelSize = ::CopyAcceleratorTable (hAccelTable, NULL, 0);

	LPACCEL lpAccel = new ACCEL [nAccelSize];
	::CopyAcceleratorTable (hAccelTable, lpAccel, nAccelSize);

	for (int i = 0; i < nAccelSize; i++)
	{
		if( lpAccel[i].cmd == ID )
		{
			*pAccel = lpAccel[i];
			result = pAccel;
		}
	}
	delete[] lpAccel;
	if( (pAccel->fVirt % 2) == 1 )
		pAccel->fVirt -= 1;
	return result;
}

void CXTPShortcutManager::AddShortcut(CFrameWnd* pFrame, long fVirt, long key, long cmd)
{
	if (!pFrame)
		return;

	ACCEL accel;
	accel.fVirt = (BYTE)(fVirt | FVIRTKEY);
	accel.key = (WORD)key;
	accel.cmd = (WORD)cmd;

	HACCEL hAccelTable = pFrame->m_hAccelTable;

	int nAccelSize = ::CopyAcceleratorTable (hAccelTable, NULL, 0);

	LPACCEL lpAccel = new ACCEL [nAccelSize + 1];
	::CopyAcceleratorTable (hAccelTable, lpAccel, nAccelSize);

	int nIndex = nAccelSize;

	for (int i = 0; i < nAccelSize; i++)
	{
		if (CKeyHelper::EqualAccels(&lpAccel[i], &accel))
		{
			nIndex = i;
			break;
		}
	}

	lpAccel[nIndex] = accel;

	UpdateAcellTable(pFrame, lpAccel, nAccelSize + (nIndex == nAccelSize ? 1 : 0));

	delete[] lpAccel;
}

HACCEL CXTPShortcutManager::CopyAccelTable(HACCEL hAccelTable)
{
		int nAccelSize = ::CopyAcceleratorTable (hAccelTable, NULL, 0);
		LPACCEL lpAccel = new ACCEL [nAccelSize];
		::CopyAcceleratorTable (hAccelTable, lpAccel, nAccelSize);
		HACCEL hAccelNew = ::CreateAcceleratorTable(lpAccel, nAccelSize);
		delete[] lpAccel;
		return hAccelNew;
}


void CXTPShortcutManager::SerializeShortcuts(CFrameWnd* pFrame, CArchive& ar)
{
	CXTPPropExchangeArchive px(ar);
	DoPropExchange(&px, pFrame);
}

void CXTPShortcutManager::SaveShortcuts(CFrameWnd* pFrame, LPCTSTR lpszProfileName)
{
	HACCEL hAccelTable = pFrame->m_hAccelTable;
	if (!hAccelTable)
		return;

	int nAccelSize = ::CopyAcceleratorTable (hAccelTable, NULL, 0);

	LPACCEL lpAccel = new ACCEL [nAccelSize];
	ASSERT (lpAccel != NULL);

	::CopyAcceleratorTable (hAccelTable, lpAccel, nAccelSize);

	AfxGetApp()->WriteProfileBinary(lpszProfileName, REG_ENTRY_DATA, (LPBYTE) lpAccel, nAccelSize * sizeof (ACCEL));

	delete[] lpAccel;

}

void CXTPShortcutManager::LoadShortcuts(CFrameWnd* pFrame, LPCTSTR lpszProfileName)
{
	UINT uiSize;
	LPACCEL lpAccel = 0;
	if (AfxGetApp()->GetProfileBinary(lpszProfileName, REG_ENTRY_DATA, (LPBYTE*) &lpAccel, &uiSize))
	{
		int nAccelSize = uiSize / sizeof (ACCEL);
		ASSERT (lpAccel != NULL);

		UpdateAcellTable(pFrame, lpAccel, nAccelSize);

		delete[] lpAccel;
	}
}

void CXTPShortcutManager::DoPropExchange(CXTPPropExchange* pPX, CFrameWnd* pFrame)
{
	if (pPX->IsStoring())
	{
		int nAccelSize = ::CopyAcceleratorTable (pFrame->m_hAccelTable, NULL, 0);
		LPACCEL lpAccel = new ACCEL [nAccelSize];
		::CopyAcceleratorTable (pFrame->m_hAccelTable, lpAccel, nAccelSize);		

		if (pPX->IsAllowBlobValues())
		{
			pPX->WriteCount(nAccelSize);
			pPX->Write(_T("Data"), lpAccel, nAccelSize* sizeof(ACCEL));
		}
		else
		{
			CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("Accel")));
			POSITION posEnum = pEnumerator->GetPosition(nAccelSize);
			
			for (int i = 0; i < nAccelSize; i++)
			{
				CXTPPropExchangeSection secItem(pEnumerator->GetNext(posEnum));
				PX_UShort(&secItem, _T("Id"), (USHORT&)(lpAccel[i].cmd), 0);
				PX_Byte(&secItem, _T("virt"), (lpAccel[i].fVirt), 0);
				PX_UShort(&secItem, _T("key"), (USHORT&)(lpAccel[i].key), 0);
			}
		}

		delete[] lpAccel;
	}
	else
	{
		if (pPX->IsAllowBlobValues())
		{
			int nAccelSize = (int)pPX->ReadCount();
			LPACCEL lpAccel = new ACCEL [nAccelSize];
			pPX->Read(_T("Data"), lpAccel, nAccelSize* sizeof(ACCEL));
			
			UpdateAcellTable(pFrame, lpAccel, nAccelSize);
			delete[] lpAccel;
		}
		else
		{
			CArray<ACCEL, ACCEL&> accels;
			CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("Accel")));
			POSITION posEnum = pEnumerator->GetPosition(0);
			while(posEnum)			
			{
				ACCEL accel;
				CXTPPropExchangeSection secItem(pEnumerator->GetNext(posEnum));
				PX_UShort(&secItem, _T("Id"), (USHORT&)(accel.cmd), 0);
				PX_Byte(&secItem, _T("virt"), (accel.fVirt), 0);
				PX_UShort(&secItem, _T("key"), (USHORT&)(accel.key), 0);
				accels.Add(accel);
			}
			UpdateAcellTable(pFrame, accels.GetData(), (int)accels.GetSize());
		}

	}
}
