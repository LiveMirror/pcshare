// CommandBarsDesignerDoc.cpp : implementation of the CCommandBarsDesignerDoc class
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
#include "CommandBarsDesigner.h"

#include "CommandBarsDesignerDoc.h"
#include "CommandBarsDesignerView.h"

#include "EmbeddedFrame.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const UINT WM_FILEDATA_RENDER = RegisterWindowMessage(_T("XCB File Data Render"));
const UINT m_cfFormat = RegisterClipboardFormat(_T("XCB File Data"));


/////////////////////////////////////////////////////////////////////////////
// CCommandBarsDesignerDoc

IMPLEMENT_DYNCREATE(CCommandBarsDesignerDoc, CDocument)

BEGIN_MESSAGE_MAP(CCommandBarsDesignerDoc, CDocument)
	//{{AFX_MSG_MAP(CCommandBarsDesignerDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CCommandBarsDesignerDoc, CDocument)
	//{{AFX_DISPATCH_MAP(CCommandBarsDesignerDoc)
	DISP_FUNCTION(CCommandBarsDesignerDoc, "OnEdit", OnEdit, VT_BOOL, VTS_HANDLE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_ICommandBarsDesigner to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the
//  dispinterface in the .ODL file.

// {157FA069-D75C-4E71-A537-7C86F65FB7E9}
static const IID IID_ICommandBarsDesigner =
{ 0x157fa069, 0xd75c, 0x4e71, { 0xa5, 0x37, 0x7c, 0x86, 0xf6, 0x5f, 0xb7, 0xe9 } };

BEGIN_INTERFACE_MAP(CCommandBarsDesignerDoc, CDocument)
	INTERFACE_PART(CCommandBarsDesignerDoc, IID_ICommandBarsDesigner, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommandBarsDesignerDoc construction/destruction

CCommandBarsDesignerDoc::CCommandBarsDesignerDoc()
{
	m_hwndEdit = 0;

	EnableAutomation();

	AfxOleLockApp();
}

CCommandBarsDesignerDoc::~CCommandBarsDesignerDoc()
{
	AfxOleUnlockApp();
}

BOOL CCommandBarsDesignerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CCommandBarsDesignerDoc serialization

void CCommandBarsDesignerDoc::Serialize(CArchive& ar)
{

	POSITION pos = GetFirstViewPosition();
	if (pos)
	{
		CCommandBarsDesignerView* pView = (CCommandBarsDesignerView*)GetNextView(pos);
		ASSERT(pView);
		pView->Serialize(ar);
	}

	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCommandBarsDesignerDoc diagnostics

#ifdef _DEBUG
void CCommandBarsDesignerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCommandBarsDesignerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCommandBarsDesignerDoc commands

BOOL CCommandBarsDesignerDoc::OnEdit(HWND hWnd)
{
	ASSERT(m_bEmbedded);

	m_hwndEdit = hWnd;

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->ShowWindow(SW_SHOW);
	pMainFrame->UpdateWindow();

	POSITION pos = GetFirstViewPosition();
	CCommandBarsDesignerView* pView = (CCommandBarsDesignerView*)GetNextView(pos);
	pView->GetParent()->ShowWindow(SW_SHOWMAXIMIZED);

	if (!OpenClipboard(hWnd))
		return FALSE;

	if (::IsClipboardFormatAvailable(m_cfFormat))
	{
		HGLOBAL hGlobal = GetClipboardData (m_cfFormat);
		LPVOID pData = GlobalLock(hGlobal);

		CMemFile file((BYTE*)pData + sizeof(DWORD), *(DWORD*)pData);
		CArchive ar(&file, CArchive::load | CArchive::bNoFlushOnDelete);
		Serialize(ar);

		GlobalUnlock(hGlobal);
	}
	else
	{
		pView->GetEmbeddedFrame()->Clear();
	}
	::CloseClipboard();

	pView->OnInitialUpdate();
	pMainFrame->RefreshPanes(TRUE);


	return TRUE;
}


BOOL CCommandBarsDesignerDoc::DoFileSave()
{
	if (m_bEmbedded)
	{
		ASSERT(m_hwndEdit);

		m_bModified = FALSE;

		CMemFile file;
		CArchive ar(&file, CArchive::store);

		Serialize(ar);

		ar.Flush();
		DWORD dwSize = (DWORD)file.GetPosition();
		BYTE* pFileData = file.Detach();

		HGLOBAL hDesignerData = GlobalAlloc(GMEM_FIXED, dwSize + sizeof(DWORD));

		LPVOID pData =  GlobalLock(hDesignerData);
		*((DWORD*)pData) = dwSize;

		memcpy((BYTE*)pData + sizeof(DWORD), pFileData, dwSize);
		GlobalUnlock(hDesignerData);

		ar.Abort();
		file.Close();

		if (!::OpenClipboard (0))
			return TRUE;

		if (!::EmptyClipboard ())
		{
			::CloseClipboard ();
			return TRUE;
		}

		UINT uFormat = RegisterClipboardFormat(_T("XCB File Data"));
		SetClipboardData(uFormat, hDesignerData);

		::CloseClipboard();

		SendMessage(m_hwndEdit, WM_FILEDATA_RENDER, 0, 0);

		if (AfxGetMainWnd() && AfxGetMainWnd()->GetSafeHwnd())
		{
			AfxGetMainWnd()->PostMessage(WM_CLOSE);
		}
		return TRUE;
	}

	return CDocument::DoFileSave();

}
