// XTPReportControl.cpp : implementation of the CXTPReportControl class.
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
#include "XTPImageManager.h"
#include "XTPVC80Helpers.h"
#include "XTPPropExchange.h"

#include "XTPReportRecordItem.h"
#include "XTPReportRecordItemText.h"
#include "XTPReportRecord.h"
#include "XTPReportRecords.h"
#include "XTPReportHeader.h"
#include "XTPReportColumn.h"
#include "XTPReportColumns.h"
#include "XTPReportRow.h"
#include "XTPReportRows.h"
#include "XTPReportControl.h"
#include "XTPReportPaintManager.h"
#include "XTPReportFilterEditControl.h"
#include "XTPReportSubListControl.h"
#include "XTPReportGroupRow.h"
#include "XTPReportControlResource.h"
#include "XTPReportInplaceControls.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CXTPReportControl
IMPLEMENT_DYNCREATE(CXTPReportControl, CWnd)

BEGIN_MESSAGE_MAP(CXTPReportControl, CWnd)
//{{AFX_MSG_MAP(CXTPReportControl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_CONTEXTMENU()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_KEYDOWN()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_SYSKEYDOWN()
	ON_WM_CAPTURECHANGED()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_KEYUP()
	ON_WM_SYSKEYUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_GETDLGCODE()
	ON_WM_CHAR()
	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CXTPReportControl::CXTPReportControl()
{
	RegisterWindowClass();

	m_bSubclassFromCreate = FALSE;

	m_nLockUpdateCount = 0;
	m_nRowsPerWheel = GetMouseScrollLines();

	m_pRows = new CXTPReportRows();
	m_pPlainTree = new CXTPReportRows();
	m_pRecords = new CXTPReportRecords();
	m_pColumns = new CXTPReportColumns(this);

	m_pPaintManager = new CXTPReportPaintManager();
	m_nTopRow = 0;

	m_nFocusedRow = -1;
	m_mouseMode = xtpReportMouseNothing;

	m_pSelectedRows = new CXTPReportSelectedRows(this);

	m_bChanged = TRUE;
	m_bRefreshIndexes = FALSE;

	m_bGroupByEnabled = FALSE;
	m_bHeaderVisible = TRUE;
	m_bMultipleSelection = TRUE;

	m_bShowTooltips = TRUE;

	m_bSkipGroupsFocus = TRUE;

	m_pImageManager = new CXTPImageManager();

	m_pFocusedColumn = NULL;
	m_pActiveItem = NULL;

	m_bFocusSubItems = FALSE;
	m_bEditOnClick = TRUE;
	m_bAllowEdit = FALSE;

	m_pInplaceEdit = new CXTPReportInplaceEdit();
	m_pInplaceButtons = new CXTPReportInplaceButtons();
	m_pInplaceList = new CXTPReportInplaceList();

	m_rcGroupByArea.SetRectEmpty();
	m_rcHeaderArea.SetRectEmpty();
	m_rcReportArea.SetRectEmpty();

	m_nLeftOffset = 0;
	m_pHotRow = 0;

	m_bVScrollBarVisible = FALSE;

	m_bPrepareDrag = FALSE;
	m_pointDrag = CPoint(0, 0);
}

BOOL CXTPReportControl::RegisterWindowClass()
{
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();

	if (!(::GetClassInfo(hInst, XTPREPORTCTRL_CLASSNAME, &wndcls)))
	{
		// otherwise we need to register a new class
		wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		wndcls.lpfnWndProc      = ::DefWindowProc;
		wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
		wndcls.hInstance        = hInst;
		wndcls.hIcon            = NULL;
		wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
		wndcls.lpszMenuName     = NULL;
		wndcls.lpszClassName    = XTPREPORTCTRL_CLASSNAME;

		if (!AfxRegisterClass(&wndcls))
		{
			AfxThrowResourceException();
			return FALSE;
		}
	}

	return TRUE;
}

CXTPReportControl::~CXTPReportControl()
{
	if (::IsWindow(m_wndTip.GetSafeHwnd()))
		m_wndTip.DestroyWindow();

	m_pRows->InternalRelease();
	m_pPlainTree->InternalRelease();
	m_pRecords->InternalRelease();
	m_pColumns->InternalRelease();
	m_pSelectedRows->InternalRelease();
	m_pPaintManager->InternalRelease();

	m_pImageManager->InternalRelease();

	delete m_pInplaceEdit;
	delete m_pInplaceButtons;
	delete m_pInplaceList;
}

void CXTPReportControl::SetImageManager(CXTPImageManager* pImageManager)
{
	if (pImageManager)
	{
		m_pImageManager->InternalRelease();
		m_pImageManager = pImageManager;
	}

}

void CXTPReportControl::SetImageList(CImageList* pImageList)
{
	for (int i = 0; i < pImageList->GetImageCount(); i++)
	{
		HICON hIcon = pImageList->ExtractIcon(i);
		m_pImageManager->SetIcon(hIcon, i);

		DestroyIcon(hIcon);
	}
}

void CXTPReportControl::SetPaintManager(CXTPReportPaintManager* pPaintManager)
{
	if (pPaintManager)
	{
		m_pPaintManager->InternalRelease();
		m_pPaintManager = pPaintManager;

		AdjustLayout();
		AdjustScrollBars();
	}
}


BOOL CXTPReportControl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	if(!CWnd::Create(XTPREPORTCTRL_CLASSNAME, NULL, dwStyle, rect, pParentWnd, nID, pContext))
		return FALSE;

	return TRUE;
}

void CXTPReportControl::PreSubclassWindow()
{
	CWnd::PreSubclassWindow();
}

int CXTPReportControl::GetIndent(int nLevel)
{
	return max(0, (nLevel - 1) * m_pPaintManager->m_nTreeIndent);
}

void CXTPReportControl::SetTreeIndent(int nIndent)
{
	m_pPaintManager->m_nTreeIndent = nIndent;
}


void CXTPReportControl::BeginUpdate()
{
	m_nLockUpdateCount++;
	m_bRefreshIndexes = FALSE;

}

void CXTPReportControl::EndUpdate()
{
	SetChanged();
	m_nLockUpdateCount--;

	if (m_nLockUpdateCount == 0)
	{
		if (m_bRefreshIndexes)
		{
			m_bRefreshIndexes = FALSE;
			_RefreshIndexes();
		}

		RedrawControl();
	}
}

