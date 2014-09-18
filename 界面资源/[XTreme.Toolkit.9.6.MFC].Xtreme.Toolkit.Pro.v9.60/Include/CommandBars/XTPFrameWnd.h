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
#if !defined(__XTPFRAMEWND_H__)
#define __XTPFRAMEWND_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "XTPCommandBars.h"
#include "XTPCommandBarsResource.h"
#include "XTPPaintManager.h"
#include "XTPControlComboBox.h"

//===========================================================================
// Summary:
//     CXTPFrameWndBase is a stand alone class. It represents the parent
//     class for a CXTPMDIFrameWnd and CXTPFrameWnd classes .
//===========================================================================
template <class TBase>
class CXTPFrameWndBase : public TBase
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPFrameWndBase object
	//-----------------------------------------------------------------------
	CXTPFrameWndBase()
	{
		m_pCommandBars = 0;
	}
	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFrameWndBase object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	~CXTPFrameWndBase()
	{
		if (m_pCommandBars)
		{
			m_pCommandBars->InternalRelease();
		}
	}
	//-----------------------------------------------------------------------
	// Summary:
	//     Creates command bars.
	// Parameters:
	//     pCommandBarsClass - Custom runtime class of CommandBars. It can be used if you want to override
	//                         some methods of CXTPCommandBars class.
	// Returns:
	//     Nonzero if successful; otherwise 0.
	//-----------------------------------------------------------------------
	BOOL InitCommandBars(CRuntimeClass* pCommandBarsClass = RUNTIME_CLASS(CXTPCommandBars))
	{
		if (XTPPaintManager() == NULL)
		{
			CXTPPaintManager::SetTheme(xtpThemeOffice2000);
		}

		ASSERT(pCommandBarsClass->IsDerivedFrom(RUNTIME_CLASS(CXTPCommandBars)));
		m_pCommandBars =  (CXTPCommandBars*) pCommandBarsClass->CreateObject();
		ASSERT(m_pCommandBars);
		m_pCommandBars->SetSite(this);
		m_pCommandBars->EnableDocking();
		return TRUE;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will re-dock a toolbar specified by 'pBarToDock'
	//     to the right of a newly docked toolbar specified by 'pBarOnLeft'.
	// Parameters:
	//     pBarToDock - A CXTPToolBar pointer to the toolbar to be docked.
	//     pBarOnLeft - A CXTPToolBar pointer to the already docked toolbar.
	//-----------------------------------------------------------------------
	void DockRightOf(CXTPToolBar* pBarToDock, CXTPToolBar* pBarOnLeft)
	{
		RecalcLayout(TRUE);
		CXTPWindowRect rcBar(pBarOnLeft);
		rcBar.OffsetRect(rcBar.Width(), 0);
		GetCommandBars()->DockCommandBar(pBarToDock, rcBar, pBarOnLeft->GetDockBar());
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to save the state information to the registry
	//     or .INI file.
	// Parameters:
	//     lpszProfileName - Points to a null-terminated string that specifies
	//                       the name of a section in the initialization file
	//                       or a key in the Windows registry where state
	//                       information is stored.
	//-----------------------------------------------------------------------
	void SaveCommandBars(LPCTSTR lpszProfileName)
	{
		if (m_pCommandBars)
		{
			m_pCommandBars->SaveOptions(lpszProfileName);
			m_pCommandBars->SaveBarState(lpszProfileName);
		}
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to retrieve state information from the registry
	//     or .INI file.
	// Parameters:
	//     lpszProfileName - Points to a null-terminated string that specifies
	//                       the name of a section in the initialization file
	//                       or a key in the Windows registry where state
	//                       information is stored.
	//     bSilent         - TRUE to disable user notifications when command bars are restore to their original state.
	//-----------------------------------------------------------------------
	void LoadCommandBars(LPCTSTR lpszProfileName, BOOL bSilent = FALSE)
	{
		if (m_pCommandBars)
		{
			m_pCommandBars->LoadOptions(lpszProfileName);
			m_pCommandBars->LoadBarState(lpszProfileName, bSilent);
		}
	}

	//----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve a pointer to the CommandBars object.
	// Returns:
	//     Retrieves Command Bars object.
	//----------------------------------------------------------------------
	CXTPCommandBars* GetCommandBars() { return m_pCommandBars; }

protected:

	//{{AFX_CODEJOCK_PRIVATE
	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		if ((pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
			&& (pMsg->wParam != VK_RETURN && pMsg->wParam != VK_TAB && pMsg->wParam != VK_ESCAPE))
		{
			CWnd* pWnd = CWnd::GetFocus();
			if (pWnd && pWnd->IsKindOf(RUNTIME_CLASS(CXTPEdit)))
				return FALSE;
		}

		if (TBase::PreTranslateMessage(pMsg))
			return TRUE;

		if (m_pCommandBars && m_pCommandBars->PreTranslateFrameMessage(pMsg))
			return TRUE;

		return FALSE;
	}

	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		if (m_pCommandBars && m_pCommandBars->OnFrameWndMsg(message, wParam, lParam, pResult))
			return TRUE;

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

	void OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState)
	{
		if (m_pCommandBars)
		{
			for (int i = 0; i < m_pCommandBars->GetCount(); i++)
			{

				CXTPToolBar* pBar = m_pCommandBars->GetAt(i);
				pBar->OnSetPreviewMode(bPreview);
			}
		}

		TBase::OnSetPreviewMode(bPreview, pState);
	}
//}}AFX_CODEJOCK_PRIVATE
private:
	CXTPCommandBars* m_pCommandBars;
};

//===========================================================================
// Summary:
//     CXTPMDIFrameWnd is a CXTPFrameWndBase derived class.
//===========================================================================
class CXTPMDIFrameWnd : public CXTPFrameWndBase<CMDIFrameWnd>
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMDIFrameWnd object
	//-----------------------------------------------------------------------
	CXTPMDIFrameWnd()
	{

	}

	//{{AFX_CODEJOCK_PRIVATE
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
		AFX_CMDHANDLERINFO* pHandlerInfo)
	{
		if (nID >= XTP_ID_WORKSAPCE_MOVEPREVIOUS && nID <= XTP_ID_WORKSAPCE_NEWVERTICAL)
		{
			CWnd* pWnd = CWnd::FromHandlePermanent(m_hWndMDIClient);
			if (pWnd && pWnd->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
				return TRUE;
		}
		// then pump through normal frame
		return CMDIFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
	}
	//}}AFX_CODEJOCK_PRIVATE
};

//===========================================================================
// Summary:
//     CXTPFrameWnd is a CXTPFrameWndBase derived class.
//===========================================================================
class CXTPFrameWnd : public CXTPFrameWndBase<CFrameWnd>
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPFrameWnd object
	//-----------------------------------------------------------------------
	CXTPFrameWnd()
	{

	}
};

#endif // #if !defined(__XTPFRAMEWND_H__)
