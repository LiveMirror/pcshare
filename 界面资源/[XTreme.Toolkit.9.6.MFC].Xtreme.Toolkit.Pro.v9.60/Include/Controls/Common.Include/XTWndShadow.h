// XTWndShadow.h: interface for the CXTShadowsManager class.
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
#if !defined(__XTWNDSHADOW_H__)
#define __XTWNDSHADOW_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTShadowWnd;
class CXTShadowsManager;

// ----------------------------------------------------------------------
// Summary:
//     The XTShadowsManager function is used for access to the
//     CXTShadowsManager class.
// Remarks:
//     Call this member function to access CXTShadowsManager members. Since
//     this class is designed as a single instance object you can only
//     access its members through this method. You cannot directly
//     instantiate an object of type CXTShadowsManager.
// Returns:
//     A pointer to the one and only CXTShadowsManager instance.
// Example:
// <code>
// XTShadowsManager()-\>SetShadow(m_hWnd, m_rcExclude);
// </code>
// See Also:
//     CXTShadowsManager
// ----------------------------------------------------------------------
_XT_EXT_CLASS CXTShadowsManager* AFX_CDECL XTShadowsManager();

// ----------------------------------------------------------------------
// Summary:
//     The CXTShadowsManager class is used to manage stationary window
//     shadows.
// Remarks:
//     The CXTShadowsManager class is used to manage shadows for floating
//     stationary windows, it is not intended for use with windows that
//     are movable. CXTShadowsManager is a singleton class that can only
//     be instantiated by using the XTShadowsManager function.
// Example:
//     See SetShadow.
// See Also:
//     XTShadowsManager, SetShadow
// ----------------------------------------------------------------------
class _XT_EXT_CLASS CXTShadowsManager
{
	// private constructor
	CXTShadowsManager(void);

public:

	// ------------------------------------------------------------
	// Summary:
	//     Destroys a CXTShadowsManager object, handles cleanup and
	//     deallocation
	// ------------------------------------------------------------
	~CXTShadowsManager(void);

public:
	// ----------------------------------------------------------------------
	// Summary:
	//     Call this member function to display a shadow for a stationary
	//     popup window.
	// Parameters:
	//     hWnd -       Handle to popup window.
	//     rcExclude -  Size of area to exclude from shadow rendering.
	//     rcWnd -      Size of popup window.
	// Remarks:
	//     The first version of SetShadow will render a shadow for the popup
	//     window around the area specified by rcWnd. The second version will
	//     render a shadow for the popup window but will exclude the area
	//     specified by rcExclude from shadow rendering.
	// Example:
	//     The following is an example of using the SetShadow method.
	// <code>
	// XTShadowsManager()-\>SetShadow(m_hWnd, m_rcExclude);
	// </code>
	// ----------------------------------------------------------------------
	void SetShadow(CRect rcWnd, HWND hWnd);
	void SetShadow(HWND hWnd, const CRect& rcExclude=CRect(0,0,0,0)); //<combine CXTShadowsManager::SetShadow@CRect@HWND>

private:
	typedef BOOL (WINAPI *LPFNUPDATELAYEREDWINDOW)(HWND hwnd, HDC hdcDst, POINT *pptDst, SIZE *psize, HDC hdcSrc, POINT *pptSrc, COLORREF crKey, BLENDFUNCTION *pblend, DWORD dwFlags);

	class CShadowList : public CList<CXTShadowWnd*, CXTShadowWnd*>
	{
	public:
		void AddShadow(CXTShadowWnd* pShadow);
		void RemoveShadow(CXTShadowWnd* pShadow);
		friend class CXTShadowWnd;
	};

private:
	BOOL AlphaShadow();
	BOOL PseudoShadow();
	void DestroyShadow(CXTShadowWnd*);
	CXTShadowWnd* CreateShadow(BOOL bHoriz, HWND hWnd, CRect rcExclude);
	CXTShadowWnd* CreateShadow(BOOL bHoriz, CRect rcWnd, CRect rcExclude, HWND hWnd);


private:
	CShadowList                 m_lstShadow;
	CShadowList                 m_lstPool;
	static CXTShadowsManager    m_instance;
	LPFNUPDATELAYEREDWINDOW     UpdateLayeredWindow;

	friend CXTShadowsManager* AFX_CDECL XTShadowsManager();
	friend class CXTShadowWnd;
};

//{{AFX_CODEJOCK_PRIVATE
class CXTShadowWnd : public CWnd // private class used internally.
{
	CXTShadowWnd();

protected:
	BOOL Create(BOOL bHoriz, CRect rcWindow);
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	void LongShadow(CXTShadowsManager::CShadowList* pList);
	BOOL ExcludeRect(CRect rcExclude);

protected:
	int OnHookMessage(UINT nMessage, WPARAM& wParam, LPARAM& lParam);

	void DrawPseudoShadow(CDC* pDC, CRect& rcClient);
	void ComputePseudoShadow(CDC* pDC, CRect& rcShadow);
	void CreatePseudoShadow();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnParentPosChanged();
	afx_msg void OnTimer(UINT_PTR nID);
	afx_msg void OnSelfDestroy();



protected:
	int         m_nWidth;
	BOOL        m_bAutoPosition;
	BOOL        m_bHoriz;
	BOOL        m_bAlphaShadow;
	BOOL        m_bPseudoShadow;
	HWND        m_hwndHook;
	CRect       m_rcExclude;
	CXTMemDC*   m_pShadowDC;
	CXTWndHook* m_pHook;


	friend class CXTShadowsManager;
	friend class CXTShadowsManager::CShadowList;
	friend class CXTShadowHook;
};

AFX_INLINE BOOL CXTShadowWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
//}}AFX_CODEJOCK_PRIVATE

#endif //#if !defined(__XTWNDSHADOW_H__)