void CXTPReportControl::RedrawControl()
{
	SetChanged();
	if (m_nLockUpdateCount == 0 && GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
}

void CXTPReportControl::UpdateSubList()
{

	CXTPReportHeader* pHeader = m_pColumns->GetReportHeader();
	if (pHeader && pHeader->m_pSubList)
		pHeader->m_pSubList->SetReportCtrl(this);
}

void CXTPReportControl::_DoCollapse(CXTPReportRow* pRow)
{
	int nIndex = pRow->GetIndex() + 1;
	int nCount = 0;

	while(nIndex < m_pRows->GetCount())
	{
		CXTPReportRow* pRowChild = m_pRows->GetAt(nIndex);
		if (!pRowChild->HasParent(pRow))
			break;

		pRowChild->m_bVisible = FALSE;
		pRowChild->m_nIndex = -1;
		m_pRows->RemoveAt(nIndex);
		nCount++;
	}

	if (nCount > 0)
	{
		m_pSelectedRows->_OnCollapsed(nIndex - 1, nCount);

		if (m_nFocusedRow >= nIndex)
			m_nFocusedRow = max(nIndex - 1, m_nFocusedRow - nCount);

	}
}

void CXTPReportControl::_DoExpand(CXTPReportRow* pRow)
{
	int nIndex = pRow->m_nIndex;
	int nCount = _DoExpand(nIndex, pRow);

	if (nCount > 0)
	{
		m_pSelectedRows->_OnExpanded(nIndex, nCount);

		if (m_nFocusedRow > nIndex)
			m_nFocusedRow += nCount;

	}
}

int CXTPReportControl::_DoExpand(int nIndex, CXTPReportRow* pRow)
{
	if (!pRow->HasChildren())
		return 0;

	int nStartIndex = nIndex;
	for (int i = 0; i < pRow->GetChilds()->GetCount(); i++)
	{
		CXTPReportRow* pRowChild = pRow->GetChilds()->GetAt(i);

		pRowChild->m_nRowLevel = pRow->m_nRowLevel + 1;
		pRowChild->m_nGroupLevel = pRow->m_nGroupLevel + (pRow->IsGroupRow()? 1: 0);

		nIndex += InsertRow(nIndex + 1, pRowChild);
	}
	return nIndex - nStartIndex;
}

void CXTPReportControl::_RefreshIndexes()
{
	if (m_nLockUpdateCount != 0)
	{
		m_bRefreshIndexes = TRUE;
		return;

	}

	int nRowCount = m_pRows->GetCount();
	for (int nIndex = 0; nIndex < nRowCount; nIndex++)
	{
		CXTPReportRow* pRow = m_pRows->GetAt(nIndex);
		pRow->m_nIndex = nIndex;
		ASSERT(pRow->m_bVisible);
	}

	AdjustScrollBars();
}

int CXTPReportControl::InsertRow(int nIndex, CXTPReportRow* pRow)
{
	m_pRows->InsertAt(nIndex, pRow);
	pRow->InternalAddRef();
	pRow->m_bVisible = TRUE;


	int nRowsInserted = 1;

	if (pRow->IsExpanded() && pRow->HasChildren())
	{
		nRowsInserted += _DoExpand(nIndex, pRow);
	}
	return nRowsInserted;
}

void CXTPReportControl::BuildTree(CXTPReportRows* pTree, CXTPReportRow* pParentRow, CXTPReportRecords* pRecords)
{
	for (int i = 0; i < pRecords->GetCount(); i++)
	{
		CXTPReportRecord* pRecord = pRecords->GetAt(i);

		// add record if all conditions are met
		if (pRecord->IsLocked() ||
			(pRecord->IsVisible() && !ApplyFilter(pRecord, GetFilterText(), IsPreviewMode())))
		{
			CXTPReportRow* pRow = CreateRow();
			pRow->InitRow(this, pRecord);

			pRow->m_pParentRow = pParentRow;
			pTree->Add(pRow);

			if (pRecord->HasChildren())
			{
				BuildTree(pRow->GetChilds(), pRow, pRecord->GetChilds());
			}
		}
	}
}


BOOL CXTPReportControl::ApplyFilter(CXTPReportRecord* pRecord, CString strFilterText, BOOL bIncludePreview)
{
	// not filtered if filter text is empty
	if (!pRecord )
		return FALSE;

	if (pRecord->IsFiltered())
		return TRUE;

	if( strFilterText.IsEmpty())
		return FALSE;

	// process each token in the filter string
	TCHAR szSeps[] = _T(" \t");
	TCHAR *szToken, *lpszContext = 0;
	//int nCurPos = 0;

	szToken = STRTOK_S(strFilterText.GetBuffer(strFilterText.GetLength()), szSeps, &lpszContext);
	while (szToken != NULL)
	{
		CString strToken(szToken);
		strToken.MakeLower();
		BOOL bTokenFound = FALSE;
		// enumerate all visible columns
		int nColumnsCount = m_pColumns->GetCount();
		for (int nCol = 0; nCol < nColumnsCount; nCol++)
		{
			CXTPReportColumn* pCol = m_pColumns->GetAt(nCol);
			if (pCol && pCol->IsVisible() && pCol->IsFiltrable())
			{
				CXTPReportRecordItem* pItem = pRecord->GetItem(pCol);
				if (pItem)
				{
					CString sItemText = pItem->GetCaption(pCol);
					// case-insensitive search
					sItemText.MakeLower();
					bTokenFound = sItemText.Find(strToken) != -1;

					if (bTokenFound)
					{
						// stop search current token - passed
						break;
					}
				}
			}
		}
		// also check preview text
		if (bIncludePreview && !bTokenFound && pRecord->GetItemPreview())
		{
			CString sItemText = pRecord->GetItemPreview()->GetCaption(NULL);
			// case-insensitive search
			sItemText.MakeLower();
			bTokenFound = sItemText.Find(strToken) != -1;
		}

		// Token not found - filter this record
		if (!bTokenFound)
		{
			return TRUE;
		}

		// get next token
		szToken = STRTOK_S(NULL, szSeps, &lpszContext);
	}

	return FALSE;
}

void CXTPReportControl::Populate()
{
	EditItem(FALSE);

	BeginUpdate();

	// save focused items
	CXTPReportRecord* pFocusedRecord = GetFocusedRow()?  GetFocusedRow()->GetRecord(): NULL;
	m_pSelectedRows->Clear();
	m_nFocusedRow = -1;
	m_pRows->Clear();
	m_pPlainTree->Clear();
	m_arrScreenRows.Clear(FALSE);

	if (IsVirtualMode())
	{
		CXTPReportRow* pRow = CreateRow();
		pRow->InitRow(this, GetRecords()->m_pVirtualRecord);
		pRow->m_bVisible = TRUE;

		m_pRows->SetVirtualMode(pRow, GetRecords()->GetCount());
	}
	else
	{

		BuildTree(m_pPlainTree, NULL, m_pRecords);
		SortRows(m_pPlainTree);

		if (m_pColumns->GetGroupsOrder()->GetCount() > 0)
		{
			CXTPReportRows* pGroupTree = new CXTPReportRows();
			CXTPReportGroupRow* pLastGroup = NULL;

			for (int nPlainRow = 0; nPlainRow < m_pPlainTree->GetCount(); nPlainRow++)
			{
				CXTPReportRow* pRow = m_pPlainTree->GetAt(nPlainRow);
				CXTPReportGroupRow* pGroupToAdd = NULL;

				for (int nColumn = 0; nColumn < m_pColumns->GetGroupsOrder()->GetCount(); nColumn++)
				{
					CXTPReportColumn* pColumn = m_pColumns->GetGroupsOrder()->GetAt(nColumn);
					CXTPReportRecordItem* pItem = pRow->GetRecord()->GetItem(pColumn);
					CString strGroup = pItem? pItem->GetGroupCaption(pColumn): _T("");

					if (pLastGroup && pLastGroup->GetCaption() == strGroup)
					{
						pGroupToAdd = pLastGroup;

						if (pGroupToAdd->HasChildren())
							pLastGroup = (CXTPReportGroupRow*)pGroupToAdd->GetChilds()->GetAt(pGroupToAdd->GetChilds()->GetCount() - 1);

					}
					else
					{
						CXTPReportGroupRow* pGroup = CreateGroupRow();

						pGroup->InitRow(this, NULL);
						pGroup->SetCaption(strGroup);

						if (pGroupToAdd)
						{
							pGroupToAdd->AddChild(pGroup);
						}
						else
						{
							pGroupTree->Add(pGroup);
						}

						pGroupToAdd = pGroup;
						pLastGroup = NULL;
					}

				}
				pGroupToAdd->AddChild(pRow);
				pRow->InternalAddRef();
				pLastGroup = (CXTPReportGroupRow*)pGroupTree->GetAt(pGroupTree->GetCount() - 1);
			}

			m_pPlainTree->InternalRelease();
			m_pPlainTree = pGroupTree;
		}

		for (int nGroupRow = 0; nGroupRow < m_pPlainTree->GetCount(); nGroupRow++)
		{
			CXTPReportRow* pRow = m_pPlainTree->GetAt(nGroupRow);
			InsertRow(m_pRows->GetCount(), pRow);
			pRow->m_nChildIndex = nGroupRow;
			ASSERT(pRow->m_pParentRows == m_pPlainTree);
		}


		m_nFocusedRow = -1;
		// Update indexes on virtual rows
		int nRowCount = m_pRows->GetCount();
		for (int nRowIndex = 0; nRowIndex < nRowCount; nRowIndex++)
		{
			CXTPReportRow* pRow = m_pRows->GetAt(nRowIndex);
			if (pRow)
			{
				pRow->SetIndex(nRowIndex);
				pRow->m_bVisible = TRUE;

				if (pFocusedRecord && pRow->GetRecord() == pFocusedRecord)
				{
					m_nFocusedRow = pRow->GetIndex();
					m_pSelectedRows->Select(pRow);
				}
			}
		}
	}

	AdjustIndentation();
	AdjustLayout();
	AdjustScrollBars();

	UpdateSubList();

	if (m_nFocusedRow == -1)
	{
		SetTopRow(0);
	}
	else
	{
		EnsureVisible(GetFocusedRow());
	}

	EndUpdate();
}

void CXTPReportControl::SortRows(CXTPReportRows* pRows)
{
	if (pRows->GetCount() == 0 ||
		(m_pColumns->GetSortOrder()->GetCount() == 0 && m_pColumns->GetGroupsOrder()->GetCount() == 0))
		return;

	pRows->Sort();
}

void CXTPReportControl::AdjustIndentation()
{
	XTP_TRACE(_T("AdjustIndentation()\n"));

	m_pColumns->GetReportHeader()->m_nIndentLevel =
		m_pColumns->GetGroupsOrder()->GetCount();
}

CXTPReportColumn* CXTPReportControl::AddColumn(CXTPReportColumn* pColumn)
{
	ASSERT(pColumn);
	m_pColumns->Add(pColumn);

	return pColumn;
}

CXTPReportRecord* CXTPReportControl::AddRecord(CXTPReportRecord* pRecord)
{
	ASSERT(pRecord);
	m_pRecords->Add(pRecord);

	return pRecord;
}

void CXTPReportControl::DrawRows(CDC* pDC, CRect& rcClient)
{
	m_arrScreenRows.Clear(FALSE);

	if (m_pColumns->GetReportHeader()->GetNextVisibleColumn(-1, 1) == NULL)
		return;

	pDC->SetBkMode(TRANSPARENT);

	int y = rcClient.top;

	int nRowCount = m_pRows->GetCount();

	if (0 == nRowCount)
	{
		pDC->SetTextColor(GetPaintManager()->m_clrWindowText);
		CString strNoItems = GetPaintManager()->m_strNoItems;

		if (!strNoItems.IsEmpty())
		{
			CRect rcText(rcClient);
			rcText.top += 5;
			CXTPFontDC font(pDC, &GetPaintManager()->m_fontText);
			pDC->DrawText(strNoItems, rcText, DT_END_ELLIPSIS|DT_CENTER|DT_NOPREFIX);

		}
	}

	int nHeaderWidth = GetReportHeader()->GetWidth();


	for (int i = m_nTopRow; i < nRowCount; i++)
	{
		CXTPReportRow* pRow = m_pRows->GetAt(i);

		if (y > rcClient.bottom)
			break;

		CRect rcRow(rcClient.left - m_nLeftOffset, y,
			rcClient.left + nHeaderWidth - m_nLeftOffset, y + pRow->GetHeight(pDC));

		pRow->Draw(pDC, rcRow);

		y += rcRow.Height();


		CXTPReportRow* pScreenRow = NULL;
		if (IsVirtualMode())
		{
			pScreenRow = CreateRow();
			pScreenRow->InitRow(pRow);
		}
		else
		{
			pScreenRow = pRow;
			pScreenRow->InternalAddRef();

		}
		m_arrScreenRows.InsertAt(m_arrScreenRows.GetCount(), pScreenRow);
	}

}

void CXTPReportControl::OnSelectionChanged()
{
	SendNotifyMessage(XTP_NM_REPORT_SELCHANGED);
	m_pSelectedRows->SetChanged(FALSE);
}


void CXTPReportControl::SetFocusedColumn(CXTPReportColumn* pColumn)
{
	if (m_pFocusedColumn != pColumn)
	{
		m_pFocusedColumn = pColumn;

		if (m_bFocusSubItems)
		{
			OnSelectionChanged();
		}
	}
}

void CXTPReportControl::SetFocusedRow(CXTPReportRow* pRow, BOOL bIgnoreSelection)
{
	EditItem(NULL);
	BeginUpdate();

	if (!bIgnoreSelection || !m_bMultipleSelection)
	{
		m_pSelectedRows->Select(pRow);
	}

	if (pRow)
	{
		m_nFocusedRow = pRow->GetIndex();
		EnsureVisible(pRow);
	}

	EndUpdate();

	if (m_pSelectedRows->IsChanged())
		OnSelectionChanged();
}

void CXTPReportControl::SetFocusedRow(CXTPReportRow* pRow, BOOL bSelectBlock, BOOL bIgnoreSelection)
{
	if (!pRow)
		return;

	EditItem(NULL);
	BeginUpdate();

	if (m_bMultipleSelection)
	{
		if (bSelectBlock && m_nFocusedRow != -1)
		{
			m_pSelectedRows->SelectBlock(m_nFocusedRow, pRow->GetIndex());

		} else if (!bIgnoreSelection)
		{
			m_pSelectedRows->Select(pRow);
		}
	} else
	{
		m_pSelectedRows->Select(pRow);
	}

	m_nFocusedRow = pRow->GetIndex();

	EnsureVisible(pRow);
	EndUpdate();

	if (m_pSelectedRows->IsChanged())
		OnSelectionChanged();
}


void CXTPReportControl::SetLeftOffset(int nOffset)
{
	if (nOffset == m_nLeftOffset)
		return;

	m_nLeftOffset = nOffset;
	SetScrollPos(SB_HORZ, nOffset);
	AdjustScrollBars();
}

void CXTPReportControl::SetTopRow(int nIndex)
{
	if (nIndex == m_nTopRow)
		return;

	ASSERT(nIndex >= 0);

	m_nTopRow = nIndex;
	SetScrollPos(SB_VERT, nIndex);
	AdjustScrollBars();
}

void CXTPReportControl::EnsureVisible(CXTPReportRow* pCheckRow)
{
	int nCheckIndex = pCheckRow? pCheckRow->GetIndex(): -1;

	if (nCheckIndex == -1 || !pCheckRow->m_bVisible || nCheckIndex >= m_pRows->GetCount())
		return;

	if (nCheckIndex < m_nTopRow)
	{
		SetTopRow(nCheckIndex);
		return;
	}

	CClientDC dc (this);

	int top = m_rcReportArea.top;

	for (int i = m_nTopRow; i < m_pRows->GetCount(); i++)
	{
		CXTPReportRow* pRow = m_pRows->GetAt(i);
		int nRowHeight = pRow->GetHeight(&dc);

		if (top + nRowHeight > m_rcReportArea.bottom)
			break;

		if (i == nCheckIndex)
			return;

		top += nRowHeight;
	}

	int nHeight= m_rcReportArea.Height();
	for (top = nCheckIndex; top >= 0; top--)
	{
		CXTPReportRow* pRow = m_pRows->GetAt(top);
		int nRowHeight = pRow->GetHeight(&dc);

		if (nHeight - nRowHeight < 0)
		{
			top++;
			break;
		}

		nHeight -= nRowHeight;
	}
	SetTopRow(top);
	RedrawControl();
}

CXTPReportRow* CXTPReportControl::HitTest(CPoint pt)
{
	if (!m_rcReportArea.PtInRect(pt))
		return NULL;

	for (int i = 0; i < m_arrScreenRows.GetCount(); i++)
	{
		CXTPReportRow* pRow = m_arrScreenRows.GetAt(i);

		CRect rc = pRow->GetRect();

		if (rc.PtInRect(pt))
			return pRow;
	}
	return NULL;
}

int CXTPReportControl::GetReportAreaRows(int nStartRow, BOOL bMoveDown)
{
	int nDirection = bMoveDown? +1: -1;

	int top = m_rcReportArea.top;

	CClientDC dc(this);

	for (int i = nStartRow; (i < m_pRows->GetCount() && i >=0); i += nDirection )
	{
		CXTPReportRow* pRow = m_pRows->GetAt(i);

		int rawHeight = pRow->GetHeight(&dc);

		if (top + rawHeight > m_rcReportArea.bottom)
			return bMoveDown? i - nStartRow - 1: nStartRow - i - 1;

		top += rawHeight;
	}

	return bMoveDown? m_pRows->GetCount()  - nStartRow: nStartRow;
}



void CXTPReportControl::MoveDown(BOOL bSelectBlock, BOOL bIgnoreSelection)
{
	SetFocusedRow(m_pRows->GetNext(GetFocusedRow(), m_bSkipGroupsFocus), bSelectBlock, bIgnoreSelection);
}

void CXTPReportControl::MoveUp(BOOL bSelectBlock, BOOL bIgnoreSelection)
{
	SetFocusedRow(m_pRows->GetPrev(GetFocusedRow(), m_bSkipGroupsFocus), bSelectBlock, bIgnoreSelection);
}

void CXTPReportControl::MovePageDown(BOOL bSelectBlock, BOOL bIgnoreSelection)
{
	int nCurrentRowIndex = m_nFocusedRow != -1? m_nFocusedRow: 0;
	nCurrentRowIndex = min(m_pRows->GetCount() - 1, nCurrentRowIndex + GetReportAreaRows(nCurrentRowIndex, true));

	SetFocusedRow(m_pRows->GetAt(nCurrentRowIndex), bSelectBlock, bIgnoreSelection);
}

void CXTPReportControl::MovePageUp(BOOL bSelectBlock, BOOL bIgnoreSelection)
{
	int nCurrentRowIndex = m_nFocusedRow != -1? m_nFocusedRow: 0;
	nCurrentRowIndex = max(0, nCurrentRowIndex - GetReportAreaRows(nCurrentRowIndex, false));

	SetFocusedRow(m_pRows->GetAt(nCurrentRowIndex), bSelectBlock, bIgnoreSelection);
}

void CXTPReportControl::MoveFirst(BOOL bSelectBlock, BOOL bIgnoreSelection)
{
	SetFocusedRow(m_pRows->GetAt(0), bSelectBlock, bIgnoreSelection);
}

void CXTPReportControl::MoveLast(BOOL bSelectBlock, BOOL bIgnoreSelection)
{
	SetFocusedRow(m_pRows->GetAt(m_pRows->GetCount() - 1), bSelectBlock, bIgnoreSelection);
}

void CXTPReportControl::AdjustLayout()
{
	CXTPClientRect rc(this);

	int nHeaderWidth = m_rcHeaderArea.Width();

	CXTPReportHeader* pHeader = m_pColumns->GetReportHeader();
	int nGroupByHeight = (m_bGroupByEnabled && pHeader) ? pHeader->GetGroupByHeight() : 0;
	m_rcGroupByArea.SetRect(0, 0, rc.Width(), nGroupByHeight);
	int nHeaderHeight = m_bHeaderVisible? GetPaintManager()->GetHeaderHeight(): 0;
	m_rcHeaderArea.SetRect(0, m_rcGroupByArea.bottom, rc.Width(), m_rcGroupByArea.bottom + nHeaderHeight);
	m_rcReportArea.SetRect(0, m_rcHeaderArea.bottom, rc.Width(), rc.Height());


	if (nHeaderWidth != m_rcHeaderArea.Width())
	{
		pHeader->AdjustColumnsWidth(m_rcHeaderArea.Width());

	}
}

void CXTPReportControl::AdjustScrollBars()
{
	EditItem(NULL);

	int nHeight = m_rcReportArea.Height();

	if (nHeight <= 0)
		return;

	BeginUpdate();

	int nCount = m_pRows->GetCount() - 1;
	int nLinesCount = nCount;

	{
		CClientDC dc(this);

		for (; nLinesCount >=0; nLinesCount--)
		{

			nHeight -= m_pRows->GetAt(nLinesCount)->GetHeight(&dc);

			if (nHeight < 0)
			{
				nLinesCount++;
				break;
			}
		}
	}

	nLinesCount = max(nLinesCount, 0);

	if (m_nTopRow > nLinesCount)
	{
		m_nTopRow = nLinesCount;
		SetScrollPos(SB_VERT, m_nTopRow);
	}

	BOOL bEnabled = nLinesCount > 0;

	if (bEnabled)
	{
		SCROLLINFO  si ;
		si.cbSize = sizeof(SCROLLINFO);
		si.nPage = nCount - nLinesCount + 1;
		si.nMax = nCount;
		si.nMin = 0 ;
		si.fMask = SIF_PAGE | SIF_RANGE ;
		SetScrollInfo(SB_VERT, &si) ;
	}

	if (bEnabled != m_bVScrollBarVisible)
	{
		EnableScrollBarCtrl(SB_VERT, bEnabled);
	}

	m_bVScrollBarVisible = bEnabled;


	if (!GetReportHeader()->m_bAutoColumnSizing)
	{
		int nHeaderWidth = max(GetReportHeader()->GetWidth() - m_rcReportArea.Width(), 0);

		if (m_nLeftOffset > nHeaderWidth)
		{
			m_nLeftOffset = nHeaderWidth;
			SetScrollPos(SB_HORZ, m_nLeftOffset);
		}

		bEnabled = nHeaderWidth > 0;

		if (bEnabled)
		{
			SCROLLINFO  si ;
			si.cbSize = sizeof(SCROLLINFO);
			si.nPage = m_rcReportArea.Width();
			si.nMax = si.nPage + nHeaderWidth;
			si.nMin = 0 ;
			si.fMask = SIF_PAGE | SIF_RANGE ;
			SetScrollInfo(SB_HORZ, &si) ;
		}

		EnableScrollBarCtrl(SB_HORZ, bEnabled);
	}


	AdjustLayout();

	EndUpdate();
}

//////////////////////////////////////////////////////////////////////////
// CXTPReportControl message handlers

void CXTPReportControl::OnPaint()
{
	CPaintDC dc(this);      // device context for painting
	CXTPClientRect rc(this);

	// start counting drawing time
#ifdef XTP_DEBUG
	LARGE_INTEGER iStartCount;
	QueryPerformanceCounter(&iStartCount);
#endif

	if (m_nLockUpdateCount == 0 && (IsChanged() || m_bmpCache.GetSafeHandle() == 0))
	{
		CDC memDC;
		memDC.CreateCompatibleDC(&dc);

		m_bmpCache.DeleteObject();
		m_bmpCache.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());


		CBitmap* pOldBitmap = memDC.SelectObject(&m_bmpCache);

		OnDraw(&memDC);

		dc.BitBlt(0, 0, rc.right, rc.bottom, &memDC, 0, 0, SRCCOPY);

		memDC.SelectObject(pOldBitmap);
	}
	else
	{
		XTP_TRACE(_T("Cached: "));
		dc.DrawState(CPoint(0, 0), rc.Size(), &m_bmpCache, 0, 0);
	}

	// count drawing time
#ifdef XTP_DEBUG
	LARGE_INTEGER iEndCount;
	QueryPerformanceCounter(&iEndCount);
	XTP_TRACE(_T("Draw counter ticks: %d\n"), iEndCount.LowPart-iStartCount.LowPart);
#endif

}

