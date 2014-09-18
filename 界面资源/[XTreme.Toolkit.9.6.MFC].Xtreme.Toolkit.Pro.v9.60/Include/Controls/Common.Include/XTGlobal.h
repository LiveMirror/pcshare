// XTGlobal.h interface for the CXTAuxData struct.
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
#if !defined(__XTGLOBALS_H__)
#define __XTGLOBALS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "CommonPro.include/XTPMacros.h"
#include "CommonPro.include/XTPManageState.h"

// ----------------------------------------------------------------------
// Summary:
//     XT_LVITEMDATA structure is used to maintain information for
//     a particular list item in a CXTShellListCtrl or CXTShellListView
//     window.
// See Also:
//     CXTShellListCtrl, CXTShellListView
// ----------------------------------------------------------------------
struct XT_LVITEMDATA
{
	ULONG           ulAttribs;  // Shell item attributes.
	LPITEMIDLIST    lpi;        // Pointer to an item ID list.
	LPSHELLFOLDER   lpsfParent; // Points to the parent shell folder item.
};

// ----------------------------------------------------------------------
// Summary:
//     XT_TVITEMDATA structure is used to maintain information for
//     a particular tree item in a CXTShellTreeCtrl or CXTShellTreeView
//     window.
// See Also:
//     CXTShellTreeCtrl, CXTShellTreeView
// ----------------------------------------------------------------------
struct XT_TVITEMDATA
{
	LPITEMIDLIST    lpi;        // Pointer to an item ID list.
	LPITEMIDLIST    lpifq;      // Pointer to an item ID list.
	LPSHELLFOLDER   lpsfParent; // Pointer to the parent shell folder item.
};


// ----------------------------------------------------------------------
// Summary:
//     XT_TCB_ITEM structure is used to maintain information for a
//     particular tab item in a CXTTabCtrl and CXTTabView window.
// See Also:
//     CXTTabCtrl and CXTTabView
// ----------------------------------------------------------------------
struct XT_TCB_ITEM
{
	UINT    uiToolTipId;        // Resource ID for the tooltip.
	CWnd*   pWnd;               // A CWnd pointer to the window associated with a tab.
	CString szTabLabel;         // User specified label for the tab.
	CString szToolTipLabel;     // Tooltip text for the tab.
	CString szCondensedLabel;   // The label actually being displayed for auto-condensing tabs.
};

//=============================================================================
// Summary:
//     CXTNonClientMetrics is a self initializing NONCLIENTMETRICS derived
//     class. It contains the scalable metrics associated with the
//     non-client area of a non-minimized window.  This class is used by
//     the SPI_GETNONCLIENTMETRICS and SPI_SETNONCLIENTMETRICS actions of
//     SystemParametersInfo.
// Example:
//    The following example demonstrates using CXTNonClientMetrics.
// <code>
// CXTLogFont lfMenuFont;
// CXTNonClientMetrics ncm;
//
// lfMenuFont.lfCharSet = (BYTE)::GetTextCharsetInfo (dc, NULL, 0);
// lfMenuFont.lfHeight  = ncm.lfMenuFont.lfHeight;
// lfMenuFont.lfWeight  = ncm.lfMenuFont.lfWeight;
// lfMenuFont.lfItalic  = ncm.lfMenuFont.lfItalic;
//
// STRCPY_S(lfMenuFont.lfFaceName, LF_FACESIZE, ncm.lfMenuFont.lfFaceName);
// </code>
//=============================================================================
class _XT_EXT_CLASS CXTNonClientMetrics : public NONCLIENTMETRICS
{
public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs an CXTNonClientMetrics object
	//-------------------------------------------------------------------------
	CXTNonClientMetrics();
};

// --------------------------------------------------------------------
// Summary:
//     The CXTAuxDataTheme class is used by the CXTAuxData class
//     to provide backward compatibility with older versions that still
//     use <i>bXPMode</i> to provide Office XP theme support.
// --------------------------------------------------------------------
class _XT_EXT_CLASS CXTAuxDataTheme
{
public:
	// ---------------------------------------------------------------------
	// Summary:
	//     Determines if Office XP themes are enabled for standard common
	//     controls.
	// Remarks:
	//     \Returns BOOL (MFC typedef) value. TRUE means that Office XP
	//     themes are enabled for standard common controls; FALSE otherwise.
	// ---------------------------------------------------------------------
	operator BOOL();

