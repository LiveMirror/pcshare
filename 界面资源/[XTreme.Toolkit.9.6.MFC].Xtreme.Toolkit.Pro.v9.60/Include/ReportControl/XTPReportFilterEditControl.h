// XTPReportFilterEditControl.h: interface for the CXTPReportFilterEditControl class.
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
#if !defined(__XTPREPORTFILTEREDITCONTROL_H__)
#define __XTPREPORTFILTEREDITCONTROL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XTPReportDefines.h"

//===========================================================================
// Summary:
//     The CXTPReportFilterEditControl class provides the functionality of the
//     filter string edit control, associated to the main Report control window.
// Remarks:
//     It has all the functionality of the CEdit control, as well as some
//     specific additions.
//
//     This control should be instantiated in the user's application.
//     To use a field chooser in an existing dialog box of your application,
//     add a standard list box to your dialog template using the dialog
//     editor and then initialize your CXTPReportSubListControl object
//     inside dialog OnInitialUpdate handler. See example below:
// <code>
// // CXTPReportFilterEditControl wndFilter;
// wndFilter.SubclassDlgItem(IDC_FILTEREDIT, &pFilterDialog);
// // CXTPReportControl wndReport;
// wndReport.GetColumns()->GetReportHeader()->SetFilterEditCtrl(&wndFilter);
// </code>
// See Also: CEdit, CXTPReportHeader::SetFilterEditCtrl
//===========================================================================
class _XTP_EXT_CLASS CXTPReportFilterEditControl : public CEdit
{

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPReportFilterEditControl object.
	// Remarks:
	//     You construct a CXTPReportFilterEditControl object in two steps.
	//     First, call the constructor CXTPReportFilterEditControl and
	//     then call Create, which initializes the Windows edit box and
	//     attaches it to the CXTPReportFilterEditControl.
	// Example:
	// <code>
	// // Declare a local CXTPReportFilterEditControl object.
	// CXTPReportFilterEditControl myFilterEdit;
	//
	// // Declare a dynamic CXTPReportFilterEditControl object.
	// CXTPReportFilterEditControl* pMyFilterEdit = new CXTPReportFilterEditControl;
	// </code>
	//
	// See Also: CEdit, SetReportCtrl, CXTPReportHeader::SetFilterEditCtrl
	//-----------------------------------------------------------------------
	CXTPReportFilterEditControl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys CXTPReportFilterEditControl object, handles its cleanup.
	//-----------------------------------------------------------------------
	~CXTPReportFilterEditControl();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the associated report control.
	// Parameters:
	//     pReportCtrl - Pointer to the associated report control.
	// Remarks:
	//     Usually this method should not be called by the user, instead
	//     should call CXTPReportHeader::SetFilterEditCtrl.
	// Returns:
	//     TRUE if set successfully, FALSE otherwise.
	// See Also: CXTPReportHeader::SetFilterEditCtrl, GetReportCtrl
	//-----------------------------------------------------------------------
	BOOL SetReportCtrl(CXTPReportControl* pReportCtrl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns a pointer to the associated report control.
	// Returns:
	//     An associated report control.
	// See Also: CXTPReportHeader::SetFilterEditCtrl, SetReportCtrl
	//-----------------------------------------------------------------------
	CXTPReportControl* GetReportCtrl();

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTPReportFilterEditControl)
	afx_msg BOOL OnChange();
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE


protected:
	CXTPReportControl* m_pReportCtrl;   // Pointer to the main report control.
};


#endif //#if !defined(__XTPREPORTFILTEREDITCONTROL_H__)
