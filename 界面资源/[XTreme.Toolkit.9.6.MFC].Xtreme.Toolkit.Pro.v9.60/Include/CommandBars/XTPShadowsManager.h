// XTPShadowsManager.h : interface for the CXTPShadowsManager class.
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
#if !defined(__XTPSHADOWSMANAGER_H__)
#define __XTPSHADOWSMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "XTPHookManager.h"


//#define NOALPHASHADOW

class CXTPControlPopup;
class CXTPCommandBar;


//===========================================================================
// Summary:
//     CXTPShadowsManager is standalone class used to manage CommandBars' shadows.
//===========================================================================
class _XTP_EXT_CLASS CXTPShadowsManager
{
private:
	typedef BOOL(WINAPI *LPFNUPDATELAYEREDWINDOW)
		(
		HWND hwnd,             // Handle to layered window
		HDC hdcDst,            // Handle to screen DC
		POINT *pptDst,         // New screen position
		SIZE *psize,           // New size of the layered window
		HDC hdcSrc,            // Handle to surface DC
		POINT *pptSrc,         // Layer position
		COLORREF crKey,        // Color key
		BLENDFUNCTION *pblend, // Blend function
		DWORD dwFlags          // Options
		);

	class CShadowList;

private:
	class CShadowWnd : public CWnd, public CXTPHookManagerHookAble
	{
	public:

		//-------------------------------------------------------------------------
		// Summary:
		//     Constructs a CShadowWnd object.
		//-------------------------------------------------------------------------
		CShadowWnd();

	public:
		BOOL  Create(BOOL bHoriz, CRect rcWindow);
		void LongShadow(CShadowList* pList);
		BOOL ExcludeRect(CRect rcExclude);

	private:
		DECLARE_MESSAGE_MAP()

		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		afx_msg void OnPaint();
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg UINT OnNcHitTest(CPoint point);

	private:
		virtual int OnHookMessage(HWND hWnd, UINT nMessage, WPARAM& wParam, LPARAM& lParam, LRESULT& lResult);
		UINT Factor(int& nRed, int& nGreen, int& nBlue, double dFactor);
		BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

	private:
		static BOOL m_bAlphaShadow;

		BOOL m_bHoriz;
		BOOL m_bOfficeLikeShadow;
		CXTPCommandBar* m_pCommandBar;

		friend class CXTPShadowsManager;
	};


	//===========================================================================
	// Summary:
	//     Shadow list
	//===========================================================================
	class _XTP_EXT_CLASS CShadowList : public CList<CShadowWnd*, CShadowWnd*>
	{
	public:

		void AddShadow(CShadowWnd* pShadow)
		{
			pShadow->LongShadow(this);
			AddTail(pShadow);
		}

		void RemoveShadow(CShadowWnd* pShadow)
		{
			POSITION pos = Find(pShadow);
			ASSERT(pos);
			RemoveAt(pos);
		}
	};

private:
	//===========================================================================
	// Summary:
	//     Constructs a CXTPShadowsManager object.
	//===========================================================================
	CXTPShadowsManager(void);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPShadowsManager object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	~CXTPShadowsManager(void);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Check the system alpha shadow ability.
	// Returns:
	//     TRUE if alpha shadow available; otherwise returns FALSE
	//-----------------------------------------------------------------------
	BOOL AlphaShadow()
	{
		#ifdef NOALPHASHADOW
			return FALSE;
		#endif

		return (UpdateLayeredWindow != NULL);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the command bar  shadow.
	// Parameters:
	//     pCommandBar - Points to a CXTPCommandBar object
	//     rcExclude - Excluded rectangle.
	//-----------------------------------------------------------------------
	void SetShadow(CXTPCommandBar* pCommandBar, const CRect& rcExclude = CRect(0, 0, 0, 0));

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the control shadow.
	// Parameters:
	//     pControl - Points to a CXTPControlPopup object
	//-----------------------------------------------------------------------
	void SetShadow(CXTPControlPopup* pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes shadows for the command bar.
	// Parameters:
	//     pCommandBar - Points to a CXTPCommandBar object
	//-----------------------------------------------------------------------
	void RemoveShadow(CXTPCommandBar* pCommandBar);

private:
	void DestroyShadow(CShadowWnd*);
	CShadowWnd* CreateShadow(BOOL bHoriz, CRect rc, CRect rcExclude, CXTPCommandBar* pCommandBar);


private:
	LPFNUPDATELAYEREDWINDOW UpdateLayeredWindow;
	CShadowList m_lstShadow;
	static CXTPShadowsManager s_managerInstance;

	friend class CShadowWnd;
	friend class CXTPShadowsManager* XTPShadowsManager();
};

AFX_INLINE CXTPShadowsManager* XTPShadowsManager() {
	return &CXTPShadowsManager::s_managerInstance;
}
AFX_INLINE BOOL CXTPShadowsManager::CShadowWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

#endif //#if !defined(__XTPSHADOWSMANAGER_H__)
