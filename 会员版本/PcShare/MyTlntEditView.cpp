// MyTlntEditView.cpp : implementation file
//

#include "stdafx.h"
#include "PcShare.h"
#include "MyTlntEditView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyTlntEditView

IMPLEMENT_DYNCREATE(CMyTlntEditView, CEditView)

CMyTlntEditView::CMyTlntEditView()
{
	nCount = 0;
	m_IsExit = FALSE;
	m_InputEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	// 初始化两款字体，若不设字体则韩文显示不正常 [9/16/2007 zhaiyinwei]
	m_bSetFont = FALSE;
	VERIFY(m_ChineseFont.CreateFont(
	   14,                        // nHeight
	   0,                         // nWidth
	   0,                         // nEscapement
	   0,                         // nOrientation
	   FW_NORMAL,                 // nWeight
	   FALSE,                     // bItalic
	   FALSE,                     // bUnderline
	   0,                         // cStrikeOut
	   ANSI_CHARSET,              // nCharSet
	   OUT_DEFAULT_PRECIS,        // nOutPrecision
	   CLIP_DEFAULT_PRECIS,       // nClipPrecision
	   DEFAULT_QUALITY,           // nQuality
	   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
	   _T("宋体")));				  // 宋体 lpszFacename

	//专为俄语找的一款字体
	VERIFY(m_RussianFont.CreateFont(
	   14,                        // nHeight
	   0,                         // nWidth
	   0,                         // nEscapement
	   0,                         // nOrientation
	   FW_NORMAL,                 // nWeight
	   FALSE,                     // bItalic
	   FALSE,                     // bUnderline
	   0,                         // cStrikeOut
	   ANSI_CHARSET,              // nCharSet
	   OUT_DEFAULT_PRECIS,        // nOutPrecision
	   CLIP_DEFAULT_PRECIS,       // nClipPrecision
	   DEFAULT_QUALITY,           // nQuality
	   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
	   _T("Courier New")));                 // 宋体 lpszFacename
}

CMyTlntEditView::~CMyTlntEditView()
{
	CloseHandle(m_InputEvent);
}

BEGIN_MESSAGE_MAP(CMyTlntEditView, CEditView)
	//{{AFX_MSG_MAP(CMyTlntEditView)
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	ON_WM_CHAR()
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_DATA_ARRIVED, OnDataArrived)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyTlntEditView drawing

BOOL CMyTlntEditView::WaitInput(CString& m_str)
{
	WaitForSingleObject(m_InputEvent, INFINITE);
	if(!m_IsExit)
	{
		CString m_StrWnd;
		GetWindowText(m_StrWnd);
		int nPos = m_StrWnd.ReverseFind(_T('>'));
		nPos ++;
		m_strSendBuf = m_StrWnd.Right(m_StrWnd.GetLength() - nPos);
		if(m_strSendBuf.Right(2) != _T("\r\n"))
		{
			m_strSendBuf += _T("\r\n");
		}
		m_str = m_strSendBuf;
		return TRUE;
	}
	return FALSE;
}

void CMyTlntEditView::CloseInput()
{
	m_IsExit = TRUE;
	SetEvent(m_InputEvent);
}

void CMyTlntEditView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CMyTlntEditView diagnostics

#ifdef _DEBUG
void CMyTlntEditView::AssertValid() const
{
	CEditView::AssertValid();
}

