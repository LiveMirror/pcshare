// MyFileDownView.cpp : implementation file
//

#include "stdafx.h"
#include "PcShare.h"
#include "MyFileDownView.h"
#include "MyFileDownList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyFileDownView

#define IDC_TABCONTROL1 105

IMPLEMENT_DYNCREATE(CMyFileDownView, CView)

CMyFileDownView::CMyFileDownView()
{
}

CMyFileDownView::~CMyFileDownView()
{
}


BEGIN_MESSAGE_MAP(CMyFileDownView, CView)
	//{{AFX_MSG_MAP(CMyFileDownView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABCONTROL1, OnSelectedChanged)
	ON_MESSAGE(WM_ADD_NEW_FILE, OnAddNewFile)
	ON_MESSAGE(WM_DL_DELETE_VIEW, OnDlDeleteView)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyFileDownView drawing

void CMyFileDownView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CMyFileDownView diagnostics

#ifdef _DEBUG
void CMyFileDownView::AssertValid() const
{
	CView::AssertValid();
}

void CMyFileDownView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyFileDownView message handlers

CWnd* CMyFileDownView::AddView(CRuntimeClass* pViewClass, LPCTSTR lpszTitle, int nIcon)
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
	CRect rect(0,0,0,0);
	if (!pWnd->Create(NULL, NULL, dwStyle, rect, &m_wndTabControl, (AFX_IDW_PANE_FIRST + nTab), &contextT))
	{
		TRACE0( "Warning: couldn't create client tab for view.\n" );
		// pWnd will be cleaned up by PostNcDestroy
		return NULL;
	}

	CXTPTabManagerItem* pItem = m_wndTabControl.InsertItem(nTab, lpszTitle, pWnd->GetSafeHwnd(), nIcon);
	pWnd->SendMessage(WM_INITIALUPDATE);
	pWnd->SetOwner(this);
	pWnd->SetParent(this);
	return pWnd;
}

BOOL CMyFileDownView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style &= ~WS_BORDER;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	return CView::PreCreateWindow(cs);
}

int CMyFileDownView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if(!m_wndTabControl.Create(WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, IDC_TABCONTROL1))
	{
		return -1;
	}

	m_wndTabControl.GetPaintManager()->SetAppearance(xtpTabAppearancePropertyPage2003);
	m_wndTabControl.GetPaintManager()->m_bHotTracking = TRUE;
	m_wndTabControl.GetPaintManager()->m_bShowIcons = TRUE;
	m_wndTabControl.GetPaintManager()->DisableLunaColors(FALSE);

	//查看文件缓冲
	TCHAR m_DirFindName[256] = {0};
	GetModuleFileName(NULL, m_DirFindName, 255);
	TCHAR* pFind = StrRChr(m_DirFindName, NULL, _T('\\'));
	if(pFind == NULL)
	{
		return 0;
	}

	*(pFind + 1) = 0x00;
	lstrcat(m_DirFindName, _T("CACHE\\FILEINFO\\*.*"));
	MakeFilePath(m_DirFindName);

	CFileFind m_Find;
	if(!m_Find.FindFile(m_DirFindName))
	{
		return 0;
	}

	while(1)
	{
		BOOL Ret = m_Find.FindNextFile();
		if(!m_Find.IsDirectory())
		{
			//打开文件
			HANDLE hFileSaveList = CreateFile(m_Find.GetFilePath(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if(hFileSaveList == INVALID_HANDLE_VALUE)
			{
				DeleteFile(m_Find.GetFilePath());
				continue;
			}

			//TITLE
			TCHAR m_Title[256] = {0};
			DWORD nRecvLen = 0;
			ReadFile(hFileSaveList, m_Title, 256 * sizeof(TCHAR), &nRecvLen, NULL);
			CloseHandle(hFileSaveList);

			CMyFileDownList* pFileView = (CMyFileDownList*) InsertView(m_Title);
			ASSERT(pFileView);

			//ID
			TCHAR m_Id[256] = {0};
			lstrcpy(m_Id, m_Find.GetFileTitle());

			//初始化VIEW
			pFileView->InitData(m_Id, m_Title, m_Find.GetFilePath());
		}
		if(!Ret)
		{
			break;
		}
	}

	return 0;
}

void CMyFileDownView::CheckFileDownView(LPCTSTR pId)
{
	int nCount = GetViewCount();
	for(int i = 0; i < nCount; i++)
	{
		CMyFileDownList* pView = (CMyFileDownList*) GetViewFromId(i);
		if(StrCmp(pView->GetId(), pId) == 0)
		{
			pView->PostMessage(WM_COMMAND, (WPARAM) ID_FILE_TRANS_START, 0);
			break;
		}
	}
}

CWnd* CMyFileDownView::InsertView(LPCTSTR pName)
{
	CWnd* pView = AddView(RUNTIME_CLASS(CMyFileDownList), pName, 10);
	ASSERT(pView);
	pView->PostMessage(WM_RESIZE_WND, 0, 0);
	if(m_wndTabControl.GetItemCount() == 1)
	{
		GetParentFrame()->SetActiveView((CView*) pView);
	}
	return pView;
}

void CMyFileDownView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	if(m_wndTabControl.GetSafeHwnd())
	{
		m_wndTabControl.MoveWindow(0, 0, cx, cy);
		if(GetViewCount() > 0)
		{
			CRect rect, rect_tab, rect_view;
			GetClientRect(&rect);
			
			rect_view.top = 24;
			rect_view.left = 1;
			rect_view.bottom = rect.Height() - 1;
			rect_view.right = rect.Width() - 1;
			GetCurView()->MoveWindow(&rect_view);
		}
	}	
}

