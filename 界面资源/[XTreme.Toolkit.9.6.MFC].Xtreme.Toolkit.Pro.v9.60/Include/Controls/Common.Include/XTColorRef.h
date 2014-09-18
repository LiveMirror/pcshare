// XTColorRef.h: interface for the CXTColorRef class.
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
#if !defined(__XTCOLORREF_H__)
#define __XTCOLORREF_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     CXTColorRef is a stand alone class. It is used to create a
//     CXTColorRef object. This object is used for manipulation of RGB values.
//===========================================================================
class _XT_EXT_CLASS CXTColorRef
{
public:

	// --------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTColorRef object
	// Parameters:
	//     cr -  An RGB value.
	//     r -   Specifies the intensity of the red color component. 'r' can
	//           range from 0 to 255. Zero is the minimum color intensity; 255 is
	//           the maximum color intensity.
	//     g -   Specifies the intensity of the green color component. 'g' can
	//           range from 0 to 255. Zero is the minimum color intensity; 255 is
	//           the maximum color intensity.
	//     b -   Specifies the intensity of the blue color component. 'b' can
	//           range from 0 to 255. Zero is the minimum color intensity; 255 is
	//           the maximum color intensity.
	// --------------------------------------------------------------------------
	CXTColorRef();
	CXTColorRef(COLORREF cr);         //<combine CXTColorRef::CXTColorRef>
	CXTColorRef(int r, int g, int b); //<combine CXTColorRef::CXTColorRef>

	//-----------------------------------------------------------------------
	// Summary:
	//     This assignment operator is used to initialize a CXTColorRef
	//     object.
	// Parameters:
	//     cr - An RGB value.
	// Returns:
	//     A reference to a valid CXTColorRef object.
	//-----------------------------------------------------------------------
	CXTColorRef& operator = (COLORREF cr);

	//-----------------------------------------------------------------------
	// Summary:
	//     This assignment operator is used to modify a CXTColorRef object.
	// Parameters:
	//     val - Specifies the intensity of the color component. 'val'
	//           can range from 0 to 255. Zero is the minimum color intensity;
	//           255 is the maximum color intensity.
	//     cr - An RGB value.
	// Returns:
	//     A CXTColorRef object.
	//-----------------------------------------------------------------------
	CXTColorRef operator + (int val) const;
	CXTColorRef operator + (COLORREF cr) const; //<combine CXTColorRef::+@int@const>

	//-----------------------------------------------------------------------
	// Summary:
	//     This assignment operator is used to modify a CXTColorRef object.
	// Parameters:
	//     val - Specifies the intensity of the color component. 'val'
	//           can range from 0 to 255. Zero is the minimum color intensity;
	//           255 is the maximum color intensity.
	//     cr - An RGB value.
	// Returns:
	//     A CXTColorRef object.
	//-----------------------------------------------------------------------
	CXTColorRef operator - (int val) const;
	CXTColorRef operator - (COLORREF cr) const; //<combine CXTColorRef::-@int@const>

	//-----------------------------------------------------------------------
	// Summary:
	//     This assignment operator is used to modify a CXTColorRef object.
	// Parameters:
	//     val - Specifies the intensity of the color component. 'val' can
	//           range from 0 to 255. Zero is the minimum color intensity;
	//           255 is the maximum color intensity.
	//     cr - An RGB value.
	// Returns:
	//     A CXTColorRef object.
	//-----------------------------------------------------------------------
	CXTColorRef operator * (int val) const;
	CXTColorRef operator * (COLORREF cr) const; //<combine CXTColorRef::*@int@const>

	//-----------------------------------------------------------------------
	// Summary:
	//     This assignment operator is used to modify a CXTColorRef object.
	// Returns:
	//     A CXTColorRef object.
	// Parameters:
	//     val - Specifies the intensity of the color component. 'val' can
	//           range from 0 to 255. Zero is the minimum color intensity;
	//           255 is the maximum color intensity.
	//     cr - An RGB value.
	//-----------------------------------------------------------------------
	CXTColorRef operator / (int val) const;
	CXTColorRef operator / (COLORREF cr) const; //<combine CXTColorRef::/@int@const>

