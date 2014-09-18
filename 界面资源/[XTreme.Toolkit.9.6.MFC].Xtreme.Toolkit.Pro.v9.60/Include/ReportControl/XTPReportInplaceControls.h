// XTPReportInplaceControls.h
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
#if !defined(__XTPREPORTINPLACECONTROLS_H__)
#define __XTPREPORTINPLACECONTROLS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPReportRecord;
class CXTPReportControl;
class CXTPReportRecordItem;
class CXTPReportRecordItemConstraints;
class CXTPReportRecordItemConstraint;

#include "XTPReportRecordItem.h"


//===========================================================================
// Summary:
//     CXTPReportInplaceControl is the base class for all in-place controls of
//     report control item.
// Remarks:
//     There are 3 built-in in-place controls for Report control item:
//     * CXTPReportInplaceEdit - represents edit control to allow users change cell
//       of report control
//     * CXTPReportInplaceButton - represents single button of item.
//     * CXTPReportInplaceList - represents in-place list box to allow user select
//       constraints of report item
// See Also: CXTPReportInplaceEdit, CXTPReportInplaceButton, CXTPReportInplaceList
//===========================================================================
class _XTP_EXT_CLASS CXTPReportInplaceControl : public XTP_REPORTRECORDITEM_ARGS
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridInplaceList object
	//-----------------------------------------------------------------------
	CXTPReportInplaceControl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyGridInplaceList object, handles cleanup
	//     and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPReportInplaceControl();

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called internal to fill item arguments of in-place control
	// Parameters:
	//     pItemArgs - Arguments of item.
	//-------------------------------------------------------------------------
	void SetItemArgs(XTP_REPORTRECORDITEM_ARGS* pItemArgs);

};

//===========================================================================
// Summary:
//     CXTPReportInplaceEdit is the CXTPReportInplaceControl derived  class,
//     it represents edit control to allow users change cell of report control.
//===========================================================================
class _XTP_EXT_CLASS CXTPReportInplaceEdit : public CEdit, public CXTPReportInplaceControl
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPReportInplaceEdit object
	//-----------------------------------------------------------------------
	CXTPReportInplaceEdit();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPReportInplaceEdit object, handles cleanup
	//     and deallocation.
	//-----------------------------------------------------------------------
	~CXTPReportInplaceEdit();

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to hide in-place edit control
	//-------------------------------------------------------------------------
	void HideWindow();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves a child report control item
	// Returns:
	//     A pointer to a CXTPReportRecordItem
	//-----------------------------------------------------------------------
	CXTPReportRecordItem* GetItem();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to create in-place edit control.
	// Parameters:
	//     pItemArgs - Parameters of item cell.
	//-----------------------------------------------------------------------
	void Create(XTP_REPORTRECORDITEM_ARGS* pItemArgs);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to change font of in-place edit control
	// Parameters:
	//     pFont - New font to be set.
	//-----------------------------------------------------------------------
	void SetFont(CFont* pFont);


protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPPropertyGridInplaceEdit)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPPropertyGridInplaceEdit)
	afx_msg void OnEnKillfocus();
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT /*nCtlColor*/);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

protected:
	CString m_strValue;                                     // The value off item before user change it,
	CFont m_fntEdit;                                        // Font of in-place edit.
	CXTPReportRecordItemConstraint* m_pSelectedConstraint;  // Selected constraint.
	COLORREF m_clrText;                                     // Text of in-place edit.

	friend class CXTPReportRecordItem;
};

AFX_INLINE CXTPReportRecordItem* CXTPReportInplaceEdit::GetItem() {
	return pItem;
}
AFX_INLINE BOOL CXTPReportInplaceEdit::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
AFX_INLINE BOOL CXTPReportInplaceEdit::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) {
	return CEdit::Create(dwStyle, rect, pParentWnd, nID);
}



