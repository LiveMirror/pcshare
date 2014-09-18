// MyTopView.cpp : implementation file
//

#include "stdafx.h"
#include "PcShare.h"
#include "MyTopView.h"
#include "MyClientTitleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyTopView

IMPLEMENT_DYNCREATE(CMyTopView, CView)

CMyTopView::CMyTopView()
{
}

CMyTopView::~CMyTopView()
{
}

#define IDC_TABCONTROL 100

BEGIN_MESSAGE_MAP(CMyTopView, CView)
	//{{AFX_MSG_MAP(CMyTopView)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_MOUSEACTIVATE()
	ON_COMMAND(ID_TAB_EDIT_ITEM, OnTabEditItem)
	ON_UPDATE_COMMAND_UI(ID_TAB_EDIT_ITEM, OnUpdateTabEditItem)
	ON_UPDATE_COMMAND_UI(ID_TAB_INSERT_ITEM, OnUpdateTabInsertItem)
	ON_COMMAND(ID_TAB_INSERT_ITEM, OnTabInsertItem)
	ON_COMMAND(ID_TAB_DELETE_ITEM, OnTabDeleteItem)
	ON_UPDATE_COMMAND_UI(ID_TAB_DELETE_ITEM, OnUpdateTabDeleteItem)
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABCONTROL, OnSelectedChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyTopView drawing

void CMyTopView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CMyTopView diagnostics

#ifdef _DEBUG
void CMyTopView::AssertValid() const
{
	CView::AssertValid();
}

void CMyTopView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyTopView message handlers
BOOL CMyTopView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	if (!CView::PreCreateWindow(cs))
		return FALSE;

	cs.style &= ~WS_BORDER;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return TRUE;

}

CWnd* CMyTopView::AddView(CRuntimeClass* pViewClass, LPCTSTR lpszTitle, int nIcon)
{
	CCreateContext contextT;
	contextT.m_pCurrentDoc     = NULL;
	contextT.m_pNewViewClass   = pViewClass;
	contextT.m_pNewDocTemplate = NULL;

	CWnd* pWnd;
	TRY
	{
		pWnd = (CWnd*) pViewClass->CreateObject();
		if (pWnd == NULL)
		{
			AfxThrowMemoryException();
		}
	}
	CATCH_ALL(e)
	{
		TRACE0( "Out of memory creating a view.\n" );
		// Note: DELETE_EXCEPTION(e) not required
		return NULL;
	}
	END_CATCH_ALL

	DWORD dwStyle = AFX_WS_DEFAULT_VIEW;
	dwStyle &= ~WS_BORDER;

	int nTab = m_wndTabControl.GetItemCount();

	// Create with the right size (wrong position)
	CRect rect(0, 0, 0, 0);
	if (!pWnd->Create(NULL, NULL, dwStyle, rect, &m_wndTabControl, (AFX_IDW_PANE_FIRST + nTab), &contextT))
	{
		TRACE0( "Warning: couldn't create client tab for view.\n" );
		// pWnd will be cleaned up by PostNcDestroy
		return NULL;
	}

	CXTPTabManagerItem* pItem = m_wndTabControl.InsertItem(nTab, lpszTitle, pWnd->GetSafeHwnd(), nIcon);
	pWnd->SendMessage(WM_INITIALUPDATE);
	pWnd->SetOwner(this);
	return pWnd;
}

