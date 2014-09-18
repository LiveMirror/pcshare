// XTFunctions.h : global functions.
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
#if !defined(__XTFUNCTIONS_H__)
#define __XTFUNCTIONS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#if defined(XT_INIT_BY_REGULAR_DLL)

// ------------------------------------------------------------------------------
// Summary:
//     This member function will initialize the resources for the Xtreme Toolkit.
//     If using the Xtreme Toolkit as an extension DLL within a regular
//     DLL, add the following two lines to your stdafx.h file. This will
//     give you access to the exported function InitXtremeExtDll():
// Example:
// <code>
// #define XT_INIT_BY_REGULAR_DLL
// #include "XTToolkit.h"
// </code>
//
//     You will then need to add XT_INIT_BY_REGULAR_DLL to the Xtreme Toolkit
//     preprocessor definitions and rebuild it. After you have done this,
//     locate your CWinApp::InitInstance() method for your regular DLL
//     and make the following call:
//
// <code>
// XTFuncInitDLL();
// </code>
//
// This will initialize the resources for the Xtreme Toolkit.
// ------------------------------------------------------------------------------
_XT_EXT_CLASS void AFXAPI XTFuncInitDLL();

#endif//#if defined(XT_INIT_BY_REGULAR_DLL)

// --------------------------------------------------------------------
// Summary:
//     The XTFuncChangeWindowFont function will set the font for
//     the window specified by <i>pWnd</i> and all of the child windows
//     \owned by <i>pWnd</i>.
// Parameters:
//     pWnd -   Pointer to a valid CWnd object.
//     pFont -  Pointer to the new font to set for the window.
// --------------------------------------------------------------------
_XT_EXT_CLASS void AFXAPI XTFuncChangeWindowFont(CWnd* pWnd,CFont* pFont);

// ----------------------------------------------------------------------------
// Summary:
//     The XTFuncDrawEmbossed function will draw an embossed icon
//     \onto the specified device context from the image list <i>imageList</i>.
//     It is typically used by toolbars and menus to draw a disabled icon
//     in color.
// Parameters:
//     pDC -        Pointer to the current device context.
//     imageList -  Address of an image list.
//     nIndex -     Index of the image in the image list.
//     point -      XY location of where to draw the icon.
//     bInColor -   TRUE to draw the item in color, otherwise the icon will
//                  be drawn with the default disabled look.
// ----------------------------------------------------------------------------
_XT_EXT_CLASS void AFXAPI XTFuncDrawEmbossed(CDC* pDC,CImageList& imageList,int nIndex,CPoint point,BOOL bInColor);

// --------------------------------------------------------------------
// Summary:
//     Renders a shadow onto the specified device context.
// Parameters:
//     pDC -   Points to the current device context.
//     rect -  Size of the area to draw.
// Remarks:
//     The XTFuncDrawShadedRect function will render a shaded or
//     'shadow' rectangle onto the the device context specified by pDC.
// --------------------------------------------------------------------
_XT_EXT_CLASS void AFXAPI XTFuncDrawShadedRect(CDC *pDC,CRect& rect);

// --------------------------------------------------------------------
// Summary:
//     The XTFuncLoadSysColorBitmap function loads a bitmap from
//     resource and translates the bitmap color such that they suit
//     requirements for a toolbar bitmap.
// Parameters:
//     hInst -  Instance handle of the module that has the required
//              resource.
//     hRsrc -  Bitmap resource handle.
//     bMono -  Tells if monochrome conversion shall be applied.
// --------------------------------------------------------------------
_XT_EXT_CLASS HBITMAP AFXAPI XTFuncLoadSysColorBitmap(HINSTANCE hInst,HRSRC hRsrc,BOOL bMono = FALSE);

// ------------------------------------------------------------------------------
// Summary:
//     The XTFuncChildWindowFromPoint function will retrieve an HWND
//     handle for the child window (if any) located directly under the
//     cursor position specified by <i>point</i>.
// Parameters:
//     hWnd -   HWND handle of the parent window to find the child for.
//     point -  Current cursor position.
// Returns:
//     An HWND handle for the child window at <i>point</i>.
// ------------------------------------------------------------------------------
_XT_EXT_CLASS HWND AFXAPI XTFuncChildWindowFromPoint(HWND hWnd,POINT point);

