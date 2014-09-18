// SplitterWndXT.cpp : implementation file
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
#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTSplitterWnd.h"
#include "XTVersionManager.h"
#include "XTColorRef.h"
#include "XTPColorManager.h"
#include "XTSplitterTheme.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SHOW_TOP_BORDER     (m_pParentWnd && m_pParentWnd->IsFrameWnd())

/////////////////////////////////////////////////////////////////////////////
// Visual attributes and other constants

// HitTest return values (values and spacing between values is important)
enum HitTestValue
{
	noHit                   = 0,
	vSplitterBox            = 1,
	hSplitterBox            = 2,
	bothSplitterBox         = 3,        // just for keyboard
	vSplitterBar1           = 101,
	vSplitterBar15          = 115,
	hSplitterBar1           = 201,
	hSplitterBar15          = 215,
	splitterIntersection1   = 301,
	splitterIntersection225 = 525
};

/////////////////////////////////////////////////////////////////////////////
// CXTSplitterWnd

IMPLEMENT_THEME_HOST(CXTSplitterWnd, CXTSplitterTheme)

CXTSplitterWnd::CXTSplitterWnd()
: CXTThemeManagerStyleHost(GetThemeFactoryClass())
{
	m_nHiddenCol    = -1;
	m_nHiddenRow    = -1;
	m_point         = CPoint(-1,-1);
	m_dwxStyle      = XT_SPLIT_NOFULLDRAG;
	m_bFlatSplit    = TRUE;
	m_cxSplitter    = 6;
	m_cySplitter    = 6;
	m_cxSplitterGap = 6;
	m_cySplitterGap = 6;

	// Get system settings for full drag.
	::SystemParametersInfo(SPI_GETDRAGFULLWINDOWS,
		0, &m_bFullDrag, 0);
	m_bForceFullDrag = FALSE;
	m_bClipStyles = FALSE;
}

CXTSplitterWnd::~CXTSplitterWnd()
{
}

IMPLEMENT_DYNAMIC(CXTSplitterWnd, CSplitterWnd)

BEGIN_MESSAGE_MAP(CXTSplitterWnd, CSplitterWnd)
	//{{AFX_MSG_MAP(CXTSplitterWnd)
	ON_WM_SETTINGCHANGE()
	ON_WM_MOUSEMOVE()
	ON_WM_NCHITTEST()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTSplitterWnd message handlers

void CXTSplitterWnd::ShowColumn()
{
	ASSERT_VALID( this );
	if ( m_nCols == m_nMaxCols && m_nHiddenCol == -1)
	{
		return;
	}
	ASSERT( m_nHiddenCol != -1 );

	int nShowCol = m_nHiddenCol;
	m_nHiddenCol = -1;

	int cxNew = m_pColInfo[m_nCols].nCurSize;
	m_nCols++;  // add a column

	ASSERT( m_nCols == m_nMaxCols );

	int nCol;

	// Show the hidden column
	int nRow;
	for ( nRow = 0; nRow < m_nRows; ++nRow )
	{
		CWnd* pPaneShow = GetDlgItem( AFX_IDW_PANE_FIRST + nRow * 16 + m_nCols );
		ASSERT( pPaneShow != NULL );
		pPaneShow->ShowWindow( SW_SHOWNA );

		for ( nCol = m_nCols - 2; nCol >= nShowCol; --nCol )
		{
			CWnd* pPane = GetPane( nRow, nCol );
			ASSERT( pPane != NULL );
			pPane->SetDlgCtrlID( IdFromRowCol( nRow, nCol + 1 ));
		}

		pPaneShow->SetDlgCtrlID( IdFromRowCol( nRow, nShowCol ));
	}

	// new panes have been created -- recalculate layout
	for ( nCol = nShowCol+1; nCol < m_nCols; nCol++ )
		m_pColInfo[nCol].nIdealSize = m_pColInfo[nCol - 1].nCurSize;

	m_pColInfo[nShowCol].nIdealSize = cxNew;
	RecalcLayout();
}

void CXTSplitterWnd::HideColumn(int nColHide)
{
	ASSERT_VALID( this );
	if (m_nHiddenCol != -1)
	{
		// return if the requested one is hidden
		if (m_nHiddenCol == nColHide)
		{
			return;
		}
		ShowColumn();
	}
	ASSERT( m_nCols > 1 );
	ASSERT( nColHide < m_nCols );
	ASSERT( m_nHiddenCol == -1 );
	m_nHiddenCol = nColHide;

	// if the column has an active window -- change it
	int nActiveRow, nActiveCol;
	if( GetActivePane( &nActiveRow, &nActiveCol ) != NULL )
	{
		if( nActiveCol == nColHide )
		{
			if( ++nActiveCol >= m_nCols )
				nActiveCol = 0;
			SetActivePane( nActiveRow, nActiveCol );
		}
	}

	// hide all column panes
	int nRow;
	for ( nRow = 0; nRow < m_nRows; nRow++)
	{
		CWnd* pPaneHide = GetPane(nRow, nColHide);
		ASSERT( pPaneHide != NULL );

		pPaneHide->ShowWindow(SW_HIDE);
		pPaneHide->SetDlgCtrlID( AFX_IDW_PANE_FIRST + nRow * 16 + m_nCols );

		int nCol;
		for ( nCol = nColHide + 1; nCol < m_nCols; nCol++ )
		{
			CWnd* pPane = GetPane( nRow, nCol );
			ASSERT( pPane != NULL );

			pPane->SetDlgCtrlID( IdFromRowCol( nRow, nCol - 1 ));
		}
	}

	m_nCols--;
	m_pColInfo[m_nCols].nCurSize = m_pColInfo[nColHide].nCurSize;
	RecalcLayout();
}

