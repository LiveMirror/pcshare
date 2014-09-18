// XTColorPageCustom.h : header file
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
#if !defined(__XTCOLORCUSTOM_H__)
#define __XTCOLORCUSTOM_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTColorDialog;

//===========================================================================
// Summary:
//     CXTColorBase is a CStatic derived base class. It is used to create
//     custom color selection windows.
//===========================================================================
class _XT_EXT_CLASS CXTColorBase : public CStatic
{
public:
	// ----------------------------------------------------------------------
	// Summary:
	//     Enumeration used to determine window focus.
	// Remarks:
	//     FocusControl type defines the constants used by CXTColorBase for
	//     determining which area of the color window control has focus.
	// See Also:
	//     CXTColorBase
	//
	// <KEYWORDS focusNone, focusColorWheel, focusLumination>
	// ----------------------------------------------------------------------
	enum FocusedControl
	{
		focusNone,       // Color window does not have input focus.
		focusColorWheel, // Color wheel window has focus.
		focusLumination  // Lumination window has focus.
	};

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTColorBase object
	//-----------------------------------------------------------------------
	CXTColorBase();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTColorBase object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTColorBase();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the color for the selection
	//     window.
	// Parameters:
	//     clr     - An RGB value that represents the color.
	//     bUpdate - true to update the cursor position.
	//-----------------------------------------------------------------------
	virtual void SetColor(COLORREF clr, bool bUpdate=true);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the current color selection.
	// Returns:
	//     An RGB value that indicates the color selection.
	//-----------------------------------------------------------------------
	virtual COLORREF GetColor();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to update the cursor position.
	// Parameters:
	//     point   - Current cursor location relative to the device context.
	//     bNotify - TRUE to notify the parent of change.
	//-----------------------------------------------------------------------
	virtual void UpdateCursorPos(CPoint point, BOOL bNotify=TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to convert an RGB color value to
	//     an HSL value.
	// Parameters:
	//     color - An RGB value that represents the value to convert to HSL.
	//     h     - Represents the color hue.
	//     s     - Represents the color saturation.
	//     l     - Represents the color illumination.
	//-----------------------------------------------------------------------
	static void RGBtoHSL(COLORREF color, double *h, double *s, double *l );

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to convert an HLS value to an RGB
	//     color value.
	// Parameters:
	//     h - Represents the color hue.
	//     l - Represents the color illumination.
	//     s - Represents the color saturation.
	// Returns:
	//     A COLORREF value.
	//-----------------------------------------------------------------------
	static COLORREF HLStoRGB(double h, double l, double s );

protected:

//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTColorBase)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTColorBase)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnInitControl(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

protected:
	CDC                   m_dcPicker;  // Background device context.
	COLORREF              m_clrColor;  // An RGB value that represents the color selection.
	static FocusedControl m_eHasFocus; // Determines which property page has input focus.
	bool m_bInitControl;               // true for initialization

public:
	double m_nLum;       // Current illumination value.
	double m_nSat;       // Current saturation value.
	double m_nHue;       // Current hue value.
	CPoint m_ptMousePos; // Current mouse position relative to the device context.
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE COLORREF CXTColorBase::GetColor() {
	return m_clrColor;
}

//===========================================================================
// Summary:
//     CXTColorWnd is a CXTColorBase derived class. It is used to create a
//     color selection window.
//===========================================================================
class _XT_EXT_CLASS CXTColorWnd : public CXTColorBase
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTColorWnd object
	//-----------------------------------------------------------------------
	CXTColorWnd();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTColorWnd object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTColorWnd();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the color for the selection window.
	// Parameters:
	//     clr     - An RGB value that represents the color.
	//     bUpdate - true to update cursor position.
	//-----------------------------------------------------------------------
	virtual void SetColor(COLORREF clr, bool bUpdate=true);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to update the cursor position.
	// Parameters:
	//     point   - Current cursor location relative to the device context.
	//     bNotify - TRUE to notify the parent of change.
	//-----------------------------------------------------------------------
	virtual void UpdateCursorPos(CPoint point, BOOL bNotify=TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to update the cross hair cursor.
	// Parameters:
	//     pDC - A CDC pointer that represents the current device context.
	//-----------------------------------------------------------------------
	void DrawCrossHair(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to set the hue for the color
	//     window.
	// Parameters:
	//     h - Represents the new hue value to set.
	//-----------------------------------------------------------------------
	void SetHue(double h);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to set the saturation for the
	//     color window.
	// Parameters:
	//     s - Represents the new saturation value to set.
	//-----------------------------------------------------------------------
	void SetSaturation(double s);

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTColorWnd)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTColorWnd)
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE
};

//===========================================================================
// Summary:
//     CXTColorLum is a CXTColorBase derived class. It is used to create a
//     color luminance selection bar.
//===========================================================================
class _XT_EXT_CLASS CXTColorLum : public CXTColorBase
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTColorLum object
	//-----------------------------------------------------------------------
	CXTColorLum();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTColorLum object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTColorLum();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the color for the selection window.
	// Parameters:
	//     clr     - An RGB value that represents the color.
	//     bUpdate - true to update the cursor position.
	//-----------------------------------------------------------------------
	virtual void SetColor(COLORREF clr, bool bUpdate=true);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to update the cursor position.
	// Parameters:
	//     point   - Current cursor location relative to the device context.
	//     bNotify - TRUE to notify the parent of change.
	//-----------------------------------------------------------------------
	virtual void UpdateCursorPos(CPoint point, BOOL bNotify=TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to draw the indicator arrow.
	// Parameters:
	//     pDC - A CDC pointer that represents the current device context.
	//-----------------------------------------------------------------------
	void DrawSliderArrow(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to draw the selection bar.
	// Parameters:
	//     pDC - A CDC pointer that represents the current device context.
	//-----------------------------------------------------------------------
	void DrawLuminanceBar(CDC* pDC);

	// ---------------------------------------------------------------------
	// Summary:
	//     This member function is called to set the luminance for the color
	//     window.
	// Parameters:
	//     l -  Represents the new luminance value to set.
	// ---------------------------------------------------------------------
	void SetLuminance(double l);

	// ----------------------------------------------------------------------
	// Summary:
	//     This member function is called to retrieve the display size of the
	//     luminance bar.
	// Parameters:
	//     rect -  Address to a CRect object.
	// ----------------------------------------------------------------------
	void GetLumBarRect(CRect& rect);

protected:

//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTColorLum)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTColorLum)
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

protected:
	int m_nSliderPos; // Current location of the slider

};


//===========================================================================
// Summary:
//     CXTColorPageCustom is derived from CXTThemePropertyPage. It is used
//     to create a CXTColorPageCustom dialog.
//===========================================================================
class _XT_EXT_CLASS CXTColorPageCustom : public CXTThemePropertyPage
{
	DECLARE_DYNCREATE(CXTColorPageCustom)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTColorPageCustom object
	// Parameters:
	//     pParentSheet - A pointer to the parent sheet.
	//-----------------------------------------------------------------------
	CXTColorPageCustom(CXTColorDialog* pParentSheet=NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTColorPageCustom object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTColorPageCustom();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to retrieve the HSL values of the RGB
	//     specified by color.
	// Parameters:
	//     color - An RGB value that represents the color.
	//     xtFocusLumination   - Address of the integer to receive the illumination value (0-255).
	//     sat   - Address of the integer to receive the saturation value (0-255).
	//     hue   - Address of the integer to receive the hue value (0-255).
	//-----------------------------------------------------------------------
	void RGBtoHSL(COLORREF color, int* xtFocusLumination, int* sat, int* hue);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the dialog to display
	//     the color selector control.
	// Parameters:
	//     pDC - A CDC pointer that represents the current device
	//           context.
	//-----------------------------------------------------------------------
	virtual void OnDrawPage(CDC* pDC);

protected:

//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTColorPageCustom)
	virtual BOOL OnSetActive();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTColorPageCustom)
	afx_msg void OnChangeEdit();
	afx_msg void OnChangeEditLum();
	afx_msg void OnChangeEditHue();
	afx_msg void OnChangeEditSat();
	afx_msg LRESULT OnUpdateColor(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

protected:
//{{AFX_CODEJOCK_PRIVATE
	//{{AFX_DATA(CXTColorPageCustom)
	enum { IDD = XT_IDD_COLORCUSTOM };
	CXTColorWnd m_colorWnd;
	CXTColorLum m_colorLum;
	CStatic m_txtSat;
	CStatic m_txtRed;
	CStatic m_txtLum;
	CStatic m_txtHue;
	CStatic m_txtGreen;
	CStatic m_txtBlue;
	CSpinButtonCtrl m_spinSat;
	CSpinButtonCtrl m_spinRed;
	CSpinButtonCtrl m_spinLum;
	CSpinButtonCtrl m_spinHue;
	CSpinButtonCtrl m_spinGreen;
	CSpinButtonCtrl m_spinBlue;
	CXTEdit m_editHue;
	CXTEdit m_editGreen;
	CXTEdit m_editBlue;
	CXTEdit m_editLum;
	CXTEdit m_editRed;
	CXTEdit m_editSat;
	int     m_nR;
	int     m_nB;
	int     m_nG;
	int     m_nH;
	int     m_nL;
	int     m_nS;
	//}}AFX_DATA
//}}AFX_CODEJOCK_PRIVATE

	friend class CXTColorLum;
};

#endif // !defined(__XTCOLORCUSTOM_H__)