void CMyTlntEditView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyTlntEditView message handlers
LRESULT CMyTlntEditView::OnDataArrived(WPARAM wParam, LPARAM lParam)
{
	CString m_StrRecv = (TCHAR*) lParam;
	if(m_strSendBuf.GetLength() > 0)
	{
		if(m_strSendBuf.CompareNoCase(_T("cls\r\n")) == 0)
		{
			GetEditCtrl().SetWindowText(_T(""));
			m_StrRecv = m_StrRecv.Right(m_StrRecv.GetLength() - 6);
		}
		else
		{
			m_StrRecv.Replace(m_strSendBuf, _T(""));
		}
		m_strSendBuf.Empty();
	}

	GetEditCtrl().SetSel(-1, -1);
	GetEditCtrl().ReplaceSel(m_StrRecv);
	SendMessage(WM_VSCROLL, MAKELONG(SB_BOTTOM, 0), 0L);

	CString strCmdText;
	GetEditCtrl().GetWindowText(strCmdText);
	nCount = strCmdText.GetLength();

	// 设置字体 [9/16/2007 zhaiyinwei]
	if (!m_bSetFont)
	{
		SetFont(&m_ChineseFont) ;
		m_bSetFont = TRUE;
	}
	return TRUE;
}

void CMyTlntEditView::OnChange() 
{
		
}

void CMyTlntEditView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(!bIsSafeChar(nChar))
	{
		return;
	}

	if(nChar == VK_RETURN)
	{
		CEditView::OnChar(nChar, nRepCnt, nFlags);
		SetEvent(m_InputEvent);
		return;
	}
	CEditView::OnChar(nChar, nRepCnt, nFlags);
}

BOOL CMyTlntEditView::PreCreateWindow(CREATESTRUCT& cs) 
{
	BOOL bPreCreated = CEditView::PreCreateWindow(cs);
	cs.style &= ~WS_BORDER;
//	cs.style &= ~WS_HSCROLL;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.dwExStyle ^= WS_EX_STATICEDGE;
	return bPreCreated;
}

int CMyTlntEditView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEditView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

BOOL CMyTlntEditView::bIsSafeChar(UINT nChar)
{
	if(nCount == 0)
	{
		return FALSE;
	}

	CString strCmdText;
	GetEditCtrl().GetWindowText(strCmdText);
	int nStart = 0, nEnd = 0;
	GetEditCtrl().GetSel(nStart, nEnd);
	if(nStart != nEnd)
	{
		return FALSE;
	}
	if(nStart < nCount)
	{
		return FALSE;
	}
	if(nChar == 8)
	{
		if(nStart <= nCount)
		{
			return FALSE;
		}
	}
	return TRUE;
}

void CMyTlntEditView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar == 38)
	{
		//向上
		return;
	}

	if(nChar == 40)
	{
		//向下
		return;
	}

	if(nChar == 46)
	{
		if(!bIsSafeChar(nChar))
		{
			return;
		}
	}
	CEditView::OnKeyDown(nChar, nRepCnt, nFlags);
}

// 注意：不会进入OnInitialUpdate！ [9/16/2007 zhaiyinwei]
void CMyTlntEditView::OnInitialUpdate() 
{
	CEditView::OnInitialUpdate();

}

void CMyTlntEditView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CPoint m_point;
	CMenu m_menu;
	m_menu.LoadMenu(IDR_MENU_EDIT);
	GetCursorPos(&m_point);
	SetForegroundWindow();   
	CXTPCommandBars::TrackPopupMenu(m_menu.GetSubMenu(0), TPM_RIGHTBUTTON, m_point.x, m_point.y, this);
	m_menu.DestroyMenu();	
}

void CMyTlntEditView::OnFileSave() 
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
	// Unicode下需要加入文件头 [9/16/2007 zhaiyinwei]
#ifdef UNICODE
	CString strUtf16Header = _T("\xFEFF") ;
	m_File.Write((LPCTSTR)strUtf16Header, strUtf16Header.GetLength()*sizeof(TCHAR)) ; 	
#endif
	m_File.Write(m_Title.LockBuffer(), m_Title.GetLength() * sizeof(TCHAR));
	m_Title.UnlockBuffer();// UnLockBuf [9/16/2007 zhaiyinwei]
	m_File.Close();	
}

void CMyTlntEditView::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	CString m_Title;
	GetWindowText(m_Title);
	pCmdUI->Enable(m_Title.GetLength() > 0);
}
