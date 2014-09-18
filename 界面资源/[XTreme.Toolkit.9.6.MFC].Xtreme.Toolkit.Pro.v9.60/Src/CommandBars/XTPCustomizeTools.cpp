// XTPCustomizeTools.cpp.
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
#include "XTPResourceManager.h"
#include "XTPDrawHelpers.h"

#include "XTPCommandBarsResource.h"
#include "XTPCustomizeTools.h"
#include "XTPCommandBar.h"
#include "XTPControl.h"
#include "XTPToolBar.h"
#include "XTPCommandBars.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPCustomizeDropSource


CXTPCustomizeDropSource::CXTPCustomizeDropSource(CXTPCommandBars* pCommandBars)
{
	m_hcurDelete = AfxGetApp ()->LoadCursor (XTP_IDC_COMMANDBARS_DRAGDELETE);
	m_hcurMove = AfxGetApp ()->LoadCursor (XTP_IDC_COMMANDBARS_DRAGMOVE);
	m_hcurCopy = AfxGetApp ()->LoadCursor (XTP_IDC_COMMANDBARS_DRAGCOPY);
	m_pSheet = NULL;
	m_pCommandBars = pCommandBars;
}


CXTPCustomizeDropSource::~CXTPCustomizeDropSource()
{
}

DROPEFFECT CXTPCustomizeDropSource::DoDragDrop(CXTPControl* pControl, BOOL bCopyOnly)
{
	ASSERT(m_pCommandBars);
	CWnd* pWndCapture = m_pSheet? (CWnd*)m_pSheet: m_pCommandBars->GetSite();
	ASSERT(pWndCapture);

	m_pControl = pControl;
	m_hwndCapture = pWndCapture->GetSafeHwnd();
	::SetCapture(m_hwndCapture);
	m_bMove = FALSE;
	m_pTarget = NULL;
	m_bCopyOnly = bCopyOnly;

	GetCursorPos(&m_ptStart);

	DROPEFFECT dropEffect = _DoDragDrop();

	if (m_pTarget) m_pTarget->OnCustomizeDragLeave();

	ReleaseCapture();
	::SetCursor(LoadCursor(NULL, IDC_ARROW));
	return dropEffect;
}

void CXTPCustomizeDropSource::Register(CXTPCommandBar* pCommandBar)
{
	m_arrTargets.Add(pCommandBar);

}
void CXTPCustomizeDropSource::UnRegister(CXTPCommandBar* pCommandBar)
{
	for (int i = 0; i < m_arrTargets.GetSize(); i++)
	{
		if (pCommandBar == m_arrTargets[i])
		{
			m_arrTargets.RemoveAt(i);
		}
	}
}

CXTPCustomizeSheet* CXTPCustomizeDropSource::GetSheet()
{
	return m_pSheet;
}


// picks the current target
void CXTPCustomizeDropSource::PickTarget(CPoint point)
{

	BOOL bFound = FALSE;

	for (int i = (int)m_arrTargets.GetSize() - 1; i >= 0; i--)
	{
		CXTPCommandBar* pCommandBar = m_arrTargets[i];

		if (pCommandBar->IsVisible() && CXTPWindowRect(pCommandBar).PtInRect(point))
		{
			bFound = TRUE;

			if (m_pTarget == pCommandBar)
			{
				m_pTarget->ScreenToClient(&point);
				m_pTarget->OnCustomizeDragOver(m_pControl, point);
			}
			else if (m_pTarget != NULL)
			{
				m_pTarget->OnCustomizeDragLeave();
				m_pTarget = 0;
			}

			if (m_pTarget == 0)
			{
				pCommandBar->ScreenToClient(&point);
				if (pCommandBar->OnCustomizeDragEnter(m_pControl, point) != DROPEFFECT_NONE)
				{
					m_pTarget = pCommandBar;
				}
			}
			break;
		}
	}

	if (!bFound && m_pTarget)
	{
		m_pTarget->OnCustomizeDragLeave();
		m_pTarget = 0;
	}


	// set the cursor as appropriate
	FreshenCursor();
}

void CXTPCustomizeDropSource::FreshenCursor()
{
	HCURSOR hCursor = m_hcurDelete;
	if (m_pTarget)
	{
		hCursor = (m_bMove &&  !m_bCopyOnly)? m_hcurMove: m_hcurCopy;
	}
	::SetCursor(hCursor);
}