	// -------------------------------------------------------------------
	// Summary:
	//     This operator will toggle the Office XP theme for standard
	//     controls.
	// Parameters:
	//     bXPMode -  [in] TRUE to enable Office XP theme.
	// Remarks:
	//     Use this operator overload to enable or disable Office XP theme
	//     support for standard common controls.
	// Returns:
	//     TRUE if Office XP themes are enabled; otherwise FALSE.
	// -------------------------------------------------------------------
	BOOL operator= (BOOL bXPMode);
};

//=============================================================================
// Summary:
//     CXTLogFont is a self initializing LOGFONT derived class. It
//     will allow you to create or copy a LOGFONT object, and defines the
//     attributes of a font.
//=============================================================================
class _XT_EXT_CLASS CXTLogFont : public LOGFONT
{
public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs an CXTLogFont object.
	//     The copy constructor will construct an CXTLogFont object and copy
	//     the data specified by 'logfont' into the structure's data members.
	// Parameters:
	//     logfont - Valid address of a LOGFONT structure.
	//-------------------------------------------------------------------------
	CXTLogFont();
	CXTLogFont(LOGFONT& logfont); //<combine CXTLogFont::CXTLogFont>

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     This overloaded operator will copy the data specified by 'logfont'
	//     into the structure's data members.
	// Parameters:
	//     logfont - Valid address of a LOGFONT structure.
	//-------------------------------------------------------------------------
	void operator = (LOGFONT& logfont);

public:
	DWORD dwType;  // Used to hold the font type, i.e. TT_FONT, DEVICE_FONT.
};

class CXTAuxData;

//---------------------------------------------------------------------------
// Summary:
//     The XTAuxData function is used for access to the
//     CXTAuxData class.
// Remarks:
//     Call this member function to access CXTAuxData members. Since
//     this class is designed as a single instance object you can only
//     access its members through this method. You cannot directly
//     instantiate an object of type CXTAuxData.
// Returns:
//     A pointer to the one and only CXTAuxData instance.
// Example:
// <code>
// bool bIsWinNT = XTAuxData().bWinNT;
// </code>
// See Also:
//     CXTAuxData
//---------------------------------------------------------------------------
_XT_EXT_CLASS CXTAuxData& AFXAPI XTAuxData();

// ---------------------------------------------------------------------
// Summary:
//     CXTAuxData is a stand alone global item data structure
//     class. It is used by the toolkit to initialize and store resource
//     and item data shared by all objects. Items include system colors,
//     icons, fonts and OS flags.
// ---------------------------------------------------------------------
class _XT_EXT_CLASS CXTAuxData
{
private:
	// ------------------------------------------
	// Summary:
	//     Constructs an CXTAuxData object
	// ------------------------------------------
	CXTAuxData();

public:
	// -------------------------------------------------------------
	// Summary:
	//     Destroys an CXTAuxData object, handles cleanup and
	//     deallocation
	// -------------------------------------------------------------
	virtual ~CXTAuxData();

public:
	// ------------------------------------------------
	// Summary:
	//     Initializes cursors used by the library.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	// ------------------------------------------------
	BOOL LoadSysCursors();

	//-------------------------------------------------------------------------
	// Summary:
	//     This member function frees cursor resources.
	//-------------------------------------------------------------------------
	void FreeSysCursors();

	// ------------------------------------------------------------------------------
	// Summary:
	//     Initializes fonts used by the library.
	// Parameters:
	//     strHorzFaceName -  [in] Points to a NULL terminated font name that is used
	//                        to create the horizontal fonts used by the library.
	// strVertFaceName -  [in] Points to a NULL terminated font name that is used
	//                        to create the vertical fonts used by the library.
	// Remarks:
	//     Call this member function to update all of the fonts used by the
	//     library.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE if a font fails to
	//     load.
	// ------------------------------------------------------------------------------
	BOOL LoadSysFonts(CString strHorzFaceName=_T(""), CString strVertFaceName=_T(""));

	//-------------------------------------------------------------------------
	// Summary:
	//     This member function frees font resources.
	//-------------------------------------------------------------------------
	void FreeSysFonts();

	// -------------------------------------------------------------------
	// Summary:
	//     This member function updates system colors used by the library.
	// -------------------------------------------------------------------
	void UpdateSysColors();

