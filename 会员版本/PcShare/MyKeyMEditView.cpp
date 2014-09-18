// MyKeyMEditView.cpp : implementation file
//

#include "stdafx.h"
#include "PcShare.h"
#include "MyKeyMEditView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyKeyMEditView

IMPLEMENT_DYNCREATE(CMyKeyMEditView, CEditView)

CMyKeyMEditView::CMyKeyMEditView()
{
	memset(m_SaveFile, 0, sizeof(TCHAR) * 256);
}

CMyKeyMEditView::~CMyKeyMEditView()
{
}

BEGIN_MESSAGE_MAP(CMyKeyMEditView, CEditView)
	//{{AFX_MSG_MAP(CMyKeyMEditView)
	ON_WM_CREATE()
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_COMMAND(ID_EMPTY, OnEmpty)
	ON_UPDATE_COMMAND_UI(ID_EMPTY, OnUpdateEmpty)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_DATA_ARRIVED, OnDataArrived)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyKeyMEditView drawing

void CMyKeyMEditView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CMyKeyMEditView diagnostics

#ifdef _DEBUG
void CMyKeyMEditView::AssertValid() const
{
	CEditView::AssertValid();
}

void CMyKeyMEditView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyKeyMEditView message handlers


LRESULT CMyKeyMEditView::OnDataArrived(WPARAM wParam, LPARAM lParam)
{
	GetEditCtrl().SetSel(-1, -1);
	GetEditCtrl().ReplaceSel((TCHAR*) lParam);

	// 滚动屏幕
	SendMessage(WM_VSCROLL, MAKELONG(SB_BOTTOM, 0), 0L); 
	return 0;
}

BOOL CMyKeyMEditView::PreCreateWindow(CREATESTRUCT& cs) 
{
	BOOL bPreCreated = CEditView::PreCreateWindow(cs);
	cs.style &= ~WS_BORDER;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.dwExStyle ^= WS_EX_STATICEDGE;
	return bPreCreated;
}

int CMyKeyMEditView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEditView::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

void CMyKeyMEditView::OnChange() 
{
		
}

void CMyKeyMEditView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{

}

void CMyKeyMEditView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	
}

void CMyKeyMEditView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	
}

void CMyKeyMEditView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CPoint m_point;
	CMenu m_menu;
	m_menu.LoadMenu(IDR_KEYM_RIGHT);
	GetCursorPos(&m_point);
	SetForegroundWindow();   
	CXTPCommandBars::TrackPopupMenu(m_menu.GetSubMenu(0), TPM_RIGHTBUTTON, m_point.x, m_point.y, this);
	m_menu.DestroyMenu();
}

void CMyKeyMEditView::OnFileSave() 
{
	CString m_Title;
	GetWindowText(m_Title);

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
    OpenFileName.lpstrTitle        = _T("保存文件");
    OpenFileName.nFileOffset       = 0;
    OpenFileName.nFileExtension    = 0;
    OpenFileName.lpstrDefExt       = _T("txt");
    OpenFileName.Flags             = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	if(!GetSaveFileName(&OpenFileName))
	{
		return;
	}
	CFile m_File(szFile, CFile::modeCreate|CFile::modeWrite);
	m_File.Write(m_Title.LockBuffer(), m_Title.GetLength() * sizeof(TCHAR));
	m_File.Close();	
}

void CMyKeyMEditView::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	CString m_Title;
	GetWindowText(m_Title);
	pCmdUI->Enable(m_Title.GetLength() > 0);
}

void CMyKeyMEditView::OnEmpty() 
{
	SetWindowText(_T(""));
	DeleteFile(m_SaveFile);
}

void CMyKeyMEditView::OnUpdateEmpty(CCmdUI* pCmdUI) 
{
	
}