int CMyTopView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}
	m_wndTabControl.Create(WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, IDC_TABCONTROL);
	m_wndTabControl.GetPaintManager()->SetAppearance(xtpTabAppearancePropertyPage2003);
	m_wndTabControl.GetPaintManager()->m_bHotTracking = TRUE;
	m_wndTabControl.GetPaintManager()->m_bShowIcons = TRUE;
	m_wndTabControl.GetPaintManager()->DisableLunaColors(FALSE);

	//分组
	TCHAR sViewCount[256] = {0};
	GetPrivateProfileString(PS_SHEZHI, PS_FENZUZONGSHU, _T("0"), sViewCount, 255, GetIniFileName());
	DWORD nViewCount = StrToInt(sViewCount);
	if(nViewCount == 0)
	{
		WritePrivateProfileString(PS_FENZU, _T("0"), PS_PUTONGKEHUZU, GetIniFileName());
		WritePrivateProfileString(PS_SHEZHI, PS_FENZUZONGSHU, _T("1"), GetIniFileName());
		InsertView(PS_PUTONGKEHUZU, FALSE);
		return TRUE;
	}

	for(DWORD i = 0; i < nViewCount; i++)
	{
		TCHAR m_ViewCount[256] = {0};
		wsprintf(m_ViewCount, _T("%d"), i);
		TCHAR m_ViewName[256] = {0};
		GetPrivateProfileString(PS_FENZU, m_ViewCount, m_ViewCount, m_ViewName, 255, GetIniFileName());
		InsertView(m_ViewName, FALSE);
	}
	return 0;
}

BOOL CMyTopView::OnEraseBkgnd(CDC*)
{
	return TRUE;
}

void CMyTopView::OnPaint()
{
	CPaintDC dc(this);
}

void CMyTopView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if(m_wndTabControl.GetSafeHwnd())
	{
		m_wndTabControl.MoveWindow(0, 0, cx, cy);

		CRect rect, rect_tab, rect_view;
		GetClientRect(&rect);
		
		rect_view.top = 24;
		rect_view.left = 1;
		rect_view.bottom = rect.Height() - 1;
		rect_view.right = rect.Width() - 1;
		GetCurView()->MoveWindow(&rect_view);
		GetCurView()->PostMessage(WM_RESIZE_WND, 0, 0);
	}
}

void CMyTopView::OnSelectedChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	if(m_wndTabControl.GetItemCount() <= 0)
	{
		return;
	}
	UNUSED_ALWAYS(pNMHDR);
	*pResult = 0;
	CView* pTmpView = DYNAMIC_DOWNCAST(CView, CWnd::FromHandle(m_wndTabControl.GetSelectedItem()->GetHandle()));
	ASSERT_KINDOF(CView, pTmpView);
	GetParentFrame()->SetActiveView(pTmpView);
}

int CMyTopView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

CMyClientManaView* CMyTopView::GetCurView()
{
	return (CMyClientManaView*) DYNAMIC_DOWNCAST(CView, CWnd::FromHandle(m_wndTabControl.GetSelectedItem()->GetHandle()));
}

CMyClientManaView* CMyTopView::GetViewFromId(int nItem)
{
	return (CMyClientManaView*) DYNAMIC_DOWNCAST(CView, CWnd::FromHandle(m_wndTabControl.GetItem(nItem)->GetHandle()));
}

CMyClientManaView* CMyTopView::GetViewFromName(LPCTSTR sViewName)
{
	for(int i = 0; i < GetViewCount(); i++)
	{
		if(GetViewNameFromId(i) == sViewName)
		{
			return (CMyClientManaView*) DYNAMIC_DOWNCAST(CView, CWnd::FromHandle(m_wndTabControl.GetItem(i)->GetHandle()));
		}
	}
	return NULL;
}

CString CMyTopView::GetCurViewName()
{
	return m_wndTabControl.GetItemCaption(m_wndTabControl.GetSelectedItem());
}

CString CMyTopView::GetViewNameFromId(int nItem)
{
	return m_wndTabControl.GetItemCaption(m_wndTabControl.GetItem(nItem));
}

void CMyTopView::SetCurSelView(int nItem)
{
	m_wndTabControl.SetCurSel(nItem);
}

int CMyTopView::GetViewCount()
{
	return m_wndTabControl.GetItemCount();
}

int CMyTopView::GetCurSel()
{
	return m_wndTabControl.GetCurSel();
}

void CMyTopView::GetPaneInfo(TCHAR* pText)
{
	int nItemCount = 0;
	int nCount = GetViewCount();
	for(int i = 0; i < nCount; i++)
	{
		CMyClientManaView* pView = GetViewFromId(i);
		nItemCount += pView->GetListCtrl().GetItemCount();
	}
	wsprintf(pText, _T("【当前组数量%d个】【总数量%d个】"),	GetCurView()->GetListCtrl().GetItemCount(), nItemCount);
}