void CXTSplitterWnd::ShowRow()
{
	ASSERT_VALID( this );
	if ( m_nRows == m_nMaxRows && m_nHiddenRow == -1)
	{
		return;
	}
	ASSERT( m_nHiddenRow != -1 );

	int nShowRow = m_nHiddenRow;
	m_nHiddenRow = -1;

	int cyNew = m_pRowInfo[m_nRows].nCurSize;
	m_nRows++;  // add a nRow

	ASSERT( m_nRows == m_nMaxRows );

	int nRow;

	// Show the hidden nRow
	int nCol;
	for ( nCol = 0; nCol < m_nCols; ++nCol )
	{
		CWnd* pPaneShow = GetDlgItem( AFX_IDW_PANE_FIRST + nCol * 16 + m_nRows );
		ASSERT( pPaneShow != NULL );
		pPaneShow->ShowWindow( SW_SHOWNA );

		for ( nRow = m_nRows - 2; nRow >= nShowRow; --nRow )
		{
			CWnd* pPane = GetPane( nRow, nCol );
			ASSERT( pPane != NULL );
			pPane->SetDlgCtrlID( IdFromRowCol( nRow + 1, nCol ));
		}

		pPaneShow->SetDlgCtrlID( IdFromRowCol( nShowRow, nCol ));
	}

	// new panes have been created -- recalculate layout
	for ( nRow = nShowRow+1; nRow < m_nRows; nRow++ )
		m_pRowInfo[nRow].nIdealSize = m_pRowInfo[nRow - 1].nCurSize;

	m_pRowInfo[nShowRow].nIdealSize = cyNew;
	RecalcLayout();
}

void CXTSplitterWnd::HideRow(int nRowHide)
{
	ASSERT_VALID( this );
	if (m_nHiddenRow != -1)
	{
		// return if the requested one is hidden
		if (m_nHiddenRow == nRowHide)
		{
			return;
		}
		ShowRow();
	}
	ASSERT( m_nRows > 1 );
	ASSERT( nRowHide < m_nRows );
	ASSERT( m_nHiddenRow == -1 );
	m_nHiddenRow = nRowHide;

	int nActiveRow, nActiveCol;

	// if the nRow has an active window -- change it
	if( GetActivePane( &nActiveRow, &nActiveCol ) != NULL )
	{
		if( nActiveRow == nRowHide )
		{
			if( ++nActiveRow >= m_nRows )
				nActiveRow = 0;
			SetActivePane( nActiveRow, nActiveCol );
		}
	}

	// hide all nRow panes.
	int nCol;
	for ( nCol = 0; nCol < m_nCols; ++nCol )
	{
		CWnd* pPaneHide = GetPane( nRowHide, nCol );
		ASSERT( pPaneHide != NULL );

		pPaneHide->ShowWindow( SW_HIDE );
		pPaneHide->SetDlgCtrlID( AFX_IDW_PANE_FIRST + nCol * 16 + m_nRows );

		int nRow;
		for ( nRow = nRowHide+1; nRow < m_nRows; ++nRow )
		{
			CWnd* pPane = GetPane( nRow, nCol );
			ASSERT( pPane != NULL );

			pPane->SetDlgCtrlID( IdFromRowCol( nRow-1, nCol ));
		}
	}

	m_nRows--;
	m_pRowInfo[m_nRows].nCurSize = m_pRowInfo[nRowHide].nCurSize;
	RecalcLayout();
}

BOOL CXTSplitterWnd::SwitchView(int nRow, int nCol, CView *pNewView)
{
	CView *pOldView = DYNAMIC_DOWNCAST( CView, GetPane( nRow, nCol ));
	ASSERT_KINDOF( CView, pOldView );

	if( pOldView == pNewView )
		return FALSE;

	int nOldID = pOldView->GetDlgCtrlID();
	int nNewID = pNewView->GetDlgCtrlID();

	// hide the views.
	pOldView->ShowWindow(SW_HIDE);
	pNewView->ShowWindow(SW_HIDE);

	// swap ids.
	pOldView->SetDlgCtrlID(nNewID);
	pNewView->SetDlgCtrlID(nOldID);

	// show the views.
	pOldView->ShowWindow(SW_SHOW);
	pNewView->ShowWindow(SW_SHOW);

	RecalcLayout();

	return TRUE;
}

