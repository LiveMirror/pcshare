// TaskPanelView.cpp : implementation of the CTaskPanelView class
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
#include "TaskPanel.h"

#include "TaskPanelDoc.h"
#include "TaskPanelView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


class CTaskPanelIconItem : public CXTPTaskPanelGroupItem
{
public:
	CTaskPanelIconItem(HICON hIcon)
	{
		m_hIcon = hIcon;

	}
	~CTaskPanelIconItem()
	{
		DestroyIcon(m_hIcon);
	}

	CRect OnReposition(CRect rc)
	{
		m_rcItem =  rc;
		m_rcItem.bottom = m_rcItem.top + 32;

		return m_rcItem;
	}
	void OnDrawItem(CDC* pDC, CRect rc)
	{
		pDC->DrawState(CPoint(rc.CenterPoint().x - 16, rc.CenterPoint().y - 16), 0, m_hIcon, 0, (CBrush*)NULL);
	}

	HICON m_hIcon;

};


/////////////////////////////////////////////////////////////////////////////
// CTaskPanelView

IMPLEMENT_DYNCREATE(CTaskPanelView, CXTListView)

BEGIN_MESSAGE_MAP(CTaskPanelView, CXTListView)
	//{{AFX_MSG_MAP(CTaskPanelView)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemchanged)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CXTListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CXTListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CXTListView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaskPanelView construction/destruction

CTaskPanelView::CTaskPanelView()
{
	m_ilIcons.Create(32, 32, ILC_COLOR32|ILC_MASK, 0, 1);

	CBitmap bmp;

	if (XTPImageManager()->IsAlphaIconsImageListSupported())
	{
		bmp.Attach((HBITMAP)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_FILEICONS_ALPHA),
			IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_CREATEDIBSECTION));

		m_ilIcons.Add(&bmp, (CBitmap*)NULL);
	} else
	{
		bmp.LoadBitmap(IDB_FILEICONS_NORMAL);
		m_ilIcons.Add(&bmp, RGB(0, 0xFF, 0));
	}
}

CTaskPanelView::~CTaskPanelView()
{
}

BOOL CTaskPanelView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style |= LVS_ICON|LVS_AUTOARRANGE;

	return CXTListView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTaskPanelView drawing

void CTaskPanelView::OnDraw(CDC* /*pDC*/)
{
	CTaskPanelDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CTaskPanelView printing

BOOL CTaskPanelView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTaskPanelView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTaskPanelView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CTaskPanelView diagnostics

#ifdef _DEBUG
void CTaskPanelView::AssertValid() const
{
	CXTListView::AssertValid();
}

void CTaskPanelView::Dump(CDumpContext& dc) const
{
	CXTListView::Dump(dc);
}

CTaskPanelDoc* CTaskPanelView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTaskPanelDoc)));
	return (CTaskPanelDoc*)m_pDocument;
}
#endif //_DEBUG



CXTPTaskPanel* CTaskPanelView::GetTaskPanel()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	ASSERT(pFrame);

	CXTPTaskPanel* pPanel = &pFrame->m_wndTaskPanel;

	ASSERT(pPanel->GetSafeHwnd());

	return pPanel;
}

/////////////////////////////////////////////////////////////////////////////
// CTaskPanelView message handlers

void CTaskPanelView::OnInitialUpdate()
{
	CXTListView::OnInitialUpdate();

	CListCtrl& wndList = GetListCtrl();

	wndList.SetImageList(&m_ilIcons, LVSIL_NORMAL);


	wndList.InsertItem(LVIF_TEXT|LVIF_PARAM|LVIF_IMAGE, 0, _T("Tools"), 0, 0, 0, TRUE);
	wndList.InsertItem(LVIF_TEXT|LVIF_PARAM|LVIF_IMAGE, 1, _T("Source"), 0, 0, 0, TRUE);
	wndList.InsertItem(LVIF_TEXT|LVIF_PARAM|LVIF_IMAGE, 2, _T("Program Files"), 0, 0, 0, TRUE);
	wndList.InsertItem(LVIF_TEXT|LVIF_PARAM|LVIF_IMAGE, 3, _T("Windows"), 0, 0, 0, TRUE);
	wndList.InsertItem(LVIF_TEXT|LVIF_PARAM|LVIF_IMAGE, 4, _T("Sound"), 0, 0, 1, FALSE);
	wndList.InsertItem(LVIF_TEXT|LVIF_PARAM|LVIF_IMAGE, 5, _T("Bitmap"), 0, 0, 2, FALSE);

}

