// XTListCtrlView.h interface for the CXTListView and CXTListCtrlView class.
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
#if !defined(__XTLISTVIEW_H__)
#define __XTLISTVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "XTListBase.h"

DECLATE_LIST_BASE(CXTListViewBase, CXTListViewHelper, CXTListBase)
DECLATE_LIST_BASE(CXTListCtrlBase, CXTListCtrlHelper, CXTListBase)

//===========================================================================
// Summary:
//     CXTListView is a multiple inheritance class derived from CListView and
//     CXTListBase. This class implements flat header and generic sorting
//     routines.
// See Also: CXTListBase
//===========================================================================
class _XT_EXT_CLASS CXTListView : public CXTListViewBase
{
	DECLARE_DYNCREATE(CXTListView)

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTListView object
	//-----------------------------------------------------------------------
	CXTListView();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTListView object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTListView();

protected:
	DECLARE_MESSAGE_MAP()
};

//---------------------------------------------------------------------------
// Summary:
//     CXTListCtrl is a multiple inheritance class derived from CListCtrl and
//     CXTListBase. This class implements flat header and generic sorting
//     routines. See CXTListBase for additional functionality.
// See Also: CXTListBase
//---------------------------------------------------------------------------
class _XT_EXT_CLASS CXTListCtrl : public CXTListCtrlBase
{
	DECLARE_DYNAMIC(CXTListCtrl)

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTListCtrl object
	//-----------------------------------------------------------------------
	CXTListCtrl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTListCtrl object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTListCtrl();

protected:
	DECLARE_MESSAGE_MAP()
};

#endif // #if !defined(__XTLISTVIEW_H__)