CView* CXTSplitterWnd::ReplaceView(int nRow, int nCol, CView* pNewView)
{
	CView* pOldView = DYNAMIC_DOWNCAST(CView, GetPane (nRow, nCol));
	ASSERT_KINDOF (CView, pOldView);

	if( pOldView == pNewView)
		return NULL;

	int nCtrlID = pOldView->GetDlgCtrlID();

	// swap ids.
	pOldView->SetDlgCtrlID(0);
	pNewView->SetDlgCtrlID(nCtrlID);

	// show the views.
	pOldView->ShowWindow(SW_HIDE);
	pNewView->ShowWindow(SW_SHOW);

	RecalcLayout();

	return pOldView;
}

CView* CXTSplitterWnd::ReplaceView(int nRow, int nCol, CRuntimeClass *pViewClass)
{
	CView* pOldView = DYNAMIC_DOWNCAST(CView, GetPane (nRow, nCol));
	ASSERT_KINDOF (CView, pOldView);

	if( pOldView->IsKindOf( pViewClass ))
		return NULL;

	// Get pointer to CDocument object so that it can be used in the creation
	// process of the new view
	CDocument* pDocument = pOldView->GetDocument();

	int nCtrlID = pOldView->GetDlgCtrlID();
	int nWidth, nHeight, nMinWidth, nMinHeight;

	GetRowInfo( nRow, nWidth, nMinWidth );
	GetColumnInfo( nCol, nHeight, nMinHeight );

	// Create new view
	CCreateContext contextT;
	contextT.m_pLastView       = NULL;
	contextT.m_pCurrentDoc     = pDocument;
	contextT.m_pNewViewClass   = pViewClass;
	contextT.m_pNewDocTemplate = pDocument ? pDocument->GetDocTemplate() : NULL;
	contextT.m_pCurrentFrame   = NULL;

	CWnd* pWnd;
	TRY
	{
		pWnd = (CWnd*)pViewClass->CreateObject();
		if (pWnd == NULL)
			AfxThrowMemoryException();
	}
	CATCH_ALL(e)
	{
		TRACE0( "Out of memory creating a view.\n" );
		// Note: DELETE_EXCEPTION(e) not required
		return NULL;
	}
	END_CATCH_ALL

	ASSERT_KINDOF(CWnd, pWnd);
	ASSERT(pWnd->m_hWnd == NULL); // not yet created.

	// Create with the right size (wrong position)
	if (!pWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE,
		CRect(0,0,0,0), this, nCtrlID, &contextT))
	{
		TRACE0( "Warning: couldn't create new view.\n" );
		// pWnd will be cleaned up by PostNcDestroy
		return NULL;
	}

	// Hide the old view.
	pOldView->ShowWindow(SW_HIDE);
	pOldView->SetDlgCtrlID(0);

	SetRowInfo (nRow, nWidth, nMinWidth);
	SetColumnInfo (nCol, nHeight, nMinHeight);
	RecalcLayout ();

	CView* pNewView = DYNAMIC_DOWNCAST (CView, GetPane (nRow, nCol));
	ASSERT_KINDOF (CView, pNewView);
	pNewView->OnInitialUpdate();

	return pOldView;
}

void CXTSplitterWnd::SetSplitCursor(int ht)
{
	if (ht == vSplitterBox || ht >= vSplitterBar1 && ht <= vSplitterBar15) {
		SetCursor(XTAuxData().hcurVSplitBar);
	}
	else if (ht == hSplitterBox || ht >= hSplitterBar1 && ht <= hSplitterBar15) {
		SetCursor(XTAuxData().hcurHSplitBar);
	}
	else {
		CSplitterWnd::SetSplitCursor(ht);
	}
}

void CXTSplitterWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CSplitterWnd::OnSettingChange(uFlags, lpszSection);

	if (!m_bForceFullDrag)
	{
		// Get system settings for full drag.
		::SystemParametersInfo(SPI_GETDRAGFULLWINDOWS,
			0, &m_bFullDrag, 0);

		// Force no display of windows contents while dragging,
		// regardless of windows setting.
		if (m_dwxStyle & XT_SPLIT_NOFULLDRAG) {
			m_bFullDrag = FALSE;
		}
	}
	GetTheme()->RefreshMetrics();
}

void CXTSplitterWnd::SetFullDrag(BOOL bFullDrag)
{
	m_bFullDrag = bFullDrag;
	m_bForceFullDrag = TRUE;
}


void CXTSplitterWnd::DrawTracker(const CRect& rect, CBrush* pBrush)
{
	if (m_bFullDrag == FALSE)
	{
		ASSERT_VALID(this);
		ASSERT(!rect.IsRectEmpty());
		ASSERT((GetStyle() & WS_CLIPCHILDREN) == 0);

		CWindowDC dc(this);
		int nSavedDC = dc.SaveDC();

		if (m_dwxStyle & XT_SPLIT_DOTTRACKER)
		{
			CRect rc;
			GetInsideRect(rc);

			if (rect.Width() < rect.Height()) {
				rc.right  = rect.right;
			}
			else {
				rc.bottom = rect.top;
			}

			dc.DrawFocusRect(rc);
		}
		else
		{
			CRect rcTracker(&rect);

			BOOL bVert = rcTracker.Height() > rcTracker.Width();

			if (bVert)
			{
				if (rcTracker.Width() != 4) {
					rcTracker.left = rcTracker.right - 4;
				}
			}
			else
			{
				if (rcTracker.Height() != 4) {
					rcTracker.bottom = rcTracker.top + 4;
				}
			}

			dc.SelectObject(pBrush);
			dc.PatBlt(rcTracker.left, rcTracker.top, rcTracker.Width(), rcTracker.Height(), PATINVERT);
		}

		dc.RestoreDC(nSavedDC);
	}
}