void CTaskPanelView::OnItemchanged(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	//NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	//pNMListView;

	CListCtrl& wndList = GetListCtrl();

	int nCount = wndList.GetSelectedCount();

	CXTPTaskPanel* pPanel = GetTaskPanel();


	CXTPTaskPanelGroup* pGroupFiles = pPanel->FindGroup(ID_TASKGROUP_FILES);

	ASSERT(pGroupFiles);
	pGroupFiles->GetItems()->Clear();

	if (nCount == 0)
	{
		pGroupFiles->AddLinkItem(ID_TASKITEM_NEWFOLDER, 3);
		pGroupFiles->AddLinkItem(ID_TASKITEM_PUBLISH, 4);
		pGroupFiles->AddLinkItem(ID_TASKITEM_SHARE, 5);

	}
	else if (nCount == 1)
	{
		pGroupFiles->AddLinkItem(ID_TASKITEM_RENAME, 10)->SetCaption(_T("Rename this file"));
		pGroupFiles->AddLinkItem(ID_TASKITEM_MOVE, 11)->SetCaption(_T("Move this file"));
		pGroupFiles->AddLinkItem(ID_TASKITEM_COPY, 12)->SetCaption(_T("Copy this file"));
		pGroupFiles->AddLinkItem(ID_TASKITEM_PUBLISH, 4)->SetCaption(_T("Publish this file to the Web"));
		pGroupFiles->AddLinkItem(ID_TASKITEM_EMAIL, 13)->SetCaption(_T("E-mail this file"));
		pGroupFiles->AddLinkItem(ID_TASKITEM_PRINT, 14)->SetCaption(_T("Print this file"));
		pGroupFiles->AddLinkItem(ID_TASKITEM_DELETE, 15)->SetCaption(_T("Delete this file"));
	}
	else
	{
		pGroupFiles->AddLinkItem(ID_TASKITEM_MOVE, 11)->SetCaption(_T("Move the selected items"));
		pGroupFiles->AddLinkItem(ID_TASKITEM_COPY, 12)->SetCaption(_T("Copy the selected items"));
		pGroupFiles->AddLinkItem(ID_TASKITEM_PUBLISH, 4)->SetCaption(_T("Publish tthe selected items to the Web"));
		pGroupFiles->AddLinkItem(ID_TASKITEM_EMAIL, 13)->SetCaption(_T("E-mail the selected items"));
		pGroupFiles->AddLinkItem(ID_TASKITEM_PRINT, 14)->SetCaption(_T("Print the selected items"));
		pGroupFiles->AddLinkItem(ID_TASKITEM_DELETE, 15)->SetCaption(_T("Delete the selected items"));

	}


	CXTPTaskPanelGroup* pGroupDetails = pPanel->FindGroup(ID_TASKGROUP_DETAILS);

	ASSERT(pGroupDetails);
	pGroupDetails->GetItems()->Clear();

	if (nCount == 0)
	{
		CXTPTaskPanelGroupItem* pItemDisk = pGroupDetails->AddTextItem(_T("Local Disk (C:)"));
		pItemDisk->SetBold(TRUE);
		pItemDisk->GetMargins().SetRectEmpty();

		pGroupDetails->AddTextItem(_T("Local Disk"));
		pGroupDetails->AddTextItem(_T("File Sytem: FAT32"));
		pGroupDetails->AddTextItem(_T("Free Space: 23,3 GB"));
		pGroupDetails->AddTextItem(_T("Total Size: 76,2 GB"));
	}
	else if (nCount == 1)
	{
		int nIndex = wndList.GetNextItem(-1, LVNI_SELECTED );

		if (nIndex == 5)
		{
			pGroupDetails->GetItems()->Add(new CTaskPanelIconItem(m_ilIcons.ExtractIcon(2)), 0);

		}

		CString strText = wndList.GetItemText(nIndex, 0);

		CXTPTaskPanelGroupItem* pItemSelected = pGroupDetails->AddTextItem(strText);
		pItemSelected->SetBold(TRUE);
		pItemSelected->GetMargins().SetRectEmpty();

		pGroupDetails->AddTextItem(wndList.GetItemData(nIndex)? _T("File Folder"): _T("File"));
		pGroupDetails->AddTextItem(_T("Date Modified: 2 october, 2003"));
		pGroupDetails->AddTextItem(_T("Size 3,65 KB"));


	}
	else
	{
		CString strSelected;
		strSelected.Format(_T("%i items selected."), nCount);

		pGroupDetails->AddTextItem(strSelected);

	}


	*pResult = 0;
}
