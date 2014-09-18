// MyLogsView.cpp : implementation file
//

#include "stdafx.h"
#include "PcShare.h"
#include "MyLogsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyLogsView

IMPLEMENT_DYNCREATE(CMyLogsView, CListView)

CMyLogsView::CMyLogsView()
{
}

CMyLogsView::~CMyLogsView()
{
}


BEGIN_MESSAGE_MAP(CMyLogsView, CListView)
	//{{AFX_MSG_MAP(CMyLogsView)
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_LOGS_DELETE_ITEM, OnLogsDeleteItem)
	ON_UPDATE_COMMAND_UI(ID_LOGS_DELETE_ITEM, OnUpdateLogsDeleteItem)
	ON_COMMAND(ID_LOGS_EMPTY, OnLogsEmpty)
	ON_UPDATE_COMMAND_UI(ID_LOGS_EMPTY, OnUpdateLogsEmpty)
	ON_COMMAND(ID_LOGS_SAVE, OnLogsSave)
	ON_UPDATE_COMMAND_UI(ID_LOGS_SAVE, OnUpdateLogsSave)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_RESIZE_WND, OnResizeWnd)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyLogsView drawing

void CMyLogsView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CMyLogsView diagnostics

#ifdef _DEBUG
void CMyLogsView::AssertValid() const
{
	CListView::AssertValid();
}

void CMyLogsView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyLogsView message handlers

BOOL CMyLogsView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= LVS_REPORT|LVS_AUTOARRANGE;
	return CXTListView::PreCreateWindow(cs);
}

int CMyLogsView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_imagelist.Create(16, 16,ILC_COLOR8|ILC_MASK,6,100);
	m_imagelist.Add(AfxGetApp()->LoadIcon(IDI_ICON_EVENT_NORMAL));
	GetListCtrl().SetExtendedStyle(LVS_EX_FULLROWSELECT);
	GetListCtrl().SetImageList(&m_imagelist,LVSIL_SMALL);
	GetListCtrl().InsertColumn(0, _T("事件发生时间"),LVCFMT_LEFT,186);
	GetListCtrl().InsertColumn(1, _T("事件内容"), LVCFMT_LEFT, 800);
	GetListCtrl().GetHeaderCtrl()->ModifyStyle(HDS_BUTTONS , HDS_HOTTRACK);
	OnResizeWnd(0, 0);
	return 0;
}

void CMyLogsView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CPoint m_point;
	CMenu m_menu;
	m_menu.LoadMenu(IDR_MENU_LOGS);
	GetCursorPos(&m_point);
	SetForegroundWindow();   
	CXTPCommandBars::TrackPopupMenu(m_menu.GetSubMenu(0), TPM_RIGHTBUTTON, m_point.x, m_point.y, this);
	m_menu.DestroyMenu();
}

void CMyLogsView::OnLogsDeleteItem() 
{
	int nCount = GetListCtrl().GetSelectedCount();
	for(int  i = 0; i < nCount; i++)
	{
		int nItem = GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
		GetListCtrl().DeleteItem(nItem);
	}
	SendMessage(WM_RESIZE_WND,0,0);
}

void CMyLogsView::OnUpdateLogsDeleteItem(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetListCtrl().GetSelectedCount() > 0);
}

void CMyLogsView::OnLogsEmpty() 
{
	GetListCtrl().DeleteAllItems();	
	SendMessage(WM_RESIZE_WND, 0, 0);
}

void CMyLogsView::OnUpdateLogsEmpty(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetListCtrl().GetItemCount() > 0);
}

void CMyLogsView::OnLogsSave() 
{
	OPENFILENAME OpenFileName = {0};
	TCHAR szFile[MAX_PATH] = {0};
  	TCHAR szFilter[1024] = {0};
	lstrcpy(szFilter, _T("文本文件"));
	lstrcpy(szFilter + lstrlen(szFilter) + 1, _T("*.txt"));
	OpenFileName.lStructSize       = sizeof(OPENFILENAME);
    OpenFileName.hwndOwner         = GetSafeHwnd();
	OpenFileName.lpstrFilter       = szFilter;
    OpenFileName.lpstrCustomFilter = NULL;
    OpenFileName.nMaxCustFilter    = 0;
    OpenFileName.nFilterIndex      = 0;
    OpenFileName.lpstrFile         = szFile;
    OpenFileName.nMaxFile          = sizeof(szFile);
    OpenFileName.lpstrFileTitle    = NULL;
    OpenFileName.nMaxFileTitle     = 0;
    OpenFileName.lpstrInitialDir   = NULL;
    OpenFileName.lpstrTitle        = _T("生成日志文件");
    OpenFileName.nFileOffset       = 0;
    OpenFileName.nFileExtension    = 0;
    OpenFileName.lpstrDefExt       = _T("txt");
    OpenFileName.Flags             = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	if(!GetSaveFileName(&OpenFileName))
	{
		return;
	}

	CFile m_File(szFile, CFile::modeCreate|CFile::modeWrite);
	int nCount = GetListCtrl().GetItemCount();
	for(int  i = 0; i < nCount; i++)
	{
		TCHAR m_Text[512] = {0};
		wsprintf(m_Text, _T("发生时间：%s 事件内容：%s\r\n"),
			GetListCtrl().GetItemText(i,0),
			GetListCtrl().GetItemText(i,1));
		m_File.Write(m_Text,lstrlen(m_Text));
	}
	m_File.Close();
}

void CMyLogsView::OnUpdateLogsSave(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetListCtrl().GetItemCount() > 0);
}

LRESULT CMyLogsView::OnResizeWnd(WPARAM wParam, LPARAM lParam)
{
	CRect rect;
	GetClientRect(&rect);
	if(rect.Width() > 0)
	{
		GetListCtrl().SetColumnWidth(0, 186);
		GetListCtrl().SetColumnWidth(1, rect.Width() - 186 - 30);
	}
	return TRUE;
}

void CMyLogsView::InsertItem(LPCTSTR Text)
{
	SYSTEMTIME m_SysTime = {0};
	GetLocalTime(&m_SysTime);
	CString m_str;
	m_str.Format(_T("%d年%d月%d日 %d时%d分%d秒"),
		m_SysTime.wYear, m_SysTime.wMonth, m_SysTime.wDay, 
		m_SysTime.wHour, m_SysTime.wMinute, m_SysTime.wSecond);
	int nCount = GetListCtrl().GetItemCount();
	GetListCtrl().InsertItem(nCount, m_str, 0);
	GetListCtrl().SetItemText(nCount, 1, Text);
	GetListCtrl().EnsureVisible(nCount, FALSE);
}

void CMyLogsView::OnSize(UINT nType, int cx, int cy) 
{
	CListView::OnSize(nType, cx, cy);
	OnResizeWnd(0, 0);	
}