BOOL CXTPReportControl::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;    // Don't erase the background.
}

void CXTPReportControl::OnDraw(CDC* pDC)
{
	// draw new image
	pDC->FillSolidRect(CXTPClientRect(this), GetPaintManager()->GetControlBackColor(this));

	CXTPReportHeader* pHeader = m_pColumns->GetReportHeader();

	CRect rcHeader(m_rcHeaderArea.left - m_nLeftOffset, m_rcHeaderArea.top,
		m_rcHeaderArea.left - m_nLeftOffset + pHeader->GetWidth(), m_rcHeaderArea.bottom);

	// draw header
	if (pHeader)
		pHeader->Draw(pDC, rcHeader);

	// draw group by box
	if (pHeader)
		pHeader->DrawGroupByControl(pDC, m_rcGroupByArea);

	DrawRows(pDC, m_rcReportArea);



	// update flag
	SetChanged(FALSE);
}


void CXTPReportControl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	AdjustLayout();
	AdjustScrollBars();
}

void CXTPReportControl::OnRButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnRButtonDown(nFlags, point);

}

void CXTPReportControl::OnRButtonUp(UINT nFlags, CPoint point)
{
	CWnd::OnRButtonUp(nFlags, point);

}


void CXTPReportControl::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDown(nFlags, point);
	SetFocus();
	EditItem(NULL);

	// columns processing
	CXTPReportHeader* pHeader = m_pColumns->GetReportHeader();
	if (pHeader)
	{
		pHeader->OnLButtonDown(point);
	}

	// rows selection
	CXTPReportRow* pRow = HitTest(point);
	if (pRow)
	{
		bool bIgnoreSelection = GetKeyState(VK_CONTROL) < 0;
		if (bIgnoreSelection)
			m_pSelectedRows->Invert(pRow);

		bool bSelectBlock = GetKeyState(VK_SHIFT) < 0;


		CXTPReportColumn* pFocusedColumn = NULL;
		pRow->HitTest(point, NULL, &pFocusedColumn);

		CXTPReportControlUpdateContext updateContext(this);

		if (m_bFocusSubItems && pFocusedColumn && pRow->GetRecord() && pRow->GetRecord()->GetItem(pFocusedColumn)->IsFocusable())
		{
			SetFocusedColumn(pFocusedColumn);
		}

		SetFocusedRow(pRow, bSelectBlock, bIgnoreSelection);

		m_pointDrag = point;
		m_bPrepareDrag = TRUE;

	}

}

