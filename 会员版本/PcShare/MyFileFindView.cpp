// MyFileFindView.cpp : implementation file
//

#include "stdafx.h"
#include "PcShare.h"
#include "MyFileFindView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyFileFindView

IMPLEMENT_DYNCREATE(CMyFileFindView, CFormView)

CMyFileFindView::CMyFileFindView()
	: CFormView(CMyFileFindView::IDD)
{
	//{{AFX_DATA_INIT(CMyFileFindView)
	m_Dir = _T("");
	m_File = _T("");
	//}}AFX_DATA_INIT
}

CMyFileFindView::~CMyFileFindView()
{
}

void CMyFileFindView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyFileFindView)
	DDX_Text(pDX, IDC_EDIT_DIR, m_Dir);
	DDX_Text(pDX, IDC_EDIT_FILE, m_File);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyFileFindView, CFormView)
	//{{AFX_MSG_MAP(CMyFileFindView)
	ON_BN_CLICKED(ID_FILE_FIND, OnFileFind)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyFileFindView diagnostics

#ifdef _DEBUG
void CMyFileFindView::AssertValid() const
{
	CFormView::AssertValid();
}

void CMyFileFindView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyFileFindView message handlers

void CMyFileFindView::OnFileFind() 
{
	UpdateData();
	if(m_Dir.IsEmpty())
	{
		return;
	}
	if(m_File.IsEmpty())
	{
		m_File = _T("*.*");
	}
	TCHAR m_FindFile[1024] = {0};
	wsprintf(m_FindFile, _T("%s\\%s"), m_Dir, m_File);
	GetParentFrame()->SendMessage(WM_FINDFILE, (WPARAM) m_FindFile, 0);
}

void CMyFileFindView::SetPath(LPCTSTR sPath)
{
	m_Dir = sPath;
	UpdateData(FALSE);
}

void CMyFileFindView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);

	

	TRACE(_T("cx = %d, cy = %d\n"), cx, cy);// "*" to _T("*") [9/15/2007 zhaiyinwei]
}