void CXTSplitterWnd::OnInvertTracker(const CRect& rect)
{
	CBrush* pBrush = NULL;

	if (m_bFlatSplit) {
		pBrush = CBrush::FromHandle((HBRUSH)::GetStockObject(WHITE_BRUSH));
	}
	else {
		pBrush = CDC::GetHalftoneBrush();
	}

	DrawTracker(rect, pBrush);
}

void CXTSplitterWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bFullDrag == FALSE)
	{
		CSplitterWnd::OnMouseMove(nFlags, point);
		return;
	}

	if (GetCapture() != this) {
		StopTracking(FALSE);
	}

	if (m_bTracking)
	{
		// move tracker to current cursor position
		point.Offset(m_ptTrackOffset); // point is the upper right of hit detect

		// limit the point to the valid split range
		if (point.y < m_rectLimit.top) {
			point.y = m_rectLimit.top;
		}
		else if (point.y > m_rectLimit.bottom) {
			point.y = m_rectLimit.bottom;
		}
		if (point.x < m_rectLimit.left) {
			point.x = m_rectLimit.left;
		}
		else if (point.x > m_rectLimit.right) {
			point.x = m_rectLimit.right;
		}

		if (m_htTrack == vSplitterBox || m_htTrack >= vSplitterBar1 && m_htTrack <= vSplitterBar15)
		{
			if (m_rectTracker.top != point.y)
			{
				OnInvertTracker(m_rectTracker);
				m_rectTracker.OffsetRect(0, point.y - m_rectTracker.top);
				OnInvertTracker(m_rectTracker);
			}
		}
		else if (m_htTrack == hSplitterBox || m_htTrack >= hSplitterBar1 && m_htTrack <= hSplitterBar15)
		{
			if (m_rectTracker.left != point.x)
			{
				OnInvertTracker(m_rectTracker);
				m_rectTracker.OffsetRect(point.x - m_rectTracker.left, 0);
				OnInvertTracker(m_rectTracker);
			}
		}
		else if (m_htTrack == bothSplitterBox || (m_htTrack >= splitterIntersection1 && m_htTrack <= splitterIntersection225))
		{
			if (m_rectTracker.top != point.y)
			{
				OnInvertTracker(m_rectTracker);
				m_rectTracker.OffsetRect(0, point.y - m_rectTracker.top);
				OnInvertTracker(m_rectTracker);
			}
			if (m_rectTracker2.left != point.x)
			{
				OnInvertTracker(m_rectTracker2);
				m_rectTracker2.OffsetRect(point.x - m_rectTracker2.left, 0);
				OnInvertTracker(m_rectTracker2);
			}
		}

		OnLButtonUp(MK_LBUTTON,point);
		OnLButtonDown(MK_LBUTTON,point);

		if (m_point != point)
		{
			RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_UPDATENOW);
			m_point = point;
		}
	}
	else
	{
		// simply hit-test and set appropriate cursor
		int ht = HitTest(point);
		SetSplitCursor(ht);
	}
}

void CXTSplitterWnd::StartTracking(int ht)
{
	m_bClipStyles = FALSE;

	if (!m_bFullDrag)
	{
		if (GetStyle() & WS_CLIPCHILDREN)
		{
			ModifyStyle(WS_CLIPCHILDREN|WS_CLIPSIBLINGS, 0);
			m_bClipStyles = TRUE;
		}

		CSplitterWnd::StartTracking(ht);
		return;
	}


	ASSERT_VALID(this);
	if (ht == noHit) {
		return;
	}

	// GetHitRect will restrict 'm_rectLimit' as appropriate
	GetInsideRect(m_rectLimit);

	if (ht >= splitterIntersection1 && ht <= splitterIntersection225)
	{
		// split two directions (two tracking rectangles)
		int row = (ht - splitterIntersection1) / 15;
		int col = (ht - splitterIntersection1) % 15;

		GetHitRect(row + vSplitterBar1, m_rectTracker);
		int yTrackOffset = m_ptTrackOffset.y;
		m_bTracking2 = TRUE;
		GetHitRect(col + hSplitterBar1, m_rectTracker2);
		m_ptTrackOffset.y = yTrackOffset;
	}
	else if (ht == bothSplitterBox)
	{
		// hit on splitter boxes (for keyboard)
		GetHitRect(vSplitterBox, m_rectTracker);
		int yTrackOffset = m_ptTrackOffset.y;
		m_bTracking2 = TRUE;
		GetHitRect(hSplitterBox, m_rectTracker2);
		m_ptTrackOffset.y = yTrackOffset;

		// center it
		m_rectTracker.OffsetRect(0, m_rectLimit.Height()/2);
		m_rectTracker2.OffsetRect(m_rectLimit.Width()/2, 0);
	}
	else
	{
		// only hit one bar
		GetHitRect(ht, m_rectTracker);
	}

	// steal focus and capture
	SetCapture();

	// set tracking state and appropriate cursor
	m_bTracking = TRUE;
	m_htTrack = ht;
	SetSplitCursor(ht);
}

