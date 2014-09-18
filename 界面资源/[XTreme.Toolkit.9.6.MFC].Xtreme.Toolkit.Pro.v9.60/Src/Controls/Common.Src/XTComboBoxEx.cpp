// XTComboBoxEx.cpp : implementation of the CXTComboBoxEx class.
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
#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTComboBoxEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTComboBoxEx
/////////////////////////////////////////////////////////////////////////////

BOOL CXTComboBoxEx::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	#if (_MSC_VER <= 1100) // Using Visual C++ 5.0

	// initialize common controls
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(icex);
	icex.dwICC = ICC_USEREX_CLASSES;
	VERIFY(InitCommonControlsEx(&icex));

	#else

	// initialize common controls
	VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTL_USEREX_REG));

	#endif//#if (_MSC_VER <= 1100) // Using Visual C++ 5.0

	CWnd* pWnd = this;
	return pWnd->Create(WC_COMBOBOXEX, NULL, dwStyle, rect, pParentWnd, nID);
}

int CXTComboBoxEx::DeleteItem(int iIndex)
{
	ASSERT(::IsWindow(m_hWnd));
	return (int) ::SendMessage(m_hWnd, CBEM_DELETEITEM, (WPARAM) iIndex, 0);
}

BOOL CXTComboBoxEx::GetItem(COMBOBOXEXITEM* pCBItem)
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(pCBItem != NULL);
	ASSERT(AfxIsValidAddress(pCBItem, sizeof(COMBOBOXEXITEM)));

	return (int) ::SendMessage(m_hWnd, CBEM_GETITEM, 0, (LPARAM) pCBItem);
}

int CXTComboBoxEx::InsertItem(const COMBOBOXEXITEM* pCBItem)
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(pCBItem != NULL);
	ASSERT(AfxIsValidAddress(pCBItem, sizeof(COMBOBOXEXITEM), FALSE));

	return (int) ::SendMessage(m_hWnd, CBEM_INSERTITEM, 0, (LPARAM) pCBItem);
}

int CXTComboBoxEx::InsertItem(int iItem, UINT nStringID, int iIndent, int iImage, int iSelectedImage, UINT mask)
{
	CString strItem;
	strItem.LoadString(nStringID);
	return InsertItem(iItem, strItem, iIndent, iImage, iSelectedImage, mask);
}

int CXTComboBoxEx::InsertItem(int iItem, LPCTSTR lpszItem, int iIndent, int iImage, int iSelectedImage, UINT mask)
{
	COMBOBOXEXITEM cbItem;

	cbItem.mask             = mask;
	cbItem.iItem            = iItem;
	cbItem.pszText          = const_cast<LPTSTR>(lpszItem);
	cbItem.iImage           = iImage;
	cbItem.iSelectedImage   = iSelectedImage;
	cbItem.iIndent          = iIndent;

	return InsertItem(&cbItem);
}

BOOL CXTComboBoxEx::SetItem(const COMBOBOXEXITEM* pCBItem)
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(pCBItem != NULL);
	ASSERT(AfxIsValidAddress(pCBItem, sizeof(COMBOBOXEXITEM), FALSE));

	return (int) ::SendMessage(m_hWnd, CBEM_SETITEM, 0, (LPARAM) pCBItem);
}

CXTComboBoxEx::~CXTComboBoxEx()
{
	DestroyWindow();
}

BOOL CXTComboBoxEx::PreTranslateMessage(MSG* pMsg)
{
	CEdit* pEditCtrl = GetEditCtrl();
	if (pEditCtrl && ::IsWindow(pEditCtrl->m_hWnd))
	{
		if ( pMsg->message == WM_KEYDOWN )
		{
			if ( ::GetKeyState( VK_CONTROL ) < 0 )
			{
				switch ( pMsg->wParam )
				{
				case 'a':
				case 'A':
					pEditCtrl->SetSel(0,-1);
					return TRUE;
				case 'c':
				case 'C':
					pEditCtrl->Copy();
					return TRUE;
				case 'x':
				case 'X':
					pEditCtrl->Cut();
					return TRUE;
				case 'v':
				case 'V':
					pEditCtrl->Paste();
					return TRUE;
				}
			}
			else
			{
				if ( ::GetKeyState( VK_CLEAR ) )
				{
					pEditCtrl->Clear();
					return TRUE;
				}
			}
		}
	}

	return CComboBox::PreTranslateMessage(pMsg);
}

IMPLEMENT_DYNAMIC(CXTComboBoxEx, CComboBox)
