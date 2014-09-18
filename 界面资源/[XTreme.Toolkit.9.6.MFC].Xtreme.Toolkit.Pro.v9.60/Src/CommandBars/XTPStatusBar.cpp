// XTPStatusBar.cpp : implementation of the CXTPStatusBar class.
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
#include "XTPDrawHelpers.h"
#include "XTPColorManager.h"

#include "XTPStatusBar.h"
#include "XTPPaintManager.h"
#include "XTPToolBar.h"
#include "XTPCommandBars.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPStatusBar::CXTPStatusBar()
{
	m_pCommandBars = 0;
	m_bCommandBarsTheme = TRUE;

	CXTPPaintManager::CNonClientMetrics ncm;
	m_fontStatus.CreateFontIndirect(&ncm.lfStatusFont );
}


CXTPStatusBar::~CXTPStatusBar()
{
	for (int i = 0; i < m_arControls.GetSize(); i++ )
	{
		if( m_arControls[i]->pWnd && ::IsWindow(m_arControls[i]->pWnd->m_hWnd) ) {
			m_arControls[i]->pWnd->CloseWindow();
		}

		if( m_arControls[i]->bAutoDelete ) {
			SAFE_DELETE (m_arControls[i]->pWnd);
		}

		SAFE_DELETE (m_arControls[i]);
	}
}


BOOL CXTPStatusBar::SetIndicators (const UINT* lpIDArray, int nIDCount)
{
	SetFont(&m_fontStatus, FALSE);

	if ( !CStatusBar::SetIndicators (lpIDArray, nIDCount) )
	{
		return FALSE;
	}
	SendMessage (SB_SETMINHEIGHT, 20);
	return TRUE;
}

BEGIN_MESSAGE_MAP(CXTPStatusBar, CStatusBar)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
END_MESSAGE_MAP()


CXTPPaintManager* CXTPStatusBar::GetPaintManager()
{
	if (!m_bCommandBarsTheme)
		return NULL;

	if (m_pCommandBars)
		return m_pCommandBars->GetPaintManager();
	return XTPPaintManager();
}

void CXTPStatusBar::OnPaint ()
{
	CXTPPaintManager* pPaintManager = GetPaintManager();

	if (pPaintManager == NULL)
	{
		Default();
		return;
	}

	CPaintDC dcPaint (this);
	CXTPClientRect rcClient(this);
	CXTPBufferDC dc(dcPaint, rcClient);

	pPaintManager->FillStatusBar(&dc, this);

	CXTPFontDC font(&dc, GetFont());

	for (int i = 0; i < m_nCount; i++ )
	{
		pPaintManager->DrawStatusBarPane(&dc, this, i);
	}

	if (((GetParent()->GetStyle() & WS_THICKFRAME) != 0) &&
		(GetStyle() & SBARS_SIZEGRIP) && !::IsZoomed(::GetParent(m_hWnd)))
		DrawStretch(&dc, rcClient);
}

BOOL CXTPStatusBar::OnEraseBkgnd(CDC* /*pDC*/)
{
	CXTPPaintManager* pPaintManager = GetPaintManager();

	if (pPaintManager == NULL)
	{
		return (BOOL)Default();
	}

	return TRUE;
}

void CXTPStatusBar::DrawStretch(CDC* pDC, CRect& rcClient)
{
	GetPaintManager()->DrawStatusBarGripper(pDC, rcClient);
}



/////////////////////////////////////////////////////////////////////////////
// CXTPStatusBar message handlers

BOOL CXTPStatusBar::GetPaneInfoEx(int nIndex, XTP_STATUSPANE* pSBP)
{
	if( nIndex < m_nCount  && nIndex >= 0 )
	{
		GetPaneInfo (nIndex,  pSBP->nID, pSBP->nStyle, pSBP->cxText);
		GetPaneText (nIndex , pSBP->strText);
		return TRUE;
	}
	return FALSE;
}

BOOL CXTPStatusBar::SetPaneInfoEx(int nIndex, XTP_STATUSPANE* pSBP)
{
	if( nIndex < m_nCount  && nIndex >= 0 )
	{
		SetPaneInfo (nIndex, pSBP->nID, pSBP->nStyle, pSBP->cxText);
		SetPaneText (nIndex, pSBP->strText);
		return TRUE;
	}
	return FALSE;
}