void CXTPReportControl::OnLButtonUp(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonUp(nFlags, point);

	m_bPrepareDrag = FALSE;

	// columns processing
	CXTPReportHeader* pHeader = m_pColumns->GetReportHeader();
	if (pHeader)
	{
		pHeader->OnLButtonUp(nFlags, point);
	}

	if (!m_bMultipleSelection || (GetKeyState(VK_SHIFT) >= 0 && GetKeyState(VK_CONTROL) >= 0) )
	{
		// rows processing
		CXTPReportRow* pRow = HitTest(point);
		if (pRow && pRow->IsFocused())
		{
			pRow->OnClick(point);
		}
	}
}

void CXTPReportControl::OnLButtonDblClk(UINT nFlags, CPoint ptDblClick)
{
	CWnd::OnLButtonDblClk(nFlags, ptDblClick);

	// rows processing
	CXTPReportRow* pRow = HitTest(ptDblClick);
	if (pRow)
	{
		pRow->OnDblClick(ptDblClick);
	}

}
void CXTPReportControl::OnContextMenu(CWnd* /*pWnd*/, CPoint pos)
{
	if (GetMouseMode() != xtpReportMouseNothing)
		return;

	CPoint ptClient = pos;
	ScreenToClient(&ptClient);

	// call context menu handler for report header if clicked inside
	if (m_rcHeaderArea.PtInRect(ptClient) ||
		m_rcGroupByArea.PtInRect(ptClient) )
	{
		CXTPReportHeader* pHeader = m_pColumns->GetReportHeader();
		if (pHeader)
			pHeader->OnContextMenu(ptClient);
		return;
	}

	// call context menu handler for report area if clicked inside
	if (m_rcReportArea.PtInRect(ptClient))
	{
		// rows processing
		CXTPReportRow* pRow = HitTest(ptClient);
		if (pRow)
		{
			SetFocusedRow(pRow, !pRow->IsGroupRow() || pRow->IsSelected());
			pRow->OnContextMenu(ptClient);
		}
	}

	CXTPReportRow* pFocusedRow = GetFocusedRow();
	if (pos == CPoint(-1, -1) && pFocusedRow)
	{
		ptClient = CPoint(pFocusedRow->GetRect().left, pFocusedRow->GetRect().bottom);
		pFocusedRow->OnContextMenu(ptClient);
	}

}