MyServerTran* CMyTopView::FindItemFromId(LPCTSTR sId)
{
	int nCount = GetViewCount();
	for(int i = 0; i < nCount; i++)
	{
		CMyClientManaView* pView = GetViewFromId(i);
		if(pView != NULL)
		{
			MyServerTran* pTran = pView->GetItemFromId(sId);
			if(pTran != NULL)
			{
				return pTran;
			}
		}
	}
	return NULL;
}

void CMyTopView::EditView(LPCTSTR pName)
{
	m_wndTabControl.GetItem(m_wndTabControl.GetCurSel())->SetCaption(pName);
}

void CMyTopView::ResizeView()
{
	for(int i = 0; i < GetViewCount(); i++)
	{
		CMyClientManaView* pView = GetViewFromId(i);
		ASSERT(pView);
		CRect rect, rect_tab, rect_view;
		GetClientRect(&rect);
		
		rect_view.top = 24;
		rect_view.left = 1;
		rect_view.bottom = rect.Height() - 1;
		rect_view.right = rect.Width() - 1;
		pView->MoveWindow(&rect_view);
		pView->PostMessage(WM_RESIZE_WND, 0, 0);
	}
}

CMyClientManaView* CMyTopView::InsertView(LPCTSTR pName, BOOL IsAddView)
{
	CMyClientManaView* pView = (CMyClientManaView*) AddView(RUNTIME_CLASS(CMyClientManaView), pName, 10);
	ASSERT(pView);

	CRect rect, rect_tab, rect_view;
	GetClientRect(&rect);
	
	rect_view.top = 24;
	rect_view.left = 1;
	rect_view.bottom = rect.Height() - 1;
	rect_view.right = rect.Width() - 1;
	pView->MoveWindow(&rect_view);
	pView->PostMessage(WM_RESIZE_WND, 0, 0);

	if(IsAddView)
	{
		//取分组总数
		TCHAR m_Index[256] = {0};
		wsprintf(m_Index, _T("%d"), GetViewCount() - 1);

		//添加分组
		WritePrivateProfileString(PS_FENZU, m_Index, pName, GetIniFileName());

		//修改总数
		wsprintf(m_Index, _T("%d"), GetViewCount());
		WritePrivateProfileString(PS_SHEZHI, PS_FENZUZONGSHU, m_Index, GetIniFileName());
	}
	return pView;
}

void CMyTopView::DeleteView()
{
	if(m_wndTabControl.GetCurSel() != 0)
	{
		GetCurView()->DestroyWindow();
		m_wndTabControl.DeleteItem(m_wndTabControl.GetCurSel());
		m_wndTabControl.SetCurSel(0);
	}
}

void CMyTopView::OnTabDeleteItem() 
{
	if(MessageBox(_T("确定真的删该分组？"), PS_CAOZUO, MB_OKCANCEL|MB_ICONQUESTION) != IDOK)
	{
		return;
	}

	//取0目标名称
	TCHAR m_ViewName[256] = {0};
	GetPrivateProfileString(PS_FENZU, _T("0"), PS_PUTONGKEHUZU, m_ViewName, 255, GetIniFileName());

	//移动到目标分组
	int nCount = GetCurView()->GetListCtrl().GetItemCount();
	for(int i = 0; i < nCount; i++)
	{
		MyServerTran* pItem = (MyServerTran*) GetCurView()->GetListCtrl().GetItemData(i);
		pItem->SetViewName(m_ViewName);
		SetUserInfo(pItem->GetId(), pItem->GetViewName(), PS_INFO_VIEW);
		GetViewFromId(0)->InsertItem(pItem, FALSE);
		GetCurView()->GetListCtrl().DeleteItem(i);
	}

	//删除
	int nCur = GetCurSel();
	nCount = GetViewCount();
	
	//移动位置
	for(int k = nCur + 1; k < nCount; k++)
	{
		//取出来
		TCHAR m_ViewCount[256] = {0};
		wsprintf(m_ViewCount, _T("%d"), k);
		GetPrivateProfileString(PS_FENZU, m_ViewCount, m_ViewCount, m_ViewName, 255, GetIniFileName());
		
		TCHAR m_PrevViewCount[256] = {0};
		wsprintf(m_PrevViewCount, _T("%d"), k - 1);
		WritePrivateProfileString(PS_FENZU, m_PrevViewCount, m_ViewName, GetIniFileName());
	}

	//删除最后一个
	TCHAR m_ViewCount[256] = {0};
	wsprintf(m_ViewCount, _T("%d"), nCount - 1);
	WritePrivateProfileString(PS_FENZU, m_ViewCount, NULL, GetIniFileName());
	DeleteView();	

	//更新总数
	nCount = GetViewCount();
	wsprintf(m_ViewCount, _T("%d"), nCount);
	WritePrivateProfileString(PS_SHEZHI, PS_FENZUZONGSHU, m_ViewCount, GetIniFileName());
}

