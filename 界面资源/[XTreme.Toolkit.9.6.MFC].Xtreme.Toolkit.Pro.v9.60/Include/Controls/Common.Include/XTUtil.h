// XTUtil.h: interface for utility classes.
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
#if !defined(__XTUTIL_H__)
#define __XTUTIL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     CXTIconHandle is a stand alone helper class.  It is used to
//     automatically destroy dynamically created hIcon handles.
//===========================================================================
class _XT_EXT_CLASS CXTIconHandle
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTIconHandle object.
	//-----------------------------------------------------------------------
	CXTIconHandle();

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTIconHandle object.
	// Parameters:
	//     hIcon - Handle to a dynamically created icon.
	//-----------------------------------------------------------------------
	CXTIconHandle(HICON hIcon);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTIconHandle object, handles cleanup and de-
	//     allocation.
	//-----------------------------------------------------------------------
	virtual ~CXTIconHandle();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This operator is used to retrieve a handle to the icon
	//     associated with the CXTIconHandle object.
	// Returns:
	//     An HICON handle to the icon.
	//-----------------------------------------------------------------------
	operator HICON() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This operator is used to initialize the icon associated with
	//     the CXTIconHandle object.
	// Parameters:
	//     hIcon - Handle to a dynamically created icon.
	// Returns:
	//     A reference to a CXTIconHandle object.
	//-----------------------------------------------------------------------
	CXTIconHandle& operator =(HICON hIcon);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function gets the extent of an icon.
	// Parameters:
	//     hIcon - Icon handle whose dimensions are to be retrieved.
	// Returns:
	//     A CSize object.
	//-----------------------------------------------------------------------
	static CSize GetExtent(HICON hIcon);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function gets the extent of an icon attached to
	//     this object.
	// Returns:
	//     A CSize object.
	//-----------------------------------------------------------------------
	CSize GetExtent() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function scales an icon to fit into a rectangle.
	//     The width and height ration is retained as much as possible.
	//     The caller assumes ownership of the returned icon handle.
	// Parameters:
	//     hIcon         - Icon to be fitted.
	//     desiredExtent - Desired icon extent.
	// Returns:
	//     An icon handle.
	//-----------------------------------------------------------------------
	static HICON ScaleToFit(HICON hIcon, CSize desiredExtent);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function scales an icon to fit into a rectangle.
	//     The width and height ration is retained as much as possible.
	//     The caller assumes  ownership of the returned icon handle.
	// Parameters:
	//     desiredExtent - Desired icon extent.
	// Returns:
	//     An icon handle.
	//-----------------------------------------------------------------------
	HICON ScaleToFit(CSize desiredExtent) const;

protected:
	HICON m_hIcon;  // Handle to a dynamically created icon.
};


AFX_INLINE CXTIconHandle::operator HICON() const {
	return m_hIcon;
}


//===========================================================================
// Summary:
//     CXTLoadLibrary is a stand alone utility class.  It is used to
//     load a module (DLL) and free the instance handle upon
//     destruction.  It wraps the LoadLibrary and the FreeLibrary API's.
//===========================================================================
class _XT_EXT_CLASS CXTLoadLibrary
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTLoadLibrary object.
	// Parameters:
	//     lpszModule - Pointer to a null-terminated string that names
	//                  the .DLL file. The name specified is the
	//                  filename of the module and is not related to the
	//                  name stored in the library module itself, as
	//                  specified by the LIBRARY keyword in the module-
	//                  definition (.DEF) file.
	//                  If the string specifies a path but the file does
	//                  not exist in the specified directory, the
	//                  function fails.  When specifying a path, be sure
	//                  to use backslashes (\), not forward slashes (/).
	//-----------------------------------------------------------------------
	CXTLoadLibrary(LPCTSTR lpszModule=NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTLoadLibrary object, handles cleanup and
	//     deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTLoadLibrary();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to load the library specified
	//     by 'lpszModule'.  Once the library is loaded, you can
	//     retrieve the instance handle by using the HINSTANCE operator.
	// Parameters:
	//     lpszModule - Pointer to a null-terminated string that names
	//                  the .DLL file. The name specified is the
	//                  filename of the module and is not related to the
	//                  name stored in the library module itself, as
	//                  specified by the LIBRARY keyword in the module-
	//                  definition (.DEF) file.
	//                  If the string specifies a path but the file does
	//                  not exist in the specified directory, the
	//                  function fails.  When specifying a path, be sure
	//                  to use backslashes (\), not forward slashes (/).
	//-----------------------------------------------------------------------
	void LoadLibrary(LPCTSTR lpszModule);

	//-----------------------------------------------------------------------
	// Summary:
	//     This overloaded operator returns a handle to the module
	//     indicating success. NULL indicates failure.
	// Returns:
	//     A handle to the module if successful, otherwise returns NULL.
	//-----------------------------------------------------------------------
	operator HINSTANCE() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this member function to return the version number of the
	//     module attached to this CLoadLibrary object. The high-order
	//     word of the return value represents the major version number
	//     and the low-order word of the returned value represents the
	//     minor version number.
	// Returns:
	//     A DWORD value if successful, otherwise 0L.
	//-----------------------------------------------------------------------
	DWORD GetModuleVersion();

protected:
	HINSTANCE m_hInstance;  // A handle to the module indicates success.

};