CXTPReportColumn* CXTPReportControl::GetNextFocusableColumn(CXTPReportRow* pRow, int nColumnIndex, int nDirection)
{
	if (!pRow->GetRecord())
		return NULL;

	while (TRUE)
	{
		CXTPReportColumn* pColumn = GetReportHeader()->GetNextVisibleColumn(nColumnIndex, nDirection);
		if (!pColumn)
			return NULL;

		CXTPReportRecordItem* pItem = pRow->GetRecord()->GetItem(pColumn);

		if (pItem && pItem->IsFocusable())
			return pColumn;

		nColumnIndex = pColumn->GetIndex();
	}
}

void CXTPReportControl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);

	if (m_pRows->GetCount() == 0)
			return;

	BOOL  bSelectBlock = GetKeyState(VK_SHIFT) < 0;
	BOOL  bSignoreSelection = GetKeyState(VK_CONTROL) < 0;

	CXTPReportRow* pFocusedRow = GetFocusedRow();

	switch (nChar)
	{
		case VK_RIGHT:
			if (pFocusedRow && pFocusedRow->HasChildren() && !pFocusedRow->IsExpanded())
			{
				pFocusedRow->SetExpanded(TRUE);
				break;
			}
			if (pFocusedRow && m_bFocusSubItems && m_pFocusedColumn)
			{
				CXTPReportColumn* pColumn = GetNextFocusableColumn(pFocusedRow, m_pFocusedColumn->GetIndex(), +1);
				if (pColumn)
				{
					SetFocusedColumn(pColumn);
					SetFocusedRow(pFocusedRow);
				}
				break;
			}
		case VK_DOWN: MoveDown(bSelectBlock, bSignoreSelection); break;
		case  VK_LEFT:
			if (pFocusedRow && pFocusedRow->HasChildren() && pFocusedRow->IsExpanded())
			{
				pFocusedRow->SetExpanded(FALSE);
				break;
			}
			if (pFocusedRow && m_bFocusSubItems && m_pFocusedColumn)
			{
				CXTPReportColumn* pColumn = GetNextFocusableColumn(pFocusedRow, m_pFocusedColumn->GetIndex(), -1);
				if (pColumn)
				{
					SetFocusedColumn(pColumn);
					SetFocusedRow(pFocusedRow);
				}
				break;
			}
			if (pFocusedRow && !pFocusedRow->HasChildren() && pFocusedRow->GetParentRow() && !bSelectBlock)
			{
				SetFocusedRow(pFocusedRow->GetParentRow());
				pFocusedRow->SetExpanded(FALSE);
				break;
			}
		case VK_UP: MoveUp(bSelectBlock, bSignoreSelection); break;
		case VK_HOME: MoveFirst(bSelectBlock, FALSE); break;
		case VK_END: MoveLast(bSelectBlock, FALSE); break;
		case VK_NEXT: MovePageDown(bSelectBlock, FALSE); break;
		case VK_PRIOR: MovePageUp(bSelectBlock, FALSE); break;
		case VK_RETURN:
			if (pFocusedRow && pFocusedRow->HasChildren())
			{
				pFocusedRow->SetExpanded(!pFocusedRow->IsExpanded());
			}
			break;

		case VK_ESCAPE:
			if (m_mouseMode != xtpReportMouseNothing)
			{
				m_pColumns->GetReportHeader()->CancelMouseMode();
			}
			break;
	}

	NMKEY nmgv;
	nmgv.nVKey        = nChar;
	nmgv.uFlags       = nFlags;
	SendNotifyMessage(NM_KEYDOWN, (NMHDR*)&nmgv);

