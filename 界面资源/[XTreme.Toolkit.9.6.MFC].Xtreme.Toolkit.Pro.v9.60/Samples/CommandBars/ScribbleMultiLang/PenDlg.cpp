// PenDlg.cpp : implementation file
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
#include "Scribble.h"
#include "PenDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPenWidthsDlg dialog


CPenWidthsDlg::CPenWidthsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPenWidthsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPenWidthsDlg)
	m_nThinWidth = 0;
	m_nThickWidth = 0;
	//}}AFX_DATA_INIT
}


void CPenWidthsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPenWidthsDlg)
	DDX_Text(pDX, IDC_THIN_PEN_WIDTH, m_nThinWidth);
	DDV_MinMaxInt(pDX, m_nThinWidth, 1, 20);
	DDX_Text(pDX, IDC_THICK_PEN_WIDTH, m_nThickWidth);
	DDV_MinMaxInt(pDX, m_nThickWidth, 1, 20);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPenWidthsDlg, CDialog)
	//{{AFX_MSG_MAP(CPenWidthsDlg)
	ON_BN_CLICKED(IDC_DEFAULT_PEN_WIDTHS, OnDefaultPenWidths)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPenWidthsDlg message handlers

void CPenWidthsDlg::OnDefaultPenWidths()
{
	m_nThinWidth = 2;
	m_nThickWidth = 5;
	UpdateData(FALSE);  // causes DoDataExchange()
		// bSave=FALSE means don't save from screen,
		// rather, write to screen
}

BOOL CPenWidthsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	LCID lcid = GetThreadLocale();

	if ( lcid == MAKELANGID(LANG_JAPANESE, SUBLANG_DEFAULT))
	{
		// set the characterset explicitly
		// -> to run the applicatin in Japanese on a non Japanges Windows
		CFont* pFont = GetFont();
		LOGFONT lf;
		if ( pFont != NULL  )
		{
			pFont->GetLogFont(&lf);
			lf.lfCharSet = SHIFTJIS_CHARSET;
			m_DlgFont.CreateFontIndirect(&lf);
			GetDlgItem(IDC_STATIC_THIN)->SetFont(&m_DlgFont);
			GetDlgItem(IDC_STATIC_THICK)->SetFont(&m_DlgFont);
			GetDlgItem(IDCANCEL)->SetFont(&m_DlgFont);
			GetDlgItem(IDC_DEFAULT_PEN_WIDTHS)->SetFont(&m_DlgFont);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
