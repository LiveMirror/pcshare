// MyLink.cpp : implementation file
//

#include "stdafx.h"
#include "PcShare.h"
#include "MyLinkCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDC_HAND MAKEINTRESOURCE(32649)

/////////////////////////////////////////////////////////////////////////////
// CMyLinkCtrl

CMyLinkCtrl::CMyLinkCtrl()
{
	m_LogoCur = LoadCursor(0, IDC_HAND);
}

CMyLinkCtrl::~CMyLinkCtrl()
{

}


BEGIN_MESSAGE_MAP(CMyLinkCtrl, CStatic)
	//{{AFX_MSG_MAP(CMyLinkCtrl)
	ON_WM_PAINT()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyLinkCtrl message handlers

void CMyLinkCtrl::OnPaint() 
{
	CPaintDC dc(this); 

	CFont *pFont = NULL;
	CString m_Text;
	GetWindowText(m_Text);
	dc.SetBkMode(TRANSPARENT);
	if(m_Font.GetSafeHandle())
	{
		pFont = &m_Font;
		CFont *pOldFont = dc.SelectObject(pFont);
		dc.SetTextColor(RGB(20,10,200));
		dc.TextOut(0,0,m_Text);
		dc.SelectObject(pOldFont);
	}
}

void CMyLinkCtrl::SetMyFont(LPCTSTR FontName,int FontSize,int FontWidth,BOOL IsLine)
{
	if(m_Font.GetSafeHandle()) m_Font.DeleteObject();
	{
		m_Font.CreateFont(FontSize,0,0,0,FontWidth,0,IsLine,0,0,0,0,0,0,FontName);
	}
}

CSize CMyLinkCtrl::GetMySize(LPCTSTR m_Text)
{
	CDC *pDC = GetDesktopWindow()->GetDC();
	CFont *pOldFont = pDC->SelectObject(&m_Font);
	CSize m_LinkSize = pDC->GetOutputTextExtent(m_Text);
	pDC->SelectObject(pOldFont);
	GetDesktopWindow()->ReleaseDC(pDC);
	return m_LinkSize;
}

BOOL CMyLinkCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	POINT m_point;
	GetCursorPos(&m_point);
	CRect rect;
	GetWindowRect(&rect);
	if(rect.PtInRect(m_point))
	{
		::SetCursor(m_LogoCur);
		return FALSE;
	}
	return CStatic::OnSetCursor(pWnd, nHitTest, message);
}