	// --------------------------------------------------------------------
	// Summary:
	//     This member function updates system metrics used by the library.
	// --------------------------------------------------------------------
	void UpdateSysMetrics();

	//-------------------------------------------------------------------------
	// Summary:
	//     This member function is called to retrieve the version information
	//     for the common control dll (comctl32.dll).
	// Returns:
	//     A DWORD value.
	//-------------------------------------------------------------------------
	DWORD GetComCtlVersion();

	//-------------------------------------------------------------------------
	// Summary:
	//     This member function is called to check the current version of
	//     the common control dll (comctl32.dll).
	// Returns:
	//     Returns TRUE if the version is 4.72 or higher, otherwise FALSE.
	//-------------------------------------------------------------------------
	BOOL IsComCtlValid() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     This member function is called by the library to initialize resources.
	// Parameters:
	//     hInst - Instance handle for resources.
	//-------------------------------------------------------------------------
	void InitResources(HINSTANCE hInst);

	// ---------------------------------------------------------------------
	// Summary:
	//     Enables or Disables Tahoma font usage for the library.
	// Parameters:
	//     bOfficeFont -  [in] True to use the "Tahoma" font if found on the
	//                    operating system.
	// Remarks:
	//     Call this member function to use the Office font (Tahoma) if it
	//     exists on the operating system.
	// ---------------------------------------------------------------------
	void UseOfficeFont(BOOL bOfficeFont);

	// --------------------------------------------------------------------------------------
	// Summary:
	//     Determines if a font is installed.
	// Parameters:
	//     strFaceName -  [in] NULL terminated string that represents the font
	//                    name to search for.
	// Remarks:
	//     Call this member function to determine if the font specified by <i>strFaceName</i>
	//     has been installed on the operating system.
	// Returns:
	//     TRUE if the font is installed, otherwise FALSE.
	// --------------------------------------------------------------------------------------
	BOOL FontExists(CString& strFaceName);

	// -------------------------------------------------------------------------
	// Summary:
	//     Initializes fonts used by the library.
	// Parameters:
	//     lf -             [in] Reference to a CXTLogFont or LOGFONT structure.
	//     font -           [in] Reference to a CFont object.
	//     lfWeight -       [in] Optional, specifies font weight.
	//     lfUnderline -    [in] Optional, specifies font underline.
	//     lfOrientation -  [in] Optional, specifies font orientation.
	//     lfEscapement -   [in] Optional, specifies font escapement.
	// Remarks:
	//     Call this member function to create a CFont object from an
	//     existing CXTLogFont or LOGFONT structure. You can specify
	//     additional values to set that will override what is specified in
	//     the LOGFONT structure passed in.
	// Returns:
	//     TRUE if the font was successfully created, otherwise FALSE.
	// -------------------------------------------------------------------------
	BOOL CreateSysFont(const CXTLogFont& lf, CFont& font, long lfWeight=-1, char lfUnderline=-1, long lfOrientation=-1, long lfEscapement=-1);

	// ----------------------------------------------------------------------------------
	// Summary:
	//     Initializes fonts used by the library.
	// Parameters:
	//     pHorzFont -         [in] Points to a valid CFont object that is used to define
	//                         the fonts the library will use.
	//     pVertFont -         [in] Points to a valid CFont object that is used to define
	//                         the vertical fonts used by the library. If NULL, 'pFont'
	//                         will be used.
	//     lpszFaceName -      [in] Points to a NULL terminated font name that is used to
	//                         create the horizontal fonts used by the library.
	//     lpszVertFaceName -  [in] Points to a NULL terminated font name that is used to
	//                         create the vertical fonts used by the library.
	// Remarks:
	//     Call this member function to update fonts used by the library.
	//     This function will not update the fonts used for Icon Title,
	//     Marlett Icon or Small Captions. To change these fonts as well use
	//     the CreateSysFont method.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE if a font fails to
	//     load.
	// ----------------------------------------------------------------------------------
	BOOL SetGlobalFont(CFont* pHorzFont, CFont* pVertFont=NULL);
	BOOL SetGlobalFont(LPCTSTR lpszFaceName,LPCTSTR lpszVertFaceName=NULL); //<combine CXTAuxData::SetGlobalFont@CFont*@CFont*>

