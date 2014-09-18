// ItemTreeView.cpp : implementation file
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
#include "GUI_Outlook.h"
#include "ItemTreeView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef enum
{
	IDX_OUTLOOK_TODAY,
	IDX_CALENDAR,
	IDX_CONTACTS,
	IDX_DELETED_ITEMS,
	IDX_DRAFTS,
	IDX_INBOX,
	IDX_JOURNAL,
	IDX_NOTES,
	IDX_OUTBOX,
	IDX_SENT_ITEMS,
	IDX_TASKS,
	IDX_COUNT
} TREE_INDEX;

typedef struct
{
	UINT    m_nIndex;
	UINT    m_nTextID;
	UINT    m_nIconID;
}
TREE_ENTRY;

static TREE_ENTRY TreeEntries[] =
{
	{ IDX_OUTLOOK_TODAY,    IDS_OUTLOOK_TODAY,  IDI_OUTLOOK_TODAY   },
	{ IDX_CALENDAR,         IDS_CALENDAR,       IDI_CALENDAR        },
	{ IDX_CONTACTS,         IDS_CONTACTS,       IDI_CONTACTS        },
	{ IDX_DELETED_ITEMS,    IDS_DELETED_ITEMS,  IDI_DELETED_ITEMS   },
	{ IDX_DRAFTS,           IDS_DRAFTS,         IDI_DRAFTS          },
	{ IDX_INBOX,            IDS_INBOX,          IDI_INBOX           },
	{ IDX_JOURNAL,          IDS_JOURNAL,        IDI_JOURNAL         },
	{ IDX_NOTES,            IDS_NOTES,          IDI_NOTES           },
	{ IDX_OUTBOX,           IDS_OUTBOX,         IDI_OUTBOX          },
	{ IDX_SENT_ITEMS,       IDS_SENT_ITEMS,     IDI_SENT_ITEMS      },
	{ IDX_TASKS,            IDS_TASKS,          IDI_TASKS           }
};

/////////////////////////////////////////////////////////////////////////////
// CItemTreeView

IMPLEMENT_DYNCREATE(CItemTreeView, CView)

CItemTreeView::CItemTreeView()
{

}

CItemTreeView::~CItemTreeView()
{
	m_ilTreeIcons.DeleteImageList();
}

BEGIN_MESSAGE_MAP(CItemTreeView, CView)
	//{{AFX_MSG_MAP(CItemTreeView)
	ON_WM_CREATE()
	ON_WM_WINDOWPOSCHANGED()
	ON_BN_CLICKED(IDC_CAPT_BUTTON, OnCaptButton)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_CTRL, OnSelchanged)
	ON_WM_SYSCOLORCHANGE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CItemTreeView drawing

void CItemTreeView::OnDraw(CDC* /*pDC*/)
{
}

/////////////////////////////////////////////////////////////////////////////
// CItemTreeView diagnostics

#ifdef _DEBUG
void CItemTreeView::AssertValid() const
{
	CView::AssertValid();
}

void CItemTreeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CItemTreeView message handlers

int CItemTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Create the caption.
	if (!m_wndCaption.Create( this, _T("Folder List"), CPWS_EX_RAISED_EDGE | CPWS_EX_CLOSEBUTTON,
		WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, CRect(0,0,0,0), IDC_CAPT_BUTTON ))
	{
		TRACE0( "Unable to create caption.\n" );
		return -1;
	}

	// create the tree control.
	if (!m_wndTreeCtrl.Create (WS_VISIBLE|TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS,
		CRect(0,0,0,0), this, IDC_TREE_CTRL))
	{
		TRACE0( "Unable to create tree control.\n" );
		return -1;
	}

	// create the image lists used by the caption and tree controls.
	if (!InitializeImageLists())
		return -1;

	// fill the tree control.
	if (!InitilalizeTreeControl())
		return -1;

	return 0;
}

