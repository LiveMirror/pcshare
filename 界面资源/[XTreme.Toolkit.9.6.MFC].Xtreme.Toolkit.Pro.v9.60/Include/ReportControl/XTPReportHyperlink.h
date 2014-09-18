// CXTPReportHyperlink.h: interface for the XTPReportHyperlink class.
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
#if !defined(__XTPREPORTRECORDHYPERLINK_H__)
#define __XTPREPORTRECORDHYPERLINK_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     This class represents hyperlinks. It stores first position and length
//     in the text for hyperlink and rectangle for drawing and
//     identify clicks on hyperlink. To create an instance of its class, you
//     simply call constructor with first position and length of the
//     hyperlink's text
//===========================================================================
class _XTP_EXT_CLASS CXTPReportHyperlink : public CCmdTarget
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default CXTPReportHyperlink constructor.
	// Parameters:
	//     nHyperTextBegin - Begin position of the hyperlink item text in the parent item caption.
	//     nHyperTextLen   - Hyperlink item text length.
	//-----------------------------------------------------------------------
	CXTPReportHyperlink(int nHyperTextBegin, int nHyperTextLen);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys CXTPReportHyperlink object. Performs cleanup operations.
	//-----------------------------------------------------------------------
	virtual ~CXTPReportHyperlink();

public:
	CRect       m_rcHyperSpot;      // Hyperlink draw place.
	int         m_nHyperTextBegin;  // Start position of the hyperlink in the item text.
	int         m_nHyperTextLen;    // The length of hyperlink text.
};

#endif // !defined(__XTPREPORTRECORDHYPERLINK_H__)
