// XTPMacros.h : common macros
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
#if !defined(__XTPMACROS_H__)
#define __XTPMACROS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//-----------------------------------------------------------------------------
// Summary:
//     Frees dynamically allocated memory.
// Parameters:
//     ptr - Points to the memory to free.
// Remarks:
//     This macro will first check to see if the block of memory specified by
//     <i>ptr</i> is NULL. If <i>ptr</i> is not NULL, SAFE_DELETE will deallocate
//     the block of memory and set its value to NULL. The pointer argument
//     must refer to a block of memory previously allocated for an object
//     created with the new operator.
// Example:
// <code>
// CName* pName = new CName;          // Allocate memory for the object
// pName->SetName("Firstname", "Lastname");
// SAFE_DELETE(pName);                // Deallocate memory for the object
// ASSERT(pName == NULL);
// </code>
//-----------------------------------------------------------------------------
#define SAFE_DELETE(ptr)
//{{AFX_CODEJOCK_PRIVATE
#undef SAFE_DELETE
#define SAFE_DELETE(ptr) \
	if(ptr) { delete ptr; ptr = NULL; }
//}}AFX_CODEJOCK_PRIVATE

//-----------------------------------------------------------------------------
// Summary:
//     Frees dynamically allocated memory for an array.
// Parameters:
//     ptr - Points to the memory array to free.
// Remarks:
//     This macro will first check to see if the block of memory specified by
//     <i>ptr</i> is NULL. If <i>ptr</i> is not NULL, SAFE_DELETE_AR will deallocate
//     the block of memory and set its value to NULL. The pointer argument
//     must refer to a block of memory previously allocated for an object
//     created with the new operator.
// Example:
// <code>
// char* pCharArray = new char[256];  // Allocate memory for the array
// strcpy(pCharArray, "Array of characters");
// SAFE_DELETE_AR(pCharArray);        // Deallocate memory for the array
// ASSERT(pCharArray == NULL);
// </code>
//-----------------------------------------------------------------------------
#define SAFE_DELETE_AR(ptr)
//{{AFX_CODEJOCK_PRIVATE
#undef SAFE_DELETE_AR
#define SAFE_DELETE_AR(ptr) \
	if(ptr) { delete [] ptr; ptr = NULL; }
//}}AFX_CODEJOCK_PRIVATE

//-----------------------------------------------------------------------------
// Summary:
//     Decrements the reference count for the specified object.
// Parameters:
//     comPointer - Points to the COM object to release.
// Remarks:
//     This macro will first check to see if the COM object specified by <i>comPointer</i>
//     is NULL. If the object is not NULL, the macro calls IUnknown::Release
//     COM object specified by <i>comPointer</i> and set its value to NULL.
// See Also:
//     IUnknown::Release
//-----------------------------------------------------------------------------
#define SAFE_RELEASE(comPointer)
//{{AFX_CODEJOCK_PRIVATE
#undef SAFE_RELEASE
#define SAFE_RELEASE(comPointer) \
	if(comPointer) { (comPointer)->Release(); (comPointer)=NULL; }
//}}AFX_CODEJOCK_PRIVATE

//-----------------------------------------------------------------------------
// Summary:
//     Checks object pointer and if not NULL calls a specified function.
// Parameters:
//     classPointer - Pointer to the class that contains the function to call.
//     functionName - Name of the function to call.
// Remarks:
//     This macro will first check to see if the class specified by <i>classPointer</i>
//     is NULL. If the class is not NULL, the macro calls the function specified
//     by <i>functionName</i>.
// Example:
// <code>
//  SAFE_CALLPTR(m_pItem, OnConstraintsChanged());
// </code>
//-----------------------------------------------------------------------------
#define SAFE_CALLPTR(classPointer, functionName)
//{{AFX_CODEJOCK_PRIVATE
#undef SAFE_CALLPTR
#define SAFE_CALLPTR(classPointer, functionName) \
	if (classPointer) classPointer->functionName
//}}AFX_CODEJOCK_PRIVATE

//{{AFX_CODEJOCK_PRIVATE

//-----------------------------------------------------------------------------
// Uncomment this definition if your application uses a resource dll, and add
// #include "XTResource.rc" to your resource dll's .rc2 file.  You will have to
// call XTAuxData().InitResources(HINSTANCE), once you have a resource handle, to
// initialize the Xtreme Toolkit resources.  You will need to rebuild the
// library after you have done this.
//-----------------------------------------------------------------------------
//#define _XT_USES_RESOURCE_DLL