AFX_INLINE CXTLoadLibrary::operator HINSTANCE() const {
	return m_hInstance;
}

//===========================================================================
// Summary:
//     CXTNoFlickerWnd is a TBase derived general purpose template
//     helper class. CXTNoFlickerWnd class is used for drawing flicker
//     free controls. To use, instantiate the template using any CWnd
//     derived class. For example, to create a tab control that is
//     flicker free you would use the following declaration:
// Example: Here is an example of how you can use CXTNoFlickerWnd to create a
//          flicker free object.
//          <code>CXTNoFlickerWnd <CTabCtrl> m_tabCtrl;</code>
//===========================================================================
template <class TBase>
class CXTNoFlickerWnd : public TBase
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTNoFlickerWnd object.
	//-----------------------------------------------------------------------
	CXTNoFlickerWnd()
	{
		m_crBack = ::GetSysColor(COLOR_3DFACE);
	}

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to set the background fill
	//     color for the flicker free control.
	// Parameters:
	//     crBack - An RGB value.
	//-----------------------------------------------------------------------
	void SetBackColor(COLORREF crBack)
	{
		m_crBack = crBack;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to retrieve the background
	//     fill color for the flicker free control.
	// Returns:
	//     An RGB value.
	//-----------------------------------------------------------------------
	COLORREF GetBackColor()
	{
		return m_crBack;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This method provides a CE procedure (WindowProc) for a CWnd
	//     object.  It dispatches messages through the window message
	//     map. The return value depends on the message.
	// Parameters:
	//     message - Specifies the Windows message to be processed.
	//     wParam  - Provides additional information used in processing
	//               the message. The parameter value depends on the
	//               message.
	//     lParam  - Provides additional information used in processing
	//               the message. The parameter value depends on the
	//               message.
	// Returns:
	//     An LRESULT object.
	//-----------------------------------------------------------------------
	virtual LRESULT WindowProc(UINT message,WPARAM wParam,LPARAM lParam)
	{
		switch (message)
		{
		case WM_PAINT:
			{
				CPaintDC dc(this);

				// Get the client rect, and paint to a memory device context.
				// This will help reduce screen flicker.  Pass the memory
				// device context to the default window procedure to do
				// default painting.

				CRect r;
				GetClientRect(&r);
				CXTMemDC memDC(&dc, r, m_crBack);

				return TBase::DefWindowProc(WM_PAINT,
					(WPARAM)memDC.m_hDC, 0);
			}

		case WM_ERASEBKGND:
			{
				return TRUE;
			}
		}

		return TBase::WindowProc(message, wParam, lParam);
	}

protected:
	COLORREF m_crBack; // An RGB value.

};

//===========================================================================
// Summary:
//     CXTSplitPath conveniently wraps the _splitpath API for easier
//     access to path components such as file name and extension.
//===========================================================================
class _XT_EXT_CLASS CXTSplitPath
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTSplitPath object and breaks a path into its
	//     four components.
	// Parameters:
	//     lpszPathBuffer - A NULL terminated string that represents the
	//                      path you
	//-----------------------------------------------------------------------
	CXTSplitPath(LPCTSTR lpszPathBuffer=NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTSplitPath object, handles cleanup and de-
	//     allocation.
	//-----------------------------------------------------------------------
	virtual ~CXTSplitPath();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call SplitPath to break a path into its four components.
	// Parameters:
	//     lpszPathBuffer - A NULL terminated string that represents the
	//                      path you
	//-----------------------------------------------------------------------
	void SplitPath(LPCTSTR lpszPathBuffer);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call GetDrive() to return the drive letter, followed by a
	//     colon (:) for the path split.
	// Returns:
	//     A NULL terminated string.
	//-----------------------------------------------------------------------
	CString GetDrive() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call GetDir() to return the directory path, including
	//     trailing slash. Forward slashes ( / ), backslashes ( \ ), or
	//     both may be used.
	// Returns:
	//     A NULL terminated string.
	//-----------------------------------------------------------------------
	CString GetDir() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call GetFName() to return the base filename without extension
	// Returns:
	//     A NULL terminated string.
	//-----------------------------------------------------------------------
	CString GetFName() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call GetExt() to return the filename extension, including
	//     leading period (.)
	// Returns:
	//     A NULL terminated string.
	//-----------------------------------------------------------------------
	CString GetExt() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call GetFullPath() to return the full path that was split.
	// Returns:
	//     A NULL terminated string.
	//-----------------------------------------------------------------------
	CString GetFullPath() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call GetFullName() to return the file name plus extension
	//     only for the path that was split.
	// Returns:
	//     A NULL terminated string.
	//-----------------------------------------------------------------------
	CString GetFullName() const;

protected:
	TCHAR m_szDrive[_MAX_DRIVE]; // Optional drive letter, followed by a colon (:)
	TCHAR m_szDir[_MAX_DIR];     // Optional directory path, including trailing slash. Forward slashes ( / ), backslashes ( \ ), or both may be used.
	TCHAR m_szFName[_MAX_FNAME]; // Base filename (no extension)
	TCHAR m_szExt[_MAX_EXT];     // Optional filename extension, including leading period (.)
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(__XTUTIL_H__)