void CMyTopView::OnUpdateTabDeleteItem(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetCurSel() != 0);
}

void CMyTopView::OnTabEditItem() 
{
	TCHAR m_ViewCount[256] = {0};
	int nCount = GetCurSel();
	wsprintf(m_ViewCount, _T("%d"), nCount);
	TCHAR m_ViewName[256] = {0};
	GetPrivateProfileString(PS_FENZU, m_ViewCount, m_ViewCount, m_ViewName, 255, GetIniFileName());
	CMyClientTitleDlg dlg(_T("分组名称"), _T("修改分组"), m_ViewName);
	if(dlg.DoModal() != IDOK)
	{
		return;
	}

	//修改
	wsprintf(m_ViewCount, _T("%d"), nCount);
	WritePrivateProfileString(PS_FENZU, m_ViewCount, dlg.m_Value, GetIniFileName());
	EditView(dlg.m_Value);

	//修改目标分组
	nCount = GetCurView()->GetListCtrl().GetItemCount();
	for(int i = 0; i < nCount; i++)
	{
		MyServerTran* pItem = (MyServerTran*) GetCurView()->GetListCtrl().GetItemData(i);
		pItem->SetViewName(dlg.m_Value);
		SetUserInfo(pItem->GetId(), pItem->GetViewName(), PS_INFO_VIEW);
	}
}

void CMyTopView::OnUpdateTabEditItem(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetViewCount() > 0);
}

void CMyTopView::OnTabInsertItem() 
{
	TCHAR m_ViewCount[256] = {0};
	int nCount = GetViewCount();
	wsprintf(m_ViewCount, _T("%s%d"), PS_PUTONGKEHUZU, nCount);
	CMyClientTitleDlg dlg(_T("分组名称"), _T("新建分组"), m_ViewCount);
	if(dlg.DoModal() != IDOK)
	{
		return;
	}
	
	//查找分组是否存在
	for(int i = 0; i < nCount; i++)
	{
		TCHAR m_ViewCount[256] = {0};
		wsprintf(m_ViewCount, _T("%d"), i);
		TCHAR m_ViewName[256] = {0};
		GetPrivateProfileString(PS_FENZU, m_ViewCount, m_ViewCount, m_ViewName, 255, GetIniFileName());
		if(dlg.m_Value == m_ViewName)
		{
			MessageBox(_T("此分组已经存在"), _T("添加错误"));
			return;
		}
	}

	InsertView(dlg.m_Value, TRUE);
}

void CMyTopView::OnUpdateTabInsertItem(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetViewCount() <= 100);
}

void CMyTopView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if(pWnd->GetSafeHwnd() == m_wndTabControl.GetSafeHwnd())
	{
		CPoint m_point;
		CMenu m_menu;
		m_menu.LoadMenu(IDR_MENU_TAB);
		GetCursorPos(&m_point);
		SetForegroundWindow();   
		CXTPCommandBars::TrackPopupMenu(m_menu.GetSubMenu(0), TPM_RIGHTBUTTON, m_point.x, m_point.y, this);
		m_menu.DestroyMenu();
	}
}