const UINT XTP_ID_REPORT_EXPANDBUTTON = 100; //<ALIAS CXTPReportInplaceButton::CXTPReportInplaceButton@UINT>
const UINT XTP_ID_REPORT_COMBOBUTTON = 101; //<ALIAS CXTPReportInplaceButton::CXTPReportInplaceButton@UINT>


//===========================================================================
// Summary:
//     CXTPReportInplaceButton is the CXTPReportInplaceControl derived  class,
//     it represents combo button and expand button of report cell.
//===========================================================================
class _XTP_EXT_CLASS CXTPReportInplaceButton : public CStatic, public CXTPReportInplaceControl
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPReportInplaceButton object
	// Parameters:
	//     nID - Identifier of in-place button. Can be any of the values listed in the Remarks section.
	// Remarks:
	//     Default identifiers are:
	//     * <b>XTP_ID_REPORT_EXPANDBUTTON</b> Indicates the button acts like an expand button
	//     * <b>XTP_ID_REPORT_COMBOBUTTON</b>  Indicates the button acts like a combo button.
	//-----------------------------------------------------------------------
	CXTPReportInplaceButton(UINT nID);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to create in-place button control.
	// Parameters:
	//     pItemArgs - Parameters of item cell.
	//     rcButtons - Bounding rectangle of the button
	//-----------------------------------------------------------------------
	void Create(XTP_REPORTRECORDITEM_ARGS* pItemArgs, CRect& rcButtons);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves identifier of the button
	// Returns:
	//     Identifier of the button.
	//-----------------------------------------------------------------------
	int GetID();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to determine if user pressed the button
	// Returns:
	//     TRUE if user pressed the button.
	//-----------------------------------------------------------------------
	BOOL IsPressed();

protected:

//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTPReportInplaceButton)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT, CPoint);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	BOOL Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID = 0xffff);

protected:
	int m_nID;                              // Identifier of the button.
	int m_nWidth;                           // Width of the button.

	BOOL m_bPressed;                        // TRUE if the button is pressed.
	BOOL m_bOver;                           // TRUE if the mouse cursor is over the button.

	friend class CXTPReportRecordItem;
};

AFX_INLINE int CXTPReportInplaceButton::GetID() {
	return m_nID;
}
AFX_INLINE BOOL CXTPReportInplaceButton::IsPressed() {
	return m_bPressed && m_bOver;
}
AFX_INLINE BOOL CXTPReportInplaceButton::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
AFX_INLINE BOOL CXTPReportInplaceButton::Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) {
	return CStatic::Create(lpszText, dwStyle, rect, pParentWnd, nID);
}


//===========================================================================
// Summary:
//     CXTPReportInplaceList is the CXTPReportInplaceControl derived  class,
//     it represents list box with constraints of item.
// See Also: CXTPReportRecordItemConstraints
//===========================================================================
class CXTPReportInplaceList : public CListBox, public CXTPReportInplaceControl
{
public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPReportInplaceList object
	//-------------------------------------------------------------------------
	CXTPReportInplaceList();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to create in-place list control.
	// Parameters:
	//     pItemArgs - Parameters of item cell.
	//     pConstaints - constraints of item
	//-----------------------------------------------------------------------
	void Create(XTP_REPORTRECORDITEM_ARGS* pItemArgs, CXTPReportRecordItemConstraints* pConstaints);

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to cancel user selection.
	//-------------------------------------------------------------------------
	void Cancel(void);

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to save selected value of list box.
	//-------------------------------------------------------------------------
	void Apply(void);

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPPropertyGridInplaceEdit)
	void PostNcDestroy();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPReportInplaceList)
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonUp(UINT, CPoint point);
	//}}AFX_MSG

//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
};

//////////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL CXTPReportInplaceList::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
AFX_INLINE BOOL CXTPReportInplaceList::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) {
	return CListBox::Create(dwStyle, rect, pParentWnd, nID);
}

#endif // #if !defined(__XTPREPORTINPLACECONTROLS_H__)