void CXTPStatusBar::SetPaneWidth(int nIndex, int cxText)
{
	XTP_STATUSPANE sbp;
	GetPaneInfoEx(nIndex, &sbp);
	sbp.cxText = cxText;
	SetPaneInfoEx(nIndex, &sbp);
}

BOOL CXTPStatusBar::RemoveIndicator(UINT nID)
{
	if (CommandToIndex(nID) == -1)
	{
		TRACE1("WARNING: Pane ID %d was not found in the status bar.\n", nID);
		return FALSE;
	}

	// initialize an array of pane pointers minus the
	// one to be removed.
	CArray<XTP_STATUSPANE*,XTP_STATUSPANE*> sbpArray;
	int nIndex;
	for (nIndex = 0; nIndex < m_nCount; ++nIndex)
	{
		XTP_STATUSPANE* pOldSBP = GetPanePtr(nIndex);

		if (pOldSBP->nID == nID)
			continue;

		XTP_STATUSPANE* pNewSBP = new XTP_STATUSPANE;

		pNewSBP->cxText  = pOldSBP->cxText;
		pNewSBP->nFlags  = pOldSBP->nFlags;
		pNewSBP->nID     = pOldSBP->nID;
		pNewSBP->nStyle  = pOldSBP->nStyle;
		pNewSBP->strText = pOldSBP->strText;
		sbpArray.Add(pNewSBP);
	}

	// allocate a new indicator array and initialize the value for each.
	UINT* lpIDArray = new UINT[sbpArray.GetSize()];
	for (nIndex = 0; nIndex < sbpArray.GetSize(); ++nIndex) {
		lpIDArray[nIndex] = sbpArray[nIndex]->nID;
	}

	// set the indicators and free all allocated memory.
	SetIndicators(lpIDArray, (int)sbpArray.GetSize());
	for (nIndex = 0; nIndex < sbpArray.GetSize(); ++nIndex)
	{
		XTP_STATUSPANE* pSBP = sbpArray[nIndex];
		SetPaneInfoEx(nIndex, pSBP);
		SAFE_DELETE (pSBP);
	}

	int i;
	for ( i = 0; i < m_arControls.GetSize(); i++ )
	{
		if (m_arControls[i]->nID == nID)
		{
			if( m_arControls[i]->pWnd && ::IsWindow(m_arControls[i]->pWnd->m_hWnd) ) {
				m_arControls[i]->pWnd->DestroyWindow();
			}

			if( m_arControls[i]->bAutoDelete ) {
				SAFE_DELETE (m_arControls[i]->pWnd);
			}

			SAFE_DELETE (m_arControls[i]);
			m_arControls.RemoveAt(i);

			break;
		}
	}

	sbpArray.RemoveAll();
	SAFE_DELETE_AR (lpIDArray);
	PositionControls();

	return TRUE;
}

BOOL CXTPStatusBar::AddIndicator(UINT nID, int nIndex)
{
	if (nIndex < 0 || nIndex > m_nCount)
	{
		TRACE0("WARNING: Pane index out of range.\n");
		return FALSE;
	}

	if (CommandToIndex(nID) != -1)
	{
		TRACE1("WARNING: Pane ID %d already exists in the status bar.\n", nID);
		return FALSE;
	}

	// initialize an array of pane pointers minus the
	// one to be removed.
	CArray<XTP_STATUSPANE*,XTP_STATUSPANE*> sbpArray;
	int iIndex;
	for (iIndex = 0; iIndex < m_nCount+1; ++iIndex)
	{
		XTP_STATUSPANE* pNewSBP = new XTP_STATUSPANE;

		if (iIndex == nIndex)
		{
			pNewSBP->cxText = 0;
			pNewSBP->nID    = nID;
			pNewSBP->nStyle = SBPS_NORMAL;
		}
		else
		{
			int idx = iIndex;
			if (iIndex > nIndex)
				--idx;

			XTP_STATUSPANE* pOldSBP = GetPanePtr(idx);
			pNewSBP->cxText  = pOldSBP->cxText;
			pNewSBP->nFlags  = pOldSBP->nFlags;
			pNewSBP->nID     = pOldSBP->nID;
			pNewSBP->nStyle  = pOldSBP->nStyle;
			pNewSBP->strText = pOldSBP->strText;
		}

		sbpArray.Add(pNewSBP);
	}

	// allocate a new indicator array and initialize the value for each.
	UINT* lpIDArray = new UINT[sbpArray.GetSize()];
	for (iIndex = 0; iIndex < sbpArray.GetSize(); ++iIndex) {
		lpIDArray[iIndex] = sbpArray[iIndex]->nID;
	}

	// set the indicators and free all allocated memory.
	SetIndicators(lpIDArray, (int)sbpArray.GetSize());
	for (iIndex = 0; iIndex < sbpArray.GetSize(); ++iIndex)
	{
		XTP_STATUSPANE* pSBP = sbpArray[iIndex];
		SetPaneInfoEx(iIndex, pSBP);
		SAFE_DELETE (pSBP);
	}

	sbpArray.RemoveAll();
	SAFE_DELETE_AR (lpIDArray);
	PositionControls();

	return TRUE;
}