#ifdef XTP_SITENOTIFY_KEY
	XTP_SITENOTIFY_KEY(this, TRUE, nChar)
#endif
}

void CXTPReportControl::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
#ifdef XTP_SITENOTIFY_KEY
	XTP_SITENOTIFY_KEY(this, TRUE, nChar)
#endif

	CWnd::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

void CXTPReportControl::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{

#ifdef XTP_SITENOTIFY_KEY
	XTP_SITENOTIFY_KEY(this, FALSE, nChar)
#endif
	CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CXTPReportControl::OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{

#ifdef XTP_SITENOTIFY_KEY
	XTP_SITENOTIFY_KEY(this, FALSE, nChar)
#endif
	CWnd::OnSysKeyUp(nChar, nRepCnt, nFlags);
}

void CXTPReportControl::OnChar(UINT nChar, UINT nRepCntr, UINT nFlags)
{
	CXTPReportRow* pFocusedRow = GetFocusedRow();

	if (m_bFocusSubItems && pFocusedRow && (nChar == VK_TAB))
	{
		EditItem(0);

		BOOL bBack = GetKeyState(VK_SHIFT) < 0;

		CXTPReportControlUpdateContext updateContext(this);

		CXTPReportColumn* pFocusedColumn = GetNextFocusableColumn(pFocusedRow, m_pFocusedColumn? m_pFocusedColumn->GetIndex(): -1, bBack? -1: 1);

		if (pFocusedColumn)
		{
			SetFocusedColumn(pFocusedColumn);
		}
		else
		{
			CXTPReportRow* pRow = bBack? m_pRows->GetPrev(pFocusedRow, FALSE): m_pRows->GetNext(pFocusedRow, FALSE);
			if (pRow && pRow->GetIndex() != m_nFocusedRow)
			{
				SetFocusedRow(pRow);
				SetFocusedColumn(GetNextFocusableColumn(GetFocusedRow(), bBack? m_pColumns->GetCount(): -1, bBack? -1: +1));
			}
		}

		return;
	}

	if (m_pFocusedColumn && pFocusedRow && pFocusedRow->GetRecord() && (nChar != VK_RETURN) && (nChar != VK_ESCAPE))
	{
		XTP_REPORTRECORDITEM_ARGS itemArgs(this, pFocusedRow, m_pFocusedColumn) ;

		if (itemArgs.pItem && itemArgs.pItem->OnChar(&itemArgs, nChar))
			return;
	}

	CWnd::OnChar(nChar, nRepCntr, nFlags);
}

void CXTPReportControl::OnCaptureChanged(CWnd *pWnd)
{
	if (m_mouseMode != xtpReportMouseNothing)
	{
		m_pColumns->GetReportHeader()->CancelMouseMode();
	}
	CWnd::OnCaptureChanged(pWnd);
}

void CXTPReportControl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar != NULL)
	{
		CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
		return;
	}
	int nCurPos = m_nTopRow;

	// decide what to do for each diffrent scroll event
	switch(nSBCode)
	{
		case SB_TOP: nCurPos = 0; break;
		case SB_BOTTOM: nCurPos = GetScrollLimit(SB_VERT); break;
		case SB_LINEUP: nCurPos = max(nCurPos - 1, 0); break;
		case SB_LINEDOWN: nCurPos = min(nCurPos + 1, GetScrollLimit(SB_VERT)); break;

		case SB_PAGEUP: nCurPos = max(nCurPos - GetReportAreaRows(nCurPos, false), 0); break;
		case SB_PAGEDOWN: nCurPos = min(nCurPos + GetReportAreaRows(nCurPos, true), GetScrollLimit(SB_VERT)); break;

		case SB_THUMBTRACK:
		case SB_THUMBPOSITION:
			{
				SCROLLINFO si;
				ZeroMemory(&si, sizeof(SCROLLINFO));
				si.cbSize = sizeof(SCROLLINFO);
				si.fMask = SIF_TRACKPOS;

				if (!GetScrollInfo(SB_VERT, &si) )
					return;
				nCurPos = si.nTrackPos;
			}
			break;
	}

	SetTopRow(nCurPos);

	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CXTPReportControl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar != NULL)
	{
		CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
		return;
	}
	int nCurPos = m_nLeftOffset;

	// decide what to do for each diffrent scroll event
	switch(nSBCode)
	{
		case SB_TOP: nCurPos = 0; break;
		case SB_BOTTOM: nCurPos = GetScrollLimit(SB_HORZ); break;
		case SB_LINEUP: nCurPos = max(nCurPos - 1, 0); break;
		case SB_LINEDOWN: nCurPos = min(nCurPos + 1, GetScrollLimit(SB_HORZ)); break;
		case SB_PAGEUP: nCurPos = max(nCurPos - m_rcReportArea.Width(), 0); break;
		case SB_PAGEDOWN: nCurPos = min(nCurPos + m_rcReportArea.Width(), GetScrollLimit(SB_HORZ)); break;

		case SB_THUMBTRACK:
		case SB_THUMBPOSITION:  nCurPos = nPos; break;
	}

	SetLeftOffset(nCurPos);

	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