	// ---------------------------------------------------------------------
	// Summary:
	//     Retrieves the library version.
	// Parameters:
	//     bVerNumOnly -  [in] true to return the version number only, minus
	//                    "Xtreme Toolkit v".
	// Remarks:
	//     Call this member function to retrieve a CString object that
	//     represents the current version of the Xtreme Toolkit. The string
	//     returned is formatted like so: "Xtreme Toolkit v1.94".
	// Returns:
	//     A NULL terminated string that indicates the version of the Xtreme
	//     Toolkit.
	// ---------------------------------------------------------------------
	CString GetXTVersion(bool bVerNumOnly=false);

	// ------------------------------------------------------------------
	// Summary:
	//     Sets the path name for the settings INI file.
	// Parameters:
	//     lpszINIFullPath -  [in] Full path to the INI file.
	// Remarks:
	//     Call this member function to store registry settings in an INI
	//     file instead of the system's registry.
	// ------------------------------------------------------------------
	void SetINIFileName(LPCTSTR lpszINIFullPath);

	//-------------------------------------------------------------------------
	// Summary:
	//     Retrieves the path of the settings INI file.
	// Remarks:
	//     This member function will get the full path to the INI file
	//     used to store registry settings.
	// Returns:
	//     An LPCTSTR data type.
	//-------------------------------------------------------------------------
	LPCTSTR GetINIFileName();

private:
	static int CALLBACK EnumFontFamExProc(ENUMLOGFONTEX* pelf,
		NEWTEXTMETRICEX*, int, LPVOID pThis);

public:
	//=======================================================================
	//## (RGB) System colors used by Xtreme Toolkit
	//=======================================================================

	COLORREF clr3DFace;                   // An RGB value that represents the face color for three dimensional display elements.
	COLORREF clr3DShadow;                 // An RGB value that represents the shadow color for three dimensional display elements.
	COLORREF clr3DDkShadow;               // An RGB value that represents the dark shadow for three dimensional display elements.
	COLORREF clr3DHilight;                // An RGB value that represents the highlight color for three dimensional display elements.
	COLORREF clr3DLight;                  // An RGB value that represents the light color for three dimensional display elements.
	COLORREF clrBtnText;                  // An RGB value that represents the text color on push buttons.
	COLORREF clrGrayText;                 // An RGB value that represents the grayed (disabled) text.
	COLORREF clrHighlight;                // An RGB value that represents the item(s) selected in a control.
	COLORREF clrHighlightText;            // An RGB value that represents the text color of item(s) selected in a control.
	COLORREF clrMenu;                     // An RGB value that represents the menu background.
	COLORREF clrMenuText;                 // An RGB value that represents the text color in menus.
	COLORREF clrWindow;                   // An RGB value that represents the window background.
	COLORREF clrWindowFrame;              // An RGB value that represents the window frame.
	COLORREF clrWindowText;               // An RGB value that represents the text color in windows.
	COLORREF clrActiveCaption;            // An RGB value that represents the active window title bar.
	COLORREF clrInActiveCaption;          // An RGB value that represents the inactive window title bar.
	COLORREF clrGradActiveCapt;           // An RGB value that represents the gradient active title bar.
	COLORREF clrGradInActiveCapt;         // An RGB value that represents the gradient inactive title bar.
	COLORREF clrActiveCaptText;           // An RGB value that represents the active caption text.
	COLORREF clrInactiveCaptText;         // An RGB value that represents the inactive caption text.
	COLORREF clrXPBarFace;                // An RGB value that represents the XP toolbar background color.
	COLORREF clrXPHighlight;              // An RGB value that represents the XP menu item selected color.
	COLORREF clrXPHighlightBorder;        // An RGB value that represents the XP menu item selected border color.
	COLORREF clrXPHighlightPushed;        // An RGB value that represents the XP menu item pushed color.
	COLORREF clrXPIconShadow;             // An RGB value that represents the XP menu item icon shadow.
	COLORREF clrXPGrayText;               // An RGB value that represents the XP menu item disabled text color.
	COLORREF clrXPHighlightChecked;       // An RGB value that represents the XP menu item checked color.
	COLORREF clrXPHighlightCheckedBorder; // An RGB value that represents the XP menu item checked border color.
	COLORREF clrXPGripper;                // An RGB value that represents the XP toolbar gripper color.
	COLORREF clrXPSeparator;              // An RGB value that represents the XP toolbar separator color.
	COLORREF clrXPDisabled;               // An RGB value that represents the XP menu icon disabled color.
	COLORREF clrXPMenuTextBack;           // An RGB value that represents the XP menu item text background color.
	COLORREF clrXPMenuExpanded;           // An RGB value that represents the XP hidden menu commands background color.
	COLORREF clrXPMenuBorder;             // An RGB value that represents the XP menu border color.
	COLORREF clrXPMenuText;               // An RGB value that represents the XP menu item text color.
	COLORREF clrXPHighlightText;          // An RGB value that represents the XP menu item selected text color.
	COLORREF clrXPBarText;                // An RGB value that represents the XP toolbar text color.
	COLORREF clrXPBarTextPushed;          // An RGB value that represents the XP toolbar pushed text color.
	COLORREF clrXPTabInactiveBack;        // An RGB value that represents the XP inactive tab background color.
	COLORREF clrXPTabInactiveText;        // An RGB value that represents the XP inactive tab text color.

