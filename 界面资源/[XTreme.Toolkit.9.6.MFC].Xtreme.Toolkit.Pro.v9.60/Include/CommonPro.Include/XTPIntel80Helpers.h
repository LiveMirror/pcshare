// XTPIntel80Helpers.h : Intel(R) C++ 8.0 helpers
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
#if !defined(__XTPINTEL80HELPERS_H__)
#define __XTPINTEL80HELPERS_H__

#if (_MSC_VER >= 1000)
#pragma once
#endif // _MSC_VER >= 1000

#ifdef __INTEL_COMPILER

#define USES_PROTECTED_ACCESS(theFiend, theClass, theType, theMember)\
class CAccess##theClass : public theClass\
{ public: inline theType Get##theMember() {return theMember;}\
	friend class theFiend;\
};

#define PROTECTED_ACCESS(theClass, thePointer, theMember)\
(((CAccess##theClass*)thePointer)->Get##theMember())

#define PROTECTED_DEFWINDOWPROC_CALL(theFiend, thePonter, m, w, l)\
class CAccessWnd : public CWnd\
{\
	public: inline void DefWindowProcBase(UINT message, WPARAM wParam, LPARAM lParam) {\
		CWnd::DefWindowProc(message, wParam, lParam);\
	}\
	friend class theFiend;\
};\
((CAccessWnd*)(thePonter))->DefWindowProcBase(m, w, l);




#else

#define USES_PROTECTED_ACCESS(theFiend, theClass, theType, theMember)\
class CAccess##theClass : public theClass\
{\
	friend class theFiend;\
};

#define PROTECTED_ACCESS(theClass, thePointer, theMember)\
(((CAccess##theClass*)thePointer)->theMember)

#define PROTECTED_DEFWINDOWPROC_CALL(theFiend, thePonter, m, w, l)\
class CAccessWnd : public CWnd\
{\
	friend class theFiend;\
};\
((CAccessWnd*)thePonter)->DefWindowProc(m, w, l);

#endif

//}}AFX_CODEJOCK_PRIVATE

//////////////////////////////////////////////////////////////////////

#endif // #if !defined(__XTPINTEL80HELPERS_H__)
