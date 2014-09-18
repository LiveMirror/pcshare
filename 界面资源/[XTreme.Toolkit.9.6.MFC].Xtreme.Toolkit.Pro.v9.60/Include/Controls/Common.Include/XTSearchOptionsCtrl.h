// XTSearchOptionsCtrl.h : header file
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
#if !defined(__XTSEARCHOPTIONSCTRL_H__)
#define __XTSEARCHOPTIONSCTRL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     CXTSearchOptionsCtrl is a CStatic derived class. CXTSearchOptionsCtrl
//     is used to create a control similar to the Search Options item found
//     in the search pane of Windows Explorer as it is seen in Windows 2000 and
//     later. This class allows you to associate a group of controls to be
//     hidden or shown when the item is expanded and contracted and a set
//     of controls that need to move depending on the CXTSearchOptionsCtrl state.
//
//          Use with CXTSearchOptionsView form view class to create a pane similar
//          to the Windows Explorer search pane. To use the control, define a set
//          of controls that are to be hidden and moved depending on the
//          CXTSearchOptionsCtrl state.
//
// Example:
// <code>
// void CExpandTestView::OnInitialUpdate()
// {
//    CXTSearchOptionsView::OnInitialUpdate();
//
//    m_expand.AddControl(&m_check1);
//    m_expand.AddControl(&m_check2);
//    m_expand.AddControl(&m_edit1);
//    m_expand.AddControl(&m_edit2);
//
//    m_expand.MoveControl(&m_button1);
//    m_expand.MoveControl(&m_button2);
//    m_expand.MoveControl(&m_combo1);
//
//    m_expand.SetLabelText(
//        _T("Search Options <<"), _T("Search Options >>"));
// }
// </code>
//
//  See the "SearchOptions" demo for a complete example.
//===========================================================================
class _XT_EXT_CLASS CXTSearchOptionsCtrl : public CStatic
{
	DECLARE_DYNAMIC(CXTSearchOptionsCtrl)

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTSearchOptionsCtrl object
	//-----------------------------------------------------------------------
	CXTSearchOptionsCtrl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTSearchOptionsCtrl object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTSearchOptionsCtrl();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the search options
	//     control has been expanded.
	// Returns:
	//     True if expanded, otherwise returns false.
	//-----------------------------------------------------------------------
	bool IsExpanded() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to add a control to the list of controls
	//     that are displayed when the hide item control is expanded.
	// Parameters:
	//     pWndCtrl - Points to a valid CWnd object to hide.
	//-----------------------------------------------------------------------
	void AddControl(CWnd* pWndCtrl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to add a control to the list of controls
	//     that are moved when the hide item control is either expanded or contracted.
	// Parameters:
	//     pWndCtrl - Points to a valid CWnd object to move.
	//-----------------------------------------------------------------------
	void MoveControl(CWnd* pWndCtrl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the text that is displayed when the
	//     hide item control is either expanded or contracted.
	// Parameters:
	//     lpszExpand   - NULL terminated string that represents the text displayed when
	//                    the control is expanded.
	//     lpszContract - NULL terminated string that represents the text displayed when
	//                    the control is contracted.
	//-----------------------------------------------------------------------
	void SetLabelText(LPCTSTR lpszExpand,LPCTSTR lpszContract);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to expand the hide item control and display
	//     CWnd objects contained in the hide item list. Called by the control
	//     whenever the user clicks on the expand label.
	//-----------------------------------------------------------------------
	void Expand();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to contract the hide item control and hide
	//     CWnd objects contained in the hide item list. Called by the control
	//     whenever the user clicks on the contract label.
	//-----------------------------------------------------------------------
	void Contract();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the minimum height of the hide
	//     item control.
	// Returns:
	//     An integer value that represents the height of the
	//     control when it is contracted.
	//-----------------------------------------------------------------------
	int GetMinSize();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the maximum height of the hide
	//     item control.
	// Returns:
	//     An integer value that represents the height of the
	//     control when it is expanded.
	//-----------------------------------------------------------------------
	int GetMaxSize();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the offset size for the
	//     CXTSearchOptionsCtrl object. This is the distance the controls will
	//     be moved to accommodate for the expansion and contraction of the control.
	//     Also used by CXTSearchOptionsView for adjusting scroll sizes.
	// Returns:
	//     An integer value that represents the distance the controls will be moved.
	//-----------------------------------------------------------------------
	int GetOffsetSize();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     defers control initialization
	//-----------------------------------------------------------------------
	void DeferInitialUpdate();

protected:

//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTSearchOptionsCtrl)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTSearchOptionsCtrl)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg LRESULT OnInitControl(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

protected:

	int         m_iMinSize;         // Height of the control when contracted.
	int         m_iMaxSize;         // Height of the control when expanded.
	bool        m_bExpanded;        // true when the control is expanded.
	CRect       m_rcLabel;          // Size of the label that is displayed.
	DWORD       m_dwInitSignature;  // used for one-time initialization.
	CString     m_strExpandLabel;   // Label to display when the control is expanded.
	CString     m_strContractLabel; // Label to display when the control is contracted.
	CPtrArray   m_arHideCtrls;      // List of controls to show or hide.
	CPtrArray   m_arMoveCtrls;      // List of controls to move when expanded or contracted.

};

//////////////////////////////////////////////////////////////////////

AFX_INLINE int CXTSearchOptionsCtrl::GetMinSize() {
	return m_iMinSize;
}
AFX_INLINE int CXTSearchOptionsCtrl::GetMaxSize() {
	return m_iMaxSize;
}
AFX_INLINE int CXTSearchOptionsCtrl::GetOffsetSize() {
	return GetMaxSize()-GetMinSize();
}
AFX_INLINE bool CXTSearchOptionsCtrl::IsExpanded() const {
	return m_bExpanded;
}

// forwards
class CXTSearchOptionsCtrl;


//===========================================================================
// Summary:
//     CXTSearchOptionsView is a CFormView derived class. It is to
//     be used with a CXTSearchOptionsCtrl object to paint the background
//     and control background color white. It is also used for resizing
//     and moving children of the form automatically.
//===========================================================================
class _XT_EXT_CLASS CXTSearchOptionsView : public CFormView
{
	DECLARE_DYNAMIC(CXTSearchOptionsView)

private:
	// CResizeWnd - private helper class
	class CResizeWnd
	{
	public:
		CResizeWnd(CWnd* pWndParent, CWnd* pWndChild, const XT_RESIZEPOINT& ptTopLeft, const XT_RESIZEPOINT& ptTopRight);
		virtual ~CResizeWnd();
		bool Resize(HDWP& hDWP, float iOffset);
		bool IsGroupBox();

		CWnd*           m_pWndChild;
		CWnd*           m_pWndParent;
		CRect           m_rcWindow;
		CRect           m_rcParent;
		CXTResizeRect   m_rcSizing;
	};

private:
	// CResizeWndArray - private helper class
	typedef CList<CResizeWnd*,CResizeWnd*> CResizeWndArray;

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     When you create an object of a type derived from CXTSearchOptionsView, invoke one of
	//     the constructors to create the view object and identify the dialog resource
	//     on which the view is based. You can identify the resource either by name
	//     (pass a string as the argument to the constructor) or by its ID (pass an
	//     unsigned integer as the argument).
	// Parameters:
	//     lpszTemplateName - Contains a null-terminated string that is the name of
	//                        a dialog-template resource.
	//
	//          The form-view window and child controls are not created until CWnd::Create
	//          is called. CWnd::Create is called by the framework as part of the document
	//          and view creation process, which is driven by the document template.
	//-----------------------------------------------------------------------
	CXTSearchOptionsView(LPCTSTR lpszTemplateName);

	//-----------------------------------------------------------------------
	// Summary:
	//     When you create an object of a type derived from CXTSearchOptionsView, invoke one of
	//     the constructors to create the view object and identify the dialog resource
	//     on which the view is based. You can identify the resource either by name
	//     (pass a string as the argument to the constructor) or by its ID (pass an
	//     unsigned integer as the argument).
	// Parameters:
	//     nIDTemplate - Contains the ID number of a dialog-template resource.
	//
	//          The form-view window and child controls are not created until CWnd::Create
	//          is called. CWnd::Create is called by the framework as part of the document
	//          and view creation process, which is driven by the document template.
	//-----------------------------------------------------------------------
	CXTSearchOptionsView(UINT nIDTemplate);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTSearchOptionsView object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTSearchOptionsView();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     The SetResize function specifies how much each side of a control will
	//     move when the dialog is resized. If a control should be repositioned
	//     (e.g. an OK button), all four sides should move by the same amount
	//     of pixels as the dialog is resized. If a control should be resized
	//     just as much as the dialog (e.g. the list control in the file dialog),
	//     the left and top sides should not move, and the right and bottom
	//     sides should move by the same amount of pixels as the dialog.
	// Parameters:
	//     nID        - Specifies the control's ID.
	//     ptTopLeft  - How much the top and left sides will move when the dialog is resized.
	//     ptTopRight - How much the bottom and right sides will move when the dialog is resized.
	//-----------------------------------------------------------------------
	void SetResize(int nID, const XT_RESIZEPOINT& ptTopLeft, const XT_RESIZEPOINT& ptTopRight);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to adjust the scroll sizes for the form view.
	// Parameters:
	//     pSOCtrl - Points to the CXTSearchOpetionsCtrl object to adjust the scroll
	//               sizes for.
	//     bAdd    - True to add the size, false to subtract size.
	//-----------------------------------------------------------------------
	void AdjustScrollSizes(CXTSearchOptionsCtrl* pSOCtrl, bool bAdd);

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTSearchOptionsView)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTSearchOptionsView)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

protected:
	HBRUSH          m_hBrush;      // Handle to the current background brush
	CRect           m_rcWindow;    // Original size of the window.
	CResizeWndArray m_arResizeWnd; // Array of children to move or resize.
};

#endif // !defined(__XTSEARCHOPTIONSCTRL_H__)
