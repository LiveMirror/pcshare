// XTResizeRect.cpp: implementation of the CXTResizeRect class.
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

#include "stdafx.h"
#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTResizeRect.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTResizeRect::CXTResizeRect()
{
}

//////////////////////////////////////////////////////////////////////

CXTResizeRect::CXTResizeRect(XT_RESIZE l, XT_RESIZE t, XT_RESIZE r, XT_RESIZE b)
{
	left   = l;
	top    = t;
	right  = r;
	bottom = b;
}

CXTResizeRect& CXTResizeRect::operator = (const RECT& rc)
{
	left   = (XT_RESIZE) rc.left;
	top    = (XT_RESIZE) rc.top;
	right  = (XT_RESIZE) rc.right;
	bottom = (XT_RESIZE) rc.bottom;

	return *this;
}

CXTResizeRect& CXTResizeRect::operator = (const XT_RESIZERECT& rrc)
{
	left   = rrc.left;
	top    = rrc.top;
	right  = rrc.right;
	bottom = rrc.bottom;

	return *this;
}

CXTResizeRect& CXTResizeRect::operator += (const XT_RESIZERECT& rrc)
{
	left   += rrc.left;
	top    += rrc.top;
	right  += rrc.right;
	bottom += rrc.bottom;

	return *this;
}

CXTResizeRect CXTResizeRect::operator & (const XT_RESIZERECT& rrc)
{
	ASSERT(IsNormalized() && ((CXTResizeRect&) rrc).IsNormalized());

	CXTResizeRect rrcI(
		__max(left,   rrc.left),
		__max(top,    rrc.top),
		__min(right,  rrc.right),
		__min(bottom, rrc.bottom));

	// only intersection if resulting rect is normalized
	return (rrcI.IsNormalized()) ? rrcI : CXTResizeRect(0,0,0,0);
}