void CXTPStatusBar::PositionControls()
{
	CRect rcClient;
	GetClientRect(&rcClient);

	int cx = ::GetSystemMetrics(SM_CXEDGE);

	for (int i = 0; i < m_arControls.GetSize(); i++ )
	{
		int   iIndex  = CommandToIndex(m_arControls[i]->nID);
		DWORD dwStyle = GetPaneStyle(iIndex);
		CWnd* pWnd    = m_arControls[i]->pWnd;

		CRect rcPane;
		GetItemRect(iIndex, &rcPane);

		// WINBUG: CStatusBar::GetItemRect(...) does not return the correct size
		// of the last item in the status bar in most cases, so we need to calculate
		// the width ourself...

		if (iIndex == (m_nCount-1))
		{
			if ((dwStyle & SBPS_STRETCH ) == 0)
			{
				UINT nID;
				UINT nStyle;
				int  cxWidth;
				GetPaneInfo(iIndex, nID, nStyle, cxWidth);

				rcPane.right = rcPane.left + cxWidth + (cx*3);
			}
			else
			{
				rcPane.right = rcClient.right;

				if ((GetStyle() & SBARS_SIZEGRIP) == SBARS_SIZEGRIP) {
					rcPane.right -= (::GetSystemMetrics(SM_CXSMICON) + cx);
				}
			}
		}

		if ((GetPaneStyle (iIndex) & SBPS_NOBORDERS) == 0) {
			rcPane.DeflateRect(cx,cx);
		}

		if (pWnd && ::IsWindow(pWnd->m_hWnd))
			pWnd->MoveWindow(rcPane);
	}
}

void CXTPStatusBar::OnSize(UINT nType, int cx, int cy)
{
	CStatusBar::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	PositionControls();
	Invalidate(FALSE);
}

BOOL CXTPStatusBar::AddControl(CWnd* pWnd, UINT nID, BOOL bAutoDelete)
{
	if (CommandToIndex (nID) == -1) {
		return FALSE;
	}

	XTP_STATUSPANE_CTRL* pSBPCtrl = new XTP_STATUSPANE_CTRL;
	pSBPCtrl->nID         = nID;
	pSBPCtrl->pWnd        = pWnd;
	pSBPCtrl->bAutoDelete = bAutoDelete;

	m_arControls.Add(pSBPCtrl);
	PositionControls();

	return TRUE;
}

XTP_STATUSPANE_CTRL* CXTPStatusBar::GetPaneControl(UINT nID)
{
	int i;
	for (i = 0; i < m_arControls.GetSize(); i++ )
	{
		if (nID == m_arControls[i]->nID)
			return m_arControls[i];
	}

	return NULL;
}

XTP_STATUSPANE* CXTPStatusBar::GetPanePtr(int nIndex) const
{
	ASSERT((nIndex >= 0 && nIndex < m_nCount) || m_nCount == 0);
	return ((XTP_STATUSPANE*)m_pData) + nIndex;
}


