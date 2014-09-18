// XTPFrameWnd.h : interface for the CXTPFrameWnd and CXTPMDIFrameWnd classes.
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
#if !defined(__XTPDIALOGBASE_H__)
#define __XTPDIALOGBASE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "XTPCommandBars.h"
#include "XTPPaintManager.h"

//////////////////////////////////////////////////////////////////////

//===========================================================================
// Summary:
//     CXTPDialogBase is a stand alone class. It represents the parent
//     class for a CXTPDialog class.
//===========================================================================
template <class TBase>
class CXTPDialogBase : public TBase
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDialog object
	// Parameters:
	//     nIDTemplate - Contains the ID number of a dialog-template resource
	//     pParentWnd  - Points to the parent of the DialogBase control
	//-----------------------------------------------------------------------
	CXTPDialogBase(UINT nIDTemplate, CWnd* pParentWnd = NULL)
		: TBase(nIDTemplate, pParentWnd)
	{
		m_pMenuBar = 0;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDialogBase object
	// Parameters:
	//     lpszTemplateName - Contains a null-terminated string that is
	//                        the name of a dialog-template resource
	//     pParentWnd       - Points to the parent of the DialogBase control
	//-----------------------------------------------------------------------
	CXTPDialogBase(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL)
		: TBase(lpszTemplateName, pParentWnd)
	{
		m_pMenuBar = 0;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDialogBase object
	//-----------------------------------------------------------------------
	CXTPDialogBase()
	{
		m_pMenuBar = 0;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPDialogBase object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	~CXTPDialogBase()
	{
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Assigns menu bar of dialog.
	// Parameters:
	//     pMenuBar - Points to a CXTPCommandBar object
	//-----------------------------------------------------------------------
	void SetMenuBar(CXTPCommandBar* pMenuBar)
	{
		m_pMenuBar = pMenuBar;
	}


protected:
//{{AFX_CODEJOCK_PRIVATE
	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		if (!XTPMouseManager()->IsMouseLocked() && !XTPMouseManager()->IsTrackedLock(0))
		{
			if (m_pMenuBar && pMsg->message == WM_SYSKEYDOWN && (HIWORD(pMsg->lParam) & KF_ALTDOWN))
			{

				TCHAR chAccel = (TCHAR)MapVirtualKey((UINT)pMsg->wParam, 2);
				if( chAccel)
				{
					BOOL bSelectOnly;
					int nIndex = m_pMenuBar->FindAccelEx(chAccel, bSelectOnly);
					if (nIndex != -1)
					{
						if (bSelectOnly)
						{
							m_pMenuBar->SetTrackingMode(TRUE, FALSE);
							m_pMenuBar->SetSelected(nIndex, TRUE);
						}
						else m_pMenuBar->SetPopuped(nIndex, TRUE);
						return TRUE;
					}
				}
			}
		}

		if (XTPMouseManager()->IsTrackedLock(0) && pMsg->message == WM_SYSKEYDOWN)
		{
			XTPMouseManager()->SendTrackLost();
			return TRUE;
		}

		if (XTPMouseManager()->IsTrackedLock(0) &&
			(pMsg->message == WM_CONTEXTMENU || pMsg->message == WM_SYSKEYUP))
		{
			return TRUE;
		}

		return TBase::PreTranslateMessage(pMsg);
	}

	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		if (XTPMouseManager()->IsTrackedLock())
		{
			switch(message)
			{
			case WM_ACTIVATEAPP:
				if (!wParam)
					XTPMouseManager()->SendTrackLost();
				return FALSE;
			case WM_SYSCOMMAND:
				if (wParam == 0 || LOWORD(wParam) != 0)
					XTPMouseManager()->SendTrackLost();
				return FALSE;
			}
		}

		if (message == WM_XTP_COMMAND)
		{
			LRESULT lResult = FALSE;
			NMHDR* pNMHDR = (NMHDR*)lParam;

			UINT nID = (UINT)pNMHDR->idFrom;
			int nCode = pNMHDR->code;

			AFX_NOTIFY notify;
			notify.pResult = &lResult;
			notify.pNMHDR = pNMHDR;
			if (OnCmdMsg(nID, MAKELONG(nCode, WM_NOTIFY), &notify, NULL))
			{
				*pResult = lResult;
				return TRUE;
			}
			return FALSE;
		}

		return TBase::OnWndMsg(message, wParam, lParam, pResult);
	}
//}}AFX_CODEJOCK_PRIVATE

private:
	CXTPCommandBar* m_pMenuBar;
};

//===========================================================================
// Summary:
//     CXTPDialog is a CXTPDialogBase derived class. Use this class in your dialog base application.
//===========================================================================
class CXTPDialog : public CXTPDialogBase<CDialog>
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDialog object
	// Parameters:
	//     nIDTemplate - Contains the ID number of a dialog-template resource
	//     pParentWnd  - Points to the parent of the Dialog control
	//-----------------------------------------------------------------------
	CXTPDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL)
		: CXTPDialogBase<CDialog>(nIDTemplate, pParentWnd)
	{

	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDialog object
	// Parameters:
	//     lpszTemplateName - Contains a null-terminated string that is
	//                        the name of a dialog-template resource
	//     pParentWnd       - Points to the parent of the Dialog control
	//-----------------------------------------------------------------------
	CXTPDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL)
		: CXTPDialogBase<CDialog>(lpszTemplateName, pParentWnd)
	{

	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDialog object
	//-----------------------------------------------------------------------
	CXTPDialog()
	{

	}

};

#endif // #if !defined(__XTPDIALOGBASE_H__)
