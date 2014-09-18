// XTPReBar.h : header file
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
#if !defined(__XTPREBAR_H__)
#define __XTPREBAR_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPToolBar;

//{{AFX_CODEJOCK_PRIVATE
#if _MSC_VER < 1200 // MFC 6.0

#ifndef AFX_IDW_REBAR
#define AFX_IDW_REBAR                   0xE804
#endif

class _XTP_EXT_CLASS CXTPReBarBase : public CControlBar
{
	DECLARE_DYNAMIC(CXTPReBarBase)

// Construction
public:
	CXTPReBarBase();
	BOOL Create(CWnd* pParentWnd, DWORD dwCtrlStyle = RBS_BANDBORDERS,
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_TOP,
		UINT nID = AFX_IDW_REBAR);

// Attributes
public:
	int m_iComCtlVersion;
	void GetComCtlVersion();

// Operations
public:
	BOOL AddBar(CWnd* pBar, LPCTSTR pszText = NULL, CBitmap* pbmp = NULL,
		DWORD dwStyle = RBBS_GRIPPERALWAYS | RBBS_FIXEDBMP);
	BOOL AddBar(CWnd* pBar, COLORREF clrFore, COLORREF clrBack,
		LPCTSTR pszText = NULL, DWORD dwStyle = RBBS_GRIPPERALWAYS);

// Implementation
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	virtual CSize CalcDynamicLayout(int nLength, DWORD nMode);
#ifdef _DEBUG
	void EnableDocking(DWORD dwDockStyle);
#endif

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	BOOL _AddBar(CWnd* pBar, REBARBANDINFO* pRBBI);