LRESULT CXTPStatusBar::OnIdleUpdateCmdUI(WPARAM wParam, LPARAM)
{
	// handle delay hide/show
	BOOL bVis = GetStyle() & WS_VISIBLE;
	UINT swpFlags = 0;
	if ((m_nStateFlags & delayHide) && bVis)
		swpFlags = SWP_HIDEWINDOW;
	else if ((m_nStateFlags & delayShow) && !bVis)
		swpFlags = SWP_SHOWWINDOW;
	m_nStateFlags &= ~(delayShow|delayHide);
	if (swpFlags != 0)
	{
		SetWindowPos(NULL, 0, 0, 0, 0, swpFlags|
			SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE);
	}

	// the style must be visible and if it is docked
	// the dockbar style must also be visible
	if ((GetStyle() & WS_VISIBLE) &&
		(m_pDockBar == NULL || (m_pDockBar->GetStyle() & WS_VISIBLE)))
	{
		// MFCBUG: removed check for IsFrameWnd so we can update statusbar
		// for dialog based applications as well as doc/view apps.
		CFrameWnd* pTarget = (CFrameWnd*)GetOwner();
		if (pTarget == NULL)
			pTarget = (CFrameWnd*)GetParent();
		if (pTarget != NULL)
			OnUpdateCmdUI(pTarget, (BOOL)wParam);
	}
	return 0L;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPStatusBarBasePane
/////////////////////////////////////////////////////////////////////////////
CXTPStatusBarBasePane::CXTPStatusBarBasePane()
{
}

CXTPStatusBarBasePane::~CXTPStatusBarBasePane()
{
}

IMPLEMENT_DYNCREATE(CXTPStatusBarBasePane, CWnd)

BEGIN_MESSAGE_MAP(CXTPStatusBarBasePane, CWnd)
	//{{AFX_MSG_MAP(CXTPStatusBarBasePane)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CXTPStatusBarBasePane::OnPaint()
{
	// background is already filled in gray
	CPaintDC dc(this);

	// Get the client rect.
	CXTPClientRect rcClient(this);

	CXTPBufferDC memDC(dc, rcClient);
	memDC.FillSolidRect(rcClient, GetXtremeColor(COLOR_3DFACE));

	// Draw the logo text
	DoPaint(&memDC, rcClient);
}

BOOL CXTPStatusBarBasePane::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

BOOL CXTPStatusBarBasePane::Create(LPCTSTR lpszWindowName, CWnd* pParentWnd, DWORD dwStyle/*=WS_CHILD|WS_VISIBLE*/, UINT nID/*=0xffff*/)
{
	CString wndClass = ::AfxRegisterWndClass(CS_BYTEALIGNCLIENT);

	if(!CWnd::Create(wndClass, lpszWindowName, dwStyle,
		CRect(0,0,0,0), pParentWnd, nID)) {
		return FALSE;
	}

	m_strWindowText = lpszWindowName;

	return TRUE;
}

void CXTPStatusBarBasePane::DoPaint(CDC* /*pDC*/, CRect& /*rcClient*/)
{
}

/////////////////////////////////////////////////////////////////////////////
// CXTPStatusBarLogoPane
/////////////////////////////////////////////////////////////////////////////
CXTPStatusBarLogoPane::CXTPStatusBarLogoPane()
{
	CXTPPaintManager::CNonClientMetrics ncm;
	ncm.lfMenuFont.lfWeight = FW_BOLD;

	SetLogoFont(ncm.lfMenuFont);
}

CXTPStatusBarLogoPane::~CXTPStatusBarLogoPane()
{
	m_Font.DeleteObject();
}

BEGIN_MESSAGE_MAP(CXTPStatusBarLogoPane, CXTPStatusBarBasePane)
	//{{AFX_MSG_MAP(CXTPStatusBarLogoPane)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CXTPStatusBarLogoPane::SetLogoFont(LPCTSTR lpszFontName, int nHeight/*=24*/,
	int nWeight/*=FW_BOLD*/, BOOL bItalic/*=TRUE*/, BOOL bUnderline/*=FALSE*/)
{
	if(m_Font.m_hObject) {
		m_Font.DeleteObject();
	}

	m_Font.CreateFont(nHeight, 0, 0, 0, nWeight, (BYTE)bItalic,
		(BYTE)bUnderline, 0,0,0,0,0,0, lpszFontName);
	SetTextSize();
}