// ----------------------------------------------------------------------------
// Summary:
//     Removes the mnemonic '&' characters from a string.
// Parameters:
//     strText -  A reference to a CString object.
// Remarks:
//     The XTFuncStripMnemonics function will search a string and
//     remove the mnemonic '&' characters found. It will ignore any
//     double '&&' found assuming that it was meant as as a literal
//     ampersand.
// Example:
//     The following example demonstrates using the XTFuncStripMnemonics
//     function.
// <code>
// CString str = _T("This a &Mnemonic && this is not");
// XTFuncStripMnemonics(str);
// // String with no mnemonics "This a Mnemonic && this is not".
// </code>
// ----------------------------------------------------------------------------
_XT_EXT_CLASS void AFXAPI XTFuncStripMnemonics(CString& strText);

// -------------------------------------------------------------------------------
// Summary:
//     The XTFuncPathExists function searches a directory for a
//     file or subdirectory whose name matches the specified name.
// Parameters:
//     lpszFileName -  [in] Pointer to a null\-terminated string that specifies a
//                     valid directory or path and file name, which can contain
//                     wild card characters (* and ?). If the string ends with a
//                     wild card, a period, or a directory name, the user must have
//                     access to the root and all subdirectories on the path.
// Returns:
//     TRUE if the file or subdirectory exists, otherwise FALSE.
// -------------------------------------------------------------------------------
_XT_EXT_CLASS BOOL AFXAPI XTFuncPathExists(LPCTSTR lpszFileName);

// -------------------------------------------------------------------------------------------
// Summary:
//     Parses a path for the next path component.
// Parameters:
//     pszPath -  Pointer to a NULL\-terminated string with the path. Paths are
//                delimited by backslashes or by the NULL at the end of the
//                path.
// Remarks:
//     This member function parses a path for the next path component,
//     not dependent on shell32.dll, and is a replacement for the windows
//     API PathFindNextComponent.
// Returns:
//     A pointer to a NULL-terminated string with the next path component
//     if successful, or NULL otherwise.
// Example:
//     The following example demonstrates using the XTFuncPathFindNextComponent function.
// <code>
// // Path to find the next part.
// char buffer_1[ ] = "c:\\\\path1\\\\path2\\\\test";
// char *lpStr1;
// lpStr1 = buffer_1;
//
// cout \<\< "Search a path for the next path component "
//      \<\< "after the root " \<\< lpStr1 \<\< endl;
// cout \<\< "Return the next path component: \\""
//      \<\<  XTFuncPathFindNextComponent(lpStr1) \<\< "\\"" \<\< endl;
//
// cout \<\< "\\nSearch a path for the next path component "
//      \<\< "after the root \\"c:\\\\path1\\\\path2\\"" \<\< endl;
// cout \<\< "Return the next path component: \\""
//      \<\<  XTFuncPathFindNextComponent("c:\\\\path1\\\\path2") \<\< "\\"" \<\< endl;
//
// cout \<\< "\\nSearch a path for the next path component "
//      \<\< "after the root \\"c:\\\\path1\\"" \<\< endl;
// cout \<\< "Return the next path component: \\""
//      \<\<  XTFuncPathFindNextComponent("c:\\\\path1") \<\< "\\"" \<\< endl;
//
// \OUTPUT:
// ===========
// Search a path for the next path component after the root c:\\path1\\path2\\test
// Return the next path component: "path1\\path2\\test"
//
// Search a path for the next path component after the root "c:\\path1\\path2"
// Return the next path component: "path1\\path2"
//
// Search a path for the next path component after the root "c:\\path1"
// Return the next path component: "path1"
// </code>
// -------------------------------------------------------------------------------------------
_XT_EXT_CLASS LPTSTR AFXAPI XTFuncPathFindNextComponent(LPTSTR pszPath);