	//=======================================================================
	//## Cursors used by Xtreme Toolkit
	//=======================================================================

	HCURSOR hcurDragCopy;  // Drag copy cursor handle.
	HCURSOR hcurDragMove;  // Drag move cursor handle.
	HCURSOR hcurDragNone;  // Drag none cursor handle.
	HCURSOR hcurHand;      // Hand cursor handle.
	HCURSOR hcurHandNone;  // No Hand cursor handle.
	HCURSOR hcurHSplitBar; // Horizontal Splitter cursor handle.
	HCURSOR hcurVSplitBar; // Vertical Splitter cursor handle.
	HCURSOR hcurMove;      // 4 way move cursor handle.

	//=======================================================================
	//## System metrics
	//=======================================================================

	int cxSmIcon;   // cx small icon size (width).
	int cySmIcon;   // cy small icon size (height).
	int cxSize;     // Width, in pixels, of a button in a window's caption or title bar.
	int cySize;     // Height, in pixels, of a button in a window's caption or title bar.
	int cxHThumb;   // Width, in pixels, of the thumb box in a horizontal scroll bar.
	int cyVThumb;   // Height, in pixels, of the thumb box in a vertical scroll bar.
	int cyMenuItem; // Height, in pixels, of single-line menu bar.

	//=======================================================================
	//## Fonts used by Xtreme Toolkit
	//=======================================================================

	CFont font;              // Menu font.
	CFont fontBold;          // Menu bold font.
	CFont fontULine;         // Menu underlined font.
	CFont fontVert;          // Vertical menu font.
	CFont fontVertBold;      // Vertical menu bold font.
	CFont fontIconTitle;     // Icon title font.
	CFont fontIconTitleBold; // Icon title bold font.
	CFont fontMarlettIcon;   // Marlett icon font.
	CFont fontStatus;        // Status bar font.
	CFont fontSmCaption;     // Small caption font.

	//=======================================================================
	//## Flags used by Xtreme Toolkit
	//=======================================================================

	BOOL bWin95;           // TRUE if Windows 95.
	BOOL bWin98;           // TRUE if Windows 98.
	BOOL bWinNT;           // TRUE if Windows NT.
	BOOL bWin2K;           // TRUE if Windows 2000.
	BOOL bWinXP;           // TRUE if Windows XP.
	BOOL bUseOfficeFont;   // TRUE to use the Tahoma font if found, good for Office style applications by default.
	BOOL bUseSolidShadows; // TRUE to use solid shadows for operating systems that do not supported layered windows.
	BOOL bMenuShadows;     // TRUE to use shadows under the menus.
	CXTAuxDataTheme bXPMode;          // TRUE to use XP style menus.

	//=======================================================================
	//## Miscellaneous settings
	//=======================================================================

	int       iComCtlVersion;  // Common control dll (comctl32.dll) version information.
	CString&  strINIFileName;  // Name of an INI file for registry settings.
	HINSTANCE hInstance;       // Instance handle for resources.

	friend _XT_EXT_CLASS CXTAuxData& AFXAPI XTAuxData();
};

//////////////////////////////////////////////////////////////////////

#endif // #if !defined(__XTGLOBALS_H__)