void CMyFileDownView::OnSelectedChanged(NMHDR* pNMHDR, LRESULT* pResult)
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

CWnd* CMyFileDownView::GetCurView()
{
	return (CWnd*) DYNAMIC_DOWNCAST(CView, CWnd::FromHandle(m_wndTabControl.GetSelectedItem()->GetHandle()));
}

CString CMyFileDownView::GetViewNameFromId(int nItem)
{
	return m_wndTabControl.GetItemCaption(m_wndTabControl.GetItem(nItem));
}

int CMyFileDownView::GetViewCount()
{
	return m_wndTabControl.GetItemCount();
}

CWnd* CMyFileDownView::GetViewFromName(LPCTSTR sViewName)
{
	for(int i = 0; i < GetViewCount(); i++)
	{
		if(GetViewNameFromId(i) == sViewName)
		{
			return (CWnd*) DYNAMIC_DOWNCAST(CView, CWnd::FromHandle(m_wndTabControl.GetItem(i)->GetHandle()));
		}
	}
	return NULL;
}

LRESULT CMyFileDownView::OnDlDeleteView(WPARAM wParam, LPARAM lParam)
{
	CMyFileDownList* pView = (CMyFileDownList*) wParam;

	for(int i = 0; i < GetViewCount(); i++)
	{
		CWnd* pWnd = DYNAMIC_DOWNCAST(CView, CWnd::FromHandle(m_wndTabControl.GetItem(i)->GetHandle()));
		if(pWnd->GetSafeHwnd() == pView->GetSafeHwnd())
		{
			m_wndTabControl.DeleteItem(i);
			pView->DestroyWindow();
		}
	}
	return TRUE;
}

LRESULT CMyFileDownView::OnAddNewFile(WPARAM wParam, LPARAM lParam)
{
	LPMYINSERTDLFILEINFO pDlFileInfo = (LPMYINSERTDLFILEINFO)(((BYTE*) wParam) + lParam - sizeof(MYINSERTDLFILEINFO));

	//查找当前视是否存在
	CMyFileDownList* pFileView = NULL;
	int nCount = GetViewCount();
	for(int i = 0; i < nCount; i++)
	{
		CMyFileDownList* pView = (CMyFileDownList*) GetViewFromId(i);
		if(StrCmp(pView->GetId(), pDlFileInfo->m_Id) == 0)
		{
			pFileView = pView;
			break;
		}
	}

	//创建视
	if(pFileView == NULL)
	{
		pFileView = (CMyFileDownList*) InsertView(pDlFileInfo->m_Title);
		ASSERT(pFileView);
		TCHAR m_FilePath[256] = {0};
		wsprintf(m_FilePath, _T("%s.dat"), pDlFileInfo->m_Id);
		GetMySaveFile(m_FilePath);
		pFileView->InitData(pDlFileInfo->m_Id, pDlFileInfo->m_Title, m_FilePath);
	}

	pFileView->AddFileDownList(pDlFileInfo, (BYTE*) wParam, lParam);

	delete [] (BYTE*) wParam;
	return TRUE;
}

CWnd* CMyFileDownView::GetViewFromId(int nItem)
{
	return (CWnd*) DYNAMIC_DOWNCAST(CView, CWnd::FromHandle(m_wndTabControl.GetItem(nItem)->GetHandle()));
}

BOOL CMyFileDownView::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CMyFileDownView::OnPaint() 
{
	CPaintDC dc(this); 
}