void CXTPStatusBarLogoPane::SetLogoFont(LOGFONT& logFont)
{
	if(m_Font.m_hObject) {
		m_Font.DeleteObject();
	}

	m_Font.CreateFontIndirect(&logFont);
	SetTextSize();
}

void CXTPStatusBarLogoPane::SetTextSize()
{
	CWindowDC dc(NULL);
	CFont* pOldFont = dc.SelectObject(&m_Font);
	m_sizeText = dc.GetTextExtent(m_strWindowText);
	dc.SelectObject(pOldFont);
}

void CXTPStatusBarLogoPane::DoPaint(CDC* pDC, CRect& rcClient)
{
	// Determine the size of the logo text.
	CSize sizeText = pDC->GetTextExtent(m_strWindowText);

	// Calculate the top "y" location.
	int y = (rcClient.Height()-sizeText.cy)/2;

	// Resize the rect to reflect the actual text size.
	rcClient.top = y;
	rcClient.bottom = rcClient.top + sizeText.cy;

	// Set the text color.
	pDC->SetTextColor(GetXtremeColor(COLOR_3DSHADOW));

	// Draw text
	pDC->SetBkMode(TRANSPARENT);

	CXTPFontDC fnt(pDC, &m_Font);
	pDC->DrawText(m_strWindowText, &rcClient,
		DT_VCENTER | DT_CENTER | DT_END_ELLIPSIS | DT_SINGLELINE);
}

/////////////////////////////////////////////////////////////////////////////
// CXTPStatusBarIconPane
/////////////////////////////////////////////////////////////////////////////

CXTPStatusBarIconPane::CXTPStatusBarIconPane()
{
	m_hIcon = NULL;
	m_sizeIcon.cx =  ::GetSystemMetrics(SM_CXSMICON);
	m_sizeIcon.cy = ::GetSystemMetrics(SM_CYSMICON);
}

CXTPStatusBarIconPane::~CXTPStatusBarIconPane()
{
	if (m_hIcon)
	{
		DestroyIcon(m_hIcon);
	}
}

BEGIN_MESSAGE_MAP(CXTPStatusBarIconPane, CXTPStatusBarBasePane)
	//{{AFX_MSG_MAP(CXTPStatusBarIconPane)
	ON_WM_ENABLE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CXTPStatusBarIconPane::DoPaint(CDC* pDC, CRect& rcClient)
{
	if (m_hIcon)
	{
		// determine the x-y location of the icon
		CPoint point;
		point.x = (rcClient.Width() -m_sizeIcon.cx)/2;
		point.y = (rcClient.Height()-m_sizeIcon.cy)/2;

		// draw the icon associated with this pane.
		if (IsWindowEnabled())
		{
			pDC->DrawState(point, m_sizeIcon, m_hIcon,
				DST_ICON | DSS_NORMAL, (HBRUSH)NULL);
		}
		else
		{
			CBrush brush( GetXtremeColor(COLOR_3DSHADOW) );
			pDC->DrawState(point, m_sizeIcon, m_hIcon,
				DST_ICON | DSS_MONO, (HBRUSH)brush);
		}
	}
}

void CXTPStatusBarIconPane::SetPaneIcon(LPCTSTR lpszIconID)
{
	// Locate the resource handle for the icon id.
	HINSTANCE hInstance = AfxFindResourceHandle(lpszIconID, RT_GROUP_ICON);

	if (m_hIcon)
	{
		DestroyIcon(m_hIcon);
	}

	// Load the image for the icon pane.
	m_hIcon = ( HICON )::LoadImage( hInstance, lpszIconID, IMAGE_ICON,
		::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR );
}

void CXTPStatusBarIconPane::SetPaneIcon(int nIconID)
{
	SetPaneIcon(MAKEINTRESOURCE(nIconID));
}

void CXTPStatusBarIconPane::OnEnable(BOOL bEnable)
{
	CXTPStatusBarBasePane::OnEnable(bEnable);

	// Refresh the icon to indicate state.
	Invalidate();
	UpdateWindow();
}
