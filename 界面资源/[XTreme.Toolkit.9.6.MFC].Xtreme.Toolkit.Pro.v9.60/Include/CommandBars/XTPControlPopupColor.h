// XTPControlPopupColor.h : interface for the CXTPControlPopupColor class.
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
#if !defined(__XTPCONTROLPOPUPCOLOR_H__)
#define __XTPCONTROLPOPUPCOLOR_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CXTPImageManagerIcon;
class CXTPImageManagerIconHandle;

//===========================================================================
// Summary:
//     CXTPControlPopupColor is a CXTPControlPopup derived class.
//     It represents a color picker control.
//===========================================================================
class _XTP_EXT_CLASS CXTPControlPopupColor : public CXTPControlPopup
{
	DECLARE_XTP_CONTROL(CXTPControlPopupColor)

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPControlPopupColor object
	//-----------------------------------------------------------------------
	CXTPControlPopupColor(void);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPControlPopupColor object, handles cleanup and
	//     deallocation
	//-----------------------------------------------------------------------
	~CXTPControlPopupColor(void);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the picker color.
	// Parameters:
	//     clr - Color to set.
	//-----------------------------------------------------------------------
	void SetColor(COLORREF clr);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the control color.
	// Returns:
	//     Color of the color picker.
	//-----------------------------------------------------------------------
	COLORREF GetColor();

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to copy the control.
	// Parameters:
	//     pControl - Points to a source CXTPControl object
	//     bRecursive - TRUE to copy recursively.
	//-----------------------------------------------------------------------
	void Copy(CXTPControl* pControl, BOOL bRecursive = FALSE);

private:

	void RedrawIcon(CXTPImageManagerIcon* pImage, CXTPImageManagerIconHandle* hIcon);

private:

	COLORREF m_clr;
};


//===========================================================================
// Summary:
//     CXTPControlButtonColor is a CXTPControlButton derived class.
//     It represents a text button in the color picker popup.
//===========================================================================
class _XTP_EXT_CLASS CXTPControlButtonColor: public CXTPControlButton
{
	DECLARE_XTP_CONTROL(CXTPControlButtonColor)
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPControlButtonColor object
	//-----------------------------------------------------------------------
	CXTPControlButtonColor()
	{
		SetStyle(xtpButtonCaption);
	}
protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to determine the size of the control.
	// Parameters:
	//     pDC - Pointer to a valid device context
	// Returns:
	//     Size of the control.
	//-----------------------------------------------------------------------
	CSize GetSize(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the control.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//-----------------------------------------------------------------------
	void Draw(CDC* pDC);
};


//===========================================================================
// Summary:
//     CXTPControlColorSelector is a CXTPControl derived class.
//     It represents a color selector control.
//===========================================================================
class _XTP_EXT_CLASS CXTPControlColorSelector: public CXTPControl
{
	DECLARE_XTP_CONTROL(CXTPControlColorSelector)

	//-----------------------------------------------------------------------
	// Summary:
	//     Internal helper class.
	//-----------------------------------------------------------------------
	struct CColorInfo;

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPControlColorSelector object
	//-----------------------------------------------------------------------
	CXTPControlColorSelector();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the color of the selector control.
	// Returns:
	//     Color of the selector.
	//-----------------------------------------------------------------------
	COLORREF GetColor();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the color of the control.
	// Parameters:
	//     clr - Color to set.
	//-----------------------------------------------------------------------
	void SetColor(COLORREF clr);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to determine the size of the control.
	// Parameters:
	//     pDC - Pointer to a valid device context
	// Returns:
	//     Size of the control.
	//-----------------------------------------------------------------------
	CSize GetSize(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the control.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//-----------------------------------------------------------------------
	void Draw(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when the mouse cursor moves.
	// Parameters:
	//     point - Specifies the x- and y-coordinate of the cursor.
	//-----------------------------------------------------------------------
	void OnMouseMove(CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the control becomes selected.
	// Parameters:
	//     bSelected - TRUE if the control becomes selected.
	// Returns:
	//     TRUE if successful; otherwise returns FALSE
	//-----------------------------------------------------------------------
	BOOL OnSetSelected(int bSelected);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user clicks the control.
	// Parameters:
	//     bKeyboard - TRUE if the control is selected using the keyboard.
	//     pt - Mouse cursor position.
	//-----------------------------------------------------------------------
	void OnClick(BOOL bKeyboard = FALSE, CPoint pt = CPoint(0, 0));

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the control's tooltip.
	// Parameters:
	//     pPoint - Mouse cursor position. Can be NULL to ignore.
	//     lpRectTip - Bounding rectangle of tootip zone. Can be NULL to ignore.
	//     pnHit - Returns hit code of tootip. Can be NULL to ignore.
	// Returns:
	//     Tooltip of the control.
	//-----------------------------------------------------------------------
	CString  GetTooltip(LPPOINT pPoint = 0, LPRECT lpRectTip = 0, int* pnHit = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the control is executed.
	//-----------------------------------------------------------------------
	void OnExecute();

private:

	CRect GetRect(int nIndex);
	int HitTest(CPoint point);

private:

	int m_nSelected;
	int m_nPressed;
	COLORREF m_clr;
};


//////////////////////////////////////////////////////////////////////////

AFX_INLINE COLORREF CXTPControlPopupColor::GetColor() {
	return m_clr;
}

#endif // #if !defined(__XTPCONTROLPOPUPCOLOR_H__)