void CXTSplitterWnd::StopTracking(BOOL bAccept)
{
	if (!m_bFullDrag)
	{
		CSplitterWnd::StopTracking(bAccept);

		if (m_bClipStyles)
		{
			ModifyStyle(0, WS_CLIPCHILDREN|WS_CLIPSIBLINGS);
			m_bClipStyles = FALSE;
		}
		return;
	}

	ASSERT_VALID(this);

	if (!m_bTracking) {
		return;
	}

	ReleaseCapture();

	// erase tracker rectangle
	OnInvertTracker(m_rectTracker);
	if (m_bTracking2)
		OnInvertTracker(m_rectTracker2);

	m_bTracking = m_bTracking2 = FALSE;

	// save old active view
	CWnd* pOldActiveView = GetActivePane();

	// m_rectTracker is set to the new splitter position (without border)
	// (so, adjust relative to where the border will be)
	m_rectTracker.OffsetRect(-CX_BORDER , -CY_BORDER);
	m_rectTracker2.OffsetRect(-CX_BORDER, -CY_BORDER);

	if (bAccept)
	{
		if (m_htTrack == vSplitterBox)
		{
			SplitRow(m_rectTracker.top);
		}
		else if (m_htTrack >= vSplitterBar1 && m_htTrack <= vSplitterBar15)
		{
			// adjust the tracking rect for flat splitter.
			if (m_bFlatSplit) {
				m_rectTracker.top = m_rectTracker.bottom - 4;
			}

			// set row height
			TrackRowSize(m_rectTracker.top, m_htTrack - vSplitterBar1);
			RecalcLayout();
		}
		else if (m_htTrack == hSplitterBox)
		{
			SplitColumn(m_rectTracker.left);
		}
		else if (m_htTrack >= hSplitterBar1 && m_htTrack <= hSplitterBar15)
		{
			// adjust the tracking rect for flat splitter.
			if (m_bFlatSplit) {
				m_rectTracker.left = m_rectTracker.right - 4;
			}

			// set column width
			TrackColumnSize(m_rectTracker.left, m_htTrack - hSplitterBar1);
			RecalcLayout();
		}
		else if (m_htTrack >= splitterIntersection1 &&
			m_htTrack <= splitterIntersection225)
		{
			// set row height and column width
			int row = (m_htTrack - splitterIntersection1) / 15;
			int col = (m_htTrack - splitterIntersection1) % 15;

			TrackRowSize(m_rectTracker.top, row);
			TrackColumnSize(m_rectTracker2.left, col);
			RecalcLayout();
		}
		else if (m_htTrack == bothSplitterBox)
		{
			// rectTracker is vSplitter (splits rows)
			// rectTracker2 is hSplitter (splits cols)
			SplitRow(m_rectTracker.top);
			SplitColumn(m_rectTracker2.left);
		}
	}

	if ((pOldActiveView == GetActivePane()) &&
		(pOldActiveView != NULL))
	{
		SetActivePane(-1, -1, pOldActiveView); // re-activate
	}
}

void CXTSplitterWnd::SetSplitterStyle(DWORD dwxStyle)
{
	m_dwxStyle = dwxStyle;

	// Force no display of windows contents while dragging,
	// regardless of windows setting.
	if (m_dwxStyle & XT_SPLIT_NOFULLDRAG) {
		m_bFullDrag = FALSE;
	}
	else
	{
		// Get system settings for full drag.
		::SystemParametersInfo(SPI_GETDRAGFULLWINDOWS,
			0, &m_bFullDrag, 0);
	}
}

UINT CXTSplitterWnd::OnNcHitTest(CPoint point)
{
	// If XT_SPLIT_NOSIZE style is defined, just return a border
	// hit so can at least activate the app if needed.
	if ((m_dwxStyle & XT_SPLIT_NOSIZE) == XT_SPLIT_NOSIZE)
	{
		return HTBORDER;
	}
	return CSplitterWnd::OnNcHitTest(point);
}

void CXTSplitterWnd::EnableFlatLook(BOOL bFlatSplitter)
{
	m_bFlatSplit    = bFlatSplitter;
	m_cxSplitter    = m_bFlatSplit ? 6 : 7;
	m_cySplitter    = m_bFlatSplit ? 6 : 7;
	m_cxSplitterGap = m_bFlatSplit ? 6 : 7;
	m_cySplitterGap = m_bFlatSplit ? 6 : 7;
	RecalcLayout();
}

