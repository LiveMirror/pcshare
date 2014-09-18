// XTPResource.h
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

#include "XTPVersion.h"

#define  XTP_IDC_HSPLITBAR                9970
#define  XTP_IDC_VSPLITBAR                9971
#define  XTP_IDC_VRESIZE                  9972
#define  XTP_IDC_HAND                     9973

#ifdef _XTP_INCLUDE_COMMANDBARS
#include "CommandBars/XTPCommandBarsResource.h"
#endif //_XTP_INCLUDE_COMMANDBARS

#ifdef _XTP_INCLUDE_PROPERTYGRID
#include "PropertyGrid/XTPPropertyGridResource.h"
#endif //_XTP_INCLUDE_PROPERTYGRID

#ifdef _XTP_INCLUDE_DOCKINGPANE
#include "DockingPane/XTPDockingPaneResource.h"
#endif //_XTP_INCLUDE_DOCKINGPANE

#ifdef _XTP_INCLUDE_CONTROLS
#include "Controls/XTResource.h"
#endif //_XTP_INCLUDE_CONTROLS