//}}AFX_CODEJOCK_PRIVATE

#ifdef _DEBUG
#ifndef _AFX_NO_DEBUG_CRT

//-----------------------------------------------------------------------------
// Summary:
//     XT_ASSERT_MSG is similar to ASSERT, except that it allows a custom
//     message to be specified.
// Parameters:
//     exp - Specifies an expression (including pointer values)
//           that evaluates to nonzero or 0.
//     msg - Specifies a NULL terminated user defined string of
//           the message to display to the user.
// Remarks:
//     Evaluates its argument. If the result is 0, the macro prints a
//     diagnostic message and aborts the program. If the condition is
//     nonzero, it does nothing.<p/>
//
//     The diagnostic message has the form "assertion failed in file
//     <i>name</i> in line <i>num</i>.<p/>
//
//     where <i>name</i> is the name of the source file, and <i>num</i>
//     is the line number of the assertion that failed in the source file.<p/>
//
//     In the Release version of MFC, XT_ASSERT_MSG does not evaluate the
//     expression and thus will not interrupt the program. If the expression
//     must be evaluated regardless of environment, use the VERIFY macro in
//     place of XT_ASSERT_MSG.<p/>
//
//     <b>Note</b> This function is available only in the Debug version of
//     MFC.
// Example:
// <code>
// // example for XT_ASSERT_MSG
// CAge* pAge = new CAge( 21 ); // CAge is derived from CObject.
//
// // Terminates program only if pAge is NOT a CAge*.
// XT_ASSERT_MSG( pAge!= NULL, _T("Failed to allocate memory.") );
// XT_ASSERT_MSG( pAge->IsKindOf( RUNTIME_CLASS( CAge ) ), _T("This is not a CAge object.") );
// </code>
// See Also:
//     ASSERT, VERIFY
//-----------------------------------------------------------------------------
#define XT_ASSERT_MSG(exp, msg)
//{{AFX_CODEJOCK_PRIVATE
#undef XT_ASSERT_MSG
#define XT_ASSERT_MSG(exp, msg) \
{ \
	if ( !(exp) && (_CrtDbgReport( _CRT_ASSERT, __FILE__, __LINE__, NULL, "\n-----------------------\n" msg "\n-----------------------" ) ) ) \
		AfxDebugBreak(); \
} \

#else
#define XT_ASSERT_MSG(exp, msg) (void)( (exp) || (_assert("\n-----------------------\n" msg "\n-----------------------", __FILE__, __LINE__), 0) )
#endif//_AFX_NO_DEBUG_CRT

#else
#define XT_ASSERT_MSG(exp, msg) ((void)0)
#endif//_DEBUG
//}}AFX_CODEJOCK_PRIVATE

//{{AFX_CODEJOCK_PRIVATE
#ifdef _UNICODE
#ifndef UNICODE
#define UNICODE
#endif
#endif

#define __STR2__(x) #x
#define __STR1__(x) __STR2__(x)
#define __LOC__ __FILE__ "("__STR1__(__LINE__)"): warning : "

#ifdef _AFXDLL
	#define SAFE_MANAGE_STATE(s) AFX_MANAGE_STATE(s)
#else
	#define SAFE_MANAGE_STATE(s)
#endif

#ifndef AFX_INLINE
#define AFX_INLINE inline
#endif

#ifndef CBRS_GRIPPER
#define CBRS_GRIPPER                    0x00400000L
#endif

#ifndef WS_EX_LAYOUTRTL
#define WS_EX_LAYOUTRTL                 0x00400000L
#endif

#ifndef _countof
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif

#if (_MSC_VER <= 1200) && !defined(_WIN64)

// IA64 Macros:
#ifndef DWORD_PTR
#define DWORD_PTR DWORD
#endif

#ifndef UINT_PTR
#define UINT_PTR UINT
#endif

#ifndef INT_PTR
#define INT_PTR int
#endif

#ifndef ULONG_PTR
#define ULONG_PTR ULONG
#endif

#ifndef LONG_PTR
#define LONG_PTR long
#endif

#ifndef SetWindowLongPtr
#define SetWindowLongPtr SetWindowLong
#endif

#ifndef GetWindowLongPtr
#define GetWindowLongPtr GetWindowLong
#endif

#ifndef GetClassLongPtr
#define GetClassLongPtr GetClassLong
#endif

#endif
//}}AFX_CODEJOCK_PRIVATE

//-----------------------------------------------------------------------------

#endif // #if !defined(__XTPMACROS_H__)