void CXTSplitterWnd::OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rectArg)
{
	COLORREF clr3DFace = GetTheme()->m_clrSplitterFace;
	COLORREF clr3DShadow = GetTheme()->m_clrSplitterBorders;


	// if pDC == NULL, then just invalidate
	if (pDC == NULL)
	{
		RedrawWindow(rectArg, NULL, RDW_INVALIDATE|RDW_NOCHILDREN);
		return;
	}
	ASSERT_VALID(pDC);

	// otherwise, actually draw
	CRect rect = rectArg;
	switch (nType)
	{
	case splitBorder:
		if (m_bFlatSplit)
		{
			if (!(m_dwxStyle & XT_SPLIT_NOBORDER))
			{
				pDC->Draw3dRect(rect, clr3DFace, clr3DFace);
				rect.InflateRect(-CX_BORDER, -CY_BORDER);
				pDC->Draw3dRect(rect, clr3DShadow, GetXtremeColor(COLOR_3DHILIGHT));
			} else
			{
				pDC->Draw3dRect(rect, clr3DFace, clr3DFace);
				rect.InflateRect(-CX_BORDER, -CY_BORDER);
				pDC->Draw3dRect(rect, clr3DFace, clr3DFace);
			}
		}
		else
		{
			pDC->Draw3dRect(rect, clr3DShadow, GetXtremeColor(COLOR_3DHILIGHT));
			rect.InflateRect(-CX_BORDER, -CY_BORDER);
			pDC->Draw3dRect(rect, GetXtremeColor(COLOR_WINDOWFRAME), clr3DFace);
		}
		return;

	case splitIntersection:
		ASSERT(!XTOSVersionInfo()->IsWin95OrGreater());
		break;

	case splitBox:
		if (m_bFlatSplit)
		{
			pDC->Draw3dRect(rect, clr3DFace, clr3DFace);
			rect.InflateRect(-CX_BORDER, -CY_BORDER);
			pDC->Draw3dRect(rect, GetXtremeColor(COLOR_3DHILIGHT), clr3DShadow);
			rect.InflateRect(-CX_BORDER, -CY_BORDER);
		}
		else
		{
			pDC->Draw3dRect(rect, clr3DFace, GetXtremeColor(COLOR_WINDOWFRAME));
			rect.InflateRect(-CX_BORDER, -CY_BORDER);
			pDC->Draw3dRect(rect, GetXtremeColor(COLOR_3DHILIGHT), clr3DShadow);
			rect.InflateRect(-CX_BORDER, -CY_BORDER);
		}
		break;

	case splitBar:
		break;

	default:
		ASSERT(FALSE);  // unknown splitter type
	}

	// fill the middle
	COLORREF clr = clr3DFace;
	pDC->FillSolidRect(rect, clr);
}

void CXTSplitterWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	CSplitterWnd::OnLButtonUp(nFlags, point);
	Invalidate();
}

void CXTSplitterWnd::SetActivePane(int row, int col, CWnd* pWnd/*=NULL*/)
{
	// set the focus to the pane
	CWnd* pPane = pWnd == NULL ? GetPane(row, col) : pWnd;
	if (pPane->IsKindOf(RUNTIME_CLASS(CView)))
	{
		CFrameWnd* pFrameWnd = GetParentFrame();
		ASSERT_VALID(pFrameWnd);
		pFrameWnd->SetActiveView((CView*)pPane);
	}
	else
	{
		pPane->SetFocus();
	}
}

//////////////////////////////////////////////////////////////////////
// CXTSplitterWndEx
//

IMPLEMENT_DYNAMIC(CXTSplitterWndEx, CXTSplitterWnd)

BEGIN_MESSAGE_MAP(CXTSplitterWndEx, CXTSplitterWnd)
	//{{AFX_MSG_MAP(CXTSplitterWndEx)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CXTSplitterWndEx::CXTSplitterWndEx()
{
	m_cxBorder       = 0;
	m_cyBorder       = 0;
	m_cxSplitter     = 4;
	m_cySplitter     = 4;
	m_cxSplitterGap  = 4;
	m_cySplitterGap  = 4;
	m_cyTopBorderGap = 6;
	m_bShowTopBorder = true;
}

CXTSplitterWndEx::~CXTSplitterWndEx()
{

}

void CXTSplitterWndEx::OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rectArg)
{
	COLORREF clr3DFace = GetTheme()->m_clrSplitterFace;
	COLORREF clr3DShadow = GetTheme()->m_clrSplitterBorders;

	if (pDC == NULL)
	{
		RedrawWindow(rectArg, NULL, RDW_INVALIDATE|RDW_NOCHILDREN);
		return;
	}
	ASSERT_VALID(pDC);
	CRect rect = rectArg;
	switch (nType)
	{
	case splitBorder:
		return;
	case splitBox:
		pDC->Draw3dRect(rect, clr3DFace, GetXtremeColor(COLOR_WINDOWFRAME));
		rect.InflateRect(-CX_BORDER, -CY_BORDER);
		pDC->Draw3dRect(rect, GetXtremeColor(COLOR_3DHILIGHT), clr3DShadow);
		rect.InflateRect(-CX_BORDER, -CY_BORDER);
		break;
	}

	pDC->FillSolidRect(rect, clr3DFace);
}