UINT CXTPReportControl::GetMouseScrollLines()
{
	int nScrollLines = 3; // default value

	// win95
	OSVERSIONINFO VersionInfo;
	VersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if (!GetVersionEx(&VersionInfo) ||
		(VersionInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS && VersionInfo.dwMinorVersion == 0))
	{

		HKEY hKey;
		if (ERROR_SUCCESS == RegOpenKeyEx(
				HKEY_CURRENT_USER, _T("Control Panel\\Desktop"), 0, KEY_QUERY_VALUE, &hKey))
		{
			TCHAR szData[128];
			DWORD dwKeyDataType;
			DWORD dwDataBufSize = sizeof(szData);

			if (ERROR_SUCCESS == RegQueryValueEx(
					hKey, _T("WheelScrollLines"), NULL, &dwKeyDataType, (LPBYTE) &szData, &dwDataBufSize) )
			{
				nScrollLines = _tcstoul(szData, NULL, 10);
			}
			RegCloseKey(hKey);
		}
	}
	// win98 or greater
	else
		SystemParametersInfo (SPI_GETWHEELSCROLLLINES, 0, &nScrollLines, 0);

	return nScrollLines;
}

BOOL CXTPReportControl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	UINT uiMsg;
	int nScrollsCount = 0;
	// calculate what should be sent
	if (m_nRowsPerWheel == -1)
	{
		// A m_nRowsPerWheel value less than 0 indicates that the mouse wheel scrolls whole pages, not just lines.
		int nPagesScrolled = zDelta / 120;
		uiMsg = nPagesScrolled > 0 ? SB_PAGEUP : SB_PAGEDOWN;
		nScrollsCount = nPagesScrolled > 0 ? nPagesScrolled : -nPagesScrolled;
	}
	else
	{
		int nRowsScrolled = m_nRowsPerWheel * zDelta / 120;
		uiMsg = nRowsScrolled > 0 ? SB_LINEUP : SB_LINEDOWN;
		nScrollsCount = nRowsScrolled > 0 ? nRowsScrolled : -nRowsScrolled;
	}
	// send scroll messages
	for (int i = 0; i < nScrollsCount; i++)
	{
		PostMessage(WM_VSCROLL, uiMsg, 0);
	}

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CXTPReportControl::OnMouseLeave()
{
	OnMouseMove(0, CPoint(-1, -1));
}
void CXTPReportControl::OnMouseMove(UINT nFlags, CPoint point)
{
	CWnd::OnMouseMove(nFlags, point);

	CXTPReportHeader* pHeader = m_pColumns->GetReportHeader();
	if (pHeader)
	{
		pHeader->OnMouseMove(nFlags, point);
	}

	if (GetMouseMode() == xtpReportMouseNothing)
	{
		CXTPReportRow* pRow = HitTest(point);
		if (pRow)
		{
			pRow->OnMouseMove(nFlags, point);

			if (m_bShowTooltips && nFlags == 0)
			{
				pRow->ShowToolTip(point, &m_wndTip);
			}
		}

		if (m_pHotRow != pRow)
		{
			TRACKMOUSEEVENT tme = {sizeof(TRACKMOUSEEVENT), TME_LEAVE, GetSafeHwnd(), 0};
			_TrackMouseEvent (&tme);

			m_pHotRow = pRow;
		}

		// If mouse moved some since down...
		if (m_bPrepareDrag && (labs (point.x - m_pointDrag.x) > 3 ||
			labs (point.x - m_pointDrag.x) > 3))
		{
			// Prevent duplicate
			m_bPrepareDrag = FALSE;

			// Begin a drag operation
			OnBeginDrag(m_pointDrag);
		}
	}



}

void CXTPReportControl::OnBeginDrag(CPoint point)
{
	SendMessageToParent(NULL, NULL, NULL, LVN_BEGINDRAG, &point);
}

BOOL CXTPReportControl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (nHitTest == HTCLIENT)
	{
		switch (m_mouseMode)
		{
		case xtpReportMouseOverColumnDivide:
			SetCursor(m_pColumns->GetReportHeader()->m_hResizeCursor);
			break;
		default:
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		}
		return TRUE;
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}


LRESULT CXTPReportControl::SendNotifyMessage(UINT nMessage, NMHDR* pNMHDR) const
{
	if (!IsWindow(m_hWnd))
		return 0;

	NMHDR nmhdr;
	if (pNMHDR == NULL)
		pNMHDR = &nmhdr;

	pNMHDR->hwndFrom = GetSafeHwnd();
	pNMHDR->idFrom   = GetDlgCtrlID();
	pNMHDR->code     = nMessage;

	CWnd *pOwner = GetOwner();
	if (pOwner && IsWindow(pOwner->m_hWnd))
		return pOwner->SendMessage(WM_NOTIFY, pNMHDR->idFrom, (LPARAM)pNMHDR);
	else
		return 0;
}