	//-----------------------------------------------------------------------
	// Summary:
	//     This assignment operator is used to modify a CXTColorRef object.
	// Parameters:
	//     val - Specifies the intensity of the color component. 'val' can
	//           range from 0 to 255. Zero is the minimum color intensity;
	//           255 is the maximum color intensity.
	//     cr - An RGB value.
	// Returns:
	//     A reference to a valid CXTColorRef object.
	//-----------------------------------------------------------------------
	CXTColorRef& operator += (int val);
	CXTColorRef& operator += (COLORREF cr); // <combine CXTColorRef::+=@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     This assignment operator is used to modify a CXTColorRef object.
	// Parameters:
	//     val - Specifies the intensity of the color component. 'val' can
	//           range from 0 to 255. Zero is the minimum color intensity;
	//           255 is the maximum color intensity.
	//     cr - An RGB value.
	// Returns:
	//     A reference to a valid CXTColorRef object.
	//-----------------------------------------------------------------------
	CXTColorRef& operator -= (int val);
	CXTColorRef& operator -= (COLORREF cr); // <combine CXTColorRef::-=@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     This assignment operator is used to modify a CXTColorRef object.
	// Parameters:
	//     val - Specifies the intensity of the color component. 'val' can
	//           range from 0 to 255. Zero is the minimum color intensity;
	//           255 is the maximum color intensity.
	//     cr - An RGB value.
	// Returns:
	//     A reference to a valid CXTColorRef object.
	//-----------------------------------------------------------------------
	CXTColorRef& operator *= (int val);
	CXTColorRef& operator *= (COLORREF cr); // <combine CXTColorRef::*=@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     This assignment operator is used to modify a CXTColorRef object.
	// Parameters:
	//     val - Specifies the intensity of the color component. 'val' can
	//           range from 0 to 255. Zero is the minimum color intensity;
	//           255 is the maximum color intensity.
	//     cr - An RGB value.
	// Returns:
	//     A reference to a valid CXTColorRef object.
	//-----------------------------------------------------------------------
	CXTColorRef& operator /= (int val);
	CXTColorRef& operator /= (COLORREF cr); // <combine CXTColorRef::/=@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     This operator retrieves the RGB value for the CXTColorRef object.
	// Returns:
	//     An RGB value.
	//-----------------------------------------------------------------------
	operator COLORREF () const;

	//////////////////////////////////////////////////////////////
	// RGB Values
	//////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------
	// Summary:
	//     This method gets the red component of this object.
	// Returns:
	//     The red component of the color.
	//-----------------------------------------------------------------------
	int getRValue() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method gets the green component of this object.
	// Returns:
	//     The green component of the color.
	//-----------------------------------------------------------------------
	int getGValue() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method gets the blue component of this object.
	// Returns:
	//     The blue component of the color.
	//-----------------------------------------------------------------------
	int getBValue() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the red intensity value for
	//     the CXTColorRef object.
	// Parameters:
	//     val - Specifies the intensity of the color component. 'val' can
	//           range from 0 to 255. Zero is the minimum color intensity;
	//           255 is the maximum color intensity.
	//-----------------------------------------------------------------------
	void setRValue(int val);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the green intensity value for the
	//     CXTColorRef object.
	// Parameters:
	//     val - Specifies the intensity of the color component. 'val' can
	//           range from 0 to 255. Zero is the minimum color intensity;
	//           255 is the maximum color intensity.
	//-----------------------------------------------------------------------
	void setGValue(int val);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the blue intensity value for the CXTColorRef
	//     object.
	// Parameters:
	//     val - Specifies the intensity of the color component. 'val' can
	//           range from 0 to 255. Zero is the minimum color intensity;
	//           255 is the maximum color intensity.
	//-----------------------------------------------------------------------
	void setBValue(int val);

	//////////////////////////////////////////////////////////////
	// Misc Information
	//////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will retrieve the overall intensity of the
	//     CXTColorRef object.
	// Returns:
	//     A double that represents the current color intensity.
	//-----------------------------------------------------------------------
	double getIntensity() const;

