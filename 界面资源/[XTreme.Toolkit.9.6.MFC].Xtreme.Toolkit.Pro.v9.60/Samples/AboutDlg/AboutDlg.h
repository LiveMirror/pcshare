// AboutDlg.h : header file
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

#if !defined(__ABOUTDLG_H__)
#define __ABOUTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog
#include "Resource.h"

#if _MSC_VER > 1200 //MFC 7.0
#include <..\src\mfc\afximpl.h> // MFC Global data
#else
#include <..\src\afximpl.h>     // MFC Global data
#endif

#if (_MSC_VER > 1310) // VS2005
#pragma comment(linker, "\"/manifestdependency:type='Win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='X86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

class CAboutDlg : public CDialog
{
// Construction
public:
	CAboutDlg(CWnd* pParent = NULL)
		: CDialog(IDD_ABOUTBOX, pParent)
	{
	}

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	//enum { IDD = IDD_ABOUTBOX };
	CStatic m_txtTitle;
	CStatic m_txtCopyright;
	CStatic m_txtAppName;
#ifdef _XTP_INCLUDE_CONTROLS
	CXTButton m_btnOk;
	CXTHyperLink m_txtURL;
	CXTHyperLink m_txtEmail;
#endif
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:

	virtual void DoDataExchange(CDataExchange* pDX)
	{
		CDialog::DoDataExchange(pDX);
		//{{AFX_DATA_MAP(CAboutDlg)
		DDX_Control(pDX, IDC_TXT_TITLE, m_txtTitle);
		DDX_Control(pDX, IDC_TXT_COPYRIGHT, m_txtCopyright);
		DDX_Control(pDX, IDC_TXT_APPNAME, m_txtAppName);
	#ifdef _XTP_INCLUDE_CONTROLS
		DDX_Control(pDX, IDOK, m_btnOk);
		DDX_Control(pDX, IDC_TXT_URL, m_txtURL);
		DDX_Control(pDX, IDC_TXT_EMAIL, m_txtEmail);
	#endif
		//}}AFX_DATA_MAP
	}

	CString GetVersion()
	{
		CString strVersion;
		strVersion.Format( _T( "Xtreme Toolkit v%d.%02d" ),
			_XTPLIB_VERSION_MAJOR, _XTPLIB_VERSION_MINOR );
		return strVersion;
	}

	virtual BOOL OnInitDialog()
	{
		CDialog::OnInitDialog();

		// get a pointer to CWinApp.
		CWinApp* pApp = AfxGetApp( );
		ASSERT( pApp != NULL );

		// construct the about title.
		CString strAppName = pApp->m_pszAppName;

		// set the strings for app and version.
		m_txtAppName.SetWindowText( strAppName );
		m_txtTitle.SetWindowText( GetVersion( ) );

		// set the about dialog's title.
		CString strWindowText;
		strWindowText.Format( _T("About %s" ), strAppName );
		SetWindowText( strWindowText );

		// construct the copyright text.
		CString strCopyright;
		strCopyright.Format( _T( "©1998-%d Codejock Software, All Rights Reserved" ),
			CTime::GetCurrentTime( ).GetYear( ) );
		m_txtCopyright.SetWindowText( strCopyright );

#ifdef _XTP_INCLUDE_CONTROLS
		// set the title text to bold font.
		m_txtTitle.SetFont( &XTAuxData().fontBold );

		// define the url for our hyperlinks.
		m_txtURL.SetURL( _T( "http://www.codejock.com" ) );
		m_txtURL.SetUnderline( false );

		m_txtEmail.SetURL( _T( "mailto:support@codejock.com" ) );
		m_txtEmail.SetUnderline( false );

		// set OK button style.
		m_btnOk.SetXButtonStyle( BS_XT_SEMIFLAT | BS_XT_HILITEPRESSED | BS_XT_WINXP_COMPAT );
#endif

		return TRUE;  // return TRUE unless you set the focus to a control
		              // EXCEPTION: OCX Property Pages should return FALSE
	}
};

AFX_INLINE void EnableManifestEditor()
{
#if !defined(_UNICODE)
	OSVERSIONINFO ovi = {sizeof(OSVERSIONINFO)};
	::GetVersionEx(&ovi);

	if ((ovi.dwPlatformId >= VER_PLATFORM_WIN32_NT) && (ovi.dwMajorVersion >= 5))
	{
#if (_MSC_VER >= 1200)
		afxData.bWin95 = TRUE;
#else
		afxData.bWin32s = TRUE;
#endif
	}
#endif
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__ABOUTDLG_H__)