DROPEFFECT CXTPCustomizeDropSource::_DoDragDrop()
{
	BOOL bMoved = FALSE;

	DROPEFFECT dropEffect = DROPEFFECT_CANCEL;

	BOOL bDone = FALSE;

	while (!bDone && ::GetCapture() == m_hwndCapture)
	{
		MSG msg;

		// assess current state
		// pump WM_PAINT first for better feedback
		while (::PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, WM_PAINT, WM_PAINT))
				break;
			DispatchMessage(&msg);
		}

		// get a message
		if (!::GetMessage(&msg, NULL, 0, 0))
		{
			AfxPostQuitMessage((int)msg.wParam);
			return DROPEFFECT_CANCEL;
		}

		if (::GetCapture() != m_hwndCapture)
		{
			// capture was stolen while repainting
			return DROPEFFECT_CANCEL;
		}

		switch (msg.message)
		{
		case WM_LBUTTONUP:
			bDone = TRUE;
			break;

		case WM_MOUSEMOVE:
			if (!bMoved)
			{
				CPoint pt(msg.pt);
				if (m_pControl && m_pControl->GetParent())
				{
					DROPEFFECT dropEffect;
					m_pControl->GetParent()->ScreenToClient(&pt);
					m_pControl->OnCustomizeDragOver(m_pControl, pt, dropEffect);
				}

				bMoved = TRUE;
			}
			m_bMove = !(msg.wParam & MK_CONTROL);
			PickTarget(msg.pt);

			// terminate loop if we happen to get mouse move with button up
			if (!(msg.wParam & MK_LBUTTON))
				bDone = TRUE;

			dropEffect = DROPEFFECT_NONE;
			break;

		case WM_KEYDOWN:
			if (msg.wParam == VK_ESCAPE)
				return DROPEFFECT_CANCEL;

			if (msg.wParam == VK_CONTROL && m_bMove)
			{
				m_bMove = FALSE;
				FreshenCursor();
			}
			break;

		case WM_KEYUP:
			if (msg.wParam == VK_CONTROL && !m_bMove)
			{
				m_bMove = TRUE;
				FreshenCursor();
			}
			break;

		case WM_RBUTTONDOWN:
			return DROPEFFECT_CANCEL;

		default:
			::DispatchMessage(&msg);
		}
	}


	if (m_pTarget)
	{

		dropEffect = m_bMove? DROPEFFECT_MOVE: DROPEFFECT_COPY;

		CPoint point;
		GetCursorPos(&point);

		m_pTarget->ScreenToClient(&point);
		m_pTarget->ScreenToClient(&m_ptStart);

		m_pTarget->OnCustomizeDrop(m_pControl, dropEffect, point, m_ptStart);
		m_pTarget = 0;
	}

	return dropEffect;
}
/////////////////////////////////////////////////////////////////////////////
// CXTPNewToolbarDlg dialog


CXTPNewToolbarDlg::CXTPNewToolbarDlg(CWnd* pParent, CXTPCommandBars* pCommandBars, CXTPCommandBar* pCommandBar)
	: m_pCommandBars(pCommandBars), m_pCommandBar(pCommandBar)
{
	InitModalIndirect(XTPResourceManager()->LoadDialogTemplate(MAKEINTRESOURCE(XTP_IDD_NEWTOOLBAR )), pParent);

	//{{AFX_DATA_INIT(CXTPNewToolbarDlg)
	m_strToolbar = _T("");
	//}}AFX_DATA_INIT
}


void CXTPNewToolbarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXTPNewToolbarDlg)
	DDX_Text(pDX, XTP_IDC_EDIT_TOOLBARNAME, m_strToolbar);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CXTPNewToolbarDlg, CDialog)
	//{{AFX_MSG_MAP(CXTPNewToolbarDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPNewToolbarDlg message handlers

BOOL CXTPNewToolbarDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (m_pCommandBar)
	{
		m_strToolbar = m_pCommandBar->GetTitle();
		UpdateData(FALSE);

		CString strWindowText;
		XTPResourceManager()->LoadString(&strWindowText, XTP_IDS_RENAMETOOLBAR);
		SetWindowText(strWindowText);

	}
	else
	{
		SetSuggestedName();
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CXTPNewToolbarDlg::SetSuggestedName()
{
	int iNewID = 1;
	CString strCustom;
	strCustom.Format(XTP_IDS_CUSTOM_BAR, iNewID);

	int nCount = m_pCommandBars->GetCount();
	int nIndex = 0;
	while (nIndex < nCount)
	{
		CXTPToolBar* pBar = m_pCommandBars->GetAt(nIndex);
		ASSERT(pBar != NULL);

		if (strCustom.Compare(pBar->GetTitle()) == 0)
		{
			strCustom.Format(XTP_IDS_CUSTOM_BAR, iNewID++);
			nIndex = 0;
			continue;
		}
		nIndex++;
	}

	m_strToolbar = strCustom;
	UpdateData(FALSE);
}

void CXTPNewToolbarDlg::OnOK()
{
	UpdateData();

	// If no text was entered, alert user.
	if (m_strToolbar.IsEmpty()) {
		AfxMessageBox(XTP_IDS_ERROR_BLANKNAME, MB_ICONSTOP);
		return;
	}

	m_nNewID = AFX_IDW_CONTROLBAR_FIRST;

	// Loop through all of the existing control bars to find
	// an available ID to use.
	int nCount = m_pCommandBars->GetCount();
	int nIndex = 0;
	while (nIndex < nCount)
	{
		CXTPToolBar* pBar = m_pCommandBars->GetAt(nIndex);
		ASSERT(pBar != NULL);

		// We found a control bar with the same ID as m_nNewID, increment
		// m_nNewID and reset the position back to the head.
		if (m_pCommandBar == NULL && pBar->GetBarID() == m_nNewID)
		{
			m_nNewID++;

			// If m_nNewID is greater than the maximum number of allowed
			// custom commands, alert the user and abort.
			if ((int)m_nNewID >= AFX_IDW_CONTROLBAR_LAST)
			{
				AfxMessageBox(XTP_IDS_ERROR_LIMIT, MB_ICONSTOP);
				CDialog::OnCancel();
				return;
			}

			nIndex = 0;
			continue;
		}


		// Now check to see if the title for the toolbar has already
		// been used, if so, alert the user and return.
		if (m_pCommandBar != pBar && m_strToolbar.Compare(pBar->GetTitle()) == 0)
		{
			CString strName;
			strName.Format(XTP_IDS_ERROR_EXISTS, m_strToolbar);
			AfxMessageBox(strName, MB_ICONSTOP);
			return;
		}

		nIndex ++;
	}

	CDialog::OnOK(); // success!
}