	//////////////////////////////////////////////////////////////
	// Color Conversion
	//////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------
	// Summary:
	//     This function converts the RGB color to an HSL (hue, saturation,
	//     luminance) trio.
	// Parameters:
	//     h - The variable to receive the hue value.
	//     s - The variable to receive the saturation value.
	//     l - The variable to receive the luminance value.
	//-----------------------------------------------------------------------
	void toHSL(double& h, double& s, double& l) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This function converts the RGB color to an HSB (hue, saturation,
	//     brightness) trio.
	// Parameters:
	//     h - The variable to receive the hue value.
	//     s - The variable to receive the saturation value.
	//     b - The variable to receive the brightness value.
	//-----------------------------------------------------------------------
	void toHSB(double& h, double& s, double& b) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This function sets an RGB value that is the equivalent of a specific
	//     HSL (hue, saturation, luminance) color.
	// Parameters:
	//     h - The hue of the source color.
	//     s - The saturation of the source color.
	//     l - The luminance of the source color.
	//-----------------------------------------------------------------------
	void setHSL(double h, double s, double l);

	//-----------------------------------------------------------------------
	// Summary:
	//     This function sets an RGB value that is the equivalent of a specific
	//     HSB (hue, saturation, brightness) color.
	// Parameters:
	//     h - The hue of the source color.
	//     s - The saturation of the source color.
	//     b - The brightness of the source color.
	//-----------------------------------------------------------------------
	void setHSB(double h, double s, double b);

	// ----------------------------------------------------------------------
	// Summary:
	//     Creates a new CXTColorRef based on hue saturation and luminance.
	// Parameters:
	//     h -  The hue of the source color.
	// s -  The saturation of the source color.
	// l -  The luminance of the source color.
	// Remarks:
	//     This static function creates a new CXTColorRef object out of an
	//     HSL (hue, saturation, luminance) color trio. Returns a CXTColorRef
	//     \object.
	// ----------------------------------------------------------------------
	static CXTColorRef fromHSL(double h, double s, double l);

	// ---------------------------------------------------------------------
	// Summary:
	//     Creates a new CXTColorRef based on hue saturation and brightness.
	// Parameters:
	//     h -  The hue of the source color.
	// s -  The saturation of the source color.
	// b -  The brightness of the source color.
	// Remarks:
	//     This static function creates a new CXTColorRef object out of an
	//     HSB (hue, saturation, brightness) color trio. Returns a
	//     CXTColorRef object.
	// ---------------------------------------------------------------------
	static CXTColorRef fromHSB(double h, double s, double b);

	//////////////////////////////////////////////////////////////////////
	// Color Manipulation
	//////////////////////////////////////////////////////////////////////

	// ----------------------------------------------------------------------
	// Summary:
	//     Blends two CXTColorRef objects into a single color.
	// Parameters:
	//     clr -      A reference to a valid CXTColorRef object.
	// opacity -  The amount of transparency.
	// Remarks:
	//     Call this member function to blend the CXTColorRef object with the
	//     \object specified by 'clr'.
	// Returns:
	//     A reference to a valid CXTColorRef object.
	// ----------------------------------------------------------------------
	CXTColorRef& blend(const CXTColorRef& clr, int opacity);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to add the CXTColorRef object specified by 'clr'.
	// Parameters:
	//     clr - A reference to a valid CXTColorRef object.
	// Returns:
	//     A reference to a valid CXTColorRef object.
	//-----------------------------------------------------------------------
	CXTColorRef& add(const CXTColorRef& clr);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to subtract the CXTColorRef object specified
	//     by 'clr'.
	// Parameters:
	//     clr - A reference to a valid CXTColorRef object.
	// Returns:
	//     A reference to a valid CXTColorRef object.
	//-----------------------------------------------------------------------
	CXTColorRef& subtract(const CXTColorRef& clr);

	//////////////////////////////////////////////////////////////////////
	// Standard Colors
	//////////////////////////////////////////////////////////////////////

	static const CXTColorRef BLACK;   // RGB value that represents the color black.
	static const CXTColorRef WHITE;   // RGB value that represents the color white.
	static const CXTColorRef GRAY_25; // RGB value that represents the 25% gray color.
	static const CXTColorRef GRAY_50; // RGB value that represents the 50% gray color.
	static const CXTColorRef GRAY_75; // RGB value that represents the 75% gray color.

	//////////////////////////////////////////////////////////////////////
	// Helper Functions
	//////////////////////////////////////////////////////////////////////

	// -----------------------------------------------------------------------------------
	// Summary:
	//     Adds two color values.
	// Parameters:
	//     iVal -  Specifies the color intensity that is the source of the
	//             operation.
	// iAdd -  Specifies the intensity to add to the source. It can be negative.
	// Remarks:
	//     This member function adds two color intensities to ensure that the
	//     sum is in the proper color value range (0-255). For example, addColorValue(200,
	//     100) will return 255 and not 300. addColorValue(100, -200) will
	//     return 0.
	// -----------------------------------------------------------------------------------
	static int addColorValue(int iVal, int iAdd);

	// ----------------------------------------------------------------------------
	// Summary:
	//     Modifies a color value based on intensity and multiplication.
	// Parameters:
	//     iVal -   Specifies the color intensity that is the source of the
	//              operation.
	// iMult -  Specifies a value to multiply the source by.
	// iDiv -   Specifies a value to divide the source by.
	// Remarks:
	//     This member function manipulates a color value by performing a
	//     multiplication and then a division. For instance, multiplyColorValue(10,
	//     2, 3) will return 10 * 2 / 3 which is 6. The function guarantees
	//     that the resulting value does not exceeded 255.
	// ----------------------------------------------------------------------------
	static int multiplyColorValue(int iVal, int iMult, int iDiv);

	// ------------------------------------------------------------------------------
	// Summary:
	//     Blends two color intensities.
	// Parameters:
	//     iClrFront :  Specifies the intensity of the foreground color component.
	//                  'iClrFront' can range from 0 to 255. Zero is the minimum
	//                  color intensity; 255 is the maximum color intensity.
	// iClrBack :   Specifies the intensity of the background color component.
	//                  'iClrBack' can range from 0 to 255. Zero is the minimum color
	//                  intensity; 255 is the maximum color intensity.
	// opacity :    The amount of transparency.
	// Remarks:
	//     This function takes a blended color intensity which is
	//     ((100-opacity)% * iClrFront and opacity% * iClrBack).
	// ------------------------------------------------------------------------------
	static int BlendColor(int iClrFront, int iClrBack, int opacity);

	// -------------------------------------------------------------------------
	// Summary:
	//     Adjusts color intensity to a valid range.
	// Parameters:
	//     iVal -  Specifies the intensity of the color component. 'iVal' can
	//             range from 0 to 255. Zero is the minimum color intensity; 255
	//             is the maximum color intensity.
	// Remarks:
	//     This function takes in a color intensity and ensures that it is
	//     between 0 and 255. Anything out of range will be adjusted to the
	//     closest number in range (i.e. -100 will become 0, 300 will become
	//     255).
	// -------------------------------------------------------------------------
	static int fixColorValue(int iVal);

	//-----------------------------------------------------------------------
	// Summary:
	//     This function will refresh the display element colors defined
	//     in the CXTColorRef object.
	//-----------------------------------------------------------------------
	static void RefreshColors();

	// ---------------------------------------------------------------------------
	// Summary:
	//     Retrieves the current color of the specified display element.
	// Parameters:
	//     nIndex -  Specifies the display element whose color is to be retrieved.
	//               This parameter can be one of the values listed in the remarks
	//               section.
	// Remarks:
	//     This function retrieves the current color of the specified XP
	//     display element. Display elements are the parts of a window and
	//     the display that appear on the system display screen.
	//
	//     Color can be defined as one of the following values:
	//     * <b>XPCOLOR_BASE</b> An RGB value that represents the XP base
	//           color.
	//     * <b>XPCOLOR_BARFACE</b> An RGB value that represents the XP
	//           toolbar background color.
	//     * <b>XPCOLOR_HIGHLIGHT</b> An RGB value that represents the XP
	//           menu item selected color.
	//     * <b>XPCOLOR_HIGHLIGHT_BORDER</b> An RGB value that represents
	//           the XP menu item selected border color.
	//     * <b>XPCOLOR_HIGHLIGHT_PUSHED</b> An RGB value that represents
	//           the XP menu item pushed color.
	//     * <b>XPCOLOR_ICONSHADDOW</b> An RGB value that represents the
	//           XP menu item icon shadow.
	//     * <b>XPCOLOR_GRAYTEXT</b> An RGB value that represents the XP
	//           menu item disabled text color.
	//     * <b>XPCOLOR_HIGHLIGHT_CHECKED</b> An RGB value that
	//           represents the XP menu item checked color.
	//     * <b>XPCOLOR_HIGHLIGHT_CHECKED_BORDER</b> An RGB value that
	//           represents the XP menu item checked border color.
	//     * <b>XPCOLOR_GRIPPER</b> An RGB value that represents the XP
	//           toolbar gripper color.
	//     * <b>XPCOLOR_SEPARATOR</b> An RGB value that represents the XP
	//           toolbar separator color.
	//     * <b>XPCOLOR_DISABLED</b> An RGB value that represents the XP
	//           menu icon disabled color.
	//     * <b>XPCOLOR_MENUTEXT_BACK</b> An RGB value that represents
	//           the XP menu item text background color.
	//     * <b>XPCOLOR_MENU_EXPANDED</b> An RGB value that represents
	//           the XP hidden menu commands background color.
	//     * <b>XPCOLOR_MENU_BORDER</b> An RGB value that represents the
	//           XP menu border color.
	//     * <b>XPCOLOR_MENUTEXT</b> An RGB value that represents the XP
	//           menu item text color.
	//     * <b>XPCOLOR_HIGHLIGHT_TEXT</b> An RGB value that represents
	//           the XP menu item selected text color.
	//     * <b>XPCOLOR_BARTEXT</b> An RGB value that represents the XP
	//           toolbar text color.
	//     * <b>XPCOLOR_BARTEXT_PUSHED</b> An RGB value that represents
	//           the XP toolbar pushed text color.
	//     * <b>XPCOLOR_TAB_INACTIVE_BACK</b> An RGB value that
	//           represents the XP inactive tab background color.
	//     * <b>XPCOLOR_TAB_INACTIVE_TEXT</b> An RGB value that
	//           represents the XP inactive tab text color.
	// Returns:
	//     The red, green, blue (RGB) color value of the given element.
	// ---------------------------------------------------------------------------
	static COLORREF GetColor(int nIndex);

protected:

	COLORREF m_ColorRef; // Current RGB value for this object

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by setHSL() to convert hue.
	// Parameters:
	//     temp1 - Temp value.
	//     temp2 - Temp value.
	//     temp3 - Temp value.
	// Returns:
	//     A double that represents either an r, g, or b COLORREF value.
	//-----------------------------------------------------------------------
	static double HuetoRGB(double temp1, double temp2, double temp3);

	// -------------------------------------------------------------------------
	// Summary:
	//     Retrieves a save COLORREF value from the specified r, g, b values.
	// Parameters:
	//     r -  Specifies the intensity of the red color component. 'r' can
	//          range from 0 to 255. Zero is the minimum color intensity; 255 is
	//          the maximum color intensity.
	// g -  Specifies the intensity of the green color component. 'g' can
	//          range from 0 to 255. Zero is the minimum color intensity; 255 is
	//          the maximum color intensity.
	// b -  Specifies the intensity of the blue color component. 'b' can
	//          range from 0 to 255. Zero is the minimum color intensity; 255 is
	//          the maximum color intensity.
	// Remarks:
	//     This function takes in a color intensity and ensures that it is
	//     between 0 and 255. Anything out of range will be adjusted to the
	//     closest number in range (i.e. -100 will become 0, 300 will become
	//     255).
	// Returns:
	//     An RGB value.
	// -------------------------------------------------------------------------
	static COLORREF SafeRGB(int r, int g, int b);

};

//////////////////////////////////////////////////////////////////////

AFX_INLINE CXTColorRef::CXTColorRef() {
}

AFX_INLINE CXTColorRef& CXTColorRef::operator = (COLORREF cr) {
	m_ColorRef = cr; return *this;
}
AFX_INLINE CXTColorRef::CXTColorRef(COLORREF r) {
	*this = r;
}
AFX_INLINE CXTColorRef::CXTColorRef(int r, int g, int b) {
	*this = RGB(r, g, b);
}
AFX_INLINE int CXTColorRef::fixColorValue(int iVal) {
	if (iVal > 255) return 255; if (iVal < 0)   return 0; return iVal;
}
AFX_INLINE CXTColorRef::operator COLORREF () const {
	return m_ColorRef;
}
AFX_INLINE int CXTColorRef::getRValue() const {
	return GetRValue(m_ColorRef);
}
AFX_INLINE int CXTColorRef::getGValue() const {
	return GetGValue(m_ColorRef);
}
AFX_INLINE int CXTColorRef::getBValue() const {
	return GetBValue(m_ColorRef);
}
AFX_INLINE void CXTColorRef::setRValue(int val) {
	m_ColorRef = RGB(val, getGValue(), getBValue());
}
AFX_INLINE void CXTColorRef::setGValue(int val) {
	m_ColorRef = RGB(getRValue(), val, getBValue());
}
AFX_INLINE void CXTColorRef::setBValue(int val) {
	m_ColorRef = RGB(getRValue(), getGValue(), val);
}
AFX_INLINE double CXTColorRef::getIntensity() const {
	return  ((299 * getRValue()) + (587 * getGValue()) + (114 * getBValue())) / 1000.0;
}
AFX_INLINE int CXTColorRef::addColorValue(int iVal, int iAdd) {
	return fixColorValue(iVal + iAdd);
}
AFX_INLINE int CXTColorRef::multiplyColorValue(int iVal, int iMult, int iDiv) {
	return fixColorValue(iVal * iMult / iDiv);
}
AFX_INLINE int CXTColorRef::BlendColor(int iClrFront, int iClrBack, int opacity) {
	return fixColorValue(((iClrFront * opacity) + (iClrBack * (100 - opacity))) / 100);
}


#endif // !defined(__XTCOLORREF_H__)