// Generic routine:
//  for X direction: pInfo = m_pColInfo, nMax = m_nMaxCols, nSize = cx
//  for Y direction: pInfo = m_pRowInfo, nMax = m_nMaxRows, nSize = cy
void CXTSplitterWndEx::LayoutRowCol(CSplitterWnd::CRowColInfo* pInfoArray,
	int nMax, int nSize, int nSizeSplitter)
{
	ASSERT(pInfoArray != NULL);
	ASSERT(nMax > 0);
	ASSERT(nSizeSplitter > 0);

	CSplitterWnd::CRowColInfo* pInfo;
	int i;

	if (nSize < 0)
		nSize = 0;  // if really too small, layout as zero size

	// start with ideal sizes
	for (i = 0, pInfo = pInfoArray; i < nMax-1; i++, pInfo++)
	{
		if (pInfo->nIdealSize < pInfo->nMinSize)
			pInfo->nIdealSize = 0;      // too small to see
		pInfo->nCurSize = pInfo->nIdealSize;
	}
	pInfo->nCurSize = INT_MAX;  // last row/column takes the rest

	for (i = 0, pInfo = pInfoArray; i < nMax; i++, pInfo++)
	{
		ASSERT(nSize >= 0);
		if (nSize == 0)
		{
			// no more room (set pane to be invisible)
			pInfo->nCurSize = 0;
			continue;       // don't worry about splitters
		}
		else if (nSize < pInfo->nMinSize && i != 0)
		{
			// additional panes below the recommended minimum size
			//   aren't shown and the size goes to the previous pane
			pInfo->nCurSize = 0;

			// previous pane already has room for splitter + border
			//   add remaining size and remove the extra border
			ASSERT(CX_BORDER*2 == CY_BORDER*2);
			(pInfo-1)->nCurSize += nSize + CX_BORDER*2;
			nSize = 0;
		}
		else
		{
			// otherwise we can add the second pane
			ASSERT(nSize > 0);
			if (pInfo->nCurSize == 0)
			{
				// too small to see
				if (i != 0)
					pInfo->nCurSize = 0;
			}
			else if (nSize < pInfo->nCurSize)
			{
				// this row/col won't fit completely - make as small as possible
				pInfo->nCurSize = nSize;
				nSize = 0;
			}
			else
			{
				// can fit everything
				nSize -= pInfo->nCurSize;
			}
		}

		// see if we should add a splitter
		ASSERT(nSize >= 0);
		if (i != nMax - 1)
		{
			// should have a splitter
			if (nSize > nSizeSplitter)
			{
				nSize -= nSizeSplitter; // leave room for splitter + border
				ASSERT(nSize > 0);
			}
			else
			{
				// not enough room - add left over less splitter size
				ASSERT(CX_BORDER*2 == CY_BORDER*2);
				pInfo->nCurSize += nSize;
				if (pInfo->nCurSize > (nSizeSplitter - CX_BORDER*2))
					pInfo->nCurSize -= (nSizeSplitter - CY_BORDER*2);
				nSize = 0;
			}
		}
	}
	ASSERT(nSize == 0); // all space should be allocated
}

// repositions client area of specified window
// assumes everything has WS_BORDER or is inset like it does
//  (includes scroll bars)
void CXTSplitterWndEx::DeferClientPos(AFX_SIZEPARENTPARAMS* lpLayout,
	CWnd* pWnd, int x, int y, int cx, int cy, BOOL bScrollBar)
{
	ASSERT(pWnd != NULL);
	ASSERT(pWnd->m_hWnd != NULL);

	if (bScrollBar)
	{
		// if there is enough room, draw scroll bar without border
		// if there is not enough room, set the WS_BORDER bit so that
		//   we will at least get a proper border drawn
		BOOL bNeedBorder = (cx <= CX_BORDER || cy <= CY_BORDER);
		pWnd->ModifyStyle(bNeedBorder ? 0 : WS_BORDER,
			bNeedBorder ? WS_BORDER : 0);
	}
	CRect rect(x, y, x+cx, y+cy);

	// adjust for border size (even if zero client size)

	// adjust for 3d border (splitter windows have implied border)
	if ((pWnd->GetExStyle() & WS_EX_CLIENTEDGE) ||
		pWnd->IsKindOf(RUNTIME_CLASS(CSplitterWnd)))
		rect.InflateRect(CX_BORDER*2, CY_BORDER*2);

	// first check if the new rectangle is the same as the current
	CRect rectOld;
	pWnd->GetWindowRect(rectOld);
	pWnd->GetParent()->ScreenToClient(&rectOld);
	if (rect != rectOld)
		AfxRepositionWindow(lpLayout, pWnd->m_hWnd, rect);
}