LRESULT CXTPReportControl::SendMessageToParent(CXTPReportRow* pRow, CXTPReportRecordItem* pItem, CXTPReportColumn* pColumn, UINT nMessage, CPoint* pPoint, int nHyperlink) const
{
	if (!IsWindow(m_hWnd))
		return 0;

	XTP_NM_REPORTRECORDITEM nmgv;
	nmgv.pItem      = pItem;
	nmgv.pColumn    = pColumn;
	nmgv.pRow       = pRow;
	nmgv.nHyperlink = nHyperlink;
	nmgv.pt.x       = 0;
	nmgv.pt.y       = 0;
	if (pPoint)
	{
		nmgv.pt = *pPoint;
	}

	return SendNotifyMessage(nMessage, (NMHDR*)&nmgv);
}

void CXTPReportControl::DoPropExchange(CXTPPropExchange* pPX)
{
	TRY
	{
		CXTPPropExchangeSection secColumns(pPX->GetSection(_T("Columns")));
		m_pColumns->DoPropExchange(&secColumns);
		
		PX_Bool(pPX, _T("ShowGroupBox"), m_bGroupByEnabled, FALSE);
		
		if (pPX->IsLoading())
		{
			GetReportHeader()->OnColumnsChanged();
			Populate();
		}
	}
	CATCH(CArchiveException, e)
	{
	}
	END_CATCH
}

void CXTPReportControl::SerializeState(CArchive& ar)
{
	/*
	TRY
	{
		m_pColumns->Serialize(ar);

		// serialize simple properties
		if (ar.IsLoading())
		{
			ar  >> m_bGroupByEnabled;

			GetReportHeader()->OnColumnsChanged();
			Populate();
		}
		else if (ar.IsStoring())
		{
			ar  << m_bGroupByEnabled;
		}
	}
	CATCH(CArchiveException, e)
	{
	}
	END_CATCH*/

	CXTPPropExchangeArchive px(ar);
	DoPropExchange(&px);
}

void CXTPReportControl::CollapseAll()
{
	BeginUpdate();
	for (int i = m_pRows->GetCount() - 1; i >= 0; i --)
		m_pRows->GetAt(i)->SetExpanded(FALSE);

	EndUpdate();
	EnsureVisible(GetFocusedRow());
}

void CXTPReportControl::ExpandAll()
{
	BeginUpdate();
	for (int i = m_pRows->GetCount() - 1; i >= 0; i --)
		m_pRows->GetAt(i)->SetExpanded(TRUE);

	EndUpdate();
	EnsureVisible(GetFocusedRow());
}

void CXTPReportControl::SetMouseMode(XTPReportMouseMode nMode)
{
	XTP_TRACE(_T("SetMouseMode: Switching from %d to %d\n"), m_mouseMode, nMode);
	m_mouseMode = nMode;
}

CXTPReportHeader* CXTPReportControl::GetReportHeader()
{
	return m_pColumns->GetReportHeader();
}

void CXTPReportControl::RelayToolTipEvent(UINT message)
{
	if (m_wndTip.GetSafeHwnd() && m_wndTip.IsWindowVisible())
	{
		CPoint pt;
		GetCursorPos(&pt);

		if (!m_wndTip.GetHoverRect().PtInRect(pt))
		{
			m_wndTip.SetTooltipText(NULL);
			m_wndTip.Activate(FALSE);
		}

		switch (message)
		{
			case WM_MOUSEWHEEL:
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
			case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_MBUTTONDOWN:
			case WM_LBUTTONUP:
			case WM_RBUTTONUP:
			case WM_MBUTTONUP:
			case WM_MOUSELEAVE:
				m_wndTip.Activate(FALSE);
		}
	}

}

BOOL CXTPReportControl::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	static BOOL bRelay = FALSE;
	if (m_wndTip.GetSafeHwnd() && m_wndTip.IsWindowVisible() && !bRelay)
	{
		bRelay = TRUE;
		RelayToolTipEvent(message);
		bRelay = FALSE;
	}


	return CWnd::OnWndMsg(message, wParam, lParam, pResult);
}

void CXTPReportControl::OnSysColorChange()
{
	CWnd::OnSysColorChange();
	m_pPaintManager->RefreshMetrics();
}

UINT CXTPReportControl::OnGetDlgCode()
{
	return m_bFocusSubItems? DLGC_WANTALLKEYS: DLGC_WANTARROWS;
}

void CXTPReportControl::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);
	RedrawControl();
}

void CXTPReportControl::OnKillFocus (CWnd* pNewWnd)
{
	CWnd::OnKillFocus(pNewWnd);
	RedrawControl();
}

int CXTPReportControl::GetHeaderIndent()
{
	return GetIndent(GetReportHeader()->m_nIndentLevel);
}

CXTPReportRow* CXTPReportControl::CreateRow()
{
	return new CXTPReportRow();
}

CXTPReportGroupRow* CXTPReportControl::CreateGroupRow()
{
	return new CXTPReportGroupRow();

}

void CXTPReportControl::EditItem(XTP_REPORTRECORDITEM_ARGS* pItemArgs)
{
	CXTPReportRecordItem* pItem = pItemArgs? pItemArgs->pItem: NULL;

	if (m_pActiveItem != NULL)
	{
		m_pActiveItem->OnCancelEdit(this, TRUE);
		m_pActiveItem = NULL;

		if (!m_bFocusSubItems)
		{
			SetFocusedColumn(NULL);
		}
	}

	if (pItem && pItemArgs)
	{
		SetFocusedColumn(pItemArgs->pColumn);
		m_pActiveItem = pItem;
		pItem->OnBeginEdit(pItemArgs);
	}

	RedrawControl();
}

BOOL CXTPReportControl::HasFocus()
{
	CWnd* pFocusWnd = GetFocus();
	if (!pFocusWnd)
		return FALSE;

	return (pFocusWnd == this) || (pFocusWnd->GetParent() == this) || (pFocusWnd->GetOwner() == this);
}


void CXTPReportControl::ReleaseItem(int nIndex)
{
	int i;

	for (i = 0; i < m_pRecords->GetCount(); i++)
	{
		CXTPReportRecord* pRecord = m_pRecords->GetAt(i);

		pRecord->m_arrItems[nIndex]->InternalRelease();
		pRecord->m_arrItems.RemoveAt(nIndex);
	}

	CXTPReportColumns* pColumns = GetColumns();

	for (i = 0; i < pColumns->GetCount(); i++)
	{
		CXTPReportColumn* pColumn = pColumns->GetAt(i);

		if (pColumn->m_nItemIndex > nIndex)
		{
			pColumn->m_nItemIndex--;
		}
		else if (pColumn->m_nItemIndex == nIndex)
		{
			pColumns->Remove(pColumn);
		}
	}
}

void CXTPReportControl::SetVirtualMode(CXTPReportRecord* pVirtualRecord, int nCount)
{
	m_pRecords->SetVirtualMode(pVirtualRecord, nCount);
}

BOOL CXTPReportControl::IsVirtualMode()
{
	return m_pRecords->IsVirtualMode();
}

CXTPReportRow* CXTPReportControl::GetFocusedRow()
{
	return m_nFocusedRow != -1? m_pRows->GetAt(m_nFocusedRow): NULL;
}
