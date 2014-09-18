// MyFileDownWnd.cpp : implementation file
//

#include "stdafx.h"
#include "PcShare.h"
#include "MyFileDownWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyFileDownWnd

IMPLEMENT_DYNCREATE(CMyFileDownWnd, CFrameWnd)

CMyFileDownWnd::CMyFileDownWnd()
{
	m_IsShow = FALSE;
	pFileDownView = NULL;
	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_FILE_DOWN);
	pLogView = NULL;
}

CMyFileDownWnd::~CMyFileDownWnd()
{
}

BEGIN_MESSAGE_MAP(CMyFileDownWnd, CFrameWnd)
	//{{AFX_MSG_MAP(CMyFileDownWnd)
	ON_WM_CREATE()
	ON_WM_COPYDATA()
	ON_WM_CLOSE()
	ON_COMMAND(ID_EXIT, OnExit)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_INSERTLOGS, OnInsertLogs)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyFileDownWnd message handlers

LRESULT CMyFileDownWnd::OnInsertLogs(WPARAM wParam, LPARAM lParam)
{
	pLogView->InsertItem((LPCTSTR) wParam, (LPCTSTR) lParam);
	return TRUE;
}

BOOL CMyFileDownWnd::StartWork(LPCTSTR sCmdStr)
{
	
	return TRUE;
}

int CMyFileDownWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if(!InitCommandBars())
	{
		return -1;
	}
	
	SetActiveView(pFileDownView);
	CXTPCommandBars* pCommandBars = GetCommandBars();

	CXTPPaintManager* pTheme = new CXTPWhidbeyTheme();
	XTP_COMMANDBARS_ICONSINFO* pIconsInfo = pTheme->GetIconsInfo();
	pIconsInfo->bUseDisabledIcons = TRUE;
	pIconsInfo->bIconsWithShadow = TRUE;
	XTPPaintManager()->SetCustomTheme(pTheme);
	GetCommandBars()->RedrawCommandBars();
	CenterWindow();
	return 0;
}

BOOL CMyFileDownWnd::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	if(pCopyDataStruct->dwData == PS_LOCAL_FILETRAN)
	{
		if(!m_IsShow)
		{
			m_IsShow = TRUE;
			ShowWindow(SW_NORMAL);
		}
		BYTE* pTmpdata = new BYTE[pCopyDataStruct->cbData];
		memcpy(pTmpdata, pCopyDataStruct->lpData, pCopyDataStruct->cbData);
		pFileDownView->PostMessage(WM_ADD_NEW_FILE, (WPARAM) pTmpdata, pCopyDataStruct->cbData);
	}
	if(pCopyDataStruct->dwData == PS_LOCAL_CHECK_FILE_DL)
	{
		pFileDownView->CheckFileDownView((LPCTSTR) pCopyDataStruct->lpData);
	}
	return TRUE;
}

BOOL CMyFileDownWnd::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	if(!m_WndSplitter.CreateStatic(this, 2, 1))
	{
		return FALSE;
	}

	if (!m_WndSplitter.CreateView(0, 0, 
		RUNTIME_CLASS(CMyFileDownView), CSize(0, 0), pContext) ||
		!m_WndSplitter.CreateView(1, 0, 
		RUNTIME_CLASS(CMyFileLogsView), CSize(0, 0), pContext))
	{
		m_WndSplitter.DestroyWindow();
		return FALSE;
	}

	pFileDownView = (CMyFileDownView*) m_WndSplitter.GetPane(0, 0);
	pLogView = (CMyFileLogsView*) m_WndSplitter.GetPane(1, 0);
	return TRUE;
}

void CMyFileDownWnd::OnClose() 
{
	ShowWindow(SW_HIDE);
}

void CMyFileDownWnd::OnExit() 
{
	CFrameWnd::OnClose();
}

void CMyFileDownWnd::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	if(pFileDownView != NULL && cx > 0)
	{
		m_WndSplitter.SetRowInfo(0, cy - 200, 0);
		m_WndSplitter.RecalcLayout();
	}	
}

BOOL CMyFileDownWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	if(!CFrameWnd::PreCreateWindow(cs))
	{
		return FALSE;
	}
	cs.style ^= WS_MAXIMIZE;   
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(
		CS_VREDRAW | CS_HREDRAW,
		::LoadCursor(NULL, IDC_ARROW),
		(HBRUSH) ::GetStockObject(WHITE_BRUSH),
		hIcon);
	return TRUE;
}