	//{{AFX_MSG(CReBar)
	afx_msg BOOL OnNcCreate(LPCREATESTRUCT);
	afx_msg void OnHeightChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNcPaint();
	afx_msg void OnNcCalcSize(BOOL, NCCALCSIZE_PARAMS*);
	afx_msg LRESULT OnShowBand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnRecalcParent();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#else
#define CXTPReBarBase CReBar
#endif
//}}AFX_CODEJOCK_PRIVATE

//===========================================================================
// Summary:
//     The CXTPReBar object extends the standard MFC CReBar class to
//     provide extended functionality such as saving and restoring of
//     band layouts.
//===========================================================================
class _XTP_EXT_CLASS CXTPReBar : public CXTPReBarBase
{
	DECLARE_DYNAMIC(CXTPReBar)

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPReBar object.
	//-------------------------------------------------------------------------
	CXTPReBar();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPReBar object, handles cleanup and deallocation.
	//-------------------------------------------------------------------------
	virtual ~CXTPReBar();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to add a band to the rebar. The specified Toolbar
	//     will be inserted into the band.
	// Parameters:
	//     pToolBar - Pointer to the Toolbar object that will be added to
	//                rebar band.
	//     dwStyle  - Contains styles that can be applied to the rebar.
	//                <p>RBBS_BREAK          - The band is on a new line.
	//                <p>RBBS_CHILDEDGE      - The band has an edge at the top and bottom of the child window.
	//                <p>RBBS_FIXEDBMP       - The background bitmap does not move when the band is resized.
	//                <p>RBBS_FIXEDSIZE      - The band cannot be sized. With this style, the sizing grip is not displayed on the band.
	//                <p>RBBS_GRIPPERALWAYS  - The band will always have sizing grip, even if it is the only band in the rebar.
	//                <p>RBBS_HIDDEN         - The band will not be visible.
	//                <p>RBBS_NOVERT         - The band is not displayed when the rebar control uses the CCS_VERT style.
	// Remarks:
	//     The id of the band that contains the toolbar is assigned the same id
	//     as the toolbar.
	// Returns:
	//     TRUE is the bar was added successfully, FALSE if the bar wasn't added.
	//-----------------------------------------------------------------------
	BOOL AddToolBar(CXTPToolBar* pToolBar, DWORD dwStyle = RBBS_GRIPPERALWAYS);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to remove a toolbar within the rebar.  This will
	//     also remove the band that contained the toolbar.
	// Parameters:
	//     pToolBar - Pointer to the toolbar that will be removed.
	//-----------------------------------------------------------------------
	void DeleteToolBar(CXTPToolBar* pToolBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to locate the band that contains a toolbar.
	// Parameters:
	//     pToolBar - Pointer to the toolbar you want to find.
	// Returns: -1 if the toolbar was not found on a band in the rebar.  If found
	//          the Id of the band that contains the toolbar is returned.
	//-----------------------------------------------------------------------
	int FindBand(CXTPToolBar* pToolBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve a pointer to a toolbar that is contained
	//     in the band with the specified id.
	// Parameters:
	//     nBand - Id of a band in the rebar.
	// Returns:
	//     A pointer to the toolbar contained in the specified band.
	//-----------------------------------------------------------------------
	CXTPToolBar* GetToolBar(int nBand);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to retrieve state information from the
	//     registry.
	// Parameters:
	//     lpszProfileName - Points to a null-teminated string that specifies
	//     the name of a key in the Windows registry where state
	//     information is stored.
	//-----------------------------------------------------------------------
	void LoadState( LPCTSTR lpszProfileName );

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to save state information from the
	//     registry.
	// Parameters:
	//     lpszProfileName - Points to a null-teminated string that specifies
	//     the name of a key in the Windows registry where the state
	//     information should be stored.
	//-----------------------------------------------------------------------
	void SaveState( LPCTSTR lpszProfileName );

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function implements the behavior of the Win32 message
	//     RB_SETBANDINFO, as described in the Platform SDK.
	// Parameters:
	//     uBand - Zero-based index of the band to receive the new settings.
	//             prbbi - Pointer to a REBARBANDINFO structure that defines the band to be inserted.
	//             You must set the 'cbSize' member of this structure to sizeof(REBARBANDINFO)
	//             before sending this message.
	// Returns:
	//     Nonzero if successful, otherwise returns zero.
	// Example:
	// <code>
	// int nCount = m_wndReBar.GetReBarCtrl().GetBandCount();
	// CString strText = "Band #:";
	//
	// int i;
	// for (i=0; i<nCount; i++)
	// {
	//     LPREBARBANDINFO prbbi = (LPREBARBANDINFO)alloca(sizeof(REBARBANDINFO));
	//     prbbi->cbSize = sizeof(REBARBANDINFO);
	//
	//     CString strText;
	//     strText.Format("Band #: %d", i);
	//
	//     LPTSTR lpszText = strText.GetBuffer(strText.GetLength());
	//     prbbi->lpText = lpszText;
	//     prbbi->cch = strlen(lpszText) + 1;
	//     prbbi->fMask = RBBIM_TEXT;
	//
	//     m_wndReBar.SetBandInfo(i, prbbi);
	//
	//     strText.ReleaseBuffer();
	// }
	// </code>
	//-----------------------------------------------------------------------
	BOOL SetBandInfo(UINT uBand, REBARBANDINFO* prbbi);

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTPReBar)
	void OnChildSize(NMHDR * pNotifyStruct, LRESULT* result );
	void OnPaint();
	BOOL OnEraseBkgnd(CDC* pDC);
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	//}}AFX_MSG

//}}AFX_CODEJOCK_PRIVATE

private:
	static const LPCTSTR m_lpszStateInfoEntry;
	static const LPCTSTR m_lpszStateInfoFormat;
};

AFX_INLINE BOOL CXTPReBar::SetBandInfo(UINT uBand, REBARBANDINFO* prbbi) {
	return (BOOL)DefWindowProc(RB_SETBANDINFO, uBand, (LPARAM)prbbi);
}
/////////////////////////////////////////////////////////////////////////////

#endif // !defined(__XTPREBAR_H__)