void CItemTreeView::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos)
{
	CView::OnWindowPosChanged(lpwndpos);

	// Move the caption and tree control to their correct locations.
	if( ::IsWindow( m_wndCaption.m_hWnd ) )
	{
		::SetWindowPos( m_wndCaption.m_hWnd, NULL, 0, 0, lpwndpos->cx - 2, 19,
			SWP_NOZORDER | SWP_NOACTIVATE );
	}
	if( ::IsWindow( m_wndTreeCtrl.m_hWnd ) )
	{
		::SetWindowPos( m_wndTreeCtrl.m_hWnd, NULL, 0, 19, lpwndpos->cx, lpwndpos->cy-19,
			SWP_NOZORDER | SWP_NOACTIVATE );
	}
}

void CItemTreeView::OnCaptButton()
{
	CMainFrame* pFrameWnd = (CMainFrame*)GetParentFrame();
	ASSERT_KINDOF (CMainFrame, pFrameWnd);
	pFrameWnd->CloseTreeViewPane();
}

void CItemTreeView::OnSelchanged(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	// Get the selected tree item and its icon.
	int nImage;
	HTREEITEM htItem = m_wndTreeCtrl.GetSelectedItem();
	if (htItem != NULL)
	{
		m_wndTreeCtrl.GetItemImage( htItem, nImage, nImage );

		CMainFrame* pFrameWnd = (CMainFrame*)GetParentFrame();
		ASSERT_KINDOF (CMainFrame, pFrameWnd);
		pFrameWnd->UpdateCaption(m_wndTreeCtrl.GetItemText( htItem ),
			m_ilTreeIcons.ExtractIcon(nImage));
	}

	*pResult = 0;
}

BOOL CItemTreeView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CView::PreCreateWindow(cs))
		return FALSE;

	// TODO: Add your specialized code here and/or call the base class
	cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;

	return TRUE;
}

BOOL CItemTreeView::InitilalizeTreeControl()
{
	// Set the tree controls image list.
	m_wndTreeCtrl.SetImageList (&m_ilTreeIcons, TVSIL_NORMAL);

	// TODO: Add items to the tree control.
	HTREEITEM hItem = NULL;
	CString   str;

	int nEntry;
	for (nEntry = 0; nEntry < _countof(TreeEntries); nEntry++)
	{
		TREE_ENTRY* pEntry = &TreeEntries[nEntry];

		str.LoadString (pEntry->m_nTextID);

		if (nEntry == 0)
			hItem = m_wndTreeCtrl.InsertItem (str, pEntry->m_nIndex, pEntry->m_nIndex);
		else
					m_wndTreeCtrl.InsertItem (str, pEntry->m_nIndex, pEntry->m_nIndex, hItem);
	}

	m_wndTreeCtrl.SetItemBold (hItem, TRUE);
	m_wndTreeCtrl.Expand (hItem, TVE_EXPAND);
	m_wndTreeCtrl.SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED);
	m_wndTreeCtrl.EnableMultiSelect(FALSE);

	return TRUE;
}

BOOL CItemTreeView::InitializeImageLists()
{
	// Create the image list used by the tree control.
	if (!m_ilTreeIcons.Create(16, 16, ILC_MASK|ILC_COLOR32, 2, 1))
		return FALSE;

	// Load icons displayed in the tree control and add them to the image list.
	int nEntry;
	for (nEntry = 0; nEntry < _countof(TreeEntries); nEntry++)
	{
		TREE_ENTRY* pEntry = &TreeEntries[nEntry];

		HICON hIcon = AfxGetApp()->LoadIcon(pEntry->m_nIconID);
		ASSERT(hIcon);

		m_ilTreeIcons.Add (hIcon);
	}

	return TRUE;
}

void CItemTreeView::OnSysColorChange()
{
	CView::OnSysColorChange();

	m_wndCaption.SetCaptionColors(
		::GetSysColor(COLOR_3DFACE),
		::GetSysColor(COLOR_3DFACE),
		::GetSysColor(COLOR_BTNTEXT) );
}