// ----------------------------------------------------------------------------
// Summary:
//     Creates a view based on a CRuntimeClass object.
// Parameters:
//     pParentWnd -  Points to the parent of the view to be created. The parent
//                   must be valid.
//     pViewClass -  Specifies the CRuntimeClass of the new view.
//     pDocument -   CDocument associated with the view. It can be NULL.
//     pContext -    Create context for the view. It can be NULL.
//     dwStyle -     Default style for the view.
//     pOwnerWnd -   Owner of the view. If NULL, 'pParentWnd' is used.
//     nID -         Control ID of the view.
// Remarks:
//     The XTFuncCreateView function will create a view based on
//     the CRuntimeClass <i>pViewClass</i>.
// Returns:
//     A CWnd* pointer to the newly created view if successful, otherwise
//     returns NULL.
// Example:
// <code>
// CView* pView = (CView*)XTFuncCreateView(this, RUNTIME_CLASS(CMyView));
// ASSERT_VALID(pView);
// </code>
// ----------------------------------------------------------------------------
_XT_EXT_CLASS CWnd* AFXAPI XTFuncCreateView(CWnd* pParentWnd,CRuntimeClass *pViewClass, CDocument *pDocument=NULL,CCreateContext *pContext=NULL,DWORD dwStyle=AFX_WS_DEFAULT_VIEW,CWnd* pOwnerWnd=NULL,UINT nID=AFX_IDW_PANE_FIRST);


// ----------------------------------------------------------------------------------------------
// Summary:
//     Call this member function to display a context menu that is
//     compatible with both pro and standard versions.
// Parameters:
//     pPopup -     Pointer to a valid CMenu object.
//     nFlags -     Specifies a screen\-position flag and a mouse\-button flag. The
//                  screen\-position flag can be one of the following\:<p/>TPM_CENTERALIGN
//                  Centers the pop\-up menu horizontally relative to the coordinate
//                  specified by x.<p/>TPM_LEFTALIGN Positions the pop\-up
//                  menu so that its left side is aligned with the coordinate
//                  specified by x.<p/>TPM_RIGHTALIGN Positions the pop\-up
//                  menu so that its right side is aligned with the coordinate
//                  specified by x.<p/>The mouse\-button flag can be either of the
//                  following\:<p/>TPM_LEFTBUTTON Causes the pop\-up menu to
//                  track the left mouse button.<p/>TPM_RIGHTBUTTON Causes
//                  the pop\-up menu to track the right mouse button.
//     x -          Specifies the horizontal position in screen coordinates of the
//                  pop\-up menu. Depending on the value of the nFlags parameter,
//                  the menu can be left\-aligned, right\-aligned, or centered
//                  relative to this position.
//     y -          Specifies the vertical position in screen coordinates of the top
//                  of the menu on the screen.
//     pWnd -       Identifies the window that owns the pop\-up menu. This window
//                  receives all WM_COMMAND messages from the menu. In
//                  Windows versions 3.1 and later, the window does not receive WM_COMMAND
//                  messages until TrackPopupMenu returns. In Windows 3.0,
//                  the window receives WM_COMMAND messages before TrackPopupMenu
//                  \returns.
//     nIDBitmap -  Toolbar resource ID that contains images to be displayed with
//                  menu commands.
// Remarks:
//     A floating pop-up menu can appear anywhere on the screen.
// See Also:
//     CXTPCommandBars::TrackPopupMenu
// Example:
//     The following example demonstrates using XTFuncContextMenu.
// <code>
// CPoint pt = point;
// ClientToScreen(&pt);
//
// CMenu menu;
// VERIFY(XTPResourceManager()-\>LoadMenu(&menu, XT_IDM_POPUP));
//
// CMenu* pPopup = menu.GetSubMenu(0);
// ASSERT(pPopup != NULL);
// CWnd* pWndPopupOwner = this;
//
// XTFuncContextMenu(pPopup, TPM_LEFTALIGN | TPM_RIGHTBUTTON,
//     pt.x, pt.y, pWndPopupOwner, XT_IDR_TBAR_OUT);
// </code>
// ----------------------------------------------------------------------------------------------
_XT_EXT_CLASS BOOL AFXAPI XTFuncContextMenu(CMenu* pPopup, UINT nFlags, int x, int y, CWnd* pWnd, int nIDBitmap);

//////////////////////////////////////////////////////////////////////

#endif // #if !defined(__XTFUNCTIONS_H__)
