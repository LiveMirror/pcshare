// ControlTools.h: interface for the CControlTools class.
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

#if !defined(AFX_CONTROLTOOLS_H__028D2A59_987E_411D_B755_4E966D6A39AE__INCLUDED_)
#define AFX_CONTROLTOOLS_H__028D2A59_987E_411D_B755_4E966D6A39AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CControlTools : public CXTPControlButton
{
public:
	CControlTools();
	virtual ~CControlTools();

	virtual BOOL IsCustomizeDragOverAvail(CXTPCommandBar* pCommandBar, CPoint /*point*/, DROPEFFECT& dropEffect);
	virtual void OnCalcDynamicSize(DWORD /*dwMode*/);


	DECLARE_XTP_CONTROL(CControlTools)

};

#endif // !defined(AFX_CONTROLTOOLS_H__028D2A59_987E_411D_B755_4E966D6A39AE__INCLUDED_)