void CXTSplitterWndEx::RecalcLayout()
{
	ASSERT_VALID(this);
	ASSERT(m_nRows > 0 && m_nCols > 0); // must have at least one pane

	CRect rectClient;
	GetClientRect(rectClient);
	rectClient.InflateRect(-m_cxBorder, -m_cyBorder);
	rectClient.top += m_cyTopBorderGap;

	CRect rectInside;
	GetInsideRect(rectInside);

	// layout columns (restrict to possible sizes)
	LayoutRowCol(m_pColInfo, m_nCols, rectInside.Width(), m_cxSplitterGap);
	LayoutRowCol(m_pRowInfo, m_nRows, rectInside.Height(), m_cySplitterGap);

	// adjust the panes (and optionally scroll bars)

	// give the hint for the maximum number of HWNDs
	AFX_SIZEPARENTPARAMS layout;
	layout.hDWP = ::BeginDeferWindowPos((m_nCols + 1) * (m_nRows + 1) + 1);

	// size of scrollbars
	int cx = (rectClient.right - rectInside.right);
	int cy = (rectClient.bottom - rectInside.bottom);

	// reposition size box
	if (m_bHasHScroll && m_bHasVScroll)
	{
		CWnd* pScrollBar = GetDlgItem(AFX_IDW_SIZE_BOX);
		ASSERT(pScrollBar != NULL);

		// fix style if necessary
		BOOL bSizingParent = (GetSizingParent() != NULL);
		// modifyStyle returns TRUE if style changes
		if (pScrollBar->ModifyStyle(SBS_SIZEGRIP|SBS_SIZEBOX,
				bSizingParent ? SBS_SIZEGRIP : SBS_SIZEBOX))
			pScrollBar->Invalidate();
		pScrollBar->EnableWindow(bSizingParent);

		// reposition the size box
		DeferClientPos(&layout, pScrollBar,
			rectInside.right, rectInside.bottom, cx, cy, TRUE);
	}

	// reposition scroll bars
	if (m_bHasHScroll)
	{
		int cxSplitterBox = m_cxSplitter;// split box bigger
		int x = rectClient.left;
		int y = rectInside.bottom;
		int col;
		for (col = 0; col < m_nCols; col++)
		{
			CWnd* pScrollBar = GetDlgItem(AFX_IDW_HSCROLL_FIRST + col);
			ASSERT(pScrollBar != NULL);
			int cx = m_pColInfo[col].nCurSize;
			if (col == 0 && m_nCols < m_nMaxCols)
				x += cxSplitterBox, cx -= cxSplitterBox;
			DeferClientPos(&layout, pScrollBar, x, y, cx, cy, TRUE);
			x += cx + m_cxSplitterGap;
		}
	}

	if (m_bHasVScroll)
	{
		int cySplitterBox = m_cySplitter;// split box bigger
		int x = rectInside.right;
		int y = rectClient.top;
		int row;
		for (row = 0; row < m_nRows; row++)
		{
			CWnd* pScrollBar = GetDlgItem(AFX_IDW_VSCROLL_FIRST + row);
			ASSERT(pScrollBar != NULL);
			int cy = m_pRowInfo[row].nCurSize;
			if (row == 0 && m_nRows < m_nMaxRows)
				y += cySplitterBox, cy -= cySplitterBox;
			DeferClientPos(&layout, pScrollBar, x, y, cx, cy, TRUE);
			y += cy + m_cySplitterGap;
		}
	}

	//BLOCK: Reposition all the panes
	{
		int x = rectClient.left;
		int col;
		for (col = 0; col < m_nCols; col++)
		{
			int cx = m_pColInfo[col].nCurSize;
			int y = rectClient.top;
			int row;
			for (row = 0; row < m_nRows; row++)
			{
				int cy = m_pRowInfo[row].nCurSize;
				CWnd* pWnd = GetPane(row, col);
				DeferClientPos(&layout, pWnd, x, y, cx, cy, FALSE);
				y += cy + m_cySplitterGap;
			}
			x += cx + m_cxSplitterGap;
		}
	}

	// move and resize all the windows at once!
	if (layout.hDWP == NULL || !::EndDeferWindowPos(layout.hDWP))
		TRACE0("Warning: DeferWindowPos failed - low system resources.\n");

	// invalidate all the splitter bars (with NULL pDC)
	DrawAllSplitBars(NULL, rectInside.right, rectInside.bottom);
}

void CXTSplitterWndEx::GetInsideRect(CRect& rect) const
{
	CSplitterWnd::GetInsideRect(rect);
	rect.top += m_cyTopBorderGap;
}

void CXTSplitterWndEx::OnPaint()
{
	CSplitterWnd::OnPaint();

	if (m_bShowTopBorder)
	{
		CClientDC dc(this);

		int iSavedDC = dc.SaveDC();

		CRect rectClient;
		GetClientRect(rectClient);

		if (GetTheme()->GetTheme() == xtThemeDefault)
		{
			CPen penShadow(PS_SOLID, 1, GetXtremeColor(COLOR_3DSHADOW));
			dc.SelectObject(&penShadow);
			dc.MoveTo(rectClient.left, rectClient.top);
			dc.LineTo(rectClient.right, rectClient.top);
		}

		CPen penHilight(PS_SOLID, 1, GetXtremeColor(COLOR_3DHILIGHT));
		dc.SelectObject(&penHilight);
		dc.MoveTo(rectClient.left, rectClient.top+1);
		dc.LineTo(rectClient.right, rectClient.top+1);

		dc.RestoreDC(iSavedDC);
	}
}

BOOL CXTSplitterWndEx::OnEraseBkgnd(CDC* pDC)
{
	CRect rectClient;
	GetClientRect(rectClient);
	rectClient.InflateRect(-m_cxBorder, -m_cyBorder);
	rectClient.bottom = rectClient.top + m_cyTopBorderGap;

	COLORREF clr3DFace = GetTheme()->m_clrSplitterFace;
	pDC->FillSolidRect(rectClient, clr3DFace);


	return TRUE;
}
