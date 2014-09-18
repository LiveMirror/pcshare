// XTPCustomizeTools.h
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPCUSTOMIZETOOLS_H__)
#define __XTPCUSTOMIZETOOLS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPCommandBar;
class CXTPCommandBars;
class CXTPControl;
class CXTPCustomizeSheet;

//{{AFX_CODEJOCK_PRIVATE

#define DROPEFFECT_CANCEL   ( 5 )

//===========================================================================
// Summary:
//     Internal helper class for customization
//===========================================================================
class _XTP_EXT_CLASS CXTPCustomizeDropSource
{

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPCustomizeDropSource object.
	// Parameters:
	//     pCommandBars - Pointer to a CommandBars object.
	//-------------------------------------------------------------------------
	CXTPCustomizeDropSource(CXTPCommandBars* pCommandBars);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPCustomizeDropSource object, handles cleanup and deallocation.
	//-------------------------------------------------------------------------
	~CXTPCustomizeDropSource();

public:
	DROPEFFECT DoDragDrop(CXTPControl* pControl, BOOL bCopyOnly = FALSE);
	void Register(CXTPCommandBar* pCommandBar);
	void UnRegister(CXTPCommandBar* pCommandBar);
	CXTPCustomizeSheet* GetSheet();

private:

	DROPEFFECT _DoDragDrop();
	void PickTarget(CPoint ptCursor);
	void FreshenCursor();

private:
	HWND m_hwndCapture;
	CXTPControl* m_pControl;
	BOOL m_bMove;
	BOOL m_bCopyOnly;
	CXTPCommandBar* m_pTarget;
	CXTPCustomizeSheet* m_pSheet;
	CPoint m_ptStart;

	CArray<CXTPCommandBar*, CXTPCommandBar*> m_arrTargets;

	CXTPCommandBars* m_pCommandBars;

	HCURSOR m_hcurDelete;
	HCURSOR m_hcurMove;
	HCURSOR m_hcurCopy;

	friend class CXTPCustomizeSheet;
	friend class CXTPCommandBars;

};
//}}AFX_CODEJOCK_PRIVATE

//===========================================================================
// Summary:
//     CXTPNewToolbarDlg is CDialog derived class. It used internally in customization sheet.
//===========================================================================
class _XTP_EXT_CLASS CXTPNewToolbarDlg : public CDialog
{
public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPNewToolbarDlg object.
	// Parameters:
	//     pParent - Points to the objects parent window.
	//     pCommandBars - Pointer to a CXTPCommandBars object.
	//     pCommandBar - Pointer to a CXTPCommandBar need to edit.
	//-------------------------------------------------------------------------
	CXTPNewToolbarDlg(CWnd* pParent, CXTPCommandBars* pCommandBars, CXTPCommandBar* pCommandBar = NULL);

private:
	void SetSuggestedName();

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPNewToolbarDlg)
	virtual void DoDataExchange(CDataExchange* pDX);
	void OnOK();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPNewToolbarDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

public:
//{{AFX_CODEJOCK_PRIVATE
	//{{AFX_DATA(CXTPNewToolbarDlg)
	enum { IDD = XTP_IDD_NEWTOOLBAR };
	UINT m_nNewID;
	CString m_strToolbar;
	//}}AFX_DATA
//}}AFX_CODEJOCK_PRIVATE

	CXTPCommandBar* m_pCommandBar;     // CommandBar to edit
	CXTPCommandBars* m_pCommandBars;   // CommandBars class.

};


#endif // #define __XTPCUSTOMIZETOOLS_H__
